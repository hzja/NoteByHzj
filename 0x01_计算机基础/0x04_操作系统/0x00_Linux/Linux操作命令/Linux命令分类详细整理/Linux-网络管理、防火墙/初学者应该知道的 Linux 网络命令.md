Linux
<a name="uzBww"></a>
## 如何找到机器的 IP 地址
这是网络中最基本的问题，它是学习网络的起点。<br />但是，请等一下。
<a name="WkArR"></a>
### 什么是 IP 地址
IP 是“互联网协议”的缩写，它是管理数据如何通过互联网或本地网络发送的协议（一套规则）。<br />IP 地址基本上是一个唯一的地址，用于识别互联网或本地网络上的设备。<br />使用 ifconfig 命令来查找机器的 IP 地址，像这样：
```bash
ifconfig
```
通过这个命令找到机器的 IP 地址。<br />![ifconfig 命令的输出示例](https://cdn.nlark.com/yuque/0/2023/png/396745/1673411395659-a9085068-4e9c-4775-a50c-6d0530517bfa.png#averageHue=%23252d32&clientId=u257b68c6-1dd1-4&from=paste&id=u352c412d&originHeight=636&originWidth=976&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u3cd355d3-8b69-4c1d-82f4-674aeae1d8c&title=ifconfig%20%E5%91%BD%E4%BB%A4%E7%9A%84%E8%BE%93%E5%87%BA%E7%A4%BA%E4%BE%8B "ifconfig 命令的输出示例")<br />在进入每个信息区间之前，可以发现有几个是所有信息区间的共同点。来了解一下它们。
<a name="PDiD2"></a>
### 网络接口的特性
第一行显示 UP、LOOPBACK、RUNNING、NOARP、MULTICAST 等。这些是网络接口的特性。例如，能够进行 BROADCAST，能够进行 MULTICAST。默认情况下，ifconfig 命令只列出 UP 状态设备。该接口也可以处于 down 状态。
<a name="xDLD3"></a>
### 什么是 MTU
MTU 是指最大传输单元。它决定了所发送的数据包的最大有效载荷大小。默认的标准值是 **1500 字节**。然而，可以增加数据包的有效载荷大小（MTU），这样就可以发送更多的数据，提高数据传输率。
<a name="TPa6v"></a>
### ifconfig 中的 inet 行
**inet** 是分配给该指定接口的互联网（IPv4）地址。它将由 DHCP 客户端设置。<br />**Netmask** 是一个 32 位的 掩码，用于将一个 IP 地址段划分为子网，并指定网络的可用主机。<br />**Broadcast address（广播地址）** 指的是同时在指定网络上的所有主机。<br />**Destination adress （目的地）** 是点对点链接另一端的远程主机的地址。<br />**inet6** 是分配给该指定接口的 IPv6 地址。<br />**prefixlen** 被称为前缀长度，它指定了 IP 地址中作为子网掩码的比特数。<br />**scopeid** 是为一个区域分配的 ID。范围是一个拓扑区域，在这个区域内，IPv6 地址可以作为一个接口或一组接口的唯一标识。
<a name="bOsh1"></a>
### Rx 和 Tx
Rx / Tx packets - 显示接收/传输的数据包数量<br />Rx / Tx bytes – 显示桶（buckets）中的数据包大小 Rx / Tx errors – 显示错误数据包的数量 Rx / Tx drop – 显示丢弃的数据包数量 Rx / Tx overrun – 显示溢出数据包的数量<br />上面提到的所有关键词都是不言自明的，除了溢出（overrun）。这里有一个关于溢出（overrun）的定义。<br />**overrun** 是指在一个特定的轮询周期内没有被发送出去的数据包。这是由于调度的原因。它并不表明数据包的失败，只是表明它没有被发送。Overrun packets（溢出数据包）会在下一个周期重新调度，但同一个数据包有可能再次被溢出（overrun）。<br />探讨一下每个信息展示的用途。<br />第一个以 “enx...” 开头的块（以前叫 “eth0”）是用于以太网连接。由于没有连接以太网电缆（网线），所以它没有显示任何数据。<br />![ifconfig 命令中的以太信息](https://cdn.nlark.com/yuque/0/2023/png/396745/1673411395629-c8306447-0563-402f-a032-fe0241fd5aec.png#averageHue=%232b353a&clientId=u257b68c6-1dd1-4&from=paste&id=u6a47137e&originHeight=107&originWidth=566&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u9291b5c0-320d-4341-9e4a-d05de3df5aa&title=ifconfig%20%E5%91%BD%E4%BB%A4%E4%B8%AD%E7%9A%84%E4%BB%A5%E5%A4%AA%E4%BF%A1%E6%81%AF "ifconfig 命令中的以太信息")<br />以 lo开头的块被称为回环接口。这是一个特殊的接口，系统用它来与自己通信。<br />![ifconfig 命令中的 LoopBack 接口信息](https://cdn.nlark.com/yuque/0/2023/png/396745/1673411395643-1a22a0af-815d-48d1-80ba-a223a3db99ef.png#averageHue=%23293338&clientId=u257b68c6-1dd1-4&from=paste&id=ued7a325b&originHeight=149&originWidth=565&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u729c2f8d-a342-4ea5-a411-02afabecc54&title=ifconfig%20%E5%91%BD%E4%BB%A4%E4%B8%AD%E7%9A%84%20LoopBack%20%E6%8E%A5%E5%8F%A3%E4%BF%A1%E6%81%AF "ifconfig 命令中的 LoopBack 接口信息")<br />以 tun0 开头的块被称为隧道接口。它包含你所连接的 VPN 的信息。<br />![ifconfig 命令中的隧道接口信息](https://cdn.nlark.com/yuque/0/2023/png/396745/1673411395658-f1068175-1af1-4c1e-9cdf-55e7ede923d2.png#averageHue=%232a3339&clientId=u257b68c6-1dd1-4&from=paste&id=u7a2e6e59&originHeight=155&originWidth=761&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u1550f2ba-0474-4dbc-bfaa-2866b9fdad1&title=ifconfig%20%E5%91%BD%E4%BB%A4%E4%B8%AD%E7%9A%84%E9%9A%A7%E9%81%93%E6%8E%A5%E5%8F%A3%E4%BF%A1%E6%81%AF "ifconfig 命令中的隧道接口信息")<br />以 wlp2s0 开头的块被称为 PCI 上的无线。这是连接到本地网络 WIFI 的主要接口。<br />![ifconfig 命令中的无线接口信息](https://cdn.nlark.com/yuque/0/2023/png/396745/1673411395625-87fc4bdf-f22c-4fc0-8216-1794a5fd45e6.png#averageHue=%232a3439&clientId=u257b68c6-1dd1-4&from=paste&id=u7b4803a8&originHeight=160&originWidth=650&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u3683fad9-a2ac-41e9-be11-65d911b3539&title=ifconfig%20%E5%91%BD%E4%BB%A4%E4%B8%AD%E7%9A%84%E6%97%A0%E7%BA%BF%E6%8E%A5%E5%8F%A3%E4%BF%A1%E6%81%AF "ifconfig 命令中的无线接口信息")<br />如果连接了 Wifi，应该使用最后一个。
<a name="D8V68"></a>
## 如何使用 Linux 终端下载文件
wget 命令非常灵活，可以在脚本和 cron 工作中使用它。由于 wget 是非交互式的，它可以在后台独立下载资源，并且不要求用户处于活动或登录状态。<br />下面的命令将从 w3schools 网站下载一张图片，作为例子，在当前的文件夹中：
```bash
wget https://www.w3schools.com/html/img_chania.jpg
```
这是从 Linux 终端下载文件的命令。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1673411395979-74eb2b3f-e83b-4f51-ad70-15115f4cb7c0.png#averageHue=%23222a2e&clientId=u257b68c6-1dd1-4&from=paste&id=ue92aaa8f&originHeight=118&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc9501dcf-5ddc-4960-b362-462d6155180&title=)<br />可以传递另一个参数来指定下载文件的目标文件夹，像这样：
```bash
wget https://www.w3schools.com/html/img_chania.jpg /home/user/downloads/pics/
```
这个终端命令下载文件到指定文件夹上。
<a name="DmAlQ"></a>
## 如何使用终端命令确定系统是否连接到了互联网上
通过使用 ping 终端命令来实现这一目标。它看起来像这样：
```bash
ping google.com
```
这是检查互联网连接的终端命令。<br />![ping 命令的输出示例](https://cdn.nlark.com/yuque/0/2023/png/396745/1673411396073-521edf9e-ce50-4a84-8b1f-e8d762daeed7.png#averageHue=%232b353b&clientId=u257b68c6-1dd1-4&from=paste&id=uce5aefd0&originHeight=243&originWidth=869&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u3b154698-c45b-41b8-9ad8-19d4284f07e&title=ping%20%E5%91%BD%E4%BB%A4%E7%9A%84%E8%BE%93%E5%87%BA%E7%A4%BA%E4%BE%8B "ping 命令的输出示例")<br />可以使用 ping 命令来检查网络连接。该命令以 URL 或 IP 地址为参数，向该指定地址发送数据包。然后它打印来自服务器的响应和传输时间。它将连续打印响应，直到取消这个过程（用 CTRL + C）。最后，它将返回以下细节：

1. 收到响应所需的最少时间
2. 接收响应的平均时间
3. 收到回复所需的最长时间

可以使用 `-c` 标志指定要发送的数据包的数量，像这样：
```bash
ping google.com -c 10
```
这是通过发送 10 个数据包来验证连接性的终端命令。<br />还可以使用 -s 标志指定数据包的大小：
```bash
ping google.com -s 40
```
这个终端命令通过发送 40 字节的数据包来验证连接性。<br />还可以使用 -i 标志来指定下一个请求时间：
```bash
ping google.com -i 2
```
用终端命令来验证连接性，两个请求之间的间隔为 2 秒。<br />以及更多。<br />执行上述命令后，希望你能发现你的系统是否连接到了互联网。最有可能的是，你的浏览器将是问题的根源。重新安装浏览器将解决这个问题。
<a name="KxpBE"></a>
## 如何找到一个网站的 IP 地址
在继续前进之前，应该能够回答以下问题：
<a name="eoB0u"></a>
### 什么是 DNS
DNS 是域名系统的意思。使用的每个网站都有一个域名（例如 google.com 或 freecodecamp.org）。这些域名中的每一个都会指向服务器的特定 IP 地址。DNS 基本上是一个系统，它有一个表，将每个域名与 IP 地址映射。<br />现在是时候回到正轨，学习如何找到一个网站的 IP 地址。<br />**nslookup** （代表“名称服务器查询”）是一个查询 DNS 服务器的命令。它是一个网络管理工具，用于查询域名系统（DNS）以获得域名或 IP 地址映射或任何其他特定的 DNS 记录。系统管理员和 DevOps 使用它来解决与 DNS 有关的问题。<br />下面是如何使用它：
```bash
nslookup google.com
```
这是查找任何网站的 IP 地址的终端命令。<br />![nslookup 命令的输出示例](https://cdn.nlark.com/yuque/0/2023/png/396745/1673411396001-5ca2c391-d405-452c-a00b-c7d63d2a47ea.png#averageHue=%2320282c&clientId=u257b68c6-1dd1-4&from=paste&id=u0bba5d1f&originHeight=183&originWidth=1036&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u42971725-dffe-4699-a8d0-c8a625c5159&title=nslookup%20%E5%91%BD%E4%BB%A4%E7%9A%84%E8%BE%93%E5%87%BA%E7%A4%BA%E4%BE%8B "nslookup 命令的输出示例")
<a name="eYY0p"></a>
## 如何知道哪个用户已经登录
Linux 支持多个用户并管理这些用户。每次都可以以不同的用户身份登录。而且可以使用 who 命令来了解以哪个用户的身份登录。
```bash
who
```
这是寻找登录用户的终端命令。<br />它看起来像这样：<br />![在 Linux 终端找到已登录用户的终端命令](https://cdn.nlark.com/yuque/0/2023/png/396745/1673411396077-6afbf07d-ff3a-4125-96ed-6bde9dad2381.png#averageHue=%2320282c&clientId=u257b68c6-1dd1-4&from=paste&id=ue753e18c&originHeight=139&originWidth=407&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u023ae72f-0bbb-40c0-9962-c9aaa26ad54&title=%E5%9C%A8%20Linux%20%E7%BB%88%E7%AB%AF%E6%89%BE%E5%88%B0%E5%B7%B2%E7%99%BB%E5%BD%95%E7%94%A8%E6%88%B7%E7%9A%84%E7%BB%88%E7%AB%AF%E5%91%BD%E4%BB%A4 "在 Linux 终端找到已登录用户的终端命令")
