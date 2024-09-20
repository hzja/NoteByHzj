<a name="kMTTU"></a>
## 全局监控
<a name="HP5Se"></a>
### dashboard-当前系统的实时数据面板
使用 **dashboard** 命令可以概览程序的 线程、内存、GC、运行环境信息。
```bash
ID        NAME                          GROUP               PRIORITY STATE     %CPU      TIME      INTERRUPT DAEMON
48        Abandoned connection cleanup  main                5        TIMED_WAI 0         0:0       false     true
5         Attach Listener               system              5        RUNNABLE  0         0:0       false     true
35        ContainerBackgroundProcessor[ main                5        TIMED_WAI 0         0:0       false     true
113       DestroyJavaVM                 main                5        RUNNABLE  0         0:23      false     false
49        Druid-ConnectionPool-Create-1 main                5        WAITING   0         0:0       false     true
50        Druid-ConnectionPool-Destroy- main                5        TIMED_WAI 0         0:0       false     true
3         Finalizer                     system              8        WAITING   0         0:0       false     true
86        IdleConnectionMonitorThread   main                5        TIMED_WAI 0         0:0       false     true
410       Keep-Alive-Timer              system              8        TIMED_WAI 0         0:0       false     true
97        NioBlockingSelector.BlockPoll main                5        RUNNABLE  0         0:0       false     true
16        RMI Scheduler(0)              system              5        WAITING   0         0:0       false     true
14        RMI TCP Accept-0              system              5        RUNNABLE  0         0:0       false     true
2         Reference Handler             system              10       WAITING   0         0:0       false     true
Memory                    used    total    max     usage    GC
heap                      608M    813M     3616M   16.84%   gc.ps_scavenge.count          23
ps_eden_space             522M    524M     1300M   40.17%   gc.ps_scavenge.time(ms)       526
ps_survivor_space         21M     21M      21M     99.63%   gc.ps_marksweep.count         3
ps_old_gen                64M     267M     2712M   2.40%    gc.ps_marksweep.time(ms)      770
nonheap                   111M    116M     -1      96.19%
code_cache                28M     29M      240M    12.08%
Runtime
os.name                                                     Windows 10
os.version                                                  10.0
java.version                                                1.8.0_192
java.home                                                   C:\Program Files\Java\jdk1.8.0_192\jre
systemload.average                                          -1.00
processors                                                  4
uptime                                                      7883s
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594878085980-6044ba3f-7d17-4441-b3f2-5c7db4665bed.png#averageHue=%234b4b4b&height=583&id=ALlky&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2029044&status=done&style=none&title=&width=1107.6666666666667)<br />使用Ctrl+C退出实时刷新
<a name="iUXsk"></a>
## 常用命令
| 命令 | 介绍 |
| --- | --- |
| dashboard | 当前系统的实时数据面板 |
| **thread** | 查看当前 JVM 的线程堆栈信息 |
| **watch** | 方法执行数据观测 |
| **trace** | 方法内部调用路径，并输出方法路径上的每个节点上耗时 |
| **stack** | 输出当前方法被调用的调用路径 |
| **tt** | 方法执行数据的时空隧道，记录下指定方法每次调用的入参和返回信息，并能对这些不同的时间下调用进行观测 |
| monitor | 方法执行监控 |
| jvm | 查看当前 JVM 信息 |
| vmoption | 查看，更新 JVM 诊断相关的参数 |
| sc | 查看 JVM 已加载的类信息 |
| sm | 查看已加载类的方法信息 |
| jad | 反编译指定已加载类的源码 |
| classloader | 查看 classloader 的继承树，urls，类加载信息 |
| heapdump | 类似 jmap 命令的 heap dump 功能 |

<a name="EWdFh"></a>
### thread-查看当前线程信息
查看线程的堆栈，可以找出当前最占 CPU 的线程。<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1698243772429-4589e8f7-e099-4773-b675-f273786644b2.png#averageHue=%23404040&clientId=u89a3f6e5-9715-4&from=paste&height=654&id=u55b91a2b&originHeight=1635&originWidth=3325&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=2210964&status=done&style=none&taskId=u9fc4b9e2-11d8-4b0d-9840-3bc9dc96951&title=&width=1330)<br />常用命令：
```bash
# 打印当前最忙的3个线程的堆栈信息
thread -n 3
# 查看ID为1都线程的堆栈信息
thread 1
# 找出当前阻塞其他线程的线程
thread -b
# 查看指定状态的线程
thread -state WAITING
```
<a name="ddo83"></a>
### sysprop-查看当前 JVM 的系统属性
比如当容器时区与宿主机不一致时，可以使用如下命令查看时区信息。
```bash
sysprop | grep timezone
user.timezone                  Asia/Shanghai
```
<a name="HKnNr"></a>
### sysenv-查看 JVM 的环境属性
<a name="SjmMu"></a>
### logger-使用logger命令可以查看日志信息，并改变日志级别
这个命令非常有用。<br />比如在生产环境上一般是不会打印DEBUG级别的日志的，在线上排查问题时可以临时开启DEBUG级别的日志，帮助我们排查问题，下面介绍下如何操作。

- 应用默认使用的是INFO级别的日志，使用logger命令可以查看；

![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1698244328649-1b8d4de8-dc9c-4450-9cab-f097d26c14c6.png#averageHue=%23373737&clientId=u89a3f6e5-9715-4&from=paste&height=364&id=u5667b3fc&originHeight=911&originWidth=3228&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=1149551&status=done&style=none&taskId=ue64cbc31-c968-42f5-8917-58a51df2526&title=&width=1291.2)

- 使用如下命令改变日志级别为DEBUG，需要使用`-c`参数指定类加载器的 HASH 值；
```bash
logger -c 18b4aac2 --name ROOT --level debug
```

- 再使用logger命令查看，发现ROOT级别日志已经更改；

![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1698244901978-240086bb-5cdc-450a-bc03-3398047efcc5.png#averageHue=%23373737&clientId=u89a3f6e5-9715-4&from=paste&height=426&id=ub7b788ff&originHeight=1064&originWidth=3325&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=1372420&status=done&style=none&taskId=u46a7250a-0a89-4ceb-a80b-d67e25b4ef5&title=&width=1330)

- 查看应用日志，发现已经打印了 DEBUG 级别的日志；

![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1698245180306-aefbf83b-d68b-4e9a-a29d-1d81709670fa.png#averageHue=%23f9f8f7&clientId=u04bc7639-e0b1-4&from=paste&height=295&id=u2f584d4b&originHeight=737&originWidth=2338&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=284182&status=done&style=none&taskId=ub8a7ea00-2fd8-4136-a1a8-9ee17121fdc&title=&width=935.2)

- 查看完日志以后记得要把日志级别再调回INFO级别。
```bash
logger -c 18b4aac2 --name ROOT --level info
```
<a name="ZgFvz"></a>
### sc-查看 JVM 已加载的类信息
Search-Class的简写，搜索出所有已经加载到 JVM 中的类信息。

- 搜索com.fcant包下所有的类；
```bash
sc com.fcant.*
```
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1698245423455-cc7ad3c1-8637-42ae-8ec4-44359fcef1b1.png#averageHue=%23404040&clientId=u04bc7639-e0b1-4&from=paste&height=256&id=ue6b06a06&originHeight=639&originWidth=2054&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=375254&status=done&style=none&taskId=u07871ac4-4d17-425b-a7fe-8b27a8064a5&title=&width=821.6)

- 打印类的详细信息，加入`-d`参数并指定全限定类名；
```bash
sc -d com.fcant.common.api.CommonResult
```
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1698245741687-bccc28b8-a15f-4964-a10d-03334fbdad17.png#averageHue=%233a3a3a&clientId=u04bc7639-e0b1-4&from=paste&height=654&id=uba1d7220&originHeight=1636&originWidth=2512&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=1153878&status=done&style=none&taskId=uf9b1f16a-8de4-4c5d-aaec-e610b2a4172&title=&width=1004.8)

- 打印出类的 Field 信息，使用`-f`参数。
```bash
sc -d -f com.fcant.blog.utils.NettyServerUtils
```
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1698245988863-a8bf3dda-63a7-4e5e-8624-8155c2b90d5d.png#averageHue=%23373737&clientId=u04bc7639-e0b1-4&from=paste&height=227&id=u967bf9cb&originHeight=568&originWidth=2311&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=319876&status=done&style=none&taskId=u54621bd6-8139-4d4f-ae04-aa4a61c6efa&title=&width=924.4)
<a name="a4TSM"></a>
### sm-查看已加载类的方法信息
Search-Method的简写，搜索出所有已经加载的类的方法信息。

- 查看类中的所有方法；
```
sm com.fcant.blog.utils.NettyServerUtils
```
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1698246112495-d64dfba0-7736-421c-9e5c-4b630251c216.png#averageHue=%23414141&clientId=u04bc7639-e0b1-4&from=paste&height=133&id=u42052a03&originHeight=333&originWidth=2212&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=204006&status=done&style=none&taskId=u41e2f9de-0b6a-4b36-84b2-82e56f4dbd3&title=&width=884.8)

- 查看指定方法信息，使用-d参数并指定方法名称；
```
sm -d com.fcant.blog.utils.NettyServerUtils ServerRun
```
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1698246659114-7de73ea6-07e5-461f-a1b1-1d904ffc5aa3.png#averageHue=%233a3a3a&clientId=uf66db7e7-f958-4&from=paste&height=236&id=u077e860a&originHeight=590&originWidth=2010&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=468192&status=done&style=none&taskId=ufe87676a-7487-44cf-9c4e-614c4c0a419&title=&width=804)
<a name="Xq55V"></a>
### monitor-实时监控方法执行信息
可以查看方法执行成功此时、失败次数、平均耗时等信息。
```bash
monitor -c 5 com.fcant.blog.controller.TestController test
```
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1698248666125-c111256e-3b36-4443-858a-1a323d4042bc.png#averageHue=%23373737&clientId=ufaae9b93-d642-4&from=paste&height=308&id=DeJNf&originHeight=769&originWidth=3284&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=1012481&status=done&style=none&taskId=u79cd90a9-99ec-4db0-b166-c8df9f3de39&title=&width=1313.6)
<a name="U2ldm"></a>
### watch-方法执行数据观测
可以观察方法执行过程中的参数和返回值。<br />使用如下命令观察方法执行参数和返回值，`-x`表示结果属性遍历深度。
```bash
watch com.fcant.blog.controller.TestController test "{params,returnObj}" -x 2
```
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1698249127977-9f6f59a3-f00e-47fc-b290-9e61616ba05d.png#averageHue=%233c3c3c&clientId=ufaae9b93-d642-4&from=paste&height=199&id=ZT60s&originHeight=497&originWidth=2620&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=507897&status=done&style=none&taskId=u6f8a5e9a-3c61-4435-80ba-e060252ecfc&title=&width=1048)
<a name="CUAxD"></a>
### jvm-查看当前 JVM 信息
```bash
[arthas@2096]$ jvm
 RUNTIME
