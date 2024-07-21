Kubernetes<br />在 Kubernetes 中，Pod 是容器组的概念，一个 Pod 可以包含多个容器。这些容器共享网络、存储等资源，为应用程序提供了一个更加灵活的运行环境。<br />容器的本质是一个特殊的进程，特殊在为其创建了 NameSpace 隔离运行环境，并用 Cgroups 控制资源开销，还借助了一些Linux 网络虚拟化技术解决了网络通信的问题。<br />Pod 所做的则是让多个容器加入同一个 NameSpace 以实现资源共享。<br />接下来，可以使用 Docker 来还原 Pod 的实现原理。
<a name="J1N4E"></a>
### 容器共享 NameSpace
现在要部署一个 Pod ，其中包含 nginx 和 busybox 两个容器，前者作为主应用提供 Web 服务，后者作为 Sidecar 调试容器。<br />首先启动 nginx 容器：
```bash
docker run -d --name nginx --ipc="shareable" -v $PWD/log:/var/log/nginx -v $PWD/html:/usr/share/nginx/html nginx
```
默认情况下，Docker 的 IPC Namespace 是私有的，可以使用 `--ipc="shareable"` 来指定允许共享。`-v` 参数的作用就不多讲了。<br />接下来启动 busybox 容器，并加入到 nginx 容器的 NET、IPC、PID NameSpace 中，同时，共享 nginx 容器的 Volume ，以便可以访问 nginx 的日志文件：
```bash
docker run -d --name busybox --net=container:nginx --ipc=container:nginx --pid=container:nginx -v $PWD/log:/var/log/nginx yauritux/busybox-curl /bin/sh -c 'while true; do sleep 1h; done;'
```
两个容器都启动后，就可以在 busybox 容器中直接调试 nginx 容器的资源了：
```bash
[root@vm ~]# echo "hello pod" > $PWD/html/index.html
[root@vm ~]# docker exec -it busybox ps
PID   USER     TIME  COMMAND
    1 root      0:00 nginx: master process nginx -g daemon off;
   29 101       0:00 nginx: worker process
   30 101       0:00 nginx: worker process
   31 root      0:00 /bin/sh -c while true; do sleep 1h; done;
   37 root      0:00 sleep 1h
   38 root      0:00 ps
[root@vm ~]# docker exec -it busybox curl localhost
hello pod
[root@vm ~]# docker exec -it busybox tail /var/log/nginx/access.log
127.0.0.1 - - [30/Mar/2023:08:07:58 +0000] "GET / HTTP/1.1" 200 10 "-" "curl/7.81.0" "-"
[root@vm ~]#
```
在 busybox 容器中，不仅可以看到 nginx 容器的进程，还可以直接访问 nginx 服务和共享的日志文件目录。<br />但是，由于 Namespace 是由 nginx 容器创建的，如果 nginx 意外崩溃，那么所有 Namespace 都会一同被删除，busybox 容器也会被终止：
```
[root@vm ~]# docker stop nginx
nginx
[root@vm ~]# docker exec -it busybox ps
Error response from daemon: Container fca6ea0fe9f177f62d4b2d5d7db5bf64766d605f0414a8995c8d93159efeed4a is not running
[root@vm ~]#
```
显然，**让业务容器充当共享基础容器是不可取的**，必须保证每个容器都是对等的关系，而不是父子关系。Kubernetes 也考虑到了这一点。
<a name="l9Jvz"></a>
### Pause 容器
Pause 容器，又叫 Infra 容器。为了**解决共享基础容器的安全问题**， Kubernetes 会在每个 Pod 里，额外起一个 Infra 容器来共享整个 Pod 的 Namespace 。<br />Pause 容器会在 Pod 创建时首先启动，并创建 Namespace 、配置网络 IP 地址及路由等相关信息。可以说，Pause 容器的生命周期就相当于是整个 Pod 的生命周期。<br />等 Pause 容器启动完成后，其它容器才接着启动，并与 Pause 容器共享 Namespace。这样，每个容器就都可以访问 Pod 中其他容器的资源了。<br />具体的创建过程可以查看 kubelet 的源码：[**pkg/kubelet/kuberuntime/kuberuntime_manager.go#L678**](https://github.com/kubernetes/kubernetes/blob/v1.26.1/pkg/kubelet/kuberuntime/kuberuntime_manager.go#L678)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680243995195-6265ae67-1cad-4d2c-aef1-e3d85632614a.png#averageHue=%23dfd9b1&clientId=u7f095ad6-a13d-4&from=paste&id=ub4a6202b&originHeight=458&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud2759275-eb05-4ff9-9736-f74359891ba&title=)<br />其中第 4 步的创建 Sandbox 沙盒容器，实际就是创建 Pause 容器。之后才继续后面的 init 容器、正常容器的创建。<br />作用如此重要的 Pause 容器也决定了它的特点：<br />1、镜像非常小：[**gcr.io/google_containers/pause-amd64**](https://console.cloud.google.com/gcr/images/google-containers/GLOBAL/pause-amd64)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680243995208-91952a04-1172-4edb-b0c8-029782d4604b.png#averageHue=%23f6f5f5&clientId=u7f095ad6-a13d-4&from=paste&id=u90cb566e&originHeight=524&originWidth=789&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uab4fb0af-3650-4947-8784-8934a750211&title=)<br />2、性能开销几乎可以忽略：[**pause.c**](https://github.com/kubernetes/kubernetes/blob/master/build/pause/linux/pause.c)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680243995352-dfcce332-2e4c-42a7-a407-b69278da911e.png#averageHue=%23918476&clientId=u7f095ad6-a13d-4&from=paste&id=u105d4e62&originHeight=527&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u62795bfa-808c-4439-a4aa-55d3b55785e&title=)<br />了解了 Pause 容器后，重新开始 Pod 原理试验。<br />从新环境开始，这次首先启动的应该是 Pause 容器了：
```bash
docker run -d --name pause --ipc="shareable" gcr.io/google_containers/pause-amd64:3.2
```
然后再启动 nginx 容器，加入到 Pause 容器的 Namespace ：
```bash
docker run -d --name nginx --net=container:pause --ipc=container:pause --pid=container:pause -v $PWD/log:/var/log/nginx -v $PWD/html:/usr/share/nginx/html nginx
```
同理 busybox 容器：
```bash
docker run -d --name busybox --net=container:pause --ipc=container:pause --pid=container:pause -v $PWD/log:/var/log/nginx yauritux/busybox-curl /bin/sh -c 'while true; do sleep 1h; done;'
```
现在在 busybox 容器中将可以同时看到 pause 和 nginx 容器的进程：
```bash
[root@vm ~]# echo "hello pod" > $PWD/html/index.html
[root@vm ~]# docker exec -it busybox ps
PID   USER     TIME  COMMAND
    1 root      0:00 /pause
    8 root      0:00 nginx: master process nginx -g daemon off;
   36 101       0:00 nginx: worker process
   37 101       0:00 nginx: worker process
   38 root      0:00 /bin/sh -c while true; do sleep 1h; done;
   45 root      0:00 sleep 1h
   46 root      0:00 ps
[root@vm ~]# docker exec -it busybox curl localhost
hello pod
[root@vm ~]# docker exec -it busybox tail /var/log/nginx/access.log
127.0.0.1 - - [30/Mar/2023:08:49:49 +0000] "GET / HTTP/1.1" 200 10 "-" "curl/7.81.0" "-"
[root@vm ~]#
```
而且即使 nginx 意外崩溃了，也不会影响到 busybox 容器：
```bash
[root@vm ~]# docker stop nginx
nginx
[root@vm ~]# docker exec -it busybox ps
PID   USER     TIME  COMMAND
    1 root      0:00 /pause
   38 root      0:00 /bin/sh -c while true; do sleep 1h; done;
   45 root      0:00 sleep 1h
   66 root      0:00 ps
[root@vm ~]#
```
<a name="QvPuK"></a>
### 回到 Kubernetes
相信你现在已经理解，在 Kubernetes 集群中执行 `kubectl apply -f nginx-busybox-pod.yaml` 命令后所发生的事情：
```yaml
apiVersion: v1
kind: Pod
metadata:
  name: nginx-busybox-pod
spec:
  shareProcessNamespace: true
  containers:
  - name: nginx
    image: nginx
    volumeMounts:
    - name: nginx-log
      mountPath: /var/log/nginx
    - name: nginx-html
      mountPath: /usr/share/nginx/html
    ports:
    - containerPort: 80
  - name: busybox
    image: yauritux/busybox-curl
    command: ["/bin/sh", "-c", "while true; do sleep 1h; done;"]
    volumeMounts:
    - name: nginx-log
      mountPath: /var/log/nginx
  volumes:
  - name: nginx-log
    emptyDir: {}
  - name: nginx-html
    emptyDir: {}
```
![（改：应为 -c busybox ps）第一个进程就是 pause](https://cdn.nlark.com/yuque/0/2023/png/396745/1680243995220-1404267b-ec91-41fe-a361-e323b0e87008.png#averageHue=%233c4144&clientId=u7f095ad6-a13d-4&from=paste&id=ub34916e0&originHeight=1339&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ua1e92297-5d58-4f8c-9711-aab78da52d9&title=%EF%BC%88%E6%94%B9%EF%BC%9A%E5%BA%94%E4%B8%BA%20-c%20busybox%20ps%EF%BC%89%E7%AC%AC%E4%B8%80%E4%B8%AA%E8%BF%9B%E7%A8%8B%E5%B0%B1%E6%98%AF%20pause "（改：应为 -c busybox ps）第一个进程就是 pause")<br />原理就是这么简单。
<a name="YFzdY"></a>
### 参考资料
**pkg/kubelet/kuberuntime/kuberuntime_manager.go#L678: **[**https://github.com/kubernetes/kubernetes/blob/v1.26.1/pkg/kubelet/kuberuntime/kuberuntime_manager.go#L678**](https://github.com/kubernetes/kubernetes/blob/v1.26.1/pkg/kubelet/kuberuntime/kuberuntime_manager.go#L678)<br />**gcr.io/google_containers/pause-amd64: **[**https://console.cloud.google.com/gcr/images/google-containers/GLOBAL/pause-amd64**](https://console.cloud.google.com/gcr/images/google-containers/GLOBAL/pause-amd64)<br />**pause.c: **[**https://github.com/kubernetes/kubernetes/blob/master/build/pause/linux/pause.c**](https://github.com/kubernetes/kubernetes/blob/master/build/pause/linux/pause.c)
