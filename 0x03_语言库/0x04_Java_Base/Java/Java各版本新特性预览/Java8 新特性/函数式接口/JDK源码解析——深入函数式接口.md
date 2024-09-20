JDK 函数式接口<br />函数式接口所涉及的知识点包含：java.util.function包，`@FunctoinInterface`注解，Lambda表达式，双冒号操作符。
<a name="QzMxf"></a>
### 概述

- 源码使用的版本为JDK-11.0.11
<a name="ts9Yp"></a>
### 什么是函数式接口
在分析具体内容之前，来对函数式接口做个基本的认知。函数式接口是JAVA语言为引入函数式编程而增加的特性，也即是说函数式接口式Java实现函数式编程的具体方式。那么，函数式编程到底是什么？他和面向对象编程又有什么关系？它能带来什么？又是否真的需要函数式编程？有很多小伙伴，是以面向对象语言开启的编程世界的，对于函数式编程其实很陌生。所以，在这里先引荐编程界的三大流派（当然还有别的流派）：过程式，函数式，对象式：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623662014535-df190389-d204-4aab-8fdc-d2c870280d63.png#clientId=u1a905085-e4d3-4&from=paste&id=uad63098f&originHeight=353&originWidth=734&originalType=url&ratio=3&status=done&style=shadow&taskId=ub3b46279-0a63-4992-a25d-6586a395815)<br />编程范式<br />函数式编程的思想脱胎于数学理论，也就是通常所说的λ演算（λ-calculus）。这也是为什么Java8中引入的函数式编程叫Lambda表达式的原因吧。如同数学中的函数一样，函数式编程范式中的函数有独特的特性，也就是通常说的无状态或引用透明性。一个函数的输出由且仅由其输入决定，同样的输入永远会产生同样的输出。<br />函数式编程的定义："函数式编程是一种编程范式。它把计算当成是数学函数的求值，从而避免改变状态和使用可变数据。它是一种声明式的编程范式，通过表达式和声明而不是语句来编程。" 函数式编程的代码通常更加简洁，但是不一定易懂。<br />近年来，随着多核平台和并发计算的发展，函数式编程的无状态特性，在处理这些问题时有着其他编程范式不可比拟的天然优势。这种发展也就进一步促使了Java引入函数式编程这一特性。
<a name="u0vhN"></a>
### 一个简单示例
先展示一个简单的函数式编程的示例：
```java
/**
   * 简单的函数式编程示例
   */
public static void lambdaDemo1() {
    // 准备测试数据
    Integer[] data = new Integer[] {1, 2, 3};
    List<Integer> list = Arrays.asList(data);

    // 简单示例：转换单位并打印数据
    list.forEach(x -> System.out.println(String.format("Cents into Yuan: %.2f", x/100.0)));
}
```
不熟悉Lambda表达式的小伙伴可能会好奇其中的语句：`x -> System.out.println(String.format("Cents into Yuan: %.2f", x/100.0))`，这是什么呢？这就是Lambda表达式。通常，要访问List对象，需要通过for、while等控制循环语句，并在循环中完成相关工作。有了函数式编程后，就可以使用Lambda表达式来完成对应的功能，是不是很简洁！小伙伴们可能会奇怪，难道Lambda自动做了循环？当然不是，这里的循环控制并没有减少，只是在forEach方法中而已。打开默认的迭代器forEach实现方法（ArrayList的forEach实现有差异，总体逻辑一致），代码显示forEach循环，并在循环中执行参数的函数逻辑。
```java
default void forEach(Consumer<? super T> action) {
    Objects.requireNonNull(action);
    for (T t : this) {
        action.accept(t);
    }
}
```
既然没有省略控制逻辑，难道费这么大的力气引入这个就只是为了简洁点？画了一下调用逻辑，参见下图<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623662014483-1754a32a-104e-4d24-b59f-6836ff633e01.png#clientId=u1a905085-e4d3-4&from=paste&id=ub8b98b17&originHeight=358&originWidth=595&originalType=url&ratio=3&status=done&style=shadow&taskId=u026120ce-4bd8-4f3b-922c-f766b1131cf)<br />从上图中是不是隐约可以看出：这种方式可以将控制部分和业务处理部分进行解耦，业务处理代码更容易集中。<br />在分析forEach源码的时候，看到forEach的参数类型为Consumer，打开Consumer源码（主要接口声明部分）：
```java
@FunctionalInterface
public interface Consumer<T> {
  ...
}
```
这就是一个简单的接口，接口使用了`@FunctionInterface`的接口，这是不是对Lambda表达式使用位置的约束呢？这个问题将在接下来的章节给出答案。
<a name="f33lj"></a>
### Lambda表达式
在示例部分，展示了在Java中如何Lambda进行函数式编程，是不是跃跃欲试想要动手了呢？在动手之前先全面学习Lambda表达式的语法。下面给出几种常见的Lambda代码片段（代码仅截取部分，无上下文）：
```java
() -> System.out.println("demo")
    ...
list.forEach(x ->  System.out.print(x));
...
map.forEach((x, y) -> {
    System.out.print(x);
    System.out.println(y);
});
...
(Integer x, String y) -> System.out.println("x: " + x ", y: " + y);
```
从上面代码片段，可以看出Lambda表达式是通过->操作符来连接的，左边为参数部分，右边为表达式主体。<br />Lambda表达式语法：

