Kubernetes<br />云计算主要是在 Kubernetes 上运行，Kubernetes 主要是在 Linux 上运行，而 Linux 在有熟练的系统管理员控制时运行得最好。现代互联网都需要了解如何在容器中创建应用和服务，按需扩展，按需扩展以及如何明智地进行监视和管理。
<a name="IE8Ne"></a>
### 安装 kubectl
kubectl 命令允许在 Kubernetes 集群上运行命令。使用 kubectl 来部署应用、查看日志、检查和管理集群资源，并在出现问题时进行故障排除。kubectl（以及整个 Kubernetes）的典型“问题”是，要对集群运行命令，首先需要一个集群。然而，有一些简单的解决方案。<br />首先，可以创建自己的 Kubernetes 集群，只需买三块树莓派板和相关外围设备（主要是电源）。当获得了硬件，阅读 Chris Collins 的 使用树莓派构建 Kubernetes 集群，就会拥有自己的安装有 kubectl 的集群。<br />另一种获得集群的方法是使用 Minikube，这是一个 Kubernetes 的实践环境。在所有建立和运行集群的方法中，这是最简单的。<br />还有更多的选择；例如，可以参加一个关于 Kubernetes 的课程，以获得一个运行集群的实验室，或者可以在云上购买时间。只要有一个 Kubernetes 环境来练习，如何获得集群并不重要。<br />能访问一个集群，就可以开始探索 kubectl 命令。
<a name="AJibc"></a>
### 了解吊舱和容器
容器是一个轻量级的、部分的 Linux 系统，专门用于运行一个应用或服务。容器受到 内核命名空间 的限制，这使它能够访问其主机（运行容器的计算机）上的重要系统组件，同时防止它向其主机发送数据。容器以容器镜像（或简称 镜像）的形式保存，并由称为 Containerfile 或 Dockerfile 的文本文件定义。<br />吊舱(Pod)是容器的正式集合，也是管理员扩展、监控和维护任何数量的容器的一种简单方法。<br />这些一起就像 Kubernetes 的“应用程序”。创建或获取容器镜像是在云上运行服务的方式。
<a name="ajeTa"></a>
### 运行一个吊舱
容器镜像的两个可靠的仓库是 Docker Hub 和 Quay。可以在仓库中搜索可用的镜像列表。通常有由项目提供的大型项目的官方镜像，也有专门的、定制的或特殊项目的社区镜像。最简单和最小的镜像之一是 BusyBox 容器，它提供了一个最小的 shell 环境和一些常用命令。<br />无论是从仓库中拉取镜像，还是自己编写镜像定义并从 Git 仓库中拉取到集群中，其工作流程都是一样的。在 Kubernetes 中启动一个吊舱时：

1. 在 Docker Hub 或 Quay 上找到一个想使用的镜像
2. 拉取镜像
3. 创建一个吊舱
4. 部署吊舱

以 BusyBox 容器为例子，可以用一条命令完成最后三个步骤：
```bash
$ kubectl create deployment my-busybox --image=busybox
```
等待 kubectl 完成这个过程，最后就有了一个正在运行的 BusyBox 实例。这个吊舱并没有暴露给其他人。它只是在后台安静地在集群上运行。<br />要看集群上有哪些吊舱在运行：
```bash
$ kubectl get pods --all-namespaces
```
也可以获得关于吊舱部署的信息：
```bash
$ kubectl describe deployment my-busybox
```
<a name="QdYw4"></a>
### 与吊舱互动
容器通常包含使其自动化的配置文件。例如，将 Nginx httpd 服务器作为容器安装，应该不需要互动。开始运行容器，它就会工作。对于添加到吊舱中的第一个容器和之后的每个容器都是如此。<br />Kubernetes 模型的优点之一是，可以根据需要扩展服务。如果网络服务被意外的流量淹没，可以在云中启动一个相同的容器（使用 scale 或 autoscale 子命令），使服务处理传入请求的能力增加一倍。<br />即便如此，有时还是很高兴看到一些证明吊舱正在按预期运行的证据，或者能够对似乎无法正常运行的某些问题进行故障排除。为此，可以在一个容器中运行任意的命令：
```bash
$ kubectl exec my-busybox -- echo "hello cloud"
```
另外，可以在容器中打开一个 shell，用管道将标准输入输入到其中，并将其输出到终端的标准输出：
```bash
$ kubectl exec --stdin --tty my-busybox -- /bin/sh
```
<a name="MAP5G"></a>
### 暴露服务
默认情况下，吊舱在创建时不会暴露给外界，这样就有时间在上线前进行测试和验证。假设想把 Nginx Web 服务器作为一个吊舱安装和部署在集群上，并使其可以访问。与任何服务一样，必须将吊舱指向服务器上的一个端口。kubectl 子命令 expose 可以做到这点：
```bash
$ kubectl create deployment \  my-nginx --image=nginx$ kubectl expose deployment \  my-nginx --type=LoadBalancer --port=8080
```
只要集群可以从互联网上访问，就可以通过打开浏览器并导航到公共 IP 地址来测试新 Web 服务器的可访问性。
<a name="DKJ4z"></a>
### 不仅仅是吊舱
Kubernetes 提供了很多东西，而不仅仅是存储普通服务的镜像。除了作为一个 容器编排 系统，它还是一个云开发的平台。可以编写和部署应用，管理和监控性能和流量，实施智能负载平衡策略等。<br />Kubernetes 是一个强大的系统，它已经迅速成为各种云的基础，最主要的是 开放混合云。
