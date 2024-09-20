JavaForkJoinPool<br />从一个简单的例子出发，与大家解释为啥要有 ForkJoinPool 的存在。接着向大家介绍 ForkJoinPool 的基本信息及使用，最后讲解 ForkJoinPool 的基本原理。
<a name="v5arJ"></a>
## 诞生原因
对于线程池来说，经常使用的是 ThreadPoolExecutor，可以用来提升任务处理效率。一般情况下，使用 ThreadPoolExecutor 的时候，各个任务之间都是没有联系的。但在某些特殊情况下，处理的任务之间是有联系的，例如经典的 Fibonacci 算法就是其中一种情况。<br />对于 Fibonacci 数列来说，我们知道 F (N) = F (N-1) + F (N-2)。当前数值的结果，都依赖后面几个数值的结果。这时候如果用 ThreadPoolExecutor 貌似就无法解决问题了。虽然可以单线程的递归算法，则其计算速度较慢，并且无法进行并行计算，无法发挥 CPU 多核的优势。<br />**ForkJoinPool 就是设计用来解决父子任务有依赖的并行计算问题的。** 类似于快速排序、二分查找、集合运算等有父子依赖的并行计算问题，都可以用 ForkJoinPool 来解决。对于 Fibonacci 数列问题，如果用 ForkJoinPool 来实现，其实现代码为：
```java
@Slf4j
public class ForkJoinDemo {
    // 1. 运行入口
    public static void main(String[] args) {
        int n = 20;

        // 为了追踪子线程名称，需要重写 ForkJoinWorkerThreadFactory 的方法
        final ForkJoinPool.ForkJoinWorkerThreadFactory factory = pool -> {
            final ForkJoinWorkerThread worker = ForkJoinPool.defaultForkJoinWorkerThreadFactory.newThread(pool);
            worker.setName("my-thread" + worker.getPoolIndex());
            return worker;
        };

        //创建分治任务线程池，可以追踪到线程名称
        ForkJoinPool forkJoinPool = new ForkJoinPool(4, factory, null, false);

        // 快速创建 ForkJoinPool 方法
        // ForkJoinPool forkJoinPool = new ForkJoinPool(4);

        //创建分治任务
        Fibonacci fibonacci = new Fibonacci(n);

        //调用 invoke 方法启动分治任务
        Integer result = forkJoinPool.invoke(fibonacci);
        log.info("Fibonacci {} 的结果是 {}", n, result);
    }
}

// 2. 定义拆分任务，写好拆分逻辑
@Slf4j
class Fibonacci extends RecursiveTask<Integer> {
    final int n;
    Fibonacci(int n) {
        this.n = n;
    }

    @Override
    public Integer compute() {
        //和递归类似，定义可计算的最小单元
        if (n <= 1) {
            return n;
        }
        // 想查看子线程名称输出的可以打开下面注释
        //log.info(Thread.currentThread().getName());

        Fibonacci f1 = new Fibonacci(n - 1);
        // 拆分成子任务
        f1.fork();
        Fibonacci f2 = new Fibonacci(n - 2);
        // f1.join 等待子任务执行结果
        return f2.compute() + f1.join();
    }
}
```
如上面代码所示，定义了一个 Fibonacci 类，继承了 RecursiveTask 抽象类。在 Fibonacci 类中，定义了拆分逻辑，并调用了 `join()` 等待子线程执行结果。运行程序，会得到如下的结果：
```
17:29:10.336 [main] INFO tech.shuyi.javacodechip.forkjoinpool.ForkJoinDemo - Fibonacci 20 的结果是 6765
```
上面代码中提到的 `fork()` 和 `join()` 是 ForkJoinPool 提供的 API 接口，主要用于执行任务以及等待子线程结果。关于其详细用法，稍后会讲到。<br />**除了用于处理父子任务有依赖的情形，其实 ForkJoinPool 也可以用于处理需要获取子任务执行结果的场景。** 例如：要计算 1 到 1 亿的和，为了加快计算的速度，自然想到算法中的分治原理，将 1 亿个数字分成 1 万个任务，每个任务计算 1 万个数值的综合，利用 CPU 的并发计算性能缩短计算时间。<br />因为 ThreadPoolExecutor 也可以通过 Future 获取执行结果，因此 ThreadPoolExecutor 也是可行的。这时候有两种实现方式，一种是用 ThreadPoolExecutor 实现，一种是用 ForkJoinPool 实现。下面将这两种方式都实现一下，看看这两种实现方式有什么不同。<br />无论哪种实现方式，其大致思路都是：

