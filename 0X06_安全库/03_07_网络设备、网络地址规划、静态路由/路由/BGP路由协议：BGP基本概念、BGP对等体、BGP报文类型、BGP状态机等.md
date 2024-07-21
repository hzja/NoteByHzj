网络<br />**BGP路由协议**是网络中使用最为广泛的协议了，不管是运营商网络、企业网络、数据中心网络，目前都有在使用BGP协议。<br />详细介绍下BGP路由协议；<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641351360431-cc71e04a-351d-4d77-9173-b52eee6e957d.webp#clientId=ue9951546-d532-4&from=paste&id=u772592b9&originHeight=537&originWidth=562&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u79b3c3ab-4aba-49a9-bb49-574d27b8262&title=)
<a name="ylgO1"></a>
## 什么是BGP路由协议
<a name="r8sg8"></a>
### 1、BGP协议出现背景
OSPF、ISIS路由协议在网络中已经广泛使用，但是随着网络规模的扩大，路由条目也随增加，IGP协议已经无法管理大规模网络了。因此出现了AS的概念。<br />AS的概念比较抽象呀。<br />官方解释：AS是指在同一组织管理下，使用统一选路策略设备的集合。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641351360506-965e805f-c546-4813-92d6-262e4af6df04.webp#clientId=ue9951546-d532-4&from=paste&id=uc6b4e6df&originHeight=260&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6ae15337-1fb6-4c62-86a8-cde7f425187&title=)<br />简单来说：就是可以把一个网络中的不同的设备划分到不同的组（AS）中，或者都划分在一个组中，那么一个组中的这些设备具备相同的路由协议。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641351360598-1c5c3d21-46f4-4321-93a9-edeb82a98768.webp#clientId=ue9951546-d532-4&from=paste&id=u98e9ce61&originHeight=251&originWidth=991&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u143b4b21-248e-4e74-9b20-f0858a2c4fb&title=)<br />比如可以将企业总部网络划分在一个AS中，企业的分部网络划分在一个AS中。不同的AS可以运行不同的路由协议。<br />那么不同AS的网络需要通信时，采用什么路由协议进行通信呢？答案就是本章的BGP路由协议。那么为什么不能够通过IGP（ISIS、BGP）进行通信呢？<br />这个举个简单的例子，企业网络或者数据中心网络对接运营商的Internet时。如果使用IGP路由协议（OSPF、ISIS）对接时，那么这个时候企业网络和运营商的网络是运行在一张网络中，这个是不安全的，可以认为运营商为什么信任你的企业网络？<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641351360628-c428aa18-9117-491d-a3c8-1af9aaf005a7.webp#clientId=ue9951546-d532-4&from=paste&id=u9148cbbf&originHeight=792&originWidth=982&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua11989bd-7661-4610-8adb-5d0205cb0e9&title=)<br />还有一点就是整个网络规模扩大，路由数量增多，路由收敛慢。<br />那么有没有一种路由协议可以实现不同网络间的路由交互呢？答案就是BGP。BGP有个好处就是不产生路由，只传递路由，并且具备丰富的路由策略。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641351360578-1ba165fb-5094-4dd6-8cc3-ab37298385b7.webp#clientId=ue9951546-d532-4&from=paste&id=u8149d399&originHeight=862&originWidth=982&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u991dd347-7e6f-440c-8bc5-1bf88041fe2&title=)<br />这个时候可能有人还会站出来说，静态协议不就可以解决吗？其实是可以解决，不过有个问题就是使用静态协议时网络变动时都需要人手动去增删路由，那么有没有一种动态的方式呢？答案就是BGP路由协议了。<br />所以说上面的例子：企业和运营商对接时可以采用BGP路由协议进行对接实现路由的交互。
<a name="kfUOs"></a>
### 2、BGP传递路由优势
不同AS之间通过BGP路由协议对接，那么使用BGP相比IGP协议有那么好处呢？<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641351361908-822cc722-1204-4369-883c-dd1db60f6e31.webp#clientId=ue9951546-d532-4&from=paste&id=u5b94f7f3&originHeight=254&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf1eb4e76-3d7b-4a80-94eb-294a2ec3e4e&title=)

