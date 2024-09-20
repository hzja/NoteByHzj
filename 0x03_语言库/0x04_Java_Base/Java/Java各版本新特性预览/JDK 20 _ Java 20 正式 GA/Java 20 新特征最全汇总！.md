Java<br />Java 20 发布了！自 Java 19 发布以来已经过去了六个月，因此又到了 Java 功能喷涌而出的时候了。浏览 Java20 发布的所有 JEP，并对每个 JEP 进行简要介绍。
<a name="mVWww"></a>
## Amber 项目
Java 20 包含了两个来源于 Amber 项目的功能：

- switch 模式匹配
- Record 模式

Amber 项目的目标是探索和孵化更小、以生产力为导向的 Java 语言功能。
<a name="aNJF2"></a>
### JEP 433：switch 模式匹配（第四预览版）
Java 17 中首次引入的“switch 模式匹配”功能已经进入第四个预览阶段，现在 Java 20 已经发布。这个功能过去一直收集了很多反馈，而且还需要与相关的 Record 模式预览功能保持一致。最终有很多理由让它在预览状态下再保持一段时间。<br />自从 Java 16 以来，就可以通过使用“instanceof 模式匹配”来避免在 `instanceof` 检查之后进行强制类型转换。看看在代码示例中它是如何工作的。
```java
static String apply(Effect effect) {
    String formatted = "";
    if (effect instanceof Delay de) {
        formatted = String.format("Delay active of %d ms.", de.timeInMs());
    } else if (effect instanceof Reverb re) {
        formatted = String.format("Reverb active of type %s and roomSize %d.", re.name(), re.roomSize());
    } else if (effect instanceof Overdrive ov) {
        formatted = String.format("Overdrive active with gain %d.", ov.gain());
    } else if (effect instanceof Tremolo tr) {
        formatted = String.format("Tremolo active with depth %d and rate %d.", tr.depth(), tr.rate());
    } else if (effect instanceof Tuner tu) {
        formatted = String.format("Tuner active with pitch %d. Muting all signal!", tu.pitchInHz());
    } else {
        formatted = String.format("Unknown effect active: %s.", effect);
    }
    return formatted;
}
```
这段代码还是挺有仪式感的。此外还留下一个小漏洞——如果添加了一个 else-if 分支，却没有给 formatted 分配任何内容会怎么样呢？因此，遵循这个 JEP（以及它的前身）的精神，看看在 switch 语句中使用模式匹配会是什么样子：
```java
static String apply(Effect effect) {
    return switch(effect) {
        case Delay de      -> String.format("Delay active of %d ms.", de.timeInMs());
        case Reverb re     -> String.format("Reverb active of type %s and roomSize %d.", re.name(), re.roomSize());
        case Overdrive ov  -> String.format("Overdrive active with gain %d.", ov.gain());
        case Tremolo tr    -> String.format("Tremolo active with depth %d and rate %d.", tr.depth(), tr.rate());
        case Tuner tu      -> String.format("Tuner active with pitch %d. Muting all signal!", tu.pitchInHz());
        case null, default -> String.format("Unknown or empty effect active: %s.", effect);
    };
}
```
使用 switch 的模式匹配使代码更加优雅。甚至可以通过为可能的 null 定义一个特定的 case 或将其与默认 case 合并来处理它们。<br />除了模式匹配之外，还可以使用 guard 来检查额外的条件（在下面的代码中，在 when 关键字后面的部分）。
```java
static String apply(Effect effect, Guitar guitar) {
    return switch(effect) {
        case Delay de      -> String.format("Delay active of %d ms.", de.timeInMs());
        case Reverb re     -> String.format("Reverb active of type %s and roomSize %d.", re.name(), re.roomSize());
        case Overdrive ov  -> String.format("Overdrive active with gain %d.", ov.gain());
        case Tremolo tr    -> String.format("Tremolo active with depth %d and rate %d.", tr.depth(), tr.rate());
        case Tuner tu when !guitar.isInTune() -> String.format("Tuner active with pitch %d. Muting all signal!", tu.pitchInHz());
        case Tuner tu      -> "Guitar is already in tune.";
        case null, default -> String.format("Unknown or empty effect active: %s.", effect);
    };
}
```
这里的 guard 确保复杂的布尔逻辑仍然可以用简洁的方式表示。在 case 分支中嵌套 if 语句来测试这种逻辑不仅更冗长，而且还可能引入一开始想避免的小错误。
<a name="l7vfv"></a>
#### Java 19 和 Java 20 相比有什么不同
与 Java 19 相比，这个功能做了一些改变：

