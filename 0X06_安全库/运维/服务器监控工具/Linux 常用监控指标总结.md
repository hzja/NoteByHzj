Linux 性能监控指标
<a name="WqYdG"></a>
### 1. Linux运维基础采集项
在系统运维过程中，经常会参考的一些指标，主要包括以下几个类别：

- CPU<br />
- Load<br />
- 内存<br />
- 磁盘<br />
- IO<br />
- 网络相关<br />
- 内核参数<br />
- ss 统计输出<br />
- 端口采集<br />
- 核心服务的进程存活信息采集<br />
- 关键业务进程资源消耗<br />
- NTP offset采集<br />
- DNS解析采集<br />

每个类别，具体的详细指标如下，这些指标，都是open-falcon的agent组件直接支持的。falcon-agent每隔一定时间间隔（目前是60秒）会采集一次相关的指标，并汇报给server端。
<a name="benS1"></a>
### 2. CPU相关采集项
计算方法：通过采集/proc/stat来得到，大家可以参考sar命令的统计输出来理解。

- cpu.idle：Percentage of time that the CPU or CPUs were idle and the system did not have an outstanding disk I/O request.<br />
- cpu.busy：与cpu.idle相对，他的值等于100减去cpu.idle。<br />
- cpu.guest：Percentage of time spent by the CPU or CPUs to run a virtual processor.<br />
- cpu.iowait：Percentage of time that the CPU or CPUs were idle during which the system had an outstanding disk I/O request.<br />
- cpu.irq：Percentage of time spent by the CPU or CPUs to service hardware interrupts.<br />
- cpu.softirq：Percentage of time spent by the CPU or CPUs to service software interrupts.<br />
- cpu.nice：Percentage of CPU utilization that occurred while executing at the user level with nice priority.<br />
- cpu.steal：Percentage of time spent in involuntary wait by the virtual CPU or CPUs while the hypervisor was servicing another virtual processor.<br />
- cpu.system：Percentage of CPU utilization that occurred while executing at the system level (kernel).<br />
- cpu.user：Percentage of CPU utilization that occurred while executing at the user level (application).<br />
- cpu.cnt：cpu核数。<br />
- cpu.switches：cpu上下文切换次数，计数器类型。<br />
<a name="4R58B"></a>
### 3. 磁盘相关采集项
计算方法：先读取/proc/mounts拿到所有挂载点，然后通过syscall.Statfs_t拿到blocks和inode的使用情况。每个metric都会附加一组tag描述，类似mount=$mount,fstype=$fstype，其中$mount是挂载点，比如/home，$fstype是文件系统，比如ext4。

- df.bytes.free：磁盘可用量，int64<br />
- df.bytes.free.percent：磁盘可用量占总量的百分比，float64，比如32.1<br />
- df.bytes.total：磁盘总大小，int64<br />
- df.bytes.used：磁盘已用大小，int64<br />
- df.bytes.used.percent：磁盘已用大小占总量的百分比，float64<br />
- df.inodes.total：inode总数，int64<br />
- df.inodes.free：可用inode数目，int64<br />
- df.inodes.free.percent：可用inode占比，float64<br />
- df.inodes.used：已用的inode数据，int64<br />
- df.inodes.used.percent：已用inode占比，float64<br />
<a name="aQ81j"></a>
### 4. megacli工具输出
使用 megacli 工具读取 RAID 相关信息，每个metric都会附件一组tag描述，用来标明所属PD或者 VD，PD格式为PD=Enclosure_ID:SLOT_ID，比如PD=32:0表明第一块磁盘 ，VD=0 表明第一个逻辑磁盘。

- sys.disk.lsiraid.pd.Media_Error_Count：这个及以下三个指标目前仅作为数据收集，不一定意味磁盘损坏（只是表示损坏概率变大）<br />
- sys.disk.lsiraid.pd.Other_Error_Count<br />
- sys.disk.lsiraid.pd.Predictive_Failure_Count<br />
- sys.disk.lsiraid.pd.Drive_Temperature<br />
- sys.disk.lsiraid.pd.Firmware_state：如果值不为0，则此物理磁盘出现问题<br />
- sys.disk.lsiraid.vd.cache_policy：如果值不为0，表示此逻辑磁盘缓存策略和设置不符<br />
- sys.disk.lsiraid.vd.state：如果值不为0，表示此逻辑磁盘出现问题<br />
<a name="FgsKL"></a>
### 5. SMART工具输出
使用 smartctl 工具读取磁盘 SMART 信息，目前所有指标仅作为数据收集，不一定意味磁盘损坏（只是表示概率变大），每个metric都会有一组tag描述，表明盘符，例如device=/dev/sda。

- sys.disk.smart.Reallocated_Sector_Ct<br />
- sys.disk.smart.Spin_Retry_Count<br />
- sys.disk.smart.Reallocated_Event_Count<br />
- sys.disk.smart.Current_Pending_Sector<br />
- sys.disk.smart.Offline_Uncorrectable<br />
- sys.disk.smart.Temperature_Celsius<br />
<a name="CpSJ2"></a>
### 6. 分区读写监控
测试所有已挂载分区是否可读写，每个metric都会有一组tag描述，表示挂载点，比如mount=/home

- sys.disk.rw：如果值不为0，表明此分区读写出现问题<br />
<a name="lefGl"></a>
### 7. IO相关采集项
计算方法：每秒采集一次/proc/diskstats，计算差值，都是计数器类型的。每个metric都会有一组tag描述，形如device=$device，用来表示具体的设备，比如sda1、sdb。用户可以参考iostat的帮助文档来理解具体的metric含义。

