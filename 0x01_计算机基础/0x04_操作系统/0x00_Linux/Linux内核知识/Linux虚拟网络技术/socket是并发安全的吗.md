Linux<br />先假设一个场景。想象中的游戏架构是下面这样的。<br />![想象中的游戏架构](https://cdn.nlark.com/yuque/0/2023/png/396745/1678417030569-64fc3986-aeb1-4f22-aeaa-21a0f6fcff7e.png#averageHue=%23e2e9ec&clientId=uc04b4c8e-4886-4&from=paste&id=u4510f128&originHeight=540&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ue326271f-50ef-45bc-a3bb-6ec3d53cf50&title=%E6%83%B3%E8%B1%A1%E4%B8%AD%E7%9A%84%E6%B8%B8%E6%88%8F%E6%9E%B6%E6%9E%84 "想象中的游戏架构")<br />也就是用户客户端直接连接游戏核心逻辑服务器，下面简称**GameServer**。GameServer主要负责实现各种玩法逻辑。<br />这当然是能跑起来，实现也很简单。<br />但这样会有个问题，因为游戏这块蛋糕很大，所以总会遇到很多**挺刑**的事情。<br />如果让用户直连GameServer，那相当于把**GameServer的ip**暴露给了所有人。<br />不赚钱还好，一旦游戏赚钱，就会遇到各种攻击。<br />《羊了个羊》最火的时候为啥老是崩溃？<br />假设一个游戏服务器能承载4k玩家，一旦服务器遭受直接攻击，那4k玩家都会被影响。<br />这攻击的是服务器吗？这明明攻击的是老板的钱包。<br />所以很多时候不会让用户直连GameServer。<br />而是在前面加入一层**网关层，下面简称gateway**。类似这样。<br />![实际的某些游戏架构](https://cdn.nlark.com/yuque/0/2023/png/396745/1678417030468-5ae23275-93c5-49ca-bcea-0dd40f72dbe6.png#averageHue=%23e4ebed&clientId=uc04b4c8e-4886-4&from=paste&id=ue50e0a07&originHeight=594&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ubc43340f-3c73-4d59-bdae-6ae438d1c8e&title=%E5%AE%9E%E9%99%85%E7%9A%84%E6%9F%90%E4%BA%9B%E6%B8%B8%E6%88%8F%E6%9E%B6%E6%9E%84 "实际的某些游戏架构")<br />GameServer就躲在了gateway背后，用户只能得到gateway的IP。<br />然后将大概每100个用户放在一个gateway里，这样如果真被攻击，就算gateway崩了，受影响的也就那100个玩家。<br />由于大部分游戏都使用TCP做开发，所以下面提到的**连接**，如果没有特别说明，那都是指**TCP连接**。<br />那么问题来了。<br />假设有100个用户连gateway，那gateway跟GameServer之间也会是 100个连接吗？<br />当然不会，gateway跟GameServer之间的连接数会**远小于100**。<br />因为这100个用户不会一直需要收发消息，总有**空闲**的时候，完全可以让多个用户复用同一条连接，将数据打包一起发送给GameServer，这样单个连接的利用率也高了，GameServer 也不再需要同时维持太多连接，可以节省了不少资源，这样就可以多服务几个~~大怨种~~金主。<br />要对网络连接写数据，就要执行 `send(socket_fd, data)`。<br />于是问题就来了。<br />已知**多个用户共用同一条连接**。<br />现在多个用户要发数据，也就是多个用户线程需要写**同一个socket_fd**。<br />那么，**socket是并发安全的吗？能让这多个线程同时并发写吗？**<br />![并发读写socket](https://cdn.nlark.com/yuque/0/2023/png/396745/1678417030575-903f835e-987d-4084-a25c-3b43547512e5.png#averageHue=%23eceff1&clientId=uc04b4c8e-4886-4&from=paste&id=ubd53f46e&originHeight=1170&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u89e2472c-2a86-4ce9-8977-f0ac7e8fa2c&title=%E5%B9%B6%E5%8F%91%E8%AF%BB%E5%86%99socket "并发读写socket")
<a name="PyunR"></a>
## 写TCP Socket是线程安全的吗？
对于TCP，一般使用下面的方式创建socket。
```c
sockfd=socket(AF_INET,SOCK_STREAM, 0))
```
返回的sockfd是socket的句柄id，用于在整个操作系统中**唯一标识**你的socket是哪个，可以理解为socket的**身份证id**。<br />创建socket时，操作系统内核会顺带为socket创建一个**发送缓冲区**和一个**接收缓冲区**。分别用于在发送和接收数据的时候给**暂存一下数据**。<br />写socket的方式有很多，既可以是send，也可以是write。<br />但不管哪个，最后在内核里都会走到 `tcp_sendmsg()` 函数下。
```c
// net/ipv4/tcp.c
int tcp_sendmsg(struct kiocb *iocb, struct sock *sk, struct msghdr *msg, size_t size)
{
    // 加锁
    lock_sock(sk);


    // ... 拷贝到发送缓冲区的相关操作


    // 解锁
    release_sock(sk);
}
```
在`tcp_sendmsg`的目的就是将要发送的数据放入到TCP的发送缓冲区中，此时并没有所谓的发送数据出去，函数就返回了，内核后续再根据实际情况异步发送。<br />![tcp_sendmsg 逻辑](https://cdn.nlark.com/yuque/0/2023/png/396745/1678417030455-6ba9a9d3-dc30-4a51-a36b-c56c2cf20a89.png#averageHue=%2388aea7&clientId=uc04b4c8e-4886-4&from=paste&id=u777556c0&originHeight=990&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ue786754a-a05c-4a74-bab3-ff806125be9&title=tcp_sendmsg%20%E9%80%BB%E8%BE%91 "tcp_sendmsg 逻辑")<br />从tcp_sendmsg的代码中可以看到，在对socket的缓冲区执行写操作的时候，linux内核已经自动**加好了锁**，也就是说，是**线程安全的**。<br />所以可以多线程不加锁并发写入数据吗？<br />**不能。**<br />问题的关键在于**锁的粒度**。<br />TCP有三大特点，**面向连接，可靠的，基于字节流**的协议。<br />![TCP是什么](https://cdn.nlark.com/yuque/0/2023/png/396745/1678417030645-6d9d1b69-864b-471e-847a-794b4b39a6e8.png#averageHue=%23c2d5de&clientId=uc04b4c8e-4886-4&from=paste&id=ucee245c9&originHeight=540&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ubaf903bf-66da-440a-9b44-106f3261e60&title=TCP%E6%98%AF%E4%BB%80%E4%B9%88 "TCP是什么")<br />问题就出在这个"**基于字节流**"，它是个源源不断的二进制数据流，无边界。来多少就发多少，但是能发多少，得看**发送缓冲区还剩多少空间**。<br />举个例子，假设A线程想发123数据包，B线程想发456数据包。<br />A和B线程同时执行send()，A先抢到锁，此时发送缓冲区就剩1个数据包的位置，那发了"1"，然后发送缓冲区**满了**，A线程退出（非阻塞），当发送缓冲区腾出位置后，此时AB再次同时争抢，这次被B先抢到了，B发了"4"之后缓冲区**又满了**，不得不退出。<br />重复这样多次争抢之后，原本的数据内容都被打乱了，变成了142356。因为数据123是个整体，456又是个整体，像现在这样数据**被打乱**的话，接收方就算收到了数据也**没办法正常解析**。<br />![并发写socket_fd导致数据异常](https://cdn.nlark.com/yuque/0/2023/png/396745/1678417031204-8e923df1-2977-4c35-b5d4-5f2e02b03938.png#averageHue=%23e7ecee&clientId=uc04b4c8e-4886-4&from=paste&id=u98786929&originHeight=1170&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u88bfa626-8cfd-477b-979c-6dbe3e6f179&title=%E5%B9%B6%E5%8F%91%E5%86%99socket_fd%E5%AF%BC%E8%87%B4%E6%95%B0%E6%8D%AE%E5%BC%82%E5%B8%B8 "并发写socket_fd导致数据异常")<br />也就是说**锁的粒度**其实是每次"**写操作**"，但每次写操作并不保证能把消息写**完整**。<br />那么问题就来了，**那是不是在写整个完整消息之前加个锁，整个消息都写完之后再解锁，这样就好了?**<br />类似下面这样。
```c
// 伪代码
int safe_send(msg string)
{
    target_len = length(msg)
        have_send_len = 0
    // 加锁
    lock();

    // 不断循环直到发完整个完整消息
       do {
     send_len := send(sockfd,msg)
     have_send_len = have_send_len + send_len
       } while(have_send_len < target_len)
   

    // 解锁
    unlock();

}
```
**这也不行**，加锁这个事情是**影响性能**的，锁的粒度越小，性能就越好。反之性能就越差。<br />当抢到了锁，使用 `send(sockfd,msg)` 发送完整数据的时候，如果此时发送缓冲区正好一写就满了，那这个线程就得一直占着这个锁直到整个消息写完。其他线程都在旁边等它解锁，啥事也干不了，焦急难耐想着抢锁。<br />但凡**某个消息体稍微大点**，这样的问题就会变得更严重。整个服务的性能也会被这波神仙操作给**拖垮**。<br />归根结底还是因为**锁的粒度太大了**。<br />**有没有更好的方式呢？**<br />其实多个线程抢锁，最后抢到锁的线程才能进行写操作，从本质上来看，就是将所有用户发给GameServer逻辑服务器的消息给**串行化**了，<br />那既然是串行化，完全可以在在业务代码里为每个socket_fd配一个**队列**来做，将数据在用户态**加锁后塞到这个队列里**，再单独开一个**线程**，这个线程的工作就是发送消息给socket_fd。<br />于是上面的场景就变成了下面这样。<br />![并发写到加锁队列后由一个线程处理](https://cdn.nlark.com/yuque/0/2023/png/396745/1678417031118-38dbd02b-fd28-47d7-9040-5a08b5589247.png#averageHue=%23e6ebee&clientId=uc04b4c8e-4886-4&from=paste&id=u7b02616f&originHeight=1530&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ub493dec0-e76e-4d77-85c2-54d79136d8b&title=%E5%B9%B6%E5%8F%91%E5%86%99%E5%88%B0%E5%8A%A0%E9%94%81%E9%98%9F%E5%88%97%E5%90%8E%E7%94%B1%E4%B8%80%E4%B8%AA%E7%BA%BF%E7%A8%8B%E5%A4%84%E7%90%86 "并发写到加锁队列后由一个线程处理")<br />于是在gateway层，多个用户线程同时写消息时，会去争抢某个socket_fd对应的队列，抢到锁之后就写数据到队列。而真正执行 `send(sockfd,msg)` 的线程其实**只有一个**。它会从这个队列中取数据，然后**不加锁的**批量发送数据到 GameServer。<br />由于加锁后要做的事情很简单，也就塞个队列而已，因此非常快。并且由于执行发送数据的只有单个线程，因此也不会有消息体乱序的问题。
<a name="F6Omh"></a>
## 读TCP Socket是线程安全的吗？
在前面有了写socket是线程安全的结论，稍微翻一下源码就能发现，**读socket其实也是加锁了的，所以并发多线程读socket这件事是线程安全的**。
```c
// net/ipv4/tcp.c
int tcp_recvmsg(struct kiocb *iocb, struct sock *sk, struct msghdr *msg,
        size_t len, int nonblock, int flags, int *addr_len)
{

    // 加锁
    lock_sock(sk);

    // ... 将数据从接收缓冲区拷贝到用户缓冲区

    // 释放锁
    release_sock(sk);

}
```
但就算是线程安全，也不代表可以用多个线程并发去读。<br />**因为这个锁，只保证在读socket 接收缓冲区时，只有一个线程在读，但并不能保证每次读的时候，都能正好读到完整消息体后才返回。**<br />所以虽然并发读不报错，但**每个线程拿到的消息肯定都不全，因为锁的粒度并不保证能读完完整消息。**<br />TCP是基于数据流的协议，数据流会源源不断从网卡那送到**接收缓冲区**。<br />如果此时接收缓冲区里有两条完整消息，比如 "**我是小白**"和"点赞在看走一波"。<br />有两个线程A和B同时**并发去读**的话，A线程就可能读到“**我是** 点赞走一波"， B线程就可能读到”**小白** 在看"<br />两条消息都变得不完整了。<br />![并发读socket_fd导致的数据异常](https://cdn.nlark.com/yuque/0/2023/png/396745/1678417031072-a6aafa0b-8db4-4856-ab57-9000eea2dc7c.png#averageHue=%23e7ebee&clientId=uc04b4c8e-4886-4&from=paste&id=ufe90e8c9&originHeight=1172&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u9df577dd-a7ec-479a-8226-01bf8702296&title=%E5%B9%B6%E5%8F%91%E8%AF%BBsocket_fd%E5%AF%BC%E8%87%B4%E7%9A%84%E6%95%B0%E6%8D%AE%E5%BC%82%E5%B8%B8 "并发读socket_fd导致的数据异常")<br />解决方案还是跟读的时候一样，读socket的只能有**一个**线程，读到了消息之后塞到**加锁队列**中，再将消息分开给到GameServer的多线程用户逻辑模块中去做处理。<br />![单线程读socket_fd后写入加锁队列](https://cdn.nlark.com/yuque/0/2023/png/396745/1678417031292-3fd1f77e-47dc-4952-a51b-918a630998a1.png#averageHue=%23e7ecee&clientId=uc04b4c8e-4886-4&from=paste&id=uc0ba9c46&originHeight=1530&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u67cabd80-4f2c-45d8-b1d6-c29fca2f7cc&title=%E5%8D%95%E7%BA%BF%E7%A8%8B%E8%AF%BBsocket_fd%E5%90%8E%E5%86%99%E5%85%A5%E5%8A%A0%E9%94%81%E9%98%9F%E5%88%97 "单线程读socket_fd后写入加锁队列")
<a name="owWBK"></a>
## 读写UDP Socket是线程安全的吗？
聊完TCP，很自然就能想到另外一个传输层协议UDP，那么它是线程安全的吗？<br />平时写代码的时候如果要使用udp发送消息，一般会像下面这样操作。
```c
ssize_t sendto(int sockfd, const void *buf, size_t nbytes, int flags, const struct sockaddr *to, socklen_t addrlen);
```
而执行到底层，会到linux内核的`udp_sendmsg`函数中。
```c
int udp_sendmsg(struct kiocb *iocb, struct sock *sk, struct msghdr *msg, size_t len) {
   if (用到了MSG_MORE的功能) {
        lock_sock(sk);
    // 加入到发送缓冲区中
    release_sock(sk);
   } else {
        // 不加锁，直接发送消息
   }
}
```
这里用伪代码改了下，大概的含义就是用到MSG_MORE就加锁，否则**不加锁**将传入的msg作为一整个数据包直接发送。<br />首先需要搞清楚，MSG_MORE 是啥。它可以通过上面提到的sendto函数最右边的flags字段进行设置。大概的意思是告诉内核，待会还有其他**更多消息**要一起发，先别着急发出去。此时内核就会把这份数据先用**发送缓冲区**缓存起来，待会应用层说ok了，再一起发。<br />但是，一般也用不到 MSG_MORE。<br />所以直接关注另外一个分支，也就是**不加锁**直接发消息。<br />**那是不是说明走了不加锁的分支时，udp发消息并不是线程安全的？**<br />其实。还是线程安全的，不用`lock_sock(sk)`加锁，单纯是因为**没必要**。<br />开启MSG_MORE时多个线程会同时写到同一个socket_fd对应的**发送缓冲区**中，然后再统一一起发送到IP层，因此需要有个锁防止出现多个线程将对方写的数据给覆盖掉的问题。而不开启MSG_MORE时，数据则会直接发送给IP层，就没有了上面的烦恼。<br />再看下udp的接收函数udp_recvmsg，会发现**情况也类似**，这里就不再赘述。
<a name="tlHVB"></a>
### 能否多线程同时并发读或写同一个UDP socket？
在TCP中，线程安全不代表可以并发地读写同一个socket_fd，因为哪怕内核态中加了`lock_sock(sk)`，这个锁的粒度并不覆盖整个完整消息的多次分批发送，它只保证单次发送的线程安全，所以建议只用一个线程去读写一个socket_fd。<br />那么问题又来了，那UDP呢？会有一样的问题吗？<br />跟TCP对比下，大家就知道了。<br />TCP不能用多线程同时读和同时写，是因为它是基于**数据流**的协议。<br />那UDP呢？它是基于**数据报**的协议。<br />![UDP是什么](https://cdn.nlark.com/yuque/0/2023/png/396745/1678417031259-16589668-e400-4434-a10b-32900b8be5b5.png#averageHue=%23c2d4de&clientId=uc04b4c8e-4886-4&from=paste&id=u18190d91&originHeight=540&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u36153ae0-5a38-42f7-8c4c-f5584ea13da&title=UDP%E6%98%AF%E4%BB%80%E4%B9%88 "UDP是什么")
<a name="fkG77"></a>
#### 基于数据流和基于数据报有什么区别呢？
基于**数据流**，意味着发给内核底层的数据就跟**水进入水管**一样，内核根本不知道什么时候是个头，没有明确的**边界**。<br />而基于**数据报**，可以类比为一件件**快递进入传送管道**一样，内核很清楚拿到的是**几件**快递，快递和快递之间**边界分明**。<br />![水滴和快递的差异](https://cdn.nlark.com/yuque/0/2023/png/396745/1678417031788-72dcfab6-6d69-4de3-8197-5b5e6509112f.png#averageHue=%23ebeef1&clientId=uc04b4c8e-4886-4&from=paste&id=u063664a1&originHeight=486&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ud3e04e98-b3e1-4c96-9ff1-8772864d711&title=%E6%B0%B4%E6%BB%B4%E5%92%8C%E5%BF%AB%E9%80%92%E7%9A%84%E5%B7%AE%E5%BC%82 "水滴和快递的差异")<br />那从使用的方式来看，应用层通过TCP去发数据，TCP就先把它放到缓冲区中，然后就返回。至于**什么时候发数据，发多少数据，发的数据是刚刚应用层传进去的一半还是全部都是不确定的**，全看内核的心情。在接收端收的时候也一样。<br />但UDP就不同，UDP 对应用层交下来的报文，**既不合并，也不拆分**，而是保留这些报文的**边界**。<br />无论应用层交给 UDP 多长的报文，UDP 都照样发送，即一次发送一个报文。至于数据包太长，需要分片，那也是IP层的事情，跟UDP没啥关系，大不了效率低一些。而接收方在接收数据报的时候，一次取一个完整的包，不存在TCP常见的半包和粘包问题。<br />正因为**基于数据报**和**基于字节流**的差异，**TCP 发送端发 10 次字节流数据，接收端可以分 100 次去取数据，每次取数据的长度可以根据处理能力作调整；但 UDP 发送端发了 10 次数据报，那接收端就要在 10 次收完，且发了多少次，就取多少次，确保每次都是一个完整的数据报。**<br />所以从这个角度来说，UDP写数据报的行为是"原子"的，不存在发一半包或收一半包的问题，要么整个包成功，要么整个包失败。因此多个线程同时读写，也就不会有TCP的问题。<br />所以，可以多个线程同时读写同一个udp socket。<br />但**就算可以，依然不建议大家这么做。**
<a name="ZAxiS"></a>
### 为什么不建议使用多线程同时读写同一个UDP socket
udp本身是**不可靠**的协议，多线程高并发执行发送时，会对系统造成较大压力，这时候**丢包**是常见的事情。虽然这时候应用层能实现**重传逻辑**，但重传这件事毕竟是越少越好。因此通常还会希望能有个应用层流量控制的功能，如果是**单线程**读写的话，就可以在同一个地方对流量实现调控。类似的，实现其他插件功能也会更加方便，比如给某些vip等级的老板更快速的游戏体验啥的（瞎说的）。<br />所以正确的做法，还是跟TCP一样，不管外面有多少个线程，还是并发加锁写到一个队列里，然后起一个单独的线程去做发送操作。<br />![udp并发写加锁队列后再写socket_fd](https://cdn.nlark.com/yuque/0/2023/png/396745/1678417031811-5ce12dbf-050c-40a6-a205-65274855e3b6.png#averageHue=%23e6ebed&clientId=uc04b4c8e-4886-4&from=paste&id=u2d3882be&originHeight=1530&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ub300ba53-c914-4ef5-9380-616b7410362&title=udp%E5%B9%B6%E5%8F%91%E5%86%99%E5%8A%A0%E9%94%81%E9%98%9F%E5%88%97%E5%90%8E%E5%86%8D%E5%86%99socket_fd "udp并发写加锁队列后再写socket_fd")
<a name="QYZFF"></a>
## 总结

1. 多线程并发读/写同一个TCP socket是线程安全的，因为TCP socket的读/写操作都上锁了。虽然线程安全，但依然不建议这么做，因为TCP本身是基于数据流的协议，一份完整的消息数据可能会**分开多次**去写/读，内核的锁只保证**单次**读/写socket是线程安全，**锁的粒度**并不覆盖整个完整消息。因此建议用一个线程去读/写TCP socket。
2. 多线程并发读/写同一个UDP socket也是线程安全的，因为UDP socket的读/写操作也都上锁了。UDP写数据报的行为是"原子"的，不存在发一半包或收一半包的问题，要么整个包成功，要么整个包失败。因此多个线程同时读写，也就不会有TCP的问题。虽然如此，但还是建议用一个线程去读/写UDP socket。
<a name="KbcY3"></a>
## 最后
上面文章里提到，建议用单线程的方式去读/写socket，但每个socket都配一个线程这件事情，显然有些奢侈，比如线程切换的代价也不小，那这种情况有什么好的解决办法吗？
