Java try-catch-finally
<a name="o5eQJ"></a>
## 坑1：finally中使用return
若在 finally 中使用 return，那么即使 try-catch 中有 return 操作，也不会立马返回结果，而是再执行完 finally 中的语句再返回。此时问题就产生了：**如果 finally 中存在 return 语句，则会直接返回 finally 中的结果，从而无情的丢弃了 try 中的返回值。**
<a name="3dJbo"></a>
### ① 反例代码
```java
public static void main(String[] args) throws FileNotFoundException {
    System.out.println("执行结果:" + test());
}
private static int test() {
    int num = 0;
    try {
        // num=1,此处不返回
        num++;
        return num;
    } catch (Exception e) {
        // do something
    } finally {
        // num=2,返回此值
        num++;
        return num;
    }
}
```
以上代码的执行结果如下：
<a name="BM9FK"></a>
### ![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1616565023007-2e263cdc-ad63-41bd-b08b-34a2e04f591e.webp#averageHue=%23373737&height=224&id=Tjwfw&originHeight=224&originWidth=766&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=766)
<a name="QcF4n"></a>
### ② 原因分析
如果在 finally 中存在 return 语句，那么 try-catch 中的 return 值都会被覆盖，如果程序员在写代码的时候没有发现这个问题，那么就会导致程序的执行结果出错。
<a name="o9FUP"></a>
### ③ 解决方案
如果 try-catch-finally 中存在 return 返回值的情况，**一定要确保 return 语句只在方法的尾部出现一次**。
<a name="w3GGU"></a>
### ④ 正例代码
```java
public static void main(String[] args) throws FileNotFoundException {
    System.out.println("执行结果:" + testAmend());
}
private static int testAmend() {
    int num = 0;
    try {
        num = 1;
    } catch (Exception e) {
        // do something
    } finally {
        // do something
    }
    // 确保 return 语句只在此处出现一次
    return num;
}
```
<a name="kVAXR"></a>
## 坑2：finally中的代码“不执行”
如果说上面的示例比较简单，那么下面这个示例会有不同的感受，直接来看代码。
<a name="Gl8vI"></a>
### ① 反例代码
```java
public static void main(String[] args) throws FileNotFoundException {
    System.out.println("执行结果:" + getValue());
}
private static int getValue() {
    int num = 1;
    try {
        return num;
    } finally {
        num++;
    }
}
```
以上代码的执行结果如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1616565022832-90f38a82-e83d-4f91-bf41-59d77feeb69a.webp#averageHue=%23383838&height=136&id=IoWE0&originHeight=136&originWidth=748&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=748)
<a name="pmKnf"></a>
### ② 原因分析
**本以为执行的结果会是 2，但万万没想到竟然是 1**。<br />有人可能会问：如果把代码换成 ++num，那么结果会不会是 2 呢？<br />答案是并不会，执行的结果依然是 1。那为什么会这样呢？想要真正的搞懂它，就得从这段代码的字节码说起了。<br />以上代码最终生成的字节码如下：
```java
// class version 52.0 (52)
// access flags 0x21
public class com/example/basic/FinallyExample {
  // compiled from: FinallyExample.java
  // access flags 0x1
  public <init>()V
   L0
    LINENUMBER 5 L0
    ALOAD 0
    INVOKESPECIAL java/lang/Object.<init> ()V
    RETURN
   L1
    LOCALVARIABLE this Lcom/example/basic/FinallyExample; L0 L1 0
    MAXSTACK = 1
    MAXLOCALS = 1
  // access flags 0x9
  public static main([Ljava/lang/String;)V throws java/io/FileNotFoundException 
   L0
    LINENUMBER 13 L0
    GETSTATIC java/lang/System.out : Ljava/io/PrintStream;
    NEW java/lang/StringBuilder
    DUP
    INVOKESPECIAL java/lang/StringBuilder.<init> ()V
    LDC "\u6267\u884c\u7ed3\u679c:"
    INVOKEVIRTUAL java/lang/StringBuilder.append (Ljava/lang/String;)Ljava/lang/StringBuilder;
    INVOKESTATIC com/example/basic/FinallyExample.getValue ()I
    INVOKEVIRTUAL java/lang/StringBuilder.append (I)Ljava/lang/StringBuilder;
    INVOKEVIRTUAL java/lang/StringBuilder.toString ()Ljava/lang/String;
    INVOKEVIRTUAL java/io/PrintStream.println (Ljava/lang/String;)V
   L1
    LINENUMBER 14 L1
    RETURN
   L2
    LOCALVARIABLE args [Ljava/lang/String; L0 L2 0
    MAXSTACK = 3
    MAXLOCALS = 1
  // access flags 0xA
  private static getValue()I
    TRYCATCHBLOCK L0 L1 L2 null
   L3
    LINENUMBER 18 L3
    ICONST_1
    ISTORE 0
   L0
    LINENUMBER 20 L0
    ILOAD 0
    ISTORE 1
   L1
    LINENUMBER 22 L1
    IINC 0 1
   L4
    LINENUMBER 20 L4
    ILOAD 1
    IRETURN
   L2
    LINENUMBER 22 L2
   FRAME FULL [I] [java/lang/Throwable]
    ASTORE 2
    IINC 0 1
   L5
    LINENUMBER 23 L5
    ALOAD 2
    ATHROW
   L6
    LOCALVARIABLE num I L0 L6 0
    MAXSTACK = 1
    MAXLOCALS = 3
}
```
这些字节码的简易版本如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1616565023012-ec9d94f5-36ac-4216-9114-bbdf8a941bb8.webp#averageHue=%2344464d&height=724&id=Zf2Yx&originHeight=724&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=1080)想要读懂这些字节码，首先要搞懂这些字节码所代表的含义，这些内容可以从 Oracle 的官网查询到（英文文档）：[https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-6.html](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-6.html)<br />在这里对这些字节码做一个简单的翻译：
> iconst 是将 int 类型的值压入操作数栈。istore 是将 int 存储到局部变量。iload 从局部变量加载 int 值。iinc 通过下标递增局部变量。ireturn 从操作数堆栈中返回 int 类型的值。astore 将引用存储到局部变量中。

