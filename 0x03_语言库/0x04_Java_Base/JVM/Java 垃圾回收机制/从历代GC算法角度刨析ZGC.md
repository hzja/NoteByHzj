JavaJVMGC
<a name="ZcHPz"></a>
## 前言
本文所有介绍仅限于HotSpot虚拟机，先介绍了垃圾回收的必要手段，基于这些手段讲解了历代垃圾回收算法是如何工作的。<br />每一种算法不会讲的特别详细，只为读者从算法角度理解工作原理，从而引出ZGC，方便循序渐进地了解。<br />GC 是 Garbage Collection 的缩写，顾名思义垃圾回收机制，即当需要分配的内存空间不再使用的时候，JVM 将调用垃圾回收机制来回收内存空间。<br />那么 JVM 的垃圾机制是如何工作的呢？<br />第一步识别出哪些空间不再使用（识别并标记出哪些对象已死）；<br />第二步回收不再使用空间（清除已死对象 ）
<a name="DcJRn"></a>
## 判断对象是否已死
判断对象是否已死通常有两种方式，引用计数法和可达性分析法
<a name="rhcob"></a>
### 引用计数法
给对象中添加一个引用计数器，每当有一个地方引用它时，计数器值就加 1：当引用失效时，计数器值就减 1; 任何时刻计数器为 0 的对象就是不能再被使用的。<br />简单高效，但无法解决循环引用问题，a=b,b=a<br />引用计数法并没有在产品级的 JVM 中得到应用
<a name="Kw8Gk"></a>
### 可达性分析法
这个算法的基本思路就是通过一系列的称为 “GC Roots” 的对象作为起始点，从这些节点开始向下搜索，搜索所走过的路径称为引用链 ( Reference Chain)，当一个对象到 GC Roots 没有任何引用链相连 (用图论的话来说，就是从 GC Roots 到这个对象不可达) 时，则证明此对象是不可用的。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1681204408035-6c1d9f3b-4114-4f35-b35e-5d8346f78b25.png#averageHue=%23f2e7dc&clientId=u31f33da4-db95-4&from=paste&id=u4230744f&originHeight=560&originWidth=937&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u62eece85-3c19-4384-bfd5-7364a6cc30d&title=)<br />不过可达性算法中的对象并不是立即死亡的，对象拥有一次自我拯救的机会，对象被系统宣告死亡至少要经历两次标记过程，第一次是经过可达性分析之后没有与 GC Roots 相连的引用链，第二次是在由虚拟机自动建立的 Finalize 队列中判断是否需要执行 `finalize()` 方法。<br />HotSopt 虚拟机采用该算法。
<a name="cW6ej"></a>
## 清除已死对象的方式
<a name="KdtLR"></a>
### 标记清除算法
先标记再清除<br />不足：1 效率问题，标记和清除效率都不高。2 空间问题，产生大量空间碎片
<a name="HIsAT"></a>
### 复制算法
内存分两块，A,B<br />A 用完了，将存活对象拷贝到 B,A 清理掉<br />代价：内存少了一半。<br />HotSopt 虚拟机用此算法回收新生代。将新生代内存划分为 8：1：1 的 Eden 和 Survivor 解决复制算法内存使用率低的问题<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1681204408078-68278ea1-79de-498a-98e6-236a43cbead7.png#averageHue=%23e8dbce&clientId=u31f33da4-db95-4&from=paste&id=u91c9bdd9&originHeight=325&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u34965bb2-e539-443b-9ec2-cb436961e77&title=)
<a name="VkpPp"></a>
### 标记整理算法
老年代使用，方式和标记清除类似，只是不直接清除，而是将后续对象向一端移动，并清理掉边界以外的内存。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1681204408121-d26b2e90-b5cc-4aaa-bc84-8d709be53f6a.png#averageHue=%23eacaa7&clientId=u31f33da4-db95-4&from=paste&id=uc8e1cae6&originHeight=548&originWidth=686&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u445285f6-c034-496e-bad8-c38cf7d998c&title=)
<a name="IstHB"></a>
### 分代收集算法
分代收集是一个算法方案，整合了以上算法的优点，一般是把 Java 堆分为新生代和老年代，在新生代中，使用复制算法老年代 “标记一清理” 或者 “标记一整理”
<a name="mc9Qd"></a>
## 历代垃圾收集器简介
通过上文了解了怎样识别垃圾，怎样清理垃圾，接下来，讲 ZGC 之前，回顾一下历代垃圾回收是怎样做的，主要是想给读者一种历史的视角，任何技术都不是凭空产生的，更多的是在前人成果之上进行优化整合<br />先看一个历代 JDK 垃圾收集器对比表格，以下表格着重说明或引出几个问题：<br />1 CMS 从来未被当作默认 GC，且已废弃<br />2 CMS 的思想其实部分被 ZGC 吸收，CMS 已死，但他的魂还在<br />3 JDK11、JDK17 为长期迭代版本，项目中应优先使用这两个版本

