JavaNIO
<a name="PHi5h"></a>
## 1、Java NIO基本介绍

- Java NlO (New lO）也有人称之为java non-blocking IO是脚ava 1.4版本开始引入的一个新的IOAPI，可以替代标准的Java lO API。NIO与原来的IO有同样的作用和目的，但是使用的方式完全不同，NIO支持面向缓冲区的、基于通道的IO操作。NIO将以更加高效的方式进行文件的读写操作。NIO可以理解为非阻塞IO，传统的IO的read和write只能阻塞执行，线程在读写期间不能干其他事情，比如调用`socket.read()`时，如果服务器一直没有数据传输过来，线程就一直阻塞，而NIO中可以配置socket为非阻塞模式
- NIO相关类都被放在java.nio包及子包下，并且对原 Java.io 包中的很多类进行改写。
- NIO有三大核心部分：Channel（通道），Buffer(缓冲区），Selector（选择器）
- Java NIO的非阻塞模式，使一个线程从某通道发送请求或者读取数据，但是它仅能得到目前可用的数据；如果目前没有数据可用时，就什么都不会获取，而不是保持线程阻塞，所以直至数据变的可以读取之前，该线程可以继续做其他的事情。非阻塞写也是如此，一个线程请求写入一些数据到某通道，但不需要等待它完全写入，这个线程同时可以去做别的事情。
- 通俗理解：NIO是可以做到用一个线程来处理多个操作的。假设有1000个请求过来，根据实际情况，可以分配 20或者80个线程来处理。不像之前的阻塞IO那样，非得分配1000个。
<a name="avTh4"></a>
## 2、NIO和BIO的比较

- BlO以流的方式处理数据，而NIO以块的方式处理数据，块I/O的效率比流I/O高很多
- BlO是阻塞的，NIO则是非阴塞的
- BlO基于字节流和字符流进行操作，而NIO基于Channel(通道）和Buffer(缓冲区）进行操作，数据总是从通道读取到缓冲区中，或者从缓冲区写入到通道中。Selector(选择器）用于监听多个通道的事件（比如：连接请求，数据到达等），因此使用单个线程就可以监听多个客户端通道
| **NIO** | **BIO** |
| --- | --- |
| 面向缓存区（Buffer) | 面向流（Stream) |
| 非阻塞（Non Blocking IO) | 阻塞IO(Blocking IO) |
| 选择器（Selector) | <br /> |

<a name="wqXTG"></a>
## 3、NIO三大核心原理示意图
NIO有三大核心部分：Channel(通道)，Buffer(缓存区)，Selector(选择器)
<a name="RJuI9"></a>
### Buffer缓存区
缓冲区本质上是一块可以写入数据，然后可以从中读取数据的内存。这块内存被包装成NIO Buffer对象，并提供了一组方法，用来方便的访问该块内存。相比较直接对数组的操作，Buffer API更加容易操作和管理。
<a name="GxXHa"></a>
### Channel通道
Java NIO的通道类似流，但又有些不同：既可以从通道中读取数据，又可以写数据到通道。但流的(input或 output)读写通常是单向的。通道可以非阻塞读取和写入通道，通道可以支持读取或写入缓冲区，也支持异步地读写。
<a name="IHPq6"></a>
### Selector选择器
Selector是一个ava NIO组件，可以能够检查一个或多个NIO通道，并确定哪些通道已经准备好进行读取或写入。这样，一个单独的线程可以管理多个channel，从而管理多个网络连接，提高效率<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1697901560998-3447adf6-cea7-469e-86b0-e087bdf8a461.jpeg)

- 每个channel都会对应一个Buffer
- 一个线程对应Selector，一个Selector对应多个channel（连接）
- 程序切换到哪个channel是由事件决定的
- Selector会根据不同的事件，在各个通道上切换
- Buffer就是一个内存块，底层是一个数组
- 数据的读取写入是通过Buffer完成的，BIO中要么是输入流，或者是输出流，不能双向，但是NIO的Buffer是可以读也可以写。
- Java NIO系统的核心在于：通道（Channel）和缓存区（Buffer)。通道表示打开到IO设备（例如：文件、套接字）的连接。若需要使用NlO系统，需要获取用于连接IO设备的通道以及用于容纳数据的缓冲区。然后操作缓冲区，对数据进行处理。简而言之，Channel负责传输，Buffer负责存取数据
<a name="jvpKo"></a>
## 4、NIO核心一：缓存区（Buffer)
<a name="DXnf9"></a>
### 缓存区（Buffer）
一个用于特定基本数据类型的容器。由Java.nio包定义的，所有缓冲区都是Buffer抽象类的子类．Java NIO中的Buffer主要用于与NIO通道进行交互，数据是从通道读入缓冲区，从缓冲区写入通道中的<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1697901694171-85089ae4-a143-4184-992f-3274749c690a.jpeg)
<a name="bfJdR"></a>
### Buffer类及其子类
Buffer就像一个数组，可以保存多个相同类型的数据。根据数据类型不同，有以下Buffer常用子类：

