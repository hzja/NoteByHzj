<a name="AxxY2"></a>
## 前言
本文介绍了EVPN（Ethernet VPN）的基本概念，EVPN基于MP-BGP，定义了一系列新的BGP EVPN路由类型，EVPN可以作为VXLAN的控制面。阅读本文，还可以了解BGP EVPN几种新路由的格式和工作场景，了解EVPN在作为VXLAN控制面时是如何传递路由、帮助设备建立VXLAN隧道的。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1698629430177-b818e40d-5aea-4486-93c5-137b41ac654e.png#averageHue=%23eff1f4&clientId=u2a8b9fb7-3a0f-4&from=paste&id=u1e53a26a&originHeight=510&originWidth=822&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u11f35fd3-f3e7-496e-b82b-e7b1c1ed96b&title=)
<a name="vKB7t"></a>
## EVPN简介
<a name="i11E6"></a>
### EVPN基本概念
为什么会有EVPN（Ethernet VPN）呢？最初的VXLAN方案（RFC7348）中没有定义控制平面，是手工配置VXLAN隧道，然后通过流量泛洪的方式进行主机地址的学习。这种方式实现上较为简单，但是会导致网络中存在很多泛洪流量、网络扩展起来困难。<br />为了解决上述问题，人们在VXLAN中引入了EVPN（Ethernet VPN）作为VXLAN的控制平面，如图1-1所示（VXLAN是一种NVO协议）。EVPN还能作为一些其他协议的控制面，本文仅描述EVPN作为VXLAN的控制面的相关信息。<br />![图1-1 将EVPN作为VXLAN的控制平面](https://cdn.nlark.com/yuque/0/2023/png/396745/1698629430160-9f08e946-78ac-4896-9768-64fafba00b04.png#averageHue=%23bcc994&clientId=u2a8b9fb7-3a0f-4&from=paste&id=u34643872&originHeight=156&originWidth=382&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u16a88bbb-f023-45bf-ab77-13866c58f3f&title=%E5%9B%BE1-1%20%E5%B0%86EVPN%E4%BD%9C%E4%B8%BAVXLAN%E7%9A%84%E6%8E%A7%E5%88%B6%E5%B9%B3%E9%9D%A2 "图1-1 将EVPN作为VXLAN的控制平面")<br />EVPN参考了BGP/MPLS IP VPN的机制，通过扩展BGP协议新定义了几种BGP EVPN路由，通过在网络中发布EVPN路由来实现VTEP的自动发现、主机地址学习等行为。采用EVPN作为VXLAN的控制平面具有以下优势：

- 可实现VTEP自动发现、VXLAN隧道自动建立，从而降低网络部署、扩展的难度。
- EVPN可以同时发布二层MAC和三层路由信息。
- 可以减少网络中泛洪流量。
<a name="iek0K"></a>
### MP-BGP基本概念
在深入理解EVPN的工作原理前，先对MP-BGP（MultiProtocol BGP）做下简单回顾：传统的BGP-4使用Update报文在对等体之间交换路由信息。一条Update报文可以通告一类具有相同路径属性的可达路由，这些路由放在NLRI（Network Layer Reachable Information，网络层可达信息）字段中。因为BGP-4只能管理IPv4单播路由信息，为了提供对多种网络层协议的支持（例如IPv6、组播），发展出了MP-BGP。MP-BGP在BGP-4基础上对NLRI作了新扩展。玄机就在于新扩展的NLRI上，扩展之后的NLRI增加了地址族的描述，可以用来区分不同的网络层协议，例如IPv6单播地址族、VPN实例地址族等。<br />类似的，EVPN也是借用了MP-BGP的机制，在L2VPN地址族下定义了新的子地址族——EVPN地址族，在这个地址族下又新增了一种NLRI，即EVPN NLRI。EVPN NLRI定义了几种BGP EVPN路由类型，这些路由可以携带主机IP、MAC、VNI、VRF等信息。这样，当一个VTEP学习到下挂的主机的IP、MAC地址信息后，就可以通过MP-BGP路由将这些信息发送给其他的VTEP，从而在控制平面实现主机IP、MAC地址的学习，抑制了数据平面的泛洪。
<a name="fnL7a"></a>
## 了解EVPN中的几种路由类型
本节介绍EVPN NLRI中定义的几种BGP EVPN路由类型的报文格式及其作用。
<a name="mDL5N"></a>
### EVPN中定义的五种路由类型概览
EVPN NLRI定义了如表1-1所示的五种EVPN路由类型。其中Type1～Type4是在RFC7432中定义的，Type5是在后来的草案中定义的。<br />![表1-1 EVPN路由类型](https://cdn.nlark.com/yuque/0/2023/png/396745/1698629430165-65397600-b147-47bb-9ee9-b66cfaf5d959.png#averageHue=%23f9f8f6&clientId=u2a8b9fb7-3a0f-4&from=paste&id=u36e115e2&originHeight=390&originWidth=572&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u3b5aeb8a-818c-4c43-8460-1f7ee4d6b3e&title=%E8%A1%A81-1%20EVPN%E8%B7%AF%E7%94%B1%E7%B1%BB%E5%9E%8B "表1-1 EVPN路由类型")<br />其中Type1和Type4是用于EVPN ESI场景，本文主要对常见的Type2、Type3、Type5类型的路由进行重点介绍。
<a name="Eq2h0"></a>
### EVPN Type2路由
<a name="zImv2"></a>
#### 格式说明
EVPN Type2路由，也就是MAC/IP路由，主要用于VTEP之间相互通告主机IP、MAC信息。Type2路由的NLRI部分格式如图1-2所示。<br />![图1-2 Type2路由的报文格式](https://cdn.nlark.com/yuque/0/2023/png/396745/1698629430147-ad96e82b-bd75-4010-969e-78e31e1b9d0a.png#averageHue=%23bccce1&clientId=u2a8b9fb7-3a0f-4&from=paste&id=u6b13efdf&originHeight=233&originWidth=275&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u053d261a-ab44-486f-b1e9-cbecaa5119b&title=%E5%9B%BE1-2%20Type2%E8%B7%AF%E7%94%B1%E7%9A%84%E6%8A%A5%E6%96%87%E6%A0%BC%E5%BC%8F "图1-2 Type2路由的报文格式")<br />各字段的解释如下表所示：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1698629430124-a87632b8-6c21-460e-b195-cca43b3c9aab.png#averageHue=%23f7f5f2&clientId=u2a8b9fb7-3a0f-4&from=paste&id=u8ca8c367&originHeight=718&originWidth=563&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u07913886-8aeb-4d00-a5bc-162c790680f&title=)
<a name="AjQmr"></a>
#### 应用说明
Type2路由在VXLAN网络中的使用场景和作用参见下表。<br />![表1-2 Type2路由使用场景说明](https://cdn.nlark.com/yuque/0/2023/png/396745/1698629430530-9556b4a3-087f-498b-8ad0-6c3bd27da20f.png#averageHue=%23f3efeb&clientId=u2a8b9fb7-3a0f-4&from=paste&id=ua519eef6&originHeight=816&originWidth=671&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u4015cd58-c1c1-41e8-8c7f-8a7cf0ecdc5&title=%E8%A1%A81-2%20Type2%E8%B7%AF%E7%94%B1%E4%BD%BF%E7%94%A8%E5%9C%BA%E6%99%AF%E8%AF%B4%E6%98%8E "表1-2 Type2路由使用场景说明")
<a name="vyxsC"></a>
### EVPN Type3路由
<a name="PDbyc"></a>
#### 格式说明
EVPN Type3路由主要用于在VTEP之间相互通告二层VNI、VTEP IP信息，以建立头端复制列表，即用于VTEP的自动发现和VXLAN隧道的动态建立：如果对端VTEP IP地址是三层路由可达的，则建立一条到对端的VXLAN隧道。同时，如果对端VNI与本端相同，则创建一个头端复制表，用于后续BUM报文转发。<br />Type3路由的NLRI是由“前缀”和“PMSI”属性组成，报文格式如图1-3所示。其中VTEP IP信息体现在NLRI的**Originating Router's IP Address**字段中，二层VNI信息则体现在PMSI属性的**MPLS Label**中。<br />![图1-3 Type3路由的报文格式](https://cdn.nlark.com/yuque/0/2023/png/396745/1698629430521-3c692b21-1ed0-42b9-b3af-656cf3480ae7.png#averageHue=%23c3d4e9&clientId=u2a8b9fb7-3a0f-4&from=paste&id=uc7211343&originHeight=318&originWidth=310&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u4fb824f7-411c-4837-a34d-4576d0f7223&title=%E5%9B%BE1-3%20Type3%E8%B7%AF%E7%94%B1%E7%9A%84%E6%8A%A5%E6%96%87%E6%A0%BC%E5%BC%8F "图1-3 Type3路由的报文格式")<br />各字段的解释如下表所示：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1698629430635-ef10e8a3-8f23-4279-a809-71894616d302.png#averageHue=%23f6f4f1&clientId=u2a8b9fb7-3a0f-4&from=paste&id=u913115a0&originHeight=435&originWidth=671&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uab36c56c-525b-4edf-8c2a-2266895c8ed&title=)
<a name="OpIMF"></a>
#### 应用说明
Type3路由动态建立头端复制列表的过程简介请参见本文的EVPN头端复制列表的建立。
<a name="RcY0t"></a>
### EVPN Type5路由
<a name="DhmfS"></a>
#### 格式说明
EVPN Type5路由又称IP前缀路由，主要用于传递网段路由。不同于Type2路由只传递32（IPv4）/128（IPv6）位的主机路由，Type5路由可传递0～32/0～128掩码长度的网段路由。<br />Type5路由的报文格式如图1-3所示。<br />![图1-4 Type5路由的报文格式](https://cdn.nlark.com/yuque/0/2023/png/396745/1698629430637-1c8b13c6-b7c8-4662-a2fa-c985cc9f9c0f.png#averageHue=%23becee2&clientId=u2a8b9fb7-3a0f-4&from=paste&id=ud44ec271&originHeight=197&originWidth=275&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ud14f9643-329e-41d1-8054-4f4044eef78&title=%E5%9B%BE1-4%20Type5%E8%B7%AF%E7%94%B1%E7%9A%84%E6%8A%A5%E6%96%87%E6%A0%BC%E5%BC%8F "图1-4 Type5路由的报文格式")<br />各字段的解释如下表所示：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1698629430646-25960ba5-d5bf-470a-be34-c1347b0e5e3f.png#averageHue=%23f8f6f4&clientId=u2a8b9fb7-3a0f-4&from=paste&id=uad6230ac&originHeight=368&originWidth=671&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u6553260f-e417-462e-b109-5fa351cf6b9&title=)
<a name="N2Ved"></a>
#### 应用说明
该类型路由的IP Prefix Length和IP Prefix字段既可以携带主机IP地址，也可以携带网段地址：

- 当携带主机IP地址时，主要用于分布式网关场景中的主机/网段路由通告，请参见本文的网段路由发布。
- 当携带网段地址时，通过传递该类型路由，可以实现VXLAN网络中的主机访问外部网络。
<a name="viPKW"></a>
## 理解EVPN作为VXLAN控制面的工作过程
BGP EVPN在VXLAN网络中是如何工作的呢？本节将介绍BGP EVPN作为VXLAN控制面的工作过程。<br />在用BGP EVPN方式部署分布式VXLAN网络的场景中，控制平面的流程包括VXLAN隧道建立、MAC地址动态学习；转发平面的流程包括同子网已知单播报文转发、同子网BUM报文转发、跨子网报文转发。BGP EVPN方式实现的功能全面，支持主机IP路由通告、主机MAC地址通告、主机ARP通告等，还可以使能ARP广播抑制功能。如果在VXLAN网络中采用分布式网关，推荐使用BGP EVPN方式。<br />本文下面的内容以Underlay网络和Overlay网络均为IPv4为例，介绍EVPN作为VXLAN控制面的工作过程。
<a name="ZiKtl"></a>
### 使用EVPN学习MAC地址
使用EVPN作为VXLAN的控制平面，可以用EVPN来进行MAC学习，以替代数据平面泛洪方式的MAC学习，减少泛洪流量。使用EVPN来进行MAC学习的过程，是通过在VTEP之间传递Type2路由完成的。<br />下面以图1-5为例，介绍VTEP之间是如何通过EVPN来实现远程主机的MAC学习的。<br />![图1-5 使用EVPN来学习远程主机MAC地址的过程示意图](https://cdn.nlark.com/yuque/0/2023/png/396745/1698629430852-4eb02f47-3c6a-4c30-8343-c91ff80daec0.png#averageHue=%23f1d5ba&clientId=u2a8b9fb7-3a0f-4&from=paste&id=u8541bc83&originHeight=500&originWidth=576&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u4b9a1b6a-c7a7-40bf-aeca-0e34c831ae0&title=%E5%9B%BE1-5%20%E4%BD%BF%E7%94%A8EVPN%E6%9D%A5%E5%AD%A6%E4%B9%A0%E8%BF%9C%E7%A8%8B%E4%B8%BB%E6%9C%BAMAC%E5%9C%B0%E5%9D%80%E7%9A%84%E8%BF%87%E7%A8%8B%E7%A4%BA%E6%84%8F%E5%9B%BE "图1-5 使用EVPN来学习远程主机MAC地址的过程示意图")<br />图中Leaf1和Leaf2作为VTEP，分别连接同网段的主机Host1和Host2，以Leaf1向Leaf2发送Type2路由为例。<br />1、Host1在连接至Leaf1时，通常会触发ARP、DHCP等行为。通过这些流量，Leaf1上就会学习到Host1的MAC信息，记录在本地MAC表中。<br />Leaf1学习到本地主机的MAC表项后，会向其对等体Leaf2发送EVPN Type2路由。该路由会携带本端EVPN实例的ERT、VTEP IP地址、二层VNI、Host1的MAC地址等信息。其中本端的EVPN实例的ERT、VTEP IP地址、二层VNI这些信息来源于本端VTEP上的配置，样例如下：
```
[Leaf1]
bridge-domain 10
 vxlan vni 10    //二层VNI
 evpn
  route-distinguisher 10:1
  vpn-target 0:10 export-extcommunity    //EVPN实例的ERT
  vpn-target 100:5000 export-extcommunity
  vpn-target 0:10 import-extcommunity
#
interface Nve1
 source 1.1.1.1   //Leaf1的VTEP IP地址
 vni 10 head-end peer-list protocol bgp
#
```
2、Leaf2收到Leaf1发来的Type2路由后，能够学习到Host1的MAC地址信息，并将其保存在MAC表中，其下一跳为Leaf1的VTEP IP地址。<br />需要说明的是，Leaf2收到Leaf1发送的EVPN路由时，能否接纳该路由信息，是需要通过EVPN实例的RT（Route Target）值是否匹配来判断的。RT是一种BGP扩展团体属性，用于控制EVPN路由的发布与接收。也就是说，RT决定了本端的EVPN路由可以被哪些对端所接收，以及本端是否接收对端发来的EVPN路由。<br />RT属性分为两类：

- ERT（Export RT）：本端发送EVPN路由时，携带的RT属性设置为ERT。
- IRT（Import RT）：本端在收到对端的EVPN路由时，将路由中携带的ERT与本端的IRT进行比较，只有两者相等时才接收该路由，否则丢弃该路由。

在本例中，Leaf2上接收Leaf1发过来的EVPN路由，则需保证Leaf2上配置的IRT（Import RT）与Leaf1配置的ERT（Export RT）一致，例如Leaf2上EVPN中的IRT配置为0:10，与上文中Leaf1上的ERT一致：
```
[Leaf2]
bridge-domain 10
 vxlan vni 10    //二层VNI
 evpn
  route-distinguisher 10:2
  vpn-target 0:10 export-extcommunity    
  vpn-target 100:5000 export-extcommunity
  vpn-target 0:10 import-extcommunity   //EVPN实例的IRT
#
```
经过以上的流程，在未发送广播请求的情况下，Leaf2就可以学习到Host1的MAC地址。类似的，Leaf1也可以学习到Host2的MAC地址。<br />另外需要强调的是，EVPN只是减少了网络中的流量泛洪，并不会完全避免，例如在以下一些场景：

- 网络中存在“静默”主机的情况，这种情况下主机不会触发ARP、DHCP等行为，导致VTEP学习不到本地主机MAC地址，从而也就无法发送MAC地址信息让其他VTEP学习到。
- 主机首次通信的过程中，主机会发送ARP广播请求报文，这种也会产生泛洪。这种情况还可以通过ARP广播抑制功能来避免泛洪，此部分在后续章节会详细描述，此处不再赘述。
<a name="YVrGS"></a>
### EVPN头端复制列表的建立
EVPN只能相对减少网络中的流量泛洪，并不能完全避免。那么这些不能避免的BUM流量，还是需要建立头端复制列表来进行转发。<br />头端复制列表的建立可以通过手工方式或者EVPN来建立，本文仅描述EVPN方式：

- 手工方式就是为逐个为每个VTEP指定其邻居VTEP，每条列表都需要人工配置。
- EVPN则是通过在VTEP之间发布Type3路由，可以为VTEP设备之间自动创建头端复制列表。

下面以图1-6为列，介绍VTEP之间是如何通过Type3路由建立头端复制列表的。<br />![图1-6 使用EVPN建立头端复制列表示意图](https://cdn.nlark.com/yuque/0/2023/png/396745/1698629430954-82ed0469-d4ae-4d88-97d1-23d0ae5e4daf.png#averageHue=%23f2d9bf&clientId=u2a8b9fb7-3a0f-4&from=paste&id=u42a00267&originHeight=532&originWidth=522&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u313c0f02-5f6b-4e20-8b57-39935c66682&title=%E5%9B%BE1-6%20%E4%BD%BF%E7%94%A8EVPN%E5%BB%BA%E7%AB%8B%E5%A4%B4%E7%AB%AF%E5%A4%8D%E5%88%B6%E5%88%97%E8%A1%A8%E7%A4%BA%E6%84%8F%E5%9B%BE "图1-6 使用EVPN建立头端复制列表示意图")<br />图中Leaf1、Leaf2、Leaf3作为VTEP，以Leaf1向Leaf2、Leaf3发送路由为例。<br />1、在Leaf1上完成VTEP IP、二层VNI、EVPN实例等相关配置后（这些配置的样例如下所示），Leaf1会向对等体Leaf2、Leaf3分别发送EVPN Type3路由。路由中会携带二层VNI、本端VTEP IP、EVPN实例的RD、出方向VPN-Target（ERT）等信息。
```
[Leaf1]
bridge-domain 10
 vxlan vni 10    //二层VNI
 evpn
  route-distinguisher 1:10    //EVPN实例的RD
  vpn-target 0:10 export-extcommunity    //EVPN实例的ERT
  vpn-target 100:5000 export-extcommunity
  vpn-target 0:10 import-extcommunity
#
interface Nve1
 source 1.1.1.1   //Leaf1的VTEP IP地址
 vni 10 head-end peer-list protocol bgp
#
```
2、Leaf2、Leaf3收到Leaf1发来的Type3路由后，如果Leaf1的VTEP IP三层路由可达，则建立一条到Leaf1的二层VXLAN隧道；同时，如果本地有相同的VNI，则建立一条头端复制列表，用于后续广播、组播、未知单播报文的转发。<br />在Leaf2、Leaf3收到Leaf1发送的EVPN路由时，会基于路由携带的RT值（EVPN实例的ERT值）是否与本地EVPN实例的IRT值匹配，来判断是否接纳该路由。<br />经过以上的流程，Leaf2、Leaf3上就能建立到Leaf1的头端复制列表，指导后续BUM报文的转发。类似的，Leaf1上也会建立到Leaf2、Leaf3的头端复制列表。
<a name="HaDvW"></a>
### 使用EVPN发布主机路由和网段路由
<a name="NJV5p"></a>
#### 主机路由发布
EVPN Type2路由不仅可以发布主机MAC地址，还可以发布主机路由信息，这是因为Type2路由还可以携带32位掩码的主机IP地址信息。主机路由的发布可以实现分布式网关场景下跨网段主机之间的互通。VTEP之间需要发布下属主机的IP路由，否则对端VTEP就无法学习到该主机的路由信息，从而没法进行三层转发。简单来说就是“你得告诉我你下面都接了什么网段的路由，否则我怎么知道要发给你呢”。<br />下面以图1-7为列，介绍VTEP之间是如何使用EVPN来发布主机路由的。<br />![图1-7 使用EVPN发布主机路由的示意图](https://cdn.nlark.com/yuque/0/2023/png/396745/1698629431050-8c2a04a4-4573-48e0-bf90-5430a4eb5ad4.png#averageHue=%23f6d3b0&clientId=u2a8b9fb7-3a0f-4&from=paste&id=ub01bceca&originHeight=505&originWidth=631&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u895cd959-38fb-4811-beb5-271042547b4&title=%E5%9B%BE1-7%20%E4%BD%BF%E7%94%A8EVPN%E5%8F%91%E5%B8%83%E4%B8%BB%E6%9C%BA%E8%B7%AF%E7%94%B1%E7%9A%84%E7%A4%BA%E6%84%8F%E5%9B%BE "图1-7 使用EVPN发布主机路由的示意图")<br />图中Leaf1和Leaf2作为VTEP，同时作为三层网关，分别连接不同网段的主机Host1和Host2，以Leaf1向Leaf2发送路由为例。<br />1、Host1在连接至Leaf1时，通常会触发ARP、DHCP等行为。通过这些流量，Leaf1上就会学习到Host1的ARP信息。同时，还可以根据Host1所属的BD域，获取相应的二层VNI、L3 VPN实例及L3 VPN实例关联的三层VNI信息。<br />为什么会有L3 VPN和三层VNI呢？因为同一个Leaf下可能接入多个租户的服务器，而为了实现不同租户之间的隔离，所以就在Leaf上通过创建不同的L3 VPN来隔离不同租户的路由表，从而将不同租户的路由存放在不同的私网路由表中。而三层VNI就是用来标识这些L3 VPN的，当Leaf节点收到对端发送来的数据报文时（报文会携带三层VNI），就根据其三层VNI找到相应的L3 VPN，通过查找该L3 VPN实例下的路由表来进行转发。<br />Leaf1获取的二层VNI、L3 VPN实例及L3 VPN实例关联的三层VNI信息依赖的关键配置示例如下：
```
[Leaf1]
ip vpn-instance vpn1    //L3 VPN实例
 ipv4-family
  route-distinguisher 20:4
  vpn-target 100:5000 export-extcommunity evpn
  vpn-target 100:5000 import-extcommunity evpn
 vxlan vni 5000      //L3 VPN实例关联的三层VNI
#
bridge-domain 10
 vxlan vni 10     //二层VNI
 evpn
  route-distinguisher 10:4
  vpn-target 0:10 export-extcommunity
  vpn-target 100:5000 export-extcommunity
  vpn-target 0:10 import-extcommunity
#
interface Vbdif10    //根据BD信息获取三层Vbdif接口和此接口绑定的L3 VPN实例
 ip binding vpn-instance vpn1 
 ip address 192.168.1.1 255.255.255.0
 mac-address 0000-5e00-0102
 vxlan anycast-gateway enable
 arp collect host enable
#
```
以上这些总结起来就是Leaf1会获取Host1的：**IP + MAC + Host1所属的二层VNI + VBDIF绑定的L3VPN实例的三层VNI，**然后：

- Leaf1上的EVPN实例就可以根据这些信息生成EVPN Type2类型的路由（参见上图中的表格），除了获取的Host1的相关信息外，还携带本端EVPN实例的ERT、路由下一跳（本端VTEP IP）、VTEP的MAC等信息，将其发送给对等体Leaf2。
- Leaf1上的EVPN实例将Host1的IP + MAC + 三层VNI发给本端的L3 VPN实例，从而在本端的L3 VPN实例中生成本地Host1的路由。

2、Leaf2收到Leaf1发来的Type2路由后，能够学习到Host1的IP地址信息，并将其保存在相应的路由表中，其下一跳为Leaf1的VTEP IP地址，同时记录对应的三层VNI信息，处理过程如下：

- 检查该路由的ERT与接收端EVPN实例的IRT是否相同。如果相同，则接收该路由，同时EVPN实例提取其中包含的主机IP+MAC信息，用于主机ARP通告。
- 检查该路由的ERT与接收端L3VPN实例的IRT是否相同（如下表中的举例所示）。如果相同，则接收该路由，同时L3VPN实例提取其中的主机IP地址+三层VNI信息，在其路由表中生成Host1的路由。该路由的下一跳会被设置为Leaf1的VXLAN隧道接口。
```
Leaf1（发送端）

ip vpn-instance vpn1
 ipv4-family
  route-distinguisher 20:2
  vpn-target 100:5000 export-extcommunity evpn
  vpn-target 100:5000 import-extcommunity evpn
 vxlan vni 5000
#
bridge-domain 10
 vxlan vni 10
 evpn
  route-distinguisher 10:2
  vpn-target 100:10 export-extcommunity
  vpn-target 100:5000 export-extcommunity   //发送端EVPN中的ERT
  vpn-target 100:10 import-extcommunity
#
```
```
Leaf2（接收端）

ip vpn-instance vpn1
 ipv4-family
  route-distinguisher 20:3
  vpn-target 100:5000 export-extcommunity evpn
  vpn-target 100:5000 import-extcommunity evpn   //接收端L3 VPN中的IRT（eIRT）
 vxlan vni 5000
#
bridge-domain 20
 vxlan vni 20
 evpn
  route-distinguisher 10:3
  vpn-target 100:20 export-extcommunity
  vpn-target 100:5000 export-extcommunity
  vpn-target 100:20 import-extcommunity
#
```

- 接收端EVPN实例或L3 VPN实例接收该路由后会通过下一跳获取Leaf1的VTEP IP地址，如果该地址三层路由可达，则建立一条到Leaf1的VXLAN隧道。

经过以上的流程，Leaf2就可以学习到Host1的IP路由信息，后续转发至Host1的报文时，可以根据查找路由表进行转发。类似的，Leaf1也可以学习到Host2的IP路由信息。
<a name="hyVht"></a>
### 网段路由发布
网段路由的发布流程与主机路由类似，区别在于网段路由是通过Type5路由发布的，Type2路由只能发布32/128位的主机路由。Type5路由也可以发布32/128位的主机路由，在发布32/128位的主机路由时，功能与Type2路由类似。<br />如果网关设备下连接的网段在整个网络中唯一，则可以配置发布网段路由，否则不能配置发布网段路由。<br />![图1-8 EVPN网段路由发布示意图](https://cdn.nlark.com/yuque/0/2023/png/396745/1698629431123-bdb42167-92b5-4bbe-919f-22b95929b6a9.png#averageHue=%23f4d7b9&clientId=u2a8b9fb7-3a0f-4&from=paste&id=u7b0153ed&originHeight=459&originWidth=626&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ufc9c7404-7aee-42f7-a8b1-ad07a1b34d9&title=%E5%9B%BE1-8%20EVPN%E7%BD%91%E6%AE%B5%E8%B7%AF%E7%94%B1%E5%8F%91%E5%B8%83%E7%A4%BA%E6%84%8F%E5%9B%BE "图1-8 EVPN网段路由发布示意图")<br />下面以图1-8为列，介绍VTEP之间是如何发布网段路由的。图中Leaf1和Leaf2作为VTEP，同时作为三层网关，其中Leaf1连接一个192.168.1.0/24的网段。

- Leaf1收集到本地IP网段路由，把该IP网段路由通过EVPN Type5路由发送给Leaf2。路由中携带有IP前缀、掩码长度、对应VRF的三层VNI等信息（如上图表格所示）。
- Leaf2收到Leaf1发来的Type5路由后，能够学习到IP网段路由信息，并将其保存在相应的路由表中，其下一跳为Leaf1的VTEP IP地址，同时记录对应的三层VNI信息。

Leaf2收到Leaf1发送的EVPN路由时，根据EVPN路由携带的RT值（Type 5路由使用L3 VPN实例的ERT值填充）是否与本地L3 VPN实例的IRT值匹配，来将网段路由添加到对应VRF的路由表中。如果某VRF的IRT值与EVPN路由携带的RT值相同，则接收该路由，同时提取其中的网段路由+三层VNI信息，在其路由表中生成网段路由。该路由的下一跳会被设置为Leaf1的VTEP IP地址。同时，如果Leaf1的VTEP IP地址三层路由可达，则建立一条到Leaf1的VXLAN隧道。
```
Leaf1（发送端）

ip vpn-instance vpn1
 ipv4-family
  route-distinguisher 20:2
  vpn-target 100:5000 export-extcommunity evpn   //Type5路由中发送端的ERT使用L3 VPN实例中的ERT（eERT）
  vpn-target 100:5000 import-extcommunity evpn
 vxlan vni 5000
#
bridge-domain 10
 vxlan vni 10
 evpn
  route-distinguisher 10:2
  vpn-target 100:10 export-extcommunity
  vpn-target 100:5000 export-extcommunity
  vpn-target 100:10 import-extcommunity
#
```
```
Leaf2（接收端）

ip vpn-instance vpn1
 ipv4-family
  route-distinguisher 20:3
  vpn-target 100:5000 export-extcommunity evpn
  vpn-target 100:5000 import-extcommunity evpn   
//接收端L3 VPN实例中的IRT（eIRT）
 vxlan vni 5000
#
bridge-domain 20
 vxlan vni 20
 evpn
  route-distinguisher 10:3
  vpn-target 100:20 export-extcommunity
  vpn-target 100:5000 export-extcommunity
  vpn-target 100:20 import-extcommunity
#
```
经过以上的流程，Leaf2就可以学习到Leaf1的网段路由信息，后续转发至该网段的报文时，可以根据查找路由表进行转发。
<a name="khow4"></a>
## VXLAN流量的转发过程
本文下面的内容以Underlay网络和Overlay网络均为IPv4为例，介绍用BGP EVPN部署的分布式VXLAN网络中，报文的转发过程。
<a name="QJTzr"></a>
### 同子网报文转发
同子网报文转发为二层转发，只在VXLAN二层网关之间进行，三层网关无需感知。
<a name="LWKqP"></a>
#### 同子网已知单播报文转发
如图1-9所示，Host1和Host2同属于一个子网，下面以Host1向Host2发送已知单播报文为例介绍报文在VXLAN网络中的转发流程。<br />![图1-9 同子网已知单播报文转发示意图](https://cdn.nlark.com/yuque/0/2023/png/396745/1698629431104-bcd0840c-f9a2-4d0c-9438-627d0b7eba69.png#averageHue=%23deb991&clientId=u2a8b9fb7-3a0f-4&from=paste&id=u746012d3&originHeight=435&originWidth=555&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u07b7e23b-32bd-49f0-a62e-c2658adefde&title=%E5%9B%BE1-9%20%E5%90%8C%E5%AD%90%E7%BD%91%E5%B7%B2%E7%9F%A5%E5%8D%95%E6%92%AD%E6%8A%A5%E6%96%87%E8%BD%AC%E5%8F%91%E7%A4%BA%E6%84%8F%E5%9B%BE "图1-9 同子网已知单播报文转发示意图")

1. Host1发送目的地址为Host2的报文。如果Host1没有Host2的MAC地址，会先发送广播ARP请求来获取Host2的MAC地址，此处该过程不再详述，认为Host1已经获取了Host2的MAC地址。
2. Leaf1收到Host1的报文后，根据报文入端口或VLAN信息判断其所属的BD，并在该BD内查找出接口（通过上一节使用EVPN学习MAC地址可以知道，Leaf1上会学习到Host2的MAC地址，出接口为VTEP 2.2.2.2）。然后Leaf1会对报文进行VXLAN封装后转发。
3. Leaf2接收到VXLAN报文后，根据报文中VNI获取二层广播域，进行VXLAN解封装，获取内层的二层报文。
4. Leaf2根据内层报文的目的MAC地址，从本地MAC表中找到对应的出接口，然后转发给对应的主机Host2。

Host2向Host1发送报文的过程与上述过程相同。
<a name="ZgRTa"></a>
#### 同子网BUM报文转发
如果是同子网的BUM报文（广播、组播、未知单播），则会向同子网的所有VTEP发送一份广播报文。<br />例如图1-9所示，Host1向外发送广播报文。Leaf1收到Host1的广播报文后，根据报文入端口或VLAN信息判断其所属的BD，并在该BD内查找所有的隧道列表，依据获取的隧道列表进行报文封装后，向所有隧道发送报文，从而将报文转发至同子网的Host2和Host3。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1698629431293-c6cad13d-517b-4526-9b53-a8be40504f41.png#averageHue=%23fbfaf9&clientId=u2a8b9fb7-3a0f-4&from=paste&id=u35aafb11&originHeight=454&originWidth=575&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4f0e3aa1-10c4-4dd4-bad4-f5341a539f0&title=)
<a name="Z2v7S"></a>
### 跨子网报文转发
如图1-10所示，在分布式网关场景下，Leaf1、Leaf2作为VXLAN的三层网关，进行VXLAN封装及三层转发，Spine仅作为VXLAN报文转发节点，不进行VXLAN报文的处理。<br />![图1-10 分布式网关场景下跨子网报文转发示意图](https://cdn.nlark.com/yuque/0/2023/png/396745/1698629431361-0b9cd240-228d-4f6b-94fb-139877b2a4f8.png#averageHue=%23fbfaf9&clientId=u2a8b9fb7-3a0f-4&from=paste&id=ud7471036&originHeight=448&originWidth=638&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ua60db2d3-5d9a-4ae7-863c-f26ddb1171f&title=%E5%9B%BE1-10%20%E5%88%86%E5%B8%83%E5%BC%8F%E7%BD%91%E5%85%B3%E5%9C%BA%E6%99%AF%E4%B8%8B%E8%B7%A8%E5%AD%90%E7%BD%91%E6%8A%A5%E6%96%87%E8%BD%AC%E5%8F%91%E7%A4%BA%E6%84%8F%E5%9B%BE "图1-10 分布式网关场景下跨子网报文转发示意图")<br />以Host1向Host2发送报文为例介绍报文在VXLAN网络中的转发流程：

1. 因为Host1与Host2属于不同网段，所以Host1会先将报文发送给网关（Leaf1），交由网关进行转发。
2. Leaf1接收到来自Host1的报文，根据报文的目的地址判断需要进行三层转发。Leaf1根据报文入端口或VLAN信息判断其所属的BD，找到绑定该BD的L3VPN实例，然后在该L3VPN实例下查找路由表。在前面使用EVPN发布主机路由和网段路由章节已经介绍过，在分布式网关场景下，网关Leaf1会学习到Host2的主机路由。Leaf1根据路由获取三层VNI、下一跳等信息，然后进行VXLAN封装，将报文转发至Leaf2。
3. Leaf2收到VXLAN报文后进行解封装，根据报文携带的三层VNI找到对应的L3VPN实例，通过查找该L3VPN实例下的路由表，获取报文的下一跳是网关接口地址，然后将目的MAC地址替换为Host2的MAC地址，源MAC地址替换为Leaf2网关的MAC地址，转发给Host2。

Host2向Host1发送报文的过程与上述过程相同。
<a name="okTyJ"></a>
## VXLAN网络中的ARP广播抑制
地址解析协议ARP（Address Resolution Protocol）用来将IP地址解析为MAC地址。网络中同网段主机首次通信时，由于没有目标主机的MAC地址信息，因此会发送ARP广播请求来获取目的MAC地址信息。ARP广播请求报文在VXLAN网络中会泛洪转发，大量的ARP报文存在会占用过多的网络资源，导致网络性能下降。<br />为了抑制ARP广播请求给网络带来的负面影响，可以通过ARP广播抑制功能来尽可能的减少ARP报文在VXLAN网络中的泛洪。ARP广播抑制有两种方式，一种是ARP广播变单播的功能，另一种是ARP二层代答功能。
<a name="CfJdU"></a>
### ARP广播变单播
ARP广播变单播，顾名思义，就是将ARP广播报文转变成ARP单播，从而以单播形式进行转发。ARP广播变单播的实现思路是在VXLAN三层网关上根据ARP生成ARP广播抑制表（包括主机IP、MAC、VNI、VTEP IP信息），然后通过EVPN将主机信息发送给二层网关；二层网关在收到ARP广播请求后，直接使用学习到的主机MAC替换原来的全F的广播MAC，从而将广播变为单播进行转发。<br />![图1-11 ARP广播变单播示意图](https://cdn.nlark.com/yuque/0/2023/png/396745/1698629431521-9b9cd5c3-80d0-4ae2-9eba-7ec957caab96.png#averageHue=%23deb688&clientId=u2a8b9fb7-3a0f-4&from=paste&id=u98761c71&originHeight=273&originWidth=525&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u6bf8293d-eed9-4941-a824-122b63c2310&title=%E5%9B%BE1-11%20ARP%E5%B9%BF%E6%92%AD%E5%8F%98%E5%8D%95%E6%92%AD%E7%A4%BA%E6%84%8F%E5%9B%BE "图1-11 ARP广播变单播示意图")<br />以图1-11所示的分布式网关为例，其中Host1和Host2属于同一子网，但是部署在不同的VTEP下。ARP广播变单播过程如下：

1. Leaf2通过Host2发送的ARP报文，可以学习到Host2的ARP表项。然后Leaf2可以根据ARP生成相应的ARP广播抑制表，并通过EVPN向Leaf1发布，这样Leaf1也可以学习到Host2的主机信息。
2. Host1初次访问Host2，发送ARP广播请求来获取Host2的MAC地址。
3. Leaf1收到ARP广播请求后，查询ARP广播抑制表。因为已经有Host2的主机信息，所以Leaf1将ARP请求报文中的全F的广播目的MAC替换为Host2的MAC地址，将ARP广播变为ARP单播，然后再进行VXLAN封装后向Leaf2发送。如果Leaf1上没有Host2的ARP广播抑制表，那么依然按照正常的流程进行广播。
4. Leaf2收到VXLAN报文并解封装后，将ARP请求发送给Host2。

可以看出ARP广播变单播功能强依赖于三层网关，需要三层网关学习到主机的ARP信息，如果三层网关学习不到主机ARP，就不能抑制ARP广播。
<a name="InoJI"></a>
### ARP二层代答
ARP广播变单播的抑制方式需要三层网关的存在，在纯二层网络中，由于不存在三层网关，没有相应的ARP表项，也就无法生成ARP广播抑制表进行ARP抑制。上述二层场景面临的ARP抑制问题，就可以通过ARP二层代答功能来解决。<br />ARP二层代答的实现思路是在二层网关上侦听主机ARP报文，获取ARP报文中的主机信息并生成ARP广播抑制表，然后通过EVPN将主机信息发送给其他二层网关；二层网关在收到ARP广播请求后，根据ARP广播抑制表中的主机信息，直接进行ARP代答。<br />![图1-12 ARP二层代答示意图](https://cdn.nlark.com/yuque/0/2023/png/396745/1698629431626-112b0761-dbf9-4345-a349-2d41e0f4ba19.png#averageHue=%23cda96f&clientId=u2a8b9fb7-3a0f-4&from=paste&id=u7d542f41&originHeight=273&originWidth=521&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u11eafdf4-0f37-4e18-9ec4-79d5ab39e04&title=%E5%9B%BE1-12%20ARP%E4%BA%8C%E5%B1%82%E4%BB%A3%E7%AD%94%E7%A4%BA%E6%84%8F%E5%9B%BE "图1-12 ARP二层代答示意图")<br />以图1-12为例，其中Host1和Host2属于同一子网，ARP二层代答过程如下：

1. Leaf2上开启ARP二层代答功能后，Leaf2会侦听主机发送的ARP报文。当Leaf2接收到Host2的ARP报文后，可以根据ARP生成相应的ARP广播抑制表项，并通过EVPN向Leaf1发布，这样Leaf1也可以学习到Host2的主机信息。
2. Host1初次访问Host2，发送ARP广播请求来获取Host2的MAC地址。
3. Leaf1收到ARP广播请求后，查询ARP广播抑制表。因为已经有Host2的主机信息，所以Leaf1直接对ARP请求进行代答。
4. 如果Leaf1上没有Host2的ARP广播抑制表，那么依然按照正常的流程进行广播。
