Java线程池ThreadPoolExecutor
<a name="M00IW"></a>
## 一、ThreadPoolExecutor类讲解
<a name="FHryl"></a>
### 1、线程池状态：
五种状态：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652227978080-716672ab-4887-47f6-966a-4cc343b47718.png#averageHue=%23f6f5f5&clientId=u07298dc1-0d66-4&from=paste&id=u9dbe1e57&originHeight=286&originWidth=963&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u14a44bb1-8e2a-4f5d-9b36-027af06670d&title=)

- 线程池的`shutdown()` 方法，将线程池由 RUNNING（运行状态）转换为 SHUTDOWN状态
- 线程池的`shutdownNow()`方法，将线程池由RUNNING 或 SHUTDOWN 状态转换为 STOP 状态。

注：SHUTDOWN 状态 和 STOP 状态 先会转变为 TIDYING 状态，最终都会变为 TERMINATED
<a name="DqLP4"></a>
### 2、`ThreadPoolExecutor`构造函数：
`ThreadPoolExecutor`继承自`AbstractExecutorService`，而`AbstractExecutorService`实现了`ExecutorService`接口。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652227978070-b37ad715-20f0-4714-a2fc-6b590a51ed09.png#averageHue=%23f4f2f0&clientId=u07298dc1-0d66-4&from=paste&id=u07b2ed0d&originHeight=168&originWidth=416&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u49cabd65-5829-48ce-b7d1-ee0b9117e53&title=)<br />接下来分别讲解这些参数的含义。
<a name="Ps3ZI"></a>
#### 2.1）线程池工作原理：

- `corePoolSize`：线程池中核心线程数的最大值
- `maximumPoolSize`：线程池中能拥有最多线程数
- `workQueue`：用于缓存任务的阻塞队列

当调用线程池`execute()` 方法添加一个任务时，线程池会做如下判断：

- 如果有空闲线程，则直接执行该任务；
- 如果没有空闲线程，且当前运行的线程数少于`corePoolSize`，则创建新的线程执行该任务；
- 如果没有空闲线程，且当前的线程数等于`corePoolSize`，同时阻塞队列未满，则将任务入队列，而不添加新的线程；
- 如果没有空闲线程，且阻塞队列已满，同时池中的线程数小于`maximumPoolSize`，则创建新的线程执行任务；
- 如果没有空闲线程，且阻塞队列已满，同时池中的线程数等于`maximumPoolSize`，则根据构造函数中的 handler 指定的策略来拒绝新的任务。

![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1702363383481-a8e2bc57-40cd-469d-8565-62c7076555b9.jpeg)
<a name="fJ4qZ"></a>
#### 2.2）`KeepAliveTime`：

- `keepAliveTime `：表示空闲线程的存活时间
- `TimeUnit unit` ：表示keepAliveTime的单位

当一个线程无事可做，超过一定的时间（keepAliveTime）时，线程池会判断，如果当前运行的线程数大于 `corePoolSize`，那么这个线程就被停掉。所以线程池的所有任务完成后，它最终会收缩到 corePoolSize 的大小。<br />注：如果线程池设置了`allowCoreThreadTimeout`参数为true（默认false），那么当空闲线程超过`keepaliveTime`后直接停掉。（不会判断线程数是否大于`corePoolSize`）即：最终线程数会变为0。
<a name="HvWue"></a>
#### 2.3）`workQueue`任务队列：

- `workQueue`：它决定了缓存任务的排队策略
- `ThreadPoolExecutor`线程池推荐了三种等待队列，它们是：`SynchronousQueue`、`LinkedBlockingQueue` 和 `ArrayBlockingQueue`。
<a name="mAkzX"></a>
##### 1）有界队列：

- `SynchronousQueue` ：一个不存储元素的阻塞队列，每个插入操作必须等到另一个线程调用移除操作，否则插入操作一直处于 阻塞状态，吞吐量通常要高于`LinkedBlockingQueue`，静态工厂方法 `Executors.newCachedThreadPool` 使用了这个队列。
- `ArrayBlockingQueue`：一个由数组支持的有界阻塞队列。此队列按 FIFO（先进先出）原则对元素进行排序。一旦创建了这样的缓存区，就不能再增加其容量。试图向已满队列中放入元素会导致操作受阻塞；试图从空队列中提取元素将导致类似阻塞。
<a name="kOWtI"></a>
##### 2）无界队列：

