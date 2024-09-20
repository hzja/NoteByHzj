Java Socket Netty
<a name="VuEZD"></a>
## 什么是Netty
> Netty是由JBOSS提供的一个java开源框架，现为Github上的独立项目。Netty提供异步的、事件驱动的网络应用程序框架和工具，用以快速开发高性能、高可靠性的网络服务器和客户端程序。

也就是说，Netty 是一个基于NIO的客户、服务器端的编程框架，使用Netty 可以确保快速和简单的开发出一个网络应用，例如实现了某种协议的客户、服务端应用。Netty相当于简化和流线化了网络应用的编程开发过程，例如：基于TCP和UDP的socket服务开发。<br />上面是来自于百度百科给出的解释，能清晰的看到，Netty是一个基于NIO的模型，使用Netty的地方很多就是socket服务开发，而关于NIO，相信大家肯定不陌生。
<a name="FcBiJ"></a>
### 对比Netty和传统的Socket
既然要说Netty，那么肯定要对Netty还有Socket不同的代码进行一个分析，分析的透彻了，才能真的选择使用Netty，而不再进行Socket的开发了，相信到时候，大家肯定会做出最正确的选择。
<a name="f4BvJ"></a>
#### 传统Socket编程服务端
```java
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

/**
 * @ClassName SocketDemo
 * @Date 2021/4/19 10:33
 * @Description SocketDemo
 */
public class SocketServerDemo {

    public static void main(String[] args) {
        ServerSocket server=null;
        try {
            server=new ServerSocket(18080);
            System.out.println("时间服务已经启动--端口号为：18080...");
            while (true){
                Socket client = server.accept();
                //每次接收到一个新的客户端连接，启动一个新的线程来处理
                new Thread(new TimeServerHandler(client)).start();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }finally {
            try {
                server.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
```
```java
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.Calendar;

/**
 * @ClassName TimeServerHandler
 * @Date 2021/4/19 10:35
 * @Description TimeServerHandler
 */
public class TimeServerHandler implements Runnable {
    private Socket clientProxxy;

    public TimeServerHandler(Socket clientProxxy) {
        this.clientProxxy = clientProxxy;
    }

    @Override
    public void run() {
        BufferedReader reader = null;
        PrintWriter writer = null;
        try {
            reader = new BufferedReader(new InputStreamReader(clientProxxy.getInputStream()));
            writer =new PrintWriter(clientProxxy.getOutputStream()) ;
            while (true) {//因为一个client可以发送多次请求，这里的每一次循环，相当于接收处理一次请求
                String request = reader.readLine();
                if (!"GET CURRENT TIME".equals(request)) {
                    writer.println("BAD_REQUEST");
                } else {
                    writer.println(Calendar.getInstance().getTime().toLocaleString());
                }
                writer.flush();
            }
        } catch (Exception e) {
            throw new RuntimeException(e);
        } finally {
            try {
                writer.close();
                reader.close();
                clientProxxy.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
```
<a name="GVLav"></a>
#### 传统Socket编程客户端
```java
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

/**
 * @ClassName SocketClientDemo
 * @Date 2021/4/19 10:42
 * @Description SocketClientDemo
 */
public class SocketClientDemo {
    public static void main(String[] args)  {
        BufferedReader reader = null;
        PrintWriter writer = null;
        Socket client=null;
        try {
            client=new Socket("127.0.0.1",18080);
            writer = new PrintWriter(client.getOutputStream());
            reader = new BufferedReader(new InputStreamReader(client.getInputStream()));

            while (true){//每隔5秒发送一次请求
                writer.println("GET CURRENT TIME");
                writer.flush();
                String response = reader.readLine();
                System.out.println("Current Time:"+response);
                Thread.sleep(5000);
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            try {
                writer.close();
                reader.close();
                client.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

    }
}
```
来执行一下才能知道效果，<br />首先运行服务端：
```
TimeServer Started on 18080...
```
接着启动客户端
```
Current Time:2021-4-19 10:48:21
Current Time:2021-4-19 10:48:26
Current Time:2021-4-19 10:48:31
Current Time:2021-4-19 10:48:36
Current Time:2021-4-19 10:48:41
Current Time:2021-4-19 10:48:46
Current Time:2021-4-19 10:48:51
```
大家看一下，这是不是就是相当于一个Socket的客户端和服务端之间进行通信的过程，在client端可以发送请求指令"GET CURRENT TIME"给server端，每隔5秒钟发送一次，每次server端都返回当前时间。<br />而这也是传统的BIO的做法，每一个client都需要去对应一个线程去进行处理，client越多，那么要开启的线程也就会越多，也就是说，如果采用BIO通信模型的服务端，通常由一个独立的Acceptor线程负责监听客户端的连接，当接收到客户端的连接请求后，会为每一个客户端请求创建新的线程进行请求的处理，处理完成后通过输出流返回信息给客户端，响应完成后销毁线程。<br />模型图如下<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624158284314-a2787593-0e5b-4781-99e6-393f88fd7d85.webp#clientId=u24dfa710-02d8-4&from=paste&id=ua5f682fe&originHeight=479&originWidth=1067&originalType=url&ratio=3&status=done&style=shadow&taskId=u4eceb4a4-0106-440a-91ff-426944f00c7)<br />这时候就有大佬说，不会用线程池么？使用线程池的话，它实际上并没有解决任何实际性的问题，他实际上就是对BIO做了一个优化，属于伪异步IO通信。<br />伪异步IO通信模型图<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624158284247-0a563ddf-ef08-4b96-befd-6717257fbdd2.webp#clientId=u24dfa710-02d8-4&from=paste&id=ubb305968&originHeight=502&originWidth=1078&originalType=url&ratio=3&status=done&style=shadow&taskId=u155c62ff-a19b-4111-aa17-42a479cac21)<br />异步IO通信确实能缓解一部分的压力，但是这种模型也是有缺陷的，当有大量客户端请求的时候，随着并发访问量的增长，伪异步IO就会造成线程池阻塞。<br />这时候就取决于是想选择，系统发生线程堆栈溢出、创建新线程失败等问题呢，还是选择大量客户端请求，造成线程池阻塞。<br />都说，技术是为了解决问题而出现的，那么接下来就有了解决这个问题的技术出现了，Netty，来看看Netty吧。
<a name="nEad6"></a>
### Netty环境搭建
在这里使用的依旧是Springboot来整合Netty的环境，然后在后续过程中，使用Netty实现服务端程序和客户端程序，虽然Netty并没有实现传说中的AIO，但是已经算是吧这个NIO的模型，实现到了极致了。
<a name="p753G"></a>
#### 先创建出来一个项目
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1634893671767-5107b2bd-857c-4abe-b87a-c8b1e52e315e.png#clientId=ub3e52f98-0b1a-4&from=paste&height=518&id=u2d92b90b&originHeight=1554&originWidth=2003&originalType=binary&ratio=1&size=192788&status=done&style=shadow&taskId=ubceed397-c952-405d-b5ad-ed0d56752c2&width=667.6666666666666)
<a name="DqbnJ"></a>
#### 加入Netty的pom的依赖
```xml
<!--Netty-->
<dependency>
  <groupId>io.netty</groupId>
  <artifactId>netty-all</artifactId>
  <version>4.1.31.Final</version>
</dependency>
<dependency>
  <groupId>org.projectlombok</groupId>
  <artifactId>lombok</artifactId>
  <version>1.16.22</version>
</dependency>

<!-- logger -->
<dependency>
  <groupId>org.slf4j</groupId>
  <artifactId>slf4j-api</artifactId>
  <version>1.7.25</version>
</dependency>
<dependency>
  <groupId>ch.qos.logback</groupId>
  <artifactId>logback-core</artifactId>
  <version>1.2.3</version>
</dependency>
<dependency>
  <groupId>ch.qos.logback</groupId>
  <artifactId>logback-classic</artifactId>
  <version>1.2.3</version>
</dependency>
```
<a name="kWLft"></a>
#### Netty服务端程序
```java
import io.netty.bootstrap.ServerBootstrap;
import io.netty.channel.ChannelFuture;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.EventLoopGroup;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.SocketChannel;
import io.netty.channel.socket.nio.NioServerSocketChannel;
import io.netty.handler.codec.LineBasedFrameDecoder;
import io.netty.handler.codec.string.StringDecoder;

/**
 * @ClassName NettyServerDemo
 * @Date 2021/4/19 11:11
 * @Description NettyServerDemo
 */
public class NettyServerDemo {

    private int port=18081;
    public void run() throws Exception {
        EventLoopGroup bossGroup = new NioEventLoopGroup(); 
        EventLoopGroup workerGroup = new NioEventLoopGroup();
        try {
            ServerBootstrap b = new ServerBootstrap(); 
            b.group(bossGroup, workerGroup)
                    .channel(NioServerSocketChannel.class) 
                    .childHandler(new ChannelInitializer<SocketChannel>() { 
                        @Override
                        public void initChannel(SocketChannel ch) throws Exception {
                            ch.pipeline().addLast(new LineBasedFrameDecoder(1024));
                            ch.pipeline().addLast(new StringDecoder());
                            ch.pipeline().addLast(new TimeServerHandler());
                        }
                    });

            ChannelFuture f = b.bind(port).sync(); 
            System.out.println("TimeServer Started on 18081...");
            f.channel().closeFuture().sync();
        } finally {
            workerGroup.shutdownGracefully();
            bossGroup.shutdownGracefully();
        }
    }
    public static void main(String[] args) throws Exception {
        new NettyServerDemo().run();
    }
}
```
```java
import io.netty.buffer.ByteBuf;
import io.netty.buffer.Unpooled;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelInboundHandlerAdapter;

import java.util.Date;

/**
 * @ClassName TimeServerHandler
 * @Date 2021/4/19 11:19
 * @Description TimeServerHandler
 */
public class TimeServerHandler extends ChannelInboundHandlerAdapter {

    @Override
    public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
        String request = (String) msg;
        String response = null;
        if ("QUERY TIME ORDER".equals(request)) {
            response = new Date(System.currentTimeMillis()).toString();
        } else {
            response = "BAD REQUEST";
        }
        response = response + System.getProperty("line.separator");
        ByteBuf resp = Unpooled.copiedBuffer(response.getBytes());
        ctx.writeAndFlush(resp);
    }
}
```
<a name="K5iy1"></a>
#### Netty客户端程序
```java
import io.netty.bootstrap.Bootstrap;
import io.netty.channel.ChannelFuture;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.EventLoopGroup;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.SocketChannel;
import io.netty.channel.socket.nio.NioSocketChannel;
import io.netty.handler.codec.LineBasedFrameDecoder;
import io.netty.handler.codec.string.StringDecoder;

/**
 * @ClassName NettyClientDemo
 * @Date 2021/4/19 11:21
 * @Description NettyClientDemo
 */
public class NettyClientDemo {
    public static void main(String[] args) throws Exception {
        String host = "localhost";
        int port = 18081;
        EventLoopGroup workerGroup = new NioEventLoopGroup();
        try {
            Bootstrap b = new Bootstrap();
            b.group(workerGroup);
            b.channel(NioSocketChannel.class);
            b.handler(new ChannelInitializer<SocketChannel>() {
                @Override
                public void initChannel(SocketChannel ch) throws Exception {
                    ch.pipeline().addLast(new LineBasedFrameDecoder(1024));
                    ch.pipeline().addLast(new StringDecoder());
                    ch.pipeline().addLast(new TimeClientHandler());
                }
            });
            // 开启客户端.
            ChannelFuture f = b.connect(host, port).sync();
            // 等到连接关闭.
            f.channel().closeFuture().sync();
        } finally {
            workerGroup.shutdownGracefully();
        }
    }
}
```
```java
import io.netty.buffer.ByteBuf;
import io.netty.buffer.Unpooled;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelInboundHandlerAdapter;

/**
 * @ClassName TimeClientHandler
 * @Date 2021/4/19 11:22
 * @Description TimeClientHandler
 */
public class TimeClientHandler extends ChannelInboundHandlerAdapter {

    private byte[] req=("QUERY TIME ORDER" + System.getProperty("line.separator")).getBytes();
    @Override
    public void channelActive(ChannelHandlerContext ctx) {//1
        ByteBuf message = Unpooled.buffer(req.length);
        message.writeBytes(req);
        ctx.writeAndFlush(message);
    }
    @Override
    public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
        String body = (String) msg;
        System.out.println("Now is:" + body);
    }
}

```
首先启动服务端，控制台输出：
```java
TimeServer Started on 18081...
```
接着启动客户端，控制要输出：
```java
Now is:Mon Apr 19 11:34:21 CST 2021
```
既然代码写了，那是不是就得来分析一下这个Netty在中间都干了什么东西，他的类是什么样子的，都有哪些方法。<br />大家先从代码的源码上开始看起，因为在代码中分别使用到了好几个类，而这些类的父类，或者是接口定义者追根到底，也就是这个样子的，从IDEA中打开他的类图可以清晰的看到。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624158284245-41fb33bb-9f42-4b34-99da-1a75402317b3.webp#clientId=u24dfa710-02d8-4&from=paste&id=u0dc7d1bc&originHeight=502&originWidth=1080&originalType=url&ratio=3&status=done&style=none&taskId=u5b9a6f64-60b4-41d7-b3ad-62c8cbffaa4)<br />而在源码中，最重要的就是这个Channel，接下来就来分析一波吧。
<a name="oyD5c"></a>
### Channel
All I/O operations are asynchronous.一句话点出核心所有的IO操作都是异步的，这意味着任何I/O调用都将立即返回，但不保证请求的I/O操作已完成。这是在源码的注释上面给出的解释。
<a name="tpc8E"></a>
#### Channel分类

