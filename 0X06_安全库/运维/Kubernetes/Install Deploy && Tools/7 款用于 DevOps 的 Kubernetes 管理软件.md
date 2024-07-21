Kubernetes
<a name="FLi6j"></a>
## 1、K9s
k9s是一个基于终端的资源仪表板。它只有一个命令行界面。无论在 Kubernetes 仪表板 Web UI 上做什么，也可以使用此终端 k9s 仪表板实用程序执行相同操作。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643416199784-12976786-f82c-41bf-8876-eab418394d8f.webp#clientId=ua64a6e67-7026-4&from=paste&id=u57f7ded5&originHeight=480&originWidth=1032&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u28014d91-bbac-4b49-9fd3-54421769d09&title=)<br />它持续关注 Kubernetes 集群，并提供命令来处理集群上定义的资源。<br />以下是 K9s 的功能：

- 集群实时跟踪
- 使用 K9s 皮肤自定义视图
- 轻松遍历 Kubernetes 资源
- 向下钻取选项以检查集群资源问题
- 提供扩展插件来创建自己的命令
<a name="sl1BS"></a>
## 2、Rancher
Rancher是一个开源容器管理平台，可让任何企业轻松采用 Kubernetes。可以部署和管理在 GKE (GCP)、EKS (AWS)、AKS (Azure) 中运行的云托管 Kubernetes 集群，也可以仅在选择的 VM 或裸机基础设施上部署 Kubernetes。<br />Rancher 简化了管理员的所有操作职责，包括：

- 监控集群的运行状况
- 设置警报和通知
- 启用集中日志记录
- 定义和应用全局安全策略
- 建立身份验证并执行后台策略
- 管理和扩展基础架构

随着 Kubernetes 在整个公司的采用加速，rancher 鼓励快速采用让用户直接访问 Kubernetes API 和 CLI。Rancher 的全新智能界面简化了应用管理；团队可以轻松部署和管理工作负载、定义 Secret 并管理私有注册表、配置持久卷声明、配置负载平衡和服务发现、管理 CI 管道。
<a name="FQYCj"></a>
## 3、Dashboard + Kubectl + Kubeadm
该Kubernetes仪表盘是一个基于Web的界面来部署集装箱式应用。它对应用程序进行故障排除并管理集群本身以及资源。<br />可以使用仪表板概览集群上运行的应用程序，以及创建或修改单个 Kubernetes 资源，例如部署作业、副本集等。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643416199753-32bd27b1-64e3-4e92-9d4e-0ae60f953c26.webp#clientId=ua64a6e67-7026-4&from=paste&id=u6376097f&originHeight=608&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u12ed73bc-f012-4451-be97-370647bdd47&title=)<br />可以扩展部署，也可以启动滚动更新，甚至可以使用仪表板上的部署向导重新启动 Pod 或部署新应用程序。<br />Kubectl是一个命令行工具，用于与 API 服务通信并向主节点发送命令。它对 Kubernetes 集群 API 服务器的 API 调用的隐蔽命令。<br />Kubeadm是一个带有内置命令的工具，用于启动最小的 Kubernetes 集群。它用于引导集群而不是配置机器。使用 kubeadm，可以运行一些基本命令来引导集群、创建令牌以加入集群、还原对 Kubernetes 集群所做的更改等。
<a name="Sy8bg"></a>
## 4、Helm
Helm是 Kubernetes 的包管理器。它允许开发人员和运营商在 Kubernetes 集群上打包、配置和部署应用程序和服务。它为操作员提供了对 Kubernetes 集群的更大控制，这些操作：

- 使应用程序部署变得简单、标准化和可重用
- 通过掌舵图轻松描述复杂的应用程序
- 提高开发人员的生产力
- 降低部署复杂性
- 增强操作准备
- 加快云原生应用的采用
- 轻松回滚到以前的版本

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643416200102-1fa7abfd-2dd0-46fb-bcf2-f63c6e34dc99.webp#clientId=ua64a6e67-7026-4&from=paste&id=uf8580a77&originHeight=376&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u365ffc16-82ab-44c9-8e46-3cdd1761791&title=)<br />Helm 使用包含所有资源定义的 Charts 在 Kubernetes 集群上运行应用程序或服务。可以在此处找到可供使用的多个Helm图表。
<a name="utIkN"></a>
## 5、KubeSpray
KubeSpray是一个集群生命周期管理器，可部署生产就绪的 Kubernetes 集群。它使用ansible-playbook来自动化 Kubernetes 集群配置。<br />其中一些功能包括：

- 基于 Ansible
- 高可用
- 跨平台
- 生产水平
- 流行的云提供商集成甚至裸机
- 多种配置选项
- 多平台 CI/CD
- 默认安全

默认情况下，Kubespray 允许通过 kube-master IP 地址和端口 6443 远程连接到 Kubernetes 集群。如果需要灵活的部署，Kubespray 最适合；它提供了很多自定义配置选项。<br />另外，如果熟悉Ansible，那么 Kubespray 非常易于使用。
<a name="KeZQT"></a>
## 6、Kontena Lens
Kontena Lens是Kubernetes的智能仪表板。<br />它是控制 Kubernetes 所需的唯一管理系统。它可免费用于 Mac OS、Windows 和 Linux 操作系统。镜头应用程序启动后，将在界面中看到所有关联集群的列表。<br />对于真正需要每天处理 Kubernetes 的人来说，它是最强大的 IDE。可以确保正确设置和配置集群，并且可以更轻松、更快速地使用集群，从根本上提高生产力和业务发展速度。<br />Kontena Lens IDE 的特点是：

- 可以一次管理多个集群
- 实时可视化集群状态
- 提供内置终端
- 安装非常简单，因为它是一个独立的应用程序
- 惊人的用户界面和用户体验
- 支持 Kubernetes RBAC。
- 经测试可处理集群中近 25K 的 Pod

Kubernetes 是一个复杂的工具，Lens IDE 甚至可以帮助初学者轻松上手 Kubernetes。它是管理和可视化 Kubernetes 集群的最佳工具之一。
<a name="XEwIQ"></a>
## 7、WKSctl
WKSctl代表 Weave Kubernetes 系统控制。它是 Weave Kubernetes 平台的一部分。<br />WKSctl 是一个使用GitOps进行 Kubernetes 配置管理的工具。GitOps 只不过是一组使用 git 请求以传统方式管理应用程序和基础设施的实践。<br />使用 WKSctl，可以通过 Git 提交来管理 Kubernetes 集群。可以升级集群或从集群中添加/删除节点。<br />可以在两种模式下运行此工具：独立模式和 GitOps 模式。在独立模式下，它会创建一个静态集群。在 GitOps 模式下，它根据 git 上存在的 cluster.yml 和machines.yml 信息配置集群。<br />WKSctl 特点：

- 使用 git 快速启动集群
- 部署失败时轻松回滚
- 记录变更以供审查和审计
- 创建集群只需要 IP 地址和 ssh 密钥
- 不断验证和纠正集群状态