-----------------------------------------------------------------------------------------------------------------------
 MACHINE-NAME                      2096@Fcant
 JVM-START-TIME                    2020-07-18 16:16:09
 MANAGEMENT-SPEC-VERSION           1.2
 SPEC-NAME                         Java Virtual Machine Specification
 SPEC-VENDOR                       Oracle Corporation
 SPEC-VERSION                      1.8
 VM-NAME                           Java HotSpot(TM) 64-Bit Server VM
 VM-VENDOR                         Oracle Corporation
 VM-VERSION                        25.192-b12
 INPUT-ARGUMENTS                   -agentlib:jdwp=transport=dt_socket,address=127.0.0.1:58815,suspend=y,server=n
                                   -XX:TieredStopAtLevel=1
                                   -Xverify:none
                                   -Dspring.output.ansi.enabled=always
                                   -Dcom.sun.management.jmxremote
                                   -Dspring.jmx.enabled=true
                                   -Dspring.liveBeansView.mbeanDomain
                                   -Dspring.application.admin.enabled=true
                                   -javaagent:D:\LinkSpace\Download\Jetbrains\ideaIU-2020.1.2.win\plugins\java\lib\rt\
                                   debugger-agent.jar
                                   -Dfile.encoding=UTF-8

 CLASS-PATH                        C:\Program Files\Java\jdk1.8.0_192\jre\lib\charsets.jar;
 BOOT-CLASS-PATH                   C:\Program Files\Java\jdk1.8.0_192\jre\lib\resources.jar;
 LIBRARY-PATH                      C:\Program Files\Java\jdk1.8.0_192\bin;

