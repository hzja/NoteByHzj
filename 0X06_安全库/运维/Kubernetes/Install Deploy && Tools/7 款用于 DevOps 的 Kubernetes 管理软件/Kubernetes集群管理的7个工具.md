Kubernetes
<a name="VxHCO"></a>
### K9s
k9s是基于终端的资源仪表板。它只有一个命令行界面。无论在Kubernetes仪表板Web UI上做什么，都可以在终端使用K9s仪表板工具进行相同的操作。k9s持续关注Kubernetes集群，并提供命令以使用集群上定义的资源。<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1597799098966-7d453e84-af8d-4ce7-b1ae-68ef06432f2f.webp#height=318&id=U28FN&originHeight=318&originWidth=640&originalType=binary&size=0&status=done&style=none&width=640)<br />K9s的功能包括集群的实时跟踪，使用K9s皮肤自定义视图，通过Kubernetes资源轻松遍历，向下通过选项以检查集群资源问题，提供扩展的插件来创建自己的命令。
<a name="EVySr"></a>
### Rancher
Rancher是开源容器管理平台，任何企业都可以轻松采用Kubernetes。可以部署和管理在谷歌云GKE，AWS EKS，Azure AKS中运行的托管Kubernetes集群，也可以仅在选择的虚拟机或裸机基础架构上部署Kubernetes。<br />Rancher简化了管理员的操作，主要功能包括监控集群的运行状况，设置警报和通知，启用集中式日志记录，定义和应用全局安全策略，建立身份验证并执行后台策略，管理和扩展基础架构等。<br />随着企业中Kubernetes的采用速度，Rancher让用户可以直接访问Kubernetes API和CLI。Rancher的新智能界面简化了应用程序管理;团队可以轻松地部署和管理工作负载，定义机密信息并管理私有注册表，配置持久卷声明，配置负载均衡和服务发现，以及管理CI管道等。
<a name="823M0"></a>
### Dashboard+Kubectl+Kubeadm
Kubernetes仪表盘基于Web的界面来部署容器应用。它可以对应用程序进行故障排除，并与资源一起管理集群本身。<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1597799098946-6fa7de92-b47e-4ccf-807a-a0f06615a18c.webp#height=360&id=pwjXp&originHeight=360&originWidth=640&originalType=binary&size=0&status=done&style=none&width=640)<br />可以使用仪表板来概述集群上运行的应用程序，以及创建或修改单个Kubernetes资源，例如部署作业，副本集等。可以扩展部署，也可以启动滚动更新，甚至可以使用仪表板上的部署向导重新启动pod或部署新应用程序。<br />Kubectl是用于与API服务进行通信，并将命令发送到主节点的命令行工具。它的隐式命令用于对Kubernetes集群API服务器的API调用。<br />Kubeadm是带有内置命令的工具，用于启动最小的Kubernetes集群。使用kubeadm，可以运行一些基本命令来引导集群，创建令牌以加入集群，还原对Kubernetes集群所做的更改等。
<a name="ezySF"></a>
### Helm
Helm是Kubernetes的软件包管理工具。它允许开发者和管理员在Kubernetes集群上打包，配置和部署应用程序和服务。它为管理员提供了对Kubernetes集群的更大控制权。<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1597799098985-9f6aca06-5954-426f-8862-cf3f94969972.webp#height=384&id=gAKD7&originHeight=384&originWidth=640&originalType=binary&size=0&status=done&style=none&width=640)<br />Helm的主要功能有使应用程序部署容易，标准化和可重用，通过Helm Charts轻松描述复杂的应用程序，提高开发人员生产力，降低部署复杂性，增强操作准备，加快采用云原生应用程序的速度，轻松回滚到以前的版本等。<br />Helm 帮助您管理 Kubernetes 应用程序。Helm Charts 帮助您定义、安装和升级最复杂的 Kubernetes 应用程序。<br />Helm 可以使用 Charts 启动 Kubernetes 集群。<br />Helm是统治之王，是打包，共享和部署k8s应用程序的事实上的标准。Helm是Kubernetes 的包管理工，它允许以一致且结构化的方式将多个YAML配置分组为一个称为“chart ”的逻辑软件包。<br />对于应用发布者而言，创建Helm Charts 可带来很多好处：

