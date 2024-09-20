JavaIO
<a name="R0OAO"></a>
## 一、基础简介
在IO流的网络模型中，以常见的「客户端-服务端」交互场景为例；<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1680952528353-3490047c-8482-41e8-876a-104dcdc63ee8.png#averageHue=%23fcfbfb&clientId=u080aca0c-c2d6-4&from=paste&height=221&id=u82ed2889&originHeight=552&originWidth=2262&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=41693&status=done&style=none&taskId=u93112139-a23f-4fe8-8de4-8fb53a5063e&title=&width=904.8)<br />客户端与服务端进行通信「交互」，可能是同步或者异步，服务端进行「流」处理时，可能是阻塞或者非阻塞模式，当然也有自定义的业务流程需要执行，从处理逻辑看就是「读取数据-业务执行-应答写数据」的形式；<br />Java提供「三种」IO网络编程模型，即：「BIO同步阻塞」、「NIO同步非阻塞」、「AIO异步非阻塞」；
<a name="fxfWE"></a>
## 二、同步阻塞
<a name="DPCHw"></a>
### 1、模型图解
BIO即同步阻塞，服务端收到客户端的请求时，会启动一个线程处理，「交互」会阻塞直到整个流程结束；<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1680952552010-649745e0-f98b-4967-9749-3ec0cf8d89c1.png#averageHue=%23fbfafa&clientId=u080aca0c-c2d6-4&from=paste&height=185&id=ua585f4bc&originHeight=463&originWidth=2057&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=41401&status=done&style=none&taskId=u7928436e-887e-4d52-af18-563f561fcd3&title=&width=822.8)<br />这种模式如果在高并发且流程复杂耗时的场景下，客户端的请求响应会存在严重的性能问题，并且占用过多资源；
<a name="selOr"></a>
### 2、参考案例
【**服务端**】启动ServerSocket接收客户端的请求，经过一系列逻辑之后，向客户端发送消息，注意这里线程的10秒休眠；
```java
public class SocketServer01 {
    public static void main(String[] args) throws Exception {
        // 1、创建Socket服务端
        ServerSocket serverSocket = new ServerSocket(8080);
        // 2、方法阻塞等待，直到有客户端连接
        Socket socket = serverSocket.accept();
        // 3、输入流，输出流
        InputStream inStream = socket.getInputStream();
        OutputStream outStream = socket.getOutputStream();
        // 4、数据接收和响应
        int readLen = 0;
        byte[] buf = new byte[1024];
        if ((readLen=inStream.read(buf)) != -1){
            // 接收数据
            String readVar = new String(buf, 0, readLen) ;
            System.out.println("readVar======="+readVar);
        }
        // 响应数据
        Thread.sleep(10000);
        outStream.write("sever-8080-write;".getBytes());
        // 5、资源关闭
        IoClose.ioClose(outStream,inStream,socket,serverSocket);
    }
}
```
【**客户端**】Socket连接，先向ServerSocket发送请求，再接收其响应，由于Server端模拟耗时，Client处于长时间阻塞状态；
```java
public class SocketClient01 {
    public static void main(String[] args) throws Exception {
        // 1、创建Socket客户端
        Socket socket = new Socket(InetAddress.getLocalHost(), 8080);
        // 2、输入流，输出流
        OutputStream outStream = socket.getOutputStream();
        InputStream inStream = socket.getInputStream();
        // 3、数据发送和响应接收
        // 发送数据
        outStream.write("client-hello".getBytes());
        // 接收数据
        int readLen = 0;
        byte[] buf = new byte[1024];
        if ((readLen=inStream.read(buf)) != -1){
            String readVar = new String(buf, 0, readLen) ;
            System.out.println("readVar======="+readVar);
        }
        // 4、资源关闭
        IoClose.ioClose(inStream,outStream,socket);
    }
}
```
<a name="WcvVG"></a>
## 三、同步非阻塞
<a name="FDGUM"></a>
### 1、模型图解
NIO即同步非阻塞，服务端可以实现一个线程，处理多个客户端请求连接，服务端的并发能力得到极大的提升；<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1680952574657-68a86360-0b14-4d0c-96ec-895e0d9f52d9.png#averageHue=%23faf9f9&clientId=u080aca0c-c2d6-4&from=paste&height=198&id=u4beb06f7&originHeight=494&originWidth=1869&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=66926&status=done&style=none&taskId=u9bd2dc85-70f0-4771-a579-0a504204102&title=&width=747.6)<br />这种模式下客户端的请求连接都会注册到Selector多路复用器上，多路复用器会进行轮询，对请求连接的IO流进行处理；
<a name="rr8Uw"></a>
### 2、参考案例
【**服务端**】单线程可以处理多个客户端请求，通过轮询多路复用器查看是否有IO请求；
```java
public class SocketServer01 {
    public static void main(String[] args) throws Exception {
        try {
            //启动服务开启监听
            ServerSocketChannel socketChannel = ServerSocketChannel.open();
            socketChannel.socket().bind(new InetSocketAddress("127.0.0.1", 8989));
            // 设置非阻塞，接受客户端
            socketChannel.configureBlocking(false);
            // 打开多路复用器
            Selector selector = Selector.open();
            // 服务端Socket注册到多路复用器，指定兴趣事件
            socketChannel.register(selector, SelectionKey.OP_ACCEPT);
            // 多路复用器轮询
            ByteBuffer buffer = ByteBuffer.allocateDirect(1024);
            while (selector.select() > 0){
                Set<SelectionKey> selectionKeys = selector.selectedKeys();
                Iterator<SelectionKey> selectionKeyIter = selectionKeys.iterator();
                while (selectionKeyIter.hasNext()){
                    SelectionKey selectionKey = selectionKeyIter.next() ;
                    selectionKeyIter.remove();
                    if(selectionKey.isAcceptable()) {
                        // 接受新的连接
                        SocketChannel client = socketChannel.accept();
                        // 设置读非阻塞
                        client.configureBlocking(false);
                        // 注册到多路复用器
                        client.register(selector, SelectionKey.OP_READ);
                    } else if (selectionKey.isReadable()) {
                        // 通道可读
                        SocketChannel client = (SocketChannel) selectionKey.channel();
                        int len = client.read(buffer);
                        if (len > 0){
                            buffer.flip();
                            byte[] readArr = new byte[buffer.limit()];
                            buffer.get(readArr);
                            System.out.println(client.socket().getPort() + "端口数据:" + new String(readArr));
                            buffer.clear();
                        }
                    }
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```
【**客户端**】每隔3秒持续的向通道内写数据，服务端通过轮询多路复用器，持续的读取数据；
```java
public class SocketClient01 {
    public static void main(String[] args) throws Exception {
        try {
            // 连接服务端
            SocketChannel socketChannel = SocketChannel.open();
            socketChannel.connect(new InetSocketAddress("127.0.0.1", 8989));
            ByteBuffer writeBuffer = ByteBuffer.allocate(1024);
            String conVar = "client-hello";
            writeBuffer.put(conVar.getBytes());
            writeBuffer.flip();
            // 每隔3S发送一次数据
            while (true) {
                Thread.sleep(3000);
                writeBuffer.rewind();
                socketChannel.write(writeBuffer);
                writeBuffer.clear();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```
