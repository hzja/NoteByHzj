Linux Bridge<br />Linux 中的 veth 是一对儿能互相连接、互相通信的虚拟网卡。通过使用它，可以让 Docker 容器和母机通信，或者是在两个 Docker 容器中进行交流。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645768961318-55a94a95-b410-489e-88d8-642f8a391223.png#clientId=uddc51e6c-91e7-4&from=paste&id=u8385df07&originHeight=111&originWidth=481&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5c0a1f64-d499-456e-8484-ecb24aab784&title=)<br />不过在实际中，会想在一台物理机上虚拟出来几个、甚至几十个容器，以求得充分压榨物理机的硬件资源。但这样带来的问题是大量的容器之间的网络互联。很明显上面简单的 veth 互联方案是没有办法直接工作的，该怎么办？？？<br />回头想一下，在物理机的网络环境中，多台不同的物理机之间是如何连接一起互相通信的呢？没错，那就是以太网交换机。同一网络内的多台物理机通过交换机连在一起，然后它们就可以相互通信了。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645768961319-99bd48a6-1d5e-4340-8d95-912d53375dfc.png#clientId=uddc51e6c-91e7-4&from=paste&id=u43b48c37&originHeight=162&originWidth=472&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u34220e95-5e1e-4f2b-9ffc-a5e29bf8192&title=)<br />在网络虚拟化环境里，和物理网络中的交换机一样，也需要这样的一个软件实现的设备。它需要有很多个虚拟端口，能把更多的虚拟网卡连接在一起，通过自己的转发功能让这些虚拟网卡之间可以通信。在 Linux 下这个软件实现交换机的技术就叫做 bridge（再强调下，这是纯软件实现的）。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645768961412-62f63b97-3bb5-4286-a2b5-4af2060bdca2.png#clientId=uddc51e6c-91e7-4&from=paste&id=ubf9f256c&originHeight=303&originWidth=504&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u34d21a77-a8dc-413c-926a-8f6951d878d&title=)<br />各个 Docker 容器都通过 veth 连接到 bridge 上，bridge 负责在不同的“端口”之间转发数据包。这样各个 Docker 之间就可以互相通信了！<br />来看看 bridge 的详细工作过程。
<a name="AYYbX"></a>
## 一、如何使用 bridge
在分析它的工作原理之前，很有必要先来看一看网桥是如何使用的。<br />为了方便大家理解，接下来通过动手实践的方式，在一台 Linux 上创建一个小型的虚拟网络出来，并让它们之间互相通信。
<a name="plk3a"></a>
### 1.1 创建两个不同的网络
Bridge 是用来连接两个不同的虚拟网络的，所以在准备实验 bridge 之前得先需要用 `net namespace` 构建出两个不同的网络空间来。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645768961320-64b4959b-35f4-48f6-8867-48053adbcbd9.png#clientId=uddc51e6c-91e7-4&from=paste&id=u85dd17d1&originHeight=203&originWidth=511&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u67531d99-935a-4d94-84d3-0936a9e6647&title=)<br />具体的创建过程如下。通过 `ip netns` 命令创建 `net namespace`。首先创建一个 net1：
```bash
# ip netns add net1
```
接下来创建一对儿 veth 出来，设备名分别是 veth1 和 veth1_p。并把其中的一头 veth1 放到这个新的 netns 中。
```bash
# ip link add veth1 type veth peer name veth1_p
# ip link set veth1 netns net1
```
因为打算是用这个 veth1 来通信，所以需要为其配置上 ip，并把它启动起来。
```bash
# ip netns exec net1 ip addr add 192.168.0.101/24 dev veth1
# ip netns exec net1 ip link set veth1 up
```
查看一下，上述的配置是否成功。
```bash
# ip netns exec net1 ip link list
# ip netns exec net1 ifconfig
```
重复上述步骤，在创建一个新的 netns出来，命名分别为。

- netns: net2
- veth pair: veth2, veth2_p
- ip: 192.168.0.102

