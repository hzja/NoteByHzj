在使用 Kubernetes（简称K8s） 时，通常会在同一台机器上部署多个 Pod。如果某个 Pod 中的服务出现问题（如出现死循环），将会导致占用大量的 CPU 时间，从而影响到其他 Pod 的正常运行。<br />为了解决这个问题，K8s 提供了一个限制 Pod 使用 CPU 资源的配置项，如下所示：
```yaml
resources:
  limits:
    cpu: "0.5"
```
上述配置限制了 Pod 只能使用 0.5 个 CPU 资源。<br />K8s 通过使用 Linux 资源控制组（cgroup）中的 CPU子系统 来限制 Pod 对 CPU 资源的使用。<br />下面来分析一下 Linux 内核是如何限制进程对 CPU 资源的使用。
<a name="NL68j"></a>
## CPU限流原理
如果来设计一个限制进程对 CPU 资源使用的算法（如限制进程 A 只能使用 10% 的 CPU 运行时间），应该如何实现呢？<br />最简单的方法是，将 CPU 的运行时间划分成一个个时间片段（称为 周期(period)，如 100 毫秒）。由于进程 A 被限制为只能使用 10% 的 CPU 运行时间，所以在一个周期内，进程 A 只能获得 10 毫秒的运行时间。当进程 A 在一个周期内运行超过 10 毫秒后，将会被内核移除可运行队列。那么在当前周期内，进程 A 将不会被调度，从而实现 CPU限流 的目的。<br />当一个新的周期开始时，进程 A 将会重新获得 10% 的 CPU 运行时间。如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680744063360-abb234e7-be8a-4178-976e-94890e3d4d33.png#averageHue=%23faf9f9&clientId=u72667af2-8c10-4&from=paste&id=u0a354cd9&originHeight=440&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u6993465e-98c2-492e-bd67-5522e196e21&title=)<br />进程在一个周期内能运行的时间被称为 配额(quota)。
<a name="VqFa6"></a>
## CPU限流实现
内核以 进程组 作为资源控制的主体，进程组使用 task_group 结构体来描述，其定义如下：
```c
struct task_group {
    ...
    // 可运行队列，每个CPU一个
    struct cfs_rq **cfs_rq;
    ...
    // 用于限制进程组对CPU资源的使用
    struct cfs_bandwidth cfs_bandwidth;
};
```
可以看出，每个进程组都有一个可运行队列，可运行队列中包含了进程组中所有可以被调度的进程。<br />在多核环境下，每个 CPU 都有一个可运行队列，主要为了解决资源竞争问题。<br />另外，task_group 结构体中还有个类型为 cfs_bandwidth 的字段，用于控制进程组对 CPU 资源的使用。cfs_bandwidth 结构体的定义如下：
```c
struct cfs_bandwidth {
    ...
    ktime_t period;
    u64 quota;
    u64 runtime;
    ...
    struct hrtimer period_timer;
};
```
下面介绍一下 cfs_bandwidth 结构体各个字段的作用：

- `period`：就是上面介绍的周期，用户可以通过修改 cgroup 的 `cpu.cfs_period_us` 文件进行设置。
- `quota`：进程组在周期内能够运行的时间，用户可以通过修改 cgroup 的 `cpu.cfs_quota_us` 文件进行设置。
- `runtime`：进程组在周期内剩余的可运行时间。
- `period_timer`：定时器，每隔一个周期执行一次，主要用于更新 runtime 字段的值。

runtime 字段用于保存进程组在当前周期内剩余的可运行时间，如果调度器选中了进程组中某个进程进行运行时，将会减少进程组的剩余可运行时间。如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680744063398-b8cf1854-8e8c-4481-8950-2825373625f3.png#averageHue=%23fceec3&clientId=u72667af2-8c10-4&from=paste&id=uc43ea3c1&originHeight=564&originWidth=776&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7b63fdff-0de5-451f-bc10-ff6029f1ca8&title=)<br />在上图中，进程组 A 中的进程 D 被调度器选中运行。如果进程组 A 原来的可运行时间为 50 毫秒，而进程 D 运行了 10 毫秒。那么，进程组 A 的可运行时间将从会减少 10 毫秒，从而变为 40 毫秒。<br />在一个周期内，当进程组的可运行时间变为 0 时，那么此进程组将会被限制运行（称为 CPU Throttling），直到下一个周期开始。<br />当进程组被限制运行时，进程组内的所有进程都不能被执行。<br />当进程组开启了 CPU 限流功能时（也就是设置了 period 和 quota 的值），内核将会为其启动一个定时器。定时器每隔一个周期触发一次，用于更新进程组的可运行时间，从而解除进程组被限制运行的情况。<br />定时器通过调用 `__refill_cfs_bandwidth_runtime()` 函数来更新进程组的可运行时间，其代码如下：
```c
void __refill_cfs_bandwidth_runtime(struct cfs_bandwidth *cfs_b)
{
    ...
    cfs_b->runtime = cfs_b->quota;
    ...
}
```
上面的代码将 quota 字段的值赋给了 runtime 字段，所以进程组重新获得了可运行时间，从而解除被限制运行的状态。
