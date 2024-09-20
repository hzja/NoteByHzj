Java jstat
<a name="qcsRo"></a>
# jstat-查看某个进程堆内存的使用情况
<a name="LhPtu"></a>
## jstat命令介绍
`jstat` 主要用于监控 JVM，主要是 GC 信息，在性能优化的时候经常用到<br />首先得明确一下JVM中的堆内存的概念，`堆内存=年轻代+年老代+永久代+元数据`，而`年轻代=Eden区+两个Survivor区（From及To）`<br />一般常用`jstat -gc PID`查看某个进程的堆内存使用情况。

| jstat -gc pid | 显示gc的信息，查看gc的次数及时间 |
| --- | --- |
| jstat -gccapacity pid | 内存GC分区中各对象的使用和占用大小 |
| jstat -gcutil pid | 统计gc信息统计 |
| jstat -gcnew pid | 年轻代对象的信息 |
| jstat -gcnewcapacity pid | 年轻代对象的信息及其占用量 |
| jstat -gcold pid old | 老年代对象的信息 |
| jstat -gcoldcapacity pid old | 老年代对象的信息及其占用量 |
| jstat -gcpermcapacity pid perm | 老年代对象的信息及其占用量 |
| jstat -class pid | 显示加载class的数量，及所占空间等信息 |
| jstat -compiler pid | 显示VM实时编译的数量等信息 |
| jstat -printcompilation pid | 当前VM执行的信息 |

<a name="6bBqN"></a>
## `jstat -help` ：命令的使用
```bash
❯ jstat -help
Usage: jstat -help|-options
       jstat -<option> [-t] [-h<lines>] <vmid> [<interval> [<count>]]

Definitions:
  <option>      An option reported by the -options option
  <vmid>        Virtual Machine Identifier. A vmid takes the following form:
                     <lvmid>[@<hostname>[:<port>]]
                Where <lvmid> is the local vm identifier for the target
                Java virtual machine, typically a process id; <hostname> is
                the name of the host running the target Java virtual machine;
                and <port> is the port number for the rmiregistry on the
                target host. See the jvmstat documentation for a more complete
                description of the Virtual Machine Identifier.
  <lines>       Number of samples between header lines.
  <interval>    Sampling interval. The following forms are allowed:
                    <n>["ms"|"s"]
                Where <n> is an integer and the suffix specifies the units as
                milliseconds("ms") or seconds("s"). The default units are "ms".
  <count>       Number of samples to take before terminating.
  -J<flag>      Pass <flag> directly to the runtime system.
```
**使用语法格式**`jstat -<option> [-t] [-h<lines>] <vmid> [<interval> [<count>]]`<br />**参数解释：**

- option 常见的都是使用-gcutil查看gc情况。
- vmid ：VM的进程号，即当前运行的java进程号。
- interval : 间隔时间，单位为秒或毫秒
- count：打印次数，如果缺省则打印无数次。

参数 interval 和 count 代表查询间隔和次数，如果省略这两个参数，说明只查询一次。假设需要每250毫秒查询一次进程2764垃圾收集的情况，一共查询20次，那么命令应该是：`**jstat -gc 2764 250 20**`<br />用的比较多的是：`jstat -gc PID`和`jstat -gcutil PID`Option Displays...

- `-class`: Statistics on the behavior of the class loader.
- `-compiler`: Statistics of the behavior of the HotSpot Just-in-Time compiler.
- `-gc`: Statistics of the behavior of the garbage collected heap.
- `-gccapacity`: Statistics of the capacities of the generations and their corresponding spaces.
- `-gccause`: Summary of garbage collection statistics (same as -gcutil), with the cause of the last and current (if applicable) garbage collection events.
- `-gcnew`: Statistics of the behavior of the new generation.
- `-gcnewcapacity`: Statistics of the sizes of the new generations and its corresponding spaces.
- `-gcold`: Statistics of the behavior of the old and permanent generations.
- `-gcoldcapacity`: Statistics of the sizes of the old generation.
- `-gcpermcapacity`: Statistics of the sizes of the permanent generation.
- `-gcutil`: Summary of garbage collection statistics. printcompilation HotSpot compilation method statistics.
| **选项** | **作用** |
| --- | --- |
| -class | 监视类装载、卸载数量、中空间及类装载所耗费的时间 |
| -gc | 监视Java堆状况，包括Eden区、2个Survivor区、老年代、永久代等容量、已用空间、GC合计时间等信息 |
| -gccapacity | 监视内容与-gc基本相同，但输出主要关注java堆各区域使用到的最大和最小空间 |
| -gcutil | 监控内容与-gc基本相同，但输出主要关注已使用空间占总空间的百分比 |
| -gccause | 与-gcutil功能一样，但是会额外输出导致上一次GC产生的原因 |
| -gcnew | 监视新生代GC的状况 |
| -gcnewcapacity | 监视内容与-gcnew基本相同，输出主要关注使用到的最大和最小空间 |
| -gcold | 监视老年代GC的状况 |
| -gcoldcapacity | 监视内容与-gcold基本相同，输出主要关注使用到的最大和最小空间 |
| -gcpermcapacity | 输出永久代使用到的最大和最小空间 |
| -compiler | 输出JIT编译器编译过的方法、耗时等信息 |
| -printcompilation | 输出已被JIT编译的方法 |

