Java<br />常用类概述：

1. 内部类
2. Object类
3. 包装类
4. 数学类
5. 时间类
6. 字符串
7. StringBuilder和StringBuffer
8. DecimalFormat
<a name="UxL2o"></a>
## 一、内部类
概念：在一个类内部再定义一个完整的类。<br />一般情况下类与类之间是相互独立的，内部类的意思就是打破这种独立思想，让一个类成为另一个类的内部信息，和成员变量、成员方法同等级别。<br />内部类的好处：<br />把一个类写在外面和写在里面最终达到的结果都一样，那为什么还要使用内部类，岂不是多此一举吗？<br />采用内部类这种技术，可以隐藏细节和内部结构，封装性更好，让程序的结构更加合理！如果类很多且都暴露在外面，那么类与类之间的调用就会十分繁琐！
<a name="DMAEF"></a>
### 内部类的分类：
<a name="mPNsq"></a>
#### 1、成员内部类（非静态内部类）
【参考代码】
```java
package NeiBuLei;
public class OuterClass {
    //成员变量
    private String OuterName;
    //成员方法
    public void display(){
        System.out.println("这是外部类方法！");
        System.out.println(OuterName);
    }
    //内部类
    public class InnerClass{
        //成员变量
        private String InnerNme;
        //构造方法
        public InnerClass() {
            InnerNme = "Inner Class";
        }
        //成员方法
        public void display(){
            System.out.println("这是内部类方法！");
            System.out.println(InnerNme);
        }
    }
    // 主方法
    public static void main(String[] args) {
        OuterClass outerClass = new OuterClass();
        outerClass.display();//这是外部类方法！null

        // 这个类是内部类，已经不是独立的类了，因此不能像外部类一样直接创建！
        //InnerClass innerClass = new InnerClass(); 行不通
        OuterClass.InnerClass innerClass = outerClass.new InnerClass();// 同成员方法/变量 只是加了个前缀
        innerClass.display();// 这是内部类方法！
    }
}
```
输出结果：
```java
这是外部类方法！
null
这是内部类方法！
Inner Class
```
总结：成员内部类（非静态内部类）的使用就是将内部类作为外部类的的一个成员变量/成员方法来使用，所以必须依赖于外部类的对象才能调用，用法和成员变量/成员方法一致！
<a name="Znp1O"></a>
#### 2、局部内部类
局部内部类：基本的内部类还可以在一个方法体中定义。
```java
package NeiBuLei;
public class OuterClass {
    //成员变量
    private String OuterName;
    //成员方法
    public void display(){
        class InnerClass {
            public void print(){
                System.out.println("这是一个局部内部类方法！");
            }
        }
        InnerClass innerClass = new InnerClass();
        innerClass.print();
    }

    // 主方法
    public static void main(String[] args) {
        OuterClass outerClass = new OuterClass();
        outerClass.display();
    }
}
```
<a name="f9XP2"></a>
#### 3、静态内部类
静态内部类的构造不需要依赖于外部类对象，类中的静态组件都不需要依赖于任何对象，可以直接通过类本身进行构造。
```java
package NeiBuLei;
public class OuterClass {
    //成员变量
    private String OuterName;
    //成员方法
    public void display(){
        System.out.println("这是外部类方法！");
        System.out.println(OuterName);
    }
    //静态内部类
    public static class InnerClass{
        private String InnerName;
        public InnerClass() {
            InnerName = "Inner Class";
        }

        //成员方法
        public void display(){
            System.out.println("这是静态内部类方法！");
            System.out.println(InnerName);
        }
    }

    // 主方法
    public static void main(String[] args) {
        OuterClass outerClass = new OuterClass();
        outerClass.display();
        // 静态内部类的构造不依赖与外部类，可以直接通过类本身进行构造！
        InnerClass innerClass = new InnerClass();
        innerClass.display();
    }
}
```
输出结果：
```java
这是外部类方法！
null
这是静态内部类方法！
Inner Class
```
<a name="w6Hbb"></a>
#### 4、匿名内部类
匿名内部类：没有名字的内部类。<br />匿名内部类主要应用与接口的实现！<br />接口：
```java
package NeiBuLei;
public interface MyInterface {
    public void test();
}
```
实现类：
```java
package NeiBuLei;
public class MyImplement implements MyInterface{
    @Override
    public void test() {
        System.out.println("test");
    }
}
```
匿名内部类的使用：
```java
package NeiBuLei;
public class Test {
    public static void main(String[] args) {        
        //实现类
        MyInterface myInterface = new MyImplement();
        myInterface.test();
        
        //匿名内部类
        MyInterface myInterface1 = new MyInterface() { // 接口是不能new的，这里new的是接口的实现类（和实现类是一样的（同一个东西）,没有实例而已）
            @Override
            public void test() {
                System.out.println("test");
            }
        };
        myInterface.test();
        /**
         * 最终两种实现方式的结果都是一样的！
         */
    }
}
```
匿名内部类的好处：<br />定义接口之后，它的实现类不需要去单独创建一个文件去写它的实现，可以把这个实现类的操作写到调用的地方就可以了！写起来更加简洁、方便。<br />匿名内部类的缺点：<br />耦合度太高了！
<a name="au0ce"></a>
## 二、Object类
jdk中文在线文档：Java 8 中文版 - 在线API中文手册 - 码工具 (matools.com)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640311293386-176853c2-c6e5-416a-b67c-de073a05c5c3.webp#averageHue=%23e0e4e2&clientId=u7c1f4330-b7a6-4&from=paste&id=uc6735b6e&originHeight=651&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3a9bc073-f846-4f9a-9e03-2e20c4ad8ce&title=)
<a name="l60jA"></a>
### Object类常用方法：
<a name="ameDT"></a>
#### 1、equals方法
`==`与`equals`的对比【面试题】+ jdk查看原码<br />==是一个比较运算符