- BGP是基于TCP的路由协议，只要能够建立TCP就能够建立BGP；
- BGP只传递路由信息，不计算路由，不会暴露AS内部的网络拓扑；
- BGP的路由更新是触发更新，不是周期性更新；
<a name="jLpyp"></a>
### 3、什么是BGP路由协议
BGP是一种基于距离矢量的路由协议，用于实现不同AS之间的路由可达。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641351361981-0200ae7a-5856-4b03-bdf7-e4c41a872cb6.webp#clientId=ue9951546-d532-4&from=paste&id=u983d39a1&originHeight=254&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u44c2fa65-f51b-4770-8139-1da5ad24b4c&title=)<br />BGP协议的基本特点：<br />（1）BGP是一种外部网关协议，其着眼点不在于发现和计算路由，而在于控制路由的传播和选择最佳路由；<br />（2）BGP使用TCP作为其传输层协议（端口号179），提高了协议的可靠性；<br />（3）BGP是一种距离矢量路由协议，在设计上就避免了环路的发生；<br />（4）BGP提供了丰富的路由策略，能够实现路由的灵活过滤和选择；<br />（5）BGP采用触发式增量更新，而不是周期性的更新；
<a name="eEFGv"></a>
## BGP对等体
<a name="tcyFI"></a>
### 1、什么是BGP对等体
**BGP发言者**：运行BGP路由协议的路由器称为BGP发言者（BGP路由器）；<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641351361968-7bc19846-3ce2-4634-8b1b-763b89123e17.webp#clientId=ue9951546-d532-4&from=paste&id=u464de776&originHeight=211&originWidth=791&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u14eb9e7a-5784-4395-b327-b561389d290&title=)<br />**BGP对等体**：两个建立BGP会话（BGP的会话是基于TCP建立的）的路由器互为BGP对等体，BGP对等体之间可以交换路由表；<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641351362112-ae7604e2-b5e8-4fa6-9985-8205ff8f6828.webp#clientId=ue9951546-d532-4&from=paste&id=u63c617d3&originHeight=227&originWidth=791&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u843dc1a5-52ad-4d7d-b0c1-9f6307e9975&title=)
<a name="y1MOh"></a>
### 2、BGP对等体分类
BGP对等体可以按照两个路由器是否AS相同，分为EBGP对等体和IBGP对等体。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641351362354-1e47bc9c-9b60-4824-82e7-32f057829143.webp#clientId=ue9951546-d532-4&from=paste&id=u3644fb54&originHeight=195&originWidth=594&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u02e4393b-220e-42a3-8af4-cf2b080f53f&title=)<br />（1）IBGP：位于相同的AS的BGP路由器互为IBGP对等体关系；<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641351362850-1ca91c42-0708-4f73-a577-49f0068142b3.webp#clientId=ue9951546-d532-4&from=paste&id=uec419851&originHeight=227&originWidth=791&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u927dcc6f-6bfe-497a-b12e-1eade580222&title=)<br />（2）EBGP：位于不同的AS的BGP路由器互为EBGP对等体关系；<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641351362976-d42e4732-1bf8-4a57-9271-c004d82c6679.webp#clientId=ue9951546-d532-4&from=paste&id=u5e2cdb88&originHeight=227&originWidth=791&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ubb0a6f54-45ac-4701-b6df-786fef1cbf8&title=)<br />**能够建立对等体的条件:**

- 两个路由器指定建立对等体的地址必需可达；
- TCP连接能够建立；
<a name="gulWz"></a>
### 3、如何建立BGP对等体
<a name="ZIS8e"></a>
#### （1）TCP连接建立
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641351362957-15ef0989-9537-431d-9eb1-4a20529f6ac1.webp#clientId=ue9951546-d532-4&from=paste&id=ua4ed22bf&originHeight=201&originWidth=641&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud1168efa-2142-4f33-8de7-f2c31b60f00&title=)<br />假设BGP路由器R1先启动，则R1先发起建立TCP连接，通过3次握手完成TCP的连接建立。
<a name="ELPMZ"></a>
#### （2）BGP路由器发送OPEN报文协商参数
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641351363501-534dfa7f-1135-4275-af9c-54e3f2a6e2b8.webp#clientId=ue9951546-d532-4&from=paste&id=u7cc32a7b&originHeight=333&originWidth=641&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u225c9b92-27a7-46d7-9d07-b392b8db247&title=)<br />三次握手建立完成后，R1和R2之间相互发送OPEN报文，用于建立对等体的参数协商。<br />**OPEN报文参数：**

