Java 线程安全
<a name="Hwqzc"></a>
## 一、线程安全在三个方面体现

1. 原子性：提供互斥访问，同一时刻只能有一个线程对数据进行操作，（`atomic`,`synchronized`）；
2. 可见性：一个线程对主内存的修改可以及时地被其他线程看到，（`synchronized`,`volatile`）；
3. 有序性：一个线程观察其他线程中的指令执行顺序，由于指令重排序，该观察结果一般杂乱无序，（happens-before原则）。

接下来，依次分析。
<a name="qzLqn"></a>
## 二、原子性---atomic
JDK里面提供了很多atomic类，`AtomicInteger`,`AtomicLong`,`AtomicBoolean`等等。<br />它们是通过CAS完成原子性。<br />依次来看`AtomicInteger`，`AtomicStampedReference`，`AtomicLongArray`，`AtomicBoolean`。
<a name="C6wya"></a>
#### （1）`AtomicInteger`
先来看一个`AtomicInteger`例子：
```java
public class AtomicIntegerExample1 {
    // 请求总数
    public static int clientTotal = 5000;
    // 同时并发执行的线程数
    public static int threadTotal = 200;
 
    public static AtomicInteger count = new AtomicInteger(0);
 
    public static void main(String[] args) throws Exception {
        ExecutorService executorService = Executors.newCachedThreadPool();//获取线程池
        final Semaphore semaphore = new Semaphore(threadTotal);//定义信号量
        final CountDownLatch countDownLatch = new CountDownLatch(clientTotal);
        for (int i = 0; i < clientTotal ; i++) {
            executorService.execute(() -> {
                try {
                    semaphore.acquire();
                    add();
                    semaphore.release();
                } catch (Exception e) {
                    log.error("exception", e);
                }
                countDownLatch.countDown();
            });
        }
        countDownLatch.await();
        executorService.shutdown();
        log.info("count:{}", count.get());
    }
 
    private static void add() {
        count.incrementAndGet();
    }
}
```
可以执行看到最后结果是5000是线程安全的。<br />那么看`AtomicInteger`的`incrementAndGet()`方法：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629939881353-8c323716-512d-4e57-a69c-5023a2ce65c4.png#clientId=u1ad65521-a75c-4&from=paste&height=114&id=u92b0a211&originHeight=342&originWidth=2239&originalType=binary&ratio=1&size=63701&status=done&style=shadow&taskId=ue8ea28a7-c2d7-401d-bbb6-a390e98b0bd&width=746.3333333333334)<br />再看`getAndAddInt()`方法：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629939936073-6e50279f-a3cf-43f4-ac7c-e2b3a2eb235c.png#clientId=u1ad65521-a75c-4&from=paste&height=141&id=u7e7aade8&originHeight=422&originWidth=2766&originalType=binary&ratio=1&size=123406&status=done&style=shadow&taskId=u3f67a756-37c1-481c-9628-834352ef39f&width=922)<br />这里面调用了`compareAndSwapInt()`方法：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629940051344-35cb0be1-7b6a-4a34-bb0f-43a22fc24729.png#clientId=u1ad65521-a75c-4&from=paste&height=27&id=uf4982727&originHeight=81&originWidth=3354&originalType=binary&ratio=1&size=40203&status=done&style=shadow&taskId=u41301b67-c157-4e68-9383-35715e0e8c7&width=1118)<br />它是`native`修饰的，代表是java底层的方法，不是通过java实现的 。<br />再重新看`getAndAddInt()`，传来第一个值是当前的一个对象 ，比如是`count.incrementAndGet()`，那么在`getAndAddInt()`中，var1就是count，而var2第二个值是当前的值，比如想执行的是2+1=3操作，那么第二个参数是2，第三个参数是1 。<br />变量5（var5）是调用底层的方法而得到的底层当前的值，如果没有别的线程过来处理`count`变量的时候，那么它正常返回值是2。<br />因此传到`compareAndSwapInt`方法里的参数是（`count`对象，当前值2，当前从底层传过来的2，从底层取出来的值加上改变量var4）。<br />`compareAndSwapInt()`希望达到的目标是对于var1对象，如果当前的值var2和底层的值var5相等，那么把它更新成后面的值（var5+var4）.<br />`compareAndSwapInt`核心就是CAS核心。<br />关于`count`值为什么和底层值不一样：`count`里面的值相当于存在于工作内存的值，底层就是主内存。
<a name="ST8gg"></a>
#### （2）`AtomicStampedReference`
接下来看一下`AtomicStampedReference`。<br />关于CAS有一个ABA问题：开始是A，后来改为B，现在又改为A。解决办法就是：每次变量改变的时候，把变量的版本号加1。<br />这就用到了`AtomicStampedReference`。<br />来看`AtomicStampedReference`里的`compareAndSet()`实现：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629940238532-bd63dbbd-8ee5-40f0-a6b8-7731c60e93da.png#clientId=u1ad65521-a75c-4&from=paste&height=357&id=u24801a95&originHeight=1070&originWidth=2446&originalType=binary&ratio=1&size=257952&status=done&style=shadow&taskId=ued9dad6a-1a88-4b2d-a0a1-137d981a814&width=815.3333333333334)<br />而在`AtomicInteger`里`compareAndSet()`实现：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629940194236-d354644a-b756-459e-a25d-b01973648b70.png#clientId=u1ad65521-a75c-4&from=paste&height=151&id=ufb0f99ba&originHeight=454&originWidth=2828&originalType=binary&ratio=1&size=116118&status=done&style=shadow&taskId=ucc66edbb-63fc-4dd2-85f3-adaf4d22fbd&width=942.6666666666666)<br />可以看到`AtomicStampedReference`里的`compareAndSet()`中多了 一个stamp比较（也就是版本），这个值是由每次更新时来维护的。
<a name="RoI3U"></a>
#### （3）`AtomicLongArray`
这种维护数组的`atomic`类，可以选择性地更新其中某一个索引对应的值，也是进行原子性操作。这种对数组的操作的各种方法，会多处一个索引。<br />比如，看一下`compareAndSet()`：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629940305991-62a40eb0-bd2e-4d1f-9efd-33eea5048fdf.png#clientId=u1ad65521-a75c-4&from=paste&height=185&id=ua1d4780f&originHeight=556&originWidth=2763&originalType=binary&ratio=1&size=125421&status=done&style=shadow&taskId=u1a2435ca-1634-45c7-b609-1cb79783946&width=921)
<a name="bVaiM"></a>
#### （4）`AtomicBoolean`
看一段代码：
```java
public class AtomicBooleanExample {
 
    private static AtomicBoolean isHappened = new AtomicBoolean(false);
 
    // 请求总数
    public static int clientTotal = 5000;
    // 同时并发执行的线程数
    public static int threadTotal = 200;
    public static void main(String[] args) throws Exception {
        ExecutorService executorService = Executors.newCachedThreadPool();
        final Semaphore semaphore = new Semaphore(threadTotal);
        final CountDownLatch countDownLatch = new CountDownLatch(clientTotal);
        for (int i = 0; i < clientTotal ; i++) {
            executorService.execute(() -> {
                try {
                    semaphore.acquire();
                    test();
                    semaphore.release();
                } catch (Exception e) {
                    log.error("exception", e);
                }
                countDownLatch.countDown();
            });
        }
        countDownLatch.await();
        executorService.shutdown();
        log.info("isHappened:{}", isHappened.get());
    }
    private static void test() {
        if (isHappened.compareAndSet(false, true)) {
            log.info("execute");
        }
    }
}
```
执行之后发现，`log.info("execute");`只执行了一次，且`isHappend`值为`true`。<br />原因就是当它第一次`compareAndSet()`之后，`isHappend`变为`true`，没有别的线程干扰。<br />通过使用`AtomicBoolean`，可以使某段代码只执行一次。
<a name="lZRn2"></a>
## 三、原子性---`synchronized`
`synchronized`是一种同步锁，通过锁实现原子操作。<br />JDK提供锁分两种：一种是`synchronized`，依赖JVM实现锁，因此在这个关键字作用对象的作用范围内是同一时刻只能有一个线程进行操作；另一种是LOCK，是JDK提供的代码层面的锁，依赖CPU指令，代表性的是`ReentrantLock`。<br />`synchronized`修饰的对象有四种：

