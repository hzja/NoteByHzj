JDK 21 于 2023 年 9 月 19 日发布，是继之前的 LTS 版本 JDK 17 之后最新的长期支持 (LTS) 版本。<br />以下是 JDK 21 的新功能列表：

- 虚拟线程
- 序列集合
- 记录模式
- 字符串模板（预览）
- 未命名模式和变量（预览）
- 未命名类和实例主要方法（预览）
- 作用域值（预览）
- 结构化并发（预览）
<a name="HmHv2"></a>
## 1、虚拟线程
从 Java 代码的角度来看，虚拟线程感觉就像普通线程，但它们没有 1:1 映射到操作系统/平台线程。它是从虚拟线程到载体线程进而到操作系统线程的M:N映射。<br />有一个所谓的载体线程池，虚拟线程临时映射（“安装”）到该线程池上。一旦虚拟线程遇到阻塞操作，虚拟线程就会从载体线程中移除（“卸载”），并且载体线程可以执行另一个虚拟线程（新的或之前被阻塞的虚拟线程）。<br />载体线程池是ForkJoinPool<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1701852837729-30a0fb61-58fe-4559-ba7c-0e1b6222e8cc.jpeg)<br />虚拟线程的一些优点：

- 提高应用程序吞吐量
- 提高应用程序可用性
- 减少内存消耗
<a name="vkcaK"></a>
### 创建虚拟线程
要创建虚拟线程，可以使用 `Thread.ofVirtual()` 工厂方法并传递可运行对象。

1. `Thread.ofVirtual().start(Runnable);`
2. `Thread.ofVirtual().unstarted(Runnable);`

