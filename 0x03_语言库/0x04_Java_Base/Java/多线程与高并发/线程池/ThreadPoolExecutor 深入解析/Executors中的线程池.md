Java Thread
<a name="NC1XM"></a>
## 一、前言
使用线程可以提升系统的性能。但是，创建线程和销毁线程都是比较耗时的操作，频繁的创建和销毁线程会浪费很多CPU的资源。此外，如果每个任务都创建一个线程去处理，这样线程会越来越多。每个线程默认情况下占1M的内存空间，如果线程非常多，内存资源将会被耗尽。这时，需要线程池去管理线程，不会出现内存资源被耗尽的情况，也不会出现频繁创建和销毁线程的情况，因为它内部是可以复用线程的。
<a name="ZA4eB"></a>
## 二、创建线程池的案例
在介绍线程池之前，先看个例子。
```java
public class MyCallable implements Callable<String> {

    @Override
    public String call() throws Exception {
          System.out.println("MyCallable call");
          return "success";
    }

    public static void main(String[] args) {

        ExecutorService threadPool = Executors.newSingleThreadExecutor();
        try {

            Future<String> future = threadPool.submit(new MyCallable());
            System.out.println(future.get());
        } catch (Exception e) {
           System.out.println(e);
        } finally {
            threadPool.shutdown();
        }
    }
}
```
这个类的功能就是使用`Executors`类的`newSingleThreadExecutor`方法创建了的一个单线程池，他里面会执行`Callable`线程任务。
<a name="w16aU"></a>
## 三、创建线程池的方法
`Executors`类里面封装了不少创建线程池的静态方法，如下图所示：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1613784665407-6af30c40-ff78-4118-ac83-ce0def812e28.png#averageHue=%23fdfde1&height=536&id=p9CNI&originHeight=1608&originWidth=1627&originalType=binary&ratio=1&rotation=0&showTitle=false&size=287877&status=done&style=none&title=&width=542.3333333333334)<br />总结一下其实只有6种：
<a name="maj6J"></a>
### 1、`newCachedThreadPool`可缓冲线程池
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1613784692308-8e64e3ad-eae6-404d-9531-2094be7281a3.png#averageHue=%23f9f8f8&height=276&id=iBPjM&originHeight=828&originWidth=2291&originalType=binary&ratio=1&rotation=0&showTitle=false&size=219034&status=done&style=shadow&title=&width=763.6666666666666)<br />它的核心线程数是0，最大线程数是integer的最大值，每隔60秒回收一次空闲线程，使用`SynchronousQueue`队列。`SynchronousQueue`队列比较特殊，内部只包含一个元素，插入元素到队列的线程被阻塞，直到另一个线程从队列中获取了队列中存储的元素。同样，如果线程尝试获取元素并且当前不存在任何元素，则该线程将被阻塞，直到线程将元素插入队列。
<a name="Vv0CB"></a>
### 2、`newFixedThreadPool`固定大小线程池
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1613784732891-414f08d2-7184-486d-8ff4-858d61f47404.png#averageHue=%23fafaf9&height=275&id=E6x60&originHeight=825&originWidth=2410&originalType=binary&ratio=1&rotation=0&showTitle=false&size=190713&status=done&style=shadow&title=&width=803.3333333333334)<br />它的核心线程数 和 最大线程数是一样，都是`nThreads`变量的值，该变量由用户自己决定，所以说是固定大小线程池。此外，它的每隔0毫秒回收一次线程，换句话说就是不回收线程，因为它的核心线程数 和 最大线程数是一样，回收了没有任何意义。此外，使用了`LinkedBlockingQueue`队列，该队列其实是有界队列，很多人误解了，只是它的初始大小比较大是`integer`的最大值。
<a name="q777j"></a>
### 3、`newScheduledThreadPool`定时任务线程池
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1613784791643-596cd1ae-f08d-4102-b428-c820fb3293c3.png#averageHue=%23fbfafa&height=160&id=mQNmw&originHeight=479&originWidth=2349&originalType=binary&ratio=1&rotation=0&showTitle=false&size=117203&status=done&style=shadow&title=&width=783)<br />它的核心线程数是`corePoolSize`变量，需要用户自己决定，最大线程数是integer的最大值，同样，它的每隔0毫秒回收一次线程，换句话说就是不回收线程。使用了`DelayedWorkQueue`队列，该队列具有延时的功能。
<a name="25f1e0b9"></a>
### 4、`newSingleThreadExecutor`单个线程池
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1613784820729-985f8722-9e40-4a1c-a7fa-6e99ecd05105.png#averageHue=%23f9f8f8&height=265&id=RiV1t&originHeight=794&originWidth=2338&originalType=binary&ratio=1&rotation=0&showTitle=false&size=210089&status=done&style=shadow&title=&width=779.3333333333334)<br />其实，跟上面的`newFixedThreadPool`是一样的，稍微有一点区别是核心线程数 和 最大线程数 都是1，这就是为什么说它是单线程池的原因。
<a name="leu89"></a>
### 5、`newSingleThreadScheduledExecutor`单线程定时任务线程池
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1613784850329-8da6fb4c-8039-4dac-992c-669d32029b1d.png#averageHue=%23f9f8f8&height=221&id=PdfEn&originHeight=662&originWidth=2478&originalType=binary&ratio=1&rotation=0&showTitle=false&size=167253&status=done&style=shadow&title=&width=826)<br />该线程池是对上面介绍过的`ScheduledThreadPoolExecutor`定时任务线程池的简单封装，核心线程数固定是1，其他的功能一模一样。
<a name="0B0Bw"></a>
### 6、`newWorkStealingPool`窃取线程池
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1613784884395-f715f456-0256-42d6-bc71-b6b442ec8f21.png#averageHue=%23fbfbfa&height=217&id=X9ygj&originHeight=651&originWidth=1983&originalType=binary&ratio=1&rotation=0&showTitle=false&size=133863&status=done&style=shadow&title=&width=661)<br />它是JDK1.8增加的新线程池，跟其他的实现方式都不一样，它底层是通过`ForkJoinPool`类来实现的。会创建一个含有足够多线程的线程池，来维持相应的并行级别，它会通过工作窃取的方式，使得多核的 CPU 不会闲置，总会有活着的线程让 CPU 去运行。<br />其实`newFixedThreadPool`、`newCachedThreadPool`、`newSingleThreadExecutor `和 `newWorkStealingPool`方法创建和使用线程池的方法是一样的。这四个方法创建线程池返回值是`ExecutorService`，通过它的`execute`方法执行线程。
```java
public class MyWorker implements Runnable {
    @Override
    public void run() {
        System.out.println("MyWorker run");
    }
    public static void main(String[] args) {
        ExecutorService threadPool = Executors.newFixedThreadPool(8);
        try {
            threadPool.execute(new MyWorker());
        } catch (Exception e) {
            System.out.println(e);
        } finally {
            threadPool.shutdown();
        }
    }
}
```
`newScheduledThreadPool `和 `newSingleThreadScheduledExecutor`方法创建和使用线程池的方法也是一样的
```java
public class MyTask implements Runnable {

    @Override
    public void run() {
        System.out.println("MyTask call");
    }

    public static void main(String[] args) {

        ScheduledExecutorService scheduledExecutorService = Executors.newScheduledThreadPool(8);
        try {
            scheduledExecutorService.schedule(new MyRunnable(), 60, TimeUnit.SECONDS);
        } finally {
            scheduledExecutorService.shutdown();
       }
    }
}
```
以上两个方法创建的线程池返回值是`ScheduledExecutorService`，通过它的`schedule`提交线程，并且可以配置延迟执行的时间。
<a name="OSa91"></a>
## 四、自定义线程池
`Executors`类有这么多方法可以创建线程池，但是阿里巴巴开发规范中却明确规定不要使用Executors类创建线程池，这是为什么呢？<br />`newCachedThreadPool`可缓冲线程池，它的最大线程数是integer的最大值，意味着使用它创建的线程池，可以创建非常多的线程，一个线程默认情况下占用内存1M，如果创建的线程太多，占用内存太大，最后肯定会出现内存溢出的问题。<br />`newFixedThreadPool`和`newSingleThreadExecutor`在这里都称为固定大小线程池，它的队列使用的`LinkedBlockingQueue`，这个队列默认大小是integer的最大值，意味着可以往该队列中加非常多的任务，每个任务也是要内存空间的，如果任务太多，最后肯定也会出现内存溢出的问题。<br />阿里建议使用`ThreadPoolExecutor`类创建线程池，其实从刚刚看到的`Executors`类创建线程池的`newFixedThreadPool`等方法可以看出，它也是使用`ThreadPoolExecutor`类创建线程池的。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1613784944269-d3c3d9c2-80b3-45ed-9661-a4478da42cd8.png#averageHue=%23fefee3&height=166&id=uaOjM&originHeight=498&originWidth=2105&originalType=binary&ratio=1&rotation=0&showTitle=false&size=101082&status=done&style=none&title=&width=701.6666666666666)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1613785016890-3896ffa7-2ba7-4c8c-b7dd-43135ee33eb1.png#averageHue=%23fbfafa&height=431&id=dGoFD&originHeight=1294&originWidth=2337&originalType=binary&ratio=1&rotation=0&showTitle=false&size=343606&status=done&style=shadow&title=&width=779)<br />从上图可以看出`ThreadPoolExecutor`类的构造方法有4个，里面包含了很多参数，先认识一下：

