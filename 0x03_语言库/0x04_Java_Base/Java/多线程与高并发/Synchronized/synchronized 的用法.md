<a name="AXYLa"></a>
## 一、简介
实际上，在多线程环境中，难免会出现多个线程对一个对象的实例变量进行同时访问和操作，如果编程处理不当，会产生**脏读**现象。
<a name="QGmNW"></a>
## 二、线程安全问题介绍
先来看一个简单的线程安全问题的例子！
```java
public class DataEntity {

    private int count = 0;

    public void addCount(){
        count++;
    }

    public int getCount(){
        return count;
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
        for (int j = 0; j < 1000000; j++) {
            entity.addCount();
        }
    }
}
```
```java
public class MyThreadTest {

    public static void main(String[] args) {
        // 初始化数据实体
        DataEntity entity = new DataEntity();
        //使用多线程编程对数据进行计算
        for (int i = 0; i < 10; i++) {
            MyThread thread = new MyThread(entity);
            thread.start();
        }

        try {
            Thread.sleep(500);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("result: " + entity.getCount());
    }
}
```
多次运行结果如下：
```
第一次运行：result: 9788554
第二次运行：result: 9861461
第三次运行：result: 6412249
...
```
上面的代码中，总共开启了 10 个线程，每个线程都累加了 1000000 次，如果结果正确的话，自然而然总数就应该是 10 * 1000000 = 10000000。<br />**但是多次运行结果都不是这个数，而且每次运行结果都不一样，为什么会出现这个结果呢**？<br />简单的说，**这是主内存和线程的工作内存数据不一致，以及多线程执行时无序，共同造成的结果**！<br />先简单的了解一下 Java 的内存模型！<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1694742006149-1f9b939c-2728-4a90-8465-8a9844cd828b.jpeg)<br />如上图所示，线程 A 和线程 B 之间，如果要完成数据通信的话，需要经历以下几个步骤：

1. 线程 A 从主内存中将共享变量读入线程 A 的工作内存后并进行操作，之后将数据重新写回到主内存中；
2. 线程 B 从主存中读取最新的共享变量，然后存入自己的工作内存中，再进行操作，数据操作完之后再重新写入到主内存中；

如果线程 A 更新后数据并没有及时写回到主存，而此时线程 B 从主内存中读到的数据，可能就是过期的数据，于是就会出现“**脏读**”现象。<br />因此在多线程环境下，如果不进行一定干预处理，可能就会出现像上文介绍的那样，采用多线程编程时，程序的实际运行结果与预期会不一致，就会产生非常严重的问题。<br />**针对多线程编程中，程序运行不安全的问题，Java 提供了**`**synchronized**`**关键字来解决这个问题，当多个线程同时访问共享资源时，会保证线程依次排队操作共享变量，从而保证程序的实际运行结果与预期一致。**<br />对上面示例中的`DataEntity.addCount()`方法进行改造，再看看效果如下。
```java
public class DataEntity {

    private int count = 0;

    /**
     * 在方法上加上 synchronized 关键字
     */
    public synchronized void addCount(){
        count++;
    }

    public int getCount(){
        return count;
    }
}
```
多次运行结果如下：
```
第一次运行：result: 10000000
第二次运行：result: 10000000
第三次运行：result: 10000000
...
```
运行结果与预期一致！
<a name="o018V"></a>
## 三、`synchronized` 使用详解
`synchronized`作为 Java 中的关键字，在多线程编程中，有着非常重要的地位，也是新手了解并发编程的基础，从功能角度看，它有以下几个比较重要的特性：

- **原子性**：即一个或多个操作要么全部执行成功，要么全部执行失败。synchronized关键字可以保证只有一个线程拿到锁，访问共享资源
- **可见性**：即一个线程对共享变量进行修改后，其他线程可以立刻看到。执行`synchronized`时，线程获取锁之后，一定从主内存中读取数据，释放锁之前，一定会将数据写回主内存，从而保证内存数据可见性
- **有序性**：即保证程序的执行顺序会按照代码的先后顺序执行。`synchronized`关键字，可以保证每个线程依次排队操作共享变量

`synchronized`也被称为同步锁，它可以把任意一个非 NULL 的对象当成锁，只有拿到锁的线程能进入方法体，并且只有一个线程能进入，其他的线程必须等待锁释放了才能进入，它属于独占式的悲观锁，同时也属于可重入锁。<br />从实际的使用角度来看，`synchronized`修饰的对象有以下几种：

- **修饰一个方法**：被修饰的方法称为同步方法，其作用的范围是整个方法，作用的对象是调用这个方法的对象
- **修饰一个静态的方法**：其作用的范围是整个静态方法，作用的对象是这个类的所有对象
- **修饰一个代码块**：被修饰的代码块称为同步语句块，其作用的范围是大括号{}括起来的代码，作用的对象是调用这个代码块的对象，使用上比较灵活

