Linux<br />CPU 上下文切换是保证 Linux 系统正常运行的核心功能。可分为**进程上下文切换**、**线程上下文切换**和**中断上下文切换**。<br />将进一步讨论如何分析 CPU 上下文切换问题。
<a name="a20Ii"></a>
## 检查 CPU 的上下文切换
过多的上下文切换会消耗 CPU 的时间来保存和恢复寄存器、程序计数器、内核栈和虚拟内存等数据，从而导致系统性能显着下降。<br />既然**上下文切换**对系统性能的影响如此之大，那么如何检查它呢？好了，可以使用 vmstat 工具来查询系统的上下文切换。
<a name="LUVSu"></a>
### vmstat
vmstat 是一种常用的系统性能分析工具。主要用于分析内存使用情况，也常用于分析 CPU 上下文切换和中断的次数。<br />例如 vmstat 5（5 秒输出间隔）：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661387028487-d183ee81-52b4-4403-8e0a-af16a3a00fc9.png#clientId=u30b6a249-fef3-4&from=paste&id=ubd6ca263&originHeight=81&originWidth=568&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7ea71467-10d8-44a9-a9c5-59e7fb418fe&title=)<br />看一下输出：

- cs（context switch）：每秒上下文切换的次数。
- in（interrupt）：每秒的中断数。
- r（running | runnable）：就绪队列的长度，即正在运行和等待 CPU 的进程数。
- b（blocked）：处于不间断睡眠状态的进程数。

在上面的例子中，可以看到上下文切换次数为 33 次，系统中断次数为 25 次，就绪队列长度，不间断状态进程数均为 0。
<a name="WDRaZ"></a>
### pidstat
vmstat 工具只给出了系统的整体上下文切换的信息。要查看每个进程的详细信息，需要使用 pidstat。添加 `-w` 选项，可以看到每个进程的上下文切换：<br />例如：
```bash
# Output interval is 5
$ pidstat -w 5
Linux 4.15.0 (ubuntu)  09/23/18  _x86_64_  (2 CPU)
08:18:26      UID       PID   cswch/s nvcswch/s  Command
08:18:31        0         1      0.20      0.00  systemd
08:18:31        0         8      5.40      0.00  rcu_sched
...
```
结果中有两列需要注意：cswch 和 nvcswch。其中，cswch 表示每秒**自愿上下文切换**的次数，nvcswch 表示每秒**非自愿上下文切换**的次数。

- **自愿上下文切换**：指进程无法获得所需资源而导致的上下文切换。例如，当 I/O 和内存等系统资源不足时，就会发生自愿上下文切换。
- **非自愿上下文切换**：指进程因时间片已过期而被系统强制重新调度时发生的上下文切换。例如，当大量进程竞争 CPU 时，很容易发生非自愿的上下文切换。

