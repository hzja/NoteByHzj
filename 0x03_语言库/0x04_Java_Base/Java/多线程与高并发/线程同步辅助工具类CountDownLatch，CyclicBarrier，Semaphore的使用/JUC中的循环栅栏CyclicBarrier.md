Java
<a name="EjGUk"></a>
## 主要内容

1. **介绍**`**CyclicBarrier**`
2. **6个示例介绍**`**CyclicBarrier**`**的使用**
3. **对比**`**CyclicBarrier**`**和**`**CountDownLatch**`
<a name="rtVef"></a>
## `CyclicBarrier`简介
`CyclicBarrier`通常称为循环屏障。它和`CountDownLatch`很相似，都可以使线程先等待然后再执行。不过`CountDownLatch`是使一批线程等待另一批线程执行完后再执行；而`CyclicBarrier`只是使等待的线程达到一定数目后再让它们继续执行。故而`CyclicBarrier`内部也有一个计数器，计数器的初始值在创建对象时通过构造参数指定，如下所示：
```java
public CyclicBarrier(int parties) {
    this(parties, null);
}
```
每调用一次`await()`方法都将使阻塞的线程数+1，只有阻塞的线程数达到设定值时屏障才会打开，允许阻塞的所有线程继续执行。除此之外，`CyclicBarrier`还有几点需要注意的地方:

