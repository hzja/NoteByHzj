Java
<a name="b2yqo"></a>
### **简介**
创建线程的方式有两种，一种是实现`Runnable`接口，另一种是继承Thread，但是这两种方式都有个缺点，那就是在任务执行完成之后无法获取返回结果。<br />那如果想要获取返回结果该如何实现呢？<br />`Executor`框架结构中提到的`Callable`接口和`Future`接口，从Java SE 5.0开始引入了`Callable`和`Future`，通过它们构建的线程，在任务执行完成后就可以获取执行结果。
<a name="PtmXi"></a>
### **1、**`**Runnable**`**接口与**`**Callable**`**接口比较**
<a name="aar8P"></a>
#### Runnable：
```java
public interface Runnable {  
    public abstract void run();  
}
```
<a name="zP2oM"></a>
#### Callable：
```java
public interface Callable<V> {   
	V call() throws Exception;   
}
```
<a name="dwfNW"></a>
### **Callable介绍**
该接口声明了一个名称为`call()`的方法，同时这个方法可以有返回值V，也可以抛出异常。下面来说明如何使用，无论是Runnable接口的实现类还是Callable接口的实现类，都可以被`ThreadPoolExecutor`或`ScheduledThreadPoolExecutor`执行，`ThreadPoolExecutor`或`ScheduledThreadPoolExecutor`都实现了`ExcutorService`接口，而因此`Callable`需要和`Executor`框架中的`ExcutorService`结合使用，先看看ExecutorService提供的方法：
```java
<T> Future<T> submit(Callable<T> task);  //submit提交一个实现Callable接口的任务，并且返回封装了异步计算结果的Future。

<T> Future<T> submit(Runnable task, T result);  //submit提交一个实现Runnable接口的任务，并且指定了在调用Future的get方法时返回的result对象。

Future<?> submit(Runnable task); //submit提交一个实现Runnable接口的任务，并且返回封装了异步计算结果的Future。
```
<a name="MVgyE"></a>
### **其他方法**
还有点要注意的是，除了自己实现`Callable`对象外，还可以使用工厂类`Executors`来把一个`Runnable`对象包装成`Callable`对象。`Executors`工厂类提供的方法如下：
```java
public static Callable<Object> callable(Runnable task)  
public static <T> Callable<T> callable(Runnable task, T result) 
```
<a name="XU9z3"></a>
### **2、**`**Future<V>**`**接口**
`Future<V>`接口是用来获取异步计算结果的，说白了就是对具体的`Runnable`或者`Callable`对象任务执行的结果进行获取(`get()`)，取消(`cancel()`)，判断是否完成等操作。
```java
public interface Future<V> {  
	boolean cancel(boolean mayInterruptIfRunning);  //如果任务还没开始，执行cancel(...)方法将返回false；如果任务已经启动，执行cancel(true)方法将以中断执行此任务线程的方式来试图停止任务，如果停止成功，返回true；当任务已经启动，执行cancel(false)方法将不会对正在执行的任务线程产生影响(让线程正常执行到完成)，此时返回false；当任务已经完成，执行cancel(...)方法将返回false。mayInterruptRunning参数表示是否中断执行中的线程。

	boolean isCancelled();  //如果任务完成前被取消，则返回true。

	boolean isDone();  //如果任务执行结束，无论是正常结束或是中途取消还是发生异常，都返回true

	V get() throws InterruptedException, ExecutionException;  //获取异步执行的结果，如果没有结果可用，此方法会阻塞直到异步计算完成。

	V get(long timeout, TimeUnit unit) throws InterruptedException, ExecutionException, TimeoutException;  //获取异步执行结果，如果没有结果可用，此方法会阻塞，但是会有时间限制，如果阻塞时间超过设定的timeout时间，该方法将返回null。
}
```
<a name="EJYDF"></a>
##### 总结：
实际上`Future`提供了3种功能：

- **能够中断执行中的任务;**
- **判断任务是否执行完成;**
- **获取任务执行完成后额结果;**

但是`Future`只是一个接口，如果想要使用的话需要使用它的实现类`FutureTask`。
<a name="kZXOT"></a>
### **3、**`**FutureTask**`
```java
public class FutureTask<V> implements RunnableFuture<V> {
}

public interface RunnableFuture<V> extends Runnable, Future<V> {  
	void run();  
} 
```
<a name="a17vw"></a>
##### 分析：
`FutureTask`除了实现了Future接口外还实现了`Runnable`接口，因此`FutureTask`也可以直接提交给`Executor`执行。当然也可以调用线程直接执行（`FutureTask.run()`）。接下来根据`FutureTask.run()`的执行时机来分析其所处的3种状态：

- **未启动，**`**FutureTask.run()**`**方法还没有被执行之前，**`**FutureTask**`**处于未启动状态，当创建一个**`**FutureTask**`**，而且没有执行**`**FutureTask.run()**`**方法前，这个**`**FutureTask**`**也处于未启动状态。**
- **已启动，**`**FutureTask.run()**`**被执行的过程中，**`**FutureTask**`**处于已启动状态。**
- **已完成，**`**FutureTask.run()**`**方法执行完正常结束，或者被取消或者抛出异常而结束，**`**FutureTask**`**都处于完成状态。**
<a name="UN44E"></a>
### **执行状态图**
![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1659531116059-886dd251-5a01-4e94-837b-1e5d3ade2326.jpeg)
<a name="e0MJN"></a>
### **方法执行示意图**
![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1659531045493-2334bb71-5cc0-41cd-b30f-f7587b438b28.jpeg)
<a name="DvqwR"></a>
##### 分析：
（1）当`FutureTask`处于未启动或已启动状态时，如果此时执行`FutureTask.get()`方法将导致调用线程阻塞；当`FutureTask`处于已完成状态时，执行`FutureTask.get()`方法将导致调用线程立即返回结果或者抛出异常。<br />（2）当`FutureTask`处于未启动状态时，执行`FutureTask.cancel()`方法将导致此任务永远不会执行。<br />当`FutureTask`处于已启动状态时，执行`cancel(true)`方法将以中断执行此任务线程的方式来试图停止任务，如果任务取消成功，`cancel(...)`返回true；但如果执行`cancel(false)`方法将不会对正在执行的任务线程产生影响(让线程正常执行到完成)，此时`cancel(...)`返回false。<br />当任务已经完成，执行`cancel(...)`方法将返回false。<br />`FutureTask`的两种构造函数：
```java
public FutureTask(Callable<V> callable) {  
}  
public FutureTask(Runnable runnable, V result) {  
}
```
<a name="YlnWg"></a>
### **Callable+FutureTask获取执行结果**
```java
public class ThreadDemo {
    public static void main(String[] args) {

        //创建线程池
        ExecutorService es = Executors.newSingleThreadExecutor();
        //创建Callable对象任务
        Callable calTask=new Callable() {
            public String call() throws Exception {
                String str = "返回值";
                return str;
            }
        };
        //创建FutureTask
        FutureTask<Integer> futureTask=new FutureTask(calTask);
        //执行任务
        es.submit(futureTask);
        //关闭线程池
        es.shutdown();
        try {
            Thread.sleep(2000);
            System.out.println("主线程在执行其他任务");

            if(futureTask.get()!=null){
                //输出获取到的结果
                System.out.println("futureTask.get()-->"+futureTask.get());
            }else{
                //输出获取到的结果
                System.out.println("futureTask.get()未获取到结果");
            }

        } catch (Exception e) {
            e.printStackTrace();
        }
        System.out.println("主线程在执行完成");
    }
}
```
