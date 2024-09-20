iptables SNAT DNAT
<a name="GNHlY"></a>
### SNAT 策略概述
SNAT全称为：`Source Network Address Translation 源地址转换`<br />SNAT 是 Linux 防火墙的一种地址转换操作，iptables 命令中的一种数据包控制类型；<br />`SNAT作用`：根据指定条件修改数据包的源 IP 地址。<br />随着 IP 地址资源逐渐减少，使企业难以申请公网 IP 地址，或只能承认一个或者少数几个公网 IP 地址的费用，所以在大部分企业中将面临着局域网内的主机接入 Internet 的需求。采用小型的企业网络，通过在网关中应用 SNAT 策略，可解决局域网共享上网的问题。<br />Linux 网关服务器需通过两块网卡 eth1、eth2分别连接 Internet 和 局域网，来分析局域网主机访问 Internet 的情况。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1602235344055-2d143155-9b59-4f33-816b-a2e844481fc0.png#align=left&display=inline&height=371&originHeight=1112&originWidth=1769&size=146928&status=done&style=shadow&width=589.6666666666666)
<a name="YXe0F"></a>
##### 局域网各主机正确设置 IP 地址 / 子网掩码 / 默认网关地址
Eth1: 192.168.3.111/24  Gw : 192.168.3.1<br />Eth2: 192.168.1.7/24  (连接内网网卡不配置网关)
```bash
# ip a
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 16436 qdisc noqueue state UNKNOWN 
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
    inet 127.0.0.1/8 scope host lo
    inet6 ::1/128 scope host 
       valid_lft forever preferred_lft forever
2: eth2: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast state UP qlen 1000
    link/ether 00:0c:29:f9:55:5b brd ff:ff:ff:ff:ff:ff
    inet 192.168.1.7/24 brd 192.168.1.255 scope global eth2
    inet6 2409:8a00:30f8:4d70:20c:29ff:fef9:555b/64 scope global dynamic 
       valid_lft 259010sec preferred_lft 172610sec
    inet6 fe80::20c:29ff:fef9:555b/64 scope link 
       valid_lft forever preferred_lft forever
3: eth1: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast state UP qlen 1000
    link/ether 00:0c:29:f9:55:65 brd ff:ff:ff:ff:ff:ff
    inet 192.168.3.111/24 brd 192.168.3.255 scope global eth1
    inet6 fe80::20c:29ff:fef9:5565/64 scope link 
       valid_lft forever preferred_lft forever
# route -n
Kernel IP routing table
Destination     Gateway         Genmask         Flags Metric Ref    Use Iface
192.168.3.0     0.0.0.0         255.255.255.0   U     1      0        0 eth1
192.168.1.0     0.0.0.0         255.255.255.0   U     1      0        0 eth2
0.0.0.0         192.168.1.1     0.0.0.0         UG    0      0        0 eth2
```
关闭防火墙和 selinux
```bash
# service iptables stop
iptables：将链设置为政策 ACCEPT：filter                    [确定]
iptables：清除防火墙规则：                                 [确定]
iptables：正在卸载模块：                                   [确定]
# setenforce 0
```
<a name="9DzKP"></a>
#### 只开启路由转发，未设置地址转换的情况
作为网关的 Linux 服务器必须打开路由转发，才能与多个网络进行连通。<br />未使用地址转换策略时，从局域网 PC 192.168.1.123 访问 Internet 的数据包经过网关转发后其源 IP 低至保持不变，当 Internet 中的主机收到这样的请求数据包后，响应数据包将无法正确返回，从而导致访问失败。`(私网地址不能在 Internet 中正常路由)`<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1602235371698-b1a8ef8a-01cf-4ea6-b5e3-88dbebbbed9b.png#align=left&display=inline&height=357&originHeight=1070&originWidth=1912&size=193047&status=done&style=shadow&width=637.3333333333334)
<a name="azJDL"></a>
#### 开启路由转发，并设置 SNAT 转换的情况
在网关服务器中正确应用 SNAT 策略，数据包转发情况就不一样了。<br />局域网 PC 访问 Internet 的数据包到达网关服务器时，先进行路由选择，若发现该数据包需要从外网接口 eth1 向外转发，则将其源 IP 地址 192.168.1.123 修改为网关的外网接口地址 192.168.3.111，然后才发送给目标主机 192.168.3.112。<br />从网关服务器的公网 IP 地址提交数据访问请求，目标主机可正确返回响应数据包，来实现局域网 PC 共享同一个公网 IP 地址接入 Internet。<br />SNAT 转换地址的过程中，网关服务器会根据之前建立的 SNAT 映射，将响应数据包正确返回局域网中的源主机。只要连接的第一个包被 SNAT 处理了，这连接及对应数据流的其他包会自动的被进行 SNAT 处理。Internet 中的服务器其实不知道局域网 PC 的 IP 地址，其中间的转换完全由网关主机完成，可起到内网保护的作用。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1602235406405-2c769359-76fa-4c4b-9d33-4dcd867993a0.png#align=left&display=inline&height=359&originHeight=1078&originWidth=1917&size=220659&status=done&style=shadow&width=639)
<a name="nuU6a"></a>
### SNAT 策略应用
`SNAT 策略应用`：是为局域网共享上网提供接入策略，处理数据包的切入时机是在路由选择后进行，是将局域网外发数据包的源 IP 地址修改为网关服务器的外网接口 IP 地址。<br />SNAT 策略用在 nat 表的 postrouting 链。<br />编写 SNAT 策略时，需使用 iptables 命令结合`--to-source IP 地址`选项来指定修改后的源 IP 地址。
<a name="VvsZv"></a>
#### 共享固定 IP 地址上网

