Java Stream<br />Java 8 API添加了一个新的抽象称为流Stream，可以一种声明的方式处理数据。<br />Stream 使用一种类似用 SQL 语句从数据库查询数据的直观方式来提供一种对 Java 集合运算和表达的高阶抽象。<br />Stream API可以极大提高Java程序员的生产力，让程序员写出高效率、干净、简洁的代码。<br />本文会对Stream的实现原理进行剖析。
<a name="lQrhc"></a>
## Stream的组成与特点
Stream（流）是一个来自数据源的元素队列并支持聚合操作：

- 元素是特定类型的对象，形成一个队列。Java中的Stream并_不会_向集合那样存储和管理元素，而是按需计算
- 数据源流的来源可以是集合Collection、数组Array、I/O channel， 产生器generator 等
- 聚合操作类似SQL语句一样的操作， 比如filter, map, reduce, find, match, sorted等

和以前的Collection操作不同， Stream操作还有两个基础的特征：

- Pipelining: 中间操作都会返回流对象本身。这样多个操作可以串联成一个管道， 如同流式风格（fluent style）。这样做可以对操作进行优化， 比如延迟执行(laziness evaluation)和短路( short-circuiting)
- 内部迭代：以前对集合遍历都是通过Iterator或者For-Each的方式，显式的在集合外部进行迭代，这叫做外部迭代。Stream提供了内部迭代的方式， 通过访问者模式 (Visitor)实现。

和迭代器又不同的是，Stream 可以并行化操作，迭代器只能命令式地、串行化操作。顾名思义，当使用串行方式去遍历时，每个 item 读完后再读下一个 item。而使用并行去遍历时，数据会被分成多个段，其中每一个都在不同的线程中处理，然后将结果一起输出。<br />Stream 的并行操作依赖于 Java7 中引入的 Fork/Join 框架（JSR166y）来拆分任务和加速处理过程。Java 的并行 API 演变历程基本如下：<br />1.0-1.4 中的 java.lang.Thread<br />5.0 中的 java.util.concurrent<br />6.0 中的 Phasers 等<br />7.0 中的 Fork/Join 框架<br />8.0 中的 Lambda<br />Stream具有平行处理能力，处理的过程会分而治之，也就是将一个大任务切分成多个小任务，这表示每个任务都是一个操作：
```java
List<Integer> numbers = Arrays.asList(1, 2, 3, 4, 5, 6, 7, 8, 9);
numbers.parallelStream()
       .forEach(out::println);
```
可以看到一行简单的代码就实现了并行输出集合中元素的功能，但是由于并行执行的顺序是不可控的所以每次执行的结果不一定相同。<br />如果非得相同可以使用`forEachOrdered`方法执行终止操作：
```java
List<Integer> numbers = Arrays.asList(1, 2, 3, 4, 5, 6, 7, 8, 9);
numbers.parallelStream()
       .forEachOrdered(out::println);
```
这里有一个疑问，如果结果需要有序，是否和并行执行的初衷相悖？是的，这个场景下明显无需使用并行流，直接用串行流执行即可， 否则性能可能更差，因为最后又强行将所有并行结果进行了排序。<br />先介绍一下Stream接口的相关知识。
<a name="FXBaM"></a>
## BaseStream接口
Stream的父接口是`BaseStream`，后者是所有流实现的顶层接口，定义如下：
```java
public interface BaseStream<T, S extends BaseStream<T, S>>
        extends AutoCloseable {
    Iterator<T> iterator();

    Spliterator<T> spliterator();

    boolean isParallel();

    S sequential();

    S parallel();

    S unordered();

    S onClose(Runnable closeHandler);

    void close();
}
```
其中，`T`为流中元素的类型，S为一个`BaseStream`的实现类，它里面的元素也是T并且S同样是自己：
```java
S extends BaseStream<T, S>
```
是不是有点晕？<br />其实很好理解，看一下接口中对S的使用就知道了：如`sequential()`、`parallel()`这两个方法，它们都返回了S实例，也就是说它们分别支持对当前流进行串行或者并行的操作，并返回「改变」后的流对象。<br />如果是并行一定涉及到对当前流的拆分，即将一个流拆分成多个子流，子流肯定和父流的类型是一致的。子流可以继续拆分子流，一直拆分下去…<br />也就是说这里的S是`BaseStream`的一个实现类，它同样是一个流，比如`Stream`、`IntStream`、`LongStream`等。
<a name="UjzdW"></a>
## Stream接口
再来看一下Stream的接口声明：
```java
public interface Stream<T> extends BaseStream<T, Stream<T>>
```
参考上面的解释这里不难理解：即`Stream<T>`可以继续拆分为`Stream<T>`，可以通过它的一些方法来证实：
```java
Stream<T> filter(Predicate<? super T> predicate);
<R> Stream<R> map(Function<? super T, ? extends R> mapper);
<R> Stream<R> flatMap(Function<? super T, ? extends Stream<? extends R>> mapper);
Stream<T> sorted();
Stream<T> peek(Consumer<? super T> action);
Stream<T> limit(long maxSize);
Stream<T> skip(long n);
...
```
这些都是操作流的中间操作，它们的返回结果必须是流对象本身。
<a name="DEZjX"></a>
## 关闭流操作
`BaseStream` 实现了 `AutoCloseable` 接口，也就是 `close()` 方法会在流关闭时被调用。同时，`BaseStream` 中还提供了`onClose()`方法：
```java
S onClose(Runnable closeHandler);
```
当`AutoCloseable`的`close()`接口被调用的时候会触发调用流对象的`onClose()`方法，但有几点需要注意：

