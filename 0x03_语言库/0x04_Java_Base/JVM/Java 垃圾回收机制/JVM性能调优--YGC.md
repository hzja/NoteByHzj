JVMYGC<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657433321706-5e56e9c6-34c2-4ed6-8bf0-5e034084f754.png#clientId=u7ce9579f-c0e4-4&from=paste&id=u171e4433&originHeight=423&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua61d426f-7f7e-4143-9f7e-bb8c8e19207&title=)
<a name="pfiLI"></a>
## YGC
YGC频次暂且忽略，问题主要集中在gc耗时上面。想要解决YGC耗时问题，首先要搞清楚YGC的耗时节点。_（YGC问题，还没有逃出过这些维度）_

- _**存活对象扫描、标记时间**_
- _**存活对象copy to S区，晋升到Old区**_
- _**等待各线程到达安全点时间**_
- _**GC日志输出**_
- _**操作系统活动(swap)**_
<a name="rmOiY"></a>
### 1、等待线程到达安全点
GC发生时，程序是会STW的(Serial, ParNew, Parallel Scanvange, ParallelOld, Serial Old全程都会STW，CMS等在初始标记重新标记阶段也会STW), JVM这时候只运行GC线程，不运行用户线程。<br />那JVM具体要在哪里，在什么时间点STW呢？ 答：_**安全点**_。<br />因此，GC时，程序需要运行到最近的一个安全点(方法返回、循环结束、异常抛出等位置)停下来，安全点日志前面文章也提到了：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657433321531-58bbe16d-056d-47cf-bff2-adb3ef091770.png#clientId=u7ce9579f-c0e4-4&from=paste&id=ufa1c4edc&originHeight=1048&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4c517710-d974-4191-8b3a-572b18762e4&title=)如果发现 spin时间段表现异常，那么就要去查看下代码中是否有影响线程快速到达安全点的逻辑块，比如大循环体等。<br />顺便提一下，安全点的作用不只是进行GC，下面这些点都是会导致程序进入安全点的，其相关问题可以单开一篇来叙述![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657433321635-61b0e22b-4eaa-4455-a455-26158bcc380e.png#clientId=u7ce9579f-c0e4-4&from=paste&id=uec072c38&originHeight=420&originWidth=880&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3f0e1e82-8586-4f47-a6af-8ffed209fcf&title=)
<a name="aQ1qK"></a>
### 2、存活对象扫描和标记
在程序进入安全点之后，下一步操作就是root对象的标记和引用对象的可达性分析。
<a name="TTCAc"></a>
#### 2.1 线程栈中的对象引用
这里需要引出两个概念，_**OopMap**_和_**RememberedSet**_<br />为了获取root对象，jvm需要从线程栈中找到堆内存对象的引用，但是不一定都是引用，如果全栈扫描那就太浪费时间了，怎么办，_**空间换时间**_，用OopMap维护对象引用，在到达安全点时，先更新oopMap，然后进行遍历。然后由RememberedSet辅助进行可达性分析。<br />这里需要注意的是，虽然有了额外存储空间的加持，但是整体的存活对象标记耗时也是不好直接忽视的。<br />例：某网友的系统，因为采用的调用方式有问题，导致系统中存活的业务线程高达几千之多，那么，要遍历这么多的OopMap的时间消耗也是非常大的。<br />还有一些程序，因为类似select等查询语句没有添加限制条件，导致大量数据加载进内存，导致GC时间过长，甚至是OOM的发生。<br />因此，要关注对象引用的个数，包括一个线程中的引用个数和总的线程个数。
<a name="WWkxF"></a>
#### 2.2 本地缓存存放的对象过多
如果对象存活的年限达到了设置的上限，是会晋升老年代的。如果有大量的本地缓存对象被创建，在其晋升到老年代之后，YGC会通过扫描card table来确认其是否存活，从而增加YGC的是存活对象标记时间。<br />因此，本地缓存方案要评估完整，或者考虑用堆外缓存减少本地缓存对GC的影响。
<a name="MGpjV"></a>
#### 2.3 system class loader 加载对象过多
如果程序中有解析xml的逻辑时，每new一个XStream对象，就会新创建一个classloader，类加载器会和类的权限定名作为key，value为真正的Klass对象，会存储在SystemDirectionary里，最终越来越多的存活对象存储在内存里，导致需要占用很长时间去标记。<br />“The XStream instance is thread-safe. That is, once the XStream instance has been created and configured, it may be shared across multiple threads allowing objects to be serialized/deserialized concurrently”.<br />XStream官方的说法是XStream线程安全，不需要重复初始化xstream对象，为每个反序列化的对象声明一个静态的XStream，重复利用即可。
<a name="x2bMs"></a>
#### 2.4 JNI & Monitor & finalizable等
暂时没有遇到过具体的异常实例，后续遇到了再补充吧。
<a name="bHAhW"></a>
### 3、存活对象Copy
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657433321571-54db4991-f209-42e9-adb2-e2af044b6b85.png#clientId=u7ce9579f-c0e4-4&from=paste&id=u75ed925d&originHeight=1086&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2dbbc308-e285-4578-8611-cba04e69f02&title=)可能不是特别准确，因为机器1的GC不一定会发生，只是想用这个简单的例子说明下eden区和新对象大小对GC copy的影响，进而对GC时间产生影响。<br />所以，要善于运用JVM监控，预估每次调用的新对象和存活对象的大小，结合并发数，设计合理的eden区和survivor区。
<a name="al1G8"></a>
### 4、GC日志输出
GC日志是比较让人忽略的点，但是确实会对GC时间产生负面的影响。因为GC时的STW的总时间内，包含了GC日志打印的时间，正常情况下，输出有限信息的GC日志对GC整体时间的影响应该是微乎其微的，但是如果正好遇到了GC日志输出时系统的IO负载很好，那么可能会在日志输出这里等待很长的时间了。<br />一个解决办法是将GC日志文件放到tmpfs上（例如，`-Xloggc:/tmpfs/gc.log`）。因为tmpfs没有磁盘文件备份，所以tmpfs文件不会导致磁盘行为，因此也不会被磁盘IO阻塞。<br />_**总结：YGC耗时问题可以说千奇百怪，但是万变不离其中，只要能够掌握YGC的几个关键节点涉及的影响，从原理着手分析，应该是没有什么大问题的~**_
