<a name="aIgfE"></a>
## 1、为什么应该从 Java 11 迁移？
尽管 Java 11 也是 LTS 版本并且被许多应用程序使用，但要转向 Java 17 有一些主要原因。

- **结束对 Java 11 的支持：** Java 11 只支持到 2023 年 9 月，并提供扩展支持到 2026 年 9 月。这意味着支持结束后，将没有补丁（甚至包括安全补丁）。
- **Spring 6：** Spring 的最新版本，Spring 6 将需要 Java 17 才能工作，并且由于有许多与它们一起工作的库，它们也将迁移到 Java 17。_如果应用程序依赖于 Spring 框架，应该绝对考虑迁移到 Java 17。_
- **适用于 Java 17 的免费 Oracle JDK：** Java 17 是根据新的 NFTC（Oracle 免费条款和条件）许可证发布的。因此，再次允许免费使用 Oracle JDK 版本用于生产和商业用途（Java 11 不允许）。
<a name="cHPt9"></a>
## 2、Java 17 有什么新功能？
Java 17 中引入了多项改进和新功能，这些功能将得到长期支持。
<a name="RJCcO"></a>
### 文本块
Java 引入了文本块，以使代码更具可读性并避免不必要的字符串格式化。现在，可以将文本放在三引号之间，并在其中包含多个双引号字符串，而无需使用转义字符。一个例子如下所示：
```java
private static void jsonBlock() {
    String text = """
            {
              "name": "John Doe",
              "age": 45,
              "address": "Doe Street, 23, Java Town"
            }
          """;
    System.out.println(text);
}
```
正如看到的，这使得编写 Json 和类似的字符串变得非常容易，而在之前的版本中，实现该功能需要大量使用转义字符。<br />此外，结尾的三个双引号表示文本块的开头或其在输出中的缩进。在上面的示例中，输出中的每行都会有两个空格，因为双引号的位置是最后一个字符后面的两个空格。<br />引入了两个新的转义字符用于文本块内，"\s" 用于添加空格，"\" 用于删除换行符。在编写长 SQL 语句时特别有用。
```java
private static void sqlStatement() {
    String sql = """
    SELECT id, firstName, lastName\s\
    FROM Employee
    WHERE departmentId = "IT" \
    ORDER BY lastName, firstName""";
    System.out.println(text);
}
```
<a name="BS0tu"></a>
### 改进的 Switch 语句
Switch 表达式将允许您从 switch case 返回值，并在赋值中使用这些返回值。Java 允许使用运算符->（箭头）而不是:（冒号）来表示返回表达式。在这个表达式中使用 switch 返回时不需要break关键字，但需要default case。
```java
private static void improvedSwitch(Fruit fruit) {
    String text = switch (fruit) {
        case APPLE, PEAR -> {
            System.out.println("the given fruit was: " + fruit);
            yield "Common fruit";
        }
        case ORANGE, AVOCADO -> "Exotic fruit";
        default -> "Undefined fruit";
    };
    System.out.println(text);
}
```
如果在 switch case 内完成多个操作，可以有一个 case 块并使用Yield关键字表示返回值。这里的yield是一个上下文相关的关键字，即你可以在函数内的其他地方使用变量名yield 。
<a name="lzjuQ"></a>
### 'Records'类型
Records类是一种特殊的不可变类，旨在取代数据传输对象（DTO）。通常，如果想在类或方法中使用一些 POJO，必须声明该类并定义所有 getter 、setter、equals和hashcode函数。例如，要在其他地方使用示例Fruit类，必须以如下方式定义类：
```java
public class Fruit {
    private String name;
    private int price;

    //getters, setters, equals and hashcode methods
}
```
尽管可以通过使用lombok等库来减少大部分样板代码，但仍然可以借助Records来进一步减少代码。对于Records，相同的代码变为：
```java
public static void doSomething() {
    record Fruit(String name, int price) {}
    Fruit fruit = new Fruit("Apple", 100);
    System.out.println(fruit.getPrice());
}
```
正如看到的，甚至可以定义方法本地记录对象。Records 对象自动为其所有字段提供getter、setter、equals和hashcode方法。<br />Records中的字段不能更改，只能通过声明记录时给出的参数来定义，如上所示（但可以定义静态变量）。还可以定义一个可以验证字段的自定义构造函数。建议不要重写记录的 getter 和 setter，这可能会影响其不变性。<br />具有多个构造函数以及静态变量和方法的Records示例如下所示：
```java
public record Employee(int id, String firstName,
                       String lastName){

    static int empToken;

    // Compact Constructor
    public Employee{
        if (id < 100) {
            throw new IllegalArgumentException(
                "Employee Id cannot be below 100.");
        }
        if (firstName.length() < 2) {
            throw new IllegalArgumentException(
                "First name must be 2 characters or more.");
        }
    }


    // Alternative Constructor
    public Employee(int id, String firstName){
        this(id, firstName, null);
    }

    // Instance methods
    public void getFullName(){
        if (lastName == null)
            System.out.println(firstName());

        else
            System.out.println(firstName() + " "
                               + lastName());
    }

    // Static methods
    public static int generateEmployeeToken(){
        return ++empToken;
    }
}
```
Records类的更多特性包括：

