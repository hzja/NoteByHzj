Java IO NIO AIO
<a name="PdO6h"></a>
## 概述
在学习Java的IO流之前，要了解几个关键词

- 同步与异步：同步是一种可靠的有序运行机制，当进行同步操作时，后续的任务是等待当前调用返回，才会进行下一步；而异步则相反，其他任务不需要等待当前调用返回，通常依靠事件、回调等机制来实现任务间次序关系
- 阻塞与非阻塞：在进行阻塞操作时，当前线程会处于阻塞状态，无法从事其他任务，只有当条件就绪才能继续，比如`ServerSocket`新连接建立完毕，或者数据读取、写入操作完成；而非阻塞则是不管IO操作是否结束，直接返回，相应操作在后台继续处理

同步和异步的概念：实际的I/O操作<br />同步是用户线程发起I/O请求后需要等待或者轮询内核I/O操作完成后才能继续执行<br />异步是用户线程发起I/O请求后仍需要继续执行，当内核I/O操作完成后会通知用户线程，或者调用用户线程注册的回调函数<br />阻塞和非阻塞的概念：发起I/O请求<br />阻塞是指I/O操作需要彻底完成后才能返回用户空间<br />非阻塞是指I/O操作被调用后立即返回一个状态值，无需等I/O操作彻底完成
<a name="xSiGR"></a>
## BIO、NIO、AIO的概述
首先，传统的 java.io包，它基于流模型实现，提供了最熟知的一些 IO 功能，比如 File 抽象、输入输出流等。交互方式是同步、阻塞的方式，也就是说，在读取输入流或者写入输出流时，在读、写动作完成之前，线程会一直阻塞在那里，它们之间的调用是可靠的线性顺序。<br />java.io包的好处是代码比较简单、直观，缺点则是 IO 效率和扩展性存在局限性，容易成为应用性能的瓶颈。<br />很多时候，人们也把 java.net下面提供的部分网络 API，比如 `Socket`、`ServerSocket`、`HttpURLConnection` 也归类到同步阻塞 IO 类库，因为网络通信同样是 IO 行为。<br />第二，在 Java 1.4 中引入了 NIO 框架（java.nio 包），提供了 Channel、Selector、Buffer 等新的抽象，可以构建多路复用的、同步非阻塞 IO 程序，同时提供了更接近操作系统底层的高性能数据操作方式。<br />第三，在 Java 7 中，NIO 有了进一步的改进，也就是 NIO 2，引入了异步非阻塞 IO 方式，也有很多人叫它 AIO（Asynchronous IO）。异步 IO 操作基于事件和回调机制，可以简单理解为，应用操作直接返回，而不会阻塞在那里，当后台处理完成，操作系统会通知相应线程进行后续工作。
<a name="FwguE"></a>
## 一、IO流（同步、阻塞）
<a name="s4kGY"></a>
### 1、概述
IO流简单来说就是input和output流，IO流主要是用来处理设备之间的数据传输，Java IO对于数据的操作都是通过流实现的，而java用于操作流的对象都在IO包中。
<a name="K8LGm"></a>
### 2、分类
按操作数据分为：字节流（`InputStream`、`OutputStream`）和字符流（`Reader`、`Writer`）<br />按流向分：输入流（`Reader`、`InputStream`）和输出流（`Writer`、`OutputStream`）<br />![](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1634894309861-dda3f816-2786-4b95-9e5b-075d5f6de8ba.jpeg)
<a name="wrX09"></a>
### 3、字符流
<a name="ynYpt"></a>
#### 概述
只用来处理文本数据<br />数据最常见的表现形式是文件，字符流用来操作文件的子类一般是`FileReader`和`FileWriter`<br />字符流读写文件注意事项：