<a name="y17ju"></a>
## 统计加载类的信息
**命令：**`jstat -class`
```bash
[root@test1117 ~]# jstat -class 21434
Loaded  Bytes  Unloaded  Bytes     Time   
  8776 18268.0        0     0.0       6.05
```
| **列名** | **说明** |
| --- | --- |
| Loaded | 装载的类的数量 |
| Bytes | 装载类所占用的字节数 |
| Unloaded | 卸载类的数量 |
| Bytes | 卸载类所占用的字节数 |
| Time | 装载类和卸载类所耗费的时间(毫秒) |

<a name="rF6gV"></a>
## 编译统计
**命令：**`jstat -compiler`
```bash
[root@test1117 ~]# jstat -compiler 21434
Compiled Failed Invalid   Time   FailedType FailedMethod
    1721      1       0    34.87          1 org/apache/catalina/loader/WebappClassLoaderBase findResourceInternal
```
| **列名** | **说明** |
| --- | --- |
| Compiled | 编译任务执行数量 |
| Failed | 编译任务执行失败的数量 |
| Invalid | 编译任务失效的数量 |
| Time | 编译总耗时（毫秒） |
| FailedType | 最后一个编译失败任务的类型 |
| FailedMethod | 最后一个编译失败任务所在的类及方法 |

<a name="RP4rA"></a>
## 垃圾回收统计
**命令：**`jstat -gc`
```bash
[root@test1117 ~]# jstat -gc 21434
 S0C    S1C    S0U    S1U      EC       EU        OC         OU       PC     PU    YGC     YGCT    FGC    FGCT     GCT   
13312.0 13824.0  0.0   1040.0 146432.0 140089.9  349696.0   252384.5  262144.0 55374.3     47    0.921   0      0.000    0.921
```
Column Description

- `S0C`： Current survivor space 0 capacity (KB).当前 `Survivor0` 的容量
- `S1C`： Current survivor space 1 capacity (KB).当前 `Survivor1` 的容量
- `S0U`： Survivor space 0 utilization (KB).当前 `Survivor0` 的利用率
- `S1U`： Survivor space 1 utilization (KB).当前 `Survivor1` 的利用率
- `EC`：Current eden space capacity (KB).Eden 的容量
- `EU`： Eden space utilization (KB).Eden 的利用率
- `OC`： Current old space capacity (KB).老年代的容量
- `OU`： Old space utilization (KB).老年代的利用率
- `MC`：表示 Metaspace 的容量，`MU` 表示 Metaspace 的利用率，
- `CCSC` 表示类指针压缩空间容量，`CCSU` 表示使用的类指针压缩空间，
- `YGC`： Number of young generation GC Events.新生代 GC 的次数
- `YGCT`： Young generation garbage collection time.新生代 GC 的时间
- `FGC`： Number of full GC events.（**常用）**Full Gc 的次数
- `FGCT`： Full garbage collection time. （**常用** ）Full GC 的时间
- `GCT`： Total garbage collection time.（**常用）**GC 总时间
| **列名** | **说明** |
| --- | --- |
| S0C | 年轻代中第一个survior（幸存区）的容量（kb） |
| S1C | 年轻代中第二个survior（幸存区）的容量（kb） |
| S0U | 年轻代中第一个survior（幸存区）目前已使用的容量（kb） |
| S1U | 年轻代中第二个survior（幸存区）目前已使用的容量（kb） |
| EC | eden区的容量（kb） |
| EU | eden区目前已使用的容量（kb） |
| OC | 老年代的容量（kb） |
| OU | 老年代目前已使用的容量（kb） |
| PC | perm永久代的容量（kb） |
| PU | perm永久代目前已使用的容量（kb） |
| YGC | 从应用程序启动到采集时年轻代中gc次数 |
| YGCT | 从应用程序启动到采集时年轻代中gc所用时间（秒） |
| FGC | 从应用程序启动到采集时老年代中gc次数 |
| FGCT | 从应用程序启动到采集时老年代gc所用的时间（秒） |
| GCT | 从应用程序启动到采集时gc所用的总时间（秒） |

