JavaNIO
<a name="Zq5xs"></a>
## 一、简介
什么是 NIO ？<br />NIO，英文全称 Non-blocking I/O，在 Java 领域，也称为 New I/O，是一种同步非阻塞的 I/O 模型，是解决高并发、I/O 高性能的有效方式，已经被越来越多地应用到大型应用服务器中。<br />既然 NIO 如此的受欢迎，那么它的本质是什么？又是如何实现高性能的呢？<br />带着这个问题，先从传统的阻塞 I/O 说起，在一步一步的分析 NIO 是如何利用非阻塞模式来解决大量请求带来的性能瓶颈问题。
<a name="VUqZp"></a>
## 二、传统 BIO 的瓶颈
在介绍 NIO 之前， 先回顾一下传统的服务器端同步阻塞 I/O （也称为 BIO，英文全称 blocking I/O）的经典编程模型。<br />采用 BIO 通信模型的服务端，通常由一个独立的 **Acceptor** 线程负责监听所有客户端的连接，当服务端接受到多个客户端的链接请求时，通常所有的客户端请求需要排队等待服务端一个一个的处理。<br />BIO 简易通信模型图如下！<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1692338950396-547a2f3a-48e6-4581-abb1-0fbced951cb3.jpeg)<br />一般在服务端通过`while(true)`循环中会调用`accept()` 方法监听客户端的连接，一旦接收到一个连接请求，就可以建立通信套接字进行读写操作，此时不能再接收其他客户端连接请求，只能等待同当前连接的客户端的操作执行完成再处理下一个连接请求。<br />**下面是一个简易版的 BIO 服务端，示例程序**：
```java
public class BioServer {

    public static void main(String[] args) throws IOException {
        // 初始化服务端socket并且绑定 8080 端口
        ServerSocket serverSocket = new ServerSocket(8080);
        // 循环监听所有客户端的请求接入
        while (true) {
            // 监听客户端请求
            Socket socket = serverSocket.accept();
            // 读取客户端发送的请求数据
            InputStream in = socket.getInputStream();
            byte[] buffer = new byte[1024];
            int len;
            while ((len = in.read(buffer))!=-1){
                System.out.println(new String(buffer,0,len));
            }
            System.out.println("接收完毕");
            // 关闭流
            in.close();
        }
    }
}
```
**下面是一个简易版的 BIO 客户端，示例程序**：
```java
public class BioClient {
    public static void main(String[] args) throws IOException {
        Socket socket = new Socket("127.0.0.1",8080);
        OutputStream out = socket.getOutputStream();
        out.write("Hello,this is Client".getBytes());
        out.close();
        socket.close();
    }
}
```
随着客户端的请求次数增多，可能需要排队的时间会越来越长，用户等待的时间越久，体验感就越差。<br />因此有的大仙就将服务端的编程模型进行了适当的改造，引入多线程方式来处理客户端的请求，**从而实现了 N （客户端请求数量）大于 M （服务端处理客户端请求的线程数量）的 I/O 模型**。<br />改造后的 IO 模型图，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1692339133338-db5f07fc-91c4-4095-8f7d-fe1cb6fe6fcc.jpeg)<br />采用线程池和任务队列可以实现一种叫做伪异步的 I/O 通信框架，当有新的客户端接入时，将客户端的 Socket 封装成一个 Task 投递到线程池中进行处理。<br />**下面是一个简易版的改造后 BIO 服务端，示例程序**：
```java
public class BioServerTest {

    public static void main(String[] args) throws IOException {
        //在线程池中创建5个固定大小线程，来处理客户端的请求
        ExecutorService executorService = Executors.newFixedThreadPool(5);

        //初始化服务端socket并且绑定 8080 端口
        ServerSocket serverSocket = new ServerSocket(8080);
        //循环监听客户端请求
        while (true) {
            // 监听客户端请求
            Socket socket = serverSocket.accept();
            //使用线程池处理多个任务
            executorService.execute(new Runnable() {

                @Override
                public void run() {
                    // 读取客户端发送的请求数据
                    InputStream in = socket.getInputStream();
                    byte[] buffer = new byte[1024];
                    int len;
                    while ((len = in.read(buffer))!=-1){
                        System.out.println(new String(buffer,0,len));
                    }
                    System.out.println("接收完毕");
                    // 关闭流
                    in.close();
                }
            });
        }
    }
}
```
服务端使用线程池来处理客户端的请求，线程数量为 5 个，由于线程池可以设置消息队列的大小和最大线程数，因此它的资源占用是可控的，无论多少个客户端并发访问，都不会导致资源的耗尽和宕机。<br />服务端的编程模型进行改造后，处理客户端的请求速度确实提升了不少。<br />之所以使用多线程，主要原因在于`socket.accept()`、`socket.read()`、`socket.write()`三个主要函数都是同步阻塞的，当一个连接在处理 I/O 的时候，系统是阻塞的，如果是单线程的话，必然所有的任务等都挂在它名下，处理效率低下；引入多线程之后，等待的资源就可以释放出来，充分发挥 CPU 多任务的并发处理能力。<br />在活动连接数不是特别高的情况下，这种编程模型还是不错的，不用过多考虑系统的过载、限流等问题。<br />但是呢，这个模型也有弊端，底层还是 BIO 模型，严重依赖于线程，在操作系统中线程是很"昂贵"的资源，主要表现在以下几点：

