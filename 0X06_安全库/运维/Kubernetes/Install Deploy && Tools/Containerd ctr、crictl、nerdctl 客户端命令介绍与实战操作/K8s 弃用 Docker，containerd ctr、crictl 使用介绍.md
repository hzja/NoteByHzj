Kubernetes<br />containerd 是一个高级容器运行时，又名 容器管理器。简单来说，它是一个守护进程，在单个主机上管理完整的容器生命周期：创建、启动、停止容器、拉取和存储镜像、配置挂载、网络等。<br />containerd 旨在轻松嵌入到更大的系统中。Docker 在底层使用 containerd 来运行容器。Kubernetes 可以通过 CRI 使用 containerd 来管理单个节点上的容器。但是较小的项目也可以从与 containerd 的轻松集成中受益——例如，faasd 使用 containerd（我们需要更多的 d！）在独立服务器上启动一个服务。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680051131354-76a99544-eec3-4439-ad37-ab17e0f0377f.png#averageHue=%23dca931&clientId=u84032f54-e2b7-4&from=paste&id=u1cb21e5b&originHeight=597&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0dc9dbd4-5cdb-4d98-9d73-b1a42e41807&title=)<br />但是，以编程方式使用 containerd 并不是唯一的选择。它还可以通过可用客户端之一从命令行使用。由此产生的容器 UX 可能不像 docker 客户端提供的那样全面和用户友好，但它仍然是有用的，例如，用于调试或学习目的。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680051131395-04b78f2d-2fc4-4042-af8b-63fb40ea425a.png#averageHue=%23f9f8f4&clientId=u84032f54-e2b7-4&from=paste&id=uce73824a&originHeight=603&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9a3bbc88-3653-41c8-9b06-da80ad22419&title=)
<a name="SUNAq"></a>
## 如何在 ctr 中使用 containerd
ctr 是作为 containerd 项目的一部分提供的命令行客户端。如果在机器上运行了 containerd，则 ctr 二进制文件很可能也在那里。<br />该 ctr 界面 与 Docker CLI 不兼容，乍一看，可能看起来不那么用户友好。显然，它的主要受众是测试守护进程的容器开发人员。但是，由于它是最接近实际 containerd API 的东西，因此它可以作为一种很好的探索手段——通过检查可用命令，可以大致了解 containerd 可以做什么和不能做什么。<br />ctr 也非常适合学习的能力低级别的使用人员，因为 ctr + containerd 是更接近实际的容器比 docker + dockerd。
<a name="TgvLK"></a>
### 使用 ctr 处理容器镜像
拉取镜像，似乎是必需和完全合规的，但是你不能忽略注册表或标签部分：
```bash
$ ctr images pull docker.io/library/nginx:1.21
$ ctr images pull docker.io/kennethreitz/httpbin:latest
$ ctr images pull docker.io/kennethreitz/httpbin:latest
$ ctr images pull quay.io/quay/redis:latest
```
要列出本地镜像，可以使用：
```bash
$ ctr images ls
```
令人惊讶的是，containerd 不提供开箱即用的镜像构建支持。但是 containerd 经常被更高级别的工具用来构建镜像。<br />ctr 可以导入使用 docker build 或其他 OCI 兼容软件构建的现有镜像，而不是使用构建镜像：
```bash
$ docker build -t my-app .
$ docker save -o my-app.tar my-app

$ ctr images import my-app.tar
```
使用 ctr，还可以学习和探索下挂载镜像：
```bash
$ mkdir /tmp/httpbin
$ ctr images mount docker.io/kennethreitz/httpbin:latest /tmp/httpbin

$ ls -l /tmp/httpbin/
total 80
drwxr-xr-x 2 root root 4096 Oct 18  2018 bin
drwxr-xr-x 2 root root 4096 Apr 24  2018 boot
drwxr-xr-x 4 root root 4096 Oct 18  2018 dev
drwxr-xr-x 1 root root 4096 Oct 24  2018 etc
drwxr-xr-x 2 root root 4096 Apr 24  2018 home
drwxr-xr-x 3 root root 4096 Oct 24  2018 httpbin
...

$ ctr images unmount /tmp/httpbin
```
要使用删除镜像 ctr，请运行：
```bash
$ ctr images remove docker.io/library/nginx:1.21
```
<a name="pDNTl"></a>
### 使用 ctr 处理容器
可以运行一个容器用`ctr run image-ref container-id`。例如：
```bash
$ ctr run --rm -t docker.io/library/debian:latest cont1
```
请注意，ctr 与用户友好地 docker run 生成唯一容器 ID 不同，必须自己提供唯一容器 ID。该 ctr run 命令还只支持一些熟悉的 docker run 标志：`--env`，-t,--tty，-d,--detach，--rm等，但没有端口指定或自动重启容器`--restart=always`。<br />与镜像类似，可以使用以下命令列出现有容器：
```bash
$ ctr containers ls
```
有趣的是，该 ctr run 命令实际上是快捷方式`ctr container create + ctr task start`：
```bash
$ ctr container create -t docker.io/library/nginx:latest nginx_1
$ ctr container ls
CONTAINER    IMAGE                              RUNTIME
nginx_1      docker.io/library/nginx:latest     io.containerd.runc.v2

$ ctr task ls
TASK    PID    STATUS        # Empty!

$ ctr task start -d nginx_1  # -d for --detach
$ ctr task list
TASK     PID      STATUS
nginx_1  10074    RUNNING
```
我喜欢 container 和 task 子命令的这种分离，因为它反映了 OCI 容器经常被遗忘的性质。尽管普遍认为容器不是进程 -容器是资源被隔离和执行受限制的进程。<br />使用 ctr task attach，可以重新连接到在容器内运行的现有任务的 stdio 流：
```bash
$ ctr task attach nginx_1
2021/09/12 15:42:20 [notice] 1#1: using the "epoll" event method
2021/09/12 15:42:20 [notice] 1#1: nginx/1.21.3
2021/09/12 15:42:20 [notice] 1#1: built by gcc 8.3.0 (Debian 8.3.0-6)
2021/09/12 15:42:20 [notice] 1#1: OS: Linux 4.19.0-17-amd64
2021/09/12 15:42:20 [notice] 1#1: getrlimit(RLIMIT_NOFILE): 1024:1024
2021/09/12 15:42:20 [notice] 1#1: start worker processes
2021/09/12 15:42:20 [notice] 1#1: start worker process 31
...
```
与 docker 非常相似，可以在现有容器中执行命令：
```bash
$ ctr task exec -t --exec-id bash_1 nginx_1 bash

# From inside the container:
$ root@host:/# curl 127.0.0.1:80
<!DOCTYPE html>
<html>
<head>
<title>Welcome to nginx!</title>
<style>
...
```
在删除容器之前，必须停止其所有任务：
```bash
$ ctr task kill -9 nginx_1
```
或者，可以使用以下标志`--force` 删除正在运行的任务：
```bash
$ ctr task rm -f nginx_1
```
最后，要删除容器，请运行：
```bash
$ ctr container rm nginx_1
```
<a name="ImC1v"></a>
## 如何在 nerdctl 中使用 containerd
nerdctl 是一个相对较新的 containerd 命令行客户端。与 ctr，nerdctl 旨在用户友好且与 Docker 兼容。在某种程度上，nerdctl + containerd可以无缝替换docker + dockerd. 但是，这似乎不是该项目的目标：<br />nerdctl 其目标是促进试验 Docker 中不存在的 containerd 的尖端功能。此类功能包括但不限于延迟拉取 (stargz) 和图像加密 (ocicrypt)。预计这些功能最终也将在 Docker 中可用，但是，这可能需要几个月甚至几年的时间，因为 Docker 目前被设计为仅使用容器子系统的一小部分。重构 Docker 以使用整个 containerd 是可能的，但并不简单。所以我们决定创建一个完全使用 containerd 的新 CLI，但我们不打算用 Docker 来完成。我们一直在为 Docker/Moby 以及 containerd 做出贡献，并将继续这样做。<br />从基本使用的角度来看，与 相比 ctr，nerdctl 支持：

