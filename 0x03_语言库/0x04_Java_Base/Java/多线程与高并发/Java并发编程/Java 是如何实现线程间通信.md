Java 多线程<br />正常情况下，每个子线程完成各自的任务就可以结束了。不过有的时候，希望多个线程协同工作来完成某个任务，这时就涉及到了线程间通信了。<br />涉及到的知识点：

1. `thread.join()`,
2. `object.wait()`,
3. `object.notify()`,
4. `CountdownLatch`,
5. `CyclicBarrier`,
6. `FutureTask`,
7. `Callable` 。

下面从几个例子作为切入点来讲解下 Java 里有哪些方法来实现线程间通信。

1. 如何让两个线程依次执行？
2. 如何让 两个线程按照指定方式有序交叉运行呢？
3. 四个线程 A B C D，其中 D 要等到 A B C 全执行完毕后才执行，而且 A B C 是同步运行的
4. 三个运动员各自准备，等到三个人都准备好后，再一起跑
5. 子线程完成某件任务后，把得到的结果回传给主线程
<a name="M6Oxd"></a>
### 如何让两个线程依次执行？
假设有两个线程，一个是线程 A，另一个是线程 B，两个线程分别依次打印 1-3 三个数字即可。来看下代码：
```java
private static void demo1() {  
    Thread A = new Thread(new Runnable() {  
        @Override  
        public void run() {  
            printNumber("A");  
        }  
    });  
    Thread B = new Thread(new Runnable() {  
        @Override  
        public void run() {  
            printNumber("B");  
        }  
    });  
    A.start();  
    B.start();  
}  
```
其中的 `printNumber(String)` 实现如下，用来依次打印 1, 2, 3 三个数字：
```java
private static void printNumber(String threadName) {  
    int i=0;  
    while (i++ < 3) {  
        try {  
            Thread.sleep(100);  
        } catch (InterruptedException e) {  
            e.printStackTrace();  
        }  
        System.out.println(threadName + " print: " + i);  
    }  
}  
```
这时得到的结果是：
```java
B print: 1 A print: 1 B print: 2 A print: 2 B print: 3 A print: 3
```
可以看到 A 和 B 是同时打印的。<br />那么，如果希望 B 在 A 全部打印 完后再开始打印呢？可以利用 `thread.join()` 方法，代码如下:
```java
private static void demo2() {  
    Thread A = new Thread(new Runnable() {  
        @Override  
        public void run() {  
            printNumber("A");  
        }  
    });  
    Thread B = new Thread(new Runnable() {  
        @Override  
        public void run() {  
            System.out.println("B 开始等待 A");  
            try {  
                A.join();  
            } catch (InterruptedException e) {  
                e.printStackTrace();  
            }  
            printNumber("B");  
        }  
    });  
    B.start();  
    A.start();  
}  
```
得到的结果如下：
```java
B 开始等待 A A print: 1 A print: 2 A print: 3
B print: 1 B print: 2 B print: 3
```
所以能看到 `A.join()` 方法会让 B 一直等待直到 A 运行完毕。
<a name="KaWQN"></a>
### 如何让 两个线程按照指定方式有序交叉运行呢？
还是上面那个例子，现在希望 A 在打印完 1 后，再让 B 打印 1, 2, 3，最后再回到 A 继续打印 2, 3。这种需求下，显然 `Thread.join()` 已经不能满足了。需要更细粒度的锁来控制执行顺序。<br />这里，可以利用 `object.wait()` 和 `object.notify()` 两个方法来实现。代码如下：
```java
/**  
 * A 1, B 1, B 2, B 3, A 2, A 3  
 */  
private static void demo3() {  
    Object lock = new Object();  
    Thread A = new Thread(new Runnable() {  
        @Override  
        public void run() {  
            synchronized (lock) {  
                System.out.println("A 1");  
                try {  
                    lock.wait();  
                } catch (InterruptedException e) {  
                    e.printStackTrace();  
                }  
                System.out.println("A 2");  
                System.out.println("A 3");  
            }  
        }  
    });  
    Thread B = new Thread(new Runnable() {  
        @Override  
        public void run() {  
            synchronized (lock) {  
                System.out.println("B 1");  
                System.out.println("B 2");  
                System.out.println("B 3");  
                lock.notify();  
            }  
        }  
    });  
    A.start();  
    B.start();  
}  
```
打印结果如下：
```java
A 1 A waiting…
B 1 B 2 B 3 A 2 A 3
```
正是期望的结果。<br />那么，这个过程发生了什么呢？

1. 首先创建一个 A 和 B 共享的对象锁 `lock = new Object();`
2. 当 A 得到锁后，先打印 1，然后调用 `lock.wait()` 方法，交出锁的控制权，进入 `wait` 状态；
3. 对 B 而言，由于 A 最开始得到了锁，导致 B 无法执行；直到 A 调用 `lock.wait()` 释放控制权后， B 才得到了锁；
4. B 在得到锁后打印 1， 2， 3；然后调用 `lock.notify()` 方法，唤醒正在 `wait` 的 A;
5. A 被唤醒后，继续打印剩下的 2，3。

