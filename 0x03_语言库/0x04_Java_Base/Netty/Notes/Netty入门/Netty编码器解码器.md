Java Netty<br />在了解Netty编解码之前，先了解Java的编解码：

- 编码（Encode）称为序列化， 它将对象序列化为字节数组，用于网络传输、数据持久化或者其它用途。
- 解码（Decode）称为反序列化，它把从网络、磁盘等读取的字节数组还原成原始对象（通常是原始对象的拷贝），以方便后续的业务逻辑操作。

java序列化对象只需要实现`java.io.Serializable`接口并生成序列化ID，这个类就能够通过`java.io.ObjectInput`和`java.io.ObjectOutput`序列化和反序列化。<br />Java序列化目的：

1. 网络传输。
2. 对象持久化。

Java序列化缺点：

1. 无法跨语言。 
2. 序列化后码流太大。
3. 序列化性能太低。

Java序列化仅仅是Java编解码技术的一种，由于它的种种缺陷，衍生出了多种编解码技术和框架，这些编解码框架实现消息的高效序列化。<br />下面来了解下Netty自己的编解码器。
<a name="tiC7M"></a>
## 一、概念
在网络应用中需要实现某种编解码器，将原始字节数据与自定义的消息对象进行互相转换。网络中都是以字节码的数据形式来传输数据的，服务器编码数据后发送到客户端，客户端需要对数据进行解码。<br />Netty 提供了强大的编解码器框架，使得编写自定义的编解码器很容易，也容易封装重用。对于Netty而言，编解码器由两部分组成：编码器、解码器。 <br />解码器：负责将消息从字节或其他序列形式转成指定的消息对象。<br />编码器：将消息对象转成字节或其他序列形式在网络上传输。<br />Netty 的编（解）码器实现了 `ChannelHandlerAdapter`，也是一种特殊的 `ChannelHandler`，所以依赖于 `ChannelPipeline`，可以将多个编（解）码器链接在一起，以实现复杂的转换逻辑。<br />Netty里面的编解码： 解码器：负责处理“入站 `InboundHandler`”数据。   编码器：负责“出站 `OutboundHandler`” 数据。
<a name="YRhcB"></a>
## 二、解码器(Decoder)
解码器负责 解码“入站”数据从一种格式到另一种格式，解码器处理入站数据是抽象`ChannelInboundHandler`的实现。实践中使用解码器很简单，就是将入站数据转换格式后<br />传递到`ChannelPipeline`中的下一个`ChannelInboundHandler`进行处理；这样的处理时很灵活的，可以将解码器放在`ChannelPipeline`中，重用逻辑。<br />对于解码器，Netty中主要提供了抽象基类`ByteToMessageDecoder`和`MessageToMessageDecoder`<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1643292827492-ef2a305e-92e3-4497-9fe0-ddca644161f2.png#clientId=u13c424bd-8e8f-4&from=paste&id=u80ca334c&originHeight=313&originWidth=458&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ufac074dc-70f2-4fe0-be24-7c2aaa5779b&title=)<br />抽象解码器<br />1)  `ByteToMessageDecoder`：用于将字节转为消息，需要检查缓冲区是否有足够的字节<br />2)  `ReplayingDecoder`：继承`ByteToMessageDecoder`，不需要检查缓冲区是否有足够的字节，但是 `ReplayingDecoder`速度略慢于`ByteToMessageDecoder`，同时不是所有的ByteBuf都支持。<br />选择：项目复杂性高则使用`ReplayingDecoder`，否则使用 `ByteToMessageDecoder`<br />3）`MessageToMessageDecoder`：用于从一种消息解码为另外一种消息（例如POJO到POJO）
<a name="t2ggK"></a>
### 1、`ByteToMessageDecoder`解码器
用于将接收到的二进制数据(Byte)解码，得到完整的请求报文(Message)。<br />`ByteToMessageDecoder`是一种`ChannelInboundHandler`，可以称为解码器，负责将byte字节流(ByteBuf)转换成一种Message，Message是应用可以自己定义的一种<br />Java对象。<br />下面列出了`ByteToMessageDecoder`两个主要方法：
```java
protected abstract void decode(ChannelHandlerContext ctx, ByteBuf in, List<Object> out)//这个方法是唯一的一个需要自己实现的抽象方法，作用是将ByteBuf数据解码成其他形式的数据。
decodeLast(ChannelHandlerContext, ByteBuf, List<Object>)//实际上调用的是decode(...)。
```
参数的作用如下：<br />`Bytubuf`：需要解码的二进制数据。<br />`List<Object>`：解码后的有效报文列表，需要将解码后的报文添加到这个List中。之所以使用一个List表示，是因为考虑到粘包问题，因此入参的in中可能包含多个有效报文。<br />当然，也有可能发生了拆包，in中包含的数据还不足以构成一个有效报文，此时不往List中添加元素即可。<br />另外特别要注意的是，在解码时，不能直接调用ByteBuf的`readXXX`方法来读取数据，而是应该首先要判断能否构成一个有效的报文。<br />案例，假设协议规定传输的数据都是int类型的整数<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1643292827431-3153d6ab-b854-44ae-8e20-4a26ebc5fd43.png#clientId=u13c424bd-8e8f-4&from=paste&id=uec879588&originHeight=316&originWidth=622&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud9d0ccce-684e-4ab0-9e7b-8e631566ae1&title=)<br />上图中显式输入的ByteBuf中包含4个字节，每个字节的值分别为：1，2，3，4。自定义一个`ToIntegerDecoder`进行解码，尽管这里看到了4个字节刚好可以构成一个int类型整数，但是在真正解码之前，并不知道ByteBuf包含的字节数能否构成完成的有效报文，因此需要首先判断ByteBuf中剩余可读的字节，是否大于等于4，如下：
```java
public class ToIntegerDecoder extends ByteToMessageDecoder {
    @Override
    public void decode(ChannelHandlerContext ctx, ByteBuf in, List<Object> out) throws Exception {
        if (in.readableBytes() >= 4) {
            out.add(in.readInt());
        } 
    }
}
```
只有在可读字节数>=4的情况下，才进行解码，即读取一个int，并添加到List中。<br />在可读字节数小于4的情况下，并没有做任何处理，假设剩余可读字节数为3，不足以构成1个int。那么父类`ByteToMessageDecoder`发现这次解码List中的元素没有变化，<br />则会对in中的剩余3个字节进行缓存，等待下1个字节的到来，之后再回到调用`ToIntegerDecoder`的`decode`方法。<br />另外需要注意：在`ToIntegerDecoder`的`decode`方法中，每次最多只读取一个1个int。如果ByteBuf中的字节数很多，例如为16，那么可以构成4个int，而这里只读取了1个int，<br />那么剩余12字节怎么办？这个其实不用担心，`ByteToMessageDecoder`再每次回调子类的`decode`方法之后，都会判断输入的`ByteBuf`中是否还有剩余字节可读，如果还有，会再次<br />回调子类的`decode`方法，直到某个回调`decode`方法`List`中的元素个数没有变化时才停止，元素个数没有变化，实际上意味着子类已经没有办法从剩余的字节中读取一个有效报文。<br />由于存在剩余可读字节时，`ByteToMessageDecoder`会自动再次回调子类`decode`方法，因此建议在实现`ByteToMessageDecoder`时，decode方法每次只解析一个有效报文<br />即可，没有必要一次全部解析出来。<br />`ByteToMessageDecoder`提供的一些常见的实现类：

