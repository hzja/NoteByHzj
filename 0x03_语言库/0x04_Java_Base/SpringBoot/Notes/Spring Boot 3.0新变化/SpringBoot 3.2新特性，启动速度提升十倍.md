<a name="aqc3Y"></a>
## 一、前言
Spring 6.1 和 Spring Boot 3.2 已发布，在这两个版本中它们均全面支持 CRaC（Coordinated Restore at Checkpoint）。<br />CRaC 是 OpenJDK 项目，能够把运行中的 JVM，将其状态（包括应用程序）存储到磁盘中。然后，在另一个时间点，可以从保存的检查点将 JVM 恢复到内存中。通过这种方式，可以启动一个应用程序，预热它，并创建一个检查点。从保存的检查点恢复到内存主要依赖于磁盘 I/O，这意味着速度非常快（在毫秒级别）。
![](https://cdn.nlark.com/yuque/__puml/617fd02fbef04ec041e2c9c7b607b200.svg#lake_card_v2=eyJ0eXBlIjoicHVtbCIsImNvZGUiOiJAc3RhcnR1bWxcblxucGFydGljaXBhbnQgXCJVc2VyIEFwcGxpY2F0aW9uXCIgYXMgQXBwbGljYXRpb24gI2IzZjJiY1xucGFydGljaXBhbnQgXCJSdW5uaW5nIEpWTVwiIGFzIEp2bVxucGFydGljaXBhbnQgXCJDUmFDXCIgYXMgQ1JhQ1xucGFydGljaXBhbnQgXCJEaXNrIFN0b3JhZ2VcIiBhcyBEaXNrXG5wYXJ0aWNpcGFudCBcIk1lbW9yeVwiIGFzIE1lbW9yeVxuXG5BcHBsaWNhdGlvbiAtPiBKdm06IHJ1bm5pbmcgaW5zaWRlIEpWTVxuSnZtIC0-IENSYUM6IHByZXBhcmUgZm9yIGNoZWNrcG9pbnRcbkNSYUMgLT4gRGlzazogc2F2ZSBzdGF0ZSB0byBkaXNrXG5ub3RlIG92ZXIgRGlzayxNZW1vcnk6IFN0YXRlXFxuVXNlciBBcHBsaWNhdGlvblxuQ1JhQyAtLT4gSnZtOiBjaGVja3BvaW50IGNyZWF0ZWRcbkp2bSAtPiBBcHBsaWNhdGlvbjogcGF1c2UgZXhlY3V0aW9uXG5cbm5vdGUgb3ZlciBBcHBsaWNhdGlvbjogYWx0XG5DUmFDIC0-IERpc2s6IHJldHJpZXZlIHN0YXRlXG5EaXNrIC0-IE1lbW9yeTogbG9hZCBzdGF0ZVxuTWVtb3J5IC0-IEp2bTogcmVpbml0aWFsaXplIGZyb20gc3RhdGVcbkp2bSAtPiBBcHBsaWNhdGlvbjogcmVzdW1lIGV4ZWN1dGlvblxuXG5AZW5kdW1sIiwidXJsIjoiaHR0cHM6Ly9jZG4ubmxhcmsuY29tL3l1cXVlL19fcHVtbC82MTdmZDAyZmJlZjA0ZWMwNDFlMmM5YzdiNjA3YjIwMC5zdmciLCJpZCI6InJJaWJEIiwibWFyZ2luIjp7InRvcCI6dHJ1ZSwiYm90dG9tIjp0cnVlfSwiY2FyZCI6ImRpYWdyYW0ifQ==)为了测试 Spring Boot 3.2 对 CRaC 的支持，使用 Spring Boot Petclinic 进行演示。
<a name="wwCqT"></a>
## 二、依赖
要在 Spring Boot 3.2 中使用 CRaC，需要满足以下三个条件：

- 支持 CRaC 的 JVM
- org.crac 的 jar 依赖
- 可以存储检查点的文件夹
<a name="P1yAG"></a>
### 2.1 安装 Zulu JDK
首先需要使用的 JDK 是**Azul Zulu 21.0.1 + CRaC**，下载地址：[https://www.azul.com/downloads/?os=linux&package=jdk-crac](https://www.azul.com/downloads/?os=linux&package=jdk-crac)。Azul 提供了 x64、aarch64 CPU 架构和 JDK 17 和 JDK 21 支持 CRaC 的 JDK 版本。**CRaC 目前仅在 Linux 上可用，MacOS 和 Windows 还需要等待。**<br />为了能够使用 CRIU，可能需要设置权限。在 Linux 机器上执行以下命令：
```bash
sudo chown root:root $JAVA_HOME/lib/criu
sudo chmod u+s $JAVA_HOME/lib/criu
```
<a name="FIGP0"></a>
### 2.2 添加 jar 依赖
首先克隆 Petclinic 代码，并添加 org.crac 依赖。我们可以在 Maven 中央库中找到 org.crac，可以按以下方式添加依赖项：
```xml
<dependency>
  <groupId>org.crac</groupId>
  <artifactId>crac</artifactId>
  <version>1.4.0</version>
</dependency>
```
<a name="zCBLR"></a>
## 三、基线测试
在 JDK（17 和 21）上进行测试，首先仅仅从 17 切换到 21 就已经将 Petclinic 的启动时间减少了 500 毫秒！<br />因此，如果可能的话，应该尽快升级到 JDK 21 以获取更好的性能。通过执行以下命令来启动应用程序：
```bash
java -jar spring-petclinic-3.2.0.jar
```
以下是启动的耗时统计：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701964043938-eae98ec0-79c6-4e0d-85d2-f2d086fee4c7.png#averageHue=%2383d655&clientId=uca31f317-fe63-4&from=paste&id=ub78d8437&originHeight=456&originWidth=700&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4b3eb265-501d-4323-94d7-071ec1e3065&title=)<br />虽然启动时间快了大约 500 毫秒，但整体上还是需要 4 秒钟整体启动。因此看一下在 Spring Boot 3.2 中实现的另一个新方法。
<a name="qSCKm"></a>
### 3.1 使用 CRaC 自动检查点
通过在应用程序启动之前自动创建一个检查点来改善 Spring Boot 应用的启动时间。<br />当设置了-Dspring.context.checkpoint=onRefresh JVM 系统属性时，在启动过程中的 LifecycleProcessor.onRefresh 阶段会自动创建一个检查点。在此阶段完成后，所有非懒加载的单例都已被实例化，并调用了InitializingBean#afterPropertiesSet回调；这个时候应用程序生命周期尚未开始，并且ContextRefreshedEvent尚未发布。

- 要使用自动检查点功能，需要按以下方式启动应用程序：
```bash
java -Dspring.context.checkpoint=onRefresh -XX:CRaCCheckpointTo=./tmp_checkpoint -jar spring-petclinic-3.2.0.jar
```
执行应用程序后，它会创建检查点，将检查点文件存储在./tmp_checkpoint 文件夹中，然后退出应用程序。

- 现在，您可以通过执行以下命令从检查点中恢复应用程序（也就是重新启动）：
```bash
java -XX:CRaCRestoreFrom=./tmp_checkpoint
```
以下是与从自动检查点恢复相关的启动时间结果：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701964043943-44f5d24f-b65a-424c-b2d2-fb0670870d19.png#averageHue=%23edbd44&clientId=uca31f317-fe63-4&from=paste&id=uf43579a1&originHeight=432&originWidth=700&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc7252667-eb54-4392-88a1-bcb6564b78c&title=)<br />太酷了，**启动时间比原来快一个数量级**，而且无需更改代码。
<a name="VZAXR"></a>
### 3.2 手动开启检查点
**使用手动检查点可以比自动检查点更快地启动应用程序。**<br />在使用手动检查点时，可以在任何时间创建检查点。比如可能希望在 10 分钟后或应用程序完全预热（大部分/全部代码已编译和优化）后创建检查点。<br />创建手动检查点的过程与自动检查点类似，唯一的区别是通过外部触发检查点，而不是由框架自动创建检查点。<br />在开始之前，请确保检查点文件夹为空。

- 首先，按以下方式启动应用程序：
```bash
java -XX:CRaCCheckpointTo=./tmp_checkpoint -jar spring-petclinic-3.2.0.jar
```

- 等待应用程序完全启动，在第二个 Shell 窗口中，执行以下命令：
```bash
jcmd spring-petclinic-3.2.0.jar JDK.checkpoint
```
现在，应该看到在启动 petclinic 应用程序的第一个 Shell 窗口中创建了一个检查点，并且应用程序已关闭。<br />通过验证文件夹./tmp_checkpoint中是否包含检查点文件，可以检查应用程序是否已进行了检查点。<br />现在，可以关闭第二个 Shell 窗口。<br />要从此检查点恢复应用程序，执行与自动检查点相同的命令：
```bash
java -XX:CRaCRestoreFrom=./tmp_checkpoint
```
**这个手动触发的检查点不仅包含框架代码，还包含应用程序代码，这意味着我们应该会看到启动速度更快，因为应用程序已由框架加载和启动**。以下是结果：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701964043957-4acd7d53-9456-4bc9-9d16-997481db8eb1.png#averageHue=%23f9f8f7&clientId=uca31f317-fe63-4&from=paste&id=u6cb886e9&originHeight=441&originWidth=700&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8fd14104-de5a-42ef-aab2-f58b3964ee8&title=)
<a name="cLX4b"></a>
## 四、特别说明
Spring Boot 3.2 已经完全支持 CRaC，无需对代码进行修改。意味着只要使用 Spring Boot，框架将在检查点之前负责关闭资源，并在应用恢复之后重新打开它们。<br />如果使用其他框架，则需要在相关类中实现 **CRaC Resource 接口**，并在`beforeCheckpoint()`方法中关闭这些资源（例如打开的文件或 socket 连接），并在`afterRestore()`方法中重新打开这些资源。
<a name="ibUlc"></a>
## 五、结论
正如看到的，使用 CRaC 可以显著减少 Spring Boot 3.2 应用程序的启动时间。<br />如果不想修改代码，可以通过简单地在 Spring Boot 3.2 中使用自动检查点功能，将启动时间缩短一个数量级。当然，追求更快的启动速度，可以手动创建一个检查点。<br />对比 GraalVM Native Image，CRaC 的优势在于它可以在通用的 JVM 上运行，并且代码可以在检查点恢复之后得到进一步优化。