- `CyclicBarrier`的计数器可以重置而`CountDownLatch`不行，这意味着`CyclicBarrier`实例可以被重复使用而`CountDownLatch`只能被使用一次。而这也是循环屏障循环二字的语义所在。
- `CyclicBarrier`允许用户自定义`barrierAction`操作，这是个可选操作，可以在创建`CyclicBarrier`对象时指定
```java
public CyclicBarrier(int parties, Runnable barrierAction) {
    if (parties <= 0) throw new IllegalArgumentException();
    this.parties = parties;
    this.count = parties;
    this.barrierCommand = barrierAction;
}
```
一旦用户在创建`CyclicBarrier`对象时设置了`barrierAction`参数，则在阻塞线程数达到设定值屏障打开前，会调用`barrierAction`的`run()`方法完成用户自定义的操作。
<a name="kSQu0"></a>
## 示例1：简单使用CyclicBarrier
公司组织旅游，大家都有经历过，10个人，中午到饭点了，需要等到10个人都到了才能开饭，先到的人坐那等着，代码如下：
```java
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.TimeUnit;

public class Demo1 {
    public static CyclicBarrier cyclicBarrier = new CyclicBarrier(10);

    public static class T extends Thread {
        int sleep;

        public T(String name, int sleep) {
            super(name);
            this.sleep = sleep;
        }

        @Override
        public void run() {
            try {
                //模拟休眠
                TimeUnit.SECONDS.sleep(sleep);
                long starTime = System.currentTimeMillis();
                //调用await()的时候，当前线程将会被阻塞，需要等待其他员工都到达await了才能继续
                cyclicBarrier.await();
                long endTime = System.currentTimeMillis();
                System.out.println(this.getName() + ",sleep:" + this.sleep + " 等待了" + (endTime - starTime) + "(ms),开始吃饭了！");
            } catch (InterruptedException e) {
                e.printStackTrace();
            } catch (BrokenBarrierException e) {
                e.printStackTrace();
            }
        }
    }

    public static void main(String[] args) throws InterruptedException {
        for (int i = 1; i <= 10; i++) {
            new T("员工" + i, i).start();
        }
    }
}
```
输出：
```
员工1,sleep:1 等待了9000(ms),开始吃饭了！
员工9,sleep:9 等待了1000(ms),开始吃饭了！
员工8,sleep:8 等待了2001(ms),开始吃饭了！
员工7,sleep:7 等待了3001(ms),开始吃饭了！
员工6,sleep:6 等待了4001(ms),开始吃饭了！
员工4,sleep:4 等待了6000(ms),开始吃饭了！
员工5,sleep:5 等待了5000(ms),开始吃饭了！
员工10,sleep:10 等待了0(ms),开始吃饭了！
员工2,sleep:2 等待了7999(ms),开始吃饭了！
员工3,sleep:3 等待了7000(ms),开始吃饭了！
```
代码中模拟了10个员工上桌吃饭的场景，等待所有员工都到齐了才能开发，可以看到第10个员工最慢，前面的都在等待第10个员工，员工1等待了9秒，上面代码中调用`cyclicBarrier.await();`会让当前线程等待。当10个员工都调用了`cyclicBarrier.await();`之后，所有处于等待中的员工都会被唤醒，然后继续运行。
<a name="hDTnz"></a>
## 示例2：循环使用CyclicBarrier
对示例1进行改造一下，吃饭完毕之后，所有人都去车上，待所有人都到车上之后，驱车去下一景点玩。
```java
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.TimeUnit;

public class Demo2 {
    public static CyclicBarrier cyclicBarrier = new CyclicBarrier(10);

    public static class T extends Thread {
        int sleep;

        public T(String name, int sleep) {
            super(name);
            this.sleep = sleep;
        }

        //等待吃饭
        void eat() {
            try {
                //模拟休眠
                TimeUnit.SECONDS.sleep(sleep);
                long starTime = System.currentTimeMillis();
                //调用await()的时候，当前线程将会被阻塞，需要等待其他员工都到达await了才能继续
                cyclicBarrier.await();
                long endTime = System.currentTimeMillis();
                System.out.println(this.getName() + ",sleep:" + this.sleep + " 等待了" + (endTime - starTime) + "(ms),开始吃饭了！");

                //休眠sleep时间，模拟当前员工吃饭耗时
                TimeUnit.SECONDS.sleep(sleep);
            } catch (InterruptedException e) {
                e.printStackTrace();
            } catch (BrokenBarrierException e) {
                e.printStackTrace();
            }
        }

        //等待所有人到齐之后，开车去下一站
        void drive() {
            try {
                long starTime = System.currentTimeMillis();
                //调用await()的时候，当前线程将会被阻塞，需要等待其他员工都到达await了才能继续
                cyclicBarrier.await();
                long endTime = System.currentTimeMillis();
                System.out.println(this.getName() + ",sleep:" + this.sleep + " 等待了" + (endTime - starTime) + "(ms),去下一景点的路上！");
            } catch (InterruptedException e) {
                e.printStackTrace();
            } catch (BrokenBarrierException e) {
                e.printStackTrace();
            }
        }

        @Override
        public void run() {
            //等待所有人到齐之后吃饭，先到的人坐那等着，什么事情不要干
            this.eat();
            //等待所有人到齐之后开车去下一景点，先到的人坐那等着，什么事情不要干
            this.drive();
        }
    }

    public static void main(String[] args) throws InterruptedException {
        for (int i = 1; i <= 10; i++) {
            new T("员工" + i, i).start();
        }
    }
}
```
输出：
```
员工10,sleep:10 等待了0(ms),开始吃饭了！
员工5,sleep:5 等待了5000(ms),开始吃饭了！
员工6,sleep:6 等待了4000(ms),开始吃饭了！
员工9,sleep:9 等待了1001(ms),开始吃饭了！
员工4,sleep:4 等待了6000(ms),开始吃饭了！
员工3,sleep:3 等待了7000(ms),开始吃饭了！
员工1,sleep:1 等待了9001(ms),开始吃饭了！
员工2,sleep:2 等待了8000(ms),开始吃饭了！
员工8,sleep:8 等待了2001(ms),开始吃饭了！
员工7,sleep:7 等待了3000(ms),开始吃饭了！
员工10,sleep:10 等待了0(ms),去下一景点的路上！
员工1,sleep:1 等待了8998(ms),去下一景点的路上！
员工5,sleep:5 等待了4999(ms),去下一景点的路上！
员工4,sleep:4 等待了5999(ms),去下一景点的路上！
员工3,sleep:3 等待了6998(ms),去下一景点的路上！
员工2,sleep:2 等待了7998(ms),去下一景点的路上！
员工9,sleep:9 等待了999(ms),去下一景点的路上！
员工8,sleep:8 等待了1999(ms),去下一景点的路上！
员工7,sleep:7 等待了2999(ms),去下一景点的路上！
员工6,sleep:6 等待了3999(ms),去下一景点的路上！
```
坑，又是员工10最慢，要提升效率了，不能吃的太多，得减肥。<br />代码中`CyclicBarrier`相当于使用了2次，第一次用于等待所有人到达后开饭，第二次用于等待所有人上车后驱车去下一景点。注意一些先到的员工会在其他人到达之前，都处于等待状态（`cyclicBarrier.await();`会让当前线程阻塞），无法干其他事情，等到最后一个人到了会唤醒所有人，然后继续。<br />`CyclicBarrier`内部相当于有个计数器（构造方法传入的），每次调用`await();`后，计数器会减1，并且`await()`方法会让当前线程阻塞，等待计数器减为0的时候，所有在`await()`上等待的线程被唤醒，然后继续向下执行，此时计数器又会被还原为创建时的值，然后可以继续再次使用。
<a name="aK48G"></a>
## 示例3：最后到的人给大家上酒，然后开饭
还是示例1中的例子，员工10是最后到达的，让所有人都久等了，那怎么办，得给所有人倒酒，然后开饭，代码如下：
```java
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.TimeUnit;

public class Demo3 {
    public static CyclicBarrier cyclicBarrier = new CyclicBarrier(10, () -> {
        //模拟倒酒，花了2秒，又得让其他9个人等2秒
        try {
            TimeUnit.SECONDS.sleep(2);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println(Thread.currentThread().getName() + "说，不好意思，让大家久等了，给大家倒酒赔罪!");
    });

    public static class T extends Thread {
        int sleep;

        public T(String name, int sleep) {
            super(name);
            this.sleep = sleep;
        }

        @Override
        public void run() {
            try {
                //模拟休眠
                TimeUnit.SECONDS.sleep(sleep);
                long starTime = System.currentTimeMillis();
                //调用await()的时候，当前线程将会被阻塞，需要等待其他员工都到达await了才能继续
                cyclicBarrier.await();
                long endTime = System.currentTimeMillis();
                System.out.println(this.getName() + ",sleep:" + this.sleep + " 等待了" + (endTime - starTime) + "(ms),开始吃饭了！");
            } catch (InterruptedException e) {
                e.printStackTrace();
            } catch (BrokenBarrierException e) {
                e.printStackTrace();
            }
        }
    }

    public static void main(String[] args) throws InterruptedException {
        for (int i = 1; i <= 10; i++) {
            new T("员工" + i, i).start();
        }
    }
}
```
输出：
```
员工10说，不好意思，让大家久等了，给大家倒酒赔罪!
员工10,sleep:10 等待了2000(ms),开始吃饭了！
员工1,sleep:1 等待了11000(ms),开始吃饭了！
员工2,sleep:2 等待了10000(ms),开始吃饭了！
员工5,sleep:5 等待了7000(ms),开始吃饭了！
员工7,sleep:7 等待了5000(ms),开始吃饭了！
员工9,sleep:9 等待了3000(ms),开始吃饭了！
员工4,sleep:4 等待了8000(ms),开始吃饭了！
员工3,sleep:3 等待了9001(ms),开始吃饭了！
员工8,sleep:8 等待了4001(ms),开始吃饭了！
员工6,sleep:6 等待了6001(ms),开始吃饭了！
```
代码中创建`CyclicBarrier`对象时，多传入了一个参数（内部是倒酒操作），先到的人先等待，待所有人都到齐之后，需要先给大家倒酒，然后唤醒所有等待中的人让大家开饭。从输出结果中发现，倒酒操作是由最后一个人操作的，最后一个人倒酒完毕之后，才唤醒所有等待中的其他员工，让大家开饭。
<a name="BeblB"></a>
## 示例4：其中一个人等待中被打断了
员工5等待中，突然接了个电话，有点急事，然后就拿起筷子开吃了，其他人会怎么样呢？看着他吃么？<br />代码如下：
```java
import java.sql.Time;
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.TimeUnit;

public class Demo4 {
    public static CyclicBarrier cyclicBarrier = new CyclicBarrier(10);

    public static class T extends Thread {
        int sleep;

        public T(String name, int sleep) {
            super(name);
            this.sleep = sleep;
        }

        @Override
        public void run() {
            long starTime = 0, endTime = 0;
            try {
                //模拟休眠
                TimeUnit.SECONDS.sleep(sleep);
                starTime = System.currentTimeMillis();
                //调用await()的时候，当前线程将会被阻塞，需要等待其他员工都到达await了才能继续
                System.out.println(this.getName() + "到了！");
                cyclicBarrier.await();
            } catch (InterruptedException e) {
                e.printStackTrace();
            } catch (BrokenBarrierException e) {
                e.printStackTrace();
            }
            endTime = System.currentTimeMillis();
            System.out.println(this.getName() + ",sleep:" + this.sleep + " 等待了" + (endTime - starTime) + "(ms),开始吃饭了！");
        }
    }

    public static void main(String[] args) throws InterruptedException {
        for (int i = 1; i <= 10; i++) {
            int sleep = 0;
            if (i == 10) {
                sleep = 10;
            }
            T t = new T("员工" + i, sleep);
            t.start();
            if (i == 5) {
                //模拟员工5接了个电话，将自己等待吃饭给打断了
                TimeUnit.SECONDS.sleep(1);
                System.out.println(t.getName() + ",有点急事，我先开干了！");
                t.interrupt();
                TimeUnit.SECONDS.sleep(2);
            }
        }
    }
}
```
输出：
```
员工4到了！
员工3到了！
员工5到了！
员工1到了！
员工2到了！
员工5,有点急事，我先开干了！
java.util.concurrent.BrokenBarrierException
员工1,sleep:0 等待了1001(ms),开始吃饭了！
    at java.util.concurrent.CyclicBarrier.dowait(CyclicBarrier.java:250)
员工3,sleep:0 等待了1001(ms),开始吃饭了！
    at java.util.concurrent.CyclicBarrier.await(CyclicBarrier.java:362)
员工4,sleep:0 等待了1001(ms),开始吃饭了！
    at com.example.chat15.Demo4$T.run(Demo4.java:31)
员工2,sleep:0 等待了1001(ms),开始吃饭了！
员工5,sleep:0 等待了1002(ms),开始吃饭了！
java.util.concurrent.BrokenBarrierException
    at java.util.concurrent.CyclicBarrier.dowait(CyclicBarrier.java:250)
    at java.util.concurrent.CyclicBarrier.await(CyclicBarrier.java:362)
    at com.example.chat15.Demo4$T.run(Demo4.java:31)
java.util.concurrent.BrokenBarrierException
    at java.util.concurrent.CyclicBarrier.dowait(CyclicBarrier.java:250)
    at java.util.concurrent.CyclicBarrier.await(CyclicBarrier.java:362)
    at com.example.chat15.Demo4$T.run(Demo4.java:31)
java.util.concurrent.BrokenBarrierException
    at java.util.concurrent.CyclicBarrier.dowait(CyclicBarrier.java:250)
    at java.util.concurrent.CyclicBarrier.await(CyclicBarrier.java:362)
    at com.example.chat15.Demo4$T.run(Demo4.java:31)
java.lang.InterruptedException
    at java.util.concurrent.locks.AbstractQueuedSynchronizer$ConditionObject.reportInterruptAfterWait(AbstractQueuedSynchronizer.java:2014)
    at java.util.concurrent.locks.AbstractQueuedSynchronizer$ConditionObject.await(AbstractQueuedSynchronizer.java:2048)
    at java.util.concurrent.CyclicBarrier.dowait(CyclicBarrier.java:234)
    at java.util.concurrent.CyclicBarrier.await(CyclicBarrier.java:362)
    at com.example.chat15.Demo4$T.run(Demo4.java:31)
java.util.concurrent.BrokenBarrierException
    at java.util.concurrent.CyclicBarrier.dowait(CyclicBarrier.java:207)
    at java.util.concurrent.CyclicBarrier.await(CyclicBarrier.java:362)
    at com.example.chat15.Demo4$T.run(Demo4.java:31)
java.util.concurrent.BrokenBarrierException
员工6到了！
    at java.util.concurrent.CyclicBarrier.dowait(CyclicBarrier.java:207)
    at java.util.concurrent.CyclicBarrier.await(CyclicBarrier.java:362)
员工9到了！
    at com.example.chat15.Demo4$T.run(Demo4.java:31)
员工8到了！
员工7到了！
员工6,sleep:0 等待了0(ms),开始吃饭了！
员工7,sleep:0 等待了1(ms),开始吃饭了！
java.util.concurrent.BrokenBarrierException
    at java.util.concurrent.CyclicBarrier.dowait(CyclicBarrier.java:207)
    at java.util.concurrent.CyclicBarrier.await(CyclicBarrier.java:362)
    at com.example.chat15.Demo4$T.run(Demo4.java:31)
java.util.concurrent.BrokenBarrierException
    at java.util.concurrent.CyclicBarrier.dowait(CyclicBarrier.java:207)
    at java.util.concurrent.CyclicBarrier.await(CyclicBarrier.java:362)
    at com.example.chat15.Demo4$T.run(Demo4.java:31)
员工8,sleep:0 等待了1(ms),开始吃饭了！
员工9,sleep:0 等待了1(ms),开始吃饭了！
Disconnected from the target VM, address: '127.0.0.1:64413', transport: 'socket'
java.util.concurrent.BrokenBarrierException
    at java.util.concurrent.CyclicBarrier.dowait(CyclicBarrier.java:207)
    at java.util.concurrent.CyclicBarrier.await(CyclicBarrier.java:362)
    at com.example.chat15.Demo4$T.run(Demo4.java:31)
员工10到了！
员工10,sleep:10 等待了0(ms),开始吃饭了！
```
输出的信息看着有点乱，给大家理一理，员工5遇到急事，拿起筷子就是吃，这样好么，当然不好，他这么做了，后面看他这么做了都跟着这么做（这种场景是不是很熟悉，有一个人拿起筷子先吃起来，其他人都跟着上了），直接不等其他人了，拿起筷子就开吃了。`CyclicBarrier`遇到这种情况就是这么处理的。前面4个员工都在`await()`处等待着，员工5也在`await()`上等待着，等了1秒（`TimeUnit.SECONDS.sleep(1);`），接了个电话，然后给员工5发送中断信号后（`t.interrupt();`），员工5的`await()`方法会触发`InterruptedException`异常，此时其他等待中的前4个员工，看着5开吃了，自己立即也不等了，内部从await()方法中触发`BrokenBarrierException`异常，然后也开吃了，后面的6/7/8/9/10员工来了以后发现大家都开吃了，自己也不等了，6-10员工调用`await()`直接抛出了`BrokenBarrierException`异常，然后继续向下。<br />**结论：**

