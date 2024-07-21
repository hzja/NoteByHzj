Docker 网络<br />Docker的技术依赖于Linux内核的虚拟化技术的发展，Docker使用到的网络技术有Network Namespace、Veth设备对、Iptables/Netfilter、网桥、路由等。
<a name="r2MtN"></a>
### Network Namespace
为了支持网络协议栈的多个实例，Linux在网络栈引入了Network Namespace，这些独立的协议栈被隔离到不同的Namespace中，处于不同Namespace中的网络栈是完全隔离的，彼此无法通信。具体有关Linux Namespace的介绍。<br />Linux的网络协议栈十分复杂，为了支持独立的协议栈，相关的全局变量都必须修改为协议栈私有。Linux实现Network Namespace的核心就是让这些全局变量称为Network Namespace变量的成员，然后为协议栈的函数调用加入一个Namespace参数。与此同时，为了保证已开发程序及内核代码的兼容性，内核代码隐式地使用了Namespace空间内的变量。应用程序如果没有对Namespace有特殊需求，那么不需要额外的代码，Network Namespace对应用程序而言是透明的。<br />在建立了新的Network Namespace，并将某个进程关联到这个网络命名空间后，就出现了如下的命名空间下的内核数据结构，所有网络栈变量都放入了Network Namespace的数据结构中，这个Network Namespace是属于它进程组私有的，与其他进程组不冲突。<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1597225756355-95359d79-80c0-40bf-a1be-ae6d5c26ac85.png#align=left&display=inline&height=681&originHeight=681&originWidth=1080&size=0&status=done&style=shadow&width=1080)<br />Docker正是利用了Network Namespace特性，实现了不同容器之间的网络隔离。如果一个容器声明使用宿主机的网络栈（-net = host），即不开启Network Namespace，例如：
```bash
docker run –d –net=host --name c_name i_name
```
这种情况下，这个容器启动之后监听的是宿主机的80端口。像这样直接使用宿主机网络栈的方式，虽然可以为容器提供良好的网络性能，但也不可避免的造成端口冲突等网络资源冲突的问题。<br />所以在一般情况下，都希望程序引入Network Namespace里的网络栈，即这个容器拥有自己的IP和端口。但是，这个时候也会带来一个新的问题，被隔离的容器进程，是怎么与其它被隔离的进程进行网络通信的？
<a name="6zOds"></a>
### Net Bridge
Linux 可以支持不同的网络，他们之间是怎么支持够互相通信的呢？如果是两台主机，那需要的可能只是一根网线，把它们连接在一台交换机上。而在Linux当中，网桥（Bridge）就起到相应的作用。本质上来说，这是一个数据链路层（data link）的设备，根据Mac地址的信息转发到网桥的不同端口上。而Docker就是在宿主机上默认创建一个docker0的网桥，凡是连接docker0的网桥，都可以用它来通信。
<a name="4lYRs"></a>
#### 细述Bridge
网桥是一个二层的虚拟网络设备，把若干个网络接口“连接”起来，使得网口之间的报文可以转发。网桥能够解析收发的报文，读取目标的Mac地址信息，和自己的Mac地址表结合，来决策报文转发的目标网口。为了实现这些功能，网桥会学习源Mac地址。在转发报文时，网桥只需要向特定的端口转发，从而避免不必要的网络交互。如果它遇到了一个自己从未记录过的地址，就无法知道这个报文应该向哪个网口转发，就将报文广播给除了报文来源之外的所有网口。<br />在实际网络中，网络拓扑不可能永久不变。如果设备移动到另一个端口上，而它没有发送任何数据，那么网桥设备就无法感知到这个变化，结果网桥还是向原来的端口发数据包，在这种情况下数据就会丢失。所以网桥还要对学习到的Mac地址表加上超时时间，默认5min。如果网桥收到了对应端口MAC地址回发的包。则重置超时时间，否则过了超时时间后，就认为哪个设备不在那个端口上了，他就会广播重发。<br />Linux为了支持越来越多的网卡以及虚拟设备，所以使用网桥去提供这些设备之间转发数据的二层设备。Linux内核支持网口的桥接（以太网接口），这与单纯的交换机还是不太一样，交换机仅仅是一个二层设备，对于接受到的报文，要么转发，要么丢弃。运行着Linux内核的机器本身就是一台主机，有可能是网络报文的目的地，其收到的报文要么转发，要么丢弃，还可能被送到网络协议的网络层，从而被自己主机本身的协议栈消化，所以可以把网桥看作一个二层设备，也可以看做是一个三层设备。
<a name="ybgUB"></a>
#### Linux中Bridge实现
Linux内核是通过一个虚拟的网桥设备（Net Device）来实现桥接的。这个虚拟设备可以绑定若干个以太网接口，从而将它们连接起来。Net Device网桥和普通的设备不同，最明显的是它还可以有一个ip地址。<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1597225756337-9dee7a50-cd7d-46f1-8798-83b57c82a121.png#align=left&display=inline&height=817&originHeight=817&originWidth=1080&size=0&status=done&style=shadow&width=1080)<br />如上图所示，网桥设备br0绑定的eth0和eth1。对于网络协议栈的上层来说，只看到br0。因为桥接是在数据链路层实现的，上层不需要关心桥接的细节，于是协议栈上层需要发送的报文被送到br0，网桥设备的处理代码判断报文被转发到eth0还是eth1，或者两者皆转发。反之，从eth0或者从eth1接收到的报文被提交给网桥的处理代码，在这里判断报文应该被转发、丢弃或者提交到协议栈上层。<br />而有时eth0、eth1也可能会作为报文的源地址或目的地址，直接参与报文的发送和接收，从而绕过网桥。
<a name="Vldgt"></a>
#### Bridge常用操作
Docker自动完成了对网桥的创建和维护。如果想要进一步理解网桥，可以看下如下举的一些常用操作命令。<br />新增一个网桥：
```bash
brctl addbr xxxxx
```
在新增网桥的基础上增加网口，在linux中，一个网口其实就是一个物理网卡。将物理网卡和网桥连接起来：
```bash
brctl addif xxxx ethx
```
网桥的物理网卡作为一个网口，由于在链路层工作，就不再需要IP地址了，这样上面的IP地址自然失效：
```bash
ipconfig ethx 0.0.0.0
```
给网桥配置一个IP地址：
```bash
ipconfig brxxx xxx.xxx.xxx.xxx
```
这样网桥就是一个有了IP地址，而连接在这之上的网卡就是一个纯链路层设备了。
<a name="D9ShV"></a>
### Veth Pair
docker在宿主机上创建docker0网桥后，凡是连接到docker0上的网桥，就可以用它来通信。那么这里又有个问题，就是这些容器是如何连接到docker0网桥上的？所以这就是Veth Pair虚拟设备的作用了，Veth Pair就是为了在不同的Network Namespace之间进行通信，利用它，可以将两个Network Namespace连接起来。<br />Veth Pair设备的特点是：它被创建出来后，总是以两张虚拟网卡（Veth Peer）的形式出现。并且，其中一个网卡发出的数据包，可以直接出现在另一张“网卡”上，哪怕这两张网卡在不同的Network Namespace中。<br />正是因为这样的特点，Veth Pair成对出现，很像是一对以太网卡，常常被看做是不同Network Namespace直连的“网线”。在Veth一端发送数据时，他会将数据发送到另一端并触发另一端的接收操作。可以把Veth Pair其中一端看做另一端的一个Peer。<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1597225756282-c425a68a-4760-4103-ab07-9f8f23ad5929.png#align=left&display=inline&height=580&originHeight=580&originWidth=1038&size=0&status=done&style=shadow&width=1038)
<a name="jURrA"></a>
#### Veth Pair操作命令
创建Veth Pair：
```
ip link add veth0 type veth peer name veth1
```
创建后查看Veth Pair的信息：
```
ip link show
```
将其中一个Veth Peer设置到另一个Namespace：
```
ip link set veth1 netns netns1
```
在netns1中查看veth1设备：
```
ip netns exec netns1 ip link show
```
当然，在docker里面，除了将Veth放入容器，还改名为eth0。想要通信必须先分配IP地址：
```
ip netns exec netns1 ip addr add 10.1.1.1/24 dev veth1ip addr add 10.1.1.2/24 dev veth0
```
启动它们：
```
ip netns exec netns1 ip link set dev veth1 upip link set dev veth0 up
```
测试通信:
```
ip netns exec netns1 ping 10.1.1.2
```
<a name="A6iba"></a>
#### Veth Pair查看端对端
在实际操作Veth Pair时，可以使用ethtool便于操作。<br />在一个Namespace中查看Veth Pair接口在设备列表中的序列号：
```
ip netns exec netns1 ethtool -S veth1
```
如果得知另一端的接口设备序列号，假如序列号为6，则可以继续查看6代表了什么设备：
```
ip netns exec netns2 ip link | grep 6
```
<a name="nx0eq"></a>
### Iptables/Netfilter
Linux协议栈非常高效且复杂。如果想要在数据处理过程中对关心的数据进行一些操作，则需要Linux提供一套相应的机制帮助用户实现自定义的数据包处理过程。<br />在Linux网络协议栈有一组网络回调函数挂接点，通过这些挂接点函数挂接的钩子函数可以在Linux网络栈处理数据包的过程中对数据包一些操作，例如过滤、修改、丢弃等。整个挂接点技术叫做Iptables和Netfilter。<br />Netfilter负责在内核中执行各种各样的挂接规则，运行在内核模式中。而Iptables是在用户模式下运行的进程，负责协助维护内核中Netfilter的各种规则表。通过二者的配合来实现整个Linux网络协议栈中灵活的数据包处理机制。<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1597225756398-764933fd-e415-432d-b658-c0fe514404db.png#align=left&display=inline&height=1093&originHeight=1093&originWidth=1080&size=0&status=done&style=shadow&width=1080)
<a name="mf9y0"></a>
#### 规则表Table
这些挂载点能挂接的规则也分不同的类型，目前主要支持的Table类型如下：<br />•RAW<br />•MANGLE<br />•NAT<br />•FILTER<br />上述4个规则链的优先级是RAW最高，FILTER最低。<br />在实际应用中，不同挂接点需要的规则类型通常不同。例如，在Input的挂接点上明显不需要FILTER的过滤规则，因为根据目标地址，已经在本机的上层协议栈了，所以无需再挂载FILTER过滤规则。
<a name="TvsWm"></a>
### Route
Linux系统包含了一个完整的路由功能。当IP层在处理数据发送或者转发时，会使用路由表来决定发往哪里。通常情况下，如果主机与目的主机直接相连，那么主机可以直接发送IP报文到目的主机。<br />路由功能是由IP层维护的一张路由表来实现。当主机收到数据报文时，它用此表来决策接下来应该做什么操作。当从网络侧接收到数据报文时，IP层首先会检查报文的IP地址是否与主机自身的地址相同。如果数据报文中的IP地址是自身主机的地址，那么报文将被发送到传输层相应的协议栈中去。如果报文中的IP地址不是主机自身的地址，并且配置了路由功能，那么报文将被转发，否则报文将被丢弃。<br />路由表的数据一般以条目形式存在，一个典型的路由表条目通常包含以下主要的条目项：<br />•目的IP地址<br />•下一个路由器的IP地址<br />•标志<br />•网络接口规范<br />通过路由表转发时，如果任何条目的第一个字段完全匹配目标条目的IP地址（主机）或部分匹配（网络），那么它将指示下一个路由器的IP地址。这些信息将告诉主机数据包该转发到哪一个“下一个路由器”。而条目中所有其它字段将提供更多的辅助信息来为路由转发做决定。<br />如果没有一个完全匹配的IP，则继续搜索网络ID。找到则转发数据到指定路由器上。由此可知，网络上所有主机都是通过这个路由表中的单个条目进行管理。<br />如果上述两个条件都不匹配，则将数据报文转发到一个默认路由器上。<br />如果上述步骤失败，默认路由器也不存在，那么这个数据报文无法转发。任何无法投递的数据都会产生一个ICMP主机不可达或者ICMP网络不可达的错误，并将该错误返回给生成此数据的应用程序。
<a name="W0Wvz"></a>
#### Route Table
Linux路由表至少2个，一个是LOCAL，一个是MAIN。<br />Local表用于供Linux协议栈识别本地地址，以及进行本地各个不同网络之间的数据转发。MAIN表用于各类网络IP的转发。它的建立既可以使用静态配置生成，也可以使用动态路由发现协议生成。动态路由发现协议一般使用组播功能来通过发送路由发现数据，动态获取和交换网络的路由信息，并更新到路由表中。<br />通过下列命令查看LOCAL表的内容：
```
ip route show table local type local
```
路由表的查看：
```
ip route list
```
<a name="ErEnI"></a>
### 1、查看系统网络配置
```
[root@iZuligp6e1dyzfZ ~]# ifconfig
docker0 -------------------Linux的虚拟网桥
```
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1566893324950-b521de8c-f6c6-4a2c-8a27-b988864d50c3.png#align=left&display=inline&height=1201&originHeight=1321&originWidth=2436&size=761768&status=done&style=none&width=2214.5454065464755)
<a name="Gb6TV"></a>
### 2、下载网桥管理工具
```
[root@iZuligp6e1dyzfZ ~]# yum install bridge-utils
```
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1566893433816-f976dec2-a6e8-4a5e-bde7-01233f206ab8.png#align=left&display=inline&height=1215&originHeight=1336&originWidth=2491&size=644768&status=done&style=none&width=2264.545405462755)
<a name="lmBA2"></a>
### 3、查看网桥
```
[root@iZuligp6e1dyzfZ ~]# brctl show
```
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1566893489496-f579fc51-4c31-41c7-8b1b-5488c329a309.png#align=left&display=inline&height=420&originHeight=462&originWidth=1518&size=60850&status=done&style=none&width=1379.999970089306)
<a name="b225e85a"></a>
### 4、修改docker0的地址
```
$ sudo ifconfig docker0 192.168.200.1 netmask 255.255.255.0
```
<a name="1RIA3"></a>
### 5、添加虚拟网桥
```
$ sudo brctl addbr br0
$ sudo ifconfig br0  192.168.200.1 netmask 255.255.255.0
更改docker守护进程的启动配置:
/etc/default/docker 中添加DOCKER_OPS值
-b=br0

```
<a name="mQwD7"></a>
### 6、查看用户组执行的操作
```
[root@iZuligp6e1dyzfZ ~]# ps -ef | grep docker
```
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1566893664166-971bc8b8-daaf-48d4-944d-a76a855d3c14.png#align=left&display=inline&height=1172&originHeight=1289&originWidth=2409&size=1543674&status=done&style=none&width=2189.9999525330295)
