Java
<a name="UjLGz"></a>
## 主要内容

1. 4种方式实现计数器功能，对比其性能
2. 介绍`LongAdder`
3. 介绍`LongAccumulator`
<a name="ubG4a"></a>
## 来个需求
**一个jvm中实现一个计数器功能，需保证多线程情况下数据正确性。**<br />来模拟50个线程，每个线程对计数器递增100万次，最终结果应该是5000万。<br />使用4种方式实现，看一下其性能，然后引出为什么需要使用`LongAdder`、`LongAccumulator`。
<a name="M8cIV"></a>
## 方式一：`synchronized`方式实现
```java
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.atomic.LongAccumulator;

public class Demo1 {
    static int count = 0;

    public static synchronized void incr() {
        count++;
    }

    public static void main(String[] args) throws ExecutionException, InterruptedException {
        for (int i = 0; i < 10; i++) {
            count = 0;
            m1();
        }
    }

    private static void m1() throws InterruptedException {
        long t1 = System.currentTimeMillis();
        int threadCount = 50;
        CountDownLatch countDownLatch = new CountDownLatch(threadCount);
        for (int i = 0; i < threadCount; i++) {
            new Thread(() -> {
                try {
                    for (int j = 0; j < 1000000; j++) {
                        incr();
                    }
                } finally {
                    countDownLatch.countDown();
                }
            }).start();
        }
        countDownLatch.await();
        long t2 = System.currentTimeMillis();
        System.out.println(String.format("结果：%s,耗时(ms)：%s", count, (t2 - t1)));
    }
}
```
输出：
```
结果：50000000,耗时(ms)：1437
结果：50000000,耗时(ms)：1913
结果：50000000,耗时(ms)：386
结果：50000000,耗时(ms)：383
结果：50000000,耗时(ms)：381
结果：50000000,耗时(ms)：382
结果：50000000,耗时(ms)：379
结果：50000000,耗时(ms)：379
结果：50000000,耗时(ms)：392
结果：50000000,耗时(ms)：384
```
**平均耗时：390毫秒**
<a name="Gcz3C"></a>
## 方式2：`AtomicLong`实现
```java
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.atomic.AtomicLong;

public class Demo2 {
    static AtomicLong count = new AtomicLong(0);

    public static void incr() {
        count.incrementAndGet();
    }

    public static void main(String[] args) throws ExecutionException, InterruptedException {
        for (int i = 0; i < 10; i++) {
            count.set(0);
            m1();
        }
    }

    private static void m1() throws InterruptedException {
        long t1 = System.currentTimeMillis();
        int threadCount = 50;
        CountDownLatch countDownLatch = new CountDownLatch(threadCount);
        for (int i = 0; i < threadCount; i++) {
            new Thread(() -> {
                try {
                    for (int j = 0; j < 1000000; j++) {
                        incr();
                    }
                } finally {
                    countDownLatch.countDown();
                }
            }).start();
        }
        countDownLatch.await();
        long t2 = System.currentTimeMillis();
        System.out.println(String.format("结果：%s,耗时(ms)：%s", count, (t2 - t1)));
    }
}
```
输出：
```
结果：50000000,耗时(ms)：971
结果：50000000,耗时(ms)：915
结果：50000000,耗时(ms)：920
结果：50000000,耗时(ms)：923
结果：50000000,耗时(ms)：910
结果：50000000,耗时(ms)：916
结果：50000000,耗时(ms)：923
结果：50000000,耗时(ms)：916
结果：50000000,耗时(ms)：912
结果：50000000,耗时(ms)：908
```
**平均耗时：920毫秒**<br />`AtomicLong`内部采用CAS的方式实现，并发量大的情况下，CAS失败率比较高，导致性能比`synchronized`还低一些。并发量不是太大的情况下，CAS性能还是可以的。
<a name="XIzNd"></a>
## 方式3：`LongAdder`实现
先介绍一下`LongAdder`，说到`LongAdder`，不得不提的就是`AtomicLong`，`AtomicLong`是JDK1.5开始出现的，里面主要使用了一个long类型的value作为成员变量，然后使用循环的CAS操作去操作value的值，并发量比较大的情况下，CAS操作失败的概率较高，内部失败了会重试，导致耗时可能会增加。<br />`**LongAdder**`**是JDK1.8开始出现的，所提供的API基本上可以替换掉原先的**`**AtomicLong**`。`LongAdder`在并发量比较大的情况下，操作数据的时候，相当于把这个数字分成了很多份数字，然后交给多个人去管控，每个管控者负责保证部分数字在多线程情况下操作的正确性。当多线程访问的时，通过hash算法映射到具体管控者去操作数据，最后再汇总所有的管控者的数据，得到最终结果。相当于降低了并发情况下锁的粒度，所以效率比较高，看一下下面的图，方便理解：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1691902790381-82b9b5af-4b11-411f-bb4f-01a6fb6c0a7d.png#averageHue=%23f8f8f8&clientId=u60b82c94-5ecb-4&from=paste&height=554&id=u1f864884&originHeight=1385&originWidth=1398&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=182384&status=done&style=none&taskId=u448b97e0-02ea-404d-b4b8-aa152d573fc&title=&width=559.2)<br />代码：
```java
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.atomic.AtomicLong;
import java.util.concurrent.atomic.LongAdder;

public class Demo3 {
    static LongAdder count = new LongAdder();

    public static void incr() {
        count.increment();
    }

    public static void main(String[] args) throws ExecutionException, InterruptedException {
        for (int i = 0; i < 10; i++) {
            count.reset();
            m1();
        }
    }

    private static void m1() throws ExecutionException, InterruptedException {
        long t1 = System.currentTimeMillis();
        int threadCount = 50;
        CountDownLatch countDownLatch = new CountDownLatch(threadCount);
        for (int i = 0; i < threadCount; i++) {
            new Thread(() -> {
                try {
                    for (int j = 0; j < 1000000; j++) {
                        incr();
                    }
                } finally {
                    countDownLatch.countDown();
                }
            }).start();
        }
        countDownLatch.await();
        long t2 = System.currentTimeMillis();
        System.out.println(String.format("结果：%s,耗时(ms)：%s", count.sum(), (t2 - t1)));
    }
}
```
输出：
```
结果：50000000,耗时(ms)：206
结果：50000000,耗时(ms)：105
结果：50000000,耗时(ms)：107
结果：50000000,耗时(ms)：107
结果：50000000,耗时(ms)：105
结果：50000000,耗时(ms)：99
结果：50000000,耗时(ms)：106
结果：50000000,耗时(ms)：102
结果：50000000,耗时(ms)：106
结果：50000000,耗时(ms)：102
```
**平均耗时：100毫秒**<br />代码中`new LongAdder()`创建一个`LongAdder`对象，内部数字初始值是0，调用`increment()`方法可以对`LongAdder`内部的值原子递增1。`reset()`方法可以重置`LongAdder`的值，使其归0。
<a name="Qokr0"></a>
## 方式4：`LongAccumulator`实现
`**LongAccumulator**`**介绍**<br />`LongAccumulator`是`LongAdder`的功能增强版。`LongAdder`的API只有对数值的加减，而`LongAccumulator`提供了自定义的函数操作，其构造函数如下：
```java
/**
  * accumulatorFunction：需要执行的二元函数（接收2个long作为形参，并返回1个long）
  * identity：初始值
 **/
public LongAccumulator(LongBinaryOperator accumulatorFunction, long identity) {
    this.function = accumulatorFunction;
    base = this.identity = identity;
}
```
示例代码：
```java
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.atomic.LongAccumulator;
import java.util.concurrent.atomic.LongAdder;

public class Demo4 {
    static LongAccumulator count = new LongAccumulator((x, y) -> x + y, 0L);

    public static void incr() {
        count.accumulate(1);
    }

    public static void main(String[] args) throws ExecutionException, InterruptedException {
        for (int i = 0; i < 10; i++) {
            count.reset();
            m1();
        }
    }

    private static void m1() throws ExecutionException, InterruptedException {
        long t1 = System.currentTimeMillis();
        int threadCount = 50;
        CountDownLatch countDownLatch = new CountDownLatch(threadCount);
        for (int i = 0; i < threadCount; i++) {
            new Thread(() -> {
                try {
                    for (int j = 0; j < 1000000; j++) {
                        incr();
                    }
                } finally {
                    countDownLatch.countDown();
                }
            }).start();
        }
        countDownLatch.await();
        long t2 = System.currentTimeMillis();
        System.out.println(String.format("结果：%s,耗时(ms)：%s", count.longValue(), (t2 - t1)));
    }
}
```
输出：
```
结果：50000000,耗时(ms)：138
结果：50000000,耗时(ms)：111
结果：50000000,耗时(ms)：111
结果：50000000,耗时(ms)：103
结果：50000000,耗时(ms)：103
结果：50000000,耗时(ms)：105
结果：50000000,耗时(ms)：101
结果：50000000,耗时(ms)：106
结果：50000000,耗时(ms)：102
结果：50000000,耗时(ms)：103
```
**平均耗时：100毫秒**<br />`LongAccumulator`的效率和`LongAdder`差不多，不过更灵活一些。<br />调用`new LongAdder()`等价于`new LongAccumulator((x, y) -> x + y, 0L)`。<br />从上面4个示例的结果来看，`LongAdder`、`LongAccumulator`全面超越同步锁及`AtomicLong`的方式，建议在使用`AtomicLong`的地方可以直接替换为`LongAdder`、`LongAccumulator`，吞吐量更高一些。
