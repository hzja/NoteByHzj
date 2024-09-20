Linux uptime dmesg lsof ps top kill proc free <br />一般来说程序分为两类，一种是系统程序，一种是应用程序。一个运行了程序，就可以说是一个进程了，进程是占用内存空间的，而当杀掉进程时，资源也会随之释放。
<a name="njKw0"></a>
## 进程的类型
用户进程：用户自己的程序，用户可以控制他的开启和关闭。<br />交互进程：与用户进行交互操作的进程。<br />批处理进程：是一个进程集合，按顺序启动其他进程。<br />守护进程：一直运行的进程.crond。
<a name="BJY5Y"></a>
## 进程的属性
进程ID： (pid) 唯一的数字标示，区分不同的进程。<br />进程有父进程和子进程。<br />启动进程的用户ID(uid)和用户的属组。<br />进程的三种状态：

- 运行 -- R
- 休眠 -- S
- 僵尸 -- Z

进程的优先级：取值范围(-20,19) ，数值越小优先级越大，默认为0。<br />进程链接的终端。<br />进程占用资源情况。
<a name="jrsI7"></a>
## 父子进程的关系
父子进程的关系，父亲管理儿子。父进程终止时子进程一定终止，而子进程终止时父进程不一定终止。
<a name="b0LNX"></a>
## 进程管理工具
ps：查看进程。<br />top：可以查看进程的动态信息。<br />kill：杀进程。<br />pstree：查看进程树。<br />pgrep：搜进程。<br />lsof：查看进程打开的文件。
<a name="gP5Qt"></a>
## uptime-快速查看系统平均负载
这是一种用来快速查看系统平均负载的方法，它表明了系统中有多少要运行的任务（进程）。在 Linux 系统中，这些数字包含了需要在 CPU 中运行的进程以及正在等待 I/O（通常是磁盘 I/O）的进程。它仅仅是对系统负载的一个粗略展示。
```bash
[root@iZuligp6e1dyzfZ /]# uptime
 15:27:02 up 14 days, 23:13,  1 user,  load average: 1.22, 1.11, 2.95
```
<a name="W2xwj"></a>
## `dmesg | tail` ：查看系统消息
这条命令显式了最近的 10 条系统消息，查找导致性能问题的错误。
```bash
[root@iZuligp6e1dyzfZ /]# dmesg | tail
[1119327.023875]  [<ffffffff81185f7e>] __get_free_pages+0xe/0x50
[1119327.024394]  [<ffffffff811db26e>] kmalloc_order_trace+0x2e/0xa0
[1119327.024947]  [<ffffffff811dda41>] __kmalloc+0x221/0x240
[1119327.025437]  [<ffffffff811dd875>] ? __kmalloc+0x55/0x240
[1119327.025941]  [<ffffffff81697809>] ? system_call_fastpath+0x16/0x1b
[1119327.026513] ---[ end trace 2bc052da79f03450 ]---
[1288415.541236] device eth0 entered promiscuous mode
[1288422.331786] device eth0 left promiscuous mode
[1288428.988294] device eth0 entered promiscuous mode
[1288432.478666] device eth0 left promiscuous mode
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596439874126-8760fd5a-f9fc-45a2-bade-f5f5dc017b54.png#averageHue=%234d4d4d&height=236&id=z0tm9&originHeight=708&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=844461&status=done&style=none&title=&width=1107.6666666666667)
<a name="tnvdg"></a>
## lsof–查看文件的进程信息
lsof命令全称“LiSt Open Files”用于查看进程开打的文件，打开文件的进程，进程打开的端口(TCP、UDP)。找回/恢复删除的文件。是十分方便的系统监视工具，因为lsof命令需要访问核心内存和各种文件，所以需要root用户执行。<br />在linux环境下，任何事物都以文件的形式存在，通过文件不仅仅可以访问常规数据，还可以访问网络连接和硬件。所以如传输控制协议 (TCP) 和用户数据报协议 (UDP) 套接字等，系统在后台都为该应用程序分配了一个文件描述符，无论这个文件的本质如何，该文件描述符为应用程序与基础操作系统之间的交互提供了通用接口。因为应用程序打开文件的描述符列表提供了大量关于这个应用程序本身的信息，因此通过lsof工具能够查看这个列表对系统监测以及排错将是很有帮助的。<br />语法格式：lsof [参数] [文件]<br />常用参数：

| -a | 列出打开文件存在的进程 |
| --- | --- |
| -c <进程名> | 列出指定进程所打开的文件 |
| -g | 列出GID号进程详情 |
| -d <文件号> | 列出占用该文件号的进程 |

<a name="PIYkf"></a>
### 查看端口属于哪个程序
```bash
lsof -i :8080
```
<a name="Chgsy"></a>
### 查看使用文件的进程信息
```bash
lsof <文件名>
# 例如，要查找名为 example.txt 的文件的进程信息
lsof example.txt
```
此命令会列出所有正在使用该文件的进程信息，包括进程 ID、进程名称和文件句柄。<br />如果要查找某个目录下所有正在使用的文件信息，可以使用以下命令：
```
lsof +D <目录路径>
# 例如，要查找 /var/log 目录下所有正在使用的文件信息：
lsof +D /var/log
```
此命令会列出所有正在使用 /var/log 目录下的文件的进程信息。
<a name="Yd8tp"></a>
## ps-静态显示当前进程信息
ps命令是“processstatus”的缩写，ps命令用于显示当前系统的进程状态。可以搭配kill指令随时中断、删除不必要的程序。<br />ps命令是最基本同时也是非常强大的进程查看命令，使用该命令可以确定有哪些进程正在运行和运行的状态、进程是否结束、进程有没有僵死、哪些进程占用了过多的资源等等，总之大部分信息都是可以通过执行该命令得到的。<br />语法格式：ps [参数]<br />常用参数：

| -a | 显示所有终端机下执行的程序，除了阶段作业领导者之外 |
| --- | --- |
| -c | 显示CLS和PRI栏位 |
| c | 列出程序时，显示每个程序真正的指令名称，而不包含路径，选项或常驻服务的标示 |
| -d | 显示所有程序，但不包括阶段作业领导者的程序 |
| -A | 所有的进程均显示出来，与 -e 具有同样的效用 |
| -u | 以用户为主的进程状态 |

```bash
a      显示所有用户的进程
r      显示运行中的进程
l      长格式输出
u      按用户名和启动时间的顺序来显示进程
f      用树形格式来显示进程
x      显示没有控制终端的进程
A      所有的进程均显示出来，与 -e 具有同样的效用
c      显示CLS和PRI栏位
l      较长、较详细的将该PID 的的信息列出
j      工作的格式 (jobs format)
-f     更为完整的输出
```
<a name="XhrbT"></a>
### 使用示例
```bash
根据CPU使用率来升序排序
$ ps -aux --sort -pcpu | less
根据内存使用率来升序排序
$ ps -aux --sort -pmem | less
将它们合并到一个命令，并通过管道显示前10个结果
$ ps -aux --sort -pcpu,+pmem | head -n 10
通过进程名和PID进行过滤
$ ps -C getty
要看到更多的细节，可以使用-f参数来查看格式化的信息列表
$ ps -f -C getty
根据线程来过滤进程
$ ps -L 'PID'
以树形结构显示进程，可以使用 -axjf 参数
$ ps -axjf
要查看现在有谁登入服务器。可以使用ps命令加上相关参数
$ ps -eo pid,user,args
参数 -e 显示所有进程信息，-o 参数控制输出。Pid,User 和 Args参数显示PID，运行应用的用户
能够与 -e 参数 一起使用的关键字是args, cmd, comm, command, fname, ucmd, ucomm, lstart, bsdstart 和 start。

