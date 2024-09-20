Java17<br />Java 开发工具包 (JDK) 17 将是一个长期支持 (LTS) 版本，预计来自 Oracle 的扩展支持将持续数年。该功能集定于 6 月 10 日冻结，届时 JDK 17 将进入初始阶段。作为 OpenJDK JDK 17 的一部分提交的功能包括：

- 特定于上下文的反序列化过滤器允许应用程序通过调用 JVM 范围的过滤器工厂来配置特定于上下文和动态选择的反序列化过滤器，以便为每个序列化操作选择一个过滤器。在解释该提议背后的动机时，Oracle 表示反序列化不受信任的数据是一种固有的危险活动，因为传入数据流的内容决定了创建的对象、其字段的值以及它们之间的引用。在许多用途中，流中的字节是从未知、不受信任或未经身份验证的客户端接收的。通过仔细构建流，攻击者可以导致恶意执行任意类中的代码。如果对象构造具有改变状态或调用其他操作的副作用，则这些操作可能会危及应用程序对象的完整性，库对象和 Java 运行时。禁用序列化攻击的关键是防止任意类的实例被反序列化，从而防止直接或间接执行它们的方法。反序列化过滤器被引入Java 9使应用程序和库代码能够在反序列化之前验证传入的数据流。代码java.io.ObjectInputFilter在创建反序列化流时提供验证逻辑。但是，依赖流的创建者来明确请求验证有局限性。JDK Enhancement Proposal 290通过引入可通过 API、系统属性或安全属性设置的 JVM 范围的反序列化过滤器解决了这些限制，但这种方法也有局限性，尤其是在复杂的应用程序中。更好的方法是配置每个流过滤器，这样它们就不需要每个流创建者的参与。计划中的增强应帮助开发人员为每个反序列化上下文和用例构建和应用适当的过滤器。
- 随着always-strict 浮点语义，浮点运算将变得始终严格，而不是同时具有严格浮点语义 ( strictfp) 和微妙不同的默认浮点语义。这将原始浮点语义恢复到语言和 VM中，匹配 Java 标准版 1.2 中引入严格和默认浮点模式之前的语义。这项工作的目标包括简化数字敏感库的开发，包括java.lang.Math和java.lang.StrictMath.改变默认浮点语义的动力来自于原始Java语言和JVM语义之间的不良交互作用，以及流行的x86体系结构的x87浮点协处理器指令集的一些特性。在所有情况下匹配精确的浮点语义，包括非正规操作数和结果，需要大量额外指令的开销。在没有溢出或下溢的情况下匹配结果可以用较少的开销完成，而这正是javase1.2中引入的修改后的默认浮点语义所允许的。但是，大约从2001年开始在奔腾4和更高版本的处理器中发布的SSE2（第二代数据流单指令多数据扩展指令集）扩展可以以一种简单的方式支持严格的JVM浮点操作，而不会产生过多的开销。由于Intel和AMD支持SSE2和更高版本的扩展，这些扩展允许自然地支持严格浮点语义，因此使用不同于strict的默认浮点语义的技术动机已不复存在。
- 弃用安全管理器，准备在未来版本中移除。追溯到 Java 1.0，安全管理器一直是保护客户端 Java 代码的主要手段，很少用于保护服务器端代码。该提案的一个目标是评估是否需要新的 API 或机制来解决使用安全管理器的特定狭窄用例，例如阻塞`System::exit`。计划要求弃用安全管理器以与旧 Applet API 一起删除，该 API 也计划在 JDK 17 中弃用。
- 模式匹配switch预览版扩展了 Java 中的模式语言，允许switch针对多个模式测试表达式和语句，每个模式都有特定的操作。这使得复杂的面向数据的查询能够简洁而安全地表达。此功能的目标包括扩展转换表达式和语句允许模式出现在case标签中，减轻了转换如果需要，引入两种模式：guarded patterns允许使用任意布尔表达式对模式匹配逻辑进行优化，以及parenthesized patterns解决了一些解析歧义。在JDK 16，的运算符运算符被扩展为获取类型模式并执行模式匹配。提议的适度扩展允许简化熟悉的instanceof和cast习惯用法。
- JDK 内部的强封装，除了关键的内部 API，如sun.misc.Unsafe，将不再可能通过单个命令行选项放松内部元素的强封装，这在 JDK 9 到 JDK 16 中是可行的。计划包括提高 JDK 的安全性和可维护性，并鼓励开发人员从内部元素迁移到标准 API。
- 删除远程方法调用 (RMI) 激活机制，同时保留 RMI 的其余部分。RMI 激活机制已过时和废弃，在JDK 15 中不推荐使用。
- 在外部函数和内存API引入了一个孵化器阶段，允许 Java 程序与 Java 运行时之外的代码和数据进行互操作。通过高效调用外部函数，即 JVM 之外的代码，安全访问外部内存，即非 JVM 管理的内存，API 使 Java 程序能够调用本地库和处理本地数据，而没有JNI（Java本机接口）的脆弱性和风险。
   - 提议的 API 是两个 API 的演变：外部内存访问 API 和外部链接器 API。
   - 外部内存访问 API 在 2019 年作为孵化 API 面向 Java 14，并在 Java 15 和 Java 16 中重新孵化。外部链接器 API 在 2020 年末面向 Java 16 作为孵化 API。API 计划的目标包括易用性、性能、通用性和安全性。
