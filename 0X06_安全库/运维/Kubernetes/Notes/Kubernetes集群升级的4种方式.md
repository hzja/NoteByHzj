Kubernetes<br />从 Kubernetes 1.19 开始，每个开源版本都提供一年的补丁。需要升级到最新的可用次要版本或补丁版本才能获得安全性和错误修复。但是，如何在不停机的情况下升级基础架构的关键部分呢？在任何环境中升级 Kubernetes 时要考虑的常见模式。<br />不会深入研究执行升级的所有工具和注意事项。如果使用的是集群管理工具或托管 Kubernetes 服务，应该查阅文档以获得最适合环境的选项。还需要注意，某些工作负载和环境可能会限制选择的升级策略。<br />讨论集群升级的一些高级模式：

- 原地升级
- 蓝绿升级
- 滚动升级
- 金丝雀升级

这些模式类似于应用程序升级选项，但由于其潜在的影响可能需要考虑一些独特的因素。升级基础设施可能会产生相当大的成本，具体取决于升级需要多长时间以及规模有多大。
<a name="Re6Hv"></a>
## 控制平面组件
Kubernetes 控制平面由 Kubernetes API Server、etcd 数据库、controller manager,、scheduler以及环境中可能拥有的任何其他控制器（例如云或ingress ）组成。升级 API Server是升级集群的第一步。Kubernetes 将状态存储在 etcd 中，并且随着任何重大应用程序升级，需要确保至少有一个备份，并且已经验证可以恢复备份。在某些情况下，API Server升级可能还需要 etcd 升级。
<a name="aIRUA"></a>
## 数据平面组件
Kubernetes 数据平面由 kubelet、容器运行时以及在集群工作负载中使用的任何网络、日志记录或存储驱动程序组成。对于许多集群，至少需要 kube-proxy 和 CNI 插件更新。数据平面组件可以小于等于 API Server版本。理想情况下，主机操作系统、容器运行时和数据平面组件可以相互独立升级。将这些组件解耦，将确保可以在出现错误修复、新功能或安全补丁时快速升级。
<a name="gSVro"></a>
## Kubernetes 托管服务
如果使用Amazon Elastic Kubernetes Service (EKS)等托管 Kubernetes 服务，则会处理控制平面升级。如果使用的是托管数据平面服务，例如托管节点组 (MNG)，数据平面升级也应该由云提供商自动处理。<br />即使使用托管服务，仍然有责任验证已安装在集群中的工作负载、附加控制器和第三方插件（例如 CNI）。在测试或开发环境中升级集群之前，应测试这些组件的 API 兼容性。<br />在所有这些升级策略中，应该避免在集群升级期间进行应用程序升级。如果可能，请保持工作负载使用相同的版本，以最大限度地减少可能错误地归因于 Kubernetes 升级的故障。还要尽量减少其他潜在问题，例如scheme升级或应用程序 API 兼容性。<br />对于任何 Kubernetes 升级，应该按以下顺序升级组件：

1. 控制平面
2. 数据平面和节点
3. 附加组件
4. 工作负载

这些升级模式将决定如何升级最适合集群和环境的组件。
<a name="bYIA9"></a>
### 1、原地升级—In-Place Upgrade
执行原地升级时，必须格外小心，确保组件保持健康，因为是在当前服务于生产流量的集群上执行工作。原地升级可以包括包更新（例如 yum、apt）、配置管理自动化（例如 Ansible、Chef）或 VM/容器镜像更改。理想情况下，升级将是脚本化和自动化的（包括回滚），但如果这是第一次升级，在开发或测试环境中手动进行升级可能会有所帮助。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640690136835-8ee0f375-f452-4c88-ace3-710e11c6c229.webp#clientId=ued5dbdbb-0d4b-4&from=paste&id=uc9aaf589&originHeight=672&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7780ab0c-66ba-4536-ba2a-4b01ac8c595&title=)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640690136586-aa21afd7-b3fa-4f6b-bc26-e925d31d3c1b.webp#clientId=ued5dbdbb-0d4b-4&from=paste&id=uee947ce5&originHeight=673&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u52b76a9a-f7f8-4e78-9093-88bf0ca6a1b&title=)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640690136849-63e9dc1f-fdc1-41c5-ab3b-c256ce143abb.webp#clientId=ued5dbdbb-0d4b-4&from=paste&id=u6e6cb4c7&originHeight=679&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u53b4e5d5-41bc-472e-9a9b-f04b50235a8&title=)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640690137028-44285343-981c-428d-99bb-539a52308287.webp#clientId=ued5dbdbb-0d4b-4&from=paste&id=u94fd22fb&originHeight=669&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4eb22b8c-6db1-4a97-9cd7-608dcbba742&title=)<br />原地升级意味着所有组件将大致同时升级。如果通过配置管理更改所需的 API Server版本并推送新配置，则所有 API Server在收到新配置后都会升级。这与稍后讨论的滚动升级不同。<br />原地升级的主要好处是：

