Java 线程池
<a name="E0E77"></a>
## 线程池创建的几种方式
<a name="CA4FL"></a>
### `newFixedThreadPool`
定长线程池，每当提交一个任务就创建一个线程，直到达到线程池的最大数量，这时线程数量不再变化，当线程发生错误结束时，线程池会补充一个新的线程<br />测试代码：
```java
public class TestThreadPool {

    //定长线程池，每当提交一个任务就创建一个线程，直到达到线程池的最大数量，这时线程数量不再变化，当线程发生错误结束时，线程池会补充一个新的线程
    static ExecutorService fixedExecutor = Executors.newFixedThreadPool(3);


    public static void main(String[] args) {
        testFixedExecutor();
    }

    //测试定长线程池，线程池的容量为3，提交6个任务，根据打印结果可以看出先执行前3个任务，3个任务结束后再执行后面的任务
    private static void testFixedExecutor() {
        for (int i = 0; i < 6; i++) {
            final int index = i;
            fixedExecutor.execute(new Runnable() {
                public void run() {
                    try {
                        Thread.sleep(3000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    System.out.println(Thread.currentThread().getName() + " index:" + index);
                }
            });
        }

        try {
            Thread.sleep(4000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("4秒后...");

        fixedExecutor.shutdown();
    }

}
```
打印结果：
```java
pool-1-thread-1 index:0
pool-1-thread-2 index:1
pool-1-thread-3 index:2
4秒后...
pool-1-thread-3 index:5
pool-1-thread-1 index:3
pool-1-thread-2 index:4
```
<a name="QLvK1"></a>
### `newCachedThreadPool`
可缓存的线程池，如果线程池的容量超过了任务数，自动回收空闲线程，任务增加时可以自动添加新线程，线程池的容量不限制<br />测试代码：
```java
public class TestThreadPool {

    //可缓存的线程池，如果线程池的容量超过了任务数，自动回收空闲线程，任务增加时可以自动添加新线程，线程池的容量不限制
    static ExecutorService cachedExecutor = Executors.newCachedThreadPool();


    public static void main(String[] args) {
        testCachedExecutor();
    }

    //测试可缓存线程池
    private static void testCachedExecutor() {
        for (int i = 0; i < 6; i++) {
            final int index = i;
            cachedExecutor.execute(new Runnable() {
                public void run() {
                    try {
                        Thread.sleep(3000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    System.out.println(Thread.currentThread().getName() + " index:" + index);
                }
            });
        }

        try {
            Thread.sleep(4000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("4秒后...");

        cachedExecutor.shutdown();
    }

}
```
打印结果：
```java
pool-1-thread-1 index:0
pool-1-thread-6 index:5
pool-1-thread-5 index:4
pool-1-thread-4 index:3
pool-1-thread-3 index:2
pool-1-thread-2 index:1
4秒后...
```
<a name="yRnSo"></a>
### `newScheduledThreadPool` 
定长线程池，可执行周期性的任务<br />测试代码：
```java
public class TestThreadPool {

    //定长线程池，可执行周期性的任务
    static ScheduledExecutorService scheduledExecutor = Executors.newScheduledThreadPool(3);


    public static void main(String[] args) {
        testScheduledExecutor();
    }

    //测试定长、可周期执行的线程池
    private static void testScheduledExecutor() {
        for (int i = 0; i < 3; i++) {
            final int index = i;
            //scheduleWithFixedDelay 固定的延迟时间执行任务；scheduleAtFixedRate 固定的频率执行任务
            scheduledExecutor.scheduleWithFixedDelay(new Runnable() {
                public void run() {
                    System.out.println(Thread.currentThread().getName() + " index:" + index);
                }
            }, 0, 3, TimeUnit.SECONDS);
        }

        try {
            Thread.sleep(4000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("4秒后...");

        scheduledExecutor.shutdown();
    }

}
```
打印结果：
```java
pool-1-thread-1 index:0
pool-1-thread-2 index:1
pool-1-thread-3 index:2
pool-1-thread-1 index:0
pool-1-thread-3 index:1
pool-1-thread-1 index:2
4秒后...
```
<a name="xWi8v"></a>
### `newSingleThreadExecutor`
单线程的线程池，线程异常结束，会创建一个新的线程，能确保任务按提交顺序执行<br />测试代码：
```java
public class TestThreadPool {

    //单线程的线程池，线程异常结束，会创建一个新的线程，能确保任务按提交顺序执行
    static ExecutorService singleExecutor = Executors.newSingleThreadExecutor();


    public static void main(String[] args) {
        testSingleExecutor();
    }

    //测试单线程的线程池
    private static void testSingleExecutor() {
        for (int i = 0; i < 3; i++) {
            final int index = i;
            singleExecutor.execute(new Runnable() {
                public void run() {
                    try {
                        Thread.sleep(3000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    System.out.println(Thread.currentThread().getName() + " index:" + index);
                }
            });
        }

        try {
            Thread.sleep(4000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("4秒后...");

        singleExecutor.shutdown();
    }

}
```
打印结果：
```java
pool-1-thread-1 index:0
4秒后...
pool-1-thread-1 index:1
pool-1-thread-1 index:2
```
<a name="DwKNC"></a>
### `newSingleThreadScheduledExecutor`
单线程可执行周期性任务的线程池<br />测试代码：
```java
public class TestThreadPool {

    //单线程可执行周期性任务的线程池
    static ScheduledExecutorService singleScheduledExecutor = Executors.newSingleThreadScheduledExecutor();


    public static void main(String[] args) {
        testSingleScheduledExecutor();
    }

    //测试单线程可周期执行的线程池
    private static void testSingleScheduledExecutor() {
        for (int i = 0; i < 3; i++) {
            final int index = i;
            //scheduleWithFixedDelay 固定的延迟时间执行任务；scheduleAtFixedRate 固定的频率执行任务
            singleScheduledExecutor.scheduleAtFixedRate(new Runnable() {
                public void run() {
                    System.out.println(Thread.currentThread().getName() + " index:" + index);
                }
            }, 0, 3, TimeUnit.SECONDS);
        }

        try {
            Thread.sleep(4000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("4秒后...");

        singleScheduledExecutor.shutdown();
    }

}
```
打印结果：
```java
pool-1-thread-1 index:0
pool-1-thread-1 index:1
pool-1-thread-1 index:2
pool-1-thread-1 index:0
pool-1-thread-1 index:1
pool-1-thread-1 index:2
4秒后...
```
<a name="wCEwm"></a>
### `newWorkStealingPool`
任务窃取线程池，不保证执行顺序，适合任务耗时差异较大。<br />线程池中有多个线程队列，有的线程队列中有大量的比较耗时的任务堆积，而有的线程队列却是空的，就存在有的线程处于饥饿状态，当一个线程处于饥饿状态时，它就会去其它的线程队列中窃取任务。解决饥饿导致的效率问题。<br />默认创建的并行 level 是 CPU 的核数。主线程结束，即使线程池有任务也会立即停止。<br />测试代码：
```java
public class TestThreadPool {

    //任务窃取线程池
    static ExecutorService workStealingExecutor = Executors.newWorkStealingPool();

    public static void main(String[] args) {
        testWorkStealingExecutor();
    }

    //测试任务窃取线程池
    private static void testWorkStealingExecutor() {
        for (int i = 0; i < 10; i++) {//本机 CPU 8核，这里创建10个任务进行测试
            final int index = i;
            workStealingExecutor.execute(new Runnable() {
                public void run() {
                    try {
                        Thread.sleep(3000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    System.out.println(Thread.currentThread().getName() + " index:" + index);
                }
            });
        }

        try {
            Thread.sleep(4000);//这里主线程不休眠，不会有打印输出
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("4秒后...");
        //  workStealingExecutor.shutdown();
    }

}
```
打印结果如下，index:8，index:9并未打印出：
```java
ForkJoinPool-1-worker-1 index:0
ForkJoinPool-1-worker-7 index:6
ForkJoinPool-1-worker-5 index:4
ForkJoinPool-1-worker-3 index:2
ForkJoinPool-1-worker-4 index:3
ForkJoinPool-1-worker-2 index:1
ForkJoinPool-1-worker-0 index:7
ForkJoinPool-1-worker-6 index:5
4秒后...
```
<a name="uRL2X"></a>
## 线程池的工作流程