| 版本 | 发布时间 | 默认收集器 | 事件 |
| --- | --- | --- | --- |
| jdk1.3 | 2000-05-08 | serial | <br /> |
| jdk1.4 | 2004-02-06 | ParNew | <br /> |
| jdk1.5/5.0 | 2004-09-30 | Parallel Scavenge/serial | CMS 登场 |
| jdk1.6/6.0 | 2006-12-11 | Parallel Scavenge/Parallel Old | <br /> |
| dk1.7/7.0 | 2011-07-28 | Parallel Scavenge/Parallel Old | G1 登场 |
| jdk1.8/8.0 | 2014-03-18 | Parallel Scavenge/Parallel Old | <br /> |
| jdk1.9/9.0 | 2014-09-8 | G1 | CMS 废弃 |
| jdk10 | 2018-03-21 | G1 | <br /> |
| jdk11 | 2018-09-25 | G1 | ZGC 登场 |
| jdk12 | 2019-3 | G1 | Shenandoah |
| jdk13 | 2019-9 | G1 | <br /> |
| jdk14 | 2020-3 | G1 | CMS 移除 |
| jdk15 | 2020-9-15 | G1 | ZGC、Shenandoah 转正 |
| jdk16 | 2021-3-16 | G1 | <br /> |
| jdk17 | 2021-09-14 | G1 | ZGC 分代 |
| jdk18 | 2022-3-22 | G1 | <br /> |
| jdk19 | 2022-9-22 | G1 | <br /> |

<a name="Buj5X"></a>
### GC 分类
经常在各种场景听到以下几种 GC 名词，Young GC、Old GC、Mixed GC、Full GC、Major GC、Minor GC，他们到底什么意思，进行了以下梳理：<br />首先 GC 分两类，Partial GC（部分回收），Full GC<br />Partial GC：并不收集整个 GC 堆的模式，以下全是 Partial GC 的子集<br />Young GC：只收集 young gen 的 GC<br />Old GC：只收集 old gen 的 GC。只有 CMS 的 concurrent collection 是这个模式<br />Mixed GC：只有 G1 有这个模式，收集整个 young gen 以及部分 old gen 的 GC。<br />Minor GC：只有 G1 有这个模式，收集整个 young gen<br />Full GC：收集整个堆，包括 young gen、old gen、perm gen（如果存在的话）等所有部分的模式。<br />Major GC：通常是跟 full GC 是等价的
<a name="krNPL"></a>
### serial 收集器
单线程收集器，“单线程” 的意义并不仅仅说明它只会使用一个 CPU 或一条收集线程去完成垃圾收集工作，更重要的是在它进行垃圾收集时，必须**暂停其他所有的工作线程**，直到它收集结束。它依然是虚拟机运行在 Client 模式下的默认新生代收集器。它也有着优于其他收集器的地方：简单而高效 (与其他收集器的单线程比)，对于限定单个 CPU 的环境来说，Serial I 收集器由于没有线程交互的开销，专心做垃圾收集自然可以获得最高的单线程收集效率。<br />下图彩色部分说明了它的算法，简单粗暴

1. 停止用户线程
2. 单线程垃圾回收新生代
3. 重启用户线程

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1681204408059-e5beca98-64cd-4943-b54b-d37f5ecdb78f.png#averageHue=%23f4ebe3&clientId=u31f33da4-db95-4&from=paste&id=u727bbb23&originHeight=373&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=udd074fed-9cf7-4a25-9a0c-1298ba98d0a&title=)
<a name="ygHBE"></a>
### ParNew 收集器
Parnew 收集器其实就是 Serial l 收集器的多线程版本。它是许多运行在 Server 模式下的虚拟机中首选的新生代收集器，其中有一个与性能无关但很重要的原因是，除了 Serial 收集器外，目前只有它能与 CMS 收集器配合工作。Pardew 收集器在单 CPU 的环境中绝对不会有比 Serial 收集器更好的效果。它默认开启的收集线程数与 CPU 的数量相同，在 CPU 非常多 (臂如 32 个) 的环境下，可以使用 `-XX:ParallelGCThreads` 参数来限制垃圾收集的线程数。<br />ParNew 收集器**追求降低 GC 时用户线程的停顿时间，**适合交互式应用，良好的反应速度提升用户体验.<br />下图彩色部分说明了它的算法，同样简单粗暴

