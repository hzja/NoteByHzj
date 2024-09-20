9 月 19 日，Oracle 宣布 Java 21 正式发布。自从 Java 改为 6 个月发布一个版本以来，此为如期交付的第 12 个版本。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1696942938885-f4bdc2f9-36e6-4ce8-b772-1dafda0f9516.png#averageHue=%23525a5b&clientId=u01cec5f5-5800-4&from=paste&id=u9cb07045&originHeight=507&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u081db2c1-1f34-4317-9f6e-db8c55d1c26&title=)<br />根据规划，Java 21 是长期支持版本，Oracle 将至少提供 8 年支持。根据客户反馈和 Java 生态系统使用情况，Oracle 还宣布将 Java 11 的支持至少延长至 2032 年 1 月，也就是至少再提供 8 年的支持与更新。<br />从 Java 11 到 Java 21 的正式发布，一共修复了 24,196 个 JIRA 问题，其中 17,288 个是由 Oracle 的工作人员完成的，而 6,908 个是由个人开发人员和其他组织的开发人员贡献的。<br />从下图可以看到，中国的腾讯、阿里巴巴、华为和龙芯做出了不少贡献。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1696942938939-073b7a8a-dfe4-4e4b-a7df-1c3470f178ab.png#averageHue=%23d55c54&clientId=u01cec5f5-5800-4&from=paste&id=u869b82af&originHeight=599&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u896cfb77-c6c8-4aea-a20e-367f7e122a7&title=)
<a name="s5HAs"></a>
## 盘点 Java 21 的新特性
除了大量的性能、稳定性和安全性方面的更新，Java 21 还包括 15 个 JEP（JDK Enhancement Proposal），其中包括 6 个预览特性和 1 个孵化特性。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1696942938933-c48cc3ae-ee91-40f4-99bb-bb4a60e18e37.png#averageHue=%23f0f0f0&clientId=u01cec5f5-5800-4&from=paste&id=u346120bf&originHeight=640&originWidth=872&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7183738f-f778-4033-b862-edbe5ca2bac&title=)<br />这些 JEP 可以分为六大类。下面具体来看看一下 Java 21 引入的部分新特性。
<a name="ayQWl"></a>
### （一）Project Amber
<a name="mHlzc"></a>
#### JEP 430：字符串模板（预览特性）
该 JEP 的目标是：