1. ==：既可以判断基本类型，又可以判断引用类型
2. ==：如果判断的是基本类型，判断的是值是否相等。 
```java
//==： 如果判断的是基本类型，判断的是 值 是否相等
int x1 = 10;
int x2 = 10;
double x3 = 10.0;
System.out.println(x1 == x2);//true
System.out.println(x1 == x3);//true
```

3. ==：如果判断的是引用类型，判断的是地址是否相等，即判断是不是同一个对象
```java
package Equals;
public class Test01 {
    public static void main(String[] args) {
        //==： 如果判断的是引用类型，判断的是地址是否相等，即判断是不是同一个对象
        A a = new A();
        A b = a;
        A c = b;
        System.out.println(a==c);// ? true
        System.out.println(b==c);// true
        B obj = a;
        System.out.println(obj==c);// true
    }
}

class B{}
class A extends B{}
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640311293370-2f1066c3-1097-4150-a219-10261d1dd343.webp#averageHue=%23f7f7f1&clientId=u7c1f4330-b7a6-4&from=paste&id=u63925e64&originHeight=456&originWidth=613&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u689bfcf0-1efa-4129-ab7b-cbe93d1565d&title=)

1. equals方法是Object类中的方法，只能判断引用类型。

idea查看Jdk原码：鼠标光标放在要查看的方法上，直接输入ctrl + b<br />查看某个类所有方法：ctrl + F12

1. 默认判断的是地址是否相等，子类(Object类是所有类的父类)往往重写该方法，用于判断内容是否相等。
```java
Object类 equals()方法原码

