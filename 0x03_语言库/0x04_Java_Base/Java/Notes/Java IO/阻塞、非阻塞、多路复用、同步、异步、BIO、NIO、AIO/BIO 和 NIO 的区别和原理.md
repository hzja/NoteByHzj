JavaBIONIO
<a name="Y9hoD"></a>
## BIO
BIO(Blocking IO) 又称同步阻塞IO，一个客户端由一个线程来进行处理<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1672643427218-27d562ed-14e8-439c-9a9c-0056cd134385.png#averageHue=%23f9f8f7&clientId=u35bcac4f-123c-4&from=paste&id=u87504259&originHeight=202&originWidth=322&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud04412b3-c4cc-4d99-9b35-bfac59abf35&title=)<br />当客户端建立连接后，服务端会开辟线程用来与客户端进行连接。以下两种情况会造成IO阻塞：

1. 服务端会一直阻塞，直到和客户端进行连接
2. 客户端也会一直阻塞，直到和服务端进行连接

基于BIO，当连接时，每有一个客户端，服务就开启线程处理，这样对资源的占用是非常大的；如果使用线城市来做优化，当大量连接时，服务端也会面临无空闲线程处理的问题。那么怎么设计才能让单个线程能够处理更多请求，而不是一个。所以NIO就被提出。
<a name="PmSCa"></a>
## NIO
NIO（Non Blocking IO）又称同步非阻塞IO。服务器实现模式为把多个连接(请求)放入集合中，只用一个线程可以处理多个请求（连接），也就是多路复用。<br />NIO有3大核心组件：

1. Buffer：缓冲区，buffer 底层就是数组
2. Channel：通道，channel 类似于流，每个 channel 对应一个 buffer缓冲区
3. Selector：多路复用器，channel 会注册到 selector 上，由 selector 根据 channel 读写事件的发生将其交由某个空闲的线程处理

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1672643427186-7ab57f3d-ae0a-4324-8bae-bb9be2bdcecd.png#averageHue=%23f7f5f4&clientId=u35bcac4f-123c-4&from=paste&id=u96008d79&originHeight=331&originWidth=241&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua89a28e3-3555-411b-a833-6de6ecb69a9&title=)<br />这样就大大提升了连接的数量，用于接收请求。<br />NIO目前有三个函数（模型）

- select
- poll
- epoll
<a name="QGcAz"></a>
### Select函数
Select 是Linux提供的一个函数，可以将一批fd一次性传递给内核，然后由内核去遍历，来确定哪个fd符合，并提供给用户空间<br />![Select 函数处理过程](https://cdn.nlark.com/yuque/0/2023/png/396745/1672643427207-0045b08f-4237-4827-b2e8-8e8bfe4d1609.png#averageHue=%23f3f3f2&clientId=u35bcac4f-123c-4&from=paste&id=kCUbp&originHeight=361&originWidth=828&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u89fcb4ee-3633-4116-b958-c74764a0167&title=Select%20%E5%87%BD%E6%95%B0%E5%A4%84%E7%90%86%E8%BF%87%E7%A8%8B "Select 函数处理过程")

1. 将用户空间的fd数组拷贝到内核空间
2. 内核空间会遍历fd数组，查看是否有数据到达
   1. 遍历**所有**fd，将当前进程挂到每个fd的等待队列中
   2. 当设备收到一条消息（网络设备）或填写完文件数据（磁盘设备）后，会唤醒设备等待队列上睡眠的进程，随后当前进程就会被唤醒
3. 遍历完成后，如果有数据到达，返回有数据到达的fd的数量，并对用户空间的fd做**标记**
4. 如果无数据到达，则当前进程进入睡眠，当有某个fd有I/O事件或当前进程睡眠超时后，当前进程重新唤醒再次遍历所有fd文件
5. 用户空间在此循环遍历，**没有标记**的 fd不处理，只有**标记**的fd才会去处理

Select存在的问题

1. fd**数量有限制**：单个进程所打开的fd是有限制的，通过 FD_SETSIZE 设置，默认1024
2. fd**拷贝耗时**：每次调用 select，需要将fd数组从**用户空间**拷贝到**内核空间**
3. **内核空间遍历耗时**：内核空间通过遍历的方式，查看fd是否有数据到达，这是一个同步的过程
4. **找到**fd**后，返回的是数量，而不是**fd**本身**：select返回的是fd的数量，具体是哪个还需要用户自己遍历
<a name="amN72"></a>
### Poll函数
Poll 也是Linux提供的内核函数，poll 和 select基本是一致，唯一的区别在于它们支持的fd的数量不一致

- select : 只能监听 1024 个fd
- poll ：无限制，操作系统支持多少，poll 就可以支持多少
<a name="mwrKf"></a>
### Epoll函数
poll解决了select函数的fd数量问题，而epoll解决了select、poll函数其余问题：

1. fd**数量有限制**：poll已经解决此问题
2. fd**拷贝耗时**：内核空间中保存一份fd数组，无需用户每次都重新传入，只需要告诉内核修改的部分即可
3. **内核空间遍历耗时**：内核空间不再通过遍历的方式找fd，而是通过**异步 IO 事件**唤醒
4. **找到**fd**后，返回的是数量，而不是**fd**本身**：内核空间会通过**异步 IO 事件**，将fd返回给用户，用户无需在遍历整个fd数组

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1672643427247-578db0b3-6d6c-492f-8da4-aa99be9607b0.png#averageHue=%23f4f3f2&clientId=u35bcac4f-123c-4&from=paste&id=ue3f8b5aa&originHeight=371&originWidth=401&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u71d836db-3017-42c0-8d05-5701737e918&title=)<br />因此，epoll提供3 个函数，来处理上述改进的方案：

1. `epoll_create`：创建 epoll 句柄
2. `epoll_ctl`：向内核空间添加，修改，删除需要监控的`fd`
3. `epoll_wait`、`epoll_pwait`：类似 `select`函数