格式化输出root用户（真实的或有效的UID）创建的进程
$ ps -U root -u root u
-U 参数按真实用户ID(RUID)筛选进程，它会从用户列表中选择真实用户名或 ID。真实用户即实际创建该进程的用户。
-u 参数用来筛选有效用户ID（EUID）。
最后的 u 参数用来决定以针对用户的格式输出，由User, PID, %CPU, %MEM, VSZ, RSS, TTY, STAT, START, TIME 和 COMMAND这几列组成。

实时监控进程状态
$ watch -n 1 'ps -aux --sort -pmem, -pcpu'
输出太长，也可以限制它，比如前20条，可以使用 head 命令来做到
$ watch -n 1 'ps -aux --sort -pmem, -pcpu | head 20'

通过PS定义显示的字段
$ watch -n 1 'ps -aux -U pungki u --sort -pmem, -pcpu | head 20'
```
<a name="jgneI"></a>
### BSD格式来显示进程
```bash
➜  / ps -aux
USER       PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
root         1  0.0  0.1  43284  2960 ?        Ss    2019   4:48 /usr/lib/systemd/systemd --switched-root --system --des
root         2  0.0  0.0      0     0 ?        S     2019   0:00 [kthreadd]
root         3  0.0  0.0      0     0 ?        S     2019   0:14 [ksoftirqd/0]
root         5  0.0  0.0      0     0 ?        S<    2019   0:00 [kworker/0:0H]
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594544327532-b38c3a82-be12-414e-b5d0-b7bd74c686cf.png#averageHue=%233a3a3a&height=511&id=C1YQg&originHeight=1533&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1914749&status=done&style=none&title=&width=1107.6666666666667)
<a name="B22IC"></a>
### 标准格式显示进程unix风格
```bash
➜  / ps -ef
UID        PID  PPID  C STIME TTY          TIME CMD
root         1     0  0  2019 ?        00:04:48 /usr/lib/systemd/systemd --switched-root --system --deserialize 21
root         2     0  0  2019 ?        00:00:00 [kthreadd]
root         3     2  0  2019 ?        00:00:14 [ksoftirqd/0]
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594544387557-444323ed-1ec7-43fa-b75c-555f8ffcc3da.png#averageHue=%23393939&height=510&id=l6BHL&originHeight=1529&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1881003&status=done&style=none&title=&width=1107.6666666666667)
<a name="GO70d"></a>
### 属性详解
USER：进程的属主<br />PID：进程的ID<br />%CPU：进程占cpu百分比<br />%MEM：进程占内存的百分比<br />VSZ：进程占用虚拟内存大小<br />RSS：固定内存使用数量<br />STAT     进程状态
```
R    正在运行可中在队列中可过行的；
S    处于休眠状态；
T    停止或被追踪；
Z    僵尸进程；
N    优先级较低的进程
L    有些页被锁进内存；
s    进程的领导者（在它之下有子进程）
```

START   启动进程的时间；<br />TIME  进程消耗CPU的时间；<br />COMMAND 命令的名称和参数;<br />按照指定属性排序：<br />按照cpu从小到大排序。
```bash
➜  / ps -aux --sort %cpu
USER       PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
root         1  0.0  0.1  43284  2960 ?        Ss    2019   4:48 /usr/lib/systemd/systemd --switched-root --system --des
root         2  0.0  0.0      0     0 ?        S     2019   0:00 [kthreadd]
root         3  0.0  0.0      0     0 ?        S     2019   0:14 [ksoftirqd/0]
root         5  0.0  0.0      0     0 ?        S<    2019   0:00 [kworker/0:0H]
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594544545117-d121d36a-807f-4fd6-a12b-fde39be4cc91.png#averageHue=%23373737&height=380&id=TMSr9&originHeight=1139&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1458959&status=done&style=none&title=&width=1107.6666666666667)
<a name="J1lwS"></a>
### ps监控Java线程数
```bash
➜  ~ ps -eLf | grep java | wc -l
0
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594712181443-57680ead-21f5-49d8-812f-5d88f222b81c.png#averageHue=%23373737&height=60&id=ut6Re&originHeight=180&originWidth=993&originalType=binary&ratio=1&rotation=0&showTitle=false&size=63500&status=done&style=none&title=&width=331)
<a name="QWrMP"></a>
### ps监控进程详情
```bash
➜  ~ ps -ef | grep 1
root         1     0  0  2019 ?        00:04:50 /usr/lib/systemd/systemd --switched-root --system --deserialize 21
root         2     0  0  2019 ?        00:00:00 [kthreadd]
root         3     2  0  2019 ?        00:00:14 [ksoftirqd/0]
root         5     2  0  2019 ?        00:00:00 [kworker/0:0H]
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594712258022-e454c109-0163-4abc-9f6a-de3ec6927da8.png#averageHue=%233e3e3e&height=583&id=PrEmp&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2075997&status=done&style=none&title=&width=1107.6666666666667)
<a name="c9Mhk"></a>
### 查看某个命令的父进程
在 Linux 系统中，可以使用 ps 命令和一些选项来查看运行 scp 命令的父进程。以下是一些可能有用的选项：

