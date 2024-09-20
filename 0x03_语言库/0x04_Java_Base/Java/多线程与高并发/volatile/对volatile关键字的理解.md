Java
<a name="qETkc"></a>
## 一、简介
在多线程环境下，如果编程不当，可能会出现程序运行结果混乱的问题。<br />出现这个原因主要是，JMM 中主内存和线程工作内存的数据不一致，以及多个线程执行时无序，共同导致的结果。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1694742006149-1f9b939c-2728-4a90-8465-8a9844cd828b.jpeg)<br />同时也提到引入`synchronized`同步锁，可以保证线程同步，让多个线程依次排队执行被`synchronized`修饰的方法或者方法块，使程序的运行结果与预期一致。<br />不可否认，采用`synchronized`同步锁确实可以保证线程安全，但是它对服务性能的消耗也很大，`synchronized`是一个独占式的同步锁，比如当多个线程尝试获取锁时，其中一个线程获取到锁之后，未获取到锁的线程会不断的尝试获取锁，而不会发生中断，当冲突严重的时候，线程会直接进入阻塞状态，不能再干别的活。<br />为了实现线程之间更加方便的访问共享变量，Java 编程语言还提供了另一种同步机制：`volatile`域变量，在某些场景下使用它会更加方便。<br />一般来说，被`volatile`修饰的变量，可以保证所有线程看到这个变量都是同一个值，同时它不会引起线程上下文的切换和调度，相比`synchronized`，`volatile`更加的轻量化。<br />比较官方的解释，`volatile`修饰变量有以下几个作用：

1. 保证变量的可见性，不保证原子性 当用`volatile`修饰一个变量时，JMM 会把当前线程本地内存中的变量强制刷新到主内存中去，这个写操作也会导致其他线程中被`volatile`修饰的变量缓存无效，然后从主内存中获取最新的值
2. 禁止指令重排 正常情况下，编译器和处理器为了优化程序执行性能会对指令序列进行重排序，当然是在不影响程序结果的前提下。`volatile`能够在一定程度上禁止 JVM 进行指令重排。

从概念上感觉比较难理解，下面结合几个例子，一起来看看它的具体应用。
<a name="q3esD"></a>
## 二、volatile 使用详解
先看一个例子。
```java
public class DataEntity {

    private boolean isRunning = true;

    public void addCount(){
        System.out.println("线程运行开始....");
        while (isRunning){ }
        System.out.println("线程运行结束....");
    }

    public boolean isRunning() {
        return isRunning;
    }

    public void setRunning(boolean running) {
        isRunning = running;
    }
}
```
```java
public class MyThread extends Thread {

    private DataEntity entity;

    public MyThread(DataEntity entity) {
        this.entity = entity;
    }

    @Override
    public void run() {
        entity.addCount();
    }
}
```
```java
public class MyThreadTest {

    public static void main(String[] args) throws InterruptedException {
        // 初始化数据实体
        DataEntity entity = new DataEntity();

        MyThread threadA = new MyThread(entity);
        threadA.start();

        // 主线程阻塞1秒
        Thread.sleep(1000);

        // 将运行状态设置为false
        entity.setRunning(false);
    }
}
```
运行结果如下：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1695264639185-326e13df-7c9b-4fe5-81e3-39af6b8e0f3d.png#averageHue=%23fcfcfc&clientId=u02d2abaf-3e7d-4&from=paste&height=224&id=ubda738c1&originHeight=559&originWidth=1966&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=53387&status=done&style=none&taskId=uf5ccb561-5395-40c1-8c73-e105c6675ed&title=&width=786.4)<br />从实际运行结果来看，程序进入死循环状态，虽然最后一行手动设置了`entity.setRunning(false)`，但是没有起到任何的作用。<br />原因其实也很简单，虽然主线程main将`isRunning`变量设置为false，但是线程threadA 里面的`isRunning`变量还是true，两个线程看到的数据不一致。<br />假如在`isRunning`变量上，加一个volatile关键字，再来看看运行效果。
```java
/**
 * 在 isRunning 变量上加一个 volatile 关键字
 */
private volatile boolean isRunning = true;
```
运行结果如下：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1695264679126-48d107e3-624b-4a63-b05d-34a0808dac0c.png#averageHue=%23fcfbfb&clientId=u02d2abaf-3e7d-4&from=paste&height=208&id=u06f03bb3&originHeight=521&originWidth=2070&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=76004&status=done&style=none&taskId=ud856fc46-b552-45ff-ad12-be9024e3d8f&title=&width=828)<br />程序运行后自动结束。<br />说明当主线程main将`isRunning`变量设置为false时，线程threadA 里面的`isRunning`值也随着发生变化。<br />说明被`volatile`修饰的变量，在多线程环境下，可以保证所有线程看到这个变量都是同一个值。
<a name="wyTik"></a>
## 三、volatile 不适用的场景
对于某些场景下，`volatile`可能并不适用，还是先看一个例子。
```java
public class DataEntity {

    private volatile int count = 0;

    public void addCount(){
        for (int i = 0; i < 100000; i++) {
            count++;
        }
    }

    public int getCount() {
        return count;
    }
}
```
```java
public class MyThreadTest {

    public static void main(String[] args) throws InterruptedException {
        // 初始化数据实体
        DataEntity entity = new DataEntity();

        // 初始化5个线程计数器
        CountDownLatch latch = new CountDownLatch(5);

        // 采用多线程进行操作
        for (int i = 0; i < 5; i++) {
            new Thread(new Runnable() {
                @Override
                public void run() {
                    entity.addCount();
                    //线程运行完毕减1
                    latch.countDown();
                }
            }).start();
        }

        // 等待以上线程执行完毕，再获取结果
        latch.await();
        System.out.println("result: " + entity.getCount());
    }
}
```
运行结果如下：
```
第一次运行：result: 340464
第二次运行：result: 318342
第三次运行：result: 305957
```
理论上使用 5 个线程分别执行了100000自增，预期的结果应该是5*100000=500000，从实际的运行结果可以看出，与预期不一致。<br />**这是因为**`**volatile**`**的作用其实是有限的，它只能保证多个线程之间看到的共享变量值是最新的，但是无法保证多个线程操作共享变量时依次有序，无法保证原子性操作**。<br />上面的例子中count++不是一个原子性操作，在处理器看来，其实一共做了三个步骤的操作：**读取数据**、**对数据加 1**、**回写数据**，在多线程随机执行情况下，输出结果不能达到预期值。<br />如果想要实现与预期一致的结果，有以下三种方案可选。<br />方案一：采用`synchronized`同步锁
```java
public class DataEntityC2 {

    private int count = 0;

    /**
     * 采用 synchronized 同步锁，可以实现多个线程执行方法时串行
     */
    public synchronized void addCount(){
        for (int i = 0; i < 100000; i++) {
            count++;
        }
    }

    public int getCount() {
        return count;
    }
}
```
方案二：采用Lock锁
```java
public class DataEntityC2 {

    private int count = 0;

    private Lock lock = new ReentrantLock();

    /**
     * 采用 Lock 锁，可以实现多个线程执行方法时串行
     */
    public void addCount(){
        for (int i = 0; i < 100000; i++) {
            lock.lock();
            try {
                count++;
            } finally {
                lock.unlock();
            }
        }
    }

    public int getCount() {
        return count;
    }
}
```
方案三：采用JUC包中的原子操作类
```java
public class DataEntity {

    private AtomicInteger inc = new AtomicInteger();

    /**
     * 采用原子操作类，原子操作类是通过CAS循环的方式来保证操作原子性
     */
    public void addCount(){
        for (int i = 0; i < 100000; i++) {
            inc.getAndIncrement();
        }
    }

    public int getCount() {
        return inc.get();
    }
}
```
以上三种方案，都可以实现程序的运行结果与预期一致！
<a name="yYfoP"></a>
## 四、volatile 的原理
通过以上的例子介绍，相信大家对`volatile`关键字的作用有了一些认识。<br />`volatile`修饰的变量，可以保证变量在内存中的可见性，但是无法保证原子性操作。<br />关于原子性、可见性和有序性的定义，这三个特性主要从多线程编程安全角度总结出来的一些基本要素，也是并发编程的三大核心基础，在上篇文章中有所提到过，这里不再重复讲了。<br />在 JVM 底层，`volatile`**是通过采用“内存屏障”来实现内存可见性和禁止指令重排**。观察不加入`volatile`和加入`volatile`关键字所生成的汇编代码发现，加入`volatile`关键字的代码会多出一个lock前缀指令，lock前缀指令实际上相当于一个内存屏障，可以提供以下 3 个功能。