- `ByteBuffer`
- `CharBuffer`
- `ShortBuffer`
- `IntBuffer`
- `LongBuffer`
- `FloatBuffer`
- `DoubleBuffer`

上述Buffer类他们都采用相似的方法进行管理数据，只是各自管理的数据类型不同而已。都是通过如下方法获取一个Buffer对象：
```java
static XxxBuffer allocate(int capacity):创建一个容量为capacity的XxxBuffer对象
```
<a name="pVdaX"></a>
### 缓存区的基本属性
Buffer中的重要概念：

- 容量（capacity）：作为一个内存块，Buffer具有一定的固定大小，也称为“容量”，缓冲区容量不能为负，并且创建后不能更改。
- 限制（limit）：表示缓冲区中可以操作数据的大小（limit后数据不能进行读写）。缓冲区的限制不能为负，并且不能大于其容量。写入模式，限制等于buffer的容量。读取模式下，limit等于写入的数据量。
- 位置（position）：下一个要读取或写入的数据的索引。缓冲区的位置不能为负，并且不能大于其限制
- 标记（mark）与重置（reset）：标记是一个索弓l，通过Buffer中的`mark()`方法指定Buffer中一个特定的 position，之后可以通过调用`reset()`方法恢复到这个position。标记、位置、限制、容量遵守以T不变式：0<=mark<=position<=limit<=capacity
- 图示

![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1697902393493-365027d4-3116-42a3-802c-fed0ecf43e86.jpeg)
<a name="v7A75"></a>
### Buffer常见方法
```
Buffer clear() 清空缓冲区并返回对缓冲区的引用
Buffer flip() 为将缓冲区的界限设置为当前位置，并将当前位置重置为0
int capacity() 返回Buffer的capacity大小
boolean hasRemaining() 判断缓冲区中是否还有元素
int limit() 返回Buffer的界限（limit）的位置
Buffer limit(int n) 将设置缓冲区界限为n，并返回一个具有新limit的缓冲区对象
Buffer mark() 对缓冲区设置标记
int position() 返回缓冲区的当前位置position
Buffer position(int n) 将设置缓冲区的当前位置为n，并返回修改后的Buffer对象
int remaining()返回position和limit之间的元素个数
Buffer reset() 将位置position转到以前设置的mark所在的位置
Buffer rewind();将位置设为为0．取消设置的mark
```
<a name="y8UTR"></a>
### 缓存区的数据操作
```
Buffer 所有子类提供了两个用于数据操作的方法：get() put()方法

获取Buffer中的数据
get()：读取单个字节
get(byte［〕dst)：批量读取多个字节到dst中
get(int index)：读取指定索引位置的字节（不会移动position)

放入数据到Buffer中
put(byte b):将给定单个字节写入缓冲区的当前位置
put(byte[] src):将src中的字节写入缓存区的当前位置
put(int index,byte b)L将指定字节写入缓存区的索引位置（不会移动position）
```
<a name="D1YrL"></a>
### 使用Buffer读写数据一般遵循以下四个步骤

