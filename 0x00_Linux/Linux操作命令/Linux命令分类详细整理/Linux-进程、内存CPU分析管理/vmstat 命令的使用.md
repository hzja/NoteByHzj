Linux<br />vmstat 是 virtual memory statistics 的缩写，它是Linux下一个非常有用的监控工具，除了内存之外，还提供 块 IO、CPU 时间 等额外的信息
<a name="eszaS"></a>
### 语法
vmstat 命令的执行不需要特殊的权限，普通用户就可以执行，其语法形式如下
```bash
vmstat [options] [delay [count]]
```
delay 表示数据更新间隔，单位是秒，如果没有指定此值，表示系统启动以来的平均时间，而且此时只输出一次结果<br />count 表示输出次数，如果没有指定该值，但是指定了 delay 的值，则表示无限次
<a name="C5hWx"></a>
### 结果字段说明
在命令行输入 vmstat 并回车， 会输出一次结果
```bash
[root@fcant ~]# vmstat
procs -----------memory---------- ---swap-- -----io---- -system-- ------cpu-----
 r  b   swpd   free   buff  cache   si   so    bi    bo   in   cs us sy id wa st
 3  0      0 991324    932 537336    0    0     0     0    3    4  0  0 100  0  0
```
结果中有许多的字段，下表列出了每个字段的详细说明

| 字段 | 说明 |
| --- | --- |
| r | 可运行进程的数量，包括运行态和就绪态 |
| b | 处于不可中断睡眠状态的进程数量 |
| swpd | 虚拟内存使用量 |
| free | 空闲内存 |
| buff | 用作buffer的内存数量 |
| cache | 用作cache的内存数量 |
| si | 从磁盘换入内存的数量 |
| so | 从内存换出到磁盘的数量 |
| bi | 从块设备接收的块，单位：块/秒 |
| bo | 发送到块设备的块，单位：块/秒 |
| in | 每秒中断数，包括时钟中断 |
| cs | 每秒上下文切换数 |
| us | 用户态执行时间 |
| sy | 内核态执行时间 |
| id | CPU空闲时间 |
| wa | 等待IO的时间 |
| st | 从虚拟机中偷取的时间 |

表中第二行 处于不可中断睡眠状态的进程数量，这里的不可中断是指 一个进程在执行某些系统调用时进入的状态，在这种状态下，进程被阻塞，并且不能被中断，直到完成系统调用<br />字段结果按照颜色分成了几部分，从上到下依次是：进程信息，内存信息，IO信息，系统中断和上下文，CPU时间<br />内存信息中数值的单位默认是 KB ( 1024 kbytes )， CPU时间字段并不表示具体的时间，而是占总CPU时间的一个百分比
<a name="pNqQJ"></a>
### 常见选项
| 选项 | 说明 |
| --- | --- |
| -a | 显示活跃和非活跃内存 |
| -f | 从系统启动至今 fork 的数量，包括 fork、vfork 以及 clone 等系统调用 |
| -s | 显示系统事件计数以及内存统计信息 |
| -d | 报告磁盘统计信息 |
| -D | 统计活动磁盘信息 |
| -p | 指定分区的详细信息 |
| -t | 追加一列时间显示 |
| -S | 按照指定字节单位显示 |
| -w | 结果按照宽模式显示 |
| -V | vmstat的版本 |

<a name="cimL1"></a>
### 常见用法
vmstat 命令主要用于识别系统的瓶颈，统计数据的时候，它不会包含自身进程
<a name="MVyjM"></a>
#### 按照指定时间间隔和次数输出
```bash
[root@fcant ~]$ vmstat 2 10
procs -----------memory---------- ---swap-- -----io---- -system-- ------cpu-----
 r  b   swpd   free   buff  cache   si   so    bi    bo   in   cs us sy id wa st
 2  0  12552 148356 234324 3382956    0    0     1    21    4    4  2  2 97  0  0
 1  0  12552 148264 234324 3382980    0    0     0     0 14974 27478  3  2 96  0  0
 1  0  12552 148232 234324 3382984    0    0     0    14 14384 27181  3  2 96  0  0
 0  0  12552 148376 234332 3383052    0    0     0   204 14197 26812  4  2 94  0  0
 0  0  12552 148512 234332 3383088    0    0     0     4 14398 27155  3  2 95  0  0
 0  0  12552 147892 234332 3383128    0    0     0   210 15515 28802  3  2 95  0  0
 1  0  12552 148388 234332 3383156    0    0     0     0 15147 28042  3  2 95  0  0
 0  0  12552 148264 234332 3383168    0    0     0     4 14380 27395  3  1 96  0  0
 0  0  12552 148264 234336 3383216    0    0     0   198 14430 27008  3  1 95  0  0
 2  0  12552 148140 234336 3383252    0    0     0     6 14233 27161  3  2 95  0  0
```
2 表示每隔 2 秒输出一次结果，10 表示总共输出 10 次，10 次之后程序自动结束
<a name="b0g9H"></a>
#### 修改内存显示单位
输出的结果中，内存数据的单位默认是 KB，可以通过 -S 选项调整显示的单位，有下面几种单位可供选择<br />注意：`-S` 选项对 `si`、 `so` 字段无效
```bash
k    # 1000 bytes
K    # 1024 bytes
m    # 1000 * 1000 bytes
M    # 1024 * 1024 bytes
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1644114649083-29f9d34f-f411-4e1b-b0da-f7d80c10e9ae.png#clientId=ud676dfc3-e4a6-4&from=paste&height=158&id=u1b404795&originHeight=394&originWidth=2394&originalType=binary&ratio=1&rotation=0&showTitle=false&size=78186&status=done&style=none&taskId=ud90e4fea-cc69-4a9e-a8c1-60aa6410a6c&title=&width=957.6)<br />上图中第一个结果中内存数据显示单位是 KB ， 第二个结果中单位是 MB，将第一个结果对应字段的数值除以 1024 就得到了第二个结果
<a name="rnwob"></a>
#### 活跃内存和非活跃内存
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1644114768883-1bd525a9-88dd-478e-b0be-a195c57917b2.png#clientId=ud676dfc3-e4a6-4&from=paste&height=156&id=uf59f6955&originHeight=390&originWidth=2907&originalType=binary&ratio=1&rotation=0&showTitle=false&size=83244&status=done&style=none&taskId=u599f63b0-dcd4-4bc0-b94e-8865e1a0fa8&title=&width=1162.8)<br />inact 是非活跃内存，active 是活跃内存<br />活跃内存是进程在使用的内存，非活跃内存是未运行进程的内存
<a name="eGajb"></a>
#### 系统启动以来 fork 的数量
这里的 fork 数量包括 fork、vfork 以及 clone 等系统调用
```bash
[root@fcant ~]# vmstat -f
        12714 forks