1. **按照线程池里线程个数 N，将 1 亿个数划分成 N 等份，随后丢入线程池进行计算。**
2. **每个计算任务使用 Future 接口获取计算结果，最后积加即可。**

先使用 ThreadPoolExecutor 实现。<br />首先，定义一个 Calculator 接口，表示计算数字总和这个动作，如下所示。
```java
public interface Calculator {
    /**
     * 把传进来的所有numbers 做求和处理
     *
     * @param numbers
     * @return 总和
     */
    long sumUp(long[] numbers);
}
```
接着，定义一个使用 ThreadPoolExecutor 线程池实现的类，如下所示。
```java
package tech.shuyi.javacodechip.forkjoinpool;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

public class ExecutorServiceCalculator implements Calculator {

    private int parallism;
    private ExecutorService pool;

    public ExecutorServiceCalculator() {
        // CPU的核心数 默认就用cpu核心数了
        parallism = Runtime.getRuntime().availableProcessors(); 
        pool = Executors.newFixedThreadPool(parallism);
    }

    // 1. 处理计算任务的线程
    private static class SumTask implements Callable<Long> {
        private long[] numbers;
        private int from;
        private int to;

        public SumTask(long[] numbers, int from, int to) {
            this.numbers = numbers;
            this.from = from;
            this.to = to;
        }

        @Override
        public Long call() {
            long total = 0;
            for (int i = from; i <= to; i++) {
                total += numbers[i];
            }
            return total;
        }
    }

    // 2. 核心业务逻辑实现
    @Override
    public long sumUp(long[] numbers) {
        List<Future<Long>> results = new ArrayList<>();

        // 2.1 数字拆分
        // 把任务分解为 n 份，交给 n 个线程处理   4核心 就等分成4份呗
        // 然后把每一份都扔个一个SumTask线程 进行处理
        int part = numbers.length / parallism;
        for (int i = 0; i < parallism; i++) {
            int from = i * part; //开始位置
            int to = (i == parallism - 1) ? numbers.length - 1 : (i + 1) * part - 1; //结束位置

            //扔给线程池计算
            results.add(pool.submit(new SumTask(numbers, from, to)));
        }

        // 2.2 阻塞等待结果
        // 把每个线程的结果相加，得到最终结果 get()方法 是阻塞的
        // 优化方案：可以采用CompletableFuture来优化  JDK1.8的新特性
        long total = 0L;
        for (Future<Long> f : results) {
            try {
                total += f.get();
            } catch (Exception ignore) {
            }
        }

        return total;
    }
}
```
如上面代码所示，实现了一个计算单个任务的类 SumTask，在该类中对数值进行累加。其次，在 `sumUp()` 方法中，对 1 亿的数字进行拆分，接着扔给线程池计算，最后阻塞等待计算结果，最终累加起来。<br />运行上面的代码，可以得到顺利得到最终结果，如下所示。
```
耗时：10ms
结果为：50000005000000
```
接着使用 ForkJoinPool 来实现。<br />首先实现 SumTask 继承 RecursiveTask 抽象类，并在 `compute()` 方法中定义拆分逻辑及计算。最后在 `sumUp()` 方法中调用 pool 方法进行计算，代码如下所示。
```java
public class ForkJoinCalculator implements Calculator {

    private ForkJoinPool pool;

    // 1. 定义计算逻辑
    private static class SumTask extends RecursiveTask<Long> {
        private long[] numbers;
        private int from;
        private int to;

        public SumTask(long[] numbers, int from, int to) {
            this.numbers = numbers;
            this.from = from;
            this.to = to;
        }

        //此方法为ForkJoin的核心方法：对任务进行拆分  拆分的好坏决定了效率的高低
        @Override
        protected Long compute() {

            // 当需要计算的数字个数小于6时，直接采用for loop方式计算结果
            if (to - from < 6) {
                long total = 0;
                for (int i = from; i <= to; i++) {
                    total += numbers[i];
                }
                return total;
            } else { 
                // 否则，把任务一分为二，递归拆分(注意此处有递归)到底拆分成多少分 需要根据具体情况而定
                int middle = (from + to) / 2;
                SumTask taskLeft = new SumTask(numbers, from, middle);
                SumTask taskRight = new SumTask(numbers, middle + 1, to);
                taskLeft.fork();
                taskRight.fork();
                return taskLeft.join() + taskRight.join();
            }
        }
    }

    public ForkJoinCalculator() {
        // 也可以使用公用的线程池 ForkJoinPool.commonPool()：
        // pool = ForkJoinPool.commonPool()
        pool = new ForkJoinPool();
    }

    @Override
    public long sumUp(long[] numbers) {
        Long result = pool.invoke(new SumTask(numbers, 0, numbers.length - 1));
        pool.shutdown();
        return result;
    }
}
```
运行上面的代码，结果为：
```
耗时：860ms
结果为：50000005000000
```
对比 ThreadPoolExecutor 和 ForkJoinPool 这两者的实现，可以发现它们都有任务拆分的逻辑，以及最终合并数值的逻辑。但 ForkJoinPool 相比 ThreadPoolExecutor 来说，做了一些实现上的封装，例如：

