<a name="Z3QYC"></a>
## 为什么要有线程池
相信广大 Java开发者都对 ThreadPool 有所耳闻，所谓线程池顾名思义就是一个保存线程的“池子”，它主要有管理和维护及分配线程的作用。现在假想一个场景：需要从数据库中打印五万条数据到 excel 表格中，相信敏锐的读者会第一时间反应过来，五万条数据对 IO 及 CPU 的资源占用是不小的，为了减轻服务器压力，会第一时间想到新开线程对其做出处理，那如果有很多导出数据的请求过来怎么办呢？线程无止境的创建销毁一定是个不晓得麻烦，在这种场景下，一个能自动维护线程的工具让我们如虎添翼—— ThreadPoolExecutor。
<a name="L4ltk"></a>
## 线程池的作用
线程池能够对线程进行统一分配，调优和监控：

- 降低资源消耗(线程无限制地创建，然后使用完毕后销毁)
- 提高响应速度(无须创建线程)
- 提高线程的可管理性
<a name="TJfHz"></a>
## ThreadPoolExecutor
现在大概知道有 ThreadPoolExecutor 这么一个管理线程的工具，介绍下其原理。简单来说，java 线程池主要包含两个部分：线程集合 workerSet 和阻塞队列 workQueue。当用户向线程池提交一个任务(也就是线程)时，线程池会先将任务放入workQueue 中。workerSet 中的线程会不断的从 workQueue 中获取线程然后执行。当 workQueue 中没有任务的时候，worker 就会阻塞，直到队列中有任务了就取出来继续执行。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1700535117659-ddee8ef0-2c7d-4a52-8060-2c67e885dc75.png#averageHue=%23f5f5f5&clientId=u407ce368-c204-4&from=paste&id=uefdf6348&originHeight=512&originWidth=1015&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua8b5ba88-d8be-43ad-bb31-664c5bb43c1&title=)<br />下面详细讲讲它的运行逻辑：
<a name="RnvsH"></a>
### 核心参数
```java
private static final ExecutorService threadPoolExecutor =
  new ThreadPoolExecutor(10, 10, 0L, TimeUnit.MILLISECONDS
                , taskQueue
                , new NamedThreadFactory("project_flow_match_work", Boolean.FALSE));
```

---

```java
public ThreadPoolExecutor(int corePoolSize,
                          int maximumPoolSize,
                          long keepAliveTime,
                          TimeUnit unit,
                          BlockingQueue<Runnable> workQueue,
                          ThreadFactory threadFactory) {
    this(corePoolSize, maximumPoolSize, keepAliveTime, unit, workQueue,
         threadFactory, defaultHandler);
```
corePoolSize 线程池中的核心线程数，当提交一个任务时，线程池创建一个新线程执行任务，直到当前线程数等于 `corePoolSize`，即使有其他空闲线程能够执行新来的任务，也会继续创建线程；如果当前线程数为 corePoolSize，继续提交的任务被保存到阻塞队列中，等待被执行；如果执行了线程池的 `prestartAllCoreThreads()` 方法，线程池会提前创建并启动所有核心线程。<br />maximumPoolSize 线程池中允许的最大线程数。如果当前阻塞队列满了，且继续提交任务，则创建新的线程执行任务，前提是当前线程数小于 `maximumPoolSize`；当阻塞队列是无界队列，则 `maximumPoolSize` 不起作用，因为无法提交至核心线程池的线程会一直持续地放入 workQueue。<br />keepAliveTime线程空闲时的存活时间，即当线程没有任务执行时，该线程继续存活的时间；默认情况下，该参数只在线程数大于 corePoolSize 时才有用，超过这个时间的空闲线程将被终止；<br />workQueue 用来保存等待被执行的任务的阻塞队列。在 JDK 中提供了如下阻塞队列

