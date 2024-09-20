Java CyclicBarrier CountDownLatch
<a name="y3kvf"></a>
## 前言
`CyclicBarrier`和`CountDownLatch`这两个工具都是在java.util.concurrent包下，并且平时很多场景都会使用到。<br />本文将会对两者进行分析，记录他们的用法和区别。
<a name="IDS6d"></a>
## `CountDownLatch`
`CountDownLatch`是一个非常实用的多线程控制工具类，称之为“倒计时器”，它允许一个或多个线程一直等待，直到其他线程的操作执行完后再执行。<br />`CountDownLatch`是通过一个计数器来实现的，计数器的初始值为线程的数量。每当一个线程完成了自己的任务后，计数器的值就会减1。当计数器值到达0时，它表示所有的线程已经完成了任务，然后在闭锁上等待的线程就可以恢复执行任务。<br />![2021-08-30-12-06-28-810564.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630296569227-5b0a7a01-be03-4c1a-825d-bf48c984a11b.png#clientId=u72f468dd-f541-4&from=ui&id=ud331528c&originHeight=858&originWidth=918&originalType=binary&ratio=1&size=2367690&status=done&style=shadow&taskId=u7493f524-39fc-475d-a13f-4b017d27e51)
<a name="ayjmx"></a>
#### 特点
只能一次性使用(不能`reset`)；主线程阻塞；某个线程中断将永远到不了屏障点，所有线程都会一直等待。
<a name="WOWKc"></a>
#### 例子
```java
public class CyclicBarrier1 implements Runnable {
    //创建初始化3个线程的线程池
    private ExecutorService threadPool = Executors.newFixedThreadPool(3);
    //保存每个学生的平均成绩
    private ConcurrentHashMap<String, Integer> map = new ConcurrentHashMap<>();
    private CountDownLatch countDownLatch = new CountDownLatch(3);

    private void count() {
        for (int i = 0; i < 3; i++) {
            threadPool.execute(() -> {
                //计算每个学生的平均成绩,代码略()假设为60~100的随机数
                int score = (int) (Math.random() * 40 + 60);
                try {
                    Thread.sleep(Math.round(Math.random() * 1000));
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                map.put(Thread.currentThread().getName(), score);
                System.out.println(Thread.currentThread().getName() + "同学的平均成绩为" + score);
                countDownLatch.countDown();
            });
        }
        this.run();
        threadPool.shutdown();
    }

    @Override
    public void run() {
        try {
            countDownLatch.await();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        int result = 0;
        Set<String> set = map.keySet();
        for (String s : set) {
            result += map.get(s);
        }
        System.out.println("三人平均成绩为:" + (result / 3) + "分");
    }

    public static void main(String[] args) throws InterruptedException {
        long now = System.currentTimeMillis();
        CyclicBarrier1 cb = new CyclicBarrier1();
        cb.count();
        Thread.sleep(100);
        long end = System.currentTimeMillis();
        System.out.println(end - now);
    }
}
```
最终输出结果：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630296763954-7cdbba48-6851-4fa3-849c-3d72c15bd5b2.png#clientId=u72f468dd-f541-4&from=paste&height=180&id=u20813187&originHeight=541&originWidth=1900&originalType=binary&ratio=1&size=134153&status=done&style=shadow&taskId=u859b279f-a782-4fcf-91ee-ea990c8ef86&width=633.3333333333334)<br />其中749ms证明了会阻塞主线程。
<a name="P17Af"></a>
## `CyclicBarrier`
`CyclicBarrier `的字面意思是可循环使用（Cyclic）的屏障（Barrier）。它要做的事情是，让一组线程到达一个屏障（也可以叫同步点）时被阻塞，直到最后一个线程到达屏障时，屏障才会开门，所有被屏障拦截的线程才会继续干活。<br />这个屏障之所以用循环修饰，是因为在所有的线程释放彼此之后，这个屏障是可以重新使用的（`reset()`方法重置屏障点），这一点与`CountDownLatch`不同。<br />`CyclicBarrier`是一种同步机制允许一组线程相互等待，等到所有线程都到达一个屏障点才退出`await`方法，它没有直接实现AQS而是借助`ReentrantLock`来实现的同步机制。它是可循环使用的，而`CountDownLatch`是一次性的，另外它体现的语义也跟`CountDownLatch`不同，`CountDownLatch`减少计数到达条件采用的是`release`方式，而`CyclicBarrier`走向屏障点（`await`）采用的是`Acquire`方式，Acquire是会阻塞的，这也实现了`CyclicBarrier`的另外一个特点，只要有一个线程中断那么屏障点就被打破，所有线程都将被唤醒（`CyclicBarrier`自己负责这部分实现，不是由AQS调度的），这样也避免了因为一个线程中断引起永远不能到达屏障点而导致其他线程一直等待。屏障点被打破的`CyclicBarrier`将不可再使用（会抛出`BrokenBarrierException`）除非执行`reset`操作。
<a name="c6P0O"></a>
#### 构造函数
`CyclicBarrier`有两个构造函数:

- `CyclicBarrier(int parties)`int类型的参数表示有几个线程来参与这个屏障拦截，(拿上面的例子，即有几个人跟团旅游)；
- `CyclicBarrier(int parties,Runnable barrierAction)`当所有线程到达一个屏障点时，优先执行`barrierAction`这个线程。

最重要的一个方法:

- `await()`：每个线程调用`await()`，表示已经到达屏障点，然后当前线程被阻塞。
<a name="q9dS1"></a>
#### 例子
```java
public class CyclicBarrier1 implements Runnable {
    //创建初始化3个线程的线程池
    private ExecutorService threadPool = Executors.newFixedThreadPool(3);
    //创建3个CyclicBarrier对象,执行完后执行当前类的run方法
    private CyclicBarrier cb = new CyclicBarrier(3, this);
    //保存每个学生的平均成绩
    private ConcurrentHashMap<String, Integer> map = new ConcurrentHashMap<>();

    private void count() {
        for (int i = 0; i < 3; i++) {
            threadPool.execute(() -> {
                //计算每个学生的平均成绩,代码略()假设为60~100的随机数
                int score = (int) (Math.random() * 40 + 60);
                try {
                    Thread.sleep(Math.round(Math.random() * 1000));
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                map.put(Thread.currentThread().getName(), score);
                System.out.println(Thread.currentThread().getName() + "同学的平均成绩为" + score);
                try {
                    //执行完运行await(),等待所有学生平均成绩都计算完毕
                    cb.await();
                } catch (InterruptedException | BrokenBarrierException e) {
                    e.printStackTrace();
                }
            });
        }
        threadPool.shutdown();
    }

    @Override
    public void run() {
        int result = 0;
        Set<String> set = map.keySet();
        for (String s : set) {
            result += map.get(s);
        }
        System.out.println("三人平均成绩为:" + (result / 3) + "分");
    }

    public static void main(String[] args) throws InterruptedException {
        long now = System.currentTimeMillis();
        CyclicBarrier1 cb = new CyclicBarrier1();
        cb.count();
        Thread.sleep(100);
        long end = System.currentTimeMillis();
        System.out.println(end - now);
    }
}
```
最终输出结果：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630296882647-37bedcae-bafa-4e5d-b1f6-b6e58e2f3a3d.png#clientId=u72f468dd-f541-4&from=paste&height=186&id=u1a99a879&originHeight=558&originWidth=1846&originalType=binary&ratio=1&size=136362&status=done&style=shadow&taskId=u5e0142c8-da63-4e17-b8fa-ab3adf8b8a2&width=615.3333333333334)<br />显然没有阻塞主线程。
<a name="HXFmk"></a>
## 两者区别

- `CountDownLatch`的计数器只能使用一次。而`CyclicBarrier`的计数器可以使用`reset()`方法重置。所以`CyclicBarrier`能处理更为复杂的业务场景，比如如果计算发生错误，可以重置计数器，并让线程们重新执行一次。
- `CyclicBarrier`还提供其他有用的方法，比如`getNumberWaiting`方法可以获得`CyclicBarrier`阻塞的线程数量。`isBroken`方法用来知道阻塞的线程是否被中断。比如以下代码执行完之后会返回true。
- `CountDownLatch`会阻塞主线程，`CyclicBarrier`不会阻塞主线程，只会阻塞子线程。
- 某线程中断`CyclicBarrier`会抛出异常，避免了所有线程无限等待。

来从jdk作者设计的目的来看，javadoc是这么描述它们的：
> `CountDownLatch`: <br />A synchronization aid that allows one or more threads to wait until a set of operations being performed in other threads completes.

> `CyclicBarrier`:<br />A synchronization aid that allows a set of threads to all wait for each other to reach a common barrier point.

从javadoc的描述可以得出：

- `CountDownLatch`：一个或者多个线程，等待其他多个线程完成某件事情之后才能执行；
- `CyclicBarrier`：多个线程互相等待，直到到达同一个同步点，再继续一起执行。

对于`CountDownLatch`来说，重点是“一个线程（多个线程）等待”，而其他的N个线程在完成“某件事情”之后，可以终止，也可以等待。而对于`CyclicBarrier`，重点是多个线程，在任意一个线程没有完成，所有的线程都必须等待。<br />`CountDownLatch`是计数器，线程完成一个记录一个，只不过计数不是递增而是递减，而`CyclicBarrier`更像是一个阀门，需要所有线程都到达，阀门才能打开，然后继续执行。
