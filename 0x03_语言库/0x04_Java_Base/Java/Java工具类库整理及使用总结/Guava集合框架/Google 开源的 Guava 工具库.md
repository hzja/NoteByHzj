Java Guava
<a name="puO6J"></a>
## 目录

- Google Guava 概述
- 不可变集合与对象
- Guava 新集合类型
- JDK 集合辅助工具类
- JDK 基本类型辅助工具类
- JDK 字符串辅助工具类
- Stopwatch 秒表

---

<a name="mCiYk"></a>
## Google Guava 概述
1、Guava 是一组来自 Google 的核心 Java 库，包括新的集合类型（如 multimap 和 multiset）、不可变集合、图形库以及用于并发、I/O、散列、缓存、原语、字符串等的实用程序！被广泛应用于 Google 的大多数 Java 项目中，也被许多其他公司广泛使用。<br />2、guava github 开源地址：GitHub - google/guava: [https://github.com/google/guava](https://github.com/google/guava)<br />3、官网用户手册：[https://github.com/google/guava/wiki](https://github.com/google/guava/wiki)<br />4、com.google.guava 依赖：
```xml
<dependency>
	<groupId>com.google.guava</groupId>
	<artifactId>guava</artifactId>
	<version>29.0-jre</version>
</dependency>
```
<a name="RYBKy"></a>
## 不可变集合与对象
1、制作对象的不可变副本是一种很好的防御性编程技术，不可变对象有许多优点，包括：

- 可供不受信任的库安全使用。
- 线程安全：可由多个线程使用，无争用风险。
- 不需要支持突变，并且可以节省时间和空间，所有不可变的集合实现都比它们的可变同级更节省内存。
- 可以用作常数，并期望它将保持不变。

2、要点：每个 Guava 不可变集合实现都拒绝 null 值。Guava 的设计上推荐使用 null 值，大多数情况下，遇到 null 值会抛异常.<br />3、一个不可变的 `ImmutableXxx` 集合可以通过以下几种方式创建：

- 使用 `copyOf` 方法，如 `ImmutableSet.copyOf(set)`
- 使用 `of` 方法，如 `ImmutableSet.of("a", "b", "c")` 或 `ImmutableMap.of("a", 1, "b", 2)`
- 使用 `Builder` 方法。

4、Guava 为 java jdk 每种标准集合类型提供了简单易用的不可变版本，包括 Guava 自己的集合变体，为 java 提供的不可变版本都是继承 java jdk 的接口而来，所以操作上基本无异。下面接口的实现类也都有对应的不可变版本。

| 接口 | JDK 或者 Guava | 不可变版本 |
| --- | --- | --- |
| Collection | JDK | ImmutableCollection |
| List | JDK | ImmutableList |
| Set | JDK | ImmutableSet |
| SortedSet/NavigableSet | JDK | ImmutableSortedSet |
| Map | JDK | ImmutableMap |
| SortedMap | JDK | ImmutableSortedMap |
| Multiset | Guava | ImmutableMultiset |
| SortedMultiset | Guava | ImmutableSortedMultiset |
| Multimap | Guava | ImmutableMultimap |
| ListMultimap | Guava | ImmutableListMultimap |
| SetMultimap | Guava | ImmutableSetMultimap |
| BiMap | Guava | ImmutableBiMap |
| ClassToInstanceMap | Guava | ImmutableClassToInstanceMap |
| Table | Guava | ImmutableTable |

在线演示源码：[https://github.com/main/java/com/wmx/guava/ImmutableCollectionTest.java](https://github.com/main/java/com/wmx/guava/ImmutableCollectionTest.java)<br />官网文档：[https://github.com/google/guava/wiki/ImmutableCollectionsExplained](https://github.com/google/guava/wiki/ImmutableCollectionsExplained)
<a name="G4roK"></a>
## Guava 新集合类型
1、Guava 引入了许多新的集合类型，这些类型不在 Java JDK 中，但却非常有用，这些都是为了与 JDK 集合框架愉快地共存而设计的，而不是将东西塞进 JDK 集合抽象中。
<a name="EMdJG"></a>
### Multiset 可重复集合
1、Guava 提供了一个新的集合类型 Multiset，它支持添加多个相同的元素，其中成员可以出现不止一次。<br />2、Multiset 相当于 Set，区别在于 Multiset 可添加相同的元素，它的内部使用一个 HashMap 来维护，<br />3、Multiset 也有自己的实现类，常用的有 HashMultiset、LinkedHashMultiset、TreeMultiset 等，HashMultiset 、TreeMultiset 是无序的，LinkedHashMultiset 是有序的，操作完全同理 JDK 的 HashSet、TreeSet、LinkedHashSet。<br />在线演示源码：[https://github.com/wangmaoxiong/apache-study/blob/master/src/main/java/com/wmx/guava/MultisetTest.java](https://github.com/wangmaoxiong/apache-study/blob/master/src/main/java/com/wmx/guava/MultisetTest.java)
<a name="SOzcd"></a>
### Multimap 多重映射
1、每个有经验的 Java 程序员都曾在某个地方实现过 `Map<K、List<V>>` 或 `Map<K、Set<V>>`，Guava 的 Multimap 框架使处理从键到多个值的映射变得容易，多重映射是将键与任意多个值关联的一种通用方法。<br />2、从概念上讲，有两种方法可以将多重映射视为从单个键到单个值的映射的集合：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650965533994-4ce36402-c456-42ef-90ef-8356dd71a597.png#clientId=ud1726023-dc00-4&from=paste&id=uc3ee863d&originHeight=85&originWidth=390&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ued5b97f2-3caa-4337-95f3-bf801193c03&title=)<br />3、Multimap 提供了多种实现：

| Multimap | 实现 key 使用的是 | value 使用的是 |
| --- | --- | --- |
| ArrayListMultimap | HashMap | ArrayList |
| HashMultimap | HashMap | HashSet |
| LinkedListMultimap * | LinkedHashMap* | LinkedList* |
| LinkedHashMultimap** | LinkedHashMap | LinkedHashSet |
| TreeMultimap | TreeMap | TreeSet |
| ImmutableListMultimap | ImmutableMap | ImmutableList |
| ImmutableSetMultimap | ImmutableMap | ImmutableSet |

4、除了不可变的实现之外，每个实现都支持空键和值。并不是所有的实现都是作为一个`Map<K，Collection<V>>`实现的（特别是一些Multimap实现使用自定义哈希表来最小化开销。）<br />在线演示源码：[https://github.com/wangmaoxiong/apache-study/blob/master/src/main/java/com/wmx/guava/MultimapTest.java](https://github.com/wangmaoxiong/apache-study/blob/master/src/main/java/com/wmx/guava/MultimapTest.java)
<a name="NXMWK"></a>
### BiMap 双向映射
1、将值映射回键的传统方法是维护两个独立的映射，并使它们保持同步，但这很容易产生错误，并且当映射中已经存在一个值
```java
Map<String, Integer> nameToId = Maps.newHashMap();
Map<Integer, String> idToName = Maps.newHashMap();
 
nameToId.put("Bob", 42);
idToName.put(42, "Bob");
```
2、BiMap 提供了多种实现：

| 键值映射实现 | 值键映射实现 | 对应BiMap |
| --- | --- | --- |
| HashMap | HashMap | HashBiMap |
| ImmutableMap | ImmutableMap | ImmutableBiMap |
| EnumMap | EnumMap | EnumBiMap |
| EnumMap | HashMap | EnumHashBiMap |

在线演示源码：[https://github.com/wangmaoxiong/apache-study/blob/master/src/main/java/com/wmx/guava/BiMapTest.java](https://github.com/wangmaoxiong/apache-study/blob/master/src/main/java/com/wmx/guava/BiMapTest.java)
<a name="duhCC"></a>
### Table 表结构数据
1、当试图一次在多个键上建立索引时，将得到类似 `Map<FirstName，Map<LastName，Person>>` 的代码，这很难看，而且使用起来很尴尬。Guava 提供了一个新的集合类型 Table，它支持任何“row”类型和“column”类型的这个用例。<br />2、Table 提供了多种实现：

- `HashBasedTable`：基本上是由 `HashMap<R，HashMap<C，V>>` 支持的。
- `TreeBasedTable`：基本上是由 `TreeMap<R，TreeMap<C，V>>` 支撑的。
- `ImmutableTable`
- `ArrayTable`：要求在构造时指定行和列的完整范围，但在表密集时由二维数组支持以提高速度和内存效率，`ArrayTable`的工作原理与其他实现有些不同

在线演示源码：[https://github.com/wangmaoxiong/apache-study/blob/master/src/main/java/com/wmx/guava/TableTest.java](https://github.com/wangmaoxiong/apache-study/blob/master/src/main/java/com/wmx/guava/TableTest.java)
<a name="ncBAB"></a>
### ClassToInstanceMap 类型映射到实例
1、有时 key 并不是单一的类型，而是多种类型，Guava 为此提供了 `ClassToInstanceMap`，key 可以是多种类型，value 是此类型的实例。<br />2、ClassToInstanceMap 的实现有： `MutableClassToInstanceMap` 和 `ImmutableClassToInstanceMap` 的实现。<br />在线演示源码：[https://github.com/wangmaoxiong/src/main/java/com/wmx/guava/ClassToInstanceMapTest.java](https://github.com/wangmaoxiong/src/main/java/com/wmx/guava/ClassToInstanceMapTest.java)
<a name="G17Os"></a>
## JDK 集合辅助工具类
1、任何有 JDK 集合框架经验的程序员都知道并喜欢其中提供的实用程序 java.util.Collections，Guava 提供了许多适用于集合的静态方法实用程序。

| 接口 | 属于 JDK 还是 Guava | 对应 Guava API |
| --- | --- | --- |
| Collection | JDK | Collections2 |
| List | JDK | Lists |
| Set | JDK | Sets |
| SortedSet | JDK | Sets |
| Map | JDK | Maps |
| SortedMap | JDK | Maps |
| Queue | JDK | Queues |
| Multiset | Guava | Multisets |
| Multimap | Guava | Multimaps |
| BiMap | Guava | Maps |
| Table | Guava | Tables |

Lists 在线演示：[https://github.com/wangmaoxiong/apache-study/blob/master/src/main/java/com/wmx/guava/ListsTest.java](https://github.com/wangmaoxiong/apache-study/blob/master/src/main/java/com/wmx/guava/ListsTest.java)<br />Sets 在线演示：[https://github.com/wangmaoxiong/apache-study/blob/master/src/main/java/com/wmx/guava/SetsTest.java](https://github.com/wangmaoxiong/apache-study/blob/master/src/main/java/com/wmx/guava/SetsTest.java)
<a name="Z1byA"></a>
## JDK 基本类型辅助工具类
1、Guava 为 Java JDK 的基本类型提供了实用程序类：

| 基本类型 | Guava 辅助工具类 |
| --- | --- |
| byte | Bytes, SignedBytes, UnsignedBytes |
| short | Shorts |
| int | Ints, UnsignedInteger, UnsignedInts |
| long | Longs, UnsignedLong, UnsignedLongs |
| float | Floats |
| double | Doubles |
| char | Chars |
| boolean | Booleans |

nts 在线演示源码：[https://github.com/wangmaoxiong/apache-study/blob/master/src/main/java/com/wmx/guava/IntsTest.java](https://github.com/wangmaoxiong/apache-study/blob/master/src/main/java/com/wmx/guava/IntsTest.java)<br />doubles 在线演示源码：[https://github.com/src/main/java/com/wmx/guava/DoublesTest.java](https://github.com/src/main/java/com/wmx/guava/DoublesTest.java)<br />booleans 在线演示源码：[https://github.com/src/main/java/com/wmx/guava/BooleansTest.java](https://github.com/src/main/java/com/wmx/guava/BooleansTest.java)
<a name="JmCwG"></a>
## JDK 字符串辅助工具类
1、Strings 类中提供了少数几个常用的符串实用程序。<br />在线演示源码：[https://github.com/wangmaoxiong/apache-study/blob/master/src/main/java/com/wmx/guava/StringsTest.java](https://github.com/wangmaoxiong/apache-study/blob/master/src/main/java/com/wmx/guava/StringsTest.java)<br />2、Joiner 是连接器，用于连接 `java.lang.Iterable`、`java.util.Iterator`、`java.lang.Object[]` 中的元素。<br />在线演示源码：[https://github.com/wangmaoxiong/apache-study/blob/master/src/main/java/com/wmx/guava/JoinerTest.java](https://github.com/wangmaoxiong/apache-study/blob/master/src/main/java/com/wmx/guava/JoinerTest.java)<br />3、Splitter 是分割器，用于分割字符序列 java.lang.CharSequence。<br />在线演示源码：[https://github.com/wangmaoxiong/apache-study/blob/master/src/main/java/com/wmx/guava/SplitterTest.java](https://github.com/wangmaoxiong/apache-study/blob/master/src/main/java/com/wmx/guava/SplitterTest.java)<br />4、CharMatcher 字符匹配器，用于匹配字符，可以将 CharMatcher 视为代表一类特定的字符，如数字或空白。注意：CharMatcher 只处理 char 值。<br />在线演示源码：[https://github.com/wangmaoxiong/apache-study/blob/master/src/main/java/com/wmx/guava/CharMatcherTest.java](https://github.com/wangmaoxiong/apache-study/blob/master/src/main/java/com/wmx/guava/CharMatcherTest.java)
<a name="QIOp9"></a>
## Stopwatch 秒表
1、google 的秒表 Stopwatch 相比 Spring framewrk core 包 和 apache commons lang3 包的秒表是最方便使用的。<br />2、此类不是线程安全的。
```java
/**
 * Stopwatch createStarted()：创建（并启动）一个新的秒表，使用 System#nanoTime 来作为其时间源。
 * Stopwatch createUnstarted()：创建（但不启动）一个新的秒表，使用 System#nanoTime 来作为其时间源。
 * long elapsed(TimeUnit desiredUnit)：返回此秒表上显示的当前已用时间，以所需的时间单位表示，任何分数向下舍入
 * boolean isRunning()：如果已在此秒表上调用start（）}，并且自上次调用start（）以来未调用stop（），则返回true
 * Stopwatch reset()：将此秒表的运行时间设置为零，并将其置于停止状态。
 * Stopwatch start()：启动秒表,如果秒表已经在运行，则 IllegalStateException
 * Stopwatch stop()：停止秒表，将来的读取将返回到目前为止经过的固定持续时间。
 * tring toString()：返回当前运行时间的字符串表示形式，比如 2.588 s，106.8 ms
 */
@Test
public void testStopwatch() throws InterruptedException {
	SecureRandom secureRandom = new SecureRandom();
	Stopwatch stopwatch = Stopwatch.createStarted();
	
	int nextInt = secureRandom.nextInt(2000);
	System.out.println("任务1预算耗时：" + nextInt);//任务1预算耗时：81
	TimeUnit.MILLISECONDS.sleep(nextInt);
	System.out.println("\t任务1实际耗时：" + stopwatch.elapsed(TimeUnit.MILLISECONDS) + "(毫秒)");// 任务1实际耗时：563(毫秒)
	
	stopwatch.reset().start();
	nextInt = secureRandom.nextInt(4000);
	System.out.println("任务2预算耗时：" + nextInt);//任务2预算耗时：1591
	TimeUnit.MILLISECONDS.sleep(nextInt);
	System.out.println("\t任务2实际耗时：" + stopwatch.toString());// 任务2实际耗时：1.592 s
	
	stopwatch.reset().start();
	nextInt = secureRandom.nextInt(3000);
	System.out.println("任务3预计耗时：" + nextInt);//任务3预计耗时：1964
	TimeUnit.MILLISECONDS.sleep(nextInt);
	System.out.println("\t任务3实际耗时：" + stopwatch.stop().toString());// 任务3实际耗时：1.965 s
}
```
