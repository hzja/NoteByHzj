Java<br />JDK 20 / Java 20 正式发布。JDK 20 是一个短期维护版本，将获得六个月的支持。尽管如此，但它仍然可用于生产环境中。根据开发计划，下一个 LTS 版本就是将于 2023 年 9 月发布的 JDK 21。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679532752658-bb386bc7-5c6c-4e4f-9b83-c3eb4acd8a20.png#averageHue=%23f0f0f0&clientId=u2414959d-ce6f-4&from=paste&id=ucf9ccb82&originHeight=270&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u36d5ddc7-e713-4161-8e65-e68217264a7&title=)<br />**此版本包括 7 个 JEP（都处于不同的孵化和预览阶段），以及数百个较小的功能增强和数千个错误修复：**

| 429: | Scoped Values (Incubator) |
| --- | --- |
| 432: | Record Patterns (Second Preview) |
| 433: | Pattern Matching for switch (Fourth Preview) |
| 434: | Foreign Function & Memory API (Second Preview) |
| 436: | Virtual Threads (Second Preview) |
| 437: | Structured Concurrency (Second Incubator) |
| 438: | Vector API (Fifth Incubator) |

<a name="ynfM6"></a>
### **作用域值（Scoped Values）进入孵化阶段**
引入 Scoped Values，它可以在线程内和线程间共享不可变数据。它们优于线程局部变量，尤其是在使用大量虚拟线程时。
<a name="jmdU8"></a>
### **记录模式 (Record Patterns) 进入第 2 预览阶段**
Record Patterns 可对 record 的值进行解构，Record patterns 和 Type patterns 通过嵌套能够实现强大的、声明性的、可组合的数据导航和处理形式。
<a name="Yz4kX"></a>
### **switch 模式匹配 (Pattern Matching for switch) 进入第 4 预览阶段**
用 switch 表达式和语句的模式匹配，以及对模式语言的扩展来增强 Java 编程语言。将模式匹配扩展到 switch 中，允许针对一些模式测试表达式，这样就可以简明而安全地表达复杂的面向数据的查询。
<a name="ZkiWs"></a>
### **外部函数和内存 API (Foreign Function & Memory API) 进入第 2 预览阶段**
引入一个 API，通过它，Java 程序可以与 Java 运行时之外的代码和数据进行互操作。通过有效地调用外部函数，以及安全地访问外部内存，该 API 使 Java 程序能够调用本地库并处理本地数据，而不会像 JNI 那样有漏洞和危险。
<a name="W7AeE"></a>
### **虚拟线程 (Virtual Threads) 进入第 2 预览阶段**
为 Java 引入虚拟线程，虚拟线程是 JDK 实现的轻量级线程，它在其他多线程语言中已经被证实是十分有用的，比如 Go 中的 Goroutine、Erlang 中的进程。虚拟线程避免了上下文切换的额外耗费，兼顾了多线程的优点，简化了高并发程序的复杂，可以有效减少编写、维护和观察高吞吐量并发应用程序的工作量。
<a name="dve4p"></a>
### **结构化并发 (Structured Concurrency) 进入第 2 孵化阶段**
JDK 19 引入了结构化并发，这是一种多线程编程方法，目的是为了通过结构化并发 API 来简化多线程编程，并不是为了取代 java.util.concurrent。<br />结构化并发将不同线程中运行的多个任务视为单个工作单元，从而简化错误处理、提高可靠性并增强可观察性。也就是说，结构化并发保留了单线程代码的可读性、可维护性和可观察性。
<a name="YyYU0"></a>
### **向量 API (Vector API) 进入第 5 孵化阶段**
向量计算由对向量的一系列操作组成。向量 API 用来表达向量计算，该计算可以在运行时可靠地编译为支持的 CPU 架构上的最佳向量指令，从而实现优于等效标量计算的性能。向量 API 的目标是为用户提供简洁易用且与平台无关的表达范围广泛的向量计算。<br />更多详情可查看：[https://openjdk.org/projects/jdk/20/](https://openjdk.org/projects/jdk/20/)<br />下载：[https://jdk.java.net/20/](https://jdk.java.net/20/)