1. 停止用户线程
2. 多线程垃圾回收新生代
3. 重启用户线程

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1681204408056-7620617d-b5aa-44e9-aa67-28dd544c5bf3.png#averageHue=%23f3eae1&clientId=u31f33da4-db95-4&from=paste&id=udf8c4c1a&originHeight=355&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u1c689dc9-3c63-4ee5-849e-588b912bfe4&title=)
<a name="JOuYK"></a>
### Parallel Scavenge 收集器
Parallel Scavenge 收集器是一个新生代收集器，它也是使用复制算法的收集器，又是并行的多线程收集器。算法的角度它和 ParNew 一样，在此就不画图解释了<br />Parallel Scavenge 收集器的目标则是达到**一个可控制的吞吐量** ( Throughput)<br />_吞吐量是指用户线程运行时间占 CPU 总时间的比例_<br />通过以下两种方式可达到目的：

1. 在多 CPU 环境中使用多条 GC 线程，从而垃圾回收的时间减少，从而用户线程停顿的时间也减少；
2. 实现 GC 线程与用户线程并发执行。
<a name="uYZIR"></a>
### Serial Old 收集器
Serial Old 是 Serial 收集器的老年代版本，它同样是一个单线程收集器，使用 “标记整理” 算法。这个收集器的主要意义也是在于给 Client 模式下的虚拟机使用。<br />如果在 Server 模式下，那么它主要还有两大用途：<br />一种用途是在 JDK1.5 以及之前的版本中与 ParallelScavenge 收集器搭配使用，<br />另一种用途就是作为 CMS 收集器的后备预案，在并发收集发生 Concurrent Mode Failure 时使用<br />下图彩色部分说明了它的算法，同样简单粗暴

1. 停止用户线程
2. 单线程垃圾回收老年代
3. 重启用户线程

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1681204408538-d08f5c37-4b02-4ec3-b8e4-700f759f2695.png#averageHue=%23f4ede5&clientId=u31f33da4-db95-4&from=paste&id=u9fdf1b93&originHeight=346&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u09ec514e-0b86-4b74-973f-54476687044&title=)
<a name="N5H2n"></a>
### Parallel Old 收集器
Paralle Old 是 Parallel Scavenge 收集器的老年代版本，一般它们搭配使用，追求 CPU 吞吐量，使用多线程和 “标记一整理” 算法。<br />下图彩色部分说明了它的算法，同样简单粗暴

1. 停止用户线程
2. 多线程垃圾回收老年代
3. 重启用户线程

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1681204408729-beff7896-61ab-4d62-8a27-421ae5188bec.png#averageHue=%23eee6dc&clientId=u31f33da4-db95-4&from=paste&id=ub5c8c2e0&originHeight=376&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u591aea68-70f5-4407-a5a7-d84cdca131f&title=)
<a name="G6CsJ"></a>
### CMS 收集器
以上 5 种垃圾回收原理不难理解，算法之所以如此简单个人理解在当时使用这种算法就够了，随着 JAVA 的攻城略地，有一种垃圾回收需求出现，即使用尽量短的回收停顿时间，以避免过久的影响用户线程，CMS 登场了。<br />CMS (Concurrent Mark Sweep) 收集器是一种以获取最短回收停顿时间为目标的收集器。<br />想要达到目的，就要分析 GC 时最占用时间的是什么操作，比较浪费时间的是标记已死对象、清除对象，那么如果可以和用户线程并发的进行，GC 的停顿基本就限制在了标记所花费的时间。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1681204408799-0af62aac-236d-4ece-940e-3ccd79869a44.png#averageHue=%23f7e9da&clientId=u31f33da4-db95-4&from=paste&id=uab45d434&originHeight=435&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub2f6b202-bb4b-40a7-89be-8ae052f2de3&title=)<br />如上图，CMS 收集器是基于 “标记一清除” 法实现的，它的运作过程分为 4 个步骤

- 初始标记 (EMS initial mark) stop the world
- 并发标记 (CMS concurrent mark)
- 重新标记 (CMS remark) stop the world
- 并发清除 (CMS concurrent sweep)

