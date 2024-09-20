Java Netty
<a name="D1UzR"></a>
## 前言
在实现TCP长连接功能中，客户端断线重连是一个很常见的问题，当使用netty实现断线重连时，是否考虑过如下几个问题：

- 如何监听到客户端和服务端连接断开 ?
- 如何实现断线后重新连接 ?
- netty客户端线程给多大比较合理 ?

其实上面都在做断线重连时所遇到的问题，而 “netty客户端线程给多大比较合理?” 这个问题更是在做断线重连时因一个异常引发的思考。下面讲讲整个过程：<br />内容主要涉及在客户端，但是为了能够运行整个程序，所以这里先给出服务端及公共的依赖和实体类。
<a name="sXWFe"></a>
## 相关代码
<a name="HGhEK"></a>
### 服务端及common代码
maven依赖：
```xml
<dependencies>
    <!--只是用到了spring-boot的日志框架-->
    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter</artifactId>
        <version>2.4.1</version>
    </dependency>

    <dependency>
        <groupId>io.netty</groupId>
        <artifactId>netty-all</artifactId>
        <version>4.1.56.Final</version>
    </dependency>

    <dependency>
        <groupId>org.jboss.marshalling</groupId>
        <artifactId>jboss-marshalling-serial</artifactId>
        <version>2.0.10.Final</version>
    </dependency>
</dependencies>
```
<a name="F7gov"></a>
### 服务端业务处理代码
主要用于记录打印当前客户端连接数，当接收到客户端信息后返回“hello netty”字符串
```java
@ChannelHandler.Sharable
public class SimpleServerHandler extends ChannelInboundHandlerAdapter {
    private static final InternalLogger log = InternalLoggerFactory.getInstance(SimpleServerHandler.class);
    public static final ChannelGroup channels = new DefaultChannelGroup(GlobalEventExecutor.INSTANCE);

    @Override
    public void channelActive(ChannelHandlerContext ctx) throws Exception {
        channels.add(ctx.channel());
        log.info("客户端连接成功: client address :{}", ctx.channel().remoteAddress());
        log.info("当前共有{}个客户端连接", channels.size());
    }

    @Override
    public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
        log.info("server channelRead:{}", msg);
        ctx.channel().writeAndFlush("hello netty");
    }

    @Override
    public void channelInactive(ChannelHandlerContext ctx) throws Exception {
        log.info("channelInactive: client close");
    }

    @Override
    public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) throws Exception {
        if (cause instanceof java.io.IOException) {
            log.warn("exceptionCaught: client close");
        } else {
            cause.printStackTrace();
        }
    }
}
```
<a name="ILBcO"></a>
### 服务端心跳检查代码
当接收心跳"ping"信息后，返回客户端’'pong"信息。如果客户端在指定时间内没有发送任何信息则关闭客户端。
```java
public class ServerHeartbeatHandler extends ChannelInboundHandlerAdapter {
    private static final InternalLogger log = InternalLoggerFactory.getInstance(ServerHeartbeatHandler.class);

    @Override
    public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
        log.info("server channelRead:{}", msg);
        if (msg.equals("ping")) {
            ctx.channel().writeAndFlush("pong");
        } else {
            //由下一个handler处理,示例中则为SimpleServerHandler
            ctx.fireChannelRead(msg);
        }
    }

    @Override
    public void userEventTriggered(ChannelHandlerContext ctx, Object evt) throws Exception {
        if (evt instanceof IdleStateEvent) {
            //该事件需要配合 io.netty.handler.timeout.IdleStateHandler使用
            IdleStateEvent idleStateEvent = (IdleStateEvent) evt;
            if (idleStateEvent.state() == IdleState.READER_IDLE) {
                //超过指定时间没有读事件,关闭连接
                log.info("超过心跳时间,关闭和服务端的连接:{}", ctx.channel().remoteAddress());
                //ctx.channel().close();
            }
        } else {
            super.userEventTriggered(ctx, evt);
        }
    }
}
```
<a name="Ns3nw"></a>
### 编解码工具类
主要使用jboss-marshalling-serial编解码工具，可自行查询其优缺点，这里只是示例使用。
```java
public final class MarshallingCodeFactory {
    /** 创建Jboss marshalling 解码器 */
    public static MarshallingDecoder buildMarshallingDecoder() {
        //参数serial表示创建的是Java序列化工厂对象,由jboss-marshalling-serial提供
        MarshallerFactory factory = Marshalling.getProvidedMarshallerFactory("serial");
        MarshallingConfiguration configuration = new MarshallingConfiguration();
        configuration.setVersion(5);
        DefaultUnmarshallerProvider provider = new DefaultUnmarshallerProvider(factory, configuration);
        return new MarshallingDecoder(provider, 1024);
    }

    /** 创建Jboss marshalling 编码器 */
    public static MarshallingEncoder buildMarshallingEncoder() {
        MarshallerFactory factory = Marshalling.getProvidedMarshallerFactory("serial");
        MarshallingConfiguration configuration = new MarshallingConfiguration();
        configuration.setVersion(5);
        DefaultMarshallerProvider provider = new DefaultMarshallerProvider(factory, configuration);
        return new MarshallingEncoder(provider);
    }
}
```
<a name="CJWbj"></a>
### 公共实体类
```java
public class UserInfo implements Serializable {
    private static final long serialVersionUID = 6271330872494117382L;
 
    private String username;
    private int age;

    public UserInfo() {
    }

    public UserInfo(String username, int age) {
        this.username = username;
        this.age = age;
    }
   //省略getter/setter/toString
}
```
下面开始客户端断线重连以及问题思考。
<a name="GzbEx"></a>
### 客户端实现

