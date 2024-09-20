<a name="RmEQ2"></a>
## 一、简介
使用`synchronized`关键字可以实现线程同步安全的效果，以及采用`wait()`、`notify()`和`notifyAll()`方法，可以实现多个线程之间的通信协调，基本可以满足并发编程的需求。<br />但是采用`synchronized`进行加锁，这种锁一般都比较重，里面的实现机制也非常复杂，同时获取锁时必须一直等待，没有额外的尝试机制，如果编程不当，可能就容易发生死锁现象。<br />从 JDK 1.5 开始，引入了一个高级的处理并发的java.util.concurrent包，它提供了大量更高级的并发功能，能大大的简化多线程程序的编写。<br />比如这里要介绍的java.util.concurrent.locks包提供的`ReentrantLock`类，一个可重入的互斥锁，它具有与使用`synchronized`加锁一样的特性，并且功能更加强大。<br />下面一起来学习一下`ReentrantLock`类的基本使用。
<a name="qlqZD"></a>
## 二、`ReentrantLock` 基本用法
在介绍`ReentrantLock`之前，先来看一下传统的使用`synchronized`对方法进行加锁的示例。
```java
public class Counter {

    private int count;

    public void add() {
        synchronized(this) {
            count ++;
            System.out.println("ThreadName：" + Thread.currentThread().getName() + ", count：" + getCount());
        }
    }

    public int getCount() {
        return count;
    }
}
```
```java
public static void main(String[] args) throws InterruptedException {
    Counter counter = new Counter();

    // 创建5个线程，同时对count进行加一操作
    for (int i = 0; i < 5; i++) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                counter.add();
            }
        }).start();
    }

    // 假设休眠1秒，5个线程执行完毕
    Thread.sleep(1000);
    System.out.println("count：" + counter.getCount());
}
```
输出结果如下：
```
ThreadName：Thread-0, count：1
ThreadName：Thread-1, count：2
ThreadName：Thread-2, count：3
ThreadName：Thread-3, count：4
ThreadName：Thread-4, count：5
count：5
```
如果用`ReentrantLock`替代，只需要将`Counter`中的代码改造为如下：
```java
public class Counter {

    private final Lock lock = new ReentrantLock();

    private int count;

    public void add() {
        // 加锁
        lock.lock();
        try {
            count ++;
            System.out.println("ThreadName：" + Thread.currentThread().getName() + ", count：" + getCount());
        } finally {
            // 释放锁
            lock.unlock();
        }
    }

    public int getCount() {
        return count;
    }
}
```
运行程序，结果与上面一致，可以证明：`ReentrantLock`具备与`synchronized`一样的加锁功能。<br />同时，`ReentrantLock`还具备在指定的时间内尝试获取锁的机制，比如下面这行代码：
```java
if (lock.tryLock(3, TimeUnit.SECONDS)) {
    try {
        ...
    } finally {
        lock.unlock();
    }
}
```
尝试在 3 秒内获取锁，如果获取不到就返回false，程序不需要无限等待下去，这个功能在实际开发中使用非常的广泛。<br />从上面的示例代码，可以总结出`synchronized`和`ReentrantLock`有以下几点不一样。

- `ReentrantLock`需要手动调用加锁方法；而`synchronized`不需要，它采用了隐藏的加锁方式，借助 jvm 来实现
- `synchronized`不需要考虑异常；而`ReentrantLock`获取锁之后，要在`finally`中正确的释放锁，否则会影响其它线程
- `ReentrantLock`拥有尝试获取锁的超时机制，利用它可以避免无限等待；而`synchronized`不具备
- `synchronized`是 Java 语言层面提供的语法；而`ReentrantLock`是 Java 代码实现的可重入锁

