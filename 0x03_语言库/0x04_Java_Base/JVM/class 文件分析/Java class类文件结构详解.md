Java
<a name="HEbq6"></a>
## 类文件结构：
在开始解析类文件前，需要先对class文件的大致结构做一个初步了解，后续的解析也将会根据这个结构分成几部分来解析。

- 校验信息：
   - 魔数：4字节(固定值0xcafe babe)
   - 次版本号：2字节
   - 主版本号：2字节，由十进制的45开始。
- 常量池：
   - 常量池计数(`constant_pool_count`)：2字节，表示常量池中常量的个数，其中下标为0的常量并不会出现在常量池中，也就是说常量池中的索引实际上是从1开始的，之所以要将下标为0的常量预留是为了满足常量池中某一常量不引用任何常量的情况(Java语言中是否有这种情况还需要确认)。
   - 常量池(`constant_pool`)：每个常量的数据结构并不是固定的，由常量的类型决定，但是每个常量的第一个字节决定了常量的类型。
- 类信息：
   - 访问标志(`access_flags`)：2字节，表示类和接口的访问控制
   - 类信息(`this_class`)：2字节
   - 父类信息(`super_class`)：2字节
   - 接口信息，包括接口个数(`interfaces_count`, 2字节)和接口信息(`interfaces` 一个`interface`是2字节，总共`interfaces_counts * 2`个字节)
- 字段信息：
   - 字段个数(`fields_count`)：2字节，表示之后有几个`field_info`的结构
   - 字段表(`field_info`)：字段表，是一个表结构，长度不固定，结构稍复杂
- 方法信息：
   - 方法个数(`methods_count`)：2字节，表示之后有几个(`method_info`)方法表结构
   - 方法表(`methods_info`)：方法表，长度不固定，可能是最复杂的一个结构了。
- 属性表信息：
   - 属性表个数(`attributes_count`)：2字节，表示之后有几个属性表结构
   - 属性表(`attribute_info`)属性表信息，表结构，长度不固定。属性表信息可能会同样嵌套在字段表，方法表的表结构中。
