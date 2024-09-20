class
<a name="Ymu30"></a>
### 1、字节码
计算机比较“傻”，只认 0 和 1，这意味着编写的代码最终都要编译成机器码才能被计算机执行。Java 在诞生之初就提出了一个非常著名的宣传口号: "**一次编写，处处运行**"。**Write Once, Run Anywhere.**<br />为了这个口号，Java 的母公司 Sun 以及其他虚拟机提供商发布了许多可以在不同平台上运行的 Java 虚拟机，而这些虚拟机都拥有一个共同的功能，那就是可以载入和执行同一种与平台无关的字节码（Byte Code）。<br />有了 Java 虚拟机的帮助，编写的 Java 源代码不必再根据不同平台编译成对应的机器码了，只需要生成一份字节码，然后再将字节码文件交由运行在不同平台上的 Java 虚拟机读取后执行就可以了。<br />如今的 Java 虚拟机非常强大，不仅支持 Java 语言，还支持很多其他的编程语言，比如说 Groovy、Scala、Koltin 等等。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618499816023-bf5f3360-738f-45cb-8a26-2b860e28c412.png#clientId=ub8591ed5-d11a-4&from=paste&id=u307ae9f6&originHeight=140&originWidth=140&originalType=url&status=done&style=shadow&taskId=u6138ede6-fd8d-48c3-8428-c7d3d5d6ee9)<br />来看一段代码吧。
```java
public class Main {
    private int age = 18;
    public int getAge() {
        return age;
    }
}
```
编译生成  Main.class  文件后，可以在命令行使用`xxd Main.class`打开 class 文件（这里用的是 Intellij IDEA，在 macOS 环境下）。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618499816131-1216dba0-258b-455d-98aa-6297dc94d101.png#clientId=ub8591ed5-d11a-4&from=paste&id=uea9cb9df&originHeight=140&originWidth=140&originalType=url&status=done&style=shadow&taskId=uce50bf6c-4bf2-4679-8c8c-f62ee20aaab)<br />对于这些 16 进制内容，除了开头的 cafe babe，剩下的内容大致可以翻译成：啥玩意啊这......<br />同学们别慌，就从"cafe babe"说起吧，这 4 个字节称之为`魔数`，也就是说，只有以"cafe babe"开头的 class 文件才能被 Java 虚拟机接受，这 4 个字节就是字节码文件的身份标识。<br />目光右移，0000 是 Java 的次版本号，0037 转化为十进制是 55，是主版本号，Java 的版本号从 45 开始，每升一个大版本，版本号加 1，大家可以启动福尔摩斯模式，推理一下。<br />再往后面就是字符串常量池。
<a name="dAfa5"></a>
### 2、反编译字节码文件
Java 内置了一个反编译命令 javap，可以通过`javap -help`了解 javap 的基本用法。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618499816142-3a7372d0-dd9d-4f11-a921-23dbf3cb8ebd.png#clientId=ub8591ed5-d11a-4&from=paste&id=u97c6efb3&originHeight=140&originWidth=140&originalType=url&status=done&style=shadow&taskId=ud6c4fec8-d8af-4024-9a9a-a76baf2cc76)<br />输入命令`javap -v -p Main.class`来查看一下输出的内容。
```java
Classfile /Users/maweiqing/Documents/GitHub/TechSisterLearnJava/codes/TechSister/target/classes/com/itwanger/jvm/Main.class
  Last modified 2021年4月15日; size 385 bytes
  SHA-256 checksum 6688843e4f70ae8d83040dc7c8e2dd3694bf10ba7c518a6ea9b88b318a8967c6
  Compiled from "Main.java"
public class com.itwanger.jvm.Main
  minor version: 0
  major version: 55
  flags: (0x0021) ACC_PUBLIC, ACC_SUPER
  this_class: #3                          // com/itwanger/jvm/Main
  super_class: #4                         // java/lang/Object
  interfaces: 0, fields: 1, methods: 2, attributes: 1
Constant pool:
   #1 = Methodref          #4.#18         // java/lang/Object."<init>":()V
   #2 = Fieldref           #3.#19         // com/itwanger/jvm/Main.age:I
   #3 = Class              #20            // com/itwanger/jvm/Main
   #4 = Class              #21            // java/lang/Object
   #5 = Utf8               age
   #6 = Utf8               I
   #7 = Utf8               <init>
   #8 = Utf8               ()V
   #9 = Utf8               Code
  #10 = Utf8               LineNumberTable
  #11 = Utf8               LocalVariableTable
  #12 = Utf8               this
  #13 = Utf8               Lcom/itwanger/jvm/Main;
  #14 = Utf8               getAge
  #15 = Utf8               ()I
  #16 = Utf8               SourceFile
  #17 = Utf8               Main.java
  #18 = NameAndType        #7:#8          // "<init>":()V
  #19 = NameAndType        #5:#6          // age:I
  #20 = Utf8               com/itwanger/jvm/Main
  #21 = Utf8               java/lang/Object
{
  private int age;
    descriptor: I
    flags: (0x0002) ACC_PRIVATE

  public com.itwanger.jvm.Main();
    descriptor: ()V
    flags: (0x0001) ACC_PUBLIC
    Code:
      stack=2, locals=1, args_size=1
         0: aload_0
         1: invokespecial #1                  // Method java/lang/Object."<init>":()V
         4: aload_0
         5: bipush        18
         7: putfield      #2                  // Field age:I
        10: return
      LineNumberTable:
        line 6: 0
        line 7: 4
      LocalVariableTable:
        Start  Length  Slot  Name   Signature
            0      11     0  this   Lcom/itwanger/jvm/Main;

  public int getAge();
    descriptor: ()I
    flags: (0x0001) ACC_PUBLIC
    Code:
      stack=1, locals=1, args_size=1
         0: aload_0
         1: getfield      #2                  // Field age:I
         4: ireturn
      LineNumberTable:
        line 9: 0
      LocalVariableTable:
        Start  Length  Slot  Name   Signature
            0       5     0  this   Lcom/itwanger/jvm/Main;
}
SourceFile: "Main.java"
```
感觉内容挺多的。来一行一行分析。<br />第 1 行：
```java
Classfile /Users/maweiqing/Documents/GitHub/TechSisterLearnJava/codes/TechSister/target/classes/com/itwanger/jvm/Main.class
```
字节码文件的位置。<br />第 2 行：
```java
Last modified 2021年4月15日; size 385 bytes
```
字节码文件的修改日期、文件大小。<br />第 3 行：
```java
SHA-256 checksum 6688843e4f70ae8d83040dc7c8e2dd3694bf10ba7c518a6ea9b88b318a8967c
```
字节码文件的 SHA-256 值。<br />第 4 行：
```java
Compiled from "Main.java"
```
说明该字节码文件编译自 Main.java 源文件。<br />第 5 行：
```java
public class com.itwanger.jvm.Main
```
字节码文件的类全名。<br />第 6 行minor version: 0，次版本号。<br />第 7 行major version: 55，主版本号。<br />第 8 行：
```java
flags: (0x0021) ACC_PUBLIC, ACC_SUPER
```
类访问标记，一共有 8 种。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618499816011-79d3c9e9-1460-4e06-ab45-41ce58f6c6b2.png#clientId=ub8591ed5-d11a-4&from=paste&id=u7007849d&originHeight=140&originWidth=140&originalType=url&status=done&style=shadow&taskId=ucdf0dbcc-81d6-4c88-a668-de078349400)<br />表明当前类是`ACC_PUBLIC | ACC_SUPER`。位运算符`|`的意思是如果相对应位是 0，则结果为 0，否则为 1，所以`0x0001 | 0x0020`的结果是`0x0021`（需要转成二进制进行运算）。<br />第 9 行：
```java
this_class: #3                          // com/itwanger/jvm/Main
```
当前类的索引，指向常量池中下标为 3 的常量，可以看得出当前类是 Main 类。<br />第 10 行：
```java
super_class: #4                         // java/lang/Object
```
父类的索引，指向常量池中下标为 6 的常量，可以看得出当前类的父类是 Object 类。<br />第 11 行：
```java
interfaces: 0, fields: 1, methods: 2, attributes: 1
```
当前类有 0 个接口，1 个字段（age），2 个方法（`write()`方法和缺省的默认构造方法），1 个属性（该类仅有的一个属性是 SourceFIle，包含了源码文件的信息）。
<a name="FfTpu"></a>
### 3、常量池
接下来是 Constant pool，也就是字节码文件最重要的常量池部分。可以把常量池理解为字节码文件中的资源仓库，主要存放两大类信息。<br />1）字面量（Literal），有点类似 Java 中的常量概念，比如文本字符串，final 常量等。<br />2）符号引用（Symbolic References），属于编译原理方面的概念，包括 3 种：

