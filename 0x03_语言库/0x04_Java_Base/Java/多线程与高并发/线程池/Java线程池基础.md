Java 线程池
<a name="b0lbc"></a>
## Java线程池概念
顾名思义，管理线程的池子，相比于手工创建、运行线程，使用线程池，有如下优点

- 降低线程创建和销毁线程造成的开销
- 提高响应速度。任务到达时，相对于手工创建一个线程，直接从线程池中拿线程，速度肯定快很多
- 提高线程可管理性。线程是稀缺资源，如果无限制地创建，不仅会消耗系统资源，还会降低系统稳定性，使用线程池可以进行统一分配、调优和监控
<a name="XGo9C"></a>
## Java线程池创建
无论是创建何种类型线程池(`FixedThreadPool`、`CachedThreadPool`…)，均会调用`ThreadPoolExecutor`构造函数，下面详细解读各个参数的作用
```java
public ThreadPoolExecutor(int corePoolSize,  
						  int maximumPoolSize,  
						  long keepAliveTime,  
						  TimeUnit unit,  
						  BlockingQueue<Runnable> workQueue) {  
	this(corePoolSize, maximumPoolSize, keepAliveTime, unit, workQueue,  
		 Executors.defaultThreadFactory(), defaultHandler);  
}
```

- `corePoolSize`：核心线程最大数量，通俗点来讲就是，线程池中常驻线程的最大数量
- `maximumPoolSize`：线程池中运行最大线程数(包括核心线程和非核心线程)
- `keepAliveTime`：线程池中空闲线程（仅适用于非核心线程）所能存活的最长时间
- `unit`：存活时间单位，与keepAliveTime搭配使用
- `workQueue`：存放任务的阻塞队列
- `handler`：线程池饱和策略
<a name="wv7VZ"></a>
## 线程池执行流程
当提交一个新任务，线程池的处理流程如下：

- 判断线程池中核心线程数是否已达阈值`corePoolSize`，若否，则创建一个新核心线程执行任务
- 若核心线程数已达阈值`corePoolSize`，判断阻塞队列`workQueue`是否已满，若未满，则将新任务添加进阻塞队列
- 若满，再判断，线程池中线程数是否达到阈值`maximumPoolSize`，若否，则新建一个非核心线程执行任务。若达到阈值，则执行线程池饱和策略。

线程池饱和策略分为一下几种：

- `AbortPolicy`：直接抛出一个异常，默认策略
- `DiscardPolicy`：直接丢弃任务
- `DiscardOldestPolicy`：抛弃下一个将要被执行的任务(最旧任务)
- `CallerRunsPolicy`：主线程中执行任务

从流程角度，更形象的图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652109204061-89618b9c-60ee-4e4a-99be-77ab852b2280.png#clientId=udf4bac81-0958-4&from=paste&id=ub7ca8a71&originHeight=290&originWidth=720&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u718a5c7c-ff68-45dd-a3c1-09cea417e28&title=)<br />从结构角度，更形象的图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652109204139-df380a85-639f-48fa-8dec-bc1bc1672923.png#clientId=udf4bac81-0958-4&from=paste&id=u34347c2e&originHeight=595&originWidth=904&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf5815225-788f-401d-a728-4743d192573&title=)
<a name="tCjFJ"></a>
## 几种常用的线程池
<a name="YAmnY"></a>
### 几种典型的工作队列

- `ArrayBlockingQueue`：使用数组实现的有界阻塞队列，特性先进先出
- `LinkedBlockingQueue`：使用链表实现的阻塞队列，特性先进先出，可以设置其容量，默认为`Interger.MAX_VALUE`，特性先进先出
- `PriorityBlockingQueue`：使用平衡二叉树堆，实现的具有优先级的无界阻塞队列
- `DelayQueue`：无界阻塞延迟队列，队列中每个元素均有过期时间，当从队列获取元素时，只有
- 过期元素才会出队列。队列头元素是最块要过期的元素。
- `SynchronousQueue`：一个不存储元素的阻塞队列，每个插入操作，必须等到另一个线程调用移除操作，否则插入操作一直处于阻塞状态
<a name="xrHzc"></a>
### 几种典型的线程池
<a name="KbMz6"></a>
#### `SingleThreadExecutor`
```java
public static ExecutorService newSingleThreadExecutor() {  
    return new FinalizableDelegatedExecutorService  
        (new ThreadPoolExecutor(1, 1,  
                                0L, TimeUnit.MILLISECONDS,  
                                new LinkedBlockingQueue<Runnable>()));  
}
```
创建单个线程。它适用于需要保证顺序地执行各个任务；并且在任意时间点，不会有多个线程是活动的应用场景。<br />`SingleThreadExecutor`的`corePoolSize`和`maximumPoolSize`被设置为1，使用无界队列`LinkedBlockingQueue`作为线程池的工作队列。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652109204170-d3e0a33e-2d34-441f-9205-e3f8351788f7.png#clientId=udf4bac81-0958-4&from=paste&id=u781cc94c&originHeight=342&originWidth=674&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uafd1618c-9810-4ef4-b3df-c1446212c37&title=)

