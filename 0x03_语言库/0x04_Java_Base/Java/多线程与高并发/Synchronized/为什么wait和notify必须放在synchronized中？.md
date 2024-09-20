Java <br />在多线程编程中，wait 方法是让当前线程进入休眠状态，直到另一个线程调用了 notify 或 notifyAll 方法之后，才能继续恢复执行。而在 Java 中，wait 和 notify/notifyAll 有着一套自己的使用格式要求，也就是在使用 wait 和 notify（notifyAll 的使用和 notify 类似，所以下文就只用 notify 用来指代二者）必须配合 synchronized 一起使用才行。
<a name="xBEqJ"></a>
## wait/notify基础使用
wait 和 notify 的基础方法如下：
```java
Object lock = new Object();
new Thread(() -> {
    synchronized (lock) {
        try {
            System.out.println("wait 之前");
            // 调用 wait 方法
            lock.wait();
            System.out.println("wait 之后");
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}).start();

Thread.sleep(100);
synchronized (lock) {
    System.out.println("执行 notify");
    // 调用 notify 方法
    lock.notify();
}
```
以上代码的执行结果如下图所示：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1646035179233-8688d229-4f60-4eef-9f93-2a53fd7a5c57.png#clientId=ubd5dd27a-7b1f-4&from=paste&id=uedbe5eb1&originHeight=267&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua25309c6-4878-4b06-ad66-991dcb38abc&title=)
<a name="daPRU"></a>
## wait/notify和synchronized一起用？
那问题来了，是不是 wait 和 notify 一定要配合 synchronized 一起使用呢？wait 和 notify 单独使用行不行呢？尝试将以上代码中的 synchronized 代码行删除，实现代码如下：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1646035179341-63333fa2-c37c-4f94-b961-f8518418c26b.png#clientId=ubd5dd27a-7b1f-4&from=paste&id=uf8e57dac&originHeight=874&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uededc035-56f3-4eaa-b5e7-dd995b406e2&title=)初看代码好像没啥问题，编译器也没报错，好像能“正常使用”，然而当运行以上程序时就会发生如下错误：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1646035179259-fcc378f4-8c7b-4056-b2bc-d81b00693b4b.png#clientId=ubd5dd27a-7b1f-4&from=paste&id=u9eb7e13f&originHeight=423&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u02b14dbb-a2ae-47d9-a679-b051c525991&title=)从上述结果可以看出：**无论是 wait 还是 notify，如果不配合 synchronized 一起使用，在程序运行时就会报 IllegalMonitorStateException 非法的监视器状态异常，而且 notify 也不能实现程序的唤醒功能了。**
<a name="he6V1"></a>
## 原因分析
从上述的报错信息可以看出，JVM 在运行时会强制检查 wait 和 notify 有没有在 synchronized 代码中，如果没有的话就会报非法监视器状态异常（IllegalMonitorStateException），但这也仅仅是运行时的程序表象，那为什么 Java 要这样设计呢？其实这样设计的原因就是为了防止多线程并发运行时，程序的执行混乱问题。初看这句话，好像是用来描述“锁”的。然而实际情况也是如此，wait 和 notify 引入锁就是来规避并发执行时程序的执行混乱问题的。那这个“执行混乱问题”到底是啥呢？接下来继续往下看。
<a name="yC7QA"></a>
## wait和notify问题复现
假设 wait 和 notify 可以不加锁，用它们来实现一个自定义阻塞队列。这里的阻塞队列是指读操作阻塞，也就是当读取数据时，如果有数据就返回数据，如果没有数据则阻塞等待数据，实现代码如下：
```java
class MyBlockingQueue {
    // 用来保存数据的集合
    Queue<String> queue = new LinkedList<>();

    /**
     * 添加方法
     */
    public void put(String data) {
        // 队列加入数据
        queue.add(data); 
        // 唤醒线程继续执行（这里的线程指的是执行 take 方法的线程）
        notify(); // ③
    }

    /**
     * 获取方法（阻塞式执行）
     * 如果队列里面有数据则返回数据，如果没有数据就阻塞等待数据
     * @return
     */
    public String take() throws InterruptedException {
        // 使用 while 判断是否有数据（这里使用 while 而非 if 是为了防止虚假唤醒）
        while (queue.isEmpty()) { // ①  
            // 没有任务，先阻塞等待
            wait(); // ②
        }
        return queue.remove(); // 返回数据
    }
}
```
注意上述代码，在代码中标识了三个关键执行步骤：<br />①：判断队列中是否有数据；<br />②：执行 wait 休眠操作；<br />③：给队列中添加数据并唤醒阻塞线程。如果不强制要求添加 synchronized，那么就会出现如下问题：

