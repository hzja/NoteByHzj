像stop、suspend这几种中断或者阻塞线程的方法在较高Java版本中已经被标记上了`@Deprecated`过期标签，那么为什么她们曾经登上了Java的历史舞台而又渐渐的推出了舞台呢？来一探究竟。
<a name="gncUj"></a>
## **一、stop的落幕**
首先stop方法的作用是什么呢，用Java源码中的一句注释来了解一下：Forces the thread to stop executing.，即强制线程停止执行，'Forces’似乎已经透漏出了stop方法的蛮狠无理。<br />那么再看看Java开发者是怎们解释stop被淘汰了的：
> This method is inherently unsafe. Stopping a thread with Thread.stop causes it to unlock all of the monitors that it has locked (as a natural consequence of the uncheckedThreadDeath exception propagating up the stack). If any of the objects previously protected by these monitors were in an inconsistent state, the damaged objects become visible to other threads, potentially resulting in arbitrary behavior. Many uses of stop should be replaced by code that simply modifies some variable to indicate that the target thread should stop running. The target thread should check this variable regularly, and return from its run method in an orderly fashion if the variable indicates that it is to stop running. If the target thread waits for long periods (on a condition variable, for example), the interrupt method should be used to interrupt the wait.

从中可以看出以下几点：

1. stop这种方法本质上是不安全的
2. 使用Thread.stop停止线程会导致它解锁所有已锁定的监视器，即直接释放当前线程已经获取到的所有锁，使得当前线程直接进入阻塞状态

举例来看一下上边提到的两点：
```java
public static void main(String[] args) throws InterruptedException {
    Object o1=new Object();
    Object o2=new Object();
    Thread t1=new Thread(()->{
        synchronized (o1)
        {
            synchronized (o2)
            {
                try {
                    System.out.println("t1获取到锁");
                    Thread.sleep(5000);
                    System.out.println("t1结束");
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    });
    t1.start();
    Thread.sleep(1000);
    Thread t2=new Thread(()->{
        synchronized (o1)
        {
            synchronized (o2)
            {
                try {
                    System.out.println("t2获取到锁");
                    Thread.sleep(5000);
                    System.out.println("t2结束");
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    });
    t2.start();
    t1.stop();
}
```
运行结果：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1702354241489-6fe4de12-482d-4115-b600-901c0c7fa93c.png#averageHue=%23fafaf9&clientId=ub65e02f4-e850-4&from=paste&height=162&id=u75f5ba1b&originHeight=406&originWidth=1881&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=69826&status=done&style=none&taskId=udb1e26c9-cff0-4d54-85ab-45148da0592&title=&width=752.4)<br />可以看到，当线程t1在获取到o1和o2两个锁开始执行，在还没有执行结束的时候，主线程调用了t1的stop方法中断了t1的执行，释放了t1线程获取到的所有锁，中断后t2获取到了o1和o2锁，开始执行直到结束，而t1却夭折在了sleep的时候，sleep后的代码没有执行。<br />因此使用stop在不知道线程到底运行到了什么地方，暴力的中断了线程，如果sleep后的代码是资源释放、重要业务逻辑等比较重要的代码的话，亦或是其他线程依赖t1线程的运行结果，那直接中断将可能造成很严重的后果。<br />那么不建议使用stop中断线程应该怎么去优雅的结束一个线程呢，可以从Java开发者的注释中窥探到一种解决方案：
> Many uses of stop should be replaced by code that simply modifies some variable to indicate that the target thread should stop running. The target thread should check this variable regularly, and return from its run method in an orderly fashion if the variable indicates that it is to stop running. If the target thread waits for long periods (on a condition variable, for example), the interrupt method should be used to interrupt the wait.

