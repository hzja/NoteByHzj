JavaThreadPoolExecutor<br />Java中的线程池是运用场景最多的并发框架，几乎所有需要异步或并发执行任务的程序 都可以使用线程池。合理地使用线程池能够带来3个好处：

1. 降低资源消耗。通过重复利用已创建的线程降低线程创建和销毁造成的消耗。
2. 提高响应速度。当任务到达时，任务可以不需要等到线程创建就能立即执行。
3. 提高线程的可管理性。线程是稀缺资源，如果无限制地创建，不仅会消耗系统资源， 还会降低系统的稳定性，使用线程池可以进行统一分配、调优和监控。
<a name="QfGgV"></a>
## 线程池的主要处理流程
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679142080094-ed4b638e-53a3-4871-8c4f-bf17901b7e29.png#averageHue=%23f8f9f6&clientId=u4b9a6e7e-f012-4&from=paste&id=ud8d95aaa&originHeight=636&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2d57ab2a-82ac-40c8-8c04-3976ec7c642&title=)
<a name="rveSz"></a>
## `ThreadPoolExecutor` 类图
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679142080196-78e5429e-9464-4667-850a-7faa35cacc2d.png#averageHue=%23383632&clientId=u4b9a6e7e-f012-4&from=paste&id=u0f386420&originHeight=531&originWidth=1020&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u568017c8-75fa-4c0a-86a0-169422e980d&title=)<br />java中的线程池都是基于`ThreadPoolExecutor` 来实现的。
<a name="ruHwI"></a>
## 核心属性
```java
// 状态控制属性：高3位表示线程池的运行状态，剩下的29位表示当前有效的线程数量
private final AtomicInteger ctl = new AtomicInteger(ctlOf(RUNNING, 0));

// 线程池的基本大小，当提交一个任务到线程池时，线程池会创建一个线程来执行任务，
// 即使其他空闲的基本线程能够执行新任务也会创建线程，等到需要执行的任务数大于
// 线程池基本大小时就不再创建。如果调用了线程池的prestartAllCoreThreads()方法，
// 线程池会提前创建并启动所有基本线程。
private volatile int corePoolSize;

// 线程池线程最大数量，如果队列满了，并且已创建的线程数小于最大线程数，
// 则线程池会再创建新的线程执行任务。如果使用了无界的任务队列这个参数就没什么效果。
private volatile int maximumPoolSize;

// 用于设置创建线程的工厂，可以通过线程工厂给每个创建出来的线程设 置更有意义的名字。
private volatile ThreadFactory threadFactory;

// 饱和策略，默认情况下是AbortPolicy。
private volatile RejectedExecutionHandler handler;

// 线程池的工作线程空闲后，保持存活的时间。如果任务很多，并且每个任务执行的时间比较短，
// 可以调大时间，提高线程的利用率。
private volatile long keepAliveTime;

// 用于保存等待执行的任务的阻塞队列，具体可以参考[JAVA并发容器-阻塞队列](https://www.jianshu.com/p/5646fb5faee1)
private final BlockingQueue<Runnable> workQueue;

// 存放工作线程的容器，必须获取到锁才能访问
private final HashSet<Worker> workers = new HashSet<Worker>();

// ctl的拆包和包装
private static int runStateOf(int c)     { return c & ~CAPACITY; }
private static int workerCountOf(int c)  { return c & CAPACITY; }
private static int ctlOf(int rs, int wc) { return rs | wc; }
```