- `corePoolSize`：核心线程数
- `maximumPoolSize`：最大线程数
- `keepAliveTime`：空闲线程回收时间间隔
- `unit`：空闲线程回收时间间隔单位
- `workQueue`：提交任务的队列，当线程数量超过核心线程数时，可以将任务提交到任务队列中。比较常用的有：`ArrayBlockingQueue`; `LinkedBlockingQueue`; `SynchronousQueue`;
- `threadFactory`：线程工厂，可以自定义线程的一些属性，比如：名称或者守护线程等
- `handler`：表示当拒绝处理任务时的策略
```
ThreadPoolExecutor.AbortPolicy:丢弃任务并抛出RejectedExecutionException异常。
ThreadPoolExecutor.DiscardPolicy：也是丢弃任务，但是不抛出异常。
ThreadPoolExecutor.DiscardOldestPolicy：丢弃队列最前面的任务，然后重新尝试执行任务（重复此过程）
ThreadPoolExecutor.CallerRunsPolicy：由调用线程处理该任务
```
根据上面的内容自定义一个线程池：
```java
public class MyThreadPool implements  Runnable {

    private static final ExecutorService  executorService = new ThreadPoolExecutor(
            8,
            10,
            30,
            TimeUnit.SECONDS,
            new ArrayBlockingQueue<>(500),
            new ThreadPoolExecutor.AbortPolicy());

    @Override
    public void run() {
        System.out.println("MyThreadPool run");
    }

    public static void main(String[] args) {
        int availableProcessors = Runtime.getRuntime().availableProcessors();
        try {
            executorService.execute(new MyThreadPool());
        } catch (Exception e) {
            System.out.println(e);
        } finally {
            executorService.shutdown();
        }
    }
}
```
从上面可以看到，使用`ThreadPoolExecutor`类自定义了一个线程池，它的核心线程数是8，最大线程数是 10，空闲线程回收时间是30，单位是秒，存放任务的队列用的`ArrayBlockingQueue`，而队列满的处理策略用的`AbortPolicy`。使用这个队列，基本可以保持线程在系统的可控范围之内，不会出现内存溢出的问题。但是也不是绝对的，只是出现内存溢出的概率比较小。<br />当然，阿里巴巴开发规范建议不使用Executors类创建线程池，并不表示它完全没用，在一些低并发的业务场景照样可以使用。
<a name="ljTKZ"></a>
## 五、最佳线程数
在使用线程池时，如何配置线程数量？
<a name="rTTT6"></a>
### 1、经验值
配置线程数量之前，首先要看任务的类型是 IO密集型，还是CPU密集型？
> 什么是IO密集型？
> 比如：频繁读取磁盘上的数据，或者需要通过网络远程调用接口。
> 什么是CPU密集型？
> 比如：非常复杂的调用，循环次数很多，或者递归调用层次很深等。

IO密集型配置线程数经验值是：2N，其中N代表CPU核数。<br />CPU密集型配置线程数经验值是：N + 1，其中N代表CPU核数。<br />获取N值
```java
int availableProcessors = Runtime.getRuntime().availableProcessors();
```
那么问题来了，混合型（既包含IO密集型，又包含CPU密集型）的如何配置线程数？<br />混合型如果IO密集型，和CPU密集型的执行时间相差不太大，可以拆分开，以便于更好配置。如果执行时间相差太大，优化的意义不大，比如IO密集型耗时60s，CPU密集型耗时1s。
<a name="ETPqZ"></a>
### 2、最佳线程数目算法
除了上面介绍是经验值之外，其实还提供了计算公式：
```
最佳线程数目 = （（线程等待时间+线程CPU时间）/线程CPU时间 ）* CPU数目
```
很显然线程等待时间所占比例越高，需要越多线程。线程CPU时间所占比例越高，需要越少线程。<br />虽说最佳线程数目算法更准确，但是线程等待时间和线程CPU时间不好测量，实际情况使用得比较少，一般用经验值就差不多了。再配合系统压测，基本可以确定最适合的线程数。
