Kubernetes Pod<br />现在，已经非常清楚：Pod，而不是容器，才是 Kubernetes 项目中的最小编排单位。将这个设计落实到 API 对象上，容器（Container）就成了 Pod 属性里的一个普通的字段。那么，一个很自然的问题就是：到底哪些属性属于 Pod 对象，而又有哪些属性属于 Container 呢？<br />要彻底理解这个问题，就一定要牢记一个结论：Pod 扮演的是传统部署环境里“虚拟机”的角色。这样的设计，是为了使用户从传统环境（虚拟机环境）向 Kubernetes（容器环境）的迁移，更加平滑。<br />而如果能把 Pod 看成传统环境里的“机器”、把容器看作是运行在这个“机器”里的“用户程序”，那么很多关于 Pod 对象的设计就非常容易理解了。<br />比如，凡是调度、网络、存储，以及安全相关的属性，基本上是 Pod 级别的。<br />这些属性的共同特征是，它们描述的是“机器”这个整体，而不是里面运行的“程序”。比如，配置这个“机器”的网卡（即：Pod 的网络定义），配置这个“机器”的磁盘（即：Pod 的存储定义），配置这个“机器”的防火墙（即：Pod 的安全定义）。更不用说，这台“机器”运行在哪个服务器之上（即：Pod 的调度）。
<a name="rECRp"></a>
## Pod 中几个重要字段的含义和用法
<a name="GlOvz"></a>
### `NodeSelector`：是一个供用户将 Pod 与 Node 进行绑定的字段
用法如下所示：
```yaml
apiVersion: v1
kind: Pod
...
spec:
 nodeSelector:
   disktype: ssd
```
这样的一个配置，意味着这个 Pod 永远只能运行在携带了“disktype: ssd”标签（Label）的节点上；否则，它将调度失败。<br />`NodeName`：一旦 Pod 的这个字段被赋值，Kubernetes 项目就会被认为这个 Pod 已经经过了调度，调度的结果就是赋值的节点名字。所以，这个字段一般由调度器负责设置，但用户也可以设置它来“骗过”调度器，当然这个做法一般是在测试或者调试的时候才会用到。
<a name="VeP3a"></a>
### `HostAliases`：定义了 Pod 的 hosts 文件（比如 /etc/hosts）里的内容
用法如下：
```yaml
apiVersion: v1
kind: Pod
...
spec:
  hostAliases:
  - ip: "10.1.2.3"
    hostnames:
    - "foo.remote"
    - "bar.remote"
...
```
在这个 Pod 的 YAML 文件中，设置了一组 IP 和 hostname 的数据。这样，这个 Pod 启动后，/etc/hosts 文件的内容将如下所示：
```bash
cat /etc/hosts
# Kubernetes-managed hosts file.
127.0.0.1 localhost
...
10.244.135.10 hostaliases-pod
10.1.2.3 foo.remote
10.1.2.3 bar.remote
```
其中，最下面两行记录，就是通过 HostAliases 字段为 Pod 设置的。需要指出的是，在 Kubernetes 项目中，如果要设置 hosts 文件里的内容，一定要通过这种方法。否则，如果直接修改了 hosts 文件的话，在 Pod 被删除重建之后，kubelet 会自动覆盖掉被修改的内容。<br />除了上述跟“机器”相关的配置外，可能也会发现，凡是跟容器的 Linux Namespace 相关的属性，也一定是 Pod 级别的。这个原因也很容易理解：Pod 的设计，就是要让它里面的容器尽可能多地共享 Linux Namespace，仅保留必要的隔离和限制能力。这样，Pod 模拟出的效果，就跟虚拟机里程序间的关系非常类似了。<br />举个例子，在下面这个 Pod 的 YAML 文件中，定义了 `shareProcessNamespace=true`：
```yaml
apiVersion: v1
kind: Pod
metadata:
  name: nginx
spec:
  shareProcessNamespace: true
  containers:
  - name: nginx
    image: nginx
  - name: shell
    image: busybox
    stdin: true
    tty: true
```
这就意味着这个 Pod 里的容器要共享 PID Namespace。<br />而在这个 YAML 文件中，还定义了两个容器：一个是 nginx 容器，一个是开启了 tty 和 stdin 的 shell 容器。在 Pod 的 YAML 文件里声明开启它们俩，其实等同于设置了 `docker run` 里的 `-it`（`-i` 即 `stdin`，`-t` 即 `tty`）参数。可以直接认为 tty 就是 Linux 给用户提供的一个常驻小程序，用于接收用户的标准输入，返回操作系统的标准输出。当然，为了能够在 tty 中输入信息，还需要同时开启 stdin（标准输入流）。<br />于是，这个 Pod 被创建后，就可以使用 shell 容器的 tty 跟这个容器进行交互了。实践一下：
```bash
$ kubectl create -f nginx.yaml
```
接下来，使用 `kubectl attach` 命令，连接到 shell 容器的 tty 上：
```bash
$ kubectl attach -it nginx -c shell
```
这样，就可以在 shell 容器里执行 `ps` 指令，查看所有正在运行的进程：
```bash
$ kubectl attach -it nginx -c shell
/ # ps ax
PID   USER     TIME  COMMAND
    1 root      0:00 /pause
    8 root      0:00 nginx: master process nginx -g daemon off;
   14 101       0:00 nginx: worker process
   15 root      0:00 sh
   21 root      0:00 ps ax
```
可以看到，在这个容器里，不仅可以看到它本身的 `ps ax` 指令，还可以看到 nginx 容器的进程，以及 Infra 容器的 /pause 进程。这就意味着，整个 Pod 里的每个容器的进程，对于所有容器来说都是可见的：它们共享了同一个 PID Namespace。<br />类似地，凡是 Pod 中的容器要共享宿主机的 Namespace，也一定是 Pod 级别的定义，比如：
```yaml
apiVersion: v1
kind: Pod
metadata:
  name: nginx
spec:
  hostNetwork: true
  hostIPC: true
  hostPID: true
  containers:
  - name: nginx
    image: nginx
  - name: shell
    image: busybox
    stdin: true
    tty: true
```
在这个 Pod 中，定义了共享宿主机的 Network、IPC 和 PID Namespace。这就意味着，这个 Pod 里的所有容器，会直接使用宿主机的网络、直接与宿主机进行 IPC 通信、看到宿主机里正在运行的所有进程。
<a name="mr7L1"></a>
### Containers
当然，除了这些属性，Pod 里最重要的字段当属“Containers”了。其实，这两个字段都属于 Pod 对容器的定义，内容也完全相同，只是 Init Containers 的生命周期，会先于所有的 Containers，并且严格按照定义的顺序执行。<br />Kubernetes 项目中对 Container 的定义，和 Docker 相比并没有什么太大区别。 Image（镜像）、Command（启动命令）、workingDir（容器的工作目录）、Ports（容器要开发的端口），以及 volumeMounts（容器要挂载的 Volume）都是构成 Kubernetes 项目中 Container 的主要字段。不过在这里，还有这么几个属性需要额外关注。
<a name="AOI3k"></a>
#### 首先，是 ImagePullPolicy 字段。它定义了镜像拉取的策略。
而它之所以是一个 Container 级别的属性，是因为容器镜像本来就是 Container 定义中的一部分。<br />ImagePullPolicy 的值默认是 Always，即每次创建 Pod 都重新拉取一次镜像。另外，当容器的镜像是类似于 nginx 或者 nginx:latest 这样的名字时，ImagePullPolicy 也会被认为 Always。<br />而如果它的值被定义为 Never 或者 IfNotPresent，则意味着 Pod 永远不会主动拉取这个镜像，或者只在宿主机上不存在这个镜像时才拉取。
<a name="D6nTz"></a>
#### 其次，是 Lifecycle 字段。它定义的是 Container Lifecycle Hooks。
顾名思义，Container Lifecycle Hooks 的作用，是在容器状态发生变化时触发一系列“钩子”。来看这样一个例子：
```yaml
apiVersion: v1
kind: Pod
metadata:
  name: lifecycle-demo
spec:
  containers:
  - name: lifecycle-demo-container
    image: nginx
    lifecycle:
      postStart:
        exec:
          command: ["/bin/sh", "-c", "echo Hello from the postStart handler > /usr/share/message"]
      preStop:
        exec:
          command: ["/usr/sbin/nginx","-s","quit"]
```
这是一个来自 Kubernetes 官方文档的 Pod 的 YAML 文件。它其实非常简单，只是定义了一个 nginx 镜像的容器。不过，在这个 YAML 文件的容器（Containers）部分，可以看到这个容器分别设置了一个 postStart 和 preStop 参数。这是什么意思呢？<br />postStart 指的是，在容器启动后，立刻执行一个指定的操作。需要明确的是，postStart 定义的操作，虽然是在 Docker 容器 ENTRYPOINT 执行之后，但它并不严格保证顺序。也就是说，在 postStart 启动时，ENTRYPOINT 有可能还没有结束。<br />当然，如果 postStart 执行超时或者错误，Kubernetes 会在该 Pod 的 Events 中报出该容器启动失败的错误信息，导致 Pod 也处于失败的状态。<br />而类似地，preStop 发生的时机，则是容器被杀死之前（比如，收到了 SIGKILL 信号）。而需要明确的是，preStop 操作的执行，是同步的。所以，它会阻塞当前的容器杀死流程，直到这个 Hook 定义操作完成之后，才允许容器被杀死，这跟 postStart 不一样。<br />所以，在这个例子中，在容器成功启动之后，在 /usr/share/message 里写入了一句“欢迎信息”（即 postStart 定义的操作）。而在这个容器被删除之前，则先调用了 nginx 的退出指令（即 preStop 定义的操作），从而实现了容器的“优雅退出”。
<a name="lNBmF"></a>
### Projected Volume——“投射数据卷”
:::tips
备注：Projected Volume 是 Kubernetes v1.11 之后的新特性
:::
在 Kubernetes 中，有几种特殊的 Volume，它们存在的意义不是为了存放容器里的数据，也不是用来进行容器和宿主机之间的数据交换。这些特殊 Volume 的作用，是为容器提供预先定义好的数据。所以，从容器的角度来看，这些 Volume 里的信息就是仿佛是被 Kubernetes“投射”（Project）进入容器当中的。这正是 Projected Volume 的含义。<br />到目前为止，Kubernetes 支持的 Projected Volume 一共有四种：

