<a name="YJrV0"></a>
## Kubernetes
<a name="RApRo"></a>
## 1、背景
随着 Kubernetes 成为云原生时代容器编排的事实标准，社区统一使用 CRD + Controller 的方式来扩展 Kubernetes 的能力，开发云原生应用程序。这也就是 Operator 的实现。而构建 Operator 的门槛相对比较高，开发者需要了解 Kubernetes 的知识并具备相关的经验。<br />Operator Framework 的出现为开发制定了一整套框架，它整合了很多 Kubernetes 的专业知识，提供了一整套标准化工具方便在 Kubernetes 上轻松地创建应用程序。**其中 Operator Lifecycle Manager，简称 OLM，用来管理 Operator 的生命周期。**OLM 扩展了 Kubernetes 的能力，遵循 Kubernetes 交互设计理念，提供声明式的方式来安装、更新和管理 Operator 及其在集群中的依赖。<br />Operator 和 OLM 的关系，就如同在操作系统里，通过软件包管理去安装更新软件。OLM 在设计时，自身组件也是用 Operator 方式进行部署。接下来着重介绍 OLM 的模型定义、组件和安装使用。
<a name="kw1uj"></a>
## 2、模型
OLM 主要定义了以下资源对象模型：
<a name="znBZS"></a>
### 2.1 CatalogSource
**目录源可以理解成 Operator 仓库**，它用来存储 Operator 的元数据信息，包括了集群服务版本信息、自定义资源信息、软件包信息。可以查询该目录源下有哪些 Operator 可以提供使用。以下示例是 OLM 默认安装提供的目录源，利用社区 Operator 这个 repo 构建出的镜像。该 repo 存放了大量社区 Operator 的元数据信息，并且各个 Operator 的开发者也会不定期地更新该仓库，保证最新开发的 Operator 能够提供给社区使用。官方 OperatorHub 也使用了这个 Operator 仓库。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675947972556-c2c5f588-518d-4c63-8e6d-f275a799e700.png#averageHue=%23242221&clientId=u4e9a6eaf-cf8f-4&from=paste&id=udb4378a7&originHeight=720&originWidth=1032&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u16533cab-3cb1-43fb-a259-7cd1078d5b6&title=)
<a name="FgabM"></a>
### 2.2 Subscription
**订阅表示要在某个目录源下，安装指定版本的 Operator。**通过订阅某个频道，获取 Operator 的版本更新信息，以及在安装时指定自动或者手动的批准策略。不同的批准策略会影响 Operator 的安装升级。另外在订阅信息里可以添加一些配置，如环境变量、文件挂载等，用于最终 Operator 控制器的部署。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675947972547-07251cd0-8ec3-4c76-abeb-8c85f797ec4e.png#averageHue=%23222120&clientId=u4e9a6eaf-cf8f-4&from=paste&id=u6e459eee&originHeight=720&originWidth=798&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ueb7d48ad-100d-484f-a95c-671a79169e2&title=)
<a name="IoNYu"></a>
### 2.3. InstallPlan
**在安装 Operator 时，会创建一个如上所述的订阅对象，然后会创建安装计划。**安装计划表示安装 Operator 或者升级 Operator 到特定版本时，需要创建的一组资源。安装版本就是 ClusterServiceVersion 的对应版本。这里会根据上面订阅对象里定义的不同批准策略，进行以下操作：

- 当设置为自动批准策略，则安装计划会自动执行，OLM 会自动管理并升级 Operator，确保集群始终运行最新版本。
- 当设置为手动批准策略，需要由管理员或者拥有批准权限的用户进行批准，方可执行。