- 写入文件必须要用`flush()`刷新
- 用完流记得要关闭流
- 使用流对象要抛出IO异常
- 定义文件路径时，可以用"/"或者"\"
- 在创建一个文件时，如果目录下有同名文件将被覆盖
- 在读取文件时，必须保证该文件已存在，否则抛出异常
<a name="CX4cw"></a>
#### 字符流的缓冲区

- 缓冲区的出现是为了提高流的操作效率而出现的
- 需要被提高效率的流作为参数传递给缓冲区的构造函数
- 在缓冲区中封装了一个数组，存入数据后一次取出
<a name="mAMKT"></a>
### 4、字节流
<a name="oHWwf"></a>
#### 概述
用来处理媒体数据<br />字节流读写文件注意事项：

- 字节流和字符流的基本操作是相同的，但是想要操作媒体流就需要用到字节流
- 字节流因为操作的是字节，所以可以用来操作媒体文件（媒体文件也是以字节存储的）
- 输入流（`InputStream`）、输出流（`OutputStream`）
- 字节流操作可以不用刷新流操作
- `InputStream`特有方法：`int available()`（返回文件中的字节个数）

字节流的缓冲区<br />字节流缓冲区跟字符流缓冲区一样，也是为了提高效率
<a name="Xizec"></a>
### 5、Java Scanner类
Java 5添加了java.util.Scanner类，这是一个用于扫描输入文本的新的实用程序
<a name="PNti2"></a>
#### 关于`nextInt()`、`next()`、`nextLine()`的理解
`nextInt()`：只能读取数值，若是格式不对，会抛出java.util.InputMismatchException异常<br />`next()`：遇见第一个有效字符（非空格，非换行符）时，开始扫描，当遇见第一个分隔符或结束符（空格或换行符）时，结束扫描，获取扫描到的内容<br />`nextLine()`：可以扫描到一行内容并作为字符串而被捕获到
<a name="pXaRo"></a>
#### 关于`hasNext()`、`hasNextLine()`、`hasNextxxx()`的理解
就是为了判断输入行中是否还存在xxx的意思
<a name="HxSLR"></a>
#### 与`delimiter()`有关的方法
应该是输入内容的分隔符设置，
<a name="lt04H"></a>
## 二、NIO（同步、非阻塞）
NIO之所以是同步，是因为它的accept/read/write方法的内核I/O操作都会阻塞当前线程<br />首先，要先了解一下NIO的三个主要组成部分：Channel（通道）、Buffer（缓冲区）、Selector（选择器）
<a name="quA4H"></a>
### （1）Channel（通道）
Channel（通道）：Channel是一个对象，可以通过它读取和写入数据。可以把它看做是IO中的流，不同的是：

- Channel是双向的，既可以读又可以写，而流是单向的
- Channel可以进行异步的读写
- 对Channel的读写必须通过buffer对象

正如上面提到的，所有数据都通过Buffer对象处理，所以，永远不会将字节直接写入到Channel中，相反，是将数据写入到Buffer中；同样，也不会从Channel中读取字节，而是将数据从Channel读入Buffer，再从Buffer获取这个字节。<br />因为Channel是双向的，所以Channel可以比流更好地反映出底层操作系统的真实情况。特别是在Unix模型中，底层操作系统通常都是双向的。<br />在Java NIO中的Channel主要有如下几种类型：

- `FileChannel`：从文件读取数据的
- `DatagramChannel`：读写UDP网络协议数据
- `SocketChannel`：读写TCP网络协议数据
- `ServerSocketChannel`：可以监听TCP连接
<a name="BVLKd"></a>
### （2）Buffer
Buffer是一个对象，它包含一些要写入或者读到Stream对象的。应用程序不能直接对 Channel 进行读写操作，而必须通过 Buffer 来进行，即 Channel 是通过 Buffer 来读写数据的。<br />在NIO中，所有的数据都是用Buffer处理的，它是NIO读写数据的中转池。Buffer实质上是一个数组，通常是一个字节数据，但也可以是其他类型的数组。但一个缓冲区不仅仅是一个数组，重要的是它提供了对数据的结构化访问，而且还可以跟踪系统的读写进程。<br />使用 Buffer 读写数据一般遵循以下四个步骤：

