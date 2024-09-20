Java<br />**Java 18** 在2022 年 3 月 22 日正式发布，Java 18 不是一个长期支持版本，这次更新共带来 9 个新功能。<br />OpenJDK Java 18 下载：[https://jdk.java.net/18/](https://jdk.java.net/18/)<br />OpenJDK Java 18 文档：[https://openjdk.java.net/projects/jdk/18/](https://openjdk.java.net/projects/jdk/18/)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640421696680-d7929446-1713-4e7f-a857-09efa3bde564.webp#clientId=u57067bd7-98c9-4&from=paste&id=uab13ca15&originHeight=451&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud595f525-c897-42cd-82ea-0b6a37f487c&title=)<br />值得注意的是：JDK 17 是一个长期支持 (LTS) 版本，将获得 Oracle 至少八年的支持，但 JDK 18 将是一个短期功能版本，只支持六个月。可以在 java.net（[https://jdk.java.net/18/](https://jdk.java.net/18/)） 上找到适用于 Linux、Windows 和 MacOS 的 JDK 18 的尝鲜版本。<br />JDK 18 新功能一览：

| JEP | 描述 |
| --- | --- |
| JEP 400 | [默认为 UTF-8](https://openjdk.java.net/jeps/400) |
| JEP 408 | [简单的网络服务器](https://openjdk.java.net/jeps/408) |
| JEP 413 | [Java API 文档中的代码片段](https://openjdk.java.net/jeps/413) |
| JEP 416 | [使用方法句柄重新实现核心反射](https://openjdk.java.net/jeps/416) |
| JEP 417 | [Vector API（三次孵化）](https://openjdk.java.net/jeps/417) |
| JEP 418 | [互联网地址解析 SPI](https://openjdk.java.net/jeps/418) |
| JEP 419 | [Foreign Function & Memory API (二次孵化)](https://openjdk.java.net/jeps/419) |
| JEP 420 | [switch 模式匹配（二次预览）](https://openjdk.java.net/jeps/420) |
| JEP 421 | [弃用完成删除](https://openjdk.java.net/jeps/421) |

<a name="pdPrT"></a>
### JEP 400：指定 UTF-8 作为标准 Java API 的默认字符集。
通过此更改，依赖于默认字符集的 API 将在所有实现、操作系统、区域设置和配置中保持一致。<br />JDK 一直都是支持 UTF-8 字符编码，这次是把 UTF-8 设置为了默认编码，也就是在不加任何指定的情况下，默认所有需要用到编码的 JDK API 都使用 UTF-8 编码，这样就可以避免因为不同系统，不同地区，不同环境之间产生的编码问题。<br />Mac OS 默认使用 UTF-8 作为默认编码，但是其他操作系统上，编码可能取决于系统的配置或者所在区域的设置。如中国大陆的 windows 使用 GBK 作为默认编码。很多同学初学 Java 时可能都遇到过一个正常编写 Java 类，在 windows 系统的命令控制台中运行却出现乱码的情况。<br />使用下面的命令可以输出 JDK 的当前编码。
```bash
# Mac 系统，默认 UTF-8
➜  ~ java -XshowSettings:properties -version 2>&1 | grep file.encoding
file.encoding = UTF-8
file.encoding.pkg = sun.io
➜  ~
```
下面编写一个简单的 Java 程序，输出默认字符编码，然后输出中文汉字 “你好”，看看 Java 18 和 Java 17 运行区别。<br />系统环境：Windows 11
```java
import java.nio.charset.Charset;

public class Hello{
	public static void main(String[] args) {
		System.out.println(Charset.defaultCharset());
		System.out.println("你好");
	}
}
```
从下面的运行结果中可以看到，使用 JDK 17 运行输出的默认字符编码是 GBK，输出的中文 “你好” 已经乱码了；乱码是因为 VsCode 默认的文本编辑器编码是 UTF-8，而中国地区的 Windows 11 默认字符编码是 GBK，也是 JDK 17 默认获取到的编码，所以会在控制台输出时乱码；而使用 JDK 18 输出的默认编码就是 UTF-8，所以可以正常的输出中文 “你好”<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649126770475-a1063ec9-78b7-418d-8d2f-314c5b108fb1.png#clientId=ud46bffe1-59ae-4&from=paste&id=ua71b0d5c&originHeight=793&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u04d70840-37a6-40b7-b966-6ed5ad2b180&title=)
<a name="HD4t1"></a>
### JEP 408：引入一个简单的 Web 服务器。
在 Java 18 中，提供了一个新命令 jwebserver，运行这个命令可以启动一个**简单的 、最小化的**静态Web 服务器，它不支持 CGI 和 Servlet，所以最好的使用场景是用来测试、教育、演示等需求。<br />其实在如 Python、Ruby、PHP、Erlang 等许多平台都提供了开箱即用的 Web 服务器，可见一个简单的Web 服务器是一个常见的需求，Java 一直没有这方面的支持，现在可以了。<br />在 Java 18 中，使用 jwebserver 启动一个 Web 服务器，默认发布的是当前目录。<br />在当前目录创建一个网页文件 index.html
```html
<html>
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
	</head>
	<body>
		<h1>标题</h1>
	</body>
</html>
```
启动 jwebserver.
```bash
➜  bin ./jwebserver
Binding to loopback by default. For all interfaces use "-b 0.0.0.0" or "-b ::".
Serving /Users/darcy/develop/jdk-18.jdk/Contents/Home/bin and subdirectories on 127.0.0.1 port 8000
URL http://127.0.0.1:8000/
```
浏览器访问：<br />![浏览器访问测试](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1649126881238-d6dfa0de-30a5-4adf-8931-39e720920aee.jpeg#clientId=ud46bffe1-59ae-4&from=paste&id=ue82650ca&originHeight=312&originWidth=858&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u38c27c54-667e-4603-9ca7-d117cbae1a3&title=%E6%B5%8F%E8%A7%88%E5%99%A8%E8%AE%BF%E9%97%AE%E6%B5%8B%E8%AF%95 "浏览器访问测试")<br />有请求时会在控制台输出请求信息：
```bash
127.0.0.1 - - [26/3月/2022:16:53:30 +0800] "GET /favicon.ico HTTP/1.1" 404 -
127.0.0.1 - - [26/3月/2022:16:55:13 +0800] "GET / HTTP/1.1" 200 -
```
通过 help 参数可以查看 jwebserver 支持的参数。
```bash
➜  bin ./jwebserver --help
Usage: jwebserver [-b bind address] [-p port] [-d directory]
                  [-o none|info|verbose] [-h to show options]
                  [-version to show version information]
Options:
-b, --bind-address    - 绑定地址. Default: 127.0.0.1 (loopback).
                        For all interfaces use "-b 0.0.0.0" or "-b ::".
-d, --directory       - 指定目录. Default: current directory.
-o, --output          - Output format. none|info|verbose. Default: info.
-p, --port            - 绑定端口. Default: 8000.
-h, -?, --help        - Prints this help message and exits.
-version, --version   - Prints version information and exits.
To stop the server, press Ctrl + C.
```
<a name="faEKe"></a>
### JEP 413：支持在 Java API 文档中加入代码片段。
 为 JavaDoc 的 Standard Doclet 引入一个 `@snippet` 标记，以简化 API 文档中嵌入示例源代码的难度。<br />以前在Java代码的注释中如果要写一些样例非常麻烦，甚至还要进行字符转义。现在Java注释引入了一个新的标记  `@snippet` 来解决注释中包含代码片段样例的问题。<br />在 Java 18 之前，使用 `<pre>{@code ...}</pre>` 来引入代码片段。
```java
 /**
  * 时间工具类
  * Java 18 之前引入代码片段：
  * <pre>{@code
  *     public static String timeStamp() {
  *        long time = System.currentTimeMillis();
  *         return String.valueOf(time / 1000);
  *     }
  * }</pre>
  *
  */
```
生成 Javadoc 之后，效果如下：<br />![Javadoc 代码片段](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1649127181082-4d85b13e-ca92-49d7-ad1b-6dd23db613c8.jpeg#clientId=ud46bffe1-59ae-4&from=paste&id=ue931accc&originHeight=562&originWidth=966&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=uf19b4ba4-c35d-4f82-b89c-695563c4652&title=Javadoc%20%E4%BB%A3%E7%A0%81%E7%89%87%E6%AE%B5 "Javadoc 代码片段")
<a name="mQTRr"></a>
#### 高亮代码片段
从 Java 18 开始，可以使用 `@snippet` 来生成注释，且可以高亮某个代码片段。
```java
/**
 * 在 Java 18 之后可以使用新的方式
 * 下面的代码演示如何使用 {@code Optional.isPresent}:
 * {@snippet :
 * if (v.isPresent()) {
 *     System.out.println("v: " + v.get());
 * }
 * }
 *
 * 高亮显示 println
 *
 * {@snippet :
 * class HelloWorld {
 *     public static void main(String... args) {
 *         System.out.println("Hello World!");      // @highlight substring="println"
 *     }
 * }
 * }
 *
 */
```
效果如下，更直观，效果更好。<br />![Java 18 Javadoc](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1649127181065-4261bf55-6bfc-487d-ad3b-da0c0df1aac6.jpeg#clientId=ud46bffe1-59ae-4&from=paste&id=u22f9ea0b&originHeight=476&originWidth=1000&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=uaf5e6525-c965-4ad2-a7d6-be489a00640&title=Java%2018%20Javadoc "Java 18 Javadoc")
<a name="g29Js"></a>
#### 正则高亮代码片段
甚至可以使用正则来高亮某一段中的某些关键词：
```java
/** 
  * 正则高亮：
  * {@snippet :
  *   public static void main(String... args) {
  *       for (var arg : args) {                 // @highlight region regex = "\barg\b"
  *           if (!arg.isBlank()) {
  *               System.out.println(arg);
  *           }
  *       }                                      // @end
  *   }
  *   }
  */
```
生成的 Javadoc 效果如下:<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1649127181108-eb59c656-c595-4bef-a38b-78a4e142dfa6.jpeg#clientId=ud46bffe1-59ae-4&from=paste&id=u01c7817a&originHeight=406&originWidth=784&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u44eb0efa-29a9-4b16-9bb9-1b6004af2f6&title=)
<a name="dz0UY"></a>
#### 替换代码片段
可以使用正则表达式来替换某一段代码。
```java
 /** 
   * 正则替换：
   * {@snippet :
   * class HelloWorld {
   *     public static void main(String... args) {
   *         System.out.println("Hello World!");  // @replace regex='".*"' replacement="..."
   *     }
   * }
   * }
   */
```
这段注释会生成如下 Javadoc 效果。
```java
class HelloWorld {
    public static void main(String... args) {
        System.out.println(...);
    }
}
```
<a name="ARqIu"></a>
#### 内联使用
```java
/**
 * The following code shows how to use {@code Optional.isPresent}:
 * {@snippet :
 * if (v.isPresent()) {
 *     System.out.println("v: " + v.get());
 * }
 * }
 */
```
<a name="hWLQi"></a>
#### 引用外部片段
```java
/**
 * The following code shows how to use {@code Optional.isPresent}:
 * {@snippet file="ShowOptional.java" region="example"}
 */
```
ShowOptional.java就是引用的源代码：
```java
public class ShowOptional {
    void show(Optional<String> v) {
        // @start region="example"
        if (v.isPresent()) {
            System.out.println("v: " + v.get());
        }
        // @end
    }
}
```
<a name="tueRH"></a>
#### 附：Javadoc 生成方式
```bash
# 使用 javadoc 命令生成 Javadoc 文档
➜  bin ./javadoc -public -sourcepath ./src -subpackages com -encoding utf-8 -charset utf-8 -d ./javadocout
# 使用 Java 18 的 jwebserver 把生成的 Javadoc 发布测试
➜  bin ./jwebserver -d /Users/darcy/develop/javadocout
```
访问测试：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1649127181141-01577be1-68ac-4426-aaf7-52d9b1ad2725.jpeg#clientId=ud46bffe1-59ae-4&from=paste&id=Tmyyg&originHeight=403&originWidth=1000&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua5d21dc2-608c-4c5f-a823-3b69af850d7&title=)
<a name="MmfpK"></a>
### JEP 416 ：用方法句柄重新实现核心反射。 
在 java.lang.invoke 的方法句柄之上，**重构** java.lang.reflect 的方法、构造函数和字段，使用方法句柄处理反射的底层机制将减少 java.lang.reflect 和 java.lang.invoke 两者的 API 维护和开发成本。<br />Java 18 改进了 java.lang.reflect.Method、Constructor 的实现逻辑，使之性能更好，速度更快。这项改动不会改动相关 API ，这意味着开发中不需要改动反射相关代码，就可以体验到性能更好反射。<br />OpenJDK 官方给出了新老实现的反射性能基准测试结果。<br />Java 18 之前：
```bash
Benchmark                                     Mode  Cnt   Score  Error  Units
ReflectionSpeedBenchmark.constructorConst     avgt   10  68.049 ± 0.872  ns/op
ReflectionSpeedBenchmark.constructorPoly      avgt   10  94.132 ± 1.805  ns/op
ReflectionSpeedBenchmark.constructorVar       avgt   10  64.543 ± 0.799  ns/op
ReflectionSpeedBenchmark.instanceFieldConst   avgt   10  35.361 ± 0.492  ns/op
ReflectionSpeedBenchmark.instanceFieldPoly    avgt   10  67.089 ± 3.288  ns/op
ReflectionSpeedBenchmark.instanceFieldVar     avgt   10  35.745 ± 0.554  ns/op
ReflectionSpeedBenchmark.instanceMethodConst  avgt   10  77.925 ± 2.026  ns/op
ReflectionSpeedBenchmark.instanceMethodPoly   avgt   10  96.094 ± 2.269  ns/op
ReflectionSpeedBenchmark.instanceMethodVar    avgt   10  80.002 ± 4.267  ns/op
ReflectionSpeedBenchmark.staticFieldConst     avgt   10  33.442 ± 2.659  ns/op
ReflectionSpeedBenchmark.staticFieldPoly      avgt   10  51.918 ± 1.522  ns/op
ReflectionSpeedBenchmark.staticFieldVar       avgt   10  33.967 ± 0.451  ns/op
ReflectionSpeedBenchmark.staticMethodConst    avgt   10  75.380 ± 1.660  ns/op
ReflectionSpeedBenchmark.staticMethodPoly     avgt   10  93.553 ± 1.037  ns/op
ReflectionSpeedBenchmark.staticMethodVar      avgt   10  76.728 ± 1.614  ns/op
```
Java 18 的新实现：
```bash
Benchmark                                     Mode  Cnt    Score   Error  Units
ReflectionSpeedBenchmark.constructorConst     avgt   10   32.392 ± 0.473  ns/op
ReflectionSpeedBenchmark.constructorPoly      avgt   10  113.947 ± 1.205  ns/op
ReflectionSpeedBenchmark.constructorVar       avgt   10   76.885 ± 1.128  ns/op
ReflectionSpeedBenchmark.instanceFieldConst   avgt   10   18.569 ± 0.161  ns/op
ReflectionSpeedBenchmark.instanceFieldPoly    avgt   10   98.671 ± 2.015  ns/op
ReflectionSpeedBenchmark.instanceFieldVar     avgt   10   54.193 ± 3.510  ns/op
ReflectionSpeedBenchmark.instanceMethodConst  avgt   10   33.421 ± 0.406  ns/op
ReflectionSpeedBenchmark.instanceMethodPoly   avgt   10  109.129 ± 1.959  ns/op
ReflectionSpeedBenchmark.instanceMethodVar    avgt   10   90.420 ± 2.187  ns/op
ReflectionSpeedBenchmark.staticFieldConst     avgt   10   19.080 ± 0.179  ns/op
ReflectionSpeedBenchmark.staticFieldPoly      avgt   10   92.130 ± 2.729  ns/op
ReflectionSpeedBenchmark.staticFieldVar       avgt   10   53.899 ± 1.051  ns/op
ReflectionSpeedBenchmark.staticMethodConst    avgt   10   35.907 ± 0.456  ns/op
ReflectionSpeedBenchmark.staticMethodPoly     avgt   10  102.895 ± 1.604  ns/op
ReflectionSpeedBenchmark.staticMethodVar      avgt   10   82.123 ± 0.629  ns/op
```
可以看到在某些场景下性能稍微好些。
<a name="Ksn5O"></a>
### JEP 417：Vector API（第三孵化器）。
引入一个 API 来表达向量计算，这些计算在运行时可以编译为支持的 CPU 架构上的最佳向量指令，从而实现优于等效标量计算的性能。<br />在 Java 16 中引入一个新的 API 来进行向量计算，它可以在运行时可靠的编译为支持的 CPU 架构，从而实现更优的计算能力。<br />在 Java 17 中改进了 Vector API 性能，增强了例如对字符的操作、字节向量与布尔数组之间的相互转换等功能。<br />现在在 JDK 18 中将继续优化其性能。
<a name="h2RhW"></a>
### JEP 418：互联网地址解析 SPI。
定义用于主机名和地址解析的服务提供者接口 (SPI)，以便`java.net.InetAddress`可以使用平台内置解析器以外的解析器。<br />对于互联网地址解析 SPI，为主机地址和域名地址解析定义一个 SPI，以便java.net.InetAddress可以使用平台内置解析器以外的解析器。
```java
InetAddress inetAddress = InetAddress.getByName("www.yuque.com");
System.out.println(inetAddress.getHostAddress());
// 输出
// 106.14.229.49
```
<a name="wxCUE"></a>
### JEP 419：外部函数和内存 API（第二孵化器）。 
引入了一个新 API，Java 程序可以通过它与 Java 运行时之外的代码和数据进行互操作。通过有效地调用外部函数(即 JVM 外的代码)，并安全地访问外部内存(即不由 JVM 管理的内存)，外部函数和内存 API 使 Java 程序能够调用本机库并处理本机数据，而不具有 JNI 的脆弱性和危险。<br />_Foreign Function & Memory API_ ( [**JEP 419**](https://openjdk.java.net/jeps/419) ) 是此版本中实现的更重要的 JEP 之一，因为它是[**Project Panama**](https://openjdk.java.net/projects/panama/)中包含的孵化组件之一。  **Panama** 正在简化将 Java 程序连接到非 **Java** 组件的过程。这一特殊功能在其第二次孵化迭代中引入了一个 **API**，**Java** 程序通过该 API 调用**Native**类库并处理**Native**数据。目的是取代设计的非常不理想的**Java Native Interface (JNI)**。<br />大家都知道其它语言有非常棒的一些类库，但是**Java**想调用其它语言的类库目前需要使用**JNI**。但是**JNI**被设计得太复杂了，让很多**Java**开发者难以上手。如果这一状况得到改变，那么利用Java去调用一些**C**或者**C++音视频处理库和Python**的机器学习库将是非常容易的事情。<br />新的 API 允许 Java 开发者与 JVM 之外的代码和数据进行交互，通过调用外部函数，可以在不使用 JNI 的情况下调用本地库。<br />这是一个孵化功能；需要添加`--add-modules jdk.incubator.foreign`来编译和运行 Java 代码，Java 18 改进了相关 API ，使之更加简单易用。<br />_历史_

- • Java 14 [JEP 370 (opens new window)](https://openjdk.java.net/jeps/370)引入了外部内存访问 API（孵化器）。
- • Java 15 [JEP 383 (opens new window)](https://openjdk.java.net/jeps/383)引入了外部内存访问 API（第二孵化器）。
- • Java 16 [JEP 389 (opens new window)](https://openjdk.java.net/jeps/389)引入了外部链接器 API（孵化器）。
- • Java 16 [JEP 393 (opens new window)](https://openjdk.java.net/jeps/393)引入了外部内存访问 API（第三孵化器）。
- • Java 17 [JEP 412 (opens new window)](https://openjdk.java.net/jeps/412)引入了外部函数和内存 API（孵化器）。
<a name="opast"></a>
### JEP 420：switch 模式匹配表达式。 
使用 `switch` 表达式和语句的模式匹配以及对模式语言的扩展来增强 Java 编程语言。将模式匹配扩展到 `switch` 允许针对多个模式测试表达式，每个模式都有特定的操作，可以简洁安全地表达复杂的面向数据的查询。<br />实现的唯一真正影响 Java 语言的 JEP 是_Pattern Matching for switch_ ( [**JEP 420**](https://openjdk.java.net/jeps/420) )，它在 Java 17 中首次预览（这是第二次预览）。_其目的是“通过对switch_ 表达式和语句的模式匹配以及对模式语言的扩展来增强 Java 编程语言 。在 Java 16 中，[**JEP 394**](https://openjdk.java.net/jeps/394)扩展了`instanceof`运算符以采用_类型模式_并执行_模式匹配_：
```java
// Old code
if (o instanceof String) {
    String s = (String)o;
    ... use s ...
}

// New code
if (o instanceof String s) {
    ... use s ...
}
```
使用`instanceof`后无需再对对象进行类型转换就可以使用其真实的类型。<br />**Java 14**又引入了`switch`表达式：
```java
int numLetters = switch (day) {
    case MONDAY, FRIDAY, SUNDAY -> 6;
    case TUESDAY                -> 7;
    case THURSDAY, SATURDAY     -> 8;
    case WEDNESDAY              -> 9;
    default                     -> 11;    
};
```
如果这两个能结合起来，`switch`能进行模式匹配的话，下面的句子将大大简化：
```java
static String formatter(Object o) {
    String formatted = "unknown";
    if (o instanceof Integer i) {
        formatted = String.format("int %d", i);
    } else if (o instanceof Long l) {
        formatted = String.format("long %d", l);
    } else if (o instanceof Double d) {
        formatted = String.format("double %f", d);
    } else if (o instanceof String s) {
        formatted = String.format("String %s", s);
    }
    return formatted;
}
```
**JEP 420**的预览特性，将会把上面冗长的代码简化为：
```java
static String formatterPatternSwitch(Object o) {
    return switch (o) {
        case Integer i -> String.format("int %d", i);
        case Long l    -> String.format("long %d", l);
        case Double d  -> String.format("double %f", d);
        case String s  -> String.format("String %s", s);
        default        -> o.toString();
    };
}
```
是不是更加清晰了<br />switch 可以和 null 进行结合判断：
```java
static void testFooBar(String s) {
    switch (s) {
        case null         -> System.out.println("Oops");
        case "Foo", "Bar" -> System.out.println("Great");
        default           -> System.out.println("Ok");
    }
}
```
case 时可以加入复杂表达式：
```java
static void testTriangle(Shape s) {
    switch (s) {
        case Triangle t && (t.calculateArea() > 100) ->
            System.out.println("Large triangle");
        default ->
            System.out.println("A shape, possibly a small triangle");
    }
}
```
case 时可以进行类型判断：
```java
sealed interface S permits A, B, C {}
final class A implements S {}
final class B implements S {}
record C(int i) implements S {}  // Implicitly final

static int testSealedExhaustive(S s) {
    return switch (s) {
        case A a -> 1;
        case B b -> 2;
        case C c -> 3;
    };
}
```
<a name="zqj56"></a>
### JEP 421：弃用 Finalization 功能。
`Object`对象有一个`finalize`方法，该方法用于实例被垃圾回收器回收的时触发的操作。当 GC (垃圾回收器) 确定不存在对该对象的有更多引用时，对象的垃圾回收器就会调用这个方法。当时它的设计用来避免内存泄露，现在已经有了更好的替代方案try-with-resources和**Java 9**引入的 `java.lang.ref.Cleaner` 。<br />因此，所有该方法会被标记为过时，未来将被移除。<br />Java 1.0 中引入的 Finalization 旨在帮助避免资源泄漏问题，然而这个功能存在**延迟不可预测、行为不受约束，以及线程无法指定**等缺陷，导致其安全性、性能、可靠性和可维护性方面都存在问题，因此将其弃用，用户可选择迁移到其他资源管理技术，例如try-with-resources 语句和清洁器。<br />2022 年 1 月 20 日会进入 Rampdown 第二阶段，初始和最终候选（RC）版本将分别于明年 2 月 10 日和 2 月 24 日发布，稳定版 JDK 18 将在 2022 年 3 月 22 日发布，可在 JDK 公告页中查看最新消息。<br />JDK 公告页：[https://openjdk.java.net/projects/jdk/18/](https://openjdk.java.net/projects/jdk/18/)
<a name="eJryy"></a>
### 参考资料
Java 18: [http://openjdk.java.net/projects/jdk/18/](http://openjdk.java.net/projects/jdk/18/)<br />java.net.InetAddress: [https://docs.oracle.com/en/java/javase/17/docs/api/java.base/java/net/InetAddress.html](https://docs.oracle.com/en/java/javase/17/docs/api/java.base/java/net/InetAddress.html)<br />JEP 419: [https://openjdk.java.net/jeps/419](https://openjdk.java.net/jeps/419)<br />Project Panama: [https://openjdk.java.net/projects/panama/](https://openjdk.java.net/projects/panama/)<br />JEP 420: [https://openjdk.java.net/jeps/420](https://openjdk.java.net/jeps/420)<br />JEP 394: [https://openjdk.java.net/jeps/394](https://openjdk.java.net/jeps/394)<br />默认为 UTF-8: [https://openjdk.java.net/jeps/400](https://openjdk.java.net/jeps/400)<br />简单的网络服务器: [https://openjdk.java.net/jeps/408](https://openjdk.java.net/jeps/408)<br />Java API 文档中的代码片段: [https://openjdk.java.net/jeps/413](https://openjdk.java.net/jeps/413)<br />使用方法句柄重新实现核心反射: [https://openjdk.java.net/jeps/416](https://openjdk.java.net/jeps/416)<br />Vector API（三次孵化）: [https://openjdk.java.net/jeps/417](https://openjdk.java.net/jeps/417)<br />互联网地址解析 SPI: [https://openjdk.java.net/jeps/418](https://openjdk.java.net/jeps/418)<br />oreign Function & Memory API (二次孵化): [https://openjdk.java.net/jeps/419](https://openjdk.java.net/jeps/419)<br />switch 模式匹配（二次预览）: [https://openjdk.java.net/jeps/420](https://openjdk.java.net/jeps/420)<br />弃用完成删除: [https://openjdk.java.net/jeps/421](https://openjdk.java.net/jeps/421)<br />JEP 370 (opens new window): [https://openjdk.java.net/jeps/370](https://openjdk.java.net/jeps/370)<br />JEP 383 (opens new window): [https://openjdk.java.net/jeps/383](https://openjdk.java.net/jeps/383)<br />JEP 389 (opens new window): [https://openjdk.java.net/jeps/389](https://openjdk.java.net/jeps/389)<br />JEP 393 (opens new window): [https://openjdk.java.net/jeps/393](https://openjdk.java.net/jeps/393)<br />JEP 412 (opens new window): [https://openjdk.java.net/jeps/412](https://openjdk.java.net/jeps/412)