- 当一个模式匹配突然失败时，现在会抛出一个 MatchException
- 在 switch 表达式和语句中支持推断 record 模式的类型参数。这意味着现在可以在要匹配的模式中使用 var
<a name="GAY5u"></a>
### JEP 432：Record 模式（第二预览版）
模式匹配是 Java 中逐步推出的一项功能。使用模式解构对象始终是该功能的终极目标之一。现在，通过引入 record 模式，可以解构记录以及嵌套记录和类型模式，以实现强大、声明性和可组合的数据导航和处理形式。<br />Record 是数据的透明载体。接收 Record 实例的代码通常会提取数据，这些数据称为组件。如果假设所有 Effect 接口的实现都是记录，那么在“switch 模式匹配”代码示例中也是如此。在那段代码中，模式变量很明显只用于访问记录字段。使用 Record 模式，可以避免完全创建模式变量：
```java
static String apply(Effect effect) {
    return switch(effect) {
        case Delay(int timeInMs) -> String.format("Delay active of %d ms.", timeInMs);
        case Reverb(String name, int roomSize) -> String.format("Reverb active of type %s and roomSize %d.", name, roomSize);
        case Overdrive(int gain) -> String.format("Overdrive active with gain %d.", gain);
        case Tremolo(int depth, int rate) -> String.format("Tremolo active with depth %d and rate %d.", depth, rate);
        case Tuner(int pitchInHz) -> String.format("Tuner active with pitch %d. Muting all signal!", pitchInHz);
        case null, default -> String.format("Unknown or empty effect active: %s.", effect);
    };
}
```
Delay(int timeInMs) 是一个 Record 模式，将 Delay 实例分解为其组件。当使用嵌套记录模式时，这种机制可以变得更加强大，应用于更复杂的对象图：
```java
record Tuner(int pitchInHz, Note note) implements Effect {}
record Note(String note) {}
class TunerApplier {
    static String apply(Effect effect, Guitar guitar) {
        return switch(effect) {
            case Tuner(int pitch, Note(String note)) -> String.format("Tuner active with pitch %d on note %s", pitch, note);
        };
    }
}
```
嵌套的记录模式也受益于类型参数推断。例如：
```java
class TunerApplier {
    static String apply(Effect effect, Guitar guitar) {
        return switch(effect) {
            case Tuner(var pitch, Note(var note)) -> String.format("Tuner active with pitch %d on note %s", pitch, note);
        };
    }
}
```
这里对于嵌套模式 `Tuner(var pitch, Note(var note))` 的类型参数进行了推断。目前仅支持嵌套模式的隐式类型推断；类型模式尚不支持隐式类型参数的推断。因此，类型模式 Tuner tu 总是被视为原始类型模式。
<a name="yIwxk"></a>
### 增强的 for 语句
记录模式现在也被允许用于增强的 for 语句中，这使得循环遍历记录值的集合并快速提取每个记录的组件变得容易：
```java
record Delay(int timeInMs) implements Effect {}

class DelayPrinter {
    static void printDelays(List<Delay> delays) {
        for (Delay(var timeInMs) : delays) {
            System.out.println("Delay found with timeInMs=" + timeInMs);
        }
    }
}
```
<a name="zYIYe"></a>
#### Java 19 与 Java 20 的不同之处
与 Java 19 相比，此功能进行了以下更改：

- 添加对泛型记录模式类型参数推断的支持；
- 添加对记录模式出现在增强for语句标题中的支持。
<a name="X7QKZ"></a>
## Loom 项目
Java 20 包含三个源于 Loom 项目 的功能：

- 虚拟线程
- 有作用域的值
- 结构化并发

