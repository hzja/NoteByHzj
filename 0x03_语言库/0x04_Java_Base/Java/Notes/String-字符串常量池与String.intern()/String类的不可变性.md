Java
<a name="sfE53"></a>
## 一、原理（为什么说String类是不可变的）
<a name="ovrCc"></a>
### 1、什么是不可变对象
如果一个对象在创建之后就不能再改变它的状态，那么这个对象是不可变的（Immutable）。不能改变状态的意思是，不能改变对象内的成员变量，包括基本数据类型变量的值不能改变，引用类型的变量不能指向其他的对象，引用类型指向的对象的状态也不能改变。
<a name="NnVJ1"></a>
### 2、`final`关键字的作用
如果要创建一个不可变对象，关键一步就是要将所有的成员变量声明为`final`类型。所以下面简单回顾一下`final`关键字的作用：

- `final`修饰类，表示该类不能被继承，俗称断子绝孙类，该类的所有方法自动地成为`final`方法
- `final`修饰方法，表示子类不可重写该方法
- `final`修饰基本数据类型变量，表示该变量为常量，值不能再修改
- `final`修饰引用类型变量，表示该引用在构造对象之后不能指向其他的对象，但该引用指向的对象的状态可以改变
<a name="M21K1"></a>
### 3、String类不可变性的分析
先看下面这段代码：
```java
String s = "abc";    //(1)
System.out.println("s = " + s);

s = "123";    //(2)
System.out.println("s = " + s);
```
打印结果为：
```java
s = abc
s = 123
```
看到这里，可能对String是不可变对象产生了疑惑，因为从打印结果可以看出，s的值的确改变了。其实不然，因为s只是一个String对象的引用，并不是String对象本身。<br />当执行(1)处这行代码之后，会先在方法区的运行时常量池创建一个String对象"abc"，然后在Java栈中创建一个String对象的引用s，并让s指向"abc"，如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1656220668667-c6875d04-4140-4485-9f35-cc68504afc71.png#clientId=u691be5a4-3ed3-4&from=paste&id=ub71b66dd&originHeight=346&originWidth=588&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u97e90c9e-a7c2-4b29-b52a-63194c3f055&title=)<br />当执行完(2)处这行代码之后，会在方法区的运行时常量池创建一个新的String对象"123"，然后让引用s重新指向这个新的对象，而原来的对象"abc"还在内存中，并没有改变，如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1656220668677-67e15592-cf9f-4e3e-ba14-20cbd4a285de.png#clientId=u691be5a4-3ed3-4&from=paste&id=ued12fffe&originHeight=346&originWidth=588&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7777eb0a-9721-4542-893a-c49a000cc0a&title=)
<a name="xHwwi"></a>
### 4、String类不可变性的原理
要理解String类的不可变性，首先看一下String类中都有哪些成员变量。在JDK1.8中，String的成员变量主要有以下几个：
```java
public final class String
    implements java.io.Serializable, Comparable<String>, CharSequence {    
    /** The value is used for character storage. */
    private final char value[];    /** Cache the hash code for the string */
    private int hash; // Default to 0

    /** use serialVersionUID from JDK 1.0.2 for interoperability */
    private static final long serialVersionUID = -6849794470754667710L;    /**
     * Class String is special cased within the Serialization Stream Protocol.
     *
     * A String instance is written into an ObjectOutputStream according to
     * <a href="{@docRoot}/../platform/serialization/spec/output.html">
     * Object Serialization Specification, Section 6.2, "Stream Elements"</a>
     */
    private static final ObjectStreamField[] serialPersistentFields = new ObjectStreamField[0];
```
首先可以看到，String类使用了`final`修饰符，表明String类是不可继承的。<br />然后，主要关注String类的成员变量value，value是`char[]`类型，因此String对象实际上是用这个字符数组进行封装的。再看value的修饰符，使用了`private`，也没有提供setter方法，所以在String类的外部不能修改value，同时value也使用了`final`进行修饰，那么在String类的内部也不能修改value，但是上面`final`修饰引用类型变量的内容提到，这只能保证value不能指向其他的对象，但value指向的对象的状态是可以改变的。**通过查看String类源码可以发现，String类不可变，关键是因为SUN公司的工程师，在后面所有String的方法里都很小心的没有去动字符数组里的元素。所以String类不可变的关键都在底层的实现，而不仅仅是一个**`**final**`**。**
<a name="hNSI8"></a>
### 5、String对象真的不可变吗
上面提到，value虽然使用了`final`进行修饰，但是只能保证vaue不能指向其他的对象，但value指向的对象的状态是可以改变的，也就是说，可以修改value指向的字符数组里面的元素。因为value是private类型的，所以只能使用反射来获取String对象的value属性，再去修改value指向的字符数组里面的元素。通过下面的代码进行验证：
```java
String s = "Hello World";
System.out.println("s = " + s);//获取String类中的value属性

Field valueField = String.class.getDeclaredField("value");//改变value属性的访问权限

valueField.setAccessible(true);//获取s对象上的value属性的值

char[] value = (char[]) valueField.get(s);//改变value所引用的数组中的第6个字符

value[5] = '_';
System.out.println("s = " + s);
```
打印结果为：
```java
s = Hello World
s = Hello_World
```
在上述代码中，s始终指向同一个String对象，但是在反射操作之后，这个String对象的内容发生了变化。也就是说，通过反射是可以修改String这种不可变对象的。
<a name="nwFeH"></a>
## 二、设计目标（为什么String要设计成不可变的）
在Java中，将String设计成不可变的是综合考虑到内存、同步、数据结构及安全等各种因素的结果，下文将为各种因素做一个小结。
<a name="l26ll"></a>
### 1、运行时常量池的需要
```java
String s = "abc";
```
执行上述代码时，JVM首先在运行时常量池中查看是否存在String对象“abc”，如果已存在该对象，则不用创建新的String对象“abc”，而是将引用s直接指向运行时常量池中已存在的String对象“abc”；如果不存在该对象，则先在运行时常量池中创建一个新的String对象“abc”，然后将引用s指向运行时常量池中创建的新String对象。
```java
String s1 = "abc";
String s2 = "abc";
```
执行上述代码时，在运行时常量池中只会创建一个String对象"abc"，这样就节省了内存空间。示意图如下所示：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1656220668675-ca3f97ac-d023-4282-bf4c-e5ac1f98fc64.png#clientId=u691be5a4-3ed3-4&from=paste&id=ud69f09a8&originHeight=346&originWidth=588&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u89592c03-c305-46a1-afab-dca12f42117&title=)
<a name="HS1ZW"></a>
### 2、同步
因为String对象是不可变的，所以是多线程安全的，同一个String实例可以被多个线程共享。这样就不用因为线程安全问题而使用同步。
<a name="CkcbG"></a>
### 3、允许String对象缓存hashcode
查看上文JDK1.8中String类源码，可以发现其中有一个字段hash，String类的不可变性保证了hashcode的唯一性，所以可以用hash字段对String对象的hashcode进行缓存，就不需要每次重新计算hashcode。所以Java中String对象经常被用来作为HashMap等容器的键。
<a name="DdTIF"></a>
### 4、安全性
如果String对象是可变的，那么会引起很严重的安全问题。比如，数据库的用户名、密码都是以字符串的形式传入来获得数据库的连接，或者在socket编程中，主机名和端口都是以字符串的形式传入。因为String对象是不可变的，所以它的值是不可改变的，否则黑客们可以钻到空子，改变String引用指向的对象的值，造成安全漏洞。
