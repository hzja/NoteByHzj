Kubernetes
<a name="mueDo"></a>
## Kube-capacity
Kube-capacity 是一个简单而强大的 CLI，它提供了Kubernetes集群中资源请求、限制和利用率的概览。它将输出的最佳部分结合kubectl top到kubectl describe一个易于使用的集中于集群资源的 CLI 中。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653024531029-d2ff1874-ab11-40a3-b187-fc512252f147.png#clientId=u4cdd5147-6a33-4&from=paste&id=u991ae58c&originHeight=302&originWidth=1050&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4c5741c4-da1c-41e4-8045-48434f94c62&title=)<br />不过像这样的工具确实好用一些，比如想看这个kube-system下有哪些pod有没有设置request和limit的时候，实际上，需要输入一段很长的命令才能列出，而且需要一些调试，这看起来不是特别方便
```bash
# kubectl get pod -n kube-system  -o=custom-columns=NAME:.metadata.name,NAMESPACE:.metadata.namespace,PHASE:.status.phase,Request-cpu:.spec.containers\[0\].resources.requests.cpu,Request-memory:.spec.containers\[0\].resources.requests.memory,Limit-cpu:.spec.containers\[0\].resources.limits.cpu,Limit-memory:.spec.containers\[0\].resources.limits.memory
NAME                                       NAMESPACE     PHASE     Request-cpu   Request-memory   Limit-cpu   Limit-memory
calico-kube-controllers-7d4c6544cc-g6x6q   kube-system   Running   <none>        <none>           <none>      <none>
calico-node-4fjn8                          kube-system   Running   250m          <none>           <none>      <none>
calico-node-7nbpj                          kube-system   Running   250m          <none>           <none>      <none>
calico-node-qvdlr                          kube-system   Running   250m          <none>           <none>      <none>
coredns-7977f69688-4nz9q                   kube-system   Running   100m          70Mi             <none>      170Mi
coredns-7977f69688-frkt5                   kube-system   Running   100m          70Mi             <none>      170Mi
etcd-k8s-master1                           kube-system   Running   100m          100Mi            <none>      <none>
etcd-k8s-master2                           kube-system   Running   100m          100Mi            <none>      <none>
etcd-k8s-master3                           kube-system   Running   100m          100Mi            <none>      <none>
kube-apiserver-k8s-master1                 kube-system   Running   250m          <none>           <none>      <none>
```
那么这个工具实际上解决的问题就是帮助快速查看概览整个集群和pod的资源配置情况。
<a name="XOCP8"></a>
### 安装
Go 二进制文件由GoReleaser随每个版本自动构建。这些可以在此项目的 GitHub发布页面上访问。<br />[https://github.com/goreleaser/goreleaser](https://github.com/goreleaser/goreleaser)<br />[https://github.com/robscott/kube-capacity/releases](https://github.com/robscott/kube-capacity/releases)
<a name="ODpbV"></a>
#### Homebrew
这个项目可以用Homebrew安装：
```bash
brew tap robscott/tap
brew install robscott/tap/kube-capacity
```
<a name="QoTWo"></a>
#### Krew
这个项目可以用Krew安装：
```bash
kubectl krew install resource-capacity
```
<a name="WQytQ"></a>
### 用法
默认情况下，kube-capacity 将输出一个节点列表，其中包含 CPU 和内存资源请求的总数以及在它们上运行的所有 pod 的限制。对于具有多个节点的集群，第一行还将包括集群范围的总数。该输出将如下所示：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653024531026-3f86c324-9522-4a6e-b110-d3d29c5ab331.png#clientId=u4cdd5147-6a33-4&from=paste&id=u7fe04330&originHeight=459&originWidth=1050&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc6117253-8cdc-4663-9867-c018ad850ca&title=)
<a name="d7p2f"></a>
#### 包括 Pod
对于更详细的输出，kube-capacity 可以在输出中包含 pod。当`-p`或`--pods`被传递给 kube-capacity 时，它将包含如下所示的特定于 pod 的输出<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653024531105-1cde1643-cb2e-451e-bea9-e666ff34cd62.png#clientId=u4cdd5147-6a33-4&from=paste&id=u3ff997b6&originHeight=380&originWidth=1050&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud38aa03b-e3f2-4d4f-94a3-bcc96d9ad49&title=)
<a name="mkQ2w"></a>
#### 包括利用率
为了帮助了解资源利用率与配置的请求和限制的比较，kube-capacity 可以在输出中包含利用率指标。请务必注意，此输出依赖于集群中的metrics-server正常运行。当`-u`或`--util`被传递给 kube-capacity 时，它将包含如下所示的资源利用率信息：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653024531034-630edb26-f9fc-4d46-8ad1-f627c08402dd.png#clientId=u4cdd5147-6a33-4&from=paste&id=u4893005d&originHeight=302&originWidth=1050&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9016adc1-1bfc-4aca-b00b-54614d8c096&title=)
<a name="KHpW3"></a>
#### 包括 Pod 和利用率
对于更详细的输出，kube-capacity 可以在输出中包含 pod 和资源利用率。当`--util`和`--pods`传递给 kube-capacity 时，它将产生如下所示的宽输出：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653024531401-b7298ecb-6fc5-4f3a-afcd-85ef245026f8.png#clientId=u4cdd5147-6a33-4&from=paste&id=u79080249&originHeight=344&originWidth=1050&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ufa1476d7-0463-4b89-b802-6f6335b5453&title=)<br />值得注意的是，来自 pod 的利用率数字可能不会与总节点利用率相加。与节点和集群级别数字代表 pod 值总和的请求和限制数字不同，节点指标直接来自指标服务器，并且可能包括其他形式的资源利用率。
<a name="m2aSV"></a>
#### 排序
要突出显示具有最高指标的节点、pod 和容器，可以按各种列进行排序：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653024531357-9128d656-4d64-45fb-a04a-0fbb1d65fe33.png#clientId=u4cdd5147-6a33-4&from=paste&id=ua13d4845&originHeight=353&originWidth=1050&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud5f570e2-b014-4166-8d5d-8d7877d4499&title=)
<a name="CHqUe"></a>
#### 显示 Pod 计数
要显示每个节点和整个集群的 pod 数量，可以通过`--pod-count`参数：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653024531372-f57f5002-f335-421c-b05d-a3158c339d35.png#clientId=u4cdd5147-6a33-4&from=paste&id=ud5914670&originHeight=397&originWidth=1050&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7c298b7d-5bee-4f26-ba7f-ce3618a232a&title=)
<a name="lfEzF"></a>
#### 按标签过滤
对于更高级的使用，kube-capacity 还支持按 pod、命名空间和/或节点标签进行过滤。以下示例展示了如何使用这些过滤器：
```bash
kube-capacity --pod-labels app=nginx 
kube-capacity --namespace 默认
kube-capacity --namespace-labels team=api 
kube-capacity --node-labels kubernetes.io/role=node
```
<a name="QIN7N"></a>
#### JSON 和 YAML 输出
默认情况下，kube-capacity 将以表格格式提供输出。要以 JSON 或 YAML 格式查看此数据，可以使用输出标志。以下是一些示例命令：
```bash
kube-capacity --pods --output json
kube-capacity --pods --containers --util --output yaml
```
<a name="jeQT0"></a>
#### 支持的标志
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653024531391-7ec98c45-a4f9-4893-b104-4b9c56bb5978.png#clientId=u4cdd5147-6a33-4&from=paste&id=u6451782c&originHeight=679&originWidth=1050&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2d09d4ff-c9c4-47d2-8c1e-cd0d477bcba&title=)
<a name="qauSY"></a>
#### 先决条件
任何请求集群利用率的命令都依赖于集群上运行的metrics-server。如果尚未安装，可以使用官方helm chart进行安装。<br />[https://github.com/helm/charts/tree/master/stable/metrics-server](https://github.com/helm/charts/tree/master/stable/metrics-server)<br />[https://github.com/kubernetes-incubator/metrics-server](https://github.com/kubernetes-incubator/metrics-server)
