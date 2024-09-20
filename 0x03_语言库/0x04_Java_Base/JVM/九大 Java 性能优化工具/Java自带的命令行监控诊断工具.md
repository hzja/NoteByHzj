<a name="9525dfd8"></a>
## 1、概述
性能诊断是软件工程师在日常工作中需要经常面对和解决的问题，在用户体验至上的今天，解决好应用的性能问题能带来非常大的收益。<br />Java 作为最流行的编程语言之一，其应用性能诊断一直受到业界广泛关注。可能造成 Java 应用出现性能问题的因素非常多，例如线程控制、磁盘读写、数据库访问、网络I/O、垃圾收集等。想要定位这些问题，一款优秀的性能诊断工具必不可少。<br />体会1：使用数据说明问题，使用知识分析问题，使用工具处理问题。<br />体会2：无监控、不调优！
<a name="Ol5BP"></a>
### 简单命令行工具
在刚接触java学习的时候，大家肯定最先了解的两个命令就是javac，java，那么除此之外，还有没有其他的命令可以供我们使用呢？<br />进入到安装jdk的bin目录，发现还有一系列辅助工具。这些辅助工具用来获取目标 JVM 不同方面、不同层次的信息，帮助开发人员很好地解决Java应用程序的一些疑难杂症。

![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1699621696094-472e12ad-3147-478e-8619-f72317bd6d54.png#averageHue=%23fbfafa&clientId=uef90f20a-b76a-4&from=paste&height=656&id=ufbe68c05&originHeight=1640&originWidth=1975&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=262112&status=done&style=none&taskId=ud04674a5-4bbc-4673-b9f3-cb4ae9307b0&title=&width=790)<br />官方源码地址：[http://hg.openjdk.java.net/jdk/jdk11/file/1ddf9a99e4ad/src/jdk.jcmd/share/classes/sun/tools](http://hg.openjdk.java.net/jdk/jdk11/file/1ddf9a99e4ad/src/jdk.jcmd/share/classes/sun/tools)
<a name="ced21557"></a>
## 2、jps：查看正在运行的Java进程
jps(Java Process Status)：显示指定系统内所有的HotSpot虚拟机进程（查看虚拟机进程信息），可用于查询正在运行的虚拟机进程。<br />说明：对于本地虚拟机进程来说，进程的本地虚拟机ID与操作系统的进程ID是一致的，是唯一的。<br />基本使用语法为：jps [options] [hostid]<br />还可以通过追加参数，来打印额外的信息。
<a name="uiUAu"></a>
### options参数

- -q：仅仅显示LVMID（local virtual machine id），即本地虚拟机唯一id。不显示主类的名称等
- -l：输出应用程序主类的全类名 或 如果进程执行的是jar包，则输出jar完整路径
- -m：输出虚拟机进程启动时传递给主类main()的参数
- -v：列出虚拟机进程启动时的JVM参数。比如：-Xms20m -Xmx50m是启动程序指定的jvm参数。

说明：以上参数可以综合使用。<br />补充：如果某 Java 进程关闭了默认开启的UsePerfData参数（即使用参数-XX：-UsePerfData），那么jps命令（以及下面介绍的jstat）将无法探知该Java 进程。
<a name="TUh0q"></a>
### hostid参数
RMI注册表中注册的主机名。如果想要远程监控主机上的 java 程序，需要安装 jstatd。<br />对于具有更严格的安全实践的网络场所而言，可能使用一个自定义的策略文件来显示对特定的可信主机或网络的访问，尽管这种技术容易受到IP地址欺诈攻击。<br />如果安全问题无法使用一个定制的策略文件来处理，那么最安全的操作是不运行jstatd服务器，而是在本地使用jstat和jps工具。
<a name="36f5b1e8"></a>
## 3、jstat：查看JVM统计信息
jstat（JVM Statistics Monitoring Tool）：用于监视虚拟机各种运行状态信息的命令行工具。它可以显示本地或者远程虚拟机进程中的类装载、内存、垃圾收集、JIT编译等运行数据。在没有GUI图形界面，只提供了纯文本控制台环境的服务器上，它将是运行期定位虚拟机性能问题的首选工具。常用于检测垃圾回收问题以及内存泄漏问题。<br />官方文档：[https://docs.oracle.com/javase/8/docs/technotes/tools/unix/jstat.html](https://docs.oracle.com/javase/8/docs/technotes/tools/unix/jstat.html)<br />基本使用语法为：`jstat -<option> [-t] [-h<lines>] <vmid> [<interval> [<count>]]`<br />查看命令相关参数：jstat-h 或 jstat-help<br />其中vmid是进程id号，也就是jps之后看到的前面的号码，如下：<br />![](https://gitee.com/vectorx/ImageCloud/raw/master/others/20210504201704.png#clientId=uef90f20a-b76a-4&height=191&id=hXi3p&originHeight=191&originWidth=360&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uaef0269d-c1f0-4702-81ec-2f651d78172&title=&width=360)
<a name="S5AQJ"></a>
### option参数
选项option可以由以下值构成。<br />类装载相关的：

- -class：显示ClassLoader的相关信息：类的装载、卸载数量、总空间、类装载所消耗的时间等

垃圾回收相关的：

- -gc：显示与GC相关的堆信息。包括Eden区、两个Survivor区、老年代、永久代等的容量、已用空间、GC时间合计等信息。
- -gccapacity：显示内容与-gc基本相同，但输出主要关注Java堆各个区域使用到的最大、最小空间。
- -gcutil：显示内容与-gc基本相同，但输出主要关注已使用空间占总空间的百分比。
- -gccause：与-gcutil功能一样，但是会额外输出导致最后一次或当前正在发生的GC产生的原因。
- -gcnew：显示新生代GC状况
- -gcnewcapacity：显示内容与-gcnew基本相同，输出主要关注使用到的最大、最小空间
- -geold：显示老年代GC状况
- -gcoldcapacity：显示内容与-gcold基本相同，输出主要关注使用到的最大、最小空间
- -gcpermcapacity：显示永久代使用到的最大、最小空间。

JIT相关的：

- -compiler：显示JIT编译器编译过的方法、耗时等信息
- -printcompilation：输出已经被JIT编译的方法
<a name="DtcIo"></a>
### `jstat -class`
![](https://gitee.com/vectorx/ImageCloud/raw/master/others/20210504202501.png#clientId=uef90f20a-b76a-4&height=80&id=DykNI&originHeight=80&originWidth=525&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uea0207ff-b5c1-4128-ad24-598db777afa&title=&width=525)
<a name="s2IPU"></a>
### `jstat -compiler`
![](https://gitee.com/vectorx/ImageCloud/raw/master/others/20210504202507.png#clientId=uef90f20a-b76a-4&height=84&id=fwU7y&originHeight=84&originWidth=1182&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1512bfba-76d5-49ba-bf12-a87e124ccb5&title=&width=1182)
<a name="BGU20"></a>
### `jstat -printcompilation`
![](https://gitee.com/vectorx/ImageCloud/raw/master/others/20210504202510.png#clientId=uef90f20a-b76a-4&height=97&id=xpgKl&originHeight=97&originWidth=598&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u15718a59-aab1-48bd-b931-3e18b059cb8&title=&width=598)
<a name="rY3RV"></a>
### `jstat -gc`
![](https://gitee.com/vectorx/ImageCloud/raw/master/others/20210504202516.png#clientId=uef90f20a-b76a-4&height=305&id=SBVlV&originHeight=305&originWidth=1771&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u45670d07-5d74-46f6-8986-5f06c2b791e&title=&width=1771)
<a name="bZ810"></a>
### `jstat -gccapacity`
![](https://gitee.com/vectorx/ImageCloud/raw/master/others/20210504202522.png#clientId=uef90f20a-b76a-4&height=198&id=Hg7EC&originHeight=198&originWidth=1355&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8c0c8c0e-34be-46cd-a771-8967a25ddb5&title=&width=1355)
<a name="XQJaA"></a>
### `jstat -gcutil`
![](https://gitee.com/vectorx/ImageCloud/raw/master/others/20210504202524.png#clientId=uef90f20a-b76a-4&height=200&id=RcetD&originHeight=200&originWidth=655&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5e0f69c0-feea-4970-8b86-5ed7208a8ad&title=&width=655)
<a name="vsCMM"></a>
### `jstat -gccause`
![](https://gitee.com/vectorx/ImageCloud/raw/master/others/20210504202527.png#clientId=uef90f20a-b76a-4&height=203&id=uV3gN&originHeight=203&originWidth=871&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2d2c5ca3-ec5d-493f-aa0c-52593908abd&title=&width=871)
<a name="CdHQj"></a>
### `jstat -gcnew`
![](https://gitee.com/vectorx/ImageCloud/raw/master/others/20210504202530.png#clientId=uef90f20a-b76a-4&height=197&id=qiJ42&originHeight=197&originWidth=672&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0731dd21-19f6-4257-9218-305a27b6386&title=&width=672)
<a name="HCsWN"></a>
### `jstat -gcnewcapacity`
![](https://gitee.com/vectorx/ImageCloud/raw/master/others/20210504202532.png#clientId=uef90f20a-b76a-4&height=201&id=z35Yt&originHeight=201&originWidth=823&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=udbd5144d-7912-48e9-80a0-34b43ee7dda&title=&width=823)
<a name="wOpQS"></a>
### `jstat -gcold`
![](https://gitee.com/vectorx/ImageCloud/raw/master/others/20210504202534.png#clientId=uef90f20a-b76a-4&height=205&id=jknOi&originHeight=205&originWidth=745&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5b351fb4-014f-4b93-b502-fe87fae5f63&title=&width=745)
<a name="ATUzI"></a>
### `jstat -gcoldcapacity`
![](https://gitee.com/vectorx/ImageCloud/raw/master/others/20210504202537.png#clientId=uef90f20a-b76a-4&height=198&id=m9XQl&originHeight=198&originWidth=625&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u18342ec0-eaad-4709-b45e-3439a8025eb&title=&width=625)
<a name="irMNQ"></a>
### `jstat -t`
![](https://gitee.com/vectorx/ImageCloud/raw/master/others/20210504202539.png#clientId=uef90f20a-b76a-4&height=208&id=CxuDn&originHeight=208&originWidth=1308&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uda73a086-b370-4b13-b0b0-3f6aee27b12&title=&width=1308)
<a name="kjR0v"></a>
### `jstat -t -h`
![](https://gitee.com/vectorx/ImageCloud/raw/master/others/20210504202541.png#clientId=uef90f20a-b76a-4&height=250&id=iEsJq&originHeight=250&originWidth=1324&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u55621b8e-327b-4744-a6ed-bc4be977be7&title=&width=1324)

| 表头 | 含义（字节） |
| --- | --- |
| EC | Eden区的大小 |
| EU | Eden区已使用的大小 |
| S0C | 幸存者0区的大小 |
| S1C | 幸存者1区的大小 |
| S0U | 幸存者0区已使用的大小 |
| S1U | 幸存者1区已使用的大小 |
| MC | 元空间的大小 |
| MU | 元空间已使用的大小 |
| OC | 老年代的大小 |
| OU | 老年代已使用的大小 |
| CCSC | 压缩类空间的大小 |
| CCSU | 压缩类空间已使用的大小 |
| YGC | 从应用程序启动到采样时young gc的次数 |
| YGCT | 从应用程序启动到采样时young gc消耗时间（秒） |
| FGC | 从应用程序启动到采样时full gc的次数 |
| FGCT | 从应用程序启动到采样时的full gc的消耗时间（秒） |
| GCT | 从应用程序启动到采样时gc的总时间 |

**interval参数：**用于指定输出统计数据的周期，单位为毫秒。即：查询间隔<br />**count参数：**用于指定查询的总次数<br />**-t参数：**可以在输出信息前加上一个Timestamp列，显示程序的运行时间。单位：秒<br />**-h参数：**可以在周期性数据输出时，输出多少行数据后输出一个表头信息<br />**补充：**jstat还可以用来判断是否出现内存泄漏。<br />第1步：在长时间运行的 Java 程序中，可以运行jstat命令连续获取多行性能数据，并取这几行数据中 OU 列（即已占用的老年代内存）的最小值。<br />第2步：然后，每隔一段较长的时间重复一次上述操作，来获得多组 OU 最小值。如果这些值呈上涨趋势，则说明该 Java 程序的老年代内存已使用量在不断上涨，这意味着无法回收的对象在不断增加，因此很有可能存在内存泄漏。
<a name="630ea82b"></a>
## 4、jinfo：实时查看和修改JVM配置参数
jinfo(Configuration Info for Java)：查看虚拟机配置参数信息，也可用于调整虚拟机的配置参数。在很多情况卡，Java应用程序不会指定所有的Java虚拟机参数。而此时，开发人员可能不知道某一个具体的Java虚拟机参数的默认值。在这种情况下，可能需要通过查找文档获取某个参数的默认值。这个查找过程可能是非常艰难的。但有了jinfo工具，开发人员可以很方便地找到Java虚拟机参数的当前值。<br />基本使用语法为：`jinfo [options] pid`<br />说明：java 进程ID必须要加上

| 选项 | 选项说明 |
| --- | --- |
| no option | 输出全部的参数和系统属性 |
| -flag name | 输出对应名称的参数 |
| -flag [+-]name | 开启或者关闭对应名称的参数 只有被标记为manageable的参数才可以被动态修改 |
| -flag name=value | 设定对应名称的参数 |
| -flags | 输出全部的参数 |
| -sysprops | 输出系统属性 |

<a name="ZfOgo"></a>
### `jinfo -sysprops`
```properties
> jinfo -sysprops
jboss.modules.system.pkgs = com.intellij.rt
java.vendor = Oracle Corporation
sun.java.launcher = SUN_STANDARD
sun.management.compiler = HotSpot 64-Bit Tiered Compilers
catalina.useNaming = true
os.name = Windows 10
...
```
<a name="C60E9"></a>
### `jinfo -flags`
```shell
> jinfo -flags 25592
Non-default VM flags: -XX:CICompilerCount=4 -XX:InitialHeapSize=333447168 -XX:MaxHeapSize=5324668928 -XX:MaxNewSize=1774714880 -XX:MinHeapDeltaBytes=524288 -XX:NewSize=111149056 -XX:OldSize=222298112 -XX:+UseCompressedClassPointers -XX:+UseCompressedOops -XX:+UseFastUnorderedTimeStamps -XX:-UseLargePagesIndividualAllocation -XX:+UseParallelGC
Command line:  -agentlib:jdwp=transport=dt_socket,address=127.0.0.1:8040,suspend=y,server=n -Drebel.base=C:\Users\Vector\.jrebel -Drebel.env.ide.plugin.version=2021.1.2 -Drebel.env.ide.version=2020.3.3 -Drebel.env.ide.product=IU -Drebel.env.ide=intellij -Drebel.notification.url=http://localhost:7976 -agentpath:C:\Users\Vector\AppData\Roaming\JetBrains\IntelliJIdea2020.3\plugins\jr-ide-idea\lib\jrebel6\lib\jrebel64.dll -Dmaven.home=D:\eclipse\env\maven -Didea.modules.paths.file=C:\Users\Vector\AppData\Local\JetBrains\IntelliJIdea2020.3\Maven\idea-projects-state-596682c7.properties -Dclassworlds.conf=C:\Users\Vector\AppData\Local\Temp\idea-6755-mvn.conf -Dmaven.ext.class.path=D:\IDEA\plugins\maven\lib\maven-event-listener.jar -javaagent:D:\IDEA\plugins\java\lib\rt\debugger-agent.jar -Dfile.encoding=UTF-8
```
<a name="KH17u"></a>
### `jinfo -flag`
```shell
> jinfo -flag UseParallelGC 25592
-XX:+UseParallelGC

> jinfo -flag UseG1GC 25592
-XX:-UseG1GC
```
<a name="NMEy4"></a>
### `jinfo -flag name`
```shell
> jinfo -flag UseParallelGC 25592
-XX:+UseParallelGC

> jinfo -flag UseG1GC 25592
-XX:-UseG1GC
```

<a name="LKT7E"></a>
### `jinfo -flag [+-]name`
```shell
> jinfo -flag +PrintGCDetails 25592
> jinfo -flag PrintGCDetails 25592
-XX:+PrintGCDetails

> jinfo -flag -PrintGCDetails 25592
> jinfo -flag PrintGCDetails 25592
-XX:-PrintGCDetails
```
拓展：

- `java -XX:+PrintFlagsInitial` 查看所有JVM参数启动的初始值
```shell
[Global flags]
     intx ActiveProcessorCount                      = -1                                  {product}
    uintx AdaptiveSizeDecrementScaleFactor          = 4                                   {product}
    uintx AdaptiveSizeMajorGCDecayTimeScale         = 10                                  {product}
    uintx AdaptiveSizePausePolicy                   = 0                                   {product}
...
```

- `java -XX:+PrintFlagsFinal` 查看所有JVM参数的最终值
```shell
[Global flags]
     intx ActiveProcessorCount                      = -1                                  {product}
...
     intx CICompilerCount                          := 4                                   {product}
    uintx InitialHeapSize                          := 333447168                           {product}
    uintx MaxHeapSize                              := 1029701632                          {product}
    uintx MaxNewSize                               := 1774714880                          {product}
```

- `java -XX:+PrintCommandLineFlags` 查看哪些已经被用户或者JVM设置过的详细的XX参数的名称和值
```shell
-XX:InitialHeapSize=332790016 -XX:MaxHeapSize=5324640256 -XX:+PrintCommandLineFlags -XX:+UseCompressedClassPointers -XX:+UseCompressedOops -XX:-UseLargePagesIndividualAllocation -XX:+UseParallelGC
```
<a name="146a85f2"></a>
## 5、jmap：导出内存映像文件&内存使用情况
jmap（JVM Memory Map）：作用一方面是获取dump文件（堆转储快照文件，二进制文件），它还可以获取目标Java进程的内存相关信息，包括Java堆各区域的使用情况、堆中对象的统计信息、类加载信息等。开发人员可以在控制台中输入命令“jmap -help”查阅jmap工具的具体使用方式和一些标准选项配置。<br />官方帮助文档：[https://docs.oracle.com/en/java/javase/11/tools/jmap.html](https://docs.oracle.com/en/java/javase/11/tools/jmap.html)<br />基本使用语法为：

- `jmap [option] <pid>`
- `jmap [option] <executable <core>`
- `jmap [option] [server_id@] <remote server IP or hostname>`
| 选项 | 作用 |
| --- | --- |
| -dump | 生成dump文件（Java堆转储快照），-dump:live只保存堆中的存活对象 |
| -heap | 输出整个堆空间的详细信息，包括GC的使用、堆配置信息，以及内存的使用信息等 |
| -histo | 输出堆空间中对象的统计信息，包括类、实例数量和合计容量，-histo:live只统计堆中的存活对象 |
| -J <flag> | 传递参数给jmap启动的jvm |
| -finalizerinfo | 显示在F-Queue中等待Finalizer线程执行finalize方法的对象，仅linux/solaris平台有效 |
| -permstat | 以ClassLoader为统计口径输出永久代的内存状态信息，仅linux/solaris平台有效 |
| -F | 当虚拟机进程对-dump选项没有任何响应时，强制执行生成dump文件，仅linux/solaris平台有效 |

说明：这些参数和linux下输入显示的命令多少会有不同，包括也受jdk版本的影响。
```shell
> jmap -dump:format=b,file=<filename.hprof> <pid>
> jmap -dump:live,format=b,file=<filename.hprof> <pid>
```
由于jmap将访问堆中的所有对象，为了保证在此过程中不被应用线程干扰，jmap需要借助安全点机制，让所有线程停留在不改变堆中数据的状态。也就是说，由jmap导出的堆快照必定是安全点位置的。这可能导致基于该堆快照的分析结果存在偏差。<br />举个例子，假设在编译生成的机器码中，某些对象的生命周期在两个安全点之间，那么:live选项将无法探知到这些对象。<br />另外，如果某个线程长时间无法跑到安全点，jmap将一直等下去。与前面讲的jstat则不同，垃圾回收器会主动将jstat所需要的摘要数据保存至固定位置之中，而jstat只需直接读取即可。
<a name="1aa88a00"></a>
## 6、jhat：JDK自带堆分析工具
jhat(JVM Heap Analysis Tool)：Sun JDK提供的jhat命令与jmap命令搭配使用，用于分析jmap生成的heap dump文件（堆转储快照）。jhat内置了一个微型的HTTP/HTML服务器，生成dump文件的分析结果后，用户可以在浏览器中查看分析结果（分析虚拟机转储快照信息）。<br />使用了jhat命令，就启动了一个http服务，端口是7000，即http://localhost:7000/，就可以在浏览器里分析。<br />说明：jhat命令在JDK9、JDK10中已经被删除，官方建议用VisualVM代替。<br />基本适用语法：jhat <option> <dumpfile>

| option参数 | 作用 |
| --- | --- |
| -stack false｜true | 关闭｜打开对象分配调用栈跟踪 |
| -refs false｜true | 关闭｜打开对象引用跟踪 |
| -port port-number | 设置jhat HTTP Server的端口号，默认7000 |
| -exclude exclude-file | 执行对象查询时需要排除的数据成员 |
| -baseline exclude-file | 指定一个基准堆转储 |
| -debug int | 设置debug级别 |
| -version | 启动后显示版本信息就退出 |
| -J <flag> | 传入启动参数，比如-J-Xmx512m |

<a name="acf8b47d"></a>
## 7、jstack：打印JVM中线程快照
jstack（JVM Stack Trace）：用于生成虚拟机指定进程当前时刻的线程快照（虚拟机堆栈跟踪）。线程快照就是当前虚拟机内指定进程的每一条线程正在执行的方法堆栈的集合。<br />生成线程快照的作用：可用于定位线程出现长时间停顿的原因，如线程间死锁、死循环、请求外部资源导致的长时间等待等问题。这些都是导致线程长时间停顿的常见原因。当线程出现停顿时，就可以用jstack显示各个线程调用的堆栈情况。<br />官方帮助文档：[https://docs.oracle.com/en/java/javase/11/tools/jstack.html](https://docs.oracle.com/en/java/javase/11/tools/jstack.html)<br />在thread dump中，要留意下面几种状态

- 死锁，Deadlock（重点关注）
- 等待资源，Waiting on condition（重点关注）
- 等待获取监视器，Waiting on monitor entry（重点关注）
- 阻塞，Blocked（重点关注）
- 执行中，Runnable
- 暂停，Suspended
- 对象等待中，`Object.wait()` 或 TIMED＿WAITING
- 停止，Parked
| option参数 | 作用 |
| --- | --- |
| -F | 当正常输出的请求不被响应时，强制输出线程堆栈 |
| -l | 除堆栈外，显示关于锁的附加信息 |
| -m | 如果调用本地方法的话，可以显示C/C++的堆栈 |

<a name="f7d6e90d"></a>
## 8、jcmd：多功能命令行
在JDK 1.7以后，新增了一个命令行工具jcmd。它是一个多功能的工具，可以用来实现前面除了jstat之外所有命令的功能。比如：用它来导出堆、内存使用、查看Java进程、导出线程信息、执行GC、JVM运行时间等。<br />官方帮助文档：[https://docs.oracle.com/en/java/javase/11/tools/jcmd.html](https://docs.oracle.com/en/java/javase/11/tools/jcmd.html)<br />jcmd拥有jmap的大部分功能，并且在Oracle的官方网站上也推荐使用jcmd命令代jmap命令<br />**jcmd -l：**列出所有的JVM进程<br />**jcmd 进程号 help：**针对指定的进程，列出支持的所有具体命令<br />![](https://gitee.com/vectorx/ImageCloud/raw/master/others/20210504213050.png#clientId=uef90f20a-b76a-4&height=583&id=NRKZI&originHeight=583&originWidth=442&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7f1805db-57a2-4f4f-8607-966c1f76a33&title=&width=442)<br />**jcmd 进程号 具体命令：**显示指定进程的指令命令的数据

- `Thread.print` 可以替换 jstack指令
- `GC.class_histogram` 可以替换 jmap中的-histo操作
- `GC.heap_dump` 可以替换 jmap中的-dump操作
- `GC.run` 可以查看GC的执行情况
- `VM.uptime` 可以查看程序的总执行时间，可以替换jstat指令中的-t操作
- `VM.system_properties` 可以替换 `jinfo -sysprops` 进程id
- `VM.flags` 可以获取JVM的配置参数信息
<a name="90675895"></a>
## 9、jstatd：远程主机信息收集
之前的指令只涉及到监控本机的Java应用程序，而在这些工具中，一些监控工具也支持对远程计算机的监控（如jps、jstat）。为了启用远程监控，则需要配合使用jstatd 工具。命令jstatd是一个RMI服务端程序，它的作用相当于代理服务器，建立本地计算机与远程监控工具的通信。jstatd服务器将本机的Java应用程序信息传递到远程计算机。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1699621074018-cc54030c-0fc9-4f30-baa2-26ea3f49287b.jpeg)
