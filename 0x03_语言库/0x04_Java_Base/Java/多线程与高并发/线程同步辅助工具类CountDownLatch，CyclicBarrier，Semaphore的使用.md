CountDownLatch CyclicBarrier Semaphore<br />Java 并发编程是整个 Java 开发体系中最难以理解但也是最重要的知识点，也是各类开源分布式框架（如 ZooKeeper、Kafka、Spring Cloud、Netty 等）中各个并发组件实现的基础。J.U.C 并发包，即 `java.util.concurrent` 包，大大提高了并发性能，是 JDK 的核心工具包，是 JDK 1.5 之后，由 Doug Lea 实现并引入。而 AQS 被认为是 J.U.C 的核心。<br />AQS 是一个抽象类，并没有对并发类提供了一个统一的接口定义，而是由子类根据自身的情况实现相应的方法，AQS 中一般包含两个方法 acquire(int)、release(int)，获取同步状态和释放同步状态，AQS 根据其状态是否独占分为独占模式和共享模式。

- 独占模式：同一时刻最多只有一个线程获取同步状态，处于该模式下，其他线程试图获取该锁将无法获取成功。
- 共享模式：同一时刻会有多个线程获取共享同步状态，处于该模式下，其他线程试图获取该锁可能会获取成功。

同步器根据同步状态分为独占模式和共享模式，独占模式包括类：`ReentrantLock`、`ReentrantReadWriteLock.WriteLock`，共享模式包括：`Semaphore`、`CountDownLatch`、`ReentrantReadWriteLock.ReadLock`，着重看一下 `java.util.concurrent` 包下一些辅助同步器类：`CountDownLatch`、`CyclicBarrier`、`Semaphore`、`Exchanger`、`Phaser`
<a name="dbbb77f6f1f4f64f8005481b395cee44"></a>
## 1、`CountDownLatch` - 闭锁
<a name="e7fbbcfe57bfb5511d0be39a4542c716"></a>
### 简介
`CountDownLatch` 是一个同步辅助工具类，通过它可以完成类似于阻塞当前线程的功能，也就是一个或多个线程一直等待直到其他线程执行完成。即允许一个或多个线程一直等待，直到其他线程执行完后再执行。例如，应用程序的主线程希望在负责启动框架服务的线程已经启动所有框架服务之后执行。<br />`CountDownLatch` 用了一个给定的计数器 `cnt` 来进行初始化，该计数器的操作是原子操作，即同时只能有一个线程操作该计数器，调用该类 `await` 方法的线程会一直处于阻塞状态，直到其他线程调用 `countDown` 方法时计数器的值变成 `0`，每次调用 `countDown` 时计数器的值会减 `1`，当计数器的值为 `0` 时所有因 await 方法而处于等待状态的线程就会继续执行。计数器 `cnt` 是闭锁需要等待的线程数量，只能被设置一次，且 `CountDownLatch` 没有提供任何机制去重新设置计数器 `count`，如果需要重置，可以考虑使用 `CyclicBarrier`。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1614872838019-9ecafc87-5023-499a-85d8-ac8e8ac01a84.webp#height=648&id=mtCFf&originHeight=338&originWidth=353&originalType=binary&size=0&status=done&style=shadow&width=677)<br />CountdownLatch_example
<a name="d7c32887d5b5b541c9ed41140ec8bdd4"></a>
### 使用场景
（1）开启多个线程分块下载一个大文件，每个线程只下载固定的一截，最后由另外一个线程来拼接所有的分段。<br />（2）应用程序的主线程希望在负责启动框架服务的线程已经启动所有的框架服务之后再执行。<br />（3）确保一个计算不会执行，直到所需要的资源被初始化。<br />（4）并行计算，处理量很大时可以将运算任务拆分成多个子任务，当所有子任务都完成之后，父任务再将所有子任务都结果进行汇总。
<a name="4928022d8d7e37a1b4aafeafa94200ca"></a>
### 主要接口分析
`CountDownLatch` 内部依赖 `Sync` 实现，而 `Sync` 继承 AQS。`CountDownLatch` 关键接口如下：