- 类和接口的全限定名（Fully Qualified Name）
- 字段的名称和描述符（Descriptor）
- 方法的名称和描述符

Java 虚拟机是在加载字节码文件的时候才进行的动态链接，也就是说，字段和方法的符号引用只有经过运行期转换后才能获得真正的内存地址。当 Java 虚拟机运行时，需要从常量池获取对应的符号引用，然后在类创建或者运行时解析并翻译到具体的内存地址上。<br />当前字节码文件中一共有 21 个常量，它们之间是有链接的，逐个分析会比较乱，采用顺藤摸瓜的方式，从上依次往下看，那些被链接的常量就点到为止。<br />_注_：

- `#`号后面跟的是索引，索引没有从 0 开始而是从 1 开始，是因为设计者考虑到，“如果要表达不引用任何一个常量的含义时，可以将索引值设为 0 来表示”（《深入理解 Java 虚拟机》描述的）。
- `=`号后面跟的是常量的类型，没有包含前缀`CONSTANT_`和后缀`_info`。
- 全文中提到的索引等同于下标，为了灵活描述，没有做统一。

---

第 1 个常量：
```java
#1 = Methodref          #4.#18         // java/lang/Object."<init>":()V
```
类型为 Methodref，表明是用来定义方法的，指向常量池中下标为 4 和 18 的常量。<br />第 4 个常量：
```java
#4 = Class              #21            // java/lang/Object
```
类型为 Class，表明是用来定义类（或者接口）的，指向常量池中下标为 21 的常量。<br />第 21 个常量：
```java
#21 = Utf8               java/lang/Object
```
类型为 Utf8，UTF-8 编码的字符串，值为java/lang/Object。<br />第 18 个常量：
```java
#18 = NameAndType        #7:#8          // "<init>":()V
```
类型为 NameAndType，表明是字段或者方法的部分符号引用，指向常量池中下标为 7 和 8 的常量。<br />第 7 个常量：
```java
#7 = Utf8               <init>
```
类型为 Utf8，UTF-8 编码的字符串，值为`<init>`，表明为构造方法。<br />第 8 个常量：
```java
#8 = Utf8               ()V
```
类型为 Utf8，UTF-8 编码的字符串，值为`()V`，表明方法的返回值为 void。<br />到此为止，第 1 个常量算是摸完了。组合起来的意思就是，Main 类使用的是默认的构造方法，来源于 Object 类。

