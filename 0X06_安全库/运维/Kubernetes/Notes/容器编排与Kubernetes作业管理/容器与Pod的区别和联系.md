Kubernetes容器Pod<br />容器本可以成为轻量级虚拟机的替代品。但是，由于 Docker/OCI 的标准化，最广泛使用的容器形式是每个容器只有一个进程服务。这种方法有很多优点——增加隔离性、简化水平扩展、更高的可重用性等。但是，它也有一个很大的缺点——正常情况下，虚拟（或物理）机器很少只运行一个服务。<br />虽然 Docker 试图提供一些变通方法来创建多服务容器，但 Kubernetes 迈出了更大胆的一步，并选择了一组称为 Pod 的内聚容器作为最小的可部署单元。<br />几年前，偶然发现 Kubernetes 时，之前的虚拟机和裸机经验很快就了解了 Pod。<br />刚开始接触 Kubernetes 时，学到的第一件事就是每个 Pod 都有一个唯一的 IP 和主机名，并且在同一个 Pod 中，容器可以通过 localhost 相互通信。所以，显而易见，一个 Pod 就像一个微型的服务器。<br />但是，过段时间，会发现 Pod 中的每个容器都有一个隔离的文件系统，并且从一个容器内部，看不到在同一 Pod 的其他容器中运行的进程。好吧！也许 Pod 不是一个微型的服务器，而只是一组具有共享网络堆栈的容器。<br />但随后会了解到，Pod 中的容器可以通过共享内存进行通信！所以，在容器之间，网络命名空间不是唯一可以共享的东西……<br />基于最后的发现，所以，决定深入了解：

- Pod 是如何在底层实现的
- Pod 和 Container 之间的实际区别是什么
- 如何使用 Docker 创建 Pod

在此过程中，希望它能帮助巩固 Linux、Docker 和 Kubernetes 技能。
<a name="EIrAc"></a>
## 1、探索 Container
OCI 运行时规范并不将容器实现仅限于 Linux 容器，即使用 namespace 和 cgroup 实现的容器。但是，除非另有明确说明，否则本文中的容器一词指的是这种相当传统的形式。
<a name="ultfY"></a>
### 设置实验环境（playground）
在了解构成容器的 namespace 和 cgroups 之前，快速设置一个实验环境：
```bash
$ cat > Vagrantfile <<EOF
# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure("2") do |config|
  config.vm.box = "debian/buster64"
  config.vm.hostname = "docker-host"
  config.vm.define "docker-host"
  config.vagrant.plugins = ['vagrant-vbguest']

  config.vm.provider "virtualbox" do |vb|
    vb.cpus = 2
    vb.memory = "2048"
  end

  config.vm.provision "shell", inline: <<-SHELL
    apt-get update
    apt-get install -y curl vim
  SHELL

  config.vm.provision "docker"
end
EOF

$ vagrant up
$ vagrant ssh
```
最后启动一个容器：
```bash
$ docker run --name foo --rm -d --memory='512MB' --cpus='0.5' nginx
```
<a name="xDYxa"></a>
### 探索容器的 namespace
首先来看一下，当容器启动后，哪些隔离原语（primitives）被创建了：
```bash
# Look up the container in the process tree.
$ ps auxf
USER       PID  ...  COMMAND
...
root      4707       /usr/bin/containerd-shim-runc-v2 -namespace moby -id cc9466b3e...
root      4727        \_ nginx: master process nginx -g daemon off;
systemd+  4781            \_ nginx: worker process
systemd+  4782            \_ nginx: worker process

# Find the namespaces used by 4727 process.
$ sudo lsns
NS TYPE   NPROCS   PID USER    COMMAND
...
4026532157 mnt         3  4727 root    nginx: master process nginx -g daemon off;
4026532158 uts         3  4727 root    nginx: master process nginx -g daemon off;
4026532159 ipc         3  4727 root    nginx: master process nginx -g daemon off;
4026532160 pid         3  4727 root    nginx: master process nginx -g daemon off;
4026532162 net         3  4727 root    nginx: master process nginx -g daemon off;
```
可以看到用于隔离以上容器的命名空间是以下这些：

