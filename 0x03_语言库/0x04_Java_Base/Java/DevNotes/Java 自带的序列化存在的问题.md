Java 序列化<br />首先解释一下序列化的概念<br />把对象转换为字节序列的过程称为对象的序列化。<br />什么情况下需要用到序列化

- 把的内存中的对象状态保存到一个文件中或者数据库中时候；
- 使用套接字在网络上传送对象的时候；
- 通过RMI传输对象的时候；

总之一句话，只要对内存中的对象进行持久化或网络传输, 都需要进行序列化和反序列化。<br />如果研究过一些常用的RPC通信框架，可以发现它们极少使用Java自带的序列化，什么原因？
<a name="vZHP2"></a>
## 1、无法跨语言
通过Java的原生`Serializable`接口与`ObjectOutputStream`实现的序列化，只有java语言自己能通过`ObjectInputStream`来解码，其他语言，如C、C++、Python等等，都无法对其实现解码。而在实际开发生产中，有时不可避免的需要基于不同语言编写的应用程序之间进行通信，这个时候Java自带的序列化就无法搞定了。
<a name="kG9sc"></a>
## 2、性能差
来对比Java自带的序列化与NIO中的ByteBuffer编码的性能<br />UserInfo类
```java
import java.io.Serializable;
import java.nio.ByteBuffer;
public class UserInfo implements Serializable {
    private static final long serialVersionUID = 1L;

    private Long id;

    private String name;

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public byte[] codeC() {
        ByteBuffer buffer = ByteBuffer.allocate(1024);
        byte[] value = this.name.getBytes();
        buffer.putInt(value.length);
        buffer.put(value);
        buffer.putLong(this.id);
        buffer.flip();
        byte[] result = new byte[buffer.remaining()];
        buffer.get(result);
        return result;
    }

    public byte[] codeC(ByteBuffer buffer) {
        buffer.clear();
        byte[] value = this.name.getBytes();
        buffer.putInt(value.length);
        buffer.put(value);
        buffer.putLong(this.id);
        buffer.flip();
        byte[] result = new byte[buffer.remaining()];
        buffer.get(result);
        return result;
    }

}
```
测试类
```java
import java.io.ByteArrayOutputStream;
import java.io.ObjectOutputStream;
import java.nio.ByteBuffer;

public class MainTest {

    public static void main(String[] args) throws Exception {
        UserInfo info = new UserInfo();
        info.setId(1L);
        info.setName("Tom");

        int loop = 100_0000;
        ByteArrayOutputStream bout = null;
        ObjectOutputStream out = null;
        long start = System.currentTimeMillis();
        for (int i = 0; i < loop; i++) {
            bout = new ByteArrayOutputStream();
            out = new ObjectOutputStream(bout);
            out.flush();
            out.close();
            byte[] b = bout.toByteArray();
            bout.close();
        }
        System.out.println("jdk serializable time : " + (System.currentTimeMillis() - start) + " ms");

        System.out.println("------------------------------");

        start = System.currentTimeMillis();
        ByteBuffer buffer = ByteBuffer.allocate(1024);
        for (int i = 0; i < loop; i++) {
            byte[] bytes = info.codeC(buffer);
        }
        System.out.println("ByteBuffer time : " + (System.currentTimeMillis() - start) + " ms");
    }
}
```
输出结果：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623725752798-8097607e-26a3-434f-9afa-17f339e630e3.png#clientId=u26742a31-8e1e-4&from=paste&id=ub52b05c9&originHeight=335&originWidth=849&originalType=url&ratio=3&status=done&style=none&taskId=ub0927a1f-e366-4026-8221-fd193b8d71a)
<a name="DaIAH"></a>
## 3、序列后的码流太大
java序列化的大小是二进制编码的5倍多！<br />序列化后的二进制数组越大，占用的存储空间就越多，如果是进行网络传输，相对占用的带宽就更多，也会影响系统的性能。<br />来测试一下：
```java
import java.io.ByteArrayOutputStream;
import java.io.ObjectOutputStream;

public class MainTest {

    public static void main(String[] args) throws Exception {
        UserInfo info = new UserInfo();
        info.setId(1L);
        info.setName("Tom");
        ByteArrayOutputStream bout = new ByteArrayOutputStream();
        ObjectOutputStream out = new ObjectOutputStream(bout);
        out.writeObject(info);
        out.flush();
        out.close();
        byte[] b = bout.toByteArray();
        System.out.println("jdk serializable: " + b.length);
        bout.close();
        System.out.println("ByteBuffer : " + info.codeC().length);
    }
}
```
输出结果：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623725753158-a743a9a7-fb3c-4368-9235-dd519ea47f8a.png#clientId=u26742a31-8e1e-4&from=paste&id=uc986eff9&originHeight=339&originWidth=812&originalType=url&ratio=3&status=done&style=none&taskId=u688fe48a-3c9d-4106-ac1d-2e305599d2c)<br />正是由于Java自带的序列化存在这些问题，开源社区涌现出很多优秀的序列化框架。<br />看一下比较主流的序列化框架的特点，可以从以下方面对比：

