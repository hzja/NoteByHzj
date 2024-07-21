Kubernetes<br />Kubernetes 可用于导出指标、日志和事件以实现可观察性。事件是了解服务中正在发生的事情的丰富信息来源，并且可以使用多种工具来充分利用它们。<br />以下是将要解释的内容的概述：

- 事件机制
- Kubernetes API 中的事件结构
- 需要关注的事件类型
- 检索事件的可用解决方案

最后会链接到 YouTube 和 Github 上的相关教程，这样就可以直接学习如何收集和检索 Kubernetes 事件。
<a name="YYll0"></a>
## Kubernetes 事件简介
Kubernetes 会生成许多与工作负载部署、调度等相关的事件。这是一个非常丰富的信息来源，可以了解集群中正在发生的事情，即回答诸如“为什么这个特定的 pod 被杀死或重新启动？”之类的问题。<br />有两种方法可以查看 K8s 中的事件：

- `kubectl describe pod`
- `kubectl get events`

当应用程序出现问题时，首先应该查看的是它的事件和它的基础设施操作。将事件保留更长的时间也很有用，因为它们可以用于事后分析或了解故障是否是由早期事件引起的。<br />Kubernetes 中有多种类型的事件，因为每个 Kubernetes 对象都会经历几种状态，直到达到所需的状态。<br />主节点和工作节点有几个核心组件，它们允许 K8s 在“服务器”上编排工作负载。调度器在节点上调度 Pod，controller manager 检测状态变化以在 Pod 消失的情况下重建 Pod，而 etcd 将存储各种 K8s 资源的状态（但仅限于最后一小时）。<br />所有的这些核心组件都能够根据事件编排工作负载。这意味着事件对于理解特定情况很重要。<br />**看一个简单的例子：**<br />部署 pod 时，调度程序会尝试识别正确的节点来启动 pod。同时，pod 将处于**pending 状态**。一旦调度程序确定了正确的节点，pod 将处于**creating 状态**。<br />要启动这个 pod，首先需要拉取容器的镜像。实际上，节点会从外部 docker 注册表中拉取镜像。调度程序还更倾向在已经拥有镜像的节点上调度 pod。<br />拉取镜像后，Pod 将处于**running 状态**。<br />如果由于某种原因，pod 消失了，controller manager 将重新创建该 pod。<br />但是如果 Pod 已经多次重启并出现相同的错误，Pod 将进入状态**CrashLoopBackOff**。<br />如果 Pod 卡在 pending 状态，则可能意味着节点上没有可用资源，或者无法找到正确的节点。<br />Pod 通常有存活探针或就绪探针来帮助 K8s 确定您的 pod 的状态或健康状况，即 /health 或 /ready。Kubelet 会调用这些探针。<br />还可以使用特定的镜像定义一个 init 容器，以便 K8s 先执行完成该 init 容器，然后运行其他容器。<br />如果您在部署文件中提供了错误的镜像，或者 docker 注册表存在连接问题，则节点无法拉取镜像，因此 Pod 将永远不会达到 running 状态。如果执行 describe 会看到**ImagePullBackOff**事件
<a name="cC5Up"></a>
### Kubernetes API 中的事件
所有事件都可以在 Kubernetes API（也可以使用 kubectl）的帮助下检索。通常，经常使用“ `_kubectl_ describe`”来收集状态、原因等。<br />与 API 交互时，您将收集：

- message
- reason
- type
- 事件中涉及的对象
- 事件发生次数
- 事件的来源

这正是使用`kubectl get events`看到的。
<a name="vH5C5"></a>
## Kubernetes 事件有哪些类型？

- **信息事件**：Pods 调度，镜像拉取，节点健康，deployment 更新，replica set 被调用，容器被杀死
- **警告**：Pod 有错误，PV 尚未绑定
- **错误**：节点已关闭，找不到 PV，无法在云提供商中创建负载均衡器等。

可以使用 REST API、API 客户端或 event recorder 直接发布您自己的事件。
<a name="hxyAG"></a>
### 最重要的 Kubernetes 事件
Kubernetes 有非常广泛的事件，这里有一些需要重点考虑的事件：