- mnt（挂载）：容器有一个隔离的挂载表。
- uts（Unix 时间共享）：容器拥有自己的 hostname 和 domain。
- ipc（进程间通信）：容器内的进程可以通过系统级 IPC 和同一容器内的其他进程进行通信。
- pid（进程 ID）：容器内的进程只能看到在同一容器内或拥有相同的 PID 命名空间的其他进程。
- net（网络）：容器拥有自己的网络堆栈。

注意，用户（user）命名空间没有被使用，OCI 运行时规范提及了对用户命名空间的支持。不过，虽然 Docker 可以将此命名空间用于其容器，但由于固有的限制，它默认情况下没有使用。因此，容器中的 root 用户很可能是主机系统中的 root 用户。谨防！<br />另一个没有出现在这里的命名空间是 cgroup。花了一段时间才理解 cgroup 命名空间与 cgroups 机制（mechanism）的不同。Cgroup 命名空间仅提供一个容器的 cgroup 层次结构的孤立视图。同样，Docker 也支持将容器放入私有 cgroup 命名空间，但默认情况下没有这么做。
<a name="ukQvj"></a>
### 探索容器的 cgroups
Linux 命名空间可以让容器中的进程认为自己是在一个专用的机器上运行。但是，看不到别的进程并不意味着不会受到其他进程的影响。一些耗资源的进程可能会意外的过多消耗宿主机上面共享的资源。<br />这时候就需要 cgroups 的帮助！<br />可以通过检查 cgroup 虚拟文件系统中的相应子树来查看给定进程的 cgroups 限制。Cgroupfs 通常被挂在 /sys/fs/cgroup 目录，并且进程特定相关的部分可以在 /proc/<pid>/cgroup 中查看：
```bash
PID=$(docker inspect --format '{{.State.Pid}}' foo)

# Check cgroupfs node for the container main process (4727).
$ cat /proc/${PID}/cgroup
11:freezer:/docker/cc9466b3eb67ca374c925794776aad2fd45a34343ab66097a44594b35183dba0
10:blkio:/docker/cc9466b3eb67ca374c925794776aad2fd45a34343ab66097a44594b35183dba0
9:rdma:/
8:pids:/docker/cc9466b3eb67ca374c925794776aad2fd45a34343ab66097a44594b35183dba0
7:devices:/docker/cc9466b3eb67ca374c925794776aad2fd45a34343ab66097a44594b35183dba0
6:cpuset:/docker/cc9466b3eb67ca374c925794776aad2fd45a34343ab66097a44594b35183dba0
5:cpu,cpuacct:/docker/cc9466b3eb67ca374c925794776aad2fd45a34343ab66097a44594b35183dba0
4:memory:/docker/cc9466b3eb67ca374c925794776aad2fd45a34343ab66097a44594b35183dba0
3:net_cls,net_prio:/docker/cc9466b3eb67ca374c925794776aad2fd45a34343ab66097a44594b35183dba0
2:perf_event:/docker/cc9466b3eb67ca374c925794776aad2fd45a34343ab66097a44594b35183dba0
1:name=systemd:/docker/cc9466b3eb67ca374c925794776aad2fd45a34343ab66097a44594b35183dba0
0::/system.slice/containerd.service
```
似乎 Docker 使用 /docker/<container-id> 模式。好吧，不管怎样：
```bash
ID=$(docker inspect --format '{{.Id}}' foo)

# Check the memory limit.
$ cat /sys/fs/cgroup/memory/docker/${ID}/memory.limit_in_bytes
536870912  # Yay! It's the 512MB we requested!

# See the CPU limits.
ls /sys/fs/cgroup/cpu/docker/${ID}
```
有趣的是在不明确设置任何资源限制的情况下启动容器都会配置一个 cgroup。实际中没有检查过，但猜测是默认情况下，CPU 和 RAM 消耗不受限制，Cgroups 可能用来限制从容器内部对某些设备的访问。<br />这是在调查后脑海中呈现的容器：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667221238939-1ac94685-a92c-4bd3-95e8-29847ff8d297.png#clientId=u690270f9-dbcc-4&from=paste&id=ufd9172dd&originHeight=593&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0bec98b2-91a2-49f6-ab5d-eb8aee25b7d&title=)
<a name="W2bmQ"></a>
## 2、探索 Pod
现在，来看看 Kubernetes Pod。与容器一样，Pod 的实现可以在不同的 CRI 运行时（runtime）之间变化。例如，当 Kata 容器被用来作为一个支持的运行时类时，某些 Pod 可以就是真实的虚拟机了！并且正如预期的那样，基于 VM 的 Pod 与传统 Linux 容器实现的 Pod 在实现和功能方面会有所不同。<br />为了保持容器和 Pod 之间公平比较，会在使用 ContainerD/Runc 运行时的 Kubernetes 集群上进行探索。这也是 Docker 在底层运行容器的机制。
<a name="ekaUn"></a>
### 设置实验环境（playground）
这次使用基于 VirtualBox driver 和 Containd 运行时的 minikube 来设置实验环境。要快速安装 minikube 和 kubectl，可以使用 Alex Ellis 编写的 arkade 工具：
```bash
# Install arkade ()
$ curl -sLS https://get.arkade.dev | sh

$ arkade get kubectl minikube

$ minikube start --driver virtualbox --container-runtime containerd
```
实验的 Pod，可以按照下面的方式设置：
```bash
$ kubectl --context=minikube apply -f - <<EOF
apiVersion: v1
kind: Pod
metadata:
  name: foo
spec:
  containers:
    - name: app
      image: docker.io/kennethreitz/httpbin
      ports:
        - containerPort: 80
      resources:
        limits:
          memory: "256Mi"
    - name: sidecar
      image: curlimages/curl
      command: ["/bin/sleep", "3650d"]
      resources:
        limits:
          memory: "128Mi"
EOF
```
<a name="s7S3Z"></a>
### 探索 Pod 的容器
实际的 Pod 检查应在 Kubernetes 集群节点上进行：
```
$ minikube ssh
```
看看那里 Pod 的进程：
```
$ ps auxf
USER       PID  ...  COMMAND
...
root      4947         \_ containerd-shim -namespace k8s.io -workdir /mnt/sda1/var/lib/containerd/...
root      4966             \_ /pause
root      4981         \_ containerd-shim -namespace k8s.io -workdir /mnt/sda1/var/lib/containerd/...
root      5001             \_ /usr/bin/python3 /usr/local/bin/gunicorn -b 0.0.0.0:80 httpbin:app -k gevent
root      5016                 \_ /usr/bin/python3 /usr/local/bin/gunicorn -b 0.0.0.0:80 httpbin:app -k gevent
root      5018         \_ containerd-shim -namespace k8s.io -workdir /mnt/sda1/var/lib/containerd/...
100       5035             \_ /bin/sleep 3650d
```
基于运行的时间，上述三个进程组很有可能是在 Pod 启动期间创建。这很有意思，因为在清单文件中，只有两个容器，httpbin 和 sleep。<br />可以使用名为 ctr 的 ContainerD 命令行来交叉检查上述的发现：
```bash
$ sudo ctr --namespace=k8s.io containers ls
CONTAINER      IMAGE                                   RUNTIME
...
097d4fe8a7002  docker.io/curlimages/curl@sha256:1a220  io.containerd.runtime.v1.linux
...
dfb1cd29ab750  docker.io/kennethreitz/httpbin:latest   io.containerd.runtime.v1.linux
...
f0e87a9330466  k8s.gcr.io/pause:3.1                    io.containerd.runtime.v1.linux
```
的确是三个容器被创建了。同时，使用另一个和 CRI 运行时监控的命令行 crictl 检测发现，仅仅只有两个容器：
```bash
$ sudo crictl ps
CONTAINER      IMAGE          CREATED            STATE    NAME     ATTEMPT  POD ID
097d4fe8a7002  bcb0c26a91c90  About an hour ago  Running  sidecar  0        f0e87a9330466
dfb1cd29ab750  b138b9264903f  About an hour ago  Running  app      0        f0e87a9330466
```
但是注意，上述的 POD ID 字段和 ctr 输出的 pause:3.1 容器 id 一致。好吧，看上去这个 Pod 是一个辅助容器。所以，它有什么用呢？<br />还没有注意到在 OCI 运行时规范中有和 Pod 相对应的东西。因此，当对 Kubernetes API 规范提供的信息不满意时，通常直接进入 Kubernetes Container Runtime 接口（CRI）Protobuf 文件中查找相应的信息：
```
// kubelet expects any compatible container runtime
// to implement the following gRPC methods:

service RuntimeService {
    ...
    rpc RunPodSandbox(RunPodSandboxRequest) returns (RunPodSandboxResponse) {}    
    rpc StopPodSandbox(StopPodSandboxRequest) returns (StopPodSandboxResponse) {}    
    rpc RemovePodSandbox(RemovePodSandboxRequest) returns (RemovePodSandboxResponse) {}    
    rpc PodSandboxStatus(PodSandboxStatusRequest) returns (PodSandboxStatusResponse) {}
    rpc ListPodSandbox(ListPodSandboxRequest) returns (ListPodSandboxResponse) {}

    rpc CreateContainer(CreateContainerRequest) returns (CreateContainerResponse) {}
    rpc StartContainer(StartContainerRequest) returns (StartContainerResponse) {}    
    rpc StopContainer(StopContainerRequest) returns (StopContainerResponse) {}    
    rpc RemoveContainer(RemoveContainerRequest) returns (RemoveContainerResponse) {}
    rpc ListContainers(ListContainersRequest) returns (ListContainersResponse) {}    
    rpc ContainerStatus(ContainerStatusRequest) returns (ContainerStatusResponse) {}    
    rpc UpdateContainerResources(UpdateContainerResourcesRequest) returns (UpdateContainerResourcesResponse) {}    
    rpc ReopenContainerLog(ReopenContainerLogRequest) returns (ReopenContainerLogResponse) {}

    // ...    
}

message CreateContainerRequest {
    // ID of the PodSandbox in which the container should be created.
    string pod_sandbox_id = 1;
    // Config of the container.
    ContainerConfig config = 2;
    // Config of the PodSandbox. This is the same config that was passed
    // to RunPodSandboxRequest to create the PodSandbox. It is passed again
    // here just for easy reference. The PodSandboxConfig is immutable and
    // remains the same throughout the lifetime of the pod.
    PodSandboxConfig sandbox_config = 3;
}
```
所以，Pod 实际上就是由沙盒以及在沙盒中运行的容器组成的。沙盒管理 Pod 中所有容器的常用资源，pause 容器会在 RunPodSandbox() 调用中被启动。简单的互联网搜索就发现了该容器仅仅是一个 idle 进程。
<a name="zaBr9"></a>
### 探索 Pod 的命名空间
下面就是集群节点上的命名空间：
```bash
$ sudo lsns
        NS TYPE   NPROCS   PID USER            COMMAND
4026532614 net         4  4966 root            /pause
4026532715 mnt         1  4966 root            /pause
4026532716 uts         4  4966 root            /pause
4026532717 ipc         4  4966 root            /pause
4026532718 pid         1  4966 root            /pause
4026532719 mnt         2  5001 root            /usr/bin/python3 /usr/local/bin/gunicorn -b 0.0.0.0:80 httpbin:app -k gevent
4026532720 pid         2  5001 root            /usr/bin/python3 /usr/local/bin/gunicorn -b 0.0.0.0:80 httpbin:app -k gevent
4026532721 mnt         1  5035 100             /bin/sleep 3650d
4026532722 pid         1  5035 100             /bin/sleep 3650d
```
前面第一部分很像 Docker 容器，pause 容器有五个命名空间：net、mnt、uts、ipc 以及 pid。但是很明显，httpbin 和 sleep 容器仅仅有两个命名空间：mnt 和 pid。这是怎么回事？<br />事实证明，lsns 不是检查进程名称空间的最佳工具。相反，要检查某个进程使用的命名空间，可以参考 /proc/${pid}/ns 位置：
```
# httpbin container
sudo ls -l /proc/5001/ns
...
lrwxrwxrwx 1 root root 0 Oct 24 14:05 ipc -> 'ipc:[4026532717]'
lrwxrwxrwx 1 root root 0 Oct 24 14:05 mnt -> 'mnt:[4026532719]'
lrwxrwxrwx 1 root root 0 Oct 24 14:05 net -> 'net:[4026532614]'
lrwxrwxrwx 1 root root 0 Oct 24 14:05 pid -> 'pid:[4026532720]'
lrwxrwxrwx 1 root root 0 Oct 24 14:05 uts -> 'uts:[4026532716]'

# sleep container
sudo ls -l /proc/5035/ns
...
lrwxrwxrwx 1 100 101 0 Oct 24 14:05 ipc -> 'ipc:[4026532717]'
lrwxrwxrwx 1 100 101 0 Oct 24 14:05 mnt -> 'mnt:[4026532721]'
lrwxrwxrwx 1 100 101 0 Oct 24 14:05 net -> 'net:[4026532614]'
lrwxrwxrwx 1 100 101 0 Oct 24 14:05 pid -> 'pid:[4026532722]'
lrwxrwxrwx 1 100 101 0 Oct 24 14:05 uts -> 'uts:[4026532716]'
```
虽然不太容易去注意到，但 httpbin 和 sleep 容器实际上重用了 pause 容器的 net、uts 和 ipc 命名空间！<br />可以用 crictl 交叉检测验证：
```bash
# Inspect httpbin container.
$ sudo crictl inspect dfb1cd29ab750
{
  ...
  "namespaces": [
    {
      "type": "pid"
    },
    {
      "type": "ipc",
      "path": "/proc/4966/ns/ipc"
    },
    {
      "type": "uts",
      "path": "/proc/4966/ns/uts"
    },
    {
      "type": "mount"
    },
    {
      "type": "network",
      "path": "/proc/4966/ns/net"
    }
  ],
  ...
}

# Inspect sleep container.
$ sudo crictl inspect 097d4fe8a7002
...
```
上述发现完美的解释了同一个 Pod 中容器具有的能力：

