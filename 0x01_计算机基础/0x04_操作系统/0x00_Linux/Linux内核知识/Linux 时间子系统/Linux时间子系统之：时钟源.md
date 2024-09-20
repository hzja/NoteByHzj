Linux<br />clock source 用于为 Linux 内核提供一个时间基线，如果用 Linux 的 date 命令获取当前时间，内核会读取当前的 clock source，转换并返回合适的时间单位给用户空间。<br />在硬件层，它通常实现为一个由固定时钟频率驱动的计数器，计数器只能单调地增加，直到溢出为止。时钟源是内核计时的基础，系统启动时，内核通过硬件RTC获得当前时间，在这以后，在大多数情况下，内核通过选定的时钟源更新实时时间信息（墙上时间），而不再读取RTC的时间。<br />本节的内核代码树基于V3.4.10。
<a name="h9ZN7"></a>
## 1、struct clocksource结构
内核用一个clocksource结构对真实的时钟源进行软件抽象，现在从clock source的数据结构开始，它的定义如下：
```c
struct clocksource {
 /*
  * Hotpath data, fits in a single cache line when the
  * clocksource itself is cacheline aligned.
  */
 cycle_t (*read)(struct clocksource *cs);
 cycle_t cycle_last;
 cycle_t mask;
 u32 mult;
 u32 shift;
 u64 max_idle_ns;
 u32 maxadj;
#ifdef CONFIG_ARCH_CLOCKSOURCE_DATA
 struct arch_clocksource_data archdata;
#endif
 
 const char *name;
 struct list_head list;
 int rating;
 int (*enable)(struct clocksource *cs);
 void (*disable)(struct clocksource *cs);
 unsigned long flags;
 void (*suspend)(struct clocksource *cs);
 void (*resume)(struct clocksource *cs);
 
 /* private: */
#ifdef CONFIG_CLOCKSOURCE_WATCHDOG
 /* Watchdog related data, used by the framework */
 struct list_head wd_list;
 cycle_t cs_last;
 cycle_t wd_last;
#endif
} ____cacheline_aligned;
```
只关注clocksource中的几个重要的字段。
<a name="ovNZ8"></a>
### 1.1 rating：时钟源的精度
同一个设备下，可以有多个时钟源，每个时钟源的精度由驱动它的时钟频率决定，比如一个由10MHz时钟驱动的时钟源，他的精度就是100nS。clocksource结构中有一个rating字段，代表着该时钟源的精度范围，它的取值范围如下：

