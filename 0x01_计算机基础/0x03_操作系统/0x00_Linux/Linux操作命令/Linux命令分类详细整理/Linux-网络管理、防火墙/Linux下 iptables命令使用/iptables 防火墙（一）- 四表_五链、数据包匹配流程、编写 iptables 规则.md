Linux iptables 防火墙
<a name="oNi9H"></a>
## 1.Linux 防火墙基础
`Linux防火墙`主要工作在网络层，针对 `TCP/IP` 数据包实施过滤和限制，典型的包过滤防火墙，基于内核编码实现，具有非常稳定的性能和高效率。

- `iptables`：用来管理 Linux 防火墙的命令程序，位于`/sbin/iptables`目录下，属于`用户空间`的防火墙管理体系。
- `netfilter：Linux` 内核中实现包过滤防火墙的内部结构，一般`不以程序`或`文件`的形式存在，属于`内核空间`的防火墙管理体系。

`iptables 的作用`：为包过滤机制的实现提供规则，通过各种不同的规则，来告诉`netfilter`对来自某些源以及前往某些目的或具有某些协议特征的数据包是如何进行处理的。
<a name="CozHk"></a>
## 2.iptables 的表、链结构
每个`规则表`，其实就相当于一个内核空间的容器，按照`规则集`的不同用途进行划分为默认的四个表，在每个`规则表`中包含不同的`规则链`，处理数据包的不同时机分为`五种链`，决定是否过滤或处理数据包的各种规则并按照先后顺序存放在`各规则链`中。

- 规则的作用：对数据包进行过滤或处理；
- 链的作用：容纳多种防火墙规则；

