JavaNetty<br />Netty 是一个在 Java 生态里应用非常广泛的的网络编程工具包，它在 2004 年诞生到现在依然是火的一塌糊涂，光在 github 上就有 30000 多个项目在用它。所以要想更好地掌握网络编程，就绕不开 Netty。所以就来分析分析 Netty 内部网络模块的工作原理。
<a name="SG7RO"></a>
## 一、Netty 用法
首先找一个 Netty 的例子，本篇文章整体都是围绕这个例子来展开叙述的。下载 Netty 的源码，并在 examples 中找到 echo 这个 demo。同时，为了防止代码更新导致对本文叙述的影响，切到 4.1 分支上来。
```java
# git checkout https://github.com/netty/netty.git
# git checkout -b 4.1
# cd example/src/main/java/io/netty/example/echo
```
在这个 demo 的 EchoServer 中，展示了使用 Netty 写 Server 的经典用法。（在文章中会在不影响核心逻辑的表达上，对原始代码尽心适当的精简，比如下面代码中的 try 就被丢了）
```java
public final class EchoServer {
	public static void main(String[] args) throws Exception {
		EventLoopGroup bossGroup = new NioEventLoopGroup(1);
		EventLoopGroup workerGroup = new NioEventLoopGroup();
		final EchoServerHandler serverHandler = new EchoServerHandler();

		ServerBootstrap b = new ServerBootstrap();
		b.group(bossGroup, workerGroup)
			.channel(NioServerSocketChannel.class)
			.option(ChannelOption.SO_BACKLOG, 100)
			.handler(new LoggingHandler(LogLevel.INFO))
			.childHandler(new ChannelInitializer<SocketChannel>() {
				@Override
				public void initChannel(SocketChannel ch) throws Exception {
					ChannelPipeline p = ch.pipeline();
					if (sslCtx != null) {
						p.addLast(sslCtx.newHandler(ch.alloc()));
					}
					p.addLast(serverHandler);
				}
			});

		// Start the server.
		ChannelFuture f = b.bind(PORT).sync();
		......
	}
}
```
如果是一个 Java 新手，或者没用 Netty 写过服务，相信上述代码基本是看不懂的。究其根本原因是相比 C/C++ ，Java 的封装程度比较高。Java 语言本身的 JVM 中 NIO 对网络的封装就已经屏蔽了很多底层的概念了，再加上 Netty 又封装了一层，所以 Java 开发者常用的一些术语和概念和其它语言出入很大。<br />比如上面代码中的 Channel、NioEventLoopGroup 等都是其它语言中所没见过的。不过也不用感到害怕，因为这其中的每一个概念都是 socket、进程等底层概念穿了一身不同的衣服而已。接下来分别细了解一下这些概念。
<a name="ggBLv"></a>
### 1.1 NioEventLoopGroup
如果没接触过 Netty，可以简单把 NioEventLoopGroup 理解为一个线程池就可以。每一个 NioEventLoopGroup 内部包含一个或者多个 NioEventLoop。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657896629316-07854874-3176-4cec-9117-a0ba645a2795.png#clientId=u30291186-4a7a-4&from=paste&id=u96bd1b01&originHeight=121&originWidth=531&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7a052359-fe20-47cd-9adf-0a0cf3e5b1d&title=)<br />其中 NioEventLoop 是对线程、epoll 等概念进行了一个集中的封装。<br />首先，EventLoop 本身就是一个线程。为什么这么说，通过看 NioEventLoop 的继承关系就能看出来。NioEventLoop 继承于 SingleThreadEventLoop，而 SingleThreadEventLoop 又继承于 SingleThreadEventExecutor。SingleThreadEventExecutor 实现了在 Netty 中对本地线程的抽象。
```java
public abstract class SingleThreadEventExecutor extends ... {
	private volatile Thread thread;
	private final Queue<Runnable> taskQueue;
}
```
在 SingleThreadEventExecutor 中不但封装了线程对象 Thread，而且还配置了一个任务队列 taskQueue，用于其它线程向它来放置待处理的任务。
<a name="mam2w"></a>
### 1.2 selector
另外 NioEventLoopEventLoop 以 selector 的名义封装了 epoll（在 Linux 操作系统下）。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657896629315-b82de4ce-7388-42be-9f7d-b6eb43dcb04d.png#clientId=u30291186-4a7a-4&from=paste&id=u0f155b64&originHeight=101&originWidth=481&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc8636bb7-83f6-4a0e-8c72-372de465bc9&title=)<br />在 NioEventLoop 对象内部，会有 selector 成员定义。这其实就是封装的 epoll 而来的。来看具体的封装过程。以及 selectedKeys，这是从 selector 上发现的待处理的事件列表。
```java
public final class NioEventLoop extends SingleThreadEventLoop{
	// selector
	private Selector selector;
	private Selector unwrappedSelector;

	// selector 上发现的各种待处理事件
	private SelectedSelectionKeySet selectedKeys;
}
```
NioEventLoopGroup 在构造的时候，会调用 `SelectorProvider#provider` 来生成 provider，在默认情况下会调用 `sun.nio.ch.DefaultSelectorProvider.create` 来创建。
```java
//file:java/nio/channels/spi/SelectorProvider.java
public abstract class SelectorProvider {

    public static SelectorProvider provider() {
     // 1. java.nio.channels.spi.SelectorProvider 属性指定实现类
        // 2. SPI 指定实现类
        ......

        // 3. 默认实现，Windows 和 Linux 下不同
        provider = sun.nio.ch.DefaultSelectorProvider.create();
        return provider;
    }
}
```
在 Linux 下，默认创建的 provider 使用的就是 epoll。
```java
//file:sun/nio/ch/DefaultSelectorProvider.java
public class DefaultSelectorProvider {
	public static SelectorProvider create() {
		String osname = AccessController
			.doPrivileged(new GetPropertyAction("os.name"));
		if (osname.equals("Linux"))
			return createProvider("sun.nio.ch.EPollSelectorProvider");
	}

}
```
<a name="K1uZs"></a>
### 1.3 Channel
Channel 是 JavaNIO 里的一个概念。大家把它理解成 socket，以及在 socket 之上的一系列操作方法的封装就可以了。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657896629310-2a1e4b04-77f6-4199-898e-bc2b152ee384.png#clientId=u30291186-4a7a-4&from=paste&id=ue058f58b&originHeight=170&originWidth=419&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6a4fed17-d5d2-4c35-983a-b9e82595e54&title=)<br />Java 在 Channel 中把 connect、bind、read、write 等方法都以成员方法的形式给封装起来了。
```java
public interface Channel extends ... {
	Channel read();
	Channel flush();
	......

	interface Unsafe {
		void bind(SocketAddress localAddress, ...);
		void connect(SocketAddress remoteAddress, ...);
		void write(Object msg, ...);
		......
	}
}
```
另外在 Java 中，习惯把 listen socket 叫做父 channel，客户端握手请求到达以后创建出来的新连接叫做子 channel，方便区分。
<a name="cLYZ8"></a>
### 1.4 Pipeline
在每个 Channel 对象的内部，除了封装了 socket 以外，还都一个特殊的数据结构 DefaultChannelPipeline pipeline。在这个 pipeline 里是各种时机里注册的 handler。<br />Channel 上的读写操作都会走到这个 DefaultChannelPipeline 中，当 channel 上完成 register、active、read、readComplete 等操作时，会触发 pipeline 中的相应方法。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657896629386-e69b8137-e044-4712-a065-b5e7bb343dcf.png#clientId=u30291186-4a7a-4&from=paste&id=uc42b1523&originHeight=123&originWidth=541&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0ff400c3-bd9c-42a5-b378-2780de5556e&title=)<br />这个 ChannelPipeline 其实就是一个双向链表，以及链表上的各式各样的操作方法。
```java
public interface ChannelPipeline {
	ChannelPipeline addFirst(String name, ChannelHandler handler);
	ChannelPipeline addBefore(String baseName, String name, ChannelHandler handler);
	ChannelPipeline addAfter(String baseName, String name, ChannelHandler handler);
	ChannelPipeline addLast(String name, ChannelHandler handler);

	ChannelPipeline fireChannelRead(Object msg);
}
```
<a name="CEhDk"></a>
### 1.5 EchoServer 解读
现在具备了对 Java、对 Netty 的初步理解以后，再后来看一下开篇提到的 EchoServer 源码。
```java
public final class EchoServer {
	public static void main(String[] args) throws Exception {
		EventLoopGroup bossGroup = new NioEventLoopGroup(1);
		EventLoopGroup workerGroup = new NioEventLoopGroup();
		final EchoServerHandler serverHandler = new EchoServerHandler();

		ServerBootstrap b = new ServerBootstrap();
		b.group(bossGroup, workerGroup)
			.channel(NioServerSocketChannel.class)
			.option(ChannelOption.SO_BACKLOG, 100)
			.handler(new LoggingHandler(LogLevel.INFO))
			.childHandler(new ChannelInitializer<SocketChannel>() {
				@Override
				public void initChannel(SocketChannel ch) throws Exception {
					ChannelPipeline p = ch.pipeline();
					if (sslCtx != null) {
						p.addLast(sslCtx.newHandler(ch.alloc()));
					}
					p.addLast(serverHandler);
				}
			});

		// Start the server.
		ChannelFuture f = b.bind(PORT).sync();
		......
	}
}
```
在该代码一开头，`bossGroup = new NioEventLoopGroup(1)` 这一行是创建了一个只有一个线程的线程池。`workerGroup = new NioEventLoopGroup` 又创建了 worker 线程池，没有指定数量，Netty 内部会根据当前机器的 CPU 核数来灵活决定。<br />ServerBootstrap 这是一个脚手架类，是为了写起服务器程序来更方便一些。<br />`b.group(bossGroup, workerGroup)` 这一行是将两个线程池传入，第一个作为 boss 只处理 accept 接收新的客户端连接请求。第二个参数作为 worker 线程池，来处理连接上的请求接收、处理以及结果发送发送。<br />注意下 childHandler是传入了一个 ChannelInitializer，这是当有新的客户端连接到达时会回调的一个方法。在这个方法内部，给这个新的 chaneel 的 pipeline 上添加了一个处理器 serverHandler，以便收到数据的时候执行该处理器进行请求处理。<br />上面的几个方法都是定义，在 `b.bind` 方法中真正开始启动服务，创建父 channel（listen socket），创建 boss 线程。当有新连接到达的时候 boss 线程再创建子 channel，为其 pipeline 添加处理器，并启动 worker 线程来进行处理。
<a name="oIqBI"></a>
## 二、Netty bootstrap 参数构建
简言之 `bootstrap.group().channel().childHandler().childOption()` 就是在构建 Netty Server 的各种参数。
<a name="i0jHn"></a>
### 2.1 group 设置
ServerBootstrap 和其父类 AbstractBootstrap 内部分别定义了两个 EventLoopGroup group 成员。父类 AbstractBootstrap 的 group 是用来处理 accpet 事件的，ServerBootstrap 下的 childGroup 用来处理其它所有的读写等事件。<br />`group()` 方法就是把 EventLoopGroup 参数设置到自己的成员上完事。其中如果调用 group() 只传入了一个线程池，那么将来本服务下的所有事件都由这个线程池来处理。详情查看精简后的源码。
```java
//file:io/netty/bootstrap/ServerBootstrap.java
public class ServerBootstrap extends AbstractBootstrap<ServerBootstrap, ServerChannel> {
	//用来处理非 accept 以外的线程池
	private volatile EventLoopGroup childGroup;
	
	public ServerBootstrap group(EventLoopGroup group) {
		return group(group, group);
	}
	
	public ServerBootstrap group(EventLoopGroup parentGroup, EventLoopGroup childGroup) {
		super.group(parentGroup);
		this.childGroup = ObjectUtil.checkNotNull(childGroup, "childGroup");
		return this;
	}
}

public abstract class AbstractBootstrap ... {
	//用来处理 accept 的线程
	volatile EventLoopGroup group;
	
	public B group(EventLoopGroup group) {
		this.group = group;
		......
		}
}
```
<a name="WG4lW"></a>
### 2.2 channel 设置
再看 `ServerBootstrap#channel` 方法 是用来定义一个工厂方法，将来需要创建 channel 的时候都调用该工厂进行创建。
```java
//file:io/netty/bootstrap/ServerBootstrap.java
public class ServerBootstrap extends AbstractBootstrap<ServerBootstrap, ServerChannel> {

 public B channel(Class<? extends C> channelClass) {
        return channelFactory(new ReflectiveChannelFactory<C>(
                ObjectUtil.checkNotNull(channelClass, "channelClass")
        ));
    }
}
```
回头看本文开头 demo，`.channel(NioServerSocketChannel.class)`指的是将来需要创建 channel 的时候，创建 NioServerSocketChannel 这个类型的。
<a name="R4awS"></a>
### 2.3 option 设置
再看 option 方法，只是设置到了 options 成员中而已
```java
//file:io/netty/bootstrap/ServerBootstrap.java
public class ServerBootstrap extends AbstractBootstrap<ServerBootstrap, ServerChannel> {

    public <T> B option(ChannelOption<T> option, T value) {
        ObjectUtil.checkNotNull(option, "option");
        synchronized (options) {
            if (value == null) {
                options.remove(option);
            } else {
                options.put(option, value);
            }
        }
        return self();
    }
}
```
<a name="vv6Yq"></a>
### 2.4 handler 方法
本文 demo 设置了两处 handler，一处是 handler，另一处是 childHandler。他们都是分别设置到自己的成员上就完事，看源码。
```java
//file:io/netty/bootstrap/ServerBootstrap.java
public class ServerBootstrap extends ...... {

    public B handler(ChannelHandler handler) {
        this.handler = ObjectUtil.checkNotNull(handler, "handler");
        return self();
    }
    public ServerBootstrap childHandler(ChannelHandler childHandler) {
        this.childHandler = ObjectUtil.checkNotNull(childHandler, "childHandler");
        return this;
    }
}
```
<a name="CPgI3"></a>
## 三、Netty bootstrap 启动服务
ServerBootstrap 下的 bind 方法是服务启动过程中非常重要的一个方法。创建父 channel（listen socket），创建 boss 线程，为 boss 线程绑定 Acceptor 处理器，调用系统调用 bind 进行绑定和监听都是在这里完成的。<br />先来直接看一下 bind 相关的入口源码。
```java
//file:io/netty/bootstrap/ServerBootstrap.java
public class ServerBootstrap extends AbstractBootstrap ... {
 ......
}

//file:io/netty/bootstrap/AbstractBootstrap.java
public abstract class AbstractBootstrap ... {

    public ChannelFuture bind(SocketAddress localAddress) {
        validate();
        return doBind(...);
    }

    private ChannelFuture doBind(final SocketAddress localAddress) {

     //创建父 channel、初始化并且注册
        final ChannelFuture regFuture = initAndRegister();
        final Channel channel = regFuture.channel();
        ......

        //如果 Register 已经完成，则直接 doBind0
        if (regFuture.isDone()) {
         ChannelPromise promise = channel.newPromise();
         doBind0(regFuture, channel, localAddress, promise);
         return promise;
        //否则就注册一个 listener（回调），等 register 完成的时候调用 
        } else {
         final PendingRegistrationPromise promise = new PendingRegistrationPromise(channel);
             regFuture.addListener(new ChannelFutureListener() {
                @Override
                public void operationComplete(ChannelFuture future) throws Exception {
                 promise.registered();
                    doBind0(regFuture, channel, localAddress, promise);
                }
            return promise;
        }
        
    }

    //创建 channel，对其初始化，并且 register（会创建 parent 线程）
    final ChannelFuture initAndRegister() {
     //3.1 创建父 channel（listen socket）
        channel = channelFactory.newChannel();

        //3.2 对父 channel（listen socket）进行初始化
        init(channel);

        //3.3 注册并启动 boss 线程
        ChannelFuture regFuture = config().group().register(channel);
        ......
    }

    //3.4 真正的bind
    private static void doBind0(...) {
        channel.eventLoop().execute(new Runnable() {
            @Override
            public void run() {
                channel.bind(localAddress, promise).addListener(ChannelFutureListener.CLOSE_ON_FAILURE);
                ......
            }
        });
    }
}
```
在这个过程中，做了如下几件重要的事情

