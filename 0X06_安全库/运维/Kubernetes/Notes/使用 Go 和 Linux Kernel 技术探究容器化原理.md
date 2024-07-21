KubernetesGo<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680245515559-3ce08e1a-508d-4cf3-b03f-2afb437e9196.png#averageHue=%2334495f&clientId=u76f1f459-8ef3-4&from=paste&id=ubf56feba&originHeight=398&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4ea45af9-739c-40f9-ab75-37ae5ba1c58&title=)<br />传统模式的部署，直接将多个应用运行在物理服务器上，如果其中一个应用占用了大部分资源，可能会导致其他应用的性能下降。<br />虚拟化部署时代，可以在单个物理服务器的 CPU 上运行多个虚拟机（VM），每个 VM 是一台完整的计算机，在虚拟化硬件之上运行所有组件（包括了操作系统）。因此，可以让不同的应用在 VM 之间安全地隔离运行，更好地利用物理服务器上的资源。<br />容器与 VM 类似，具有自己的文件系统、CPU、内存、进程空间等，但与 VM 不同的是，容器之间共享操作系统（OS）。 所以，容器被认为是一种轻量级的操作系统层面的虚拟化技术。<br />相比于 VM ，轻量级的容器更适合云原生模式的实践。
<a name="tQncq"></a>
## 容器的本质
![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1680245515507-f237f6fb-a4dd-4649-b7ce-0229011180c7.jpeg#averageHue=%23f2eee8&clientId=u76f1f459-8ef3-4&from=paste&id=ue3913c3c&originHeight=392&originWidth=640&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf5d515e0-0dd1-479c-8284-b90dcf619c8&title=)<br />容器是一种轻量级的操作系统层面的虚拟化技术。<br />重点是 “操作系统层面” ，即容器本质上是利用操作系统提供的功能来实现虚拟化。<br />容器技术的代表之作 Docker ，则是一个基于 Linux 操作系统，使用 Go 语言编写，调用了 Linux Kernel 功能的虚拟化工具。<br />为了更好地理解容器的本质，来看看容器具体使用了哪些 Linux Kernel 技术，以及在 Go 中应该如何去调用。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680245515550-d904bf7a-52b3-4345-a1ca-58f95768207a.png#averageHue=%23e9ebf0&clientId=u76f1f459-8ef3-4&from=paste&id=u6acfaa2c&originHeight=442&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7dee50d5-1a11-40d7-ad1a-2692b76756d&title=)
<a name="IHq26"></a>
### 1、NameSpace
NameSpace 即命名空间是 Linux Kernel 一个强大的特性，可用于进程间资源隔离。<br />由于容器之间共享 OS ，对于操作系统而言，容器的实质就是进程，多个容器运行，对应操作系统也就是运行着多个进程。<br />当进程运行在自己单独的命名空间时，命名空间的资源隔离可以保证进程之间互不影响，大家都以为自己身处在独立的一个操作系统里。这种进程就可以称为容器。<br />回到资源隔离上，从 Kernel: 5.6 版本开始，已经提供了 8 种 NameSpace ，这 8 种 NameSpace 可以对应地隔离不同的资源（ Docker 主要使用了前 6 种）。

| 命名空间 | 系统调用参数 | 作用 |
| --- | --- | --- |
| Mount (mnt) | CLONE_NEWNS | 文件目录挂载隔离。用于隔离各个进程看到的挂载点视图 |
| Process ID (pid) | CLONE_NEWPID | 进程 ID 隔离。使每个命名空间都有自己的初始化进程，PID 为 1，作为所有进程的父进程 |
| Network (net) | CLONE_NEWNET | 网络隔离。使每个 net 命名空间有独立的网络设备，IP 地址，路由表，/proc/net 目录等网络资源 |
| Interprocess Communication (ipc) | CLONE_NEWIPC | 进程 IPC 通信隔离。让只有相同 IPC 命名空间的进程之间才可以共享内存、信号量、消息队列通信 |
| UTS | CLONE_NEWUTS | 主机名或域名隔离。使其在网络上可以被视作一个独立的节点而非主机上的一个进程 |
| User ID (user) | CLONE_NEWUSER | 用户 UID 和组 GID 隔离。例如每个命名空间都可以有自己的 root 用户 |
| Control group (cgroup) Namespace | CLONE_NEWCGROUP | Cgroup 信息隔离。用于隐藏进程所属的控制组的身份，使命名空间中的 cgroup 视图始终以根形式来呈现，保障安全 |
| Time Namespace | CLONE_NEWTIME | 系统时间隔离。允许不同进程查看到不同的系统时间 |

NameSpace 的具体描述可以查看 [**Linux man 手册中的 NAMESPACES**](https://man7.org/linux/man-pages/man7/namespaces.7.html) 章节，手册中还描述了几个 NameSpace API ，主要是和进程相关的系统调用函数。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680245515572-b82c01c1-13f4-4367-bfec-b132e9e819ab.png#averageHue=%23eaedf3&clientId=u76f1f459-8ef3-4&from=paste&id=udb3ad7df&originHeight=470&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua351397a-a3c0-4757-8984-608db7c99ee&title=)
<a name="vwtHB"></a>
#### `clone()`
```c
int clone(int (*fn)(void *), void *stack, int flags, void *arg, ...
          /* pid_t *parent_tid, void *tls, pid_t *child_tid */ );
```
`clone()` 用于创建新进程，通过传入一个或多个系统调用参数（ flags 参数）可以创建出不同类型的 NameSpace ，并且子进程也将会成为这些 NameSpace 的成员。
<a name="IBQdG"></a>
#### `setns()`
```c
int setns(int fd, int nstype);
```
`setns()` 用于将进程加入到一个现有的 Namespace 中。其中 fd 为文件描述符，引用 /proc/[pid]/ns/ 目录里对应的文件，nstype 代表 NameSpace 类型。
<a name="VkXk3"></a>
#### `unshare()`
```c
int unshare(int flags);
```
`unshare()` 用于将进程移出原本的 NameSpace ，并加入到新创建的 NameSpace 中。同样是通过传入一个或多个系统调用参数（ flags 参数）来创建新的 NameSpace 。
<a name="MpetX"></a>
#### `ioctl()`
```c
int ioctl(int fd, unsigned long request, ...);
```
`ioctl()` 用于发现有关 NameSpace 的信息。<br />上面的这些系统调用函数，可以直接用 C 语言调用，创建出各种类型的 NameSpace ，这是最直观的做法。而对于 Go 语言，其内部已经帮封装好了这些函数操作，可以更方便地直接使用，降低心智负担。<br />先来看一个简单的小工具（[**源自 Containers From Scratch • Liz Rice • GOTO 2018**](https://www.youtube.com/watch?v=8fi7uSYlOdc)）：
```go
package main

import (
    "os"
    "os/exec"
)

func main() {
    switch os.Args[1] {
        case "run":
        run()
        default:
        panic("help")
    }
}

func run() {
    cmd := exec.Command(os.Args[2], os.Args[3:]...)
    cmd.Stdin = os.Stdin
    cmd.Stdout = os.Stdout
    cmd.Stderr = os.Stderr
    must(cmd.Run())
}

func must(err error) {
    if err != nil {
        panic(err)
    }
}
```
这个程序接收用户命令行传递的参数，并使用 `exec.Command` 运行，例如当执行 `go run main.go run echo hello` 时，会创建出 main 进程， main 进程内执行 echo hello 命令创建出一个新的 echo 进程，最后随着 echo 进程的执行完毕，main 进程也随之结束并退出。
```bash
[root@host go]# go run main.go run echo hello
hello
[root@host go]#
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680245515595-1248dc22-a88a-4206-9c18-3a7e98b9b728.png#averageHue=%23ebeff4&clientId=u76f1f459-8ef3-4&from=paste&id=u00e0a61e&originHeight=688&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u28a4069d-4c96-49c0-8405-6713c3c1d3e&title=)<br />但是上面创建的进程太快退出了，不便于观察。如果让 main 进程启动一个 bash 进程会怎样呢？<br />为了直观对比，先看看当前会话的进程信息。
```bash
[root@host go]# ps
  PID TTY          TIME CMD
 1115 pts/0    00:00:00 bash
 1205 pts/0    00:00:00 ps
[root@host go]# echo $$
1115
[root@host go]#
```
当前正处于 PID 1115 的 bash 会话进程中，继续下一步操作：
```bash
[root@host go]# go run main.go run /bin/bash
[root@host go]# ps
  PID TTY          TIME CMD
 1115 pts/0    00:00:00 bash
 1207 pts/0    00:00:00 go
 1225 pts/0    00:00:00 main
 1228 pts/0    00:00:00 bash
 1240 pts/0    00:00:00 ps
[root@host go]# echo $$
1228
[root@host go]# exit
exit
[root@host go]# ps
  PID TTY          TIME CMD
 1115 pts/0    00:00:00 bash
 1241 pts/0    00:00:00 ps
[root@host go]# echo $$
1115
[root@host go]#
```
在执行 `go run main.go run /bin/bash` 后，会话被切换到了 PID 1228 的 bash 进程中，而 main 进程也还在运行着（当前所处的 bash 进程是 main 进程的子进程，main 进程必须存活着，才能维持 bash 进程的运行）。当执行 exit 退出当前所处的 bash 进程后，main 进程随之结束，并回到原始的 PID 1115 的 bash 会话进程。<br />容器的实质是进程，你现在可以把 main 进程当作是 “Docker” 工具，把 main 进程启动的 bash 进程，当作一个 “容器” 。这里的 “Docker” 创建并启动了一个 “容器”。<br />为什么打了双引号，是因为在这个 bash 进程中，可以随意使用操作系统的资源，并没有做资源隔离。<br />要想实现资源隔离，也很简单，在 `run()` 函数增加 `SysProcAttr` 配置，先从最简单的 UTS 隔离开始，传入对应的 CLONE_NEWUTS 系统调用参数，并通过 `syscall.Sethostname` 设置主机名：
```go
func run() {
    cmd := exec.Command(os.Args[2], os.Args[3:]...)
    cmd.Stdin = os.Stdin
    cmd.Stdout = os.Stdout
    cmd.Stderr = os.Stderr
    cmd.SysProcAttr = &syscall.SysProcAttr{
        Cloneflags: syscall.CLONE_NEWUTS,
    }
    must(syscall.Sethostname([]byte("mycontainer")))
    must(cmd.Run())
}
```
这段代码看似没什么问题，但仔细思考一下。<br />`syscall.Sethostname` 这一行到底是哪个进程在执行？main 进程还是 main 进程创建的子进程？<br />不用想，子进程都还没 Run 起来呢！现在调用肯定是 main 进程在执行，main 进程可没进行资源隔离，相当于直接更改宿主机的主机名了。<br />子进程还没 Run 起来，还不能更改主机名，等子进程 Run 起来后，又会进入到阻塞状态，无法再通过代码方式更改到子进程内的主机名。那有什么办法呢？<br />看来只能把 /proc/self/exe 这个神器请出来了。<br />在 Linux 2.2 内核版本及其之后，/proc/[pid]/exe 是对应 pid 进程的二进制文件的符号链接，包含着被执行命令的实际路径名。如果打开这个文件就相当于打开了对应的二进制文件，甚至可以通过重新输入 /proc/[pid]/exe 重新运行一个对应于 pid 的二进制文件的进程。<br />对于 /proc/self ，当进程访问这个神奇的符号链接时，可以解析到进程自己的 /proc/[pid] 目录。<br />合起来就是，当进程访问 /proc/self/exe 时，可以运行一个对应进程自身的二进制文件。<br />这有什么用呢？继续看下面的代码：
```go
package main

import (
    "os"
    "os/exec"
    "syscall"
)

func main() {
    switch os.Args[1] {
        case "run":
        run()
        case "child":
        child()
        default:
        panic("help")
    }
}

func run() {
    cmd := exec.Command("/proc/self/exe", append([]string{"child"}, os.Args[2:]...)...)
    cmd.Stdin = os.Stdin
    cmd.Stdout = os.Stdout
    cmd.Stderr = os.Stderr
    cmd.SysProcAttr = &syscall.SysProcAttr{
        Cloneflags: syscall.CLONE_NEWUTS,
    }
    must(cmd.Run())
}

func child() {
    must(syscall.Sethostname([]byte("mycontainer")))
    cmd := exec.Command(os.Args[2], os.Args[3:]...)
    cmd.Stdin = os.Stdin
    cmd.Stdout = os.Stdout
    cmd.Stderr = os.Stderr
    must(cmd.Run())
}

func must(err error) {
    if err != nil {
        panic(err)
    }
}
```
在 `run()` 函数中，不再是直接运行用户所传递的命令行参数，而是运行 /proc/self/exe ，并传入 child 参数和用户传递的命令行参数。<br />同样当执行 `go run main.go run echo hello` 时，会创建出 main 进程， main 进程内执行 /proc/self/exe child echo hello 命令创建出一个新的 exe 进程，关键也就是这个 exe 进程，已经为其配置了 CLONE_NEWUTS 系统调用参数进行 UTS 隔离。也就是说，exe 进程可以拥有和 main 进程不同的主机名，彼此互不干扰。<br />进程访问 /proc/self/exe 代表着运行对应进程自身的二进制文件。因此，按照 exe 进程的启动参数，会执行 child() 函数，而 child() 函数内首先调用 `syscall.Sethostname` 更改了主机名（此时是 exe 进程执行的，并不会影响到 main 进程），接着和本文最开始的 `run()` 函数一样，再次使用 `exec.Command` 运行用户命令行传递的参数。<br />总结一下就是， main 进程创建了 exe 进程（exe 进程已经进行 UTS 隔离，exe 进程更改主机名不会影响到 main 进程）， 接着 exe 进程内执行 echo hello 命令创建出一个新的 echo 进程，最后随着 echo 进程的执行完毕，exe 进程随之结束，exe 进程结束后， main 进程再结束并退出。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680245731062-1e75aa0e-565a-41d3-b26a-2d733bd919d9.png#averageHue=%23e9eef4&clientId=u76f1f459-8ef3-4&from=paste&id=u05344701&originHeight=435&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u54a95b00-61ab-4494-91e9-3676a3586e3&title=)<br />那经过 exe 这个中间商所创建出来的 echo 进程和之前由 main 进程直接创建的 echo 进程，两者有何不同呢。<br />创建 exe 进程的同时传递了 CLONE_NEWUTS 标识符创建了一个 UTS NameSpace ，Go 内部封装了系统调用函数 `clone()` 的调用，由 `clone()` 函数创建出的进程的子进程也将会成为这些 NameSpace 的成员，所以默认情况下（创建新进程时无继续指定系统调用参数），由 exe 进程创建出的 echo 进程会继承 exe 进程的资源， echo 进程将拥有和 exe 进程相同的主机名，并且同样和 main 进程互不干扰。<br />因此，借助中间商 exe 进程 ，echo 进程可以成功实现和宿主机（ main 进程）资源隔离，拥有不同的主机名。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680245731149-cb7f87de-c7fb-4803-bb0a-89918f7bd8a3.png#averageHue=%23918774&clientId=u76f1f459-8ef3-4&from=paste&id=u3a2cdf7d&originHeight=407&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf8e45f52-8a01-4426-99c8-d0b6db4d3fb&title=)<br />再次通过启动 /bin/bash 进行验证主机名是否已经成功隔离：
```bash
[root@host go]# hostname
host
[root@host go]# go run main.go run /bin/bash
[root@mycontainer go]# hostname
mycontainer
[root@mycontainer go]# ps
  PID TTY          TIME CMD
 1115 pts/0    00:00:00 bash
 1250 pts/0    00:00:00 go
 1268 pts/0    00:00:00 main
 1271 pts/0    00:00:00 exe
 1275 pts/0    00:00:00 bash
 1287 pts/0    00:00:00 ps
[root@mycontainer go]# exit
exit
[root@host go]# hostname
host
[root@host go]#
```
当执行 `go run main.go run /bin/bash` 时，也可以在另一个 ssh 会话中，使用 `ps afx` 查看关于 PID 15243 的 bash 会话进程的层次信息：
```bash
[root@host ~]# ps afx
......
 1113 ?        Ss     0:00  \_ sshd: root@pts/0
 1115 pts/0    Ss     0:00  |   \_ -bash
 1250 pts/0    Sl     0:00  |       \_ go run main.go run /bin/bash
 1268 pts/0    Sl     0:00  |           \_ /tmp/go-build2476789953/b001/exe/main run /bin/bash
 1271 pts/0    Sl     0:00  |               \_ /proc/self/exe child /bin/bash
 1275 pts/0    S+     0:00  |                   \_ /bin/bash
......
```
以此类推，新增资源隔离只要继续传递指定的系统调用参数即可：
```go
package main

import (
    "fmt"
    "os"
    "os/exec"
    "syscall"
)

func main() {
    switch os.Args[1] {
        case "run":
        run()
        case "child":
        child()
        default:
        panic("help")
    }
}

func run() {
    fmt.Println("[main]", "pid:", os.Getpid())
    cmd := exec.Command("/proc/self/exe", append([]string{"child"}, os.Args[2:]...)...)
    cmd.Stdin = os.Stdin
    cmd.Stdout = os.Stdout
    cmd.Stderr = os.Stderr
    cmd.SysProcAttr = &syscall.SysProcAttr{
        Cloneflags: syscall.CLONE_NEWUTS |
        syscall.CLONE_NEWPID |
        syscall.CLONE_NEWNS,
        Unshareflags: syscall.CLONE_NEWNS,
    }
    must(cmd.Run())
}

func child() {
    fmt.Println("[exe]", "pid:", os.Getpid())
    must(syscall.Sethostname([]byte("mycontainer")))
    must(os.Chdir("/"))
    must(syscall.Mount("proc", "proc", "proc", 0, ""))
    cmd := exec.Command(os.Args[2], os.Args[3:]...)
    cmd.Stdin = os.Stdin
    cmd.Stdout = os.Stdout
    cmd.Stderr = os.Stderr
    must(cmd.Run())
    must(syscall.Unmount("proc", 0))
}

func must(err error) {
    if err != nil {
        panic(err)
    }
}
```
Cloneflags 参数新增了 CLONE_NEWPID 和 CLONE_NEWNS 分别隔离进程 pid 和文件目录挂载点视图，Unshareflags: `syscall.CLONE_NEWNS` 则是用于禁用挂载传播（如果不设置该参数，container 内的挂载会共享到 host ，挂载传播不在本文的探讨范围内）。<br />当创建 PID Namespace 时，exe 进程包括其创建出来的子进程的 pid 已经和 main 进程隔离了，这一点可以通过打印 os.Getpid() 结果或执行 `echo $$` 命令得到验证。但此时还不能使用 ps 命令查看，因为 ps 和 top 等命令会使用 /proc 的内容，所以才继续引入了 Mount Namespace ，并在 exe 进程挂载 /proc 目录。<br />Mount Namespace 是 Linux 第一个实现的 Namespace ，其系统调用参数是 CLONE_NEWNS ( New Namespace ) ，是因为当时并没意识到之后还会新增这么多的 Namespace 类型。
```bash
[root@host go]# ps
  PID TTY          TIME CMD
 1115 pts/0    00:00:00 bash
 3792 pts/0    00:00:00 ps
[root@host go]# echo $$
1115
[root@host go]# go run main.go run /bin/bash
[main] pid: 3811
[exe] pid: 1
[root@mycontainer /]# ps
  PID TTY          TIME CMD
    1 pts/0    00:00:00 exe
    4 pts/0    00:00:00 bash
   15 pts/0    00:00:00 ps
[root@mycontainer /]# echo $$
4
[root@mycontainer /]# exit
exit
[root@host go]#
```
此时，exe 作为初始化进程，pid 为 1 ，创建出了 pid 4 的 bash 子进程，而且已经看不到 main 进程了。<br />剩下的 IPC 、NET、 USER 等 NameSpace 就不在本文一一展示了。
<a name="cmsY2"></a>
### 2、Cgroups
借助 NameSpace 技术可以帮进程隔离出自己单独的空间，成功实现出最简容器。但是怎样限制这些空间的物理资源开销（CPU、内存、存储、I/O 等）就需要利用 Cgroups 技术了。<br />限制容器的资源使用，是一个非常重要的功能，如果一个容器可以毫无节制的使用服务器资源，那便又回到了传统模式下将应用直接运行在物理服务器上的弊端。这是容器化技术不能接受的。<br />Cgroups 的全称是 Control groups 即控制组，最早是由 Google 的工程师（主要是 Paul Menage 和 Rohit Seth）在 2006 年发起，一开始叫做进程容器（process containers）。在 2007 年时，因为在 Linux Kernel 中，容器（container）这个名词有许多不同的意义，为避免混乱，被重命名为 cgroup ，并且被合并到 2.6.24 版本的内核中去。<br />Android 也是凭借这个技术，为每个 APP 分配不同的 cgroup ，将每个 APP 进行隔离，而不会影响到其他的 APP 环境。<br />Cgroups 是对进程分组管理的一种机制，提供了对一组进程及它们的子进程的资源限制、控制和统计的能力，并为每种可以控制的资源定义了一个 subsystem （子系统）的方式进行统一接口管理，因此 subsystem 也被称为 resource controllers （资源控制器）。<br />几个主要的 subsystem 如下（ Cgroups V1 ）：

| 子系统 | 作用 |
| --- | --- |
| cpu | 限制进程的 cpu 使用率 |
| cpuacct | 统计进程的 cpu 使用情况 |
| cpuset | 在多核机器上为进程分配单独的 cpu 节点或者内存节点（仅限 NUMA 架构） |
| memory | 限制进程的 memory 使用量 |
| blkio | 控制进程对块设备（例如硬盘） io 的访问 |
| devices | 控制进程对设备的访问 |
| net_cls | 标记进程的网络数据包，以便可以使用 tc 模块（traffic control）对数据包进行限流、监控等控制 |
| net_prio | 控制进程产生的网络流量的优先级 |
| freezer | 挂起或者恢复进程 |
| pids | 限制 cgroup 的进程数量 |
| 更多子系统参考 [**Linux man cgroups**](https://man7.org/linux/man-pages/man7/cgroups.7.html)文档 | [https://man7.org/linux/man-pages/man7/cgroups.7.html](https://man7.org/linux/man-pages/man7/cgroups.7.html) |

借助 Cgroups 机制，可以将一组进程（task group）和一组 subsystem 关联起来，达到控制进程对应关联的资源的能力。如图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680245747661-febc4f4e-ecd6-4a25-9e46-2986d16913e9.png#averageHue=%23f8f7f7&clientId=u76f1f459-8ef3-4&from=paste&id=u12959163&originHeight=1039&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8116ba22-9f69-4f53-ba5c-9f4e079630f&title=)<br />Cgroups 的层级结构称为 hierarchy （即 cgroup 树），是一棵树，由 cgroup 节点组成。<br />系统可以有多个 hierarchy ，当创建新的 hierarchy 时，系统所有的进程都会加入到这个 hierarchy 默认创建的 root cgroup 根节点中，在树中，子节点可以继承父节点的属性。<br />对于同一个 hierarchy，进程只能存在于其中一个 cgroup 节点中。如果把一个进程添加到同一个 hierarchy 中的另一个 cgroup 节点，则会从第一个 cgroup 节点中移除。<br />hierarchy 可以附加一个或多个 subsystem 来拥有对应资源（如 cpu 和 memory ）的管理权，其中每一个 cgroup 节点都可以设置不同的资源限制权重，而进程（ task ）则绑定在 cgroup 节点中，并且其子进程也会默认绑定到父进程所在的 cgroup 节点中。<br />基于 Cgroups 的这些运作原理，可以得出：如果想限制某些进程的内存资源，就可以先创建一个 hierarchy ，并为其挂载 memory subsystem ，然后在这个 hierarchy 中创建一个 cgroup 节点，在这个节点中，将需要控制的进程 pid 和控制属性写入即可。<br />接下来就来实践一下。<br />Linux 一切皆文件。<br />在 Linux Kernel 中，为了让 Cgroups 的配置更直观，使用了目录的层级关系来模拟 hierarchy ，以此通过虚拟的树状文件系统的方式暴露给用户调用。<br />创建一个 hierarchy ，并为其挂载 memory subsystem ，这一步可以跳过，因为系统已经默认为每个 subsystem 创建了一个默认的 hierarchy ，可以直接使用。<br />例如 memory subsystem 默认的 hierarchy 就在 /sys/fs/cgroup/memory 目录。
```bash
[root@host go]# mount | grep memory
cgroup on /sys/fs/cgroup/memory type cgroup (rw,nosuid,nodev,noexec,relatime,memory)
[root@host go]# cd /sys/fs/cgroup/memory
[root@host memory]# pwd
/sys/fs/cgroup/memory
[root@host memory]#
```
只要在这个 hierarchy 目录下创建一个文件夹，就相当于创建了一个 cgroup 节点：
```bash
[root@host memory]# mkdir hello
[root@host memory]# cd hello/
[root@host hello]# ls
cgroup.clone_children           memory.kmem.slabinfo                memory.memsw.failcnt             memory.soft_limit_in_bytes
cgroup.event_control            memory.kmem.tcp.failcnt             memory.memsw.limit_in_bytes      memory.stat
cgroup.procs                    memory.kmem.tcp.limit_in_bytes      memory.memsw.max_usage_in_bytes  memory.swappiness
memory.failcnt                  memory.kmem.tcp.max_usage_in_bytes  memory.memsw.usage_in_bytes      memory.usage_in_bytes
memory.force_empty              memory.kmem.tcp.usage_in_bytes      memory.move_charge_at_immigrate  memory.use_hierarchy
memory.kmem.failcnt             memory.kmem.usage_in_bytes          memory.numa_stat                 notify_on_release
memory.kmem.limit_in_bytes      memory.limit_in_bytes               memory.oom_control               tasks
memory.kmem.max_usage_in_bytes  memory.max_usage_in_bytes           memory.pressure_level
[root@host hello]#
```
其中创建的 hello 文件夹内的所有文件都是系统自动创建的。常用的几个文件功能如下：

| 文件名 | 功能 |
| --- | --- |
| tasks | cgroup 中运行的进程（ PID）列表。将 PID 写入一个 cgroup 的 tasks 文件，可将此进程移至该 cgroup |
| cgroup.procs | cgroup 中运行的线程群组列表（ TGID ）。将 TGID 写入 cgroup 的 cgroup.procs 文件，可将此线程组群移至该 cgroup |
| cgroup.event_control | event_fd() 的接口。允许 cgroup 的变更状态通知被发送 |
| notify_on_release | 用于自动移除空 cgroup 。默认为禁用状态（0）。设定为启用状态（1）时，当 cgroup 不再包含任何任务时（即，cgroup 的 tasks 文件包含 PID，而 PID 被移除，致使文件变空），kernel 会执行 release_agent 文件（仅在 root cgroup 出现）的内容，并且提供通向被清空 cgroup 的相关路径（与 root cgroup 相关）作为参数 |
| memory.usage_in_bytes | 显示 cgroup 中进程当前所用的内存总量（以字节为单位） |
| memory.memsw.usage_in_bytes | 显示 cgroup 中进程当前所用的内存量和 swap 空间总和（以字节为单位） |
| memory.max_usage_in_bytes | 显示 cgroup 中进程所用的最大内存量（以字节为单位） |
| memory.memsw.max_usage_in_bytes | 显示 cgroup 中进程的最大内存用量和最大 swap 空间用量（以字节为单位） |
| memory.limit_in_bytes | 设定用户内存（包括文件缓存）的最大用量 |
| memory.memsw.limit_in_bytes | 设定内存与 swap 用量之和的最大值 |
| memory.failcnt | 显示内存达到 memory.limit_in_bytes 设定的限制值的次数 |
| memory.memsw.failcnt | 显示内存和 swap 空间总和达到 memory.memsw.limit_in_bytes 设定的限制值的次数 |
| memory.oom_control | 可以为 cgroup 启用或者禁用“内存不足”（Out of Memory，OOM） 终止程序。默认为启用状态（0），尝试消耗超过其允许内存的任务会被 OOM 终止程序立即终止。设定为禁用状态（1）时，尝试使用超过其允许内存的任务会被暂停，直到有额外内存可用。 |
| 更多文件的功能说明可以查看 kernel 文档中的 [**cgroup-v1/memory**](https://www.kernel.org/doc/Documentation/cgroup-v1/memory.txt) | [https://www.kernel.org/doc/Documentation/cgroup-v1/memory.txt](https://www.kernel.org/doc/Documentation/cgroup-v1/memory.txt) |

在这个 hello cgroup 节点中，想限制某些进程的内存资源，只需将对应的进程 pid 写入到 tasks 文件，并把内存最大用量设定到 `memory.limit_in_bytes` 文件即可。
```bash
[root@host hello]# cat memory.oom_control
oom_kill_disable 0
under_oom 0
[root@host hello]# cat memory.failcnt
0
[root@host hello]# echo 100M > memory.limit_in_bytes
[root@host hello]# cat memory.limit_in_bytes
104857600
[root@host hello]#
```
hello cgroup 节点默认启用了 OOM 终止程序，因此，当有进程尝试使用超过可用内存时会被立即终止。查询 memory.failcnt 可知，目前还没有进程内存达到过设定的最大内存限制值。<br />已经设定了 hello cgroup 节点可使用的最大内存为 100M ，此时新启动一个 bash 会话进程并将其移入到 hello cgroup 节点中：
```bash
[root@host hello]# /bin/bash
[root@host hello]# echo $$
4123
[root@host hello]# cat tasks
[root@host hello]# echo $$ > tasks
[root@host hello]# cat tasks
4123
4135
[root@host hello]# cat memory.usage_in_bytes
196608
[root@host hello]#
```
后续在此会话进程所创建的子进程都会加入到该 hello cgroup 节点中（例如 pid 4135 就是由于执行 cat 命令而创建的新进程，被系统自动加入到了 tasks 文件中）。<br />继续使用 [**memtester**](https://pyropus.ca./software/memtester/) 工具来测试 100M 的最大内存限制是否生效：
```bash
[root@host hello]# memtester 50M 1
memtester version 4.5.1 (64-bit)
Copyright (C) 2001-2020 Charles Cazabon.
Licensed under the GNU General Public License version 2 (only).

pagesize is 4096
pagesizemask is 0xfffffffffffff000
want 50MB (52428800 bytes)
got  50MB (52428800 bytes), trying mlock ...locked.
Loop 1/1:
  Stuck Address       : ok
  Random Value        : ok
  Compare XOR         : ok
  Compare SUB         : ok
  Compare MUL         : ok
  Compare DIV         : ok
  Compare OR          : ok
  Compare AND         : ok
  Sequential Increment: ok
  Solid Bits          : ok
  Block Sequential    : ok
  Checkerboard        : ok
  Bit Spread          : ok
  Bit Flip            : ok
  Walking Ones        : ok
  Walking Zeroes      : ok
  8-bit Writes        : ok
  16-bit Writes       : ok

Done.
[root@host hello]# memtester 100M 1
memtester version 4.5.1 (64-bit)
Copyright (C) 2001-2020 Charles Cazabon.
Licensed under the GNU General Public License version 2 (only).

pagesize is 4096
pagesizemask is 0xfffffffffffff000
want 100MB (104857600 bytes)
got  100MB (104857600 bytes), trying mlock ...over system/pre-process limit, reducing...
got  99MB (104853504 bytes), trying mlock ...over system/pre-process limit, reducing...
got  99MB (104849408 bytes), trying mlock ...over system/pre-process limit, reducing...
......
[root@host hello]# cat memory.failcnt
1434
[root@host hello]#
```
可以看到当 memtester 尝试申请 100M 内存时，失败了，而 memory.failcnt 报告显示内存达到 memory.limit_in_bytes 设定的限制值（100M）的次数为 1434 次。<br />如果想要删除 cgroup 节点，也只需要删除对应的文件夹即可。
```bash
[root@host hello]# exit
exit
[root@host hello]# cd ../
[root@host memory]# rmdir hello/
[root@host memory]#
```
经过上面对 Cgroups 的使用和实践，可以将其应用到之前的 Go 程序中：
```go
package main

import (
    "fmt"
    "io/ioutil"
    "os"
    "os/exec"
    "path/filepath"
    "strconv"
    "syscall"
)

func main() {
    switch os.Args[1] {
        case "run":
        run()
        case "child":
        child()
        default:
        panic("help")
    }
}

func run() {
    fmt.Println("[main]", "pid:", os.Getpid())
    cmd := exec.Command("/proc/self/exe", append([]string{"child"}, os.Args[2:]...)...)
    cmd.Stdin = os.Stdin
    cmd.Stdout = os.Stdout
    cmd.Stderr = os.Stderr
    cmd.SysProcAttr = &syscall.SysProcAttr{
        Cloneflags: syscall.CLONE_NEWUTS |
        syscall.CLONE_NEWPID |
        syscall.CLONE_NEWNS,
        Unshareflags: syscall.CLONE_NEWNS,
    }
    must(cmd.Run())
}

func child() {
    fmt.Println("[exe]", "pid:", os.Getpid())
    cg()
    must(syscall.Sethostname([]byte("mycontainer")))
    must(os.Chdir("/"))
    must(syscall.Mount("proc", "proc", "proc", 0, ""))
    cmd := exec.Command(os.Args[2], os.Args[3:]...)
    cmd.Stdin = os.Stdin
    cmd.Stdout = os.Stdout
    cmd.Stderr = os.Stderr
    must(cmd.Run())
    must(syscall.Unmount("proc", 0))
}

func cg() {
    mycontainer_memory_cgroups := "/sys/fs/cgroup/memory/mycontainer"
    os.Mkdir(mycontainer_memory_cgroups, 0755)
    must(ioutil.WriteFile(filepath.Join(mycontainer_memory_cgroups, "memory.limit_in_bytes"), []byte("100M"), 0700))
    must(ioutil.WriteFile(filepath.Join(mycontainer_memory_cgroups, "notify_on_release"), []byte("1"), 0700))
    must(ioutil.WriteFile(filepath.Join(mycontainer_memory_cgroups, "tasks"), []byte(strconv.Itoa(os.Getpid())), 0700))
}

func must(err error) {
    if err != nil {
        panic(err)
    }
}
```
在 exe 进程添加了对 `cg()` 函数的调用，代码相对简单，和实践流程几乎是一致的，区别只在于为 notify_on_release 文件设定为 1 值，使得当 exe 进程退出后，可以自动移除所创建的 cgroup 。
```bash
[root@host go]# go run main.go run /bin/bash
[main] pid: 4693
[exe] pid: 1
[root@mycontainer /]# ps
  PID TTY          TIME CMD
    1 pts/2    00:00:00 exe
    4 pts/2    00:00:00 bash
   15 pts/2    00:00:00 ps
[root@mycontainer /]# cat /sys/fs/cgroup/memory/mycontainer/tasks
1
4
16
[root@mycontainer /]# cat /sys/fs/cgroup/memory/mycontainer/notify_on_release
1
[root@mycontainer /]# cat /sys/fs/cgroup/memory/mycontainer/memory.limit_in_bytes
104857600
[root@mycontainer /]#
```
使用 memtester 测试和结果预期一致：
```bash
[root@mycontainer /]# memtester 100M 1
memtester version 4.5.1 (64-bit)
Copyright (C) 2001-2020 Charles Cazabon.
Licensed under the GNU General Public License version 2 (only).

pagesize is 4096
pagesizemask is 0xfffffffffffff000
want 100MB (104857600 bytes)
got  100MB (104857600 bytes), trying mlock ...over system/pre-process limit, reducing...
got  99MB (104853504 bytes), trying mlock ...over system/pre-process limit, reducing...
got  99MB (104849408 bytes), trying mlock ...over system/pre-process limit, reducing...
......
[root@mycontainer /]# exit
exit
[root@host go]#
```
同样篇幅问题，剩下的 subsystem 也不在本文一一展示了。<br />其实到这里，已经通过 NameSpace 技术帮进程隔离出自己单独的空间，并使用 Cgroups 技术限制和监控这些空间的资源开销，这种特殊的进程就是容器的本质。可以说，本篇文章的目的已达成，可以结束了。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680245747618-8f933aba-466c-4769-9ede-a47a9b08c878.png#averageHue=%23e9ebf0&clientId=u76f1f459-8ef3-4&from=paste&id=uef4c43c5&originHeight=442&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8f206b45-15e3-4333-91e5-8b284c99665&title=)<br />但是除了利用 NameSpace 和 Cgroups 来实现 **容器（container）** ，在 Docker 中,还使用到了一个 Linux Kernel 技术：UnionFS 来实现 **镜像（images）** 功能。<br />鉴于本篇文章的主旨 —— 使用 Go 和 Linux Kernel 技术探究容器化原理的主要技术点是 NameSpace 和 Cgroups 。镜像的实现技术 UnionFS 属于加餐内容，可自行选择是否需要消化。
<a name="ME9Kd"></a>
### 3、UnionFS
UnionFS 全称 Union File System （联合文件系统），在 2004 年由纽约州立大学石溪分校开发，是为 Linux、FreeBSD 和 NetBSD 操作系统设计的一种分层、轻量级并且高性能的文件系统，可以 **把多个目录内容联合挂载到同一个目录下** ，而目录的物理位置是分开的，并且对文件系统的修改是类似于 git 的 commit 一样 **作为一次提交来一层层的叠加的** 。<br />在 Docker 中，镜像相当于是容器的模板，一个镜像可以衍生出多个容器。镜像利用 UnionFS 技术来实现，就可以利用其 **分层的特性** 来进行镜像的继承，基于基础镜像，制作出各种具体的应用镜像，不同容器就可以直接 **共享基础的文件系统层** ，同时再加上自己独有的改动层，大大提高了存储的效率。<br />[**以该 Dockerfile 为例**](https://docs.docker.com/storage/storagedriver/)：
```dockerfile
FROM ubuntu:18.04
LABEL org.opencontainers.image.authors="org@example.com"
COPY . /app
RUN make /app
RUN rm -r $HOME/.cache
CMD python /app/app.py
```
镜像的每一层都可以代表 Dockerfile 中的一条指令，并且除了最后一层之外的每一层都是只读的。<br />在该 Dockerfile 中包含了多个命令，如果命令修改了文件系统就会创建一个层（利用 UnionFS 的原理）。<br />首先 FROM 语句从 ubuntu:18.04 镜像创建一个层 【1】，而 LABEL 命令仅修改镜像的元数据，不会生成新镜像层，接着 COPY 命令会把当前目录中的文件添加到镜像中的 /app 目录下，在层【1】的基础上生成了层【2】。<br />第一个 RUN 命令使用 make 构建应用程序，并将结果写入新层【3】。第二个 RUN 命令删除缓存目录，并将结果写入新层【4】。最后，CMD 指令指定在容器内运行什么命令，只修改了镜像的元数据，也不会产生镜像层。<br />这【4】个层（layer）相互堆叠在一起就是一个镜像。当创建一个新容器时，会在 **镜像层（image layers）** 上面再添加一个新的可写层，称为 **容器层（container layer）** 。对正在运行的容器所做的所有更改，例如写入新文件、修改现有文件和删除文件，都会写入到这个可写容器层。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1680245769150-6dd8cd3b-5f3d-4d2b-af24-1e9a100aae63.jpeg#averageHue=%23f4f1e5&clientId=u76f1f459-8ef3-4&from=paste&id=u056857f6&originHeight=469&originWidth=675&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u292c2d89-01b4-41a7-9696-08ac18ebea2&title=)<br />对于相同的镜像层，每一个容器都会有自己的可写容器层，并且所有的变化都存储在这个容器层中，所以多个容器可以共享对同一个底层镜像的访问，并且拥有自己的数据状态。而当容器被删除时，其可写容器层也会被删除，如果用户需要持久化容器里的数据，就需要使用 Volume 挂载到宿主机目录。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1680245769149-c900c61e-d1f8-4f34-bfc0-593b138d6b95.jpeg#averageHue=%23efeee6&clientId=u76f1f459-8ef3-4&from=paste&id=ue631f22d&originHeight=475&originWidth=769&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uda3eda0a-6d9c-4319-ba97-247b1eb76d9&title=)<br />看完 Docker 镜像的运作原理，回到其实现技术 UnionFS 本身。<br />目前 Docker 支持的 UnionFS 有以下几种类型：

| 联合文件系统 | 存储驱动 | 说明 |
| --- | --- | --- |
| OverlayFS | overlay2 | 当前所有受支持的 Linux 发行版的 **首选** 存储驱动程序，并且不需要任何额外的配置 |
| OverlayFS | fuse-overlayfs | 仅在不提供对 rootless 支持的主机上运行 Rootless Docker 时才首选 |
| Btrfs 和 ZFS | btrfs 和 zfs | 允许使用高级选项，例如创建快照，但需要更多的维护和设置 |
| VFS | vfs | 旨在用于测试目的，以及无法使用写时复制文件系统的情况下使用。此存储驱动程序性能较差，一般不建议用于生产用途 |
| AUFS | aufs | Docker 18.06 和更早版本的首选存储驱动程序。但是在没有 overlay2 驱动的机器上仍然会使用 aufs 作为 Docker 的默认驱动 |
| Device Mapper | devicemapper | RHEL （旧内核版本不支持 overlay2，最新版本已支持）的 Docker Engine 的默认存储驱动，有两种配置模式：loop-lvm（零配置但性能差） 和 direct-lvm（生产环境推荐） |
| OverlayFS | overlay | 推荐使用 overlay2 存储驱动 |

在尽可能的情况下，推荐使用 OverlayFS 的 overlay2 存储驱动，这也是当前 Docker 默认的存储驱动（以前是 AUFS 的 aufs ）。<br />可查看 Docker 使用了哪种存储驱动：
```bash
[root@host ~]# docker -v
Docker version 20.10.15, build fd82621
[root@host ~]# docker info | grep Storage
 Storage Driver: overlay2
[root@host ~]#
```
OverlayFS 其实是一个类似于 AUFS 的、面向 Linux 的现代联合文件系统，在 2014 年被合并到 Linux Kernel （version 3.18）中，相比 AUFS 其速度更快且实现更简单。 overlay2 （Linux Kernel version 4.0 或以上）则是其推荐的驱动程序。<br />overlay2 由四个结构组成，其中：

- lowerdir ：表示较为底层的目录，对应 Docker 中的只读镜像层
- upperdir ：表示较为上层的目录，对应 Docker 中的可写容器层
- workdir ：表示工作层（中间层）的目录，在使用过程中对用户不可见
- merged ：所有目录合并后的联合挂载点，给用户暴露的统一目录视图，对应 Docker 中用户实际看到的容器内的目录视图

这是在 Docker 文档中关于 [**overlay 的架构图**](https://docs.docker.com/storage/storagedriver/overlayfs-driver/#how-the-overlay-driver-works)，但是对于 overlay2 也同样可以适用：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1680245769114-e7a34f74-a62a-4a6f-9b10-500a130e3253.jpeg#averageHue=%23e5e5e2&clientId=u76f1f459-8ef3-4&from=paste&id=u5c0aa149&originHeight=193&originWidth=754&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7433e85f-1250-4846-a404-19af459dbf3&title=)<br />其中 lowerdir 所对应的镜像层（ Image layer ），实际上是可以有很多层的，图中只画了一层。<br />细心的小伙伴可能会发现，图中并没有出现 workdir ，它究竟是如何工作的呢？<br />可以从读写的视角来理解，对于读的情况：

- 文件在 upperdir ，直接读取
- 文件不在 upperdir ，从 lowerdir 读取，会产生非常小的性能开销
- 文件同时存在 upperdir 和 lowerdir 中，从 upperdir 读取（upperdir 中的文件隐藏了 lowerdir 中的同名文件）

对于写的情况：

- 创建一个新文件，文件在 upperdir 和 lowerdir 中都不存在，则直接在 upperdir 创建
- 修改文件，如果该文件在 upperdir 中存在，则直接修改
- 修改文件，如果该文件在 upperdir 中不存在，将执行 copy_up 操作，把文件从 lowerdir 复制到 upperdir ，后续对该文件的写入操作将对已经复制到 upperdir 的副本文件进行操作。这就是 **写时复制（copy-on-write）**
- 删除文件，如果文件只在 upperdir 存在，则直接删除
- 删除文件，如果文件只在 lowerdir 存在，会在 upperdir 中创建一个同名的空白文件（whiteout file），lowerdir 中的文件不会被删除，因为他们是只读的，但 whiteout file 会阻止它们继续显示
- 删除文件，如果文件在 upperdir 和 lowerdir 中都存在，则先将 upperdir 中的文件删除，再创建一个同名的空白文件（whiteout file）
- 删除目录和删除文件是一致的，会在 upperdir 中创建一个同名的不透明的目录（opaque directory），和 whiteout file 原理一样，opaque directory 会阻止用户继续访问，即便 lowerdir 内的目录仍然存在

说了半天，好像还是没有讲到 workdir 的作用，这得理解一下，毕竟人家在使用过程中对用户是不可见的。<br />但其实 workdir 的作用不可忽视。想象一下，在删除文件（或目录）的场景下（文件或目录在 upperdir 和 lowerdir 中都存在），对于 lowerdir 而言，倒没什么，毕竟只读，不需要理会，但是对于 upperdir 来讲就不同了。在 upperdir 中，要先删除对应的文件，然后才可以创建同名的 whiteout file ，如何保证这两步必须都执行，这就涉及到了原子性操作了。<br />workdir 是用来进行一些中间操作的，其中就包括了原子性保证。在上面的问题中，完全可以先在 workdir 创建一个同名的 whiteout file ，然后再在 upperdir 上执行两步操作，成功之后，再删除掉 workdir 中的 whiteout file 即可。<br />而当修改文件时，workdir 也在充当着中间层的作用，当对 upperdir 里面的副本进行修改时，会先放到 workdir ，然后再从 workdir 移到 upperdir 里面去。<br />理解完 overlay2 运作原理，接下来正式进入到演示环节。<br />首先可以来看看在 Docker 中启动了一个容器后，其挂载点是怎样的：
```bash
[root@host ~]# mount | grep overlay
[root@host ~]# docker run -d -it ubuntu:18.04 /bin/bash
cb25841054d9f037ec5cf4c24a97a05f771b43a358dd89b40346ca3ab0e5eaf4
[root@host ~]# mount | grep overlay
overlay on /var/lib/docker/overlay2/56bbb1dbdd636984e4891db7850939490ece5bc7a3f3361d75b1341f0fb30b85/merged type overlay (rw,relatime,lowerdir=/var/lib/docker/overlay2/l/OOPFROHUDK727Z5QKNPWG5FBWV:/var/lib/docker/overlay2/l/6TWQL4UC7XYLZWZBKPS6F4IKLF,upperdir=/var/lib/docker/overlay2/56bbb1dbdd636984e4891db7850939490ece5bc7a3f3361d75b1341f0fb30b85/diff,workdir=/var/lib/docker/overlay2/56bbb1dbdd636984e4891db7850939490ece5bc7a3f3361d75b1341f0fb30b85/work)
[root@host ~]# ll /var/lib/docker/overlay2/56bbb1dbdd636984e4891db7850939490ece5bc7a3f3361d75b1341f0fb30b85/merged
total 76
drwxr-xr-x  2 root root 4096 Apr 28 08:04 bin
drwxr-xr-x  2 root root 4096 Apr 24  2018 boot
drwxr-xr-x  1 root root 4096 May 10 11:17 dev
drwxr-xr-x  1 root root 4096 May 10 11:17 etc
drwxr-xr-x  2 root root 4096 Apr 24  2018 home
drwxr-xr-x  8 root root 4096 May 23  2017 lib
drwxr-xr-x  2 root root 4096 Apr 28 08:03 lib64
drwxr-xr-x  2 root root 4096 Apr 28 08:03 media
drwxr-xr-x  2 root root 4096 Apr 28 08:03 mnt
drwxr-xr-x  2 root root 4096 Apr 28 08:03 opt
drwxr-xr-x  2 root root 4096 Apr 24  2018 proc
drwx------  2 root root 4096 Apr 28 08:04 root
drwxr-xr-x  5 root root 4096 Apr 28 08:04 run
drwxr-xr-x  2 root root 4096 Apr 28 08:04 sbin
drwxr-xr-x  2 root root 4096 Apr 28 08:03 srv
drwxr-xr-x  2 root root 4096 Apr 24  2018 sys
drwxrwxrwt  2 root root 4096 Apr 28 08:04 tmp
drwxr-xr-x 10 root root 4096 Apr 28 08:03 usr
drwxr-xr-x 11 root root 4096 Apr 28 08:04 var
[root@host ~]#
```
可以看到，挂载后的 merged 目录包括了 lowerdir 、upperdir 、workdir 目录，而 merged 目录实际上就是容器内用户看到的目录视图。<br />回到技术本身，可以自己来尝试一下如何使用 [**mount 的 overlay 挂载选项**](https://man7.org/linux/man-pages/man8/mount.8.html) ：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680245769181-9a436f9e-663a-4967-a44d-e23a096dc500.png#averageHue=%23fbfaf9&clientId=u76f1f459-8ef3-4&from=paste&id=u4f24218c&originHeight=1115&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc7d744d3-64eb-4535-b799-69023686a8c&title=)<br />首先创建好 lowerdir（创建了 2 个） 、upperdir 、workdir、 merged 目录，并为 lowerdir 和 upperdir 目录写入一些文件：
```bash
[root@host ~]# mkdir test_overlay
[root@host ~]# cd test_overlay/
[root@host test_overlay]# mkdir lower1
[root@host test_overlay]# mkdir lower2
[root@host test_overlay]# mkdir upper
[root@host test_overlay]# mkdir work
[root@host test_overlay]# mkdir merged
[root@host test_overlay]# echo 'lower1-file1' > lower1/file1.txt
[root@host test_overlay]# echo 'lower2-file2' > lower2/file2.txt
[root@host test_overlay]# echo 'upper-file3' > upper/file3.txt
[root@host test_overlay]# tree
.
|-- lower1
|   `-- file1.txt
|-- lower2
|   `-- file2.txt
|-- merged
|-- upper
|   `-- file3.txt
`-- work

5 directories, 3 files
[root@host test_overlay]#
```
使用 mount 命令的 overlay 选项模式进行挂载：
```bash
[root@host test_overlay]# mount -t overlay overlay -olowerdir=lower1:lower2,upperdir=upper,workdir=work merged
[root@host test_overlay]# mount | grep overlay
......
overlay on /root/test_overlay/merged type overlay (rw,relatime,lowerdir=lower1:lower2,upperdir=upper,workdir=work)
[root@host test_overlay]#
```
此时进入 merged 目录就可以看到所有文件了：
```bash
[root@host test_overlay]# cd merged/
[root@host merged]# ls
file1.txt  file2.txt  file3.txt
[root@host merged]#
```
尝试修改 lowerdir 目录内的文件：
```bash
[root@host merged]# echo 'lower1-file1-hello' > file1.txt
[root@host merged]# cat file1.txt
lower1-file1-hello
[root@host merged]# cat /root/test_overlay/lower1/file1.txt
lower1-file1
[root@host merged]# ls /root/test_overlay/upper/
file1.txt  file3.txt
[root@host merged]# cat /root/test_overlay/upper/file1.txt
lower1-file1-hello
[root@host merged]#
```
和之前所说的一致，当修改 lowerdir 内的文件时，会执行 copy_up 操作，把文件从 lowerdir 复制到 upperdir ，后续对该文件的写入操作将对已经复制到 upperdir 的副本文件进行操作。<br />其它的读写情况，大家就可以自行尝试了。
<a name="T1x6l"></a>
## 总结
其实容器的底层原理并不难，本质上就是一个特殊的进程，特殊在为其创建了 NameSpace 隔离运行环境，用 Cgroups 为其控制了资源开销，这些都是站在 Linux 操作系统的肩膀上实现的，包括 Docker 的镜像实现也是利用了 UnionFS 的分层联合技术。<br />甚至可以说几乎所有应用的本质都是 **上层调下层 ，下层支撑着上层** 。
<a name="nFpsr"></a>
## 参考资料
**Linux man 手册中的 NAMESPACES: **[**https://man7.org/linux/man-pages/man7/namespaces.7.html**](https://man7.org/linux/man-pages/man7/namespaces.7.html)<br />**源自 Containers From Scratch • Liz Rice • GOTO 2018: **[**https://www.youtube.com/watch?v=8fi7uSYlOdc**](https://www.youtube.com/watch?v=8fi7uSYlOdc)<br />**Linux man cgroups: **[**https://man7.org/linux/man-pages/man7/cgroups.7.html**](https://man7.org/linux/man-pages/man7/cgroups.7.html)<br />**cgroup-v1/memory: **[**https://www.kernel.org/doc/Documentation/cgroup-v1/memory.txt**](https://www.kernel.org/doc/Documentation/cgroup-v1/memory.txt)<br />**memtester: **[**https://pyropus.ca./software/memtester/**](https://pyropus.ca./software/memtester/)<br />**以该 Dockerfile 为例: **[**https://docs.docker.com/storage/storagedriver/**](https://docs.docker.com/storage/storagedriver/)<br />**overlay 的架构图: **[**https://docs.docker.com/storage/storagedriver/overlayfs-driver/#how-the-overlay-driver-works**](https://docs.docker.com/storage/storagedriver/overlayfs-driver/#how-the-overlay-driver-works)<br />**mount 的 overlay 挂载选项: **[**https://man7.org/linux/man-pages/man8/mount.8.html**](https://man7.org/linux/man-pages/man8/mount.8.html)
