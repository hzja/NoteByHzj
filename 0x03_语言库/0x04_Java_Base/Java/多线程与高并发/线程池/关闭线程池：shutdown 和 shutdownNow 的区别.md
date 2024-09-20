Java 线程池
<a name="GSAaE"></a>
### 1、线程池示例
```java
public class ShutDownThreadPoolDemo {

    private ExecutorService service = Executors.newFixedThreadPool(10);

    public static void main(String[] args) {
        new ShutDownThreadPoolDemo().executeTask();
    }

    public void executeTask() {
        for (int i = 0; i < 100; i++) {
            service.submit(() -> {
                System.out.println(Thread.currentThread().getName() + "->执行");
            });
        }
    }
}
```
执行结果
```java
pool-1-thread-2->执行
pool-1-thread-3->执行
pool-1-thread-1->执行
pool-1-thread-4->执行
pool-1-thread-5->执行
pool-1-thread-6->执行
...
```
执行完成之后，主线程会一直阻塞，那么如何关闭线程池呢？介绍 5 种在 ThreadPoolExecutor 中涉及关闭线程池的方法，如下所示
```java
void shutdown
boolean isShutdown
boolean isTerminated
boolean awaitTermination(long timeout, TimeUnit unit) throws InterruptedException
List<Runnable> shutdownNow
```
<a name="ub1hp"></a>
### 2、`shutdown`
第一种方法叫作 `shutdown()`，它可以安全地关闭一个线程池，调用 `shutdown()` 方法之后线程池并不是立刻就被关闭，因为这时线程池中可能还有很多任务正在被执行，或是任务队列中有大量正在等待被执行的任务，调用 `shutdown()` 方法后线程池会在执行完正在执行的任务和队列中等待的任务后才彻底关闭。<br />调用 `shutdown()` 方法后如果还有新的任务被提交，线程池则会根据拒绝策略直接拒绝后续新提交的任务。<br />这段源码位置（jdk 1.8 版本）

- `java.util.concurrent.ThreadPoolExecutor#execute`
```java
public void execute(Runnable command) {
    if (command == null)
        throw new NullPointerException();
    int c = ctl.get();
    // 线程池中的线程比核心线程数少 
    if (workerCountOf(c) < corePoolSize) {
        // 新建一个核心线程执行任务
        if (addWorker(command, true))
            return;
        c = ctl.get();
    }
    // 核心线程已满，但是任务队列未满，添加到队列中
    if (isRunning(c) && workQueue.offer(command)) {
        int recheck = ctl.get();
        // 任务成功添加到队列以后，再次检查是否需要添加新的线程，因为已存在的线程可能被销毁了
        if (! isRunning(recheck) && remove(command))
            // 如果线程池处于非运行状态，并且把当前的任务从任务队列中移除成功，则拒绝该任务
            reject(command);
        else if (workerCountOf(recheck) == 0)
            // 如果之前的线程已经被销毁完，新建一个非核心线程
            addWorker(null, false);
    }
    // 核心线程池已满，队列已满，尝试创建一个非核心新的线程
    else if (!addWorker(command, false))
        // 如果创建新线程失败，说明线程池关闭或者线程池满了，拒绝任务
        reject(command);
}

```

- 1373 行 `if (! isRunning(recheck) && remove(command))`如果线程池被关闭，将当前的任务从任务队列中移除成功，并拒绝该任务
- 1378 行 `else if (!addWorker(command, false))`如果创建新线程失败，说明线程池关闭或者线程池满了，拒绝任务。
<a name="tdYcD"></a>
### 3、`isShutdown`
第二个方法叫作 `isShutdown()`，它可以返回 true 或者 false 来判断线程池是否已经开始了关闭工作，也就是是否执行了 `shutdown` 或者 `shutdownNow` 方法。<br />这里需要注意，如果调用 `isShutdown()` 方法的返回的结果为 true 并不代表线程池此时已经彻底关闭了，这仅仅代表线程池开始了关闭的流程，也就是说，此时可能线程池中依然有线程在执行任务，队列里也可能有等待被执行的任务。
<a name="hmbao"></a>
### 4、`isTerminated`
第三种方法叫作 `isTerminated()`，这个方法可以检测线程池是否真正“终结”了，这不仅代表线程池已关闭，同时代表线程池中的所有任务都已经都执行完毕了。<br />比如上面提到的情况，如果此时已经调用了 `shutdown` 方法，但是还有任务没有执行完，那么此时调用 `isShutdown` 方法返回的是 true，而 `isTerminated` 方法则会返回 false。<br />直到所有任务都执行完毕了，调用 `isTerminated()` 方法才会返回 true，这表示线程池已关闭并且线程池内部是空的，所有剩余的任务都执行完毕了。
<a name="GHIxR"></a>
### 5、`awaitTermination`
第四个方法叫作 `awaitTermination()`，它本身并不是用来关闭线程池的，而是主要用来判断线程池状态的。<br />比如给 `awaitTermination` 方法传入的参数是 10 秒，那么它就会陷入 10 秒钟的等待，直到发生以下三种情况之一：

- 等待期间（包括进入等待状态之前）线程池已关闭并且所有已提交的任务（包括正在执行的和队列中等待的）都执行完毕，相当于线程池已经“终结”了，方法便会返回 true
- 等待超时时间到后，第一种线程池“终结”的情况始终未发生，方法返回 false
- 等待期间线程被中断，方法会抛出 `InterruptedException` 异常

调用 `awaitTermination` 方法后当前线程会尝试等待一段指定的时间，如果在等待时间内，线程池已关闭并且内部的任务都执行完毕了，也就是说线程池真正“终结”了，那么方法就返回 true，否则超时返回 fasle。
<a name="UmxiC"></a>
### 6、`shutdownNow`
最后一个方法是 `shutdownNow()`，它和 `shutdown()` 的区别就是多了一个 Now，表示立刻关闭的意思，不推荐使用这一种方式关闭线程池。<br />在执行 `shutdownNow` 方法之后，首先会给所有线程池中的线程发送 `interrupt` 中断信号，尝试中断这些任务的执行，然后会将任务队列中正在等待的所有任务转移到一个 List 中并返回，可以根据返回的任务 List 来进行一些补救的操作，例如记录在案并在后期重试。<br />`shutdownNow` 源码如下：
```java
public List<Runnable> shutdownNow() {
    List<Runnable> tasks;
    final ReentrantLock mainLock = this.mainLock;
    mainLock.lock();
    try {
        checkShutdownAccess();
        advanceRunState(STOP);
        interruptWorkers();
        tasks = drainQueue();
    } finally {
        mainLock.unlock();
    }
    tryTerminate();
    return tasks;
}

```

- `interruptWorkers`

让每一个已经启动的线程都中断，这样线程就可以在执行任务期间检测到中断信号并进行相应的处理，提前结束任务
<a name="I3wMb"></a>
### 7、`shutdown` 和 `shutdownNow` 的区别？
`shutdown` 会等待线程池中的任务执行完成之后关闭线程池，而 `shutdownNow` 会给所有线程发送中断信号，中断任务执行，然后关闭线程池<br />`shutdown` 没有返回值，而 `shutdownNow` 会返回关闭前任务队列中未执行的任务集合（List）