- 自身AS号；
- hold time：用于协商后续keepalive报文发送时间；
- BGP identifier：自身router id；
<a name="vlobD"></a>
#### （3）BGP路由器发送keepalive报文完成对等体建立
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641351363558-0085818a-0734-4c63-8fd4-59037e878a1d.webp#clientId=ue9951546-d532-4&from=paste&id=u997b087f&originHeight=483&originWidth=641&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uca16a425-3ea2-4a5e-be9e-18d8d7062a2&title=)<br />参数协商正常后双方相互发送keepalive报文，收到对方的keepalive报文后对等体建立成功，同时后续定期发送keepalive报文用于保持连接。
<a name="IyEES"></a>
#### （4）BGP路由器发送UPDATE报文通告路由
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641351363570-41881ea5-ab34-4455-bfba-1406a5733110.webp#clientId=ue9951546-d532-4&from=paste&id=u9006f9d1&originHeight=623&originWidth=641&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8d16a297-028b-440d-8871-c0eb81e828b&title=)<br />BGP对等体关系建立好了，就可以通过BGP update 报文通告路由到对等体。收到对方的keepalive报文后对等体建立成功，同时后续定期发送keepalive报文用于保持连接。
<a name="q67Sw"></a>
### 4、BGP对等体建立使用源地址
BGP默认使用出接口的IP地址建立对等体。<br />（1）在建立IBGP对等体关系时，建议使用loopback地址建立IBGP对等体。因为loopback地址稳定，可以借助内部IGP冗余保证可靠性。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641351363541-ce580d61-1ab4-45f5-ad89-003b551090d9.webp#clientId=ue9951546-d532-4&from=paste&id=u528578ad&originHeight=287&originWidth=502&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6c7aac00-58c5-438c-83e4-2797c6644c7&title=)<br />例如上图：<br />R1和R2通过loopback地址建立IBGP对等体，如果R1和R2之间的物理链路断了，loopback地址的连通性也不会受影响，可以通过R3实现R1和R2的loopback地址的互通，那么R1和R2之间的对等体关系依然可以保持建立。如果通过接口建立的话，就会导致BGP对等体关系中断。<br />（2）在建立EBGP对等体关系时，建议使用直连地址建立EBGP对等体。如果使用loopback，需要注意EBGP多跳的问题。
<a name="qLY3k"></a>
## BGP报文类型
<a name="fPbLb"></a>
### 1、BGP报文类型
BGP存在5种不同类型的报文，分别是open、update、notification、keepalive、route-refresh。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641351364036-89bfc46f-2df5-4b88-9c84-1cb2600c83eb.webp#clientId=ue9951546-d532-4&from=paste&id=u988f1360&originHeight=286&originWidth=732&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ueff4ae81-191e-489e-81ae-3c52ed61dc7&title=)
<a name="iBkKY"></a>
### 2、BGP报文头格式
不同于常见的IGP协议，BGP使用TCP作为传输层协议，端口号179。BGP存在5种不同类型的报文，不同类型的报文具有相同的头部。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641351364268-1afd99a3-a7c9-4fa0-abe8-4e1c9ecb389a.webp#clientId=ue9951546-d532-4&from=paste&id=u9c1f5d59&originHeight=284&originWidth=506&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud17a2a4a-6279-40aa-8e62-3b7c213c12c&title=)<br />报文头字段解释：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641351364617-f65bfe6c-3031-462c-a6a1-1250adc1b2fe.webp#clientId=ue9951546-d532-4&from=paste&id=u6cfc10e6&originHeight=293&originWidth=953&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7c512c73-237c-4306-bc5e-5b6ef5cfe78&title=)

- Marker：16Byte，用于标明BGP的边界，所有的bit均为“1”
- Length：2Byte，BGP总报文长度（包括报文头）
- Type：1Byte，BGP报文的类型。其取值从1到5，分别表示open、update、notification、keepalive、route-refresh报文。
<a name="JsiKw"></a>
### 3、BGP报文格式
<a name="MXWVY"></a>
#### （1）OPEN报文
open报文是TCP连接建立后发送的第一个报文，用于协商参数<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641351364263-2a19fb6f-d118-4691-97da-d23006091978.webp#clientId=ue9951546-d532-4&from=paste&id=u153cf5f0&originHeight=290&originWidth=941&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ufc6ef9da-510b-4784-932b-daab077c569&title=)

