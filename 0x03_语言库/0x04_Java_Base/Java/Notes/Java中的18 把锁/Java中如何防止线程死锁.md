Java<br />每当两个或多个线程等待对方完成对资源的访问时，就会发生死锁。本编文章讨论了死锁、导致线程死锁的原因以及程序员如何在Java中防止线程死锁。
<a name="mFIlY"></a>
## 什么是死锁？
在计算中，当两个或多个并发操作等待彼此完成时，就会发生死锁。换句话说，当两个线程因为等待另一个线程放弃锁而永远阻塞对方时，就会发生死锁。当两个线程共享一个资源并且都在等待获取另一个线程持有的共享资源的锁时，通常会出现这种情况。<br />要发生死锁，必须满足以下条件：

- 至少有一个资源必须是互斥的（如互斥），这样只有一个线程可以同时访问它。
- 保持并等待：线程必须保持一个资源，同时等待另一个资源。
- 无抢占：一旦资源被线程获取，就不能强制移除该资源上的锁（即，该锁不能被抢占）。
- 循环等待：每个线程必须以循环方式等待另一个资源。
<a name="FUoku"></a>
## 如何避免Java中的死锁
Java提供了各种方法来避免线程死锁，例如使用同步块、使用线程安全集合和使用原子操作。
<a name="V9qMG"></a>
### 使用`Thread.join()`
程序员可以通过几种方式避免Java中的死锁。首先，可以使用`Thread.join()`方法。可以使用`Thread.join()`来确保一个线程在启动另一个线程之前完成。<br />例如，当一个线程正在读取文件，而另一个线程在写入同一文件时。因此，不会出现死锁。
<a name="SaRFG"></a>
### 使用`Synchronization`关键字
通过同步和使用同步原语可以避免死锁。使用同步对象（如互斥锁或信号量）是防止死锁的另一种方法。这可以防止多个线程争夺同一资源上的锁而导致的死锁。<br />始终确保以固定的顺序使用同步块，以避免Java中的死锁。这意味着，如果多个线程试图访问相同的资源，它们应该始终以相同的顺序获取资源的锁。此外，为了防止死锁，避免嵌套的同步块非常重要。
<a name="C3KwW"></a>
## 避免嵌套锁
开发人员还可以通过避免嵌套锁来避免死锁，即，在对象上的锁已经获取时，避免获取另一个锁。<br />还可以通过实现获取锁的超时策略并确保跨不同线程以相同的顺序访问资源来避免死锁情况。
<a name="Slhrf"></a>
## 避免在不需要时使用锁
只有在绝对必要时才能获取锁，并应尽快释放。如果一个线程获得了它不需要的锁，那么其他线程可能会被不必要地阻塞。<br />为了避免不必要的锁，了解每个线程正在访问的资源及其持有的锁非常重要。
<a name="JTWFR"></a>
## 规范的正确设计
此外，可以设计代码，使死锁永远不会发生。此外，应用程序的设计应确保线程之间没有循环等待依赖关系。<br />使用线程安全类和数据结构来降低Java应用程序中线程死锁的风险。<br />当执行多个任务时，程序员应该建立一个主任务，以指定的顺序执行一系列子任务。<br />通过这种方式，可以确保没有两个线程试图同时获得相同的锁，从而防止出现任何死锁。
<a name="lw5Ie"></a>
## Java中死锁的例子
以下代码示例说明了Java中的死锁情况：
```java
public class MyThreadDeadlockDemo {
    public static Object lockObjectA = new Object();
    public static Object lockObjectB = new Object();
    public static void main(String args[]) {
        MyThreadClassA threadObjectA = new MyThreadClassA();
        MyThreadClassB threadObjectB = new MyThreadClassB();

        threadObjectA.start();
        threadObjectB.start();
    }

    private static class MyThreadClassA extends Thread {
        public void run() {
            synchronized(lockObjectA) {
                System.out.println("Thread A: 获取锁 A");

                try {
                    Thread.sleep(100);
                } catch (Exception ex) {}
                System.out.println("Thread A: 等待锁 B");
                synchronized(lockObjectB) {
                    System.out.println("Thread A: 获取A和B的锁");
                }
            }
        }
    }
    private static class MyThreadClassB extends Thread {
        public void run() {
            synchronized(lockObjectB) {
                System.out.println("Thread B: 获取锁 B");

                try {
                    Thread.sleep(100);
                } catch (Exception ex) {}
                System.out.println("Thread B: 等待锁 A");

                synchronized(lockObjectA) {
                    System.out.println("Thread B: 获取A和B的锁");
                }
            }
        }
    }
}
```
要解决上述代码示例中的死锁问题，只需更改MyThreadClassB类的`run`方法中锁的顺序，如下面给出的代码片段所示：
```java
public void run() {
    synchronized (lockObjectA) {
        System.out.println("Thread B: 获得锁 B");
        try 
        { 
            Thread.sleep(100); 
        }
        catch (Exception ex) {}
        System.out.println("Thread B:等待锁 A");
                        
        synchronized (lockObjectB) {
            System.out.println("Thread B: 获取A和B的锁");
        }
    }
}
```
下面给出了完整的Java代码，供参考：
```java
public class MyThreadDeadlockDemo {
    public static Object lockObjectA = new Object();
    public static Object lockObjectB = new Object();

    public static void main(String args[]) {
        MyThreadClassA threadObjectA = new MyThreadClassA();
        MyThreadClassB threadObjectB = new MyThreadClassB();

        threadObjectA.start();
        threadObjectB.start();
    }

    private static class MyThreadClassA extends Thread {
        public void run() {
            synchronized(lockObjectA) {
                System.out.println("Thread A: 获得锁 A");

                try {
                    Thread.sleep(100);
                } catch (Exception ex) {}
                System.out.println("Thread A: 等待锁 B");

                synchronized(lockObjectB) {
                    System.out.println("Thread A: 获取A和B的锁");
                }
            }
        }
    }

    private static class MyThreadClassB extends Thread {
        public void run() {
            synchronized(lockObjectA) {
                System.out.println("Thread B: 获得锁 B");

                try {
                    Thread.sleep(100);
                } catch (Exception ex) {}
                System.out.println("Thread B: 等待锁 A");

                synchronized(lockObjectB) {
                    System.out.println("Thread B: 获取A和B的锁");
                }
            }
        }
    }
}
```
注意：如何在MyThreadClassB类的run方法中按顺序获取锁以防止死锁。
<a name="STWDs"></a>
## 防止Java死锁的最后思考
线程死锁是一个主要问题，它会导致Java程序冻结并变得无响应。<br />但是，开发人员可以遵循本教程中概述的最佳实践来避免死锁。<br />还应该监视应用程序中等待时间过长的线程，并采取措施来识别潜在的死锁。