- `countDown()` 如果当前计数器的值大于 `1`，则将其减 `1`；若当前值为 `1`，则将其置为 `0` 并唤醒所有通过 `await` 等待的线程；若当前值为 `0`，则什么也不做直接返回。
- `await()` 等待计数器的值为 `0`，若计数器的值为 `0` 则该方法返回；若等待期间该线程被中断，则抛出 `InterruptedException` 并清除该线程的中断状态。
- `await(long timeout, TimeUnit unit)` 在指定的时间内等待计数器的值为 `0`，若在指定时间内计数器的值变为 `0`，则该方法返回 `true`；若指定时间内计数器的值仍未变为 `0`，则返回 `false`；若指定时间内计数器的值变为 `0` 之前当前线程被中断，则抛出 `InterruptedException` 并清除该线程的中断状态。
- `getCount()` 读取当前计数器的值，一般用于调试或者测试。
<a name="38f418cad213c36c2e938e700640029e"></a>
### Coding 演示
<a name="29b9197a32a471cb9dd28e3acfe2fe87"></a>
#### （1）作为一个开关 / 入口
将初始计数值为 1 的 `CountDownLatch` 作为一个的开关或入口，在调用 `countDown()` 的线程打开入口前，所有调用 `await` 的线程都一直在入口处等待。
```java
public class Driver {
    private static final Integer WORK_COUNT = 10;
    private static final ExecutorService executorService = Executors.newCachedThreadPool();
    public static void main(String[] args) {
        // 初始化计数器为 10 的 CountDownLatch
        CountDownLatch countDownLatch = new CountDownLatch(1);
        for (int i = 0; i < WORK_COUNT; i++) {
            executorService.execute(new Worker(countDownLatch));
        }
        // 主线程执行
        doSomething();
        // 主线程开启开关
        countDownLatch.countDown();
        // 平滑地关闭 ExecutorService
        executorService.shutdown();
    }
    private static void doSomething() {
        // ...
        System.out.print("start..");
    }
}
class Worker implements Runnable {
    private final CountDownLatch countDownLatch;
    Worker(CountDownLatch countDownLatch) {
        this.countDownLatch = countDownLatch;
    }
    @Override
    public void run() {
        try {
            // 所有执行线程在此处等待开关开启 [多个子线程同时执行]
            countDownLatch.await();
            // 子线程执行
            doWork();
        } catch (InterruptedException ignored) {
        }
    }
    private void doWork() {
        // ...
        System.out.print("run..");
    }
}
// Output
// start..run..run..run..run..run..run..run..run..run..run..
```
<a name="dba6e707a3299216002e6be97e85277e"></a>
#### （2）作为一个完成信号
将初始计数值为 N 的 `CountDownLatch` 作为一个完成信号点，使某个线程在其它 N 个线程完成某项操作之前一直等待。
```java
public class Driver {
    private static final Integer WORK_COUNT = 10;
    private static final ExecutorService executorService = Executors.newCachedThreadPool();
    public static void main(String[] args) throws InterruptedException {
        // 初始化计数器为 10 的 CountDownLatch
        CountDownLatch countDownLatch = new CountDownLatch(WORK_COUNT);
        for (int i = 0; i < WORK_COUNT; i++) {
            executorService.execute(new Worker(countDownLatch));
        }
        // 主线程等待其它 N 个线程完成
        countDownLatch.await();
        // 主线程执行
        doSomething();
        // 平滑地关闭 ExecutorService
        executorService.shutdown();
    }
    private static void doSomething() {
        // ...
        System.out.println("end");
    }
}
class Worker implements Runnable {
    private final CountDownLatch countDownLatch;
    Worker(CountDownLatch countDownLatch) {
        this.countDownLatch = countDownLatch;
    }
    @Override
    public void run() {
        // 子线程执行
        doWork();
        // 每个线程做完自己的事情后, 就将计数器减去 1
        countDownLatch.countDown();
    }
    private void doWork() {
        // ...
        System.out.print("run..");
    }
}
// Output
// run..run..run..run..run..run..run..run..run..run..end
```
<a name="3ff4675902d3a2d3444521a38ef6c180"></a>
## 2、`CyclicBarrier` - 循环栅栏
<a name="51de59f3ce7cea8b08e15e3cb556b951"></a>
### 简介
`CyclicBarrier` 和 `CountDownLatch` 是非常类似的，`CyclicBarrier` 核心的概念是在于设置一个等待线程的数量边界，到达了此边界之后进行执行。`CyclicBarrier` 也是一个同步辅助工具类，它允许一组线程相互等待直到到达某个公共的屏障点（Common Barrier Point），通过它可以完成多个线程之间相互等待时，只有当每个线程都准备就绪后才能各自继续执行后面的操作。<br />`CyclicBarrier` 也是通过计数器来实现，当某个线程调用 `await` 方法后就进入等待状态，计数器执行加一操作。当计数器的值达到了设置的初始值时等待状态的线程会被唤醒继续执行。通过调用 `CyclicBarrier` 对象的 `await()` 方法，两个线程可以实现互相等待。一旦 N 个线程在等待 `CyclicBarrier` 达成，所有线程将被释放掉去继续执行。由于 `CyclicBarrier` 在释放等待线程后可以重用，所以可以称之为循环栅栏。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1614872837935-b7fd4930-2991-4c24-a03c-d63062c165e5.webp#height=635&id=CJsU9&originHeight=452&originWidth=482&originalType=binary&size=0&status=done&style=shadow&width=677)

<a name="cc6b9e0bb803c873f0bf7c480ea3d074"></a>
### 使用场景
`CyclicBarrier` 特别适用于并行迭代计算，每个线程负责一部分计算，然后在栅栏处等待其他线程完成，所有线程到齐后，交换数据和计算结果，再进行下一次迭代。
<a name="8d2cac72dc91e0eebdcd3a0c8ac6e60e"></a>
### 主要接口分析
`CyclicBarrier` 并没有自己去实现 AQS 框架的 API，而是利用了 `ReentrantLock` 和 `Condition`。<br />`CyclicBarrier` 提供的关键方法如下：

