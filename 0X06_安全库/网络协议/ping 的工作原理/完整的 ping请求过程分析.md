一般来说，用 ping查看网络情况，主要是检查两个指标：

- 第一个是看看是不是超时
- 第二个看看是不是延迟太高

如果超时那么肯定是网络有问题（禁 ping情况除外），如果延迟太高，网络情况肯定也是很糟糕的。<br />ping是如何检查网络的？<br />ping背后的原理到底是啥样的？<br />来跟着 ping命令走一圈，看看 ping是如何工作的
<a name="M9Pn0"></a>
## 1、环境准备和抓包
<a name="sPcDc"></a>
### 1、环境准备
抓包工具：Wireshark 准备两台电脑，进行互 ping操作：

- A电脑（IP地址：192.168.2.135 / MAC地址：98:22:EF:E8:A8:87）
- B电脑（IP地址：192.168.2.179 / MAC地址：90:A4:DE:C2:DF:FE）
<a name="YHQ3L"></a>
### 2、抓包操作
打开 Wireshark，选取指定的网卡进行抓包，进行 ping操作，在 A电脑上 ping B电脑的 IP。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1697162050868-a245605d-0158-4b28-b960-99261eb3e90a.png#averageHue=%2365612e&clientId=ueeb3eb6a-65f4-4&from=paste&id=u3b7d9bd7&originHeight=122&originWidth=468&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc4cc9c88-afbe-4cfe-9334-4f4bacb00d9&title=)<br />抓包情况如下：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1697162050874-5bbad0a3-87d9-4caf-8e3c-ecad22bcac58.jpeg#averageHue=%23b4e6e6&clientId=ueeb3eb6a-65f4-4&from=paste&id=u5d0870a7&originHeight=292&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u621115ee-ed18-44e1-b881-5b4df87acf6&title=)<br />这里先简单的介绍下Wireshark的控制面板，这个面板包含7个字段，分别是：