| 步骤 | 线程1 | 线程2 |
| --- | --- | --- |
| 1 | 执行步骤 ① 判断当前队列中没有数据 | 

 |
| 2 | 

 | 执行步骤 ③ 将数据添加到队列，并唤醒线程1继续执行 |
| 3 | 执行步骤 ② 线程 1 进入休眠状态 | 

 |

从上述执行流程看出问题了吗？**如果 wait 和 notify 不强制要求加锁，那么在线程 1 执行完判断之后，尚未执行休眠之前，此时另一个线程添加数据到队列中。然而这时线程 1 已经执行过判断了，所以就会直接进入休眠状态，从而导致队列中的那条数据永久性不能被读取，这就是程序并发运行时“执行结果混乱”的问题。**然而如果配合 synchronized 一起使用的话，代码就会变成以下这样：
```java
class MyBlockingQueue {
    // 用来保存任务的集合
    Queue<String> queue = new LinkedList<>();
    
    /**
    * 添加方法
    */
    public void put(String data) {
        synchronized (MyBlockingQueue.class) {
            // 队列加入数据
            queue.add(data);
            // 为了防止 take 方法阻塞休眠，这里需要调用唤醒方法 notify
            notify(); // ③
        }
    }
    
    /**
    * 获取方法（阻塞式执行）
    * 如果队列里面有数据则返回数据，如果没有数据就阻塞等待数据
    * @return
    */
    public String take() throws InterruptedException {
        synchronized (MyBlockingQueue.class) {
            // 使用 while 判断是否有数据（这里使用 while 而非 if 是为了防止虚假唤醒）
            while (queue.isEmpty()) {  // ①
                // 没有任务，先阻塞等待
                wait(); // ②
            }
        }
        return queue.remove(); // 返回数据
    }
}
```
这样改造之后，关键步骤 ① 和关键步骤 ② 就可以一起执行了，从而当线程执行了步骤 ③ 之后，线程 1 就可以读取到队列中的那条数据了，它们的执行流程如下：

| 步骤 | 线程1 | 线程2 |
| --- | --- | --- |
| 1 | 执行步骤 ① 判断当前队列没有数据 | <br /> |
| 2 | 执行步骤 ② 线程进入休眠状态 | <br /> |
| 3 | <br /> | 执行步骤 ③ 将数据添加到队列，并执行唤醒操作 |
| 4 | 线程被唤醒，继续执行 | <br /> |
| 5 | 判断队列中有数据，返回数据 | <br /> |

这样咱们的程序就可以正常执行了，这就是为什么 Java 设计一定要让 wait 和 notify 配合上 synchronized 一起使用的原因了。
<a name="eHhY2"></a>
## 总结
本文介绍了 wait 和 notify 的基础使用，以及为什么 wait 和 notify/notifyAll 一定要配合 synchronized 使用的原因。如果 wait 和 notify/notifyAll 不强制和 synchronized 一起使用，那么在多线程执行时，就会出现 wait 执行了一半，然后又执行了添加数据和 notify 的操作，从而导致线程一直休眠的缺陷。