- `await()` 等待其它参与方的到来（调用 `await()`）。如果当前调用是最后一个调用，则唤醒所有其它的线程的等待并且如果在构造 `CyclicBarrier` 时指定了 `action`，当前线程会去执行该 `action`，然后该方法返回该线程调用 await 的次序（`getParties()-1` 说明该线程是第一个调用 `await` 的，`0` 说明该线程是最后一个执行 `await` 的），接着该线程继续执行 `await` 后的代码；如果该调用不是最后一个调用，则阻塞等待；如果等待过程中，当前线程被中断，则抛出 `InterruptedException`；如果等待过程中，其它等待的线程被中断，或者其它线程等待超时，或者该 `barrier` 被 `reset`，或者当前线程在执行 `barrier` 构造时注册的 `action` 时因为抛出异常而失败，则抛出 `BrokenBarrierException`。
- `await(long timeout, TimeUnit unit)` 与 `await()` 唯一的不同点在于设置了等待超时时间，等待超时时会抛出 `TimeoutException`。
- `reset()` 该方法会将该 `barrier` 重置为它的初始状态，并使得所有对该 `barrier` 的 `await` 调用抛出 `BrokenBarrierException`。

`CyclicBarrier` 提供的两个构造函数：

- `CyclicBarrier(int parties)`：`parties` 表示拦截线程的数量。创建一个新的 `CyclicBarrier`，它将在给定数量的参与者（线程）处于等待状态时启动，但它不会在启动 `barrier` 时执行预定义的操作。
- `CyclicBarrier(int parties, Runnable barrierAction)` ：`barrierAction` 为` CyclicBarrier` 接收的 `Runnable` 命令，用于在线程到达屏障时，优先执行 `barrierAction` ，用于处理更加复杂的业务场景。创建一个新的 `CyclicBarrier`，它将在给定数量的参与者（线程）处于等待状态时启动，并在启动 `barrier` 时执行给定的屏障操作，该操作由最后一个进入 `barrier` 的线程执行。
<a name="f3401ffc16fcf0b015bfb33f6c184d77"></a>
### Coding 演示
<a name="c80e4c59dc974c4d8e276d0f7ec9dc65"></a>
#### （1）简单例子
```java
public class Solver {
    private static final Integer WORK_COUNT = 10;
    private static final ExecutorService executorService = Executors.newCachedThreadPool();
    public static void main(String[] args) {
        // 初始化计数器为 10 的 CyclicBarrier
        CyclicBarrier cyclicBarrier = new CyclicBarrier(WORK_COUNT);
        for (int i = 0; i < WORK_COUNT; i++) {
            executorService.execute(new Worker(cyclicBarrier));
        }
        // 平滑地关闭 ExecutorService
        executorService.shutdown();
    }
}
class Worker implements Runnable {
    private final CyclicBarrier cyclicBarrier;
    Worker(CyclicBarrier cyclicBarrier) {
        this.cyclicBarrier = cyclicBarrier;
    }
    @Override
    public void run() {
        System.out.print("before..");
        try {
            // 多个线程之间相互等待时，只有当每个线程都准备就绪后才能各自继续执行后面的操作
            cyclicBarrier.await();
        } catch (InterruptedException | BrokenBarrierException e) {
            e.printStackTrace();
        }
        // 子线程执行
        doWork();
    }
    private void doWork() {
        // ...
        System.out.print("after..");
    }
}
// Output
// before..before..before..before..before..before..before..before..before..before..after..after..after..after..after..after..after..after..after..after..
```
<a name="2d49b2a36727fa5260e0c97798ec2bda"></a>
#### （2）执行 `barrierAction`
在 `ready` 状态时日志是每秒输出一条，当有 5 条 `ready` 时会一次性输出 5 条 `continue`。这就是前面讲的全部线程准备就绪后同时开始执行。在初始化 `CyclicBarrier` 时还可以在等待线程数后指定一个 `runnable`，含义是当线程到达这个屏障时优先执行这里的 `runnable`。
```java
public class Solver {
    private static final Integer WORK_COUNT = 10;
    private static final ExecutorService executorService = Executors.newCachedThreadPool();
    public static void main(String[] args) throws InterruptedException {
        // 初始化计数器为 5 的 CyclicBarrier
        CyclicBarrier cyclicBarrier = new CyclicBarrier(5, () -> System.out.println(String.format("%s call back is ready.", Thread.currentThread().getName())));
        for (int i = 0; i < WORK_COUNT; i++) {
            Thread.sleep(1000);
            executorService.execute(new Worker(cyclicBarrier));
        }
        // 平滑地关闭 ExecutorService
        executorService.shutdown();
    }
}
class Worker implements Runnable {
    private final CyclicBarrier cyclicBarrier;
    Worker(CyclicBarrier cyclicBarrier) {
        this.cyclicBarrier = cyclicBarrier;
    }
    @Override
    public void run() {
        try {
            System.out.println(String.format("%s is ready", Thread.currentThread().getName()));
            cyclicBarrier.await();
            System.out.println(String.format("%s continue", Thread.currentThread().getName()));
        } catch (InterruptedException | BrokenBarrierException e) {
            e.printStackTrace();
        }
    }
}
// Output
// pool-1-thread-1 is ready
// pool-1-thread-2 is ready
// pool-1-thread-3 is ready
// pool-1-thread-4 is ready
// pool-1-thread-5 is ready
// pool-1-thread-5 call back is ready.
// pool-1-thread-5 continue
// pool-1-thread-3 continue
// pool-1-thread-4 continue
// pool-1-thread-1 continue
// pool-1-thread-2 continue
// pool-1-thread-5 is ready
// pool-1-thread-1 is ready
// pool-1-thread-4 is ready
// pool-1-thread-2 is ready
// pool-1-thread-3 is ready
// pool-1-thread-3 call back is ready.
// pool-1-thread-3 continue
// pool-1-thread-5 continue
// pool-1-thread-4 continue
// pool-1-thread-2 continue
// pool-1-thread-1 continue
```
<a name="63913d91ed675cd07653b0aca906a201"></a>
### `CyclicBarrier` 与 `CountDownLatch` 区别
`CyclicBarrier` 与 `CountDownLatch` 可能容易混淆，强调下其区别：

