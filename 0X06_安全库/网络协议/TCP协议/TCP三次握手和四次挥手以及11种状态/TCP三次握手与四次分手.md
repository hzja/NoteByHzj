网络协议 TCP
<a name="gFUKw"></a>
## TCP协议简介
TCP协议是五层协议中运输层的协议，下面依赖网络层、链路层、物理层，对于一个报文想发到另一台机器(假设是服务器)上对等层，每一个所依赖的层都会对报文进行包装，例如TCP协议就依赖网络层的IP协议，所以发送的报文会经过如下封装:<br />![TCP协议](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641614106238-7eff4b4b-f0ca-46be-aaee-c5e4fb8e8e5a.webp#clientId=u1adb13ef-fd7a-4&from=paste&id=ua92b7b8a&originHeight=151&originWidth=663&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u99ee5da7-4281-48e2-9287-90869848842&title=TCP%E5%8D%8F%E8%AE%AE "TCP协议")<br />当这个数据包到达服务器时，服务器的网络层会对IP相关协议内容解封装、校验，然后运输层对TCP层进行解封，解封涉及到一系列的步骤，例如这个数据包是要干嘛？是发给我的吗?这些操作需要根据 TCP 报文的首部信息来判断，首部包含以下内容：<br />![TCP 报文的首部信息](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641614106187-b3511f42-6cd0-48b2-91fe-b9eab3395b21.webp#clientId=u1adb13ef-fd7a-4&from=paste&id=u8a35b8d5&originHeight=571&originWidth=653&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u163cedf3-5ca6-452d-b7d8-3e52382aee4&title=TCP%20%E6%8A%A5%E6%96%87%E7%9A%84%E9%A6%96%E9%83%A8%E4%BF%A1%E6%81%AF "TCP 报文的首部信息")<br />主要通过首部信息来了解这个包是干嘛的,关于首部信息,这儿需要用到的几个如下:

- ACK：TCP协议规定，只有ACK=1时有效，也规定连接建立后所有发送的报文的ACK必须为1
- SYN(SYNchronization)：在连接建立时用来同步序号。当SYN=1而ACK=0时，表明这是一个连接请求报文。对方若同意建立连接，则应在响应报文中使SYN=1和ACK=1. 因此, SYN置1就表示这是一个连接请求或连接接受报文。
- FIN （finis）：即完，终结的意思， 用来释放一个连接。当 FIN = 1 时，表明此报文段的发送方的数据已经发送完毕，并要求释放连接。

「注意」 ：·URG、ACK、PSH、PST、RST、SYN、FIN只有一位，也就是只有 0 或者 1 两种状态。
<a name="tDfiL"></a>
## TCP协议三次握手
![TCP协议三次握手](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641614106280-5da681ba-7bd0-4194-a00a-e3a0368d2089.webp#clientId=u1adb13ef-fd7a-4&from=paste&id=u7d5033f4&originHeight=976&originWidth=875&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=udef45d71-57cd-467f-8b05-24ec0fb1ae0&title=TCP%E5%8D%8F%E8%AE%AE%E4%B8%89%E6%AC%A1%E6%8F%A1%E6%89%8B "TCP协议三次握手")<br />「第一次握手」 ：客户端先向服务端发送一个请求连接的报文段，这个报文段SYN位设置为1,序列号Seq(Sequence Number)设置为某一值，假设为X，发送出去之后客户端进入SYN_SEND状态，等待服务器的确认；<br />「第二次握手」 ：服务器收到SYN报文段。服务器收到客户端的SYN报文段，需要对这个SYN报文段进行确认，设置Acknowledgment Number为x+1(Sequence Number+1)；同时，自己自己还要发送SYN请求信息，将SYN位置为1，Sequence Number为y；服务器端将上述所有信息放到一个报文段（即SYN+ACK报文段）中，一并发送给客户端，此时服务器进入SYN_RECV状态；<br />「第三次握手」 ：客户端收到服务器的SYN+ACK报文段。然后将Acknowledgment Number设置为y+1，向服务器发送ACK报文段，这个报文段发送完毕以后，客户端和服务器端都进入ESTABLISHED状态，完成TCP三次握手。<br />完成了三次握手，客户端和服务器端就可以开始传送数据。以上就是TCP三次握手的总体介绍。
<a name="qegvr"></a>
## 为什么要三次握手而不是两次?
为什么非要进行三次连接呢？两次行吗?在谢希仁的《计算机网络》中是这样说的：<br />为了防止已失效的连接请求报文段突然又传送到了服务端，因而产生错误。如下：
> “已失效的连接请求报文段”的产生在这样一种情况下：client发出的第一个连接请求报文段并没有丢失，而是在某个网络结点长时间的滞留了，以致延误到连接释放以后的某个时间才到达server。本来这是一个早已失效的报文段。但server收到此失效的连接请求报文段后，就误认为是client再次发出的一个新的连接请求。于是就向client发出确认报文段，同意建立连接。假设不采用“三次握手”，那么只要server发出确认，新的连接就建立了。由于现在client并没有发出建立连接的请求，因此不会理睬server的确认，也不会向server发送数据。但server却以为新的运输连接已经建立，并一直等待client发来数据。这样，server的很多资源就白白浪费掉了。采用“三次握手”的办法可以防止上述现象发生。例如刚才那种情况，client不会向server的确认发出确认。server由于收不到确认，就知道client并没有要求建立连接。”，防止了服务器端的一直等待而浪费资源。

<a name="Q3Jwa"></a>
## 第三次握手失败了怎么办?
在tcp三次握手中 第二次握手完成后connect 就成功返回了 如果第三次握手的ack包丢了 此时 客户端已认为连接是成功的,如果没有应用层的心跳包,客户端会一直维护这个连接 请问如何避免这种情况？<br />第二次握手服务器收到SYN包,然后发出SYN+ACK数据包确认收到并且请求建立连接，服务器进入SYN_RECV状态。而这个时候第三次握手时客户端发送ACK给服务器失败了，服务器没办法进入ESTABLISH状态，这个时候肯定不能传输数据的，不论客户端主动发送数据与否，服务器都会有定时器发送第二步SYN+ACK数据包，如果客户端再次发送ACK成功，建立连接。<br />如果一直不成功，服务器肯定会有超时（大概64s）设置，超时之后会给客户端发**「RTS报文」** (连接重置)，进入CLOSED状态，防止SYN洪泛攻击，这个时候客户端应该也会关闭连接。<br />「SYN洪泛攻击：」<br />SYN攻击利用的是TCP的三次握手机制，攻击端利用伪造的IP地址向被攻击端发出请求，而被攻击端发出的响应 报文将永远发送不到目的地，那么**「被攻击端在等待关闭这个连接的过程中消耗了资源」** ，如果有成千上万的这种连接，主机资源将被耗尽，从而达到攻击的目的。
<a name="wSFwy"></a>
## TCP协议四次分手
还是这个图镇帖：<br />![TCP协议四次分手](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641614106268-48ebfbec-3eb9-425b-9d02-9828b325c4d2.webp#clientId=u1adb13ef-fd7a-4&from=paste&id=u6496e4ce&originHeight=976&originWidth=875&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u0f5c8047-7e51-48d1-a1d8-80a9dbe815c&title=TCP%E5%8D%8F%E8%AE%AE%E5%9B%9B%E6%AC%A1%E5%88%86%E6%89%8B "TCP协议四次分手")<br />四次分手，意思是某一端（可以使客户端，也可以是服务器端）想结束会话断开连接，那么具体流程是：<br />「第一次分手」 ：主机1，设置序列号Seq(Sequence Number)和确认包ACK(Acknowledgment Number)，假设seq为x+2,ACK=y+1,再将FIN标志位设置为1,向主机2发送FIN报文段；之后主机1进入FIN_WAIT_1状态；这表示主机1没有数据要发送给主机2了；<br />「第二次分手」 ：主机2收到了主机1发送的FIN报文段，向主机1回一个ACK报文段(其值为接收到的FIN报文的seq值+1)；主机1进入FIN_WAIT_2状态,等待主机二的断开请求包FIN；<br />「第三次分手」 ：主机2向主机1发送FIN报文段，意思是我可以断开连接了,请求关闭连接，同时主机2进入CLOSE_WAIT状态；<br />「第四次分手」 ：主机1收到主机2发送的FIN报文段，向主机2发送ACK报文段,值为刚刚接收到的FIN包Seq值+1，然后主机1进入TIME_WAIT状态；主机2收到主机1的ACK报文段以后，就关闭连接；此时，主机1等待2MSL后依然没有收到回复，则证明Server端已正常关闭，那好，主机1也可以关闭连接了。
<a name="vbHSN"></a>
## 为什么要四次挥手
TCP是全双工模式，这就意味着，当主机1发出FIN报文段时，只是表示主机1已经没有数据要发送了，主机1告诉主机2，它的数据已经全部发送完毕了；但是，这个时候主机1还是可以接受来自主机2的数据；当主机2返回ACK报文段时，表示它已经知道主机1没有数据发送了，但是主机2还是可以发送数据到主机1的；当主机2也发送了FIN报文段时，这个时候就表示主机2也没有数据要发送了，就会告诉主机1，我也没有数据要发送了，之后彼此就会愉快的中断这次TCP连接。如果要正确的理解四次分手的原理，就需要了解四次分手过程中的状态变化。
<a name="kiVyR"></a>
## 四次挥手状态解释
「FIN_WAIT_1」 : 这个状态要好好解释一下，其实FIN_WAIT_1和FIN_WAIT_2状态的真正含义都是表示等待对方的FIN报文。而这两种状态的区别是：FIN_WAIT_1状态实际上是当SOCKET在ESTABLISHED状态时，它想主动关闭连接，向对方发送了FIN报文，此时该SOCKET即进入到FIN_WAIT_1状态。「也就是,发出FIN包之后进入FIN_WAIT_1状态」<br />而当对方回应ACK报文后，则进入到FIN_WAIT_2状态，当然在实际的正常情况下，无论对方何种情况下，都应该马上回应ACK报文，所以FIN_WAIT_1状态一般是比较难见到的，而FIN_WAIT_2状态还有时常常可以用netstat看到。「也就是,发出ACK报文之后进入FIN_WAIT_2状态」<br />「主动方FIN_WAIT_2」 ：上面已经详细解释了这种状态，实际上FIN_WAIT_2状态下的SOCKET，表示半连接，也即有一方要求close连接，但另外还告诉对方，我暂时还有点数据需要传送给你(ACK信息)，稍后再关闭连接。<br />「主动方CLOSE_WAIT」 ：这种状态的含义其实是表示在等待关闭。怎么理解呢？当对方close一个SOCKET后发送FIN报文给自己，你系统毫无疑问地会回应一个ACK报文给对方，此时则进入到CLOSE_WAIT状态。接下来呢，实际上你真正需要考虑的事情是察看你是否还有数据发送给对方，如果没有的话，那么你也就可以 close这个SOCKET，发送FIN报文给对方，也即关闭连接。所以你在CLOSE_WAIT状态下，需要完成的事情是等待你去关闭连接。<br />「被动方LAST_ACK」 : 这个状态还是比较容易好理解的，它是被动关闭一方在发送FIN报文后，最后等待对方的ACK报文。当收到ACK报文后，也即可以进入到CLOSED可用状态了。「也就是接收到了对方的FIN包,自己发出了ACK以及FIN包之后的状态」<br />「被动方TIME_WAIT」 : 表示收到了对方的FIN报文，并发送出了ACK报文，就等2MSL后即可回到CLOSED可用状态了。如果FIN_WAIT1状态下，收到了对方同时带FIN标志和ACK标志的报文时，可以直接进入到TIME_WAIT状态，而无须经过FIN_WAIT_2状态。<br />「主动方CLOSED」 : 表示连接中断。
<a name="aUx1X"></a>
## TCP状态图
![TCP状态图](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641614106267-acb9c2a8-a026-4554-9446-6e59a15e2b73.webp#clientId=u1adb13ef-fd7a-4&from=paste&id=u86946b9d&originHeight=829&originWidth=826&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ufcf85a06-9601-41dd-bf8b-84be969d09f&title=TCP%E7%8A%B6%E6%80%81%E5%9B%BE "TCP状态图")
<a name="FFMA1"></a>
## 状态图解释

1. CLOSED：起始点，在超时或者连接关闭时候进入此状态。
2. LISTEN：svr端在等待连接过来时候的状态，svr端为此要调用socket， bind,listen函数，就能进入此状态。此称为应用程序被动打开（等待客户端来连接）。
3. SYN_SENT:客户端发起连接，发送SYN给服务器端。如果服务器端不能连接，则直接进入CLOSED状态。
4. SYN_RCVD：跟3对应，服务器端接受客户端的SYN请求，服务器端由LISTEN状态进入SYN_RCVD状态。同时服务器端要回应一个ACK，同时发送一个SYN给客户端；另外一种情况，客户端在发起SYN的同时接收到服务器端得SYN请求，客户端就会由SYN_SENT到SYN_RCVD状态。
5. ESTABLISHED：服务器端和客户端在完成3次握手进入状态，说明已经可以开始传输数据了。以上是建立连接时服务器端和客户端产生的状态转移说明。相对来说比较简单明了，如果你对三次握手比较熟悉，建立连接时的状态转移还是很容易理解。接下来服务器端和客户端就进行数据传输。当然，里面也大有学问，就此打住，稍后再表。下面，我们来看看连接关闭时候的状态转移说明，关闭需要进行4次双方的交互，还包括要处理一些善后工作（TIME_WAIT状态），注意，这里主动关闭的一方或被动关闭的一方不是指特指服务器端或者客户端，是相对于谁先发起关闭请求来说的。
6. FIN_WAIT_1:主动关闭的一方，由状态5进入此状态。具体的动作时发送FIN给对方。
7. FIN_WAIT_2:主动关闭的一方，接收到对方的FIN ACK，进入此状态。由此不能再接收对方的数据。但是能够向对方发送数据。
8. CLOSE_WAIT：接收到FIN以后，被动关闭的一方进入此状态。具体动作时接收到FIN，同时发送ACK。
9. LAST_ACK：被动关闭的一方，发起关闭请求，由状态8进入此状态。具体动作时发送FIN给对方，同时在接收到ACK时进入CLOSED状态。
10. CLOSING：两边同时发起关闭请求时，会由FIN_WAIT_1进入此状态。具体动作是，接收到FIN请求，同 时响应一个ACK。
11. TIME_WAIT：最纠结的状态来了。从状态图上可以看出，有3个状态可以转化成它，我们一一来分析：
   1. 由FIN_WAIT_2进入此状态：在双方不同时发起FIN的情况下，主动关闭的一方在完成自身发起的关闭请求后，接收到被动关闭一方的FIN后进入的状态。
   2. 由CLOSING状态进入:双方同时发起关闭，都做了发起FIN的请求，同时接收到了FIN并做了ACK的情况下，由CLOSING状态进入。
   3. 由FIN_WAIT_1状态进入：同时接受到FIN（对方发起），ACK（本身发起的FIN回应），与b的区别在于本身发起的FIN回应的ACK先于对方的FIN请求到达，而b是FIN先到达。这种情况概率最小。

关闭的4次连接最难理解的状态是TIME_WAIT，存在TIME_WAIT的 2 个理由：

1. 可靠地实现TCP全双工连接的终止。
2. 允许老的重复分节在网络中消逝。
