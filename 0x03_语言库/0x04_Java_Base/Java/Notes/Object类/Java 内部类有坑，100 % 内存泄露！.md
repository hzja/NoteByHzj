Java<br />分享一种，Java内部类使用不当导致的内存泄露问题，最终导致内存溢出！希望能够帮助到大家！
<a name="FpZMi"></a>
## 1、简介
<a name="nUXOd"></a>
### 说明
本文介绍 Java 内部类持有外部类导致内存泄露的原因以及其解决方案。
<a name="mcIA6"></a>
### 为什么内部类持有外部类会导致内存泄露？
非静态内部类会持有外部类，如果有地方引用了这个非静态内部类，会导致外部类也被引用，垃圾回收时无法回收这个外部类（即使外部类已经没有其他地方在使用了）。
<a name="riMYj"></a>
### 解决方案

- 不要让其他的地方持有这个非静态内部类的引用，直接在这个非静态内部类执行业务。
- 将非静态内部类改为静态内部类。内部类改为静态的之后，它所引用的对象或属性也必须是静态的，所以静态内部类无法获得外部对象的引用，只能从 JVM 的 Method Area（方法区）获取到static类型的引用。
<a name="ak9GO"></a>
## 2、为什么要持有外部类
Java 语言中，非静态内部类的主要作用有两个：

- 当内部类只在外部类中使用时，匿名内部类可以让外部不知道它的存在，从而减少了代码的维护工作。
- 当内部类持有外部类时，它就可以直接使用外部类中的变量了，这样可以很方便的完成调用，如下代码所示：
```java
package org.example.a;

class Outer{
    private String outerName = "Tony";

    class Inner{
        private String name;

        public Inner() {
            this.name = outerName;
        }
    }

    Inner createInner() {
        return new Inner();
    }
}

public class Demo {
    public static void main(String[] args) {
        Outer.Inner inner = new Outer().createInner();
        System.out.println(inner);
    }
}
```
但是，静态内部类就无法持有外部类和其非静态字段了。<br />比如下边这样就会报错：
```java
package org.example.a;

class Outer{
    private String outerName = "Tony";

    static class Inner{
        private String name;

        public Inner() {
            this.name = outerName;
        }
    }

    Inner createInner() {
        return new Inner();
    }
}

public class Demo {
    public static void main(String[] args) {
        Outer.Inner inner = new Outer().createInner();
        System.out.println(inner);
    }
}
```
报错：<br />![2022-12-19-19-14-08.419387600.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1671449037868-bbf42c07-6fdb-455e-a637-6255bdf7cdde.png#averageHue=%23c6e0ca&clientId=u941c473b-0826-4&from=ui&id=ue5c402f3&originHeight=498&originWidth=959&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1435631&status=done&style=none&taskId=uf675e975-280a-4227-ae2a-a2d7fad2874&title=)
<a name="zfdex"></a>
## 3、实例：持有外部类

### 代码
```java
package org.example.a;

class Outer{
    class Inner {

    }

    Inner createInner() {
        return new Inner();
    }
}

public class Demo {
    public static void main(String[] args) {
        Outer.Inner inner = new Outer().createInner();
        System.out.println(inner);
    }
}
```

### 断点调试
可以看到：内部类持有外部类的对象的引用，是以“this$0”这个字段来保存的。<br />![2022-12-19-19-14-08.510824600.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1671454291207-6076ee8a-c457-41f9-b0c4-ec399bf84f6b.png#averageHue=%239bc075&clientId=u941c473b-0826-4&from=ui&id=u3996851f&originHeight=719&originWidth=723&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1562829&status=done&style=none&taskId=ud1ecae33-5889-4f1b-a1f6-21f1c7cc04b&title=)
<a name="l1XJ5"></a>
## 4、实例：不持有外部类

### 代码
```java
package org.example.a;

class Outer{
    static class Inner {

    }

    Inner createInner() {
        return new Inner();
    }
}

public class Demo {
    public static void main(String[] args) {
        Outer.Inner inner = new Outer().createInner();
        System.out.println(inner);
    }
}
```
<a name="FyEzl"></a>
### 断点调试
可以发现：内部类不再持有外部类了。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1671448637818-3dd9cea2-4bf7-48de-979d-cac7653bb72e.png#averageHue=%23c6dfca&clientId=u941c473b-0826-4&from=paste&id=u19956b5b&originHeight=718&originWidth=726&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u81c21b4c-6c41-47fe-b3ae-87365f948d5&title=)
<a name="jCPBW"></a>
## 5、实例：内存泄露
<a name="qjjie"></a>
### 简介
若内部类持有外部类的引用，对内部类的使用很多时，会导致外部类数目很多。此时，就算是外部类的数据没有被用到，外部类的数据所占空间也不会被释放。<br />本处在外部类存放大量的数据来模拟。
<a name="xBI9c"></a>
### 代码
```java
package org.example.a;

import java.util.ArrayList;
import java.util.List;

class Outer{
    private int[] data;

    public Outer(int size) {
        this.data = new int[size];
    }

    class Innner{

    }

    Innner createInner() {
        return new Innner();
    }
}

public class Demo {
    public static void main(String[] args) {
        List<Object> list = new ArrayList<>();
        int counter = 0;
        while (true) {
            list.add(new Outer(100000).createInner());
            System.out.println(counter++);
        }
    }
}
```
<a name="w3jvk"></a>
### 测试
可以看到：运行了八千多次的时候就内存溢出了。<br />![2022-12-19-19-14-08.538660500.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1671454323191-fd8db7e0-9a50-4d63-b3da-055f91aac3f7.png#averageHue=%23f7f7f6&clientId=u941c473b-0826-4&from=ui&id=u0f1098cd&originHeight=482&originWidth=1016&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1472070&status=done&style=none&taskId=ud38426d5-8935-45cc-bca8-de3f0171b7e&title=)<br />换了一台 mac 电脑，4000 多就内存溢出了。
<a name="OBLuH"></a>
## 6、不会内存泄露的方案
<a name="yEyyr"></a>
### 简介
内部类改为静态的之后，它所引用的对象或属性也必须是静态的，所以静态内部类无法获得外部对象的引用，只能从 JVM 的 Method Area（方法区）获取到 static 类型的引用。
<a name="NARek"></a>
### 代码
```java
package org.example.a;

import java.util.ArrayList;
import java.util.List;

class Outer{
    private int[] data;

    public Outer(int size) {
        this.data = new int[size];
    }

    static class Inner {

    }

    Inner createInner() {
        return new Inner();
    }
}

public class Demo {
    public static void main(String[] args) {
        List<Object> list = new ArrayList<>();
        int counter = 0;
        while (true) {
            list.add(new Outer(100000).createInner());
            System.out.println(counter++);
        }
    }
}
```
<a name="n1MpG"></a>
### 测试
可以发现：循环了四十多万次都没有内存溢出。<br />![2022-12-19-19-14-08.590844000.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1671454322838-00143e68-0b85-4126-bfb1-f3a82ac3e112.png#averageHue=%23f7f7f6&clientId=u941c473b-0826-4&from=ui&id=q8lQ2&originHeight=406&originWidth=481&originalType=binary&ratio=1&rotation=0&showTitle=false&size=587290&status=done&style=none&taskId=u03c83c2f-432c-424e-8148-f9f06a3b5f2&title=)