- `CountDownLatch` 的参与线程是有不同角色的，有的负责倒计时，有的在等待倒计时变为 0，负责倒计时和等待倒计时的线程都可以有多个，它用于不同角色线程间的同步。
- `CyclicBarrier` 的参与线程角色是一样的，用于同一角色线程间的协调一致。
- `CountDownLatch` 是一次性的，而 `CyclicBarrier` 是可以重复利用的。
<a name="91e586086ac1c8047eac7da7d5202b85"></a>
## 3、`Semaphore` - 信号量
<a name="9a56e17ef82a0befe80836d15f8283d6"></a>
### 简介
`Semaphore`，又名信号量，这个类的作用有点类似于 “许可证”。信号量 `Semaphore` 是一个控制访问多个共享资源的计数器，和 `CountDownLatch` 一样，其本质上是一个 “共享锁”。从源码角度来看，Semaphore 的实现方式和 `CountDownLatch` 非常相似，基于 AQS 做了一些定制。通过维持 AQS 的锁全局计数 state 字段来实现定量锁的加锁和解锁操作。`Semaphore` 通常用于限制可以访问某些资源（物理或逻辑的）的线程数目。<br />有时，因为一些原因需要控制同时访问共享资源的最大线程数量，比如出于系统性能的考虑需要限流，或者共享资源是稀缺资源，需要有一种办法能够协调各个线程，以保证合理的使用公共资源。当有线程想要访问共享资源时，需要先获取 (`acquire`) 的许可；如果许可不够了，线程需要一直等待，直到许可可用。当线程使用完共享资源后，可以归还 (`release`) 许可，以供其它需要的线程使用；然而，实际上并没有真实的许可证对象供线程使用，Semaphore 只是对可用的数量进行管理维护。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1614872837978-b9e5678a-f035-484f-a3da-7dcb3848dee5.webp#height=577&id=G9lB5&originHeight=577&originWidth=398&originalType=binary&size=0&status=done&style=shadow&width=398)
<a name="ce737afac51834cbd1e984b07f5d2fb6"></a>
### 使用场景
`Semaphore` 可以用于做流量控制，特别公用资源有限的应用场景，比如数据库连接。
<a name="d83dda464317962a4dc86bcd0e75e5db"></a>
### 主要接口分析
`Semaphore` 内部包含公平锁（`FairSync`）和非公平锁（`NonfairSync`），继承内部类 `Sync`，其中 `Sync` 继承 AQS，作为 `Semaphore` 的公平锁和非公平锁的基类。<br />`CyclicBarrier` 提供的关键方法如下：

- `isFair()`：是否公平模式 FIFO
- `availablePermits()`：获取当前可用的许可证数量
- `acquire()`：当前线程尝试去阻塞的获取 1 个许可证。此过程是阻塞的，它会一直等待许可证，直到发生以下任意一件事：当前线程获取了 1 个可用的许可证，则会停止等待，继续执行；当前线程被中断，则会抛出 `InterruptedException` 异常，并停止等待，继续执行。
- `acquire(permits)`：当前线程尝试去阻塞的获取 permits 个许可证。此过程是阻塞的，它会一直等待许可证，直到发生以下任意一件事：当前线程获取了 n 个可用的许可证，则会停止等待，继续执行；当前线程被中断，则会抛出 `InterruptedException` 异常，并停止等待，继续执行。
- `acquierUninterruptibly()`：当前线程尝试去阻塞的获取 1 个许可证 (不可中断的)。此过程是阻塞的，它会一直等待许可证，直到发生以下任意一件事：当前线程获取了 1 个可用的许可证，则会停止等待，继续执行。
- `acquireUninterruptibly(permits)`：当前线程尝试去阻塞的获取 permits 个许可证。此过程是阻塞的，它会一直等待许可证，直到发生以下任意一件事：当前线程获取了 n 个可用的许可证，则会停止等待，继续执行。
- `tryAcquire()`：当前线程尝试去获取 1 个许可证。此过程是非阻塞的，它只是在方法调用时进行一次尝试。如果当前线程获取了 1 个可用的许可证，则会停止等待，继续执行，并返回 `true`。如果当前线程没有获得这个许可证，也会停止等待，继续执行，并返回 `false`。
- `tryAcquire(permits)`：当前线程尝试去获取 `permits` 个许可证。此过程是非阻塞的，它只是在方法调用时进行一次尝试。如果当前线程获取了 permits 个可用的许可证，则会停止等待，继续执行，并返回 `true`。如果当前线程没有获得 `permits` 个许可证，也会停止等待，继续执行，并返回 `false`。
- `tryAcquire(timeout, TimeUnit)`：当前线程在限定时间内，阻塞的尝试去获取 1 个许可证。此过程是阻塞的，它会一直等待许可证，直到发生以下任意一件事：当前线程获取了可用的许可证，则会停止等待，继续执行，并返回 true；当前线程等待时间 `timeout` 超时，则会停止等待，继续执行，并返回 `false`；当前线程在 `timeout` 时间内被中断，则会抛出 `InterruptedException` 一次，并停止等待，继续执行。
- `tryAcquire(permits, timeout, TimeUnit)`：当前线程在限定时间内，阻塞的尝试去获取 `permits` 个许可证。此过程是阻塞的，它会一直等待许可证，直到发生以下任意一件事：当前线程获取了可用的 `permits` 个许可证，则会停止等待，继续执行，并返回 `true`；当前线程等待时间 `timeout` 超时，则会停止等待，继续执行，并返回 `false`；当前线程在 `timeout` 时间内被中断，则会抛出 `InterruptedException` 一次，并停止等待，继续执行。
- `release()`：当前线程释放 1 个可用的许可证。
- `release(permits)`：当前线程释放 permits 个可用的许可证。
- `drainPermits()`：当前线程获得剩余的所有可用许可证。
- `hasQueuedThreads()`：判断当前 `Semaphore` 对象上是否存在正在等待许可证的线程。
- `getQueueLength()`：获取当前 `Semaphore` 对象上是正在等待许可证的线程数量。

