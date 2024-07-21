Linux<br />在性能优化中，经常会关注 CPU 平均负载这个指标。但如果让你来解释一下什么是平均负载，能说得清楚吗？它跟 CPU 使用率有什么区别？可能很多人都数不清楚，来看看 CPU 平均负载这个指标！<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1663206661281-e433a0f6-b8d6-47ad-a993-78812df6ba29.jpeg)<br />平均负载与 CPU 使用率，到底有啥区别？
<a name="aZS8K"></a>
## Linux 进程状态
要弄明白 CPU 平均负载，还需要从 Linux 进程状态说起。<br />在 Linux 源码的 fs/proc/array.c 文件中，其定义了进程的 7 种状态，如下所示：
```c
/*
* The task state array is a strange "bitmap" of
* reasons to sleep. Thus "running" is zero, and
* you can test for combinations of others with
* simple bit tests.
*/
static const char * const task_state_array[] = {
  "R (running)",        /*   0 */
  "S (sleeping)",        /*   1 */
  "D (disk sleep)",    /*   2 */
  "T (stopped)",        /*   4 */
  "t (tracing stop)",    /*   8 */
  "X (dead)",        /*  16 */
  "Z (zombie)",        /*  32 */
};
```
<a name="uQ9mW"></a>
### 第一种状态：TASK_RUNNING 可执行状态，缩写 R。
该状态表示进程可以在 CPU 上运行，即具备运行的条件。但同一时刻可能有多个进程可以运行，但并不代表该进程已经在运行。处于 TASK_RUNNING 的进程会被放入 CPU 的可执行队列中，随后会被进程调度器分配到其中一个 CPU 上 运行。<br />很多操作系统教科书将 CPU 上执行的进程定义为 RUNNING 状态，而将可执行但是尚未被调度执行的进程定义为 READY 状态，这两种状态在 Linux 系统下统一为 TASK_RUNNING 状态。
<a name="BRT1K"></a>
### 第二种状态：TASK_INTERRUPTIBLE 可中断的睡眠状态，缩写 S。
该状态的进程表示因为等待某某时间的发生而被挂起，例如：等待 socket 连接、等待信号量等等。这些进程会被放入对应事件的等待队列中，当这些事件发生时，对应等待队列中的一个或多个进程将被唤醒。<br />通过 ps 命令可以看到，绝大多数进程都处于 TASK_INTERRUPTIBLE 状态。这是因为由于 CPU 只有那么几个，而进程却动辄几百上千，因此绝大多数进程在某个时刻都是处于睡眠状态的。如果不是绝大多数进程都在睡眠，CPU 是无法响应得过来的。
<a name="oQDH8"></a>
### 第三种状态：TASK_UNINTERRUPTIBLE 不可中断睡眠状态，缩写 D。
该状态与 TASK_INTERRUPTIBLE 状态类似，进程处于睡眠状态，但唯一不同的点是该进程是不可中断的。不可中断指的并不是 CPU 不响应外部硬件的中断，而是指进程不响应异步信号。<br />绝大多数情况下，进程处在睡眠状态时，总是应该能够响应异步信号的。即能够通过 `kill -15 pid` 方式传递异步信号，程序可以做出响应。但有时候你会惊奇地发现，`kill -9` 竟然杀不死一个正在睡眠的进程了，这时候有可能就是该进程处于不可中断睡眠状态了！<br />TASK_UNINTERRUPTIBLE 状态存在的意义就在于：内核的某些处理流程是不能被打断的，例如：在进程对某些硬件进行操作时，如果产生中断的话会导致进程与硬件设备交互被打断，使得设备陷入不可控状态。这种情况下的 TASK_UNINTERRUPTIBLE 状态总是非常短暂的，通过 ps 命令基本上不可能捕捉到。
<a name="RNcja"></a>
### 第四、五种状态：TASK_STOPPED 暂停状态、TASK_TRACED 状态，缩写 T。
当向进程发送一个 SIGSTOP 信号时，它就会因响应该信号而进入 TASK_STOPPED 状态。除非该进程本身处于 TASK_UNINTERRUPTIBLE 状态而不响应信号）。向进程发送一个 SIGCONT 信号，可以让其从 TASK_STOPPED 状态恢复到 TASK_RUNNING 状态。
<a name="vpPSA"></a>
### 第六种状态：TASK_DEAD - EXIT_ZOMBIE 退出状态，缩写 Z。
进程退出之后，进程所占有的所有资源将被回收，随后该进程就被成为僵尸进程（Zombie）。一般情况下是子进程先于父进程退出，并且父进程没有调用 wait 或 waitpid 回收子进程。此时子进程即处于僵尸状态。
<a name="PwXej"></a>
### 第七种状态：TASK_DEAD - EXIT_DEAD 退出状态，缩写 X。
进程被置于 EXIT_DEAD 退出状态，这意味着接下来的代码立即就会将该进程彻底释放。一般情况下 EXIT_DEAD 状态是非常短暂的，几乎不可能通过 ps 命令捕捉到。<br />看完了 Linux 进程的 7 种状态，是不是有点懵了？**其实只需要记住 Linux 有这 7 种状态，其中最重要的是 RUNNING 状态、UNINTERRUPTIBLE 状态就可以了。**<br />总结一下 Linux 的 7 种进程状态：

