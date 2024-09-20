JVM<br />对于高并发访问量的电商、物联网、金融、社交等系统来说，**JVM内存优化是非常有必要的**，可以提高系统的**吞吐量和性能**。通常调优的首选方式是减少FGC次数或者FGC时间，以避免系统过多地暂停。FGC达到理想值后，比如一天或者两天触发一次FGC。FCT时间优化为100～300毫秒后，再减少YoungGC次数或者YoungGC时间，YoungGC仍然会消耗CPU资源，优化YoungGC调用次数和消耗的CPU资源，可以提高系统的吞吐量。<br />优化GC前，必须获取GC的实际使用情况，最好的方式是通过CG Log收集垃圾回收日志，通过一些可视化工具查看垃圾回收分析数据，比如GCEasy。持续优化和对比优化前后的GC Log，能确认吞吐量和性能是否得到提升。<br />下面推荐几个常用的**内存分析命令和工具**
<a name="ZS4nb"></a>
### **jstat命令**
JDK自带的jstat命令用于**查看虚拟机垃圾回收的情况**，如下命令使用gcutil参数输出堆内存使用情况统计：
```bash
jstat –gcutil -h 20 pid 1000 100
```
此命令显示进程为pid的内存使用汇总，1000毫秒输出一次，总共输出100行。-h 20表示每20行输出一次表头。-gcutil表示显示JVM内存使用汇总统计：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676349052617-1584cac7-69ff-4046-9b4e-ac777a3e6c4f.png#averageHue=%23f8f7f5&clientId=u120095e5-8c4e-4&from=paste&id=u84660e41&originHeight=339&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf21b9556-b2c6-42a3-bbb0-df2834fa8df&title=)<br />列表显示了虚拟机各个代的使用情况，**描述了堆内存的使用占比和垃圾回收次数，以及占用时间**，具体含义如下： 

- S0，第一个幸存区使用比值。
- S1，第二个幸存区的使用率。
- E，伊甸园区的使用比值。
- O，老年代。
- M，方法区、元空间使用率。
- CCS，压缩使用比值。
- YGC，年轻代垃圾回收次数。
- YGCT，年轻带垃圾回收占用时间。
- FGC，全局垃圾回收次数，这对性能影响至关重要。
- FGCT，全局垃圾回收的消耗时间。
- GCT，总的垃圾回收时间。

可以看到S0、S1、E变化频率高，说明程序在频繁创建生命周期短的对象，FGC为0，表示还未做过全局垃圾回收。**如果FGC变化频率很高，则说明系统性能和吞吐量将下降**，或者可能出现内存溢出。<br />其他查看汇总信息的常用选项如下：