`Semaphore` 提供了两个构造函数：

- `Semaphore(int permits)`：创建具有给定的许可数和非公平的公平设置的 `Semaphore`，`Semaphore` 默认选择非公平锁。
- `Semaphore(int permits, boolean fair)`：创建具有给定的许可数和给定的公平设置的 `Semaphore`。`Semaphore` 有两种模式，公平模式和非公平模式。公平模式就是调用 `acquire` 的顺序就是获取许可证的顺序，遵循 FIFO；而非公平模式是抢占式的，也就是有可能一个新的获取线程恰好在一个许可证释放时得到了这个许可证，而前面还有等待的线程，简单的说就是随机选取新线程来运行。
<a name="5abf34e3e3a546213e7680992a38d2da"></a>
### Coding 演示
```java
public class SemaphoreExample {
    public static void main(String[] args) {
        final int clientCount = 3;
        final int totalRequestCount = 10;
        Semaphore semaphore = new Semaphore(clientCount);
        ExecutorService executorService = Executors.newCachedThreadPool();
        for (int i = 0; i < totalRequestCount; i++) {
            executorService.execute(()->{
                try {
                    semaphore.acquire();
                    System.out.print(semaphore.availablePermits() + " ");
                } catch (InterruptedException e) {
                    e.printStackTrace();
                } finally {
                    semaphore.release();
                }
            });
        }
        executorService.shutdown();
    }
}
// Output
// 2 1 2 2 2 2 2 1 2 2
```
<a name="e4907d180c3f83bda5ab66124eb0f1de"></a>
## 4、Exchanger - 交换器
<a name="03b3ce41b07b7f380a3692d8667692e2"></a>
### 简介
Exchanger（交换器）是一个用于线程间协作的工具类，是 JDK 1.5 开始提供的一个用于两个工作线程之间交换数据的封装工具类。Exchanger 有点类似于 CyclicBarrier ，CyclicBarrier 是一个栅栏，到达栅栏的线程需要等待其它一定数量的线程到达后，才能通过栅栏，Exchanger 可以看成是一个双向栅栏。它提供一个同步点，在这个同步点两个线程可以交换彼此的数据。<br />可简单地将 Exchanger 对象理解为一个包含两个格子的容器，通过 exchanger 方法可以向两个格子中填充信息。当两个格子中的均被填充时，该对象会自动将两个格子的信息交换，然后返回给线程，从而实现两个线程的信息交换。这两个线程通过 exchange 方法交换数据，如果第一个线程先执行 exchange 方法，它会一直等待第二个线程也执行 exchange，当两个线程都到达同步点时，这两个线程就可以交换数据，将本线程生产出来的数据传递给对方。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1614872837982-8f57cb0b-55da-4646-bc5a-9f244d9f10df.webp#height=358&id=qWvTp&originHeight=358&originWidth=800&originalType=binary&size=0&status=done&style=shadow&width=800)
<a name="76f70e759220dd139b58b5915633a0cb"></a>
### 主要接口分析
`Exchanger` 是最简单的也是最复杂的，简单在于 API 非常简单，就一个构造方法和两个 `exchange()` 方法，最复杂在于它的实现是最复杂的。<br />`Exchanger` 提供的关键方法如下：

- `exchange(V x)` ：当前线程跟另外一个线程交换数据 `x`，如果另外一个线程的数据准备好，那么当前线程会立刻返回，并获得另外一个线程的数据；否则当前线程会进入等待状态。
- `V exchange(V x, long timeout, TimeUnit unit)`：当前线程跟另外一个线程交换数据 `x`，有一个指定的超时时间，如果在等待时间超时了，而且还没有收到对方的数据的话，则会抛出 `TimeoutException` 异常。