- 能够互相通信
- 通过 localhost 和/或
- 使用 IPC（共享内存，消息队列等）
- 共享 domain 和 hostname

然而，在看过所有这些命名空间如何在容器之间自由重用之后，开始怀疑默认边界可以被打破。实际上，在对 Pod API 规范的更深入阅读后发现，将 shareProcessNamespace 标志设置为 true 时，Pod 的容器将拥有四个通用命名空间，而不是默认的三个。但是有一个更令人震惊的发现——hostIPC、hostNetwork 和 hostPID 标志可以使容器使用相应主机的命名空间。<br />有趣的是，CRI API 规范似乎更加灵活。至少在语法上，它允许将 net、pid 和 ipc 命名空间限定为 CONTAINER、POD 或 NODE。因此，可以构建一个 Pod 使其容器无法通过 localhost 相互通信 。
<a name="H4oef"></a>
### 探索 Pod 的 cgroups
Pod 的 cgroups 是什么样的？systemd-cgls 可以很好地可视化 cgroups 层次结构：
```bash
$ sudo systemd-cgls
Control group /:
-.slice
├─kubepods
│ ├─burstable
│ │ ├─pod4a8d5c3e-3821-4727-9d20-965febbccfbb
│ │ │ ├─f0e87a93304666766ab139d52f10ff2b8d4a1e6060fc18f74f28e2cb000da8b2
│ │ │ │ └─4966 /pause
│ │ │ ├─dfb1cd29ab750064ae89613cb28963353c3360c2df913995af582aebcc4e85d8
│ │ │ │ ├─5001 /usr/bin/python3 /usr/local/bin/gunicorn -b 0.0.0.0:80 httpbin:app -k gevent
│ │ │ │ └─5016 /usr/bin/python3 /usr/local/bin/gunicorn -b 0.0.0.0:80 httpbin:app -k gevent
│ │ │ └─097d4fe8a7002d69d6c78899dcf6731d313ce8067ae3f736f252f387582e55ad
│ │ │   └─5035 /bin/sleep 3650d
...
```
所以，Pod 本身有一个父节点（Node），每个容器也可以单独调整。这符合预期，因为在 Pod 清单中，可以为 Pod 中的每个容器单独设置资源限制。<br />此刻，脑海中的 Pod 看起来是这样的：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667221238957-0d716cc8-b51e-4c8a-b919-ff5bb9989fa9.png#clientId=u690270f9-dbcc-4&from=paste&id=u26734776&originHeight=616&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3176be47-6cc2-43aa-8bf0-4d47ae71b4d&title=)
<a name="Diw9r"></a>
## 3、利用 Docker 实现 Pod
如果 Pod 的底层实现是一组具有共同 cgroup 父级的半融合（emi-fused）容器，是否可以使用 Docker 生产类似 Pod 的构造？<br />最近尝试做了一些类似的事情来让多个容器监听同一个套接字，Docker 可以通过 `docker run --network container:<other-container-name>` 语法来创建一个可以使用已存在的网络命名空间容器。但我也知道 OCI 运行时规范只定义了 create 和 start 命令。<br />因此，当使用 `docker exec <existing-container><command>` 在现有容器中执行命令时，实际上是在运行（即 create 然后 start）一个全新的容器，该容器恰好重用了目标容器的所有命名空间（证明 [1](https://github.com/opencontainers/runtime-spec/issues/345) 和 [2](https://github.com/opencontainers/runtime-spec/pull/388)）。这让我非常有信心可以使用标准 Docker 命令生成 Pod。<br />可以使用仅仅安装了 Docker 的机器作为实验环境。但是这里会使用一个额外的包来简化使用 cgroups：
```bash
$ sudo apt-get install cgroup-tools
```
首先，配置一个父 cgroup 条目。为了简洁起见，将仅使用 CPU 和内存控制器：
```bash
sudo cgcreate -g cpu,memory:/pod-foo

# Check if the corresponding folders were created:
ls -l /sys/fs/cgroup/cpu/pod-foo/
ls -l /sys/fs/cgroup/memory/pod-foo/
```
然后创建一个沙盒容器：
```bash
$ docker run -d --rm \
  --name foo_sandbox \
  --cgroup-parent /pod-foo \
  --ipc 'shareable' \
  alpine sleep infinity
```
最后，启动重用沙盒容器命名空间的实际容器：
```bash
# app (httpbin)
$ docker run -d --rm \
  --name app \
  --cgroup-parent /pod-foo \
  --network container:foo_sandbox \
  --ipc container:foo_sandbox \
  kennethreitz/httpbin

# sidecar (sleep)
$ docker run -d --rm \
  --name sidecar \
  --cgroup-parent /pod-foo \
  --network container:foo_sandbox \
  --ipc container:foo_sandbox \
  curlimages/curl sleep 365d
```
注意到省略了哪个命名空间吗？不能在容器之间共享 uts 命名空间。似乎目前在 `docker run` 命令中没法实现。是有点遗憾。但是除开 uts 命名空间之外，它是成功的！<br />cgroups 看上去很像 Kubernetes 创建的：
```bash
$ sudo systemd-cgls memory
Controller memory; Control group /:
├─pod-foo
│ ├─488d76cade5422b57ab59116f422d8483d435a8449ceda0c9a1888ea774acac7
│ │ ├─27865 /usr/bin/python3 /usr/local/bin/gunicorn -b 0.0.0.0:80 httpbin:app -k gevent
│ │ └─27880 /usr/bin/python3 /usr/local/bin/gunicorn -b 0.0.0.0:80 httpbin:app -k gevent
│ ├─9166a87f9a96a954b10ec012104366da9f1f6680387ef423ee197c61d37f39d7
│ │ └─27977 sleep 365d
│ └─c7b0ec46b16b52c5e1c447b77d67d44d16d78f9a3f93eaeb3a86aa95e08e28b6
│   └─27743 sleep infinity
```
全局命名空间列表看上去也很相似：
```bash
$ sudo lsns
        NS TYPE   NPROCS   PID USER    COMMAND
...
4026532157 mnt         1 27743 root    sleep infinity
4026532158 uts         1 27743 root    sleep infinity
4026532159 ipc         4 27743 root    sleep infinity
4026532160 pid         1 27743 root    sleep infinity
4026532162 net         4 27743 root    sleep infinity
4026532218 mnt         2 27865 root    /usr/bin/python3 /usr/local/bin/gunicorn -b 0.0.0.0:80 httpbin:app -k gevent
4026532219 uts         2 27865 root    /usr/bin/python3 /usr/local/bin/gunicorn -b 0.0.0.0:80 httpbin:app -k gevent
4026532220 pid         2 27865 root    /usr/bin/python3 /usr/local/bin/gunicorn -b 0.0.0.0:80 httpbin:app -k gevent
4026532221 mnt         1 27977 _apt    sleep 365d
4026532222 uts         1 27977 _apt    sleep 365d
4026532223 pid         1 27977 _apt    sleep 365d
```
httpbin 和 sidecar 容器看上去共享了 ipc 和 net 命名空间：
```bash
# app container
$ sudo ls -l /proc/27865/ns
lrwxrwxrwx 1 root root 0 Oct 28 07:56 ipc -> 'ipc:[4026532159]'
lrwxrwxrwx 1 root root 0 Oct 28 07:56 mnt -> 'mnt:[4026532218]'
lrwxrwxrwx 1 root root 0 Oct 28 07:56 net -> 'net:[4026532162]'
lrwxrwxrwx 1 root root 0 Oct 28 07:56 pid -> 'pid:[4026532220]'
lrwxrwxrwx 1 root root 0 Oct 28 07:56 uts -> 'uts:[4026532219]'

# sidecar container
$ sudo ls -l /proc/27977/ns
lrwxrwxrwx 1 _apt systemd-journal 0 Oct 28 07:56 ipc -> 'ipc:[4026532159]'
lrwxrwxrwx 1 _apt systemd-journal 0 Oct 28 07:56 mnt -> 'mnt:[4026532221]'
lrwxrwxrwx 1 _apt systemd-journal 0 Oct 28 07:56 net -> 'net:[4026532162]'
lrwxrwxrwx 1 _apt systemd-journal 0 Oct 28 07:56 pid -> 'pid:[4026532223]'
lrwxrwxrwx 1 _apt systemd-journal 0 Oct 28 07:56 uts -> 'uts:[4026532222]'
```
<a name="skzjM"></a>
## 4、总结
Container 和 Pod 是相似的。在底层，它们主要依赖 Linux 命名空间和 cgroup。但是，Pod 不仅仅是一组容器。Pod 是一个自给自足的高级构造。所有 Pod 的容器都运行在同一台机器（集群节点）上，它们的生命周期是同步的，并且通过削弱隔离性来简化容器间的通信。这使得 Pod 更接近于传统的 VM，带回了熟悉的部署模式，如 sidecar 或反向代理。<br />相关链接：

1. [https://github.com/opencontainers/runtime-spec/issues/345](https://github.com/opencontainers/runtime-spec/issues/345)
2. [https://github.com/opencontainers/runtime-spec/pull/388](https://github.com/opencontainers/runtime-spec/pull/388)
