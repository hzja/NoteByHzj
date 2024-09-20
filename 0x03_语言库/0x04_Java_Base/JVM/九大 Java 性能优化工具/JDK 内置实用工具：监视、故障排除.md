JDK
<a name="NjJhz"></a>
## 监视工具
<a name="G5LWn"></a>
### JConsole
JConsole 图形用户界面是一种符合 Java 管理扩展 （JMX） 规范的监视工具。JConsole 使用 Java 虚拟机 （Java VM） 的广泛检测来提供有关在 Java 平台上运行的应用程序的性能和资源消耗的信息。
<a name="NqB8o"></a>
#### 使用方法
<a name="QyQYT"></a>
##### 本地

- 使用`jconsole`命令：监视本地运行的所有 Java 应用程序，JConsole 可以连接到这些应用程序。
- 使用`jconsole PID`命令：监视指定PID的Java应用程序。

获取java PID的方法：通过任务管理器查看、通过Java提供的jps命令查看。
<a name="LOXvB"></a>
##### 远程
使用`jsconsole hostName:portNum`命令：hostName是运行应用程序的系统的名称，portNum是在启动 Java VM 时启用 JMX 代理时指定的端口号。<br />使用`service:jmx:<protocol>:<sap>`命令：使用 JMX 服务 URL 进行连接。
<a name="IWDn2"></a>
#### 内容分析
将 JConsole 连接到应用程序后，JConsole 由六个选项卡组成。