- **TASK_RUNNING 可执行状态，缩写 R。**
- **TASK_INTERRUPTIBLE 可中断的睡眠状态，缩写 S。**
- **TASK_UNINTERRUPTIBLE 不可中断睡眠状态，缩写 D。**
- **TASK_STOPPED 暂停状态、TASK_TRACED 状态，缩写 T。**
- **TASK_DEAD - EXIT_ZOMBIE 退出状态，缩写 Z。**
- **TASK_DEAD - EXIT_DEAD 退出状态，缩写 X。**
<a name="tQkes"></a>
## 平均负载的定义
**平均负载的定义是：单位时间内，系统中处于可运行状态和不可中断状态的平均进程数。** 这里的可运行状态和不可中断状态，指的就是上文说到的进程状态。从平均负载的定义来看，其与进程所处的状态有关系，因此后续分析平均负载的时候，要以该定义为基础去分析。<br />对于有 4 核 CPU 的机器，如果一共运行了 4 个进程，那么每个 CPU 都运行了 1 个进程，此时所有的 CPU 都刚好被完全占用。<br />而如果只有 2 个进程，那么意味着 CPU 有 50% 的空闲。而如果有 8 个进程，那么意味着 CPU 超载了，平均负载达到了 2，但单位时间内单个 CPU 需要运行 2 个进程。<br />可以通过读取 /proc/cpuinfo 文件获取系统 CPU 信息，如下所示：
```bash
$ grep 'model name' /proc/cpuinfo | wc -l2
```
当平均负载比 CPU 个数还多的时候，就表示系统已经出现了负载。一般情况下，负载不超过 70% 的情况下都是正常的。<br />很多人都会将 CPU 平均负载与 CPU 使用率搞混，实际上它们有一定关联，但不是同一个东西。<br />平均负载是指单位时间内，处于可运行状态和不可中断的进程数。**从其定义可以知道，其不仅包括了正在使用 CPU 的进程，还包括等待 CPU 和等待 I/O 的进程。而 CPU 使用率指的是正在使用 CPU 的进程，由此可见它们两者是不同的。**<br />如果是 CPU 密集型的进程，因为进程大量使用 CPU，因此平均负载会上升，CPU 使用率会上升。但如果是 I/O 密集型进程，有很多进程在等待 I/O 操作，此时进程处于不可中断状态，因此平均负载会升高，但是 CPU 使用率却不一定很高。<br />**由此可见，平均负载与 CPU 使用率有一定关联，但并没有绝对的关系。**
<a name="vbEVZ"></a>
## 如何查看平均负载？
一般来说，可以通过 top 和 uptime 命令来监控服务器的平均负载。<br />在服务器命令行输入 top 即可查看到当前系统的负载情况，如下图所示。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1663206100472-36c7b811-8639-4b89-ac02-ce263a6a3e81.jpeg#clientId=u6206e291-8b3b-4&from=paste&id=u425773e3&originHeight=431&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0fdd673e-4694-4f48-b1f6-fcf5f896d4e&title=)<br />上图中平均负载的 3 个数值分别代表 1 分钟、5 分钟、15 分钟系统的平均负载情况。通过这三个数值的变化，可以知道系统最近一段时间的压力变化趋势。例如：load average: 15.00, 10.75, 3.25 表示过去 1 分钟负载为 15，过去 5 分钟负载为 10.75，过去 15 分钟负载为 3.25，可以看到其平均负载压力是越来越大的。<br />top 命令输出的信息非常多，有时候会干扰我们的视野。所以如果只需要看系统负载情况，那么可以用 uptime 命令，如下图所示。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1663206100548-b145372f-8c07-47c2-9e9a-0c096b22ece8.jpeg#clientId=u6206e291-8b3b-4&from=paste&id=u53d0cec3&originHeight=112&originWidth=876&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u80db3bf0-b0d8-4df6-9629-9cbe26fcf63&title=)<br />uptime 命令只输出了一行信息，非常简洁。<br />如果需要持续地查看平均负载的变化，那么可以用如下命令。该命令会会持续输出最新的负载信息，并高亮变化的部分。
```bash
watch -d uptime
```
![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1663206100468-921acc37-cc11-4f5e-b273-13276b90910a.jpeg#clientId=u6206e291-8b3b-4&from=paste&id=ueba39830&originHeight=134&originWidth=998&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u78f3b1e9-7844-4257-b8e6-9df8c7eb492&title=)
<a name="onMyA"></a>
## 总结
首先介绍了 Linux 进程的 7 种状态，分别是：

- **TASK_RUNNING 可执行状态，缩写 R。**
- **TASK_INTERRUPTIBLE 可中断的睡眠状态，缩写 S。**
- **TASK_UNINTERRUPTIBLE 不可中断睡眠状态，缩写 D。**
- **TASK_STOPPED 暂停状态、TASK_TRACED 状态，缩写 T。**
- **TASK_DEAD - EXIT_ZOMBIE 退出状态，缩写 Z。**
- **TASK_DEAD - EXIT_DEAD 退出状态，缩写 X。**

随后，介绍了平均负载指标的定义，即：**单位时间内，系统中处于可运行状态和不可中断状态的平均进程数。**<br />接着，将其与 CPU 使用率做了对比，知道两者的区别在于：**平均负载不仅包括了正在使用 CPU 的进程，还包括等待 CPU 和等待 I/O 的进程。而 CPU 使用率指的是正在使用 CPU 的进程，由此可见它们两者是不同的。**<br />最后，介绍了查看平均负载指标的 2 个命令，即 top 和 uptime 命令。如果需要持续关注平均负载的变化，那么可以使用 `watch -d uptime` 命令持续输出，并高亮变化的部分。