- 是否支持跨平台，跨语言是否符合系统要求
- 编码后的码流大小
- 效率
- 使用上是否便捷（包括社区维护、API复杂度等）

可以方便结合自身业务来选择适合自己的一款，来优化系统的序列化性能。
<a name="A0HwD"></a>
### Protobuf

- 结构化数据存储格式（xml,json等）
- 高性能编解码技术
- 语言和平台无关，扩展性好，支持java、C++、Python三种语言。
- Google开源

使用Protobuf进行序列化，他和XML，json一样都有自己的语法，xml的后缀是.xml，json文件的后缀是.json，自然Protobuf文件的后缀就是.proto。<br />下面使用Protobuf来封装一段消息，通过一个案例简单介绍一下它的使用。<br />首先用Protobuf的语法格式来写一段需要序列化的对象，命名格式为：Msg.proto
```java
syntax = "proto3";
option java_package = "cn.edu.hust.netty.demo10";
option java_outer_classname = "MessageProto";

message RequestMsg{
    bytes msgType = 1;
    string receiveOne = 2;
    string msg = 3;
}

message ResponseMsg{
    bytes msgType = 1;
    string receiveOne = 2;
    string msg = 3;
}
```
:::tips
`syntax = "proto3";`表示在protoc3中需要指定对应的语法版本，否则会报如下错误<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1623772578821-e95f5df3-6c06-4b22-9989-280e3fe4bdb0.png#clientId=ufa8f92f4-389d-4&from=paste&height=143&id=QKpqT&originHeight=428&originWidth=3323&originalType=binary&ratio=3&size=478677&status=done&style=none&taskId=ub69d7f07-ce7d-41ae-834e-9ac2f53f009&width=1107.6666666666667)<br />在protoc3中不允许`required`字段<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1623772711672-7c05c7a2-91d1-482e-8bd9-7d775c30af09.png#clientId=ufa8f92f4-389d-4&from=paste&height=180&id=ud31294b4&originHeight=540&originWidth=3323&originalType=binary&ratio=3&size=616964&status=done&style=none&taskId=u0eaa9fca-8be6-4a9e-b0d2-68a0adbc2ab&width=1107.6666666666667)
:::
关于Message.proto中的语法格式，简单就上面的语法说明一下：

- option java_package：表示生成的.java文件的包名
- option java_outer_classname：生成的java文件的文件名
- message ：为他的基本类型，如同java中的class一样

字段修饰符：

- required：一个格式良好的消息一定要含有1个这种字段。表示该值是必须要设置的；
- optional：消息格式中该字段可以有0个或1个值（不超过1个）。
- repeated：在一个格式良好的消息中，这种字段可以重复任意多次（包括0次）。重复的值的顺序会被保留。表示该值可以重复，相当于java中的List。