- 1.线程的创建和销毁成本很高，在 Linux 这样的操作系统中，线程本质上就是一个进程，创建和销毁都是重量级的系统函数
- 2.线程本身占用较大内存，像 Java 的线程栈，一般至少分配 512K～1M 的空间，如果系统中的线程数过千，恐怕整个 JVM 的内存都会被吃掉一半
- 3.线程的切换成本也很高，操作系统发生线程切换的时候，需要保留线程的上下文，然后执行系统调用，如果线程数过高，可能执行线程切换的时间甚至会大于线程执行的时间，这时候带来的表现往往是系统 load 偏高、CPU 系统使用率特别高，导致系统几乎陷入不可用的状态

最重要的是，当面对十万甚至百万级请求接入的时候，传统的 BIO 模型真的无能为力，随着移动端应用的兴起和各种网络游戏的盛行，百万级的请求接入非常普遍，因此需要一种更高效的 I/O 处理模型来应对更高的并发量。
<a name="P5Fhe"></a>
## 三、NIO 解析
在上文中也提到，**BIO 是同步阻塞的 IO 模型**，**NIO 是同步非阻塞的 IO 模型**，仅仅一个字的差别，它们到底有何不同呢？<br />首先来看看几种常见 I/O 模型！<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1692342747029-f5a4f547-1a02-4927-b4cd-1ff5df63c63d.jpeg)<br />在 Linux 操作系统上，以发起读取数据为例：

- 在传统的 BIO 中，也就是同步阻塞 IO 模型，当系统调用`recvfrom()`函数时，如果里面没有数据，函数会一直阻塞，直到收到数据，最后返回读到的数据。
- 而在 NIO 中，也就是同步非阻塞 IO 模型，当系统调用`recvfrom()`函数时，如果里面有数据，就把数据读取出来并返回；如果没有就返回一个`EWOULDLOCK`标记，永远不会阻塞。
- 同时还有最新的 AIO，它是一种异步非阻塞的 IO 模型，主要基于**事件回调机制**来实现，当系统调用`recvfrom()`函数时，不管里面有没有数据，直接返回结果；当后台处理完成后，操作系统会通知相应的线程进行后续的操作。

通俗的说，在读取数据的过程中，BIO 只关注“**我要读**”，NIO 只关注“**我可以读了**”，而 AIO 只关注“**我读完了**”。<br />其中 NIO 一个很重要的特点就是：**采用单线程非阻塞的编程方式来处理客户端所有的连接请求，虽然执行过程比较消耗 CPU，但性能非常的高**。<br />那 NIO 具体是如何实现的呢？下面一起来看看！
<a name="KMpTs"></a>
### 3.1、NIO 工作原理介绍
与传统的 BIO 不同，NIO 新增了 **Channel、Selector、Buffer** 等抽象概念，**支持面向缓冲、基于通道的 I/O 数据传输方法**。<br />NIO 简易模型图，如下：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1692339637868-99b4ffde-f9b8-4094-aae6-557e4e7a4585.jpeg)<br />与此同时，NIO 还提供了与传统 BIO 模型中的 Socket 和 `ServerSocket` 相对应的 `SocketChannel` 和 `ServerSocketChannel` 两种不同的套接字通道实现。<br />NIO 这两种通道都支持**阻塞和非阻塞**两种模式，阻塞模式使用就像传统中的 BIO 一样，比较简单，但是性能和可靠性都不好；**非阻塞模式正好与之相反**。<br />对于低负载、低并发的应用程序，可以使用同步阻塞 I/O 来提升开发效率和更好的维护性；对于高负载、高并发的**网络**应用，可以使用 NIO 的非阻塞模式来开发，可以显著的提升数据传输效率。<br />NIO 在 Java 1.4 中引入，对应的代码实现在java.nio包下，涉及到的核心类关联关系，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1692341791283-4a4c242d-35ff-4252-b34a-c433d3b3c675.jpeg)<br />上图中有三个关键类：**Channel 、Selector 和 Buffer**，它们是 NIO 中的核心概念。

