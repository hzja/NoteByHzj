网络协议 TCP
<a name="w5GpX"></a>
## 前言
网上许多博客针对增大 TCP 半连接队列和全连接队列的方式如下：

- 增大 TCP 半连接队列方式是增大 tcp_max_syn_backlog；
- 增大 TCP 全连接队列方式是增大 `listen()` 函数中的 backlog；

这里先说下，**上面的方式都是不准确的。**<br />这里做了实验和看了 TCP 协议栈的内核源码，发现要增大这两个队列长度，不是简简单单增大某一个参数就可以的。<br />提纲：<br />![本文提纲](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773269779-c96c0797-fd39-43e3-a986-00bd100f2246.png#clientId=ud01b60ee-4cb1-4&from=paste&id=u1cc6c4eb&originHeight=590&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ubdea195c-885b-4d87-a7bd-fbbc7a6b34d&title=%E6%9C%AC%E6%96%87%E6%8F%90%E7%BA%B2 "本文提纲")

---

<a name="xEQBS"></a>
## 正文
<a name="UFit6"></a>
### 什么是 TCP 半连接队列和全连接队列？
在 TCP 三次握手的时候，Linux 内核会维护两个队列，分别是：

- 半连接队列，也称 SYN 队列；
- 全连接队列，也称 accepet 队列；

服务端收到客户端发起的 SYN 请求后，**内核会把该连接存储到半连接队列**，并向客户端响应 SYN+ACK，接着客户端会返回 ACK，服务端收到第三次握手的 ACK 后，**内核会把连接从半连接队列移除，然后创建新的完全的连接，并将其添加到 accept 队列，等待进程调用 accept 函数时把连接取出来。**<br />![半连接队列与全连接队列](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773269757-ccba0b65-185d-4136-aab4-08f4b459c85c.png#clientId=ud01b60ee-4cb1-4&from=paste&id=u7f66e2d1&originHeight=1081&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u776c4993-b211-4240-a6db-521166305a2&title=%E5%8D%8A%E8%BF%9E%E6%8E%A5%E9%98%9F%E5%88%97%E4%B8%8E%E5%85%A8%E8%BF%9E%E6%8E%A5%E9%98%9F%E5%88%97 "半连接队列与全连接队列")<br />不管是半连接队列还是全连接队列，都有最大长度限制，超过限制时，内核会直接丢弃，或返回 RST 包。

---

<a name="j1DCQ"></a>
### 实战 - TCP 全连接队列溢出
<a name="vFqcP"></a>
#### 如何知道应用程序的 TCP 全连接队列大小？
在服务端可以使用 ss 命令，来查看 TCP 全连接队列的情况：<br />但需要注意的是 ss 命令获取的 Recv-Q/Send-Q 在「LISTEN 状态」和「非 LISTEN 状态」所表达的含义是不同的。从下面的内核代码可以看出区别：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773269717-30a86c45-41d2-4778-9b51-151b38f990bb.png#clientId=ud01b60ee-4cb1-4&from=paste&id=u967037cf&originHeight=1086&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue9350552-f98c-4203-8ee2-6494bf09d9f&title=)<br />在「LISTEN 状态」时，Recv-Q/Send-Q 表示的含义如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773269747-86fd1f2f-8d03-4f83-903d-2d524b14c47b.png#clientId=ud01b60ee-4cb1-4&from=paste&id=uc419a377&originHeight=315&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u33bdb47a-4f80-4f35-9f1d-6ba47a3a026&title=)

- Recv-Q：当前全连接队列的大小，也就是当前已完成三次握手并等待服务端 accept() 的 TCP 连接个数；
- Send-Q：当前全连接最大队列长度，上面的输出结果说明监听 8088 端口的 TCP 服务进程，最大全连接长度为 128；

在「非 LISTEN 状态」时，Recv-Q/Send-Q 表示的含义如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773269782-1e574d4e-778b-4192-926b-01f61c023960.png#clientId=ud01b60ee-4cb1-4&from=paste&id=u01e23125&originHeight=282&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=udaf2f80d-ad6c-4288-a178-f540b8201df&title=)

- Recv-Q：已收到但未被应用进程读取的字节数；
- Send-Q：已发送但未收到确认的字节数；
<a name="hz9Wn"></a>
#### 如何模拟 TCP 全连接队列溢出的场景？
![测试环境](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773270097-6d2554a0-9b7f-429b-8137-3feacd2a5566.png#clientId=ud01b60ee-4cb1-4&from=paste&id=u1e51c250&originHeight=377&originWidth=572&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ubef8a8c2-1328-448c-bbf7-ad393c75021&title=%E6%B5%8B%E8%AF%95%E7%8E%AF%E5%A2%83 "测试环境")<br />实验环境：

- 客户端和服务端都是 CentOs 6.5 ，Linux 内核版本 2.6.32
- 服务端 IP 192.168.3.200，客户端 IP 192.168.3.100
- 服务端是 Nginx 服务，端口为 8088

这里先介绍下 wrk 工具，它是一款简单的 HTTP 压测工具，它能够在单机多核 CPU 的条件下，使用系统自带的高性能 I/O 机制，通过多线程和事件模式，对目标机器产生大量的负载。<br />本次模拟实验就使用 wrk 工具来压力测试服务端，发起大量的请求，一起看看服务端 TCP 全连接队列满了会发生什么？有什么观察指标？<br />客户端执行 wrk 命令对服务端发起压力测试，并发 3 万个连接：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773270322-fd8651ca-66cc-43b2-bdc0-b2904def8340.png#clientId=ud01b60ee-4cb1-4&from=paste&id=u096c6b4a&originHeight=499&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u35d290ed-44e2-4ce9-b06a-92b483c4c60&title=)<br />在服务端可以使用 ss 命令，来查看当前 TCP 全连接队列的情况：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773270280-6cc7f18e-665f-44ce-9496-cdbe4fbd752d.png#clientId=ud01b60ee-4cb1-4&from=paste&id=ubd3be10d&originHeight=343&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u82e25cf7-61bd-4289-ae53-0519dbfa248&title=)<br />其间共执行了两次 ss 命令，从上面的输出结果，可以发现当前 TCP 全连接队列上升到了 129 大小，超过了最大 TCP 全连接队列。<br />**当超过了 TCP 最大全连接队列，服务端则会丢掉后续进来的 TCP 连接**，丢掉的 TCP 连接的个数会被统计起来，可以使用 `netstat -s` 命令来查看：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773270323-b7ea257a-788a-4c5a-b51e-fff04fba6f32.png#clientId=ud01b60ee-4cb1-4&from=paste&id=u38b8d410&originHeight=476&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5ef7ba7d-97ed-4907-bc2a-45f468e5ce6&title=)<br />上面看到的 41150 times ，表示全连接队列溢出的次数，注意这个是累计值。可以隔几秒钟执行下，如果这个数字一直在增加的话肯定全连接队列偶尔满了。<br />从上面的模拟结果，可以得知，**当服务端并发处理大量请求时，如果 TCP 全连接队列过小，就容易溢出。发生 TCP 全连接队溢出的时候，后续的请求就会被丢弃，这样就会出现服务端请求数量上不去的现象。**<br />![全连接队列溢出](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773270606-3073ef4b-2d6f-4208-a88a-0612f7446a5a.png#clientId=ud01b60ee-4cb1-4&from=paste&id=ue2568b5f&originHeight=347&originWidth=839&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u1e00c687-213c-4b16-a9d2-f6a451ffd00&title=%E5%85%A8%E8%BF%9E%E6%8E%A5%E9%98%9F%E5%88%97%E6%BA%A2%E5%87%BA "全连接队列溢出")
<a name="qjLa6"></a>
#### 全连接队列满了，就只会丢弃连接吗？
实际上，丢弃连接只是 Linux 的默认行为，还可以选择向客户端发送 RST 复位报文，告诉客户端连接已经建立失败。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773270580-a217d095-632b-47ef-afd0-8c140089a22c.png#clientId=ud01b60ee-4cb1-4&from=paste&id=ub8ada6a8&originHeight=271&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u60f1c6ee-5abc-4414-a848-74396d1efa2&title=)<br />tcp_abort_on_overflow 共有两个值分别是 0 和 1，其分别表示：

- 0 ：表示如果全连接队列满了，那么 server 扔掉 client  发过来的 ack ；
- 1 ：表示如果全连接队列满了，那么 server 发送一个 reset 包给 client，表示废掉这个握手过程和这个连接；

如果要想知道客户端连接不上服务端，是不是服务端 TCP 全连接队列满的原因，那么可以把 tcp_abort_on_overflow 设置为 1，这时如果在客户端异常中可以看到很多 connection reset by peer 的错误，那么就可以证明是由于服务端 TCP 全连接队列溢出的问题。<br />通常情况下，应当把 tcp_abort_on_overflow 设置为 0，因为这样更有利于应对突发流量。<br />举个例子，当 TCP 全连接队列满导致服务器丢掉了 ACK，与此同时，客户端的连接状态却是 ESTABLISHED，进程就在建立好的连接上发送请求。只要服务器没有为请求回复 ACK，请求就会被多次**重发**。如果服务器上的进程只是**短暂的繁忙造成 accept 队列满，那么当 TCP 全连接队列有空位时，再次接收到的请求报文由于含有 ACK，仍然会触发服务器端成功建立连接。**<br />所以，tcp_abort_on_overflow 设为 0 可以提高连接建立的成功率，只有非常肯定 TCP 全连接队列会长期溢出时，才能设置为 1 以尽快通知客户端。
<a name="cvIZu"></a>
#### 如何增大 TCP 全连接队列呢？
是的，当发现 TCP 全连接队列发生溢出的时候，就需要增大该队列的大小，以便可以应对客户端大量的请求。<br />**TCP 全连接队列足最大值取决于 somaxconn 和 backlog 之间的最小值，也就是 min(somaxconn, backlog)**。从下面的 Linux 内核代码可以得知：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773270715-d2e43526-26b4-41f6-afe1-de7aeab7973a.png#clientId=ud01b60ee-4cb1-4&from=paste&id=u4ed8f625&originHeight=749&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4e66c5f1-8f31-4dd5-8274-ef795fa4e3d&title=)

- somaxconn 是 Linux 内核的参数，默认值是 128，可以通过 /proc/sys/net/core/somaxconn 来设置其值；
- backlog 是 `listen(int sockfd, int backlog)` 函数中的 backlog 大小，Nginx 默认值是 511，可以通过修改配置文件设置其长度；

前面模拟测试中，测试环境：

- somaxconn 是默认值 128；
- Nginx 的 backlog 是默认值 511

所以测试环境的 TCP 全连接队列最大值为 min(128, 511)，也就是 128，可以执行 ss 命令查看：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773270794-f8e9cc39-e7ba-43be-98c2-ac1168260fb3.png#clientId=ud01b60ee-4cb1-4&from=paste&id=uaa3bd1bd&originHeight=323&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u92d95603-fe57-44b3-92ac-7af598736ca&title=)<br />现在重新压测，把 TCP 全连接队列**搞大**，把 somaxconn 设置成 5000：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773270873-4dbfcf47-a3d4-4342-a444-01ada55e6509.png#clientId=ud01b60ee-4cb1-4&from=paste&id=ucee6449b&originHeight=292&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue91be280-a896-4534-af91-eab2f7cf682&title=)<br />接着把 Nginx 的 backlog 也同样设置成 5000：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773271013-f0163a6e-af2b-4488-84fe-a4b14432f469.png#clientId=ud01b60ee-4cb1-4&from=paste&id=u5453e719&originHeight=498&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u79c99de3-49f7-4755-95ae-0b8db18d008&title=)<br />最后要重启 Nginx 服务，因为只有重新调用 `listen()` 函数， TCP 全连接队列才会重新初始化。<br />重启完后 Nginx 服务后，服务端执行 ss 命令，查看 TCP 全连接队列大小：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773271023-edb8b72e-6e98-411c-a441-0f42d5abce73.png#clientId=ud01b60ee-4cb1-4&from=paste&id=ubfb8238e&originHeight=311&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u868a2dae-feec-4bcc-bd74-d40bb001f1a&title=)<br />从执行结果，可以发现 TCP 全连接最大值为 5000。
<a name="mqURQ"></a>
#### 增大 TCP 全连接队列后，继续压测
客户端同样以 3 万个连接并发发送请求给服务端：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773271265-0c49a5de-45f5-43ea-ad15-f6cee8f8623c.png#clientId=ud01b60ee-4cb1-4&from=paste&id=u8a5f7ed4&originHeight=499&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u660f2641-4e1b-4cec-9025-830442a3258&title=)<br />服务端执行 ss 命令，查看 TCP 全连接队列使用情况：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773271328-f2a6185d-33f8-42a9-b6af-2d1bb874201c.png#clientId=ud01b60ee-4cb1-4&from=paste&id=u8edda968&originHeight=561&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6121bfea-24fd-4241-b279-bdaac873ed5&title=)<br />从上面的执行结果，可以发现全连接队列使用增长的很快，但是一直都没有超过最大值，所以就不会溢出，那么 netstat -s 就不会有 TCP 全连接队列溢出个数的显示：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773271436-17117ce0-d0ec-467a-8c64-dc289e390adc.png#clientId=ud01b60ee-4cb1-4&from=paste&id=uf4fce2e0&originHeight=319&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ube3c25bc-a738-4b64-9cfc-2d5130adab0&title=)<br />说明 TCP 全连接队列最大值从 128 增大到 5000 后，服务端抗住了 3 万连接并发请求，也没有发生全连接队列溢出的现象了。<br />**如果持续不断地有连接因为 TCP 全连接队列溢出被丢弃，就应该调大 backlog 以及 somaxconn 参数。**

---

<a name="Lad0C"></a>
### 实战 - TCP 半连接队列溢出
<a name="SQl5O"></a>
#### 如何查看 TCP 半连接队列长度？
很遗憾，TCP 半连接队列长度的长度，没有像全连接队列那样可以用 ss 命令查看。<br />但是可以抓住 TCP 半连接的特点，就是服务端处于 SYN_RECV 状态的 TCP 连接，就是在 TCP 半连接队列。<br />于是，可以使用如下命令计算当前 TCP 半连接队列长度：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773271521-7d533a2c-6b35-4043-a141-10caa1e2219e.png#clientId=ud01b60ee-4cb1-4&from=paste&id=u55c2a9d3&originHeight=356&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u973df34f-0bde-420f-afe8-93c080187d2&title=)
<a name="HoCI1"></a>
#### 如何模拟 TCP 半连接队列溢出场景？
模拟 TCP 半连接溢出场景不难，实际上就是对服务端一直发送 TCP SYN 包，但是不回第三次握手 ACK，这样就会使得服务端有大量的处于 SYN_RECV 状态的 TCP 连接。<br />这其实也就是所谓的 SYN 洪泛、SYN 攻击、DDos 攻击。<br />![测试环境](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773271746-d9f3f5d7-65d7-4472-8f16-737c8cc66ce2.png#clientId=ud01b60ee-4cb1-4&from=paste&id=u3193006e&originHeight=377&originWidth=572&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ua3d8cd33-fcd3-42b4-b2ea-172e5460813&title=%E6%B5%8B%E8%AF%95%E7%8E%AF%E5%A2%83 "测试环境")<br />实验环境：

- 客户端和服务端都是 CentOs 6.5 ，Linux 内核版本 2.6.32
- 服务端 IP 192.168.3.200，客户端 IP 192.168.3.100
- 服务端是 Nginx 服务，端口为 8088

注意：本次模拟实验是没有开启 tcp_syncookies，关于 tcp_syncookies 的作用，后续会说明。<br />本次实验使用 hping3 工具模拟 SYN 攻击：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773271798-05bf9a94-63b2-4d4e-b0a4-e859df2c507d.png#clientId=ud01b60ee-4cb1-4&from=paste&id=u751041a2&originHeight=366&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=udc0a9c66-122a-4d5a-a236-dd65fe1a0d1&title=)<br />当服务端受到 SYN 攻击后，连接服务端 ssh 就会断开了，无法再连上。只能在服务端主机上执行查看当前 TCP 半连接队列大小：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773271819-f7103c4e-8efa-466c-b84b-35b181a4462f.png#clientId=ud01b60ee-4cb1-4&from=paste&id=u8d174252&originHeight=277&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2837d892-6fa1-4002-a4aa-337a9f45c5f&title=)<br />同时，还可以通过 netstat -s 观察半连接队列溢出的情况：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773271933-08b6e23e-87b6-4874-8532-9bc3ee0970ba.png#clientId=ud01b60ee-4cb1-4&from=paste&id=u06372929&originHeight=394&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ucd6d1dd0-4408-4db0-8b6a-d587b650085&title=)<br />上面输出的数值是**累计值**，表示共有多少个 TCP 连接因为半连接队列溢出而被丢弃。**隔几秒执行几次，如果有上升的趋势，说明当前存在半连接队列溢出的现象**。
<a name="qdAg1"></a>
#### 大部分人都说 tcp_max_syn_backlog 是指定半连接队列的大小，是真的吗？
很遗憾，半连接队列的大小并不单单只跟 tcp_max_syn_backlog 有关系。<br />上面模拟 SYN 攻击场景时，服务端的 tcp_max_syn_backlog 的默认值如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773272010-5efbea20-9c60-4fd2-b877-9d21ab39665c.png#clientId=ud01b60ee-4cb1-4&from=paste&id=u57e05e31&originHeight=281&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ufd33a963-6cb1-426a-895c-68ead34b62c&title=)<br />但是在测试的时候发现，服务端最多只有 256 个半连接队列，而不是 512，所以**半连接队列的最大长度不一定由 tcp_max_syn_backlog 值决定的**。
<a name="Loc5w"></a>
#### 接下来，走进 Linux 内核的源码，来分析 TCP 半连接队列的最大值是如何决定的。
TCP 第一次握手（收到 SYN 包）的 Linux 内核代码如下，其中缩减了大量的代码，只需要重点关注 TCP 半连接队列溢出的处理逻辑：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773272133-2840695e-608c-45fc-8aab-2c09626340ef.png#clientId=ud01b60ee-4cb1-4&from=paste&id=ud0ad5f5f&originHeight=1621&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8038cb13-d68c-4061-87f4-1182877ef90&title=)<br />从源码中，可以得出共有三个条件因队列长度的关系而被丢弃的：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773272257-8e6f0d57-0c77-4c88-acfd-5a3dd25373d3.png#clientId=ud01b60ee-4cb1-4&from=paste&id=u80f98683&originHeight=1383&originWidth=858&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7fe3960d-d22a-452c-9713-c530c11c5f0&title=)

1. **如果半连接队列满了，并且没有开启 tcp_syncookies，则会丢弃；**
2. **若全连接队列满了，且没有重传 SYN+ACK 包的连接请求多于 1 个，则会丢弃；**
3. **如果没有开启 tcp_syncookies，并且 max_syn_backlog 减去 当前半连接队列长度小于 (max_syn_backlog >> 2)，则会丢弃；**

关于 tcp_syncookies 的设置，后面在详细说明，可以先给大家说一下，开启 tcp_syncookies 是缓解 SYN 攻击其中一个手段。<br />接下来，继续跟一下检测半连接队列是否满的函数 inet_csk_reqsk_queue_is_full 和 检测全连接队列是否满的函数 sk_acceptq_is_full ：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773272232-1ed0cb92-7d7a-4462-88e2-2899c89f3f7b.png#clientId=ud01b60ee-4cb1-4&from=paste&id=u24ff6128&originHeight=1107&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9d0d4252-a279-4ee8-b728-2f0d5600ebc&title=)<br />从上面源码，可以得知：

- **全**连接队列的最大值是 sk_max_ack_backlog 变量，sk_max_ack_backlog 实际上是在 listen() 源码里指定的，也就是 **min(somaxconn, backlog)**；
- **半**连接队列的最大值是 max_qlen_log 变量，max_qlen_log 是在哪指定的呢？现在暂时还不知道，继续跟进；

继续跟进代码，看一下是哪里初始化了半连接队列的最大值 max_qlen_log：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773272464-0a5175b2-de01-4101-8b26-1b7284b62527.png#clientId=ud01b60ee-4cb1-4&from=paste&id=ua749b900&originHeight=1695&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue6b6ba59-d1d7-4141-9bf8-5d08cb704df&title=)<br />从上面的代码中，可以算出 max_qlen_log 是 8，于是代入到 检测半连接队列是否满的函数 reqsk_queue_is_full ：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773272631-6d7daa91-3ff5-4f95-971a-57d727e37a1c.png#clientId=ud01b60ee-4cb1-4&from=paste&id=u5d97e023&originHeight=443&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6dd07a21-4b02-4852-9a77-56299cdb8e6&title=)<br />也就是 qlen >> 8 什么时候为 1 就代表半连接队列满了。这计算这不难，很明显是当 qlen 为 256 时，256 >> 8 = 1。<br />至此，总算知道为什么上面模拟测试 SYN 攻击的时候，服务端处于 SYN_RECV 连接最大只有 256 个。<br />可见，**半连接队列最大值不是单单由 max_syn_backlog 决定，还跟 somaxconn 和 backlog 有关系。**<br />在 Linux 2.6.32 内核版本，它们之间的关系，总体可以概况为：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773272694-25e6a9f5-4c19-48df-8213-1713292ea4ff.png#clientId=ud01b60ee-4cb1-4&from=paste&id=u29a59048&originHeight=553&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ufc343d16-11f0-49c8-85b6-c218b46b408&title=)<br />1. 当 max_syn_backlog > min(somaxconn, backlog) 时， 半连接队列最大值 max_qlen_log = min(somaxconn, backlog) * 2;<br />2. 当 max_syn_backlog < min(somaxconn, backlog) 时， 半连接队列最大值 max_qlen_log = max_syn_backlog * 2;
<a name="vI03l"></a>
#### 半连接队列最大值 max_qlen_log 就表示服务端处于 SYN_REVC 状态的最大个数吗？
依然很遗憾，并不是。<br />max_qlen_log 是**理论**半连接队列最大值，并不一定代表服务端处于 SYN_REVC 状态的最大个数。<br />在前面在分析 TCP 第一次握手（收到 SYN 包）时会被丢弃的三种条件：

1. 如果半连接队列满了，并且没有开启 tcp_syncookies，则会丢弃；
2. 若全连接队列满了，且没有重传 SYN+ACK 包的连接请求多于 1 个，则会丢弃；
3. **如果没有开启 tcp_syncookies，并且 max_syn_backlog 减去 当前半连接队列长度小于 (max_syn_backlog >> 2)，则会丢弃；**

假设条件 1 当前半连接队列的长度 「没有超过」理论的半连接队列最大值  max_qlen_log，那么如果条件 3 成立，则依然会丢弃 SYN 包，也就会使得服务端处于 SYN_REVC 状态的最大个数不会是理论值 max_qlen_log。<br />似乎很难理解，继续接着做实验，实验见真知。<br />服务端环境如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773272952-f82667cf-19bc-444a-91f2-d321a03d5220.png#clientId=ud01b60ee-4cb1-4&from=paste&id=u259c679d&originHeight=219&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u81c3b173-9d73-4477-a7ad-4ea14018170&title=)<br />配置完后，服务端要重启 Nginx，因为全连接队列最大和半连接队列最大值是在 listen() 函数初始化。<br />根据前面的源码分析，可以计算出半连接队列 max_qlen_log 的最大值为 256：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773272840-320072f1-341a-4719-b207-60a2bfc7f2a8.png#clientId=ud01b60ee-4cb1-4&from=paste&id=u424a4ce3&originHeight=362&originWidth=602&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u651e85b1-326d-438b-a7f1-8897961d45b&title=)<br />客户端执行 hping3 发起 SYN 攻击：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773272993-f7c588df-b55f-402c-ae39-a467a4581952.png#clientId=ud01b60ee-4cb1-4&from=paste&id=u64b891f3&originHeight=299&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4fb691a7-cf00-4950-8c35-5bd96b7a868&title=)<br />服务端执行如下命令，查看处于 SYN_RECV 状态的最大个数：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773273062-6cbcdf67-4118-4541-8e94-a2dccb7c60ef.png#clientId=ud01b60ee-4cb1-4&from=paste&id=u178da577&originHeight=311&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5024a51c-f8cf-4f02-b831-fc521a14555&title=)<br />可以发现，服务端处于 SYN_RECV 状态的最大个数并不是 max_qlen_log 变量的值。<br />这就是前面所说的原因：**如果当前半连接队列的长度 「没有超过」理论半连接队列最大值  max_qlen_log，那么如果条件 3 成立，则依然会丢弃 SYN 包，也就会使得服务端处于 SYN_REVC 状态的最大个数不会是理论值 max_qlen_log。**<br />来分析一波条件 3 :<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773273300-7ccac75d-86fe-4d7c-a736-85261731400d.png#clientId=ud01b60ee-4cb1-4&from=paste&id=ud5acb0da&originHeight=737&originWidth=572&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uedb6bea8-a470-4cb9-a328-06a224ccd21&title=)<br />从上面的分析，可以得知如果触发「当前半连接队列长度 > 192」条件，TCP 第一次握手的 SYN 包是会被丢弃的。<br />在前面测试的结果，服务端处于 SYN_RECV 状态的最大个数是 193，正好是触发了条件 3，所以处于 SYN_RECV 状态的个数还没到「理论半连接队列最大值 256」，就已经把 SYN 包丢弃了。<br />所以，服务端处于 SYN_RECV 状态的最大个数分为如下两种情况：<br />1. 如果「当前半连接队列」**没超过**「理论半连接队列最大值」，但是**超过** max_syn_backlog - (max_syn_backlog >> 2)，那么处于 SYN_RECV 状态的最大个数就是 max_syn_backlog - (max_syn_backlog >> 2)；<br />2. 如果「当前半连接队列」**超过**「理论半连接队列最大值」，那么处于 SYN_RECV 状态的最大个数就是「理论半连接队列最大值」；
<a name="QFx33"></a>
#### 每个 Linux 内核版本「理论」半连接最大值计算方式会不同。
在上面是针对 Linux 2.6.32 版本分析的「理论」半连接最大值的算法，可能每个版本有些不同。<br />比如在 Linux 5.0.0 的时候，「理论」半连接最大值就是全连接队列最大值，但依然还是有队列溢出的三个条件：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773273277-64c7752a-e7dd-4d38-8d38-aaa583caac53.png#clientId=ud01b60ee-4cb1-4&from=paste&id=u95dbedd7&originHeight=2458&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf3a18c68-ff40-4d2b-acc8-a3c9ecd6f91&title=)
<a name="ftWJ0"></a>
#### 如果 SYN 半连接队列已满，只能丢弃连接吗？
并不是这样，**开启 syncookies 功能就可以在不使用 SYN 半连接队列的情况下成功建立连接**，在前面源码分析也可以看到这点，当开启了  syncookies 功能就不会丢弃连接。<br />syncookies 是这么做的：服务器根据当前状态计算出一个值，放在己方发出的 SYN+ACK 报文中发出，当客户端返回 ACK 报文时，取出该值验证，如果合法，就认为连接建立成功，如下图所示。<br />![开启 syncookies 功能](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773273531-79362d15-4b97-424c-89bc-77c0231dfc13.png#clientId=ud01b60ee-4cb1-4&from=paste&id=u0015e20d&originHeight=392&originWidth=899&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u0c7e8d12-aba7-4106-a2c9-673738c62a9&title=%E5%BC%80%E5%90%AF%20syncookies%20%E5%8A%9F%E8%83%BD "开启 syncookies 功能")<br />syncookies 参数主要有以下三个值：

- 0 值，表示关闭该功能；
- 1 值，表示仅当 SYN 半连接队列放不下时，再启用它；
- 2 值，表示无条件开启功能；

那么在应对 SYN 攻击时，只需要设置为 1 即可：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773273465-ae28ab24-28bf-47a7-a518-7d12d07645b0.png#clientId=ud01b60ee-4cb1-4&from=paste&id=u46ad6ce5&originHeight=275&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9aade384-9981-4fc5-9af3-8fd31d72830&title=)
<a name="tucsC"></a>
#### 如何防御 SYN 攻击？
这里给出几种防御 SYN 攻击的方法：

- 增大半连接队列；
- 开启 tcp_syncookies 功能
- 减少 SYN+ACK 重传次数
<a name="UkKn4"></a>
##### 方式一：增大半连接队列
在前面源码和实验中，得知**要想增大半连接队列，不能只单纯增大 tcp_max_syn_backlog 的值，还需一同增大 somaxconn 和 backlog，也就是增大全连接队列**。否则，只单纯增大 tcp_max_syn_backlog 是无效的。<br />增大 tcp_max_syn_backlog 和 somaxconn 的方法是修改 Linux 内核参数：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773273432-7c39a0a8-c523-4845-8f8f-afa5e04e21d9.png#clientId=ud01b60ee-4cb1-4&from=paste&id=ud61faac0&originHeight=329&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5a1cf60f-dded-4ddc-a285-5364832cd77&title=)<br />增大 backlog 的方式，每个 Web 服务都不同，比如 Nginx 增大 backlog 的方法如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773273738-5f3c6b10-76b9-4537-938d-5f58ae2f0814.png#clientId=ud01b60ee-4cb1-4&from=paste&id=u37763df1&originHeight=496&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4d3dbc4a-99b3-4fa6-9c6e-c5c9929a5c9&title=)<br />最后，改变了如上这些参数后，要重启 Nginx 服务，因为半连接队列和全连接队列都是在 listen() 初始化的。
<a name="D6Cmo"></a>
##### 方式二：开启 tcp_syncookies 功能
开启 tcp_syncookies 功能的方式也很简单，修改 Linux 内核参数：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773273787-f3b99ab5-ee58-4d4b-a88d-3971130d4e27.png#clientId=ud01b60ee-4cb1-4&from=paste&id=u2b7c9851&originHeight=275&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4f570d48-37b3-4878-b8b9-cfd6b55e1ea&title=)
<a name="eha7X"></a>
##### 方式三：减少 SYN+ACK 重传次数
当服务端受到 SYN 攻击时，就会有大量处于 SYN_REVC 状态的 TCP 连接，处于这个状态的 TCP 会重传 SYN+ACK ，当重传超过次数达到上限后，就会断开连接。<br />那么针对 SYN 攻击的场景，可以减少 SYN+ACK 的重传次数，以加快处于 SYN_REVC 状态的 TCP 连接断开。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649773273914-e2acba82-fb97-4cb2-bebc-51a1d0ab9abe.png#clientId=ud01b60ee-4cb1-4&from=paste&id=u71d0fd77&originHeight=261&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue271cfd1-4c26-4dfe-9287-1d6e9df3893&title=)
