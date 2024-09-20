Java<br />Java11 于 2018 年 9 月 25 日正式发布，这是很重要的一个版本！Java 11 和 2017 年 9 月份发布的 Java 9 以及 2018 年 3 月份发布的 Java 10 相比，其最大的区别就是：在长期支持(Long-Term-Support)方面，Oracle 表示会对 Java 11 提供大力支持，这一支持将会持续至 2026 年 9 月。这是据 Java 8 以后支持的首个长期版本。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1630928540951-108f11e4-0909-456f-8262-b12cceeff4d3.webp#averageHue=%23dfe5df&clientId=u48b1ea96-6a31-4&from=paste&id=ubd195ac4&originHeight=376&originWidth=1013&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ueb6fb8ad-b795-4e26-8b2d-61556562de1&title=)
<a name="UyLXT"></a>
## Java11新特性预览
jdk1.11 正式版将在2018年9月份发布, jdk1.11变化内容：<br />1:Nest-Based访问控制<br />2:动态类文件常量<br />3:改善Aarch64 intrinsic<br />4:无操作垃圾收集器<br />5:消除Java EE和CORBA模块<br />6:HTTP客户端(标准)<br />7:局部变量的语法λ参数<br />8:Curve25519和Curve448关键协议<br />9:Unicode 10<br />10:飞行记录器<br />11:ChaCha20和Poly1305加密算法<br />12:发射一列纵队源代码程序<br />13:低开销堆分析<br />14:传输层安全性(Transport Layer Security,TLS)1.3<br />15:动作:一个可伸缩的低延迟垃圾收集器 (实验)<br />16:反对Nashorn JavaScript引擎<br />17:反对Pack200工具和API

---

- 可以直接使用 `java` 命令运行 Java 程序，源代码将会隐式编译和运行。
- String 类新增了一些方法，比如说 `isBlank()`、`lines()`、`strip()` 等等。
- Files 类新增了两个读写方法，`readString()` 和 `writeString()`。
- 可以在 Lambda 表达式中使用 var 作为变量类型。