//默认判断地址是否一样
public boolean equals(Object obj) {
    return (this == obj);
}
```
子类往往重写该方法，用于判断内容是否相等  String类中的`equals()`方法原码（重写了父类equals()方法）
```java
public boolean equals(Object anObject) {
    if (this == anObject) { // 如果是同一个对象(地址相同)
        return true; // 返回true
    }
    if (anObject instanceof String) { // 判断类型
        String anotherString = (String)anObject; // 向下转型
        int n = value.length;
        if (n == anotherString.value.length) { // 如果长度相同
            char v1[] = value;
            char v2[] = anotherString.value;
            int i = 0;
            while (n-- != 0) { // 比较每一个字符
                if (v1[i] != v2[i])
                    return false;
                i++;
            }
            return true; // 如果两个字符串每一个字符都相同，则返回true
        }
    }
    return false;
}
```
在看个例子<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640311293361-5934dc55-b487-4d3e-a496-e1902e65c7e1.webp#averageHue=%23f8fbf8&clientId=u7c1f4330-b7a6-4&from=paste&id=u078e5c9f&originHeight=467&originWidth=1046&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u93cd119a-d3e1-48e3-b05d-f3524a29029&title=)<br />【小练习】<br />写出输出结果：
```java
package Equals;
public class EqualsTest01 {
    public static void main(String[] args) {
        Person p1 = new Person();
        p1.name = "tom";
        Person p2 = new Person();
        p2.name = "tom";
        System.out.println(p1 == p2);// 引用类型——判断是否为同一个对象（地址）
        System.out.println(p1.name.equals(p2.name));// p.name是String类型，重写了equals()方法——判断内容是否一样
        System.out.println(p1.equals(p2));//p1,p2属于Person类，该类并没有重写equals()方法（继承父类equals()方法，即判断地址）

        String s1 = new String("abc");
        String s2 = new String("abc");

        System.out.println(s1.equals(s2));
        System.out.println(s1 == s2);
    }
}

class Person{
    public String name;
}
```
输出结果：
```java
false
true
false
true
false
```
<a name="e9fXv"></a>
#### 2、hashCode方法
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640311293604-893730f1-0f17-4e75-8c79-0efee4a0bbf5.webp#averageHue=%23ebeae7&clientId=u7c1f4330-b7a6-4&from=paste&id=u77b83e07&originHeight=330&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u851df5b0-9bc6-499b-9d19-f06efbc4b87&title=)<br />小结：（可以当作地址来看但它本质上不是地址）

1. 提高具有哈希结构的容器的效率
2. 两个引用，如果指向的是同一个对象，则哈希值肯定一样
3. 两个引用，如果指向的是不同对象，则哈希值是不一样的
4. 哈希值主要根据地址号来！不能将哈希值完全等价于地址
5. 在后面的集合中hashCode如果需要的话，也会重写
```java
package hashCode;
public class HashCode {
    public static void main(String[] args) {
        AA aa = new AA();
        AA aa2 = new AA();
        AA aa3 = aa;
        System.out.println("aa.hashCode()="+ aa.hashCode());
        System.out.println("aa2.hashCode()="+ aa2.hashCode());
        System.out.println("aa3.hashCode()="+ aa3.hashCode());
    }
}