- 阻塞队列可以参考JAVA并发容器-阻塞队列。
- ctl状态控制属性，高3位表示线程池的运行状态（`runState`），剩下的29位表示当前有效的线程数量（`workerCount`）
- 线程池最大线程数是(1 << COUNT_BITS) - 1 = 536 870 911
<a name="bKhyW"></a>
### 线程池的运行状态runState
| 状态 | 解释 |
| --- | --- |
| RUNNING | 运行态，可处理新任务并执行队列中的任务 |
| SHUTDOW | 关闭态，不接受新任务，但处理队列中的任务 |
| STOP | 停止态，不接受新任务，不处理队列中任务，且打断运行中任务 |
| TIDYING | 整理态，所有任务已经结束，`workerCount = 0` ，将执行`terminated()`方法 |
| TERMINATED | 结束态，`terminated()` 方法已完成 |
| ![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679142080100-9c8814c4-1ee4-45e0-b366-a2e2e590f182.png#averageHue=%23f5f4ef&clientId=u4b9a6e7e-f012-4&from=paste&id=u9d61c073&originHeight=583&originWidth=507&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub8ee2365-9328-4b13-8d4c-39a7fb34ca4&title=) | 

 |

<a name="J7D4j"></a>
### `RejectedExecutionHandler`（拒绝策略）

- `AbortPolicy`：直接抛出异常。
- `CallerRunsPolicy`：只用调用者所在线程来运行任务。
- `DiscardOldestPolicy`：丢弃队列里最近的一个任务，并执行当前任务。
- `DiscardPolicy`：不处理，丢弃掉。
<a name="JSGQy"></a>
## 核心内部类 Worker
```java
private final class Worker  extends AbstractQueuedSynchronizer  implements Runnable {
    // 正在执行任务的线程
    final Thread thread;
    // 线程创建时初始化的任务
    Runnable firstTask;
    // 完成任务计数器
    volatile long completedTasks;

    Worker(Runnable firstTask) {
        // 在runWorker方法运行之前禁止中断，要中断线程必须先获取worker内部的互斥锁
        setState(-1); // inhibit interrupts until runWorker
        this.firstTask = firstTask;
        this.thread = getThreadFactory().newThread(this);
    }

    /** delegates main run loop to outer runworker  */
    // 直接委托给外部runworker方法
    public void run() {
        runWorker(this);
    }
    ...
}
```
Worker 类他将执行任务的线程封装到了内部，在初始化Worker 的时候，会调用`ThreadFactory`初始化新线程；Worker 继承了`AbstractQueuedSynchronizer`，在内部实现了一个互斥锁，主要目的是控制工作线程的中断状态。<br />线程的中断一般是由其他线程发起的，比如`ThreadPoolExecutor#interruptIdleWorkers(boolean)`方法，它在调用过程中会去中断worker内部的工作线程，Work的互斥锁可以保证正在执行的任务不被打断。它是怎么保证的呢？在线程真正执行任务的时候，也就是`runWorker`方法被调用时，它会先获取到Work的锁，当在其他线程需要中断当前线程时也需要获取到work的互斥锁，否则不能中断。
<a name="R1cTC"></a>
## 构造函数
```java
public ThreadPoolExecutor(int corePoolSize,
                          int maximumPoolSize,
                          long keepAliveTime,
                          TimeUnit unit,
                          BlockingQueue<Runnable> workQueue,
                          ThreadFactory threadFactory,
                          RejectedExecutionHandler handler) {
    if (corePoolSize < 0 ||
        maximumPoolSize <= 0 ||
        maximumPoolSize < corePoolSize ||
        keepAliveTime < 0)
        throw new IllegalArgumentException();
    if (workQueue == null || threadFactory == null || handler == null)
        throw new NullPointerException();
    this.corePoolSize = corePoolSize;
    this.maximumPoolSize = maximumPoolSize;
    this.workQueue = workQueue;
    this.keepAliveTime = unit.toNanos(keepAliveTime);
    this.threadFactory = threadFactory;
    this.handler = handler;
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
<a name="jN68e"></a>
### `execute()` 提交线程
```java
public void execute(Runnable command) {
    if (command == null)
        throw new NullPointerException();
    // 获取控制的值
    int c = ctl.get();
    // 判断工作线程数是否小于corePoolSize
    if (workerCountOf(c) < corePoolSize) {
        // 新创建核心线程
        if (addWorker(command, true))
            return;
        c = ctl.get();
    }
    // 工作线程数大于或等于corePoolSize
    // 判断线程池是否处于运行状态，如果是将任务command入队
    if (isRunning(c) && workQueue.offer(command)) {
        int recheck = ctl.get();
        // 再次检查线程池的运行状态，如果不在运行中，那么将任务从队列里面删除，并尝试结束线程池
        if (! isRunning(recheck) && remove(command))
            // 调用驱逐策略
            reject(command);
        // 检查活跃线程总数是否为0
        else if (workerCountOf(recheck) == 0)
            // 新创建非核心线程
            addWorker(null, false);
    }
    // 队列满了，新创建非核心线程
    else if (!addWorker(command, false))
        // 调用驱逐策略
        reject(command);
}
```
该方法是没有返回值的
<a name="GPdZV"></a>
### `addWorker()` 新创建线程
```java
private boolean addWorker(Runnable firstTask, boolean core) {
    retry:
    for (;;) {
        int c = ctl.get();
        int rs = runStateOf(c);

        // 仅在必要的时候检查队列是否为NULL
        // 检查队列是否处于非运行状态
        if (rs >= SHUTDOWN &&
            ! (rs == SHUTDOWN &&
               firstTask == null &&
               ! workQueue.isEmpty()))
            return false;

        for (;;) {
            // 获取活跃线程数
            int wc = workerCountOf(c);
            // 判断线程是否超过最大值，当队列满了则验证线程数是否大于maximumPoolSize，
            // 没有满则验证corePoolSize
            if (wc >= CAPACITY ||
                wc >= (core ? corePoolSize : maximumPoolSize))
                return false;
            // 增加活跃线程总数，否则重试
            if (compareAndIncrementWorkerCount(c))
                // 如果成功跳出外层循环
                break retry;
            c = ctl.get();  // Re-read ctl
            // 再次校验一下线程池运行状态
            if (runStateOf(c) != rs)
                continue retry;
            // else CAS failed due to workerCount change; retry inner loop
        }
    }

    // 工作线程是否启动
    boolean workerStarted = false;
    // 工作线程是否创建
    boolean workerAdded = false;
    Worker w = null;
    try {
        // 新创建线程
        w = new Worker(firstTask);
        // 获取新创建的线程
        final Thread t = w.thread;
        if (t != null) {
            // 创建线程要获得全局锁
            final ReentrantLock mainLock = this.mainLock;
            mainLock.lock();
            try {
                // Recheck while holding lock.
                // Back out on ThreadFactory failure or if
                // shut down before lock acquired.
                int rs = runStateOf(ctl.get());
                // 检查线程池的运行状态
                if (rs < SHUTDOWN ||
                    (rs == SHUTDOWN && firstTask == null)) {
                    // 检查线程的状态
                    if (t.isAlive()) // precheck that t is startable
                        throw new IllegalThreadStateException();
                    // 将新建工作线程存放到容器
                    workers.add(w);
                    int s = workers.size();
                    if (s > largestPoolSize) {
                        // 跟踪线程池最大的工作线程总数
                        largestPoolSize = s;
                    }
                    workerAdded = true;
                }
            } finally {
                mainLock.unlock();
            }
            // 启动工作线程
            if (workerAdded) {
                t.start();
                workerStarted = true;
            }
        }
    } finally {
        if (! workerStarted)
            // 启动新的工作线程失败，
            // 1. 将工作线程移除workers容器
            // 2. 还原工作线程总数（workerCount）
            // 3. 尝试结束线程
            addWorkerFailed(w);
    }
    return workerStarted;
}
```
如果启动新线程失败那么`addWorkerFailed()`这个方法将做一下三件事：

1. 将工作线程移除workers容器
2. 还原工作线程总数（`workerCount`）
3. 尝试结束线程
<a name="zACzd"></a>
### `execute()` 执行过程
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679142080059-0489cab7-03ab-45f3-b4ed-dc53413cfe2b.png#averageHue=%23f2f2f2&clientId=u4b9a6e7e-f012-4&from=paste&id=ued53a219&originHeight=725&originWidth=823&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u25adb3f5-91f6-4bc1-a5b8-ebdf0f0b52e&title=)

1. 如果当前运行的线程少于`corePoolSize`，即使有空闲线程也会创建新线程来执行任务，（注意，执行这一步骤 需要获取全局锁）。如果调用了线程池的`restartAllCoreThreads()`方法， 线程池会提前创建并启动所有基本线程。
2. 如果运行的线程等于或多于`corePoolSize`，则将任务加入`BlockingQueue`。
3. 如果无法将任务加入`BlockingQueue`（队列已满），则创建新的线程来处理任务（注意，执 行这一步骤需要获取全局锁）。
4. 如果创建新线程将使当前运行的线程超出`maximumPoolSize`，任务将被拒绝，并调用 `RejectedExecutionHandler.rejectedExecution()`方法。
<a name="OD33x"></a>
## 线程任务的执行
线程的正在执行是`ThreadPoolExecutor.Worker#run()`方法，但是这个方法直接委托给了外部的`runWorker()`方法，源码如下：
```java
// 直接委托给外部runworker方法
public void run() {
    runWorker(this);
}
```
<a name="ohqX4"></a>
### `runWorker()` 执行任务
```java
final void runWorker(Worker w) {
    // 当前Work中的工作线程
    Thread wt = Thread.currentThread();
    // 获取初始任务
    Runnable task = w.firstTask;
    // 初始任务置NULL(表示不是建线程)
    w.firstTask = null;
    // 修改锁的状态，使需发起中断的线程可以获取到锁（使工作线程可以响应中断）
    w.unlock(); // allow interrupts
    // 工作线程是否是异常结束
    boolean completedAbruptly = true;
    try {
        // 循环的从队列里面获取任务
        while (task != null || (task = getTask()) != null) {
            // 每次执行任务时需要获取到内置的互斥锁
            w.lock();
            // 1. 当前工作线程不是中断状态，且线程池是STOP,TIDYING,TERMINATED状态，需要中断当前工作线程
            // 2. 当前工作线程是中断状态，且线程池是STOP,TIDYING,TERMINATED状态，需要中断当前工作线程
            if ((runStateAtLeast(ctl.get(), STOP) || (Thread.interrupted() && runStateAtLeast(ctl.get(), STOP)))
                    && !wt.isInterrupted())
                // 中断线程，中断标志位设置成true
                wt.interrupt();
            try {
                // 执行任务前置方法,扩展用
                beforeExecute(wt, task);
                Throwable thrown = null;
                try {
                    // 执行任务
                    task.run();
                } catch (RuntimeException x) {
                    thrown = x; throw x;
                } catch (Error x) {
                    thrown = x; throw x;
                } catch (Throwable x) {
                    thrown = x; throw new Error(x);
                } finally {
                    // 执行任务后置方法,扩展用
                    afterExecute(task, thrown);
                }
            } finally {
                // 任务NULL表示已经处理了
                task = null;
                w.completedTasks++;
                w.unlock();
            }
        }
        completedAbruptly = false;
    } finally {
        // 将工作线程从容器中剔除
        processWorkerExit(w, completedAbruptly);
    }
}
```
正在执行线程的方法，执行流程：

1. 获取到当前的工作线程
2. 获取初始化的线程任务
3. 修改锁的状态，使工作线程可以响应中断
4. 获取工作线程的锁（保证在任务执行过程中工作线程不被外部线程中断），如果获取到的任务是NULL，则结束当前工作线程
5. 判断先测试状态，看是否需要中断当前工作线程
6. 执行任务前置方法`beforeExecute(wt, task);`
7. 执行任务(执行提交到线程池的线程)`task.run();`
8. 执行任务后置方法`afterExecute(task, thrown);`，处理异常信息
9. 修改完成任务的总数
10. 解除当前工作线程的锁
11. 获取队列里面的任务，循环第4步
12. 将工作线程从容器中剔除
- `wt.isInterrupted()`：获取中断状态，无副作用
- `Thread.interrupted()`：获取中断状态，并将中断状态恢重置成false(不中断)
- `beforeExecute(wt, task);`：执行任务前置方法，扩展用。如果这个方法在执行过程中抛出异常，那么会导致当前工作线程直接死亡而被回收，工作线程异常结束标记位`completedAbruptly`被设置成true，任务线程不能被执行
- `task.run();`：执行任务
- `afterExecute(task, thrown);`：执行任务后置方法，扩展用。这个方法可以收集到任务运行的异常信息，这个方法如果有异常抛出，也会导致当前工作线程直接死亡而被回收，工作线程异常结束标记位`completedAbruptly`被设置成true
- 任务运行过程中的异常信息除了`RuntimeException`以外，其他全部封装成Error，然后被`afterExecute`方法收集
- `terminated()`这也是一个扩展方法，在线程池结束的时候调用
<a name="vDYu6"></a>
### `getTask()` 获取任务
```java
private Runnable getTask() {
    // 记录最后一次获取任务是不是超时了
    boolean timedOut = false; // Did the last poll() time out?

    for (;;) {
        int c = ctl.get();
        // 获取线程池状态
        int rs = runStateOf(c);

        // 线程池是停止状态或者状态是关闭并且队列为空
        if (rs >= SHUTDOWN && (rs >= STOP || workQueue.isEmpty())) {
            // 扣减工作线程总数
            decrementWorkerCount();
            return null;
        }
        // 获取工作线程总数
        int wc = workerCountOf(c);

        // 工作线程是否需要剔除
        boolean timed = allowCoreThreadTimeOut || wc > corePoolSize;

        if ((wc > maximumPoolSize || (timed && timedOut))
            && (wc > 1 || workQueue.isEmpty())) {
            // 扣减工作线程总数
            if (compareAndDecrementWorkerCount(c))
                // 剔除工作线程，当返回为NULL的时候，runWorker方法的while循环会结束
                return null;
            continue;
        }

        try {
            Runnable r = timed ?
                workQueue.poll(keepAliveTime, TimeUnit.NANOSECONDS) :
                workQueue.take();
            if (r != null)
                return r;
            timedOut = true;
        } catch (InterruptedException retry) {
            timedOut = false;
        }
    }
}
```
`getTask()` 阻塞或定时获取任务。当该方法返回NULL时，当前工作线程会结束，最后被回收，下面是返回NULL的几种情况：

1. 当前工作线程总数wc大于`maximumPoolSize`最大工作线程总数。`maximumPoolSize`可能被`setMaximumPoolSize`方法改变。
2. 当线程池处于停止状态时。
3. 当线程池处于关闭状态且阻塞队列为空。
4. 当前工作线程超时等待任务，并且当前工作线程总数wc大于`corePoolSize`或者`allowCoreThreadTimeOut=true`允许核心线程超时被回收，默认是false。

线程池在运行过程中可以调用`setMaximumPoolSize()`方法来修改`maximumPoolSize`值，新的值必须大于`corePoolSize`，如果新的`maximumPoolSize`小于原来的值，那么在该方法会去中断当前的空闲线程(工作线程内置锁的是解锁状态的线程为空闲线程)。
<a name="jIsaY"></a>
### `processWorkerExit()` 工作线程结束
```java
private void processWorkerExit(Worker w, boolean completedAbruptly) {
    // 判断是否是异常情况导致工作线程被回收
    if (completedAbruptly) // If abrupt, then workerCount wasn't adjusted
        // 如果是扣减工作线程总数，如果不是在getTask()方法就已经扣减了
        decrementWorkerCount();

    final ReentrantLock mainLock = this.mainLock;
    mainLock.lock();
    try {
        // 将当前工作线程完成任务的总数加到completedTaskCount标志位上
        completedTaskCount += w.completedTasks;
        // 剔除当前工作线程
        workers.remove(w);
    } finally {
        mainLock.unlock();
    }
    // 尝试结束线程池
    tryTerminate();

    // 判刑是否需要新实例化工程线程
    int c = ctl.get();
    if (runStateLessThan(c, STOP)) {
        if (!completedAbruptly) {
            int min = allowCoreThreadTimeOut ? 0 : corePoolSize;
            if (min == 0 && ! workQueue.isEmpty())
                min = 1;
            if (workerCountOf(c) >= min)
                return; // replacement not needed
        }
        addWorker(null, false);
    }
}
```
剔除线程流程：

1. 判断是否是异常情况导致工作线程被回收，如果是`workerCount--`
2. 获取到全局锁
3. 将当前工作线程完成任务的总数加到`completedTaskCount`标志位上
4. 剔除工作线程
5. 解锁
6. 尝试结束线程池`tryTerminate()`
7. 判刑是否需要重新实例化工程线程放到`workers`容器
<a name="zeE6c"></a>
## 结束线程池
<a name="puQs4"></a>
### `shutdown()` 关闭线程池
```java
public void shutdown() {
    final ReentrantLock mainLock = this.mainLock;
    mainLock.lock();
    try {
        // 检查权限
        checkShutdownAccess();
        // 设置线程池状态为关闭
        advanceRunState(SHUTDOWN);
        // 中断线程
        interruptIdleWorkers();
        // 扩展方法
        onShutdown(); // hook for ScheduledThreadPoolExecutor
    } finally {
        mainLock.unlock();
    }
    // 尝试结束线池
    tryTerminate();
}
```

- 通过遍历工作线程容器`workers`，然后逐个中断工作线程，如果无法响应中断的任务可能永远无法终止
- `shutdown`只是将线程池的状态设置成`SHUTDOWN`状态，然后中断所有没有正在执行任务的线程。
<a name="Uh2ch"></a>
### `shutdown()` 关闭线程池
```java
public List<Runnable> shutdownNow() {
    List<Runnable> tasks;
    final ReentrantLock mainLock = this.mainLock;
    mainLock.lock();
    try {
        // 检查权限
        checkShutdownAccess();
        // 设置线程池状态为停止状态
        advanceRunState(STOP);
        // 中断线程
        interruptIdleWorkers();
        // 将所有任务移动到list容器
        tasks = drainQueue();
    } finally {
        mainLock.unlock();
    }
    // 尝试结束线池
    tryTerminate();
    // 返回所有未执行的任务
    return tasks;
}
```

- 通过遍历工作线程容器`workers`，然后逐个中断工作线程，如果无法响应中断的任务可能永远无法终止
- `shutdownNow`首先将线程池的状态设置成 `STOP`，然后尝试停止所有的正在执行或暂停任务的线程，并返回等待执行任务的列表
<a name="az7Gu"></a>
### `tryTerminate()` 尝试结束线程池
```java
final void tryTerminate() {
    for (;;) {
        int c = ctl.get();
        //  判断是否在运行中,如果是直接返回
        if (isRunning(c) ||
            // 判断是否进入整理状态，如果进入了直接返回
            runStateAtLeast(c, TIDYING) ||
            // 如果是状态是关闭并且队列非空，也直接返回（关闭状态需要等到队列里面的线程处理完）
            (runStateOf(c) == SHUTDOWN && ! workQueue.isEmpty()))
            return;
        // 判断工作线程是否都关闭了
        if (workerCountOf(c) != 0) { // Eligible to terminate
            // 中断空闲线程
            interruptIdleWorkers(ONLY_ONE);
            return;
        }

        final ReentrantLock mainLock = this.mainLock;
        mainLock.lock();
        try {
            // 将状态替换成整理状态
            if (ctl.compareAndSet(c, ctlOf(TIDYING, 0))) {
                try {
                    // 整理发放执行
                    terminated();
                } finally {
                    // 状态替换成结束状态
                    ctl.set(ctlOf(TERMINATED, 0));
                    termination.signalAll();
                }
                return;
            }
        } finally {
            mainLock.unlock();
        }
        // else retry on failed CAS
    }
}
```
结束线程池大致流程为：

1. 判断是否在运行中，如果是则不结束线程
2. 判断是否进入整理状态，如果是也不用执行后面内容了
3. 判断如果线程池是关闭状态并且队列非空，则不结束线程池（关闭状态需要等到队列里面的线程处理完）
4. 判断工作线程是否都关闭了，如果没有就发起中断工作线程的请求
5. 获取全局锁将线程池状态替换成整理状态
6. 调用`terminated();`扩展方法（这也是一个扩展方法，在线程池结束的时候调用）
7. 将线程池状态替换成结束状态
8. 解除全局锁

**注意：**

- 可以通过的`shutdown`或`shutdownNow`方法来结束线程池。他们都是通过遍历工作线程容器，然后逐个中断工作线程，所以无法响应中断的任务 可能永远无法终止。
- `shutdown`和`shutdownNow`的区别在于：`shutdownNow`首先将线程池的状态设置成 STOP，然后尝试停止所有的正在执行或暂停任务的线程，并返回等待执行任务的列表；而 `shutdown`只是将线程池的状态设置成SHUTDOWN状态，然后中断所有没有正在执行任务的线 程。
- 只要调用了`shutdown`和`shutdownNow`那么`isShutdown`方法就会返回true
- 当所有的任务都已关闭后，才表示线程池关闭成功，这时调用`isTerminaed`方法会返回true
<a name="dubkz"></a>
## 线程池的监控
通过扩展线程池进行监控。可以通过继承线程池来自定义线程池，重写线程池的 `beforeExecute`、`afterExecute`和`terminated`方法，也可以在任务执行前、执行后和线程池关闭前执 行一些代码来进行监控。例如，监控任务的平均执行时间、最大执行时间和最小执行时间等。这几个方法在线程池里是空方法。
<a name="EOQQJ"></a>
### `getTaskCount()`
```java
public long getTaskCount() {
    final ReentrantLock mainLock = this.mainLock;
    mainLock.lock();
    try {
        long n = completedTaskCount;
        for (Worker w : workers) {
            n += w.completedTasks;
            if (w.isLocked())
                ++n;
        }
        return n + workQueue.size();
    } finally {
        mainLock.unlock();
    }
}
```
获取线程池需要执行的任务数量。总数=已经结束线工作程完成的任务数(`completedTaskCount`) + 还未结束线程工作线程完成的任务数(`w.completedTasks`)+正在执行的任务数(`w.isLocked()`)+还未执行的任务数(`workQueue.size()`)
<a name="pnURB"></a>
### `getCompletedTaskCount()`
```java
public long getCompletedTaskCount() {
    final ReentrantLock mainLock = this.mainLock;
    mainLock.lock();
    try {
        long n = completedTaskCount;
        for (Worker w : workers)
            n += w.completedTasks;
        return n;
    } finally {
        mainLock.unlock();
    }
}
```
获取线程池在运行过程中已完成的任务数量。总数=已经结束线工作程完成的任务数(`completedTaskCount`) + 还未结束线程工作线程完成的任务数(`w.completedTasks`)
<a name="OTJKY"></a>
### `getLargestPoolSize()`
```java
public int getLargestPoolSize() {
    final ReentrantLock mainLock = this.mainLock;
    mainLock.lock();
    try {
        return largestPoolSize;
    } finally {
        mainLock.unlock();
    }
}
```
获取线程池里曾经创建过的最大线程数量。通过这个数据可以知道线程池是 否曾经满过。如该数值等于线程池的最大大小，则表示线程池曾经满过。
<a name="i5bqe"></a>
### `getPoolSize()`
```java
public int getPoolSize() {
    final ReentrantLock mainLock = this.mainLock;
    mainLock.lock();
    try {
        // Remove rare and surprising possibility of
        // isTerminated() && getPoolSize() > 0
        return runStateAtLeast(ctl.get(), TIDYING) ? 0
            : workers.size();
    } finally {
        mainLock.unlock();
    }
}
```
获取线程池的线程数量。如果线程池不销毁的话，线程池里的线程不会自动销 毁，所以这个大小只增不减。
<a name="vjGpW"></a>
### `getActiveCount()`
```java
public int getActiveCount() {
    final ReentrantLock mainLock = this.mainLock;
    mainLock.lock();
    try {
        int n = 0;
        for (Worker w : workers)
            if (w.isLocked())
                ++n;
        return n;
    } finally {
        mainLock.unlock();
    }
}
```
获取活动的线程数。
<a name="LufCl"></a>
## 合理地配置线程池
要想合理地配置线程池，就必须首先分析任务特性，可以从以下几个角度来分析。

- 任务的性质：CPU密集型任务、IO密集型任务和混合型任务。
- 任务的优先级：高、中和低。
- 任务的执行时间：长、中和短。
- 任务的依赖性：是否依赖其他系统资源，如数据库连接。

性质不同的任务可以用不同规模的线程池分开处理。CPU密集型任务应配置尽可能小的 线程，如配置Ncpu+1个线程的线程池。由于IO密集型任务线程并不是一直在执行任务，则应配 置尽可能多的线程，如2*Ncpu。混合型的任务，如果可以拆分，将其拆分成一个CPU密集型任务 和一个IO密集型任务，只要这两个任务执行的时间相差不是太大，那么分解后执行的吞吐量 将高于串行执行的吞吐量。如果这两个任务执行时间相差太大，则没必要进行分解。<br />优先级不同的任务可以使用优先级队列`PriorityBlockingQueue`来处理。它可以让优先级高 的任务先执行。

- 如果一直有优先级高的任务提交到队列里，那么优先级低的任务可能永远不能 执行。
- 可以通过 `Runtime.getRuntime().availableProcessors()`方法获得当前设备的CPU个数。
- 建议使用有界队列。有界队列能增加系统的稳定性和预警能力，可以根据需要设大一点 儿，比如几千。无界队列在某些异常情况下可能会撑爆内存。

N核服务器，通过执行业务的单线程分析出本地计算时间为x，等待时间为y，则工作线程数（线程池线程数）设置为 N*(x+y)/x，能让CPU的利用率最大化，详情可以参考线程数究竟设多少合理
