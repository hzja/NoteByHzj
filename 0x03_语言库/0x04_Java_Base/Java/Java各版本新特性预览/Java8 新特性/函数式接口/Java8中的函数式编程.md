Java <br />介绍一个函数式Java工具包，它表现了很多优秀的函数式编程思想。熔断降级组件Hystrix的替代品resilience4j就基于vavr库。
<a name="FmBKl"></a>
## Vavr
Vavr是一个Java8函数库，它运用了大量的函数式编程范式。创造性地封装了一些持久性的数据结构和函数式控制结构。而且从中可以学到很多有用的编程思想。
<a name="r6qOJ"></a>
### 可观察的副作用
的代码中经常会出现一些看不见的陷阱，从代码语义中这些陷阱是无法被观察的。例如
```java
int divide(int a, int b){
    return a/b;
}
```
知道`a/b`会得到一个整数，但是却不能从代码上明确地知道如果`b=0`将会抛出java.lang.ArithmeticException异常；而如果是`a+b`则不会带来任何副作用。所以需要让这种副作用是可观察的。对于这一点Vavr做出了一种设计：
```java
Try<Integer> divide(Integer a, Integer b) {
    return Try.of(() -> a / b);
}
```
将可能的副作用封装到一个容器中，明确了可能的失败，当看到返回的是`Try<Integer>`时，就意味着结果可能“并不顺利”，以便于针对性地进行预防。
<a name="yXBiX"></a>
### 不可变的数据结构
很多语言都在使用不可变的数据结构，比如Golang、Kotlin。主要原因是不可变的值：

- 本质上是线程安全的，因此不需要同步
- 对于equals和hashCode是可靠的
- 不需要克隆
- 在非受检`unchecked`类型转换中是类型安全的
- 对于函数式编程来说不可变值是最透明的