有了这些信息之后，来翻译一下上面的字节码内容：
```
0 iconst_1   在操作数栈中存储数值 1
1 istore_0   将操作数栈中的数据存储在局部变量的位置 0
2 iload_0    从局部变量读取值到操作数栈
3 istore_1   将操作数栈中存储 1 存储在局部变量的位置 1
4 iinc 0 by 1 把局部变量位置 0 的元素进行递增（+1）操作
7 iload_1 将局部位置 1 的值加载到操作数栈中
8 ireturn 返回操作数栈中的 int 值
```
通过以上信息也许并不能直观的看出此方法的内部执行过程，可以通过方法执行流程图：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1616565407520-7254cf73-911a-4635-841f-0fcd348fd47d.png#averageHue=%23f7f7f7&height=1011&id=U2d3y&originHeight=1011&originWidth=1865&originalType=binary&ratio=1&rotation=0&showTitle=false&size=214868&status=done&style=shadow&title=&width=1865)<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1616565023234-482f1600-46d7-4973-b470-07a25a8c365c.png#averageHue=%23f7f7f7&height=606&id=bXZMY&originHeight=606&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=1080)![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1616565022983-10e2aba1-2f8c-4de5-ba22-6a3d3ea5e369.webp#averageHue=%23f9f9f9&height=778&id=nxgjk&originHeight=778&originWidth=748&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=748)通过以上图片可以看出：在 finally 语句（iinc 0, 1）执行之前，本地变量表中存储了两个信息，位置 0 和位置 1 都存储了一个值为 1 的 int 值。而在执行 finally（iinc 0, 1）之前只把位置 0 的值进行了累加，之后又将位置 1 的值（1）返回给了操作数栈，所以当执行返回操作（ireturn）时会从操作数栈中读到返回值为 1 的结果，因此最终的执行是 1 而不是 2。
<a name="IP3q8"></a>
### ③ 解决方案
关于 Java 虚拟机是如何编译 finally 语句块的问题，有兴趣的读者可以参考《The JavaTM Virtual Machine Specification, Second Edition》中 7.13 节 Compiling finally。那里详细介绍了 Java 虚拟机是如何编译 finally 语句块。<br />实际上，Java 虚拟机会把 finally 语句块作为 subroutine（对于这个 subroutine 不知该如何翻译为好，干脆就不翻译了，免得产生歧义和误解）直接插入到 try 语句块或者 catch 语句块的控制转移语句之前。但是，还有另外一个不可忽视的因素，那就是在执行 subroutine（也就是 finally 语句块）之前，try 或者 catch 语句块会保留其返回值到本地变量表（Local Variable Table）中，待 subroutine 执行完毕之后，再恢复保留的返回值到操作数栈中，然后通过 return 或者 throw 语句将其返回给该方法的调用者（invoker）。<br />因此如果在 try-catch-finally 中如果有 return 操作，**一定要确保 return 语句只在方法的尾部出现一次！**这样就能保证 try-catch-finally 中所有操作代码都会生效。
<a name="MK8gT"></a>
### ④ 正例代码
```java
private static int getValueByAmend() {
    int num = 1;
    try {
        // do something
    } catch (Exception e) {
        // do something
    } finally {
        num++;
    }
    return num;
}
```
<a name="pxEUI"></a>
## 坑3：finally中的代码“非最后”执行
<a name="XJXYR"></a>
### ① 反例代码
```java
public static void main(String[] args) throws FileNotFoundException {
    execErr();
}
private static void execErr() {
    try {
        throw new RuntimeException();
    } catch (RuntimeException e) {
        e.printStackTrace();
    } finally {
        System.out.println("执行 finally.");
    }
}
```
以上代码的执行结果如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1616565023000-977709c2-d575-4ae0-8c02-7cd3622cf5e6.webp#averageHue=%23282c2a&height=282&id=eqe1E&originHeight=282&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=1080)从以上结果可以看出 **finally 中的代码并不是最后执行的，而是在 catch 打印异常之前执行的**，这是为什么呢？
<a name="QCAmt"></a>
### ② 原因分析
产生以上问题的真实原因其实并不是因为 try-catch-finally，当打开 `e.printStackTrace` 的源码就能看出一些端倪了，源码如下：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1616565022997-ba1b712e-1805-4869-9619-abf66fffc97a.png#averageHue=%2332302d&height=198&id=U0OIz&originHeight=198&originWidth=806&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=806)从上图可以看出，当执行 `e.printStackTrace()`  和 finally 输出信息时，使用的并不是同一个对象。**finally 使用的是标准输出流：**`**System.out**`**，而 **`**e.printStackTrace()**`**  使用的却是标准错误输出流：**`**System.err.println**`，它们执行的效果等同于：
```java
public static void main(String[] args) {
    System.out.println("我是标准输出流");
    System.err.println("我是标准错误输出流");
}
```
而以上代码执行结果的顺序也是随机的，而产生这一切的原因，或许可以通过标准错误输出流（System.err）的注释和说明文档中看出：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1616565023027-821ef165-55e4-45c1-a74e-31f3f3bc3db0.webp#averageHue=%23394635&height=535&id=ACHFt&originHeight=535&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=1080)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1616565023270-6a3e61aa-28e2-43b5-958d-5e8111b5fddf.webp#averageHue=%23fcfefe&height=415&id=OmeBQ&originHeight=415&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=1080)简单的对以上的注释做一个简单的翻译：
> “标准”错误输出流。该流已经打开，并准备接受输出数据。通常，此流对应于主机环境或用户指定的显示输出或另一个输出目标。按照惯例，即使主要输出流（out 输出流）已重定向到文件或其他目标位置，该输出流（err 输出流）也能用于显示错误消息或其他信息，这些信息应引起用户的立即注意。

