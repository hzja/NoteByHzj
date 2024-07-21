SSH<br />在工作中遇到此场景，如下两条网络限制下，总部如何访问分公司内部web服务器？

- dmz服务器可以访问总部外网服务器22端口，不可以访问web服务器；
- web服务器不可访问公网，但是到dmz网络无限制。

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650760668701-712c0b72-66a6-4dda-8a47-8e6b9ed0045f.png#clientId=u3313dc30-07c7-4&from=paste&id=u4d72d88b&originHeight=431&originWidth=1065&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u71dc5394-bd7f-43fe-8939-1265dce64f4&title=)<br />初看需求，第一个想到的肯定是内网端口映射到公网，或者vpn，但是不修改网络策略情况下很难实现。有没有别的方法呢，继续从纯网络角度分析现有条件。<br />网络通信是双向的，有请求，有回应，就是俗称的“通”。dmz可以访问外部22端口，代表请求，回包两个通信通道都是通畅的，是否可以借助回包通道，从外部发起到内部的反向访问呢？答案当然是有的，来试一试，需要ssh工具。<br />在dmz执行如下命令。
```bash
[root@dmz]#  ssh -f -N -g -R  6606:10.1.1.1:80 root@115.100.100.100
```
> -f：代表后台运行程序
> -N：表示使用远程端口转发创建ssh隧道
> -g：监听本机所有IP地址
> -R，表示使用远程端口转发创建ssh隧道

命令结合起来什么意思呢，使用root用户远程连接到115.100.100.100，并且远程主机监听6606端口，当访问此端口时，会跳转到dmz的80端口。此过程会使用到ssh隧道。dmz运行之后，总部服务器的已经有了端口监听。
```bash
[root@center]# netstat -tunlp | grep 6606
    tcp        0      0 127.0.0.1:6606          0.0.0.0:*               LISTEN      8616/sshd: root
```
在总部服务器尝试端口提示拒绝，代表网络已经打通了，但是dmz服务器并没有监听80端口，所以报端口拒绝。
```bash
[root@center]# telnet 127.0.0.1 6606
    Trying 127.0.0.1...
    telnet: connect to address 127.0.0.1: Connection refused
```
如法炮制，再把web服务器到dmz的网络反向打通，dmz服务器访问本地80端口时将跳转到web服务器的80端口。
```bash
[root@web]# ssh -f -N -g -R  80:10.1.1.1:80 root@10.1.1.2
```
再次到总部服务器测试访问就能通信了。
```bash
[root@center]# telnet 127.0.0.1 6606
    Trying 127.0.0.1...
    Connected to 127.0.0.1.
    Escape character is '^]'.
```
最后从网络角度来回顾数据包的的转发过程。<br />从总部服务器看到如下信息。
```bash
#dmz服务器以115.100.100.101:29493作源，访问本地22端口，建立了tcp连接。
[root@center]# ss | grep 115.
    tcp    ESTAB      0      0      172.16.1.1:22                115.100.100.101:29493
[root@center]# netstat -tpna | grep 115.
    tcp        0      0 172.16.1.127:22      101.230.91.53:29493     ESTABLISHED 8555/sshd: root
#本地端口也对应到了进程号8616[root@center]#netstat -tunlp | grep 6606
    tcp        0      0 127.0.0.1:6606          0.0.0.0:*               LISTEN      8616/sshd: root
[root@center]# ps -ef | grep 8616
    root      8616  8555  0 Dec03 ?        00:01:04 sshd: root.
```
当总部服务器访问127.0.0.1:6606时，网络连接信息如下。<br />双向通道已经建立
```bash
[root@center]# ss | grep 6606
tcp    ESTAB      0      0      127.0.0.1:6606                 127.0.0.1:51158
    tcp    ESTAB      0      0      127.0.0.1:51158                127.0.0.1:6606
```
最后用图片来展示最终网络转发过程。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650760668672-8662bf83-5dc1-4f7b-8f64-4490607ab0ba.png#clientId=u3313dc30-07c7-4&from=paste&id=u987cee63&originHeight=450&originWidth=1079&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ubef43a58-f1a6-4a24-bb53-14d0a8df22f&title=)<br />dmz发起ssh连接到总部服务器，并且远程端口转发。远程服务器访问转发端口时，数据将封装到回包通道，由于ssh本身加密，外部网络无法知晓网络交互逻辑，从而实现反向访问。
