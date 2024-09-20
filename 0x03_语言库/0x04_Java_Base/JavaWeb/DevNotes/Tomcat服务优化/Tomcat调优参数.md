Tomcat
<a name="EPmvh"></a>
## 一、3个参数搞定并发配置
作为一个能承接高并发互联网请求的Web容器，首当其冲的当然是海量请求的冲击。幸运的是Tomcat支持NIO，可以通过调整线程数和并发配置，让它表现出最佳的性能。

- `maxThreads`  -- Tomcat接收客户端请求的最大线程数，也就是同时处理任务的个数，它的默认大小为200；一般来说，在高并发的I/O密集型应用中，这个值设置为1000左右比较合理
- `maxConnections` 这个参数是指在同一时间，Tomcat能够接受的最大连接数。对于Java的阻塞式BIO，默认值是`maxthreads`的值；如果在BIO模式使用定制的Executor执行器，默认值将是执行器中`maxThreads`的值。对于Java 新的NIO模式，`maxConnections` 默认值是10000，所以这个参数一般保持不动即可
- `acceptCount`  -- 当线程数量达到上面设置的值，所能接受的最大排队数量。超过了这个值，请求就会被拒绝。我一般会设置成和maxThreads设置成一样大的

简单说明一下上面三个参数的关系：
<a name="njuoE"></a>
### 系统能够保持的连接数
`maxConnections`+`acceptCount`，区别是`maxConnections`中的连接可以被调度处理；`acceptCount`中的连接只能等待排队
<a name="yinlm"></a>
### 系统能处理的请求数
`maxThreads`的大小，实际能够工作的线程数量。<br />幸福指数：`maxThreads` > `maxConnections` > `acceptCount`。<br />现在有些文章还充斥着`maxProcessors`和`minProcessors`。但这两个参数，从Tomcat5开始被deprecated，从6开始就彻底没了。<br />以8为代表，具体配置参数可以参见：[https://tomcat.apache.org/tomcat-8.0-doc/config/http.html](https://tomcat.apache.org/tomcat-8.0-doc/config/http.html)
<a name="oRuUV"></a>
## 二、线程配置
在并发配置方面，可以看到只有`minSpareThreads`，但是却没有`maxSpareThreads`。这是因为，从Tomcat 6开始增加Executor 节点，这个参数已经没用了。<br />由于线程是一个池子，所以它的配置，满足池的一切特点。<br />参照：[https://tomcat.apache.org/tomcat-8.0-doc/config/executor.html](https://tomcat.apache.org/tomcat-8.0-doc/config/executor.html)

- `namePrefix`  -- 每个新开线程的名称前缀
- `maxThreads`  -- 线程池中的最大线程数
- `minSpareThreads`  --  一直处于活跃状态的线程数
- `maxIdleTime`  -- 线程的空闲时间，在超过空闲时间时这些线程则会被销毁
- `threadPriority`  -- 线程池中线程的优先级，默认为5
<a name="aKwxj"></a>
## 三、搞定JVM配置
Tomcat是Java应用，所以JVM的配置同样会影响它的性能。比较重要的配置参数如下。
<a name="cfpCq"></a>
### 2.1、内存区域大小
首先要调整的，就是各个分区的大小，不过这也要分垃圾回收器，我们仅看一下一些全局的参数。

- `-XX:+UseG1GC` 首先，要指定JVM使用的垃圾回收器。尽量不要靠默认值去保证，要显式的指定一个。
- `-Xmx`  设置堆的最大值，一般为操作系统的2/3大小。
- `-Xms` 设置堆的初始值，一般设置成和Xmx一样的大小来避免动态扩容。
- `-Xmn` 年轻代大小，默认新生代占堆大小的1/3。高并发快消亡场景可适当加大这个区域。对半，或者更多，都是可以的。但是在G1下，就不用再设置这个值了，它会自动调整。
- `-XX:MaxMetaspaceSize` 限制元空间的大小，一般256M足够。这一般和初始大小**`-XX:MetaspaceSize`**设置成一样的。
- `-XX:MaxDirectMemorySize` 设置直接内存的最大值，限制通过`DirectByteBuffer`申请的内存。
- `-XX:ReservedCodeCacheSize` 设置JIT编译后的代码存放区大小，如果观察到这个值有限制，可以适当调大，一般够用。
- `-Xss` 设置栈的大小，默认为1M，已经足够用了。
<a name="bzQhX"></a>
### 2.2、内存调优

- `-XX:+AlwaysPreTouch`  启动时就把参数里说好了的内存全部初始化，启动时间会慢一些，但运行速度会增加。
- `-XX:SurvivorRatio` 默认值为8。表示伊甸区和幸存区的比例。
- `-XX:MaxTenuringThreshold` 这个值在CMS下默认为6，G1下默认为15。这个值和前面提到的对象提升有关，改动效果会比较明显。对象的年龄分布可以使用**`-XX:+PrintTenuringDistribution`**打印，如果后面几代的大小总是差不多，证明过了某个年龄后的对象总能晋升到老生代，就可以把晋升阈值设小。
- `PretenureSizeThreshold`  超过一定大小的对象，将直接在老年代分配。不过这个参数用的不是很多。
<a name="DEj8L"></a>
### 2.3、垃圾回收器优化
G1垃圾回收器

- `-XX:MaxGCPauseMillis` 设置目标停顿时间，G1会尽力达成。
- `-XX:G1HeapRegionSize` 设置小堆区大小。这个值为2的次幂，不要太大，也不要太小。如果是在不知道如何设置，保持默认。
- `-XX:InitiatingHeapOccupancyPercent` 当整个堆内存使用达到一定比例（默认是45%），并发标记阶段就会被启动。
- `-XX:ConcGCThreads` 并发垃圾收集器使用的线程数量。默认值随JVM运行的平台不同而不同。不建议修改。
<a name="Pdyfz"></a>
## 四、其他重要配置
再看几个在Connector中配置的重要参数。

- `enableLookups` -- 调用`request`、`getRemoteHost()`执行DNS查询，以返回远程主机的主机名，如果设置为false，则直接返回IP地址。这个要根据需求来
- `URIEncoding`  -- 用于解码URL的字符编码，没有指定默认值为`ISO-8859-1`
- `connectionTimeout`  -- 连接的超时时间(以毫秒为单位)
- `redirectPort` -- 指定服务器正在处理http请求时收到了一个SSL传输请求后重定向的端口号