- 与平台无关的矢量 API作为孵化 API集成到JDK 16中，将在 JDK 17 中再次孵化，提供一种机制来表达矢量计算，这些计算在运行时可靠地编译为支持的 CPU 架构上的最佳矢量指令。这比等效的标量计算获得了更好的性能。在 JDK 17 中，向量 API 已针对性能和实现进行了增强，包括在字节向量与布尔数组之间进行转换的增强功能。
- 密封类和接口限制哪些其他类或接口可以扩展或实现它们。该提案的目标包括允许类或接口的作者控制由哪些代码负责实现它，提供一种比访问修饰符更具声明性的方式来限制超类的使用，并通过为模式的详尽分析提供基础来支持模式匹配的未来方向。
- 删除实验性 AOT 和 JIT 编译器，它们几乎没有使用，但需要大量维护工作。该计划要求维护 Java 级别的 JVM 编译器接口，以便开发人员可以继续使用外部构建的编译器版本进行 JIT 编译。AOT 编译（jaotc 工具）作为一个实验性特性被整合到JDK 9，它本身是用 Java 编写的，用于 AOT 编译。这些实验性功能未包含在JDK 16 中由 Oracle 发布的版本，没有人抱怨。根据规定的计划，将删除三个 JDK 模块：jdk.aot（jaotc 工具）；internal.vm.compiler，Graal 编译器；和 jdk.internal.vm.compiler.management，Graal MBean。与 AOT 编译相关的 HotSpot 代码也将被删除。
- 将 JDK 移植到 MacOS/AArch64以响应Apple 将其 Macintosh 计算机从 x64 转换到 AArch64 的计划。适用于 Linux 的 Java 的 AArch64 端口已经存在，并且 Windows 的工作正在进行中。Java 构建者希望通过使用条件编译来重用来自这些端口的现有 AArch64 代码，这是 JDK 端口中的规范，以适应低级约定的差异，例如应用程序二进制接口和一组保留的处理器寄存器。MacOS/AArch64 的更改可能会破坏现有的 Linux/AArch64、Windows/AArch64 和 MacOS/x64 端口，但通过预集成测试将降低风险。
- 弃用 Applet API 以进行删除。这个 API 本质上是无关紧要的，因为所有 Web 浏览器供应商要么已经取消了对 Java 浏览器插件的支持，要么已经宣布了这样做的计划。Applet API 之前在 2017 年 9 月的java中已被弃用，但并未删除。
- 用于 MacOS 的新渲染管道，使用 Apple Metal API 作为使用已弃用 OpenGL API 的现有管道的替代方案。该提案旨在为使用 MacOS Metal 框架的 Java 2D API 提供功能齐全的渲染管道，并在 Apple 从未来版本的 MacOS 中删除 OpenGL API 时做好准备。该管道旨在与现有的 OpenGL 管道具有同等功能，在选定的应用程序和基准测试中具有相同或更好的性能。将创建适合当前 Java 2D 模型的干净架构。管道将与 OpenGL 管道共存直到过时。添加任何新的 Java 或 JDK API 并不是提案的目标。
- 增强的伪随机数生成器将为伪随机数生成器 (PRNG) 提供新的接口类型和实现，包括可跳转的 PRNG 和额外的一类可拆分 PRNG 算法 (LXM)。新接口RandomGenerator将为所有现有的和新的 PRNG 提供统一的 API。将提供四个专门的 RandomGenerator 接口。推动该计划的重点是 Java 伪随机数生成领域的多个改进领域。这项工作不需要提供许多其他 PRNG 算法的实现。但是已经添加了三种常用算法，这些算法已经广泛部署在其他编程语言环境中。该计划的目标包括：
   - 使在应用程序中交替使用各种 PRNG 算法变得更容易。
   - 改进了对基于流的编程的支持，提供了 PRNG 对象流。
   - 消除现有 PRNG 类中的代码重复。
   - 保留类的现有行为java.util.Random。