- Linux 网关服务器通过两块网卡eth1、eth2分别连接 Internet 和 局域网。<br />
- 所有局域网 PC 的默认网关设置为：192.168.1.7，且已经设置了 DNS 服务器。<br />
- 要求 192.168.3.0/24 网段的 PC 能够通过共享方式正常访问 Internet。<br />
<a name="EuiED"></a>
##### 打开网关的路由转发
IP 转发是实现路由功能的关键，对应 /proc 文件系统中的 ip_forward 设置。<br />当值为 1 时表示开启，为 0 时表示关闭。<br />使用 Linux 主机作为网关设备，可修改`sysctl.conf 配置文件`，需永久开启路由转发功能。
```bash
# vi /etc/sysctl.conf 
7 net.ipv4.ip_forward = 1                   # 将第 7 行的配置由原来的 0 改为 1
# sysctl -p                                 # 读取修改后的配置
```
测试过程中，可临时开启路由转发。
```bash
# echo 1 > /proc/sys/net/ipv4/ip_forward 
# sysctl -w net.ipv4.ip_forward=1
net.ipv4.ip_forward = 1
```
<a name="NTZ0M"></a>
##### 正确设置 SNAT 策略
针对局域网 PC 访问 Internet 的数据包采用 SNAT 策略，并将源地址改为网关的公网 IP 地址，设置防火墙规则，保持 SNAT 策略长期有效，并将相关命令写入到`rc.local`配置文件，方便开机后自动设置。
```bash
# iptables -t nat -A POSTROUTING -s 192.168.1.0/24 -o eth1 -j SNAT--to-source 192.168.3.111
```
<a name="CE0z5"></a>
##### 测试 SNAT 策略共享接入结果
使用局域网中的 PC 便可以正常访问 Internet 中的网站了。<br />被访问的网站服务器，会认为是网关主机 192.168.3.111 在访问获取 web 日志，而并不知道是否是实际企业内网的 PC 地址 192.168.3.123 在访问。<br />启动一台 Windows 设备，根据实际情况配置 IP 地址，并测试连通性；<br />在 Windows 操作系统上安装 IIS 服务，选择 Internet 信息服务选项即可；<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1602232596370-0a5aeef4-a3bb-40bd-a49d-1a9b344767f9.png#align=left&display=inline&height=493&originHeight=1479&originWidth=2813&size=327326&status=done&style=shadow&width=937.6666666666666)<br />安装完毕之后，打开浏览器输入本机的 IP 地址即可访问；
<a name="V8UnP"></a>
#### 共享动态 IP 地址上网
iptables 提供了一个名为 masquerade 的数据包控制类型，可用来修改数据包源 IP 地址，能够自动获取外网接口的 IP 地址，不需要使用`--to-source`指定固定的 IP 地址。<br />若要使用 MASQUERADE 伪装策略，只需去掉 SNAT 策略中的 --to-source IP 地址，然后使用 -j MASQUERADE 指定数据包控制类型。<br />ADSL 宽带链接，通常连接名称为：ppp0、ppp1等等；
```bash
# iptables -t nat -A POSTROUTING -s 192.168.1.0/24 -o ppp0 -j MASQUERADE
```
如果网关使用固定的公网 IP 地址，最好选择 SNAT 策略而不是 masquerade 策略，以减少不必要的系统开销。 <br />**DNAT 策略及应用**<br />`DNAT 全称为：Destination Network Address Translation （目标地址转换）`<br />DNAT 是`Linux 防火墙`的另一种地址转换操作，是`iptables`命令中的一种数据包控制类型；
<a name="x4fzS"></a>
##### 作用：根据指定条件修改数据包的目标 IP 地址和目标端口。
<a name="PyIcA"></a>
### DNAT 策略概述
DNAT：用来修改目标 IP 地址和目标端口，用在 nat 表的 PREROUTING 链和 OUTPUT 链中。<br />与 SNAT 相似，SNAT 用来修改源 IP 地址，用在 nat 表的 POSTROUTING 链。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1602235772957-8f8c6074-1a5a-4d97-8c0e-2d463b049874.png#align=left&display=inline&height=376&originHeight=1128&originWidth=2135&size=189684&status=done&style=shadow&width=711.6666666666666)<br />在 Internet 环境中，所注册的网站域名必须对应合法的公网 IP 地址，Internet PC 将无法访问公司内网的服务器，除非在网关服务器中正确设置 DNAT 策略。<br />Internet PC 提交的 HTTP 请求到企业的网关服务器时，网关服务器首先判断数据包的目标地址和目标端口，发现数据包需要访问本机的 80 端口，则将其目标 IP 地址修改为内网中的网站服务器 IP 地址，然后发送给内部的网站服务器。<br />网关服务器会根据之前建立的 DNAT 映射，修改返回的 HTTP 应答数据包的源 IP 地址，再返回给 Internet PC 。<br />Internet PC 其实不知道企业网站服务器的局域网地址，中间转换是由网关主机完成，通过设置的 DNAT 策略，企业内部的服务器就可以面向 Internet 提供服务。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1602235559326-26eaa94d-0826-4406-a289-ae8064f9a832.png#align=left&display=inline&height=382&originHeight=1145&originWidth=2010&size=221159&status=done&style=shadow&width=670)
<a name="3XnaT"></a>
### DNAT 策略应用
DNAT 应用：是在 Internet 中发布企业内部的服务器，处理数据包的切入时机是在路由选择之前进行。将访问网关外网接口 IP 地址的数据包的目标地址修改为实际提供服务的内部服务器的 IP 地址。<br />使用 iptables 命令设置 DNAT 策略时，需要结合`--to-destination IP 地址`选项来指定内部服务器的 IP 地址。
<a name="VQIH5"></a>
#### 发布企业内部的 web 服务器