- `onClose()` 方法会返回流对象本身，也就是说可以对改对象进行多次调用
- 如果调用了多个`onClose()` 方法，它会按照调用的顺序触发，但是如果某个方法有异常则只会向上抛出第一个异常
- 前一个` onClose()` 方法抛出了异常不会影响后续 `onClose()` 方法的使用
- 如果多个 `onClose()` 方法都抛出异常，只展示第一个异常的堆栈，而其他异常会被压缩，只展示部分信息
<a name="x3OBJ"></a>
## 并行流和串行流
`BaseStream`接口中分别提供了并行流和串行流两个方法，这两个方法可以任意调用若干次，也可以混合调用，但最终只会以最后一次方法调用的返回结果为准。<br />参考`parallel()`方法的说明：
> Returns an equivalent stream that is parallel. May return itself, either because the stream was already parallel, or because the underlying stream state was modified to be parallel.

所以多次调用同样的方法并不会生成新的流，而是直接复用当前的流对象。<br />下面的例子里以最后一次调用`parallel()`为准，最终是并行地计算sum：
```java
stream.parallel()
   .filter(...)
   .sequential()
   .map(...)
   .parallel()
   .sum();
```
<a name="WtMye"></a>
## ParallelStream背后的男人：ForkJoinPool
ForkJoin框架是从JDK7中新特性，它同ThreadPoolExecutor一样，也实现了Executor和ExecutorService 接口。它使用了一个「无限队列」来保存需要执行的任务，而线程的数量则是通过构造函数传入， 如果没有向构造函数中传入希望的线程数量，那么当前计算机可用的CPU数量会被设置为线程数量作为默认值。<br />ForkJoinPool主要用来使用分治法(Divide-and-Conquer Algorithm) 来解决问题，典型的应用比如_快速排序算法_。这里的要点在于，ForkJoinPool需要使用相对少的线程来处理大量的任务。<br />比如要对1000万个数据进行排序，那么会将这个任务分割成两个500 万的排序任务和一个针对这两组500万数据的合并任务。<br />以此类推，对于500万的数据也会做出同样的分割处理，到最后会设置一个阈值来规定当数据规模到多少时，停止这样的分割处理。比如，当元素的数量小于10时，会停止分割，转而使用插入排序对它们进行排序。那么到最后，所有的任务加起来会有大概2000000+个。<br />问题的关键在于，对于一个任务而言，只有当它所有的子任务完成之后，它才能够被执行，想象一下归并排序的过程。<br />所以当使用`ThreadPoolExecutor`时，使用分治法会存在问题，因为ThreadPoolExecutor中的线程无法向 任务队列中再添加一个任务并且在等待该任务完成之后再继续执行。而使用ForkJoinPool时，就能够让其中的线程创建新的任务，并挂起当前的任务，此时线程就能够从队列中选择子任务执行。<br />那么使用ThreadPoolExecutor或者ForkJoinPool，会有什么性能的差异呢？<br />首先，使用ForkJoinPool能够使用数量有限的线程来完成非常多的具有「父子关系」的任务，比如使用4个线程来完成超过200万个任务。使用ThreadPoolExecutor 时，是不可能完成的，因为ThreadPoolExecutor中的Thread无法选择优先执行子任务，需要完成200万个具有父子关系的任务时，也需要200万个线程，显然这是不可行的。<br />Work Stealing原理：

