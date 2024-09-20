Java
<a name="YtJ1L"></a>
## 引言
Java 8 引入了函数式接口，提供了一种新的方法定义和传递行为的方式。函数式接口是指只包含一个抽象方法的接口。这种接口的实例可以通过 Lambda 表达式、方法引用或构造函数引用来创建，让代码更加简洁和可读性更强。
<a name="xjees"></a>
## 一、函数式接口
关于什么是函数式接口往前有详细说过，再啰嗦几句。<br />函数式接口是只有一个抽象方法的接口。Java 8中引入的函数式接口可以用Lambda表达式来实现。使用Lambda表达式可以简化代码，并使代码更加易读。
```java
@FunctionalInterface
interface MyFunction {
    void run();
}

public class Example {
    public static void main(String[] args) {
        MyFunction func = () -> System.out.println("Hello, world!");
        func.run();
    }
}
```
在这个例子中，定义了一个函数式接口MyFunction，该接口只有一个抽象方法`run()`。接下来在main函数中使用Lambda表达式实现MyFunction接口，并通过调用`run()`方法来执行Lambda表达式。
<a name="nfzF9"></a>
## 二、常用的接口
Java 8中内置了许多常用的函数式接口。下面是一些常用的函数式接口及其用途：<br />`Predicate`：接受一个输入参数，返回一个布尔值结果。<br />`Consumer`：接受一个输入参数，不返回任何结果。<br />`Function<T, R>`：接受一个输入参数，返回一个结果。<br />`Supplier`：不接受任何输入参数，返回一个结果。
<a name="QbnEi"></a>
### 2.1、`Predicate`
`Predicate` 是一个函数式接口，它接受一个参数并返回一个布尔值。可以使用它来实现各种过滤器和检查器。例如，可以使用 Predicate 来检查一个字符串是否为空：
```java
Predicate<String> isEmpty = str -> str.isEmpty();
boolean result = isEmpty.test(""); // 返回 true
```
也可以使用 `Predicate` 来过滤集合中的元素：
```java
List<String> list = Arrays.asList("apple", "banana", "pear", "orange");
Predicate<String> startsWithA = str -> str.startsWith("a");
List<String> filteredList = list.stream()
                               .filter(startsWithA)
                               .collect(Collectors.toList()); // 返回 ["apple"]
```
<a name="zxhBn"></a>
### 2.2、`Function`
`Function` 是一个函数式接口，它接受一个参数并返回一个结果。可以使用它来进行各种转换和映射。例如，可以使用 `Function` 来将一个字符串转换为大写：
```java
Function<String, String> toUpperCase = str -> str.toUpperCase();
String result = toUpperCase.apply("hello"); // 返回 "HELLO"
```
也可以使用 `Function` 来映射集合中的元素：
```java
List<String> list = Arrays.asList("apple", "banana", "pear", "orange");
Function<String, Integer> length = str -> str.length();
List<Integer> lengths = list.stream()
                            .map(length)
                            .collect(Collectors.toList()); // 返回 [5, 6, 4, 6]
```
<a name="kVuyG"></a>
### 2.3、`Consumer`
`Consumer` 是一个函数式接口，它接受一个参数并不返回任何结果。可以使用它来对一组元素进行操作。例如，可以使用 `Consumer` 来打印一个字符串：
```java
Consumer<String> print = str -> System.out.println(str);
print.accept("hello"); // 输出 "hello"
```
也可以使用 `Consumer` 来对集合中的元素进行操作：
```java
List<String> list = Arrays.asList("apple", "banana", "pear", "orange");
Consumer<String> printUpperCase = str -> System.out.println(str.toUpperCase());
list.forEach(printUpperCase); // 输出 "APPLE", "BANANA", "PEAR", "ORANGE"
```
<a name="j6dkK"></a>
### 2.4、`Suppiler`
`Supplier` 是一个函数式接口，它不接受任何参数，但返回一个结果。可以使用它来生成值。例如，可以使用 `Supplier` 来生成一个随机数：
```java
Supplier<Integer> random = () -> new Random().nextInt(100);
int result = random.get(); // 返回 0~99 中的一个随机数
```
也可以使用 `Supplier` 来生成一组默认值：
```java
Supplier<List<String>> defaultList = () -> Arrays.asList("apple", "banana", "pear", "orange");
List<String> list = defaultList.get(); // 返回 ["apple", "banana", "pear", "orange"]
```
<a name="qXUuy"></a>
## 小结
函数式编程成为了Java编程中的一大趋势，Java 8中引入了函数式接口，使得函数式编程更加容易。了解以上这些内置的函数式接口，基本在日常开发中80%的场景都可以满足了，如果对自己有更高的要求，推荐大家把java.util.function包下的接口都熟悉一下。