1. 可以使用模板或自定义参数，使应用程序可配置。
2. 可以通过版本化和可跟踪的方式将应用程序发布到私有或公共存储库中。
3. 可以使应用程序依赖于其他Helm chart，依次来管理应用依赖关系 。
4. 总体而言，Helm提供了强大的封装机制，以确保应用程序按预期部署。

对于使用者而言，安装 Helm chart 可以提供更多好处：

1. 可以访问公共chart 以及组织提供的任何私人chart 的存储库。
2. 可以基于应用发布者提供的设置来自定义应用程序。
3. 能够知道程序包的真实版本信息。
4. 不必担心Kubernetes规范的复杂性。
5. Helm支持原子升级或回滚操作，能够降低应用程序和集群的风险。
6. 总体而言，将获得一个易于理解并且可以立即使用的应用程序包。
<a name="HKT5c"></a>
#### Helm有什么新功能？
在2019年末，看到了Helm v3的发布，其中删除了集群侧组件（Tiller）和许多其他功能。到2020年底，已经不推荐使用Helm v2，大多数公共chart 已迁移到v3格式。如果仍在使用Helm v2，请尽快安排升级。<br />在最近的几个发行版中，Helm团队一直在忙于解决Helm用户最大的抱怨-使用YAML模板的困难。现在Helm包括一个功能强大的Lint命令，当调试YAML问题时，它应该是新向导。<br />Post Rendering功能是Helm中的另一个新功能，可以使用Kustomize等工具自定义Helm chart 。<br />2020年chart管理方式，开始向去中心化的的转变，并推出了两个用于共享chart的中央存储库：