1. 每个工作线程都有自己的工作队列WorkQueue；
2. 这是一个双端队列dequeue，它是线程私有的；
3. ForkJoinTask中fork的子任务，将放入运行该任务的工作线程的队头，工作线程将以LIFO的顺序来处理工作队列中的任务，即堆栈的方式；
4. 为了最大化地利用CPU，空闲的线程将从其它线程的队列中「窃取」任务来执行
5. 但是是从工作队列的尾部窃取任务，以减少和队列所属线程之间的竞争；
6. 双端队列的操作：push()/pop()仅在其所有者工作线程中调用，poll()是由其它线程窃取任务时调用的；
7. 当只剩下最后一个任务时，还是会存在竞争，是通过CAS来实现的；
<a name="yorFq"></a>
## 用ForkJoinPool的眼光来看ParallelStream
Java 8为ForkJoinPool添加了一个通用线程池，这个线程池用来处理那些没有被显式提交到任何线程池的任务。它是ForkJoinPool类型上的一个静态元素，它拥有的默认线程数量等于运行计算机上的CPU数量。<br />当调用Arrays 类上添加的新方法时，自动并行化就会发生。<br />比如用来排序一个数组的并行快速排序，用来对一个数组中的元素进行并行遍历。自动并行化也被运用在Java 8新添加的Stream API中。<br />比如下面的代码用来遍历列表中的元素并执行需要的操作：
```java
List<UserInfo> userInfoList =
        DaoContainers.getUserInfoDAO().queryAllByList(new UserInfoModel());
userInfoList.parallelStream().forEach(RedisUserApi::setUserIdUserInfo);
```
对于列表中的元素的操作都会以并行的方式执行。`forEach`方法会为每个元素的计算操作创建一个任务，该任务会被前文中提到的`ForkJoinPool`中的`commonPool`处理。<br />以上的并行计算逻辑当然也可以使用`ThreadPoolExecutor`完成，但是就代码的可读性和代码量而言，使用`ForkJoinPool`明显更胜一筹。<br />对于`ForkJoinPool`通用线程池的线程数量，通常使用默认值就可以了，即运行时计算机的处理器数量。也可以通过设置系统属性：`-Djava.util.concurrent .ForkJoinPool.common.parallelism=N` （N为线程数量），来调整`ForkJoinPool`的线程数量。<br />值得注意的是，当前执行的线程也会被用来执行任务，所以最终的线程个数为N+1，1就是当前的主线程。<br />这里就有一个问题，如果在并行流的执行计算使用了_阻塞操作_，如I/O，那么很可能会导致一些问题：
```java
public static String query(String question) {
  List<String> engines = new ArrayList<String>();
  engines.add("http://www.google.com/?q=");
  engines.add("http://duckduckgo.com/?q=");
  engines.add("http://www.bing.com/search?q=");

  // get element as soon as it is available
  Optional<String> result = engines.stream().parallel().map((base) - {
    String url = base + question;
    // open connection and fetch the result
    return WS.url(url).get();
  }).findAny();
  return result.get();
}
```
这个例子很典型，来分析一下：