-----------------------------------------------------------------------------------------------------------------------
 CLASS-LOADING
-----------------------------------------------------------------------------------------------------------------------
 LOADED-CLASS-COUNT                12278
 TOTAL-LOADED-CLASS-COUNT          12278
 UNLOADED-CLASS-COUNT              0
 IS-VERBOSE                        false

-----------------------------------------------------------------------------------------------------------------------
 COMPILATION
-----------------------------------------------------------------------------------------------------------------------
 NAME                              HotSpot 64-Bit Tiered Compilers
 TOTAL-COMPILE-TIME                3769(ms)

-----------------------------------------------------------------------------------------------------------------------
 GARBAGE-COLLECTORS
-----------------------------------------------------------------------------------------------------------------------
 PS Scavenge                       15/273(ms)
 [count/time]
 PS MarkSweep                      3/586(ms)
 [count/time]

-----------------------------------------------------------------------------------------------------------------------
 MEMORY-MANAGERS
-----------------------------------------------------------------------------------------------------------------------
 CodeCacheManager                  Code Cache

 Metaspace Manager                 Metaspace
                                   Compressed Class Space

 PS Scavenge                       PS Eden Space
                                   PS Survivor Space

 PS MarkSweep                      PS Eden Space
                                   PS Survivor Space
                                   PS Old Gen


