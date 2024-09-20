Java 锁
<a name="hNmhF"></a>
## 1、概念
说到Java中的锁要联系到多线程。<br />多线程确实在效率上带来了很大的便利，便利的同时不得不考虑多个线程之间对资源竞争引起的安全问题。<br />同步关键字`synchronized`是比较熟悉的用来解决线程安全的一个关键字，但是锁（Lock）是一个在资源竞争激励的情况下性能更优于`synchronized`的方法。<br />再看一些多并发文章中，会提及各种各样锁如公平锁，乐观锁，读写锁等等，这里说一些常用的锁，把主要的锁讲透，会用。
<a name="rvlkB"></a>
## 2、分类
按照锁的特性和设计来划分，分为如下几类：<br />1、公平锁/非公平锁<br />2、可重入锁<br />3、独享锁/共享锁<br />4、互斥锁/读写锁<br />5、乐观锁/悲观锁<br />6、分段锁<br />7、偏向锁/轻量级锁/重量级锁<br />8、自旋锁（java.util.concurrent包下的几乎都是利用锁）<br />从底层角度看常见的锁也就两种：`Synchronized`和Lock接口以及`ReadWriteLock`接口（读写锁）
<a name="wEKGo"></a>
## 3、介绍
说一下每一个锁的概念。
<a name="CfmFa"></a>
### 1、公平锁/非公平锁
公平锁指多个线程按照申请锁的顺序来依次获取锁。<br />非公平锁指多个线程获取锁的顺序并不是按照申请锁的顺序来获取，有可能后申请锁的线程比先申请锁的线程优先获取到锁，此极大的可能会造成线程饥饿现象，迟迟获取不到锁。<br />由于`ReentrantLock`是通过AQS来实现线程调度，可以实现公平锁，但是`synchroized`是非公平的，无法实现公平锁。
<a name="JUJKU"></a>
### 2、可重入锁
可重入锁又名递归锁，是指在同一个线程，在外层方法获取锁的时候，在进入内层方法会自动获取锁。
<a name="dyds8"></a>
### 3、独享锁/共享锁
独享锁是指该锁一次只能被一个线程所持有。<br />共享锁是指该锁可被多个线程所持有。
<a name="CltMF"></a>
### 4、互斥锁/读写锁
上面讲的独享锁/共享锁就是一种广义的说法，互斥锁/读写锁就是具体的实现。<br />互斥锁在Java中的具体实现就是`ReentrantLock`<br />读写锁在Java中的具体实现就是`ReadWriteLock`
<a name="oiQVi"></a>
### 5、乐观锁/悲观锁
悲观锁认为对于同一个数据的并发操作，一定是会发生修改的，哪怕没有修改，也会认为修改。因此对于同一个数据的并发操作，悲观锁采取加锁的形式。悲观的认为，不加锁的并发操作一定会出问题。<br />乐观锁则认为对于同一个数据的并发操作，是不会发生修改的。在更新数据的时候，会采用尝试更新。乐观的认为，不加锁的并发操作是没有事情的。
<a name="hS99k"></a>
### 6、分段锁
分段锁其实是一种锁的设计，并不是具体的一种锁，对于`ConcurrentHashMap`而言，其并发的实现就是通过分段锁的形式来实现高效的并发操作。
<a name="hoekj"></a>
### 7、偏向锁
偏向锁是指一段同步代码一直被一个线程所访问，那么该线程会自动获取锁。降低获取锁的代价。
<a name="JqZz9"></a>
### 8、自旋锁（java.util.concurrent包下的几乎都是利用锁）
自旋锁是指尝试获取锁的线程不会立即阻塞，而是采用循环的方式去尝试获取锁，这样的好处是减少线程上下文切换的消耗，缺点是循环会消耗CPU。
<a name="foGjb"></a>
## 4、实战
其实上面说的很多概念，没有使用前也是一脸懵的，毕竟使用很少的情况下，只是惯性的去使用，没有多想其他，更别说有的几乎没有使用过，第一次听说的都有。<br />遇到这种使用很少或者没有使用过的，最好通过实例来认识。<br />这里不做更多的介绍，只介绍常用的几种。<br />再说其他几种锁之前说先说 Lock接口与`synchronized`关键字。
<a name="UaYUq"></a>
### 1、`synchronized`关键字
很多人都知道 `synchronized`关键字 是一种同步锁。<br />它的原理是：一个线程访问一个对象中的`synchronized(this)`同步代码块时，其他试图访问该对象的线程将被阻塞。
```java
import java.util.ArrayList;
import java.util.List;

public class Test {
    public static void main(String[] args) {
        System.out.println("使用关键字synchronized");
        SyncThread syncThread = new SyncThread();
        Thread thread1 = new Thread(syncThread, "SyncThread1");
        Thread thread2 = new Thread(syncThread, "SyncThread2");
        thread1.start();
        thread2.start();
    }
}
class SyncThread implements Runnable {
    private static int count;
    public SyncThread() {
        count = 0;
    }
    public  void run() {
       synchronized (this){
            for (int i = 0; i < 5; i++) {
                try {
                    System.out.println("线程name:"+Thread.currentThread().getName() + ":" + (count++));
                    Thread.sleep(100);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }
    public int getCount() {
        return count;
    }
}
```
分析：<br />1、当两个并发线程(thread1和thread2)访问同一个对象(syncThread)中的`synchronized`代码块时，在同一时刻只能有一个线程得到执行，另一个线程受阻塞，必须等待当前线程执行完这个代码块以后才能执行该代码块。<br />2、Thread1和thread2是互斥的，因为在执行`synchronized`代码块时会锁定当前的对象，只有执行完该代码块才能释放该对象锁，下一个线程才能执行并锁定该对象。<br />修饰一个方法<br />`Synchronized`修饰一个方法很简单，就是在方法的前面加`synchronized`。<br />将上面的代码修改如下：
```java
public synchronized void run() {
   {
        for (int i = 0; i < 5; i++) {
            try {
                System.out.println("线程name:"+Thread.currentThread().getName() + ":" + (count++));
                Thread.sleep(100);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
```
分析：<br />1、输入结果可以发现两个线程是交替的<br />2、虽然可以使用`synchronized`来定义方法，但`synchronized`并不属于方法定义的一部分，因此，`synchronized`关键字不能被继承。<br />如果在父类中的某个方法使用了`synchronized`关键字，而在子类中覆盖了这个方法，在子类中的这个方法默认情况下并不是同步的，而必须显式地在子类的这个方法中加上`synchronized`关键字才可以
<a name="DgYzs"></a>
### 2、Lock接口
JDK1.5之后并发包中新增了Lock接口以及相关实现类来实现锁功能。
```java
Lock lock=new ReentrantLock()；
lock.lock();
try{
}finally{
    lock.unlock();
}
```
分析：上面的代码时Lock接口的简单使用比较简单。<br />还是以一个售票的机制来看 Lock接口锁时怎么实现的？
```java

import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;
/**
 * 一、用于解决多线程安全问题的方式：
 * 1.同步代码块 synchronized 隐式锁
 * 2.同步方法 synchronized 隐式锁
 * 3.同步锁Lock (jdk1.5以后) 显示锁
 * 注意：显示锁，需要通过lock()方式上锁，必须通过unlock()方式进行释放锁
 */
public class TestLock {
    public static void main(String[] args) {
        TicketCar ticketCar = new TicketCar();
        new Thread(ticketCar, "1号窗口").start();
        new Thread(ticketCar, "2号窗口").start();
        new Thread(ticketCar, "3号窗口").start();
    }
}
class TicketCar implements Runnable {
    private int tick = 100;
    private Lock lock = new ReentrantLock();
    @Override
    public void run() {
        while (true) {
            lock.lock();
            try {
                if (tick > 0) {
                    try {
                        Thread.sleep(200);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    System.out.println(Thread.currentThread().getName() + " 完成售票，余票为 " + --tick);
                }
            } finally {
                lock.unlock();
            }
        }
    }
}
```
上面说了工作中常用的两种方式，下面再简单聊几种比较常用的。
<a name="Vmqeh"></a>
### 3、读写锁
关于使用读写锁就是在读的时候上读锁，写的时候上写锁，主要用来解决`synchronized`不能解决的问题（两个线程同时读，理论上是可以并行的，但是`synchronized`是加了锁的）。<br />`ReadWriteLock`接口中有两个方法，分别是`readLock`和`writeLock`。源码如下：
```java
public interface ReadWriteLock {
    /**
     * 返回读锁
     */
    Lock readLock();
    /**
     * 返回写锁
     */
    Lock writeLock();
}
```
关于读写锁做一个操作，在写操作的时候不允许读操作，读写分开。
```java
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantReadWriteLock;
public class threadTest {
    static Map<String, Object> map = new HashMap<String, Object>();
    static ReentrantReadWriteLock rwl = new ReentrantReadWriteLock();
    static Lock r = rwl.readLock();
    static Lock w = rwl.writeLock();
    // 获取一个key对应的value
    public static final Object get(String key) {
        Object object = null;
        try {
            r.lock();
            System.out.println("正在做读的操作,key:" + key + " 开始");
            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
            object = map.get(key);
            System.out.println("正在做读的操作,key:" + key + " 结束");
            System.out.println();
        } catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }finally{
            r.unlock();
        }
        return object;
    }
    // 设置key对应的value，并返回旧有的value
    public static final Object put(String key, Object value) {
        Object object = null;
        try {
            w.lock();
            System.out.println("正在做写的操作,key:" + key + ",value:" + value + "开始.");
            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
            object = map.put(key, value);
            System.out.println("正在做写的操作,key:" + key + ",value:" + value + "结束.");
            System.out.println();
        } catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }finally{
            w.unlock();
        }
        return object;
    }
    public static void main(String[] args) {
        new Thread(new Runnable() {
            public void run() {
                for (int i = 0; i < 10; i++) {
                    threadTest.put(i + "", i + "");
                }
            }
        }).start();
        new Thread(new Runnable() {
            public void run() {
                for (int i = 0; i < 10; i++) {
                    threadTest.get(i + "");
                }
            }
        }).start();
    }
}
```
1、调用读写方法的时候 添加了 锁 lock ，只有在`finally`执行玩调用`unlock`，解锁后才能允许其他锁的执行。
<a name="jbcRp"></a>
### 4、可重入锁
对于Java `ReentrantLock`而言，他的名字就可以看出是一个可重入锁，其名字是Re entrant Lock重新进入锁。<br />对于`Synchronized`而言，也是一个可重入锁。可重入锁的一个好处是可一定程度避免死锁。
<a name="D1KdR"></a>
### 5、公平锁/非公平锁
```java

/**
 * 公平锁与非公平锁
 */
public class FairAndUnFairThread {
    public static void main(String[] args) throws InterruptedException {
        //默认非公平锁
        final Lock lock = new ReentrantLock(true);
        final MM m = new MM(lock);
        for (int i=1;i<=10 ;i++){
            String name = "线程"+i;
            Thread tt = new Thread(new Runnable() {
                @Override
                public void run() {
                    for(int i=0;i<2;i++){
                        m.testReentrant();
                    }
                }
            },name);
            tt.start();
        }

    }
}
class MM {
    Lock lock = null;
    MM(Lock lock){
        this.lock = lock;
    }

    public void testReentrant(){
        lock.lock();
        try{
            Thread.sleep(1);
            System.out.println(Thread.currentThread().getName() );
        } catch (InterruptedException e) {
            e.printStackTrace();
        } finally {
            lock.unlock();
        }
    }

    public synchronized  void testSync(){
        System.out.println(Thread.currentThread().getName());
    }

}
```
1、这种公平也不是绝对的 不一定就是按照顺序，可能因为CPU准备原因，可能会有一些不公平的。