- 公司需注册网站域名，IP 地址为 192.168.3.111<br />
- 公司的网站服务器位于局域网内，IP 地址为 192.168.3.112<br />
- 要求能够从 Internet 中通过访问网站域名来查看公司的网站内容<br />
<a name="7J9xS"></a>
##### 打开网关的路由转发
```bash
# vi /etc/sysctl.conf 
7 net.ipv4.ip_forward = 1                   # 将第 7 行的配置由原来的 0 改为 1
# sysctl -p                                 # 读取修改后的配置
```
<a name="hd1es"></a>
##### 正确设置 DNAT 策略
针对 Internet 中的任意主机访问网关 80 端口的数据包，将目标地址修改为位于内网的网站服务器 IP 地址。
```bash
# iptables -t nat -A PREROUTING -i eth2 -d 192.168.1.7 -p tcp --dport 80 -j DNAT --to-destination 192.168.3.112
# iptables -t nat -L -n -v
Chain PREROUTING (policy ACCEPT 0 packets, 0 bytes)
 pkts bytes target     prot opt in     out     source               destination         
    0     0 DNAT       tcp  --  eth2   *       0.0.0.0/0            192.168.1.7         tcp dpt:80 to:192.168.3.112 
Chain POSTROUTING (policy ACCEPT 1 packets, 136 bytes)
 pkts bytes target     prot opt in     out     source               destination         
Chain OUTPUT (policy ACCEPT 1 packets, 136 bytes)
 pkts bytes target     prot opt in     out     source               destination
```
<a name="tgXsm"></a>
##### 测试 DNAT 发布结果
在网站服务器 192.168.3.112 中正确设置，启动 web 服务，测试网页，可自定义修改首页文件，然后通过 Internet 中的客户机访问网站，不做 DNS 解析，也可直接访问。
<a name="dbwqq"></a>
#### 发布企业内部的 OpenSSH 服务器
DNAT 策略：用来修改数据包的目标 IP 地址，但在需要时也可修改目标端口号。
<a name="0Mshr"></a>
##### 配置 OpenSSH 服务
在网关，网站服务器中开启 OpenSSH 服务，分别使用 2345、22 端口。<br />网关的 sshd 服务因直接面向 Internet，因此不使用默认端口。
<a name="U9mbE"></a>
##### 打开网关的路由转发
```bash
# vi /etc/sysctl.conf 
7 net.ipv4.ip_forward = 1                   # 将第 7 行的配置由原来的 0 改为 1
# sysctl -p                                 # 读取修改后的配置
```
<a name="OkFO4"></a>
##### 正确设置 DNAT 策略
网关本机的 sshd 服务直接面向 Internet，不需要地址转换，但网站服务器位于内网，需通过 DNAT 策略进行发布。<br />网关设置防火墙规则，修改访问外网的 IP 地址 2346 端口的数据包，及修改目标地址和端口，便于转发给网站服务器。
```bash
# iptables -t nat -A PREROUTING -i eth2 -d 192.168.1.7 -p tcp --dport 2346 -j DNAT --to-destination 192.168.3.112:22
```
<a name="wu4Kl"></a>
##### 测试 DNAT 发布结果
使用`ssh -p 2345 / 2346 用户名@eth2 IP 地址`即可登录网关服务器
