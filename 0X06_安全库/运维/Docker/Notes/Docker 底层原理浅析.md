Docker<br />容器的实质是进程，与宿主机上的其他进程是共用一个内核，但与直接在宿主机执行的进程不同，容器进程运行在属于自己的独立的命名空间。命名空间隔离了进程间的资源，使得 a,b 进程可以看到 S 资源，而 c 进程看不到。
<a name="ykhUQ"></a>
## 1. 演进
对于统一开发、测试、生产环境的渴望，要远远早于 docker 的出现。先来了解一下在 docker 之前出现过哪些解决方案。
<a name="XMAsZ"></a>
### 1.1 vagrant
Vagarant 是最早接触到的一个解决环境配置不统一的技术方案。它使用 Ruby 语言编写，由 HashCorp 公司在 2010 年 1 月发布。Vagrant 的底层是虚拟机，最开始选用的是 virtualbox。一个个已经配置好的虚拟机被称作 box。用户可自由在虚拟机内部的安装依赖库和软件服务，并将 box 发布。通过简单的命令，就能够拉取 box，将环境搭建起来。
```bash
// 拉取一个ubuntu12.04的box
$ vagrant init hashicorp/precise32

// 运行该虚拟机
$ vagrant up

// 查看当前本地都有哪些box
$ vagrant box list
```
如果需要运行多个服务，也可以通过编写 vagrantfile，将相互依赖的服务一起运行，颇有如今 docker-compose 的味道。
```bash
config.vm.define("web") do |web|web.vm.box = "apache"
end
config.vm.define("db") do |db|db.vm.box = "mysql”
end
```
![2021-05-03-11-23-52-837930.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620012356297-a50b0b0d-e6a9-4625-a740-12a9d03f9d02.jpeg#clientId=ucf95d1f1-da39-4&from=ui&id=u9c91b260&originHeight=656&originWidth=864&originalType=binary&ratio=1&rotation=0&showTitle=false&size=38780&status=done&style=shadow&taskId=udb20d44f-8b09-4902-a2b8-b9392f5b2f7&title=)
<a name="KanC7"></a>
### 1.2 LXC (LinuX Container)
![2021-05-03-11-23-53-050830.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620012369736-7c00a8cf-bc84-4e94-b21b-9e4ea84330fe.jpeg#clientId=ucf95d1f1-da39-4&from=ui&id=u432eb701&originHeight=476&originWidth=465&originalType=binary&ratio=1&rotation=0&showTitle=false&size=13610&status=done&style=shadow&taskId=udfec5295-933c-4466-bded-9a68f691179&title=)<br />在 2008 年，Linux 2.6.24 将 cgroups 特性合入了主干。Linux Container 是 Canonical 公司基于 namespace 和 cgroups 等技术，瞄准容器世界而开发的一个项目，目标就是要创造出运行在 Linux 系统中，并且隔离性良好的容器环境。当然它最早也就见于 Ubuntu 操作系统上。<br />2013 年，在 PyCon 大会上 Docker 正式面世。当时的 Docker 是在 Ubuntu 12.04 上开发实现的，只是基于 LXC 之上的一个工具，屏蔽掉了 LXC 的使用细节（类似于 vagrant 屏蔽了底层虚拟机），让用户可以一句  docker run  命令行便创建出自己的容器环境。
<a name="ij8A5"></a>
## 2. 技术发展
容器技术是操作系统层面的虚拟化技术，可以概括为使用 Linux 内核的 cgroup，namespace 等技术，对进程进行的封装隔离。早在  Docker 之前，Linux 就已经提供了今天的 Docker 所使用的那些基础技术。Docker 一夜之间火爆全球，但技术上的积累并不是瞬间完成的。摘取其中几个关键技术节点进行介绍。<br />![2021-05-03-11-23-53-255345.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620012382405-442cc1f5-f979-4f23-91aa-7357f9d33e4f.jpeg#clientId=ucf95d1f1-da39-4&from=ui&id=u5e737dbc&originHeight=106&originWidth=554&originalType=binary&ratio=1&rotation=0&showTitle=false&size=6830&status=done&style=none&taskId=ud9fd38d2-0aeb-4435-aac1-63b053bb4c4&title=)
<a name="x3eHj"></a>
### 2.1 Chroot
软件主要分为系统软件和应用软件，而容器中运行的程序并非系统软件。容器中的进程实质上是运行在宿主机上，与宿主机上的其他进程共用一个内核。而每个应用软件运行都需要有必要的环境，包括一些 lib 库依赖之类的。所以，为了避免不同应用程序的 lib 库依赖冲突，很自然地会想是否可以把他们进行隔离，让他们看到的库是不一样的。基于这个朴素的想法，1979 年， chroot 系统调用首次问世。来举个例子感受一下。在 devcloud 上申请的云主机，现在在 home 目录下准备好了一个 alpine 系统的 rootfs，如下：<br />![2021-05-03-11-23-53-455812.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620012391369-1decdf76-af22-4806-9a0e-9bd74a8af481.jpeg#clientId=ucf95d1f1-da39-4&from=ui&id=ube893e60&originHeight=79&originWidth=713&originalType=binary&ratio=1&rotation=0&showTitle=false&size=13736&status=done&style=none&taskId=u6557d848-ab6a-475c-b575-cf8c04bef92&title=)<br />在该目录下执行：
```bash
chroot rootfs/ /bin/bash
```
然后将/etc/os-release 打印出来，就看到是“Alpine Linux”，说明新运行的 bash 跟 devcloud 主机上的 rootfs 隔离了。<br />![2021-05-03-11-23-53-657782.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620012426585-99e91812-cafd-4c50-93ba-763bf89c3baf.jpeg#clientId=ucf95d1f1-da39-4&from=ui&id=ub0ea2d04&originHeight=174&originWidth=649&originalType=binary&ratio=1&rotation=0&showTitle=false&size=23081&status=done&style=shadow&taskId=u0d4d0f2e-a378-419f-a83c-3c2579d3b5c&title=)
<a name="rWf7B"></a>
### 2.1 Namespace
简单来说 namespace 是由 Linux 内核提供的，用于进程间资源隔离的一种技术，使得 a,b 进程可以看到 S 资源；而 c 进程看不到。它是在 2002 年 Linux 2.4.19 开始加入内核的特性，到 2013 年 Linux 3.8 中 user namespace 的引入，对于现在所熟知的容器所需的全部 namespace 就都实现了。<br />Linux 提供了多种 namespace，用于对多种不同资源进行隔离。容器的实质是进程，但与直接在宿主机执行的进程不同，容器进程运行在属于自己的独立的命名空间。因此容器可以拥有自己的 root 文件系统、自己的网络配置、自己的进程空间，甚至自己的用户 ID 空间。<br />还是来看一个简单的例子，有个感性认识，namespace 到底是啥，在哪里能直观的看到。在 devcloud 云主机上，执行：ls-l /proc/self/ns  看到的就是当前系统所支持的 namespace。<br />![2021-05-03-11-23-53-877364.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620012440080-298a3a8d-39f3-4fed-9b2c-412a25a4aef2.jpeg#clientId=ucf95d1f1-da39-4&from=ui&id=uf6a6708d&originHeight=179&originWidth=1012&originalType=binary&ratio=1&rotation=0&showTitle=false&size=46868&status=done&style=none&taskId=u55cb55bb-08ce-4356-b750-0799ed951f0&title=)<br />接着使用 unshare 命令，运行一个 bash，让它不使用当前的 pid namespace：
```bash
unshare --pid --fork --mount-proc bash
```
然后运行: `ps -a`看看当前 pid namespace 下的进程都有哪些：<br />![2021-05-03-11-23-54-052946.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620012464365-64a95eb1-272a-4d40-82df-96ffa2a03dbd.jpeg#clientId=ucf95d1f1-da39-4&from=ui&id=u07ff9167&originHeight=80&originWidth=515&originalType=binary&ratio=1&rotation=0&showTitle=false&size=8031&status=done&style=none&taskId=u8e80b249-494e-4671-9893-fc1c11c7f86&title=)<br />在新起的 bash 上执行：ls -l /proc/self/ns，发现当前 bash 的 pid namespace 与之前是不相同的。<br />![2021-05-03-11-23-54-270869.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620012477564-94cf3180-5f4f-4eef-a1d0-07600fb4a0f1.jpeg#clientId=ucf95d1f1-da39-4&from=ui&id=u4509692a&originHeight=175&originWidth=990&originalType=binary&ratio=1&rotation=0&showTitle=false&size=52138&status=done&style=none&taskId=u08e152a8-2b91-4406-8b38-9f8dcbb94f0&title=)<br />既然 docker 就是基于内核的 namespace 特性来实现的，那么可以简单来认证一下，执行指令：
```bash
 docker run –pid host --rm -it alpine sh
```
运行一个简单的 alpine 容器，让它与主机共用同一个 pid namespace。然后在容器内部执行指令 ps -a 会发现进程数量与 devcloud 机器上的一样；执行指令ls -l /proc/self/ns/同样会看到容器内部的 pid namespace 与 devcloud 机器上的也是一样。
<a name="O8fyz"></a>
### 2.2 cgroups
cgroups 是 namespace 的一种，是为了实现虚拟化而采取的资源管理机制，决定哪些分配给容器的资源可被管理，分配容器使用资源的多少。容器内的进程是运行在一个隔离的环境里，使用起来，就好像是在一个独立于宿主的系统下操作一样。这种特性使得容器封装的应用比直接在宿主运行更加安全。例如可以设定一个 memory 使用上限，一旦进程组（容器）使用的内存达到限额再申请内存，就会出发 OOM（out of memory），这样就不会因为某个进程消耗的内存过大而影响到其他进程的运行。<br />还是来看个例子感受一下。在 devcloud 机器上运行一个 apline 容器，限制只能使用前 2 个 CPU 且只能使用 1.5 个核：
```bash
docker run --rm -it --cpus "1.5" --cpuset-cpus 0,1 alpine
```
然后再开启一个新的终端，先看看系统上有哪些资源是可以控制的：
```bash
cat /proc/cgroups
```
![2021-05-03-11-23-54-478309.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620012512589-f9cc485e-0cec-4371-8cef-5c1b39faa1c5.jpeg#clientId=ucf95d1f1-da39-4&from=ui&id=ufb01ed53&originHeight=317&originWidth=802&originalType=binary&ratio=1&rotation=0&showTitle=false&size=26830&status=done&style=none&taskId=u0ec8af5d-25b9-4133-a9f8-bc8411e085c&title=)<br />最左边一侧就是可以设置的资源了。接着需要找到这些控制资源分配的信息都放在哪个目录下：
```bash
mount | grep cgroup
```
![2021-05-03-11-23-54-793249.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620012531326-a1b5046a-c9be-42bb-817b-73f1d215442a.jpeg#clientId=ucf95d1f1-da39-4&from=ui&id=uac40a508&originHeight=233&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=79672&status=done&style=none&taskId=u3b4fe5bb-fc5c-4532-8a9b-0e8487dd8b8&title=)<br />然后找到刚刚运行的 alpine 镜像的 cgroups 配置：
```bash
cat /proc/`docker inspect --format='{{.State.Pid}}' $(docker ps -ql)`/cgroup
```
![2021-05-03-11-23-55-081512.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620012550283-f6039dcc-4b43-4776-8ffc-ad2d3da06d41.jpeg#clientId=ucf95d1f1-da39-4&from=ui&id=uf537ac3d&originHeight=242&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=93801&status=done&style=none&taskId=u98ed0ef4-6479-4101-b7dd-d082c1e3bd2&title=)<br />这样，把二者拼接起来，就可以看到这个容器的资源配置了。先来验证 cpu 的用量是否是 1.5 个核：
```bash
cat /sys/fs/cgroup/cpu,cpuacct/docker/c1f68e86241f9babb84a9556dfce84ec01e447bf1b8f918520de06656fa50ab4/cpu.cfs_period_us
```
输出 100000，可以认为是单位，然后再看配额：
```bash
cat /sys/fs/cgroup/cpu,cpuacct/docker/c1f68e86241f9babb84a9556dfce84ec01e447bf1b8f918520de06656fa50ab4/cpu.cfs_quota_us
```
输出 150000，与单位相除正好是设置的 1.5 个核，接着验证是否使用的是前两个核心：
```bash
cat /sys/fs/cgroup/cpuset/docker/c1f68e86241f9babb84a9556dfce84ec01e447bf1b8f918520de06656fa50ab4/cpuset.cpus
```
输出 0-1。<br />目前来看，容器的资源配置都是按照设定的来分配的，但实际真能在 CPU0-CPU1 上限制使用 1.5 个核吗？先看一下当前 CPU 的用量：
```bash
docker stats $(docker ps -ql)
```
![2021-05-03-11-23-55-197810.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620012598170-e9575383-6a77-4e85-aec5-7fc19628db44.jpeg#clientId=ucf95d1f1-da39-4&from=ui&id=u9a9a1bb4&originHeight=93&originWidth=614&originalType=binary&ratio=1&rotation=0&showTitle=false&size=7869&status=done&style=none&taskId=ubb5d053d-1eb7-4b87-be92-ce5868fe1da&title=)<br />因为没有在 alpine 中运行程序，所以 CPU 用量为 0，现在回到最开始执行 docker 指令的 alpine 终端，执行一个死循环：
```bash
i=0; while true; do i=i+i; done
```
再来观察当前的 CPU 用量：<br />![2021-05-03-11-23-55-386559.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620012623871-fcfd0415-b6ac-4469-9f6c-bd905015bcab.jpeg#clientId=ucf95d1f1-da39-4&from=ui&id=ua8643771&originHeight=93&originWidth=623&originalType=binary&ratio=1&rotation=0&showTitle=false&size=9597&status=done&style=none&taskId=u9245a186-ae76-455e-abb9-25b2f9404b3&title=)<br />接近 1，但为啥不是 1.5？因为刚刚运行的死循环只能跑在一个核上，所以再打开一个终端，进入到 alpine 镜像中，同样执行死循环的指令，看到 CPU 用量稳定在了 1.5，说明资源的使用量确实是限制住了的。<br />![2021-05-03-11-23-55-500253.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620012658893-f6e0e249-1686-48fc-b803-d620de7099be.jpeg#clientId=ucf95d1f1-da39-4&from=ui&id=u897d80b7&originHeight=107&originWidth=650&originalType=binary&ratio=1&rotation=0&showTitle=false&size=7965&status=done&style=none&taskId=ud0b3d3b2-d5a3-4416-98b7-d540e70782a&title=)<br />现在对 docker 容器实现了进程间资源隔离的黑科技有了一定认识。如果单单就隔离性来说，vagrant 也已经做到了。那么为什么是 docker 火爆全球？是因为它允许用户将容器环境打包成为一个镜像进行分发，而且镜像是分层增量构建的，这可以大大降低用户使用的门槛。
<a name="mpm5J"></a>
## 3、存储
Image 是 Docker 部署的基本单位，它包含了程序文件，以及这个程序依赖的资源的环境。Docker Image 是以一个 mount 点挂载到容器内部的。容器可以近似理解为镜像的运行时实例，默认情况下也算是在镜像层的基础上增加了一个可写层。所以，一般情况下如果在容器内做出的修改，均包含在这个可写层中。
<a name="eAvjG"></a>
### 3.1 联合文件系统（UFS）
Union File System 从字面意思上来理解就是“联合文件系统”。它将多个物理位置不同的文件目录联合起来，挂载到某一个目录下，形成一个抽象的文件系统。<br />![2021-05-03-11-23-55-707449.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620012688012-2efec2b1-3b08-4844-ac0b-26e738e1bae7.jpeg#clientId=ucf95d1f1-da39-4&from=ui&id=u9a4136fd&originHeight=193&originWidth=754&originalType=binary&ratio=1&rotation=0&showTitle=false&size=32229&status=done&style=shadow&taskId=u6144ea7c-2953-4125-8a78-d163c32e26b&title=)<br />如上图，从右侧以 UFS 的视角来看，lowerdir 和 upperdir 是两个不同的目录，UFS 将二者合并起来，得到 merged 层展示给调用方。从左侧的 docker 角度来理解，lowerdir 就是镜像，upperdir 就相当于是容器默认的可写层。在运行的容器中修改了文件，可以使用 docker commit 指令保存成为一个新镜像。
<a name="aPAay"></a>
### 3.2 Docker 镜像的存储管理
有了 UFS 的分层概念，就很好理解这样的一个简单 Dockerfile：
```dockerfile
FROM alpine
COPY foo /foo
COPY bar /bar
```
在构建时的输出所代表的含义了。<br />![2021-05-03-11-23-55-915333.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620012712969-ec04228d-3714-4313-86d1-fe4f7759c01e.jpeg#clientId=ucf95d1f1-da39-4&from=ui&id=u3715e46e&originHeight=180&originWidth=488&originalType=binary&ratio=1&rotation=0&showTitle=false&size=14891&status=done&style=shadow&taskId=ue57da63f-b6c9-4545-afe6-8a3decab615&title=)<br />但是使用 `docker pull` 拉取的镜像文件，在本地机器上存储在哪，又是如何管理的呢？还是来实际操作认证一下。在 devcloud 上确认当前 docker 所使用的存储驱动（默认是 overlay2）：
```bash
docker info --format '{{.Driver}}'
```
以及镜像下载后的存储路径（默认存储在/var/lib/docker）：
```bash
docker info --format '{{.DockerRootDir}}'
```
当前的 docker 修改了默认存储路径，配置到/data/docker-data，就以它为例进行展示。先查看一下该目录下的结构：
```bash
tree -L 1 /data/docker-data
```
![2021-05-03-11-23-56-093948.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620012754742-66e88f70-db28-498e-b42f-31c8af8a8d59.jpeg#clientId=ucf95d1f1-da39-4&from=ui&id=ub1274af2&originHeight=333&originWidth=564&originalType=binary&ratio=1&rotation=0&showTitle=false&size=16314&status=done&style=none&taskId=u5ec0f06e-b5ec-4777-a020-4fc489c507d&title=)<br />关注一下其中的 image 和 overlay2 目录。前者就是存放镜像信息的地方，后者则是存放具体每一分层的文件内容。先深入看一下 image 目录结构：
```bash
tree -L 2 /data/docker-data/image/
```
![2021-05-03-11-23-56-238187.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620012771122-69914537-a9e4-4185-bd29-520a938a4f38.jpeg#clientId=ucf95d1f1-da39-4&from=ui&id=u031f5ea6&originHeight=197&originWidth=503&originalType=binary&ratio=1&rotation=0&showTitle=false&size=13841&status=done&style=none&taskId=ue4ce92a7-8336-4522-bbac-b585ddd1b85&title=)<br />留心这个 imagedb 目录，接下来以以最新的 alpine 镜像为例子，看看 docker 是如何管理镜像的。执行指令：
```bash
docker pull alpine:latest
```
![2021-05-03-11-23-56-410714.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620012789975-6b51c630-fd2a-4de3-bc67-29a6cad17e99.jpeg#clientId=ucf95d1f1-da39-4&from=ui&id=uf04b2e4f&originHeight=102&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=20890&status=done&style=none&taskId=u7d3228e5-ef5a-4f1f-a233-4cb39e4e5c1&title=)<br />紧接着查看它的镜像 ID：
```bash
docker image ls alpine:latest
```
![2021-05-03-11-23-56-727825.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620012808287-6a5e22fc-5fee-4db2-82fe-62750d7a9f4d.jpeg#clientId=ucf95d1f1-da39-4&from=ui&id=ua665de1b&originHeight=54&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=9747&status=done&style=none&taskId=u5b058899-e459-4094-9ae0-72a00886faa&title=)<br />记住这个 ID a24bb4013296，现在可以看一下 imagedb 目录下的变化：
```bash
tree -L 2 /data/docker-data/image/overlay2/imagedb/content/ | grepa24bb4013296
```
![2021-05-03-11-23-56-925532.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620012846392-bebcae45-8de5-43dd-bc61-a2c042811f38.jpeg#clientId=ucf95d1f1-da39-4&from=ui&id=u455f038e&originHeight=31&originWidth=959&originalType=binary&ratio=1&rotation=0&showTitle=false&size=10565&status=done&style=none&taskId=ub524d676-fa56-4449-8514-589e690c00a&title=)<br />多了这么一个镜像 ID 的文件，它是一个 json 格式的文件，这里包含了该镜像的参数信息：
```bash
jq .
/data/docker-data/image/overlay2/imagedb/content/sha256/a24bb4013296f61e89ba57005a7b3e52274d8edd3ae2077d04395f806b63d83e
```
![2021-05-03-11-23-57-141280.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620012872144-0c651fe9-4135-4b22-99ab-6479fa1e38a9.jpeg#clientId=ucf95d1f1-da39-4&from=ui&id=uff57cb21&originHeight=594&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=48348&status=done&style=none&taskId=ub88481ce-ebf6-425b-80ef-3b5582db4d1&title=)<br />接下来看看将一个镜像运行起来之后会有什么变化。运行一个 alpine 容器，让它 sleep10 分钟：
```bash
docker run --rm -d alpine sleep 600
```
然后找到它的 overlay 挂载点：
```bash
docker inspect --format='{{.GraphDriver.Data}}' $(docker ps -ql) | grep MergedDir
```
![2021-05-03-11-23-57-438901.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620012901086-46f1f33b-284e-4bd1-a98f-9ea118b38c3a.jpeg#clientId=ucf95d1f1-da39-4&from=ui&id=u464f7807&originHeight=126&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=49589&status=done&style=none&taskId=u9ce53ef3-f620-43b7-9117-425c92071fa&title=)<br />结合上一节讲到的 UFS 文件系统，可以 ls 一下：
```bash
ls /data/docker-data/overlay2/74e92699164736980c9e20475388568f482671625a177cb946c4b136e4d94a64/merged
```
![2021-05-03-11-23-57-730293.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620012994997-eaaaa88e-75ff-42ef-97bc-4af4aee6a74c.jpeg#clientId=ucf95d1f1-da39-4&from=ui&id=u680a130e&originHeight=29&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=7093&status=done&style=none&taskId=ub8867a93-eb9d-4433-8c46-07bbd21ac80&title=)<br />看到它就是合并后所呈现在 alpine 容器的文件系统。先进入到容器内：
```bash
docker exec -it $(docker ps -ql) sh
```
紧接着新开一个终端查看容器运行起来后跟镜像相比，有哪些修改：
```bash
docker diff $(docker ps -ql)
```
![2021-05-03-11-23-57-863985.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620013022611-4e8076dd-fc29-4b22-8aae-80ccdc2df04c.jpeg#clientId=ucf95d1f1-da39-4&from=ui&id=u0e11b3c9&originHeight=55&originWidth=577&originalType=binary&ratio=1&rotation=0&showTitle=false&size=4142&status=done&style=none&taskId=u40d434a4-e1ba-4582-a5ef-1607264fba2&title=)<br />在/root 目录下，增加了 sh 的历史记录文件。然后在容器中手动增加一个 hello.txt 文件：
```bash
echo 'Hello Docker' > hello.txt
```
![2021-05-03-11-23-58-049555.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620013043742-8b31af81-70cf-4e7d-9c38-df48d7966547.jpeg#clientId=ucf95d1f1-da39-4&from=ui&id=u396e3411&originHeight=81&originWidth=855&originalType=binary&ratio=1&rotation=0&showTitle=false&size=8103&status=done&style=none&taskId=u7502a196-67ae-4438-bf02-02642c441bf&title=)<br />这时候来看看容器默认在镜像之上增加的可写层 UpperDir 目录的变化：
```bash
ls /data/docker-data/overlay2/74e92699164736980c9e20475388568f482671625a177cb946c4b136e4d94a64/diff
```
![2021-05-03-11-23-58-168320.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620013072825-272dff2d-8f6b-45d4-a5e5-102c053aa3db.jpeg#clientId=ucf95d1f1-da39-4&from=ui&id=u2b7eaf91&originHeight=33&originWidth=377&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2995&status=done&style=none&taskId=u57d41976-e40c-4566-8352-c7023f1ae01&title=)<br />这就认证了 overlay2 驱动是将镜像和可写层的内容 merged 之后，供容器作为文件系统使用。多个运行的容器共用一份基础镜像，而各自有独立的可写层，节省了存储空间。<br />这个时候，也可以回答一下镜像的实际内容是存储在哪里：
```bash
cat /data/docker-data/overlay2/74e92699164736980c9e20475388568f482671625a177cb946c4b136e4d94a64/lower
```
![2021-05-03-11-23-58-377710.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620013091124-177df827-b632-4157-9ca8-d12af25cdc0d.jpeg#clientId=ucf95d1f1-da39-4&from=ui&id=u6380eda5&originHeight=37&originWidth=746&originalType=binary&ratio=1&rotation=0&showTitle=false&size=7141&status=done&style=none&taskId=u517bd566-1fde-4e6c-9f67-88bd42f2608&title=)<br />查看这些分层：
```bash
ls /data/docker-data/overlay2/l/ZIIZFSQUQ4CIKRNCMOXXY4VZHY/
```
![2021-05-03-11-23-58-581413.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620013106803-05c07f0d-3607-4aad-992d-dcfe2e13bb04.png#clientId=ucf95d1f1-da39-4&from=ui&id=uaedc3a2e&originHeight=32&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=5120&status=done&style=none&taskId=u986f2cfd-94e6-4360-a3e2-a7feab7401c&title=)<br />就是 UFS 中低层的镜像内容。
<a name="hA6Gg"></a>
## 总结
Docker 使用的底层技术包括 namespace，cgroups 和 overlay2 联合文件系统，着重介绍了隔离环境是如何在宿主机上演进实现的。通过实际手动操作，对这些概念有了真实的感受。
