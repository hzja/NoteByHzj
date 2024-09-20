Linuxtcpdump
<a name="R2Gv2"></a>
## 前言
Tcpdump是用于分析网络和查找相关网络问题的出色工具。它会在数据包经过时捕获数据包，并显示网络上正在发生的事情和传入情况。该命令的输出显示在 STDOUT 上，也可以存储在文件中。<br />感谢开发人员，他们将Tcpdump保留为开源项目。它可以在Unix和Linux系统上免费获得。视窗有一个“微海外杠TCPDUMP用于视窗”变体，并带有相关的价格标签。<br />tcpdump有一长串可用的选项。在本文中，我将重点介绍经常使用的核心选项。
<a name="qnq6G"></a>
## 检查可用接口
要检查要捕获的所有可用接口，请使用`-D`标志作为：
```
sudo tcpdump -D
```
这将列出系统上的所有接口，包括无线和有线接口等。该标志也可以获得相同的功能：`--list-interfaces`
```
sudo tcpdump --list-interfaces
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668491903177-8b1a34d9-efa5-4918-a835-9cfb2be4ae82.png#averageHue=%234b4684&clientId=u94a05f17-9534-4&from=paste&id=u2fd29177&originHeight=448&originWidth=884&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u446023e7-15d4-4aac-9a81-847ec96b531&title=)
<a name="bgEsB"></a>
## 捕获特定接口的数据包
在不使用任何选项的情况下，Tcpdump 将扫描所有接口。该标志捕获来自特定接口的流量：`-i`
```
tcpdump -i <target-interface>
```
将 替换为要扫描的接口的名称。例如，在 接口 的情况下，此命令将作为：target-interfaceeth0
```
sudo tcpdump -i eth0
```
注意：从现在开始，将使用 or 作为目标接口。因此，无论在何处看到 `-i` 标志，它都将伴随界面或 eth0 eth1 eth0 eth1
<a name="K8HBA"></a>
## 预设捕获计数
该标志可用于预设要捕获的数据包数`-c`<br />例如，将此值设置为 4 以捕获四个数据包。在本例中，该命令将为：
```bash
sudo tcpdump -c 4 -i eth0
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668491903195-f9ac5170-3904-4e11-a864-5eeaa50bbf81.png#averageHue=%2351518d&clientId=u94a05f17-9534-4&from=paste&id=ubd45327c&originHeight=386&originWidth=891&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub9bac07b-0d0a-4aa5-a909-337eede9f1a&title=)如果未指定计数，则将使用组合键或 手动中断捕获操作。ctrl+cctrl+z<br />在下面的文章中，将根据需要将标志与其他标志一起添加。这将有助于我们清晰，轻松地理解命令的输出。`-c`
<a name="Vvne0"></a>
## 获取详细输出
要获取命令的详细输出，可以使用标志：`tcpdump -v`
```bash
sudo tcpdump -c 6 -v -i eth0
```
可以使用更多标志 as 或 进一步提高详细程度。这将在终端上产生更详细的输出：`-v` `-vv` `-vvv`
```bash
sudo tcpdump -vv -i eth0
```
<a name="YdAUH"></a>
## 以 ASCII 格式打印捕获的数据
有时可能要求 Tcp 转储输出采用十六进制或 ASCII 格式。可以使用 ASCII 格式以及 ASCII 和十六进制格式的选项来解决这个问题：`-A- XX`
```bash
sudo tcpdump -XX -i eth0
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668491903198-59a1adce-f209-4e16-a21d-de3c58a05033.png#averageHue=%23310b26&clientId=u94a05f17-9534-4&from=paste&id=udf414938&originHeight=310&originWidth=892&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u32938a82-8231-4776-a151-7d201d5f0a6&title=)
<a name="S08IG"></a>
## 捕获从特定源 IP 发送的数据包
如果要检查来自特定源 IP 地址的流量，请使用以下命令：
```bash
sudo tcpdump -i eth0 src <source-ip-address>
```
将源 IP 作为并查看流量的详细信息：192.168.56.11
```bash
sudo tcpdump -i eth1 -c 5 src 192.168.56.11
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668491903198-7fa3da95-cca4-46ee-acc3-36cca570d42a.png#averageHue=%23310c27&clientId=u94a05f17-9534-4&from=paste&id=u48d366a8&originHeight=222&originWidth=889&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u91d4fd2a-20ac-4a1d-9621-e5197737ee8&title=)此处的计数 5 将仅捕获前五个数据包。
<a name="wRE5y"></a>
## 捕获发送到特定目标 IP 的数据包
如果要检查发送到特定目标 IP 地址的流量，请使用以下命令：
```bash
sudo tcpdump -i eth0 dst <source-ip-address>
```
将目标 IP 作为并查看流量的详细信息：192.168.56.11
```bash
sudo tcpdump -i eth1 -c 5 dst 192.168.56.11
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668491903194-f85c0580-9ad1-4791-9ef4-12bdf2f3bbc3.png#averageHue=%23310b26&clientId=u94a05f17-9534-4&from=paste&id=u7824671d&originHeight=278&originWidth=896&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u41757739-4c01-4582-a59e-f906c69cd94&title=)
<a name="Nfk6K"></a>
## 将过滤选项与 Tcp 转储结合使用
这是缩小捕获数据范围以进行检查的好方法。这将消除不必要的流量并简化工作。为此，可以根据主机、端口、协议和其他条件筛选流量。<br />看看其中的一些：
<a name="ppTVw"></a>
### 端口号
如果要根据端口号过滤流量，例如端口 22，则按如下方式执行命令：tcpdump
```bash
sudo tcpdump -i eth0 port 22
```
此命令将捕获 TCP 和 UDP 流量。
<a name="JtuLq"></a>
### 协议
与端口指令类似，该指令根据特定流量过滤数据包捕获。在这里，可以使用协议名称或协议编号作为参数值：proto
```bash
sudo tcpdump -i eth0 proto tcp
```
```bash
sudo tcpdump -i eth0 proto 6
```
令人惊讶的是，上面的两个命令是等效的。这是因为 是 TCP 的协议编号。6
<a name="STPbX"></a>
### 主机过滤器
host 参数只是使用其 IP 过滤来自特定主机的流量：
```bash
sudo tcpdump -i eth0 host 192.168.56.10
```
这将捕获所有流量并从此主机流出。有趣的是，可以将多个过滤器应用于主机，以针对特定类型的数据包流量。<br />例如：
```bash
sudo tcpdump -i eth1 -c 50 “(host 192.168.56.11) and (port 443 or port 80)"
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668491903911-c544f832-e854-4df2-9550-acba33d2171b.png#averageHue=%23310b26&clientId=u94a05f17-9534-4&from=paste&id=u8b88d272&originHeight=408&originWidth=894&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1e961f66-6a4b-4f37-a847-7f47278995d&title=)在这里，将不同的过滤规则合并到一个规则中。可以看到此规则是过滤和流量。这是因为该规则包含端口 80 和 443（公共网络端口）的筛选器。http https
<a name="lPV6N"></a>
## 保存捕获的数据
如果要将捕获的数据存储在文件中，可以这样操作。
```bash
sudo tcpdump -i eth0 -c 10 -w my_capture.pcap
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668491904254-14a49543-7e7b-4fc5-9f62-c0e352751ec1.png#averageHue=%23300a25&clientId=u94a05f17-9534-4&from=paste&id=u7cf183e9&originHeight=139&originWidth=891&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u677931eb-8a07-436e-bd03-203cc111f4e&title=)将数据包计数保持在较小的值;否则，可能需要手动停止该过程。
<a name="ozRrI"></a>
## 读取捕获的数据
可以使用存储在文件中的数据与Wireshark或任何其他图形网络协议分析器进行分析。.pcap<br />可以使用 tcp 转储本身来读取它。
```bash
tcpdump -r my_capture.pcap
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668491904215-3530cd57-781d-473b-9c3b-55c717bc676d.png#averageHue=%23300b25&clientId=u94a05f17-9534-4&from=paste&id=ubdfaa577&originHeight=169&originWidth=900&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u00976128-c6db-4510-8abf-c93d4c82c22&title=)上面的屏幕截图显示了上述文件的数据。my_capture.pcap
<a name="Fhky2"></a>
## 结语
对如何使用不同的方式使用tcpdump命令有一个很好的想法。当从远程无外设计算机捕获数据包时，这是最佳选择。<br />如果想要一种更直观的方式来理解数据包捕获，请尝试使用Wireshark。