1. 可以在Records内使用嵌套类和接口。
2. 也可以有嵌套的Records，这将是隐式静态的。
3. 一条Records可以实现接口。
4. 可以创建通用Records类。
5. Records是可序列化的。
<a name="eyZxY"></a>
### 密封类
密封类将使我们能够更好地控制哪些类可以扩展我们的类。在 Java 11 中，类可以是最终类或扩展类。如果您想控制哪些类可以扩展您的超类，可以将所有类放在同一个包中，并赋予超类包可见性。但是，无法再从包外部访问超类。作为示例，请参阅下面的代码：
```java
public abstract class Fruit {
}
public final class Apple extends Fruit {
}
public final class Pear extends Fruit {
}
```
```java
private static void problemSpace() {
    Apple apple = new Apple();
    Pear pear = new Pear();
    Fruit fruit = apple;
    class Avocado extends Fruit {};
}
```
在这里，无法阻止Avocado扩展Fruit类。如果将Fruit类设置为 default ，那么将apple分配给fruit对象将无法编译。但是现在可以使用密封类来仅允许特定的类扩展超类。下面给出一个例子：
```java
public abstract sealed class FruitSealed permits AppleSealed, PearSealed {
}
public non-sealed class AppleSealed extends FruitSealed {
}
public final class PearSealed extends FruitSealed {
}
```
正如看到的，使用一个新的关键字seal来表示这是一个密封类。定义可以使用permits关键字扩展的类。任何扩展密封类的类都可以是像PearSealed这样的Final 类，也可以像AppleSealed一样在声明类时使用non-sealed关键字由其他类扩展。<br />此实现将允许将AppleSealed分配给FruitSealed类 ，但不允许任何其他未由allowed关键字定义的类扩展FruitSealed类。
<a name="tCEsA"></a>
### 与“实例”的模式匹配
在Java 11中，通常使用`instance of`运算符来检查一个对象是否属于某个类。如果想在 check实例返回 true后对其执行某些操作，则需要将对象显式转换为该特定类。一个例子如下所示：
```java
private static void oldStyle() {
    Object o = new Grape(Color.BLUE, 2);
    if (o instanceof GrapeClass) {
        Grape grape = (Grape) o;
        System.out.println("This grape has " + grape.getPits() + " pits.");
    }
}
```
这里，需要显式地将对象转换为Grape类型，然后找出坑的数量。在 Java 17 中，可以将其更改为：
```java
private static void patternMatchingInJava17() {
    Object o = new Grape(Color.BLUE, 2);
    if (o instanceof Grape grape) {
        System.out.println("This grape has " + grape.getPits() + " pits.");
    }
}
```
可以将check实例与&&( and) 条件配对，但不能|| （或）在“或”条件的情况下，即使检查实例返回 false，该语句也可以达到另一个条件<br />如果 check实例返回 true，则变量Grape的范围甚至可以超出 if 块。在下面的示例中，如果对象不是Grape类型，则会抛出运行时异常，因此编译器在到达 print 语句时将确定Grape对象应该存在。
```java
private static void patternMatchingScopeException() {
    Object o = new Grape(Color.BLUE, 2);
    if (!(o instanceof  Grape grape)) {
        throw new RuntimeException();
    }
    System.out.println("This grape has " + grape.getPits() + " pits.");
}
```
<a name="UZOlc"></a>
### 有用的空指针异常
在 Java 11 中，当得到 NullPointerException 时，只得到发生异常的行号，但没有得到解析为 null 的方法或变量。在 Java 17 中，消息传递得到了改进，因为 NullPointerException 消息还告诉我们导致 NullPointerException 的确切方法调用。
```java
public static void main(String[] args) {
    HashMap<String, Grape> grapes = new HashMap<>();
    grapes.put("grape1", new GrapeClass(Color.BLUE, 2));
    grapes.put("grape2", new GrapeClass(Color.white, 4));
    grapes.put("grape3", null);
    var color = ((Grape) grapes.get("grape3")).getColor();
}
```
正如在这里看到的，正在尝试获取“grape3”对象的颜色，该对象为空。当比较 Java 11 和 Java 17 中收到的错误消息时，会看到错误消息的差异，因为现在确切地知道是对映射中存在的 null 对象调用 get 方法导致了异常。
```
// Java 11
Exception in thread "main" java.lang.NullPointerException
        at com.rg.java17.HelpfulNullPointerExceptions.main(HelpfulNullPointerExceptions.java:13)
```
```
// Java 17
Exception in thread "main" java.lang.NullPointerException: Cannot invoke "com.rg.java17.Grape.getColor()" because the return value of "java.util.HashMap.get(Object)" is null
    at com.rg.java17.HelpfulNullPointerExceptions.main(HelpfulNullPointerExceptions.java:13)
```
<a name="iOt90"></a>
## 3、更多改进
<a name="cVjbx"></a>
### 紧凑数字格式支持
`NumberFormat`类中添加了一个工厂方法，以便根据Unicode标准以紧凑、人类可读的形式格式化数字。有SHORT和LONG格式可用，示例如下：
```java
NumberFormat shortFormat = NumberFormat.getCompactNumberInstance(Locale.ENGLISH, NumberFormat.Style.SHORT);
System.out.println(shortFormat.format(1000))

NumberFormat longFormat = NumberFormat.getCompactNumberInstance(Locale.ENGLISH, NumberFormat.Style.LONG);
System.out.println(shortFormat.format(1000))
```
```
// Output
1K
1 thousand
```
<a name="Io57U"></a>
### 添加了日间支持
新的模式“ B”被添加到 DateTime 模式中，允许它指定一天中的时间。
```java
DateTimeFormatter timeOfDayFomatter = DateTimeFormatter.ofPattern("B");
System.out.println(timeOfDayFomatter.format(LocalTime.of(8, 0)));
System.out.println(timeOfDayFomatter.format(LocalTime.of(13, 0)));
System.out.println(timeOfDayFomatter.format(LocalTime.of(20, 0)));
System.out.println(timeOfDayFomatter.format(LocalTime.of(23, 0)));
System.out.println(timeOfDayFomatter.format(LocalTime.of(0, 0)));
```
```
// Output
in the morning
in the afternoon
in the evening
at night
midnight
```
<a name="fhSRo"></a>
## 4、性能基准
Java 17 在内存使用和时间复杂度方面也比 Java 11 有所改进。已经完成了一个这样的基准测试，他们通过让两个版本编写的代码执行一系列任务来统计其性能。完整的结果和任务描述可以在这里找到。<br />已注意到的一些一般结果是：

1. 对于 G1GC（默认垃圾收集器），Java 17 比 Java 11 快 8.66%，比 Java 16 快 2.41%。
2. 对于ParallelGC（并行垃圾收集器），Java 17 比 Java 11 快 6.54%，比 Java 16 快 0.37%。
3. 并行垃圾收集器（Java 17 中提供）比 G1 垃圾收集器（Java 11 中使用）快 16.39%。

从 Java 11 迁移到 Java 17 可以提供许多好处，包括新功能和改进的性能。但是，必须了解迁移过程中可能出现的潜在瓶颈。许多库也会升级到更新版本以支持 Java 17。<br />因此，如果在项目中使用外部库，应该非常小心。通过了解这些潜在问题并采取必要的步骤来解决它们，您可以确保顺利成功地迁移到 Java 17。