class AA{}
```
```java
aa.hashCode()=460141958
aa2.hashCode()=1163157884
aa3.hashCode()=460141958
```
<a name="u95Lp"></a>
#### 3、toString方法
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640311293767-2a000ba4-5313-4899-b675-c713e1874a44.webp#averageHue=%23dfe2df&clientId=u7c1f4330-b7a6-4&from=paste&id=ub1532f86&originHeight=291&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u875ec7b6-5ba6-446b-ab6e-4f04e5970b6&title=)<br />基本介绍：<br />默认返回：全类名 + @ + 哈希值的十六进制
```java
/*
    Object toString()原码
    //(1)getClass().getName() 类的全类名（包名+类名）
    //(2)Integer.toHexString(hashCode()) 将hashCode的值转成16进制字符串
    public String toString() {
        return getClass().getName() + "@" + Integer.toHexString(hashCode());
    }
 */
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640311293906-fbf8135d-5e72-46ac-a753-947116aa8ec8.webp#averageHue=%232f332f&clientId=u7c1f4330-b7a6-4&from=paste&id=ub9d63bf6&originHeight=122&originWidth=1040&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue0fa0e68-0d0a-4c3e-adb0-a6dd70f828d&title=)<br />子类往往重写toString方法，用于返回对象的属性信息（快捷键：alt + insert），当然也可以自己定制。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640311293949-54eefce0-305b-4dcb-b5c1-75177503cb51.webp#averageHue=%23373c3a&clientId=u7c1f4330-b7a6-4&from=paste&id=ud89fde0c&originHeight=443&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub529d416-0baa-4c79-afb9-6a3054d2dfa&title=)<br />当输出一个对象时，`toString()`方法会被默认调用<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640311294021-03cb0343-90a7-4e9c-b04e-e72a1c33681e.webp#averageHue=%233f3d38&clientId=u7c1f4330-b7a6-4&from=paste&id=uf4facf6c&originHeight=79&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u27fa9abd-80d5-417e-8753-ce8142b0a7f&title=)
<a name="LXf7R"></a>
#### 4、`finzlize`方法
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640311294035-7d718a70-7d76-4b67-91c3-56168e4edd33.webp#averageHue=%23d2d4cf&clientId=u7c1f4330-b7a6-4&from=paste&id=ue7957a1a&originHeight=375&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u81f8c2d9-4060-4396-b3bc-dc0c2223176&title=)<br />`finzlize`方法：当垃圾收集确定不再有对该对象的引用时，垃圾收集器在对象上调用该方法。

1. 当对象被回收时，系统自动调用该对象的`finzlize`方法。子类可以重写该方法，做一些释放资源的操作
2. 什么时候被回收：当某个对象没有任何引用时，则jvm就认为这个对象是一个垃圾对象，JVM在垃圾回收时就会销毁该对象，在销毁该对象前，会先调用`finzlize`方法。

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640311294285-fda8d6c5-3723-418f-9b3d-9d01a44a7ed2.webp#averageHue=%23444844&clientId=u7c1f4330-b7a6-4&from=paste&id=ub775b44d&originHeight=668&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1e239f09-eb0d-498c-bcae-53b467bffb7&title=)

1. 垃圾回收机制的调用，是由系统来决定（即有自己的GC算法），也可以通过`System.gc()`主动触发垃圾回收机制。

注：在实际开发中，几乎不会用`finzlize`方法，更多的是为了应付面试
<a name="UxD1t"></a>
## 三、包装类
<a name="faf9c"></a>
### 1、基本数据类型以及对应的包装类：
byte -> Byte<br />short -> Short<br />int -> Integer<br />long -> Long<br />float -> Float<br />double -> Double<br />char -> Character<br />boolean -> Boolean<br />这些类都在java.lang包
<a name="lwZbk"></a>
### 2、包装类的意义：

1. 让基本数据类型有面向对象的特征
2. 封装了字符串转化成基本数据类型的方法（重点）
<a name="YN9fI"></a>
### 3、包装类常用方法：

1. `Integer.parseInt()`
2. `Long.paseLong()`
3. `Double.parseDouble()`

【参考代码】
```java
public class Test {
    public static void main(String[] args) {

        //        Integer i = new Integer(10);// 创建包装类对象
        //        Integer ii = 10; // 自动打包
        //        System.out.println(i+10); // 在使用上，int 和Integer 其实没有区别，可以互相使用
        //        System.out.println(ii+10);
        //        int j = ii;// 自动解包
        //        System.out.println(j+100);

        String a = "12";
        String b = "34";
        System.out.println(a+b); // 1234
        // 转型:
        // 字符串转成int的唯一方案
        int c = Integer.parseInt(a);
        int d = Integer.parseInt(b);
        System.out.println(c+d); // 46

        // 字符串转成double类型
        String e = "1.25";
        double f = Double.parseDouble(e);
        System.out.println(f*6); // 7.5

        // 转成long类型
        long l = Long.parseLong("1234567");
        System.out.println(l);
    }
}
```
<a name="XN71I"></a>
## 四、数学类
数学类的方法都是静态方法，可以直接引用——Math.方法();
<a name="wTxwU"></a>
### 常用数学类方法：

