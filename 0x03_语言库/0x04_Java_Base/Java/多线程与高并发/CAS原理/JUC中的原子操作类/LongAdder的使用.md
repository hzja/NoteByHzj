Java LongAdder<br />阿里《Java开发手册》最新嵩山版在 8.3 日发布，其中有一段内容如下：
> 【参考】volatile 解决多线程内存不可见问题。对于一写多读，是可以解决变量同步问题，但是如果多写，同样无法解决线程安全问题。
> 说明：如果是 count++ 操作，使用如下类实现：`AtomicInteger count = new AtomicInteger(); count.addAndGet(1);` 如果是 JDK8，推荐使用 `LongAdder` 对象，比 `AtomicLong` 性能更好（减少乐观 锁的重试次数）。

以上内容共有两个重点：

1. 类似于 count++ 这种非一写多读的场景不能使用 `volatile`；
2. 如果是 JDK8 推荐使用 `LongAdder` 而非 `AtomicLong` 来替代 `volatile`，因为 `LongAdder` 的性能更好。
<a name="0VTFS"></a>
### 为什么需要`AtomicInteger`原子操作类？
对于Java中的运算操作，例如自增或自减，若没有进行额外的同步操作，在多线程环境下就是线程不安全的。num++解析为num=num+1，明显，这个操作不具备原子性，多线程并发共享这个变量时必然会出现问题。测试代码如下:
```java
public class AtomicIntegerTest {
 
    private static final int THREADS_CONUT = 20;
    public static int count = 0;
 
    public static void increase() {
        count++;
    }
 
    public static void main(String[] args) {
        Thread[] threads = new Thread[THREADS_CONUT];
        for (int i = 0; i < THREADS_CONUT; i++) {
            threads[i] = new Thread(new Runnable() {
                @Override
                public void run() {
                    for (int i = 0; i < 1000; i++) {
                        increase();
                    }
                }
            });
            threads[i].start();
        }
        while (Thread.activeCount() > 1) {
            Thread.yield();
        }
        System.out.println(count);
    }
}
```
这里运行了20个线程，每个线程对count变量进行1000此自增操作，如果上面这段代码能够正常并发的话，最后的结果应该是20000才对，但实际结果却发现每次运行的结果都不相同，都是一个小于20000的数字。这是为什么呢？
<a name="jOBdy"></a>
### `volatile` 线程安全测试
`volatile`关键字很重要的两个特性:<br />1、保证变量在线程间可见，对volatile变量所有的写操作都能立即反应到其他线程中，换句话说，`volatile`变量在各个线程中是一致的（得益于java内存模型—"先行发生原则"）；<br />2、禁止指令的重排序优化；<br />首先来测试 `volatile` 在多写环境下的线程安全情况，测试代码如下：
```java
public class VolatileExample {
    public static volatile int count = 0; // 计数器
    public static final int size = 100000; // 循环测试次数
    public static void main(String[] args) {
        // ++ 方式 10w 次
        Thread thread = new Thread(() -> {
            for (int i = 1; i <= size; i++) {
                count++;
            }
        });
        thread.start();
        // -- 10w 次
        for (int i = 1; i <= size; i++) {
            count--;
        }
        // 等所有线程执行完成
        while (thread.isAlive()) {}
        System.out.println(count); // 打印结果
    }
}
```
把 `volatile` 修饰的 `count` 变量 ++ 10w 次，在启动另一个线程 -- 10w 次，正常来说结果应该是 0，但是执行的结果却为：
> 1063