Loom 项目旨在通过引入虚拟线程和结构化并发 API 等方式来简化 Java 中并发应用程序的维护。
<a name="MXpFZ"></a>
### JEP 436：虚拟线程（第二个预览版）
自 Java 诞生以来，线程一直是 Java 的一部分，自从 Loom 项目开始，渐渐开始将其称为“平台线程”。平台线程在底层操作系统线程上运行 Java 代码，并在代码的整个生命周期中捕获操作系统线程。因此，平台线程的数量受限于可用的操作系统线程数。<br />然而，现代应用程序可能需要比这更多的线程。例如，在同时处理成千上万个请求时。这就是虚拟线程的作用。虚拟线程是 java.lang.Thread 的实例，它在底层操作系统线程上运行 Java 代码，但不会在代码的整个生命周期中捕获操作系统线程。这意味着许多虚拟线程可以在同一个操作系统线程上运行它们的 Java 代码，有效地共享它。因此，虚拟线程的数量可以比可用的操作系统线程数量多得多。<br />虚拟线程不仅数量众多，而且创建和处理开销也很低。这意味着一个 Web 框架可以将一个新的虚拟线程专门用于处理请求的任务，并且仍然能够处理成千上万甚至数百万个请求。
<a name="mxxOT"></a>
#### 典型用例
使用虚拟线程不需要学习新概念，但可能需要放弃开发用于应付现代高成本线程的习惯。虚拟线程不仅将帮助应用程序开发人员，而且还将帮助框架设计人员提供易于使用的 API，这些 API 与平台的设计兼容，而不会影响可扩展性。
<a name="CUP4o"></a>
#### 创建虚拟线程
与平台线程一样，虚拟线程也是 java.lang.Thread 的一个实例。因此，可以像使用平台线程一样使用虚拟线程。创建虚拟线程与创建平台线程有所不同，但同样容易：
```java
Response handle() throws IOException {
    String theUser = findUser();
    int theOrder = fetchOrder();
    return new Response(theUser, theOrder);
}
```
当代码已经使用了 `ExecutorService` 接口时，切换到虚拟线程工作量甚至更少：
```java
Response handle() throws ExecutionException, InterruptedException {
    try (var scope = new StructuredTaskScope.ShutdownOnFailure()) {
        Future < String > user = scope.fork(() - > findUser());
        Future < Integer > order = scope.fork(() - > fetchOrder());
        scope.join(); // Join both forks
        scope.throwIfFailed(); // ... and propagate errors
        // Here, both forks have succeeded, so compose their results
        return new Response(user.resultNow(), order.resultNow());
    }
}
```
注意：Java 19 中的 ExecutorService 接口进行了调整，扩展了 AutoCloseable。因此，现在可以在 try-with-resources 结构中使用它。
<a name="LSjq9"></a>
#### 与 Java 19 有何不同
此功能处于“第二个预览版”阶段，以便获得更多反馈。除此之外，还有一些API更改已经成为常规功能，不再提供预览。这是因为它们涉及的功能通常很有用，不仅适用于虚拟线程，还包括：<br />Thread 类中的新方法：

- `join(Duration);`
- `sleep(Duration);`
- `threadId().`