每个对象都有一个指向它自身类的指针，_klass: 指向类的 4 字节指针，64 位平台上 _klass: 指向类的 8 字节的指针，为了节约这些空间，引入了类指针压缩空间。
```bash
❯ jstat -gc 17400
 S0C    S1C    S0U    S1U      EC       EU        OC         OU       MC     MU    CCSC   CCSU   YGC     YGCT    FGC    FGCT     GCT
34048.0 34048.0 34048.0  0.0   272640.0 170423.0  916072.0   569125.8  519136.0 497750.4 72000.0 63945.6    724   14.670   7     10.124   24.794
fcsca@FCANT /                                                                                             [16:45]
❯ jstat -gc 17400 250 10
 S0C    S1C    S0U    S1U      EC       EU        OC         OU       MC     MU    CCSC   CCSU   YGC     YGCT    FGC    FGCT     GCT
34048.0 34048.0  0.0   34048.0 272640.0 39847.5   916072.0   612811.5  519392.0 497771.7 72000.0 63945.6    725   14.740   7     10.124   24.864
34048.0 34048.0  0.0   34048.0 272640.0 39847.5   916072.0   612811.5  519392.0 497771.7 72000.0 63945.6    725   14.740   7     10.124   24.864
34048.0 34048.0  0.0   34048.0 272640.0 39850.1   916072.0   612811.5  519392.0 497771.7 72000.0 63945.6    725   14.740   7     10.124   24.864
34048.0 34048.0  0.0   34048.0 272640.0 39850.1   916072.0   612811.5  519392.0 497771.7 72000.0 63945.6    725   14.740   7     10.124   24.864
34048.0 34048.0  0.0   34048.0 272640.0 39850.1   916072.0   612811.5  519392.0 497771.7 72000.0 63945.6    725   14.740   7     10.124   24.864
34048.0 34048.0  0.0   34048.0 272640.0 39850.1   916072.0   612811.5  519392.0 497771.7 72000.0 63945.6    725   14.740   7     10.124   24.864
34048.0 34048.0  0.0   34048.0 272640.0 39850.1   916072.0   612811.5  519392.0 497771.7 72000.0 63945.6    725   14.740   7     10.124   24.864
34048.0 34048.0  0.0   34048.0 272640.0 39850.1   916072.0   612811.5  519392.0 497771.7 72000.0 63945.6    725   14.740   7     10.124   24.864
34048.0 34048.0  0.0   34048.0 272640.0 39893.0   916072.0   612811.5  519392.0 497771.7 72000.0 63945.6    725   14.740   7     10.124   24.864
34048.0 34048.0  0.0   34048.0 272640.0 39893.0   916072.0   612811.5  519392.0 497771.7 72000.0 63945.6    725   14.740   7     10.124   24.864
fcsca@FCANT /                                                                                                                              [16:46]
❯ jstat -gcutil 17400 250 10
  S0     S1     E      O      M     CCS    YGC     YGCT    FGC    FGCT     GCT
  0.00 100.00  28.93  66.90  95.84  88.81    725   14.740     7   10.124   24.864
  0.00 100.00  28.93  66.90  95.84  88.81    725   14.740     7   10.124   24.864
  0.00 100.00  28.93  66.90  95.84  88.81    725   14.740     7   10.124   24.864
  0.00 100.00  28.93  66.90  95.84  88.81    725   14.740     7   10.124   24.864
  0.00 100.00  28.93  66.90  95.84  88.81    725   14.740     7   10.124   24.864
  0.00 100.00  28.93  66.90  95.84  88.81    725   14.740     7   10.124   24.864
  0.00 100.00  28.93  66.90  95.84  88.81    725   14.740     7   10.124   24.864
  0.00 100.00  28.95  66.90  95.84  88.81    725   14.740     7   10.124   24.864
  0.00 100.00  28.95  66.90  95.84  88.81    725   14.740     7   10.124   24.864
  0.00 100.00  28.95  66.90  95.84  88.81    725   14.740     7   10.124   24.864
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594716475285-40858c54-066f-43ed-8f53-88d8938d8fd7.png#averageHue=%233a3a3a&height=460&id=JExa4&originHeight=1380&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1828055&status=done&style=none&title=&width=1107.6666666666667)
<a name="acfJL"></a>
## 统计gc信息
**命令：**`jstat -gcutil`
```bash
[root@test1117 ~]# jstat -gcutil 21434
  S0     S1     E      O      P     YGC     YGCT    FGC    FGCT     GCT   
  7.81   0.00  46.68  72.18  21.12     48    0.934     0    0.000    0.934