1. 写入数据到Buffer
2. 调用`flip()`方法，转换为读取模式
3. 从Buffer中读取数据
4. 调用`buffer.clear()`方法或则`buffer.compact()`方法清除缓存区
<a name="i2Rzr"></a>
### 案例演示
```java
/**
 * 目标：对缓存区Buffer的常用API进行案例实现
 */
public class BufferTest {
    @Test
    public void test01() {
        //1.分配一个缓存区，容量设置为10
        ByteBuffer buffer = ByteBuffer.allocate(10);
        System.out.println(buffer.position());//0
        System.out.println(buffer.limit());//10
        System.out.println(buffer.capacity());//10
        System.out.println("-----------------------");

        //put 往缓存区中添加数据
        String name = "itheima";
        buffer.put(name.getBytes());
        System.out.println(buffer.position());//7
        System.out.println(buffer.limit());//10
        System.out.println(buffer.capacity());//10
        System.out.println("-----------------------");

        //3.flip() 为 将缓存区的界限设置为当前位置，并将当前位置设置为 0 可读模式
        buffer.flip();
        System.out.println(buffer.position());//0
        System.out.println(buffer.limit());//7
        System.out.println(buffer.capacity());//10
        System.out.println("-----------------------");

        //4. get数据的读取
        char ch = (char) buffer.get();
        System.out.println(ch);//i
        System.out.println(buffer.position());//1
        System.out.println(buffer.limit());//7
        System.out.println(buffer.capacity());//10
    }

    @Test
    public void test02() {
        //1.分配一个缓存区，容量设置为10
        ByteBuffer buffer = ByteBuffer.allocate(10);
        System.out.println(buffer.position());//0
        System.out.println(buffer.limit());//10
        System.out.println(buffer.capacity());//10
        System.out.println("-----------------------");

        String name = "itheima";
        buffer.put(name.getBytes());
        System.out.println(buffer.position());//7
        System.out.println(buffer.limit());//10
        System.out.println(buffer.capacity());//10
        System.out.println("-----------------------");

        //2.clear 清楚缓存区中的数据
        buffer.clear();
        System.out.println(buffer.position());//0
        System.out.println(buffer.limit());//10
        System.out.println(buffer.capacity());//10
        System.out.println((char) buffer.get());//i 表明数据并没有清除 只是恢复了position的位置
        System.out.println("-----------------------");

        //3.定义一个缓存区
        ByteBuffer buf = ByteBuffer.allocate(10);
        String n = "itheima";
        buf.put(n.getBytes());
        buf.flip();
        //读取数据
        byte[] b = new byte[2];
        buf.get(b);
        String rs = new String(b);
        System.out.println(rs);//it
        System.out.println(buf.position());//2
        System.out.println(buf.limit());//7
        System.out.println(buf.capacity());//10
        System.out.println("-----------------------");

        buf.mark();//标记此刻这个位置：2
        byte[] b2 = new byte[3];
        buf.get(b2);
        System.out.println(new String(b2));//hei
        System.out.println(buf.position());//5
        System.out.println(buf.limit());//7
        System.out.println(buf.capacity());//10
        System.out.println("-----------------------");

        buf.reset();//回到标记位置 2
        if (buf.hasRemaining()) {
            System.out.println(buf.remaining());//5
        }
    }
}
```
<a name="oTRPc"></a>
### 直接与非直接缓存区
什么是直接内存与非直接内存<br />根据官方文档的描述：<br />byte buffer。可以是两种类型，一种是基于直接内存（也就是非堆内存）；另一种是非直接内存（也就是堆内存）。对于直接内存来说，JVM将会在IO操作上具有更高的性能，因为它直接作用于本地系统的IO操作。而非直接内存，也就是堆内存中的数据，如果要作IO操作，会先从本进程内存复制到直接内存，再利用本地IO处理。<br />从数据流的角度，非直接内存是下面这样的作用链：
```
本地IO--->直接内存--->非直接内存--->直接内存--->本地IO
```
而直接内存是：
```
本地IO--->直接内存--->本地IO
```
很明显，在做IO处理时，比如网络发送大量数据时，直接内存会具有更高的效率。直接内存使用allocateDirect创建，但是它比申请普通的堆内存需要耗费更高的性能。不过，这部分的数据是在JVM之外的，因此它不会占用应用的内存。所以呢，当你有很大的数据要缓存，并且它的生命周期又很长，那么就比较适合使用直接内存。只是一般来说，如果不是能带来很明显的性能提升，还是推荐直接使用堆内存。字节缓冲区是直接缓冲区还是非直接缓冲区可通过调用其`isDirect()`方法来确定。
```java
@Test
public void test03(){
    //创建一个非直接内存的缓存区
    ByteBuffer buffer = ByteBuffer.allocate(1024);
    //buffer.isDirect()用于判断是否为直接内存
    System.out.println(buffer.isDirect());
    System.out.println("----------------");
    //创建一个直接内存的缓存区
    ByteBuffer buffer2 = ByteBuffer.allocateDirect(1024);
    System.out.println(buffer2.isDirect());
}
```
输出：
```
false
----------------
true
```
**使用场景**

1. 有很大的数据需要存储，他的生命周期又很长
2. 适合频繁的IO操作，比如网络并发场景
<a name="oFe35"></a>
## 5、NIO核心二：通道（Channel）
<a name="WuMyl"></a>
### Channel概述
通道（Channel)：由java.nio.channels包定义的。Channel表示IO源与目标打开的连接。Channel类似于传统的“流”。只不过Channel本身不能直接访问数据，Channel只能与Buffer进行交互。<br />1、NIO的通道类似于流，但有些区别如下

- 通道可以同时进行读写，而流只能读或者只能写
- 通道可以实现异步读写数据
- 通道可以从缓冲读数据，也可以写数据到缓冲

2、BlO中的stream是单向的，例如`FilelnputStream`对象只能进行读取数据的操作，而NIO中的通道（Channel）是双向的，可以读操作，也可以写操作。<br />3、Channel在NIO中是一个接口
```java
public interface Channel extends Closeable()
```
<a name="KKk1m"></a>
### 常用的`Channel`实现类

