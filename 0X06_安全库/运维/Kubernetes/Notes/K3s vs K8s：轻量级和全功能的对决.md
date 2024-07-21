Kubernetes，通常缩写为 K8s，是领先的容器编排工具。该开源项目最初由 Google 开发，帮助塑造了现代编排的定义。该系统包括了部署和运行容器化系统所需的一切。<br />社区供应商基于 Kubernetes 创建了适用于不同用例的独立发行版。**K3s** 是由 Rancher 创建的一种 kubernetes 流行发行版，现在作为**（CNCF）的一部分进行维护。<br />K3s 的目标是成为一个轻量级的 Kubernetes 版本，适合在资源受限的硬件上使用，例如 IoT 设备。它还易于设置和使用，因此非常适合用于本地开发集群。它专注于边缘部署，但并不排除对大规模云部署的支持：**K3s 的 CNCF 认证意味着它提供了所有 Kubernetes 功能，并且可以用于生产环境**。<br />在本文中，将了解 K3s 与 Kubernetes 项目提供的官方发行版之间的比较，包括它们的区别、何时使用它们以及掌握它们的容易程度。
<a name="pKv93"></a>
## 什么是 K8s
Kubernetes 是一个开源的容器编排引擎，用来对容器化应用进行自动化部署、 扩缩和管理。该项目托管在 CNCF。<br />Kubernetes 提供了部署容器并在多个主机上进行扩展的所有所需工具。Kubernetes 集群中的每台主机被称为一个节点，节点由 Kubernetes 控制平面管理。它会将容器调度到空闲节点上，管理网络和存储并提供与之交互的 API。
<a name="zldSS"></a>
## 什么是 K3s
K3s 是由 Rancher 主导开发的 Kubernetes 发行版。它在不分叉的基础上构建了上游项目。概念上，Kubernetes 发行版类似于 Linux 操作系统：K3s 是一种 Kubernetes 发行版，就像 Ubuntu 是一种 Linux 发行版一样。K3s 在保留 Kubernetes 功能的基础上，还增加了自己的功能。<br />K3s 经过专门设计，即使在最小的硬件环境中也能良好运行。K3s 提供了一个小于 60MB 的单个二进制文件。这个轻量级可执行文件包含了启动完全功能的 Kubernetes 集群所需的一切。<br />通过放弃非必要的 Kubernetes 功能（如云服务提供商集成和**非 CSI[3]** 存储提供商），实现了这个小巧的二进制文件大小。利用 Go 语言的 **goroutines[4]**，将各个 Kubernetes 组件从单个入口点运行起来。
<a name="WUIEw"></a>
## K3s 和 K8s 易于部署
通常情况下，相比 K8s，K3s 更容易部署和维护。轻量级的二进制文件可以用一个命令启动所有的 Kubernetes 控制平面组件。而要启动官方的 Kubernetes 集群则需要更多的时间和步骤，并且可能更难维护。
<a name="Odscb"></a>
### 部署 K3s
以下命令可以启动并运行一个 K3s 集群：
```bash
$ curl -sFL https://get.k3s.io | sh -
```
官方的安装脚本会下载二进制文件并注册一个系统服务，该系统服务会在进程终止或主机重新启动时自动启动 K3s。它还配置了 Kubernetes 实用工具，包括 kubectl CLI。在新的机器上运行脚本后，应该能够在几秒钟内与集群进行交互：
```bash
$ kubectl run nginx --image=nginx
pod/nginx created
```
可以通过在其他节点上运行以下命令，轻松地将节点加入到 K3s 集群中：
```bash
$ sudo k3s agent --server https://<control-plane-ip>:6443 \
--token <node-token>
```
可以通过从运行 K3s 控制平面的机器上读取 /var/lib/rancher/k3s/server/node-token 文件来获取 <node-token> 的值。<br />K3s 还可以通过 Rancher 开发的 **k3d** 部署。k3d 将 K3s 封装在 docker 容器内并运行。K3d 允许在单个主机上运行多个 K3s 集群，并使用熟悉的 Docker 工具进行管理。可以使用以下命令安装 k3d：
```bash
$ curl -s https://raw.githubusercontent.com/k3d-io/k3d/main/install.sh | \
TAG=v5.4.5 bash
```
然后创建第一个集群：
```bash
$ k3d create cluster demo-cluster
```
现在可以使用 kubectl 来向 k3d/K3s 集群添加对象：
```bash
$ kubectl run nginx –image nginx:latest
pod/nginx created
```
<a name="MQv9x"></a>
### 部署 K8s
K8s 的部署过程比较复杂。Kubernetes 项目提供了各个组件的下载，比如：API server、controller manager 和 scheduler。需要成功部署每个组件来创建控制平面。然后，还需要在每个工作节点上安装 kubelet。<br />通过 kubeadm 工具，可以简化 Kubernetes 安装。在使用 kubeadm 之前，需要安装一个容器运行时，比如 containerd。然后，可以运行以下命令在主机上初始化 Kubernetes 控制平面：
```bash
$ kubeadm init
```
初始化完成后，系统会告诉你运行下一步需要做什么：
```bash
$ mkdir -p $HOME/.kube
$ sudo cp -i /etc/kubernetes/admin.conf $HOME/.kube/config
$ sudo chown $(id -u):$(id -g) $HOME/.kube/config
```
然后，需要手动选择并安装一个 Pod 网络插件，这样 Pod 才能相互通信。Flannel 是一个受欢迎的选项：
```bash
$ kubectl apply -f https://raw.githubusercontent.com/flannel-io/flannel/master/Documentation/kube-flannel.yaml
```
经过所有这些步骤，就可以向集群中添加节点了：
```bash
kubeadm join --token <token> <control-plane-ip>:<control-plane-port> \
--discovery-token-ca-cert-hash sha256:<hash>
```
<token> 的值是通过在控制平面主机上运行 `kubeadm token list` 命令获取的。要找到正确的 discovery-token-ca-cert-hash，需要在控制平面主机上运行以下命令：
```bash
openssl x509 -pubkey -in /etc/kubernetes/pki/ca.crt | openssl rsa -pubin \
-outform der 2>/dev/null | openssl dgst -sha256 -hex | sed 's/^.* //'
```
使用 kubeadm 启动本地 Kubernetes 集群比使用 K3s 要复杂得多。K3s 抽象了集群设置步骤，使得快速启动和运行变得更加简单。在本地使用 K8s 需要额外的时间和精力来学习安装过程并配置环境。
<a name="vb9PY"></a>
## K3s 和 K8s 之间的主要区别
K8s 和 K3s 对用户提供了相同的功能接口。如果有一个 Kubernetes YAML 清单，可以在两个集群中使用任何一个进行部署，而无需进行修改。<br />这两个发行版的区别在于它们的打包方式和包含的组件。以下是一些应该考虑的关键特点：
<a name="hTFr7"></a>
### 默认安装的组件
K8s 和 K3s 打包了不同的组件来实现 Kubernetes 的架构。其中最大的变化之一就是控制平面使用的数据存储：上游 Kubernetes 使用 etcd，而 K3s 选择使用内置的 SQLite 数据库。这通常提高了性能并减小了二进制文件的大小，但可能不适用于大规模集群。如果需要，K3s 也可以连接到外部的 etcd 或使用 K3s 内置的 etcd 数据存储，以及其他基于 SQL 的数据库，如 MySQL 和 PostgreSQL。<br />标准的 Kubernetes 发行版只包括控制平面所需的组件。K3s 还内置了常用的生态工具，比如：kubectl。<br />K3s 集成了 Helm 支持，可以通过将 Helm Chart 表示为集群中的 HelmChart 对象来部署 Helm Chart。然而，上游 Kubernetes 不理解 Helm；需要单独安装 Helm CLI 并使用其命令来安装 Chart。<br />K8s 和 K3s 都使用 containerd 作为默认的容器运行时，但这可以进行定制。K3s 还包含了其他几个来自社区的组件，包括用于 Pod 网络的 flannel 以及作为入口控制器和内置负载均衡器的 Traefik。Kubernetes 让你自己选择和安装这些工具，而 K3s 则内置了常用的工具。<br />如果不想花太多的时间成本去学习 K8s 各个组件作用时，K3s 是更好的选择。它可以启动一个功能完整的集群，并且可用于生产。
<a name="Os5gW"></a>
### 资源要求
K3s 可以在 1C 512MB 的设备上运行，K3s 二进制文件大小不到 60 MB，并且不需要外部依赖。<br />使用 kubeadm 创建的集群具有更高的资源需求。文档建议至少有两个空闲的 CPU 核和 2 GB 的内存。控制平面组件的增加开销意味着需要更多的硬件资源才能达到相同的结果。这在云上部署集群时可能会增加成本。<br />K3s 是资源受限环境的优先选择。这是该项目的核心关注领域。请记住，虽然 K3s 可以运行在 512 MB 内存的设备上，但并不推荐这样，因为需要给应用程序预留运行空间。
<a name="WqtjC"></a>
### 升级
K3s 提供简化的集群升级体验。只需再次运行安装脚本即可下载最新版本并自动完成升级：
```bash
$ curl -sfL https://get.k3s.io | sh -
```
在每个节点上重复执行此命令将使集群升级到最新的稳定版本，无需任何手动干预。<br />对于使用 kubeadm 创建的 K8s 集群进行升级需要执行几个额外的步骤。需要获取最新版本的 kubeadm：
```bash
# Updating to v1.24.1
$ apt-get update
$ apt-get install -y kubeadm=1.24.1
```
接下来，使用 kubeadm 升级控制平面：
```bash
$ kubeadm upgrade apply v1.24.1
```
最后，升级每个工作节点上的 kubelet 和 kubectl：
```bash
$ apt-get update
$ apt-get install -y kubelet=1.24.1 kubectl=1.24.1
$ systemctl daemon-reload
$ systemctl restart kubelet
```
K3s 再次提供了更简单、无需干预的体验。Kubeadm 的升级同样也相对简单，但需要运行更多的命令。这增加了升级过程中出错的可能性。而使用 K3s，只需调用安装脚本并等待集群更新即可。
<a name="I5Glc"></a>
### 速度
在等效硬件上部署的 K8s 集群和 K3s 集群应该可以以相似的性能运行容器，因为它们使用相同的 containerd 运行时。然而，K3s 非常轻巧，它安装和启动控制平面的速度要比 K8s 快得多。<br />相比之下，上游的 Kubernetes 可能需要几分钟才能启动（而 K3s 通常在一分钟内可用）。这使得 K3s 更适合于临时的集群，例如本地开发和测试环境。可以快速启动一个集群，使用后再将 K3s 关闭。
<a name="kUoQT"></a>
### 安全
K3s 在设计上是安全的，并提供了一个最小的攻击面。所有组件都打包在一个二进制文件中，减少的依赖关系使得安全漏洞的出现可能性较小。<br />这并不意味着 K8s 是不安全的。Kubernetes 已成为最受欢迎的开源项目之一，被全球各大公司采用。它经过定期审查，以确保集群受到攻击的保护。<br />无论使用哪种解决方案，都应该在安装后加强集群的安全保护。K3s 和 **Kubernetes 都有自己的安全建议**，用于创建安全的集群。
<a name="WjrSO"></a>
## K3s 和 K8s 的理想使用场景
K3s 具有最低的硬件要求，适用于资源受限的环境，这些环境无法容纳标准的 K8s 集群。K3s 放弃了诸如 etcd 之类的组件，而选择了更小的替代方案，这意味着 K3s 可以适应 IoT 和边缘设备。<br />同时，K3s 也是在开发测试环境中运行本地 Kubernetes 集群的理想解决方案。工程师可以在几秒钟内快速启动自己的环境，而无需安装依赖项或在云中使用托管的 Kubernetes 服务产生成本。甚至可以在 CI 流水线脚本中运行 K3s，以简化测试流程。<br />虽然 K3s 适用于许多不同的环境，但在某些情况下，使用更大的 Kubernetes 发行版可能更合理。例如，在 大规模部署或需要使用依赖于 K8s 特定组件的场景时。如果在这个规模上运作，可能需要使用 kubeadm 或其他工具部署和维护 K8s。对于希望对各个控制平面组件具有完全控制权的大型集群，K3s 的简单性可能会过于局限。
<a name="Vpvfs"></a>
## 总结
Kubernetes 是部署和分发容器的领先编排工具。虽然它在推动容器进入生产技术方面发挥了重要作用，但纯 Kubernetes 仍然相当复杂且难以维护。<br />K3s 通过提供一个经过 CNCF 认证的 Kubernetes 发行版，将其打包为一个不到 60MB 的单个二进制文件，解决了这些挑战。它的轻量级方案让你可以在边缘、工作站和传统云环境中运行相同版本的 Kubernetes。
