Linuxiptables
<a name="QGlwq"></a>
## 1、链的解释
<a name="WQett"></a>
### 1.1 PREROUTING
PREROUTING 链是 mangle 和 nat 表中用于修改数据包的报头信息的链。<br />具体来说，PREROUTING 链主要用于处理刚到达本机并在路由转发前的数据包。在该链上添加的规则可以检查数据包的目标地址、端口等信息，然后根据实际需求进行处理，例如将目标地址修改为本地 IP 地址或者其他网络设备的 IP 地址，从而实现端口映射或者 DNAT（Destination NAT）等功能。<br />PREROUTING 链常常用于以下场景：

1. 端口映射：通过在 PREROUTING 链上添加规则，将数据包中的目标端口转发到本地计算机的指定端口，从而实现端口映射的功能。
2. 源地址转换：通过在 PREROUTING 链上添加规则，将数据包中的源 IP 地址替换为其他 IP 地址，从而实现匿名访问或者欺骗攻击等功能。
3. DNAT：通过在 PREROUTING 链上添加规则，将数据包中的目标地址替换为其他 IP 地址，从而实现 NAT 转换、负载均衡等功能。
<a name="snb6p"></a>
### 1.2 POSTROUTING
POSTROUTING 链是 mangle、nat 和 raw 表中用于修改数据包的报头信息的链。该链主要用于处理将要离开本机的数据包，其作用是对数据包的源 IP 地址进行 NAT 转换或者修改规则的匹配等操作。<br />一般来说，POSTROUTING 链被用于以下场景：

1. SNAT：通过在 POSTROUTING 链上添加规则，将数据包中的源 IP 地址替换为其他 IP 地址，从而实现 NAT 转换，将内部 IP 地址转换为公网 IP 地址，从而实现外部网络对本地设备的访问。
2. Masquerade：Masquerade 是一种特殊的 SNAT 操作，通过将数据包源地址都修改为本机的公网 IP 地址来实现 NAT 转换，通常用于连接互联网的本地网络设备，如路由器等。
3. 连接跟踪：POSTROUTING 链上的一些规则可以使连接跟踪模块（conntrack）记录数据包的状态信息，跟踪连接的建立、维护和释放。
4. 防火墙策略：在 POSTROUTING 链上也可以添加防火墙策略，通过设置过滤规则来保护本机或网络设备的安全和隐私。
<a name="fmdWo"></a>
### 1.3 INPUT
INPUT 链是 filter 表中用于处理传入数据包的链。该链主要用于过滤本机接收到的数据包，控制对本机服务的访问。<br />具体来说，INPUT 链可以用于以下场景：

1. 防火墙策略：通过在 INPUT 链上添加过滤规则，可以限制和过滤不符合安全规范和政策的数据包，保护本机或网络设备的安全和隐私。
2. 服务访问控制：通过在 INPUT 链上添加规则，可以限定允许访问本地服务的来源 IP 地址和端口号，防止未经授权的访问和攻击。
3. ICMP 控制：在 INPUT 链上也可以添加规则控制 ICMP 数据包的传入，例如允许某些 ICMP 数据包类型，禁止某些 ICMP 数据包类型，从而保护本机的网络通信稳定性。
4. 连接跟踪：对于已经建立连接的数据包，可以使用连接跟踪模块在 INPUT 链上记录连接状态，并根据实际需求进行处理，如允许、拒绝、重定向等。
<a name="o1Kle"></a>
### 1.4 OUTPUT
OUTPUT 链是 filter 表中用于处理传出数据包的链。该链主要用于过滤本机发送的数据包，控制本机对外部服务的访问。<br />具体来说，OUTPUT 链可以用于以下场景：

1. 防火墙策略：通过在 OUTPUT 链上添加过滤规则，可以限制和过滤不符合安全规范和政策的数据包，阻止本机对外部网络的不当操作和请求。
2. 服务访问控制：通过在 OUTPUT 链上添加规则，可以限定允许本机访问的目的地 IP 地址和端口号，防止访问未经授权或不安全的服务或设备。
3. NAT 转换：在 OUTPUT 链上也可以添加规则实现 SNAT 转换，将本机的内网 IP 地址转换为公网 IP 地址，从而实现外部网络对本机的访问。
4. 连接跟踪：对于已经建立连接的数据包，可以使用连接跟踪模块在 OUTPUT 链上记录连接状态，并根据实际需求进行处理，如允许、拒绝、重定向等。
<a name="h3413"></a>
### 1.5 FORWARD
FORWARD 链是 filter 表中用于处理转发数据包的链。该链主要用于过滤经过本机进行转发的数据包，控制外部网络访问内部网络的流量。<br />具体来说，FORWARD 链可以用于以下场景：