初始标记的作用是查找 GC Roots 集合的过程，这个过程处理对象相对较少，速度很快。(为什么要进行初始标记：枚举根结点。)<br />并发标记是实际标记所有对象是否已死的过程，比较耗时，所以采用并发的方式。<br />重新标记主要是处理并发标记期间所产生的新的垃圾。重新标记阶段不需要再重新标记所有对象，只对并发标记阶段改动过的对象做标记即可。<br />**优点：**<br />并发收集、低停顿<br />**缺点：**<br />CMS 收集器对 CPU 资源非常敏感。<br />CMS 收集器无法处理浮动垃圾 (Floating Garbage)，可能出现 “Concurrent ModeFailure” 失败而导致另一次 Full GC 的产生。<br />“标记一清除” 法导致大量空间碎片产生，以至于老年代还有大量空间，却没有整块空间存储某对象。<br />Concurrent ModeFailure可能原因及方案<br />原因1：CMS触发太晚<br />方案：将`-XX:CMSInitiatingOccupancyFraction=N`调小 (达到百分比进行垃圾回收)；<br />原因2：空间碎片太多<br />方案：开启空间碎片整理，并将空间碎片整理周期设置在合理范围；<br />`-XX:+UseCMSCompactAtFullCollection` （空间碎片整理）<br />`-XX:CMSFullGCsBeforeCompaction=n`<br />原因3：垃圾产生速度超过清理速度<br />晋升阈值过小；<br />Survivor空间过小，导致溢出；<br />Eden区过小，导致晋升速率提高；存在大对象；
<a name="sKzfN"></a>
## G1 收集器
G1 是一款面向服务端应用的垃圾收集器。下文会简单讲解一下它的 “特点” 和 “内存分配与回收策略”，有基础或不感兴趣的同学直接跳到 “G1 垃圾回收流程”
<a name="CHjDr"></a>
### 特点
<a name="ikWqq"></a>
#### 并行与并发
G1 能充分利用多 CPU、多核环境下的硬件优势，使用多个 CPU (CPU 或者 CPU 核心) 来缩短 Stop-The- World 停顿的时间，部分其他收集器原本需要停顿 Java 线程执行的 GC 动作，G1 收集器仍然可以通过并发的方式让 Java 程序继续执行。
<a name="rvxTS"></a>
#### 分代收集
与其他收集器一样，分代概念在 G1 中依然得以保留。虽然 G1 可以不需要其他收集器配合就能独立管理整个 GC 堆，但它能够采用不同的方式去处理新创建的对象和已经存活了一段时间、熬过多次 GC 的旧对象以获取更好的收集效果。
<a name="q8Fr9"></a>
#### 空间整合
与 CMS 的 “标记一清理” 算法不同，G1 从整体来看是基于 “标记一整理” 算法实现的收集器，从局部 (两个 Region 之间) 上来看是基于 “复制” 算法实现的，但无论如何，这两种算法都意味着 G1 运作期间不会产生内存空间碎片，收集后能提供规整的可用内存。这种特性有利于程序长时间运行，分配大对象时不会因为无法找到连续内存空间而提前触发下一次 GC。
<a name="K9Kb6"></a>
#### 可预测的停顿
这是 G1 相对于 CMS 的另一大优势，降低停顿时间是 G1 和 CMS 共同的关注点，但 G1 除了追求低停顿外，还能建立可预测的停顿时间模型，能让使用者明确指定在一个长度为 M 毫秒的时间片段内，消耗在垃圾收集上的时间不得超过 N 毫秒，这几乎已经是实时 Java (RTSJ) 的垃圾收集器的特征了。<br />在 G1 之前的其他收集器进行收集的范围都是整个新生代或者老年代，而 G1 不再是这样。使用 G1 收集器时，Java 堆的内存布局就与其他收集器有很大差别，它将整个 Java 堆划分为多个大小相等的独立区域 (Region)，虽然还保留有新生代和老年代的概念，但新生代和老年代不再是物理隔离的了，它们都是一部分 Region (不需要连续) 的集合
<a name="jpMoj"></a>
### **内存分配与回收策略**
<a name="sWQf9"></a>
#### 对象优先在 Eden 分配
大多数情况下，对象在新生代 Eden 区中分配。当 Eden 区没有足够空间进行分配时，虚拟机将发起一次 Minor[ˈmaɪnə(r)] GC
<a name="H9gJm"></a>
#### 大对象直接进入老年代
所谓的大对象是指，需要大量连续内存空间的 Java 对象，最典型的大对象就是那种很长的字符串以及数组。大对象对虚拟机的内存分配来说就是一个坏消息 (比遇到一个大对象更加坏的消息就是遇到一群 “朝生夕灭” 的 “短命大对象” 写程序的时候应当避免)，经常出现大对象容易导致内存还有不少空间时就提前触发垃圾收集以获取足够的连续空间来 “安置” 它们。
<a name="sofRk"></a>
#### 长期存活的对象将进入老年代
虚拟机给每个对象定义了一个对象年龄 (Age) 计数器。如果对象在 Eden 出生并经过第一次 Minor GC 后仍然存活，并且能被 Survivor 容纳的话，将被移动到 Survivor 空间中，并且对象年龄设为 1。对象在 Survivor 区中每 “熬过” 一次 Minor GC，年龄就增加 1 岁，当它的年龄增加到一定程度（默认 15 岁）会被晋升到老年代中。对象晋升老年代的年龄阈值，可以通过参数据 `-XX:MaxTenuringThreshold` 设置
<a name="lMO5X"></a>
#### 动态对象年龄判定
为了能更好地适应不同程序的内存状况，虚拟机并不是水远地要求对象的年龄必须达到了 MaxTenuringThreshold 才能晋升老年代，如果在 Survivor 空间中相同年龄所有对象大小的总和大于 Survivor 空间的一半，年龄大于或等于该年龄的对象就可以直接进入老年代，无须等到 MaxTenuringThreshold 中要求的年龄。
<a name="kQejf"></a>
#### 空间分配担保
在发生 Minor GC 之前，虚拟机会先检査老年代最大可用的连续空间是否大于新生代所有对象总空间，如果这个条件成立，那么 Minor GC 可以确保是安全的。如果不成立，则虚拟机会查看 HandlePromotionFailure 设置值是否允许担保失败。如果允许，那么会继续检查老年代最大可用的连续空间是否大于历次晋升到老年代对象的平均大小，如果大于，将尝试着进行一次 Minor GC，尽管这次 Minor GC 是有风险的；如果小于，或者 HandlePromotionFailure 设置不允许冒险，那这时也要改为进行一次 Full GC.<br />为什么要担保：<br />Minor GC 后还有大量对象存活且空间不够存放新对象，就要直接在老年代存放<br />为什么是历次晋升到老年代对象的平均大小：<br />取平均值进行比较其实仍然是一种动态概率的手段，也就是说，如果某次 Minor GCd 存活后的对象突增，远远高于平均值的话，依然会导致担保失败 (HandlePromotionFailure) 如果出现了 HandlePromotionFailure 失败，那就只好在失败后重新发起一次 Full GC。虽然担保失败时绕的子是最大的，但大部分情况下都还是会将 HandlePromotionFailure 开关打开，避免 Full GC 过于频繁。<br />_eden 的大小范围默认是 =【-XX:G1NewSizePercent，-XX:G1MaxNewSizePercent】=【整堆 5%，整堆 60%】_<br />_humongous 如果一个对象的大小已经超过 Region 大小的 50% 了，那么就会被放入大对象专门的 Region 中，这种 Region 叫 humongous_
<a name="Kg20z"></a>
### G1 垃圾回收流程
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1681204432687-b4d3237d-befd-4e7e-8070-0290c40ff9fe.png#averageHue=%23f9e7df&clientId=u31f33da4-db95-4&from=paste&id=u160233c3&originHeight=543&originWidth=635&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5f0168a8-0084-403f-93d4-2d9e5341c0c&title=)<br />网上对 G1 的回收阶段有不同的说法，参考 Oracle JVM 工程师的一个说法：<br />他把整个 G1 的垃圾回收阶段分成了这么三个，第一个叫 Minor GC，就是对新生代的垃圾收集，第二个阶段呢叫 Minor GC + Concurrent Mark，就是新生代的垃圾收集同时呢会执行一些并发的标记，这是第二个阶段，第三个阶段呢它叫 Mixed GC 混合收集，这三个阶段是一个循环的过程。刚开始是这个新生代的垃圾收集，经过一段时间，当老年代的内存超过一个阈值了，它会在新生代垃圾收集的同时进行并发的标记，等这个阶段完成了以后，它会进行一个混合收集，混合收集就是会对新生代、幸存区还有老年代都来进行一个规模较大的一次收集，等内存释放掉了，混合收集结束。这时候伊甸园的内存都被释放掉，它会再次进入新生代的一个垃圾收集过程，那先来看看这个新生代的收集 Minor GC。
<a name="Vn4xG"></a>
#### Minor GC 的回收过程（eden 满了回收）
选定所有 Eden Region 放入 CSet，使用多线程复制算法将 CSet 的存活对象复制到 Survivor Region 或者晋升到 Old Region。<br />下图分 7 步演示了这个过程

