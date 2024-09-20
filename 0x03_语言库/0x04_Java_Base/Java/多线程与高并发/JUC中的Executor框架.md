Java
<a name="dRgj9"></a>
## 主要内容

1. 介绍`Executor`框架相关内容
2. 介绍`Executor`
3. 介绍`ExecutorService`
4. 介绍线程池`ThreadPoolExecutor`及案例
5. 介绍定时器`ScheduledExecutorService`及案例
6. 介绍`Excecutors`类的使用
7. 介绍`Future`接口
8. 介绍`Callable`接口
9. 介绍`FutureTask`的使用
10. 获取异步任务的执行结果的几种方法
<a name="GgbMf"></a>
## `Executors`框架介绍
`Executors`框架是Doug Lea的神作，通过这个框架，可以很容易的使用线程池高效地处理并行任务。<br />`**Excecutor**`**框架主要包含3部分的内容：**

1. 任务相关的：包含被执行的任务要实现的接口：`**Runnable**`接口或`**Callable**`接口
2. 任务的执行相关的：包含任务执行机制的**核心接口**`**Executor**`，以及继承自`Executor`的`ExecutorService`接口。`Executor`框架中有两个关键的类实现了`ExecutorService`接口（`ThreadPoolExecutor`和`ScheduledThreadPoolExecutor`）
3. 异步计算结果相关的：包含**接口**`**Future**`和**实现**`**Future**`**接口的**`**FutureTask**`**类**

`**Executors**`**框架包括：**

- `Executor`
- `ExecutorService`
- `ThreadPoolExecutor`
- `Executors`
- `Future`
- `Callable`
- `FutureTask`
- `CompletableFuture`
- `CompletionService`
- `ExecutorCompletionService`