9 月 14 日已被定为 JDK 17 的全面可用日期。生产版本之前将在 6 月和 7 月进行斜降阶段，并在 8 月发布候选版本。JDK 17 的早期访问开源版本可以在jdk.java.net上找到。<br />JDK 17 等 LTS 版本每三年发布一次。最后一个 LTS 版本JDK 11于 2018 年 9 月发布。Java 的新版本每六个月发布一次。
<a name="Es5Cy"></a>
## Java17 最值得了解的10个新特性
<a name="IZPTG"></a>
### 1、Switch 表达式的增强
Java 17 在 Switch 表达式方面进行了增强，可以支持 Lambda 表达式和块语句。以下是一个简单的示例：
```java
String day = "MONDAY";
int numLetters = switch (day) {
    case "MONDAY", "FRIDAY", "SUNDAY" -> 6;
    case "TUESDAY"                -> 7;
    case "THURSDAY", "SATURDAY"   -> 8;
    case "WEDNESDAY"              -> 9;
    default                      -> 0;
};
System.out.println(numLetters); // 输出 6
```
<a name="DXBdr"></a>
### 2、新的类型推断机制
Java 17 改进了类型推断机制，允许在 Lambda 表达式和匿名内部类中使用 var 关键字。以下是一个简单的示例
```java
var list = new ArrayList<String>();
list.add("Java 17");
list.add("新特性");
list.forEach(s -> System.out.println(s));
```
<a name="UVRes"></a>
### 3、实例方法引用
Java 17 允许使用 `::` 操作符进行实例方法引用，可以方便地引用一个对象的方法。以下是一个简单的示例
```java
List<Integer> list = Arrays.asList(3, 1, 4, 1, 5, 9);
list.stream().distinct().sorted().forEach(System.out::println);
```
<a name="S7Ypp"></a>
### 4、HTTP 客户端改进
Java 17 对 HTTP 客户端进行了改进，引入了新的 API，简化了 HTTP 请求的编写。以下是一个简单的示例
```java
HttpClient client = HttpClient.newHttpClient();
HttpRequest request = HttpRequest.newBuilder()
        .uri(URI.create("https://www.example.com/"))
        .build();
HttpResponse<String> response = client.send(request, HttpResponse.BodyHandlers.ofString());
System.out.println(response.body());
```
<a name="RZwAd"></a>
### 5、ZGC 的改进
ZGC 是一种基于 Region 的低延迟垃圾收集器，Java 17 引入了以下新特性：

- 卸载类：在不停机的情况下，卸载不需要的类，可以减少内存占用。
- 日志记录：提供了更丰富的日志记录选项，方便进行调试和性能分析。

下面是使用 ZGC 的示例代码
```java
// 启用 ZGC
java -XX:+UnlockExperimentalVMOptions -XX:+UseZGC -jar MyApp.jar

// 在代码中启用 ZGC
VMOption option = new VMOption("-XX:+UnlockExperimentalVMOptions");
VMOption option2 = new VMOption("-XX:+UseZGC");
List<VMOption> options = Arrays.asList(option, option2);
Launcher launcher = Launcher.createLauncher(options.toArray(new VMOption[0]));
launcher.launch("com.example.MyApp", "arg1", "arg2");
```
<a name="KLa9D"></a>
### 6、C++ 14 语言标准的支持
Java 17 引入了对 C++ 14 语言标准的支持，可以使用 C++ 14 中的新特性和语法，如下所示
```java
// C++ 14 中的泛型 Lambda 表达式
auto multiply = [](auto x, auto y) { return x * y; };
int result = multiply(5, 6);
System.out.println(result); // 输出 30
```
<a name="I65qc"></a>
### 7、Unicode 13 的支持
Java 17 支持 Unicode 13，包括新增的字符、块和属性。以下是一个简单的示例
```java
// 支持 Unicode 13 中的新增字符
String emoji = "🙂";
System.out.println(emoji.length()); // 输出 2
```
<a name="lfKPP"></a>
### 8、针对嵌入式系统的改进
Java 17 改进了对嵌入式系统的支持，包括增强的垃圾收集器、更小的内存占用和更快的启动时间。以下是一个简单的示例
```java
// 使用 Embedded JRE 启动应用程序
Path path = Path.of("myapp.jar");
Path jrePath = Path.of("jre");
ProcessBuilder pb = new ProcessBuilder(
        jrePath.resolve("bin").resolve("java").toString(),
        "--module-path", jrePath.resolve("mods").toString(),
        "--module", "com.example.myapp",
        "--add-modules", "ALL-MODULE-PATH",
        "-jar", path.toString());
Process process = pb.start();
```
<a name="Nw2qi"></a>
### 9、生成的代码的性能改进
Java 17 引入了一些新特性，可以优化生成的代码的性能，特别是在反射和 Lambda 表达式等方面。以下是一个简单的示例
```java
// 反射
Class<?> clazz = MyClass.class;
Method method = clazz.getMethod("myMethod");
Object obj = clazz.getDeclaredConstructor().newInstance();
method.invoke(obj);

// Lambda 表达式
List<String> list = Arrays.asList("Java", "17", "新特性");
list.stream().filter(s ->
```
<a name="VKDx8"></a>
### 10、record 关键字
它提供了一种简洁的方式来定义不可变的数据类。记录类是一种类似于值类型的数据类型，通常用于表示数据传输对象、API 返回类型、函数参数等等。
```java
public record Person(String name, int age) {}
```
这个记录类定义了一个名为 Person 的不可变数据类，包含两个属性：name 和 age。记录类自动生成了一个构造函数，以及对应属性的访问器和 `equals()`/`hashCode()`/`toString()` 方法。可以看到，相较于传统的类定义，记录类更加简洁。<br />使用记录类可以方便地创建不可变的数据对象，如下所示：
```java
Person person = new Person("Tom", 30);
System.out.println(person.name()); // 输出 "Tom"
System.out.println(person.age()); // 输出 30
```