- 对于需要在运行时求值的字符串，简化其表达方式；
- 对于既包含文本（不管是单行文本还是多行文本）又包含表达式的复杂表达式，提高其可读性；
- 有的 Java 程序需要根据用户提供的值来构建字符串并传递给其他系统，字符串模板支持对模板和其中嵌入的表达式的值进行验证和转换，从而提高这类程序的安全性；
- 允许 Java 库定义字符串模板中使用的格式化语法，保持灵活性；
- 对于接受 Java 之外的语言（如 SQL、XML、JSON）编写的字符串的 API，使之用起来更加方便；
- 支持从文字文本和被嵌入的表达式计算非字符串值，而无需通过中间字符串表示进行转换。
```java
String name = "Joan";
String info = STR."My name is \{name}";  // 模板表达式
assert info.equals("My name is Joan");   // 返回true
```
这里的 STR 是模板处理器，它是 Java 引入的一个 `public static final` 的字段，会被自动引入到每个 Java 源代码文件中。<br />因为这是个实验特性，所以编译和运行的时候要添加相关的参数。例如，假设代码保存在 Test.java 文件中，则编译运行方式为：
```bash
javac --release 21 --enable-preview Test.java
java --enable-preview Test
```
该特性可以提高 Java 程序的可读性和可维护性，编写起来也更方便。
<a name="KBARZ"></a>
#### JEP 440：record 模式
该特性将模式匹配扩展到可以解构 record 类的实例，从而实现更复杂的数据查询。添加嵌套模式，使数据查询可以更方便地组合起来。<br />在 Java 16 中，要提取一个 record 实例的数据，需要这样操作：
```java
record Point(int x, int y) {}

static void printSum(Object obj) {
    if (obj instanceof Point p) {
        int x = p.x();
        int y = p.y();
        System.out.println(x+y);
    }
}
```
而在 Java 21 中，可以直接这样实现：
```java
static void printSum(Object obj) {
    if (obj instanceof Point(int x, int y)) {
        System.out.println(x+y);
    }
}
```
<a name="uCGZt"></a>
#### JEP 441：用于 switch 的模式匹配
该特性为 switch 表达式和语句引入了模式匹配。在 switch 中支持对表达式进行多个模式的测试，每个模式都有特定的操作，从而可以简洁而安全地表示复杂的面向数据的查询。<br />可能希望使用模式来针对同一变量测试多种可能性，并在每种情况下执行特定的操作。例如：
```java
static String formatter(Object obj) {
    String formatted = "unknown";
    if (obj instanceof Integer i) {
        formatted = String.format("int %d", i);
    } else if (obj instanceof Long l) {
        formatted = String.format("long %d", l);
    } else if (obj instanceof Double d) {
        formatted = String.format("double %f", d);
    } else if (obj instanceof String s) {
        formatted = String.format("String %s", s);
    }
    return formatted;
}
```
而在 Java 21 中，可以这样实现：
```java
static String formatterPatternSwitch(Object obj) {
    return switch (obj) {
        case Integer i -> String.format("int %d", i);
        case Long l    -> String.format("long %d", l);
        case Double d  -> String.format("double %f", d);
        case String s  -> String.format("String %s", s);
        default        -> obj.toString();
    };
}
```
在 Java 21 之前，如果 switch 后面的表达式的值为 null，则会抛出 NullPointerException。<br />为安全起见，会在 switch 语句之外执行检测。
```java
static void testFooBarOld(String s) {
    if (s == null) {
        System.out.println("Oops!");
        return;
    }
    switch (s) {
        case "Foo", "Bar" -> System.out.println("Great");
        default           -> System.out.println("Ok");
    }
}
```
现在Java21可以这样处理：
```java
static void testFooBarNew(String s) {
    switch (s) {
        case null         -> System.out.println("Oops");
        case "Foo", "Bar" -> System.out.println("Great");
        default           -> System.out.println("Ok");
    }
}
```
<a name="wInD5"></a>
#### JEP 443：无名模式和无名变量（预览特性）
无名模式可以在不指定 record 组件的名称或类型的情况下匹配组件，无名变量可以用于初始化，但不能被使用。它们都用下划线字符表示。<br />例如：
```java
int acc = 0;
for (Order _ : orders) {
    if (acc < LIMIT) { 
        ... acc++ ...
    }
}
```
再看如下例子：
```java
String s = ...
try { 
    int i = Integer.parseInt(s);
    ... i ...
} catch (NumberFormatException _) { 
    System.out.println("Bad number: " + s);
}
```
<a name="oU0Xa"></a>
#### JEP 445：无名的类和实例 main 方法（预览特性）
方便学生学习，无需理解很多为编写大型程序而设计的语言特性即可上手。<br />之前，要写个最简单的程序也要写这么多代码：
```java
public class HelloWorld { 
    public static void main(String[] args) { 
        System.out.println("Hello, World!");
    }
}
```
现在可以简化为这样，似乎在向Python，PHP等脚本语言看齐：
```java
class HelloWorld { 
    void main() { 
        System.out.println("Hello, World!");
    }
}
```
Java 甚至还引入了无名的类，将类的声明隐藏掉：
```java
void main() {
    System.out.println("Hello, World!");
}
```
<a name="E6IWs"></a>
### （二）Project Loom
<a name="bCUY5"></a>
#### JEP 444：虚拟线程
这是很重要的一个特性，将虚拟线程引入到了Java平台中。虚拟线程是轻量级线程，大大降低了编写、维护和观察高吞吐量并发应用程序的工作量。<br />该特性的目标是：使采用简单的每个请求一个线程的方式编写的服务器应用程序能够充分利用硬件资源进行扩展；<br />使现有的使用 java.lang.Thread API 的代码能够以最小的修改支持虚拟线程；<br />使现有的 JDK 工具能够轻松进行虚拟线程的故障排除、调试和性能分析。<br />虚拟线程是 java.lang.Thread 类的一个实例，它在底层的操作系统线程上运行 Java 代码，但并不会在整个生命周期中占用这个操作系统线程。这意味着许多虚拟线程可以在同一个操作系统线程上运行其 Java 代码，有效地共享该线程。平台线程会独占一个宝贵的操作系统线程，虚拟线程则不会。虚拟线程的数量可以远远大于操作系统线程的数量。虚拟线程是由 JDK 提供的轻量级线程实现，而不是由操作系统提供的。<br />例如，为每个请求创建一个新的虚拟线程来进行处理的代码：
```java
void handle(Request request, Response response) {
    var url1 = ...
    var url2 = ...
    try (var executor = Executors.newVirtualThreadPerTaskExecutor()) {
        var future1 = executor.submit(() -> fetchURL(url1));
        var future2 = executor.submit(() -> fetchURL(url2));
        response.send(future1.get() + future2.get());
    } catch (ExecutionException | InterruptedException e) {
        response.fail(e);
    }
}
String fetchURL(URL url) throws IOException {
    try (var in = url.openStream()) {
        return new String(in.readAllBytes(), StandardCharsets.UTF_8);
    }
}
```
<a name="HWENj"></a>
#### JEP 446：作用域值（预览特性）
引入了作用域值，这些值可以安全高效地共享给方法，而不需要使用方法参数。特别是在使用大量虚拟线程时，它们优于线程局部变量。它是 `ScopedValue<T>` 类型的变量。<br />例如：
```java
final static ScopedValue<...> V = ScopedValue.newInstance();
// In some method
ScopedValue.where(V, <value>)
           .run(() -> { ... V.get() ... call methods ... });
// In a method called directly or indirectly from the lambda expression
... V.get() ...
```
<a name="iKrYI"></a>
#### JEP 453：结构化并发（预览特性）
引入结构化并发 API，简化并发编程。结构化并发将在不同线程中运行的相关任务组视为一个工作单元，从而简化错误处理和取消操作，进而可靠性，并增强可观测性。结构化并发 API 的主要类是位于 java.util.concurrent 包中的 `StructuredTaskScope`。
<a name="RZisH"></a>
### （三）Project Panama
<a name="Um5kq"></a>
#### JEP 442：外部的函数和内存 API（第 3 次预览）
Foreign Function & Memory API (FFM API) 位于 java.lang.foreign 包中，它定义了一系列类和接口，使Java程序能够与Java运行时之外的代码和数据进行互操作。通过有效地调用外部函数（即JVM之外的代码），并安全地访问外部内存（即并非由JVM管理的内存），该 API 使 Java 程序能够调用本地库并处理本地数据，而不会出现 JNI 的脆弱性和危险性。
<a name="L4mwV"></a>
#### JEP 448：向量 API（第6次 孵化特性）
引入`VectorSpecies<E>`，用于表达向量计算，这些计算可以在受支持的 CPU 架构上在运行时可靠地编译为最佳的向量指令，从而实现优于等效标量计算的性能。
<a name="Zlf0T"></a>
### （四）核心类库
<a name="acrk6"></a>
#### JEP 431：有顺序的集合
引入新的接口 `SequencedCollection<E>` 来表示具有明确定义的遍历顺序的集合。每个这样的集合都有明确定义的第一个元素、第二个元素等，一直到最后一个元素。它还提供了统一的 API 来访问它的第一个和最后一个元素，以及以相反的顺序处理其元素。
<a name="q7KfM"></a>
### （五）性能更新
<a name="jbciE"></a>
#### JEP 439：分代 ZGC
扩展 ZGC，使之在不同的代中维护年轻的对象和年老的对象，从而提高应用程序的性能。这将允许 ZGC 更频繁地收集年轻对象，因为它们更容易早些时候被回收。
<a name="JLRXy"></a>
#### JEP 452：密钥封装机制 API
引入了一个新类 KEM，用于密钥封装机制，这是一种使用公钥加密来保护对称密钥的加密技术。
<a name="XDW57"></a>
### （六）维护与弃用
<a name="tkYin"></a>
#### JEP 449：弃用 32 位 x86 平台构建版本，将来移除
弃用 Windows 32 位 x86 构建版本，并计划在未来的版本中移除。
<a name="ID46y"></a>
#### JEP 451：准备禁止动态加载代理
在将代理动态加载到运行中的 JVM 时发出警告。这些警告旨在让用户有所准备，以便在将来的发布版本中，默认禁止动态加载代理。在任何发布版本中，通过服务工具（serviceability tool）在启动时加载代理不会引发警告。<br />Java 21 的新特性就介绍到这里，赶快下载体验吧！<br />JDK下载地址：[https://www.oracle.com/java/technologies/downloads/](https://www.oracle.com/java/technologies/downloads/)