- `FileChannel`：用于读取、写入、映射和操作文件的通道
- `DatagramChannel`：通过UDP读写网络中的数据通道
- `SocketChannel`：通过TCP读写网络中额数据
- `ServerSocketChannel`：可以监听新进来的TCP连接，对每一个新进来的连接都会创建一个`SocketChannel`。【`ServerSocketChannel`类似`ServerSocket`，`SocketChannel`类似Socket】
<a name="q8mPZ"></a>
### `FileChannel`类
获取通道的一种方式是对支持通道的对象调用`getChannel()`方法。支持通道的类如下：

- `FileInputStream`
- `FileOutputStream`
- `RandomAccessFile`
- `DatagramSocket`
- `Socket`
- `ServerSocket`

获取通道的其他方式是使用Files类的静态方法`newByteChannel()`获取字节通道。或通过通道的静态方法<br />`open()`打开并返回指定通道
<a name="LrINA"></a>
### `FileChannel`的常用方法
```java
int read(ByteBuffer dst) // 从Channel当中读取数据至ByteBuffer
long read(ByteBuffer[] dsts) // 将channel当中的数据“分散”至ByteBuffer[]
int write(ByteBuffer src) // 将ByteBuffer当中的数据写入到Channel
long write(ByteBuffer[] srcs) // 将Bytesuffer[]当中的数据“聚集”到Channel
long position() // 返回此通道的文件位置
FileChannel position(long p) // 设置此通道的文件位置
long size() // 返回此通道的文件的当前大小
FileChannel truncate(long s) // 将此通道的文件截取为给定大小
void force(boolean metaData) // 强制将所有对此通道的文件更新写入到存储设备中
```
<a name="QXZLg"></a>
### 案例1-本地文件写数据
```java
public class ChanellTest {
    @Test
    public void write(){
        try{
            //1.字节输出流通向目标文件
            FileOutputStream fos = new FileOutputStream("data01_txt");
            //2.得到字节输出流对应的通道Channel
            FileChannel channel = fos.getChannel();
            //3.分配缓存区
            ByteBuffer buffer = ByteBuffer.allocate(1024);
            buffer.put("hello，java！".getBytes());
            //4.把缓存区切换为写模式
            buffer.flip();
            channel.write(buffer);
            channel.close();
            System.out.println("写数据到文件中！");
        }catch (Exception e){
            e.printStackTrace();
        }
    }
}
```
<a name="jcH9s"></a>
### 案例2-本地文件读数据
```java
@Test
public void read() throws Exception {
    //1.定义一个文件字节输入流与源文件接通
    FileInputStream is = new FileInputStream("data01_txt");
    //2.需要得到文件字节输入流的文件通道
    FileChannel channel = is.getChannel();
    //3.定义一个缓存区
    ByteBuffer buffer = ByteBuffer.allocate(1024);
    //4.读取数据到缓存区
    channel.read(buffer);
    buffer.flip();//归位
    //5.读取出缓存区中的数据并输出即可
    String rs = new String(buffer.array(), 0, buffer.remaining());
    System.out.println(rs);
}
```
输出：
```
hello，java！
```
<a name="XoN06"></a>
### 案例3-使用Buffer完成文件复制
```java
@Test
public void copy() throws Exception {
    //源文件
    File srcFile = new File("C:\\Users\\Lenovo\\Desktop\\1.jpg");
    File destFile = new File("C:\\Users\\Lenovo\\Desktop\\server\\1_copy.jpg");
    //得到一个字节输出流、字节输入流
    FileInputStream fis = new FileInputStream(srcFile);
    FileOutputStream fos = new FileOutputStream(destFile);
    //得到文件通道
    FileChannel fisChannel = fis.getChannel();
    FileChannel fosChannel = fos.getChannel();
    //分配缓存区
    ByteBuffer buffer = ByteBuffer.allocate(1024);
    while (true){
        //必须先清空缓存区然后再写入数据到缓存区
        buffer.clear();
        //开始读取一次数据
        int flag = fisChannel.read(buffer);
        if(flag == -1){
            break;
        } 
        //已经读取了数据，把缓存区的模式切换为可读模式
        buffer.flip();
        //把数据写出到输出通道
        fosChannel.write(buffer);
    } 
    fisChannel.close();;
    fosChannel.close();
    System.out.println("复制完成");
}
```
<a name="jqEd3"></a>
### 案例4-分散（Scatter)和聚集（Gatter)
分散读取（Scatter)：是指把Channel通道的数据读取入到多个缓存区中去聚集写入（Gathering)：是指将多个Buffer中的数据聚集到Channel。
```java
@Test
public void test() throws Exception {
    //1.字节输入管道
    FileInputStream is = new FileInputStream("data01_txt");
    FileChannel isChannel = is.getChannel();
    //2.字节输出管道
    FileOutputStream os = new FileOutputStream("data02_txt");
    FileChannel osChannel = os.getChannel();
    //3.定义多个缓存区做数据分散
    ByteBuffer buffer1 = ByteBuffer.allocate(4);
    ByteBuffer buffer2 = ByteBuffer.allocate(1024);
    ByteBuffer[] buffers = {buffer1, buffer2};
    //4.从通道中读取数据分散到各个缓存区
    isChannel.read(buffers);
    //5.从每个缓存区中查询是否有数据读取到了
    for (ByteBuffer buffer : buffers) {
        buffer.flip();//切换到读数据模式
        System.out.println(new String(buffer.array(), 0, buffer.remaining()));
    }
    //6.聚集写入到通道
    osChannel.write(buffers);
    isChannel.close();
    osChannel.close();
    System.out.println("文件复制~");
}
```
输出：
```
hello,Java！
文件复制~
```
<a name="WnU81"></a>
### 案例5-`transferForm()`
从目标通道中去复制原通道数据
```java
@Test
public void test02() throws Exception{
    //1.字节输入管道
    FileInputStream is = new FileInputStream("data01_txt");
    FileChannel isChannel = is.getChannel();//原通道
    //2.字节输出管道
    FileOutputStream os = new FileOutputStream("data03_txt");
    FileChannel osChannel = os.getChannel();//目标通道
    //3.复制数据
    osChannel.transferFrom(isChannel,isChannel.position(),isChannel.size());
    isChannel.close();
    osChannel.close();
    System.out.println("复制完成！");
}
```
<a name="SSpar"></a>
### 案例6-`transferTo()`
把原通道数据复制到目标通道
```java
@Test
public void test03() throws Exception{
    //1.字节输入管道
    FileInputStream is = new FileInputStream("data01_txt");
    FileChannel isChannel = is.getChannel();
    //2.字节输出管道
    FileOutputStream os = new FileOutputStream("data04_txt");
    FileChannel osChannel = os.getChannel();
    //3.复制数据
    //osChannel.transferFrom(isChannel,isChannel.position(),isChannel.size());
    isChannel.transferTo(isChannel.position(),isChannel.size(),osChannel);
    isChannel.close();
    osChannel.close();
    System.out.println("复制完成！");
}
```
<a name="PABv8"></a>
## 6、NIO核心三：选择器（Selector)
<a name="Odr6u"></a>
### 选择器（Selector）概述
选择器（Selector）是SeIectabIeChannIe对象的多路复用器，Selector可以同时监控多个SelectableChannel 的IO状况，也就是说，利用Selector可使一个单独的线程管理多个Channel。<br />Selector是非阻塞IO的核心<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1697902966654-1896c946-4773-4096-a4db-d0fbf9f102e7.png#averageHue=%23f7f7f7&clientId=u1f2f1c64-4049-4&from=paste&height=376&id=u426b3c89&originHeight=941&originWidth=2908&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=128478&status=done&style=none&taskId=u35556bad-f339-4e57-a1a3-b080c638f9a&title=&width=1163.2)