- version：bgp版本，通常为4；
- My AS：本地AS号；
- Hold time：保持时间。在建立对等体关系时两端要协商hold time，如果在这段时间内未收到对端发来的keepalive报文和update报文，则认为BGP连接中断；
- BGP  identifier: BGP标识符，以IP地址形式标识，用来识别路由器。
<a name="Glnti"></a>
#### （2）Update报文
Update报文用于在对等体之间传递路由信息，可以用来发布和撤销路由。<br />一个Update报文可以通告具有相同路径属性的多条路由，这些路由保存在NLRI（网络层可达信息）中。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641351364537-2a2b5911-1d45-4022-8e11-d70f3245eef9.webp#clientId=ue9951546-d532-4&from=paste&id=uebd6f67a&originHeight=457&originWidth=1070&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf58f85e8-1db5-4789-99db-2c6a94db4cf&title=)

- Withdrawn Routes Length：标明Withdrawn Routes部分的长度。其值为零时，表示没有撤销的路由。
- Total path attribute length：路径属性字段的长度，以Byte为单位。如果为0则说明没有Path Attributes 字段。
<a name="j8Gti"></a>
#### （3）Notification报文
当BGP检测到错误状态时，就会向对等体发送Notification报文，告知对端错误，之后BGP连接会立即中断。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641351365065-f10fd630-b641-4067-bb70-ce586d67d75e.webp#clientId=ue9951546-d532-4&from=paste&id=u79eea407&originHeight=87&originWidth=337&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3fec556f-285c-4849-9a2b-8c3182cf6e4&title=)

- Error Code、Error Code：差错码、差错子码，描述错误类型；
- Data：错误内容；
<a name="fMxzU"></a>
#### （4）Keepalive报文
双方相互发送keepalive报文，收到对方的keepalive报文后对等体建立成功，同时后续定期发送keepalive报文用于保持连接。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641351365308-450a0687-6f49-4244-bfee-461f177ead50.webp#clientId=ue9951546-d532-4&from=paste&id=u4f89beb8&originHeight=237&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u54f9fda0-8177-44b5-ad52-ff39972445f&title=)
<a name="MMKCE"></a>
#### （5）Route-refresh报文
Route-refresh报文用于要求对等体重新发送指定地址族的路由信息，一般是本地修改了相关的路由策略，让对方重新发送update报文。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641351365275-fb6ed479-f074-4bf4-a0d2-e28df36bb85e.webp#clientId=ue9951546-d532-4&from=paste&id=uf1c0920a&originHeight=288&originWidth=946&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2f7e8f94-9d5e-481a-bb96-c8e4cacdbce&title=)<br />在Open报文协商时会协商是否支持Route-refresh，如果对等体支持Route-refresh能力，则可以通过refresh bgp命令手工对BGP连接进行软复位，BGP软复位可以在不中断BGP连接的情况下重新刷新BGP路由表，并应用新的策略。
<a name="STEOg"></a>
## BGP状态机
<a name="zIciT"></a>
### 1、BGP 6种状态
BGP的状态有idle、connect、active、opensent、openconfirm、established六种状态。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641351365634-94ffdde0-8a9c-4b58-9c41-7f806341d1e7.webp#clientId=ue9951546-d532-4&from=paste&id=u85689609&originHeight=289&originWidth=765&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ufa44c947-81f3-42bb-a563-9c2765c3b15&title=)
<a name="Htvac"></a>
### 2、BGP 状态变换
BGP的状态有idle、connect、active、opensent、openconfirm、established六种状态。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641351365849-9cf0997c-8bf7-4ae1-938c-609594c7c950.webp#clientId=ue9951546-d532-4&from=paste&id=uc3f64813&originHeight=292&originWidth=705&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue06269ea-3413-42ea-ad2b-cbe393a4da2&title=)
<a name="aism2"></a>
#### （1）Idle状态是BGP初始状态。
在Idle状态下，BGP拒绝对等体发送的连接请求。只有在收到本设备的Start事件后，BGP才开始尝试和其它BGP对等体进行TCP连接，并转至Connect状态。<br />Start事件是由一个操作者配置一个BGP过程，或者重置一个已经存在的过程或者路由器软件重置BGP过程引起的。<br />任何状态中收到Notification报文或TCP拆链通知等Error事件后，BGP都会转至Idle状态。
<a name="puhEJ"></a>
#### （2）Connect状态
在Connect状态下，BGP启动连接重传定时器（Connect Retry），等待TCP完成连接。

- 如果TCP连接成功，那么BGP向对等体发送Open报文，并转至OpenSent状态。
- 如果TCP连接失败，那么BGP转至Active状态。
- 如果连接重传定时器超时，BGP仍没有收到BGP对等体的响应，那么BGP继续尝试和其它BGP对等体进行TCP连接，停留在Connect状态。
<a name="FApJW"></a>
#### （3）Active状态
在Active状态下，BGP总是在试图建立TCP连接。

