Kubernetes<br />Kubernetes 是一个由主节点和工作节点组成的容器编排工具。它只允许通过作为控制平面核心组件的 API 服务器进行通信。API 服务器公开了一个 HTTP REST API，允许内部组件（如用户和集群）和外部组件之间的通信。<br />你可以将 API 服务器视为 Kubernetes 的主要用户界面或前端。它使您能够查询、更新或管理 Kubernetes 对象或资源的状态。为了建立这些交互，Kubernetes API 可以直接发出 REST 请求、使用客户端库或通过kubectl 命令行接收直接命令。<br />kubectl 可以帮助您执行各种操作，包括：

- 部署容器化应用
- 运行 Kubernetes 操作
- 监控任务
- 检查和管理集群资源
- 查看系统日志
<a name="FLe8D"></a>
## Kubectl 概念
在开始使用 kubectl 之前，对命令结构有一个基本的了解很重要。以下是命令的一般语法：
```bash
kubectl [command] [TYPE] [NAME] [flags］
```
回顾一下这些属性：

- command — 描述要执行的操作类型。常见操作包括创建、写入、获取、应用 和删除。这些命令要么创建新的 Kubernetes 对象，要么修改现有对象，要么请求有关现有对象的信息。你可以在单个命令中指定多个资源。
- TYPE —描述你的命令所针对的资源类型。常见的选项是pod、 service、 deployment、 daemonset、 statefulset、 job 或cronjob。
- NAME —这是区分大小写的，指定你的命令应该应用到的资源的名称。提供资源名称不是强制性的——如果你提供名称，则命令仅限于该特定资源（或者如果没有该名称的资源，你会收到错误消息。如果不指定，则该命令适用于当前命名空间集群中的所有资源。
- flags —这些表示特殊选项或对特定信息的请求。它们也可以用作修饰符来覆盖默认值或环境变量。
<a name="iWnaH"></a>
## 8 大 kubectl 命令详解
<a name="qXSrN"></a>
### **1、List Kubernetes Resources（列出 Kubernetes 资源）**
使用 kubectl get 操作列出一个或多个资源。例如，使用kubectl get pods列出所有 Kubernetes pods。添加一个输出标志，如get pods -o wide将列出pod和额外的数据，如他们相关的节点名称。<br />get 操作可以列出额外的资源，例如服务和复制控制器。使用 kubectl 命令get rc或get services列出所有服务和复制控制器。<br />get操作的不同变体允许你执行诸如指定特定节点和使用短别名减少资源长度等操作。
<a name="iDp0u"></a>
### **2、Describe（描述）**
get 命令提供了一个紧凑的资源列表，而kubectl describe命令提供了一个或多个资源状态的详细报告。Kubernetes 资源。`kubectl describe pods`命令描述了所有 Kubernetes pods。如果复制控制器正在管理 pods，您可以使用describe pods命令显示指定控制器的 pods的详细信息。<br />kubectl describe操作可以专注于特定的 pod 或节点。例如，可以使用 kubectl 命令describe nodes来显示指定节点的详细信息。另外，describe pods将显示指定 pod 的详细信息。
<a name="H3xjE"></a>
### **3、Create and Modify（创建和修改 ）**
你可以使用 kubectl 中的apply命令，从特定文件或标准输入 (stdin) 创建资源。`kubectl apply -F servicename.yaml`命令使用特定的 YAML 文件创建一个新服务。`-f`标志表示文件的使用。<br />例如，如果想使用 YAML 文件的内容创建一个新的 RC，可以使用`apply -f controllername.yaml`命令。另一种选择是使用更广泛的命令（例如apply -f）在指定目录的 JSON 或 YAML 文件中创建服务资源。
<a name="ryRrO"></a>
### **4、Delete（删除）**
kubectl delete操作会终止你不再需要的服务和资源。它对于管理 Kubernetes 至关重要，允许你为不同的 Kubernetes 任务释放计算能力。<br />例如，你可以使用`kubectl delete pods –all`命令删除所有 pod。删除 pod 时，使用单独的 YAML 文件中指定的资源名称和类型是比较安全的。如果你使用examplepod.yaml文件创建 pod，你可以使用命令`delete -f examplepod.yaml`删除该 pod 。<br />Kubectl还可以删除共享特定标签的服务和 pod，你可以使用标签操作分配这些标签。例如，delete pods,services -l name=命令会删除所有标记为“example-name”的 pod 和服务。
<a name="adGYc"></a>
### **5、PersistentVolume 持久卷（PV）**
Kubernetes PersistentVolume是一种在 Kubernetes 集群中配置存储的机制。它可以由管理员手动配置，也可以使用 StorageClasses 自动配置。PV 是集群中的独立资源，独立于使用它们的各个 pod。如果一个 pod 出现故障，PV 将保持原位并可以安装在其他 pod 上。<br />在幕后，PV 对象使用 NFS、iSCSI 或公共云存储服务与物理存储设备交互。<br />以下是可用于处理 PersistentVolume 的三个有用命令：<br />运行以下命令在节点上创建 PV（提供 PV 清单的 URL）：`kubectl apply -f https://k8s.io/examples/pods/storage/pv-volume.yaml`<br />创建一个PersistentVolumeClaim（PVC），请求一个具有特定标准的PV。这可以实现PV的动态配置。运行此命令以在集群中创建PVC：`kubectl apply -f https://k8s.io/examples/pods/storage/pv-claim.yaml`<br />创建 PVC 后，Kubernetes 控制平面就开始寻找合适的 PV。当它找到一个，它就把 PVC 绑定到 PV。运行以下命令以查看 PV 的状态：kubectl 获取 pv 任务-pv-volume
<a name="QozJp"></a>
### **6、Security Context（安全环境）**
在 Kubernetes 中安全地运行工作负载可能是一种挑战。各种设置会影响 Kubernetes API 使用的安全控制。Kubernetes 提供的强大工具之一是设置所有 pod 清单都可以使用的 security Context。<br />在 Kubernetes 中使用security Context很容易。您只需在部署 pod 时在部署清单中包含security Context。例如，以下代码块指示 Kubernetes 运行用户 ID 为 1000 且组 ID 为 2000 的 pod：Spec: securityContext: runAsUser: 1000 fsGroup: 2000<br />与 RBAC 不同，安全上下文不需要你定义不同类型的文件（例如 Roles 和 RoleBindings）来应用安全规则。只需在声明部署时添加所需的安全上下文代码，Kubernetes 就会自动为你应用规则。
<a name="LNeZB"></a>
### **7、Managing Deployments（管理部署）**
有几个有用的命令可用于管理 Kubernetes 部署。部署管理还包括 StatefulSet 和 DaemonSet 管理。<br />更新部署、有状态集或守护程序集时，你可以使用rollout status命令查看更新的状态。还可以使用命令rollout undo <statefulset-name / app-name>取消部署。 另一方面，`rollout history <deployment name / app name>`命令将为你提供部署更改的历史记录。<br />这些命令在实践中并不常见，因为大多数管理员使用 Helm 等工具来管理部署。<br />你可以使用命令`scale –replicas=N <deployment name / app name>`来调整为你的部署运行的 pod 数量。在这种情况下，N表示更新的副本数。它产生与使用`kubectl edit <deployment name / app name>`命令调整副本数量相同的结果。<br />因为你可能正在使用 Helm 或 pod 自动缩放器进行静态更改，所以你不太可能在实践中使用这种方法或进行任何手动更改。还可以使用`kubectl autoscale`操作设置基本的Kubernetes 自动缩放功能。然而，此命令仅适用于 CPU 利用率指标。
<a name="IM0PB"></a>
### **8、Executing Commands（执行命令）**
可以通过kubectl使用exec操作来执行命令，该操作针对容器或 pod 运行命令。例如，可以使用`kubectl execdate`命令在指定 pod 的 shell 中运行date命令并显示输出。默认情况下，此命令在 pod 中的第一个容器上执行。<br />再举一个例子，可以在 pod 中的指定容器中使用 `exec-c container-name date`命令。
<a name="TIHv6"></a>
## 结论
虽然这个命令列表不会让你在 Kubernetes 中完成所有操作，但足以完成许多日常任务。希望这将有助于你在成为 Kubernetes高手的过程中提高对容器化环境的掌握程度。
