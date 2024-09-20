JVM 堆
<a name="On5x6"></a>
### 堆的概述
一般来说：

- 一个Java程序的运行对应一个进程；
- 一个进程对应着一个JVM实例（JVM的启动由引导类加载器加载启动），同时也对应着多个线程；
- 一个JVM实例拥有一个运行时数据区（Runtime类，为饿汉式单例类）；
- 一个运行时数据区中的堆和方法区是多线程共享的，而本地方法栈、虚拟机栈、程序计数器是线程私有的。

堆空间差不多是最大的内存空间，也是运行时数据区最重要的内存空间。堆可以处于物理上不连续的内存空间，但在逻辑上它应该被视为连续的。<br />在方法结束后，堆中的对象不会马上被移除，仅仅在垃圾收集的时候才会被移除。堆，是GC(Garbage Collection，垃圾收集器)执行垃圾回收的重点区域。
<a name="fLWDP"></a>
### 堆内存大小设置
堆一旦被创建，它的大小也就确定了，初始内存默认为电脑物理内存大小的1/64，最大内存默认为电脑物理内存的1/4，但是堆空间的大小是可以调节，接下来演示一下。
<a name="qRy9z"></a>
#### 准备工具
JDK自带内存分析的工具:在已安装JDK的bin目录下找到jvisualvm.exe。打开该软件，下载插件Visual GC，一定要点击检查最新版本，否则会导致安装失败。![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623808250088-9661b752-4839-4cd2-84f6-e908cea0acc9.webp#clientId=u497b44d4-f005-4&from=paste&id=u8aaded54&originHeight=572&originWidth=1080&originalType=url&ratio=3&status=done&style=none&taskId=u301aa1c3-3d8d-4519-a644-67c610a586d)<br />安装完重启jvisualvm![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623808250116-5e562f3a-aee1-40f3-b56d-ee4069a9f926.webp#clientId=u497b44d4-f005-4&from=paste&id=ucd3d475d&originHeight=384&originWidth=1080&originalType=url&ratio=3&status=done&style=shadow&taskId=uae2c86b7-a9ba-474f-acec-0b39817dd62)
<a name="GuwIn"></a>
#### 代码样例
```java
public class HeapDemo {
    public static void main(String[] args) {
        System.out.println("start...");
        try {
            Thread.sleep(1000000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("end...");
    }
}
```
<a name="igzIE"></a>
#### IDEA设置
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623808250058-be64ccc7-4482-4a89-b73b-a8f5b4d593bb.webp#clientId=u497b44d4-f005-4&from=paste&id=u102869dd&originHeight=557&originWidth=843&originalType=url&ratio=3&status=done&style=none&taskId=u9e005efb-de67-4a1a-b8e6-fc792da4e8b)![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623808250200-401a15f4-5667-4bce-a09f-bff7a6672ff6.webp#clientId=u497b44d4-f005-4&from=paste&id=u32e1c28d&originHeight=791&originWidth=1080&originalType=url&ratio=3&status=done&style=none&taskId=ud527f1cb-eeee-4ec4-ba79-28b73a8b5b9)

- `-Xms10m`用于表示堆区的起始内存为10m，等价于`-XX:InitialHeapSize`；
- `-Xmx10m`用于表示堆区的最大内存为10m，等价于`-XX:MaxHeapSize`；
- 其中-X是JVM的运行参数，ms是memory start

通常会将-Xms和-Xmx两个参数配置相同的值，其目的就是为了能够在java垃圾回收机制清理完堆区后不需要重新分隔计算堆区的大小，从而提高性能。
<a name="dRab2"></a>
#### 启动程序
启动程序之后去jvisualvm查看![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623808250092-6869a6c8-a866-440f-83e3-456ed44cfe76.webp#clientId=u497b44d4-f005-4&from=paste&id=ud64bfad2&originHeight=516&originWidth=1080&originalType=url&ratio=3&status=done&style=shadow&taskId=uad880993-6d8c-4523-bc13-9136a86a207)一旦堆区中的内存大小超过-Xmx所指定的最大内存时，将会抛出OOM(Out Of MemoryError)异常。
<a name="YK6QG"></a>
### 堆的分代
存储在JVM中的java对象可以被划分为两类：

- 一类是生命周期较短的瞬时对象，这类对象的创建和消亡都非常迅速；
- 另一类是生命周期非常长，在某些情况下还能与JVM的生命周期保持一致；
<a name="WdHf9"></a>
#### 堆区分代
经研究表明70%-99%的对象属于临时对象，为了提高GC的性能，Hotspot虚拟机又将堆区进行了进一步划分。![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623808250580-c64cea77-42fd-4665-89b7-096d82c8f9b4.webp#clientId=u497b44d4-f005-4&from=paste&id=u00d92de6&originHeight=589&originWidth=1076&originalType=url&ratio=3&status=done&style=shadow&taskId=ubc7552b2-e1f7-4f44-8879-4a05de7825b)如图所示，堆区又分为年轻代（YoungGen）和老年代（OldGen）；其中年轻代又分为伊甸园区（Eden）和幸存者区（Survivor）；幸存者区分为幸存者0区（Survivor0，S0）和幸存者1区（Survivor1，S1），有时也叫from区和to区。<br />分代完成之后，GC时主要检测新生代Eden区。<br />**「统一概念：」**<br />新生区<=>新生代<=>年轻代<br />养老区<=>老年区<=>老年代<br />几乎所有的Java对象都是在Eden区被new出来的，有的大对象在该区存不下可直接进入老年代。绝大部分的Java对象都销毁在新生代了（IBM公司的专门研究表明，新生代80%的对象都是“朝生夕死”的）。
<a name="AFp79"></a>
#### 新生代与老年代在堆结构的占比

- 默认参数`-XX:NewRatio=2`，表示新生代占1，老年代占2，新生代占整个堆的1/3；
- 可以修改`-XX:NewRatio=4`，表示新生代占1，老年代占4，新生代占整个堆的1/5;

该参数在开发中一般不会调整，如果生命周期长的对象偏多时可以选择调整。
<a name="my2iq"></a>
#### Eden与Survivor在堆结构的占比
在HotSpot中，Eden空间和另外两个Survivor空间所占的比例是8：1：1（测试的时候是6：1：1），开发人员可以通过选项`-XX:SurvivorRatio`调整空间比例，如`-XX:SurvivorRatio=8`<br />可以在cmd中通过`jps 查询进程号-> jinfo -flag NewRatio(SurvivorRatio) + 进程号` 查询配置信息<br />-Xmn设置新生代最大内存大小（默认就好），如果既设置了该参数，又设置了NewRatio的值，则以该参数设置为准。
<a name="ZLIMY"></a>
#### 查看设置的参数
以上边的代码为例：设置启动参数`-XX:+PrintGCDetails`；可在cmd窗口中输入jps查询进程号，然后通过jstat -gc 进程id指令查看进程的内存使用情况。![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623808250582-c23590dd-e6cf-4ccf-b159-6ddd0d2a1c6d.webp#clientId=u497b44d4-f005-4&from=paste&id=uf3020513&originHeight=413&originWidth=1080&originalType=url&ratio=3&status=done&style=none&taskId=u3bebf874-c221-460b-b7a0-18d5c1aa0f6)
<a name="Q4aPc"></a>
### 图解对象分配过程
<a name="uVi1M"></a>
#### 对象分配过程
![](https://cdn.nlark.com/yuque/0/2021/gif/396745/1623808250577-2174170a-91a2-4830-92f9-6b5c23e5c08f.gif#clientId=u497b44d4-f005-4&from=paste&id=u7b732c78&originHeight=777&originWidth=1079&originalType=url&ratio=3&status=done&style=shadow&taskId=uf56260ad-95de-493f-9a38-8ffdb6c0c43)

1. new的对象先放伊甸园区，此区有大小限制；
2. 当伊甸园的空间填满时，程序继续创建对象，JVM的垃圾回收器将对伊甸园区进行垃圾回收（Minor GC，也叫YGC)：将伊甸园区中的不再被其他对象所引用的对象进行销毁，将未被销毁的对象移动到幸存者0区并分配age；
3. 然后再加载新的对象放到伊甸园区；
4. 如果再次触发垃圾回收，将此次未被销毁的对象和上一次放在幸存者0区且此次也未被销毁的对象一齐移动到幸存者一区，此时新对象的age为1，上次的对象的age加1变为2；
5. 如果再次经历垃圾回收，此时会重新放回幸存者0区，接着再去幸存者1区，age也随之增加；
6. 默认当age为15时，未被回收的对象将移动到老年区。可以通过设置参数来更改默认配置：`-XX:MaxTenuringThreshold=<N>`；该过程称为晋升（promotion)；
7. 在养老区，相对悠闲，当老年区内存不足时，再次触发GC（Major GC），进行养老区的内存清理；
8. 若养老区执行了Major GC之后发现依然无法进行对象的保存，就会产生OOM异常。

S0，S1满时不会触发YGC，但是YGC会回收S0，S1的对象。<br />**「总结」**

- 针对幸存者s0，s1区：复制之后有交换，谁空谁是to；
- 关于垃圾回收：频繁在新生区收集，很少在养老区收集，几乎不再永久区/元空间收集。
<a name="RU03a"></a>
#### 对象特殊情况分配过程
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623808250574-8b699eea-b891-4957-b781-1072db6a3417.webp#clientId=u497b44d4-f005-4&from=paste&id=u3063f552&originHeight=727&originWidth=842&originalType=url&ratio=3&status=done&style=shadow&taskId=ua9a871af-bd74-4ba3-a0d3-6a2a243cdfc)

1. 新对象申请内存，如果Eden放的下，则直接存入Eden；如果存不下则进行YGC；
2. YGC之后如果能存下则放入Eden，如果还存不下（为超大对象），则尝试存入Old区；
3. 如果Old区可以存放，则存入；如果不能存入，则进行Full GC；
4. Full GC之后如果可以存入Old区，则存入；如果内存空间还不够，则OOM；
5. 图右侧为YGC的流程图：当YGC之后未销毁的对象放入幸存者区，此时如果幸存者区的空间可以装下该对象，则存入幸存者区，否则，直接存入老年代；
6. 当在幸存者区的对象超过阈值时，可以晋升为老年代，未达到阈值的依旧在幸存者区复制交换。
<a name="ppHJy"></a>
#### 内存分配策略
针对不同年龄段的对象分配原则如下：

1. 优先分配到Eden；
2. 大对象直接分配到老年代：尽量避免程序中出现过多的大对象；
3. 长期存活的对象分配到老年代；
4. 动态对象年龄判断：如果Survivor区中相同年龄的所有对象大小的总和大于Survivor空间的一半，年龄大于或等于该年龄的对象可以直接进入到老年代。无需等到`MaxTenuringThreshold`中要求的年龄；
<a name="eshMM"></a>
#### 数值变小原理
代码样例，设置参数:`-Xms600m，-Xmx600m`
```java
public class HeapSpaceInitial {
    public static void main(String[] args) {

        //返回Java虚拟机中的堆内存总量
        long initialMemory = Runtime.getRuntime().totalMemory() / 1024 / 1024;
        //返回Java虚拟机试图使用的最大堆内存量
        long maxMemory = Runtime.getRuntime().maxMemory() / 1024 / 1024;

        System.out.println("-Xms : " + initialMemory + "M");
        System.out.println("-Xmx : " + maxMemory + "M");
        
        try {
            Thread.sleep(1000000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
```
执行结果
```java
-Xms : 575M
-Xmx : 575M
```
明明设置的600M，怎么变成575M了呢？这是因为在堆内存存取数据时，新生代里边只有伊甸园和幸存者1区或者是幸存者2区存储对象，所以会少一个幸存者区的内存空间。
<a name="GDDdm"></a>
### GC
JVM进行GC时，并非每次都对新生代、老年代、方法区（永久代、元空间）这三个区域一起回收，大部分回收是指新生代。<br />针对HotSpot VM的实现，它里面的GC按照回收区域又分为两大种类型：一种是部分收集（Partial GC），一种是整堆收集（Full GC）
<a name="aTo69"></a>
#### Partial GC
部分收集：不是完整收集整个Java堆的垃圾收集。其中又分为：

- 新生代收集（Minor GC/Young GC）：只是新生代的垃圾收集；
- 老年代收集（Major GC/Old GC）：只是老年代的垃圾收集；
- 混合收集（Mixed GC）：收集整个新生代以及部分老年代的垃圾收集，只有G1 GC （按照region划分新生代和老年代的数据）会有这种行为。

目前，只有CMS GC会有单独收集老年代的行为；很多时候Major GC会和Full GC 混淆使用，需要具体分辨是老年代回收还是整堆回收。
<a name="WkkUY"></a>
#### Full GC
整堆收集（Full GC）：整个java堆和方法区的垃圾收集。
<a name="kDjVH"></a>
#### 触发机制
<a name="bpuyz"></a>
##### 年轻代GC（Minor GC）触发机制

1. 当年轻代空间不足时，就会触发Minor GC，这里的年轻代满指的是Eden代满，Survivor满不会引发GC。(每次Minor GC会清理年轻代的内存，Survivor是被动GC，不会主动GC)
2. 因为Java对象大多都具备“朝生夕灭”的特性，所以Minor GC非常频繁，一般回收速度也比较快。
3. Minor GC会引发STW（Stop The World），暂停其他用户的线程，等垃圾回收结束，用户线程才恢复运行。
<a name="MsdPy"></a>
##### 老年代GC(Major GC/Full GC)触发机制

1. 指发生在老年代的GC，对象从老年代消失时，Major GC或者Full GC发生了；
2. 出现了Major GC，经常会伴随至少一次的Minor GC（不是绝对的，在Parallel Scavenge收集器的收集策略里就有直接进行Major GC的策略选择过程），也就是老年代空间不足时，会先尝试触发Minor GC。如果之后空间还不足，则触发Major GC；
3. Major GC速度一般会比Minor GC慢10倍以上，STW时间更长；
4. 如果Major GC后，内存还不足，就报OOM了。
<a name="eKHDN"></a>
##### Full GC触发机制
触发Full GC执行的情况有以下五种：

1. 调用`System.gc()`时，系统建议执行Full GC，但是不必然执行；
2. 老年代空间不足；
3. 方法区空间不足；
4. 通过Minor GC后进入老年代的平均大小小于老年代的可用内存；
5. 由Eden区，Survivor S0（from）区向S1（to）区复制时，对象大小大于To Space可用内存，则把该对象转存到老年代，且老年代的可用内存小于该对象大小。
:::info
Full GC是开发或调优中尽量要避免的，这样暂停时间会短一些。
:::