1. 线程池刚创建时，里面没有一个线程。任务队列是作为参数传进来的。不过，就算队列里面有任务，线程池也不会马上执行他们。
2. 当调用`execute()`方法添加一个任务时，线程池会做如下判断：<br />a. 如果正在运行的线程数量小于`corePoolSize`，那么马上创建线程运行这个任务<br />b. 如果正在运行的线程数量大于或等于`corePoolSize`，那么将这个任务放入队列<br />c. 如果这时候队列满了，而且正在运行的线程数量小于`maximunPoolSize`，那么还是要创建非核心线程立刻运行这个任务<br />d. 如果队列满了，而且正在运行的线程数量大于或等于`maximunPoolSize`，那么线程池会抛出`RejectedExecutionException`
3. 当一个线程完成任务时，它会从队列中取下一个任务来执行
4. 当一个线程无事可做，超过一定的时间（keepAliveTime）时，线程池会判断，如果当前运行的线程数大于`corePoolSize`，那么这个线程就被停掉。所以线程池的所有任务完成后，它最终会收缩到`corePoolSize`的大小

可以用如下图来表示整体流程<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600243882013-da1aa272-d29a-4840-a3d5-9ba92d6e1fee.png#height=336&id=jcvBx&originHeight=1009&originWidth=2282&originalType=binary&ratio=1&size=174956&status=done&style=shadow&width=760.6666666666666)