安装计划会创建指定的资源，这里主要包括 ClusterServiceVersion、CRD 和 Operator SDK 打包 Bundle 里的其他一些依赖资源，并在订阅所在的命名空间下安装 Operator。**这里需要特别注意，当有一个 Operator 正在进行审批时，其他的 Operator 无法直接安装。**<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675947972570-5a95700c-cca6-4bc7-a2c3-fd1dfdfbc879.png#averageHue=%23222120&clientId=u4e9a6eaf-cf8f-4&from=paste&id=u0f32ca35&originHeight=720&originWidth=761&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u59274f13-d124-4aaa-9247-457a2b5563c&title=)
<a name="HWpeF"></a>
### 2.4. OperatorGroup
**Operator 组为 Operator 的安装提供多租户支持。**当开发 Operator 时，可以设置该 Operator 部署集群后的作用范围，也就是通常说 watch 哪些命名空间。可以在 ClusterServiceVersion 里设置具体的安装模式。OLM 默认会在 operators 命名空间下安装一个 global-operators 的 Operator 组，该组定义安装在所有命名空间。当选择安装某个 Operator 时，根据不同的安装模式，来确定是否创建 Operator 组。分为以下几种情形：

- 可以将全局作用范围的 Operator 安装在 operators 命名空间下。
- 可以在其他命名空间下创建一个全局的 Operator 组，然后进行 Operator 安装。
- 针对只作用于某些命名空间的 Operator，可以创建 Operator 组，定义好要选择安装 Operator 的命名空间列表。OLM 会在这些命名空间下创建 Operator 所需要的 RBAC 权限。

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675947972599-0f821275-6175-42a9-bb76-8e7ada4d879f.png#averageHue=%23222020&clientId=u4e9a6eaf-cf8f-4&from=paste&id=u98372885&originHeight=720&originWidth=1000&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8e16e655-fef3-44e1-8b03-3c105c463c1&title=)
<a name="DBErG"></a>
### 2.5. ClusterServiceVersion
**集群服务版本，简称 CSV，记录了某个 Operator 具体的版本信息。**它提供了 Operator 元数据清单信息，主要包括了 Operator 的名称、版本、图标、描述、安装模式、能力级别、维护者信息、安装所需资源并提供 Operator 使用的一些 sample。在发布新版本 Operator 时会提供版本更新策略。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675947972610-461c3324-9728-4b96-a898-ac8b38bda15e.png#averageHue=%23222020&clientId=u4e9a6eaf-cf8f-4&from=paste&id=u5a13aa5c&originHeight=794&originWidth=720&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u85eadd66-ebc3-46d8-bc1e-f0d93f28bc5&title=)
<a name="fmbcK"></a>
## 3、架构
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675947973060-0e2bf021-59bc-49aa-be07-21c15659b533.png#averageHue=%23adaaa6&clientId=u4e9a6eaf-cf8f-4&from=paste&id=u7a04135f&originHeight=613&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0d3882fd-5217-4cfd-9d45-6f11237ec06&title=)<br />如图所示，OLM 的组件包括 Package Server、Operator Registry、Catalog Operator、OLM Operator。<br />**Package Server：**会在集群上注册 packagemanifest 的 APIService，通过 gRPC 协议调用 operator registry 服务，获取 package 信息，并生成一个个 packagemanifest 对象。<br />**Operator Registry：**上面 catalogsource 创建后，运行的 pod 其实就是 registry 服务。该服务存储了 CSV、CRD、软件包和频道的元数据信息。package manifest 是 registry 中的一个实体对象，软件包里会包含多个频道，每个频道会指向特定的 CSV 版本。各个 CSV 版本之间会明确替换关系以及升级规则，这样可以完成 Operator 每个中间版本的替换和安装，最终达到运行频道中最新版本的目的。<br />**Catalog Operator：**负责目录源的创建，并负责完成 Operator 部分资源的安装。主要包括以下步骤：

