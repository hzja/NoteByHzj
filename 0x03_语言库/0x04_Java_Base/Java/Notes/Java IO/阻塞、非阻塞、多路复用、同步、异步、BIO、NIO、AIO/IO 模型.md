JavaIO<br />Java 常见 IO 模型有：

- 同步阻塞 BIO
- 同步非阻塞 NIO
- 异步非阻塞 AIO
<a name="d6qPb"></a>
## Socket 网络编程
网络编程中有一个重要的概念就是：Socket，简单了解一下。<br />在网络通信中，客户端和服务端通过一个双向的通信连接实现数据的交换，连接的任意一端都可称为一个 **Socket**。<br />Talk is cheap， show me the diagram，Socket 网络通信基本过程如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1652058089438-d62c7764-564f-4e60-a270-26cc79dcab3f.jpeg#clientId=u25f6166f-e653-4&from=paste&id=u74cd8321&originHeight=337&originWidth=741&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7e674df6-9f13-4b19-b62c-236be3438fe&title=)<br />总结一下流程，可以简单描述为这四步：

- （1）服务端启动，监听指定端口，等待客户端连接；
- （2）客户端尝试与服务端连接，建立可信数据传输通道；
- （3）客户端与服务端进行数据交换；
- （4）客户端或者服务端断开连接，终止通信；

了解了基本流程，有些小伙伴可能对 Socket 这玩意很感兴趣了，Socket 到底是什么东西呢？Socket 中文翻译过来就是**套接字**，是网络通信对象的抽象表达，听起来还是很模糊，从编码者视角来看，本质上就是一套编程接口，是对复杂的 TCP/IP 协议进行封装供上层应用使用，这样总明白了吧。<br />那 Socket 对象一般包括什么东西呢？一般包括五种信息：**连接使用的协议**、**本地主机的IP地址**、**本地进程的协议端口**、**远端主机的IP地址**、**远端进程的协议端口**。从这里可以看到 Socket 包含的信息非常丰富，也就是说拿到一个 Socket 对象就相当于知己知彼了。
<a name="U54Cg"></a>
## 传统 BIO 模式
上面从理论角度讲解了什么是Socket，现在回到开发语言实现层面上来，以 Java 为例，Java 语言从 1.0 版本就已经封装了 Socket 相关的接口供开发者使用，对这部分代码感兴趣的小伙伴可以出门向左拐，在java.net 包下面查看源码。<br />尝试用一个 demo 来演示一下传统的网络编程：<br />**服务端代码：**
```java
public static void main(String[] args) throws IOException {
	// 创建一个ServerSocket，监听端口8888
	ServerSocket ss = new ServerSocket(8888);

	// 循环方式监听客户端的请求
	while (true) {
		// 这里一直会阻塞，直到客户端连接上
		Socket socket = ss.accept();

		// 输入流用于接收消息
		InputStream inputStream = socket.getInputStream();
		BufferedInputStream bufferedInputStream = new BufferedInputStream(inputStream);

		// 输出流用于回复消息
		OutputStream outputStream = socket.getOutputStream();
		final PrintStream printStream = new PrintStream(outputStream);

		// 循环接收并回复客户端发送的消息
		byte[] bytes = new byte[1024];
		int len;
		while ((len = bufferedInputStream.read(bytes)) != -1) {
			printStream.print("服务端收到：" + new String(bytes, 0, len));
		}
	}
}
```
**效果演示：**<br />服务端运行起来后，使用 telnet 命令来模拟客户端发送消息:
```bash
telnet 127.0.0.1 8888
```
客户端每发送一条消息，服务端都会回复，演示效果如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652058089362-a5577e2a-95fb-420c-94b5-4809cedb9c59.png#clientId=u25f6166f-e653-4&from=paste&id=u27e98abc&originHeight=386&originWidth=868&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue8752e42-63df-4f05-8f4f-8e81c774f71&title=)<br />仔细想一下，上面的代码可能会有问题，如果前面一个客户端一直不断开，服务端就不能处理其他客户端的消息了，也就是说程序不具备并发的能力。<br />稍加改造一下，将前面的处理逻辑代码全部抽取到一个新的`handle()`方法， 每当有客户端连接上就新开一个线程处理：
```java
public static void main(String[] args) throws IOException {
	// 创建一个ServerSocket，监听端口8888
	ServerSocket ss = new ServerSocket(8888);
	
	// 循环方式监听客户端的请求
	while (true) {
		// 这里一直会阻塞，直到客户端连接上
		Socket socket = ss.accept();
		// 启动一个新的线程处理
		new Thread(() -> handle(socket)).start();
	}
}
```
这里为了演示方便直接新起了一个线程，当然更好的办法是用线程池，但是也解决不了根本性问题。<br />看了两段代码，先简单总结一下 BIO 模式的劣势：

- 如果 BIO 使用单线程接收连接，则会阻塞其他连接，效率较低。
- 如果使用多线程，虽然减弱了单线程带来的影响，但当有大并发进来时，会导致服务器线程太多，压力太大而崩溃。
- 就算使用线程池，也只能同时允许有限个数的线程进行连接，如果并发量远大于线程池设置的数量，还是与单线程无异。
- IO 代码里 read 操作是阻塞操作，如果连接不做数据读写操作会导致线程阻塞，就是说只占用连接，不发送数据，则会浪费资源。比如线程池中 500个连接，只有 100 个是频繁读写的连接，其他占着茅坑不拉屎，浪费资源！
- 另外多线程也会有线程切换带来的消耗。