- `LinkedBlockingQueue`：基于链表结构的无界阻塞队列，它可以指定容量也可以不指定容量（实际上任何无限容量的队列/栈都是有容量的，这个容量就是`Integer.MAX_VALUE`）
- `PriorityBlockingQueue`：是一个按照优先级进行内部元素排序的无界阻塞队列。队列中的元素必须实现 Comparable 接口，这样才能通过实现compareTo()方法进行排序。优先级最高的元素将始终排在队列的头部；`PriorityBlockingQueue` 不会保证优先级一样的元素的排序。

注意：`keepAliveTime`和`maximumPoolSize`及`BlockingQueue`的类型均有关系。如果`BlockingQueue`是无界的，那么永远不会触发`maximumPoolSize`，自然`keepAliveTime`也就没有了意义。
<a name="pTzfA"></a>
#### 2.4）`threadFactory`：
`threadFactory `：指定创建线程的工厂。（可以不指定）<br />如果不指定线程工厂时，`ThreadPoolExecutor` 会使用`ThreadPoolExecutor.defaultThreadFactory` 创建线程。默认工厂创建的线程：同属于相同的线程组，具有同为 `Thread.NORM_PRIORITY` 的优先级，以及名为 “pool-XXX-thread-” 的线程名（XXX为创建线程时顺序序号），且创建的线程都是非守护进程。
<a name="IBXI4"></a>
#### 2.5）handler 拒绝策略：
`handler `：表示当 `workQueue` 已满，且池中的线程数达到 `maximumPoolSize` 时，线程池拒绝添加新任务时采取的策略。（可以不指定）<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652227978032-8d01b608-560a-4e2b-a88d-339907beb410.png#averageHue=%23f6f5f4&clientId=u07298dc1-0d66-4&from=paste&id=u7fcda291&originHeight=216&originWidth=821&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9dfc8e09-9adb-422a-8d1c-c0e9a1e519f&title=)<br />最科学的的还是 AbortPolicy 提供的处理方式：抛出异常，由开发人员进行处理。
<a name="cAH25"></a>
### 3、常用方法：
除了在创建线程池时指定上述参数的值外，还可在线程池创建以后通过如下方法进行设置。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652227978048-be639f6c-ea16-4b79-951c-b605a0c9813e.png#averageHue=%23f7f5f3&clientId=u07298dc1-0d66-4&from=paste&id=u0ae78238&originHeight=123&originWidth=601&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6936b42c-194d-4abf-95b7-a39a6f9cd18&title=)<br />此外，还有一些方法：

- `getCorePoolSize()`：返回线程池的核心线程数，这个值是一直不变的，返回在构造函数中设置的coreSize大小；
- `getMaximumPoolSize()`：返回线程池的最大线程数，这个值是一直不变的，返回在构造函数中设置的coreSize大小；
- `getLargestPoolSize()`：记录了曾经出现的最大线程个数（水位线）；
- `getPoolSize()`：线程池中当前线程的数量；
- `getActiveCount()`：Returns the approximate（近似） number of threads that are actively executing tasks。
- `prestartAllCoreThreads()`：会启动所有核心线程，无论是否有待执行的任务，线程池都会创建新的线程，直到池中线程数量达到 corePoolSize；
- `prestartCoreThread()`：会启动一个核心线程（同上）；
- `allowCoreThreadTimeOut(true)`：允许核心线程在KeepAliveTime时间后，退出；
<a name="f55i6"></a>
### 4、`Executors`类：
`Executors`类的底层实现便是ThreadPoolExecutor！Executors 工厂方法有：

- `Executors.newCachedThreadPool()`：无界线程池，可以进行自动线程回收
- `Executors.newFixedThreadPool(int)`：固定大小线程池
- `Executors.newSingleThreadExecutor()`：单个后台线程

它们均为大多数使用场景预定义了设置。不过在阿里java文档中说明，尽量不要用该类创建线程池。
<a name="lkLDt"></a>
## 二、线程池相关接口介绍：
<a name="Hrw4g"></a>
### 1、`ExecutorService`接口：
该接口是真正的线程池接口。上面的`ThreadPoolExecutor`以及下面的`ScheduledThreadPoolExecutor`都是该接口的实现类。改接口常用方法：

