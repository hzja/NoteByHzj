Java 线程<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624008577915-1537c0a7-9197-4bcf-bf32-623f375c7900.png#clientId=ud44a91a5-cb25-4&from=paste&id=ufc47384f&originHeight=275&originWidth=425&originalType=url&ratio=3&status=done&style=shadow&taskId=ub49cb395-745b-4b9e-940f-daca9df9b26)
<a name="UyREF"></a>
## LockSupport基本概念
LockSupport是线程工具类，主要作用是阻塞和唤醒线程，底层实现依赖Unsafe，同时它还是锁和其他同步类实现的基础，LockSupport提供两类静态函数分别是`park`和`unpark`，即阻塞与唤醒线程，下面是两段代码示例
<a name="lAs7Z"></a>
### 示例-1
```java
public static void main(String[] agrs) throws InterruptedException {
        Thread th = new Thread(() -> {
            //阻塞当前线程
            LockSupport.park();
            System.out.println("子线程执行---------");
        });
        th.start();
        //睡眠2秒
        Thread.sleep(2000);
        System.out.println("主线程执行---------");
        //唤醒线程
        LockSupport.unpark(th);
    }
}
```
输出结果：
```java
主线程执行---------
子线程执行---------
```
上述示例中，子线程th调用`LockSupport.park()`阻塞，主线程睡眠2秒后，执行`LockSupport.unpark(th)`唤醒th线程，先阻塞后唤醒非常好理解，接下来再看下面的示例
<a name="LIxMl"></a>
### 示例-2
```java
public static void main(String[] agrs) throws InterruptedException {
        Thread th = new Thread(() -> {
            //唤醒当前线程
            LockSupport.unpark(Thread.currentThread());
            //阻塞当前线程
            LockSupport.park();
            System.out.println("子线程执行---------");
        });
        th.start();
        //睡眠2秒
        Thread.sleep(2000);
        System.out.println("主线程执行---------");
}
```
输出结果：
```java
子线程执行---------
主线程执行---------
```
嗯？先唤醒th线程，再阻塞th线程，最终th线程没有被阻塞，这是为什么？下面LockSupport的设计思路会为读者们解开疑惑，并更进一步明确是`park`和`unpark`的语义（从广义上来说`park`和`unpark`代表阻塞和唤醒）。
<a name="KMRgU"></a>
## 设计思路
LockSupport的设计思路是通过许可证来实现的，就像汽车上高速公路，入口处要获取通行卡，出口处要交出通行卡，如果没有通行卡就无法出站，当然可以选择补一张通行卡。<br />LockSupport会为使用它的线程关联一个许可证（`permit`）状态，`permit`的语义「是否拥有许可」，0代表否，1代表是，默认是0。

- `LockSupport.unpark`：指定线程关联的`permit`直接更新为1，如果更新前的`permit<1`，唤醒指定线程
- `LockSupport.park`：当前线程关联的`permit`如果>0，直接把`permit`更新为0，否则阻塞当前线程

