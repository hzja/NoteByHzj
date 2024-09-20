JavaSpringBoot
<a name="tSHcN"></a>
### 前言
解析SpringBoot内置Tomcat调优并发线程数的一些参数，并结合源码进行分析
<a name="V4Jxc"></a>
### 参数
<a name="uT366"></a>
##### 线程池核心线程数
`server.tomcat.min-spare-threads`：该参数为Tomcat处理业务的核心线程数大小，默认值为10
<a name="OQS5k"></a>
##### 线程池最大线程数
`server.tomcat.max-threads`：该参数为Tomcat处理业务的最大线程数大小，默认值为200，当对并发量有一点值时可以调大该参数
<a name="tqAL6"></a>
##### 请求最大连接数
`server.tomcat.max-connections`：该参数为请求的最大连接数，默认值为10000，注意这个参数并不是设置在线程池上的，而是在Tomcat的Acceptor类（专门处理连接的线程类）来控制的，结合源码可以看到<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1653915569974-98d15832-c74c-4490-a864-55efae105e80.png#clientId=u38cdec31-1866-4&from=paste&height=508&id=u393eecc9&originHeight=1269&originWidth=2338&originalType=binary&ratio=1&rotation=0&showTitle=false&size=204274&status=done&style=shadow&taskId=ucdb3c0f8-d006-4c90-9f90-e735136e6a7&title=&width=935.2)
```java
protected void countUpOrAwaitConnection() throws InterruptedException {
	if (maxConnections==-1) return;
	LimitLatch latch = connectionLimitLatch;
	if (latch!=null) latch.countUpOrAwait();
}
```
可以看到当最大连接数满了之后会进行等待
<a name="Z9LXg"></a>
##### accept-count
`server.tomcat.accept-count`：这个参数实际上和Tomcat没有太大关系，默认值为100<br />先看下文档的定义
```java
/**
 * Allows the server developer to specify the acceptCount (backlog) that
 * should be used for server sockets. By default, this value
 * is 100.
 */
private int acceptCount = 100;
```
这个参数是服务端创建`ServerSocket`时操作系统控制同时连接的最大数量的，服务端接收连接是通过`accept()`来的，`accept()`是非常快的，所以accept-count的不需要太大，正常保持默认值100即可了，`acceptCount`这个参数和线程池无关，会被映射为backlog参数，是socket的参数，在源码的使用是在`NioEndpoint`类的`initServerSocket`方法，在Tomcat中的名字是backlog在SpringBoot内置Tomcat中名字没有使用backlog而是使用`acceptCount`<br />`**serverSock.socket().bind(addr,getAcceptCount())**`
```java
protected void initServerSocket() throws Exception {
	if (!getUseInheritedChannel()) {
		serverSock = ServerSocketChannel.open();
		socketProperties.setProperties(serverSock.socket());
		InetSocketAddress addr = (getAddress()!=null?new InetSocketAddress(getAddress(),getPort()):new InetSocketAddress(getPort()));
		// 核心代码
		serverSock.socket().bind(addr,getAcceptCount());
	} else {
		// Retrieve the channel provided by the OS
		Channel ic = System.inheritedChannel();
		if (ic instanceof ServerSocketChannel) {
			serverSock = (ServerSocketChannel) ic;
		}
		if (serverSock == null) {
			throw new IllegalArgumentException(sm.getString("endpoint.init.bind.inherited"));
		}
	}
	serverSock.configureBlocking(true); //mimic APR behavior
}
```
<a name="LjUoU"></a>
### Tomcat线程池处理机制
Tomcat最终使用线程池来处理业务逻辑，java默认的线程池的规则：<br />**核心线程数满了则优先放入队列，当队列满了之后才会创建非核心线程来处理，那么**Tomcat**是这样做的吗？**<br />首先如果Tomcat这样做，那么当达到核心线程后后续任务就需要等待了，这显然是不合理的，通过源码来看下Tomcat是如何处理的<br />在`AbstractEndpoint`的`createExecutor`创建了处理业务数据的线程池
```java
public void createExecutor() {
	internalExecutor = true;
	TaskQueue taskqueue = new TaskQueue();
	TaskThreadFactory tf = new TaskThreadFactory(getName() + "-exec-", daemon, getThreadPriority());
	executor = new ThreadPoolExecutor(getMinSpareThreads(), getMaxThreads(), 60, TimeUnit.SECONDS,taskqueue, tf);
	taskqueue.setParent( (ThreadPoolExecutor) executor);
}
```
主要是使用了TaskQueue队列，`ThreadPoolExecutor`并不是jdk的，而是Tomcat重写的。<br />从线程池的处理方法`execute`看起
```java
public void execute(Runnable command) {
	execute(command,0,TimeUnit.MILLISECONDS);
}

public void execute(Runnable command, long timeout, TimeUnit unit) {
	submittedCount.incrementAndGet();
	try {
		// 核心代码
		super.execute(command);
	} catch (RejectedExecutionException rx) {
		if (super.getQueue() instanceof TaskQueue) {
			final TaskQueue queue = (TaskQueue)super.getQueue();
			try {
				if (!queue.force(command, timeout, unit)) {
					submittedCount.decrementAndGet();
					throw new RejectedExecutionException("Queue capacity is full.");
				}
			} catch (InterruptedException x) {
				submittedCount.decrementAndGet();
				throw new RejectedExecutionException(x);
			}
		} else {
			submittedCount.decrementAndGet();
			throw rx;
		}

	}
}
```
又调用会jdk的`execute`了
```java
public void execute(Runnable command) {
	if (command == null)
		throw new NullPointerException();

	int c = ctl.get();
	// 1、工作线程数小于核心线程数则添加任务，核心线程会处理
	if (workerCountOf(c) < corePoolSize) {
		if (addWorker(command, true))
			return;
		c = ctl.get();
	}
	// 2、工作线程不小于核心线程数，则放到workQueue队列中
	if (isRunning(c) && workQueue.offer(command)) {
		int recheck = ctl.get();
		if (! isRunning(recheck) && remove(command))
			reject(command);
		else if (workerCountOf(recheck) == 0)
			addWorker(null, false);
	}
	// 3、否则添加任务，addWorker会进行创建线程
	else if (!addWorker(command, false))
		reject(command);
}
```
从这里可以看到jdk线程池的机制，Tomcat使用了自己的TaskQueue队列，所以看代码2处当核心线程用完了会调用队列的offer方法<br />看TaskQueue的offer
```java
public boolean offer(Runnable o) {
	//we can't do any checks
	// parent就是指线程池，没有线程池则添加到队列
	if (parent==null) return super.offer(o);
	//we are maxed out on threads, simply queue the object
	// 线程数量已经达到了最大线程数，那么只能添加到队列
	if (parent.getPoolSize() == parent.getMaximumPoolSize()) return super.offer(o);
	//we have idle threads, just add it to the queue
	// 如果当前处理的任务数量小于当前线程池中线程的数量，那么任务放到线程池，即相当于马上会有空闲线程来处理
	if (parent.getSubmittedCount()<=(parent.getPoolSize())) return super.offer(o);
	//if we have less threads than maximum force creation of a new thread
	// TODO 核心代码，如果当前线程数量还没有达到线程池最大线程池的数量，那么就直接创建线程，这里返回false
	if (parent.getPoolSize()<parent.getMaximumPoolSize()) return false;
	//if we reached here, we need to add it to the queue
	// 最后的策略，放到队列
	return super.offer(o);
}
```
可以看到当执行offer时，不是直接放到队列的，当线程池总线程数量还没达到线程池最大线程数时会返回false，返回false时就会执行线程池`execute`的代码3处，执行`addWorker(command, false)`，也就开始创建新的线程来处理当前任务了。
<a name="z9vcf"></a>
### 总结
Tomcat主要通过使用自己的`TaskQueue`队列来对线程池做出了不同的策略，也就是Tomcat当线程数大于核心数时就会直接创建新的线程来处理，而不是放到队列。