综上所述，BIO 模式不能满足大并发业务场景，仅适用于连接数目比较小且固定的架构。
<a name="b62l5"></a>
### 同步阻塞 BIO 模式
根据上面的例子再画图抽象一下 BIO 网络编程场景：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1652058089600-688e63c4-663d-4287-83f5-f4accd00f982.jpeg#clientId=u25f6166f-e653-4&from=paste&id=u7e900f44&originHeight=541&originWidth=501&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua249d568-6bc8-481f-8127-693c4d94a7c&title=)<br />传统 BIO 的特点是只要来了一个新客户端连接，服务端就会开辟一个线程处理客户端请求，但是客户端连接后并不是一直都对服务端进行 IO 操作，这样会导致服务端阻塞，一直占用着线程资源，造成很多非要的开销。<br />为了解决这个问题，Java 引入了 NIO，接着往下看。
<a name="egPym"></a>
## NIO
在 Java 1.4 版本之前 BIO 是开发者唯一的选择，1.4 版本开始引入了 NIO 框架。<br />NIO 的 N 有两层含义，一层是：New IO，另一层是 Non Blocking IO。<br />「New」是相对于传统 BIO 来说的，在当时确实挺新的；Non Blocking IO 又被称为：**同步非阻塞 IO**，同步非阻塞体现在：

- **同步**：调用的结果会在本次调用后返回，不存在异步线程回调之类的。
- **非阻塞**：表现为线程不会一直在等待，把连接加入集合后，线程会一直轮询集合中的连接，有则处理，无则继续接受请求。
<a name="JyyhU"></a>
### NIO 三大基础组件
学习 NIO必须得知道下面这三个基础组件：
<a name="OUxTg"></a>
#### （1）Buffer(缓冲区)
IO 是面向流（字节流或者字符流）的，而 NIO 是面向**块**的，**块**指的是 Buffer 缓冲区。面向块的方式一次性可以获取或者写入一整块数据，而不需要一个字节一个字节的从流中读取，这样处理数据的速度会比流方式更快。<br />Buffer 缓冲区的底层实现是数组，根据数组类型可以细分为：ByteBuffe、CharBuffer、DoubleBuffer、FloatBuffer、IntBuffer、LongBuffer、ShortBuffer等。
<a name="oanNU"></a>
#### （2）Channel(通道)
Channel 翻译成中文是**通道**的意思，作用类似于 IO 中的 Stream 流。但是 Channel 和 Stream 不同之处在于 Channel 是双向的，Stream 只是在一个方向移动，而且 Channel 可以用于读、写或者同时用于读写。<br />常见 Channel 通道类型：

- FileChannel 用于文件操作场景；
- ServerSocketChannel 和 SocketChannel 主要用于 TCP 网络通信 IO，这是本文的重点；
- DatagramChannel: 从 UDP 网络中读取或者写入数据。

**Channel 与 Buffer 之间的关系：**<br />每个 Channel 对应一个 Buffer 缓冲区，永远无法将数据直接写入到Channel或者从Channel中读取数据。需要通过Buffer与Channel交互。
<a name="C6uXO"></a>
#### （3）Selector(多路复用器)
NIO 服务端的实现模式是把多个连接(请求)放入集合中，只用一个线程可以处理多个请求(连接)，也就是**多路复用**，Linux 环境下多路复用底层主要用的是内核函数（select，poll）来实现的，为了提升效率，Java 1.5 版本开始使用 epoll。<br />关于 select、poll、epoll 之间的对比，感兴趣的小伙伴可以自行上网查询。<br />在 NIO 中多路复用器称之为：**Selector**，Channel 会注册到 Selector 上，由 Selector 根据 Channel 读写事件的发生将其交由某个空闲的线程处理。<br />Buffer、Channel、Selector 这三个组件的之间的关系可以用下面的图来描述：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1652058089482-623f6a58-d664-4662-ae1a-1534840ff7ce.jpeg#clientId=u25f6166f-e653-4&from=paste&id=u39bf551a&originHeight=691&originWidth=781&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u496ba20c-99cf-4b0d-baa0-541951d6d9c&title=)<br />基本的工作流程如下：<br />（1）首先将 Channel 注册到 Selector 中；<br />（2）初始化 Selector，调用 `select()` 方法，select 方法会阻塞直到感兴趣的事件来临；<br />（3）当某个 Channel 有连接或者读写事件时，该 Channel 就会处于就绪状态；<br />（4）Selector 开始轮询所有处于就绪状态的SelectionKey，通过 SelectionKey 可以获取对应的Channel 集合；
<a name="R7f7x"></a>
### NIO 比 BIO 好用在哪？
NIO 相对于 BIO 最大的改进就是使用了多路复用技术，用少量线程处理大量客户端 IO 请求，提高了并发量并减少了资源消耗；<br />另外NIO 的操作时非阻塞的，比如说，单线程中从通道读取数据到buffer，同时可以继续做别的事情，当数据读取到buffer中后，线程再继续处理数据。写数据也是一样的。
<a name="hA7in"></a>
### NIO 存在的问题
NIO这么牛了，是不是就是终极解决方案了？其实也不是，NIO 也存在很多问题。<br />来看看 NIO 有哪些问题？<br />（1）NIO 的 API 使用起来非常麻烦，门槛比较高，开发者需要熟练掌握：Selector、ServerSocketChannel、SocketChannel、ByteBuffer 等类。<br />（2）NIO 编程涉及到 Reactor 模式，开发者需要对多线程和网络编程非常熟悉才能写出高质量的 NIO 程序；<br />（3）异常场景处理麻烦，比如：客户端断连重连、网络闪断、拆包粘包、网络拥塞等等；<br />（4）NIO 有 bug，不稳定，比如：臭名昭著的 Epoll bug，会导致 Selector 空轮询，最终导致 CPU 100%。