1. 它确保指令重排序时，不会把后面的指令排到内存屏障之前的位置，也不会把前面的指令排到内存屏障的后面，禁止处理器对影响程序执行结果的指令进行重排
2. 它会强制将缓存的修改操作立刻写入主存，保证内存变量可见
3. 如果是写操作，它会导致其它 CPU 中对应的行缓存无效，目的是让其他线程中被volatile修饰的变量缓存无效，然后从主内存中获取最新的值
<a name="idgR9"></a>
## 五、单例模式中的双重检锁为什么要加 `volatile`？
单例设计模式中的双重校验锁实现。
```java
public class Singleton {  

    private volatile static Singleton singleton;  

    private Singleton (){}  

    public static Singleton getSingleton() {  
        if (singleton == null) {  //第一行
            synchronized (Singleton.class) {  //第二行
                if (singleton == null) {  //第三行
                    singleton = new Singleton();  //第四行
                }  
            }  
        }  
        return singleton;  //第五行
    }  
}
```
`synchronized`可以保证原子性、可见性和有序性，为什么变量`singleton`还需要加`volatile`关键字呢？<br />之所以需要加`volatile`关键字的原因是：**问题出在第一行代码不在同步代码块之类，可能出现这个对象地址不为空，但是内容为空**。<br />以初始化一个`Singleton singleton = new Singleton();`为例，JVM 会分三个步骤完成：
```
a. memory = allocate() //分配内存
b. ctorInstanc(memory) //初始化对象
c. instance = memory   //设置instance指向刚分配的地址
```
上面的代码在编译运行时可能会出现重排序，因为b和c无逻辑关联，执行的顺序是a -> b -> c或者a -> c -> b，在多线程的环境下可能会出现问题。<br />分析过程如下：

1. 线程 A 执行到第四行代码时，线程 B 进来执行第一行代码
2. 假设线程 A 在执行过程中发生了指令重排序，先执行了a和c，没有执行b
3. 由于线程 A 执行了c导致instance指向了一段地址，此时线程 B 检查singleton发现不为null，会直接跳转到第五行代码，返回一个未初始化的对象，导致程序会出现报错
4. 因此需要在singleton变量上加一个volatile关键字，当线程 A 执行完毕b操作之后，会变量强制刷新到主内存中，此时线程 B 也可以拿到最新的对象

这就是为啥双重检锁模式中，singleton变量为啥要加一个volatile关键字的原因。<br />采用双重检锁的方式，可以显著的提升并发查询的效率。
