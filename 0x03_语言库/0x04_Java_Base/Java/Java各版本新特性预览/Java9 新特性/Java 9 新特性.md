Java 9 发布于 2017 年 9 月 22 日。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1701396907074-33178d8e-2f9d-46f9-a85d-e6bd3662eb53.jpeg)
<a name="FnE5c"></a>
## **JEP 261：模块系统**
在传统的 Java 应用中，类路径的机制限制了封装性。即使类被标记为 package-private，也可以被同一个类路径中的其他任意代码访问。而且随着应用程序规模的增长，类路径依赖变得越来越复杂。<br />Java 为了能够改进大型应用程序和库的封装性、可维护性和性能，在 Java 9 版本引入模块系统。模块系统是 Java 平台架构的一次重大变革，它旨在解决长期以来 Java 应用所面临的一些结构性问题，特别是在大型系统和微服务架构中。<br />其主要内容是：

- **模块化 JDK**：JDK 被划分为一系列的模块，使得可以只需要引入必需的 JDK 模块，减少了应用程序的体积。
- **模块路径**：取代了传统的类路径，用于指定模块的位置。
- **模块声明**：使用 module-info.java 文件来声明模块，指定模块所需的其他模块和模块导出的包。

使用方法是：<br />新建模块后，就需要在 module-info.java 中定义模块信息了，信息主要包括如下几个部分：

1. 使用`module`关键字定义模块，并指定模块的名称，例如：`module java.module01 { }`。
2. 使用`requires`关键字声明模块之间的依赖关系，例如：`requires java.sql;` 表示模块依赖于java.sql模块。
3. 使用`exports`关键字声明模块中哪些包可以被其他模块访问，例如：`exports com.skjava.module01.entity;` 表示导出com.skjava.module01.entity包。

模块化系统带来三大好处：

1. **更好的封装性**：允许封装内部实现，只暴露必要的 API，从而减少了意外的依赖。
2. **性能提升**：模块化可以帮助 JVM 更高效地加载代码，提高启动速度和降低内存占用。
3. **更好的安全性**：可以限制哪些模块可以访问 JDK 的特定部分，从而提高安全性。
<a name="crvbq"></a>
## **JEP 269：集合工厂方法**
在 Java 9 之前创建一个不可变集合通常涉及多个步骤，包括创建、填充和包装。Java 9 引入该特性旨在提供一种简洁、安全且不可变的方式来创建集合（List、Set、Map）。<br />其内容包括：

- `List.of()`：创建一个不可变的 List，可以传递任意数量的元素。
- `Set.of()`：创建一个不可变的 Set，元素不可重复。
- `Map.of()` 和 `Map.ofEntries()`：用于创建一个不可变的 Map。`Map.of()` 可以直接传递键值对，而 `Map.ofEntries()` 可以通过 `Map.entry(k, v)` 创建条目。
<a name="KV6pS"></a>
## **JEP 222：Jshell**
在传统的 Java 开发中，即使是为了运行一个简单的程序，也需要编写完整的类和方法，然后编译和运行，这种方式对初学者很不友好，而且也不利于想要快速验证算法或逻辑的需求。<br />所以，Java 9 引入 Java Shell，其目的是提供一个官方的 Java REPL，支持快速测试、探索和实验 Java 代码片段。<br />Java Shell，是一个交互式的 Java REPL（Read-Eval-Print Loop），即一个用于交互式地执行 Java 代码的命令行工具。它的主要特点是：

- **交互式执行**：可以直接在命令行中输入并执行 Java 代码，无需创建完整的类文件。
- **自动补全**：支持代码自动补全，提高编码效率。
- **临时变量和历史记录**：自动为表达式的结果分配临时变量，并保存命令历史，方便回顾和修改。
- **错误诊断**：即使代码片段出错，JShell 也可以继续运行，提供错误信息帮助诊断问题。

能带来三大好处：