这样就在一台 Linux 就创建出来了两个虚拟的网络环境。
<a name="PfuOc"></a>
### 1.2 把两个网络连接到一起
在上一个步骤中，只是创建出来了两个独立的网络环境而已。这个时候这两个环境之间还不能互相通信。需要创建一个虚拟交换机 - bridge， 来把这两个网络环境连起来。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645768961324-8fb5e812-1390-4386-9140-0ddfe321e9f6.png#clientId=uddc51e6c-91e7-4&from=paste&id=u306deec0&originHeight=293&originWidth=511&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3716f86d-1873-407a-951d-395893bae0f&title=)<br />创建过程如下。创建一个 bridge 设备, 把刚刚创建的两对儿 veth 中剩下的两头“插”到 bridge 上来。
```bash
# brctl addbr br0
# ip link set dev veth1_p master br0
# ip link set dev veth2_p master br0
# ip addr add 192.168.0.100/24 dev br0
```
再为 bridge 配置上 IP，并把 bridge 以及插在其上的 veth 启动起来。
```bash
# ip link set veth1_p up
# ip link set veth2_p up
# ip link set br0 up
```
查看一下当前 bridge 的状态，确认刚刚的操作是成功了的。
```bash
# brctl show
bridge name     bridge id               STP enabled     interfaces
br0             8000.4e931ecf02b1       no              veth1_p
                                                        veth2_p
```
<a name="RJENz"></a>
### 1.3 网络连通测试
在 net1 里（通过指定 `ip netns exec net1` 以及 `-I veth1`），ping 一下 net2 里的 IP（192.168.0.102）试试。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645768961714-72ec2410-5e26-48ff-88ec-fabc204891a5.png#clientId=uddc51e6c-91e7-4&from=paste&id=uf76a676d&originHeight=293&originWidth=511&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub8d6510a-a323-4b6a-aa34-0d5125f9b23&title=)
```bash
# ip netns exec net1 ping 192.168.0.102 -I veth1
PING 192.168.0.102 (192.168.0.102) from 192.168.0.101 veth1: 56(84) bytes of data.
64 bytes from 192.168.0.102: icmp_seq=1 ttl=64 time=0.037 ms
64 bytes from 192.168.0.102: icmp_seq=2 ttl=64 time=0.008 ms
64 bytes from 192.168.0.102: icmp_seq=3 ttl=64 time=0.005 ms
```
这样，就在一台 Linux 上虚拟出了 net1 和 net2 两个不同的网络环境。还可以按照这种方式创建更多的网络，都可以通过一个 bridge 连接到一起。这就是 Docker 中网络系统工作的基本原理。
<a name="gs7Fp"></a>
## 二、Bridge 是如何创建出来的
在内核中，bridge 是由两个相邻存储的内核对象来表示的。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645768961599-3a1d4fd6-b492-44a7-a2fd-bf8cb466dec3.png#clientId=uddc51e6c-91e7-4&from=paste&id=u50a7d293&originHeight=91&originWidth=501&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uedcc7a47-adec-4d50-b560-d50ef8d3634&title=)<br />先看下它是如何被创建出来的。内核中创建 bridge 的关键代码在 br_add_bridge 这个函数里。
```c
//file:net/bridge/br_if.c
int br_add_bridge(struct net *net, const char *name)
{
    //申请网桥设备，并用 br_dev_setup 来启动它
    dev = alloc_netdev(sizeof(struct net_bridge), name,
                       br_dev_setup);
    
    dev_net_set(dev, net);
    dev->rtnl_link_ops = &br_link_ops;
    
    //注册网桥设备
    res = register_netdev(dev);
    if (res)
        free_netdev(dev);
    return res;
}
```
上述代码中注册网桥的关键代码是 alloc_netdev 这一行。在这个函数里，将申请网桥的内核对象 net_device。在这个函数调用里要注意两点。

- 1.第一个参数传入了 struct net_bridge 的大小
- 2.第三个参数传入的 br_dev_setup 是一个函数。

