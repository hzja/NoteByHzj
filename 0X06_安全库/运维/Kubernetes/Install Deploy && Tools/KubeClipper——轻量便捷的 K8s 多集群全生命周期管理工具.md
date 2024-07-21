Kubernetes
<a name="mts0G"></a>
## KubeClipper 的起源
九州云（99cloud）致力于成为开放云边架构领导者，在大量 MEC 边缘计算项目建设过程中，KubeClipper 应运而生。<br />KubeClipper 旨在提供易使用、易运维、极轻量、生产级的 Kubernetes 多集群全生命周期管理服务，让运维工程师从繁复的配置和晦涩的命令行中解放出来，实现一站式管理跨区域、跨基础设施的多 K8S 集群。<br />2022 年 8 月，KubeClipper 正式开源，源代码托管在 GitHub，可以访问 [https://github.com/KubeClipper-labs](https://github.com/KubeClipper-labs) 项目主页了解更多详情。KubeClipper 的吉祥物是一只帅气呆萌、轻盈矫健的小海鸥，logo 是在这只小海鸥的保驾护航下扬帆远航的帆船（Clipper）。帆船取自李清照《渔家傲》里“星河欲转千帆舞”，希望 KubeClipper 能乘风破浪会有时，“蓬舟吹取三山去”！<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662097112121-8bcdc9a8-53a1-41d8-a2a4-8591a3d64d97.png#clientId=u881426cb-81b2-4&from=paste&id=u2221c4e0&originHeight=733&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub59cb4ef-b4fa-4f87-8be9-910d5719f12&title=)
<a name="bHcJe"></a>
## 为什么需要 KubeClipper？
云原生时代，Kubernetes 已毋庸置疑地成为容器编排的事实标准。虽然有诸多辅助 K8S 集群安装和管理的工具，但搭建和运维一套生产级别的 K8S 集群仍然十分复杂。九州云在大量的服务和实践过程中，沉淀出一个极轻量、易使用的图形化界面 Kubernetes 多集群管理工具——KubeClipper。<br />KubeClipper 在完全兼容原生 Kubernetes 的前提下，基于社区广泛使用的 kubeadm 工具进行二次封装，提供在企业自有基础设施中快速部署 K8S 集群和持续化全生命周期管理（安装、卸载、升级、扩缩容、远程访问等）能力，支持在线、代理、离线等多种部署方式，还提供了丰富可扩展的 CRI、CNI、CSI、以及各类 CRD 组件的管理服务。<br />与现有的 Sealos、KubeKey、Kubeasz、KubeOperator、K0S 等 K8S 生命周期管理工具相比，KubeClipper 更贴近开放原生、轻量便捷、稳定易用

| 

 | KubeClipper | Sealos | KubeKey | Kubeasz | KubeOperator | K0S |
| --- | --- | --- | --- | --- | --- | --- |
| 图形化页面 | ✅ | ❌ | ❌ | ❌ | ✅ | ❌ |
| 轻依赖 - 不依赖 ansible 等 | ✅ | ✅ | ✅ | ❌ | ❌ | ✅ |
| 多区域、多集群管理 | ✅ | ❌ | ❌ | ❌ | ✅ | ❌ |
| 支持多版本 K8S、CRI | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
| 离线安装 | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
| 基于 kubeadm 封装 | ✅ | ✅ | ✅ | ❌ | ❌ | ❌ |

更多功能列表和快速体验，请戳：KubeClipper-README<br />九州云自 2012 年创业以来，始终秉持拥抱开源、回馈社区的初心，先后开源了若干明星项目，诸如：

1. 面向 AutoOps 的建木 [https://jianmu.dev/](https://jianmu.dev/)
2. OpenStack 全新仪表盘 Skyline [https://opendev.org/openstack/skyline-apiserver](https://opendev.org/openstack/skyline-apiserver)
3. 面向车路协同的 OpenV2X https://openv2x.org/

KubeClipper 是九州云推出的又一款优秀开源软件 [https://github.com/kubeclipper-labs/](https://github.com/kubeclipper-labs/)，它致力于提供轻便易用，又稳定可靠的 K8S 生命周期管理工具，以降低企业规模化建设容器云的门槛，为企业用户与开发者提供更多便利。希望开源能吸引有兴趣的同学一起参与，与行业协同发展。
<a name="fKyzF"></a>
## KubeClipper 的技术优势
<a name="XekcH"></a>
### 1、易使用：图形化页面
KubeClipper 提供向导式的图形化界面，运维工程师可以通过友好的界面，快速完成一个生产级 K8S 集群和所需组件的安装部署，一键完成集群的扩缩容、备份恢复、升级、插件管理等运维管理操作。KubeClipper 从实践中来，在细节上更懂用户。<br />以创建集群为例，节点配置时，KubeClipper 会自动为控制节点添加污点，同时允许用户更改节点污点、设置节点标签。在生产场景中，用户可以将常用配置保存为模版，在创建时简单配置节点后一键部署，实现集群的分钟级创建。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662097112205-4f3a5350-df32-458b-ba6d-ece0a028f724.png#clientId=u881426cb-81b2-4&from=paste&id=ub14b9d36&originHeight=600&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u316abf40-fee6-4b85-82a4-eb9f487da91&title=)
<a name="SKeBK"></a>
### 2、极轻量：架构简单，少依赖
部署实验用的 KubeClipper 集群，仅需两行命令，兼容多种常用的 Linux 操作系统（CentOS / Ubuntu）。
```bash
curl -sfL https://oss.kubeclipper.io/kcctl.sh | KC_REGION=cn sh -
kcctl deploy  [--user root] (--passwd SSH_PASSWD | --pk-file SSH_PRIVATE_KEY)
```
KubeClipper 没有选择相对笨重的 Ansible（版本依赖复杂，速度较慢，内存占用多等），而是通过更轻量的 kcctl 命令行工具（功能与图形化界面对应），提供对 KubeClipper 平台自身的安装、清除和其他运维管理。在架构设计上，KubeClipper 追求轻量优雅，参考 K8S 架构设计，通过少量服务节点实现对大量工作节点和多 K8S 集群的管理。
<a name="faV3J"></a>
### 3、生产级：易用性和专业性兼顾
KubeClipper 为解决生产场景的问题而生，在追求使用简单的同时，提供更丰富、更灵活的功能和服务。比如 KubeClipper 提供了多种网络环境下、多版本的安装包和镜像拉取，支持 GCR 镜像代理、支持完全离线环境下的 K8S 集群部署和插件安装，支持用户自定义多版本的 K8S、CRI、CNI 部署安装。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662097112138-f5a6ea26-5b06-4be2-a185-0a9ba8ffa3a6.png#clientId=u881426cb-81b2-4&from=paste&id=u2df0df33&originHeight=600&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u13dd2d1a-6ff2-43b3-aa39-712df86b694&title=)
<a name="r75jn"></a>
### 4、面向边缘场景
边缘计算是一个重要的容器技术应用场景，也对 Kubernetes 集群快速部署和管理有更高要求。KubeClipper 通过区域对集群和节点进行逻辑或物理的隔离，更易适配边缘计算场景，同时也更符合企业多数据中心的生产场景。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662097112316-2f8a3ba9-c17d-46bd-8fed-a3020f9b2c41.png#clientId=u881426cb-81b2-4&from=paste&id=u667d837a&originHeight=599&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6aff4fef-9a3e-4f67-9a54-0146351c550&title=)
<a name="Rsd8r"></a>
## KubeClipper 的未来规划
KubeClipper 还处于快速成长阶段，未来会继续保持轻量化、易使用的设计风格，加强对边缘场景的支持，如提供对 K3S、K0S、Kube-Edge 等边缘场景 kubernetes 方案的支持，提供更成熟的生产级解决方案，如提供更丰富的 CNI、CSI 和其他管理插件的支持等。<br />希望能通过 KubeClipper 项目结识更多志同道合的朋友，一起看到这个项目茁壮成长。<br />戳快速入门文档：[https://github.com/kubeclipper-labs/kubeclipper/blob/master/README_zh.md#quick-start](https://github.com/kubeclipper-labs/kubeclipper/blob/master/README_zh.md#quick-start)，立即体验。参与开发贡献或开源活动，有机会获得精美周边。
