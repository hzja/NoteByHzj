Docker<br />docker是什么？OCI又是什么？CRI又是什么？containerd又是什么？有没有感觉概念非常的乱？<br />造成这一切乱象的根本，是各个利益公司之间的竞争造成的。商业竞争造成了用户的困扰，尤其是每个公司都在推销自己技术概念的时候。
<a name="O6kQp"></a>
## Docker并不是全部
自从docker加入了镜像仓库，引爆了容器技术，很多人认为docker就代表了容器的全部。这种观点很普遍，主要是由于docker实在是太火了。即使k8s二次官宣抛弃docker，它的热度依然不减。<br />其实，现阶段，docker只是众多容器技术中的其中一种。它有三个主要的概念。

- **镜像**  代表了最终的软件包，不可变的软件载体。相当于安装文件
- **容器** 镜像的运行时，实际运行的实例，具有明确的进程号
- **仓库** 存放镜像的仓库，可以进行统一的版本管理和权限管理

docker是运行时和一堆开发工具集合的统称。docker-cli就不必多说了，就是一堆命令行的集合，主要看一下运行时。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1631252097591-6c7b74d6-1e7a-426d-9aa2-94f8faab2f32.webp#clientId=ubbefa5fa-1f93-4&from=paste&id=uc0ef1927&originHeight=704&originWidth=888&originalType=url&ratio=1&status=done&style=shadow&taskId=u820de8dd-70b5-4a8d-9e32-ace7b100665)

- **docker** 平常操作docker，使用的就是docker命令，它就是命令行接口，相当于一个客户端。它将指令发送到dockerd
- **dockerd** docker的服务端。比如在Linux上安装docker，就要启动一个常驻进程，才能管理所有的docker进程
- **containerd** 这个组件，是从Docker1.11版本才有的，是从dockerd里拆出来的，是容器标准化后的产物。它遵循的是OCI标准，这个标准后面还会用图来说明它的位置。containerd功能齐全，换句话说，服务器上可以没有dockerd，只需要containerd就能运行容器
- **runc** 容器运行时组件，是一个标准的OCI容器实现运行时，可用来直接创建和运行容器。可以看到containerd和runc都是OCI的实现，区别是前者是管理工具，后者是运行容器
- **containerd-shim** 垫片的意思，主要是将containerd与真正的容器进行解耦
- **ctr** 也叫做containerd-ctr，是containerd的客户端

可能会特别奇怪，仅仅一个小小的docker，怎么就分了这么多层！按照常理，俺只需要一个client端，一个守护进程就够了，怎么拆分了这么多层？<br />这是由于历史发展原因引起的。为了解耦，为了实现OCI标准，docker的组件不得不拆了又拆，最后形成了一个松耦合的架构。后来，由于k8s加入竞争，又出现了一个新的名词：CRI。接下来，综合k8s，来看一下各个组件所存在的层次。
<a name="Izypg"></a>
## 两个标准割裂世界
为什么搞得这么复杂，主要是因为两个标准的加入。CRI和OCI。下面这张大图，大体体现了它们之间的关系。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1631252097546-75949255-bb45-4b3a-982d-412056d21dd2.webp#clientId=ubbefa5fa-1f93-4&from=paste&id=ub362944c&originHeight=892&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=u08502b66-d9fb-492f-b6e4-a2f54ea0e60)<br />OCI全称是Open Container Initiative，定义得是容器运行时得标准。这个标准，使用Linux的cgroup和namespace等技术，和docker所使用的技术没什么两样。docker只不过是OCI的一个实现而已，就像gVisor（runsc）所实现的一样。<br />CRI是全称是Container Runtime Interface，是k8s定义的一套与容器运行时进行交互的接口。containerd就是docker为了适应这个标准而开发的CRI实现，但它已经是CNCF的了，不再属于docker了。<br />当然，除了containerd其他厂商也可以基于CRI-O做一些事情，同样实现了CRI接口。这样就可以无缝接入到k8s中，比如redhat的OpenShift，就选用的CRI-O。但对于容器的真正调度，其实还是OCI负责的，CRI只是个中转站而已。比如，Podman，原来就是CRI-O项目的一部分，现在它可以直接操作runc来启动容器。<br />docker整个体系，被两个标准拦腰斩了两次，组件多也就不足为怪了。<br />在早些版本中，k8s为了支持docker，不得不包含一个叫做dockershim的组件。后来，k8s宣布不再支持docker，其实是放弃了dockershim组件，依然可以使用containerd来调用docker的所有功能。<br />”k8s不再支持docker“，这种文字游戏，让很多人过早的放弃了docker。
