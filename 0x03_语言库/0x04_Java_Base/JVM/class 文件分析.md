Java <br />Java 是跨平台的，JVM 作为中间层，自然要针对不同的操作系统提供不同的实现。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617805039847-bdad87cb-e484-41b8-92be-f37789012fb4.png#align=left&display=inline&height=765&originHeight=765&originWidth=1080&size=0&status=done&style=shadow&width=1080)<br />通过不同操作系统的 JVM，源代码就可以不用根据不同的操作系统编译成不同的二进制可执行文件了，跨平台的目标也就实现了。那这个 class 文件到底是什么玩意呢？它是怎么被 JVM 识别的呢？<br />编写一段简单的 Java 代码，文件名为 Hello.java。
```java
package com.fcant.jvm;
class Hello {
    public static void main(String[] args) {
        System.out.println("Hello!");
    }
}
```
点击编译按钮后，IDEA 会自动生成一个名为 Hello.class 的文件，在 `target/classes` 的对应包目录下。直接双击打开后长下面这样子：
```java
//
// Source code recreated from a .class file by IntelliJ IDEA
// (powered by Fernflower decompiler)
//
package com.fcant.jvm;
class Hello {
    Hello() {
    }
    public static void main(String[] args) {
        System.out.println("Hello!");
    }
}
```
看起来和源代码很像，只是多了一个空的构造方法，它是 class 文件被 IDEA 自带的反编译工具 Fernflower 反编译后的样子。那真实的 class 文件长什么样子呢？<br />可以在 terminal 面板下用 `xxd Hello.class` 命令来查看。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617805040013-ca5132c2-2e5b-4b84-8366-aa45e50785c9.png#align=left&display=inline&height=1134&originHeight=1134&originWidth=1080&size=0&status=done&style=shadow&width=1080)<br />它是 class 文件的一种十六进制形式，`xxd` 这个命令的神奇之处就是它能将一个给定文件转换成十六进制形式。
<a name="qRrb5"></a>
### 1、魔数
第一行中有一串特殊的字符 `cafebabe`，它就是一个魔数，是 JVM 识别 class 文件的标志，JVM 会在验证阶段检查 class 文件是否以该魔数开头，如果不是则会抛出 `ClassFormatError`。<br />魔数 `cafebabe` 的中文意思显而易见——咖啡，再加上 Java 的图标本来就是一个热气腾腾的咖啡，可见 Java 与咖啡的渊源有多深。
<a name="FKuPW"></a>
### 2、版本号
紧跟着魔数后面的四个字节 `0000 0037` 分别表示副版本号和主版本号。也就是说，主版本号为 55（0x37 的十进制），也就是 Java 11 对应的版本号，副版本号为 0。<br />上一个 LTS 版本是 Java 8，对应的主版本号为 52，也就是说 Java 9 是 53，Java 10 是 54，只不过 Java 9 和 Java 10 都是过渡版本，下一个 LTS 版本是 Java 17，预计 2021 年 9 月份推出。
<a name="okrde"></a>
### 3、常量池
紧跟在版本号之后的是常量池，字符串常量和较大的整数都会存储在常量池中，当使用这些数值时，会根据常量池中的索引来查找。<br />Java 定义了 boolean、byte、short、char 和 int 等基本数据类型，它们在常量池中都会被当做 int 来处理。来通过一段简单的 Java 代码了解下。
```java
public class ConstantTest {
    public final boolean bool = true;
    public final char aChar = 'a';
    public final byte b = 66;
    public final short s = 67;
    public final int i = 68;
}
```
布尔值 true 的十六进制是 0x01、字符 a 的十六进制是 0x61，字节 66 的十六进制是 0x42，短整型 67 的十六进制是 0x43，整形 68 的十六进制是 0x44。所以编译生成的整形常量在 class 文件中的位置如下图所示。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617805040135-d7dda664-0ada-480b-ab69-ee73361b58e0.png#align=left&display=inline&height=573&originHeight=573&originWidth=1080&size=0&status=done&style=shadow&width=1080)<br />第一个字节 0x03 表示常量的类型为 _CONSTANT_Integer_info_，是 JVM 中定义的 14 种常量类型之一，对应的还有 _CONSTANT_Float_info_、_CONSTANT_Long_info_、_CONSTANT_Double_info_，对应的标识分别是 0x04、0x05、0x06。<br />对于 int 和 float 来说，它们占 4 个字节；对于 long 和 double 来说，它们占 8 个字节。来个 long 型的最大值观察下。
```java
public class ConstantTest {
    public final long ong = Long.MAX_VALUE;
}
```
来看一下它在 class 文件中的位置。05 开头，7f ff ff ff ff ff ff ff 结尾，果然占 8 个字节，以前知道 long 型会占 8 个字节，但没有直观的感受，现在有了。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617805040156-2153975b-32ca-434e-9b19-f693ea316cc3.png#align=left&display=inline&height=289&originHeight=289&originWidth=1080&size=0&status=done&style=shadow&width=1080)<br />接下来，再来看一段代码。
```java
class Hello {
    public final String s = "hello";
}
```
“hello”是一个字符串，它的十六进制为 `68 65 6c 6c 6f`，来看一下它在 class 文件中的位置。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617805040248-74e02ba5-34d7-4608-bb15-5e796982d342.png#align=left&display=inline&height=850&originHeight=850&originWidth=1080&size=0&status=done&style=shadow&width=1080)<br />前面还有 3 个字节，第一个字节 0x01 是标识，标识类型为 _CONSTANT_Uft8_info_，第二个和第三个自己 0x00 0x05 用来表示第三部分字节数组的长度。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617805040373-2fa09046-2f72-44c2-8d36-9ad8a554b930.png#align=left&display=inline&height=327&originHeight=327&originWidth=1080&size=0&status=done&style=shadow&width=1080)<br />与 _CONSTANT_Uft8_info_ 类型对应的，还有一个 _CONSTANT_String_info_，用来表示字符串对象（之前代码中的 s），标识是 0x08。前者存储了字符串真正的值，后者并不包含字符串的内容，仅仅包含了一个指向常量池中 _CONSTANT_Uft8_info_ 的索引。来看一下它在 class 文件中的位置。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617805040280-6ace88aa-5d14-4da5-b017-41feb098fb5d.png#align=left&display=inline&height=877&originHeight=877&originWidth=1080&size=0&status=done&style=shadow&width=1080)<br />_CONSTANT_String_info_ 通过索引 19 来找到 _CONSTANT_Uft8_info_。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617805040680-8f5be924-ff8a-4402-b71c-e1d02b72e40e.png#align=left&display=inline&height=238&originHeight=238&originWidth=1080&size=0&status=done&style=shadow&width=1080)<br />除此之外，还有 _CONSTANT_Class_info_，用来表示类和接口，结构和 _CONSTANT_String_info_ 类似，第一个字节是标识，值为 0x07，后面两个字节是常量池索引，指向 _CONSTANT_Utf8_info_——字符串存储的是类或者接口的全路径限定名。<br />拿 Hello.java 类来说，它的全路径限定名为 `com/itwanger/jvm/Hello`，对应的十六进制为“636f6d2f697477616e6765722f6a766d2f48656c6c6f”，是一串 _CONSTANT_Uft8_info_，指向它的 _CONSTANT_Class_info_ 在 class 文件中的什么位置呢？<br />这里介绍一款可视化字节码的工具 jclasslib bytecode viewer，可以直接在 IDEA 的插件市场安装。安装完成后，选中 class 文件，然后在 View 菜单里找到 Show Bytecode With Jclasslib 子菜单，就可以查看 class 文件的关键信息了。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617805040676-9cb93b65-9c58-494a-a067-f2411bc88dc5.png#align=left&display=inline&height=720&originHeight=720&originWidth=1080&size=0&status=done&style=shadow&width=1080)<br />从上图中可以看到，常量池的总大小为 23，索引为 04 的 _CONSTANT_Class_info_ 指向的是是索引为 21 的 _CONSTANT_Uft8_info_，值为 `com/itwanger/jvm/Hello`。21 的十六进制为 0x15，有了这个信息，就可以找到 _CONSTANT_Class_info_ 在 class 文件中的位置了。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617805040673-e32cd89e-1e8a-4d7b-9385-8a621203d911.png#align=left&display=inline&height=830&originHeight=830&originWidth=1080&size=0&status=done&style=shadow&width=1080)<br />0x07 是第一个字节，_CONSTANT_Class_info_ 的标识符，然后是两个字节，标识索引。<br />还有 _CONSTANT_NameAndType_info_，用来标识字段或方法，标识符为 12，对应的十六进制是 0x0c。后面还有 4 个字节，前两个是字段或者方法的索引，后两个是字段或方法的描述符，也就是字段或者方法的类型。<br />来看下面这段代码。
```java
class Hello {
    public void testMethod(int id, String name) {
    }
}
```
用 jclasslib 可以看到 _CONSTANT_NameAndType_info_ 包含的索引有两个。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617805040876-879ba860-f5b6-43fd-a749-bec39391229a.png#align=left&display=inline&height=806&originHeight=806&originWidth=1080&size=0&status=done&style=shadow&width=1080)<br />一个是 4，一个是 5，可以通过下图来表示 _CONSTANT_NameAndType_info_ 的构成。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617805040881-f226efe0-c5e7-41f4-a36c-45bacd437e64.png#align=left&display=inline&height=312&originHeight=312&originWidth=884&size=0&status=done&style=shadow&width=884)<br />对应 class 文件中的位置如下图所示。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617805040866-bb8e2a26-ac9e-4503-b95d-420446b12e18.png#align=left&display=inline&height=983&originHeight=983&originWidth=1080&size=0&status=done&style=shadow&width=1080)<br />接下来是 _CONSTANT_Fieldref_info_ 、_CONSTANT_Methodref_info_ 和 _CONSTANT_InterfaceMethodref_info_，它们三个的结构比较类似，可以通过下面的伪代码来表示。
```c
CONSTANT_*ref_info {
  u1 tag;
  u2 class_index;
  u2 name_and_type_index;
}
```
学过 C 语言的符号表（Symbol Table）的话，对这段伪代码并不会陌生。

