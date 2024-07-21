CNCF Kubernetes<br />像[**Kubernetes**](https://containerjournal.com/features/when-using-kubernetes-does-and-doesnt-make-sense/)这样的容器管理器已经成为管理大量容器的关键。通过在集群（一组机器）中运行容器，系统可以优化每个应用程序消耗内存的方式。随着云原生生态系统向微服务设计和日益分散的计算发展，自动化这些容器管理操作变得至关重要。<br />[**Kubernetes**](https://containerjournal.com/features/kubernetes-separating-the-hype-from-production-success/)已成为接近标准的容器编排和调度实用程序，使工程师能够从一个控制平面更好地管理其分布式节点。如今，它是维护最积极的编排器，这意味着该领域的许多其他工具都倾向于与 Kubernetes 集成并将其扩展到独特的开发场景中。例如，kube-rs 使使用 Rust 语言的 Kubernetes 更容易。<br />云原生计算基金会 (CNCF) 一直是[**streaming**](https://containerjournal.com/features/5-cncf-projects-for-streaming-and-messaging/), [**observability**](https://containerjournal.com/topics/container-management/7-open-source-cloud-native-tools-for-observability-and-analysis/), [**container runtime**](https://containerjournal.com/topics/container-networking/4-container-runtime-projects-from-cncf/), [**CI/CD**](https://containerjournal.com/features/6-cncf-projects-for-ci-cd/)等领域新的云原生工具的一股力量。<br />下面，将研究由[**CNCF**](https://landscape.cncf.io/guide#orchestration-management--scheduling-orchestration)托管的七个与调度和编排相关的项目。其中许多开源项目都支持 Kubernetes，并有助于处理多集群管理、多云、数据密集型工作负载和其他特定要求等问题。
<a name="KZ7dW"></a>
## 1、Kubernetes
_用于自动化部署、扩展和管理容器化应用程序的系统_

- GitHub：[https://github.com/kubernetes/kubernetes](https://github.com/kubernetes/kubernetes)
- 网站：[https://kubernetes.io/](https://kubernetes.io/)

Kubernetes，通常缩写为 K8s，是使用最广泛的容器编排器。Kubernetes 最初由 Google 设计，擅长调度和扩展大规模容器工作负载、优化它们的运行位置以及处理许多其他应用程序生命周期职责。正如之前详述的，[**Kubernetes 与 Docker 不同**](https://containerjournal.com/editorial-calendar/best-of-2021/whats-the-difference-between-docker-and-kubernetes/)**，**尽管这两种工具经常同时使用。Kubernetes 于 2014 年开源，现在是一个完全毕业的 CNCF 项目。
<a name="ehKov"></a>
## 2、Crossplane
_通用控制平面。_

- GitHub：[https://github.com/crossplane/crossplane](https://github.com/crossplane/crossplane)
- 网站：[https://crossplane.io/](https://crossplane.io/)

现在有多种 Kubernetes 风格。Kubernetes 可以通过 EKS、AKS、GKE、ACK 或 PKS 等服务进行自我管理或云管理。[**多集群管理**](https://containerjournal.com/features/cncf-radar-highlights-kubernetes-multi-cluster-preferences)也有不同的做法。Crossplane 是一种扩展 K8s API 的工具，可以更轻松地与多个云提供商交互。使用 Crossplane，团队可以通过一个通用 API 进行通信，以自定义方式管理多个集群和供应商。Crossplane 还支持插入安全策略和护栏，以帮助保护您的云原生基础架构。Crossplane 由 Upbound 创建，在 CNCF 中处于孵化状态。
<a name="zdIta"></a>
## 3、Fluid
_用于弹性数据抽象和加速的云原生编排平台。_

- GitHub：[https://github.com/fluid-cloudnative/fluid](https://github.com/fluid-cloudnative/fluid)
- 网站：[http://pasa-bigdata.nju.edu.cn/fluid/index.html](http://pasa-bigdata.nju.edu.cn/fluid/index.html)

如今，在 AI/ML 开发等场景中使用数据密集型应用程序很常见。Kubernetes 是一个容器编排器，而 Fluid 则是为编排数据层而设计的。Fluid 是一个开源的 Kubernetes 原生项目，可帮助处理数据并按需扩展数据集缓存。基于 Alluxio，Fluid 为管理数据集提供了运行时和额外的抽象，使其成为处理分布式缓存的主要工具。通过优化数据处理方式，Fluid 可以比其他方法更快地训练[**AI/ML 模型**](http://tbm-auth.alicdn.com/e99361edd833010b/dSVC55aoHBRio4co9aD/NQYZru9jomF79n3wGnO_277528130570_hd_hq.mp4?auth_key=1646776242-0-0-4aa8790873b3730a8c51365bc7209db7)**。**在撰写本文时，Fluid 是一个沙盒 CNCF 项目。
<a name="xRuCJ"></a>
## 4、Karmada
_多云、多集群 K8s 编排器。_

- GitHub：[https://github.com/karmada-io/karmada](https://github.com/karmada-io/karmada)
- 网站：[https://karmada.io/](https://karmada.io/)

如前所述，扩展的云原生架构可能使用不同风格的 Kubernetes。Karmada 就是为这一现实而构建的，因为它是一个实用程序，可跨各种 Kubernetes 集群和云运行应用程序。使用 Karmada 作为集中管理点可以帮助避免供应商锁定并实现混合多云计算安排。Karmada 使用 REST API 服务器运行，该服务器与访问各种云中的集群的控制器进行通信。此过程根据约束和可用资源来调度集群。Karmada 目前是一个由 CNCF 托管的具有开放治理的沙盒项目。
<a name="Ug90D"></a>
## 5、kube-rs
_Kubernetes 的 Rust 客户端。_

- GitHub：[https://github.com/kube-rs/kube-rs](https://github.com/kube-rs/kube-rs)
- 网站：[https://kube.rs/](https://kube.rs/)

与 Kubernetes 的[**Go 客户端**](https://github.com/kubernetes/client-go)类似，kube-rs 为 Kubernetes 自定义资源定义 (CRD) 提供运行时抽象层和宏。该项目使编写与 Rust 中的 Kubernetes API 交互的应用程序变得容易。Kube.rs 将反射器、控制器和自定义资源接口等 Kubrentes 原生元素重新解释为 Rust 语言。查看[**入门指南**](https://kube.rs/getting-started)以开始使用 kube-rs。在撰写本文时，kube-rs 是 CNCF 的沙盒项目。
<a name="CEuAK"></a>
## 6、Open Cluster Management
_用于管理多个 K8s 集群的集中控制平面。_

- GitHub：[https://github.com/open-cluster-management-io/ocm](https://github.com/open-cluster-management-io/ocm)
- 网站：[https://open-cluster-management.io/](https://open-cluster-management.io/)

随着组织部署的 Kubernetes 集群数量的增加，同时管理它们变得越来越困难，更不用说保持对其内部运作的可见性了。跨多个云供应商使用 Kubernetes 的团队加剧了这种情况。开放集群管理 (OCM) 项目是一个有趣的开源计划，旨在提供对多个 Kubernetes 集群的集中控制。借助 OCM，操作员可以从单个控制平面创建和删除多个云上的集群。也可以注册集群并跨多个集群应用标准安全策略。OCM 提供[**clusteradm**](https://github.com/open-cluster-management-io/clusteradm)，一个用于多集群管理的命令行工具。在撰写本文时，Open Cluster Management 是 CNCF 的沙盒项目。
<a name="ML9ur"></a>
## 7、Volcano
_云原生批处理系统。_

- GitHub：[https://github.com/volcano-sh/volcano](https://github.com/volcano-sh/volcano)
- 网站：[https://volcano.sh/en/](https://volcano.sh/en/)

Volcano 是一个开源项目，它为批处理和弹性计算提供组件。创建它是因为 Kubernetes 不提供开箱即用的数据密集型工作负载所需的那种批处理调度功能。该工具有助于在 AI/ML 或基因组学等领域将大数据应用程序作为 Kubernetes 工作负载运行。在撰写本文时，Volcano 是 CNCF 的沙盒项目。
<a name="COedK"></a>
## 想法：开源调度器工具
上面回顾了 CNCF 下一些新兴的容器编排和调度项目。Kubernetes 无处不在的后果影响了许多新软件包的创建，以迎合这种新范式。无论运营商是试图统一不同的 Kubernetes 风格，使用他们选择的语言工作，还是在 Kubernetes 集群中运行数据密集型计算，CNCF 都有一些有趣的项目来满足这些特殊需求。
<a name="ttt4b"></a>
## 翻译
**7 CNCF Tools for Scheduling and Orchestration**
<a name="kH4XJ"></a>
## 参考资料
**Kubernetes: **[https://containerjournal.com/features/when-using-kubernetes-does-and-doesnt-make-sense/](https://containerjournal.com/features/when-using-kubernetes-does-and-doesnt-make-sense/)<br />**Kubernetes: **[https://containerjournal.com/features/kubernetes-separating-the-hype-from-production-success/](https://containerjournal.com/features/kubernetes-separating-the-hype-from-production-success/)<br />**streaming: **[https://containerjournal.com/features/5-cncf-projects-for-streaming-and-messaging/](https://containerjournal.com/features/5-cncf-projects-for-streaming-and-messaging/)<br />**observability: **[https://containerjournal.com/topics/container-management/7-open-source-cloud-native-tools-for-observability-and-analysis/](https://containerjournal.com/topics/container-management/7-open-source-cloud-native-tools-for-observability-and-analysis/)<br />**container runtime: **[https://containerjournal.com/topics/container-networking/4-container-runtime-projects-from-cncf/](https://containerjournal.com/topics/container-networking/4-container-runtime-projects-from-cncf/)<br />**CI/CD: **[https://containerjournal.com/features/6-cncf-projects-for-ci-cd/](https://containerjournal.com/features/6-cncf-projects-for-ci-cd/)<br />**CNCF: **[https://landscape.cncf.io/guide#orchestration-management--scheduling-orchestration](https://landscape.cncf.io/guide#orchestration-management--scheduling-orchestration)<br />**Kubernetes 与 Docker 不同: **[https://containerjournal.com/editorial-calendar/best-of-2021/whats-the-difference-between-docker-and-kubernetes/](https://containerjournal.com/editorial-calendar/best-of-2021/whats-the-difference-between-docker-and-kubernetes/)<br />**多集群管理: **[https://containerjournal.com/features/cncf-radar-highlights-kubernetes-multi-cluster-preferences](https://containerjournal.com/features/cncf-radar-highlights-kubernetes-multi-cluster-preferences)<br />**AI/ML 模型: **[http://tbm-auth.alicdn.com/e99361edd833010b/dSVC55aoHBRio4co9aD/NQYZru9jomF79n3wGnO_277528130570_hd_hq.mp4?auth_key=1646776242-0-0-4aa8790873b3730a8c51365bc7209db7](http://tbm-auth.alicdn.com/e99361edd833010b/dSVC55aoHBRio4co9aD/NQYZru9jomF79n3wGnO_277528130570_hd_hq.mp4?auth_key=1646776242-0-0-4aa8790873b3730a8c51365bc7209db7)<br />**Go 客户端: **[https://github.com/kubernetes/client-go](https://github.com/kubernetes/client-go)<br />**入门指南: **[https://kube.rs/getting-started](https://kube.rs/getting-started)<br />**clusteradm: **[https://github.com/open-cluster-management-io/clusteradm](https://github.com/open-cluster-management-io/clusteradm)
