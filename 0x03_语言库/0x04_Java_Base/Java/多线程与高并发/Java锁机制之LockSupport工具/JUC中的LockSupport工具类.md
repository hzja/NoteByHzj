Java
<a name="Jgbvw"></a>
## 主要内容

1. **讲解3种让线程等待和唤醒的方法，每种方法配合具体的示例**
2. **介绍LockSupport主要用法**
3. **对比3种方式，了解他们之间的区别**

**LockSupport**位于**java.util.concurrent**（**简称juc**）包中，算是juc中一个基础类，juc中很多地方都会使用LockSupport。<br />关于线程等待/唤醒的方法，有2种：

1. 方式1：使用Object中的`wait()`方法让线程等待，使用Object中的`notify()`方法唤醒线程
2. 方式2：使用juc包中Condition的`await()`方法让线程等待，使用`signal()`方法唤醒线程

这2种方式，先来看一下示例。
<a name="y5KnL"></a>
## 使用Object类中的方法实现线程等待和唤醒
<a name="Qas3S"></a>
### 示例1：
```java
import java.util.concurrent.TimeUnit;

public class Demo1 {

    static Object lock = new Object();

    public static void main(String[] args) throws InterruptedException {
        Thread t1 = new Thread(() -> {
            synchronized (lock) {
                System.out.println(System.currentTimeMillis() + "," + Thread.currentThread().getName() + " start!");
                try {
                    lock.wait();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                System.out.println(System.currentTimeMillis() + "," + Thread.currentThread().getName() + " 被唤醒!");
            }
        });
        t1.setName("t1");
        t1.start();
        //休眠5秒
        TimeUnit.SECONDS.sleep(5);
        synchronized (lock) {
            lock.notify();
        }
    }
}
```
输出：
```
1563592938744,t1 start!
1563592943745,t1 被唤醒!
```
t1线程中调用`lock.wait()`方法让t1线程等待，主线程中休眠5秒之后，调用`lock.notify()`方法唤醒了t1线程，输出的结果中，两行结果相差5秒左右，程序正常退出。
<a name="rBuKW"></a>
### 示例2
把上面代码中main方法内部改一下，删除了`synchronized`关键字，看看有什么效果：
```java
import java.util.concurrent.TimeUnit;

public class Demo2 {

    static Object lock = new Object();

    public static void main(String[] args) throws InterruptedException {
        Thread t1 = new Thread(() -> {
            System.out.println(System.currentTimeMillis() + "," + Thread.currentThread().getName() + " start!");
            try {
                lock.wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println(System.currentTimeMillis() + "," + Thread.currentThread().getName() + " 被唤醒!");
        });
        t1.setName("t1");
        t1.start();
        //休眠5秒
        TimeUnit.SECONDS.sleep(5);
        lock.notify();
    }
}
```
运行结果：
```
Exception in thread "t1" java.lang.IllegalMonitorStateException
1563593178811,t1 start!
    at java.lang.Object.wait(Native Method)
    at java.lang.Object.wait(Object.java:502)
    at com.example.chat10.Demo2.lambda$main$0(Demo2.java:16)
    at java.lang.Thread.run(Thread.java:745)
Exception in thread "main" java.lang.IllegalMonitorStateException
    at java.lang.Object.notify(Native Method)
    at com.example.chat10.Demo2.main(Demo2.java:26)
```
上面代码中将`**synchronized**`去掉了，发现调用`wait()`方法和调用`notify()`方法都抛出了IllegalMonitorStateException异常，原因：**Object类中的wait、notify、notifyAll用于线程等待和唤醒的方法，都必须在同步代码中运行（必须用到关键字**`**synchronized**`**）**。
<a name="LzLpc"></a>
### 示例3
唤醒方法在等待方法之前执行，线程能够被唤醒么？代码如下：
```java
import java.util.concurrent.TimeUnit;

public class Demo3 {

    static Object lock = new Object();

    public static void main(String[] args) throws InterruptedException {
        Thread t1 = new Thread(() -> {
            try {
                TimeUnit.SECONDS.sleep(5);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            synchronized (lock) {
                System.out.println(System.currentTimeMillis() + "," + Thread.currentThread().getName() + " start!");
                try {
                    //休眠3秒
                    lock.wait();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                System.out.println(System.currentTimeMillis() + "," + Thread.currentThread().getName() + " 被唤醒!");
            }
        });
        t1.setName("t1");
        t1.start();
        //休眠1秒之后唤醒lock对象上等待的线程
        TimeUnit.SECONDS.sleep(1);
        synchronized (lock) {
            lock.notify();
        }
        System.out.println("lock.notify()执行完毕");
    }
}
```
运行代码，输出结果：
```
lock.notify()执行完毕
1563593869797,t1 start!
```
输出了上面2行之后，程序一直无法结束，t1线程调用`wait()`方法之后无法被唤醒了，从输出中可见，`notify()`方法在`wait()`方法之前执行了，等待的线程无法被唤醒了。说明：唤醒方法在等待方法之前执行，线程无法被唤醒。<br />**关于Object类中的用户线程等待和唤醒的方法，总结一下：**

