Java Condition<br />`Condition`接口提供了与`Object`阻塞(`wait()`)与唤醒(`notify()`或`notifyAll()`)相似的功能，只不过`Condition`接口提供了更为丰富的功能，如：限定等待时长等。`Condition`需要与`Lock`结合使用，需要通过锁对象获取`Condition`。
<a name="geCVZ"></a>
## 一、基本使用
基于`Condition`实现生产者、消费者模式。代码基本与`Object#wait()`和`Object#notify()`类似，只不过这里使用`Lock`替换了`synchronized`关键字。
<a name="ARkYK"></a>
### 生产者
```java
public class Producer implements Runnable {
    private Lock lock;
    private Condition condition;
    private Queue<String> queue;
    private int maxSize;

    public Producer(Lock lock, Condition condition, Queue<String> queue, int maxSize) {
        this.lock = lock;
        this.condition = condition;
        this.queue = queue;
        this.maxSize = maxSize;
    }

    @Override
    public void run() {
        int i = 0;
        for (; ; ) {
            lock.lock();
            // 如果满了，则阻塞
            while (queue.size() == maxSize) {
                System.out.println("生产者队列满了，等待...");
                try {
                    condition.await();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            try {
                TimeUnit.SECONDS.sleep(2);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            queue.add("一个消息:" + ++i);
            System.out.printf("生产者%s生产了一个消息:%s\n", Thread.currentThread().getName(), i);
            condition.signal();
            lock.unlock();
        }
    }
}
```
<a name="ErgdF"></a>
### 消费者
```java
public class Consumer implements Runnable {
    private Lock lock;
    private Condition condition;
    private Queue<String> queue;
    private int maxSize;

    public Consumer(Lock lock, Condition condition, Queue<String> queue, int maxSize) {
        this.lock = lock;
        this.condition = condition;
        this.queue = queue;
        this.maxSize = maxSize;
    }

    @Override
    public void run() {
        for (; ; ) {
            lock.lock();
            while (queue.isEmpty()) {
                System.out.println("消费者队列为空，等待...");
                try {
                    condition.await();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            try {
                TimeUnit.SECONDS.sleep(1);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            String obj = queue.remove();
            System.out.printf("消费者%s消费一个消息：%s\n", Thread.currentThread().getName(), obj);
            condition.signal();
            lock.unlock();
        }
    }
}
```
<a name="Pg4Hw"></a>
### 测试类
```java
public class ConditionProducerConsumer {
    public static void main(String[] args) {
        Lock lock = new ReentrantLock();
        Condition condition = lock.newCondition();
        Queue<String> queue = new LinkedBlockingQueue<>();
        int maxSize = 10;

        Producer producer = new Producer(lock, condition, queue, maxSize);
        Consumer consumer = new Consumer(lock, condition, queue, maxSize);

        new Thread(producer).start();
        new Thread(consumer).start();

    }
}
```
<a name="lWv8z"></a>
## 二、源码分析
上述示例中使用的`Lock`是`ReentrantLock`，关于它的`lock`方法与`unlock`方法的原理详见`ReentrantLock`实现原理。上述示例中的`Condition`对象是调用了`Lock#newCondition()`方法，源码如下：
```java
public class ReentrantLock implements Lock, java.io.Serializable {
    ...
        public Condition newCondition() {
        return sync.newCondition();
    }

    abstract static class Sync extends AbstractQueuedSynchronizer {
        ...
            final ConditionObject newCondition() {
            return new ConditionObject();
        }
        ...
        }
    ...
}
```
上述的`ConditionObject`定义在AQS中，如下:
```java
public abstract class AbstractQueuedSynchronizer
    extends AbstractOwnableSynchronizer
    implements java.io.Serializable {
    ...
 	public class ConditionObject implements Condition, java.io.Serializable {
  		...
 	}
 ...
}
```
首先来分析下`Condition#await()`方法
```java
public final void await() throws InterruptedException {
    if (Thread.interrupted())
        throw new InterruptedException();
    Node node = addConditionWaiter();
    int savedState = fullyRelease(node);
    int interruptMode = 0;
    while (!isOnSyncQueue(node)) {
        LockSupport.park(this);
        if ((interruptMode = checkInterruptWhileWaiting(node)) != 0)
            break;
    }
    if (acquireQueued(node, savedState) && interruptMode != THROW_IE)
        interruptMode = REINTERRUPT;
    if (node.nextWaiter != null) // clean up if cancelled
        unlinkCancelledWaiters();
    if (interruptMode != 0)
        reportInterruptAfterWait(interruptMode);
}

private Node addConditionWaiter() {
    Node t = lastWaiter;
    // If lastWaiter is cancelled, clean out.
    if (t != null && t.waitStatus != Node.CONDITION) {
        unlinkCancelledWaiters();
        t = lastWaiter;
    }
    Node node = new Node(Thread.currentThread(), Node.CONDITION);
    if (t == null)
        firstWaiter = node;
    else
        t.nextWaiter = node;
    lastWaiter = node;
    return node;
}
```
根据AQS队列的特性，若有多个线程执行`lock#lock()`方法，会将处于阻塞状态的线程维护到一个双向链表中，如下：<br />![2021-07-24-18-34-19-791476.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627122882392-45ee7c24-8ee7-4004-9af8-0a16caae9974.png#clientId=ud3ed44be-dfb3-4&from=ui&id=ud139ee66&originHeight=285&originWidth=899&originalType=binary&ratio=1&size=6847&status=done&style=shadow&taskId=ufc292116-ee05-416a-b3b6-9d4bb850179)<br />假设当前是线程A获取到锁，其他线程执行`lock#lock()`方法时，将会构建成一个上述链表。<br />若获取锁的线程（线程A）执行`Condition#await()`方法，则会将当前线程添加至`Condition`队列中，如下：<br />![2021-07-24-18-34-19-971481.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627122891295-fc378832-1527-4b4e-88be-c9df5020fc94.png#clientId=ud3ed44be-dfb3-4&from=ui&id=u5892d12d&originHeight=495&originWidth=876&originalType=binary&ratio=1&size=12063&status=done&style=shadow&taskId=uf043b474-6cd4-44f7-b0cb-07d77d1e104)<br />然后在调用`fullyRelease()`方法时会释放当前线程的锁，然后唤醒处于阻塞队列中的下一个线程：<br />![2021-07-24-18-34-20-079457.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627122899824-2a08046c-0888-4c11-bcd8-104b4d12bf2e.png#clientId=ud3ed44be-dfb3-4&from=ui&id=uf18640fb&originHeight=501&originWidth=895&originalType=binary&ratio=1&size=25907&status=done&style=shadow&taskId=u791c52da-8ad7-47e3-9798-0159f511722)<br />在调用`isOnSyncQueue()`方法时会检查当前节点是否在同步队列中，若不存在，则会调用`LockSupport.park()`进行阻塞。<br />假设当前线程A是生产者线程，调用`await()`方法后，会释放锁，并且将当前线程加入到`Condition`队列中。此时，消费者能获取到锁资源，然后继续执行。假设线程B是消费者线程，当添加一个元素后会调用`condition#signal()`方法，定义如下：
```java
public final void signal() {
    if (!isHeldExclusively())
        throw new IllegalMonitorStateException();
    Node first = firstWaiter;
    if (first != null)
        doSignal(first);
}

private void doSignal(Node first) {
    do {
        if ( (firstWaiter = first.nextWaiter) == null)
            lastWaiter = null;
        first.nextWaiter = null;
    } while (!transferForSignal(first) &&
             (first = firstWaiter) != null);
}

final boolean transferForSignal(Node node) {
    /*
         * If cannot change waitStatus, the node has been cancelled.
         */
    if (!compareAndSetWaitStatus(node, Node.CONDITION, 0))
        return false;

    /*
         * Splice onto queue and try to set waitStatus of predecessor to
         * indicate that thread is (probably) waiting. If cancelled or
         * attempt to set waitStatus fails, wake up to resync (in which
         * case the waitStatus can be transiently and harmlessly wrong).
         */
    Node p = enq(node);
    int ws = p.waitStatus;
    if (ws > 0 || !compareAndSetWaitStatus(p, ws, Node.SIGNAL))
        LockSupport.unpark(node.thread);
    return true;
}
```
执行`signal()`方法，会将`Condition`队列中的第一个节点移除，将其变为同步队列中的尾结点，如下：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1627122599118-6b67971a-3237-438b-8d9d-47415e464e22.png#clientId=ud3ed44be-dfb3-4&from=paste&id=u5e1856c5&originHeight=417&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=ua1678dbf-7602-4b39-9b07-578cc5832ef)<br />至此，完成了`Condition`队列转换为同步队列的过程。后续流程基本就是重复以上操作。<br />详细介绍了单个`Condition`队列的执行流程，其实一个`Lock`中可以有多个`Condition`队列，比如：JUC中提供的`LinkedBlockingDeque`、`ArrayBlockingQueue`等。