- 当线程池中没有线程时，会创建一个新线程来执行任务。
- 当前线程池中有一个线程后，将新任务加入LinkedBlockingQueue
- 线程执行完第一个任务后，会在一个无限循环中反复从LinkedBlockingQueue 获取任务来执行。

**使用场景：**适用于串行执行任务场景
<a name="jB6wS"></a>
#### `FixedThreadPool`
```java
public static ExecutorService newFixedThreadPool(int nThreads, ThreadFactory threadFactory) {  
	return new ThreadPoolExecutor(nThreads, nThreads,  
								  0L, TimeUnit.MILLISECONDS,  
								  new LinkedBlockingQueue<Runnable>(),  
								  threadFactory);  
}
```
`corePoolSize`等于`maximumPoolSize`，所以线程池中只有核心线程，使用无界阻塞队列`LinkedBlockingQueue`作为工作队列<br />`FixedThreadPool`是一种线程数量固定的线程池，当线程处于空闲状态时，他们并不会被回收，除非线程池被关闭。当所有的线程都处于活动状态时，新的任务都会处于等待状态，直到有线程空闲出来。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652109204166-168ca23f-b40f-44d6-9cd2-eab8cd581d58.png#clientId=udf4bac81-0958-4&from=paste&id=u7c6a8657&originHeight=463&originWidth=674&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2b38ee03-d8d2-4543-9fbb-4b0ab7d2bd5&title=)

- 如果当前运行的线程数少于`corePoolSize`，则创建新线程来执行任务。
- 在线程数目达到`corePoolSize`后，将新任务放到`LinkedBlockingQueue`阻塞队列中。
- 线程执行完(1)中任务后，会在循环中反复从`LinkedBlockingQueue`获取任务来执行。

使用场景：适用于处理CPU密集型的任务，确保CPU在长期被工作线程使用的情况下，尽可能的少的分配线程，即适用执行长期的任务。
<a name="vEhRk"></a>
#### `CachedThreadPool`
```java
public static ExecutorService newCachedThreadPool() {  
	return new ThreadPoolExecutor(0, Integer.MAX_VALUE,  
								  60L, TimeUnit.SECONDS,  
								  new SynchronousQueue<Runnable>());  
}
```
核心线程数为0，总线程数量阈值为`Integer.MAX_VALUE`，即可以创建无限的非核心线程<br />**执行流程**

- 先执行`SynchronousQueue`的`offer`方法提交任务，并查询线程池中是否有空闲线程来执行`SynchronousQueue`的`poll`方法来移除任务。如果有，则配对成功，将任务交给这个空闲线程
- 否则，配对失败，创建新的线程去处理任务
- 当线程池中的线程空闲时，会执行`SynchronousQueue`的`poll`方法等待执行`SynchronousQueue`中新提交的任务。若等待超过60s，空闲线程就会终止

**流程形象图**<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1652109769871-2a78f3cc-3bec-4824-aced-a523608e55ca.jpeg)<br />**结构形象图**<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652109204469-b6fa1d01-89a2-4966-bb8f-2a6cf7e50318.png#clientId=udf4bac81-0958-4&from=paste&id=u7dcdada0&originHeight=519&originWidth=674&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ucd45f0f6-58e5-42ec-b601-d1ff644b5e2&title=)<br />使用场景：执行大量短生命周期任务。因为`maximumPoolSize`是无界的，所以提交任务的速度 > 线程池中线程处理任务的速度就要不断创建新线程；每次提交任务，都会立即有线程去处理，因此`CachedThreadPool`适用于处理大量、耗时少的任务。
<a name="KXAdE"></a>
#### `ScheduledThreadPool`
```java
public static ScheduledExecutorService newScheduledThreadPool(int corePoolSize) {  
	return new ScheduledThreadPoolExecutor(corePoolSize);  
}  
public ScheduledThreadPoolExecutor(int corePoolSize) {  
	super(corePoolSize, Integer.MAX_VALUE, 0, NANOSECONDS,  
		  new DelayedWorkQueue());  
}
```
线程总数阈值为`Integer.MAX_VALUE`，工作队列使用`DelayedWorkQueue`，非核心线程存活时间为0，所以线程池仅仅包含固定数目的核心线程。<br />两种方式提交任务：

- `scheduleAtFixedRate`：按照固定速率周期执行
- `scheduleWithFixedDelay`：上个任务延迟固定时间后执行

使用场景：周期性执行任务，并且需要限制线程数量的场景
<a name="pFQwa"></a>
##### 面试题：使用无界队列的线程池会导致内存飙升吗？
答案 ：会的，`newFixedThreadPool`使用了无界的阻塞队列`LinkedBlockingQueue`，如果线程获取一个任务后，任务的执行时间比较长，会导致队列的任务越积越多，导致机器内存使用不停飙升， 最终导致OOM。
