Java<br />Jarboot 是一款对业务代码无侵入的可视化 Java 进程管理平台，支持 Java 进程启停、诊断、监控。<br />![](https://cdn.nlark.com/yuque/0/2021/gif/1305156/1640489481233-1b7c6c54-397e-4354-9fdd-31cba57315b5.gif#from=url&id=rF46U&originHeight=743&originWidth=1349&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=)<br />Jarboot 具体的技术原理是通过 JavaAgent + ASM 来往目标 Java 进程注入代码：

- JavaAgent 可以在加载 Java 文件之前对字节码进行修改，也可以在运行期间对已经加载的类的字节码进行修改。
- ASM 是一个 Java 字节码操作框架，可以帮助操作 Java 字节码。

市面上绝大部分的 Java 诊断/分析工具的原理都类似，只是具体使用的字节码操作框架的不一样。<br />Jarboot 基于 React（前端，） + SpringBoot（后端），通过 WebSocket 向前端界面实时推送进程的相关信息。<br />类似于 Arthas，Jarboot 也提供了一些开箱即用的命令（如获取 JVM 信息、 监控线程状态、获取线程栈信息等），并且，还可以通过 SPI 扩展（支持 JDK 和 Spring 的 SPI）机制来实现自己的命令。

- Gitee 地址：[https://gitee.com/majz0908/jarboot](https://gitee.com/majz0908/jarboot)
- 文档：[https://www.yuque.com/jarboot](https://www.yuque.com/jarboot)
