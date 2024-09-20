JUC
<a name="vs21m"></a>
### `CountDownLatch`
`CountDownLatch`是目前使用比较多的类，`CountDownLatch`初始化时会给定一个计数，然后每次调用`countDown()` 计数减1，<br />当计数未到达0之前调用`await()` 方法会阻塞直到计数减到0；<br />使用场景：多用于划分任务由多个线程执行，例如：最近写个豆瓣爬虫，需要爬取每个电影的前五页短评，可以划分成五个线程来处理数据。通过`latch.await()`保证全部完成再返回。
```java
public void latch() throws InterruptedException {
    int count= 5;
    CountDownLatch latch = new CountDownLatch(count);
    for (int x=0;x<count;x++){
        new Worker(x*20,latch).start();
    }
    latch.await();
    System.out.println("全部执行完毕");
}

class Worker extends Thread{
    Integer start;
    CountDownLatch latch;
    public Worker(Integer start,CountDownLatch latch){
        this.start=start;
        this.latch=latch;
    }        @Override
    public void run() {
        System.out.println(start+" 已执行");
        latch.countDown();
    }
}
```
输出如下：
```
20 已执行
0 已执行
40 已执行
60 已执行
80 已执行
全部执行完毕
```
<a name="zxb9C"></a>
### `CyclicBarrier`
它允许一组线程互相等待，直到到达某个公共屏障点 (common barrier point)也就是阻塞在调用`cyclicBarrier.await()`的地方。<br />看上去`CyclicBarrier` 跟`CountDownLatch` 功能上类似，在官方doc上`CountDownLatch`的描述上就说明了，`CountDownLatch` 的计数无法被重置，<br />如果需要重置计数，请考虑使用`CyclicBarrier`。<br />`CyclicBarrier`初始时还可添加一个`Runnable`的参数， 此`Runnable`在`CyclicBarrier`的数目达到后，所有其它线程被唤醒前被最后一个进入 `CyclicBarrier` 的线程执行<br />使用场景：类似`CyclicBarrier`，但是 `CyclicBarrier`提供了几个`CountDownLatch`没有的方法以应付更复杂的场景，例如：<br />`getNumberWaiting()` 获取阻塞线程数量，<br />`isBroken()` 用来知道阻塞的线程是否被中断等方法。<br />`reset()` 将屏障重置为其初始状态。如果所有参与者目前都在屏障处等待，则它们将返回，同时抛出一个 `BrokenBarrierException`。
```java
public void latch() throws InterruptedException {
    int count = 5;
    CyclicBarrier cb = new CyclicBarrier(count, new Runnable() {
        @Override
        public void run() {
            System.out.println("全部执行完毕");
        }
    });
    ExecutorService executorService = Executors.newFixedThreadPool(count);
    while (true){
        for (int x=0;x<count;x++){
            executorService.execute(new Worker(x,cb));
        }
    }
}    

class Worker extends Thread {
    Integer start;
    CyclicBarrier cyclicBarrier;        public Worker(Integer start, CyclicBarrier cyclicBarrier) {
        this.start = start;
        this.cyclicBarrier = cyclicBarrier;
    }        @Override
    public void run() {
        System.out.println(start + " 已执行");
        try {
            cyclicBarrier.await();
        } catch (InterruptedException e) {
            e.printStackTrace();
        } catch (BrokenBarrierException e) {
            e.printStackTrace();
        }
    }
}
```
输出如下：
```
0 已执行
3 已执行
4 已执行
2 已执行
1 已执行
全部执行完毕
0 已执行
1 已执行
2 已执行
3 已执行
4 已执行
全部执行完毕
```
<a name="HXRQf"></a>
### `Semaphore`
`Semaphore` 信号量维护了一个许可集,每次使用时执行`acquire()`从`Semaphore`获取许可，如果没有则会阻塞，每次使用完执行`release()`释放许可。<br />使用场景：`Semaphore`对用于对资源的控制，比如数据连接有限，使用`Semaphore`限制访问数据库的线程数。
```java
public void latch() throws InterruptedException, IOException {
    int count = 5;
    Semaphore semaphore = new Semaphore(1);
    ExecutorService executorService = Executors.newFixedThreadPool(count);
    for (int x=0;x<count;x++){
        executorService.execute(new Worker(x,semaphore));
    }
    System.in.read();
}    

class Worker extends Thread {
    Integer start;
    Semaphore semaphore;        public Worker(Integer start, Semaphore semaphore) {
        this.start = start;
        this.semaphore = semaphore;
    }        @Override
    public void run() throws IllegalArgumentException {
        try {
            System.out.println(start + " 准备执行");
            TimeUnit.SECONDS.sleep(1);
            semaphore.acquire();
            System.out.println(start + " 已经执行");
            semaphore.release();
            System.out.println(start + " 已经释放");
        } catch (InterruptedException e) {
            e.printStackTrace();
        }        }
}
```
输出如下：
```
0 准备执行
2 准备执行
1 准备执行
3 准备执行
4 准备执行
2 已经执行
2 已经释放
4 已经执行
4 已经释放
1 已经执行
1 已经释放
0 已经执行
0 已经释放
3 已经执行
3 已经释放
```
<a name="hhCcG"></a>
### `Exchanger`
`Exchanger` 用于两个线程间的数据交换，它提供一个同步点，在这个同步点两个线程可以交换彼此的数据。<br />使用场景：两个线程相互等待处理结果并进行数据传递。
```java
public void latch() throws InterruptedException, IOException {
    int count = 5;
    Exchanger<String> exchanger = new Exchanger<>();
    ExecutorService executorService = Executors.newFixedThreadPool(count);
    for (int x=0;x<count;x++){
        executorService.execute(new Worker(x,exchanger));
    }
    System.in.read();
}    

class Worker extends Thread {
    Integer start;
    Exchanger<String>  exchanger;        public Worker(Integer start, Exchanger<String> exchanger) {
        this.start = start;
        this.exchanger = exchanger;
    }        @Override
    public void run() throws IllegalArgumentException {
        try {
            System.out.println(Thread.currentThread().getName() + " 准备执行");
            TimeUnit.SECONDS.sleep(start);
            System.out.println(Thread.currentThread().getName() + " 等待交换");
            String value = exchanger.exchange(Thread.currentThread().getName());
            System.out.println(Thread.currentThread().getName() + " 交换得到数据为："+value);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }        }
}
```
输出如下：
```
pool-1-thread-1 准备执行
pool-1-thread-1 等待交换
pool-1-thread-3 准备执行
pool-1-thread-2 准备执行
pool-1-thread-5 准备执行
pool-1-thread-4 准备执行
pool-1-thread-2 等待交换
pool-1-thread-1 交换得到数据为：pool-1-thread-2
pool-1-thread-2 交换得到数据为：pool-1-thread-1
pool-1-thread-3 等待交换
pool-1-thread-4 等待交换
pool-1-thread-4 交换得到数据为：pool-1-thread-3
pool-1-thread-3 交换得到数据为：pool-1-thread-4
pool-1-thread-5 等待交换
```
`Exchanger`必须成对出现，否则会像上面代码执行结果那样，pool-1-thread-5一直阻塞等待与其交换数据的线程，为了避免这一现象，可以使用`exchange(V x, long timeout, TimeUnit unit)`设置最大等待时长。