可以看出，当一个线程到达 `exchange` 调用点时，如果其他线程此前已经调用了此方法，则其他线程会被调度唤醒并与之进行对象交换，然后各自返回；如果其他线程还没到达交换点，则当前线程会被挂起，直至其他线程到达才会完成交换并正常返回，或者当前线程被中断或超时返回。
<a name="b263b855494587bc8b5f95c15f3fd840"></a>
### Coding 演示
```java
public class ExchangerExample {
    private static final Integer WORK_COUNT = 2;
    public static void main(String[] args) {
        Exchanger<String> exchanger = new Exchanger<>();
        ExecutorService executorService = Executors.newCachedThreadPool();
        for (int i = 0; i < WORK_COUNT; i++) {
            executorService.execute(() -> {
                String beforeObj = Thread.currentThread().getName();
                try {
                    String afterObj = exchanger.exchange(Thread.currentThread().getName());
                    System.out.println(String.format("currentThread %s , before exchange %s , after exchange %s", Thread.currentThread().getName(), beforeObj, afterObj));
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            });
        }
        executorService.shutdown();
    }
}
// Output
// currentThread pool-1-thread-1 , before exchange pool-1-thread-1 , after exchange pool-1-thread-2 
// currentThread pool-1-thread-2 , before exchange pool-1-thread-2 , after exchange pool-1-thread-1
```
<a name="ca8ec787fecba7451ec4eff4ae089df2"></a>
## 5、`Phaser` - 多阶段栅栏
<a name="f8c87103b061d8eb9ce8453f59765597"></a>
### 简介
`CountDownLatch` 和 `CyclicBarrier` 都是 JDK 1.5 引入的，而 `Phaser` 是 JDK 1.7 引入的。`Phaser` 的功能与 `CountDownLatch` 和 `CyclicBarrier` 有部分重叠，它几乎可以取代 `CountDownLatch` 和 `CyclicBarrier`， 其功能更灵活，更强大，支持动态调整需要控制的线程数。<br />`CountDownLatch`，闭锁，在完成一组正在其他线程中执行的操作之前，它允许一个或多个线程一直等待，它提供了 `await()`、`countDown()` 两个方法来进行操作；`CyclicBarrier`，循环栅栏，允许一组线程互相等待，直到到达某个公共屏障点，它提供的 `await()` 可以实现让所有参与者在临界点到来之前一直处于等待状态；Phaser，多阶段栅栏，它把多个线程协作执行的任务划分为多个阶段，编程时需要明确各个阶段的任务，每个阶段都可以有任意个参与者，线程都可以随时注册并参与到某个阶段，当到达的参与者数量满足栅栏设定的数量后，会进行阶段升级（advance）。<br />`Phaser` 顾名思义，与阶段相关。`Phaser` 比较适合这样一种场景，一种任务可以分为多个阶段，现希望多个线程去处理该批任务，对于每个阶段，多个线程可以并发进行，但是希望保证只有前面一个阶段的任务完成之后才能开始后面的任务。这种场景可以使用多个 `CyclicBarrier` 来实现，每个 `CyclicBarrier` 负责等待一个阶段的任务全部完成。但是使用 `CyclicBarrier` 的缺点在于，需要明确知道总共有多少个阶段，同时并行的任务数需要提前预定义好，且无法动态修改。而 `Phaser` 可同时解决这两个问题。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1614872837997-35f67c4b-b78b-45b1-ae20-b424b396b0e1.webp#height=1063&id=BZnBq&originHeight=526&originWidth=335&originalType=binary&size=0&status=done&style=shadow&width=677)
<a name="bb706d527828d9a64511986a74f278f1"></a>
### 使用场景
`Phaser` 主要接口如下：
<a name="242299a43a53ce0db78bfaa7f6d0fde1"></a>
### 主要接口分析
`Phaser` 提供的关键方法如下：