---

第 2 个常量：
```java
#2 = Fieldref           #3.#19         // com/itwanger/jvm/Main.age:I
```
类型为 Fieldref，表明是用来定义字段的，指向常量池中下标为 3 和 19 的常量。<br />第 3 个常量：
```java
#3 = Class              #20            // com/itwanger/jvm/Main
```
类型为 Class，表明是用来定义类（或者接口）的，指向常量池中下标为 20 的常量。<br />第 19 个常量：
```java
#19 = NameAndType        #5:#6          // age:I
```
类型为 NameAndType，表明是字段或者方法的部分符号引用，指向常量池中下标为 5 和 6 的常量。<br />第 5 个常量：
```java
#5 = Utf8               age
```
类型为 Utf8，UTF-8 编码的字符串，值为`age`，表明字段名为 age。<br />第 6 个常量：
```java
#6 = Utf8               I
```
类型为 Utf8，UTF-8 编码的字符串，值为`I`，表明字段的类型为 `int`。<br />关于字段类型的描述符映射表如下图所示。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618499816199-834983f0-3003-4b4d-8076-ed406846605f.png#clientId=ub8591ed5-d11a-4&from=paste&id=u75431e4b&originHeight=140&originWidth=140&originalType=url&status=done&style=shadow&taskId=u7024dcb4-9417-4dc5-9856-19d0eef0186)<br />到此为止，第 2 个常量算是摸完了。组合起来的意思就是，声明了一个类型为 int 的字段 age。

---

<a name="zQioU"></a>
### 4、字段表集合
字段表用来描述接口或者类中声明的变量，包括类变量和成员变量，但不包含声明在方法中局部变量。<br />字段的修饰符一般有：