1. `**wait()**`**/**`**notify()**`**/**`**notifyAll()**`**方法都必须放在同步代码（必须在**`**synchronized**`**内部执行）中执行，需要先获取锁**
2. **线程唤醒的方法（notify、notifyAll）需要在等待的方法（wait）之后执行，等待中的线程才可能会被唤醒，否则无法唤醒**
<a name="Ekffl"></a>
## 使用Condition实现线程的等待和唤醒
关于Condition准备了3个示例。
<a name="o93FI"></a>
### 示例1
```java
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

public class Demo4 {

    static ReentrantLock lock = new ReentrantLock();
    static Condition condition = lock.newCondition();

    public static void main(String[] args) throws InterruptedException {
        Thread t1 = new Thread(() -> {
            lock.lock();
            try {
                System.out.println(System.currentTimeMillis() + "," + Thread.currentThread().getName() + " start!");
                try {
                    condition.await();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                System.out.println(System.currentTimeMillis() + "," + Thread.currentThread().getName() + " 被唤醒!");
            } finally {
                lock.unlock();
            }
        });
        t1.setName("t1");
        t1.start();
        //休眠5秒
        TimeUnit.SECONDS.sleep(5);
        lock.lock();
        try {
            condition.signal();
        } finally {
            lock.unlock();
        }

    }
}
```
输出：
```
1563594349632,t1 start!
1563594354634,t1 被唤醒!
```
t1线程启动之后调用`condition.await()`方法将线程处于等待中，主线程休眠5秒之后调用`condition.signal()`方法将t1线程唤醒成功，输出结果中2个时间戳相差5秒。
<a name="n90UA"></a>
### 示例2
将上面代码中的`lock.lock()`、`lock.unlock()`去掉，看看会发生什么。代码：
```java
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

public class Demo5 {

    static ReentrantLock lock = new ReentrantLock();
    static Condition condition = lock.newCondition();

    public static void main(String[] args) throws InterruptedException {
        Thread t1 = new Thread(() -> {
            System.out.println(System.currentTimeMillis() + "," + Thread.currentThread().getName() + " start!");
            try {
                condition.await();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println(System.currentTimeMillis() + "," + Thread.currentThread().getName() + " 被唤醒!");
        });
        t1.setName("t1");
        t1.start();
        //休眠5秒
        TimeUnit.SECONDS.sleep(5);
        condition.signal();
    }
}
```
输出：
```
Exception in thread "t1" java.lang.IllegalMonitorStateException
1563594654865,t1 start!
    at java.util.concurrent.locks.ReentrantLock$Sync.tryRelease(ReentrantLock.java:151)
    at java.util.concurrent.locks.AbstractQueuedSynchronizer.release(AbstractQueuedSynchronizer.java:1261)
    at java.util.concurrent.locks.AbstractQueuedSynchronizer.fullyRelease(AbstractQueuedSynchronizer.java:1723)
    at java.util.concurrent.locks.AbstractQueuedSynchronizer$ConditionObject.await(AbstractQueuedSynchronizer.java:2036)
    at com.example.chat10.Demo5.lambda$main$0(Demo5.java:19)
    at java.lang.Thread.run(Thread.java:745)
Exception in thread "main" java.lang.IllegalMonitorStateException
    at java.util.concurrent.locks.AbstractQueuedSynchronizer$ConditionObject.signal(AbstractQueuedSynchronizer.java:1939)
    at com.example.chat10.Demo5.main(Demo5.java:29)
```
有异常发生，`condition.await();`和`condition.signal();`都触发了IllegalMonitorStateException异常。原因：**调用condition中线程等待和唤醒的方法的前提是必须要先获取lock的锁**。
<a name="YoWNe"></a>
### 示例3
唤醒代码在等待之前执行，线程能够被唤醒么？代码如下：
```java
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

public class Demo6 {

    static ReentrantLock lock = new ReentrantLock();
    static Condition condition = lock.newCondition();

    public static void main(String[] args) throws InterruptedException {
        Thread t1 = new Thread(() -> {
            try {
                TimeUnit.SECONDS.sleep(5);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            lock.lock();
            try {
                System.out.println(System.currentTimeMillis() + "," + Thread.currentThread().getName() + " start!");
                try {
                    condition.await();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                System.out.println(System.currentTimeMillis() + "," + Thread.currentThread().getName() + " 被唤醒!");
            } finally {
                lock.unlock();
            }
        });
        t1.setName("t1");
        t1.start();
        //休眠5秒
        TimeUnit.SECONDS.sleep(1);
        lock.lock();
        try {
            condition.signal();
        } finally {
            lock.unlock();
        }
        System.out.println(System.currentTimeMillis() + ",condition.signal();执行完毕");
    }
}
```
运行结果:
```
1563594886532,condition.signal();执行完毕
1563594890532,t1 start!
```
输出上面2行之后，程序无法结束，代码结合输出可以看出`signal()`方法在`await()`方法之前执行的，最终t1线程无法被唤醒，导致程序无法结束。<br />**关于Condition中方法使用总结：**

