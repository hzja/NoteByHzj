Kubernetes<br />随着 Kubernetes 的普及，其复杂性也在增加，强大的开源社区和云原生生态圈，围绕集群管理、开发、测试、安全等提供大量工具和服务。<br />重点介绍基于命令行的工具和插件，进行集群管理、可观察性、常规日常运维。<br />这些工具和插件必须：

- 开源
- 基于命令行
- 易于安装
- 为 Kubernetes 工作流程带来切实的好处
<a name="BKSHK"></a>
## [Kubectl krew 插件管理器](https://krew.sigs.k8s.io/)
Krew 是 kubectl 命令行工具的插件管理器，可以使用 git/curl 安装在Win、Linux、MacOS上安装使用，具体[Krew安装步骤](https://krew.sigs.k8s.io/docs/user-guide/quickstart/)<br />Krew 与 kubectl CLI 集成并提供了机制，来安装额外命令行工具作为插件。目前，已有[174 个插件](https://krew.sigs.k8s.io/plugins/)可用。<br />为了充分利用 krew 插件管理器，需要查看[插件说明文档](https://krew.sigs.k8s.io/plugins/)，知道哪些插件是有用的。<br />这是经常使用的插件列表，以供参考：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640689673805-f2f84e2d-3340-440b-9b25-ede5c5e8721f.webp#clientId=ua5531eb9-7d8f-4&from=paste&id=u13795526&originHeight=407&originWidth=269&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua0160363-7d30-476d-863e-8d46d00c941&title=)<br />Krew 提供了一个用于发现和管理插件的一站式商店，使得添加新插件，来改进 Kubernetes 工作流程变得更加方便。
<a name="M77RS"></a>
## [Kubectx和Kubens](https://github.com/ahmetb/kubectx)
这实际上是两个独立的工具，但它们经常在一个工作流程中使用。<br />kubectx仓库<br />![](https://cdn.nlark.com/yuque/0/2021/gif/396745/1640689673951-32ef5aee-4b58-4220-b666-7aa087695119.gif#clientId=ua5531eb9-7d8f-4&from=paste&id=ud7b0836b&originHeight=372&originWidth=1077&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u12bb5642-85f4-4d68-9b67-8b61ad598d9&title=)<br />Kubens<br />![](https://cdn.nlark.com/yuque/0/2021/gif/396745/1640689673850-e145909b-a2ae-4d35-915f-4da7a54772a1.gif#clientId=ua5531eb9-7d8f-4&from=paste&id=udd01f9a5&originHeight=444&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u30459819-68f5-4b92-90c5-a245585def0&title=)<br />安装：
```bash
kubectl krew install ctx
kubectl krew install ns
```
这两个插件都是用 Go 编写的，并修改kubeconfig切换到新的上下文或命名空间。<br />为了获得最佳体验，请在[交互模式下](https://github.com/ahmetb/kubectx#interactive-mode)使用这两个插件。此模式需要[安装 fzf](https://github.com/junegunn/fzf)并启用使用光标而不是复制、粘贴来选择上下文。<br />以下是配合使用示例：<br />![](https://cdn.nlark.com/yuque/0/2021/gif/396745/1640689673862-59dc2883-9440-44f0-be25-dbb34ea207ce.gif#clientId=ua5531eb9-7d8f-4&from=paste&id=ub3aa97a1&originHeight=461&originWidth=355&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=udd77b79d-0d6d-4729-8e20-28dd2c1a2da&title=)<br />这些工具的最大好处是不必输入命令。提高效率，必不可少！
<a name="IS6y1"></a>
## [Kube-ps1仓库](https://github.com/jonmosco/kube-ps1)
这个插件与 kubectx 和 kubens 结合使用效果很好。一旦切换到不同的上下文/命名空间，怎么知道在哪里？kube-ps1可以做到这一点，此插件在 Windows 上不可用。<br />![](https://cdn.nlark.com/yuque/0/2021/gif/396745/1640689673821-c447eb3d-48eb-408d-85c1-d098e7e4b9aa.gif#clientId=ua5531eb9-7d8f-4&from=paste&id=ud351f6a2&originHeight=677&originWidth=1079&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud601db86-3777-40a2-80d5-75ce957dc4f&title=)<br />这个脚本允许将Kubernetes上下文和kubectl上配置的命名空间添加到Bash/Zsh提示字符串中。Kube-ps1 可以通过`kubeoff`命令关闭、使用`kubeon`开启。<br />使用 kube-ps1，可以避免混淆导致的错误，因为始终知道所处的上下文和命名空间。
<a name="aOzyR"></a>
## [使用kail流式传输日志]()
流式传输所有日志或基于查询条件流式传输日志比较困难，Kali解决了这个问题。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640689674296-374afce8-63be-48d6-88b6-7948aa966240.webp#clientId=ua5531eb9-7d8f-4&from=paste&id=u5ce4cdb8&originHeight=698&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua6fa8bcc-bf28-46f7-930d-397ff8d7989&title=)<br />安装：
```bash
# mac
$ brew tap boz/repo
$ brew install boz/repo/kail
# linux
$ bash <( curl -sfL https://raw.githubusercontent.com/boz/kail/master/godownloader.sh) -b "$GOPATH/bin"
```
Kail 可以根据不同的匹配条件流式传输 Pod 的日志，例如，Service选择的所有 Pod 或匹配任意标签的 Pod 集合。<br />最好打开两个Tab，下面是示例：<br />![](https://cdn.nlark.com/yuque/0/2021/gif/396745/1640689674454-683eb4bc-2e87-40d8-b581-7cc2e92415a0.gif#clientId=ua5531eb9-7d8f-4&from=paste&id=u9f79fbdc&originHeight=487&originWidth=1079&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u089be647-4bec-4004-8553-95b0c603aeb&title=)<br />使用 kail，根据您的要求，流式传输日志和查看实时日志容易得多。
<a name="MQ9ro"></a>
## [K9s](https://github.com/derailed/k9s)
K9s 提供了一个终端UI来与Kubernetes集群交互。这个项目的目的是让您的应用程序更容易观察和管理。K9s不断地监视Kubernetes的变化，并提供后续命令来与观察到的资源交互。<br />K9s 可以运行在所有主流操作系统之上。它还支持其他更高级的功能，如端口转发、查看 Kubernetes 对象的 YAML 等等。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640689674644-4161638d-bf6c-4fcc-906d-79853ae99e06.webp#clientId=ua5531eb9-7d8f-4&from=paste&id=udc256c5c&originHeight=514&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4f6c9738-51bc-4b24-9946-c72dd482006&title=)

---

以下是涉及到上面提到的所有工具和插件的工作流程。在 Windows 10 上使用 WSL2（Windows Subsystem for Linux 2），使用键盘快捷键将终端窗口拆分为3个窗格。使用 kail 在右下窗格中显示创建的 nginx pod 的日志。一段时间后，使用 k9s 将本地端口 8080 转发到 nginx 容器端口 80，并使用左下面板中的 curl 与其交互，同时 kail 显示日志。<br />![](https://cdn.nlark.com/yuque/0/2021/gif/396745/1640689674672-85dd92a0-1313-4e2c-af00-3c62ad889930.gif#clientId=ua5531eb9-7d8f-4&from=paste&id=uccd33514&originHeight=368&originWidth=809&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0a6f0652-5604-4787-8bca-819272f8d9f&title=)
<a name="hPlNQ"></a>
## 结论
通过仅使用 5 个插件和工具明显改进原生 Kubernetes 工作流程。可以在本地机器上轻松安装这些工具，也可以使用 Docker 将这些工具打包成镜像，随用随拉。
<a name="NtycH"></a>
## 参考资料
Kubectl krew 插件管理器: [https://krew.sigs.k8s.io/](https://krew.sigs.k8s.io/)<br />Krew安装步骤: [https://krew.sigs.k8s.io/docs/user-guide/quickstart/](https://krew.sigs.k8s.io/docs/user-guide/quickstart/)<br />174 个插件: [https://krew.sigs.k8s.io/plugins/](https://krew.sigs.k8s.io/plugins/)<br />插件说明文档: [https://krew.sigs.k8s.io/plugins/](https://krew.sigs.k8s.io/plugins/)<br />Kubectx和Kubens: [https://github.com/ahmetb/kubectx](https://github.com/ahmetb/kubectx)<br />交互模式下: [https://github.com/ahmetb/kubectx#interactive-mode](https://github.com/ahmetb/kubectx#interactive-mode)<br />安装 fzf: [https://github.com/junegunn/fzf](https://github.com/junegunn/fzf)<br />Kube-ps1仓库: [https://github.com/jonmosco/kube-ps1](https://github.com/jonmosco/kube-ps1)<br />使用kail流式传输日志: [https://github.com/boz/kail](https://github.com/boz/kail)<br />K9s: [https://github.com/derailed/k9s](https://github.com/derailed/k9s)
