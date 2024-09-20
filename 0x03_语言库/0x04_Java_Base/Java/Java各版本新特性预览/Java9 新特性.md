Java
<a name="f065bd23"></a>
## Java9新特性预览
jdk1.9 正式版将在2017年9月份发布，jdk1.9主要变化内容有：<br />1. Java 平台级模块系统<br />2. Linking<br />3. JShell : 交互式 Java REPL<br />4. 改进的 Javadoc<br />5. 集合工厂方法<br />6. 改进的 Stream API<br />7. 私有接口方法<br />8. HTTP/2<br />9. 多版本兼容 JAR

---

- 模块系统
- 不可变的 List、Set、Map 的工厂方法
- 接口中可以有私有方法
- 垃圾收集器改进

![](https://cdn.nlark.com/yuque/0/2020/png/396745/1602514711901-16fa31c5-fbc5-49ef-acd2-41418ae8c0ed.png#height=547&id=mVfDA&originHeight=547&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=1080)
<a name="N2fyu"></a>
### 1、 java模块系统
<a name="1ZDVw"></a>
#### 模块化
> 一个大型系统，比如一个商城网站，它会包含很多模块的，如：订单模块，用户信息模块，商品信息模块，广告位模块等等。各个模块之间会相互调用。如果每个模块单独运行都会带动其他所有模块，性能非常低效。但是，如果某一模块运行时，只会启动它所依赖的模块，性能大大提升。这就是JDK 9模块化的思想。

<a name="cExxb"></a>
#### JDK9模块化
> Java 平台模块系统，即Project Jigsaw，把模块化开发实践引入到了Java平台中。在引入了模块系统之后，JDK 被重新组织成94个模块。Java 应用可以通过新增的jlink 工具，创建出只包含所依赖的JDK模块的自定义运行时镜像。这样可以极大的减少Java运行时环境的大小。

<a name="rzin3"></a>
#### Java 9 模块的重要特征：
> - 在其工件（artifact）的根目录中包含了一个描述模块的 module-info.class 文 件。
> - 工件的格式可以是传统的 JAR 文件或是 Java 9 新增的 JMOD 文件。
> - 这个文件由根目录中的源代码文件 module-info.java 编译而来。
> - 该模块声明文件可以描述模块的不同特征。

在 module-info.java 文件中，可以用新的关键词module来声明一个模块，如下所示。下面给出了一个模块com.mycompany.mymodule的最基本的模块声明
```java
module com.jay.sample {   //关键词module来声明一个模块
    exports com.jay.sample; //使用 exports可以声明模块对其他模块所导出的包。
    requires com.jay.common; //使用requires可以声明模块对其他模块的依赖关系。
}
```
简单来说，可以将一个模块看作是一组唯一命名、可重用的包、资源和模块描述文件（module-info.java）。<br />任意一个 jar 文件，只要加上一个 模块描述文件（module-info.java），就可以升级为一个模块。<br />![2021-09-06-16-57-04-661867.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630918985194-caac3e14-aaed-4814-bd39-02d3fcc680c9.png#clientId=u3ee6f248-ac9d-4&from=ui&id=u86cd4ef4&originHeight=267&originWidth=657&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2944&status=done&style=none&taskId=u8add3da2-d93b-4234-aac7-72f69f40473&title=)<br />在引入了模块系统之后，JDK 被重新组织成 94 个模块。Java 应用可以通过新增的 jlink 工具，创建出只包含所依赖的 JDK 模块的自定义运行时镜像。这样可以极大的减少 Java 运行时环境的大小。<br />可以通过 `exports` 关键词精准控制哪些类可以对外开放使用，哪些类只能内部使用。
```java
module my.module {
    //exports 公开指定包的所有公共成员
    exports com.my.package.name;
}

module my.module {
     //exports…to 限制访问的成员范围
    export com.my.package.name to com.specific.package;
}
```
Java 9 模块的重要特征是在其工件（artifact）的根目录中包含了一个描述模块的 module-info.java 文 件。工件的格式可以是传统的 JAR 文件或是 Java 9 新增的 JMOD 文件。
<a name="ab6ba3f1"></a>
### 2、不可变集合工厂方法
为了创建不可变集合，JDK9之前是不可以的：
```java
List<String> stringList = new ArrayList<>();
stringList.add("Hello");
stringList.add("Fcant");
List<String> unmodifiableList = Collections.unmodifiableList(stringList);
```
JDK 9 提供了`List.of()`、`Set.of()`、`Map.of()`和`Map.ofEntries()`等工厂方法来创建不可变集合：
```java
List<String> unmodifiableList = List.of("Hello","Fcant");
```
使用 `of()` 创建的集合为不可变集合，不能进行添加、删除、替换、 排序等操作，不然会报 `java.lang.UnsupportedOperationException` 异常。<br />`Collectors` 中增加了新的方法 `filtering()` 和 `flatMapping()`。<br />`Collectors` 的 `filtering()` 方法类似于 `Stream` 类的 `filter()` 方法，都是用于过滤元素。<br />Java 8 为 `Collectors` 类引入了 `groupingBy` 操作，用于根据特定的属性将对象分组。
```java
List<String> list = List.of("x","www", "yy", "zz");
Map<Integer, List<String>> result = list.stream()
        .collect(Collectors.groupingBy(String::length,
                Collectors.filtering(s -> !s.contains("z"),
                        Collectors.toList())));

System.out.println(result); // {1=[x], 2=[yy], 3=[www]}
```
<a name="shSgS"></a>
### 3、接口支持私有方法
JDK 8支持在接口实现默认方法和静态方法，但是不能在接口中创建私有方法，为了避免了代码冗余和提高阅读性，JDK 9在接口中支持私有方法。
```java
public interface IPrivateInterfaceTest {
    //JDK 7 之前
    String a = "jay";
    void method7();
    //JDK 8
    default void methodDefault8(){
        System.out.println("JDK 8新特性默认方法");
    }
    static void methodStatic8() {
        System.out.println("JDk 8新特性静态方法");
    }
    
    //Java 9 接口支持私有方法
    private void method9(){}
}
```
现在可以在接口里定义私有方法，然后在默认方法里调用接口的私有方法。<br />这样一来，既可以重用私有方法里的代码，又可以不公开代码
```java
public interface TestInterface {
    default void wrapMethod(){
        innerMethod();
    }
    private void innerMethod(){
        System.out.println("");
    }
}
```
<a name="Wlczv"></a>
### 4、钻石操作符升级

- 钻石操作符是在 java 7 中引入的，可以让代码更易读，但它不能用于匿名的内部类。
- 在 java 9 中， 它可以与匿名的内部类一起使用，从而提高代码的可读性。
```java
//JDK 5,6
Map<String, String> map56 = new HashMap<String,String>();
//JDk 7,8
Map<String, String> map78 = new HashMap<>();
//JDK 9 结合匿名内部类的实现
Map<String, String> map9 = new HashMap<>(){};
```
<a name="b7wom"></a>
### 5、Optional 类改进
java 9 中，java.util.Optional 添加了很多新的有用方法，如：

- `stream()` Optional现在可以转`Stream`。
- `ifPresentOrElse(Consumer<? super T> action, Runnable emptyAction)` 如果有值了怎么消费，没有值了怎么消费。
- `or(Supplier<? extends Optional<? extends T>> supplier)` 如果有值就返回有值的`Optional`，否则就提供能获取一个有值的`Optional`的渠道（`Supplier`）。

`ifPresentOrElse` 方法的改进就是有了 else，接受两个参数 Consumer 和 Runnable。
```java
import java.util.Optional;
 
public class OptionalTest {
   public static void main(String[] args) {
      Optional<Integer> optional = Optional.of(1);
 
      optional.ifPresentOrElse( x -> System.out.println("Value: " + x),() -> 
         System.out.println("Not Present."));
 
      optional = Optional.empty();
 
      optional.ifPresentOrElse( x -> System.out.println("Value: " + x),() -> 
         System.out.println("Not Present."));
   }  
}
```
`Optional` 类中新增了 `ifPresentOrElse()`、`or()` 和 `stream()` 等方法<br />`ifPresentOrElse()` 方法接受两个参数 `Consumer` 和 `Runnable` ，如果 `Optional` 不为空调用 `Consumer` 参数，为空则调用 `Runnable` 参数。
```java
public void ifPresentOrElse(Consumer<? super T> action, Runnable emptyAction)

Optional<Object> objectOptional = Optional.empty();
objectOptional.ifPresentOrElse(System.out::println, () -> System.out.println("Empty!!!"));// Empty!!!
```
`or()` 方法接受一个 `Supplier` 参数 ，如果 `Optional` 为空则返回 `Supplier` 参数指定的 `Optional` 值。
```java
public Optional<T> or(Supplier<? extends Optional<? extends T>> supplier)

Optional<Object> objectOptional = Optional.empty();
objectOptional.or(() -> Optional.of("java")).ifPresent(System.out::println);
```
<a name="2cc0c42a"></a>
### 6、多版本兼容Jar包
很多公司使用的JDK都是老版本的，JDK6、JDk5 ，甚至JDk4的，不是他们不想升级JDk版本，而是担心兼容性问题。JDK 9的一个新特性，多版本兼容Jar包解决了这个问题。举个例子：假设一直用的是小米8，已经非常习惯它的运行流程了，突然出来小米9，即使小米9很多新功能引人入胜，但是有些人不会轻易买小米9，因为已经已经习惯小米8的流程。同理，为什么很多公司不升级JDK，就是在此。但是呢，JDK 9的这个功能很强大，它可以让版本升级到JDK 9，但是还是老版本的运行流程，即在老的运行流程继承新的功能~
<a name="f5877d62"></a>
### 7、JShell工具
jshell 是 Java 9 新增的一个实用工具。为 Java 提供了类似于 Python 的实时命令行交互工具。<br />在 Jshell 中可以直接输入表达式并查看其执行结果。<br />![2021-09-06-16-57-04-930898.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630919023728-917ad790-50e3-4a8b-8f0f-e8dc766c91d6.png#clientId=u3ee6f248-ac9d-4&from=ui&id=udcb720ec&originHeight=316&originWidth=526&originalType=binary&ratio=1&rotation=0&showTitle=false&size=41253&status=done&style=none&taskId=u792c60fb-12e2-4f9b-b2a7-5a335676b7d&title=)<br />jShell工具相当于cmd工具，然后呢，可以像在cmd工具操作一样，直接在上面运行Java方法，Java语句等~
```java
jshell> System.out.println("Hello Fcant");
Hello Fcant
```
<a name="70b45440"></a>
### 8、try-with-resources的改进
在Java 7 中引入了try-with-resources功能，保证了每个声明了的资源在语句结束的时候都会被关闭。任何实现了`java.lang.AutoCloseable`接口的对象，和实现了`java.io.Closeable`接口的对象，都可以当做资源使用。<br />JDK 9对try-with-resources异常处理机制进行了升级~
```java
//JDK 7,8
try (BufferedReader br = new BufferedReader(new FileReader("d:七里香.txt")) {
   br.readLine();
}catch(IOException e){
  log.error("IO 异常，e:{}",e);
}
//JDk 9
BufferedReader br = new BufferedReader(new FileReader("d:七里香.txt")
try(br){
  br.readLine();
}catch(IOException e){
  log.error("IO 异常，e:{}",e);
}
```
但需要声明多个资源变量时，代码看着就有点难受了，需要在 try 中写多个变量的创建过程：
```java
try (BufferedReader bufferReader0 = new BufferedReader(...);
    BufferedReader bufferReader1 = new BufferedReader(...)) {
    return bufferReader0.readLine();
}
```
JAVA 9 中对这个功能进行了增强，可以引用 try 代码块之外的变量来自动关闭：
```java
BufferedReader bufferReader0 = new BufferedReader(...);
BufferedReader bufferReader1 = new BufferedReader(...);
try (bufferReader0; bufferReader1) {
    System.out.println(br1.readLine() + br2.readLine());
}
```
<a name="Hllqz"></a>
### 9、Stream API的改进
JDK 9 为Stream API引入以下这些方法，丰富了流处理操作：

- `takeWhile()`
- `dropWhile()`
- `iterate`
- `ofNullable`

Java 9 中的 `ofNullable()` 方法允许创建一个单元素的 `Stream`，可以包含一个非空元素，也可以创建一个空 `Stream`。而在 Java 8 中则不可以创建空的 `Stream` 。
```java
Stream<String> stringStream = Stream.ofNullable("Java");
System.out.println(stringStream.count());// 1
Stream<String> nullStream = Stream.ofNullable(null);
System.out.println(nullStream.count());//0
```
<a name="jeogL"></a>
#### 「`takeWhile`」
`Stream.takeWhile(Predicate)`   `Stream`中元素会被断言`Predicate`，一旦元素断言为`false`就中断操作，忽略掉没有断言的元素（及时未断言中的元素有满足条件的），仅仅把之前满足元素返回。<br />使用一个断言（Predicate 接口）作为参数，返回给定Stream的子集直到断言语句第一次返回 false
```java
// 语法格式
default Stream<T> takeWhile(Predicate<? super T> predicate)
//代码示例
Stream.of(1,2,3).takeWhile(s-> x<2)
         .forEach(System.out::println); 
 //输出
 1
```
```java
List<Integer> integerList = List.of(11, 33, 66, 8, 9, 13);
integerList.stream().takeWhile(x -> x < 50).forEach(System.out::println);
// 11 33
```
<a name="67768e7b"></a>
#### 「`dropWhile`」
与 `takeWhile()`作用相反，使用一个断言（Predicate 接口）作为参数，直到断言语句第一次返回true，返回给定Stream的子集
```java
//语法
default Stream<T> dropWhile(Predicate<? super T> predicate)
//代码示例
Stream.of(1,2,3).dropWhile(s-> x<2)
         .forEach(System.out::println);
//输出
2
3
```
```java
List<Integer> integerList2 = List.of(11, 33, 66, 8, 9, 13);
integerList2.stream().dropWhile(x -> x < 50).forEach(System.out::println);
// 66 8 9 13
```
<a name="SFTB8"></a>
#### 「`iterate`」
`iterate()` 方法能够返回以seed（第一个参数）开头，匹配 Predicate（第二个参数）直到返回false，并使用第三个参数生成下一个元素的元素流。<br />用来生成有限流的新迭代实现。

- `seed` 初始种子值
- `hasNext` 用来判断何时结束流，这个与`seed`有关。如何该函数不迭代保留`seed`计算，返回的流可能为空。
- `next`函数用来计算下一个元素值。
```java
//语法
static <T> Stream<T> iterate(T seed, Predicate<? super T> hasNext, UnaryOperator<T> next)
//代码示例
IntStream.iterate(2, x -> x < 10, x -> x*x).forEach(System.out::println);
//输出
2
4

Stream.iterate(0, i -> i < 5, i -> i + 1)
  .forEach(System.out::println);

//等同于传统的
for (int i = 0; i < 5; ++i) {
    System.out.println(i);
}
```
```java
public static<T> Stream<T> iterate(final T seed, final UnaryOperator<T> f) {
}
// 新增加的重载方法
public static<T> Stream<T> iterate(T seed, Predicate<? super T> hasNext, UnaryOperator<T> next) {

}
```
两者的使用对比如下，新的 `iterate()` 重载方法更加灵活一些。
```java
// 使用原始 iterate() 方法输出数字 1~10
Stream.iterate(1, i -> i + 1).limit(10).forEach(System.out::println);
// 使用新的 iterate() 重载方法输出数字 1~10
Stream.iterate(1, i -> i <= 10, i -> i + 1).forEach(System.out::println);
```
<a name="v7NOz"></a>
#### 「`ofNullable`」
`Stream<T> ofNullable(T t)`  返回包含单个元素的顺序`Stream `，如果非空，否则返回空`Stream `。<br />如果指定元素为非null，则获取一个元素并生成单个元素流，元素为null则返回一个空Stream。
```java
//语法
static <T> Stream<T> ofNullable(T t)
//代码示例
Stream<Integer> s1= Stream.ofNullable(100);
s1.forEach(System.out::println)
Stream<Integer> s2 = Stream.ofNullable(null)；
s2.forEach(System.out::println)
//输出
100
```
<a name="Mcg57"></a>
### 10、String 存储结构变更
JDK 8 及之前的版本，String 一直是用 `char[]` 存储。在 Java 9 之后，String 的实现改用 `byte[]` 数组存储字符串。
<a name="Wo94t"></a>
### 11、进程 API
Java 9 增加了 `ProcessHandle` 接口，可以对原生进程进行管理，尤其适合于管理长时间运行的进程。
```java
System.out.println(ProcessHandle.current().pid());
System.out.println(ProcessHandle.current().info());
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1630919691090-fdfe1214-8f86-4a56-b2ff-c3ef7e843c4a.webp#clientId=u3ee6f248-ac9d-4&from=paste&id=ubd542ce4&originHeight=478&originWidth=428&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf5292915-ae7b-4f00-bf1c-892047c42c5&title=)
<a name="Zh1Yp"></a>
### 12、平台日志 API 和服务
Java 9 允许为 JDK 和应用配置同样的日志实现。新增了 `System.LoggerFinder` 用来管理 JDK 使 用的日志记录器实现。JVM 在运行时只有一个系统范围的 `LoggerFinder` 实例。<br />可以通过添加自己的 `System.LoggerFinder` 实现来让 JDK 和应用使用 SLF4J 等其他日志记录框架。
<a name="xa3b6"></a>
### 13、反应式流 （ Reactive Streams ）
在 Java9 中的 `java.util.concurrent.Flow` 类中新增了反应式流规范的核心接口 。<br />`Flow` 中包含了 `Flow.Publisher`、`Flow.Subscriber`、`Flow.Subscription` 和 `Flow.Processor` 等 4 个核心接口。Java 9 还提供了`SubmissionPublisher` 作为`Flow.Publisher` 的一个实现。
<a name="PqXsG"></a>
### 14、变量句柄
变量句柄是一个变量或一组变量的引用，包括静态域，非静态域，数组元素和堆外数据结构中的组成部分等<br />变量句柄的含义类似于已有的方法句柄 `MethodHandle` ，由 Java 类 `java.lang.invoke.VarHandle` 来表示，可以使用类 `java.lang.invoke.MethodHandles.Lookup` 中的静态工厂方法来创建 `VarHandle` 对象。<br />`VarHandle` 的出现替代了 `java.util.concurrent.atomic` 和 `sun.misc.Unsafe` 的部分操作。并且提供了一系列标准的内存屏障操作，用于更加细粒度的控制内存排序。在安全性、可用性、性能上都要优于现有的 API。
<a name="yAbt8"></a>
### 15、改进方法句柄（Method Handle）
方法句柄从 Java7 开始引入，Java9 在类java.lang.invoke.MethodHandles 中新增了更多的静态方法来创建不同类型的方法句柄。
<a name="mvFXP"></a>
### 16、引入HttpClient
定义一个新的 HTTP 客户端 API 来实现 HTTP/2 和 WebSocket，并且可以替换旧的`HttpURLConnection`API。Java以前原生的确实难用，所以诞生了Apache `HttpClientComponents` 、`OkHttp`等好用的客户端。
```java
HttpRequest httpRequest = HttpRequest.newBuilder(newURI)
        .header("Content-Type","*/*")
        .GET()
        .build();
HttpClient httpClient = HttpClient.newBuilder()
        .connectTimeout(Duration.of(10, ChronoUnit.SECONDS))
        .version(HttpClient.Version.HTTP_2)
        .build();
```
<a name="gYdJ0"></a>
### 17、Flow
Spring WebFlux响应式Web框架已经4年了，响应流规范（reactive streams）在Java 9 中也初步引入到了JDK中。
<a name="YIw5E"></a>
### 18、其他
> - HTTP 2客户端 (支持 WebSocket和 HTTP2 流以及服务器推送)
> - 标识符添加限制( `String _ ="hello"`不能用)
> - 响应式流 API (支持Java 9中的响应式编程)
> - 类` CompletableFuture` 中增加了几个新的方法（`completeAsync` ，`orTimeout` 等）
> - Nashorn 引擎的增强 ：Nashorn 从 Java8 开始引入的 JavaScript 引擎，Java9 对 Nashorn 做了些增强，实现了一些 ES6 的新特性（Java 11 中已经被弃用）。
> - I/O 流的新特性 ：增加了新的方法来读取和复制 InputStream 中包含的数据
> - 改进应用的安全性能 ：Java 9 新增了 4 个 SHA- 3 哈希算法，SHA3-224、SHA3-256、SHA3-384 和 SHA3-512

