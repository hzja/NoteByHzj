Java 线程
<a name="SChrr"></a>
## 前言
Java 线程通信是将多个独立的线程个体进行关联处理，使得线程与线程之间能进行相互通信。比如线程 A 修改了对象的值，然后通知给线程 B，使线程 B 能够知道线程 A 修改的值，这就是线程通信。
<a name="nuKr7"></a>
## `wait`/`notify` 机制
一个线程调用 Object 的 `wait()` 方法，使其线程被阻塞；另一线程调用 Object 的 `notify()`/`notifyAll()` 方法，`wait()` 阻塞的线程继续执行。<br />`wait`/`notify` 方法

| 方法 | 说明 |
| --- | --- |
| `wait()` | 当前线程被阻塞，线程进入 `WAITING` 状态 |
| `wait(long)` | 设置线程阻塞时长，线程会进入 `TIMED_WAITING` 状态。如果设置时间内（毫秒）没有通知，则超时返回 |
| `wait(long, int)` | 纳秒级别的线程阻塞时长设置 |
| `notify()` | 通知同一个对象上已执行 `wait()` 方法且获得对象锁的等待线程 |
| `notifyAll()` | 通知同一对象上所有等待的线程 |

实现 `wait`/`notify` 机制的条件：

- 调用 wait 线程和 `notify` 线程必须拥有相同对象锁。
- `wait()` 方法和 `notify()`/`notifyAll()` 方法必须在 `Synchronized` 方法或代码块中。

