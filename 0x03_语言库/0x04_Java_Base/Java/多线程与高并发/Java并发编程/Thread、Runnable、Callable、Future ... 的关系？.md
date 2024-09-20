Java多线程<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1670333730081-2a9977cf-f2c5-4afe-8e90-b8c9d2dd274e.png#averageHue=%23fcfbfb&clientId=u1acae143-856c-4&from=paste&id=ubb169888&originHeight=517&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9adec8ec-fd36-4dc0-8264-73345fa3b38&title=)
<a name="A8jsF"></a>
## 1、Thread 和 Runnable
<a name="JJ73z"></a>
### 1.1 Thread
先看一下 Thread 最简单的使用姿势：
```java
public class MyThread extends Thread {
    public MyThread(String name) {
        super(name);
    }
    @Override
    public void run() {
        String name = Thread.currentThread().getName();
        System.out.println(name + "已经运行");

    }
    public static void main(String[] args) {
        new MyThread("线程一").start();
    }
}
```
线程包含 4 个状态：创建 -> 就绪 -> 运行 -> 结束。<br />当执行 `start()` 后，线程进入就绪状态，当对应的线程抢占到 cpu 调度资源之后，进入运行状态，此时调用的是 `run` 方法，执行完毕之后就结束了。
<a name="h8qa9"></a>
### 1.2 Runnable
看一下 Runnable 最简单的使用姿势：
```java
public class MyTask implements Runnable {
    @Override
    public void run() {
        String name = Thread.currentThread().getName();
        System.out.println(name + "已经运行");
    }
    public static void main(String[] args) {
        new Thread(new MyTask(),"线程二").start();
    }
}
```
这里 MyTask 就是一个 Runnable，实现了 `run()` 方法，作为 `Thread()` 的入参。<br />基本所有同学都知道这样使用，但是知道原理么？
<a name="SjJvv"></a>
### 1.3 Thread 和 Runnable 的关系
看一下 Runnable 的接口定义：
```java
public interface Runnable {
    /**
     * When an object implementing interface <code>Runnable</code> is used
     * to create a thread, starting the thread causes the object's
     * <code>run</code> method to be called in that separately executing
     * thread.
     * <p>
     * The general contract of the method <code>run</code> is that it may
     * take any action whatsoever.
     *
     * @see     java.lang.Thread#run()
     */
    public abstract void run();
}
```
英文翻译大致如下：**当一个对象继承并实现了 **`**run()**`** 方法，当线程 **`**start()**`** 后，会在该线程中单独执行该对象的 **`**run()**`** 方法。**<br />这段翻译，基本就告诉了 Runnable 和 Thread 的关系：

1. **MyTask 继承 Runnable，并实现了 **`**run()**`** 方法；**
2. **Thread 初始化，将 MyTask 作为自己的成员变量；**
3. **Thread 执行 **`**run()**`** 方法，线程处于“就绪”状态；**
4. **等待 CPU 调度，执行 Thread 的 **`**run()**`** 方法，但是 **`**run()**`** 的内部实现，其实是执行的 **`**MyTask.run()**`** 方法，线程处于“运行”状态。**