- 服务端: `NioServerSocketChannel`
- 客户端: `NioSocketChannel`

看到这个，大家肯定也都不陌生，因为Channel即可以在JDK的Socket中充当管道出现，同时，也在Netty的服务端和客户端进行IO数据交互，充当一个媒介的存在，那么他的区别在哪？<br />Netty对Jdk原生的ServerSocketChannel进行了封装和增强封装成了NioXXXChannel, 相对于原生的JdkChannel,Netty的Channel增加了如下的组件。

- id 标识唯一身份信息
- 可能存在的parent Channel
- 管道 pepiline
- 用于数据读写的unsafe内部类
- 关联上相伴终生的`NioEventLoop`

在官网可以了解这个这个类的API有更多的信息[io.netty.channel](https://io.netty.channel)<br />而关于`Channel`，其实换成大家容易理解的话的话，那就是由它负责同对端进行网络通信、注册和数据操作等功能
> A Channel can have a parent depending on how it was created. For instance, a SocketChannel, that was accepted by ServerSocketChannel, will return the ServerSocketChannel as its parent on parent().<br />The semantics of the hierarchical structure depends on the transport implementation where the Channel belongs to. For example, you could write a new Channel implementation that creates the sub-channels that share one socket connection, as BEEP and SSH do.

一个`Channel`可以有一个父`Channel`，这取决于它是如何创建的。例如，被`ServerSocketChannel`接受的`SocketChannel`将返回`ServerSocketChannel`作为其`parent()`上的父对象。层次结构的语义取决于通道所属的传输实现。<br />`Channel`的抽象类`AbstractChannel`中有一个受保护的构造方法，而`AbstractChannel`内部有一个`pipeline`属性，Netty在对`Channel`进行初始化的时候将该属性初始化为`DefaultChannelPipeline`的实例。
<a name="ZZ52c"></a>
## 为什么选择Netty
|  | 同步阻塞I/O(BIO) | 伪异步I/O | 非阻塞I/O (NIO) | 异步I/O (AIO) |
| --- | --- | --- | --- | --- |
| I/O类型(同步) | 同步I/O | 同步I/O | 同步I/O (I/O多路复用) | 异步I/O |
| API使用难度 | 简单 | 简单 | 非常复杂 | 复杂 |
| 调试难度 | 简单 | 简单 | 复杂 | 复杂 |
| 可靠性 | 非常差 | 差 | 高 | 高 |
| 吞吐量 | 低 | 中 | 高 | 高 |

其实在上面的图中，已经能看出来了，不同的I/O模型，效率，使用难度，吞吐量都是非常重要的，所以选择的时候，肯定要慎重选择，而为什么不使用Java原生的呢？<br />实际上很简单，1.复杂，2.不好用<br />对于Java的NIO的类库和API繁杂使用麻烦，需要熟练掌握`Selectol`,`ServerSocketChannel`,`SocketChannel`,`ByteBuffer` 等<br />JDK NIO的BUG，比如epoll bug，这个BUG会在linux上导致cpu 100%，使得nio server/client不可用，而且在1.7中都没有解决完这个bug,只不过发生频率比较低。<br />而Netty是一个高性能、异步事件驱动的NIO框架，它提供了对TCP、UDP和文件传输的支持，作为一个异步NIO框架，Netty的所有IO操作都是异步非阻塞的，通过Future-Listener机制，用户可以方便的主动获取或者通过通知机制获得IO操作结果。
