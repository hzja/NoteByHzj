Kubernetes
<a name="W41gm"></a>
## KubeSphere
KubeSphere 愿景是打造一个以 Kubernetes 为内核的 云原生分布式操作系统，它的架构可以非常方便地使第三方应用与云原生生态组件进行即插即用（plug-and-play）的集成，支持云原生应用在多云与多集群的统一分发和运维管理。KubeSphere 也是一个多租户容器平台，提供全栈的 IT 自动化运维的能力，简化企业的 DevOps 工作流。KubeSphere 提供了运维友好的向导式操作界面，帮助企业快速构建一个强大和功能丰富的容器云平台。<br />官方：[https://kubesphere.io/](https://kubesphere.io/)<br />运行以下命令以在现有 Kubernetes 集群上安装 KubeSphere：
```bash
kubectl apply -f https://github.com/kubesphere/ks-installer/releases/download/v3.4.0/kubesphere-installer.yaml
   
kubectl apply -f https://github.com/kubesphere/ks-installer/releases/download/v3.4.0/cluster-configuration.yaml
```
<a name="ZMuX6"></a>
## Rancher
Rancher是一个开源软件平台，使组织能够在生产中运行和管理Docker和Kubernetes。使用Rancher，组织不再需要使用一套独特的开源技术从头开始构建容器服务平台。Rancher提供了管理生产中的容器所需的整个软件堆栈。![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1694654857372-3b3332be-3edd-42bd-9920-1df946eb9b7a.png#averageHue=%23dadada&clientId=u03597dfc-3dbd-4&from=paste&height=502&id=ud9b766c3&originHeight=1254&originWidth=2094&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=155111&status=done&style=none&taskId=u517d88e0-7e5b-4ee2-aa00-c63cf5ed02a&title=&width=837.6)它解决了在任何基础架构上管理多个Kubernetes集群的运营和安全挑战，同时为DevOps团队提供了用于运行容器化工作负载的集成工具。<br />官网：[https://rancher.com/](https://rancher.com/)<br />安装
```bash
docker run --privileged -d --restart=unless-stopped -p 80:80 -p 443:443 rancher/rancher
```
<a name="BInCn"></a>
## Lens
Lens 是为那些每天使用 Kubernetes 的人设计的 IDE。它可以探索和导航 Kubernetes 集群，而无需学习 kubectl 命令，实时检查实时统计信息、事件和日志流。<br />它将所有集群、服务、工作负载、工具、自动化和相关资源整合在一起，以便于访问。Lens 与 Prometheus 集成，可视化并查看资源使用指标的趋势，包括 CPU、内存、网络和磁盘，以及总容量、实际使用情况、请求和限制。每个 k8s 资源都会自动生成详细的可视化信息。Lens 带有 Helm 图表管理，允许发现和快速部署数千个公开可用的 Helm 图表并管理自己的存储库。浏览已安装的 Helm 图表及其修订版，只需单击一下即可升级。<br />Lens 是适用于 MacOS、Windows 和 Linux 操作系统的独立应用程序。<br />官网：[https://k8slens.dev/](https://k8slens.dev/)
<a name="kHuOy"></a>
## K9S
k9s 是一个基于 curses 的全屏终端 UI 管理工具，可以与 Kubernetes 集群进行交互，可以观察系统资源，在各种资源之间切换，检查清单、日志、监控事件并执行 Pod 等，从而确保桌面空间不至于被大量终端窗格所占据。![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1694655091552-3d3ad838-f48b-41bc-95de-83aebd8a7d49.png#averageHue=%23d2d2d2&clientId=u03597dfc-3dbd-4&from=paste&height=668&id=u03910fad&originHeight=1670&originWidth=2597&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=1057304&status=done&style=none&taskId=u3a198cd6-9408-4f68-b648-6671cfcee39&title=&width=1038.8)<br />k9s 会以特定时间间隔监控 Kubernetes 资源，默认为 2 秒，并允许查看自己集群中的内容。它可以一目了然地提供了运行中 Pod、日志和部署的可视化视图，以及对 Shell 的快速访问。<br />官网：[https://k9scli.io/](https://k9scli.io/)
<a name="hXxsq"></a>
## Shipyard
Shipyard 是一个基于 Web 的 Docker 管理工具，支持多 host，可以把多个 Docker host 上的 containers 统一管理；可以查看 images，甚至 build images；并提供 RESTful API 等等。Shipyard 要管理和控制 Docker host 的话需要先修改 Docker host 上的默认配置使其支持远程管理。<br />官网：[https://shipyard-project.io/](https://shipyard-project.io/)
<a name="ke0Wp"></a>
## Kubernetic
Kubernetic 是一个强大的 kubernetes IDE,可以实时查看kubernetes集群状态,Pod实时日志，集群故障等，可以更方便快捷地使用你的集群，从根本上提高工作效率和业务迭代速度。![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1694655222656-55523e0d-3b7a-4196-808c-bcee15716590.png#averageHue=%233b81f4&clientId=u03597dfc-3dbd-4&from=paste&height=666&id=uf9fb3148&originHeight=1666&originWidth=2970&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=295897&status=done&style=none&taskId=u8275cf26-5deb-4f4e-9c77-20a7413ad13&title=&width=1188)<br />官网：[https://kubernetic.com/](https://kubernetic.com/)
<a name="t0a11"></a>
## Grafana
一款监控和可视化工具，Grafana也可以与K8S集成，提供对集群的可视化展示。通过Grafana的仪表盘功能，你可以创建和定制自己的K8S集群监控视图。<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1694655496222-82dbf89c-8810-409e-9311-88e2e5e24a64.png#averageHue=%23161210&clientId=u03597dfc-3dbd-4&from=paste&height=77&id=u32519a7b&originHeight=192&originWidth=662&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=14319&status=done&style=none&taskId=uac562e03-fc24-4e76-9213-d9defe166bc&title=&width=264.8)<br />官网：[https://grafana.com/](https://grafana.com/)![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1694655397586-3cc6beeb-1637-411e-9d68-5b59bf35ccef.jpeg#averageHue=%23222526&clientId=u03597dfc-3dbd-4&from=paste&id=ue497b620&originHeight=1662&originWidth=2954&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u170477df-9093-4462-a64c-8f6f8489f08&title=)
<a name="E7lfD"></a>
## Kuboard
Kuboard 是一款专为 Kubernetes 设计的免费管理界面，兼容 Kubernetes 版本 1.13 及以上。Kuboard 每周发布一个 beta 版本，最长每月发布一个正式版本，经过两年的不断迭代和优化，已经具备多集群管理、权限管理、监控套件、日志套件等丰富的功能，并且有 1000+ 的企业将 Kuboard 应用于其生产环境。![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1694655785607-fed2b980-0d04-4d56-89f4-38f3fcab85f9.png#averageHue=%230666dc&clientId=u03597dfc-3dbd-4&from=paste&height=364&id=u3a696058&originHeight=910&originWidth=1109&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=317544&status=done&style=none&taskId=ue64d4087-b599-40c8-821b-fbcb709d3cf&title=&width=443.6)<br />Kuboard 自 2019年8月发布第一个版本以来，得到了众多用户的认可，目前已经获得了 10000+ GitHub Star。![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1694656201809-8de7071e-b926-460d-a3c8-2cec8edbe767.png#averageHue=%233ef097&clientId=u03597dfc-3dbd-4&from=paste&height=707&id=u9c90467a&originHeight=1768&originWidth=3840&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=1314649&status=done&style=none&taskId=u62fff772-82d4-41b5-be50-d526c8dc133&title=&width=1536)![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1694656458648-05535b51-b26c-49ff-adc1-3348c95c6fb5.png#averageHue=%2319c094&clientId=u03597dfc-3dbd-4&from=paste&height=707&id=u197f5f2d&originHeight=1768&originWidth=3840&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=372915&status=done&style=none&taskId=u5c5ca141-f88e-4996-a207-a4dafbfddea&title=&width=1536)<br />官网：[https://kuboard.cn/](https://kuboard.cn/)<br />开源地址：[https://github.com/eip-work/kuboard-press](https://kuboard.cn/)
<a name="KkNLe"></a>
## Kubevious
Kubevious ([https://github.com/kubevious/kubevious](https://github.com/kubevious/kubevious)) 是一个开源的 Kubernetes Dashboard，但是和主流的 Dashboard 却不太一样，可以说非常有特色，他将应用程序相关的所有配置都集中在一起，这可以大大节省操作人员的时间，其实这都不是最主要的，主要的是他具有一个 TimeMachine（时光机）功能，允许回到之前的时间去查看应用的错误信息。![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1694656373476-be790b04-ddac-47a3-9d3d-5defbb4c8265.png#averageHue=%23261869&clientId=u03597dfc-3dbd-4&from=paste&height=707&id=u3ff3aad2&originHeight=1768&originWidth=3840&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=611779&status=done&style=none&taskId=uaa4f0d2a-c82a-4683-b4d2-8028bcc5606&title=&width=1536)<br />官网：[https://kubevious.io/](https://kubevious.io/)
<a name="oPOZG"></a>
## Octant
官方解释：A web-based, highly extensible platform for developers to better understand the complexity of Kubernetes clusters.基于Web的高度可扩展平台，供开发人员更好地了解Kubernetes集群的复杂性。<br />[![](https://cdn.nlark.com/yuque/0/2023/svg/396745/1694656598644-86e9d8d2-5b19-470f-b65c-0a1545195225.svg#clientId=u03597dfc-3dbd-4&from=paste&id=uf2dc6cb3&originHeight=109&originWidth=300&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ued50064f-6745-4510-863c-53dc256d2c5&title=)](https://octant.dev/)<br />作为一款开源的K8S可视化工具，Octant提供了简洁而功能丰富的用户界面，支持自定义插件和扩展，助你更好地查看和管理K8S集群。<br />官网：[https://octant.dev/](https://octant.dev/)
<a name="my2ux"></a>
## Kontena Lens
Kontena Lens是Kubernetes的智能仪表板。作为控制Kubernetes唯一需要的管理系统。它可免费用于Mac OS，Windows和Linux操作系统。应用程序启动后，将在界面中看到所有相关集群的列表。对于确实需要每天处理Kubernetes的用户来说，它是功能最强大的IDE。可以确保正确设置和配置集群，并且可以更轻松、更快速地使用集群，并从根本上提高生产率和业务速度。<br />Kontena Lens IDE主要功能包括可以一次管理多个集群，实时可视化集群状态，提供内置终端;安装非常简单，因为它是独立的应用程序;支持Kubernetes RBAC。经过测试可处理集群中近25000个Pod。<br />官网：[https://www.kontena.io/lens](https://www.kontena.io/lens)<br />开源地址：[https://github.com/multiservicio/lens](https://github.com/multiservicio/lens)
<a name="PGMTq"></a>
## Loodse Kubermatic
Kubermatic能够为其提供统一的管理界面，用于跨AWS、微软Azure、谷歌云等云平台和本地基础架构管理Kubernetes集群。![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1694657180529-f152b799-eb82-455f-bac8-a23e6b5ba9d4.png#averageHue=%23011a32&clientId=u03597dfc-3dbd-4&from=paste&height=707&id=u7b41695f&originHeight=1768&originWidth=3796&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=1178685&status=done&style=none&taskId=uae88758f-e4ff-42ac-a746-7ee8f43b66b&title=&width=1518.4)Kubermatic几分钟即可帮助企业建立基础架构。安装后，管理员可以开始使用它根据应用程序需求来扩大或缩小集群，创建新集群并实施数据保护配置。该平台具有高可用性模式，该模式允许Kubernetes环境自动从中断中恢复。<br />官网：[https://www.kubermatic.com/](https://www.kubermatic.com/)
<a name="xEFmH"></a>
## Portainer
Portainer 是一个轻量级的管理界面，可以让您轻松地管理不同的 Docker 环境（Docker 主机或 Swarm 集群）。Portainer 提供状态显示面板、应用模板快速部署、容器镜像网络数据卷的基本操作、事件日志显示、容器控制台操作、Swarm 集群和服务等集中管理和操作、登录用户管理和控制等功能。![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1694657357617-b6ebbf32-c559-47b4-a97e-12af66933816.png#averageHue=%23fefefe&clientId=u03597dfc-3dbd-4&from=paste&height=680&id=uaeb29031&originHeight=1701&originWidth=3799&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=401969&status=done&style=none&taskId=ub10423ef-8b43-46b2-bb80-74860fd9b41&title=&width=1519.6)<br />功能十分全面，基本能满足中小型单位对容器管理的全部需求。<br />Portainer：[https://www.portainer.io/](https://www.portainer.io/)
<a name="ayWUm"></a>
## Weave Scope
Weave Scope 用于监控、可视化和管理 Docker 以及 Kubernetes。<br />Scope 提供了至上而下的集群基础设施和应用的完整视图，用户可以轻松对分布式的容器化应用进行实时监控和问题诊断,以确保容器应用程序的稳定性和性能，通过查看容器上下文的度量/标记，以及原数据，在容器内部的进程之间可以轻松的确定运行服务消耗最多CPU/内存资源的容器，<br />官网：[https://www.weave.works/oss/scope/](https://www.weave.works/oss/scope/)<br />Weave Scope 这个项目会自动生成容器之间的关系图，方便理解容器之间的关系，也方便监控容器化和微服务化的应用。
