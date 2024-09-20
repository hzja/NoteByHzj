<a name="88042b2b"></a>
## 1、工具概述
命令行工具或组合能获取目标Java应用性能相关的基础信息，但它们存在下列局限：

1. 无法获取方法级别的分析数据，如方法间的调用关系、各方法的调用次数和调用时间等（这对定位应用性能瓶颈至关重要）。
2. 要求用户登录到目标 Java 应用所在的宿主机上，使用起来不是很方便。
3. 分析数据通过终端输出，结果展示不够直观。

为此，JDK提供了一些内存泄漏的分析工具，如jconsole，jvisualvm等，用于辅助开发人员定位问题，但是这些工具很多时候并不足以满足快速定位的需求。所以这里介绍的工具相对多一些、丰富一些。
<a name="nOQp2"></a>
### JDK自带的工具

- jconsole：JDK自带的可视化监控工具。查看Java应用程序的运行概况、监控堆信息、永久区（或元空间）使用情况、类加载情况等 
- Visual VM：Visual VM是一个工具，它提供了一个可视界面，用于查看Java虚拟机上运行的基于Java技术的应用程序的详细信息。 
- JMC：Java Mission Control，内置Java Flight Recorder。能够以极低的性能开销收集Java虚拟机的性能数据。 
<a name="w8eCI"></a>
### 第三方工具