- **Channel：可以理解为通道**
- **Selector：可以理解为选择器**
- **Buffer：可以理解为数据缓冲区**

刚接触 NIO 的同学，当第一眼看到 **Channel、Selector、Buffer** 等抽象概念，可能感觉难以理解，下面还是用之前介绍的城市交通工具来继续形容 一下 NIO 的工作方式。<br />这里的 **Channel** 要比 **Socket** 更加具体，它可以比作为某种具体的交通工具，如汽车、高铁或者飞机等，而 **Selector** 可以比作为一个车站的车辆运行调度系统，它将负责监控每辆车的当前运行状态，是已经出站还是在路上等等，也就是说它可以轮询每个 **Channel** 的状态。<br />还有一个 **Buffer** 类，可以将它看作为 IO 中 **Stream**，但是它比 IO 中的 **Stream** 更加具体化，可以将它比作为车上的座位，**Channel** 如果是汽车的话，那么 **Buffer** 就是汽车上的座位，**Channel** 如果是高铁上，那么 **Buffer** 就是高铁上的座位，它始终是一个具体的概念，这一点与 **Stream** 不同。<br />NIO 引入了 **Channel、Buffer 和 Selector** 就是想把 IO 传输过程中涉及到的**信息具体化**，让程序员有机会去控制它们。<br />当进行传统的网络 IO 操作时，比如调用`write()`往 Socket 中的SendQ队列写数据时，当一次写的数据超过SendQ长度时，操作系统会按照SendQ 的长度进行分割的，这个过程中需要将用户空间数据和内核地址空间进行切换，而这个切换不是程序员可以控制的，由底层操作系统来处理。<br />而在Buffer中，可以控制Buffer的capacity（容量），并且是否扩容以及如何扩容都可以控制。<br />讲了这么多，可能有的同学感觉，依然很难理解，下面直接来看看具体的代码实现。<br />**下面是一个简易版的 NIO 服务端，示例程序**：
```java
/**
 * NIO 服务端
 */
public class NioServerTest {

    public static void main(String[] args) throws IOException {
        // 打开服务器套接字通道
        ServerSocketChannel ssc = ServerSocketChannel.open();
        // 服务器配置为非阻塞
        ssc.configureBlocking(false);
        // 进行服务的绑定，监听8080端口
        ssc.socket().bind(new InetSocketAddress(8080));

        // 构建一个Selector选择器,并且将channel注册上去
        Selector selector = Selector.open();
        // 将serverSocketChannel注册到selector,并对accept事件感兴趣(serverSocketChannel只能支持accept操作)
        ssc.register(selector, SelectionKey.OP_ACCEPT);

        while (true){
            // 查询指定事件已经就绪的通道数量，select方法有阻塞效果,直到有事件通知才会有返回，如果为0就跳过
            int readyChannels = selector.select();
            if(readyChannels == 0) {
                continue;
            };
            //通过选择器取得所有key集合
            Set<SelectionKey> selectedKeys = selector.selectedKeys();
            Iterator<SelectionKey> iterator = selectedKeys.iterator();
            while (iterator.hasNext()){
                SelectionKey key = iterator.next();
                //判断状态是否有效
                if (!key.isValid()) {
                    continue;
                }
                if (key.isAcceptable()) {
                    // 处理通道中的连接事件
                    ServerSocketChannel server = (ServerSocketChannel) key.channel();
                    SocketChannel sc = server.accept();
                    sc.configureBlocking(false);
                    System.out.println("接收到新的客户端连接，地址：" + sc.getRemoteAddress());

                    // 将通道注册到选择器并处理通道中可读事件
                    sc.register(selector, SelectionKey.OP_READ);
                } else if (key.isReadable()) {
                    // 处理通道中的可读事件
                    SocketChannel channel = (SocketChannel) key.channel();
                    ByteBuffer byteBuffer = ByteBuffer.allocate(1024);
                    while (channel.isOpen() && channel.read(byteBuffer) != -1) {
                        // 长连接情况下,需要手动判断数据有没有读取结束 (此处做一个简单的判断: 超过0字节就认为请求结束了)
                        if (byteBuffer.position() > 0) {
                            break;
                        };
                    }
                    byteBuffer.flip();

                    //获取缓冲中的数据
                    String result = new String(byteBuffer.array(), 0, byteBuffer.limit());
                    System.out.println("收到客户端发送的信息，内容：" + result);

                    // 将通道注册到选择器并处理通道中可写事件
                    channel.register(selector, SelectionKey.OP_WRITE);
                } else if (key.isWritable()) {
                    // 处理通道中的可写事件
                    SocketChannel channel = (SocketChannel) key.channel();
                    ByteBuffer byteBuffer = ByteBuffer.allocate(1024);
                    byteBuffer.put("server send".getBytes());
                    byteBuffer.flip();
                    channel.write(byteBuffer);

                    // 将通道注册到选择器并处理通道中可读事件
                    channel.register(selector, SelectionKey.OP_READ);
                    //写完之后关闭通道
                    channel.close();
                }
                //当前事件已经处理完毕，可以丢弃
                iterator.remove();
            }
        }
    }
}
```
**下面是一个简易版的 NIO 客户端，示例程序**：
```java
/**
 * NIO 客户端
 */
public class NioClientTest {

    public static void main(String[] args) throws IOException {
        // 打开socket通道
        SocketChannel sc = SocketChannel.open();
        //设置为非阻塞
        sc.configureBlocking(false);
        //连接服务器地址和端口
        sc.connect(new InetSocketAddress("127.0.0.1", 8080));
        while (!sc.finishConnect()) {
            // 没连接上,则一直等待
            System.out.println("客户端正在连接中，请耐心等待");
        }

        // 发送内容
        ByteBuffer writeBuffer = ByteBuffer.allocate(1024);
        writeBuffer.put("Hello，我是客户端".getBytes());
        writeBuffer.flip();
        sc.write(writeBuffer);

        // 读取响应
        ByteBuffer readBuffer = ByteBuffer.allocate(1024);
        while (sc.isOpen() && sc.read(readBuffer) != -1) {
            // 长连接情况下,需要手动判断数据有没有读取结束 (此处做一个简单的判断: 超过0字节就认为请求结束了)
            if (readBuffer.position() > 0) {
                break;
            };
        }
        readBuffer.flip();

        String result = new String(readBuffer.array(), 0, readBuffer.limit());
        System.out.println("客户端收到服务端：" + sc.socket().getRemoteSocketAddress() + "，返回的信息：" + result);

        // 关闭通道
        sc.close();
    }
}
```
最后，依次启动服务端、客户端，看看控制台输出情况如何。<br />服务端控制台结果如下：
```
接收到新的客户端连接，地址：/127.0.0.1:57644
收到客户端发送的信息，内容：Hello，我是客户端
```
客户端控制台结果如下：
```
客户端收到服务端：/127.0.0.1:8080，返回的信息：server send
```
**从编程上可以看到，NIO 的操作比传统的 IO 操作要复杂的多**！<br />**Selector** 被称为**选择器** ，当然也可以翻译为**多路复用器** 。它是 Java NIO 核心组件中的一个，用于检查一个或多个 **Channel**（通道）的状态是否处于**连接就绪**、**接受就绪**、**可读就绪**、**可写就绪**。<br />如此可以实现单线程管理多个 **channel** 的目的，也就是可以管理多个网络连接。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1692340106199-3414f0af-4117-469e-81b1-3595c6f8fdee.jpeg)<br />**使用 Selector 的好处在于** ：相比传统方式使用多个线程来管理 IO，Selector 只使用了一个线程就可以处理所有的通道，从而实现网络高效传输！<br />同时，Java 对`Selector`还做了优化处理，当调用`Selector.select()`方法时有阻塞的效果，**当没有**`**Channel**`**的时候，不会一直不停的去空循环，避免消耗 CPU 资源**，这个功能需要操作系统来支持。<br />针对不同的操作系统，Java 会执行对应的系统调用（Linux 2.6 之前是 select、poll，2.6 之后是 epoll，Windows 是 IOCP），当有新事件到来的时候才会返回。<br />所以，可以放心大胆地在一个`while(true)`里面调用这个函数而不用担心 CPU 空转。
<a name="zS4BJ"></a>
### 3.2、Buffer 类详解
接着再来说说 **Buffer**，也称为**缓冲区**，在 Java NIO 中负责数据的存取。<br />Buffer 其实是一个数组，可以用于存储不同类型的数据，根据数据类型的不同（boolean 除外），提供了相应类型的缓冲区类，类关系图如下：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1692340438806-db59458c-2f42-468c-8dba-455dab7f37f1.png#averageHue=%23fafafa&clientId=u883342f5-d07f-4&from=paste&height=411&id=ua7beb0da&originHeight=1027&originWidth=3086&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=102835&status=done&style=none&taskId=uf9ffcb15-fe3f-42e1-b8a8-4d91508d3e5&title=&width=1234.4)<br />上述的子类，管理方式几乎一致，都可以通过`allocate()`静态方法来获取堆内缓冲区对象。<br />Buffer 有 2 个核心方法和 4 个核心属性，下面一起来看看。<br />核心方法，内容如下：