- -gc，类似gcutil，gcutil以百分比形式显示内存的使用情况，gc显示的是内存占用的字节数，以KB的形式输出堆内存的使用情况。
- -gccause，类似gcutil，额外输出GC的原因。
<a name="hmQ7C"></a>
### jmap命令
jmap命令用于保存虚拟机内存镜像到文件中，然后可以使用JVisualVM或者MAT工具进行进一步分析。命令如下：
```bash
jmap -dump:format=b,file=filename.hprof pid
```
需要注意，实际系统会有2GB到8GB内存，**此命令会导致虚拟机暂停工作1～3秒。**还有一种是被动获取方式，当虚拟机出现内存溢出的时候，会主动“dump”内存文件。添加虚拟机启动参数：
```bash
-XX:+HeapDumpOnOutOfMemoryError -XX:HeapDumpPath=/tmp/heapdump.hprof
```
当虚拟机判断达到内存溢出触发条件的时候，会有如下输出并保存镜像文件：<br />java.lang.OutOfMemoryError: Java heap spaceDumping heap to heapdump.hprof ...<br />当获得镜像文件后，打开JvisualVM工具，选择菜单“File”，点击装入，选择保存过的dump文件，这时面板会打开内存镜像文件。打开较大的内存镜像文件需要较长的时间，需要耐心等候，其他工具，如**MAT，或者商业的YourKit Java Profiler打开镜像文件更快**，分析功能更强大。
<a name="ofELy"></a>
### **GCeasy**
GCeasy是一个分析GC日志文件的在线网站，能根据上传的GC日志，以图表形式显示GC回收过程和统计数据。下图显示的是**GC性能的统计情况**，如吞吐量显示为99.935%，说明只有少量CPU资源用于垃圾回收。最长的GC时间是20毫秒，属于正常范围。在测试JVM参数调整是否能增加吞吐量，减小垃圾回收占用的CPU时，可以使用这个统计功能。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676349052574-c0c3ade4-efad-45ff-84eb-3894f7e54634.png#averageHue=%23f1f1f1&clientId=u120095e5-8c4e-4&from=paste&id=u4ebf6029&originHeight=240&originWidth=546&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2ab3fb72-b923-4cc8-8b7d-1e2a87b3e3a&title=)<br />下图统计了GC总的时间和回收的字节数，也显示了Full GC的统计情况。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676349052610-783fb8e3-ca63-48eb-bb05-dceead644ad2.png#averageHue=%23a5b0a3&clientId=u120095e5-8c4e-4&from=paste&id=u5d54d7ee&originHeight=198&originWidth=544&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7a19a874-93c3-4a3d-bc10-0fd3f4e72af&title=)
<a name="qP3WI"></a>
### **JMC**
Java Mission Control简称JMC，是JDK自带的工具，是一个**高性能的对象监视、管理、产生时间分析和诊断的工具套件**，笔者主要用来**追踪热点代码与热点线程，是主要的内存优化调优工具。**<br />类似JVisualVM，通过JMX连接进入JMC控制台。<br />通过连接到远程JVM进程后，可以执行飞行记录（FlightRecord），选择飞行记录存放的路径与执行时间即可，如下图所示。需要注意的是，执行飞行记录功能时会对当前JVM进程有一定的性能影响（大约为5%～10%），所以**建议JMC连接隔离环境中的服务器并执行飞行记录功能。**<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676349052619-3dd98491-920f-452f-80eb-5ac583599aaf.png#averageHue=%23f3f2f2&clientId=u120095e5-8c4e-4&from=paste&id=u74698e54&originHeight=345&originWidth=554&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0a50c735-8474-4dcd-8158-d7e3b657789&title=)<br />通过一段时间的记录，飞行记录可以**反映线程的繁忙程度，以及CPU的热点方法。**<br />使用热点方法可以直接找到最耗时的几个方法，**对热点方法重点优化就可以使CPU的使用率下降一大截。**<br />飞行记录还可以**反映内存增长的热点方法**，以及显示单位时间内创建的最多对象的方法。下图为找到的内存对象中创建的最多的char[]的方法，一个是Fastjson，另一个是Kryo。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676349052618-8a45b2ca-6196-41f9-af8c-1ab0ecd9a8dc.png#averageHue=%23f3efee&clientId=u120095e5-8c4e-4&from=paste&id=u574b6837&originHeight=354&originWidth=549&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8579958c-f7e6-47fd-821a-c2b2aec2cb9&title=)<br />小结：通过JMC的热点方法的统计结果可以有针对性地进行优化，笔者通过对线上系统进行优化使得CPU使用率下降了40%、内存GC频率下降了100%以上。
<a name="qlJ2H"></a>
### **MAT**
MAT是Memory Analyzer的简称，它是**一款功能强大的Java堆内存分析器**，可以分析具有数亿个对象的内存镜像，快速计算对象大小，自动找到嫌疑的泄漏对象并形成内存泄漏报告。MAT是基于Eclipse开发的，是一款免费的内存镜像分析工具，是笔者**发现内存泄漏原因的主要工具。**<br />通过File-Open Heap Dump可以打开内存镜像文件，显示内容如下图所示。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676349053343-e7ea2dce-bf3f-40b4-afd1-505260d9b93f.png#averageHue=%23f9f9f9&clientId=u120095e5-8c4e-4&from=paste&id=u18c26bbc&originHeight=284&originWidth=546&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7008e15c-e7d4-4fc4-b104-c99c020ba07&title=)<br />它提供了Leak Suspects 报告，输出有可能发生内存泄漏的对象。
<a name="lqY2O"></a>
### **OQL**
OQL语句类似SQL语句，可以在VisualVM、MAT等大多数内存镜像分析工具中执行，**完成对象查找任务。**