下面一起来看看它们的具体用法。
<a name="EqmgM"></a>
### 3.1、修饰一个方法
当`synchronized`修饰一个方法时，多个线程访问同一个对象，哪个线程持有该方法所属对象的锁，就拥有执行权限，否则就只能等待。<br />**如果多线程访问的不是同一个对象，不会起到保证线程同步的作用**。<br />示例如下：
```java
public class DataEntity {

    private int count;

    /**
     * 在方法上加上 synchronized 关键字
     */
    public synchronized void addCount(){
        for (int i = 0; i < 3; i++) {
            try {
                System.out.println(Thread.currentThread().getName() + ":" + (count++));
                Thread.sleep(100);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public int getCount() {
        return count;
    }
}
```
```java
public class MyThreadA extends Thread {

    private DataEntity entity;

    public MyThreadA(DataEntity entity) {
        this.entity = entity;
    }

    @Override
    public void run() {
        entity.addCount();
    }
}
```
```java
public class MyThreadB extends Thread {

    private DataEntity entity;

    public MyThreadB(DataEntity entity) {
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

    public static void main(String[] args) {
        // 初始化数据实体
        DataEntity entity = new DataEntity();

        MyThreadA threadA = new MyThreadA(entity);
        threadA.start();

        MyThreadB threadB = new MyThreadB(entity);
        threadB.start();


        try {
            Thread.sleep(1000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("result: " + entity.getCount());
    }
}
```
运行结果如下：
```
Thread-0:0
Thread-0:1
Thread-0:2
Thread-1:3
Thread-1:4
Thread-1:5
result: 6
```
当两个线程共同操作一个对象时，此时每个线程都会依次排队执行。<br />假如两个线程操作的不是一个对象，此时没有任何效果，示例如下：
```java
public class MyThreadTest {

    public static void main(String[] args) {
        DataEntity entity1 = new DataEntity();
        MyThreadA threadA = new MyThreadA(entity1);
        threadA.start();

        DataEntity entity2 = new DataEntity();
        MyThreadA threadB = new MyThreadA(entity2);
        threadB.start();


        try {
            Thread.sleep(1000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("result: " + entity1.getCount());
        System.out.println("result: " + entity2.getCount());
    }
}
```
运行结果如下：
```
Thread-0:0
Thread-1:0
Thread-0:1
Thread-1:1
Thread-0:2
Thread-1:2
result: 3
result: 3
```
从结果上可以看出，当`synchronized`修饰一个方法，当多个线程访问同一个对象的方法，每个线程会依次排队；如果访问的不是一个对象，线程不会进行排队，像正常执行一样。
<a name="Lbse5"></a>
### 3.2、修饰一个静态的方法
`synchronized`修改一个静态的方法时，代表的是对当前.java文件对应的 Class 类加锁，不区分对象实例。<br />示例如下：
```java
public class DataEntity {

    private static int count;

    /**
     * 在静态方法上加上 synchronized 关键字
     */
    public synchronized static void addCount(){
        for (int i = 0; i < 3; i++) {
            try {
                System.out.println(Thread.currentThread().getName() + ":" + (count++));
                Thread.sleep(100);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public static int getCount() {
        return count;
    }
}
```
```java
public class MyThreadA extends Thread {

    @Override
    public void run() {
        DataEntity.addCount();
    }
}
```
```java
public class MyThreadB extends Thread {

    @Override
    public void run() {
        DataEntity.addCount();
    }
}
```
```java
public class MyThreadTest {

    public static void main(String[] args) {

        MyThreadA threadA = new MyThreadA();
        threadA.start();

        MyThreadB threadB = new MyThreadB();
        threadB.start();


        try {
            Thread.sleep(1000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("result: " + DataEntity.getCount());
    }
}
```
运行结果如下：
```
Thread-0:0
Thread-0:1
Thread-0:2
Thread-1:3
Thread-1:4
Thread-1:5
result: 6
```
静态同步方法和非静态同步方法持有的是不同的锁，前者是类锁，后者是对象锁，类锁可以理解为这个类的所有对象。
<a name="zJ331"></a>
### 3.3、修饰一个代码块
`synchronized`用于修饰一个代码块时，只会控制代码块内的执行顺序，其他试图访问该对象的线程将被阻塞，编程比较灵活，在实际开发中用的应用比较广泛。<br />示例如下
```java
public class DataEntity {

    private int count;

    /**
     * 在方法上加上 synchronized 关键字
     */
    public void addCount(){
        synchronized (this){
            for (int i = 0; i < 3; i++) {
                try {
                    System.out.println(Thread.currentThread().getName() + ":" + (count++));
                    Thread.sleep(100);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    public int getCount() {
        return count;
    }
}
```
```java
public class MyThreadTest {

    public static void main(String[] args) {
        // 初始化数据实体
        DataEntity entity = new DataEntity();

        MyThreadA threadA = new MyThreadA(entity);
        threadA.start();

        MyThreadB threadB = new MyThreadB(entity);
        threadB.start();


        try {
            Thread.sleep(1000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("result: " + entity.getCount());
    }
}
```
运行结果如下：
```
Thread-0:0
Thread-0:1
Thread-0:2
Thread-1:3
Thread-1:4
Thread-1:5
result: 6
```
其中`synchronized (this)`中的this，表示的是当前类实例的对象，效果等同于`public synchronized void addCount()`。<br />除此之外，`synchronized()`还可以修饰任意实例对象，作用的范围就是具体的实例对象。<br />比如，修饰个自定义的类实例对象，作用的范围是拥有`lock`对象，其实也等价于`synchronized (this)`。
```java
public class DataEntity {

    private Object lock = new Object();

    /**
     * synchronized 可以修饰任意实例对象
     */
    public void addCount(){
        synchronized (lock){
            // todo...
        }
    }
}
```
当然也可以用于修饰类，表示类锁，效果等同于`public synchronized static void addCount()`。
```java
public class DataEntity {

    /**
     * synchronized 可以修饰类，表示类锁
     */
    public void addCount(){
        synchronized (DataEntity.class){
            // todo...
        }
    }
}
```
`synchronized`修饰代码块，比较经典的应用案例，就是单例设计模式中的双重校验锁实现。
```java
public class Singleton {  

    private volatile static Singleton singleton;  

    private Singleton (){}  

    public static Singleton getSingleton() {  
        if (singleton == null) {  
            synchronized (Singleton.class) {  
                if (singleton == null) {  
                    singleton = new Singleton();  
                }  
            }  
        }  
        return singleton;  
    }  
}
```
采用代码块的实现方式，编程会更加灵活，可以显著的提升并发查询的效率。
<a name="q0tiv"></a>
## 四、`synchronized` 锁重入介绍
`synchronized`关键字拥有锁重入的功能，所谓锁重入的意思就是：当一个线程得到一个对象锁后，再次请求此对象锁时可以再次得到该对象的锁，而无需等待。<br />看个例子就能明白。
```java
public class DataEntity {

    private int count = 0;


    public synchronized void addCount1(){
        System.out.println(Thread.currentThread().getName() + ":" + (count++));
        addCount2();
    }

    public synchronized void addCount2(){
        System.out.println(Thread.currentThread().getName() + ":" + (count++));
        addCount3();
    }

    public synchronized void addCount3(){
        System.out.println(Thread.currentThread().getName() + ":" + (count++));

    }

    public int getCount() {
        return count;
    }
}
```
```java
public class MyThreadA extends Thread {

    private DataEntity entity;

    public MyThreadA(DataEntity entity) {
        this.entity = entity;
    }

    @Override
    public void run() {
        entity.addCount1();
    }
}
```
```java
public class MyThreadB extends Thread {

    private DataEntity entity;

    public MyThreadB(DataEntity entity) {
        this.entity = entity;
    }

    @Override
    public void run() {
        entity.addCount1();
    }
}
```
```java
public class MyThreadTest {

    public static void main(String[] args) {
        // 初始化数据实体
        DataEntity entity = new DataEntity();

        MyThreadA threadA = new MyThreadA(entity);
        threadA.start();

        MyThreadB threadB = new MyThreadB(entity);
        threadB.start();


        try {
            Thread.sleep(1000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("result: " + entity.getCount());
    }
}
```
运行结果如下：
```
Thread-0:0
Thread-0:1
Thread-0:2
Thread-1:3
Thread-1:4
Thread-1:5
result: 6
```
从结果上看线程没有交替执行，线程Thread-0获取到锁之后，再次调用其它带有`synchronized`关键字的方法时，可以快速进入，而Thread-1线程需等待对象锁完全释放之后再获取，这就是锁重入。
<a name="Xw4Zr"></a>
## 五、小结
在多线程环境下，恰当的使用`synchronized`关键字可以保证线程同步，使程序的运行结果与预期一致。

1. 当`synchronized`修饰一个方法时，作用的范围是整个方法，作用的对象是调用这个方法的对象；
2. 当`synchronized`修饰一个静态方法时，作用的范围是整个静态方法，作用的对象是这个类的所有对象；
3. 当`synchronized`修饰一个代码块时，作用的范围是代码块，作用的对象是修饰的内容，如果是类，则这个类的所有对象都会受到控制；如果是任意对象实例子，则控制的是具体的对象实例，谁拥有这个对象锁，就能进入方法体

`synchronized`是一种同步锁，属于独占式，使用它进行线程同步，JVM 性能开销很大，大量的使用未必会带来好处。