- **提高效率**：允许快速测试小片段的代码，而无需编写完整的应用程序。
- **便于学习**：为初学者提供了一个友好的环境，帮助他们理解 Java 语言的基础概念。
- **快速原型验证**：可以迅速验证想法和算法，加速开发过程。
<a name="MgpDf"></a>
## **JEP 213：接口支持私有方法**
我们知道 Java 8 支持默认方法和静态方法，虽然提高了 Java 接口的灵活性和扩展性，但他们限制了接口的封装性和复用性。<br />为了提高接口的封装性和代码复用性，Java 9 支持在接口中定义私有方法和私有静态方法。

- **私有方法**：可以在接口内部定义私有方法，以帮助实现默认方法或其他私有方法。
- **私有静态方法**：类似地，可以定义私有静态方法，用于辅助接口内部的静态方法。

引入接口私有方法，除了增强接口的封装性和代码复用外，还能够使接口的设计者可以更清晰地区分公共 API 和内部实现细节，从而提供更干净、更简洁的公共 API。
<a name="om6qX"></a>
## **Stream API 增强**
Java 9 对 Stream API 新增了几个方法，这些方法使得 Stream 在处理数据流变得更加方便和强大了。

1. `takeWhile()`：允许从流的开始处理元素，直到给定的谓词返回 false。这在处理有序流时特别有用。
2. `dropWhile()`：与 `takeWhile()` 相反，它从流的开始丢弃元素，直到谓词返回 false，然后处理剩余的元素。
3. `ofNullable()`：用于创建单元素流，如果元素是 null，则返回一个空流，避免了 NullPointerException。
4. `iterate()` 的新重载：在 Java 8 中，`iterate()` 方法是无限的。Java 9 添加了一个重载，允许你提供一个谓词作为终止条件，这样就可以创建有限的流。
<a name="mnSMH"></a>
## **Optional 的增强**
Optional 是在 Java 8 中引入的，主要是用来解决 NullPointerException 的。Java 9 引入 3 个方法，进一步增强 Optional 的使用场景。

1. `stream()`：允许将 Optional 对象转换为一个（最多只有一个元素的）流。这在将多个 Optional 对象组合到一个流中时特别有用。
2. `ifPresentOrElse()`：这个方法允许执行一个操作，如果 Optional 包含值，则执行一个操作，否则执行另一个操作。这提供了类似于“if-else”语句的功能。
3. `or()`：允许在当前的 Optional 为空时，提供一个替代的 Optional。这类似于 `orElse()` 和 `orElseGet()`，但返回的是 Optional 对象而不是值。
<a name="KwBAk"></a>
## **改进 try-with-resources**
try-with-resources 是在 Java 7 中引入的，它能够完成资源的自动管理，例如文件和套接字的关闭，但是它需要在 try 语句内部声明和初始化，尽管它已经在外部声明了，这导致了代码的重复和冗余。<br />Java 9 对其进行了一个小的改动：允许使用在 try 语句块外部声明的资源。这意味着如果资源已经是 final 或者 effectively final（即实际上没有被后续代码修改），就可以在 try-with-resources 语句中直接使用，而无需在 try 语句内再声明一个新的局部变量。例如：

- Java 7
```java
BufferedReader reader = new BufferedReader(new FileReader("file.txt"));
try (BufferedReader r = reader) {
    // 使用 reader
}
```

- Java 9
```java
BufferedReader reader = new BufferedReader(new FileReader("file.txt"));
try (reader) {
    // 使用 reader
}
```
<a name="e14pJ"></a>
## **JEP 102：Process API**
在 Java 9 之前，Java 提供的进程控制功能相对有限，很难直接获取关于系统进程的详细信息，也难以管理这些进程。这导致开发者需要依赖于特定平台的代码来完成这些任务，降低了代码的可移植性和易用性。<br />为了使 Java 应用能够更方便、更有效地管理和控制操作系统级别的进程，Java 9 引入 Process API，其目的是为了提供更好的控制和管理操作系统进程的能力，并使其在不同操作系统上的行为更加一致。其主要内容包括：

- **增强的 Process 类**：Java 9 增强了 `Process` 类，提供了更多方法来管理和控制进程。
- `**ProcessHandle**`** 接口**：引入了 `ProcessHandle` 接口，它提供了获取进程的 PID（进程标识符）、父进程、子进程、进程状态等信息的能力。
- **流式 API**：利用流式 API，可以更方便地处理进程的信息和状态。

