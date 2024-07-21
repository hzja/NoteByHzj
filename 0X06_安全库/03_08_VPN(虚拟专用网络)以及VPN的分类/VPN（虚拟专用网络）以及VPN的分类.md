# VPN

## 前言
​	**VPN技术**还是企业比较常用的通信技术，如果一个企业的分公司和总部的互访，或者出差员工需要访问总部的网络，都会使用VPN技术；这里介绍常见的几种VPN技术，主要包括**IPsec VPN、GRE VPN、L2TP VPN、MPLS VPN**等。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642392101209-95196e32-0a04-4c4f-9a24-3510f37c42cd.webp#clientId=uea7f35bc-a955-4&from=paste&id=u725d0dc6&originHeight=238&originWidth=440&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7e8ff8bd-3cdd-4685-a240-5641fd99327&title=)



## 什么是VPN

### 1、VPN技术出现背景
​        一个技术的出现都是由于某种需求触发的。那么为什么会出现VPN技术呢？VPN技术解决了什么问题呢？在没有VPN之前，企业的总部和分部之间的互通都是采用运营商的internet进行通信，那么Internet中往往是不安全的，通信的内容可能被窃取、修改等，从而造成安全事件。

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642392101303-0b166259-bbbb-43d8-ae50-467bc201ff35.webp#clientId=uea7f35bc-a955-4&from=paste&id=ub3f7d56b&originHeight=147&originWidth=647&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9e30e42b-64a8-4fab-87f3-425fcd415a7&title=)

那么有没有一种技术既能实现总部和分部间的互通，也能够保证数据传输的安全性呢？答案是当然有。



​        一开始大家想到的是专线，在总部和分部拉条专线，只传输自己的业务，但是这个专线的费用确不是一般公司能够承受的。而且维护也很困难。

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642392101222-1c49d128-ba04-4ea2-afbd-a6f0f3f3bdd0.webp#clientId=uea7f35bc-a955-4&from=paste&id=u480e27b9&originHeight=147&originWidth=647&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0c83904b-0b4e-4d00-9b0c-d423f804f55&title=)



​	那么有没有成本也比较低的方案呢？有，那就是VPN。VPN通过在现有的Internet网中构建专用的虚拟网络，实现企业总部和分部的通信，解决了互通、安全、成本的问题；

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642392101190-f7c30c7a-edf7-4411-b259-09d5ea274b2c.webp#clientId=uea7f35bc-a955-4&from=paste&id=uc624c017&originHeight=162&originWidth=637&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u00d080de-6cc7-4bad-85c5-76ca784569b&title=)


### 2、什么是VPN技术
那么什么是VPN技术吗？

VPN即虚拟专用网，指通过VPN技术在**公有网络**中构建**专用**的**虚拟**网络；

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642392101208-f3c9607e-756e-404c-9f9c-2e694fe9f8cb.webp#clientId=uea7f35bc-a955-4&from=paste&id=u5df4f1a7&originHeight=366&originWidth=658&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u66762a43-5b0c-4721-b7ad-395948379cd&title=)

用户在此虚拟网络中传输流量，从而在Internet网络中实现安全、可靠的连接。

(1)专用：VPN虚拟网络是专门给VPN用户使用的网络，对于用户而言，使用VPN和Internet，用户是不感知的，是由VPN虚拟网络提供安全保证。

(2)虚拟：相对于公有网络而言，VPN网络是虚拟的，是逻辑意义上的一个专网。


### 3、VPN技术优势
VPN和传统的公网Internet相比具有如下优势：

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642392101509-8f9b67ab-5b2b-414f-8c42-03dfb08e5044.webp#clientId=uea7f35bc-a955-4&from=paste&id=u91cebd4a&originHeight=323&originWidth=548&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7dffba1a-1165-4273-9272-fc0515d144d&title=)

- 安全：在远端用户、驻外机构、合作伙伴、供应商与公司总部之间建立可靠的连接，保证数据传输的安全性。这对于实现电子商务或金融网络与通讯网络的融合特别重要。
- 成本低：利用公共网络进行信息通讯，企业可以用更低的成本连接远程办事机构、出差人员和业务伙伴。
- 支持移动业务：支持出差VPN用户在任何时间、任何地点的移动接入，能够满足不断增长的移动业务需求。
- 可扩展性：由于VPN为逻辑上的网络，物理网络中增加或修改节点，不影响VPN的部署。

## VPN分类
### 1、根据VPN建设单位不同进行划分
#### （1）租用运营商VPN专线搭建企业网络
运营商的专线网络大多数都是使用的MPLS VPN；

企业通过购买运营商提供的VPN专线服务实现总部和分部间的通信需求。VPN网关为运营商所有。

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642392101717-5bac1d09-7352-4634-aef7-712cb497f452.webp#clientId=uea7f35bc-a955-4&from=paste&id=uf2995e95&originHeight=192&originWidth=838&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4f7afcd7-4dca-44ff-8914-14669a5d1da&title=)




#### （2）企业自建VPN网络

企业自己基于Internet自建vpn网络，常见的如IPsec VPN、GRE VPN、L2TP VPN。

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642392101758-bd6ee462-d5b4-45ea-a4af-83007f75e2b2.webp#clientId=uea7f35bc-a955-4&from=paste&id=u2d2c023b&originHeight=192&originWidth=991&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud1e6206f-718d-48cd-b07a-ad0dbf38f61&title=)