1. `abs()`：获取绝对值
2. `max()`：求最大值
3. `min()`：求最小值
4. `pow()`：求次幂
5. `round()`：四舍五入
6. `sqrt()`：求平方根
<a name="jjeeO"></a>
## 五、时间类
<a name="svGAo"></a>
### Java常用时间类：

1. Date 日期类
2. Calendar 日历类
3. SimpleDateFormat 格式化时间类

Date和Calendar类 在java.util包中<br />SimpleDateFormat类 在java.text包
<a name="J81T2"></a>
#### 1、Date 日期
【1】`new Date()` 可以获取到系统时间<br />【2】`getTime()` 能获取到时间的long形式，可以用来计算时间差<br />`getTime()`——获取计算机底层存储的数字，返回一个数字用来表示时间，这个数字的类型long，单位为毫秒。<br />【参考代码】
```java
import java.util.Date;
public class Test {
    public static void main(String[] args) {
        Date d = new Date();
        System.out.println(d); // 系统时间
        //get...()——获取年月日.....
        System.out.println(d.getYear()+1900); // 从1900年开始算的
        System.out.println(d.getMonth()+1); // 月份从0开始计算
        System.out.println(d.getDate()); // 天数
        System.out.println(d.getHours());// 小时

        //getTime()——获取到时间的毫秒形式 返回的是long
        System.out.println(d.getTime());
    }
}
```
<a name="gsOtU"></a>
#### 2、Calendar 日历
【1】`get()` 获取到时间的某一部分<br />【2】`set()` 设置时间 - -> 计算时间：系统已经设置好了，不用担心二月有多少天等问题，计算时间十分方便<br />注：Calendar 日历类是抽象类，因此不可以去new对象。虽然抽象类不能创建对象，但是jdk官方提供了一个实例对象的操作：
```java
Calendar rightNow = Calendar.getInstance();
```
通过这条代码就是直接造了一个Calender的对象<br />【参考代码】：`get()` 获取到时间的某一部分：
```java
package date;
import java.util.Calendar;
public class TestCalendar {
    public static void main(String[] args) {
        Calendar cal = Calendar.getInstance();
        //        System.out.println(cal);

        /*
 		假设当天：
        2021
        8
        10
         */
        cal.set(Calendar.DATE,cal.get(Calendar.DATE)+31); // 计算时间(这里用天数计算的) 

        // 获取Calendar创建的对象里的所有内容
        System.out.println(cal.get(Calendar.YEAR)); // 2021 年
        System.out.println(cal.get(Calendar.MONTH)+1); // 月份：从0开始的  结果：为10月
        System.out.println(cal.get(Calendar.DATE)); // 日
        System.out.println(cal.get(Calendar.HOUR_OF_DAY));// 小时
        System.out.println(cal.get(Calendar.MINUTE));
        System.out.println(cal.get(Calendar.SECOND));
    }
}
```
【参考代码】：`set()` 设置时间 - -> 计算时间：<br />注：`cal.setTime(d);` 把Date转化成Calendar
```java
package date;
import java.util.Calendar;
import java.util.Date;
public class TestCalendar {
    public static void main(String[] args) {
        Date d = new Date();
        Calendar cal = Calendar.getInstance();
        cal.setTime(d);// 把Date转化成Calendar
        System.out.println(cal);
        System.out.println(cal.get(Calendar.YEAR)); //  年
        System.out.println(cal.get(Calendar.MONTH)+1); // 月份：从0开始的
        System.out.println(cal.get(Calendar.DATE)); // 日        
    }
}
```
<a name="OHmuT"></a>
#### 3、`SimpleDateFormat`格式化时间
Date，Calendar通过引用也可以进行时间的格式化，但比较繁琐，而`SimpleDateFormat`类是专门格式化时间的工具类，它在java.text包中。<br />【时间格式】：`yyyy-MM-dd HH:mm:ss`<br />`SimpleDateFormat`类有两大常用方法：
<a name="Gt5lZ"></a>
##### 【1】`format(Date)`：
`format(Date)` 把时间转成字符串，字符串的格式为`SimpleDateFormat`类定义对象时设置的时间格式<br />【参考代码】
```java
package Simple;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.logging.SimpleFormatter;
public class Test {
    public static void main(String[] args) {
        Date d = new Date();
        System.out.println(d); //Thu Aug 12 08:40:08 CST 2021  不美观

  //                  设置格式化时间的模式，我们常用yyyy-MM-dd HH:mm:ss这个模式
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");// 时间格式
        String s = sdf.format(d); // 格式化时间
        System.out.println(s); // 2021-08-12 08:45:09
    }
}
```
<a name="UtB4S"></a>
##### 【2】`parse(String)`：
`parse(String)` 把字符串转化成时间<br />【参考代码】
```java
package Simple;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Scanner;

public class Test2 {
    public static void main(String[] args) throws ParseException {
        Scanner sc = new Scanner(System.in);
        System.out.println("请输入一个时间(yyyy-MM-dd HH:mm:ss)：");
        String s = sc.nextLine();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
       Date d =  sdf.parse(s); // 把字符串转成时间
        System.out.println(d);        
        /*
        请输入一个时间(yyyy-MM-dd HH:mm:ss)：
        2021-08-12 12:25:21
        Thu Aug 12 12:25:21 CST 2021
         */
    }
}
```
注：由于用户输入的字符串不一定是要求的格式，可能是任何东西，想把它们转成时间是不可能的，不可能把一个人转成时间，因此存在着很大的风险未处理(异常: java.text.ParseException)，为此需要处理异常。
<a name="xxaRE"></a>
#### 4、计算时间差
计算思路：