- tag 为标识符，Fieldref 的为 9，也就是十六进制的 0x09；Methodref 的为 10，也就是十六进制的 0x0a；InterfaceMethodref 的为 11， 也就是十六进制的 0x0b。
- class_index 为 _CONSTANT_Class_info_ 的常量池索引，表示字段 | 方法 | 接口方法所在的类信息。
- name_and_type_index 为 _CONSTANT_NameAndType_info_ 的常量池索引，拿 Fieldref 来说，表示字段名和字段类型；拿 Methodref 来说，表示方法名、方法的参数和返回值类型；拿 InterfaceMethodref 来说，表示接口方法名、接口方法的参数和返回值类型。

还有 _CONSTANT_MethodHandle_info_ 、_CONSTANT_MethodType_info_ 和 _CONSTANT_InvokeDynamic_info_。
<a name="J8Cju"></a>
### 4、访问标记
紧跟着常量池之后的区域就是访问标记（Access flags），这个标记用于识别类或接口的访问信息，比如说到底是 class 还是 interface？是 public 吗？是 abstract 抽象类吗？是 final 类吗？等等。总共有 16 个标记位可供使用，但常用的只有其中 7 个。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617805041213-d29b0058-92d7-4036-9e14-f097ad21c32d.png#align=left&display=inline&height=422&originHeight=422&originWidth=1080&size=0&status=done&style=shadow&width=1080)<br />来看一个简单的枚举代码。
```java
public enum Color {
    RED,GREEN,BLUE;
}
```
通过 jclasslib 可以看到访问标记的信息有 `0x4031 [public final enum]`。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617805041242-9156e9d5-0602-4b8b-a96a-ba0d1c95b60c.png#align=left&display=inline&height=931&originHeight=931&originWidth=1080&size=0&status=done&style=shadow&width=1080)<br />对应 class 文件中的位置如下图所示。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617805041353-74d0a93e-c0cb-47eb-97d8-197a846e04d3.png#align=left&display=inline&height=619&originHeight=619&originWidth=1080&size=0&status=done&style=shadow&width=1080)
<a name="jJDxr"></a>
### 5、this_class、super_class、interfaces
这三部分用来确定类的继承关系，this_class 为当前类的索引，super_class 为父类的索引，interfaces 为接口。<br />来看下面这段简单的代码，没有接口，默认继承 Object 类。
```java
class Hello {
    public static void main(String[] args) {
        
    }
}
```
通过 jclasslib 可以看到类的继承关系。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617805041394-d0a8e510-10c9-455e-8568-66d549ecf522.png#align=left&display=inline&height=510&originHeight=510&originWidth=1080&size=0&status=done&style=shadow&width=1080)

