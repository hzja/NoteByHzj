JavaSynchronizedLock
<a name="bqVXc"></a>
## synchronized
<a name="LfTiU"></a>
### 特性
`synchronized`是Java的关键字，是基于JVM底层的`Monitor`锁实现的，用于修饰方法或代码块。在JDK1.6之前，`synchronized`的锁性能较低，但是在JDK1.6开始，JDK对`synchronized`锁进行了大量的优化，同时引入了无锁、偏向锁、轻量级锁、重量级锁、锁消除、锁粗化等技术来提升`synchronized`锁的性能。
<a name="aJxSe"></a>
### 使用方法
`synchronized`有两种使用方法

1. 锁代码块
2. 锁方法
```java
public class SynchronizedExample {
    public static void main(String[] args) {

        SynchronizedExample synchronizedExample = new SynchronizedExample();

        // 1.锁代码块
        synchronizedExample.syncCodeBlock();

        // 2.锁方法
        synchronizedExample.syncMethod();
    }

    private void syncCodeBlock() {
        synchronized (this) {
            System.out.println("sync code block");
        }
    }

    private synchronized void  syncMethod() {
        System.out.println("sync method");
    }
}
```
<a name="KabVc"></a>
## Lock
<a name="TxbhO"></a>
### 特性
Lock是Java并发包JUC下的接口，其有很多实现类，较为常用的是`ReentrantLock`，默认使用非公平锁。`Lock`需要进行手动加锁(`lock`)和解锁(`unlock`)。同时，`Lock`还有更强大的功能，如它的`tryLock()`方法可以非阻塞的去获取锁、使用`Condition`实现等待通知效果。
<a name="oaeqE"></a>
### 使用方法
```java
public class LockExample {
    public static void main(String[] args) {

        Lock lock = new ReentrantLock();

        lock.lock();
        try {
            System.out.println("lock...");
        }finally {
            lock.unlock();
        }
    }
}
```
<a name="xLFK1"></a>
## 对比
| 特征 | synchronized | Lock |
| --- | --- | --- |
| 锁类型 | 内置锁 | 显式锁 |
| 使用方式 | 关键字 | 对象实例化 |
| 可重入 | 是 | 是 |
| 可中断 | 否 | 是 |
| 公平性 | 不保证 | 可手动设置为公平或非公平 |
| 锁的数量 | 单一 | 可以创建多个 |
| 锁的灵活性 | 限制较多，无法扩展或自定义功能 | 提供更多灵活性，可以自定义功能 |
| 异常处理 | 由JVM自动处理 | 需要手动处理 |

<a name="SWlsF"></a>
## 总结

1. `synchronized`是一个关键字而`lock`是一个接口（`lock`、`lockInterruptibly`、`tryLock`、`unlock`、`newCondition`）。
2. `synchronized`是隐式的加锁，`lock`是显示的加锁。
3. `synchronized`可以作用在方法和代码块上，而`lock`只能作用在代码块上。

`synchronized` 作用在静态方法上锁的是当前类的class；作用在普通方法上锁的是当前类的对象；作用于代码块上锁的是`synchronized(xxx)`，括号里的对象 使用javap反编译成字节码后，锁代码块时，`synchronized`关键字需要有一个代码块进入的点`monitorenter`，代码块退出和代码块异常的出口点`monitorexit`；锁方法时，使用`ACC_SYNCHRONIZED`标识

4. `synchronized`是阻塞式加锁，而`lock`中的`trylock`支持非阻塞式加锁。
5. `synchronized`没有超时机制，而`lock`中的`trylcok`可以支持超时机制。
6. `synchronized`不可中断，而`lock`中的`lockInterruptibly`可中断的获取锁。

`ReentrantLock.lockInterruptibly`允许在等待时由其它线程调用等待线程的`Thread.interrupt`方法来中断等待线程的等待而直接返回，这时不用获取锁，而会抛出一个`InterruptedException`。`ReentrantLock.lock`方法不允许`Thread.interrupt`中断，即使检测到`Thread.isInterrupted`，一样会继续尝试获取锁，失败则继续休眠。只是在最后获取锁成功后再把当前线程置为`interrupted`状态，然后再中断线程。

7. `synchronized`采用的是`monitor`对象监视器，`lock`的底层原理是AQS
8. `synchronized`只有一个同步队列和一个等待队列，而`lock`有一个同步队列，可以有多个等待队列。
- 同步队列：排队取锁的线程所在的队列。
- 等待队列：调用 `wait` 方法后，线程会从同步队列转移到等待队列。
9. `synchronized`是非公平锁，而`lock`可以是公平锁也可以是非公平锁。
10. `synchronized`用object的`notify` / `notifyAll`方法进行唤醒，而`lock`用`condition`进行唤醒。
11. `lock`有`ReadWriteLock`支持并发读。

synchronized是一种简单且易于使用的锁机制，适合简单的线程同步需求。而Lock接口提供了更多的功能和灵活性，适用于复杂的同步场景。所以，选择哪一种锁需要基于具体的需求和设计。