从源码的注释信息可以看出，标准错误输出流（`System.err`）和标准输出流（`System.out`）使用的是不同的流对象，即使标准输出流并定位到其他的文件，也不会影响到标准错误输出流。那么就可以大胆的猜测：二者是独立执行的，并且为了更高效的输出流信息，二者在执行时是并行执行的，因此看到的结果是打印顺序总是随机的。<br />为了验证此观点，将标准输出流重定向到某个文件，然后再来观察 `System.err` 能不能正常打印，实现代码如下：
```java
public static void main(String[] args) throws FileNotFoundException {
    // 将标准输出流的信息定位到 log.txt 中
    System.setOut(new PrintStream(new FileOutputStream("log.txt")));
    System.out.println("我是标准输出流");
    System.err.println("我是标准错误输出流");
}
```
以上代码的执行结果如下：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1616565023111-8b68dbc8-0742-4637-b6fe-79422869eb1a.png#averageHue=%23312f2f&height=410&id=pJhmQ&originHeight=410&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=1080)当程序执行完成之后，发现在项目的根目录出现了一个新的 log.txt 文件，打开此文件看到如下结果：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1616565023016-e19ebe76-3ccc-4344-bc5a-a85f9f292935.webp#averageHue=%233c3f44&height=589&id=SHohO&originHeight=589&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=1080)<br />从以上结果可以看出标准输出流和标准错误输出流是彼此独立执行的，且 JVM 为了高效的执行会让二者并行运行，所以最终看到的结果是 finally 在 catch 之前执行了。
<a name="GVQn4"></a>
### ③ 解决方案
知道了原因，那么问题就好处理，只需要将 try-catch-finally 中的输出对象，改为统一的输出流对象就可以解决此问题了。
<a name="UHYX3"></a>
### ④ 正例代码
```java
private static void execErr() {
    try {
        throw new RuntimeException();
    } catch (RuntimeException e) {
        System.out.println(e);
    } finally {
        System.out.println("执行 finally.");
    }
}
```
改成了统一的输出流对象之后，手工执行了 n 次，并没有发现任何问题。
<a name="daa1c"></a>
## 坑4：finally中的代码“不执行”
finally 中的代码一定会执行吗？如果是之前会毫不犹豫的说“是的”，但是现在可能会这样回答：**正常情况下 finally 中的代码一定会执行的，但如果遇到特殊情况 finally 中的代码就不一定会执行了**，比如下面这些情况：