- 访问权限修饰符，比如 `public private protected`
- 静态变量修饰符，比如 `static`
- `final` 修饰符
- 并发可见性修饰符，比如 `volatile`
- 序列化修饰符，比如 `transient`

然后是字段的类型（可以是基本数据类型、数组和对象）和名称。<br />在 Main.class 字节码文件中，字段表的信息如下所示。
```java
private int age;
    descriptor: I
    flags: (0x0002) ACC_PRIVATE
```
表明字段的访问权限修饰符为 private，类型为 int，名称为 age。<br />字段的访问标志和类的访问标志非常类似。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618499816921-b79289ca-c142-4ad8-8f2a-7c4c7dc4c1b6.png#clientId=ub8591ed5-d11a-4&from=paste&id=ub7c24ff5&originHeight=140&originWidth=140&originalType=url&status=done&style=shadow&taskId=u0dc7aefe-9e1a-4749-80ab-2a08b5ca117)
<a name="yZxWD"></a>
### 5、方法表集合
方法表用来描述接口或者类中声明的方法，包括类方法和成员方法，以及构造方法。方法的修饰符和字段略有不同，比如说 `volatile `和 transient 不能用来修饰方法，再比如说方法的修饰符多了 `synchronized`、`native`、`strictfp `和 `abstract`。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618499816887-80855eef-29fa-4dd4-9142-706b0bdcc94d.png#clientId=ub8591ed5-d11a-4&from=paste&id=u533b4098&originHeight=140&originWidth=140&originalType=url&status=done&style=shadow&taskId=u459c50d5-6673-4b99-a605-7b2bea668ec)<br />下面这部分为构造方法，返回类型为 `void`，访问标志为 `public`。
```java
public com.itwanger.jvm.Main();
	descriptor: ()V
    flags: (0x0001) ACC_PUBLIC
```
来详细看一下其中 Code 属性。
```java
    Code:
      stack=2, locals=1, args_size=1
         0: aload_0
         1: invokespecial #1                  // Method java/lang/Object."<init>":()V
         4: aload_0
         5: bipush        18
         7: putfield      #2                  // Field age:I
        10: return
      LineNumberTable:
        line 6: 0
        line 7: 4
      LocalVariableTable:
        Start  Length  Slot  Name   Signature
            0      11     0  this   Lcom/itwanger/jvm/Main;
```

- `stack` 为最大操作数栈，Java 虚拟机在运行的时候会根据这个值来分配栈帧的操作数栈深度。
- `locals` 为局部变量所需要的存储空间，单位为槽（slot），方法的参数变量和方法内的局部变量都会存储在局部变量表中。
- `args_size` 为方法的参数个数。

为什么 stack 的值为 2，`locals` 的值为 1，`args_size` 的值为 1 呢？默认的构造方法不是没有参数和局部变量吗？<br />这是因为有一个隐藏的 this 变量，只要不是静态方法，都会有一个当前类的对象 this 悄悄的存在着。这就解释了为什么 `locals` 和 `args_size` 的值为 1 的问题。那为什么 stack 的值为 2 呢？因为字节码指令 invokespecial（调用父类的构造方法进行初始化）会消耗掉一个当前类的引用，所以 aload_0 执行了 2 次，也就意味着操作数栈的大小为 2。<br />关于字节码指令，后面再详细介绍。

- LineNumberTable，该属性的作用是描述源码行号与字节码行号(字节码偏移量)之间的对应关系。
- LocalVariableTable，该属性的作用是描述帧栈中的局部变量与源码中定义的变量之间的关系。大家仔细看一下，就能看到 this 的影子了。

下面这部分为成员方法`getAge()`，返回类型为 `int`，访问标志为 `public`。
```java
public int getAge();
	descriptor: ()I
    flags: (0x0001) ACC_PUBLIC
```
理解了构造方法的 Code 属性后，再看  `getAge()`方法的 Code 属性时，就很容易理解了。
```java
   Code:
      stack=1, locals=1, args_size=1
         0: aload_0
         1: getfield      #2                  // Field age:I
         4: ireturn
      LineNumberTable:
        line 9: 0
      LocalVariableTable:
        Start  Length  Slot  Name   Signature
            0       5     0  this   Lcom/itwanger/jvm/Main;
```
最大操作数栈为 1，局部变量所需要的存储空间为 1，方法的参数个数为 1，是因为局部变量只有一个隐藏的 this，并且字节码指令中只执行了一次 aload_0。
