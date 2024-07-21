Kubernetes 监控指标
<a name="Qeh6r"></a>
## 对比
node-exporter用于采集服务器层面的运行指标，包括机器的loadavg、filesystem、meminfo等基础监控，类似于传统主机监控维度的zabbix-agent。<br />metric-server/heapster是从api-server中获取CPU、内存使用率这种监控指标，并把他们发送给存储后端，如InfluxDB或云厂商，他当前的核心作用是：为HPA等组件提供决策指标支持。<br />kube-state-metrics关注于获取Kubernetes各种资源的最新状态，如Deployment或者DaemonSet。<br />例如：

- 调度了多少个Replicas？现在可用的有几个？
- 多少个Pod是running/stopped/terminated状态？
- Pod重启了多少次？
- 有多少job在运行中?

这些指标都由kube-state-metrics提供。<br />之所以没有把kube-state-metrics纳入到metric-server的能力中，是因为他们的关注点本质上是不一样的。

- metric-server仅仅是获取、格式化现有数据，写入特定的存储，实质上是一个监控系统。
- kube-state-metrics是将Kubernetes的运行状况在内存中做了个快照，并且获取新的指标，但他没有能力导出这些指标。
<a name="fzdn9"></a>
### 部署metric-server
下载metric-server部署的yaml文件到本地。
```bash
wget https://github.com/kubernetes-sigs/metrics-server/releases/download/v0.3.7/components.yaml
```
拉取metric-server的镜像到本地：
```bash
# docker pull zhaoqinchang/metrics-server:0.3.7
0.3.7: Pulling from zhaoqinchang/metrics-server
9ff2acc3204b: Pull complete 
9d14b55ff9a0: Pull complete 
Digest: sha256:c0efe772bb9e5c289db6cc4bc2002c268507d0226f2a3815f7213e00261c38e9
Status: Downloaded newer image for zhaoqinchang/metrics-server:0.3.7
docker.io/zhaoqinchang/metrics-server:0.3.7
```
修改components.yaml文件为如下内容：
```yaml
# cat components.yaml 
---
apiVersion: rbac.authorization.k8s.io/v1
kind: ClusterRole
metadata:
  name: system:aggregated-metrics-reader
  labels:
    rbac.authorization.k8s.io/aggregate-to-view: "true"
    rbac.authorization.k8s.io/aggregate-to-edit: "true"
    rbac.authorization.k8s.io/aggregate-to-admin: "true"
rules:
- apiGroups: ["metrics.k8s.io"]
  resources: ["pods", "nodes"]
  verbs: ["get", "list", "watch"]
---
apiVersion: rbac.authorization.k8s.io/v1
kind: ClusterRoleBinding
metadata:
  name: metrics-server:system:auth-delegator
roleRef:
  apiGroup: rbac.authorization.k8s.io
  kind: ClusterRole
  name: system:auth-delegator
subjects:
- kind: ServiceAccount
  name: metrics-server
  namespace: kube-system
---
apiVersion: rbac.authorization.k8s.io/v1
kind: RoleBinding
metadata:
  name: metrics-server-auth-reader
  namespace: kube-system
roleRef:
  apiGroup: rbac.authorization.k8s.io
  kind: Role
  name: extension-apiserver-authentication-reader
subjects:
- kind: ServiceAccount
  name: metrics-server
  namespace: kube-system
---
apiVersion: apiregistration.k8s.io/v1beta1
kind: APIService
metadata:
  name: v1beta1.metrics.k8s.io
spec:
  service:
    name: metrics-server
    namespace: kube-system
  group: metrics.k8s.io
  version: v1beta1
  insecureSkipTLSVerify: true
  groupPriorityMinimum: 100
  versionPriority: 100
---
apiVersion: v1
kind: ServiceAccount
metadata:
  name: metrics-server
  namespace: kube-system
---
apiVersion: apps/v1
kind: Deployment
metadata:
  name: metrics-server
  namespace: kube-system
  labels:
    k8s-app: metrics-server
spec:
  selector:
    matchLabels:
      k8s-app: metrics-server
  template:
    metadata:
      name: metrics-server
      labels:
        k8s-app: metrics-server
    spec:
      serviceAccountName: metrics-server
      volumes:
      # mount in tmp so we can safely use from-scratch images and/or read-only containers
      - name: tmp-dir
        emptyDir: {}
      containers:
      - name: metrics-server
        image: zhaoqinchang/metrics-server:0.3.7    #修改镜像为刚刚拉取下来的镜像
        imagePullPolicy: IfNotPresent
        args:
          - --cert-dir=/tmp
          - --secure-port=4443
        command:                 #添加以下三行command命令
            - /metrics-server
            - --kubelet-preferred-address-types=InternalIP
            - --kubelet-insecure-tls
        ports:
        - name: main-port
          containerPort: 4443
          protocol: TCP
        securityContext:
          readOnlyRootFilesystem: true
          runAsNonRoot: true
          runAsUser: 1000
        volumeMounts:
        - name: tmp-dir
          mountPath: /tmp
      nodeSelector:
        kubernetes.io/os: linux
---
apiVersion: v1
kind: Service
metadata:
  name: metrics-server
  namespace: kube-system
  labels:
    kubernetes.io/name: "Metrics-server"
    kubernetes.io/cluster-service: "true"
spec:
  selector:
    k8s-app: metrics-server
  ports:
  - port: 443
    protocol: TCP
    targetPort: main-port
---
apiVersion: rbac.authorization.k8s.io/v1
kind: ClusterRole
metadata:
  name: system:metrics-server
rules:
- apiGroups:
  - ""
  resources:
  - pods
  - nodes
  - nodes/stats
  - namespaces
  - configmaps
  verbs:
  - get
  - list
  - watch
---
apiVersion: rbac.authorization.k8s.io/v1
kind: ClusterRoleBinding
metadata:
  name: system:metrics-server
roleRef:
  apiGroup: rbac.authorization.k8s.io
  kind: ClusterRole
  name: system:metrics-server
subjects:
- kind: ServiceAccount
  name: metrics-server
  namespace: kube-system
```
部署metric-server：
```bash
# kubectl apply  -f components.yaml 
clusterrole.rbac.authorization.k8s.io/system:aggregated-metrics-reader created
clusterrolebinding.rbac.authorization.k8s.io/metrics-server:system:auth-delegator created
rolebinding.rbac.authorization.k8s.io/metrics-server-auth-reader created
apiservice.apiregistration.k8s.io/v1beta1.metrics.k8s.io created
serviceaccount/metrics-server created
deployment.apps/metrics-server created
service/metrics-server created
clusterrole.rbac.authorization.k8s.io/system:metrics-server created
clusterrolebinding.rbac.authorization.k8s.io/system:metrics-server created
```
查看metric.k8s.io是否出现在Kubernetes集群的API群组列表中：
```bash
# kubectl api-versions | grep metrics 
metrics.k8s.io/v1beta1
```
<a name="tb4j2"></a>
## 使用
`kubectl top`命令可显示节点和Pod对象的资源使用信息，它依赖于集群中的资源指标API来收集各项指标数据。它包含有Node和Pod两个子命令，可分别显示Node对象和Pod对象的相关资源占用率。<br />列出Node资源占用率命令的语法格式为“`kubectl top node [-l label | NAME]`”，例如下面显示所有节点的资源占用状况的结果中显示了各节点累计CPU资源占用时长及百分比，以及内容空间占用量及占用比例。必要时，也可以在命令直接给出要查看的特定节点的标识，以及使用标签选择器进行节点过滤。
```bash
[root@master metric]# kubectl top nodes
NAME      CPU(cores)   CPU%   MEMORY(bytes)   MEMORY%   
master    282m         14%    1902Mi          51%       
node-02   70m          3%     1371Mi          37%       
node-03   121m         1%     892Mi           11%
```
而名称空间级别的Pod对象资源占用率的使用方法会略有不同，使用时，一般应该跟定名称空间及使用标签选择器过滤出目标Pod对象。例如，下面显示kube-system名称空间下的Pod资源使用状况：
```bash
[root@master metric]# kubectl top pods -n kube-system
NAME                              CPU(cores)   MEMORY(bytes)   
etcd-master                       32m          300Mi           
kube-apiserver-master             86m          342Mi           
kube-controller-manager-master    30m          48Mi            
kube-flannel-ds-l5ghn             5m           10Mi            
kube-flannel-ds-rqlm2             4m           12Mi            
kube-flannel-ds-v92r9             4m           14Mi            
kube-proxy-7vjcv                  18m          15Mi            
kube-proxy-xrz8f                  13m          21Mi            
kube-proxy-zpwn6                  1m           14Mi            
kube-scheduler-master             7m           17Mi            
metrics-server-5549c7694f-7vb66   2m           14Mi
```
`kubectl top`命令为用户提供简洁、快速获取Node对象及Pod对象系统资源占用状况的接口，是集群运行和维护的常用命令之一。
