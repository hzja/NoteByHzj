JVM垃圾回收算法<br />谈垃圾回收器之前，要先讲讲垃圾回收算法，以及JVM对垃圾的认定策略，JVM垃圾回收器是垃圾回收算法的具体实现，了解了前面的前置知识，有利于对垃圾回收器的理解。
<a name="MVayh"></a>
## 什么是垃圾?
垃圾，主要是指堆上的对象，那么如何确定这些对象是可以被回收的呢?<br />大概思路就是，如果一个对象永远不可能被访问到，那么就是垃圾，可以被回收了如何确定对象永远不会被使用呢?
<a name="gYqVG"></a>
### 引用计数法
在对象中添加一个引用计数器，每当有一个地方引用它时，计数器值就加一；当引用失效时，计数器值就减一；任何时刻计数器为零的对象就是不可能再被使用的。但是，在Java领域，至少主流的Java虚拟机里面都没有选用引用计数算法来管理内存，主要原因是，这个看似简单的算法有很多例外情况要考虑，必须要配合大量额外处理才能保证正确地工作，譬如单纯的引用计数就很难解决对象之间相互循环引用的问题。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1656393881355-1d876d3a-1ccd-4126-ad27-651ba462add0.jpeg#clientId=uba2b4285-ab7d-4&from=paste&id=u7fac4887&originHeight=490&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ufcb24b38-dd8b-4b77-b889-dc7c006a209&title=)<br />如图，每一个对象的引用都是1，构成了循环引用，但是并不能被其他对象访问，这两个对象再无任何引用，引用计数算法也就无法回收它们。<br />代码验证:
```java
package com.courage;
public class ReferenceCountingGC {
    public Object instance = null;
    private static final int _1MB = 1024 * 1024;
    /**
     * 这个成员属性的唯一意义就是占点内存，以便能在GC日志中看清楚是否有回收过
     */
    private byte[] bigSize = new byte[5* _1MB];
    public static void testGC() {
        //5 M
        ReferenceCountingGC objA = new ReferenceCountingGC();
        //5 M
        ReferenceCountingGC objB = new ReferenceCountingGC();
        objA.instance = objB;
        objB.instance = objA;
        objA = null;
        objB = null;
// 假设在这行发生GC，objA和objB是否能被回收？
        System.gc();
    }
    public static void main(String[] args) {
        testGC();
    }
}
```
执行结果:
```java
[0.004s][warning][gc] -XX:+PrintGCDetails is deprecated. Will use -Xlog:gc* instead.
[0.012s][info   ][gc,heap] Heap region size: 1M
[0.015s][info   ][gc     ] Using G1
[0.015s][info   ][gc,heap,coops] Heap address: 0x0000000701000000, size: 4080 MB, Compressed Oops mode: Zero based, Oop shift amount: 3
......
[0.119s][info   ][gc,metaspace   ] GC(0) Metaspace: 805K->805K(1056768K)
[0.119s][info   ][gc             ] GC(0) Pause Full (System.gc()) 14M->0M(8M) 2.886ms
[0.119s][info   ][gc,cpu         ] GC(0) User=0.03s Sys=0.00s Real=0.00s
[0.120s][info   ][gc,heap,exit   ] Heap
......
```
将部分打印内容省略了，可见`System.gc()`后内存占用由14M->0M，将对象这10M释放了。也就是JVM里面并没使用引用计数法来标记垃圾。
<a name="eSzDh"></a>
### 根可达算法
这个算法的基本思路就是通过一系列称为“GC Roots”的根对象作为起始节点集，从这些节点开始，根据引用关系向下搜索，搜索过程所走过的路径称为“引用链”（Reference Chain），如果某个对象到GC Roots间没有任何引用链相连，或者用图论的话来说就是从GC Roots到这个对象不可达时，则证明此对象是不可能再被使用的。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1656393881483-fef3258a-abd6-4528-a296-c108d88888c8.jpeg#clientId=uba2b4285-ab7d-4&from=paste&id=u5486a37b&originHeight=374&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uafa83260-e3c2-49d6-b5c0-7ffdf91aae5&title=)<br />在Java技术体系里面，固定可作为GC Roots的对象包括以下几种：

