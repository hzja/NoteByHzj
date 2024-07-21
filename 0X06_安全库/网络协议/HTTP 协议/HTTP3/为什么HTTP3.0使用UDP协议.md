<a name="Je8hd"></a>
## 1、本文内容
- HTTP2.0和TCP存在的一些问题
- QUIC协议为什么选择UDP
- QUIC协议的重要特性
- HTTP3.0和QUIC协议的前景和应用效果
<a name="Dtvnt"></a>
## 2、HTTP2.0和HTTP3.0
互联网中业务是不断迭代前进的，像HTTP这种重要的网络协议也是如此，**新版本是对旧版本的扬弃**。
<a name="aRfsF"></a>
### 2.1 HTTP2.0和TCP的爱恨纠葛
HTTP2.0是2015年推出的，还是比较年轻的，其重要的**二进制分帧协议**、**多路复用**、**头部压缩**、**服务端推送**等重要优化使HTTP协议真正上了一个新台阶。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1699844388557-1847089d-59fe-48b8-9fe8-7b070e16bbdc.png#averageHue=%23f9f7f5&clientId=ufd0b5589-0ced-4&from=paste&id=u00d67194&originHeight=962&originWidth=828&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub590ab8c-db00-4aad-a740-5eeaa9dceb9&title=)<br />像谷歌这种重要的公司并没有满足于此，而且想继续提升HTTP的性能，花最少的时间和资源获取极致体验。<br />那么肯定要问HTTP2.0虽然性能已经不错了，还有什么不足吗？

- 建立连接时间长(本质上是TCP的问题)
- 队头阻塞问题
- 移动互联网领域表现不佳(弱网环境)
- ......

熟悉HTTP2.0协议的同学应该知道，这些缺点基本都是由于TCP协议引起的，**水能载舟亦能覆舟**，其实TCP也很无辜呀！<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1699844388556-181bc34d-7fdf-48d8-aeb1-a7f4f871432d.png#averageHue=%23fbfaf8&clientId=ufd0b5589-0ced-4&from=paste&id=u872299b2&originHeight=748&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0bae3c74-6bd3-4163-a359-312dc1f2497&title=)<br />TCP是面向连接、可靠的传输层协议，当前几乎所有重要的协议和应用都是基于TCP来实现的。<br />网络环境的改变速度很快，但是TCP协议相对缓慢，正是这种矛盾促使谷歌做出了一个看似**出乎意料的决定**-基于UDP来开发新一代HTTP协议。
<a name="MBZoc"></a>
### 2.2 谷歌为什么选择UDP
上文提到，谷歌选择UDP是**看似出乎意料**的，仔细想一想其实很有道理。<br />单纯地看看TCP协议的不足和UDP的一些优点：

- 基于TCP开发的设备和协议非常多，兼容困难
- TCP协议栈是Linux内部的重要部分，修改和升级成本很大
- UDP本身是无连接的、没有建链和拆链成本
- UDP的数据包无队头阻塞问题
- UDP改造成本小

从上面的对比可以知道，谷歌要想**从TCP上进行改造升级绝非易事**，但是UDP虽然没有TCP为了保证可靠连接而引发的问题，但是**UDP本身不可靠，又不能直接用**。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1699844388554-1edd0e51-389b-486f-88b3-b5247305875c.png#averageHue=%23f9f7f1&clientId=ufd0b5589-0ced-4&from=paste&id=u5414e93d&originHeight=378&originWidth=548&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf9ec44a2-5dfc-4906-9302-e1d893b41ed&title=)<br />综合而知，谷歌决定在**UDP基础上改造一个具备TCP协议优点的新协议**也就顺理成章了，这个新协议就是**QUIC协议**。
<a name="ypYfS"></a>
### 2.3 QUIC协议和HTTP3.0
QUIC其实是**Quick UDP Internet Connections**的缩写，直译为快速UDP互联网连接。<br />来看看百科对于QUIC协议的一些介绍：
> QUIC协议最初由Google的Jim Roskind设计，实施并于2012年部署，在2013年随着实验的扩大而公开宣布，并向IETF进行了描述。

