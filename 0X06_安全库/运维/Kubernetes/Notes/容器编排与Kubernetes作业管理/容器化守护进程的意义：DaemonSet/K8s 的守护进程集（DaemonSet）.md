Kubernetes DaemonSet<br />顾名思义，**DaemonSet** 的主要功能是可在 K8s 集群中运行一个**守护进程 Pod**。**DaemonSet** 可确保在所有（或部分）工作节上点运行 Pod 的副本。<br />随着节点被添加到集群中，Pod 也被添加进去。当从集群中删除节点时，这些 Pod 会被垃圾回收。删除**DaemonSet**将清理其创建的 Pod。与其他编排对象不同的是，DaemonSet 开始运行的时间往往早于整个 K8s 集群出现的时间。<br />**守护进程 Pod** 具有以下特点：

- 它运行在 K8s 集群中的每个节点（大多数情况下）上
- 每个节点上只有一个这样的 Pod
- 当有新节点加入 K8s 集群时，就会在该新节点上自动创建 Pod
- 当一个节点被删除时，Pod 会相应地被回收

**DaemonSet**的一些典型用途：

- 在每个节点上运行**集群存储守护进程**，例如 ceph。
- 在每个节点上运行**日志收集守护进程**，例如 fluentd。
- 在每个节点上运行**节点监控守护进程**，例如 Prometheus node exporter。
<a name="pdmFu"></a>
## K8s 系统守护程序集
事实上，K8s 本身就是使用 **DaemonSet** 来运行系统组件的。如果您运行以下命令：
```bash
$ kubectl get daemonset -n kube-system
NAME         DESIRED   CURRENT   READY   UP-TO-DATE   AVAILABLE   NODE SELECTOR   AGE
aws-node     2         2         2       2            2           <none>          160d
kube-proxy   2         2         2       2            2           <none>          160d
```
可以看到 K8s 集群中运行着 aws-node 和 kube-proxy 两个 **DaemonSet** 。
<a name="Mn2MF"></a>
## DaemonSet 定义
为了理解 **DaemonSet** 是如何工作的，首先需要看一下它的定义：
```yaml
apiVersion: apps/v1
kind: DaemonSet
metadata:
  name: fluentd-elasticsearch
  namespace: kube-system
  labels:
    k8s-app: fluentd-logging
spec:
  selector:
    matchLabels:
      name: fluentd-elasticsearch
  template:
    metadata:
      labels:
        name: fluentd-elasticsearch
    spec:
      tolerations:
      # this toleration is to have the daemonset runnable on master nodes
      # remove it if your masters can't run pods
      - key: node-role.kubernetes.io/master
        operator: Exists
        effect: NoSchedule
      containers:
      - name: fluentd-elasticsearch
        image: quay.io/fluentd_elasticsearch/fluentd:v2.5.2
        resources:
          limits:
            memory: 200Mi
          requests:
            cpu: 100m
            memory: 200Mi
        volumeMounts:
        - name: varlog
          mountPath: /var/log
        - name: varlibdockercontainers
          mountPath: /var/lib/docker/containers
          readOnly: true
      terminationGracePeriodSeconds: 30
      volumes:
      - name: varlog
        hostPath:
          path: /var/log
      - name: varlibdockercontainers
        hostPath:
          path: /var/lib/docker/containers
```
这个 **DaemonSet** 管理着一个 fluentd-elasticsearch 镜像 Pod。该镜像的功能非常有用：通过 fluentd 可将 Docker 容器中的日志转发到 ElasticSearch。<br />需要注意的是，在 **DaemonSet** 上，为了防止它占用过多的主机资源，一般应该添加 resources 字段来限制它的 CPU 和内存使用。<br />可以看到，**DaemonSet** 其实和 Deployment很像，只不过没有 replicas 字段；它还使用选择器来选择和管理所有带有 `name=fluentd-elasticsearch` 标签的 Pod。<br />这些 Pod 模板也是用 template 字段定义的。在该字段中，使用 fluentd-elasticsearch:2.5.2 镜像定义了一个容器，该容器挂载了两个 hostPath 类型的卷，分别对应主机的 /var/log 和 /var/lib/docker/ 容器目录。<br />很明显，fluentd 启动后，会从这两个目录中收集日志信息，然后转发给 ElasticSearch 保存。这样就可以通过 ElasticSearch 来轻松检索这些日志。
<a name="fE1V8"></a>
## 在所有节点上运行 Pod
那么 **DaemonSet** 是如何保证每个节点上只托管一个 Pod 的呢？这通常由**守护进程集控制器**DaemonSet Controller 处理。<br />**DaemonSet 控制器**首先从 api-server 上获取所有节点的列表（从 etcd 获取数据），然后遍历所有节点，并检查当前每个节点上是否有 DaemonSet Pod 在运行。<br />检查结果可能有以下三种情况：