- this_class 指向常量池中索引为 2 的 _CONSTANT_Class_info_。
- super_class 指向常量池中索引为 3 的 _CONSTANT_Class_info_。
- 由于没有接口，所以 interfaces 的信息为空。

对应 class 文件中的位置如下图所示。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617805041407-0a82af75-09d3-4ac3-8fc7-21ebb244e420.png#align=left&display=inline&height=257&originHeight=257&originWidth=1080&size=0&status=done&style=shadow&width=1080)
<a name="G93mR"></a>
### 6、字段表
一个类中定义的字段会被存储在字段表（fields）中，包括静态的和非静态的。<br />来看这样一段代码。
```java
public class FieldsTest {
    private String name;
}
```
字段只有一个，修饰符为 private，类型为 String，字段名为 name。可以用下面的伪代码来表示 field 的结构。
```c
field_info {
  u2 access_flag;
  u2 name_index;
  u2 description_index;
}
```

- access_flag 为字段的访问标记，比如说是不是 public | private | protected，是不是 static，是不是 final 等。
- name_index 为字段名的索引，指向常量池中的 _CONSTANT_Utf8_info_， 比如说上例中的值就为 name。
- description_index 为字段的描述类型索引，也指向常量池中的 _CONSTANT_Utf8_info_，针对不同的数据类型，会有不同规则的描述信息。