字符类型稍微有些不同：double,float,int32,int64,bool(boolean) ,string,bytes。稍微有些不同，String，boolean，int有差别。<br />另外可以看到上面3个字段分别赋值了，这个值是什么意思呢？消息定义中，每个字段都有唯一的一个数字标识符。这些标识符是用来在消息的二进制格式中识别各个字段的，一旦开始使用就不能够再改变。注：[1,15]之内的标识号在编码的时候会占用一个字节。[16,2047]之内的标识号则占用2个字节。所以应该为那些频繁出现的消息元素保留 [1,15]之内的标识号。<br />关于Protobuf 的语法就简单的介绍这么多，更多细节可以查阅文档吧。下面开始使用Protobuf 来进行序列化。<br />首先在工程中引入protobuf的jar包，目前官方版本最高3.2，这里用3.0的：
```xml
<dependency>
    <groupId>com.google.protobuf</groupId>
    <artifactId>protobuf-java</artifactId>
    <version>3.0.2</version>
</dependency>
```
Protobuf的文件已经定义好了，下就需要把它编译成java代码，这里借助到google为提供的脚本工具protoc。要注意protoc的版本需要和Protobuf的版本对应上，不然不同的版本之间会有一些差异解析可能会有问题。<br />protoc工具的下载地址：[https://github.com/protocolbuffers/protobuf/releases](https://github.com/protocolbuffers/protobuf/releases)<br />[protoc-3.17.3-win64.zip](https://www.yuque.com/attachments/yuque/0/2021/zip/396745/1623771763072-10d77235-1937-4276-acf1-91f1fda4052c.zip?_lake_card=%7B%22src%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2021%2Fzip%2F396745%2F1623771763072-10d77235-1937-4276-acf1-91f1fda4052c.zip%22%2C%22name%22%3A%22protoc-3.17.3-win64.zip%22%2C%22size%22%3A1504404%2C%22type%22%3A%22application%2Fx-zip-compressed%22%2C%22ext%22%3A%22zip%22%2C%22status%22%3A%22done%22%2C%22taskId%22%3A%22ue850e814-45d8-434c-b58d-568d764431a%22%2C%22taskType%22%3A%22upload%22%2C%22id%22%3A%22u8a3f071b%22%2C%22card%22%3A%22file%22%7D)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1623771708792-dab58d00-44aa-4741-a725-8e5f4c5ffaec.png#clientId=ufa8f92f4-389d-4&from=paste&height=519&id=u5dcf8681&originHeight=1558&originWidth=1595&originalType=binary&ratio=3&size=196342&status=done&style=none&taskId=uddc0abdd-f91b-464e-ae53-d2903616f9a&width=531.6666666666666)<br />接着进cmd输入如下命令：
```java
D:\LinkSpace\Download\DevelopPackage\JDK\protoc-3.17.3-win64\bin\protoc.exe .\Msg.proto --java_out=./
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1623772464086-d7ae5110-9198-458c-a5ee-55ae0324812a.png#clientId=ufa8f92f4-389d-4&from=paste&height=296&id=u4dabef5b&originHeight=887&originWidth=3323&originalType=binary&ratio=3&size=1068798&status=done&style=none&taskId=u7414e527-2450-45ce-ad72-ec642858bc9&width=1107.6666666666667)<br />主要是第三句命令。如果输入没有报错的话proto文件夹应该会生成一个子文件夹：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1623772487271-478efaae-07b5-42bb-b7c9-c092b5070a82.png#clientId=ufa8f92f4-389d-4&from=paste&height=503&id=ua7109b8c&originHeight=1508&originWidth=2132&originalType=binary&ratio=3&size=319118&status=done&style=none&taskId=uf91df096-8dd1-41cd-9070-19f60dfea8d&width=710.6666666666666)<br />进去该文件夹会看到已经生成了MessageProto.java文件，这时候已经完成了protobuf序列化文件的生成。然后把该文件拷贝至工程目录下。接下来用生成的文件去发消息吧。还是服务端和客户端。<br />服务端：
```java
public class ProtoBufServer {
    private int port;

    public ProtoBufServer(int port) {
        this.port = port;
    }

    public void start(){
        EventLoopGroup bossGroup = new NioEventLoopGroup();
        EventLoopGroup workGroup = new NioEventLoopGroup();

        ServerBootstrap server = new ServerBootstrap().group(bossGroup,workGroup)
            .channel(NioServerSocketChannel.class)
            .childHandler(new ServerChannelInitializer());

        try {
            ChannelFuture future = server.bind(port).sync();
            future.channel().closeFuture().sync();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }finally {
            bossGroup.shutdownGracefully();
            workGroup.shutdownGracefully();
        }
    }

    public static void main(String[] args) {
        ProtoBufServer server = new ProtoBufServer(7788);
        server.start();
    }
}
```
服务端Initializer：
```java
public class ServerChannelInitializer extends ChannelInitializer<SocketChannel> {
    @Override
    protected void initChannel(SocketChannel socketChannel) throws Exception {
        ChannelPipeline pipeline = socketChannel.pipeline();

        pipeline.addLast(new ProtobufVarint32FrameDecoder());
        pipeline.addLast(new ProtobufDecoder(MessageProto.RequestMsg.getDefaultInstance()));
        pipeline.addLast(new ProtoBufServerHandler());
    }
}
```
服务端handler：
```java
public class ProtoBufServerHandler extends ChannelInboundHandlerAdapter {
    @Override
    public void channelActive(ChannelHandlerContext ctx) throws Exception {
        MessageProto.ResponseMsg.Builder builder = MessageProto.ResponseMsg.newBuilder();
        builder.setMsgType(ByteString.copyFromUtf8("CBSP"));
        builder.setReceiveOne("小红");
        builder.setMsg("你好，你有啥事");

        ctx.writeAndFlush(builder.build());
    }


    @Override
    public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
        MessageProto.RequestMsg m = (MessageProto.RequestMsg)msg;
        System.out.println("Client say: "+m.getReceiveOne()+","+m.getMsg());
    }

    @Override
    public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) throws Exception {
        super.exceptionCaught(ctx, cause);
        ctx.close();
    }
}
```
客户端：
```java
public class ProtoBufClient {
    private  int port;
    private  String address;