带着这两点注意事项，进入到 alloc_netdev 的实现中。
```c
//file: include/linux/netdevice.h
#define alloc_netdev(sizeof_priv, name, setup) \
 alloc_netdev_mqs(sizeof_priv, name, setup, 1, 1)
```
好吧，竟然是个宏。那就得看 alloc_netdev_mqs 了。
```c
//file: net/core/dev.c
struct net_device *alloc_netdev_mqs(int sizeof_priv, ...，void (*setup)(struct net_device *))
{
 //申请网桥设备
 alloc_size = sizeof(struct net_device);
 if (sizeof_priv) {
  alloc_size = ALIGN(alloc_size, NETDEV_ALIGN);
  alloc_size += sizeof_priv;
 }

 p = kzalloc(alloc_size, GFP_KERNEL);
 dev = PTR_ALIGN(p, NETDEV_ALIGN);

 //网桥设备初始化
 dev->... = ...;
 setup(dev); //setup是一个函数指针，实际使用的是 br_dev_setup

 ...
}
```
在上述代码中。kzalloc 是用来在内核态申请内核内存的。需要注意的是，申请的内存大小是一个 struct net_device 再加上一个 struct net_bridge（第一个参数传进来的）。一次性就申请了两个内核对象，这说明**bridge 在内核中是由两个内核数据结构来表示的，分别是 struct net_device 和 struct net_bridge。**<br />申请完了一家紧接着调用 setup，这实际是外部传入的 br_dev_setup 函数。在这个函数内部进行进一步的初始化。
```c
//file: net/bridge/br_device.c
void br_dev_setup(struct net_device *dev)
{
 struct net_bridge *br = netdev_priv(dev);
 dev->... = ...;
 br->... = ...;
 ...
}
```
**总之，**`**brctl addbr br0**`** 命令主要就是完成了 bridge 内核对象（struct net_device 和 struct net_bridge）的申请以及初始化。**
<a name="GtIeV"></a>
## 三、添加设备
调用 `brctl addif br0 veth0` 给网桥添加设备的时候，会将 veth 设备以虚拟的方式连到网桥上。当添加了若干个 veth 以后，内核中对象的大概逻辑图如下。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645768961864-dd08ac5c-1c8f-4993-ad41-5b05aa438651.png#clientId=uddc51e6c-91e7-4&from=paste&id=ua050c2b0&originHeight=211&originWidth=573&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf8425c85-5050-4435-b80a-3a1fd2a9cc3&title=)<br />其中 veth 是由 struct net_device来表示，bridge 的虚拟插口是由 struct net_bridge_port 来表示。接下来看看源码，是如何达成上述的逻辑结果的。<br />添加设备会调用到 net/bridge/br_if.c 下面的 br_add_if。
```c
//file: net/bridge/br_if.c
int br_add_if(struct net_bridge *br, struct net_device *dev)
{
 // 申请一个 net_bridge_port
 struct net_bridge_port *p;
 p = new_nbp(br, dev);

 // 注册设备帧接收函数
 err = netdev_rx_handler_register(dev, br_handle_frame, p);

 // 添加到 bridge 的已用端口列表里
 list_add_rcu(&p->list, &br->port_list);

 ......
}
```
这个函数中的第二个参数 dev 传入的是要添加的设备。在本文中，就可以认为是 veth 的其中一头。比较关键的是 net_bridge_port 这个结构体，它模拟的是物理交换机上的一个插口。它起到一个连接的作用，把 veth 和 bridge 给连接了起来。见 new_nbp 源码如下：
```c
//file: net/bridge/br_if.c
static struct net_bridge_port *new_nbp(struct net_bridge *br,
           struct net_device *dev)
{
 //申请插口对象
 struct net_bridge_port *p;
 p = kzalloc(sizeof(*p), GFP_KERNEL);

 //初始化插口
 index = find_portno(br);
 p->br = br;
 p->dev = dev;
 p->port_no = index;
 ...
}
```
在 new_nbp 中，先是申请了代表插口的内核对象。find_portno 是在当前 bridge 下寻找一个可用的端口号。接下来插口对象通过 p->br = br 和 bridge 设备关联了起来，通过 p->dev = dev 和代表 veth 设备的 dev 对象也建立了联系。<br />在 br_add_if 中还调用 netdev_rx_handler_register 注册了设备帧接收函数，设置 veth 上的 rx_handler 为 br_handle_frame。**后面在接收包的时候会回调到它**。
```c
//file:
int netdev_rx_handler_register(struct net_device *dev,
          rx_handler_func_t *rx_handler,
          void *rx_handler_data)
{
 ... 
 rcu_assign_pointer(dev->rx_handler_data, rx_handler_data);
 rcu_assign_pointer(dev->rx_handler, rx_handler);
}
```
<a name="r1OTj"></a>
## 四、数据包处理过程
在Linux网络包接收过程中数据包会被网卡先从到 RingBuffer 中，然后依次经过硬中断、软中断处理。在软中断中再依次把包送到设备层、协议栈，最后唤醒应用程序。<br />不过，拿 veth 设备来举例，如果它连接到了网桥上的话，在设备层的 __netif_receive_skb_core 函数中和上述过程有所不同。连在 bridge 上的 veth 在收到数据包的时候，不会进入协议栈，而是会进入网桥处理。网桥找到合适的转发口（另一个 veth），通过这个 veth 把数据转发出去。工作流程如下图。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645768961899-5d9e3838-6296-411b-aff7-4ef55b212261.png#clientId=uddc51e6c-91e7-4&from=paste&id=u091ebc4a&originHeight=231&originWidth=591&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u390bddfb-c466-4a9f-8285-5ab9c33058f&title=)<br />从 veth1_p 设备的接收看起，所有的设备的接收都一样，都会进入 __netif_receive_skb_core 设备层的关键函数。
```c
//file: net/core/dev.c
static int __netif_receive_skb_core(struct sk_buff *skb, bool pfmemalloc)
{
 ...

 // tcpdump 抓包点
 list_for_each_entry_rcu(...);

 // 执行设备的 rx_handler（也就是 br_handle_frame）
 rx_handler = rcu_dereference(skb->dev->rx_handler);
 if (rx_handler) {
  switch (rx_handler(&skb)) { 
  case RX_HANDLER_CONSUMED:
   ret = NET_RX_SUCCESS;
   goto unlock;
  }
 }

 // 送往协议栈
 //...

unlock:
 rcu_read_unlock();
out:
 return ret;
}
```
在 __netif_receive_skb_core 中先是过了 tcpdump 的抓包点，然后查找和执行了 rx_handler。在上面小节中看到，把 veth 连接到网桥上的时候，veth 对应的内核对象 dev 中的 rx_handler 被设置成了 br_handle_frame。**所以连接到网桥上的 veth 在收到包的时候，会将帧送入到网桥处理函数 br_handle_frame 中**。<br />另外要注意的是网桥函数处理完的话，一般来说就 goto unlock 退出了。和普通的网卡数据包接收相比，并不会往下再送到协议栈了。<br />接着来看下网桥是咋工作的吧，进入到 br_handle_frame 中来搜寻。
```c
//file: net/bridge/br_input.c
rx_handler_result_t br_handle_frame(struct sk_buff **pskb)
{
 ...

forward:
 NF_HOOK(NFPROTO_BRIDGE, NF_BR_PRE_ROUTING, skb, skb->dev, NULL,
   br_handle_frame_finish);
}
```
上面对 br_handle_frame 的逻辑进行了充分的简化，简化后它的核心就是调用 br_handle_frame_finish。同样 br_handle_frame_finish 也有点小复杂。这里主要想了解的 Docker 场景下 bridge 上的 veth 设备转发。所以根据这个场景，又对该函数进行了充分的简化。
```c
//file: net/bridge/br_input.c
int br_handle_frame_finish(struct sk_buff *skb)
{  
 // 获取 veth 所连接的网桥端口、以及网桥设备
 struct net_bridge_port *p = br_port_get_rcu(skb->dev);
 br = p->br;

 // 更新和查找转发表
 struct net_bridge_fdb_entry *dst;
 br_fdb_update(br, p, eth_hdr(skb)->h_source, vid);
 dst = __br_fdb_get(br, dest, vid)

 // 转发
 if (dst) {
  br_forward(dst->dst, skb, skb2);
 } 
}
```
在硬件中，交换机和集线器的主要区别就是它会智能地把数据送到正确的端口上去，而不会像集线器那样给所有的端口都群发一遍。所以在上面的函数中，看到了更新和查找转发表的逻辑。这就是网桥在学习，它会根据它的自学习结果来工作。<br />在找到要送往的端口后，下一步就是调用 br_forward => __br_forward 进入真正的转发流程。
```c
//file: net/bridge/br_forward.c
static void __br_forward(const struct net_bridge_port *to, struct sk_buff *skb)
{
 // 将 skb 中的 dev 改成新的目的 dev
 skb->dev = to->dev;

 NF_HOOK(NFPROTO_BRIDGE, NF_BR_FORWARD, skb, indev, skb->dev,
  br_forward_finish);
}
```
在 __br_forward 中，将 skb 上的设备 dev 改为了新的目的 dev。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645768961723-6cc02a49-528f-4545-b338-c9b38a3698a3.png#clientId=uddc51e6c-91e7-4&from=paste&id=ua877aa35&originHeight=201&originWidth=490&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u20ee5d5f-ba24-4583-bc21-986a771a922&title=)<br />然后调用 br_forward_finish 进入发送流程。在 br_forward_finish 里会依次调用 br_dev_queue_push_xmit、dev_queue_xmit。
```c
//file: net/bridge/br_forward.c
int br_forward_finish(struct sk_buff *skb)
{
 return NF_HOOK(NFPROTO_BRIDGE, NF_BR_POST_ROUTING, skb, NULL, skb->dev,
         br_dev_queue_push_xmit);
}
int br_dev_queue_push_xmit(struct sk_buff *skb)
{
 dev_queue_xmit(skb);
 ...
}
```
dev_queue_xmit 就是发送函数，后续的发送过程就是 dev_queue_xmit => dev_hard_start_xmit => veth_xmit。在 veth_xmit 中会获取到当前 veth 的对端，然后把数据给它发送过去。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645768962019-becfea1a-6c5b-4a1f-9453-5d2afbf1a0ee.png#clientId=uddc51e6c-91e7-4&from=paste&id=u27ec2593&originHeight=201&originWidth=524&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub0f87aa7-4bef-47a8-9d61-859f6d62302&title=)<br />至此，bridge 上的转发流程就算是完毕了。要注意到的是，整个 bridge 的工作的源码都是在 net/core/dev.c 或 net/bridge 目录下。都是在设备层工作的。这也就充分印证了经常说的 bridge（物理交换机也一样） 是二层上的设备。<br />接下来，收到网桥发过来数据的 veth 会把数据包发送给它的对端 veth2，veth2再开始自己的数据包接收流程。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645768962223-daa8c614-eb0b-4975-953f-0584336fcc12.png#clientId=uddc51e6c-91e7-4&from=paste&id=ub230f2f3&originHeight=221&originWidth=530&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u187345be-5302-45c2-a2f3-c8a7f5646e7&title=)
<a name="lX3Am"></a>
## 五、总结
所谓网络虚拟化，其实用一句话来概括就是**用软件来模拟实现真实的物理网络连接**。<br />Linux 内核中的 bridge 模拟实现了物理网络中的交换机的角色。和物理网络类似，可以将虚拟设备插入到 bridge 上。不过和物理网络有点不一样的是，一对儿 veth 插入 bridge 的那端其实就不是设备了，可以理解为退化成了一个网线插头。<br />当 bridge 接入了多对儿 veth 以后，就可以通过自身实现的网络包转发的功能来让不同的 veth 之间互相通信了。<br />回到 Docker 的使用场景上来举例，完整的 Docker 1 和 Docker 2 通信的过程是这样的：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645768962105-4ac1d509-2bfb-4f3c-a71a-9e15fe2eae3e.png#clientId=uddc51e6c-91e7-4&from=paste&id=uf90a14c8&originHeight=311&originWidth=641&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ucc296dd1-46cc-4d78-a9ef-794bf81dac1&title=)<br />大致步骤是：