1. 在虚拟机栈（栈帧中的本地变量表）中引用的对象，譬如各个线程被调用的方法堆栈中使用到的<br />参数、局部变量、临时变量等在方法区中类静态属性引用的对象，譬如Java类的引用类型静态变量。
2. 在方法区中常量引用的对象，譬如字符串常量池（String Table）里的引用。
3. 在本地方法栈中JNI（即通常所说的Native方法）引用的对象。
4. Java虚拟机内部的引用，如基本数据类型对应的Class对象，一些常驻的异常对象（比如<br />NullPointExcepiton、OutOfMemoryError）等，还有系统类加载器。
5. 所有被同步锁（synchronized关键字）持有的对象。
6. 反映Java虚拟机内部情况的JMXBean、JVMTI中注册的回调、本地代码缓存等。
<a name="s7xRB"></a>
## 垃圾回收算法
本文介绍了常见的三种垃圾回收算法（mark-sweep,mark-compact,mark-copy），是java虚拟机各种垃圾收集器的算法基础。
<a name="ROIJi"></a>
### 垃圾回收算法思想
当前商业虚拟机的垃圾收集器，大多数都遵循了“分代收集”（Generational Collection）的理论进行设计，分代收集名为理论，实质是一套符合大多数程序运行实际情况的经验法则，它建立在两个分代假说之上：<br />1）弱分代假说（Weak Generational Hypothesis）：绝大多数对象都是朝生夕灭的。<br />2）强分代假说（Strong Generational Hypothesis）：熬过越多次垃圾收集过程的对象就越难以消亡。<br />这两个分代假说共同奠定了多款常用的垃圾收集器的一致的设计原则：收集器应该将Java堆划分出不同的区域，然后将回收对象依据其年龄（年龄即对象熬过垃圾收集过程的次数）分配到不同的区域之中存储显而易见，如果一个区域中大多数对象都是朝生夕灭，难以熬过垃圾收集过程的话，那么把它们集中放在一起，每次回收时只关注如何保留少量存活而不是去标记那些大量将要被回收的对象，就能以较低代价回收到大量的空间；<br />如果剩下的都是难以消亡的对象，那把它们集中放在一块，虚拟机便可以使用较低的频率来回收这个区域，这就同时兼顾了垃圾收集的时间开销和内存的空间有效利用。
<a name="JRQte"></a>
### 标记-清除算法 Mark-Sweep
算法分为“标记”和“清除”两个阶段：首先标记出所有需要回收的对象，在标记完成后，统一回收掉所有被标记的对象，也可以反过来，标记存活的对象，统一回收所有未被标记的对象。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1656393881355-a053af75-8d47-4c24-af6e-41d2cb299815.jpeg#clientId=uba2b4285-ab7d-4&from=paste&id=ud843779a&originHeight=515&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc6a87af8-2186-49f4-862f-eb2ae0c640f&title=)<br />它的主要缺点有两个：<br />第一个是执行效率不稳定，如果Java堆中包含大量对象，而且其中大部分是需要被回收的，这时必须进行大量标记和清除的动作，导致标记和清除两个过程的执行效率都随对象数量增长而降低；<br />第二个是内存空间的碎片化问题，标记、清除之后会产生大量不连续的内存碎片，空间碎片太多可能会导致当以后在程序运行过程中需要分配较大对象时无法找到足够的连续内存而不得不提前触发另一次垃圾收集动作。
<a name="VyWFx"></a>
### 标记-复制 Mark-Copy
标记-复制算法常被简称为复制算法它将可用内存按容量划分为大小相等的两块，每次只使用其中的一块。当这一块的内存用完了，就将还存活着的对象复制到另外一块上面，然后再把已使用过的内存空间一次清理掉。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1656393881519-4ca0b99d-9b06-42c3-abbb-f692b28039b4.jpeg#clientId=uba2b4285-ab7d-4&from=paste&id=u1cfee760&originHeight=495&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u06ad243b-1c31-4bcf-b464-90e5b6f5b23&title=)<br />如果内存中多数对象都是存活的，这种算法将会产生大量的内存间复制的开销，但对于多数对象都是可回收的情况，算法需要复制的就是占少数的存活对象，而且每次都是针对整个半区进行内存回收，分配内存时也就不用考虑有空间碎片的复杂情况，只要移动堆顶指针，按顺序分配即可。这样实现简单，运行高效，不过其缺陷也显而易见，这种复制回收算法的代价是将可用内存缩小为了原来的一半，空间浪费未免太多了。
<a name="TbrwX"></a>
### 标记-压缩 Mark-Compact
标记-复制算法在对象存活率较高时就要进行较多的复制操作，效率将会降低。更关键的是，如果不想浪费50%的空间，就需要有额外的空间进行分配担保，以应对被使用的内存中所有对象都100%存活的极端情况，所以在老年代一般不能直接选用这种算法。<br />标记-压缩算法其中的标记过程仍然与“标记-清除”算法一样，但后续步骤不是直接对可回收对象进行清理，而是让所有存活的对象都向内存空间一端移动，然后直接清理掉边界以外的内存:<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1656393881428-d3e89b75-bd43-48db-a2b3-0d5d3f9afafb.jpeg#clientId=uba2b4285-ab7d-4&from=paste&id=uaa5cefb6&originHeight=527&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u842d3924-aa37-44a6-8958-1bdaaaea1ba&title=)<br />标记-清除算法与标记-整理算法的本质差异在于前者是一种非移动式的回收算法，而后者是移动式的。是否移动回收后的存活对象是一项优缺点并存的风险决策：如果移动存活对象，尤其是在老年代这种每次回收都有大量对象存活区域，移动存活对象并更新所有引用这些对象的地方将会是一种极为负重的操作，而且这种对象移动操作必须全程暂停用户应用程序(STW问题)才能进行 。
<a name="g1F1x"></a>
## 垃圾处理器
基于上面的三种垃圾回收算法，衍生出7种垃圾回收器:
<a name="nh3Vk"></a>
### Serial收集器
这个收集器是一个单线程工作的收集器，但它的“单线程”的意义并不仅仅是说明它只会使用一个处理器或一条收集线程去完成垃圾收集工作，更重要的是强调在它进行垃圾收集时，必须暂停其他所有工作线程，直到它收集结束。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1656393881767-707ade55-1876-4ff9-87ab-5e6728d2eec6.jpeg#clientId=uba2b4285-ab7d-4&from=paste&id=ua3c64670&originHeight=222&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7af04269-45e1-4725-a51a-7f431b826bf&title=)<br />迄今为止，它依然是HotSpot虚拟机运行在客户端模式下的默认新生代收集器，有着优于其他收集器的地方，那就是简单而高效（与其他收集器的单线程相比），对于内存资源受限的环境，它是所有收集器里额外内存消耗（Memory Footprint） [1] 最小的；对于单核处理器或处理器核心数较少的环境来说，Serial收集器由于没有线程交互的开销，专心做垃圾收集自然可以获得最高的单线程收集效率。Serial收集器对于运行在客户端模式下的虚拟机来说是一个很好的选择。
<a name="IhnLg"></a>
### ParNew收集器
ParNew收集器实质上是Serial收集器的多线程并行版本，除了同时使用多条线程进行垃圾收集之<br />外，其余的行为包括Serial收集器可用的所有控制参数（例如：-XX：SurvivorRatio、-XX：<br />PretenureSizeThreshold、-XX：HandlePromotionFailure等）、收集算法、Stop The World、对象分配规<br />则、回收策略等都与Serial收集器完全一致，在实现上这两种收集器也共用了相当多的代码。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1656393881710-f97938ce-ac4c-474c-b81e-ed5cf2495384.jpeg#clientId=uba2b4285-ab7d-4&from=paste&id=uefb9f9f5&originHeight=201&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc1bc32ba-f902-4827-a2a1-3e363543ce2&title=)<br />ParNew收集器除了支持多线程并行收集之外，其他与Serial收集器相比并没有太多创新之处，但它<br />却是不少运行在服务端模式下的HotSpot虚拟机，尤其是JDK 7之前的遗留系统中首选的新生代收集<br />器，其中有一个与功能、性能无关但其实很重要的原因是：<br />除了Serial收集器外，目前只有它能与CMS<br />收集器配合工作，另一方面CMS的出现巩固了ParNew的地位<br />ParNew收集器在单核心处理器的环境中绝对不会有比Serial收集器更好的效果，甚至由于存在线程<br />交互的开销，该收集器在通过超线程（Hyper-Threading）技术实现的伪双核处理器环境中都不能百分之百保证超越Serial收集器。当然，随着可以被使用的处理器核心数量的增加，ParNew对于垃圾收集时<br />系统资源的高效利用还是很有好处的。
<a name="fSseU"></a>
### Parallel Scavenge收集器
Parallel Scavenge收集器也是一款新生代收集器，它同样是基于标记-复制算法实现的收集器，也是能够并行收集的多线程收集器……Parallel Scavenge的诸多特性从表面上看和ParNew非常相似，那它有什么特别之处呢？<br />Parallel Scavenge收集器的特点是它的关注点与其他收集器不同，CMS等收集器的关注点是尽可能地缩短垃圾收集时用户线程的停顿时间，而Parallel Scavenge收集器的目标则是达到一个可控制的吞吐量（Throughput）。所谓吞吐量就是处理器用于运行用户代码的时间与处理器总消耗时间的比值，即：<br />吞吐量=运行用户代码时间运行用户代码时间+运行垃圾收集器的时间吞吐量=运行用户代码时间运行用户代码时间+运行垃圾收集器的时间<br />如果虚拟机完成某个任务，用户代码加上垃圾收集总共耗费了100分钟，其中垃圾收集花掉1分钟，那吞吐量就是99%。停顿时间越短就越适合需要与用户交互或需要保证服务响应质量的程序，良好的响应速度能提升用户体验；而高吞吐量则可以最高效率地利用处理器资源，尽快完成程序的运算任务，主要适合在后台运算而不需要太多交互的分析任务。<br />由于与吞吐量关系密切，Parallel Scavenge收集器也经常被称作“吞吐量优先收集器”。
<a name="WxX3a"></a>
### Serial Old收集器
Serial Old是Serial收集器的老年代版本，它同样是一个单线程收集器，使用标记-整理算法。这个收集器的主要意义也是供客户端模式下的HotSpot虚拟机使用。如果在服务端模式下，它也可能有两种用途：一种是在JDK 5以及之前的版本中与Parallel Scavenge收集器搭配使用，另外一种就是作为CMS收集器发生失败时的后备预案，在并发收集发生Concurrent Mode Failure时使用。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1656393881777-6c1804a9-aad8-4f59-98db-891938cc392c.jpeg#clientId=uba2b4285-ab7d-4&from=paste&id=u57f374a2&originHeight=226&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u35c9cc79-405b-4889-b8c0-58a485b95a6&title=)
<a name="uNaji"></a>
### Parallel Old收集器
Parallel Old是Parallel Scavenge收集器的老年代版本，支持多线程并发收集，基于标记-整理算法实现。这个收集器是直到JDK 6时才开始提供的，在此之前，新生代的Parallel Scavenge收集器一直处于相当尴尬的状态，原因是如果新生代选择了Parallel Scavenge收集器，老年代除了Serial Old（PSMarkSweep）收集器以外别无选择，其他表现良好的老年代收集器，如CMS无法与它配合工作。<br />由于老年代Serial Old收集器在服务端应用性能上的“拖累”，使用Parallel Scavenge收集器也未必能在整体上获得吞吐量最大化的效果。<br />同样，由于单线程的老年代收集中无法充分利用服务器多处理器的并行处理能力，在老年代内存空间很大而且硬件规格比较高级的运行环境中，这种组合的总吞吐量甚至不一定比ParNew加CMS的组合来得优秀。直到Parallel Old收集器出现后，“吞吐量优先”收集器终于有了比较名副其实的搭配组合，在注重吞吐量或者处理器资源较为稀缺的场合，都可以优先考虑Parallel Scavenge加Parallel Old收集器这个组合。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1656393881921-4a2cc0c4-9125-40ec-b2b6-4369e02e14fa.jpeg#clientId=uba2b4285-ab7d-4&from=paste&id=u54101bb8&originHeight=235&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u33b9cf18-abb3-45b5-bb7f-4fdc0df027d&title=)
<a name="qsZZH"></a>
### CMS收集器
CMS（Concurrent Mark Sweep）收集器是一种以获取最短回收停顿时间为目标的收集器。目前很大一部分的Java应用集中在互联网网站或者基于浏览器的B/S系统的服务端上，这类应用通常都会较为关注服务的响应速度，希望系统停顿时间尽可能短，以给用户带来良好的交互体验。CMS收集器就非常符合这类应用的需求。<br />从名字（包含“Mark Sweep”）上就可以看出CMS收集器是基于标记-清除算法实现的，它的运作过程相对于前面几种收集器来说要更复杂一些，整个过程分为四个步骤，包括：<br />1）初始标记（CMS initial mark）<br />2）并发标记（CMS concurrent mark）<br />3）重新标记（CMS remark）<br />4）并发清除（CMS concurrent sweep）<br />其中初始标记、重新标记这两个步骤仍然需要“Stop The World”。初始标记仅仅只是标记一下GCRoots能直接关联到的对象，速度很快；并发标记阶段就是从GC Roots的直接关联对象开始遍历整个对象图的过程，这个过程耗时较长但是不需要停顿用户线程，可以与垃圾收集线程一起并发运行；而重新标记阶段则是为了修正并发标记期间，因用户程序继续运作而导致标记产生变动的那一部分对象的标记记录，这个阶段的停顿时间通常会比初始标记阶段稍长一些，但也远比并发标记阶段的时间短；最后是并发清除阶段，清理删除掉标记阶段判断的已经死亡的对象，由于不需要移动存活对象，所以这个阶段也是可以与用户线程同时并发的。由于在整个过程中耗时最长的并发标记和并发清除阶段中，垃圾收集器线程都可以与用户线程一起工作，所以从总体上来说，CMS收集器的内存回收过程是与用户线程一起并发执行的。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1656393881986-ca053135-e637-40fb-b6ba-78de14ccf611.jpeg#clientId=uba2b4285-ab7d-4&from=paste&id=u0d5cabcd&originHeight=202&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=udbc11d54-9573-43fb-8296-3b3c5a02e44&title=)<br />优点：并发收集、低停顿<br />缺点：1.对处理器资源非常敏感<br />2.无法处理“浮动垃圾”（Floating Garbage）<br />3.空间碎片
<a name="DwTuC"></a>
### Garbage First收集器
Garbage First（简称G1）收集器是垃圾收集器技术发展历史上的里程碑式的成果，它开创了收集器面向局部收集的设计思路和基于Region的内存布局形式。G1是一款主要面向服务端应用的垃圾收集器。<br />在G1收集器出现之前的所有其他收集器，包括CMS在内，垃圾收集的目标范围要么是整个新生代（Minor GC），要么就是整个老年代（Major GC），再要么就是整个Java堆（Full GC）。而G1跳出了这个樊笼，它可以面向堆内存任何部分来组成回收集（Collection Set，一般简称CSet）进行回收，衡量标准不再是它属于哪个分代，而是哪块内存中存放的垃圾数量最多，回收收益最大，这就是G1收集器的Mixed GC模式。G1开创的基于Region的堆内存布局是它能够实现这个目标的关键。虽然G1也仍是遵循分代收集理论设计的，但其堆内存的布局与其他收集器有非常明显的差异：G1不再坚持固定大小以及固定数量的分代区域划分，而是把连续的Java堆划分为多个大小相等的独立区域（Region），每一个Region都可以根据需要，扮演新生代的Eden空间、Survivor空间，或者老年代空间。收集器能够对扮演不同角色的Region采用不同的策略去处理，这样无论是新创建的对象还是已经存活了一段时间、熬过多次收集的旧对象都能获取很好的收集效果。<br />Region中还有一类特殊的Humongous区域，专门用来存储大对象。G1认为只要大小超过了一个Region容量一半的对象即可判定为大对象。每个Region的大小可以通过参数-XX：G1HeapRegionSize设定，取值范围为1MB～32MB，且应为2的N次幂。而对于那些超过了整个Region容量的超级大对象，将会被存放在N个连续的Humongous Region之中，G1的大多数行为都把Humongous Region作为老年代的一部分来进行看待，如图3-12所示。<br />虽然G1仍然保留新生代和老年代的概念，但新生代和老年代不再是固定的了，它们都是一系列区域（不需要连续）的动态集合。G1收集器之所以能建立可预测的停顿时间模型，是因为它将Region作为单次回收的最小单元，即每次收集到的内存空间都是Region大小的整数倍，这样可以有计划地避免在整个Java堆中进行全区域的垃圾收集。更具体的处理思路是让G1收集器去跟踪各个Region里面的垃圾堆积的“价值”大小，价值即回收所获得的空间大小以及回收所需时间的经验值，然后在后台维护一个优先级列表，每次根据用户设定允许的收集停顿时间（使用参数-XX：MaxGCPauseMillis指定，默认值是200毫秒），优先处理回收价值收益最大的那些Region，这也就是“Garbage First”名字的由来。这种使用Region划分内存空间，以及具有优先级的区域回收方式，保证了G1收集器在有限的时间内获取尽可能高的收集效率。
<a name="jqSVV"></a>
### 垃圾处理器总结
目前是新生代老年代垃圾回收器组合方式：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1656393882080-fdad5bd6-0a59-4577-9e4b-bc7d5250cc20.jpeg#clientId=uba2b4285-ab7d-4&from=paste&id=u40d4f0ec&originHeight=368&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u892ec98e-c661-40ea-9945-c01de827359&title=)
