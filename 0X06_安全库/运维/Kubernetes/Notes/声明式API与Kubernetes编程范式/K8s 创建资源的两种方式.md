Kubernetes<br />在 K8s 中，创建资源有两种方式：

- 直接使用 `kubectl run` 命令创建
- 使用 `kubectl create/apply` 命令从 YAML 文件创建

通过示例解释这两种方法，以及它们分别应该在何时使用。
<a name="FU6ZW"></a>
## `kubectl run`
`kubectl run` 命令在 pod 中创建并运行特定的镜像。例如：
```bash
$ kubectl run nginx --image=nginx --port 80
pod/nginx created
$ kubectl get po nginx
NAME    READY   STATUS    RESTARTS   AGE
nginx   1/1     Running   0          25s
```
如果对这个新 pod 执行 kubectl describe 命令，将看到：
```bash
$ kubectl describe po nginx
Name:         nginx
Namespace:    default
Priority:     0
Node:         ip-10-0-0-146.ec2.internal/10.0.0.146
Start Time:   Sat, 09 Apr 2022 16:56:29 -0400
Labels:       run=nginx
Annotations:  kubernetes.io/psp: eks.privileged
Status:       Running
IP:           10.0.0.69
IPs:
  IP:  10.0.0.69
```
请注意，Labels 是 run=nginx。<br />还可以在 run 命令中指定环境变量，例如：
```bash
$ kubectl run nginx --image=nginx --port 80 --env="DNS_DOMAIN=cluster" --env="POD_NAMESPACE=default"
```
如果现在执行 `kubectl describe nginx`，就可以看到刚刚提供的新环境变量：
```bash
$ kubectl describe po nginx | grep "Env" -A2
    Environment:
      DNS_DOMAIN:     cluster
      POD_NAMESPACE:  default
```
通常，`kubectl run` 命令用于一些简单、直观和快速的任务，它适用于 ad-hoc（点对点网络模式）的测试或实验。
<a name="udoeB"></a>
## `kubectl kubectl create/apply`
`kubectl create/apply` 命令基于给定的配置文件。该配置文件具有以下特点：

1. 配置文件描述了应用程序最终将达到的 Whatstate。
2. 配置文件提供创建资源的模板，可以重复部署。
3. 部署可以像代码一样进行管理。
4. 适用于正式的、跨环境的、大规模的部署。
5. 需要熟悉配置文件的语法才能使用此方法。

再看一个例子。创建相同的 nginx pod，这一次，通过 YAML 文件来完成：
```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: nginx
  labels:
    app: nginx
spec:
  replicas: 1
  selector:
    matchLabels:
      app: nginx
  template:
    metadata:
      labels:
        app: nginx
    spec:
      containers:
        - name: nginx
          image: nginx
          ports:
        - containerPort: 80
```
不用在意 Deployment 文件语法。<br />使用 `kubectl create` 命令来创建资源：
```bash
$ kubectl create -f nginx-deployment.yaml
deployment.apps/nginx created
$ kubectl get po
NAME                     READY   STATUS    RESTARTS   AGE
nginx-6799fc88d8-k7tfl   1/1     Running   0          5s
$ kubectl delete -f nginx-deployment.yaml
deployment.apps "nginx" deleted
```
还可以使用 `kubectl apply` 命令：
```bash
$ kubectl apply -f nginx-deployment.yaml
deployment.apps/nginx created
$ kubectl get po
NAME                     READY   STATUS    RESTARTS   AGE
nginx-6799fc88d8-2fsxk   1/1     Running   0          17s
$ kubectl delete -f nginx-deployment.yaml
deployment.apps "nginx" deleted
```
可以看到，create 和 apply 命令都可以用来创建资源，但是二者有什么区别呢？
<a name="aUpGU"></a>
## `kubectl create` 与 `kubectl apply`
看看 create 和 apply 之间的区别。
<a name="xnn8J"></a>
### create
此命令将告诉 K8s API 服务器，要创建、删除或替换一个或多个资源。以更简化的方式，这意味着可以从头开始**创建**一个全新的对象。或者，它通过定义需求对任何现有对象进行一些更改。
<a name="MKrIg"></a>
### apply
此命令意味着通过在给定的 YAML 文件中声明确切需要的内容来更改已经存在的对象。<br />为了演示，使用 nginx Deployment 文件。首先创建资源：
```bash
$ kubectl create -f nginx-deployment.yaml
deployment.apps/nginx created
```
现在，假设要向 pod 添加一个 label，将 label 更新为：
```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: nginx
  labels:
    app: nginx
    env: prod
spec:
  replicas: 1
  selector:
    matchLabels:
      app: nginx
  template:
    metadata:
      labels:
        app: nginx
        env: prod
    spec:
      containers:
        - name: nginx
          image: nginx
```
如果再次运行 `create` 命令：
```bash
$ kubectl create -f nginx-deployment.yaml
Error from server (AlreadyExists): error when creating "nginx-deployment.yaml": deployments.apps "nginx" already exists
```
即使 YAML 文件略有不同，也会收到 **“nginx” 已存在的**错误。<br />那如果运行 `kubectl apply` 呢？试一试：
```bash
$ kubectl apply -f nginx-deployment.yaml
deployment.apps/nginx configured
```
现在描述一下资源：
```bash
$ kubectl get po
NAME                     READY   STATUS    RESTARTS   AGE
nginx-595f565474-zkl4t   1/1     Running   0          3m39s
$ kubectl describe po nginx-595f565474-zkl4t
Name:         nginx-595f565474-zkl4t
Namespace:    default
Priority:     0
Node:         ip-10-0-0-146.ec2.internal/10.0.0.146
Start Time:   Sat, 09 Apr 2022 17:20:04 -0400
Labels:       app=nginx
              env=prod
              pod-template-hash=595f565474
```
可以看到应用了新的 labels 是 env=prod。<br />现在了解了 `kubectl create` 和 `kubectl apply` 之间的区别，可能想知道应该使用哪一个？<br />在 `kubectl create` 命令中，指定了一个**特定行为**，也就是 create，因此它是一种更具命令式的方法。在 `kubectl apply` 命令中，指定系统的**目标状态**，而不指定一个特定的行为，因此它是更具声明性的方法。让系统决定采取什么行动。如果资源不存在，它将创建它，如果资源存在，则它将配置应用于现有资源。<br />简单来说，如果对单个文件运行一个操作来创建资源，create 和 apply 基本是相同的。但是，apply 允许在一个目录中的多个文件上同时创建和修补。