1. **内部有一个人把规则破坏了（接收到中断信号），其他人都不按规则来了，不会等待了**
2. **接收到中断信号的线程，**`**await**`**方法会触发**`**InterruptedException**`**异常，然后被唤醒向下运行**
3. **其他等待中 或者后面到达的线程，会在**`**await()**`**方法上触发**`**BrokenBarrierException**`**异常，然后继续执行**
<a name="IyKfl"></a>
## 示例5：其中一个人只愿意等的5秒
基于示例1，员工1只愿意等的5秒，5s后如果大家还没到期，自己要开吃了，员工1开吃了，其他人会怎么样呢？
```java
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

public class Demo5 {
    public static CyclicBarrier cyclicBarrier = new CyclicBarrier(10);

    public static class T extends Thread {
        int sleep;

        public T(String name, int sleep) {
            super(name);
            this.sleep = sleep;
        }

        @Override
        public void run() {
            long starTime = 0, endTime = 0;
            try {
                //模拟休眠
                TimeUnit.SECONDS.sleep(sleep);
                starTime = System.currentTimeMillis();
                //调用await()的时候，当前线程将会被阻塞，需要等待其他员工都到达await了才能继续
                System.out.println(this.getName() + "到了！");
                if (this.getName().equals("员工1")) {
                    cyclicBarrier.await(5, TimeUnit.SECONDS);
                } else {
                    cyclicBarrier.await();
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            } catch (BrokenBarrierException e) {
                e.printStackTrace();
            } catch (TimeoutException e) {
                e.printStackTrace();
            }
            endTime = System.currentTimeMillis();
            System.out.println(this.getName() + ",sleep:" + this.sleep + " 等待了" + (endTime - starTime) + "(ms),开始吃饭了！");
        }
    }

    public static void main(String[] args) throws InterruptedException {
        for (int i = 1; i <= 10; i++) {
            T t = new T("员工" + i, i);
            t.start();
        }
    }
}
```
输出：
```
员工1到了！
员工2到了！
员工3到了！
员工4到了！
员工5到了！
员工6到了！
员工1,sleep:1 等待了5001(ms),开始吃饭了！
员工5,sleep:5 等待了1001(ms),开始吃饭了！
java.util.concurrent.TimeoutException
    at java.util.concurrent.CyclicBarrier.dowait(CyclicBarrier.java:257)
    at java.util.concurrent.CyclicBarrier.await(CyclicBarrier.java:435)
    at com.example.chat15.Demo5$T.run(Demo5.java:32)
java.util.concurrent.BrokenBarrierException
    at java.util.concurrent.CyclicBarrier.dowait(CyclicBarrier.java:250)
    at java.util.concurrent.CyclicBarrier.await(CyclicBarrier.java:362)
    at com.example.chat15.Demo5$T.run(Demo5.java:34)
java.util.concurrent.BrokenBarrierException
    at java.util.concurrent.CyclicBarrier.dowait(CyclicBarrier.java:207)
    at java.util.concurrent.CyclicBarrier.await(CyclicBarrier.java:362)
    at com.example.chat15.Demo5$T.run(Demo5.java:34)
员工6,sleep:6 等待了2(ms),开始吃饭了！
java.util.concurrent.BrokenBarrierException
员工2,sleep:2 等待了4002(ms),开始吃饭了！
    at java.util.concurrent.CyclicBarrier.dowait(CyclicBarrier.java:250)
员工3,sleep:3 等待了3001(ms),开始吃饭了！
    at java.util.concurrent.CyclicBarrier.await(CyclicBarrier.java:362)
员工4,sleep:4 等待了2001(ms),开始吃饭了！
    at com.example.chat15.Demo5$T.run(Demo5.java:34)
java.util.concurrent.BrokenBarrierException
    at java.util.concurrent.CyclicBarrier.dowait(CyclicBarrier.java:250)
    at java.util.concurrent.CyclicBarrier.await(CyclicBarrier.java:362)
    at com.example.chat15.Demo5$T.run(Demo5.java:34)
java.util.concurrent.BrokenBarrierException
    at java.util.concurrent.CyclicBarrier.dowait(CyclicBarrier.java:250)
    at java.util.concurrent.CyclicBarrier.await(CyclicBarrier.java:362)
    at com.example.chat15.Demo5$T.run(Demo5.java:34)
java.util.concurrent.BrokenBarrierException
员工7到了！
    at java.util.concurrent.CyclicBarrier.dowait(CyclicBarrier.java:207)
员工7,sleep:7 等待了0(ms),开始吃饭了！
    at java.util.concurrent.CyclicBarrier.await(CyclicBarrier.java:362)
    at com.example.chat15.Demo5$T.run(Demo5.java:34)
员工8到了！
员工8,sleep:8 等待了0(ms),开始吃饭了！
java.util.concurrent.BrokenBarrierException
    at java.util.concurrent.CyclicBarrier.dowait(CyclicBarrier.java:207)
    at java.util.concurrent.CyclicBarrier.await(CyclicBarrier.java:362)
    at com.example.chat15.Demo5$T.run(Demo5.java:34)
员工9到了！
java.util.concurrent.BrokenBarrierException
员工9,sleep:9 等待了0(ms),开始吃饭了！
    at java.util.concurrent.CyclicBarrier.dowait(CyclicBarrier.java:207)
    at java.util.concurrent.CyclicBarrier.await(CyclicBarrier.java:362)
    at com.example.chat15.Demo5$T.run(Demo5.java:34)
java.util.concurrent.BrokenBarrierException
员工10到了！
    at java.util.concurrent.CyclicBarrier.dowait(CyclicBarrier.java:207)
    at java.util.concurrent.CyclicBarrier.await(CyclicBarrier.java:362)
员工10,sleep:10 等待了0(ms),开始吃饭了！
    at com.example.chat15.Demo5$T.run(Demo5.java:34)
```
从输出结果中可以看到：1等待5秒之后，开吃了，其他等待人都开吃了，后面来的人不等待，直接开吃了。<br />员工1调用有参`await`方法等待5秒之后，触发了`TimeoutException`异常，然后继续向下运行，其他的在5开吃之前已经等了一会的的几个员工，他们看到5开吃了，自己立即不等待了，也也开吃了（他们的`await`抛出了`BrokenBarrierException`异常）；还有几个员工在5开吃之后到达的，他们直接不等待了，直接抛出`BrokenBarrierException`异常，然后也开吃了。<br />**结论：**