1. `(parameters)->expression`
2. `(parameters)->{statements;}`

参数说明：`([[type] parameter [, ...]])`

- 参数包括在圆括号内，参数数量可以0到多个，多个参数通过逗号“,”分割，例如`(x, y)->`
- 参数类型可明确声明，也可以省略，省略时根据上下文进行推断， 例如：`(x)->, (int x)->`
- 无参数，直接使用括号，例如：`()->`
- 一个参数时，且参数类型省略，则括号可以省略 `x->`

表达式主体：

- 由0到多条语句组成
- 只有一条语句时，语句块符号“{}”可省略，此时语句的结果将作为返回值，例如：`->x*x`, `->System.out.print(x)`。
- 超过一条语句时，必须使用语句块符号“{}”包含起来。
- 带`return`关键字必须用代码块，例如：`->{return x+x}`。

常见的组合形式：
```java
(int a, int b) -> {  return a + b; }

() -> System.out.println("Demo")

(String s) -> { 
  System.out.println(s); 
}

() -> 42

() -> { return 3.1415 };
```
启动线程
```java
new Thread(
    () -> System.out.println("start in thread.")
).start();
```
其他的代码遵循基本的Java语法，小伙伴们现在就可以大展拳脚，试试通过Lambda表达式进行函数式编程。
<a name="Msjn1"></a>
### 双冒号操作符
Java类中的的方法也是函数，可不可以在传入Lambda表达式的地方传入普通方法呢？类似下面这种效果：
```java
List<String> list = new ArrayList<String>();
...
list.forEach(xxxMethod());
```
首先`xxxMethod()`会直接触发方法执行，并且返回的类型也不匹配`forEach`方法。那么，正确的形式应该如何写呢？这就需要双冒号云算法登场了。双冒号云算符标准名称为eta-conversion，有下面四种常用场景

1. 实例方法引用 `object::instanceMethod`
2. 静态方法引用 `Class::staticMethod`
3. 实例方法引用（实例作为参数传入） `Class::instanceMethod`
4. 构造方法引用 `Class:new`
- 无参数：`Supplier`
- 一个参数：`Function`
- 二个参数：`BiFunction`
- 更多：自定义函数接口

示例代码
```java
public class FunctionInterfaceInvoke {

    public static void main(String[] args) {

        // 1-1 构造方法（无参数），编译会做参数检查（包含输入参数和返回值）
        Supplier<FunctionInterfaceInvoke> s = FunctionInterfaceInvoke::new;
        s.get();

        //1-2 构造方法（1个参数）
        IntFunction<FunctionInterfaceInvoke> func = FunctionInterfaceInvoke::new;
        func.apply(1);

        // 1-3 构造方法（多个参数）
        BiFunction<Integer, Integer, FunctionInterfaceInvoke> func2 = FunctionInterfaceInvoke::new;
        func2.apply(1, 2);

        // 2 静态方法
        Consumer<Integer> sta1 = FunctionInterfaceInvoke::staticMethod;
        sta1.accept(1);

        // 3 实例方法
        IntConsumer sta2 = new FunctionInterfaceInvoke()::instanceMethod;
        sta2.accept(2);

    }

    public FunctionInterfaceInvoke() {
        System.out.println("none parameters");
    }

    public FunctionInterfaceInvoke(int p1) {
        System.out.println("constructor whith one parameter: " + p1);
    }

    public FunctionInterfaceInvoke(Integer p1, Integer p2) {
        System.out.println(String.format("constructor whith 2 parameters %1s, %2s", p1, p2));
    } 

    public static void staticMethod(Integer p1) {
        System.out.println("static method:" + p1);
    }

    public void instanceMethod(int p1) {
        System.out.println("instance method:"+p1);
    }
}
```