下面来一个个介绍其用途和使用方法。
<a name="ExxSo"></a>
## `Executor`接口
`Executor`接口中定义了方法`execute(Runable able)`接口，该方法接受一个`Runable`实例，他来执行一个任务，任务即实现一个`Runable`接口的类。
<a name="sJZCX"></a>
## `ExecutorService`接口
`ExecutorService`继承于`Executor`接口，他提供了更为丰富的线程实现方法，比如`ExecutorService`提供关闭自己的方法，以及为跟踪一个或多个异步任务执行状况而生成`Future`的方法。<br />`ExecutorService`有三种状态：运行、关闭、终止。创建后便进入运行状态，当调用了`shutdown()`方法时，便进入了关闭状态，此时意味着`ExecutorService`不再接受新的任务，但是他还是会执行已经提交的任务，当所有已经提交了的任务执行完后，便达到终止状态。如果不调用`shutdown`方法，`ExecutorService`方法会一直运行下去，系统一般不会主动关闭。
<a name="tn86E"></a>
## `ThreadPoolExecutor`类
线程池类，实现了`ExecutorService`接口中所有方法，该类也是经常要用到的，非常重要。
<a name="cmXCU"></a>
## `ScheduledThreadPoolExecutor`定时器
`ScheduledThreadPoolExecutor`继承自`ThreadPoolExecutor`，他主要用来延迟执行任务，或者定时执行任务。功能和Timer类似，但是`ScheduledThreadPoolExecutor`更强大、更灵活一些。Timer后台是单个线程，而`ScheduledThreadPoolExecutor`可以在创建的时候指定多个线程。<br />常用方法介绍：
<a name="qR3ev"></a>
### `schedule`：延迟执行任务1次
使用`ScheduledThreadPoolExecutor`的`schedule`方法，看一下这个方法的声明：
```java
public ScheduledFuture<?> schedule(Runnable command, long delay, TimeUnit unit)
```
3个参数：<br />`command`：需要执行的任务<br />`delay`：需要延迟的时间<br />`unit`：参数2的时间单位，是个枚举，可以是天、小时、分钟、秒、毫秒、纳秒等<br />**示例代码：**
```java
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

public class Demo1 {
    public static void main(String[] args) throws ExecutionException, InterruptedException {
        System.out.println(System.currentTimeMillis());
        ScheduledExecutorService scheduledExecutorService = Executors.newScheduledThreadPool(10);
        scheduledExecutorService.schedule(() -> {
            System.out.println(System.currentTimeMillis() + "开始执行");
            //模拟任务耗时
            try {
                TimeUnit.SECONDS.sleep(3);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println(System.currentTimeMillis() + "执行结束");
        }, 2, TimeUnit.SECONDS);
    }
}
```
输出：
```
1564575180457
1564575185525开始执行
1564575188530执行结束
```
<a name="qINz0"></a>
### `scheduleAtFixedRate`：固定的频率执行任务
使用`ScheduledThreadPoolExecutor`的`scheduleAtFixedRate`方法，该方法设置了执行周期，下一次执行时间相当于是上一次的执行时间加上period，任务每次执行完毕之后才会计算下次的执行时间。<br />看一下这个方法的声明：
```java
public ScheduledFuture<?> scheduleAtFixedRate(Runnable command,
                                              long initialDelay,
                                              long period,
                                              TimeUnit unit);
```
4个参数：<br />`command`：表示要执行的任务<br />`initialDelay`：表示延迟多久执行第一次<br />`period`：连续执行之间的时间间隔<br />`unit`：参数2和参数3的时间单位，是个枚举，可以是天、小时、分钟、秒、毫秒、纳秒等<br />假设系统调用scheduleAtFixedRate的时间是T1，那么执行时间如下：<br />第1次：T1+initialDelay<br />第2次：T1+initialDelay+period<br />第3次：T1+initialDelay+2*period<br />第n次：T1+initialDelay+(n-1)*period<br />**示例代码：**
```java
import java.sql.Time;
import java.util.concurrent.*;
import java.util.concurrent.atomic.AtomicInteger;

public class Demo2 {
    public static void main(String[] args) throws ExecutionException, InterruptedException {
        System.out.println(System.currentTimeMillis());
        //任务执行计数器
        AtomicInteger count = new AtomicInteger(1);
        ScheduledExecutorService scheduledExecutorService = Executors.newScheduledThreadPool(10);
        scheduledExecutorService.scheduleAtFixedRate(() -> {
            int currCount = count.getAndIncrement();
            System.out.println(Thread.currentThread().getName());
            System.out.println(System.currentTimeMillis() + "第" + currCount + "次" + "开始执行");
            try {
                TimeUnit.SECONDS.sleep(2);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println(System.currentTimeMillis() + "第" + currCount + "次" + "执行结束");
        }, 1, 1, TimeUnit.SECONDS);
    }
}
```
前面6次输出结果：
```
1564576404181
pool-1-thread-1
1564576405247第1次开始执行
1564576407251第1次执行结束
pool-1-thread-1
1564576407251第2次开始执行
1564576409252第2次执行结束
pool-1-thread-2
1564576409252第3次开始执行
1564576411255第3次执行结束
pool-1-thread-1
1564576411256第4次开始执行
1564576413260第4次执行结束
pool-1-thread-3
1564576413260第5次开始执行
1564576415265第5次执行结束
pool-1-thread-2
1564576415266第6次开始执行
1564576417269第6次执行结束
```
代码中设置的任务第一次执行时间是系统启动之后延迟一秒执行。后面每次时间间隔1秒，从输出中可以看出系统启动之后过了1秒任务第一次执行（1、3行输出），输出的结果中可以看到任务第一次执行结束时间和第二次的结束时间一样，为什么会这样？前面有介绍，任务当前执行完毕之后会计算下次执行时间，下次执行时间为上次执行的开始时间+period，第一次开始执行时间是1564576405247，加1秒为1564576406247，这个时间小于第一次结束的时间了，说明小于系统当前时间了，会立即执行。
<a name="olngl"></a>
### `scheduleWithFixedDelay`：固定的间隔执行任务
使用`ScheduledThreadPoolExecutor`的`scheduleWithFixedDelay`方法，该方法设置了执行周期，与`scheduleAtFixedRate`方法不同的是，下一次执行时间是上一次任务执行完的系统时间加上period，因而具体执行时间不是固定的，但周期是固定的，是采用相对固定的延迟来执行任务。看一下这个方法的声明：
```java
public ScheduledFuture<?> scheduleWithFixedDelay(Runnable command,
                                                 long initialDelay,
                                                 long delay,
                                                 TimeUnit unit);
```
4个参数：<br />`command`：表示要执行的任务<br />`initialDelay`：表示延迟多久执行第一次<br />`period`：表示下次执行时间和上次执行结束时间之间的间隔时间<br />`unit`：参数2和参数3的时间单位，是个枚举，可以是天、小时、分钟、秒、毫秒、纳秒等<br />假设系统调用`scheduleAtFixedRate`的时间是T1，那么执行时间如下：<br />第1次：T1+initialDelay，执行结束时间：E1<br />第2次：E1+period，执行结束时间：E2<br />第3次：E2+period，执行结束时间：E3<br />第4次：E3+period，执行结束时间：E4<br />第n次：上次执行结束时间+period<br />**示例代码：**
```java
import java.util.concurrent.*;
import java.util.concurrent.atomic.AtomicInteger;

public class Demo3 {
    public static void main(String[] args) throws ExecutionException, InterruptedException {
        System.out.println(System.currentTimeMillis());
        //任务执行计数器
        AtomicInteger count = new AtomicInteger(1);
        ScheduledExecutorService scheduledExecutorService = Executors.newScheduledThreadPool(10);
        scheduledExecutorService.scheduleWithFixedDelay(() -> {
            int currCount = count.getAndIncrement();
            System.out.println(Thread.currentThread().getName());
            System.out.println(System.currentTimeMillis() + "第" + currCount + "次" + "开始执行");
            try {
                TimeUnit.SECONDS.sleep(2);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println(System.currentTimeMillis() + "第" + currCount + "次" + "执行结束");
        }, 1, 3, TimeUnit.SECONDS);
    }
}
```
前几次输出如下：
```
1564578510983
pool-1-thread-1
1564578512087第1次开始执行
1564578514091第1次执行结束
pool-1-thread-1
1564578517096第2次开始执行
1564578519100第2次执行结束
pool-1-thread-2
1564578522103第3次开始执行
1564578524105第3次执行结束
pool-1-thread-1
1564578527106第4次开始执行
1564578529106第4次执行结束
```
延迟1秒之后执行第1次，后面每次的执行时间和上次执行结束时间间隔3秒。<br />`scheduleAtFixedRate`和`scheduleWithFixedDelay`示例建议多看2遍。
<a name="jDFPC"></a>
### 定时任务有异常会怎么样？
示例代码：
```java
import java.util.concurrent.*;
import java.util.concurrent.atomic.AtomicInteger;

public class Demo4 {
    public static void main(String[] args) throws ExecutionException, InterruptedException {
        System.out.println(System.currentTimeMillis());
        //任务执行计数器
        AtomicInteger count = new AtomicInteger(1);
        ScheduledExecutorService scheduledExecutorService = Executors.newScheduledThreadPool(10);
        ScheduledFuture<?> scheduledFuture = scheduledExecutorService.scheduleWithFixedDelay(() -> {
            int currCount = count.getAndIncrement();
            System.out.println(Thread.currentThread().getName());
            System.out.println(System.currentTimeMillis() + "第" + currCount + "次" + "开始执行");
            System.out.println(10 / 0);
            System.out.println(System.currentTimeMillis() + "第" + currCount + "次" + "执行结束");
        }, 1, 1, TimeUnit.SECONDS);

        TimeUnit.SECONDS.sleep(5);
        System.out.println(scheduledFuture.isCancelled());
        System.out.println(scheduledFuture.isDone());

    }
}
```
系统输出如下内容就再也没有输出了：
```
1564578848143
pool-1-thread-1
1564578849226第1次开始执行
false
true
```
**先说补充点知识**：`schedule`、`scheduleAtFixedRate`、`scheduleWithFixedDelay`这几个方法有个返回值`ScheduledFuture`，通过`ScheduledFuture`可以对执行的任务做一些操作，如判断任务是否被取消、是否执行完成。<br />再回到上面代码，任务中有个10/0的操作，会触发异常，发生异常之后没有任何现象，被`ScheduledExecutorService`内部给吞掉了，然后这个任务再也不会执行了，`scheduledFuture.isDone()`输出true，表示这个任务已经结束了，再也不会被执行了。**所以如果程序有异常，开发者自己注意处理一下，不然跑着跑着发现任务怎么不跑了，也没有异常输出。**
<a name="U456W"></a>
### 取消定时任务的执行
可能任务执行一会，想取消执行，可以调用`ScheduledFuture`的`cancel`方法，参数表示是否给任务发送中断信号。
```java
import java.util.concurrent.*;
import java.util.concurrent.atomic.AtomicInteger;

public class Demo5 {
    public static void main(String[] args) throws ExecutionException, InterruptedException {
        System.out.println(System.currentTimeMillis());
        //任务执行计数器
        AtomicInteger count = new AtomicInteger(1);
        ScheduledExecutorService scheduledExecutorService = Executors.newScheduledThreadPool(1);
        ScheduledFuture<?> scheduledFuture = scheduledExecutorService.scheduleWithFixedDelay(() -> {
            int currCount = count.getAndIncrement();
            System.out.println(Thread.currentThread().getName());
            System.out.println(System.currentTimeMillis() + "第" + currCount + "次" + "开始执行");
            try {
                TimeUnit.SECONDS.sleep(2);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println(System.currentTimeMillis() + "第" + currCount + "次" + "执行结束");
        }, 1, 1, TimeUnit.SECONDS);

        TimeUnit.SECONDS.sleep(5);
        scheduledFuture.cancel(false);
        TimeUnit.SECONDS.sleep(1);
        System.out.println("任务是否被取消："+scheduledFuture.isCancelled());
        System.out.println("任务是否已完成："+scheduledFuture.isDone());
    }
}
```
输出：
```
1564579843190
pool-1-thread-1
1564579844255第1次开始执行
1564579846260第1次执行结束
pool-1-thread-1
1564579847263第2次开始执行
任务是否被取消：true
任务是否已完成：true
1564579849267第2次执行结束
```
输出中可以看到任务被取消成功了。
<a name="QMfHi"></a>
## `Executors`类
`Executors`类，提供了一系列工厂方法用于创建线程池，返回的线程池都实现了`ExecutorService`接口。常用的方法有：
<a name="MjToT"></a>
### `newSingleThreadExecutor`
```java
public static ExecutorService newSingleThreadExecutor()
public static ExecutorService newSingleThreadExecutor(ThreadFactory threadFactory)
```
创建一个单线程的线程池。这个线程池只有一个线程在工作，也就是相当于单线程串行执行所有任务。如果这个唯一的线程因为异常结束，那么会有一个新的线程来替代它。此线程池保证所有任务的执行顺序按照任务的提交顺序执行。内部使用了无限容量的`LinkedBlockingQueue`阻塞队列来缓存任务，任务如果比较多，单线程如果处理不过来，会导致队列堆满，引发OOM。
<a name="dWOOi"></a>
### `newFixedThreadPool`
```java
public static ExecutorService newFixedThreadPool(int nThreads)
public static ExecutorService newFixedThreadPool(int nThreads, ThreadFactory threadFactory)
```
创建固定大小的线程池。每次提交一个任务就创建一个线程，直到线程达到线程池的最大大小。线程池的大小一旦达到最大值就会保持不变，在提交新任务，任务将会进入等待队列中等待。如果某个线程因为执行异常而结束，那么线程池会补充一个新线程。内部使用了无限容量的`LinkedBlockingQueue`阻塞队列来缓存任务，任务如果比较多，如果处理不过来，会导致队列堆满，引发OOM。
<a name="BkLxL"></a>
### `newCachedThreadPool`
```java
public static ExecutorService newCachedThreadPool()
public static ExecutorService newCachedThreadPool(ThreadFactory threadFactory)
```
创建一个可缓存的线程池。如果线程池的大小超过了处理任务所需要的线程，<br />那么就会回收部分空闲（60秒处于等待任务到来）的线程，当任务数增加时，此线程池又可以智能的添加新线程来处理任务。此线程池的最大值是`Integer`的最大值(2^31-1)。内部使用了`SynchronousQueue`同步队列来缓存任务，此队列的特性是放入任务时必须要有对应的线程获取任务，任务才可以放入成功。如果处理的任务比较耗时，任务来的速度也比较快，会创建太多的线程引发OOM。
<a name="sxE13"></a>
### `newScheduledThreadPool`
```java
public static ScheduledExecutorService newScheduledThreadPool(int corePoolSize)
public static ScheduledExecutorService newScheduledThreadPool(int corePoolSize, ThreadFactory threadFactory)
```
创建一个大小无限的线程池。此线程池支持定时以及周期性执行任务的需求。<br />在《阿里巴巴java开发手册》中指出了线程资源必须通过线程池提供，不允许在应用中自行显示的创建线程，这样一方面是线程的创建更加规范，可以合理控制开辟线程的数量；另一方面线程的细节管理交给线程池处理，优化了资源的开销。而线程池不允许使用`Executors`去创建，而要通过`ThreadPoolExecutor`方式，这一方面是由于jdk中`Executor`框架虽然提供了如`newFixedThreadPool()`、`newSingleThreadExecutor()`、`newCachedThreadPool()`等创建线程池的方法，但都有其局限性，不够灵活；另外由于前面几种方法内部也是通过`ThreadPoolExecutor`方式实现，使用`ThreadPoolExecutor`有助于大家明确线程池的运行规则，创建符合自己的业务场景需要的线程池，避免资源耗尽的风险。
<a name="oot2C"></a>
## `Future`、`Callable`接口
`Future`接口定义了操作异步异步任务执行一些方法，**如获取异步任务的执行结果、取消任务的执行、判断任务是否被取消、判断任务执行是否完毕**等。<br />`Callable`接口中定义了需要有返回的任务需要实现的方法。
```java
@FunctionalInterface
public interface Callable<V> {
    V call() throws Exception;
}
```
比如主线程让一个子线程去执行任务，子线程可能比较耗时，启动子线程开始执行任务后，主线程就去做其他事情了，过了一会才去获取子任务的执行结果。
<a name="pqW62"></a>
### 获取异步任务执行结果
**示例代码：**
```java
import java.util.concurrent.*;

public class Demo6 {
    public static void main(String[] args) throws ExecutionException, InterruptedException {
        ExecutorService executorService = Executors.newFixedThreadPool(1);
        Future<Integer> result = executorService.submit(() -> {
            System.out.println(System.currentTimeMillis() + "," + Thread.currentThread().getName()+",start!");
            TimeUnit.SECONDS.sleep(5);
            System.out.println(System.currentTimeMillis() + "," + Thread.currentThread().getName()+",end!");
            return 10;
        });
        System.out.println(System.currentTimeMillis() + "," + Thread.currentThread().getName());
        System.out.println(System.currentTimeMillis() + "," + Thread.currentThread().getName() + ",结果：" + result.get());
    }
}
```
输出：
```
1564581941442,main
1564581941442,pool-1-thread-1,start!
1564581946447,pool-1-thread-1,end!
1564581941442,main,结果：10
```
代码中创建了一个线程池，调用线程池的`submit`方法执行任务，`submit`参数为`Callable`接口：表示需要执行的任务有返回值，`submit`方法返回一个`Future`对象，`Future`相当于一个凭证，可以在任意时间拿着这个凭证去获取对应任务的执行结果（调用其get方法），代码中调用了`result.get()`方法之后，此方法会阻塞当前线程直到任务执行结束。
<a name="f0HW4"></a>
### 超时获取异步任务执行结果
可能任务执行比较耗时，比如耗时1分钟，最多只能等待10秒，如果10秒还没返回，就去做其他事情了。<br />刚好get有个超时的方法，声明如下：
```java
V get(long timeout, TimeUnit unit)
        throws InterruptedException, ExecutionException, TimeoutException;
```
**示例代码：**
```java
import java.util.concurrent.*;

public class Demo8 {
    public static void main(String[] args) throws ExecutionException, InterruptedException {
        ExecutorService executorService = Executors.newFixedThreadPool(1);
        Future<Integer> result = executorService.submit(() -> {
            System.out.println(System.currentTimeMillis() + "," + Thread.currentThread().getName()+",start!");
            TimeUnit.SECONDS.sleep(5);
            System.out.println(System.currentTimeMillis() + "," + Thread.currentThread().getName()+",end!");
            return 10;
        });
        System.out.println(System.currentTimeMillis() + "," + Thread.currentThread().getName());
        try {
            System.out.println(System.currentTimeMillis() + "," + Thread.currentThread().getName() + ",结果：" + result.get(3,TimeUnit.SECONDS));
        } catch (TimeoutException e) {
            e.printStackTrace();
        }
        executorService.shutdown();
    }
}
```
输出：
```
1564583177139,main
1564583177139,pool-1-thread-1,start!
java.util.concurrent.TimeoutException
    at java.util.concurrent.FutureTask.get(FutureTask.java:205)
    at com.example.chat18.Demo8.main(Demo8.java:19)
1564583182142,pool-1-thread-1,end!
```
任务执行中休眠了5秒，`get`方法获取执行结果，超时时间是3秒，3秒还未获取到结果，`get`触发了`TimeoutException`异常，当前线程从阻塞状态苏醒了。<br />`**Future**`**其他方法介绍一下**<br />`**cancel**`：取消在执行的任务，参数表示是否对执行的任务发送中断信号，方法声明如下：
```java
boolean cancel(boolean mayInterruptIfRunning);
```
`**isCancelled**`：用来判断任务是否被取消<br />`**isDone**`：判断任务是否执行完毕。<br />`**cancel**`**方法来个示例：**
```java
import java.util.concurrent.*;

public class Demo7 {
    public static void main(String[] args) throws ExecutionException, InterruptedException {
        ExecutorService executorService = Executors.newFixedThreadPool(1);
        Future<Integer> result = executorService.submit(() -> {
            System.out.println(System.currentTimeMillis() + "," + Thread.currentThread().getName()+",start!");
            TimeUnit.SECONDS.sleep(5);
            System.out.println(System.currentTimeMillis() + "," + Thread.currentThread().getName()+",end!");
            return 10;
        });

        executorService.shutdown();

        TimeUnit.SECONDS.sleep(1);
        result.cancel(false);
        System.out.println(result.isCancelled());
        System.out.println(result.isDone());

        TimeUnit.SECONDS.sleep(5);
        System.out.println(System.currentTimeMillis() + "," + Thread.currentThread().getName());
        System.out.println(System.currentTimeMillis() + "," + Thread.currentThread().getName() + ",结果：" + result.get());
        executorService.shutdown();
    }
}
```
输出：
```
1564583031646,pool-1-thread-1,start!
true
true
1564583036649,pool-1-thread-1,end!
1564583037653,main
Exception in thread "main" java.util.concurrent.CancellationException
    at java.util.concurrent.FutureTask.report(FutureTask.java:121)
    at java.util.concurrent.FutureTask.get(FutureTask.java:192)
    at com.example.chat18.Demo7.main(Demo7.java:24)
```
输出2个true，表示任务已被取消，已完成，最后调用`get`方法会触发`CancellationException`异常。<br />**总结：从上面可以看出**`**Future**`**、**`**Callable**`**接口需要结合**`**ExecutorService**`**来使用，需要有线程池的支持。**
<a name="bdYbA"></a>
## `FutureTask`类
`FutureTask`除了实现`Future`接口，还实现了`Runnable`接口，因此`FutureTask`可以交给`Executor`执行，也可以交给线程执行执行（**Thread有个**`**Runnable**`**的构造方法**），`**FutureTask**`表示带返回值结果的任务。<br />上面演示的是通过线程池执行任务然后获取执行结果。<br />这次通过`FutureTask`类，自己启动一个线程来获取执行结果，示例如下：
```java
import java.util.concurrent.*;

public class Demo9 {
    public static void main(String[] args) throws ExecutionException, InterruptedException {
        FutureTask<Integer> futureTask = new FutureTask<Integer>(()->{
            System.out.println(System.currentTimeMillis() + "," + Thread.currentThread().getName()+",start!");
            TimeUnit.SECONDS.sleep(5);
            System.out.println(System.currentTimeMillis() + "," + Thread.currentThread().getName()+",end!");
            return 10;
        });
        System.out.println(System.currentTimeMillis()+","+Thread.currentThread().getName());
        new Thread(futureTask).start();
        System.out.println(System.currentTimeMillis()+","+Thread.currentThread().getName());
        System.out.println(System.currentTimeMillis()+","+Thread.currentThread().getName()+",结果:"+futureTask.get());
    }
}
```
输出：
```
1564585122547,main
1564585122547,main
1564585122547,Thread-0,start!
1564585127549,Thread-0,end!
1564585122547,main,结果:10
```
**大家可以回过头去看一下上面用线程池的**`**submit**`**方法返回的**`**Future**`**实际类型正是**`**FutureTask**`**对象，有兴趣的可以设置个断点去看看。**<br />`**FutureTask**`**类还是相当重要的，标记一下。**
<a name="cQiK8"></a>
## 需要解决的问题
还是举个例子说明更好理解一些。<br />买新房了，然后在网上下单买冰箱、洗衣机，电器商家不同，所以送货耗时不一样，然后等他们送货，快递只愿送到楼下，然后自己将其搬到楼上的家中。<br />用程序来模拟上面的实现。示例代码如下：
```java
import java.util.concurrent.*;

public class Demo12 {
    static class GoodsModel {
        //商品名称
        String name;
        //购物开始时间
        long startime;
        //送到的时间
        long endtime;

        public GoodsModel(String name, long startime, long endtime) {
            this.name = name;
            this.startime = startime;
            this.endtime = endtime;
        }

        @Override
        public String toString() {
            return name + "，下单时间[" + this.startime + "," + endtime + "]，耗时:" + (this.endtime - this.startime);
        }
    }

    /**
     * 将商品搬上楼
     *
     * @param goodsModel
     * @throws InterruptedException
     */
    static void moveUp(GoodsModel goodsModel) throws InterruptedException {
        //休眠5秒，模拟搬上楼耗时
        TimeUnit.SECONDS.sleep(5);
        System.out.println("将商品搬上楼，商品信息:" + goodsModel);
    }

    /**
     * 模拟下单
     *
     * @param name     商品名称
     * @param costTime 耗时
     * @return
     */
    static Callable<GoodsModel> buyGoods(String name, long costTime) {
        return () -> {
            long startTime = System.currentTimeMillis();
            System.out.println(startTime + "购买" + name + "下单!");
            //模拟送货耗时
            try {
                TimeUnit.SECONDS.sleep(costTime);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            long endTime = System.currentTimeMillis();
            System.out.println(startTime + name + "送到了!");
            return new GoodsModel(name, startTime, endTime);
        };
    }

    public static void main(String[] args) throws InterruptedException, ExecutionException {
        long st = System.currentTimeMillis();
        System.out.println(st + "开始购物!");

        //创建一个线程池，用来异步下单
        ExecutorService executor = Executors.newFixedThreadPool(5);
        //异步下单购买冰箱
        Future<GoodsModel> bxFuture = executor.submit(buyGoods("冰箱", 5));
        //异步下单购买洗衣机
        Future<GoodsModel> xyjFuture = executor.submit(buyGoods("洗衣机", 2));
        //关闭线程池
        executor.shutdown();

        //等待冰箱送到
        GoodsModel bxGoodModel = bxFuture.get();
        //将冰箱搬上楼
        moveUp(bxGoodModel);
        //等待洗衣机送到
        GoodsModel xyjGooldModel = xyjFuture.get();
        //将洗衣机搬上楼
        moveUp(xyjGooldModel);
        long et = System.currentTimeMillis();
        System.out.println(et + "货物已送到家里咯，哈哈哈！");
        System.out.println("总耗时:" + (et - st));
    }
}
```
输出：
```
1564653121515开始购物!
1564653121588购买冰箱下单!
1564653121588购买洗衣机下单!
1564653121588洗衣机送到了!
1564653121588冰箱送到了!
将商品搬上楼，商品信息:冰箱，下单时间[1564653121588,1564653126590]，耗时:5002
将商品搬上楼，商品信息:洗衣机，下单时间[1564653121588,1564653123590]，耗时:2002
1564653136591货物已送到家里咯，哈哈哈！
总耗时:15076
```
从输出中可以看出几个时间：