- Java的NIO，用非阻塞的IO方式。可以用一个线程，处理多个的客户端连接，就会使用到Selector（选择器）
- Selector能够检测多个注册的通道上若有事件发生（注意：多个Channel以事件的方式可以注册到同一个 Selector)，如果有事件发生，便获取事件然后针对每个事件进行相应的处理。这样就可以只用一个单线程去管理多个通道，也就是管理多个连接和请求。
- 只有在连接／通道真正有读写事件发生时，才会进行读写，就大大地减少了系统开销，并且不必为每个连接都创建一个线程，不用去维护多个线程避免了多线程之间的上下文切换导致的开销
<a name="UmrIb"></a>
### 选择器（Selector）的应用
创建Selector：通过`Selector.open()`方法创建一个Selector。
```java
Selector selector = Selector.open();
```
向选择器注册通道：`SelectableChannel.register(Selector sel,int ops);`
```java
//1.获取通道
ServerSocketChannel ssChannel = ServerSocketChannel.open();
//2.切换非阻塞模式
ssChannel.configureBlocking(false);
//3.绑定连接
ssChannel.bind(new InetSocketAddress(9898));
//4.获取选择器
Selector selector = Selector.open();
//5.将通道注册到选择器上，并且指定“监听接收事件”
ssChannel.register(select,SelectionKey.OP_ACCEPT);
```
当调用`register(Selector sel, mt ops)`将通道注册选择器时，选择器对通道的监听事件，需要通过第二个参数。ops指定。可以监听的事件类型（用可使用Selection Key的四个常量表示）：

