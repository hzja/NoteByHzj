Java线程池
<a name="v4iAd"></a>
## 什么是线程？
线程是指进程中的一个执行流程，一个进程中可以运行多个线程。比如 java.exe 进程中可以运行很多线程。线程总是属于某个进程，线程没有自己的虚拟地址空间，与进程内的其他线程一起共享分配给该进程的所有资源。<br />线程在执行过程中与进程是有区别的。每个独立的线程有一个程序运行的入口、顺序执行序列和程序的出口。但是线程不能够独立执行，必须依存在应用程序中，由应用程序提供多个线程执行控制。<br />线程是进程的一个实体，是 CPU 调度和分派的基本单位，它是比进程更小的能独立运行的基本单位。线程自己基本上不拥有系统资源，只拥有一点在运行中必不可少的资源(如程序计数器,一组寄存器和栈)，但是它可与同属一个进程的其他的线程共享进程所拥有的全部资源。<br />在 JAVA 中，线程是 java.lang.Thread 类的一个实例。
<a name="UDeI5"></a>
## JAVA 中线程创建的方式？

1. 继承 Thread，并重写 run 方法。
2. 实现 Runnable，并重写 run 方法。
3. 实现 Callable 接口。
<a name="kkx6X"></a>
## 什么是线程池？为什么要使用线程池？
线程池，顾名思义，存放线程的池子。线程是比较稀缺的资源，被无限创建的话，会大量消耗系统资源，降低系统的稳定性。JAVA 中的线程池，可以对线程进行统一的管理。<br />在并发环境下，系统不能够确定在任意时刻中，执行多少任务，投入多少资源。这种不确定性可能会带来一些问题：

1. 频繁的创建，删除线程，会带来一定的性能损耗。
2. 线程无限创建，可能带来资源耗尽的风险。
3. 线程随意的创建，删除。可能会导致系统的不稳定。

线程池解决的核心问题就是资源管理问题。使用线程池的优点：

1. 降低资源损耗。通过复用已经创建的线程，来降低线程创建销毁带来的消耗。
2. 提高响应速度。当接到任务时，减免了原本需要创建线程所消耗的时间。
3. 提高了线程的可管理性。线程由线程池统一管理，调度。合理的使用线程池，能够避免线程的随意创建销毁，增加系统稳定性，控制线程数量，也可以避免资源耗尽的风险。
<a name="sbS80"></a>
## JAVA 中线程池的继承关系图
`Executor` 接口是线程框架最基础的部分，该接口只定义一个用于执行 Runnable 的 `execute` 方法，用来分离任务的提交和任务的执行。<br />`ExecutorService` 接口继承了 Executor，在其基础上做了 `submit()`，`shutdown()`， `invokeAll()` 等方法的扩展，算是真正意义上的线程池接口。<br />`AbstractExecutorService` 抽象类实现了 `ExecutorService` 中的部分方法，如部分的 `submit()`， `invokeAll()`方法。<br />`ThreadPoolExecutor` 是线程池的核心实现类，用来执行被提交的任务。<br />`ScheduledExecutorService` 接口继承了 `ExecutorService` 接口，提供了一些延时执行的方法。<br />`ScheduledThreadPoolExecutor` 是一个实现类，可以在给定一个延时后运行命令，或者定时执行命令。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1694569390817-1f8ad753-c94c-4864-b897-d261c93718ef.jpeg#averageHue=%232c2c2c&clientId=u14e47f4d-eeba-4&from=paste&id=ue91b8d3b&originHeight=383&originWidth=822&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua0254c03-da1a-47ea-a654-39c273a6eb4&title=)<br />从继承图可以看出，`ExecutorService` 是线程池的一个比较核心的接口类。该类里面继承了和定义了一些具体的方法：

