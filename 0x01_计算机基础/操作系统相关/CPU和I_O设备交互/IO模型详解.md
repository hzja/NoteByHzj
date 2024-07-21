操作系统 I/O
<a name="Xjl1i"></a>
## 什么是IO呢？
IO，英文全称是Input/Output，翻译过来就是**输入/输出**。平时听得挺多，就是什么磁盘IO，网络IO。那IO到底是什么呢？是不是有种懵懵懂懂的感觉呀，好像大概知道它是什么，又好像说不清楚。<br />**IO，即输入/输出**，到底谁是输入？谁是输出呢？IO如果脱离了主体，就会让人疑惑。
<a name="sUdyN"></a>
### 计算机角度的IO
常说的输入输出，比较直观的意思就是**计算机的输入输出**，**计算机就是主体**。大家是否还记得，大学学**计算机组成原理**的时候，有个**冯.诺依曼结构**，它将计算机分成分为5个部分：运算器、控制器、存储器、输入设备、输出设备。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638318840431-c579ec32-b661-4500-a9b9-2806b02f7356.webp#clientId=u8a228e4c-f751-4&from=paste&id=ua72dd797&originHeight=565&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u90e4f4c3-8019-4faa-84a3-625ddf3e28b&title=)<br />输入设备是向计算机输入数据和信息的设备，键盘，鼠标都属于输入设备；输出设备是计算机硬件系统的终端设备，用于接收计算机数据的输出显示，一般显示器、打印机属于输出设备。<br />例如在鼠标键盘敲几下，它就会把指令数据，传给主机，主机通过运算后，把返回的数据信息，输出到显示器。<br />鼠标、显示器这只是直观表面的输入输出，回到计算机架构来说，**涉及计算机核心与其他设备间数据迁移的过程，就是IO**。如磁盘IO，就是从磁盘读取数据到内存，这算一次输入，对应的，将内存中的数据写入磁盘，就算输出。这就是IO的本质。
<a name="vmwlK"></a>
### 操作系统的IO
要将内存中的数据写入到磁盘的话，主体会是什么呢？主体可能是一个应用程序，比如一个Java进程（假设网络传来二进制流，一个Java进程可以把它写入到磁盘）。<br />**操作系统**负责计算机的资源管理和进程的调度。电脑上跑着的应用程序，其实是需要经过**操作系统**，才能做一些特殊操作，如**磁盘文件读写、内存的读写**等等。因为这些都是比较危险的操作，不可以由应用程序乱来，只能交给底层操作系统来。也就是说，应用程序要把数据写入磁盘，只能通过调用操作系统开放出来的API来操作。

- 什么是用户空间？什么是内核空间?
- 以32位操作系统为例，它为每一个进程都分配了4G(2的32次方)的内存空间。这4G可访问的内存空间分为二部分，一部分是用户空间，一部分是内核空间。内核空间是操作系统内核访问的区域，是受保护的内存空间，而用户空间是用户应用程序访问的内存区域。

应用程序是跑在用户空间的，它不存在实质的IO过程，真正的IO是在**操作系统**执行的。即应用程序的IO操作分为两种动作：**IO调用和IO执行**。IO调用是由进程（应用程序的运行态）发起，而IO执行是**操作系统内核**的工作。此时所说的IO是应用程序对操作系统IO功能的一次触发，即IO调用。
<a name="zxbbg"></a>
## 操作系统的一次IO过程
应用程序发起的一次IO操作包含两个阶段：

- IO调用：应用程序进程向操作系统**内核**发起调用。
- IO执行：操作系统内核完成IO操作。

操作系统内核完成IO操作还包括两个过程：

- 准备数据阶段：内核等待I/O设备准备好数据
- 拷贝数据阶段：将数据从内核缓冲区拷贝到用户进程缓冲区

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638318840138-8bda1b7a-6dab-4898-a652-1f4072473cba.webp#clientId=u8a228e4c-f751-4&from=paste&id=u6fdaa664&originHeight=553&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5767297c-75be-4001-a74d-79d8d143351&title=)<br />其实IO就是把进程的内部数据转移到外部设备，或者把外部设备的数据迁移到进程内部。外部设备一般指硬盘、socket通讯的网卡。一个完整的**IO过程**包括以下几个步骤：

