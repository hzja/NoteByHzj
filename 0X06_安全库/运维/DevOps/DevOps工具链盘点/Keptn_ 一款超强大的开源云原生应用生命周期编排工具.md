![](https://cdn.nlark.com/yuque/0/2023/png/396745/1673944555998-9deabf73-7f0f-49dd-805c-03cdc636ea7f.png#averageHue=%234190c1&clientId=ub6d37616-df99-4&from=paste&id=u0bdc36e9&originHeight=519&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ueeff9c47-5eb7-4da5-8dfb-ef0627f8a71&title=)<br />DevOps 自动化已经跨越私有和公有云环境，并包含监视，警报，审核以及连续，渐进式交付。这种自动化，支持 DevOps 实践和 Kubernetes 架构，旨在帮助 DevOps 团队更高效地开发和发布高质量，安全的软件，从而为组织创造更高的商业价值。但是，挑战也很多。
<a name="D21ag"></a>
## DevOps 的主要挑战
通过了解，在采用 DevOps 和可扩展性方面，面临的一些主要挑战有：

- 每个失败的部署，都需要大约五个小时的修复，并需要进行六次重试才能正确完成。
- 复杂的流水线，维护困难。
- 持续交付中，大约80％的时间用于人工质量审计。
- 故障排除中，的大约90％的时间用于人工修复。

为什么会这样呢？为什么部署会占用大量时间和资源？为什么如此多的组织无法释放 DevOps 的真正潜力，无法更快地开发出更好，更安全的软件？<br />为了回答这些问题，了解和确定了一些关键因素，这些因素扼杀了整个组织中 DevOps 的采用和可扩展性。
<a name="qVKpB"></a>
## 阻碍 DevOps 采用和扩展的7个瓶颈

1. 缺乏多云可观察性：对混合云和多云环境的有限访问和可见性，掩盖了 DevOps 采用的真实状态。对环境的可观察性越差，在组织内成熟和自动化的 DevOps 实践的难度就越大。
2. 对传统工具的依赖：微服务与单体应用程序不同，但是许多团队仍在使用相同的传统工具进行软件交付。
3. 对传统流程的依赖：同样，并非所有微服务都是平等的，但是组织通常会在整个流程中应用顺序的，瀑布式的开发流程，就好像它们是平等的一样。
4. 紧密耦合的体系结构：某些组织结构和流程导致紧密的体系结构耦合和相互依赖的系统，从而使内部扩展 DevOps 更加困难。
5. 定制：存在大量定制化产品。
6. 缺乏标准：缺少统一标准指导实践。
7. 缺乏自动化：忽略了对自动化操作的需求。
<a name="qhZpf"></a>
## Keptn 应用而生
这些瓶颈，背后缺少的是端到端的可观察性，自动化和 AI，以推动数据驱动的交付和编排。<br />这些需求激发了一个名为 **Keptn(**[**https://keptn.sh/**](https://keptn.sh/)**)** 的开发，该项目是基于事件的控制平面，用于云原生应用程序的连续交付和自动化操作。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1673944555862-9ed7bf81-9f6f-4292-9dea-dad8142b5130.png#averageHue=%23a7d5d4&clientId=ub6d37616-df99-4&from=paste&id=u6ad59ee9&originHeight=440&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4a0de319-038d-4a27-af4e-387a2e4a0b4&title=)<br />使用数据驱动的声明式编程方法进行编排，Keptn 消除了将流程放入脚本的需求。基于 GitOps，服务级别目标（SLO）和开源互操作性标准（例如用于与工具进行通信的 CloudEvents），Keptn 实现了持续交付和自动修复。<br />前面提到：流水线中约 95％ 时间都花费在扩展流程，更改工具以及修补程序上。所有这些都是因为传统流水线太复杂而无法扩展。<br />在这种情况下，解决方案是删除硬依赖性和自定义集成。通过将流程，例如构建、准备、部署、测试、通知、回滚与工具，例如配置、管理、部署、回滚、监视、测试和 ChatOps 分开，团队可以使用事件驱动的体系结构连接这些流程和功能。<br />基于 Keptn 的业务流程范例，可以在整个组织中快速扩展和采用这些 DevOps 流程。
<a name="VuMhM"></a>
## 解决 DevOps 瓶颈的7种方法
以下是 Keptn 解决上述 DevOps 采用和可伸缩性挑战的七种方法：

1. 专为多云而构建：**Keptn** 专为现代云原生和现有企业技术而设计。
2. 灵活的工具编排：无需使用旧版工具进行交付，而是根据组织的独特的体系结构来编排所有工具。
3. 适应性强的流程：它不会在所有微服务中应用相同的旧流程，而是应用最适合的流程。
4. 解耦的体系结构：**Keptn**没有紧密耦合的相互依赖性，而独立于底层基础结构来运行流程。
5. 与定制无关：开放的集成标准可确保与所有 DevOps 工具的连接，而无需指定供应商。
6. 明确的标准：使用标准化的 SLO 进行数据驱动的生命周期编排。
7. 专为自动化而构建：针对先前专注于自动化交付而不是运维的模型，**Keptn** 进行了协调。

DevOps 的目标是更快地发布更好，更安全的软件。采用和扩展 DevOps 流程的瓶颈，使太多团队无法实现这种方法的全部优势，并且限制了团队将其运维提升到更高水平的能力。<br />为了改变工作方式，并促进更有效的协作，更快的创新和对业务的更积极影响，DevOps 团队需要能够利用自助服务平台模型来进行数据驱动的交付和业务编排（例如 Keptn），以扩展 DevOps 的交付。<br />原文链接：[https://thenewstack.io/7-ways-devops-can-overcome-scalability-challenges-using-automated-orchestration/](https://thenewstack.io/7-ways-devops-can-overcome-scalability-challenges-using-automated-orchestration/)
