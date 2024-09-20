Java
<a name="lYG0c"></a>
## 概览
CountDownLatch是JDK提供的一个同步工具，它可以让一个或多个线程等待，一直等到其他线程中执行完成一组操作。<br />CountDownLatch 基于AQS构建同步器：<br />AQS - `AbstractQueuedSynchronizer` ，即抽象的队列同步器，是一种用来构建锁和同步器的框架。<br />使用 `LockSupport.park(this);` 挂起线程，最后会调用到 `UnSafe`的`park`，然后调用到`native`层。
> 注意：计数器值递减到0的时候，不能再复原。

<a name="Zs0hn"></a>
### 2.1 作用
构建锁和同步器，进行线程的等待。
<a name="pqL34"></a>
### 2.2 使用场景
`CountDownLatch`适用于在多线程的场景需要等待所有子线程全部执行完毕之后再做操作的场景。<br />`CountDownLatch`可以理解为并发计数器，主要的使用场景是当一个任务被拆分成多个子任务时，需要等待子任务全部完成后再操作，不然会阻塞线程(当前线程)，每完成一个任务计数器会 -1，直到没有。<br />一般用作多线程倒计时计数器，强制它们等待其他一组任务，计数器的减法是一个不可逆的过程。
> eg：人齐了再吃饭，人不齐不能动筷子，大家都坐那等着。
> 开会等人，人没到齐的时候，大家也都坐那等着。
> 启动顺序，启动顺序的先后及依赖关系。

来一段代码：
```java
public class Waitress implements Runnable {
    private CountDownLatch latch;
    private String name;

    public Waitress(CountDownLatch latch, String name) {
        this.latch = latch;
        this.name = name;
    }

    @Override
    public void run() {
        try {
            SimpleDateFormat sdf = new SimpleDateFormat("HH:mm:ss.SSS");
            System.out.println(sdf.format(new Date()) + " " + name  + "等待顾客");
            latch.await();
            System.out.println(sdf.format(new Date()) + " " + name  + "开始上菜");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```
顾客类
```java
public class Customer implements Runnable {
    // 主线程 countdown
    private CountDownLatch mainThreadLatch;

    // 当前线程 countdown
    private CountDownLatch latchThread;
    private String name;

    public Customer(CountDownLatch mainThreadLatch, String name) {
        this.mainThreadLatch = mainThreadLatch;
        this.name = name;
        latchThread = new CountDownLatch(1);
        try {
            // 阻塞当前线程 1s
            latchThread.await(1000, TimeUnit.MILLISECONDS);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void run() {
        try {
            SimpleDateFormat sdf = new SimpleDateFormat("HH:mm:ss.SSS");
            Random random = new Random();

            System.out.println(sdf.format(new Date()) + " " + name + "出发去饭店");
            Thread.sleep((long) (random.nextDouble() * 3000) + 1000);
            System.out.println(sdf.format(new Date()) + " " + name + "到了饭店");
            mainThreadLatch.countDown();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```
测试代码
```java
public static void main(String[] args) throws InterruptedException {
    CountDownLatch latch = new CountDownLatch(3);

    List<Thread> threads = new ArrayList<>(3);
    threads.add(new Thread(new Customer(latch, "张三")));
    threads.add(new Thread(new Customer(latch, "李四")));
    threads.add(new Thread(new Customer(latch, "王五")));
    for (Thread thread : threads) {
        thread.start();
    }

    Thread.sleep(100);
    new Thread(new Waitress(latch, "♥小芳♥")).start();
}
```
<a name="Z8zKw"></a>
### 2.3 优劣

- 优点 ：代码优雅，不需要对线程池进行操作，将线程池作为 Bean 的情况下有很好的使用场景。
- 缺点 ：需要提前知道线程数量；性能不强。还需要在线程代码块内加上异常判断，否则在 `countDown` 之前发生异常而没有处理，就会导致主线程永远阻塞在 await。

`CountDownLatch` 相比于 `join()` 方法，提供了更多的API，更加灵活。
<a name="kXDB4"></a>
## 三、原理
<a name="fIcXU"></a>
### 3.1 原理
`CountDownLatch`就是利用了 AQS - `AbstractQueuedSynchronizer` 的共享锁来实现。<br />同时使用 CAS（Compare And Swap） 对计数器进行操作。<br />再使用 `volatile` 修饰的变量state维持倒数状态，使多线程共享变量可见。<br />AQS的数据结构核心就是两个虚拟队列：同步队列sync queue 和条件队列condition queue，不同的条件会有不同的条件队列。<br />我们看下源码：
```java
// 1、 创建CountDownLatch并设置计数器值，count代表计数器个数
//（内部是共享锁，本质就是上了几次锁）
public CountDownLatch(int count)

// 2、启动多线程并且调用CountDownLatch实例的countDown()方法
// 每countDown一次，计数器就减一，就是释放了一次共享锁，直到为0全部结束
//调用countDown的线程可以继续执行，不需要等待计数器被减到0，
//只是调用await方法的线程需要等待。
public void countDown()

// 3、 主线程调用 await() 方法，这样主线程的操作就会在这个方法上阻塞，
//直到count值为0，停止阻塞，主线程继续执行
// 在AQS队列里一直等待，直到countDown减到0，才会继续往下执行，
// 使用 LockSupport.park(this);  挂起线程
//方法在倒计数为0之前会阻塞 = 当前线程 =
public void await()

// 等待一定时间
public void await(long timeout, TimeUnit unit)
```
子类的任务有：