1. 没有这样的 Pod，就意味着是在该 Node 上创建这样的 Pod；
2. 如果有这样的 Pod，但数量大于 1，则表示应该从该 Node 中删除多余的 Pod；
3. 正好有 1 个这样的 Pod，说明这个节点是正常的。

如果要在**选定节点**上调度 Pod，则必须指定 .spec.template.spec.nodeSelector，然后 **DaemonSet 控制器**将在匹配节点选择器的节点上创建 Pod。如果不指定，则 **DaemonSet 控制器**将在所有节点上创建 Pod。例如：
```yaml
nodeSelector:
    name: Node_Name
```
但是，在 K8s 项目中，`nodeSelector` 实际上是一个会被弃用的字段 因为，现在有一个功能更全的新字段来代替它，即：`nodeAffinity`。
<a name="HlMCg"></a>
## `nodeAffinity`
举个例子吧：
```yaml
apiVersion: v1
kind: Pod
metadata:
  name: with-node-affinity
spec:
  affinity:
    nodeAffinity:
      requiredDuringSchedulingIgnoredDuringExecution:
        nodeSelectorTerms:
        - matchExpressions:
          - key: metadata.name
            operator: In
            values:
            - node-name
```
在上面的例子中，声明了一个 `spec.affinity` 字段，并定义了一个 `nodeAffinity`。其中，`spec.affinity` 字段是 Pod 中与调度相关的字段。

- `requiredDuringSchedulingIgnoredDuringExecution`：表示每次调度时都要考虑该 `nodeAffinity`。同时，这也意味着在某些情况下 nodeAffinity 也可以设置为被忽略；
- 这个 Pod 以后只被允许在"metadata.name" 为 "node-name" 的节点上运行。

因此，**DaemonSet 控制器**在创建 Pod 时会自动将这样的 `nodeAffinity` 定义添加到 Pod 的 API 对象中。其中，需要绑定的节点名称为当前正在遍历的节点。
<a name="pwus3"></a>
## 容忍度（Tolerations）
另外，**DaemonSet** 会自动为这个 Pod 添加另一个称为 tolerations 的**调度相关字段**。该字段意味着这个 Pod 将“**容忍（Toleration）**”一些节点“**污点（Taint）**”。<br />看一个例子：
```yaml
apiVersion: v1
kind: Pod
metadata:
  name: with-toleration
spec:
  tolerations:
  - key: node.kubernetes.io/unschedulable
    operator: Exists
    effect: NoSchedule
```
这个 **Toleration** 的意思是：“**容忍**”所有标记为不可调度的“**污点**”的节点；“**容忍**”的效果是**允许调度**。<br />正常情况下，标记为不可调度的“**污点**”的 Node 不会有任何 Pod 被调度（效果：NoSchedule）。但是，**DaemonSet** 会自动将这个特殊的 **Toleration** 添加到托管的 Pod 中，这样这些 Pod 就可以忽略这个限制，然后确保每个节点上都会调度一个 Pod。当然，如果节点出现故障，Pod 可能启动失败，**DaemonSet** 会一直尝试，直到 Pod 启动成功。
<a name="ZYLU4"></a>
## 创建 DaemonSet
最后，创建这个 fluentd DaemonSet。执行以下命令：
```bash
$ kubectl create -f fluentd-elasticsearch.yaml
```
创建成功之后，可以看到如下输出：
```bash
$ kubectl get pod -n kube-system -l name=fluentd-elasticsearch
NAME                          READY     STATUS    RESTARTS   AGE
fluentd-elasticsearch-dqfv9   1/1       Running   0          10m
fluentd-elasticsearch-pf9z5   1/1       Running   0          10m
```
此时，若想通过 `kubectl get` 查看 Kubernetes 集群中的 **DaemonSet** 对象，则：
```bash
$ kubectl get ds -n kube-system fluentd-elasticsearch
NAME                    DESIRED   CURRENT   READY     UP-TO-DATE   AVAILABLE   NODE SELECTOR   AGE
fluentd-elasticsearch   2         2         2         2            2           <none>          1h
```
<a name="F2dei"></a>
## 结论
**DaemonSet** 只管理 Pod 对象，然后通过 `**nodeAffinity**` 和 `**Toleration**` 的小功能保证每个节点上只有一个 Pod。这个控制器的实现原理和 Deployment 类似，应该简单易懂。
