Linux
<a name="kAGF5"></a>
## 硬件架构
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1648466512018-40b6e637-88ce-4cc8-a112-e55117a6e5df.png#clientId=u160b99b8-b636-4&from=paste&id=u6adcb415&originHeight=439&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6f81d74c-3f9e-4c10-9e67-b8da795f3da&title=)<br />从硬件架构图中可以看出以下特点：

- 每个 CPU 核都包含各自的 local timer，相互独立。
- 每个 local timer 都支持中断的产生，中断类型为 PPI，即 CPU 的私有中断，GIC 负责分发到指定的 CPU，这些中断都可以用来产生系统事件。local timer的中断为以下四种：
1. Secure Physical Timer event (ID 29，也就是上面device node中的13，29 = 16 + 13)
2. Non-secure Physical Timer event (ID 30，也就是上面device node中的14，30 = 16 + 14)
3. Virtual Timer event (ID 27)
4. Hypervisor Timer event (ID 26)
- 系统中存在一个 always-powered 的域，这个域提供一个 system counter，所有 core 的定时器都是基于这个 system counter 提供的 counter 值，因此理论上所有的 local timer 都是基于同样的时间基准。
- 为什么要强调 system counter 是 always powered，而且要独立出来，这是因为在系统运行期间某些 core 为了节能可能进入睡眠状态，local timer 可能也会因此被关闭，但是系统的时间戳不能丢，以便在特定的时间唤醒 CPU，而且在唤醒之后还能获得正确的时间。同时，system counter 也支持休眠模式，它的休眠不是关闭，而是降频，通常情况下该 timer 的频率是 1~50MHz，假设是以 10MHz 运行，将其降到 1MHz，那么，system counter 每次运行时 counter 不再是加1，而是加 10，这样就不会丢失时间精度。
- system counter 的实现标准为:
1. 至少 56 bits 的宽度。
2. 频率在 1-50MHz。
3. 溢出时间至少在 40 年。
4. arm 没有对精度做出特别要求，不过最低的建议值为24小时, 误差不超过 10s。
5. 从 0 开始计数，正常情况下每一个时钟脉冲加1，节能模式下除外。
- system counter 可以被所有 core 访问，通过总线地址映射的方式，而 local timer 由对应的 CPU core 访问，访问方式则是通过操作 CP15 协处理器。
<a name="sc5TK"></a>
## 软件架构
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1648466512054-bc5fb934-f2f5-4de8-a1da-9e0f1ee72c6b.png#clientId=u160b99b8-b636-4&from=paste&id=u026a2a0a&originHeight=637&originWidth=706&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3c259f01-6028-43bc-a85a-6af9153f826&title=)