- 修饰代码块，作用于调用的对象；
- 修饰方法，作用于调用的对象；
- 修饰静态方法，作用于所有对象；
- 修饰类，作用于所有对象。

修饰代码块和方法：
```java
@Slf4j
public class SynchronizedExample1 {
 
    // 修饰一个代码块
    public void test1(int j) {
        synchronized (this) {
            for (int i = 0; i < 10; i++) {
                log.info("test1 {} - {}", j, i);
            }
        }
    }
 
    // 修饰一个方法
    public synchronized void test2(int j) {
        for (int i = 0; i < 10; i++) {
            log.info("test2 {} - {}", j, i);
        }
    }
 
    public static void main(String[] args) {
        SynchronizedExample1 example1 = new SynchronizedExample1();
        SynchronizedExample1 example2 = new SynchronizedExample1();
        ExecutorService executorService = Executors.newCachedThreadPool();
        //一
        executorService.execute(() -> {
            example1.test1(1);
        });
        executorService.execute(() -> {
            example1.test1(2);
        });
        //二
        executorService.execute(() -> {
            example2.test2(1);
        });
        executorService.execute(() -> {
            example2.test2(2);
        });
        //三
        executorService.execute(() -> {
            example1.test1(1);
        });
        executorService.execute(() -> {
            example2.test1(2);
        });
    }
}
```
执行后可以看到对于情况一，test1内部方法块作用于example1，先执行完一次0-9输出，再执行下一次0-9输出；情况二，同情况一类似，作用于example2；情况三，可以看到交叉执行，test1分别独立作用于example1和example2，互不影响。<br />修饰静态方法和类：
```java
@Slf4j
public class SynchronizedExample2 {
 
    // 修饰一个类
    public static void test1(int j) {
        synchronized (SynchronizedExample2.class) {
            for (int i = 0; i < 10; i++) {
                log.info("test1 {} - {}", j, i);
            }
        }
    }
 
    // 修饰一个静态方法
    public static synchronized void test2(int j) {
        for (int i = 0; i < 10; i++) {
            log.info("test2 {} - {}", j, i);
        }
    }
 
    public static void main(String[] args) {
        SynchronizedExample2 example1 = new SynchronizedExample2();
        SynchronizedExample2 example2 = new SynchronizedExample2();
        ExecutorService executorService = Executors.newCachedThreadPool();
        executorService.execute(() -> {
            example1.test1(1);
        });
        executorService.execute(() -> {
            example2.test1(2);
        });
    }
}
```
test1和test2会锁定调用它们的对象所属的类，同一个时间只有一个对象在执行。
<a name="sbBPL"></a>
## 四、可见性---`volatile`
对于可见性，JVM提供了`synchronized`和`volatile`。这里看`volatile`。
<a name="W2eHG"></a>
#### （1）`volatile`的可见性是通过内存屏障和禁止重排序实现的
`volatile`会在写操作时，会在写操作后加一条`store`屏障指令，将本地内存中的共享变量值刷新到主内存：<br />![2021-08-26-08-50-02-104385.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629939518634-672c0a19-0acf-4681-ad35-57dc8fcb2082.png#clientId=u1ad65521-a75c-4&from=ui&id=u15e77a87&originHeight=521&originWidth=836&originalType=binary&ratio=1&size=176197&status=done&style=shadow&taskId=u38cb0b3e-32f2-43f6-9c65-b72f1bce034)<br />`volatile`在进行读操作时，会在读操作前加一条`load`指令，从内存中读取共享变量：<br />![2021-08-26-08-50-02-312392.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629939518646-127147bf-3db3-4f79-8486-141092c825a8.png#clientId=u1ad65521-a75c-4&from=ui&id=SEEA8&originHeight=508&originWidth=806&originalType=binary&ratio=1&size=177728&status=done&style=shadow&taskId=u5048ce85-db77-4610-9e35-c86b3e4c66b)
<a name="U1Xyh"></a>
#### （2）但是`volatile`不是原子性的，进行++操作不是安全的
```java
@Slf4j
public class VolatileExample {
 
    // 请求总数
    public static int clientTotal = 5000;
 
    // 同时并发执行的线程数
    public static int threadTotal = 200;
 
    public static volatile int count = 0;
 
    public static void main(String[] args) throws Exception {
        ExecutorService executorService = Executors.newCachedThreadPool();
        final Semaphore semaphore = new Semaphore(threadTotal);
        final CountDownLatch countDownLatch = new CountDownLatch(clientTotal);
        for (int i = 0; i < clientTotal ; i++) {
            executorService.execute(() -> {
                try {
                    semaphore.acquire();
                    add();
                    semaphore.release();
                } catch (Exception e) {
                    log.error("exception", e);
                }
                countDownLatch.countDown();
            });
        }
        countDownLatch.await();
        executorService.shutdown();
        log.info("count:{}", count);
    }
 
    private static void add() {
        count++;
    }
}
```
执行后发现线程不安全，原因是 执行`conut++` 时分成了三步，第一步是取出当前内存 count 值，这时 count 值时最新的，接下来执行了两步操作，分别是 +1 和重新写回主存。假设有两个线程同时在执行 count++ ，两个内存都执行了第一步，比如当前 count 值为 5 ，它们都读到了，然后两个线程分别执行了 +1 ，并写回主存，这样就丢掉了一次加一的操作。
<a name="y0nlm"></a>
#### （3）`volatile`适用的场景
既然`volatile`不适用于计数，那么`volatile`适用于哪些场景呢：

1. 对变量的写操作不依赖于当前值
2. 该变量没有包含在具有其他变量不变的式子中

因此，`volatile`适用于状态标记量：<br />![2021-08-26-08-50-02-471387.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629939580126-4db4ac4f-ca47-49bb-b22c-996b63855956.png#clientId=u1ad65521-a75c-4&from=ui&id=u8b26e99e&originHeight=356&originWidth=472&originalType=binary&ratio=1&size=69780&status=done&style=shadow&taskId=u411f4a4a-b061-4c08-aebd-9941e66d298)<br />线程1负责初始化，线程2不断查询inited值，当线程1初始化完成后，线程2就可以检测到inited为true了。
<a name="bwv7X"></a>
## 五、有序性
有序性是指，在JMM中，允许编译器和处理器对指令进行重排序，但是重排序过程不会影响到单线程程序的执行，却会影响到多线程并发执行的正确性。<br />可以通过`volatile`、`synchronized`、`lock`保证有序性。<br />另外，JMM具有先天的有序性，即不需要通过任何手段就可以得到保证的有序性。这称为happens-before原则。<br />如果两个操作的执行次序无法从happens-before原则推导出来，那么它们就不能保证它们的有序性。虚拟机可以随意地对它们进行重排序。<br />happens-before原则：

1. 程序次序规则：在一个单独的线程中，按照程序代码书写的顺序执行。
2. 锁定规则：一个`unlock`操作happen—before后面对同一个锁的`lock`操作。
3. volatile变量规则：对一个`volatile`变量的写操作happen—before后面对该变量的读操作。
4. 线程启动规则：Thread对象的`start()`方法happen—before此线程的每一个动作。
5. 线程终止规则：线程的所有操作都happen—before对此线程的终止检测，可以通过Thread.join()方法结束、`Thread.isAlive()`的返回值等手段检测到线程已经终止执行。
6. 线程中断规则：对线程`interrupt()`方法的调用happen—before发生于被中断线程的代码检测到中断时事件的发生。
7. 对象终结规则：一个对象的初始化完成（构造函数执行结束）happen—before它的`finalize()`方法的开始。
8. 传递性：如果操作A happen—before操作B，操作B happen—before操作C，那么可以得出A happen—before操作C。