必须牢记这两个概念，因为它们意味着不同的性能问题。
<a name="Pambw"></a>
## 案例分析
既然知道如何查看这些指标，那么就会出现另一个问题，上下文切换频率多久才是正常的呢？看一个示例案例。<br />将使用 sysbench ([https://github.com/akopytov/sysbenc](https://github.com/akopytov/sysbenc))，一个多线程的基准测试工具通过生成负载来模拟上下文切换过多的问题。假设已经在 Linux 系统上安装了 sysbench 和 sysstat。<br />在模拟负载之前，在一个终端中运行一下 vmstat：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661387028507-a889cd7c-2f36-47ad-a2ee-119531c052c0.png#clientId=u30b6a249-fef3-4&from=paste&id=uc55f0dba&originHeight=103&originWidth=568&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uebc6efc9-8db4-45e5-b79f-08c614bc6bd&title=)<br />在这里可以看到当前的上下文切换次数 cs 是 35，中断次数 in 是 19，r 和 b 都是 0。由于目前没有其他任务在运行，因此它们是**空闲系统**中的**上下文切换数量**。<br />现在运行 sysbench 来模拟**多线程调度系统**的瓶颈：
```bash
$ sysbench --threads=10 --max-time=300 threads run
```
现在，应该会看到 vmstat 输出了与上面不同的结果：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661387028512-518ee396-7b79-4aa8-8bf3-96965d7e2aca.png#clientId=u30b6a249-fef3-4&from=paste&id=u61006e60&originHeight=103&originWidth=571&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u17399098-94be-497b-ae72-387105bbed2&title=)<br />应该可以发现 cs 栏的上下文切换次数从之前的 35 次突增到 139 万次。同时，注意观察其他几个指标：

- r：就绪队列的长度已达到 8
- us 和 sy：us 和 sy 的 CPU 使用率加起来是 100%，系统 CPU 使用率是 84%，说明 CPU 主要被内核占用。
- in：中断数也上升到了 10000，说明中断处理也是一个潜在的问题。

结合这些指标可以知道系统的**就绪队列太长**了，也就是有太多的进程在运行等待 CPU，导致大量的上下文切换，而大量的上下文切换导致了系统 CPU 使用率的增长。<br />那么是什么过程导致了这些问题呢？<br />继续分析，同时在第三个终端使用 pidstat，看看 CPU 和进程上下文切换的情况：
```bash
# 1 means output interval is 1 second
# -w: output process switching index，
# -u: output CPU usage index
$ pidstat -w -u 1
08:06:33      UID       PID    %usr %system  %guest   %wait    %CPU   CPU  Command
08:06:34        0     10488   30.00  100.00    0.00    0.00  100.00     0  sysbench
08:06:34        0     26326    0.00    1.00    0.00    0.00    1.00     0  kworker/u4:2
08:06:33      UID       PID   cswch/s nvcswch/s  Command
08:06:34        0         8     11.00      0.00  rcu_sched
08:06:34        0        16      1.00      0.00  ksoftirqd/1
08:06:34        0       471      1.00      0.00  hv_balloon
08:06:34        0      1230      1.00      0.00  iscsid
08:06:34        0      4089      1.00      0.00  kworker/1:5
08:06:34        0      4333      1.00      0.00  kworker/0:3
08:06:34        0     10499      1.00    224.00  pidstat
08:06:34        0     26326    236.00      0.00  kworker/u4:2
08:06:34     1000     26784    223.00      0.00  sshd
```
从 pidstat 的输出可以发现，CPU 使用率的增加确实是 sysbench 造成的，它的 CPU 使用率已经达到了 100%。但上下文切换来自其他进程，包括**非自愿上下文切换**频率最高的 pidstat，以及**自愿上下文切换**频率最高的内核线程 kworker 和 sshd。<br />注意：默认情况下 pidstat 只显示进程的上下文切换，如果要查看实际线程的上下文切换，请添加 -t 选项。
<a name="HoydE"></a>
### 中断
要找出中断数量也很高的原因所在，可以检查 /proc/interrupts 文件。该文件会提供一个只读的中断使用情况。
```bash
# -d: Highlight the change area
$ watch -d cat /proc/interrupts
           CPU0       CPU1
...
RES:    2450431    5279697   Rescheduling interrupts
...
```
观察一段时间后，可以发现变化最快的是**重新调度中断**（RES, REScheduling interrupt）。这种中断类型表明处于空闲状态的 CPU 被唤醒以调度新的任务运行。所以这里的中断增加是因为太多的任务调度问题，这和前面上下文切换次数的分析结果是一致的<br />现在回到最初的问题，每秒多少次上下文切换是正常的？<br />这个值实际上取决于系统本身的 CPU 性能。如果系统的上下文切换次数比较稳定的话，几百到一万应该是正常的。但是，当上下文切换次数超过 10000，或者切换次数快速增加时，很可能是出现了性能问题。
<a name="CjTnd"></a>
## 结论
此时，应该可以根据上下文切换的类型做一些具体的分析了。

- **自愿上下文切换较多**，说明进程在等待资源，可能会出现 I/O 饱和等其他问题。
- **非自愿上下文切换较多**，说明进程正在被强制调度，也就是都在争抢 CPU，说明 CPU 确实产生了瓶颈。
- **中断次数增多**，说明 CPU 被中断处理程序占用，需要通过查看 /proc/interrupts 文件来分析具体的中断类型。