这里面的第2、4步，需要对照着源码看看。<br />在 Thread 初始化时，**MyTask 作为入参 target，最后赋值给 **`**Thread.target**`：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659056406958-8be6d461-9540-44ec-b729-9db9238db6d3.png#averageHue=%232e2c2b&clientId=u737b92c8-379c-4&from=paste&id=ubb1719dc&originHeight=143&originWidth=765&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u919a59a3-51bc-4a8f-87d5-20d387f1fd8&title=)![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659056406972-9f5d0938-92ee-43ff-a8a0-0a14c28baff2.png#averageHue=%232e2c2b&clientId=u737b92c8-379c-4&from=paste&id=uc1dc6947&originHeight=141&originWidth=778&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc093dad0-e04c-461b-b125-5432fd6ec63&title=)<br />当执行 `Thread.run()` 时，**其实是执行的 **`**target.run()**`**，即 **`**MyTask.run()**`**，这个是典型的策略模式**：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659056406890-1b34a124-5461-4824-9dcd-8d16cbd4eebe.png#averageHue=%23302c2a&clientId=u737b92c8-379c-4&from=paste&id=u5d390f29&originHeight=153&originWidth=765&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6592014a-2f66-4f0e-9365-c3dbeb18679&title=)
<a name="n5xmk"></a>
## 2、Callable 、Future 和 FutureTask
先看一下它们的整体关系图谱：<br />![2022-07-29-09-00-45.923803300.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1659056647829-59d5db03-494d-4d34-9e21-c394b934812d.png#averageHue=%23e7edf4&clientId=u737b92c8-379c-4&from=ui&id=u50193664&originHeight=725&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2353596&status=done&style=shadow&taskId=u73e12247-a2f1-4778-b0d0-95c0b6d0b3c&title=)<br />刚开始看到这幅图，感觉 Java 真是麻烦，已经有了 Thread 和 Runnable 这两种创建线程的方式，为啥又搞这 3 个东西呢？<br />其实对于 Thread 和 Runable，其 `run()` 都是无返回值的，并且无法抛出异常，**所以当需要返回多线程的数据，就需要借助 Callable 和 Future。**
<a name="tUcHI"></a>
### 2.1 `Callable`
Callable 是一个接口，里面有个 `V call()` 方法，这个 V 就是返回值类型：
```java
public interface Callable<V> {
    /**
     * Computes a result, or throws an exception if unable to do so.
     *
     * @return computed result
     * @throws Exception if unable to compute a result
     */
    V call() throws Exception;
}
```
一般会用匿名类的方式使用 `Callable`，`call()` 中是具体的业务逻辑：
```java
Callable<String> callable = new Callable<String>() {
    @Override
    public String call() throws Exception {
        // 执行业务逻辑 ...
        return "this is Callable is running";
    }
};
```
这里抛出一个问题，这个 `callable.call()` 和 `Thread.run()` 是什么关系呢？
<a name="chjEE"></a>
### 2.2 `FutureTask`
通过关系图谱，FutureTask 继承了 `RunnableFuture`，`RunnableFuture` 继承了 Runnable 和 Future：
```java
public interface RunnableFuture<V> extends Runnable, Future<V> {
    /**
     * Sets this Future to the result of its computation
     * unless it has been cancelled.
     */
    void run();
}
```
**所以，FutureTask 也是个 Runnable ！！！**<br />这里就有点意思了，既然 FutureTask 是个 Runnable，肯定就需要实现 FutureTask.run() 方法，那么 FutureTask 也可以作为 Thread 的初始化入参，使用姿势如下：
```java
new Thread(FutureTask对象).start();
```
**所以当执行 **`**Thread.run()**`** 时，其实是执行的 **`**FutureTask.run()**`，这个是破解的第一层。<br />下面再破解 `FutureTask.run()` 和 `Callable.call()` 的关系。
<a name="oLAHU"></a>
### 2.3 Callable 和 FutureTask 的关系
FutureTask 初始化时，Callable 必须作为 FutureTask 的初始化入参：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659056406906-9e5f371b-f46b-4b5f-bad5-9775dac2c490.png#averageHue=%23342e2b&clientId=u737b92c8-379c-4&from=paste&id=uda6e023f&originHeight=151&originWidth=802&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc784d851-12e4-4279-8e5e-27271ccddd5&title=)<br />当执行 `FutureTask.run()` 时，其实执行的是 `Callable.call()`：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659056407381-608b2f4b-2b54-47fb-8be3-fa01a2c71787.png#averageHue=%232e2c2b&clientId=u737b92c8-379c-4&from=paste&id=ua563d125&originHeight=718&originWidth=715&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uec2c3528-522e-4260-9520-30b6a73486e&title=)<br />**所以，这里又是一个典型的策略模式 ！！！**<br />现在应该可以很清楚知道 Thread 、Runnable、FutureTask 和 Callable 的关系：

- `**Thread.run()**`** 执行的是 **`**Runnable.run()**`**；**
- **FutureTask 继承了 Runnable，并实现了 **`**FutureTask.run()**`**；**
- `**FutureTask.run()**`** 执行的是 **`**Callable.run()**`**；**
- **依次传递，最后 **`**Thread.run()**`**，其实是执行的 **`**Callable.run()**`**。**

所以整个设计方法，其实就是 2 个策略模式，**Thread 和 Runnable 是一个策略模式，FutureTask 和 Callable 又是一个策略模式，最后通过 Runnable 和 FutureTask 的继承关系，将这 2 个策略模式组合在一起。**<br />是不是把 Future 给忘了~~
<a name="IwKJ5"></a>
### 2.4 Future
为什么要有 Future 呢？再问一个问题，大家可能就知道了。<br />通过 FutureTask，借助 Thread 执行线程后，结果数据怎么获取到呢？这里就需要借助到 Future。<br />看一下 Future 接口：
```java
public interface Future<V> {
	// 取消任务，如果任务正在运行的，mayInterruptIfRunning为true时，表明这个任务会被打断的，并返回true；
	// 为false时，会等待这个任务执行完，返回true；若任务还没执行，取消任务后返回true，如任务执行完，返回false
	boolean cancel(boolean mayInterruptIfRunning);
	// 判断任务是否被取消了,正常执行完不算被取消
	boolean isCancelled();
	// 判断任务是否已经执行完成，任务取消或发生异常也算是完成，返回true
	boolean isDone();
	// 获取任务返回结果，如果任务没有执行完成则等待完成将结果返回，如果获取的过程中发生异常就抛出异常，
	// 比如中断就会抛出InterruptedException异常等异常
	V get() throws InterruptedException, ExecutionException;
	// 在规定的时间如果没有返回结果就会抛出TimeoutException异常
	V get(long timeout, TimeUnit unit) throws InterruptedException, ExecutionException, TimeoutException;
}
```
对于 FutureTask，Callable 就是他的任务，而 FutureTask 内部维护了一个任务状态，所有的状态都是围绕这个任务来进行的，随着任务的进行，状态也在不断的更新。<br />FutureTask 继承了 Future，实现对任务的取消、数据获取、任务状态判断等功能。<br />比如经常会调用 `get()` 方法获取数据，如果任务没有执行完成，会将当前线程放入阻塞队列等待，当任务执行完后，会唤醒阻塞队列中的线程。
<a name="zRY0T"></a>
## 3、具体实例
```java
private static List<String> processByMultiThread(Integer batchSize) throws ExecutionException, InterruptedException {
	List<String> output = new ArrayList<>();

	// 获取分批数据
	List<List<Integer>> batchProcessData = getProcessData(batchSize);

	// 启动线程
	List<FutureTask<List<String>>> futureTaskList = new ArrayList<>();
	for (List<Integer> processData : batchProcessData) {
		Callable<List<String>> callable = () -> processOneThread(processData);
		FutureTask<List<String>> futureTask = new FutureTask<>(callable);
		new Thread(futureTask).start();  // 启动线程
		futureTaskList.add(futureTask);
	}

	// 获取线程返回的数据
	for (FutureTask futureTask : futureTaskList) {
		List<String> processData = (List<String>) futureTask.get();
		output.addAll(processData);
	}
	return output;
}
```
这个示例很简单：

1. **先将数据按照 batchSize 分成 N 批；**
2. **启动 N 个线程，去执行任务；**
3. **通过 **`**futureTask.get()**`** 获取每个线程数据，并汇总输出。**

这个示例其实不太适合线上的场景，因为每次调用都会初始化线程，如果调用过多，内存可能会被撑爆，需要借助线程池。
