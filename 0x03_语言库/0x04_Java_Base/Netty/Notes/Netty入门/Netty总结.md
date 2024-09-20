<a name="eW9hf"></a>
## 简介
Netty 是由 JBOSS 提供的一个 java 开源框架。 Netty 提供异步的、事件驱动的网络应用程序框架和工具，用以快速开发高性能、高可靠性的网络服务器和客户端程序。<br />Netty 是一个基于 NIO 的客户、服务器端编程框架，使用 Netty 可以确保你快速和简单的开发出一个网络应用，例如实现了某种协议的客户、服务端应用。<br />Netty 是一个吸收了多种协议（包括 FTP、 SMTP、 HTTP等各种二进制文本协议）的实现经验，并经过相当精心设计的项目。<br />Netty 在互联网领域、大数据分布式计算领域、游戏行业、通信行业等获得了广泛的应用，Elasticsearch和Dubbo就是基于netty设计的。
<a name="iiswq"></a>
## Reactor模型
<a name="kR0pj"></a>
### Reactor单线程模型
用户发起 IO 请求到 Reactor 线程，Ractor 线程将用户的 IO 请求放入到通道，然后再进行后续处理，处理完成后，Reactor 线程重新获得控制权，继续其他客户端的处理。<br />这种模型一个时间点只有一个任务在执行，这个任务执行完了，再去执行下一个任务。<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1698396988069-9832eb12-770f-43a9-9ced-9af1f729e4dc.png#averageHue=%23f6edc8&clientId=u37a0dad8-a673-4&from=paste&height=365&id=uc424ca5a&originHeight=913&originWidth=2188&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=640084&status=done&style=none&taskId=u8d45b7d3-361f-4fa8-997a-4d950263746&title=&width=875.2)
<a name="XLm23"></a>
### Reactor多线程模型
Reactor 多线程模型是由一组 NIO 线程来处理 IO 操作（之前是单个线程），所以在请求处理上会比上一中模型效率更高，可以处理更多的客户端请求。<br />这种模式使用多个线程执行多个任务， 任务可以同时执行。<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1698397382559-be03c230-8144-4f36-a8fb-89b9863bdcea.png#averageHue=%23f8f2dd&clientId=u37a0dad8-a673-4&from=paste&height=612&id=u20c26315&originHeight=1529&originWidth=2208&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=978265&status=done&style=none&taskId=udabd61b1-9b6b-486c-abcc-213bce91bbd&title=&width=883.2)
<a name="vwIqb"></a>
### Reactor主从多线程模型
这种线程模型是 Netty 推荐使用的线程模型。这种模型适用于高并发场景，一组线程池接收请求，一组线程池处理 IO。<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1698398031494-af1fc58c-e09e-4fd5-88d8-17a614249784.png#averageHue=%23f7f3df&clientId=u37a0dad8-a673-4&from=paste&height=623&id=u753da75f&originHeight=1557&originWidth=2171&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=1037112&status=done&style=none&taskId=u1f17a39a-a8f9-42bc-a2f6-da60c7f4dd9&title=&width=868.4)
<a name="iMqY1"></a>
## Netty模型
<a name="jrrHQ"></a>
### 线程模型
Server端包含1个Boss NioEventLoopGroup和1个Worker NioEventLoopGroup。NioEventLoopGroup相当于1个事件循环组，这个组里包含多个事件循环NioEventLoop，每个NioEventLoop包含1个selector和1个事件循环线程。<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1698398197833-a02d1885-c5bb-4fab-a486-016ab2b18571.png#averageHue=%23383a47&clientId=u37a0dad8-a673-4&from=paste&height=536&id=u8df0ec07&originHeight=1340&originWidth=2147&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=569022&status=done&style=none&taskId=ud6d16229-5bfd-43fb-9752-85ad84822ff&title=&width=858.8)
<a name="hrja8"></a>
### Boss、Worker、NioEventLoop
BossGroup专门负责接收客户端连接，WorkerGroup专门负责网络读写操作。NioEventLoop表示一个不断循环执行处理任务的线程，每个 NioEventLoop 都有一个 selector，用于监听绑定在其上的 socket 网络通道。NioEventLoop内部采用串行化设计，从消息的读取->解码->处理->编码->发送，始终由 IO 线程 NioEventLoop 负责。<br />对应关系：<br />一个 NioEventLoopGroup 下包含多个 NioEventLoop<br />每个 NioEventLoop 中包含有一个 Selector，一个 taskQueue<br />每个 NioEventLoop 的 Selector 上可以注册监听多个 NioChannel<br />每个 NioChannel 只会绑定在唯一的 NioEventLoop 上<br />每个 NioChannel 都绑定有一个自己的 ChannelPipeline<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1698410237045-86ae5ff8-4071-44bb-9c5e-e0b43881c068.jpeg)
<a name="Joirm"></a>
### 每个Boss NioEventLoop循环执行的任务包含3步