1. 写入数据到 Buffer；
2. 调用 `flip()` 方法；
3. 从 Buffer 中读取数据；
4. 调用 `clear()` 方法或者 `compact()` 方法。

当向 Buffer 写入数据时，Buffer 会记录下写了多少数据。一旦要读取数据，需要通过 `flip()` 方法将 Buffer 从写模式切换到读模式。在读模式下，可以读取之前写入到 Buffer 的所有数据。<br />一旦读完了所有的数据，就需要清空缓冲区，让它可以再次被写入。有两种方式能清空缓冲区：调用 `clear()` 或 `compact() `方法。`clear()` 方法会清空整个缓冲区。`compact()` 方法只会清除已经读过的数据。任何未读的数据都被移到缓冲区的起始处，新写入的数据将放到缓冲区未读数据的后面。<br />Buffer主要有如下几种：

- `ByteBuffer`
- `CharBuffer`
- `DoubleBuffer`
- `FloatBuffer`
- `IntBuffer`
- `LongBuffer`
- `ShortBuffer`
<a name="jSwNN"></a>
#### copyFile实例（NIO）
CopyFile是一个非常好的读写结合的例子，通过CopyFile这个实力让大家体会NIO的操作过程。CopyFile执行三个基本的操作：创建一个Buffer，然后从源文件读取数据到缓冲区，然后再将缓冲区写入目标文件。
```java
public static void copyFileUseNIO(String src,String dst) throws IOException{
    //声明源文件和目标文件
    FileInputStream fi=new FileInputStream(new File(src));
    FileOutputStream fo=new FileOutputStream(new File(dst));
    //获得传输通道channel
    FileChannel inChannel=fi.getChannel();
    FileChannel outChannel=fo.getChannel();
    //获得容器buffer
    ByteBuffer buffer=ByteBuffer.allocate(1024);
    while(true){
        //判断是否读完文件
        int eof =inChannel.read(buffer);
        if(eof==-1){
            break;  
        }
        //重设一下buffer的position=0，limit=position
        buffer.flip();
        //开始写
        outChannel.write(buffer);
        //写完要重置buffer，重设position=0,limit=capacity
        buffer.clear();
    }
    inChannel.close();
    outChannel.close();
    fi.close();
    fo.close();
}  
```
<a name="XxDpt"></a>
### （3）Selector（选择器对象）
首先需要了解一件事情就是线程上下文切换开销会在高并发时变得很明显，这是同步阻塞方式的低扩展性劣势。<br />Selector是一个对象，它可以注册到很多个Channel上，监听各个Channel上发生的事件，并且能够根据事件情况决定Channel读写。这样，通过一个线程管理多个Channel，就可以处理大量网络连接了。
<a name="wB6la"></a>
#### selector优点
有了Selector，就可以利用一个线程来处理所有的channels。线程之间的切换对操作系统来说代价是很高的，并且每个线程也会占用一定的系统资源。所以，对系统来说使用的线程越少越好。
<a name="Lfstk"></a>
#### 1、如何创建一个Selector
Selector 就是注册对各种 I/O 事件兴趣的地方，而且当那些事件发生时，就是这个对象告诉你所发生的事件。
```java
Selector selector = Selector.open();
```
<a name="RO0ZR"></a>
#### 2、注册Channel到Selector
为了能让Channel和Selector配合使用，需要把Channel注册到Selector上。通过调用 `channel.register()`方法来实现注册：
```java
channel.configureBlocking(false);
SelectionKey key =channel.register(selector,SelectionKey.OP_READ);
```
注意，注册的Channel 必须设置成异步模式 才可以,否则异步IO就无法工作，这就意味着不能把一个FileChannel注册到Selector，因为`FileChannel`没有异步模式，但是网络编程中的`SocketChannel`是可以的。
<a name="NpxHK"></a>
#### 3、关于SelectionKey
请注意对`register()`的调用的返回值是一个SelectionKey。SelectionKey 代表这个通道在此 Selector 上注册。当某个 Selector 通知某个传入事件时，它是通过提供对应于该事件的 SelectionKey 来进行的。SelectionKey 还可以用于取消通道的注册。<br />SelectionKey中包含如下属性：