1. **等待超时的方法**
```java
public int await(long timeout, TimeUnit unit) throws InterruptedException,BrokenBarrierException,TimeoutException
```

2. **内部有一个人把规则破坏了（等待超时），其他人都不按规则来了，不会等待了**
3. **等待超时的线程，**`**await**`**方法会触发**`**TimeoutException**`**异常，然后被唤醒向下运行**
4. **其他等待中 或者后面到达的线程，会在**`**await()**`**方法上触发**`**BrokenBarrierException**`**异常，然后继续执行**
<a name="wvLrd"></a>
## 示例6：重建规则
示例5中改造一下，员工1等待5秒超时之后，开吃了，打破了规则，先前等待中的以及后面到达的都不按规则来了，都拿起筷子开吃。过了一会，导游重新告知大家，要按规则来，然后重建了规则，大家都按规则来了。<br />代码如下：
```java
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

public class Demo6 {
    public static CyclicBarrier cyclicBarrier = new CyclicBarrier(10);

    //规则是否已重建
    public static boolean guizhe = false;

    public static class T extends Thread {
        int sleep;

        public T(String name, int sleep) {
            super(name);
            this.sleep = sleep;
        }

        @Override
        public void run() {
            long starTime = 0, endTime = 0;
            try {
                //模拟休眠
                TimeUnit.SECONDS.sleep(sleep);
                starTime = System.currentTimeMillis();
                //调用await()的时候，当前线程将会被阻塞，需要等待其他员工都到达await了才能继续
                System.out.println(this.getName() + "到了！");
                if (!guizhe) {
                    if (this.getName().equals("员工1")) {
                        cyclicBarrier.await(5, TimeUnit.SECONDS);
                    } else {
                        cyclicBarrier.await();
                    }
                } else {
                    cyclicBarrier.await();

                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            } catch (BrokenBarrierException e) {
                e.printStackTrace();
            } catch (TimeoutException e) {
                e.printStackTrace();
            }
            endTime = System.currentTimeMillis();
            System.out.println(this.getName() + ",sleep:" + this.sleep + " 等待了" + (endTime - starTime) + "(ms),开始吃饭了！");
        }
    }

    public static void main(String[] args) throws InterruptedException {
        for (int i = 1; i <= 10; i++) {
            T t = new T("员工" + i, i);
            t.start();
        }

        //等待10秒之后，重置，重建规则
        TimeUnit.SECONDS.sleep(15);
        cyclicBarrier.reset();
        guizhe = true;
        System.out.println("---------------大家太皮了，请大家按规则来------------------");
        //再来一次
        for (int i = 1; i <= 10; i++) {
            T t = new T("员工" + i, i);
            t.start();
        }
    }
}
```
输出：
```
员工1到了！
员工2到了！
员工3到了！
员工4到了！
员工5到了！
java.util.concurrent.TimeoutException
    at java.util.concurrent.CyclicBarrier.dowait(CyclicBarrier.java:257)
    at java.util.concurrent.CyclicBarrier.await(CyclicBarrier.java:435)
    at com.example.chat15.Demo6$T.run(Demo6.java:36)
java.util.concurrent.BrokenBarrierException
    at java.util.concurrent.CyclicBarrier.dowait(CyclicBarrier.java:250)
    at java.util.concurrent.CyclicBarrier.await(CyclicBarrier.java:362)
    at com.example.chat15.Demo6$T.run(Demo6.java:38)
java.util.concurrent.BrokenBarrierException
    at java.util.concurrent.CyclicBarrier.dowait(CyclicBarrier.java:250)
    at java.util.concurrent.CyclicBarrier.await(CyclicBarrier.java:362)
    at com.example.chat15.Demo6$T.run(Demo6.java:38)
java.util.concurrent.BrokenBarrierException
    at java.util.concurrent.CyclicBarrier.dowait(CyclicBarrier.java:250)
    at java.util.concurrent.CyclicBarrier.await(CyclicBarrier.java:362)
    at com.example.chat15.Demo6$T.run(Demo6.java:38)
java.util.concurrent.BrokenBarrierException
    at java.util.concurrent.CyclicBarrier.dowait(CyclicBarrier.java:250)
    at java.util.concurrent.CyclicBarrier.await(CyclicBarrier.java:362)
    at com.example.chat15.Demo6$T.run(Demo6.java:38)
java.util.concurrent.BrokenBarrierException
    at java.util.concurrent.CyclicBarrier.dowait(CyclicBarrier.java:250)
    at java.util.concurrent.CyclicBarrier.await(CyclicBarrier.java:362)
    at com.example.chat15.Demo6$T.run(Demo6.java:38)
员工6到了！
员工1,sleep:1 等待了5002(ms),开始吃饭了！
员工6,sleep:6 等待了4(ms),开始吃饭了！
员工4,sleep:4 等待了2004(ms),开始吃饭了！
员工5,sleep:5 等待了1004(ms),开始吃饭了！
员工3,sleep:3 等待了3002(ms),开始吃饭了！
员工2,sleep:2 等待了4004(ms),开始吃饭了！
员工7到了！
员工7,sleep:7 等待了0(ms),开始吃饭了！
java.util.concurrent.BrokenBarrierException
    at java.util.concurrent.CyclicBarrier.dowait(CyclicBarrier.java:207)
    at java.util.concurrent.CyclicBarrier.await(CyclicBarrier.java:362)
    at com.example.chat15.Demo6$T.run(Demo6.java:38)
java.util.concurrent.BrokenBarrierException
    at java.util.concurrent.CyclicBarrier.dowait(CyclicBarrier.java:207)
    at java.util.concurrent.CyclicBarrier.await(CyclicBarrier.java:362)
    at com.example.chat15.Demo6$T.run(Demo6.java:38)
员工8到了！
员工8,sleep:8 等待了0(ms),开始吃饭了！
java.util.concurrent.BrokenBarrierException
员工9到了！
    at java.util.concurrent.CyclicBarrier.dowait(CyclicBarrier.java:207)
员工9,sleep:9 等待了0(ms),开始吃饭了！
    at java.util.concurrent.CyclicBarrier.await(CyclicBarrier.java:362)
    at com.example.chat15.Demo6$T.run(Demo6.java:38)
java.util.concurrent.BrokenBarrierException
员工10到了！
    at java.util.concurrent.CyclicBarrier.dowait(CyclicBarrier.java:207)
员工10,sleep:10 等待了0(ms),开始吃饭了！
    at java.util.concurrent.CyclicBarrier.await(CyclicBarrier.java:362)
    at com.example.chat15.Demo6$T.run(Demo6.java:38)
---------------大家太皮了，请大家按规则来------------------
员工1到了！
员工2到了！
员工3到了！
员工4到了！
员工5到了！
员工6到了！
员工7到了！
员工8到了！
员工9到了！
员工10到了！
员工10,sleep:10 等待了0(ms),开始吃饭了！
员工1,sleep:1 等待了9000(ms),开始吃饭了！
员工2,sleep:2 等待了8000(ms),开始吃饭了！
员工3,sleep:3 等待了6999(ms),开始吃饭了！
员工7,sleep:7 等待了3000(ms),开始吃饭了！
员工6,sleep:6 等待了4000(ms),开始吃饭了！
员工5,sleep:5 等待了5000(ms),开始吃饭了！
员工4,sleep:4 等待了6000(ms),开始吃饭了！
员工9,sleep:9 等待了999(ms),开始吃饭了！
员工8,sleep:8 等待了1999(ms),开始吃饭了！
```
第一次规则被打乱了，过了一会导游重建了规则（`cyclicBarrier.reset();`），接着又重来来了一次模拟等待吃饭的操作，正常了。
<a name="fAJ6V"></a>
## `CountDownLatch`和`CyclicBarrier`的区别
还是举例子说明一下：<br />`**CountDownLatch**`**示例**<br />主管相当于 `**CountDownLatch**`，干活的小弟相当于做事情的线程。<br />老板交给主管了一个任务，让主管搞完之后立即上报给老板。主管下面有10个小弟，接到任务之后将任务划分为10个小任务分给每个小弟去干，主管一直处于等待状态（主管会调用`await()`方法，此方法会阻塞当前线程），让每个小弟干完之后通知一下主管（调用`countDown()`方法通知主管，此方法会立即返回），主管等到所有的小弟都做完了，会被唤醒，从`await()`方法上苏醒，然后将结果反馈给老板。期间主管会等待，会等待所有小弟将结果汇报给自己。<br />**而CyclicBarrier是一批线程让自己等待，等待所有的线程都准备好了，自己才能继续。**