    public ProtoBufClient(int port, String address) {
        this.port = port;
        this.address = address;
    }

    public void start(){
        EventLoopGroup group = new NioEventLoopGroup();

        Bootstrap bootstrap = new Bootstrap();
        bootstrap.group(group)
            .channel(NioSocketChannel.class)
            .handler(new ClientChannelInitializer());

        try {

            ChannelFuture future = bootstrap.connect(address,port).sync();
            future.channel().closeFuture().sync();
        } catch (Exception e) {
            e.printStackTrace();
        }finally {
            group.shutdownGracefully();
        }

    }

    public static void main(String[] args) {
        ProtoBufClient client = new ProtoBufClient(7788,"127.0.0.1");
        client.start();
    }
}
```
客户端Initializer：
```java
public class ClientChannelInitializer extends  ChannelInitializer<SocketChannel> {

    protected void initChannel(SocketChannel socketChannel) throws Exception {
        ChannelPipeline pipeline = socketChannel.pipeline();

        pipeline.addLast(new ProtobufVarint32LengthFieldPrepender());
        pipeline.addLast(new ProtobufEncoder());
        pipeline.addLast(new ProtoBufClientHandler());
    }
}
```
客户端handler：
```java
public class ProtoBufClientHandler extends ChannelInboundHandlerAdapter {
    @Override
    public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
        MessageProto.ResponseMsg m = (MessageProto.ResponseMsg)msg;
        System.out.println("Server say: "+m.getReceiveOne()+","+m.getMsg());
    }

    @Override
    public void channelActive(ChannelHandlerContext ctx) throws Exception {
        MessageProto.RequestMsg.Builder builder = MessageProto.RequestMsg.newBuilder();
        builder.setMsgType(ByteString.copyFromUtf8("CBSP"));
        builder.setReceiveOne("小明");
        builder.setMsg("你好，我找你有事");

        ctx.writeAndFlush(builder.build());
    }

    @Override
    public void channelInactive(ChannelHandlerContext ctx) throws Exception {
        System.out.println("Client is close");
    }
}
```
启动服务端和客户端，输出如下：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623771228658-35674399-bcf7-40eb-b823-d1947ee9e04e.png#clientId=ufa8f92f4-389d-4&from=paste&id=u69bf9816&originHeight=172&originWidth=356&originalType=url&ratio=3&status=done&style=none&taskId=u273800f5-c2a0-4df9-8f17-f9be856a5ad)
<a name="l3Ad2"></a>
### Thrift

- 支持多种语言（C++、C#、Cocoa、Erlag、Haskell、java、Ocami、Perl、PHP、Python、Ruby和SmallTalk）
- 使用了组建大型数据交换及存储工具，对于大型系统中的内部数据传输，相对于Json和xml在性能上和传输大小上都有明显的优势。
- 支持通用二进制编码，压缩二进制编码，优化的可选字段压缩编解码等三种方式。
- FaceBook开源
<a name="dqAMf"></a>
### Jackson

- Jackson所依赖的jar包较少，简单易用并且性能也要相对高些。
- 对于复杂类型的json转换bean会出现问题，一些集合Map，List的转换出现问题。
- Jackson对于复杂类型的bean转换Json，转换的json格式不是标准的Json格式
<a name="HxBJh"></a>
### Gson

- Gson是目前功能最全的Json解析神器
- Gson的应用主要为toJson与fromJson两个转换函数，无依赖，不需要例外额外的jar
- 类里面只要有get和set方法，Gson完全可以将复杂类型的json到bean或bean到json的转换，是JSON解析的神器
<a name="Spkyx"></a>
### FastJson

- 无依赖，不需要例外额外的jar，能够直接跑在JDK上。
- FastJson在复杂类型的Bean转换Json上会出现一些问题，可能会出现引用的类型，导致Json转换出错，需要制定引用。
- FastJson采用独创的算法，将parse的速度提升到极致，超过所有json库。
- 频繁爆漏洞，被相当部分公司禁止使用
