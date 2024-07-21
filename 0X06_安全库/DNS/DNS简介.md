<a name="la7iW"></a>
## 1、域名与域名服务器
在日常上网过程中，出于好记的原因，人们更喜欢在浏览器中输入网站的域名，而不是 IP 地址。比如想要访问百度，则会输入 _www.baidu.com_，而不是 _202.108.22.5_（或者百度网站的其他 IP）。<br />然而计算机网络通信中所识别的标识并不是域名，而是 IP 地址，因为其可以提供主机在互联网中的位置信息，而且是定长的，路由器等设备更容易处理。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676249313542-f8ff57a8-3cd0-465a-adb4-e0e9f2a6dc7d.png#averageHue=%23f0efee&clientId=uefbb2643-c08c-4&from=paste&id=u85423dde&originHeight=163&originWidth=682&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uec23e3fa-7b8f-415b-b664-c27658daf7d&title=)<br />为了折中人类和计算机不同的偏好，就出现了 DNS（**D**omain **N**ame **S**ystem，域名系统），其主要任务是根据域名查出对应的 IP 地址。<br />下面先介绍一下域名和域名服务器，之后再介绍 DNS 的工作原理。<br />**域名**由若干个英文字符串组成（不区分大小写），各字符串之间用点号『.』分隔连接，其中越靠右的表示域名级别越高。<br />举个例子，百度的域名为 _www.baidu.com_，其中 _com_ 是顶级域名（一级域名），_baidu_ 是二级域名，_www_ 是三级域名。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676249313509-3cd5ca3f-8b0d-4481-9dde-2c17bab4eafa.png#averageHue=%23fcfcfc&clientId=uefbb2643-c08c-4&from=paste&id=u5d33af87&originHeight=200&originWidth=463&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0e08b8da-1620-4f32-af3b-52aa77b6492&title=)<br />**域名服务器**（也称为 DNS 服务器）负责存储域名和 IP 地址的映射关系，当我们需要获取某个域名对应的 IP 地址时，只需要从域名服务器中查询即可。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676249313552-60c7c290-73ff-4ac8-bf6d-e9ad5a6fc6bb.png#averageHue=%23f8f8f8&clientId=uefbb2643-c08c-4&from=paste&id=u79c56525&originHeight=400&originWidth=1071&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8bfa6e04-d2d0-44ad-9e25-d28d1948d4e&title=)<br />由于域名非常非常多，如果都存放在一台域名服务器中，那么不仅查询速度慢，服务器压力大，而且难以保证服务的可靠性。因此，**DNS 采用了分布式的设计方案，大量的域名服务器之间通过层次方式组织，分布在全世界范围内。**<br />一般而言，域名服务器可以分为以下四类：

- 根域名服务器：最高层级的域名服务器，因特网上一共有 13 个根域名服务器（以英文字母 A 到 M 依序命名，格式为[a~m].root-servers.net），**每个根域名服务器都知道所有顶级域名服务器的 IP 地址**，比如知道负责 com 域的顶级域名服务器的 IP 地址。
- 顶级域名服务器：对于每个顶级域名，如 com、org、edu 等，都有对应的顶级域名服务器。**顶级域名服务器知道其所管理的所有权威域名服务器的 IP 地址。**，比如负责 com 域的顶级域名服务器知道负责 baidu.com 域的权威域名服务器 IP 地址。
- 权威域名服务器：一个网站需要将其域名和 IP 地址注册到权威域名服务器中，比如网站 www.baidu.com 的域名和 IP 地址就存储在负责 baidu.com 域的权威域名服务器中。
- 本地域名服务器：本地域名服务器不属于上述域名服务器的层次结构，但是它对域名系统非常重要。每个 ISP（如一个大学、一个公司）都有一个本地域名服务器（也叫默认域名服务器）。当我们的主机想要进行 DNS 查询时，会首先向本地域名服务器发起查询请求，然后本地域名服务器再向域名服务器层次结构中的相关服务器发起查询请求，并将查询结果发送给我们的主机，具体过程在下一节中介绍。

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676249313604-ac44f085-2366-4728-a9b8-08f1600959fe.png#averageHue=%23f8f4f3&clientId=uefbb2643-c08c-4&from=paste&id=u8e56895b&originHeight=524&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc42ac527-b6e0-42cd-bb05-761f3d6832f&title=)
<a name="ymrAI"></a>
## 2、域名解析流程
知道了域名和域名服务器的基础知识后，我们来了解一下**域名解析**的具体流程，以输入百度域名为例，看看我们的主机是如何得到 _www.baidu.com_ 的 IP 地址的。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676249313536-30867299-078f-451c-a222-65435acc7e0d.png#averageHue=%23fcfcfc&clientId=uefbb2643-c08c-4&from=paste&id=u0764b864&originHeight=775&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u11dc915e-da51-4633-9059-9f4c6eb2fb9&title=)

