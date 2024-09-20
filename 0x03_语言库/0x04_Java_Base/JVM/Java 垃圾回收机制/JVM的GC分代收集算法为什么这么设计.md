JVM
<a name="AXowe"></a>
## JVM 的垃圾回收机制
先来说说这个回收机制的算法都有哪些，如图所示。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658191233524-46b5135a-38d4-44e1-bedc-6ae5faf1b178.jpeg)<br />目前面试比较常问的垃圾回收算法就是这几种，分开来说，最后说说分代收集为什么选择不同的算法来实现。
<a name="kGYUC"></a>
### 标记清除算法 Mark-Sweep
都知道，标记清除算法，是垃圾回收算法当中算是最基础的算法了，因为标记算法就只有两个阶段，<br />阶段一 标记<br />阶段二 清除<br />标记的是什么内容呢？<br />标记的都是所有的**需要被回收**的对象，当执行到清除阶段的时候，就会直接把这些标记的对象给完整的清除掉。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658190909952-ace187d6-5c87-4aec-81be-e0c66c55c86d.png#clientId=u34863382-a2d1-4&from=paste&id=ua813dd0c&originHeight=583&originWidth=1010&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uedfac590-d444-449a-b80b-2e06f3aa063&title=)<br />如果是这样的话，那么就会出现了一个问题，大家看，如果灰色的是内存空间，然后把需要把被回收的对象清除的话，不能保证这个被回收的对象，一定会是连续排在一起的，就比如所有需要被回收的对象，都排在最上面的内存空间中，这个是不太可能的，所以，执行完清除之后，这些未使用的内存空间，就成了一个**不连续**的内存空间。<br />标记清除算法，最大的弊端出现了，碎片化就非常的严重，如果有大对象想要存入，而内存中出现没有连续空间的话，那他就没有可用空间保存了。<br />为了解决碎片化严重的这种情况，就有了下面的这种垃圾回收算法。
<a name="eJ5oz"></a>
### 复制算法（copying）
为了解决这个内存碎片化严重的问题，按内存容量将内存划分为等大小的两块。每次只使用其中一块，当这一块内存满后将尚存活的对象复制到另一块上去，把已使用的内存清掉。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658190909870-f143df19-3b5f-45ed-84f0-fb6323444225.png#clientId=u34863382-a2d1-4&from=paste&id=u25c67cc3&originHeight=596&originWidth=1027&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf67c4724-c04f-4caf-8fa2-bd56cf84328&title=)<br />实际上，这种方式大家看起来，有没有什么问题呢？解决了碎片化严重的情况，但是他把内存空间，直接划分成了相等的两块，如果目前**需要被回收的对象**比较少，存活的对象比较多的话，那么这种复制算法的效率，真的是有点低了。<br />那么有没有一个折中的呢？<br />这就出现了另外一个算法，
<a name="JS71Z"></a>
### 标记整理算法(Mark-Compact)
这种算法比较特殊了，标记阶段和 Mark-Sweep 算法相同，但是整理的时候，就不一样了，标记后不是清理对象，而是将存活对象移向内存的一端。然后清除端边界外的对象。也就是，有可能是存活对象被移到左边，然后右边是需要被清理的对象，<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658190910038-ba5ac7e7-d0cc-461f-8191-cfc663d61e9b.png#clientId=u34863382-a2d1-4&from=paste&id=u179659eb&originHeight=603&originWidth=1002&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0ca79cce-01c8-4fdd-a12b-bf13941df40&title=)<br />这样既能保证了内存空间是连续的，而且还能让效率提升。<br />那么可以回归这个标题了，GC分代收集，为什么这么设计。
<a name="XQtVh"></a>
## 分代
这个就挺好理解的，毕竟都知道一个共同的知识点，那就是 GC堆内存分为了老年代和新生代。<br />如果要选择算法，那么一定得从他们的本质去入手。<br />老年代：存活数量多，需要被处理的对象少<br />新生代：存活数量少，需要被处理的对象多<br />这种从本质的区别就划分出来了，一个存活对象多，一个存活对象少，一个需要被清理的对象多，一个需要被清理的对象少。<br />复制算法因为每次复制的都是存活的对象，而新生代的存活对象都是比较少的，所以这个时候就可以采用复制算法来实现。<br />也就是说，新生代中划分出来的大Eden 区 和两个 Survivor区，每次使用的时候都是 Eden 区和其中的一块 Survivor 区，当进行回收时，将该两块空间中还存活的对象复制到另一块 Survivor 区中。<br />所以因为新生代的这种特性，所以使用复制算法。<br />而老年代因为每次只回收少量对象，因而采用 Mark-Compact 算法。<br />这就是为什么面试的时候，面试官会问为什么GC分代收集时选择不同算法的原因。
<a name="GHKZE"></a>
## JVM的垃圾收集器
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658190909940-d7b683b5-0f46-41db-b5d9-f5124021471a.png#clientId=u34863382-a2d1-4&from=paste&id=uca5bfc82&originHeight=513&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8ddb01c8-89e0-4e67-ac46-9a3bf87afae&title=)<br />一般面试很多都是执着于去问垃圾回收机制和算法，很少有涉及到JVM的垃圾收集器的，稍微科普一下这个小知识。
<a name="LyWS3"></a>
### Serial 收集器（新生代）
最早的收集器<br />采用复制算法，暂停所有用户线程，<br />特点是简单高效并且是单线程，但是容易导致全局停顿，就是经常所说的 STW（全局暂停）。<br />STW：<br />全局停顿，Java 代码停止运行，native 代码继续运行，但不能与 JVM 进行交互
<a name="H8uxF"></a>
### ParNew收集器（新生代）
实际上属于 Serial 收集器 的升级版，从单线程变成了多线程，算法一样，也是暂停所有用户线程。<br />主要用来搭配 CMS 收集器一起使用。
<a name="GBhfx"></a>
### Parallel Scavenge收集器（新生代）
吞吐量收集器，这个收集器关注的是吞吐量<br />在 JVM 中有参数可以配置