1. `void execute(Runnable command);` 执行 Runnable 类型的任务。
2. `Future<?> submit(Runnable/Callable task);` 用来提交 Runnable/Callable 任务，并返回该任务的 Future 对象。
3. `void shutdown();` 在完成已经提交的任务后，关闭。不再接收新的任务。
4. `List shutdownNow();` 停止所有正在执行的任务，同时不再接收新的任务。
5. `boolean isTerminated();` 判断是否所有的任务都已经执行完成了。
6. `boolean isShutdown();` 判断 ExecutorService 是否被关闭。
7. `List<Future> invokeAll(Collection<? extends Callable> tasks);` 执行指定的任务集合，执行完成后返回结果。
8. `T invokeAny(Collection<? extends Callable> tasks);` 执行指定的任务集合，任意一个任务执行完成，返回结果，其他任务终止。
<a name="g7Pil"></a>
## 线程池核心实现类之一：`ThreadPoolExecutor`
```java
public ThreadPoolExecutor(int corePoolSize,
  int maximumPoolSize,
  long keepAliveTime,
  TimeUnit unit,
  BlockingQueue<Runnable> workQueue,
  ThreadFactory threadFactory,
  RejectedExecutionHandler handler) {
    if (corePoolSize < 0 || maximumPoolSize <= 0 || maximumPoolSize < corePoolSize || keepAliveTime < 0)
      throw new IllegalArgumentException();
    if (workQueue == null || threadFactory == null || handler == null) throw new NullPointerException();
    this.acc = System.getSecurityManager() == null ? null : AccessController.getContext();
    this.corePoolSize = corePoolSize;
    this.maximumPoolSize = maximumPoolSize;
    this.workQueue = workQueue;
    this.keepAliveTime = unit.toNanos(keepAliveTime);
    this.threadFactory = threadFactory;
    this.handler = handler;
}
```
线程池创建的各个参数的意义：
<a name="GWZzb"></a>
### `corePoolSize`：线程池的核心线程数量。
提交一个任务，就会创建一个线程，直到线程数量达到 `corePoolSize`。<br />如果线程数量等于 `corePoolSize`，继续提交任务，则会被保存在线程阻塞队列里，等待执行。<br />执行线程池的 `prestartAllCoreThreads()`方法，线程池会提前创建并启用所有的核心线程。
<a name="JKdpr"></a>
### `maximumPoolSize`：线程池最大线程数量。
在线程阻塞队列满的时候，继续提交任务，则会创建新的线程执行任务，线程最大的数量不可超过 `maximumPoolSize`。
<a name="onggK"></a>
### `keepAliveTime`：线程存活时间。
在没有任务需要执行的时候，线程存活的时间。该参数在线程数量大于 `corePoolSize` 的时候使用。
<a name="wFE18"></a>
### `unit`：`keepAliveTime` 时间单位。
<a name="WOe3L"></a>
### `workQueue`：线程阻塞队列。
当正在执行任务的线程数量等于 `corePoolSize` 的时候，继续提交任务，任务则会进入 `workQueue` 等待被执行。<br />一般情况下，workQueue 都是有限队列，如果 `workQueue` 是无限队列，可能会对系统带来一定的影响：

1. 如果是无限队列，当线程数达到 `corePoolSize` 时，新任务就会一直进入 `workQueue`。`maximumPoolSize` 参数就会无效，同时 `keepAliveTime` 也会变成无效参数。
2. 如果是无限队列，也有可能出现资源耗尽的情况。