- -o ppid：只显示父进程的 ID。
- -o comm：只显示进程的名称。

例如，要查找运行 scp 命令的父进程，可以在终端中输入以下命令：
```bash
ps -o ppid= -o comm= -C scp
```
这个命令会列出所有正在运行的 scp 进程的父进程 ID 和名称。如果需要查找某个具体的 scp 进程的父进程，可以在命令的末尾加上 grep 命令来过滤输出结果，例如：
```bash
ps -o ppid= -o comm= -C scp | grep <进程名称或进程 ID>
```
这个命令会列出指定进程的父进程信息。<br />需要注意的是，如果 scp 命令是从脚本或其他程序中调用的，那么父进程可能是脚本或程序的名称，而不是 scp。在这种情况下，可以使用 ps 命令的 -f 选项来显示完整的进程信息，例如：
```bash
ps -fC <脚本或程序名称>
```
这个命令会列出指定脚本或程序的完整进程信息，包括父进程的 ID 和名称。
<a name="JDEcQ"></a>
## top-进程的信息动态查看
与ps相反的是top命令可以查看进程的动态信息。 `top`命令是用于监控Linux系统状况，可以通过该命令实时查看系统各个进程的资源占用状况。
```bash
➜  ~ top --h
top: inappropriate '-h'
Usage:
  top -hv | -bcHiOSs -d secs -n max -u|U user -p pid(s) -o field -w [cols]
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594711033774-5b08169b-2d51-4edb-a466-58108f0eced4.png#averageHue=%23363636&height=97&id=U2cft&originHeight=291&originWidth=2156&originalType=binary&ratio=1&rotation=0&showTitle=false&size=228556&status=done&style=none&title=&width=718.6666666666666)<br />命令：top<br />语法：top 参数<br />常用参数：<br />操作：
<a name="KYyUe"></a>
### top 命令选项
`top [-] [d] [p] [q] [c] [b] [S] [s] [n]`

- `d`：指定两次频幕刷新频率，即时间间隔，默认为5秒。如`top -d 4`，指定更新周期为4秒。
- `p`：指定监控进程id来监控某个进程状态。如`top -p 1201`，显示进程号为1201的进程信息、cpu、内存占用等信息。
- `q`：无延时刷新，若有超级用户权限，则top会以高优先级运行。
- `c`：显示整个命令行。如`top -c`，显示每个进程的完整命令。
- `S`：指定累计模式。如`top -S`，以累计模式显示程序信息。
- `s`：使top命令在安全模式中运行。如`top -s`，不能交互使用s键位，会报`unavailable in secure mode`提示。
- `n`：指定更新的次数。如`top -n 2`只更新2次，就退出top程序。
- `b`：批处理模式显示程序信息。如`top -b`，以批处理模式显示程序信息。
- `i`：使top不显示任何闲置或者僵死进程。
<a name="hBslq"></a>
### top界面程序交互常用按键说明

1. 按`c`键位：切换显示命令名称和完整的命令行。
2. 按`P`键位：根据CPU使用百分比大小进行排序。
3. 按`M`键位：根据内存mem占有率大小进行排序。
4. 按`n`键位：设置在进程列表中显示进程的数量。
5. 按`T`键位：根据时间/累计时间进行排序。
6. 按`s`键位：改变画面更新频率，输入两次刷新之间的延迟时间，以秒为单位。
7. 按`S`键位：切换到累计模式；开关。
8. 按`i`键位：忽略闲置和僵尸进程；开关。
9. 按`h`键位：显示帮助画面。
10. 按`u`键位：输入指定的user，就会显示所有该user的程序进程。
11. 按`q`键位：退出top查看程序。
12. 按`f`键位：编排视图显示字段。
```bash
➜  / top
top - 17:03:59 up 332 days, 21:31,  1 user,  load average: 0.00, 0.01, 0.05
Tasks:  71 total,   1 running,  70 sleeping,   0 stopped,   0 zombie
%Cpu(s):  0.0 us,  0.0 sy,  0.0 ni,100.0 id,  0.0 wa,  0.0 hi,  0.0 si,  0.0 st
KiB Mem :  1883724 total,   713140 free,   119512 used,  1051072 buff/cache
KiB Swap:        0 total,        0 free,        0 used.  1457088 avail Mem