![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624008577870-7fac36c3-4593-49c1-9cac-9c3bf313def8.png#clientId=ud44a91a5-cb25-4&from=paste&id=u01bf7e69&originHeight=1105&originWidth=993&originalType=url&ratio=3&status=done&style=shadow&taskId=ubc2c788a-2eab-4ecd-a296-a0900400a1b)

- 线程A执行`LockSupport.park`，发现`permit`为0，未持有许可证，阻塞线程A
- 线程B执行`LockSupport.unpark`（入参线程A），为A线程设置许可证，`permit`更新为1，唤醒线程A
- 线程B流程结束
- 线程A被唤醒，发现`permit`为1，消费许可证，`permit`更新为0
- 线程A执行临界区
- 线程A流程结束

经过上面的分析得出结论`unpark`的语义明确为「使线程持有许可证」，`park`的语义明确为「消费线程持有的许可」，所以`unpark`与`park`的执行顺序没有强制要求，只要控制好使用的线程即可，`unpark=>park`执行流程如下<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624008577999-5cf0d837-d630-4095-96de-664123a20c3b.png#clientId=ud44a91a5-cb25-4&from=paste&id=udab68404&originHeight=860&originWidth=715&originalType=url&ratio=3&status=done&style=shadow&taskId=u230a3c53-537a-4b9a-88e9-5b84b8178f4)

- `permit`默认是0，线程A执行`LockSupport.unpark`，`permit`更新为1，线程A持有许可证
- 线程A执行`LockSupport.park`，此时`permit`是1，消费许可证，`permit`更新为0
- 执行临界区
- 流程结束

最后再补充下`park`注意点，因`park`阻塞的线程不仅仅会被`unpark`唤醒，还可能会被线程中断（`Thread.interrupt`）唤醒，而且不会抛出InterruptedException异常，所以建议在park后自行判断线程中断状态，来做对应的业务处理。
<a name="t4C1K"></a>
### 优点
为什么推荐使用LockSupport来做线程的阻塞与唤醒（线程间协同工作），因为它具备如下优点

- 以线程为操作对象更符合阻塞线程的直观语义
- 操作更精准，可以准确地唤醒某一个线程（`notify`随机唤醒一个线程，`notifyAll`唤醒所有等待的线程）
- 无需竞争锁对象（以线程作为操作对象），不会因竞争锁对象产生死锁问题
- `unpark`与`park`没有严格的执行顺序，不会因执行顺序引起死锁问题，比如「`Thread.suspend`和`Thread.resume`」没按照严格顺序执行，就会产生死锁

另外LockSupport还提供了park的重载函数，提升灵活性

- `void parkNanos(long nanos)`：增加了超时机制
- `void parkUntil(long deadline)`：加入超时机制（指定到某个时间点，1970年到指定时间点的毫秒数）
- `void park(Object blocker)`：设置blocker对象，当线程没有许可证被阻塞时，该对象会被记录到该线程的内部，方便后续使用诊断工具进行问题排查
- `void parkNanos(Object blocker, long nanos)`：设置`blocker`对象，加入超时机制
- `void parkUntil(Object blocker, long deadline)`：设置`blocker`对象，加入超时机制（指定到某个时间点，1970年到指定时间点的毫秒数）

建议使用时，传入`blocker`对象，至于超时根据业务场景选择
<a name="hedzv"></a>
### 实践
使用LockSupport来完成一道阿里经典的多线程协同工作面试题。<br />有3个独立的线程，一个只会输出A，一个只会输出B，一个只会输出C，在三个线程启动的情况下，请用合理的方式让他们按顺序打印ABCABC。<br />思路如下

- 准备3个线程，分别固定打印A、B、C
- 线程输出完A、B、C后需要阻塞等待唤醒
- 额外准备第4个线程，作为另外3个线程的调度器，有序的控制3个线程执行

是不是很简单，下面通过代码来实践
```java
public static void main(String[] agrs) throws InterruptedException {

        LockSupportMain lockSupportMain = new LockSupportMain();
        
        //定义线程t1、t2、t3执行的函数方法
        Consumer<String> consumer = str -> {
            while (true) {
                //线程消费许可证，并传入blocker，方便后续排查问题
                LockSupport.park(lockSupportMain);
                //防止线程是因中断操作唤醒
                if (Thread.currentThread().isInterrupted()){
                    throw new RuntimeException("线程被中断，异常结束");
                }
                System.out.println(Thread.currentThread().getName() + ":" + str);
            }
        };
        
        /**
         * 定义分别输出A、B、C的线程
         */
        Thread t1 = new Thread(() -> {
            consumer.accept("A");
        },"T1");
        Thread t2 = new Thread(() -> {
            consumer.accept("B");
        },"T2");
        Thread t3 = new Thread(() -> {
            consumer.accept("C");
        },"T3");

        
        /**
         * 定义调度线程
         */
        Thread dispatch = new Thread(() -> {
            int i=0;
            try {
                while (true) {
                    if((i%3)==0) {
                        //线程t1设置许可证，并唤醒线程t1
                        LockSupport.unpark(t1);
                    }else if((i%3)==1) {
                        //线程t2设置许可证，并唤醒线程t2
                        LockSupport.unpark(t2);
                    }else {
                        //线程t3设置许可证，并唤醒线程t3
                        LockSupport.unpark(t3);
                    }
                    i++;
                    TimeUnit.MILLISECONDS.sleep(500);
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        });

        //启动相关线程
        t1.start();
        t2.start();
        t3.start();
        dispatch.start();
}
```
输出内容：
```java
T1:A
T2:B
T3:C
T1:A
T2:B
T3:C
T1:A
T2:B
T3:C
```
