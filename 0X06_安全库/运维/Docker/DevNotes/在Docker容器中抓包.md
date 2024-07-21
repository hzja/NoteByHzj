Docker
<a name="ES5Uf"></a>
## 抓包命令
<a name="JMpoM"></a>
### nsenter
Nsenter命令用于进入指定进程的命名空间，包括最常用的挂载命名空间和网络命名空间。适用于容器这种特殊的进程。
```bash
#常用用法：
#进入网络命名空间
$ nsenter -t <Pid> -n 
#进入挂载命名空间，类似docker exec进入容器
$ nsenter -t <Pid> -m
```
<a name="tcpdump"></a>
### tcpdump
Tcpdump是Linux原生的抓包工具，常用与在linux系统上抓包，常见用法如下。
```bash
# 常用用法：
# 抓取某网卡所有数据包
$ tcpdump -i <网卡名>
# 抓取包含某IP的网络包
$ tcpdump -i <网卡名> -vnn host <IP>
# 抓取某源IP的网络包
$ tcpdump -i <网卡名> -vnn src host <源IP>
# 抓取某目标IP的网络包
$ tcpdump -i <网卡名> -vnn dst host <目标IP>
# 抓取网络包放到文件
$ tcpdump -i <网卡名> -w <文件名>
```
接下来使用上述命令对容器进行抓包。
<a name="mZ2hZ"></a>
## 找到容器Pid
<a name="wpPDW"></a>
### 方法一：`docker top`
```bash
$ docker top <container-id>
```
对于一般的容器，只有一个进程，Pid如图所示。有的容器里面运行着多个进程，也是取第一行的Pid。
<a name="irF1T"></a>
### 方法二：`docker inspect`
```bash
$ docker inspect <container-id>| grep Pid
```
可以截取到容器进程的Pid。进入容器的命名空间<br />接下来通过nsenter命令进入容器进程的网络命名空间。
```basic
$ sudo nsenter -t <进程标识符Pid> -n
```
此时使用ifconfig命令查看到的就是容器内部的网卡信息。
<a name="849c421c"></a>
## 使用tcpdump抓包
```bash
$ tcpdump -i <网卡名>
```
使用tcpdump命令对eth0网卡抓包<br />如果是在kubernetes环境中，需要定位容器的位置，并到对应节点去抓包；
```bash
$ kubectl get pod -n <命名空间> <pod名> -o wide
```
查看该pod的容器名
```bash
$ kubectl get pod -n <命名空间> <pod名> -o yaml | grep containerID
```
继续按照上述方式去抓容器的包，最后把抓到的包放到wireshark上分析。
<a name="OEG7X"></a>
## 使用nsenter抓包
<a name="raAQx"></a>
### 简介
nsenter 命令是一个可以在指定进程的命令空间下运行指定程序的命令。它位于 util-linux 包中。
<a name="nREVm"></a>
### 用途
一个最典型的用途就是进入容器的网络命令空间。相当多的容器为了轻量级，是不包含较为基础的命令的，比如说 ip address，ping，telnet，ss，tcpdump 等等命令，这就给调试容器网络带来相当大的困扰：只能通过 docker inspect ContainerID 命令获取到容器 IP，以及无法测试和其他网络的连通性。这时就可以使用 nsenter 命令仅进入该容器的网络命名空间，使用宿主机的命令调试容器网络。<br />此外，nsenter 也可以进入mnt，uts，ipc，pid，user命令空间，以及指定根目录和工作目录。
<a name="XvV78"></a>
### 使用
首先看下 nsenter 命令的语法：
```bash
nsenter [options] [program [arguments]]
```
<a name="solEC"></a>
#### 参数说明
```
-t, --target pid #指定被进入命名空间的目标进程的pid
-m, --mount[=file] #进入mount命令空间。如果指定了file，则进入file的命令空间
-u, --uts[=file] #进入uts命令空间。如果指定了file，则进入file的命令空间
-i, --ipc[=file] #进入ipc命令空间。如果指定了file，则进入file的命令空间
-n, --net[=file] #进入net命令空间。如果指定了file，则进入file的命令空间
-p, --pid[=file] #进入pid命令空间。如果指定了file，则进入file的命令空间
-U, --user[=file] #进入user命令空间。如果指定了file，则进入file的命令空间
-G, --setgid gid  #设置运行程序的gid
-S, --setuid uid  #设置运行程序的uid
-r, --root[=directory] #设置根目录
-w, --wd[=directory]  #设置工作目录
```
如果没有给出program，则默认执行$SHELL。
<a name="KFcYS"></a>
#### 示例
运行一个 Nginx 容器，查看该容器的 pid：
```bash
[root@staight ~]# docker inspect -f {{.State.Pid}} nginx
5645
```
然后，使用 nsenter 命令进入该容器的网络命令空间：
```bash
[root@staight ~]# nsenter -n -t5645
[root@staight ~]# ip addr
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
    inet 127.0.0.1/8 scope host lo
       valid_lft forever preferred_lft forever
18: eth0@if19: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue state UP group default 
    link/ether 02:42:ac:11:00:02 brd ff:ff:ff:ff:ff:ff link-netnsid 0
    inet 172.17.0.2/16 brd 172.17.255.255 scope global eth0
       valid_lft forever preferred_lft forever
```
进入成功~<br />在 Kubernetes 中，在得到容器 pid 之前还需获取容器的 ID，可以使用如下命令获取：
```bash
[root@node1 test]# kubectl get pod test -oyaml|grep containerID
  - containerID: docker://cf0873782d587dbca6aa32f49605229da3748600a9926e85b36916141597ec85
```
或者更为精确地获取 containerID ：
```bash
[root@node1 test]# kubectl get pod test -o template --template='{{range .status.containerStatuses}}{{.containerID}}{{end}}'
docker://cf0873782d587dbca6aa32f49605229da3748600a9926e85b36916141597ec85
```
<a name="hvbRD"></a>
### 原理
<a name="Obt7w"></a>
#### namespace
namespace 是 Linux 中一些进程的属性的作用域，使用命名空间，可以隔离不同的进程。<br />Linux在不断的添加命名空间，目前有：
```
mount：#挂载命名空间，使进程有一个独立的挂载文件系统，始于Linux 2.4.19
ipc：#ipc命名空间，使进程有一个独立的ipc，包括消息队列，共享内存和信号量，始于Linux 2.6.19
uts：#uts命名空间，使进程有一个独立的hostname和domainname，始于Linux 2.6.19
net：#network命令空间，使进程有一个独立的网络栈，始于Linux 2.6.24
pid：#pid命名空间，使进程有一个独立的pid空间，始于Linux 2.6.24
user：#user命名空间，是进程有一个独立的user空间，始于Linux 2.6.23，结束于Linux 3.8
cgroup：#cgroup命名空间，使进程有一个独立的cgroup控制组，始于Linux 4.6
```
Linux 的每个进程都具有命名空间，可以在 /proc/PID/ns 目录中看到命名空间的文件描述符。
```bash
[root@staight ns]# pwd
/proc/1/ns
[root@staight ns]# ll
total 0
lrwxrwxrwx 1 root root 0 Sep 23 19:53 ipc -> ipc:[4026531839]
lrwxrwxrwx 1 root root 0 Sep 23 19:53 mnt -> mnt:[4026531840]
lrwxrwxrwx 1 root root 0 Sep 23 19:53 net -> net:[4026531956]
lrwxrwxrwx 1 root root 0 Sep 23 19:53 pid -> pid:[4026531836]
lrwxrwxrwx 1 root root 0 Sep 23 19:53 user -> user:[4026531837]
lrwxrwxrwx 1 root root 0 Sep 23 19:53 uts -> uts:[4026531838]
```
<a name="UFFiJ"></a>
#### clone
clone 是 Linux 的系统调用函数，用于创建一个新的进程。<br />clone 和 fork 比较类似，但更为精细化，比如说使用 clone 创建出的子进程可以共享父进程的虚拟地址空间，文件描述符表，信号处理表等等。不过这里要强调的是，clone 函数还能为新进程指定命名空间。
<a name="Er6m8"></a>
##### clone的语法
```c
#define _GNU_SOURCE
#include <sched.h>

int clone(int (*fn)(void *), void *child_stack,
          int flags, void *arg, ...
          /* pid_t *ptid, void *newtls, pid_t *ctid */ );
```
其中 flags 即可指定命名空间，包括：
```
CLONE_NEWCGROUP：cgroup

CLONE_NEWIPC：ipc

CLONE_NEWNET：net

CLONE_NEWNS：mount

CLONE_NEWPID：pid

CLONE_NEWUSER：user

CLONE_NEWUTS：uts
```
<a name="hckaH"></a>
##### 使用示例：
```
pid = clone(childFunc, stackTop, CLONE_NEWUTS | SIGCHLD, argv[1]);
```
<a name="nk78u"></a>
#### setns
clone 用于创建新的命令空间，而 setns 则用来让当前线程（单线程即进程）加入一个命名空间。
<a name="BBdAi"></a>
##### 语法
```c
#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <sched.h>

int setns(int fd, int nstype);
```

