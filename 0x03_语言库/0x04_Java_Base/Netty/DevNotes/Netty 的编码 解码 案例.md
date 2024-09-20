Java Netty
<a name="Cihsf"></a>
## 1、半包粘包
例如发送两个数据包给服务器，由于服务端一次读取到的字节数不一定的分<br />没有半包和拆包：服务器分两次读取到两个地理的数据包，这个情况没有拆包和粘包的情况

- 粘包：服务器一次收到两个数据包，在一起收到的
- 拆包：第一次读取到完成的第一个包和第二个包的一部分内容，第二次读取到第二个包的剩余内容
- 整包：第一次读取到第一包的部分内容，第二次读取到第一个包的剩余部分和第二个包的全部
- 多次拆包：如果接收滑窗非常小，数据量大的时候发生多次发送的接收的情况
<a name="vxyAz"></a>
### 为什么会出现半包和粘包
Netty粘包和拆包的问题，本质上归结于TCP的粘包和拆包。
:::info
网络上发送一个完整的数据包时，可能会被TCP拆分成多个包进行发送，也可能把多个小的包封装成一个大的数据包发送，这就是所谓的TCP的粘包和拆包问题
:::
1、HTTP 中有一个 Nagle 算法，每个报文都是一段的，使用网络发送发现网络效率低，然后 HTTP 设置一个算法，设置到一定程度发，所以出现一些延时，提高销量，所以形成了粘包<br />2、HTTP缓冲区引起的，报文段大的时候的时候直接弄在一起发送过去。
<a name="AjXjH"></a>
### 怎么解决
不断的从 TCP 的缓冲区中读取数据，每次读取完成都需要判断是否是一个完整的数据包<br />如果是读取的数据不足以拼接成一个完整的业务数据包，那就保留该数据，继续从 TCP 缓冲区中读取，直到得到一个完整的数据包
> - 定长
> - 分隔符
> - 基于长度的变长包

如果当前读到的数据加上已经读取到的数据足以拼接成一个数据包，那就讲已经读取的数据拼接本次读取的数据，构成一个完整的业务数据包传递到业务逻辑上，多余的数据保留，方便下次的读取或者数据链接。<br />TCP以流的方式进行数据传输，上层应用协议为了对消息进行区分，往往采用如下4种方式。<br />（1）消息长度固定：累计读取到固定长度为LENGTH之后就认为读取到了一个完整的消息。然后将计数器复位，重新开始读下一个数据报文。<br />（2）回车换行符作为消息结束符：在文本协议中应用比较广泛。<br />（3）将特殊的分隔符作为消息的结束标志，回车换行符就是一种特殊的结束分隔符。<br />（4）通过在消息头中定义长度字段来标示消息的总长度。<br />Netty中针对这四种场景均有对应的解码器作为解决方案，比如：<br />（1）通过`FixedLengthFrameDecoder`定长解码器来解决定长消息的黏包问题；<br />（2）通过`LineBasedFrameDecoder`和StringDecoder来解决以回车换行符作为消息结束符的TCP黏包的问题；<br />（3）通过`DelimiterBasedFrameDecoder`特殊分隔符解码器来解决以特殊符号作为消息结束符的TCP黏包问题；<br />（4）通过`LengthFieldBasedFrameDecoder`自定义长度解码器解决TCP黏包问题。
<a name="tqbmc"></a>
## 2、Netty常用的解码器
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1635378547789-34a53e54-0cdc-4f29-a9c7-be4c583f0d86.png#averageHue=%23fdfde2&clientId=ua87560e9-8ba0-4&from=paste&height=375&id=u4a9d3788&originHeight=749&originWidth=707&originalType=binary&ratio=1&rotation=0&showTitle=false&size=50723&status=done&style=shadow&taskId=u3ee026be-c2aa-4c6e-9d7e-71f67a30c78&title=&width=354)
<a name="md4Uj"></a>
### `LineBasedFrameDecoder`
回车换行解码器<br />配合`StringDecoder`
<a name="Tqt8s"></a>
### `DelimiterBasedFrameDecoder`
分隔符解码器
<a name="RsTBG"></a>
### `FixedLengthFrameDecoder`
固定长度解码器，从缓冲池读取固定长度的内容。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1635261024203-9ee9e74f-2375-4d6f-9610-e639d5cfc1d0.png#averageHue=%23f5f3f2&clientId=ucf95d36e-b03d-4&from=paste&height=556&id=ub0cc62e6&originHeight=1668&originWidth=2942&originalType=binary&ratio=1&rotation=0&showTitle=false&size=871587&status=done&style=shadow&taskId=u9973bf80-3e19-4d8d-832f-55ccc5dfcaf&title=&width=980.6666666666666)
<a name="QdAzK"></a>
### `LengthFieldBasedFrameDecoder`
不能超过1024个字节不然会报错<br />基于'长度'解码器(私有协议最常用)
<a name="rFgYZ"></a>
## 3、拆包的类
<a name="zbe7O"></a>
### Netty拆包的基类 - `ByteToMessageDecoder`
自解析<br />内部维护了一个数据累积器`cumulation`，每次读取到数据都会不断累加，然后尝试对累加到`cumulation`<br />的数据进行拆包，拆成一个完整的业务数据包。每次都将读取到的数据通过内存拷贝的方式， 累积到`cumulation`中<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1635262019795-df7c9258-ab74-4a1f-bfa4-098613217b92.png#averageHue=%23fcfbfa&clientId=ue90521b7-2d2d-4&from=paste&height=506&id=u1258baa8&originHeight=1517&originWidth=3281&originalType=binary&ratio=1&rotation=0&showTitle=false&size=494870&status=done&style=shadow&taskId=u0cc144f1-afa0-4cae-8193-237e9c0ccdf&title=&width=1093.6666666666667)<br />然后调用子类的 `decode` 方法对累积的数据尝试进行拆包
<a name="MXVWU"></a>
### `LengthFieldPrepender`
长度解码器<br />参数说明