1. **使用Condtion中的线程等待和唤醒方法之前，需要先获取锁。否者会报IllegalMonitorStateException异常**
2. `**signal()**`**方法先于**`**await()**`**方法之前调用，线程无法被唤醒**
<a name="qJC66"></a>
## Object和Condition的局限性
关于Object和Condtion中线程等待和唤醒的局限性，有以下几点：

1. **2中方式中的让线程等待和唤醒的方法能够执行的先决条件是：线程需要先获取锁**
2. **唤醒方法需要在等待方法之后调用，线程才能够被唤醒**

关于这2点，LockSupport都不需要，就能实现线程的等待和唤醒。下面来说一下LockSupport类。
<a name="b3YcF"></a>
## LockSupport类介绍
LockSupport类可以阻塞当前线程以及唤醒指定被阻塞的线程。主要是通过`park()`和`unpark(thread)`方法来实现阻塞和唤醒线程的操作的。<br />每个线程都有一个许可(permit)，permit只有两个值1和0，默认是0。

1. 当调用`unpark(thread)`方法，就会将thread线程的许可permit设置成1(注意多次调用`unpark`方法，不会累加，permit值还是1)。
2. 当调用`park()`方法，如果当前线程的permit是1，那么将permit设置为0，并立即返回。如果当前线程的permit是0，那么当前线程就会阻塞，直到别的线程将当前线程的permit设置为1时，park方法会被唤醒，然后会将permit再次设置为0，并返回。

注意：因为permit默认是0，所以一开始调用`park()`方法，线程必定会被阻塞。调用`unpark(thread)`方法后，会自动唤醒thread线程，即`park`方法立即返回。
<a name="w3Ys5"></a>
### LockSupport中常用的方法
<a name="JsYIs"></a>
#### 阻塞线程

- `void park()`：阻塞当前线程，如果调用**unpark方法**或者**当前线程被中断**，从能从`park()`方法中返回
- `void park(Object blocker)`：功能同方法1，入参增加一个Object对象，用来记录导致线程阻塞的阻塞对象，方便进行问题排查
- `void parkNanos(long nanos)`：阻塞当前线程，最长不超过nanos纳秒，增加了超时返回的特性
- `void parkNanos(Object blocker, long nanos)`：功能同方法3，入参增加一个Object对象，用来记录导致线程阻塞的阻塞对象，方便进行问题排查
- `void parkUntil(long deadline)`：阻塞当前线程，直到deadline，deadline是一个绝对时间，表示某个时间的毫秒格式
- `void parkUntil(Object blocker, long deadline)`：功能同方法5，入参增加一个Object对象，用来记录导致线程阻塞的阻塞对象，方便进行问题排查；
<a name="w7QUM"></a>
#### 唤醒线程