1. 网络隔离：通过在 FORWARD 链上添加规则，可以限制和过滤从外部网络到内部网络的数据包，实现不同安全级别和应用之间的隔离和保护。
2. NAT 转换：在 FORWARD 链上也可以添加规则实现 DNAT 转换，将外部网络对公网 IP 地址的请求转换为内网服务的 IP 地址和端口号，从而实现访问内部服务。
3. 负载均衡：在 FORWARD 链上也可以添加规则实现负载均衡，将从外部网络访问内部服务的请求分发到多个内部服务器，提高服务的可用性和性能。
4. 连接跟踪：对于已经建立连接的数据包，可以使用连接跟踪模块在 FORWARD 链上记录连接状态，并根据实际需求进行处理，如允许、拒绝、重定向等。
<a name="yJtBD"></a>
## 2、表的解释
<a name="HgyEa"></a>
### 2.1 raw 表
Iptables 中的 raw 表用于在经过连接跟踪、NAT 和路由表处理之前对数据包进行处理，因此 raw 表的优先级最高：

- 在收包时：第一关卡（链）是 PREROUTING，第一张表就是 raw 表
- 在发包时，第一关卡（链）是  OUTPUT，第一张表也是 raw 表

在下面这张图中，可以很清晰的看到，raw 表的优先级都是最高的，并且由于他的“特定”用途，导致 raw 也只会被应用在 PREROUTING 和 OUTPUT 链上。<br />![DM_20230318210752_001.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1679145696731-7d9b82c2-21f5-44db-a8e5-e0404d8a3e73.png#averageHue=%23faf0e7&clientId=ubf39ae80-9a3f-4&from=ui&id=u06a724a5&originHeight=533&originWidth=1012&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=2161682&status=done&style=none&taskId=u526cb467-5e4e-42a5-87db-a15692ebdc7&title=)<br />一旦用户在某个链上使用了 raw 表，则处理完 raw 表后将跳过 NAT 表和连接跟踪处理，即不再做地址转换和数据包的连接跟踪处理。这可以应用在那些不需要进行 NAT 的情况下，以提高性能。<br />例如，当一台 web 服务器面对大量的访问请求时，可以使用 raw 表，让 80 端口不再让 iptables 做数据包的连接跟踪处理，从而提高用户的访问速度。<br />下面举两个例子，来说明 raw 表中的规则是如何提高机器性能的

1. 禁止 TCP SYN Cookie 在 Linux 中，允许使用 TCP SYN Cookie 技术来抵御 SYN Flood 攻击。但是，在某些情况下，这种技术可能会影响系统性能。因此，可以使用 raw 表来禁止使用 TCP SYN Cookie，具体规则如下：
```bash
iptables -t raw -A PREROUTING -p tcp --syn -j CT --notrack
```
这个规则的作用是将所有的 TCP SYN 数据包转发给 conntrack 模块进行处理，同时不做任何连接跟踪，进而消除了 TCP SYN Cookie 技术对系统性能的影响。<br />TCP SYN Cookie 是一种防范 SYN Flood 攻击的技术。在 TCP 三次握手过程中，客户端向服务器发送 SYN 数据包，服务器收到之后应该回复一个 SYN+ACK 数据包，并进入 SYN_RCVD 状态等待客户端的 ACK 确认。但是，如果攻击者向服务器发送大量的 SYN 数据包而不回复 ACK 确认，那么服务器就会不断地创建新的半连接队列，耗尽资源，最终导致系统崩溃。TCP SYN Cookie 技术可以有效地抵御这种攻击。

1. 防范 ICMP 攻击 有些攻击者可能会利用 ICMP 协议对目标主机进行攻击。为了防范这种攻击，可以使用 raw 表来拦截所有的 ICMP 数据包，具体规则如下：
```bash
iptables -t raw -A PREROUTING -p icmp -j DROP
```
这个规则的作用是将所有的 ICMP 数据包直接拒绝，从而有效地防止了 ICMP 攻击。<br />通过使用 raw 表，可以避免数据包经过 NAT 表、连接跟踪和路由表处理前的额外处理，进而提高系统的性能。例如，在上述两个例子中，可以直接将数据包传递给底层协议栈，而不需要进行 NAT 或连接跟踪等处理，从而提高了系统的处理速度和吞吐量。
<a name="xtC1R"></a>
### 2.2 filter
filter 表是 iptables 中的默认表，当不使用 `-t` 来指定具体的表时，就会默认使用 filter 表。<br />filter 主要用于过滤数据包，控制网络流量。<br />Filter 表中有三种链（chains）：

1. INPUT 链：处理输入到本地计算机的数据包。例如，当在本地计算机上运行 web 服务器时，这个服务器就会听取特定的端口，以接收来自客户端的 http 请求，这些请求便会经过 INPUT 链处理。可以在 INPUT 链中设置规则或过滤器来决定是否让这些请求通过。
2. OUTPUT 链：处理从本地计算机出去的数据包。例如，当从本地计算机向远程服务器发出 http 请求时，这个请求就会通过 OUTPUT 链处理。同样可以在 OUTPUT 链中设置规则或过滤器来决定是否让这些请求通过。
3. FORWARD 链：处理通过本地计算机的数据包，例如路由器、防火墙等设备，它们需要转发来自一个界面（interface）的数据包到另一个界面。

在 Filter 表中，可以根据源 IP 地址、目标 IP 地址、协议类型、端口号等条件来设置规则或过滤器，以决定数据包是否允许通过。
<a name="KjQXB"></a>
### 2.3 nat
iptables 中的 NAT（Network Address Translation）表用于实现网络地址转换，主要作用是修改数据包的源地址和目的地址，从而实现网络流量的调整和控制。<br />NAT 表包括三个链（chains）：

1. PREROUTING 链：对进入本地计算机之前的数据包进行处理，例如将外部访问本地服务器的请求数据包转发到正确的内部 IP。
2. POSTROUTING 链：对离开本地计算机之后的数据包进行处理，例如将本地计算机发出的请求数据包进行源地址替换。
3. OUTPUT 链：对本地计算机自身发出的数据包进行处理，例如重新定义目标地址和端口，以实现数据包的转发和映射。

NAT 表常用的功能有：

1. IP 地址转换：可以用 NAT 将一个公网 IP 地址映射到多个内网 IP 地址，这将有助于实现有效的地址分配。同样，也可以将局域网内的私网 IP 地址转换成公网 IP 地址，这样就能够在公网上访问到这些内网资源。
2. 端口映射：可以将公网 IP 地址的某个端口映射到内网 IP 地址的某个端口，这样就能在公网上访问到内网的服务了。比如，可以把本地的 Web 服务器（内网 IP 为 192.168.1.100，监听端口为 80）通过 NAT 映射到公网 IP 的 8080 端口上，这样就可以通过访问公网 IP 的 8080 端口来访问内网的 Web 服务器了。

总的来说，NAT 表是 iptables 中非常重要的一部分，通过配置 NAT 规则，可以实现网络地址转换和端口映射等功能，从而方便地管理和控制网络流量，提高网络的安全性和性能。
<a name="Er07y"></a>
### 2.4 mangle
iptables 中的 mangle 表用于指定如何处理数据包，主要作用是对数据包的报头进行修改、标记或重定向。mangle 表具有五个内建链（chains），分别是：

1. PREROUTING 链：处理数据包到达网卡之前的操作。
2. POSTROUTING 链：处理数据包从网卡出去后的操作。
3. INPUT 链：处理数据包到达本地计算机之后的操作。
4. OUTPUT 链：处理数据包离开本地计算机之前的操作。
5. FORWARD 链：处理数据包被转发到其他主机上的操作。

mangle 表的主要作用如下：

1. 修改数据包的 TOS 字段：通过修改 TOS（Type of Service）字段可以实现 QoS（Quality of Service）服务，即为不同的网络流量设置优先级或带宽限制等。
2. 标记数据包：通过标记数据包可以根据其特性进行分类和处理，例如将特定端口的数据包标记为 VoIP 流量，并对其进行特殊处理，以保证其质量和稳定性。
3. 重定向数据包：通过重定向数据包可以将流量引导到不同的路径上，从而实现负载均衡、访问控制等功能。

下面给一些 mangle 的具体规则

1. 将特定 IP 地址的数据包标记为高优先级：
```bash
iptables -t mangle -A PREROUTING -s 192.168.1.100/32 -j MARK --set-mark 10
```

1. 限制特定端口的上传带宽：
```bash
iptables -t mangle -A POSTROUTING -p tcp --sport 80 -j MARK --set-mark 30
iptables -t mangle -A POSTROUTING -m mark --mark 30 -j CLASSIFY --set-class 1:10
iptables -t mangle -A POSTROUTING -m mark --mark 0 -j CLASSIFY --set-class 1:20
tc qdisc add dev eth0 root handle 1: htb
tc class add dev eth0 parent 1: classid 1:1 htb rate 10mbps ceil 10mbps
tc class add dev eth0 parent 1:1 classid 1:10 htb rate 5mbps ceil 10mbps
tc class add dev eth0 parent 1:1 classid 1:20 htb rate 5mbps ceil 10mbps
```

1. 将特定 IP 的 TCP 数据包延迟 100ms：
```bash
iptables -t mangle -A OUTPUT -p tcp -s 192.168.1.100/32 -j DELAY --delay 100ms
```

1. 设置特定端口的 TOS 字段为最高优先级：
```bash
iptables -t mangle -A OUTPUT -p udp --dport 5000 -j TOS --set-tos Minimize-Delay
```
这些规则只是示例，实际应用中需要根据具体需求进行调整。例如，第二个规则中的 Class-Based queuing（CBQ）算法可以替换为其他算法，如 Hierarchical Token Bucket (HTB) 算法、Stochastic Fair Queuing (SFQ) 算法等。
<a name="rQYFz"></a>
## 3、命令速查
<a name="N9LZP"></a>
### 3.1 查询命令
查询包含如下内容

- 查询规则：使用  -L 表示列出规格
- 查询表：使用 -t 查询指定表
- 查询链：如果有需要的话，直接在最后加链名即可，没有指定则查询所有链
```bash
# 不加 -t 表名，默认是 filter，
iptables -t 表名 -L [链名]
```
另外，还有几个附加参数：

- 显示详细信息：使用 -v ，即 verbosr，来显示详细的信息（比如 计数器）
- 计数器显示为精确值：使用 -x，会显示链上包计数器的精确值（不加 -x 显示 322K bytes，加了后显示 334017 bytes）
- 不解析 IP 地址：使用 -n，表示不解析 IP 地址，会加快速度
- 显示规则序号：使用 --line-numbers 或者 --line 来显示规则的序号，在删除或修改规则时有用。

要注意的是，多个短选项是可以合并的，比如下面两条命令是等价的
```bash
iptables -n -v -L
iptables -nvL
```
但长选项是不可以与短选项合并的
```bash
iptables --line -nvL
```
<a name="TixPV"></a>
### 3.2 增加命令
增加一条命令有两种方式：

- 插入：插入表示规则插入在链的指定位置，使用的是 -I 参数（Insert），不指定的话，默认是 1
- 追加：追加表示规则插入在链的最后位置，使用的是 -A 参数（Append）

这里是插入的示例
```bash
# 插入在第 1 条
iptables -I INPUT -s 192.168.1.146 -j ACCEPT

# 插入在第 2 条
iptables -I INPUT 2 -s 192.168.1.146 -j ACCEPT
```
这里是追加的示例
```bash
iptables -A INPUT  -s 192.168.1.146 -j ACCEPT
```
<a name="bSx2o"></a>
### 3.3 删除命令
<a name="hve3x"></a>
#### 清除表中的规则
在执行此动作前，请明确自己在干什么，若有需要请注意保存规则<br />使用 -F 来清空指定表和链的规则：

- 如果该链的默认规则是 ACCEPT，那清空后，所有进入本机的报文都会放行。
- 如果该链的默认规则是 DROP，那清空后，所有进入本机的报文都会拒绝。
```bash
iptables -t filter -F INPUT
```
<a name="YOzll"></a>
#### 删除指定规则
删除指定规则有两种方法：

- 根据规则的编号去删除
- 根据规则的匹配条件与动作去删除

而删除的参数是 `-D` ，下面分别举个例子<br />首先是根据规则编号删除
```bash
# 先使用 --line 查得规则编号
iptables -t <表名> --line -nvL

# 再指定编号去删除（注意要指定链）
iptables -t [表名] -D INPUT <编号>
```
然后是匹配规则删除
```bash
# 先查询具体规则的命令，你也可以使用 iptables -nvL，但是命令要自己写，比较麻烦
iptables-save

# 然后删除（注意 -A 去除）
iptables -D [规则]
```
<a name="cN8Fi"></a>
### 3.4 修改命令
<a name="b3YOm"></a>
#### 修改规则
修改规则也有两种方法：

- 在原来规则上修改
- 删除后再加新规则

**第一种**：在原来的规则上修改<br />使用 `-R` 参数来指定要修改哪条链上的第几条规则
```bash
命令语法：iptables -t 表名 -R 链名 规则序号 规则原本的匹配条件 -j 动作
示例：iptables -t filter -R INPUT 3 -s 192.168.1.146 -j ACCEPT
```
**第二种**：删除后再加新规则

- 删除：参考前面删除的教程
- 添加：参考前面删除的教程
<a name="bDxXY"></a>
#### 修改策略
修改指定表的指定链的默认策略（默认动作），可以使用如下命令。
```bash
命令语法：iptables -t 表名 -P 链名 动作
示例：iptables -t filter -P FORWARD ACCEPT
```
上例表示将filter表中FORWARD链的默认策略修改为ACCEPT
<a name="uwBup"></a>
## 4、一级匹配条件
<a name="Ow65K"></a>
### 4.1 匹配源地址
```bash
# 匹配多个 IP
iptables -t filter -I INPUT -s 192.168.1.111,192.168.1.118 -j DROP

# 匹配网段
iptables -t filter -I INPUT -s 192.168.1.0/24 -j ACCEPT

#匹配取反
iptables -t filter -I INPUT ! -s 192.168.1.0/24 -j ACCEPT
```
<a name="rEARo"></a>
### 4.2 匹配目标地址
```bash
# 匹配多个 IP
iptables -t filter -I OUTPUT -d 192.168.1.111,192.168.1.118 -j DROP

# 匹配网段
iptables -t filter -I INPUT -d 192.168.1.0/24 -j ACCEPT

#匹配取反
iptables -t filter -I INPUT ! -d 192.168.1.0/24 -j ACCEPT
```
<a name="sAi8A"></a>
### 4.3 匹配协议
可以匹配的协议类型tcp、udp、udplite、icmp、esp、ah、sctp等（centos7中还支持icmpv6、mh）
```bash
# 示例如下
iptables -t filter -I INPUT -p tcp -s 192.168.1.146 -j ACCEPT
iptables -t filter -I INPUT ! -p udp -s 192.168.1.146 -j ACCEPT
```
<a name="IVRZj"></a>
### 4.4 匹配网卡
匹配流入网卡，在OUTPUT链与POSTROUTING链中不能使用此选项。
```bash
#示例如下
iptables -t filter -I INPUT -p icmp -i eth4 -j DROP

iptables -t filter -I INPUT -p icmp ! -i eth4 -j DROP
```
匹配流出网卡，在INPUT链与PREROUTING链中不能使用此选项。
```bash
#示例如下
iptables -t filter -I OUTPUT -p icmp -o eth4 -j DROP

iptables -t filter -I OUTPUT -p icmp ! -o eth4 -j DROP
```
<a name="gN0Q2"></a>
## 5、扩展匹配条件
扩展匹配需要模块支持，常见的扩展模块有：

- tcp
- udp
<a name="tPsSM"></a>
### 5.1 匹配连续 ip
前面一级匹配条件中，`-s` 能匹配 ip，能匹配 ip 段，能匹配离散 ip，但不能匹配随意的连续 ip。<br />若想匹配随意的连续 ip，可以使用 iprange 模块
```bash
iptables -A INPUT -d 172.16.1.100 -p tcp --dport 80 -m iprange --src-range 172.16.1.5-172.16.1.10 -j DROP
```
<a name="dle52"></a>
### 5.2 匹配端口
要使用扩展端口，必须先使用 `-m` 参数指定对应模块，如果指定了协议，则默认会使用与协议同名的模块。<br />例如，下面两条命令等价
```bash
iptables -t filter -I INPUT -s 192.168.1.146 -p tcp --sport 22 -j REJECT
iptables -t filter -I INPUT -s 192.168.1.146 -p tcp -m tcp --sport 22 -j REJECT
```
在与协议同名的模块中，都支持对端口进行匹配，包括：

- 源端口：使用 `--sport`
- 目标端口：使用 `--dport`

并且这些端口可以是一个连续的范围（以下例子中 dport 和 sport 通用，只举一个例子）
```bash
#示例如下
iptables -t filter -I OUTPUT -d 192.168.1.146 -p tcp -m tcp --sport 22 -j REJECT

# 连续端口
iptables -t filter -I INPUT -s 192.168.1.146 -p tcp -m tcp --dport 22:25 -j REJECT

# 匹配 0-22 和 80-65535 的端口
iptables -t filter -I INPUT -s 192.168.1.146 -p tcp -m tcp --dport :22 -j REJECT
iptables -t filter -I INPUT -s 192.168.1.146 -p tcp -m tcp --dport 80: -j REJECT

# 端口匹配取反
iptables -t filter -I OUTPUT -d 192.168.1.146 -p tcp -m tcp ! --sport 22 -j ACCEPT
```
如果要匹配不连续（离散）的端口，则需要用到 multiport 模块，但最多也只能指定15个端口
```bash
iptables -t filter -I INPUT -s 192.168.1.146 -p tcp -m multiport --dport 22,80 -j REJECT
```
<a name="NfJ74"></a>
### 5.3 匹配 mac
mac 模块可以指明源MAC地址,，适用于：PREROUTING, FORWARD, INPUT chains
```bash
iptables -A INPUT -s 172.16.0.100 -m mac  --mac-source  00:50:56:12:34:56 -j ACCEPT
iptables -A INPUT -s 172.16.0.100  -j REJECT
```
<a name="bTP9Q"></a>
### 5.4 匹配报文数据
使用 string 模块，可以对报文中的应用层数据做字符串模式匹配检测。<br />示例如下
```bash
iptables -A OUTPUT -p tcp --sport 80 -m string --algo bm --from 62  --string   "google" -j REJECT
```
对应的参数解释

- `--algo`：字符串匹配检测算法，可选值有：bm （Boyer-Moore）与 kmp （Knuth-Pratt-Morris）
- `--from`：开始偏移
- `--to`：结束偏移
- [!] `--string`：匹配的字符串（是否取反）
- [!] `--hex-string`：匹配字符串模式，16进制格式（是否取反）
```bash
--algo {bm|kmp} 字符串匹配检测算法
    bm：Boyer-Moore
    kmp：
--from offset       开始偏移
--to offset         结束偏移
[!] --string pattern    要检测的字符串模式
[!] --hex-string pattern要检测字符串模式，16进制格式
```
<a name="nDwaU"></a>
### 5.5 匹配收发速率
limit 模块有两个参数<br />`-limit-burst`：类比”令牌桶”算法，此选项用于指定令牌桶中令牌的最大数量<br />`-limit`：类比”令牌桶”算法，此选项用于指定令牌桶中生成新令牌的频率，可用时间单位有second、minute 、hour、day。
```bash
iptables -t filter -I INPUT -p icmp -m limit --limit-burst 3 --limit 10/minute -j ACCEPT
iptables -t filter -A INPUT -p icmp -j REJECT
```
<a name="sFWbu"></a>
### 5.6 匹配并发数
connlimit 模块是 iptables 中的一个扩展模块，用于控制连接数，限制每个源 IP 地址的并发连接数。<br />使用 connlimit 模块可以有效地保护服务器免受 DDoS（分布式拒绝服务）攻击，防止某个 IP 地址通过大量并发连接造成服务器负载过高，影响正常的网络服务。<br />connlimit 模块的工作原理是通过检查 iptables 所匹配到的连接数来实现连接限制。用户可以设置多种参数，如 `--connlimit-above` 用于限制允许的最大连接数，`--connlimit-mask` 用于指定判断连接数的掩码，`--connlimit-upto` 用于限制每个 IP 地址的允许最大连接数。<br />下面是一个示例规则，用于限制每个 IP 地址在 TCP 端口 80 上的最大连接数为 10：
```bash
iptables -I INPUT -p tcp --dport 80 -m connlimit --connlimit-above 10 -j DROP
```
上述规则表示如果某个 IP 地址建立的连接数超过了 10，则将该 IP 地址的所有连接都丢弃。<br />除了限制连接数之外，还可以在特定情况下排除某些IP地址对于连接数控制的限制，例如允许某些代理服务器或特定的IP地址建立更多的连接。具体操作可以使用感叹号 "!" 对特定的IP地址进行标记以进行放开，例如：
```bash
iptables -I INPUT -p tcp --dport 80 -m connlimit --connlimit-above 10 --connlimit-mask 32 ! -s 192.168.1.1 -j DROP
```
这条规则允许IP地址为192.168.1.1的主机建立更多的连接，而其他主机则被限制连接数不超过10。
<a name="bIIyl"></a>
### 5.7 匹配状态
state 模块是iptables中的一个扩展模块，用于匹配TCP连接的状态。<br />TCP连接有多种状态，包括ESTABLISHED、NEW、RELATED、INVALID等。ESTABLISHED表示连接已经建立，可以进行正常的数据传输；NEW表示新的连接请求，即SYN标志位被设置；RELATED表示与已经建立的连接相关的连接，例如FTP传输和DNS查询使用的UDP连接；INVALID表示无效的连接，可能由于网络原因等导致的连接错误。<br />state模块可以帮助iptables根据连接状态进行过滤，从而有效地保护服务器免受攻击。<br />下面是一些常见的用法：
<a name="FgTt4"></a>
#### 允许所有以太网帧（包括ARP请求）进入本机
```basic
iptables -A INPUT -i eth0 -p all -m state --state RELATED,ESTABLISHED -j ACCEPT
```
<a name="uqOmj"></a>
#### 允许SSH流量进入本机
```bash
iptables -A INPUT -i eth0 -p tcp --dport 22 -m state --state NEW,ESTABLISHED -j ACCEPT
```
<a name="mg3V1"></a>
#### 允许PING流量通过本机
```bash
iptables -A INPUT -i eth0 -p icmp --icmp-type echo-request -m state --state NEW,ESTABLISHED,RELATED -j ACCEPT
```
<a name="gp259"></a>
#### 允许HTTP/HTTPS流量进入本机
```bash
iptables -A INPUT -i eth0 -p tcp --dport 80 -m state --state NEW,ESTABLISHED -j ACCEPT
iptables -A INPUT -i eth0 -p tcp --dport 443 -m state --state NEW,ESTABLISHED -j ACCEPT
```
<a name="gEPQj"></a>
## 6、设定动作
使用 `-j` （即 jump）来指定动作，动作可以是内置动作，也可以是自定义动作

- 内置动作：如 ACCEPT，DROP，REJECT
- 自定义动作：参见后面自定义链
<a name="x1AIg"></a>
### 6.1 ACCEPT
常规命令如下
```bash
iptables -I INPUT -s 192.168.1.146 -j ACCEPT
```
<a name="UmNBs"></a>
### 6.2 REJECT
常规命令如下
```bash
iptables -I INPUT -s 192.168.1.146 -j REJECT
```
还可以用 `-reject-with` 来设置提示信息，可选值有：

- icmp-net-unreachable
- icmp-host-unreachable
- icmp-port-unreachable
- icmp-proto-unreachable
- icmp-net-prohibited
- icmp-host-pro-hibited
- icmp-admin-prohibited

当不设置任何值时，默认值为icmp-port-unreachable。
```bash
iptables -I INPUT -s 192.168.1.146 -j REJECT –reject-with icmp-host-unreachable
```
<a name="udJcr"></a>
### 6.3 DROP
常规命令如下
```bash
iptables -I INPUT -s 192.168.1.146 -j DROP
```
<a name="Zc26y"></a>
### 6.4 LOG
LOG 可以用于记录匹配报文的记录，为了避免过多，匹配的规则应当越精准越好。
```bash
iptables -I INPUT -p tcp --dport 22 -J LOG
```
日志默认会记录在 /var/log/message 中，当然，也可以将相关信息记录在指定的文件中，以防止iptables的相关信息与其他日志信息相混淆，修改/etc/rsyslog.conf文件（或者/etc/syslog.conf），在rsyslog配置文件中添加如下配置即可。
```bash
# vim /etc/rsyslog.conf

kern.warning /var/log/iptables.log
```
加入上述配置后，报文的相关信息将会被记录到/var/log/iptables.log文件中。<br />完成上述配置后，重启rsyslog服务（或者syslogd），服务重启后，配置即可生效，匹配到的报文的相关信息将被记录到指定的文件中。
```bash
service rsyslog restart
```
LOG动作也有自己的选项，常用选项如下（先列出概念，后面有示例）

- `-log-level`选项可以指定记录日志的日志级别，可用级别有emerg，alert，crit，error，warning，notice，info，debug。
- `-log-prefix`选项可以给记录到的相关信息添加”标签”之类的信息，以便区分各种记录到的报文信息，方便在分析时进行过滤。

注：`-log-prefix`对应的值不能超过29个字符。
<a name="FBhzo"></a>
### 6.5 SNAT
注意事项：要实现两个不同段网络的互通，理论上只要开启机器上的路由转发功能即可实现（前提路由规则要规划好，详情见https://www.zsythink.net/archives/1663）。之所以常用 SNAT 和 DNAT，都是为了实现隐藏 IP 的目的。<br />另：所有涉及路由转发的规则，都要事先执行命令：`echo 1 > /proc/sys/net/ipv4/ip_forward`，开启路由转发<br />假设网络内部有10台主机，它们有各自的IP地址，当网络内部的主机与其他网络中的主机通讯时，则会暴露自己的IP地址，如果想要隐藏这些主机的IP地址，该怎么办呢？<br />可以这么办：当网络内部的主机向外部发送报文时，当报文经过防火墙或路由器时，将报文的源IP修改为防火墙或者路由器的IP地址，显示的源IP地址则是路由器或者防火墙的，而不是那10台主机的IP地址。<br />通过这种方式，可以实现多台内网机器，都通过同一台网关机器与公网通信。<br />那么此时就会有一个问题：回程的报文，是如何回到内网机器的呢？<br />原来，在防火墙或路由器里有维护着一张 NAT 表：

- 在出网时， SNAT 后会在 NAT 表中记录下内网源 IP 、PORT和路由器的 IP 、PORT对应关系，对于外网机器来说，只能看到路由器的 IP 、PORT。
- 在回程时，根据回程包的 Dest PORT 在 NAT 表中查找对应的内网 IP 和 PORT，修改报文的目的 IP 和 PORT 为内网机器的 IP 及 PORT

因此，一个 SNAT规则，虽然只写了 SNAT，但回程的 DNAT 其实是自动帮我们做了。
```bash
iptables -t nat -A POSTROUTING -s 10.1.0.0/16 -j SNAT --to-source 公网IP
```
<a name="qIMwQ"></a>
### 6.6 DNAT
在上面学习 SNAT 的过程中，其实也涉及了 DNAT ，只是由于前半段是使用的 SNAT ，因此前面称之为 SNAT。<br />同样的，若前半段使用的是 DNAT，而后半段使用 SNAT，则这个过程称之为 DNAT。<br />SNAT 和 DNAT 在应用场景上有什么区别：

- SNAT：可以让内网的机器访问公网
- DNAT：可以让公网的机器访问内网

DNAT 的规则如下
```bash
iptables -t nat -I PREROUTING -d 公网IP -p tcp --dport 公网端口 -j DNAT --to-destination 私网IP:端口号
```
当做 SNAT 时， 回程的DNAT会自动处理，但做 DNAT 时，回程的 SNAT 却需要自己处理
```bash
iptables -t nat -A POSTROUTING -s 10.1.0.0/16 -j SNAT --to-source 公网IP
```
<a name="MkumL"></a>
### 6.7 REDIRECT
以上的 DNAT 是本机转他机，ip 发生变化，port 也发生变化（也可以不变化，但要指定）<br />其实还有另外一种情况，单纯的本机的上端口转发，使用的是 REDIRECT
```bash
iptables -t nat -A PREROUTING -p tcp --dport 80 -j REDIRECT --to-ports 8080
```
配置完成上述规则后，其他机器访问本机的80端口时，会被映射到8080端口。
<a name="dYKkw"></a>
### 6.8 MASQUERADE
前面在配置 SNAT 时，都需要明确指定本地的公网 IP 是多少？<br />在大批量的机器部署过程中，会造成一些不方便，如果规则里可以自动获取网卡 IP，那就省事多了。<br />MASQUERADE 就是用来做这事情的，只要指定 -o 网卡，MASQUERADE 就会自动获取网卡上的 IP
```bash
iptables -t nat -A POSTROUTING -s 10.1.0.0/16 -o eth0 -j MASQUERADE
```
<a name="G7UbF"></a>
## 7、自定义链
当一条链中的规则数量非常多时，有针对 httpd ，也有针对 sshd 的，这样维护 iptables 规则就非常痛苦。<br />而自定义链，就是用 iptables 链的方式，来实现 “规则” 分组的功能。
<a name="L7dIu"></a>
### 7.1 新键自定义链
使用如下命令来新建自定义链
```bash
iptables -t filter -N IN_WEB
```
而此时还没有其他链引用 IN_WEB ，因此会显示 0 references
```
Chain IN_WEB (0 references)
```
<a name="UqqrK"></a>
### 7.2 创建规则
在自定义链中创建规则与内置链中一样
```bash
iptables -t filter -I IN_WEB -s 192.168.1.139 -j REJECT
```
此时，虽然创建了规则，但由于 IN_WEB 还没有人引用，虽然该条规则是不会匹配到规则的
<a name="tVxjb"></a>
### 7.3 引用自定义链
引用自定义链，就是把该链当成和 ACCEPT、REJECT 一样的动作就行。<br />意思是：匹配上的报文全部发往这些链中。
```bash
iptables -I input -p tcp --dport 80 -j IN_WEB
```
<a name="ZHgfb"></a>
### 7.4 修改自定义链
若想把 IN_WEB 的链名改成 WEB ，可以使用 `-E` 参数
```bash
iptables -E IN_WEB WEB
```
<a name="yIP3y"></a>
### 7.5 删除自定义链
若想删除自定义链，可以使用 `-X` 命令
```bash
iptables -X WEB
```
但要注意，若该链还在被引用者，则不能删除，需要事先清除引用，即删除引用该链的规则
```bash
iptables -D INPUT 1
```
<a name="srQKs"></a>
## 8、总结一下
iptables 是 Linux防火墙系统的重要组成部分，能够实现对网络数据包进出设备及转发的控制，应用在服务器、路由器等网络设备上，可以保障网络数据的安全与可靠性。<br />对于每个日常需要接触到 Linux 的开发人或运维来说，系统学习iptables是有必要的：<br />1、实现网络安全：iptables提供了一系列的过滤规则，可以限制外部主机对本地主机的访问，以及控制本地主机对外部主机的访问。通过配置iptables规则，可以过滤恶意流量，避免黑客攻击、病毒入侵等安全问题。<br />2、提高网络性能：通过使用iptables的QoS(Quality of Service)功能，可以对流量进行优化，使得关键业务的网络带宽得到更好的保障，从而提高网络性能和响应速度。<br />3、简化网络管理：通过iptables可以设置简单明了的规则，来实现对数据包的过滤和转发，从而达到灵活控制的效果。可以方便管理，减少管理员的工作负担。<br />4、符合行业标准：iptables是目前Linux平台网络安全的主流工具，学习iptables有助于符合相关行业标准和规范，例如PCI DSS（Payment Card Industry Data Security Standard）等。