常用的 `workQueue`：`ArrayBlockingQueue`，`LinkedBlockingQueue` 等
<a name="CvR4n"></a>
### `threadFactory`：线程工厂。
自己设置自定义线程工厂，`Executors` 的默认线程工厂 `DefaultThreadFactory`
```java
static class DefaultThreadFactory implements ThreadFactory {
    private static final AtomicInteger poolNumber = new AtomicInteger(1);
    private final ThreadGroup group;
    private final AtomicInteger threadNumber = new AtomicInteger(1);
    private final String namePrefix;

    DefaultThreadFactory() {
        SecurityManager s = System.getSecurityManager();
        group = (s != null) ? s.getThreadGroup() : Thread.currentThread().getThreadGroup();
        namePrefix = "pool-" + poolNumber.getAndIncrement() + "-thread-";
    }

    public Thread newThread(Runnable r) {
        Thread t = new Thread(group, r, namePrefix + threadNumber.getAndIncrement(), 0);
        if (t.isDaemon()) t.setDaemon(false);
        if (t.getPriority() != Thread.NORM_PRIORITY) t.setPriority(Thread.NORM_PRIORITY);
        return t;
    }
}
```
可以看出默认线程工厂的线程命名规则是：pool-''数字''-thread-"数字"
<a name="lye1n"></a>
### `executionHandler`：拒绝策略。
当线程阻塞队列已满，线程数量也已经到 `maximumPoolSize` 时，如果继续提交任务，就必须找一种方法处理现在的情况。就有了拒绝策略。线程池提供了四种拒绝策略供大家选择：
<a name="nIaTH"></a>
#### `AbortPolicy`：直接抛出异常，也是线程池的默认拒绝策略。
```java
public static class AbortPolicy implements RejectedExecutionHandler {
    /**
     * Creates an {@code AbortPolicy}.
     */
    public AbortPolicy() { }

    /**
     * Always throws RejectedExecutionException.
     *
     * @param r the runnable task requested to be executed
     * @param e the executor attempting to execute this task
     * @throws RejectedExecutionException always
     */
    public void rejectedExecution(Runnable r, ThreadPoolExecutor e) {
        throw new RejectedExecutionException("Task " + r.toString() + " rejected from " + e.toString());
    }
}
```
<a name="ne6Ro"></a>
#### `CallerRunsPolicy`：用调用者所在的当前线程执行。
一般使用在不允许失败，对性要求不高，并发不算太高的场景。如果并发很高，则会造成程序阻塞，性能效率损失很大。
```java
public static class CallerRunsPolicy implements RejectedExecutionHandler {
    /**
     * Creates a {@code CallerRunsPolicy}.
     */
    public CallerRunsPolicy() { }

    /**
     * Executes task r in the caller's thread, unless the executor
     * has been shut down, in which case the task is discarded.
     *
     * @param r the runnable task requested to be executed
     * @param e the executor attempting to execute this task
     */
    public void rejectedExecution(Runnable r, ThreadPoolExecutor e) {
        if (!e.isShutdown()) {
            r.run();
        }
    }
}
```
<a name="JTkTV"></a>
#### `DiscardOldestPolicy`：丢弃阻塞队列里面最前面的任务，并开始执行当前任务。
```java
public static class DiscardOldestPolicy implements RejectedExecutionHandler {
    /**
     * Creates a {@code DiscardOldestPolicy} for the given executor.
     */
    public DiscardOldestPolicy() { }

    /**
     * Obtains and ignores the next task that the executor
     * would otherwise execute, if one is immediately available,
     * and then retries execution of task r, unless the executor
     * is shut down, in which case task r is instead discarded.
     *
     * @param r the runnable task requested to be executed
     * @param e the executor attempting to execute this task
     */
    public void rejectedExecution(Runnable r, ThreadPoolExecutor e) {
        if (!e.isShutdown()) {
            e.getQueue().poll();
            e.execute(r);
        }
    }
}
```
<a name="PpPNU"></a>
#### `DiscardPolicy`：直接丢弃任务。
```java
public static class DiscardPolicy implements RejectedExecutionHandler {
        /**
         * Creates a {@code DiscardPolicy}.
         */
        public DiscardPolicy() { }

        /**
         * Does nothing, which has the effect of discarding task r.
         *
         * @param r the runnable task requested to be executed
         * @param e the executor attempting to execute this task
         */
        public void rejectedExecution(Runnable r, ThreadPoolExecutor e) {
        }
    }
```
<a name="vUQe3"></a>
## 线程池的重要属性
```java
private final AtomicInteger ctl = new AtomicInteger(ctlOf(RUNNING, 0));
private static final int COUNT_BITS = Integer.SIZE - 3;
private static final int CAPACITY   = (1 << COUNT_BITS) - 1;
```
<a name="MerC2"></a>
### ctl
是对线程池的运行状态，线程池中有效线程数量进行控制的一个字段，ctl 包含两部分信息：线程池的运行状态（runState），线程池内有效线程数量（`workerCount`）；这两个信息用一个 Integer 类型来保存，runState 用高三位保存，workerCount 用低 29 位保存。COUNT_BITS 就是 29，CAPACITY 是 1 左移 29 位再减去 1（二进制就是 29 个 1），这个常量表示 workerCount 的上限值 536870911。<br />线程池的五种状态
```java
private static final int RUNNING    = -1 << COUNT_BITS;
private static final int SHUTDOWN   =  0 << COUNT_BITS;
private static final int STOP       =  1 << COUNT_BITS;
private static final int TIDYING    =  2 << COUNT_BITS;
private static final int TERMINATED =  3 << COUNT_BITS;
```

