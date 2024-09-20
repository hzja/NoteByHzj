Java
<a name="ZkRIX"></a>
# Java12新特性预览
jdk1.12 正式版将在2019年3月份发布, jdk1.12变化内容：

1. 低暂停时间的 GC 
2. 微基准测试套件 
3. Switch 表达式 
4. JVM 常量 API 
5. 只保留一个 AArch64 实现 
6. 默认类数据共享归档文件 
7. 可中止的 G1 Mixed GC 
8. G1 及时返回未使用的已分配内存

---

- JVM 更新
- `File.mismatch()` 方法
- 紧凑型数字格式
- String 类新增了一些方法，如 `indent()`

![](https://cdn.nlark.com/yuque/0/2020/png/396745/1602515709938-d058ec07-ab50-4b26-b25c-c1926ea7bf01.png#averageHue=%23f7f6f5&height=305&id=FTcKF&originHeight=305&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=1080)
<a name="z4jfZ"></a>
### 1、Switch 表达式扩展（预览功能）
Java 11 以及之前 Java 版本中的 Switch 语句是按照类似 C、C++ 这样的语言来设计的，在默认情况下支持 fall-through 语法。虽然这种传统的控制流通常用于编写低级代码，但 Switch 控制语句通常运用在高级别语言环境下的，因此其容易出错性掩盖其灵活性。<br />在 Java 12 中重新拓展了 Switch 让它具备了新的能力，通过扩展现有的 Switch 语句，可将其作为增强版的 Switch 语句或称为 "Switch 表达式"来写出更加简化的代码。<br />Switch 表达式也是作为预览语言功能的第一个语言改动被引入新版 Java 中来的，预览语言功能的想法是在 2018 年初被引入 Java 中的，本质上讲，这是一种引入新特性的测试版的方法。通过这种方式，能够根据用户反馈进行升级、更改，在极端情况下，如果没有被很好的接纳，则可以完全删除该功能。预览功能的关键在于它们没有被包含在 Java SE 规范中。<br />传统的`switch`语句，容易漏写`break`而出错，同时写法并不简洁优雅。<br />Java 12之前
```java
switch (day) {
    case MONDAY:
    case FRIDAY:
    case SUNDAY:
        System.out.println(6);
        break;
    case TUESDAY:
        System.out.println(7);
        break;
    case THURSDAY:
    case SATURDAY:
        System.out.println(8);
        break;
    case WEDNESDAY:
        System.out.println(9);
        break;
}
```
上面代码中多处出现 break 语句，显得代码比较冗余，同时如果某处漏写一段 break 语句，将导致程序一直向下穿透执行的逻辑错误，出现异常结果，同时这种写法比较繁琐，也容易出问题。<br />JDk 12 之后，Switch表达式得到增强，能接受语句和表达式。使用类似 lambda 语法条件匹配成功后的执行块，不需要多写 break 。
```java
switch (day) {
    case MONDAY, FRIDAY, SUNDAY -> System.out.println(6);
    case TUESDAY                -> System.out.println(7);
    case THURSDAY, SATURDAY     -> System.out.println(8);
    case WEDNESDAY              -> System.out.println(9);
}
```
使用 Java 12 中 Switch 表达式的写法，省去了 break 语句，避免了因少些 break 而出错，同时将多个 case 合并到一行，显得简洁、清晰也更加优雅的表达逻辑分支，其具体写法就是将之前的 case 语句表成了：case L ->，即如果条件匹配 case L，则执行 标签右侧的代码 ，同时标签右侧的代码段只能是表达式、代码块或 throw 语句。为了保持兼容性，case 条件语句中依然可以使用字符 : ，这时 fall-through 规则依然有效的，即不能省略原有的 break 语句，但是同一个 Switch 结构里不能混用 -> 和 : ，否则会有编译错误。并且简化后的 Switch 代码块中定义的局部变量，其作用域就限制在代码块中，而不是蔓延到整个 Switch 结构，也不用根据不同的判断条件来给变量赋值。<br />Java 11 以及之前版本中，Switch 表达式支持下面类型：byte、char、short、int、Byte、Character、Short、Integer、enum、tring，在未来的某个 Java 版本有可能会允许支持 float、double 和 long （以及上面类型的封装类型）。
<a name="blYe9"></a>
### 2、紧凑的数据格式
JDK 12 新增了NumberFormat对复杂数字的格式化，引入了新的基于区域的紧凑型数字化格式类`CompactNumberFormat`，用来对长数字进行简写。通常程序员喜欢把工资区间标为10k-20k，而其它一些行业喜欢10000-20000。
```java
NumberFormat numberFormat = NumberFormat.getCompactNumberInstance(Locale.CHINA, NumberFormat.Style.SHORT);
System.out.println(numberFormat.format(100000));
//output
10万

NumberFormat chnFormat = NumberFormat.getCompactNumberInstance(Locale.CHINA,
                                                               NumberFormat.Style.SHORT);
chnFormat.setMaximumFractionDigits(3);
// 8.232万
String cformat = chnFormat.format(82323);

NumberFormat usFormat = NumberFormat.getCompactNumberInstance(Locale.US,
                                                              NumberFormat.Style.SHORT);
usFormat.setMaximumFractionDigits(3);
// 82.323K
String uformat = usFormat.format(82323);
```
:::tips
还可以自定义`CompactNumberFormat`来实现个性化的数字格式化。
:::
<a name="DWpJT"></a>
### 3、字符串支持`transform`、`indent`操作
<a name="OKjZx"></a>
#### `transform` 字符串转换，可以配合函数式接口`Function`一起使用
`transform()` 方法可以用来转变指定字符串。
```java
String result = "foo".transform(input -> input + " bar");
System.out.println(result); 
// foo bar
```
```java
List<String> list1 = List.of("Fcant", "World");
List<String> list2 = new ArrayList<>();
list1.forEach(element ->
            list2.add(element.transform(String::strip)
                    .transform((e) -> "Hello," + e))
    );
list2.forEach(System.out::println);
//输出
Hello,Fcant
Hello,World
```
<a name="kDgan"></a>
#### `indent` 缩进，每行开头增加空格`space`和移除空格，可以实现字符串缩进。
```java
String text = "Java";
// 缩进 4 格
text = text.indent(4);
System.out.println(text);
text = text.indent(-10);
System.out.println(text);
```
输出：
```java
     Java
Java
```
```java
String result = "Java\n Python\nC".indent(3);
System.out.println(result);
//输出
Java
Python
C
```
<a name="daad27f3"></a>
### 4、文件比较：`Files.mismatch(Path, Path)`
Java 12 新增了`mismatch`方法，此方法返回第一个不匹配的位置，如果没有不匹配，则返回 -1L。
```java
public static long mismatch(Path path, Path path2) throws IOException;
```
代码示例：
```java
Path file1 = Paths.get("c:\\fcant.txt");
Path file2 = Paths.get("c：\\World.txt");
try {
    long fileMismatch = Files.mismatch(file1, file2);
    System.out.println(fileMismatch);
} catch (IOException e) {
    e.printStackTrace();
}
```
代码示例（两个文件内容相同的情况）：
```java
Path filePath1 = Files.createTempFile("file1", ".txt");
Path filePath2 = Files.createTempFile("file2", ".txt");
Files.writeString(filePath1, "Java 12 Article");
Files.writeString(filePath2, "Java 12 Article");

long mismatch = Files.mismatch(filePath1, filePath2);
assertEquals(-1, mismatch);
```
代码示例（两个文件内容不相同的情况）：
```java
Path filePath3 = Files.createTempFile("file3", ".txt");
Path filePath4 = Files.createTempFile("file4", ".txt");
Files.writeString(filePath3, "Java 12 Article");
Files.writeString(filePath4, "Java 12 Tutorial");

long mismatch = Files.mismatch(filePath3, filePath4);
assertEquals(8, mismatch);
```
该方法和另外一个方法`Files.isSameFile(Path, Path)`的效果有点类似，但是还是有区别的。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635576357428-17d8286b-1c84-4bf4-81ac-c017363ba8da.webp#averageHue=%23f7f8f2&clientId=u39ae4f92-0130-4&from=paste&id=u022eac42&originHeight=258&originWidth=836&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2f063c29-bdf2-4ef2-a84b-d9a128b258a&title=)
<a name="db0St"></a>
### 5、Teeing Collector
Teeing Collector 是 Streams API 中引入的新的收集器实用程序，它的作用是 merge 两个 collector 的结果，API格式如下：
```java
public static <T, R1, R2, R>
    Collector<T, ?, R> teeing(Collector<? super T, ?, R1> downstream1,
    Collector<? super T, ?, R2> downstream2,
    BiFunction<? super R1, ? super R2, R> merger)
```
直接看代码，如下为求学生的平均分和总分的例子
```java
List<Student> studentList= Arrays.asList(
    new Student("fcant", 90),
    new Student("world", 100),
    new Student("world", 80)
);
String teeingResult=studentList.stream().collect(
    Collectors.teeing(
        Collectors.averagingInt(Student::getScore),
        Collectors.summingInt(Student::getScore),
        (s1,s2)-> s1+ ":"+ s2
                )
        );
System.out.println(teeingResult); //90:270
```
<a name="v8AGU"></a>
### 6、`instanceof`+ 类型强转一步到位
`instanceof` 主要在类型强转前探测对象的具体类型。<br />之前处理动态类型碰上要强转时，需要先 `instanceof` 判断一下，然后再强转为该类型处理：
```java
Object obj = "Hello Java 12!";
if (obj instanceof String) {
    String s = (String) obj;
    int length = s.length();
}
```
现在 `instanceof` 支持直接类型转换了，不需要再来一次额外的强转：
```java
Object obj = "Hello Java 12!";
if (obj instanceof String str) {
    int length = str.length();
}
```
<a name="ayVY1"></a>
### 7、Shenandoah GC：一个低停顿垃圾收集器（实验阶段）
Redhat 主导开发的 Pauseless GC 实现，主要目标是 99.9% 的暂停小于 10ms，暂停与堆大小无关等<br />和 Java11 开源的 ZGC 相比（需要升级到 JDK11 才能使用），Shenandoah GC 有稳定的 JDK8u 版本，在 Java8 占据主要市场份额的今天有更大的可落地性。<br />Java 12 中引入一个新的垃圾收集器：Shenandoah，它是作为一中低停顿时间的垃圾收集器而引入到 Java 12 中的，其工作原理是通过与 Java 应用程序中的执行线程同时运行，用以执行其垃圾收集、内存回收任务，通过这种运行方式，给虚拟机带来短暂的停顿时间。<br />Shenandoah 垃圾回收器是 Red Hat 在 2014 年宣布进行的一项垃圾收集器研究项目，旨在针对 JVM 上的内存收回实现低停顿的需求。该设计将与应用程序线程并发，通过交换 CPU 并发周期和空间以改善停顿时间，使得垃圾回收器执行线程能够在 Java 线程运行时进行堆压缩，并且标记和整理能够同时进行，因此避免了在大多数 JVM 垃圾收集器中所遇到的问题。<br />据 Red Hat 研发 Shenandoah 团队对外宣称，Shenandoah 垃圾回收器的暂停时间与堆大小无关，这意味着无论将堆设置为 200 MB 还是 200 GB，都将拥有一致的系统暂停时间，不过实际使用性能将取决于实际工作堆的大小和工作负载。<br />Shenandoah GC 工作周期如下所示：<br />![2021-09-29-19-31-37-026313.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632915188354-5e8617b8-1b13-4883-a5de-217196d8f7d8.png#averageHue=%23a8d0ad&clientId=ube59fcdc-02e6-4&from=ui&id=uad3ab74f&originHeight=198&originWidth=766&originalType=binary&ratio=1&rotation=0&showTitle=false&size=107712&status=done&style=shadow&taskId=u5c296238-014a-424b-88f5-681b7593287&title=)<br />上图对应工作周期如下：

1. Init Mark 启动并发标记 阶段
2. 并发标记遍历堆阶段
3. 并发标记完成阶段
4. 并发整理回收无活动区域阶段
5. 并发 Evacuation 整理内存区域阶段
6. Init Update Refs 更新引用初始化 阶段
7. 并发更新引用阶段
8. Final Update Refs 完成引用更新阶段
9. 并发回收无引用区域阶段

需要了解不是唯有 GC 停顿可能导致常规应用程序响应时间比较长。具有较长的 GC 停顿时间会导致系统响应慢的问题，但响应时间慢并非一定是 GC 停顿时间长导致的，队列延迟、网络延迟、其他依赖服务延迟和操作提供调度程序抖动等都可能导致响应变慢。使用 Shenandoah 时需要全面了解系统运行情况，综合分析系统响应时间。各种 GC 工作负载对比如下所示：<br />各种 GC 工作负载对比：<br />![2021-09-29-19-31-37-217301.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632915188395-e51cd57f-2e95-4c5f-871c-d82f4487f2b7.png#averageHue=%23deebde&clientId=ube59fcdc-02e6-4&from=ui&id=D8DJl&originHeight=334&originWidth=800&originalType=binary&ratio=1&rotation=0&showTitle=false&size=94532&status=done&style=shadow&taskId=uf14c019c-2702-410e-a71a-833921648c6&title=)<br />下面推荐几个配置或调试 Shenandoah 的 JVM 参数:

- `-XX:+AlwaysPreTouch`：使用所有可用的内存分页，减少系统运行停顿，为避免运行时性能损失。
- `-Xmx == -Xmsv`：设置初始堆大小与最大值一致，可以减轻伸缩堆大小带来的压力，与 AlwaysPreTouch 参数配合使用，在启动时提交所有内存，避免在最终使用中出现系统停顿。
- `-XX:+ UseTransparentHugePages`：能够大大提高大堆的性能，同时建议在 Linux 上使用时将 /sys/kernel/mm/transparent_hugepage/enabled 和 /sys/kernel/mm/transparent_hugepage/defragv 设置为：madvise，同时与 AlwaysPreTouch 一起使用时，init 和 shutdownv 速度会更快，因为它将使用更大的页面进行预处理。
- `-XX:+UseNUMA`：虽然 Shenandoah 尚未明确支持 NUMA（Non-Uniform Memory Access），但最好启用此功能以在多插槽主机上启用 NUMA 交错。与 AlwaysPreTouch 相结合，它提供了比默认配置更好的性能。
- `-XX:+DisableExplicitGC`：忽略代码中的 `System.gc()` 调用。当用户在代码中调用 `System.gc()` 时会强制 Shenandoah 执行 STW Full GC ，应禁用它以防止执行此操作，另外还可以使用 -`XX:+ExplicitGCInvokesConcurrent`，在 调用 `System.gc()` 时执行 CMS GC 而不是 Full GC，建议在有 `System.gc()` 调用的情况下使用。
- 不过目前 Shenandoah 垃圾回收器还被标记为实验项目，需要使用参数：`- XX:+UnlockExperimentalVMOptions` 启用。更多有关如何配置、调试 Shenandoah 的信息，请参阅 henandoah wiki。
<a name="tXrDo"></a>
### 8、G1 收集器提升
Java12 为默认的垃圾收集器 G1 带来了两项更新:

- 可中止的混合收集集合 ：JEP344 的实现，为了达到用户提供的停顿时间目标，JEP 344 通过把要被回收的区域集（混合收集集合）拆分为强制和可选部分，使 G1 垃圾回收器能中止垃圾回收过程。G1 可以中止可选部分的回收以达到停顿时间目标
- 及时返回未使用的已分配内存 ：JEP346 的实现，增强 G1 GC，以便在空闲时自动将 Java 堆内存返回给操作系统
<a name="hfpH3"></a>
#### 改善 G1 垃圾收集器，使其能够中止混合集合
G1 是垃圾收集器，设计用于具有大量内存的多处理器机器，提高了垃圾回收效率。该垃圾收集器 设计的主要目标之一是满足用户设置的预期的 JVM 停顿时间，G1 采用一个高级分析引擎来选择在收集期间要处理的工作量，此选择过程的结果是一组称为 GC 回收集的区域。一旦收集器确定了 GC 回收集 并且 GC 回收、整理工作已经开始，则 G1 收集器必须完成收集集合集的所有区域中的所有活动对象之后才能停止；但是如果收集器选择过大的 GC 回收集，可能会导致 G1 回收器停顿时间超过预期时间。<br />Java 12 中将把 GC 回收集（混合收集集合）拆分为必需和可选两部分，使 G1 垃圾回收器能中止垃圾回收过程。其中必需处理的部分包括 G1 垃圾收集器不能递增处理的 GC 回收集的部分（如：年轻代），同时也可以包含老年代以提高处理效率。将 GC 回收集拆分为必需和可选部分时，需要为可选 GC 回收集部分维护一些其他数据，这会产生轻微的 CPU 开销，但小于 1 ％的变化，同时在 G1 回收器处理 GC 回收集期间，本机内存使用率也可能会增加，使用上述情况只适用于包含可选 GC 回收部分的 GC 混合回收集合。<br />在 G1 垃圾回收器完成收集需要必需回收的部分之后，便开始收集可选的部分，如果还有时间的话，但是粗粒度的处理，可选部分的处理粒度取决于剩余的时间，一次只能处理可选部分的一个子集区域。在完成可选收集部分的收集后，G1 垃圾回收器可以根据剩余时间决定是否停止收集。如果在处理完 必需处理的 部分后，属于时间不足，总时间花销接近预期时间，G1 垃圾回收器也可以中止可选部分的回收以达到满足预期停顿时间的目标。
<a name="TLwl6"></a>
#### 增强 G1 垃圾收集器，使其能自动返回未用堆内存给操作系统
上节中介绍了 Java 12 中增强了 G1 垃圾收集器关于混合收集集合的处理策略，这节主要介绍在 Java 12 中同时也对 G1 垃圾回收器进行了改进，使其能够在空闲时自动将 Java 堆内存返还给操作系统，这也是 Java 12 中的另外一项重大改进。<br />目前 Java 11 版本中包含的 G1 垃圾收集器 暂时无法及时将已提交的 Java 堆内存返回给操作系统， G1 垃圾收集器仅在进行完整 GC (Full GC) 或并发处理周期时才能将 Java 堆返回内存。由于 G1 回收器尽可能避免完整 GC，并且只触发基于 Java 堆占用和分配活动的并发周期，因此在许多情况下 G 1 垃圾回收器不能回收 Java 堆内存，除非有外部强制执行。<br />在使用云平台的容器环境中，这种不利之处特别明显。即使在虚拟机不活动，但如果仍然使用其分配的内存资源，哪怕是其中的一小部分，G1 回收器也仍将保留所有已分配的 Java 堆内存。而这将导致用户需要始终为所有资源付费，哪怕是实际并未用到，而云提供商也无法充分利用其硬件。如果在次期间虚拟机能够检测到 Java 堆内存的实际使用情况，并在利用空闲时间自动将 Java 堆内存返还，则两者都将受益。<br />为了尽可能的向操作系统返回空闲内存，G1 垃圾收集器将在应用程序不活动期间定期生成或持续循环检查整体 Java 堆使用情况，以便 G 1 垃圾收集器能够更及时的将 Java 堆中不使用内存部分返还给操作系统。对于长时间处于空闲状态的应用程序，此项改进将使 JVM 的内存利用率更加高效。<br />如果应用程序为非活动状态，在下面两种情况下，G1 回收器会触发定期垃圾收集：<br />自上次垃圾回收完成 以来已超过 `G1PeriodicGCInterval` 毫秒， 并且此时没有正在进行的垃圾回收任务。如果 `G1PeriodicGCInterval` 值为零表示禁用快速回收内存的定期垃圾收集。应用所在主机系统上执行方法 `getloadavg()`，一分钟内系统返回的平均负载值低于 `G1PeriodicGCSystemLoadThreshold`。如果 `G1PeriodicGCSystemLoadThreshold` 值为零，则此条件不生效。如果不满足上述条件中的任何一个，则取消当期的定期垃圾回收。等一个 `G1PeriodicGCInterval` 时间周期后，将重新考虑是否执行定期垃圾回收。<br />G1 定期垃圾收集的类型根据 `G1PeriodicGCInvokesConcurrent` 参数的值确定：如果设置值了，G1 垃圾回收器将继续上一个或者启动一个新并发周期；如果没有设置值，则 G1 回收器将执行一个完整的 GC。在每次一次 GC 回收末尾，G1 回收器将调整当前的 Java 堆大小，此时便有可能会将未使用内存返还给操作系统。新的 Java 堆内存大小根据现有配置确定，具体包括下列配置：`- XX:MinHeapFreeRatio`、`-XX:MaxHeapFreeRatio`、`-Xms`、`-Xmx`。<br />默认情况下，G1 回收器在定期垃圾回收期间新启动或继续上一轮并发周期，将最大限度地减少应用程序的中断。如果定期垃圾收集严
<a name="n0Yx8"></a>
### 9、预览新特性
作为预览特性加入，需要在`javac`编译和`java`运行时增加参数`--enable-preview` 。
<a name="xtYDs"></a>
### 10、增加一套微基准套件
Java 12 中添加一套新的基本的微基准测试套件，该套微基准测试套件基于 JMH（Java Microbenchmark Harness），使开发人员可以轻松运行现有的微基准测试并创建新的基准测试，其目标在于提供一个稳定且优化过的基准，其中包括将近 100 个基准测试的初始集合，并且能够轻松添加新基准、更新基准测试和提高查找已有基准测试的便利性。<br />微基准套件与 JDK 源代码位于同一个目录中，并且在构建后将生成单个 Jar 文件。但它是一个单独的项目，在支持构建期间不会执行，以方便开发人员和其他对构建微基准套件不感兴趣的人在构建时花费比较少的构建时间。<br />要构建微基准套件，用户需要运行命令：`make build-microbenchmark`，类似的命令还有：`make test TEST="micro:java.lang.invoke"` 将使用默认设置运行 java.lang.invoke 相关的微基准测试。关于配置本地环境可以参照文档 docs/testing.md|html。
<a name="mfQC6"></a>
### 11、引入 JVM 常量 API
Java 12 中引入 JVM 常量 API，用来更容易地对关键类文件 (key class-file) 和运行时构件（artefact）的名义描述 (nominal description) 进行建模，特别是对那些从常量池加载的常量，这是一项非常技术性的变化，能够以更简单、标准的方式处理可加载常量。<br />此项改进主要在新的 java.lang.invoke.constant 包中定义了一系列基于值的符号引用类型，能够描述每种可加载常量。符号引用以纯粹 nominal 的形式描述可加载常量，与类加载或可访问 性上下文分开。同时有些类可以作为自己的符号引用（例如 String），而对于可链接常量，另外定义了一系列符号引用类型，具体包括：ClassDesc (Class 的可加载常量标称描述符) ，MethodTypeDesc(方法类型常量标称描述符) ，MethodHandleDesc (方法句柄常量标称描述符) 和 DynamicConstantDesc (动态常量标称描述符) ，它们包含描述这些常量的 nominal 信息。
<a name="tej0r"></a>
### 12、改进 AArch64 实现
Java 12 中将只保留一套 AArch64 实现，删除所有与 arm64 实现相关的代码，只保留 32 位 ARM 端口和 64 位 aarch64 的端口。删除此套实现将允许所有开发人员将目标集中在剩下的这个 64 位 ARM 实现上，消除维护两套端口所需的重复工作。<br />当前 Java 11 中存在两套 64 位 AArch64 端口，它们主要存在于 src/hotspot/cpu/arm 和 open/src/hotspot/cpu/aarch64 目录中。这两套代码中都实现了 AArch64，Java 12 中将删除目录 open/src/hotspot/cpu/arm 中关于 64-bit 的这套实现，只保留其中有关 32-bit 的实现，余下目录的 open/src/hotspot/cpu/aarch64 代码部分就成了 AArch64 的默认实现。
<a name="ik9N8"></a>
### 13、使用默认类数据共享（CDS）存档
类数据共享机制 (Class Data Sharing ，简称 CDS) ，允许将一组类预处理为共享归档文件，以便在运行时能够进行内存映射以减少 Java 程序的启动时间，当多个 Java 虚拟机（JVM）共享相同的归档文件时，还可以减少动态内存的占用量，同时减少多个虚拟机在同一个物理或虚拟的机器上运行时的资源占用。<br />自 Java 8 以来，在基本 CDS 功能上进行了许多增强、改进，启用 CDS 后应用的启动时间和内存占用量显着减少。使用 Java 11 早期版本在 64 位 Linux 平台上运行 HelloWorld 进行测试，测试结果显示启动时间缩短有 32 ％，同时在其他 64 位平台上，也有类似或更高的启动性能提升。<br />Java 12 针对 64 位平台下的 JDK 构建过程进行了增强改进，使其默认生成类数据共享（CDS）归档，以进一步达到改进应用程序的启动时间的目的，同时也避免了需要手动运行：-Xshare:dump 的需要，修改后的 JDK 将在 lib/server 目录中保留构建时生成的 CDS 存档。<br />当然如果需要，也可以添加其他 GC 参数，来调整堆大小等，以获得更优的内存分布情况，同时用户也可以像之前一样创建自定义的 CDS 存档文件。
<a name="6931227f"></a>
### 14、其他特性

- 支持unicode 11（684个新字符、11个新blocks、7个新脚本）
- JVM 常量 API （主要在新的java.lang.invoke.constant包中定义了一系列基于值的符号引用类型，能够描述每种可加载常量。）
- Shenandoah GC（低暂停时间垃圾收集器）
- G1 收集器提升 （可中止的混合收集集合、及时返回未使用的已分配内存）
- 默认CDS档案
- JMH 基准测试