Future 中的新方法（用于检查任务状态和结果）<br />`ExecutorService` 扩展 `AutoCloseable`，以便可以在 try-with-resources 块中使用<br />除此之外，ThreadGroup 的降级也已经成为常规功能。
<a name="aAU9h"></a>
### JEP 429： 作用域值 Scoped Value（孵化器）
作用域值可以在线程内和线程之间共享不可变数据。它们比线程本地变量更好用，特别是使用大量虚拟线程时。
<a name="eD0rN"></a>
#### ThreadLocal
自Java 1.2 以来，可以使用 ThreadLocal 变量将某个值限制在创建它的线程中。在某些情况下，这可能是实现线程安全的简单方法。<br />但是，线程本地变量也有一些注意事项。每个线程本地变量都是可变的，这使得很难区分哪个组件更新了共享状态以及以何种顺序。还存在内存泄漏的风险，因为除非在 ThreadLocal 上调用 `remove()`，否则数据将保留直到垃圾回收（仅在线程终止后才会发生）。最后，父线程的线程本地变量可以被子线程继承，这导致子线程必须为在父线程中先前编写的每个线程本地变量分配存储。<br />现在虚拟线程被引入后，这些缺点变得更加明显，因为可能会有数百万个虚拟线程处于活动状态，即每个线程都有自己的线程本地变量，这将导致显着的内存占用。
<a name="B35lw"></a>
#### 作用域值
与线程本地变量类似，作用域值有多个实例，每个线程一个。不同之处在于，作用域值只写入一次，然后是不可变的，并且仅在线程执行期间的有界时间内可用。<br />以下是 JEP 用伪代码示例说明如何使用作用域值：
```java
final static ScopedValue < ... > V = ScopedValue.newInstance();
// In some method
ScopedValue.where(V, < value > )
    .run(() - > { ...V.get()...call methods...
    });
// In a method called directly or indirectly from the lambda expression
...V.get()...
```
可以看到，调用 `ScopedValue.where(...)` 方法会返回一个作用域范围的值和一个要绑定到该值的对象。调用 `run(...)` 方法会绑定该作用域的值，提供一个特定于当前线程的实例，然后执行作为参数传递的 lambda 表达式。在调用 `run(...)` 方法的生命周期内，lambda 表达式或任何直接或间接从该表达式调用的方法都可以通过值的 `get()` 方法读取作用域范围的值。在 `run(...)` 方法结束后，绑定会被销毁。<br />**典型应用场景**<br />作用域范围的值在所有当前使用单向传输不变数据的线程本地变量的地方都很有用。
<a name="BZj9Z"></a>
#### 与 Java 19 有什么不同
Java 19 中还没有任何与作用域范围的值相关的内容，因此 Java 20 是第一次尝试使用它们。<br />请注意：JEP 处于孵化器阶段，因此需要在命令行中添加 `--enable-preview --add-modules jdk.incubator.concurrent` 才能试用该功能。
<a name="m0qGD"></a>
### JEP 437：结构化并发（第二个孵化器版本）
Java 当前的并发实现是非结构化的，这可能会使处理多个任务的错误处理和取消变得困难。当异步启动多个任务时，如果第一个任务返回错误，目前无法取消剩余的任务。<br />通过 JEP 中的示例代码来说明这一点：
```java
Response handle() throws ExecutionException, InterruptedException {
    Future < String > user = executor.submit(() - > findUser());
    Future < Integer > order = executor.submit(() - > fetchOrder());
    String theUser = user.get(); // Join findUser    int theOrder = order.get();  // Join fetchOrder    return new Response(theUser, theOrder);}
```
当 `user.get()` 调用出错时，没有办法取消第二个任务，从而防止获取一个不会被使用的结果。<br />不过，如果将此代码重写为只使用一个线程，情况将变得简单得多：
```java
Response handle() throws IOException {
    String theUser = findUser();
    int theOrder = fetchOrder();
    return new Response(theUser, theOrder);
}
```
在 Java 中，如果任务及其子任务之间的父子关系在语法上得到了表达，那么多线程编程将更加容易、可靠和可观察 - 就像单线程代码一样。语法结构将定义子任务的生命周期，并启用一个运行时表示线程间层次结构，从而实现错误传播和取消以及并发程序的有意义观察。<br />这就是所谓的“结构化并发”。现在使用新的 StructuredTaskScope API 对代码示例进行重写：
```java
Response handle() throws ExecutionException, InterruptedException {
    try (var scope = new StructuredTaskScope.ShutdownOnFailure()) {
        Future < String > user = scope.fork(() - > findUser());
        Future < Integer > order = scope.fork(() - > fetchOrder());
        scope.join(); // Join both forks      scope.throwIfFailed();  // ... and propagate errors      // Here, both forks have succeeded, so compose their results      return new Response(user.resultNow(), order.resultNow());  }}
```
在结构化并发中，子任务代表任务工作。任务等待子任务的结果并监视它们的失败。`StructuredTaskScope` 类允许开发人员将任务结构化为并发子任务家族，并将它们作为一个单元协调。子任务通过单独分叉它们在自己的线程中执行，然后将它们作为一个单元加入并可能作为一个单元取消。父任务处理子任务的成功结果或异常。<br />与原始示例相比，这里涉及的线程的生命周期理解起来很容易。在所有情况下，它们的生命周期都限于词法作用域，即 try-with-resources 语句的主体。此外，使用 `StructuredTaskScope` 确保了许多有价值的属性：

- **短路错误处理：** 如果子任务失败，则另一个子任务将被取消（如果它尚未完成）。这由 `ShutdownOnFailure` 实现的取消策略管理；其他策略，如 `ShutdownOnSuccess` 也是可用的。
- **取消传播：** 如果在调用 join() 之前或期间中断运行 `handle()` 的线程，则当该线程退出作用域时，两个分叉将自动取消。
- **清晰可见：** 上面的代码具有明确的结构：设置子任务，等待它们完成或被取消，然后决定是成功（并处理子任务的结果，这些结果已经完成）还是失败（子任务已经完成，因此没有更多的清理工作）。