- `Future<?> submit(Runnable task)`：提交Runnable任务到线程池，返回Future对象，由于Runnable没有返回值，也就是说调用Future对象get()方法返回null；
- `<T> Future<T> submit(Callable<T> task)`：提交Callable任务到线程池，返回Future对象，调用Future对象get()方法可以获取Callable的返回值；
- `<T> Future<T> submit(Runnable task，T result)`：提交Runnable任务到线程池，返回Future对象，调用Future对象get()方法可以获取Runnable的参数值；
- `invokeAll(collection of tasks)/invokeAll(collection of tasks, long timeout, TimeUnit unit)`：`invokeAll`会按照任务集合中的顺序将所有的Future添加到返回的集合中，该方法是一个阻塞的方法。只有当所有的任务都执行完毕时，或者调用线程被中断，又或者超出指定时限时，`invokeAll`方法才会返回。当`invokeAll`返回之后每个任务要么返回，要么取消，此时客户端可以调用`get`/`isCancelled`来判断具体是什么情况。
- `invokeAny(collection of tasks)/invokeAny(collection of tasks, long timeout, TimeUnit unit)`：阻塞的方法，不会返回 Future 对象，而是返回集合中某一个Callable 对象的结果，而且无法保证调用之后返回的结果是哪一个 Callable，如果一个任务运行完毕或者抛出异常，方法会取消其它的 Callable 的执行。和`invokeAll`区别是只要有一个任务执行完了，就把结果返回，并取消其他未执行完的任务；同样，也带有超时功能；
- `shutdown()`：在完成已提交的任务后关闭服务，不再接受新任；
- `shutdownNow()`：停止所有正在执行的任务并关闭服务；
- `isTerminated()`：测试是否所有任务都执行完毕了；
- `isShutdown()`：测试是否该ExecutorService已被关闭。
<a name="K5I4D"></a>
#### 1.1）`submit`方法示例：
线程池接口中有以下三个主要方法，接下来看一下具体示例：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652227978418-27dcc1a3-65ea-4e42-bc92-6cd13a9fbb85.png#averageHue=%23edebe9&clientId=u07298dc1-0d66-4&from=paste&id=u911b079d&originHeight=58&originWidth=508&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5eef2f5f-cd36-4812-a28c-de7f27d70a7&title=)
<a name="IeUaQ"></a>
##### 1）Callable：
```java
public static ThreadPoolExecutor threadPool = new ThreadPoolExecutor(5, 50, 300, TimeUnit.SECONDS, 
																	 new ArrayBlockingQueue<Runnable>(50),  
																	 new ThreadFactory(){ public Thread newThread(Runnable r) {
																		 return new Thread(r, "schema_task_pool_" + r.hashCode());
																	 }}, new ThreadPoolExecutor.DiscardOldestPolicy());

public static void callableTest() {
	int a = 1;
	//callable
	Future<Boolean> future = threadPool.submit(new Callable<Boolean>(){
		@Override
		public Boolean call() throws Exception {
			int b = a + 100;
			System.out.println(b);
			return true;
		}
	});
	try {
		System.out.println("feature.get");
		Boolean boolean1 = future.get();
		System.out.println(boolean1);
	} catch (InterruptedException e) {
		System.out.println("InterruptedException...");
		e.printStackTrace();
	} catch (ExecutionException e) {
		System.out.println("execute exception...");
		e.printStackTrace();
	} 
}
```
<a name="ecQka"></a>
##### 2）Runnable：
```java
public static void runnableTest() {
	int a = 1;
	//runnable
	Future<?> future1 = threadPool.submit(new Runnable(){
		@Override
		public void run() {
			int b = a + 100;
			System.out.println(b);
		}
	});
	try {
		System.out.println("feature.get");
		Object x = future1.get(900,TimeUnit.MILLISECONDS);
		System.out.println(x);//null
	} catch (InterruptedException e) {
		e.printStackTrace();
	} catch (ExecutionException e) {
		System.out.println("execute exception...");
		e.printStackTrace();
	} catch (TimeoutException e) {
		e.printStackTrace();
	}
}
```
<a name="FitZx"></a>
##### 3）Runnable+result：
```java
class RunnableTask implements Runnable {
	Person p;
	RunnableTask(Person p) {
		this.p = p;
	}

	@Override
	public void run() {
		p.setId(1);
		p.setName("Runnable Task...");
	}
}
class Person {
	private Integer id;
	private String name;

	public Person(Integer id, String name) {
		super();
		this.id = id;
		this.name = name;
	}
	public Integer getId() {
		return id;
	}
	public void setId(Integer id) {
		this.id = id;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	@Override
	public String toString() {
		return "Person [id=" + id + ", name=" + name + "]";
	}
}

public static void runnableTest2() {
	//runnable + result
	Person p = new Person(0,"person");
	Future<Person> future2 = threadPool.submit(new RunnableTask(p),p);
	try {
		System.out.println("feature.get");
		Person person = future2.get();
		System.out.println(person);
	} catch (InterruptedException e) {
		e.printStackTrace();
	} catch (ExecutionException e) {
		e.printStackTrace();
	}
}
```
<a name="noG7t"></a>
#### 1.2）线程池执行时，`Callable`的`call`方法（`Runnable`的`run`方法）抛出异常后，会出现什么？
在上面的例子中可以看到，线程池无论是执行`Callable`还是`Runnable`，调用返回的`Future`对象`get()`方法时需要处理两种异常（如果是调用`get(timeout)`方法，需要处理三种异常），如下：
```java
//在线程池上运行
Future<Object> future = threadPool.submit(callable);
try {
	System.out.println("feature.get");
	Object x = future.get(900,TimeUnit.MILLISECONDS);
	System.out.println(x);
} catch (InterruptedException e) {
	e.printStackTrace();
} catch (ExecutionException e) {
	System.out.println("execute exception...");
	e.printStackTrace();
} catch (TimeoutException e) {
	e.printStackTrace();
}
```