因此，在并发编程中，使用`ReentrantLock`比直接使用`synchronized`更灵活、更安全，采用`tryLock(long time, TimeUnit unit)`方法，即使未获取到锁也不会导致死锁。
<a name="OHGIu"></a>
## 三、`ReentrantLock` 和 `synchronized` 持有的对象监视器是同一个吗？
可能有的同学会发出这样的一个问题，使用`ReentrantLock`进行加锁和使用`synchronized`加锁，两者持有的对象监视器是同一个吗？<br />下面一起来看一个例子。
```java
public class Counter {

    private final Lock lock = new ReentrantLock();

    private int count;


    public synchronized void methodA() {
        System.out.println("ThreadName：" + Thread.currentThread().getName() + "，begin methodA, count：" + getCount());
        try {
            Thread.sleep(3000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        count ++;
        System.out.println("ThreadName：" + Thread.currentThread().getName() + ", count：" + getCount());

    }

    public void methodB() {
        // 加锁
        lock.lock();
        try {
            System.out.println("ThreadName：" + Thread.currentThread().getName() + "，begin methodB, count：" + getCount());
            Thread.sleep(3000);
            count ++;
            System.out.println("ThreadName：" + Thread.currentThread().getName() + ", count：" + getCount());
        } catch (Exception e){
            e.printStackTrace();
        } finally {
            // 释放锁
            lock.unlock();
        }
    }

    public int getCount() {
        return count;
    }
}
```
```java
public class MyThreadA extends Thread {

    private Counter counter;

    public MyThreadA(Counter counter) {
        this.counter = counter;
    }

    @Override
    public void run() {
        counter.methodA();
    }
}
```
```java
public class MyThreadB extends Thread {

    private Counter counter;

    public MyThreadB(Counter counter) {
        this.counter = counter;
    }

    @Override
    public void run() {
        counter.methodB();
    }
}
```
```java
public class MyThreadTest {

    public static void main(String[] args) {
        Counter counter = new Counter();

        MyThreadA threadA = new MyThreadA(counter);
        threadA.start();

        MyThreadB threadB = new MyThreadB(counter);
        threadB.start();
    }
}
```
看一下运行结果：
```
ThreadName：Thread-0，begin methodA, count：0
ThreadName：Thread-1，begin methodB, count：0
ThreadName：Thread-0, count：2
ThreadName：Thread-1, count：2
```
从日志上可以看出，采用两个线程分别采用`synchronized`和`ReentrantLock`两种加锁方式对`count`进行操作，两个线程交替执行，可以得出一个结论：`synchronized`和`ReentrantLock`持有的对象监视器不同。
<a name="FsLpG"></a>
## 四、`Condition` 基本用法
在`synchronized`同步方法／代码块中，使用`wait()`、`notify()`和`notifyAll()`可以实现线程之间的等待／通知模型。<br />`ReentrantLock`同样也可以，只需要借助`Condition`类即可实现，`Condition`提供的`await()`、`signal()`、`signalAll()`原理和`synchronized`锁对象的`wait()`、`notify()`、`notifyAll()`是一致的，并且其行为也是一样的。<br />还是先来看一个简单的示例。
```java
public class Counter {

    private final Lock lock = new ReentrantLock();

    private Condition condition = lock.newCondition();

    private int count;

    public void await(){
        // 加锁
        lock.lock();
        try {
            condition.await();
            System.out.println("await等待结束，count：" + getCount());
        } catch (Exception e){
            e.printStackTrace();
        } finally {
            // 释放锁
            lock.unlock();
        }
    }


    public void signal(){
        // 加锁
        lock.lock();
        try {
            count++;
            // 唤醒某个等待线程
            condition.signal();
            // 唤醒所有等待线程
            //            condition.signalAll();
            System.out.println("signal 唤醒通知完毕");
        } catch (Exception e){
            e.printStackTrace();
        } finally {
            // 释放锁
            lock.unlock();
        }
    }

    public int getCount() {
        return count;
    }
}
```
```java
public class MyThreadA extends Thread {

    private Counter counter;

    public MyThreadA(Counter counter) {
        this.counter = counter;
    }

    @Override
    public void run() {
        counter.await();
    }
}
```
```java
public class MyThreadB extends Thread {

    private Counter counter;

    public MyThreadB(Counter counter) {
        this.counter = counter;
    }

    @Override
    public void run() {
        counter.signal();
    }
}
```
```java
public class MyThreadTest {

    public static void main(String[] args) throws InterruptedException {
        Counter counter = new Counter();

        // 先启动执行等待的线程
        MyThreadA threadA = new MyThreadA(counter);
        threadA.start();

        Thread.sleep(3000);

        // 过3秒，再启动执行通知的线程
        MyThreadB threadB = new MyThreadB(counter);
        threadB.start();
    }
}
```
看一下运行结果：
```
signal 唤醒通知完毕
await等待结束，count：1
```
从结果上看很明显的看出，等待线程MyThreadA先启动，过了 3 秒之后再启动了MyThreadB，但是`signal()`方法先执行完毕，再通知`await()`方法执行，符合代码预期。<br />这个例子也证明了一点：`condition.await()`方法是释放了锁，不然`signal()`方法体不会被执行。<br />相比wait/notify/notifyAll的等待／通知模型，`Condition`更加灵活，理由有以下几点：