- 刚开始启动时需要进行同步连接，指定连接次数内没用通过则抛出异常，进程退出。
- 客户端启动后，开启定时任务，模拟客户端数据发送。

客户端业务处理handler，接收到数据后，通过日志打印。
```java
public class SimpleClientHandler extends ChannelInboundHandlerAdapter {
    private static final InternalLogger log = InternalLoggerFactory.getInstance(SimpleClientHandler.class);
    private NettyClient client;

    public SimpleClientHandler(NettyClient client) {
        this.client = client;
    }

    @Override
    public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
        log.info("client receive:{}", msg);
    }
}
```
封装连接方法、断开连接方法、`getChannel()`返回`io.netty.channel.Channel`用于向服务端发送数据。`boolean connect()`是一个同步连接方法，如果连接成功返回true，连接失败返回false。
```java
public class NettyClient {
    private static final InternalLogger log = InternalLoggerFactory.getInstance(NettyClient.class);

    private EventLoopGroup workerGroup;
    private Bootstrap bootstrap;
    private volatile Channel clientChannel;

    public NettyClient() {
        this(-1);
    }

    public NettyClient(int threads) {
        workerGroup = threads > 0 ? new NioEventLoopGroup(threads) : new NioEventLoopGroup();
        bootstrap = new Bootstrap();
        bootstrap.group(workerGroup)
                .channel(NioSocketChannel.class)
                .option(ChannelOption.TCP_NODELAY, true)
                .option(ChannelOption.SO_KEEPALIVE, false)
                .option(ChannelOption.CONNECT_TIMEOUT_MILLIS, 30000)
                .handler(new ClientHandlerInitializer(this));
    }

    public boolean connect() {
        log.info("尝试连接到服务端: 127.0.0.1:8088");
        try {
            ChannelFuture channelFuture = bootstrap.connect("127.0.0.1", 8088);

            boolean notTimeout = channelFuture.awaitUninterruptibly(30, TimeUnit.SECONDS);
            clientChannel = channelFuture.channel();
            if (notTimeout) {
                if (clientChannel != null && clientChannel.isActive()) {
                    log.info("netty client started !!! {} connect to server", clientChannel.localAddress());
                    return true;
                }
                Throwable cause = channelFuture.cause();
                if (cause != null) {
                    exceptionHandler(cause);
                }
            } else {
                log.warn("connect remote host[{}] timeout {}s", clientChannel.remoteAddress(), 30);
            }
        } catch (Exception e) {
            exceptionHandler(e);
        }
        clientChannel.close();
        return false;
    }

    private void exceptionHandler(Throwable cause) {
        if (cause instanceof ConnectException) {
            log.error("连接异常:{}", cause.getMessage());
        } else if (cause instanceof ClosedChannelException) {
            log.error("connect error:{}", "client has destroy");
        } else {
            log.error("connect error:", cause);
        }
    }

    public void close() {
        if (clientChannel != null) {
            clientChannel.close();
        }
        if (workerGroup != null) {
            workerGroup.shutdownGracefully();
        }
    }

    public Channel getChannel() {
        return clientChannel;
    }

    static class ClientHandlerInitializer extends ChannelInitializer<SocketChannel> {
        private static final InternalLogger log = InternalLoggerFactory.getInstance(NettyClient.class);
        private NettyClient client;

        public ClientHandlerInitializer(NettyClient client) {
            this.client = client;
        }

        @Override
        protected void initChannel(SocketChannel ch) throws Exception {
            ChannelPipeline pipeline = ch.pipeline();
            pipeline.addLast(MarshallingCodeFactory.buildMarshallingDecoder());
            pipeline.addLast(MarshallingCodeFactory.buildMarshallingEncoder());
            //pipeline.addLast(new IdleStateHandler(25, 0, 10));
            //pipeline.addLast(new ClientHeartbeatHandler());
            pipeline.addLast(new SimpleClientHandler(client));
        }
    }
}
```
<a name="WZCV3"></a>
### 客户端启动类
```java
public class NettyClientMain {
    private static final InternalLogger log = InternalLoggerFactory.getInstance(NettyClientMain.class);
    private static final ScheduledExecutorService scheduledExecutor = Executors.newSingleThreadScheduledExecutor();

    public static void main(String[] args) {
        NettyClient nettyClient = new NettyClient();
        boolean connect = false;
        //刚启动时尝试连接10次,都无法建立连接则不在尝试
        //如果想在刚启动后,一直尝试连接,需要放在线程中,异步执行,防止阻塞程序
        for (int i = 0; i < 10; i++) {
            connect = nettyClient.connect();
            if (connect) {
                break;
            }
            //连接不成功,隔5s之后重新尝试连接
            try {
                Thread.sleep(5000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        if (connect) {
            log.info("定时发送数据");
            send(nettyClient);
        } else {
            nettyClient.close();
            log.info("进程退出");
        }
    }

    /** 定时发送数据 */
    static void send(NettyClient client) {
        scheduledExecutor.schedule(new SendTask(client,scheduledExecutor), 2, TimeUnit.SECONDS);
    }
}
```
<a name="VYrkT"></a>
### 客户端断线重连
断线重连需求：