QUIC提高了当前正在使用TCP的面向连接的Web应用程序的性能。它在两个端点之间使用用户数据报协议（UDP）建立多个复用连接来实现此目的。<br />QUIC的次要目标包括减少连接和传输延迟，在每个方向进行带宽估计以避免拥塞。它还将拥塞控制算法移动到用户空间，而不是内核空间，此外使用前向纠错（FEC）进行扩展，以在出现错误时进一步提高性能。<br />HTTP3.0又称为**HTTP Over QUIC**，其**弃用TCP协议**，改为使用**基于UDP协议**的QUIC协议来实现。

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1699844388577-21784ee6-df49-44e4-8468-3ad5f77ed596.png#averageHue=%23eee9b7&clientId=ufd0b5589-0ced-4&from=paste&id=udd27e5d8&originHeight=306&originWidth=357&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5bf5f9e1-0e8b-40f9-9aa6-33447568be0&title=)
<a name="zCBmP"></a>
## 3、QUIC协议详解
**择其善者而从之，其不善者而改之。**<br />HTTP3.0既然选择了QUIC协议，也就意味着HTTP3.0基本继承了HTTP2.0的强大功能，并且进一步解决了HTTP2.0存在的一些问题，同时必然引入了新的问题。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1699844388590-7ef6a3d6-7e00-4d37-a90d-d3cea3fcd4a5.png#averageHue=%23f9f7f4&clientId=ufd0b5589-0ced-4&from=paste&id=u4963526e&originHeight=305&originWidth=778&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc17cdd8d-0887-440f-ad78-c6b40e385b3&title=)<br />QUIC协议必须要实现HTTP2.0在TCP协议上的重要功能，同时解决遗留问题，来看看QUIC是如何实现的。
<a name="JNzmI"></a>
### 3.1 队头阻塞问题
**队头阻塞** Head-of-line blocking（缩写为HOL blocking）是计算机网络中是一种性能受限的现象，通俗来说就是：一个数据包影响了一堆数据包，它不来大家都走不了。<br />队头阻塞问题可能**存在于HTTP层和TCP层**，在HTTP1.x时两个层次都存在该问题。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1699844389503-e3429e5b-ec3a-4b85-a55a-d0395213f2dc.png#averageHue=%23f5f2f2&clientId=ufd0b5589-0ced-4&from=paste&id=u8376370b&originHeight=444&originWidth=812&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ufcb57098-17ac-4378-97cb-c13619b5f28&title=)<br />HTTP2.0协议的**多路复用机制**解决了HTTP层的队头阻塞问题，但是在**TCP层仍然存在队头阻塞问题**。<br />TCP协议在收到数据包之后，这部分数据可能是乱序到达的，但是TCP必须将所有数据收集排序整合后给上层使用，**如果其中某个包丢失了，就必须等待重传，从而出现某个丢包数据阻塞整个连接的数据使用**。<br />QUIC协议是基于UDP协议实现的，在一条链接上可以有**多个流**，流与流之间是**互不影响**的，当一个流出现丢包影响范围非常小，从而解决队头阻塞问题。
<a name="AGRNh"></a>
### 3.2 0RTT 建链
衡量网络建链的常用指标是**RTT** Round-Trip Time，也就是数据包一来一回的时间消耗。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1699844390805-c09c07cb-9e52-4c7a-950a-a1e37dba51f5.png#averageHue=%23f8f3f0&clientId=ufd0b5589-0ced-4&from=paste&id=u68730ef4&originHeight=226&originWidth=372&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u66bf92bb-d695-4689-ae2b-33b437627b8&title=)<br />RTT包括三部分：**往返传播时延**、**网络设备内排队时延**、**应用程序数据处理时延**。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1699844390826-63850a3e-bba3-47bc-92e4-9c5ea89d5d38.png#averageHue=%23faf9f9&clientId=ufd0b5589-0ced-4&from=paste&id=u0416e5f7&originHeight=239&originWidth=579&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2e147c4e-8a36-4b33-877d-5a45789454d&title=)<br />一般来说HTTPS协议要建立完整链接包括:**TCP握手**和**TLS握手**，总计需要至少2-3个RTT，普通的HTTP协议也需要至少1个RTT才可以完成握手。<br />然而，**QUIC协议可以实现在第一个包就可以包含有效的应用数据**，从而实现0RTT，但这也是有条件的。<br />![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1699844390901-68d6d25f-ef5a-4b62-9284-dc5798c792dd.gif#averageHue=%23f9f9f9&clientId=ufd0b5589-0ced-4&from=paste&id=u9d03cc96&originHeight=381&originWidth=600&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u56769eef-4b1a-4b6d-b3eb-9ff1d61e35d&title=)<br />简单来说，基于TCP协议和TLS协议的HTTP2.0在真正发送数据包之前需要花费一些时间来完成握手和加密协商，完成之后才可以真正传输业务数据。<br />但是QUIC则**第一个数据包就可以发业务数据**，从而在连接延时有很大优势，可以节约数百毫秒的时间。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1699844390961-6620f995-7c74-4c6a-8f1a-1ab75072a0b5.png#averageHue=%23f4f4ec&clientId=ufd0b5589-0ced-4&from=paste&id=u3d76e0d9&originHeight=530&originWidth=748&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u1fb14c9f-44d1-4232-b628-275ad456029&title=)<br />QUIC的**0RTT也是需要条件的**，对于第一次交互的客户端和服务端0RTT也是做不到的，毕竟双方完全陌生。<br />因此，QUIC协议可以分为**首次连接**和**非首次连接**，两种情况进行讨论。
<a name="OzHel"></a>
### 3.3 首次连接和非首次连接
使用QUIC协议的客户端和服务端要使用1RTT进行**密钥交换**，使用的交换算法是DH(Diffie-Hellman)**迪菲-赫尔曼算法**。<br />DH算法开辟了密钥交换的新思路，在之前的文章中提到的RSA算法也是基于这种思想实现的，但是DH算法和RSA的密钥交换不完全一样，感兴趣的读者可以看看DH算法的数学原理。<br />DH算法开辟了密钥交换的新思路，在之前的文章中提到的RSA算法也是基于这种思想实现的，但是DH算法和RSA的密钥交换不完全一样，感兴趣的读者可以看看DH算法的数学原理。
<a name="hwrnS"></a>
#### 3.3.1 首次连接
简单来说一下，首次连接时**客户端和服务端的密钥协商和数据传输过程**，其中涉及了DH算法的基本过程：