- `put()`：存入数据到缓冲区中
- `get()`：从缓冲区中的读取数据

核心属性，内容如下：

- `capacity`：容量，表示缓冲区中最大存储数据的容量，一旦声明不能更改
- `limit`：界限，表示缓冲区中可以操作数据的最大界限
- `position`：位置，表示缓冲区中正在操作数据的位置
- `mark`：标记，可以使用`mark()`方法记录当前`position`的位置，后续可以通过`reset()`方法恢复到`mark`标记的位置

以分配一个大小为 10 个字节容量的缓冲区，向里面写入`abcde`并读取数据为例，Buffer 处理过程如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1692343349441-5844fdf2-5f17-4a53-b6ec-de07d8f9783e.jpeg)<br />有几个地方，需要特别注意：

- 0 <= mark <= position <= limit <= capacity
- `limit`后的数据不能进行读写

下面是一些关于 Buffer 类相关操作示例介绍！

- `put` 和 `get` 方法基本操作，示例程序
```java
public static void main(String[] args) {
    String str = "abcde";

    //分配一个指定大小的缓冲区
    ByteBuffer byteBuffer = ByteBuffer.allocate(1024);
    System.out.println("---------allocate-----------");
    System.out.println(byteBuffer.capacity());   //1024
    System.out.println(byteBuffer.limit());      //1024
    System.out.println(byteBuffer.position());   //0

    //利用 put() 存入数据到缓冲区中
    byteBuffer.put(str.getBytes());
    System.out.println("---------put-----------");
    System.out.println(byteBuffer.capacity());   //1024
    System.out.println(byteBuffer.limit());      //1024
    System.out.println(byteBuffer.position());   //5

    //切换到读数据模式
    byteBuffer.flip();
    System.out.println("---------flip-----------");
    System.out.println(byteBuffer.capacity());   //1024
    System.out.println(byteBuffer.limit());      //5,limit 表示可以操作数据的大小,只有 5 个字节的数据给你读,所以可操作数据大小是 5
    System.out.println(byteBuffer.position());   //0,读数据要从第 0 个位置开始读

    //利用 get() 读取缓冲区中的数据
    byte[] dst = new byte[byteBuffer.limit()];
    byteBuffer.get(dst);
    System.out.println(new String(dst,0,dst.length));
    System.out.println("---------get-----------");
    System.out.println(byteBuffer.capacity());   //1024
    System.out.println(byteBuffer.limit());      //5,可以读取数据的大小依然是 5 个
    System.out.println(byteBuffer.position());   //5,读完之后位置变到了第 5 个

    //rewind() 可重复读
    byteBuffer.rewind();         //这个方法调用完后,又变成了读模式
    System.out.println("---------rewind-----------");
    System.out.println(byteBuffer.capacity());   //1024
    System.out.println(byteBuffer.limit());      //5
    System.out.println(byteBuffer.position());  //0

    //clear() 清空缓冲区,虽然缓冲区被清空了，但是缓冲区中的数据依然存在，只是出于"被遗忘"状态。意思其实是，缓冲区中的界限、位置等信息都被置为最初的状态了，所以无法再根据这些信息找到原来的数据了，原来数据就出于"被遗忘"状态
    byteBuffer.clear();
    System.out.println("---------clear-----------");
    System.out.println(byteBuffer.capacity());   //1024
    System.out.println(byteBuffer.limit());      //1024
    System.out.println(byteBuffer.position());  //0
}
```
输出结果如下：
```
---------allocate-----------
1024
1024
0
---------put-----------
1024
1024
5
---------flip-----------
1024
5
0
abcde
---------get-----------
1024
5
5
---------rewind-----------
1024
5
0
---------clear-----------
1024
1024
0
```