:::danger
**结论：由以上结果可以看出 **`**volatile**`** 在多写环境下是非线程安全的，测试结果和《Java开发手册》相吻合。**
:::
<a name="uNnqq"></a>
### `AtomicInteger`类实现多线程自增操作
把上面的代码改造成`AtomicInteger`原子类型，先看看效果
```java
import java.util.concurrent.atomic.AtomicInteger;
 
public class AtomicIntegerTest {
 
    private static final int THREADS_CONUT = 20;
    public static AtomicInteger count = new AtomicInteger(0);
 
    public static void increase() {
        count.incrementAndGet();
    }
 
    public static void main(String[] args) {
        Thread[] threads = new Thread[THREADS_CONUT];
        for (int i = 0; i < THREADS_CONUT; i++) {
            threads[i] = new Thread(new Runnable() {
                @Override
                public void run() {
                    for (int i = 0; i < 1000; i++) {
                        increase();
                    }
                }
            });
            threads[i].start();
        }
 
        while (Thread.activeCount() > 1) {
            Thread.yield();
        }
        System.out.println(count);
    }
}
```
结果每次都输出20000，程序输出了正确的结果，这都归功于`AtomicInteger.incrementAndGet()`方法的原子性。
<a name="jiZJq"></a>
#### 非阻塞同步
同步：多线程并发访问共享数据时，保证共享数据再同一时刻只被一个或一些线程使用。<br />阻塞同步和非阻塞同步都是实现线程安全的两个保障手段，非阻塞同步对于阻塞同步而言主要解决了阻塞同步中线程阻塞和唤醒带来的性能问题，那什么叫做非阻塞同步呢？<br />在并发环境下，某个线程对共享变量先进行操作，如果没有其他线程争用共享数据那操作就成功；如果存在数据的争用冲突，那就才去补偿措施，比如不断的重试机制，直到成功为止，因为这种乐观的并发策略不需要把线程挂起，也就把这种同步操作称为非阻塞同步（操作和冲突检测具备原子性）。<br />在硬件指令集的发展驱动下，使得 "操作和冲突检测" 这种看起来需要多次操作的行为只需要一条处理器指令便可以完成，这些指令中就包括非常著名的CAS指令（Compare-And-Swap比较并交换）。《深入理解Java虚拟机第二版.周志明》第十三章中这样描述关于CAS机制:
> CAS指合需要有3个操作数，分别是内存位置(在Java中可以简单理解为变量的内存地址，用V表示)、旧的预期值（用A表示）和新值（用B表示)。CAs指合执行时，当且仅当V符合旧预期值A时，处理器用新值B更新v的值，否则它就不执行更新﹐但是无论是否更新了V的值，都会返回V的旧值，上述的处理过程是一个原子操作。
> 在JDK 1.5之后，Java程序中才可以使用CAS操作，该操作由`sun.misc.Unsafe`类里面的`compareAndSwapInt()`和`compareAndSwapLong()`等几个方法包装提供，虚拟机在内部对这些方法做了特殊处理，即时编译出来的结果就是一条平台相关的处理器CAS指令，没有方法调用的过程，或者可以认为是无条件内联进去了。
> 由于Unsafe类不是提供给用户程序调用的类(`Unsafe.getUnsafe()`的代码中限制了只有启动类加载器(Bootstrap ClassLoader)加载的Class才能访问它），因此，如果不采用反射手段﹐只能通过其他的Java API来间接使用它，如J.U.C包里面的整数原子类，其中的`compareAndSet()`和`getAndIncrement()`等方法都使用了Unsafe类的CAS操作。

所以再返回来看`AtomicInteger.incrementAndGet()`方法，它的实现也比较简单
```java
/**
     * Atomically increments by one the current value.
     *
     * @return the updated value
     */
public final int incrementAndGet() {
    for (;;) {
        int current = get();
        int next = current + 1;
        if (compareAndSet(current, next))
            return next;
    }
}
```
`incrementAndGet()`方法在一个无限循环体内，不断尝试将一个比当前值大1的新值赋给自己，如果失败则说明在执行"获取-设置"操作的时已经被其它线程修改过了，于是便再次进入循环下一次操作，直到成功为止。这个便是`AtomicInteger`原子性的"诀窍"了，继续进源码看它的`compareAndSet`方法：
```java
    /**
     * Atomically sets the value to the given updated value
     * if the current value {@code ==} the expected value.
     *
     * @param expect the expected value
     * @param update the new value
     * @return true if successful. False return indicates that
     * the actual value was not equal to the expected value.
     */
public final boolean compareAndSet(int expect, int update) {
    return unsafe.compareAndSwapInt(this, valueOffset, expect, update);
}
```
可以看到，`compareAndSet()`调用的就是`Unsafe.compareAndSwapInt()`方法，即Unsafe类的CAS操作。

---

注意：

1. 子类相比于普通的锁，粒度更细、效率更高(除了高度竞争的情况下)
2. 果对于上面的示例代码中使用了thread.yield()之类的方法不清晰的，可以直接看下面的代码压测：
```java
public class AtomicIntegerTest implements Runnable {
 
    static AtomicInteger atomicInteger = new AtomicInteger(0);
 
    static int commonInteger = 0;
 
    public void addAtomicInteger() {
        atomicInteger.getAndIncrement();
    }
 
    public void addCommonInteger() {
        commonInteger++;
    }
 
    @Override
    public void run() {
        //可以调大10000看效果更明显
        for (int i = 0; i < 10000; i++) {
            addAtomicInteger();
            addCommonInteger();
        }
    }
 
    public static void main(String[] args) throws InterruptedException {
        AtomicIntegerTest atomicIntegerTest = new AtomicIntegerTest();
        Thread thread1 = new Thread(atomicIntegerTest);
        Thread thread2 = new Thread(atomicIntegerTest);
        thread1.start();
        thread2.start();
        //join()方法是为了让main主线程等待thread1、thread2两个子线程执行完毕
        thread1.join();
        thread2.join();
        System.out.println("AtomicInteger add result = " + atomicInteger.get());
        System.out.println("CommonInteger add result = " + commonInteger);
    }
}
```

3. 子类一览图参考如下：
| `Atomic*` | 基本类型原子类 | `AtomicInteger`<br />`AtomicLong`<br />`AtomicBoolean` |
| --- | --- | --- |
| `Atomic*Array` | 数组类型原子类 | `AtomicIntegerArray`<br />`AtomicLongArray`<br />`AtomicReferenceArray` |
| `Atomic*Reference` | 应用类型原子类 | `AtomicReference`<br />`AtomicStampedReference`<br />`AtomicMarkAbleReference` |
| `Atomic*FieldUpdater` | 升级类型原子类 | `AtomicIntegerFieldUpdater`<br />`AtomicLongFieldUpdater`<br />`AtomicReferenceFieldUpdater` |
| `*Adder` | 累加器 | `LongAdder`<br />`DoubleAdder` |
| `*Accumulator` | 累加器 | `LongAccumulator`<br />`DoubleAccumulator` |

4. 何把普通变量升级为原子变量？主要是`tomicIntegerFieldUpdater<T>`，参考如下代码：
```java
/**
 * @description 将普通变量升级为原子变量
 **/
public class AtomicIntegerFieldUpdaterTest implements Runnable {
 
    static Goods phone;
    static Goods computer;
 
    AtomicIntegerFieldUpdater<Goods> atomicIntegerFieldUpdater =
            AtomicIntegerFieldUpdater.newUpdater(Goods.class, "price");
 
    @Override
    public void run() {
        for (int i = 0; i < 10000; i++) {
            phone.price++;
            atomicIntegerFieldUpdater.getAndIncrement(computer);
        }
    }
 
    static class Goods {
        //商品定价
        volatile int price;
    }
 
    public static void main(String[] args) throws InterruptedException {
        phone = new Goods();
        computer = new Goods();
        AtomicIntegerFieldUpdaterTest atomicIntegerFieldUpdaterTest = new AtomicIntegerFieldUpdaterTest();
        Thread thread1 = new Thread(atomicIntegerFieldUpdaterTest);
        Thread thread2 = new Thread(atomicIntegerFieldUpdaterTest);
        thread1.start();
        thread2.start();
        //join()方法是为了让main主线程等待thread1、thread2两个子线程执行完毕
        thread1.join();
        thread2.join();
        System.out.println("CommonInteger price = " + phone.price);
        System.out.println("AtomicInteger price = " + computer.price);
    }
}
```
<a name="oznDq"></a>
### `LongAdder` VS `AtomicLong`
接下来，使用 Oracle 官方的 [JMH（Java Microbenchmark Harness， JAVA 微基准测试套件）](https://mp.weixin.qq.com/s?__biz=MzU1NTkwODE4Mw==&mid=2247486145&idx=1&sn=06e9e09018787f968fa05577e2663b42&scene=21#wechat_redirect)来测试一下两者的性能，测试代码如下：
```java
import org.openjdk.jmh.annotations.*;
import org.openjdk.jmh.infra.Blackhole;
import org.openjdk.jmh.runner.Runner;
import org.openjdk.jmh.runner.RunnerException;
import org.openjdk.jmh.runner.options.Options;
import org.openjdk.jmh.runner.options.OptionsBuilder;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.LongAdder;

@BenchmarkMode(Mode.AverageTime) // 测试完成时间
@OutputTimeUnit(TimeUnit.NANOSECONDS)
@Warmup(iterations = 1, time = 1, timeUnit = TimeUnit.SECONDS) // 预热 1 轮，每次 1s
@Measurement(iterations = 5, time = 5, timeUnit = TimeUnit.SECONDS) // 测试 5 轮，每次 3s
@Fork(1) // fork 1 个线程
@State(Scope.Benchmark)
@Threads(1000) // 开启 1000 个并发线程
public class AlibabaAtomicTest {
    public static void main(String[] args) throws RunnerException {
        // 启动基准测试
        Options opt = new OptionsBuilder()
                .include(AlibabaAtomicTest.class.getSimpleName()) // 要导入的测试类
                .build();
        new Runner(opt).run(); // 执行测试
    }
    @Benchmark
    public int atomicTest(Blackhole blackhole) throws InterruptedException {
        AtomicInteger atomicInteger = new AtomicInteger();
        for (int i = 0; i < 1024; i++) {
            atomicInteger.addAndGet(1);
        }
        // 为了避免 JIT 忽略未被使用的结果
        return atomicInteger.intValue();
    }
    @Benchmark
    public int longAdderTest(Blackhole blackhole) throws InterruptedException {
        LongAdder longAdder = new LongAdder();
        for (int i = 0; i < 1024; i++) {
            longAdder.add(1);
        }
        return longAdder.intValue();
    }
}
```
程序执行的结果为：<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1607477343881-a447c933-a7a3-4abc-ac6e-127f53d04a8b.webp#averageHue=%23373737&height=120&id=mBDcN&originHeight=120&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=1080)<br />从上述的数据可以看出，在开启了 1000 个线程之后，程序的 `LongAdder` 的性能比 `AtomicInteger` 快了约 1.53 倍，开了 1000 个线程，为什么要开这么多呢？这其实是为了模拟高并发高竞争的环境下二者的性能查询。<br />如果在低竞争下，比如开启 100 个线程，测试的结果如下：<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1607477343993-67cdaf83-20e9-4a21-a110-6c32be600236.webp#averageHue=%23373737&height=136&id=lAs1e&originHeight=136&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=1080)<br />在高度并发竞争情形下，`AtomicLong`每次进行`add`都需要`flush`和`refresh`（这一块涉及到java内存模型中的工作内存和主内存的，所有变量操作只能在工作内存中进行，然后写回主内存，其它线程再次读取新值），每次`add()`都需要同步，在高并发时会有比较多冲突，比较耗时导致效率低；而`LongAdder`中每个线程会维护自己的一个计数器，在最后执行`LongAdder.sum()`方法时候才需要同步，把所有计数器全部加起来，不需要`flush`和`refresh`操作。
:::success
结论：从上面结果可以看出，**在低竞争的并发环境下 **`**AtomicInteger**`** 的性能是要比 **`**LongAdder**`** 的性能好，而高竞争环境下 **`**LongAdder**`** 的性能比 **`**AtomicInteger**`** 好**，当有 1000 个线程运行时，`LongAdder` 的性能比 `AtomicInteger` 快了约 1.53 倍，所以要根据自己业务情况选择合适的类型来使用。
:::
<a name="oAixr"></a>
#### 性能分析
为什么会出现上面的情况？这是因为 `AtomicInteger` 在高并发环境下会有多个线程去竞争一个原子变量，而始终只有一个线程能竞争成功，而其他线程会一直通过 CAS 自旋尝试获取此原子变量，因此会有一定的性能消耗；而 `LongAdder` 会将这个原子变量分离成一个 Cell 数组，每个线程通过 Hash 获取到自己数组，这样就减少了乐观锁的重试次数，从而在高竞争下获得优势；而在低竞争下表现的又不是很好，可能是因为自己本身机制的执行时间大于了锁竞争的自旋时间，因此在低竞争下表现性能不如 `AtomicInteger`。
<a name="HeFDh"></a>
### 总结
测试了 `volatile` 在多写情况下是非线程安全的，而**在低竞争的并发环境下 **`**AtomicInteger**`** 的性能是要比 **`**LongAdder**`** 的性能好，而高竞争环境下 **`**LongAdder**`** 的性能比 **`**AtomicInteger**`** 好**，因此在使用时要结合自身的业务情况来选择相应的类型。
