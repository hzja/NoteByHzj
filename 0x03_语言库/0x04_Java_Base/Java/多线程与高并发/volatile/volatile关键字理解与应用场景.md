Java volatile<br />`volatile`关键字是由JVM提供的最轻量级同步机制。与被滥用的`synchronized`不同，并不习惯使用它。想要正确且完全的理解它并不容易。
<a name="A7rHy"></a>
## 1、Java内存模型
Java内存模型由Java虚拟机规范定义，用来屏蔽各个平台的硬件差异。简单来说:

- 所有变量储存在主内存。
- 每条线程拥有自己的工作内存，其中保存了主内存中线程使用到的变量的副本。
- 线程不能直接读写主内存中的变量，所有操作均在工作内存中完成。线程，主内存，工作内存的交互关系如图。

![2021-08-26-21-59-25-138407.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629986415286-942fb00f-ba3c-4dd6-bad6-c4c7331cfa74.png#clientId=u9b75ef8d-9946-4&from=ui&id=u78b5bbb7&originHeight=436&originWidth=956&originalType=binary&ratio=1&size=1252982&status=done&style=shadow&taskId=uc6bcbaa4-12a9-4626-b959-69d2ee83476)<br />内存间的交互操作有很多，和`volatile`有关的操作为：

- `read`（读取）：作用于主内存变量，把一个变量值从主内存传输到线程的工作内存中，以便随后的`load`动作使用
- `load`（载入）：作用于工作内存的变量，它把read操作从主内存中得到的变量值放入工作内存的变量副本中。
- `use`（使用）：作用于工作内存的变量，把工作内存中的一个变量值传递给执行引擎，每当虚拟机遇到一个需要使用变量的值的字节码指令时将会执行这个操作。
- `assign`（赋值）：作用于工作内存的变量，它把一个从执行引擎接收到的值赋值给工作内存的变量，每当虚拟机遇到一个给变量赋值的字节码指令时执行这个操作。
- `store`（存储）：作用于工作内存的变量，把工作内存中的一个变量的值传送到主内存中，以便随后的`write`的操作。
- `write`（写入）：作用于主内存的变量，它把`store`操作从工作内存中一个变量的值传送到主内存的变量中。

对被`volatile`修饰的变量进行操作时，需要满足以下规则：

- 规则1：线程对变量执行的前一个动作是`load`时才能执行`use`，反之只有后一个动作是use时才能执行load。线程对变量的`read`，`load`，`use`动作关联，必须连续一起出现。-----这保证了线程每次使用变量时都需要从主存拿到最新的值，保证了其他线程修改的变量本线程能看到。
- 规则2：线程对变量执行的前一个动作是`assign`时才能执行`store`，反之只有后一个动作是`store`时才能执行`assign`。线程对变量的`assign`，`store`，`write`动作关联，必须连续一起出现。-----这保证了线程每次修改变量后都会立即同步回主内存，保证了本线程修改的变量其他线程能看到。
- 规则3：有线程T，变量V、变量W。假设动作A是T对V的`use`或`assign`动作，P是根据规则2、3与A关联的`read`或`write`动作；动作B是T对W的`use`或`assign`动作，Q是根据规则2、3与B关联的read或`write`动作。如果A先与B，那么P先与Q。------这保证了`volatile`修饰的变量不会被指令重排序优化，代码的执行顺序与程序的顺序相同。
<a name="OTZm4"></a>
## 2、使用`volatile`关键字的特性
<a name="IeNsT"></a>
### 被`volatile`修饰的变量保证对所有线程可见。
由上文的规则1、2可知，`volatile`变量对所有线程是立即可见的，在各个线程中不存在一致性问题。那么，是否能得出结论：`volatile`变量在并发运算下是线程安全的呢？这确实是一个非常常见的误解，写个简单的例子：
```java
public class VolatileTest extends Thread{
    static volatile int increase = 0;
    static AtomicInteger aInteger=new AtomicInteger();//对照组
    static void increaseFun() {
        increase++;
        aInteger.incrementAndGet();
    }
    public void run(){
        int i=0;
        while (i < 10000) {
            increaseFun();
            i++;
        }
    }
    public static void main(String[] args) {
        VolatileTest vt = new VolatileTest();
        int THREAD_NUM = 10;
        Thread[] threads = new Thread[THREAD_NUM];
        for (int i = 0; i < THREAD_NUM; i++) {
            threads[i] = new Thread(vt, "线程" + i);
            threads[i].start();
        }
        //idea中会返回主线程和守护线程，如果用Eclipse的话改为1
        while (Thread.activeCount() > 2) {
            Thread.yield();
        }
        System.out.println("volatile的值: "+increase);
        System.out.println("AtomicInteger的值: "+aInteger);
    }
}
```
这个程序跑了10个线程同时对`volatile`修饰的变量进行10000的自增操作(`AtomicInteger`实现了原子性，作为对照组)，如果`volatile`变量是并发安全的话，运行结果应该为100000，可是多次运行后，每次的结果均小于预期值。显然上文的说法是有问题的。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629987666395-5db98414-afc3-41b1-96e2-f3b6694f00b8.png#clientId=u9b75ef8d-9946-4&from=paste&height=145&id=u811fbcee&originHeight=434&originWidth=1886&originalType=binary&ratio=1&size=70200&status=done&style=shadow&taskId=u96c193e5-543c-4f5f-9090-eeb4ba5ebe3&width=628.6666666666666)<br />`volatile`修饰的变量并不保值原子性，所以在上述的例子中，用`volatile`来保证线程安全不靠谱。用`Javap`对这段代码进行反编译，为什么不靠谱简直一目了然：<br />![2021-08-26-21-59-25-310405.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629987726386-1819529d-2c3c-46d3-b20d-d4c8d0db3909.png#clientId=u9b75ef8d-9946-4&from=ui&id=u4874bdbc&originHeight=257&originWidth=983&originalType=binary&ratio=1&size=759453&status=done&style=shadow&taskId=u1f1d4167-30d9-4ed3-a11a-8c7a505fc16)<br />`getstatic`指令把`increase`的值拿到了操作栈的顶部，此时由于`volatile`的规则，该值是正确的。<br />`iconst_1`和`iadd`指令在执行的时候`increase`的值很有可能已经被其他线程加大，此时栈顶的值过期。<br />`putstatic`指令接着把过期的值同步回主存，导致了最终结果较小。<br />`volatile`关键字只保证可见性，所以在以下情况中，需要使用锁来保证原子性：

- 运算结果依赖变量的当前值，并且有不止一个线程在修改变量的值。
- 变量需要与其他状态变量共同参与不变约束

那么`volatile`的这个特性的使用场景是什么呢？

- 模式1：状态标志
- 模式2：独立观察（independent observation）
- 模式3：“volatile bean” 模式
- 模式4：开销较低的“读－写锁”策略
<a name="b7QN9"></a>
### 禁止指令重排序优化
由上文的规则3可知，`volatile`变量的第二个语义是禁止指令重排序。指令重排序是什么？简单点说就是 jvm会把代码中没有依赖赋值的地方打乱执行顺序，由于一些规则限定，在单线程内观察不到打乱的现象（线程内表现为串行的语义），但是在并发程序中，从别的线程看另一个线程，操作是无序的。一个非常经典的指令重排序例子：
```java
public class SingletonTest {
    private volatile static SingletonTest instance = null;
    private SingletonTest() { }
    public static SingletonTest getInstance() {
        if(instance == null) {
            synchronized (SingletonTest.class){
                if(instance == null) {
                    instance = new SingletonTest();  //非原子操作
                }
            }
        }
        return instance;
    }
}
```
这是单例模式中的“双重检查加锁模式”，可以看到`instance`用了`volatile`修饰，由于 `instance = new SingletonTest();`可分解为：

1. `memory =allocate(); //分配对象的内存空间`
2. `ctorInstance(memory); //初始化对象`
3. `instance =memory; //设置instance指向刚分配的内存地址`

操作2依赖1，但是操作3不依赖2，所以有可能出现1,3,2的顺序，当出现这种顺序的时候，虽然`instance`不为空，但是对象也有可能没有正确初始化，会出错。
<a name="VjXwA"></a>
## 3、总结
并发三特征可见性和有序性和原子性中，`volatile`通过新值立即同步到主内存和每次使用前从主内存刷新机制保证了可见性。通过禁止指令重排序保证了有序性。无法保证原子性。`synchronized`关键字通过`lock`和`unlock`操作保证了原子性，通过对一个变量`unlock`前，把变量同步回主内存中保证了可见性，通过一个变量在同一时刻只允许一条线程对其进行`lock`操作保证了有序性。<br />他的“万能”也间接导致了对`synchronized`关键字的滥用，越泛用的控制，对性能的影响也越大，虽然jvm不断的对`synchronized`关键字进行各种各样的优化，但是还是要在合适的时候想起`volatile`关键字。
