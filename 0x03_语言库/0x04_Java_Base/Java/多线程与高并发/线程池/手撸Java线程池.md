Java线程池<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682000713738-8f3b70d4-f320-405a-9a52-1ca1450e281d.png#averageHue=%23f8f6f5&clientId=uf7f882e7-77dd-4&from=paste&id=u69efe395&originHeight=675&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u781e3965-1a5f-430c-8a87-5cc9b0a02b8&title=)
<a name="Oqm0M"></a>
## Java线程池核心原理
看过Java线程池源码的小伙伴都知道，在Java线程池中最核心的类就是`ThreadPoolExecutor`，而在`ThreadPoolExecutor`类中最核心的构造方法就是带有7个参数的构造方法，如下所示。
```java
public ThreadPoolExecutor(int corePoolSize,
                          int maximumPoolSize,
                          long keepAliveTime,
                          TimeUnit unit,
                          BlockingQueue<Runnable> workQueue,
                          ThreadFactory threadFactory,
                          RejectedExecutionHandler handler)
```
各参数的含义如下所示。

- `corePoolSize`：线程池中的常驻核心线程数。
- `maximumPoolSize`：线程池能够容纳同时执行的最大线程数，此值大于等于1。
- `keepAliveTime`：多余的空闲线程存活时间，当空间时间达到`keepAliveTime`值时，多余的线程会被销毁直到只剩下`corePoolSize`个线程为止。
- `unit`：`keepAliveTime`的单位。
- `workQueue`：任务队列，被提交但尚未被执行的任务。
- `threadFactory`：表示生成线程池中工作线程的线程工厂，用户创建新线程，一般用默认即可。
- `handler`：拒绝策略，表示当线程队列满了并且工作线程大于等于线程池的最大显示数(`maxnumPoolSize`)时，如何来拒绝请求执行的`runnable`的策略。

并且Java的线程池是通过 **生产者-消费者模式** 实现的，线程池的使用方是生产者，而线程池本身就是消费者。<br />Java线程池的核心工作流程如下图所示。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682000713769-48a1573c-3c25-4f13-9c97-b86aacbc9047.png#averageHue=%23fafafa&clientId=uf7f882e7-77dd-4&from=paste&id=u97a833f5&originHeight=719&originWidth=977&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc5c28a7c-a4eb-4d70-a1ff-b3b608aa73c&title=)
<a name="ZkDEU"></a>
## 手撸Java线程池
自己手动实现的线程池要比Java自身的线程池简单的多，去掉了各种复杂的处理方式，只保留了最核心的原理：线程池的使用者向任务队列中添加任务，而线程池本身从任务队列中消费任务并执行任务。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682000713784-b5703a17-bf7e-479a-ae2f-8935d5d5938a.png#averageHue=%23f7f7f7&clientId=uf7f882e7-77dd-4&from=paste&id=u2e65598e&originHeight=216&originWidth=845&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u555d9086-5ed4-4f36-b50e-1084fed9f19&title=)<br />只要理解了这个核心原理，接下来的代码就简单多了。在实现这个简单的线程池时，可以将整个实现过程进行拆解。拆解后的实现流程为：定义核心字段、创建内部类`WorkThread`、创建`ThreadPool`类的构造方法和创建执行任务的方法。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682000713853-7334d153-eac3-46a2-8fa3-ad9a2f2228be.png#averageHue=%23f7f6f4&clientId=uf7f882e7-77dd-4&from=paste&id=u5ca3463d&originHeight=590&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5e7b6db9-9db2-4274-b9df-d2526a0b8bc&title=)

<a name="kSqZ1"></a>
### 定义核心字段
首先，创建一个名称为ThreadPool的Java类，并在这个类中定义如下核心字段。

- `DEFAULT_WORKQUEUE_SIZE`：静态常量，表示默认的阻塞队列大小。
- `workQueue`：模拟实际的线程池使用阻塞队列来实现生产者-消费者模式。
- `workThreads`：模拟实际的线程池使用List集合保存线程池内部的工作线程。

