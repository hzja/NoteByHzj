DevOps<br />带有DevOps特性的工具列表每天都在增长，了解团队的需求对于使用正确的服务来促进新应用程序的开发非常重要。本文将按类别了解不同的 Devops 工具，以提高团队的生产力和协作。<br />如果没有开发团队和运维人员之间真正的合作愿望，这些工具的部署是没有用的。
<a name="h0YsR"></a>
## Devops 文化及其好处
DevOps是“开发人员”和“运维”这两个词的组合。它是一种协作文化，旨在使开发团队和运营部门之间的流程自动化，以促进软件的开发、测试和交付。<br />开发人员应该创造价值并使产品/服务更具创新性，而运营人员的目标是保持基础设施的稳定性。<br />Devops 文化的优势是多方面的。DevOps 协作允许更快 、更高质量的发布。Devops 团队在保持基础架构质量和稳定性的同时更频繁地交付。良好的Devops 策略基于运维人员和开发人员之间的广泛协作、更好的沟通以及更好的团队绩效。
<a name="RJ5y0"></a>
## Devops 中使用的不同工具
Devops 团队每天使用各种工具来完成各种任务和任务。在此处准备了这些工具的（非详尽的）列表。
<a name="lqx49"></a>
### 源代码管理工具
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651456872242-6e7d6908-e002-4fe8-aa2e-b51f58bcc800.png#clientId=ub6d80978-073b-4&from=paste&id=ucd3f9903&originHeight=240&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u076d9ce1-5740-4cd1-aa55-c9ba9e5b522&title=)<br />Devops 协作的第一步是让开发团队和运维人员在**单一源代码管理工具上保持一致**。它是一个版本控制工具：任何代码修改都会导致新版本的创建。从历史上看，运维人员不使用这种工具，因为自动化程度很低，一切都是手动的，因此没有代码。但是，一旦有了代码，最好的做法是分享它并让您的同行对其进行审查。这就是代码管理工具发挥作用的地方。<br />代码管理有两种类型：

- Git和Subversion等工具，用于创建文件历史记录：此时，您的文件已发生此类更改。Subversion 是一个比 Git 更老、效率更低的工具。
- Github、Gitlab和Bitbucket等工具用于共享其代码，以及随之而来的历史。它们基于 Git，并且可以拥有代码的历史并对其进行多次处理。如果说 Github 在历史上是垄断的，那么 Gitlab 越来越受欢迎，尤其要感谢Gitlab CI的高效。
<a name="pprj8"></a>
### 持续集成/持续部署测试
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651456872330-001c5fdb-bd03-4426-a817-88d1e9524e11.png#clientId=ub6d80978-073b-4&from=paste&id=u5e9cb584&originHeight=146&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u69354519-f0bb-4082-8f84-15bc83c24df&title=)<br />持续集成和持续部署( CI/CD )工具可实现源代码更改测试的自动化。实际上，CI/CD 工具通过减少创建新功能所需的时间来实现应用程序的现代化。<br />有许多 CI/CD 工具。最常用的平台之一是Jenkins，这是一个开源工具（虽然可能很难开始使用）。<br />还有付费解决方案，例如GitlabCI、Bamboo、TeamCity、Concourse、CircleCI或Travis CI。<br />云提供商，尤其是谷歌和 AWS，也提供了他们自己的持续集成和部署工具。
<a name="DqQjB"></a>
### 容器<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651456872247-9d22c566-3645-4400-ab1d-85bf6a4b23ab.png#clientId=ub6d80978-073b-4&from=paste&id=uc7d4b195&originHeight=211&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=udecec7d8-cb2a-4933-9e6f-3f2e00b937d&title=)
容器使隔离应用程序及其运行所需的所有元素成为可能。容器的使用使得从开发人员的代码到生产尽可能“iso”成为可能，并且在生产时没有意外。<br />Docker 将应用程序在这些虚拟容器中的部署自动化和标准化，并在这一领域的工具中脱颖而出。Docker 的替代品是RKT，它是 CoreOS 基金会推动的标准。<br />使用容器时，对编排的需求会很快出现。<br />编排容器可以更轻松地部署和管理它们。市场上使用最广泛的编排器是Kubernetes，但还有其他的，例如MesOs和Docker-Swarm。
<a name="qUmIX"></a>
### 云提供商
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651456872241-70a57e69-aa0f-45d3-bb09-6af2d8352843.png#clientId=ub6d80978-073b-4&from=paste&id=u018d79e7&originHeight=297&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0b3921a6-fd82-47da-9f9c-8bf723194fe&title=)<br />云提供商为企业和个人提供远程存储解决方案。如今，三大巨头共享云服务市场：谷歌云平台、Azure 和 AWS。通过提供最广泛的服务，AWS 无疑是该市场的全球领导者。<br />当谈论云提供商时，会想到负载均衡服务。负载均衡服务的任务是将负载分布在不同的设备上，从而提高响应时间。HAproxy是负载均衡中的参考。
<a name="msrv0"></a>
### 自动化和配置管理
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651456872570-750490bc-5ffd-4205-95d1-8ca819da1342.png#clientId=ub6d80978-073b-4&from=paste&id=u090cf4a0&originHeight=316&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9eb7b90c-c06b-469f-a33a-e86ef333bae&title=)<br />自动化有助于消除 DevOps 团队的重复性任务。<br />Devops 中存在几种类型的自动化：