- 概述：显示有关 Java VM 和受监视值的概述信息。
- 内存：显示有关内存使用的信息。
- 线程：显示有关线程使用的信息。
- 类：显示有关类加载的信息。
- VM：显示有关 Java VM 的信息。
- MBeans：显示有关 MBeans 的信息。
<a name="UGV18"></a>
#### 组成部分
<a name="vduk2"></a>
##### 概览
显示有关 CPU 使用情况、内存使用情况、线程计数和在 Java VM 中加载的类的图形监视信息。<br />![提供有关内存消耗和内存池的信息](https://cdn.nlark.com/yuque/0/2022/png/396745/1652230264447-227473c3-88a9-4685-a364-d33404e79aee.png#averageHue=%23f7f7f7&clientId=u732c5ca5-2075-4&from=paste&height=605&id=uda9c5f44&originHeight=1512&originWidth=2156&originalType=binary&ratio=1&rotation=0&showTitle=true&size=136027&status=done&style=shadow&taskId=u6020639f-ffba-4a63-864a-8860d2627b6&title=%E6%8F%90%E4%BE%9B%E6%9C%89%E5%85%B3%E5%86%85%E5%AD%98%E6%B6%88%E8%80%97%E5%92%8C%E5%86%85%E5%AD%98%E6%B1%A0%E7%9A%84%E4%BF%A1%E6%81%AF&width=862.4 "提供有关内存消耗和内存池的信息")<br />提供执行GC的操作，可以随时点击按钮进行垃圾回收

- **伊甸园空间（堆）：** 最初为大多数对象分配内存的池。
- **幸存者空间（堆）：** 包含在伊甸园空间垃圾回收中幸存下来的物体的池。
- **终身代（堆）：** 包含在幸存者空间中存在一段时间的对象的池。
- **永久生成（非堆）：** 包含虚拟机本身的所有反射数据的池，如类和方法对象。使用类数据共享的 Java VM，这一代分为只读和读写区域。
- **代码缓存（非堆）：** HotSpotJava VM 还包括一个代码缓存，其中包含用于编译和存储本机代码的内存。
<a name="NBn90"></a>
##### 堆和非堆内存
Java VM 管理两种类型的内存：堆内存和非堆内存，这两种内存都是在 Java VM 启动时创建的。

- 堆内存是 Java VM 为所有类实例和数组分配内存的运行时数据区域。堆的大小可能是固定的或可变的。垃圾回收器是一个自动内存管理系统，用于回收对象的堆内存。
- 非堆内存包括所有线程之间共享的方法区域和 Java VM 的内部处理或优化所需的内存。它存储每类结构，如运行时常量池、字段和方法数据，以及方法和构造函数的代码。方法区域在逻辑上是堆的一部分，但是，根据实现，Java VM 可能不会对它进行垃圾回收或压缩。与堆内存一样，方法区域可能为固定大小或可变大小。方法区域的内存不需要连续。
<a name="DjzJX"></a>
##### 内存池和内存管理器
内存池和内存管理器是 Java VM 内存系统的关键方面。

- 内存池表示 Java VM 管理的内存区域。Java VM 至少有一个内存池，它可能会在执行期间创建或删除内存池。内存池可以属于堆内存或非堆内存。
- 内存管理器管理一个或多个内存池。垃圾回收器是一种内存管理器，负责回收不可到达的对象使用的内存。Java VM 可能具有一个或多个内存管理器。它可以在执行期间添加或删除内存管理器。内存池可以由多个内存管理器管理。
<a name="gLqWY"></a>
##### 垃圾回收
垃圾回收 （GC） 是 Java VM 释放不再引用的对象占用的内存的方式。通常认为具有活动引用为"活动"且未引用（或无法访问）对象的对象为"已死"。垃圾回收是释放死对象使用的内存的过程。GC 使用的算法和参数对性能有显著影响。<br />Java hotspot VM 垃圾回收器使用代数 GC。生成 GC 利用大多数程序符合以下概括的观察。

- 它们创建许多寿命较短的对象，例如迭代器和局部变量。
- 它们创建一些寿命很长的对象，例如高级持久对象。

![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1652230293679-a4c1024b-77c3-44b3-83a7-b19e34dbd272.png#averageHue=%23f7f7f7&clientId=u732c5ca5-2075-4&from=paste&height=604&id=u8fe16d5a&originHeight=1511&originWidth=2156&originalType=binary&ratio=1&rotation=0&showTitle=false&size=99615&status=done&style=shadow&taskId=u40983f63-a2a7-452b-af84-f66d2a746fc&title=&width=862.4)
<a name="FR4xa"></a>
##### 线程
提供有关线程使用的信息。

- **查找监视器死锁线程：** 检测对象监视器锁上是否有任何线程死锁。此操作返回死锁线程指示的数组。
- **getThreadInfo：** 返回线程信息。这包括线程当前被阻止的名称、堆栈跟踪和监视器锁（如果有）以及持有该锁的线程以及线程争用统计信息。
- **获取ThreadCpu时间：** 返回给定线程消耗的 CPU 时间

![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1652230328699-f1cec77d-1411-4599-b7ce-e270eb89fc22.png#averageHue=%23f8f8f8&clientId=u732c5ca5-2075-4&from=paste&height=603&id=u535ea509&originHeight=1507&originWidth=2156&originalType=binary&ratio=1&rotation=0&showTitle=false&size=126170&status=done&style=shadow&taskId=u2320fa9a-c8c9-4df1-a865-8ea56b3e270&title=&width=862.4)
<a name="kLmAg"></a>
##### 类
显示有关类加载的信息。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1652230343231-98a9be7b-b654-4d99-ba45-a444bf84cdac.png#averageHue=%23f8f8f8&clientId=u732c5ca5-2075-4&from=paste&height=604&id=u363cbde2&originHeight=1511&originWidth=2156&originalType=binary&ratio=1&rotation=0&showTitle=false&size=81408&status=done&style=shadow&taskId=uba6a1e23-6829-4ce8-a6ec-eae1b61ca9d&title=&width=862.4)
<a name="oAvlk"></a>
##### VM 概要
提供有关 Java VM 的信息。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1652230482131-47838f15-2c1a-4827-b9ba-60113f40e0b5.png#averageHue=%23f7f5f4&clientId=u732c5ca5-2075-4&from=paste&height=603&id=u54b2af64&originHeight=1508&originWidth=2156&originalType=binary&ratio=1&rotation=0&showTitle=false&size=182254&status=done&style=shadow&taskId=uc55fbb43-d5c3-4616-b8b7-9a15259a937&title=&width=862.4)
<a name="qu3Pu"></a>
##### MBean
以通用方式显示有关在平台 MBean 服务器注册的所有 MBeans 的信息。MBeans 选项卡允许您访问平台 MXBean 检测的完整集，包括在其他选项卡中不可见的仪器。此外，您还可以使用 MBeans 选项卡监视和管理应用程序的 MBeans。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1652230548028-d80c3cc0-164c-453a-83ba-9a6b8480b41e.png#averageHue=%23f2f2f1&clientId=u732c5ca5-2075-4&from=paste&height=603&id=udbd4d572&originHeight=1507&originWidth=2156&originalType=binary&ratio=1&rotation=0&showTitle=false&size=117028&status=done&style=shadow&taskId=u308eb5b2-0f80-42a4-b2c6-7a89f7c5555&title=&width=862.4)
<a name="zmx41"></a>
### jps
列出目标系统上已检测的 Java 虚拟机 （JVM）。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652229807812-280a6a39-6590-47bd-bc32-f9c0eb06ae0f.png#averageHue=%23161616&clientId=u732c5ca5-2075-4&from=paste&id=uc88cb645&originHeight=123&originWidth=240&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1b544571-ba1c-4aaf-ac32-295a75db0f7&title=)
<a name="ciK1P"></a>
### jstat
监视 Java 虚拟机 （JVM） 统计信息。<br />对Java应用程序的资源和性能进行实时的命令行的监控，包括了对Heap size和垃圾回收状况的监控。
<a name="SWgXA"></a>
#### 命令格式
```bash
jstat [-option] [PID]
```
<a name="H26uB"></a>
#### option参数

- **class：**显示有关类加载器行为的统计信息。
- **compiler：**显示有关 Java HotSpot VM 实时编译器行为的统计信息。
- **gc：**显示有关垃圾回收堆行为的统计信息。
- **gccapacity：**显示有关几代人容量及其相应空间的统计信息。
- **gccause：**显示有关垃圾回收统计信息（与 相同）的摘要，以及最后和当前（如果适用）垃圾回收事件的原因。-gcutil
- **gcnew：**显示新一代行为的统计信息。
- **gcnewcapacity：**显示有关新一代大小及其相应空间的统计信息。
- **gcold：**显示有关旧一代和元空间统计信息行为的统计信息。
- **gcoldcapacity：**显示有关旧一代大小的统计信息。
- **gcmetacapacity：**显示有关元空间大小的统计信息。
- **gcutil：**显示有关垃圾回收统计信息的摘要。
- **printcompilation：**显示 Java 热点 VM 编译方法统计信息。
<a name="SRipR"></a>
#### `jstat –class <pid>`：显示加载class的数量，及所占空间等信息。
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652229807864-57d99aa0-f272-4712-8828-0455617c9c96.png#averageHue=%232d2e31&clientId=u732c5ca5-2075-4&from=paste&id=u53e5cc97&originHeight=232&originWidth=551&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u55d282b7-a0ae-48a6-b7b7-366231ab19f&title=)
<a name="w0icW"></a>
#### `jstat -compiler <pid>`：显示VM实时编译的数量等信息。
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652229808055-f249ab38-6299-4510-8334-d6ea1e4b6d4a.png#averageHue=%232d2e31&clientId=u732c5ca5-2075-4&from=paste&id=u6395edc7&originHeight=274&originWidth=599&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9bd970da-8b65-469e-9adb-5472483d234&title=)
<a name="E60Qz"></a>
#### `jstat -gc <pid>`：可以显示gc的信息，查看gc的次数，及时间。
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652229808006-9c825f98-8506-487d-b4fe-accbc6beb66b.png#averageHue=%232a2b2f&clientId=u732c5ca5-2075-4&from=paste&id=u44a4f346&originHeight=691&originWidth=707&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u87a57704-33af-458f-b9ac-86dfd709859&title=)
<a name="IgFM3"></a>
#### `jstat -gccapacity <pid>`：可以显示，VM内存中三代（young,old,perm）对象的使用和占用大小
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652229808280-adb69dc1-bf01-4379-8b89-9e81ba4a415b.png#averageHue=%232a2b2f&clientId=u732c5ca5-2075-4&from=paste&id=u10a65769&originHeight=735&originWidth=708&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua7421ccc-24b0-4c66-a619-58fa7f79bb3&title=)
<a name="RFNuT"></a>
#### `jstat -gcutil <pid>`：统计gc信息
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652229808261-5669dfac-6a6c-4cba-b5fd-f9be7368934a.png#averageHue=%232a2b2f&clientId=u732c5ca5-2075-4&from=paste&id=u1c494015&originHeight=473&originWidth=654&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua92977ee-a0f1-4e90-980c-e20166f25b3&title=)
<a name="quyIV"></a>
#### `jstat -gcnew <pid>`：年轻代对象的信息。
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652229808213-881a152e-06e5-4ec7-ae9b-1043b2a2e0fb.png#averageHue=%232a2b2f&clientId=u732c5ca5-2075-4&from=paste&id=uf9f001bb&originHeight=477&originWidth=614&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf7d25c43-4b6b-4c9a-b9cb-abab6053703&title=)
<a name="UCsuJ"></a>
#### `jstat -gcnewcapacity<pid>`：年轻代对象的信息及其占用量。
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652229808331-6a0483fe-c034-459e-8180-a4efe0d9e1af.png#averageHue=%232a2c30&clientId=u732c5ca5-2075-4&from=paste&id=u1b5dbba2&originHeight=517&originWidth=626&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0fa1c8db-ce24-453f-8ed7-ef48821bcc2&title=)
<a name="DcC3R"></a>
#### `jstat -gcold <pid>`：old代对象的信息。
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652229808385-c4d22def-70b1-42c5-8a86-e0944e1e423c.png#averageHue=%23292b2f&clientId=u732c5ca5-2075-4&from=paste&id=u1da2d3fc&originHeight=393&originWidth=600&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u34dc1d5f-178f-4e2c-99c9-8ce39a41e36&title=)
<a name="QU2Q9"></a>
#### `jstat -gcoldcapacity <pid>`：old代对象的信息及其占用量。
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652229808527-2e917926-3ba3-4317-8814-3417a8846e4c.png#averageHue=%23292b2f&clientId=u732c5ca5-2075-4&from=paste&id=uaf85bef9&originHeight=388&originWidth=633&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub0148216-7695-48da-94b8-437acb196ef&title=)
<a name="FS0d9"></a>
#### `jstat -gcpermcapacity<pid>`：perm对象的信息及其占用量。
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652229808592-cfcb955b-d643-4514-877e-5da9235c8b37.png#averageHue=%232a2b2f&clientId=u732c5ca5-2075-4&from=paste&id=uc51b4758&originHeight=388&originWidth=619&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u418d047d-db41-48a2-8cbd-407a70be2e2&title=)
<a name="jZRgT"></a>
#### `jstat -printcompilation <pid>`：当前VM执行的信息。
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652229808619-f2d13cd4-36a9-48f9-8ccb-0d0d250fbb76.png#averageHue=%2328292e&clientId=u732c5ca5-2075-4&from=paste&id=u974d54d9&originHeight=239&originWidth=936&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u952395f6-851d-4603-bfcf-a1578c6a784&title=)
<a name="JkDfa"></a>
### jstatd
监视 Java 虚拟机 （JVM），并使远程监视工具能够连接到 JVM
<a name="inG8z"></a>
#### 命令格式
```bash
jstatd -[option]
```
<a name="grNJS"></a>
#### option

- -nr 当找不到现有的RMI注册表时，不尝试使用jstatd进程创建一个内部的RMI注册表。
- -p port 在指定的端口查找RMI注册表。如果没有找到，并且没有指定-nr选项，则在该端口自行创建一个内部的RMI注册表。
- -n rminame RMI注册表中绑定的RMI远程对象的名称。默认的名称为JStatRemoteHost。如果多个jstatd服务器在同一主机上运行，你可以通过指定该选项来让每个服务器导出的RMI对象具有唯一的名称。不管如何，这样做需要将唯一的服务器名称包含进监控客户端的hostid和vmid字符串中。
- -Joption 将选项参数传递给被javac调用的java启动程序。例如，-J-Xms48m设置启动内存为48 MB。使用-J将选项参数传递给执行Java应用程序的底层虚拟机，这是一种常见惯例。
<a name="r62hD"></a>
#### 使用方法

1. 在jdk的bin目录下创建文件jstatd.all.policy
2. 写入下面的安全配置
```
grant codebase "file:/usr/lib/jvm/java-1.8.0-openjdk-1.8.0.191.b12-1.el7_6.x86_64/lib/tools.jar" {
permission java.security.AllPermission;
};
#此处写绝对路径，主要是防止路径错误问题，排查问题，应该写成相对路径
```

3. 启动jstatd
```bash
./jstatd -J-Djava.security.policy=jstatd.all.policy -J-Djava.rmi.server.hostname=x.x.x.x &
```

4. 使用jvisualvm工具远程连接，进行监控
<a name="gAUhM"></a>
### jvisualvm
VisualVM，能够监控线程，内存情况，查看方法的CPU时间和内存中的对 象，已被GC的对象，反向查看分配的堆栈(如100个String对象分别由哪几个对象分配出来的).<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1652230946842-8bab37a5-3c92-4f0e-a49c-48d9a83dd750.png#averageHue=%23f7f7f6&clientId=u732c5ca5-2075-4&from=paste&height=604&id=u63d7c2ff&originHeight=1510&originWidth=2610&originalType=binary&ratio=1&rotation=0&showTitle=false&size=221416&status=done&style=shadow&taskId=ud53994cf-1e83-4073-8d04-866e4c7a283&title=&width=1044)<br />同时他还提供很多插件可以自己安装，是一款不错的监控分析工具。
<a name="YFy48"></a>
## 故障排除工具
<a name="f6f5j"></a>
### JInfo
可以用来查看正在运行的 java 应用程序的扩展参数，包括Java System属性和JVM命令行参数；也可以动态的修改正在运行的 JVM 一些参数。当系统崩溃时，jinfo可以从core文件里面知道崩溃的Java应用程序的配置信息
<a name="iFTd4"></a>
#### 命令格式
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652229808738-8d521edd-b647-428d-b2d7-c59d15819b2b.png#averageHue=%23171717&clientId=u732c5ca5-2075-4&from=paste&id=u4e7d132f&originHeight=285&originWidth=577&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u51dee7af-76a7-4862-8705-dd9986f6092&title=)
<a name="LTnEo"></a>
#### 参数说明

- pid 对应jvm的进程id
- executable core 产生core dump文件
- [server-id@]remote server IP or hostname 远程的ip或者hostname，server-id标记服务的唯一性id。
<a name="Jy6n7"></a>
#### option

- no option 输出全部的参数和系统属性
- -flag name 输出对应名称的参数
- -flag [+|-]name 开启或者关闭对应名称的参数
- -flag name=value 设定对应名称的参数
- -flags 输出全部的参数
- -sysprops 输出系统属性
<a name="v6IGF"></a>
### Javacore
<a name="jkDZ4"></a>
#### 概述
Javacore，也可以称为“threaddump”或是“javadump”，它是 Java 提供的一种诊断特性，能够提供一份可读的当前运行的 JVM 中线程使用情况的快照。即在某个特定时刻，JVM 中有哪些线程在运行，每个线程执行到哪一个类，哪一个方法。应用程序如果出现不可恢复的错误或是内存泄露，就会自动触发 Javacore 的生成。
<a name="WCik0"></a>
#### 使用方法
<a name="f7LuA"></a>
##### `jinfo pid`：输出当前 jvm 进程的全部参数和系统属性
<a name="AaiOh"></a>
##### `jinfo -flag name pid`：输出对应名称的参数使用该命令，可以查看指定的 jvm 参数的值。如：查看当前 jvm 进程是否开启打印 GC 日志。
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652229808886-542257e3-8dca-4110-9496-bba2b37c28fc.png#averageHue=%23181818&clientId=u732c5ca5-2075-4&from=paste&id=u779ff948&originHeight=149&originWidth=464&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub10ac85b-3cb6-4bcf-ba60-e6659964395&title=)
<a name="SYpnA"></a>
##### `jinfo -flag [+|-]name pid`：开启或者关闭对应名称的参数
使用 jinfo 可以在不重启虚拟机的情况下，可以动态的修改 jvm 的参数。尤其在线上的环境特别有用。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652229809083-aeab5bb9-1520-49f1-ad9c-b325499ccac9.png#averageHue=%23191919&clientId=u732c5ca5-2075-4&from=paste&id=u2db54729&originHeight=115&originWidth=402&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4b440abf-59b7-4028-a760-097d77f1a76&title=)
<a name="IXhZq"></a>
##### `jinfo -flag name=value pid`：修改指定参数的值。
注意：jinfo虽然可以在java程序运行时动态地修改虚拟机参数，但并不是所有的参数都支持动态修改
<a name="i6FJt"></a>
##### `jinfo -flags pid`：输出全部的参数
<a name="vQ9Uq"></a>
##### `jinfo -sysprops pid`：输出当前 jvm 进行的全部的系统属性
<a name="okZce"></a>
### jhat
主要是用来分析java堆的命令，可以将堆中的对象以html的形式显示出来，包括对象的数量，大小等等，并支持对象查询语言。
<a name="q9mBC"></a>
#### 1、使用jmap命令导出堆文件
```bash
jmap -dump:live,file=a.log pid
```
也可以使用下面方式导出堆文件<br />1、使用 jconsole 选项通过 HotSpotDiagnosticMXBean 从运行时获得堆转储（生成dump文件）、<br />2、虚拟机启动时如果指定了 `-XX:+HeapDumpOnOutOfMemoryError` 选项，则在抛出 OutOfMemoryError 时，会自动执行堆转储。<br />3、使用 hprof 命令
<a name="JcDHf"></a>
#### 2、使用jhat分析堆文件
```bash
jhat -J-Xmx512M a1.log
```
<a name="gojFk"></a>
#### 3、查看分析的html页面
http://ip:7000/jhat
<a name="dl9zl"></a>
#### OQL（对象查询语言）
如果需要根据某些条件来过滤或查询堆的对象，这是可能的，可以在jhat的html页面中执行OQL，来查询符合条件的对象
<a name="BQKGU"></a>
##### 基本语法：
```bash
select <javascript expression to select>
[from [instanceof] <class name> <identifier>]
[where <javascript boolean expression to filter>]
```
<a name="SCvfQ"></a>
##### 解释：
(1)class name是java类的完全限定名，如：`java.lang.String`, `java.util.ArrayList`, C是char数组, java.io.File是java.io.File[]<br />(2)类的完全限定名不足以唯一的辨识一个类，因为不同的ClassLoader载入的相同的类，它们在jvm中是不同类型的<br />(3)instanceof表示也查询某一个类的子类，如果不明确instanceof，则只精确查询class name指定的类。<br />(4)from和where子句都是可选的<br />(5)java域表示：obj.field_name；java数组表示：array[index]
<a name="Jw0Co"></a>
##### 举例：
（1）查询长度大于100的字符串
```bash
select s from java.lang.String s where s.count > 100
```
（2）查询长度大于256的数组
```bash
select a from [I a where a.length > 256
```
（3）显示匹配某一正则表达式的字符串
```bash
select a.value.toString() from java.lang.String s where /java/(s.value.toString())
```
（4）显示所有文件对象的文件路径
```bash
select file.path.value.toString() from java.io.File file
```
（5）显示所有ClassLoader的类名
```bash
select classof(cl).name from instanceof java.lang.ClassLoader cl
```
（6）通过引用查询对象
```bash
select o from instanceof 0xd404d404 o
```
**built-in对象 -- heap**<br />(1)`heap.findClass(class name)` -- 找到类
```bash
select heap.findClass("java.lang.String").superclass
```
(2)`heap.findObject(object id)` -- 找到对象
```bash
select heap.findObject("0xd404d404")
```
(3)`heap.classes` -- 所有类的枚举
```bash
select heap.classes
```
(4)`heap.objects` -- 所有对象的枚举
```bash
select heap.objects("java.lang.String")
```
(5)`heap.finalizables` -- 等待垃圾收集的java对象的枚举<br />(6)`heap.livepaths` -- 某一对象存活路径
```bash
select heaplivepaths(s) from java.lang.String s
```
(7)`heap.roots` -- 堆根集的枚举<br />**辨识对象的函数**<br />(1)`classof(class name)` -- 返回java对象的类对象
```bash
select classof(cl).name from instanceof java.lang.ClassLoader cl
```
(2)`identical(object1,object2)` -- 返回是否两个对象是同一个实例
```bash
select identical(heap.findClass("java.lang.String").name, heap.findClass("java.lang.String").name)
```
(3)`objectid(object)` -- 返回对象的id
```bash
select objectid(s) from java.lang.String s
```
(4)`reachables` -- 返回可从对象可到达的对象
```bash
select reachables(p) from java.util.Properties p -- 查询从Properties对象可到达的对象
select reachables(u, "java.net.URL.handler") from java.net.URL u -- 查询从URL对象可到达的对象，但不包括从URL.handler可到达的对象
```
(5)`referrers(object)` -- 返回引用某一对象的对象
```bash
select referrers(s) from java.lang.String s where s.count > 100
```
(6)`referees(object)` -- 返回某一对象引用的对象
```bash
select referees(s) from java.lang.String s where s.count > 100
```
(7)`refers(object1,object2)` -- 返回是否第一个对象引用第二个对象
```bash
select refers(heap.findObject("0xd4d4d4d4"),heap.findObject("0xe4e4e4e4"))
```
(8)`root(object)` -- 返回是否对象是根集的成员
```bash
select root(heap.findObject("0xd4d4d4d4")) 
```
(9)`sizeof(object)` -- 返回对象的大小
```bash
select sizeof(o) from [I o
```
(10)`toHtml(object)` -- 返回对象的html格式
```bash
select "<b>" + toHtml(o) + "</b>" from java.lang.Object o
```
(11)选择多值
```bash
select {name:t.name?t.name.toString():"null",thread:t} from instanceof java.lang.Thread t
```
**数组、迭代器等函数**<br />(1)`concat(enumeration1,enumeration2)` -- 将数组或枚举进行连接
```bash
select concat(referrers(p),referrers(p)) from java.util.Properties p
```
(2)`contains(array, expression)` -- 数组中元素是否满足某表达式
```bash
select p from java.util.Properties where contains(referres(p), "classof(it).name == 'java.lang.Class'")
```
返回由java.lang.Class引用的java.util.Properties对象

- built-in变量
- it -- 当前的迭代元素
- index -- 当前迭代元素的索引
- array -- 被迭代的数组

(3)`count(array, expression)` -- 满足某一条件的元素的数量
```bash
select count(heap.classes(), "/java.io./(it.name)")
```
(4)`filter(array, expression)` -- 过滤出满足某一条件的元素
```bash
select filter(heap.classes(), "/java.io./(it.name)")
```
(5)`length(array)` -- 返回数组长度
```bash
select length(heap.classes())
```
(6)`map(array,expression)` -- 根据表达式对数组中的元素进行转换映射
```bash
select map(heap.classes(),"index + '-->' + toHtml(it)")
```
(7)`max(array,expression)` -- 最大值，`min(array,expression)`
```bash
select max(heap.objects("java.lang.String"),"lhs.count>rhs.count")
```

- built-in变量
- lhs -- 左边元素
- rhs -- 右边元素

(8)`sort(array,expression)` -- 排序
```bash
select sort(heap.objects('[C'),'sizeof(lhs)-sizeof(rhs)')
```
(9)`sum(array,expression)` -- 求和
```bash
select sum(heap.objects('[C'),'sizeof(it)')
```
(10)`toArray(array)` -- 返回数组<br />(11)`unique(array)` -- 唯一化数组
<a name="qOFg2"></a>
### jmap
打印进程、核心文件或远程调试服务器的共享对象内存映射或堆内存详细信息。
```bash
jmap [option] <pid>
		(to connect to running process) 连接到正在运行的进程

jmap [option] <executable <core>
		(to connect to a core file)     连接到核心文件

jmap [option] [server_id@]<remote server IP or hostname>
		(to connect to remote debug server) 连接到远程调试服务
```
<a name="x5dpu"></a>
#### option

- **pid:**    目标进程的PID，进程编号，可以采用ps -ef | grep java 查看java进程的PID;
- **executable:**     产生core dump的java可执行程序;
- **core:**     将被打印信息的core dump文件;
- **remote-hostname-or-IP:**     远程debug服务的主机名或ip;
- **server-id:**     唯一id，假如一台主机上多个远程debug服务;
<a name="jKoU5"></a>
#### 使用方法

1. `jmap -dump:[live,]format=b,file=<filename> PID` ：使用hprof二进制形式，输出jvm的heap内容到文件
2. `jmap -finalizerinfo PID`：打印正等候回收的对象的信息
3. `jmap -heap PID`：打印heap的概要信息，GC使用的算法，heap（堆）的配置及JVM堆内存的使用情况。
4. `jmap -histo:live PID`：打印每个class的实例数目，内存占用，类全名信息。VM的内部类名字开头会加上前缀”*”. 如果live子参数加上后，只统计活的对象数量。
5. `jmap -permstat PID`：打印classload和jvm heap长久层的信息. 包含每个classloader的名字、活泼性、地址、父classloader和加载的class数量。另外，内部String的数量和占用内存数也会打印出来。
6. `-F` 强迫.在pid没有相应的时候使用-dump或者-histo参数。在这个模式下，live子参数无效。
7. `-h` | -help 打印辅助信息
8. `-J` 传递参数给jmap启动的jvm。
<a name="elN31"></a>
### jstack
jstack命令主要用于调试java程序运行过程中的线程堆栈信息，可以用于检测死锁，进程耗用cpu过高报警问题的排查。jstack命令会打印出所有的线程，包括用户自己启动的线程和jvm后台线程。
<a name="nUhkc"></a>
#### 命令格式
```bash
jstack -[option] pid
```
<a name="DfbaT"></a>
#### option

- `-F` 强制dump线程堆栈信息. 用于进程hung住， `jstack <pid>`命令没有响应的情况
- `-m` 同时打印java和本地(native)线程栈信息，m是mixed mode的简写
- `-l` 打印锁的额外信