- 这个并行流计算操作将由主线程和JVM默认的`ForkJoinPool.commonPool()`来共同执行。
- map中是一个阻塞方法，需要通过访问HTTP接口并得到它的response，所以任何一个worker线程在执行到这里的时候都会阻塞并等待结果。
- 所以当此时再其他地方通过并行流方式调用计算方法的时候，将会受到此处阻塞等待的方法的影响。
- 目前的`ForkJoinPool`的实现并未考虑补偿等待那些阻塞在等待新生成的线程的工作worker线程，所以最终`ForkJoinPool.commonPool()`中的线程将备用光并且阻塞等待。

正如上面那个列子的情况分析得知，`lambda`的执行并不是瞬间完成的，所有使用parallel streams的程序都有可能成为阻塞程序的源头， 并且在执行过程中程序中的其他部分将无法访问这些workers，这意味着任何依赖parallel streams的程序在什么别的东西占用着common ForkJoinPool时将会变得不可预知并且暗藏危机。<br />小结：

1. 当需要处理递归分治算法时，考虑使用ForkJoinPool。
2. 仔细设置不再进行任务划分的阈值，这个阈值对性能有影响。
3. Java 8中的一些特性会使用到ForkJoinPool中的通用线程池。在某些场合下，需要调整该线程池的默认的线程数量
4. lambda应该尽量避免副作用，也就是说，避免突变基于堆的状态以及任何IO
5. lambda应该互不干扰，也就是说避免修改数据源（因为这可能带来线程安全的问题）
6. 避免访问在流操作生命周期内可能会改变的状态
<a name="halNe"></a>
## 并行流的性能
并行流框架的性能受以下因素影响：

- 数据大小：数据够大，每个管道处理时间够长，并行才有意义；
- 源数据结构：每个管道操作都是基于初始数据源，通常是集合，将不同的集合数据源分割会有一定消耗；
- 装箱：处理基本类型比装箱类型要快；
- 核的数量：默认情况下，核数量越多，底层fork/join线程池启动线程就越多；
- 单元处理开销：花在流中每个元素身上的时间越长，并行操作带来的性能提升越明显；

源数据结构分为以下3组：

- 性能好：`ArrayList`、数组或`IntStream.range`(数据支持随机读取，能轻易地被任意分割)
- 性能一般：`HashSet`、`TreeSet`(数据不易公平地分解，大部分也是可以的)
- 性能差：`LinkedList`(需要遍历链表，难以对半分解)、`Stream.iterate`和`BufferedReader.lines`(长度未知，难以分解)