- 1.Docker1 往 veth1 上发送数据
- 2.由于 veth1_p 是 veth1 的 pair， 所以这个虚拟设备上可以收到包
- 3.veth 收到包以后发现自己是连在网桥上的，于是乎进入网桥处理。在网桥设备上寻找要转发到的端口，这时找到了 veth2_p 开始发送。网桥完成了自己的转发工作
- 4.veth2 作为 veth2_p 的对端，收到了数据包
- 5.Docker2 里的就可以从 veth2 设备上收到数据了

再继续拉大视野，从两个 Docker 的用户态来开始看一看。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645768962423-c28389f7-e237-4891-9174-04a15e90086a.png#clientId=uddc51e6c-91e7-4&from=paste&id=ucea30cf1&originHeight=479&originWidth=762&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1ffb7bde-51d2-4da2-aefe-b7c36fc8371&title=)<br />Docker 1 在需要发送数据的时候，先通过 send 系统调用发送，这个发送会执行到协议栈进行协议头的封装等处理。经由邻居子系统找到要使用的设备（veth1）后，从这个设备将数据发送出去，veth1 的对端 veth1_p 会收到数据包。<br />收到数据的 veth1_p 是一个连接在 bridge 上的设备，这时候 bridge 会接管该 veth 的数据接收过程。从自己连接的所有设备中查找目的设备。找到 veth2_p 以后，调用该设备的发送函数将数据发送出去。同样 veth2_p 的对端 veth2 即将收到数据。<br />其中 veth2 收到数据后，将和 lo、eth0 等设备一样，进入正常的数据接收处理过程。Docker 2 中的用户态进程将能够收到 Docker 1 发送过来的数据了就。