-----------------------------------------------------------------------------------------------------------------------
 MEMORY
-----------------------------------------------------------------------------------------------------------------------
 HEAP-MEMORY-USAGE                 1038090240(990.0 MiB)/268435456(256.0 MiB)/3791650816(3.5 GiB)/727045728(693.4 MiB)
 [committed/init/max/used]
 NO-HEAP-MEMORY-USAGE              101081088(96.4 MiB)/2555904(2.4 MiB)/-1(-1 B)/97810432(93.3 MiB)
 [committed/init/max/used]
 PENDING-FINALIZE-COUNT            0

-----------------------------------------------------------------------------------------------------------------------
 OPERATING-SYSTEM
-----------------------------------------------------------------------------------------------------------------------
 OS                                Windows 10
 ARCH                              amd64
 PROCESSORS-COUNT                  4
 LOAD-AVERAGE                      -1.0
 VERSION                           10.0

-----------------------------------------------------------------------------------------------------------------------
 THREAD
-----------------------------------------------------------------------------------------------------------------------
 COUNT                             39
 DAEMON-COUNT                      32
 PEAK-COUNT                        39
 STARTED-COUNT                     153
 DEADLOCK-COUNT                    0

-----------------------------------------------------------------------------------------------------------------------
 FILE-DESCRIPTOR
-----------------------------------------------------------------------------------------------------------------------
 MAX-FILE-DESCRIPTOR-COUNT         -1
 OPEN-FILE-DESCRIPTOR-COUNT        -1
Affect(row-cnt:0) cost in 79 ms.
```
<a name="WvOpx"></a>
### vmoption-查看，更新 JVM 诊断相关的参数
```bash
[arthas@2096]$ vmoption -h
 USAGE:
   vmoption [-h] [name] [value]

 SUMMARY:
   Display, and update the vm diagnostic options.

 Examples:
   vmoption
   vmoption PrintGCDetails
   vmoption PrintGCDetails true

 WIKI:
   https://alibaba.github.io/arthas/vmoption

 OPTIONS:
 -h, --help                              this help
 <name>                                  VMOption name
 <value>                                 VMOption value
[arthas@2096]$ vmoption
 KEY                           VALUE                         ORIGIN                       WRITEABLE
-----------------------------------------------------------------------------------------------------------------------
 HeapDumpBeforeFullGC          false                         DEFAULT                      true
 HeapDumpAfterFullGC           false                         DEFAULT                      true
 HeapDumpOnOutOfMemoryError    false                         DEFAULT                      true
 HeapDumpPath                                                DEFAULT                      true
 CMSAbortablePrecleanWaitMill  100                           DEFAULT                      true
 is
 CMSWaitDuration               2000                          DEFAULT                      true
 CMSTriggerInterval            -1                            DEFAULT                      true
 PrintGC                       false                         DEFAULT                      true
 PrintGCDetails                false                         DEFAULT                      true
 PrintGCDateStamps             false                         DEFAULT                      true
 PrintGCTimeStamps             false                         DEFAULT                      true
 PrintGCID                     false                         DEFAULT                      true
 PrintClassHistogramBeforeFul  false                         DEFAULT                      true
 lGC
 PrintClassHistogramAfterFull  false                         DEFAULT                      true
 GC
 PrintClassHistogram           false                         DEFAULT                      true
 MinHeapFreeRatio              0                             DEFAULT                      true
 MaxHeapFreeRatio              100                           DEFAULT                      true
 PrintConcurrentLocks          false                         DEFAULT                      true
 UnlockCommercialFeatures      false                         DEFAULT                      true
[arthas@2096]$ vmoption PrintGCDetails
 KEY                           VALUE                         ORIGIN                       WRITEABLE
-----------------------------------------------------------------------------------------------------------------------
 PrintGCDetails                false                         DEFAULT                      true
[arthas@2096]$ vmoption PrintGCDetails true
Successfully updated the vm option.
 NAME            BEFORE-VALUE  AFTER-VALUE