top - 17:04:17 up 332 days, 21:32,  1 user,  load average: 0.00, 0.01, 0.05
Tasks:  71 total,   1 running,  70 sleeping,   0 stopped,   0 zombie
%Cpu(s):  0.0 us,  0.0 sy,  0.0 ni,100.0 id,  0.0 wa,  0.0 hi,  0.0 si,  0.0 st
KiB Mem :  1883724 total,   713140 free,   119512 used,  1051072 buff/cache
KiB Swap:        0 total,        0 free,        0 used.  1457088 avail Mem

  PID USER      PR  NI    VIRT    RES    SHR S %CPU %MEM     TIME+ COMMAND
    1 root      20   0   43284   2960   1640 S  0.0  0.2   4:48.80 systemd
    2 root      20   0       0      0      0 S  0.0  0.0   0:00.59 kthreadd
    3 root      20   0       0      0      0 S  0.0  0.0   0:14.16 ksoftirqd/0
    5 root       0 -20       0      0      0 S  0.0  0.0   0:00.00 kworker/0:0H
    7 root      rt   0       0      0      0 S  0.0  0.0   0:00.00 migration/0
    8 root      20   0       0      0      0 S  0.0  0.0   0:00.00 rcu_bh
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594544648662-44078afe-feb3-4d91-9e15-5f9881d9bfe9.png#averageHue=%23404040&height=687&id=tZq1W&originHeight=2060&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2789897&status=done&style=none&title=&width=1280)
<a name="nik6L"></a>
### top命令的参数说明
统计信息区的前5行是系统整体的系统信息。<br />1）第1行是任务队列信息，这个类似于`uptime`命令的执行结果
```bash
➜  ~ uptime
 15:21:49 up 334 days, 19:49,  1 user,  load average: 0.00, 0.01, 0.05
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594711363146-a6238920-dcdd-43ee-ab6a-24f18882ac88.png#averageHue=%23404040&height=687&id=Sm7Nz&originHeight=2060&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2787695&status=done&style=none&title=&width=1280)
```
11:38:42     当前时间
up 13:15     系统运行时间,格式为时:分
2 user         当前登陆用户数量
load average: 0.01, 0.03, 0.05  系统负载,任务队列平均长度 1分钟,5分钟,15分钟前到现在的平均值。
```
2）第2、3行是进程和cpu的统计信息。`tasks`：进程任务数
```
98 total  进程总数量
 1 running 正在运行的进程数量
 97 sleeping 睡眠的进程数量
 0 stopped  停止的进程数量
 0 zombie   僵尸进程数量
 %Cpu(s):  cpu使用情况，多核展示也可以通过按1数字，就会变成%Cpu0，%Cpu1，%Cpu2... ...
 0.2 us ：user，time running un-niced user processes 用户空间占用cpu百分比
 0.0 sy ：system，time running kernel processes 内核空间占用cpu百分比
 0.0 ni ：nice ， time running niced user processes 用户进程空间内改变过优先级的进程占用cpu百分比
 99.8 id：idle， time idel processes 空闲cpu百分比
 0.0 wa：IO-wait ，time waiting for I/O completion 等待输入输出cpu时间百分比
 0.0 hi：hardware interrupts，time spent servicing hardware interrupts 硬件cpu中断占用百分比
 0.0 si：software interrupts，time spent servicing software interrupts 软中断占用百分比
 0.0 st：time stolen from this vm by the hypervisor虚拟机占用百分比；可以通过cat /proc/cpuinfo查看cpu信息
```
第四五行是内存信息<br />Mem：(单位K)
```
3881688 total  物理内存总量
229264 free    空闲内存总量
543932 used    使用的物理内存总量
3108492 buff/cache 内核缓存的内存量
```
Swap：(单位K)
```
1049596 total      交换区总量
 1049596 free       空闲交换区总量
 0 used             使用的交换区总量
 3026944 avail Mem  可利用的内存量