由于 `wait`/`notify` 方法是定义在java.lang.Object中，所以在任何 Java 对象上都可以使用。
<a name="xWQwF"></a>
## `wait` 方法
在执行 `wait()` 方法前，当前线程必须已获得对象锁。调用它时会阻塞当前线程，进入等待状态，在当前 `wait()` 处暂停线程。同时，`wait()` 方法执行后，会立即释放获得的对象锁。<br />下面通过案例来查看 `wait()` 释放锁。<br />首先查看不使用 `wait()` 方法时的代码执行情况：
```java
public class WaitTest {
    
    static Object object = new Object();
    
    public static void main(String[] args) {

        new Thread(() -> {
            synchronized (object){
                System.out.println("开始线程 A");
                try {
                    Thread.sleep(2000L);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                System.out.println("结束线程 A");
            }
        }, "线程 A").start();


        new Thread(() -> {
            try {
                Thread.sleep(500L);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            synchronized (object){
                System.out.println("开始线程 B");

                System.out.println("结束线程 B");
            }
        }, "线程 B").start();
    }
}
```
创建 A、B 两个线程，。首先在 B 线程创建后 sleep ，保证 B 线程的打印后于 A 线程执行。在 A 线程中，获取到对象锁后，sleep 一段时间，且时间大于 B 线程的 sleep 时间。<br />执行结果为：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623509736397-b2803324-0b73-483d-92e8-61041d871e06.webp#clientId=u22c4e6ce-3139-4&from=paste&id=u5e77beb8&originHeight=179&originWidth=608&originalType=url&ratio=3&status=done&style=none&taskId=u92e35798-a0ab-4295-81c4-33010cde226)<br />从上图结果中，可以看到，B 线程一定等 A 线程执行完 `synchronize` 代码块释放对象锁后 A 线程再获取对象锁进入 `synchronize` 代码块中。在这过程中，`Thread.sleep()` 方法也不会释放锁。<br />当前在 A 线程 `synchronize` 代码块中执行 `wait()` 方法后，就会主动释放对象锁，A 线程代码如下：
```java
new Thread(() -> {
    synchronized (object){
        System.out.println("开始线程 A");
        try {
            // 调用 object 对象的 wait 方法
            object.wait();
            Thread.sleep(2000L);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("结束线程 A");
    }
}, "线程 A").start();
```
执行结果：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623509736767-c012f036-6675-47fc-9594-c954914bae0d.webp#clientId=u22c4e6ce-3139-4&from=paste&id=u230c3ce2&originHeight=176&originWidth=616&originalType=url&ratio=3&status=done&style=none&taskId=u769a6aa4-9959-4ebd-b8f7-f52c00c5be4)<br />同时 A 线程一直处于阻塞状态，不会打印结束线程 A。<br />`wait(long)` 方法是设置超时时间，当等待时间大于设置的超时时间后，会继续往 `wait(long)` 方法后的代码执行。
```java
new Thread(() -> {
    synchronized (object){
        System.out.println("开始线程 A");
        try {
            object.wait(1000);
            Thread.sleep(2000L);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("结束线程 A");
    }
}, "线程 A").start();
```
执行结果<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623509736590-3ea10665-a2df-4a9c-8d6c-d79ff8d00164.webp#clientId=u22c4e6ce-3139-4&from=paste&id=uaf0212ec&originHeight=186&originWidth=596&originalType=url&ratio=3&status=done&style=none&taskId=ue877cb9b-a15b-4c49-902e-6630a717611)<br />同理，`wait(long, int)` 方法与 `wait(long)` 同样，只是多个纳秒级别的时间设置。
<a name="AXq3K"></a>
## `notify` 方法
同样，在执行 `notify()` 方法前，当前线程也必须已获得线程锁。调用 `notify()` 方法后，会通知一个执行了 `wait()` 方法的阻塞等待线程，使该等待线程重新获取到对象锁，然后继续执行 `wait()` 后面的代码。但是，与 `wait()` 方法不同，执行 `notify()` 后，不会立即释放对象锁，而需要执行完 `synchronize` 的代码块或方法才会释放锁，所以接收通知的线程也不会立即获得锁，也需要等待执行 `notify()` 方法的线程释放锁后再获取锁。
<a name="hkWU5"></a>
### `notify()`
下面是 `notify()` 方法的使用，实现一个完整的 `wait`/`notify` 的例子，同时验证发出通知后，执行 `notify()` 方法的线程是否立即释放锁，执行 `wait()` 方法的线程是否立即获取锁。
```java
public class WaitNotifyTest {

    static Object object = new Object();

    public static void main(String[] args) {
        System.out.println();

        new Thread(() -> {
            synchronized (object){
                System.out.println("开始线程 A");
                try {
                    object.wait();
                    System.out.println("A 线程重新获取到锁，继续进行");
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                System.out.println("结束线程 A");
            }
        }, "线程 A").start();


        new Thread(() -> {
            try {
                Thread.sleep(500L);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            synchronized (object){
                System.out.println("开始线程 B");
                object.notify();
                System.out.println("线程 B 通知完线程 A");
                try {
                    // 试验执行完 notify() 方法后，A 线程是否能立即获取到锁
                    Thread.sleep(2000L);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                System.out.println("结束线程 B");
            }
        }, "线程 B").start();
    }
}
```
以上 A 线程执行 `wait()` 方法，B 线程执行 `notify()` 方法，执行结果为：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623509736816-2588266b-7b58-4c16-a41b-e68e5ce2d08c.webp#clientId=u22c4e6ce-3139-4&from=paste&id=ubdd5806c&originHeight=260&originWidth=625&originalType=url&ratio=3&status=done&style=none&taskId=ua5398da5-0b34-4f6a-af16-04e90ec9069)<br />执行结果中可以看到，B 线程执行 `notify()` 方法后，即使 sleep 了，A 线程也没有获取到锁，可知，`notify()` 方法并没有释放锁。<br />`notify()` 是通知到等待中的线程，但是调用一次 `notify()` 方法，只能通知到一个执行 `wait()` 方法的等待线程。如果有多个等待状态的线程，则需多次调用 `notify()` 方法，通知到线程顺序则根据执行 `wait()` 方法的先后顺序进行通知。<br />下面创建有两个执行 `wait()` 方法的线程的代码：
```java
public class MultiWaitNotifyTest {

    static Object object = new Object();

    public static void main(String[] args) {
        System.out.println();

        new Thread(() -> {
            synchronized (object){
                System.out.println("开始线程 A");
                try {
                    object.wait();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                System.out.println("结束线程 A");
            }
        }, "线程 A").start();


        new Thread(() -> {
            try {
                Thread.sleep(500L);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            synchronized (object){
                System.out.println("开始线程 B");
                try {
                    object.wait();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                System.out.println("结束线程 B");
            }
        }, "线程 B").start();


        new Thread(() -> {
            try {
                Thread.sleep(3000L);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            synchronized (object){
                System.out.println("开始通知线程 C");
                object.notify();
                object.notify();
                System.out.println("结束通知线程 C");
            }
        }, "线程 C").start();
    }
}
```
先 A 线程执行 `wait()` 方法，然后 B 线程执行 `wait()` 方法，最后 C 线程调用两次 `notify()` 方法，执行结果：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623509736762-008a2bf1-ec1f-461e-9077-a221366122f4.webp#clientId=u22c4e6ce-3139-4&from=paste&id=ufa7c3e5e&originHeight=227&originWidth=610&originalType=url&ratio=3&status=done&style=none&taskId=ue47510d4-f45f-413e-b333-3a19ccf922f)
<a name="Z9Sn2"></a>
### `notifyAll()`
通知多个等待状态的线程，通过多次调用 `notify()` 方法实现的方案，在实际应用过程中，实现过程不太友好，如果是想通知所有等待状态的线程，可使用 `notifyAll()` 方法，就能唤醒所有线程。<br />实现方式，只需将上面 C 线程的多次调用 `notify()` 方法部分改为调用一次 `notifyAll()` 方法即可。
```java
new Thread(() -> {
    try {
        Thread.sleep(3000L);
    } catch (InterruptedException e) {
        e.printStackTrace();
    }
    synchronized (object){
        System.out.println("开始通知线程 C");
        object.notifyAll();
        System.out.println("结束通知线程 C");
    }
}, "线程 C").start();
```
执行结果：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623509737340-e56a940b-dd66-49d4-9785-7cc79ccb5c57.webp#clientId=u22c4e6ce-3139-4&from=paste&id=u07bd2fa2&originHeight=229&originWidth=592&originalType=url&ratio=3&status=done&style=none&taskId=u93a66cb2-863f-48d8-a3b2-f75ef148f1e)<br />根据不同 JVM 的实现，`notifyAll()` 的唤醒顺序会有所不同，当前测试环境中，以倒序顺序唤醒线程。
<a name="wTGQl"></a>
## 实现生产者消费者模式
生产消费者模式就是一个线程生产数据进行存储，另一线程进行数据提取消费。下面就以两个线程来模拟，生产者生成一个 UUID 存放到 List 对象中，消费者读取 List 对象中的数据，读取完成后进行清除。<br />实现代码如下：
```java
import java.util.ArrayList;
import java.util.List;
import java.util.UUID;

public class WaitNotifyModelTest {

    // 存储生产者产生的数据
    static List<String> list = new ArrayList<>();

    public static void main(String[] args) {

        new Thread(() -> {
            while (true){
                synchronized (list){
                    // 判断 list 中是否有数据，如果有数据的话，就进入等待状态，等数据消费完
                    if (list.size() != 0){
                        try {
                            list.wait();
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                    }

                    // list 中没有数据时，产生数据添加到 list 中
                    list.add(UUID.randomUUID().toString());
                    list.notify();
                    System.out.println(Thread.currentThread().getName() + list);
                }
            }
        }, "生产者线程 A ").start();


        new Thread(() -> {
            while (true){
                synchronized (list){
                    // 如果 list 中没有数据，则进入等待状态，等收到有数据通知后再继续运行
                    if (list.size() == 0){
                        try {
                            list.wait();
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                    }

                    // 有数据时，读取数据
                    System.out.println(Thread.currentThread().getName() + list);
                    list.notify();
                    // 读取完毕，将当前这条 UUID 数据进行清除
                    list.clear();
                }
            }
        }, "消费者线程 B ").start();
    }
}
```
运行结果：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623509737514-f2d71bba-eb54-4c10-ad4f-b0e90adc55e0.webp#clientId=u22c4e6ce-3139-4&from=paste&id=uc813089f&originHeight=156&originWidth=640&originalType=url&ratio=3&status=done&style=none&taskId=u65e6b1e7-2292-4666-9da2-88db539f103)<br />生产者线程运行时，如果已存在未消费的数据，则当前线程进入等待状态，收到通知后，表明数据已消费完，再继续向 list 中添加数据。<br />消费者线程运行时，如果不存在未消费的数据，则当前线程进入等待状态，收到通知后，表明 List 中已有新数据被添加，继续执行代码消费数据并清除。<br />不管是生产者还是消费者，基于对象锁，一次只能一个线程能获取到，如果生产者获取到锁就校验是否需要生成数据，如果消费者获取到锁就校验是否有数据可消费。<br />一个简单的生产者消费者模式就以完成。
<a name="iC4j4"></a>
## 总结
等待/通知机制是实现 Java 线程间通信的一种方式，将多线程中，各个独立运行的线程通过相互通信来更高效的协作完成工作，更大效率利用 CPU 处理程序。
