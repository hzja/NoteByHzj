Java 21 / JDK 21 已正式 GA，此版本是继 JDK 17 后的长期支持版本 (LTS)，Oracle 将为其提供**至少八年的技术支持和更新**。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1695347468879-a6cbaaaa-616e-4a96-863e-3460258f61e5.png#averageHue=%23f6f4f2&clientId=u6ccda4b1-3292-4&from=paste&id=ubc18dfa1&originHeight=209&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc81ce53d-74d8-4ec9-8448-b8595c0932c&title=)<br />Download：[https://jdk.java.net/21/](https://jdk.java.net/21/)<br />Release notes：[https://jdk.java.net/21/release-notes](https://jdk.java.net/21/release-notes)<br />Features：[https://openjdk.org/projects/jdk/21/](https://openjdk.org/projects/jdk/21/)
<a name="Y8lni"></a>
## 发布版本说明
根据发布的规划，这次发布的 JDK 21 将是一个长期支持版（LTS 版）。LTS 版每 2 年发布一个，上一次长期支持版是 21 年 9 月发布的 JDK 17。<br />本版本是 Java SE 平台 21 版的参考实现，由 Java 社区流程中的 JSR 396 指定。
<a name="TvoA2"></a>
## 安装包下载
主要分为 OpenJDK 版本和 Oracle 版本，下载地址如下：

- OpenJDK 版本：[https://jdk.java.net/21/](https://jdk.java.net/21/)
- Oracle 版本：[https://www.oracle.com/java/technologies/downloads/](https://www.oracle.com/java/technologies/downloads/)

上述版本，如果是个人学习用途，则差异不大。但如果是用于商业用途，则需要仔细看好相关的授权。Oracle JDK 商用需要遵循 Oracle No-Fee Terms and Conditions (NFTC) 协议，而 OpenJDK 根据 GPL v2 许可获得许可。<br />更多有关 Java 的基本知识，可以参阅《Java 核心编程》这本书，描述的非常详细。
<a name="cMUDW"></a>
## JDK 21 新特性说明
<a name="CBH6H"></a>
## 正式稳定功能
<a name="iIehQ"></a>
### JEP 431：序列集合
序列集合（Sequenced Collections）在 core-libs/java.util:collections 包中。<br />该 JEP 提议引入 “一个新的接口族，用于表示集合的概念，这些集合的元素按照预定义的序列或顺序排列，它们是作为集合的结构属性。” 这一提案的动机是由于集合框架中缺乏预定义的顺序和统一的操作集。
<a name="BZAWO"></a>
### JEP 439：分代 ZGC
分代 ZGC（Generational ZGC）在 hotspot/gc 包中。<br />通过扩展 Z 垃圾回收器（ZGC）来维护年轻对象和年老对象的独立生成，从而提高应用程序性能。这将使 ZGC 能够更频繁地收集年轻对象 —— 这些对象往往英年早逝。
<a name="pX1lA"></a>
### JEP 440：记录模式
使用记录模式（Record Patterns）增强 Java 编程语言，以解构记录值。可以嵌套记录模式和类型模式，以实现功能强大、声明性和可组合形式的数据导航和处理。
<a name="SHwI2"></a>
### JEP 441：switch 模式匹配
通过 switch 表达式和语句的模式匹配来增强 Java 编程语言。通过将模式匹配扩展到 switch，可以针对多个模式测试表达式，每个模式都有一个特定的操作，从而可以简洁、安全地表达复杂的面向数据的查询。
<a name="VougZ"></a>
### JEP 444：虚拟线程
将虚拟线程（Virtual Threads）引入 Java 平台。虚拟线程是轻量级线程，可以显著减少编写、维护和观察高吞吐量并发应用程序的工作量。
<a name="Lx3QR"></a>
### JEP 449：弃用 Windows 32 位 x86 移植
弃用 Windows 32 位 x86 移植，并打算在将来的版本中将其删除。
<a name="vSbjE"></a>
### JEP 451：准备禁止动态加载代理
将代理动态加载到正在运行的 JVM 中时发出警告。这些警告旨在让用户为将来的版本做好准备，该版本默认情况下不允许动态加载代理，以提高默认情况下的完整性。在启动时加载代理的可服务性工具不会导致在任何版本中发出警告。
<a name="YWqLu"></a>
### JEP 452：密钥封装机制 API
介绍一种用于密钥封装机制（Key Encapsulation Mechanism，简称 KEM）的 API，这是一种使用公钥加密来保护对称密钥的加密技术。
<a name="DUL87"></a>
## 预览阶段功能
<a name="RmTgd"></a>
### JEP 430：字符串模板（预览）
使用字符串模板（String Templates）增强 Java 编程语言。字符串模板通过将文本与嵌入的表达式和模板处理器耦合来生成专门的结果，从而补充 Java 现有的字符串文本和文本块。这是一个预览语言功能和 API。
<a name="hp0ob"></a>
### JEP 442：外部函数和内存 API（第三次预览）
引入 API，Java 程序可以通过该 API 与 Java 运行时之外的代码和数据进行互操作。通过有效地调用外部函数（即 JVM 外部的代码），并通过安全地访问外部内存（即不受 JVM 管理的内存），API 使 Java 程序能够调用本机库并处理本机数据，而不会出现 JNI 的脆弱性和危险性。这是一个预览 API。
<a name="wrzbe"></a>
### JEP 443：未命名模式和变量（预览）
使用未命名模式和未命名变量来增强 Java 语言，未命名模式匹配记录组件而不说明组件的名称或类型，未命名变量可以初始化但不使用。两者都用下划线字符_表示。这是一个预览语言功能。
<a name="Y0lIA"></a>
### JEP 445：未命名类和实例主方法（预览）
发展 Java 语言，使学生无需理解为大型程序设计的语言功能即可编写第一个程序。学生们不用使用单独的 Java 方言，就可以为单类程序编写精简的声明，然后随着技能的发展，无缝地扩展程序，使用更高级的功能。这是一个预览语言功能。
<a name="k6H6d"></a>
### JEP 446：作用域值（预览）
引入作用域值，这些值可以在不使用方法参数的情况下安全有效地共享给方法。它们优先于线程化局部变量，尤其是在使用大量虚拟线程时。这是一个预览 API。<br />实际上，作用域值是一个隐式方法参数。这 “就好像” 一系列调用中的每个方法都有一个额外的、不可见的参数。没有任何方法声明此参数，只有能够访问作用域值对象的方法才能访问其值（数据）。通过作用域值，可以通过一系列中间方法将数据从调用者安全地传递给遥远的被调用者，这些中间方法不声明数据的参数，也无法访问数据。
<a name="QaTZF"></a>
### JEP 453：结构化并发（预览）
通过引入用于结构化并发（Structured Concurrency）的 API 来简化并发编程。结构化并发将在不同线程中运行的相关任务组视为单个工作单元，从而简化错误处理和消除，提高可靠性，并增强可观察性。这是一个预览 API。
<a name="ZJ7TL"></a>
## 孵化阶段功能
<a name="SABdT"></a>
### JEP 448：Vector API（孵化器第六阶段）
引入 API 来表示向量计算，这些向量计算在运行时可靠地编译为支持的 CPU 架构上的最佳向量指令，从而实现优于等效标量计算的性能。
