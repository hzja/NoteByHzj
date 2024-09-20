Java<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653281081776-1d56d0cc-5b2b-4364-9457-4856ccac66d0.png#clientId=u2ab6d121-d339-4&from=paste&id=u5c64cf05&originHeight=234&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u241cc6fb-47dc-4da5-8865-d8daa809800&title=)
<a name="Nd7Jd"></a>
## 阻塞模式
先看服务端方法：
```java
public static void main(String[] args) throws Exception {
	ByteBuffer buffer = ByteBuffer.allocate(100);
	ServerSocketChannel serverSocket = ServerSocketChannel.open()
		serverSocket.bind(new InetSocketAddress(8080));
	List<SocketChannel> channels = new ArrayList<>();
	while(true) {
		SocketChannel sc = serverSocket.accept();
		channels.add(sc);

		for (SocketChannel asc :channels) {
			asc.read(buffer);
			buffer.flip();
			ByteBufferUtil.debugAll(buffer);
			buffer.clear();
		}
	}
}
```

- 首先新建一个 ServerSocketChannel 类，同时绑定 8080 端口
- 然后 `while(true)`循环调用 `accept()`来建立连接，同时将该 SocketChannel 加入到 List 集合中，该集合用来装所有与服务端建立连接的 SocketChannel
- 最后接受客户端发送来的数据，打印出来

现在来运行下（这里就不写客户端程序了，就用 MAC 的 iTerm 来模拟即可）。需要开 2 个客户端。<br />先打开 client-01，然后发送一条 “hi,i am client-01”<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653281081829-943002c1-d581-46d7-b478-9d800828d820.png#clientId=u2ab6d121-d339-4&from=paste&id=u70b45ccf&originHeight=174&originWidth=670&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uabc7eb95-c9ec-4538-9dc7-8e651ba012f&title=)<br />服务器运行结果：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653281081776-7d219bcc-2bef-490e-8286-ade0cd1fb438.png#clientId=u2ab6d121-d339-4&from=paste&id=uac687e40&originHeight=296&originWidth=870&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u92aa7bea-b5c8-417e-84ca-343fe81f661&title=)<br />服务端准确无误打印出 client-01 发送过来的消息（hi,i am client-01）。这个时候再发一条消息：“hi,i am client-11”，会惊奇地发现，服务端竟然不输出客户端发来的消息。这个时候再启动 client-02，神奇的事情发生了，服务端把 client-01 发送的消息（hi,i am client-11）给打印出来了：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653281081794-e01f08d9-38e4-418f-be5d-28e6b6d5a9ff.png#clientId=u2ab6d121-d339-4&from=paste&id=u4a068aad&originHeight=479&originWidth=623&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ucb33fff9-93b3-4dd4-ad5f-189ac257ef3&title=)<br />为什么会出现这种神奇的现象？主要原因就是该 **ServerSocketChannel 是阻塞模式，相关方法都会导致线程的阻塞**，当 client-01 建立连接，第一次发送消息时，服务端正常打印消息（hi,i am client-01），这时服务端又运行到 `accept()`，注意这个方法是阻塞方法，如果没有客户端来建立连接，它会一直阻塞在这里，哪怕 client-01 再次发送消息（hi,i am client-11），服务端也不会打印。这时 client-02 与服务端建立连接，服务端就不会阻塞，打印 client-01 第二次发来的消息（hi,i am client-11）。<br />所以，阻塞模式存在如下缺陷

- 单线程情况下，阻塞方法都会导致线程暂停
   - `ServerSocketChannel.accept()` 会在没有连接建立时让线程暂停，即使有客户端向服务端发送消息，服务单也接收不到直到有新客户端连接服务端，不再阻塞在`accept()`方法上。
   - `SocketChannel.read()` 会在通道中没有数据可读时让线程暂停，即使之后有新客户端向服务端发起连接请求也接受不了，直到读取完毕，不再阻塞在`read()`方法上

所以在单线程情况，服务端几乎不可能正常工作。那多线程呢？多线程情况下，如果连接数过多，必然会导致 OOM，然后线程的上下文切换也会导致性能低下。
<a name="otyCW"></a>
## 非阻塞模式
上面的阻塞模式几乎导致整个服务端是可能使用的，是可以使用非阻塞模式来避免的。如下
```java
public static void main(String[] args) throws Exception {
	ByteBuffer buffer = ByteBuffer.allocate(100);
	ServerSocketChannel serverSocket = ServerSocketChannel.open();

	serverSocket.bind(new InetSocketAddress(8080));
	List<SocketChannel> channels = new ArrayList<>();
	while(true) {
		// 非阻塞模式
		serverSocket.configureBlocking(false);

		SocketChannel sc = serverSocket.accept();
		if (sc != null){
			channels.add(sc);
		}

		for (SocketChannel asc :channels) {
			asc.configureBlocking(false);

			int size = asc.read(buffer);
			if (size > 0) {
				buffer.flip();
				ByteBufferUtil.debugAll(buffer);
				buffer.clear();
			}
		}
	}
}
```

- 通过 `ServerSocketChannel.configureBlocking(false)` 将 serverSocket 设置为非阻塞模式，这样 serverSocket 在调用 `accept()`方法时就不会阻塞了，如果没有连接，则会返回 null
- 通过 `SocketChannel..configureBlocking(false)` 将 asc 设置为非阻塞模式，这 asc 在调用 `read()` 方法就不会阻塞了，如果没有可读数据，它则会返回 -1。

非阻塞模式虽然不会影响业务的使用，但由于在 `while(true)` 循环里面，CPU 会一直处理运行状态，占用和浪费 CPU 资源。<br />所以，采用这种 `while(true)` 循环的暴力方式根本就不适合业务使用，对于 SocketChannel 而言，希望他只担任一个通道，传传数据的角色即可，不需再有额外的角色了，故而不能放任他们，需要对其进行统一管理，既要有管理器，有连接来了，就告诉你该建立连接了，有要读的数据，我就告诉你可以读数据了，这样 SocketChannel 是不是就很爽了。在 NIO 中，这个管理器称之为 Selector。
