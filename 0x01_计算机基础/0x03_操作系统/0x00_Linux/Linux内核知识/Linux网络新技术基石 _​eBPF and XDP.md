Linux 网络<br />当前正流行网络技是什么？那就是eBPF和XDP技术，Cilium+eBPF超级火热，Google GCP也刚刚全面转过来。
<a name="d5F5Q"></a>
## 新技术出现的历史原因
iptables/netfilter<br />iptables/netfilter 是上个时代Linux网络提供的优秀的防火墙技术，扩展性强，能够满足当时大部分网络应用需求。但该框架也存在很多明显问题：

- 路径太长

netfilter 框架在IP层，报文需要经过链路层，IP层才能被处理，如果是需要丢弃报文，会白白浪费很多CPU资源，影响整体性能；

- O(N)匹配

![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623550625173-84daa635-b054-4b59-a657-a524d92d3fa2.png#clientId=u6ab4687e-5980-4&from=paste&id=u07fa05d8&originHeight=637&originWidth=807&originalType=url&ratio=3&status=done&style=shadow&taskId=u9ce7385c-2e8c-49ff-8361-5e513605324)<br />如上图所示，极端情况下，报文需要依次遍历所有规则，才能匹配中，极大影响报文处理性能；

- 规则太多

netfilter 框架类似一套可以自由添加策略规则专家系统，并没有对添加规则进行合并优化，这些都严重依赖操作人员技术水平，随着规模的增大，规则数量n成指数级增长，而报文处理又是0（n）复杂度，最终性能会直线下降。<br />内核协议栈<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623550625234-8ee34d79-e9c4-4ac9-9e1a-699c1e83da19.png#clientId=u6ab4687e-5980-4&from=paste&id=uca995dd0&originHeight=508&originWidth=885&originalType=url&ratio=3&status=done&style=shadow&taskId=u3cdf2ceb-b0ed-4174-bc97-9ee3672991b)<br />随着互联网流量越来愈大, 网卡性能越来越强，Linux内核协议栈在10Mbps/100Mbps网卡的慢速时代是没有任何问题的，那个时候应用程序大部分时间在等网卡送上来数据。<br />现在到了1000Mbps/10Gbps/40Gbps网卡的时代，数据被很快地收入，协议栈复杂处理逻辑，效率捉襟见肘，把大量报文堵在内核里。<br />各类链表在多CPU环境下的同步开销。<br />不可睡眠的软中断路径过长。<br />sk_buff的分配和释放。<br />内存拷贝的开销。<br />上下文切换造成的cache miss。<br />…<br />于是，内核协议栈各种优化措施应着需求而来：<br />网卡RSS，多队列。<br />中断线程化。<br />分割锁粒度。<br />Busypoll。<br />…<br />但却都是见招拆招，治标不治本。问题的根源不是这些机制需要优化，而是这些机制需要推倒重构。蒸汽机车刚出来的时候，马车夫为了保持竞争优势，不是去换一匹昂贵的快马，而是卖掉马去买一台蒸汽机装上。基本就是这个意思。<br />重构的思路很显然有两个：<br />upload方法：别让应用程序等内核了，让应用程序自己去网卡直接拉数据。<br />offload方法：别让内核处理网络逻辑了，让网卡自己处理。<br />总之，绕过内核就对了，内核协议栈背负太多历史包袱。<br />DPDK让用户态程序直接处理网络流，bypass掉内核，使用独立的CPU专门干这个事。<br />XDP让灌入网卡的eBPF程序直接处理网络流，bypass掉内核，使用网卡NPU专门干这个事。<br />如此一来，内核协议栈就不再参与数据平面的事了，留下来专门处理诸如路由协议，远程登录等控制平面和管理平面的数据流。<br />改善iptables/netfilter的规模瓶颈，提高Linux内核协议栈IO性能，内核需要提供新解决方案，那就是eBPF/XDP框架，来看一看，这套框架是如何解决问题的。
<a name="PZmtO"></a>
## eBPF到底是什么?
<a name="PLdbz"></a>
### eBPF的历史
BPF 是 Linux 内核中高度灵活和高效的类似虚拟机的技术，允许以安全的方式在各个挂钩点执行字节码。它用于许多 Linux 内核子系统，最突出的是网络、跟踪和安全（例如沙箱）。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623550625226-a3dee262-72fb-461f-8944-849a6cfd3255.png#clientId=u6ab4687e-5980-4&from=paste&id=u3248fbeb&originHeight=418&originWidth=781&originalType=url&ratio=3&status=done&style=shadow&taskId=uc224a055-9c4d-400e-80e6-de357af73b6)<br />BPF架构<br />BPF 是一个通用目的 RISC 指令集，其最初的设计目标是：用 C 语言的一个子集编 写程序，然后用一个编译器后端（例如 LLVM）将其编译成 BPF 指令，稍后内核再通 过一个位于内核中的（in-kernel）即时编译器（JIT Compiler）将 BPF 指令映射成处理器的原生指令（opcode ），以取得在内核中的最佳执行性能。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623550625185-b3f5de20-1f76-4bcf-8be9-6476c9e05bde.png#clientId=u6ab4687e-5980-4&from=paste&id=u163a2a3d&originHeight=313&originWidth=519&originalType=url&ratio=3&status=done&style=none&taskId=u2e949b21-d49e-4006-b0ac-4097b3c5e8d)<br />BPF指令<br />尽管 BPF 自 1992 年就存在，扩展的 Berkeley Packet Filter (eBPF) 版本首次出现在 Kernel3.18中，如今被称为“经典”BPF (cBPF) 的版本已过时。许多人都知道 cBPF是tcpdump使用的数据包过滤语言。现在Linux内核只运行 eBPF，并且加载的 cBPF 字节码在程序执行之前被透明地转换为内核中的eBPF表示。除非指出 eBPF 和 cBPF 之间的明确区别，一般现在说的BPF就是指eBPF。
<a name="JZJLx"></a>
### eBPF总体设计
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623550625177-94ffd22e-9ae6-4aad-8ce0-8cffc4af92ca.png#clientId=u6ab4687e-5980-4&from=paste&id=u93d258e4&originHeight=608&originWidth=1080&originalType=url&ratio=3&status=done&style=shadow&taskId=u9a7cdf59-edeb-48d3-ab3a-6fd27e214fd)

- BPF 不仅通过提供其指令集来定义自己，而且还通过提供围绕它的进一步基础设施，例如充当高效键/值存储的映射、与内核功能交互并利用内核功能的辅助函数、调用其他 BPF 程序的尾调用、安全加固原语、用于固定对象（地图、程序）的伪文件系统，以及允许将 BPF 卸载到网卡的基础设施。
- LLVM 提供了一个 BPF后端，因此可以使用像 clang 这样的工具将 C 编译成 BPF 目标文件，然后可以将其加载到内核中。BPF与Linux 内核紧密相连，允许在不牺牲本机内核性能的情况下实现完全可编程。

eBPF总体设计包括以下几个部分：<br />eBPF Runtime<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623550626096-db77857c-e7e9-4ec7-a4a1-3ffe89a6c435.png#clientId=u6ab4687e-5980-4&from=paste&id=u74a20347&originHeight=473&originWidth=1080&originalType=url&ratio=3&status=done&style=shadow&taskId=udb04b6d7-f554-4044-9f9e-c08c45bbfb2)

- 安全保障 ： eBPF的verifier 将拒绝任何不安全的程序并提供沙箱运行环境
- 持续交付： 程序可以更新在不中断工作负载的情况下
- 高性能：JIT编译器可以保证运行性能

eBPF Hooks<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623550626226-3200223f-ef5c-4b7c-8a77-cb7131a79969.png#clientId=u6ab4687e-5980-4&from=paste&id=u30352ea8&originHeight=602&originWidth=1060&originalType=url&ratio=3&status=done&style=shadow&taskId=ua4c93fad-7348-46bc-b53e-bb7e8a39825)

- 内核函数 (kprobes)、用户空间函数 (uprobes)、系统调用、fentry/fexit、跟踪点、网络设备 (tc/xdp)、网络路由、TCP 拥塞算法、套接字（数据面）

eBPF Maps

![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623550626506-319e6503-d2b9-4a84-a7e5-2cfdb34b0ac7.png#clientId=u6ab4687e-5980-4&from=paste&id=u54676cd6&originHeight=465&originWidth=1060&originalType=url&ratio=3&status=done&style=shadow&taskId=u19e980ee-f821-4643-a9e6-134cb26657a)<br />Map 类型<br />- Hash tables, Arrays<br />- LRU (Least Recently Used)<br />- Ring Buffer<br />- Stack Trace<br />- LPM (Longest Prefix match)<br />作用

- 程序状态
- 程序配置
- 程序间共享数据
- 和用户空间共享状态、指标和统计

eBPF Helpers<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623550626548-86e5e9cb-04ce-42ac-829f-92c6d3402615.png#clientId=u6ab4687e-5980-4&from=paste&id=u4d11057b&originHeight=420&originWidth=1040&originalType=url&ratio=3&status=done&style=shadow&taskId=ud5ab6b8c-adca-4d31-a78b-a650e47cba0)<br />有哪些Helpers？

- 随机数
- 获取当前时间
- map访问
- 获取进程/cgroup 上下文
- 处理网络数据包和转发
- 访问套接字数据
- 执行尾调用
- 访问进程栈
- 访问系统调用参数
- ...

eBPF Tail and Function Calls<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623550626535-4f442d00-ff76-411b-b7af-63ca6e0de53d.png#clientId=u6ab4687e-5980-4&from=paste&id=udd1c4cb9&originHeight=258&originWidth=988&originalType=url&ratio=3&status=done&style=shadow&taskId=ue8143023-d225-4928-9085-13393ad11e9)<br />尾调用有什么用？<br />● 将程序链接在一起<br />● 将程序拆分为独立的逻辑组件<br />● 使 BPF 程序可组合<br />函数调用有什么用？<br />● 重用内部的功能程序<br />● 减少程序大小（避免内联）<br />eBPF JIT Compiler<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623550626810-35155a5e-d091-409a-b913-3e283f9abd96.png#clientId=u6ab4687e-5980-4&from=paste&id=u5771ad95&originHeight=418&originWidth=683&originalType=url&ratio=3&status=done&style=shadow&taskId=uf981729c-ddba-49ad-9bee-41687edb9f9)

- 确保本地执行性能而不需要了解CPU
- 将 BPF字节码编译到CPU架构特定指令集
<a name="GORBm"></a>
### eBPF可以做什么？
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623550629138-4e007d44-345c-4a28-a201-0f90d1cc0ef2.png#clientId=u6ab4687e-5980-4&from=paste&id=uf5f4cd8b&originHeight=310&originWidth=539&originalType=url&ratio=3&status=done&style=shadow&taskId=u8d1bb48a-cb58-4ba3-a5d0-d44729fda08)
<a name="OZg3f"></a>
### eBPF 开源 Projects
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623550627229-0ffc2ac4-d2ee-4ff7-a922-9ea5888fe73a.png#clientId=u6ab4687e-5980-4&from=paste&id=uadd1aa50&originHeight=444&originWidth=1080&originalType=url&ratio=3&status=done&style=shadow&taskId=u1859a6eb-e8c0-476a-b321-58f71ce09c3)
<a name="oK6b7"></a>
#### Cilium
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623550627425-e6a314ae-58b9-41cc-89c7-b25302b1f37b.png#clientId=u6ab4687e-5980-4&from=paste&id=u3e555f9c&originHeight=348&originWidth=607&originalType=url&ratio=3&status=done&style=shadow&taskId=ua60bcac4-5f65-4538-a0bd-d49d87610d3)

- Cilium 是开源软件，用于Linux容器管理平台（如 Docker 和 Kubernetes）部署的服务之间的透明通信和提供安全隔离保护。
- Cilium基于微服务的应用，使用HTTP、gRPC、Kafka等轻量级协议API相互通信。

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623550627353-430b20f1-234c-4e5f-8dca-5ef372e9a79c.webp#clientId=u6ab4687e-5980-4&from=paste&id=ued3b6d79&originHeight=634&originWidth=1080&originalType=url&ratio=3&status=done&style=shadow&taskId=u74bbbf6f-4fe0-491c-8185-e58fa124828)

- Cilium 的基于 eBPF 的新 Linux 内核技术，它能够在 Linux 本身中动态插入强大的安全可见性和控制逻辑。由于 eBPF 在 Linux 内核中运行，因此可以在不更改应用程序代码或容器配置的情况下应用和更新 Cilium 安全策略。
<a name="K16Px"></a>
##### Cilium在它的 datapath 中重度使用了 BPF 技术
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623550627415-8f172f0b-809a-40e3-a57d-d5268a276a5a.png#clientId=u6ab4687e-5980-4&from=paste&id=u8d076741&originHeight=962&originWidth=1080&originalType=url&ratio=3&status=done&style=shadow&taskId=u80153a6e-1eda-4844-81ee-b8a0fdb13a1)

- Cilium 是位于 Linux kernel 与容器编排系统的中间层。向上可以为容器配置网络，向下可以向 Linux 内核生成 BPF 程序来控制容器的安全性和转发行为。
- 利用 Linux BPF，Cilium 保留了透明地插入安全可视性 + 强制执行的能力，但这种方式基于服务 /pod/ 容器标识（与传统系统中的 IP 地址识别相反），并且可以根据应用层进行过滤 （例如 HTTP）。因此，通过将安全性与寻址分离，Cilium 不仅可以在高度动态的环境中应用安全策略，而且除了提供传统的第 3 层和第 4 层分割之外，还可以通过在 HTTP 层运行来提供更强的安全隔离。
- BPF 的使用使得 Cilium 能够以高度可扩展的方式实现以上功能，即使对于大规模环境也不例外。

对比传统容器网络（采用iptables/netfilter）：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623550627900-35b2733b-1fca-49dd-9a2a-36e7f1c78e15.webp#clientId=u6ab4687e-5980-4&from=paste&id=ufabffc10&originHeight=605&originWidth=1080&originalType=url&ratio=3&status=done&style=shadow&taskId=u0ce12fbf-70f2-48bb-b1a6-20bffe678d0)

- eBPF主机路由允许绕过主机命名空间中所有的 iptables 和上层网络栈，以及穿过Veth对时的一些上下文切换，以节省资源开销。网络数据包到达网络接口设备时就被尽早捕获，并直接传送到Kubernetes Pod的网络命名空间中。在流量出口侧，数据包同样穿过Veth对，被eBPF捕获后，直接被传送到外部网络接口上。eBPF直接查询路由表，因此这种优化完全透明。
- 基于eBPF中的kube-proxy网络技术正在替换基于iptables的kube-proxy技术，与Kubernetes中的原始kube-proxy相比，eBPF中的kuber-proxy替代方案具有一系列重要优势，例如更出色的性能、可靠性以及可调试性等等。
<a name="HI8wf"></a>
#### BCC(BPF Compiler Collection)
BCC 是一个框架，它使用户能够编写嵌入其中的 eBPF 程序的 Python 程序。该框架主要针对涉及应用程序和系统分析/跟踪的用例，其中 eBPF 程序用于收集统计信息或生成事件，用户空间中的对应部分收集数据并以人类可读的形式显示。运行 python 程序将生成 eBPF 字节码并将其加载到内核中。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623550628127-82214bb1-d28f-403e-9be6-b0c79ebf7017.png#clientId=u6ab4687e-5980-4&from=paste&id=u582e18bc&originHeight=389&originWidth=954&originalType=url&ratio=3&status=done&style=shadow&taskId=uac03c555-aff9-4df6-9e5d-bb17839c99f)
<a name="zNnIW"></a>
#### bpftrace
bpftrace 是一种用于 Linux eBPF 的高级跟踪语言，可在最近的 Linux 内核 (4.x) 中使用。bpftrace 使用 LLVM 作为后端将脚本编译为 eBPF 字节码，并利用 BCC 与 Linux eBPF 子系统以及现有的 Linux 跟踪功能进行交互：内核动态跟踪 (kprobes)、用户级动态跟踪 (uprobes) 和跟踪点. bpftrace 语言的灵感来自 awk、C 和前身跟踪器，例如 DTrace 和 SystemTap。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623550628391-6907e34d-01c8-4214-abb1-7fca4ccb1d8d.png#clientId=u6ab4687e-5980-4&from=paste&id=ue6f54921&originHeight=365&originWidth=930&originalType=url&ratio=3&status=done&style=shadow&taskId=ue0c6d550-9121-4443-a217-925ce672ed9)
<a name="hw1wE"></a>
#### eBPF Go 库
eBPF Go 库提供了一个通用的 eBPF 库，它将获取 eBPF 字节码的过程与 eBPF 程序的加载和管理解耦。eBPF 程序通常是通过编写高级语言创建的，然后使用 clang/LLVM 编译器编译为 eBPF 字节码。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623550628412-90b792c7-9d23-4252-987b-ba2ec7d3a547.png#clientId=u6ab4687e-5980-4&from=paste&id=u435e9902&originHeight=496&originWidth=938&originalType=url&ratio=3&status=done&style=shadow&taskId=u557803d1-63de-4b8e-a505-f0c937978ab)
<a name="Lx1ik"></a>
#### libbpf C/C++ 库
libbpf 库是一个基于 C/C++ 的通用 eBPF 库，它有助于解耦从 clang/LLVM 编译器生成的 eBPF 目标文件加载到内核中，并通过提供易于使用的库 API 来抽象与 BPF 系统调用的交互应用程序。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623550628714-599d62e1-5c4c-4303-ac07-3849e62a5bbd.png#clientId=u6ab4687e-5980-4&from=paste&id=u6f8909ed&originHeight=487&originWidth=951&originalType=url&ratio=3&status=done&style=shadow&taskId=u4b3df4e7-eb36-44d7-96c9-464a2e640e7)
<a name="TD1dD"></a>
## 那XDP又是什么?
XDP的全称是： eXpress Data Path<br />XDP 是Linux 内核中提供高性能、可编程的网络数据包处理框架。
<a name="PFx1t"></a>
### XDP整体框架
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623550628823-da70ed65-125c-4f07-ae3f-60d169c04a8a.png#clientId=u6ab4687e-5980-4&from=paste&id=ua0ec14d1&originHeight=560&originWidth=1024&originalType=url&ratio=3&status=done&style=shadow&taskId=ubaa832e5-b299-4527-bbbf-089c7088685)

- 直接接管网卡的RX数据包（类似DPDK用户态驱动）处理；
- 通过运行BPF指令快速处理报文；
- 和Linux协议栈无缝对接；
<a name="SLZUw"></a>
### XDP总体设计
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623550629172-60266ff9-af47-4168-813f-d3958145ef31.png#clientId=u6ab4687e-5980-4&from=paste&id=uf06d46fc&originHeight=658&originWidth=616&originalType=url&ratio=3&status=done&style=shadow&taskId=uf5ce9e61-b907-48c4-b9a7-e89d2654e56)
<a name="FA86w"></a>
#### XDP总体设计包括以下几个部分：
<a name="TNOdU"></a>
##### XDP驱动
网卡驱动中XDP程序的一个挂载点，每当网卡接收到一个数据包就会执行这个XDP程序；XDP程序可以对数据包进行逐层解析、按规则进行过滤，或者对数据包进行封装或者解封装，修改字段对数据包进行转发等；
<a name="e8yGM"></a>
##### BPF虚拟机
并没有在图里画出来，一个XDP程序首先是由用户编写用受限制的C语言编写的，然后通过clang前端编译生成BPF字节码，字节码加载到内核之后运行在eBPF虚拟机上，虚拟机通过即时编译将XDP字节码编译成底层二进制指令；eBPF虚拟机支持XDP程序的动态加载和卸载；
<a name="G0c3W"></a>
##### BPF maps
存储键值对，作为用户态程序和内核态XDP程序、内核态XDP程序之间的通信媒介，类似于进程间通信的共享内存访问；用户态程序可以在BPF映射中预定义规则，XDP程序匹配映射中的规则对数据包进行过滤等；XDP程序将数据包统计信息存入BPF映射，用户态程序可访问BPF映射获取数据包统计信息；
<a name="yAeWZ"></a>
##### BPF程序校验器
XDP程序肯定是自己编写的，那么如何确保XDP程序加载到内核之后不会导致内核崩溃或者带来其他的安全问题呢？程序校验器就是在将XDP字节码加载到内核之前对字节码进行安全检查，比如判断是否有循环，程序长度是否超过限制，程序内存访问是否越界，程序是否包含不可达的指令；
<a name="c1Fc7"></a>
### XDP Action
XDP用于报文的处理，支持如下action：<br />enum xdp_action {    XDP_ABORTED = 0,    XDP_DROP,    XDP_PASS,    XDP_TX,    XDP_REDIRECT,};

- XDP_DROP：在驱动层丢弃报文，通常用于实现DDos或防火墙
- XDP_PASS：允许报文上送到内核网络栈，同时处理该报文的CPU会分配并填充一个skb，将其传递到GRO引擎。之后的处理与没有XDP程序的过程相同。
- XDP_TX：从当前网卡发送出去。
- XDP_REDIRECT：从其他网卡发送出去。
- XDP_ABORTED：表示程序产生了异常，其行为和 XDP_DROP相同，但 XDP_ABORTED 会经过 trace_xdp_exception tracepoint，因此可以通过 tracing 工具来监控这种非正常行为。
<a name="ajtj9"></a>
### AF_XDP
AF_XDP 是为高性能数据包处理而优化的地址族，AF_XDP 套接字使 XDP 程序可以将帧重定向到用户空间应用程序中的内存缓冲区。
<a name="nY7KA"></a>
### XDP设计原则

- XDP 专为高性能而设计。它使用已知技术并应用选择性约束来实现性能目标
- XDP 还具有可编程性。无需修改内核即可即时实现新功能
- XDP 不是内核旁路。它是内核协议栈的快速路径
- XDP 不替代TCP/IP 协议栈。与协议栈协同工作
- XDP 不需要任何专门的硬件。它支持网络硬件的少即是多原则
<a name="ZyuPB"></a>
### XDP技术优势
<a name="x4qTR"></a>
#### 及时处理

- 在网络协议栈前处理，由于 XDP 位于整个 Linux 内核网络软件栈的底部，能够非常早地识别并丢弃攻击报文，具有很高的性能。可以改善 iptables 协议栈丢包的性能瓶颈
- DDIO
- Packeting steering
- 轮询式
<a name="qg2iV"></a>
#### 高性能优化

- 无锁设计
- 批量I/O操作
- 不需要分配skbuff
- 支持网络卸载
- 支持网卡RSS
<a name="HWV1a"></a>
#### 指令虚拟机

- 规则优化，编译成精简指令，快速执行
- 支持热更新，可以动态扩展内核功能
- 易编程-高级语言也可以间接在内核运行
- 安全可靠，BPF程序先校验后执行，XDP程序没有循环
<a name="F2Sxx"></a>
#### 可扩展模型

- 支持应用处理（如应用层协议GRO）
- 支持将BPF程序卸载到网卡
- BPF程序可以移植到用户空间或其他操作系统
<a name="ckxEM"></a>
#### 可编程性

- 包检测，BPF程序发现的动作
- 灵活（无循环）协议头解析
- 可能由于流查找而有状态
- 简单的包字段重写（encap/decap）
<a name="bnzZr"></a>
### XDP 工作模式
XDP 有三种工作模式，默认是 native（原生）模式，当讨论 XDP 时通常隐含的都是指这 种模式。

- Native XDP默认模式，在这种模式中，XDP BPF 程序直接运行在网络驱动的早期接收路径上（ early receive path）。
- Offloaded XDP在这种模式中，XDP BPF程序直接 offload 到网卡。
- Generic XDP对于还没有实现 native 或 offloaded XDP 的驱动，内核提供了一个 generic XDP 选 项，这种设置主要面向的是用内核的 XDP API 来编写和测试程序的开发者，对于在生产环境使用XDP，推荐要么选择native要么选择offloaded模式。
<a name="BHTbc"></a>
### XDP vs DPDK
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623550629179-3f21c99d-1539-447a-939f-0bfd6737f791.png#clientId=u6ab4687e-5980-4&from=paste&id=u5719878f&originHeight=352&originWidth=686&originalType=url&ratio=3&status=done&style=shadow&taskId=ub71cb33c-e14b-4206-b2f3-d8ec1f17163)<br />相对于DPDK，XDP：
<a name="ytImr"></a>
### 优点

- 无需第三方代码库和许可
- 同时支持轮询式和中断式网络
- 无需分配大页
- 无需专用的CPU
- 无需定义新的安全网络模型
<a name="kElpB"></a>
### 缺点
注意XDP的性能提升是有代价的，它牺牲了通用型和公平性

- XDP不提供缓存队列（qdisc），TX设备太慢时直接丢包，因而不要在RX比TX快的设备上使用XDP
- XDP程序是专用的，不具备网络协议栈的通用性
<a name="jz4xl"></a>
### 如何选择？

- 内核延伸项目，不想bypass内核的下一代高性能方案；
- 想直接重用内核代码；
- 不支持DPDK程序环境；
<a name="feqvP"></a>
### XDP适合场景

- DDoS防御
- 防火墙
- 基于XDP_TX的负载均衡
- 网络统计
- 流量监控
- 栈前过滤/处理
- ...
<a name="Lgvmo"></a>
### XDP例子
下面是一个最小的完整 XDP 程序，实现丢弃包的功能（xdp-example.c）：
```c
#include <linux/bpf.h>

#ifndef __section
# define __section(NAME)                  \
   __attribute__((section(NAME), used))
#endif

__section("prog")
int xdp_drop(struct xdp_md *ctx)
{
    return XDP_DROP;
}

char __license[] __section("license") = "GPL";
```
用下面的命令编译并加载到内核：
```bash
$ clang -O2 -Wall -target bpf -c xdp-example.c -o xdp-example.o
$ ip link set dev em1 xdp obj xdp-example.o
```
以上命令将一个 XDP 程序 attach 到一个网络设备，需要是 Linux 4.11 内核中支持 XDP 的设备，或者 4.12+ 版本的内核。
<a name="F8ZyF"></a>
## 最后
eBPF/XDP 作为Linux网络革新技术正在悄悄改变着Linux网络发展模式。<br />eBPF正在将Linux内核转变为微内核，越来越多的新内核功能采用eBPF实现，让新增内核功能更加快捷高效。<br />总体而言，基于业界基准测试结果，eBPF 显然是解决具有挑战性的云原生需求的最佳技术。