为了更好理解，在上面的代码里加上 log 方便查看。
```java
private static void demo3() {  
    Object lock = new Object();  
    Thread A = new Thread(new Runnable() {  
        @Override  
        public void run() {  
            System.out.println("INFO: A 等待锁 ");  
            synchronized (lock) {  
                System.out.println("INFO: A 得到了锁 lock");  
                System.out.println("A 1");  
                try {  
                    System.out.println("INFO: A 准备进入等待状态，放弃锁 lock 的控制权 ");  
                    lock.wait();  
                } catch (InterruptedException e) {  
                    e.printStackTrace();  
                }  
                System.out.println("INFO: 有人唤醒了 A, A 重新获得锁 lock");  
                System.out.println("A 2");  
                System.out.println("A 3");  
            }  
        }  
    });  
    Thread B = new Thread(new Runnable() {  
        @Override  
        public void run() {  
            System.out.println("INFO: B 等待锁 ");  
            synchronized (lock) {  
                System.out.println("INFO: B 得到了锁 lock");  
                System.out.println("B 1");  
                System.out.println("B 2");  
                System.out.println("B 3");  
                System.out.println("INFO: B 打印完毕，调用 notify 方法 ");  
                lock.notify();  
            }  
        }  
    });  
    A.start();  
    B.start();  
}
```
打印结果如下:
```java
INFO: A 等待锁 INFO: A 得到了锁 lock A 1 INFO: A 准备进入等待状态，调用 lock.wait() 放弃锁 lock 的控制权 INFO: B 等待锁 INFO: B 得到了锁 lock B 1 B 2 B 3 INFO: B 打印完毕，调用 lock.notify() 方法 INFO: 有人唤醒了 A, A 重新获得锁 lock A 2 A 3
```
<a name="VX7cf"></a>
### 四个线程 A B C D，其中 D 要等到 A B C 全执行完毕后才执行，而且 A B C 是同步运行的
最开始介绍了 `thread.join()`，可以让一个线程等另一个线程运行完毕后再继续执行，那可以在 D 线程里依次 join A B C，不过这也就使得 A B C 必须依次执行，而要的是这三者能同步运行。<br />或者说，希望达到的目的是：A B C 三个线程同时运行，各自独立运行完后通知 D；对 D 而言，只要 A B C 都运行完了，D 再开始运行。针对这种情况，可以利用 CountdownLatch 来实现这类通信方式。它的基本用法是：

1. 创建一个计数器，设置初始值，`CountdownLatch countDownLatch = new CountDownLatch(2);`
2. 在 等待线程 里调用 `countDownLatch.await()` 方法，进入等待状态，直到计数值变成 0；
3. 在 其他线程 里，调用 `countDownLatch.countDown()` 方法，该方法会将计数值减小 1；
4. 当 其他线程 的 `countDown()` 方法把计数值变成 0 时，等待线程 里的 `countDownLatch.await()` 立即退出，继续执行下面的代码。

实现代码如下：
```java
private static void runDAfterABC() {  
    int worker = 3;  
    CountDownLatch countDownLatch = new CountDownLatch(worker);  
    new Thread(new Runnable() {  
        @Override  
        public void run() {  
            System.out.println("D is waiting for other three threads");  
            try {  
                countDownLatch.await();  
                System.out.println("All done, D starts working");  
            } catch (InterruptedException e) {  
                e.printStackTrace();  
            }  
        }  
    }).start();  
    for (char threadName='A'; threadName <= 'C'; threadName++) {  
        final String tN = String.valueOf(threadName);  
        new Thread(new Runnable() {  
            @Override  
            public void run() {  
                System.out.println(tN + " is working");  
                try {  
                    Thread.sleep(100);  
                } catch (Exception e) {  
                    e.printStackTrace();  
                }  
                System.out.println(tN + " finished");  
                countDownLatch.countDown();  
            }  
        }).start();  
    }  
}  
```
下面是运行结果：
```java
D is waiting for other three threads A is working B is working C is working
A finished C finished B finished All done, D starts working
```
其实简单点来说，`CountDownLatch` 就是一个倒计数器，把初始计数值设置为3，当 D 运行时，先调用 `countDownLatch.await()` 检查计数器值是否为 0，若不为 0 则保持等待状态；当A B C 各自运行完后都会利用`countDownLatch.countDown()`，将倒计数器减 1，当三个都运行完后，计数器被减至 0；此时立即触发 D 的 `await()` 运行结束，继续向下执行。<br />因此，`CountDownLatch` 适用于一个线程去等待多个线程的情况。
<a name="NoVva"></a>
### 三个运动员各自准备，等到三个人都准备好后，再一起跑
上面是一个形象的比喻，针对 线程 A B C 各自开始准备，直到三者都准备完毕，然后再同时运行 。也就是要实现一种 线程之间互相等待 的效果，那应该怎么来实现呢？<br />上面的 `CountDownLatch` 可以用来倒计数，但当计数完毕，只有一个线程的 `await()` 会得到响应，无法让多个线程同时触发。<br />为了实现线程间互相等待这种需求，可以利用 `CyclicBarrier` 数据结构，它的基本用法是：