-  MAT：MAT（Memory Analyzer Tool）是基于Eclipse的内存分析工具，是一个快速、功能丰富的Java heap分析工具，它可以帮助我们查找内存泄漏和减少内存消耗
-  JProfiler：商业软件，需要付费。功能强大。
<a name="d41befc0"></a>
## 2、JConsole
jconsole：从Java5开始，在JDK中自带的java监控和管理控制台。用于对JVM中内存、线程和类等的监控，是一个基于JMX（java management extensions）的GUI性能监控工具。<br />官方地址：[https://docs.oracle.com/javase/7/docs/technotes/guides/management/jconsole.html](https://docs.oracle.com/javase/7/docs/technotes/guides/management/jconsole.html)<br />![](https://gitee.com/vectorx/ImageCloud/raw/master/others/20210505141653.png#clientId=u47c4d581-aed1-4&id=WENFV&originHeight=1016&originWidth=1920&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2ee12d56-bfdf-4dc0-89c2-9a1639502bf&title=)<br />![](https://gitee.com/vectorx/ImageCloud/raw/master/others/20210505141728.png#clientId=u47c4d581-aed1-4&id=bkGzv&originHeight=1015&originWidth=1920&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue8b97a79-d927-4c1f-a42c-bb3df1a3a70&title=)<br />![](https://gitee.com/vectorx/ImageCloud/raw/master/others/20210505141930.png#clientId=u47c4d581-aed1-4&id=wKxyR&originHeight=1012&originWidth=1920&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub593e3ea-87e7-41c0-8ba4-58d1e1a3ac2&title=)<br />![](https://gitee.com/vectorx/ImageCloud/raw/master/others/20210505141952.png#clientId=u47c4d581-aed1-4&id=XnGZw&originHeight=943&originWidth=1920&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0bcb9729-4681-4177-bb63-ac3a33c1ba9&title=)<br />![](https://gitee.com/vectorx/ImageCloud/raw/master/others/20210505142053.png#clientId=u47c4d581-aed1-4&id=rjjd0&originHeight=815&originWidth=1919&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u978f3a7d-2d77-48a7-916b-552f1450e54&title=)
<a name="6fd1f587"></a>
## 3、Visual VM
Visual VM是一个功能强大的多合一故障诊断和性能监控的可视化工具。它集成了多个JDK命令行工具，使用Visual VM可用于显示虚拟机进程及进程的配置和环境信息（jps，jinfo），监视应用程序的CPU、GC、堆、方法区及线程的信息（jstat、jstack）等，甚至代替JConsole。在JDK 6 Update 7以后，Visual VM便作为JDK的一部分发布（VisualVM 在JDK／bin目录下）即：它完全免费。<br />**主要功能：**

1. 生成/读取堆内存/线程快照
2. 查看JVM参数和系统属性
3. 查看运行中的虚拟机进程
4. 程序资源的实时监控
5. JMX代理连接、远程环境监控、CPU分析和内存分析

官方地址：[https://visualvm.github.io/index.html](https://visualvm.github.io/index.html)<br />![](https://gitee.com/vectorx/ImageCloud/raw/master/others/20210505144206.png#clientId=u47c4d581-aed1-4&id=QIUw1&originHeight=836&originWidth=1322&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1ec67c40-c375-4dfb-80f1-7baa41b7585&title=)<br />![](https://gitee.com/vectorx/ImageCloud/raw/master/img/20210505144718.png#clientId=u47c4d581-aed1-4&id=Ma2JF&originHeight=895&originWidth=1384&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue3fbfd85-557c-4fea-a581-c997ee1bcf5&title=)<br />![](https://gitee.com/vectorx/ImageCloud/raw/master/img/20210505144806.png#clientId=u47c4d581-aed1-4&id=a6AyU&originHeight=482&originWidth=1388&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1ccaf6cd-6172-455b-8fb7-3e3c0f2d511&title=)
<a name="173d2662"></a>
## 4、Eclipse MAT
MAT（Memory Analyzer Tool）工具是一款功能强大的Java堆内存分析器。可以用于查找内存泄漏以及查看内存消耗情况。MAT是基于Eclipse开发的，不仅可以单独使用，还可以作为插件的形式嵌入在Eclipse中使用。是一款免费的性能分析工具，使用起来非常方便。<br />MAT可以分析heap dump文件。在进行内存分析时，只要获得了反映当前设备内存映像的hprof文件，通过MAT打开就可以直观地看到当前的内存信息。一般说来，这些内存信息包含：

- 所有的对象信息，包括对象实例、成员变量、存储于栈中的基本类型值和存储于堆中的其他对象的引用值。
- 所有的类信息，包括classloader、类名称、父类、静态变量等
- GCRoot到所有的这些对象的引用路径
- 线程信息，包括线程的调用栈及此线程的线程局部变量（TLS）

MAT 不是一个万能工具，它并不能处理所有类型的堆存储文件。但是比较主流的厂家和格式，例如Sun，HP，SAP 所采用的 HPROF 二进制堆存储文件，以及 IBM的 PHD 堆存储文件等都能被很好的解析。<br />最吸引人的还是能够快速为开发人员生成内存泄漏报表，方便定位问题和分析问题。虽然MAT有如此强大的功能，但是内存分析也没有简单到一键完成的程度，很多内存问题还是需要从MAT展现给我们的信息当中通过经验和直觉来判断才能发现。<br />官方地址： [https://www.eclipse.org/mat/downloads.php](https://www.eclipse.org/mat/downloads.php)<br />![](https://gitee.com/vectorx/ImageCloud/raw/master/img/20210505145710.png#clientId=u47c4d581-aed1-4&id=bAyfa&originHeight=973&originWidth=1388&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u992f622c-3626-4b30-94a6-d865dabf792&title=)<br />![](https://gitee.com/vectorx/ImageCloud/raw/master/img/20210505145828.png#clientId=u47c4d581-aed1-4&id=fpHMC&originHeight=723&originWidth=1367&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1f4fc922-75b8-4635-905d-f59fec090e0&title=)<br />![](https://gitee.com/vectorx/ImageCloud/raw/master/img/20210505145951.png#clientId=u47c4d581-aed1-4&id=HWMWz&originHeight=322&originWidth=1375&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u269a1ad9-1616-4418-86c0-c1599a7c14b&title=)<br />![](https://gitee.com/vectorx/ImageCloud/raw/master/img/20210505150040.png#clientId=u47c4d581-aed1-4&id=dh59j&originHeight=869&originWidth=1371&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud9b64c75-b7ce-4c78-9136-fca4b1b39c9&title=)
<a name="60e6cbe8"></a>
## 5、JProfiler
在运行Java的时候有时候想测试运行时占用内存情况，这时候就需要使用测试工具查看了。在eclipse里面有 Eclipse Memory Analyzer tool（MAT）插件可以测试，而在IDEA中也有这么一个插件，就是JProfiler。JProfiler 是由 ej-technologies 公司开发的一款 Java 应用性能诊断工具。功能强大，但是收费。
<a name="FZiUU"></a>
### 特点：

- 使用方便、界面操作友好（简单且强大）
- 对被分析的应用影响小（提供模板）
- CPU，Thread，Memory分析功能尤其强大
- 支持对jdbc，noSql，jsp，servlet，socket等进行分析
- 支持多种模式（离线，在线）的分析
- 支持监控本地、远程的JVM
- 跨平台，拥有多种操作系统的安装版本
<a name="tFtxF"></a>
### 主要功能：

- 1-方法调用：对方法调用的分析可以帮助您了解应用程序正在做什么，并找到提高其性能的方法
- 2-内存分配：通过分析堆上对象、引用链和垃圾收集能帮您修复内存泄露问题，优化内存使用
- 3-线程和锁：JProfiler提供多种针对线程和锁的分析视图助您发现多线程问题
- 4-高级子系统：许多性能问题都发生在更高的语义级别上。例如，对于JDBC调用，可能希望找出执行最慢的SQL语句。JProfiler支持对这些子系统进行集成分析

官网地址：[https://www.ej-technologies.com/products/jprofiler/overview.html](https://www.ej-technologies.com/products/jprofiler/overview.html)
<a name="MqU7J"></a>
### 数据采集方式：
JProfier数据采集方式分为两种：Sampling（样本采集）和Instrumentation（重构模式）<br />**Instrumentation**：这是JProfiler全功能模式。在class加载之前，JProfier把相关功能代码写入到需要分析的class的bytecode中，对正在运行的jvm有一定影响。

- 优点：功能强大。在此设置中，调用堆栈信息是准确的。
- 缺点：若要分析的class较多，则对应用的性能影响较大，CPU开销可能很高（取决于Filter的控制）。因此使用此模式一般配合Filter使用，只对特定的类或包进行分析

**Sampling**：类似于样本统计，每隔一定时间（5ms）将每个线程栈中方法栈中的信息统计出来。

- 优点：对CPU的开销非常低，对应用影响小（即使你不配置任何Filter）
- 缺点：一些数据／特性不能提供（例如：方法的调用次数、执行时间）

注：JProfiler本身没有指出数据的采集类型，这里的采集类型是针对方法调用的采集类型。因为JProfiler的绝大多数核心功能都依赖方法调用采集的数据，所以可以直接认为是JProfiler的数据采集类型。
<a name="nIPHz"></a>
### 遥感监测 Telemetries
![](https://gitee.com/vectorx/ImageCloud/raw/master/img/20210505164523.png#clientId=u47c4d581-aed1-4&id=X8YOB&originHeight=817&originWidth=1481&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u10d95037-7146-4492-a585-bfaa47aafe7&title=)<br />![](https://gitee.com/vectorx/ImageCloud/raw/master/img/20210505164909.png#clientId=u47c4d581-aed1-4&id=Su2RV&originHeight=866&originWidth=1915&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7f40b8b3-a9c8-4058-a27a-105c8b0f0f4&title=)<br />![](https://gitee.com/vectorx/ImageCloud/raw/master/img/20210505164918.png#clientId=u47c4d581-aed1-4&id=Z6yw9&originHeight=860&originWidth=1905&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7d01188b-969c-4f05-ad87-9f23e85b070&title=)<br />![](https://gitee.com/vectorx/ImageCloud/raw/master/img/20210505164947.png#clientId=u47c4d581-aed1-4&id=gyCAy&originHeight=857&originWidth=1911&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u04d219db-efab-45ad-a4f4-a0b46275064&title=)<br />![](https://gitee.com/vectorx/ImageCloud/raw/master/img/20210505165012.png#clientId=u47c4d581-aed1-4&id=JNboY&originHeight=852&originWidth=1910&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u755f2bf0-2f2d-4fe6-82b5-44f7b79e7a3&title=)<br />![](https://gitee.com/vectorx/ImageCloud/raw/master/img/20210505165215.png#clientId=u47c4d581-aed1-4&id=Vwz5I&originHeight=863&originWidth=1908&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u686c274d-0201-426d-b296-fb21e5ffda5&title=)<br />![](https://gitee.com/vectorx/ImageCloud/raw/master/img/20210505165252.png#clientId=u47c4d581-aed1-4&id=zsqtZ&originHeight=855&originWidth=1904&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3a3c61b5-65a3-4f46-953d-bb771061a71&title=)
<a name="y61Hz"></a>
### 内存视图 Live Memory
Live memory 内存剖析：class／class instance的相关信息。例如对象的个数，大小，对象创建的方法执行栈，对象创建的热点。

- **所有对象 All Objects**：显示所有加载的类的列表和在堆上分配的实例数。只有Java 1.5（JVMTI）才会显示此视图。
- **记录对象 Record Objects**：查看特定时间段对象的分配，并记录分配的调用堆栈。
- **分配访问树 Allocation Call Tree**：显示一棵请求树或者方法、类、包或对已选择类有带注释的分配信息的J2EE组件。
- **分配热点 Allocation Hot Spots**：显示一个列表，包括方法、类、包或分配已选类的J2EE组件。你可以标注当前值并且显示差异值。对于每个热点都可以显示它的跟踪记录树。
- **类追踪器 Class Tracker**：类跟踪视图可以包含任意数量的图表，显示选定的类和包的实例与时间。

![](https://gitee.com/vectorx/ImageCloud/raw/master/img/20210505164556.png#clientId=u47c4d581-aed1-4&id=qsokU&originHeight=819&originWidth=1479&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7102b88d-4d08-4924-8aa1-d7e1c6a38cf&title=)<br />![](https://gitee.com/vectorx/ImageCloud/raw/master/img/20210505165521.png#clientId=u47c4d581-aed1-4&id=Ashmw&originHeight=810&originWidth=1906&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2aaf4a0a-ade7-410c-981b-767e8b108ab&title=)
<a name="ousGX"></a>
### 堆遍历 heap walker
![](https://gitee.com/vectorx/ImageCloud/raw/master/img/20210505165712.png#clientId=u47c4d581-aed1-4&id=otWL8&originHeight=855&originWidth=1909&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u94f02280-20f1-408a-b959-e0571bc532e&title=)<br />![](https://gitee.com/vectorx/ImageCloud/raw/master/img/20210505193750.png#clientId=u47c4d581-aed1-4&id=r91gR&originHeight=839&originWidth=1899&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3424dbbd-641f-4550-9049-b618e884912&title=)
<a name="VzE0o"></a>
### cpu视图 cpu views
JProfiler 提供不同的方法来记录访问树以优化性能和细节。线程或者线程组以及线程状况可以被所有的视图选择。所有的视图都可以聚集到方法、类、包或J2EE组件等不同层上。

- **访问树 Call Tree**：显示一个积累的自顶向下的树，树中包含所有在JVM中已记录的访问队列。JDBC，JMS和JNDI服务请求都被注释在请求树中。请求树可以根据Servlet和JSP对URL的不同需要进行拆分。
- **热点 Hot Spots**：显示消耗时间最多的方法的列表。对每个热点都能够显示回溯树。该热点可以按照方法请求，JDBC，JMS和JNDI服务请求以及按照URL请求来进行计算。
- **访问图 Call Graph**：显示一个从已选方法、类、包或J2EE组件开始的访问队列的图。
- **方法统计 Method Statistis**：显示一段时间内记录的方法的调用时间细节。

![](https://gitee.com/vectorx/ImageCloud/raw/master/img/20210505170057.png#clientId=u47c4d581-aed1-4&id=IPj7k&originHeight=531&originWidth=1914&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ubeabcf3d-4519-4a32-a8a8-8909480ebcb&title=)<br />![](https://gitee.com/vectorx/ImageCloud/raw/master/img/20210505170143.png#clientId=u47c4d581-aed1-4&id=chvgJ&originHeight=858&originWidth=1914&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7c167b4a-aad1-4042-abca-b838a297269&title=)
<a name="Z8LQb"></a>
### 线程视图 threads
JProfiler通过对线程历史的监控判断其运行状态，并监控是否有线程阻塞产生，还能将一个线程所管理的方法以树状形式呈现。对线程剖析。

- **线程历史 Thread History**：显示一个与线程活动和线程状态在一起的活动时间表。
- **线程监控 Thread Monitor**：显示一个列表，包括所有的活动线程以及它们目前的活动状况。
- **线程转储 Thread Dumps**：显示所有线程的堆栈跟踪。

线程分析主要关心三个方面：

- 1．web容器的线程最大数。比如：Tomcat的线程容量应该略大于最大并发数。
- 2．线程阻塞
- 3．线程死锁

![](https://gitee.com/vectorx/ImageCloud/raw/master/img/20210505170742.png#clientId=u47c4d581-aed1-4&id=mZG0m&originHeight=845&originWidth=1893&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua3b9b3c6-d95e-4724-a947-cefa6a1a2ae&title=)
<a name="gmLzE"></a>
### 监控和锁 Monitors ＆Locks
所有线程持有锁的情况以及锁的信息。观察JVM的内部线程并查看状态：

- **死锁探测图表 Current Locking Graph**：显示JVM中的当前死锁图表。
- **目前使用的监测器 Current Monitors**：显示目前使用的监测器并且包括它们的关联线程。
- **锁定历史图表 Locking History Graph**：显示记录在JVM中的锁定历史。
- **历史检测记录 Monitor History**：显示重大的等待事件和阻塞事件的历史记录。
- **监控器使用统计 Monitor Usage Statistics**：显示分组监测，线程和监测类的统计监测数据
<a name="0a9509e1"></a>
## 6、Arthas
上述工具都必须在服务端项目进程中配置相关的监控参数，然后工具通过远程连接到项目进程，获取相关的数据。这样就会带来一些不便，比如线上环境的网络是隔离的，本地的监控工具根本连不上线上环境。并且类似于Jprofiler这样的商业工具，是需要付费的。<br />那么有没有一款工具不需要远程连接，也不需要配置监控参数，同时也提供了丰富的性能监控数据呢？<br />阿里巴巴开源的性能分析神器Arthas应运而生。<br />Arthas是Alibaba开源的Java诊断工具，深受开发者喜爱。在线排查问题，无需重启；动态跟踪Java代码；实时监控JVM状态。Arthas 支持JDK 6＋，支持Linux／Mac／Windows，采用命令行交互模式，同时提供丰富的 Tab 自动补全功能，进一步方便进行问题的定位和诊断。当你遇到以下类似问题而束手无策时，Arthas可以帮助你解决：

- 这个类从哪个 jar 包加载的？为什么会报各种类相关的 Exception？
- 改的代码为什么没有执行到？难道是没 commit？分支搞错了？
- 遇到问题无法在线上 debug，难道只能通过加日志再重新发布吗？
- 线上遇到某个用户的数据处理有问题，但线上同样无法 debug，线下无法重现！
- 是否有一个全局视角来查看系统的运行状况？
- 有什么办法可以监控到JVM的实时运行状态？
- 怎么快速定位应用的热点，生成火焰图？

官方地址：[https://arthas.aliyun.com/doc/quick-start.html](https://arthas.aliyun.com/doc/quick-start.html)<br />安装方式：如果速度较慢，可以尝试国内的码云Gitee下载。
```shell
wget https://io/arthas/arthas-boot.jar
wget https://arthas/gitee/io/arthas-boot.jar
```
Arthas只是一个java程序，所以可以直接用java -jar运行。<br />除了在命令行查看外，Arthas目前还支持 Web Console。在成功启动连接进程之后就已经自动启动，可以直接访问 http://127.0.0.1:8563/ 访问，页面上的操作模式和控制台完全一样。
<a name="wdQF5"></a>
### 基础指令
```shell
quit/exit 退出当前 Arthas客户端，其他 Arthas喜户端不受影响
stop/shutdown 关闭 Arthas服务端，所有 Arthas客户端全部退出
help 查看命令帮助信息
cat 打印文件内容，和linux里的cat命令类似
echo 打印参数，和linux里的echo命令类似
grep 匹配查找，和linux里的gep命令类似
tee 复制标隹输入到标准输出和指定的文件，和linux里的tee命令类似
pwd 返回当前的工作目录，和linux命令类似
cls 清空当前屏幕区域
session 查看当前会话的信息
reset 重置增强类，将被 Arthas增强过的类全部还原, Arthas服务端关闭时会重置所有增强过的类
version 输出当前目标Java进程所加载的 Arthas版本号
history 打印命令历史
keymap Arthas快捷键列表及自定义快捷键
```
<a name="JRZO6"></a>
### jvm相关
```shell
dashboard 当前系统的实时数据面板
thread 查看当前JVM的线程堆栈信息
jvm 查看当前JVM的信息
sysprop 查看和修改JVM的系统属性
sysem 查看JVM的环境变量
vmoption 查看和修改JVM里诊断相关的option
perfcounter 查看当前JVM的 Perf Counter信息
logger 查看和修改logger
getstatic 查看类的静态属性
ognl 执行ognl表达式
mbean 查看 Mbean的信息
heapdump dump java heap，类似jmap命令的 heap dump功能
```
<a name="EYaqT"></a>
### class/classloader相关
```shell
sc 查看JVM已加载的类信息
	-d 输出当前类的详细信息，包括这个类所加载的原始文件来源、类的声明、加载的Classloader等详细信息。如果一个类被多个Classloader所加载，则会出现多次
	-E 开启正则表达式匹配，默认为通配符匹配
	-f 输出当前类的成员变量信息（需要配合参数-d一起使用）
	-X 指定输出静态变量时属性的遍历深度，默认为0，即直接使用toString输出
sm 查看已加载类的方法信息
	-d 展示每个方法的详细信息
	-E 开启正则表达式匹配,默认为通配符匹配
jad 反编译指定已加载类的源码
mc 内存编译器，内存编译.java文件为.class文件
retransform 加载外部的.class文件, retransform到JVM里
redefine 加载外部的.class文件，redefine到JVM里
dump dump已加载类的byte code到特定目录
classloader 查看classloader的继承树，urts，类加载信息，使用classloader去getResource
	-t 查看classloader的继承树
	-l 按类加载实例查看统计信息
	-c 用classloader对应的hashcode来查看对应的 Jar urls
```
<a name="o6TWQ"></a>
### monitor/watch/trace相关
```
monitor 方法执行监控，调用次数、执行时间、失败率
	-c 统计周期，默认值为120秒
watch 方法执行观测，能观察到的范围为：返回值、抛出异常、入参，通过编写groovy表达式进行对应变量的查看
	-b 在方法调用之前观察(默认关闭)
	-e 在方法异常之后观察(默认关闭)
	-s 在方法返回之后观察(默认关闭)
	-f 在方法结束之后(正常返回和异常返回)观察(默认开启)
	-x 指定输岀结果的属性遍历深度,默认为0
trace 方法内部调用路径,并输出方法路径上的每个节点上耗时
	-n 执行次数限制
stack 输出当前方法被调用的调用路径
tt 方法执行数据的时空隧道,记录下指定方法每次调用的入参和返回信息,并能对这些不同的时间下调用进行观测
```
<a name="NLgf8"></a>
### 其他
```shell
jobs 列出所有job
kill 强制终止任务
fg 将暂停的任务拉到前台执行
bg 将暂停的任务放到后台执行
grep 搜索满足条件的结果
plaintext 将命令的结果去除ANSI颜色
wc 按行统计输出结果
options 查看或设置Arthas全局开关
profiler 使用async-profiler对应用采样，生成火焰图
```
<a name="b7b8f652"></a>
## 7、Java Misssion Control
在Oracle收购Sun之前，Oracle的JRockit虚拟机提供了一款叫做 JRockit Mission Control 的虚拟机诊断工具。<br />在Oracle收购sun之后，Oracle公司同时拥有了Hotspot和 JRockit 两款虚拟机。根据Oracle对于Java的战略，在今后的发展中，会将JRokit的优秀特性移植到Hotspot上。其中一个重要的改进就是在Sun的JDK中加入了JRockit的支持。<br />在Oracle JDK 7u40之后，Mission Control这款工具己经绑定在Oracle JDK中发布。<br />自Java11开始，本节介绍的JFR己经开源。但在之前的Java版本，JFR属于Commercial Feature通过Java虚拟机参数`-XX:+UnlockCommercialFeatures` 开启。<br />Java Mission Control（简称JMC) ， Java官方提供的性能强劲的工具，是一个用于对 Java应用程序进行管理、监视、概要分析和故障排除的工具套件。它包含一个GUI客户端以及众多用来收集Java虚拟机性能数据的插件如 JMX Console（能够访问用来存放虚拟机齐个于系统运行数据的MXBeans）以及虚拟机内置的高效 profiling 工具 Java Flight Recorder（JFR）。<br />JMC的另一个优点就是：采用取样，而不是传统的代码植入技术，对应用性能的影响非常非常小，完全可以开着JMC来做压测（唯一影响可能是 full gc 多了）。<br />官方地址：[https://github.com/JDKMissionControl/jmc](https://github.com/JDKMissionControl/jmc)<br />![](https://gitee.com/vectorx/ImageCloud/raw/master/img/20210505193559.png#clientId=u47c4d581-aed1-4&id=fDL9O&originHeight=997&originWidth=1920&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue689a4e5-560f-4586-9df5-f8aba1686b6&title=)
<a name="ue6HX"></a>
### Java Flight Recorder
Java Flight Recorder是JMC的其中一个组件，能够以极低的性能开销收集Java虚拟机的性能数据。与其他工具相比，JFR的性能开销很小，在默认配置下平均低于1%。JFR能够直接访问虚拟机内的敌据并且不会影响虚拟机的优化。因此它非常适用于生产环境下满负荷运行的Java程序。<br />Java Flight Recorder 和 JDK Mission Control共同创建了一个完整的工具链。JDK Mission Control 可对 Java Flight Recorder 连续收集低水平和详细的运行时信息进行高效、详细的分析。<br />当启用时 JFR将记录运行过程中发生的一系列事件。其中包括Java层面的事件如线程事件、锁事件，以及Java虚拟机内部的事件，如新建对象，垃圾回收和即时编译事件。按照发生时机以及持续时间来划分，JFR的事件共有四种类型，它们分别为以下四种：

- 瞬时事件（Instant Event) ，用户关心的是它们发生与否，例如异常、线程启动事件。 
- 持续事件(Duration Event) ，用户关心的是它们的持续时间，例如垃圾回收事件。 
- 计时事件(Timed Event) ，是时长超出指定阈值的持续事件。 
- 取样事件（Sample Event)，是周期性取样的事件。 

取样事件的其中一个常见例子便是方法抽样（Method Sampling），即每隔一段时问统计各个线程的栈轨迹。如果在这些抽样取得的栈轨迹中存在一个反复出现的方法，那么可以推测该方法是热点方法<br />![](https://gitee.com/vectorx/ImageCloud/raw/master/img/20210505185942.png#clientId=u47c4d581-aed1-4&id=i7Cpj&originHeight=896&originWidth=1491&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4642ba3a-0e85-4da3-a6d3-b57a97c987c&title=)<br />![](https://gitee.com/vectorx/ImageCloud/raw/master/img/20210505185955.png#clientId=u47c4d581-aed1-4&id=GMqbH&originHeight=898&originWidth=1491&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u89e46256-530a-4d59-ad8d-16a23c8b0ee&title=)<br />![](https://gitee.com/vectorx/ImageCloud/raw/master/img/20210505190010.png#clientId=u47c4d581-aed1-4&id=HQyah&originHeight=896&originWidth=1491&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ucd6d0e41-a1da-4d2c-8e23-8f6924471d4&title=)<br />![](https://gitee.com/vectorx/ImageCloud/raw/master/img/20210505190024.png#clientId=u47c4d581-aed1-4&id=vaSpp&originHeight=896&originWidth=1488&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf54cd5f0-d632-44ba-9a1c-da839938649&title=)<br />![](https://gitee.com/vectorx/ImageCloud/raw/master/img/20210505190038.png#clientId=u47c4d581-aed1-4&id=s6Zkf&originHeight=897&originWidth=1486&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1d780049-465a-45a4-b095-125155d5eb7&title=)<br />![](https://gitee.com/vectorx/ImageCloud/raw/master/img/20210505190053.png#clientId=u47c4d581-aed1-4&id=tNfBU&originHeight=896&originWidth=1491&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3887d83a-238f-4405-9e72-b94900f065e&title=)<br />![](https://gitee.com/vectorx/ImageCloud/raw/master/img/20210505193710.png#clientId=u47c4d581-aed1-4&id=UlGNe&originHeight=896&originWidth=1491&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u13065e79-430c-410e-a181-8eee5c023db&title=)
<a name="b32eb9f8"></a>
## 8、其他工具
<a name="CqDGT"></a>
### Flame Graphs（火焰图）
在追求极致性能的场景下，了解你的程序运行过程中cpu在干什么很重要，火焰图就是一种非常直观的展示CPU在程序整个生命周期过程中时间分配的工具。火焰图对于现代的程序员不应该陌生，这个工具可以非常直观的显示出调用找中的CPU消耗瓶颈。<br />网上的关于Java火焰图的讲解大部分来自于Brenden Gregg的博客 [http://new.brendangregg.com/flamegraphs.html ](http://new.brendangregg.com/flamegraphs.html)<br />![](https://gitee.com/vectorx/ImageCloud/raw/master/img/20210505190824.png#clientId=u47c4d581-aed1-4&id=AE5N9&originHeight=430&originWidth=715&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8b076a5d-6fd6-4257-982f-e605634d9f6&title=)<br />火焰图，简单通过x轴横条宽度来度量时间指标，y轴代表线程栈的层次。
<a name="aHfhS"></a>
### Tprofiler
案例： 使用JDK自身提供的工具进行JVM调优可以将下 TPS 由2.5提升到20（提升了7倍），并准确定位系统瓶颈。<br />系统瓶颈有：应用里释态对象不是太多、有大量的业务线程在频繁创建一些生命周期很长的临时对象，代码里有问题。<br />那么，如何在海量业务代码里边准确定位这些性能代码？这里使用阿里开源工具 Tprofiler 来定位这些性能代码，成功解决掉了GC 过于频繁的性能瓶预，并最终在上次优化的基础上将 TPS 再提升了4倍，即提升到100。

- Tprofiler配置部署、远程操作、日志阅谈都不太复杂，操作还是很简单的。但是其却是能够起到一针见血、立竿见影的效果，解决了GC过于频繁的性能瓶预。
- Tprofiler最重要的特性就是能够统汁出你指定时间段内 JVM 的 top method 这些 top method 极有可能就是造成你 JVM 性能瓶颈的元凶。这是其他大多数 JVM 调优工具所不具备的，包括 JRockit Mission Control。JRokit 首席开发者 Marcus Hirt 在其私人博客《 Lom Overhead Method Profiling cith Java Mission Control》下的评论中曾明确指出  JRMC 井不支持 TOP 方法的统计。

官方地址：[http://github.com/alibaba/Tprofiler](http://github.com/alibaba/Tprofiler)
<a name="gGYct"></a>
### Btrace
常见的动态追踪工具有BTrace、HouseHD（该项目己经停止开发）、Greys-Anatomy（国人开发 个人开发者）、Byteman（JBoss出品），注意Java运行时追踪工具井不限干这几种，但是这几个是相对比较常用的。<br />BTrace是SUN Kenai 云计算开发平台下的一个开源项目，旨在为java提供安全可靠的动态跟踪分析工具。先看一下Trace的官方定义：
> BTrace is a safe, dynamic tracing tool for the Java platform. BTrace can be used to dynamically trace a running Java program (similar to DTrace for OpenSolaris applications and OS). BTrace dynamically instruments the classes of the target application to inject tracing code ("bytecode tracing").

大概意思是一个 Java 平台的安全的动态追踪工具，可以用来动态地追踪一个运行的 Java 程序。BTrace动态调整目标应用程序的类以注入跟踪代码（“字节码跟踪“）。
<a name="vkzkx"></a>
### YourKit
<a name="Euc0l"></a>
### JProbe
<a name="iReFA"></a>
### Spring Insight