- 应用程序进程向操作系统发起**IO调用请求**
- 操作系统**准备数据**，把IO外部设备的数据，加载到**内核缓冲区**
- 操作系统拷贝数据，即将内核缓冲区的数据，拷贝到用户进程缓冲区
<a name="qNmGk"></a>
## 阻塞IO模型
已经知道IO是什么啦，那什么是**阻塞IO**呢？<br />假设应用程序的进程发起**IO调用**，但是如果**内核的数据还没准备好**的话，那应用程序进程就一直在**阻塞等待**，一直等到内核数据准备好了，从内核拷贝到用户空间，才返回成功提示，此次IO操作，称之为**阻塞IO**。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638318840288-162396ac-3d8a-430c-bd76-4ebf43a8ac15.webp#clientId=u8a228e4c-f751-4&from=paste&id=ud69877f7&originHeight=565&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=udf4bb05f-c947-4783-a3a3-658d8d5860a&title=)

- 阻塞IO比较经典的应用就是**阻塞socket、Java BIO**。
- 阻塞IO的缺点就是：如果内核数据一直没准备好，那用户进程将一直阻塞，**浪费性能**，可以使用**非阻塞IO**优化。
<a name="UPA36"></a>
## 非阻塞IO模型
如果内核数据还没准备好，可以先返回错误信息给用户进程，让它不需要等待，而是通过轮询的方式再来请求。这就是非阻塞IO，流程图如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638318840215-3a8e8711-9035-43e2-b879-cd30a387ece4.webp#clientId=u8a228e4c-f751-4&from=paste&id=u7ef251d4&originHeight=764&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0ad755e5-1b9e-48b9-bf0f-450e8a858f9&title=)<br />非阻塞IO的流程如下：

- 应用进程向操作系统内核，发起`recvfrom`读取数据。
- 操作系统内核数据没有准备好，立即返回EWOULDBLOCK错误码。
- 应用程序进程轮询调用，继续向操作系统内核发起`recvfrom`读取数据。
- 操作系统内核数据准备好了，从内核缓冲区拷贝到用户空间。
- 完成调用，返回成功提示。

非阻塞IO模型，简称**NIO**，Non-Blocking IO。它相对于阻塞IO，虽然大幅提升了性能，但是它依然存在**性能问题**，即**频繁的轮询**，导致频繁的系统调用，同样会消耗大量的CPU资源。可以考虑**IO复用模型**，去解决这个问题。
<a name="nyESB"></a>
## IO多路复用模型
既然**NIO**无效的轮询会导致CPU资源消耗，等到内核数据准备好了，主动通知应用进程再去进行系统调用，那不就好了嘛？<br />在这之前，先来复习下，什么是**文件描述符fd**(File Descriptor),它是计算机科学中的一个术语，形式上是一个非负整数。当程序打开一个现有文件或者创建一个新文件时，内核向进程返回一个文件描述符。<br />IO复用模型核心思路：系统提供**一类函数**（如耳濡目染的**select、poll、epoll**函数），它们可以同时监控多个fd的操作，任何一个返回内核数据就绪，应用进程再发起`recvfrom`系统调用。
<a name="neW8n"></a>
### IO多路复用之`select`
应用进程通过调用`**select**`函数，可以同时监控多个fd，在`select`函数监控的fd中，只要有任何一个数据状态准备就绪了，`select`函数就会返回可读状态，这时应用进程再发起`recvfrom`请求去读取数据。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638318840319-c0da499c-5778-4fa3-a60d-b2c2e78df286.webp#clientId=u8a228e4c-f751-4&from=paste&id=u07ad2189&originHeight=660&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u45184da0-e6e9-463f-b8ef-decd84c16a9&title=)<br />非阻塞IO模型（NIO）中，需要N（N>=1）次轮询系统调用，然而借助`select`的IO多路复用模型，只需要发起一次询问就够了，大大优化了性能。<br />但是呢，`select`有几个缺点：

- 监听的IO最大连接数有限，在Linux系统上一般为1024。
- select函数返回后，是通过**遍历**fdset，找到就绪的描述符fd。（仅知道有I/O事件发生，却不知是哪几个流，所以**遍历所有流**）

因为**存在连接数限制**，所以后来又提出了**poll**。与select相比，**poll**解决了**连接数限制问题**。但是呢，select和poll一样，还是需要通过遍历文件描述符来获取已经就绪的`socket`。如果同时连接的大量客户端，在一时刻可能只有极少处于就绪状态，伴随着监视的描述符数量的增长，**效率也会线性下降**。<br />因此经典的多路复用模型`epoll`诞生。
<a name="bhabi"></a>
### IO多路复用之`epoll`
为了解决`select`/`poll`存在的问题，多路复用模型`epoll`诞生，它采用事件驱动来实现，流程图如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638318840649-10add3ee-eed5-437b-ba9a-a007f401a4a0.webp#clientId=u8a228e4c-f751-4&from=paste&id=u2875143b&originHeight=656&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3ce243d5-239d-4532-84aa-838e86ac27a&title=)<br />**epoll**先通过`epoll_ctl()`来注册一个fd（文件描述符），一旦基于某个fd就绪时，内核会采用回调机制，迅速激活这个fd，当进程调用`epoll_wait()`时便得到通知。这里去掉了**遍历文件描述符**的坑爹操作，而是采用**监听事件回调**的机制。这就是epoll的亮点。<br />**总结一下select、poll、epoll的区别**

