Kubernetes<br />容器在主机的内核上运行，并获得时钟，但时区不是来自内核，而是来自用户空间。在大多数情况下，默认使用协调世界时 (UTC)。<br />时区的不一致，会带来很多困扰。即使代码与时区无关，但容器日志与系统日志时间相关联排查问题也会让人头疼。一些应用程序使用机器的时区作为默认时区，并希望用户设置时区。当集群中容器的时区不一致时，管理会很不容易。
<a name="Ff2Ju"></a>
## k8tz
k8tz是开源项目，请查看：[github.com/k8tz/k8tz](https://github.com/k8tz/k8tz)<br />k8tz是一个 Kubernetes 准入控制器和一个将时区注入 Pod 的 CLI 工具。可以用作手动工具来自动转换 Deployment 和 Pod 可以作为准入控制器安装并使用注释来完全自动化创建 Pod 的过程。<br />k8tz 可以使用 hostPath的方式，或者将 emptyDir 注入 initContainer并用 TZif（时区信息格式) 文件填充卷。然后将 emptyDir挂载到 Pod 每个容器的 /etc/localtime和 /usr/share/zoneinfo。为了确保所需的时区有效，它向所有容器添加了 TZ环境变量。
<a name="i2KRR"></a>
## 安装
用 Helm 安装 k8tz准入控制器：
```bash
helm repo add k8tz https://k8tz.github.io/k8tz/
helm install k8tz k8tz/k8tz --set timezone=Asia/Shanghai
```
查看 Pod 状态、Mutatingwebhookconfigurations、Service 等资源是否正常：
```bash
# kubectl get mutatingwebhookconfigurations.admissionregistration.k8s.io  k8tz
NAME   WEBHOOKS   AGE
k8tz   1          31m

# kubectl get svc -n  k8tz
NAME   TYPE        CLUSTER-IP      EXTERNAL-IP   PORT(S)   AGE
k8tz   ClusterIP   10.233.212.11   <none>        443/TCP   31m

# kubectl get pod -n  k8tz
NAME                    READY   STATUS    RESTARTS   AGE
k8tz-59bb7f7cbd-5dzmq   1/1     Running   0          31m
```
<a name="kzK0Y"></a>
## 测试
现在可以创建 Pod，不需要任何额外配置：
```bash
# kubectl run -i -t ubuntu --image=ubuntu:21.04 --restart=OnFailure --rm=true --command date
Defaulted container "ubuntu" out of: ubuntu, k8tz (init)
Wed Jun 15 14:11:53 CST 2022
pod "ubuntu" deleted
```
此时的 Pod yaml 如下，环境变量 TZ 使用安装时指定的 Asia/Shanghai，以及注入了 initContainers、volumeMounts、volumes 等配置：
```yaml
apiVersion: v1
kind: Pod
metadata:
  labels:
    run: ubuntu
  name: ubuntu
  namespace: default
spec:
  containers:
  - command:
    - date
    env:
    - name: TZ
      value: Asia/Shanghai
    image: ubuntu:21.04
    imagePullPolicy: IfNotPresent
    name: ubuntu
    volumeMounts:
    - mountPath: /etc/localtime
      name: k8tz
      readOnly: true
      subPath: Asia/Shanghai
    - mountPath: /usr/share/zoneinfo
      name: k8tz
      readOnly: true
  initContainers:
  - args:
    - bootstrap
    image: quay.io/k8tz/k8tz:0.5.0
    imagePullPolicy: IfNotPresent
    name: k8tz
    volumeMounts:
    - mountPath: /mnt/zoneinfo
      name: k8tz
  volumes:
  - emptyDir: {}
    name: k8tz
```
还可以指定 annotations，例如 `k8tz.io/timezone=Europe/London`选择 pod 的时区：
```bash
# kubectl run -i -t ubuntu --image=ubuntu:21.04 --restart=OnFailure --rm=true --command date --annotations k8tz.io/timezone=Europe/London
Defaulted container "ubuntu" out of: ubuntu, k8tz (init)
Wed Jun 15 07:13:42 BST 2022
pod "ubuntu" deleted
```
或者使用注解 k8tz.io/inject禁用时区注入 Pod ：
```bash
# kubectl run -i -t ubuntu --image=ubuntu:21.04 --restart=OnFailure --rm=true --command date --annotations k8tz.io/inject=false
Wed Jun 15 06:14:47 UTC 2022
pod "ubuntu" deleted
```
如果想使用 hostPath而不是 initContainer方式注入时区配置，可以使用 k8tz.io/strategy注解：
```bash
# kubectl run -i -t ubuntu --image=ubuntu:21.04 --restart=OnFailure --rm=true --command date --annotations k8tz.io/strategy=hostPath
Wed Jun 15 14:15:26 CST 2022
pod "ubuntu" deleted
```
annotations 也可以在命名空间中指定，并影响在命名空间中创建的所有 pod。下面创建一个 test-k8tz namespace 用于测试：
```bash
# k create ns test-k8tz
namespace/test-k8tz created

# k annotate ns test-k8tz k8tz.io/strategy=hostPath
namespace/test-k8tz annotated

# k annotate ns test-k8tz k8tz.io/timezone=Europe/London
namespace/test-k8tz annotated
```
上面将策略设置为 hostPath 注入方式。因为安装 k8tz 时默认时区已经设置为 Asia/Shanghai，所以这里将 test-k8tz namespace 时区设置为 Europe/London，方便区分。<br />此时创建的 Pod 不需要加任何注解：
```bash
# kubectl run -n test-k8tz  -i -t ubuntu --image=ubuntu:21.04 --restart=OnFailure  --command date
Wed Jun 15 07:19:48 BST 2022
```
此时创建的 Pod yaml 如下，此时用的是 hostPath 注入方式：
```yaml
apiVersion: v1
kind: Pod
metadata:
  labels:
    run: ubuntu
  name: ubuntu
  namespace: test-k8tz
spec:
  containers:
  - command:
    - date
    env:
    - name: TZ
      value: Europe/London
    image: ubuntu:21.04
    imagePullPolicy: IfNotPresent
    name: ubuntu
    volumeMounts:
    - mountPath: /etc/localtime
      name: k8tz
      readOnly: true
      subPath: Europe/London
    - mountPath: /usr/share/zoneinfo
      name: k8tz
      readOnly: true
  volumes:
  - hostPath:
      path: /usr/share/zoneinfo
      type: ""
    name: k8tz
```
<a name="lb7DE"></a>
## 结论
Kubernetes 中的时区问题有多种解决方案，这些解决方案可以手动实现，但在此过程中存在一些挑战和限制。<br />使用 k8tz可以自动执行该过程，确保系统中所有组件的时区一致，并且所有组件都可以访问有关不同时区的信息。并且无需额外设置或更改现有资源即可工作，即使在节点上没有所需文件时也是如此。