1. Secret；
2. ConfigMap；
3. Downward API；
4. ServiceAccountToken。
<a name="SigXR"></a>
#### Secret
Secret的作用是把 Pod 想要访问的加密数据，存放到 Etcd 中。然后，就可以通过在 Pod 的容器里挂载 Volume 的方式，访问到这些 Secret 里保存的信息了。<br />Secret 最典型的使用场景，莫过于存放数据库的 Credential 信息，比如下面这个例子：
```yaml
apiVersion: v1
kind: Pod
metadata:
  name: test-projected-volume 
spec:
  containers:
  - name: test-secret-volume
    image: busybox
    args:
    - sleep
    - "86400"
    volumeMounts:
    - name: mysql-cred
      mountPath: "/projected-volume"
      readOnly: true
  volumes:
  - name: mysql-cred
    projected:
      sources:
      - secret:
          name: user
      - secret:
          name: pass
```
在这个 Pod 中，定义了一个简单的容器。它声明挂载的 Volume，并不是常见的 emptyDir 或者 hostPath 类型，而是 projected 类型。而这个 Volume 的数据来源（sources），则是名为 user 和 pass 的 Secret 对象，分别对应的是数据库的用户名和密码。<br />这里用到的数据库的用户名、密码，正是以 Secret 对象的方式交给 Kubernetes 保存的。完成这个操作的指令，如下所示：
```bash
$ cat ./username.txt
admin
$ cat ./password.txt
c1oudc0w!

$ kubectl create secret generic user --from-file=./username.txt
$ kubectl create secret generic pass --from-file=./password.txt
```
其中，username.txt 和 password.txt 文件里，存放的就是用户名和密码；而 user 和 pass，则是为 Secret 对象指定的名字。而想要查看这些 Secret 对象的话，只要执行一条 `kubectl get` 命令就可以了：
```bash
$ kubectl get secrets
NAME           TYPE                                DATA      AGE
user          Opaque                                1         51s
pass          Opaque                                1         51s
```
当然，除了使用 `kubectl create secret` 指令外，也可以直接通过编写 YAML 文件的方式来创建这个 Secret 对象，比如：
```yaml
apiVersion: v1
kind: Secret
metadata:
  name: mysecret
type: Opaque
data:
  user: YWRtaW4=
  pass: MWYyZDFlMmU2N2Rm
```
可以看到，通过编写 YAML 文件创建出来的 Secret 对象只有一个。但它的 data 字段，却以 Key-Value 的格式保存了两份 Secret 数据。其中，“user”就是第一份数据的 Key，“pass”是第二份数据的 Key。<br />需要注意的是，Secret 对象要求这些数据必须是经过 Base64 转码的，以免出现明文密码的安全隐患。这个转码操作也很简单，比如：
```bash
$ echo -n 'admin' | base64
YWRtaW4=
$ echo -n '1f2d1e2e67df' | base64
MWYyZDFlMmU2N2Rm
```
这里需要注意的是，像这样创建的 Secret 对象，它里面的内容仅仅是经过了转码，而并没有被加密。在真正的生产环境中，需要在 Kubernetes 中开启 Secret 的加密插件，增强数据的安全性。<br />接下来，尝试一下创建这个 Pod：
```bash
$ kubectl create -f test-projected-volume.yaml
```
当 Pod 变成 Running 状态之后，再验证一下这些 Secret 对象是不是已经在容器里了：
```bash
$ kubectl exec -it test-projected-volume -- /bin/sh
$ ls /projected-volume/
user
pass
$ cat /projected-volume/user
root
$ cat /projected-volume/pass
1f2d1e2e67df
```
从返回结果中，可以看到，保存在 Etcd 里的用户名和密码信息，已经以文件的形式出现在了容器的 Volume 目录里。而这个文件的名字，就是 `kubectl create secret` 指定的 Key，或者说是 Secret 对象的 data 字段指定的 Key。<br />更重要的是，像这样通过挂载方式进入到容器里的 Secret，一旦其对应的 Etcd 里的数据被更新，这些 Volume 里的文件内容，同样也会被更新。其实，**这是 kubelet 组件在定时维护这些 Volume。**<br />需要注意的是，这个更新可能会有一定的延时。所以**在编写应用程序时，在发起数据库连接的代码处写好重试和超时的逻辑，绝对是个好习惯。**
<a name="g2ZrM"></a>
#### ConfigMap
与 Secret 类似的是 ConfigMap，它与 Secret 的区别在于，ConfigMap 保存的是不需要加密的、应用所需的配置信息。而 ConfigMap 的用法几乎与 Secret 完全相同：可以使用 `kubectl create configmap` 从文件或者目录创建 ConfigMap，也可以直接编写 ConfigMap 对象的 YAML 文件。<br />比如，一个 Java 应用所需的配置文件（.properties 文件），就可以通过下面这样的方式保存在 ConfigMap 里：
```bash
# .properties文件的内容
$ cat example/ui.properties
color.good=purple
color.bad=yellow
allow.textmode=true
how.nice.to.look=fairlyNice

# 从.properties文件创建ConfigMap
$ kubectl create configmap ui-config --from-file=example/ui.properties

# 查看这个ConfigMap里保存的信息(data)
$ kubectl get configmaps ui-config -o yaml
apiVersion: v1
data:
  ui.properties: |
    color.good=purple
    color.bad=yellow
    allow.textmode=true
    how.nice.to.look=fairlyNice
kind: ConfigMap
metadata:
  name: ui-config
  ...
```
备注：`kubectl get -o yaml` 这样的参数，会将指定的 Pod API 对象以 YAML 的方式展示出来。
<a name="gloHx"></a>
#### Downward API
Downward API的作用是让 Pod 里的容器能够直接获取到这个 Pod API 对象本身的信息。<br />举个例子：
```yaml
apiVersion: v1
kind: Pod
metadata:
  name: test-downwardapi-volume
  labels:
    zone: us-est-coast
    cluster: test-cluster1
    rack: rack-22
spec:
  containers:
    - name: client-container
      image: k8s.gcr.io/busybox
      command: ["sh", "-c"]
      args:
      - while true; do
          if [[ -e /etc/podinfo/labels ]]; then
            echo -en '\n\n'; cat /etc/podinfo/labels; fi;
          sleep 5;
        done;
      volumeMounts:
        - name: podinfo
          mountPath: /etc/podinfo
          readOnly: false
  volumes:
    - name: podinfo
      projected:
        sources:
        - downwardAPI:
            items:
              - path: "labels"
                fieldRef:
                  fieldPath: metadata.labels
```
在这个 Pod 的 YAML 文件中，定义了一个简单的容器，声明了一个 projected 类型的 Volume。只不过这次 Volume 的数据来源，变成了 Downward API。而这个 Downward API Volume，则声明了要暴露 Pod 的 metadata.labels 信息给容器。<br />通过这样的声明方式，当前 Pod 的 Labels 字段的值，就会被 Kubernetes 自动挂载成为容器里的 /etc/podinfo/labels 文件。<br />而这个容器的启动命令，则是不断打印出 /etc/podinfo/labels 里的内容。所以，当创建了这个 Pod 之后，就可以通过 `kubectl logs` 指令，查看到这些 Labels 字段被打印出来，如下所示：
```bash
$ kubectl create -f dapi-volume.yaml
$ kubectl logs test-downwardapi-volume
cluster="test-cluster1"
rack="rack-22"
zone="us-est-coast"
```
目前，Downward API 支持的字段已经非常丰富了，比如：
```bash
1. 使用fieldRef可以声明使用:
spec.nodeName - 宿主机名字
status.hostIP - 宿主机IP
metadata.name - Pod的名字
metadata.namespace - Pod的Namespace
status.podIP - Pod的IP
spec.serviceAccountName - Pod的Service Account的名字
metadata.uid - Pod的UID
metadata.labels['<KEY>'] - 指定<KEY>的Label值
metadata.annotations['<KEY>'] - 指定<KEY>的Annotation值
metadata.labels - Pod的所有Label
metadata.annotations - Pod的所有Annotation

2. 使用resourceFieldRef可以声明使用:
容器的CPU limit
容器的CPU request
容器的memory limit
容器的memory request
```
上面这个列表的内容，随着 Kubernetes 项目的发展肯定还会不断增加。所以这里列出来的信息仅供参考，在使用 Downward API 时，还是要记得去查阅一下官方文档。<br />不过，需要注意的是，Downward API 能够获取到的信息，**一定是 Pod 里的容器进程启动之前就能够确定下来的信息**。而如果想要获取 Pod 容器运行后才会出现的信息，比如，容器进程的 PID，那就肯定不能使用 Downward API 了，而应该考虑在 Pod 里定义一个 sidecar 容器。<br />其实，Secret、ConfigMap，以及 Downward API 这三种 Projected Volume 定义的信息，大多还可以通过环境变量的方式出现在容器里。但是，通过环境变量获取这些信息的方式，不具备自动更新的能力。所以，一般情况下，建议使用 Volume 文件的方式获取这些信息。
<a name="ZAHwl"></a>
#### Service Account
现在有了一个 Pod，能不能在这个 Pod 里安装一个 Kubernetes 的 Client，这样就可以从容器里直接访问并且操作这个 Kubernetes 的 API 了呢？<br />这当然是可以的。不过，首先要解决 API Server 的授权问题。<br />Service Account 对象的作用，就是 Kubernetes 系统内置的一种“服务账户”，它是 Kubernetes 进行权限分配的对象。比如，Service Account A，可以只被允许对 Kubernetes API 进行 GET 操作，而 Service Account B，则可以有 Kubernetes API 的所有操作权限。<br />像这样的 Service Account 的授权信息和文件，实际上保存在它所绑定的一个特殊的 Secret 对象里的。这个特殊的 Secret 对象，就叫作 ServiceAccountToken。任何运行在 Kubernetes 集群上的应用，都必须使用这个 ServiceAccountToken 里保存的授权信息，也就是 Token，才可以合法地访问 API Server。<br />所以说，Kubernetes 项目的 Projected Volume 其实只有三种，因为第四种 ServiceAccountToken，只是一种特殊的 Secret 而已。<br />另外，为了方便使用，Kubernetes 已经提供了一个默认“服务账户”（default Service Account）。并且，任何一个运行在 Kubernetes 里的 Pod，都可以直接使用这个默认的 Service Account，而无需显示地声明挂载它。这是如何做到的呢？<br />当然还是靠 Projected Volume 机制。<br />如果查看一下任意一个运行在 Kubernetes 集群里的 Pod，就会发现，每一个 Pod，都已经自动声明一个类型是 Secret、名为 default-token-xxxx 的 Volume，然后 自动挂载在每个容器的一个固定目录上。比如：
```bash
$ kubectl describe pod nginx-deployment-5c678cfb6d-lg9lw
Containers:
...
  Mounts:
    /var/run/secrets/kubernetes.io/serviceaccount from default-token-s8rbq (ro)
Volumes:
  default-token-s8rbq:
  Type:       Secret (a volume populated by a Secret)
  SecretName:  default-token-s8rbq
  Optional:    false
```
这个 Secret 类型的 Volume，正是默认 Service Account 对应的 ServiceAccountToken。所以说，Kubernetes 其实在每个 Pod 创建的时候，自动在它的 spec.volumes 部分添加上了默认 ServiceAccountToken 的定义，然后自动给每个容器加上了对应的 volumeMounts 字段。这个过程对于用户来说是完全透明的。<br />这样，一旦 Pod 创建完成，容器里的应用就可以直接从这个默认 ServiceAccountToken 的挂载目录里访问到授权信息和文件。这个容器内的路径在 Kubernetes 里是固定的，即：/var/run/secrets/kubernetes.io/serviceaccount ，而这个 Secret 类型的 Volume 里面的内容如下所示：
```bash
$ ls /var/run/secrets/kubernetes.io/serviceaccount 
ca.crt namespace  token
```
所以，应用程序只要直接加载这些授权文件，就可以访问并操作 Kubernetes API 了。而且，如果使用的是 Kubernetes 官方的 Client 包（k8s.io/client-go）的话，它还可以自动加载这个目录下的文件，不需要做任何配置或者编码操作。
:::success
这种把 Kubernetes 客户端以容器的方式运行在集群里，然后使用 default Service Account 自动授权的方式，被称作“InClusterConfig”，也是最推荐的进行 Kubernetes API 编程的授权方式。
:::
当然，考虑到自动挂载默认 ServiceAccountToken 的潜在风险，Kubernetes 允许设置默认不为 Pod 里的容器自动挂载这个 Volume。<br />除了这个默认的 Service Account 外，很多时候还需要创建一些自己定义的 Service Account，来对应不同的权限设置。这样， Pod 里的容器就可以通过挂载这些 Service Account 对应的 ServiceAccountToken，来使用这些自定义的授权信息。
<a name="Qq0AG"></a>
### 容器健康检查和恢复机制
在 Kubernetes 中，可以为 Pod 里的容器定义一个健康检查“探针”（Probe）。这样，kubelet 就会根据这个 Probe 的返回值决定这个容器的状态，而不是直接以容器镜像是否运行（来自 Docker 返回的信息）作为依据。这种机制，是生产环境中保证应用健康存活的重要手段。<br />一起来看一个 Kubernetes 文档中的例子。
```yaml
apiVersion: v1
kind: Pod
metadata:
  labels:
    test: liveness
  name: test-liveness-exec
spec:
  containers:
  - name: liveness
    image: busybox
    args:
    - /bin/sh
    - -c
    - touch /tmp/healthy; sleep 30; rm -rf /tmp/healthy; sleep 600
    livenessProbe:
      exec:
        command:
        - cat
        - /tmp/healthy
      initialDelaySeconds: 5
      periodSeconds: 5
```
在这个 Pod 中，定义了一个有趣的容器。它在启动之后做的第一件事，就是在 /tmp 目录下创建了一个 healthy 文件，以此作为自己已经正常运行的标志。而 30 s 过后，它会把这个文件删除掉。<br />与此同时，定义了一个这样的 livenessProbe（健康检查）。它的类型是 exec，这意味着，它会在容器启动后，在容器里面执行一条指定的命令，比如：“cat /tmp/healthy”。这时，如果这个文件存在，这条命令的返回值就是 0，Pod 就会认为这个容器不仅已经启动，而且是健康的。这个健康检查，在容器启动 5 s 后开始执行（initialDelaySeconds: 5），每 5 s 执行一次（periodSeconds: 5）。
<a name="frsBS"></a>
#### 具体实践过程
<a name="RaMLK"></a>
##### 首先，创建这个 Pod：
```bash
$ kubectl create -f test-liveness-exec.yaml
```
<a name="rpopj"></a>
##### 然后，查看这个 Pod 的状态：
```bash
$ kubectl get pod
NAME                READY     STATUS    RESTARTS   AGE
test-liveness-exec   1/1       Running   0          10s
```
可以看到，由于已经通过了健康检查，这个 Pod 就进入了 Running 状态。<br />而 30 s 之后，再查看一下 Pod 的 Events：
```bash
$ kubectl describe pod test-liveness-exec
```
可以发现，这个 Pod 在 Events 报告了一个异常：
```bash
FirstSeen LastSeen    Count   From            SubobjectPath           Type        Reason      Message
--------- --------    -----   ----            -------------           --------    ------      -------
2s        2s      1   {kubelet worker0}   spec.containers{liveness}   Warning     Unhealthy   Liveness probe failed: cat: can't open '/tmp/healthy': No such file or directory
```
显然，这个健康检查探查到 /tmp/healthy 已经不存在了，所以它报告容器是不健康的。那么接下来会发生什么呢？<br />不妨再次查看一下这个 Pod 的状态：
```bash
$ kubectl get pod test-liveness-exec
NAME           READY     STATUS    RESTARTS   AGE
liveness-exec   1/1       Running   1          1m
```
这时可以发现，Pod 并没有进入 Failed 状态，而是保持了 Running 状态。这是为什么呢？<br />其实，如果注意到 RESTARTS 字段从 0 到 1 的变化，就明白原因了：这个异常的容器已经被 Kubernetes 重启了。在这个过程中，Pod 保持 Running 状态不变。<br />需要注意的是：Kubernetes 中并没有 Docker 的 Stop 语义。所以虽然是 Restart（重启），但实际却是重新创建了容器。<br />这个功能就是 Kubernetes 里的 **Pod 恢复机制**，也叫 restartPolicy。它是 Pod 的 Spec 部分的一个标准字段（pod.spec.restartPolicy），默认值是 Always，即：任何时候这个容器发生了异常，它一定会被重新创建。<br />但一定要强调的是，Pod 的恢复过程，永远都是发生在当前节点上，而不会跑到别的节点上去。事实上，一旦一个 Pod 与一个节点（Node）绑定，除非这个绑定发生了变化（`pod.spec.node` 字段被修改），否则它永远都不会离开这个节点。这也就意味着，如果这个宿主机宕机了，这个 Pod 也不会主动迁移到其他节点上去。<br />而如果想让 Pod 出现在其他的可用节点上，就必须使用 Deployment 这样的“控制器”来管理 Pod，哪怕只需要一个 Pod 副本。即一个单 Pod 的 Deployment 与一个 Pod 最主要的区别。<br />而作为用户，还可以通过设置 restartPolicy，改变 Pod 的恢复策略。除了 Always，它还有 OnFailure 和 Never 两种情况：