![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1602232033558-7b745063-163a-4dce-a8f0-9e24db63da5e.png#height=429&id=SZgt3&originHeight=1286&originWidth=2041&originalType=binary&ratio=1&rotation=0&showTitle=false&size=192635&status=done&style=shadow&title=&width=680.3333333333334)
<a name="dhCD5"></a>
### 规则表
`iptables`管理着四个不同的规则表，分别由独立的内核模块实现。

- `filter 表`：用来对数据包进行过滤，具体的规则要求决定如何处理一个数据包。<br />对应的内核模块为：`iptable_filter`，其表内包括三个链：`input`、`forward`、`output`;
- `nat 表`：nat 全称：network address translation 网络地址转换，`主要用来修改数据包的 IP 地址、端口号信息`。<br />对应的内核模块为：`iptable_nat`，其表内包括三个链：`prerouting`、`postrouting`、`output`;
- `mangle 表`：主要用来修改数据包的`服务类型`，`生存周期`，为数据包设置标记，实现流量整形、策略路由等。<br />对应的内核模块为：`iptable_mangle`，其表内包括五个链：`prerouting`、`postrouting`、`input`、`output`、`forward`;
- `raw 表`：主要用来决定是否对数据包进行状态跟踪。<br />对应的内核模块为：`iptable_raw`，其表内包括两个链：`output`、`prerouting`;
<a name="FiINE"></a>
### 规则链

- `input 链`：当收到访问防火墙本机地址的数据包时，将应用此链中的规则；
- `output 链`：当防火墙本机向外发送数据包时，将应用此链中的规则；
- `forward 链`：当收到需要通过防火中转发给其他地址的数据包时，将应用此链中的规则；
- `prerouting 链`：在对数据包做路由选择之前，将应用此链中的规则；
- `postrouting 链`：在对数据包做路由选择之后，将应用此链中的规则；

**input 链 和 output 链主要用在主机型防火墙，是针对服务器本机进行保护的防火墙；**<br />**forward 链、prerouting 链、postrouting 链主要用在网络型防火墙，是针对公司内网与 Internet 之间进行安全控制。**
<a name="sGMVM"></a>
## 3.数据包过滤的匹配流程
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1602232095188-2d580380-6983-432a-8fd7-5de19522bde6.png#height=403&id=ZKnXP&originHeight=1209&originWidth=2534&originalType=binary&ratio=1&rotation=0&showTitle=false&size=278522&status=done&style=shadow&title=&width=844.6666666666666)
<a name="GuDmZ"></a>
### 规则表之间的顺序
当数据包到达防火墙时，如果对应的链内有规则存在，将按照顺序依次从`raw 表`→`mangle 表`→`nat 表`→`filter 表`。
<a name="izMd6"></a>
### 规则链之间的顺序

- `入数据流向`：如果是外边的数据包到达防火墙后，要先通过`prerouting 链`：对数据包做路由选择之后，将应用此链中的规则，然后将进行路由选择，确认数据包的目标地址是否是防火墙本机，结合内核传送给`input`链做处理，确认通过之后，便可以交给服务器端来进行响应。
- `转发数据流向`：如果是外边的数据包到达防火墙后，要先通过`prerouting 链`做相关处理，随后进行路由选择，数据包的目标地址是任何其他地址的话，则将通过内核传给`forward`来进行处理是否需要通过或直接丢弃，最后将交给`postrouting 链`来查看是否有需要修改的数据包信息来进行处理。
- `出数据流向`：出流量，其实就是防火墙本机向外的地址发送数据包，首先被`output 链`处理，然后选择路由，然后在`postrouting 链`查看是否修改数据包的信息来进行处理。
<a name="5L8sZ"></a>
### 规则链内部各条防火墙规则之间的顺序
数据包经过每条规则链时，将按照第一条规则、第二条规则……的顺序进行匹配和处理。
<a name="FtxFH"></a>
#### 链内的过滤原则

- 将按照从`匹配到停止`的方式，如找到一条匹配的规则，将不再执行本链中后续的其他规则；
- 如果对比整个链也找不到与数据包相匹配的规则时，将按照该规则链的默认策略进行处理；
<a name="C6UO1"></a>
## 4.编写防火墙规则
<a name="0228b49f"></a>
### iptables 的基本语法命令格式
```
iptables [-t 表名] 管理选项 [链名] [匹配条件] [-j 控制类型]
```

- `表名`、`链名`：指定`iptables`命令所操作的`表`和`链`，未指定表名时将默认使用`filter`表；
- `管理选项`：表示`iptables`规则的操作方式，比如：`插入`、`增加`、`删除`、`查看`等；
- `匹配条件`：指定要处理的数据包的特征，不符合指定条件的数据包不在处理；
- `控制类型`：指数据包的处理方式，比如：`允许`、`拒绝`、`丢弃`等；
<a name="ckpni"></a>
### iptables 防火墙最常用的控制类型

- `accept`：允许数据包；
- `drop`：丢弃数据包，且不给任何回应信息；
- `reject`：拒绝数据包，必要时会给数据发送端一个响应信息；
- `log`：在`/var/log/messages`文件中记录日志信息，然后将数据包传递给下一个地址；
<a name="z0zfY"></a>
### iptables 防火墙的最常用基本操作
```
iptables 命令的常用管理选项
-A:在指定链的末尾添加一条新的规则
-D:删除指定链中的某一条规则，可删除指定序号或具体内容
-I:在指定链中插入一条新规则，未指定序号时默认作为第一条规则
-R:修改、替换指定链中的某一条规则，可指定规则序号或具体内容
-L:列出指定链中所有的规则，未指定链名，则列出表中的所有链
-F:清空指定链中所有的规则，未指定链名，则清空表中的所有链
-P:设置指定链的默认策略
-n:使用数字形式显示输出结果
-v:查看规则列表时显示详细的信息
-h:查看命令帮助信息
--line-numbers:查看规则列表时，同时显示规则在链中的顺序好
```
<a name="YDs8S"></a>
### 添加新的规则
添加新的防火墙规则时，可使用管理选项`-A`：用来追加规则`-l`：用来插入规则；<br />在`filter 表 INPUT 链`的末尾添加一条规则，可使用`-p`协议名来做匹配条件；
```
# iptables -t filter -A INPUT -p tcp -j ACCEPT
```
使用管理选项`-I`，允许同时制定添加规则的顺序号，未指定序号时默认作为第一条规则；<br />添加两条规则分别位于`filter`表的第一条、第二条，将使用默认的`filter`表；
```
# iptables -I INPUT -p udp -j ACCEPT
# iptables -I INPUT 2 -p icmp -j ACCEPT
```
<a name="5c584d45"></a>
### 查看规则列表
查看防火墙规则时，需使用管理选项`-L`并结合`--line-numbers`选项显示各条规则在链内的顺序号；<br />查看`filter 表 INPUT 链`中的所有规则，并显示规则序号；
```
# iptables -L INPUT --line-numbers
Chain INPUT (policy ACCEPT)
num  target     prot opt source               destination         
1    ACCEPT     udp  --  anywhere             anywhere            
2    ACCEPT     icmp --  anywhere             anywhere            
3    ACCEPT     all  --  anywhere             anywhere            state RELATED,ESTABLISHED 
4    ACCEPT     icmp --  anywhere             anywhere            
5    ACCEPT     all  --  anywhere             anywhere            
6    ACCEPT     tcp  --  anywhere             anywhere            state NEW tcp dpt:ssh 
7    REJECT     all  --  anywhere             anywhere            reject-with icmp-host-prohibited 
8    ACCEPT     tcp  --  anywhere             anywhere
```
防火墙规则多时，可以以数字形式显示地址和端口信息，减少地址解析的过程，加快命令的执行速度。<br />以数字形式查看默认表`INPUT 链`中的所有规则，可结合参数`-nL`或者拆分`-n -L`的方式来操作;
```
# iptables -nL INPUT
Chain INPUT (policy ACCEPT)
target     prot opt source               destination         
ACCEPT     udp  --  0.0.0.0/0            0.0.0.0/0           
ACCEPT     icmp --  0.0.0.0/0            0.0.0.0/0           
ACCEPT     all  --  0.0.0.0/0            0.0.0.0/0           state RELATED,ESTABLISHED 
ACCEPT     icmp --  0.0.0.0/0            0.0.0.0/0           
ACCEPT     all  --  0.0.0.0/0            0.0.0.0/0           
ACCEPT     tcp  --  0.0.0.0/0            0.0.0.0/0           state NEW tcp dpt:22 
REJECT     all  --  0.0.0.0/0            0.0.0.0/0           reject-with icmp-host-prohibited 
ACCEPT     tcp  --  0.0.0.0/0            0.0.0.0/0
```
<a name="8CWni"></a>
### 删除、清空规则
删除`filter 表 INPUT 链`中的第三条规则，可使用管理选项`-D`来删除一条防火墙规则；
```
# iptables -D INPUT 3
# iptables -n -L INPUT
```
清空指定链或表中的所有防火墙规则，可使用管理选项`-F`;
```
# iptables -F INPUT
# iptables -n -L INPUT
Chain INPUT (policy ACCEPT)
target     prot opt source               destination
```
省略链名而清空指定表所有链的规则，可使用管理选项`-F`;
```
# iptables -F
# iptables -t nat -F
# iptables -t mangle -F
```
<a name="7SAnI"></a>
### 设置默认策略
默认策略的控制类型分为：`accept`、`drop`两种。<br />将`filter 表`中的`FORWARD 链`的默认策略设置为：丢弃，`OUTPUT 链`的默认策略设置为：允许；
```
# iptables -t filter -P FROWARD DROP
# iptables -P OUTPUT ACCEPT
```
<a name="k4PO3"></a>
## 5.规则的匹配条件
`通用匹配`：称为常规匹配，可独立使用，不依赖于其他条件或扩展模块。
<a name="Uu5bY"></a>
### 协议匹配
编写规则时使用`-p 协议名`的形式指定；<br />用来检查数据包所使用的网络协议，可用的协议类型文件位于：`/etc/procotols`中。
```
# iptables -I INPUT -p icmp -j DROP
# iptables -A FORWARD ! -p icmp -j ACCEPT
```
<a name="kmF9t"></a>
### 地址匹配
编写`iptables`规则时使用`-s 源地址`或`-d 目标地址`的形式指定；<br />用来检查数据包的源地址或目标地址。（IP地址、网段地址）
<a name="Bibyh"></a>
##### 不建议使用主机名、域名地址；
比方说：若要拒绝转发源地址为 192.168.123.123 的数据，但允许转发源地址位于 192.168.1.0/24 网段的数据；
```
# iptables -A FORWARD -s 192.168.123.123 -j REJECT
# iptables -A FORWARD -s 192.168.1.0/24 -j ACCEPT
```
比方说：若要检测到来自某个网站的频繁扫描、登录时，可添加防火墙规则进行封锁；
```
# iptables -I INPUT -s 192.168.123.0/24 -j DROP
# iptables -I FORWARD -s 192.168.123.0/24 -j DROP
```
<a name="a74eedcc"></a>
### 网络接口匹配
编写`iptables`规则时使用`-i 接口名`和`-o 接口名`的形式指定；<br />用来检查数据包从防火墙的哪一个接口进或出的，并且分别对应入站/出站网卡；<br />比方说：若要丢弃从外网接口访问防火墙本机且源地址为私有地址的数据包；
```
# iptables -A INPUT -i eth0 -s 10.0.0.0/8 -j DROP
# iptables -A INPUT -i eth0 -s 172.16.0.0/12 -j DROP
# iptables -A INPUT -i eth0 -s 192.168.0.0/16 -j DROP
```
`隐含匹配`：以指定的协议匹配作为前提条件，对应的功能由`iptables`在需要时自动`隐含`载入内核。<br />常见的隐含匹配：端口匹配、TCP匹配、ICMP匹配；
<a name="eB2Qj"></a>
### 端口匹配
编写`iptables`规则时使用`--sport 源端口`或者`--dport 目标端口`的形式指定；<br />针对的协议类型为：TCP、UDP<br />用来检查数据包的源端口或目标端口。<br />单个端口号或者以冒号`:`分隔的端口范围是可以接受的，但不连续的多个端口是不可以采用的。<br />比方说：允许为网段 192.168.123.0/24 转发 http 查询数据包；
```
# iptables -A FORWARD -s 192.168.123.0/24 -p udp --dport 80 -j ACCEPT
# iptables -A FORWARD -d 192.168.123.0/24 -p udp --dport 80 -j ACCEPT
```
比方说：搭建 ftp 服务时，开放 20、21 端口，以及用于被动模式的端口范围为 24 100~25 200；
```
# iptables -A INPUT -p tcp --dport 20:21 -j ACCEPT
# iptables -A INPUT -p tcp --dport 24100:25200 -j ACCEPT
```
<a name="eZ42j"></a>
### ICMP 匹配
编写`iptables`规则时使用`--icmp-type ICMP 类型`的形式指定；<br />针对的协议：ICMP，用来检查 ICMP 数据包的类型。<br />ICMP 类型使用字符串或数字显示

- `echo-request`：8（ICMP 协议请求）
- `echo-reply`：0（ICMP 协议回显）
- `destination-unreachable`：3（ICMP 协议目标不可达）

比方说：若要禁止从其他主机 ping 本机，但是允许本机 ping 其他主机。
```
# iptables -A INPUT -p icmp --icmp-type 8 -j DROP
# iptables -A INPUT -p icmp --icmp-type 0 -j ACCEPT
# iptables -A INPUT -p icmp --icmp-type 3 -j ACCEPT
# iptables -A INPUT -p icmp -j DROP
```
更多可用的 ICMP 协议类型，可执行`iptables -p icmp -h`进行查看；
<a name="7rHQO"></a>
### 显示匹配
该匹配方式需要有额外的内核模块提供支持，以`-m 模块名称`的形式调用相应的模块，然后设置匹配条件。<br />常见的显示匹配：多端口匹配、IP 范围匹配、MAC 地址匹配、状态匹配；
<a name="6460117f"></a>
#### 多端口匹配
编写`iptables`规则时使用`-m multiport --dports 端口列表`、`-m multiport --sports 端口列表`的形式指定；<br />用来检查数据包的源端口、目标端口、多个端口之间以逗号进行分隔；<br />比方说：若要允许本机开放端口：80、443、110 等等，提供相关服务；
```
# iptables -A INPUT -p tcp -m multiport --dport 80,443,110 -j ACCEPT
```
<a name="1gkGR"></a>
#### IP 范围匹配
编写`iptables`规则时使用`-m iprange --src-range IP 范围`、`-m iprange --dst-range IP 范围`的形式指定；<br />用来检查数据包的源地址、目标地址；<br />IP 范围：`起始地址-结束地址`的形式表示；<br />比方说：要禁止转发源 IP 地址位于 192.168.8.100 与 192.168.8.123 之间的 TCP 数据包；
```
# iptables -A FORWARD -p tcp -m iprange --src-range 192.168.8.100-192.168.8.123 -j ACCEPT
```
<a name="62993957"></a>
#### MAC 地址匹配
编写`iptables`规则时使用`-m mac --mac-source MAC 地址`的形式指定；<br />用来检查数据包的源 MAC 地址，只适用于内部网络；<br />比方说：要根据 MAC 地址封锁主机，禁止其访问本机的任何应用；
```
# iptables -A INPUT -m mac --mac-source MAC 地址 xxx -j DROP
```
<a name="pYYEJ"></a>
#### 状态匹配
编写`iptables`规则时使用`-m state --state 连接状态`的形式指定；<br />基于`iptables`的状态跟踪机制用来检查数据包的连接状态；<br />常见的连接状态包括：new（与任何连接无关）、established（响应请求或已建立连接）、related（与有连接相关性的）；<br />比方说：要禁止转发与正常 TCP 连接无关的非 --syn 请求数据包；
```
# iptables -A FORWARD -m state --state NEW -p tcp ! --syn -j DROP
```
只开放本机的 WEB 服务，但对发给本机的 TCP 数据包进行放行，其他入数据包丢弃；
```
# iptables -I INPUT -p tcp -m multiport --dport 80 -j ACCEPT
# iptables -I INPUT -p tcp -m state --state ESTABLISHED -j ACCEPT
# iptables -P INPUT DROP
```