1. 格式化时间
2. 先将字符串转化成long类型时间
3. 计算毫秒级别时间差，取绝对值
4. 毫秒级别时间差转成秒级别
5. 秒级别时间差转成分钟级别
6. 分钟级别时间差转化显示成xx小时xx分钟

【参考代码】
```java
package Simple;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
public class TestDiff {
    public static void main(String[] args) throws ParseException {
        String s1 = "2021-08-12 12:00:00"; // 开始时间
        String s2 = "2021-08-12 14:35:00"; // 结束时间
        //格式化时间
        SimpleDateFormat sdf = new SimpleDateFormat("YYYY-MM-dd HH:mm:ss");
        //将字符串转成时间形式
        Date d1 = sdf.parse(s1);
        Date d2 = sdf.parse(s2);

        //计算时间差:先要获取时间毫秒形式（long类型） 再做差
        long long1 = d1.getTime();
        long long2 = d2.getTime();
        long diffTime = Math.abs(long1 - long2);

        // 秒级别时间差
        long diffSec = diffTime / 1000;

        // 分级别时间差
        long diffMin = diffSec / 60;

        //显示 xx小时xx分钟
        long displayHours = diffMin / 60; // 小时
        long displayMin = diffMin % 60; //分钟
        System.out.println("您学习的时长为："+displayHours+"小时"+displayMin+"分钟");
    }
}
```
<a name="ZFxoS"></a>
## 六、String类
<a name="LGbAn"></a>
### 字符串类常用方法
方法汇总：