- Always：在任何情况下，只要容器不在运行状态，就自动重启容器；
- OnFailure：只在容器 异常时才自动重启容器；
- Never：从来不重启容器。

在实际使用时，需要根据应用运行的特性，合理设置这三种恢复策略。<br />比如，一个 Pod，它只计算 1+1=2，计算完成输出结果后退出，变成 Succeeded 状态。这时，如果再用 restartPolicy=Always 强制重启这个 Pod 的容器，就没有任何意义了。<br />而如果要关心这个容器退出后的上下文环境，比如容器退出后的日志、文件和目录，就需要将 restartPolicy 设置为 Never。因为一旦容器被自动重新创建，这些内容就有可能丢失掉了（被垃圾回收了）。<br />值得一提的是，Kubernetes 的官方文档，把 restartPolicy 和 Pod 里容器的状态，以及 Pod 状态的对应关系，总结了非常复杂的一大堆情况。实际上，根本不需要死记硬背这些对应关系，只要记住如下两个基本的设计原理即可：

1. **只要 Pod 的 restartPolicy 指定的策略允许重启异常的容器（比如：Always），那么这个 Pod 就会保持 Running 状态，并进行容器重启**。否则，Pod 就会进入 Failed 状态 。
2. **对于包含多个容器的 Pod，只有它里面所有的容器都进入异常状态后，Pod 才会进入 Failed 状态**。在此之前，Pod 都是 Running 状态。此时，Pod 的 READY 字段会显示正常容器的个数，比如：
```bash
$ kubectl get pod test-liveness-exec
NAME           READY     STATUS    RESTARTS   AGE
liveness-exec   0/1       Running   1          1m
```
所以，假如一个 Pod 里只有一个容器，然后这个容器异常退出了。那么，只有当 `restartPolicy=Never` 时，这个 Pod 才会进入 Failed 状态。而其他情况下，由于 Kubernetes 都可以重启这个容器，所以 Pod 的状态保持 Running 不变。<br />而如果这个 Pod 有多个容器，仅有一个容器异常退出，它就始终保持 Running 状态，哪怕即使 `restartPolicy=Never`。只有当所有容器也异常退出之后，这个 Pod 才会进入 Failed 状态。
<a name="JJ5Ul"></a>
### livenessProbe
除了在容器中执行命令外，livenessProbe 也可以定义为发起 HTTP 或者 TCP 请求的方式，定义格式如下：
```yaml
...
livenessProbe:
     httpGet:
       path: /healthz
       port: 8080
       httpHeaders:
       - name: X-Custom-Header
         value: Awesome
       initialDelaySeconds: 3
       periodSeconds: 3
```
```yaml
...
livenessProbe:
  tcpSocket:
  	port: 8080
  initialDelaySeconds: 15
  periodSeconds: 20
```
所以 Pod 其实可以暴露一个健康检查 URL（比如 /healthz），或者直接让健康检查去检测应用的监听端口。这两种配置方法，在 Web 服务类的应用中非常常用。
<a name="VoOCS"></a>
### readinessProbe
在 Kubernetes 的 Pod 中，还有一个叫 readinessProbe 的字段。虽然它的用法与 livenessProbe 类似，但作用却大不一样。readinessProbe 检查结果的成功与否，决定的这个 Pod 是不是能被通过 Service 的方式访问到，而并不影响 Pod 的生命周期。
<a name="sOLH0"></a>
## Kubernetes  Pod 字段预配置填充
Pod 的字段这么多，又不可能全记住，Kubernetes 能不能自动给 Pod 填充某些字段呢？<br />这个需求实际上非常实用。比如，开发人员只需要提交一个基本的、非常简单的 Pod YAML，Kubernetes 就可以自动给对应的 Pod 对象加上其他必要的信息，比如 labels，annotations，volumes 等等。而这些信息，可以是运维人员事先定义好的。<br />这么一来，开发人员编写 Pod YAML 的门槛，就被大大降低了。<br />所以，这个叫作 PodPreset（Pod 预设置）的功能 已经出现在了 v1.11 版本的 Kubernetes 中。<br />举个例子，现在开发人员编写了如下一个 pod.yaml 文件：
```yaml
apiVersion: v1
kind: Pod
metadata:
  name: website
  labels:
    app: website
    role: frontend
spec:
  containers:
    - name: website
      image: nginx
      ports:
        - containerPort: 80
```
作为 Kubernetes 的初学者，肯定眼前一亮：这不就是最擅长编写的、最简单的 Pod 嘛。这个 YAML 文件里的字段，想必现在闭着眼睛也能写出来。<br />可是，如果运维人员看到了这个 Pod，他一定会连连摇头：这种 Pod 在生产环境里根本不能用！<br />所以，这个时候，运维人员就可以定义一个 PodPreset 对象。在这个对象中，凡是他想在开发人员编写的 Pod 里追加的字段，都可以预先定义好。比如这个 preset.yaml：
```yaml
apiVersion: settings.k8s.io/v1alpha1
kind: PodPreset
metadata:
  name: allow-database
spec:
  selector:
    matchLabels:
      role: frontend
  env:
    - name: DB_PORT
      value: "6379"
  volumeMounts:
    - mountPath: /cache
      name: cache-volume
  volumes:
    - name: cache-volume
      emptyDir: {}
```
在这个 PodPreset 的定义中，首先是一个 selector。这就意味着后面这些追加的定义，只会作用于 selector 所定义的、带有“role: frontend”标签的 Pod 对象，这就可以防止“误伤”。<br />然后，定义了一组 Pod 的 Spec 里的标准字段，以及对应的值。比如，env 里定义了 DB_PORT 这个环境变量，volumeMounts 定义了容器 Volume 的挂载目录，volumes 定义了一个 emptyDir 的 Volume。<br />接下来，假定运维人员先创建了这个 PodPreset，然后开发人员才创建 Pod：
```bash
$ kubectl create -f preset.yaml
$ kubectl create -f pod.yaml
```
这时，Pod 运行起来之后，查看一下这个 Pod 的 API 对象：
```bash
$ kubectl get pod website -o yaml
apiVersion: v1
kind: Pod
metadata:
  name: website
  labels:
    app: website
    role: frontend
  annotations:
    podpreset.admission.kubernetes.io/podpreset-allow-database: "resource version"
spec:
  containers:
    - name: website
      image: nginx
      volumeMounts:
        - mountPath: /cache
          name: cache-volume
      ports:
        - containerPort: 80
      env:
        - name: DB_PORT
          value: "6379"
  volumes:
    - name: cache-volume
      emptyDir: {}
```
这个时候，就可以清楚地看到，这个 Pod 里多了新添加的 labels、env、volumes 和 volumeMount 的定义，它们的配置跟 PodPreset 的内容一样。此外，这个 Pod 还被自动加上了一个 annotation 表示这个 Pod 对象被 PodPreset 改动过。<br />需要说明的是，PodPreset 里定义的内容，只会在 Pod API 对象被创建之前追加在这个对象本身上，而不会影响任何 Pod 的控制器的定义。<br />比如，现在提交的是一个 nginx-deployment，那么这个 Deployment 对象本身是永远不会被 PodPreset 改变的，被修改的只是这个 Deployment 创建出来的所有 Pod。这一点请务必区分清楚。<br />这里有一个问题：如果定义了同时作用于一个 Pod 对象的多个 PodPreset，会发生什么呢？<br />实际上，Kubernetes 项目会合并（Merge）这两个 PodPreset 要做的修改。而如果它们要做的修改有冲突的话，这些冲突字段就不会被修改。
<a name="gVVZS"></a>
## 一个的 Pod 对象在 Kubernetes 中的生命周期
Pod 生命周期的变化，主要体现在 Pod API 对象的 Status 部分，这是它除了 Metadata 和 Spec 之外的第三个重要字段。其中，pod.status.phase，就是 Pod 的当前状态，它有如下几种可能的情况：

