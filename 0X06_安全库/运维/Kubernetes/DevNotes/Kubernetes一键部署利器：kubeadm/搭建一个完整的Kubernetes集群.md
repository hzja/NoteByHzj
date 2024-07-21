Kubernetes kubeadm
<a name="iZwcL"></a>
## 机器需求
准备机器。最直接的办法，到公有云上申请几个虚拟机。当然，如果条件允许的话，拿几台本地的物理服务器来组集群是最好不过了。这些机器只要满足如下几个条件即可：

1. 满足安装 Docker 项目所需的要求，比如 64 位的 Linux 操作系统、3.10 及以上的内核版本；
2. x86 或者 ARM 架构均可；
3. 机器之间网络互通，这是将来容器之间网络互通的前提；
4. 有外网访问权限，因为需要拉取镜像；
5. 能够访问到gcr.io、quay.io这两个 docker registry，因为有小部分镜像需要在这里拉取；
6. 单机可用资源建议 2 核 CPU、8 GB 内存最低4GB或以上，再小的话问题也不大，但是能调度的 Pod 数量就比较有限了；
7. 30 GB 或以上的可用磁盘空间，这主要是留给 Docker 镜像和日志文件用的。否则会导致Pod因为磁盘不足被驱逐。

![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1640308557354-204d8de4-b975-4f0b-ba65-2f536ff0fe67.png#clientId=u7ab2e8ab-9a26-4&from=paste&height=277&id=uddfcf7e4&originHeight=830&originWidth=2539&originalType=binary&ratio=1&rotation=0&showTitle=false&size=182555&status=done&style=none&taskId=u7cc979b9-a43b-4b31-96e1-4ec396a7e31&title=&width=846.3333333333334)
<a name="W74o6"></a>
## 部署 Kubernetes 的 Master 节点
kubeadm 可以一键部署 Master 节点。可以通过配置文件来开启一些实验性功能。<br />这里编写了一个给 kubeadm 用的 YAML 文件（名叫：kubeadm.yaml）：
```yaml
apiVersion: kubeadm.k8s.io/v1alpha1
kind: MasterConfiguration
controllerManagerExtraArgs:
  horizontal-pod-autoscaler-use-rest-clients: "true"
  horizontal-pod-autoscaler-sync-period: "10s"
  node-monitor-grace-period: "10s"
apiServerExtraArgs:
  runtime-config: "api/all=true"
kubernetesVersion: "1.23.1"
```
这个配置中，给 kube-controller-manager 设置了：
```yaml
horizontal-pod-autoscaler-use-rest-clients: "true"
```
这意味着，将来部署的 kube-controller-manager 能够使用自定义资源（Custom Metrics）进行自动水平扩展。<br />然后，只需要参数指定对应的配置文件：
```bash
$ kubeadm init --config kubeadm.yaml
```
就可以完成 Kubernetes Master 的部署了，这个过程只需要几分钟。部署完成后，kubeadm 会生成一行指令：
```bash
kubeadm join 10.168.0.2:6443 --token 00bwbx.uvnaa2ewjflwu1ry --discovery-token-ca-cert-hash sha256:00eb62a2a6020f94132e3fe1ab721349bbcd3e9b94da9654cfe15f2985ebd711
```
这个 `kubeadm join` 命令，就是用来给 Master 节点添加更多工作节点（Worker）的命令。在后面部署 Worker 节点的时候马上会用到它，所以找一个地方把这条命令记录下来。<br />此外，kubeadm 还会提示第一次使用 Kubernetes 集群所需要的配置命令：
```bash
mkdir -p $HOME/.kube
sudo cp -i /etc/kubernetes/admin.conf $HOME/.kube/config
sudo chown $(id -u):$(id -g) $HOME/.kube/config
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1640184930598-497ce98b-d429-4121-b19c-b6ab96c729d9.png#clientId=uacbb8c8b-d505-4&from=paste&height=359&id=u4d7b5378&originHeight=1076&originWidth=2492&originalType=binary&ratio=1&rotation=0&showTitle=false&size=236352&status=done&style=none&taskId=u22673e5b-b0c1-4282-8b5d-22e88e66026&title=&width=830.6666666666666)<br />而需要这些配置命令的原因是：Kubernetes 集群默认需要加密方式访问。所以，这几条命令，就是将刚刚部署生成的 Kubernetes 集群的安全配置文件，保存到当前用户的.kube 目录下，kubectl 默认会使用这个目录下的授权信息访问 Kubernetes 集群。<br />如果不这么做的话，每次都需要通过 `export KUBECONFIG` 环境变量告诉 kubectl 这个安全配置文件的位置。<br />现在，就可以使用 `kubectl get` 命令来查看当前唯一一个节点的状态了：
```bash
$ kubectl get nodes

NAME      STATUS     ROLES     AGE       VERSION
master    NotReady   master    1d        v1.11.1
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1640185094166-9669d121-b86b-4a52-8201-9e4edd622306.png#clientId=uacbb8c8b-d505-4&from=paste&height=48&id=ufc51ad0d&originHeight=143&originWidth=2199&originalType=binary&ratio=1&rotation=0&showTitle=false&size=32259&status=done&style=none&taskId=ubde6b763-abf7-4b8f-b13a-7856d0734c6&title=&width=733)<br />可以看到，这个 get 指令输出的结果里，Master 节点的状态是 NotReady，这是为什么呢？<br />在调试 Kubernetes 集群时，最重要的手段就是用 `kubectl describe` 来查看这个节点（Node）对象的详细信息、状态和事件（Event），来试一下：
```bash
$ kubectl describe node master

...
Conditions:
...

Ready   False ... KubeletNotReady  runtime network not ready: NetworkReady=false reason:NetworkPluginNotReady message:docker: network plugin is not ready: cni config uninitialized
```
通过 `kubectl describe` 指令的输出，可以看到 NodeNotReady 的原因在于，尚未部署任何网络插件。<br />另外，还可以通过 kubectl 检查这个节点上各个系统 Pod 的状态，其中，kube-system 是 Kubernetes 项目预留的系统 Pod 的工作空间（Namepsace，注意它并不是 Linux Namespace，它只是 Kubernetes 划分不同工作空间的单位）：
```bash
$ kubectl get pods -n kube-system

NAME               READY   STATUS   RESTARTS  AGE
coredns-78fcdf6894-j9s52     0/1    Pending  0     1h
coredns-78fcdf6894-jm4wf     0/1    Pending  0     1h
etcd-master           1/1    Running  0     2s
kube-apiserver-master      1/1    Running  0     1s
kube-controller-manager-master  0/1    Pending  0     1s
kube-proxy-xbd47         1/1    NodeLost  0     1h
kube-scheduler-master      1/1    Running  0     1s
```
可以看到，CoreDNS、kube-controller-manager 等依赖于网络的 Pod 都处于 Pending 状态，即调度失败。这当然是符合预期的：因为这个 Master 节点的网络尚未就绪。
<a name="nO5T3"></a>
## 部署网络插件
在 Kubernetes 项目“一切皆容器”的设计理念指导下，部署网络插件非常简单，只需要执行一句 kubectl apply 指令，以 Weave 为例：
```bash
$ kubectl apply -f weave-daemonset-k8s-1.11.yaml
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1640187213294-fd5d651a-0cfe-4719-8074-43a6de1cb46d.png#clientId=uc8534b81-c6e8-4&from=paste&height=114&id=u64754f03&originHeight=341&originWidth=2349&originalType=binary&ratio=1&rotation=0&showTitle=false&size=75328&status=done&style=none&taskId=u1e727820-7329-457f-b4a1-1edad662200&title=&width=783)<br />部署完成后，可以通过 `kubectl get` 重新检查 Pod 的状态：
```bash
$ kubectl get pods -n kube-system

NAME                             READY     STATUS    RESTARTS   AGE
coredns-78fcdf6894-j9s52         1/1       Running   0          1d
coredns-78fcdf6894-jm4wf         1/1       Running   0          1d
etcd-master                      1/1       Running   0          9s
kube-apiserver-master            1/1       Running   0          9s
kube-controller-manager-master   1/1       Running   0          9s
kube-proxy-xbd47                 1/1       Running   0          1d
kube-scheduler-master            1/1       Running   0          9s
weave-net-cmk27                  2/2       Running   0          19s
```
可以看到，所有的系统 Pod 都成功启动了，而刚刚部署的 Weave 网络插件则在 kube-system 下面新建了一个名叫 weave-net-cmk27 的 Pod，一般来说，这些 Pod 就是容器网络插件在每个节点上的控制组件。<br />Kubernetes 支持容器网络插件，使用的是一个名叫 CNI 的通用接口，它也是当前容器网络的事实标准，市面上的所有容器网络开源项目都可以通过 CNI 接入 Kubernetes，比如 Flannel、Calico、Canal、Romana 等等，它们的部署方式也都是类似的“一键部署”。<br />至此，Kubernetes 的 Master 节点就部署完成了。如果只需要一个单节点的 Kubernetes，现在就可以使用了。不过，在默认情况下，Kubernetes 的 Master 节点是不能运行用户 Pod 的，所以还需要额外做一个小操作。
<a name="e1oOo"></a>
## 部署 Kubernetes 的 Worker 节点
Kubernetes 的 Worker 节点跟 Master 节点几乎是相同的，它们运行着的都是一个 kubelet 组件。唯一的区别在于，在 `kubeadm init` 的过程中，kubelet 启动后，Master 节点上还会自动运行 kube-apiserver、kube-scheduler、kube-controller-manger 这三个系统 Pod。<br />所以，相比之下，部署 Worker 节点反而是最简单的，只需要两步即可完成。<br />第一步，在所有 Worker 节点上执行“安装 kubeadm 和 Docker”。<br />第二步，执行部署 Master 节点时生成的 `kubeadm join` 指令：
```bash
$ kubeadm join 10.168.0.2:6443 --token 00bwbx.uvnaa2ewjflwu1ry --discovery-token-ca-cert-hash sha256:00eb62a2a6020f94132e3fe1ab721349bbcd3e9b94da9654cfe15f2985ebd711
```
<a name="p08eg"></a>
## 通过 Taint/Toleration 调整 Master 执行 Pod 的策略
默认情况下 Master 节点是不允许运行用户 Pod 的。而 Kubernetes 做到这一点，依靠的是 Kubernetes 的 Taint/Toleration 机制。<br />它的原理非常简单：一旦某个节点被加上了一个 Taint，即被“打上了污点”，那么所有 Pod 就都不能在这个节点上运行，因为 Kubernetes 的 Pod 都有“洁癖”。<br />除非，有个别的 Pod 声明自己能“容忍”这个“污点”，即声明了 Toleration，它才可以在这个节点上运行。
<a name="U9VNy"></a>
### 为节点打上“污点”（Taint）的命令是：
```bash
$ kubectl taint nodes node1 foo=bar:NoSchedule
```
这时，该 node1 节点上就会增加一个键值对格式的 Taint，即：`foo=bar:NoSchedule`。其中值里面的 NoSchedule，意味着这个 Taint 只会在调度新 Pod 时产生作用，而不会影响已经在 node1 上运行的 Pod，哪怕它们没有 Toleration。<br />污点可选参数：

- NoSchedule： 一定不能被调度 
- PreferNoSchedule： 尽量不要调度 
- NoExecute： 不仅不会调度, 还会驱逐Node上已有的Pod
<a name="AsXnV"></a>
### Pod 声明 Toleration
只要在 Pod 的.yaml 文件中的 spec 部分，加入 tolerations 字段即可：
```yaml
apiVersion: v1
kind: Pod
...
spec:
  tolerations:
  - key: "foo"
    operator: "Equal"
    value: "bar"
    effect: "NoSchedule"
```
这个 Toleration 的含义是，这个 Pod 能“容忍”所有键值对为 foo=bar 的 Taint（`operator: "Equal"`，“等于”操作）。<br />现在回到已经搭建的集群上来。这时，如果通过 `kubectl describe` 检查一下 Master 节点的 Taint 字段，就会有所发现了：
```bash
$ kubectl describe node master

Name:               master
Roles:              master
Taints:             node.kubernetes.io/disk-pressure:NoSchedule
```
可以看到，Master 节点默认被加上了node.kubernetes.io/disk-pressure:NoSchedule这样一个“污点”，其中“键”是node.kubernetes.io/disk-pressure，而没有提供“值”。<br />此时，就需要像下面这样用“Exists”操作符（`operator: "Exists"`，“存在”即可）来说明，该 Pod 能够容忍所有以 foo 为键的 Taint，才能让这个 Pod 运行在该 Master 节点上：
```yaml
apiVersion: v1
kind: Pod
...
spec:
  tolerations:
  - key: "foo"
    operator: "Exists"
    effect: "NoSchedule"
```
<a name="P498I"></a>
### 允许master节点部署pod
当然，如果就是想要一个单节点的 Kubernetes，删除这个 Taint 才是正确的选择：<br />注意这里的键是通过`kubectl describe node master`命令查询出来的Taints键对应的值：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1640266645934-a4212d40-beb1-4b25-a703-fa2bf5a06da7.png#clientId=u517da687-2002-4&from=paste&height=409&id=u214ca417&originHeight=1226&originWidth=2455&originalType=binary&ratio=1&rotation=0&showTitle=false&size=226969&status=done&style=none&taskId=u50e89b40-2a21-4373-a459-724fc516187&title=&width=818.3333333333334)
```bash
$ kubectl taint nodes --all node.kubernetes.io/disk-pressure-
```
如上所示，在“node.kubernetes.io/disk-pressure”这个键后面加上了一个短横线“`-`”，这个格式就意味着移除所有以“node.kubernetes.io/disk-pressure”为键的 Taint。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1640266771538-f94551d3-6b0f-4c3d-a0d3-a2ab10d1574e.png#clientId=u517da687-2002-4&from=paste&height=32&id=u815f2d7e&originHeight=96&originWidth=2525&originalType=binary&ratio=1&rotation=0&showTitle=false&size=27969&status=done&style=none&taskId=u10b786a4-a5ec-4cf1-8a16-1cd84446bf8&title=&width=841.6666666666666)<br />如果不允许调度，可以执行以下命令进行设置
```bash
kubectl taint nodes master1 node.kubernetes.io/disk-pressure=:NoSchedule
```
到了这一步，一个基本完整的 Kubernetes 集群就部署完毕了。<br />有了 kubeadm 这样的原生管理工具，Kubernetes 的部署已经被大大简化。更重要的是，像证书、授权、各个组件的配置等部署中最麻烦的操作，kubeadm 都已经完成了。<br />接下来，再在这个 Kubernetes 集群上安装一些其他的辅助插件，比如 Dashboard 和存储插件。
<a name="gdki2"></a>
## 部署 Dashboard 可视化插件
在 Kubernetes 社区中，有一个很受欢迎的 Dashboard 项目，它可以给用户提供一个可视化的 Web 界面来查看当前集群的各种信息。毫不意外，它的部署也相当简单：
```bash
$ kubectl apply -f https://raw.githubusercontent.com/kubernetes/dashboard/v2.0.0-rc6/aio/deploy/recommended.yaml
```
部署完成之后，就可以查看 Dashboard 对应的 Pod 的状态了：
```bash
$ kubectl get pods -n kube-system

kubernetes-dashboard-6948bdb78-f67xk   1/1       Running   0          1m
```
需要注意的是，由于 Dashboard 是一个 Web Server，很多人经常会在自己的公有云上无意地暴露 Dashboard 的端口，从而造成安全隐患。所以，1.7 版本之后的 Dashboard 项目部署完成后，默认只能通过 Proxy 的方式在本地访问。具体的操作，可以查看 Dashboard 项目的[官方文档](https://github.com/kubernetes/dashboard)。<br />而如果想从集群外访问这个 Dashboard 的话，就需要用到 Ingress。
<a name="GGmab"></a>
## 部署容器存储插件
很多时候需要用数据卷（Volume）把外面宿主机上的目录或者文件挂载进容器的 Mount Namespace 中，从而达到容器和宿主机共享这些目录或者文件的目的。容器里的应用，也就可以在这些数据卷中新建和写入文件。<br />可是，如果在某一台机器上启动的一个容器，显然无法看到其他机器上的容器在它们的数据卷里写入的文件。**这是容器最典型的特征之一：无状态。**<br />而容器的持久化存储，就是用来保存容器存储状态的重要手段：存储插件会在容器里挂载一个基于网络或者其他机制的远程数据卷，使得在容器里创建的文件，实际上是保存在远程存储服务器上，或者以分布式的方式保存在多个节点上，而与当前宿主机没有任何绑定关系。这样，无论在其他哪个宿主机上启动新的容器，都可以请求挂载指定的持久化存储卷，从而访问到数据卷里保存的内容。**这就是“持久化”的含义。**<br />由于 Kubernetes 本身的松耦合设计，绝大多数存储项目，比如 Ceph、GlusterFS、NFS 等，都可以为 Kubernetes 提供持久化存储能力。这里选择部署一个很重要的 Kubernetes 存储插件项目：Rook。<br />Rook 项目是一个基于 Ceph 的 Kubernetes 存储插件（它后期也在加入对更多存储实现的支持）。不过，不同于对 Ceph 的简单封装，Rook 在自己的实现中加入了水平扩展、迁移、灾难备份、监控等大量的企业级功能，使得这个项目变成了一个完整的、生产级别可用的容器存储插件。<br />得益于容器化技术，用几条指令，Rook 就可以把复杂的 Ceph 存储后端部署起来：
```bash
$ kubectl apply -f https://raw.githubusercontent.com/rook/rook/master/cluster/examples/kubernetes/ceph/common.yaml

$ kubectl apply -f https://raw.githubusercontent.com/rook/rook/master/cluster/examples/kubernetes/ceph/operator.yaml

$ kubectl apply -f https://raw.githubusercontent.com/rook/rook/master/cluster/examples/kubernetes/ceph/cluster.yaml
```
在部署完成后，就可以看到 Rook 项目会将自己的 Pod 放置在由它自己管理的两个 Namespace 当中：
```bash
$ kubectl get pods -n rook-ceph-system
NAME                                  READY     STATUS    RESTARTS   AGE
rook-ceph-agent-7cv62                 1/1       Running   0          15s
rook-ceph-operator-78d498c68c-7fj72   1/1       Running   0          44s
rook-discover-2ctcv                   1/1       Running   0          15s

$ kubectl get pods -n rook-ceph
NAME                   READY     STATUS    RESTARTS   AGE
rook-ceph-mon0-kxnzh   1/1       Running   0          13s
rook-ceph-mon1-7dn2t   1/1       Running   0          2s
```
这样，一个基于 Rook 持久化存储集群就以容器的方式运行起来了，而接下来在 Kubernetes 项目上创建的所有 Pod 就能够通过 Persistent Volume（PV）和 Persistent Volume Claim（PVC）的方式，在容器里挂载由 Ceph 提供的数据卷了。<br />而 Rook 项目，则会负责这些数据卷的生命周期管理、灾难备份等运维工作。
<a name="TJe8W"></a>
### 为什么要选择 Rook 项目呢？
因为这个项目很有前途。<br />如果去研究一下 Rook 项目的实现，就会发现它巧妙地依赖了 Kubernetes 提供的编排能力，合理的使用了很多诸如 Operator、CRD 等重要的扩展特性。这使得 Rook 项目，成为了目前社区中基于 Kubernetes API 构建的最完善也最成熟的容器存储插件。<br />备注：其实，在很多时候，所谓的“云原生”，就是“Kubernetes 原生”的意思。而像 Rook、Istio 这样的项目，正是贯彻这个思路的典范。