1. 请求主机向本地域名服务器发送 DNS 查询报文，询问 _www.baidu.com_ 的 IP 地址是什么；
2. 本地域名服务器转发此查询报文到**根域名服务器**；
3. **根域名服务器**发现要查询的顶级域名为 _com_，于是向本地域名服务器发送响应报文，报文中封装了负责 _com_ 域名的顶级域名服务器的 IP 地址列表；
4. 本地域名服务器收到根域名服务器响应的报文后，选择其中一个**顶级域名服务器**的 IP 地址，并向其发送查询报文；
5. **顶级域名服务器**发现要查询的二级域名为 _baidu_，于是向本地域名服务器发送响应报文，报文中封装了负责_baidu.com_ 域名的权威域名服务器的 IP 地址列表；
6. 本地域名服务器收到顶级域名服务器响应的报文后，选择其中一个**权威域名服务器**的 IP 地址，并向其发送查询报文；
7. **权威域名服务器**通过查询数据库，找到 _www.baidau.com_ 的 IP 地址，并将此信息封装为一个响应报文，发送给本地域名服务器；
8. 本地域名服务器将响应报文发送给原请求主机。我们的主机就知道了百度的 IP 地址，DNS 查询过程结束。

在此过程中，请求主机与本地域名服务器之间的交互称为递归查询，而本地域名服务器与域名服务器层次结构中相关服务器的交互称为迭代查询。<br />**请求主机是如何知道本地域名服务器的 IP 地址的？**<br />当用户插上网线或者连上 WIFI 后，电脑会通过 DHCP 协议分配一个 IP 地址，与此同时，也会获取到本地域名服务器的 IP 地址！<br />**本地域名服务器是如何知道根域名服务器的 IP 地址的？**<br />因特网上一共有 13 个根域名服务器，它们的 IP 地址是固定不变的，因此被集成在了操作系统中，每台电脑都知道！<br />为了解析出百度域名的 IP 地址，一共发送了 8 份 DNS 报文。用户本来只是想和百度的服务器进行交互，却耗费了大量的时间进行域名解析，**如果每次都这样搞，岂不是得慢死？因此就需要有一些提升域名解析速度的方式。**
<a name="uEqt4"></a>
## 3、提升域名解析速度
<a name="Uttdw"></a>
### （1）TCP or UDP
我们都知道，**TCP 相较于 UDP 更可靠，但是速度更慢**。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676249314030-5ad1006f-215e-40d9-b743-4982f7bcd6ca.png#averageHue=%23fafafa&clientId=uefbb2643-c08c-4&from=paste&id=ud36374af&originHeight=542&originWidth=1023&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue607d6a4-1b0d-4cc3-a4da-a2486f2eac6&title=)<br />DNS 应该采用哪个传输层协议呢？

- 如果采用 TCP 协议，不仅需要三次握手建立连接，而且需要进行拥塞控制等，那么域名解析速度将慢成龟速，不利于用户体验。
- 如果采用 UDP 协议，万一丢包了怎么办？如果解析不出来 IP，怎么访问目标网站？不利于用户体验。

