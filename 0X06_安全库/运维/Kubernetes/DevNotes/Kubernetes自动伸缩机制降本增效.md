Kubernetes<br />从技术上讲，容器化应用能该帮助组织更具有成本的优势，但Kubernetes到处布满了成本陷阱，可能会使你超出预算。幸运的是，有一些策略可以控制云成本，自动伸缩就是其中之一。Kubernetes带有三个内置的自动伸缩机制来做到这一点。它们配合得越好，运行应用程序的成本就越低。
<a name="FvjxC"></a>
### 1、Pod水平自动扩缩(HPA)
Pod 水平自动扩缩（HorizontalpodAutoscaler） 可以基于CPU利用率自动扩缩 ReplicationController、Deployment、ReplicaSet 和 StatefulSet 中的pod数量。除了CPU利用率，也可以基于其他应程序提供的 自定义度量指标 来执行自动扩缩。<br />生产环境中，许多应用程序的使用情况会出现波动，这意味着实时添加或删除pod副本会带来更好的成本收益。这就是 HorizontalpodAutoscaler (HPA) 通过自动执行此操作来提供帮助的地方。
<a name="RMGI5"></a>
#### 何时使用 HPA？
它非常适合扩展无状态应用程序，当然也可以用于有状态应用。如果将 HPA 与Cluster Autoscaler(CA)结合使用，能够为经常变化的工作负载获得最大的成本节约。当pod数量减少时，这将减少活动节点的数量。
<a name="vRwFx"></a>
#### HPA 是如何工作的？
HPA 监控pod以了解pod副本的数量是否需要更改。为了确定这一点，它采用每个pod指标值的平均值，并检查删除或添加副本，是否会使该值更接近目标。<br />例如，如果部署的目标CPU利用率为50%，而现在有五个pod在那里运行，则平均CPU利用率为 75%。为了使pod平均值更接近目标，HPA 控制器将增加三个副本。
<a name="NwO5t"></a>
#### HPA 最佳实践

- 为 HPA 提供每个pod资源指标：需要在Kubernetes集群中安装 metrics-server。
- 为每个容器配置值：HPA 根据观察到的pod的CPU利用率值（来自单个pod的资源请求的百分比）做出扩展决策。如果没有包含某些容器的值，则计算将不准确并可能导致出现糟糕的扩展决策。
- 使用自定义指标：HPA 扩展决策的另一个来源是自定义指标。HPA 支持两种类型的自定义指标：pod 指标和对象指标。确保使用正确的目标类型。还可以使用来自第三方监控系统的外部指标。

![2021-07-11-23-50-58-183742.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1626018687635-da395962-c6ab-4637-8472-6b6a20010678.png#clientId=ub2249f11-4c53-4&from=ui&id=u8086de1b&originHeight=576&originWidth=1024&originalType=binary&ratio=1&size=217529&status=done&style=none&taskId=uf3f10a19-6900-4fc6-9b0c-a772ed7422e)
<a name="yx2yQ"></a>
### 2、Pod垂直自动扩缩 (VPA)
Vertical Pod Autoscaler(VPA)，即垂直 Pod 自动扩缩容，它根据容器资源使用率自动设置 CPU 和 内存 的requests，从而允许在节点上进行适当的调度，以便为每个 Pod 提供适当的资源。它既可以缩小过度请求资源的容器，也可以根据其使用情况随时提升资源不足的容量。<br />这种自动缩放机制增加和减少了pod容器的CPU和内存资源请求，以使分配的集群资源与实际使用情况保持一致。VPA 还需要访问Kubernetes指标服务器，因为它仅替换由副本控制器管理(Replication Controller)的pod。<br />提示：如果 HPA 配置没有使用CPU或内存来设置其扩展目标，请同时使用 VPA 和 HPA。
<a name="yeEZY"></a>
#### 何时使用 VPA？
工作负载可能会在某个时候遇到高利用率，但不断增加其请求限制，不是一个好办法。可能会浪费CPU或内存资源并限制运行它们的节点。将工作负载分布到多个应用程序实例有时候也是很棘手的，这就是 Vertical Pod Autoscaler 的帮助所在。
<a name="fMXLu"></a>
#### VPA 是如何工作的？
VPA 部署由三个组件组成：

- Recommender : 监控资源利用率并计算目标值，也就是检查历史资源利用率和当前使用的模式，并推荐一个理想的资源请求值
- Updater : 检查pods 资源限制是否需要更新
- Admission Controller：在创建pod时覆盖其资源请求

由于Kubernetes不允许更改正在运行的pod的资源限制，因此 VPA 首先终止旧的pod，然后将更新的值注入新的pod规范。
<a name="CUHlP"></a>
#### VPA 最佳实践
避免在Kubernetes的1.11版本之前使用。<br />使用updateMode:Off运行 VPA，以了解要自动缩放的pod的资源使用情况。这将提供推荐的CPU和内存请求，也是以后调整的重要基础。<br />如果工作负载经常出现高使用率和低使用率的峰值，则 VPA 可能会过于激进，因为它可能会不断地一遍又一遍地替换pod。在这种情况下，使用HPA 效果更好。
<a name="b4LhX"></a>
### 3、Cluster Autoscaler(CA)
Cluster Autoscaler 在K8S集群中，通过增加/删除其中的Node，达到集群扩缩容的目的。由于 Cluster Autoscaler 控制器工作在基础设施级别，因此它需要具备添加和删除基础设施的权限，应该安全地管理这些授权信息（例如，遵循最小权限原则）。
<a name="L5ynh"></a>
#### 何时使用集群自动扩缩器？
如果希望通过动态扩展节点数量，以最大化当前集群利用率来优化成本，则此自动扩展机制非常有效。对于旨在扩展和满足动态需求的工作负载而言，它是一款出色的工具。
<a name="n3ape"></a>
#### Cluster Autoscaler 如何工作？
它会检查不可调度的pod，然后计算是否可以整合当前部署的所有pod，以在较少数量的节点上运行它们。如果 Cluster Autoscaler 识别出具有可以重新调度到集群中其他节点的pod的节点，它会驱逐它们并删除备用节点。
<a name="AKv95"></a>
#### 集群自动扩缩器最佳实践

- 部署 Cluster Autoscaler 时，要与之相匹配的Kubernetes版本一起使用。（兼容性列表）。
- 检查集群节点是否具有相同的CPU和内存容量：否则集群自动缩放器将无法工作，因为它假设集群中的每个节点都具有相同的容量。
- 确保自动缩放的pod都具有指定的资源请求。
<a name="kRmrD"></a>
### 总结
自动缩放机制对于控制云成本非常有价值，但它们需要大量的手动配置：

- 防止 HPA 和 VPA 冲突：需要检查 HPA 和 VPA 策略是否最终发生冲突。密切关注成本，以防止它们失控。
- 平衡三种机制：需要平衡三种机制的组合，以确保工作负载支持峰值负载，并在负载较低时将成本保持在最低水平。
