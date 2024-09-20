Java
<a name="VqSTm"></a>
## 认识 Semaphore
<a name="JF5iG"></a>
### Semaphore 是什么
Semaphore 一般译作信号量，它也是一种线程同步工具，主要用于多个线程对共享资源进行并行操作的一种工具类。它代表了一种许可的概念，是否允许多线程对同一资源进行操作的许可，使用 Semaphore 可以控制并发访问资源的线程个数。
<a name="pb1Cn"></a>
### Semaphore 的使用场景
Semaphore 的使用场景主要用于流量控制，比如数据库连接，同时使用的数据库连接会有数量限制，数据库连接不能超过一定的数量，当连接到达了限制数量后，后面的线程只能排队等前面的线程释放数据库连接后才能获得数据库连接。<br />再比如交通公路上的红绿灯，绿灯亮起时只能让 100 辆车通过，红灯亮起不允许车辆通过。<br />再比如停车场的场景中，一个停车场有有限数量的车位，同时能够容纳多少台车，车位满了之后只有等里面的车离开停车场外面的车才可以进入。
<a name="ohJBb"></a>
#### 使用Semaphore控制最大并发数来进行限流
以秒杀业务为例，10个iphone，100万人抢购，100万人同时发起请求，最终能够抢到的人也就是前面几个人，后面的基本上都没有希望了，那么可以通过控制并发数来实现，比如并发数控制在10个，其他超过并发数的请求全部拒绝，提示：秒杀失败，请稍后重试。<br />并发控制的，通俗解释：一大波人去商场购物，必须经过一个门口，门口有个门卫，兜里面有指定数量的门禁卡，来的人先去门卫那边拿取门禁卡，拿到卡的人才可以刷卡进入商场，拿不到的可以继续等待。进去的人出来之后会把卡归还给门卫，门卫可以把归还来的卡继续发放给其他排队的顾客使用。<br />JUC中提供了这样的工具类：Semaphore，示例代码：
```java
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;

public class Demo1 {

    static Semaphore semaphore = new Semaphore(5);

    public static void main(String[] args) {
        for (int i = 0; i < 20; i++) {
            new Thread(() -> {
                boolean flag = false;
                try {
                    flag = semaphore.tryAcquire(100, TimeUnit.MICROSECONDS);
                    if (flag) {
                        //休眠2秒，模拟下单操作
                        System.out.println(Thread.currentThread() + "，尝试下单中。。。。。");
                        TimeUnit.SECONDS.sleep(2);
                    } else {
                        System.out.println(Thread.currentThread() + "，秒杀失败，请稍微重试！");
                    }
                } catch (InterruptedException e) {
                    e.printStackTrace();
                } finally {
                    if (flag) {
                        semaphore.release();
                    }
                }
            }).start();
        }
    }

}
```
输出：
```
Thread[Thread-10,5,main]，尝试下单中。。。。。
Thread[Thread-8,5,main]，尝试下单中。。。。。
Thread[Thread-9,5,main]，尝试下单中。。。。。
Thread[Thread-12,5,main]，尝试下单中。。。。。
Thread[Thread-11,5,main]，尝试下单中。。。。。
Thread[Thread-2,5,main]，秒杀失败，请稍微重试！
Thread[Thread-1,5,main]，秒杀失败，请稍微重试！
Thread[Thread-18,5,main]，秒杀失败，请稍微重试！
Thread[Thread-16,5,main]，秒杀失败，请稍微重试！
Thread[Thread-0,5,main]，秒杀失败，请稍微重试！
Thread[Thread-3,5,main]，秒杀失败，请稍微重试！
Thread[Thread-14,5,main]，秒杀失败，请稍微重试！
Thread[Thread-6,5,main]，秒杀失败，请稍微重试！
Thread[Thread-13,5,main]，秒杀失败，请稍微重试！
Thread[Thread-17,5,main]，秒杀失败，请稍微重试！
Thread[Thread-7,5,main]，秒杀失败，请稍微重试！
Thread[Thread-19,5,main]，秒杀失败，请稍微重试！
Thread[Thread-15,5,main]，秒杀失败，请稍微重试！
Thread[Thread-4,5,main]，秒杀失败，请稍微重试！
Thread[Thread-5,5,main]，秒杀失败，请稍微重试！
```
<a name="cWkQZ"></a>
### Semaphore 使用
下面就来模拟一下停车场的业务场景：在进入停车场之前会有一个提示牌，上面显示着停车位还有多少，当车位为 0 时，不能进入停车场，当车位不为 0 时，才会允许车辆进入停车场。所以停车场有几个关键因素：停车场车位的总容量，当一辆车进入时，停车场车位的总容量 - 1，当一辆车离开时，总容量 + 1，停车场车位不足时，车辆只能在停车场外等待。
```java
public class CarParking {

    private static Semaphore semaphore = new Semaphore(10);

    public static void main(String[] args){

        for(int i = 0;i< 100;i++){

            Thread thread = new Thread(new Runnable() {
                @Override
                public void run() {
                    System.out.println("欢迎 " + Thread.currentThread().getName() + " 来到停车场");
                    // 判断是否允许停车
                    if(semaphore.availablePermits() == 0) {
                        System.out.println("车位不足，请耐心等待");
                    }
                    try {
                        // 尝试获取
                        semaphore.acquire();
                        System.out.println(Thread.currentThread().getName() + " 进入停车场");
                        Thread.sleep(new Random().nextInt(10000));// 模拟车辆在停车场停留的时间
                        System.out.println(Thread.currentThread().getName() + " 驶出停车场");
                        semaphore.release();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }, i + "号车");
            thread.start();
        }
    }
}
```
在上面这段代码中，给出了 Semaphore 的初始容量，也就是只有 10 个车位，用这 10 个车位来控制 100 辆车的流量，所以结果和预想的很相似，即大部分车都在等待状态。但是同时仍允许一些车驶入停车场，驶入停车场的车辆，就会 `semaphore.acquire` 占用一个车位，驶出停车场时，就会 `semaphore.release` 让出一个车位，让后面的车再次驶入。
<a name="QfhJg"></a>
### Semaphore 信号量的模型
上面代码虽然比较简单，但是却能了解到一个信号量模型的五脏六腑。下面是一个信号量的模型：<br />![2021-05-15-21-17-34-636779.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621084669518-5c570ad7-7714-4f9a-96fd-622b201bc7f5.png#averageHue=%23f9f5f1&clientId=u6c5d2130-402d-4&from=ui&id=u83c24a7c&originHeight=446&originWidth=783&originalType=binary&ratio=1&rotation=0&showTitle=false&size=102512&status=done&style=shadow&taskId=u2cdaabfb-da95-4326-b1d9-f82517d89a4&title=)<br />来解释一下 Semaphore ，Semaphore 有一个初始容量，这个初始容量就是 Semaphore 所能够允许的信号量。在调用 Semaphore 中的 `acquire` 方法后，Semaphore 的容量 -1，相对的在调用 `release` 方法后，Semaphore 的容量 + 1，在这个过程中，计数器一直在监控 Semaphore 数量的变化，等到流量超过 Semaphore 的容量后，多余的流量就会放入等待队列中进行排队等待。等到 Semaphore 的容量允许后，方可重新进入。<br />Semaphore 所控制的流量其实就是一个个的线程，因为并发工具最主要的研究对象就是线程。<br />它的工作流程如下<br />![2021-05-15-21-17-34-746180.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621084682744-56af63f8-ddc1-4fe3-a552-0c29e17a3d91.png#averageHue=%23f9e0dc&clientId=u6c5d2130-402d-4&from=ui&id=uc0dcd0c6&originHeight=647&originWidth=604&originalType=binary&ratio=1&rotation=0&showTitle=false&size=79943&status=done&style=shadow&taskId=u61663874-75c6-4113-87d8-55e00d5f6ca&title=)<br />这幅图应该很好理解吧，这里就不再过多解释。
<a name="PyL1q"></a>
## Semaphore 深入理解
在了解 Semaphore 的基本使用和 Semaphore 的模型后，下面还是得从源码来和看一看Semaphore 的种种细节问题！
<a name="VO9do"></a>
### Semaphore 基本属性
Semaphore 中只有一个属性
```java
private final Sync sync;
```
Sync 是 Semaphore 的同步实现，Semaphore 保证线程安全性的方式和 `ReentrantLock` 、CountDownLatch 类似，都是继承于 AQS 的实现。同样的，这个 Sync 也是继承于`AbstractQueuedSynchronizer`的一个变量，也就是说，聊 Semaphore 也绕不开 AQS，所以说 AQS 真的太重要了。
<a name="IzdlU"></a>
### Semaphore 的公平性和非公平性
那么进入 Sync 内部看看它实现了哪些方法
```java
abstract static class Sync extends AbstractQueuedSynchronizer {
    private static final long serialVersionUID = 1192457210091910933L;

    Sync(int permits) {
        setState(permits);
    }

    final int getPermits() {
        return getState();
    }

    final int nonfairTryAcquireShared(int acquires) {
        for (;;) {
            int available = getState();
            int remaining = available - acquires;
            if (remaining < 0 ||
                compareAndSetState(available, remaining))
                return remaining;
        }
    }

    protected final boolean tryReleaseShared(int releases) {
        for (;;) {
            int current = getState();
            int next = current + releases;
            if (next < current) // overflow
                throw new Error("Maximum permit count exceeded");
            if (compareAndSetState(current, next))
                return true;
        }
    }

    final void reducePermits(int reductions) {
        for (;;) {
            int current = getState();
            int next = current - reductions;
            if (next > current) // underflow
                throw new Error("Permit count underflow");
            if (compareAndSetState(current, next))
                return;
        }
    }

    final int drainPermits() {
        for (;;) {
            int current = getState();
            if (current == 0 || compareAndSetState(current, 0))
                return current;
        }
    }
}
```
首先是 Sync 的初始化，内部调用了`setState`并传递了 `permits`，AQS 中的 State 其实就是同步状态的值，而 Semaphore 的这个 `permits` 就是代表了许可的数量。<br />`getPermits` 其实就是调用了 `getState` 方法获取了一下线程同步状态值。后面的 `nonfairTryAcquireShared` 方法其实是在 Semaphore 中构造了 `NonfairSync` 中的 `tryAcquireShared` 调用的<br />![2021-05-15-21-17-34-854878.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621084699390-ed25fb54-3ad5-4362-9295-c69c46822928.png#averageHue=%23fbfbfb&clientId=u6c5d2130-402d-4&from=ui&id=u47f51630&originHeight=190&originWidth=610&originalType=binary&ratio=1&rotation=0&showTitle=false&size=26745&status=done&style=shadow&taskId=ub1d00dcd-c1bd-4fa6-96b6-62e641cb433&title=)<br />这里需要提及一下什么是`NonfairSync`，除了 `NonfairSync` 是不是还有 `FairSync` 呢？查阅 JDK 源码发现确实有。<br />那么这里的 `FairSync` 和 `NonfairSync` 都代表了什么？为什么会有这两个类呢？<br />事实上，Semaphore 就像 `ReentrantLock` 一样，也存在“公平”和"不公平"两种，默认情况下 Semaphore 是一种不公平的信号量<br />![2021-05-15-21-17-34-969722.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621084720146-39e4e703-52b9-496d-b65a-fe9c980658ce.png#averageHue=%23fbfafa&clientId=u6c5d2130-402d-4&from=ui&id=ua8b848fd&originHeight=919&originWidth=653&originalType=binary&ratio=1&rotation=0&showTitle=false&size=104667&status=done&style=shadow&taskId=u17a3e383-e36e-4906-8506-b9ee0e601c0&title=)<br />Semaphore 的不公平意味着它不会保证线程获得许可的顺序，Semaphore 会在线程等待之前为调用 `acquire` 的线程分配一个许可，拥有这个许可的线程会自动将自己置于线程等待队列的头部。<br />当这个参数为 true 时，Semaphore 确保任何调用 `acquire` 的方法，都会按照先入先出的顺序来获取许可。
```java
final int nonfairTryAcquireShared(int acquires) {
    for (;;) {
        // 获取同步状态值
        int available = getState();
        // state 的值 - 当前线程需要获取的信号量（通常默认是 -1），只有
        // remaining > 0 才表示可以获取。
        int remaining = available - acquires;
        // 先判断是否小于 0 ，如果小于 0 则表示无法获取，如果是正数
        // 就需要使用 CAS 判断内存值和同步状态值是否一致，然后更新为同步状态值 - 1
        if (remaining < 0 ||
            compareAndSetState(available, remaining))
            return remaining;
    }
}
```
![2021-05-15-21-17-35-065465.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621084739058-57afe76f-57fc-41e2-8a21-5c59b0ffd2c9.png#averageHue=%23f9f8f8&clientId=u6c5d2130-402d-4&from=ui&id=u5c297fc4&originHeight=389&originWidth=810&originalType=binary&ratio=1&rotation=0&showTitle=false&size=128995&status=done&style=shadow&taskId=ued589dcc-2b3a-44f1-b5a3-89a2fca318c&title=)<br />从上面这幅源码对比图可以看到，`NonfairSync` 和 `FairSync` 最大的区别就在于`tryAcquireShared`方法的区别。<br />NonfairSync 版本中，是不会管当前等待队列中是否有排队许可的，它会直接判断信号许可量和 CAS 方法的可行性。<br />`FairSync` 版本中，它首先会判断是否有许可进行排队，如果有的话就直接获取失败。<br />上面说公平性和非公平性的区别一直针对的是 `acquire` 方法来说的，怎么现在他们两个主要的区别在于`tryAcquireShared`方法呢？<br />进入到`acquire`方法一探究竟<br />![2021-05-15-21-17-35-183169.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621084751917-3f664e1d-7298-4253-b49a-0d5f54f94628.png#averageHue=%23fbf9f9&clientId=u6c5d2130-402d-4&from=ui&id=u3bee6f3a&originHeight=414&originWidth=792&originalType=binary&ratio=1&rotation=0&showTitle=false&size=112933&status=done&style=shadow&taskId=u6f3aa144-0e03-4f4d-8af4-8d63ec33b72&title=)<br />可以看到，在 `acquire` 方法中，会调用 `tryAcquireShared` 方法，根据其返回值判断是否调用`doAcquireSharedInterruptibly`方法<br />这里需要注意下，`acquire` 方法具有阻塞性，而 `tryAcquire` 方法不具有阻塞性。<br />这也就是说，调用 `acquire` 方法如果获取不到许可，那么 `Semaphore` 会阻塞，直到有可用的许可。而 `tryAcquire` 方法如果获取不到许可会直接返回 `false`。<br />这里还需要注意下`acquireUninterruptibly`方法，其他 `acquire` 的相关方法要么是非阻塞，要么是阻塞可中断，而 `acquireUninterruptibly` 方法不仅在没有许可的情况下执着的等待，而且也不会中断，使用这个方法时需要注意，这个方法很容易在出现大规模线程阻塞而导致 Java 进程出现假死的情况。<br />有获取许可相对应的就有释放许可，但是释放许可不会区分到底是公平释放还是非公平释放。不管方式如何都是释放一个许可给 Semaphore ，同样的 Semaphore 中的许可数量会增加。<br />![2021-05-15-21-17-35-268033.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621084773427-39e36318-3522-4e76-8a4d-58fd0b15faa2.png#averageHue=%23fcfafa&clientId=u6c5d2130-402d-4&from=ui&id=u8dc2ee35&originHeight=505&originWidth=805&originalType=binary&ratio=1&rotation=0&showTitle=false&size=106001&status=done&style=shadow&taskId=u04ab616c-e080-41ad-bbff-ad3d88071f0&title=)<br />在上图中调用 `tryReleaseShared` 判断是否能进行释放后，再会调用 AQS 中的`releasedShared`方法进行释放。<br />![2021-05-15-21-17-35-355651.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621084787574-176333f0-d0a3-43d7-aadf-513b7e6ff71f.png#averageHue=%23fbfbfb&clientId=u6c5d2130-402d-4&from=ui&id=uea42e309&originHeight=375&originWidth=660&originalType=binary&ratio=1&rotation=0&showTitle=false&size=48093&status=done&style=shadow&taskId=u9a9c331d-6bb6-464c-aa07-36f48f9af46&title=)<br />上面这个释放流程只是释放一个许可，除此之外，还可以释放多个许可
```java
public void release(int permits) {
    if (permits < 0) throw new IllegalArgumentException();
    sync.releaseShared(permits);
}
```
后面这个 `releaseShared` 的释放流程和上面的释放流程一致。
<a name="oqTqM"></a>
### 其他 Semaphore 方法
除了上面基本的 `acquire` 和 `release` 相关方法外，也要了解一下 Semaphore 的其他方法。Semaphore 的其他方法比较少，只有下面这几个<br />`drainPermits`：获取并退还所有立即可用的许可，其实相当于使用 CAS 方法把内存值置为 0<br />`reducePermits`：和`nonfairTryAcquireShared`方法类似，只不过 `nonfairTryAcquireShared` 是使用 CAS 使内存值 + 1，而 `reducePermits` 是使内存值 - 1 。<br />`isFair`：对 Semaphore 许可的争夺是采用公平还是非公平的方式，对应到内部的实现就是 FairSync 和 NonfairSync。<br />`hasQueuedThreads`：当前是否有线程由于要获取 Semaphore 许可而进入阻塞。<br />`getQueuedThreads`：返回一个包含了等待获取许可的线程集合。<br />`getQueueLength`：获取正在排队而进入阻塞状态的线程个数。