- 如果`get`方法被打断，进入`InterruptedException`异常；
- 如果线程执行过程（`call`、`run`方法）中抛出异常，进入`ExecutionException`异常；
- 如果`get`方法超时，进入`TimeoutException`异常；
<a name="U9J6z"></a>
#### 1.3）`submit()`和`execute()`方法区别：
`ExecutorService`、`ScheduledExecutorService`接口的`submit()`和`execute()`方法都是把任务提交到线程池中，但二者的区别是

- 接收的参数不一样，`execute`只能接收`Runnable`类型、`submit`可以接收`Runnable`和`Callable`两种类型；
- `submit`有返回值，而`execute`没有返回值；`submit`方便`Exception`处理；
<a name="fIrB1"></a>
##### 1）`submit`方法内部实现：
其实`submit`方法也没有什么神秘的，就是将任务封装成了`RunnableFuture`接口（继承了`Runnable`、`Future`接口），再调用`execute`方法，看源码：
```java
public Future<?> submit(Runnable task) {
	if (task == null) throw new NullPointerException();
	RunnableFuture<Void> ftask = newTaskFor(task, null);  //转成 RunnableFuture，传的result是null
	execute(ftask);
	return ftask;
}

public <T> Future<T> submit(Runnable task, T result) {
	if (task == null) throw new NullPointerException();
	RunnableFuture<T> ftask = newTaskFor(task, result);
	execute(ftask);
	return ftask;
}

public <T> Future<T> submit(Callable<T> task) {
	if (task == null) throw new NullPointerException();
	RunnableFuture<T> ftask = newTaskFor(task);
	execute(ftask);
	return ftask;
}
```
<a name="HXl8v"></a>
##### 2）`newTaskFor`方法内部实现：
`newTaskFor`方法是new了一个`FutureTask`返回，所以三个方法其实都是把task转成`FutureTask`，如果task是`Callable`，就直接赋值，如果是`Runnable` 就转为`Callable`再赋值。<br />当`submit`参数是`Callable` 时：
```java
protected <T> RunnableFuture<T> newTaskFor(Callable<T> callable) {
	return new FutureTask<T>(callable);
}
public FutureTask(Callable<V> callable) {
	if (callable == null)
		throw new NullPointerException();
	this.callable = callable;
	this.state = NEW;      
}
```
当`submit`参数是`Runnable`时：
```java
// 按顺序看，层层调用
protected <T> RunnableFuture<T> newTaskFor(Runnable runnable, T value) {
	return new FutureTask<T>(runnable, value);
}
public FutureTask(Runnable runnable, V result) {
	this.callable = Executors.callable(runnable, result);  //转 runnable 为 callable 
	this.state = NEW; 
}
// 以下为Executors中的方法
public static <T> Callable<T> callable(Runnable task, T result) {
	if (task == null)
		throw new NullPointerException();
	return new RunnableAdapter<T>(task, result);
}
static final class RunnableAdapter<T> implements Callable<T> {  //适配器
	final Runnable task;
	final T result;
	RunnableAdapter(Runnable task, T result) {
		this.task = task;
		this.result = result;
	}
	public T call() {   
		task.run();
		return result;
	}
}
```
看了源码就揭开了神秘面纱了，就是因为`Future`需要返回结果，所以内部task必须是`Callable`，如果task是`Runnable` 就偷天换日，在`Runnable` 外面包个`Callable`马甲，返回的结果在构造时就写好。
<a name="iGOVz"></a>
#### 1.4）`ScheduledExecutorService`接口：
继承`ExecutorService`，并且提供了按时间安排执行任务的功能，它提供的方法主要有：