1. 初始状态，堆无占用
2. Eden Region 满了进行标记
3. 将存活对象复制到 Survivor Region
4. 清理 Eden Region
5. Eden Region 又满了进行再次标记，此时会连带 Survivor Region 一起标记
6. 将存活对象复制到另一个 Survivor Region
7. 再次清理 Eden Region 和被标记过的 Survivor Region

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1681204432765-613a5f8c-134c-42c3-840d-513c11e21c2c.png#averageHue=%23dbd6d0&clientId=u31f33da4-db95-4&from=paste&id=u70800fcb&originHeight=784&originWidth=815&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ucfa75c3c-69bb-4581-827e-8c00a329663&title=)<br />Minor GC 结束后自动进行并发标记，为以后可能的 Mixed GC 做准备
<a name="oF5AU"></a>
#### Mixed GC 的回收过程（专注垃圾最多的分区）
选定所有 Eden Region 和全局并发标记计算得到的收益较高的部分 Old Region 放入 CSet，使用多线程复制算法将 CSet 的存活对象复制到 Survivor Region 或者晋升到 Old Region。<br />当堆空间的占用率达到一定阈值后会触发 Mixed GC（默认 45%，由参数决定）<br />Mixed GC 它一定会回收年轻代，并会采集部分老年代的 Region 进行回收的，所以它是一个 “混合” GC。<br />下图分 3 步演示了这个过程