```
内存中的内容被换出到交换区，而后又被换入到内存，但使用过的交换区尚未被覆盖，该数值即为这些内容已存在于内存中的交换区的大小。<br />相应的内存再次被换出时可不必再对交换区写入。
<a name="JwpEV"></a>
### 进程信息区
默认情况下仅显示比较重要的 PID、USER、PR、NI、VIRT、RES、SHR、S、% CPU、% MEM、TIME+、COMMAND 列。<br />PID：进程id<br />USER：进程所有者用户名<br />PR：优先级<br />NI：进程优先级，nice值，负值 -> 高优先级，正值 -> 低优先级<br />VIRT：虚拟内存总量 virt=swap + res<br />RES：实际使用内存大小<br />SHR：共享内存大小<br />S：进程状态

- d：不可中断的睡眠状态
- r：运行
- s：睡眠
- t：跟踪
- z：僵尸进程

%CPU：上次更新到现在cpu时间占用百分比<br />%MEM：进程使用物理内存百分比<br />TIME+：进程使用cpu的时间总计，单位 1/100秒<br />COMMAND：命令行<br />统计信息区域的下方显示了各个进程的详细信息。完整的各列含义如下
```
PID 进程id 
PPID 父进程id 
RUSER Real user name 
d UID 进程所有者的用户id 
e USER 进程所有者的用户名 
f GROUP 进程所有者的组名 
g TTY 启动进程的终端名。不是从终端启动的进程则显示为 ? 
h PR 优先级 
i NI nice值。负值表示高优先级，正值表示低优先级 
j P 最后使用的CPU，仅在多CPU环境下有意义 
k %CPU 上次更新到现在的CPU时间占用百分比 
l TIME 进程使用的CPU时间总计，单位秒 
m TIME+ 进程使用的CPU时间总计，单位1/100秒 
n %MEM 进程使用的物理内存百分比 
o VIRT 进程使用的虚拟内存总量，单位kb。VIRT=SWAP+RES 
p SWAP 进程使用的虚拟内存中，被换出的大小，单位kb。
q RES 进程使用的、未被换出的物理内存大小，单位kb。RES=CODE+DATA 
r CODE 可执行代码占用的物理内存大小，单位kb 
s DATA 可执行代码以外的部分(数据段+栈)占用的物理内存大小，单位kb 
t SHR 共享内存大小，单位kb 
u nFLT 页面错误次数 
v nDRT 最后一次写入到现在，被修改过的页面数。
w S 进程状态。
D=不可中断的睡眠状态 
R=运行 
S=睡眠 
T=跟踪/停止 
Z=僵尸进程 
x COMMAND 命令名/命令行 
y WCHAN 若该进程在睡眠，则显示睡眠中的系统函数名 
z Flags 任务标志，参考 sched.h
```
<a name="ukuMl"></a>
### top查看进程线程
命令： `top -Hp PID`
```bash
➜  ~ top -Hp 1
top - 15:47:33 up 334 days, 20:15,  1 user,  load average: 0.00, 0.01, 0.05
Threads:   1 total,   0 running,   1 sleeping,   0 stopped,   0 zombie
%Cpu(s):  0.0 us,  0.0 sy,  0.0 ni,100.0 id,  0.0 wa,  0.0 hi,  0.0 si,  0.0 st
KiB Mem :  1883724 total,   679708 free,   118172 used,  1085844 buff/cache
KiB Swap:        0 total,        0 free,        0 used.  1458588 avail Mem

  PID USER      PR  NI    VIRT    RES    SHR S %CPU %MEM     TIME+ COMMAND
    1 root      20   0   43284   2960   1640 S  0.0  0.2   4:50.26 systemd
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594712841852-334ac1bf-0279-414b-a919-25de2d3f4e72.png#averageHue=%23525252&height=246&id=hLJID&originHeight=737&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=779347&status=done&style=none&title=&width=1107.6666666666667)
<a name="RPlfU"></a>
### top 命令常见的操作
<a name="f1lNN"></a>
#### 1、2500 毫秒刷新一次 TOP 内容，总共 5 次，输出内容存放到 performace.txt 文件中
```bash
top -b -d 2.5 -n 5 > performace.txt
```
> 注：要将内容输出到文件中，必须使用 - b，表示批处理选项

<a name="KtOs5"></a>
#### 2、top 命令如何快速按 % CPU、% MEM、TIME + 列排序
a) % CPU：使用大写字母按键：`P`<br />b) % MEM：使用大写字母按键：`M`<br />c) T IME + 使用大写字母按键：`T`
> 注：top 默认排序为倒序，如果确实需要升序排序，可以使用大写字母按键：R

<a name="hCrdL"></a>
#### 3、top 命令中显示其它列值、将两列互换等
a) 选择显示列或隐藏列：使用小写字母按键：`f`<br />c) 交换列显示顺序：使用小写字母按键：`o`<br />b) 选择需要排序的列：使用大写字母按键：`F`
<a name="FDIiL"></a>
### CPU steal time
如果想要部署虚拟环境（例如：Amazon EC2），steal time就是想要关注的性能指标之一。如果这个指标的数值很高，那么说明机器状态非常糟糕。<br />虚拟机（VM）会与虚拟环境的宿主机上的多个虚拟机实例共享物理资源。其中之一共享的就是CPU时间切片。如果VM的物理机虚拟比是1/4， 那么它的CPU使用率不会限制于25%的CPU时间切片－它能够使用超过它设置的虚拟比。（有别于内存的使用，内存大小是严格控制的）。<br />可以使用Linux 的 TOP 命令来看到实时的一些性能指标。<br />两个可能较为熟悉的是 %id(空闲 百分比) 和 %wa(I/O 等待 百分比)。如果 %id 很低， 那么说明CPU的工作负载很大并且没有多少计算负载能力剩余。如果 %wa 很高，则说明瓶 CPU 处于等待计算的状态，但是正在等待I/O活动的完成(类似 从数据库中获取存储在 磁盘上 的一行数据)。<br />%st（percent steal time） 是CPU展示的最后一个性能指标。
<a name="udcqG"></a>
## iotop-磁盘I/O使用情况监控
`iotop` 是一个可以用来进行磁盘I/O使用情况监控的工具命令，其UI界面和上面提到的几个`top`命令也都类似，包括进程表信息也都给了出来。<br />其实Linux下系统自带的IO统计工具也有不少，常用的就比如`iostat`等等，但是`iotop`命令可以很直接了当地查看到占用磁盘I/O的进程信息，比较直观且容易上手。<br />如果未安装则可以自行安装。
```bash
yum install iotop
```
这里也简单看一下该工具命令执行后的效果：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1610374659370-a4ddd00d-0e1a-420a-b3f1-24f94b423bf6.png#averageHue=%23464646&height=583&id=xMXIs&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2152954&status=done&style=none&title=&width=1107.6666666666667)
<a name="f0zcO"></a>
## kill-终止进程
命令：kill<br />语法：
```
kill [-s signal|-p] [-q sigval] [-a] [--] pid...
kill -l [signal]
```
<a name="SwNLB"></a>
### 常用参数
```
-l 列出所有信号名称
-s 指定发送信号（默认）
-u 指定用户
```
<a name="KCPtB"></a>
### 操作
<a name="nPtUB"></a>
#### 列出所有信号名称
```bash
➜  / kill -l
HUP INT QUIT ILL TRAP ABRT BUS FPE KILL USR1 SEGV USR2 PIPE ALRM TERM STKFLT CHLD CONT STOP TSTP TTIN TTOU URG XCPU XFSZ VTALRM PROF WINCH POLL PWR SYS
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594544859346-b2e29ba1-bc6b-4cd0-b0ea-24da30d6b088.png#averageHue=%23383838&height=82&id=SHh34&originHeight=246&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=307170&status=done&style=none&title=&width=1107.6666666666667)
<a name="geP4y"></a>
#### 强制终止
```bash
➜  / kill -9 23423
```
```bash
终止
➜  / kill -15 22323
```
<a name="6Sl5j"></a>
#### 杀死指定用户的所有进程
```bash
➜  / kill -u superh
```
<a name="Z5Ye3"></a>
## proc目录-目录中有内核，进程运行状态的信息
这个目录中有内核，进程运行状态的信息。
```bash
➜  /proc ls
```
<a name="znsKj"></a>
## free-查看系统使用和空闲的内存情况
<a name="HQDE8"></a>
### 命令参数
```bash
➜  ~ free --h
free: option '--h' is ambiguous; possibilities: '--human' '--help'