1. 轮询accept事件
2. 处理accept I/O事件，与Client建立连接，生成`NioSocketChannel`，并将`NioSocketChannel`注册到某个Worker NioEventLoop的Selector上
3. 处理任务队列中的任务，runAllTasks。任务队列中的任务包括用户调用`eventloop.execute`或schedule执行的任务，或者其它线程提交到该`eventloop`的任务。
<a name="yAg8Y"></a>
### 每个Worker NioEventLoop循环执行的任务包含3步

1. 轮询read、write事件；
2. 处I/O事件，即read、write事件，在`NioSocketChannel`可读、可写事件发生时进行处理
3. 处理任务队列中的任务，runAllTasks。
<a name="yR38j"></a>
### 异步模型
异步的概念和同步相对。当一个异步过程调用发出后，调用者不能立刻得到结果。实际处理这个调用的组件在完成后，通过状态、 通知和回调来通知调用者。
<a name="gR6AX"></a>
### Future，Callback和Handler
Netty 的异步模型是建立在 future 和 callback 的之上的，callback 就是回调。Future表示异步的执行结果，可以通过它提供的方法来检测执行是否完成。<br />在使用 Netty 进行编程时，拦截操作和转换出入站数据只需要您提供 callback 或利用future 即可。
<a name="r8z7z"></a>
### Future-Listener机制
Future的核心思想是：假设一个方法 fun，计算过程可能非常耗时，等待 fun 返回显然不合适。那么可以在调用 fun 的时候，立马返回一个 Future， 后续可以通过 Future 去监控方法 fun 的处理过程。<br />Netty 中的 I/O 操作是异步的，包括 Bind、 Write、 Connect 等操作会简单的返回一个 ChannelFuture。调用者并不能立刻获得结果，而是通过 Future-Listener 机制，用户可以方便的主动获取或者通过通知机制获得IO 操作结果。<br />链式操作<br />在使用 Netty 进行编程时， 拦截操作和转换出入站数据等使用链式操作，只需要提供 callback 或利用future 即可实现，Handler由于事件的处理。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1698398407776-235e1f24-cb3f-4e55-9d90-214598e4f032.jpeg)
<a name="b5otF"></a>
## Netty中的主要类
<a name="ryMjd"></a>
### ChannelHandle
经常需要自定义一个Handler类去继承 `ChannelInboundHandlerAdapter`，然后通过重写相应方法实现业务逻辑，一般需要重写的方法：<br />常用方法：
```java
public void channelActive(ChannelHandlerContext ctx) // 通道就绪事件
public void channelRead(ChannelHandlerContext ctx, Object msg) // 通道读取数据事件
public void channelReadComplete(ChannelHandlerContext ctx)  // 数据读取完毕事件
public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) // 通道发生异常事件
```
<a name="WbXPE"></a>
### Pipeline 和 `ChannelPipeline`
`ChannelPipeline`是一个Handler的集合，它负责处理和拦截 inbound 或者 outbound 的事件和操作，相当于一个贯穿 Netty 的链。<br />常用方法：
```java
// 把一个业务处理类（handler）添加到链中的第一个位置
ChannelPipeline addFirst(ChannelHandler... handlers) 

// 把一个业务处理类（handler）添加到链中的最后一个位置
ChannelPipeline addLast(ChannelHandler... handlers) 
```
<a name="BBaOb"></a>
### `ChannelHandlerContext`
这是事件处理器上下文对象，Pipeline链中的实际处理节点 。<br />每个处理节点`ChannelHandlerContext` 中包含一个具体的事件处理器 `ChannelHandler`，<br />同时`ChannelHandlerContext` 中也绑定了对应的 pipeline 和 Channel 的信息，方便对 `ChannelHandler`进行调用。<br />常用方法：
```java
// 关闭通道
ChannelFuture close();
// 刷新
ChannelOutboundInvoker flush();
// 将数据写到ChannelPipeline中当前ChannelHandler的下一个ChannelHandler开始处理。
ChannelFuture writeAndFlush(Object msg);
```
<a name="UM1zv"></a>
### ChannelOption
Netty 在创建 Channel 实例后，一般都需要设置 `ChannelOption` 参数。`ChannelOption` 是 Socket 的标准参数，而非 Netty 独创的。<br />常用的参数配置有两个：
```java
ChannelOption.SO_BACKLOG
```
对应 TCP/IP 协议 listen 函数中的 backlog 参数，用来初始化服务器可连接队列大小。<br />服务端处理客户端连接请求是顺序处理的，所以同一时间只能处理一个客户端连接。<br />多个客户端来的时候，服务端将不能处理的客户端连接请求放在队列中等待处理，backlog 参数指定了队列的大小。
```java
ChannelOption.SO_KEEPALIVE
```
一直保持连接活动状态。
<a name="S6U9b"></a>
### `ChannelFuture`
表示 Channel 中异步 I/O 操作的结果，在 Netty 中所有的 I/O 操作都是异步的，I/O 的调用会直接返回，调用者并不能立刻获得结果，但是可以通过 `ChannelFuture` 来获取 I/O 操作的处理状态。<br />常用方法：
```java
// 返回当前正在进行 IO 操作的通道
Channel channel();
// 等待异步操作执行完毕
ChannelFuture sync();
```
<a name="AXpXa"></a>
### `EventLoopGroup`和其实现类 `NioEventLoopGroup`
EventLoopGroup 是一组 EventLoop 的抽象，Netty 为了更好的利用多核 CPU 资源，一般会有多个 EventLoop 同时工作，每个 EventLoop 维护着一个 Selector 实例。<br />EventLoopGroup 提供 next 接口，可以从组里面按照一定规则获取其中一个 EventLoop来处理任务。<br />在 Netty 服务器端编程中，一般都需要提供两个 EventLoopGroup，例如：`BossEventLoopGroup` 和 WorkerEventLoopGroup。<br />通常一个服务端口即一个 `ServerSocketChannel`对应一个Selector 和一个EventLoop线程。`BossEventLoop` 负责接收客户端的连接并将 SocketChannel 交给 `WorkerEventLoopGroup` 来进行 IO 处理。<br />`BossEventLoopGroup`和`WorkerEventLoopGroup`执行流程：<br />`BossEventLoopGroup` 通常是一个单线程的 EventLoop，EventLoop 维护着一个注册了`ServerSocketChannel` 的 Selector 实例，BossEventLoop 不断轮询 Selector 将连接事件分离出来，通常是 OP_ACCEPT 事件，然后将接收到的 SocketChannel 交给 `WorkerEventLoopGroup`，`WorkerEventLoopGroup` 会由 next 选择其中一个 EventLoopGroup 来将这个 SocketChannel 注册到其维护的 Selector 并对其后续的 IO 事件进行处理。<br />常用方法：
```java
// 构造方法
public NioEventLoopGroup();
// 断开连接，关闭线程
public Future<?> shutdownGracefully();
```
<a name="jWeQe"></a>
### ServerBootstrap 和 Bootstrap
ServerBootstrap 是 Netty 中的服务器端启动助手，通过它可以完成服务器端的各种配置；<br />Bootstrap 是 Netty 中的客户端启动助手，通过它可以完成客户端的各种配置。<br />常用方法：
```java
// 该方法用于服务器端，用来设置两个 EventLoop
public ServerBootstrap group(EventLoopGroup parentGroup, EventLoopGroup childGroup)

// 该方法用于客户端，用来设置一个 EventLoop
public B group(EventLoopGroup group) 

// 该方法用来设置一个服务器端的通道实现
public B channel(Class<? extends C> channelClass) 

// 用来给 ServerChannel 添加配置
public <T> B option(ChannelOption<T> option, T value) 

// 用来给接收到的通道添加配置
public <T> ServerBootstrap childOption(ChannelOption<T> childOption, T value)

// 该方法用来设置业务处理类（自定义的 handler）
public ServerBootstrap childHandler(ChannelHandler childHandler)

// 该方法用于服务器端，用来设置占用的端口号
public ChannelFuture bind(int inetPort)

// 该方法用于客户端，用来连接服务器端
public ChannelFuture connect(String inetHost, int inetPort)
```
<a name="HuPfk"></a>
### Unpooled 类
Unpooled 是 Netty 提供的一个专门用来操作缓冲区的工具类。<br />常用方法：
```java
// 通过给定的数据和字符编码返回一个 ByteBuf 对象（类似于 NIO 中的 ByteBuffer 对象）
public static ByteBuf copiedBuffer(CharSequence string, Charset charset)
```
