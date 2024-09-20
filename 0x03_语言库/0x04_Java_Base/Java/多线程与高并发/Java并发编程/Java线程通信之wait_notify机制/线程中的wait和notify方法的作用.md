Java
<a name="TMd2V"></a>
## 一、简介
`synchronized`和`volatile`关键字，使用它能解决线程同步的问题，但是它们无法解决线程之间协调和通信的问题。<br />举个简单的例子，比如线程 A 负责将 int 型变量 i 值累加操作到 10000，然后通知线程 B 负责把结果打印出来。<br />这个怎么实现呢？其中一个最简单的办法就是，线程 B 不断的通过轮询方式`while(i == 10000)`检查是否满足条件，这样就可以实现了。<br />虽然这种方式可以实现需求，但是也带来了另一个问题：线程 B 中的`while()`操作不会释放 CPU 资源，会导致 CPU 一直在这个方法上做判断操作，极大的浪费 CPU 资源。<br />CPU 资源是非常非常昂贵的，因为使用 CPU 资源不只是当前一个应用程序，还有其它许许多多的应用程序。如果把这些轮询的时间释放出来，给别的线程使用，更能显著提升应用程序的运行效率。比如，线程 A 操作完成之后，通知线程 B 进行后续的操作，线程 B 无需通过轮询检查的方式来完成线程之间的协调，这样是不是更好。<br />在 Java 的父类中，也就是Object类中，就有三个方法：`wait()`、`notify()`、`notifyAll()`，它们就可以实现线程之间的通信。<br />如果没有接触多线程，这些方法可能基本上使用不到。下面一起来看看它们的使用方式！
<a name="eYVb8"></a>
## 二、方法介绍
<a name="TWJFf"></a>
### `wait()`
`wait()`方法，顾名思义，表示等待的意思，它的作用是：使执行当前代码的线程进入阻塞状态，将当前线程置入"预执行队列"中，并且`wait()`所在的代码处停止执行，直到接到通知或被中断。<br />不过有个前提，**在调用**`**wait()**`**方法之前，线程必须获得该对象的锁，因此只能在**`**synchronized**`**修饰的同步方法/同步代码块中调用**`**wait()**`**方法；同时，**`**wait()**`**方法执行后，会立即释放获得的对象锁以便其它线程使用，当前线程被阻塞，进入等待状态**。<br />至于`wait()`为什么有阻塞的效果，其内部机制非常复杂，主要由 JVM 的 C 代码实现，大家了解就行。
<a name="tuGiQ"></a>
### `notify()`
`notify()`方法，顾名思义，表示通知的意思，它的作用是：让处于同一监视器下的等待线程被重新唤醒，如果有多个线程等待，那么随机挑选出一个等待的线程，对其发出通知`notify()`，并使它等待获取该对象的对象锁。<br />注意“等待获取该对象的对象锁”，这意味着即使收到了通知，等待的线程也不会马上获取对象锁，必须等待`notify()`方法的线程释放锁才可以。<br />调用环境和`wait()`一样，`notify()`也要在`synchronized`修饰的同步方法/同步代码块中调用。
<a name="oQrp0"></a>
### `notifyAll()`
`notifyAll()`方法，顾名思义，也是表示通知的意思，它的作用是：让所有处于同一监视器下的等待线程被重新唤醒，`notify()`方法只会随机的唤醒一个线程，而使用`notifyAll()`方法将一次性全部唤醒。<br />通常来说，`notifyAll()`方法更安全，因为当代码逻辑考虑不周的时候，使用`notify()`会导致只唤醒了一个线程，而其他线程可能永远等待下去醒不过来了。<br />调用环境和`notify()`一样，`notifyAll()`也要在`synchronized`修饰的同步方法/同步代码块中调用。<br />三个方法总结下来就是：