1. 监听目录源，可以定义不同类型的 catalogsource 资源对象。
2. 当目录源是 gRPC 类型时，可以使用 catalogsource 镜像去运行 registry server 的 pod 或者直接指定一个已存在 registry server 的 gRPC 地址。当目录源是 configmap 或 internal 类型时，会创建 configmap server。
3. 由上述 Package Server 组件完成相应的功能。
4. 安装 Operator 时，会创建订阅，订阅信息里会指定需要安装 Operator 的版本。此时 Catalog Operator 控制器会运行 bundleUnpack job，该 job 会运行 pod 去解压指定 Operator 的 bundle 镜像，将 bundle 镜像里Operator 的元数据信息，写入到 configmap 资源对象里。
5. 控制器会为订阅创建相对应的 installplan 资源对象。
6. 基于 configmap 里提供的 Operator 元数据信息，调和 installplan 对象，添加需要安装的资源信息。包括 CSV 资源、CSV 里定义的依赖资源如 CRD、APIService 和 webhook、RBAC 相关资源以及 Operator 运行依赖的其他资源对象。
7. 根据 installplan 对象里的安装内容，进行资源对象的创建。

另外控制器会监听目录源中，安装包对应的频道版本是否有更新，当有更新时，同样会创建出新版本的 CSV 资源和 installplan 对象，以便进行更新升级。<br />**OLM Operator：**在 Catalog Operator 控制器完成一些资源对象创建后，OLM Operator 的控制器主要调和 CSV 资源和 Operator 组资源，执行以下逻辑：

8. OLM Operator 控制器会处理 CSV 的版本升级，并保持使用最新的版本。
9. 当 Operator 作用在所有命名空间时，会将安装命名空间下的 role、rolebinding 提升到 clusterrole 和 clusterrolebinding，这样可以拥有集群管理的权限。
10. Operator 作用在指定多个命名空间或所有命名空间时，根据 olmconfig 里 disableCopiedCSVs 的功能配置，在其他命名空间下拷贝安装的源 CSV 去创建新的 CSV。
11. 根据不同的安装模式会选择创建 Operator 组，当 Operator 作用在指定多个命名空间时，会到这些命名空间下创建 Operator 运行需要的 RBAC 权限。
12. 根据订阅信息里的 config 配置和 CSV 里定义的 deployment 的安装信息，由 OLM Operator 完成每个 Operator 控制器部署。
<a name="tqBsK"></a>
## 4、安装
在了解 OLM 的功能和基础架构后，可以看一下 OLM 的安装。这里以 0.21.2 版本为例，提供脚本和命令行安装两种方式。
<a name="S9svZ"></a>
### 4.1. 脚本
通过执行以下命令来安装 OLM。
```bash
# curl -Lhttps://github.com/operator-framework/operator-lifecycle-manager/releases/download/v0.21.2/install.sh -o install.sh# chmod +x install.sh# ./install.sh v0.21.2
```
<a name="Xn9Bk"></a>
### 4.2. 命令行
用户只需要安装好 Operator SDK 命令行工具，执行以下命令便可在集群上进行 OLM 的安装。
```bash
# operator-sdk olm install --version v0.21.2
```
大概的安装过程如下：

1. 部署 OLM 资源对象模型，即各个自定义资源对象 CRD；

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675947973086-caa50b25-84c0-445e-8087-672bf5563b61.png#averageHue=%23140f0f&clientId=u4e9a6eaf-cf8f-4&from=paste&id=u2c1afc6e&originHeight=300&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubf64f983-402f-4c9d-8b48-0140bbc2f45&title=)

2. 部署 OLM olm-operator 和 catalog-operator 组件，创建名为 olm-operator 和 catalog-operator 的 deployment ；
3. 部署 OLM package-server 组件，先创建名为 packageserver 的 CSV，CSV 创建后会部署一个名为 packageserver 的 deployment；

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675947973160-97ceddc6-a1b7-46e3-9f6e-d542e70ae20e.png#averageHue=%230a0a0a&clientId=u4e9a6eaf-cf8f-4&from=paste&id=u36a2ae3a&originHeight=216&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ufcc7a297-7772-4638-8788-db9ea16727c&title=)

4. 部署一个 operator-registry组件，先创建名为 operatorhubio-catalog 的 catalogsource CR，catalog-operator 组件 watch 该资源并使用 catalogsource CR 里指定镜像生成 pod，该 pod 提供50051端口的 gRPC 服务；

