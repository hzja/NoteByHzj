<a name="EGoQt"></a>
## 表的概念
我们再想想另外一个问题，我们对每个"链"上都放置了一串规则，但是这些规则有些很相似，比如，A类规则都是对IP或者端口的过滤，B类规则是修改报文，那么这个时候，我们是不是能把实现相同功能的规则放在一起呢，必须能的。<br /> <br />我们把具有相同功能的规则的集合叫做"表"，所以说，不同功能的规则，我们可以放置在不同的表中进行管理，而iptables已经为我们定义了4种表，每种表对应了不同的功能，而我们定义的规则也都逃脱不了这4种功能的范围，所以，学习iptables之前，我们必须先搞明白每种表 的作用。<br />iptables为我们提供了如下规则的分类，或者说，iptables为我们提供了如下"表"<br /> <br />filter表：负责过滤功能，防火墙；内核模块：iptables_filter<br />nat表：network address translation，网络地址转换功能；内核模块：iptable_nat<br />mangle表：拆解报文，做出修改，并重新封装 的功能；iptable_mangle<br />raw表：关闭nat表上启用的连接追踪机制；iptable_raw<br />也就是说，我们自定义的所有规则，都是这四种分类中的规则，或者说，所有规则都存在于这4张"表"中。
<a name="anQq6"></a>
## 表链关系
但是我们需要注意的是，某些"链"中注定不会包含"某类规则"，就像某些"关卡"天生就不具备某些功能一样，比如，A"关卡"只负责打击陆地敌人，没有防空能力，B"关卡"只负责打击空中敌人，没有防御步兵的能力，C"关卡"可能比较NB，既能防空，也能防御陆地敌人，D"关卡"最屌，海陆空都能防。<br />那让我们来看看，每个"关卡"都有哪些能力，或者说，让我们看看每个"链"上的规则都存在于哪些"表"中。<br />我们还是以图为例，先看看prerouting"链"上的规则都存在于哪些表中。<br />注意：下图只用于说明prerouting链上的规则存在于哪些表中，并没有描述表的顺序。<br /> <br /> <br />![](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599894632798-aa877b8c-9e07-4936-846d-3c4ed28b6299.png#align=left&display=inline&height=577&originHeight=577&originWidth=237&size=0&status=done&style=none&width=237)<br />这幅图是什么意思呢？它的意思是说，prerouting"链"只拥有nat表、raw表和mangle表所对应的功能，所以，prerouting中的规则只能存放于nat表、raw表和mangle表中。<br /> <br />那么，根据上述思路，我们来总结一下，每个"关卡"都拥有什么功能，<br />或者说，每个"链"中的规则都存在于哪些"表"中。<br />PREROUTING      的规则可以存在于：raw表，mangle表，nat表。<br />INPUT          的规则可以存在于：mangle表，filter表，（centos7中还有nat表，centos6中没有）。<br />FORWARD         的规则可以存在于：mangle表，filter表。<br />OUTPUT         的规则可以存在于：raw表mangle表，nat表，filter表。<br />POSTROUTING      的规则可以存在于：mangle表，nat表。<br />但是，我们在实际的使用过程中，往往是通过"表"作为操作入口，对规则进行定义的，之所以按照上述过程介绍iptables，是因为从"关卡"的角度更容易从入门的角度理解，但是为了以便在实际使用的时候，更加顺畅的理解它们，此处我们还要将各"表"与"链"的关系罗列出来，

表（功能）<-->   链（钩子）：

raw     表中的规则可以被哪些链使用：PREROUTING，OUTPUT

mangle  表中的规则可以被哪些链使用：PREROUTING，INPUT，FORWARD，OUTPUT，POSTROUTING

nat     表中的规则可以被哪些链使用：PREROUTING，OUTPUT，POSTROUTING（centos7中还有INPUT，centos6中没有）

filter  表中的规则可以被哪些链使用：INPUT，FORWARD，OUTPUT

其实我们还需要注意一点，因为数据包经过一个"链"的时候，会将当前链的所有规则都匹配一遍，但是匹配时总归要有顺序，我们应该一条一条的去匹配，而且我们说过，相同功能类型的规则会汇聚在一张"表"中，那么，哪些"表"中的规则会放在"链"的最前面执行呢，这时候就需要有一个优先级的问题，我们还拿prerouting"链"做图示。

iptables详解（1）：iptables概念<br />prerouting链中的规则存放于三张表中，而这三张表中的规则执行的优先级如下：

raw --> mangle --> nat

但是我们知道，iptables为我们定义了4张"表",当他们处于同一条"链"时，执行的优先级如下。

优先级次序（由高而低）：

raw --> mangle --> nat --> filter

但是我们前面说过，某些链天生就不能使用某些表中的规则，所以，4张表中的规则处于同一条链的目前只有output链，它就是传说中海陆空都能防守的关卡。

为了更方便的管理，我们还可以在某个表里面创建自定义链，将针对某个应用程序所设置的规则放置在这个自定义链中，但是自定义链接不能直接使用，只能被某个默认的链当做动作去调用才能起作用，我们可以这样想象，自定义链就是一段比较"短"的链子，这条"短"链子上的规则都是针对某个应用程序制定的，但是这条短的链子并不能直接使用，而是需要"焊接"在iptables默认定义链子上，才能被IPtables使用，这就是为什么默认定义的"链"需要把"自定义链"当做"动作"去引用的原因。这是后话，后面再聊，在实际使用时我们即可更加的明白。

数据经过防火墙的流程<br />结合上述所有的描述，我们可以将数据包通过防火墙的流程总结为下图：

iptables详解（1）：iptables概念

我们在写Iptables规则的时候，要时刻牢记这张路由次序图，灵活配置规则。

我们将经常用到的对应关系重新写在此处，方便对应图例查看。

链的规则存放于哪些表中（从链到表的对应关系）：

PREROUTING   的规则可以存在于：raw表，mangle表，nat表。

INPUT        的规则可以存在于：mangle表，filter表，（centos7中还有nat表，centos6中没有）。

FORWARD      的规则可以存在于：mangle表，filter表。

OUTPUT       的规则可以存在于：raw表mangle表，nat表，filter表。

POSTROUTING  的规则可以存在于：mangle表，nat表。

表中的规则可以被哪些链使用（从表到链的对应关系）：

raw     表中的规则可以被哪些链使用：PREROUTING，OUTPUT

mangle  表中的规则可以被哪些链使用：PREROUTING，INPUT，FORWARD，OUTPUT，POSTROUTING

nat     表中的规则可以被哪些链使用：PREROUTING，OUTPUT，POSTROUTING（centos7中还有INPUT，centos6中没有）

filter  表中的规则可以被哪些链使用：INPUT，FORWARD，OUTPUT

下图中nat表在centos7中的情况就不再标明。

iptables详解（1）：iptables概念

规则的概念<br />说了一圈又说回来了，在上述描述中我们一直在提规则，可是没有细说，现在说说它。

先说说规则的概念，然后再通俗的解释它。

规则：根据指定的匹配条件来尝试匹配每个流经此处的报文，一旦匹配成功，则由规则后面指定的处理动作进行处理；

那么我们来通俗的解释一下什么是iptables的规则，之前打过一个比方，每条"链"都是一个"关卡"，每个通过这个"关卡"的报文都要匹配这个关卡上的规则，如果匹配，则对报文进行对应的处理，比如说，你我二人此刻就好像两个"报文"，你我二人此刻都要入关，可是城主有命，只有器宇轩昂的人才能入关，不符合此条件的人不能入关，于是守关将士按照城主制定的"规则"，开始打量你我二人，最终，你顺利入关了，而我已被拒之门外，因为你符合"器宇轩昂"的标准，所以把你"放行"了，而我不符合标准，所以没有被放行，其实，"器宇轩昂"就是一种"匹配条件"，"放行"就是一种"动作"，"匹配条件"与"动作"组成了规则。

了解了规则的概念，那我们来聊聊规则的组成部分,此处只是大概的将规则的结构列出，后面的文章中会单独对规则进行总结。

规则由匹配条件和处理动作组成。

匹配条件<br />匹配条件分为基本匹配条件与扩展匹配条件

基本匹配条件：

源地址Source IP，目标地址 Destination IP

上述内容都可以作为基本匹配条件。

扩展匹配条件：

除了上述的条件可以用于匹配，还有很多其他的条件可以用于匹配，这些条件泛称为扩展条件，这些扩展条件其实也是netfilter中的一部分，只是以模块的形式存在，如果想要使用这些条件，则需要依赖对应的扩展模块。

源端口Source Port, 目标端口Destination Port

上述内容都可以作为扩展匹配条件

处理动作<br />处理动作在iptables中被称为target（这样说并不准确，我们暂且这样称呼），动作也可以分为基本动作和扩展动作。

此处列出一些常用的动作，之后的文章会对它们进行详细的示例与总结：

ACCEPT：允许数据包通过。

DROP：直接丢弃数据包，不给任何回应信息，这时候客户端会感觉自己的请求泥牛入海了，过了超时时间才会有反应。

REJECT：拒绝数据包通过，必要时会给数据发送端一个响应的信息，客户端刚请求就会收到拒绝的信息。

SNAT：源地址转换，解决内网用户用同一个公网地址上网的问题。

MASQUERADE：是SNAT的一种特殊形式，适用于动态的、临时会变的ip上。

DNAT：目标地址转换。

REDIRECT：在本机做端口映射。

LOG：在/var/log/messages文件中记录日志信息，然后将数据包传递给下一条规则，也就是说除了记录以外不对数据包做任何其他操作，仍然让下一条规则去匹配。
<a name="29mEP"></a>
## ![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599894931340-58a94060-5a54-4499-9ece-401e4b49a88a.png#align=left&display=inline&height=379&originHeight=758&originWidth=1353&size=601467&status=done&style=none&width=676.5)
<a name="lHpBa"></a>
## ![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599895510858-63cf0a25-cd6f-4d17-931c-7cd61b34c5e3.png#align=left&display=inline&height=136&originHeight=272&originWidth=864&size=69926&status=done&style=none&width=432)![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599895610649-1d061263-a30e-407b-8854-66d413a5cd89.png#align=left&display=inline&height=335&originHeight=670&originWidth=1509&size=585007&status=done&style=none&width=754.5)
<a name="Gd6mN"></a>
## 语法构成

![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599894177693-5c4b3b10-97b8-4f20-b1b1-d641b02133dc.png#align=left&display=inline&height=333&originHeight=666&originWidth=1378&size=541412&status=done&style=none&width=689)

<a name="0Bbe8"></a>
## 控制类型
![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599896153382-f9e0ff65-d0f2-4c90-bcf9-c6a085fc3d3d.png#align=left&display=inline&height=308&originHeight=615&originWidth=1147&size=226257&status=done&style=none&width=573.5)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599896188734-c42b73d1-45e7-4012-9cb4-6dbb4b8845c6.png#align=left&display=inline&height=333&originHeight=666&originWidth=1091&size=157129&status=done&style=none&width=545.5)
<a name="RJplK"></a>
## 常见示范
![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599896344633-a269497c-6f76-4ddd-84dd-f38a0548cab3.png#align=left&display=inline&height=400&originHeight=800&originWidth=1133&size=235736&status=done&style=none&width=566.5)
<a name="udjVg"></a>
## 删除规则
![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599896431684-b5bcf77d-a7f4-4f04-8201-452cd36f4c32.png#align=left&display=inline&height=400&originHeight=800&originWidth=1137&size=211765&status=done&style=none&width=568.5)
<a name="Dekki"></a>
## 实战演示
<a name="7x7CW"></a>
### 查看规则
```c
[root@client ~]# iptables -L
Chain INPUT (policy ACCEPT)
target     prot opt source               destination

Chain FORWARD (policy ACCEPT)
target     prot opt source               destination

Chain OUTPUT (policy ACCEPT)
target     prot opt source               destination
[root@client ~]# iptables -t filter -L
Chain INPUT (policy ACCEPT)
target     prot opt source               destination

Chain FORWARD (policy ACCEPT)
target     prot opt source               destination

Chain OUTPUT (policy ACCEPT)
target     prot opt source               destination
[root@client ~]#

```
<a name="w0BmN"></a>
### 以序号的方式查看规则
```c
[root@client ~]# iptables -t filter -L --line-numbers
Chain INPUT (policy ACCEPT)
num  target     prot opt source               destination
1    ACCEPT     icmp --  anywhere             anywhere
2    ACCEPT     udp  --  anywhere             anywhere
3    ACCEPT     udp  --  anywhere             anywhere
4    ACCEPT     tcp  --  anywhere             anywhere

Chain FORWARD (policy ACCEPT)
num  target     prot opt source               destination

Chain OUTPUT (policy ACCEPT)
num  target     prot opt source               destination
```
<a name="jZhjD"></a>
### 添加一条记录只要是tcp的数据包都放行
```c
[root@client ~]# iptables -t filter -A INPUT -p tcp -j ACCEPT
[root@client ~]# iptables -t filter -L
Chain INPUT (policy ACCEPT)
target     prot opt source               destination
ACCEPT     tcp  --  anywhere             anywhere

Chain FORWARD (policy ACCEPT)
target     prot opt source               destination

Chain OUTPUT (policy ACCEPT)
target     prot opt source               destination
[root@client ~]#
[root@client ~]# iptables -t filter -L -n
Chain INPUT (policy ACCEPT)
target     prot opt source               destination
ACCEPT     tcp  --  0.0.0.0/0            0.0.0.0/0

Chain FORWARD (policy ACCEPT)
target     prot opt source               destination

Chain OUTPUT (policy ACCEPT)
target     prot opt source               destination
[root@client ~]#
[root@client ~]# iptables -t filter -L -n -v
Chain INPUT (policy ACCEPT 0 packets, 0 bytes)
 pkts bytes target     prot opt in     out     source               destination   
   32  2064 ACCEPT     tcp  --  *      *       0.0.0.0/0            0.0.0.0/0     

Chain FORWARD (policy ACCEPT 0 packets, 0 bytes)
 pkts bytes target     prot opt in     out     source               destination   

Chain OUTPUT (policy ACCEPT 24 packets, 2080 bytes)
 pkts bytes target     prot opt in     out     source               destination   
[root@client ~]#
```
**备注：**在生产使用中将数据包最多的规则放在第一条这样可以减少防火墙的压力。

<a name="XjokP"></a>
### 添加一条记录到第一条规则
```c
[root@client ~]# iptables -I INPUT -p udp -j ACCEPT
[root@client ~]# iptables -t filter -L -n
Chain INPUT (policy ACCEPT)
target     prot opt source               destination
ACCEPT     udp  --  0.0.0.0/0            0.0.0.0/0
ACCEPT     tcp  --  0.0.0.0/0            0.0.0.0/0

Chain FORWARD (policy ACCEPT)
target     prot opt source               destination

Chain OUTPUT (policy ACCEPT)
target     prot opt source               destination
```
<a name="6CJpu"></a>
### 插入到指定的规则 -I 
```c
[root@client ~]# iptables -I INPUT 2 -p udp -j ACCEPT
[root@client ~]# iptables -t filter -L -n
Chain INPUT (policy ACCEPT)
target     prot opt source               destination
ACCEPT     icmp --  0.0.0.0/0            0.0.0.0/0
ACCEPT     udp  --  0.0.0.0/0            0.0.0.0/0
ACCEPT     udp  --  0.0.0.0/0            0.0.0.0/0
ACCEPT     tcp  --  0.0.0.0/0            0.0.0.0/0

Chain FORWARD (policy ACCEPT)
target     prot opt source               destination

Chain OUTPUT (policy ACCEPT)
target     prot opt source               destination
[root@client ~]#
```
<a name="HgsHo"></a>
### 删除一条规则
```c
[root@client ~]# iptables -t filter -D INPUT 4
[root@client ~]# iptables -t filter -L --line-numbers
Chain INPUT (policy ACCEPT)
num  target     prot opt source               destination
1    ACCEPT     icmp --  anywhere             anywhere
2    ACCEPT     udp  --  anywhere             anywhere
3    ACCEPT     udp  --  anywhere             anywhere

Chain FORWARD (policy ACCEPT)
num  target     prot opt source               destination

Chain OUTPUT (policy ACCEPT)
num  target     prot opt source               destination
[root@client ~]#
```
<a name="B6O0v"></a>
### 删除所有的规则
```c
[root@client ~]# iptables -t filter -F
[root@client ~]# iptables -t filter -L --line-numbers
Chain INPUT (policy ACCEPT)
num  target     prot opt source               destination

Chain FORWARD (policy ACCEPT)
num  target     prot opt source               destination

Chain OUTPUT (policy ACCEPT)
num  target     prot opt source               destination
[root@client ~]#
```
<a name="e12942d8"></a>
### 配置Apache和ssh规则其他的全部拒绝
```c
[root@client ~]# iptables -t filter -A INPUT -p tcp --dport 22 -j ACCEPT
[root@client ~]# iptables -t filter -I INPUT -p tcp --dport 80 -j ACCEPT
[root@client ~]# iptables -P INPUT DROP
[root@client ~]# iptables -t filter -L
Chain INPUT (policy DROP)
target     prot opt source               destination
ACCEPT     tcp  --  anywhere             anywhere             tcp dpt:http
ACCEPT     tcp  --  anywhere             anywhere             tcp dpt:ssh

Chain FORWARD (policy ACCEPT)
target     prot opt source               destination

Chain OUTPUT (policy ACCEPT)
target     prot opt source               destination
```
<a name="db06c78d"></a>
### 测试
![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599897905014-4a96e830-fc3b-4446-8b0c-c4d7d85209c7.png#align=left&display=inline&height=261&originHeight=522&originWidth=772&size=222121&status=done&style=none&width=386)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599897917027-b31a3329-1475-49bd-8943-908dfc93aea3.png#align=left&display=inline&height=65&originHeight=129&originWidth=574&size=22453&status=done&style=none&width=287)<br />**解释：**因为我们只允许了Apache和ssh的协议进站其他的全部被拒绝
<a name="jcqsD"></a>
### 总结上面的语法格式

![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599898112187-c24ba0e0-be95-444e-9a5a-460a417fab43.png#align=left&display=inline&height=290&originHeight=579&originWidth=1055&size=230819&status=done&style=none&width=527.5)

<a name="x3Ltj"></a>
## 匹配类型
![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599898248869-c28f4643-bd52-438a-bebd-674f90d3eed5.png#align=left&display=inline&height=408&originHeight=815&originWidth=1181&size=238147&status=done&style=none&width=590.5)
<a name="MCNrU"></a>
## 通过匹配
![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599898381111-51395680-06e5-47cf-826c-b983604fde59.png#align=left&display=inline&height=366&originHeight=732&originWidth=1184&size=213499&status=done&style=none&width=592)

<a name="8BeKo"></a>
### drop与reject的区别演示
<a name="npBZ9"></a>
#### DROP
服务器端
```bash
[root@client ~]# iptables -A INPUT -p tcp --dport 80 -j DROP

```
客户端
```c
[root@zabbix-server ~]# curl 192.168.40.24
curl: (7) Failed connect to 192.168.40.24:80; Connection timed out
^C            #会一直处于等待服务器端回数据包我们看到状态就是卡住
```
<a name="nibk6"></a>
#### REJECT
服务端添加规则
```c
[root@client ~]# iptables -A INPUT -p tcp --dport 80 -j REJECT
[root@client ~]#
```
客户端测试
```c
[root@zabbix-server ~]# curl 192.168.40.24
curl: (7) Failed connect to 192.168.40.24:80; Connection refused

```
<a name="COY8F"></a>
## 隐含匹配
![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599899377049-e7266977-7e09-4ad4-8413-1325e8658b44.png#align=left&display=inline&height=396&originHeight=792&originWidth=1296&size=259843&status=done&style=none&width=648)
<a name="OLwUw"></a>
### 显示匹配
![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599901854962-867b25b7-0f5e-47d4-9ddd-68c541dde438.png#align=left&display=inline&height=296&originHeight=591&originWidth=1096&size=131101&status=done&style=none&width=548)

![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599901902651-98404351-ea62-4d90-9852-da2ff065b2f4.png#align=left&display=inline&height=455&originHeight=910&originWidth=1454&size=270337&status=done&style=none&width=727)

<a name="80VAi"></a>
### SNAT策略
![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599902207927-a3730b23-d060-430c-a470-4dbbfc9839a3.png#align=left&display=inline&height=333&originHeight=665&originWidth=1298&size=142084&status=done&style=none&width=649)


<a name="ThQr3"></a>
## 案例实战

<a name="CKJDO"></a>
### SNAT配置
| 名称 | ip地址 | 作用 |
| --- | --- | --- |
| server | 192.168.40.23(能上网) 172.24.8.11 | SNAT转发 |
| client | 172.24.8.12 | 上网 |


**客户机配置**
```c
[root@client ~]# grep -i "gateway" /etc/sysconfig/network-scripts/ifcfg-eno16777736
GATEWAY=172.24.8.11
[root@client ~]# systemctl restart network
[root@client ~]# ifconfig
eno16777736: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 172.24.8.12  netmask 255.255.255.0  broadcast 172.24.8.255
        inet6 fe80::250:56ff:fe3d:37eb  prefixlen 64  scopeid 0x20<link>
        ether 00:50:56:3d:37:eb  txqueuelen 1000  (Ethernet)
        RX packets 131045  bytes 146295005 (139.5 MiB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 64639  bytes 31918239 (30.4 MiB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

lo: flags=73<UP,LOOPBACK,RUNNING>  mtu 65536
        inet 127.0.0.1  netmask 255.0.0.0
        inet6 ::1  prefixlen 128  scopeid 0x10<host>
        loop  txqueuelen 0  (Local Loopback)
        RX packets 34830  bytes 2304388 (2.1 MiB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 34830  bytes 2304388 (2.1 MiB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

[root@client ~]#

```
**<br />**服务器配置**

基础配置
```c
[root@server ~]# ifconfig
eno16777736: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 192.168.40.23  netmask 255.255.255.0  broadcast 192.168.40.255
        inet6 fe80::20c:29ff:feca:cf76  prefixlen 64  scopeid 0x20<link>
        ether 00:0c:29:ca:cf:76  txqueuelen 1000  (Ethernet)
        RX packets 17733  bytes 1339975 (1.2 MiB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 17656  bytes 1355241 (1.2 MiB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

ens38: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 172.24.8.11  netmask 255.255.255.0  broadcast 172.24.8.255
        inet6 fe80::20c:29ff:feca:cf80  prefixlen 64  scopeid 0x20<link>
        ether 00:0c:29:ca:cf:80  txqueuelen 1000  (Ethernet)
        RX packets 182  bytes 15956 (15.5 KiB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 353  bytes 36896 (36.0 KiB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

lo: flags=73<UP,LOOPBACK,RUNNING>  mtu 65536
        inet 127.0.0.1  netmask 255.0.0.0
        inet6 ::1  prefixlen 128  scopeid 0x10<host>
        loop  txqueuelen 0  (Local Loopback)
        RX packets 42513  bytes 2585644 (2.4 MiB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 42513  bytes 2585644 (2.4 MiB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

[root@server ~]# ping www.baidu.com
PING www.a.shifen.com (39.156.66.18) 56(84) bytes of data.
64 bytes from 39.156.66.18: icmp_seq=1 ttl=128 time=47.3 ms
64 bytes from 39.156.66.18: icmp_seq=2 ttl=128 time=52.9 ms
64 bytes from 39.156.66.18: icmp_seq=3 ttl=128 time=44.1 ms
64 bytes from 39.156.66.18: icmp_seq=4 ttl=128 time=44.0 ms
64 bytes from 39.156.66.18: icmp_seq=5 ttl=128 time=46.1 ms
64 bytes from 39.156.66.18: icmp_seq=6 ttl=128 time=44.8 ms
[root@server ~]# echo "1" > /proc/sys/net/ipv4/ip_forward
[root@server ~]# sysctl -p
[root@server ~]#
```

iptables配置
```c
[root@server ~]# iptables -t nat -A POSTROUTING -s 172.24.8.0/24 -o eno16777736 -j SNAT --to-source 192.168.40.23
[root@server ~]# iptables -t nat -L
Chain PREROUTING (policy ACCEPT)
target     prot opt source               destination

Chain INPUT (policy ACCEPT)
target     prot opt source               destination

Chain OUTPUT (policy ACCEPT)
target     prot opt source               destination

Chain POSTROUTING (policy ACCEPT)
target     prot opt source               destination
SNAT       all  --  172.24.8.0/24        anywhere             to:192.168.40.23
[root@server ~]#
```

客户端验证

```c
[root@client ~]# vim /etc/resolv.conf
[root@client ~]# ping -c 4 www.baidu.com
PING www.a.shifen.com (39.156.66.14) 56(84) bytes of data.
64 bytes from 39.156.66.14: icmp_seq=1 ttl=127 time=48.3 ms
64 bytes from 39.156.66.14: icmp_seq=2 ttl=127 time=46.9 ms
64 bytes from 39.156.66.14: icmp_seq=3 ttl=127 time=46.3 ms
64 bytes from 39.156.66.14: icmp_seq=4 ttl=127 time=50.4 ms

--- www.a.shifen.com ping statistics ---
4 packets transmitted, 4 received, 0% packet loss, time 23799ms
rtt min/avg/max/mdev = 46.307/48.000/50.463/1.622 ms
[root@client ~]#

```
**备注：**ping: unknown host www.baidu.com要是出现这个问题就添加一个DNS服务器地址/etc/resolv.conf也可以写在和主机一起的配置文件当中，DNS地址最好是服务器的DNS地址。
<a name="zKcRu"></a>
#### 
server端使用地址伪装
```c
[root@server ~]# iptables -t nat -A POSTROUTING -s 172.24.8.0/24 -o eno16777736 -j MASQUERADE
[root@server ~]# iptables -t nat -L
Chain PREROUTING (policy ACCEPT)
target     prot opt source               destination

Chain INPUT (policy ACCEPT)
target     prot opt source               destination

Chain OUTPUT (policy ACCEPT)
target     prot opt source               destination

Chain POSTROUTING (policy ACCEPT)
target     prot opt source               destination
MASQUERADE  all  --  172.24.8.0/24        anywhere

```

客户端访问
```c
[root@client ~]# ping -c 4 www.baidu.com
PING www.a.shifen.com (39.156.66.14) 56(84) bytes of data.
64 bytes from 39.156.66.14: icmp_seq=1 ttl=127 time=52.8 ms
64 bytes from 39.156.66.14: icmp_seq=2 ttl=127 time=47.9 ms
64 bytes from 39.156.66.14: icmp_seq=3 ttl=127 time=57.1 ms
64 bytes from 39.156.66.14: icmp_seq=4 ttl=127 time=48.2 ms

--- www.a.shifen.com ping statistics ---
4 packets transmitted, 4 received, 0% packet loss, time 23725ms
rtt min/avg/max/mdev = 47.951/51.531/57.106/3.770 ms
[root@client ~]#

```
**备注：地址伪装技术更加接近我们实际的工作生活当中因为我们拨号上网有可能我们使用的公网IP就发生了变化。**


<a name="HVtfY"></a>
### DNAT配置
![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599906319175-dfc096a6-26be-4f76-9bba-b77856fc53ef.png#align=left&display=inline&height=268&originHeight=535&originWidth=1189&size=339142&status=done&style=none&width=594.5)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599906371742-fa9eed38-2eb2-41f1-a829-439e6c69f424.png#align=left&display=inline&height=295&originHeight=590&originWidth=1095&size=127100&status=done&style=none&width=547.5)

| 名称 | IP地址 | 角色 |
| --- | --- | --- |
| client | 172.24.8.12 | 客户端  |
| iptables | 192.168.40.24 172.24.8.11 | iptables |
| web | 192.168.40.25 | web服务器 |


客户端配置

```c
[root@client ~]# grep -i "gateway" /etc/sysconfig/network-scripts/ifcfg-eno16777736
GATEWAY=172.24.8.11
[root@client ~]# systemctl restart network
[root@client ~]# ifconfig
eno16777736: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 172.24.8.12  netmask 255.255.255.0  broadcast 172.24.8.255
        inet6 fe80::250:56ff:fe3d:37eb  prefixlen 64  scopeid 0x20<link>
        ether 00:50:56:3d:37:eb  txqueuelen 1000  (Ethernet)
        RX packets 131045  bytes 146295005 (139.5 MiB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 64639  bytes 31918239 (30.4 MiB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

lo: flags=73<UP,LOOPBACK,RUNNING>  mtu 65536
        inet 127.0.0.1  netmask 255.0.0.0
        inet6 ::1  prefixlen 128  scopeid 0x10<host>
        loop  txqueuelen 0  (Local Loopback)
        RX packets 34830  bytes 2304388 (2.1 MiB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 34830  bytes 2304388 (2.1 MiB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
[root@client ~]# route -n
Kernel IP routing table
Destination     Gateway         Genmask         Flags Metric Ref    Use Iface
0.0.0.0         172.24.8.11     0.0.0.0         UG    100    0        0 eno16777736
172.24.8.0      0.0.0.0         255.255.255.0   U     100    0        0 eno16777736
[root@client ~]#
```

服务器端配置

```c
[root@server ~]# ifconfig
eno16777736: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 192.168.40.23  netmask 255.255.255.0  broadcast 192.168.40.255
        inet6 fe80::20c:29ff:feca:cf76  prefixlen 64  scopeid 0x20<link>
        ether 00:0c:29:ca:cf:76  txqueuelen 1000  (Ethernet)
        RX packets 17733  bytes 1339975 (1.2 MiB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 17656  bytes 1355241 (1.2 MiB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

ens38: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 172.24.8.11  netmask 255.255.255.0  broadcast 172.24.8.255
        inet6 fe80::20c:29ff:feca:cf80  prefixlen 64  scopeid 0x20<link>
        ether 00:0c:29:ca:cf:80  txqueuelen 1000  (Ethernet)
        RX packets 182  bytes 15956 (15.5 KiB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 353  bytes 36896 (36.0 KiB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

lo: flags=73<UP,LOOPBACK,RUNNING>  mtu 65536
        inet 127.0.0.1  netmask 255.0.0.0
        inet6 ::1  prefixlen 128  scopeid 0x10<host>
        loop  txqueuelen 0  (Local Loopback)
        RX packets 42513  bytes 2585644 (2.4 MiB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 42513  bytes 2585644 (2.4 MiB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

[root@server ~]# ping www.baidu.com
PING www.a.shifen.com (39.156.66.18) 56(84) bytes of data.
64 bytes from 39.156.66.18: icmp_seq=1 ttl=128 time=47.3 ms
64 bytes from 39.156.66.18: icmp_seq=2 ttl=128 time=52.9 ms
64 bytes from 39.156.66.18: icmp_seq=3 ttl=128 time=44.1 ms
64 bytes from 39.156.66.18: icmp_seq=4 ttl=128 time=44.0 ms
64 bytes from 39.156.66.18: icmp_seq=5 ttl=128 time=46.1 ms
64 bytes from 39.156.66.18: icmp_seq=6 ttl=128 time=44.8 ms
[root@server ~]# echo "1" > /proc/sys/net/ipv4/ip_forward
[root@server ~]# sysctl -p
[root@server ~]#
```

DNAT配置信
```c
[root@iptables ~]# iptables -t nat -A PREROUTING -i ens38 -d 172.24.8.11 -p tcp  --dport 80 -j DNAT --to-destination 192.168.40.25
[root@iptables ~]# iptables -t nat -L
Chain PREROUTING (policy ACCEPT)
target     prot opt source               destination
DNAT       tcp  --  anywhere             172.24.8.11          tcp dpt:http to:192.168.40.25

Chain INPUT (policy ACCEPT)
target     prot opt source               destination

Chain OUTPUT (policy ACCEPT)
target     prot opt source               destination

Chain POSTROUTING (policy ACCEPT)
target     prot opt source               destination
[root@iptables ~]#
```

客户端访问
```c
[root@client ~]# curl 172.24.8.11
web-1
[root@client ~]#

```

