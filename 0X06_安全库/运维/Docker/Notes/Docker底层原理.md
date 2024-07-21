Docker<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638274878225-9cc34c7b-0209-42fd-a82c-dd5498e7e303.webp#clientId=u619c952f-4bae-4&from=paste&id=u6dbbffeb&originHeight=572&originWidth=1038&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5642d506-2cd3-4f2c-8f86-649cdd50053&title=)<br />Linux 的内核，提供了多达 8 种类型的 Namespace。在这些独立的 Namespace 中，资源互不影响，隔离措施做的非常好。
<a name="ljK25"></a>
## 8 种类型
先来看一下，Linux 都支持哪些 Namespace。可以通过 unshare 命令来观察到这些细节。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638274878186-810c3eef-2baa-4126-a5d5-c872060e27ee.webp#clientId=u619c952f-4bae-4&from=paste&id=u8e80b0d4&originHeight=760&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub2a6976e-7e8c-4794-8cf9-6a496974667&title=)<br />在终端执行 man unshare，将会出现这些 Namespace 的介绍:

- Mount（mnt）：隔离挂载点
- Process ID (pid)：隔离进程 ID
- Network (net)：隔离网络设备，端口号等
- Interprocess Communication (ipc)：隔离 System V IPC 和 POSIX message queues
- UTS Namespace(uts)：隔离主机名和域名
- User Namespace (user)：隔离用户和用户组

另外，Linux 在 4.6 版本，5.6 版本，分别加入了 cgroups 和 Time 两种隔离类型，加起来就有 8 种。<br />Control group (cgroup) Namespace 隔离 Cgroups 根目录 (4.6 版本加入)；Time Namespace 隔离系统时间 (5.6 版本加入)。
<a name="VnuLT"></a>
## 1 个例子
通过 unshare 命令，可以快速建立一些隔离的例子，拿最简单直观的 pid namespace 来看一下它的效果。<br />众所周知，Linux 进程号为 1 的，叫做 systemd 进程。但在 Docker 中，通过执行 `ps` 命令，却只能看到非常少的进程列表。<br />执行下面的命令，进入隔离环境，并将 bash 作为根进程：
```bash
unshare --pid --fork --mount-proc /bin/bash
```
效果如图所示：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638274878369-2b25f328-0934-4e7a-b887-44cd9f07c7cb.webp#clientId=u619c952f-4bae-4&from=paste&id=ue8cf02e5&originHeight=361&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8ed63331-a3a3-4812-9b80-9c62756064b&title=)<br />可以看到，我们的 bash，已经成为了 1 号进程，而宿主机和其他隔离环境的进程信息，在这里是不可见的<br />先在隔离环境中，执行 sleep 1000。再开一个终端，在宿主机上执行 pstree，将会看到这个隔离环境的进行信息。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638274878177-f8714ddd-3753-4f16-ae51-94f37655931f.webp#clientId=u619c952f-4bae-4&from=paste&id=ua2f89f69&originHeight=342&originWidth=891&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u80a311b0-4c74-4a57-90b5-3c847619679&title=)<br />接下来，在宿主机上，把 sleep 对应进程的命名空间信息，和宿主机的命名空间信息作一下对比。<br />可以看到，它们的 pid namespace，对应的数值是不同的。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638274878400-d31b7a3c-e28c-438a-b94d-f2c3f3a43d83.webp#clientId=u619c952f-4bae-4&from=paste&id=uf24b1ca3&originHeight=531&originWidth=923&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5232eb7f-a60f-43cd-b51b-2e295a10951&title=)<br />下面给出其他 namespace 的实验性命令，可以实际操作一下。
<a name="EnmoD"></a>
## 试验一下
```bash
unshare --mount --fork /bin/bash
```
创建 mount namespace，并在每个不同的环境中，使用不同的挂载目录。
```bash
unshare --uts --fork /bin/bash
```
uts 可以用来隔离主机名称，允许每个 namespace 拥有一个独立的主机名，可以通过 hostname 命令进行修改。
```bash
unshare --ipc --fork /bin/bash
```
IPC Namespace 主要是用来隔离进程间通信的。Linux 的进程间通信，有管道、信号、报文、共享内存、信号量、套接口等方式。<br />使用了 IPC 命名空间，意味着跨 Namespace 的这些通信方式将全部失效！不过，这也正是希望看到的。
```bash
unshare --user -r /bin/bash
```
用户命名空间，就非常好理解了。可以在一个 Namespace 中建立 xjjdog 账号，也可以在另外一个 Namespace 中建立 xjjdog 账号，而且它们是相互不影响的。
```bash
unshare --net --fork /bin/bash
```
`net namespace`，这个就非常有用了。它可以用来隔离网络设备、IP 地址和端口等信息。
<a name="iKXCp"></a>
## 结语
可以看到，通过各种 Namespace，Linux 能够对各种资源进行精细化的隔离。<br />Docker 本身也是一个新瓶装旧酒的玩具。Docker 的创新之处，在于它加入了一个中央仓库，并封装了很多易用的命令。<br />到目前为止，并没有对 CPU 和内存的资源使用进行隔离，也没有对应的 Namespace 来解决这些问题。<br />资源限制的功能，是使用 Cgroups 进行限额配置来完成的，和 Namespace 没什么关系。<br />最后，附上 Docker 的一张生命周期图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638274878712-f27432db-ce25-4271-8609-4f479a7bd3b1.webp#clientId=u619c952f-4bae-4&from=paste&id=u8838c43f&originHeight=550&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u911b618d-81e7-4f5e-aad4-a016e2e7b90&title=)<br />Docker 发展到现在，应用工具链已经非常成熟了，很多同学已经驾轻就熟，如果对容器技术非常感兴趣，不如多看一下最底层的原理。这样，不管是谷歌推自己的容器，还是继续使用 Docker，都能快速把它掌握。