- `FixedLengthFrameDecoder`：定长协议解码器，可以指定固定的字节数算一个完整的报文
- `LineBasedFrameDecoder`：行分隔符解码器，遇到`\n`或者`\r\n`，则认为是一个完整的报文
- `DelimiterBasedFrameDecoder`：分隔符解码器，与`LineBasedFrameDecoder`类似，只不过分隔符可以自己指定
- `LengthFieldBasedFrameDecoder`：长度编码解码器，将报文划分为报文头`/`报文体，根据报文头中的Length字段确定报文体的长度，因此报文提的长度是可变的
- `JsonObjectDecoder`：json格式解码器，当检测到匹配数量的"{" 、"}"或"[""]"时，则认为是一个完整的json对象或者json数组。

这些实现类，都只是将接收到的二进制数据，解码成包含完整报文信息的ByteBuf实例后，就直接交给了之后的`ChannelInboundHandler`处理。
<a name="ll6wk"></a>
### 2、`ReplayingDecoder `解码器
`ReplayingDecoder`是byte-to-message解码的一种特殊的抽象基类，byte-to-message解码读取缓冲区的数据之前需要检查缓冲区是否有足够的字节，使用`ReplayingDecoder`就无需自己检查；若ByteBuf中有足够的字节，则会正常读取；若没有足够的字节则会停止解码。<br />也正因为这样的包装使得`ReplayingDecoder`带有一定的局限性。<br />1) 不是所有的操作都被ByteBuf支持，如果调用一个不支持的操作会抛出`DecoderException`。<br />2) `ByteBuf.readableBytes()`大部分时间不会返回期望值。<br />如果能忍受上面列出的限制，相比`ByteToMessageDecoder`，可能更喜欢`ReplayingDecoder`。在满足需求的情况下推荐使用`ByteToMessageDecoder`，因为它的处理比较简单，没有`ReplayingDecoder`实现的那么复杂。`ReplayingDecoder`继承于`ByteToMessageDecoder`，所以他们提供的接口是相同的。下面代码是`ReplayingDecoder`的实现：
```java
/**
 * Integer解码器,ReplayingDecoder实现
 */
public class ToIntegerReplayingDecoder extends ReplayingDecoder<Void> {

    @Override
    protected void decode(ChannelHandlerContext ctx, ByteBuf in, List<Object> out) throws Exception {
        out.add(in.readInt());
    }
}
```
<a name="MIYZI"></a>
### 3、`MessageToMessageDecoder`
`ByteToMessageDecoder`是将二进制流进行解码后，得到有效报文。而`MessageToMessageDecoder`则是将一个本身就包含完整报文信息的对象转换成另一个Java对象。<br />举例：前面介绍了`ByteToMessageDecoder`的部分子类解码后，会直接将包含了报文完整信息的ByteBuf实例交由之后的`ChannelInboundHandler`处理，此时，可以在`ChannelPipeline`中，再添加一个`MessageToMessageDecoder`，将`ByteBuf`中的信息解析后封装到Java对象中，简化之后的`ChannelInboundHandler`的操作。<br />另外：一些场景下，有可能报文信息已经封装到了Java对象中，但是还要继续转成另外的Java对象，因此一个`MessageToMessageDecoder`后面可能还跟着另一个`MessageToMessageDecoder`。一个比较容易的理解的类比案例是Java Web编程，通常客户端浏览器发送过来的二进制数据，已经被web容器(如tomcat)解析成了一个<br />`HttpServletRequest`对象，但是还是需要将`HttpServletRequest`中的数据提取出来，封装成自己的POJO类，也就是从一个Java对象(`HttpServletRequest`)转换成另一个Java对象(POJO类)。<br />`MessageToMessageDecoder`的类声明如下：
```java
/**
  * 其中泛型参数I表示要解码的消息类型。例前面，在ToIntegerDecoder中，把二进制字节流转换成了一个int类型的整数。
  */
public abstract class MessageToMessageDecoder<I> extends ChannelInboundHandlerAdapter
```
类似的，`MessageToMessageDecoder`也有一个`decode`方法需要覆盖 ，如下：
```java
/**
* 参数msg，需要进行解码的参数。例如ByteToMessageDecoder解码后的得到的包含完整报文信息ByteBuf
* List<Object> out参数：将msg经过解析后得到的java对象，添加到放到List<Object> out中
*/
protected abstract void decode(ChannelHandlerContext ctx, I msg, List<Object> out) throws Exception;
```
例如，现在编写一个`IntegerToStringDecoder`，把前面编写的`ToIntegerDecoder`输出的int参数转换成字符串，此时泛型`I`就应该是`Integer`类型。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1643292827994-4e40a863-0e82-459c-8376-e77367d240c4.png#clientId=u13c424bd-8e8f-4&from=paste&id=u8bcd4cc4&originHeight=315&originWidth=620&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4c5fdadf-cc8d-4426-9cb2-64d351a4a78&title=)<br />`IntegerToStringDecoder`源码如下所示
```java
public class IntegerToStringDecoder extends MessageToMessageDecoder<Integer> {
    @Override
    public void decode(ChannelHandlerContext ctx, Integer msg List<Object> out) throws Exception {
        out.add(String.valueOf(msg));
    }
}
```
此时应该按照如下顺序组织`ChannelPipieline`中`ToIntegerDecoder`和`IntegerToStringDecoder `的关系：
```java
ChannelPipieline ch=....
    ch.addLast(new ToIntegerDecoder());
    ch.addLast(new IntegerToStringDecoder());
```
也就是说，前一个`ChannelInboudHandler`输出的参数类型，就是后一个`ChannelInboudHandler`的输入类型。<br />特别注意，如果指定`MessageToMessageDecoder`的泛型参数为`ByteBuf`，表示其可以直接针对`ByteBuf`进行解码，那么其是否能替代`ByteToMessageDecoder`呢？<br />答案是不可以的。因为`ByteToMessageDecoder`除了进行解码，还要会对不足以构成一个完整数据的报文拆包数据(拆包)进行缓存。而`MessageToMessageDecoder`则没有这样的逻辑。<br />因此通常的使用建议是，使用一个`ByteToMessageDecoder`进行粘包、拆包处理，得到完整的有效报文的`ByteBuf`实例，然后交由之后的一个或者多个<br />`MessageToMessageDecoder`对`ByteBuf`实例中的数据进行解析，转换成`POJO`类。
<a name="pnLe2"></a>
## 三、编码器(Encoder)
与`ByteToMessageDecoder`和`MessageToMessageDecoder`相对应，Netty提供了对应的编码器实现`MessageToByteEncoder`和`MessageToMessageEncoder`，<br />二者都实现`ChannelOutboundHandler`接口。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1643292828205-b7d41798-67e4-4b7d-be40-ebb9718e4511.png#clientId=u13c424bd-8e8f-4&from=paste&id=ua3798ad7&originHeight=291&originWidth=495&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=udbe7e998-b089-4f70-8a54-bea65de55fd&title=)<br />相对来说，编码器比解码器的实现要更加简单，原因在于解码器除了要按照协议解析数据，还要要处理粘包、拆包问题；而编码器只要将数据转换成协议规定的二进制格式发送即可。
<a name="UiGuZ"></a>
### 1、抽象类`MessageToByteEncoder`
`MessageToByteEncoder`也是一个泛型类，泛型参数`I`表示将需要编码的对象的类型，编码的结果是将信息转换成二进制流放入ByteBuf中。子类通过覆写其抽象方法`encode`<br />来实现编码，如下所示：
```java
public abstract class MessageToByteEncoder<I> extends ChannelOutboundHandlerAdapter {
    ....
        protected abstract void encode(ChannelHandlerContext ctx, I msg, ByteBuf out) throws Exception;
}
```
可以看到，`MessageToByteEncoder`的输出对象out是一个`ByteBuf`实例，应该将泛型参数msg包含的信息写入到这个out对象中。<br />`MessageToByteEncoder`使用案例：
```java
public class IntegerToByteEncoder extends MessageToByteEncoder<Integer> {
    @Override
    protected void encode(ChannelHandlerContext ctx, Integer msg, ByteBuf out) throws Exception {
        out.writeInt(msg);//将Integer转成二进制字节流写入ByteBuf中
    }
}
```
<a name="Y53pd"></a>
### 2、抽象类`MessageToMessageEncoder`
`MessageToMessageEncoder`同样是一个泛型类，泛型参数`I`表示将需要编码的对象的类型，编码的结果是将信息放到一个List中。子类通过覆写其抽象方法encode，来实现编码，如下所示：
```java
public abstract class MessageToMessageEncoder<I> extends ChannelOutboundHandlerAdapter {
   ...
   protected abstract void encode(ChannelHandlerContext ctx, I msg, List<Object> out) throws Exception;
   ...
}
```
与`MessageToByteEncoder`不同的，`MessageToMessageEncoder`编码后的结果放到的out参数类型是一个`List`中。例如，一次发送2个报文，因此msg参数中实际上包含了<br />2个报文，因此应该解码出两个报文对象放到List中。<br />`MessageToMessageEncoder`提供的常见子类包括：