1. CNCF的ArtifactHub
2. JFrog的ChartCenter
<a name="Uiykz"></a>
### KubeSpray
KubeSpray是一个集群生命周期管理器，可帮助部署可用于生产的Kubernetes集群。它使用ansible-playbook来自动化Kubernetes集群配置。<br />主要功能包括基于Ansible，高度可用，跨平台;流行的云提供商集成甚至是裸机，多种配置选项，多平台CI/CD等等。<br />默认情况下，Kubespray允许通过kube-master IP地址和端口6443远程连接到Kubernetes集群。Kubespray最适合于需要灵活部署的用户。它提供了许多自定义配置选项。另外，如果熟悉Ansible，那么Kubespray将会非常易于使用。
<a name="A7GSO"></a>
### Kontena Lens
Kontena Lens是Kubernetes的智能仪表板。<br />作为控制Kubernetes唯一需要的管理系统。它可免费用于Mac OS，Windows和Linux操作系统。应用程序启动后，将在界面中看到所有相关集群的列表。<br />对于确实需要每天处理Kubernetes的用户来说，它是功能最强大的IDE。可以确保正确设置和配置集群，并且可以更轻松、更快速地使用集群，并从根本上提高生产率和业务速度。<br />Kontena Lens IDE主要功能包括可以一次管理多个集群，实时可视化集群状态，提供内置终端;安装非常简单，因为它是独立的应用程序;支持Kubernetes RBAC。经过测试可处理集群中近25000个Pod<br />Kubernetes是一个复杂的工具，Lens IDE可以帮助初学者轻松地使用Kubernetes。它是用于管理和可视化Kubernetes集群的优秀工具之一。
<a name="S10Z2"></a>
### WKSctl
WKSctl代表Weave Kubernetes系统控制，它是Weave Kubernetes平台的一部分。<br />WKSctl是使用GitOps进行Kubernetes配置管理的工具。GitOps只是一组实践，这些实践使用git请求以传统方式管理应用程序和基础结构。<br />使用WKSctl，可以通过Git提交来管理Kubernetes集群。可以升级集群或从集群添加/删除节点。<br />可以在2种模式下运行它：独立模式和GitOps模式。在独立模式下，它将创建一个静态集群。在GitOps模式下，它将根据git上存在的cluster.yml和machines.yml信息配置集群。<br />WKSctl主要功能有使用git快速启动集群，部署失败时轻松回滚，记录更改以供审核，创建集群只需要IP地址和ssh密钥，持续验证和纠正集群状态。
<a name="dgOi3"></a>
### Kustomize
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1619237905956-558b3e22-7523-4948-9990-682bfa8b152e.png#clientId=uff4d36aa-ad3e-4&from=paste&id=u3fa6d7f6&originHeight=140&originWidth=140&originalType=url&status=done&style=none&taskId=u45ca8f0f-66a6-4589-a3a5-2c6da512f82)<br />Helm的最大缺点是，应用程序的定制仅限于预先存在的配置选项。不仅如此，chart创建者还必须以某种复杂的模板方式来实现这些自定义选项。好吧，这就是Kustomize的用武之地。<br />Kustomize 是一个独立的工具，用来通过 kustomization 文件 定制 Kubernetes 对象，它允许将Kubernetes应用程序构建为一系列的层和补丁，从而实现无限的自定义。通过使用 Kustomize ，也可以实现对 Kubernetes 对象进行声明式管理。<br />从 Kubernetes 1.14 版本开始，kubectl 也开始支持使用 kustomization 文件来管理 Kubernetes 对象。要查看包含 kustomization 文件的目录中的资源，执行下面的命令：<br />kubectl kustomize <kustomization_directory><br />要应用这些资源，使用参数 –kustomize 或 -k 标志来执行 kubectl apply：<br />kubectl apply -k <kustomization_directory><br />Kustomize是一个功能强大的工具，可以所需的任何方式修改Kubernetes应用程序。不幸的是，这意味着学习曲线可能非常陡峭。任意定制，还意味着错误配置应用程序的可能性增加。<br />Kustomize的一种高级用例，是使用了Helm的Post Rendering功能来完善现有的Helm chart功能 ，，从而有望使版本升级变得平滑。
<a name="uHQHO"></a>
#### Kustomize有什么新功能？
Kubernetes生态系统中的许多工具都嵌入了Kustomize，最突出的例子是编排和持续部署工具，例如 ArgoCD，Flux和Kubestack。<br />要知道的是, kubectl内的Kustomize版本缺少大量的增强功能，如果需要这些功能，请考虑使用Kustomize作为独立的CLI工具。
<a name="CRkpo"></a>
### Skaffold
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1619237906032-e63531f0-b46f-4bae-98bb-f0fe0789f6e8.png#clientId=uff4d36aa-ad3e-4&from=paste&id=u4e258554&originHeight=140&originWidth=140&originalType=url&status=done&style=shadow&taskId=u2b24287d-4f61-4869-9c30-08da8ef15c7)<br />Skaffold是干啥的？<br />Google的软件工程师Nick Kubala解释说：k8s的用户在构建和管理容器镜像方面花的时间太长了，需要手动更新k8s清单，即使进行很少的代码更新都要重新部署一次应用。于是就打算开发个工具来自动化完成这些任务，让用户可以专注于代码本身，而不是做这些重复性工作。<br />Skaffold 是一个命令行工具，有助于 Kubernetes 应用程序的持续开发。<br />Skaffold遵循DevOps最佳做法。使用Skaffold，可以方便的将本地代码部署到kubernetes上进行测试。通过 skaffold.yaml ，可以管理项目的构建（build）和部署（deploy）过程。<br />对于构建，Skaffold可以利用Dockerfiles，Buildpacks，Bazel甚至是自定义脚本。对于部署，Skaffold包含其有限的模板引擎，并且可以调用kubectl，Helm或Kustomize。<br />Skaffold具有三种主要的操作模式：

1. skaffold dev-变更代码自动部署应用。可以随时监测代码更改，然后随时rebuild和redeploy到用户的集群里。
2. skaffold build-这将构建应用程序，并将其推送到选择的存储库中。
3. skaffold deploy-这会将构建的应用程序部署到选择的集群中，可能会利用Helm或Kustomize来实现。如果要使用单个命令进行构建和部署，则可以使用skaffold run。
<a name="k5Wf4"></a>
#### Skaffold有什么新功能？
2020年，Skaffold团队致力于使该项目更易于适应各种工作流程，并与其他工具实现更高的互操作性。其中的一些改进包括与CI/CD和GitOps更灵活的集成，以及对Python和Java的更好支持。<br />此外，skaffold debug可以对应用程序进行远程调试。尽管这是一个很好的功能，但是在微服务环境中使用它是比较棘手的，强烈建议使用Rookout工具。