顺便说一下，结构化并发正好与虚拟线程同时出现在 Java 中，这绝非偶然。现代 Java 程序可能会使用大量的线程，并且需要正确而稳健地协调它们。结构化并发正好可以提供这种功能，同时还可以使观察工具按照开发人员的理解显示线程。
<a name="ry5Rm"></a>
#### 与 Java 19 有何不同
情况与 Java 19 中的情况大致相同（请参见 JEP 428）。唯一的变化是更新了 `StructuredTaskScope`，以使其支持在任务作用域中创建的线程继承作用域值。这简化了在线程之间共享不可变数据。再次注意，JEP 处于孵化器阶段，因此需要将 `--enable-preview --add-modules jdk.incubator.concurrent` 添加到命令行中，以便能够使用该功能。
<a name="pmZ2O"></a>
## Panama 项目
Java 20 包含了两个源自 Panama 项目的功能：

- 外部函数与内存 API
- Vector API

Panama 项目旨在改善 JVM 与外部（非 Java）库之间的连接。
<a name="ANilp"></a>
### JEP 434：外部函数 & 内存 API（第二个预览版）
Java 程序一直可以选择与 Java 运行时外部的代码和数据进行交互，可以使用 Java 本地接口（JNI）调用外部函数（在 JVM 之外但在同一台机器上）。使用 ByteBuffer API 或 sun.misc.Unsafe API 可以访问外部内存（在 JVM 之外，因此是堆外内存）。<br />但是，这三种机制都具有自己的缺点，因此现在提出了更现代的 API 以更好地支持外部函数和外部内存。<br />性能关键的库（如 Tensorflow、Lucene 或 Netty）通常依赖使用外部内存，因为它们需要更多地控制它们使用的内存以防止垃圾回收带来的成本和不可预测性。
<a name="nCIwq"></a>
#### 示例代码
为了演示新 API，JEP 434 列出了一个代码示例，该示例获取 C 库函数 radixsort 的方法句柄，然后将其用于排序最初作为 Java 数组元素的四个字符串：
```java
// 1. Find foreign function on the C library path

Linker linker          = Linker.nativeLinker();
SymbolLookup stdlib    = linker.defaultLookup();
MethodHandle radixsort = linker.downcallHandle(stdlib.find("radixsort"), ...);
// 2. Allocate on-heap memory to store four strings
String[] javaStrings = { "mouse", "cat", "dog", "car" };
// 3. Use try-with-resources to manage the lifetime of off-heap memory
try (Arena offHeap = Arena.openConfined()) {
    // 4. Allocate a region of off-heap memory to store four pointers
    MemorySegment pointers = offHeap.allocateArray(ValueLayout.ADDRESS, javaStrings.length);
    // 5. Copy the strings from on-heap to off-heap
    for (int i = 0; i < javaStrings.length; i++) {
        MemorySegment cString = offHeap.allocateUtf8String(javaStrings[i]);
        pointers.setAtIndex(ValueLayout.ADDRESS, i, cString);
    }
    // 6. Sort the off-heap data by calling the foreign function
    radixsort.invoke(pointers, javaStrings.length, MemorySegment.NULL, '\0');
    // 7. Copy the (reordered) strings from off-heap to on-heap
    for (int i = 0; i < javaStrings.length; i++) {
        MemorySegment cString = pointers.getAtIndex(ValueLayout.ADDRESS, i);
        javaStrings[i] = cString.getUtf8String(0);
    }
} // 8. All off-heap memory is deallocated here
assert Arrays.equals(javaStrings, new String[] {"car", "cat", "dog", "mouse"});  // true
```
仔细看看代码中用到的一些类型，了解它们在外部函数和内存 API 中的功能和目的：