1. `wait()`方法，使线程阻塞，进入等待状态
2. `notify()`方法，唤醒处于等待的线程，如果有多个线程就随机从中取一个
3. `notifyAll()`方法，唤醒所有处于等待的线程
<a name="SR7Ai"></a>
### 2.1、wait/notify/notifyAll 使用介绍
通常`wait()`方法，一般与`notify()`或者`notifyAll()`搭配使用比较多。<br />下面看一个简单的示例。
```java
public class MyThreadA extends Thread{

    private Object lock;

    public MyThreadA(Object lock) {
        this.lock = lock;
    }

    @Override
    public void run() {
        synchronized (lock){
            System.out.println(DateUtil.format(new Date()) + " 当前线程：" + Thread.currentThread().getName() + " wait begin");
            try {
                // 进入阻塞等待
                lock.wait();
            } catch (Exception e) {
                e.printStackTrace();
            }
            System.out.println(DateUtil.format(new Date()) + " 当前线程：" + Thread.currentThread().getName() + " wait end");
        }
    }
}
```
```java
public class MyThreadB extends Thread{

    private Object lock;

    public MyThreadB(Object lock) {
        this.lock = lock;
    }

    @Override
    public void run() {
        synchronized (lock){
            System.out.println(DateUtil.format(new Date()) + " 当前线程：" + Thread.currentThread().getName() + " notify begin");
            // 唤醒其它等待线程
            lock.notify();
            System.out.println(DateUtil.format(new Date()) + " 当前线程：" + Thread.currentThread().getName() + " notify end");
        }
    }
}
```
```java
public class MyThreadTest {

    public static void main(String[] args) throws InterruptedException {
        Object lock = new Object();
        MyThreadA threadA = new MyThreadA(lock);
        threadA.start();

        //过3秒再启动下一个线程
        Thread.sleep(3000);

        MyThreadB threadB = new MyThreadB(lock);
        threadB.start();
    }
}
```
运行服务，输出结果如下：
```
2023-09-28 16:42:19 当前线程：Thread-0 wait begin
2023-09-28 16:42:22 当前线程：Thread-1 notify begin
2023-09-28 16:42:22 当前线程：Thread-1 notify end
2023-09-28 16:42:22 当前线程：Thread-0 wait end
```
从日志上可以得出，threadA线程先启动，然后进入阻塞状态，过了 3 秒之后，再启动threadB线程，运行结束之后，通知threadA线程可以获取对象锁，最后执行完毕。<br />整个线程之间的协调和通信，大体就是这样的。<br />假如把threadA线程数量增加到 5 个，再来看看运行效果。
```java
public class MyThreadTest {

    public static void main(String[] args) throws InterruptedException {
        Object lock = new Object();
        // 创建5个wait线程
        for (int i = 0; i < 5; i++) {
            MyThreadA threadA = new MyThreadA(lock);
            threadA.start();
        }

        //过3秒再启动下一个线程
        Thread.sleep(3000);

        MyThreadB threadB = new MyThreadB(lock);
        threadB.start();
    }
}
```
运行服务，输出结果如下：
```
2023-09-28 17:02:05 当前线程：Thread-0 wait begin
2023-09-28 17:02:05 当前线程：Thread-4 wait begin
2023-09-28 17:02:05 当前线程：Thread-3 wait begin
2023-09-28 17:02:05 当前线程：Thread-2 wait begin
2023-09-28 17:02:05 当前线程：Thread-1 wait begin
2023-09-28 17:02:08 当前线程：Thread-5 notify begin
2023-09-28 17:02:08 当前线程：Thread-5 notify end
2023-09-28 17:02:08 当前线程：Thread-0 wait end
```
从日志中，可以很清晰的看到，当多个线程处于等待状态时，调用`notify()`方法，只会唤醒其中一个等待的线程；**同时服务无法关闭，因为剩下的 4 个线程一直处于阻塞状态**。<br />假如把MyThreadB类中的`lock.notify()`方法改成`lock.notifyAll()`方法，再看看效果怎样。
```java
public class MyThreadB extends Thread{

    private Object lock;

    public MyThreadB(Object lock) {
        this.lock = lock;
    }

    @Override
    public void run() {
        synchronized (lock){
            System.out.println(DateUtil.format(new Date()) + " 当前线程：" + Thread.currentThread().getName() + " notify begin");
            // 唤醒所有等待的线程
            lock.notifyAll();
            System.out.println(DateUtil.format(new Date()) + " 当前线程：" + Thread.currentThread().getName() + " notify end");
        }
    }
}
```
运行服务，输出结果如下：
```
2023-09-28 17:18:13 当前线程：Thread-0 wait begin
2023-09-28 17:18:13 当前线程：Thread-4 wait begin
2023-09-28 17:18:13 当前线程：Thread-3 wait begin
2023-09-28 17:18:13 当前线程：Thread-2 wait begin
2023-09-28 17:18:13 当前线程：Thread-1 wait begin
2023-09-28 17:18:16 当前线程：Thread-5 notify begin
2023-09-28 17:18:16 当前线程：Thread-5 notify end
2023-09-28 17:18:16 当前线程：Thread-1 wait end
2023-09-28 17:18:16 当前线程：Thread-2 wait end
2023-09-28 17:18:16 当前线程：Thread-3 wait end
2023-09-28 17:18:16 当前线程：Thread-4 wait end
2023-09-28 17:18:16 当前线程：Thread-0 wait end
```
从日志上可以很清晰的看到，3 秒后所有处于等待的线程都被唤醒，并且服务运行结束。
<a name="cBDrV"></a>
### 2.2、wait 释放锁介绍
在多线程的编程中，任何时候都要关注锁，因为它对当前代码执行是否安全，发挥了重要的作用。<br />在上面提到，调用`wait()`方法，除了让线程进入阻塞，进入等待状态以外，还会释放锁。<br />可以看一个简单的示例就知道了。
```java
public class MyThreadA1 extends Thread{

    private Object lock;

    public MyThreadA1(Object lock) {
        this.lock = lock;
    }

    @Override
    public void run() {
        synchronized (lock){
            System.out.println(DateUtil.format(new Date()) + " 当前线程：" + Thread.currentThread().getName() + " wait begin");
            try {
                // 进入阻塞等待
                lock.wait();
            } catch (Exception e) {
                e.printStackTrace();
            }
            System.out.println(DateUtil.format(new Date()) + " 当前线程：" + Thread.currentThread().getName() + " wait end");
        }
    }
}
```
```java
public class MyThreadTest1 {

    public static void main(String[] args) throws InterruptedException {
        Object lock = new Object();

        // 创建两个调用wait的线程
        MyThreadA1 threadA1 = new MyThreadA1(lock);
        threadA1.start();

        MyThreadA1 threadA2 = new MyThreadA1(lock);
        threadA2.start();
    }
}
```
运行服务，输出结果如下：
```
2023-09-28 17:31:56 当前线程：Thread-0 wait begin
2023-09-28 17:31:56 当前线程：Thread-1 wait begin
```
从日志结果可以清晰的看出，两个线程中其中一个调用`lock.wait()`之后，进入了阻塞状态，同时把对象锁也释放掉了，另一个线程拿到锁并进入同步代码块内，所以看到两个线程都打印了wait begin。<br />在Thread类中也有一个`sleep()`方法可以让当前线程阻塞，但是它们之间是有区别的，`sleep()`方法不会让当前线程释放锁。<br />可以看一个简单的例子。
```java
public class MyThreadA1 extends Thread{

    private Object lock;

    public MyThreadA1(Object lock) {
        this.lock = lock;
    }

    @Override
    public void run() {
        synchronized (lock){
            System.out.println(DateUtil.format(new Date()) + " 当前线程：" + Thread.currentThread().getName() + " sleep begin");
            try {
                // 进入阻塞等待
                Thread.sleep(100);
            } catch (Exception e) {
                e.printStackTrace();
            }
            System.out.println(DateUtil.format(new Date()) + " 当前线程：" + Thread.currentThread().getName() + " sleep end");
        }
    }
}
```
```java
public class MyThreadTest1 {

    public static void main(String[] args) throws InterruptedException {
        Object lock = new Object();

        // 创建两个调用sleep的线程
        MyThreadA1 threadA1 = new MyThreadA1(lock);
        threadA1.start();

        MyThreadA1 threadA2 = new MyThreadA1(lock);
        threadA2.start();
    }
}
```
运行服务，输出结果如下：
```
2023-09-28 17:55:20 当前线程：Thread-0 sleep begin
2023-09-28 17:55:21 当前线程：Thread-0 sleep end
2023-09-28 17:55:21 当前线程：Thread-1 sleep begin
2023-09-28 17:55:21 当前线程：Thread-1 sleep end
```
从日志上看，线程没有交替执行，而是串性执行。
<a name="ZSer3"></a>
### 2.3、notify/notifyAll 不释放锁介绍
于此对应的还有`notify()`和`notifyAll()`， 调用`notify()`或者`notifyAll()`方法当前线程是不会释放锁的，只有当同步方法／同步代码块执行完毕，才会释放锁。<br />同样的，可以看一个简单的示例。
```java
public class MyThreadA2 extends Thread{

    private Object lock;

    public MyThreadA2(Object lock) {
        this.lock = lock;
    }

    @Override
    public void run() {
        synchronized (lock){
            System.out.println(DateUtil.format(new Date()) + " 当前线程：" + Thread.currentThread().getName() + " notify begin");
            // 唤醒其它等待线程
            lock.notify();
            System.out.println(DateUtil.format(new Date()) + " 当前线程：" + Thread.currentThread().getName() + " notify end");
        }
    }
}
```
```java
public class MyThreadTest2 {

    public static void main(String[] args) throws InterruptedException {
        Object lock = new Object();

        // 创建两个调用notify()的线程
        MyThreadA2 threadA1 = new MyThreadA2(lock);
        threadA1.start();

        MyThreadA2 threadA2 = new MyThreadA2(lock);
        threadA2.start();
    }
}
```
运行服务，输出结果如下：
```
2023-09-28 18:11:36 当前线程：Thread-0 notify begin
2023-09-28 18:11:36 当前线程：Thread-0 notify end
2023-09-28 18:11:36 当前线程：Thread-1 notify begin
2023-09-28 18:11:36 当前线程：Thread-1 notify end
```
从日志结果可以清晰的看出，两个线程没有交替执行，而是串行执行。
<a name="ylwtx"></a>
### 2.4、IllegalMonitorStateException 异常介绍
虽然`wait()`、`notify()`、`notifyAll()`方法是在 Object 类中，理论上每个类都可以直接调用，但不是每个地方都可以随便调用，如果调用这三个方法，不在同步方法/同步代码块中，程序运行时会直接抛一次抛异常java.lang.IllegalMonitorStateException。<br />下面看一个简单的示例就知道了。
```java
public class MyThreadTest3 {

    public static void main(String[] args) throws Exception {
        Object lock = new Object();
        lock.wait();
    }
}
```
运行程序，直接抛异常。
```
Exception in thread "main" java.lang.IllegalMonitorStateException
 at java.lang.Object.wait(Native Method)
 at java.lang.Object.wait(Object.java:502)
 at com.example.thread.e3.MyThreadTest3.main(MyThreadTest3.java:19)
```
换成`notify()`、`notifyAll()`，运行结果也是一样。
<a name="hxoIG"></a>
## 三、小结
主要围绕线程之间的协调和通信相关技术进行一些知识总结，使用Object类中的`wait()`、`notify()`、`notifyAll()`方法，可以实现线程之间的协调和通信，但是它们只有在`synchronized`修饰的同步方法/同步代码块才会生效。如果不在同步方法/同步代码块调用，会抛java.lang.IllegalMonitorStateException异常。
