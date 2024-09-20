Java

- 什么是`System.out.println()`
- UML图
- 输出重定向——改变输出路径
- `System.out.println`性能分析
- `System.out.println` VS 日志记录组件（Log4j等）
- 静态导入来缩短`System.out.println`
- `System.err`和`System.in`
<a name="WwJIo"></a>
## 什么是`System.out.println()`
`System.out.println`是一个Java语句，一般情况下是将传递的参数，打印到控制台。<br />**「System」**：是 java.lang包中的一个**「final类」**。根据javadoc，“`java.lang.System`该类提供的设施包括标准输入，标准输出和错误输出流；访问外部定义的属性和环境变量; 一种加载文件和库的方法; 以及用于快速复制数组等一部分的实用方法... ”<br />**「out」**：是System类的**「静态成员字段」**，类型为`PrintStream`。
```java
public static final PrintStream out
```
他在启动时就会被实例化，并与主机的标准输出控制台进行映射。该流在实例化之后立即打开，并准备接受数据。<br />**「println」**：是`PrintStream`类的一个方法。`println`打印（参数内容+换行符） 到控制台。<br />`PrintStream`类中有多个重载的`println`方法。每个`println`是通过调用`print`方法并添加一个换行符实现的。`print`方法是通过调用`write`方法实现的。<br />以下是JDK源中`System.out.println`的代码片段。<br />部分代码段：
```java
public final class System {  
    static PrintStream out;  
    static PrintStream err;  
    static InputStream in;  
    ...  
}  
  
public class PrintStream extends FilterOutputStream {  
    //out object is inherited from FilterOutputStream class  
    public void println() {  
    ...  
}
```
<a name="eN3Ye"></a>
## 输出重定向——改变输出路径
out对象可以自定义的。在启动时由java运行时环境初始化，并且可以在执行期间由开发人员更改。代替在默认情况下的标准输出。当您通过命令行运行程序时，输出将打印在同一个命令窗口中。可以使用setOut方法来改变这种行为。在以下示例中，我将输出重定向到同一目录中的文本文件。
```
`public class ChangeOut {  
  public static void main(String args[]) {  
    try {  
      System.setOut(new PrintStream(new FileOutputStream("log.txt")));  
      System.out.println("Now the output is redirected!");  
    } catch(Exception e) {}  
  }  
}  
`
```
<a name="LMtl4"></a>
## `System.out.println`性能分析
有一个普遍的观念需要大家知道——`System.out.println`性能并不好。当深入分析时，其调用顺序如下`println` - > `print` - > `write()`+ `newLine()`。这个顺序流是Sun / Oracle JDK的实现。`write()`和`newLine()`都包含一个`synchronized`块。同步有一点开销，但更多的是添加字符到缓冲区和打印的开销更大。<br />当运行性能分析时，运行多个`System.out.println`并记录时间，执行时间会按比例增加。当打印超过50个字符并打印超过50,000行时，性能下降。<br />当然这一切都取决于使用的场景。不过无论如何请勿使用`System.out.println`打印日志（ logging）到stdout。
<a name="SsKhw"></a>
## `System.out.println` VS 日志记录组件（Log4j等）
Log4J具有多种记录级别。如果正在编写一个小程序，只是为了实验/学习目的那么使用 `System.out.println` 就很不错。但当开发生产质量软件时，应该注意到应该使用记录组件（log4j等），并且应该避免使用`System.out.println`。为什么？

- **「灵活性」**：log4j的记录器提供了多种记录级别。可以相应地分隔日志信息。例如，X消息只能在PRODUCTION上打印，Y消息应打印在ERROR等上。
- **「可重构性」**：log4j只需一个参数更改即可关闭所有日志记录。
- **「可维护性」**：想象一下，如果有数百个`System.out.println`全部通过应用程序散落，那么在一段时间内将难以维护程序。
- **「粒度」**：在应用程序中，每个类都可以有不同的记录器并相应地进行控制。
- **「实用性」**：在`System.out`中限制重定向消息的选项，但是如果是记录器(like log4j)，则可以提供多种选项。甚至可以创建自定义输出选项并将其重定向。

所以不应该使用`System.out.println`进行日志记录和调试（logging and debugging）
<a name="YoQb0"></a>
## 静态导入来缩短`System.out.println`
有时觉得`System.out.println`是一个很长的语句要打印。静态导入可能会缩短一点，但不推荐使用，因为它导致可读性差。我只是使用这种情况来解释静态导入，并避免在下面的情况下使用它。
```java
import static java.lang.System.out;  

public class ShortSOP {  
    public static void main(String[] args) {  
        out.println("Hello, world");  
    }  
}  
```
不静态导入的话直接写`out.println`会提示编译错误的。
<a name="V6IHQ"></a>
## `System.err`和`System.in`
作为相关部分，接下来介绍一下“err”和“in”。'in'与`InputStream`相关联。与“out”相对，“in”用于从标准控制台通用键盘获取输入。<br />'err'与`PrintStream`相关联，并将参数打印到标准错误输出流。当使用eclipse等的IDE时，可以看到“out”和“err”之间的输出差异。
```java
import java.io.*;  

public class InOutErr {  
    public static void main(String args[]) {  
        try {  
            System.out.println("请输入一段话");  
            BufferedReader reader = new BufferedReader(new InputStreamReader(System.in ));  
            String filename = reader.readLine();  

            InputStream input = new FileInputStream(filename);  
            System.out.println("File opened...");  

        } catch (IOException e){  
            System.err.println("Where is that file?");  
        }  
    }  
}
```