- 服务端和客户端之间网络异常，或响应超时（例如有个很长时间的fullGC），客户端需要主动重连其他节点。
- 服务端宕机时或者和客户端之间发生任何异常时，客户端需要主动重连其他节点。
- 服务端主动向客户端发送（服务端）下线通知时，客户端需要主动重连其他节点。
<a name="jyqXa"></a>
## 如何监听到客户端和服务端连接断开 ?
netty的`io.netty.channel.ChannelInboundHandler`接口中提供了许多重要的接口方法。为了避免实现全部的接口方法，可以通过继承`io.netty.channel.ChannelInboundHandlerAdapter`来重写相应的方法即可。

1. `void channelInactive(ChannelHandlerContext ctx);`在客户端关闭时被调用，表示客户端断开连接。当如下几种情况发生时会触发：
- 客户端在正常active状态下，主动调用channel或者ctx的close方法。
- 服务端主动调用channel或者ctx的close方法关闭客户端的连接 。
- 发生java.io.IOException（一般情况下是双方连接断开）或者java.lang.OutOfMemoryError（4.1.52版本中新增）时
2. `void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) throws Exception;`则是在入栈发生任何异常时被调用。如果异常是`java.io.IOException`或者`java.lang.OutOfMemoryError`(4.1.52版本新增)时，还会触发`channelInactive`方法，也就是上面`channelInactive`被触发的第3条情况。
3. 心跳检查也是检查客户端与服务端之间连接状态的必要方式，因为在一些状态下，两端实际上已经断开连接，但客户端无法感知，这时候就需要通过心跳来判断两端的连接状态。心跳可以是客户端心跳和服务端心跳。
- 客户端心跳：即为客户端发送心跳ping信息，服务端回复pong信息。这样在指定时间内，双方有数据交互则认为是正常连接状态。
- 服务端心跳：则是服务端向客户端发送ping信息，客户端回复pong信息。在指定时间内没有收到回复，则认为对方下线。

netty提供了非常简单的心跳检查方式，只需要在channel的handler链上，添加`io.netty.handler.timeout.IdleStateHandler`即可实现。<br />`IdleStateHandler`有如下几个重要的参数：

- `readerIdleTimeSeconds`，读超时. 即当在指定的时间间隔内没有从 Channel 读取到数据时，会触发一个`READER_IDLE`的IdleStateEvent 事件.
- `writerIdleTimeSeconds`，写超时. 即当在指定的时间间隔内没有数据写入到 Channel 时，会触发一个`WRITER_IDLE`的`IdleStateEvent`事件.
- `allIdleTimeSeconds`，读/写超时. 即当在指定的时间间隔内没有读或写操作时，会触发一个`ALL_IDLE`的`IdleStateEvent` 事件.

