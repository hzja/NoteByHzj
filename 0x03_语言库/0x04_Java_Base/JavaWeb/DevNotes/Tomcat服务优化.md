Java JavaWeb Tomcat<br />Tomcat作为Web服务器，它的处理性能直接关系到用户体验，下面是几种常见的优化措施：
<a name="tv2ZM"></a>
## 利用缓存和压缩

- 对于静态页面最好是能够缓存起来，这样就不必每次从磁盘上读。这里采用了Nginx作为缓存服务器，将图片、css、js文件都进行了缓存，有效的减少了后端Tomcat的访问。
- 另外，为了能加快网络传输速度，开启gzip压缩也是必不可少的。但考虑到Tomcat已经需要处理很多东西了，所以把这个压缩的工作就交给前端的Nginx来完成。
- 除了文本可以用gzip压缩，其实很多图片也可以用图像处理工具预先进行压缩，找到一个平衡点可以让画质损失很小而文件可以减小很多。曾经就见过一个图片从300多kb压缩到几十kb，自己几乎看不出来区别。
<a name="Bvs9l"></a>
## 采用集群
单个服务器性能总是有限的，最好的办法自然是实现横向扩展，那么组建Tomcat集群是有效提升性能的手段。还是采用了Nginx来作为请求分流的服务器，后端多个Tomcat共享session来协同工作。
<a name="iefDO"></a>
## 优化线程数优化
找到`Connector port="8080" protocol="HTTP/1.1"`，增加`maxThreads`和`acceptCount`属性（使`acceptCount`大于等于`maxThreads`），如下：
```xml
<Connector port="8080" protocol="HTTP/1.1"connectionTimeout="20000" redirectPort="8443"acceptCount="500" maxThreads="400" />
```
其中：<br />• `maxThreads`：tomcat可用于请求处理的最大线程数，默认是200<br />• `minSpareThreads`：tomcat初始线程数，即最小空闲线程数<br />• `maxSpareThreads`：tomcat最大空闲线程数，超过的会被关闭<br />• `acceptCount`：当所有可以使用的处理请求的线程数都被使用时，可以放到处理队列中的请求数，超过这个数的请求将不予处理.默认100
<a name="yDKwM"></a>
## 使用线程池优化
在server.xml中增加`executor`节点，然后配置`connector`的`executor`属性，如下：
```xml
<Executor name="tomcatThreadPool" namePrefix="req-exec-"maxThreads="1000" minSpareThreads="50"maxIdleTime="60000"/>
<Connector port="8080" protocol="HTTP/1.1"executor="tomcatThreadPool"/>
```
其中：

- `namePrefix`：线程池中线程的命名前缀
- `maxThreads`：线程池的最大线程数
- `minSpareThreads`：线程池的最小空闲线程数
- `maxIdleTime`：超过最小空闲线程数时，多的线程会等待这个时间长度，然后关闭
- `threadPriority`：线程优先级

注：<br />当Tomcat并发用户量大的时候，单个jvm进程确实可能打开过多的文件句柄，这时会报java.net.SocketException:Too many open files错误。<br />可使用下面步骤检查：

- `ps -ef |grep tomcat` 查看Tomcat的进程ID，记录ID号，假设进程ID为10001
- `lsof -p 10001|wc -l` 查看当前进程id为10001的 文件操作数
- 使用命令：`ulimit -a` 查看每个用户允许打开的最大文件数
<a name="lIz4S"></a>
## 启动速度优化

- 删除没用的web应用：因为Tomcat启动每次都会部署这些应用。
- 关闭WebSocket：websocket-api.jar和tomcat-websocket.jar。
- 随机数优化：设置JVM参数：`-Djava.security.egd=file:/dev/./urandom`。
<a name="wvH5P"></a>
## 内存优化
有富余物理内存的情况，加大Tomcat使用的jvm的内存<br />服务器所能提供CPU、内存、硬盘的性能对处理能力有决定性影响。

- 对于高并发情况下会有大量的运算，那么CPU的速度会直接影响到处理速度。
- 内存在大量数据处理的情况下，将会有较大的内存容量需求，可以用`-Xmx` `-Xms` `-XX:MaxPermSize`等参数对内存不同功能块进行划分。之前就遇到过内存分配不足，导致虚拟机一直处于full GC，从而导致处理能力严重下降。
- 硬盘主要问题就是读写性能，当大量文件进行读写时，磁盘极容易成为性能瓶颈。最好的办法还是利用下面提到的缓存。

堆内存相关参数，比如说：<br />• `-Xms`：虚拟机初始化时的最小堆内存。<br />• `-Xmx`：虚拟机可使用的最大堆内存。`-Xms`与`-Xmx`设成一样的值，避免JVM因为频繁的GC导致性能大起大落<br />• `-XX:MaxNewSize`：新生代占整个堆内存的最大值。<br />另外还有方法区参数调整（注意：JDK版本）、垃圾收集器等优化。