- `void unpark(Thread thread):`唤醒处于阻塞状态的指定线程
<a name="mt8qn"></a>
### 示例1
主线程线程等待5秒之后，唤醒t1线程，代码如下：
```java
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.LockSupport;
import java.util.concurrent.locks.ReentrantLock;

public class Demo7 {

    public static void main(String[] args) throws InterruptedException {
        Thread t1 = new Thread(() -> {
            System.out.println(System.currentTimeMillis() + "," + Thread.currentThread().getName() + " start!");
            LockSupport.park();
            System.out.println(System.currentTimeMillis() + "," + Thread.currentThread().getName() + " 被唤醒!");
        });
        t1.setName("t1");
        t1.start();
        //休眠5秒
        TimeUnit.SECONDS.sleep(5);
        LockSupport.unpark(t1);
        System.out.println(System.currentTimeMillis() + ",LockSupport.unpark();执行完毕");
    }
}
```
输出：
```
1563597664321,t1 start!
1563597669323,LockSupport.unpark();执行完毕
1563597669323,t1 被唤醒!
```
t1中调用`LockSupport.park();`让当前线程t1等待，主线程休眠了5秒之后，调用`LockSupport.unpark(t1);`将t1线程唤醒，输出结果中1、3行结果相差5秒左右，说明t1线程等待5秒之后，被唤醒了。<br />`LockSupport.park();`无参数，内部直接会让当前线程处于等待中；`unpark`方法传递了一个线程对象作为参数，表示将对应的线程唤醒。
<a name="DgzBG"></a>
### 示例2
唤醒方法放在等待方法之前执行，看一下线程是否能够被唤醒呢？代码如下：
```java
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.LockSupport;

public class Demo8 {

    public static void main(String[] args) throws InterruptedException {
        Thread t1 = new Thread(() -> {
            try {
                TimeUnit.SECONDS.sleep(5);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println(System.currentTimeMillis() + "," + Thread.currentThread().getName() + " start!");
            LockSupport.park();
            System.out.println(System.currentTimeMillis() + "," + Thread.currentThread().getName() + " 被唤醒!");
        });
        t1.setName("t1");
        t1.start();
        //休眠1秒
        TimeUnit.SECONDS.sleep(1);
        LockSupport.unpark(t1);
        System.out.println(System.currentTimeMillis() + ",LockSupport.unpark();执行完毕");
    }
}
```
输出：
```
1563597994295,LockSupport.unpark();执行完毕
1563597998296,t1 start!
1563597998296,t1 被唤醒!
```
代码中启动t1线程，t1线程内部休眠了5秒，然后主线程休眠1秒之后，调用了`LockSupport.unpark(t1);`唤醒线程t1，此时`LockSupport.park();`方法还未执行，说明唤醒方法在等待方法之前执行的；输出结果中2、3行结果时间一样，表示`LockSupport.park();`没有阻塞了，是立即返回的。<br />说明：**唤醒方法在等待方法之前执行，线程也能够被唤醒，这点是另外2中方法无法做到的。Object和Condition中的唤醒必须在等待之后调用，线程才能被唤醒。而LockSupport中，唤醒的方法不管是在等待之前还是在等待之后调用，线程都能够被唤醒。**
<a name="Enqlz"></a>
### 示例3
`park()`让线程等待之后，是否能够响应线程中断？代码如下：
```java
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.LockSupport;

public class Demo9 {

    public static void main(String[] args) throws InterruptedException {
        Thread t1 = new Thread(() -> {
            System.out.println(System.currentTimeMillis() + "," + Thread.currentThread().getName() + " start!");
            System.out.println(Thread.currentThread().getName() + ",park()之前中断标志：" + Thread.currentThread().isInterrupted());
            LockSupport.park();
            System.out.println(Thread.currentThread().getName() + ",park()之后中断标志：" + Thread.currentThread().isInterrupted());
            System.out.println(System.currentTimeMillis() + "," + Thread.currentThread().getName() + " 被唤醒!");
        });
        t1.setName("t1");
        t1.start();
        //休眠5秒
        TimeUnit.SECONDS.sleep(5);
        t1.interrupt();

    }
}
```
输出：
```
1563598536736,t1 start!
t1,park()之前中断标志：false
t1,park()之后中断标志：true
1563598541736,t1 被唤醒!
```
t1线程中调用了`park()`方法让线程等待，主线程休眠了5秒之后，调用`t1.interrupt();`给线程t1发送中断信号，然后线程t1从等待中被唤醒了，输出结果中的1、4行结果相差5秒左右，刚好是主线程休眠了5秒之后将t1唤醒了。**结论：park方法可以相应线程中断。**<br />`**LockSupport.park**`**方法让线程等待之后，唤醒方式有2种：**