- `LineEncoder`：按行编码，给定一个`CharSequence`(如String)，在其之后添加换行符`\n`或者`\r\n`，并封装到ByteBuf进行输出，与`LineBasedFrameDecoder`相对应。
- `Base64Encoder`：给定一个ByteBuf，得到对其包含的二进制数据进行Base64编码后的新的ByteBuf进行输出，与`Base64Decoder`相对应。
- `LengthFieldPrepender`：给定一个ByteBuf，为其添加报文头Length字段，得到一个新的ByteBuf进行输出。Length字段表示报文长度，与`LengthFieldBasedFrameDecoder`相对应。
- `StringEncoder`：给定一个`CharSequence`(如：`StringBuilder`、`StringBuffer`、`String`等)，将其转换成ByteBuf进行输出，与`StringDecoder`对应。

这些`MessageToMessageEncoder`实现类最终输出的都是ByteBuf，因为最终在网络上传输的都要是二进制数据。
<a name="yfr9q"></a>
## 四、编码解码器Codec
 编码解码器：同时具有编码与解码功能，特点同时实现了`ChannelInboundHandler`和`ChannelOutboundHandler`接口，因此在数据输入和输出时都能进行处理。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1643292828720-d40530e0-ab9b-4af9-a545-39beb32cc01d.png#clientId=u13c424bd-8e8f-4&from=paste&id=ua973eb64&originHeight=379&originWidth=694&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u32930d2c-c27b-480d-8e22-b60b12ea31f&title=)<br />Netty提供提供了一个`ChannelDuplexHandler`适配器类，编码解码器的抽象基类 `ByteToMessageCodec `、`MessageToMessageCodec`都继承与此类，如下：<br />`ByteToMessageCodec`内部维护了一个`ByteToMessageDecoder`和一个`MessageToByteEncoder`实例，可以认为是二者的功集合，泛型参数I是接受的编码类型：
