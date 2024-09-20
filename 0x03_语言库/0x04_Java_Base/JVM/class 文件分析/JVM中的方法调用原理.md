<a name="VkXbK"></a>
## **1、引言**
多态是Java语言极为重要的一个特性，可以说是Java语言动态性的根本，那么线程执行一个方法时到底在内存中经历了什么，JVM又是如何确定方法执行版本的呢？
<a name="WFRgs"></a>
## **2、栈帧**
JVM中由栈帧存储方法的局部变量表、操作数栈、动态连接和方法返回地址等信息。每一个方法的调用就是从入栈到出栈到过程。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1699882354488-3418406a-cca7-48a6-8702-706b6d314188.png#averageHue=%23fafafa&clientId=u751e6a34-5174-4&from=paste&id=u04317030&originHeight=890&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5b2da384-6e31-4152-935f-44eec74695e&title=)
<a name="pZK3k"></a>
### **2.1 局部变量表**
局部变量表由变量槽组成，《Java虚拟机规范》指出：“每个变量槽都应该能存放一个boolean、byte、char、short、int、float、reference或returnAddress类型的数据”。<br />这八种数据类型都可以使用32位或更小的物理内存来存储，如果是64位虚拟机环境下，虚拟机需要通过对齐填充来使变量槽与在32位虚拟机环境下外观一致。<br />如果是64位的数据类型，比如long和double，JVM会以高位对齐的方式为其分配两个连续的变量槽空间。且规定不允许以任何方式访问这两个变量槽的其中一个，类加载的校验阶段会针对违反规定的行为抛出异常。<br />类变量会有两次赋值，一次是准备阶段给赋值一个默认值，二是初始化阶段，赋予程序定义的值。但方法变量没有准备阶段，所以没赋值的方法变量不能被使用。
<a name="QRrVg"></a>
### **2.2 变量槽的复用**
为了节省内存空间，变量槽是可以复用的。当程序计数器的值超过方法体中定义的变量的作用域时，这个变量的变量槽就可以被其他变量复用了。不过虽然这样可以节省内存空间，但对GC有一定影响。<br />举个例子，**如果没有发生即时编译的前提下**，在方法清单1中placeholder不会被回收。原因是，方法清单1中gc发生时，变量槽仍然保持着对placeholder的引用，所以不会被标记为可回收对象。而在方法清单2中国呢增加了int a = 0后，placeholder原有的变量槽被变量a复用了，也就不存在引用placeholder的变量槽了，所以placeholder就可以被回收了。<br />方法清单1:
```java
public static void main (String[] args) {
    {    
        byte[] placeholder = new byte[64 * 1024 * 1024];
    }  
    System.gc();
}
```
方法清单2:
```java
public static void main (String[] args) {
    {
        byte[] placeholder = new byte[64 * 1024 * 1024];
    }  
    int a = 0;  
    System.gc();
}
```
但是实际上，大部分程序都是运行在即时编译下的，所以编译器会对其进行优化，实际情况下方法清单1中placeholder也能被回收。
<a name="GyXLA"></a>
### **2.3 操作数栈**
操作数栈主要作用有二：<br />1.作为计算过程中的所需变量的临时存储空间<br />2.存储系统运行过程中的计算中间结果<br />操作数栈不能通过指针访问，只能通过弹栈和压栈来操作其内部元素。当执行某项指令前会将所需变量压入栈顶，然后真正执行指令时从栈顶依次取出用来执行具体指令，执行完成后会将结果在压入操作数栈。<br />大多数虚拟机实现会有一些优化处理，将两个栈帧部分重叠：上一个栈帧的**部分操作数栈**和下一个栈帧的**部分局部变量表**。不仅节约空间，还让下面栈的操作可以直接使用上面栈的内容，减少了参数传递。
<a name="qYzdE"></a>
### **2.4 动态链接**
Java文件被编译成Class文件后，变量和方法的引用都作为符号引用保存在Class文件中的常量池中。而对于方法的引用，某些可以在编译期就确定下来称为“直接引用”，而有些方法只能在运行期才能确定下来（比如方法的重载）。<br />**动态链接的作用**就是在运行期将符号引用转换为直接引用。
<a name="YIpww"></a>
### **2.5 方法返回地址**
一个方法执行完成后，有两种方式退出：正常完成和抛出异常。<br />当方法A中调用方法B时，A的栈帧中会保存程序计数器的值作为返回地址。而异常退出时，返回地址是要通过异常处理器表来确定的。<br />方法返回后还会进行几个操作：

1. 恢复主调线程对应栈帧中的局部变量表和操作数栈
2. 把返回值压入主调线程的栈帧中
3. 调整程序计数器到方法调用指令的下一条指令
<a name="WcJWo"></a>
### 2.6 附加信息
不同虚拟机在实现时可以自定义一些例如调试、性能收集等信息放到栈帧之中。
<a name="Q79AY"></a>
## **3、方法调用**
一切方法调用在Class文件里面存储的都只是符号引用，某些调用需要在类加载时甚至运行期间才能确定目标方法的直接引用，这是Java强大的动态扩展能力的基础。
<a name="ljQ3V"></a>
### **3.1 方法调用指令**
JVM共支持以下5种方法调用字节码指令：

- invokestatic调用静态方法
- invokespecial调用构造器()方法、私有方法和父类中的方法
- invokevirtual调用所有虚方法
- invokeinterface调用接口方法，运行期会确定具体实现该接口的对象
- invokedynamic调用运行期动态解析出具体调用的方法

