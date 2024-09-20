Java<br />对异步和同步的理解：

- **同步调用**：调用方在调用过程中，持续等待返回结果。
- **异步调用**：调用方在调用过程中，不直接等待返回结果，而是执行其他任务，结果返回形式通常为回调函数。

其实，两者的区别还是很明显的，这里也不再细说，主要来说一下Java如何将异步调用转为同步。换句话说，就是需要在异步调用过程中，持续阻塞至获得调用结果。<br />不卖关子，先列出五种方法，然后一一举例说明：

- 使用`wait`和`notify`方法
- 使用条件锁
- `Future`
- 使用`CountDownLatch`
- 使用`CyclicBarrier`
<a name="zGQ7a"></a>
### 构造一个异步调用
首先，写demo需要先写基础设施，这里的话主要是需要构造一个异步调用模型。异步调用类:
```java
public class AsyncCall {

    private Random random = new Random(System.currentTimeMillis());

    private ExecutorService tp = Executors.newSingleThreadExecutor();

    //demo1,2,4,5调用方法
    public void call(BaseDemo demo){

        new Thread(()->{
            long res = random.nextInt(10);

            try {
                Thread.sleep(res*1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            demo.callback(res);
        }).start();
    }

    //demo3调用方法
    public Future<Long> futureCall(){

        return tp.submit(()-> {
            long res = random.nextInt(10);

            try {
                Thread.sleep(res*1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            return res;
        });
    }

    public void shutdown(){
        tp.shutdown();
    }

}
```
主要关心`call`方法，这个方法接收了一个demo参数，并且开启了一个线程，在线程中执行具体的任务，并利用demo的`callback`方法进行回调函数的调用。大家注意到了这里的返回结果就是一个`[0,10)`的长整型，并且结果是几，就让线程sleep多久——这主要是为了更好地观察实验结果，模拟异步调用过程中的处理时间。<br />至于`futureCall`和`shutdown`方法，以及线程池tp都是为了demo3利用`Future`来实现做准备的。<br />demo的基类:
```java
public abstract class BaseDemo {

    protected AsyncCall asyncCall = new AsyncCall();

    public abstract void callback(long response);

    public void call(){
        System.out.println("发起调用");
        asyncCall.call(this);
        System.out.println("调用返回");
    }

}
```
BaseDemo非常简单，里面包含一个异步调用类的实例，另外有一个call方法用于发起异步调用，当然还有一个抽象方法callback需要每个demo去实现的——主要在回调中进行相应的处理来达到异步调用转同步的目的。
<a name="ruEkZ"></a>
### 使用`wait`和`notify`方法
这个方法其实是利用了锁机制，直接贴代码：
```java
public class Demo1 extends BaseDemo{

    private final Object lock = new Object();

    @Override
    public void callback(long response) {
        System.out.println("得到结果");
        System.out.println(response);
        System.out.println("调用结束");

        synchronized (lock) {
            lock.notifyAll();
        }

    }

    public static void main(String[] args) {

        Demo1 demo1 = new Demo1();

        demo1.call();

        synchronized (demo1.lock){
            try {
                demo1.lock.wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        System.out.println("主线程内容");

    }
}
```
可以看到在发起调用后，主线程利用`wait`进行阻塞，等待回调中调用`notify`或者`notifyAll`方法来进行唤醒。注意，和大家认知的一样，这里`wait`和`notify`都是需要先获得对象的锁的。<br />在主线程中最后打印了一个内容，这也是用来验证实验结果的，如果没有`wait`和`notify`，主线程内容会紧随调用内容立刻打印；而像上面的代码，主线程内容会一直等待回调函数调用结束才会进行打印。<br />没有使用同步操作的情况下，打印结果：
```java
发起调用
调用返回
主线程内容
得到结果
1
调用结束
```
而使用了同步操作后：
```java
发起调用
调用返回
得到结果
9
调用结束
主线程内容
```
<a name="zEwcW"></a>
### 使用条件锁
和方法一的原理类似：
```java
public class Demo2 extends BaseDemo {

    private final Lock lock = new ReentrantLock();
    private final Condition con = lock.newCondition();

    @Override
    public void callback(long response) {

        System.out.println("得到结果");
        System.out.println(response);
        System.out.println("调用结束");
        lock.lock();
        try {
            con.signal();
        }finally {
            lock.unlock();
        }

    }

    public static void main(String[] args) {

        Demo2 demo2 = new Demo2();

        demo2.call();

        demo2.lock.lock();

        try {
            demo2.con.await();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }finally {
            demo2.lock.unlock();
        }
        System.out.println("主线程内容");
    }
}
```
基本上和方法一没什么区别，只是这里使用了条件锁，两者的锁机制有所不同。
<a name="BXQ9Z"></a>
### `Future`
使用`Future`的方法和之前不太一样，调用的异步方法也不一样。
```java
public class Demo3{

    private AsyncCall asyncCall = new AsyncCall();

    public Future<Long> call(){

        Future<Long> future = asyncCall.futureCall();

        asyncCall.shutdown();

        return future;

    }

    public static void main(String[] args) {

        Demo3 demo3 = new Demo3();

        System.out.println("发起调用");
        Future<Long> future = demo3.call();
        System.out.println("返回结果");

        while (!future.isDone() && !future.isCancelled());

        try {
            System.out.println(future.get());
        } catch (InterruptedException e) {
            e.printStackTrace();
        } catch (ExecutionException e) {
            e.printStackTrace();
        }

        System.out.println("主线程内容");

    }
}
```
调用`futureCall`方法，方法中会想线程池tp提交一个`Callable`，然后返回一个Future，这个Future就是demo3中call中得到的，得到`future`对象之后就可以关闭线程池啦，调用`asyncCall`的`shutdown`方法。关于关闭线程池这里有一点需要注意，回过头来看看`asyncCall`的`shutdown`方法：
```java
public void shutdown(){
   tp.shutdown();
}
```
发现只是简单调用了线程池的`shutdown`方法，然后说注意的点，这里最好不要用tp的`shutdownNow`方法，该方法会试图去中断线程中中正在执行的任务；也就是说，如果使用该方法，有可能future所对应的任务将被中断，无法得到执行结果。<br />然后关注主线程中的内容，主线程的阻塞由自己来实现，通过`future`的`isDone`和`isCancelled`来判断执行状态，一直到执行完成或被取消。随后，打印get到的结果。
<a name="bn3sR"></a>
### 使用`CountDownLatch`
使用`CountDownLatch`或许是日常编程中最常见的一种了，也感觉是相对优雅的一种：
```java
public class Demo4 extends BaseDemo{

    private final CountDownLatch countDownLatch = new CountDownLatch(1);

    @Override
    public void callback(long response) {

        System.out.println("得到结果");
        System.out.println(response);
        System.out.println("调用结束");

        countDownLatch.countDown();

    }

    public static void main(String[] args) {

        Demo4 demo4 = new Demo4();

        demo4.call();

        try {
            demo4.countDownLatch.await();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        System.out.println("主线程内容");

    }
}
```
正如大家平时使用的那样，此处在主线程中利用`CountDownLatch`的`await`方法进行阻塞，在回调中利用`countDown`方法来使得其他线程`await`的部分得以继续运行。<br />当然，这里和demo1和demo2中都一样，主线程中阻塞的部分，都可以设置一个超时时间，超时后可以不再阻塞。
<a name="MzQt6"></a>
### 使用`CyclicBarrier`
`CyclicBarrier`的情况和`CountDownLatch`有些类似：
```java
public class Demo5 extends BaseDemo{

    private CyclicBarrier cyclicBarrier = new CyclicBarrier(2);


    @Override
    public void callback(long response) {

        System.out.println("得到结果");
        System.out.println(response);
        System.out.println("调用结束");

        try {
            cyclicBarrier.await();
        } catch (InterruptedException e) {
            e.printStackTrace();
        } catch (BrokenBarrierException e) {
            e.printStackTrace();
        }

    }

    public static void main(String[] args) {

        Demo5 demo5 = new Demo5();
        demo5.call();
        try {
            demo5.cyclicBarrier.await();
        } catch (InterruptedException e) {
            e.printStackTrace();
        } catch (BrokenBarrierException e) {
            e.printStackTrace();
        }

        System.out.println("主线程内容");

    }
}
```
大家注意一下，`CyclicBarrier`和`CountDownLatch`仅仅只是类似，两者还是有一定区别的。比如，一个可以理解为做加法，等到加到这个数字后一起运行；一个则是减法，减到0继续运行。一个是可以重复计数的；另一个不可以等等等等。<br />另外，使用`CyclicBarrier`的时候要注意两点。第一点，初始化的时候，参数数字要设为2，因为异步调用这里是一个线程，而主线程是一个线程，两个线程都`await`的时候才能继续执行，这也是和`CountDownLatch`区别的部分。<br />第二点，也是关于初始化参数的数值的，和这里的demo无关，在平时编程的时候，需要比较小心，如果这个数值设置得很大，比线程池中的线程数都大，那么就很容易引起死锁了。
<a name="juDJd"></a>
### 总结
综上，就是本次需要说的几种方法了。事实上，所有的方法都是同一个原理，也就是在调用的线程中进行阻塞等待结果，而在回调中函数中进行阻塞状态的解除。