1. 并发标记所有 Region
2. 并发复制
3. 并发清理

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1681204432680-29bdf7a0-d86c-4f0b-a3d7-4067a80367f7.png#averageHue=%23ccc6bf&clientId=u31f33da4-db95-4&from=paste&id=u6b61d872&originHeight=357&originWidth=1031&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud1ab0b45-5c79-4e9f-9651-ca9bba2e387&title=)
<a name="T8MkT"></a>
## ZGC
ZGC（Z Garbage Collector） 是一款性能比 G1 更加优秀的垃圾收集器。ZGC 第一次出现是在 JDK 11 中以实验性的特性引入，这也是 JDK 11 中最大的亮点。在 JDK 15 中 ZGC 不再是实验功能，可以正式投入生产使用了。
<a name="uTkBx"></a>
### 目标低延迟

- 保证最大停顿时间在几毫秒之内，不管堆多大或者存活的对象有多少。
- 可以处理 8MB-16TB 的堆

通过以上历代垃圾回收器的讲解，大致了解到减少延迟的底层思想不外乎将 stop the world 进行极限压缩，将能并行的部分全部采用和用户线程并行的方式处理，然而 ZGC 更 "过分" 它甚至把一分部垃圾回收的工作交给了用户线程去做，那么它是怎么做到的呢？ZGC 的标记和清理工作同 CMS、G1 大致差不多，仔细看下图的过程，和 CMS 特别像，这就是在上文说的 CMS 其实并没有真正被抛弃，它的部分思想在 ZGC 有发扬。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1681204432673-088f5ede-4f73-43e1-b184-856c7846df42.png#averageHue=%23f7eadd&clientId=u31f33da4-db95-4&from=paste&id=uc372fe89&originHeight=328&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9e7fcea7-3dda-49ba-a4ca-b2c617fb6b4&title=)<br />ZGC 的步骤大致可分为三大阶段分别是标记、转移、重定位。<br />标记：从根开始标记所有存活对象<br />转移：选择部分活跃对象转移到新的内存空间上<br />重定位：因为对象地址变了，所以之前指向老对象的指针都要换到新对象地址上。<br />并且这三个阶段都是并发的。<br />初始转移需要扫描 GC Roots 直接引用的对象并进行转移，这个过程需要 STW，STW 时间跟 GC Roots 成正比。<br />并发转移准备 ：分析最有回收价值 GC 分页（无 STW） 初始转移应对初始标记的数据<br />并发转移应对并发标记的数据<br />除了标记清理过程继承了 CMS 和 G1 的思想，ZGC 要做了以下优化
<a name="zSIRA"></a>
### 并发清理（转移对象）
在 CMS 和 G1 中都用到了写屏障，而 ZGC 用到了读屏障。<br />写屏障是在对象引用赋值时候的 AOP，而读屏障是在读取引用时的 AOP。<br />比如 Object a = obj.foo;，这个过程就会触发读屏障。<br />也正是用了读屏障，ZGC 可以并发转移对象，而 G1 用的是写屏障，所以转移对象时候只能 STW。<br />简单的说就是 GC 线程转移对象之后，应用线程读取对象时，可以利用读屏障通过指针上的标志来判断对象是否被转移。<br />读屏障会对应用程序的性能有一定影响，据测试，对性能的最高影响达到 4%，但提高了 GC 并发能力，降低了 STW。这就是上面所说的 ZGC “过分” 地将部分垃圾回收工作交给用户线程的原因。
<a name="m2iLs"></a>
### 染色指针
染色指针其实就是从 64 位的指针中，拿几位来标识对象此时的情况，分别表示 Marked0、Marked1、Remapped、Finalizable。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1681204432663-c1ea1aa1-b980-4d9a-8daa-59fcda0e0896.png#averageHue=%23e7d0b9&clientId=u31f33da4-db95-4&from=paste&id=ubdfc999e&originHeight=307&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u77191b4d-db7d-4e2b-a806-be5fac4e653&title=)<br />0-41 这 42 位就是正常的地址，所以说 ZGC 最大支持 4TB (理论上可以 16TB) 的内存，因为就 42 位用来表示地址<br />也因此 ZGC 不支持 32 位指针，也不支持指针压缩。<br />其实对象只需要两个状态 Marked，Remapped，对象被标记了，对象被重新映射了，为什么会有 M0，M1，用来区分本次 GC 标记和上次 GC 标记<br />以下是标记转移算法说明：

