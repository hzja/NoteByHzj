Java ThreadPoolExecutor
<a name="FFhLE"></a>
### 线程池的作用
● 利用线程池管理并复用线程、控制最大并发数等既然使用了线程池就需要确保线程池是在复用的，每次new一个线程池出来可能比不用线程池还糟糕。如果没有直接声明线程池而是使用其他人提供的类库来获得一个线程池，请务必查看源码，以确认线程池的实例化方式和配置是符合预期的。<br />● 实现任务线程队列缓存策略和拒绝机制。<br />● 实现某些与时间相关的功能，如定时执行、周期执行等<br />● 隔离线程环境比如，交易服务和搜索服务在同一台服务器上，分别开启两个线程池，交易线程的资源消耗明显要大；因此，通过配置独立的线程池，将较慢的交易服务与搜索服务隔离开，避免各服务线程相互影响。<br />Java中的线程池是运用场景最多的并发框架，几乎所有需要异步或并发执行任务的程序 都可以使用线程池。合理地使用线程池能够带来3个好处：<br />1、降低资源消耗。通过重复利用已创建的线程降低线程创建和销毁造成的消耗。<br />2、提高响应速度。当任务到达时，任务可以不需要等到线程创建就能立即执行。<br />3、提高线程的可管理性。线程是稀缺资源，如果无限制地创建，不仅会消耗系统资源， 还会降低系统的稳定性，使用线程池可以进行统一分配、调优和监控。
<a name="AF4iu"></a>
### 线程池的主要处理流程
![2021-05-07-13-33-03-966222.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620366519346-82aa23e7-6466-4e76-81c2-acc0043aaa13.png#height=768&id=u289e4380&originHeight=768&originWidth=502&originalType=binary&ratio=1&size=1159327&status=done&style=shadow&width=502)
<a name="L8FfO"></a>
#### 接口定义和实现类
| 类型 | 名称 | 描述 |
| --- | --- | --- |
| 接口 | `Executor` | 最上层的接口，定义了执行任务的方法`execute` |
| 接口 | `ExecutorService` | 继承了`Executor`接口，拓展了`Callable`、`Future`、关闭方法 |
| 接口 | `ScheduledExecutorService` | 继承了`ExecutorService`，增加了定时任务相关方法 |
| 实现类 | `ThreadPoolExecutor` | 基础、标准的线程池实现 |
| 实现类 | `ScheduledThreadPoolExecutor` | 继承了`ThreadPoolExecutor`，实现了`ScheduledExecutorService`中相关定时任务的方法 |

<a name="k26pP"></a>
### ThreadPoolExecutor 类图
![2021-05-07-13-33-05-636628.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620366431683-afe46b4d-feac-45b8-bab9-8cad9c54dd8d.png#height=486&id=ua1945e9b&originHeight=486&originWidth=822&originalType=binary&ratio=1&size=7295&status=done&style=shadow&width=822)<br />java中的线程池都是基于ThreadPoolExecutor 来实现的。<br />可以认为ScheduledThreadPoolExecutor是最丰富的实现类。
<a name="tiY3Z"></a>
### ExecutorService 方法定义
```java
public interface ExecutorService extends Executor {

    /**
     * 在之前提交的，需要被执行的任务中，有序的进行关闭操作，并且此时不会再接受新的任务
     * 如果此时所有的任务已经关闭的话，那么就不会起到什么效果，因为已经没有任务可关闭了
     */
    void shutdown();

    /**
     * 尝试关闭所有正在执行的任务，并且中断正在等待要执行的任务，返回一个包含正在等待的任务的列表
     * @return
     */
    List<Runnable> shutdownNow();

    /**
     * 如果线程已经关闭了，就返回true
     * @return
     */
    boolean isShutdown();

    /**
     * 如果所有的线程任务已经关闭了，就返回true
     * @return
     */
    boolean isTerminated();

    /**
     * 只有当所有的任务都成功执行，否则会一直处于阻塞状态，只有当一下情况发生时，才会中断阻塞
     * 例如收到一个关闭的请求，或者超时发生、或者当前的线程被中断后
     * @param timeout
     * @param unit
     * @return
     * @throws InterruptedException
     */
    boolean awaitTermination(long timeout, TimeUnit unit)
        throws InterruptedException;

    /**
     * 提交一个需要返回结果的任务去执行，返回一个有结果的消息体，只有成功执行后，才会返回结果
     * @param task
     * @param <T>
     * @return
     */
    <T> Future<T> submit(Callable<T> task);

    /**
     * 只有当任务成功被执行后，才会返回给定的结果
     * @param task
     * @param result
     * @param <T>
     * @return
     */
    <T> Future<T> submit(Runnable task, T result);

    /**
     * 提交一个Runnable任务用于执行，和返回代表任务的Future。
     * Future的get方法成功执行后，返回null
     */
    Future<?> submit(Runnable task);

    /**
     * 提交一批任务，并返回一批任务的结果列表
     * @param tasks
     * @param <T>
     * @return
     * @throws InterruptedException
     */
    <T> List<Future<T>> invokeAll(Collection<? extends Callable<T>> tasks)
        throws InterruptedException;

    /**
     * 执行给定的任务集合，执行完毕或者超时后，返回结果，其他任务终止
     *
     */
    <T> List<Future<T>> invokeAll(Collection<? extends Callable<T>> tasks,
                                  long timeout, TimeUnit unit)
        throws InterruptedException;

    /**
     * 提交一批任务信息，当其中一个成功的执行，没有返回异常的时候，就返回结果
     * @param tasks
     * @param <T>
     * @return
     * @throws InterruptedException
     * @throws ExecutionException
     */
    <T> T invokeAny(Collection<? extends Callable<T>> tasks)
        throws InterruptedException, ExecutionException;

    /**
     * 执行给定的任务集合，任意一个执行成功或超时后，返回结果，其他任务终止
     */
    <T> T invokeAny(Collection<? extends Callable<T>> tasks,
                    long timeout, TimeUnit unit)
        throws InterruptedException, ExecutionException, TimeoutException;
}
```
<a name="xgwle"></a>
#### ScheduledExecutorService
```java
public interface ScheduledExecutorService extends ExecutorService {

    //创建并执行一个一次性任务, 过了延迟时间就会被执行
    public ScheduledFuture<?> schedule(Runnable command,
                                       long delay, TimeUnit unit);

    //创建并执行一个一次性任务, 过了延迟时间就会被执行
    public <V> ScheduledFuture<V> schedule(Callable<V> callable,
                                           long delay, TimeUnit unit);

    //创建并执行一个周期性任务
    //过了给定的初始延迟时间，会第一次被执行
    //执行过程中发生了异常,那么任务就停止
    //一次任务 执行时长超过了周期时间，下一次任务会等到该次任务执行结束后，立刻执行，
    //这也是它和scheduleWithFixedDelay的重要区别
    public ScheduledFuture<?> scheduleAtFixedRate(Runnable command,
                                                  long initialDelay,
                                                  long period,
                                                  TimeUnit unit);

    //创建并执行一个周期性任务
    //过了初始延迟时间，第一次被执行，后续以给定的周期时间执行
    //执行过程中发生了异常,那么任务就停止
    //一次任务执行时长超过了周期时间，下一次任务会在该次任务执行结束的时间基础上，计算执行延时。
    //对于超过周期的长时间处理任务的不同处理方式，这是它和scheduleAtFixedRate的重要区别。
    public ScheduledFuture<?> scheduleWithFixedDelay(Runnable command,
                                                     long initialDelay,
                                                     long delay,
                                                     TimeUnit unit);

}
```
<a name="vWTl3"></a>
### Executors工具类常用方法
可以自己实例化线程池，也可用Executors创建线程池的工厂类，推荐自己实例化线程池。<br />ExecutorService 的抽象类AbstractExecutorService提供了submit、invokeAll 等方法的实现，但是核心方法`Executor.execute()`并没有在这里实现。因为所有的任务都在该方法执行，不同实现会带来不同的执行策略。<br />通过Executors的静态工厂方法可以创建三个线程池的包装对象

- ForkJoinPool
- ThreadPoolExecutor
- ScheduledThreadPoolExecutor
<a name="lteE3"></a>
#### ●`Executors.newWorkStealingPool`
JDK8 引入，创建持有足够线程的线程池支持给定的并行度，并通过使用多个队列减少竞争，构造方法中把CPU数量设置为默认的并行度。返回ForkJoinPool ( JDK7引入)对象，它也是AbstractExecutorService 的子类
```java
public static ExecutorService newWorkStealingPool(int parallelism) {
    return new ForkJoinPool
        (parallelism,
         ForkJoinPool.defaultForkJoinWorkerThreadFactory,
         null, true);
}
```
<a name="hNZnX"></a>
#### ●`Executors.newCachedThreadPool`
创建一个无界的缓冲线程池，它的任务队列是一个同步队列。任务加入到池中

- 若池中有空闲线程，则用空闲线程执行
- 若无，则创建新线程执行
- 池中的线程空闲超过60秒，将被销毁。线程数随任务的多少变化。适用于执行耗时较小的异步任务。
- `线程池的核心线程数=0`，`最大线程数= Integer.MAX_ _VALUE`
- `maximumPoolSize` 最大可至`Integer.MAX_VALUE`，是高度可伸缩的线程池。若达到该上限，没有服务器能够继续工作，直接OOM。
- `keepAliveTime`默认为60秒;
- 工作线程处于空闲状态，则回收工作线程；如果任务数增加，再次创建出新线程处理任务。
```java
public static ExecutorService newCachedThreadPool() {
    return new ThreadPoolExecutor(0, Integer.MAX_VALUE,
                                  60L, TimeUnit.SECONDS,
                                  new SynchronousQueue<Runnable>());
}
```
<a name="trRXo"></a>
#### ●`Executors.newScheduledThreadPool`
能定时执行任务的线程池。该池的核心线程数由参数指定，线程数最大至`Integer.MAX_ VALUE`，与上述一样存在OOM风险。`ScheduledExecutorService`接口的实现类，支持定时及周期性任务执行；相比`Timer`、`ScheduledExecutorService` 更安全，功能更强大。与`newCachedThreadPool`的区别是不回收工作线程。
```java
public static ScheduledExecutorService newScheduledThreadPool(int corePoolSize) {
    return new ScheduledThreadPoolExecutor(corePoolSize);
}
```
<a name="BFnR9"></a>
#### ●`Executors.newSingleThreadExecutor`
创建一个单线程的线程池，相当于单线程串行执行所有任务，保证按任务的提交顺序依次执行。只有1个线程来执行无界任务队列的单-线程池。该线程池确保任务按加入的顺序一个一个依次执行。当唯一的线程因任务异常中止时，将创建一个新的线程来继续执行后续的任务。与newFixedThreadPool(1)的区别在于，单线程池的池大小在newSingleThreadExecutor方法中硬编码，不能再改变的。
```java
public static ExecutorService newSingleThreadExecutor() {
    return new FinalizableDelegatedExecutorService
        (new ThreadPoolExecutor(1, 1,
                                0L, TimeUnit.MILLISECONDS,
                                new LinkedBlockingQueue<Runnable>()));
}
```
<a name="nNaQ4"></a>
#### ●`Executors.newFixedThreadPool`
创建一个固定大小任务队列容量无界的线程池，输入的参数即是固定线程数；既是核心线程数也是最大线程数；不存在空闲线程，所以keepAliveTime等于0。
```java
public static ExecutorService newFixedThreadPool(int nThreads) {
    return new ThreadPoolExecutor(nThreads, nThreads,
                                  0L, TimeUnit.MILLISECONDS,
                                  new LinkedBlockingQueue<Runnable>());
}
```
<a name="MCtTD"></a>
### ThreadPoolExecutor 核心属性
```java
// 状态控制属性：高3位表示线程池的运行状态，剩下的29位表示当前有效的线程数量
private final AtomicInteger ctl = new AtomicInteger(ctlOf(RUNNING, 0));

// 线程池的基本大小，当提交一个任务到线程池时，线程池会创建一个线程来执行任务，
// 即使其他空闲的基本线程能够执行新任务也会创建线程，等到需要执行的任务数大于
// 线程池基本大小时就不再创建。如果调用了线程池的prestartAllCoreThreads()方法，
// 线程池会提前创建并启动所有基本线程。
private volatile int corePoolSize;

// 线程池线程最大数量，如果队列满了，并且已创建的线程数小于最大线程数，
// 则线程池会再创建新的线程执行任务。如果使用了无界的任务队列这个参数就没什么效果。
private volatile int maximumPoolSize;

// 用于设置创建线程的工厂，可以通过线程工厂给每个创建出来的线程设 置更有意义的名字。
private volatile ThreadFactory threadFactory;

// 饱和策略，默认情况下是AbortPolicy。
private volatile RejectedExecutionHandler handler;

// 线程池的工作线程空闲后，保持存活的时间。如果任务很多，并且每个任务执行的时间比较短，
// 可以调大时间，提高线程的利用率。
private volatile long keepAliveTime;

// 用于保存等待执行的任务的阻塞队列
private final BlockingQueue<Runnable> workQueue;

// 存放工作线程的容器，必须获取到锁才能访问
private final HashSet<Worker> workers = new HashSet<Worker>();

// ctl的拆包和包装
private static int runStateOf(int c)     { return c & ~CAPACITY; }
private static int workerCountOf(int c)  { return c & CAPACITY; }
private static int ctlOf(int rs, int wc) { return rs | wc; }
```
:::info
ctl状态控制属性，高3位表示线程池的运行状态（runState），剩下的29位表示当前有效的线程数量（workerCount）线程池最大线程数是(1 << COUNT_BITS) - 1 = 536 870 911
:::
```java
@Native public static final int SIZE = 32;
private static final int COUNT_BITS = Integer.SIZE - 3;
private static final int CAPACITY   = (1 << COUNT_BITS) - 1;
```
<a name="ZNIKn"></a>
##### 线程池的运行状态runState
| 状态 | 解释 |
| --- | --- |
| `RUNNING` | 运行态，可处理新任务并执行队列中的任务 |
| `SHUTDOW` | 关闭态，不接受新任务，但处理队列中的任务 |
| `STOP` | 停止态，不接受新任务，不处理队列中任务，且打断运行中任务 |
| `TIDYING` | 整理态，所有任务已经结束，`workerCount = 0` ，将执行`terminated()`方法 |
| `TERMINATED` | 结束态，`terminated()` 方法已完成 |

![2021-05-07-13-33-07-649700.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620366140523-b857cd06-996f-469c-9aae-bab471589aca.png#height=583&id=u07e3656a&originHeight=583&originWidth=507&originalType=binary&ratio=1&size=108374&status=done&style=shadow&width=507)
<a name="oJ5H3"></a>
#### RejectedExecutionHandler（拒绝策略）

- `AbortPolicy`：直接抛出异常。
- `CallerRunsPolicy`：只用调用者所在线程来运行任务。
- `DiscardOldestPolicy`：丢弃队列里最近的一个任务，并执行当前任务。
- `DiscardPolicy`：不处理，丢弃掉。
<a name="Churv"></a>
### 核心内部类 Worker
```java
private final class Worker  extends AbstractQueuedSynchronizer  implements Runnable {
    // 正在执行任务的线程
    final Thread thread;
    // 线程创建时初始化的任务
    Runnable firstTask;
    // 完成任务计数器
    volatile long completedTasks;

    Worker(Runnable firstTask) {
        // 在runWorker方法运行之前禁止中断，要中断线程必须先获取worker内部的互斥锁
        setState(-1); // inhibit interrupts until runWorker
        this.firstTask = firstTask;
        this.thread = getThreadFactory().newThread(this);
    }

    /** delegates main run loop to outer runworker  */
    // 直接委托给外部runworker方法
    public void run() {
        runWorker(this);
    }

    ...
}
```
Worker 类将执行任务的线程封装到了内部，在初始化Worker 的时候，会调用ThreadFactory初始化新线程；Worker 继承了AbstractQueuedSynchronizer，在内部实现了一个互斥锁，主要目的是控制工作线程的中断状态。<br />线程的中断一般是由其他线程发起的，比如ThreadPoolExecutor#interruptIdleWorkers(boolean)方法，它在调用过程中会去中断worker内部的工作线程，Work的互斥锁可以保证正在执行的任务不被打断。它是怎么保证的呢？在线程真正执行任务的时候，也就是runWorker方法被调用时，它会先获取到Work的锁，当在其他线程需要中断当前线程时也需要获取到work的互斥锁，否则不能中断。
<a name="Zb8Zt"></a>
### 构造函数
```java
public ThreadPoolExecutor(int corePoolSize,
                          int maximumPoolSize,
                          long keepAliveTime,
                          TimeUnit unit,
                          BlockingQueue<Runnable> workQueue,
                          ThreadFactory threadFactory,
                          RejectedExecutionHandler handler) {
    if (corePoolSize < 0 ||
        maximumPoolSize <= 0 ||
        maximumPoolSize < corePoolSize ||
        keepAliveTime < 0)
        throw new IllegalArgumentException();
    if (workQueue == null || threadFactory == null || handler == null)
        throw new NullPointerException();
    this.acc = System.getSecurityManager() == null ?
            null :
            AccessController.getContext();
    this.corePoolSize = corePoolSize;
    this.maximumPoolSize = maximumPoolSize;
    this.workQueue = workQueue;
    this.keepAliveTime = unit.toNanos(keepAliveTime);
    this.threadFactory = threadFactory;
    this.handler = handler;
}
```
通过构造函数可以发现，构造函数就是在对线程池核心属性进行赋值，下面来介绍一下这些核心属性：

- `corePoolSize`：核心线程数
- `maximumPoolSize`：线程池最大数量
- `keepAliveTime`：线程池的工作线程空闲后，保持存活的时间。
- `unit`：线程活动保持时间的单位。
- `workQueue`：用于保存等待执行的任务的阻塞队列，具体可以参考JAVA并发容器-阻塞队列
- `threadFactory`：用于设置创建线程的工厂
- `handler`：饱和策略，默认情况下是`AbortPolicy`。
<a name="frdXe"></a>
### execute() 提交线程
```java
public void execute(Runnable command) {
    if (command == null)
        throw new NullPointerException();
    // 获取控制的值
    int c = ctl.get();
    // 判断工作线程数是否小于corePoolSize
    if (workerCountOf(c) < corePoolSize) {
        // 新创建核心线程
        if (addWorker(command, true))
            return;
        c = ctl.get();
    }
    // 工作线程数大于或等于corePoolSize
    // 判断线程池是否处于运行状态，如果是将任务command入队
    if (isRunning(c) && workQueue.offer(command)) {
        int recheck = ctl.get();
        // 再次检查线程池的运行状态，如果不在运行中，那么将任务从队列里面删除，并尝试结束线程池
        if (! isRunning(recheck) && remove(command))
            // 调用驱逐策略
            reject(command);
        // 检查活跃线程总数是否为0
        else if (workerCountOf(recheck) == 0)
            // 新创建非核心线程
            addWorker(null, false);
    }
    // 队列满了，新创建非核心线程
    else if (!addWorker(command, false))
        // 调用驱逐策略
        reject(command);
}
```
该方法是没有返回值的
<a name="cYOmu"></a>
#### `addWorker()` 新创建线程
```java
private boolean addWorker(Runnable firstTask, boolean core) {
    retry:
    for (;;) {
        int c = ctl.get();
        int rs = runStateOf(c);

        // 仅在必要的时候检查队列是否为NULL
        // 检查队列是否处于非运行状态
        if (rs >= SHUTDOWN &&
            ! (rs == SHUTDOWN &&
               firstTask == null &&
               ! workQueue.isEmpty()))
            return false;

        for (;;) {
            // 获取活跃线程数
            int wc = workerCountOf(c);
            // 判断线程是否超过最大值，当队列满了则验证线程数是否大于maximumPoolSize，
            // 没有满则验证corePoolSize
            if (wc >= CAPACITY ||
                wc >= (core ? corePoolSize : maximumPoolSize))
                return false;
            // 增加活跃线程总数，否则重试
            if (compareAndIncrementWorkerCount(c))
                // 如果成功跳出外层循环
                break retry;
            c = ctl.get();  // Re-read ctl
            // 再次校验一下线程池运行状态
            if (runStateOf(c) != rs)
                continue retry;
            // else CAS failed due to workerCount change; retry inner loop
        }
    }

    // 工作线程是否启动
    boolean workerStarted = false;
    // 工作线程是否创建
    boolean workerAdded = false;
    Worker w = null;
    try {
        // 新创建线程
        w = new Worker(firstTask);
        // 获取新创建的线程
        final Thread t = w.thread;
        if (t != null) {
            // 创建线程要获得全局锁
            final ReentrantLock mainLock = this.mainLock;
            mainLock.lock();
            try {
                // Recheck while holding lock.
                // Back out on ThreadFactory failure or if
                // shut down before lock acquired.
                int rs = runStateOf(ctl.get());
                // 检查线程池的运行状态
                if (rs < SHUTDOWN ||
                    (rs == SHUTDOWN && firstTask == null)) {
                    // 检查线程的状态
                    if (t.isAlive()) // precheck that t is startable
                        throw new IllegalThreadStateException();
                    // 将新建工作线程存放到容器
                    workers.add(w);
                    int s = workers.size();
                    if (s > largestPoolSize) {
                        // 跟踪线程池最大的工作线程总数
                        largestPoolSize = s;
                    }
                    workerAdded = true;
                }
            } finally {
                mainLock.unlock();
            }
            // 启动工作线程
            if (workerAdded) {
                t.start();
                workerStarted = true;
            }
        }
    } finally {
        if (! workerStarted)
            // 启动新的工作线程失败，
            // 1\. 将工作线程移除workers容器
            // 2\. 还原工作线程总数（workerCount）
            // 3\. 尝试结束线程
            addWorkerFailed(w);
    }
    return workerStarted;
}
```
如果启动新线程失败那么`addWorkerFailed()`这个方法将做以下三件事：

- 1、将工作线程移除workers容器
- 2、还原工作线程总数（workerCount）
- 3、尝试结束线程
<a name="wKsul"></a>
#### `execute()` 执行过程
![2021-05-07-13-33-09-727760.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620366041087-b065fd01-cb28-403f-ad34-62c8c4abacf9.png#height=725&id=u3be6aa98&originHeight=725&originWidth=823&originalType=binary&ratio=1&size=200043&status=done&style=shadow&width=823)<br />1、如果当前运行的线程少于`corePoolSize`，即使有空闲线程也会创建新线程来执行任务，（注意，执行这一步骤 需要获取全局锁）。如果调用了线程池的`restartAllCoreThreads()`方法， 线程池会提前创建并启动所有基本线程。<br />2、如果运行的线程等于或多于`corePoolSize`，则将任务加入`BlockingQueue`。<br />3、如果无法将任务加入`BlockingQueue`（队列已满），则创建新的线程来处理任务（注意，执 行这一步骤需要获取全局锁）。<br />4、如果创建新线程将使当前运行的线程超出`maximumPoolSize`，任务将被拒绝，并调用 `RejectedExecutionHandler.rejectedExecution()`方法。
<a name="mSRMf"></a>
### 线程任务的执行
线程的正在执行是`ThreadPoolExecutor.Worker#run()`方法，但是这个方法直接委托给了外部的`runWorker()`方法，源码如下：
```java
// 直接委托给外部runworker方法
public void run() {
    runWorker(this);
}
```
<a name="DN7Mv"></a>
#### `runWorker()` 执行任务
```java
final void runWorker(Worker w) {
    // 当前Work中的工作线程
    Thread wt = Thread.currentThread();
    // 获取初始任务
    Runnable task = w.firstTask;
    // 初始任务置NULL(表示不是建线程)
    w.firstTask = null;
    // 修改锁的状态，使需发起中断的线程可以获取到锁（使工作线程可以响应中断）
    w.unlock(); // allow interrupts
    // 工作线程是否是异常结束
    boolean completedAbruptly = true;
    try {
        // 循环的从队列里面获取任务
        while (task != null || (task = getTask()) != null) {
            // 每次执行任务时需要获取到内置的互斥锁
            w.lock();
            // 1\. 当前工作线程不是中断状态，且线程池是STOP,TIDYING,TERMINATED状态，需要中断当前工作线程
            // 2\. 当前工作线程是中断状态，且线程池是STOP,TIDYING,TERMINATED状态，需要中断当前工作线程
            if ((runStateAtLeast(ctl.get(), STOP) || (Thread.interrupted() && runStateAtLeast(ctl.get(), STOP)))
                    && !wt.isInterrupted())
                // 中断线程，中断标志位设置成true
                wt.interrupt();
            try {
                // 执行任务前置方法,扩展用
                beforeExecute(wt, task);
                Throwable thrown = null;
                try {
                    // 执行任务
                    task.run();
                } catch (RuntimeException x) {
                    thrown = x; throw x;
                } catch (Error x) {
                    thrown = x; throw x;
                } catch (Throwable x) {
                    thrown = x; throw new Error(x);
                } finally {
                    // 执行任务后置方法,扩展用
                    afterExecute(task, thrown);
                }
            } finally {
                // 任务NULL表示已经处理了
                task = null;
                w.completedTasks++;
                w.unlock();
            }
        }
        completedAbruptly = false;
    } finally {
        // 将工作线程从容器中剔除
        processWorkerExit(w, completedAbruptly);
    }
}
```
正在执行线程的方法，执行流程：<br />1、获取到当前的工作线程<br />2、获取初始化的线程任务<br />3、修改锁的状态，使工作线程可以响应中断<br />4、获取工作线程的锁（保证在任务执行过程中工作线程不被外部线程中断），如果获取到的任务是NULL，则结束当前工作线程<br />5、判断先测试状态，看是否需要中断当前工作线程<br />6、执行任务前置方法`beforeExecute(wt, task);`<br />7、执行任务(执行提交到线程池的线程)`task.run();`<br />8、执行任务后置方法`afterExecute(task, thrown);`，处理异常信息<br />9、修改完成任务的总数<br />10、解除当前工作线程的锁<br />11、获取队列里面的任务，循环第4步<br />12、将工作线程从容器中剔除<br />`wt.isInterrupted()`：获取中断状态，无副作用<br />`Thread.interrupted()`：获取中断状态，并将中断状态恢重置成false(不中断)<br />`beforeExecute(wt, task)`：执行任务前置方法，扩展用。如果这个方法在执行过程中抛出异常，那么会导致当前工作线程直接死亡而被回收，工作线程异常结束标记位`completedAbruptly`被设置成true，任务线程不能被执行<br />`task.run()`：执行任务<br />`afterExecute(task, thrown)`：执行任务后置方法，扩展用。这个方法可以收集到任务运行的异常信息，这个方法如果有异常抛出，也会导致当前工作线程直接死亡而被回收，工作线程异常结束标记位`completedAbruptly`被设置成true<br />任务运行过程中的异常信息除了`RuntimeException`以外，其他全部封装成Error，然后被`afterExecute`方法收集<br />`terminated()`这也是一个扩展方法，在线程池结束的时候调用
<a name="hmO8k"></a>
#### `getTask()` 获取任务
```java
private Runnable getTask() {
    // 记录最后一次获取任务是不是超时了
    boolean timedOut = false; // Did the last poll() time out?

    for (;;) {
        int c = ctl.get();
        // 获取线程池状态
        int rs = runStateOf(c);

        // 线程池是停止状态或者状态是关闭并且队列为空
        if (rs >= SHUTDOWN && (rs >= STOP || workQueue.isEmpty())) {
            // 扣减工作线程总数
            decrementWorkerCount();
            return null;
        }
        // 获取工作线程总数
        int wc = workerCountOf(c);

        // 工作线程是否需要剔除
        boolean timed = allowCoreThreadTimeOut || wc > corePoolSize;

        if ((wc > maximumPoolSize || (timed && timedOut))
            && (wc > 1 || workQueue.isEmpty())) {
            // 扣减工作线程总数
            if (compareAndDecrementWorkerCount(c))
                // 剔除工作线程，当返回为NULL的时候，runWorker方法的while循环会结束
                return null;
            continue;
        }

        try {
            Runnable r = timed ?
                workQueue.poll(keepAliveTime, TimeUnit.NANOSECONDS) :
                workQueue.take();
            if (r != null)
                return r;
            timedOut = true;
        } catch (InterruptedException retry) {
            timedOut = false;
        }
    }
}
```
`getTask()` 阻塞或定时获取任务。当该方法返回NULL时，当前工作线程会结束，最后被回收，下面是返回NULL的几种情况：<br />1、当前工作线程总数wc大于`maximumPoolSize`最大工作线程总数。`maximumPoolSize`可能被`setMaximumPoolSize`方法改变。<br />2、当线程池处于停止状态时。<br />3、当线程池处于关闭状态且阻塞队列为空。<br />4、当前工作线程超时等待任务，并且当前工作线程总数wc大于`corePoolSize`或者`allowCoreThreadTimeOut=true`允许核心线程超时被回收，默认是false。
<a name="roOIa"></a>
#### `processWorkerExit()` 工作线程结束
```java
private void processWorkerExit(Worker w, boolean completedAbruptly) {
    // 判断是否是异常情况导致工作线程被回收
    if (completedAbruptly) // If abrupt, then workerCount wasn't adjusted
        // 如果是扣减工作线程总数，如果不是在getTask()方法就已经扣减了
        decrementWorkerCount();

    final ReentrantLock mainLock = this.mainLock;
    mainLock.lock();
    try {
        // 将当前工作线程完成任务的总数加到completedTaskCount标志位上
        completedTaskCount += w.completedTasks;
        // 剔除当前工作线程
        workers.remove(w);
    } finally {
        mainLock.unlock();
    }
    // 尝试结束线程池
    tryTerminate();

    // 判刑是否需要新实例化工程线程
    int c = ctl.get();
    if (runStateLessThan(c, STOP)) {
        if (!completedAbruptly) {
            int min = allowCoreThreadTimeOut ? 0 : corePoolSize;
            if (min == 0 && ! workQueue.isEmpty())
                min = 1;
            if (workerCountOf(c) >= min)
                return; // replacement not needed
        }
        addWorker(null, false);
    }
}
```
剔除线程流程：<br />1、判断是否是异常情况导致工作线程被回收，如果是`workerCount--`<br />2、获取到全局锁<br />3、将当前工作线程完成任务的总数加到`completedTaskCount`标志位上<br />4、剔除工作线程<br />5、解锁<br />6、尝试结束线程池`tryTerminate()`<br />7、判刑是否需要重新实例化工程线程放到workers容器
<a name="zGANN"></a>
### 结束线程池
<a name="dAvgn"></a>
#### `shutdown()` 关闭线程池
```java
public void shutdown() {
    final ReentrantLock mainLock = this.mainLock;
    mainLock.lock();
    try {
        // 检查权限
        checkShutdownAccess();
        // 设置线程池状态为关闭
        advanceRunState(SHUTDOWN);
        // 中断线程
        interruptIdleWorkers();
        // 扩展方法
        onShutdown(); // hook for ScheduledThreadPoolExecutor
    } finally {
        mainLock.unlock();
    }
    // 尝试结束线池
    tryTerminate();
}
```
1、通过遍历工作线程容器`workers`，然后逐个中断工作线程，如果无法响应中断的任务可能永远无法终止。<br />2、`shutdown`只是将线程池的状态设置成`SHUTDOWN`状态，然后中断所有没有正在执行任务的线程。
<a name="WIkIg"></a>
#### `shutdownNow()` 关闭线程池
```java
public List<Runnable> shutdownNow() {
    List<Runnable> tasks;
    final ReentrantLock mainLock = this.mainLock;
    mainLock.lock();
    try {
        // 检查权限
        checkShutdownAccess();
        // 设置线程池状态为停止状态
        advanceRunState(STOP);
        // 中断线程
        interruptIdleWorkers();
        // 将所有任务移动到list容器
        tasks = drainQueue();
    } finally {
        mainLock.unlock();
    }
    // 尝试结束线池
    tryTerminate();
    // 返回所有未执行的任务
    return tasks;
}
```
1、通过遍历工作线程容器`workers`，然后逐个中断工作线程，如果无法响应中断的任务可能永远无法终止。<br />2、`shutdownNow`首先将线程池的状态设置成 STOP，然后尝试停止所有的正在执行或暂停任务的线程，并返回等待执行任务的列表。
<a name="Co4VM"></a>
#### `tryTerminate()` 尝试结束线程池
```java
final void tryTerminate() {
    for (;;) {
        int c = ctl.get();
        //  判断是否在运行中,如果是直接返回
        if (isRunning(c) ||
            // 判断是否进入整理状态，如果进入了直接返回
            runStateAtLeast(c, TIDYING) ||
            // 如果是状态是关闭并且队列非空，也直接返回（关闭状态需要等到队列里面的线程处理完）
            (runStateOf(c) == SHUTDOWN && ! workQueue.isEmpty()))
            return;
        // 判断工作线程是否都关闭了
        if (workerCountOf(c) != 0) { // Eligible to terminate
            // 中断空闲线程
            interruptIdleWorkers(ONLY_ONE);
            return;
        }

        final ReentrantLock mainLock = this.mainLock;
        mainLock.lock();
        try {
            // 将状态替换成整理状态
            if (ctl.compareAndSet(c, ctlOf(TIDYING, 0))) {
                try {
                    // 整理发放执行
                    terminated();
                } finally {
                    // 状态替换成结束状态
                    ctl.set(ctlOf(TERMINATED, 0));
                    termination.signalAll();
                }
                return;
            }
        } finally {
            mainLock.unlock();
        }
        // else retry on failed CAS
    }
}
```
结束线程池大致流程为：<br />1、判断是否在运行中，如果是则不结束线程<br />2、判断是否进入整理状态，如果是也不用执行后面内容了<br />3、判断如果线程池是关闭状态并且队列非空，则不结束线程池（关闭状态需要等到队列里面的线程处理完）<br />4、判断工作线程是否都关闭了，如果没有就发起中断工作线程的请求<br />5、获取全局锁将线程池状态替换成整理状态<br />6、调用`terminated();`扩展方法（这也是一个扩展方法，在线程池结束的时候调用）<br />7、将线程池状态替换成结束状态<br />8、解除全局锁
<a name="ZwUjT"></a>
##### 注意：

- 1、可以通过的`shutdown`或`shutdownNow`方法来结束线程池。他们都是通过遍历工作线程容器，然后逐个中断工作线程，所以无法响应中断的任务 可能永远无法终止。
- 2、`shutdown`和`shutdownNow`的区别在于：`shutdownNow`首先将线程池的状态设置成 `STOP`，然后尝试停止所有的正在执行或暂停任务的线程，并返回等待执行任务的列表；而 `shutdown`只是将线程池的状态设置成`SHUTDOWN`状态，然后中断所有没有正在执行任务的线程。
- 3、只要调用了`shutdown`和`shutdownNow`那么`isShutdown`方法就会返回`true`。
- 4、当所有的任务都已关闭后，才表示线程池关闭成功，这时调用`isTerminaed`方法会返回`true`。
<a name="yIefu"></a>
### 线程池的监控
通过扩展线程池进行监控。可以通过继承线程池来自定义线程池，重写线程池的 `beforeExecute`、`afterExecute`和`terminated`方法，也可以在任务执行前、执行后和线程池关闭前执 行一些代码来进行监控。例如，监控任务的平均执行时间、最大执行时间和最小执行时间等。这几个方法在线程池里是空方法。
<a name="SXTAP"></a>
#### `getTaskCount()`
```java
public long getTaskCount() {
    final ReentrantLock mainLock = this.mainLock;
    mainLock.lock();
    try {
        long n = completedTaskCount;
        for (Worker w : workers) {
            n += w.completedTasks;
            if (w.isLocked())
                ++n;
        }
        return n + workQueue.size();
    } finally {
        mainLock.unlock();
    }
}
```
获取线程池需要执行的任务数量。总数=已经结束线工作程完成的任务数(completedTaskCount) + 还未结束线程工作线程完成的任务数(`w.completedTasks`)+正在执行的任务数(`w.isLocked()`)+还未执行的任务数(`workQueue.size()`)
<a name="Snt0T"></a>
#### `getCompletedTaskCount()`
```java
public long getCompletedTaskCount() {
    final ReentrantLock mainLock = this.mainLock;
    mainLock.lock();
    try {
        long n = completedTaskCount;
        for (Worker w : workers)
            n += w.completedTasks;
        return n;
    } finally {
        mainLock.unlock();
    }
}
```
获取线程池在运行过程中已完成的任务数量。总数=已经结束线工作程完成的任务数(`completedTaskCount`) + 还未结束线程工作线程完成的任务数(`w.completedTasks`)
<a name="QQfQl"></a>
#### `getLargestPoolSize()`
```java
public int getLargestPoolSize() {
    final ReentrantLock mainLock = this.mainLock;
    mainLock.lock();
    try {
        return largestPoolSize;
    } finally {
        mainLock.unlock();
    }
}
```
获取线程池里曾经创建过的最大线程数量。通过这个数据可以知道线程池是 否曾经满过。如该数值等于线程池的最大大小，则表示线程池曾经满过。
<a name="n0qFt"></a>
#### `getPoolSize()`
```java
public int getPoolSize() {
    final ReentrantLock mainLock = this.mainLock;
    mainLock.lock();
    try {
        // Remove rare and surprising possibility of
        // isTerminated() && getPoolSize() > 0
        return runStateAtLeast(ctl.get(), TIDYING) ? 0
            : workers.size();
    } finally {
        mainLock.unlock();
    }
}
```
获取线程池的线程数量。如果线程池不销毁的话，线程池里的线程不会自动销 毁，所以这个大小只增不减。
<a name="NiojT"></a>
#### `getActiveCount()`
```java
public int getActiveCount() {
    final ReentrantLock mainLock = this.mainLock;
    mainLock.lock();
    try {
        int n = 0;
        for (Worker w : workers)
            if (w.isLocked())
                ++n;
        return n;
    } finally {
        mainLock.unlock();
    }
}
```

获取活动的线程数。
<a name="QHh4N"></a>
### 合理地配置线程池
要想合理地配置线程池，就必须首先分析任务特性，可以从以下几个角度来分析。

- 任务的性质：CPU密集型任务、IO密集型任务和混合型任务。
- 任务的优先级：高、中和低。
- 任务的执行时间：长、中和短。
- 任务的依赖性：是否依赖其他系统资源，如数据库连接。

性质不同的任务可以用不同规模的线程池分开处理。CPU密集型任务应配置尽可能小的 线程，如配置Ncpu+1个线程的线程池。由于IO密集型任务线程并不是一直在执行任务，则应配 置尽可能多的线程，如2*Ncpu。混合型的任务，如果可以拆分，将其拆分成一个CPU密集型任务 和一个IO密集型任务，只要这两个任务执行的时间相差不是太大，那么分解后执行的吞吐量 将高于串行执行的吞吐量。如果这两个任务执行时间相差太大，则没必要进行分解。<br />优先级不同的任务可以使用优先级队列PriorityBlockingQueue来处理。它可以让优先级高 的任务先执行。

- 如果一直有优先级高的任务提交到队列里，那么优先级低的任务可能永远不能 执行。
- 可以通过 `Runtime.getRuntime().availableProcessors()`方法获得当前设备的CPU个数。
- 建议使用有界队列。有界队列能增加系统的稳定性和预警能力，可以根据需要设大一点 儿，比如几千。无界队列在某些异常情况下可能会撑爆内存。

N核服务器，通过执行业务的单线程分析出本地计算时间为x，等待时间为y，则工作线程数（线程池线程数）设置为 N*(x+y)/x，能让CPU的利用率最大化。