- `notify()`方法唤醒线程时，被通知的线程由 Java 虚拟机随机选择；而采用`ReentrantLock`结合`Condition`可以实现有选择性地通知，这一特性在实际编程中非常实用
- 一个`Lock`里面可以创建多个`Condition`实例，实现多路通知，使用多个`Condition`的应用场景很常见，比如`ArrayBlockingQueue`
<a name="yydh5"></a>
## 五、常用方法介绍
<a name="IsA09"></a>
### 5.1、构造方法
ReentrantLock类有两个构造方法，核心源码内容如下：
```java
/**
 * 默认创建非公平锁
 */
public ReentrantLock() {
    sync = new NonfairSync();
}
```
```java
/**
 * fair为true表示是公平锁，fair为false表示是非公平锁
 */
public ReentrantLock(boolean fair) {
    sync = fair ? new FairSync() : new NonfairSync();
}
```
相比于`synchronized`同步锁，`ReentrantLock`有一个很大的特点，就是开发人员可以手动指定采用公平锁机制还是非公平锁机制。<br />公平锁：顾名思义，就是每个线程获取锁的顺序是按照线程排队的顺序来分配的，最前面的线程总是最先获取到锁。

- 优点：所有的线程都有机会得到资源
- 缺点：公平锁机制实现比较复杂，程序流程比较多，执行速度比较慢

非公平锁：每个线程获取锁的顺序是随机的，并不会遵循先来先得的规则，任何线程在某时刻都有可能直接获取并拥有锁，之前介绍的`synchronized`其实就是一种非公平锁

- 优点：公平锁机制实现相对比较简单，程序流程比较少，执行速度比较快
- 缺点：有可能某些线程一直拿不到锁，导致饿死

