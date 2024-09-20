介绍如何使用 JVM 堆转储的工具 MAT 来分析 OOM 问题。
<a name="Bq0UA"></a>
## 使用 MAT 分析 OOM 问题
对于排查 OOM 问题、分析程序堆内存使用情况，最好的方式就是分析堆转储。<br />堆转储，包含了堆现场全貌和线程栈信息（Java 6 Update 14 开始包含）。<br />Java 8 自带工具，轻松分析定位 JVM 问题！使用 jstat 等工具虽然可以观察堆内存使用情况的变化，但是对程序内到底有多少对象、哪些是大对象还一无所知，也就是说只能看到问题但无法定位问题。而堆转储，就好似得到了病人在某个瞬间的全景核磁影像，可以拿着慢慢分析。<br />Java 的 OutOfMemoryError 是比较严重的问题，需要分析出根因，所以对生产应用一般都会这样设置 JVM 参数，方便发生 OOM 时进行堆转储：
```bash
-XX:+HeapDumpOnOutOfMemoryError -XX:HeapDumpPath=.
```
更推荐使用 Eclipse 的 Memory Analyzer（也叫做 MAT）做堆转储的分析。可以点击这个链接：[https://www.eclipse.org/mat/](https://www.eclipse.org/mat/)，下载 MAT。<br />使用 MAT 分析 OOM 问题，一般可以按照以下思路进行：

1. 通过支配树功能或直方图功能查看消耗内存最大的类型，来分析内存泄露的大概原因；
2. 查看那些消耗内存最大的类型、详细的对象明细列表，以及它们的引用链，来定位内存泄露的具体点；
3. 配合查看对象属性的功能，可以脱离源码看到对象的各种属性的值和依赖关系，理清程序逻辑和参数；
4. 辅助使用查看线程栈来看 OOM 问题是否和过多线程有关，甚至可以在线程栈看到 OOM 最后一刻出现异常的线程。

比如，手头有一个 OOM 后得到的转储文件 **java_pid29569.hprof**，现在要使用 MAT 的直方图、支配树、线程栈、OQL 等功能来分析此次 OOM 的原因。<br />首先，用 MAT 打开后先进入的是概览信息界面，可以看到整个堆是 437.6MB：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694397378471-6b1fefd6-e8e9-4003-9cf3-e2d463700198.png#averageHue=%23fafafa&clientId=u13ae6592-53db-4&from=paste&id=u71bb1a7b&originHeight=680&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3cdb46a0-4aea-4a98-b635-856e9679a5b&title=)<br />那么，这 437.6MB 都是什么对象呢？<br />如图所示，工具栏的第二个按钮可以打开直方图，直方图按照类型进行分组，列出了每个类有多少个实例，以及占用的内存。可以看到，char[]字节数组占用内存最多，对象数量也很多，结合第二位的 String 类型对象数量也很多，大概可以猜出（String 使用 char[]作为实际数据存储）程序可能是被字符串占满了内存，导致 OOM。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694397378498-d5da03e7-f041-47fa-8fd7-2e3ace132e8e.png#averageHue=%23efefef&clientId=u13ae6592-53db-4&from=paste&id=uf1a6232b&originHeight=966&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubf248720-9148-4c8c-ba30-30e01515146&title=)<br />继续分析下，到底是不是这样呢。<br />在 `char[]`上点击右键，选择 List objects->with incoming references，就可以列出所有的 `char[]`实例，以及每个 `char[]`的整个引用关系链：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694397378509-caeaace7-fa63-4c9a-9b06-bd703c4d0819.png#averageHue=%2383ae80&clientId=u13ae6592-53db-4&from=paste&id=u6817bcf2&originHeight=126&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u21557175-a5fe-4221-bef6-82b4b8df26e&title=)<br />随机展开一个 char[]，如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694397378492-59a4939f-e454-4dcc-b38d-c942276d20d3.png#averageHue=%23d6d1c7&clientId=u13ae6592-53db-4&from=paste&id=uda7fc11b&originHeight=301&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u453dfe75-24d2-4652-8481-e3cffcdd463&title=)<br />接下来，按照红色框中的引用链来查看，尝试找到这些大 char[]的来源：

- 在①处看到，这些 char[]几乎都是 10000 个字符、占用 20000 字节左右（char 是 UTF-16，每一个字符占用 2 字节）；
- 在②处看到，char[]被 String 的 value 字段引用，说明 char[]来自字符串；
- 在③处看到，String 被 ArrayList 的 elementData 字段引用，说明这些字符串加入了一个 ArrayList 中；
- 在④处看到，ArrayList 又被 FooService 的 data 字段引用，这个 ArrayList 整个 RetainedHeap 列的值是 431MB。

Retained Heap（深堆）代表对象本身和对象关联的对象占用的内存，Shallow Heap（浅堆）代表对象本身占用的内存。<br />比如，FooService 中的 data 这个 ArrayList 对象本身只有 16 字节，但是其所有关联的对象占用了 431MB 内存。<br />这些就可以说明，肯定有哪里在不断向这个 List 中添加 String 数据，导致了 OOM。<br />左侧的蓝色框可以查看每一个实例的内部属性，图中显示 FooService 有一个 data 属性，类型是 ArrayList。<br />如果希望看到字符串完整内容的话，可以右键选择 Copy->Value，把值复制到剪贴板或保存到文件中：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694397378449-0630209d-da56-4112-8620-bdb04f7f9cf2.png#averageHue=%23e0e1e0&clientId=u13ae6592-53db-4&from=paste&id=ue5cdaf43&originHeight=403&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uab0a4d73-198f-4243-973d-10512e8f6ea&title=)<br />这里，复制出的是 10000 个字符 a（下图红色部分可以看到）。对于真实案例，查看大字符串、大数据的实际内容对于识别数据来源，有很大意义：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694397379644-4864be9c-131a-4218-86a6-647724733fd5.png#averageHue=%23222322&clientId=u13ae6592-53db-4&from=paste&id=u8b066a29&originHeight=698&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u1961f81c-35b0-4b63-8006-6645c93e333&title=)<br />看到这些，已经基本可以还原出真实的代码是怎样的了。<br />其实，之前使用直方图定位 FooService，已经走了些弯路。可以点击工具栏中第三个按钮（下图左上角的红框所示）进入支配树界面（有关支配树的具体概念参考这里）。这个界面会按照对象保留的 Retained Heap 倒序直接列出占用内存最大的对象。<br />可以看到，第一位就是 FooService，整个路径是 `**FooSerice**`**->**`**ArrayList**`**->**`**Object[]**`**->**`**String**`**->**`**char[]**` （蓝色框部分），一共有 **21523** 个字符串（绿色方框部分）：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694397380358-5c5e46a7-3b77-4eba-ad66-4b9c9a025e74.png#averageHue=%23eeeeee&clientId=u13ae6592-53db-4&from=paste&id=ude379152&originHeight=456&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3c9d0791-b17c-4711-b607-d437dd95898&title=)<br />这样，就从内存角度定位到 FooService 是根源了。那么，OOM 的时候，FooService 是在执行什么逻辑呢？<br />为解决这个问题，可以点击工具栏的第五个按钮（下图红色框所示）。打开线程视图，首先看到的就是一个名为 main 的线程（Name 列），展开后果然发现了 FooService：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694397380313-38def557-72f4-4566-a649-7bf29cadec4b.png#averageHue=%23eaeaea&clientId=u13ae6592-53db-4&from=paste&id=u8089f64b&originHeight=475&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub59854f7-2f6f-406e-85fb-b765125c0c3&title=)<br />先执行的方法先入栈，所以线程栈最上面是线程当前执行的方法，逐一往下看能看到整个调用路径。因为我们希望了解 `FooService.oom()` 方法，看看是谁在调用它，它的内部又调用了谁，所以选择以 `FooService.oom()` 方法（蓝色框）为起点来分析这个调用栈。<br />往下看整个绿色框部分，`**oom()**` 方法被 `OOMApplication` 的 `**run**` 方法调用，而这个 run 方法又被 `SpringAppliction.callRunner` 方法调用。看到参数中的 `CommandLineRunner` 应该能想到，`OOMApplication` 其实是实现了 `CommandLineRunner` 接口，所以是 SpringBoot 应用程序启动后执行的。<br />以 FooService 为起点往上看，从紫色框中的 Collectors 和 `IntPipeline`，大概也可以猜出，这些字符串是由 Stream 操作产生的。再往上看，可以发现在 StringBuilder 的 `append` 操作的时候，出现了 OutOfMemoryError 异常（黑色框部分），说明这这个线程抛出了 OOM 异常。<br />可以看到，整个程序是 Spring Boot 应用程序，那么 FooService 是不是 Spring 的 Bean 呢，又是不是单例呢？如果能分析出这点的话，就更能确认是因为反复调用同一个 FooService 的 oom 方法，然后导致其内部的 ArrayList 不断增加数据的。<br />点击工具栏的第四个按钮（如下图红框所示），来到 OQL 界面。在这个界面，可以使用类似 SQL 的语法，在 dump 中搜索数据（可以直接在 MAT 帮助菜单搜索 OQL Syntax，来查看 OQL 的详细语法）。<br />比如，输入如下语句搜索 FooService 的实例：
```sql
SELECT * FROM org.geekbang.time.commonmistakes.troubleshootingtools.oom.FooService
```
可以看到只有一个实例，然后通过 List objects 功能搜索引用 FooService 的对象:<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694397380327-dbcff887-f2cd-4e9e-94ac-8eb2cc8973e2.png#averageHue=%23f3f3f3&clientId=u13ae6592-53db-4&from=paste&id=ue43830eb&originHeight=433&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0998d76d-f56d-482f-a4a7-f8aba01f8b3&title=)<br />得到以下结果：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694397380404-ee13085e-2208-497d-aa2f-aadbe643f910.png#averageHue=%23a0e090&clientId=u13ae6592-53db-4&from=paste&id=u173b8bcb&originHeight=367&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf13468bb-76f1-43e8-b87f-abb6397bc88&title=)<br />可以看到，一共两处引用：

- 第一处是，`OOMApplication` 使用了 FooService，这个已经知道了
- 第二处是一个 `ConcurrentHashMap`。可以看到，这个 HashMap 是 `DefaultListableBeanFactory` 的 `singletonObjects` 字段，可以证实 FooService 是 Spring 容器管理的单例的 Bean。

甚至可以在这个 HashMap 上点击右键，选择 Java Collections->Hash Entries 功能，来查看其内容：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694397380626-fe6b16b0-c93d-4448-a8f3-a592259b6ba6.png#averageHue=%23ebeceb&clientId=u13ae6592-53db-4&from=paste&id=ucb259150&originHeight=384&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uda220b0e-72d5-43dd-8e56-bcf528cf3c0&title=)<br />这样就列出了所有的 Bean，可以在 Value 上的 Regex 进一步过滤。输入 FooService 后可以看到，类型为 FooService 的 Bean 只有一个，其名字是 fooService：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694397380774-e9296820-753c-46b3-97ae-cbe62641fe5b.png#averageHue=%23c5c2b9&clientId=u13ae6592-53db-4&from=paste&id=u1b24957f&originHeight=83&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8db2d579-0d16-4a1a-abaa-f0f2a5f29db&title=)<br />到现在为止，虽然没看程序代码，但是已经大概知道程序出现 OOM 的原因和大概的调用栈了。再贴出程序来对比一下，果然和看到得一模一样：
```java
@SpringBootApplication
public class OOMApplication implements CommandLineRunner {
    @Autowired
    FooService fooService;
    public static void main(String[] args) {
        SpringApplication.run(OOMApplication.class, args);
    }
    @Override
    public void run(String... args) throws Exception {
        //程序启动后，不断调用Fooservice.oom()方法
        while (true) {
            fooService.oom();
        }
    }
}
@Component
public class FooService {
    List<String> data = new ArrayList<>();
    public void oom() {
        //往同一个ArrayList中不断加入大小为10KB的字符串
        data.add(IntStream.rangeClosed(1, 10_000)
                 .mapToObj(__ -> "a")
                 .collect(Collectors.joining("")));
    }
}
```
到这里，使用 MAT 工具从对象清单、大对象、线程栈等视角，分析了一个 OOM 程序的堆转储。可以发现，有了堆转储，几乎相当于拿到了应用程序的源码 + 当时那一刻的快照，OOM 的问题无从遁形。
<a name="lfOG7"></a>
## 使用OQL语言查询对象信息
MAT 支持一种类似于 SQL 的查询语言 OQL（Object Query Language）。OQL 使用类 SQL 语法，可以在堆中进行对象的查找和筛选。
<a name="b5sVa"></a>
### 1、SELECT 子句
在 MAT 中，Select 子句的格式与 SQL 基本一致，用于指定要显示的列。Select 子句中可以使用“＊”，查看结果对象的引用实例（相当于 outgoing references）。
```sql
SELECT * FROM java.util.Vector v
```
使用“OBJECTS”关键字，可以将返回结果集中的项以对象的形式显示。
```sql
SELECT objects v.elementData FROM java.util.Vector v

SELECT OBJECTS s.value FROM java.lang.String s
```
在 Select 子句中，使用“AS RETAINED SET”关键字可以得到所得对象的保留集。
```sql
SELECT AS RETAINED SET *FROM com.example.mat.Student
```
“DISTINCT”关键字用于在结果集中去除重复对象。
```sql
SELECT DISTINCT OBJECTS classof(s) FROM java.lang.String s
```
<a name="Qo8g9"></a>
### 2、FROM 子句
From 子句用于指定查询范围，它可以指定类名、正则表达式或者对象地址。
```sql
SELECT * FROM java.lang.String s
```
使用正则表达式，限定搜索范围，输出所有 com.example 包下所有类的实例
```sql
SELECT * FROM "com\.example\..*"
```
使用类的地址进行搜索。使用类的地址的好处是可以区分被不同 ClassLoader 加载的同一种类型。
```sql
select * from 0x37a0b4d
```
<a name="vzT0R"></a>
### 3、WHERE 子句
Where 子句用于指定 OQL 的查询条件。OQL 查询将只返回满足 Where 子句指定条件的对象。Where 子句的格式与传统 SQL 极为相似。<br />返回长度大于 10 的 char 数组。
```sql
SELECT *FROM Ichar[] s WHERE s.@length>10
```
返回包含“java”子字符串的所有字符串，使用“LIKE”操作符，“LIKE”操作符的操作参数为正则表达式。
```sql
SELECT * FROM java.lang.String s WHERE toString(s) LIKE ".*java.*"
```
返回所有 value 域不为 null 的字符串，使用“＝”操作符。
```sql
SELECT * FROM java.lang.String s where s.value!=null
```
返回数组长度大于 15，并且深堆大于 1000 字节的所有 Vector 对象。
```sql
SELECT * FROM java.util.Vector v WHERE v.elementData.@length>15 AND v.@retainedHeapSize>1000
```
<a name="NwsnJ"></a>
### 4、内置对象与方法
OQL 中可以访问堆内对象的属性，也可以访问堆内代理对象的属性。访问堆内对象的属性时，格式如下，其中 alias 为对象名称：<br />[ <alias>. ] <field> . <field>. <field><br />访问 java.io.File 对象的 path 属性，并进一步访问 path 的 value 属性：
```sql
SELECT toString(f.path.value) FROM java.io.File f
```
显示 String 对象的内容、objectid 和 objectAddress。
```sql
SELECT s.toString(),s.@objectId, s.@objectAddress FROM java.lang.String s
```
显示 java.util.Vector 内部数组的长度。
```sql
SELECT v.elementData.@length FROM java.util.Vector v
```
显示所有的 java.util.Vector 对象及其子类型
```sql
select * from INSTANCEOF java.util.Vector
```
