Java
<a name="VMmrF"></a>
## 基础线程机制
<a name="pKmQe"></a>
### Executor
管理多个异步任务的执行，而无需程序员显式地管理线程的生命周期。这里的异步是指多个任务的执行互不干扰，不需要进行同步操作。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1655995189992-4f131e94-b695-477e-8d91-8cf5b9095620.png#clientId=u29262091-af46-4&from=paste&id=uf0a0f003&originHeight=376&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ufc836837-aed7-4441-ad12-0c5ec9e6046&title=)
<a name="FOGp1"></a>
### Daemon
守护线程是程序运行时在后台提供服务的线程，不属于程序中不可或缺的部分。<br />当所有非守护线程结束时，程序也就终止，同时会杀死所有守护线程。<br />`main()` 属于非守护线程。<br />使用 `setDaemon()` 方法将一个线程设置为守护线程。
```java
public  static  void main(String[] args) {
    Thread thread =  new Thread(new MyRunnable());
    thread.setDaemon(true);
}
```
<a name="VBMbx"></a>
### `sleep()`
`Thread.sleep(millisec)` 方法会休眠当前正在执行的线程，millisec 单位为毫秒。<br />`sleep()` 可能会抛出 `InterruptedException`，因为异常不能跨线程传播回 `main()` 中，因此必须在本地进行处理。线程中抛出的其它异常也同样需要在本地进行处理。
```java
public void run() {
    try {
        Thread.sleep(3000);
    } catch (InterruptedException e) {
        e.printStackTrace();
    }
}
```
<a name="KgDZH"></a>
### `yield()`
对静态方法 `Thread.yield()` 的调用声明了当前线程已经完成了生命周期中最重要的部分，可以切换给其它线程来执行。该方法只是对线程调度器的一个建议，而且也只是建议具有相同优先级的其它线程可以运行。
```java
public void run() {
    Thread.yield();
}
```
<a name="ccftL"></a>
## 中断
一个线程执行完毕之后会自动结束，如果在运行过程中发生异常也会提前结束。
<a name="YEYu5"></a>
### `InterruptedException`
通过调用一个线程的 `interrupt()` 来中断该线程，如果该线程处于阻塞、限期等待或者无限期等待状态，那么就会抛出 `InterruptedException`，从而提前结束该线程。但是不能中断 I/O 阻塞和 `synchronized` 锁阻塞。<br />对于以下代码，在 `main()` 中启动一个线程之后再中断它，由于线程中调用了 `Thread.sleep()` 方法，因此会抛出一个 `InterruptedException`，从而提前结束线程，不执行之后的语句。
```java
public class InterruptExample {

    private static class MyThread1 extends Thread {
        @Override
        public void run() {
            try{
                Thread.sleep(2000);
                System.out.println("Thread run");
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}

public static void main(String[] args) throws InterruptedException {
    Thread thread1 = new MyThread1();
    thread1.start();
    thread1.interrupt();
    System.out.println("Main run");
}

//结果
Main run
java.lang.InterruptedException: sleep interrupted
    at java.lang.Thread.sleep(Native Method)
    at InterruptExample.lambda$main$0(InterruptExample.java:5)
    at InterruptExample$$Lambda$1/713338599.run(Unknown Source)
    at java.lang.Thread.run(Thread.java:745)
```
<a name="kYqij"></a>
### `interrupted()`
如果一个线程的 `run()` 方法执行一个无限循环，并且没有执行 `sleep()` 等会抛出 `InterruptedException` 的操作，那么调用线程的 `interrupt()` 方法就无法使线程提前结束。<br />但是调用 `interrupt()` 方法会设置线程的中断标记，此时调用 `interrupted()` 方法会返回 true。因此可以在循环体中使用 `interrupted()` 方法来判断线程是否处于中断状态，从而提前结束线程。
```java
public class InterruptExample {
    private static class MyThread2 extends Thread {
         @Override
        public void run() {
             while (!interrupted()) {
                  // ..
            }
            System.out.println("Thread end");
        }
    }
}

public static void main(String[] args) throws InterruptedException {
    Thread thread2 = new MyThread2();
    thread2.start();
    thread2.interrupt();
}

//结果
Thread end
```
<a name="qHc8l"></a>
### Executor 的中断操作
调用 Executor 的 `shutdown()` 方法会等待线程都执行完毕之后再关闭，但是如果调用的是 `shutdownNow()` 方法，则相当于调用每个线程的 `interrupt()` 方法。<br />以下使用 Lambda 创建线程，相当于创建了一个匿名内部线程。
```java
public static void main(String[] args) {
    ExecutorService executorService = Executors.newCachedThreadPool();
    executorService.execute(() -> {
        try {
            Thread.sleep(2000);
            System.out.println("Thread run");
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    });
    executorService.shutdownNow();
    System.out.println("Main run");
}

//结果
Main run
java.lang.InterruptedException: sleep interrupted
    at java.lang.Thread.sleep(Native Method)
    at ExecutorInterruptExample.lambda$main$0(ExecutorInterruptExample.java:9)
    at ExecutorInterruptExample$$Lambda$1/1160460865.run(Unknown Source)
    at java.util.concurrent.ThreadPoolExecutor.runWorker(ThreadPoolExecutor.java:1142)
    at java.util.concurrent.ThreadPoolExecutor$Worker.run(ThreadPoolExecutor.java:617)
    at java.lang.Thread.run(Thread.java:745)
```
如果只想中断 Executor 中的一个线程，可以通过使用 `submit()` 方法来提交一个线程，它会返回一个 `Future<?>` 对象，通过调用该对象的 `cancel(true)` 方法就可以中断线程。
```java
Future<?> future = executorService.submit(() -> {
    // ..
});
future.cancel(true);
```