- `schedule(task, initDelay)`：安排所提交的Callable或Runnable任务在initDelay指定的时间后执行；
- `scheduleAtFixedRate()`：安排所提交的Runnable任务按指定的间隔重复执行；
- `scheduleWithFixedDelay()`：安排所提交的Runnable任务在每次执行完后，等待delay所指定的时间后重复执行。

注：该接口的实现类是`ScheduledThreadPoolExecutor`。
<a name="jfmrz"></a>
### 2、`Callable`接口：
jdk1.5以后创建线程可以通过一下方式：

- 继承`Thread`类，实现`void run()`方法；
- 实现`Runnable`接口，实现`void run()`方法；
- 实现`Callable`接口，实现`V call() Throws Exception`方法
<a name="l47I9"></a>
##### 1）`Callable`和`Runnale`接口区别：

- `Callable`可以抛出异常，和`Future`、`FutureTask`配合可以用来获取异步执行的结果；
- `Runnable`没有返回结果，异常只能内部消化；
<a name="hVRix"></a>
##### 2）执行`Callable`的线程的方法可以通过以下两种方式：

- 借助`FutureTask`，使用`Thread`的`start`方法来执行；
- 加入到线程池中，使用线程池的`execute`或`submit`执行；

注：`Callable`无法直接使用`Thread`来执行；<br />都知道，`Callable`带有返回值的，如果不需要返回值，却又想用`Callable`该如何做？<br />jdk中有个`Void`类型（大写V），但必须也要`return null`。
```java
threadpool.submit(new Callable<Void>() {
    @Override
    public Void call() {
        //...
        return null;
    }
});
```
<a name="nhqIo"></a>
##### 3）通过`Executors`工具类可以把`Runnable`接口转换成`Callable`接口：
`Executors`中的`callable`方法可以将`Runnable`转成`Callable`，如下：
```java
public static <T> Callable<T> callable(Runnable task, T result) {
	if (task == null)
		throw new NullPointerException();
	return new RunnableAdapter<T>(task, result);
}
```
`RunnableAdapter`类在上面已经看过源码，原理就是将返回值result作为成员变量，通过参数传递进去，进而实现了`Runnable`可以返回值。<br />示例：
```java
public static void test5() {
	Person p = new Person(0,"person");
	RunnableTask runnableTask = new RunnableTask(p);//创建runnable
	Callable<Person> callable = Executors.callable(runnableTask,p);//转换
	Future<Person> future1 = threadPool.submit(callable);//在线程池上执行Callable
	try {
		Person person = future1.get();
		System.out.println(person);
	} catch (InterruptedException | ExecutionException e) {
		e.printStackTrace();
	}

	Runnable runnable = new Runnable() {//创建Runnable
		@Override
		public void run() {

		}
	};
	Callable<Object> callable2 = Executors.callable(runnable);//转换
	Future<Object> future2 = threadPool.submit(callable2);//在线程池上执行Callable
	try {
		Object o = future2.get();
		System.out.println(o);
	} catch (InterruptedException | ExecutionException e) {
		e.printStackTrace();
	}
}
```
<a name="h61ah"></a>
### 3、`Future`接口：
<a name="wpa5Y"></a>
#### 3.1）Future是用来获取异步计算结果的接口，常用方法：