- 在服务器上设置自动配置
- 自动化服务器操作

根据现有基础设施和公司需求，存在多种工具：

- Terraform：基础设施供应；
- Ansible：从服务器的配置管理；
- Puppet：从服务器的配置管理；
- Salt：从服务器的配置管理。
<a name="G5wEE"></a>
### 监控和警报
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651456872567-4f01418f-c7e5-4a7e-8aec-0b8ce4c77f5a.png#clientId=ub6d80978-073b-4&from=paste&id=u5103968a&originHeight=278&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2847b862-a1f7-440d-93a0-059472b8cde&title=)<br />监视和警报工具使您能够对基础架构有一个概览，以解决出现的问题并提高性能。<br />开源Prometheus应用程序和Grafana服务允许您监控 Kubernetes 集群。通过耦合三个工具，ELK（Elasticsearch、Logstash 和 Kibana）是一个强大的日志分析解决方案。您可以单独发挥每个工具的性能并使其适应您的需求：Logstash 用于规范化/发送日志，Elasticsearch 用于存储，Kibana 用于可视化。ELK 允许进行日志分析（取证）和聚合（仪表板）。
<a name="Hj0bL"></a>
### 项目管理工具
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651456872617-4bcbc90c-110c-4b7b-ab0b-e9ac5622de0b.png#clientId=ub6d80978-073b-4&from=paste&id=u122c7ff4&originHeight=360&originWidth=814&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud9520f47-3fbe-4fe3-917c-cd605d45e11&title=)<br />要进行软件的开发，依靠 Devops 团队中通用的项目管理工具似乎是必不可少的。<br />Jira是一种敏捷项目管理工具，可帮助规划、跟踪和管理软件开发项目。使用 Jira，开发团队的每个成员都可以跟踪项目的进度并设置 sprint 优先级。<br />另一方面，Trello因其在管理项目的各种任务方面的直观性和简单性而脱颖而出。
<a name="sQsYW"></a>
### 秘密管理
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651456872667-bfad0e71-713d-4100-a3bd-691e32bf67ed.png#clientId=ub6d80978-073b-4&from=paste&id=uc01d59a5&originHeight=356&originWidth=832&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uedc2dd6e-64bf-469f-86d6-f3071c625d5&title=)<br />随着对更高效安全的需求，新的秘密管理工具出现了，例如Vault。Vault 允许静态和动态组织秘密。<br />Secrets，Kubernetes 秘密管理服务是 Vault 的替代品。
