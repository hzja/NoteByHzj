Linux Window <br />Windows 和 Linux 可以说是比较常见的两款操作系统的。<br />Windows 基本占领了电脑时代的市场，商业上取得了很大成功，但是它并不开源，所以要想接触源码得加入 Windows 的开发团队中。<br />对于服务器使用的操作系统基本上都是 Linux，而且内核源码也是开源的，任何人都可以下载，并增加自己的改动或功能，Linux 最大的魅力在于，全世界有非常多的技术大佬为它贡献代码。<br />这两个操作系统各有千秋，不分伯仲。<br />操作系统核心的东西就是内核**Linux 内核和 Windows 内核有什么区别？**

---

<a name="JjPjf"></a>
## 内核
<a name="q5VBd"></a>
### 什么是内核呢？
计算机是由各种外部硬件设备组成的，比如内存、cpu、硬盘等，如果每个应用都要和这些硬件设备对接通信协议，那这样太累了。<br />所以，这个中间人就由内核来负责，**让内核作为应用连接硬件设备的桥梁**，应用程序只需关心与内核交互，不用关心硬件的细节。<br />![内核](https://cdn.nlark.com/yuque/0/2021/png/396745/1617505157415-b6573ace-87bf-4eb5-9447-a0af22ae0d2d.png#averageHue=%2366eeaa&height=853&id=FKfBQ&originHeight=853&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=true&size=0&status=done&style=shadow&title=%E5%86%85%E6%A0%B8&width=1080 "内核")
<a name="wQMHS"></a>
### 内核有哪些能力呢？
现代操作系统，内核一般会提供 4 个基本能力：

- 管理进程、线程，决定哪个进程、线程使用 CPU，也就是进程调度的能力；
- 管理内存，决定内存的分配和回收，也就是内存管理的能力；
- 管理硬件设备，为进程与硬件设备之间提供通信能力，也就是硬件通信能力；
- 提供系统调用，如果应用程序要运行更高权限运行的服务，那么就需要有系统调用，它是用户程序与操作系统之间的接口。
<a name="pLXuU"></a>
### 内核是怎么工作的？
内核具有很高的权限，可以控制 cpu、内存、硬盘等硬件，而应用程序具有的权限很小，因此大多数操作系统，把内存分成了两个区域：

- 内核空间，这个内存空间只有内核程序可以访问；
- 用户空间，这个内存空间专门给应用程序使用；

用户空间的代码只能访问一个局部的内存空间，而内核空间的代码可以访问所有内存空间。<br />因此，当程序使用用户空间时，常说该程序在**用户态**执行，而当程序使内核空间时，程序则在**内核态**执行。<br />应用程序如果需要进入内核空间，就需要通过「系统调用」，下面来看看系统调用的过程：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617505157403-24bf416f-cddb-4864-9ee2-415392dc0130.png#averageHue=%23fafaf9&height=332&id=J2Lro&originHeight=332&originWidth=1053&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=1053)<br />内核程序执行在内核态，用户程序执行在用户态。当应用程序使用系统调用时，会产生一个中断。发生中断后， CPU 会中断当前在执行的用户程序，转而跳转到中断处理程序，也就是开始执行内核程序。内核处理完后，主动触发中断，把 CPU 执行权限交回给用户程序，回到用户态继续工作。

---

<a name="IDZhf"></a>
## Linux 的设计
Linux 的开山始祖是来自一位名叫 Linus Torvalds 的芬兰小伙子，他在 1991 年用 C 语言写出了第一版的 Linux 操作系统，那年他 22 岁。<br />完成第一版 Linux 后，Linux Torvalds 就在网络上发布了 Linux 内核的源代码，每个人都可以免费下载和使用。<br />Linux 内核设计的理念主要有这几个点：

- _MutiTask_，多任务
- _SMP_，对称多处理
- _ELF_，可执行文件链接格式
- _Monolithic Kernel_，宏内核
<a name="OeaV3"></a>
### MutiTask
MutiTask 的意思是**多任务**，代表着 Linux 是一个多任务的操作系统。<br />多任务意味着可以有多个任务同时执行，这里的「同时」可以是并发或并行：

- 对于单核 CPU 时，可以让每个任务执行一小段时间，时间到就切换另外一个任务，从宏观角度看，一段时间内执行了多个任务，这被称为并发。
- 对于多核 CPU 时，多个任务可以同时被不同核心的 CPU 同时执行，这被称为并行。
<a name="crVrM"></a>
### SMP
SMP 的意思是**对称多处理**，代表着每个 CPU 的地位是相等的，对资源的使用权限也是相同的，多个 CPU 共享同一个内存，每个 CPU 都可以访问完整的内存和硬件资源。<br />这个特点决定了 Linux 操作系统不会有某个 CPU 单独服务应用程序或内核程序，而是每个程序都可以被分配到任意一个 CPU 上被执行。
<a name="PUMVQ"></a>
### ELF
ELF 的意思是**可执行文件链接格式**，它是 Linux 操作系统中可执行文件的存储格式，可以从下图看到它的结构：<br />![ELF 文件格式](https://cdn.nlark.com/yuque/0/2021/png/396745/1617505157489-c194b05e-eb39-47bc-9735-d540e5596fb1.png#averageHue=%23797979&height=1196&id=fjfHV&originHeight=1196&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=true&size=0&status=done&style=shadow&title=ELF%20%E6%96%87%E4%BB%B6%E6%A0%BC%E5%BC%8F&width=1080 "ELF 文件格式")<br />ELF 把文件分成了一个个分段，每一个段都有自己的作用。<br />另外，ELF 文件有两种索引，Program header table 中记录了「运行时」所需的段，而 Section header table 记录了二进制文件中各个「段的首地址」。<br />那 ELF 文件怎么生成的呢？<br />编写的代码，首先通过「编译器」编译成汇编代码，接着通过「汇编器」变成目标代码，也就是目标文件，最后通过「链接器」把多个目标文件以及调用的各种函数库链接起来，形成一个可执行文件，也就是 ELF 文件。<br />那 ELF 文件是怎么被执行的呢？<br />执行 ELF 文件的时候，会通过「装载器」把 ELF 文件装载到内存里，CPU 读取内存中的指令和数据，于是程序就被执行起来了。
<a name="quFCu"></a>
### Monolithic Kernel
Monolithic Kernel 的意思是**宏内核**，Linux 内核架构就是宏内核，意味着 Linux 的内核是一个完整的可执行程序，且拥有最高的权限。<br />宏内核的特征是系统内核的所有模块，比如进程调度、内存管理、文件系统、设备驱动等，都运行在内核态。<br />不过，Linux 也实现了动态加载内核模块的功能，例如大部分设备驱动是以可加载模块的形式存在的，与内核其他模块解藕，让驱动开发和驱动加载更为方便、灵活。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617505157465-0598bc96-ed47-46cf-8619-b713c7bf2c20.webp#averageHue=%23b2c14b&height=276&id=cLavD&originHeight=276&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=1080)分别为宏内核、微内核、混合内核的操作系统结构<br />与宏内核相反的是**微内核**，微内核架构的内核只保留最基本的能力，比如进程调度、虚拟机内存、中断等，把一些应用放到了用户空间，比如驱动程序、文件系统等。这样服务与服务之间是隔离的，单个服务出现故障或者完全攻击，也不会导致整个操作系统挂掉，提高了操作系统的稳定性和可靠性。 <br />微内核内核功能少，可移植性高，相比宏内核有一点不好的地方在于，由于驱动程序不在内核中，而且驱动程序一般会频繁调用底层能力的，于是驱动和硬件设备交互就需要频繁切换到内核态，这样会带来性能损耗。华为的鸿蒙操作系统的内核架构就是微内核。<br />还有一种内核叫**混合类型内核**，它的架构有点像微内核，内核里面会有一个最小版本的内核，然后其他模块会在这个基础上搭建，然后实现的时候会跟宏内核类似，也就是把整个内核做成一个完整的程序，大部分服务都在内核中，这就像是宏内核的方式包裹着一个微内核。

---

<a name="RrNWg"></a>
## Windows 设计
当今 Windows 7、Windows 10 使用的内核叫 Windows NT，NT 全称叫 New Technology。<br />下图是 Windows NT 的结构图片：<br />![Windows NT 的结构](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617505157468-95b9efa6-f5b9-4b63-91b7-009356d91085.webp#averageHue=%23d6dada&height=1313&id=Lc9X7&originHeight=1313&originWidth=1024&originalType=binary&ratio=1&rotation=0&showTitle=true&size=0&status=done&style=shadow&title=Windows%20NT%20%E7%9A%84%E7%BB%93%E6%9E%84&width=1024 "Windows NT 的结构")<br />Windows 和 Linux 一样，同样支持 MutiTask 和 SMP，但不同的是，**Windows 的内核设计是混合型内核**，在上图可以看到内核中有一个 _MicroKernel_ 模块，这个就是最小版本的内核，而整个内核实现是一个完整的程序，含有非常多模块。<br />Windows 的可执行文件的格式与 Linux 也不同，所以这两个系统的可执行文件是不可以在对方上运行的。<br />Windows 的可执行文件格式叫 PE，称为**可移植执行文件**，扩展名通常是`.exe`、`.dll`、`.sys`等。<br />PE 的结构可以从下图中看到，它与 ELF 结构有一点相似。<br />![PE 文件结构](https://cdn.nlark.com/yuque/0/2021/png/396745/1617505157472-41f54a22-6b0d-4f11-83ec-9b0df90e8dea.png#averageHue=%23b4bab5&height=527&id=NsMIG&originHeight=527&originWidth=242&originalType=binary&ratio=1&rotation=0&showTitle=true&size=0&status=done&style=shadow&title=PE%20%E6%96%87%E4%BB%B6%E7%BB%93%E6%9E%84&width=242 "PE 文件结构")

---

<a name="f5jDt"></a>
## 总结
对于内核的架构一般有这三种类型：

- 宏内核，包含多个模块，整个内核像一个完整的程序；
- 微内核，有一个最小版本的内核，一些模块和服务则由用户态管理；
- 混合内核，是宏内核和微内核的结合体，内核中抽象出了微内核的概念，也就是内核中会有一个小型的内核，其他模块就在这个基础上搭建，整个内核是个完整的程序；

Linux 的内核设计是采用了宏内核，Windows 的内核设计则是采用了混合内核。<br />这两个操作系统的可执行文件格式也不一样， Linux 可执行文件格式叫作 ELF，Windows 可执行文件格式叫作 PE。
