Java
<a name="Ruyj8"></a>
## 一、简介
在计算机中，IO 传输数据有三种工作方式，分别是： **BIO、NIO、AIO**。<br />在讲解 **BIO、NIO、AIO** 之前，先来回顾一下这几个概念：**同步与异步，阻塞与非阻塞**。<br />**同步与异步的区别**

- 同步就是发起一个请求后，接受者未处理完请求之前，不返回结果。
- 异步就是发起一个请求后，立刻得到接受者的回应表示已接收到请求，但是接受者并没有处理完，接受者通常依靠事件回调等机制来通知请求者其处理结果。

**阻塞和非阻塞的区别**

- 阻塞就是请求者发起一个请求，一直等待其请求结果返回，也就是当前线程会被挂起，无法从事其他任务，只有当条件就绪才能继续。
- 非阻塞就是请求者发起一个请求，不用一直等着结果返回，可以先去干其他事情，当条件就绪的时候，就自动回来。

要讲的 **BIO、NIO、AIO** 就是**同步与异步、阻塞与非阻塞**的组合。

- BIO：同步阻塞 IO；
- NIO：同步非阻塞 IO；
- AIO：异步非阻塞 IO;

**不同的工作方式，带来的传输效率是不一样的，下面以网络 IO 为例，一起看看不同的工作方式下，彼此之间有何不同**。
<a name="xV3w3"></a>
## 二、BIO
BIO 俗称同步阻塞 IO，是一种非常传统的 IO 模型，也是最常用的网络数据传输处理方式，优点就是编程简单，但是缺点也很明显，I/O 传输性能一般比较差，CPU 大部分处于空闲状态。<br />采用 BIO 通信模型的服务端，通常由一个独立的 **Acceptor** 线程负责监听所有客户端的连接，当服务端接受到多个客户端的请求时，所有的客户端只能排队等待服务端一个一个的处理。<br />BIO 通信模型图如下！<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1689040454470-25e82cab-ce7a-49c9-9b1e-6f245a81b74e.jpeg)<br />一般在服务端通过`while(true)`循环中会调用`accept()` 方法监听客户端的连接，一旦接收到一个连接请求，就可以建立通信套接字进行读写操作，此时不能再接收其他客户端连接请求，只能等待同当前连接的客户端的操作执行完成。<br />**服务端操作，样例程序如下**：
```java
public class BioServerTest {

    public static void main(String[] args) throws IOException {
        //初始化服务端socket并且绑定 8080 端口
        ServerSocket serverSocket = new ServerSocket(8080);
        //循环监听客户端请求
        while (true){
            try {
                //监听客户端请求
                Socket socket = serverSocket.accept();

                //将字节流转化成字符流，读取客户端输入的内容
                BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                //读取一行数据
                String str = bufferedReader.readLine();
                //打印客户端发送的信息
                System.out.println("服务端收到客户端发送的信息：" + str);

                //向客户端返回信息，将字符转化成字节流，并输出
                PrintWriter printWriter = new PrintWriter(new OutputStreamWriter(socket.getOutputStream()),true);
                printWriter.println("hello，我是服务端，已收到消息");

                // 关闭流
                bufferedReader.close();
                printWriter.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
```
**客户端操作，样例程序如下**：
```java
public class BioClientTest {

    public static void main(String[] args) {
        //创建10个线程，模拟10个客户端，同时向服务端发送请求
        for (int i = 0; i < 10; i++) {
            final int j = i;//定义变量
            new Thread(new Runnable() {

                @Override
                public void run() {
                    try {
                        //通过IP和端口与服务端建立连接
                        Socket socket =new Socket("127.0.0.1",8080);
                        //将字符流转化成字节流，并输出
                        PrintWriter printWriter = new PrintWriter(new OutputStreamWriter(socket.getOutputStream()),true);
                        String str="Hello，我是" + j + "个，客户端!";
                        printWriter.println(str);

                        //从输入流中读取服务端返回的信息，将字节流转化成字符流
                        BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                        //读取内容
                        String result = bufferedReader.readLine();
                        //打印服务端返回的信息
                        System.out.println("客户端发送请求内容：" + str + " -> 收到服务端返回的内容：" + result);

                        // 关闭流
                        bufferedReader.close();
                        printWriter.close();
                        // 关闭socket
                        socket.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }).start();
        }
    }
}
```
最后，依次启动服务端、客户端，看看控制台输出情况如何。<br />服务端控制台结果如下：
```
服务端收到客户端发送的信息：Hello，我是8个，客户端!
服务端收到客户端发送的信息：Hello，我是9个，客户端!
服务端收到客户端发送的信息：Hello，我是7个，客户端!
服务端收到客户端发送的信息：Hello，我是5个，客户端!
服务端收到客户端发送的信息：Hello，我是4个，客户端!
服务端收到客户端发送的信息：Hello，我是3个，客户端!
服务端收到客户端发送的信息：Hello，我是6个，客户端!
服务端收到客户端发送的信息：Hello，我是2个，客户端!
服务端收到客户端发送的信息：Hello，我是1个，客户端!
服务端收到客户端发送的信息：Hello，我是0个，客户端!
```
客户端控制台结果如下：
```
客户端发送请求内容：Hello，我是8个，客户端! -> 收到服务端返回的内容：hello，我是服务端，已收到消息
客户端发送请求内容：Hello，我是9个，客户端! -> 收到服务端返回的内容：hello，我是服务端，已收到消息
客户端发送请求内容：Hello，我是7个，客户端! -> 收到服务端返回的内容：hello，我是服务端，已收到消息
客户端发送请求内容：Hello，我是5个，客户端! -> 收到服务端返回的内容：hello，我是服务端，已收到消息
客户端发送请求内容：Hello，我是4个，客户端! -> 收到服务端返回的内容：hello，我是服务端，已收到消息
客户端发送请求内容：Hello，我是3个，客户端! -> 收到服务端返回的内容：hello，我是服务端，已收到消息
客户端发送请求内容：Hello，我是6个，客户端! -> 收到服务端返回的内容：hello，我是服务端，已收到消息
客户端发送请求内容：Hello，我是2个，客户端! -> 收到服务端返回的内容：hello，我是服务端，已收到消息
客户端发送请求内容：Hello，我是1个，客户端! -> 收到服务端返回的内容：hello，我是服务端，已收到消息
客户端发送请求内容：Hello，我是0个，客户端! -> 收到服务端返回的内容：hello，我是服务端，已收到消息
```
随着客户端的请求次数越来越多，可能需要排队的时间会越来越长，因此是否可以在服务端，采用多线程编程进行处理呢？<br />答案是，可以的！<br />下面对服务端的代码进行改造，**服务端多线程操作，样例程序如下：**
```java
public class BioServerTest {

    public static void main(String[] args) throws IOException {
        //初始化服务端socket并且绑定 8080 端口
        ServerSocket serverSocket = new ServerSocket(8080);
        //循环监听客户端请求
        while (true){
            //监听客户端请求
            Socket socket = serverSocket.accept();
            new Thread(new Runnable() {

                @Override
                public void run() {
                    try {
                        String threadName = Thread.currentThread().toString();
                        //将字节流转化成字符流，读取客户端输入的内容
                        BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                        //读取一行数据
                        String str = bufferedReader.readLine();
                        //打印客户端发送的信息
                        System.out.println("线程名称" + threadName + "，服务端收到客户端发送的信息：" + str);

                        //向客户端返回信息，将字符转化成字节流，并输出
                        PrintWriter printWriter = new PrintWriter(new OutputStreamWriter(socket.getOutputStream()),true);
                        printWriter.println("hello，我是服务端，已收到消息");

                        // 关闭流
                        bufferedReader.close();
                        printWriter.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }).start();
        }
    }
}
```
依次启动服务端、客户端，服务端控制台输出结果如下：
```
线程名称Thread[Thread-8,5,main]，服务端收到客户端发送的信息：Hello，我是4个，客户端!
线程名称Thread[Thread-4,5,main]，服务端收到客户端发送的信息：Hello，我是8个，客户端!
线程名称Thread[Thread-0,5,main]，服务端收到客户端发送的信息：Hello，我是1个，客户端!
线程名称Thread[Thread-7,5,main]，服务端收到客户端发送的信息：Hello，我是5个，客户端!
线程名称Thread[Thread-5,5,main]，服务端收到客户端发送的信息：Hello，我是2个，客户端!
线程名称Thread[Thread-9,5,main]，服务端收到客户端发送的信息：Hello，我是3个，客户端!
线程名称Thread[Thread-1,5,main]，服务端收到客户端发送的信息：Hello，我是0个，客户端!
线程名称Thread[Thread-3,5,main]，服务端收到客户端发送的信息：Hello，我是7个，客户端!
线程名称Thread[Thread-2,5,main]，服务端收到客户端发送的信息：Hello，我是9个，客户端!
线程名称Thread[Thread-6,5,main]，服务端收到客户端发送的信息：Hello，我是6个，客户端!
```
当服务端接收到客户端的请求时，会给每个客户端创建一个新的线程进行链路处理，处理完成之后，通过输出流返回应答给客户端，最后线程会销毁。<br />但是这样的编程模型也有很大的弊端，如果出现 100、1000、甚至 10000 个客户端同时请求服务端，采用这种编程模型，服务端也会创建与之相同的线程数量，**线程数急剧膨胀可能会导致线程堆栈溢出、创建新线程失败等问题，最终可能导致服务端宕机或者僵死，不能对外提供服务**。
<a name="EsEG9"></a>
## 三、伪异步 BIO
为了解决上面提到的同步阻塞 I/O 面临的一个链路需要一个线程处理的问题，后来有人对它的编程模型进行了优化。<br />在服务端通过使用 Java 中`ThreadPoolExecutor`线程池机制来处理多个客户端的请求接入，防止由于海量并发接入导致资源耗尽，让线程的创建和回收成本相对较低，保证了系统有限的资源得以控制，**实现了 N （客户端请求数量）大于 M （服务端处理客户端请求的线程数量）的伪异步 I/O 模型。**<br />伪异步 IO 模型图，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1689041833050-f506d30e-c034-4766-b74f-d498e41150af.jpeg)<br />采用线程池和任务队列可以实现一种叫做伪异步的 I/O 通信框架，当有新的客户端接入时，将客户端的 Socket 封装成一个 Task 投递到线程池中进行处理。<br />**服务端采用线程池处理客户端请求，样例程序如下：**
```java
public class BioServerTest {

    public static void main(String[] args) throws IOException {
        //在线程池中创建5个固定大小线程，来处理客户端的请求
        ExecutorService executorService = Executors.newFixedThreadPool(5);

        //初始化服务端socket并且绑定 8080 端口
        ServerSocket serverSocket = new ServerSocket(8080);
        //循环监听客户端请求
        while (true){
            //监听客户端请求
            Socket socket = serverSocket.accept();
            //使用线程池执行任务
            executorService.execute(new Runnable() {

                @Override
                public void run() {
                    try {
                        String threadName = Thread.currentThread().toString();
                        //将字节流转化成字符流，读取客户端输入的内容
                        BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                        //读取一行数据
                        String str = bufferedReader.readLine();
                        //打印客户端发送的信息
                        System.out.println("线程名称" + threadName + "，服务端收到客户端发送的信息：" + str);

                        //向客户端返回信息，将字符转化成字节流，并输出
                        PrintWriter printWriter = new PrintWriter(new OutputStreamWriter(socket.getOutputStream()),true);
                        printWriter.println("hello，我是服务端，已收到消息");

                        // 关闭流
                        bufferedReader.close();
                        printWriter.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            });
        }
    }
}
```
依次启动服务端、客户端，服务端控制台输出结果如下：
```
线程名称Thread[pool-1-thread-4,5,main]，服务端收到客户端发送的信息：Hello，我是6个，客户端!
线程名称Thread[pool-1-thread-2,5,main]，服务端收到客户端发送的信息：Hello，我是8个，客户端!
线程名称Thread[pool-1-thread-3,5,main]，服务端收到客户端发送的信息：Hello，我是9个，客户端!
线程名称Thread[pool-1-thread-5,5,main]，服务端收到客户端发送的信息：Hello，我是5个，客户端!
线程名称Thread[pool-1-thread-1,5,main]，服务端收到客户端发送的信息：Hello，我是7个，客户端!
线程名称Thread[pool-1-thread-5,5,main]，服务端收到客户端发送的信息：Hello，我是2个，客户端!
线程名称Thread[pool-1-thread-5,5,main]，服务端收到客户端发送的信息：Hello，我是0个，客户端!
线程名称Thread[pool-1-thread-1,5,main]，服务端收到客户端发送的信息：Hello，我是1个，客户端!
线程名称Thread[pool-1-thread-5,5,main]，服务端收到客户端发送的信息：Hello，我是3个，客户端!
线程名称Thread[pool-1-thread-1,5,main]，服务端收到客户端发送的信息：Hello，我是4个，客户端!
```
本例中测试的客户端数量是 10，服务端使用 java 线程池来处理任务，线程数量为 5 个，服务端不用为每个客户端都创建一个线程，由于线程池可以设置消息队列的大小和最大线程数，因此它的资源占用是可控的，无论多少个客户端并发访问，都不会导致资源的耗尽和宕机。<br />在活动连接数不是特别高的情况下，这种模型还是不错的，可以让每一个连接专注于自己的 I/O 并且编程模型简单，也不用过多考虑系统的过载、限流等问题。<br />但是，它的底层仍然是同步阻塞的 BIO 模型，当面对十万甚至百万级请求接入的时候，传统的 BIO 模型无能为力，因此需要一种更高效的 I/O 处理模型来应对更高的并发量。
<a name="kXG0P"></a>
## 四、NIO
NIO，英文全称：**Non-blocking-IO**，一种同步非阻塞的 I/O 模型。<br />在 Java 1.4 中引入，对应的代码在java.nio包下。<br />与传统的 IO 不同，NIO 新增了 **Channel、Selector、Buffer** 等抽象概念，**支持面向缓冲、基于通道的 I/O 数据传输方法**。<br />NIO 模型图，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1689040820433-64aa95b6-926d-461f-9c6b-e329580b603e.jpeg)<br />与此同时，NIO 还提供了与传统 BIO 模型中的 Socket 和 `ServerSocket` 相对应的 `SocketChannel` 和 `ServerSocketChannel` 两种不同的套接字通道实现。<br />NIO 这两种通道都支持**阻塞和非阻塞**两种模式。阻塞模式使用就像传统中的 BIO 一样，比较简单，但是性能和可靠性都不好；**非阻塞模式正好与之相反**。<br />对于低负载、低并发的应用程序，可以使用同步阻塞 I/O 来提升开发效率和更好的维护性；对于高负载、高并发的（**网络**）应用，使用 NIO 的非阻塞模式来开发可以显著的提升数据传输效率。<br />在介绍样例之前，先看一下 NIO 涉及到的核心关联类图，如下：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1689044487223-9fddc62a-f4f5-487b-ac97-c7dcd0cef7d0.jpeg)<br />上图中有三个关键类：**Channel 、Selector 和 Buffer**，它们是 NIO 中的核心概念。