- 读：`SelectionKey.OP_READ(1)`
- 写：`SelectionKey.OP_WRITE(4)`
- 连接：`SelectionKey.OP_CONNECT(8)`
- 接收：`SelectionKey.OP_ACCEPT(16)`
- 若注册时不止监听一个事件，则可以使用‘位或”操作符连接
```java
int interestSet = selectionKey.OP_READ | SelectionKey.OP_WERITE
```
<a name="sdnvd"></a>
## 7、NIO非阻塞式网络通信原理分析
<a name="iW8KO"></a>
### Selector示意图和特点说明
Selector可以实现：一个I/O线程可以并发处理N个客户端连接和读写操作，这从根本上解决了传统同步阻塞I/O一连接一线程模型，架构的性能、弹性伸缩能力和可靠性都得到了极大的提升。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1697901560998-3447adf6-cea7-469e-86b0-e087bdf8a461.jpeg)
<a name="dtuhZ"></a>
### 服务端流程
1、当客户端连接服务端时，服务端会通过`ServerSocketChannel`得到SocketChannel，获取通道
```java
ServerSocketChannel ssChannel = ServerSocketChannel.open();
```
2、切换非阻塞模式
```java
ssChannel.configureBlocking(false);
```
3、绑定连接
```java
ssChannle.bind(new InetSocketAddress(9999))
```
4、获取选择器
```java
Selector selector = Selector.open()
```
5、将通道注册到选择器上，并且指定“监听接收事件”
```java
ssChannel.register(selector,SelectionKey.OP_ACCEPT);
```
6、轮询式的获取选择器上已经“准备就绪”的事件
```java
//轮询式的获取选择器上已经“准备就绪”的事件
while(selector.select() > 0){
    System.out.println("轮一轮");
    //7. 获取当前选择器中所有注册的“选择键（已就绪的监听事件）“
    Iterator<SelectionKey> it = selector.selector.selectedKeys().iterator();
    while(it.hasNext()){
        //8.获取准备”就绪“的事件
        SelectionKey sk = it.next();
        //9.判断具体是什么事件准备就绪
        if(sk.isAcceptable()){
            //10.若”接收就绪“，获取客户端连接
            SocletChannel sChannel = ssChannel.accept();
            //11.切换非阻塞模式
            sChannel。configure Blocking(fales);
            //12.将该通道注册到选择器上
            sChannel.register(selector,SelectionKey.OP_READ);
        }else if(sk.isReadable()){
            //13.获取当前选择器上”读就绪“状态的通道
            SocketChannle sChannel = (SocketChannel)sk.channel();
            //14.读取数据
            ByteBuffer buf = ByteBuffer.allocate(1024);
            int len = 0;
            while((len = sChannel.read(buf)) > 0){
                buf.flip();
                System.out.println()new String(buf.array(),0,len);
                buf.clear();
            }
        } 
        //15.取消选择键SelectionKey
        it.remove();
    }
}
```
<a name="XruSy"></a>
### 客户端流程
1、获取通道
```java
SocketChannel sChannel = SocketChannel.open(new InetSocketAddress("127.0.0.1",9999));
```
2、切换非阻塞模式
```java
sChannel.configureBlocking(false);
```
3、分配指定大小的缓存区
```java
ByteBuffer buf = ByteBuffer.allocate(1024);
```
4、发送数据给服务端
```java
Scanner scan = new Scanner(System.in);
while(scan.hasNext()){
    String str = scan.nextLine();
    buf.put((new SimpleDateFormat("yyyy/MM/ddHH:mm:ss").format(System.currentTimeMillis()) + "\n" + str).getByte());
    buf.flip();
    sChannel.write(buf);
    buf.clear();
} 
//关闭通道
sChannel.close();
```
<a name="MGTUZ"></a>
## 8、NIO非阻塞式网络通信入门案例
需求：服务端接收客户端的连接请求，并接收多个客户端发送过来的事件<br />代码案例
```java
/**
 * 目标：客户端案例实现-基于NIO非阻塞通信
 */
public class Client {
    public static void main(String[] args) throws IOException {
        //1.获取通道
        SocketChannel sChannel = SocketChannel.open(new InetSocketAddress("127.0.0.1",9999));
        //2.切换为非阻塞模式
        sChannel.configureBlocking(false);
        //3.分配指定缓存区大小
        ByteBuffer buf = ByteBuffer.allocate(1024);
        //4.发送数据给服务端
        Scanner sc = new Scanner(System.in);
        while (true){
            System.out.println("请输入：");
            String msg = sc.nextLine();
            buf.put(("波仔：" + msg).getBytes());
            buf.flip();
            sChannel.write(buf);
            buf.clear();
        }
    }
}
```
```java
/**
 * 目标：NIO非阻塞通信下的入门案例：服务端开发
 */
public class Server {
    public static void main(String[] args) throws IOException {
        System.out.println("---服务端启动---");
        //1.获取通道
        ServerSocketChannel ssChannel = ServerSocketChannel.open();
        //2.切换为非阻塞模式
        ssChannel.configureBlocking(false);
        //3.绑定连接的端口
        ssChannel.bind(new InetSocketAddress(9999));
        //4.获取选择器
        Selector selector = Selector.open();
        //5.将通道都注册到选择器上去，并且开始指定监听接收事件
        ssChannel.register(selector, SelectionKey.OP_ACCEPT);
        //6.使用Selector选择器轮询已经就绪好的事件
        while(selector.select() > 0){
            System.out.println("开始一轮事件处理~~~");
            //7.获取选择器中的所有注册的通道中已经就序好的事件
            Iterator<SelectionKey> it = selector.selectedKeys().iterator();
            //8.开始遍历这些准备好的事件
            while(it.hasNext()){
                //提取当前这个事件
                SelectionKey sk = it.next();
                //9.判断这个事件具体是什么事件
                if(sk.isAcceptable()){
                    //10.直接获取当前接入的客户端通道
                    SocketChannel schannel = ssChannel.accept();
                    //11.将客户端通道也设置为非阻塞式的
                    schannel.configureBlocking(false);
                    //12.将客户端通道也注册到选择器Selector上
                    schannel.register(selector,SelectionKey.OP_READ);
                }else if(sk.isReadable()){
                    //13.获取当前选择器上的”读就绪事件“
                    SocketChannel sChannel = (SocketChannel) sk.channel();
                    //14.开始读取数据
                    ByteBuffer buf = ByteBuffer.allocate(1024);
                    int len = 0;
                    while ((len = sChannel.read(buf)) > 0){
                        buf.flip();
                        System.out.println(new String(buf.array(),0,len));
                        buf.clear();//”清除“之前的数据
                    }
                } 
                //处理完毕当前事件后，需要移除掉当前事件.否则会重复处理
                it.remove();
            }
        }
    }
}
```
输出：
```
//server
---服务端启动---
开始一轮事件处理~~~
开始一轮事件处理~~~
波仔：你好
开始一轮事件处理~~~
开始一轮事件处理~~~
波仔：晚上好

//client1
请输入：
你好
请输入：

//client2
请输入：
晚上好
请输入：
```
<a name="k90qa"></a>
## 9、NIO网络编程应用实例-群聊系统
<a name="DlfuV"></a>
### 目标
需求：进一步理解NIO非阻塞网络编程机制，实现多人群聊