`ReentrantLock`默认的构造方法是非公平锁，如果想要构造公平锁机制，只需要传入true就可以了。<br />示例代码如下：
```java
public static void main(String[] args) {
    // 创建公平锁实现机制
    Lock lock = new ReentrantLock(true);

    // 创建5个线程
    for (int i = 0; i < 5; i++) {
        new Thread(new Runnable() {

            @Override
            public void run() {
                System.out.println("ThreadName：" + Thread.currentThread().getName() + ", 启动了!");

                // 尝试获取锁
                lock.lock();
                try {
                    System.out.println("ThreadName：" + Thread.currentThread().getName() + ", 获得锁!");
                } finally {
                    lock.unlock();
                }
            }
        }).start();
    }
}
```
运行一下程序，结果如下：
```
ThreadName：Thread-0, 启动了!
ThreadName：Thread-1, 启动了!
ThreadName：Thread-0, 获得锁!
ThreadName：Thread-1, 获得锁!
ThreadName：Thread-2, 启动了!
ThreadName：Thread-2, 获得锁!
ThreadName：Thread-3, 启动了!
ThreadName：Thread-3, 获得锁!
ThreadName：Thread-4, 启动了!
ThreadName：Thread-4, 获得锁!
```
从日志上可以看到，启动顺序为0,1,2,3,4，获取锁的顺序为0,1,2,3,4，启动与获取锁的排队机制一致。<br />假如构造方法里面的把true改成false，也就是非公平锁机制，在看看运行效果，结果如下：
```
ThreadName：Thread-1, 启动了!
ThreadName：Thread-2, 启动了!
ThreadName：Thread-1, 获得锁!
ThreadName：Thread-0, 启动了!
ThreadName：Thread-2, 获得锁!
ThreadName：Thread-3, 启动了!
ThreadName：Thread-3, 获得锁!
ThreadName：Thread-0, 获得锁!
ThreadName：Thread-4, 启动了!
ThreadName：Thread-4, 获得锁!
```
从日志上可以看到，启动顺序为1,2,0,3,4，获取锁的顺序为1,2,3,0,4，线程启用与获取锁的顺序不一致。<br />从实际的运行结果看，非公平锁要比公平锁执行速度要快一些，当线程数越多的时候，效果越明显。
<a name="XE0au"></a>
### 5.2、核心方法
`ReentrantLock`类的核心方法就比较多了，如下表！

| 方法 | 描述 |
| --- | --- |
| `public void lock()` | 阻塞等待获取锁；不允许Thread.interrupt中断，即使检测到Thread.isInterrupted一样会继续尝试 |
| `public void lockInterruptibly()` | 当前线程未被中断，则获取锁；允许在等待时由其它线程调用等待线程的Thread.interrupt方法来中断等待线程的等待而直接返回 |
| `public boolean tryLock()` | 尝试申请一个锁，在成功获得锁后返回true，否则，立即返回false |
| `public boolean tryLock(long timeout, TimeUnit unit)` | 在一段时间内尝试申请一个锁，在成功获得锁后返回true，否则，立即返回false |
| `public void unlock()` | 释放锁 |
| `public Condition newCondition()` | 条件实例，用于线程等待/通知模式 |
| `public int getHoldCount()` | 获取当前线程持有此锁的次数 |
| `public boolean isHeldByCurrentThread()` | 检测是否被当前线程持有 |
| `public boolean isLocked()` | 查询此锁是否由任意线程持有 |
| `public final boolean isFair()` | 如果是公平锁返回true，否则返回false |
| `public final boolean hasQueuedThreads()` | 查询是否有线程正在等待 |
| `public final boolean hasQueuedThread(Thread thread)` | 查询给定线程是否正在等待获取此锁 |
| `public final int getQueueLength()` | 获取正等待获取此锁的线程数 |
| `public boolean hasWaiters(Condition condition)` | 是否存在正在等待并符合相关给定条件的线程 |
| `public int getWaitQueueLength(Condition condition)` | 正在等待并符合相关给定条件的线程数量 |

虽然方法很多，但是实际上常用方法就那么几个，下面主要抽一些常用的方法进行介绍。
<a name="BnsJU"></a>
#### 5.2.1、tryLock 方法
`lock()`、`lockInterruptibly()`、`tryLock()`和`tryLock(long timeout, TimeUnit unit)`这几个方法，目的其实是一样的，都是为了获取锁，只是针对不同的场景做了单独的处理。<br />`lock()`：阻塞等待获取锁，如果没有获取到会一直阻塞，即使检测到`Thread.isInterrupted`一样会继续尝试；

- `lockInterruptibly()`：同样也是阻塞等待获取锁，稍有不同的是，允许在等待时由其它线程调用等待线程的`Thread.interrupt`方法来中断等待线程的等待而直接返回
- `tryLock()`：表示尝试申请一个锁，在成功获得锁后返回true，否则，立即返回false，不会阻塞等待获取锁
- `tryLock(long timeout, TimeUnit unit)`：表示在一段时间内尝试申请一个锁，在成功获得锁后返回true，否则，立即返回false