- `arriveAndAwaitAdvance()`：当前线程当前阶段执行完毕，等待其它线程完成当前阶段。如果当前线程是该阶段最后一个未到达的，则该方法直接返回下一个阶段的序号（阶段序号从 0 开始），同时其它线程的该方法也返回下一个阶段的序号。`arriveAndAwaitAdvance` 方法是不响应中断的，也就是说即使当前线程被中断，`arriveAndAwaitAdvance` 方法也不会返回或抛出异常，而是继续等待。如果希望能够响应中断，可以参考 `awaitAdvanceInterruptibly` 方法。
- `arriveAndDeregister()`：该方法立即返回下一阶段的序号，并且其它线程需要等待的个数减一，并且把当前线程从之后需要等待的成员中移除。如果该 `Phaser` 是另外一个 `Phaser` 的子 `Phaser`，并且该操作导致当前 `Phaser` 的成员数为 `0`，则该操作也会将当前 `Phaser` 从其父 `Phaser` 中移除。
- `arrive()`：该方法不作任何等待，直接返回下一阶段的序号。
- `awaitAdvance(int phase)`：该方法等待某一阶段执行完毕。如果当前阶段不等于指定的阶段或者该 `Phaser` 已经被终止，则立即返回。该阶段数一般由 `arrive()` 方法或者 `arriveAndDeregister()` 方法返回。返回下一阶段的序号，或者返回参数指定的值（如果该参数为负数），或者直接返回当前阶段序号（如果当前 `Phaser` 已经被终止）。
- `awaitAdvanceInterruptibly(int phase)`：效果与 `awaitAdvance(int phase)` 相当，唯一的不同在于若该线程在该方法等待时被中断，则该方法抛出 `InterruptedException`。
- `awaitAdvanceInterruptibly(int phase, long timeout, TimeUnit unit)`：效果与 `awaitAdvanceInterruptibly(int phase)` 相当，区别在于如果超时则抛出 `TimeoutException`。
- `bulkRegister(int parties)`：注册多个 `party`。如果当前 `phaser` 已经被终止，则该方法无效，并返回负数。如果调用该方法时，`onAdvance` 方法正在执行，则该方法等待其执行完毕。如果该 `Phaser` 有父 `Phaser` 则指定的 `party` 数大于 `0`，且之前该 `Phaser` 的 `party` 数为 `0`，那么该 `Phaser` 会被注册到其父 `Phaser` 中。
- `forceTermination()`：强制让该 `Phaser` 进入终止状态。已经注册的 party 数不受影响。如果该 `Phaser` 有子 `Phaser`，则其所有的子 `Phaser` 均进入终止状态。如果该 `Phaser` 已经处于终止状态，该方法调用不造成任何影响。
<a name="c477438b2e2fdf2763a9308e5f958991"></a>
### Coding 演示
<a name="9dbe718412dadc4139dd40b56a2acef1"></a>
#### （1）通过 `Phaser` 实现 `CyclicBarrier` 控制多个线程的执行时机的功能
通过 `Phaser` 控制多个线程的执行时机：有时候希望所有线程到达指定点后再同时开始执行，可以利用 `CyclicBarrier` 来实现，这里给出使用 `Phaser` 的版本。
```java
public class PhaserExample {
    public static void main(String[] args) {
        final int totalRequestCount = 10;
        Phaser phaser = new Phaser();
        ExecutorService executorService = Executors.newCachedThreadPool();
        for (int i = 0; i < totalRequestCount; i++) {
            phaser.register();
            executorService.execute(() -> {
                // 等待其它参与者线程到达 [arriveAndAwaitAdvance 方法是不响应中断的，也就是说即使当前线程被中断，arriveAndAwaitAdvance 方法也不会返回或抛出异常，而是继续等待。如果希望能够响应中断，可以参考 awaitAdvanceInterruptibly 方法]
                int j = phaser.arriveAndAwaitAdvance();
                // do something
                System.out.println(String.format("currentThread:%s, Executing the task, currentPhase:%s", Thread.currentThread().getName(), j));
            });
        }
        executorService.shutdown();
    }
}
// Output
// currentThread:pool-1-thread-1, Executing the task, currentPhase:1
// currentThread:pool-1-thread-2, Executing the task, currentPhase:1
// currentThread:pool-1-thread-7, Executing the task, currentPhase:1
// currentThread:pool-1-thread-4, Executing the task, currentPhase:1
// currentThread:pool-1-thread-6, Executing the task, currentPhase:1
// currentThread:pool-1-thread-3, Executing the task, currentPhase:1
// currentThread:pool-1-thread-10, Executing the task, currentPhase:1
// currentThread:pool-1-thread-9, Executing the task, currentPhase:1
// currentThread:pool-1-thread-5, Executing the task, currentPhase:1
// currentThread:pool-1-thread-8, Executing the task, currentPhase:1
```
<a name="c11669a6f75aadf99c913fbcfcda9bb4"></a>
#### （2）通过 `Phaser` 实现 `CyclicBarrier` 执行 `barrierAction`
`CyclicBarrier` 支持 barrier action, `Phaser` 同样也支持。不同之处是 `Phaser` 的 barrier action 需要改写 `onAdvance` 方法来进行定制。
```java
public class PhaserExample {
    public static void main(String[] args) {
        final int totalRequestCount = 10;
        Phaser phaser = new Phaser() {
            @Override
            protected boolean onAdvance(int phase, int registeredParties) {
                System.out.println(String.format("%s call back is ready.", Thread.currentThread().getName()));
                return super.onAdvance(phase, registeredParties);
            }
        };
        ExecutorService executorService = Executors.newCachedThreadPool();
        for (int i = 0; i < totalRequestCount; i++) {
            // 注册各个参与者线程
            phaser.register();
            executorService.execute(() -> {
                // 等待其它参与者线程到达 [arriveAndAwaitAdvance 方法是不响应中断的，也就是说即使当前线程被中断，arriveAndAwaitAdvance 方法也不会返回或抛出异常，而是继续等待。如果希望能够响应中断，可以参考 awaitAdvanceInterruptibly 方法]
                int j = phaser.arriveAndAwaitAdvance();
                // do something
                System.out.println(String.format("currentThread:%s, Executing the task, currentPhase:%s", Thread.currentThread().getName(), j));
            });
        }
        executorService.shutdown();
    }
}
// Output
// pool-1-thread-10 call back is ready.
// currentThread:pool-1-thread-10, Executing the task, currentPhase:1
// currentThread:pool-1-thread-9, Executing the task, currentPhase:1
// currentThread:pool-1-thread-7, Executing the task, currentPhase:1
// currentThread:pool-1-thread-8, Executing the task, currentPhase:1
// currentThread:pool-1-thread-5, Executing the task, currentPhase:1
// currentThread:pool-1-thread-3, Executing the task, currentPhase:1
// currentThread:pool-1-thread-1, Executing the task, currentPhase:1
// currentThread:pool-1-thread-6, Executing the task, currentPhase:1
// currentThread:pool-1-thread-2, Executing the task, currentPhase:1
// currentThread:pool-1-thread-4, Executing the task, currentPhase:1
```
<a name="eb7f362843ee1d9f11b13e21d51d400b"></a>
#### （3）通过 `Phaser` 实现 `CountDownLatch` 作为一个开关 / 入口功能
```java
public class PhaserExample {
    public static void main(String[] args) throws IOException {
        final int totalRequestCount = 10;
        // 注册主线程, 当外部条件满足时, 由主线程打开开关
        Phaser phaser = new Phaser(1);
        ExecutorService executorService = Executors.newCachedThreadPool();
        for (int i = 0; i < totalRequestCount; i++) {
            // 注册各个参与者线程
            phaser.register();
            executorService.execute(() -> {
                // 等待其它参与者线程到达 [arriveAndAwaitAdvance 方法是不响应中断的，也就是说即使当前线程被中断，arriveAndAwaitAdvance 方法也不会返回或抛出异常，而是继续等待。如果希望能够响应中断，可以参考 awaitAdvanceInterruptibly 方法]
                int j = phaser.arriveAndAwaitAdvance();
                // do something
                System.out.println(String.format("currentThread:%s, Executing the task, currentPhase:%s", Thread.currentThread().getName(), j));
            });
        }
        // 打开开关 [parties 共 11 个, 主线程从之后需要等待的成员中移除, 即 parties 还剩 10]
        phaser.arriveAndDeregister();
        System.out.println("主线程打开了开关");
        executorService.shutdown();
    }
}
// Output
// 主线程打开了开关
// currentThread:pool-1-thread-6, Executing the task, currentPhase:1
// currentThread:pool-1-thread-7, Executing the task, currentPhase:1
// currentThread:pool-1-thread-1, Executing the task, currentPhase:1
// currentThread:pool-1-thread-8, Executing the task, currentPhase:1
// currentThread:pool-1-thread-3, Executing the task, currentPhase:1
// currentThread:pool-1-thread-9, Executing the task, currentPhase:1
// currentThread:pool-1-thread-4, Executing the task, currentPhase:1
// currentThread:pool-1-thread-2, Executing the task, currentPhase:1
// currentThread:pool-1-thread-5, Executing the task, currentPhase:1
```
<a name="af8248abfb12d4f65aa025edc9feefb4"></a>
#### （4）通过 `Phaser` 实现分层
```
public class PhaserExample {
    public static void main(String[] args) {
        final int parties = 3;
        final int phases = 4;
        Phaser phaser = new Phaser() {
            @Override
            protected boolean onAdvance(int phase, int registeredParties) {
                System.out.println("====== Phase :" + phase + "======");
                return super.onAdvance(phase, registeredParties);
            }
        };
        ExecutorService executorService = Executors.newCachedThreadPool();
        for (int i = 0; i < parties; i++) {
            // 注册各个参与者线程
            phaser.register();
            executorService.execute(() -> {
                for (int phase = 0; phase < phases; phase++) {
                    // 等待其它参与者线程到达 [arriveAndAwaitAdvance 方法是不响应中断的，也就是说即使当前线程被中断，arriveAndAwaitAdvance 方法也不会返回或抛出异常，而是继续等待。如果希望能够响应中断，可以参考 awaitAdvanceInterruptibly 方法]
                    int j = phaser.arriveAndAwaitAdvance();
                    // do something
                    System.out.println(String.format("currentThread:%s, Executing the task, currentPhase:%s", Thread.currentThread().getName(), j));
                }
            });
        }
        executorService.shutdown();
    }
}
// Output
// ====== Phase : 0 ======
// currentThread:pool-1-thread-1, Executing the task, currentPhase:1
// currentThread:pool-1-thread-2, Executing the task, currentPhase:1
// currentThread:pool-1-thread-3, Executing the task, currentPhase:1
// ====== Phase : 1 ======
// currentThread:pool-1-thread-3, Executing the task, currentPhase:2
// currentThread:pool-1-thread-1, Executing the task, currentPhase:2
// currentThread:pool-1-thread-2, Executing the task, currentPhase:2
// ====== Phase : 2 ======
// currentThread:pool-1-thread-2, Executing the task, currentPhase:3
// currentThread:pool-1-thread-1, Executing the task, currentPhase:3
// currentThread:pool-1-thread-3, Executing the task, currentPhase:3
// ====== Phase : 3 ======
// currentThread:pool-1-thread-3, Executing the task, currentPhase:4
// currentThread:pool-1-thread-1, Executing the task, currentPhase:4
// currentThread:pool-1-thread-2, Executing the task, currentPhase:4
```
