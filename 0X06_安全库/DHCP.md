如果知道某台电脑的IP，就可以向这个IP发起连接请求，建立连接后就可以操作收发数据。<br />![五层网络协议对应的消息体变化分析](https://cdn.nlark.com/yuque/0/2022/png/396745/1667876158796-7e88299d-ef6d-4fa1-a346-bbfd0d18e86a.png#averageHue=%23e0e6ea&clientId=u795b7ecf-e6f8-4&from=paste&id=u3e9ed613&originHeight=720&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=ud31eb29f-5958-4ae6-a248-3f22d8cfa9b&title=%E4%BA%94%E5%B1%82%E7%BD%91%E7%BB%9C%E5%8D%8F%E8%AE%AE%E5%AF%B9%E5%BA%94%E7%9A%84%E6%B6%88%E6%81%AF%E4%BD%93%E5%8F%98%E5%8C%96%E5%88%86%E6%9E%90 "五层网络协议对应的消息体变化分析")<br />要发送的数据，会在网络层里加入**IP头**。<br />![ip报头](https://cdn.nlark.com/yuque/0/2022/png/396745/1667876158872-81e74b19-107c-4860-9e5c-09b9f4833613.png#averageHue=%2386bea0&clientId=u795b7ecf-e6f8-4&from=paste&id=u6afc0b45&originHeight=450&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u219337e3-3d42-4865-89da-b048665786c&title=ip%E6%8A%A5%E5%A4%B4 "ip报头")<br />这里面最重要的是**发送端和接收端的IP地址**。这个IP地址就像是一个门牌号一样，有了它，数据包就能在这个纷繁复杂的网络世界里找到该由谁来接收这个数据包。<br />所以说上面的网络通信离不开IP。<br />假设有一台新买的电脑，**还没联网**呢，这时候拿着新买的网线，插入网线口，网线插口亮起来了。<br />然后就可以开始用它上网了。<br />那么问题来了。<br />**刚插上网线，电脑怎么知道自己的IP是什么？**怎么就突然能上网了呢？<br />这个话题，从**DHCP**聊起吧。
<a name="xW3FS"></a>
### DHCP是什么
插上网线之后，获得IP的方式主要有两种。<br />第一种是，自己**手动在电脑里配**。像下图那样，是macOS的一个截图，在选择**手动**配置之后，除了**IP地址**还需要配上**子网掩码**和**路由器的地址**。<br />![手动配IP](https://cdn.nlark.com/yuque/0/2022/png/396745/1667876158883-47b4c33b-f535-47f9-8e94-113d25bef3f1.png#averageHue=%23f5f3f2&clientId=u795b7ecf-e6f8-4&from=paste&id=u6b10705f&originHeight=827&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u3293c7e5-10ca-45cd-b2c0-41c8c255e3b&title=%E6%89%8B%E5%8A%A8%E9%85%8DIP "手动配IP")<br />这就很不科学了，电脑又不只是卖给程序员，这几个词对于大部分普通人来说，比**赋能抓手闭环**这种黑话还要难理解。<br />大部分人没事都不应该去配这玩意。<br />有没有办法可以让这些IP信息自动获得？<br />有，这就是**第二种**获取IP的方式，**DHCP**（**D**ynamic **H**ost **C**onfiguration **P**rotocol，**动态主机配置协议**）。<br />![DHCP自动生成IP](https://cdn.nlark.com/yuque/0/2022/png/396745/1667876158830-7a3cffa5-cba7-4120-84c3-6e969b90c43a.png#averageHue=%23f5f3f3&clientId=u795b7ecf-e6f8-4&from=paste&id=u0e5b811d&originHeight=839&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u27cf2e15-5c19-491f-afd4-f65eaed91e4&title=DHCP%E8%87%AA%E5%8A%A8%E7%94%9F%E6%88%90IP "DHCP自动生成IP")<br />通过DHCP，在联网之后可以自动获取到本机需要的**IP地址，子网掩码还有路由器地址**。
<a name="l4qUc"></a>
### DHCP的工作原理
DHCP的工作原理也非常简单。<br />说白了，就是向某个**管IP分配的服务器**，也就是**DHCP服务器**，申请IP地址。其实一般家里用的路由器就自带这个功能。<br />整个操作流程分为**4个**阶段。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667876159151-53f64717-73fc-4941-97c5-f00f5300ccd4.png#averageHue=%23e0e7ea&clientId=u795b7ecf-e6f8-4&from=paste&id=udccfbf9b&originHeight=291&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub2423418-a61d-427e-945f-11caf6bfd07&title=)<br />DHCP协议

- **DHCP Discover：**在联网时，本机由于没有IP，也不知道DHCP服务器的IP地址是多少，所以根本不知道该向谁发起请求，于是索性选择**广播**，向本地网段内所有人发出消息，询问"**谁能给个IP用用**"。
- **DHCP Offer：**不是DHCP服务器的机子会忽略你的广播消息，而DHCP服务器收到消息后，会在自己维护的一个IP池里拿出一个空闲IP，通过**广播**的形式给回你的电脑。
- **DHCP Request：**你的电脑在拿到IP后，再次发起**广播**，就说"这个IP我要了"。
- **DHCP ACK：**DHCP服务器此时再回复你一个ACK，意思是"ok的"。你就正式获得这个IP在**一段时间（比如24小时）**里的使用权了。后续**只要IP租约不过期**，就可以一直用这个IP进行通信了。

到这里，问题来了
<a name="uoPIG"></a>
#### 为什么要有第三和第四阶段
大家有没有发现，**在Offer阶段，其实你的机子就已经拿到了IP了，为什么还要有后面的Request和ACK呢？是不是有些多此一举？**<br />这是因为本地网段内，可能有**不止一台DHCP服务器**，在你广播之后，每个DHCP服务器都有可能给你发Offer。<br />本着先到先得的原则，你的机子一般会对第一个到的Offer响应DHCP Request，目的是为了确认offer，在你确认Offer这段时间内，DHCP服务器确认这个IP还没被分出去，你才可以安心使用这个IP。<br />**像不像你找工作的过程？**<br />你海投简历（**DHCP Discover**），然后拿到了多个offer(**DHCP Offer**)。<br />这时候事情还没完，你一般会跟HR说："**你给我两天时间，我要跟家里人商量下**"。<br />HR也会对你说："**那你尽快确认，我这边还有不少候选人等着**"。<br />之后你考虑下来觉得不错，跟HR说要接这个Offer（**DHCP Request**），HR看了下这个**岗位还在**，才能确认让你第二天来上班（**DHCP ACK**）。如果这个公司的岗位已经招到其他候选人了，第四阶段的消息就会改为发**DHCP NAK**，意思是拒绝了你的接Offer请求。
<a name="i174j"></a>
### DHCP抓包
光看原理是有些枯燥，可以尝试下抓包看下数据。<br />在命令行里执行下面的命令，可以强行让电脑的en0网卡重新走一遍DHCP流程。
```bash
sudo ipconfig set en0 DHCP
```
en0可以替换成其他网卡，比如eth0啥的。<br />这时候就可以抓到相关的数据包。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667876159231-d71f1752-e528-437c-8dac-78382da48519.png#averageHue=%23aaf5aa&clientId=u795b7ecf-e6f8-4&from=paste&id=u9384774c&originHeight=102&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4f73ac48-fea3-48bd-8efe-ecb0054b004&title=)<br />可以看到蓝色的四个数据包，分别对应上面提到的四个DHCP阶段。<br />其中第二阶段中的DHCP Offer里会返回给我们需要的**IP、子网掩码、路由器地址以及DNS服务器地址**。<br />![offer阶段](https://cdn.nlark.com/yuque/0/2022/png/396745/1667876159329-0cd0c7a3-38af-4d22-a5dc-a8276bea0419.png#averageHue=%23f3f2f2&clientId=u795b7ecf-e6f8-4&from=paste&id=ud95c3869&originHeight=622&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=ue7ae7268-336b-4adf-9d6e-c0efe68aeb7&title=offer%E9%98%B6%E6%AE%B5 "offer阶段")<br />另外，通过抓包，可以发现DHCP是应用层的协议，**基于传输层UDP协议**进行数据传输。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667876159345-f294b2ea-2e85-4667-9c69-de64ac0381ec.png#averageHue=%23f4ebd2&clientId=u795b7ecf-e6f8-4&from=paste&id=u455c44a9&originHeight=150&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ubf41a49d-c432-4d48-8a03-a10c7b1e57b&title=)<br />那么问题又来了。
<a name="DJ9Ia"></a>
### 为什么DHCP用UDP，能不能改用TCP？
按道理说，UDP能做到的，TCP一般也能做到。但**这次真不行**。<br />主要原因还是因为**TCP是面向连接的，而UDP是无连接的**。<br />所谓"连接"，他就只有一个发送端和一个接收端，就跟水管一样。<br />而DHCP由于一开始并不知道要跟谁建立连接，所以只能通过广播的形式发送消息，注意，小细节，**广播**。<br />![广播寻找DHCP服务器](https://cdn.nlark.com/yuque/0/2022/png/396745/1667876159377-ac5d83ec-2982-45b2-b78d-1612f8c6855b.png#averageHue=%23e7ecee&clientId=u795b7ecf-e6f8-4&from=paste&id=u0b99d891&originHeight=410&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u3f523fe5-21b9-45eb-8ecd-5fd796208fd&title=%E5%B9%BF%E6%92%AD%E5%AF%BB%E6%89%BEDHCP%E6%9C%8D%E5%8A%A1%E5%99%A8 "广播寻找DHCP服务器")<br />同样是在本地网段内发广播消息，UDP只需要发给255.255.255.255。**它实际上并不是值某个具体的机器，而是一个特殊地址**，这个地址有特殊含义，只要设了这个目的地址，就会在一定本地网段内进行**广播**。<br />而TCP却不同，它需要先建立连接，但实际上255.255.255.255对应的机器并不存在，因此也不能建立连接。如果同样要做到广播的效果，就需要先得到本地网段内所有机器的IP，然后**挨个建立连接，再挨个发消息**。这就很低效了。<br />**因此DHCP选择了UDP，而不是TCP**。
<a name="ChJLL"></a>
### 为什么第二阶段不是广播，而是单播。
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667876159600-5c61af02-7e4c-4bc4-8892-a5dd953298d4.png#averageHue=%23f2ead6&clientId=u795b7ecf-e6f8-4&from=paste&id=ue165fc72&originHeight=185&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u95cd9097-d105-43f9-badd-c691eba01ab&title=)<br />另外一个小细节不知道大家注意到没，上面在提到 DHCP Offer 阶段时，提到的是DHCP服务器会**使用广播的形式**回复。但抓个包下来却**发现并不是广播，而是单播**。<br />其实，这是DHCP协议的一个**小优化**。原则上大家在DHCP offer阶段，都用广播，那肯定是最稳的，**目标机器**收到后自然就会进入第三阶段DHCP Request。而**非目标机器**，收到后解包后发现目的机器的mac地址跟自己的不同，也会丢掉这个包。<br />但是问题就出在，这个非目的机器需要每次都在网卡**收到包，并解完包**，才发现原来这不是给它的消息，这。。。真，**有被打扰到。**<br />如果本地网段内这样的包满天飞，也浪费机器性能。<br />如果能用单播，那当然是最好的。但这时候目的机器其实并没有IP地址，有些系统在这种情况下能收单播包，有些则认为不能收，这个跟系统的实现有关。因此，对于能收单播包的系统，会在发**DHCP Discover**阶段设一个 **Broadcast flag = 0 (unicast)** 的标志位，告诉服务器，支持单播回复，于是服务器就会在**DHCP Offer阶段以单播的形式进行回复**。<br />![Discover要求使用单播回复](https://cdn.nlark.com/yuque/0/2022/png/396745/1667876160067-fb1c9ba7-197f-400e-a979-c89d4ea3c43a.png#averageHue=%23e3dedd&clientId=u795b7ecf-e6f8-4&from=paste&id=ua7a3a23d&originHeight=370&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=ud4c5ac8a-63c4-4083-9627-f0666e8e87a&title=Discover%E8%A6%81%E6%B1%82%E4%BD%BF%E7%94%A8%E5%8D%95%E6%92%AD%E5%9B%9E%E5%A4%8D "Discover要求使用单播回复")
<a name="axEhw"></a>
### 是不是每次联网都要经历DHCP四个阶段？
只要想联网，就需要IP，要用IP，就得走DHCP协议去分配。<br />但大家也发现了，DHCP第一阶段和第二阶段都可能会发**广播**消息。对于家用电脑还好，插个网线，之后就雷打不动。但像手机这样的**移动设备**，是要带着到处跑的，坐个地铁，进个电梯，公司里到处走走，都可能会涉及到网络切换。<br />这每次都要来一个完整的四阶段，各种广播消息满天飞，其实对网络环境不太友好。<br />于是问题叒来了，**是不是每次联网都要经历DHCP四个阶段？**<br />**当然不需要。**<br />可以发现每次断开wifi再打开wifi时，机子会经历一个**从没网到有网**的过程。<br />这时候去抓个包，会发现。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667876159771-5bfbe2b7-1553-42aa-b141-f55b6c1f7b0b.png#averageHue=%23abf5ab&clientId=u795b7ecf-e6f8-4&from=paste&id=u7a04a5a2&originHeight=92&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9519d922-b50a-4a6b-bf04-0f459a45192&title=)<br />其实只发生了DHCP的第三和第四阶段。这是因为机子记录了曾经使用过 192.168.31.170这个IP，重新联网后，会优先再次请求这个IP，这样就省下了第一第二阶段的广播了。<br />另外需要注意的是，抓包图里DHCP Request之所以出现两次，是因为**第一次Request发出后太久没得到回应，因此重发。**
<a name="pv2Dd"></a>
### DHCP分配下来的IP一定不会重复吗？
一般来说DHCP服务器会在它维护的IP池里找到一个没人用的IP分配给机子，<br />这个IP如果重复分配了，那本地网段内就会出现两个**同样的IP**，这个IP下面却对应两个**不同的mac地址**。但其他机器上的**ARP缓存**中却只会记录其中一条**mac地址到IP的映射关系**。<br />于是，数据在传递的过程中就会出错。<br />**因此本地网段内IP必须唯一。**<br />**那么DHCP分配下来的IP有没有可能跟别的IP是重复的？**<br />都这么问了，那肯定是可能的。<br />有两个常见的情况会出现**IP重复**。

- 文章开头提到，IP是可以自己手动配的，自己配的IP是有可能跟其他DHCP分配下来的IP是相同的。解决方案也很简单，尽量不要手动去配IP，统一走DHCP。或者在DHCP服务器里维护的IP范围里，将这条IP剔除。
- 一个本地网段内，是可以有多个DHCP服务器的，而他们维护的**IP地址范围是有可能重叠的**，于是就有可能将相同的IP给到不同的机子。解决方案也很简单，修改两台DHCP服务器的维护的IP地址范围，让它们不重叠就行了。

不过吧，上面的解决方案，都需要有权限去修改**DHCP服务器**。
<a name="RRNU5"></a>
#### 得到**DHCP ACK**之后立马就能使用这个IP了吗？
这就好像在问，拿到offer之后你是第一时间就去上班吗？<br />不。<br />你会先告诉你的同事同学朋友，甚至会发朋友圈。<br />你的机子也一样。<br />在得到**DHCP ACK**之后，机子不会立刻就用这个IP。<br />而是会先发**三条ARP消息**。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667876159881-e9769bb8-a221-41e7-beca-5a15363b200a.png#averageHue=%23f2ede8&clientId=u795b7ecf-e6f8-4&from=paste&id=u52a98cd1&originHeight=338&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u686e837b-0596-4e66-bc1f-f3f05f52485&title=)<br />大家知道**ARP消息的目的是通过IP地址去获得mac地址**。所以**普通的ARP消息里，是填了IP地址，不填mac地址的**。<br />但这三条ARP协议，比较特殊，它们叫**无偿ARP**（Gratuitous ARP），特点是它会把**IP和mac地址都填好了**，而且**填的还是自己的IP和mac地址**。<br />目的有两个。

- 一个是为了告诉本地网段内所有机子，从现在起，xx IP地址属于xx mac地址，让大家记录在ARP缓存中。
- 另一个就是看下本地网段里有没有其他机子也用了这个IP，如果有冲突的话，那需要重新再走一次DHCP流程。

**在三次无偿ARP消息之后，确认没有冲突了，才会开始使用这个IP地址进行通信。**<br />这种行为，实际上就跟你拿了offer之后**发了这么个朋友圈**没啥区别。<br />而且，**还连发了三条**。<br />秀offer，offer冲突了不可怕。秀对象秀冲突了才可怕。
<a name="ZYTpA"></a>
### 总结

- 电脑插上网线，联网后会通过DHCP协议动态申请一个IP，同时获得子网掩码，路由器地址等信息。
- DHCP分为四个阶段，分别是 Discover，Offer， Request和ACK。如果曾经连过这个网，机器会记录你上次使用的IP，再次连接时优先使用原来的那个IP，因此只需要经历第三第四阶段。
- DHCP是应用层协议，考虑到需要支持广播功能，底层使用的是UDP协议，而不是TCP协议。
- DHCP分配下来的IP是有可能跟某台手动配置的IP地址重复的。
- DHCP得到IP之后还会发3次无偿ARP通告，在确认没有冲突后开始使用这个IP。

最后留个问题。上面的IP都是从DHCP服务器上申请的，在服务器返回DHCP Offer的时候，可以看到上面写了DHCP服务器的IP。比如192.168.31.1，这明显是个局域网内的IP，但这能说明，你的DHCP服务器一定在这个局域网里吗？<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667876160291-b532b6aa-8dc6-4244-a156-07d4b3a9dd30.png#averageHue=%23f0efef&clientId=u795b7ecf-e6f8-4&from=paste&id=u2eb934be&originHeight=750&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud6af4e54-623f-4dc6-abc4-ae3041db34e&title=)