- disk.io.ios_in_progress：Number of actual I/O requests currently in flight.<br />
- disk.io.msec_read：Total number of ms spent by all reads.<br />
- disk.io.msec_total：Amount of time during which ios_in_progress >= 1.<br />
- disk.io.msec_weighted_total：Measure of recent I/O completion time and backlog.<br />
- disk.io.msec_write：Total number of ms spent by all writes.<br />
- disk.io.read_merged：Adjacent read requests merged in a single req.<br />
- disk.io.read_requests：Total number of reads completed successfully.<br />
- disk.io.read_sectors：Total number of sectors read successfully.<br />
- disk.io.write_merged：Adjacent write requests merged in a single req.<br />
- disk.io.write_requests：total number of writes completed successfully.<br />
- disk.io.write_sectors：total number of sectors written successfully.<br />
- disk.io.read_bytes：单位是byte的数字<br />
- disk.io.write_bytes：单位是byte的数字<br />
- disk.io.avgrq_sz：下面几个值就是iostat -x 1看到的值<br />
- disk.io.avgqu-sz<br />
- disk.io.await<br />
- disk.io.svctm<br />
- disk.io.util：是个百分数，比如56.43，表示56.43%<br />
<a name="4Ua3u"></a>
### 8. 机器负载相关采集项
计算方法：读取/proc/loadavg，都是原始值类型的：

- load.1min<br />
- load.5min<br />
- load.15min<br />
<a name="bEH1B"></a>
### 9. 内存相关采集项
计算方法：读取/proc/meminfo 中的内容，其中的mem.memfree是free+buffers+cached，mem.memused=mem.memtotal-mem.memfree。用户具体可以参考free命令的输出和帮助文档来理解每个metric的含义。

- mem.memtotal：内存总大小<br />
- mem.memused：使用了多少内存<br />
- mem.memused.percent：使用的内存占比<br />
- mem.memfree<br />
- mem.memfree.percent<br />
- mem.swaptotal：swap总大小<br />
- mem.swapused：使用了多少swap<br />
- mem.swapused.percent：使用的swap的占比<br />
- mem.swapfree<br />
- mem.swapfree.percent<br />
<a name="0f604009"></a>
### 10. 网络相关采集项
计算方法：读取/proc/net/dev的内容，每个metric都附加有一组tag，形如iface=$iface，标明具体那个interface，比如eth0。metric中带有in的表示流入情况，out表示流出情况，total是总量in+out，支持的metric如下：

- net.if.in.bytes<br />
- net.if.in.compressed<br />
- net.if.in.dropped<br />
- net.if.in.errors<br />
- net.if.in.fifo.errs<br />
- net.if.in.frame.errs<br />
- net.if.in.multicast<br />
- net.if.in.packets<br />
- net.if.out.bytes<br />
- net.if.out.carrier.errs<br />
- net.if.out.collisions<br />
- net.if.out.compressed<br />
- net.if.out.dropped<br />
- net.if.out.errors<br />
- net.if.out.fifo.errs<br />
- net.if.out.packets<br />
- net.if.total.bytes<br />
- net.if.total.dropped<br />
- net.if.total.errors<br />
- net.if.total.packets<br />
<a name="hCQzj"></a>
### 11. 端口采集项
计算方法，通过ss -ln，来判断指定的端口是否处于listen状态。原始值类型，值要么是1：代表在监听，要么是0，代表没有在监听。每个metric都附件一组tag，形如port=$port，$port就是具体的端口。

- net.port.listen<br />
<a name="hzH9G"></a>
### 12. 机器内核配置

- kernel.maxfiles：读取的/proc/sys/fs/file-max<br />
- kernel.files.allocated：读取的/proc/sys/fs/file-nr第一个Field<br />
- kernel.files.left：值=kernel.maxfiles-kernel.files.allocated<br />
- kernel.maxproc：读取的/proc/sys/kernel/pid_max<br />
<a name="jtlct"></a>
### 13. ntp采集项
使用 ntpq -pn 获取本机时间相对于 ntp 服务器的 offset。

- sys.ntp.offset：本机偏移时间，单位为ms，值过大或者为0则表明有异常，需要报警<br />
<a name="3fe5b7fd"></a>
### 14. 进程监控

- proc.num：判断某个进程的数目，这里需要分两个场景，一种是根据进程的名字来判定，比如name=sshd；另外一种是根据cmdline来判定，比如Java的应用进程名可能都是java，根据第一种情况没法做区分，此时可以配置cmdline，如cmdline=./falcon_agent-c./cfg.ini<br />
<a name="Eiuz8"></a>
### 15. 进程资源监控

- process.cpu.all：进程和它的子进程使用的sys+user的cpu，单位是jiffies<br />
- process.cpu.sys：进程和它的子进程使用的sys cpu，单位是jiffies<br />
- process.cpu.user：进程和它的子进程使用的user cpu，单位是jiffies<br />
- process.swap：进程和它的子进程使用的swap，单位是page<br />
- process.fd：进程使用的文件描述符个数<br />
- process.mem：进程占用内存，单位byte<br />
<a name="62ed86fb"></a>
### 16. ss命令输出

- ss.orphaned<br />
- ss.closed<br />
- ss.timewait<br />
- ss.slabinfo.timewait<br />
- ss.synrecv<br />
- ss.estab<br />