注意：下面几个部分节选自：Streams 的幕后原理，顺便感谢一下作者_Brian Goetz_，写的太通透了。
<a name="CeD7b"></a>
## NQ模型
要确定并行性是否会带来提速，需要考虑的最后两个因素是：可用的数据量和针对每个数据元素执行的计算量。<br />在最初的并行分解描述中，采用的概念是拆分来源，直到分段足够小，以致解决该分段上的问题的顺序方法更高效。分段大小必须依赖于所解决的问题，确切的讲，取决于每个元素完成的工作量。<br />例如，计算一个字符串的长度涉及的工作比计算字符串的 SHA-1 哈希值要少得多。为每个元素完成的工作越多，“大到足够利用并行性” 的阈值就越低。类似地，拥有的数据越多， 拆分的分段就越多，而不会与 “太小” 阈值发生冲突。<br />一个简单但有用的并行性能模型是 NQ 模型，其中 N 是数据元素数量，Q 是为每个元素执行的工作量。乘积 N*Q 越大，就越有可能获得并行提速。对于具有很小的 Q 的问题，比如对数字求和，您通常可能希望看到 N > 10,000 以获得提速；随着 Q 增加，获得提速所需的数据大小将会减小。<br />并行化的许多阻碍（比如拆分成本、组合成本或遇到顺序敏感性）都可以通过 Q 更高的操作来缓解。尽管拆分某个 LinkedList 特征的结果可能很糟糕，但只要拥有足够大的 Q，仍然可能获得并行提速。
<a name="EJkCT"></a>
## 遇到顺序
遇到顺序指的是来源分发元素的顺序是否对计算至关重要。一些来源（比如基于哈希的集合和映射）没有有意义的遇到顺序。流标志 ORDERED 描述了流是否有有意义的遇到顺序。<br />JDK 集合的 `spliterator` 会根据集合的规范来设置此标志；<br />一些中间操作可能注入 ORDERED (`sorted()`) 或清除它 (`unordered()`)。<br />如果流没有遇到顺序，大部分流操作都必须遵守该顺序。对于顺序执行，会「自动保留遇到顺序」，因为元素会按遇到它们的顺序自然地处理。<br />甚至在并行执行中，许多操作（无状态中间操作和一些终止操作（比如 `reduce()`）），遵守遇到顺序不会产生任何实际成本。<br />但对于其他操作（有状态中间操作，其语义与遇到顺序关联的终止操作，比如 `findFirst()` 或 `forEachOrdered()`）， 在并行执行中遵守遇到顺序的责任可能很重大。<br />如果流有一个已定义的遇到顺序，但该顺序对结果没有意义， 那么可以通过使用 `unordered()` 操作删除 ORDERED 标志，加速包含顺序敏感型操作的管道的顺序执行。<br />作为对遇到顺序敏感的操作的示例，可以考虑 `limit()`，它会在指定大小处截断一个流。在顺序执行中实现 `limit()` 很简单：保留一个已看到多少元素的计数器，在这之后丢弃任何元素。<br />但是在并行执行中，实现 `limit()` 要复杂得多；您需要保留前 N 个元素。此要求大大限制了利用并行性的能力；如果输入划分为多个部分，您只有在某个部分之前的所有部分都已完成后，才知道该部分的结果是否将包含在最终结果中。<br />因此，该实现一般会错误地选择不使用所有可用的核心，或者缓存整个试验性结果，直到您达到目标长度。<br />如果流没有遇到顺序，`limit()` 操作可以自由选择任何 N 个元素，这让执行效率变得高得多。知道元素后可立即将其发往下游， 无需任何缓存，而且线程之间唯一需要执行的协调是发送一个信号来确保未超出目标流长度。<br />遇到顺序成本的另一个不太常见的示例是排序。如果遇到顺序有意义，那么 `sorted()` 操作会实现一种稳定 排序 （相同的元素按照它们进入输入时的相同顺序出现在输出中），而对于无序的流，稳定性（具有成本）不是必需的。<br />`distinct()` 具有类似的情况：如果流有一个遇到顺序，那么对于多个相同的输入元素，`distinct()` 必须发出其中的第一个， 而对于无序的流，它可以发出任何元素 — 同样可以获得高效得多的并行实现。<br />在使用 `collect()` 聚合时会遇到类似的情形。如果在无序流上执行 `collect(groupingBy()`) 操作， 与任何键对应的元素都必须按它们在输入中出现的顺序提供给下游收集器。<br />此顺序对应用程序通常没有什么意义，而且任何顺序都没有意义。在这些情况下，可能最好选择一个并发 收集器（比如 `groupingByConcurrent()`），它可以忽略遇到顺序， 并让所有线程直接收集到一个共享的并发数据结构中（比如 `ConcurrentHashMap`），而不是让每个线程收集到它自己的中间映射中， 然后再合并中间映射（这可能产生很高的成本）。