- The interest set
- The ready set
- The Channel
- The Selector
- An attached object (optional)
<a name="LxAgi"></a>
#### （1）Interest set
就像在前面讲到的把Channel注册到Selector来监听感兴趣的事件，interest set就是要选择的感兴趣的事件的集合。可以通过`SelectionKey`对象来读写interest set:
```java
int interestSet = selectionKey.interestOps();
boolean isInterestedInAccept  = interestSet & SelectionKey.OP_ACCEPT;
boolean isInterestedInConnect = interestSet & SelectionKey.OP_CONNECT;
boolean isInterestedInRead    = interestSet & SelectionKey.OP_READ;
boolean isInterestedInWrite   = interestSet & SelectionKey.OP_WRITE; 
```
通过上面例子可以看到，可以通过用AND 和SelectionKey 中的常量做运算，从SelectionKey中找到感兴趣的事件。
<a name="ziUsC"></a>
#### （2）Ready Set
ready set 是通道已经准备就绪的操作的集合。在一次选Selection之后，应该会首先访问这个ready set。Selection将在下一小节进行解释。可以这样访问ready集合：
```java
int readySet = selectionKey.readyOps();
```
可以用像检测interest集合那样的方法，来检测Channel中什么事件或操作已经就绪。但是，也可以使用以下四个方法，它们都会返回一个布尔类型：
```java
selectionKey.isAcceptable();
selectionKey.isConnectable();
selectionKey.isReadable();
selectionKey.isWritable();
```
<a name="q108q"></a>
#### （3）Channel 和 Selector
可以通过`SelectionKey`获得`Selector`和注册的Channel：
```java
Channel  channel  = selectionKey.channel();
Selector selector = selectionKey.selector(); 
```
<a name="xRD14"></a>
#### （4）Attach一个对象
可以将一个对象或者更多信息attach 到`SelectionKey`上，这样就能方便的识别某个给定的通道。例如，可以附加 与通道一起使用的Buffer，或是包含聚集数据的某个对象。使用方法如下：
```java
selectionKey.attach(theObject);
Object attachedObj = selectionKey.attachment();
```
还可以在用`register()`方法向`Selector`注册Channel的时候附加对象。如：
```java
SelectionKey key = channel.register(selector, SelectionKey.OP_READ, theObject);
```
<a name="wWYiq"></a>
#### 4、关于`SelectedKeys()`
生产系统中一般会额外进行就绪状态检查<br />一旦调用了`select()`方法，它就会返回一个数值，表示一个或多个通道已经就绪，然后就可以通过调用`selector.selectedKeys()`方法返回的`SelectionKey`集合来获得就绪的Channel。请看演示方法：
```java
Set<SelectionKey> selectedKeys = selector.selectedKeys();
```
当通过Selector注册一个`Channel`时，`channel.register()`方法会返回一个`SelectionKey`对象，这个对象就代表了注册的`Channel`。这些对象可以通过`selectedKeys()`方法获得。可以通过迭代这些selected key来获得就绪的`Channel`，下面是演示代码：
```java
Set<SelectionKey> selectedKeys = selector.selectedKeys();
Iterator<SelectionKey> keyIterator = selectedKeys.iterator();
while(keyIterator.hasNext()) { 
    SelectionKey key = keyIterator.next();
    if(key.isAcceptable()) {
        // a connection was accepted by a ServerSocketChannel.
    } else if (key.isConnectable()) {
        // a connection was established with a remote server.
    } else if (key.isReadable()) {
        // a channel is ready for reading
    } else if (key.isWritable()) {
        // a channel is ready for writing
    }
    keyIterator.remove();
}
```
这个循环遍历selected key的集合中的每个key，并对每个key做测试来判断哪个Channel已经就绪。<br />请注意循环中最后的`keyIterator.remove()`方法。Selector对象并不会从自己的selected key集合中自动移除SelectionKey实例。需要在处理完一个Channel的时候自己去移除。当下一次Channel就绪的时候，Selector会再次把它添加到selected key集合中。<br />`SelectionKey.channel()`方法返回的Channel需要转换成具体要处理的类型，比如是`ServerSocketChannel`或者`SocketChannel`等等。
<a name="ekWol"></a>
### （4）NIO多路复用
主要步骤和元素：

