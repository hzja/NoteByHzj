Java<br />JDK 19 定于 9 月 20 日正式发布以供生产使用，非长期支持版本。不过，JDK 19 中有一些比较重要的新特性值得关注。<br />JDK 19 只有 7 个新特性：

- [**JEP 405: Record Patterns（记录模式）**](https://openjdk.org/jeps/405)（预览）
- [**JEP 422: Linux/RISC-V Port**](https://openjdk.org/jeps/422)
- [**JEP 424: Foreign Function & Memory API（外部函数和内存 API）**](https://openjdk.org/jeps/424)（预览）
- [**JEP 425: Virtual Threads（虚拟线程）**](https://openjdk.org/jeps/425)（预览）
- [**JEP 426: Vector（向量）API**](https://openjdk.java.net/jeps/426)（第四次孵化）
- [**JEP 427: Pattern Matching for switch（switch 模式匹配）**](https://openjdk.java.net/jeps/427)
- [**JEP 428: Structured Concurrency（结构化并发）**](https://openjdk.org/jeps/428)（孵化）

这里只对 424、425、426、428 这 4 个比较重要的新特性进行详细介绍。
<a name="e7BYD"></a>
## JEP 405 Record Patterns
这是对Java 17正式生效的**Record Class**的增强。JEP 405让记录模式和类型模式可以嵌套，以实现强大的、可声明的、可组合的数据处理形式。<br />在JDK 16 中，已经可以实现下列特性：
```java
// jdk 16 以前
if (o instanceof String) {
    // 及时类型匹配 依然需要转换
    String s = (String)o;     
}

// jdk 16 以后
if (o instanceof String s) {
    // 直接使用 s 
}
```
但是上面的特性应用在**Record Class**中并不是很丝滑，依然需要通过属性方法来获取属性值：
```java
public record Position(int x, int y) {}

// record 结合类型匹配 依然需要通过方法获取属性
private void print(Object object) {
    if (object instanceof Position position) {
        int x =  position.x();
        int y =  position.y();
    } 
}
```
JEP 405中可以这样：
```java
private void print(Object object) {
  if (object instanceof Position(int x, int y)) {
      // 直接使用 x 和 y
  } 
}
```
当然这仅仅是 JEP 405 的一小部分，该特性也可以应用到`switch`语句，甚至是嵌套条件中。
<a name="dy4A4"></a>
## JEP 422 Linux/RISC-V Port
由于 **RISC-V** 指令集架构的硬件越来越多，从Java 19开始提供相应架构的端口。<br />**RISC-V**是一个免费和开源的 **RISC** 指令集架构 (**ISA**)，最初由加州大学伯克利分校设计，现在在**RISC-V International**的赞助下合作开发。它已经被广泛的语言工具链支持。随着 **RISC-V** 硬件的日益普及，**JDK** 的移植将是有价值的。
<a name="eBrAz"></a>
## JEP 424: 外部函数和内存 API（预览）
通过该特性，Java 程序可以通过该 API 与 Java 运行时之外的代码和数据进行互操作。通过高效地调用外部函数（即 JVM 之外的代码）和安全地访问外部内存（即不受 JVM 管理的内存），该 API 使 Java 程序能够调用 native 库并处理 native 数据，而不会像 JNI 那样危险和脆弱。<br />外部函数和内存 API 之前在 JDK 17 中孵化，在 JDK 18 中重新孵化。<br />在没有外部函数和内存 API 之前：

- Java 通过 **`**[**sun.misc.Unsafe**](https://hg.openjdk.java.net/jdk/jdk/file/tip/src/jdk.unsupported/share/classes/sun/misc/Unsafe.java)**`** 提供一些执行低级别、不安全操作的方法（如直接访问系统内存资源、自主管理内存资源等），`Unsafe` 类让 Java 语言拥有了类似 C 语言指针一样操作内存空间的能力的同时，也增加了 Java 语言的不安全性，不正确使用 `Unsafe` 类会使得程序出错的概率变大。
- Java 1.1 就已通过 Java 原生接口（JNI）支持了原生方法调用，但并不好用。JNI 实现起来过于复杂，步骤繁琐（具体的步骤可以参考这篇文章：[**Guide to JNI (Java Native Interface)**](https://www.baeldung.com/jni) ），不受 JVM 的语言安全机制控制，影响 Java 语言的跨平台特性。并且，JNI 的性能也不行，因为 JNI 方法调用不能从许多常见的 JIT 优化(如内联)中受益。虽然[**JNA**](https://github.com/java-native-access/jna)、[**JNR**](https://github.com/jnr/jnr-ffi)和[**JavaCPP**](https://github.com/bytedeco/javacpp)等框架对 JNI 进行了改进，但效果还是不太理想。

引入外部函数和内存 API 就是为了解决 Java 访问外部函数和外部内存存在的一些痛点。<br />Foreign Function & Memory API (FFM API) 定义了类和接口：

- 分配外部内存 ：`MemorySegment`、`MemoryAddress`和`SegmentAllocator`）；
- 操作和访问结构化的外部内存：`MemoryLayout`，`VarHandle`；
- 控制外部内存的分配和释放：`MemorySession`；
- 调用外部函数：`Linker`、`FunctionDescriptor`和`SymbolLookup`。

下面是 FFM API 使用示例，这段代码获取了 C 库函数的 `radixsort` 方法句柄，然后使用它对 Java 数组中的四个字符串进行排序。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663047728789-7485d7e0-cccd-404d-af2a-3166b22aefa9.png#clientId=u26e76f4f-bfd7-4&errorMessage=unknown%20error&from=paste&id=uc1cc4ce4&originHeight=868&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=error&style=none&taskId=u64a64354-8f97-4ad9-8410-f59784b09bc&title=)
<a name="Q7CdB"></a>
## JEP 425: 虚拟线程（预览）
虚拟线程是 JDK 而不是 OS 实现的轻量级线程(Lightweight Process，LWP），许多虚拟线程共享同一个操作系统线程，虚拟线程的数量可以远大于操作系统线程的数量。<br />虚拟线程在其他多线程语言中已经被证实是十分有用的，比如 Go 中的 Goroutine、Erlang 中的进程。<br />虚拟线程避免了上下文切换的额外耗费，兼顾了多线程的优点，简化了高并发程序的复杂，可以有效减少编写、维护和观察高吞吐量并发应用程序的工作量。<br />![传统线程和虚拟线程](https://cdn.nlark.com/yuque/0/2022/png/396745/1663721373922-ebc5ee75-0b52-468e-89ee-3de0c7de69b1.png#clientId=u0fc9c16a-786d-4&from=paste&id=ua53a153c&originHeight=315&originWidth=720&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=ufca7dc7d-2f6e-481a-b85b-5894e22aea2&title=%E4%BC%A0%E7%BB%9F%E7%BA%BF%E7%A8%8B%E5%92%8C%E8%99%9A%E6%8B%9F%E7%BA%BF%E7%A8%8B "传统线程和虚拟线程")<br />在本次 Java 19 中虚拟线程正式以预览的状态亮相，这能够简化对多线程的操作，让以往“昂贵”的线程更加“廉价”。
<a name="kPoYG"></a>
## JEP 426: 向量 API（第四次孵化）
向量（Vector） API 最初由[**JEP 338**](https://openjdk.java.net/jeps/338)提出，并作为[**孵化 API**](http://openjdk.java.net/jeps/11)集成到 JDK 16 中。第二轮孵化由[**JEP 414**](https://openjdk.java.net/jeps/414)提出并集成到 JDK 17 中。第三轮孵化由[**JEP 417**](https://openjdk.java.net/jeps/417)提出并集成到 JDK 18 中。<br />向量计算由对向量的一系列操作组成。向量 API 用来表达向量计算，该计算可以在运行时可靠地编译为支持的 CPU 架构上的最佳向量指令，从而实现优于等效标量计算的性能。<br />向量 API 的目标是为用户提供简洁易用且与平台无关的表达范围广泛的向量计算。<br />这是对数组元素的简单标量计算：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663047728766-89a35143-a7ed-404e-bbb1-a1349cbe590d.png#clientId=u26e76f4f-bfd7-4&errorMessage=unknown%20error&from=paste&id=u66d8c84b&originHeight=280&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=error&style=none&taskId=u45726131-74b5-4779-a232-ee956548626&title=)<br />这是使用 Vector API 进行的等效向量计算：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663047728857-ce97a139-e18f-4643-bb84-0bcade29de32.png#clientId=u26e76f4f-bfd7-4&errorMessage=unknown%20error&from=paste&id=u8bd9c5d7&originHeight=848&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=error&style=none&taskId=u244b16b0-f8be-4c2e-bd8e-86ed337292a&title=)
<a name="OzoYR"></a>
## JEP 428: 结构化并发(孵化)
JDK 19 引入了结构化并发，一种多线程编程方法，目的是为了通过结构化并发 API 来简化多线程编程，并不是为了取代`java.util.concurrent`，目前处于孵化器阶段。<br />结构化并发将不同线程中运行的多个任务视为单个工作单元，从而简化错误处理、提高可靠性并增强可观察性。也就是说，结构化并发保留了单线程代码的可读性、可维护性和可观察性。<br />结构化并发的基本 API 是**`**[**StructuredTaskScope**](https://download.java.net/java/early_access/loom/docs/api/jdk.incubator.concurrent/jdk/incubator/concurrent/StructuredTaskScope.html)**`**。`StructuredTaskScope` 支持将任务拆分为多个并发子任务，在它们自己的线程中执行，并且子任务必须在主任务继续之前完成。<br />`StructuredTaskScope` 的基本用法如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663047728866-311272a7-10dc-4fd9-8ad8-c0e834688f70.png#clientId=u26e76f4f-bfd7-4&errorMessage=unknown%20error&from=paste&id=u2d30e115&originHeight=492&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=error&style=none&taskId=u7dc6a8b5-ddad-40cb-80fd-9a6bd09fb99&title=)<br />此类允许开发人员将任务构建为一系列并发子任务，并将它们作为一个单元进行协调。子任务在它们自己的线程中执行，方法是将它们单独分叉(fork)，然后将它们作为一个单元加入，并可能将它们作为一个单元取消( cancel )。子任务的成功结果或异常由父任务聚合和处理。`StructuredTaskScope`将子任务或分叉的生命周期限制在明确的词法范围内，这样可以像写单线程代码一样来写多线程代码。官方给来一个例子：
```java
Response handle() throws ExecutionException, InterruptedException {
    try (var scope = new StructuredTaskScope.ShutdownOnFailure()) {
        Future<String>  user  = scope.fork(() -> findUser());
        Future<Integer> order = scope.fork(() -> fetchOrder());

        scope.join();           // Join both forks
        scope.throwIfFailed();  // ... and propagate errors

        // Here, both forks have succeeded, so compose their results
        return new Response(user.resultNow(), order.resultNow());
    }
}
```
结构化并发非常适合虚拟线程，虚拟线程是 JDK 实现的轻量级线程。许多虚拟线程共享同一个操作系统线程，从而允许非常多的虚拟线程。
<a name="pdUW5"></a>
## 参考资料
JEP 405: Record Patterns（记录模式）: [_https://openjdk.org/jeps/405_](https://openjdk.org/jeps/405)<br />JEP 422: Linux/RISC-V Port: [_https://openjdk.org/jeps/422_](https://openjdk.org/jeps/422)<br />JEP 424: Foreign Function & Memory API（外部函数和内存 API）: [_https://openjdk.org/jeps/424_](https://openjdk.org/jeps/424)<br />JEP 425: Virtual Threads（虚拟线程）: [_https://openjdk.org/jeps/425_](https://openjdk.org/jeps/425)<br />JEP 426: Vector（向量）API: [_https://openjdk.java.net/jeps/426_](https://openjdk.java.net/jeps/426)<br />JEP 427: Pattern Matching for switch（switch 模式匹配）: [_https://openjdk.java.net/jeps/427_](https://openjdk.java.net/jeps/427)<br />JEP 428: Structured Concurrency（结构化并发）: [_https://openjdk.org/jeps/428_](https://openjdk.org/jeps/428)<br />sun.misc.Unsafe: [_https://hg.openjdk.java.net/jdk/jdk/file/tip/src/jdk.unsupported/share/classes/sun/misc/Unsafe.java_](https://hg.openjdk.java.net/jdk/jdk/file/tip/src/jdk.unsupported/share/classes/sun/misc/Unsafe.java)<br />Guide to JNI (Java Native Interface): [_https://www.baeldung.com/jni_](https://www.baeldung.com/jni)<br />JNA: [_https://github.com/java-native-access/jna_](https://github.com/java-native-access/jna)<br />JNR: [_https://github.com/jnr/jnr-ffi_](https://github.com/jnr/jnr-ffi)<br />JavaCPP: [_https://github.com/bytedeco/javacpp_](https://github.com/bytedeco/javacpp)<br />JEP 338: [_https://openjdk.java.net/jeps/338_](https://openjdk.java.net/jeps/338)<br />孵化 API: [_http://openjdk.java.net/jeps/11_](http://openjdk.java.net/jeps/11)<br />JEP 414: [_https://openjdk.java.net/jeps/414_](https://openjdk.java.net/jeps/414)<br />JEP 417: [_https://openjdk.java.net/jeps/417_](https://openjdk.java.net/jeps/417)<br />StructuredTaskScope: [_https://download.java.net/java/early_access/loom/docs/api/jdk.incubator.concurrent/jdk/incubator/concurrent/StructuredTaskScope.html_](https://download.java.net/java/early_access/loom/docs/api/jdk.incubator.concurrent/jdk/incubator/concurrent/StructuredTaskScope.html)