-------------------------------------------
 PrintGCDetails  false         true
 [arthas@2096]$
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595061637764-23ff66a1-9c2e-4f8b-91b4-b827020aa9c8.png#averageHue=%233b3b3b&height=687&id=ic97q&originHeight=2060&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2782013&status=done&style=none&title=&width=1280)
<a name="oVmhN"></a>
### jad-反编译指定已加载类的源码
反编译已加载类的源码，觉得线上代码和预期不一致，可以反编译看看。

- 查看启动类的相关信息，默认会带有ClassLoader信息；
```
jad com.fcant.blog.BlogApplication
```
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1698248114439-ffd49f9b-c957-4c1f-b8f9-67a6f3189752.png#averageHue=%23353535&clientId=uf66db7e7-f958-4&from=paste&height=541&id=u4558d2c0&originHeight=1353&originWidth=2123&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=1145089&status=done&style=none&taskId=u23830d8b-d59e-4a70-9c5d-43269317289&title=&width=849.2)

- 使用`--source-only`参数可以只打印类信息。
```bash
# 反编译只显示源码
jad --source-only com.Arthas
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595033800914-81553b23-ab3c-485f-924e-ddd2e9f0833d.png#averageHue=%23383838&height=687&id=rGMwz&originHeight=2060&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2843464&status=done&style=none&title=&width=1280)
```bash
# 反编译某个类的某个方法
jad --source-only com.Arthas mysql
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595034002757-29afc5e4-3d96-48ea-94be-1e49faf2a608.png#averageHue=%233d3d3d&height=583&id=ZF6oJ&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2118465&status=done&style=none&title=&width=1107.6666666666667)
<a name="BEqF2"></a>
### mc-内存编译器
Memory Compiler的缩写，编译.java文件生成.class。
<a name="dkCG2"></a>
### classloader-查看 classloader 的继承树，urls，类加载信息
<a name="KhEdD"></a>
### heapdump-类似 jmap 命令的 heap dump 功能
<a name="Gys4s"></a>
### redefine-实现热部署
加载外部的.class文件，覆盖掉 JVM 中已经加载的类。
<a name="MtYSt"></a>
#### ①通过 `sc -d <ClassName>` 获取类的信息（主要为获取classLoaderHash）
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595069305775-a4a588fe-3bbc-45bb-a44c-8fccc260352c.png#averageHue=%23414141&height=469&id=GMdDM&originHeight=1407&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1663729&status=done&style=none&title=&width=1107.6666666666667)
<a name="jkt0q"></a>
#### ②使用内存编译器把改.java文件编译成.class文件
注意需要使用-c指定类加载器；
```bash
mc -c 160b5db0 /tmp/PersonServiceImpl.java -d /tmp
```
<a name="KJHjz"></a>
#### ③进行热部署替换
```bash
redefine -c 160b5db0 /tmp/PersonServiceImpl.class
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595069627948-13428889-d7bd-428e-aba8-89dc95efb63a.png#averageHue=%23404040&height=583&id=WTcPr&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2221689&status=done&style=none&title=&width=1107.6666666666667)
<a name="Q8uqc"></a>
### jobs-执行后台异步任务
线上有些问题是偶然发生的，这时就需要使用异步任务，把信息写入文件。<br />使用& 指定命令去后台运行，使用 `>` 将结果重写到日志文件，以trace为例
```bash
trace -j cn.test.mobile.controller.order.OrderController getOrderInfo > test.out &
```
jobs——列出所有job
```
jobs
[76]*  
       Running           trace -j cn.test.mobile.controller.order.OrderController getOrderInfo >> test.out &
       execution count : 0
       start time      : Wed Nov 13 16:13:23 CST 2019
       timeout date    : Thu Nov 14 16:13:23 CST 2019
       session         : f4fba846-e90b-4234-959e-e78ad0a5db8c (current)
```
jobid是76，* 表示此job是当前session创建，状态是Running，execution count是执行次数，timeout date是超时时间<br />异步执行时间，默认为1天，如果要修改，使用options命令：
```bash
options job-timeout 2d
```
options可选参数 1d, 2h, 3m, 25s，分别代表天、小时、分、秒<br />kill——强制终止任务
```bash
kill 76
kill job 76 success
```
最多同时支持8个命令使用重定向将结果写日志，请勿同时开启过多的后台异步命令，以免对目标JVM性能造成影响。
<a name="xDu2I"></a>
## 退出命令
使用 shutdown 退出时 **Arthas** 同时自动重置所有增强过的类 。
