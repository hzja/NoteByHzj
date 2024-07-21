DevOps<br />构建可扩展且高度可靠的软件系统是每个SRE的最终目标。<br />![2021-05-16-23-11-14-535736.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621177891675-9d39949f-d64c-4e42-bd4c-f318b557866c.png#align=left&display=inline&height=457&id=u95005171&originHeight=457&originWidth=1080&size=560415&status=done&style=none&width=1080)<br />在SRE/DevOps领域中，有大量出色的开源项目，每个项目都有新颖而激动人心的解决方案。介绍一些在监控，部署和运维领域最受欢迎的开源项目。
<a name="jUwog"></a>
### 1、Cloudprober
Cloudprober可以主动跟踪和监控应用程序，并提前发现故障。它使用“活动(active)”监控模型来检查组件是否按预期运行。例如，它会主动运行探针，以确保前端是否可以访问后端。同样，可以运行探针来验证本地系统是否可以真正访问云虚拟机。通过这种跟踪方法，可以轻松地跟踪应用程序的配置，还可以轻松定位系统中发生的问题。
<a name="Xt4OC"></a>
#### 特征：

- 能与Prometheus和Grafana等开源监控系统集成。Cloudprober也可以导出探测结果。
- 目标自动发现。为GCE和Kubernetes提供了开箱即用的支持；其他云服务可以轻松配置。
- 部署简单。它可以通过Docker容器快速部署。
- 占用空间小。Cloudprober镜像大小很小，仅包含静态编译的二进制文件，并且它需要非常少的CPU和RAM就能运行大量的探针。

![2021-05-16-23-11-14-604002.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621177898494-1e763853-45d0-4589-a1fa-439121d85083.png#align=left&display=inline&height=484&id=u24849684&originHeight=484&originWidth=608&size=19915&status=done&style=shadow&width=608)
<a name="IK8pn"></a>
### 2、Cloud Operations Sandbox（Alpha）
Cloud Operations Sandbox是一个开源平台，可以通过该平台了解Google服务的可靠性工程实践，还可以使用Cloud Operations工具套件，并将其应用于云服务。注意：这需要一个Google云服务帐户。
<a name="nNrsj"></a>
#### 特征：

- 演示服务—基于现代云原生微服务架构设计的应用程序。
- 一键式部署-通过脚本将服务部署到Google Cloud Platform的工作。
- 负载生成器-在演示服务上模拟流量的组件。

![2021-05-16-23-11-14-669874.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1621177907564-9a7cb5ac-be07-468e-8c7f-30366f2993d7.jpeg#align=left&display=inline&height=511&id=u7b66704e&originHeight=511&originWidth=1080&size=45281&status=done&style=none&width=1080)
<a name="sV6Ar"></a>
### 3、Kubernetes的版本检查器
这是一个Kubernetes工具，可以查看集群中运行的镜像的版本。该工具还允许在Grafana仪表板上，以表格查看镜像版本。
<a name="rbWbi"></a>
#### 特征：

- 可以一次设置多个镜像仓库。
- 可以将版本信息视为Prometheus指标。
- 支持诸如ACR，DockerHub，ECR之类的镜像仓库。

![2021-05-16-23-11-14-796686.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621177915798-80fd0d49-ac0a-44c6-b783-212d3c565802.png#align=left&display=inline&height=789&id=u1029c922&originHeight=789&originWidth=1080&size=210996&status=done&style=none&width=1080)
<a name="TeDVi"></a>
### 4、Istio
Istio是一个开源框架，用于监控微服务流量，实施策略以及以标准化方式汇总遥测数据。Istio的控制平面上为集群管理（例如Kubernetes）提供了一个抽象层。
<a name="rQXtF"></a>
#### 特征：

- 支持HTTP，gRPC，WebSocket和TCP通信的负载均衡。
- 通过丰富的路由规则，重试，故障转移和故障注入对流量行为进行细粒度控制。
- 可插拔的策略层和配置API，支持访问控制，速率限制和配额。
- 集群的入口和出口，以及集群内所有流量指标，日志和跟踪信息。
- 基于身份的验证和授权，保障了服务间通信的安全。

![2021-05-16-23-11-14-843564.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1621177923387-00ba6394-8cb9-4dd7-9b11-32e5890dcdf8.jpeg#align=left&display=inline&height=738&id=u0c5a8837&originHeight=738&originWidth=995&size=43558&status=done&style=none&width=995)
<a name="gky55"></a>
### 5、Checkov
Checkov是“基础结构即代码”的实践，它是一个静态代码检查工具，能够扫描Terraform，Cloud Details，Cubanet，Serverless或ARM模型云基础架构，并能够检测配置是否有误。
<a name="F2zAl"></a>
#### 特征：

- 超过400条内置规则涵盖了AWS，Azure和Google Cloud的最佳安全实践。
- 能够监控Terraform管理的IaaS，PaaS或SaaS的开发，维护和更新。
- 能够在EC2用户数据，Lambda上下文和Terraform程序中检测AWS凭证。

![2021-05-16-23-11-14-907427.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621177931156-5606bfc2-fb61-4ab4-b3a5-a4945b9d46b2.png#align=left&display=inline&height=1036&id=u2403065c&originHeight=1036&originWidth=984&size=56283&status=done&style=none&width=984)
<a name="UVy7q"></a>
### 6、Litmus
Litmus是一个云原生混沌工程。Litmus提供了一些工具来协调Kubernetes上的混沌情况，以帮助SRE发现其部署中的漏洞。SRE首先使用Litmus进行混沌测试，然后使用它来发现故障和漏洞，提高系统的弹性。
<a name="aFCyp"></a>
#### 特征：

- 开发人员可以在应用程序开发期间运行混沌测试，作为单元测试或集成测试的扩展。
- 对于CI流水线构建者：当应用程序遇到流水线中的故障时，可以运行混沌测试以查找错误。

![2021-05-16-23-11-14-959272.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1621177941867-46d94a54-9f6a-4595-8ba5-ab2495b1bb31.jpeg#align=left&display=inline&height=880&id=u1ef72a54&originHeight=880&originWidth=1080&size=68565&status=done&style=none&width=1080)
<a name="x11sY"></a>
### 7、Locust
Locust是一个易于使用，可编写脚本且灵活的性能测试工具。可以使用Python代码自定义操作，而不是使用笨拙的UI。这使Locust可以扩展且并对开发人员友好。
<a name="QMrG0"></a>
#### 特征：

- Locust具有分布式和可扩展性，可轻松支持成百上千的用户。
- UI界面，可实时显示进度。
- 稍加修改就可以测试任何系统。

![2021-05-16-23-11-15-066016.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621177953156-2127ae6f-e004-4690-8649-1d3c0fcffca6.png#align=left&display=inline&height=430&id=u4defbd71&originHeight=430&originWidth=1080&size=137470&status=done&style=none&width=1080)
<a name="bhZrY"></a>
### 8、Prometheus
Prometheus是Cloud Native Computing Foundation项目，是一个系统和服务监控系统。它在特定时间从已配置的目标中提取指标，测试规则并显示结果。如果违反了指定条件，它将触发警告通知。
<a name="s25T2"></a>
#### 特征：

- 多维数据模型（由度量标准定义的时间序列）。
- 通过服务发现或静态配置发现目标。
- 不依赖于分布式存储；单个节点是具有自治能力。
- PromQL，一种强大而灵活的查询语言。

![2021-05-16-23-11-15-153749.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1621177961581-20df3136-a8ec-4042-939f-ace26df54a91.jpeg#align=left&display=inline&height=745&id=u9a5b2a28&originHeight=745&originWidth=1080&size=59477&status=done&style=none&width=1080)
<a name="JRL3u"></a>
### 9、Kube-monkey
Kube-monkey是Netflix的Chaos Monkey在Kubernetes集群中的实现方式。对一个系统进行压力测试的一种万无一失的方法是随机打碎东西。这就是Netflix的Chaos Monkey背后的理论，这是一种混沌工程工具。
<a name="sOP3m"></a>
#### 特征：

- Kube-monkey的工作方式是在指定的集群中随机破坏pod，并且可以在特定的时间窗口内进行微调。
- 可根据要求，进行高度可定制。

![2021-05-16-23-11-15-238523.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621177972058-e1c9c0a4-0ab7-4c93-b76a-14e260c29da2.png#align=left&display=inline&height=359&id=u0607edff&originHeight=359&originWidth=638&size=105497&status=done&style=none&width=638)
<a name="Q2qFd"></a>
### 10、PowerfulSeal
PowerfulSeal会将故障注入Kubernetes集群中，从而尽快识别问题。它使创建完整的混沌工程成为可能。
<a name="JO0HM"></a>
#### 特征：

- 与Kubernetes，OpenStack，AWS，Azure，GCP和本地环境兼容。
- 可以与Prometheus和Datadog集成以进行指标收集。
- 通过自定义用例，支持多种模式。

![2021-05-16-23-11-15-342245.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1621177988250-4033f94d-e589-4819-9c6f-a836b3aa4e84.jpeg#align=left&display=inline&height=692&id=u156d0b8d&originHeight=692&originWidth=1080&size=61382&status=done&style=shadow&width=1080)
<a name="tIev1"></a>
### 结论
开源技术的最大好处是它们的可扩展性。可以根据需要向该工具添加功能，以更好地适应项目的基础架构。<br />由于微服务架构成为主流，因此使用可靠的工具来监控和诊断系统，必将成为每个开发人员的必备技能。