实际上，**DNS 主要使用 UDP，在特殊情况下，也会使用 TCP，端口号都是 53**。<br />一般情况下，DNS 报文都比较小，只需要一个包就能承载所有信息。既然只有一个包，就无需考虑哪个包未送达，直接重发这个包即可，因此无需使用 TCP 那样复杂的协议，直接使用 UDP 协议，DNS 协议自己处理超时和重传问题，以提供可靠性服务。<br />当然有的时候 DNS 报文比较大，比如响应报文中可能一个域名包含有很多 IP 记录，当服务器响应时，会将报文中的 TC 标志位设置为 1，表示响应长度超过了 512 字节，此报文仅仅返回前 512 字节。当我们的主机收到响应后，就会使用 TCP 协议重发原来的查询请求，以获取完整报文。<br />此外，为了防止本地域名服务器（也称主域名服务器）宕机而导致无法对域名进行解析，本机还需要设置一个辅助域名服务器。**当主域名服务器宕机后，由辅助域名服务器继续提供域名解析的服务。**辅助域名服务器会定时（通常是每隔 3小时）向主服务器发送查询请求以实现同步，此时传输数据较多，因而使用 TCP 协议。
<a name="mmcKc"></a>
### （2）DNS 缓存
即使采用 UDP 协议，但是如果每次都需要从根服务器开始一层一层的查询，仍然很慢，且处于层级结构中的域名服务器将会接收到大量的请求，处理速度进一步降低！<br />为了提升域名解析速度并减轻域名服务器的压力，DNS 广泛使用了**缓存**技术。<br />当用户访问了某个网站后，本地域名服务器会将解析出的域名和 IP 地址的映射关系缓存一定时间。**在缓存过期前，用户再访问相同网站时，本地域名服务器就可以直接返回查询结果**，而无需再去询问根域名服务器、顶级域名服务器等，这样就能大大减少传输的 DNS 报文数量！<br />实际上，不仅在本地域名服务器中设置了高度缓存，用户主机也有缓存。对于 Windows 电脑，可以通过命令ipconfig/displaydns查询当前 DNS 缓存，比如当我访问了百度后，本机就会缓存以下信息：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676249313915-abb3a3e3-3b4f-46e1-9446-c72c077d177f.png#averageHue=%23353433&clientId=uefbb2643-c08c-4&from=paste&id=u5dc6cee3&originHeight=400&originWidth=650&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u6001fd44-2902-4287-a460-925a5a71a77&title=)<br />缓存虽然提升了 DNS 解析速度，但并不能保证一致性，因为一个网站的域名和 IP 地址的映射关系并不是永久不变的，可能缓存的解析结果已失效，因而 DNS 缓存时间不能设置太大！<br />Windows 电脑也可以通过命令ipconfig/flushdns清空本机缓存。
<a name="tQAuU"></a>
### （3）切换本地域名服务器
前面提到，在进行域名解析时，主机会向本地域名服务器发起递归查询，如果本地域名服务器的性能较差，或者未正确配置缓存，那么我们上网的速度将会变得非常慢，因此**选择一个好的本地域名服务器将有助于提升冲浪速度**！<br />默认情况下，本机在联网时会通过 DHCP 协议自动获得一个 DNS 服务器地址，那么如果此服务器性能不好，该如何更换呢？<br />以 Windows 为例，可以通过控制面板—>网络和 Internet—>网络连接—>Internet 协议版本4(TCP/IPv4) 修改本地域名服务器的 IP 地址。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676249313971-9bc53ace-15d7-469f-ae00-43a9160d61cd.png#averageHue=%23f4f2f1&clientId=uefbb2643-c08c-4&from=paste&id=u65989411&originHeight=733&originWidth=597&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0d1de421-9227-4a9b-81c8-5d95c32f7c4&title=)<br />互联网上常见的**公共 DNS 服务器**的 IP 地址如下：

| <br /> | 首选 DNS 服务器地址 | 备用 DNS 服务器地址 |
| --- | --- | --- |
| 阿里 | 223.5.5.5 | 223.6.6.6 |
| 腾讯 | 119.29.29.29 | 182.254.116.116 |
| 百度 | 180.76.76.76 | 114.114.114.114 |
| 谷歌 | 8.8.8.8 | 8.8.4.4 |
| 114DNS | 114.114.114.114 | 114.114.115.115 |

一般情况下，自动获取的本地域名服务器与主机位于同一个子网中，速度都挺快的。但是如果在上网过程中，发现打开网页的速度很慢，也可以尝试使用上面的公共 DNS 服务器，说不定网速会有所改善。
<a name="oO9vM"></a>
## 4、DNS 记录和报文
实际上，域名服务器中保存并不仅仅是域名和 IP 地址，而是保存了一个资源记录（Resource Record，RR）。<br />一个资源记录包含四部分内容，分别是 Name，Value，Type，TTL。<br />TTL 指的是记录的生存时间，以秒为单位，它决定了缓存此记录的过期时间。<br />Name 和 Value 的含义随着 Type 的不同而不同，举几个常见例子：

