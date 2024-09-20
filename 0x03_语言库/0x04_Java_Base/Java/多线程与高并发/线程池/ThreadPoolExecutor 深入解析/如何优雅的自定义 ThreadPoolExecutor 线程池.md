Java线程池
<a name="z82P5"></a>
## 1、概述
java 中经常需要用到多线程来处理一些业务，非常不建议单纯使用继承Thread或者实现Runnable接口的方式来创建线程，那样势必有创建及销毁线程耗费资源、线程上下文切换问题。同时创建过多的线程也可能引发资源耗尽的风险，这个时候引入线程池比较合理，方便线程任务的管理。<br />java中涉及到线程池的相关类均在 jdk 1.5 开始的java.util.concurrent包中，涉及到的几个核心类及接口包括：`Executor`、`Executors`、`ExecutorService`、`ThreadPoolExecutor`、`FutureTask`、`Callable`、`Runnable`等。<br />JDK 自动创建线程池的几种方式都封装在`Executors`工具类中：
<a name="rGOFT"></a>
### `newFixedThreadPool`
使用的构造方式为
```java
new ThreadPoolExecutor(var0, var0, 0L, TimeUnit.MILLISECONDS, new LinkedBlockingQueue())
```
设置了`corePoolSize=maxPoolSize，keepAliveTime=0`(此时该参数没作用)，无界队列，任务可以无限放入，当请求过多时(任务处理速度跟不上任务提交速度造成请求堆积)可能导致占用过多内存或直接导致OOM异常。
<a name="Lo0Aa"></a>
### `newSingleThreadExector`
使用的构造方式为
```java
new ThreadPoolExecutor(1, 1, 0L, TimeUnit.MILLISECONDS, new LinkedBlockingQueue(), var0)
```
基本同 `newFixedThreadPool`，但是将线程数设置为了1，单线程，弊端和`newFixedThreadPool`一致。
<a name="c78yE"></a>
### `newCachedThreadPool`
使用的构造方式为
```java
new ThreadPoolExecutor(0, 2147483647, 60L, TimeUnit.SECONDS, new SynchronousQueue())
```
`corePoolSize=0`，`maxPoolSize`为很大的数，同步移交队列，也就是说不维护常驻线程(核心线程)，每次来请求直接创建新线程来处理任务，也不使用队列缓冲，会自动回收多余线程，由于将`maxPoolSize`设置成`Integer.MAX_VALUE`，当请求很多时就可能创建过多的线程，导致资源耗尽OOM。
<a name="CGlO4"></a>
### `newScheduledThreadPool`
使用的构造方式为
```java
new ThreadPoolExecutor(var1, 2147483647, 0L, TimeUnit.NANOSECONDS, new ScheduledThreadPoolExecutor.DelayedWorkQueue())
```
支持定时周期性执行，注意一下使用的是延迟队列，弊端同`newCachedThreadPool`一致。<br />那么上面说了使用Executors工具类创建的线程池有隐患，那如何使用才能避免这个隐患呢？如何才是最优雅的方式去使用过线程池吗？<br />生产环境要怎么去配置自己的线程池才是合理的呢？需要对症下药，建立自己的线程工厂类，灵活设置关键参数。
<a name="ClxY9"></a>
## 2、`ThreadPoolExecutor` 类
要自定义线程池，需要使用`ThreadPoolExecutor`类。<br />`ThreadPoolExecutor`类的构造方法：
```java
public ThreadPoolExecutor(int coreSize,int maxSize,long KeepAliveTime,TimeUnit unit,BlockingQueue queue,ThreadFactory factory,RejectedExectionHandler handler)
```
上述构造方法共有七个参数，这七个参数的含义分别是：

