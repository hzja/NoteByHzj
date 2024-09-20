Java<br />Object 类属于 java.lang 包，此包下的所有类在使用时无需手动导入，系统会在程序编译期间自动导入。Object 类是所有类的基类，当一个类没有直接继承某个类时，默认继承Object类，也就是说任何类都直接或间接继承此类，Object 类中能访问的方法在所有类中都可以调用，下面分别介绍Object 类中的所有方法。
<a name="ShETn"></a>
### 1、Object 类的结构图
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624637769363-bb5c3176-eb59-4107-85a6-cbf0dec1364e.png#clientId=u0efae26f-7d49-4&from=paste&id=u126336cc&originHeight=705&originWidth=605&originalType=binary&ratio=3&size=63291&status=done&style=none&taskId=u50e9368e-5df8-44cd-ba6d-21c28af998b)<br />Object.class类
```java
/*
 * Copyright (c) 1994, 2012, Oracle and/or its affiliates. All rights reserved.
 * ORACLE PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 *
 */

package java.lang;

/**
 * Class {@code Object} is the root of the class hierarchy.
 * Every class has {@code Object} as a superclass. All objects,
 * including arrays, implement the methods of this class.
 *
 * @author  unascribed
 * @see     java.lang.Class
 * @since   JDK1.0
 */
public class Object {

    private static native void registerNatives();
    static {
        registerNatives();
    }

    public final native Class<?> getClass();

    public native int hashCode();

    public boolean equals(Object obj) {
        return (this == obj);
    }

    protected native Object clone() throws CloneNotSupportedException;
    
    public String toString() {
        return getClass().getName() + "@" + Integer.toHexString(hashCode());
    }

    public final native void notify();
    
    public final native void notifyAll();
    
    public final native void wait(long timeout) throws InterruptedException;
    
    public final void wait(long timeout, int nanos) throws InterruptedException {
        if (timeout < 0) {
            throw new IllegalArgumentException("timeout value is negative");
        }
        if (nanos < 0 || nanos > 999999) {
            throw new IllegalArgumentException(
                    "nanosecond timeout value out of range");
        }
        if (nanos > 0) {
            timeout++;
        }
        wait(timeout);
    }
    
    public final void wait() throws InterruptedException {
        wait(0);
    }
    
    protected void finalize() throws Throwable { }
}
```
<a name="glDhh"></a>
### 2、为什么java.lang包下的类不需要手动导入？
不知道大家注意到没，在使用诸如Date类时，需要手动导入`import java.util.Date`，再比如使用`File`类时，也需要手动导入`import java.io.File`。但是在使用`Object`类，`String` 类，`Integer`类等不需要手动导入，而能直接使用，这是为什么呢？<br />这里先告诉大家一个结论：使用 java.lang 包下的所有类，都不需要手动导入。<br />另外介绍一下Java中的两种导包形式，导包有两种方法：<br />①、单类型导入（single-type-import），例如`import java.util.Date`<br />②、按需类型导入(type-import-on-demand)，例如`import java.util.*`<br />单类型导入比较好理解，编程所使用的各种工具默认都是按照单类型导包的，需要什么类便导入什么类，这种方式是导入指定的`public`类或者接口；<br />按需类型导入，比如 `import java.util.*`，可能看到后面的 `*`，大家会以为是导入java.util包下的所有类，其实并不是这样，根据名字按需导入要知道他是按照需求导入，并不是导入整个包下的所有类。<br />Java编译器会从启动目录(bootstrap)，扩展目录(extension)和用户类路径下去定位需要导入的类，而这些目录仅仅是给出了类的顶层目录，编译器的类文件定位方法大致可以理解为如下公式：<br />顶层路径名 \ 包名 \ 文件名.class = 绝对路径<br />单类型导入知道包名和文件名，所以编译器可以一次性查找定位到所要的类文件。按需类型导入则比较复杂，编译器会把包名和文件名进行排列组合，然后对所有的可能性进行类文件查找定位。例如:
```java
package com;

import java.io.*;

import java.util.*;
```
如果文件中使用到了 File 类，那么编译器会根据如下几个步骤来进行查找 File 类：<br />①、`File` // File类属于无名包，就是说File类没有package语句，编译器会首先搜索无名包<br />②、`com.File` // File类属于当前包，就是当前编译类的包路径<br />③、`java.lang.File` //由于编译器会自动导入java.lang包，所以也会从该包下查找<br />④、`java.io.File`<br />⑤、`java.util.File`<br />......<br />需要注意的地方就是，编译器找到`java.io.File`类之后并不会停止下一步的寻找，而要把所有的可能性都查找完以确定是否有类导入冲突。假设此时的顶层路径有三个，那么编译器就会进行3*5=15次查找。<br />如果在查找完成后，编译器发现了两个同名的类，那么就会报错。要删除不用的那个类，然后再编译。<br />所以可以得出这样的结论：按需类型导入是绝对不会降低Java代码的执行效率的，但会影响到Java代码的编译速度。所以在编码时最好是使用单类型导入，这样不仅能提高编译速度，也能避免命名冲突。<br />讲清楚Java的两种导包类型了，再回到为什么可以直接使用 Object 类，看到上面查找类文件的第③步，编译器会自动导入 java.lang 包，那么当然能直接使用了。至于原因，因为用的多，提前加载了，省资源。
<a name="ppWEC"></a>
### 3、类构造器
类构造器是创建Java对象的途径之一，通过new 关键字调用构造器完成对象的实例化，还能通过构造器对对象进行相应的初始化。一个类必须要有一个构造器的存在，如果没有显示声明，那么系统会默认创造一个无参构造器，在JDK的Object类源码中，是看不到构造器的，系统会自动添加一个无参构造器。可以通过：`Object obj = new Object();`构造一个Object类的对象。
<a name="wxXxN"></a>
### 4、`equals` 方法
通常很多面试题都会问 `equals()` 方法和 == 运算符的区别，== 运算符用于比较基本类型的值是否相同，或者比较两个对象的引用是否相等，而 `equals` 用于比较两个对象是否相等，这样说可能比较宽泛，两个对象如何才是相等的呢？这个标尺该如何定？可以看看 Object 类中的equals 方法：
```java
public boolean equals(Object obj) {
    return (this == obj);
}
```
可以看到，在 Object 类中，== 运算符和 `equals` 方法是等价的，都是比较两个对象的引用是否相等，从另一方面来讲，如果两个对象的引用相等，那么这两个对象一定是相等的。对于自定义的一个对象，如果不重写 `equals` 方法，那么在比较对象的时候就是调用 Object 类的 `equals` 方法，也就是用 == 运算符比较两个对象。可以看看 String 类中的重写的 `equals` 方法：
```java
public boolean equals(Object anObject) {
    if (this == anObject) {
        return true;
    }
    if (anObject instanceof String) {
        String anotherString = (String)anObject;
        int n = value.length;
        if (n == anotherString.value.length) {
            char v1[] = value;
            char v2[] = anotherString.value;
            int i = 0;
            while (n-- != 0) {
                if (v1[i] != v2[i])
                    return false;
                i++;
            }
            return true;
        }
    }
    return false;
}
```
String 是引用类型，比较时不能比较引用是否相等，重点是字符串的内容是否相等。所以 String 类定义两个对象相等的标准是字符串内容都相同。<br />在Java规范中，对 equals 方法的使用必须遵循以下几个原则：<br />①、自反性：对于任何非空引用值 x，`x.equals(x)` 都应返回 true。<br />②、对称性：对于任何非空引用值 x 和 y，当且仅当 `y.equals(x)` 返回 true 时，`x.equals(y)` 才应返回 true。<br />③、传递性：对于任何非空引用值 x、y 和 z，如果 `x.equals(y)` 返回 true，并且 `y.equals(z)` 返回 true，那么 `x.equals(z)` 应返回 true。<br />④、一致性：对于任何非空引用值 x 和 y，多次调用 `x.equals(y)` 始终返回 true 或始终返回 false，前提是对象上 `equals` 比较中所用的信息没有被修改<br />⑤、对于任何非空引用值 x，`x.equals(null)` 都应返回 false。<br />下面自定义一个 Person 类，然后重写其equals 方法，比较两个 Person 对象：
```java
public class Person {
    private String pname;
    private int page;

    public Person(){}

    public Person(String pname,int page){
        this.pname = pname;
        this.page = page;
    }
    public int getPage() {
        return page;
    }

    public void setPage(int page) {
        this.page = page;
    }

    public String getPname() {
        return pname;
    }

    public void setPname(String pname) {
        this.pname = pname;
    }
    @Override
    public boolean equals(Object obj) {
        if(this == obj){//引用相等那么两个对象当然相等
            return true;
        }
        if(obj == null || !(obj instanceof  Person)){//对象为空或者不是Person类的实例
            return false;
        }
        Person otherPerson = (Person)obj;
        if(otherPerson.getPname().equals(this.getPname()) && otherPerson.getPage()==this.getPage()){
            return true;
        }
        return false;
    }

    public static void main(String[] args) {
        Person p1 = new Person("Tom",21);
        Person p2 = new Person("Marry",20);
        System.out.println(p1==p2);//false
        System.out.println(p1.equals(p2));//false

        Person p3 = new Person("Tom",21);
        System.out.println(p1.equals(p3));//true
    }

}
```
通过重写 `equals` 方法，自定义两个对象相等的标尺为Person对象的两个属性都相等，则对象相等，否则不相等。如果不重写 equals 方法，那么始终是调用 Object 类的equals 方法，也就是用 == 比较两个对象在栈内存中的引用地址是否相等。<br />这时候有个Person 类的子类 Man，也重写了 equals 方法：
```java
public class Man extends Person{
    private String sex;

    public Man(String pname,int page,String sex){
        super(pname,page);
        this.sex = sex;
    }
    @Override
    public boolean equals(Object obj) {
        if(!super.equals(obj)){
            return false;
        }
        if(obj == null || !(obj instanceof  Man)){//对象为空或者不是Person类的实例
            return false;
        }
        Man man = (Man) obj;
        return sex.equals(man.sex);
    }

    public static void main(String[] args) {
        Person p = new Person("Tom",22);
        Man m = new Man("Tom",22,"男");

        System.out.println(p.equals(m));//true
        System.out.println(m.equals(p));//false
    }
}
```
通过打印结果可以发现 `person.equals(man)`得到的结果是 true，而`man.equals(person)`得到的结果却是`false`，这显然是不正确的。<br />问题出现在 `instanceof`关键字上。<br />Man 是 Person 的子类，`person instanceof Man` 结果当然是false。这违反了上面说的对称性。<br />实际上用 `instanceof` 关键字是做不到对称性的要求的。这里推荐做法是用 `getClass()`方法取代 `instanceof` 运算符。`getClass()` 关键字也是 Object 类中的一个方法，作用是返回一个对象的运行时类，下面会详细讲解。<br />那么 Person 类中的 `equals` 方法为
```java
public boolean equals(Object obj) {
    if(this == obj){//引用相等那么两个对象当然相等
        return true;
    }
    if(obj == null || (getClass() != obj.getClass())){//对象为空或者不是Person类的实例
        return false;
    }
    Person otherPerson = (Person)obj;
    if(otherPerson.getPname().equals(this.getPname()) && otherPerson.getPage()==this.getPage()){
        return true;
    }
    return false;
}
```
打印结果 `person.equals(man)`得到的结果是 `false`，`man.equals(person)`得到的结果也是`false`，满足对称性。<br />注意：使用 `getClass` 不是绝对的，要根据情况而定，毕竟定义对象是否相等的标准是由程序员自己定义的。而且使用 `getClass` 不符合多态的定义，比如 `AbstractSet` 抽象类，它有两个子类 `TreeSet` 和 `HashSet`，他们分别使用不同的算法实现查找集合的操作，但无论集合采用哪种方式实现，都需要拥有对两个集合进行比较的功能，如果使用 getClass 实现equals方法的重写，那么就不能在两个不同子类的对象进行相等的比较。而且集合类比较特殊，其子类是不需要自定义相等的概念的。<br />所以什么时候使用 `instanceof` 运算符，什么时候使用 `getClass()` 有如下建议：<br />①、如果子类能够拥有自己的相等概念，则对称性需求将强制采用 `getClass` 进行检测。<br />②、如果有超类决定相等的概念，那么就可以使用 `instanceof` 进行检测，这样可以在不同的子类的对象之间进行相等的比较。<br />下面给出一个完美的 `equals` 方法的建议：<br />1、显示参数命名为 otherObject，稍后会将它转换成另一个叫做 other 的变量。<br />2、判断比较的两个对象引用是否相等，如果引用相等那么表示是同一个对象，那么当然相等<br />3、如果 otherObject 为 null，直接返回false，表示不相等<br />4、比较 this 和 otherObject 是否是同一个类：如果 equals 的语义在每个子类中有所改变，就使用 getClass 检测；如果所有的子类都有统一的定义，那么使用 `instanceof` 检测<br />5、将 otherObject 转换成对应类的类型变量<br />6、最后对对象的属性进行比较。使用 `==` 比较基本类型，使用 equals 比较对象。如果都相等则返回true，否则返回false。注意如果是在子类中定义equals，则要包含 `super.equals(other)`<br />下面给出 Person 类中完整的 equals 方法的书写：
```java
@Override
public boolean equals(Object otherObject) {
    //1、判断比较的两个对象引用是否相等，如果引用相等那么表示是同一个对象，那么当然相等
    if(this == otherObject){
        return true;
    }
    //2、如果 otherObject 为 null，直接返回false，表示不相等
    if(otherObject == null ){//对象为空或者不是Person类的实例
        return false;
    }
    //3、比较 this 和 otherObject 是否是同一个类（注意下面两个只能使用一种）
    //3.1：如果 equals 的语义在每个子类中所有改变，就使用 getClass 检测
    if(this.getClass() != otherObject.getClass()){
        return false;
    }
    //3.2：如果所有的子类都有统一的定义，那么使用 instanceof 检测
    if(!(otherObject instanceof Person)){
        return false;
    }

    //4、将 otherObject 转换成对应的类类型变量
    Person other = (Person) otherObject;

    //5、最后对对象的属性进行比较。使用 == 比较基本类型，使用 equals 比较对象。如果都相等则返回true，否则返回false
    //   使用 Objects 工具类的 equals 方法防止比较的两个对象有一个为 null而报错，因为 null.equals() 是会抛异常的
    return Objects.equals(this.pname,other.pname) && this.page == other.page;

    //6、注意如果是在子类中定义equals，则要包含 super.equals(other)
    //return super.equals(other) && Objects.equals(this.pname,other.pname) && this.page == other.page;

}
```
请注意，无论何时重写此方法，通常都必须重写`hashCode`方法，以维护hashCode方法的一般约定，该方法声明相等对象必须具有相同的哈希代码。`hashCode` 也是 `Object` 类中的方法，后面会详细讲解。
<a name="qv5uU"></a>
### 5、`getClass` 方法
上面在介绍 equals 方法时，介绍如果 equals 的语义在每个子类中有所改变，那么使用 getClass 检测，为什么这样说呢？<br />`getClass()`在 Object 类中如下，作用是返回对象的运行时类。
```java
public final native Class<?> getClass();
```
这是一个用 `native` 关键字修饰的方法。<br />这里要知道用 `native` 修饰的方法不用考虑，由操作系统实现，该方法的作用是返回一个对象的运行时类，通过这个类对象可以获取该运行时类的相关属性和方法。也就是Java中的反射，各种通用的框架都是利用反射来实现的，这里不做详细的描述。<br />这里详细的介绍 `getClass` 方法返回的是一个对象的运行时类对象，这该怎么理解呢？Java中还有一种这样的用法，通过 类名.class 获取这个类的类对象 ，这两种用法有什么区别呢？<br />父类：Parent.class
```java
public class Parent {}
```
子类：Son.class
```java
public class Son extends Parent{}
```
测试：
```java
@Test
public void testClass(){
    Parent p = new Son();
    System.out.println(p.getClass());
    System.out.println(Parent.class);
}
```
打印结果：<br />![2021-06-26-10-45-24-834885.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624675534284-2bc0e83a-4b12-42de-8992-4f6d2dd29c3c.png#clientId=u137988d1-6d9a-4&from=ui&id=u130debbb&originHeight=56&originWidth=282&originalType=binary&ratio=3&size=1014&status=done&style=shadow&taskId=u5d5afa62-ba8e-477e-8e1b-e619d34012e)<br />结论：class 是一个类的属性，能获取该类编译时的类对象，而 `getClass()` 是一个类的方法，它是获取该类运行时的类对象。<br />还有一个需要大家注意的是，虽然`Object`类中`getClass()` 方法声明是：`public final native Class getClass();`返回的是一个 Class，但是如下是能通过编译的：
```java
Class<? extends String> c = "".getClass();
```
也就是说类型为T的变量getClass方法的返回值类型其实是Class而非getClass方法声明中的Class。<br />这在官方文档中也有说明：[https://docs.oracle.com/javase/8/docs/api/java/lang/Object.html#getClass--](https://docs.oracle.com/javase/8/docs/api/java/lang/Object.html#getClass--)
<a name="J8udj"></a>
### 6、`hashCode` 方法
`hashCode` 在 Object 类中定义如下：
```java
public native int hashCode();
```
这也是一个用 `native` 声明的本地方法，作用是返回对象的散列码，是 int 类型的数值。<br />那么这个方法存在的意义是什么呢?<br />在Java 中有几种集合类，比如 List、Set，还有 Map，List集合一般是存放的元素是有序可重复的，Set 存放的元素则是无序不可重复的，而 Map 集合存放的是键值对。<br />前面说过判断一个元素是否相等可以通过 equals 方法，每增加一个元素，那么就通过 equals 方法判断集合中的每一个元素是否重复，但是如果集合中有10000个元素了，但新加入一个元素时，那就需要进行10000次equals方法的调用，这显然效率很低。<br />于是，Java 的集合设计者就采用了 哈希表 来实现。关于哈希表的数据结构有过介绍。哈希算法也称为散列算法，是将数据依特定算法产生的结果直接指定到一个地址上。这个结果就是由 hashCode 方法产生。这样一来，当集合要添加新的元素时，先调用这个元素的 `hashCode` 方法，就一下子能定位到它应该放置的物理位置上。<br />①、如果这个位置上没有元素，它就可以直接存储在这个位置上，不用再进行任何比较了；<br />②、如果这个位置上已经有元素了，就调用它的equals方法与新元素进行比较，相同的话就不存了；<br />③、不相同的话，也就是发生了Hash key相同导致冲突的情况，那么就在这个Hash key的地方产生一个链表，将所有产生相同HashCode的对象放到这个单链表上去，串在一起（很少出现）。这样一来实际调用equals方法的次数就大大降低了，几乎只需要一两次。 <br />![2021-06-26-10-45-25-006426.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624675549996-17ade3fd-680e-4b07-80e7-0ba729c2e9da.png#clientId=u137988d1-6d9a-4&from=ui&id=ueac82329&originHeight=341&originWidth=618&originalType=binary&ratio=3&size=14045&status=done&style=shadow&taskId=u57d5e95b-ef59-4c91-90d2-f0e24a38f99)<br />这里有 A,B,C,D四个对象，分别通过 hashCode 方法产生了三个值，注意 A 和 B 对象调用 hashCode 产生的值是相同的，即 A.hashCode() = B.hashCode() = 0x001,发生了哈希冲突，这时候由于最先是插入了 A，在插入的B的时候，B 是要插入到 A 所在的位置，而 A 已经插入了，这时候就通过调用 `equals` 方法判断 A 和 B 是否相同，如果相同就不插入 B，如果不同则将 B 插入到 A 后面的位置。所以对于 `equals` 方法和 `hashCode` 方法有如下要求：
<a name="uSEYQ"></a>
#### 6.1 `hashCode` 要求 
①、在程序运行时期间，只要对象的（字段的）变化不会影响`equals`方法的决策结果，那么，在这个期间，无论调用多少次`hashCode`，都必须返回同一个散列码。<br />②、通过`equals`调用返回true 的2个对象的hashCode一定一样。<br />③、通过`equasl`返回`false` 的2个对象的散列码不需要不同，也就是他们的hashCode方法的返回值允许出现相同的情况。<br />因此可以得到如下推论：<br />两个对象相等，其 `hashCode` 一定相同;<br />两个对象不相等，其 `hashCode` 有可能相同;<br />`hashCode` 相同的两个对象，不一定相等;<br />`hashCode` 不相同的两个对象，一定不相等;<br />这四个推论通过上图可以更好的理解。<br />可能会有人疑问，对于不能重复的集合，为什么不直接通过 `hashCode` 对于每个元素都产生唯一的值，如果重复就是相同的值，这样不就不需要调用 `equals` 方法来判断是否相同了吗？ 实际上对于元素不是很多的情况下，直接通过 `hashCode` 产生唯一的索引值，通过这个索引值能直接找到元素，而且还能判断是否相同。比如数据库存储的数据，ID 是有序排列的，能通过 ID 直接找到某个元素，如果新插入的元素 ID 已经有了，那就表示是重复数据，这是很完美的办法。但现实是存储的元素很难有这样的 ID 关键字，也就很难这种实现 `hashCode` 的唯一算法，再者就算能实现，但是产生的 `hashCode` 码是非常大的，这会大的超过 Java 所能表示的范围，很占内存空间，所以也是不予考虑的。
<a name="Rid6f"></a>
#### 6.2 `hashCode` 编写指导
①、不同对象的`hash`码应该尽量不同，避免`hash`冲突，也就是算法获得的元素要尽量均匀分布。<br />②、hash 值是一个 int 类型，在Java中占用 4 个字节，也就是 2的32 次方，要避免溢出。<br />在 JDK 的 `Integer`类，`Float` 类，`String` 类等都重写了 `hashCode` 方法，自定义对象也可以参考这些类来写。<br />下面是 JDK String 类的hashCode 源码：
```java
public int hashCode() {
    int h = hash;
    if (h == 0 && value.length > 0) {
        char val[] = value;

        for (int i = 0; i < value.length; i++) {
            h = 31 * h + val[i];
        }
        hash = h;
    }
    return h;
}
```
再次提醒大家，对于 Map 集合，可以选取Java中的基本类型，还有引用类型 String 作为 key，因为它们都按照规范重写了 `equals` 方法和 `hashCode` 方法。但是如果用自定义对象作为 key，那么一定要覆写 `equals` 方法和 `hashCode` 方法，不然会有意想不到的错误产生。
<a name="upzi6"></a>
### 7、`toString` 方法
该方法在 JDK 的源码如下：
```java
public String toString() {
    return getClass().getName() + "@" + Integer.toHexString(hashCode());
}
```
`getClass().getName()`是返回对象的全类名（包含包名）,`Integer.toHexString(hashCode())` 是以16进制无符号整数形式返回此哈希码的字符串表示形式。<br />打印某个对象时，默认是调用 toString 方法，比如 `System.out.println(person)`。等价于 
```java
System.out.println(person.toString())
```
<a name="LXwzy"></a>
### 8、`notify()`/`notifyAll()`/`wait()`
这是用于多线程之间的通信方法，在后面讲解多线程会详细描述，这里就不做讲解了。
```java
protected void finalize() throws Throwable { }
```
该方法用于垃圾回收，一般由 JVM 自动调用，一般不需要程序员去手动调用该方法。后面再讲解 JVM 的时候会详细展开描述。
<a name="SyHyN"></a>
### 10、`registerNatives` 方法
该方法在 Object 类中定义如下：
```java
private static native void registerNatives();
```
这是一个本地方法，在 native 介绍 中知道一个类定义了本地方法后，想要调用操作系统的实现，必须还要装载本地库，但是在 `Object.class` 类中具有很多本地方法，但是却没有看到本地库的载入代码。而且这是用 `private` 关键字声明的，在类外面根本调用不了，接着往下看关于这个方法的类似源码：
```java
static {
    registerNatives();
}
```
看到上面的代码，这就明白了吧。静态代码块就是一个类在初始化过程中必定会执行的内容，所以在类加载的时候是会执行该方法的，通过该方法来注册本地方法。