- 通过CAS操作维护共享变量state。
- 重写资源的获取方式。
- 重写资源释放的方式。

`CountDownLatch`有一个内部类叫做Sync（sɪŋk），它继承了`AbstractQueuedSynchronizer`类，其中维护了一个整数 state，并且保证了修改state的可见性和原子性。<br />在创建`CountDownLatch`实例时，也会创建一个`Sync`的实例，同时把计数器的值传给`Sync`实例，具体是这样的：
```java
public CountDownLatch(int count) {
    if (count < 0) throw new IllegalArgumentException("count < 0");
    this.sync = new Sync(count);
}
```
在 `countDown` 方法中，只调用了`Sync`实例的 `releaseShared`方法，具体是这样的：
```java
public void countDown() {
    sync.releaseShared(1);
}
```
其中的 `releaseShared` 方法，先对计数器进行减1操作，如果减1后的计数器为0，唤醒被 await 方法阻塞的所有线程，具体是这样的：
```java
public final boolean releaseShared(int arg) {
    if (tryReleaseShared(arg)) { //对计数器进行减一操作
        doReleaseShared();//如果计数器为0，唤醒被await方法阻塞的所有线程
        return true;
    }
    return false;
}
```
其中的`tryReleaseShared`方法，先获取当前计数器的值，如果计数器为0时，就直接返回；如果不为0时，使用 CAS（Compare And Swap） 方法对计数器进行减1操作，具体是这样的：
```java
protected boolean tryReleaseShared(int releases) {
    for (;;) {//死循环，如果CAS操作失败就会不断继续尝试。  自旋不断判断是否释放了state同步锁
        int c = getState();//获取当前计数器的值。
        if (c == 0)// 计数器为0时，就直接返回。说明之前已经释放了同步锁，这时候就不需要做任何操作了，因为之前已经做完了
            return false;
            
        // state - 1释放一次同步锁
        int nextc = c-1;
        
        // 通过CAS设置state同步状态，如果成功判断state是否为0，为0代表锁全部释放
        // 被await的程序可以继续往下执行了
        if (compareAndSetState(c, nextc))// 使用CAS方法对计数器进行减1操作
            return nextc == 0;//如果操作成功，返回计数器是否为0
    }
}
```
<a name="jHpii"></a>
### 3.2 CountDownLatch 是如何阻塞线程的
线程阻塞原理，在`await`方法中，调用了`Sync`实例的`acquireSharedInterruptibly`方法：
```java
// 创建一个节点，加入到AQS阻塞队列，并同时把当前线程挂起
public void await() throws InterruptedException {
    sync.acquireSharedInterruptibly(1);
}
```
其中`acquireSharedInterruptibly`方法，判断计数器是否为0，如果不为0则阻塞当前线程，具体是这样的：
```java
public final void acquireSharedInterruptibly(int arg)
        throws InterruptedException {
    if (Thread.interrupted())
        throw new InterruptedException();
    if (tryAcquireShared(arg) < 0)//判断计数器是否为0
        doAcquireSharedInterruptibly(arg);//如果不为0 则阻塞当前线程
}


private void doAcquireSharedInterruptibly(int arg)
    throws InterruptedException {
    // 以共享模式添加到等待队列 ，新建节点加入阻塞队列   
    final Node node = addWaiter(Node.SHARED);
    boolean failed = true;
    try {
        for (;;) {
            // 返回前一个节点
            final Node p = node.predecessor();
            if (p == head) {
                int r = tryAcquireShared(arg); //返回锁的state

                if (r >= 0) {
                    setHeadAndPropagate(node, r);
                    p.next = null;
                    failed = false;
                    return;
                }
            }
            // 检查并更新未能获取的节点的状态。如果线程应该阻塞，则返回 true
            if (shouldParkAfterFailedAcquire(p, node) &&
                parkAndCheckInterrupt())
                throw new InterruptedException();
        }
    } finally {
        // 失败就取消
        if (failed)
            cancelAcquire(node);
    }
}

// 挂起线程
private final boolean parkAndCheckInterrupt() {
    LockSupport.park(this);
    return Thread.interrupted();
}
```
其中`tryAcquireShared`方法，是`AbstractQueuedSynchronizer`中的一个模板方法，其具体实现在`Sync`类中，其主要是判断计数器是否为零，如果为零则返回1，如果不为零则返回-1，具体是这样的：
```java
// await判断共享锁是否全部释放，是则从队列中移除，继续往下执行，实现AQS的模板方法
protected int tryAcquireShared(int acquires) {
    return (getState() == 0) ? 1 : -1;
}
```