1. Pending。这个状态意味着，Pod 的 YAML 文件已经提交给了 Kubernetes，API 对象已经被创建并保存在 Etcd 当中。但是，这个 Pod 里有些容器因为某种原因而不能被顺利创建。比如，调度不成功。
2. Running。这个状态下，Pod 已经调度成功，跟一个具体的节点绑定。它包含的容器都已经创建成功，并且至少有一个正在运行中。
3. Succeeded。这个状态意味着，Pod 里的所有容器都正常运行完毕，并且已经退出了。这种情况在运行一次性任务时最为常见。
4. Failed。这个状态下，Pod 里至少有一个容器以不正常的状态（非 0 的返回码）退出。这个状态的出现，意味着得想办法 Debug 这个容器的应用，比如查看 Pod 的 Events 和日志。
5. Unknown。这是一个异常状态，意味着 Pod 的状态不能持续地被 kubelet 汇报给 kube-apiserver，这很有可能是主从节点（Master 和 Kubelet）间的通信出现了问题。

更进一步地，Pod 对象的 Status 字段，还可以再细分出一组 Conditions。这些细分状态的值包括：PodScheduled、Ready、Initialized，以及 Unschedulable。它们主要用于描述造成当前 Status 的具体原因是什么。<br />比如，Pod 当前的 Status 是 Pending，对应的 Condition 是 Unschedulable，这就意味着它的调度出现了问题。<br />而其中，Ready 这个细分状态非常值得关注：它意味着 Pod 不仅已经正常启动（Running 状态），而且已经可以对外提供服务了。这两者之间（Running 和 Ready）是有区别的，不妨仔细思考一下。Pod 的这些状态信息，是判断应用运行情况的重要标准，尤其是 Pod 进入了非“Running”状态后，一定要能迅速做出反应，根据它所代表的异常情况开始跟踪和定位，而不是去手忙脚乱地查阅文档。
