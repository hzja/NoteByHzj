Java16<br />与 JDK 15 一样，JDK 16 也将是个短期版本，仅提供 6 个月的支持。预计于 2021 年 9 月发布的 JDK 17 将是一个长期支持（LTS）版本，会获得数年的支持。目前的 LTS 版本 JDK 11 则于 2018 年 9 月发布。<br />伴随着数千个性能、稳定性和安全性更新，Java 16 为用户提供了十七项主要的增强 / 更改（称为 JDK 增强提案——JEP），包括三个孵化器模块和一个预览特性。<br />孵化器模块（Incubator Module）中引入了一些增强，这是一种将非最终 API 和非最终工具交给开发人员的方法，该方法允许用户提供反馈，从而改善 Java 平台的质量。<br />同样，一些增强被作为 Java SE 平台的预览特性、语言或 VM 特性引入，这些增强已完全指定、完全实现但不是永久性的。JDK 特性版本中提供了这些增强，以推动开发人员根据实际使用情况提供反馈，这可能会导致它们在将来的版本中永久保留。这为用户提供了及时反馈的机会，并让工具供应商有机会在大量 Java 开发人员在生产中使用特性之前为其提供支持。<br />Java 16 随附的 17 个 JEP 分为六个不同类别：
<a name="VIFPR"></a>
## 新语言特性
<a name="fKIQ1"></a>
### JEP 394，适用于 instanceof 的模式匹配
模式匹配（Pattern Matching）最早在 Java 14 中作为预览特性引入，在 Java 15 中还是预览特性。模式匹配通过对 instacneof 运算符进行模式匹配来增强 Java 编程语言。<br />模式匹配使程序中的通用逻辑（即从对象中有条件地提取组件）得以更简洁、更安全地表示。
<a name="ALDAo"></a>
### JEP 395，记录
记录（Records）在 Java 14 和 Java 15 中作为预览特性引入。它提供了一种紧凑的语法来声明类，这些类是浅层不可变数据的透明持有者。这将大大简化这些类，并提高代码的可读性和可维护性。
<a name="Oj4OQ"></a>
## JVM 改进
<a name="eit0D"></a>
### JEP 376，ZGC 并发线程处理
JEP 376 将 ZGC 线程栈处理从安全点转移到一个并发阶段，甚至在大堆上也允许在毫秒内暂停 GC 安全点。消除 ZGC 垃圾收集器中最后一个延迟源可以极大地提高应用程序的性能和效率。
<a name="Aq6zE"></a>
### JEP 387，弹性元空间
此特性可将未使用的 HotSpot 类元数据（即元空间，metaspace）内存更快速地返回到操作系统，从而减少元空间的占用空间。具有大量类加载和卸载活动的应用程序可能会占用大量未使用的空间。新方案将元空间内存按较小的块分配，它将未使用的元空间内存返回给操作系统来提高弹性，从而提高应用程序性能并降低内存占用。
<a name="EQIE9"></a>
## 新工具和库
<a name="Bt8ra"></a>
### JEP 380，Unix-Domain 套接字通道
Unix-domain 套接字一直是大多数 Unix 平台的一个特性，现在在 Windows 10 和 Windows Server 2019 也提供了支持。此特性为 java.nio.channels 包的套接字通道和服务器套接字通道 API 添加了 Unix-domain（AF_UNIX）套接字支持。它扩展了继承的通道机制以支持 Unix-domain 套接字通道和服务器套接字通道。Unix-domain 套接字用于同一主机上的进程间通信（IPC）。它们在很大程度上类似于 TCP/IP，区别在于套接字是通过文件系统路径名而不是 Internet 协议（IP）地址和端口号寻址的。对于本地进程间通信，Unix-domain 套接字比 TCP/IP 环回连接更安全、更有效。
<a name="koyNh"></a>
### JEP 392，打包工具
此特性最初是作为 Java 14 中的一个孵化器模块引入的，该工具允许打包自包含的 Java 应用程序。它支持原生打包格式，为最终用户提供自然的安装体验，这些格式包括 Windows 上的 msi 和 exe、macOS 上的 pkg 和 dmg，还有 Linux 上的 deb 和 rpm。它还允许在打包时指定启动时参数，并且可以从命令行直接调用，也可以通过 ToolProvider API 以编程方式调用。注意 jpackage 模块名称从 jdk.incubator.jpackage 更改为 jdk.jpackage。这将改善最终用户在安装应用程序时的体验，并简化了“应用商店”模型的部署。
<a name="Mgxp7"></a>
## 为未来做好准备
<a name="SGYVI"></a>
### JEP 390，对基于值的类发出警告
此特性将原始包装器类（java.lang.Integer、java.lang.Double 等）指定为基于值的（类似于 java.util.Optional 和 java.time.LocalDateTime），并在其构造器中添加 forRemoval（自 JDK 9 开始被弃用），这样会提示新的警告。在 Java 平台中尝试在任何基于值的类的实例上进行不正确的同步时，它会发出警告。<br />许多流行的开源项目已经在其源中删除了包装构造器调用来响应 Java 9 的弃用警告，并且鉴于“弃用移除”警告的紧迫性，可以期望更多开源项目跟上这一步伐。
<a name="LMKwc"></a>
### JEP 396，默认强封装 JDK 内部元素
此特性会默认强封装 JDK 的所有内部元素，但关键内部 API（例如 sun.misc.Unsafe）除外。默认情况下，使用早期版本成功编译的访问 JDK 内部 API 的代码可能不再起作用。鼓励开发人员从使用内部元素迁移到使用标准 API 的方法上，以便他们及其用户都可以无缝升级到将来的 Java 版本。强封装由 JDK 9 的启动器选项–illegal-access 控制，到 JDK 15 默认改为 warning，从 JDK 16 开始默认为 deny。（目前）仍然可以使用单个命令行选项放宽对所有软件包的封装，将来只有使用–add-opens 打开特定的软件包才行。
<a name="WZ2wW"></a>
## 孵化器和预览特性
<a name="VUo77"></a>
### JEP 338，向量 API（孵化器）
该孵化器 API 提供了一个 API 的初始迭代以表达一些向量计算，这些计算在运行时可靠地编译为支持的 CPU 架构上的最佳向量硬件指令，从而获得优于同等标量计算的性能，充分利用单指令多数据（SIMD）技术（大多数现代 CPU 上都可以使用的一种指令）。尽管 HotSpot 支持自动向量化，但是可转换的标量操作集有限且易受代码更改的影响。该 API 将使开发人员能够轻松地用 Java 编写可移植的高性能向量算法。
<a name="nHlDD"></a>
### JEP 389，外部链接器 API（孵化器）
该孵化器 API 提供了静态类型、纯 Java 访问原生代码的特性，该 API 将大大简化绑定原生库的原本复杂且容易出错的过程。Java 1.1 就已通过 Java 原生接口（JNI）支持了原生方法调用，但并不好用。Java 开发人员应该能够为特定任务绑定特定的原生库。它还提供了外来函数支持，而无需任何中间的 JNI 粘合代码。
<a name="axT3L"></a>
### JEP 393，外部存储器访问 API（第 3 个孵化器）
在 Java 14 和 Java 15 中作为孵化器 API 引入的这个 API 使 Java 程序能够安全有效地对各种外部存储器（例如本机存储器、持久性存储器、托管堆存储器等）进行操作。它提供了外部链接器 API 的基础。
<a name="Y7PKO"></a>
### JEP 397，密封类（第二预览）
这个预览特性可以限制哪些类或接口可以扩展或实现它们；它允许类或接口的作者控制负责实现它的代码；它还提供了比访问修饰符更具声明性的方式来限制对超类的使用。它还通过对模式进行详尽的分析来支持模式匹配的未来发展。<br />提升 OpenJDK 开发人员的生产力<br />其余更改对 Java 开发人员（使用 Java 编写代码和运行应用程序的人员）不会直接可见，而只对 Java 开发人员（参与 OpenJDK 开发的人员）可见。
<a name="d825P"></a>
### JEP 347，启用 C++14 语言特性（在 JDK 源代码中）
它允许在 JDK C++ 源代码中使用 C++14 语言特性，并提供在 HotSpot 代码中可以使用哪些特性的具体指导。在 JDK 15 中，JDK 中 C++ 代码使用的语言特性仅限于 C++98/03 语言标准。它要求更新各种平台编译器的最低可接受版本
<a name="YOJRL"></a>
### JEP 357，从 Mercurial 迁移到 Git；JEP 369，迁移到 GitHub
这些 JEP 将 OpenJDK 社区的源代码存储库从 Mercurial（hg）迁移到 Git，并将它们托管在 GitHub 上以供 JDK 11 及更高版本使用，其中包括将 jcheck、webrev 和 defpath 工具等工具更新到 Git。Git 减小了元数据的大小（约 1/4），可节省本地磁盘空间并减少克隆时间。与 Mercurial 相比，现代工具链可以更好地与 Git 集成。<br />Open JDK Git 存储库现在位于 [https://github.com/openjdk](https://github.com/openjdk)。
<a name="PHcIL"></a>
### JEP 386，AlpineLinux 移植；JEP 388，Windows/AArch64 移植
这些 JEP 的重点不是移植工作本身，而是将它们集成到 JDK 主线存储库中；JEP 386 将 JDK 移植到 Alpine Linux 和其他使用 musl 作为 x64 上主要 C 库的发行版上。此外，JEP 388 将 JDK 移植到 Windows AArch64（ARM64）。
<a name="ypXJ1"></a>
## 工具链支持
工具链有助于提高开发人员的生产力。目前，对 Java 16 提供支持的 IDE 有 JetBrainsIDEA、EclipseIDE。