- `**-XX:MaxGCPauseMillis**`**：控制最大的垃圾收集停顿时间**
- `**-XX:GCTimeRatio**`**：设置吞吐量的大小，取值 0-100， 系统花费不超过 1/(1+n) 的时间用于垃圾收集**

**Serial Old 收集器（老年代）**<br />老年代的收集器，采用标记-整理算法
<a name="pMMJB"></a>
### CMS 收集器（老年代）
算法采用标记-清除算法实现，<br />一般这个面试问的可能比较多，因为它属于并发的收集器，因为它并不会像前面说的那些收集器一样，会直接导致所有用户线程停止，直到清除结束，而是在标记过程中会有短暂的停止。<br />而是先进行初始标记，然后进行并发标记，修正并发标记用以进行重新标记，最后进行并发清除。
<a name="xEDVD"></a>
### G1 收集器
G1（Garbage-First）收集器将堆内存分割成不同的区域，然后并发的对其进行垃圾回收。G1收集器的设计目标是取代CMS收集器，它同CMS相比，不会产生大量内存碎片，并可以添加预测机制，用户可以指定期望停顿时间(可通过配置`-XX:MaxGCPauseMills=n`最大停顿时间)<br />收集演示图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658190913506-02cfa6ed-a780-464b-af0c-a695c09f7b5b.png#clientId=u34863382-a2d1-4&from=paste&id=uf2e30e02&originHeight=276&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf26363ff-0cad-4756-b62c-f0c574f51e4&title=)<br />说这些，最重要的却是，如何选择合适的垃圾收集器<br />组合选择：

- **单CPU或小内存，单机程序 **`**-XX:+UseSerialGC**`
- **多CPU，需要最大吞吐量，如后台计算型应用 **`**-XX:+UseParallelGC**`**或者 **`**-XX:+UseParallelOldGC**`
- **多CPU，追求低停顿时间，需快速响应如互联网应用 **`**-XX:+UseConcMarkSweepGC**`** **`**-XX:+ParNewGC**`