1. 先创建一个公共 `CyclicBarrier` 对象，设置 同时等待 的线程数，`CyclicBarrier cyclicBarrier = new CyclicBarrier(3);`
2. 这些线程同时开始自己做准备，自身准备完毕后，需要等待别人准备完毕，这时调用 `cyclicBarrier.await();` 即可开始等待别人；
3. 当指定的 同时等待 的线程数都调用了 `cyclicBarrier.await();`时，意味着这些线程都准备完毕好，然后这些线程才 同时继续执行。

实现代码如下，设想有三个跑步运动员，各自准备好后等待其他人，全部准备好后才开始跑：
```java
private static void runABCWhenAllReady() {  
    int runner = 3;  
    CyclicBarrier cyclicBarrier = new CyclicBarrier(runner);  
    final Random random = new Random();  
    for (char runnerName='A'; runnerName <= 'C'; runnerName++) {  
        final String rN = String.valueOf(runnerName);  
        new Thread(new Runnable() {  
            @Override  
            public void run() {  
                long prepareTime = random.nextInt(10000) + 100;  
                System.out.println(rN + " is preparing for time: " + prepareTime);  
                try {  
                    Thread.sleep(prepareTime);  
                } catch (Exception e) {  
                    e.printStackTrace();  
                }  
                try {  
                    System.out.println(rN + " is prepared, waiting for others");  
                    cyclicBarrier.await(); // 当前运动员准备完毕，等待别人准备好  
                } catch (InterruptedException e) {  
                    e.printStackTrace();  
                } catch (BrokenBarrierException e) {  
                    e.printStackTrace();  
                }  
                System.out.println(rN + " starts running"); // 所有运动员都准备好了，一起开始跑  
            }  
        }).start();  
    }  
}  
```
打印的结果如下：
```java
A is preparing for time: 4131 B is preparing for time: 6349 C is preparing for time: 8206 A is prepared, waiting for others B is prepared, waiting for others C is prepared, waiting for others C starts running A starts running B starts running
```
<a name="N70D6"></a>
### 子线程完成某件任务后，把得到的结果回传给主线程
实际的开发中，经常要创建子线程来做一些耗时任务，然后把任务执行结果回传给主线程使用，这种情况在 Java 里要如何实现呢？<br />回顾线程的创建，一般会把 Runnable 对象传给 Thread 去执行。Runnable定义如下：
```java
public interface Runnable {  
    public abstract void run();  
}
```
可以看到 `run()` 在执行完后不会返回任何结果。那如果希望返回结果呢？这里可以利用另一个类似的接口类 `Callable`：
```java
@FunctionalInterface  
public interface Callable<V> {  
    /**  
     * Computes a result, or throws an exception if unable to do so.  
     *  
     * @return computed result  
     * @throws Exception if unable to compute a result  
     */  
    V call() throws Exception;  
}
```
可以看出 `Callable` 最大区别就是返回范型 V 结果。<br />那么下一个问题就是，如何把子线程的结果回传回来呢？在 Java 里，有一个类是配合 Callable 使用的：`FutureTask`，不过注意，它获取结果的 get 方法会阻塞主线程。<br />举例，让子线程去计算从 1 加到 100，并把算出的结果返回到主线程。
```java
private static void doTaskWithResultInWorker() {  
    Callable<Integer> callable = new Callable<Integer>() {  
        @Override  
        public Integer call() throws Exception {  
            System.out.println("Task starts");  
            Thread.sleep(1000);  
            int result = 0;  
            for (int i=0; i<=100; i++) {  
                result += i;  
            }  
            System.out.println("Task finished and return result");  
            return result;  
        }  
    };  
    FutureTask<Integer> futureTask = new FutureTask<>(callable);  
    new Thread(futureTask).start();  
    try {  
        System.out.println("Before futureTask.get()");  
        System.out.println("Result: " + futureTask.get());  
        System.out.println("After futureTask.get()");  
    } catch (InterruptedException e) {  
        e.printStackTrace();  
    } catch (ExecutionException e) {  
        e.printStackTrace();  
    }  
}  
```
打印结果如下：
```java
Before futureTask.get() Task starts Task finished and return result Result: 5050 After futureTask.get()
```
可以看到，主线程调用 `futureTask.get()` 方法时阻塞主线程；然后 `Callable` 内部开始执行，并返回运算结果；此时 `futureTask.get()` 得到结果，主线程恢复运行。<br />这里可以学到，通过 `FutureTask` 和 `Callable` 可以直接在主线程获得子线程的运算结果，只不过需要阻塞主线程。当然，如果不希望阻塞主线程，可以考虑利用 `ExecutorService`，把 `FutureTask` 放到线程池去管理执行。