- mark 方法基本操作，示例程序
```java
public static void main(String[] args) {
    String str = "abcde";

    // 写数据
    ByteBuffer byteBuffer = ByteBuffer.allocate(1024);
    byteBuffer.put(str.getBytes());

    // 读取指定几个数据
    byteBuffer.flip();
    byte[] byteArray = new byte[byteBuffer.limit()];
    byteBuffer.get(byteArray,0,2);
    System.out.println(new String(byteArray,0,2));  //结果是 ab
    System.out.println(byteBuffer.position());   //结果是 2

    // 利用mark()标记一下当前 position 的位置
    byteBuffer.mark();
    byteBuffer.get(byteArray,2,2);
    System.out.println(new String(byteArray,2,2));
    System.out.println(byteBuffer.position());   //结果是 4

    // 利用 reset() 恢复到 mark 的位置
    byteBuffer.reset();
    System.out.println(byteBuffer.position());   //结果是 2

    //判断缓冲区中是否还有剩余数据
    if (byteBuffer.hasRemaining()) {
        //获取缓冲区中可以操作的数量
        System.out.println(byteBuffer.remaining());  //结果是 3,上面 position 是从 2 开始的
    }
}
```
输出结果如下：
```
ab
2
cd
4
2
3
```

- `allocate` 和 `allocateDirect` 的区别