- 编写一个NIO群聊系统，实现客户端与客户端的通信需求（非阻塞）
- 服务器端：可以监测用户上线，离线，并实现消息转发功能
- 客户端：通过channel可以无阻塞发送消息给其他所有客户端用户，同时可以接受其它客户端用户通过服务端转发来的消息
<a name="j7oAH"></a>
### 服务端代码实现
```java
/**
 * 目标：服务端群聊系统实现
 */
public class Server {
    //定义属性
    private Selector selector;
    private ServerSocketChannel ssChannel;
    private static final int PORT = 9999;
    //构造器
    //初始化工作
    public Server() {
        try {
            //1.创建选择器
            selector = Selector.open();
            //2.获取通道
            ssChannel = ServerSocketChannel.open();
            //3.切换为非阻塞模式
            ssChannel.configureBlocking(false);
            //4.绑定连接的端口
            ssChannel.bind(new InetSocketAddress(PORT));
            //5.将通道都注册到选择器上去，并且开始指定监听接收事件
            ssChannel.register(selector, SelectionKey.OP_ACCEPT);
        }catch (IOException e){
            e.printStackTrace();
        }
    } 

    public static void main(String[] args) {
        //创建服务端对象
        Server server = new Server();
        //开始监听客户端的各种消息事件：连接、群聊消息、离线消息
        server.listen();
    } 

    //监听
    public void listen() {
        //System.out.println("监听线程：" + Thread.currentThread().getName());
        try {
            while (selector.select() > 0){
                //a.获取选择器中所有注册通道的就绪事件
                Iterator<SelectionKey> it = selector.selectedKeys().iterator();
                //b.开始遍历这个事件
                while (it.hasNext()){
                    //提取这个事件
                    SelectionKey sk = it.next();
                    //c.判断这个事件
                    if(sk.isAcceptable()) {
                        //客户端接入请求
                        //获取当前客户端通道
                        SocketChannel schannel = ssChannel.accept();
                        //注册成非阻塞模式
                        schannel.configureBlocking(false);
                        //注册给选择器，监听读数据的事件
                        schannel.register(selector,SelectionKey.OP_READ);
                    }else if(sk.isReadable()){
                        //处理这个客户端的消息，接收它，然后实现转发逻辑
                        readClientData(sk);
                    } 
                    it.remove();//处理完毕之后，需要移除当前事件
                }
            }
        }catch (Exception e){
            e.printStackTrace();
        }
    }

    //接收当前客户端的信息，转发给其他全部客户端通道
    private void readClientData(SelectionKey sk) {
        SocketChannel sChannel = null;
        try {
            //直接得到当前客户端通道
            sChannel = (SocketChannel) sk.channel();
            //创建缓存区对象，开始接收客户端通道的数据
            ByteBuffer buffer = ByteBuffer.allocate(1024);
            int count = sChannel.read(buffer);
            if(count > 0) {
                buffer.flip();
                //提取读取到的信息
                String msg = new String(buffer.array(),0,buffer.remaining());
                System.out.println("接收到了客户端的消息：" + msg);
                //把这个消息推送给全部客户端接收
                sendMsgToAllClient(msg,sChannel);
            }
        }catch (Exception e){
            try {
                System.out.println("有人离线了：" + sChannel.getRemoteAddress());
                //当前客户端离线
                sk.cancel();//取消注册
                sChannel.close();
            } catch (IOException e1) {
                e1.printStackTrace();
            }
        }
    }

    //把当前客户端的消息推送给当前全部在线注册的channel
    private void sendMsgToAllClient(String msg, SocketChannel sChannel) throws
        IOException {
        System.out.println("服务端开始转发这个消息，当前处理的线程" + Thread.currentThread().getName());
        for(SelectionKey key : selector.keys()) {
            Channel channel = key.channel();
            //不要把消息发给自己
            if(channel instanceof SocketChannel && channel != sChannel){
                ByteBuffer buffer = ByteBuffer.wrap(msg.getBytes());
                ((SocketChannel)channel).write(buffer);
            }
        }
    }
}
```
<a name="f2hbb"></a>
### 客户端代码实现
```java
/**
 * 目标：客户端代码逻辑的实现
 */
public class Client {
    //1.定义客户端相关属性
    private Selector selector;
    private static int PORT = 9999;
    private SocketChannel socketChannel;

    //2.初始化客户端信息
    public Client(){
        try {
            //a.创建选择器
            selector = Selector.open();
            //b.连接服务器
            socketChannel = SocketChannel.open(new
                                               InetSocketAddress("127.0.0.1",PORT));
            //c.设置非阻塞模式
            socketChannel.configureBlocking(false);
            socketChannel.register(selector, SelectionKey.OP_READ);
            System.out.println("当前客户端准备完成");
        }catch (Exception e){
            e.printStackTrace();
        }
    } 

    public static void main(String[] args) {
        Client client = new Client();
        //定义一个线程，专门负责监听服务端发送过来的读消息事件
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    client.readInfo();
                }catch (Exception e){
                    e.printStackTrace();
                }
            }
        }).start();
        //发消息
        Scanner sc = new Scanner(System.in);
        while (sc.hasNextLine()){
            System.out.println("------------------");
            String s = sc.nextLine();
            client.sendToServer(s);
        }
    } 

    private void sendToServer(String s) {
        try {
            socketChannel.write(ByteBuffer.wrap(("波仔说：" + s).getBytes()));
        } catch (IOException e) {
            e.printStackTrace();
        }
    } 

    //
    private void readInfo() throws IOException {
        //if(selector.select() > 0){
        while(selector.select() > 0){
            Iterator<SelectionKey> iterator =
            selector.selectedKeys().iterator();
            while (iterator.hasNext()){
                SelectionKey key = iterator.next();
                if(key.isReadable()){
                    SocketChannel sc = (SocketChannel) key.channel();
                    ByteBuffer buffer = ByteBuffer.allocate(1024);
                    sc.read(buffer);
                    System.out.println(new String(buffer.array()).trim());
                    System.out.println("-dsd------------------------");
                } 
                iterator.remove();
            }
        }
    }
}
```
<a name="TwV5u"></a>
### 输出
```
//服务端
接收到了客户端的消息：波仔说：lll
服务端开始转发这个消息，当前处理的线程main
接收到了客户端的消息：波仔说：lkkkk
服务端开始转发这个消息，当前处理的线程main
接收到了客户端的消息：波仔说：mnkkkk
服务端开始转发这个消息，当前处理的线程main
有人离线了：/127.0.0.1:55574


//客户端一
当前客户端准备完成
lll
------------------
波仔说：lkkkk
-dsd------------------------
mnkkkk
------------------


//客户端二
当前客户端准备完成
lkkkk
------------------
波仔说：mnkkkk
-dsd------------------------
```