- `boolean cancel(boolean mayInterruptIfRunning)`：试图取消对此任务的执行。如果任务已完成、或已取消，或者由于某些其他原因而无法取消，则此尝试将失败。当调用 cancel 时，如果调用成功，而此任务尚未启动，则此任务将永不运行。如果任务已经启动，则 `mayInterruptIfRunning` 参数确定是否应该以试图停止任务的方式来中断执行此任务的线程。此方法返回后，对 `isDone()` 的后续调用将始终返回 true。如果此方法返回 true，则对 `isCancelled()` 的后续调用将始终返回 true。
- `boolean isCancelled()`：如果在任务正常完成前将其取消，则返回 true。
- `boolean isDone()`：如果任务已完成，则返回 true，可能由于正常终止、异常或取消而完成，在所有这些情况中，此方法都将返回 true。
- `V get()throws InterruptedException,ExecutionException`：获取异步结果，此方法会一直阻塞等到计算完成；
- `V get(long timeout,TimeUnit unit) throws InterruptedException,ExecutionException,TimeoutException`：获取异步结果，此方法会在指定时间内一直阻塞等到计算完成，超时后会抛出超时异常。

通过方法分析也知道实际上Future提供了3种功能：

- 能够中断执行中的任务；
- 判断任务是否执行完成；
- 获取任务执行完成后额结果。

但是Future只是一个接口，无法直接创建对象，因此就需要其实现类FutureTask登场。
<a name="HZGVz"></a>
#### 3.2）`FutureTask`类：
<a name="zv9Ve"></a>
##### 1）`FutureTask`类的实现：
```java
public class FutureTask<V> implements RunnableFuture<V> {
//...
}
 
public interface RunnableFuture<V> extends Runnable, Future<V> {
    /**
     * Sets this Future to the result of its computation
     * unless it has been cancelled.
     */
    void run();
}
```
`FutureTask`实现了`Runnable`、`Future`两个接口。由于`FutureTask`实现了`Runnable`，因此它既可以通过`Thread`包装来直接执行，也可以提交给`ExecuteService`来执行。并且还可以直接通过`get()`函数获取执行结果，该函数会阻塞，直到结果返回。<br />因此`FutureTask`既是`Future`、`Runnable`，又是包装了`Callable`( 如果是`Runnable`最终也会被转换为`Callable `)， 它是这两者的合体。
<a name="tGgWi"></a>
##### 2）`FutureTask`的构造函数：
```java
public FutureTask(Callable<V> callable) {
 
}
 
public FutureTask(Runnable runnable, V result) {
 
}
```
<a name="iCB2R"></a>
#### 3.3）示例：（FutureTask两种构造函数、以及在Thread和线程池上运行）
<a name="ixf6u"></a>
##### 1）FutureTask包装过的Callable在Thread、线程池上执行：
```java
public static void test3() {
	int a = 1,b = 2;
	Callable<Integer> callable = new Callable<Integer>() {
		@Override
		public Integer call() throws Exception {
			return a + b;
		}
	};
	//通过futureTask来执行Callable
	FutureTask<Integer> futureTask = new FutureTask<>(callable);

	//1.使用Thread执行线程
	new Thread(futureTask).start();
	try {
		Integer integer = futureTask.get();
		System.out.println(integer);
	} catch (InterruptedException e) {
		e.printStackTrace();
	} catch (ExecutionException e) {
		e.printStackTrace();
	}

	//2.使用线程池执行线程
	Executors.newFixedThreadPool(1).submit(futureTask);
	threadPool.shutdown();
	try {
		Integer integer = futureTask.get();
		System.out.println(integer);
	} catch (InterruptedException | ExecutionException e) {
		e.printStackTrace();
	} 
}
```
<a name="G3D5i"></a>
##### 2）FutureTask包装过的Runnable在Thread、线程池上执行：
```java
public static void test4() {
	Person p = new Person(0,"person");
	RunnableTask runnableTask = new RunnableTask(p);

	//创建futureTask来执行Runnable
	FutureTask<Person> futureTask = new FutureTask<>(runnableTask,p);

	//1.使用Thread执行线程
	new Thread(futureTask).start();
	try {
		Person x = futureTask.get();
		System.out.println(x);
	} catch (InterruptedException | ExecutionException e) {
		e.printStackTrace();
	} 

	//2.使用线程池执行线程
	threadPool.submit(futureTask);
	threadPool.shutdown();
	try {
		Person y = futureTask.get();
		System.out.println(y);
	} catch (InterruptedException | ExecutionException e) {
		e.printStackTrace();
	}
}
```
Person、RunnableTask类同上面的示例中。
