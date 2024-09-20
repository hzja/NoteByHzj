Java 高并发 CyclicBarrier
<a name="WQViv"></a>
## CyclicBarrier
`CyclicBarrier`是什么？把它拆开来翻译就是循环（`Cycle`）和屏障（`Barrier`）。它的主要作用其实和`CountDownLanch`差不多，都是让一组线程到达一个屏障时被阻塞，直到最后一个线程到达屏障时，屏障会被打开，所有被屏障阻塞的线程才会继续执行，不过它是可以循环执行的，这是它与`CountDownLanch`最大的不同。`CountDownLanch`是只有当最后一个线程把计数器置为`0`的时候，其他阻塞的线程才会继续执行。<br />首先先来看下关于使用`CyclicBarrier`的一个`demo`：比如游戏中有个关卡的时候，每次进入下一关的时候都需要进行加载一些地图、特效背景音乐什么的只有全部加载完了才能够进行游戏：
```java
public class CyclicBarrierExample {
    static class PreTaskThread implements Runnable {
        private String task;
        private CyclicBarrier cyclicBarrier;
        public PreTaskThread(String task, CyclicBarrier cyclicBarrier) {
            this.task = task;
            this.cyclicBarrier = cyclicBarrier;
        }
        @Override
        public void run() {
            for (int i = 0; i < 4; i++) {
                Random random = new Random();
                try {
                    Thread.sleep(random.nextInt(1000));
                    System.out.println(String.format("关卡 %d 的任务 %s 完成", i, task));
                    cyclicBarrier.await();
                } catch (InterruptedException | BrokenBarrierException e) {
                    e.printStackTrace();
                }
            }
        }
        public static void main(String[] args) {
            CyclicBarrier cyclicBarrier = new CyclicBarrier(3, () -> {
                System.out.println("本关卡所有的前置任务完成，开始游戏... ...");
            });
            new Thread(new PreTaskThread("加载地图数据", cyclicBarrier)).start();
            new Thread(new PreTaskThread("加载人物模型", cyclicBarrier)).start();
            new Thread(new PreTaskThread("加载背景音乐", cyclicBarrier)).start();
        }
    }
}
```
输出结果如下：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617032471962-5c1bfe59-6482-45b5-89c6-9670e44d9d69.png#align=left&display=inline&height=284&originHeight=566&originWidth=1080&size=0&status=done&style=shadow&width=541)<br />可以看到每次游戏开始都会等当前关卡把游戏的人物模型，地图数据、背景音乐加载完成后才会开始进行游戏。并且还是可以循环控制的。
<a name="fzz8b"></a>
## 源码分析
<a name="WwFM1"></a>
### 结构组成
```java
/** The lock for guarding barrier entry */
private final ReentrantLock lock = new ReentrantLock();
/** Condition to wait on until tripped */
private final Condition trip = lock.newCondition();
/** The number of parties */
private final int parties;
/* The command to run when tripped */
private final Runnable barrierCommand;
/** The current generation */
private Generation generation = new Generation();
```