[root@fcant ~]# vmstat -f
        12715 forks
[root@fcant ~]# vmstat -f
        12716 forks
```
每次在控制台执行一次命令，系统就会 fork 一个新的进程来执行命令，比如像上面的例子，每执行一次 `vmstat -f` 命令，系统就会 fork 一个新进程<br />这个选项还可以用于统计某个操作消耗多少次 fork 调用，只需要在操作前后各执行一次 `vmstat -f` 命令，比较两次结果的差值即可
<a name="GA2H2"></a>
#### 每行追加一列时间
追加一列时间显示，有助于比较一段时间内的结果<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1644114732598-549cdc1b-c438-49ca-9656-0e692857728e.png#clientId=ud676dfc3-e4a6-4&from=paste&height=98&id=uc3e8ec2a&originHeight=244&originWidth=2847&originalType=binary&ratio=1&rotation=0&showTitle=false&size=56096&status=done&style=none&taskId=u13828f13-6f42-461b-afe7-da7a3a19843&title=&width=1138.8)
<a name="nv6Pr"></a>
#### 按照宽模式显示
vmstat 结果中的某些字段的数字有时会比较长，而且跟字段名的位置有偏差， 不太适合人类的观看习惯，-w 选项可以按照宽模式显示数据，使结果看起来更直观，下图是分别未使用宽模式和使用了宽模式的一个对比<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1644114791850-ba8c4460-3052-4511-8531-37156f4ccec3.png#clientId=ud676dfc3-e4a6-4&from=paste&height=158&id=uf28b0619&originHeight=394&originWidth=2918&originalType=binary&ratio=1&rotation=0&showTitle=false&size=84109&status=done&style=none&taskId=uae32f928-19c6-4d37-b290-2aad2d95448&title=&width=1167.2)
<a name="Z7wXO"></a>
#### 统计磁盘信息
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1644114824197-fb6f3c4f-5e34-470f-84b1-7eab5db3f867.png#clientId=ud676dfc3-e4a6-4&from=paste&height=79&id=u7f4ef1d2&originHeight=197&originWidth=2636&originalType=binary&ratio=1&rotation=0&showTitle=false&size=42752&status=done&style=none&taskId=uccbfbf5a-8383-4df2-9156-a72b3a99264&title=&width=1054.4)<br />磁盘信息主要分三个方面：读、写、IO ，读和写以毫秒为单位，IO以秒为单位<br />读<br />    total：     成功读取的总数<br />    merged：    分组读取（产生一个 IO）<br />    sectors：   成功读取的扇区数<br />    ms：        读取花费的毫秒<br />    <br />写<br />    total:      成功写入的总数<br />    merged:     分组写入（产生一个 IO）<br />    sectors:    成功写入的扇区数<br />    ms:         写花费的毫秒<br />    <br />IO<br />    cur:    正在进行的IO<br />    s:      IO花费的秒数
<a name="L1Jgo"></a>
#### 指定磁盘分区信息
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1644116704684-2426e34c-b4f6-45b8-9bf2-2fff44ba15e5.png#clientId=ud676dfc3-e4a6-4&from=paste&height=80&id=u15962b49&originHeight=199&originWidth=1961&originalType=binary&ratio=1&rotation=0&showTitle=false&size=39867&status=done&style=none&taskId=u4f05ee20-38b7-4f75-93b1-fc75ac718b7&title=&width=784.4)<br />上图中，输出结果显示 vda3 分区设备的信息，它们包括：读计数、读取的扇区数，写计数，分区写请求总数