在 Buffer 类中，除了有`allocate()`静态方法可以创建对象外，还有`allocateDirect()`也可以创建对象。<br />它们直接最明显的区别是：`allocate()`创建的对象，是建立在 JVM 的内存之中的，也称为堆内内存；`allocateDirect()`创建的对象，是建立在 JVM 的内存之外的，不会占用 JVM 的内存空间，也称为堆外内存，更具体的说是建立到物理内存之中，由操作系统来代管。<br />示例程序如下：
```java
public static void main(String[] args) {
    // 分配直接缓冲区
    ByteBuffer byteBuffer = ByteBuffer.allocateDirect(1024);
    // 判断是直接缓冲区还是非直接缓冲区
    System.out.println(byteBuffer.isDirect());
}
```
输出结果：
```
true
```
JVM 对堆外对象的管理，主要是通过操作系统与物理内存之间建立一份映射文件，以此来扩充或者回收物理内存空间。<br />此外，JVM 堆内内存和堆外内存的区别，可以用如下的例子来解释。<br />以 Java 程序读取磁盘文件为例，操作系统出于安全的考虑，当应用程序向操作系统发起读取磁盘数据的操作时，首先是操作系统会将从磁盘读取的数据存放到内核空间，然后 CPU 再将内核空间的数据复制到 JVM 内存中，以供 Java 程序使用，在 JVM 中的数据，都属于堆内内存。<br />还有另一种方式，就是省掉 CPU 将内核空间的数据复制到 JVM 内存的过程，当操作系统将从磁盘读取的数据存放到内核空间后，Java 程序通过映射文件直接操作物理内存的数据，效率上会有所提高，在 JVM 中的数据，都属于堆外内存。<br />那是不是所有的对象都可以直接采用堆外内存呢？<br />通过`allocateDirect()`方式分配的内存，比起 JVM 内存的分配要耗时得多，所以并非不论什么时候使用`allocateDirect()`的操作效率都是最高的。<br />以拷贝文件为例，从实际的使用情况看，当操作的数据量很小时，两种操作使用时间基本是同样的，第一种方式有时可能会更快些；当操作的数据量大，比如大文件，堆外内存有优势。
<a name="YtmAx"></a>
## 四、小结
最后总结一些，NIO 相比传统的 BIO 模型，最大的不同点在于：采用**单线程非阻塞的编程方式**来处理客户端所有的连接请求，虽然执行过程比较消耗 CPU，但性能非常的高。<br />不过 java NIO 并没有完全屏蔽平台的差异，它仍然是基于各个操作系统的 I/O 系统实现的，差异仍然存在。<br />其次 JDK 的 NIO 底层由 epoll 实现，该实现饱受诟病的空轮询 bug 会导致 cpu 飙升 100%！<br />最后如果自己使用 NIO 做网络编程并不容易，自行实现的 NIO 很容易出现各类 bug，陷阱重重，维护成本较高。<br />推荐大家使用成熟的 NIO 框架，比如 Netty，MINA 等。解决了很多 NIO 的陷阱，并屏蔽了操作系统的差异，有较好的性能和编程模型。
