JVM 调优参数
<a name="PcGwD"></a>
## 堆栈内存相关

- `-Xms` 设置初始堆的大小
- `-Xmx` 设置最大堆的大小
- `-Xmn` 设置年轻代大小，相当于同时配置`-XX:NewSize`和`-XX:MaxNewSize`为一样的值
- `-Xss`  每个线程的堆栈大小
- `-XX:NewSize` 设置年轻代大小(for 1.3/1.4)
- `-XX:MaxNewSize` 年轻代最大值(for 1.3/1.4)
- `-XX:NewRatio` 年轻代与年老代的比值(除去持久代)
- `-XX:SurvivorRatio` Eden区与Survivor区的的比值
- `-XX:PretenureSizeThreshold` 当创建的对象超过指定大小时，直接把对象分配在老年代。
- `-XX:MaxTenuringThreshold` 设定对象在Survivor复制的最大年龄阈值，超过阈值转移到老年代
<a name="XlmnL"></a>
## 垃圾收集器相关

- `-XX:+UseParallelGC`：选择垃圾收集器为并行收集器。
- `-XX:ParallelGCThreads=20`：配置并行收集器的线程数
- `-XX:+UseConcMarkSweepGC`：设置年老代为并发收集。
- `-XX:CMSFullGCsBeforeCompaction=5` 由于并发收集器不对内存空间进行压缩、整理，所以运行一段时间以后会产生“碎片”，使得运行效率降低。此值设置运行5次GC以后对内存空间进行压缩、整理。
- `-XX:+UseCMSCompactAtFullCollection`：打开对年老代的压缩。可能会影响性能，但是可以消除碎片
<a name="JFPdq"></a>
## 辅助信息相关

- `-XX:+PrintGCDetails` 打印GC详细信息
- `-XX:+HeapDumpOnOutOfMemoryError` 让JVM在发生内存溢出的时候自动生成内存快照,排查问题用
- `-XX:+DisableExplicitGC` 禁止系统`System.gc()`，防止手动误触发FGC造成问题.
- `-XX:+PrintTLAB` 查看TLAB空间的使用情况