- 首先，通过 `Selector.open()` 创建一个 Selector，作为类似调度员的角色。
- 然后，创建一个 `ServerSocketChannel`，并且向 Selector 注册，通过指定 `SelectionKey.OP_ACCEPT`，告诉调度员，它关注的是新的连接请求。
- 注意，为什么要明确配置非阻塞模式呢？这是因为阻塞模式下，注册操作是不允许的，会抛出 IllegalBlockingModeException 异常。
- Selector 阻塞在 select 操作，当有 Channel 发生接入请求，就会被唤醒。
- 在 具体的 方法中，通过 SocketChannel 和 Buffer 进行数据操作

IO 都是同步阻塞模式，所以需要多线程以实现多任务处理。而 NIO 则是利用了单线程轮询事件的机制，通过高效地定位就绪的 Channel，来决定做什么，仅仅 select 阶段是阻塞的，可以有效避免大量客户端连接时，频繁线程切换带来的问题，应用的扩展能力有了非常大的提高
<a name="graIV"></a>
## 三、NIO2(异步、非阻塞)
AIO是异步IO的缩写，虽然NIO在网络操作中，提供了非阻塞的方法，但是NIO的IO行为还是同步的。对于NIO来说，业务线程是在IO操作准备好时，得到通知，接着就由这个线程自行进行IO操作，IO操作本身是同步的。<br />但是对AIO来说，则更加进了一步，它不是在IO准备好时再通知线程，而是在IO操作已经完成后，再给线程发出通知。因此AIO是不会阻塞的，此时业务逻辑将变成一个回调函数，等待IO操作完成后，由系统自动触发。<br />与NIO不同，当进行读写操作时，只须直接调用API的`read`或`write`方法即可。这两种方法均为异步的，对于读操作而言，当有流可读取时，操作系统会将可读的流传入`read`方法的缓冲区，并通知应用程序；对于写操作而言，当操作系统将`write`方法传递的流写入完毕时，操作系统主动通知应用程序。即可以理解为，read/write方法都是异步的，完成后会主动调用回调函数。在JDK1.7中，这部分内容被称作NIO.2，主要在Java.nio.channels包下增加了下面四个异步通道：

- `AsynchronousSocketChannel`
- `AsynchronousServerSocketChannel`
- `AsynchronousFileChannel`
- `AsynchronousDatagramChannel`

在AIO socket编程中，服务端通道是`AsynchronousServerSocketChannel`，这个类提供了一个`open()`静态工厂，一个`bind()`方法用于绑定服务端IP地址（还有端口号），另外还提供了`accept()`用于接收用户连接请求。在客户端使用的通道是`AsynchronousSocketChannel`,这个通道处理提供`open`静态工厂方法外，还提供了`read`和`write`方法。<br />在AIO编程中，发出一个事件（accept read write等）之后要指定事件处理类（回调函数），AIO中的事件处理类是`CompletionHandler`，这个接口定义了如下两个方法，分别在异步操作成功和失败时被回调。
```java
void completed(V result, A attachment);
void failed(Throwable exc, A attachment);
```