```java
public abstract class ByteToMessageCodec<I> extends ChannelDuplexHandler {
    private final TypeParameterMatcher outboundMsgMatcher;
    private final MessageToByteEncoder<I> encoder;
    private final ByteToMessageDecoder decoder = new ByteToMessageDecoder(){…}
  
    ...
    protected abstract void encode(ChannelHandlerContext ctx, I msg, ByteBuf out) throws Exception;
    protected abstract void decode(ChannelHandlerContext ctx, ByteBuf in, List<Object> out) throws Exception;
    ...
}
```
`MessageToMessageCodec`内部维护了一个`MessageToMessageDecoder`和一个`MessageToMessageEncoder`实例，可以认为是二者的功集合，泛型参数`INBOUND_IN`和`OUTBOUND_IN`分别表示需要解码和编码的数据类型。
```java
public abstract class MessageToMessageCodec<INBOUND_IN, OUTBOUND_IN> extends ChannelDuplexHandler {
   private final MessageToMessageEncoder<Object> encoder= ...
   private final MessageToMessageDecoder<Object> decoder =…
   ...
   protected abstract void encode(ChannelHandlerContext ctx, OUTBOUND_IN msg, List<Object> out) throws Exception;
   protected abstract void decode(ChannelHandlerContext ctx, INBOUND_IN msg, List<Object> out) throws Exception;
}
```
<a name="Vumnx"></a>
### 其他编解码方式
使用编解码器来充当编码器和解码器的组合失去了单独使用编码器或解码器的灵活性，编解码器是要么都有要么都没有。是否有解决这个僵化问题的方式，还可以让编码器和解码器在`ChannelPipeline`中作为一个逻辑单元。幸运的是，Netty提供了一种解决方案，使用`CombinedChannelDuplexHandler`。虽然这个<br />类不是编解码器API的一部分，但是它经常被用来简历一个编解码器。<br />如何使用`CombinedChannelDuplexHandler`来结合解码器和编码器呢？下面从两个简单的例子看了解。
```java
/**
 * 解码器，将byte转成char
 */
public class ByteToCharDecoder extends ByteToMessageDecoder {

    @Override
    protected void decode(ChannelHandlerContext ctx, ByteBuf in, List<Object> out) throws Exception {
        while(in.readableBytes() >= 2){
            out.add(Character.valueOf(in.readChar()));
        }
    }
}
```
```java
/**
 * 编码器，将char转成byte
 */
public class CharToByteEncoder extends MessageToByteEncoder<Character> {
 
    @Override
    protected void encode(ChannelHandlerContext ctx, Character msg, ByteBuf out) throws Exception {
        out.writeChar(msg);
    }
}
```
```java
/**
 * 继承CombinedChannelDuplexHandler，用于绑定解码器和编码器
 */
public class CharCodec extends CombinedChannelDuplexHandler<ByteToCharDecoder, CharToByteEncoder> {
    public CharCodec(){
        super(new ByteToCharDecoder(), new CharToByteEncoder());
    }
}
```
从上面代码可以看出，使用`CombinedChannelDuplexHandler`绑定解码器和编码器很容易实现，比使用`*Codec`更灵活。
