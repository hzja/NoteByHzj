JVM
<a name="EtbjN"></a>
## 使用 JDK 自带工具查看 JVM 情况
JDK 自带了很多命令行甚至是图形界面工具，帮助查看 JVM 的一些信息。比如，在机器上运行 **ls** 命令，可以看到 JDK 8 提供了非常多的工具或程序：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650165523953-db8eae1b-fe8a-4629-a630-09726d871fc1.png#clientId=u67fd43b5-a019-4&from=paste&id=u2c941268&originHeight=79&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1fba90ee-839a-4c1e-832a-345cb68b6d3&title=)<br />接下来介绍些常用的监控工具。也可以先通过下面这张图了解下各种工具的基本作用：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1650165523960-288ad0aa-568a-427e-b66f-6787be78328d.jpeg#clientId=u67fd43b5-a019-4&from=paste&id=u7c7996fb&originHeight=716&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc145a263-7695-4926-8f6c-2a1c101dda1&title=)<br />为了测试这些工具，先来写一段代码：启动 10 个死循环的线程，每个线程分配一个 10MB 左右的字符串，然后休眠 10 秒。可以想象到，这个程序会对 GC 造成压力：
```java
//启动10个线程
IntStream.rangeClosed(1, 10).mapToObj(i -> new Thread(() -> {
	while (true) {
		//每一个线程都是一个死循环，休眠10秒，打印10M数据
		String payload = IntStream.rangeClosed(1, 10000000)
			.mapToObj(__ -> "a")
			.collect(Collectors.joining("")) + UUID.randomUUID().toString();
		try {
			TimeUnit.SECONDS.sleep(10);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		System.out.println(payload.length());
	}
})).forEach(Thread::start);


TimeUnit.HOURS.sleep(1);
```
修改 pom.xml，配置 spring-boot-maven-plugin 插件打包的 Java 程序的 main 方法类：
```xml
<plugin>
	<groupId>org.springframework.boot</groupId>
	<artifactId>spring-boot-maven-plugin</artifactId>
	<configuration>
		<mainClass>org.geekbang.time.commonmistakes.troubleshootingtools.jdktool.CommonMistakesApplication
		</mainClass>
	</configuration>
</plugin>
```
然后使用 `java -jar` 启动进程，设置 JVM 参数，让堆最小最大都是 1GB：
```bash
java -jar common-mistakes-0.0.1-SNAPSHOT.jar -Xms1g -Xmx1g
```
完成这些准备工作后，就可以使用 JDK 提供的工具，来观察分析这个测试程序了。
<a name="nDKPa"></a>
## jps
首先，使用 jps 得到 Java 进程列表，这会比使用 ps 来的方便：
```bash
➜  ~ jps
12707
22261 Launcher
23864 common-mistakes-0.0.1-SNAPSHOT.jar
15608 RemoteMavenServer36
23243 Main
23868 Jps
22893 KotlinCompileDaemon
```
<a name="USyBN"></a>
## jinfo
然后，可以使用 jinfo 打印 JVM 的各种参数：
```bash
➜  ~ jinfo 23864
Java System Properties:
#Wed Jan 29 12:49:47 CST 2020
...
user.name=zhuye
path.separator=\:
os.version=10.15.2
java.runtime.name=Java(TM) SE Runtime Environment
file.encoding=UTF-8
java.vm.name=Java HotSpot(TM) 64-Bit Server VM
...


VM Flags:
-XX:CICompilerCount=4 -XX:ConcGCThreads=2 -XX:G1ConcRefinementThreads=8 -XX:G1HeapRegionSize=1048576 -XX:GCDrainStackTargetSize=64 -XX:InitialHeapSize=268435456 -XX:MarkStackSize=4194304 -XX:MaxHeapSize=4294967296 -XX:MaxNewSize=2576351232 -XX:MinHeapDeltaBytes=1048576 -XX:NonNMethodCodeHeapSize=5835340 -XX:NonProfiledCodeHeapSize=122911450 -XX:ProfiledCodeHeapSize=122911450 -XX:ReservedCodeCacheSize=251658240 -XX:+SegmentedCodeCache -XX:+UseCompressedClassPointers -XX:+UseCompressedOops -XX:+UseG1GC


VM Arguments:
java_command: common-mistakes-0.0.1-SNAPSHOT.jar -Xms1g -Xmx1g
java_class_path (initial): common-mistakes-0.0.1-SNAPSHOT.jar
Launcher Type: SUN_STANDARD
```
查看第 15 行和 19 行可以发现，**设置 JVM 参数的方式不对，**`**-Xms1g**`** 和 **`**-Xmx1g**`** 这两个参数被当成了 Java 程序的启动参数**，整个 JVM 目前最大内存是 4GB 左右，而不是 1GB。<br />因此，当怀疑 JVM 的配置很不正常的时候，要第一时间使用工具来确认参数。除了使用工具确认 JVM 参数外，也可以打印 VM 参数和程序参数：
```java
System.out.println("VM options");
System.out.println(ManagementFactory.getRuntimeMXBean().getInputArguments().stream().collect(Collectors.joining(System.lineSeparator())));
System.out.println("Program arguments");
System.out.println(Arrays.stream(args).collect(Collectors.joining(System.lineSeparator())));
```
把 JVM 参数放到 -jar 之前，重新启动程序，可以看到如下输出，从输出也可以确认这次 JVM 参数的配置正确了：
```bash
➜  target git:(master) ✗ java -Xms1g -Xmx1g -jar common-mistakes-0.0.1-SNAPSHOT.jar test
VM options
-Xms1g
-Xmx1g
Program arguments
test
```
<a name="ypyJz"></a>
## jvisualvm
然后，启动另一个重量级工具 jvisualvm 观察一下程序，可以在概述面板再次确认 JVM 参数设置成功了：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650165523964-39bd07c0-0099-4564-be14-eb9f546a1a30.png#clientId=u67fd43b5-a019-4&from=paste&id=ue4705b5f&originHeight=675&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2aaec4cd-c4d0-4056-b2ac-df1d8e4ad80&title=)<br />继续观察监视面板可以看到，JVM 的 GC 活动基本是 10 秒发生一次，堆内存在 250MB 到 900MB 之间波动，活动线程数是 22。可以在监视面板看到 JVM 的基本情况，也可以直接在这里进行手动 GC 和堆 Dump 操作：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650165523978-20bc9f7c-9db1-4029-82ac-9e34244164e6.png#clientId=u67fd43b5-a019-4&from=paste&id=u9178d0f7&originHeight=675&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u39b31e2f-eeff-4db0-92c2-9f7d4f34b75&title=)<br />jconsole如果希望看到各个内存区的 GC 曲线图，可以使用 jconsole 观察。jconsole 也是一个综合性图形界面监控工具，比 jvisualvm 更方便的一点是，可以用曲线的形式监控各种数据，包括 MBean 中的属性值：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650165523934-e1742adb-d8a7-4f79-9239-0e9526ca2065.png#clientId=u67fd43b5-a019-4&from=paste&id=ucf1b3efc&originHeight=806&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8e18fc44-e4eb-4585-a91a-b0217ee7862&title=)
<a name="vJpoD"></a>
## jstat
同样，如果没有条件使用图形界面（毕竟在 Linux 服务器上，主要使用命令行工具），又希望看到 GC 趋势的话，可以使用 jstat 工具。<br />jstat 工具允许以固定的监控频次输出 JVM 的各种监控指标，比如使用 `-gcutil` 输出 GC 和内存占用汇总信息，每隔 5 秒输出一次，输出 100 次，可以看到 Young GC 比较频繁，而 Full GC 基本 10 秒一次：
```bash
➜  ~ jstat -gcutil 23940 5000 100
  S0     S1     E      O      M     CCS    YGC     YGCT    FGC    FGCT    CGC    CGCT     GCT
  0.00 100.00   0.36  87.63  94.30  81.06    539   14.021    33    3.972   837    0.976   18.968
  0.00 100.00   0.60  69.51  94.30  81.06    540   14.029    33    3.972   839    0.978   18.979
  0.00   0.00   0.50  99.81  94.27  81.03    548   14.143    34    4.002   840    0.981   19.126
  0.00 100.00   0.59  70.47  94.27  81.03    549   14.177    34    4.002   844    0.985   19.164
  0.00 100.00   0.57  99.85  94.32  81.09    550   14.204    34    4.002   845    0.990   19.196
  0.00 100.00   0.65  77.69  94.32  81.09    559   14.469    36    4.198   847    0.993   19.659
  0.00 100.00   0.65  77.69  94.32  81.09    559   14.469    36    4.198   847    0.993   19.659
  0.00 100.00   0.70  35.54  94.32  81.09    567   14.763    37    4.378   853    1.001   20.142
  0.00 100.00   0.70  41.22  94.32  81.09    567   14.763    37    4.378   853    1.001   20.142
  0.00 100.00   1.89  96.76  94.32  81.09    574   14.943    38    4.487   859    1.007   20.438
  0.00 100.00   1.39  39.20  94.32  81.09    575   14.946    38    4.487   861    1.010   20.442
```
其中，S0 表示 Survivor0 区占用百分比，S1 表示 Survivor1 区占用百分比，E 表示 Eden 区占用百分比，O 表示老年代占用百分比，M 表示元数据区占用百分比，YGC 表示年轻代回收次数，YGCT 表示年轻代回收耗时，FGC 表示老年代回收次数，FGCT 表示老年代回收耗时。<br />jstat 命令的参数众多，包含 `-class`、`-compiler`、`-gc` 等。Java 8、Linux/Unix 平台 jstat 工具的完整介绍，可以查看这里。jstat 定时输出的特性，可以方便持续观察程序的各项指标。<br />继续来到线程面板可以看到，大量以 Thread 开头的线程基本都是有节奏的 10 秒运行一下，其他时间都在休眠，和代码逻辑匹配：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650165524385-3555f056-fc13-425a-873b-9e1c286b214e.png#clientId=u67fd43b5-a019-4&from=paste&id=u9422d6c3&originHeight=675&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4e1d11a3-c4e4-4f1a-ac36-a46e7295221&title=)<br />点击面板的线程 Dump 按钮，可以查看线程瞬时的线程栈：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650165524334-b497ccc2-0483-4502-958a-80f9598fb4ac.png#clientId=u67fd43b5-a019-4&from=paste&id=uf3ba8d3d&originHeight=675&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u61e8f8ae-8d7c-4cdd-be59-e6e4301c4a8&title=)
<a name="mGYNb"></a>
## jstack
通过命令行工具 jstack，也可以实现抓取线程栈的操作：
```bash
➜  ~ jstack 23940
2020-01-29 13:08:15
Full thread dump Java HotSpot(TM) 64-Bit Server VM (11.0.3+12-LTS mixed mode):

...

"main" #1 prio=5 os_prio=31 cpu=440.66ms elapsed=574.86s tid=0x00007ffdd9800000 nid=0x2803 waiting on condition  [0x0000700003849000]
java.lang.Thread.State: TIMED_WAITING (sleeping)
at java.lang.Thread.sleep(java.base@11.0.3/Native Method)
at java.lang.Thread.sleep(java.base@11.0.3/Thread.java:339)
at java.util.concurrent.TimeUnit.sleep(java.base@11.0.3/TimeUnit.java:446)
at org.geekbang.time.commonmistakes.troubleshootingtools.jdktool.CommonMistakesApplication.main(CommonMistakesApplication.java:41)
at jdk.internal.reflect.NativeMethodAccessorImpl.invoke0(java.base@11.0.3/Native Method)
at jdk.internal.reflect.NativeMethodAccessorImpl.invoke(java.base@11.0.3/NativeMethodAccessorImpl.java:62)
at jdk.internal.reflect.DelegatingMethodAccessorImpl.invoke(java.base@11.0.3/DelegatingMethodAccessorImpl.java:43)
at java.lang.reflect.Method.invoke(java.base@11.0.3/Method.java:566)
at org.springframework.boot.loader.MainMethodRunner.run(MainMethodRunner.java:48)
at org.springframework.boot.loader.Launcher.launch(Launcher.java:87)
at org.springframework.boot.loader.Launcher.launch(Launcher.java:51)
at org.springframework.boot.loader.JarLauncher.main(JarLauncher.java:52)

"Thread-1" #13 prio=5 os_prio=31 cpu=17851.77ms elapsed=574.41s tid=0x00007ffdda029000 nid=0x9803 waiting on condition  [0x000070000539d000]
java.lang.Thread.State: TIMED_WAITING (sleeping)
at java.lang.Thread.sleep(java.base@11.0.3/Native Method)
at java.lang.Thread.sleep(java.base@11.0.3/Thread.java:339)
at java.util.concurrent.TimeUnit.sleep(java.base@11.0.3/TimeUnit.java:446)
at org.geekbang.time.commonmistakes.troubleshootingtools.jdktool.CommonMistakesApplication.lambda$null$1(CommonMistakesApplication.java:33)
at org.geekbang.time.commonmistakes.troubleshootingtools.jdktool.CommonMistakesApplication$$Lambda$41/0x00000008000a8c40.run(Unknown Source)
at java.lang.Thread.run(java.base@11.0.3/Thread.java:834)


...
```
抓取后可以使用类似**fastthread**（[https://fastthread.io/](https://fastthread.io/)）这样的在线分析工具来分析线程栈。
<a name="j3yut"></a>
## jcmd
最后，来看一下 Java HotSpot 虚拟机的 NMT 功能。<br />通过 NMT，可以观察细粒度内存使用情况，设置 `-XX:NativeMemoryTracking=summary/detail` 可以开启 NMT 功能，开启后可以使用 jcmd 工具查看 NMT 数据。<br />重新启动一次程序，这次加上 JVM 参数以 detail 方式开启 NMT：
```bash
-Xms1g -Xmx1g -XX:ThreadStackSize=256k -XX:NativeMemoryTracking=detail
```
在这里，还增加了 `-XX:ThreadStackSize` 参数，并将其值设置为 256k，也就是期望把线程栈设置为 256KB。通过 NMT 观察一下设置是否成功。<br />启动程序后执行如下 jcmd 命令，以概要形式输出 NMT 结果。可以看到，当前有 32 个线程，线程栈总共保留了差不多 4GB 左右的内存。明明配置线程栈最大 256KB，为什么会出现 4GB 这么夸张的数字呢，到底哪里出了问题呢？
```bash
➜  ~ jcmd 24404 VM.native_memory summary
24404:


Native Memory Tracking:


Total: reserved=6635310KB, committed=5337110KB
-                 Java Heap (reserved=1048576KB, committed=1048576KB)
                            (mmap: reserved=1048576KB, committed=1048576KB)


-                     Class (reserved=1066233KB, committed=15097KB)
                            (classes #902)
                            (malloc=9465KB #908)
                            (mmap: reserved=1056768KB, committed=5632KB)


-                    Thread (reserved=4209797KB, committed=4209797KB)
                            (thread #32)
                            (stack: reserved=4209664KB, committed=4209664KB)
                            (malloc=96KB #165)
                            (arena=37KB #59)


-                      Code (reserved=249823KB, committed=2759KB)
                            (malloc=223KB #730)
                            (mmap: reserved=249600KB, committed=2536KB)


-                        GC (reserved=48700KB, committed=48700KB)
                            (malloc=10384KB #135)
                            (mmap: reserved=38316KB, committed=38316KB)


-                  Compiler (reserved=186KB, committed=186KB)
                            (malloc=56KB #105)
                            (arena=131KB #7)


-                  Internal (reserved=9693KB, committed=9693KB)
                            (malloc=9661KB #2585)
                            (mmap: reserved=32KB, committed=32KB)


-                    Symbol (reserved=2021KB, committed=2021KB)
                            (malloc=1182KB #334)
                            (arena=839KB #1)


-    Native Memory Tracking (reserved=85KB, committed=85KB)
                            (malloc=5KB #53)
                            (tracking overhead=80KB)


-               Arena Chunk (reserved=196KB, committed=196KB)
                            (malloc=196KB)            
```
重新以 `VM.native_memory detail` 参数运行 jcmd：
```bash
jcmd 24404 VM.native_memory detail
```
可以看到，**有 16 个可疑线程，每一个线程保留了 262144KB 内存，也就是 256MB**（通过下图红框可以看到，使用关键字 262144KB for Thread Stack from 搜索到了 16 个结果）：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650165524389-e597fbeb-0495-44f1-9a60-a46715adee9e.png#clientId=u67fd43b5-a019-4&from=paste&id=u3b45ba7a&originHeight=776&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6901970d-27c7-4c5a-b6ec-7c61ccfdca2&title=)<br />其实，ThreadStackSize 参数的单位是 KB，所以如果要设置线程栈 256KB，那么应该设置 256 而不是 256k。重新设置正确的参数后，使用 jcmd 再次验证下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650165524421-1c13656c-d3fd-4268-8473-4422c8c0a67d.png#clientId=u67fd43b5-a019-4&from=paste&id=uf77d0ec7&originHeight=799&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uae1e8a60-56a5-4c1f-bf5c-f44262a4c6d&title=)<br />除了用于查看 NMT 外，jcmd 还有许多功能。可以通过 help，看到它的所有功能：
```bash
jcmd 24781 help
```
除了 jps、jinfo、jcmd、jstack、jstat、jconsole、jvisualvm 外，JDK 中还有一些工具，可以通过官方文档查看完整介绍。<br />官方文档：[https://docs.oracle.com/javase/8/docs/technotes/tools/](https://docs.oracle.com/javase/8/docs/technotes/tools/)