下面是获取本地所有进程的相关信息：
```java
@Test
public void processHandleTest() {
    ProcessHandle.allProcesses() // 获取所有进程
            .forEach(processHandle -> {
                System.out.printf("Process ID: %s, Command: %s, Start Time: %s, User: %s%n",
                        processHandle.pid(),                                                  // 获取进程ID
                        processHandle.info().command().orElse("Unknown"),               // 获取进程的命令信息
                        processHandle.info().startInstant()
                                .map(i -> i.toString()).orElse("Unknown"),              // 获取进程的启动时间
                        processHandle.info().user().orElse("Unknown"));                 // 获取运行进程的用户
            });

}
```
执行结果（部分）：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1701396583679-fea7db77-fb35-42eb-8489-c2f8a4b2edcf.png#averageHue=%23f3f1f0&clientId=ue520fb3f-dfaa-4&from=paste&height=336&id=u626da0bd&originHeight=841&originWidth=2419&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=328634&status=done&style=none&taskId=u004135b1-6520-4255-885d-dfc98b4046c&title=&width=967.6)
<a name="sum9D"></a>
## **JEP 264：平台日志 API 和 服务**
在 Java 9 之前，JDK 内部使用了多种日志记录机制，比如 `System.out.println` 和 `java.util.logging`，这种方法缺乏统一性，使得维护和控制日志变得复杂。因此，需要一个统一的日志系统，使 JDK 自身的日志更加一致和可管理。<br />Java 9 引入该特性其主要目的是为 JDK 提供一个统一的日志系统，它能够通过不同的日志框架来捕获 JDK 内部的日志信息。这不仅简化了 JDK 自身的日志处理，也为开发者提供了更大的灵活性和控制力，使得他们能够更好地管理和监控 JDK 产生的日志信息。<br />主要内容：

1. **新的日志API**：引入了一组新的日志API，称为 `System.Logger` API，用于 JDK 内部日志记录。
2. **日志级别支持**：支持不同的日志级别，例如 ERROR, WARNING, INFO, DEBUG, 和 TRACE。
3. **日志服务接口**：定义了一个服务接口，允许替换JDK的日志记录系统，或者将其桥接到其他日志框架。

下面代码是该日志 API 的示例：
```java
@Test
public void loggerTest() {
    System.Logger logger = System.getLogger("Java9Test");
    logger.log(System.Logger.Level.INFO, "这是 INFO 级别");
    logger.log(System.Logger.Level.WARNING, "这是 WARNING 级别");
    logger.log(System.Logger.Level.ERROR, "这是 ERROR 级别");
}
```
它带来如下几个好处：

1. **更好的日志管理**：统一的API使日志管理变得更加简单和一致。
2. **灵活性和可扩展性**：开发者可以根据需要选择不同的日志实现，提高了灵活性和可扩展性。
3. **更好的集成**：使得JDK与现代日志框架（如 SLF4J, Log4j）之间的集成更加容易和无缝。
<a name="H3Yi8"></a>
## **JEP 266：反应式流（Reactive Streams）**
在传统的阻塞 IO 和同步处理模式中，处理大量数据流或异步操作时常常面临效率和响应性问题。反应式编程是一种更有效地处理异步数据流的编程范式。Java 9 之前，并没有标准的方式在 Java 中实现反应式编程。因此 Java 9 引入反应式流，其目的是提供一种在 Java 中处理异步数据流的标准方式，同时保证高效率、低延迟，并支持背压（back-pressure），即允许接收者控制数据流的速度，防止被快速生产的数据淹没。<br />主要内容为：

- 引入了 `java.util.concurrent.Flow` 类，它包含了几个嵌套的静态接口：`Publisher`、`Subscriber`、`Subscription` 和 `Processor`。
   - `Publisher`：一个数据流的生产者。
   - `Subscriber`：订阅 `Publisher` 并处理数据的消费者。
   - `Subscription`：连接 `Publisher` 和 `Subscriber`，允许 `Subscriber` 控制数据流。
   - `Processor`：充当生产者和消费者的中间人，即 `Publisher` 和 `Subscriber` 的组合。