- `ArrayBlockingQueue`：基于数组结构的有界阻塞队列，按 FIFO 排序任务；
- `LinkedBlockingQueue`：基于链表结构的阻塞队列，按 FIFO 排序任务，吞吐量通常要高于 ArrayBlockingQueue；
- `SynchronousQueue`：一个不存储元素的阻塞队列，每个插入操作必须等到另一个线程调用移除操作，否则插入操作一直处于阻塞状态，吞吐量通常要高于 `LinkedBlockingQueue`；
- `PriorityBlockingQueue`：具有优先级的无界阻塞队列；

threadFactory 创建线程的工厂，通过自定义的线程工厂可以给每个新建的线程设置一个具有识别度的线程名。默认为 `DefaultThreadFactory`；<br />handler 线程池的饱和策略，当阻塞队列满了，且没有空闲的工作线程，如果继续提交任务，必须采取一种策略处理该任务，线程池提供了4种策略：

- `AbortPolicy`：直接抛出异常，默认策略；
- `CallerRunsPolicy`：用调用者所在的线程来执行任务；
- `DiscardOldestPolicy`：丢弃阻塞队列中靠最前的任务，并执行当前任务；
- `DiscardPolicy`：直接丢弃任务；
1. 如果当前运行的线程数小于核心线程数，那么就会新建一个线程来执行任务。
2. 如果当前运行的线程数等于或大于核心线程数，但是小于最大线程数，那么就把该任务放入到任务队列里等待执行。
3. ·如果向任务队列投放任务失败（任务队列已经满了），但是当前运行的线程数是小于最大线程数的，就新建一个线程来执行任务。
4. 如果当前运行的线程数已经等同于最大线程数了，新建线程将会使当前运行的线程超出最大线程数，那么当前任务会被拒绝，饱和策略会调用`RejectedExecutionHandler.rejectedExecution()`方法。

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1700535117591-4c3ff076-f51e-481a-b061-964b680c3993.png#averageHue=%23e6f5ee&clientId=u407ce368-c204-4&from=paste&id=uca4891dc&originHeight=251&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uda38cfc1-2e79-496e-85a0-7d9fe512138&title=)<br />`ThreadPoolTaskExecutor` 定义一些策略：

- `ThreadPoolExecutor.AbortPolicy`：抛出 `RejectedExecutionException`来拒绝新任务的处理。
- `ThreadPoolExecutor.CallerRunsPolicy`：调用执行自己的线程运行任务，也就是直接在调用`execute`方法的线程中运行(run)被拒绝的任务，如果执行程序已关闭，则会丢弃该任务。因此这种策略会降低对于新任务提交速度，影响程序的整体性能。如果您的应用程序可以承受此延迟并且你要求任何一个任务请求都要被执行的话，可以选择这个策略。
- `ThreadPoolExecutor.DiscardPolicy`：不处理新任务，直接丢弃掉。
- `ThreadPoolExecutor.DiscardOldestPolicy`：此策略将丢弃最早的未处理的任务请求。
<a name="AXjuS"></a>
### 三种类型
一般情况会将线程池分为以下三种类型，主要关注构造方法的入参并结合上文的运行逻辑加以思考：
<a name="jLU2G"></a>
#### newFixedThreadPool
```java
public static ExecutorService newFixedThreadPool(int nThreads) {
  return new ThreadPoolExecutor(nThreads, nThreads,
                                0L, TimeUnit.MILLISECONDS,
                                new LinkedBlockingQueue<Runnable>());
}
```
可以发现其中核心线程数及最大线程数一样，很容易推理出在达到核心线程数之后线程池并不会扩容，线程池的线程数量达 corePoolSize 后，即使线程池没有可执行任务时，也不会释放线程，很容易发现这是一个含有固定线程数的线程池，一般使用在规定最大并发数量的场景。<br />`FixedThreadPool` 的工作队列为无界队列 `LinkedBlockingQueue` (队列容量为 `Integer.MAX_VALUE`)，这会导致以下问题：

