Java JVM <br />JDK16 GA已经发布很久了，但是，可以肯定的是，绝大多数的生产环境依然运行的是JDK8。既然运行的是JDK8，那么生产环境的垃圾回收器基本上就是下面3种：
<a name="XM6dt"></a>
## 默认垃圾回收器
这里只专注于如何配置一个比较合理的采用CMS作为垃圾回收器的JVM参数。首先要说的是，JDK8要使用CMS，那么必须显示申明，因为它采用的默认垃圾回收器是ParallelGC。如何验证它默认采用的垃圾回收器呢？非常简单，运行如下代码：
```java
import java.util.ArrayList;
import java.util.List;

public class Main {

    private static final int _1M = 1024*1024;

    public static void main(String[] args) {

        List<byte[]> byteList = new ArrayList<>();
        for(int i=0; i<Integer.MAX_VALUE; i++){
            byte[] test = new byte[_1M];
            byteList.add(test);
        }
    }

}
```
然后配置JVM参数：
```bash
-verbose:gc -XX:+PrintGCDetails
```
运行几秒钟后，强行停止JVM进程，就会在控制台中看到如下日志从而佐证JDK8采用的默认垃圾回收器就是ParallelGC：
```bash
[Full GC (Allocation Failure) [PSYoungGen: 342021K->342021K(348672K)] [ParOldGen: 1397423K->1397406K(1398272K)] 1739445K->1739427K(1746944K), [Metaspace: 3357K->3357K(1056768K)], 0.1902415 secs] [Times: user=0.26 sys=0.01, real=0.19 secs] 
```
或者可以通过如下信息得知默认垃圾回收器为ParallelGC：<br />![2021-07-26-21-06-30-523800.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627305110539-d5ae0497-c26c-45f0-8b03-83c779f74081.png#clientId=u941c0d4b-4c93-4&from=ui&id=u04be305b&originHeight=241&originWidth=1080&originalType=binary&ratio=1&size=31552&status=done&style=none&taskId=u2ac5610d-70ef-43ef-b2c4-451933c4241)
<a name="QFXaS"></a>
## CMS用法
接下来从多个方面介绍如何配置一个较好的使用CMS垃圾回收器的JVM参数参数。
<a name="IDcdq"></a>
### 显示申明CMS
显示申明垃圾回收器为CMS+parNew非常简单，只需要添加如下两个JVM参数：
```bash
-XX:+UseConcMarkSweepGC -XX:+UseParNewGC
```
这时候，再运行上面的代码，就会得到如下信息。由下图可知，这时候年轻代采用的是ParNew，而老年代采用的是CMS（concurrent mark-sweep）：<br />![2021-07-26-21-06-30-788214.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627305117922-4a1be29b-8d97-4c27-aadc-835f4fffde9e.png#clientId=u941c0d4b-4c93-4&from=ui&id=uf9e35795&originHeight=175&originWidth=1080&originalType=binary&ratio=1&size=18809&status=done&style=none&taskId=u5e7a2506-b275-444f-8efe-1e26a0854f1)<br />显示申明CMS只是使用CMS的第一步，接下来还有很多优化需要去做，还有很多JVM参数去配置。
<a name="OOE55"></a>
### 堆大小
接下来，最重要的就是申明年轻代和老年代的大小。由于采用的CMS+ParNew。建议堆大小不要超过8G，最好6G以内，因为CMS+ParNew组合情况下发生的FGC是采用MSC算法且单线程回收，如果堆内存很大，FGC时STW时间会非常恐怖。这里以4G举例，这时候再添加几个JVM参数，得到如下的配置。这里设置的年轻代大概是1.5G，老年代大概是2.5G。这算是一个比较合理的比例搭配。如果JVM参数这样搭配但是GC情况仍然不是很好，那么可能需要根据业务特性进行特别的调优：
```bash
-Xmx4g -Xms4g -Xmn1512m
```
<a name="Q4tZZ"></a>
### 线程栈
JDK8默认的线程栈大小为1M，有点偏大。以经验来看，绝大部分微服务项目是可以调整为512k，甚至256k的：
```bash
-Xss256k
```
<a name="cuyLJ"></a>
### Old回收阈值
既然配置的是CMS，那么如下两个参数一定要加上。为什么要加上这两个JVM参数呢？这是因为CMS回收条件非常复杂，如果不通过`CMSInitiatingOccupancyFraction`和`UseCMSInitiatingOccupancyOnly`限制只在老年代达到75%才回收的话（这个阈值可以根据具体情况适当调整），当线上碰到一些CMS GC时，是很难搞清楚原因的：
```bash
-XX:CMSInitiatingOccupancyFraction=75 -XX:+UseCMSInitiatingOccupancyOnly 
```
<a name="L00VM"></a>
### 元数据空间
如果是微服务架构，那么对于绝大部分应用来说，128M的元数据完全够用。不过，JDK8的元数据空间并不是指定多少就初始化多大的空间。而是按需扩展元数据空间。所以，可以设置256M。如果不设置这两个参数的话，元数据空间默认初始化只有20M出头，那么就会在应用启动过程中，Metaspace扩容发生FGC：
```bash
-XX:MaxMetaspaceSize=256m -XX:MetaspaceSize=256M
```
<a name="QcHdk"></a>
### dump路径
设定如下两个参数（需要说明的是，HeapDumpPath参数指定的路径需要提前创建好，JVM没办法在生成dump文件时创建该目录），当JVM内存导致导致JVM进程退出时能自动在该目录下生成dump文件：
```bash
-XX:+HeapDumpOnOutOfMemoryError -XX:HeapDumpPath=/data/log/jvmdump/
```
<a name="hQ5VY"></a>
### GC日志
这个必须有，不然线上环境GC问题都不好排查。并且loggc所在目录（/data/log/gclog/）和dump路径一样，必须提前创建好，JVM无法自动创建该目录：
```bash
-XX:+PrintGCDetails -XX:+PrintGCDateStamps -Xloggc:/data/log/gclog/gc.log
```
<a name="jRWsT"></a>
### 压缩
CMS GC是并发的垃圾回收器，它采用的是标记清除算法，而不是压缩算法。意味着随着时间的推移，碎片会越来越多，JVM终究会触发内存整理这个动作。那么，什么时候整理内存碎片呢？跟下面两个参数有很大的关系。第一个参数是开启这个能力，第二个参数表示在压缩（compaction）内存之前需要发生多少次不压缩内存的FGC。CMS GC不是FGC，在CMS GC搞不定的时候（比如：concurrent mode failure），会触发完全STW但不压缩内存的FGC（假定命名为NoCompactFGC），或者触发完全STW并且压缩内存的FGC（假定命名为CompactFGC）。所以，这个参数的意思就是，连续多少次NoCompactFGC后触发CompactFGC。如果中间出现了CMS GC，那么又需要重新计数NoCompactFGC发生的次数：
```bash
-XX:+UseCMSCompactAtFullCollection -XX:CMSFullGCsBeforeCompaction=0
```
这里给出的配置事实上是默认值，即每次CMS GC搞不定的情况下触发CompactFGC。这两个参数很不好理解，为此，举几个例子，假定有3种GC方式：CMS GC，NoCompactFGC， CompactFGC（如下时yi du a）：
```java
if(should_compact){
    // mark-sweep-compact
    do_compaction_work(clear_all_soft_refs) 
} else {   
    // mark-sweep
    do_mark_sweep_work(clear_all_soft_refs,first_state,should_start_over);
}
```
NoCompactFGC就是不压缩内存的FGC，采用的是标记清除（Mark-Sweep）算法，CompactFGC是会压缩内存的FGC，采用的是标记清除压缩算法(Mark Sweep Compact)，然后假设配置了`-XX:CMSFullGCsBeforeCompaction=3`，那么：<br />1、CMS GC, NoCompactFGC, NoCompactFGC, NoCompactFGC, CompactFGC（这时候如果发生FGC就会压缩内存）<br />2、CMS GC, NoCompactFGC, NoCompactFGC, CMS GC, NoCompactFGC（这时候如果发生FGC不会压缩内存，因为在此之前并没有连续3次NoCompactFGC）<br />3、CMS GC, CMS GC, CMS GC, NoCompactFGC（如果前面连续发生的是CMS GC，那么接下来触发的FGC还不会压缩内存）
<a name="SCH5t"></a>
### one more
最后，再推荐一个搭配CMS时很好用的JVM参数，如下所示。官方对该参数的说明为:`A System.gc()` request invokes a concurrent collection and also unloads classes during such a concurrent gc cycle (effective only when UseConcMarkSweepGC)。这句话总结如下：<br />1、只有在使用CMS时才有效。<br />2、当调用`System.gc()`时会用CMS这个并行垃圾回收器去进行回收（比如大量使用DirectByteBuffer进行堆外内存操作，需要FGC来回收堆外内存的场景。就可以通过该参数让本来需要FGC才能搞定的事情用CMS GC就可以搞定了）。<br />3、除了能唤起并行垃圾回收器，还能卸载类。
```bash
-XX:+ExplicitGCInvokesConcurrentAndUnloadsClasses
```
最终，得到配置的完整的JVM参数配置如下（此参数在以前的一个微服务项目中运行了数年，单机并发1000+，CMS GC大概是8天左右一次）：
```bash
-Xms4g -Xmx4g -Xmn1512m -server -Xss256k -XX:MetaspaceSize=256M  -XX:MaxMetaspaceSize=256m -XX:+PrintGCDetails -XX:+PrintGCDateStamps -Xloggc:/data/log/gclog/gc.log -XX:+HeapDumpOnOutOfMemoryError -XX:HeapDumpPath=/data/log/jvmdump/ -XX:+UseConcMarkSweepGC -XX:+UseParNewGC  -XX:CMSInitiatingOccupancyFraction=75 -XX:+UseCMSInitiatingOccupancyOnly -XX:+UseCMSCompactAtFullCollection -XX:CMSFullGCsBeforeCompaction=0 -XX:+CMSClassUnloadingEnabled -XX:+TieredCompilation  -XX:+ExplicitGCInvokesConcurrentAndUnloadsClasses
```
最后再介绍一个很好的校验JVM参数的网址：[https://opts.console.heapdump.cn/](https://opts.console.heapdump.cn/)，这里可以用到它的“参数检查”。不过需要说明的是：此网址的校验结果只是作为参考，是否完全符合生产环境，还得视情况而定，毕竟JVM调优可不是一件简单的事情：<br />![2021-07-26-21-06-30-901192.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627305130312-d0badce5-62bc-4c52-b3af-e3cf5257c768.png#clientId=u941c0d4b-4c93-4&from=ui&id=ubbee648e&originHeight=351&originWidth=1080&originalType=binary&ratio=1&size=101469&status=done&style=none&taskId=ubaa6cc3b-a430-4507-b4b4-36dc254e49b)