- NO：编号
- Time：包的时间戳
- Source：源地址
- Destination：目标地址
- Protocol：协议
- Length：包长度
- Info：数据包附加信息
<a name="ZqQpw"></a>
## 2、深入解析
上图中抓包编号 54-132 显示的就是整个 ping命令的过程，ping命令不是依托于 TCP或者 UDP这种传输层协议的，而是依托于 ICMP协议实现的， 那么什么是 ICMP 协议呢？这里简单介绍下：
<a name="MuQ3J"></a>
### 1、ICMP协议的产生背景
[RFC792]中说明了 ICMP产生的原因：<br />由于互联网之间通讯会涉及很多网关和主机，为了能够报告数据错误，所以产生了 ICMP协议。也就是说 ICMP 协议就是为了更高效的转发 IP数据报和提高交付成功的机会。
<a name="kRTBi"></a>
### 2、ICMP协议的数据格式
![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1697162050866-43a71570-220c-48a5-8af5-de70741af802.jpeg#averageHue=%23a7c1b8&clientId=ueeb3eb6a-65f4-4&from=paste&id=u0f94fad8&originHeight=531&originWidth=1004&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7491c281-0462-4cfc-83e6-9f262a03c78&title=)<br />根据上图，可以知道 ICMP协议头包含 4个字节，头部主要用来说明类型和校验 ICMP报文。<br />下图是对应的类型和代码释义列表，后面分析抓包的时候会用到。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1697162050870-58971a65-c037-4d04-b67f-24453c4abf62.jpeg#averageHue=%23cadde2&clientId=ueeb3eb6a-65f4-4&from=paste&id=u610ef462&originHeight=763&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ufced121f-25aa-4436-9aac-7d31cde4e46&title=)<br />简单介绍完了 ICMP，那么抓包过程中出现的 ARP协议是什么呢？同样来简单解释下：
<a name="bUNZj"></a>
### 3、ARP协议
在一个局域网中，计算机通信实际上是依赖于 MAC地址进行通信的，那么 ARP（ AddressResolutionProtocol）的作用就是根据 IP地址查找出对应的 MAC地址。
<a name="TeqNo"></a>
### 4、Ping过程解析
了解了上面的基础概念后，来分析下抓包的数据，其流程如下：

- A 电脑（ 192.168.2.135）发起 ping请求， ping192.168.2.179
- A 电脑广播发起 ARP请求，查询 192.168.2.179的 MAC地址
- B 电脑应答 ARP请求，向 A电脑发起单向应答，告诉 A电脑自己的 MAC地址为 90:A4:DE:C2:DF:FE
- 知道了 MAC地址后，开始进行真正的 ping请求，由于 B电脑可以根据A电脑发送的请求知道 源 MAC地址，所以就可以根据源 MAC地址进行响应了

**上面的请求过程画成流程图比较直观一点：**<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1697162050876-339bfdf9-5f9b-4c0b-b3e4-dbb48f4804d1.jpeg#averageHue=%23fafafa&clientId=ueeb3eb6a-65f4-4&from=paste&id=ube774ad3&originHeight=540&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u77765192-8e26-4e08-9457-82da72ed3d3&title=)<br />观察仔细的朋友，可能已经发现，Ping 4次请求和响应结束后，还有一次 B电脑对 A电脑的 ARP请求，这是为什么？<br />这里猜测应该是有 2个原因：

- 由于 ARP有缓存机制，为了防止 ARP过期，结束后重新更新下 ARP缓存，保证下次请求能去往正确的路径，如果 ARP过期就会导致出现一次错误，从而影响测试准确性。
- 由于 ping命令的响应时间是根据请求包和响应包的时间戳计算出来的，所以一次 ARP过程也是会消耗时间。这里提前缓存最新的 ARP结果就是节省了下次 ping的 ARP时间。

为了验证猜测，再进行一次 ping操作，抓包看看是不是和猜测的一样。此时，计算机里面已经有了ARP的缓存，执行 `ARP -a` 看看缓存的arp列表：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1697162051167-28407654-e828-4a19-acc5-37a55fe90166.jpeg#averageHue=%23101a10&clientId=ueeb3eb6a-65f4-4&from=paste&id=udeb8609c&originHeight=269&originWidth=720&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4e6a7443-5397-4ee4-a4aa-c452f39ddb8&title=)<br />看看第二次 ping的抓包：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1697162051481-60297e4e-93e2-4cf8-b8f1-fcebfab5c0d7.jpeg#averageHue=%23bbe9e3&clientId=ueeb3eb6a-65f4-4&from=paste&id=uf35e09e2&originHeight=346&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u45604dbe-2aa1-4715-8679-1e29ca8fd04&title=)<br />可以看到，上图中在真正 ping之前并没有进行一次 ARP请求。<br />这也就是说，直接拿了缓存中的 ARP来执行了，另外当 B计算机进行响应之前还是进行了一次 ARP请求，它还是要确认下之前的 ARP缓存是否为正确的。<br />结束ping操作之后，同样再发一次 ARP请求，更新下自己的 ARP缓存，这里和猜想基本一致。<br />弄懂了ping的流程之后来解析下之前解释的 ICMP数据结果是否和抓包的一致。<br />来点击一个 ping request看看 ICMP协议详情：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1697162051656-b4a52818-4300-42b7-973c-b6ce509db2aa.jpeg#averageHue=%23e5dae9&clientId=ueeb3eb6a-65f4-4&from=paste&id=u0d90016c&originHeight=365&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubdc4b4e4-a860-4e3f-b77f-29d5edeedc9&title=)<br />图中红框内就行 ICMP协议的详情了，这里的 Type=8,code=0, 校验是正确，且这是一个请求报文。<br />再点击Responseframe:57，这里说明响应报文在序号 57。详情如下：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1697162051745-5b448d94-6b8a-489d-8071-a6f5f6f24c00.jpeg#averageHue=%23c2c1d4&clientId=ueeb3eb6a-65f4-4&from=paste&id=u3ae03225&originHeight=338&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0f5ccfc0-6b2c-44d3-922d-379e03a61ba&title=)<br />上图的响应报文， Type=0,code=0，这里知道就是响应报文了，然后最后就是根据请求和响应的时间戳计算出来的响应延迟。3379.764ms-3376.890ms=2.874ms。
<a name="QfIzE"></a>
## 3、总结
上面分析了一次完整的 ping请求过程。<br />ping命令是依托于 ICMP协议的， ICMP协议的存在就是为了更高效的转发 IP数据报和提高交付成功的机会。 <br />ping命令除了依托于 ICMP，在局域网下还要借助于 ARP协议， ARP协议能根据 IP地址反查出计算机的 MAC地址。<br />另外 ARP是有缓存的，为了保证 ARP的准确性，计算机会更新ARP缓存。<br />学了这个，以后面试再问 ping的详细过程，应该就比较稳了吧。
