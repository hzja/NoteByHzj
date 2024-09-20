<a name="nW6OL"></a>
## 垃圾回收流程的一些流程
<a name="uthLg"></a>
### 哪些对象是垃圾？
当进行垃圾回收的时候，首先需要判断哪些对象是存活的？<br />常用的方法有如下两种

1. 引用计数法
2. 可达性分析法

Python判断对象存活的算法用的是引用计数法，而Java则使用的是可达性分析法。<br />**「通过GC ROOT可达的对象，不能被回收，不可达的对象则可以被回收，搜索走过的路径叫做引用链」**<br />不可达对象会进行2次标记的过程，通过GC ROOT不可达，会被第一次标记。如果需要执行`finalize()`方法，则这个对象会被放入一个队列中执行`finalize()`，如果在`finalize()`方法中成功和引用链上的其他对象关联，则会被移除可回收对象集合（**「一般不建议使用**`**finalize**`**方法」**），否则被回收<br />**「常见的GC ROOT有如下几种」**

1. 虚拟机栈（栈帧中的本地变量表）中引用的对象
2. 方法区中类静态属性引用的对象
3. 方法区中常量引用的对象
4. 本地方法栈中JNI（Native方法）引用的对象

**「照这样看，程序中的GC ROOT有很多，每次垃圾回收都要对GC ROOT的引用链分析一遍，感觉耗费的时间很长，有没有可能减少每次扫描的GC ROOT？」**
<a name="Ojc0C"></a>
### 分代和跨代引用
其实当前虚拟机大多数都遵循了“分代收集”理论进行设计，它的实现基于2个分代假说之上

1. 绝大多数对象都是朝生夕灭的
2. 熬过多次垃圾收集过程的对象就越难以消亡