- 在 try-catch 语句中执行了 `System.exit`；
- 在 try-catch 语句中出现了死循环；
- 在 finally 执行之前掉电或者 JVM 崩溃了。

如果发生了以上任意一种情况，finally 中的代码就不会执行了。**虽然感觉这一条有点“抬杠”的嫌疑，墨菲定律中，如果一件事有可能会发生，那么他就一定会发生。所以从严谨的角度来说，这个观点还是成立的**，尤其是对于新手来说，神不知鬼不觉的写出一个自己发现不了的死循环是一件很容易的事。
<a name="JbheK"></a>
### ① 反例代码
```java
public static void main(String[] args) {
    noFinally();
}
private static void noFinally() {
    try {
        System.out.println("我是 try~");
        System.exit(0);
    } catch (Exception e) {
        // do something
    } finally {
        System.out.println("我是 fially~");
    }
}
```
以上代码的执行结果如下：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1616565023089-2e1e1fcf-9c06-48c4-b30a-be5f1d6b7802.png#averageHue=%23353535&height=234&id=bG56Y&originHeight=234&originWidth=918&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=918)<br />从以上结果可以看出 finally 中的代码并没有执行。
<a name="JRh8V"></a>
### ② 解决方案
排除掉代码中的 `System.exit` 代码，除非是业务需要，但也**要注意如果在 try-cacth 中出现了 **`**System.exit**`** 的代码，那么 finally 中的代码将不会被执行。**
