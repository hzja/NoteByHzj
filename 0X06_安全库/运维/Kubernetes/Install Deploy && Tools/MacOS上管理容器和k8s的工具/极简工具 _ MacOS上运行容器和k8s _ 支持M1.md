Kubernetes MacOS<br />Colima 是一个以最小化设置来在MacOS上运行容器运行时和 Kubernetes 的工具。支持 m1（文末讨论），同样也支持 Linux。<br />Colima 的名字取自 Container on Lima。Lima 是一个虚拟机工具，可以实现自动的文件共享、端口转发以及 containerd。<br />Colima 实际上是通过 Lima 启动了名为 colima 的虚拟机，使用虚拟机中的 containerd 作为容器运行时。<br />![](https://cdn.nlark.com/yuque/0/2021/gif/396745/1640571444179-ec934874-20e3-46bb-b955-3150334150fe.gif#clientId=ufb56d213-d106-4&from=paste&id=u86f32bbf&originHeight=449&originWidth=721&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub4155c54-070a-438b-99e9-32a1d5f9e1b&title=)
<a name="F4Icc"></a>
## 使用
Colima 的使用很简单，执行下面的命令就可以创建虚拟机，默认是 Docker 的运行时。<br />初次运行需要下载虚拟机镜像创建虚拟机，耗时因网络情况有所差异。之后，启动虚拟机就只需要 30s 左右的时间。
```bash
colima start
INFO[0000] starting colima
INFO[0000] creating and starting ...                     context=vm
INFO[0119] provisioning ...                              context=docker
INFO[0119] provisioning in VM ...                        context=docker
INFO[0133] restarting VM to complete setup ...           context=docker
INFO[0133] stopping ...                                  context=vm
INFO[0136] starting ...                                  context=vm
INFO[0158] starting ...                                  context=docker
INFO[0159] done
```
此时，在宿主机上就可以使用 Docker 相关的命令了：
```bash
docker ps
CONTAINER ID   IMAGE     COMMAND   CREATED   STATUS    PORTS     NAMES

docker pull busybox

docker images
REPOSITORY   TAG       IMAGE ID       CREATED       SIZE
busybox      latest    b34806a1af7a   2 weeks ago   1.41MB
```
也可以使用 Lima 的命令行 limact工具查看虚拟机的情况：
```bash
limactl list
NAME      STATUS     SSH                ARCH       CPUS    MEMORY    DISK     DIR
colima    Running    127.0.0.1:64505    aarch64    2       2GiB      60GiB    /Users/addo/.lima/colima
```
查看操作系统信息：
```bash
uname -a
Darwin Addos-Macbook-Pro.local 21.2.0 Darwin Kernel Version 21.2.0: Sun Nov 28 20:28:41 PST 2021; root:xnu-8019.61.5~1/RELEASE_ARM64_T6000 arm64

limactl shell colima uname -a
Linux lima-colima 5.13.0-22-generic #22-Ubuntu SMP Fri Nov 5 13:22:27 UTC 2021 aarch64 aarch64 aarch64 GNU/Linux
```
或者使用 Colima 的 ssh 命令进入虚拟机：
```bash
# on host
colima ssh
# in vm
uname -a
Linux lima-colima 5.13.0-22-generic #22-Ubuntu SMP Fri Nov 5 13:22:27 UTC 2021 aarch64 aarch64 aarch64 GNU/Linux
```
<a name="EuRR2"></a>
### 其他运行时
也可以在创建的时候通过 `--runtime containerd` 参数指定使用 Containerd 作为运行时。此时就需要使用 `colima nerdctl` 来使用 `nerdctl` 与 Containerd 进行交互。
```bash
colima start --runtime containerd
```
同样，还可以创建一个 k3s 作为 Kubernetes 运行时：
```bash
colima start --with-kubernetes
```
<a name="zlVG2"></a>
## Demo
尝试启动一个 nginx 容器：
```bash
docker run --rm -d --name nginx -p 8080:80 nginx:latest

docker ps
CONTAINER ID   IMAGE          COMMAND                  CREATED         STATUS         PORTS                                   NAMES
20d6c56e038b   nginx:latest   "/docker-entrypoint.…"   9 seconds ago   Up 8 seconds   0.0.0.0:8080->80/tcp, :::8080->80/tcp   nginx
```
Colima 会自动配置端口转发：
```bash
curl -I http://localhost:8080
HTTP/1.1 200 OK
Server: nginx/1.21.4
Date: Sun, 26 Dec 2021 04:17:22 GMT
Content-Type: text/html
Content-Length: 615
Last-Modified: Tue, 02 Nov 2021 14:49:22 GMT
Connection: keep-alive
ETag: "61814ff2-267"
Accept-Ranges: bytes
```
<a name="VaSHr"></a>
## 虚拟机配置
Colima 启动的虚拟机默认是 2CPU、2GiB 内存 和 60GiB 存储。可以在创建时通过 `--cpu` 、`--memory` 和 `--disk` 来分配更多资源。
```bash
colima start --cpu 4 --memory 16
```
也可以修改当前虚拟机的配置：
```bash
colima stop
colima start --cpu 4 --memory 16
```
<a name="LbbON"></a>
## 同类工具比较
其实有不少类似的工具，比如 kind、k3d 和 minikube 三种都是用来创建 Kubernetes 环境。此前用的 k3d 就比较多。<br />对于 Docker 容器环境，这三个其实都没有提供。minikube 的虚拟机中也有容器运行时，但是无法单纯安装 Docker 环境。<br />对于 Kubernetes 环境来说，这几种都适合，相比 Colima 来说还支持创建多个集群（当前 Colima 最新版本是 0.2.2，多集群的支持也在开发中。估计 0.3.0 会提供，毕竟创建多个虚拟机就能实现）。但使用 Colima 的话，Kubernetes 和 Docker 可以共享镜像（本地镜像）和运行时。
<a name="k1UGr"></a>
## 不足
<a name="Wozoj"></a>
### 多集群的支持
前面提到，目前还不支持创建多个 Kubernetes 集群，估计 0.3.0 会提供。
<a name="IXtNy"></a>
### m1 的支持
这里还是要说下 m1，主要用 m1 的电脑，本地的容器运行时用的 Docker Desktop。<br />前面有留意到虚拟机使用的是 aarch64 架构系统，对于某些不支持 arm64 的镜像还是无法运行。毕竟 Lima 是原生支持 m1，而不是使用 Rosetta 转译的 Docker Desktop。<br />有兴趣的同学可以尝试用 Rosetta 转译 Lima。