- fd 参数是一个指向一个命名空间的文件描述符，位于/proc/PID/ns/目录。
- nstype指定了允许进入的命名空间，一般可设置为0，表示允许进入所有命名空间。

因此，往往该函数的用法为：

- 调用`setns`函数：指定该线程的命名空间。
- 调用`execvp`函数：执行指定路径的程序，创建子进程并替换父进程。

这样，就可以指定命名空间运行新的程序了。<br />代码示例：
```c
#define _GNU_SOURCE
#include <fcntl.h>
#include <sched.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                        } while (0)

int
main(int argc, char *argv[])
{
    int fd;

    if (argc < 3) {
        fprintf(stderr, "%s /proc/PID/ns/FILE cmd args...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    fd = open(argv[1], O_RDONLY); /* Get file descriptor for namespace */
    if (fd == -1)
        errExit("open");

    if (setns(fd, 0) == -1)       /* Join that namespace */
        errExit("setns");

    execvp(argv[2], &argv[2]);    /* Execute a command in namespace */
    errExit("execvp");
}
```
<a name="HZDzV"></a>
##### 使用示例：
```
./ns_exec /proc/3550/ns/uts /bin/bash
```
<a name="KuX0Q"></a>
#### nsenter
那么，最后就是 nsenter 了，nsenter 相当于在setns的示例程序之上做了一层封装，无需指定命名空间的文件描述符，而是指定进程号即可。<br />指定进程号PID以及需要进入的命名空间后，nsenter会找到对应的命名空间文件描述符/proc/PID/ns/FD，然后使用该命名空间运行新的程序。
