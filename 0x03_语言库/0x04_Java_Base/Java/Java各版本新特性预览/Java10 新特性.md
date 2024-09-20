Java
<a name="hZuWu"></a>
## Java10新特性预览
jdk1.10正式版将在2018年3月份发布，jdk1.10变化内容：<br />1.局部变量类型推断:局部变量类型推断将引入"var"关键字，也就是可以随意定义变量而不必指定变量的类型.<br />2.GC改进和内存管理<br />3.线程本地握手（JEP 312）<br />4.备用内存设备上的堆分配（JEP 316）<br />5.其他Unicode语言 - 标记扩展（JEP 314）<br />6.基于Java的实验性JIT编译器<br />7.根证书（JEP 319）<br />9.将JDK生态整合单个存储库（JEP 296）<br />10.删除工具javah（JEP 313）

---

- 局部变量类型推断，举个例子，`var list = new ArrayList();`，可以使用 var 来作为变量类型，Java 编译器知道 list 的类型为字符串的 ArrayList。
- 增强 `java.util.Locale`。
- 提供了一组默认的根证书颁发机构（CA）。

![](https://cdn.nlark.com/yuque/0/2020/png/396745/1602515175551-e41cf485-ffe0-4663-bbaf-684f014d79b2.png#height=336&id=zuSzc&originHeight=336&originWidth=1077&originalType=binary&ratio=1&size=0&status=done&style=shadow&width=1077)
<a name="OYT2S"></a>
### 1、局部变量类型推断
JDK 10增加了局部变量类型推断（Local-Variable Type Inference）功能，让 Java 可以像Js里的var一样可以自动推断数据类型。Java中的var是一个保留类型名称，而不是关键字。<br />JDK 10之前
```java
List<String> list = new ArrayList<String>();
Stream<Integer> stream = Stream.of(1, 2, 3);
```
JDK 10 之后
```java
var list = new ArrayList<String>(); // ArrayList<String>
var stream =  Stream.of(1, 2, 3);
```
var 变量类型推断的使用也有局限性，仅「局限」于以下场景：

- 具有初始化器的局部变量
- 增强型for循环中的索引变量
- 传统for循环中声明的局部变量

而「不能用于」

- 推断方法的参数类型
- 构造函数参数类型推断
- 推断方法返回类型
- 字段类型推断
- 捕获表达式

Scala 和 Kotlin 中有 val 关键字 ( `final var` 组合关键字)，Java10 中并没有引入。<br />Java 10 只引入了 var，而
```java
var id = 0;
var codefx = new URL("https://mp.weixin.qq.com/");
var list = new ArrayList<>();
var list = List.of(1, 2, 3);
var map = new HashMap<String, String>();
var p = Paths.of("src/test/java/Java9FeaturesTest.java");
var numbers = List.of("a", "b", "c");
for (var n : list)
    System.out.print(n+ " ");
```
var 关键字只能用于带有构造器的局部变量和 for 循环中。
```java
var count=null; //❌编译不通过，不能声明为 null
var r = () -> Math.random();//❌编译不通过,不能声明为 Lambda表达式
var array = {1,2,3};//❌编译不通过,不能声明数组
```
var 并不会改变 Java 是一门静态类型语言的事实，编译器负责推断出类型。
<a name="SJha7"></a>
#### var 不应该被滥用
虽然这样“爽起来了”，但是`var`也不应该被滥用。<br />下面这种写法明细可读性差，导致变量的类型需要你去DEBUG:
```java
var data = someObject.getData();
```
Stream流中也尽量不要使用：
```java
// 可读性差
var names= apples.stream()
    .map(Apple::getName)
    .collect(Collectors.toList());
```
因此，在使用`var`时应该保证必要的可读性。<br />另外，在多态这个重要的Java特性中，`var`表现的并不是很完美。如果`Fruit`有`Apple`和`Orange`两种实现。
```java
var x = new Apple();
```
如果对`x`重新赋值为`new Orange()`就会报错，因为编译后`x`的类型就已经固定下来了。所以`var`和泛型一样都是在编译过程中起了作用。必须保证`var`的类型是确定的。<br />`var`结合泛型的钻石符号`<>`会有什么情况发生呢？<br />下面的 `empList`的类型是`ArrayList<Object>`：
```java
var empList = new ArrayList<>();
```
如果需要明确集合中放的都是`Apple`就必须在右边显式声明：
```java
var apples = new ArrayList<Apple>();
```
<a name="78aa94d3"></a>
### 2、不可变集合的改进
JDK 10中，List，Set，Map 提供了一个新的静态方法`copyOf(Collection<? extends E> coll)`，它返回Collection集合一个不可修改的副本。<br />JDK 源码：
```java
static <E> List<E> copyOf(Collection<? extends E> coll) {
    return ImmutableCollections.listCopy(coll);
}
```
使用实例：
```java
var oldList = new ArrayList<String>();
oldList.add("Hello");
oldList.add("Fcant");
var copyList = List.copyOf(oldList);
oldList.add("World"); 
copyList.add("Fcscanf");  //UnsupportedOperationException异常
```
使用 `copyOf()` 创建的集合为不可变集合，不能进行添加、删除、替换、 排序等操作，不然会报 `java.lang.UnsupportedOperationException` 异常。IDEA 也会有相应的提示。<br />![2021-09-06-16-57-05-467867.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630920203707-b71307cb-7a4f-4548-bdd2-62ea458ab636.png#clientId=u8e70e1a2-7bd6-4&from=ui&id=u4d142452&originHeight=67&originWidth=712&originalType=binary&ratio=1&size=21709&status=done&style=none&taskId=u463b302a-64a6-4ee5-916d-53a53a2ec67)<br />`java.util.stream.Collectors` 中新增了静态方法，用于将流中的元素收集为不可变的集合。
```java
var list = new ArrayList<>();
list.stream().collect(Collectors.toUnmodifiableList());
list.stream().collect(Collectors.toUnmodifiableSet());
```

- 不可变性（immutability），这是函数式编程的基石之一，因此加强不可变集合有助于函数式编程在Java中的发展。
- 安全性，由于集合不可变，因此就不存在竞态条件，天然的线程安全性，无论在编码过程中和内存使用中都有一定的优势，这种特性在Scala和Kotlin这两种编程语言中大放异彩。

在Java 10 中又引入了一些新的API。
<a name="pTU3z"></a>
#### 集合副本
复制一个集合为不可变集合：
```java
 List<Apple> copyList = List.copyOf(apples);
```
任何修改此类集合的尝试都会导致`java.lang.UnsupportedOperationException`异常。
<a name="q3EJn"></a>
#### Stream归纳为不可变集合
之前Stream API的归纳操作`collect(Collector collector)`都只会把流归纳为可变集合，现在它们都有对应的不可变集合了。举个例子：
```java
List<String> names= apples.stream()    .map(Apple::getName)    .collect(Collectors.toUnmodifiableList());
```
 
<a name="a4ed5ec5"></a>
### 3、并行全垃圾回收器 G1
> JDK 9引入 G1 作为默认垃圾收集器，执行GC 时采用的是基于单线程标记扫描压缩算法（mark-sweep-compact）。为了最大限度地减少 Full GC 造成的应用停顿的影响，Java 10 中将为 G1 引入多线程并行 GC，同时会使用与年轻代回收和混合回收相同的并行工作线程数量，从而减少了 Full GC 的发生，以带来更好的性能提升、更大的吞吐量。

从 Java9 开始 G1 就了默认的垃圾回收器，G1 是以一种低延时的垃圾回收器来设计的，旨在避免进行 Full GC,但是 Java9 的 G1 的 FullGC 依然是使用单线程去完成标记清除算法，这可能会导致垃圾回收期在无法回收内存的时候触发 Full GC。<br />为了最大限度地减少 Full GC 造成的应用停顿的影响，从 Java10 开始，G1 的 FullGC 改为并行的标记清除算法，同时会使用与年轻代回收和混合回收相同的并行工作线程数量，从而减少了 Full GC 的发生，以带来更好的性能提升、更大的吞吐量。
<a name="WXlH2"></a>
### 4、线程本地握手
Java 10 中线程管控引入JVM安全点的概念，将允许在不运行全局JVM安全点的情况下实现线程回调，由线程本身或者JVM线程来执行，同时保持线程处于阻塞状态，这将会很方便使得停止单个线程或不停止线程成为可能。
<a name="n1j6M"></a>
### 5、`Optional`新增`orElseThrow()`方法
`Optional`、`OptionalDouble`等类新增一个方法`orElseThrow()`，在没有值时抛出异常
```java
Optional.ofNullable(cache.getIfPresent(key))
        .orElseThrow(() -> new PrestoException(NOT_FOUND, "Missing entry found for key: " + key));
```
`Optional`如果值为`null`时去`get`会抛出`NoSuchElementException`异常。从语义上`get`应该肯定能得到什么东西，但是实际上异常了，这种歧义性太大了。所以增加了一个`orElseThrow()`方法来增强语义性。
```java
Optional<String> optional = Optional.ofNullable(nullableVal);        
// 可能会 NoSuchElementException        String  nullable = optional.get();
```
<a name="eg2Xa"></a>
### 6、应用程序类数据共享(扩展 CDS 功能)
在 Java 5 中就已经引入了类数据共享机制 (Class Data Sharing，简称 CDS)，允许将一组类预处理为共享归档文件，以便在运行时能够进行内存映射以减少 Java 程序的启动时间，当多个 Java 虚拟机（JVM）共享相同的归档文件时，还可以减少动态内存的占用量，同时减少多个虚拟机在同一个物理或虚拟的机器上运行时的资源占用。CDS 在当时还是 Oracle JDK 的商业特性。<br />Java 10 在现有的 CDS 功能基础上再次拓展，以允许应用类放置在共享存档中。CDS 特性在原来的 bootstrap 类基础之上，扩展加入了应用类的 CDS 为 (Application Class-Data Sharing，AppCDS) 支持，大大加大了 CDS 的适用范围。其原理为：在启动时记录加载类的过程，写入到文本文件中，再次启动时直接读取此启动文本并加载。设想如果应用环境没有大的变化，启动速度就会得到提升。
<a name="CkFy7"></a>
### 7、Java10 其他新特性

- 线程-局部管控：Java 10 中线程管控引入 JVM 安全点的概念，将允许在不运行全局 JVM 安全点的情况下实现线程回调，由线程本身或者 JVM 线程来执行，同时保持线程处于阻塞状态，这种方式使得停止单个线程变成可能，而不是只能启用或停止所有线程
- 备用存储装置上的堆分配：Java 10 中将使得 JVM 能够使用适用于不同类型的存储机制的堆，在可选内存设备上进行堆内存分配
- 统一的垃圾回收接口：Java 10 中，hotspot/gc 代码实现方面，引入一个干净的 GC 接口，改进不同 GC 源代码的隔离性，多个 GC 之间共享的实现细节代码应该存在于辅助类中。统一垃圾回收接口的主要原因是：让垃圾回收器（GC）这部分代码更加整洁，便于新人上手开发，便于后续排查相关问题。
- 基于 Java 的 实验性 JIT 编译器
- 类数据共享
- Unicode 语言标签扩展
- 根证书
- 基于时间（Time-Based）的版本控制模型