企业自己购买VPN网络设备，搭建自己的VPN网络，实现总部和分部的通信，或者是出差员工和总部的通信。


### 2、根据组网方式进行划分
#### （1）远程访问VPN
这种方式适用于出差员工拨号接入VPN的方式，员工可以在只要有Internet的地方都可以通过VPN接入访问内网资源。最常见的就是SSL VPN、L2TP VPN。



![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642392101711-569c6014-b29e-4376-b7db-9bf566f8581f.webp#clientId=uea7f35bc-a955-4&from=paste&id=u88a3e37a&originHeight=366&originWidth=642&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0d95ad1a-6995-40e1-80d7-c2f83173a60&title=)




#### （2）站点到站点的VPN

这种方式适用于企业两个局域网互通的情况。例如企业的分部访问总部。最常见的就是MPLS VPN、IPSEC VPN。

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642392102135-e89d1e2e-ddea-491f-8fe9-431a94b446f8.webp#clientId=uea7f35bc-a955-4&from=paste&id=u37a2e4ac&originHeight=192&originWidth=991&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1cb250c5-0bce-419b-ac10-14cdd03106e&title=)


### 3、根据工作网络层次进行划分
VPN可以按照工作层次进行划分：

（1）应用层：SSL VPN

（2）网络层：IPSEC VPN 、GRE VPN

（3）数据链路层：L2TP VPN、PPTP VPN

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642392101941-eb09ba6f-2a6d-4741-8599-1aba24a59063.webp#clientId=uea7f35bc-a955-4&from=paste&id=u97fb281f&originHeight=471&originWidth=621&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud6ccb114-1f77-43df-8b09-a65d5e3ed21&title=)


## VPN关键技术
### 1、隧道技术
VPN技术的基本原理其实就是用的隧道技术，就类似于火车的轨道、地铁的轨道一样，从A站点到B站点都是直通的，不会堵车。对于乘客而言，就是专车。

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642392102034-4eb035b5-0c19-4df3-ab34-752125426a5e.webp#clientId=uea7f35bc-a955-4&from=paste&id=u6b5e3565&originHeight=715&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud0f39d40-f71a-4975-96c0-0d8fd4cdc81&title=)

隧道技术其实就是对传输的报文进行封装，利用公网的建立专用的数据传输通道，从而完成数据的安全可靠性传输；![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642392102192-b45b6f94-4fc9-4a43-9851-7cbd62730a39.webp#clientId=uea7f35bc-a955-4&from=paste&id=u2d3a44eb&originHeight=237&originWidth=991&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8913133a-3175-479a-bee0-736b9ae0878&title=)

可以看到原始报文在隧道的一端进行封装，封装后的数据在公网上传输，在隧道另一端进行解封装，从而实现了数据的安全传输。隧道通过**隧道协议**实现。如GRE（Generic Routing Encapsulation）、L2TP（Layer 2 Tunneling Protocol）等。隧道协议通过在隧道的一端给数据加上隧道协议头，即进行**封装**，使这些被封装的数据能都在某网络中传输，并且在隧道的另一端去掉该数据携带的隧道协议头，即进行解封装。

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642392102278-96366eea-6afd-4d1d-afb0-3cc006a31f76.webp#clientId=uea7f35bc-a955-4&from=paste&id=ub9b12775&originHeight=191&originWidth=821&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u952c0166-8a6c-432b-95f1-17401ee0906&title=)<br />报文在隧道中传输前后都要通过封装和解封装两个过程。


### 2、身份认证、数据加密、数据验证
身份认证、数据加密、数据验证可以有效保证VPN网络和数据的安全性。

- **身份认证：**VPN网关对接入VPN的用户进行身份认证，保证接入的用户都是合法用户。

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642392102399-a362533a-2801-49fc-96a4-ad82d8075549.webp#clientId=uea7f35bc-a955-4&from=paste&id=u7ad3e229&originHeight=412&originWidth=642&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0cd7062b-d44b-4b28-84e9-67c49bde96b&title=)

- **数据加密**：将明文通过加密技术成密文，哪怕信息被获取了，也无法识别。

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642392102584-0f401e97-66ce-4857-8102-430e2bbfdd8d.webp#clientId=uea7f35bc-a955-4&from=paste&id=uee9d4351&originHeight=151&originWidth=541&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u59aea44e-70e5-4b52-9c04-1bbf4d23951&title=)

- **数据验证：**通过数据验证技术验证报文的完整性和真伪进行检查，防止数据被篡改。

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642392102618-7e3c34a8-8c58-44d7-b7cf-6b0f602d59fc.webp#clientId=uea7f35bc-a955-4&from=paste&id=u3ebae0a8&originHeight=191&originWidth=541&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u64cfa9c5-ffed-4b2e-9ff0-750735bd77f&title=)

VPN隧道身份认证、数据加密、验证如下：

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642392102743-9a43f04f-8be6-462c-9f13-fa23b9cef3c0.webp#clientId=uea7f35bc-a955-4&from=paste&id=u2eff8805&originHeight=209&originWidth=698&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud4cb5adb-4b08-4e8a-bb51-73a7526e5cb&title=)

