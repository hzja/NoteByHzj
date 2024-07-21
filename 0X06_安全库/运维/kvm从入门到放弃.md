<a name="ypRW7"></a>
# 一、虚拟化技术概述
![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1604563112410-6ae64d3f-cf60-4106-ac74-989eb0ac24f3.png#align=left&display=inline&height=258&originHeight=515&originWidth=825&size=468178&status=done&style=none&width=412.5)<br />      虚拟化[Virtualization]技术最早出现在 20 世纪 60 年代的 IBM 大型机系统，在70年代的 System370 系列中逐渐流行起来，这些机器通过一种叫虚拟机监控器[Virtual Machine Monitor，VMM]的程序在物理硬件之上生成许多可以运行独立操作系统软件的虚拟机[Virtual Machine]实例。随着近年多核系统、集群、网格甚至云计算的广泛部署，虚拟化技术在商业应用上的优势日益体现，不仅降低了 IT 成本，而且还增强了系统安全性和可靠性，虚拟化的概念也逐渐深入到人们日常的工作与生活中。

     虚拟化是一个广义的术语，对于不同的人来说可能意味着不同的东⻄，这要取决他们所处的环境。在计算机科学领域中，虚拟化代表着对计算资源的抽象，而不仅仅局限于虚拟机的概念。例如对物理内存的抽象，产生了虚拟内存技术，使得应用程序认为其自身拥有连续可用的地址空间[Address Space]，而实际上，应用程序的代码和数据可能是被分隔成多个碎片⻚或段），甚至被交换到磁盘、闪存等外部存储器上，即使物理内存不足，应用程序也能顺利执行。
<a name="VZN1l"></a>
# 二、主流虚拟化方案介绍
 
<a name="UVVKP"></a>
## 1、虚拟化技术主要分类
平台虚拟化（Platform Virtualization）针对计算机和操作系统的虚拟化。<br />资源虚拟化（Resource Virtualization）针对特定的系统资源的虚拟化，比如内存、存储、网络资源等。应用程序虚拟化（Application Virtualization） 包括仿真、模拟、解释技术等。
<a name="7rLbo"></a>
## 2、平台虚拟化技术分类
   我们通常所说的虚拟化主要是指平台虚拟化技术，通过使用控制程序（Control Program，也被称为Virtual Machine Monitor 或Hypervisor），隐藏特定计算平台的实际物理特性，为用户提供抽象的、统一的、模拟的计算环境（称为虚拟机）。虚拟机中运行的操作系统被称为客户机操作系统（GuestOS），运行虚拟机监控器的操作系统被称为主机操作系统（Host OS），当然某些虚拟机监控器可以脱离操作系统直接运行在硬件之上（如 VMWARE 的 ESX 产品）。运行虚拟机的真实系统我们称之为主机系统。
<a name="1AGOS"></a>
### a、操作系统级虚拟化（Operating System Level Virtualization）
     在传统操作系统中，所有用户的进程本质上是在同一个操作系统的实例中运行，因此内核或应用程序的缺陷可能影响到其它进程。操作系统级虚拟化是一种在服务器操作系统中使用的轻量级的虚拟化技术，内核通过创建多个虚拟的操作系统实例（内核和库）来隔离不同的进程，不同实例中的进程完全不了解对方的存在。

比较著名的有 Solaris Container，FreeBSD Jail 和 OpenVZ 等。

 <br />    比如OPENVZ：这个平台是最便宜的VPS平台，在各个vps商哪里都是价格最低的。OPENVZ本身运行在linux之上，它通过自己的虚拟化技术把一个服务器虚拟化成多个可以分别安装操作系统的实例，这样的每一个实体就是一个VPS，从客户的⻆度来看这就是一个虚拟的服务器，可以等同看做一台独立的服务器。

   OPENVZ虚拟化出来的VPS只能安装linux操作系统，不能安装windows系统，比如Centos、Fedora、Gentoo、Debian等。不能安装windows操作系统是openvz的第一个缺点，需要使用windows平台的用户不能使用OPENVZVPS。

     OPENVZ的第二个缺点是OPENVZ不是完全的虚拟化，每个VPS账户共用母机内核，不能单独修改内核。好在绝大多少用户根本不需要修改内核，所以这个缺点对多数人可以忽略不计。而这一点也正是openvz的优点，这一共用内核特性使得openvz的效率最高，超过KVM、Xen、VMware等平台。在不超售的情况下，openvz是最快速效率最高的VPS平台。
<a name="mCygg"></a>
### b、部分虚拟化（Partial Virtualization）
       VMM 只模拟部分底层硬件，因此客户机操作系统不做修改是无法在虚拟机中运行的，其它程序可能也需要进行修改。在历史上，部分虚拟化是通往全虚拟化道路上的重要里程碑,最早出现在第一代的分时系统 CTSS 和 IBM M44/44X 实验性的分⻚系统中。
<a name="6ad09bb8"></a>
### c、全虚拟化（Full Virtualization）
    全虚拟化是指虚拟机模拟了完整的底层硬件，包括处理器、物理内存、时钟、外设等，使得为原始硬件设计的操作系统或其它系统软件完全不做任何修改就可以在虚拟机中运行

    操作系统与真实硬件之间的交互可以看成是通过一个预先规定的硬件接口进行的。全虚拟化 VMM 以完整模拟硬件的方式提供全部接口（同时还必须模拟特权指令的执行过程）。举例而言，x86 体系结构中，对于操作系统切换进程⻚表的操作，真实硬件通过提供一个特权CR3 寄存器来实现该接口，操作系统只需执行 "mov pgtable,%%cr3"汇编指令即可。<br /> <br />    全虚拟化 VMM 必须完整地模拟该接口执行的全过程。如果硬件不提供虚拟化的特殊支持，那么这个模拟过程将会十分复杂：一般而言，VMM 必须运行在最高优先级来完全控制主机系统，而 Guest OS 需要降级运行，从而不能执行特权操作。当 Guest OS 执行前面的特权汇编指令时，主机系统产生异常（General Protection Exception），执行控制权重新从 Guest OS转到 VMM 手中。VMM 事先分配一个变量作为影子 CR3 寄存器给 Guest OS，将 pgtable 代表的客户机物理地址（Guest Physical Address）填入影子 CR3 寄存器，然后 VMM 还需要pgtable 翻译成主机物理地址（Host Physical Address）并填入物理 CR3 寄存器，最后返回到 Guest OS中。随后 VMM 还将处理复杂的 Guest OS 缺⻚异常（Page Fault）。

    比较著名的全虚拟化 VMM 有 Microsoft Virtual PC、VMware Workstation、Sun VirtualBox、Parallels Desktop for Mac 和 QEMU。

<a name="N6Mks"></a>
### d、超虚拟化（Paravirtualization）

     这是一种修改 Guest OS 部分访问特权状态的代码以便直接与 VMM 交互的技术。在超虚拟化虚拟机中，部分硬件接口以软件的形式提供给客户机操作系统，这可以通过 Hypercall（VMM提供给 Guest OS 的直接调用，与系统调用类似）的方式来提供。例如，Guest OS 把切换⻚表的代码修改为调用 Hypercall 来直接完成修改影子 CR3 寄存器和翻译地址的工作。由于不需要产生额外的异常和模拟部分硬件执行流程，超虚拟化可以大幅度提高性能，比较著名的VMM 有 Denali、Xen

<a name="SiFlW"></a>
### e、硬件辅助虚拟化（Hardware-Assisted Virtualization）

    硬件辅助虚拟化是指借助硬件（主要是主机处理器）的支持来实现高效的全虚拟化。例如有了Intel-VT 技术的支持，Guest OS 和 VMM 的执行环境自动地完全隔离开来，Guest OS 有自己的""套寄存器"，可以直接运行在最高级别。因此在上面的例子中，Guest OS 能够执行修改⻚表的汇编指令。Intel-VT 和 AMD-V 是目前 x86 体系结构上可用的两种硬件辅助虚拟化技术。

     这种分类并不是绝对的，一个优秀的虚拟化软件往往融合了多项技术。例如 VMware Workstation是一个著名的全虚拟化的 VMM，但是它使用了一种被称为动态二进制翻译的技术把对特权状态的访问转换成对影子状态的操作，从而避免了低效的 Trap-And-Emulate 的处理方式，这与超虚拟化相似，只不过超虚拟化是静态地修改程序代码。对于超虚拟化而言，如果能利用硬件特性，那么虚拟机的管理将会大大简化，同时还能保持较高的性能。
<a name="zWSyP"></a>
# 三、KVM虚拟化技术简介
<a name="ccldW"></a>
## 1、KVM架构
![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1604564004354-599bb97e-aa99-434e-9e81-3e41c24df893.png#align=left&display=inline&height=535&originHeight=1070&originWidth=897&size=658460&status=done&style=none&width=448.5)
<a name="3Tyc2"></a>
## 2、KVM架构解析
      <br />      从rhel6开始使用，红帽公司直接把KVM的模块做成了内核的一部分。xen用在rhel6之前的企业版中默认内核不支持，需要重新安装带xen功能的内核KVM 针对运行在 x86 硬件上的、驻留在内核中的虚拟化基础结构。KVM 是第一个成为原生 Linux 内核（2.6.20）的一部分的 hypervisor，它是由 Avi Kivity 开发和维护的，现在归 Red Hat 所有。这个 hypervisor 提供 x86 虚拟化，同时拥有到 PowerPC® 和 IA64 的通道。另外，KVM 最近还添加了对对称多处理（SMP）主机（和来宾）的支持，并且支持企业级特性，比如活动迁移（允许来宾操作系统在物理服务器之间迁移）。

    KVM 是作为内核模块实现的，因此 Linux 只要加载该模块就会成为一个hypervisor。KVM 为支持hypervisor  指令的硬件平台提供完整的虚拟化（比如 Intel® Virtualization Technology [Intel VT] 或AMD  Virtualization [AMD-V] 产品）。KVM 还支持准虚拟化来宾操作系统，包括 Linux 和 Windows®。这种技术由两个组件实现。第一个是可加载的 KVM 模块，当在 Linux 内核安装该模块之后，它就可以管理虚拟化硬件，并通过 /proc  文件系统公开其功能。第二个组件用于 PC 平台模拟，它是由修改版QEMU 提供的。QEMU  作为用户空间进程执行，并且在来宾操作系统请求方面与内核协调

    当新的操作系统在 KVM 上启动时（通过一个称为 KVM 的实用程序），它就成为宿主操作系统的一个进程，因此就可以像其他进程一样调度它。但与传统的 Linux 进程不一样，来宾操作系统被 hypervisor标识为处于 "来宾" 模式（独立于内核和用户模式）。每个来宾操作系统都是通过 /dev/KVM 设备映射的，它们拥有自己的虚拟地址空间，该空间映射到主机内核的物理地址空间。如前所述，KVM 使用底层硬件的虚拟化支持来提供完整的（原生）虚拟化。I/O请求通过主机内核映射到在主机上（hypervisor）执行的 QEMU 进程。KVM 在 Linux 环境中以主机的方式运行，不过只要底层硬件虚拟化支持，它就能够支持大量的来宾操作系统.
<a name="ayfwm"></a>
# 四、KVM软件安装
<a name="fjyLw"></a>
## 1、环境准备
至少一台能平通外网的centos7.8的机器
<a name="EUXVv"></a>
## 2、查看CPU是否支持VT技术
```c
[root@kvm-server iso]# cat /proc/cpuinfo | grep -E 'vmx|svm'
flags           : fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush mmx fxsr sse sse2 ss syscall nx pdpe1gb rdtscp lm constant_tsc arch_perfmon nopl xtopology tsc_reliable nonstop_tsc eagerfpu pni pclmulqdq vmx ssse3 fma cx16 pcid sse4_1 sse4_2 x2apic movbe popcnt tsc_deadline_timer aes xsave avx f16c rdrand hypervisor lahf_lm abm 3dnowprefetch invpcid_single ssbd ibrs ibpb stibp tpr_shadow vnmi ept vpid fsgsbase tsc_adjust bmi1 avx2 smep bmi2 invpcid mpx rdseed adx smap clflushopt xsaveopt xsavec arat spec_ctrl intel_stibp flush_l1d arch_capabilities
flags           : fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush mmx fxsr sse sse2 ss syscall nx pdpe1gb rdtscp lm constant_tsc arch_perfmon nopl xtopology tsc_reliable nonstop_tsc eagerfpu pni pclmulqdq vmx ssse3 fma cx16 pcid sse4_1 sse4_2 x2apic movbe popcnt tsc_deadline_timer aes xsave avx f16c rdrand hypervisor lahf_lm abm 3dnowprefetch invpcid_single ssbd ibrs ibpb stibp tpr_shadow vnmi ept vpid fsgsbase tsc_adjust bmi1 avx2 smep bmi2 invpcid mpx rdseed adx smap clflushopt xsaveopt xsavec arat spec_ctrl intel_stibp flush_l1d arch_capabilities
[root@kvm-server iso]#

```
<a name="0cf5154d"></a>
## 3、清理环境：卸载KVM
```c
[root@kvm-server iso]# yum remove `rpm -qa | egrep 'qemu|virt|KVM'` -y
[root@kvm-server iso]# rm -rf /var/lib/libvirt  /etc/libvirt/
```
<a name="xZCpm"></a>
## 4、安装软件
```c
[root@kvm-server iso]# yum install  *qemu*  *virt*  librbd1-devel -y
```
<a name="hgNfU"></a>
## 5、启动服务
```c
[root@kvm-server iso]# systemctl start libvirtd
```
<a name="K3qnh"></a>
## 6、查看KVM模块加载
```c
[root@kvm-server iso]# lsmod |grep -i kvm
kvm_intel             188688  9
kvm                   636965  1 kvm_intel
irqbypass              13503  7 kvm
[root@kvm-server iso]#

```
<a name="Hpggu"></a>
# 五、kvm虚拟机安装
<a name="pxMSW"></a>
## 1、图形化界面安装
![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1604630041022-7c94f59d-e5ac-4481-be76-549a9d8f2bb1.png#align=left&display=inline&height=219&originHeight=437&originWidth=401&size=32340&status=done&style=none&width=200.5)![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1604630082466-c70b7c21-30df-4013-a7b5-428bb8806bc4.png#align=left&display=inline&height=218&originHeight=435&originWidth=471&size=36922&status=done&style=none&width=235.5)![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1604630099915-8574c9e1-3c84-4228-ba6d-3afdc1a6448a.png#align=left&display=inline&height=217&originHeight=434&originWidth=399&size=25148&status=done&style=none&width=199.5)![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1604630126932-751e4496-fc39-47bb-b205-4fdd8c233a93.png#align=left&display=inline&height=220&originHeight=440&originWidth=399&size=29277&status=done&style=none&width=199.5)![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1604630145997-eb5a88bb-fd66-4742-8d72-d7e78759a34e.png#align=left&display=inline&height=224&originHeight=448&originWidth=402&size=35708&status=done&style=none&width=201)
<a name="4lMSy"></a>
## 2、auto_ui安装系统
这个方式类似于pxe装机模式、将安装好的机器上的anaconda-ks.cfg（将其重命名为auto.ks）作为新建机器的启动文件，这种方式装机的缺点就是会调用virt-viewer界面安装不够智能化
```c
[root@kvm-server auto]# virt-install -n auto_ui --memory 1024 --vcpus 1 --disk size=8 --location CentOS-7-x86_64-Minimal-2003.iso --os-variant centos7.0 --initrd-inject=auto.ks --extra-args "ks=file:/auto.ks"
```
执行命令之后示意图<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1604630667397-e14ad342-63ab-4999-bc9d-e9f916339639.png#align=left&display=inline&height=82&originHeight=164&originWidth=1865&size=28170&status=done&style=none&width=932.5)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1604630726899-2bb4d257-872a-42b9-a20a-c9276501864a.png#align=left&display=inline&height=379&originHeight=824&originWidth=1282&size=173563&status=done&style=none&width=589)![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1604631143708-482454bf-64e1-44e7-b6d9-da0b4c61c28d.png#align=left&display=inline&height=381&originHeight=952&originWidth=1282&size=180368&status=done&style=none&width=513)
<a name="dJuQo"></a>
## 3、auto_con自动化装机
需要的是修改auto.ks的配置文件这种方式
```c
[root@kvm-server auto]# cat auto.ks
#version=DEVEL
# System authorization information
auth --enableshadow --passalgo=sha512
# Use CDROM installation media
cdrom
eula --agreed   #修改安装方式
reboot          #增加这行
# Use graphical install
text            #修改这行
# Run the Setup Agent on first boot
firstboot --enable
ignoredisk --only-use=vda
# Keyboard layouts
keyboard --vckeymap=us --xlayouts='us'
# System language
lang en_US.UTF-8

# Network information
network  --bootproto=dhcp --device=eth0 --onboot=yes --ipv6=auto --no-activate          #将off改为yes
network  --hostname=kvm-jiangjw         #修改主机名称

# Root password
rootpw --iscrypted $6$fFbCeFDOEPb6oihd$wblmecxj4m3xzwt2aU7S5yDMFIQhWYViwq3sqCRBbz0ltt91KJXkGyngwr3tsBQGFl8RKcubmqwOPXW/VFfHp0
# System services
services --enabled="chronyd"
# System timezone
timezone America/New_York --isUtc
user --name=test --password=$6$aKawmDzjjdBJfrNv$864seC..gUngPDsnH9XCBNBnXOJioJAiJKnfpVsSotWM/vCBKpoB59ux0GpFN5a4Y74Wi8EibufCKk9E.y9m6. --iscrypted --gecos="test"
# System bootloader configuration
bootloader --append=" crashkernel=auto" --location=mbr --boot-drive=vda
autopart --type=lvm
# Partition clearing information
clearpart --none --initlabel

%packages
@^minimal
@core
chrony
kexec-tools

%end

%addon com_redhat_kdump --enable --reserve-mb='auto'

%end

%anaconda
pwpolicy root --minlen=6 --minquality=1 --notstrict --nochanges --notempty
pwpolicy user --minlen=6 --minquality=1 --notstrict --nochanges --emptyok
pwpolicy luks --minlen=6 --minquality=1 --notstrict --nochanges --notempty
%end

```
创建虚拟机命令
```c
[root@kvm-server auto]# virt-install -n auto_con --memory 1024 --vcpus 1 --disk size=8 --location CentOS-7-x86_64-Minimal-2003.iso --os-variant centos7.0 --initrd-inject=auto.ks --extra-args "console=ttyS0 ks=file:/auto.ks" --console pty,target_type=serial --graphics none
```
执行命令之后截图整个过程消耗的时间比较长耐心等待<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1604631695234-aaf0c0e7-9f72-405b-86df-23026ba02eec.png#align=left&display=inline&height=222&originHeight=444&originWidth=514&size=53562&status=done&style=none&width=257)![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1604631837721-9520c06e-51f2-4b7c-98b4-cfb12e93ba6e.png#align=left&display=inline&height=220&originHeight=439&originWidth=477&size=33282&status=done&style=none&width=238.5)![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1604631943282-6f91e601-424d-4155-9240-4558c609bf11.png#align=left&display=inline&height=221&originHeight=488&originWidth=617&size=54119&status=done&style=none&width=280)
<a name="Zx7zo"></a>
# 六、配置文件
<a name="3xUC4"></a>
## 1、虚拟机启动相关文件
```c
[root@kvm-server ~]# ls -l /var/lib/libvirt/
total 8
drwx--x--x.  2 root root    6 Nov  5 21:58 boot
drwxr-xr-x.  2 root root 4096 Nov  5 22:27 dnsmasq
drwx--x--x.  2 root root    6 May 12 12:30 filesystems
drwx--x--x.  2 root root   69 Nov  5 21:58 images			#实际上是虚拟文件
drwx------.  2 root root    6 May 12 12:30 lxc
drwx------.  2 root root    6 May 12 12:30 network
drwxr-x--x. 11 qemu qemu 4096 Nov  5 22:04 qemu
drwx--x--x.  2 root root    6 May 12 12:30 swtpm
[root@kvm-server ~]#
```
```c
[root@kvm-server ~]# ls -l /var/lib/libvirt/images/			#可以看到就是我们创建虚拟机生成的系统文件
total 13767700
-rw-------. 1 qemu qemu  8591507456 Nov  5 22:29 auto_con.qcow2
-rw-------. 1 qemu qemu  8591507456 Nov  5 22:34 auto_ui.qcow2
-rw-------. 1 qemu qemu 10739318784 Nov  5 22:35 centos7.0.qcow2
[root@kvm-server ~]#
```
<a name="SetWN"></a>
## 2、虚拟机的配置文件
```c
[root@kvm-server ~]# cd  /etc/libvirt/qemu					#创建好虚拟机之后生成的配置文件
[root@kvm-server qemu]# ls -l
total 20
-rw-------. 1 root root 3703 Nov  5 21:58 auto_con.xml
-rw-------. 1 root root 4609 Nov  5 21:44 auto_ui.xml
-rw-------. 1 root root 4615 Nov  5 07:15 centos7.0.xml
drwx------. 3 root root   40 Nov  5 06:57 networks
[root@kvm-server qemu]#
```
配置文件记录了虚拟机的各种信息，比如CPU、存储、网络等。KVM虚拟机的配置文件在 /下，为xml文件<br />整体结构如下：<br />  <domain type='kvm'><br />      虚拟机整体信息<br />      系统信息<br />      硬件资源特性<br />      突发事件处理<br />      虚拟磁盘（单个或多个）<br />      虚拟光盘（可选）<br />      虚拟网络（单个或多个）<br />      vnc/spice配置<br /> </domain>
<a name="st3XW"></a>
### a、查看虚拟机的配置文件-1
```c
[root@kvm-server ~]# virsh dumpxml auto_con
<domain type='kvm' id='1'>
  <name>auto_con</name>
  <uuid>448c9abc-a1d5-4b88-9195-288f6b160508</uuid>
  <memory unit='KiB'>1048576</memory>
  <currentMemory unit='KiB'>1048576</currentMemory>
  <vcpu placement='static'>1</vcpu>
  <resource>
    <partition>/machine</partition>
  </resource>
  <os>
    <type arch='x86_64' machine='pc-i440fx-rhel7.0.0'>hvm</type>
    <boot dev='hd'/>
  </os>
  <features>
    <acpi/>
    <apic/>
  </features>
  <cpu mode='custom' match='exact' check='full'>
    <model fallback='forbid'>Broadwell</model>
    <feature policy='disable' name='hle'/>
    <feature policy='disable' name='rtm'/>
    <feature policy='require' name='hypervisor'/>
    <feature policy='disable' name='erms'/>
    <feature policy='require' name='xsaveopt'/>
  </cpu>
```
<a name="e2yVC"></a>
### b、查看虚拟机的配置文件-2
```c
[root@kvm-server ~]# virsh edit auto_con
<domain type='kvm' id='1'>
  <name>auto_con</name>
  <uuid>448c9abc-a1d5-4b88-9195-288f6b160508</uuid>
  <memory unit='KiB'>1048576</memory>
  <currentMemory unit='KiB'>1048576</currentMemory>
  <vcpu placement='static'>1</vcpu>
  <resource>
    <partition>/machine</partition>
  </resource>
  <os>
    <type arch='x86_64' machine='pc-i440fx-rhel7.0.0'>hvm</type>
    <boot dev='hd'/>
  </os>
  <features>
    <acpi/>
    <apic/>
  </features>
```
<a name="vFhNl"></a>
### c、查看虚拟机的配置文件-3
```c
[root@kvm-server ~]# cat /etc/libvirt/qemu/auto_con.xml
<!--
WARNING: THIS IS AN AUTO-GENERATED FILE. CHANGES TO IT ARE LIKELY TO BE
OVERWRITTEN AND LOST. Changes to this xml configuration should be made using:
  virsh edit auto_con
or other application using the libvirt API.
-->

<domain type='kvm'>
  <name>auto_con</name>
  <uuid>448c9abc-a1d5-4b88-9195-288f6b160508</uuid>
  <memory unit='KiB'>1048576</memory>
  <currentMemory unit='KiB'>1048576</currentMemory>
  <vcpu placement='static'>1</vcpu>
  <os>
    <type arch='x86_64' machine='pc-i440fx-rhel7.0.0'>hvm</type>
    <boot dev='hd'/>
  </os>
```
<a name="TdQua"></a>
# 七、虚拟机的管理
<a name="mT60Q"></a>
## 1、基础命令
```c
[root@kvm-server ~]# virsh list						#查看当前正在运行的虚拟机
 Id    Name                           State
----------------------------------------------------

[root@kvm-server ~]# virsh list --all				#查看所有虚拟机
 Id    Name                           State
----------------------------------------------------
 -     auto_con                       shut off
 -     auto_ui                        shut off
 -     centos7.0                      shut off
 -     kvm-cp                         shut off
[root@kvm-server images]# cp -a auto_ui.qcow2 auto_con.qcow2
[root@kvm-server images]# virsh start auto_con 		#启动虚拟机
Domain auto_con started

[root@kvm-server images]# virsh shutdown 			#关闭虚拟机
Domain auto_con is being shutdown

[root@kvm-server images]#
[root@kvm-server images]# virsh dumpxml auto_con	#查看当前机器的xml配置文件
<domain type='kvm'>
  <name>auto_con</name>
  <uuid>448c9abc-a1d5-4b88-9195-288f6b160508</uuid>
  <memory unit='KiB'>1048576</memory>
  <currentMemory unit='KiB'>1048576</currentMemory>
  <vcpu placement='static'>1</vcpu>
[root@kvm-server ~]# virsh suspend auto_con			#暂停相当于vm中的挂起备注要是centos机器关机了kvm中挂起的机器会被关机
Domain auto_con suspended

[root@kvm-server ~]# virsh resume auto_con			#相当于是取消挂起开启的状态
Domain auto_con resumed

[root@kvm-server ~]# virsh destroy auto_con			#强制关机可能会导致虚拟可能会导致虚拟机下次不一定能正常启动，运用于一些不能正常关机的机器
Domain auto_con destroyed

[root@kvm-server ~]# virsh reboot auto_con			#重启虚拟机
Domain auto_con is being rebooted				

[root@kvm-server ~]# virsh reset auto_con			#重置虚拟机、类似于将虚拟机恢复出厂设置
Domain auto_con was reset



#删除虚拟机的配置文件没有删除虚拟机的镜像文件、如果在删除之前对配置文件做了备份是可以重启创建出undefine的机器
[root@kvm-server ~]# ls /etc/libvirt/qemu/
auto_con.xml  auto_ui.xml  centos7.0.xml  kvm-cp.xml  networks
[root@kvm-server ~]#
[root@kvm-server ~]# ls /var/lib/libvirt/images/
auto_con.qcow2  auto_ui.qcow2  centos7.0.qcow2
[root@kvm-server ~]#

[root@kvm-server ~]# virsh undefine auto_con		#删除虚拟机的配置文件没有删除虚拟机的镜像文件、如果在删除之前对配置文件做了备份是可以重启创建出undefine的机器
Domain auto_con has been undefined

[root@kvm-server ~]# ls /etc/libvirt/qemu/			#可以看到的是已经没有auto_con的这个虚拟机的配置文件
auto_ui.xml  centos7.0.xml  kvm-cp.xml  networks
[root@kvm-server ~]# ls /var/lib/libvirt/images/	#但是还是可以看到虚拟机的镜像文件
auto_con.qcow2  auto_ui.qcow2  centos7.0.qcow2
[root@kvm-server ~]#
```
<a name="qtliO"></a>
## 2、获取虚拟机的ip
```c
[root@kvm-server ~]# virsh domifaddr auto_ui				#方法1获取IP地址
 Name       MAC address          Protocol     Address
-------------------------------------------------------------------------------
 vnet0      52:54:00:13:3a:92    ipv4         192.168.124.105/24

[root@kvm-server ~]# ssh test@192.168.124.105
test@192.168.124.105's password:
[test@kvm-jiangjw ~]$
[root@kvm-server ~]# virsh dumpxml auto_ui |grep mac		#方法2获取IP地址
    <partition>/machine</partition>
    <type arch='x86_64' machine='pc-i440fx-rhel7.0.0'>hvm</type>
      <mac address='52:54:00:13:3a:92'/>
[root@kvm-server ~]# arp -a|grep 52:54:00:13:3a:92
? (192.168.124.105) at 52:54:00:13:3a:92 [ether] on virbr0
[root@kvm-server ~]#

```
<a name="r4wn4"></a>
## 3、配置console登录
在使用ssh登录的时候发现登录的时间消耗非常的长解决这个问题我们采取console的登录方式
```c
[root@kvm-server ~]# virsh domifaddr auto_ui
 Name       MAC address          Protocol     Address
-------------------------------------------------------------------------------
 vnet0      52:54:00:13:3a:92    ipv4         192.168.124.105/24

[root@kvm-server ~]# ssh test@192.168.124.105
test@192.168.124.105's password:
Last login: Sun Nov  8 22:11:59 2020 from gateway
[test@kvm-jiangjw ~]$ su - root
Password:
Last login: Sun Nov  8 22:10:02 EST 2020 on tty1
[root@kvm-jiangjw ~]# systemctl enable  serial-getty@ttyS0.service
[root@kvm-jiangjw ~]# systemctl start serial-getty@ttyS0.service
[root@kvm-jiangjw ~]# exit
[root@kvm-server ~]# virsh console auto_ui
Connected to domain auto_ui
Escape character is ^]

CentOS Linux 7 (Core)
Kernel 3.10.0-1127.el7.x86_64 on an x86_64

kvm-jiangjw login: root
Password:
Last login: Sun Nov  8 22:22:17 on pts/0
[root@kvm-jiangjw ~]#			#此时不能使用exit退出、使用ctrl+]才能正常的退出
```
<a name="1uhO1"></a>
## 4、虚拟机添加设备
图形化界面<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1604892738404-85397746-4244-4ef8-bef8-a3d3bb5562bf.png#align=left&display=inline&height=340&originHeight=679&originWidth=833&size=96046&status=done&style=none&width=416.5)
<a name="4be1ba5b"></a>
### a、手动创建修改虚拟机
```c
[root@kvm-server ~]# cp /etc/libvirt/qemu/kvm-cp.xml /etc/libvirt/qemu/kvm-man.xml
[root@kvm-server ~]# cp /var/lib/libvirt/images/centos7.0.qcow2 /var/lib/libvirt/kvm-man.xml
[root@kvm-server ~]# grep -n "kvm-man" /etc/libvirt/qemu/kvm-man.xml	#主要修改的是主机名称镜像uuid等信息
4:  virsh edit kvm-man
9:  <name>kvm-man</name>
41:      <source file='/var/lib/libvirt/images/kvm-man.qcow2'/>
[root@kvm-server ~]#
[root@kvm-server ~]# virsh define /etc/libvirt/qemu/kvm-man.xml
[root@kvm-server ~]# virsh domifaddr kvm-man
 Name       MAC address          Protocol     Address
-------------------------------------------------------------------------------
 vnet0      52:54:00:73:1c:d6    ipv4         192.168.124.117/24
[root@kvm-server ~]# ssh root@192.168.124.117
The authenticity of host '192.168.124.117 (192.168.124.117)' can't be established.
ECDSA key fingerprint is SHA256:gh1Ev3WkLPnrXtFa+RRJDMsf1Qt9NFoIVLHDDMI3I48.
ECDSA key fingerprint is MD5:bc:9b:bd:04:61:cd:d2:1c:19:28:92:06:a6:6b:da:bb.
Are you sure you want to continue connecting (yes/no)? yes
Warning: Permanently added '192.168.124.117' (ECDSA) to the list of known hosts.
root@192.168.124.117's password:
Last failed login: Mon Nov  9 01:00:08 EST 2020 from gateway on ssh:notty
There was 1 failed login attempt since the last successful login.
Last login: Thu Nov  5 21:51:01 2020
[root@kvm-jiangjw ~]# ip a
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
    inet 127.0.0.1/8 scope host lo
       valid_lft forever preferred_lft forever
    inet6 ::1/128 scope host
       valid_lft forever preferred_lft forever
2: eth0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast state UP group default qlen 1000
    link/ether 52:54:00:73:1c:d6 brd ff:ff:ff:ff:ff:ff
    inet 192.168.124.117/24 brd 192.168.124.255 scope global noprefixroute dynamic eth0
       valid_lft 3471sec preferred_lft 3471sec
    inet6 fe80::116a:5e00:16fd:7e14/64 scope link noprefixroute
       valid_lft forever preferred_lft forever
[root@kvm-jiangjw ~]# exi
```
<a name="ppTpH"></a>
### b、手动添加添加2G的硬盘
```c
[root@kvm-server ~]# virsh shutdown kvm-man
Domain kvm-man is being shutdown
[root@kvm-server ~]# grep -A 5 'disk ' /etc/libvirt/qemu/kvm-man.xml
    <suspend-to-disk enabled='no'/>
  </pm>
  <devices>
    <emulator>/usr/libexec/qemu-kvm</emulator>
    <disk type='file' device='disk'>
      <driver name='qemu' type='qcow2'/>
      <source file='/var/lib/libvirt/images/kvm-man.qcow2'/>
      <target dev='vda' bus='virtio'/>
      <address type='pci' domain='0x0000' bus='0x00' slot='0x06' function='0x0'/>
    </disk>
    <disk type='file' device='disk'>
      <driver name='qemu' type='qcow2'/>
      <source file='/var/lib/libvirt/images/kvm-man-2.qcow2'/>						#添加磁盘信息
      <target dev='vdb' bus='virtio'/>												#添加磁盘名称注意磁盘名称vdb是全局唯一
      <address type='pci' domain='0x0000' bus='0x00' slot='0x09' function='0x0'/>	#注意修改磁盘插槽0x09'是全局唯一的
    </disk>
[root@kvm-server ~]# qemu-img create -f qcow2 /var/lib/libvirt/images/kvm-man-2.qcow2 2G		#创建一个2G大小的磁盘
Formatting '/var/lib/libvirt/images/kvm-man-2.qcow2', fmt=qcow2 size=2147483648 encryption=off cluster_size=65536 lazy_refcounts=off
[root@kvm-server ~]# virsh define /etc/libvirt/qemu/kvm-man.xml
Domain kvm-man defined from /etc/libvirt/qemu/kvm-man.xml

[root@kvm-server ~]# virsh start kvm-man
Domain kvm-man started

[root@kvm-server ~]# virsh domifaddr kvm-man
 Name       MAC address          Protocol     Address
-------------------------------------------------------------------------------
 vnet0      52:54:00:73:1c:d6    ipv4         192.168.124.117/24

[root@kvm-server ~]# ssh root@192.168.124.117
root@192.168.124.117's password:
Last login: Mon Nov  9 01:00:11 2020 from gateway
[root@kvm-jiangjw ~]# lsblk
NAME                         MAJ:MIN RM  SIZE RO TYPE MOUNTPOINT
sr0                           11:0    1 1024M  0 rom
vda                          252:0    0    8G  0 disk
├─vda1                       252:1    0    1G  0 part /boot
└─vda2                       252:2    0    7G  0 part
  ├─centos_kvm--jiangjw-root 253:0    0  6.2G  0 lvm  /
  └─centos_kvm--jiangjw-swap 253:1    0  820M  0 lvm  [SWAP]
vdb                          252:16   0    2G  0 disk
```
qcow2和raw格式的对比：<br />qcow2 格式的文件虽然在性能上比raw 格式的有一些损失，但是 qcow2 格式的镜像比 raw 格式文件更小，只有在虚拟机实际占用了磁盘空间时，其文件才会增长，能方便的减少迁移花费的流量，更适用于云计算系统，同时，它还具有加密，压缩，以及快照等 raw 格式不具有的功能。

<a name="lTMQF"></a>
### c、手动创建两块网卡
```c
[root@kvm-server ~]# openssl rand -hex 3 |sed -r 's/..\B/&:/g'			#手动生成mac的设备地址
3f:63:5e
[root@kvm-server ~]# grep -A 5 "interface" /etc/libvirt/qemu/kvm-man.xml
    <interface type='network'>
      <mac address='52:54:00:73:1c:d6'/>
      <source network='default'/>
      <model type='virtio'/>
      <address type='pci' domain='0x0000' bus='0x00' slot='0x03' function='0x0'/>
    </interface>
    <interface type='network'>
      <mac address='52:54:00:3f:63:5e'/>	#mac地址的后面三段换成上面生成的设备地址								
      <source network='default'/>
      <model type='virtio'/>
      <address type='pci' domain='0x0000' bus='0x00' slot='0x13' function='0x0'/>		#总线插槽地址换成全局唯一的0x13不然会造成地址冲突
[root@kvm-server ~]# virsh define /etc/libvirt/qemu/kvm-man.xml
Domain kvm-man defined from /etc/libvirt/qemu/kvm-man.xml

[root@kvm-server ~]# virsh start kvm-man
Domain kvm-man started

[root@kvm-server ~]# virsh domifaddr kvm-man
 Name       MAC address          Protocol     Address
-------------------------------------------------------------------------------
 vnet0      52:54:00:73:1c:d6    ipv4         192.168.124.117/24

[root@kvm-server ~]# ssh root@192.168.124.117
root@192.168.124.117's password:
Last login: Mon Nov  9 01:23:29 2020 from gateway
[root@kvm-jiangjw ~]# ip a
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
    inet 127.0.0.1/8 scope host lo
       valid_lft forever preferred_lft forever
    inet6 ::1/128 scope host
       valid_lft forever preferred_lft forever
2: eth0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast state UP group default qlen 1000
    link/ether 52:54:00:73:1c:d6 brd ff:ff:ff:ff:ff:ff
    inet 192.168.124.117/24 brd 192.168.124.255 scope global noprefixroute dynamic eth0
       valid_lft 3588sec preferred_lft 3588sec
    inet6 fe80::116a:5e00:16fd:7e14/64 scope link noprefixroute
       valid_lft forever preferred_lft forever
3: eth1: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast state UP group default qlen 1000
    link/ether 52:54:00:3f:63:5e brd ff:ff:ff:ff:ff:ff			#可以看到的是我们添加的网卡的mac地址为我们手动指定的mac地址
    inet 192.168.124.90/24 brd 192.168.124.255 scope global noprefixroute dynamic eth1
       valid_lft 3591sec preferred_lft 3591sec
    inet6 fe80::78de:191f:af2:fdfc/64 scope link noprefixroute
       valid_lft forever preferred_lft forever
[root@kvm-jiangjw ~]#exit
logout
Connection to 192.168.124.117 closed.
[root@kvm-server ~]# ssh root@192.168.124.90
The authenticity of host '192.168.124.90 (192.168.124.90)' can't be established.
ECDSA key fingerprint is SHA256:gh1Ev3WkLPnrXtFa+RRJDMsf1Qt9NFoIVLHDDMI3I48.
ECDSA key fingerprint is MD5:bc:9b:bd:04:61:cd:d2:1c:19:28:92:06:a6:6b:da:bb.
Are you sure you want to continue connecting (yes/no)? yes
Warning: Permanently added '192.168.124.90' (ECDSA) to the list of known hosts.
root@192.168.124.90's password:
Last login: Mon Nov  9 01:47:11 2020 from gateway
[root@kvm-jiangjw ~]#

```
<a name="Yl2De"></a>
### d、通过agent获取信息

```c
[root@kvm-server ~]# virsh list --all
 Id    Name                           State
----------------------------------------------------
 -     auto_ui                        shut off
 -     centos7.0                      shut off
 -     kvm-cp                         shut off
 -     kvm-man                        shut off

[root@kvm-server ~]# virsh start centos7.0
Domain centos7.0 started

[root@kvm-server ~]# virsh console centos7.0
Connected to domain centos7.0
Escape character is ^]

CentOS Linux 7 (Core)
Kernel 3.10.0-1127.el7.x86_64 on an x86_64

kvm-jw login: root
Password:
Last login: Mon Nov  9 15:07:38 on ttyS0
[root@kvm-jw ~]# yum install qemu-guest-agent
然后按住ctrl+]
[root@kvm-server ~]#  virsh domifaddr centos7.0		#查看机器的IP地址
 Name       MAC address          Protocol     Address
-------------------------------------------------------------------------------
 vnet0      52:54:00:7d:48:5c    ipv4         192.168.124.51/24

[root@kvm-server ~]#  virsh domfsinfo centos7.0		#查看机器的磁盘信息
Mountpoint                           Name     Type     Target
-------------------------------------------------------------------
/                                    dm-0     xfs      vda
/boot                                vda1     xfs      vda

[root@kvm-server ~]#  virsh set-user-password centos7.0 root 1234	#重置机器的root用户密码
Password set successfully for root in centos7.0
[root@kvm-server ~]#
[root@kvm-server ~]# virsh domblklist centos7.0 --details			#查看机器磁盘的相关信息
Type       Device     Target     Source
------------------------------------------------
file       disk       vda        /var/lib/libvirt/images/centos7.0.qcow2
file       cdrom      hda        -

[root@kvm-server ~]#  virsh domblkstat centos7.0 --device hda --human	#查看指定磁盘（hda）的统计信息
Device: hda
 number of read operations:      3
 number of bytes read:           82
 number of write operations:     0
 number of bytes written:        0
 number of flush operations:     0
 total duration of reads (ns):   270740
 total duration of writes (ns):  0
 total duration of flushes (ns): 0

[root@kvm-server ~]#
```
<a name="Hgfp5"></a>
### e、虚拟机的克隆
克隆虚拟机一般会将一个几个机器作为一个标准的模板需要直接克隆做好的机器
```c
[root@kvm-jw ~]# timedatectl set-timezone Asia/Shanghai				#设置时区
timedatectl set-t
[root@kvm-jw ~]# timedatectl
      Local time: Tue 2020-11-10 08:10:57 CST
  Universal time: Tue 2020-11-10 00:10:57 UTC
        RTC time: Tue 2020-11-10 00:10:56
       Time zone: Asia/Shanghai (CST, +0800)
     NTP enabled: yes
NTP synchronized: no
 RTC in local TZ: no
      DST active: n/a
[root@kvm-jw ~]#
[root@kvm-jw ~]# systemctl restart network
[root@kvm-jw ~]# cat /etc/sysconfig/network-scripts/ifcfg-eth0
TYPE=Ethernet
BOOTPROTO=dhcp
DEVICE=eth0
ONBOOT=yes
[root@kvm-jw ~]# rm -rf /etc/ssh/ssh_host_*							#删除ssh的秘钥信息
[root@kvm-jw ~]#  rm -f /etc/udev/rules.d/70-persistent-net.rules	#删除网卡的默认规则
[root@kvm-server ~]# virt-clone  --original centos7.0 --name kvm-clone --auto-clone
Allocating 'kvm-clone.qcow2'                                                                                                                                      |  10 GB  00:02:25

Clone 'kvm-clone' created successfully.
[root@kvm-server ~]# ll /var/lib/libvirt/images/kvm-clone.qcow2
-rw-------. 1 root root 1656291328 Nov 10 05:57 /var/lib/libvirt/images/kvm-clone.qcow2
[root@kvm-server ~]# virsh list --all		
 Id    Name                           State
----------------------------------------------------
 -     auto_ui                        shut off
 -     centos7.0                      shut off
 -     kvm-clone                      shut off
 -     kvm-cp                         shut off
 -     kvm-man                        shut off

[root@kvm-server ~]# virsh start kvm-clone
Domain kvm-clone started

[root@kvm-server ~]# virsh console kvm-clone
Connected to domain kvm-clone
Escape character is ^]

CentOS Linux 7 (Core)
Kernel 3.10.0-1127.el7.x86_64 on an x86_64

kvm-jw login: root
Password:
Last login: Mon Nov  9 15:07:38 on ttyS0
[root@kvm-jw ~]#
[root@kvm-jw ~]# timedatectl								#查看通过模板机克隆出来的机器时区是否和原生机器一致
      Local time: Tue 2020-11-10 18:59:37 CST
  Universal time: Tue 2020-11-10 10:59:37 UTC
        RTC time: Tue 2020-11-10 10:59:36
       Time zone: Asia/Shanghai (CST, +0800)
     NTP enabled: yes
NTP synchronized: yes
 RTC in local TZ: no
      DST active: n/a
[root@kvm-jw ~]# cat /etc/sysconfig/network-scripts/ifcfg-eth0	#查看克隆的机器的网卡配置信息
TYPE=Ethernet
BOOTPROTO=dhcp
DEVICE=eth0
ONBOOT=yes
[root@kvm-jw ~]# ip a
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
    inet 127.0.0.1/8 scope host lo
       valid_lft forever preferred_lft forever
    inet6 ::1/128 scope host
       valid_lft forever preferred_lft forever
2: eth0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast state UP group default qlen 1000
    link/ether 52:54:00:de:32:ce brd ff:ff:ff:ff:ff:ff
    inet 192.168.124.222/24 brd 192.168.124.255 scope global noprefixroute dynamic eth0
       valid_lft 3524sec preferred_lft 3524sec
    inet6 fe80::5054:ff:fede:32ce/64 scope link
       valid_lft forever preferred_lft forever
[root@kvm-jw ~]#
```
<a name="vDEcw"></a>
# 八、虚拟机的高级管理