Usage:
 free [options]

Options:
 -b, --bytes         show output in bytes
 -k, --kilo          show output in kilobytes
 -m, --mega          show output in megabytes
 -g, --giga          show output in gigabytes
     --tera          show output in terabytes
 -h, --human         show human-readable output
     --si            use powers of 1000 not 1024
 -l, --lohi          show detailed low and high memory statistics
 -t, --total         show total for RAM + swap
 -s N, --seconds N   repeat printing every N seconds
 -c N, --count N     repeat printing N times, then exit
 -w, --wide          wide output

     --help     display this help and exit
 -V, --version  output version information and exit

For more details see free(1).
```
查看系统使用和空闲的内存情况。<br />命令：free<br />语法：free 参数<br />常用参数：
```
-b, --bytes        byte为单位展示
-k, --kilo            k为单位展示
-m, --mega            m为单位展示
-g, --giga            g为单位展示
```
<a name="hvSIY"></a>
### 操作
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594546534522-6c920f8d-d12e-4c9c-9770-ffedc631b858.png#averageHue=%23343434&height=385&id=ii0Uv&originHeight=1156&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1380278&status=done&style=none&title=&width=1107.6666666666667)

- `Mem`：内存的使用情况。
- `Swap`：交换空间的使用情况，是磁盘的一块区域，可以是swap分区也可以是swap文件，当系统物理内存使用比较紧张时，Linux会将内存中不常被访问的数据保存到Swap中，若需要访问该内容，会加载到内存中，即换出和换入的意思。
- `total`：物理内存总数。
- `used`：已使用内存大小。
- `free`：可使用内存大小，即未被真正使用过的物理内存大小。
- `shared`：多个进程可共享内存大小。
- `buff/cache`：磁盘缓存大小。
   - buffers：用于块设备 I/O 的缓冲区缓存。
   - cached：用于文件系统的页面缓存。
- `available`：可提供的内存大小，区别于free列，如果没有足够的free内存可用，会从buffer/cache回收内存来满足应用程序的需求，所以`available=free+buffer+cache`。
<a name="Z78Yp"></a>
## `mpstat -P ALL 1` ：报告单个或组合处理器相关的统计数据
```bash
[root@iZuligp6e1dyzfZ /]# mpstat -P ALL 1
Linux 3.10.0-514.26.2.el7.x86_64 (iZuligp6e1dyzfZ)      08/03/2020      _x86_64_        (1 CPU)

03:24:24 PM  CPU    %usr   %nice    %sys %iowait    %irq   %soft  %steal  %guest  %gnice   %idle
03:24:25 PM  all   50.00    0.00   50.00    0.00    0.00    0.00    0.00    0.00    0.00    0.00
03:24:25 PM    0   50.00    0.00   50.00    0.00    0.00    0.00    0.00    0.00    0.00    0.00

03:24:25 PM  CPU    %usr   %nice    %sys %iowait    %irq   %soft  %steal  %guest  %gnice   %idle
03:24:26 PM  all   57.14    0.00   42.86    0.00    0.00    0.00    0.00    0.00    0.00    0.00
03:24:26 PM    0   57.14    0.00   42.86    0.00    0.00    0.00    0.00    0.00    0.00    0.00
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596439573349-a1b0dfff-b90f-4052-a41a-6f4b8b15bfaf.png#averageHue=%23505050&height=217&id=nBmJI&originHeight=651&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=748786&status=done&style=none&title=&width=1107.6666666666667)<br />说明：