其中，invokestatic和invokespecial指令调用的方法，都可以类加载的解析阶段确定调用的方法版本，Java中符合这个条件的方法共有五种：静态方法、私有方法、实例构造器、父类方法和final修饰的方法（它使用invokevirtual指令调用）。<br />这五种方法称为“非虚方法”（Non-Virtual Method），剩下的均为“虚方法”（Virtual Method）。
<a name="OGXLL"></a>
### **3.2 解析**
如果一个方法在类加载的解析阶段就能确定方法的调用版本，那么这类方法的调用被称为解析（Resolution）。<br />Java中符合解析标准的主要是**静态方法**和**私有方法**。前者与类型直接相关，后者对外不可见。<br />方法调用指令中，invokestatic和invokespecial指令调用的方法，再加上final修饰的方法，都被称作“非虚方法”，他们都可以在解析阶段确定唯一的调用版本。其他的方法都被称作“虚方法”。
<a name="sIO48"></a>
### **3.3 分派**
在编译阶段，依赖静态类型确定方法的调用版本，这就叫做**“静态分派”**。<br />而在运行期，根据实际类型确定方法调用版本被称作**“动态分派”**。
<a name="JDL2t"></a>
#### **3.3.1 静态分派**
直接上个🌰：
```java
public class StaticDispatch {

    static abstract class Human {}

    static class Man extends Human {}

    static class Woman extends Human {}

    public void sayHello (Human guy) {
        System.out.println("hello,guy!");
    }
    public void sayHello (Man guy) {
        System.out.println("hello,gentleman!");
    }
    public void sayHello (Woman guy) {
        System.out.println("hello,lady!");
    }
    public static void main (String[] args) {
        StaticDispatch sd = new StaticDispatch();
        Human man = new Man();
        Human woman = new Woman();
        sd.sayHello(man);
        sd.sayHello(woman);
    }
}
```
输出如下：
```
hello,guy!
hello,guy!
```
首先介绍一下静态类型和实际类型。<br />当声明变量man时：
```java
Human man = new Man();
```
其中Human被称为“静态类型”，Man和Woman叫做“实际类型”。静态类型在编译期可知，实际类型需要等到运行期才能确定。<br />回到上面的示例，为什么两次输出都是hello,guy!呢？<br />这是因为**编译器在重载时是通过参数的静态类型而不是实际类型作为判定依据的。**所以上面代码示例中，多次不同对象调用`sayHello()`均找到各自的静态类型对应的方法版本，即`sayHello (Human guy)`。
<a name="Nu3gM"></a>
##### 方法版本的选择顺序
事实上，虽然编译器能确定方法的重载版本，但往往这并不是唯一的，仅仅只能确定一个“相对更合适的“版本。
```java
public class Overload{
    public static void sayHello (Object arg){
        System.out.println("hello Object");
    }
    public static void sayHello (int arg) {
        System.out.println("hello int");
    }
    public static void sayHello (long arg) {
        System.out.println("hello long");
    }
    public static void sayHello (Character arg){
        System.out.println("hello Character");
    }
    public static void sayHello (char arg) {
        System.out.println("hello char");}
    public static void sayHello (char... arg) {
        System.out.println("hello char...");
    }
    public static void sayHello (Serializable arg) {
        System.out.println("hello Serializable");
    }
    public static void main (String[] args) {
        sayHello('a');
    }
}
```
输出如下：
```
hello char
```
如果删除掉`sayHello (char arg)`方法，则会匹配到`sayHello (int arg)`方法，编译器匹配的转型顺序是char > int > long > float > double，但不会匹配到byte和short类型的重载，因为char到byte或short的转型是不安全的。
<a name="TjdBQ"></a>
#### **3.3.2 动态分派**
区别于静态分派，如果需要在运行期根据对象类型来确定方法版本，则属于动态分派。
```java
public class DynamicDispatch {
    static abstract class Human {
        public void speak() {
            System.out.println("I'm human");
        }
    }
    static class Man extends Human {
        @Override
        public void speak() {
            System.out.println("I'm man");
        }
    }
    static class Woman extends Human {
        @Override
        public void speak() {
            System.out.println("I'm woman");
        }
    }
    public static void main(String[] args) {
        Human man = new Man();
        Human woman = new Woman();
        man.speak();
        woman.speak();
        man = new Woman();
        man.speak();
    }
}
```
输出如下：
```
I'm man
I'm woman
I'm woman
```
在上面的例子中，对于`speak()`方法的调用在编译期完全无法确定，所以只能动态匹配对应的方法版本。那么JVM是如何进行匹配呢？答案就是`invokevirtual`指令。
<a name="zUJYT"></a>
##### `invokevirtual`指令运行过程
Java做到多态的根本原因是`invokevirtual`指令的执行逻辑，该命令的运行过程如下：

1. 找到操作数栈顶元素的对象的实际类型，记作C。
2. 如果在C中找到与常量中描述符和简单名称都一样的方法，则进行访问权限校验，通过则返回方法的直接引用，否则抛出异常java.lang.IllegalAccessError。
3. 否则，按照继承关系从下往上依次寻找C类的父类，来进行第二步中的方法查找和校验。
4. 最后仍未能找到方法则抛出异常java.lang.AbastractMethodError。

_注意：由此也能看出，Java中只有虚方法，没有“虚字段”，如果子类和父类存在相同名称的字段，子类中的字段会覆盖父类中的字段。_
<a name="KOAPm"></a>
#### **3.3.3 静态分派和动态分派的对比**
| 分派类型 | 原理 | 发生阶段 | 应用场景 |
| --- | --- | --- | --- |
| 静态分派 | 根据静态类型判断方法版本 | 编译期 | 重载 |
| 动态分派 | 根据实际类型判断方法版本 | 运行期 | 重写 |

