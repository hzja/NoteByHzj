Kubernetes<br />使用 Kubernetes 时，用户往往需要共享使用 Kubernetes 集群（多租户），以在满足多团队、多客户需求的同时简化运维、降低成本。虽然 Kubernetes 本身不直接提供多租户功能，但它提供了一系列可被用于支持实现多租户的功能。基于这些功能，Kubernetes 社区涌现了一些实现多租户的项目。**本文将粗浅谈谈 Kubernetes 多租户的现有实现机制及优化方案，以及针对多租户（共享集群）和多集群方案，企业该如何选择。**
<a name="hiUAA"></a>
## Kubernetes 隔离技术
<a name="J61QQ"></a>
### 控制平面隔离
Kubernetes 提供了三个机制来实现控制平面的隔离：namespace、RBAC 和 quota。<br />Namespace 应该是每个 Kubernetes 用户都接触过的概念，它用来提供独立的命名空间。不同命名空间内的对象可以重名。此外，namespace 也限定 RBAC 以及 quota 的作用范围。<br />RBAC 被用来限定用户或者负载对 API 的访问权限。通过设定合适的 RBAC 规则，可以实现对 API 资源的隔离访问。<br />ResourceQuota 可以被用来限制 namespace 下各类资源的使用上限，以防止某个 namespace 占据过多的集群资源而影响其他 namespace 下的应用。不过使用 ResourceQuota 有一个限制条件，即要求 namespace 下的每个容器都指定 resource request 和 limit。<br />尽管这三个机制能一定程度上提供控制平面的隔离，但无法彻底解决问题。例如 CRD 这类集群范围的资源，就无法做到很好的隔离。
<a name="tmrts"></a>
### 数据平面隔离
数据平面的隔离主要分为三个方面：容器运行时、存储和网络。<br />容器和主机共享 kernel，应用程序或者主机系统上的漏洞可能被攻击者所利用，从而突破容器的边界而攻击到主机或者其他容器。解决方法通常是将容器放到一个隔离的环境中运行，例如虚拟机或者是用户态 kernel。前者以 Kata Containers 为代表，后者的代表则是 gVisor。<br />存储的隔离应保证 volume 不会跨租户访问。由于 StorageClass 是集群范围的资源，为防止 PV 被跨租户访问，应指定其 reclaimPolicy 为 Delete。此外，也应禁止使用例如 hostPath 这样的 volume，以避免节点的本地存储被滥用。<br />网络的隔离通常由 NetworkPolicy 保证。默认地，Kubernetes 内所有的 pod 相互之间是可以通信的。利用 NetworkPolicy 则可以限定 pod 能够通信的 pod 范围，以防止意外的网络访问。此外，service mesh 通常能提供更高级的网络隔离功能。
<a name="QitTL"></a>
## 多租户方案选择
上面提到的控制平面和数据平面的隔离功能，都是 Kubernetes 内较为独立零散的功能，与完整的多租户方案还有很大差距，想要把它们组织起来也需要相当大的工作量。不过，Kubernetes 社区有许多开源项目专门解决多租户问题。从大方向上，它们分为两类。**一类是以 namespace 为边界划分租户，另一类则为租户提供虚拟控制平面。**
<a name="FXEKP"></a>
### 按 namespace 划分租户
Kubernetes 的控制平面隔离中的 RBAC 和 ResourceQuota 均以 namespace 为边界，因此以 namespace 来划分租户是比较自然的想法。不过，在现实中，限定一个租户只能使用一个命名空间存在较大局限性。例如无法进一步以团队，或者以应用为粒度进行细分，造成一定的管理难度。因此 Kubernetes 官方提供了支持层级化 namespace 的 controller。此外，第三方开源项目例如 Capsule 和 kiosk 提供了更为丰富的多租户支持。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668558559578-858c4ebf-016c-49b7-9836-11b2710b0e58.png#averageHue=%23958f40&clientId=uc7990602-267e-4&from=paste&id=u0dd606d8&originHeight=685&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u67181f7e-0b05-4314-9f60-50f398d7315&title=)
<a name="BB6QU"></a>
### 虚拟控制平面
另一种多租户的实现方案是为每个租户提供一个独立的虚拟控制平面，以彻底隔离租户的资源。虚拟控制平面的实现方式通常是为每个租户运行一套独立的 apiserver，同时利用 controller 将租户 apiserver 中的资源同步到原 Kubernetes 集群中。每个租户只能访问自己对应的 apiserver，而原 Kubernetes 集群的 apiserver 则通常不对外访问。<br />这类方案的代价是额外的 apiserver 的开销，但能够获得更为彻底的控制平面隔离。结合数据平面的隔离技术，虚拟控制平面可以实现更为彻底和安全的多租户方案。此类方案以 vcluster 项目为代表。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668558559582-4467d3d5-5204-4ce9-9f39-d79dbe7dd3a9.png#averageHue=%23fdfdfc&clientId=uc7990602-267e-4&from=paste&id=ua57266f3&originHeight=403&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u738938b6-dc09-4ad7-94d7-64ce8c641cc&title=)
<a name="aoAPH"></a>
### 如何选择？
**选择按 namespace 划分租户还是使用虚拟控制平面应取决于多租户的使用场景。**通常来说，按 namespace 划分租户的隔离性和自由度会略有欠缺，但优势在于轻量。对于多团队共享使用的场景，按 namespace 划分租户较为合适。而对于多客户共享使用的场景，选择虚拟控制平面通常能够提供更好的隔离保障。
<a name="DBR6f"></a>
## 多集群方案
从上文可以看出，共享使用 Kubernetes 集群并非易事；Kubernetes 集群并非天然地支持多租户，仅仅是提供了一些细粒度上的功能支持。要想让 Kubernetes 支持多租户场景需要其他项目的支持，以同时在控制平面和数据平面上实现租户之间的隔离。这使得整个方案存在不小的学习和适应成本。因此，**目前有许多用户采用的不是共享集群，而是多集群的方案。**<br />相比共享集群，多集群方案有利有弊：优势是隔离性强、边界清晰，劣势是资源开销以及运维成本较高。由于每个集群需要独立的控制平面和工作节点，为了提高物理集群的利用率，通常会选择在虚拟机上搭建 Kubernetes 集群。**然而传统的虚拟化产品因为需要顾及更为广泛的场景，所以功能上往往大而全，并且售价高昂，并非支撑虚拟化 Kubernetes 集群的最佳选择。**<br />基于此，可以认为，支撑虚拟化 Kubernetes 集群理想的虚拟化平台应该具备以下特点：

