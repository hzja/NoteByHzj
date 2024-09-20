JVM GC
<a name="b3953f7a"></a>
### jvm内存简图
![](https://cdn.nlark.com/yuque/0/2020/png/396745/1602810535384-bbf9d198-e79e-4cca-b20c-3107a96af1b9.png#align=left&display=inline&height=396&originHeight=396&originWidth=550&size=0&status=done&style=shadow&width=550)

1. jvm内存分为堆内存和非堆内存，堆内存分为年轻代、老年代，非堆内存里只有个永久代。<br />
2. 年轻代分为生成区（Eden）和幸存区（Survivor），幸存区由FromSpace和Tospace两部分组成，默认情况下，内存大小比例：Eden：FromSpace：ToSpace 为 8:1:1。<br />
3. 堆内存存放的是对象，垃圾收集器回收的就是这里的对象，不同区域的对象根据不同的GC算法回收，比如年轻代对应Minor GC，老年代对应Major GC。<br />
4. 非堆内存即永久代，也称为方法区，存储的是程序运行时长期存活的对象，比如类的元数据、方法、常量、属性等。<br />
5. 在jdk1.8废弃了永久代，使用元空间（MetaSpace）取而代之，元空间存储的对象与永久代相同，区别是：元空间并不在jvm中，使用的是本地内存。<br />
6. 为什么移除永久代呢<br />

为融合HotSpot JVM与JRockit VM（新JVM技术）而做出的改变，因为JRockit没有永久代。
<a name="GRbB1"></a>
### 分代概念
首先，GC是Garbage Collection，即垃圾回收。<br />新生成的对象首先存放在生成区，当生成区满了，触发Minor GC，存活下来的对象转移到Survivor0，即FromSpace，Survivor0区满后触发执行Minor GC，存活对象移动到Suvivor1区，即ToSpace，经过多次Minor GC仍然存活的对象转移到老年代。<br />所以老年代存储的是长期活动的对象，当老年代满了会触发Major GC。<br />Minor GC和Major GC是俗称，有些情况下Major GC和Full GC是等价的，如果出发了Full GC,那么所有线程必须等待GC完成才能继续（见GC分类和算法）。
<a name="4feac642"></a>
### 分代原因
将对象根据存活概率进行分类，对存活时间长的对象，放到固定区，从而减少扫描垃圾时间及GC频率。针对分类进行不同的垃圾回收算法，对算法扬长避短。
<a name="Gk91o"></a>
### 为什么幸存区分为大小相同的两部分：S0，S1
主要为了解决碎片化，因为回收一部分对象后，剩余对象占用的内存不连续，也就是碎片化，过于严重的话，当前连续的内存不够新对象存放就会触发GC，这样会提高GC的次数，降低性能，当S0 GC后存活对象转移到S1后存活对象占用的就是连续的内存。
<a name="PuhYo"></a>
### GC分类和相关算法
看一下GC分类，才能清楚什么时候触发Full GC、和非Full GC，GC大致分为两种：

- Partial GC：并不收集整个GC堆的模式，即可以理解为非Full GCYoung GC：只收集young gen的GC<br />
- Old GC：只收集old gen的GC。只有CMS有这个模式<br />
- Mixed GC：收集整个young gen以及部分old gen的GC。只有G1有这个模式<br />
- Full GC：收集整个堆，包括young gen、old gen、perm gen（如果存在的话）等所有部分的模式。<br />

上面说的CMS和G1都是GC的算法，相关GC算法如下：

1. Serial GC算法：Serial Young GC ＋ Serial Old GC （实际上它是全局范围的Full GC）；<br />
2. Parallel GC算法：Parallel Young GC ＋ 非并行的PS MarkSweep GC / 并行的Parallel Old GC（这俩实际上也是全局范围的Full GC），选PS MarkSweep GC 还是 Parallel Old GC 由参数UseParallelOldGC来控制；<br />
3. CMS算法：ParNew（Young）GC + CMS（Old）GC ＋ Full GC for CMS算法；<br />
4. G1 GC算法：Young GC + mixed GC（新生代，再加上部分老生代）＋ Full GC for G1 GC算法（应对G1 GC算法某些时候的不赶趟，开销很大）；<br />
<a name="yzpYW"></a>
### GC触发条件

- Young GC：各种Young GC触发的条件都是Eden区满了。<br />
- Serial Old GC／PS MarkSweep GC／Parallel Old GC：当准备要触发一次young GC时，如果发现统计数据说之前young GC的平均晋升大小比目前old gen剩余的空间大，则不会触发young GC而是转为触发full GC。<br />
- Full GC for CMS：老年代使用比率超过某个值。<br />
- Full GC for G1 GC：Heap使用比率超过某个值。<br />
- 如果有perm gen的话，要在perm gen分配空间但已经没有足够空间时，也要触发一次full GC。<br />
- 小结：不同算法对应的GC回收条件是不同的。<br />
<a name="8iIUf"></a>
### GC方式
<a name="a676e516"></a>
#### 标记-清除（Mark-Sweep）
GC分为两个阶段，标记和清除。首先标记所有可回收的对象，在标记完成后统一回收所有被标记的对象。同时会产生不连续的内存碎片，碎片过多会导致以后程序运行时需要分配较大对象时，无法找到足够的连续内存，而不得已再次触发GC。

- 红色代表被标记的可回收对象，绿色代表存活对象<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1602810535351-8f69609d-1664-47bb-a1c9-5c3b09278cae.png#align=left&display=inline&height=115&originHeight=115&originWidth=399&size=0&status=done&style=shadow&width=399)<br />清除后如下：<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1602810535404-cbf6814c-0db6-4788-bfe9-9d6dffcfd6ad.png#align=left&display=inline&height=118&originHeight=118&originWidth=403&size=0&status=done&style=none&width=403)
<a name="JDuCt"></a>
#### 复制（Copy）
将内存按容量划分为两块，每次只使用其中一块。当这一块内存用完了，就将存活的对象复制到另一块上，然后再把已使用的内存空间一次清理掉。这样使得每次都是对半个内存区回收，也不用考虑内存碎片问题，简单高效。缺点需要两倍的内存空间。<br />清除前后如下：<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1602810535333-93748b34-e2af-4e33-88d1-987915d6bca6.png#align=left&display=inline&height=78&originHeight=78&originWidth=550&size=0&status=done&style=none&width=550)![](https://cdn.nlark.com/yuque/0/2020/png/396745/1602810535400-b04985b4-e204-4af3-9dec-afa139de3832.png#align=left&display=inline&height=79&originHeight=79&originWidth=550&size=0&status=done&style=none&width=550)
<a name="mXNE2"></a>
#### 标记-整理（Mark-Compact）
也分为两个阶段，首先标记可回收的对象，再将存活的对象都向一端移动，然后清理掉边界以外的内存。此方 法避免标记-清除算法的碎片问题，同时也避免了复制算法的空间问题。<br />一般年轻代中执行GC后，会有少量的对象存活，就会选用复制算法，只要付出少量的存活对象复制成本就可以 完成收集。而老年代中因为对象存活率高，没有额外过多内存空间分配，就需要使用标记-清理或者标记-整理算法来进行回收。<br />清除前后如下：<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1602810535346-20842edc-3a97-40ae-93da-1b7a41dceeca.png#align=left&display=inline&height=115&originHeight=115&originWidth=400&size=0&status=done&style=none&width=400)![](https://cdn.nlark.com/yuque/0/2020/png/396745/1602810535456-b6166a76-b98a-4f3e-9948-a7eaa44ccedc.png#align=left&display=inline&height=41&originHeight=116&originWidth=400&size=0&status=done&style=none&width=140)
<a name="PUGSm"></a>
### GC算法参数
| 参数 | 描述 |
| :--- | :--- |
| -XX:+UseSerialGC | 串行收集器 |
| -XX:+UseParallelGC | 并行收集器 |
| -XX:+UseParallelGCThreads=8 | 并行收集器线程数，同时有多少个线程进行垃圾回收，一般与CPU数量相等 |
| -XX:+UseParallelOldGC | 指定老年代为并行收集 |
| -XX:+UseConcMarkSweepGC | CMS收集器（并发收集器） |
| -XX:+UseCMSCompactAtFullCollection | 开启内存空间压缩和整理，防止过多内存碎片 |
| -XX:CMSFullGCsBeforeCompaction=0 | 表示多少次Full GC后开始压缩和整理，0表示每次Full GC后立即执行压缩和整理 |
| -XX:CMSInitiatingOccupancyFraction=80% | 表示老年代内存空间使用80%时开始执行CMS收集，防止过多的Full GC |
| -XX:+UseG1GC | G1收集器 |
| -XX:MaxTenuringThreshold=0 | 在年轻代经过几次GC后还存活，就进入老年代，0表示直接进入老年代 |

<a name="GExkP"></a>
### OOM原因
1）老年代内存不足：java.lang.OutOfMemoryError:Javaheapspace<br />2）永久代内存不足：java.lang.OutOfMemoryError:PermGenspace<br />3）代码bug，占用内存无法及时回收。<br />可以通过添加个参数-XX:+HeapDumpOnOutMemoryError，让虚拟机在出现内存溢出异常时Dump出当前的内存堆转储快照以便事后分析。