- **lock**：用于保护屏障入口的锁
- **trip**：达到屏障并且不能放行的线程在trip条件变量上等待
- **parties**：栅栏开启需要的到达线程总数
- **barrierCommand**：最后一个线程到达屏障后执行的回调任务
- **generation**：这是一个内部类，通过它实现`CyclicBarrier`重复利用，每当`await`达到最大次数的时候，就会重新`new` 一个，表示进入了下一个轮回。里面只有一个`boolean`型属性，用来表示当前轮回是否有线程中断。
<a name="KQlvC"></a>
### 主要方法
`await`方法
```java
public int await() throws InterruptedException, BrokenBarrierException {
    try {
        return dowait(false, 0L);
    } catch (TimeoutException toe) {
        throw new Error(toe); // cannot happen
    }
}
/**
     * Main barrier code, covering the various policies.
     */
private int dowait(boolean timed, long nanos)
    throws InterruptedException, BrokenBarrierException, TimeoutException {
    final ReentrantLock lock = this.lock;
    lock.lock();
    try {
        //获取barrier当前的 “代”也就是当前循环
        final Generation g = generation;
        if (g.broken)
            throw new BrokenBarrierException();
        if (Thread.interrupted()) {
            breakBarrier();
            throw new InterruptedException();
        }
        // 每来一个线程调用await方法都会进行减1
        int index = --count;
        if (index == 0) {  // tripped
            boolean ranAction = false;
            try {
                final Runnable command = barrierCommand;
                // new CyclicBarrier 传入 的barrierCommand, command.run()这个方法是同步的，如果耗时比较多的话,是否执行的时候需要考虑下是否异步来执行。
                if (command != null)
                    command.run();
                ranAction = true;
                // 这个方法1. 唤醒所有阻塞的线程，2. 重置下count（count 每来一个线程都会进行减1）和generation，以便于下次循环。
                nextGeneration();
                return 0;
            } finally {
                if (!ranAction)
                    breakBarrier();
            }
        }
        // loop until tripped, broken, interrupted, or timed out
        for (;;) {
            try {
                // 进入if条件，说明是不带超时的await
                if (!timed)
                    // 当前线程会释放掉lock，然后进入到trip条件队列的尾部，然后挂起自己，等待被唤醒。
                    trip.await();
                else if (nanos > 0L)
                    //说明当前线程调用await方法时 是指定了 超时时间的！
                    nanos = trip.awaitNanos(nanos);
            } catch (InterruptedException ie) {
                //Node节点在 条件队列内 时 收到中断信号时 会抛出中断异常！
                //g == generation 成立，说明当前代并没有变化。
                //! g.broken 当前代如果没有被打破，那么当前线程就去打破，并且抛出异常..
                if (g == generation && ! g.broken) {
                    breakBarrier();
                    throw ie;
                } else {
                    // We're about to finish waiting even if we had not
                    // been interrupted, so this interrupt is deemed to
                    // "belong" to subsequent execution.
                    //执行到else有几种情况？
                    //1.代发生了变化，这个时候就不需要抛出中断异常了，因为 代已经更新了，这里唤醒后就走正常逻辑了..只不过设置下 中断标记。
                    //2.代没有发生变化，但是代被打破了，此时也不用返回中断异常，执行到下面的时候会抛出  brokenBarrier异常。也记录下中断标记位。
                    Thread.currentThread().interrupt();
                }
            }
            //唤醒后，执行到这里，有几种情况？
            //1.正常情况，当前barrier开启了新的一代（trip.signalAll()）
            //2.当前Generation被打破，此时也会唤醒所有在trip上挂起的线程
            //3.当前线程trip中等待超时，然后主动转移到 阻塞队列 然后获取到锁 唤醒。
            if (g.broken)
                throw new BrokenBarrierException();
            //唤醒后，执行到这里，有几种情况？
            //1.正常情况，当前barrier开启了新的一代（trip.signalAll()）
            //2.当前线程trip中等待超时，然后主动转移到 阻塞队列 然后获取到锁 唤醒。
            if (g != generation)
                return index;
            //唤醒后，执行到这里，有几种情况？
            //.当前线程trip中等待超时，然后主动转移到 阻塞队列 然后获取到锁 唤醒。
            if (timed && nanos <= 0L) {
                breakBarrier();
                throw new TimeoutException();
            }
        }
    } finally {
        lock.unlock();
    }
}
```
<a name="Je33S"></a>
### 小结
**到了这里可以知道为啥`CyclicBarrier`可以进行循环计数？**

- `CyclicBarrier`采用一个内部类`Generation`来维护当前循环，每一个`await`方法都会存储当前的`generation`，获取到相同`generation`对象的属于同一组，每当`count`的次数耗尽就会重新`new`一个`Generation`并且重新设置`count`的值为`parties`，表示进入下一次新的循环。

从这个`await`方法可以知道只要有一个线程被中断了，当代的 `generation`的`broken` 就会被设置为`true`，所以会导致其他的线程也会被抛出`BrokenBarrierException`。相当于一个失败其他也必须失败，感觉有“强一致性“的味道。
<a name="DdBI8"></a>
## 总结

- `CountDownLanch`是为计数器是设置一个值，当多次执行`countdown`后，计数器减为`0`的时候所有线程被唤醒，然后`CountDownLanch`失效，只能够使用一次。
- `CyclicBarrier`是当`count`为`0`时同样唤醒全部线程，同时会重新设置`count`为`parties`，重新`new`一个`generation`来实现重复利用。