- **Channel：可以理解为通道；**
- **Selector：可以理解为选择器；**
- **Buffer：可以理解为数据缓冲区；**

从名词上看感觉很抽象，还是用之前介绍的城市交通工具来继续形容 NIO 的工作方式，这里的 **Channel** 要比 **Socket** 更加具体，它可以比作为某种具体的交通工具，如汽车或是高铁、飞机等，而 **Selector** 可以比作为一个车站的车辆运行调度系统，它将负责监控每辆车的当前运行状态，是已经出站还是在路上等等，也就是说它可以轮询每个 **Channel** 的状态。<br />还有一个 **Buffer** 类，可以将它看作为 IO 中 **Stream**，但是它比 IO 中的 **Stream** 更加具体化，可以将它比作为车上的座位，**Channel** 如果是汽车的话，那么 **Buffer** 就是汽车上的座位，**Channel** 如果是高铁上，那么 **Buffer** 就是高铁上的座位，它始终是一个具体的概念，这一点与 **Stream** 不同。<br />**Socket 中的 Stream** 只能代表是一个座位，至于是什么座位由你自己去想象，也就是说在上车之前并不知道这个车上是否还有座位，也不知道上的是什么车，因为并不能选择，这些信息都已经被封装在了运输工具（**Socket**）里面了。<br />NIO 引入了 **Channel、Buffer 和 Selector** 就是想把 IO 传输过程中涉及到的**信息具体化**，让程序员有机会去控制它们。<br />当进行传统的网络 IO 操作时，比如调用`write()`往 Socket 中的SendQ队列写数据时，当一次写的数据超过SendQ长度时，操作系统会按照SendQ 的长度进行分割的，这个过程中需要将用户空间数据和内核地址空间进行切换，而这个切换不是程序员可以控制的，由底层操作系统来处理。<br />而在Buffer中，可以控制Buffer的`capacity`（容量），并且是否扩容以及如何扩容都可以控制。<br />理解了这些概念后看一下，实际上它们是如何工作的呢？<br />一起来看看代码实例！<br />**服务端操作，样例程序如下**：
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
**客户端操作，样例程序如下**：
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
**从编程上可以看到，NIO 的操作比传统的 IO 操作要复杂的多**！<br />**Selector** 被称为**选择器** ，当然也可以翻译为**多路复用器** 。它是Java NIO 核心组件中的一个，用于检查一个或多个 **Channel**（通道）的状态是否处于**连接就绪**、**接受就绪**、**可读就绪**、**可写就绪**。<br />如此可以实现单线程管理多个 **channels** 的目的，也就是可以管理多个网络连接。<br />**使用 Selector 的好处在于** ：相比传统方式使用多个线程来管理 IO，Selector 使用了更少的线程就可以处理通道了，并且实现网络高效传输！<br />虽然 Java 中的 nio 传输比较快，为什么大家都不愿意用 JDK 原生 NIO 进行开发呢？<br />从上面的代码中大家都可以看出来，除了编程复杂之外，还有几个让人诟病的问题：