如果想让虚拟线程立即启动，你可以使用`start()` 方法，它会立即执行传递给它的`Runnable start()`。<br />如果不希望虚拟线程立即启动，可以使用该`unstarted()`方法。
<a name="hcV6Q"></a>
### 创建使用虚拟线程的`ExecutorService`
只需要替换`newFixedThreadPool`为`newVirtualThreadPerTaskExecutor`
```java
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class VirtualThreadExample {

    public static void main(String[] args) {
        ExecutorService executor = Executors.newVirtualThreadPerTaskExecutor();

        executor.submit(() -> {
            System.out.println(Thread.currentThread().getName())
        });

        executor.shutdown();
    }
}
```
<a name="cTG6I"></a>
## 2、顺序集合
顺序集合提供了defined encounter order（是一种所见即所得的顺序，含义是从队列中取出元素的顺序既是你存放该元素时候的顺序），用于访问第一个和最后一个元素并以相反的顺序迭代。<br />这意味着可以在集合的两端添加、检索或删除元素。<br />[![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1701845297599-9a17770e-f7b5-4959-95d4-c0b1f860cc05.png#averageHue=%23f3f3f3&clientId=u4705a8c1-c59d-4&from=paste&id=ubbf4f4b0&originHeight=696&originWidth=1400&originalType=url&ratio=2.5&rotation=0&showTitle=false&size=124128&status=done&style=none&taskId=u6eccc717-a84f-41cf-8165-424762d5bd9&title=)](https://cr.openjdk.org/~smarks/collections/SequencedCollectionDiagram20220216.png)
```java
public interface SequencedCollection<E> extends Collection<E> {
    default void addFirst(E e) { ... }
    default void addLast(E e) { ... }
    default E getFirst() { ... }
    default E getLast() { ... }
    default E removeFirst() { ... }
    default E removeLast() { ... }
    SequencedCollection<E> reversed();
}
```
正如看到的，除了`reverse()`之外的所有方法都是默认方法并提供默认实现。<br />这意味着现有的集合类（例如 ArrayList 和 LinkedList）都可以实现此接口，而无需更改其代码。
```java
ArrayList<Integer> list = new ArrayList<>();
list.add(1); // [1]
list.addFirst(0); // [0, 1]
list.addLast(2); // [0, 1, 2]
list.getFirst(); // 0
list.getLast(); // 2
list.reversed(); // [2, 1, 0]
```
<a name="ZMcRY"></a>
### SequencedSet
SequencedSet 接口对于具有有序元素的 Set 非常有用，特别是当您必须执行某些操作（例如检索或删除第一个或最后一个索引处的元素）时。它还提供了一种反转元素的方法。<br />还需要知道两个 SequencedSet 对象的比较与其他类型的 Set 相同，不依赖于元素顺序。
```java
interface SequencedSet<E> extends Set<E>, SequencedCollection<E> {
    SequencedSet<E> reversed();
}
```
`LinkedHashSet` 是 Set 的一个实现，它实现了 `SequencedSet` 接口。<br />因此，可以使用 `LinkedHashSet` 来创建 `SequencedSet`。<br />Set 的其他实现（例如 HashSet 和 TreeSet）未实现该接口。<br />探索一些示例来演示如何访问第一个和最后一个元素，以及如何使用反向函数：
```java
SequencedSet<String> values = new LinkedHashSet<>();
values.add("one");
values.add("two");
System.out.println(values); // [one, two]

values.addFirst("zero");
System.out.println(values); // [zero, one, two]
values.addFirst("one");
System.out.println(values); // [one, zero, two]

values.addLast("three");
System.out.println(values); // [one, zero, two, three]

values.removeFirst();
System.out.println(values); // [zero, two, three]

SequencedSet<String> reversedSet = values.reversed();
System.out.println(reversedSet); // [three, two, zero]

boolean isEqual = values.equals(reversedSet);
System.out.println(isEqual); // true
System.out.println(values.hashCode()); // 612888
System.out.println(reversedSet.hashCode()); // 612888
System.out.println(values.hashCode() == reversedSet.hashCode()); // true
```
<a name="Bn61G"></a>
### `SequencedMap`
如果要使用 `SequencedMap` 中定义的新方法，则需要使用 Map 实现，例如 `LinkedHashMap` 或实现 `SortedMap` 的 Map。<br />HashMap 不利用 Sequenced Collections，因为它没有定义 defined encounter order（是一种所见即所得的顺序，含义是从队列中取出元素的顺序既是你存放该元素时候的顺序）。
```java
interface SequencedMap<K,V> extends Map<K,V> {
    SequencedMap<K,V> reversed();
    SequencedSet<K> sequencedKeySet();
    SequencedCollection<V> sequencedValues();
    SequencedSet<Entry<K,V>> sequencedEntrySet();
    V putFirst(K, V);
    V putLast(K, V);
    Entry<K, V> firstEntry();
    Entry<K, V> lastEntry();
    Entry<K, V> pollFirstEntry();
    Entry<K, V> pollLastEntry();
}
```
在下面的示例中，正如看到的，可以通过`firstEntry()`和`lastEntry()`方法访问第一个和最后一个元素。<br />`pollFirstEntry()`方法将删除并返回第一个键值元素，如果映射为空，则返回 null。<br />此外，调用`reverse()`只会比较元素，而不依赖于它们的顺序。
```java
SequencedMap<String, Integer> myMap = new LinkedHashMap<>();
myMap.put("one", 1);
myMap.put("two", 2);
System.out.println(myMap); // {one=1, two=2}

Entry<String, Integer> firstEntry = myMap.firstEntry();
System.out.println(firstEntry); // one=1

Entry<String, Integer> lastEntry = myMap.lastEntry();
System.out.println(lastEntry); // two=2

myMap.putFirst("zero", 0);
System.out.println(myMap); // {zero=0, one=1, two=2}
myMap.putFirst("one", -1);
System.out.println(myMap); // {one=-1, zero=0, two=2}

Entry<String, Integer> polledFirstEntry = myMap.pollFirstEntry();
System.out.println(polledFirstEntry); // one=-1
System.out.println(myMap); // {zero=0, two=2}

SequencedMap<String, Integer> reversedMap = myMap.reversed();
System.out.println(reversedMap); // {two=2, zero=0}

boolean isEqual = myMap.equals(reversedMap);
System.out.println(isEqual); // true
System.out.println(myMap.hashCode()); // 692224
System.out.println(reversedMap.hashCode()); // 692224
System.out.println(myMap.hashCode() == reversedMap.hashCode()); // true
```
<a name="yOkqC"></a>
## 3、字符串模板
这是预览功能，默认禁用，需要使用`--enable-preview`启用字符串模板。<br />首先，在深入探讨字符串模板之前，将探讨一些用于组合字符串的技术。<br />**+（加号）运算符：**最大的缺点是每次使用 + 运算符时都会创建一个新字符串。<br />`**StringBuffer**`** 和 **`**StringBuilder**`**：** `StringBuffer` 是线程安全的，而 `StringBuilder` 是在 Java 5 中添加的，性能更高，但不是线程安全的替代方案。<br />它们的主要缺点是冗长，尤其是对于更简单的字符串：
```java
var greeting = new StringBuilder()
.append("Hello, welcome ")
.append(name)
.toString();
```
`**String::format**`** 和 **`**String::formatter**`**：** 它们允许可重用模板，但它们要求指定格式并以正确的顺序提供变量。
```java
var format = "Good morning %s, It's a beautiful day!";
var text = String.format(format, name);
// Java 15+
var text = format.formatter(name);
```
尽管节省了字符串分配的数量，但现在 JVM 必须解析/验证模板字符串。<br />`**java.text.MessageFormat**`**：** 与String格式相同，但更详细
```java
var format = new MessageFormat("Good morning {0}, It's a beautiful day!");
var greeting = format.format(name);
```
<a name="FvGVF"></a>
### 现在有字符串模板来拯救
它简单、简洁，处理字符串的新方法称为模板表达式。它们可以执行插值，还为我们提供了组合字符串的灵活性，并将结构化文本转换为任何对象，而不仅仅是字符串。<br />模板表达式由三个组成部分组成：

- 模板处理器：Java 提供了两种用于执行字符串插值的模板处理器：STR 和 FMT
- 包含包装表达式的模板，如 `{name}`
- 点 (`.`) 字符

以下是一些关于如何将字符串模板与模板处理器一起使用的示例：
```java
package com.mina.stringtemplates;

import static java.util.FormatProcessor.FMT;

import java.time.LocalDate;
import java.time.format.DateTimeFormatter;

public class StringTemplateExamples {

    public static String greeting(String firstName, String lastName) {
        return STR."Hello! Good morning \{ firstName } \{ lastName }" ;
    }

    public static String multiplyWithArithmeticExpressions(int a, int b) {
        return STR."\{ a } times \{ b } = \{ a * b }" ;
    }

    public static String multiplyWithJavaExpression(int a, int b) {
        return STR."\{ a } times \{ b } = \{ Math.multiplyExact(a, b) }" ;
    }

    //  multiplication with floating point numbers rounded to two decimal places using the FMT template processor
    public static String multiplyFloatingNumbers(double a, double b) {
        return FMT."%.2f\{ a } times %.2f\{ b } = %.2f\{ a * b }" ;
    }

    public static String getErrorResponse(int httpStatus, String errorMessage) {
        return STR."""
        {
        "httpStatus": \{ httpStatus },
        "errorMessage": "\{ errorMessage }"
    }""" ;
    }

    public static String getCurrentDate() {
        return STR."Today's date: \{
            LocalDate.now().format(
                DateTimeFormatter.ofPattern("yyyy-MM-dd")
            ) }" ;
    }
}
```
<a name="GqM0y"></a>
## 4、记录模式
记录模式匹配是一种在单个步骤中匹配记录类型并访问其组件的方法。<br />我们用它来测试一个值是否是记录类类型的实例，如果是，则对其组件值执行模式匹配。<br />下面的示例测试是否是具有记录模式transaction的记录实例`TransactionTransaction(String type, double amount)`
```java
package com.mina.recordpattern;

public class RecordPatternExample {

    // I'm using "_" for readability here, this won't compile
    public static String getTransactionType(Transaction transaction) {
        return switch (transaction) {
            case null -> throw new IllegalArgumentException("Transaction can not be null.");
            case Transaction(String type, double amount) when type.equals("Deposit") && amount > 0 -> "Deposit";
            case Transaction(String type, _) when type.equals("Withdrawal") -> "Withdrawal";
            default -> "Unknown transaction type";
        };
    }

    record Transaction(String type, double amount) {

    }
}
```
如果事务为空，会发生什么？抛出了一个空指针异常。这也是Java 21中的情况，但是现在可以通过写case null->来显式地使用null case，这样可以避免NullPointerException。<br />保护模式：也可以保护特定情况。例如，使用when关键字来检查相等性。
<a name="qTxhX"></a>
## 5、switch 模式匹配
switch模式匹配在 Java 17 中作为预览功能引入，并在 Java 21 中永久保留。<br />语句switch将控制转移到多个语句或表达式之一，具体取决于其选择器表达式的值（可以是任何类型），并且case标签可以具有模式。<br />它检查其选择器表达式是否与模式匹配，与测试其选择器表达式是否完全等于常量相比，这更具可读性和灵活性。
```java
package com.mina.switchpatternmatching;

import com.mina.switchpatternmatching.SwitchPatternMatchingExample.Transaction.Deposit;
import com.mina.switchpatternmatching.SwitchPatternMatchingExample.Transaction.Withdrawal;

public class SwitchPatternMatchingExample {

    public static String getTransactionType(Transaction transaction) {
        return switch (transaction) {
            case null:
                throw new IllegalArgumentException("Transaction can't be null.");
            case Deposit deposit when deposit.getAmount() > 0: //  Guarded pattern with when clause
                yield "Deposit";
            case Withdrawal withdrawal:
            yield "Withdrawal";
            default:
                yield "Unknown transaction type";
        };
    }

    sealed class Transaction permits Deposit, Withdrawal {

        private double amount;

        public Transaction(double amount) {
        this.amount = amount;
    }

    public double getAmount() {
        return amount;
    }


    final class Withdrawal extends Transaction {

        public Withdrawal(double amount) {
            super(amount);
        }
    }

    final class Deposit extends Transaction {

        public Deposit(double amount) {
            super(amount);
        }
    }
}
```
