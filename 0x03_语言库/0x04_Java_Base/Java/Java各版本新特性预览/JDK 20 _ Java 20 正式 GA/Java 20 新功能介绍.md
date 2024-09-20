Java<br />![Java 版本新特性情况](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1683428006105-798df37f-f9af-434d-bc5e-e31d5bd8b134.jpeg#averageHue=%23204b5a&clientId=uea568365-578f-4&from=paste&id=uaf762a48&originHeight=494&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u3c240228-3373-4ed6-95c8-0449407434e&title=Java%20%E7%89%88%E6%9C%AC%E6%96%B0%E7%89%B9%E6%80%A7%E6%83%85%E5%86%B5 "Java 版本新特性情况")
```bash
➜  bin pwd
/Users/darcy/develop/jdk-20.0.1.jdk/Contents/Home/bin
➜  bin ./java -version
openjdk version "20.0.1" 2023-04-18
OpenJDK Runtime Environment (build 20.0.1+9-29)
OpenJDK 64-Bit Server VM (build 20.0.1+9-29, mixed mode, sharing)
```
**Java 20 共带来 7 个新特性功能**，其中三个是孵化提案，孵化也就是说尚在征求意见阶段，未来可能会删除此功能。

| JEP | 描述 | 分类 |
| --- | --- | --- |
| 429 | [作用域值（孵化器）](https://openjdk.org/jeps/429) | Project Loom，Java 开发相关 |
| 432 | [Record 模式匹配（第二次预览）](https://openjdk.org/jeps/432) | Project Amber，新的语言特性 |
| 433 | [switch 的模式匹配（第四次预览）](https://openjdk.org/jeps/433) | Project Amber，新的语言特性 |
| 434 | [外部函数和内存 API（第二个预览版）](https://openjdk.org/jeps/434) | Project Panama，非 Java 库 |
| 436 | [虚拟线程（第二个预览版）](https://openjdk.org/jeps/436) | Project Loom，Java 开发相关 |
| 437 | [结构化并发（第二孵化器）](https://openjdk.org/jeps/437) | Project Loom，Java 开发相关 |
| 438 | [Vector API（第五孵化器）](https://openjdk.org/jeps/438) | Project Panama，非 Java 库 |

JEP：JDK Enhancement Proposal，JDK 增强建议，或者叫 Java 未来发展建议。<br />**JDK 20 不是长期支持 (LTS) 版本**，因此它只会在六个月后被 JDK 21 取代之前收到更新。JDK 17（ 2021 年 9 月 14 日发布）是 Java 的最新 LTS 版本。Oracle 宣布计划将 LTS 版本之间的时间从三年缩短到两年，因此 JDK 21（2023 年 9 月）计划成为下一个LTS。
<a name="SK9Yu"></a>
## Java 20 安装
Java 20 OpenJDK 下载：[https://jdk.java.net/19/](https://jdk.java.net/19/)<br />Java 20 OpenJDK 文档：[https://openjdk.java.net/projects/jdk/20/](https://openjdk.java.net/projects/jdk/20/)<br />Java 20 OracleJDK 下载：[Oracle JDK 20 Archive Downloads](https://www.oracle.com/java/technologies/javase/jdk20-archive-downloads.html)
```bash
# 此文中示例代码运行都在 Java 20 环境下使用命令
➜  src $ /jdk-20.0.1.jdk/Contents/Home/bin/java --version                                                         
openjdk 20.0.1 2023-04-18
OpenJDK Runtime Environment (build 20.0.1+9-29)
OpenJDK 64-Bit Server VM (build 20.0.1+9-29, mixed mode, sharing)
➜  src $ /jdk-20.0.1.jdk/Contents/Home/bin/java  --add-modules jdk.incubator.concurrent Xxx.java
WARNING: Using incubator modules: jdk.incubator.concurrent
hello wdbyte
没有信息
```
<a name="m7Ewl"></a>
## JEP 429: Scoped Value
在线程之间**共享变量**不是一件简单的事，可以使用 `ThreadLocal` 来保存当前线程变量，但是需要手动清理，开发者常常忘记，且变量不能被子线程继承；而使用 `InheritableThreadLocal` 共享信息可以被子线程继承，但是数据会拷贝多份，占用更多内存。<br />引入Scoped values，**允许在线程内和线程间共享不可变数据**，这比线程局部变量更加方便，尤其是在使用大量虚拟线程时。这提高了易用性、可理解性、健壮性以及性能。不过这是一个[正在孵化的 API](https://openjdk.org/jeps/11)，未来可能会被删除。<br />scoped values 有下面几个目标：

- _易用性_——提供一个编程模型来在线程内和子线程之间共享数据，从而简化数据流的推理。
- _可理解性_——使共享数据的生命周期从代码的句法结构中可见。
- _稳健性_——确保调用者共享的数据只能由合法的被调用者检索。
- _性能_——将共享数据视为不可变的，以便允许大量线程共享，并启用运行时优化。
<a name="HugV6"></a>
### 例子
如果每个请求都是用一个单独的线程来处理，现在需要接受一个请求，然后根据不同身份访问数据库，那么可以用传递参数的方式，直接把身份信息在调用访问数据库方法时传递过去。如果不这么做，那么就要使用 ThreadLocal 来共享变量了。
```
Thread 1                             Thread 2
--------                             --------
8. 数据库 - 开始查询 ()                8. throw new InvalidPrincipalException()
7. 数据库 - 开始访问 () <---+          7. 数据库 - 开始访问 () <---+ 
   ...                    |             ...                   |
   ...                身份(管理员)        ...                  身份(访客)
2. 开始处理(..)            |          2. 开始处理(..)            |
1. 收到请求(..) -----------+          1. 收到请求(..) -----------+
```
示意代码：
```java
class Server {
    final static ThreadLocal<Principal> PRINCIPAL = new ThreadLocal<>();  

    void serve(Request request, Response response) {
        var level     = (request.isAuthorized() ? ADMIN : GUEST);
        var principal = new Principal(level);
        PRINCIPAL.set(principal);                                         
        Application.handle(request, response);
    }
}

class DBAccess {
    DBConnection open() {
        var principal = Server.PRINCIPAL.get();                           
        if (!principal.canOpen()) throw new InvalidPrincipalException();
        return newConnection(...);                                        
    }
}
```
这是常见的写法，但是使用 ThreadLocal 的问题是：

- `PRINCIPAL.set(principal)` 可以被任意设置修改。
- 使用 ThreadLocal 可能会忘记 `remove`。
- 如果想要子线程继承共享的变量，**需要占用新的内存空间**。
- 在虚拟线程场景下，可能会有几十万线程，使用 ThreadLocal 过于复杂，且有安全性能隐患。

虚拟线程自 Java 19 引入。
<a name="vQMEM"></a>
### 使用 ScopedValue
```java
import jdk.incubator.concurrent.ScopedValue;

/**
 * 启动命令加上 --add-modules jdk.incubator.concurrent
 */
public class Jep429ScopedValueTest {
    final static ScopedValue<String> SCOPED_VALUE = ScopedValue.newInstance();

    public static void main(String[] args) {
        // 创建线程
        Thread thread1 = new Thread(Jep429ScopedValueTest::handle);
        Thread thread2 = new Thread(Jep429ScopedValueTest::handle);
        String str = "hello wdbyte";
        // 传入线程里使用的字符串信息
        ScopedValue.where(SCOPED_VALUE, str).run(thread1);
        ScopedValue.where(SCOPED_VALUE, str).run(thread2);
        // 执行完毕自动清空，这里获取不到了。
        System.out.println(SCOPED_VALUE.orElse("没有信息"));
    }
    public static void handle() {
        String result = SCOPED_VALUE.get();
        System.out.println(result);
    }
}
```
运行：
```bash
➜  src $ /jdk-20.0.1.jdk/Contents/Home/bin/java --version                                                         
openjdk 20.0.1 2023-04-18
OpenJDK Runtime Environment (build 20.0.1+9-29)
OpenJDK 64-Bit Server VM (build 20.0.1+9-29, mixed mode, sharing)
  
➜  src $ /jdk-20.0.1.jdk/Contents/Home/bin/java  --add-modules jdk.incubator.concurrent Jep429ScopedValueTest.java
WARNING: Using incubator modules: jdk.incubator.concurrent
hello wdbyte.com
hello wdbyte.com  
没有信息
```
可见使用 ScopedValue 有几个显而易见的好处。

- 代码方便，容易理解。符合编程逻辑。
- 不允许修改值，安全性高。（没有 set 方法）
- **生命周期明确**。只传递到 `run()` 方法体中。
- **不需要清理，自动释放。**
- 从实现来讲，也是一种轻量级实现。
<a name="EKfQg"></a>
## JEP 432: Record 模式匹配（二次预览）
在 Java 14 的 JEP 359 中增加了 Record 类，在 Java 16 的 JEP 394中，新增了 instanceof 模式匹配。<br />这两项都简化了 Java 开发的代码编写。在 Java 19 的 JEP 405 中，增又加了 Record 模式匹配功能的第一次预览，这把 JEP 359 和 JEP 394 的功能进行了结合，但是还不够强大，现在 JEP 432 又对其进行了增强。<br />JEP 359 功能回顾：
```java
public class RecordTest {
    public static void main(String[] args) {
        Dog dog = new Dog("name", 1);
        System.out.println(dog.name()); // name
        System.out.println(dog.age());  // 1
    }
}

record Dog(String name, Integer age) {
}
```
JEP 394 功能回顾：
```java
// Old code
if (obj instanceof String) {
    String s = (String)obj;
    ... use s ...
}

// New code
if (obj instanceof String s) {
    ... use s ...
}
```
<a name="O02A1"></a>
### JEP 432 例子
而现在，可以进行更加复杂的组合嵌套，依旧可以准确识别类型。
```java
public class Jep432RecordAndInstance {
    public static void main(String[] args) {
        ColoredPoint coloredPoint1 = new ColoredPoint(new Point(0, 0), Color.RED);
        ColoredPoint coloredPoint2 = new ColoredPoint(new Point(1, 1), Color.GREEN);
        Rectangle rectangle = new Rectangle(coloredPoint1, coloredPoint2);
        printUpperLeftColoredPoint(rectangle);
    }

    static void printUpperLeftColoredPoint(Rectangle r) {
        if (r instanceof Rectangle(ColoredPoint ul, ColoredPoint lr)) {
            System.out.println(ul.c());
        }
    }
}

record Point(int x, int y) {}
enum Color { RED, GREEN, BLUE }
record ColoredPoint(Point p, Color c) {}
record Rectangle(ColoredPoint upperLeft, ColoredPoint lowerRight) {}
```
输出：RED。
<a name="U8h1t"></a>
## JEP 433: switch 模式匹配（四次预览）
Switch 的使用体验改造早在 Java 17 就已经开始了，现在 JEP 433 进行第四次预览，对其功能进行了增强，直接从下面的新老代码看其变化。
```java
public class JEP433SwitchTest {
    public static void main(String[] args) {
        Object obj = 123;
        System.out.println(matchOld(obj)); // 是个数字
        System.out.println(matchNew(obj)); // 是个数字
        obj = "wdbyte.com";
        System.out.println(matchOld(obj)); // 是个字符串，长度大于2
        System.out.println(matchNew(obj)); // 是个字符串，长度大于2
    }

    /**
     * 老代码
     *
     * @param obj
     * @return
     */
    public static String matchOld(Object obj) {
        if (obj == null) {
            return "数据为空";
        }
        if (obj instanceof String) {
            String s = obj.toString();
            if (s.length() > 2) {
                return "是个字符串，长度大于2";
            }
            if (s.length() <= 2) {
                return "是个字符串，长度小于等于2";
            }
        }
        if (obj instanceof Integer) {
            return "是个数字";
        }
        throw new IllegalStateException("未知数据：" + obj);
    }

    /**
     * 新代码
     *
     * @param obj
     * @return
     */
    public static String matchNew(Object obj) {
        String res = switch (obj) {
            case null -> "数据为空";
            case String s when s.length() > 2 -> "是个字符串，长度大于2";
            case String s when s.length() <= 2 -> "是个字符串，长度小于等于于2";
            case Integer i -> "是个数字";
            default -> throw new IllegalStateException("未知数据：" + obj);
        };
        return res;
    }

}
```
<a name="dfBYX"></a>
## JEP 434: 外部函数和内存 API（二次预览)
此功能引入的 API 允许 Java 开发者与 JVM 之外的代码和数据进行交互，通过调用外部函数（JVM 之外）和安全的访问外部内存（非 JVM 管理），让 Java 程序可以调用本机库并处理本机数据，而不会像 JNI 一样存在很多安全风险。<br />这不是一个新功能，自 Java 14 就已经引入，此次对其进行了性能、通用性、安全性、易用性上的优化。<br />_历史_

- Java 14 [JEP 370](https://openjdk.org/jeps/11) 引入了外部内存访问 API（孵化器）。
- Java 15 [JEP 383](https://openjdk.java.net/jeps/383) 引入了外部内存访问 API（第二孵化器）。
- Java 16 [JEP 389](https://openjdk.java.net/jeps/389) 引入了外部链接器 API（孵化器）。
- Java 16 [JEP 393](https://openjdk.java.net/jeps/393) 引入了外部内存访问 API（第三孵化器）。
- Java 17 [JEP 412](https://openjdk.java.net/jeps/412) 引入了外部函数和内存 API（孵化器）。
- Java 18 [JEP 419](https://openjdk.java.net/jeps/419) 引入了外部函数和内存 API（二次孵化器）。
- Java 19 [JEP 424](https://openjdk.java.net/jeps/419) 引入了外部函数和内存 API（孵化器）。
<a name="Epe4Z"></a>
## JEP 436: 虚拟线程（二次预览）
通过将轻量级虚拟线程引入 Java 平台，简化了编写、维护和观察高吞吐量、并发应用程序的过程。使开发人员能够使用现有的 JDK 工具和技术轻松地排除故障、调试和分析并发应用程序，虚拟线程有助于加速应用程序开发。<br />这个特性自 Java 19 的 JEP 425: 虚拟线程 (预览)引入。
<a name="BOHOg"></a>
## JEP 437: Structured Concurrency（二次孵化）
通过引入用于结构化并发 API 来简化多线程编程。结构化并发将在不同线程中运行的多个任务视为单个工作单元，从而简化错误处理，提高可靠性，增强可观察性。因为是个孵化状态提案，这里不做过多研究。

- 相关 Java 19，[JEP 428](https://openjdk.org/jeps/428)：结构化并发（孵化）
<a name="M364I"></a>
## JEP 438: Vector API（五次孵化）
再次提高性能，实现优于等效标量计算的性能。这是通过引入一个 API 来表达矢量计算，该 API 在运行时可靠地编译为支持的 CPU 架构上的最佳矢量指令，从而实现优于等效标量计算的性能。Vector API 在 JDK 16 到 19 中孵化。JDK 20 整合了这些版本用户的反馈以及性能改进和实现增强。
<a name="C2UvG"></a>
## 引用链接
作用域值（孵化器）: [https://openjdk.org/jeps/429](https://openjdk.org/jeps/429)<br />Record 模式匹配（第二次预览）: [https://openjdk.org/jeps/432](https://openjdk.org/jeps/432)<br />switch 的模式匹配（第四次预览）: [https://openjdk.org/jeps/433](https://openjdk.org/jeps/433)<br />外部函数和内存 API（第二个预览版）: [https://openjdk.org/jeps/434](https://openjdk.org/jeps/434)<br />虚拟线程（第二个预览版）: [https://openjdk.org/jeps/436](https://openjdk.org/jeps/436)<br />结构化并发（第二孵化器）: [https://openjdk.org/jeps/437](https://openjdk.org/jeps/437)<br />Vector API（第五孵化器）: [https://openjdk.org/jeps/438](https://openjdk.org/jeps/438)<br />[https://jdk.java.net/19/:](https://jdk.java.net/19/:) [https://jdk.java.net/20/](https://jdk.java.net/20/)<br />Oracle JDK 20 Archive Downloads: [https://www.oracle.com/java/technologies/javase/jdk20-archive-downloads.html](https://www.oracle.com/java/technologies/javase/jdk20-archive-downloads.html)<br />正在孵化的 API: [https://openjdk.org/jeps/11](https://openjdk.org/jeps/11)<br />JEP 370: [https://openjdk.java.net/jeps/370](https://openjdk.java.net/jeps/370)<br />JEP 383: [https://openjdk.java.net/jeps/383](https://openjdk.java.net/jeps/383)<br />JEP 389: [https://openjdk.java.net/jeps/389](https://openjdk.java.net/jeps/389)<br />JEP 393: [https://openjdk.java.net/jeps/393](https://openjdk.java.net/jeps/393)<br />JEP 412: [https://openjdk.java.net/jeps/412](https://openjdk.java.net/jeps/412)<br />JEP 419: [https://openjdk.java.net/jeps/419](https://openjdk.java.net/jeps/419)<br />JEP 424: [https://openjdk.java.net/jeps/419](https://openjdk.java.net/jeps/419)<br />JEP 428: [https://openjdk.org/jeps/428](https://openjdk.org/jeps/428)<br />Github.com/niumoo/javaNotes: [https://github.com/niumoo/JavaNotes/tree/master/core-java-modules/core-java-20](https://github.com/niumoo/JavaNotes/tree/master/core-java-modules/core-java-20)