- **创建父 channel（listen socket）**
- **对父 channel（listen socket）进行初始化**
- **register父 channel（listen socket）到主 group，并启动主进程**
- **真正的 bind**

接下来分开来看。
<a name="VmqGo"></a>
### 3.1 创建父 channel（listen socket）
在 `initAndRegister()` 方法中创建 channel（socket），它调用了 `channelFactory.newChannel()`。
```java
public abstract class AbstractBootstrap
    //创建 channel，对其初始化，并且 register（会创建 parent 线程）
    final ChannelFuture initAndRegister() {
     //3.1 创建 listen socket
        channel = channelFactory.newChannel();
        ......
    }
}
```
回想下 2.2 节的channel 方法，返回的是一个反射 `ReflectiveChannelFactory`。没错这里的 `newChannel` 就是调用这个工厂方法来创建出来一个 `NioServerSocketChannel` 对象。
<a name="Tn6OF"></a>
### 3.2 对父 channel（listen socket）进行初始化
在 initAndRegister 创建除了 channel 之后，需要调用 init 对其进行初始化。
```java
public abstract class AbstractBootstrap
    final ChannelFuture initAndRegister() {
     //3.1 创建父 channel（listen socket）
        //3.2 对父 channel（listen socket）进行初始化
        init(channel);
        ......
    }
}
```
在 `init()` 中对 channel 进行初始化，一是给 options 和 attrs 赋值，二是构建了父 channel 的 pipeline。
```java
//file:src/main/java/io/netty/bootstrap/ServerBootstrap.java
public class ServerBootstrap extends AbstractBootstrap<ServerBootstrap, ServerChannel> {

 void init(Channel channel) {
  //设置 option 和 attr
        setChannelOptions(channel, newOptionsArray(), logger);
        setAttributes(channel, newAttributesArray());

        //设置 pipeline
        ChannelPipeline p = channel.pipeline();
        p.addLast(new ChannelInitializer<Channel>() {
           ......
        });
    }
}
```
在 setChannelOptions 中对 channel 的各种 option 进行设置。回忆在使用 ServerBootstrap 时可以传入 SO_BACKLOG，这就是其中的一个 option。在这里会真正设置到 channel（socket）上。
```java
ServerBootstrap b = new ServerBootstrap();
b.option(ChannelOption.SO_BACKLOG, 100)
```
在 init 中，稍微难理解一点是 `p.addLast(new ChannelInitializer...)`。这一段代码只是给父 channel 添加一个 handler 而已。其真正的执行要等到 register 后，待会再看。
<a name="TMIEb"></a>
### 3.3 register 父 channel
父 channel 在创建完，并且初始化之后，需要注册到 boss 线程上才可用。
```java
public abstract class AbstractBootstrap
    final ChannelFuture initAndRegister() {
     //3.1 创建父 channel（listen socket）
        //3.2 对父 channel（listen socket）进行初始化
        //3.3 注册并启动 boss 线程
        ChannelFuture regFuture = config().group().register(channel);
        ......
    }
}
```
其中 `config().group()` 最终会调用到 `AbstractBootstrap#group`，在这个方法里获取的是传入进来的 bossGroup。
```java
public abstract class AbstractBootstrap
    volatile EventLoopGroup group;
    public final EventLoopGroup group() {
        return group;
    }
}
```
其中 bossGroup 是一个 NioEventLoopGroup 实例，所以代码会进入到 NioEventLoopGroup#register 方法。
```java
public class NioEventLoopGroup extends MultithreadEventLoopGroup {}

public abstract class MultithreadEventLoopGroup extends ... {

 @Override
    public ChannelFuture register(Channel channel) {
        return next().register(channel);
    }

    @Override
    public EventLoop next() {
        return (EventLoop) super.next();
    }
}
```
在 NioEventLoopGroup 里包含一个或多个 EventLoop。上面的 next 方法就是从中选择一个出来，然后将 channel 注册到其上。<br />对于本文来讲，使用的是 NioEventLoopGroup，其内部包含的自然也就是 NioEventLoop，继续查找其 register 方法。
```java
public final class NioEventLoop extends SingleThreadEventLoop
 //在 eventloop 里注册一个 channle（socket）
 public void register(final SelectableChannel ch, ...) {
  ......
        register0(ch, interestOps, task);
 }

 //最终调用 channel 的 register
 private void register0(SelectableChannel ch, int interestOps, NioTask<?> task) {
        ch.register(unwrappedSelector, interestOps, task);
    }
}
```
可见，NioEventLoop 的 register 最后又调用到 channel 的 register 上了。在本文中，创建的 channel 是 NioServerSocketChannel，就依照这条线索来查。
```java
//file:src/main/java/io/netty/channel/AbstractChannel.java
public abstract class AbstractChannel extends DefaultAttributeMap implements Channel {

    public final void register(EventLoop eventLoop, final ChannelPromise promise) {
        ......

        //关联自己到 eventLoop
        AbstractChannel.this.eventLoop = eventLoop;

        if (eventLoop.inEventLoop()) {
            register0(promise);
        } else {
         try {
                eventLoop.execute(new Runnable() {
                    @Override
                    public void run() {
                        register0(promise);
                    }
                });
            }
         ......
        }
    }
}
```
在 channel 的父类 AbstractChannel 中的 register 中，先是把自己关联到传入的 eventLoop 上。接着调用 inEventLoop 来判断线程当前运行的线程是否是 EventExecutor的支撑线程，是则返回直接 register0。<br />一般来说，服务在启动的时候都是主线程在运行。这个时候很可能 boss 线程还没有启动。所以如果发现当前不是 boss 线程的话，就调用 `eventLoop.execute` 来启动 boss 线程。<br />NioEventLoop 的父类是 `SingleThreadEventExecutor`，找到 `execute` 方法。
```java
public abstract class SingleThreadEventExecutor extends AbstractScheduledEventExecutor implements OrderedEventExecutor {

    public void execute(Runnable task) {
        execute0(task);
    }

    private void execute0(@Schedule Runnable task) {
        execute(task, !(task instanceof LazyRunnable) && wakesUpForTask(task));
    }

    private void execute(Runnable task, boolean immediate) {
        boolean inEventLoop = inEventLoop();
        addTask(task);
        if (!inEventLoop) {
            startThread();
        }

        if (!addTaskWakesUp && immediate) {
            wakeup(inEventLoop);
        }
    }
}
```
先来看 `addTask(task)`，它是将 task 添加到任务队列中。等待线程起来以后再运行。
```java
public abstract class SingleThreadEventExecutor extends ... {

 private final Queue<Runnable> taskQueue;
 protected void addTask(Runnable task) {
        (task);
    }
 final boolean offerTask(Runnable task) {
        return taskQueue.offer(task);
    }
}
```
`inEventLoop()` 是判断当前线程是不是自己绑定的线程，这时还在主线程中运行，所以 inEventLoop 为假，会进入 startThread 开始为 EventLoop 创建线程。
```java
public abstract class SingleThreadEventExecutor extends AbstractScheduledEventExecutor implements OrderedEventExecutor {

    private void startThread() {
        doStartThread();
        ......
    }

    private void doStartThread() {
        executor.execute(new Runnable() {
            @Override
            public void run() {
                SingleThreadEventExecutor.this.run();
                ......
            }
        }
    }  
}
```
在 doStartThread 中调用 Java 线程管理工具 Executor 来启动 boss 线程。
<a name="LO8op"></a>
### 3.4 boss 线程启动
当线程起来以后就进入了自己的线程循环中了，会遍历自己的任务队列，然后开始处理自己的任务。
```java
public final class NioEventLoop extends SingleThreadEventLoop {

    protected void run() {
        for (;;) {
            if (!hasTasks()) {
                strategy = select(curDeadlineNanos);
            }

            //如果有任务的话就开始处理
            runAllTasks(0);

            //任务处理完毕就调用 epoll_wait 等待事件发生
            processSelectedKeys();
        }
    }
}
```
前面在 3.3 节看到 `eventLoop.execute` 把一个 Runnable 任务添加到了任务队列里。当 EventLoop 线程启动后，它会遍历自己的任务队列并开始处理。这时会进入到 `AbstractChannel#register0` 方法开始运行。
```java
//file:src/main/java/io/netty/channel/AbstractChannel.java
public abstract class AbstractChannel extends ... {

    public final void register(...) {
        eventLoop.execute(new Runnable() {
            @Override
            public void run() {
                register0(promise);
            }
        });
        ......
    }
    private void register0(ChannelPromise promise) {
        doRegister();
        ......
    }
}
```
函数 `doRegister` 是在 `AbstractNioChannel` 类下。
```java
//file:io/netty/channel/nio/AbstractNioChannel.java
public abstract class AbstractNioChannel extends AbstractChannel {

    private final SelectableChannel ch;

    protected SelectableChannel javaChannel() {
        return ch;
    }
    public NioEventLoop eventLoop() {
        return (NioEventLoop) super.eventLoop();
    }
    protected void doRegister() throws Exception {
        boolean selected = false;
        for (;;) {
            selectionKey = javaChannel().register(eventLoop().unwrappedSelector(), 0, this);            
        }
    }
}
```
上面最关键的一句是 `selectionKey = javaChannel().register(eventLoop().unwrappedSelector(), 0, this);`。这一句就相当于在 C 语言下调用 epoll_ctl 把 listen socket 添加到了 epoll 对象下。<br />其中 javaChannel 获取父 channel，相当于 listen socket。unwrappedSelector 获取 selector，相当于 epoll 对象。register 相当于使用 epoll_ctl 执行 add 操作。<br />当 channel 注册完后，前面 init 时注册的 ChannelInitializer 回调就会被执行。再回头看它的 回调定义。
```java
//file:src/main/java/io/netty/bootstrap/ServerBootstrap.java
public class ServerBootstrap extends AbstractBootstrap<ServerBootstrap, ServerChannel> {

 void init(Channel channel) 
  ......

        p.addLast(new ChannelInitializer<Channel>() {
            @Override
            public void initChannel(final Channel ch) {
             ......
                ch.eventLoop().execute(new Runnable() {
                    @Override
                    public void run() {
                        pipeline.addLast(new ServerBootstrapAcceptor(
                                ch, currentChildGroup, currentChildHandler, currentChildOptions, currentChildAttrs));
                    }
                });
            }
        });
    }
}
```
在 `ChannelInitializer#initChannel` 里，又给 boss 线程的 pipeline 里添加了一个任务。该任务是让其在自己的 pipeline 上注册一个 ServerBootstrapAcceptor handler。将来有新连接到达的时候，ServerBootstrapAcceptor 将会被执行。
<a name="C2D82"></a>
### 3.5 真正的 bind
再看 `doBind0` 方法，调用 `channel.bind` 完成绑定。
```java
private static void doBind0(...) {
	channel.eventLoop().execute(new Runnable() {
		@Override
		public void run() {
			channel.bind(localAddress, promise).addListener(ChannelFutureListener.CLOSE_ON_FAILURE);
			......
			}
	});
}
```
<a name="oQM6U"></a>
## 四、新连接到达
再回到 boss 线程的主循环中。
```java
public final class NioEventLoop extends SingleThreadEventLoop {
    protected void run() {
        for (;;) {
            strategy = selectStrategy.calculateStrategy(selectNowSupplier, hasTasks());

            //任务队列都处理完就开始 select
            if (!hasTasks()) {
                strategy = select(curDeadlineNanos);
            }
            //处理各种事件
            if (strategy > 0) {
                processSelectedKeys();
            }
        }  
    }
    
    private int select(long deadlineNanos) throws IOException {
        if (deadlineNanos == NONE) {
            return selector.select();
        }
        // Timeout will only be 0 if deadline is within 5 microsecs
        long timeoutMillis = deadlineToDelayNanos(deadlineNanos + 995000L) / 1000000L;
        return timeoutMillis <= 0 ? selector.selectNow() : selector.select(timeoutMillis);
    }
}
```
假如线程任务队列中的任务都处理干净了的情况下，boss 线程会调用 select 来发现其 selector 上的各种事件。相当于 C 语言中的 epoll_wait。<br />当发现有事件发生的时候，例如 OP_WRITE、OP_ACCEPT、OP_READ 等的时候，会进入相应的处理
```java
public final class NioEventLoop extends SingleThreadEventLoop {
    private void processSelectedKey(SelectionKey k, AbstractNioChannel ch) {
     ......
        if ((readyOps & SelectionKey.OP_WRITE) != 0) {
         ch.unsafe().forceFlush();
     }
     if ((readyOps & (SelectionKey.OP_READ | SelectionKey.OP_ACCEPT)) != 0 || readyOps == 0) {
         unsafe.read();
     }
    }
}
```
对于服务端的 `Unsafe.read()`  这里会执行 `io.netty.channel.nio.AbstractNioMessageChannel.NioMessageUnsafe.read()` 方法，它会调用 JDK 底层的 `ServerSocketChannel.accept()` 接收到客户端的连接后，将其封装成 Netty 的 NioSocketChannel，再通过 Pipeline 将 ChannelRead 事件传播出去，这样 ServerBootstrapAcceptor 就可以在 ChannelRead 回调里处理新的客户端连接了。<br />直接看 `ServerBootstrapAcceptor#ChannelRead`。
```java
//file:
public class ServerBootstrap extends AbstractBootstrap<ServerBootstrap, ServerChannel> {
 
 ......

    private static class ServerBootstrapAcceptor extends ChannelInboundHandlerAdapter {

        public void channelRead(ChannelHandlerContext ctx, Object msg) {
         // 获取child channel
            final Channel child = (Channel) msg;

            // 设置 childHandler 到 child channel
            child.pipeline().addLast(childHandler);

            // 设置 childOptions、 childAttrs
            setChannelOptions(child, childOptions, logger);
            setAttributes(child, childAttrs);

            // 将 child channel 注册到 childGroup
            childGroup.register(child).addListener(new ChannelFutureListener() {
                @Override
                public void operationComplete(ChannelFuture future) throws Exception {
                    if (!future.isSuccess()) {
                        forceClose(child, future.cause());
                    }
                }
            });
        }
    }
}
```
在 channelRead 先是获取到了新创建出来的子 channel，并为其 pipeline 添加 childHandler。回头看 1.5 节，childHandler 是自定义的。<br />紧接着调用 `childGroup.register(child)` 将子 channel 注册到 workerGroup 上。这个 register 过程和 3.3节、3.5节过程一样。区别就是前面是父 channel 注册到 bossGroup 上，这里是子 channel 注册到 workerGroup上。<br />在 register 完成后，子 channel 被挂到了 workerGroup 其中一个线程上，相应的线程如果没有创建也会被创建出来并进入到自己的线程循环中。<br />当子 channel 注册完毕的时候，childHandler 中 `ChannelInitializer#initChannel` 会被执行
```java
public final class EchoServer {
	public static void main(String[] args) throws Exception {
		...
			ServerBootstrap b = new ServerBootstrap();
		b.childHandler(new ChannelInitializer<SocketChannel>() {
			@Override
			public void initChannel(SocketChannel ch) throws Exception {
				ChannelPipeline p = ch.pipeline();
				if (sslCtx != null) {
					p.addLast(sslCtx.newHandler(ch.alloc()));
				}
				p.addLast(serverHandler);
			}
		});
		......
	}
}
```
在 initChannel 把子 channel 的处理类 serverHandler 添加上来了。Netty demo 中对这个处理类的定义非常的简单，仅仅只是打印出来而已。
```java
public class EchoServerHandler extends ChannelInboundHandlerAdapter {
    public void channelRead(......) {
        ctx.write(msg);
    }
    ......
}
```
<a name="H0otc"></a>
## 五、用户请求到达
当 worker 线程起来以后，会进入线程循环(boss 线程和 worker 线程的 run 函数是一个)。在循环中会遍历自己的任务队列，如果没有任务可处理，便 select 来观察自己所负责的 channel 上是否有事件发生。
```java
public final class NioEventLoop extends SingleThreadEventLoop {

    protected void run() {
        for (;;) {
            if (!hasTasks()) {
                strategy = select(curDeadlineNanos);
            }

            //如果有任务的话就开始处理
            runAllTasks(0);

            //任务处理完毕就调用 epoll_wait 等待事件发生
            processSelectedKeys();
        }
    }
    private int select(long deadlineNanos) throws IOException {
        selector.selectNow();
        ......    
    }
}
```
worker 线程会调用 select 发现自己所管理的所有子 channel 上的可读可写事件。在发现有可读事件后，会调用 `processSelectedKeys`，最后触发 `pipeline` 使得 `EchoServerHandler` 方法开始执行。
```java
public class EchoServerHandler extends ChannelInboundHandlerAdapter {
    public void channelRead(......) {
        ctx.write(msg);
    }
    ......
}
```
<a name="Z2mjM"></a>
## 六、总结
事实上，Netty 对网络封装的比较灵活。既支持单线程 Reactor，也支持多线程 Reactor、还支持主从多线程 Reactor。三种模型对应的用法如下：
```java
public static void main(String[] args) throws Exception {
	//单线程 Reactor
	EventLoopGroup eventGroup = new NioEventLoopGroup(1);
	ServerBootstrap serverBootstrap = new ServerBootstrap(); 
	serverBootstrap.group(eventGroup);    
	......

		//多线程 Reactor
		EventLoopGroup eventGroup = new NioEventLoopGroup();
	ServerBootstrap serverBootstrap = new ServerBootstrap(); 
	serverBootstrap.group(eventGroup);    
	......

		//主从多线程 Reactor
		EventLoopGroup bossGroup = new NioEventLoopGroup(1); 
	EventLoopGroup workerGroup = new NioEventLoopGroup();
	ServerBootstrap serverBootstrap = new ServerBootstrap(); 
	serverBootstrap.group(bossGroup, workerGroup);    
	......
}
```
为了表述的更全面，本文选择的是最为经典的 主从多线程 Reactor 模式。本文中所描述的内容可以用下面一幅图来表示。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657896629383-51e4fa14-7663-4628-88a0-c16e85bb6876.png#clientId=u30291186-4a7a-4&from=paste&id=u8f34184a&originHeight=498&originWidth=564&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uda3cef39-ae45-4659-b241-c8361529522&title=)<br />在 Netty 中的 boss 线程中负责对父 channel（listen socket）上事件的监听和处理，当有新连接到达的时候，选择一个 worker 线程把这个子 channel（连接 socket ）叫给 worker 线程来处理。<br />其中 Worker 线程就是等待其管理的所有子 channel（连接 socket）上的事件的监听和处理。当发现有事件发生的时候，回调用户设置的 handler 进行处理。在本文的例子中，这个用户 handler 就是 `EchoServerHandler#channelRead`。<br />至此，Netty 网络模块的工作核心原理咱们就介绍完了。一直“鼓吹”内功的好处。只要具备了坚实的内功，各种语言里看似风牛马不相及的东西，在底层实际上原理是想通的。对epoll有了深入理解的时候，再看Netty也能很容易看懂，很快就能理解它的核心。这就是锻炼内功的好处！