- `**corePoolSize**`**： 核心线程数，也是线程池中常驻的线程数，线程池初始化时默认是没有线程的，当任务来临时才开始创建线程去执行任务**
- `**maximumPoolSize**`**： 最大线程数，在核心线程数的基础上可能会额外增加一些非核心线程，需要注意的是只有当**`**workQueue**`**队列填满时才会创建多于**`**corePoolSize**`**的线程(线程池总线程数不超过**`**maxPoolSize**`**)**
- `**keepAliveTime**`**： 非核心线程的空闲时间超过**`**keepAliveTime**`**就会被自动终止回收掉，注意当**`**corePoolSize=maxPoolSize**`**时，**`**keepAliveTime**`**参数也就不起作用了(因为不存在非核心线程)；**
- `**unit**`**： **`**keepAliveTime**`**的时间单位**
- `**workQueue**`**： 用于保存任务的队列，可以为无界、有界、同步移交三种队列类型之一，当池子里的工作线程数大于**`**corePoolSize**`**时，这时新进来的任务会被放到队列中**
- `**threadFactory**`**： 创建线程的工厂类，默认使用**`**Executors.defaultThreadFactory()**`**，也可以使用guava库的**`**ThreadFactoryBuilder**`**来创建**
- `**handler**`**： 线程池无法继续接收任务(队列已满且线程数达到**`**maximunPoolSize**`**)时的饱和策略，取值有**`**AbortPolicy**`**、**`**CallerRunsPolicy**`**、**`**DiscardOldestPolicy**`**、**`**DiscardPolicy**`
<a name="C5vtd"></a>
## 3、线程池配置相关
<a name="aCQaM"></a>
### 3.1 线程池大小的设置
首先针对于这个问题，必须要明确需求是计算密集型还是IO密集型，只有了解了这一点，才能更好的去设置线程池的数量进行限制。
<a name="gZkGw"></a>
#### 计算密集型
顾名思义就是应用需要非常多的CPU计算资源，在多核CPU时代，要让每一个CPU核心都参与计算，将CPU的性能充分利用起来，这样才算是没有浪费服务器配置，如果在非常好的服务器配置上还运行着单线程程序那将是多么重大的浪费。对于计算密集型的应用，完全是靠CPU的核数来工作，所以为了让它的优势完全发挥出来，避免过多的线程上下文切换，比较理想方案是：<br />线程数 = CPU核数+1，也可以设置成CPU核数*2，但还要看JDK的版本以及CPU配置(服务器的CPU有超线程)。<br />一般设置CPU * 2即可。
<a name="AXjjn"></a>
#### IO密集型
现在做的开发大部分都是WEB应用，涉及到大量的网络传输，不仅如此，与数据库，与缓存间的交互也涉及到IO，一旦发生IO，线程就会处于等待状态，当IO结束，数据准备好后，线程才会继续执行。<br />因此从这里可以发现，对于IO密集型的应用，可以多设置一些线程池中线程的数量，这样就能让在等待IO的这段时间内，线程可以去做其它事，提高并发处理效率。那么这个线程池的数据量是不是可以随便设置呢？当然不是的，请一定要记得，线程上下文切换是有代价的。目前总结了一套公式，对于IO密集型应用：<br />线程数 = CPU核心数/(1-阻塞系数) 这个阻塞系数一般为0.8~0.9之间，也可以取0.8或者0.9。<br />套用公式，对于双核CPU来说，它比较理想的线程数就是20，当然这都不是绝对的，需要根据实际情况以及实际业务来调整：`final int poolSize = (int)(cpuCore/(1-0.9))`<br />针对于阻塞系数，《Programming Concurrency on the JVM Mastering》即《Java 虚拟机并发编程》中有提到一句话：<br />对于阻塞系数，可以先试着猜测，抑或采用一些细嫩分析工具或java.lang.management API 来确定线程花在系统/IO操作上的时间与CPU密集任务所耗的时间比值。
<a name="NcKpt"></a>
### 3.2 线程池相关参数配置
<a name="XOj1g"></a>
#### 一定不要选择没有上限限制的配置项。
这也是为什么不建议使用 Executors 中创建线程的方法。<br />例如，`Executors.newCachedThreadPool` 的设置与无界队列的设置因为某些不可预期的情况，线程池会出现系统异常，导致线程暴增的情况或者任务队列不断膨胀，内存耗尽导致系统崩溃和异常。<br />推荐使用自定义线程池来避免该问题，这也是在使用线程池规范的首要原则！
<a name="Clzsw"></a>
#### 第二，合理设置线程数量、和线程空闲回收时间，
根据具体的任务执行周期和时间去设定，避免频繁的回收和创建，虽然使用线程池的目的是为了提升系统性能和吞吐量，但是也要考虑下系统的稳定性，不然出现不可预期问题会很麻烦！
<a name="GRTop"></a>
#### 第三，根据实际场景，选择适用于自己的拒绝策略。
进行补偿，不要乱用JDK支持的自动补偿机制！尽量采用自定义的拒绝策略去进行兜底！
<a name="h4kem"></a>
#### 第四，线程池拒绝策略，自定义拒绝策略可以实现RejectedExecutionHandler接口。
JDK自带的拒绝策略如下：