其中`tryLock(long timeout, TimeUnit unit)`方法的应用最广泛，因为它能防止程序发生死锁，即使在一段时间内没有获取锁，也会自动退出，不会一直阻塞。<br />可以看一个简单的例子，如下！
```java
public static void main(String[] args) {
    // 创建公平锁实现机制
    Lock lock = new ReentrantLock();

    // 创建5个线程
    for (int i = 0; i < 5; i++) {
        new Thread(new Runnable() {

            @Override
            public void run() {
                boolean flag = false;
                try {
                    // 尝试3秒内获取锁
                    flag = lock.tryLock(3, TimeUnit.SECONDS);
                    if(flag){
                        System.out.println("ThreadName：" + Thread.currentThread().getName() + ", 获取到锁");
                        // 模拟进行5秒的业务操作
                        Thread.sleep(5000);
                    } else {
                        System.out.println("ThreadName：" + Thread.currentThread().getName() + ", 经过3秒钟的尝试未获取到锁，放弃尝试");
                    }
                } catch (InterruptedException e) {
                    e.printStackTrace();
                } finally {
                    if (flag){
                        System.out.println("ThreadName：" + Thread.currentThread().getName() + ", 释放对象");
                        lock.unlock();
                    }
                }
            }
        }).start();
    }
}
```
运行一下程序，结果如下：
```
ThreadName：Thread-0, 获取到锁
ThreadName：Thread-3, 经过3秒钟的尝试未获取到锁，放弃尝试
ThreadName：Thread-1, 经过3秒钟的尝试未获取到锁，放弃尝试
ThreadName：Thread-2, 经过3秒钟的尝试未获取到锁，放弃尝试
ThreadName：Thread-4, 经过3秒钟的尝试未获取到锁，放弃尝试
ThreadName：Thread-0, 释放对象
```
可以很清晰的看到，非Thread-0线程尝试了 3 秒没有获取到锁，自动放弃；如果换成lock()方法进行获取锁，线程Thread-0如果不释放锁，其它线程会一直阻塞。
<a name="nYCep"></a>
#### 5.2.2、`unlock` 方法
`unlock()`方法也是常用方法，表示释放锁。当获取到锁之后，一定要手动释放锁，否则可能会造成其它程序执行出现问题，通常用在`finally`方法块里面。
```java
// 阻塞等待获取锁
lock.lock();
try {
    // 业务操作...
} finally {
    // 一定要释放锁
    lock.unlock();
}
```
<a name="keq1h"></a>
#### 5.2.2、`newCondition` 方法
`newCondition()`方法，在上文中介绍过，`ReentrantLock`和`Condition`结合，可以实现线程之间的等待／通知模型。<br />简单的示例，如下！
```java
public class Counter {

    private final Lock lock = new ReentrantLock();

    private Condition condition = lock.newCondition();

    private int count;

    public void await(){
        // 加锁
        lock.lock();
        try {
            // 让当前线程进入等待状态，并释放锁
            condition.await();
            System.out.println("await等待结束，count：" + getCount());
        } catch (Exception e){
            e.printStackTrace();
        } finally {
            // 释放锁
            lock.unlock();
        }
    }


    public void signal(){
        // 加锁
        lock.lock();
        try {
            count++;
            // 唤醒某个等待线程
            condition.signal();
            System.out.println("signal 唤醒通知完毕");
        } catch (Exception e){
            e.printStackTrace();
        } finally {
            // 释放锁
            lock.unlock();
        }
    }

    public int getCount() {
        return count;
    }
}
```
```java
public class MyThreadTest {

    public static void main(String[] args) throws InterruptedException {
        Counter counter = new Counter();

        // 先启动执行等待的线程
        new Thread(new Runnable() {
            @Override
            public void run() {
                counter.await();
            }
        }).start();

        Thread.sleep(3000);

        // 过3秒，再启动执行通知的线程
        new Thread(new Runnable() {
            @Override
            public void run() {
                counter.signal();
            }
        }).start();
    }
}
```
运行一下程序，结果如下：
```
signal 唤醒通知完毕
await等待结束，count：1
```
<a name="E3vLY"></a>
#### 5.2.3、`getHoldCount` 方法
`getHoldCount()`方法的作用是返回的是当前线程调用`lock()`的次数。<br />示例代码如下：
```java
public static void main(String[] args) {
    ReentrantLock lock = new ReentrantLock();

    new Thread(new Runnable() {

        @Override
        public void run() {
            // 第一次获取锁
            lock.lock();
            try {
                System.out.println("ThreadName：" + Thread.currentThread().getName() + ", getHoldCount：" +  lock.getHoldCount());

                // 第二次获取锁
                lock.lock();
                try {
                    System.out.println("ThreadName：" + Thread.currentThread().getName() + ", getHoldCount：" +  lock.getHoldCount());
                } finally {
                    lock.unlock();
                }
            } finally {
                lock.unlock();
            }
        }
    }).start();
}
```
运行一下程序，结果如下：
```
ThreadName：Thread-0, getHoldCount：1
ThreadName：Thread-0, getHoldCount：2
```
侧面也证明了一点，`ReentrantLock`和`synchronized`一样，锁都具有**可重入**特性，也就是说同一个线程多次调用同一个`ReentrantLock`的`lock()`方法，可以再次进入方法体，无需阻塞等待。
<a name="tZulM"></a>
#### 5.2.4、`isLocked` 方法
`isHeldByCurrentThread()`和`isLocked()`方法都是用于检测锁是否被持有。<br />其中`isHeldByCurrentThread()`方法表示此锁是否由当前线程持有；`isLocked()`方法表示此锁是否由任意线程持有。<br />看一个简单的示例，如下：
```java
public class Counter {

    private ReentrantLock lock = new ReentrantLock();

    public void methodA(){
        lock.lock();
        try {
            System.out.println("ThreadName：" + Thread.currentThread().getName() + ", 当前线程是否持有锁:" +  lock.isHeldByCurrentThread());
            System.out.println("ThreadName：" + Thread.currentThread().getName() + ", 任意线程是否持有锁:" +  lock.isLocked());
            Thread.sleep(1000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        } finally {
            lock.unlock();
        }
    }

    public void methodB(){
        System.out.println("ThreadName：" + Thread.currentThread().getName() + ", 当前线程是否持有锁:" +  lock.isHeldByCurrentThread());
        System.out.println("ThreadName：" + Thread.currentThread().getName() + ", 任意线程是否持有锁:" +  lock.isLocked());
    }
}
```
```java
public class MyThreadTest {

    public static void main(String[] args) throws InterruptedException {
        Counter counter = new Counter();

        new Thread(new Runnable() {
            @Override
            public void run() {
                counter.methodA();
            }
        }).start();

        new Thread(new Runnable() {
            @Override
            public void run() {
                counter.methodB();
            }
        }).start();
    }
}
```
运行一下程序，结果如下：
```
ThreadName：Thread-0, 当前线程是否持有锁:true
ThreadName：Thread-0, 任意线程是否持有锁:true
ThreadName：Thread-1, 当前线程是否持有锁:false
ThreadName：Thread-1, 任意线程是否持有锁:true
```
从日志结果很容易理解，Thread-0线程持有锁，因此调用`isHeldByCurrentThread()`和`isLocked()`方法，返回结果都是true；Thread-1线程没有持有锁，因此`isHeldByCurrentThread()`方法返回false，`isLocked()`方法返回true
<a name="ImLva"></a>
#### 5.2.5、isFair 方法
`isFair()`方法用来获取此锁是否公平锁。<br />简单的示例，如下：
```java
ReentrantLock lock = new ReentrantLock(true);
System.out.println("是否公平锁:" +  lock.isFair());
```
输出结果如下：
```
是否公平锁:true
```
`ReentrantLock`默认的是非公平锁，当通过构造方法显式传入true时，采用的是公平锁机制
<a name="sgwjh"></a>
#### 5.2.6、`hasQueuedThreads` 方法
`hasQueuedThreads()`和`hasQueuedThread()`方法都用于查询是否有线程等待获取锁，稍有不同的是：`hasQueuedThreads()`方法表示查询是否有线程正在等待获取锁；`hasQueuedThread()`方法表示查询给定线程是否正在等待获取此锁。<br />另外还有一个`getQueueLength()`方法，表示获取正等待获取此锁的线程数。<br />看一个简单的示例，如下：
```java
public static void main(String[] args) throws InterruptedException {
    ReentrantLock lock = new ReentrantLock();

    Thread threadA = new Thread(new Runnable() {
        @Override
        public void run() {
            lock.lock();
            try {
                Thread.sleep(5000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            } finally {
                lock.unlock();
            }
        }
    });
    threadA.start();

    Thread threadB = new Thread(new Runnable() {
        @Override
        public void run() {
            lock.lock();
            try {
                Thread.sleep(5000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            } finally {
                lock.unlock();
            }
        }
    });
    threadB.start();

    // 等待线程都启动完毕
    Thread.sleep(1000);

    System.out.println("查询是否有线程正在等待：" + lock.hasQueuedThreads());
    System.out.println("查询处于等待的线程数：" + lock.getQueueLength());
    System.out.println("threadA 是否处于等待状态：" +  lock.hasQueuedThread(threadA));
    System.out.println("threadB 是否处于等待状态：" +  lock.hasQueuedThread(threadB));
}
```
输出结果如下：
```
查询是否有线程正在等待：true
查询处于等待的线程数：1
threadA 是否处于等待状态：false
threadB 是否处于等待状态：true
```
从日志上可以清晰的看到，线程threadA先获取了锁，线程threadB处于等待获取锁的状态，处于等待的线程数为1。
<a name="tqPtI"></a>
#### 5.2.7、`hasWaiters` 方法
`hasWaiters()`和`getWaitQueueLength()`方法，支持传入`condition`条件对象进行查询。<br />其中`hasWaiters()`方法表示查询是否存在正在等待并符合相关给定条件的线程；`getWaitQueueLength()`方法表示查询正在等待并符合相关给定条件的线程数量。<br />看一个简单的示例，如下：
```java
public static void main(String[] args) throws InterruptedException {
    ReentrantLock lock = new ReentrantLock();
    Condition condition = lock.newCondition();

    Thread threadA = new Thread(new Runnable() {
        @Override
        public void run() {
            lock.lock();
            try {
                condition.await();
                System.out.println("await等待结束");
            } catch (InterruptedException e) {
                e.printStackTrace();
            } finally {
                lock.unlock();
            }
        }
    });
    threadA.start();

    // 睡眠1秒
    Thread.sleep(1000);

    Thread threadB = new Thread(new Runnable() {
        @Override
        public void run() {
            lock.lock();
            try {
                System.out.println("是否存在正在等待并符合相关给定条件的线程：" + lock.hasWaiters(condition));
                System.out.println("正在等待并符合相关给定条件的线程数量：" + lock.getWaitQueueLength(condition));
                Thread.sleep(5000);
                condition.signal();
                System.out.println("signal 唤醒通知完毕");
            } catch (InterruptedException e) {
                e.printStackTrace();
            } finally {
                lock.unlock();
            }
        }
    });
    threadB.start();
}
```
输出结果如下：
```
是否存在正在等待并符合相关给定条件的线程：true
正在等待并符合相关给定条件的线程数量：1
signal 唤醒通知完毕
await等待结束
```
需要注意的是，调用`condition`对象的方法，必须要在获取锁的方法体内执行。
<a name="HmEht"></a>
## 六、小结
本文主要围绕`ReentrantLock`类的核心方法进行了一些知识总结，其中最常用方法的主要就两个，`tryLock(long timeout, TimeUnit unit)`和`unlock()`，通过它可以实现线程同步安全的效果。