1. 在垃圾回收开始前：Remapped
2. 标记过程：

标记线程访问<br />发现对象地址视图是 Remapped 这时候将指针标记为 M0<br />发现对象地址视图是 M0，则说明这个对象是标记开始之后新分配的或者已经标记过的对象，所以无需处理<br />应用线程<br />如果创建新对象，则将其地址视图置为 M0

3. 标记阶段结束后

ZGC 会使用一个对象活跃表来存储这些对象地址，此时活跃的对象地址视图是 M0

4. 并发转移阶段

转移线程：<br />转移成功后对象地址视图被置为 Remapped（也就是说 GC 线程如果访问到对象，此时对象地址视图是 M0，并且存在或活跃表中，则将其转移，并将地址视图置为 Remapped ）<br />如果在活跃表中，但是地址视图已经是 Remapped 说明已经被转移了，不做处理。<br />应用线程：<br />如果创建新对象，地址视图会设为 Remapped

5. 下次标记使用 M1

M1 标识本次垃圾回收中活跃的对象<br />M0 是上一次回收被标记的对象，但是没有被转移，且在本次回收中也没有被标记活跃的对象。<br />下图展示了 Marked，Remapped 的过程，<br />初始化时 A，B，C 三个对象处于 Remapped 状态<br />第一次 GC，A 被转移，B 未被转移，C 无引用将被回收<br />第二次 GC，由于 A 被转移过了（Remapped 状态），所以被标记 M1，此时恰好 B 为不活跃对象，将被清理<br />第三次 GC，A 又被标记成 M0<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1681204433223-149b1cc9-70a3-41e2-8151-9dfedb13ce9b.png#averageHue=%23f4efeb&clientId=u31f33da4-db95-4&from=paste&id=ud0ebd8d1&originHeight=482&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u646a60c6-f3b3-41d2-a5a1-f1805caf3b5&title=)
<a name="d80at"></a>
### 多重映射
Marked0、Marked1 和 Remapped 三个视图<br />ZGC 为了能高效、灵活地管理内存，实现了两级内存管理：虚拟内存和物理内存，并且实现了物理内存和虚拟内存的映射关系 在 ZGC 中这三个空间在同一时间点有且仅有一个空间有效，利用虚拟空间换时间，这三个空间的切换是由垃圾回收的不同阶段触发的，通过限定三个空间在同一时间点有且仅有一个空间有效高效的完成 GC 过程的并发操作<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1681204433296-f6fce4ba-21ff-4f8b-959e-b61690518d0b.png#averageHue=%23f9f6f4&clientId=u31f33da4-db95-4&from=paste&id=u084c2d90&originHeight=545&originWidth=935&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ufba68439-1516-4a19-8248-c39420590f8&title=)
<a name="XXywK"></a>
### 支持 NUMA
NUMA 是非一致内存访问的缩写 （Non-Uniform Memory Access，NUMA）<br />早年如下图：SMP 架构 （Symmetric Multi-Processor），因为任一个 CPU 对内存的访问速度是一致的，不用考虑不同内存地址之间的差异，所以也称一致内存访问（Uniform Memory Access， UMA ）。这个核心越加越多，渐渐的总线和北桥就成为瓶颈，那不能够啊，于是就想了个办法。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1681204433542-9b9f4232-2bdd-433e-9e3d-520b6afb2e0d.png#averageHue=%23e2dbd4&clientId=u31f33da4-db95-4&from=paste&id=u9f5b50b3&originHeight=554&originWidth=894&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u155f393a-6dbd-4fe9-8034-cefebc62854&title=)<br />把 CPU 和内存集成到一个单元上，这个就是非一致内存访问 （Non-Uniform Memory Access，NUMA）。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1681204433591-52280611-c75e-40e7-b7cf-e994e95d507e.png#averageHue=%23d4c6b8&clientId=u31f33da4-db95-4&from=paste&id=u6bf35401&originHeight=456&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua56b17e8-73b7-4a68-8ec3-d3d7b159078&title=)<br />ZGC 对 NUMA 的支持是小分区分配时会优先从本地内存分配，如果本地内存不足则从远程内存分配。
<a name="uu6zG"></a>
### ZGC 优劣
综上分析，ZGC 在战略上沿用了上几代 GC 的算法策略，采用并发标记，并发清理的思路，在战术上，通过染色指针、多重映射，读屏障等优化达到更理想的并发清理，通过支持 NUMA 达到了更快的内存操作。但 ZGC 同样不是银弹，它也有自身的优缺点，如下
<a name="DfJU3"></a>
#### 优势：
1、一旦某个 Region 的存活对象被移走之后，这个 Region 立即就能够被释放和重用掉，而不必等待整个堆中所有指向该 Region 的引用都被修正后才能清理，这使得理论上只要还有一个空闲 Region，ZGC 就能完成收集。<br />2、颜色指针可以大幅减少在垃圾收集过程中内存屏障的使用数量，ZGC 只使用了读屏障。<br />3、颜色指针具备强大的扩展性，它可以作为一种可扩展的存储结构用来记录更多与对象标记、重定位过程相关的数据，以便日后进一步提高性能。
<a name="TTtF9"></a>
#### 劣势：
1、它能承受的对象分配速率不会太高<br />ZGC 准备要对一个很大的堆做一次完整的并发收集。在这段时间里面，由于应用的对象分配速率很高，将创造大量的新对象，这些新对象很难进入当次收集的标记范围，通常就只能全部当作存活对象来看待 —— 尽管其中绝大部分对象都是朝生夕灭的，这就产生了大量的浮动垃圾。如果这种高速分配持续维持的话，每一次完整的并发收集周期都会很长，回收到的内存空间持续小于期间并发产生的浮动垃圾所占的空间，堆中剩余可腾挪的空间就越来越小了。目前唯一的办法就是尽可能地增加堆容量大小，获得更多喘息的时间。<br />2、吞吐量低于 G1 GC<br />一般来说，可能会下降 5%-15%。对于堆越小，这个效应越明显，堆非常大的时候，比如 100G，其他 GC 可能一次 Major 或 Full GC 要几十秒以上，但是对于 ZGC 不需要那么大暂停。这种细粒度的优化带来的副作用就是，把很多环节其他 GC 里的 STW 整体处理，拆碎了，放到了更大时间范围内里去跟业务线程并发执行，甚至会直接让业务线程帮忙做一些 GC 的操作，从而降低了业务线程的处理能力。
<a name="nzedc"></a>
## 总结
综上，其实 ZGC 并不是一个凭空冒出的全新垃圾回收，它结合前几代 GC 的思想，同时在战术上做了优化以达到极限的 STW，ZGC 的优秀表现有可能会改变未来程序编写方式，站在垃圾收集器的角度，垃圾收集器特别喜欢不可变对象，原有编程方式鉴于内存、GC 能力所限使用可变对象来复用对象而不是销毁重建，试想如果有了 ZGC 的强大回收能力的加持，是不是就可以无脑的使用不可变对象进行代码编写。