| 

 | select | poll | epoll |
| --- | --- | --- | --- |
| 底层数据结构 | 数组 | 链表 | 红黑树和双链表 |
| 获取就绪的fd | 遍历 | 遍历 | 事件回调 |
| 事件复杂度 | O(n) | O(n) | O(1) |
| 最大连接数 | 1024 | 无限制 | 无限制 |
| fd数据拷贝 | 每次调用select，需要将fd数据从用户空间拷贝到内核空间 | 每次调用poll，需要将fd数据从用户空间拷贝到内核空间 | 使用内存映射(mmap)，不需要从用户空间频繁拷贝fd数据到内核空间 |

**epoll**明显优化了IO的执行效率，但在进程调用`epoll_wait()`时，仍然可能被阻塞。能不能酱紫：不用老是去问数据是否准备就绪，等我发出请求后，数据准备好了通知我就行了，这就诞生了**信号驱动IO模型**。
<a name="ApoT0"></a>
## IO模型之信号驱动模型
信号驱动IO不再用主动询问的方式去确认数据是否就绪，而是向内核发送一个信号（调用`sigaction`的时候建立一个`SIGIO`的信号），然后应用用户进程可以去做别的事，不用阻塞。当内核数据准备好后，再通过`SIGIO`信号通知应用进程，数据准备好后的可读状态。应用用户进程收到信号之后，立即调用`recvfrom`，去读取数据。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638318840994-961f2ad4-d6b7-4a1c-b148-ede1beb0a971.webp#clientId=u8a228e4c-f751-4&from=paste&id=u5aba949e&originHeight=662&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u94665427-16ff-482b-8d05-bfd500d7285&title=)<br />信号驱动IO模型，在应用进程发出信号后，是立即返回的，不会阻塞进程。它已经有异步操作的感觉了。但是细看上面的流程图，**发现数据复制到应用缓冲的时候**，应用进程还是阻塞的。回过头来看下，不管是BIO，还是NIO，还是信号驱动，在数据从内核复制到应用缓冲的时候，都是阻塞的。还有没有优化方案呢？**AIO**（真正的异步IO）！
<a name="jEDg9"></a>
## IO 模型之异步IO(AIO)
前面讲的BIO，NIO和信号驱动，在数据从内核复制到应用缓冲的时候，都是**阻塞**的，因此都不算是真正的异步。AIO实现了IO全流程的非阻塞，就是应用进程发出系统调用后，是立即返回的，但是**立即返回的不是处理结果，而是表示提交成功类似的意思**。等内核数据准备好，将数据拷贝到用户进程缓冲区，发送信号通知用户进程IO操作执行完毕。<br />流程如下：![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638318840857-e75dd67e-7219-4c01-975c-a99156176d44.webp#clientId=u8a228e4c-f751-4&from=paste&id=u9ba40825&originHeight=676&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud865695e-2121-46ba-9324-f42321e496c&title=)<br />异步IO的优化思路很简单，只需要向内核发送一次请求，就可以完成数据状态询问和数据拷贝的所有操作，并且不用阻塞等待结果。日常开发中，有类似思想的业务场景：<br />比如发起一笔批量转账，但是批量转账处理比较耗时，这时候后端可以先告知前端转账提交成功，等到结果处理完，再通知前端结果即可。
<a name="xwqlE"></a>
## 阻塞、非阻塞、同步、异步IO划分
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638318840966-d6cd5727-0c4d-439e-b748-4c344fe150c8.webp#clientId=u8a228e4c-f751-4&from=paste&id=uf8622dfd&originHeight=403&originWidth=810&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9616ba06-68d9-4056-8ad9-56d4a39bc83&title=)

| IO模型 | <br /> |
| --- | --- |
| 阻塞I/O模型 | 同步阻塞 |
| 非阻塞I/O模型 | 同步非阻塞 |
| I/O多路复用模型 | 同步阻塞 |
| 信号驱动I/O模型 | 同步非阻塞 |
| 异步IO（AIO）模型 | 异步非阻塞 |

<a name="amGjS"></a>
## 一个通俗例子读懂BIO、NIO、AIO

- 同步阻塞(blocking-IO)简称BIO
- 同步非阻塞(non-blocking-IO)简称NIO
- 异步非阻塞(asynchronous-non-blocking-IO)简称AIO