1. **调用**`**LockSupport.unpark**`**方法**
2. **调用等待线程的**`**interrupt()**`**方法，给等待的线程发送中断信号，可以唤醒线程**
<a name="En1Oj"></a>
### 示例4
LockSupport有几个阻塞放有一个blocker参数，这个参数什么意思，上一个实例代码，大家一看就懂了：
```java
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.LockSupport;

public class Demo10 {

    static class BlockerDemo {
    }

    public static void main(String[] args) throws InterruptedException {
        Thread t1 = new Thread(() -> {
            LockSupport.park();
        });
        t1.setName("t1");
        t1.start();

        Thread t2 = new Thread(() -> {
            LockSupport.park(new BlockerDemo());
        });
        t2.setName("t2");
        t2.start();
    }
}
```
运行上面代码，然后用jstack查看一下线程的堆栈信息：
```
"t2" #13 prio=5 os_prio=0 tid=0x00000000293ea800 nid=0x91e0 waiting on condition [0x0000000029c3f000]
   java.lang.Thread.State: WAITING (parking)
        at sun.misc.Unsafe.park(Native Method)
        - parking to wait for  <0x00000007180bfeb0> (a com.example.chat10.Demo10$BlockerDemo)
        at java.util.concurrent.locks.LockSupport.park(LockSupport.java:175)
        at com.example.chat10.Demo10.lambda$main$1(Demo10.java:22)
        at com.example.chat10.Demo10$$Lambda$2/824909230.run(Unknown Source)
        at java.lang.Thread.run(Thread.java:745)

"t1" #12 prio=5 os_prio=0 tid=0x00000000293ea000 nid=0x9d4 waiting on condition [0x0000000029b3f000]
   java.lang.Thread.State: WAITING (parking)
        at sun.misc.Unsafe.park(Native Method)
        at java.util.concurrent.locks.LockSupport.park(LockSupport.java:304)
        at com.example.chat10.Demo10.lambda$main$0(Demo10.java:16)
        at com.example.chat10.Demo10$$Lambda$1/1389133897.run(Unknown Source)
        at java.lang.Thread.run(Thread.java:745)
```
代码中，线程t1和t2的不同点是，t2中调用park方法传入了一个BlockerDemo对象，从上面的线程堆栈信息中，发现t2线程的堆栈信息中多了一行- `parking to wait for <0x00000007180bfeb0> (a com.example.chat10.Demo10$BlockerDemo)`，刚好是传入的BlockerDemo对象，park传入的这个参数可以在线程堆栈信息中方便排查问题，其他暂无他用。<br />`**LockSupport**`**的其他等待方法，包含有超时时间了，过了超时时间，等待方法会自动返回，让线程继续运行，这些方法在此就不提供示例了。**
<a name="sNpqB"></a>
## 线程等待和唤醒的3种方式做个对比
到目前为止，已经说了3种让线程等待和唤醒的方法了

1. 方式1：Object中的wait、notify、notifyAll方法
2. 方式2：juc中Condition接口提供的await、signal、signalAll方法
3. 方式3：juc中的LockSupport提供的park、unpark方法

**3种方式对比：**

|  | **Object** | **Condtion** | **LockSupport** |
| --- | --- | --- | --- |
| 前置条件 | 需要在synchronized中运行 | 需要先获取Lock的锁 | 无 |
| 无限等待 | 支持 | 支持 | 支持 |
| 超时等待 | 支持 | 支持 | 支持 |
| 等待到将来某个时间返回 | 不支持 | 支持 | 支持 |
| 等待状态中释放锁 | 会释放 | 会释放 | 不会释放 |
| 唤醒方法先于等待方法执行，能否唤醒线程 | 否 | 否 | 可以 |
| 是否能响应线程中断 | 是 | 是 | 是 |
| 线程中断是否会清除中断标志 | 是 | 是 | 否 |
| 是否支持等待状态中不响应中断 | 不支持 | 支持 | 不支持 |