- `%user`     显示在用户级别(application)运行使用 CPU 总时间的百分比。
- `%nice`     显示在用户级别，用于nice操作，所占用 CPU 总时间的百分比。
- `%system`  在核心级别(kernel)运行所使用 CPU 总时间的百分比。
- `%iowait`  显示用于等待I/O操作占用 CPU 总时间的百分比。
- `%irq`   显示在interval时间段内，硬中断占用的CPU总时间。
- `%soft`   显示在interval时间段内，软中断占用的CPU总时间。
- `%steal`   管理程序(hypervisor)为另一个虚拟进程提供服务而等待虚拟CPU的百分比。
- `%idle`     显示 CPU 空闲时间占用CPU总时间的百分比。
- `intr/s`  在internal时间段里，每秒CPU接收的中断的次数。
<a name="gzcCD"></a>
## vmstat - 虚拟内存统计
```bash
[root@iZuligp6e1dyzfZ /]# vmstat -h

Usage:
 vmstat [options] [delay [count]]

Options:
 -a, --active           active/inactive memory
 -f, --forks            number of forks since boot
 -m, --slabs            slabinfo
 -n, --one-header       do not redisplay header
 -s, --stats            event counter statistics
 -d, --disk             disk statistics
 -D, --disk-sum         summarize disk statistics
 -p, --partition <dev>  partition specific statistics
 -S, --unit <char>      define display unit
 -w, --wide             wide output
 -t, --timestamp        show timestamp

 -h, --help     display this help and exit
 -V, --version  output version information and exit

For more details see vmstat(8).
[root@iZuligp6e1dyzfZ /]# vmstat 1
procs -----------memory---------- ---swap-- -----io---- -system-- ------cpu----- -----timestamp-----
 r  b   swpd   free   buff  cache   si   so    bi    bo   in   cs us sy id wa st                 CST
 3  0      0 131960 141792 932268    0    0     3    54   15   10  2  1 97  0  0 2020-08-03 15:03:59
 2  0      0 131588 141792 932272    0    0     0    64 1784 1723 69 31  0  0  0 2020-08-03 15:04:00
 1  0      0 131588 141792 932276    0    0     0    64 1769 1632 64 36  0  0  0 2020-08-03 15:04:01
 1  0      0 131588 141792 932280    0    0     0   128 1752 1604 50 50  0  0  0 2020-08-03 15:04:02
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596438329414-57c50836-9043-4380-a7d9-da2f04484816.png#averageHue=%23404040&height=583&id=mcvKG&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2110649&status=done&style=none&title=&width=1107.6666666666667)

- r：CPU 中正在运行和等待运行的进程的数量。其提供了一个比平均负载更好的信号来确定 CPU 是否饱和，因为其不包含 I/O。解释：“r”的值大于了 CPU 的数量就表示已经饱和了。
- free：以 kb 为单位显式的空闲内存。如果数字位数很多，说明有足够的空闲内存。
- si, so：Swap-ins 和 swap-outs。如果它们不是零，则代表内存不足了。
- us, sy, id, wa, st：这些都是平均了所有 CPU 的 CPU 分解时间。它们分别是用户时间（user）、系统时间（内核）（system）、空闲（idle）、等待 I/O（wait）、以及占用时间（stolen）（被其他访客，或使用 Xen，访客自己独立的驱动域）。

CPU 分解时间将会通过用户时间加系统时间确认 CPU 是否为忙碌状态。等待 I/O 的时间一直不变则表明了一个磁盘瓶颈；这就是 CPU 的闲置，因为任务都阻塞在等待挂起磁盘 I/O 上了。可以把等待 I/O 当成是 CPU 闲置的另一种形式，其给出了为什么 CPU 闲置的一个线索。<br />对于 I/O 处理来说，系统时间是很重要的。一个高于 20% 的平均系统时间。<br />在上面的例子中，CPU 时间几乎完全花在了用户级，表明应用程序占用了太多 CPU 时间。而 CPU 的平均使用率也在 90% 以上。这不一定是一个问题；检查一下“r”列中的饱和度。
<a name="pLQin"></a>
## pidstat - Linux任务 (进程) 的统计信息：I/O、CPU、内存等
```bash
[root@iZuligp6e1dyzfZ /]# pidstat 1
Linux 3.10.0-514.26.2.el7.x86_64 (iZuligp6e1dyzfZ)      08/03/2020      _x86_64_        (1 CPU)

03:06:00 PM   UID      TGID       TID    %usr %system  %guest    %CPU   CPU  Command
03:06:01 PM     0      1207         -   20.00    0.00    0.00   20.00     0  java
03:06:01 PM     0         -     17329   20.00    0.00    0.00   20.00     0  |__AliYunDun
03:06:01 PM     0     28797         -    0.00   20.00    0.00   20.00     0  redis-server
03:06:01 PM     0     30729         -    0.00   40.00    0.00   40.00     0  pidstat
03:06:01 PM     0         -     30729    0.00   40.00    0.00   40.00     0  |__pidstat
03:06:01 PM     0     30850         -    0.00   20.00    0.00   20.00     0  redis-server
03:06:01 PM     0         -     30865   20.00    0.00    0.00   20.00     0  |__redis-server
03:06:01 PM     0         -     30877    0.00   20.00    0.00   20.00     0  |__redis-server

