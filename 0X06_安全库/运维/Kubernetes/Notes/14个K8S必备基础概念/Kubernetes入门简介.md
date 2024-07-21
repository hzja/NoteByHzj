Kubernetes
<a name="LZtHT"></a>
## 概述
在JAVA开发中使用 `docker run`命令配合上自建的Docker仓库可以很容易部署JAVA服务，但是使用Docker部署应用会有几个问题：

1. 一个`docker run` 不是部署服务的可靠方法，因为它创建的容器在单个机器运行。虽然Docker引擎提供了一些基本的管理功能，例如在容器崩溃或计算器重启时自动重启容器。但是它不能处理机器崩溃。无法保证服务的高可用！
2. 另一个问题是服务通常不是孤立存在，而是相互依赖的，例如数据库和消息队列。通常需要将服务及其依赖项作为一个单元部署或取消部署。

在开发过程中特别好用的方法是使用Docker Compose。Docker Compose是一个工具，它允许使用YAML文件以声明方式定义一组容器，然后以组的形式启动和停止这些容器。<br />但是使用Docker Compose也有个很明显的问题就是它仅限于一台机器。要可靠的部署服务，必须使用Docker编排框架，例如Kubernetes。
<a name="x7RGx"></a>
## Kubernetes简介
Kubernates是一个Docker编排框架，是Docker之上的一个软件层，它将一组计算机硬件资源转变成用于运行服务的单一资源池。它努力保持每个服务所需要的实例数量，并确保它们一直在线，即使服务实例或机器崩溃也是如此。容器的灵活性和Kubernetes的复杂性相结合是部署服务的一种强有力的方式。<br />Kubernetes有三个主要功能：

- 资源管理：将一组计算机视为由CPU、内存和存储卷构成的资源池，将计算机集群视为一台计算机。
- 调度：选择要运行容器的机器。默认情况下，调度考虑容器的资源需求和每个节点的可用资源。它还可以实现在同一节点部署具有亲和性（affinity）的容器，或保持特定几个容器分散部署在不同的节点上（反亲和性，anti-affinity）
- 服务管理：实现命名和版本化服务的概念，这个概念可以直接映射到微服务架构中的具体服务。编排框架确保始终运行所需数量的正常实例。它实现请求的负载均衡。编排框架也可以执行服务的滚动升级，并允许回滚到旧版本。
<a name="PsSrH"></a>
## Kubernetes架构
![Kubernetes架构](https://cdn.nlark.com/yuque/0/2023/png/396745/1695260025884-59b06a14-29e0-4e4e-b9b4-6439d0e80dbf.png#averageHue=%23fbfbfb&clientId=uf8631b25-4020-4&from=paste&id=ub25601de&originHeight=765&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u528a09a0-222c-4d0f-a06f-1bc7c794911&title=Kubernetes%E6%9E%B6%E6%9E%84 "Kubernetes架构")<br />Kubernetes在一组机器上运行。Kubernetes集群中的计算机角色分为主节点和普通节点。集群中只有很少的几个主节点（可能只有一个）和很多普通节点。<br />**主节点**负责管理集群。Kubernetes的**普通节点**称为 “工作节点”，它会运行一个或多个Pod。Pod是Kubernetes的部署单元，由一组容器组成。<br />主节点运行多个组件，包括以下内容：

- **API服务器**：用于部署和管理服务的REST API，例如，可被kubectl命令行使用。
- **Etcd**：存储集群数据键值的NoSQL数据库。
- **调度器**：选择要运行POD的节点。
- **控制器管理器**：运行控制器，确保集群状态与预期状态匹配。例如，一种被称为 **复制（replication）控制器** 的控制器通过启动和终止实例来确保运行所需要的服务实例。

普通节点运行多个组件，包括以下内容：

- **Kubelet**：创建和管理节点上运行的Pod。
- **Kube-proxy**：管理网络，包括跨Pod的负载均衡。
- **Pods**：应用程序服务。

接下来看一下Kubernetes上部署服务需要掌握的关键Kubernetes概念，掌握这几个概念就抓住了Kubernetes的核心。
<a name="tY6tK"></a>
## Kubernetes的关键概念
Kubernetes是很复杂的，但是，一旦掌握了一些 **关键对象 **的概念，就可以高效的使用Kubernetes。Kubernetes定义了许多类型的对象，从开发人员的角度来看，最重要的对象如下：

- **Pod**：Pod是Kubernetes的基本部署单元。它由一个或多个共享IP地址和存储卷的容器组成。服务实例的pod通常由单个容器组成，例如运行 JVM 的容器。但在某些情况下，Pod包含一个或多个实现支持功能的 **「边车」**（sidecar)容器。例如，Nginx 服务器可以有一个边车容器，定期执行 git pull 以下载最新版本的网站。Pod的生命周期很短，因为Pod的容器或它运行的节点可能会崩溃。
- **Deployment**：Deployment : Pod 的声明性规范。Deployment是一个控制器，可确保始终运行所需数量的Pod实例 (服务实例)。它通过滚动升级和回滚来支持版本控制。
- **Service**：向应用程序服务的客户端提供的一个静态/稳定的网络地址。它是基础设施提供的服务发现的一种形式。每个 Service具有一个 IP 地址和一个可解析为该 IP 地址的 DNS 名称，并跨一个或多个 Pod对 TCP 和 UDP 流量进行负载均衡处理。IP地址和 DNS 名称只能在Kubernetes内部访问。Service默认是使用ClusterIp模式，如果需要外部能访问到这个Service则需要使用另外两种类型的对象：NodePort 和 LoadBalancer。
- **ConfigMap**：名称与值对的命名集合，用于定义一个或多个应用程序服务的外部化配置。Pod容器的定义可以引用ConfigMap来定义容器的环境变量。它还可以使用ConfigMap在容器内创建配置文件。可以使用Secret来存储敏感信息（如密码)，它也是 ConfigMap的一种形式。