- 线程池里的线程数量不超过 corePoolSize，这导致了 `maximumPoolSize` 和 keepAliveTime 将会是个无用参数
- 由于使用了无界队列，所以 `FixedThreadPool` 永远不会拒绝，即饱和策略失效
<a name="NbBiM"></a>
#### `newSingleThreadExecutor`
```java
public static ExecutorService newSingleThreadExecutor() {
    return new FinalizableDelegatedExecutorService
    (new ThreadPoolExecutor(1, 1,
                            0L, TimeUnit.MILLISECONDS,
                            new LinkedBlockingQueue<Runnable>()));
}
```
初始化的线程池中只有一个线程，如果该线程异常结束，会重新创建一个新的线程继续执行任务，唯一的线程可以保证所提交任务的顺序执行。<br />由于使用了无界队列，所以 `SingleThreadPool` 永远不会拒绝，即饱和策略失效
<a name="KfBHq"></a>
#### `newCachedThreadPool`
```java
public static ExecutorService newCachedThreadPool() {
    return new ThreadPoolExecutor(0, Integer.MAX_VALUE,
                                  60L, TimeUnit.SECONDS,
                                  new SynchronousQueue<Runnable>());
}
```
线程池的线程数可达到 `Integer.MAX_VALUE`，即 2147483647，内部使用 `SynchronousQueue` 作为阻塞队列；和 `newFixedThreadPool` 创建的线程池不同，`newCachedThreadPool` 在没有任务执行时，当线程的空闲时间超过 keepAliveTime，会自动释放线程资源，当提交新任务时，如果没有空闲线程，则创建新线程执行任务，会导致一定的系统开销， 执行过程与前两种稍微不同：