1. 客户端对于首次连接的服务端先发送client hello请求。
2. 服务端生成一个素数p和一个整数g，同时生成一个随机数 ![](https://cdn.nlark.com/yuque/__latex/3fd9b0ec94190f4214679e257baa5a2d.svg#card=math&code=K_%7Bs%5C_pri%7D&id=Zmpog)为私钥，然后计算出公钥![](https://cdn.nlark.com/yuque/__latex/8757f5f4c0ccfc0aef3eba107f5cf07a.svg#card=math&code=K_%7Bs%5C_pub%7D&id=mO3nD) =  ![](https://cdn.nlark.com/yuque/__latex/3c9f8ddf0d354c94ce02f61f18340a13.svg#card=math&code=g%5E%7BK_%7Bs%5C_pri%7D%7D&id=aFz1U)mod p，服务端将![](https://cdn.nlark.com/yuque/__latex/8757f5f4c0ccfc0aef3eba107f5cf07a.svg#card=math&code=K_%7Bs%5C_pub%7D&id=YUsL4)，p，g三个元素打包称为config，后续发送给客户端。
3. 客户端随机生成一个自己的私钥![](https://cdn.nlark.com/yuque/__latex/d44d552205f071545473bb4c7a43ac01.svg#card=math&code=K_%7Bc%5C_pri%7D&id=ZvdYY)，再从config中读取g和p，计算客户端公钥![](https://cdn.nlark.com/yuque/__latex/6c979997592af3ec5ab2e9d800d76587.svg#card=math&code=K_%7Bc%5C_pub%7D&id=ztwE8) =  ![](https://cdn.nlark.com/yuque/__latex/6ab6a6aacd0c232121ec446251c88a2f.svg#card=math&code=g%5E%7BK_%7Bc%5C_pri%7D%7D&id=c4grY)mod p。
4. 客户端使用自己的私钥![](https://cdn.nlark.com/yuque/__latex/d44d552205f071545473bb4c7a43ac01.svg#card=math&code=K_%7Bc%5C_pri%7D&id=yVzfY)和服务端发来的config中读取的服务端公钥![](https://cdn.nlark.com/yuque/__latex/8757f5f4c0ccfc0aef3eba107f5cf07a.svg#card=math&code=K_%7Bs%5C_pub%7D&id=KW11o)，生成后续数据加密用的密钥![](https://cdn.nlark.com/yuque/__latex/00b43ec51ce758888ed9fdd41cf17b82.svg#card=math&code=K%3DK%5E%7BK_%7Bc%5C_pri%7D%7D_%7Bs%5C_pub%7D&id=nHni2)mod p。
5. 客户端使用密钥K加密业务数据，并追加自己的公钥![](https://cdn.nlark.com/yuque/__latex/6c979997592af3ec5ab2e9d800d76587.svg#card=math&code=K_%7Bc%5C_pub%7D&id=lqQAZ)，都传递给服务端。
6. 服务端根据自己的私钥![](https://cdn.nlark.com/yuque/__latex/3fd9b0ec94190f4214679e257baa5a2d.svg#card=math&code=K_%7Bs%5C_pri%7D&id=rAg7S)和客户端公钥![](https://cdn.nlark.com/yuque/__latex/6c979997592af3ec5ab2e9d800d76587.svg#card=math&code=K_%7Bc%5C_pub%7D&id=Xtp7Y)生成客户端加密用的密钥![](https://cdn.nlark.com/yuque/__latex/3b7aa26f67fc0700ec3e432b08500961.svg#card=math&code=K%3DK%5E%7BK_%7Bs%5C_pri%7D%7D_%7Bc%5C_pub%7D&id=bnThJ)mod p。
7. 为了保证数据安全，上述生成的密钥K只会生成使用1次，后续服务端会按照相同的规则生成一套全新的公钥和私钥，并使用这组公私钥生成新的密钥M。
8. 服务端将新公钥和新密钥M加密的数据发给客户端，客户端根据新的服务端公钥和自己原来的私钥计算出本次的密钥M，进行解密。
9. 之后的客户端和服务端数据交互都使用密钥M来完成，密钥K只使用1次。

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1699844391056-602bbe7a-f815-4514-979d-1e26faa0e381.png#averageHue=%23faf9f9&clientId=ufd0b5589-0ced-4&from=paste&id=u3e688d56&originHeight=1030&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub1b043dc-9df0-472e-8047-e82b281e3ba&title=)
<a name="wJwhK"></a>
#### 3.3.2 非首次连接
前面提到客户端和服务端首次连接时服务端传递了**config包**，里面包含了服务端公钥和两个随机数，**客户端会将config存储下来**，后续再连接时可以直接使用，从而跳过这个1RTT，实现0RTT的业务数据交互。<br />客户端保存config是有时间期限的，在config失效之后仍然需要进行首次连接时的密钥交换。
<a name="cGKax"></a>
### 3.4 前向安全问题
前向安全是**密码学领域的专业术语**，看下百度上的解释：<br />前向安全或前向保密Forward Secrecy是密码学中通讯协议的安全属性，指的是长期使用的主密钥泄漏不会导致过去的会话密钥泄漏。<br />前向安全能够保护过去进行的通讯不受密码或密钥在未来暴露的威胁，如果系统具有前向安全性，就可以保证在主密钥泄露时历史通讯的安全，即使系统遭到主动攻击也是如此。<br />通俗来说，**前向安全指的是密钥泄漏也不会让之前加密的数据被泄漏，影响的只有当前，对之前的数据无影响**。<br />前面提到QUIC协议首次连接时先后生成了两个加密密钥，由于config被客户端存储了，如果期间服务端私钥![](https://cdn.nlark.com/yuque/__latex/3fd9b0ec94190f4214679e257baa5a2d.svg#card=math&code=K_%7Bs%5C_pri%7D&id=IKnZX)泄漏，那么可以根据![](https://cdn.nlark.com/yuque/__latex/3b7aa26f67fc0700ec3e432b08500961.svg#card=math&code=K%3DK%5E%7BK_%7Bs%5C_pri%7D%7D_%7Bc%5C_pub%7D&id=YfaXp)mod p计算出密钥K。<br />如果一直使用这个密钥进行加解密，那么就可以用K解密所有历史消息，因此后续又生成了新密钥，使用其进行加解密，当时完成交互时则销毁，从而实现了前向安全。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1699844393761-3feecb61-6324-4410-9ef4-128c41e023aa.png#averageHue=%23f1e8de&clientId=ufd0b5589-0ced-4&from=paste&id=ua075a763&originHeight=722&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u77c3eeb9-4857-4f6f-885c-b2cfcccde5b&title=)
<a name="Xeq8e"></a>
### 3.5 前向纠错
前向纠错是通信领域的术语，看下百科的解释：<br />前向纠错也叫前向纠错码Forward Error Correction 简称FEC 是增加数据通讯可信度的方法，在单向通讯信道中，一旦错误被发现，其接收器将无权再请求传输。<br />FEC 是利用数据进行传输冗余信息的方法，当传输中出现错误，将允许接收器再建数据。<br />听这段描述就是做校验的，看看QUIC协议是如何实现的：<br />QUIC每发送一组数据就对这组数据进行**异或运算**，并将结果作为一个FEC包发送出去，接收方收到这一组数据后根据数据包和FEC包即可进行校验和纠错。
<a name="ctQnG"></a>
### 3.6 连接迁移
网络切换几乎无时无刻不在发生。<br />TCP协议使用**五元组来表示一条唯一的连接**，当从4G环境切换到wifi环境时，手机的IP地址就会发生变化，这时必须创建新的TCP连接才能继续传输数据。<br />QUIC协议基于UDP实现摒弃了五元组的概念，**使用64位的随机数**作为连接的ID，并使用该ID表示连接。<br />基于QUIC协议之下，在**日常wifi和4G切换**时，或者不同基站之间切换都不会重连，从而提高业务层的体验。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1699844393789-a127926b-c969-44f1-826f-a27b1cb8af6f.png#averageHue=%23f1e5de&clientId=ufd0b5589-0ced-4&from=paste&id=u38cdd334&originHeight=723&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u07074d2c-5d83-4f52-8c09-d035611dc34&title=)
<a name="lgIaj"></a>
## 4、QUIC的应用和前景
通过前面的一些介绍可以看出来**QUIC协议虽然是基于UDP来实现的，但是它将TCP的重要功能都进行了实现和优化**，否则使用者是不会买账的。<br />QUIC协议的核心思想是将TCP协议在内核实现的诸如可靠传输、流量控制、拥塞控制等功能转移到**用户态**来实现，同时在加密传输方向的尝试也推动了**TLS1.3**的发展。<br />但是TCP协议的势力过于强大，很多**网络设备甚至对于UDP数据包做了很多不友好的策略**，进行拦截从而导致成功连接率下降。<br />主导者谷歌在自家产品做了很多尝试，国内腾讯公司也做了很多关于QUIC协议的尝试。<br />其中腾讯云对QUIC协议表现了很大的兴趣，并做了一些优化然后在一些重点产品中对**连接迁移**、**QUIC成功率**、**弱网环境耗时**等进行了实验，给出了来自生产环境的诸多宝贵数据。<br />简单看一组腾讯云在移动互联网场景下的不同丢包率下的请求耗时分布：![](https://cdn.nlark.com/yuque/0/2023/png/396745/1699844394133-ac2bcd33-bc3d-43cb-8b58-55ef359f5a92.png#averageHue=%23fcfbfa&clientId=ufd0b5589-0ced-4&from=paste&id=ucbe98294&originHeight=608&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u864a02f6-10ad-4402-91ec-0d2b2b9d207&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1699844394240-6fa704ca-4bcc-475d-b9cb-646af0db9bd4.png#averageHue=%23f9f9fa&clientId=ufd0b5589-0ced-4&from=paste&id=u7e1298e0&originHeight=305&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u75dd7ee5-8984-4dcb-9c25-d8e4e94c557&title=)<br />**任何新生事物的推动都是需要时间的**，出现多年的HTTP2.0和HTTPS协议的普及度都没有预想高，IPv6也是如此，不过QUIC已经展现了强大的生命力，拭目以待吧！
<a name="mcAhu"></a>
## 5、小结
网络协议本身就很复杂，本文只能从整体出发对重要的部分做**粗浅的阐述**，如果对某个点很感兴趣，可以查阅相关代码和RFC文档。<br />之前可能遇到过这个面试题：<br />如何用UDP协议来实现TCP协议的主要功能。<br />确实笔试遇到过这道题，可以说很抓狂，题目太宏大了。<br />不过现在看看QUIC协议就回答了这个问题：基于UDP主体将TCP的重要功能转移到用户空间来实现，从而绕开内核实现用户态的TCP协议，但是真正实现起来还是非常复杂的。
