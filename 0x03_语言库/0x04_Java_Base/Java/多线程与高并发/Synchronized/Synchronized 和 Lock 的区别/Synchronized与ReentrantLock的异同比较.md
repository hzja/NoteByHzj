Java<br />在Java中，`Synchronized`和`ReentrantLock`都是用于实现线程同步的机制。它们的目标都是为了解决多线程并发访问共享资源时可能出现的竞态条件和数据不一致的问题。然而，它们在实现方式、使用场景和功能特性上存在一些不同之处。接下来将详细比较`Synchronized`和`ReentrantLock`的异同。
<a name="yuN2w"></a>
## 相同点

1. 线程同步：`Synchronized`和`ReentrantLock`都可以保证多个线程在访问共享资源时的互斥性。它们都可以防止多个线程同时对同一个资源进行修改，从而保证数据的一致性。
2. 锁定机制：`Synchronized`和`ReentrantLock`都采用了锁定机制来实现线程同步。当一个线程获取到锁后，其他线程必须等待锁的释放才能继续执行。
3. 可重入性：`Synchronized`和`ReentrantLock`都支持可重入性，即一个线程可以重复获得已经持有的锁，而不会发生死锁。
<a name="Qvq95"></a>
## 不同点

1. 实现方式：`Synchronized`是Java语言内置的关键字，是基于JVM实现的同步机制。而`ReentrantLock`是基于Java的Lock接口实现的，并提供了更灵活的同步机制。
2. 性能：在低并发的情况下，`Synchronized`的性能表现通常要优于`ReentrantLock`。`Synchronized`在JDK的不断优化下，其性能得到了很大的提升。但是在高并发的情况下，`ReentrantLock`的性能可能会更好，因为它在功能上提供了更多的选择和控制。
3. 可中断性：`ReentrantLock`相比于`Synchronized`更具有可中断性。`ReentrantLock`提供了`lockInterruptibly()`方法，可以响应线程的中断请求，而`Synchronized`不支持这种机制。
4. 公平性：`ReentrantLock`可以设置为公平锁或非公平锁，而`Synchronized`是一种非公平锁。公平锁是指多个线程按照请求的顺序来获取锁，而非公平锁则允许插队现象的存在。
5. 条件变量：`ReentrantLock`可以通过`Condition`对象实现线程间的等待和唤醒操作，而`Synchronized`没有直接对应的功能。`Condition`可以方便地实现复杂的线程间协作。
6. 灵活性：`ReentrantLock`提供了更灵活的同步机制。它支持多个条件变量、可选择的公平性和可中断性，并且可以自由地控制锁的获取和释放。而`Synchronized`的同步机制相对简单，只能通过`synchronized`关键字来实现。
<a name="b8kTf"></a>
## 代码示例
下面是一个使用`Synchronized`和`ReentrantLock`的示例代码：
```java
import java.util.concurrent.locks.ReentrantLock;

public class SynchronizedAndReentrantLockExample {
    private static int counter = 0;
    private static final Object lock = new Object();
    private static final ReentrantLock reentrantLock = new ReentrantLock();

    public static void main(String[] args) throws InterruptedException {
        Thread t1 = new Thread(() -> {
            for (int i = 0; i < 100000; i++) {
                synchronized (lock) {
                    counter++;
                }
            }
        });

        Thread t2 = new Thread(() -> {
            for (int i = 0; i < 100000; i++) {
                reentrantLock.lock();
                try {
                    counter++;
                } finally {
                    reentrantLock.unlock();
                }
            }
        });

        t1.start();
        t2.start();

        t1.join();
        t2.join();

        System.out.println("Counter: " + counter);
    }
}
```
以上代码创建了两个线程t1和t2，分别使用`Synchronized`和`ReentrantLock`来对`counter`进行递增操作。最后输出`counter`的值。<br />通过运行以上代码，可以观察到两种不同的同步机制的效果和性能差异。<br />以上就是对`Synchronized`和`ReentrantLock`的详细比较。了解它们的异同对于合理选择和使用线程同步机制非常重要。根据实际的需求和场景选择合适的同步机制，可以更好地保证多线程程序的稳定性和性能。