- 主线程调用 `SynchronousQueue` 的 `offer()` 方法放入 task，倘若此时线程池中有空闲的线程尝试读取 `SynchronousQueue` 的 task，即调用了 `SynchronousQueue` 的 `poll()`，那么主线程将该 task 交给空闲线程。否则执行(2)
- 当线程池为空或者没有空闲的线程，则创建新的线程执行任务。
- 执行完任务的线程倘若在 60s 内仍空闲，则会被终止。因此长时间空闲的 `CachedThreadPool` 不会持有任何线程资源。
<a name="XEvo5"></a>
## `ThreadPoolExecutor`源码详解
<a name="iXXJz"></a>
### 几个关键属性
```java
//这个属性是用来存放 当前运行的worker数量以及线程池状态的
//int是32位的，这里把int的高3位拿来充当线程池状态的标志位,后29位拿来充当当前运行worker的数量
private final AtomicInteger ctl = new AtomicInteger(ctlOf(RUNNING, 0));
//存放任务的阻塞队列
private final BlockingQueue<Runnable> workQueue;
//worker的集合,用set来存放
private final HashSet<Worker> workers = new HashSet<Worker>();
//历史达到的worker数最大值
private int largestPoolSize;
//当队列满了并且worker的数量达到maxSize的时候,执行具体的拒绝策略
private volatile RejectedExecutionHandler handler;
//超出coreSize的worker的生存时间
private volatile long keepAliveTime;
//常驻worker的数量
private volatile int corePoolSize;
//最大worker的数量,一般当workQueue满了才会用到这个参数
private volatile int maximumPoolSize;
```
<a name="mDfSl"></a>
### 内部状态
```java
private final AtomicInteger ctl = new AtomicInteger(ctlOf(RUNNING, 0));
private static final int COUNT_BITS = Integer.SIZE - 3;
private static final int CAPACITY   = (1 << COUNT_BITS) - 1;

// runState is stored in the high-order bits
private static final int RUNNING    = -1 << COUNT_BITS;
private static final int SHUTDOWN   =  0 << COUNT_BITS;
private static final int STOP       =  1 << COUNT_BITS;
private static final int TIDYING    =  2 << COUNT_BITS;
private static final int TERMINATED =  3 << COUNT_BITS;

// Packing and unpacking ctl
private static int runStateOf(int c)     { return c & ~CAPACITY; }
private static int workerCountOf(int c)  { return c & CAPACITY; }
private static int ctlOf(int rs, int wc) { return rs | wc; }
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1700535117570-43285c77-a1a3-4c39-9ad6-d1640e3d61ee.png#averageHue=%23e6f5ee&clientId=u407ce368-c204-4&from=paste&id=u8c2b65ba&originHeight=251&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ufb82a227-0329-42fb-8a88-357fe74cc29&title=)<br />RUNNING：该状态的线程池会接收新任务，并处理阻塞队列中的任务；<br />SHUTDOWN：该状态的线程池不会接收新任务，但会处理阻塞队列中的任务；<br />STOP：该状态的线程不会接收新任务，也不会处理阻塞队列中的任务，而且会中断正在运行的任务；<br />TIDYING：所有的任务都已经终止；<br />TERMINATED：terminated() 方法已经执行完成
<a name="lhERy"></a>
### 配置线程池需要考虑因素
从任务的优先级，任务的执行时间长短，任务的性质( CPU 密集 / IO 密集)，任务的依赖关系这四个角度来分析。并且近可能地使用有界的工作队列。<br />性质不同的任务可用使用不同规模的线程池分开处理：

- CPU 密集型：尽可能少的线程，Ncpu + 1
- IO 密集型：尽可能多的线程，Ncpu * 2，比如数据库连接池
- 混合型：CPU 密集型的任务与 IO 密集型任务的执行时间差别较小，拆分为两个线程池；否则没有必要拆分。

实际上，确定线程池大小并不是一个简单的事，要分析业务需求、服务器压力、网络 IO 等等，以上的确定方式可以保证不会出大问题但是想精确还远远不够，简单介绍一下阿姆达尔定律——对于固定负载情况下描述并行处理效果的加速比 s，阿姆达尔经过深入研究给出了如下公式：S = 1/ ( ( 1 - a ) + a / n )，其中，a 为并行计算部分所占比例，n 为并行处理结点个数。这样，当 1 - a = 0 时，(即没有串行，只有并行)最大加速比 s = n ；当 a = 0 时（即只有串行，没有并行），最小加速比 s = 1；当 n → ∞ 时，极限加速比 s → 1 /（1 - a ），这也就是加速比的上限。例如，若串行代码占整个代码的 25%，则并行处理的总体性能不可能超过 4。这一公式已被学术界所接受，并被称做“阿姆达尔定律”，也称为“安达尔定理”( Amdahl law )。
<a name="kKw9h"></a>
### 监控线程池的状态
可以使用ThreadPoolExecutor以下方法：

- `getTaskCount()` Returns the approximate total number of tasks that have ever been scheduled for execution.
- `getCompletedTaskCount()` Returns the approximate total number of tasks that have completed execution. 返回结果少于 getTaskCount()。
- `getLargestPoolSize()` Returns the largest number of threads that have ever simultaneously been in the pool. 返回结果小于等于 maximumPoolSize
- `getPoolSize()` Returns the current number of threads in the pool.
- `getActiveCount()` Returns the approximate number of threads that are actively executing tasks.
<a name="Bvj9e"></a>
## 回到开头
还记得开头设想的场景吗？这是在开发中实际遇到的需求，以下是解决方案，希望对大家有借鉴意义。
```java
public  ExportSupport() {
  threadPoolExecutor = new ThreadPoolExecutor(10, 10, 10, TimeUnit.SECONDS
    , new LinkedBlockingQueue<>(500), runnable ->
       new Thread(runnable, "Export_Thread-" + threadNo.getAndIncrement())
    );
}
```
可以发现采用了前面提到的 `newFixedThreadPool` 类型，这是考虑到业务的需求需要固定线程数。
