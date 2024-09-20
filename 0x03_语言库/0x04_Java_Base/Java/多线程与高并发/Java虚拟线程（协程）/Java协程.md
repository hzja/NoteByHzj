Java 协程<br />OpenJDK 的 JEP 425 ：虚拟线程（预览版）功能提案显示：Java 平台将引入虚拟线程特性（期待已久的协程）。虚拟线程是轻量级线程，可显著地减少编写、维护和观察高吞吐量并发应用程序的工作量。<br />Java 开发人员一直依赖线程作为并发服务器应用程序的构建块，每个方法中的语句都在一个线程内执行，每个线程提供一个堆栈来存储局部变量和协调方法调用，以及报错时的上下文捕获。线程是 Java 的并发单元，也是 Java 工具的核心基础：调试器逐步执行线程方法中的语句，分析器则可视化多个线程的行为。<br />目前，JDK 将其平台线程实现为操作系统 (OS) 线程的包装器，JDK 中每个实例都是一个平台线程，平台线程在底层操作系统线程上运行 Java 代码 ，并在代码的整个生命周期内捕获 OS 线程。平台线程数受限于 OS 线程数，而 OS 线程的成本很高，不能占用太多。因此，目前 JDK 的这种线程实现方法限制了其应用程序的吞吐量，使吞吐量远低于硬件支持的水平。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649811169720-dabcca45-7adc-4281-8da7-34631b92d9a3.png#clientId=u5f6e6e5d-b147-4&from=paste&id=ua6c9e586&originHeight=524&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u195e6fc8-4fb2-4f18-a237-80f344ae8ba&title=)
<a name="cove0"></a>
## 关于虚拟线程
虚拟线程java.lang.Thread是在底层操作系统线程（OS 线程）上运行 Java 代码，但在代码的整个生命周期内不捕获 OS 线程的实例。这意味着许多虚拟线程可以在同一个 OS 线程上运行 Java 代码，从而有效地共享它。<br />虚拟线程是由 JDK 而不是操作系统提供的线程的轻量级实现，也是**用户模式线程**的一种形式。用户模式线程在 Java 的早期版本中被称为“绿色线程”，当时操作系统线程的概念还不够成熟和普及， Java 的所有绿色线程都共享一个 OS 线程（M:1 调度），随着线程概念的发展，绿色线程最终被现在的平台线程超越，实现为 OS 线程的包装器（1:1 调度），而最新引入的虚拟线程采用 M:N 调度，其中大量 (M) 虚拟线程被调度为在较少数量 (N) 的 OS 线程上运行。
<a name="eesPV"></a>
## 更高的吞吐量
开发者可以选择使用虚拟线程还是平台线程，但虚拟线程在高吞吐量的服务器应用程序中表现更好。比如下面这段休眠一秒钟的代码就创建了大量的虚拟线程，程序首先获得一个 ExecutorService，它为每个提交的任务创建一个新的虚拟线程，然后提交 10000 个任务并等待所有任务完成：
```java
try (var executor = Executors.newVirtualThreadPerTaskExecutor()) {
    IntStream.range(0, 10_000).forEach(i -> {
        executor.submit(() -> {
            Thread.sleep(Duration.ofSeconds(1));
            return i;
        });
    });
}  // executor.close() is called implicitly, and waits
```
现代硬件可以很容易地支持 10000 个虚拟线程同时运行这样的代码。如果该程序使用为每个任务都创建一个新平台线程的 ExecutorService，例如 `Executors.newCachedThreadPool()` ， 那么它将尝试创建 10000 个平台线程，也就意味着 10000 个 OS 线程，那么这个程序在大多数操作系统上都会崩溃。又或者这个程序使用从池中获取平台线程的 ExecutorService，如 `Executors.newFixedThreadPool(200)`，也好不到哪去。ExecutorService 将创建 200 个平台线程供这 10000 个任务共享，任务将按顺序运行而不是同时运行，程序需要很长时间才能跑完。<br />对于上述程序来说，具有 200 个平台线程的池只能实现每秒 200 个任务的吞吐量，而虚拟线程可以实现大约每秒 10000 个任务的吞吐量（在充分预热之后）。此外，如果将示例程序中的 10000 更改为 1,000,000 ，则程序将提交 1,000,000 个任务，创建 1,000,000 个并发运行的虚拟线程，并且（在充分预热后）达到大约 1,000,000 个任务/秒的吞吐量。<br />总而言之，虚拟线程不是更快的线程 —— 它们运行代码的速度并不比平台线程快。它们的存在是为了提供规模（更高的吞吐量），而不是速度（更低的延迟）。
<a name="x7l8T"></a>
## 如何启用虚拟线程？
目前虚拟线程在其他多线程语言中被广泛使用（例如 Go 中的协程 和 Erlang 中的进程，在 C++ 中也是一个稳定特性），但在 Java 中还是一个预览 API，默认禁用。如要在 JDK XX 上尝试该功能，则必须通过以下方法启用预览 API：

- 使用 `javac --release XX --enable-preview Main.java` 编译程序，并使用 `java --enable-preview Main` 运行
- 使用源代码启动器时，使用 `java --release XX --enable-preview Main.java` 运行程序
- 使用 jshell 时，用 `jshell --enable-preview` 启动

有关虚拟线程的更多信息可在 OpenJDK 的 JDK Issue-8277131 中查看，目前该提案于 2021/11/15 创立，目前还处于 JEP 流程的第一阶段，距离稳定版本还需要一段时间。
