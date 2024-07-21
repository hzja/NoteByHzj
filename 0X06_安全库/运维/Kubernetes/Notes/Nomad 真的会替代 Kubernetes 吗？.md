KubernetesNomad
> 链接：[https://vinothkumar-p.medium.com/is-nomad-taking-over-kubernetes-70d7418d3464](https://vinothkumar-p.medium.com/is-nomad-taking-over-kubernetes-70d7418d3464)
> 来自网络

根据目前的市场状况，大多数人都同意 Kubernetes（又名“K8s”）赢得了容器编排之战，战胜了 Docker Swarm 和 Mesos 等替代品。但在 K8s (2014) 的同一时间还有另一个编排项目，那就是 HashiCorp 的 Nomad。(2015)<br />Kubernetes 由 Google 孵化并得到持续的社区支持，实际上是容器编排的理想之选。然而，Kubernetes 并不是适用于所有用例的解决方案。正如许多开发人员提到新功能的复杂性一样，开发人员将疑虑和安全错误配置视为实施的潜在障碍。并不是说这些问题无法克服，但大多数用户承认 Kubernetes 固有的复杂性使得平台的入门和安全变得相当混乱。
<a name="QRGBE"></a>
## Nomad 到底是什么？
Nomad 是Hashicorp的一个编排工具，允许用户部署和管理不同类型的应用程序，例如：

- 支持容器
- 支持传统的应用程序栈。
- 支持微服务应用程序。
- 支持批量应用。

从上面看，Nomad 看起来像这样<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1669270241340-3ff19e53-ac72-4a18-9064-149df08b0453.png#averageHue=%23f5f3f0&clientId=ua5e869a5-4390-4&from=paste&id=u6242f8e1&originHeight=295&originWidth=524&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5ce7a187-d42f-448e-8281-c775436fc21&title=)<br />使 Nomad 有用的是一组丰富的 API：

- 帮助我们自动化部署、应用程序扩展和升级。
- 使开发人员能够直接管理部署。
- 自动管理故障。

隐藏节点管理等复杂细节，让用户只选择启动和运行应用程序所需的内容。如前所述，Docker 是受支持的，但是任何类型的应用程序都可以在需要的任何类型的操作系统上使用（Linux、Windows、BSD 和 OSX 都支持）。可以创建集群，添加来自不同数据中心和不同区域的节点。<br />最后，Nomad 是一个高度可扩展的工具，它允许用户超越标准扩展概念的限制。
<a name="MVMtd"></a>
## 为什么是 Nomad？
Nomad 和 Kubernetes 的主要区别在于 Nomad 更加通用和轻量级。Nomad 可以像一个简单的任务调度程序一样工作，也可以根据项目规范承担更重的编排角色。Nomad 是由 HashiCorp 生产的补充工具组成的套件的一部分：

- Terraform，用于快速基础设施开发
- Consul，用于自动服务网络
- Vault，Hashicorp 的 secrets 管理工具
<a name="Qrkvs"></a>
## 比较
Kubernetes 是一个端到端的容器编排平台，它依赖于各种松散耦合组件的动态生态系统。<br />Nomad 在架构上要简单得多，但提供了与强大的协调器相同的功能。这两个平台有相同点，也有异同点，具体请往下看。
<a name="obO26"></a>
## 相同点
Kubernetes 和 Nomad 都是为容器编排和支持类似用例而构建的开源工具。因此，两者都具有许多共同特征：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1669270241296-fe55810a-8a43-4c2f-aff5-7da383130ac7.png#averageHue=%23fbfbfb&clientId=ua5e869a5-4390-4&from=paste&id=u12676fec&originHeight=642&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u36a9c95c-e198-480d-bdea-c3a86458ce4&title=)
<a name="iHIMY"></a>
## 差异点
尽管这两个平台都适用于容器编排，但 Nomad 和 Kubernetes 有一些根本区别：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1669270241317-83d654ce-bb7e-422a-b5a8-480429af8387.png#averageHue=%23fafafa&clientId=ua5e869a5-4390-4&from=paste&id=u9a0aeaf1&originHeight=541&originWidth=862&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uaa449c8e-6001-4355-9d65-36612ea78c6&title=)
<a name="oB7EJ"></a>
## 优缺点
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1669270241230-e6bbe603-e94a-4939-8133-933e86e731ea.png#averageHue=%23fcfcfc&clientId=ua5e869a5-4390-4&from=paste&id=ue045a45f&originHeight=535&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0c533ef2-783f-4a5b-86c6-c0301d6a3b9&title=)
<a name="nwoih"></a>
## 选择哪一个？
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1669270241220-b9b89445-5cc3-4a9b-bcf3-6698af7a4a9f.png#averageHue=%23f8f8f8&clientId=ua5e869a5-4390-4&from=paste&id=ucc586afa&originHeight=402&originWidth=651&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ubccdc5cf-ad0e-41e4-8967-b078114c345&title=)
<a name="WFkBM"></a>
## 结论
Kubernetes 是一个独立的编排工具，具有许多内置服务，可提供运行基于容器的应用程序所需的所有功能。它有庞大及时响应的社区支持，提供丰富的工具包和大量开箱即用的解决方案。但是，手动设置很困难，并且仅针对容器化应用程序而设计。<br />相比之下，Nomad 易于安装和操作，因为它只专注于集群管理。它还支持各种类型的工作负载，但它提供的功能有限，需要安装第三方工具来解决 Kubernetes 默认实现的任务。<br />如果您的应用程序**需要额外的功能，并且您愿意花时间学习该工具，那么 Kubernetes 可能是更好的选择**。但是，如果您**更喜欢没有附加功能的更简单的工作流程，那么 Nomad 可能适合您**的应用程序。<br />最终，您选择哪种工具取决于您的用例以及您在生产周期中实施该工具的能力。从来没有一种工具适合所有人，所以在做出决定时，请务必考虑上述因素。