- 1--99：不适合于用作实际的时钟源，只用于启动过程或用于测试；
- 100--199：基本可用，可用作真实的时钟源，但不推荐；
- 200--299：精度较好，可用作真实的时钟源；
- 300--399：很好，精确的时钟源；
- 400--499：理想的时钟源，如有可能就必须选择它作为时钟源；
<a name="zx0W5"></a>
### 1.2 read回调函数
时钟源本身不会产生中断，要获得时钟源的当前计数，只能通过主动调用它的read回调函数来获得当前的计数值，注意这里只能获得计数值，也就是所谓的cycle，要获得相应的时间，必须要借助clocksource的mult和shift字段进行转换计算。
<a name="uwobl"></a>
### 1.3 mult和shift字段
因为从clocksource中读到的值是一个cycle计数值，要转换为时间，必须要知道驱动clocksource的时钟频率F，一个简单的计算就可以完成：
```c
t = cycle / F;
```
可是clocksource并没有保存时钟的频率F，因为使用上面的公式进行计算，需要使用浮点运算，这在内核中是不允许的。<br />因此，内核使用了另外一个变通的办法，根据时钟的频率和期望的精度，事先计算出两个辅助常数mult和shift，然后使用以下公式进行cycle和t的转换：
```c
t = (cycle * mult) >> shift;
```
只要保证：
```c
F = (1 << shift) / mult;
```
内核内部使用64位进行该转换计算：
```c
static inline s64 
clocksource_cyc2ns(cycle_t cycles, u32 mult, u32 shift)
{
    return ((u64) cycles * mult) >> shift;
}
```
从转换精度考虑，mult 的值是越大越好，但是为了计算过程不发生溢出，mult 的值又不能取得过大。为此内核假设 cycle 计数值被转换后的最大时间值：10分钟（600秒），主要的考虑是 CPU 进入 IDLE 状态后，时间信息不会被更新，只要在10分钟内退出IDLE，clocksource 的 cycle 计数值就可以被正确地转换为相应的时间，然后系统的时间信息可以被正确地更新。<br />当然最后的结果不一定是10分钟，它由 clocksource_max_deferment 进行计算，并保存 max_idle_ns 字段中，tickless 的代码要考虑这个值，以防止在 NO_HZ 配置环境下，系统保持 IDLE 状态的时间过长。在这样，由 10 分钟这个假设的时间值，可以推算出合适的 mult 和 shift 值。
<a name="MMUcT"></a>
## 2、clocksource的注册和初始化
通常，clocksource要在初始化阶段通过 clocksource_register_hz 函数通知内核它的工作时钟的频率，调用的过程如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1672018268736-39af49b2-eba4-4fb2-ad28-c33ac7332a5f.png#averageHue=%23f7f6f4&clientId=u0982e2af-e537-4&from=paste&id=ue6f65a7a&originHeight=616&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue53496cb-e743-48da-9549-537cd29b1cf&title=)<br />由上图可见，最终大部分工作会转由 __clocksource_register_scale 完成，该函数首先完成对 mult 和 shift 值的计算，然后根据 mult 和 shift 值，最终通过 clocksource_max_deferment 获得该 clocksource 可接受的最大 IDLE 时间，并记录在 clocksource 的 max_idle_ns 字段中。clocksource_enqueue 函数负责按 clocksource 的 rating 的大小，把该 clocksource 按顺序挂在全局链表 clocksource_list 上，rating 值越大，在链表上的位置越靠前。<br />每次新的 clocksource 注册进来，都会触发 clocksource_select 函数被调用，它按照 rating 值选择最好的 clocksource，并记录在全局变量 curr_clocksource 中，然后通过 timekeeping_notify 函数通知 timekeeping，当前clocksource 已经变更，关于 timekeeping，将会在后续的博文中阐述。
<a name="WYfyd"></a>
## 3、clocksource watchdog
系统中可能同时会注册对个 clocksource，各个 clocksource 的精度和稳定性各不相同，为了筛选这些注册的 clocksource，内核启用了一个定时器用于监控这些 clocksource 的性能，定时器的周期设为0.5秒：
```c
#define WATCHDOG_INTERVAL (HZ >> 1)
#define WATCHDOG_THRESHOLD (NSEC_PER_SEC >> 4)
```
当有新的 clocksource 被注册时，除了会挂在全局链表 clocksource_list 外，还会同时挂在一个 watchdog 链表上：watchdog_list。<br />定时器周期性地（0.5秒）检查 watchdog_list 上的 clocksource，WATCHDOG_THRESHOLD 的值定义为0.0625秒。如果在0.5秒内，clocksource 的偏差大于这个值就表示这个 clocksource 是不稳定的，定时器的回调函数通过 clocksource_watchdog_kthread 线程标记该 clocksource，并把它的 rate 修改为0，表示精度极差。
<a name="CNOFR"></a>
## 4、建立clocksource的简要过程
在系统的启动阶段，内核注册了一个基于 jiffies 的 clocksource，代码位于 kernel/time/jiffies.c：
```c
struct clocksource clocksource_jiffies = {
 .name  = "jiffies",
 .rating  = 1, /* lowest valid rating*/
 .read  = jiffies_read,
 .mask  = 0xffffffff, /*32bits*/
 .mult  = NSEC_PER_JIFFY << JIFFIES_SHIFT, /* details above */
 .shift  = JIFFIES_SHIFT,
};
......
 
static int __init init_jiffies_clocksource(void)
{
 return clocksource_register(&clocksource_jiffies);
}
 
core_initcall(init_jiffies_clocksource);
```
它的精度只有 1/HZ 秒，rating 值为1，如果平台的代码没有提供定制的 clocksource_default_clock 函数，它将返回该 clocksource：
```c
struct clocksource * __init __weak clocksource_default_clock(void)
{
 return &clocksource_jiffies;
}
```
然后，在初始化的后段，clocksource 的代码会把全局变量 curr_clocksource 设置为上述的 clocksource：
```c
static int __init clocksource_done_booting(void)
{
        ......
 curr_clocksource = clocksource_default_clock();
        ......
 finished_booting = 1;
        ......
 clocksource_select();
        ......
 return 0;
}
fs_initcall(clocksource_done_booting);
```
当然，如果平台级的代码在初始化时也会注册真正的硬件 clocksource，所以经过 clocksource_select 函数后，curr_clocksource 将会被设为最合适的 clocksource。如果 clocksource_select 函数认为需要切换更好的时钟源，它会通过 timekeeping_notify 通知 timekeeping 系统，使用新的 clocksource 进行时间计数和更新操作。
