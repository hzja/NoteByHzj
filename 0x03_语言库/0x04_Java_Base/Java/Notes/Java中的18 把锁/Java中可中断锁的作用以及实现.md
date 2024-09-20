Java 可中断锁<br />在 Java 中有两种锁，一种是内置锁 `synchronized`，一种是显示锁 `Lock`，其中 `Lock` 锁是可中断锁，而 `synchronized` 则为不可中断锁。<br />所谓的**中断锁指的是锁在执行时可被中断，也就是在执行时可以接收 **`**interrupt**`** 的通知，从而中断锁执行**。
:::info
PS：默认情况下 Lock 也是不可中断锁，但是可以通过特殊的“手段”，可以让其变为可中断锁。
:::
<a name="PSMdZ"></a>
## 为什么需要可中断锁？
不可中断锁的问题是，当出现“异常”时，只能一直阻塞等待，别无其他办法，比如下面这个程序。下面的这个程序中有两个线程，其中线程 1 先获取到锁资源执行相应代码，而线程 2 在 0.5s 之后开始尝试获取锁资源，但线程 1 执行时忘记释放锁了，这就造成线程 2 一直阻塞等待的情况，实现代码如下：
```java
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

publicclass InterruptiblyExample {
    public static void main(String[] args) {
        Lock lock = new ReentrantLock();

        // 创建线程 1
        Thread t1 = new Thread(new Runnable() {
            @Override
            public void run() {
                lock.lock();
                System.out.println("线程 1：获取到锁.");
                // 线程 1 未释放锁
            }
        });
        t1.start();

        // 创建线程 2
        Thread t2 = new Thread(new Runnable() {
            @Override
            public void run() {
                // 先休眠 0.5s，让线程 1 先执行
                try {
                    Thread.sleep(500);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                // 获取锁
                System.out.println("线程 2:等待获取锁.");
                lock.lock();
                try {
                    System.out.println("线程 2：获取锁成功.");
                } finally {
                    lock.unlock();
                }
            }
        });
        t2.start();
    }
}
```
以上代码执行的结果如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1631540423627-ddfca084-7c5b-43b3-b36c-3e4a152baa3b.webp#clientId=ud9c28efc-810e-4&from=paste&id=uca9b66d4&originHeight=191&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=u49a69f7c-fc9f-4004-aa13-43cfcc394ee)从上述结果可以看出，此时线程 2 在等待获取锁的操作，然而经历了 N 久之后...再次查看结果，依然是熟悉的画面：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1631540423616-dc379a19-7adc-413c-949e-afe6bd2e70f8.webp#clientId=ud9c28efc-810e-4&from=paste&id=u385b25eb&originHeight=191&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=u31ff626d-46f7-439c-a5fe-43a3a746342)线程 2 还在阻塞等待获取线程 1 释放锁资源，此时的线程 2 除了等之外，并无其他方法。<br />并且，熟练地拿出了 JConsole，试图得到一个死锁的具体信息时，却得到了这样的结果：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1631540423633-5a29383d-0a36-40ed-81df-c9de0a58cd8f.webp#clientId=ud9c28efc-810e-4&from=paste&id=u43dd1b0c&originHeight=917&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=u32ff951a-7c5a-4657-896e-21a4f9f5ea1)并没有检测到任何死锁信息，从上图可以看出，当只有一个锁资源的时候，系统并不会把这种情况判定为死锁，当然也没有阻塞等待的具体信息喽，此时只剩下线程 2 孤单地等待着它的“锁儿”。
<a name="DLlZV"></a>
## 使用中断锁
然而，中断锁的出现，就可以打破这一僵局，它可以在等待一定时间之后，主动的中断线程 2，以解决线程阻塞等待的问题。<br />中断锁的核心实现代码是 `lock.lockInterruptibly()` 方法，它和 `lock.lock()` 方法作用类似，只不过使用 `lockInterruptibly` 方法可以优先接收中断的请求，中断锁的具体实现如下：
```java
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

publicclass InterruptiblyExample {
    public static void main(String[] args) throws InterruptedException {
        Lock lock = new ReentrantLock();

        // 创建线程 1
        Thread t1 = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    // 加锁操作
                    lock.lock();
                    System.out.println("线程 1:获取到锁.");
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                // 线程 1 未释放锁
            }
        });
        t1.start();

        // 创建线程 2
        Thread t2 = new Thread(new Runnable() {
            @Override
            public void run() {
                // 先休眠 0.5s，让线程 1 先执行
                try {
                    Thread.sleep(500);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                // 获取锁
                try {
                    System.out.println("线程 2:尝试获取锁.");
                    lock.lockInterruptibly(); // 可中断锁
                    System.out.println("线程 2:获取锁成功.");
                } catch (InterruptedException e) {
                    System.out.println("线程 2:执行已被中断.");
                }
            }
        });
        t2.start();

        // 等待 2s 后,终止线程 2
        Thread.sleep(2000);
        if (t2.isAlive()) { // 线程 2 还在执行
            System.out.println("执行线程的中断.");
            t2.interrupt();
        } else {
            System.out.println("线程 2:执行完成.");
        }
    }
}
```
以上代码执行结果如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1631540423603-c1bac4bd-0ba7-4b9a-99e3-519158260daf.webp#clientId=ud9c28efc-810e-4&from=paste&id=u10c3b6f4&originHeight=260&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=u95574a44-8e18-43a2-b703-617c00b48b2)从上述结果可以看出，当使用了 `lockInterruptibly` 方法就可以在一段时间之后，判断它是否还在阻塞等待，如果结果为真，就可以直接将他中断，如上图效果所示。<br />但当尝试将 `lockInterruptibly` 方法换成 `lock` 方法之后（其他代码都不变），执行的结果就完全不一样了，实现代码如下：
```java
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

publicclass InterruptiblyExample {
    public static void main(String[] args) throws InterruptedException {
        Lock lock = new ReentrantLock();

        // 创建线程 1
        Thread t1 = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    // 加锁操作
                    lock.lockInterruptibly();
                    System.out.println("线程 1:获取到锁.");
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                // 线程 1 未释放锁
            }
        });
        t1.start();

        // 创建线程 2
        Thread t2 = new Thread(new Runnable() {
            @Override
            public void run() {
                // 先休眠 0.5s，让线程 1 先执行
                try {
                    Thread.sleep(500);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                // 获取锁
                try {
                    System.out.println("线程 2:尝试获取锁.");
                    lock.lock();
                    System.out.println("线程 2:获取锁成功.");
                } catch (Exception e) {
                    System.out.println("线程 2:执行已被中断.");
                }
            }
        });
        t2.start();

        // 等待 2s 后,终止线程 2
        Thread.sleep(2000);
        if (t2.isAlive()) { // 线程 2 还在执行
            System.out.println("执行线程的中断.");
            t2.interrupt();
        } else {
            System.out.println("线程 2:执行完成.");
        }
    }
}
```
以上程序执行结果如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1631540423627-4b259317-9530-4e88-8040-6e7ffbb1775c.webp#clientId=ud9c28efc-810e-4&from=paste&id=u5f56e444&originHeight=174&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=ucd6bcb62-b473-45af-a313-61fb94c7a2b)从上图可以看出，当使用 `lock` 方法时，即使调用了 `interrupt` 方法依然不能将线程 2 进行中断。
<a name="ckhXJ"></a>
## 总结
本文介绍了中断锁的实现，通过显示锁 `Lock` 的 `lockInterruptibly` 方法来完成，它和 lock 方法作用类似，但 `lockInterruptibly` 可以优先接收到中断的通知，而 `lock` 方法只能“死等”锁资源的释放，同时这两个方法的区别也是常见的面试题。
