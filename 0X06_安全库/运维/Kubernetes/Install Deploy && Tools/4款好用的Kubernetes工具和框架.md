Kubernetes<br />Kubernetes工具和框架是发挥Kubernetes技术的重要组成部分，可满足各种需求并增强体验，因此在做技术选型的时候，需要选择一个最优的工具、最稳的框架。<br />这里介绍几款可以最大化发挥K8s效用的工具和框架。
<a name="is02k"></a>
## 1、Istio
Istio是Kubernetes中最受欢迎的服务网格工具，既开源、功能又很强大，它的独特之处在于它提供了巨大的灵活性，而没有通常的复杂性，除此之外，Istio 还有着其他更加关键的设计目标，这些目标对于使系统能够应对大规模流量和高性能地服务处理至关重要。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649225255918-0f4f19db-44de-4646-ac1a-404842da1fe0.png#clientId=u76395258-6eb3-4&from=paste&id=u1b74b59e&originHeight=305&originWidth=510&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc515327f-c468-45f8-ab37-46ae58e281f&title=)<br />Isito的核心功能大致有5点：流量管理、安全、可观察性、平台独立、集成和定制，相比其他 Service Mesh工具，用一张图说明Istio所存在的优势。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649225255870-0bc0d4d8-777f-4d45-b973-118708f37e3f.png#clientId=u76395258-6eb3-4&from=paste&id=u7401a686&originHeight=614&originWidth=583&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5490e9ce-d97f-44a1-823f-ea37008c9be&title=)<br />由此可见，Istio 作为目前众多 Service Mesh 中最闪耀的新星，并不奇怪。
<a name="aR0UG"></a>
## 2、Knative
Knative是谷歌发起的一个Serverless开源解决方案，官方给它的定位是“基于Kubernetes的平台，用来构建、部署和管理现代Serverless工作负载”。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1649225255867-e2ebee35-f9ee-497c-8fe3-9152578ea0b0.jpeg#clientId=u76395258-6eb3-4&from=paste&id=u347370d5&originHeight=404&originWidth=728&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud63dc602-2417-4287-b81c-0f7c8b3a4cc&title=)<br />通过Knative可将云原生应用开发在三个领域的最佳实践结合起来，这三个领域的最佳实践是指服务构建部署的自动化、服务编排的弹性化以及事件驱动基础设施的标准化。它既可以以托管服务形式运行在公有云中，也可以部署在企业内部的数据中心，从而很好地解决多云部署以及供应商锁定的问题。<br />Knative是一个云原生Serverless框架，可以运行任何无状态容器应用。目前成为云原生计算基金会一部分的首批无服务器平台之一。
<a name="Yknai"></a>
## 3、Tekton
Tekton是一种适用于创建持续集成和持续部署/交付(CI/CD)系统的谷歌开源的Kubernetes原生框架，原本是Knative项目里的一个子项目，后来成长为一个通用的框架，能够提供灵活强大的能力去做基于Kubernetes的构建发布。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1649225255940-54d19b04-5d9e-462a-a920-e305c4c5e101.jpeg#clientId=u76395258-6eb3-4&from=paste&id=u7343dd2f&originHeight=500&originWidth=890&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0d38c6b8-3cd8-4d5c-aee3-80d1b6d77fe&title=)<br />它支持多云/多集群下进行搭建、测试和部署，可实现滚动部署、蓝/绿部署、金丝雀部署或 GitOps 工作流等高级部署。阿里就选择Tekton之上构建应用交付体系。<br />目前，Jenkins 的子项目 JenkinsX 也开始默认使用 Tekton 作为 CI 引擎。使用云原生一等公民 CRD + Controller 实现的 Tekton ，无疑有机会成为云原生的主流编排引擎。
<a name="twe9g"></a>
## 4、ArgoCD
Argo CD 是一个为 Kubernetes 而生的，遵循声明式 GitOps 理念的持续部署（CD）工具，它的配置和使用非常简单，并且自带一个简单易用的 Dashboard 页面，并且支持多种配置管理/模板工具。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649225255871-0518ba01-e698-4a12-8092-3feb8d2c9907.png#clientId=u76395258-6eb3-4&from=paste&id=u6dfc6ed1&originHeight=483&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9124c8d6-b93f-403d-b363-52b62bd780b&title=)<br />该功能集虽然侧重于应用程序部署的管理，但是却非常出色，功能十分丰富，与Kubernetes的集成完美无瑕。根据文档，它可以处理：

1. Kustomize应用程序
2. Helm Charts
3. Ksonnet应用
4. YAML/JSON清单目录，包含Jsonnet
5. 配置管理插件配置的任何自定义配置管理工具

Kubernetes不是一个岛屿，为了支持生产级部署，需要许多可移动的工具和框架。但对于IT人来说，如何精准应用这些工具和框架，才是自我能力的证明。