![](https://cdn.nlark.com/yuque/0/2020/png/396745/1602515499844-98350f52-a5a1-4865-8457-f2f7ca5a7142.png#averageHue=%23f7f6f5&height=265&id=KeSB4&originHeight=265&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=1080)
<a name="mnE3h"></a>
### 1、字符串操作
String类是Java最常用的类，JDK 11增加了一系列好用的字符串处理方法

- `isBlank()` 判空。
- `strip()` 去除首尾空格，去除字符串前后的“全角和半角”空白字符
- `stripLeading()` 去除字符串首部空格，去除前面的全角半角空白符
- `stripTrailing()` 去除字符串尾部空格，去除尾部的全角半角空白符
- `lines()` 分割获取字符串流，将一个字符串按照行终止符（换行符`\n`或者回车符`\r`）进行分割，并将分割为Stream流。
- `repeat()` 复制字符串，按照给定的次数重复串联字符串的内容
```java
// 判断字符串是否为空白
"  ".isBlank();    // true
// 去除首尾空格
" Fcant ".strip();  // "Fcant"

String str = "HELLO\u3000";
System.out.println("str = " + str.length());
// str = 6
System.out.println("trim = " + str.trim().length());
// trim = 6
System.out.println("strip = " + str.strip().length());
// strip = 5

// 从上面也看出来了差别，trim()只能去除半角空白符。

// 去除首部空格 
" Fcant ".stripLeading();   // "Fcant "
去除字符串尾部空格
" Fcant ".stripLeading();   // " Fcant"
// 行数统计
"a\nb\nc".lines().count();    // 3
// 复制字符串
"Fcant".repeat(3);   // "FcantFcantFcant"

String str = "HELLO";
String empty = str.repeat(0);
// 空字符
String repeatOne = str.repeat(1);
// HELLO
String repeatTwo = str.repeat(2);
// HELLOHELLO

//返回由行终止符分隔的字符串集合。
"A\nB\nC".lines().count();    // 3
"A\nB\nC".lines().collect(Collectors.toList());
```
<a name="F7Qvq"></a>
### 2、集合转对应类型的数组
之前想集合转对应的数组很麻烦，要么用迭代；要么用`Stream`流，现在可以这样：
```java
List<String> sampleList = Arrays.asList("fcant.cn", "java 11");
// array = {"fcant.cn", "java 11"};
String[] array = sampleList.toArray(String[]::new);
```
<a name="N7pCv"></a>
### 3、断言取反
`java.util.function.Predicate<T>`是很常用的断言谓词函数。在以前取反得借助于`!`符号，到了Java 11可以借助于其静态方法`not`来实现，这样语义就更加清晰了：
```java
List<String> sampleList = Arrays.asList("fcant.cn", "java 11","Fcant");
// [Fcant]
List<String> result = sampleList.stream()
    // 过滤以j开头的字符串
    .filter(s -> s.startsWith("j"))
    // 同时不包含11的字符串
    .filter(Predicate.not(s -> s.contains("11")))
    .collect(Collectors.toList());
```
其实`Predicate<T>`在最初版本还提供了一个取反的默认方法：
```java
default Predicate<T> negate() {
    return (t) -> !test(t);
}
```
<a name="x97sG"></a>
### 4、用于 Lambda 参数的局部变量语法
局部变量类型推断是Java 10引入的新特性，但是不能在Lambda 表达式中使用。Java 11再次创新，它允许开发者在 Lambda 表达式中使用 var 进行参数声明。<br />从 Java 10 开始，便引入了局部变量类型推断这一关键特性。类型推断允许使用关键字 var 作为局部变量的类型而不是实际类型，编译器根据分配给变量的值推断出类型。这一改进简化了代码编写、节省了开发者的工作时间，因为不再需要显式声明局部变量的类型，而是可以使用关键字 var，且不会使源代码过于复杂。<br />可以使用关键字 var 声明局部变量，如下所示：
```java
var s = "Hello Java 11";
System.out.println(s);
```
Java 10 中对 var 关键字存在几个限制

- 只能用于局部变量上
- 声明时必须初始化
- 不能用作方法参数
- 不能在 Lambda 表达式中使用

Java 11 与 Java 10 的不同之处在于允许开发者在 Lambda 表达式中使用 var 进行参数声明。乍一看，这一举措似乎有点多余，因为在写代码过程中可以省略 Lambda 参数的类型，并通过类型推断确定它们。但是，添加上类型定义同时使用 `@Nonnull` 和 `@Nullable` 等类型注释还是很有用的，既能保持与局部变量的一致写法，也不丢失代码简洁。<br />Lambda 表达式使用隐式类型定义，它形参的所有类型全部靠推断出来的。隐式类型 Lambda 表达式如下：
```java
(x, y) -> x.process(y)
```
Java 10 为局部变量提供隐式定义写法如下：
```java
var x = new Foo();
for (var x : xs) { ... }
try (var x = ...) { ... } catch ...
```
为了 Lambda 类型表达式中正式参数定义的语法与局部变量定义语法的不一致，且为了保持与其他局部变量用法上的一致性，希望能够使用关键字 `var` 隐式定义 Lambda 表达式的形参：
```java
(var x, var y) -> x.process(y)
```
于是在 Java 11 中将局部变量和 Lambda 表达式的用法进行了统一，并且可以将注释应用于局部变量和 Lambda 表达式：
```java
@Nonnull var x = new Foo();
(@Nonnull var x, @Nullable var y) -> x.process(y)
```
Java11 开始允许开发者在 `Lambda` 表达式中使用 `var` 进行参数声明。其实对于一个Lambda表达式来说它入参的类型其实是可以根据上下文推断出来的。拿上面的例子来说，`s -> s.startsWith("j")`中的`s`肯定是字符串类型，因此在Java 11中`var`可以用于修饰Lambda局部变量：
```java
// 下面两者是等价的
Consumer<String> consumer = (var i) -> System.out.println(i);
Consumer<String> consumer = (String i) -> System.out.println(i);
```
Java 11 与 Java 10 的不同之处在于允许开发者在 Lambda 表达式中使用 var 进行参数声明。这一举措似乎有点多余，因为在写代码过程中可以省略 Lambda 参数的类型，并通过类型推断确定它们。但是，添加上类型定义同时使用 `@Nonnull` 和 `@Nullable` 等类型注释还是很有用的，既能保持与局部变量的一致写法，也不丢失代码简洁。
```python
List<String> sampleList = Arrays.asList("fcant.cn", "java 11","Fcant");
// [Fcant]
List<String> result = sampleList.stream()
                // 过滤以j开头的字符串
                .filter(s -> s.startsWith("j"))
                // 同时不包含11的字符串
                .filter(Predicate.not(s -> s.contains("11")))
                .collect(Collectors.toList());
List<String> result = sampleList.stream()
                // 过滤以j开头的字符串
                .filter((@NotNull var s) -> s.startsWith("j"))
                // 同时不包含11的字符串
                .filter(Predicate.not((@NotNull var s) -> s.contains("11")))
                .collect(Collectors.toList());
```
```java
@Nonnull var x = new Foo();
(@Nonnull var x, @Nullable var y) -> x.process(y)
```
:::tips
如果不声明`var`就没有办法为输入参数添加`@NotNull`注解。
:::
```java
List<String> languages = Arrays.asList("Java", "Groovy");
String language = sampleList.stream()
  .map((@Nonnull var x) -> x.toUpperCase())
  .collect(Collectors.joining(", "));

assertThat(language).isEqualTo("Java, Groovy");
```
<a name="xBPeB"></a>
### 5、嵌套类的访问控制规则
与 Java 语言中现有的嵌套类型概念一致, 嵌套访问控制是一种控制上下文访问的策略，允许逻辑上属于同一代码实体，但被编译之后分为多个分散的 class 文件的类，无需编译器额外的创建可扩展的桥接访问方法，即可访问彼此的私有成员，并且这种改进是在 Java 字节码级别的。<br />在 Java 11 之前的版本中，编译之后的 class 文件中通过 InnerClasses 和 Enclosing Method 两种属性来帮助编译器确认源码的嵌套关系，每一个嵌套的类会编译到自己所在的 class 文件中，不同类的文件通过上面介绍的两种属性的来相互连接。这两种属性对于编译器确定相互之间的嵌套关系已经足够了，但是并不适用于访问控制。这里大家可以写一段包含内部类的代码，并将其编译成 class 文件，然后通过 javap 命令行来分析。<br />在Java 11之前，内部嵌套类访问外部类的私有属性和方法是可行的：
```java
public class Outer {
    private int outerInt;

     class Inner {
       public void printOuterField() {
         System.out.println("Outer field = " + outerInt);
       }
    }
}
```
但是如果通过反射API实现内部类访问外部类的私有属性和方法就会抛出`IllegalStateException`异常。Java 11 修复了反射不能访问的问题。<br />Java 11 中引入了两个新的属性：一个叫做 NestMembers 的属性，用于标识其它已知的静态 nest 成员；另外一个是每个 nest 成员都包含的 NestHost 属性，用于标识出它的 nest 宿主类。
:::tips
JVM 访问规则不允许嵌套类之间进行私有访问。能通过常规方式可以访问是因为 JVM 在编译时隐式地创建了桥接方法。Java 11 中引入了两个新的属性：一个叫做 `NestMembers` 的属性，用于标识其它已知的静态 nest 成员；另外一个是每个 nest 成员都包含的 `NestHost` 属性，用于标识出它的 nest 宿主类。在编译期就映射了双方的寄宿关系，不再需要桥接了。
:::
<a name="LPTCo"></a>
### 6、文件中读写字符串内容更方便
Java 11中可以更轻松地从文件中读取和写入字符串内容了，可以通过`Files`工具类提供的新的静态方法`readString`和`writeString`分别进行读写文件的字符串内容，放在之前老麻烦了，特别是对IO流不熟悉的同学来说。现在简单几行就搞定了：
```java
String dir= "C://yourDir";
// 写入文件
Path path = Files.writeString(Files.createTempFile(dir, "hello", ".txt"), "hello java 11");
// 读取文件
String fileContent = Files.readString(path);
```
<a name="Mz0LI"></a>
### 7、标准化HTTP Client
Java 9 引入Http Client API，Java 10对它更新，Java 11 对它进行标准化。这几个版本后，Http Client几乎被完全重写，支持HTTP/1.1和HTTP/2 ，也支持 websockets。<br />HttpClient到了Java 11后开始支持HTTP2，底层进行了大幅度的优化，并且现在完全支持异步非阻塞。<br />并且，Java11 中，Http Client 的包名由 jdk.incubator.http 改为java.net.http，该 API 通过 `CompleteableFuture` 提供非阻塞请求和响应语义。可以联合使用以触发相应的动作，并且 RX Flo w 的概念也在 Java 11 中得到了实现。现在，在用户层请求发布者和响应发布者与底层套接字之间追踪数据流更容易了。这降低了复杂性，并最大程度上提高了 HTTP/1 和 HTTP/2 之间的重用的可能性。使用起来也很简单，如下：
```java
HttpClient client = HttpClient.newHttpClient();
HttpRequest request = HttpRequest.newBuilder()
            .uri(URI.create("https://github.com/fcscanf"))
            .GET()
            .build();
// 同步
HttpResponse<String> response = client.send(request, HttpResponse.BodyHandlers.ofString());
System.out.println(response.body());
// 异步
client.sendAsync(request, HttpResponse.BodyHandlers.ofString())
    .thenApply(HttpResponse::body)
    .thenAccept(System.out::println);
```
Java 11 中的新 Http Client API，提供了对 HTTP/2 等业界前沿标准的支持，同时也向下兼容 HTTP/1.1，精简而又友好的 API 接口，与主流开源 API（如：Apache HttpClient、Jetty、OkHttp 等）类似甚至拥有更高的性能。与此同时它是 Java 在 Reactive-Stream 方面的第一个生产实践，其中广泛使用了 Java Flow API，终于让 Java 标准 HTTP 类库在扩展能力等方面，满足了现代互联网的需求，是一个难得的现代 Http/2 Client API 标准的实现，Java 工程师终于可以摆脱老旧的 `HttpURLConnection` 了。下面模拟 Http GET 请求并打印返回内容：<br />GET 请求示例
```java
HttpClient client = HttpClient.newHttpClient();
HttpRequest request = HttpRequest.newBuilder()
    .uri(URI.create("http://openjdk.java.net/"))
    .build();
client.sendAsync(request, BodyHandlers.ofString())
    .thenApply(HttpResponse::body)
    .thenAccept(System.out::println)
    .join();
```
<a name="31b89872"></a>
### 8、单个命令编译运行源代码
Java 11增强了Java 启动器，使之能够运行单一文件的Java 源代码。<br />JEP 330:启动单文件源代码程序（aunch Single-File Source-Code Programs） 可以运行单一文件的 Java 源代码。此功能允许使用 Java 解释器直接执行 Java 源代码。源代码在内存中编译，然后由解释器执行，不需要在磁盘上生成 .class 文件了。<br />唯一的约束在于所有相关的类必须定义在同一个 Java 文件中。<br />对于 Java 初学者并希望尝试简单程序的人特别有用，并且能和 jshell 一起使用<br />一定能程度上增强了使用 Java 来写脚本程序的能力。

- Java 11之前,要运行一个 Java 源代码必须先编译，再运行
```java
// 编译
javac Jay.java
// 运行
java Jay
```

- Java 11之后,只要一个java命令就搞定
```java
java Jay.java
```
<a name="ElVCm"></a>
### 9、Optional
新增了`empty()`方法来判断指定的 `Optional` 对象是否为空。
```java
var op = Optional.empty();
System.out.println(op.isEmpty());//判断指定的 Optional 对象是否为空
```
<a name="tF4Wc"></a>
### 10、ZGC：可伸缩低延迟垃圾收集器
ZGC ，即 Z Garbage Collector（垃圾收集器或垃圾回收器）。它是一个可伸缩的、低延迟的垃圾收集器。 ZGC 主要为了满足如下目标进行设计：

- GC 停顿时间不超过 10ms
- 既能处理几百 MB 的小堆，也能处理几个 TB 的大堆
- 应用吞吐能力不会下降超过 15%（与 G1 回收算法相比）
- 方便在此基础上引入新的 GC 特性和利用 colord
- 针以及 Load barriers 优化奠定基础
- 当前只支持 Linux/x64 位平台停顿时间在 10ms 以下，10ms 其实是一个很保守的数据，即便是 10ms 这个数据，也是 GC 调优几乎达不到的极值。根据 SPECjbb 2015 的基准测试，128G 的大堆下最大停顿时间才 1.68ms，远低于 10ms，和 G1 算法相比，改进非常明显。

不过目前 ZGC 还处于实验阶段，目前只在 Linux/x64 上可用，如果有足够的需求，将来可能会增加对其他平台的支持。同时作为实验性功能的 ZGC 将不会出现在 JDK 构建中，除非在编译时使用 configure 参数：`--with-jvm-features=zgc` 显式启用。<br />在实验阶段，编译完成之后，已经迫不及待的想试试 ZGC，需要配置以下 JVM 参数，才能使用 ZGC，具体启动 ZGC 参数如下：
```bash
-XX：+ UnlockExperimentalVMOptions -XX：+ UseZGC -Xmx10g
```
其中参数：-Xmx 是 ZGC 收集器中最重要的调优选项，大大解决了程序员在 JVM 参数调优上的困扰。ZGC 是一个并发收集器，必须要设置一个最大堆的大小，应用需要多大的堆，主要有下面几个考量：

- 对象的分配速率，要保证在 GC 的时候，堆中有足够的内存分配新对象。
- 一般来说，给 ZGC 的内存越多越好，但是也不能浪费内存，所以要找到一个平衡。

与 CMS 中的 ParNew 和 G1 类似，ZGC 也采用标记-复制算法，不过 ZGC 对该算法做了重大改进。<br />在 ZGC 中出现 Stop The World 的情况会更少！
<a name="rNAM0"></a>
### 11、Epsilon：低开销垃圾回收器
Epsilon 垃圾回收器的目标是开发一个控制内存分配，但是不执行任何实际的垃圾回收工作。它提供一个完全消极的 GC 实现，分配有限的内存资源，最大限度的降低内存占用和内存吞吐延迟时间。<br />Java 版本中已经包含了一系列的高度可配置化的 GC 实现。各种不同的垃圾回收器可以面对各种情况。但是有些时候使用一种独特的实现，而不是将其堆积在其他 GC 实现上将会是事情变得更加简单。<br />下面是 no-op GC 的几个使用场景：

- 性能测试：什么都不执行的 GC 非常适合用于 GC 的差异性分析。no-op （无操作）GC 可以用于过滤掉 GC 诱发的性能损耗，比如 GC 线程的调度，GC 屏障的消耗，GC 周期的不合适触发，内存位置变化等。此外有些延迟者不是由于 GC 引起的，比如 scheduling hiccups, compiler transition hiccups，所以去除 GC 引发的延迟有助于统计这些延迟。
- 内存压力测试：在测试 Java 代码时，确定分配内存的阈值有助于设置内存压力常量值。这时 no-op 就很有用，它可以简单地接受一个分配的内存分配上限，当内存超限时就失败。例如：测试需要分配小于 1G 的内存，就使用-Xmx1g 参数来配置 no-op GC，然后当内存耗尽的时候就直接 crash。
- VM 接口测试：以 VM 开发视角，有一个简单的 GC 实现，有助于理解 VM-GC 的最小接口实现。它也用于证明 VM-GC 接口的健全性。
- 极度短暂 job 任务：一个短声明周期的 job 任务可能会依赖快速退出来释放资源，这个时候接收 GC 周期来清理 heap 其实是在浪费时间，因为 heap 会在退出时清理。并且 GC 周期可能会占用一会时间，因为它依赖 heap 上的数据量。延迟改进：对那些极端延迟敏感的应用，开发者十分清楚内存占用，或者是几乎没有垃圾回收的应用，此时耗时较长的 GC 周期将会是一件坏事。
- 吞吐改进：即便对那些无需内存分配的工作，选择一个 GC 意味着选择了一系列的 GC 屏障，所有的 OpenJDK GC 都是分代的，所以他们至少会有一个写屏障。避免这些屏障可以带来一点点的吞吐量提升。

Epsilon 垃圾回收器和其他 OpenJDK 的垃圾回收器一样，可以通过参数 `-XX:+UseEpsilonGC` 开启。<br />Epsilon 线性分配单个连续内存块。可复用现存 VM 代码中的 TLAB 部分的分配功能。非 TLAB 分配也是同一段代码，因为在此方案中，分配 TLAB 和分配大对象只有一点点的不同。Epsilon 用到的 barrier 是空的(或者说是无操作的)。因为该 GC<br />执行任何的 GC 周期，不用关系对象图，对象标记，对象复制等。引进一种新的 barrier-set 实现可能是该 GC 对 JVM 最大的变化。
<a name="sD5PP"></a>
### 12、简化启动单个源代码文件的方法
Java 11 版本中最令人兴奋的功能之一是增强 Java 启动器，使之能够运行单一文件的 Java 源代码。此功能允许使用 Java 解释器直接执行 Java 源代码。源代码在内存中编译，然后由解释器执行。唯一的约束在于所有相关的类必须定义在同一个 Java 文件中。<br />此功能对于开始学习 Java 并希望尝试简单程序的人特别有用，并且能与 jshell 一起使用，将成为任何初学者学习语言的一个很好的工具集。不仅初学者会受益，专业人员还可以利用这些工具来探索新的语言更改或尝试未知的 API。<br />如今单文件程序在编写小实用程序时很常见，特别是脚本语言领域。从中开发者可以省去用 Java 编译程序等不必要工作，以及减少新手的入门障碍。在基于 Java 10 的程序实现中可以通过三种方式启动：

- 作为 `*.class` 文件
- 作为 `*.jar` 文件中的主类
- 作为模块中的主类

而在最新的 Java 11 中新增了一个启动方式，即可以在源代码中声明类，例如：如果名为 HelloWorld.java 的文件包含一个名为 hello.World 的类，那么该命令：
```bash
$ java HelloWorld.java
```
也等同于：
```bash
$ javac HelloWorld.java
$ java -cp . hello.World
```
<a name="fbBh6"></a>
### 13、低开销的 Heap Profiling
Java 11 中提供一种低开销的 Java 堆分配采样方法，能够得到堆分配的 Java 对象信息，并且能够通过 JVMTI 访问堆信息。<br />引入这个低开销内存分析工具是为了达到如下目的：

- 足够低的开销，可以默认且一直开启
- 能通过定义好的程序接口访问
- 能够对所有堆分配区域进行采样
- 能给出正在和未被使用的 Java 对象信息

对用户来说，了解它们堆里的内存分布是非常重要的，特别是遇到生产环境中出现的高 CPU、高内存占用率的情况。目前有一些已经开源的工具，允许用户分析应用程序中的堆使用情况，比如：Java Flight Recorder、jmap、YourKit 以及 VisualVM tools.。但是这些工具都有一个明显的不足之处：无法得到对象的分配位置，headp dump 以及 heap histogram 中都没有包含对象分配的具体信息，但是这些信息对于调试内存问题至关重要，因为它能够告诉开发人员他们的代码中发生的高内存分配的确切位置，并根据实际源码来分析具体问题，这也是 Java 11 中引入这种低开销堆分配采样方法的原因。
<a name="uLEVN"></a>
### 14、支持 TLS 1.3 协议
Java 11 中包含了传输层安全性（TLS）1.3 规范（RFC 8446）的实现，替换了之前版本中包含的 TLS，包括 TLS 1.2，同时还改进了其他 TLS 功能，例如 OCSP 装订扩展（RFC 6066，RFC 6961），以及会话散列和扩展主密钥扩展（RFC 7627），在安全性和性能方面也做了很多提升。<br />新版本中包含了 Java 安全套接字扩展（JSSE）提供 SSL，TLS 和 DTLS 协议的框架和 Java 实现。目前，JSSE API 和 JDK 实现支持 SSL 3.0，TLS 1.0，TLS 1.1，TLS 1.2，DTLS 1.0 和 DTLS 1.2。<br />同时 Java 11 版本中实现的 TLS 1.3，重新定义了以下新标准算法名称：

- TLS 协议版本名称：TLSv1.3
- SSLContext 算法名称：TLSv1.3
- TLS 1.3 的 TLS 密码套件名称：TLS_AES_128_GCM_SHA256，TLS_AES_256_GCM_SHA384
- 用于 X509KeyManager 的 keyType：RSASSA-PSS
- 用于 X509TrustManager 的 authType：RSASSA-PSS

还为 TLS 1.3 添加了一个新的安全属性 jdk.tls.keyLimits。当处理了特定算法的指定数据量时，触发握手后，密钥和 IV 更新以导出新密钥。还添加了一个新的系统属性 jdk.tls.server.protocols，用于在 SunJSSE 提供程序的服务器端配置默认启用的协议套件。<br />之前版本中使用的 KRB5 密码套件实现已从 Java 11 中删除，因为该算法已不再安全。同时注意，TLS 1.3 与以前的版本不直接兼容。<br />升级到 TLS 1.3 之前，需要考虑如下几个兼容性问题：

- TLS 1.3 使用半关闭策略，而 TLS 1.2 以及之前版本使用双工关闭策略，对于依赖于双工关闭策略的应用程序，升级到 TLS 1.3 时可能存在兼容性问题。
- TLS 1.3 使用预定义的签名算法进行证书身份验证，但实际场景中应用程序可能会使用不被支持的签名算法。
- TLS 1.3 再支持 DSA 签名算法，如果在服务器端配置为仅使用 DSA 证书，则无法升级到 TLS 1.3。
- TLS 1.3 支持的加密套件与 TLS 1.2 和早期版本不同，若应用程序硬编码了加密算法单元，则在升级的过程中需要修改相应代码才能升级使用 TLS 1.3。
- TLS 1.3 版本的 session 用行为及秘钥更新行为与 1.2 及之前的版本不同，若应用依赖于 TLS 协议的握手过程细节，则需要注意。
<a name="yZlmy"></a>
### 15、飞行记录器
飞行记录器之前是商业版 JDK 的一项分析工具，但在 Java 11 中，其代码被包含到公开代码库中，这样所有人都能使用该功能了。<br />Java 语言中的飞行记录器类似飞机上的黑盒子，是一种低开销的事件信息收集框架，主要用于对应用程序和 JVM 进行故障检查、分析。飞行记录器记录的主要数据源于应用程序、JVM 和 OS，这些事件信息保存在单独的事件记录文件中，故障发生后，能够从事件记录文件中提取出有用信息对故障进行分析。<br />启用飞行记录器参数如下：
```bash
-XX:StartFlightRecording
```
也可以使用 bin/jcmd 工具启动和配置飞行记录器：<br />飞行记录器启动、配置参数示例
```bash
$ jcmd <pid> JFR.start
$ jcmd <pid> JFR.dump filename=recording.jfr
$ jcmd <pid> JFR.stop
```
JFR 使用测试：<br />JFR 使用示例
```java
public class FlightRecorderTest extends Event {
    @Label("Hello World")
    @Description("Helps the programmer getting started")
    static class HelloWorld extends Event {
        @Label("Message")
        String message;
    }

    public static void main(String[] args) {
        HelloWorld event = new HelloWorld();
        event.message = "hello, world!";
        event.commit();
    }
}
```
在运行时加上如下参数：
```bash
java -XX:StartFlightRecording=duration=1s, filename=recording.jfr
```
下面读取上一步中生成的 JFR 文件：recording.jfr<br />飞行记录器分析示例
```java
public void readRecordFile() throws IOException {
    final Path path = Paths.get("D:\\ java \\recording.jfr");
    final List<RecordedEvent> recordedEvents = RecordingFile.readAllEvents(path);
    for (RecordedEvent event : recordedEvents) {
        System.out.println(event.getStartTime() + "," + event.getValue("message"));
    }
}
```
<a name="jOvjZ"></a>
### 16、动态类文件常量
为了使 JVM 对动态语言更具吸引力，Java 的第七个版本已将 invokedynamic 引入其指令集。<br />不过 Java 开发人员通常不会注意到此功能，因为它隐藏在 Java 字节代码中。通过使用 invokedynamic，可以延迟方法调用的绑定，直到第一次调用。例如，Java 语言使用该技术来实现 Lambda 表达式，这些表达式仅在首次使用时才显示出来。这样做，invokedynamic 已经演变成一种必不可少的语言功能。<br />Java 11 引入了类似的机制，扩展了 Java 文件格式，以支持新的常量池：CONSTANT_Dynamic，它在初始化的时候，像 invokedynamic 指令生成代理方法一样，委托给 bootstrap 方法进行初始化创建，对上层软件没有很大的影响，降低开发新形式的可实现类文件约束带来的成本和干扰。
<a name="cvnYY"></a>
### 17、其他一些特性

- 添加 Epsilon 垃圾收集器 ：一个完全消极的 GC 实现，分配有限的内存资源，最大限度的降低内存占用和内存吞吐延迟时间。
- 支持 TLS 1.3 协议：Java 11 中包含了传输层安全性（TLS）1.3 规范（RFC 8446）的实现，替换了之前版本中包含的 TLS，包括 TLS 1.2，同时还改进了其他 TLS 功能，例如 OCSP 装订扩展（RFC 6066，RFC 6961），以及会话散列和扩展主密钥扩展（RFC 7627），在安全性和性能方面也做了很多提升。
- 飞行记录器分析工具(Java Flight Recorder) ：飞行记录器之前是商业版 JDK 的一项分析工具，但在 Java 11 中，其代码被包含到公开代码库中，这样所有人都能使用该功能了。
- 动态类文件常量
- 低开销的 Heap Profiling ：Java 11 中提供一种低开销的 Java 堆分配采样方法，能够得到堆分配的 Java 对象信息，并且能够通过 JVMTI 访问堆信息。
- Java Flight Recorder 登陆 OpenJDK(「Java Flight Recorder」 是个非常好用的调试诊断工具，不过之前是在 Oracle JDK 中，也跟着 JDK 11 开源了)
