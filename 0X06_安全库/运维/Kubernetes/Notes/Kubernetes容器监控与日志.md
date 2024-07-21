Kubernetes 容器监控 日志
<a name="lAIne"></a>
## Prometheus、Metrics Server 与 Kubernetes 监控体系
首先需要明确指出的是，Kubernetes 项目的监控体系曾经非常繁杂，在社区中也有很多方案。但这套体系发展到今天，已经完全演变成了以 Prometheus 项目为核心的一套统一的方案。
<a name="gFo0U"></a>
### Prometheus简介
先来简单介绍一下Prometheus这个项目。<br />实际上，Prometheus 项目是当年 CNCF 基金会起家时的“第二把交椅”。而这个项目发展到今天，已经全面接管了 Kubernetes 项目的整套监控体系。<br />比较有意思的是，Prometheus 项目与 Kubernetes 项目一样，也来自于 Google 的 Borg 体系，它的原型系统，叫作 BorgMon，是一个几乎与 Borg 同时诞生的内部监控系统。而 Prometheus 项目的发起原因也跟 Kubernetes 很类似，都是希望通过对用户更友好的方式，将 Google 内部系统的设计理念，传递给用户和开发者。<br />作为一个监控系统，Prometheus 项目的作用和工作方式，其实可以用如下所示的一张官方示意图来解释。<br />![2ada1ece66fcc81d704c2ba46f9dd7d3[1].png](https://cdn.nlark.com/yuque/0/2022/png/396745/1642594990538-4b4e6b1d-7571-4d06-a16f-303d9545ef19.png#clientId=u357aa977-0e96-4&from=ui&id=ubd983831&originHeight=811&originWidth=1351&originalType=binary&ratio=1&rotation=0&showTitle=false&size=115427&status=done&style=shadow&taskId=u308b7326-0bc7-4a08-8744-5e14b658c21&title=)<br />可以看到，Prometheus 项目工作的核心，是使用 Pull （抓取）的方式去搜集被监控对象的 Metrics 数据（监控指标数据），然后，再把这些数据保存在一个 TSDB （时间序列数据库，比如 OpenTSDB、InfluxDB 等）当中，以便后续可以按照时间进行检索。<br />有了这套核心监控机制， Prometheus 剩下的组件就是用来配合这套机制的运行。比如 Pushgateway，可以允许被监控对象以 Push 的方式向 Prometheus 推送 Metrics 数据。而 Alertmanager，则可以根据 Metrics 信息灵活地设置报警。当然， Prometheus 最受用户欢迎的功能，还是通过 Grafana 对外暴露出的、可以灵活配置的监控数据可视化界面。<br />有了 Prometheus 之后，就可以按照 Metrics 数据的来源，来对 Kubernetes 的监控体系做一个汇总了。
<a name="F4KUM"></a>
### Metrics
<a name="qff56"></a>
#### 第一种 Metrics，是宿主机的监控数据。
这部分数据的提供，需要借助一个由 Prometheus 维护的[Node Exporter](https://github.com/prometheus/node_exporter) 工具。一般来说，Node Exporter 会以 DaemonSet 的方式运行在宿主机上。其实，所谓的 Exporter，就是代替被监控对象来对 Prometheus 暴露出可以被“抓取”的 Metrics 信息的一个辅助进程。<br />而 Node Exporter 可以暴露给 Prometheus 采集的 Metrics 数据， 也不单单是节点的负载（Load）、CPU 、内存、磁盘以及网络这样的常规信息，它的 Metrics 指标可以说是“包罗万象”，可以查看[这个列表](https://github.com/prometheus/node_exporter#enabled-by-default)来感受一下。
<a name="Z9YHf"></a>
#### 第二种 Metrics，是来自于 Kubernetes 的 API Server、kubelet 等组件的 /metrics API。
除了常规的 CPU、内存的信息外，这部分信息还主要包括了各个组件的核心监控指标。比如，对于 API Server 来说，它就会在 /metrics API 里，暴露出各个 Controller 的工作队列（Work Queue）的长度、请求的 QPS 和延迟数据等等。这些信息，是检查 Kubernetes 本身工作情况的主要依据。
<a name="hIc9T"></a>
#### 第三种 Metrics，是 Kubernetes 相关的监控数据。
这部分数据，一般叫作 Kubernetes 核心监控数据（core metrics）。这其中包括了 Pod、Node、容器、Service 等主要 Kubernetes 核心概念的 Metrics。<br />其中，容器相关的 Metrics 主要来自于 kubelet 内置的 cAdvisor 服务。在 kubelet 启动后，cAdvisor 服务也随之启动，而它能够提供的信息，可以细化到每一个容器的 CPU 、文件系统、内存、网络等资源的使用情况。<br />需要注意的是，这里提到的 Kubernetes 核心监控数据，其实使用的是 Kubernetes 的一个非常重要的扩展能力，叫作 Metrics Server。<br />Metrics Server 在 Kubernetes 社区的定位，其实是用来取代 Heapster 这个项目的。在 Kubernetes 项目发展的初期，Heapster 是用户获取 Kubernetes 监控数据（比如 Pod 和 Node 的资源使用情况） 的主要渠道。而后面提出来的 Metrics Server，则把这些信息，通过标准的 Kubernetes API 暴露了出来。这样，Metrics 信息就跟 Heapster 完成了解耦，允许 Heapster 项目慢慢退出舞台。<br />而有了 Metrics Server 之后，用户就可以通过标准的 Kubernetes API 来访问到这些监控数据了。比如，下面这个 URL：
```http
http://127.0.0.1:8001/apis/metrics.k8s.io/v1beta1/namespaces/<namespace-name>/pods/<pod-name>
```
当访问这个 Metrics API 时，它就会返回一个 Pod 的监控数据，而这些数据，其实是从 kubelet 的 Summary API （即 :/stats/summary）采集而来的。Summary API 返回的信息，既包括了 cAdVisor 的监控数据，也包括了 kubelet 本身汇总的信息。<br />需要指出的是， Metrics Server 并不是 kube-apiserver 的一部分，而是通过 Aggregator 这种插件机制，在独立部署的情况下同 kube-apiserver 一起统一对外服务的。<br />这里，Aggregator APIServer 的工作原理，可以用如下所示的一幅示意图来表示清楚：<br />![0b767b5224ad1906ddc4cce075618809[1].png](https://cdn.nlark.com/yuque/0/2022/png/396745/1642595342780-07b6831c-ebef-459b-83c1-00055ae92b2a.png#clientId=u357aa977-0e96-4&from=ui&id=u64371e7e&originHeight=367&originWidth=736&originalType=binary&ratio=1&rotation=0&showTitle=false&size=26285&status=done&style=shadow&taskId=u2e849fa5-2b3f-4051-87dc-3b9d2dd5c44&title=)<br />备注：图片出处[https://blog.jetstack.io/blog/resource-and-custom-metrics-hpa-v2/](https://blog.jetstack.io/blog/resource-and-custom-metrics-hpa-v2/)<br />可以看到，当 Kubernetes 的 API Server 开启了 Aggregator 模式之后，再访问 apis/metrics.k8s.io/v1beta1 的时候，实际上访问到的是一个叫作 kube-aggregator 的代理。而 kube-apiserver，正是这个代理的一个后端；而 Metrics Server，则是另一个后端。<br />而且，在这个机制下，还可以添加更多的后端给这个 kube-aggregator。所以 kube-aggregator 其实就是一个根据 URL 选择具体的 API 后端的代理服务器。通过这种方式，就可以很方便地扩展 Kubernetes 的 API 了。<br />而 Aggregator 模式的开启也非常简单：

- 如果是使用 kubeadm 或者[官方的 kube-up.sh 脚本](https://github.com/kubernetes/kubernetes/blob/master/cluster/kube-up.sh)部署 Kubernetes 集群的话，Aggregator 模式就是默认开启的；
- 如果是手动 DIY 搭建的话，就需要在 kube-apiserver 的启动参数里加上如下所示的配置：
```bash
--requestheader-client-ca-file=<path to aggregator CA cert>
--requestheader-allowed-names=front-proxy-client
--requestheader-extra-headers-prefix=X-Remote-Extra-
--requestheader-group-headers=X-Remote-Group
--requestheader-username-headers=X-Remote-User
--proxy-client-cert-file=<path to aggregator proxy cert>
--proxy-client-key-file=<path to aggregator proxy key>
```
而这些配置的作用，主要就是为 Aggregator 这一层设置对应的 Key 和 Cert 文件。而这些文件的生成，就需要自己手动完成了，具体流程请参考这篇[官方文档](https://github.com/kubernetes-incubator/apiserver-builder/blob/master/docs/concepts/auth.md)。<br />Aggregator 功能开启之后，只需要将 Metrics Server 的 YAML 文件部署起来，如下所示：
```bash
$ git clone https://github.com/kubernetes-incubator/metrics-server
$ cd metrics-server
$ kubectl create -f deploy/1.8+/
```
接下来，就会看到 metrics.k8s.io 这个 API 出现在了 Kubernetes API 列表当中。<br />在理解了 Prometheus 关心的三种监控数据源，以及 Kubernetes 的核心 Metrics 之后，作为用户，其实要做的就是将 Prometheus Operator 在 Kubernetes 集群里部署起来。然后，按照一开始介绍的架构，把上述 Metrics 源配置起来，让 Prometheus 自己去进行采集即可。
<a name="Q2kFq"></a>
### 小结
介绍了 Kubernetes 当前监控体系的设计，介绍了 Prometheus 项目在这套体系中的地位，以 Prometheus 为核心的监控系统的架构设计。<br />然后，解读了 Kubernetes 核心监控数据的来源，即：Metrics Server 的具体工作原理，以及 Aggregator APIServer 的设计思路。<br />对 Kubernetes 的监控体系形成一个整体的认知，体会到 Kubernetes 社区在监控这个事情上，全面以 Prometheus 项目为核心进行建设的大方向。<br />最后，在具体的监控指标规划上，建议遵循业界通用的 USE 原则和 RED 原则。<br />其中，USE 原则指的是，按照如下三个维度来规划资源监控指标：

1. 利用率（Utilization），资源被有效利用起来提供服务的平均时间占比；
2. 饱和度（Saturation），资源拥挤的程度，比如工作队列的长度；
3. 错误率（Errors），错误的数量。

而 RED 原则指的是，按照如下三个维度来规划服务监控指标：

1. 每秒请求数量（Rate）；
2. 每秒错误数量（Errors）；
3. 服务响应时间（Duration）。

不难发现， USE 原则主要关注的是“资源”，比如节点和容器的资源使用情况，而 RED 原则主要关注的是“服务”，比如 kube-apiserver 或者某个应用的工作情况。这两种指标，在、 Kubernetes + Prometheus 组成的监控体系中，都是可以完全覆盖到的。
<a name="EOM23"></a>
### Custom Metrics
不难看到，Prometheus 项目在Kubernetes 的核心监控体系的架构其中占据了最为核心的位置。<br />实际上，借助上述监控体系，Kubernetes 就可以提供一种非常有用的能力，那就是 Custom Metrics，自定义监控指标。<br />在过去的很多 PaaS 项目中，其实都有一种叫作 Auto Scaling，即自动水平扩展的功能。只不过，这个功能往往只能依据某种指定的资源类型执行水平扩展，比如 CPU 或者 Memory 的使用值。<br />而在真实的场景中，用户需要进行 Auto Scaling 的依据往往是自定义的监控指标。比如，某个应用的等待队列的长度，或者某种应用相关资源的使用情况。这些复杂多变的需求，在传统 PaaS 项目和其他容器编排项目里，几乎是不可能轻松支持的。<br />而凭借强大的 API 扩展机制，Custom Metrics 已经成为了 Kubernetes 的一项标准能力。并且，Kubernetes 的自动扩展器组件 Horizontal Pod Autoscaler （HPA）， 也可以直接使用 Custom Metrics 来执行用户指定的扩展策略，这里的整个过程都是非常灵活和可定制的。<br />不难想到，Kubernetes 里的 Custom Metrics 机制，也是借助 Aggregator APIServer 扩展机制来实现的。这里的具体原理是，当把 Custom Metrics APIServer 启动之后，Kubernetes 里就会出现一个叫作custom.metrics.k8s.io的 API。而当访问这个 URL 时，Aggregator 就会把请求转发给 Custom Metrics APIServer 。<br />而 Custom Metrics APIServer 的实现，其实就是一个 Prometheus 项目的 Adaptor。<br />比如，现在要实现一个根据指定 Pod 收到的 HTTP 请求数量来进行 Auto Scaling 的 Custom Metrics，这个 Metrics 就可以通过访问如下所示的自定义监控 URL 获取到：
```http
https://<apiserver_ip>/apis/custom-metrics.metrics.k8s.io/v1beta1/namespaces/default/pods/sample-metrics-app/http_requests 
```
这里的工作原理是，当访问这个 URL 的时候，Custom Metrics APIServer 就会去 Prometheus 里查询名叫 sample-metrics-app 这个 Pod 的 http_requests 指标的值，然后按照固定的格式返回给访问者。<br />当然，http_requests 指标的值，就需要由 Prometheus 从目标 Pod 上采集。<br />这里具体的做法有很多种，最普遍的做法，就是让 Pod 里的应用本身暴露出一个 `/metrics` API，然后在这个 API 里返回自己收到的 HTTP 的请求的数量。所以说，接下来 HPA 只需要定时访问前面提到的自定义监控 URL，然后根据这些值计算是否要执行 Scaling 即可。<br />接下来，通过一个具体的实例，来演示一下 Custom Metrics 具体的使用方式。这个实例的 [GitHub 库](https://github.com/resouer/kubeadm-workshop)，可以点击链接查看。在这个例子中，依然假设集群是 kubeadm 部署出来的，所以 Aggregator 功能已经默认开启了。
<a name="qmo59"></a>
#### 第一步，部署 Prometheus 项目。
这一步，使用 Prometheus Operator 来完成，如下所示：
```bash
$ kubectl apply -f demos/monitoring/prometheus-operator.yaml
clusterrole "prometheus-operator" created
serviceaccount "prometheus-operator" created
clusterrolebinding "prometheus-operator" created
deployment "prometheus-operator" created

$ kubectl apply -f demos/monitoring/sample-prometheus-instance.yaml
clusterrole "prometheus" created
serviceaccount "prometheus" created
clusterrolebinding "prometheus" created
prometheus "sample-metrics-prom" created
service "sample-metrics-prom" created
```
<a name="Nwi1Z"></a>
#### 第二步，需要把 Custom Metrics APIServer 部署起来
如下所示：
```bash
$ kubectl apply -f demos/monitoring/custom-metrics.yaml
namespace "custom-metrics" created
serviceaccount "custom-metrics-apiserver" created
clusterrolebinding "custom-metrics:system:auth-delegator" created
rolebinding "custom-metrics-auth-reader" created
clusterrole "custom-metrics-read" created
clusterrolebinding "custom-metrics-read" created
deployment "custom-metrics-apiserver" created
service "api" created
apiservice "v1beta1.custom-metrics.metrics.k8s.io" created
clusterrole "custom-metrics-server-resources" created
clusterrolebinding "hpa-controller-custom-metrics" created
```
<a name="GU0vK"></a>
#### 第三步，需要为 Custom Metrics APIServer 创建对应的 ClusterRoleBinding
以便能够使用 curl 来直接访问 Custom Metrics 的 API：
```bash
$ kubectl create clusterrolebinding allowall-cm --clusterrole custom-metrics-server-resources --user system:anonymous
clusterrolebinding "allowall-cm" created
```
<a name="sys5O"></a>
#### 第四步，把待监控的应用和 HPA 部署起来了
如下所示：
```bash
$ kubectl apply -f demos/monitoring/sample-metrics-app.yaml
deployment "sample-metrics-app" created
service "sample-metrics-app" created
servicemonitor "sample-metrics-app" created
horizontalpodautoscaler "sample-metrics-app-hpa" created
ingress "sample-metrics-app" created
```
这里，需要关注一下 HPA 的配置，如下所示：
```yaml
kind: HorizontalPodAutoscaler
apiVersion: autoscaling/v2beta1
metadata:
  name: sample-metrics-app-hpa
spec:
  scaleTargetRef:
    apiVersion: apps/v1
    kind: Deployment
    name: sample-metrics-app
  minReplicas: 2
  maxReplicas: 10
  metrics:
  - type: Object
    object:
      target:
        kind: Service
        name: sample-metrics-app
      metricName: http_requests
      targetValue: 100
```
可以看到，HPA 的配置，就是设置 Auto Scaling 规则的地方。<br />比如，scaleTargetRef 字段，就指定了被监控的对象是名叫 sample-metrics-app 的 Deployment，也就是上面部署的被监控应用。并且，它最小的实例数目是 2，最大是 10。<br />在 metrics 字段，指定了这个 HPA 进行 Scale 的依据，是名叫 http_requests 的 Metrics。而获取这个 Metrics 的途径，则是访问名叫 sample-metrics-app 的 Service。<br />有了这些字段里的定义， HPA 就可以向如下所示的 URL 发起请求来获取 Custom Metrics 的值了：
```bash
https://<apiserver_ip>/apis/custom-metrics.metrics.k8s.io/v1beta1/namespaces/default/services/sample-metrics-app/http_requests
```
需要注意的是，上述这个 URL 对应的被监控对象，是应用对应的 Service。这跟一开始举例用到的 Pod 对应的 Custom Metrics URL 是不一样的。当然，对于一个多实例应用来说，通过 Service 来采集 Pod 的 Custom Metrics 其实才是合理的做法。<br />这时候，可以通过一个名叫 hey 的测试工具来为应用增加一些访问压力，具体做法如下所示：
```bash
$ # Install hey
$ docker run -it -v /usr/local/bin:/go/bin golang:1.8 go get github.com/rakyll/hey

$ export APP_ENDPOINT=$(kubectl get svc sample-metrics-app -o template --template {{.spec.clusterIP}}); echo ${APP_ENDPOINT}
$ hey -n 50000 -c 1000 http://${APP_ENDPOINT}
```
与此同时，如果去访问应用 Service 的 Custom Metircs URL，就会看到这个 URL 已经可以返回应用收到的 HTTP 请求数量了，如下所示：
```bash
$ curl -sSLk https://<apiserver_ip>/apis/custom-metrics.metrics.k8s.io/v1beta1/namespaces/default/services/sample-metrics-app/http_requests
{
  "kind": "MetricValueList",
  "apiVersion": "custom-metrics.metrics.k8s.io/v1beta1",
  "metadata": {
    "selfLink": "/apis/custom-metrics.metrics.k8s.io/v1beta1/namespaces/default/services/sample-metrics-app/http_requests"
  },
  "items": [
    {
      "describedObject": {
        "kind": "Service",
        "name": "sample-metrics-app",
        "apiVersion": "/__internal"
      },
      "metricName": "http_requests",
      "timestamp": "2018-11-30T20:56:34Z",
      "value": "501484m"
    }
  ]
}
```
这里需要注意的是，Custom Metrics API 返回的 Value 的格式。<br />在为被监控应用编写 /metrics API 的返回值时，其实比较容易计算的，是该 Pod 收到的 HTTP request 的总数。所以，这个应用的代码其实是如下所示的样子：
```go
if (request.url == "/metrics") {
  response.end("# HELP http_requests_total The amount of requests served by the server in total\n# TYPE http_requests_total counter\nhttp_requests_total " + totalrequests + "\n");
  return;
}
```
可以看到，应用在 /metrics 对应的 HTTP response 里返回的，其实是 http_requests_total 的值。这，也就是 Prometheus 收集到的值。<br />而 Custom Metrics APIServer 在收到对 http_requests 指标的访问请求之后，它会从 Prometheus 里查询 http_requests_total 的值，然后把它折算成一个以时间为单位的请求率，最后把这个结果作为 http_requests 指标对应的值返回回去。<br />所以说，在对前面的 Custom Metircs URL 进行访问时，会看到值是 501484m，这里的格式，其实就是 milli-requests，相当于是在过去两分钟内，每秒有 501 个请求。这样，应用的开发者就无需关心如何计算每秒的请求个数了。而这样的“请求率”的格式，是可以直接被 HPA 拿来使用的。<br />这时候，如果同时查看 Pod 的个数的话，就会看到 HPA 开始增加 Pod 的数目了。<br />不过，在这里可能会有一个疑问，Prometheus 项目，又是如何知道采集哪些 Pod 的 /metrics API 作为监控指标的来源呢。<br />实际上，如果仔细观察一下前面创建应用的输出，会看到有一个类型是 ServiceMonitor 的对象也被创建了出来。它的 YAML 文件如下所示：
```yaml
apiVersion: monitoring.coreos.com/v1
kind: ServiceMonitor
metadata:
  name: sample-metrics-app
  labels:
    service-monitor: sample-metrics-app
spec:
  selector:
    matchLabels:
      app: sample-metrics-app
  endpoints:
  - port: web
```
这个 ServiceMonitor 对象，正是 Prometheus Operator 项目用来指定被监控 Pod 的一个配置文件。可以看到，其实是通过 Label Selector 为 Prometheus 来指定被监控应用的。
<a name="AcWSI"></a>
#### 小结
Kubernetes 里对自定义监控指标，即 Custom Metrics 的设计与实现机制。这套机制的可扩展性非常强，也终于使得 Auto Scaling 在 Kubernetes 里面不再是一个“食之无味”的鸡肋功能了。<br />另外可以看到，Kubernetes 的 Aggregator APIServer，是一个非常行之有效的 API 扩展机制。而且，Kubernetes 社区已经提供了一套叫作 [KubeBuilder](https://github.com/kubernetes-sigs/kubebuilder) 的工具库，生成一个 API Server 的完整代码框架，只需要在里面添加自定义 API，以及对应的业务逻辑即可。
<a name="HAeZC"></a>
## 容器日志收集与管理
首先需要明确的是，Kubernetes 里面对容器日志的处理方式，都叫作 cluster-level-logging，即：这个日志处理系统，与容器、Pod 以及 Node 的生命周期都是完全无关的。这种设计当然是为了保证，无论是容器挂了、Pod 被删除，甚至节点宕机的时候，应用的日志依然可以被正常获取到。<br />而对于一个容器来说，当应用把日志输出到 stdout 和 stderr 之后，容器项目在默认情况下就会把这些日志输出到宿主机上的一个 JSON 文件里。这样，通过 `kubectl logs` 命令就可以看到这些容器的日志了。<br />上述机制，就是容器日志收集的基础假设。而如果应用是把文件输出到其他地方，比如直接输出到了容器里的某个文件里，或者输出到了远程存储里，那就属于特殊情况了。<br />而 Kubernetes 本身，实际上是不会做容器日志收集工作的，所以为了实现上述 cluster-level-logging，需要在部署集群的时候，提前对具体的日志方案进行规划。而 Kubernetes 项目本身，主要推荐了三种日志方案。
<a name="r9b9t"></a>
### 第一种，在 Node 上部署 logging agent，将日志文件转发到后端存储里保存起来。
这个方案的架构图如下所示。<br />![b5515aed076aa6af63ace55b62d36243[1].jpg](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1642596956763-8df677be-a02e-4b2f-9c79-35c10750e845.jpeg#clientId=u357aa977-0e96-4&from=ui&id=u660ae457&originHeight=1610&originWidth=2284&originalType=binary&ratio=1&rotation=0&showTitle=false&size=305816&status=done&style=shadow&taskId=ue18e0114-4ab4-4d92-ab4f-a4e15680529&title=)<br />不难看到，这里的核心就在于 logging agent ，它一般都会以 DaemonSet 的方式运行在节点上，然后将宿主机上的容器日志目录挂载进去，最后由 logging-agent 把日志转发出去。<br />举个例子，可以通过 Fluentd 项目作为宿主机上的 logging-agent，然后把日志转发到远端的 ElasticSearch 里保存起来供将来进行检索。具体的操作过程，可以通过阅读[这篇文档](https://kubernetes.io/docs/user-guide/logging/elasticsearch)来了解。另外，在很多 Kubernetes 的部署里，会自动启用 [logrotate](https://linux.die.net/man/8/logrotate)，在日志文件超过 10MB 的时候自动对日志文件进行 rotate 操作。<br />可以看到，在 Node 上部署 logging agent 最大的优点，在于一个节点只需要部署一个 agent，并且不会对应用和 Pod 有任何侵入性。所以，这个方案，在社区里是最常用的一种。<br />但是也不难看到，这种方案的不足之处就在于，它要求应用输出的日志，都必须是直接输出到容器的 stdout 和 stderr 里。
<a name="jjImv"></a>
### 第二种，通过一个 sidecar 容器把这些日志文件重新输出到 sidecar 的 stdout 和 stderr 上
所以，Kubernetes 容器日志方案的第二种，就是对这种特殊情况的一个处理，即：当容器的日志只能输出到某些文件里的时候，可以通过一个 sidecar 容器把这些日志文件重新输出到 sidecar 的 stdout 和 stderr 上，这样就能够继续使用第一种方案了。这个方案的具体工作原理，如下所示。<br />![4863e3d7d1ef02a5a44e431369ac4120[1].jpg](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1642597071114-aaba6126-0f86-4a24-86c5-e62acd3b9fe9.jpeg#clientId=u357aa977-0e96-4&from=ui&id=u9786c043&originHeight=1610&originWidth=2284&originalType=binary&ratio=1&rotation=0&showTitle=false&size=279927&status=done&style=shadow&taskId=ub01677b6-e9a7-4572-9271-7a593b212ac&title=)<br />比如，现在的应用 Pod 只有一个容器，它会把日志输出到容器里的 /var/log/1.log 和 2.log 这两个文件里。这个 Pod 的 YAML 文件如下所示：
```yaml
apiVersion: v1
kind: Pod
metadata:
  name: counter
spec:
  containers:
  - name: count
    image: busybox
    args:
    - /bin/sh
    - -c
    - >
      i=0;
      while true;
      do
        echo "$i: $(date)" >> /var/log/1.log;
        echo "$(date) INFO $i" >> /var/log/2.log;
        i=$((i+1));
        sleep 1;
      done
    volumeMounts:
    - name: varlog
      mountPath: /var/log
  volumes:
  - name: varlog
    emptyDir: {}
```
在这种情况下，用 `kubectl logs` 命令是看不到应用的任何日志的。而且前面说的、最常用的方案一，也是没办法使用的。<br />那么这个时候，就可以为这个 Pod 添加两个 sidecar 容器，分别将上述两个日志文件里的内容重新以 stdout 和 stderr 的方式输出出来，这个 YAML 文件的写法如下所示：
```yaml
apiVersion: v1
kind: Pod
metadata:
  name: counter
spec:
  containers:
  - name: count
    image: busybox
    args:
    - /bin/sh
    - -c
    - >
      i=0;
      while true;
      do
        echo "$i: $(date)" >> /var/log/1.log;
        echo "$(date) INFO $i" >> /var/log/2.log;
        i=$((i+1));
        sleep 1;
      done
    volumeMounts:
    - name: varlog
      mountPath: /var/log
  - name: count-log-1
    image: busybox
    args: [/bin/sh, -c, 'tail -n+1 -f /var/log/1.log']
    volumeMounts:
    - name: varlog
      mountPath: /var/log
  - name: count-log-2
    image: busybox
    args: [/bin/sh, -c, 'tail -n+1 -f /var/log/2.log']
    volumeMounts:
    - name: varlog
      mountPath: /var/log
  volumes:
  - name: varlog
    emptyDir: {}
```
这时候，就可以通过 `kubectl logs` 命令查看这两个 sidecar 容器的日志，间接看到应用的日志内容了，如下所示：
```bash
$ kubectl logs counter count-log-1
0: Mon Jan 1 00:00:00 UTC 2001
1: Mon Jan 1 00:00:01 UTC 2001
2: Mon Jan 1 00:00:02 UTC 2001
...
$ kubectl logs counter count-log-2
Mon Jan 1 00:00:00 UTC 2001 INFO 0
Mon Jan 1 00:00:01 UTC 2001 INFO 1
Mon Jan 1 00:00:02 UTC 2001 INFO 2
...
```
由于 sidecar 跟主容器之间是共享 Volume 的，所以这里的 sidecar 方案的额外性能损耗并不高，也就是多占用一点 CPU 和内存罢了。<br />但需要注意的是，这时候，宿主机上实际上会存在两份相同的日志文件：一份是应用自己写入的；另一份则是 sidecar 的 stdout 和 stderr 对应的 JSON 文件。这对磁盘是很大的浪费。所以说，除非万不得已或者应用容器完全不可能被修改，否则还是建议直接使用方案一，或者直接使用下面的第三种方案。
<a name="UpHW0"></a>
### 第三种方案，就是通过一个 sidecar 容器，直接把应用的日志文件发送到远程存储里面去。
也就是相当于把方案一里的 logging agent，放在了应用 Pod 里。这种方案的架构如下所示：<br />![d464401baec60c11f96dfeea3ae3a9c7[1].jpg](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1642597209549-f9814834-b690-4915-939c-488e8159af0b.jpeg#clientId=u357aa977-0e96-4&from=ui&id=ueca7652f&originHeight=998&originWidth=2284&originalType=binary&ratio=1&rotation=0&showTitle=false&size=179912&status=done&style=shadow&taskId=ua0c880fb-c2de-4152-ae35-f2042273e3b&title=)<br />在这种方案里，应用还可以直接把日志输出到固定的文件里而不是 stdout，logging-agent 还可以使用 fluentd，后端存储还可以是 ElasticSearch。只不过， fluentd 的输入源，变成了应用的日志文件。一般来说，会把 fluentd 的输入源配置保存在一个 ConfigMap 里，如下所示：
```yaml
apiVersion: v1
kind: ConfigMap
metadata:
  name: fluentd-config
data:
  fluentd.conf: |
    <source>
      type tail
      format none
      path /var/log/1.log
      pos_file /var/log/1.log.pos
      tag count.format1
    </source>
    
    <source>
      type tail
      format none
      path /var/log/2.log
      pos_file /var/log/2.log.pos
      tag count.format2
    </source>
    
    <match **>
      type google_cloud
    </match>
```
然后，在应用 Pod 的定义里，就可以声明一个 Fluentd 容器作为 sidecar，专门负责将应用生成的 1.log 和 2.log 转发到 ElasticSearch 当中。这个配置，如下所示：
```yaml
apiVersion: v1
kind: Pod
metadata:
  name: counter
spec:
  containers:
  - name: count
    image: busybox
    args:
    - /bin/sh
    - -c
    - >
      i=0;
      while true;
      do
        echo "$i: $(date)" >> /var/log/1.log;
        echo "$(date) INFO $i" >> /var/log/2.log;
        i=$((i+1));
        sleep 1;
      done
    volumeMounts:
    - name: varlog
      mountPath: /var/log
  - name: count-agent
    image: k8s.gcr.io/fluentd-gcp:1.30
    env:
    - name: FLUENTD_ARGS
      value: -c /etc/fluentd-config/fluentd.conf
    volumeMounts:
    - name: varlog
      mountPath: /var/log
    - name: config-volume
      mountPath: /etc/fluentd-config
  volumes:
  - name: varlog
    emptyDir: {}
  - name: config-volume
    configMap:
      name: fluentd-config
```
可以看到，这个 Fluentd 容器使用的输入源，就是通过引用前面编写的 ConfigMap 来指定的。这里用到了 Projected Volume 来把 ConfigMap 挂载到 Pod 里。<br />需要注意的是，这种方案虽然部署简单，并且对宿主机非常友好，但是这个 sidecar 容器很可能会消耗较多的资源，甚至拖垮应用容器。并且，由于日志还是没有输出到 stdout 上，所以通过 `kubectl logs` 是看不到任何日志输出的。<br />以上，就是 Kubernetes 项目对容器应用日志进行管理最常用的三种手段了。
<a name="Ebl30"></a>
### 小结
这里分析了 Kubernetes 项目对容器应用日志的收集方式。综合对比以上三种方案，比较建议将应用日志输出到 stdout 和 stderr，然后通过在宿主机上部署 logging-agent 的方式来集中处理日志。<br />这种方案不仅管理简单，kubectl logs 也可以用，而且可靠性高，并且宿主机本身，很可能就自带了 rsyslogd 等非常成熟的日志收集组件来供你使用。<br />除此之外，还有一种方式就是在编写应用的时候，就直接指定好日志的存储后端，如下所示：<br />![13e8439d9945fea58c9672fc4ca30799[1].jpg](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1642597347042-ce55dd8d-88b1-46ac-a880-8cf2ffe73700.jpeg#clientId=u357aa977-0e96-4&from=ui&id=u7fe9f4fa&originHeight=705&originWidth=2284&originalType=binary&ratio=1&rotation=0&showTitle=false&size=136733&status=done&style=shadow&taskId=u3a0afca0-9bf2-427a-ad41-75c0b56e9dd&title=)<br />在这种方案下，Kubernetes 就完全不必操心容器日志的收集了，这对于本身已经有完善的日志处理系统的公司来说，是一个非常好的选择。<br />最后需要指出的是，无论是哪种方案，都必须要及时将这些日志文件从宿主机上清理掉，或者给日志目录专门挂载一些容量巨大的远程盘。否则，一旦主磁盘分区被打满，整个系统就可能会陷入崩溃状态，这是非常麻烦的。