为了能够监听到这些事件的触发，还需要重写`ChannelInboundHandler#userEventTriggered(ChannelHandlerContext ctx, Object evt)`方法，通过参数evt判断事件类型。在指定的时间类如果没有读写则发送一条心跳的ping请求，在指定时间内没有收到读操作则任务已经和服务端断开连接。则调用channel或者ctx的close方法，使客户端Handler执行`channelInactive`方法。<br />到这里看来只要在`channelInactive`和`exceptionCaught`两个方法中实现自己的重连逻辑即可，但是遇到了第一个坑，重连方法执行了两次。<br />先看示例代码和结果，在`com.bruce.netty.rpc.client.SimpleClientHandler`中添加如下代码：
```java
public class SimpleClientHandler extends ChannelInboundHandlerAdapter {
    private static final InternalLogger log = InternalLoggerFactory.getInstance(SimpleClientHandler.class);
    //省略部分代码......
    /** 客户端正常下线时执行该方法 */
    @Override
    public void channelInactive(ChannelHandlerContext ctx) throws Exception {
        log.warn("channelInactive:{}", ctx.channel().localAddress());
        reconnection(ctx);
    }

    /** 入栈发生异常时执行exceptionCaught */
    @Override
    public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) throws Exception {
        if (cause instanceof IOException) {
            log.warn("exceptionCaught:客户端[{}]和远程断开连接", ctx.channel().localAddress());
        } else {
            log.error(cause);
        }
        reconnection(ctx);
    }

    private void reconnection(ChannelHandlerContext ctx) {
        log.info("5s之后重新建立连接");
        //暂时为空实现
    }
}
```
`ClientHandlerInitializer` 中添加`io.netty.handler.timeout.IdleStateHandler`用于心跳检查，`ClientHeartbeatHandler`用于监听心跳事件，接收心跳pong回复。
```java
static class ClientHandlerInitializer extends ChannelInitializer<SocketChannel> {
    private static final InternalLogger log = InternalLoggerFactory.getInstance(NettyClient.class);
    private NettyClient client;

    public ClientHandlerInitializer(NettyClient client) {
        this.client = client;
    }

    @Override
    protected void initChannel(SocketChannel ch) throws Exception {
        ChannelPipeline pipeline = ch.pipeline();
        pipeline.addLast(MarshallingCodeFactory.buildMarshallingDecoder());
        pipeline.addLast(MarshallingCodeFactory.buildMarshallingEncoder());
        //25s内没有read操作则触发READER_IDLE事件
        //10s内既没有read又没有write操作则触发ALL_IDLE事件
        pipeline.addLast(new IdleStateHandler(25, 0, 10));
        pipeline.addLast(new ClientHeartbeatHandler());
        pipeline.addLast(new SimpleClientHandler(client));
    }
}
```
com.bruce.netty.rpc.client.ClientHeartbeatHandler
```java
public class ClientHeartbeatHandler extends ChannelInboundHandlerAdapter {
    private static final InternalLogger log = InternalLoggerFactory.getInstance(ClientHeartbeatHandler.class);

    @Override
    public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
        if (msg.equals("pong")) {
            log.info("收到心跳回复");
        } else {
            super.channelRead(ctx, msg);
        }
    }

    @Override
    public void userEventTriggered(ChannelHandlerContext ctx, Object evt) throws Exception {
        if (evt instanceof IdleStateEvent) {
            //该事件需要配合 io.netty.handler.timeout.IdleStateHandler使用
            IdleStateEvent idleStateEvent = (IdleStateEvent) evt;
            if (idleStateEvent.state() == IdleState.ALL_IDLE) {
                //向服务端发送心跳检测
                ctx.writeAndFlush("ping");
                log.info("发送心跳数据");
            } else if (idleStateEvent.state() == IdleState.READER_IDLE) {
                //超过指定时间没有读事件,关闭连接
                log.info("超过心跳时间,关闭和服务端的连接:{}", ctx.channel().remoteAddress());
                ctx.channel().close();
            }
        } else {
            super.userEventTriggered(ctx, evt);
        }
    }
}
```
先启动server端，再启动client端，待连接成功之后kill掉 server端进程。<br />![2021-12-24-14-56-04-089145.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1640329814959-18453865-0d55-4380-a241-741813225a82.png#clientId=u151b5ff2-2e81-4&from=ui&id=u10c84bd3&originHeight=269&originWidth=1024&originalType=binary&ratio=1&rotation=0&showTitle=false&size=828046&status=done&style=none&taskId=udd4f1591-bfd2-4771-a0da-3160d8714bd&title=)<br />通过客户端日志可以看出，先是执行了`exceptionCaught`方法然后执行了`channelInactive`方法，但是这两个方法中都调用了`reconnection`方法，导致同时执行了两次重连。<br />为什么执行了`exceptionCaught`方法又执行了`channelInactive`方法呢？<br />可以在`exceptionCaught`和`channelInactive`方法添加断点一步步查看源码<br />![2021-12-24-14-56-04-486685.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1640329805867-15936f13-ebe6-4b26-89bf-1eb0743d21df.png#clientId=u151b5ff2-2e81-4&from=ui&id=uc7d0b188&originHeight=565&originWidth=839&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1425045&status=done&style=none&taskId=u24d0ee6d-0a71-4db4-9f00-8b91629e44f&title=)<br />当`NioEventLoop`执行`select`操作之后，处理相应的`SelectionKey`，发生异常后，会调用`AbstractNioByteChannel.NioByteUnsafe#handleReadException`方法进行处理，并触发`pipeline.fireExceptionCaught(cause)`，最终调用到用户handler的`fireExceptionCaught`方法。
```java
private void handleReadException(ChannelPipeline pipeline, ByteBuf byteBuf, Throwable cause, boolean close,
                                 RecvByteBufAllocator.Handle allocHandle) {
  if (byteBuf != null) {
    if (byteBuf.isReadable()) {
      readPending = false;
      pipeline.fireChannelRead(byteBuf);
    } else {
      byteBuf.release();
    }
  }
  allocHandle.readComplete();
  pipeline.fireChannelReadComplete();
  pipeline.fireExceptionCaught(cause);

  // If oom will close the read event, release connection.
  // See https://github.com/netty/netty/issues/10434
  if (close || cause instanceof OutOfMemoryError || cause instanceof IOException) {
    closeOnRead(pipeline);
  }
}
```
该方法最后会判断异常类型，执行close连接的方法。在连接断线的场景中，这里即为`java.io.IOException`，所以执行了`close`方法，当debug到`AbstractChannel.AbstractUnsafe#close(ChannelPromise, Throwable, ClosedChannelException, notify)`方法中会发现最后又调用了`AbstractUnsafe#fireChannelInactiveAndDeregister`方法，继续debug最后则会执行自定义的`fireChannelInactive`方法。<br />到这里可以总结一个知识点：netty中当执行到handler地`fireExceptionCaught`方法时，可能会继续触发到`fireChannelInactive`，也可能不会触发`fireChannelInactive`。<br />除了netty根据异常类型判断是否执行`close`方法外，其实开发人员也可以自己通过`ctx`或者`channel`去调用close方法，代码如下：
```java
@Override
public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) throws Exception {
    if (cause instanceof IOException) {
        log.warn("exceptionCaught:客户端[{}]和远程断开连接", ctx.channel().localAddress());
    } else {
        log.error(cause);
    }
    //ctx.close();
    ctx.channel().close();
}
```
但这种显示调用`close`方法，是否一定会触发调用`fireChannelInactive`呢？<br />如果是，那么只需要在`exceptionCaught`中调用`close`方法，`fireChannelInactive`中做重连的逻辑即可！！<br />通过日志观察到，在`exceptionCaught`中调用`close`方法每次都会调用`fireChannelInactive`方法。但是查看源码，可以看到这是不一定的，因为在`AbstractChannel.AbstractUnsafe#close(ChannelPromise,Throwable, ClosedChannelException, notify)`中会调用`io.netty.channel.Channel#isActive`进行判断，只有为true，才会执行`fireChannelInactive`方法。
```java
//io.netty.channel.socket.nio.NioSocketChannel#isActive
@Override
public boolean isActive() {
    SocketChannel ch = javaChannel();
    return ch.isOpen() && ch.isConnected();
}
```
<a name="FbuFJ"></a>
## 如何解决同时执行两次问题呢？
在netty初始化时，都会添加一系列的handler处理器，这些handler实际上会在netty创建Channel对象(`NioSocketChannel`)时，被封装在`DefaultChannelPipeline`中，而`DefaultChannelPipeline`实际上是一个双向链表，头节点为`TailContext`，尾节点为`TailContext`，而中间的节点则是添加的一个个handler（被封装成`DefaultChannelHandlerContext`），当执行`Pipeline`上的方法时，会从链表上遍历handler执行，因此当执行exceptionCaught方法时，只需要提前移除链表上自定义的Handler则无法执行`fireChannelInactive`方法。<br />![2021-12-24-14-56-04-870366.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1640329763699-b9ccae88-03fc-4948-adcc-5ff0d7011eaa.png#clientId=u151b5ff2-2e81-4&from=ui&id=u43f1380a&originHeight=352&originWidth=834&originalType=binary&ratio=1&rotation=0&showTitle=false&size=882554&status=done&style=none&taskId=u1fb38706-c060-493f-a045-06ee7ee3aa9&title=)<br />最后实现代码如下：
```java
public class SimpleClientHandler extends ChannelInboundHandlerAdapter {

    private static final InternalLogger log = InternalLoggerFactory.getInstance(SimpleClientHandler.class);

    @Override
    public void channelInactive(ChannelHandlerContext ctx) throws Exception {
        log.warn("channelInactive:{}", ctx.channel().localAddress());
        ctx.pipeline().remove(this);
        ctx.channel().close();
        reconnection(ctx);
    }

    @Override
    public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) throws Exception {
        if (cause instanceof IOException) {
            log.warn("exceptionCaught:客户端[{}]和远程断开连接", ctx.channel().localAddress());
        } else {
            log.error(cause);
        }
        ctx.pipeline().remove(this);
        //ctx.close();
        ctx.channel().close();
        reconnection(ctx);
    }
}
```
执行效果如下，可以看到当发生异常时，只是执行了`exceptionCaught`方法，并且通过channel关闭了上一次连接资源，也没有执行当前handler的`fireChannelInactive`方法。<br />![2021-12-24-14-56-05-381521.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1640329741873-e7fa2a74-1449-4615-ac99-a0c649a17893.png#clientId=u151b5ff2-2e81-4&from=ui&id=u41c84863&originHeight=228&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=740222&status=done&style=none&taskId=u68aad079-2ad4-4e0b-81fb-ea8214a4403&title=)
<a name="poD5g"></a>
## 如何实现断线后重新连接 ?
通过上面分析，已经知道在什么方法中实现自己的重连逻辑，但是具体该怎么实现呢，怀着好奇的心态搜索了一下各大码友的实现方案。大多做法是通过`ctx.channel().eventLoop().schedule`添加一个定时任务调用客户端的连接方法。实现代码如下：
```java
private void reconnection(ChannelHandlerContext ctx) {
    log.info("5s之后重新建立连接");
    ctx.channel().eventLoop().schedule(new Runnable() {
        @Override
        public void run() {
            boolean connect = client.connect();
            if (connect) {
                log.info("重新连接成功");
            } else {
                reconnection(ctx);
            }
        }
    }, 5, TimeUnit.SECONDS);
}
```
测试：先启动server端，再启动client端，待连接成功之后kill掉 server端进程。客户端如期定时执行重连，但也就去茶水间倒杯水的时间，回来后发现了如下异常。
```java
......省略14条相同的重试日志
[2021-01-17 18:46:45.032] INFO   [nioEventLoopGroup-2-1] [com.bruce.netty.rpc.client.SimpleClientHandler] : 5s之后重新建立连接
[2021-01-17 18:46:48.032] INFO   [nioEventLoopGroup-2-1] [com.bruce.netty.rpc.client.NettyClient] : 尝试连接到服务端: 127.0.0.1:8088
[2021-01-17 18:46:50.038] ERROR   [nioEventLoopGroup-2-1] [com.bruce.netty.rpc.client.NettyClient] : 连接异常:Connection refused: no further information: /127.0.0.1:8088
[2021-01-17 18:46:50.038] INFO   [nioEventLoopGroup-2-1] [com.bruce.netty.rpc.client.SimpleClientHandler] : 5s之后重新建立连接
[2021-01-17 18:46:53.040] INFO   [nioEventLoopGroup-2-1] [com.bruce.netty.rpc.client.NettyClient] : 尝试连接到服务端: 127.0.0.1:8088
[2021-01-17 18:46:53.048] ERROR   [nioEventLoopGroup-2-1] [com.bruce.netty.rpc.client.NettyClient] : connect error:
io.netty.util.concurrent.BlockingOperationException: DefaultChannelPromise@10122121(incomplete)
     at io.netty.util.concurrent.DefaultPromise.checkDeadLock(DefaultPromise.java:462)
     at io.netty.channel.DefaultChannelPromise.checkDeadLock(DefaultChannelPromise.java:159)
     at io.netty.util.concurrent.DefaultPromise.await0(DefaultPromise.java:667)
     at io.netty.util.concurrent.DefaultPromise.awaitUninterruptibly(DefaultPromise.java:305)
     at com.bruce.netty.rpc.client.NettyClient.connect(NettyClient.java:49)
     at com.bruce.netty.rpc.client.SimpleClientHandler$1.run(SimpleClientHandler.java:65)
     at io.netty.util.concurrent.PromiseTask.runTask(PromiseTask.java:98)
     at io.netty.util.concurrent.ScheduledFutureTask.run(ScheduledFutureTask.java:170)
     at io.netty.util.concurrent.AbstractEventExecutor.safeExecute$$$capture(AbstractEventExecutor.java:164)
     at io.netty.util.concurrent.AbstractEventExecutor.safeExecute(AbstractEventExecutor.java)
     at io.netty.util.concurrent.SingleThreadEventExecutor.runAllTasks(SingleThreadEventExecutor.java:472)
     at io.netty.channel.nio.NioEventLoop.run(NioEventLoop.java:500)
```
根据异常栈，可以发现是`com.bruce.netty.rpc.client.NettyClient#connect`方法中调用了等待方法
```java
boolean notTimeout = channelFuture.awaitUninterruptibly(20, TimeUnit.SECONDS);
```
而该方法内部会进行检测，是否在io线程上执行了同步等待，这会导致抛出异常`BlockingOperationException`。
```java
@Override
protected void checkDeadLock() {
    if (channel().isRegistered()) {
        super.checkDeadLock();
    }
}

protected void checkDeadLock() {
    EventExecutor e = executor();
    if (e != null && e.inEventLoop()) {
        throw new BlockingOperationException(toString());
    }
}
```
奇怪的是为什么不是每次尝试重连都抛出该异常，而是每隔16次抛出一次呢?<br />联想到笔记本是8核处理器，而netty默认线程池是2 * c，就是16条线程，这之间似乎有些关联。<br />实际上在调用`ChannelFuture channelFuture = bootstrap.connect("127.0.0.1", 8088);`，netty首先会创建一个`io.netty.channel.Channel`(示例中是`NioSocketChannel`)，然后通过`io.netty.util.concurrent.EventExecutorChooserFactory.EventExecutorChooser`依次选择一个`NioEventLoop`，将Channel绑定到`NioEventLoop`上。<br />![2021-12-24-14-56-05-743905.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1640329622944-2ee80e39-0988-48b1-804d-53c4218d0609.png#clientId=u151b5ff2-2e81-4&from=ui&id=u74134ce1&originHeight=757&originWidth=849&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1932035&status=done&style=none&taskId=u57d46fe4-980b-487b-bdb7-b270a221799&title=)<br />io.netty.util.concurrent.SingleThreadEventExecutor#inEventLoop
```java
//Return true if the given Thread is executed in the event loop, false otherwise.
@Override
public boolean inEventLoop(Thread thread) {
    return thread == this.thread;
}
```
重连的方法是在一个`NioEventLoop`(也就是io线程)上被调用，第1次重连实际上是选择了第2个`NioEventLoop`，第2次重连实际上是选择了第3个`NioEventLoop`，以此类推，当一轮选择过后，重新选到第一个`NioEventLoop`时，`boolean inEventLoop()`返回true，则抛出了`BlockingOperationException`。
<a name="vrLHC"></a>
### 方案1
不要在netty的io线程上执行同步连接，使用单独的线程池定时执行重试，该线程还可以执行自己重连的业务逻辑操作，不阻塞io线程。（如果不需要业务操作之后销毁线程池）。<br />`com.bruce.netty.rpc.client.SimpleClientHandler` 修改`reconnection`方法
```java
private static ScheduledExecutorService SCHEDULED_EXECUTOR;

private void initScheduledExecutor() {
    if (SCHEDULED_EXECUTOR == null) {
        synchronized (SimpleClientHandler.class) {
            if (SCHEDULED_EXECUTOR == null) {
                SCHEDULED_EXECUTOR = Executors.newSingleThreadScheduledExecutor(r -> {
                    Thread t = new Thread(r, "Client-Reconnect-1");
                    t.setDaemon(true);
                    return t;
                });
            }
        }
    }
}

private void reconnection(ChannelHandlerContext ctx) {
    log.info("5s之后重新建立连接");
    initScheduledExecutor();

    SCHEDULED_EXECUTOR.schedule(() -> {
        boolean connect = client.connect();
        if (connect) {
            //连接成功,关闭线程池
            SCHEDULED_EXECUTOR.shutdown();
            log.info("重新连接成功");
        } else {
            reconnection(ctx);
        }
    }, 3, TimeUnit.SECONDS);
}
```
<a name="Q4bCy"></a>
### 方案2
可以在io线程上使用异步重连:<br />`com.bruce.netty.rpc.client.NettyClient`添加方法`connectAsync`方法，两者的区别在于`connectAsync`方法中没有调用`channelFuture`的同步等待方法。而是改成监听器(`ChannelFutureListener`)的方式，实际上这个监听器是运行在io线程上。
```java
public void connectAsync() {
    log.info("尝试连接到服务端: 127.0.0.1:8088");
    ChannelFuture channelFuture = bootstrap.connect("127.0.0.1", 8088);
    channelFuture.addListener((ChannelFutureListener) future -> {
        Throwable cause = future.cause();
        if (cause != null) {
            exceptionHandler(cause);
            log.info("等待下一次重连");
            channelFuture.channel().eventLoop().schedule(this::connectAsync, 5, TimeUnit.SECONDS);
        } else {
            clientChannel = channelFuture.channel();
            if (clientChannel != null && clientChannel.isActive()) {
                log.info("Netty client started !!! {} connect to server", clientChannel.localAddress());
            }
        }
    });
}
```
com.bruce.netty.rpc.client.SimpleClientHandler
```java
public class SimpleClientHandler extends ChannelInboundHandlerAdapter {
    private static final InternalLogger log = InternalLoggerFactory.getInstance(SimpleClientHandler.class);
    private NettyClient client;

    public SimpleClientHandler(NettyClient client) {
        this.client = client;
    }

    @Override
    public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
        log.info("client receive:{}", msg);
    }

    @Override
    public void channelInactive(ChannelHandlerContext ctx) throws Exception {
        log.warn("channelInactive:{}", ctx.channel().localAddress());
        ctx.pipeline().remove(this);
        ctx.channel().close();
        reconnectionAsync(ctx);
    }

    @Override
    public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) throws Exception {
        if (cause instanceof IOException) {
            log.warn("exceptionCaught:客户端[{}]和远程断开连接", ctx.channel().localAddress());
        } else {
            log.error(cause);
        }
        ctx.pipeline().remove(this);
        ctx.close();
        reconnectionAsync(ctx);
    }

    private void reconnectionAsync(ChannelHandlerContext ctx) {
        log.info("5s之后重新建立连接");
        ctx.channel().eventLoop().schedule(new Runnable() {
            @Override
            public void run() {
                client.connectAsync();
            }
        }, 5, TimeUnit.SECONDS);
    }
}
```
<a name="rK0k4"></a>
#### netty客户端线程给多大比较合理 ?
netty中一个`NioEventLoopGroup`默认创建的线程数是`cpu核心数 * 2` ，这些线程都是用于io操作，那么对于客户端应用程序来说真的需要这么多io线程么？<br />通过上面分析`BlockingOperationException`异常时分析到，实际上netty在创建一个`Channel`对象后只会从`NioEventLoopGroup`中选择一个`NioEventLoop`来绑定，只有创建多个`Channel`才会依次选择下一个`NioEventLoop`，也就是说一个`Channel`只会对应一个`NioEventLoop`，而`NioEventLoop`可以绑定多个`Channel`。

1. 对于客户端来说，如果只是连接的一个server节点，那么只要设置1条线程即可。即使出现了断线重连，在连接断开之后，之前的`Channel`会从`NioEventLoop`移除。重连之后，仍然只会在仅有的一个`NioEventLoop`注册一个新的`Channel`。
2. 如果客户端同时如下方式多次调用`io.netty.bootstrap.Bootstrap#connect(String inetHost, int inetPort)`连接多个Server节点，那么线程可以设置大一点，但不要超过`2*c`，而且只要出现断线重连，同样不能保证每个`NioEventLoop`都会绑定一个客户端Channel。
```java
public boolean connect() {
    try {
        ChannelFuture channelFuture1 = bootstrap.connect("127.0.0.1", 8088);
        ChannelFuture channelFuture2 = bootstrap.connect("127.0.0.1", 8088);
        ChannelFuture channelFuture3 = bootstrap.connect("127.0.0.1", 8088);
    } catch (Exception e) {
        exceptionHandler(e);
    }
    clientChannel.close();
    return false;
}
```

3. 如果netty客户端线程数设置大于1有什么影响么？

明显的异常肯定是不会有的，但是造成资源浪费，首先会创建多个`NioEventLoop`对象，但`NioEventLoop`是处于非运行状态。一旦出现断线重连，那么重新连接时，下一个`NioEventLoop`则会被选中，并创建/启动线程一直处于runnable状态。而上一个`NioEventLoop`也是一直处于runnable状态，由于上一个`Channel`已经被close，所以会造成每次select结果都是空的，没有意义的空轮询。<br />如下则是netty客户端使用默认线程数，4次断线重连后一共创建的5条`NioEventLoop`线程，但是实际上只有第5条线程在执行读写操作。<br />![2021-12-24-14-56-06-007260.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1640329524262-12903e9d-a2ec-4a90-a22d-17e64715be47.png#clientId=u151b5ff2-2e81-4&from=ui&id=u41c22994&originHeight=202&originWidth=579&originalType=binary&ratio=1&rotation=0&showTitle=false&size=351719&status=done&style=shadow&taskId=ubd78c2e6-3e3b-413b-9ed6-c7bf4c21813&title=)![2021-12-24-14-56-06-437679.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1640329524110-fec08fff-5232-43b0-83b9-14ce6cbf8370.png#clientId=u151b5ff2-2e81-4&from=ui&id=u04da32be&originHeight=672&originWidth=976&originalType=binary&ratio=1&rotation=0&showTitle=false&size=186971&status=done&style=none&taskId=u09f607a6-f563-43d6-bd03-eef19890edb&title=)

4. 如果客户端存在耗时的业务逻辑，应该单独使用业务线程池，避免在netty的io线程中执行耗时逻辑处理。