1. 最底层是硬件和驱动层，每个cpu core都有自己的cpu local timer，此外SOC内部肯定会有一个用于全局的global counter。
2. 中间层是linux内核层，内核抽象出了时钟源(clocksource), 时钟事件设备(clock_event_device), tick设备(tick_device)用于时间管理。分为左右两部分:
- 右边实现**计时功能**。linux内核有各种time line, 包括real time clock, monotonic clock, monotonic raw clock等。clocksource提供了一个单调增加的计时器产生tick，为timeline提供时钟源。timekeeper是内核提供时间服务的基础模块，负责选择并维护最优的clocksource。
- 左边实现**定时功能**。clock event管理可产生event或是触发中断的定时器，(一般而言，每个CPU形成自己的一个小系统，也就要管理自己的clock event。）tick device是基于clock event设备进行工作的，cpu管理自己的调度、进程统计等是基于tick设备的。低精度timer和高精度timer都是基于tick device生成的定时器设备，关于它们的事件和周期信号的关系在上面的图中有一个大体的介绍。
1. 最上层是linux应用层。基于timekeeping设备的是时间管理的库time lib，基于定时器设备的是定时管理的库timer lib。
<a name="tYG3d"></a>
## 数据结构

- clocksource：来自系统计时的需求，换句话说系统需要知道现在是xx年xx月xx日xx时xx分xx秒xx纳秒。

local timer 的 clocksource 相关的配置信息：
```c
static struct clocksource clocksource_counter = {
 .name = "arch_sys_counter",
 .rating = 400,
 .read = arch_counter_read,
 .mask = CLOCKSOURCE_MASK(56),
 .flags = CLOCK_SOURCE_IS_CONTINUOUS,
};
```

- clock_event_device：来自系统定时的需求（即 timer）。即从当前时间点开始，到xxx纳秒之后通知做某些事情。

local timer 的 clock_event_device 相关的配置信息：
```c
static void __arch_timer_setup(unsigned type,
							   struct clock_event_device *clk)
{
	clk->features = CLOCK_EVT_FEAT_ONESHOT;
	
	if (type == ARCH_TIMER_TYPE_CP15) {
		if (arch_timer_c3stop)
			clk->features |= CLOCK_EVT_FEAT_C3STOP;
		clk->name = "arch_sys_timer";
		clk->rating = 450;
		clk->cpumask = cpumask_of(smp_processor_id());
		clk->irq = arch_timer_ppi[arch_timer_uses_ppi];
		switch (arch_timer_uses_ppi) {
				......
					case ARCH_TIMER_PHYS_NONSECURE_PPI:
			case ARCH_TIMER_HYP_PPI:
				clk->set_state_shutdown = arch_timer_shutdown_phys;
				clk->set_state_oneshot_stopped = arch_timer_shutdown_phys;
				clk->set_next_event = arch_timer_set_next_event_phys;
				break;
			default:
				BUG();
		}
}
```
system counter 的 clock_event_device 相关的配置信息如下所示，充当硬件timer，当CPU进入idle后用来唤醒CPU。
```c
static struct clock_event_device clockevent_sysctr = {
 .name   = "i.MX system counter timer",
 .features  = CLOCK_EVT_FEAT_ONESHOT | CLOCK_EVT_FEAT_DYNIRQ,
 .set_state_oneshot = sysctr_set_state_oneshot,
 .set_next_event  = sysctr_set_next_event,
 .set_state_shutdown = sysctr_set_state_shutdown,
 .rating   = 200,
};
```

- tick_device 是 clock_event_device 的子类。
```c
struct tick_device {
    struct clock_event_device *evtdev;
    enum tick_device_mode mode;
};

tick device的工作模式定义如下：
enum tick_device_mode {
    TICKDEV_MODE_PERIODIC,
    TICKDEV_MODE_ONESHOT,
};

static struct tick_device tick_broadcast_device;
```
<a name="y8u3o"></a>
## local timer 驱动
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1648466512005-91284617-7973-4bd9-9c95-f25a9fcd9c69.png#clientId=u160b99b8-b636-4&from=paste&id=u25396b73&originHeight=1033&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=udb07734b-1535-49d5-a05b-2a8d04472fc&title=)
<a name="dws5O"></a>
## system counter 驱动
当没有进程调度到该 CPU 上执行的时候，swapper进程会将该 CPU 推入到 idle 状态。当 CPU 睡的时候，有可能会关闭 local timer 硬件。这就会导致 local timer 将无法唤醒 CPU。<br />为了在 CPU 进入 idle 后还能被唤醒，有两种方案，一种是通过hrtimer的软件方案，还有一种是硬件方案。这里只讲述硬件方案，一般采用 alway-on 的硬件 timer 作为唤醒源，它不属于任何 CPU，使用 SPI 类型的中断来唤醒 CPU，处理软件 timer。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1648466512046-aca27bde-757f-45de-9144-0503d2e411b9.png#clientId=u160b99b8-b636-4&from=paste&id=uaecdd773&originHeight=451&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9579aff3-c7d7-473e-b402-1223a96e665&title=)
