Kubernetes<br />深入了解支持服务间通信的 3 个原生 k8s 对象：ClusterIP Service、DNS 和 Kube-Proxy。<br />![概述](https://cdn.nlark.com/yuque/0/2022/png/396745/1658449157486-12ad1bd3-2811-4cd1-921d-47581f3d55c2.png#clientId=u2a9d6507-fdd7-4&from=paste&id=u38a9c3f3&originHeight=890&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u4262ba90-d940-4922-b43e-a0588160cda&title=%E6%A6%82%E8%BF%B0 "概述")
<a name="m4O73"></a>
## 传统的服务到服务通信
在进入 Kubernetes 生态系统之前，快速了解一下传统的服务到服务通信：通信是通过 IP 地址进行的，因此为了让服务 A 调用服务 B，一种方法是为服务 B 分配一个静态 IP 地址。现在，服务 A 已经知道该 IP 地址（这在处理极少数服务时可能会起作用）或服务 B 使用域名注册自己，并且服务 A 通过 DNS 查找获取服务 B 的联系地址。<br />![传统的服务到服务通信](https://cdn.nlark.com/yuque/0/2022/png/396745/1658449157575-b157555f-ba59-441d-b1b4-7ba3332dd333.png#clientId=u2a9d6507-fdd7-4&from=paste&id=u7f2ba05b&originHeight=200&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=uc6df071c-fb3a-43d5-bf39-6f2e2b5db8f&title=%E4%BC%A0%E7%BB%9F%E7%9A%84%E6%9C%8D%E5%8A%A1%E5%88%B0%E6%9C%8D%E5%8A%A1%E9%80%9A%E4%BF%A1 "传统的服务到服务通信")
<a name="NQAEe"></a>
## Kubernetes 网络模型
现在在 Kubernetes 集群中，拥有构成集群管理组件和一组工作机器（称为节点）的控制平面。这些节点托管 Pod，这些 Pod 将后端微服务作为容器化服务运行。<br />![集群内的 Pod 到 Pod 通信](https://cdn.nlark.com/yuque/0/2022/png/396745/1658449157508-ea86f002-9fe5-4233-9527-834912fe5ebb.png#clientId=u2a9d6507-fdd7-4&from=paste&id=ufdde5eb1&originHeight=522&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u75fc1b71-81e9-480c-ad92-e70e5f9e736&title=%E9%9B%86%E7%BE%A4%E5%86%85%E7%9A%84%20Pod%20%E5%88%B0%20Pod%20%E9%80%9A%E4%BF%A1 "集群内的 Pod 到 Pod 通信")<br />根据 Kubernetes 网络模型：

1. **集群中的每个 pod 都有自己唯一的集群范围 IP 地址**
2. **所有 pod 都可以与集群内的每个 pod 通信**
3. **通信在没有 NAT 的情况下发生，这意味着目标 pod 可以看到源 pod 的真实 IP 地址。Kubernetes 认为容器网络或在其上运行的应用程序是可信的，不需要在网络级别进行身份验证。**
<a name="c8OsZ"></a>
## ClusterIP 服务 ~ 基于 Pod 的抽象
既然集群中的每个 pod 都有自己的 IP 地址，那么一个 pod 与另一个 pod 通信应该很容易吧？<br />不，因为 Pod 是易变的，每次创建 pod 时都会获得一个新的 IP 地址。所以客户端服务必须以某种方式切换到下一个可用的 pod。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658449157599-19644b00-ae0c-47f0-8436-bf7d484fe878.png#clientId=u2a9d6507-fdd7-4&from=paste&id=u6e552a3e&originHeight=323&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3a772c5e-964d-4283-b58f-5c32ef70c8c&title=)<br />Pod 直接相互交谈的问题是另一个目标 Pod 的短暂性（随时可能销毁），其次是发现新 Pod IP 地址。<br />因此 Kubernetes 可以在一组 Pod 之上创建一个层，该层可以为该组提供单个 IP 地址并可以提供基本的负载平衡。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658449157518-87706593-03e3-49ab-accd-95be8d078977.png#clientId=u2a9d6507-fdd7-4&from=paste&id=u651f68ab&originHeight=323&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf39a5930-db50-4105-9894-35c267486a7&title=)<br />通过持久 IP 地址上的 ClusterIP 服务公开的 Pod，客户端与服务对话，而不是直接与 Pod 对话。<br />这种抽象是由 Kubernetes 中一个名为**ClusterIP service**的服务对象提供的。它在多个节点上产生，从而在集群中创建单个服务。它可以接收任何端口上的请求并将其转发到 pod 上的任何端口。<br />因此，当应用服务 A 需要与服务 B 对话时，它会调用服务 B 对象的 ClusterIP 服务，而不是运行该服务的单个 pod。<br />ClusterIP 使用 Kubernetes 中标签和选择器的标准模式来不断扫描匹配选择标准的 pod。Pod 有标签，服务有选择器来查找标签。使用它，可以进行基本的流量拆分，其中新旧版本的微服务在同一个 clusterIP 服务后共存。
<a name="Bg88L"></a>
## CoreDNS ~ 集群内的服务发现
现在服务 B 已经获得了一个持久的 IP 地址，服务 A 仍然需要知道这个 IP 地址是什么，然后才能与服务 B 通信。<br />Kubernetes 支持使用 CoreDNS 进行名称解析。服务 A 应该知道它需要与之通信的 ClusterIP 的名称（和端口）。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658449158050-1184cc09-5be9-4217-888c-3f0e892d4658.png#clientId=u2a9d6507-fdd7-4&from=paste&id=udd610e4f&originHeight=402&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3b1fe14b-ed48-45a4-a3c5-93eba0e126a&title=)

1. **CoreDNS 扫描集群，每当创建 ClusterIP 服务时，它的条目就会添加到 DNS 服务器（如果已配置，它还会为每个 pod 添加一个条目，但它与服务到服务的通信无关）。**
2. **接下来，CoreDNS 将自己暴露为 cluster IP 服务（默认称为 kube-dns），并且该服务被配置为 pod 中的 nameserver。**
3. **发起请求的 Pod 从 DNS 获取 ClusterIP 服务的 IP 地址，然后可以使用 IP 地址和端口发起请求。**
<a name="qIWt8"></a>
## Kube-proxy 打通 Service 和后端 Pod 之间（DNAT）
到目前为止，从本文来看，似乎是 ClusterIP 服务将请求调用转发到后端 Pod。但实际上，它是由 Kube-proxy 完成的。<br />Kube-proxy 在每个节点上运行，并监视 Service 及其选择的 Pod（实际上是 Endpoint 对象）。

1. **当节点上运行的 pod 向 ClusterIP 服务发出请求时，kube-proxy 会拦截它。**
2. **通过查看目的 IP 地址和端口，可以识别目的 ClusterIP 服务。并将此请求的目的地替换为实际 Pod 所在的端点地址。**
<a name="OO2Bx"></a>
## 如何协同工作？
![ClusterIP Service、CoreDNS、客户端 Pod、Kube-Proxy、EndPoint的交互](https://cdn.nlark.com/yuque/0/2022/png/396745/1658449158020-8b9bdcb4-ee7d-44a1-8d06-11fe52f21a70.png#clientId=u2a9d6507-fdd7-4&from=paste&id=u521bc8fa&originHeight=469&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u64fa013a-62fc-4848-ae38-6e441bf7b3e&title=ClusterIP%20Service%E3%80%81CoreDNS%E3%80%81%E5%AE%A2%E6%88%B7%E7%AB%AF%20Pod%E3%80%81Kube-Proxy%E3%80%81EndPoint%E7%9A%84%E4%BA%A4%E4%BA%92 "ClusterIP Service、CoreDNS、客户端 Pod、Kube-Proxy、EndPoint的交互")

1. **目标的 ClusterIP 服务在 CoreDNS 中注册**
2. **DNS 解析：每个 pod 都有一个 resolve.conf 文件，其中包含 CoreDNS 服务的 IP 地址，pod 执行 DNS 查找。**
3. **Pod 使用它从 DNS 收到的 IP 地址和它已经知道的端口来调用 clusterIP 服务。**
4. **目标地址转换：Kube-proxy 将目标 IP 地址更新为服务 B 的 Pod 可用的地址。**
<a name="AYfOV"></a>
## 概括
通过本文，看到了使服务到服务通信成为可能的原生 Kubernetes 对象，这些细节对应用程序层是隐藏的。<br />_原文：_[_https://medium.com/codex/east-west-communication-in-kubernetes-how-do-services-communicate-within-a-cluster-310e9dc9dd53_](https://medium.com/codex/east-west-communication-in-kubernetes-how-do-services-communicate-within-a-cluster-310e9dc9dd53)