- **轻量**。不需要顾及到桌面虚拟化等场景，只需专注于服务器虚拟化，减去一切不必要的功能和开销。
- **高效**。全面使用 virtio 这样的半虚拟化 I/O 技术，提高效率。
- **安全**。最大程度减少主机受到攻击的可能。
- **Kubernetes 原生**。虚拟化平台本身最好是 Kubernetes 集群，以降低学习和运维成本。

而这些特点，恰恰是 SmartX 前段时间发布的 Virtink 虚拟化引擎所具备的。Virtink 基于 Cloud Hypervisor 项目，提供在 Kubernetes 上编排轻量化、现代化虚拟机的能力。相比基于 QEMU 和 libvirt 的 KubeVirt 项目，Virtink 能为每个 VM 降低至少 100MB 的额外开销。此外，Virtink 全面使用 virtio，提高 I/O 效率。最后，在安全性方面，Cloud Hypervisor 由 Rust 语言编写，具备更为安全的内存管理。并且通过减少过时和不必要的外设支持，尽可能地减小暴露给 VM 的可攻击面，以提高主机的安全性。**更轻量、更高效、更安全的 Virtink 为 Kubernetes in Kubernetes 提供了更为理想和经济的虚拟化支持，尽最大可能降低虚拟化层的开销。**<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668558559653-4d9ea804-0403-422f-bf52-6c2ce31f7afb.png#averageHue=%23b0b3ef&clientId=uc7990602-267e-4&from=paste&id=ua13bb960&originHeight=462&originWidth=882&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u92e8a71b-e7f9-4d21-88b3-4ed86d9834d&title=)<br />此外，针对在 Virtink 上创建和运维虚拟化 Kubernetes 集群的需要，**SmartX 开发了 knest 命令行工具来帮助用户一键式创建集群以及对集群进行扩缩容管理。**在 Virtink 集群上创建一个虚拟化 Kubernetes 集群仅需执行“knest create”命令即可实现。对集群进行后续的扩缩容也可以通过 knest 工具进行一键式操作。
<a name="g7COW"></a>
## 总   结
Kubernetes 并未内建多租户功能，但提供了一些细粒度的功能支持。利用这些功能，结合一些第三方工具，能够实现多租户共享使用集群。但同时这些工具也带来了额外的学习和运维成本。在这个情况下，多个虚拟化集群依然是很多用户选择的方案。然而传统的虚拟化平台在效率和成本上并非此场景的最佳选择。SmartX Virtink 开源虚拟化引擎基于高效、安全的 Cloud Hypervisor，提供 Kubernetes 上编排轻量虚拟机的能力，最大程度降低虚拟化 Kubernetes 集群的资源开销。配套提供的 knest 命令行工具则支持集群一键式创建和运维，有效降低多个集群的运维成本<br />**相关链接：**

1. Kubernetes-sigs / hierarchical-namespaces [https://github.com/kubernetes-sigs/hierarchical-namespaces](https://github.com/kubernetes-sigs/hierarchical-namespaces)
2. clastix / capsule [https://github.com/clastix/capsule](https://github.com/clastix/capsule)
3. loft-sh / kiosk [https://github.com/loft-sh/kiosk](https://github.com/loft-sh/kiosk)
4. loft-sh / vcluster [https://github.com/loft-sh/vcluster](https://github.com/loft-sh/vcluster)
5. smartxworks / virtink [https://github.com/smartxworks/virtink](https://github.com/smartxworks/virtink)
6. smartxworks / knest [https://github.com/smartxworks/knest](https://github.com/smartxworks/knest)
