Kubernetes
<a name="ws2Do"></a>
## Docker Desktop可以做什么
Docker Desktop 可实现很多功能。以下列出一部分：

- 创建和管理 Docker 镜像
- 使用和发布来自 Container Registry 的镜像
- 在本地 Kubernetes 集群上测试 Kubernetes 应用程序
- 在不需要时关闭 Docker 服务器的能力
<a name="KcMk1"></a>
## Docker 和 OCI
[**开放容器计划**](https://opencontainers.org/) (OCI) 是一个开放的治理结构，其明确目的是围绕容器格式和运行时创建开放的行业标准。<br />当谈论容器时，实际上是在创建符合 OCI 的镜像。Docker 就是这样一个项目，但还有其他项目也可以做到这一点。Docker 有时与 OCI 兼容镜像交替使用。
<a name="HX7IW"></a>
## [containerd](https://containerd.io/)
在 [**Docker 的 7 种替代方案**](https://jfrog.com/knowledge-base/the-basics-7-alternatives-to-docker-all-in-one-solutions-and-standalone-container-tools/) 中，containerd 目前很流行，而且新版k8s原生支持。<br />这些项目中的大多数都在 Linux 和 Windows 上的裸机上运行（通过 Linux 的 Windows 子系统），但在 macOS 上，您很可能正在运行 Linux VM，尽管对您隐藏。像 Docker Desktop 这样的应用程序只是隐藏了这一点，以创建更无缝的体验、转发端口、帮助挂载文件系统等。<br />因此，在研究了 macOS 上 Docker 桌面的替代方案后，会发现很多项目，主要用于帮助创建符合 OCI 标准的镜像，并在本地Kubernetes集群中使用它们和一个易使用 Image Registry 的项目。如果它与本地端口很好地集成，会更好。

---

**如果是容器开发者，并且很习惯使用CLI(命令行接口)。下面分享几个值得关注的项目，来管理容器、k8s 的工具。**
<a name="pZJYd"></a>
## [rancher desktop](https://rancherdesktop.io/)
Rancher Desktop 是适用于 Mac、Windows 和 Linux 的开源桌面应用程序。它提供了 Kubernetes 和容器管理。<br />还可以选择要运行的 Kubernetes 版本，并且镜像 Registry 直接连接到该集群。
<a name="sKZgU"></a>
## [microK8S](https://microk8s.io/)
MicroK8S 是 Canonical,Ltd的一个项目，它也维护着 Ubuntu。<br />MicroK8S 可以帮助您创建跨多个节点的集群，如果您有本地云，则它是在本地云（On-Premise Cloud）中运行 Kubernetes 的绝佳选择。
<a name="XUQF3"></a>
## [colima](https://github.com/abiosoft/colima)
MacOS、Linux上的容器运行时，只需最少的设置。设置非常简单，它支持 docker 或 containerd 作为其容器运行时。<br />对本地Kubernetes集群有很好的支持，通过containerd创建的镜像可以直接在集群中使用。它还支持从 Linux VM 转发到 mac 主机的端口，并将文件系统安装到容器中。<br />**Colima似乎是 Docker Desktop 的完美开源替代品，但没有GUI。**<br />![](https://cdn.nlark.com/yuque/0/2021/gif/396745/1640574548232-8b9b70a6-75de-484c-b9c7-f046208c89f7.gif#clientId=u0e741410-1ff6-4&from=paste&id=ucecf5b54&originHeight=449&originWidth=721&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9da3426b-074b-4d2c-9234-1bb71de3abf&title=)
<a name="S6Vdi"></a>
## 参考资料
开放容器计划: [https://opencontainers.org/](https://opencontainers.org/)<br />containerd: [https://containerd.io/](https://containerd.io/)<br />Docker 的 7 种替代方案: [https://jfrog.com/knowledge-base/the-basics-7-alternatives-to-docker-all-in-one-solutions-and-standalone-container-tools/](https://jfrog.com/knowledge-base/the-basics-7-alternatives-to-docker-all-in-one-solutions-and-standalone-container-tools/)<br />rancher desktop: [https://rancherdesktop.io/](https://rancherdesktop.io/)<br />microK8S: [https://microk8s.io/](https://microk8s.io/)<br />colima: [https://github.com/abiosoft/colima](https://github.com/abiosoft/colima)