```
| **列名** | **说明** |
| --- | --- |
| S0 | 年轻代中第一个（survisor）幸存区已使用的容量占比 |
| S1 | 年轻代中第二个（survisor）幸存区已使用的容量占比 |
| E | 伊旬园（eden）区已使用的容量占比 |
| O | 老年代区已使用的容量占比 |
| P | 永久代（perm）已使用的容量占比 |
| YGC | 年轻代到目前gc次数 |
| YGCT | 年轻代到目前gc耗费的总时间（秒） |
| FGC | 老年代目前gc次数 |
| FGCT | 老年代目前gc耗费的总时间（秒） |
| GC | 从应用程序到目前gc总耗时（秒） |

<a name="OaX88"></a>
## 堆内存统计
**命令：**`jstat -gccapacity`
```
[root@test1117 ~]# jstat -gccapacity 21434
 NGCMN    NGCMX     NGC     S0C   S1C       EC      OGCMN      OGCMX       OGC         OC      PGCMN    PGCMX     PGC       PC     YGC    FGC 
175104.0 393216.0 175104.0 13312.0 12288.0 149504.0   349696.0   655360.0   349696.0   349696.0 262144.0 524288.0 262144.0 262144.0     48     0
```
| **列名** | **说明** |
| --- | --- |
| NGCMN | 年轻代（young）中初始化（最小）的大小（kb） |
| NGCMX | 年轻代（young）中初始化（最大）的大小（kb） |
| NGC | 年轻代（young）中当前的容量（kb） |
| S0C | 年轻代中第一个（survisor）幸存区的容量（kb） |
| S1C | 年轻代中第二个（survisor）幸存区的容量（kb） |
| EC | 年轻代中（Eden)伊旬园的容量（kb） |
| OGCMN | 老年代（old）中初始化（最小）的容量（kb） |
| OGCMX | 老年代（old）中初始化（最大）的容量（kb） |
| OGC | 当前老年代的大小（kb） |
| OC | 当前老年代的大小（kb） |
| PGCMN | 永久代（perm）中初始化（最小）的大小（kb） |
| PGCMX | 永久代（perm）中初始化（最大）的大小（kb） |
| PGC | 永久代当前的大小（kb） |
| PC | 永久代当前的大小（kb） |
| YGC | 从应用程序启动到采集时年轻代gc的次数 |
| FGC | 从应用程序启动带采集时老年代gc的次数 |

<a name="PF9hB"></a>
## 新生代垃圾回收统计
**命令：**`jstat -gcnew`
```bash
D:\code\IdeaProjects\javafx-demo1\src\test>jstat -gcnew 4016
 S0C    S1C    S0U    S1U   TT MTT  DSS      EC       EU     YGC     YGCT
1024.0 1024.0    0.0    0.0 15  15  512.0   8192.0   7328.4    465    0.120
```
| **列名** | **说明** |
| --- | --- |
| S0C | 年轻代中第一个（survisor）幸存区的容量（kb） |
| S1C | 年轻代中第二个（survisor）幸存区的容量（kb） |
| S0U | 年轻代中第一个（survisor）幸存区目前已使用的容量（kb） |
| S1U | 年轻代中第二个（survisor）幸存区目前已使用的容量（kb） |
| TT | 对象在新生代中存活的次数 |
| MTT | 对象在新生代中存活的最大次数 |
| DSS | 当前需要survivor(幸存区)的容量 (kb) |
| EC | 伊旬园（eden）区的大小（kb） |
| EU | 伊旬园（eden）区已使用的大小（kb） |
| YGC | 到目前年轻代gc的次数 |
| YGCT | 到目前年轻代gc所耗费的时间（秒） |

<a name="tL9Ui"></a>
## 新生代内存统计
**命令：**`jstat -gcnewcapacity`
```bash
D:\code\IdeaProjects\javafx-demo1\src\test>jstat -gcnewcapacity 4016
  NGCMN      NGCMX       NGC      S0CMX     S0C     S1CMX     S1C       ECMX        EC      YGC   FGC
   10240.0    10240.0    10240.0   1024.0   1024.0   1024.0   1024.0     8192.0     8192.0   647     0
