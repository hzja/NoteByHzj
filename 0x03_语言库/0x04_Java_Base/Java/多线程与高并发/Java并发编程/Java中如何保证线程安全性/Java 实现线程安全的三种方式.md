Java线程安全<br />一个程序在运行起来的时候会转换成进程，通常含有多个线程。<br />通常情况下，一个进程中的比较耗时的操作（如长循环、文件上传下载、网络资源获取等），往往会采用多线程来解决。<br />比如显示生活中，银行取钱问题、火车票多个售票窗口的问题，通常会涉及到并发的问题，从而需要多线程的技术。<br />当进程中有多个并发线程进入一个重要数据的代码块时，在修改数据的过程中，很有可能引发线程安全问题，从而造成数据异常。例如，正常逻辑下，同一个编号的火车票只能售出一次，却由于线程安全问题而被多次售出，从而引起实际业务异常。现在就以售票问题来演示线程安全的问题
<a name="pbQPR"></a>
## 在不对多线程数据进行保护的情况下会引发的状况
```java
public class ThreadUnSecurity {
    
    static int tickets = 10;
    
    class SellTickets implements Runnable{

        @Override
        public void run() {
            // 未加同步时产生脏数据
            while(tickets > 0) {
                
                System.out.println(Thread.currentThread().getName()+"--->售出第：  "+tickets+" 票");
                tickets--;
                
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                
            }
            
            if (tickets <= 0) {
                
                System.out.println(Thread.currentThread().getName()+"--->售票结束！");
            }
        }
    }
    
    
    public static void main(String[] args) {
        
        
        SellTickets sell = new ThreadUnSecurity().new SellTickets();
        
        Thread thread1 = new Thread(sell, "1号窗口");
        Thread thread2 = new Thread(sell, "2号窗口");
        Thread thread3 = new Thread(sell, "3号窗口");
        Thread thread4 = new Thread(sell, "4号窗口");
        
        thread1.start();
        thread2.start();
        thread3.start();
        thread4.start();
        
        
    }
}
```
上述代码运行的结果：
```java
1号窗口--->售出第：  10 票
3号窗口--->售出第：  10 票
2号窗口--->售出第：  10 票
4号窗口--->售出第：  10 票
2号窗口--->售出第：  6 票
1号窗口--->售出第：  5 票
3号窗口--->售出第：  4 票
4号窗口--->售出第：  3 票
2号窗口--->售出第：  2 票
4号窗口--->售出第：  1 票
1号窗口--->售出第：  1 票
3号窗口--->售票结束！
2号窗口--->售票结束！
1号窗口--->售票结束！
4号窗口--->售票结束！
```
可以看出同一张票在不对票数进行保护时会出现同一张票会被出售多次！由于线程调度中的不确定性，在演示上述代码时，出现的运行结果会有不同。
<a name="T4cyy"></a>
### 第一种方式：同步代码块
```java
package com.bpan.spring.beans.thread;

import com.sun.org.apache.regexp.internal.recompile;

public class ThreadSynchronizedSecurity {
    
    static int tickets = 10;
    
    class SellTickets implements Runnable{

        @Override
        public void run() {
            // 同步代码块
            while(tickets > 0) {
                
                synchronized (this) {
                    
//                    System.out.println(this.getClass().getName().toString());
                    
                    if (tickets <= 0) {
                        
                        return;
                    }
                    
                    System.out.println(Thread.currentThread().getName()+"--->售出第：  "+tickets+" 票");
                    tickets--;
                    
                    try {
                        Thread.sleep(100);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
                
                if (tickets <= 0) {
                    
                    System.out.println(Thread.currentThread().getName()+"--->售票结束！");
                }
            }
        }
    }
    
    
    public static void main(String[] args) {
        
        
        SellTickets sell = new ThreadSynchronizedSecurity().new SellTickets();
        
        Thread thread1 = new Thread(sell, "1号窗口");
        Thread thread2 = new Thread(sell, "2号窗口");
        Thread thread3 = new Thread(sell, "3号窗口");
        Thread thread4 = new Thread(sell, "4号窗口");
        
        thread1.start();
        thread2.start();
        thread3.start();
        thread4.start();
        
        
    }
    

}
```
输出结果读者可自行调试，不会出现同一张票被出售多次的情况。
<a name="SDVRH"></a>
### 第二种方式：同步方法
```java
package com.bpan.spring.beans.thread;

public class ThreadSynchroniazedMethodSecurity {
    
    
    static int tickets = 10;
    
    class SellTickets implements Runnable{

        @Override
        public void run() {
            //同步方法
            while (tickets > 0) {
                
                synMethod();
                
                try {
                    Thread.sleep(100);
                } catch (InterruptedException e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                }
                
                if (tickets<=0) {
                    
                    System.out.println(Thread.currentThread().getName()+"--->售票结束");
                }
                
            }
            
            
        }
        
        synchronized void synMethod() {
            
            synchronized (this) {
                if (tickets <=0) {
                    
                    return;
                }
                
                System.out.println(Thread.currentThread().getName()+"---->售出第 "+tickets+" 票 ");
                tickets-- ;
            }
            
        }
        
    }
    public static void main(String[] args) {
        
        
        SellTickets sell = new ThreadSynchroniazedMethodSecurity().new SellTickets();
        
        Thread thread1 = new Thread(sell, "1号窗口");
        Thread thread2 = new Thread(sell, "2号窗口");
        Thread thread3 = new Thread(sell, "3号窗口");
        Thread thread4 = new Thread(sell, "4号窗口");
        
        thread1.start();
        thread2.start();
        thread3.start();
        thread4.start();
        
    }

}
```
可自行调试上述代码的运行结果。
<a name="pxevS"></a>
### 第三种方式：Lock锁机制
通过创建Lock对象，采用`lock()`加锁，`unlock()`解锁，来保护指定的代码块
```java
package com.bpan.spring.beans.thread;

import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class ThreadLockSecurity {
    
    static int tickets = 10;
    
    class SellTickets implements Runnable{
        
        Lock lock = new ReentrantLock();

        @Override
        public void run() {
            // Lock锁机制
            while(tickets > 0) {
                
                try {
                    lock.lock();
                    
                    if (tickets <= 0) {
                        
                        return;
                    }
                        
                    System.out.println(Thread.currentThread().getName()+"--->售出第：  "+tickets+" 票");
                    tickets--;
                } catch (Exception e1) {
                    // TODO Auto-generated catch block
                    e1.printStackTrace();
                }finally {
                    
                    lock.unlock();
                    try {
                        Thread.sleep(100);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
                
            if (tickets <= 0) {
                
                System.out.println(Thread.currentThread().getName()+"--->售票结束！");
            }
            
        }
    }
    
    
    public static void main(String[] args) {
        
        
        SellTickets sell = new ThreadLockSecurity().new SellTickets();
        
        Thread thread1 = new Thread(sell, "1号窗口");
        Thread thread2 = new Thread(sell, "2号窗口");
        Thread thread3 = new Thread(sell, "3号窗口");
        Thread thread4 = new Thread(sell, "4号窗口");
        
        thread1.start();
        thread2.start();
        thread3.start();
        thread4.start();
        
    }
}
```
<a name="ePSSw"></a>
## 最后总结
由于`synchronized`是在JVM层面实现的，因此系统可以监控锁的释放与否；而`ReentrantLock`是使用代码实现的，系统无法自动释放锁，需要在代码中的`finally`子句中显式释放锁`lock.unlock()`。<br />另外，在并发量比较小的情况下，使用`synchronized`是个不错的选择；但是在并发量比较高的情况下，其性能下降会很严重，此时`ReentrantLock`是个不错的方案。
<a name="w6H3L"></a>
## 补充
在使用`synchronized` 代码块时，可以与`wait()`、`notify()`、`nitifyAll()`一起使用，从而进一步实现线程的通信。<br />其中，`wait()`方法会释放占有的对象锁，当前线程进入等待池，释放cpu，而其他正在等待的线程即可抢占此锁，获得锁的线程即可运行程序；<br />线程的`sleep()`方法则表示，当前线程会休眠一段时间，休眠期间，会暂时释放cpu，但并不释放对象锁，也就是说，在休眠期间，其他线程依然无法进入被同步保护的代码内部，当前线程休眠结束时，会重新获得cpu执行权，从而执行被同步保护的代码。<br />`wait()`和`sleep()`最大的不同在于`wait()`会释放对象锁，而`sleep()`不会释放对象锁。<br />`notify()`方法会唤醒因为调用对象的`wait()`而处于等待状态的线程，从而使得该线程有机会获取对象锁。调用`notify()`后，当前线程并不会立即释放锁，而是继续执行当前代码，直到`synchronized`中的代码全部执行完毕，才会释放对象锁。JVM会在等待的线程中调度一个线程去获得对象锁，执行代码。<br />需要注意的是，`wait()`和`notify()`必须在`synchronized`代码块中调用。<br />`notifyAll()`是唤醒所有等待的线程。<br />下面是示例代码，
```java
package com.bpan.spring.beans.thread;

public class ThreadDemo {
    
    static final Object obj = new Object();
    
    //第一个子线程
    static class ThreadA implements Runnable{

        @Override
        public void run() {
            
            
            int count = 10;
            while(count > 0) {
                
                synchronized (ThreadDemo.obj) {
                    
                    System.out.println("A-----"+count);
                    count--;
                    
                    synchronized (ThreadDemo.obj) {
                        
                        //notify()方法会唤醒因为调用对象的wait()而处于等待状态的线程，从而使得该线程有机会获取对象锁。
                        //调用notify()后，当前线程并不会立即释放锁，而是继续执行当前代码，直到synchronized中的代码全部执行完毕，
                        ThreadDemo.obj.notify();
                        
                        try {
                            ThreadDemo.obj.wait();
                        } catch (InterruptedException e) {
                            // TODO Auto-generated catch block
                            e.printStackTrace();
                        }
                    }
                }
            }
            
        }
        
    }
    
    static class ThreadB implements Runnable{
        
        
        @Override
        public void run() {
            
            int count = 10;
            
            while(count > 0) {
                
                synchronized (ThreadDemo.obj) {
                    System.out.println("B-----"+count);
                    count--;
                    
                    synchronized (ThreadDemo.obj) {
                    
                        //notify()方法会唤醒因为调用对象的wait()而处于等待状态的线程，从而使得该线程有机会获取对象锁。
                        //调用notify()后，当前线程并不会立即释放锁，而是继续执行当前代码，直到synchronized中的代码全部执行完毕，
                        ThreadDemo.obj.notify();
                        
                        try {
                            ThreadDemo.obj.wait();
                        } catch (InterruptedException e) {
                            // TODO Auto-generated catch block
                            e.printStackTrace();
                        }
                    }
                    
                }
                
            }
            
        }
        
    }
    
    public static void main(String[] args) {
        new Thread(new ThreadA()).start();
        new Thread(new ThreadB()).start();
    }
}
```