1）对于基本数据类型来说，使用一个字符来表示，比如说 I 对应的是 int，B 对应的是 byte。<br />2）对于引用数据类型来说，使用 `L***;` 的方式来表示，`L` 开头，`;` 结束，比如字符串类型为 `Ljava/lang/String;`。<br />3）对于数组来说，会用一个前置的 `[` 来表示，比如说字符串数组为 `[Ljava/lang/String;`。<br />对应到 class 文件中的位置如下图所示。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617805041495-f8106ba0-51ea-4c45-b8d8-269cea200e45.png#align=left&display=inline&height=810&originHeight=810&originWidth=1080&size=0&status=done&style=shadow&width=1080)
<a name="COBxk"></a>
### 7、方法表
方法表和字段表类似，区别是用来存储方法的信息，包括方法名，方法的参数，方法的签名。<br />就拿 main 方法来说吧。
```java
public class MethodsTest {
    public static void main(String[] args) {
        
    }
}
```
先用 jclasslib 看一下大概的信息。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617805041766-257880cd-0e41-4db2-bd56-5cc8a2f8ceb1.png#align=left&display=inline&height=563&originHeight=563&originWidth=1080&size=0&status=done&style=shadow&width=1080)

- 访问标记是 public static 的。
- 方法名为 main。
- 方法的参数为字符串数组；返回类型为 Void。

对应到 class 文件中的位置如下图所示。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617805041862-0519e8b7-ec5b-4a40-82a2-ba872a3e0cb4.png#align=left&display=inline&height=292&originHeight=292&originWidth=1080&size=0&status=done&style=shadow&width=1080)
<a name="JjAKB"></a>
### 8、属性表
属性表是 class 文件中的最后一部分，通常出现在字段和方法中。<br />来看这样一段代码。
```java
public class AttributeTest {
    public static final int DEFAULT_SIZE = 128;
}
```
只有一个常量 DEFAULT_SIZE，它属于字段中的一种，就是加了 final 的静态变量。先通过 jclasslib 看一下它当中一个很重要的属性——ConstantValue，用来表示静态变量的初始值。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617805041948-cc3b593a-3ebf-444f-b845-487a06a9676e.png#align=left&display=inline&height=986&originHeight=986&originWidth=1080&size=0&status=done&style=shadow&width=1080)

- Attribute name index 指向常量池中值为“ConstantValue”的常量。
- Attribute length 的值为固定的 2，因为索引只占两个字节的大小。
- Constant value index 指向常量池中具体的常量，如果常量类型为 int，指向的就是 _CONSTANT_Integer_info_。

下图可以完整的表示字段的结构，包含属性表在内。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617805041921-0d68d5d8-9639-49bc-86c8-e5c61ca3ca82.png#align=left&display=inline&height=470&originHeight=470&originWidth=1080&size=0&status=done&style=shadow&width=1080)<br />对应到 class 文件中的位置如下图所示。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617805041855-1c399c2c-e107-404d-b550-d33e1a080f89.png#align=left&display=inline&height=883&originHeight=883&originWidth=1080&size=0&status=done&style=shadow&width=1080)<br />来看下面这段代码。
```java
public class MethodCode {
    public static void main(String[] args) {
        foo();
    }
    private static void foo() {
    }
}
```
main 方法中调用了 foo 方法。通过 jclasslib 看一下它当中一个很重要的属性——Code， 方法的关键信息都存储在里面。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617805041916-7e572855-fa46-458b-8ea5-a59994d5c4e4.png#align=left&display=inline&height=1509&originHeight=1509&originWidth=1080&size=0&status=done&style=shadow&width=1080)

- Attribute name index 指向常量池中值为“Code”的常量。
- Attribute length 为属性值的长度大小。
- bytecode 存储真正的字节码指令。
- exception table 表示方法内部的异常信息。
- maximum stack size 表示操作数栈的最大深度，方法执行的任意期间操作数栈深度都不会超过这个值。
- maximum local variable 表示临时变量表的大小，注意，并不等于方法中所有临时变量的数量之和，当一个作用域结束，内部的临时变量占用的位置就会被替换掉。
- code length 表示字节码指令的长度。

对应 class 文件中的位置如下图所示。<br />![640.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1617805070471-cc0cc3bc-5566-4be7-ab0e-ac5bfacb371b.png#align=left&display=inline&height=434&originHeight=434&originWidth=1080&size=260473&status=done&style=shadow&width=1080)