1. RUNNING ：线程池的初始状态，线程池被创建就是 RUNNING 状态，且线程池的任务为 0。线程池处于 RUNNING 状态，能接收新任务，并且能够已添加的任务做处理。
2. SHUTDOWN：调用线程池的 `shutdown()` 接口，线程池的状态会从 RUNNING -> SHUTDOWN。线程池处于 SUTDOWN 状态时，线程池不再接收新任务，可以继续完成已经收到的任务。
3. STOP：调用线程池的 `shutdownNow()` 接口，线程池状态会从 RUNNING/SHUTDOWN -> STOP 。线程池处于 STOP 状态时，线程池不再接收新任务，也不会继续完成已经收到的任务，并中断正在执行的任务。
4. TIDYING：当线程池在 SHUTDOWN 状态下，阻塞队列为空并且线程池中执行的任务也为空时，就会由 SHUTDOWN -> TIDYING；当线程池在 STOP 状态下，线程池中执行的任务为空时，就会由 STOP -> TIDYING。当所有的任务已终止，ctl 记录的”任务数量”为 0，线程池会变为 TIDYING 状态。当线程池变为 TIDYING 状态时，会执行钩子函数 `terminated()`。`terminated()` 在`ThreadPoolExecutor` 类中是空的，若用户想在线程池变为 TIDYING 时，进行相应的处理；可以通过重载 `terminated()` 函数来实现。
5. TERMINATED：线程池在 TIDYING 状态下，执行过 terminated() 之后，从 TIDYING -> TERMINATED 状态。线程池彻底终止就变成了 TERMINATED 状态。进入 TERMINATED 状态的条件：(1) 线程池不在 RUNNING；(2) 线程池状态不在 TIDYING/TERMINATED；(3) 线程池的状态是 SHUTDOWN，并且阻塞队列为空时。(4) workerCount=0；(5) 设置 TIDYING 成功。

![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1694569390790-d8a572dc-cf74-41fe-86cb-12eeba3a9068.jpeg#averageHue=%23fafafa&clientId=u14e47f4d-eeba-4&from=paste&id=uafa60cbc&originHeight=725&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf29d4abc-ce81-4951-9de8-320353cb4f7&title=)
<a name="ZzuFc"></a>
## 线程池的工作原理

1. 如果当前线程数量少于 `corePoolSize` 时，新来任务时会获取全局锁创建新的线程；
2. 当前线程数量等于或多余 `corePoolSize` 时，新来任务时则会被加到 `BlockingQueue`；
3. 如果 `BlockingQueue` 已经满了，则会创建新的线程来处理任务；
4. 新创建线程时，如果线程总数即将超过 `maximumPoolSize`，则当前任务则会被拒绝。

![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1694569390751-c072f783-6dec-467b-aa2d-fa1edf90c7e7.jpeg#averageHue=%23ebeae5&clientId=u14e47f4d-eeba-4&from=paste&id=u550d806c&originHeight=406&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua10ff5dc-5e89-4031-ae41-5113cfd68e8&title=)
<a name="y13Fc"></a>
## 学习总结
主要以线程为切入点，并抛砖引玉以提出问题的方式，去学习 JAVA 中 Executor 接口的继承关系，了解到了线程池的核心实现类 ThreadPoolExecutor 的所有参数都是什么含义，学习了四种线程拒绝策略，在不同的业务场景中该如何使用。<br />通过线程池的一些重要属性学习到了线程池的状态流转机制。进一步了解了线程池的工作原理，真正的知道了线程池的优缺点，以及在什么业务场景下再去使用线程池。