| 修饰符和返回值的类型 | 方法名 | 解释 |
| --- | --- | --- |
| char | `charAt()` | 获取某个位置的字符 |
| String | `concat()` | 字符串的拼接。一般字符串拼接直接相加就好了 |
| boolean | `contains()` | 判断原字符串是否含有xxx字符串，常用于子串的判断 |
| boolean | `endsWith()` | 判断原字符串是否以xxx字符串结尾 |
| boolean | `startsWith()` | 判断原字符串是否以xxx字符串开头 |
| boolean | `equals()` | 判断两边字符串内容是否相同；==判断地址是否相同 |
| boolean | `equalsIgnoreCase()` | 忽略大小写判断两边字符串的内容是否一样 |
| int | `indexOf()` | 计算给出字符串第一个出现的位置 |
| int | `LastindexOf()` | 计算给出字符串最后一个出现的位置 |
| int | `length()` | 计算字符串的长度 |
| String | `replace()` | 字符串内容的替换 |
| String[] | `split()` | 字符串切割，最终结果是一个字符串数组 |
| String | `substring()` | 字符串截取，左闭右开：[ ) |
| String | `trim()` | 去掉字符串左右两边的空格，中间的不行 |
| static String | `valueOf()` | 官方：基本数据类型转为字符串操作；直接：变量 + "" |

注：字符串是一个不可变的类型（`final`类），几乎所有的字符串操作都会返回一个新字符串而不是在原有基础上进行修改。<br />【示例代码】
```java
public class Test {
    public static void main(String[] args) {
        String s = "我的名字叫李华";        
        s.concat("hhh"); // 在字符串s上拼接，拼接hhh
        System.out.println(s);// 我的名字叫李华
        //字符串是不可变的数据类型
        //几乎所有的字符串操作都会返回一个新字符串
        String s1 = s.concat("hhh"); // 在字符串s上拼接，拼接hhh
        System.out.println(s1);//我的名字叫李华hhh        
        
        String str1 = "李华喜欢看罗老师的视频";
        str1.replace("李华","张三");
        System.out.println(str3); //李华喜欢看罗老师的视频  并没有替换 字符串是不变的str1还是str1

        String str2 = str1.replace("李华","张三");//几乎所有的字符串操作都会返回一个新字符串 新串要用新变量接
        System.out.println(str2);//张三喜欢看罗老师的视频         
   }
}
```
```java
package String;
import java.util.Scanner;
public class Test {
    public static void main(String[] args) {
        String s = "我的名字叫李华";
        System.out.println(s.charAt(0)); // 获取第0个位置的字符
        s.concat("hhh");
        System.out.println(s);// 我的名字叫李华
        //字符串是不可变的数据类型
        //几乎所有的字符串操作都会返回一个新字符串
        String s1 = s.concat("hhh"); // 在字符串s上拼接，拼接hhh
        System.out.println(s1);//我的名字叫李华hhh
        System.out.println(s.contains("李华")); //true
        System.out.println(s.contains("牛津")); //false
        System.out.println("邀请李华来参加英语沙龙活动".endsWith("活动"));//true 判断是否以xxx为结尾
        System.out.println("邀请李华来参加英语沙龙活动".startsWith("李华"));//false 判断是否以xxx开头

        // equals字符串内容是否相同

        // 接受邀请参加活动的李华到现场后要输入验证码
//        String yanZhengMa = "AAkm";
//
//        Scanner sc = new Scanner(System.in);
//
//        System.out.println("请输入验证码("+yanZhengMa+")");
//
//        String userInput = sc.nextLine();
//
//        if(yanZhengMa.equalsIgnoreCase("aakm")){  // 忽略大小写判断两边的内容是否一样
//            System.out.println("欢迎参加英语沙龙活动！");
//        }else{
//            System.out.println("您未受到邀请，请现场报名！");
//        }

//        String str = "李华玩得很开心！";
//        System.out.println(str.indexOf("开心"));// 5 计算给出字符串第一个出现的位置

        String str2 = "李华成绩很好";
        System.out.println(str2.length()); // 6 计算字符串的长度

        String str3 = "李华喜欢看罗老师的视频";
        str3.replace("李华","张三");
        System.out.println(str3); //李华喜欢看罗老师的视频  并没有替换 字符串是不变的str3还是str3

        String str4 = str3.replace("李华","张三");//几乎所有的字符串操作都会返回一个新字符串 新串要用新变量接
        System.out.println(str4);//张三喜欢看罗老师的视频
        String str5 = "哈哈_呵呵_嘻嘻_噢no";
        String[] ss = str5.split("_");//切割
        System.out.println(ss[0]);//哈哈
        System.out.println(ss[1]);//哈哈
        System.out.println(ss[2]);//嘻嘻
        System.out.println(ss[3]);//噢no

        String str6 = "今天天气不错";
        System.out.println(str6.substring(2,4));//天气 字符串截取 [ ) 左闭右开，右边取不到

        String str7 ="     哈    哈      ";
        System.out.println(str7.trim());// 去掉左右两边的空格
        int i = 10;
        System.out.println(String.valueOf(i)); // 基本数据类型转为字符串
        System.out.println(i+""); // 野路子
    }
}
```
<a name="tbtIz"></a>
## 七、StringBuilder和StringBuffer
<a name="CRktd"></a>
### String类的缺点：
String 是一个不可变的数据类型，每每拼接都会产生一个新的字符串，那么内存迟早会被这些拼接的字符串塞满。
<a name="GBxSz"></a>
### String类和StringBuilder和StringBuffer类的区别：
StringBuilder和StringBuffer：可变的字符串，不产生新对象，比较省内存，当进行大量的字符串拼接时建议使用StringBuffer和StringBuilder，但它们两个一些方法的实现几乎跟String一样。
<a name="fENxM"></a>
### StringBuffer和StringBuilder类：
【相似点】<br />两者用法一模一样，可以认为是一个类<br />【区别】

