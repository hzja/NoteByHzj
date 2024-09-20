Netty
<a name="V0dV5"></a>
## 简介
看一下epoll的使用。两者都是更加高级的IO方式，都需要借助`native`的方法实现，不同的是Kqueue用在mac系统中，而`epoll`用在liunx系统中。
<a name="p0XKk"></a>
## epoll的详细使用
epoll的使用也很简单，还是以常用的聊天室为例来讲解epoll的使用。<br />对于server端来说需要创建`bossGroup`和`workerGroup`，在NIO中这两个group是`NIOEventLoopGroup`，在epoll中则需要使用`EpollEventLoopGroup`：
```java
EventLoopGroup bossGroup = new EpollEventLoopGroup(1);
EventLoopGroup workerGroup = new EpollEventLoopGroup();
```
接着需要将`bossGroup`和`workerGroup`传入到`ServerBootstrap`中：
```java
ServerBootstrap b = new ServerBootstrap();
b.group(bossGroup, workerGroup)
	.channel(EpollServerSocketChannel.class)
	.handler(new LoggingHandler(LogLevel.INFO))
	.childHandler(new NativeChatServerInitializer());
```
注意，这里传入的channel是`EpollServerSocketChannel`，专门用来处理epoll的请求。其他的部分和普通的NIO服务是一样的。<br />接下来看下epoll的客户端，对于客户端来说需要创建一个`EventLoopGroup`，这里使用的是`EpollEventLoopGroup`：
```java
EventLoopGroup group = new EpollEventLoopGroup();
```
然后将这个`group`传入`Bootstrap`中去：
```java
Bootstrap b = new Bootstrap();
b.group(group)
	.channel(EpollSocketChannel.class)
	.handler(new NativeChatClientInitializer());
```
这里使用的channel是`EpollSocketChannel`，是和`EpollServerSocketChannel`对应的客户端的channel。
<a name="B5yH4"></a>
## `EpollEventLoopGroup`
先看下`EpollEventLoopGroup`的定义：
```java
public final class EpollEventLoopGroup extends MultithreadEventLoopGroup
```
和`KqueueEventLoopGroup`一样，`EpollEventLoopGroup`也是继承自`MultithreadEventLoopGroup`，表示它可以开启多个线程。<br />在使用`EpollEventLoopGroup`之前，需要确保`epoll`相关的JNI接口都已经准备完毕：
```java
Epoll.ensureAvailability();
```
`newChild`方法用来生成`EpollEventLoopGroup`的子`EventLoop`：
```java
protected EventLoop newChild(Executor executor, Object... args) throws Exception {
	Integer maxEvents = (Integer) args[0];
	SelectStrategyFactory selectStrategyFactory = (SelectStrategyFactory) args[1];
	RejectedExecutionHandler rejectedExecutionHandler = (RejectedExecutionHandler) args[2];
	EventLoopTaskQueueFactory taskQueueFactory = null;
	EventLoopTaskQueueFactory tailTaskQueueFactory = null;

	int argsLength = args.length;
	if (argsLength > 3) {
		taskQueueFactory = (EventLoopTaskQueueFactory) args[3];
	}
	if (argsLength > 4) {
		tailTaskQueueFactory = (EventLoopTaskQueueFactory) args[4];
	}
	return new EpollEventLoop(this, executor, maxEvents,
							  selectStrategyFactory.newSelectStrategy(),
							  rejectedExecutionHandler, taskQueueFactory, tailTaskQueueFactory);
}
```
从方法中可以看到，newChild接受一个executor和多个额外的参数，这些参数分别是`SelectStrategyFactory`，`RejectedExecutionHandler`，`taskQueueFactory`和`tailTaskQueueFactory`，最终将这些参数传入`EpollEventLoop`中，返回一个新的`EpollEventLoop`对象。
<a name="xJDwd"></a>
## `EpollEventLoop`
`EpollEventLoop`是由`EpollEventLoopGroup`通过使用`new child`方法来创建的。<br />对于`EpollEventLoop`本身来说，是一个`SingleThreadEventLoop`：
```java
class EpollEventLoop extends SingleThreadEventLoop
```
借助于native epoll IO的强大功能，`EpollEventLoop`可以在单线程的情况下快速进行业务处理，十分优秀。<br />和`EpollEventLoopGroup`一样，`EpollEventLoop`在初始化的时候需要检测系统是否支持epoll：
```java
static {
	Epoll.ensureAvailability();
}
```
在`EpollEventLoopGroup`调用的`EpollEventLoop`的构造函数中，初始化了三个`FileDescriptor`，分别是`epollFd`,`eventFd`和`timerFd`，这三个`FileDescriptor`都是调用`Native`方法创建的：
```java
this.epollFd = epollFd = Native.newEpollCreate();
this.eventFd = eventFd = Native.newEventFd();
this.timerFd = timerFd = Native.newTimerFd();
```
然后调用`Native.epollCtlAdd`建立`FileDescriptor`之间的关联关系：
```java
Native.epollCtlAdd(epollFd.intValue(), eventFd.intValue(), Native.EPOLLIN | Native.EPOLLET);
Native.epollCtlAdd(epollFd.intValue(), timerFd.intValue(), Native.EPOLLIN | Native.EPOLLET);
```
在`EpollEventLoop`的`run`方法中，首先会调用`selectStrategy.calculateStrategy`方法，拿到当前的`select`状态，默认情况下有三个状态，分别是：
```java
int SELECT = -1;

int CONTINUE = -2;

int BUSY_WAIT = -3;
```
这三个状态在kqueue中已经介绍过了，不同的是epoll支持BUSY_WAIT状态，在BUSY_WAIT状态下，会去调用`Native.epollBusyWait(epollFd, events)`方法返回busy wait的event个数。<br />如果是在select状态下，则会去调用`Native.epollWait(epollFd, events, 1000)`方法返回`wait`状态下的`event`个数。<br />接下来会分别调用`processReady(events, strategy)`和`runAllTasks`方法，进行event的ready状态回调处理和最终的任务执行。
<a name="PJnzZ"></a>
## `EpollServerSocketChannel`
先看下`EpollServerSocketChannel`的定义：
```java
public final class EpollServerSocketChannel extends AbstractEpollServerChannel implements ServerSocketChannel
```
`EpollServerSocketChannel`继承自`AbstractEpollServerChannel`并且实现了`ServerSocketChannel`接口。<br />`EpollServerSocketChannel`的构造函数需要传入一个`LinuxSocket`：
```java
EpollServerSocketChannel(LinuxSocket fd) {
	super(fd);
	config = new EpollServerSocketChannelConfig(this);
}
```
`LinuxSocket`是一个特殊的`socket`，用来处理和linux的`native socket`连接。<br />`EpollServerSocketChannelConfig`是构建`EpollServerSocketChannel`的配置，这里用到了4个配置选项，分别是SO_REUSEPORT,IP_FREEBIND,IP_TRANSPARENT,TCP_DEFER_ACCEPT和TCP_MD5SIG。每个配置项都对应着网络协议的特定含义。<br />再看一下`EpollServerSocketChannel`的`newChildChannel`方法：
```java
protected Channel newChildChannel(int fd, byte[] address, int offset, int len) throws Exception {
	return new EpollSocketChannel(this, new LinuxSocket(fd), address(address, offset, len));
}
```
`newChildChannel`和`KqueueServerSocketChannel`方法一样，也是返回一个`EpollSocketChannel`，并且将传入的fd构造成为`LinuxSocket`。
<a name="y91sK"></a>
## `EpollSocketChannel`
`EpollSocketChannel`是由`EpollServerSocketChannel`创建返回的，先来看下`EpollSocketChannel`的定义：
```java
public final class EpollSocketChannel extends AbstractEpollStreamChannel implements SocketChannel {
```
可以看到`EpollSocketChannel`继承自`AbstractEpollStreamChannel`，并且实现了`SocketChannel`接口。<br />回到之前`EpollServerSocketChannel`创建`EpollSocketChannel`时调用的`newChildChannel`方法，这个方法会调用`EpollSocketChannel`的构造函数如下所示：
```java
EpollSocketChannel(Channel parent, LinuxSocket fd, InetSocketAddress remoteAddress) {
	super(parent, fd, remoteAddress);
	config = new EpollSocketChannelConfig(this);

	if (parent instanceof EpollServerSocketChannel) {
		tcpMd5SigAddresses = ((EpollServerSocketChannel) parent).tcpMd5SigAddresses();
	}
}
```
从代码的逻辑可以看到，如果`EpollSocketChannel`是从`EpollServerSocketChannel`创建出来的话，那么默认会开启tcpMd5Sig的特性。<br />什么是tcpMd5Sig呢？<br />简单点说，tcpMd5Sig就是在TCP的数据报文中添加了MD5 sig，用来进行数据的校验，从而提示数据传输的安全性。<br />TCP MD5是在RFC 2385中提出的，并且只在Linux内核中才能开启，也就是说如果想使用tcpMd5Sig，那么必须使用`EpollServerSocketChannel`和`EpollSocketChannel`。<br />所以如果是追求性能或者特殊使用场景的朋友，需要接触这种`native transport`的时候还是很多的，可以仔细研究其中的配置选项。<br />再看一下`EpollSocketChannel`中非常重要的`doConnect0`方法：
```java
boolean doConnect0(SocketAddress remote) throws Exception {
	if (IS_SUPPORTING_TCP_FASTOPEN_CLIENT && config.isTcpFastOpenConnect()) {
		ChannelOutboundBuffer outbound = unsafe().outboundBuffer();
		outbound.addFlush();
		Object curr;
		if ((curr = outbound.current()) instanceof ByteBuf) {
			ByteBuf initialData = (ByteBuf) curr;
			long localFlushedAmount = doWriteOrSendBytes(
				initialData, (InetSocketAddress) remote, true);
			if (localFlushedAmount > 0) {
				outbound.removeBytes(localFlushedAmount);
				return true;
			}
		}
	}
	return super.doConnect0(remote);
}
```
在这个方法中会首先判断是否开启了`TcpFastOpen`选项，如果开启了该选项，那么最终会调用LinuxSocket的`write`或者`sendTo`方法，这些方法可以添加初始数据，可以在建立连接的同时传递数据，从而达到Tcp fast open的效果。<br />如果不是tcp fast open，那么需要调用Socket的`connect`方法去建立传统的连接。
<a name="GkJFC"></a>
## 总结
epoll在Netty中的实现和`kqueue`很类似，他们的不同在于运行的平台和具体的功能参数，如果追求高性能的朋友可以深入研究。