<a name="TKIPr"></a>
## 四、异步非阻塞
<a name="vx1u2"></a>
### 1、模型图解
AIO即异步非阻塞，对于通道内数据的「读」和「写」动作，都是采用异步的模式，对于性能的提升是巨大的；<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1680952598721-1e88fe3b-59d7-4f03-8bf2-46636368a35e.png#averageHue=%23f9f8f8&clientId=u080aca0c-c2d6-4&from=paste&height=218&id=u739f909b&originHeight=544&originWidth=2262&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=58182&status=done&style=none&taskId=ubc3cd5e6-90cb-45e7-ad42-b34d6dca7bf&title=&width=904.8)<br />这与常规的第三方对接模式很相似，本地服务在请求第三方服务时，请求过程耗时很大，会异步执行，第三方第一次回调，确认请求可以被执行；第二次回调则是推送处理结果，这种思想在处理复杂问题时，可以很大程度的提高性能，节省资源：
<a name="QyG5c"></a>
### 2、参考案例
【**服务端**】各种「accept」、「read」、「write」动作是异步，通过Future来获取计算的结果；
```java
public class SocketServer01 {
    public static void main(String[] args) throws Exception {
        // 启动服务开启监听
        AsynchronousServerSocketChannel socketChannel = AsynchronousServerSocketChannel.open() ;
        socketChannel.bind(new InetSocketAddress("127.0.0.1", 8989));
        // 指定30秒内获取客户端连接，否则超时
        Future<AsynchronousSocketChannel> acceptFuture = socketChannel.accept();
        AsynchronousSocketChannel asyChannel = acceptFuture.get(30, TimeUnit.SECONDS);

        if (asyChannel != null && asyChannel.isOpen()){
            // 读数据
            ByteBuffer inBuffer = ByteBuffer.allocate(1024);
            Future<Integer> readResult = asyChannel.read(inBuffer);
            readResult.get();
            System.out.println("read："+new String(inBuffer.array()));

            // 写数据
            inBuffer.flip();
            Future<Integer> writeResult = asyChannel.write(ByteBuffer.wrap("server-hello".getBytes()));
            writeResult.get();
        }

        // 关闭资源
        asyChannel.close();
    }
}
```
【**客户端**】相关「connect」、「read」、「write」方法调用是异步的，通过Future来获取计算的结果；
```java
public class SocketClient01 {
    public static void main(String[] args) throws Exception {
        // 连接服务端
        AsynchronousSocketChannel socketChannel = AsynchronousSocketChannel.open();
        Future<Void> result = socketChannel.connect(new InetSocketAddress("127.0.0.1", 8989));
        result.get();

        // 写数据
        String conVar = "client-hello";
        ByteBuffer reqBuffer = ByteBuffer.wrap(conVar.getBytes());
        Future<Integer> writeFuture = socketChannel.write(reqBuffer);
        writeFuture.get();

        // 读数据
        ByteBuffer inBuffer = ByteBuffer.allocate(1024);
        Future<Integer> readFuture = socketChannel.read(inBuffer);
        readFuture.get();
        System.out.println("read："+new String(inBuffer.array()));

        // 关闭资源
        socketChannel.close();
    }
}
```
<a name="qhQRg"></a>
## 五、Reactor模型
<a name="YjnKu"></a>
### 1、模型图解
这部分内容，可以参考「**Doug Lea的《IO》**」文档，查看更多细节；
<a name="clxgM"></a>
#### 1.1 Reactor设计原理
Reactor模式基于事件驱动设计，也称为「反应器」模式或者「分发者」模式；服务端收到多个客户端请求后，会将请求分派给对应的线程处理；<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1680952645594-7c64b714-a5b8-4c03-a667-0bd7f30cf360.png#averageHue=%23fcfcfc&clientId=u080aca0c-c2d6-4&from=paste&height=276&id=u412d113c&originHeight=690&originWidth=2076&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=80186&status=done&style=none&taskId=u27dc2e01-d4de-4518-918a-15edc9c3a34&title=&width=830.4)<br />Reactor：负责事件的监听和分发；Handler：负责处理事件，核心逻辑「`read`读」、「`decode`解码」、「`compute`业务计算」、「`encode`编码」、「`send`应答数据」；
<a name="LWUCW"></a>
#### 1.2 单Reactor单线程
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1680952662290-13178b51-5723-4f61-9003-792b1f52e40a.png#averageHue=%23fcfbfb&clientId=u080aca0c-c2d6-4&from=paste&height=444&id=u68375681&originHeight=1110&originWidth=2080&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=107892&status=done&style=none&taskId=u64ea4f56-5bc0-4dcb-950b-d91e7e90e5e&title=&width=832)<br />【1】Reactor线程通过select监听客户端的请求事件，收到事件后通过Dispatch进行分发；<br />【2】如果是建立连接请求事件，Acceptor通过「accept」方法获取连接，并创建一个Handler对象来处理后续业务；<br />【3】如果不是连接请求事件，则Reactor会将该事件交由当前连接的Handler来处理；<br />【4】在Handler中，会完成相应的业务流程；<br />这种模式将所有逻辑「连接、读写、业务」放在一个线程中处理，避免多线程的通信，资源竞争等问题，但是存在明显的并发和性能问题；
<a name="ZAOFN"></a>
#### 1.3 单Reactor多线程
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1680952699156-f56652cf-5c47-423e-8f02-e2cac6498749.png#averageHue=%23fcfbfb&clientId=u080aca0c-c2d6-4&from=paste&height=372&id=ue51d0f8b&originHeight=931&originWidth=2087&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=111181&status=done&style=none&taskId=u76d28b84-cd71-4951-baf0-00ed477902a&title=&width=834.8)<br />【1】Reactor线程通过select监听客户端的请求事件，收到事件后通过Dispatch进行分发；<br />【2】如果是建立连接请求事件，Acceptor通过「accept」方法获取连接，并创建一个Handler对象来处理后续业务；<br />【3】如果不是连接请求事件，则Reactor会将该事件交由当前连接的Handler来处理；<br />【4】在Handler中，只负责事件响应不处理具体业务，将数据发送给Worker线程池来处理；<br />【5】Worker线程池会分配具体的线程来处理业务，最后把结果返回给Handler做响应；<br />这种模式将业务从Reactor单线程分离处理，可以让其更专注于事件的分发和调度，Handler使用多线程也充分的利用cpu的处理能力，导致逻辑变的更加复杂，Reactor单线程依旧存在高并发的性能问题；
<a name="pA5F3"></a>
#### 1.4 主从Reactor多线程
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1680952722491-298ab355-3603-41f7-952a-3a8962b218eb.png#averageHue=%23fcfbfb&clientId=u080aca0c-c2d6-4&from=paste&height=356&id=u0f476994&originHeight=891&originWidth=2298&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=125387&status=done&style=none&taskId=u21bd83cb-95c1-46a4-bd9c-3cb7ce7cb26&title=&width=919.2)<br />【1】 MainReactor主线程通过select监听客户端的请求事件，收到事件后通过Dispatch进行分发；<br />【2】如果是建立连接请求事件，Acceptor通过「accept」方法获取连接，之后MainReactor将连接分配给SubReactor；<br />【3】如果不是连接请求事件，则MainReactor将连接分配给SubReactor，SubReactor调用当前连接的Handler来处理；<br />【4】在Handler中，只负责事件响应不处理具体业务，将数据发送给Worker线程池来处理；<br />【5】Worker线程池会分配具体的线程来处理业务，最后把结果返回给Handler做响应；<br />这种模式Reactor线程分工明确，MainReactor负责接收新的请求连接，SubReactor负责后续的交互业务，适应于高并发的处理场景，是Netty组件通信框架的所采用的模式；
<a name="pgR9S"></a>
### 2、参考案例
【**服务端**】提供两个`EventLoopGroup`，「`ParentGroup`」主要是用来接收客户端的请求连接，真正的处理是转交给「`ChildGroup`」执行，即Reactor多线程模型；
```java
@Slf4j
public class NettyServer {
    public static void main(String[] args) {
        // EventLoop组，处理事件和IO
        EventLoopGroup parentGroup = new NioEventLoopGroup();
        EventLoopGroup childGroup = new NioEventLoopGroup();
        try {
            // 服务端启动引导类
            ServerBootstrap serverBootstrap = new ServerBootstrap();
            serverBootstrap.group(parentGroup, childGroup)
                    .channel(NioServerSocketChannel.class).childHandler(new ServerChannelInit());

            // 异步IO的结果
            ChannelFuture channelFuture = serverBootstrap.bind(8989).sync();
            channelFuture.channel().closeFuture().sync();
        } catch (Exception e){
            e.printStackTrace();
        } finally {
            parentGroup.shutdownGracefully();
            childGroup.shutdownGracefully();
        }
    }
}

class ServerChannelInit extends ChannelInitializer<SocketChannel> {
    @Override
    protected void initChannel(SocketChannel socketChannel) {
        // 获取管道
        ChannelPipeline pipeline = socketChannel.pipeline();
        // 编码、解码器
        pipeline.addLast(new StringDecoder(CharsetUtil.UTF_8));
        pipeline.addLast(new StringEncoder(CharsetUtil.UTF_8));
        // 添加自定义的handler
        pipeline.addLast("serverHandler", new ServerHandler());
    }
}

class ServerHandler extends ChannelInboundHandlerAdapter {
    /**
     * 通道读和写
     */
    @Override
    public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
        System.out.println("Server-Msg【"+msg+"】");
        TimeUnit.MILLISECONDS.sleep(2000);
        String nowTime = DateTime.now().toString(DatePattern.NORM_DATETIME_PATTERN) ;
        ctx.channel().writeAndFlush("hello-client；time：" + nowTime);
        ctx.fireChannelActive();
    }
    @Override
    public void exceptionCaught(ChannelHandlerContext ctx,Throwable cause) throws Exception {
        cause.printStackTrace();
        ctx.close();
    }
}
```
【**客户端**】通过`Bootstrap`类，与服务器建立连接，服务端通过`ServerBootstrap`启动服务，绑定在8989端口，然后服务端和客户端进行通信；
```java
public class NettyClient {
    public static void main(String[] args) {
        // EventLoop处理事件和IO
        NioEventLoopGroup eventLoopGroup = new NioEventLoopGroup();
        try {
            // 客户端通道引导
            Bootstrap bootstrap = new Bootstrap();
            bootstrap.group(eventLoopGroup)
                    .channel(NioSocketChannel.class).handler(new ClientChannelInit());

            // 异步IO的结果
            ChannelFuture channelFuture = bootstrap.connect("localhost", 8989).sync();
            channelFuture.channel().closeFuture().sync();
        } catch (Exception e){
            e.printStackTrace();
        } finally {
            eventLoopGroup.shutdownGracefully();
        }
    }
}

class ClientChannelInit extends ChannelInitializer<SocketChannel> {
    @Override
    protected void initChannel(SocketChannel socketChannel) {
        // 获取管道
        ChannelPipeline pipeline = socketChannel.pipeline();
        // 编码、解码器
        pipeline.addLast(new StringDecoder(CharsetUtil.UTF_8));
        pipeline.addLast(new StringEncoder(CharsetUtil.UTF_8));
        // 添加自定义的handler
        pipeline.addLast("clientHandler", new ClientHandler());
    }
}

class ClientHandler extends ChannelInboundHandlerAdapter {
    /**
     * 通道读和写
     */
    @Override
    public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
        System.out.println("Client-Msg【"+msg+"】");
        TimeUnit.MILLISECONDS.sleep(2000);
        String nowTime = DateTime.now().toString(DatePattern.NORM_DATETIME_PATTERN) ;
        ctx.channel().writeAndFlush("hello-server；time：" + nowTime);
    }
    @Override
    public void channelActive(ChannelHandlerContext ctx) throws Exception {
        ctx.channel().writeAndFlush("channel...active");
    }
    @Override
    public void exceptionCaught(ChannelHandlerContext ctx,Throwable cause) throws Exception {
        cause.printStackTrace();
        ctx.close();
    }
}
```