1. StringBuffer线程安全，StringBuilder非线程安全。
2. StringBuilder相比于StringBuffer有速度优势，多数情况下建议使用StringBuilder类，但当被要求线程安全时必须使用StringBuffer类

字符串拼接方法：`append()`方法<br />StringBuffer和StringBuilder 转成String类 ：
```java
StringBuilder sb = new StringBuilder("猫喜欢吃鱼");
String s = sb.toString();
```
【参考代码】
```java
package String;
public class TestStringBuilder {
    public static void main(String[] args) {
        StringBuilder sb = new StringBuilder();// 一个空的字符串""
        StringBuilder sb2 = new StringBuilder("猫喜欢吃鱼");
        System.out.println(sb2);// 猫喜欢吃鱼

        sb2.append("，狗也喜欢吃鱼");
        System.out.println(sb2);// 追加  猫喜欢吃鱼，狗也喜欢吃鱼

        sb2.insert(1,"哈哈哈");
        System.out.println(sb2); //猫哈哈哈喜欢吃鱼，狗也喜欢吃鱼

        // 上述的操作huan'c
        // 把StringBuilder转化成String
        String s = sb2.toString();
        System.out.println(s); //猫哈哈哈喜欢吃鱼，狗也喜欢吃鱼
        // 上述操作都可以将StringBuilder换成StringBuffer，结果一样
    }
}
```
<a name="VU5L4"></a>
## 八、DecimalFormat
DecimalFormat：对小数进行格式化，保留几位小数。与格式化时间联想记。<br />`.` 表示小数点<br />`0`和`#` 表示数位，保留几位就几个`0`或者`#`<br />【参考代码】
```java
import java.text.DecimalFormat;
import java.util.Scanner;
public class Test {
    public static void main(String[] args) {
        double d= 10/3.0;
        System.out.println(d);//3.3333333333333335
        
        // . 表示小数点
        // 0和#表示数字

        // 保留两位小数                        格式
        DecimalFormat df = new DecimalFormat(".00"); // 或者.##
        String s = df.format(d); // 把 d 转成上面设置的格式
        System.out.println(s);//3.33
   }
}
```