<a name="w19wp"></a>
## 对Class文件解析前的准备
对Class文件的解析首先需要写一个类用作测试，然后编译该类生成class文件，在针对该class文件进行解析。<br />为了更好的对比解析过程的正确性，可以通过`javap -p -verbose`命令先对class文件反编译，输出结果。
<a name="PIaUN"></a>
### Java代码：
```java
package com.insanexs.mess.javap;

public class JavapTest {

    protected static final String VAR_CONSTANT = "CONSTANT";

    private volatile int intField;

    private int[] intArraysField;

    private String strField;

    public JavapTest(){

    }

    public void publicMethod(){

    }

    protected String protectedReturnStrMethod(){
        return strField;
    }

    private synchronized void privateSynchronizedMethod(int intArgs){
        intField = intArgs;
    }
}
```
编译生成的class文件如下(已经按各部分拆开)：<br />![2021-10-30-14-11-20-021984.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1635574309756-c75ed97e-bde5-43ac-a44c-224d7c545dec.png#clientId=u3a2e40c6-b8be-4&from=ui&id=u0e6a5bd0&originHeight=1000&originWidth=594&originalType=binary&ratio=1&size=1785958&status=done&style=none&taskId=u696afcc9-b11c-4416-9382-02451d8a43b)<br />classfile 16进制显示
<a name="dT0fn"></a>
## 开始分析
<a name="SOFZE"></a>
### 校验信息
<a name="lS37q"></a>
#### 魔数
魔数是class文件的前四个字节，固定为0xcafe babe。用途是判断文件是否正确。
<a name="vSDvj"></a>
#### 主次版本号
0x0000 0034 其中前两个字节表示次版本号，后两个字节表示主版本号。0x34转成十进制为52，52-45+1 = 8(第一个主版本号从45开始)，因此推断出Java版本为JDK 8。主次版本号用来校验字节码和JVM是否匹配，JVM的版本需要大于等于class文件的版本号。
<a name="zL7vp"></a>
### 常量池
常量池中存量了两种类型的常量：字面量和符号引用。<br />字面量可以理解为常量的值(无法修改的值)。<br />符号引用则包括三种类型：类或接口的全限定名，字段的名称和描述符，方法的名称和描述符(后两种情况其实是很对称的，一个针对字段另一个针对方法)。
<a name="wBCd1"></a>
#### 常量池计数constant_pool_count
校验信息后，接下来的两个字节表示常量池中常量的个数，这里是0x0024，转成十进制为36。说明常量池中共有36个常量。<br />但是由于下标为0的常量是由JVM故意空出的，不会显示出现在字节码中，因此实际常量池的常量从1开始，直到35。
<a name="dQlnb"></a>
#### 常量池
已经知道了常量池共有36个常量(实际只有35个)，但由于不同类型的常量结构并不是固定的，无法通过常量个数直接推出之后多少字节属于常量池的内容。<br />因此，只能逐个解析常量池中的常量，直到解析出常量的个数达到`constant_pool_count - 1`，常量池才算解析完成。<br />由于篇幅限制，本文不会解析全部常量池，只会解析其中一些常量用作示范，读者可根据示范自行完成剩下的常量解析。<br />上面已经说了不同类型的常量其结构也是不同的，但是所有常量的第一个字节都是标志位，因此解析常量池中的常量的方式是解析一个字节，根据该字节确定常量的类型，在查找对应的结构完成剩余部分的解析。
<a name="xSXBH"></a>
###### 譬如，这里的第一个常量#1：
第一个字节：0x0a，对比常量池的类型后发现是一个`CONSTANT_Methodref_info`(类中方法的符号引用)。<br />在查找该类型的结构，应该是：
```java
CONSTANT_Methodref_info {
    u1 tag;
    u2 class_index;
    u2 name_and_type_index;
}
```
应该是一个字节的标识位(tag)，2个字节的类名称索引(`class_index`)和2个字节的名称和类型描述符(`name_and_type_index`)，共5个字节。<br />因此常量池#1对应的字节分别是:0a 0005 001e(含已经分析过的0a)。其中`class_index` 为0x0005，`name_and_type_index`为0x001e。这两个index的值均是指向常量池的其他常量的，转成十进制分别是指向#5和#30。<br />这样就解析完了常量池中的第一个常量。
```java
//常量#1
0a //tag 表示类型为Methodref_info(类中方法) 其结构为(U1 flag;U2 index(指向类描述符); U2 index(指向名称和类型描述符)) 
0005 //指向常量池中0x05的常量 => #5
001e //指向常量池中0x1e的常量 => #30
```
<a name="I8KVe"></a>
###### 同理解析常量池中的第二个常量#2:
首先，第一个字节是0x09，查表确定类型为`CONSTANT_Fieldref_info`(字段的符号引用)。确定其结构和`CONSTANT_Methodref_info`相同：
```java
CONSTANT_Fieldref_info {
    u1 tag;
    u2 class_index;
    u2 name_and_type_index;
}
```
同样解析得：
```java
//常量#2
09 //0x09表示类型为Fieldref_info(类中字段) 其结构为(U1 flag; U2 index(指向类描述符); U2 index(指向名称和类型描述))=>
0004 //指向常量池中0x04的常量 =>#4
001f //指向常量池中0x1f的常量 =>#31
```
之后的解析过程不再赘述，直接贴上解析常量池#3-#35的结果：
```java
//常量#3
09 //同样是Fieldref_info
0004 // =>#4
0020 // =>#32
//常量#4
07 //0x07表示Class_info(类或接口的符号引用) 其结构为(U1 flag; U2 index(指向全限定名常量))
0021 //指向常量池中的0x21 => #33
//常量#5
07 //同样是Class_info类型
0022 // => #34
//常量#6
01 //0x01表示Utf8_info 表示一个UTF8字符串常量（U1 flag; U2 length(字符串占的字节数); U1 数量为length个，表示byte）说明理论上JVM的字符串常量的字节上线为65535???
000c //length = 0x0c 表示之后12个字节是字符串常量字节内容
5641 525f 434f 4e53 5441 4e54 //UTF-8字符串的内容 用工具翻译成字符串表示为：VAR_CONSTANT
//常量#7
01 //同样是Utf8_info
0012 //length = 18
4c6a 6176 612f 6c61 6e67 2f53 7472 696e 673b //翻译成字符串为：Ljava/lang/String;
//常量#8
01 //同样是Utf8_info
000d //length = 13
436f 6e73 7461 6e74 5661 6c75 65 //翻译成字符串为：ConstantValue
//常量#9
08 //0x08表示String_info 表示字符串字面常量(U1 flag; U2 index(指向字符串字面量))
0023 //指向常量池中的0x23 =>#35
//常量#10
01 //Utf8_info
0008 //length = 8
696e 7446 6965 6c64 //翻译成字符串：intField
//常量#11
01 //Utf8_info
0001 //length = 1
49 //翻译成字符串：I
//常量#12
01 //Utf8_info
000e //length = 14
696e 7441 7272 6179 7346 6965 6c64 //翻译成字符串为：intArraysField
//常量#13
01 //Utf8_info
0002 //length = 2
5b49 //翻译成字符串为：[I
//常量#14
01 //Utf8_info
0008 //length = 8
7374 7246 6965 6c64 //翻译成字符串为：strField
//常量#15
01 //Utf8_info
0006 //length = 6
3c69 6e69 743e //翻译成字符串为：<init>
//常量#16
01 //Utf8_info
0003 //length = 3
2829 56 //翻译成字符串为：()V
//常量#17
01 //Utf8_info
0004 // length = 4
436f 6465 //翻译成字符串为：Code
//常量#18
01 //Utf8_info
000f //length = 15
4c69 6e65 4e75 6d62 6572 5461 626c 65 //翻译成字符串为：LineNumberTable
//常量#19
01 //Utf8_info
0012 //length = 18
4c6f 6361 6c56 6172 6961 626c 6554 6162 6c65 //翻译成字符串为：LocalVariableTable
/常量#20
01 //Utf8_info
0004 //length = 4
7468 6973 //翻译成字符串为：this
//常量#21
01 //Utf8_info
0023 //length = 35
4c 636f 6d2f 696e 7361 6e65 7873 2f6d 6573 732f 6a61 7661 702f 4a61 7661 7054 6573 743b //翻译成字符串为:Lcom/insanexs/mess/javap/JavapTest;
//常量#22
01 //Utf8_info
000c //length = 12
70 7562 6c69 634d 6574 686f 64 //翻译成字符串为：publicMethod
//常量#23
01 //Utf8_info
0018 //length = 24
7072 6f74 6563 7465 6452 6574 7572 6e53 7472 4d65 7468 6f64 //翻译成字符串为：protectedReturnStrMethod
//常量#24
01 //Utf8_info
0014 //length = 20
28 294c 6a61 7661 2f6c 616e 672f 5374 7269 6e67 3b //翻译成字符串为：()Ljava/lang/String;
//常量#25
01 //Utf8_info
0019 //length = 25
7072 6976 6174 6553 796e 6368 726f 6e69 7a65 644d 6574 686f 64 //翻译成字符串为：privateSynchronizedMethod
//常量#26
01 //Utf8_info
0004 //length = 4
2849 2956 //翻译成字符串为：(I)V
//常量#27
01 //Utf8_info
0007 //length = 7
69 6e74 4172 6773 //翻译成字符串为：intArgs
//常量#28
01 //Utf8_info
000a //length = 10
53 6f75 7263 6546 696c 65 //翻译成字符串为：SourceFile
//常量#29
01 //Utf8_info
000e //length = 14
4a61 7661 7054 6573 742e 6a61 7661 //翻译成字符串为：JavapTest.java
//常量#30
0c //0x0c表示NameAndType_info 表示字段或方法的部分符号引用(U1 flag; U2 index(指向字段或方法的名称常量); U2 index(指向字段或方法的描述符常量))
000f //指向常量池中的0x0f => #15
0010 //指向常量池中的0x10 => #16
//常量#31
0c //同样是NameAndType_info
000e //指向常量池中的0x0e => #14
0007 //指向常量池中的0x07 => #7
//常量#32
0c //同样是NameAndType_info
000a //指向常量池中的0x0a => #10
000b //指向常量池中的0x0b => #11
//常量#33
01 //Utf8_info
0021 //length = 33
636f 6d2f 696e 7361 6e65 7873 2f6d 6573 732f 6a61 7661 702f 4a61 7661 7054 6573 74 //翻译成字符串为：com/insanexs/mess/javap/JavapTest
//常量#34
01 //Utf8_info
0010 //length = 16
6a61 7661 2f6c 616e 672f 4f62 6a65 6374 //翻译成字符串为：java/lang/Object
//常量#35
01 //Utf8_info
0008 //length = 8
43 4f4e 5354 414e 54 //翻译成字符串为：CONSTANT
```
上述常量池得很多常量都直接(或是比较直接的)出现在代码中，比如字段名称，类名称，方法全限定名等。有一部分是根据代码能推测，比如方法描述符等，但是还有一部分似乎不明所以，比如上述的CODE，LineNumberTable等。<br />不着急，这些常量在之后得解析中会再次遇到。
<a name="v1KSX"></a>
### 类信息
在解析完常量池的数据后，接下来的一部分数据表示类得一些信息。从上述得字节码得0x0021开始(29行的最后一组)<br />类信息这部分主要有类的访问控制属性，类索引和父类索引(指向常量池中的常量，通常是类得全限定名)，接口个数和接口索引(接口索引同样指向常量池中的常量)。
<a name="ygriv"></a>
#### 访问控制`access_flag`
访问控制占两个字节(16位，每一个二进制代表一种标志,因此理论上最多能有16种标志，Java SE 8中定义了8种)。上述文件中对应访问控制标志的字节位0x0021。0x0021 = (0x0020 | 0x0001)，查表得出`ACC_SUPER`,`ACC_PUBLIC`这个类的访问控制标志位。现代版本编译的类都会带有`ACC_SUPER`，而`ACC_PUBLIC`表示这个类是`public`的。
<a name="R7dQe"></a>
#### 类索引`this_class`
类索引占两个字节，同样指向常量池中的常量(类型为类的符号饮用)。这里对应的数据为：
```java
0004 //this_class U2，指向常量池中的Class_info 这里指向常量池#4
```
<a name="O5Dvg"></a>
#### 父类索引`super_class`
父类索引和类索引类似，同样占两个字节，指向常量池中的常量，只不过指向的类的符号饮用代码的是父类，这里对应的数据为：
```java
0005 //super_class U2 同样指向常量池中的Class_info 这里指向常量池#5
```
<a name="tpugN"></a>
#### 接口个数`interfaces_count`和接口索引`interface`
之后的两个字节表示类实现的接口的个数，然后对应的`interfaces_count * 2`个字节表示接口的索引数据。由于测试类没有实现任何接口，因此`interfaces_counts`为0，之后也没有表示接口数据的字节。
```java
0000 //interface_count 表示接口的个数 这里为0 表示类没有实现接口，之后也没有字节表示接口索引
```
<a name="wSaGL"></a>
### 字段信息
分析完类信息后，之后的数据表示类中字段的信息。这里分为两部分：字段个数`field_count`和字段表`field_info`。
<a name="fXEF2"></a>
#### 字段个数`field_count`
字段个数表示后面将会有几个字段表结构，因为字段表结构长度也不是固定的，因此也只能解析完所有的字段表后才能继续解析下一部分内容，无法直接通过字段个数推出之后的多少字节表示字段信息相关的数据。<br />字段个数占两个字节。
```java
0004 //field_count 表示字段的个数 这里为4 表示接下来的4个字段表结构
```
<a name="lA9O5"></a>
#### 字段表`field_info`
字段表接口较为复杂，因为其是一个表接口，且可能嵌套其他表。<br />先来看一下字段表的接口：
```java
field_info {
    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    u2 attributes_count;
    attribute_info attributes[attributes_count];
}
```
`access_flags`占两字节，表示字段的访问属性，`name_index`占两字节，指向常量池中的常量，表示字段名称，`descriptor_index`占两字节，同样指向常量池中的常量，表示字段的描述符，`attributes_count`占两字节，表示后面有几个属性表，`attribute_info`即属性表，用来描述额外属性，为表结构，长度不固定。<br />前文已知测试类会有4个字段表，这里只针对第一个字段表分析，后续的读者可以自己按规则解析。<br />首先分析固定的前八个字节：
```java
001c //access_flags 字段的访问属性 0x1c = (0x10 | 0x08 | 0x04 ) =>ACC_PROTECTED ACC_FINAL ACC_STATIC
0006 //name_index 指向常量池中#6 即变量名为VAR_CONSTANT
0007 //descriptor_index 指向常量池#7 即描述符Ljava/lang/String; 说明是String类型的字段
0001 //attributes_count 表示有1个attribute_info
```
如果某个字段代表的`attributes_count`的字节值为0，那么对这个字段的解析就已经完成了，但是好巧不巧的，这里分析的`attributes_count`为1，说明之后还有一些字节是用来表示属性表的。<br />属性表用来描述某些特定的额外信息，其整个结构并非是固定长度的，甚至可能属性表中嵌套属性表的情况。<br />了解下属性表的通用结构：
```java
attribute_info {
    u2 attribute_name_index;
    u4 attribute_length;
    u1 info[attribute_length];
}
```
`attribute_name_index`：2字节，指向常量池中的常量，表示常量的名称（Java SE 8规定能识别的常量类型有23种）。<br />`attribute_length`：4字节，表示之后还有多少长度的字节均数据该属性表的内容<br />`info`：不固定长字节，解析方式由属性表的类型决定。<br />可以看到前六个字节都是相同的，然后由`attribute_length`表示之后还有多少个字节，这一点和一些变长协议类似。<br />回到需要解析的数据中来，属于字段1中属性表的字节应该是`0008 0000 0002 0009`。<br />通过解析前两个字节`0008`，得知其指向常量池中的#8，为即`ConstantValue`。这是当字段被`final`修饰后，出现在字段中的属性表，表示一个常量。该属性表的结构如下：
```java
ConstantValue_attribute {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 constantvalue_index;
}
```
可以看到其代表`attribute_length`的四字节数据为`0000 0002`(因为`constantvalue_index`的长度固定为2)，对比字节数据也确实如此。然后解析代表`constantvalue_index`的字节0009，表示指向常量池中#9，为`CONSTANT`。正好是常量的值。<br />之后还剩三个字段，有兴趣的读者可以自己分析，这里直接贴上结果：
```java
//field1
001c //access_flags 字段的访问属性 0x1c = (0x10 | 0x08 | 0x04 ) =>ACC_PROTECTED ACC_FINAL ACC_STATIC
0006 //name_index 指向常量池中0x06 => #6 即VAR_CONSTANT
0007 //descriptor_index 指向常量池中0x07 => #7 即Ljava/lang/String; 说明是String类型的字段
0001 //attributes_count 表示有1个attribute_info 属性表 attribute_info是一个比较复杂的结构，虚拟机规范中定义了虚拟机应当识别的二十多种属性（Java SE 8 23种）所有属性的开始的6字节都是相同的(U2 attribute_name_index + U4 attribute_length),之后的结构由属性自己定义，属性表可以出现在类，字段及方法上
0008 //attribute_name_index 指向常量池中的Utf8_info常量 0x08 => #8 即ConstantValue ConstantValue是属性表的一种，出现在字段中，表示final定义的常量值
0000 0002 //length = 2 表示后面2个字节长度的数据为该属性表的数据
0009 //对于ConstantValue而言 这部分数据表示constantvalue_index 指向常量池中的常量 0x09即 =>#9 即String_info 具体值为#35 为字符串 CONSTANT

//filed2
0042 //access_flags =>(0x40 | 0x02) => ACC_PRIVATE ACC_VOLATILE
000a //name_index 指向常量池中的0x0a #10 即intField
000b //descriptor_index 指向常量池中的0x0b #11 即I 表示int类型的field
0000 //attribute_count = 0 说明无attribute_info

//filed3
0002 //access_flags => 0x02 =>ACC_PRIVATE
000c //name_index 指向常量池中的0x0c #12 即intArraysField
000d //descriptor_index 指向常量池中的0x0d #13 即[I 表示int数组
0000 //attribute_count = 0 说明无attribute_info

//field4
0002 //access_flags => 0x02 =>ACC_PRIVATE
000e //name_index 指向常量池中的0x0e #14 即strField
0007 //descriptor_index 指向常量池中的0x07 #7 即 Ljava/lang/String; 说明是String类型的字段
0000 //attribute_count = 0 说明无attribute_info
```
<a name="zHmzw"></a>
### 方法信息
解析完字段信息，之后的字节是从`0x0004`开始(32行最后一个字节和33行第一个字节)，方法信息和字段信息的解析其实很对成，同样显示通过`methods_count`表示之后有几个方法表，再逐个解析方法表`method_info`，直到达到方法个数。方法表的整体结构和字段表的整体结构也是类似的，只是方法表上的属性表会更多，因此，解析起来要比字段复杂。
<a name="KCl5O"></a>
#### 方法个数`methods_count`
`methods_count`占两个字节，表示之后共有多少个方法表。`0004`表示之后有四个方法表。
<a name="M5tD4"></a>
#### 方法表method_info
方法表和字段表结构是对称的，通用结构如下：
```java
method_info {
    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    u2 attributes_count;
    attribute_info attributes[attributes_count];
}
```
可以看到其结构和`field_info`是相同的，只不过`name_index`指向常量池中表示的方法名称的常量，`descriptor_index`则指向常量池中表示方法描述符的常量。<br />以第一个方法表为例，先解析前八个固定的字节。<br />`access_flag`为0001，对应标识位为`ACC_PUBLIC`，表示为共有方法。<br />`name_index`对应的字节为`000f`,指向常量池#15,即方法名为：，<br />`descriptor_index`为`0010`,指向常量池的#16，即方法描述符为：()V 无参无返回。<br />`attributes_count`为`0001`,表示之后有一个attribute_info属性表。<br />属性表的解析方式在解析字段过程时，已经介绍过了。<br />首先是根据前两字节确认属性表的类型：`0011`，指向常量池中#17，发现是之前不明所以的`CODE`常量，原来是一种属性表的类型。<br />CODE属性表是很重要的一部分信息，因为它含有方法的执行逻辑(代码块)。<br />查看`CODE`属性表的结构：
```java
Code_attribute {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 max_stack;
    u2 max_locals;
    u4 code_length;
    u1 code[code_length];
    u2 exception_table_length;
    { u2 start_pc;
     u2 end_pc;
     u2 handler_pc;
     u2 catch_type;
    } exception_table[exception_table_length];
    u2 attributes_count;
    attribute_info attributes[attributes_count];
}
```
可以看到CODE属性表是一个非固定长度的结构。
<a name="EGKIk"></a>
###### 先看相对简单的部分：
`attribute_name_index`:属性表名称索引，2字节，表示属性表的类型。<br />`attribute_length`:属性长度，4字节，表示之后多少个字节的内容均属于该属性表。对于CODE这种非固定长度的属性表结构而言，长度显得格外重要。<br />`max_stack`:2字节，表示操作数栈的最大深度(注意和方法的调用栈深度是不同的概念)。<br />`max_locals`:2字节，局部变量表的大小，单位是slot，一个slot可以存放32位长度的数据，但是像long和double类型的变量，需要使用两个slot。<br />`code_length`：字节码长度，表示之后n个字节均和JVM字节码指令相关。<br />`code`：字节码，JVM字节码指令占一个字节。但是部分字节码指令需要带上参数(因此消耗了部分字节数据)。<br />参考例子，对这部分结果的解析如下：
```java
0011 //attribute_name_index 指向常量池中的0x11 #17 即Code 表示CODE属性表 CODE属性表结构为(U2 attribute_name_index + U4 attribute_length + U2 max_stack + U2 max_locals + U4 code_length + code_length * U1 code +  U2 exception_table_length + exception_table_length * exception_info + U2 attribute_count + attribute_count * attribute_info)
0000 0033 //length = 0x33 表示之后51个字节为CODE属性表中的信息
0001 //max_stack 1
0001 //max_locals 1
0000 0005 //code_length 5 后面接CODE 每个指令占一个字节 部分指令后的字节表示指令的参数
2a b7 00 01 b1 //aload_0 invokespecial (0001 => 常量池#1) return
```
需要注意的是指令部分`b7`后面两个字节`0001`是b7(invokespecial)需要的参数，指向常量池#1。因此并非CODE中的每一个字节都是指令集中的指令。
<a name="BVzki"></a>
###### 对异常表的解析
这里的例子并没有涉及异常处理的代码，因此`exception_table_length`对应的字节是`0000`，长度为0，说明之后没有异常表。<br />读者如果感兴趣可以自己写代码测试，分析方法类似。
<a name="VhMov"></a>
###### CODE中嵌套属性表的解析
方法表结构之所以复杂，正是因为经常出现表嵌套表的情况。CODE属性表就可能含有其他属性表。<br />同理，解析之后表示`attributes_count`的两个字节——`0002`。<br />第一个属性表的类型索引`0012`，表示指向常量池#18，为`LineNumberTable`(又出现了一个之前不明所以的常量，看来这些常量多半是属性表的类型)，表示指令和行号的对应关系。查找其结构如下：
```java
LineNumberTable_attribute {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 line_number_table_length;
    { u2 start_pc;
     u2 line_number;
    } line_number_table[line_number_table_length];
}
```
得，结构中前面几个还好，后面又嵌套了一个比较复杂的属性。但是没在怕。<br />`attribute_name_index`:这个现在已经很面熟了，表示属性表的名称索引。<br />`attribute_length`:这个也是老伙计，后面多少字节依旧是该属性表得内容。<br />`line_number_table_length`:4字节，表示代码行号表的个数，之后有几个`line_number_table`。<br />`line_number_table`:代码行号表，一个复杂结构，但是好在是固定的，占4字节，前两字节`start_pc`表示一个偏移量，应该是对着之前分析的CODE属性表中的code部分顺序(要注意区分这里的两个CODE，大写的CODE表示属性表的一种类型，小写的code表示CODE属性表中表示字节码指令的部分)。后两个字节`line_number`表示指令对应出现在代码中的行号。
```java
0012 //attribute_name_index 指向常量池中的0x12 即#18 LineNumberTable 表示行号和字节码指令的对应关系
0000 000a //attribute_length 表示后面10个字节均为该属性表的信息
0002 //line_number_table_length 表示后面有2个line_number_table 一个line_number_table结构为(U2 start_pc + U2 line_number)
0000 //start_pc start_pc表示上述指令集中的索引 0对应上述指令集既为2a 为aload_0指令
000d //line_number 等于行号line:13 表示aload_0 对应代码13行
0004 //start_pc 同理对应索引为4的指令 return
000f //line_number 等于行号line:15 表示return 对应代码15行
```
在解析第二个属性表：`attribute_name_index`代表的字节是`0013`，对应常量池#19，为LocalVariableTable 表示方法局部变量的描述。<br />其结构如下：
```java
LocalVariableTable_attribute {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 local_variable_table_length;
    { u2 start_pc;
     u2 length;
     u2 name_index;
     u2 descriptor_index;
     u2 index;
    } local_variable_table[local_variable_table_length];
}
```
前两个部分已经很熟悉了不再赘述。<br />`local_variable_table_length`2字节，表示局部变量表的个数。<br />`local_variable_table`是一个复杂结构，但也是固定长度的，共10字节。<br />`start_pc`占2字节，表示偏移量，`length`占2字节，表示长度，这两部分信息结合起来可以确认变量的作用域是从`start_pc`到`start_pc+length`。<br />`name_index`表示2字节，变量的名称索引，指向常量池中的常量。`descriptor_index`表示2字节，变量的描述符索引，指向常量池中的常量。这两部分信息结合起来可以确定变量的名称和类型。<br />`index`表示变量在局部变量表中的索引，前文已经介绍过了变量存储在局部变量中是以slot为单位，这里的`index`就表示该变量存放在第几个slot。<br />理论分析了，结合情况实践一下：
```java
0013 //attribute_name_index 指向常量池中的0x13 即#19  LocalVariableTable 表示方法局部变量的描述
0000 000c //attribute_length 表示之后12个字节均为 LocalVariableTable 属性表中的内容
0001 //local_variable_table_length 表示有一个局部变量表 local_variable_table的结构为(U2 start_pc + U2 length + U2 name_index + U2 descriptor_index + U2 index)
0000 //start_pc 0
0005 //length 5 说明该局部变量从偏移量0开始到0+5 一直被使用
0014 //name_index 指向常量池中的常量 0x14 => #20 即this
0015 //descriptor_index 指向常量池中的常量 0x15 => #21 即Lcom/insanexs/mess/javap/JavapTest;
0000 //index 0
```
补充说明一点:类中的非静态方法 虚拟机会默认将this指针作为方法的第一个变量。<br />这样第一个方法就解析完成了，读者感兴趣的可以针对剩下的三个方法实操一下，这里就直接贴上解析结果：
```java
//method_1
0001 //access_flag => 0x01 => ACC_PUBLIC
000f //name_index 指向常量池中的0x0f #15 即方法名为：<init>
0010 //descriptor_index 指向常量池中的0x10 #16 即方法描述符为：()V 无参无返回
0001 //attributes_count 表示之后有一个attribute_info
0011 //attribute_name_index 指向常量池中的0x11 #17 即Code 表示CODE属性表 CODE属性表结构为(U2 attribute_name_index + U4 attribute_length + U2 max_stack + U2 max_locals + U4 code_length + code_length * U1 code +  U2 exception_table_length + exception_table_length * exception_info + U2 attribute_count + attribute_count * attribute_info)
0000 0033 //length = 0x33 表示之后51个字节为CODE属性表中的信息
0001 //max_stack 1
0001 //max_locals 1
0000 0005 //code_length 5 后面接CODE 每个指令占一个字节 部分指令后的字节表示指令的参数
2a b7 00 01 b1 //aload_0 invokespecial (0001 => 常量池#1) return
0000 //exception_table_length = 0 说明没有异常表的数据 如果exception_table_length为n 后面的n个字节为异常表相关的信息
0002 //attributes_count = 2
0012 //attribute_name_index 指向常量池中的0x12 即#18 LineNumberTable 表示行号和字节码指令的对应关系
0000 000a //attribute_length 表示后面10个字节均为该属性表的信息
0002 //line_number_table_length 表示后面有2个line_number_table 一个line_number_table结构为(U2 start_pc + U2 line_number)
0000 //start_pc start_pc表示上述指令集中的索引 0对应上述指令集既为2a 为aload_0指令
000d //line_number 等于行号line:13 表示aload_0 对应代码13行
0004 //start_pc 同理对应索引为4的指令 return
000f //line_number 等于行号line:15 表示return 对应代码15行

0013 //attribute_name_index 指向常量池中的0x13 即#19  LocalVariableTable 表示方法局部变量的描述
0000 000c //attribute_length 表示之后12个字节均为 LocalVariableTable 属性表中的内容
0001 //local_variable_table_length 表示有一个局部变量表 local_variable_table的结构为(U2 start_pc + U2 length + U2 name_index + U2 descriptor_index + U2 index)
0000 //start_pc 0
0005 //length 5 说明该局部变量从偏移量0开始到0+5 一直被使用
0014 //name_index 指向常量池中的常量 0x14 => #20 即this
0015 //descriptor_index 指向常量池中的常量 0x15 => #21 即Lcom/insanexs/mess/javap/JavapTest;
0000 //index 0

//method_2
0001 //access_flag =>0x01 =>ACC_PUBLIC
0016 //name_index 指向常量池中的0x16 =>#22 即  publicMethod
0010 //descriptor_index 指向常量池中的0x10 =>#16 即 ()V 表示无参且无返回值
0001 //attribute_count 表示之后有1个attributes_info
0011 //attribute_name_index 同样指向常量中的0x11 #17即CODE属性表
0000 002b //length = 43 表示之后43个字节为CODE属性表的内容
0000 //max_stack = 0
0001 //max_locals = 1
0000 0001 //code_length = 1
b1 //指令 表示return
0000 //exception_table_length = 0 无异常表
0002 //attributes_count = 2
0012 //attribute_name_index 指向常量池中的#18 LineNumberTable
0000 0006 //attribute_length 表示后6个字节为LineNumberTable的信息
0001 //表示只有一个line_number_table
0000 //start_pc 0 对应指令return
0013 //line_number 19  表示return对应的行号是19
0013 //attribute_name_index 指向常量池中的#19 LocalVariableTable
0000 000c //attribute_length 表示之后12个字节均为 LocalVariableTable 属性表中的内容
0001 //local_variable_table_length 表示有1个局部变量表
0000 //start_pc 0
0001 //length 1
0014 //name_index 指向常量池中的常量 0x14 => #20 即this
0015 //descriptor_index 指向常量池中的常量 0x15 => #21 即Lcom/insanexs/mess/javap/JavapTest;
0000 //index 0

//method 3
0004 //access_flag =>0x04 =>ACC_PROTECTED
0017 //name_index 常量池中#23 即 protectedReturnStrMethod
0018 //descriptor_index 常量池中#24 ()Ljava/lang/String; 表示无参，单接返回值类型为String
0001 //attribute_count 表示有一个attribute_info
0011 //attribute_name_index 同样指向常量中的0x11 #17即CODE属性表
0000 002f //length = 47 之后47个字节均为CODE属性表的内容
0001 //max_stack = 1
0001 //max_locals = 1
0000 0005 //code_length = 5 表示方法含有五个指令
2a b4 00 02 b0 //字节码指令 分别表示aload_0 getfield (0002 =>常量池#2) areturn
0000 //exception_table_length = 0 表示无异常表
0002 //attributes_count表示有两个属性表
0012 //attribute_name_index 常量池#18 LineNumberTable
0000 0006 //attribute_length 表示后6个字节为LineNumberTable的信息
0001 //表示只有一个line_number_table
0000 //start_pc 0 对应的指令aload_0
0016 //line_number 对应line:22
0013 //attribute_name_index 常量池#19 LocalVariableTable
0000 000c //attribute_length 表示之后12个字节均为 LocalVariableTable 属性表中的内容
0001 //local_variable_table_length 表示有1个局部变量表
0000 //start_pc 0
0005 //length 5
0014 //name_index 指向常量池#20 即this
0015 //descriptor_index 指向常量池#21 即Lcom/insanexs/mess/javap/JavapTest;
0000 //index 0

//method 4
0022 //access_flag => (0x20 | 0x02) => ACC_SYNCHRONIZED ACC_PRIVATE
0019 //name_index 常量池中#25 即 privateSynchronizedMethod
001a // descriptor_index 常量池中#26 (I)V 接受一个int参数 但无返回值
0001 //attribute_count 表示有一个attribute_info
0011 //attribute_name_index 同样指向常量中的0x11 #17即CODE属性表
0000 003e //length = 62 之后的62个字节均为CODE属性
0002 //max_stack = 2
0002 //max_locals = 2
0000 0006 //code_length = 6 表示之后6个字节均为字节码指令
2a 1b b5 00 03 b1 //分别为aload_0 iload_1 putfield (0003 =>常量池#3) return
0000 //exception_table_length = 0 表示无异常表
0002 //attributes_count表示有两个属性表
0012 //attribute_name_index 常量池#18 LineNumberTable
0000 000a //attribute_length 表示后10个字节为LineNumberTable的信息
0002 //表示有2个line_number_table
0000 //start_pc 0 对应的指令aload_0
001a //line_number 对应line:26
0005 //start_pc 5 对应的指令return
001b //line_number 对应line:27
0013 //attribute_name_index 常量池#19 LocalVariableTable
0000 0016 //attribute_length = 22 之后22个字节均为局部变量表的内容
0002 //local_variable_table_length = 2 表示存在两个局部变量表
0000 //start_pc 0
0006 //length 6
0014 //name_index 常量池#20 即this
0015 //descriptor_index 指向常量池#21 即Lcom/insanexs/mess/javap/JavapTest;
0000 //index 0
0000 //start_pc 0
0006 //length 6
001b //name_index 指向常量池#27 即intArgs
000b //descriptor_index 指向常量池#11 即I 表示int类型
0001 //index 1
```
<a name="MTkXr"></a>
### 属性表信息
在解析完方法后，剩下还有一小部分内容是属性表信息。现在对属性表的解析过程可以说是轻车熟路了。<br />未解析的字节并不多了：
```java
0001 001c 0000 0002 001d
```
首先`attributes_count`为`0001`,说明只有一个属性表，so easy!<br />`attributes_name_index`为`001c`，指向常量池#28，即SourceFile 用于记录源文件名称。<br />SourceFile的结构如下：
```java
SourceFile_attribute {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 sourcefile_index;
}
```
定长的结构，最后2个字节表示源文件名称索引，指向常量池。这里为`001d`，指向常量池中#29，为JavapTest.java。<br />这样，就完成了所有的字节码文件解析。
<a name="dqEiF"></a>
### 其他问题的小测试
这里针对类文件结构学习过程中，几个疑问做了下测试。
<a name="atbyH"></a>
#### 同步方法和同步块在类文件结构中的表示有什么不同？
上文已经测了同步方法，其是通过方法的`access_flag`的标志位(`ACC_SYNCHRONIZED`)表示的。但是这种方式对于同步块而言已经是不行的，那么同步块是如何实现同步控制的呢？<br />测试代码类如下：
```java
public class SynchronizedTest {

    public synchronized void synchronizedMethod(){
        return;
    }

    public static synchronized void staticSynchronizedMethod(){
        return;
    }

    public void synchronizedCode(){
        synchronized (this){
            return;
        }
    }

    public void staticSynchronizedCode(){
        synchronized (SynchronizedTest.class){
            return;
        }
    }
}
```
使用`javap -verbose -c` 反编译class文件。<br />截取相关四个方法，如下：
```java
public synchronized void synchronizedMethod();
  descriptor: ()V
  flags: ACC_PUBLIC, ACC_SYNCHRONIZED
  Code:
    stack=0, locals=1, args_size=1
       0: return
    LineNumberTable:
      line 11: 0
    LocalVariableTable:
      Start  Length  Slot  Name   Signature
          0       1     0  this   Lcom/insanexs/mess/javap/SynchronizedTest;

public static synchronized void staticSynchronizedMethod();
  descriptor: ()V
  flags: ACC_PUBLIC, ACC_STATIC, ACC_SYNCHRONIZED
  Code:
    stack=0, locals=0, args_size=0
       0: return
    LineNumberTable:
      line 15: 0

public void synchronizedCode();
  descriptor: ()V
  flags: ACC_PUBLIC
  Code:
    stack=2, locals=3, args_size=1
       0: aload_0
       1: dup
       2: astore_1
       3: monitorenter
       4: aload_1
       5: monitorexit
       6: return
       7: astore_2
       8: aload_1
       9: monitorexit
      10: aload_2
      11: athrow
    Exception table:
       from    to  target type
           4     6     7   any
           7    10     7   any
    LineNumberTable:
      line 19: 0
      line 20: 4
      line 21: 7
    LocalVariableTable:
      Start  Length  Slot  Name   Signature
          0      12     0  this   Lcom/insanexs/mess/javap/SynchronizedTest;
    StackMapTable: number_of_entries = 1
      frame_type = 255 /* full_frame */
        offset_delta = 7
        locals = [ class com/insanexs/mess/javap/SynchronizedTest, class java/lang/Object ]
        stack = [ class java/lang/Throwable ]

public void staticSynchronizedCode();
  descriptor: ()V
  flags: ACC_PUBLIC
  Code:
    stack=2, locals=3, args_size=1
       0: ldc           #2                  // class com/insanexs/mess/javap/SynchronizedTest
       2: dup
       3: astore_1
       4: monitorenter
       5: aload_1
       6: monitorexit
       7: return
       8: astore_2
       9: aload_1
      10: monitorexit
      11: aload_2
      12: athrow
    Exception table:
       from    to  target type
           5     7     8   any
           8    11     8   any
    LineNumberTable:
      line 25: 0
      line 26: 5
      line 27: 8
    LocalVariableTable:
      Start  Length  Slot  Name   Signature
          0      13     0  this   Lcom/insanexs/mess/javap/SynchronizedTest;
    StackMapTable: number_of_entries = 1
      frame_type = 255 /* full_frame */
        offset_delta = 8
        locals = [ class com/insanexs/mess/javap/SynchronizedTest, class java/lang/Object ]
         stack = [ class java/lang/Throwable ]
```
从上面可以分析出：无论是类还是实例的同步方法，都是通过在`ACCESS_FLAG`中将`ACC_SYNCHRONIZED`位标识为真实现的。<br />而对于同步块，则是通过字节码指令中添加`monitorenter`和`monitorexit`实现的。而锁的对象则是根据操作数栈当前的对象所决定。
<a name="lHQrK"></a>
#### 测试`max_stack`
这个测试主要是加深对操作数栈的理解，设计一个代码，让其在类文件结构中的max_stack为2。<br />满足上面要求的测试代码如下：
```java
public class MaxStackTest {

    public int maxStack2Method(){
        int var1 = 1;
        int var2 = 2;
        return var1 + var2;
    }
}
```
为什么说此时操作数栈最大深度为2，因为首先var1从局部变量表中加载到操作数栈，此时操作数栈的深度为1，接着继续从局部变量表中将var2加载到操作数栈，此时栈深度为2。而后为了计算和，操作数栈弹出var2和var1，深度重回0。所以最大深度为2。<br />同样可以拿javap命令反编译验证：
```java
public int maxStack2Method();
    descriptor: ()I
    flags: ACC_PUBLIC
    Code:
      stack=2, locals=3, args_size=1
         0: iconst_1
         1: istore_1
         2: iconst_2
         3: istore_2
         4: iload_1
         5: iload_2
         6: iadd
         7: ireturn
      LineNumberTable:
        line 11: 0
        line 12: 2
        line 13: 4
      LocalVariableTable:
        Start  Length  Slot  Name   Signature
            0       8     0  this   Lcom/insanexs/mess/javap/MaxStackTest;
            2       6     1  var1   I
            4       4     2  var2   I
}
```
<a name="bG7wA"></a>
#### 测试64位虚拟机下，`int`,`long`,`reference`分别占几个slot
因为在32位虚拟机下`int`和`reference`只占一个slot，而`long`和`double`占两个slot，那么这种情况在64位的虚拟机下是否依旧如此？<br />测试代码如下：
```java
public class SlotTest {

    private Object reference;

    public void testSlot(){
        int i =0;
        long l = 1L;
        Object reference = new Object();

        int j = 1;
        System.out.println(i + ","+ l + "," + reference + "," + j);
    }
}
```
通过javap查看局部变量表，发现除了long是占2个slot的，其余的像int和reference都只占1个slot。说明和32位的情况一致。<br />仔细想想，确实如此。因此在class文件的开头校验部分只针对版本号进行了校验，并不区分是32位还是64位，说明二者的编译规则应该是一致的。
