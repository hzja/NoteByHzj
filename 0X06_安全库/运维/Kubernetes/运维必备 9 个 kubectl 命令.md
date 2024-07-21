K8S<br />kubectl 是 Kubernetes 的一个命令行管理工具，可用于 Kubernetes 上的应用部署和日常管理。列举 9 个常见的 kubectl 命令，并总结了一些使用技巧，可以帮助系统管理员简化管理工作。
<a name="Gz3aK"></a>
## 使用 Kubectl 查询、创建、编辑和删除资源
对于刚开始使用命令行工具的开发者，最保险的方法是提出问题（读取操作），而不是发出命令（写入操作），所以从使用 `get` 命令开始是个不错的选择。
<a name="keqfd"></a>
### `Kubectl get`
使用 `get` 命令可以获取当前集群中可用的资源列表，包括：

- Namespace
- Pod
- Node
- Deployment
- Service
- ReplicaSet

每个 `get` 命令都能提供集群中可用资源的详细信息。例如 `get nodes` 命令就提供了 Kubernetes 的状态和版本。
```bash
$ kubectl get nodes
NAME       STATUS   ROLES    AGE   VERSION
minikube   Ready    master   9d    v1.18.0
```
这些命令大多数还具有简写版本。例如，要获取命名空间，可以使用 `kubectl get namespaces` 或者 `kubectl get ns` 命令:
```bash
$ kubectl get ns
NAME              STATUS   AGE
charts            Active   8d
default           Active   9d
kube-node-lease   Active   9d
kube-public       Active   9d
kube-system       Active   9d
```
每个 `get` 命令都可以使用 `–namespace` 或 `-n` 参数指定对应的命名空间。这点对于查看 `kube-system` 中的 Pods 会非常有用，因为这些 Pods 是 Kubernetes 自身运行所需的服务。
```bash
$ kubectl get pods -n kube-system
NAME                               READY   STATUS    RESTARTS   AGE
coredns-66bff467f8-mjptx           1/1     Running   2          9d
coredns-66bff467f8-t2xcz           1/1     Running   2          9d
etcd-minikube                      1/1     Running   1          9d
kube-apiserver-minikube            1/1     Running   1          9d
kube-controller-manager-minikube   1/1     Running   2          9d
kube-proxy-rpc9d                   1/1     Running   1          9d
kube-scheduler-minikube            1/1     Running   2          9d
storage-provisioner                1/1     Running   1          9d
```
<a name="KS3kM"></a>
### Kubectl create
可以查询资源后，下一步是创建资源。可以用 kubectl 在集群中创建任何类型的资源，包括：

- Service
- Cronjob
- Deployment
- Job
- Namespace（ns）