```
| **列名** | **说明** |
| --- | --- |
| MGCMN | 年轻代中初始化最小容量（kb） |
| MGCMX | 年轻代中初始化最大容量（kb） |
| NGC | 年轻代当前容量（kb） |
| S0CMX | 年轻代第一个幸存区（survisor）最大容量（kb） |
| S0C | 年轻代第一个幸存区（survisor）当前容量（kb） |
| S1CMX | 年轻代第二个幸存区（survisor）最大容量（kb） |
| S1C | 年轻代第二个幸存区（survisor）当前容量（kb） |
| ECMX | 年轻代伊旬园区（Eden）最大容量（kb） |
| EC | 年轻代伊旬园区（Eden）当前容量（kb） |
| YGC | 截止到目前年轻代gc次数 |
| FGC | 截止到目前老年代gc次数 |

<a name="VP10a"></a>
## 老年代垃圾回收统计
**命令：**`jstat -gcold`
```bash
[root@test1117 ~]# jstat -gcold 21434
   PC       PU        OC          OU       YGC    FGC    FGCT     GCT   
262144.0  55374.3    349696.0    252424.5     48     0    0.000    0.934
```
| **列名** | **说明** |
| --- | --- |
| PC | 永久区（perm）容量（kb） |
| PU | 永久区（perm）已使用容量（kb） |
| OC | 老年代容量（kb） |
| OU | 老年代已使用容量（kb） |
| YGC | 截止到目前年轻代gc次数 |
| FGC | 截止到目前老年代gc次数 |
| GCT | 截止到目前gc耗费的总时间（秒） |

<a name="AtEsW"></a>
## 老年代内存统计
**命令：**`jstat -gcoldcapacity`
```bash
[root@test1117 ~]# jstat -gcoldcapacity 21434
   OGCMN       OGCMX        OGC         OC       YGC   FGC    FGCT     GCT   
   349696.0    655360.0    349696.0    349696.0    48     0    0.000    0.934
```
| **列名** | **说明** |
| --- | --- |
| OGCMN | 老年代最小容量（kb） |
| OGCMX | 老年代最大容量（kb） |
| OGC | 老年代目前生成的容量（kb） |
| OC | 老年代目前容量（kb） |
| YGC | 截止到目前年轻代gc次数 |
| FGC | 截止到目前老年代gc次数 |
| FGCT | 截止到目前老年代gc耗费的总时间（秒） |
| GCT | 截止到目前gc耗费的总时间（秒） |

<a name="q8Q0V"></a>
## 永久代内存统计
**命令：**`jstat -gcpermcapacity`
```bash
[root@test1117 ~]# jstat -gcpermcapacity 21434
  PGCMN      PGCMX       PGC         PC      YGC   FGC    FGCT     GCT   
  262144.0   524288.0   262144.0   262144.0    49     0    0.000    0.948
```
| **列名** | **说明** |
| --- | --- |
| PGCMN | 永久代最小容量（kb） |
| PGCMX | 永久代最大容量（kb） |
| PGC | 永久代当前生成的容量（kb） |
| PC | 永久代当前容量（kb） |
| YGC | 截止目前年轻代gc次数 |
| FGC | 截止目前老年代gc次数 |
| FGCT | 截止目前年轻代gc耗费的总时间（秒） |
| GCT | 截止目前老年代gc耗费的总时间（秒） |

<a name="WyOXj"></a>
## 最近二次gc统计
**命令：**`jstat -gccause`
```bash
[root@test1117 ~]# jstat -gccause 21434
  S0     S1     E      O      P     YGC     YGCT    FGC    FGCT     GCT    LGCC                 GCC                 
  0.00   8.72  33.06  72.20  21.12     49    0.948     0    0.000    0.948 Allocation Failure   No GC
```
| **列名** | **说明** |
| --- | --- |
| LGCC | 最近垃圾回收的原因 |
| GCC | 当前垃圾回收的原因 |

<a name="pXVcP"></a>
## JVM编译方法统计
**命令：**`jstat -printcompilation`
```bash
[root@test1117 ~]# jstat -printcompilation 21434
Compiled  Size  Type Method
    1721    554    1 org/jboss/netty/channel/socket/nio/AbstractNioWorker write0
```
| **列名** | **说明** |
| --- | --- |
| Compiled | 最近编译方法的数量 |
| Size | 最近编译方法的字节码数量 |
| Type | 最近编译方法的编译类型 |
| Method | 方法名标识 |