1. 购买冰箱耗时5秒
2. 购买洗衣机耗时2秒
3. 将冰箱送上楼耗时5秒
4. 将洗衣机送上楼耗时5秒
5. 共计耗时15秒

购买洗衣机、冰箱都是异步执行的，先把冰箱送上楼了，然后再把冰箱送上楼了。上面大家应该发现了一个问题，洗衣机先到的，洗衣机到了，并没有去把洗衣机送上楼，而是在等待冰箱到货（`bxFuture.get();`），然后将冰箱送上楼，中间导致浪费了3秒，现实中应该是这样的，先到的先送上楼，修改一下代码，洗衣机先到的，先送洗衣机上楼，代码如下：
```java
import java.util.concurrent.*;

public class Demo13 {
    static class GoodsModel {
        //商品名称
        String name;
        //购物开始时间
        long startime;
        //送到的时间
        long endtime;

        public GoodsModel(String name, long startime, long endtime) {
            this.name = name;
            this.startime = startime;
            this.endtime = endtime;
        }

        @Override
        public String toString() {
            return name + "，下单时间[" + this.startime + "," + endtime + "]，耗时:" + (this.endtime - this.startime);
        }
    }

    /**
     * 将商品搬上楼
     *
     * @param goodsModel
     * @throws InterruptedException
     */
    static void moveUp(GoodsModel goodsModel) throws InterruptedException {
        //休眠5秒，模拟搬上楼耗时
        TimeUnit.SECONDS.sleep(5);
        System.out.println("将商品搬上楼，商品信息:" + goodsModel);
    }

    /**
     * 模拟下单
     *
     * @param name     商品名称
     * @param costTime 耗时
     * @return
     */
    static Callable<GoodsModel> buyGoods(String name, long costTime) {
        return () -> {
            long startTime = System.currentTimeMillis();
            System.out.println(startTime + "购买" + name + "下单!");
            //模拟送货耗时
            try {
                TimeUnit.SECONDS.sleep(costTime);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            long endTime = System.currentTimeMillis();
            System.out.println(endTime + name + "送到了!");
            return new GoodsModel(name, startTime, endTime);
        };
    }

    public static void main(String[] args) throws InterruptedException, ExecutionException {
        long st = System.currentTimeMillis();
        System.out.println(st + "开始购物!");

        //创建一个线程池，用来异步下单
        ExecutorService executor = Executors.newFixedThreadPool(5);
        //异步下单购买冰箱
        Future<GoodsModel> bxFuture = executor.submit(buyGoods("冰箱", 5));
        //异步下单购买洗衣机
        Future<GoodsModel> xyjFuture = executor.submit(buyGoods("洗衣机", 2));
        //关闭线程池
        executor.shutdown();

        //等待洗衣机送到
        GoodsModel xyjGooldModel = xyjFuture.get();
        //将洗衣机搬上楼
        moveUp(xyjGooldModel);
        //等待冰箱送到
        GoodsModel bxGoodModel = bxFuture.get();
        //将冰箱搬上楼
        moveUp(bxGoodModel);
        long et = System.currentTimeMillis();
        System.out.println(et + "货物已送到家里咯，哈哈哈！");
        System.out.println("总耗时:" + (et - st));
    }
}
```
输出：
```
1564653153393开始购物!
1564653153466购买洗衣机下单!
1564653153466购买冰箱下单!
1564653155467洗衣机送到了!
1564653158467冰箱送到了!
将商品搬上楼，商品信息:洗衣机，下单时间[1564653153466,1564653155467]，耗时:2001
将商品搬上楼，商品信息:冰箱，下单时间[1564653153466,1564653158467]，耗时:5001
1564653165469货物已送到家里咯，哈哈哈！
总耗时:12076
```
耗时12秒，比第一种少了3秒。<br />问题来了，上面是通过调整代码达到了最优效果，实际上，购买冰箱和洗衣机具体哪个耗时时间长是不知道的，怎么办呢，有没有什么解决办法？
<a name="lxiyv"></a>
## `CompletionService`接口
`CompletionService`相当于一个执行任务的服务，通过`submit`丢任务给这个服务，服务内部去执行任务，可以通过服务提供的一些方法获取服务中已经完成的任务。<br />**接口内的几个方法：**
```java
Future<V> submit(Callable<V> task);
```
用于向服务中提交有返回结果的任务，并返回`Future`对象
```java
Future<V> submit(Runnable task, V result);
```
用户向服务中提交有返回值的任务去执行，并返回`Future`对象
```java
Future<V> take() throws InterruptedException;
```
从服务中返回并移除一个已经完成的任务，如果获取不到，会一致阻塞到有返回值为止。此方法会响应线程中断。
```java
Future<V> poll();
```
从服务中返回并移除一个已经完成的任务，如果内部没有已经完成的任务，则返回空，此方法会立即响应。
```java
Future<V> poll(long timeout, TimeUnit unit) throws InterruptedException;
```
尝试在指定的时间内从服务中返回并移除一个已经完成的任务，等待的时间超时还是没有获取到已完成的任务，则返回空。此方法会响应线程中断<br />通过`submit`向内部提交任意多个任务，通过`take`方法可以获取已经执行完成的任务，如果获取不到将等待。
<a name="UoTjG"></a>
## `ExecutorCompletionService`类
`ExecutorCompletionService`类是`CompletionService`接口的具体实现。<br />说一下其内部原理，`ExecutorCompletionService`创建的时候会传入一个线程池，调用`submit`方法传入需要执行的任务，任务由内部的线程池来处理；`ExecutorCompletionService`内部有个阻塞队列，任意一个任务完成之后，会将任务的执行结果（`Future`类型）放入阻塞队列中，然后其他线程可以调用它`take`、`poll`方法从这个阻塞队列中获取一个已经完成的任务，获取任务返回结果的顺序和任务执行完成的先后顺序一致，所以最先完成的任务会先返回。<br />看一下构造方法：
```java
public ExecutorCompletionService(Executor executor) {
    if (executor == null)
        throw new NullPointerException();
    this.executor = executor;
    this.aes = (executor instanceof AbstractExecutorService) ?
        (AbstractExecutorService) executor : null;
    this.completionQueue = new LinkedBlockingQueue<Future<V>>();
}
```
构造方法需要传入一个`Executor`对象，这个对象表示任务执行器，所有传入的任务会被这个执行器执行。<br />`completionQueue`是用来存储任务结果的阻塞队列，默认用采用的是`LinkedBlockingQueue`，也支持开发自己设置。通过`submit`传入需要执行的任务，任务执行完成之后，会放入`completionQueue`中，有兴趣的可以看一下原码，还是很好理解的。
<a name="yiJXI"></a>
## 使用`ExecutorCompletionService`解决上面的问题
代码如下：
```java
import java.util.concurrent.*;

public class Demo14 {
    static class GoodsModel {
        //商品名称
        String name;
        //购物开始时间
        long startime;
        //送到的时间
        long endtime;

        public GoodsModel(String name, long startime, long endtime) {
            this.name = name;
            this.startime = startime;
            this.endtime = endtime;
        }

        @Override
        public String toString() {
            return name + "，下单时间[" + this.startime + "," + endtime + "]，耗时:" + (this.endtime - this.startime);
        }
    }

    /**
     * 将商品搬上楼
     *
     * @param goodsModel
     * @throws InterruptedException
     */
    static void moveUp(GoodsModel goodsModel) throws InterruptedException {
        //休眠5秒，模拟搬上楼耗时
        TimeUnit.SECONDS.sleep(5);
        System.out.println("将商品搬上楼，商品信息:" + goodsModel);
    }

    /**
     * 模拟下单
     *
     * @param name     商品名称
     * @param costTime 耗时
     * @return
     */
    static Callable<GoodsModel> buyGoods(String name, long costTime) {
        return () -> {
            long startTime = System.currentTimeMillis();
            System.out.println(startTime + "购买" + name + "下单!");
            //模拟送货耗时
            try {
                TimeUnit.SECONDS.sleep(costTime);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            long endTime = System.currentTimeMillis();
            System.out.println(endTime + name + "送到了!");
            return new GoodsModel(name, startTime, endTime);
        };
    }

    public static void main(String[] args) throws InterruptedException, ExecutionException {
        long st = System.currentTimeMillis();
        System.out.println(st + "开始购物!");
        ExecutorService executor = Executors.newFixedThreadPool(5);

        //创建ExecutorCompletionService对象
        ExecutorCompletionService<GoodsModel> executorCompletionService = new ExecutorCompletionService<>(executor);
        //异步下单购买冰箱
        executorCompletionService.submit(buyGoods("冰箱", 5));
        //异步下单购买洗衣机
        executorCompletionService.submit(buyGoods("洗衣机", 2));
        executor.shutdown();

        //购买商品的数量
        int goodsCount = 2;
        for (int i = 0; i < goodsCount; i++) {
            //可以获取到最先到的商品
            GoodsModel goodsModel = executorCompletionService.take().get();
            //将最先到的商品送上楼
            moveUp(goodsModel);
        }

        long et = System.currentTimeMillis();
        System.out.println(et + "货物已送到家里咯，哈哈哈！");
        System.out.println("总耗时:" + (et - st));
    }
}
```
输出：
```
1564653208284开始购物!
1564653208349购买冰箱下单!
1564653208349购买洗衣机下单!
1564653210349洗衣机送到了!
1564653213350冰箱送到了!
将商品搬上楼，商品信息:洗衣机，下单时间[1564653208349,1564653210349]，耗时:2000
将商品搬上楼，商品信息:冰箱，下单时间[1564653208349,1564653213350]，耗时:5001
1564653220350货物已送到家里咯，哈哈哈！
总耗时:12066
```
从输出中可以看出和期望的结果一致，代码中下单顺序是：冰箱、洗衣机，冰箱送货耗时5秒，洗衣机送货耗时2秒，洗衣机先到的，然后被送上楼了，冰箱后到被送上楼，总共耗时12秒，和期望的方案一样。
<a name="y4nzw"></a>
## 示例：执行一批任务，然后消费执行结果
代码如下：
```java
import java.util.ArrayList;
import java.util.Collection;
import java.util.List;
import java.util.concurrent.*;
import java.util.function.Consumer;

public class Demo15 {
    public static void main(String[] args) throws ExecutionException, InterruptedException {
        ExecutorService executorService = Executors.newFixedThreadPool(5);
        List<Callable<Integer>> list = new ArrayList<>();
        int taskCount = 5;
        for (int i = taskCount; i > 0; i--) {
            int j = i * 2;
            list.add(() -> {
                TimeUnit.SECONDS.sleep(j);
                return j;
            });
        }
        solve(executorService, list, a -> {
            System.out.println(System.currentTimeMillis() + ":" + a);
        });
        executorService.shutdown();
    }


    public static <T> void solve(Executor e, Collection<Callable<T>> solvers, Consumer<T> use) throws InterruptedException, ExecutionException {
        CompletionService<T> ecs = new ExecutorCompletionService<T>(e);
        for (Callable<T> s : solvers) {
            ecs.submit(s);
        }
        int n = solvers.size();
        for (int i = 0; i < n; ++i) {
            T r = ecs.take().get();
            if (r != null) {
                use.accept(r);
            }
        }
    }
}
```
输出：
```
1564667625648:2
1564667627652:4
1564667629649:6
1564667631652:8
1564667633651:10
```
代码中传入了一批任务进行处理，最终将所有处理完成的按任务完成的先后顺序传递给Consumer进行消费了。
<a name="xUu0X"></a>
## 示例：异步执行一批任务，有一个完成立即返回，其他取消
这个给大家讲解2种方式。
<a name="ohpDg"></a>
### 方式1
使用`ExecutorCompletionService`实现，`ExecutorCompletionService`提供了获取一批任务中最先完成的任务结果的能力。<br />**代码如下：**
```java
import java.util.ArrayList;
import java.util.Collection;
import java.util.List;
import java.util.concurrent.*;
import java.util.function.Consumer;

public class Demo16 {
    public static void main(String[] args) throws ExecutionException, InterruptedException {
        long startime = System.currentTimeMillis();
        ExecutorService executorService = Executors.newFixedThreadPool(5);
        List<Callable<Integer>> list = new ArrayList<>();
        int taskCount = 5;
        for (int i = taskCount; i > 0; i--) {
            int j = i * 2;
            String taskName = "任务"+i;
            list.add(() -> {
                TimeUnit.SECONDS.sleep(j);
                System.out.println(taskName+"执行完毕!");
                return j;
            });
        }
        Integer integer = invokeAny(executorService, list);
        System.out.println("耗时:" + (System.currentTimeMillis() - startime) + ",执行结果:" + integer);
        executorService.shutdown();
    }


    public static <T> T invokeAny(Executor e, Collection<Callable<T>> solvers) throws InterruptedException, ExecutionException {
        CompletionService<T> ecs = new ExecutorCompletionService<T>(e);
        List<Future<T>> futureList = new ArrayList<>();
        for (Callable<T> s : solvers) {
            futureList.add(ecs.submit(s));
        }
        int n = solvers.size();
        try {
            for (int i = 0; i < n; ++i) {
                T r = ecs.take().get();
                if (r != null) {
                    return r;
                }
            }
        } finally {
            for (Future<T> future : futureList) {
                future.cancel(true);
            }
        }
        return null;
    }
}
```
程序输出下面结果然后停止了：
```
任务1执行完毕!
耗时:2072,执行结果:2
```
代码中执行了5个任务，使用`CompletionService`执行任务，调用take方法获取最先执行完成的任务，然后返回。在`finally`中对所有任务发送取消操作（`future.cancel(true);`），从输出中可以看出只有任务1执行成功，其他任务被成功取消了，符合预期结果。
<a name="NUnWp"></a>
### 方式2
其实`ExecutorService`已经提供了这样的方法，方法声明如下：
```java
<T> T invokeAny(Collection<? extends Callable<T>> tasks)
        throws InterruptedException, ExecutionException;
```
**示例代码：**
```java
import java.util.ArrayList;
import java.util.Collection;
import java.util.List;
import java.util.concurrent.*;

public class Demo17 {
    public static void main(String[] args) throws ExecutionException, InterruptedException {
        long startime = System.currentTimeMillis();
        ExecutorService executorService = Executors.newFixedThreadPool(5);

        List<Callable<Integer>> list = new ArrayList<>();
        int taskCount = 5;
        for (int i = taskCount; i > 0; i--) {
            int j = i * 2;
            String taskName = "任务" + i;
            list.add(() -> {
                TimeUnit.SECONDS.sleep(j);
                System.out.println(taskName + "执行完毕!");
                return j;
            });
        }
        Integer integer = executorService.invokeAny(list);
        System.out.println("耗时:" + (System.currentTimeMillis() - startime) + ",执行结果:" + integer);
        executorService.shutdown();
    }
}
```
输出下面结果之后停止：
```
任务1执行完毕!
耗时:2061,执行结果:2
```
输出结果和方式1中结果类似。
