Java Thread<br />Java中在处理异常的时候，通常的做法是使用try-catch-finally来包含代码块，但是Java自身还有一种方式可以处理——使用UncaughtExceptionHandler。它能检测出某个线程由于未捕获的异常而终结的情况。当一个线程由于未捕获异常而退出时，JVM会把这个事件报告给应用程序提供的UncaughtExceptionHandler异常处理器（这是Thread类中的接口）：
```java
//Thread类中的接口
public interface UncaughtExceptionHanlder {
	void uncaughtException(Thread t, Throwable e);
}
```
在Java 5以后，可以通过以下实例方法来为每个线程设置一个UncaughtExceptionHandler：
```java
Thread.setUncaughtExceptionHandler(UncaughtExceptionHandler handler);//实例方法
```
或者通过以下静态方法来设置默认的UncaughtExceptionHandler：
```java
Thread.setDefaultUncaughtExceptionHandler(UncaughtExceptionHandler handler);//静态方法
```
**这些异常处理器中，只有一个将会被调用**——JVM首先搜索每个线程的异常处理器，若没有，则搜索该线程的ThreadGroup的异常处理器。ThreadGroup中的默认异常处理器实现是将处理工作逐层委托给上层的ThreadGroup，直到某个ThreadGroup的异常处理器能够处理该异常，否则一直传递到顶层的ThreadGroup。**顶层ThreadGroup的异常处理器委托给默认的系统处理器**（如果默认的处理器存在，默认情况下为空），**否则把栈信息输出到System.err**。下面是一个Example：
```java
import java.lang.Thread.UncaughtExceptionHandler;

public class T {
	public static void main(String[] args) throws Exception {
		//为所有线程设置默认的未捕捉异常处理器
		Thread.setDefaultUncaughtExceptionHandler(new MyDefaultExceptionHandler());
		Thread.currentThread().setName("Main Thread");
		
		Thread thread = new Thread(new MyTask("MyTask"), "Child Thread");
		//为某个线程单独设置异常处理器
		thread.setUncaughtExceptionHandler(new MyUncaughtExceptionHandler());
		thread.start();
		//主线程抛出异常，将会使用默认的异常处理器
		throw new RuntimeException("IllegalArgumentException");
	}
}

class MyDefaultExceptionHandler implements UncaughtExceptionHandler {
	@Override
	public void uncaughtException(Thread t, Throwable e) {
		System.out.println("MyDefaultExceptionHandler: Thread: " + 
				t.getName() + ", Message: " + e.getMessage());
	}
}
class MyUncaughtExceptionHandler implements UncaughtExceptionHandler {
	@Override
	public void uncaughtException(Thread t, Throwable e) {
		System.out.println("MyUncaughtExceptionHandler: Thread: " + 
				t.getName() + ", Message: " + e.getMessage());
	}
}
class MyTask implements Runnable {
	private String name;
	public MyTask(String name) {
		this.name = name;
	}
	public MyTask(){}
	public String getName() {
		return name;
	}
	@Override
	public void run() {
		throw new RuntimeException(name + " gets a NullPointerException");
	}
}
```
执行结果：
```java
MyDefaultExceptionHandler: Thread: Main Thread, Message: IllegalArgumentException
MyUncaughtExceptionHandler: Thread: Child Thread, Message: MyTask gets a NullPointerException
```
可以看到，Main Thread由于没有显式设置UncaughtExceptionHandler，其抛出的未捕获异常，被默认异常处理器MyDefaultUncaughtExceptionHandler处理了，而Child Thread由于单独设置了UncaughtExceptionHanlder，其抛出的未捕获异常，则被`Thread`本身自带的MyUncaughtExceptionHandler处理了。<br />如果要为线程池中的所有线程设置一个UncaughtExceptionHandler，则需要为ThreadPoolExecutor的构造函数提供一个**自定义**的`ThreadFactory`（与所有的线程操控一样，只有线程的所有者能够改变线程的UncaughtExceptionHandler）：
```java
import java.lang.Thread.UncaughtExceptionHandler;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.ThreadFactory;

public class T {
	public static void main(String[] args) throws Exception {
		//使用自定义的ThreadFactory来创建线程，并绑定同一个异常处理器
		UncaughtExceptionHandler handler = new MyUncaughtExceptionHandler();
		ExecutorService executor = Executors.newCachedThreadPool(new MyThreadFactory(handler));
		executor.execute(new MyTask("task1"));
		executor.execute(new MyTask("task2"));
		executor.execute(new MyTask("task3"));
		executor.shutdown();
	}
}
class MyTask implements Runnable {
	private String name;
	public MyTask(String name) {
		this.name = name;
	}
	public MyTask(){}
	public String getName() {
		return name;
	}
	@Override
	public void run() {
		throw new RuntimeException(name + " gets a NullPointerException");
	}
}
class MyUncaughtExceptionHandler implements UncaughtExceptionHandler {
	@Override
	public void uncaughtException(Thread t, Throwable e) {
		System.out.println("MyUncaughtExceptionHandler: Thread: " + 
				t.getName() + ", Message: " + e.getMessage());
	}
}
class MyThreadFactory implements ThreadFactory {
	private UncaughtExceptionHandler handler;
	public MyThreadFactory(UncaughtExceptionHandler handler) {
		this.handler = handler;
	}
	@Override
	public Thread newThread(Runnable r) {
		Thread thread = new Thread(r);
		//在这里设置异常处理器
		thread.setUncaughtExceptionHandler(handler);
		return thread;
	}
}
```
执行结果：
```java
MyUncaughtExceptionHandler: Thread: Thread-0, Message: task1 gets a NullPointerException
MyUncaughtExceptionHandler: Thread: Thread-2, Message: task3 gets a NullPointerException
MyUncaughtExceptionHandler: Thread: Thread-1, Message: task2 gets a NullPointerException
```
从结果中可以看出，线程池中的每个线程都使用同一个异常处理器来处理未捕获的异常。<br />不过，上面的结果能证明：**通过**`**execute**`**提交的任务，能将它抛出的异常交给未捕获的异常处理器**。下面的例子只修改了main方法（其余部分请参考前文），以`submit`方式提交任务：
```java
public class T {
	public static void main(String[] args) throws Exception {
		//使用自定义的ThreadFactory来创建线程，并绑定同一个异常处理器
		UncaughtExceptionHandler handler = new MyUncaughtExceptionHandler();
		ExecutorService executor = Executors.newCachedThreadPool(new MyThreadFactory(handler));
		//通过submit方法提交任务
		Future future1 = executor.submit(new MyTask("task1"));
		Future future2 = executor.submit(new MyTask("task2"));
		System.out.println(future1.get());
		System.out.println(future2.get());
		executor.shutdown();
	}
}
```
执行结果：
```java
Exception in thread "main" java.util.concurrent.ExecutionException: 
java.lang.RuntimeException: task1 gets a NullPointerException
	at java.util.concurrent.FutureTask.report(FutureTask.java:122)
	at java.util.concurrent.FutureTask.get(FutureTask.java:188)
	at T.main(T.java:15)
Caused by: java.lang.RuntimeException: task1 gets a NullPointerException
	at MyTask.run(T.java:31)
	at java.util.concurrent.Executors$RunnableAdapter.call(Executors.java:471)
	at java.util.concurrent.FutureTask.run(FutureTask.java:262)
	at java.util.concurrent.ThreadPoolExecutor.runWorker(ThreadPoolExecutor.java:1145)
	at java.util.concurrent.ThreadPoolExecutor$Worker.run(ThreadPoolExecutor.java:615)
	at java.lang.Thread.run(Thread.java:745)
```
结果说明，`future1.get`的时候遇到了ExecutionException。再来看看`Future.get`方法的实现（在java.util.concurrent.FutureTask类中）：
```java
public V get() throws InterruptedException, ExecutionException {
    int s = state;
    if (s <= COMPLETING)//如果任务没有结束，则等待结束
        s = awaitDone(false, 0L);
    return report(s);//如果执行结束，则报告执行结果
}

@SuppressWarnings("unchecked")
private V report(int s) throws ExecutionException {
    Object x = outcome;
    if (s == NORMAL)//如果执行正常，则返回结果
        return (V)x;
    if (s >= CANCELLED)//如果任务被取消，调用get则报CancellationException
        throw new CancellationException();
    throw new ExecutionException((Throwable)x);//执行异常，则抛出ExecutionException
}
```
源代码说明：如果一个由`submit`提交的任务由于抛出了异常而结束，那么这个异常将被`Future.get`封装在`ExecutionException`中重新抛出。所以，**通过**`**submit**`**提交到线程池的任务，无论是抛出的未检查异常还是已检查异常，都将被认为是任务返回状态的一部分，因此不会交由异常处理器来处理。**
