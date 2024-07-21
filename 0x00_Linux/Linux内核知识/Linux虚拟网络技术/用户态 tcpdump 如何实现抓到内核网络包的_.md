Linux<br />在网络包的发送和接收过程中，绝大部分的工作都是在内核态完成的。那么问题来了，常用的运行在用户态的程序 tcpdump 是那如何实现抓到内核态的包的呢？有的同学知道 tcpdump 是基于 libpcap 的，那么 libpcap 的工作原理又是啥样的呢。如果裸写一个抓包程序，有没有思路？

- tcpdump 是如何工作的？
- netfilter 过滤的包 tcpdump 是否可以抓的到？
- 自己写一个抓包程序的话该如何下手？

借助这几个问题，来展开探索之旅！
<a name="iXrrt"></a>
## 一、网络包接收过程
网络包从网卡到达用户进程中的这个过程可以简单用如下这个图来表示。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653834621596-5023fd48-5f9a-4ca4-b0bd-ab9d0bfce992.png#clientId=u4188de8d-9d39-4&from=paste&id=u6b9155db&originHeight=137&originWidth=541&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue40afa9f-53dd-4216-be2c-f3c78fd6a70&title=)
<a name="rYRbD"></a>
### 找到 tcpdump 抓包点
在网络设备层的代码里找到了 tcpdump 的抓包入口。在 __netif_receive_skb_core 这个函数里会遍历 ptype_all 上的协议。还记得上文中提到 tcpdump 在 ptype_all 上注册了虚拟协议。这时就能执行的到了。来看函数：
```c
//file: net/core/dev.c
static int __netif_receive_skb_core(struct sk_buff *skb, bool pfmemalloc)
{
    ......
    //遍历 ptype_all （tcpdump 在这里挂了虚拟协议）
    list_for_each_entry_rcu(ptype, &ptype_all, list) {
        if (!ptype->dev || ptype->dev == skb->dev) {
            if (pt_prev)
                ret = deliver_skb(skb, pt_prev, orig_dev);
            pt_prev = ptype;
        }
    }
}
```
在上面函数中遍历 ptype_all，并使用 deliver_skb 来调用协议中的回调函数。
```c
//file: net/core/dev.c 
static inline int deliver_skb(...)
{
 return pt_prev->func(skb, skb->dev, pt_prev, orig_dev);
}
```
对于 tcpdump 来说，就会进入 packet_rcv 了（后面再说为啥是进入这个函数）。这个函数在 net/packet/af_packet.c 文件中。
```c
//file: net/packet/af_packet.c
static int packet_rcv(struct sk_buff *skb, ...)
{
 __skb_queue_tail(&sk->sk_receive_queue, skb);
 ......
}
```
可见 packet_rcv 把收到的 skb 放到了当前 packet socket 的接收队列里了。这样后面调用 recvfrom 的时候就可以获取到所抓到的包！！
<a name="JpTMI"></a>
### 再找 netfilter 过滤点
为了解释开篇中提到的问题，这里再稍微到协议层中多看一些。在 ip_rcv 中找到了一个 netfilter 相关的执行逻辑。
```c
//file: net/ipv4/ip_input.c
int ip_rcv(...)
{
 ......
 return NF_HOOK(NFPROTO_IPV4, NF_INET_PRE_ROUTING, skb, dev, NULL,
         ip_rcv_finish);
}
```
如果用 NF_HOOK 作为关键词来搜索，还能搜到不少 netfilter 的过滤点。不过所有的过滤点都是位于 IP 协议层的。<br />在接收包的过程中，数据包是先经过网络设备层然后才到协议层的。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653834621516-f317a1f7-238a-4c66-8a71-fd203c3cc503.png#clientId=u4188de8d-9d39-4&from=paste&id=u65f5f105&originHeight=225&originWidth=531&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u93137774-a27c-48b5-9584-1ff5ac02076&title=)<br />那么开篇中的一个问题就有了答案了。假如设置了 netfilter 规则，在接收包的过程中，工作在网络设备层的 tcpdump 先开始工作。还没等 netfilter 过滤，tcpdump 就抓到包了！<br />**所以，在接收包的过程中，netfilter 过滤并不会影响 tcpdump 的抓包！**
<a name="AWSIf"></a>
## 二、网络包发送过程
网络包的发送过程可以汇总成简单的一张图。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653834621530-a3442572-7729-40af-86f3-63d8247b33ea.png#clientId=u4188de8d-9d39-4&from=paste&id=uc0617439&originHeight=137&originWidth=573&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=udd745174-5625-48f1-acfe-6acbbc9fac4&title=)
<a name="NqMVt"></a>
### 找到 netfilter 过滤点
在发送的过程中，同样是在 IP 层进入各种 netfilter 规则的过滤。
```c
//file: net/ipv4/ip_output.c  
int ip_local_out(struct sk_buff *skb)
{
	//执行 netfilter 过滤
	err = __ip_local_out(skb);
}

int __ip_local_out(struct sk_buff *skb)
{
	......
		return nf_hook(NFPROTO_IPV4, NF_INET_LOCAL_OUT, skb, NULL,
					   skb_dst(skb)->dev, dst_output);
}
```
在这个文件中，还能看到若干处 netfilter 过滤逻辑。
<a name="AoKpf"></a>
### 找到 tcpdump 抓包点
发送过程在协议层处理完毕到达网络设备层的时候，也有 tcpdump 的抓包点。
```c
//file: net/core/dev.c
int dev_hard_start_xmit(struct sk_buff *skb, struct net_device *dev,
						struct netdev_queue *txq)
{
	...
		if (!list_empty(&ptype_all))
			dev_queue_xmit_nit(skb, dev);
}

static void dev_queue_xmit_nit(struct sk_buff *skb, struct net_device *dev)
{
	list_for_each_entry_rcu(ptype, &ptype_all, list) {
		if ((ptype->dev == dev || !ptype->dev) &&
			(!skb_loop_sk(ptype, skb))) {
			if (pt_prev) {
				deliver_skb(skb2, pt_prev, skb->dev);
				pt_prev = ptype;
				continue;
			}
			......
			}
	} 
}
```
在上述代码中可以看到，在 dev_queue_xmit_nit 中遍历 ptype_all 中的协议，并依次调用 deliver_skb。这就会执行到 tcpdump 挂在上面的虚拟协议。<br />在网络包的发送过程中，和接收过程恰好相反，是协议层先处理、网络设备层后处理。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653834621509-d9da6065-d124-4ad7-b123-a4a4e7b5c8ac.png#clientId=u4188de8d-9d39-4&from=paste&id=u461ffaab&originHeight=226&originWidth=565&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u689459a5-a70a-43ed-a58a-3fa72fa4063&title=)<br />**如果 netfilter 设置了过滤规则，那么在协议层就直接过滤掉了。在下层网络设备层工作的 tcpdump 将无法再捕获到该网络包**。
<a name="Z9UMO"></a>
## 三、TCPDUMP 启动
前面两小节说到了内核收发包都通过遍历 ptype_all 来执行抓包的。那么现在来看看用户态的 tcpdump 是如何挂载协议到内 ptype_all 上的。<br />通过 strace 命令抓一下 tcpdump 命令的系统调用，显示结果中有一行 socket 系统调用。Tcpdump 秘密的源头就藏在这行对 socket 函数的调用里。
```c
# strace tcpdump -i eth0
socket(AF_PACKET, SOCK_RAW, 768)
......
```
socket 系统调用的第一个参数表示创建的 socket 所属的地址簇或者协议簇，取值以 AF 或者 PF 开头。在 Linux 里，支持很多种协议族，在 include/linux/socket.h 中可以找到所有的定义。这里创建的是 packet 类型的 socket。<br />协议族和地址族：每一种协议族都有其对应的地址族。比如 IPV4 的协议族定义叫 PF_INET，其地址族的定义是 AF_INET。它们是一一对应的，而且值也完全一样，所以经常混用。
```c
//file: include/linux/socket.h
#define AF_UNSPEC 0
#define AF_UNIX  1 /* Unix domain sockets   */
#define AF_LOCAL 1 /* POSIX name for AF_UNIX */
#define AF_INET  2 /* Internet IP Protocol  */
#define AF_INET6 10 /* IP version 6   */
#define AF_PACKET 17 /* Packet family  */
......
```
另外上面第三个参数 768 代表的是 ETH_P_ALL，socket.htons(ETH_P_ALL) = 768。<br />来展开看这个 packet 类型的 socket 创建的过程中都干了啥，找到 socket 创建源码。
```c
//file: net/socket.c
SYSCALL_DEFINE3(socket, int, family, int, type, int, protocol) 
{
	......
		retval = sock_create(family, type, protocol, &sock); 
}

int __sock_create(struct net *net, int family, int type, ...)
{
	......
		pf = rcu_dereference(net_families[family]);
	err = pf->create(net, sock, protocol, kern);
}
```
在 __sock_create 中，从 net_families 中获取了指定协议。并调用了它的 create 方法来完成创建。<br />net_families 是一个数组，除了常用的 PF_INET（ ipv4 ） 外，还支持很多种协议族。比如 PF_UNIX、PF_INET6（ipv6）、PF_PACKET等等。每一种协议族在 net_families 数组的特定位置都可以找到其 family 类型。在这个 family 类型里，成员函数 create 指向该协议族的对应创建函数。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653834621528-5ffe13e9-e2c6-4438-a5b8-5686b5d756fe.png#clientId=u4188de8d-9d39-4&from=paste&id=u4e054e76&originHeight=333&originWidth=804&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u67ad2070-8136-429a-b15e-6dac60c82e8&title=)<br />根据上图，可以看到对于 packet 类型的 socket，pf->create 实际调用到的是 packet_create 函数。进入到这个函数中来一探究竟，这是理解 tcpdump 工作原理的关键！
```c
//file: packet/af_packet.c
static int packet_create(struct net *net, struct socket *sock, int protocol,
						 int kern)
{
	...
		po = pkt_sk(sk);
	po->prot_hook.func = packet_rcv;

	//注册钩子
	if (proto) {
		po->prot_hook.type = proto;
		register_prot_hook(sk);
	}
}

static void register_prot_hook(struct sock *sk)
{
	struct packet_sock *po = pkt_sk(sk);
	dev_add_pack(&po->prot_hook);
}
```
在 packet_create 中设置回调函数为 packet_rcv，再通过 register_prot_hook => dev_add_pack 完成注册。注册完后，是在全局协议 ptype_all 链表中添加了一个虚拟的协议进来。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653834621906-a56656df-4c35-46bf-a5c0-84bd8b7ed341.png#clientId=u4188de8d-9d39-4&from=paste&id=u1c38dd25&originHeight=245&originWidth=610&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u39bcf3d7-3c08-451d-9232-5c1fcb54b10&title=)<br />再来看下 dev_add_pack 是如何注册协议到 ptype_all 中的。回顾开头看到的 socket 函数调用，第三个参数 proto 传入的是 ETH_P_ALL。那 dev_add_pack 其实最后是把 hook 函数添加到了 ptype_all 里了，代码如下。
```c
//file: net/core/dev.c
void dev_add_pack(struct packet_type *pt)
{
	struct list_head *head = ptype_head(pt);
	list_add_rcu(&pt->list, head);
}

static inline struct list_head *ptype_head(const struct packet_type *pt)
{
	if (pt->type == htons(ETH_P_ALL))
		return &ptype_all;
	else
		return &ptype_base[ntohs(pt->type) & PTYPE_HASH_MASK];
}
```
整篇文章都以 ETH_P_ALL 为例，但其实有的时候也会有其它情况。在别的情况下可能会注册协议到 ptype_base 里了，而不是 ptype_all。同样， ptype_base 中的协议也会在发送和接收的过程中被执行到。<br />**总结：tcpdump 启动的时候内部逻辑其实很简单，就是在 ptype_all 中注册了一个虚拟协议而已。**
<a name="GAnMn"></a>
## 四、总结
现在再回头看开篇提到的几个问题。
<a name="SFbJO"></a>
### 1. tcpdump是如何工作的
用户态 tcpdump 命令是通过 socket 系统调用，在内核源码中用到的 ptype_all 中挂载了函数钩子上去。无论是在网络包接收过程中，还是在发送过程中，都会在网络设备层遍历 ptype_all 中的协议，并执行其中的回调。tcpdump 命令就是基于这个底层原理来工作的。
<a name="oMFI9"></a>
### 2. netfilter 过滤的包 tcpdump是否可以抓的到
关于这个问题，得分接收和发送过程分别来看。在网络包接收的过程中，由于 tcpdump 近水楼台先得月，所以完全可以捕获到命中 netfilter 过滤规则的包。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653834621896-26e4e5bf-15f4-44fc-9c43-0051311ae0ed.png#clientId=u4188de8d-9d39-4&from=paste&id=u4db46208&originHeight=225&originWidth=531&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2992a627-609a-4beb-bbcc-af216130d4c&title=)<br />但是在发送的过程中，恰恰相反。网络包先经过协议层，这时候被 netfilter 过滤掉的话，底层工作的 tcpdump 还没等看见就啥也没了。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653834621925-943576fe-97fe-4bb0-884e-0b45e6124254.png#clientId=u4188de8d-9d39-4&from=paste&id=u0e569e62&originHeight=226&originWidth=565&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uad86544f-f06b-4a35-aff9-91a7dcffe29&title=)
<a name="TIuar"></a>
### 3. 自己写一个抓包程序的话该如何下手
如果想自己写一段类似 tcpdump 的抓包程序的话，使用 packet socket 就可以了。这里用 c 写了一段抓包，并且解析源 IP 和目的 IP 的简单 demo。<br />源码地址：[https://github.com/yanfeizhang/coder-kung-fu/blob/main/tests/network/test04/main.c](https://github.com/yanfeizhang/coder-kung-fu/blob/main/tests/network/test04/main.c)<br />编译一下，注意运行需要 root 权限。<br />_# gcc -o main main.c_<br />_# ./main _

运行结果预览如下。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653834621961-d9ebe3bb-6daa-4348-8e63-9d5982b57a6c.png#clientId=u4188de8d-9d39-4&from=paste&id=uaf9123e0&originHeight=180&originWidth=537&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud716c5d4-8e03-4a8e-a11f-b569058bd23&title=)