其中，一些资源的创建需要设置配置文件、命名空间以及资源名称。例如，创建命名空间就需要一个额外参数来指定命名空间。
```bash
$ kubectl create ns hello-there
namespace/hello-there created
```
Linux 里可以使用 cron 创建定时运行的任务。同样的，这里使用 `cronjob` 每五秒钟返回一次“hello”。
```bash
$ kubectl create cronjob my-cron --image=busybox --schedule="*/5 * * * *" -- echo hello
cronjob.batch/my-namespaced-cron created
```
也可以使用 `cronjob` 的简写版本 `cj`。
```bash
$ kubectl create cj my-existing-cron --image=busybox --schedule="*/15 * * * *" -- echo hello
cronjob.batch/my-existing-cron created
```
<a name="XBcy9"></a>
### `Kubectl edit`
当创建好资源后，如果需要修改，该怎么办？这时候就需要 `kubectl edit` 命令了。<br />可以用这个命令编辑集群中的任何资源。它会打开默认文本编辑器。如果要编辑现有的 cron job，则可以执行：
```bash
$ kubectl edit cronjob/my-existing-cron
```
要编辑的配置如下：
```yaml
# Please edit the object below. Lines beginning with a '#' will be ignored,
# and an empty file will abort the edit. If an error occurs while saving this file will be
# reopened with the relevant failures.
#
apiVersion: batch/v1beta1
kind: CronJob
metadata:
  creationTimestamp: "2020-04-19T16:06:06Z"
  managedFields:
  - apiVersion: batch/v1beta1
    fieldsType: FieldsV1
    fieldsV1:
      f:spec:
        f:concurrencyPolicy: {}
        f:failedJobsHistoryLimit: {}
        f:jobTemplate:
          f:metadata:
            f:name: {}
          f:spec:
            f:template:
              f:spec:
                f:containers:
                  k:{"name":"my-new-cron"}:
                    .: {}
                    f:command: {}
                    f:image: {}
                    f:imagePullPolicy: {}
```
原本调度间隔设置为 15 秒：<br />![2021-05-06-21-31-00-323996.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620308411613-2ebdc0fb-0bab-43cd-a7a6-1d5e66939c0d.png#clientId=ude2ec143-c7bf-4&from=ui&id=ua7469cd1&originHeight=65&originWidth=297&originalType=binary&size=6950&status=done&style=none&taskId=u6f9063d4-650a-405c-bb60-b937a5d099b)<br />将其更改为每 25 秒：<br />![2021-05-06-21-31-00-421060.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620308430467-551b94cc-5544-495a-b341-81181cbefde4.png#clientId=ude2ec143-c7bf-4&from=ui&id=ube8118e6&originHeight=109&originWidth=324&originalType=binary&size=10006&status=done&style=none&taskId=uaeaae2b5-c2aa-4ae2-adb6-f411df74794)<br />编写完成后，可以看到修改已生效。
```bash
$ kubectl edit cronjob/my-existing-croncronjob.batch/my-existing-cron edited
```
另外，可以通过 `KUBE_EDITOR` 命令来使用其他编辑器。
```bash
$ KUBE_EDITOR="nano" kubectl edit cronjob/my-existing-cron
```
<a name="IkL53"></a>
### `Kubectl delete`
学会了以上命令后，下面将进行删除操作。刚刚编辑的 `cronjob` 是两个 `cronjobs` 之一，现在删除整个资源。
```bash
$ kubectl delete cronjob my-existing-cron
cronjob.batch "my-existing-cron" deleted
```
需要注意的是，如果不知道资源是否有关联信息，最好不要删除。因为删除后无法恢复，只能重新创建。
<a name="WHNwP"></a>
### `Kubectl apply`
某些命令需要配置文件，而 `apply` 命令可以在集群内调整配置文件应用于资源。虽然也可以通过命令行 `standard in (STNIN)` 来完成，但 `apply` 命令更好一些，因为它可以让你知道如何使用集群，以及要应用哪种配置文件。作为示例，下面将 Helm 的基于角色的访问控制（RBAC）配置用于服务帐户。
```bash
$ kubectl apply -f commands.yaml
serviceaccount/tiller created
clusterrolebinding.rbac.authorization.k8s.io/tiller created
```
可以应用几乎任何配置，但是一定要明确所要应用的配置，否则可能会引发意料之外的后果。
<a name="Fnuv4"></a>
## 使用 Kubectl 对 Kubernetes 进行故障排除
<a name="WXz6i"></a>
### `Kubectl describe`
`describe` 命令可以查看资源的详细信息。比较常见的用法是查看一个 Pod 或节点信息，以检查是否有异常、资源是否耗尽。<br />该命令可以查看的资源包括：

- Nodes
- Pods
- Services
- Deployments
- Replica sets
- Cronjobs

举个例子，用 `describe` 命令查看上文集群中 `cronjob` 的详细信息。
```bash
$ kubectl describe cronjob my-cron
```
以下是部分信息：
```bash
Name:                         my-cron
Namespace:                    default
Labels:                       <none>
Annotations:                  <none>
Schedule:                     */5 * * * *
Concurrency Policy:           Allow
Suspend:                      False
Successful Job History Limit: 3
Failed Job History Limit:     1
Starting Deadline Seconds:    <unset>
Selector:                     <unset>
Parallelism:                  <unset>
Completions:                  <unset>
Pod Template:
  Labels: <none>
  Containers:
   my-cron:
    Image:     busybox
    Port:      <none>
    Host Port: <none>
```
<a name="yf04s"></a>
### `Kubectl logs`
虽然 `describe` 命令可以让你知道 Pod 内部应用程序发生的事，但 `logs` 命令可以提供 Kubernetes 中 Pod 的更多详细信息。了解这种区别可以帮助开发者更好地对应用程序内部以及 Kubernetes 内部发生的问题，并进行故障排除，这二者往往并不相同。
```bash
$ kubectl logs cherry-chart-88d49478c-dmcfv -n charts
```
以上命令的部分输出结果如下：
```bash
172.17.0.1 - - [19/Apr/2020:16:01:15 +0000] "GET / HTTP/1.1" 200 612 "-" 
"kube-probe/1.18" "-"
172.17.0.1 - - [19/Apr/2020:16:01:20 +0000] "GET / HTTP/1.1" 200 612 "-" 
"kube-probe/1.18" "-"
172.17.0.1 - - [19/Apr/2020:16:01:25 +0000] "GET / HTTP/1.1" 200 612 "-" 
"kube-probe/1.18" "-"
172.17.0.1 - - [19/Apr/2020:16:01:30 +0000] "GET / HTTP/1.1" 200 612 "-" 
"kube-probe/1.18" "-"
172.17.0.1 - - [19/Apr/2020:16:01:35 +0000] "GET / HTTP/1.1" 200 612 "-" 
"kube-probe/1.18" "-"
172.17.0.1 - - [19/Apr/2020:16:01:40 +0000] "GET / HTTP/1.1" 200 612 "-" 
"kube-probe/1.18" "-"
172.17.0.1 - - [19/Apr/2020:16:01:45 +0000] "GET / HTTP/1.1" 200 612 "-" 
"kube-probe/1.18" "-"
172.17.0.1 - - [19/Apr/2020:16:01:50 +0000] "GET / HTTP/1.1" 200 612 "-" 
"kube-probe/1.18" "-"
172.17.0.1 - - [19/Apr/2020:16:01:55 +0000] "GET / HTTP/1.1" 200 612 "-" 
"kube-probe/1.18" "-"
```
`grep` 命令可以过滤无关信息或查看特定事件。例如，下面的 `kube-probe` 可能是无关信息，用 `grep` 命令对其进行过滤。
```bash
$ kubectl logs cherry-chart-88d49478c-dmcfv -n charts | grep -vie kube-probe
127.0.0.1 - - [10/Apr /2020:23:01:55 +0000] "GET / HTTP/1.1" 200 612 "-" "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:75.0) Gecko/20100101 Firefox/75.0" “-”
```
在有些部署中，存在一个 Pod 有多个容器的情况，因此可以在 `logs` 命令中使用 `-c <容器名称>`，以查找指定容器的日志。
<a name="mGYwe"></a>
### `Kubectl exec`
与 `docker exec` 命令相似，`exec` 命令也可以在容器中直接对应用程序进行故障排除。尤其当 Pod 的日志无法定位问题时， 它会特别好用。另外要注意的是，使用 `exec` 命令时，必须要以 Pod 内使用的 shell 作为命令的最后一个参数。
```bash
$ kubectl exec -it cherry-chart-88d49478c-dmcfv -n charts -- /bin/bash
root@cherry-chart-88d49478c-dmcfv:/#
```
<a name="Lfjzq"></a>
### `Kubectl cp`
`Kubectl cp` 命令与 Linux `cp` 命令类似，用于容器之间复制文件和目录。另外，该命令还能在自动化失败等紧急情况下进行恢复备份。<br />以下是将本地文件拷贝到容器的示例。命令格式为：`kubectl cp <filename> <namespace/podname:/path/tofile>`。
```bash
$ kubectl cp commands_copy.txt charts/cherry-chart-88d49478c-dmcfv:commands.txt
$ kubectl exec -it cherry-chart-88d49478c-dmcfv -n charts -- /bin/bash
root@cherry-chart-88d49478c-dmcfv:/# ls
bin  boot  commands.txt  dev  etc  home  lib  lib64  media  mnt  opt  proc  root  run  sbin  srv  sys  tmp  usr  var
```
下面是将容器内的文件拷贝到本地计算机上的示例。命令格式为：`kubectl cp <namespace/podname:/path/tofile>`。
```bash
$ kubectl cp charts/cherry-chart-88d49478c-dmcfv:commands.txt commands_copy.txt
$ ls
commands_copy.txt
```