因此堆一般被分为新生代和老年代，针对新生代的GC叫MinorGC，针对老年代的GC叫OldGC。但是分代后有一个问题，为了找到新生代的存活对象，不得不遍历老年代，反过来也一样![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683510086262-9a5eb57f-8b5e-4315-8c16-d57d7e85ad3a.png#averageHue=%23f9f7f4&clientId=u132bf46d-7e4e-4&from=paste&id=u9a7217df&originHeight=371&originWidth=642&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u98727472-9f82-4fc0-9dc9-ed4a8ea8b71&title=)<br />当进行MinorGC的时候，如果只遍历新生代，那么可以判定ABCD为存活对象。但是E不会被判断为存活对象，所以就会有问题。<br />为了解决这种跨代引用的对象，最笨的办法就是遍历老年代的对象，找出这些跨代引用的对象。但这种方式对性能影响较大<br />这时就不得不提到第三个假说<br />**「跨代引用相对于同代引用来说仅占极少数。」**<br />根据这条假说，就不需要为了少量的跨代引用去扫描整个老年代。**「为了避免遍历老年代的性能开销，垃圾回收器会引入一种记忆集的技术，记忆集就是用来记录跨代引用的表」**<br />如新生代的记忆集就保存了老年代持有新生代的引用关系<br />所以在进行MinorGC的时候，只需要将包含跨代引用的内存区域加入GC ROOT一起扫描就行了
<a name="pKQzB"></a>
### 卡表
前面说到垃圾收集器用记忆集来记录跨代引用。其实可以把记忆集理解为接口，卡表理解为实现，类比Map和HashMap。<br />卡表最简单的形式可以只是一个字节数组， 而HotSpot虚拟机确实也是这样做的。以下这行代码是HotSpot默认的卡表标记逻辑：
```c
CARD_TABLE [this address >> 9] = 0;
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683510086262-bdabee29-a035-4b31-9cba-38ccecef53c7.png#averageHue=%23fbf8f3&clientId=u132bf46d-7e4e-4&from=paste&id=u71a2f253&originHeight=331&originWidth=521&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4376ad6f-b3a1-4b63-bd5e-b6a10f41f37&title=)<br />HotSpot用一个数组元素来保存对应的内存地址是有有跨代引用对象（从this address右移9位可以看出每个元素映射了512字节的内存）<br />当数组元素值为0时表明对应的内存地址不存在跨代引用对象，否则存在（称为卡表中这个元素变脏）
<a name="IE8qt"></a>
### 如何更新卡表？
**「将卡表元素变脏的过程，HotSpot是通过写屏障来实现的」**，即当其他代对象引用当前分代对象的时候，在引用赋值阶段更新卡表，具体实现方式类似于AOP
```c
void oop_field_store(oop* field, oop new_value) { 
    // 引用字段赋值操作
    *field = new_value;
    // 写后屏障，在这里完成卡表状态更新 
    post_write_barrier(field, new_value);
}
```
<a name="QnoAY"></a>
## 三色标记法
<a name="eAuhZ"></a>
### 执行思路
**「如何判断一个对象可达呢？这就不得不提到三色标记法」**<br />白色：刚开始遍历的时候所有对象都是白色的 灰色：被垃圾回收器访问过，但至少还有一个引用未被访问 黑色：被垃圾回收器访问过，并且这个对象的所有引用都被访问过，是安全存活的对象（GC ROOT会被标记为黑色）<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683510086201-2dd36c41-2300-40ee-b16d-5f7bd969b276.png#averageHue=%23e0e0e0&clientId=u132bf46d-7e4e-4&from=paste&id=ufee6a39d&originHeight=337&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf9b5bca7-c4ae-4298-83b0-7cf91a1c344&title=)以上图为例，三色标记法的执行流程如下

1. 先将GC ROOT引用的对象B和E标记为灰色
2. 接着将B和E引用的对象A，C和F标记为灰色，此时B和E标记为黑色
3. 依次类推，最终被标记为白色的对象需要被回收
<a name="okPob"></a>
### 三色标记法问题
可达性分析算法根节点枚举这一步必须要在一个能保障一致性的快照中分析，所以要暂停用户线程（Stop The World ，STW），在各种优化技巧的加持下，停顿时间已经非常短了。<br />在从根节点扫描的过程则不需要STW，但是也会发生一些问题。由于此时垃圾回收线程和用户线程一直运行，所以引用关系会发生变化

1. 应该被回收的对象被标记为不被回收
2. 不应该被回收的对象标记为应该回收

第一种情况影响不大，大不了后续回收即可。但是第二种情况则会造成致命错误<br />所以经过研究表明，只有同时满足两个条件才会发生第二种情况

1. 插入了一条或者多条黑色到白色对象的引用
2. 删除了全部从灰色到白色对象的引用

为了解决这个问题，破坏2个条件中任意一个不就行了，由此产生了2中解决方案，**「增量更新」**和**「原始快照」**。CMS使用的是增量更新，G1使用的是原始快照<br />**「增量更新要破坏的是第一个条件」**， 当黑色对象插入新的指向白色对象的引用关系时， 就将这个新插入的引用记录下来， 等并发扫描结束之后， 再将这些记录过的引用关系中的黑色对象为根， 重新扫描一次。这可以简化理解为， 黑色对象一旦新插入了指向白色对象的引用之后， 它就变回灰色对象了<br />**「原始快照要破坏的是第二个条件」**， 当灰色对象要删除指向白色对象的引用关系时， 就将这个要删除的引用记录下来， 在并发扫描结束之后， 再将这些记录过的引用关系中的灰色对象为根， 重新扫描一次。这也可以简化理解为， 无论引用关系删除与否， 都会按照刚刚开始扫描那一刻的对象图快照来进行搜索。<br />参考自《深入理解Java虚拟机》
<a name="lLu9E"></a>
## 垃圾收集器
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683510086190-817708d2-e8f1-4647-90fb-cd7150d19424.png#averageHue=%23c4c4bc&clientId=u132bf46d-7e4e-4&from=paste&id=u6a67377b&originHeight=907&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u1cfeac79-aa54-4f6e-9470-9783f39a98d&title=)图中展示了七种作用于不同分代的收集器，如果两个收集器之间存在连线，就说明它们可以搭配使用。在JDK8时将Serial+CMS，ParNew+Serial Old这两个组合声明为废弃，并在JDK9中完全取消了这些组合的支持<br />并行和并发都是并发编程中的专业名词，在谈论垃圾收集器的上下文语境中， 它们可以理解为<br />**「并行（Parallel）」**：指多条垃圾收集线程并行工作，但此时用户线程仍然处于等待状态<br />**「并发（Concurrent」**）：指用户线程与垃圾收集线程同时执行
<a name="YXx8K"></a>
## Serial收集器
**「新生代，标记-复制算法，单线程。进行垃圾收集时，必须暂停其他所有工作线程，直到它收集结束」**![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683510086221-3845f811-f919-4179-bc58-123e5483d8d2.png#averageHue=%23fafafa&clientId=u132bf46d-7e4e-4&from=paste&id=u08a607c8&originHeight=346&originWidth=830&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7bf82bd4-a4e5-40b6-bc5e-03d88dfcbfd&title=)
<a name="slZ6Q"></a>
## ParNew收集器
**「ParNew本质上是Serial收集器的多线程并行版本」**![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683510086677-5d6e756a-e832-452a-b7a8-198302eb90dc.png#averageHue=%23f9f9f9&clientId=u132bf46d-7e4e-4&from=paste&id=u3c1490e6&originHeight=342&originWidth=837&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u142bee6d-d3d4-4adc-b673-1bd5acdf059&title=)
<a name="NN5Ua"></a>
## Parallel Scavenge收集器
**「新生代，标记复制算法，多线程，主要关注吞吐量」**<br />吞吐量=运行用户代码时间/(运行用户代码时间+运行垃圾收集时间)
<a name="rv3kJ"></a>
## Serial Old收集器
**「老年代，标记-整理算法，单线程，是Serial收集器的老年代版本」**<br />用处有如下2个

1. 在JDK5以及之前的版本中与Parallel Scavenge收集器搭配使用
2. 作为CMS收集器发生失败时的后备预案，在并发收集发生Concurrent Mode Failure时使用
<a name="Tzzbc"></a>
## Parallel Old收集器
**「老年代，标记-整理算法，多线程，是Parallel Scavenge收集器的老年代版本」**<br />在注重吞吐量或者处理器资源较为稀缺的场合，都可以优先考虑Parallel Scavenge加Parallel Old收集器这个组合![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683510086768-59d00e5c-0b9c-4ed5-b4b8-127accbc7b0f.png#averageHue=%23f9f9f9&clientId=u132bf46d-7e4e-4&from=paste&id=u345907d0&originHeight=338&originWidth=850&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u1d767871-dc4f-48c0-b943-254ba5e782d&title=)
<a name="b7Ijg"></a>
## CMS收集器
**「老年代，标记-清除算法，多线程，主要关注延迟」**<br />运作过程分为4个步骤

1. 初始标记（CMS initial mark）
2. 并发标记（CMS concurrent mark）
3. 重新标记（CMS remark）
4. 并发清除（CMS concurrent sweep）

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683510086963-7c4e0bc0-29be-48e6-a831-3dee5a3345b2.png#averageHue=%23f9f9f9&clientId=u132bf46d-7e4e-4&from=paste&id=u88d4aa6c&originHeight=363&originWidth=884&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u45bb2243-328d-456e-a7ba-e677ee9c6f2&title=)

1. 初始标记：标记一下GC Roots能直接关联到的对象，速度很快（这一步会发生STW）
2. 并发标记：从GC Roots的直接关联对象开始遍历整个对象图的过程，这个过程耗时较长但是不需要停顿用户线程，可以与垃圾收集一起并发运行
3. 重新标记：为了修正并发标记期间，因用户程序继续运作而导致标记产生变动的那一部分对象的标记记录（**「就是三色标记法中的增量更新」**，这一步也会发生STW）
4. 并发清除：清理删除掉标记阶段判断的已经死亡的对象，由于不需要移动存活对象，所以看这个阶段也是可以与用户线程同时并发的
<a name="xnWsO"></a>
## 总结
| 收集器 | 收集对象和算法 | 收集器类型 | 说明 | 适用场景 |
| --- | --- | --- | --- | --- |
| Serial | 新生代，复制算法 | 单线程 | 

 | 简单高效；适合内存不大的情况 |
| ParNew | 新生代，复制算法 | 并行的多线程收集器 | ParNew垃圾收集器是Serial收集器的多线程版本 | 搭配CMS垃圾回收器的首选 |
| Parallel Scavenge吞吐量优先收集器 | 新生代，复制算法 | 并行的多线程收集器 | 类似ParNew，更加关注吞吐量，达到一个可控制的吞吐量 | 本身是Server级别多CPU机器上的默认GC方式，主要适合后台运算不需要太多交互的任务 |


---

| 收集器 | 收集对象和算法 | 收集器类型 | 说明 | 适用场景 |
| --- | --- | --- | --- | --- |
| Serial Old | 老年代，标记整理算法 | 单线程 | 

 | Client模式下虚拟机使用 |
| Parallel Old | 老年代，标记整理算法 | 并行的多线程收集器 | Paraller Scavenge收集器的老年代版本，为了配置Parallel Svavenge的面向吞吐量的特性而开发的对应组合 | 在注重吞吐量以及CPU资源敏感的场合采用 |
| CMS | 老年代，标记清除算法 | 并行与并发收集器 | 尽可能的缩短垃圾收集时用户线程停止时间；缺点在于，1.内存碎片，2.需要更多CPU资源，3.浮动垃圾问题，需要更大的堆空间 | 重视服务的相应速度，系统停顿时间和用户体验的互联网网站或者B/S系统。互联网后端目前cms是主流的垃圾回收器 |
| G1 | 跨新生代和老年代；标记整理+化整为零 | 

 | 并行与并发收集器 | JDK1.7才正式引入，采用分区回收的思维，基本不牺牲吞吐量的前提下完成低停顿的内存回收；可预测的停顿是其最大的优势 |