- **Linker：**提供了从Java代码访问外部函数和从外部函数访问Java代码的功能。它通过downcall方法句柄允许Java代码链接到外部函数。它还通过生成upcall stubs允许外部函数调用Java方法句柄。有关更多信息，请参见此类型的JavaDoc。
- **SymbolLookup：**检索一个或多个库中符号的地址。有关更多信息，请参见此类型的JavaDoc。
- **Arena:** 控制内存段的生命周期。Arena具有称为竞技场范围的范围。当竞技场关闭时，竞技场范围不再存在。因此，与竞技场范围关联的所有段都无效，它们的支撑内存区域被释放（在适用的情况下），并且在竞技场关闭后不能再访问它们。有关更多信息，请参见此类型的JavaDoc。
- **MemorySegment：**提供对连续内存区域的访问。有两种类型的内存段：heap segments（在Java内存堆中）和native segments（在Java内存堆之外）。有关更多信息，请参见此类型的JavaDoc。
- **ValueLayout：**对基本数据类型的值进行建模，例如integral值、floating-point值和address值。除此之外，它还为Java原始类型和地址定义了有用的值布局常量。有关更多信息，请参见此类型的JavaDoc。
<a name="ZI6qF"></a>
#### 与 Java 19 有何不同
在 Java 19 中，此功能处于第一次预览状态（以 JEP 424 形式存在），因此语言功能已经完成并收集了开发者反馈。基于这些反馈，Java 20 发生了以下变化：

- 合并了 MemorySegment 和 MemoryAddress 抽象（内存地址现在由零长度的内存段建模）；
- 密封的 MemoryLayout 层次结构得到了增强，以便与 switch 模式匹配一起使用；
- MemorySession 已被分成 Arena 和 SegmentScope，以便跨维护边界共享段。
<a name="BWVfo"></a>
### JEP 438  Vector API（第五个孵化器版本）
Vector API 将使得能够表达可靠地在运行时编译为最优向量指令的向量计算。这意味着这些计算将在支持的 CPU 体系结构（x64 和 AArch64）上显着优于等价的标量计算。<br />向量（Vector）计算是对一个或多个任意长度的一维矩阵进行的数学运算。将向量视为具有动态长度的数组。此外，可以通过索引在常数时间内访问向量中的元素，就像数组一样。<br />过去，Java程序员只能在汇编代码级别上编写此类计算。但现在，现代 CPU 支持高级单指令，多数据（SIMD）功能，因此重要性更大，需要利用 SIMD 指令和多个并行操作的性能增益。Vector API让Java程序员更容易实现这一点。
<a name="uTYdB"></a>
#### 示例代码
以下是一段代码示例（取自 JEP），它比较了使用数组元素的简单标量计算以及使用 Vector API 的等效计算：
```java
void scalarComputation(float[] a, float[] b, float[] c) {
    for (int i = 0; i < a.length; i++) {
        c[i] = (a[i] * a[i] + b[i] * b[i]) * -1.0f;
    }
}

static final VectorSpecies<Float> SPECIES = FloatVector.SPECIES_PREFERRED;

void vectorComputation(float[] a, float[] b, float[] c) {
    int i = 0;
    int upperBound = SPECIES.loopBound(a.length);
    for (; i < upperBound; i += SPECIES.length()) {
        // FloatVector va, vb, vc;
        var va = FloatVector.fromArray(SPECIES, a, i);
        var vb = FloatVector.fromArray(SPECIES, b, i);
        var vc = va.mul(va)
        .add(vb.mul(vb))
        .neg();
        vc.intoArray(c, i);
    }
    for (; i < a.length; i++) {
        c[i] = (a[i] * a[i] + b[i] * b[i]) * -1.0f;
    }
}
```
从 Java 开发者的角度来看，这只是表达标量计算的另一种方式。它可能会显得略微冗长，但另一方面它可以带来惊人的性能提升。
<a name="Msc7F"></a>
#### 典型用例
Vector API 提供了一种在 Java 中编写复杂向量算法的方法，例如向量化的 hashCode 实现或专门的数组比较。许多领域可以从中受益，包括机器学习、线性代数、加密、文本处理、金融和 JDK 本身的代码。
<a name="SNj7k"></a>
### 与 Java 19 有何不同
除了一小部分错误修复和性能增强外，这个特性与 Valhalla 项目的对齐是与 Java 19 最大的不同之处。它的意义是非常明确的，因为向量 API 和 Valhalla 项目都专注于性能提升。<br />回想一下，Valhalla 项目的目标是通过值对象和用户自定义原语增强 Java 对象模型，将面向对象编程的抽象与简单原语的性能特性相结合。<br />一旦 Valhalla 项目的功能可用，将会适应 Vector API 以利用值对象，届时它将被提升为预览功能。