- 如果TCP连接成功，那么BGP向对等体发送Open报文，关闭连接重传定时器，并转至OpenSent状态。
- 如果TCP连接失败，那么BGP停留在Active状态。
- 如果连接重传定时器超时，BGP仍没有收到BGP对等体的响应，那么BGP转至Connect状态。
<a name="n34hY"></a>
#### （4）Opensent状态、openconfirm状态
TCP三次握手建立成功后，发送open报文建立对等体关系，此时的状态为<br />opensent状态，当收到对端回应的open报文，并且参数检查无误，在发送keepalive报文后进入openconfirm状态。
<a name="JiOvo"></a>
#### （5）established状态
进入openconfirm状态后，收到对端的keepalive报文后进入established状态。
<a name="D4mS9"></a>
## BGP路由表
<a name="G5gK0"></a>
### 1、BGP对等体表
可以通过`dis bgp peer` 查看BGP对等体表；<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641351365816-5cdae981-520f-4bb7-8069-ae5579c73275.webp#clientId=ue9951546-d532-4&from=paste&id=ud2cdce95&originHeight=181&originWidth=711&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub7f92ca2-e0c8-4baa-a23b-2030087520c&title=)<br />（1）peer：对等体地址<br />（2）V：版本号<br />（3）AS：对等体AS号<br />（4）UP/DOWN：对等体存在up或者down的时间<br />（5）state：对等体状态<br />（6）prefRce：从该对等体收到的路由前缀数目
<a name="q0VDk"></a>
### 2、BGP路由表
可以通过`display  bgp routing-table`  查看BGP路由表；<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641351365839-01a04eea-f217-4ccb-a28a-52ce15709e9e.webp#clientId=ue9951546-d532-4&from=paste&id=u80b2f823&originHeight=199&originWidth=694&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u27be3b7e-89ee-45b2-9a91-ad16a733577&title=)<br />如果到达同一个目的地存在多条路由，则将路由都进行罗列，但每个目的地只会优选一条路由。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641351365951-3efebbac-a5a7-4087-9c61-e339911fdf89.webp#clientId=ue9951546-d532-4&from=paste&id=u7f854e6e&originHeight=286&originWidth=710&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1307994a-df32-4148-a45c-6ec07c1f415&title=)<br />通过`display bgp routing-table ipv4-address { mask | mask-length }` 可以显示指定IP地址/掩码长度的路由信息，在其中有关于该BGP路由的详细信息，如：路由始发者、下一跳地址、路由的路径属性等。
<a name="fBRpK"></a>
## BGP路由生成
BGP路由是通过BGP命令通告而成的，而通告BGP路由的方法有两种：network和Import。
<a name="fZPTv"></a>
### （1）network方式：
使用network命令可以将当前设备路由表中的路由（非BGP）发布到BGP路由表中并通告给邻居，和OSPF中使用network命令的方式大同小异，只不过在BGP宣告时，只需要宣告网段+掩码数即可，如：network 12.12.0.0 16。
<a name="c91vT"></a>
### （2）Import方式：
使用Import命令可以将该路由器学到的路由信息重分发到BGP路由表中，是BGP宣告路由的一种方式，可以引入BGP的路由包括：直连路由、静态路由及动态路由协议学到的路由。其命令格式与在RIP中重分发OSPF差不多。
<a name="HUSD8"></a>
## BGP通告原则
BGP设备会将最优路由加入BGP路由表，形成BGP路由。<br />BGP设备与对等体建立邻居关系后，采用以下交互原则：

- 从IBGP对等体获得的BGP路由，BGP设备只传递给它的EBGP对等体。
- 从EBGP对等体获得的BGP路由，BGP设备传递给它所有EBGP和IBGP对等体（对等体是IBGP只能传递一跳，对等体是EBGP则不限制）
- 当存在多条到达同一目的地址的有效路由时，BGP设备只将最优路由发布给对等体
- 路由更新时，BGP设备只发送更新的BGP路由
- 所有对等体发送的路由，BGP设备都会接收
- 所有EBGP对等体在传递过程中下一跳改变
- 所有IBGP对等体在传递过程中下一跳不变（需要特别注意）
- 默认EBGP传递时 TTL值为1（需要特别注意）
- 默认IBGP传递时 TTL值为255
