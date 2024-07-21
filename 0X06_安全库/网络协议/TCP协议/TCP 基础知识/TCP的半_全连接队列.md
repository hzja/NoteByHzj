网络 TCP<br />![](https://cdn.nlark.com/yuque/0/2021/gif/396745/1636948644778-da0849d3-8e49-4d23-b19e-0947a2ad641f.gif#clientId=u81f4f865-fd05-4&from=paste&id=u1b929dd3&originHeight=607&originWidth=1079&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u889c5bbe-b9a8-4be3-ba8c-13bb8e21e3e&title=)<br />握手建立连接流程<br />上面这个动图，是平时客户端和服务端建立连接时的代码流程。<br />对应的是下面一段简化过的服务端伪代码。
```c
int main()
{
  /*Step 1: 创建服务器端监听socket描述符listen_fd*/    
  listen_fd = socket(AF_INET, SOCK_STREAM, 0);

  /*Step 2: bind绑定服务器端的IP和端口，所有客户端都向这个IP和端口发送和请求数据*/    
  bind(listen_fd, xxx);

  /*Step 3: 服务端开启监听*/    
  listen(listen_fd, 128);

  /*Step 4: 服务器等待客户端的链接，返回值cfd为客户端的socket描述符*/    
  cfd = accept(listen_fd, xxx);

  /*Step 5: 读取客户端发来的数据*/
  n = read(cfd, buf, sizeof(buf));
}
```
估计大家也是老熟悉这段伪代码了。<br />需要注意的是，在执行`listen()`方法之后还会执行一个`accept()`方法。<br />**一般情况**下，如果启动服务器，会发现最后程序会**阻塞在**`accept()`里。<br />此时服务端就算ok了，就等客户端了。<br />那么，再看下简化过的客户端伪代码。
```c
int main()
{
    /*Step 1: 创建客户端端socket描述符cfd*/    
    cfd = socket(AF_INET, SOCK_STREAM, 0);

    /*Step 2: connect方法,对服务器端的IP和端口号发起连接*/    
    ret = connect(cfd, xxxx);

    /*Step 4: 向服务器端写数据*/
    write(cfd, buf, strlen(buf));
}
```
客户端比较简单，创建好socket之后，直接就发起`connect`方法。<br />此时回到服务端，会发现**之前一直阻塞的**`**accept**`**方法，返回结果了**。<br />这就算两端成功建立好了一条连接。之后就可以愉快的进行读写操作了。<br />那么，今天的问题是，**如果没有这个**`**accept**`**方法，TCP连接还能建立起来吗？**<br />其实只要在执行`accept()` 之前执行一个 `sleep(20)`，然后立刻执行客户端相关的方法，同时抓个包，就能得出结论。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636948644848-83cb5246-f560-4e68-bf56-1fab3f347ca0.webp#clientId=u81f4f865-fd05-4&from=paste&id=u0451ea3a&originHeight=240&originWidth=992&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u51638608-a09e-480d-97f0-00610034caa&title=)<br />不执行accept时抓包结果<br />从抓包结果看来，**就算不执行**`**accept()**`**方法，三次握手照常进行，并顺利建立连接。**<br />更骚气的是，**在服务端执行**`**accept()**`**前，如果客户端发送消息给服务端，服务端是能够正常回复ack确认包的。**<br />并且，`sleep(20)`结束后，服务端正常执行`accept()`，客户端前面发送的消息，还是能正常收到的。<br />通过这个现象，可以多想想为什么。顺便好好了解下三次握手的细节。
<a name="TKWgR"></a>
## 三次握手的细节分析
先看三次握手。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636948644841-2acfb340-9614-485a-86e9-680c8f940b19.webp#clientId=u81f4f865-fd05-4&from=paste&id=u426d8873&originHeight=990&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf82bb2cc-8cf3-437e-8cc5-278676779fe&title=)<br />TCP三次握手<br />服务端代码，对socket执行`bind`方法可以绑定监听端口，然后执行`listen`方法后，就会进入监听（LISTEN）状态。内核会为每一个处于`LISTEN`状态的socket 分配两个队列，分别叫**半连接队列和全连接队列**。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636948644717-a4bcc3bf-3b18-46cb-971c-7f9873af5ca8.webp#clientId=u81f4f865-fd05-4&from=paste&id=u61630bad&originHeight=450&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ueedd59e3-a9a3-4c34-9ec2-6c7cd8f13eb&title=)<br />每个listen Socket都有一个全连接和半连接队列
<a name="Imt0L"></a>
### 半连接队列、全连接队列是什么
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636948644854-bdee42c1-4320-4824-8bf5-8f919a128765.webp#clientId=u81f4f865-fd05-4&from=paste&id=u0a6c8b2d&originHeight=800&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uca5b0c2a-992d-477e-a1cd-fb4c9d159ca&title=)<br />半连接队列和全连接队列

- **半连接队列（SYN队列）**，服务端收到**第一次握手**后，会将sock加入到这个队列中，队列内的sock都处于SYN_RECV 状态。
- **全连接队列（ACCEPT队列）**，在服务端收到**第三次握手**后，会将半连接队列的sock取出，放到全连接队列中。队列里的sock都处于 ESTABLISHED状态。这里面的连接，就**等着服务端执行accept()后被取出了。**

看到这里，文章开头的问题就有了答案，建立连接的过程中根本不需要`accept()` 参与， **执行accept()只是为了从全连接队列里取出一条连接。**<br />把话题再重新回到这两个队列上。<br />虽然都叫**队列**，但其实**全连接队列（icsk_accept_queue）是个链表**，而**半连接队列（syn_table）是个哈希表**。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636948645240-cf488196-27fa-47dc-8510-e814f6fb34ea.webp#clientId=u81f4f865-fd05-4&from=paste&id=ua5ad9365&originHeight=540&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u44a404fe-f88a-4941-aff5-ffbeab54940&title=)<br />半连接全连接队列的内部结构
<a name="i7Kqi"></a>
### 为什么半连接队列要设计成哈希表
先对比下**全连接里队列**，他本质是个链表，因为也是线性结构，说它是个队列也没毛病。它里面放的都是已经建立完成的连接，这些连接正等待被取走。而服务端取走连接的过程中，并不关心具体是哪个连接，只要是个连接就行，所以直接从队列头取就行了。这个过程算法复杂度为O(1)。<br />而**半连接队列**却不太一样，因为队列里的都是不完整的连接，嗷嗷等待着第三次握手的到来。那么现在有一个第三次握手来了，则需要从队列里把相应IP端口的连接取出，**如果半连接队列还是个链表，那就需要依次遍历，才能拿到想要的那个连接，算法复杂度就是O(n)。**<br />而如果将半连接队列设计成哈希表，那么查找半连接的算法复杂度就回到O(1)了。<br />因此出于效率考虑，全连接队列被设计成链表，而半连接队列被设计为哈希表。
<a name="WoI8P"></a>
### 怎么观察两个队列的大小
<a name="WOoWf"></a>
#### 查看全连接队列
```bash
# ss -lnt
State      Recv-Q Send-Q     Local Address:Port           Peer Address:Port
LISTEN     0      128        127.0.0.1:46269              *:*              
```
通过ss -lnt命令，可以看到全连接队列的大小，其中Send-Q是指全连接队列的最大值，可以看到这上面的最大值是128；Recv-Q是指当前的全连接队列的使用值，这边用了0个，也就是全连接队列里为空，连接都被取出来了。<br />当上面Send-Q和Recv-Q数值很接近的时候，那么全连接队列可能已经满了。可以通过下面的命令查看是否发生过队列**溢出**。
```bash
# netstat -s | grep overflowed
    4343 times the listen queue of a socket overflowed
```
上面说明发生过4343次全连接队列溢出的情况。这个查看到的是**历史发生过的次数**。<br />如果配合使用watch -d 命令，可以自动每2s间隔执行相同命令，还能高亮显示变化的数字部分，如果溢出的数字不断变多，说明**正在发生**溢出的行为。
```bash
# watch -d 'netstat -s | grep overflowed'
Every 2.0s: netstat -s | grep overflowed                                Fri Sep 17 09:00:45 2021

    4343 times the listen queue of a socket overflowed
```
<a name="C7bEy"></a>
#### 查看半连接队列
半连接队列没有命令可以直接查看到，但因为半连接队列里，放的都是SYN_RECV 状态的连接，那可以通过统计处于这个状态的连接的数量，间接获得半连接队列的长度。
```bash
# netstat -nt | grep -i '127.0.0.1:8080' | grep -i 'SYN_RECV' | wc -l
0
```
注意半连接队列和全连接队列都是挂在某个Listen socket上的，这里用的是127.0.0.1:8080，大家可以替换成自己想要查看的**IP端口**。<br />可以看到机器上的半连接队列长度为0，这个很正常，**正经连接谁会没事老待在半连接队列里。**<br />当队列里的半连接不断增多，最终也是会发生溢出，可以通过下面的命令查看。
```bash
# netstat -s | grep -i "SYNs to LISTEN sockets dropped" 
    26395 SYNs to LISTEN sockets dropped
```
可以看到，机器上一共发生了26395次半连接队列溢出。同样建议配合`watch -d` 命令使用。
```bash
# watch -d 'netstat -s | grep -i "SYNs to LISTEN sockets dropped"'
Every 2.0s: netstat -s | grep -i "SYNs to LISTEN sockets dropped"       Fri Sep 17 08:36:38 2021

    26395 SYNs to LISTEN sockets dropped
```
<a name="QVQqb"></a>
### 全连接队列满了会怎么样？
如果队列满了，服务端还收到客户端的第三次握手ACK，默认当然会丢弃这个ACK。<br />但除了丢弃之外，还有一些附带行为，这会受 tcp_abort_on_overflow 参数的影响。
```bash
# cat /proc/sys/net/ipv4/tcp_abort_on_overflow
0
```

- `tcp_abort_on_overflow`设置为 0，全连接队列满了之后，会丢弃这个第三次握手ACK包，并且开启定时器，重传第二次握手的SYN+ACK，如果重传超过一定限制次数，还会把对应的**半连接队列里的连接**给删掉。

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636948645389-3124dd96-613c-48bb-95fc-ba863cfabaa0.webp#clientId=u81f4f865-fd05-4&from=paste&id=uf8bce487&originHeight=1067&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uccadc566-c8dd-470f-b2e4-a9a40d70f39&title=)<br />tcp_abort_on_overflow为0

- `tcp_abort_on_overflow`设置为 1，全连接队列满了之后，就直接发RST给客户端，效果上看就是连接断了。

这个现象是不是很熟悉，服务端**端口未监听**时，客户端尝试去连接，服务端也会回一个RST。这两个情况长一样，所以客户端这时候收到RST之后，其实无法区分到底是**端口未监听**，还是**全连接队列满了**。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636948645408-9f03edc9-6b05-48ee-abb7-6947f016fa74.webp#clientId=u81f4f865-fd05-4&from=paste&id=ue33542ee&originHeight=800&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u933ed6ab-ce37-435c-99e6-045b599c2fd&title=)<br />tcp_abort_on_overflow为1
<a name="cwOMq"></a>
### 半连接队列要是满了会怎么样
**一般是丢弃**，但这个行为可以通过 tcp_syncookies 参数去控制。但比起这个，更重要的是先了解下半连接队列为什么会被打满。<br />首先需要明白，一般情况下，半连接的"生存"时间其实很短，只有在第一次和第三次握手间，如果半连接都满了，说明服务端疯狂收到第一次握手请求，如果是线上游戏应用，能有这么多请求进来，那说明你可能要富了。但现实往往比较骨感，可能遇到了**SYN Flood攻击**。<br />所谓**SYN Flood攻击**，可以简单理解为，攻击方模拟客户端疯狂发第一次握手请求过来，在服务端憨憨地回复第二次握手过去之后，客户端死活不发第三次握手过来，这样做，可以把服务端半连接队列打满，从而导致正常连接不能正常进来。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636948645572-cce81546-c0e6-4eff-affa-c974ad42c134.webp#clientId=u81f4f865-fd05-4&from=paste&id=uda34d11b&originHeight=576&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u01c9bdd5-0630-41ab-bd03-6b46e2a4efb&title=)<br />syn攻击<br />那这种情况怎么处理？有没有一种方法可以**绕过半连接队列**？<br />有，上面提到的tcp_syncookies派上用场了。
```bash
# cat /proc/sys/net/ipv4/tcp_syncookies
1
```
当它被设置为1的时候，客户端发来**第一次握手**SYN时，服务端**不会将其放入半连接队列中**，而是直接生成一个cookies，这个cookies会跟着**第二次握手**，发回客户端。客户端在发**第三次握手**的时候带上这个cookies，服务端验证到它就是当初发出去的那个，就会建立连接并放入到全连接队列中。可以看出整个过程不再需要半连接队列的参与。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636948645543-1b2db90a-d61d-4061-98f2-5c4f5ed7cadb.webp#clientId=u81f4f865-fd05-4&from=paste&id=u6e768c4e&originHeight=800&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7137c577-dfe1-483c-97ff-35c46bd9b06&title=)<br />tcp_syncookies=1
<a name="DUlv5"></a>
#### 会有一个cookies队列吗
生成是cookies，保存在哪呢？**是不是会有一个队列保存这些cookies？**<br />可以反过来想一下，如果有cookies队列，那它会跟半连接队列一样，到头来，还是会被**SYN Flood 攻击**打满。<br />实际上cookies并不会有一个专门的队列保存，它是通过**通信双方的IP地址端口、时间戳、MSS**等信息进行**实时计算**的，保存在**TCP报头**的seq里。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636948645736-a056a529-23d0-4bfb-bff9-a46cdacae7dc.webp#clientId=u81f4f865-fd05-4&from=paste&id=u8f93a382&originHeight=450&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ubf63946f-3049-47fd-855c-ddcb37c9bc6&title=)<br />tcp报头_seq的位置<br />当服务端收到客户端发来的第三次握手包时，会通过seq还原出**通信双方的IP地址端口、时间戳、MSS**，验证通过则建立连接。
<a name="MAAU4"></a>
#### cookies方案为什么不直接取代半连接队列？
目前看下来syn cookies方案省下了半连接队列所需要的队列内存，还能解决 **SYN Flood攻击**，那为什么不直接取代半连接队列？<br />凡事皆有利弊，cookies方案虽然能防 **SYN Flood攻击**，但是也有一些问题。因为服务端并不会保存连接信息，所以如果传输过程中数据包丢了，也不会重发第二次握手的信息。<br />另外，编码解码cookies，都是比较**耗CPU**的，利用这一点，如果此时攻击者构造大量的**第三次握手包（ACK包）**，同时带上各种瞎编的cookies信息，服务端收到ACK包后**以为是正经cookies**，憨憨地跑去解码（**耗CPU**），最后发现不是正经数据包后才丢弃。<br />这种通过构造大量ACK包去消耗服务端资源的攻击，叫**ACK攻击**，受到攻击的服务器可能会因为**CPU资源耗尽**导致没能响应正经请求。<br />![](https://cdn.nlark.com/yuque/0/2021/gif/396745/1636948646077-56f649d0-d310-45cc-9a1c-08b3032d77df.gif#clientId=u81f4f865-fd05-4&from=paste&id=u4b171285&originHeight=607&originWidth=1079&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf73aa9de-bcd8-4560-926d-aeacce9c113&title=)<br />ack攻击
<a name="KPj6U"></a>
### 没有`listen`，为什么还能建立连接
那既然没有`accept`方法能建立连接，那是不是没有`listen`方法，也能建立连接？是的，之前写的一篇文章提到过客户端是可以自己连自己的形成连接（**TCP自连接**），也可以两个客户端同时向对方发出请求建立连接（**TCP同时打开**），这两个情况都有个共同点，就是**没有服务端参与，也就是没有**`**listen**`**，就能建立连接。**<br />当时文章最后也留了个疑问，**没有**`**listen**`**，为什么还能建立连接？**<br />执行`listen`方法时，会创建半连接队列和全连接队列。<br />三次握手的过程中会在这两个队列中暂存连接信息。<br />所以形成连接，前提是得**有个地方存放着**，方便握手的时候能根据IP端口等信息找到socket信息。<br />**那么客户端会有半连接队列吗？**<br />**显然没有**，因为客户端没有执行`listen`，因为半连接队列和全连接队列都是在执行`listen`方法时，内核自动创建的。<br />但内核还有个**全局hash表**，可以用于存放`sock`连接的信息。这个全局`hash`表其实还细分为`ehash`，`bhash`和`listen_hash`等，但因为过于细节，大家理解成有一个**全局hash**就够了，<br />在TCP自连接的情况中，客户端在`connect`方法时，最后会将自己的连接信息放入到这个**全局**`**hash**`**表**中，然后将信息发出，消息在经过回环地址重新回到TCP传输层的时候，就会根据IP端口信息，再一次从这个**全局**`**hash**`中取出信息。于是握手包一来一回，最后成功建立连接。<br />TCP同时打开的情况也类似，只不过从一个客户端变成了两个客户端而已。
<a name="Aet6E"></a>
## 总结

- **每一个**`socket`执行`listen`时，内核都会自动创建一个半连接队列和全连接队列。
- 第三次握手前，TCP连接会放在半连接队列中，直到第三次握手到来，才会被放到全连接队列中。
- `accept`方法只是为了从全连接队列中拿出一条连接，本身跟三次握手几乎**毫无关系**。
- 出于效率考虑，虽然都叫队列，但半连接队列其实被设计成了**哈希表**，而全连接队列本质是链表。
- 全连接队列满了，再来第三次握手也会丢弃，此时如果`tcp_abort_on_overflow=1`，还会直接发RST给客户端。
- 半连接队列满了，可能是因为受到了SYN Flood攻击，可以设置`tcp_syncookies`，绕开半连接队列。
- 客户端没有半连接队列和全连接队列，但有一个**全局hash**，可以通过它实现自连接或TCP同时打开。