- **当 Type = A 时（A 是 Address 缩写，也可用编号 1 表示），Name 表示域名，Value 表示对应的 IP 地址**，如（_www.example.com，93.184.216.34，A，86400_）。
- **当 Type = NS 时（NS 是 Name Server 缩写，也可用编号 2 表示），Name 表示一个域，Value 为负责该域解析的域名服务器的域名**，如（_baidu.com，ns1.baidu.com，NS，172800_），此记录用于沿着层级结构查询链来路由 DNS 查询。

如果一台域名服务器是用于某特定域名的权威域名服务器，那么其将会有一条包含该域名的 A 记录。<br />如果一台域名服务器不是用于某特定域名的权威域名服务器，那么其将包含一条 NS 记录，该记录用来指定该域名由哪个域名服务器来进行解析；除此之外，它还将包含一条 A 记录，该记录提供了在 NS 记录中的 Value 字段中的域名服务器的 IP 地址。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676249314189-8a0f54b1-ab9f-40b3-8374-936b427f6dcc.png#averageHue=%23f6f5f4&clientId=uefbb2643-c08c-4&from=paste&id=ub307f53d&originHeight=285&originWidth=820&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u16a3a236-571e-4b80-9237-e8fc9208953&title=)<br />接下来介绍一下 DNS 报文的具体内容。DNS 报文分为两类：**查询报文**和**回答报文**，二者有着相同的格式，如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676249314265-0557234b-3449-4446-9f27-e3f9846c557c.png#averageHue=%23f4f3f2&clientId=uefbb2643-c08c-4&from=paste&id=u242db1dd&originHeight=430&originWidth=776&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u689d6e1b-2fc0-4cc8-9d8f-82cbe87e0f6&title=)

- 事务 ID：用于标识 DNS 查询的标识符。查询报文和其对应的回答报文有着相同的事务 ID，因此通过它可以区分 DNS 回答报文是对哪个请求进行响应的。
- 标志：此字段中含有若干标志，比如有一个『QR』标志位用于指出此报文是查询报文（0）还是回答报文（1），再比如有一个『TC』标志位用于指出此报文长度是否大于 512 字节。
- 问题数：对应于下面查询问题的数量（支持同时查询多个域名，通常为一个）。
- 回答资源记录数：对应于下面回答问题相关资源记录的数量（一个域名可能有多个 IP 对应，那么将会有多个回答记录）。
- 权威资源记录数：对应于下面权威域名服务器相关资源记录的数量。
- 附加资源记录数：对应于下面附加信息相关资源记录的数量。
- 查询问题：此区域为查询内容，包含查询域名和查询类型（如  _www.example.com，A_）。
- 回答问题：此区域为查询结果，包含一到多条资源记录（如 _www.example.com，93.184.216.34， A，300_）。
- 权威域名服务器：此区域为其他权威域名服务器的记录，即含有指向权威域名服务器的资源记录，用以继续解析过程。（如 _baidu.com，ns1.baidu.com，NS，172800_）。
- 附加信息：此区域为其他有帮助的信息，比如提供权威域名服务器所对应的 IP 地址。

下面使用 Wireshark 抓一个 DNS 查询报文和回答报文：<br />**查询报文：**<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676249314316-9fe8f87d-bee0-4c6d-b44b-fdc435cc616b.png#averageHue=%23f5f3f2&clientId=uefbb2643-c08c-4&from=paste&id=u2793fdd8&originHeight=492&originWidth=788&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8d3d0d04-c1f5-42e1-a24a-20b8e12da5b&title=)<br />**回答报文：**<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676249314555-96fafc9a-e1a2-4ee0-bc7d-6ac1b2e2bb48.png#averageHue=%23f6f4f3&clientId=uefbb2643-c08c-4&from=paste&id=u497d1f54&originHeight=694&originWidth=785&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5642f2fb-b3aa-458b-9110-e7599f2e00a&title=)