可以看到Java开发者推荐使用以下两种方法来优雅的停止线程。
<a name="RC4L9"></a>
### 1、定义一个变量，由目标线程去不断的检查变量的状态，当变量达到某个状态时停止线程。
代码举例如下：
```java
volatile static boolean flag=false;
public static void main(String[] args) throws InterruptedException {
    Object o1=new Object();
    Thread t1=new Thread(()->{
        synchronized (o1)
        {
            try {
                System.out.println("t1获取到锁");
                while (!flag)
                Thread.sleep(5000);//执行业务逻辑
                System.out.println("t1结束");
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    });
    t1.start();
    Thread.sleep(1000);
    Thread t2=new Thread(()->{
        synchronized (o1)
        {
            try {
                System.out.println("t2获取到锁");
                Thread.sleep(5000);//执行业务逻辑
                System.out.println("t2结束");
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    });
    t2.start();
    flag=true;
}
```
运行结果：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1702354316956-b6018b96-be6f-467b-9609-0dc6d4b2504a.png#averageHue=%23fafaf9&clientId=ub65e02f4-e850-4&from=paste&height=171&id=uec7f5866&originHeight=427&originWidth=1838&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=74233&status=done&style=none&taskId=ue05ae300-7e5d-41fc-ace0-de13b3d9513&title=&width=735.2)
<a name="C8eyp"></a>
### 2、使用`interrupt`方法中断线程。
代码举例如下：
```java
public static void main(String[] args) throws InterruptedException {
    Object o1=new Object();
    Thread t1=new Thread(()->{
        synchronized (o1)
        {
            System.out.println("t1获取到锁");
            while (!Thread.currentThread().isInterrupted()) {
                for (int i = 0; i < 100; i++) {
                    if(i==50)
                        System.out.println();
                    System.out.print(i+" ");
                }
                System.out.println();
            }
            System.out.println("t1结束");
        }
    });
    t1.start();
    Thread t2=new Thread(()->{
        synchronized (o1)
        {
            try {
                System.out.println("t2获取到锁");
                Thread.sleep(5000);//执行业务逻辑
                System.out.println("t2结束");
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    });
    t2.start();
    t1.interrupt();
}
```
运行结果：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1702354384638-b21ace24-12eb-45c5-a6f8-6108a45ed092.png#averageHue=%23fbfafa&clientId=ub65e02f4-e850-4&from=paste&height=222&id=u71f14d58&originHeight=554&originWidth=2981&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=120943&status=done&style=none&taskId=ue1fc02b6-197c-482a-9b3b-36b4d7b7dca&title=&width=1192.4)<br />用`while (!Thread.currentThread().isInterrupted())`来不断判断当前线程是否被中断，中断的话则让线程自然消亡并释放锁。<br />可以看到调用`interrupt`方法后并不会像stop那样暴力的中断线程，会等到当前运行的逻辑结束后再检查是否中断，非常的优雅。<br />注：运行举例代码可能不会打印出数字，这是因为t1线程运行到`while(!Thread.currentThread().isInterrupted())`时，主线程已经调了`interrupt`方法，因此多次运行可能会打印出数字。
<a name="vGwmI"></a>
## **二、suspend的落幕**
`suspend`方法的作用是挂起某个线程直到调用`resume`方法来恢复该线程，但是调用了suspend方法后并不会释放被挂起线程获取到的锁，正因如此就给suspend和resume这哥俩贴上了容易引发死锁的标签，当然这也正是导致suspend和resume退出历史舞台的罪魁祸首。同样看看Java开发者为suspend的淘汰给出的理由：
> This method has been deprecated, as it is inherently deadlock-prone. If the target thread holds a lock on the monitor protecting a critical system resource when it is suspended, no thread can access this resource until the target thread is resumed. If the thread that would resume the target thread attempts to lock this monitor prior to calling resume, deadlock results. Such deadlocks typically manifest themselves as “frozen” processes.

从中可以得出以下结论：

1. `suspend`具有天然的死锁倾向
2. 当某个线程被suspend后，该线程持有的锁不会被释放，其他线程也就不能访问这些资源
3. `suspend`某个线程后，如果在resume的过程中出现异常导致resume方法执行失败，则lock无法释放，导致死锁

接下来模拟一下由`suspend`引起的死锁场景，Talk is cheap,show my code：
```java
public static void main(String[] args) throws InterruptedException {
    Object o1=new Object();
    Object o2=new Object();
    Thread t1=new Thread(()->{
        synchronized (o1)
        {
            System.out.println("t1获取到o1锁开始执行");
            try {
                Thread.sleep(5000);//模拟执行业务逻辑
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println("t1执行结束");
        }
    });
    t1.start();
    Thread t2=new Thread(()->{
        synchronized (o2)
        {
            System.out.println("t2获取到o2开始执行");
            try {
                Thread.sleep(2000);//执行耗时业务
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            synchronized (o1)
            {
                System.out.println("t2获取到o1锁开始继续执行");
            }
            System.out.println("t2执行结束");
        }
    });
    t2.start();

    Thread.sleep(1000);
    t1.suspend();
    //假设抛出了一个未知异常
    int i=1/0;
    t1.resume();
}
```
运行结果：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1702354444223-aca02c7d-03d6-4344-b42c-f5b92fefb3a8.png#averageHue=%23f5f3f2&clientId=ub65e02f4-e850-4&from=paste&height=85&id=u4adf30eb&originHeight=213&originWidth=2758&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=95794&status=done&style=none&taskId=u2a10f609-33d0-4a05-b0a7-fd432deec5f&title=&width=1103.2)<br />可以看到，整个程序卡的死死的，在调用resume恢复t1线程之前抛出了一个未知异常，导致t1一直挂起进而无法释放o1锁，而t2需要获取到o1锁后才能继续执行，但苦苦等待，奈何o1被t1拿捏的死死的，从此整个程序就陷入了无尽的等待中----死锁。<br />**参考：**<br />[https://docs.oracle.com/javase/9/docs/api/java/lang/doc-files/threadPrimitiveDeprecation.html](https://docs.oracle.com/javase/9/docs/api/java/lang/doc-files/threadPrimitiveDeprecation.html)