- **CrashLoopBackOff**，当 Pod 启动、崩溃、再次启动、然后再次崩溃时发生
- **ImagePullBackOff**，当节点无法拉取镜像时发生
- **驱逐事件**，当节点确定需要驱逐或终止 pod 以释放一些资源（CPU、内存等）时，可能会发生这种情况。发生这种情况时，K8s 应该在另一个节点上重新调度 pod。
- **FailedMount / FailedAttachVolume**，当 pod 需要持久卷或存储时，如果存储不可访问，此事件会阻止它们启动。
- **FailedSchedulingEvents**，当调度程序无法找到运行您的 pod 的节点时。
- **NodeNotReady**，当节点由于潜在问题而无法运行 pod 时。
- **Rebooted**
- **HostPort 冲突**
<a name="KkkmW"></a>
## 检索 Kubernetes 事件的解决方案
有多种解决方案可用于检索 Kubernetes 事件。看看现成可用的项目。
<a name="pvcGt"></a>
### Eventrouter
正如 Eventrouter 项目的 GitHub 页面所述：“事件路由器充当 Kubernetes 系统中事件资源的活动观察者，它接收这些事件并将它们推送到用户指定的接收器。这对于许多不同的目的很有用，但最值得注意的是对在 Kubernetes 集群上运行的工作负载的长期行为分析。”<br />详细信息请看 [**eventrouter** GitHub](https://github.com/heptiolabs/eventrouter)
<a name="Fc4ok"></a>
### Kubewatch
Kubewatch 是一个 K8s 事件监视工具，用于跟踪 Kubewatch 中的每个资源更改。它支持通知，它将能够在 Slack、Hipchat、Webhook、Flock、SMTP 等中发布通知。<br />详细信息请看 [**kubewatch** GitHub](https://github.com/bitnami-labs/kubewatch)
<a name="wTsXI"></a>
### Sloop
Sloop 监控 Kubernetes，记录事件和资源状态变化的历史，并提供可视化来帮助调试过去的事件。<br />详细信息请看 [**sloop** GitHub](https://github.com/salesforce/sloop)
<a name="JtXjg"></a>
### kubernetes-event-exporter
事件导出器允许将经常错过的 Kubernetes 事件导出到各种输出，以便它们可用于可观察性或警报目的。<br />事件导出器实现起来很简单，但功能非常强大。一旦事件被记录，它利用 Prometheus 客户端以 Prometheus 格式计数和报告事件。<br />详细信息请看 [**kubernetes-event-exporter** GitHub](https://github.com/opsgenie/kubernetes-event-exporter)
<a name="KVxzd"></a>
### Kspan
Kspan 是 Weaveworks 创建的一个项目，它将 Kubernetes 事件转换为 OpenTelemetry Spans，通过因果关系将它们连接起来，并将它们组合成 traces。<br />Kspan 将与 Kubernetes API 交互以收集各种事件并将生成的跟踪转发到 OpenTelemetry 收集器。<br />详细信息请看 [**kspan** GitHub](https://github.com/weaveworks-experiments/kspan)
<a name="ByBvj"></a>
## Kubernetes 事件教程
现在已经大致了解了 Kubernetes 事件是什么以及如何利用它们，可以在 YouTube 和 GitHub 上找到更详细教程：

- YouTube：[**强大的 Kubernetes events**](https://www.youtube.com/watch?v=K-F-p-ekSsM)
- GitHub：[**Kspan 和 event exporter**](https://github.com/isItObservable/Episode-6---Kubernetes-Events)
<a name="eTMWH"></a>
### 参考资料
**eventrouter: **[https://github.com/heptiolabs/eventrouter](https://github.com/heptiolabs/eventrouter)<br />**kubewatch: **[https://github.com/bitnami-labs/kubewatch](https://github.com/bitnami-labs/kubewatch)<br />**sloop: **[https://github.com/salesforce/sloop](https://github.com/salesforce/sloop)<br />**kubernetes-event-exporter: **[https://github.com/opsgenie/kubernetes-event-exporter](https://github.com/opsgenie/kubernetes-event-exporter)<br />**kspan: **[https://github.com/weaveworks-experiments/kspan](https://github.com/weaveworks-experiments/kspan)<br />**强大的 Kubernetes events: **[https://www.youtube.com/watch?v=K-F-p-ekSsM](https://www.youtube.com/watch?v=K-F-p-ekSsM)<br />**Kspan 和 event exporter: **[https://github.com/isItObservable/Episode-6---Kubernetes-Events](https://github.com/isItObservable/Episode-6---Kubernetes-Events)