为此Vavr设计了一个集合类库，旨在代替Java中的集合框架。Vavr 的集合库包含一组丰富的函数式数据结构，这些数据结构建立在 lambdas 之上。它们与 Java 原始集合共享的唯一接口是Iterable。这些数据结构是持久性的，一旦初始化本身就不可改变，可以使用一些操作来返回更改后的副本。例如经典的数据结构单向链表：
```java
// 1   2  3
List<Integer> source = List.of(1, 2, 3);
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624195973471-c5f483cf-c5cd-4623-88c6-7dab43c403db.webp#clientId=u1d6f7d28-3d1a-4&from=paste&id=u4c6a8deb&originHeight=40&originWidth=260&originalType=url&ratio=3&status=done&style=shadow&taskId=u5d303aa1-0dfe-4608-a881-2916adcdb0a)<br />如果将一个新元素`0`放在原始链表尾部的前面
```java
//  0  2  3
List<Integer> newHeadList = source.tail().prepend(0);
//  1  2  3
System.out.println(source);
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624195973704-b261b13e-1993-44b7-a6c8-91ce8d0d6113.webp#clientId=u1d6f7d28-3d1a-4&from=paste&id=u632a8782&originHeight=86&originWidth=260&originalType=url&ratio=3&status=done&style=shadow&taskId=uf95d2776-e679-42f3-b722-60a1af444af)<br />原始链表保持不变，新的链表大小保持不变元素被替换了。当然可以使用其它API来生成一个大小变化的副本，不过可以肯定的是原始的链表一定不会发生改变。
```java
// 0 1 2 3
List<Integer> prepend = source.prepend(0);
// 1 2 3 0
List<Integer> append = source.append(0);
```
这只是其中的一部分编程思想，接下来将介绍Vavr的一些特色。
<a name="wyGKm"></a>
## Vavr的一些特色
Vavr提供了一些非常有用的而且有特色的API。
<a name="iBxYR"></a>
### 元组
熟悉Python的同学对元组（Tuple）一定不陌生。元组将固定数量的元素组合在一起，以便它们可以作为一个整体传递。与数组或列表不同，元组可以包含不同类型的对象，但它也是不可变的。目前Vavr提供了最多8个元素的元组结构。
```java
// (fcant.cn, 22)
Tuple2<String, Integer> java8 = Tuple.of("fcant.cn", 22); 
// fcant.cn
String s = java8._1; 
// 22
Integer i = java8._2;
```
这个可以用来模拟Java中不具有的多返回值的特性。
<a name="JmISc"></a>
### Function
Java本身提供了`Function`接口，但是Vavr则提供了更加丰富的`Function`扩展，例如可以组合多个`Function`
```java
Function1<Integer, Integer> multiplyByTwo = a -> a * 2;
Function1<Integer, Integer> compose = multiplyByTwo.compose(a -> a + 1);
// 6
Integer apply = compose.apply(2);
```
除此之外，还可以让潜在的副作用降级（lift），有点类似于微服务的熔断，以避免在函数执行中处理异常
```java
Function2<Integer, Integer, Integer> divide = (a, b) -> a / b;
// 降级 
Function2<Integer, Integer, Option<Integer>> safeDivide = Function2.lift(divide);
// 返回一个加强版的Optional
Option<Integer> apply = safeDivide.apply(1, 0);
boolean empty = apply.isEmpty();
// true
System.out.println(empty);
```
还有派生操作：
```java
 Function2<Integer, Integer, Integer> divide = (a, b) -> a / b;
 Function1<Integer, Integer> a = divide.apply(4);
 Integer apply = a.apply(2);
```
这有点类似于柯里化，当用到更多入参时柯里化才更加明显：
```java
Function3<Integer, Integer, Integer, Integer> sum = (a, b, c) -> a + b + c;
final Function1<Integer, Function1<Integer, Integer>> add2 = sum.curried().apply(1);
Integer apply = add2.apply(2).apply(3);
```
猜一猜答案是几？
<a name="PcUvL"></a>
### 带有特性的值容器
这个不太好用中文说明，有一些值带有独特的性质，比如开头提到的Try,用来显式表明可能遇到异常。Vavr提供了很多具有独特性质的值容器。
<a name="MT8Jv"></a>
#### Option
类似`Optional`，但是比`Optional`更加强大。
<a name="M4xow"></a>
#### Lazy
Lazy是一个惰性计算的容器，表示当使用时才去计算且只计算一次。
```java
Lazy<Double> lazy = Lazy.of(Math::random);
lazy.isEvaluated(); // = false
lazy.get();         // = 0.123  
lazy.isEvaluated(); // = true
lazy.get();         // = 0.123 
// 需要使用数据时才从数据源加载
Data lazyData = Lazy.val(DataSourceService::get, Data.class);
```
其它还有一些非常有用的容器，可以尝试它们。
<a name="eP39J"></a>
### 模式匹配
函数式编程语言大都支持模式匹配，同为JVM语言的Scala中就有这种特性，而Java目前是没有的。可以有效地帮助减少if-else，举个例子：
```java
public static String convert(int input) {

    String output;
    if (input == 1) {
        output = "one";
    } else if (input == 2) {
        output = "two";
    } else if (input == 3) {
        output = "three";
    } else {
        output = "unknown";
    }
    return output;
}
```
Vavr就清爽多了。
```java
public static String vavrMatch(int input) {
    return Match(input).of(
        Case($(1), "one"),
        Case($(2), "two"),
        Case($(3), "three"),
        Case($(), "unknown")
    );
}
```
当然还有其它一些玩法需要自己去发现。
<a name="r0p9M"></a>
## 总结
函数式编程作为Java8最大的一个亮点（个人认为），对于习惯于传统OOP编程的开发者来说确实不容易接受。不妨从Vavr类库入手去学习函数式编程的思想。今天介绍的只是它很少的一部分，还有更多内容去发现、去借鉴。忘记说了，如果想在项目中引用它，可以引入下面这个坐标：
```xml
<!-- https://mvnrepository.com/artifact/io.vavr/vavr -->
<dependency>
    <groupId>io.vavr</groupId>
    <artifactId>vavr</artifactId>
    <version>0.10.3</version>
</dependency>
```
