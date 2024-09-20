尽管 Java 是使用过的向后兼容程度最高的语言和环境之一，但始终存在功能弃用甚至删除的可能性。Java 21 将弃用两个功能。
<a name="aERLZ"></a>
## 1、为什么要弃用功能？
弃用代码或功能意味着不鼓励使用它，并且可能在未来的版本中不再存在。为什么不鼓励它可能有很多原因。<br />弃用的最常见原因是：

- 它已被更好的替代方案所取代。
- 存在设计缺陷，甚至使用起来可能存在危险。但由于向后兼容性，它不能立即删除，或者根本不能删除。
- 它被认为是多余的，应该删除以简化系统及其使用方式。
- 未来的更新将使得支持旧功能/代码变得不可能/不切实际。

无论根本原因如何，已弃用的功能仍然是系统的一部分，因此仍然可用，最起码到现在。
<a name="xcKyy"></a>
### 弃用 Windows 32 位 x86 端口
JEP 449旨在弃用 Windows 的 32 位 x86 支持，最终目标是在将来完全删除它。<br />这种弃用及其未来删除背后的原因主要是技术性的。
<a name="DuTQz"></a>
#### Windows 32 位支持
为任何系统提供软件总是需要决定您实际想要支持哪些平台。针对不再受支持的平台或版本是可能的，但通常意味着增加支持工作、向后移植、自行修复内容等。<br />以Windows平台为例，最后一个32位版本于2020年发布，官方支持于2025年10月结束。<br />如果知道 64 位 Windows 如何处理 32 位应用程序，您可能想知道为什么不能通过 Windows集成的 WOW64 模拟层来运行 JVM ？嗯，通常可以以这种方式运行应用程序，但性能会急剧下降。<br />这就是 OpenJDK 团队决定继续弃用的原因，因为它只影响 Java 的未来版本。旧系统仍然可以使用删除之前的所有 Java 版本。<br />Java 21 中的一项直接更改会影响 JDK 的构建过程，因为默认情况下禁用配置构建的可能性。尝试运行bash ./configure会出现错误：
```
...
checking compilation type... native
configure: error: The Windows 32-bit x86 port is deprecated and may be removed in a future release. \
Use --enable-deprecated-ports=yes to suppress this error.
configure exiting with result code 1
```
由于该功能只是被弃用，而不是被删除，因此 OpenJDK 团队添加了新的配置选项（如错误所示），`--enable-deprecated-ports=yes`以仍然允许配置。但是，会发出警告以强调弃用和未来可能的删除。
```bash
$ bash ./configure --enable-deprecated-ports=yes
...
checking compilation type... native
configure: WARNING: The Windows 32-bit x86 port is deprecated and may be removed in a future release.
...
Build performance summary:
* Cores to use:   32
* Memory limit:   96601 MB

The following warnings were produced. Repeated here for convenience:
WARNING: The Windows 32-bit x86 port is deprecated and may be removed in a future release.
```
<a name="srvKq"></a>
#### 虚拟 VS 内核线程
Java 21 充满了令人敬畏的新功能，虚拟线程 (JEP 444)的添加就是其中之一。它引入了轻量级（虚拟）线程，这可能会通过减少编写、维护和观察此类应用程序所需的工作量，从而显着改变处理 Java 中高吞吐量并发应用程序的方式。它们的开销比传统平台（内核）线程少得多。<br />然而，在 Windows 32 位 x86 上，由于技术限制，此功能必须回退到内核线程。底层平台的这种缺失功能通常是未来弃用和删除的有力指标。<br />尽管如此，仍然可以编写和使用新的线程代码，但在实际操作中却缺少预期的好处。
<a name="mJ6ER"></a>
#### 已弃用，但尚未删除
正如看到的，弃用是有道理的，因为 Windows 32 位 x86 无论如何都无法运行。此外，针对特定平台进行构建仍然是可能的，只是目前不鼓励这样做。因此，如果仍然需要支持遗留系统并知道在做什么以及后果是什么，仍然可以使用它。
<a name="n2xdB"></a>
### 禁止动态加载代理
代理使用Instrumentation API通过更改 JVM 中已加载的字节码来修改现有应用程序。这使您能够更改应用程序的行为，而无需实际更改其源代码。它通常用于分析器和监视工具（例如Datadog和YourKit）、面向方面的编程等等。
<a name="FoF9E"></a>
#### 如何加载代理
有两种方法可以加载代理，一种是通过添加参数或调用来静态加载，另一种是通过运行如下代码从另一个应用程序动态加载：`-javaagent:agent-to-load.jar-agentlib:optionsjava`
```java
import java.lang.management.ManagementFactory;
import com.sun.tools.attach.VirtualMachine;

public class DynamicAgentLoader {

    public static void main(String... args) {

        int pidOfOtherJVM = ...;
        File agentJar = ...;

        try {
            VirtualMachine vm = VirtualMachine.attach(pidOfOtherJVM);
            vm.loadAgent(agentJar.toAbsolutePath);

            // ... do your work

            vm.detach();
        } catch (Exception e) {
            // ...
        }
    }
}
```
第一个选项问题不大。这是对 JVM 代理的明确且有意的使用。然而，后者是间接的，并且可能不受所连接的 JVM 的控制。
<a name="DZGd3"></a>
#### 动态加载的问题
Java 平台默认致力于实现完整性，为构建应用程序提供强大而坚实的基础。代理的设计考虑到了最好的意图，提供（良性）工具的力量。然而，为了确保这种完整性，通过（动态）代理进行检测是一个大问题，因为它们超出了直接控制范围，并且可能会对应用程序造成严重破坏。这就是为什么作为应用程序的所有者必须对允许和加载哪些代理做出有意识且明确的决定。<br />在 Java 21 中，仍然可以加载动态代理，但 JVM 会生成多个警告，通知您潜在的问题以及如何隐藏这些警告：
```
WARNING: A {Java,JVM TI} agent has been loaded dynamically (file:/path/to/agent.jar)
WARNING: If a serviceability tool is in use, please run with -XX:+EnableDynamicAgentLoading to hide this warning
WARNING: If a serviceability tool is not in use, please run with -Djdk.instrument.traceUsage for more information
WARNING: Dynamic loading of agents will be disallowed by default in a future release
```
未来的 Java 版本将默认禁止加载动态代理，并且任何使用Attach API都会引发异常：
```
com.sun.tools.attach.AgentLoadException: Failed to load agent library: \
Dynamic agent loading is not enabled. Use -XX:+EnableDynamicAgentLoading \
to launch target VM.
```
异常消息包括启用动态代理加载所需的步骤：参数`-XX:+EnableDynamicAgentLoading`。因此，如果有意识地决定允许动态代理，那么仍然可以。
<a name="zhA4R"></a>
#### 立即禁用动态加载
到目前为止，仅发出警告。但是，可以完全禁止动态加载 Java 代理。可以通过使用将（加号）与（破折号/减号）`-XX:-EnableDynamicAgentLoading`交换的参数来执行此操作，以强化应用程序或为即将到来的更改做好准备。+-
<a name="BB1VP"></a>
## 2、结论
本文中提到的两个功能的弃用是有道理的。<br />Windows 10 32 位 x86 支持是一项技术债务，阻碍了创新，例如利用虚拟线程的全部功能。<br />动态加载代理严重损害了 Java 平台的完整性，并且存在潜在的安全风险。如果攻击者“足够接近”可以连接到另一个 JVM，那么可能会遇到更大的问题。<br />尽管如此，始终必须意识到将来可能会发生变化或删除的内容，因为很可能无法决定它何时发生。Java 通常对弃用和删除时间框架相当慷慨，某些功能可能会弃用数十年，但看不到删除的迹象。所以很自然地，是否应该使用已弃用的 API 的问题就出现了。<br />如果可能的话，应该尽量避免使用已弃用的 API。随着时间的推移，它正在成为技术债务，最终必须偿还。没有什么比因为不相关的原因而需要升级代码更有压力的了，而且多年来依赖的一些已弃用的功能最终被删除，使得升级方式比需要的更加复杂。