03:06:01 PM   UID      TGID       TID    %usr %system  %guest    %CPU   CPU  Command
03:06:02 PM     0      1207         -   33.33   16.67    0.00   50.00     0  java
03:06:02 PM     0         -      2173   33.33    0.00    0.00   33.33     0  |__java
03:06:02 PM     0     17315         -   16.67    0.00    0.00   16.67     0  AliYunDun
03:06:02 PM     0         -     17315   16.67    0.00    0.00   16.67     0  |__AliYunDun
03:06:02 PM     0     28797         -   16.67    0.00    0.00   16.67     0  redis-server
03:06:02 PM     0     30729         -   16.67   16.67    0.00   33.33     0  pidstat
03:06:02 PM     0         -     30729   16.67   16.67    0.00   33.33     0  |__pidstat
03:06:02 PM     0     30850         -   16.67    0.00    0.00   16.67     0  redis-server
03:06:02 PM     0         -     30877   16.67    0.00    0.00   16.67     0  |__redis-server
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596438499596-a601f480-30b4-47cf-a3a0-6145da64f764.png#averageHue=%23424242&height=458&id=kyMvz&originHeight=1374&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1703381&status=done&style=none&title=&width=1107.6666666666667)
<a name="iVrbq"></a>
## iostat - 设备、分区和网络文件系统的CPU统计和硬盘吞吐效率的数据
参数释义：<br />`-c`：仅显示cpu的状态<br />`-d`：仅显示存储设备的状态，不可以和-c一起使用<br />`-k`：默认显示的是读入读出的block信息，用-k可以改成KB大小来显示 -m<br />`-t`：显示日期<br />`-p device | ALL`：device为某个设备或者某个分区，如果使用ALL，就表示要显示所有分区和设备的信息<br />`-x`：显示扩展状态，显示更多内容
```bash
[root@iZuligp6e1dyzfZ /]# iostat -xz 1
Linux 3.10.0-514.26.2.el7.x86_64 (iZuligp6e1dyzfZ)      08/03/2020      _x86_64_        (1 CPU)

avg-cpu:  %user   %nice %system %iowait  %steal   %idle
           1.91    0.00    1.17    0.02    0.00   96.90

Device:         rrqm/s   wrqm/s     r/s     w/s    rkB/s    wkB/s avgrq-sz avgqu-sz   await r_await w_await  svctm  %util
vda               0.00     8.09    0.09    2.62     2.62    54.08    41.78     0.01    2.24    7.68    2.04   0.40   0.11

avg-cpu:  %user   %nice %system %iowait  %steal   %idle
          71.43    0.00   28.57    0.00    0.00    0.00

Device:         rrqm/s   wrqm/s     r/s     w/s    rkB/s    wkB/s avgrq-sz avgqu-sz   await r_await w_await  svctm  %util
vda               0.00   428.57   14.29  228.57   114.29  2628.57    22.59     0.24    1.00    2.00    0.94   0.47  11.43
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596438939471-b8d401c3-b839-4a96-86f7-8d0601219f15.png#averageHue=%23414141&height=367&id=devEB&originHeight=1102&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1306179&status=done&style=none&title=&width=1107.6666666666667)

- r/s, w/s, rkB/s, wkB/s：这些分别代表该设备每秒的读次数、写次数、读取 kb 数，和写入 kb 数。这些用于描述工作负载。性能问题可能仅仅是由于施加了过大的负载。
- await：以毫秒为单位的 I/O 平均消耗时间。这是应用程序消耗的实际时间，因为它包括了排队时间和处理时间。比预期更大的平均时间可能意味着设备的饱和，或设备出了问题。
- avgqu-sz：向设备发出的请求的平均数量。值大于 1 说明已经饱和了（虽说设备可以并行处理请求，尤其是由多个磁盘组成的虚拟设备。）
- %util：设备利用率。这个值是一个显示出该设备在工作时每秒处于忙碌状态的百分比。若值大于 60％，通常表明性能不佳（可以从 await 中看出），虽然它取决于设备本身。值接近  100% 通常意味着已饱和。

如果该存储设备是一个面向很多后端磁盘的逻辑磁盘设备，则 100% 利用率可能只是意味着当前正在处理某些 I/O 占用，然而，后端磁盘可能远未饱和，并且可能能够处理更多的工作。<br />请记住，磁盘 I/O 性能较差不一定是程序的问题。许多技术通常是异步 I/O，使应用程序不会被阻塞并遭受延迟（例如，预读，以及写缓冲）。
<a name="E004f"></a>
## tapestat  - 连接到系统的磁带驱动器的统计信息
```bash
[root@iZuligp6e1dyzfZ /]# tapestat -k
Linux 3.10.0-514.26.2.el7.x86_64 (iZuligp6e1dyzfZ)      08/04/2020      _x86_64_        (1 CPU)

No tape drives with statistics found
```
<a name="cifsiostat"></a>
## cifsiostat - CIFS统计
<a name="bvfxd"></a>
## pmap-JAVA分析线程堆栈
```bash
➜  ~ pmap 1
1:   /usr/lib/systemd/systemd --switched-root --system --deserialize 21
00007f46bdb59000     16K r-x-- libuuid.so.1.3.0
00007f46bdb5d000   2044K ----- libuuid.so.1.3.0
00007f46bdd5c000      4K r---- libuuid.so.1.3.0
00007f46bdd5d000      4K rw--- libuuid.so.1.3.0
00007f46bdd5e000    228K r-x-- libblkid.so.1.1.0
00007f46bdd97000   2048K ----- libblkid.so.1.1.0
00007f46bdf97000     12K r---- libblkid.so.1.1.0
00007f46bdf9a000      4K rw--- libblkid.so.1.1.0
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594712635174-c31334ed-728c-400d-a472-b95d40d252ba.png#averageHue=%23414141&height=583&id=BxFDI&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2048620&status=done&style=none&title=&width=1107.6666666666667)