<a name="NtNGM"></a>
## **JEP 224：HTML5 Javadoc**
在 Java 9 之前，Javadoc 主要使用较老的 HTML 4 格式。随着 Web 标准的发展，特别是 HTML5 的普及，有必要更新 Javadoc 以支持更现代的 Web 技术和标准。<br />Java 9 引入 HTML5 Javadoc，主要目的是将 Javadoc 文档更新为使用 HTML5，从而提高文档的兼容性、可用性和可访问性。这包括支持更多现代浏览器的特性，提供更好的布局和样式，以及改善搜索功能。
<a name="bgXSF"></a>
## **JEP 238：多版本兼容 JAR 文件**
在 Java 9 之前，一个 JAR 文件只能包含针对一个特定 Java 版本编译的类文件。随着 Java 平台的不断发展和版本的迭代，这限制了库和应用程序在不同 Java 版本间的兼容性。<br />Java 9 引入该特性的主要目的是提供一种机制，使得库开发者可以在单个 JAR 文件中包含针对不同 Java 版本编译的类文件。这样，应用程序可以在不同的 Java 运行时环境中运行，而无需更改或重新打包。
<a name="rvjT7"></a>
## **JEP 277：改进的弃用注解 **`**@Deprecated**`
`@Deprecated` 注解用于标记过时的 API，但它并没有提供关于 API 为何过时、何时过时以及替代方案等信息。这导致开发者在使用或维护这些 API 时缺乏足够的信息。Java 9 对其进行了改进，增加了两个的属性：

- `since` 属性用于指明从哪个版本开始 API 被弃用。
- `forRemoval`：指出这个 API 是否计划在未来的版本中被移除。

该项特性可以让开发者能够更清晰地了解 API 的状态和未来规划，比如是否继续使用该 API、寻找替代方案。
<a name="EneJk"></a>
## **JEP213：改进钻石操作符(Diamond Operator)**
在 Java 7 中引入的钻石操作符简化了泛型实例的创建，但它不能用于匿名内部类。由于这个限制，开发者不得不在使用匿名内部类时指定泛型参数，这增加了代码的冗余和复杂性。<br />在 Java 9 中改进了钻石操作符，它可以与匿名内部类一起使用。这意味着当我们在创建一个匿名内部类的实例，并且该类具有泛型参数时，我们可以省略这些参数，Java 编译器会根据上下文推断出正确的类型。
<a name="LocX2"></a>
## **增强 **`**CompletableFuture**`
`CompletableFuture` 是在 Java 8 中引入的，它是一个非常强大的用于异步编程的工具，但是在实际使用过程中，发现它还有一些改进空间。故而，Java 9 对其进行了一些增强，内容如下：
<a name="wOcQI"></a>
### 新增方法

- `completeAsync()`：允许异步地完成 `CompletableFuture`。它受一个 `Supplier` 函数和可选的 `Executor`，用于异步生成结果。
- `orTimeout()`：为 `CompletableFuture` 添加超时功能。如果在指定的时间内未完成，`CompletableFuture` 将会被异常地完成。
- `completeOnTimeout()`：类似于 `orTimeout()`，但在超时发生时，它会使用提供的值来完成 `CompletableFuture`，而不是抛出异常。
<a name="VvIev"></a>
### 改进异常处理

- `exceptionallyCompose()`：它允许在 `CompletableFuture` 遇到异常时，构建并返回一个新的 `CompletionStage`，这为异常处理提供了更多的灵活性。
<a name="yCvRG"></a>
### 增强的组合操作

- `delayedExecutor()`：这是一个工具方法，用于创建一个延迟执行任务的 Executor。它可以和其他 `CompletableFuture` 方法结合使用，实现延迟执行的效果。
- `minimalCompletionStage()` 和 `completeMinimalFuture()`：这两个方法分别用于创建一个具有最小完成状态的 `CompletionStage`，以及从 `CompletionStage` 创建一个 `CompletableFuture`。这些方法有助于在不需要 `CompletableFuture` 完整功能的场景中减少资源消耗。