安装完后，可以调用 packagemanifest 的 API，获取到目录源提供了哪些 Operator。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675947973205-a7fe93d1-cd52-485b-abbd-f3dd497d2e61.png#averageHue=%230a0a0a&clientId=u4e9a6eaf-cf8f-4&from=paste&id=u43ea28fa&originHeight=503&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua09b5a0f-6f21-4d58-95b9-582343f5814&title=)
<a name="lfnU1"></a>
## 5、使用
在完成 OLM 默认安装后，用户可以去安装自己需要的 Operator。如安装Elasticsearch Operator，去创建一个订阅信息。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675947973304-af2c3d5f-e6c0-4a85-a5ea-b82dc00aa4b5.png#averageHue=%23242221&clientId=u4e9a6eaf-cf8f-4&from=paste&id=u3bcb0cf0&originHeight=669&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u378dd3e9-2f66-4bb6-8a44-1044ada540f&title=)<br />创建一个 Operator 组，只在 eck1 和 eck2 命名空间下安装Elasticsearch Operator。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675947973662-3dc924fc-8404-4dd2-a58a-f9a122775b7b.png#averageHue=%23212020&clientId=u4e9a6eaf-cf8f-4&from=paste&id=u03d24c6d&originHeight=520&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2ffc17b7-526e-4501-b16f-391dc60de26&title=)<br />订阅创建后，会创建安装计划，并进行 CSV 等资源的创建。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675947973767-22d9bb4d-376a-458f-9f5a-7035cf306d00.png#averageHue=%230d0d0d&clientId=u4e9a6eaf-cf8f-4&from=paste&id=u4652108f&originHeight=246&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uef3a7844-709b-4a38-977d-ebf91ab19f5&title=)<br />安装成功后，会在集群上安装 CRDs，并在 eck1 命名空间下运行 elastic-operator 的deployment，此时就可以在 eck1 和 eck2 命名空间下，创建 CR 使用 ECK 的功能。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675947973855-f4f778bc-2aa4-4d5e-a4b6-9350bf22b454.png#averageHue=%2311100f&clientId=u4e9a6eaf-cf8f-4&from=paste&id=uf505a78e&originHeight=359&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud88ae9cd-e8a9-478d-941c-366da7f8ca4&title=)
<a name="hqVww"></a>
## 6、总结 
目前 **Operator 技术已经成为扩展 Kubernetes 能力的标准方式**，Operator 的开发从像使用原生资源对象一样扩展应用，发展到了封装自己的组件，开发一些有状态的、持久化的中间件，将能力以标准的方式暴露出来。在开发过程中，OLM 会提供软件的打包、安装和升级，并实现一些监控告警能力，更加高效地进行 Operator 的生命周期管理，简化运维的复杂度。目前官方 OperatorHub 上提供的 Operator 已经有 297 个，涉及应用运行时、数据库、可观测性、消息中间件、开发工具等众多类别。随着像红帽、亚马逊、腾讯云等巨头公司的使用，Operator 的技术发展也会更加地成熟稳定。
<a name="KukHz"></a>
## 参考资料
[https://operatorhub.io/](https://operatorhub.io/)<br />[https://github.com/k8s-operatorhub/community-operators](https://github.com/k8s-operatorhub/community-operators)<br />[https://operatorframework.io/operator-capabilities/](https://operatorframework.io/operator-capabilities/)<br />[https://github.com/operator-framework/operator-sdk](https://github.com/operator-framework/operator-sdk)<br />[https://github.com/operator-framework/operator-lifecycle-manager](https://github.com/operator-framework/operator-lifecycle-manager)<br />[https://github.com/operator-framework/operator-registry](https://github.com/operator-framework/operator-registry)<br />[https://github.com/operator-framework/community-operators](https://github.com/operator-framework/community-operators)<br />[https://olm.operatorframework.io/docs/getting-started/#installing-olm-in-your-cluster](https://olm.operatorframework.io/docs/getting-started/#installing-olm-in-your-cluster)