- 在任何规模下，它都是最快的。
- 如果手动完成，则可以更好地控制组件和升级过程。
- 它很容易适用于多种环境（本地或云）。
- 从基础设施成本的角度来看，它是最便宜的。

根据流程、规模和工具，原地升级可能是能够编写脚本的最直接的方法。脚本可以在本地或在开发集群中进行测试，而无需重新配置集群管理员团队可能无法访问的资源——例如负载均衡器或 DNS。<br />如果要使用此方法进行升级，原地升级还需要考虑以下限制：

- 如果所有 API Server或控制器同时升级，则可能导致停机。
- 如果想从 Kubernetes 1.16 迁移到 1.20，必须将整个集群四次升级到每个次要版本。
- 验证每个步骤可能是一个手动过程，这可能会增加额外的时间和出错的机会。
- 应该在失败的情况下测试回滚计划，因为某些升级无法轻松恢复。（例如，scheme更改）。
<a name="EE2oT"></a>
### 2、蓝/绿升级
蓝/绿集群升级需要使用新版本的 Kubernetes 创建第二个集群。需要部署新的控制平面和数据平面，然后将所有工作负载复制到新集群，然后再将流量从旧集群切换到新集群。可以使用蓝/绿来更新集群的每个组件，但整体集群升级更易于部署和回滚。<br />![](https://cdn.nlark.com/yuque/0/2021/gif/396745/1640690136844-2937120f-c911-4409-9d47-81f826691036.gif#clientId=ued5dbdbb-0d4b-4&from=paste&id=udb794f55&originHeight=543&originWidth=670&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua3d5c4af-2bcf-4f3c-aac5-3d9490f4e04&title=)<br />好消息是，设置新集群通常比升级集群更容易。关于如何将工作负载部署到新集群，有多种选择。如果工作负载已经是 GitOps 或持续交付的一部分，可以在升级之前或期间将部署同时转到新集群和旧集群。如果没有自动部署，可以使用Velero 之类的工具来备份现有的工作负载并将它们部署到新集群。<br />创建新的“Green”集群可以对新版本按预期工作充满信心，并控制何时切换版本。新集群还可用于验证自动化工具，例如 Terraform 模块或 GitOps 存储库。可以随时通过 DNS 或负载均衡器进行更改，甚至可以在维护时段或低利用率期间进行更改。<br />蓝/绿升级的主要好处是：

- 在发送流量之前预先验证所有组件是否正常。
- 可以一次升级多个版本（例如，从 1.16 直接升级到 1.20）。
- 可以更改可能难以测试的基础架构的其他部分（例如，切换区域、添加区域、更改实例类型）。
- 回滚是最安全和最容易的。

蓝/绿部署要考虑的缺点包括：

- 这是基础架构成本中最昂贵的策略，因为必须在迁移期间运行两倍的计算容量。
- 如果有数千个工作程序节点，可能无法获得运行完整的第二个集群所需的所有计算容量。
- 如果有多个并发集群升级，则此策略很难扩展到数十个或数百个集群。
- 除非有备用服务器，否则在没有虚拟化的情况下在本地实现蓝/绿并不容易。
- 如果有很多端点要更新，一次切换所有流量可能并不容易。负载均衡器可能需要预先调整并预热缓存。请注意 DNS 生存时间 (TTL)，它可能会或可能不会用于分散负载。
- 一次切换所有集群流量需要跨团队协调迁移到新集群；以及工程周期来验证工作负载的规模是否正确。

当拥有较少数量的集群或少于几百个工作节点时，蓝/绿可能是一个很好的策略。它允许跳过版本并且回滚是安全的，但它可能会需要更多的基础设施支出和协调时间。
<a name="EOMlv"></a>
### 3、滚动升级
如果熟悉 Kubernetes 部署策略，就会熟悉滚动升级。滚动升级将部署组件的一个升级为新副本，然后缩减一个旧副本。它将继续这种模式，直到所有旧组件都被删除。滚动升级的增量性质比原地升级和蓝/绿策略有一些优势。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640690137027-5ad9d3b9-ca64-43b3-b340-dc68f8d5551e.webp#clientId=ued5dbdbb-0d4b-4&from=paste&id=u9c0e9682&originHeight=680&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u89b18e2e-fadf-4a16-8a70-51ec1aebb06&title=)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640690137506-6ac923fa-eda7-4e91-9f2b-836fa577189d.webp#clientId=ued5dbdbb-0d4b-4&from=paste&id=u8452e419&originHeight=665&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8c5fa919-57fe-472f-b902-8d78be0d242&title=)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640690137236-f7191a91-af80-44ca-a616-81fddecc509b.webp#clientId=ued5dbdbb-0d4b-4&from=paste&id=u8e526005&originHeight=673&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ufeaa8c61-3973-4266-8cf2-9332c45e8f8&title=)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640690137430-5f9a7e44-ca02-42b0-a5bf-3dfc2e2e57b5.webp#clientId=ued5dbdbb-0d4b-4&from=paste&id=ue1e2912e&originHeight=678&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf074e078-d562-421a-b266-debe85babb8&title=)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640690137862-be2e73bb-6033-4394-b885-68dbda571171.webp#clientId=ued5dbdbb-0d4b-4&from=paste&id=u5e8408db&originHeight=672&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u83166a07-fc59-4626-89b6-a26fec43c9a&title=)<br />与原地升级类似，需要一次升级 Kubernetes 的一个次要版本。当需要升级多个版本时，这可能是额外的工作，但它是唯一受支持的选项。根据要升级的组件，可以使用不同的工具来升级每个组件。<br />对于像控制平面这样的资源，可能希望将带有升级的 API Server的新服务器添加到控制平面，然后关闭旧服务器。如果使用的是 AWS，则可以更改 Auto Scaling 组启动配置 AMI 并一次替换一个实例。其他控制平面组件（例如调度程序）可能在集群内作为容器运行，因此可以使用标准的 Kubernetes 滚动部署升级来升级这些组件。<br />与蓝/绿相比，滚动升级的主要区别在于外部流量路由（DNS 和负载均衡器）将保持指向同一位置。在进行生产集群升级之前，需要确保在不同的集群或环境中测试所有附加组件和工作负载。<br />请注意，AWS 托管节点组、kOps、Cluster-API和许多其他 Kubernetes 集群管理工具使用滚动升级策略。好处包括：

- 与原地升级相比，更安全的更新和回滚。
- 成本低于蓝色/绿色，并且资源耗尽的可能性较小。
- 如果出现问题，可以在升级过程中暂停。
- 可以在本地环境模拟。

滚动升级是最常见的自动化工具。它们在速度和成本之间取得了很好的平衡，也减少手动工作和风险。<br />升级生产集群时，现有的所有工作负载仍将被部署；只要测试了它们的兼容性，升级就应该是可自动化的。<br />使用滚动升级时的进一步考虑包括：

- 滚动升级可能会很慢，具体取决于规模。
- 在升级期间，可能需要协调控制器、守护进程或插件升级。
- 可能无法进行集群范围的更改，例如添加可用区或更改架构。
<a name="u00fx"></a>
### 4、金丝雀升级
Canary 应用程序部署一次为应用程序的新版本提供少量流量。Canary 升级可以被认为是具有蓝/绿优势的滚动升级。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640690137897-26900e40-04e8-41b0-820a-8c23a062aea2.webp#clientId=ued5dbdbb-0d4b-4&from=paste&id=ub72e79ac&originHeight=902&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf76fd223-f512-4e5b-a209-3f4e6cc9270&title=)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640690137965-691fd833-e872-4867-ad0e-c37841115b4e.webp#clientId=ued5dbdbb-0d4b-4&from=paste&id=u31ba252e&originHeight=910&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u312e1dcd-74ee-4444-8fdb-e59d760e157&title=)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640690138205-36c16eba-3ffa-4c3e-a040-78c63e161619.webp#clientId=ued5dbdbb-0d4b-4&from=paste&id=ud18fb663&originHeight=902&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua4402434-73c4-4b78-9aca-5787631aed5&title=)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640690138284-7e469793-1c79-4072-91f6-4f79f531ba65.webp#clientId=ued5dbdbb-0d4b-4&from=paste&id=u2a092e92&originHeight=910&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u760abd71-9da5-488d-b5fd-276a129239b&title=)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640690138672-3b95b324-7a47-4124-924f-6267cc114561.webp#clientId=ued5dbdbb-0d4b-4&from=paste&id=u6e9fbef8&originHeight=915&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u77857a86-23a4-4081-b7ca-4f7e06ef1a0&title=)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640690138661-fac1e0f7-385e-439a-a995-83a2f696aaa2.webp#clientId=ued5dbdbb-0d4b-4&from=paste&id=uee0de969&originHeight=905&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u25f9ee07-6be7-421d-9b88-80041673a94&title=)<br />通过 Canary 升级，将使用要部署的版本创建一个新的 Kubernetes 集群。然后添加一个小型数据平面并将现有的应用程序以较小的规模部署到新集群。通过负载均衡器配置、DNS 循环或服务网格将新的集群工作负载添加到现有的生产流量中。<br />现在，可以监控流向新集群的流量，慢慢扩展新集群中的工作负载并缩减旧集群中的工作负载。可以一次完成一项工作，并且可以根据自己的习惯缓慢或快速完成。如果任何单个工作负载开始出现错误，可以缩减新集群中的单个工作负载，使其自动使用旧集群。<br />Canary 集群升级的好处包括：

- 新集群更容易创建和验证。
- 可以在升级期间跳过次要 Kubernetes 版本（例如，1.16 到 1.20）。
- 可以在每个团队的基础上选择加入应用程序部署。
- 由于增加的流量使用，错误的影响最小。
- 可以在升级期间进行大型基础架构更改。
- 集群从小规模开始，因此基础设施成本较低，可以在扩展时预热缓存和负载均衡器。

如果想进行较大的更改（例如更改架构）或者想添加额外的可用区，那么 Canary 是一个不错的选择。通过启动较小的集群并根据工作负载增加它，可以确保在新实例更高效或工作负载请求和限制发生变化时不会过度配置基础设施。<br />与任何事情一样，需要权衡取舍。使用金丝雀部署时，应该注意以下一些问题：

- 回滚应用程序可能需要手动干预来更改负载均衡器或缩小新集群的规模。
- 调试应用程序可能更难，需要知道发生了哪些集群错误。
- 如果有数十个或数百个集群，随着集群的升级，集群数量可能会增加 50% 或更多。
- Canary 是最复杂的升级策略，但它受益于自动化部署、健康检查和性能监控。
<a name="upDkI"></a>
## 结论
无论选择哪种升级策略，重要的是要了解它们的工作原理以及随着 Kubernetes 使用量的增长可能出现的任何问题。需要有一个升级策略，因为 Kubernetes 有频繁的发布和偶尔的错误。<br />与新版本保持同步可能是基础设施安全流程的重要组成部分，并使应用程序能够快速利用新功能。如果部署了 Kubernetes 并迁移了所有工作负载，而没有考虑如何升级，那么现在是开始计划的最佳时机。<br />如果没有运行自己的 Kubernetes 集群的业务需求，建议使用可用的托管 Kubernetes 选项之一。选择托管控制平面和数据平面可以每年节省数天或数周的规划和升级时间。每个托管选项可能执行不同的升级，但它们专注于工作负载和业务价值，而不是控制平面高可用性或数据平面兼容性。
