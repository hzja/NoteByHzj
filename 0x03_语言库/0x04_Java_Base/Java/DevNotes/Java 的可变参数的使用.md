Java <br />Java 支持可变参数的形式定义方法，这种语法糖在某些时候可以简化代码。
<a name="TmlHa"></a>
### 可变参数方法的定义
首先看下可变参数方法在代码上是如何定义的，如下所示：
```java
public static void method1(Integer id, String... names) {
      System.out.println("id:" + id + " names:" + names.length);
}
```
通过上面的示例，可以看出在定义方法时，在最后一个形参类型后加上三点 `…`，就表示该形参可以接受多个相同类型的参数值，多个参数值被当成数组传入。这里需要注意几个点：

1. 可变参数只能作为函数的最后一个参数，在其前面可以有也可以没有任何其他参数；
2. 由于可变参数必须是最后一个参数，所以一个函数最多只能有一个可变参数；
3. Java 的可变参数，会被编译器转型为一个数组；

上面提到可变参数的形式会被编译成一个数组，那么问题来了，可不可以写两个下面这样的方法呢？
```java
public static void method1(Integer id, String... names) {
    System.out.println("id:" + id + " names:"+ names.length);
}

public static void method1(Integer id, String[] names) {
    System.out.println("id:" + id + " names:" + names.length);
}
```
在一个类中的定义相同名字的一个可变参数的方法和一个包含数组的方法，写完过后就发现 IDEA 已经提示这种写法的编译不了的了。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642379037699-6bc4d72c-cdbc-4cfb-994b-45aa71df42b1.webp#clientId=u60221730-7db5-4&from=paste&id=u92578883&originHeight=334&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u04a36304-7dd6-4f1a-ba45-913c20dd716&title=)<br />从这里可以知道可变参数在编译为字节码后，在方法签名中会以数组形态出现的，导致这两个方法的签名一致的，如果同时出现，是不能编译通过的。
<a name="cFRkQ"></a>
### 可变参数方法的调用
可变参数方法的调用跟其他方法的调用没什么区别，这里要说明的是，除了通过可变参数进行调用之外，还可以通过传入数组的形式来进行调用，如下所示：
```java
public static void main(String[] args) {
        //直接传递参数
        method1(1, "hello", "Fcant");
        //通过数组的形式传递参数
        String[] array = new String[]{"hello", "Fcant", "World"};
        method1(2, array);
        //不传递可变参数
        method1(3);
}
```
通过可变参数和数组的形式，这两种调用形式本质上是一样的；另外可变参数的个数也可以为 0。
<a name="GWokX"></a>
### 可变参数方法的重载
试想一下如果定义了下面这样的两个方法，定义和使用的时候会是什么情况
```java
public static void method2(String... names) {
        System.out.println("111111");
}

public static void method2(String value1, String value2) {
        System.out.println("22222");
 }
```
第一个是只有一个可变参数形参的方法；第二个是一个 String 类型的固定参数和第二个参数是可变参数的方法。首先，定义的时候完全没有问题，IDEA 也没有任何错误提示，编译也不会有问题。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642379037733-73261dd3-1d9a-4794-b38b-1f4aecb0bd5f.webp#clientId=u60221730-7db5-4&from=paste&id=uffabf420&originHeight=489&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=udf69e40c-c348-4786-beb1-7f0b8a67d37&title=)<br />那么在使用的时候呢？比如下面这样的写法会输出什么结果呢？
```java
public static void main(String[] args) {
        method2("Hello", "Fcant");
}
```
在看输出结果之前，可以看到，main 函数中的调用，其实这两个重载的函数都是可以满足的，而且编译也没有错，那么程序运行会输出什么呢？![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642379037696-99615a1e-c20a-4167-827a-c44498957399.webp#clientId=u60221730-7db5-4&from=paste&id=u8a51a847&originHeight=691&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud6e3cdff-fba9-4257-a1f6-f53e684185e&title=)<br />通过实际的运行结果可以看到，输出的结果是22222 表示运行的是`method2(String value1, String value2)` 这个方法，那说明什么问题呢？<br />**说明当存在与可变参数方法形成重载方法的时候的，会优先固定参数的方法进行执行**。<br />那如果第二个方法中的 value2 也是可变参数呢？那这种情况会怎么样呢？为此再看一下，下面的这种形式会怎样。
```java
public static void method2(String... names) {
    System.out.println("111111");
}

public static void method2(String value1, String value2) {
    System.out.println("22222");
}

public static void method2(String value1, String... value2) {
    System.out.println("33333");
}
```
首先定义的时候 IDEA 没有任何错误提示，说明编译是没有问题的，那调用的时候呢？<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1642379512108-0e1d7108-9212-4586-a037-9217af6b99b2.png#clientId=u60221730-7db5-4&from=paste&height=420&id=u072267d5&originHeight=1261&originWidth=2269&originalType=binary&ratio=1&rotation=0&showTitle=false&size=196976&status=done&style=shadow&taskId=u8c13a90b-6887-4be7-ab75-19b5c3f8277&title=&width=756.3333333333334)<br />可以看到这个时候 IDEA 已经提示匹配到多个方法合适的方法，不能编译通过，主要是第一个和第三个方式的写法导致的，匹配到了多个可变参数的方法，日常开发的时候要注意这个问题。
<a name="MBaY8"></a>
### Object 可变参数
那可不可以创建一个基于 Object 的可变参数方法，这样子这个方法不就是可以接受所有类型的参数了吗？就像这样：
```java
public static void method3(Object... objects) {
        System.out.println("objects size" + objects.length);
}
```
首先要说的是，这么定义当然是没有问题的，但是可读性会差很多，调用方完全不知道要传入什么类型；要是真的写了太多像这样的代码，估计维护起来也是害人害己。