- 镜像管理 `nerdctl build`
- 容器网络管理
- Docker 与 `nerdctl compose up`

最酷的部分是 nerdctl 尝试提供与 docker（和 podman）相同的命令行用户体验。因此，如果熟悉 docker（或 podman）CLI，那么您已经熟悉 nerdctl。
<a name="tsQCD"></a>
## 如何在 crictl 中使用 containerd
crictl 是 Kubernetes CRI 兼容容器运行时的命令行客户端。<br />引入了 Kubernetes 容器运行时接口 (CRI)以使 Kubernetes 容器运行时充满魔法。Kubernetes 节点代理 kubelet 实现了 CRI 客户端 API，可以使用任何实现 CRI 服务器 API 的容器运行时来管理其节点上的容器和 pod。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680051131415-0a7fd2c8-1243-468b-8ab2-4d2fcf4e21b2.png#averageHue=%23d99910&clientId=u84032f54-e2b7-4&from=paste&id=uc051ba59&originHeight=889&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uefac9ae0-2ed7-4c5d-9796-0fbd158b120&title=)<br />从 1.1 版开始，containerd 带有一个内置的 CRI 插件。因此，containerd 是一个兼容 CRI 的容器运行时。因此，它可以与 crictl 共存.<br />crictl 创建用于检查和调试 Kubernetes 节点上的容器运行时和应用程序。它支持以下操作：
```
attach: Attach to a running container
create: Create a new container
exec: Run a command in a running container
version: Display runtime version information
images, image, img: List images
inspect: Display the status of one or more containers
inspecti: Return the status of one or more images
imagefsinfo: Return image filesystem info
inspectp: Display the status of one or more pods
logs: Fetch the logs of a container
port-forward: Forward local port to a pod
ps: List containers
pull: Pull an image from a registry
run: Run a new container inside a sandbox
runp: Run a new pod
rm: Remove one or more containers
rmi: Remove one or more images
rmp: Remove one or more pods
pods: List pods
start: Start one or more created containers
info: Display information of the container runtime
stop: Stop one or more running containers
stopp: Stop one or more running pods
update: Update one or more running containers
config: Get and set crictl client configuration options
stats: List container(s) resource usage statistics
```
这里有趣的部分是，通过 crictl + containerdbundle 可以了解 pod 是如何实现的，但是这个话题值得自己去探索。