- **不用手动去获取子任务的结果，而是使用 **`**join()**`** 方法直接获取结果。**
- **将任务拆分的逻辑，封装到 RecursiveTask 实现类中，而不是裸露在外。**

因此对于没有父子任务依赖，但是希望获取到子任务执行结果的并行计算任务，也可以使用 ForkJoinPool 来实现。**在这种情况下，使用 ForkJoinPool 实现更多是代码实现方便，封装做得更加好。**
<a name="Xv2M9"></a>
## 使用指南
使用 ForkJoinPool 来进行并行计算，主要分为两步：

1. **定义 RecursiveTask 或 RecursiveAction 的任务子类。**
2. **初始化线程池及计算任务，丢入线程池处理，取得处理结果。**

**首先，需要定义一个 RecursiveTask 或 RecursiveAction 的子类，然后再该类的 **`**compute()**`** 方法中定义拆分逻辑和计算逻辑。** 这两个抽象类的区别在于：前者有返回值，后者没有返回值。例如前面讲到的 1 到 1 亿的叠加问题，其定义的 RecursiveTask 实现类 SumTask 的代码如下：
```java
private static class SumTask extends RecursiveTask<Long> {
    private long[] numbers;
    private int from;
    private int to;

    public SumTask(long[] numbers, int from, int to) {
        this.numbers = numbers;
        this.from = from;
        this.to = to;
    }

    @Override
    protected Long compute() {
        // 1. 定义拆分退出逻辑
        if (to - from < 6) {
            long total = 0;
            for (int i = from; i <= to; i++) {
                total += numbers[i];
            }
            return total;
        } else {
            // 2. 定义计算逻辑
            int middle = (from + to) / 2;
            SumTask taskLeft = new SumTask(numbers, from, middle);
            SumTask taskRight = new SumTask(numbers, middle + 1, to);
            taskLeft.fork();
            taskRight.fork();
            return taskLeft.join() + taskRight.join();
        }
    }
}
```
对于 `compute()` 方法的实现，核心是想清楚：怎么拆分成子任务？什么时候结束拆分？<br />**接着，初始化 ForkJoinPool 线程池，初始化计算任务，最后将任务丢入线程池中。**
```java
// 初始化线程池
public ForkJoinCalculator() {
    pool = new ForkJoinPool();
}
// 初始化计算任务，将任务丢入线程池
public long sumUp(long[] numbers) {
    Long result = pool.invoke(new SumTask(numbers, 0, numbers.length - 1));
    pool.shutdown();
    return result;
}
```
通过上面两步操作，就完成了一个 ForkJoinPool 任务代码的编写。
<a name="zZFDL"></a>
## 原理解析
ForkJoinPool 的设计思想是分治算法，即将任务不断拆分（fork）成更小的任务，最终再合并（join）各个任务的计算结果。通过这种方式，可以充分利用 CPU 资源，再结合工作窃取算法（worksteal）整体提高执行效率。其简单的流程如下图：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1664152004476-48f9cf64-f955-4061-9463-01093c09a540.jpeg#clientId=u3047892e-4762-4&from=paste&id=u94dcfa7e&originHeight=828&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2a413462-6a2a-4380-9ddc-9913a14766d&title=)<br />从图中可以看出 ForkJoinPool 要先执行完子任务才能执行上一层任务。**因此 ForkJoinPool 最适合有父子任务依赖的场景，其次就是需要获取子任务执行结果的场景。比如：Fibonacci 数列、快速排序、二分查找等。**
<a name="hbJpU"></a>
### 源码实现
ForkJoinPool 的主要实现类为：ForkJoinPool 和 ForkJoinTask 抽象类。<br />ForkJoinTask 实现了 Future 接口，可以用于获取处理结果。ForkJoinTask 有两个抽象子类：RecursiveAction 和 RecursiveTask 抽象类，其区别在于前者没有返回值，后者有返回值，其类图如下所示。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1664152004467-778f7062-591b-428a-bf3f-8a07268da8cc.jpeg#clientId=u3047892e-4762-4&from=paste&id=ud0d4bac0&originHeight=414&originWidth=732&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4791357c-55ee-48de-b931-949a01afdb3&title=)<br />ForkJoinPool 则是具体的逻辑实现，由于暂时没有应用场景，就不了解这么深了，这里就不深入解析了。
<a name="SwKNj"></a>
### 窃取算法
ForkJoinPool 的父子任务之间是有依赖关系的，那么 ForkJoinPool 是如何实现的呢？**答案是：利用不同任务队列执行。** 在 ForkJoinPool 中有一个数组形式的成员变量 `workQueue[]`，其对应一个队列数组，每个队列对应一个消费线程。丢入线程池的任务，根据特定规则进行转发。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1664152004609-92c46999-db77-4fd7-ac0d-ebc84ea89629.jpeg#clientId=u3047892e-4762-4&from=paste&id=u2a1a6893&originHeight=465&originWidth=732&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u80613305-bbd3-4154-8360-4d13149b5c6&title=)<br />这样就有一个问题：有些队列可能任务比较多，有些队列任务比较少，这样就会导致不同线程负载不一样，整体不够高效，怎么办呢？<br />**答案是：利用窃取算法，空闲的线程从尾部去消费其他队列的任务。**<br />一般情况下，线程获取自己队列中的任务是 LIFO（Last Input First Output 后进先出）的方式，即类似于栈的操作方式。如下图所示，首先放入队列的时候先将任务 Push 进队列的头部（top），之后消费的时候在 pop 出队列头部（top）。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1664152004482-01c3df0a-66ba-4fc2-a309-1db81495cb8a.jpeg#clientId=u3047892e-4762-4&from=paste&id=uac4c7c93&originHeight=408&originWidth=732&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4b486689-ecae-494d-b86e-54c2ad02e92&title=)<br />而当某个线程对应的队列空闲时，该线程则去队列的底部（base）窃取（poll）任务到自己的队列，然后进行消费。**那问题来了：为什么不从头部（top）获取任务，而要从底部（base）获取任务呢？** 那是为了避免冲突！如果两个线程同时从顶部获取任务，那就会有多线程的冲突问题，就需要加锁操作，从而降低了执行效率。