核心代码如下所示。
```java
//默认阻塞队列大小
private static final int DEFAULT_WORKQUEUE_SIZE = 5;

//模拟实际的线程池使用阻塞队列来实现生产者-消费者模式
private BlockingQueue<Runnable> workQueue;

//模拟实际的线程池使用List集合保存线程池内部的工作线程
private List<WorkThread> workThreads = new ArrayList<WorkThread>();
```
<a name="zwrr9"></a>
### 创建内部类`WordThread`
在`ThreadPool`类中创建一个内部类`WorkThread`，模拟线程池中的工作线程。主要的作用就是消费`workQueue`中的任务，并执行任务。由于工作线程需要不断从`workQueue`中获取任务，所以，这里使用了`while(true)`循环不断尝试消费队列中的任务。<br />核心代码如下所示。
```java
//内部类WorkThread，模拟线程池中的工作线程
//主要的作用就是消费workQueue中的任务，并执行
//由于工作线程需要不断从workQueue中获取任务，使用了while(true)循环不断尝试消费队列中的任务
class WorkThread extends Thread{
    @Override
    public void run() {
        //不断循环获取队列中的任务
        while (true){
            //当没有任务时，会阻塞
            try {
                Runnable workTask = workQueue.take();
                workTask.run();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
```
<a name="UmCBG"></a>
### 创建ThreadPool类的构造方法
这里为ThreadPool类创建两个构造方法，一个构造方法中传入线程池的容量大小和阻塞队列，另一个构造方法中只传入线程池的容量大小。<br />核心代码如下所示。
```java
//在ThreadPool的构造方法中传入线程池的大小和阻塞队列
public ThreadPool(int poolSize, BlockingQueue<Runnable> workQueue){
    this.workQueue = workQueue;
    //创建poolSize个工作线程并将其加入到workThreads集合中
    IntStream.range(0, poolSize).forEach((i) -> {
        WorkThread workThread = new WorkThread();
        workThread.start();
        workThreads.add(workThread);
    });
}

//在ThreadPool的构造方法中传入线程池的大小
public ThreadPool(int poolSize){
    this(poolSize, new LinkedBlockingQueue<>(DEFAULT_WORKQUEUE_SIZE));
}
```
<a name="H9ROJ"></a>
### 创建执行任务的方法
在`ThreadPool`类中创建执行任务的方法`execute()`，`execute()`方法的实现比较简单，就是将方法接收到的`Runnable`任务加入到`workQueue`队列中。<br />核心代码如下所示。
```java
//通过线程池执行任务
public void execute(Runnable task){
    try {
        workQueue.put(task);
    } catch (InterruptedException e) {
        e.printStackTrace();
    }
}
```
<a name="BABSk"></a>
### 完整源码
这里给出手动实现的ThreadPool线程池的完整源代码，如下所示。
```java
package io.binghe.thread.pool;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.stream.IntStream;

/**
 * @description 自定义线程池
 */
public class ThreadPool {

    //默认阻塞队列大小
    private static final int DEFAULT_WORKQUEUE_SIZE = 5;

    //模拟实际的线程池使用阻塞队列来实现生产者-消费者模式
    private BlockingQueue<Runnable> workQueue;

    //模拟实际的线程池使用List集合保存线程池内部的工作线程
    private List<WorkThread> workThreads = new ArrayList<WorkThread>();

    //在ThreadPool的构造方法中传入线程池的大小和阻塞队列
    public ThreadPool(int poolSize, BlockingQueue<Runnable> workQueue){
        this.workQueue = workQueue;
        //创建poolSize个工作线程并将其加入到workThreads集合中
        IntStream.range(0, poolSize).forEach((i) -> {
            WorkThread workThread = new WorkThread();
            workThread.start();
            workThreads.add(workThread);
        });
    }

    //在ThreadPool的构造方法中传入线程池的大小
    public ThreadPool(int poolSize){
        this(poolSize, new LinkedBlockingQueue<>(DEFAULT_WORKQUEUE_SIZE));
    }

    //通过线程池执行任务
    public void execute(Runnable task){
        try {
            workQueue.put(task);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    //内部类WorkThread，模拟线程池中的工作线程
    //主要的作用就是消费workQueue中的任务，并执行
    //由于工作线程需要不断从workQueue中获取任务，使用了while(true)循环不断尝试消费队列中的任务
    class WorkThread extends Thread{
        @Override
        public void run() {
            //不断循环获取队列中的任务
            while (true){
                //当没有任务时，会阻塞
                try {
                    Runnable workTask = workQueue.take();
                    workTask.run();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }
}
```
没错，仅仅用了几十行Java代码就实现了一个极简版的Java线程池，没错，这个极简版的Java线程池的代码却体现了Java线程池的核心原理。<br />接下来，测试下这个极简版的Java线程池。
<a name="AJJnU"></a>
## 编写测试程序
测试程序也比较简单，就是通过在`main()`方法中调用`ThreadPool`类的构造方法，传入线程池的大小，创建一个ThreadPool类的实例，然后循环10次调用`ThreadPool`类的`execute()`方法，向线程池中提交的任务为：打印当前线程的名称--->> Hello ThreadPool。<br />整体测试代码如下所示。
```java
package io.binghe.thread.pool.test;

import io.binghe.thread.pool.ThreadPool;
import java.util.stream.IntStream;

/**
 * @description 测试自定义线程池
 */
public class ThreadPoolTest {

    public static void main(String[] args){
        ThreadPool threadPool = new ThreadPool(10);
        IntStream.range(0, 10).forEach((i) -> {
            threadPool.execute(() -> {
                System.out.println(Thread.currentThread().getName() + "--->> Hello ThreadPool");
            });
        });
    }
}
```
接下来，运行`ThreadPoolTest`类的`main()`方法，会输出如下信息。
```
Thread-0--->> Hello ThreadPool
Thread-9--->> Hello ThreadPool
Thread-5--->> Hello ThreadPool
Thread-8--->> Hello ThreadPool
Thread-4--->> Hello ThreadPool
Thread-1--->> Hello ThreadPool
Thread-2--->> Hello ThreadPool
Thread-5--->> Hello ThreadPool
Thread-9--->> Hello ThreadPool
Thread-0--->> Hello ThreadPool
```
至此，自定义的Java线程池就开发完成了。
<a name="EP11I"></a>
## 总结
线程池的核心原理其实并不复杂，只要耐心的分析，深入其源码理解线程池的核心本质，就会发现线程池的设计原来是如此的优雅。通过这个手写线程池的小例子，能够更好的理解线程池的核心原理。
