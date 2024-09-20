Linux<br />在Linux中，系统默认的用户是root，其实和 windows 的 administrator 类似，root 用户可以操作操作系统的任何文件和设备，所以在生产环境就不要乱用root了，权利越大，责任越大。<br />学习Linux，就要习惯通过命令行的方式学习，。目前的Linux导图如下<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650863508357-2d620ae2-8bd0-4921-8d2a-3e992e379b8c.png#clientId=u47a477fe-9cb5-4&from=paste&id=ucac72206&originHeight=2755&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua5886d80-645d-44dd-b525-af139242ecb&title=)
<a name="GF42F"></a>
## 1、系统与硬件相关
<a name="bSyiw"></a>
### 查看系统的 pci 设备
`lspci` 命令可以查看所有的 pci 设备，比如主板，声卡等<br />![lspci查看设备信息](https://cdn.nlark.com/yuque/0/2022/png/396745/1650863508389-4f115dc5-cf51-44bd-8099-f8ddef90743b.png#clientId=u47a477fe-9cb5-4&from=paste&id=u74655aa0&originHeight=147&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u1fdba7ab-1bd5-4e6a-a684-0edb6aafc4a&title=lspci%E6%9F%A5%E7%9C%8B%E8%AE%BE%E5%A4%87%E4%BF%A1%E6%81%AF "lspci查看设备信息")
<a name="aY5qv"></a>
### 查看CPU信息
```bash
more /proc/cpuinfo 
```
其中 processor 是逻辑处理器的唯标识符，vendor-id 是处理器类型，如果是英特尔的处理器，那么字符串就是genuinelntel。physical id 是每个封装的唯一标识符，也是一个物理的CPU，siblings 表示位于相同物理封装中的逻辑处理器数量，core id 为每个内核的唯一标识符<br />在 siblings 和 CPU cores 有个对应关系，如果 siblings 是 CPU cores 的两倍，则系统支持超线程。
<a name="cC1jN"></a>
### 查看 CPU 的个数
如果要查看 CPU 的个数，那么可以通过下面的命令
```bash
cat /proc/cpuinfo | grep “physical id”  | sort | uniq | wc -l
```
<a name="O972z"></a>
### 查看系统所有的逻辑CPU个数
```bash
cat /proc/cpuinfo |grep "processor" | wc -l
```
<a name="G0gl2"></a>
### 查看内存的信息
![查看内存相关信息](https://cdn.nlark.com/yuque/0/2022/png/396745/1650863508288-24d8a04c-e55c-4fe2-bbd9-3087eba4170b.png#clientId=u47a477fe-9cb5-4&from=paste&id=u5afd97d1&originHeight=142&originWidth=817&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u842c59ce-ea92-4264-8687-bfbe7e731d8&title=%E6%9F%A5%E7%9C%8B%E5%86%85%E5%AD%98%E7%9B%B8%E5%85%B3%E4%BF%A1%E6%81%AF "查看内存相关信息")
```bash
more /proc/meminfo
```
从结果可以看出，目前总内存为1.8G，可用1.5G，缓冲区占用1260KB。
```bash
more /proc/meminfo
```
<a name="tleX8"></a>
### 查看磁盘分区的信息
```bash
fdisk  -l
```
![查看磁盘分区信息](https://cdn.nlark.com/yuque/0/2022/png/396745/1650863508298-4e125894-d17e-4f51-a3a6-6fdce8869259.png#clientId=u47a477fe-9cb5-4&from=paste&id=uf2a879e9&originHeight=463&originWidth=825&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u4b41ba7c-1c18-4c0e-b2bd-3edf0638159&title=%E6%9F%A5%E7%9C%8B%E7%A3%81%E7%9B%98%E5%88%86%E5%8C%BA%E4%BF%A1%E6%81%AF "查看磁盘分区信息")<br />从上图可以看见虚拟了 42G 的磁盘以及每个分区的大小，是不是关于硬件相关的信息看的清清楚楚了，就这样不再神秘
<a name="Bgtqk"></a>
## 2、文件系统的了解
<a name="aHmig"></a>
### 经典树形目录
Linux 牛逼之一在于将所有的内容以文件的形式展现出来，通过一个树形的结构统一管理和组织这些文件。Linux 典型的树形结构如下图所示<br />这个树有个根，根下面很多「杈」，「杈」上可以长出很多叶子

![树形目录](https://cdn.nlark.com/yuque/0/2022/png/396745/1650863508391-d7449295-20b1-4354-a01d-4241946f3202.png#clientId=u47a477fe-9cb5-4&from=paste&id=ub81c1472&originHeight=371&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u3c73bf44-1312-4219-9c30-d78c3e0b7a5&title=%E6%A0%91%E5%BD%A2%E7%9B%AE%E5%BD%95 "树形目录")<br />整个系统都是按照文件的形式放在根目录中，同时会将文件分类，这样形成一个树形目录结构，这样的布局给管理文件带来了很大的方便，确实需要去了解这些目录结构，对于后续的系统排错还是蛮重要的哒。虽然有些啰嗦，不过这些图确实是实打实的画出来的，没事看看就知道了，不用记住。
<a name="keZwP"></a>
### /etc目录
这个目录主要存放的是管理文件用的相关配置文件，比较重要的  /etc/rc，用户信息文件 /etc/passwd，具体的如下图<br />![etc目录](https://cdn.nlark.com/yuque/0/2022/png/396745/1650863508534-df343a3b-c23d-4531-9129-fe7b6abf2858.png#clientId=u47a477fe-9cb5-4&from=paste&id=ubec6ac7e&originHeight=1217&originWidth=1037&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=uf2e3cfd3-d475-464f-bac8-209f6beb552&title=etc%E7%9B%AE%E5%BD%95 "etc目录")
<a name="yZp8P"></a>
### /usr目录
这个目录主要存放的是应用程序和文件，如果在安装软件的时候，选择默认安装的位置，通常就会默认在这个位置，具体信息如下图<br />![usr目录](https://cdn.nlark.com/yuque/0/2022/png/396745/1650863508581-24b3d988-2846-4547-afbe-c843fc10eeac.png#clientId=u47a477fe-9cb5-4&from=paste&id=uefe6638e&originHeight=947&originWidth=1022&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u735a743f-e228-4681-ac8d-cf1944a1c4c&title=usr%E7%9B%AE%E5%BD%95 "usr目录")
<a name="B9RD9"></a>
### /var目录
通常用来存放系统运行的日志文件，如下图<br />![var目录](https://cdn.nlark.com/yuque/0/2022/png/396745/1650863508756-f4f32dc1-2709-420f-b77d-0e12dd1f9ff6.png#clientId=u47a477fe-9cb5-4&from=paste&id=u1cfcd857&originHeight=692&originWidth=1037&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ue0644ec1-048b-48aa-8853-b7f2612caa4&title=var%E7%9B%AE%E5%BD%95 "var目录")
<a name="jHHAB"></a>
### /dev 目录
包含所有的设备文件
<a name="aI9vq"></a>
### /proc 目录
这个目录是虚拟目录，主要存放的是内存的映射，通过这个目录和内核的数据结构打交道比如修改内核参数，获取进程的相关信息，具体信息如下<br />![proc目录](https://cdn.nlark.com/yuque/0/2022/png/396745/1650863508845-e192de89-85b7-4dca-bc75-7b212e45bd3f.png#clientId=u47a477fe-9cb5-4&from=paste&id=uf5b8fc67&originHeight=977&originWidth=1052&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u232d1f38-4791-4be9-8da7-56320df0024&title=proc%E7%9B%AE%E5%BD%95 "proc目录")
<a name="tiTGW"></a>
### 其他目录
<a name="efkuN"></a>
#### /boot目录
它存放了启动 Linux 的核心文件，包含镜像文件和链接文件，破坏后系统基本上就不能启动
<a name="HPUpQ"></a>
#### /bin/sbin
当装相关的软件或者安装包后，很多时候都会链接在这个目录下面，另外这里也存放了平时使用的各种 shell 命令。比如cp，ls，dd等<br />对于sbin，这里的 s 是 super 的意思，意味着需要超级用户才能执行的命令。常见磁盘分区 fdisk，创建文件系统的 mkfs就在这里
<a name="doeLM"></a>
#### /home
这个目录是系统中每个用户的工作目录，每个用户都会有自己主目录，通常名称为用户名，如果有个用户是小蓝，那么主目录就是xiaolan
<a name="fNA3K"></a>
#### /lib
在开发的过程中，很多共享库文件等很多放在这里，这个目录会包含引导进程所需要的静态库文件。后续在分享程序开发的时候会深入掰扯
<a name="ROQ46"></a>
#### /root
一般用户没有进入这个目录的权限，它是root的主目录
<a name="NcgcA"></a>
#### /lost+found
保存丢失的文件。什么意思，如果不恰当的关机操作，可能导致一些文件丢失，这些丢失的临时文件可能就会存放在这里。当重新启动的时候，引导程序就会运行 fsck 程序并发现这个文件
<a name="IBN5u"></a>
#### /tmp
用于存放临时文件，随时可能被删除
<a name="PkvQ9"></a>
## 3、CPU核心部件
Linux 的内核相当于 CPU 的大脑，经过了十几年的共同努力，现在已经到xxx核。<br />作为一个开源操作系统，也是一个难得的机会去学习这个成熟的操作系统，一个完整的计算机内核包含五个部分，分别是内存管理，进程管理，进程间通信，虚拟文件系统和网络接口，这一部分在后面的文章会单独介绍，先了解个大概，如果看过之前的面试连载文章，很多时候都会涉及到这些问题，这么说吧，在面试中的百分之八十的问题，都可以联系到操作系统层面，或者和操作系统相关知识点，相关设计理念巧合，当然不是巧合，就是学习过往成熟的经验罢了
<a name="GGaZy"></a>
### 内存管理
在区区的小内存环境下，可以运行很多的软件。Linux 内存管理中支持虚拟内存，即在系统上可以运行所有程序占用的内存总量大于实际物理内存，多余的这部分内存是从磁盘申请所得，在内存紧缺的情况下，内存管理就会负责在磁盘和内存之间进行交换。
<a name="EA5cg"></a>
### 进程管理
Linux 可多任务运行，那么在单CPU的情况下怎么支持多任务呢。这个任务就得交给进程调度来完成了。每当系统运行的时候，调度器会分配一个时间片，然后调度器根据不同的时间片选择进程运行。比如A进程的时间片用完了，那么调度器会选择一个新的进程继续运行，由于这个切换的速度和频率都很快，以致于感觉是多个程序在同时的运行，实际上CPU 在同一个时间只有一个进程在运行。
<a name="GyGmK"></a>
### 进程间通信
各个进程的友好相处才能共建繁华生态嘛。不同的进程有自己的进程空间，要通信就得找只隔壁大哥内核中专转。在一般情况下，一个进程等待硬件操作完成就会被挂起，硬件操作完毕进程恢复执行，这个协调过程就是进程间的通信机制了
<a name="u9tPd"></a>
### 虚拟文件系统
为了让众多的文件系统有一个统一的接口，采用了通用的文件模型---虚拟文件系统。虚拟文件系统分为逻辑文件系统和设备驱动程序。其中逻辑文件系统如 ex2，ex3 等。设备驱动程序包含了硬件控制器所编写的设备驱动程序模块。
<a name="x4q1q"></a>
### 网络接口
首先需要知道每一种网络硬件设备都会对应相应的设备驱动程序，网络设备驱动程序主要负责与硬件设备来进行通信
<a name="uog3C"></a>
## 4、CPU运行机制
知道了CPU的目录结构，现在说说其运行机制，主要包括了 init 系统，系统的运行级别，系统的关闭方法
<a name="w3KT3"></a>
#### Linux 初始化init系统
启动首先从 Bios 开始，接下来 Linux 引导程序将内核载入内存进行内核初始化，随后启动 init 进程，这个进程是系统的第一个进程，负责产出其他的进程<br />仅仅让内核运行起来还没啥用处，还需要管理，控制这些进程，从而系统进入一个用户设定的用户模式，即 init 系统<br />起初采用 sysinit，它主要依赖于脚本，串行启动进程，最大的缺点就是慢，在服务器上看还没有什么影响，如果是在手机移动端，这得急死人，随后出现了systemd取代了慢的upstart<br />在 Wndows 中，系统有安全运行模式和正常运行的模式，在 Linux 中的运行级别更加的灵活<br />一共有七种运行级别，定义在 /etc/inittab 中，sysinit 会检查 /etc/inittab 文件查看是否含有 initdefault 来启动一个默认的运行级别

- 0:表示关机模式
- 1:单用户模式
- 6:重启模式，关闭所有的运行进程
- 2:表示多用户模式
- 3:表示多用户模式，支持nfs服务
- 4:表示基本不用的用户模式，可以实现特定的等了请求
- 5:表示完成多用户模式，默认等了到x-windows系统

这些数字和 init 结合就产生了相应的含义，比如init 1表示系统进入单用户模式，、init 6表示系统将重新启动<br />有了新的systemd管理体系以后，默认的运行级别通过软连接的方式实现。<br />查看系统的默认target，如下所示<br />![默认target](https://cdn.nlark.com/yuque/0/2022/png/396745/1650863508974-f92ee5a5-145d-4738-bb9a-8c44cb599457.png#clientId=u47a477fe-9cb5-4&from=paste&id=u9fe9c80a&originHeight=40&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ue570958c-ddbd-42ea-a97e-57204d155b2&title=%E9%BB%98%E8%AE%A4target "默认target")<br />如果需要修改运行级别和targe的对应关系，可以通过下面命令实现
```bash
rm -rf /etcsystemd/system/default.target
ls -sf /lib/systemd/system/multi-user.target /etc/systemd/system/defalut.target
```
<a name="QoxJV"></a>
## 5、系统关机过程
关机方式很多，比如init shutdown halt reboot，下面分别看看这些方式以及
<a name="XLXjy"></a>
### shutdown
才开始学习 Linux 操作系统，不要动不动的就拔电源，此时很多的进程正在如火如荼的进行，这一操作，很可能就导致场面混乱，很多进程数据丢失，严重一点可能进程间开始打架，导致硬件设备损坏<br />shutdown 是 shell 编写的程序，必须要超级用户才能执行。首先通过广播的方式告诉系统所有的用户，系统将在xx时间关闭，同时 login 指令冻结，意味着其他用户此时不能等了，时间一到，i就会发信号给shuinit程序吧，init程序就会改变运行级别
<a name="ngnSw"></a>
#### shutdown命令之shutdown常见参数

- -f:重新启动的时候不执行fsck
- -k：只是发送西悉尼给所有用户但不会真正关机
- -r：shutdown之后重启系统
- 时间：设置多长时间可以九天跑一次
- halt命令
- reboot

reboot和halt类似，只是reboot是关机后重启<br />halt参数

- -i:关机之前关闭所有的网络接口
- -n：halt执行的时候不会调用系统指令sync

init主要用户系统在不同运行级别的切换，切换工作是立即完成的
<a name="NgvGL"></a>
## 6、系统服务管理工具
在 Centos7 以后，一个非常大的改变即使用了 systemd 管理工具，它不仅可以完成系统初始化的工作，还可以对系统和服务进行管理。同时它也兼容了之前的 sysinit，之前的版本中，这些初始化脚本服务存在于 /etc/rc.d/init 中，现在被.service文件扩展结尾的代替<br />配置文件位于 /etc/systemd 目录下，配置工具的命令使用 /bin/sbin 目录下，备用的配置文件位于 /lib/systemd 目录，可以通过`rpm -ql systemd`查看所有的路径<br />另外，为了方便操作，systemd提供了强大的命令行工具systemctl，下面看看这些常见的启动停止服务的命令
<a name="Wqga6"></a>
### 启动 停止 重启服务
通过systemctl启动一个服务
```bash
systemctl start httpd.service
```
<a name="lP0rY"></a>
### 停掉此服务
```bash
systemctl stop httpd.service
```
<a name="LcFEN"></a>
### 重启服务
```bash
systemctl restart httpd.service
systemctl try-restart httpd.service#只会在服务已经运行的状态下重启服务
systemctl reload httpd.service#重新加载配置文件
```
<a name="mqPTq"></a>
### 关闭服务
```bash
systemctl disable httpd.service
```
<a name="fPeMc"></a>
### 如果要查看这个服务状态
```bash
systemctl status httpd.service
```
