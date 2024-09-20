Java<br />在日常写Java代码的时候，对于字符串的操作是非常普遍的，其中最常见的就是对字符串的组织。也因为这个操作非常普遍，所以诞生了很多方案，总下来大概有这么几种：

- 使用+拼接
- 使用`StringBuffer`和`SpringBuilder`
- `String::format` and `String::formatted`
- 使用`java.text.MessageFormat`

下面，一起来学习一下Java 21中的新方案！
<a name="LHo6s"></a>
## 模版表达式
在Java 21中处理字符串的新方法称为：**Template Expressions**，即：模版表达式。<br />模版表达式是Java语言中的一种新表达式。它可以执行字符串插值，帮助开发人员通过编程的方式安全高效地组织字符串。此外，模板表达式不仅仅可以用于组织字符串，它还可以根据特定模型的规则将结构化文本转换为任何类型的对象。<br />下面通过一个简单的案例来认识一下它：
```java
String blog = "https://www.yuque.com/fcant";
String str = STR."My blog is \{blog}";
```
上述代码中的第2行就是一个模版表达式，其中主要包含三个部分：

- 模版处理器：STR
- 包含内嵌表达式（\{blog}）的模版
- 通过.把前面两部分组合起来，形式上类似方法调用

当模版表达式运行的时候，模版处理器会将模版内容与内嵌表达式的值组合起来，生成结果，所以上面案例中的字符串str在运行时的结果为：My blog is https://www.yuque.com/fcant。
<a name="vcmMW"></a>
## 多行模版表达式
Java中的文本块特性，类似下面这样的写法：
```java
String html = """
    <html>
    <body>
      <h1>Java 15 新特性</h1>
      <p>文本块</p>
    </body>
    </html>
    """;
```
模版表达式也支持类似的多行字符串处理，所以开发者可以用它来方便的组织html、json、xml等字符串内容，比如下面这样：
```java
var json = STR."""
{
    "user": "\{name}",
    "age: \{age}
}
""";
```
<a name="kCHti"></a>
## STR模版处理器
上面案例中所用的STR模版处理器中的内嵌表达式都采用了字符串内容，而实际上STR模版处理器还有更多的用途。

1. 内嵌表达式中还可以之前数学运算，比如：
```java
int x = 10, y = 20;
String s = STR."\{x} + \{y} = \{x + y}";
```
最终s字符串结果为：10 + 20 = 30

2. 内嵌表达式中还可以调用方法，比如：
```java
String s = STR."My blog is \{getMyBlog()}";
```
假设`getMyBlog`方法返回的内容为https://www.yuque.com/fcant，那么最终s字符串结果为：My blog is https://www.yuque.com/fcant。

3. 内嵌表达式中还可以访问对象的成员变量，比如：
```java
User u = new User("Fcant", "https://www.yuque.com/fcant"); // 构造函数参数为name属性和blog属性
String s = STR."\{u.name}的博客地地址为：+ \{u.blog}";
```
最终s字符串结果为：Fcant的博客地址为：https://www.yuque.com/fcant。
<a name="P5oKK"></a>
## FMT模版处理器
除了STR模版处理器之外，Java中还提供了另外一个模版处理器：FMT。FMT模版处理器除了与STR模版处理器一样提供插值能力之外，还提供了左侧的格式化处理。下面通过例子来直接理解FMT的功能：
```java
record Rectangle(String name, double width, double height) {
    double area() {
        return width * height;
    }
}
Rectangle[] zone = new Rectangle[] {
    new Rectangle("Alfa", 17.8, 31.4),
    new Rectangle("Bravo", 9.6, 12.4),
    new Rectangle("Charlie", 7.1, 11.23),
};
String table = STR."""
    Description  Width  Height  Area
    \{zone[0].name}  \{zone[0].width}  \{zone[0].height}     \{zone[0].area()}
    \{zone[1].name}  \{zone[1].width}  \{zone[1].height}     \{zone[1].area()}
    \{zone[2].name}  \{zone[2].width}  \{zone[2].height}     \{zone[2].area()}
    Total \{zone[0].area() + zone[1].area() + zone[2].area()}
    """;
```
上面这块是由STR模版处理器组织字符串表格数据，从模版表达式来看是非常整齐的，但是由于模版中内嵌表达式的值长短不一，最终输出结果很有可能差强人意，比如STR模版处理器处理后的结果是这样的：
```
Description  Width  Height  Area
Alfa  17.8  31.4     558.92
Bravo  9.6  12.4     119.03999999999999
Charlie  7.1  11.23     79.733
Total 757.693
```
为了解决这个问题，就可以采用FMT模版处理器，在每一列左侧定义格式
```java
String table = FMT."""
    Description     Width    Height     Area
    %-12s\{zone[0].name}  %7.2f\{zone[0].width}  %7.2f\{zone[0].height}     %7.2f\{zone[0].area()}
    %-12s\{zone[1].name}  %7.2f\{zone[1].width}  %7.2f\{zone[1].height}     %7.2f\{zone[1].area()}
    %-12s\{zone[2].name}  %7.2f\{zone[2].width}  %7.2f\{zone[2].height}     %7.2f\{zone[2].area()}
    \{" ".repeat(28)} Total %7.2f\{zone[0].area() + zone[1].area() + zone[2].area()}
    """;
```
这样的结果将是如下这样，获得一个整齐的结果：
```
Description     Width    Height     Area
Alfa            17.80    31.40      558.92
Bravo            9.60    12.40      119.04
Charlie          7.10    11.23       79.73
                             Total  757.69
```
关于Java 21中推出的String Templates特性还有很多高级能力，比如：自定义模版处理器等。因为该特性还处于Preview阶段，这里仅尝鲜了一下基本功能。