- `lengthFieldLength`：长度属性的字节长度
- `lengthIncludesLengthFieldLength`：false，长度字节不算在总长度中，true，算到总长度中
<a name="CoeGi"></a>
### `LengthFieldBasedFrameDecoder`
参数说明

- `maxFrameLength`：包的最大长度
- `lengthFieldOffset`：长度属性的起始位（偏移位），包中存放长度属性字段的起始位置
- `lengthFieldLength`：长度属性的长度 
- `lengthAdjustment`：长度调节值，在总长被定义为包含包头长度时，修正信息长度
- `initialBytesToStrip`：跳过的字节数，根据需要跳过`lengthFieldLength`个字节，以便接收端直接接受到不含“长度属性”的内容

编解码器的作用就是讲原始字节数据与自定义的消息对象进行互转

- `Decoder`(解码器)
- `Encoder`(编码器)

支持业界主流的序列化框架

- Protobuf
- Jboss Marshalling
- Java Serialization

解码1拆包：把整个 ByteBuf 数据，分成一个个 ByteBuf，每个表示一个包<br />解码2反序列化：把每个包的 ByteBuf 字节数组转成 Java Object
```java
import io.netty.bootstrap.Bootstrap;
import io.netty.buffer.Unpooled;
import io.netty.channel.ChannelFuture;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelOption;
import io.netty.channel.EventLoopGroup;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.SocketChannel;
import io.netty.channel.socket.nio.NioSocketChannel;
import io.netty.handler.codec.DelimiterBasedFrameDecoder;

public class StickyDemoClient {

    public static void main(String[] args) throws Exception {
        int port = 8080;
        if (args != null && args.length > 0) {
            try {
                port = Integer.valueOf(args[0]);
            } catch (NumberFormatException e) {
            }
        }
        new StickyDemoClient().connect(port, "127.0.0.1");
    }

    public void connect(int port, String host) throws Exception {
        // 工作线程组 
        EventLoopGroup group = new NioEventLoopGroup();
        try {
            Bootstrap b = new Bootstrap();
            b.group(group).channel(NioSocketChannel.class)
                .option(ChannelOption.TCP_NODELAY, true)
                .handler(new ChannelInitializer<SocketChannel>() {
                    @Override
                    public void initChannel(SocketChannel ch) throws Exception {
                        //ch.pipeline().addLast("framer", new FixedLengthFrameDecoder(139));
                        // ch.pipeline().addLast("framer", new StickyDemoDecodeHandlerV2(
                        //       Unpooled.wrappedBuffer(new byte[] { '#' })));
                        ch.pipeline().addLast("framer", new DelimiterBasedFrameDecoder(8192,
                                                                                       Unpooled.wrappedBuffer(new byte[] { '#' })));

                        ch.pipeline().addLast(new StickyDemoClientHandler());

                    }
                });

            // 发起异步连接操作
            ChannelFuture f = b.connect(host, port).sync();

            // 等待客户端链路关闭
            f.channel().closeFuture().sync();
        } finally {
            // 优雅退出，释放线程池资源
            group.shutdownGracefully();
        }
    }
}
```
```java
import io.netty.buffer.ByteBuf;
import io.netty.buffer.Unpooled;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.SimpleChannelInboundHandler;
import io.netty.util.CharsetUtil;

public class StickyDemoClientHandler extends SimpleChannelInboundHandler<ByteBuf> {

    private static String[] alphabets = {"A", "B", "C", "D", "E", "F", "G", "H", "I",
                                         "J", "K", "L", "M", "N", "O", "P"};

    @Override
    public void channelActive(ChannelHandlerContext ctx) {
        for(int i=0; i<10; i++) {
            StringBuilder builder = new StringBuilder();
            builder.append("这是第");
            builder.append(i);
            builder.append("条消息, 内容是：");
            for(int j=0; j<100; j++) {
                builder.append(alphabets[i]);
            }
            builder.append("......");
            builder.append("#");


            System.out.println(builder.toString().getBytes().length);

            ctx.writeAndFlush(Unpooled.copiedBuffer(builder.toString(),
                                                    CharsetUtil.UTF_8));
        }
    }

    @Override
    public void channelRead0(ChannelHandlerContext ctx, ByteBuf in) {
        System.out.println("客户端接收到消息：" + in.toString(CharsetUtil.UTF_8));
    }

    @Override
    public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) {
        cause.printStackTrace();
        ctx.close();
    }
}
```
```java
import io.netty.buffer.ByteBuf;
import io.netty.buffer.ByteBufAllocator;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelInboundHandlerAdapter;

import java.util.ArrayList;
import java.util.List;

public class StickyDemoDecodeHandler extends ChannelInboundHandlerAdapter {

    //存放待拆包数据的缓冲区
    private ByteBuf cache;
    private int frameLength;

    public StickyDemoDecodeHandler(int length) {
        this.frameLength = length;
    }

    static ByteBuf expandCache(ByteBufAllocator alloc, ByteBuf cache, int readable) {
        ByteBuf oldCache = cache;
        cache = alloc.buffer(oldCache.readableBytes() + readable);
        cache.writeBytes(oldCache);
        oldCache.release();
        return cache;
    }

    @Override
    public void channelRead(ChannelHandlerContext ctx, Object msg) {

        ByteBuf data = (ByteBuf) msg;
        try {
            //读取每一个消息，创建缓冲区
            if (cache == null) {
                cache = ctx.alloc().buffer(1024);
            } else {
                //如果现有的缓冲区容量太小，无法容纳原有数据+新读入的数据，就扩容（重新创建一个大的，并把数据拷贝过去）
                if (cache.writerIndex() > cache.maxCapacity() - data.readableBytes()) {
                    cache = expandCache(ctx.alloc(), cache, data.readableBytes());
                }
            }
            //把新的数据读入缓冲区
            cache.writeBytes(data);

            //每次读取frameLength（定长）的数据，做为一个包，存储起来 
            List<ByteBuf> output = new ArrayList<>();
            while (cache.readableBytes() >= frameLength) {
                output.add(cache.readBytes(frameLength));
            }

            //还有部分数据不够一个包，10， 15， 一个10个，还剩5个
            if (cache.isReadable()) {
                cache.discardReadBytes();
            }

            for (int i = 0; i < output.size(); i++) {
                ctx.fireChannelRead(output.get(i));
            }
        } finally {
            data.release();
        }

    }


    @Override
    public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) {
        cause.printStackTrace();
        ctx.close();
    }
}
```
```java
import io.netty.buffer.ByteBuf;
import io.netty.buffer.ByteBufAllocator;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelInboundHandlerAdapter;

import java.util.ArrayList;
import java.util.List;

public class StickyDemoDecodeHandlerV2 extends ChannelInboundHandlerAdapter {
    private ByteBuf cache;
    private byte delimiter; //包分隔符

    public StickyDemoDecodeHandlerV2(ByteBuf delimiter) {
        if (delimiter == null) {
            throw new NullPointerException("delimiter");
        }
        if (!delimiter.isReadable()) {
            throw new IllegalArgumentException("empty delimiter");
        }

        this.delimiter =  delimiter.readByte();
        ;
    }

    static ByteBuf expandCache(ByteBufAllocator alloc, ByteBuf cache, int readable) {
        ByteBuf oldCache = cache;
        cache = alloc.buffer(oldCache.readableBytes() + readable);
        cache.writeBytes(oldCache);
        oldCache.release();
        return cache;
    }

    @Override
    public void channelRead(ChannelHandlerContext ctx, Object msg) {

        ByteBuf data = (ByteBuf) msg;
        try {
            if (cache == null) {
                cache = ctx.alloc().buffer(1024);
            } else {
                if (cache.writerIndex() > cache.maxCapacity() - data.readableBytes()) {
                    cache = expandCache(ctx.alloc(), cache, data.readableBytes());
                }
            }
            cache.writeBytes(data);

            List<ByteBuf> output = new ArrayList<>();

            int frameIndex = 0;
            int frameEndIndex = 0;
            int length = cache.readableBytes();
            while (frameIndex <= length) {
                frameEndIndex = cache.indexOf(frameIndex + 1, length, delimiter);

                if (frameEndIndex == -1) {
                    cache.discardReadBytes();
                    break;
                }

                output.add(cache.readBytes(frameEndIndex - frameIndex));
                cache.skipBytes(1);
                frameIndex = frameEndIndex + 1;

            }

            if (cache.isReadable()) {
                cache.discardReadBytes();
            }

            for (int i = 0; i < output.size(); i++) {
                ctx.fireChannelRead(output.get(i));
            }
        } finally {
            data.release();
        }

    }

    @Override
    public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) {
        cause.printStackTrace();
        ctx.close();
    }
}
```
```java
import io.netty.bootstrap.ServerBootstrap;
import io.netty.buffer.Unpooled;
import io.netty.channel.ChannelFuture;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.EventLoopGroup;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.SocketChannel;
import io.netty.channel.socket.nio.NioServerSocketChannel;
import io.netty.handler.codec.DelimiterBasedFrameDecoder;

public class StickyDemoServer {

    public static void main(String[] args) throws Exception {
        int port = 8080;
        if (args != null && args.length > 0) {
            try {
                port = Integer.valueOf(args[0]);
            } catch (NumberFormatException e) {
                // 采用默认值
            }
        }
        new StickyDemoServer().bind(port);
    }

    public void bind(int port) throws Exception {
        // 第一步：
        // 配置服务端的NIO线程组
        // 主线程组, 用于接受客户端的连接，但是不做任何具体业务处理，像老板一样，负责接待客户，不具体服务客户
        EventLoopGroup bossGroup = new NioEventLoopGroup(1);
        // 工作线程组, 老板线程组会把任务丢给他，让手下线程组去做任务，服务客户
        EventLoopGroup workerGroup = new NioEventLoopGroup();
        try {
            // 类ServerBootstrap用于配置Server相关参数，并启动Server
            ServerBootstrap b = new ServerBootstrap();

            // 链式调用
            // 配置parentGroup和childGroup
            b.group(bossGroup, workerGroup)
                    // 配置Server通道
                    .channel(NioServerSocketChannel.class)
                    // 配置通道的ChannelPipeline
                    .childHandler(new ChildChannelHandler());

            // 绑定端口，并启动server，同时设置启动方式为同步
            ChannelFuture f = b.bind(port).sync();

            System.out.println(StickyDemoServer.class.getName() + " 启动成功，在地址[" + f.channel().localAddress() + "]上等待客户请求......");

            // 等待服务端监听端口关闭
            f.channel().closeFuture().sync();
        } finally {
            // 优雅退出，释放线程池资源
            bossGroup.shutdownGracefully();
            workerGroup.shutdownGracefully();
        }
    }

    private class ChildChannelHandler extends ChannelInitializer<SocketChannel> {
        @Override
        protected void initChannel(SocketChannel ch) throws Exception {
            //ch.pipeline().addLast("framer", new FixedLengthFrameDecoder(139));
            ch.pipeline().addLast("framer", new DelimiterBasedFrameDecoder(8192, Unpooled.wrappedBuffer(new byte[] { '#' })));
             //ch.pipeline().addLast("framer", new StickyDemoDecodeHandler(139));
            // ch.pipeline().addLast("framer", new StickyDemoDecodeHandlerV2(
            //       Unpooled.wrappedBuffer(new byte[] { '#' })));

            ch.pipeline().addLast(new StickyDemoServerHandler());
        }
    }
}
```
```java
import io.netty.buffer.ByteBuf;
import io.netty.buffer.Unpooled;
import io.netty.channel.ChannelFutureListener;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelInboundHandlerAdapter;
import io.netty.util.CharsetUtil;

public class StickyDemoServerHandler extends ChannelInboundHandlerAdapter {

    @Override
    public void channelRead(ChannelHandlerContext ctx, Object msg) {
        ByteBuf in = (ByteBuf) msg;
        System.out.println(
            "服务器接收到消息：" + in.toString(CharsetUtil.UTF_8));
        ctx.write(in);
        // ctx.write(Unpooled.copiedBuffer("#", CharsetUtil.UTF_8));
        //compositeBuffer.addComponent(in);
        // ByteBuf buf =  ctx.alloc().directBuffer();
        // buf.writeBytes("#".getBytes());
        // CompositeByteBuf compositeBuffer = ctx.alloc().compositeBuffer();
        //  compositeBuffer.addComponents(true, in, buf);


        // ctx.write(compositeBuffer);
    }

    @Override
    public void channelReadComplete(ChannelHandlerContext ctx)
        throws Exception {
        ctx.writeAndFlush(Unpooled.EMPTY_BUFFER)
            .addListener(ChannelFutureListener.CLOSE);
    }

    @Override
    public void exceptionCaught(ChannelHandlerContext ctx,
                                Throwable cause) {
        cause.printStackTrace();
        ctx.close();
    }
}
```