- `**AbortPolicy**`**：直接抛出异常阻止系统正常工作。**
- `**CallerRunsPolicy**`**：只要线程池未关闭，该策略直接在调用者线程中，运行当前被丢弃的任务。**
- `**DiscardOldestPolicy**`**：丢弃最老的一个请求，尝试再次提交当前任务。**
- `**DiscardPolicy**`**：丢弃无法处理的任务，不给予任何处理。**
<a name="pEO7f"></a>
## 4、利用Hook
利用Hook，留下线程池执行轨迹：<br />`ThreadPoolExecutor`提供了`protected`类型可以被覆盖的钩子方法，允许用户在任务执行之前会执行之后做一些事情。可以通过它来实现比如初始化`ThreadLocal`、收集统计信息、如记录日志等操作。这类Hook如`beforeExecute`和`afterExecute`。另外还有一个Hook可以用来在任务被执行完的时候让用户插入逻辑，如`rerminated` 。<br />如果hook方法执行失败，则内部的工作线程的执行将会失败或被中断。<br />可以使用`beforeExecute`和`afterExecute`来记录线程之前前和后的一些运行情况，也可以直接把运行完成后的状态记录到ELK等日志系统。
<a name="xJE8J"></a>
## 5、关闭线程池
当线程池不再被引用并且工作线程数为0的时候，线程池将被终止。也可以调用`shutdown`来手动终止线程池。如果忘记调用`shutdown`，为了让线程资源被释放，还可以使用`keepAliveTime` 和 `allowCoreThreadTimeOut`来达到目的!<br />当然，稳妥的方式是使用虚拟机`Runtime.getRuntime().addShutdownHook`方法，手工去调用线程池的关闭方法。
<a name="DkLFs"></a>
## 6、可优化事项
<a name="l8gqQ"></a>
### 6.1 设置线程池中线程为Daemon
一般情况下，关闭线程池后，线程池会自行将其中的线程结束掉。但针对一些自己伪装或直接new Thread()的这种线程，则仍会阻塞进程关闭。<br />按照，java进程关闭判定方法，当只存在Daemon线程时，进程才会正常关闭。因此，这里建议这些非主要线程均设置为 daemon，即不会阻塞进程关闭。
<a name="q7Ul9"></a>
### 6.2 正确命名Thread
在使用线程池时，一般会接受 `ThreadFactory` 对象，来控制如何创建`thread`。在java自带的`ExecutorService`时，如果没有设置此参数，则会使用默认的 `DefaultThreadFactory`。效果就是，会在线程栈列表中，看到一堆的 pool-x-thread-y，在实际使用 jstack时，根本看不清这些线程每个所属的组，以及具体作用。
<a name="J80uX"></a>
### 6.3 丢弃不再可用周期性任务
一般情况下，使用 java 自带的 `ScheduledThreadPoolExecutor`，调用 `scheduleAtFixedRate` 及 `scheduleWithFixedDelay` 均会将任务设置为周期性的(`period`)。在线程池关闭时，这些任务均可以直接被丢弃掉(默认情况下). 但如果使用 schedule 添加远期的任务时，线程池则会因为其不是周期性任务而不会关闭所对应的线程<br />如 spring 体系中 `TriggerTask`(包括CronTask)，来进行定时调度的任务，其最终均是通过 schedule 来实现调度，并在单个任务完成之后，再次 schedule 下一次任务的方式来执行。这种方式会被认为并不是 period. 因此，使用此调度方式时，尽管容器关闭时，执行了 shutdown 方法，但相应底层的 `ScheduledExecutorService` 仍然不会成功关闭掉(尽管所有的状态均已经设置完)。最终效果就是，会看到一个已经处于shutdown状态的线程池，但线程仍然在运行(状态为 wait 任务)的情况.<br />为解决此方法，java 提供一个额外的设置参数 `executeExistingDelayedTasksAfterShutdown`，此值默认为true，即 shutdown 之后，仍然执行。可以通过在定义线程池时将其设置为 false，即线程池关闭之后，不再运行这些延时任务。