- **JDK 的 NIO 底层由 epoll 实现，该实现饱受诟病的空轮询 bug 会导致 cpu 飙升 100%！**
- **项目庞大之后，自行实现的 NIO 很容易出现各类 bug，维护成本较高！**

**但是，Google 的 Netty 框架的出现，很大程度上改善了 JDK 原生 NIO 所存在的一些让人难以忍受的问题**，关于 Netty 框架应用，会在后期的文章里进行介绍。
<a name="bIp9D"></a>
## 五、AIO
最后就是 AIO 了，全称 Asynchronous I/O，可以理解为异步 IO，也被称为 NIO 2，在 Java 7 中引入，它是异步非阻塞的 IO 模型。<br />异步 IO 是**基于事件回调机制**实现的，也就是应用操作之后会直接返回，不会堵塞在那里，当后台处理完成，操作系统会通知相应的线程进行后续的操作。<br />具体的实例如下！<br />**服务端操作，样例程序如下**：
```java
/**
 * aio 服务端
 */
public class AioServer {

    public AsynchronousServerSocketChannel serverChannel;

    /**
     * 监听客户端请求
     * @throws Exception
     */
    public void listen() throws Exception {
        //打开一个服务端通道
        serverChannel = AsynchronousServerSocketChannel.open();
        serverChannel.bind(new InetSocketAddress(8080));//监听8080端口
        //服务监听
        serverChannel.accept(this, new CompletionHandler<AsynchronousSocketChannel,AioServer>(){

            @Override
            public void completed(AsynchronousSocketChannel client, AioServer attachment) {
                try {
                    if (client.isOpen()) {
                        System.out.println("接收到新的客户端连接，地址：" + client.getRemoteAddress());
                        final ByteBuffer buffer = ByteBuffer.allocate(1024);
                        //读取客户端发送的信息
                        client.read(buffer, client, new CompletionHandler<Integer, AsynchronousSocketChannel>(){

                            @Override
                            public void completed(Integer result, AsynchronousSocketChannel attachment) {
                                try {
                                    //读取请求，处理客户端发送的数据
                                    buffer.flip();
                                    String content = new String(buffer.array(), 0, buffer.limit());
                                    System.out.println("服务端收到客户端发送的信息：" + content);

                                    //向客户端发送数据
                                    ByteBuffer writeBuffer = ByteBuffer.allocate(1024);
                                    writeBuffer.put("server send".getBytes());
                                    writeBuffer.flip();
                                    attachment.write(writeBuffer).get();
                                } catch (Exception e) {
                                    e.printStackTrace();
                                }
                            }

                            @Override
                            public void failed(Throwable exc, AsynchronousSocketChannel attachment) {
                                try {
                                    exc.printStackTrace();
                                    attachment.close();
                                } catch (IOException e) {
                                    e.printStackTrace();
                                }
                            }
                        });
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                } finally {
                    //当有新客户端接入的时候，直接调用accept方法，递归执行下去，保证多个客户端都可以阻塞
                    attachment.serverChannel.accept(attachment, this);
                }
            }

            @Override
            public void failed(Throwable exc, AioServer attachment) {
                exc.printStackTrace();
            }
        });
    }

    public static void main(String[] args) throws Exception {
        //启动服务器，并监听客户端
        new AioServer().listen();
        //因为是异步IO执行，让主线程睡眠但不关闭
        Thread.sleep(Integer.MAX_VALUE);
    }
}
```
**客户端操作，样例程序如下**：
```java
/**
 * aio 客户端
 */
public class AioClient {

    public static void main(String[] args) throws IOException, InterruptedException {
        //打开一个客户端通道
        AsynchronousSocketChannel channel = AsynchronousSocketChannel.open();
        //与服务器建立连接
        channel.connect(new InetSocketAddress("127.0.0.1", 8080));

        //睡眠1s，等待与服务器建立连接
        Thread.sleep(1000);
        try {
            //向服务器发送数据
            channel.write(ByteBuffer.wrap("Hello，我是客户端".getBytes())).get();
        } catch (Exception e) {
            e.printStackTrace();
        }
        try {
            //从服务器读取数据
            ByteBuffer byteBuffer = ByteBuffer.allocate(1024);
            channel.read(byteBuffer).get();//将通道中的数据写入缓冲buffer
            byteBuffer.flip();
            String result = new String(byteBuffer.array(), 0, byteBuffer.limit());
            System.out.println("客户端收到服务器返回的内容：" + result);//输出返回结果
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```
同样的，依次启动服务端程序，再启动客户端程序，看看运行结果！<br />服务端控制台结果如下：
```
接收到新的客户端连接，地址：/127.0.0.1:56606
服务端收到客户端发送的信息：Hello，我是客户端
```
客户端控制台结果如下：
```
客户端收到服务器返回的内容：server send
```
这种组合方式用起来十分复杂，只有在一些非常复杂的分布式情况下使用，像集群之间的消息同步机制一般用这种 I/O 组合方式。如 Cassandra 的 Gossip 通信机制就是采用异步非阻塞的方式，可以实现非常高的网络传输性能。<br />Netty 之前也尝试使用过 AIO，不过又放弃了！
