Java 高并发 
<a name="PoS9L"></a>
## Phaser
<a name="UG2sG"></a>
### Phaser简介
**Phaser **是一个更加复杂和强大的同步辅助类，对 `CountDownLatch` 与 `CyclicBarrier` 的全面升级，是一个 java 并发 api 的一个重量级类。
<a name="sbLWG"></a>
### Phaser常用方法
常用api：

- `arriveAndAwaitAdvance()`每凑齐指定人数就报团执行一次，同一个线程可以执行多次`arriveAndAwaitAdvance()`，表示不同阶段的报团
- `arriveAndDeregister()`退出当前团，且当前团规则人数减1（报完当前团后，不再报下阶段的团）
- `getArrivedParties()`当前团凑足了多少人
- `getRegisteredParties()`获取注册的团规定人数
- `arrive()`使`getArrivedParties()`数量加1，即用一个虚拟线程占据一个线程的位置， 此虚拟线程不阻塞
- `register()`动态增加一个团的规定人数
- `bulkRegister(int parties)`动态的增加规定报团人数，是`register()`的多次调用版
- `forceTermination()`取消报团，线程执行各自代码，不再有Phaser阻塞等待情况
- `getUnarrivedParties()`当前还差多少线程开团，是`getArrivedParties()`方法的补集
- `isTerminated()`判断Phaser对象是否已为销毁状态
<a name="6Garg"></a>
### 使用案例
<a name="HIGrj"></a>
#### 作CountDownLatch使用
```java
import java.util.Date;
import java.util.concurrent.Phaser;
import java.util.concurrent.TimeUnit;
import static java.util.concurrent.ThreadLocalRandom.current;
import static java.lang.Thread.currentThread;
/**
 * 将 Phaser当作 CountDownLatch来用
 * 
 **/
public class PhaserForCountDownLatch {
    public static void main(String[] args) throws InterruptedException {
        // 定义一个 Phaser ， 并未指定“分片数量 parties”，此时在 Phaser 内部分片的数量 parties 默认为 0 ，
        // 后面可以通过 register() 方法来动态增加
        final Phaser phaser = new Phaser();
        // 定义 5 个线程
        for (int i = 0; i < 5; i++) {
            new Thread(() -> {
                // 调用 Phaser 的 register() 方法使得 phaser 内部的 parties 加一
                phaser.register();
                try {
                    // 采用随机休眠的方式模拟线程的运行时间开销
                    TimeUnit.SECONDS.sleep(current().nextInt(20));
                    // 线程任务结束，执行 arrive()
                    /**
                     * 补充：arrive() 方法类似于 CountDownLatch 的 countdown()
                     * 方法，代表着“当前线程已经到达屏障”，
                     * 但是它不需要等待其他的线程也到达屏障。因此该方法“不是阻塞的方法”，执行之后会立即返回，
                     * 同时该方法会返回一个整数类型的数字，代表着已经到达的 Phase(阶段)编号
                     */
                    phaser.arrive();
                    System.out.println(new Date() + ":" + currentThread() + " completed the work.");
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }, "T-" + i).start();
        }
        /**
         * 这里让线程休眠的目的： 为了保证在主线程 register() 之前，所有的子线程都能顺利 register ， 否则就会出现
         * phaser 只注册一个 parties ， 并且很快 arrive 的情况。
         */
        TimeUnit.SECONDS.sleep(current().nextInt(10));
        // 主线程也调用注册方法
        phaser.register();
        // 主线程也 arrive() ， 但是它要等待下一个阶段，等待下一个阶段的前提“所有的线程都 arrive ,
        // 也就是 phaser 内部当前 phase 的 unarrived 数量为 0 ”
        phaser.arriveAndAwaitAdvance();
        System.out.println(new Date() + ": all of sub task completed work.");
    }
}
```
<a name="PEmwj"></a>
#### 作CyclicBarrier使用
```java
/**
 * 将 Phaser 当作 CyclicBarrier 来使用
 *
 *  phaser.arriveAndAwaitAdvance(): 该方法会等待当前 Phaser 中所有的 part（子线程）都完成了
 *                                  任务才能使线程退出阻塞状态
 **/
public class PhaserForCyclicBarrier
{
   public static void main(String[] args) throws InterruptedException
   {
       // 定义一个分片 parties 为0 的 Phaser
      final Phaser phaser = new Phaser();
      for (int i = 0; i < 5; i++)
      {
         new Thread(()->
         {
            // 子线程调用注册方法
            phaser.register();
            try
            {
               TimeUnit.SECONDS.sleep(current().nextInt(20));
               // 调用 arriveAndAwaitAdvance() 等待所有线程 arrive 然后继续前行
               phaser.arriveAndAwaitAdvance();
               System.out.println(new Date() + ":" + currentThread() + " completed the work.");
            }
            catch (InterruptedException e)
            {
               e.printStackTrace();
            }
         } , "T-"+i).start();
      }
      // 休眠以确保其他子线程顺利调用 register()
      TimeUnit.SECONDS.sleep(10);
      // 主线程调用 register()
      phaser.register();
      phaser.arriveAndAwaitAdvance();
      System.out.println(new Date() + ": all of sub task completed work.");
   }
}
```
<a name="cFPyz"></a>
## Exchanger
<a name="jGEhg"></a>
### Exchanger简介
它提供一个同步点，在这个同步点两个线程可以交换彼此的数据。这个两个线程通过exchange方法交换数据，如果第一个线程先执行exchange方法，它会一直等待第二个线程也执行exchange，当两个线程都到达同步点时，这两个线程就可以交换数据，将本线程生产出来的数据传递给对方。因此使用Exchanger的中断时成对的线程使用`exchange()`方法，当有一对线程到达了同步点，就会进行交换数据，因此该工具类的线程对象是成对的。<br />线程可以在成对内配对和交换元素的同步点。每个线程在输入exchange方法时提供一些对象，与合作者线程匹配，并在返回时接收其合作伙伴的对象。交换器可以被视为一个的双向形式的SynchroniuzedQueue。交换器在诸如遗传算法和管道设计的应用中可能是有用的。<br />一个用于两个工作线程之间交换数据的封装工具类，简单说就是一个线程在完成一定事务后想与另一个线程交换数据，则第一个先拿出数据的线程会一直等待第二个线程，直到第二个线程拿着数据到来时才能彼此交换对应数据。**![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1612705407898-c08b5a11-e8ea-4a60-9fbe-76affc1c1741.webp#align=left&display=inline&height=319&originHeight=509&originWidth=1080&size=0&status=done&style=shadow&width=677)**
<a name="KIUni"></a>
### 常用方法

- Exchanger 泛型类型，其中V表示可交换的数据类型
- `V exchanger(V v)`：等待另一个线程到达此交换点（除非当前线程被中断），然后将给定的对象传送该线程，并接收该线程的对象。
- `V exchanger(V v, long timeout, TimeUnit unit)`：等待另一个线程到达此交换点（除非当前线程被中断或超出类指定的等待时间），然后将给定的对象传送给该线程，并接收该线程的对象。
```java
import java.util.concurrent.Exchanger;
public class ExechangerExample {
    public static void main(String[] args) {
        Exchanger<String> exchanger = new Exchanger<>();
        new Thread() {
            @Override
            public void run() {
                String data1 = "data1";
                try {
                    System.out.println(Thread.currentThread().getName() + "交换前的数据:" + data1);
                    String data2 = exchanger.exchange(data1);
                    System.out.println(Thread.currentThread().getName() + "交换后的数据:" + data2);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }.start();
        new Thread() {
            @Override
            public void run() {
                String data2 = "data2";
                try {
                    System.out.println(Thread.currentThread().getName() + "交换前的数据:" + data2);
                    String data1 = exchanger.exchange(data2);
                    System.out.println(Thread.currentThread().getName() + "交换后的数据:" + data1);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }.start();
    }
}
```
