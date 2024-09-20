Java LockSupport
<a name="gr1lq"></a>
## 前言
熟悉 Java 并发包的人一定对 LockSupport 的 park/unpark 方法不会感到陌生，它是 Lock（AQS）的基石，给 Lock（AQS）提供了挂起/恢复当前线程的能力。<br />LockSupport 的 park/unpark 方法本质上是对 Unsafe 的 park/unpark 方法的简单封装，而后者是 `native` 方法，对 Java 程序来说是一个黑箱操作，那么要想了解它的底层实现，就必须深入 Java 虚拟机的源码。<br />以park的源码为例：
```java
public class LockSupport {

    public static void park(Object blocker) {
        //获取当前线程
        Thread t = Thread.currentThread();
        //记录当前线程阻塞的原因,底层就是unsafe.putObject,就是把对象存储起来
        setBlocker(t, blocker);
        //执行park
        unsafe.park(false, 0L);
        //线程恢复后，去掉阻塞原因
        setBlocker(t, null);
    }

    //无限阻塞线程，直到有其他线程调用unpark方法
    public static void park() {
        UNSAFE.park(false, 0L);
    }       
}
```
从源码可以看到真实的实现均在 unsafe。
<a name="uOKxR"></a>
## 一、LockSupport
LockSupport是JDK中比较底层的类，用来创建锁和其他同步工具类的基本线程阻塞原语。<br />Java锁和同步器框架的核心AQS:AbstractQueuedSynchronizer，就是通过调用`LockSupport.park()`和`LockSupport.unpark()`实现线程的阻塞和唤醒的。LockSupport很类似于二元信号量(只有1个许可证可供使用)，如果这个许可还没有被占用，当前线程获取许可并继续执行；如果许可已经被占用，当前线程阻塞，等待获取许可。<br />LockSupport中的`park()` 和 `unpark()` 的作用分别是阻塞线程和解除阻塞线程，而且`park()`和`unpark()`不会遇到“Thread.suspend 和 Thread.resume所可能引发的死锁”问题。因为`park()` 和 `unpark()`有许可的存在；调用 `park()` 的线程和另一个试图将其 `unpark()` 的线程之间的竞争将保持活性。
<a name="bKbUF"></a>
### 1.1、LockSupport函数列表
```java
public class LockSupport {

    // 返回提供给最近一次尚未解除阻塞的 park 方法调用的 blocker 对象，如果该调用不受阻塞，则返回 null。
    static Object getBlocker(Thread t);

    // 为了线程调度，禁用当前线程，除非许可可用。
    static void park();

    // 为了线程调度，在许可可用之前禁用当前线程。
    static void park(Object blocker);

    // 为了线程调度禁用当前线程，最多等待指定的等待时间，除非许可可用。
    static void parkNanos(long nanos);

    // 为了线程调度，在许可可用前禁用当前线程，并最多等待指定的等待时间。
    static void parkNanos(Object blocker, long nanos);

    // 为了线程调度，在指定的时限前禁用当前线程，除非许可可用。
    static void parkUntil(long deadline);

    // 为了线程调度，在指定的时限前禁用当前线程，除非许可可用。
    static void parkUntil(Object blocker, long deadline);

    // 如果给定线程的许可尚不可用，则使其可用。
    static void unpark(Thread thread);
}
```
说明：LockSupport是通过调用Unsafe函数中的接口实现阻塞和解除阻塞的。
<a name="zuVgb"></a>
### 1.2、基本使用
```java
// 暂停当前线程
LockSupport.park();

// 恢复某个线程的运行
LockSupport.unpark(暂停线程对象)
```
<a name="e0RyX"></a>
#### 先 park 再 unpark
```java
Thread t1 = new Thread(() -> {
    log.debug("start...");
    sleep(1);
    log.debug("park...");
    LockSupport.park();
    log.debug("resume...");
},"t1");
t1.start();
sleep(2);
log.debug("unpark...");
LockSupport.unpark(t1);
```
输出：
```java
18:42:52.585 c.TestParkUnpark [t1] - start...
18:42:53.589 c.TestParkUnpark [t1] - park...
18:42:54.583 c.TestParkUnpark [main] - unpark...
18:42:54.583 c.TestParkUnpark [t1] - resume...
```
<a name="rnscC"></a>
#### 先 unpark 再 park
```java
Thread t1 = new Thread(() -> {
    log.debug("start...");
    sleep(2);
    log.debug("park...");
    LockSupport.park();
    log.debug("resume...");
}, "t1");
t1.start();
sleep(1);
log.debug("unpark...");
LockSupport.unpark(t1);
```
输出：
```java
18:43:50.765 c.TestParkUnpark [t1] - start...
18:43:51.764 c.TestParkUnpark [main] - unpark...
18:43:52.769 c.TestParkUnpark [t1] - park...
18:43:52.769 c.TestParkUnpark [t1] - resume...
```
<a name="okMHA"></a>
### 1.3、特点
在调用对象的Wait之前当前线程必须先获得该对象的监视器(Synchronized)，被唤醒之后需要重新获取到监视器才能继续执行。而LockSupport并不需要获取对象的监视器。
<a name="Bs6Y4"></a>
###### 与 Object 的 wait & notify 相比

- 1、wait，notify 和 notifyAll 必须配合 Object Monitor 一起使用，而 park，unpark 不必。
- 2、park & unpark 是以线程为单位来【阻塞】和【唤醒】线程，而 notify 只能随机唤醒一个等待线程，notifyAll是唤醒所有等待线程，但不那么【精确】。
- 3、park & unpark 可以先 unpark，而 wait & notify 不能先 notify。

因为它们本身的实现机制不一样，所以它们之间没有交集，也就是说LockSupport阻塞的线程，notify/notifyAll没法唤醒。<br />虽然两者用法不同，但是有一点， LockSupport 的park和Object的wait一样也能响应中断。
```java
public class LockSupportTest {

    public static void main(String[] args) throws InterruptedException {
        Thread t = new Thread(() -> {
            LockSupport.park();
            System.out.println("thread:"+Thread.currentThread().getName()+"awake");
            },"t1");
        t.start();

        Thread.sleep(2000);
        //中断
        t.interrupt();
    }
}
```
<a name="eBoAo"></a>
## 二、LockSupport park & unpark原理
每个线程都会关联一个 Parker 对象，每个 Parker 对象都各自维护了三个角色：`_counter`(计数器)、 `_mutex`(互斥量)、`_cond`(条件变量)。
<a name="ly7Xx"></a>
#### 2.1、情况一，先调用park，再调用unpark
<a name="cvcy9"></a>
###### park 操作
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639190122096-3a042496-c0a7-4f77-b88b-97da38c1e1d0.webp#clientId=ufe533be2-8cfa-4&from=paste&id=ua640ae90&originHeight=371&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u66ab6d7e-583c-4203-b62d-7c566a6c386&title=)

1. 当前线程调用 `Unsafe.park()` 方法
2. 检查 `_counter` ，本情况为 0，这时，获得 `_mutex` 互斥锁
3. 线程进入 `_cond` 条件变量阻塞
4. 设置 `_counter = 0`
<a name="PlEvF"></a>
###### unpark 操作
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639190121775-ce33edf9-9497-484d-954e-bf49c3883307.webp#clientId=ufe533be2-8cfa-4&from=paste&id=uc22f7a7f&originHeight=396&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4cc1afeb-1d19-4273-8df8-e4a656e6d13&title=)

1. 调用 `Unsafe.unpark(Thread_0)` 方法，设置 `_counter` 为 1
2. 唤醒 `_cond` 条件变量中的 `Thread_0`
3. `Thread_0` 恢复运行
4. 设置 `_counter` 为 0
<a name="j1Kwf"></a>
#### 2.2、情况二，先调用unpark，再调用park
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639190122065-55f2d3ea-af4d-4962-aa62-b4da7c4b2a0c.webp#clientId=ufe533be2-8cfa-4&from=paste&id=u57c3c757&originHeight=334&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u47ab013e-203e-45e4-8c7d-63b69503a15&title=)

1. 调用 `Unsafe.unpark(Thread_0)` 方法，设置 `_counter` 为 1
2. 当前线程调用 `Unsafe.park()` 方法
3. 检查 `_counter` ，本情况为 1，这时线程无需阻塞，继续运行
4. 设置 `_counter` 为 0
<a name="WEMVT"></a>
## 三、LockSupport Java源码解析
<a name="SkISu"></a>
### 3.1 变量说明
```java
public class LockSupport {

    // Hotspot implementation via intrinsics API
    //unsafe常量，设置为使用Unsafe.compareAndSwapInt进行更新
    //UNSAFE字段表示sun.misc.Unsafe类，一般程序中不允许直接调用
    private static final sun.misc.Unsafe UNSAFE;

    //表示parkBlocker在内存地址的偏移量
    private static final long parkBlockerOffset;

    //表示threadLocalRandomSeed在内存地址的偏移量，此变量的作用暂时还不了解
    private static final long SEED;

    //表示threadLocalRandomProbe在内存地址的偏移量，此变量的作用暂时还不了解
    private static final long PROBE;

    //表示threadLocalRandomSecondarySeed在内存地址的偏移量
    // 作用是 可以通过nextSecondarySeed()方法来获取随机数
    private static final long SECONDARY;
}
```
<a name="qVRn2"></a>
#### 变量是如何获取其实例对象的？
```java
public class LockSupport {

    static {
        try {
            //实例化unsafe对象
            UNSAFE = sun.misc.Unsafe.getUnsafe();
            Class<?> tk = Thread.class;

            //利用unsafe对象来获取parkBlocker在内存地址的偏移量
            parkBlockerOffset = UNSAFE.objectFieldOffset(tk.getDeclaredField("parkBlocker"));

            //利用unsafe对象来获取threadLocalRandomSeed在内存地址的偏移量
            SEED = UNSAFE.objectFieldOffset(tk.getDeclaredField("threadLocalRandomSeed"));

            //利用unsafe对象来获取threadLocalRandomProbe在内存地址的偏移量  
            PROBE = UNSAFE.objectFieldOffset(tk.getDeclaredField("threadLocalRandomProbe"));

            //利用unsafe对象来获取threadLocalRandomSecondarySeed在内存地址的偏移量  
            SECONDARY = UNSAFE.objectFieldOffset(tk.getDeclaredField("threadLocalRandomSecondarySeed"));
        } catch (Exception ex) { throw new Error(ex); }
    }
}
```
由上面代码可知这些变量是通过`static`代码块在类加载的时候就通过`unsafe`对象获取其在内存地址的偏移量了。
<a name="q7a8V"></a>
### 3.2 构造方法
```java
public class LockSupport {

    //LockSupport只有一个私有构造函数，无法被实例化。
    private LockSupport() {} // Cannot be instantiated.
}
```
<a name="PdeyM"></a>
### 3.3 两个特殊的方法
```java
public class LockSupport {

    //设置线程t的parkBlocker字段的值为arg
    private static void setBlocker(Thread t, Object arg) {
        // Even though volatile, hotspot doesn't need a write barrier here.
        //尽管hotspot易变，但在这里并不需要写屏障。
        UNSAFE.putObject(t, parkBlockerOffset, arg);
    }

    //获取当前线程的Blocker值
    public static Object getBlocker(Thread t) {
        //若当前线程为空就抛出异常
        if (t == null)
            throw new NullPointerException();

        //利用unsafe对象获取当前线程的Blocker值 
        return UNSAFE.getObjectVolatile(t, parkBlockerOffset);
    }
}   
```
<a name="zZubA"></a>
### 3.4 常用方法
<a name="PHX1k"></a>
#### 1、`unpark(Thread thread)`方法
```java
public class LockSupport {

    //释放该线程的阻塞状态，即类似释放锁，只不过这里是将许可设置为1
    public static void unpark(Thread thread) {
        //判断线程是否为空
        if (thread != null)
            //释放该线程许可
            UNSAFE.unpark(thread);
    }
}  
```
<a name="hLxTU"></a>
#### 2、`park(Object blocker)`方法 和 `park()`方法
```java
public class LockSupport {

    //阻塞当前线程，并且将当前线程的parkBlocker字段设置为blocker
    public static void park(Object blocker) {
        //获取当前线程
        Thread t = Thread.currentThread();
        //将当前线程的parkBlocker字段设置为blocker
        setBlocker(t, blocker);
        //阻塞当前线程，第一个参数表示isAbsolute，是否为绝对时间，第二个参数就是代表时间
        UNSAFE.park(false, 0L);
        //重新可运行后再此设置Blocker
        setBlocker(t, null);
    }

    //无限阻塞线程，直到有其他线程调用unpark方法
    public static void park() {
        UNSAFE.park(false, 0L);
    }   
} 
```
 说明：

- 调用`park`函数时，首先获取当前线程，然后设置当前线程的`parkBlocker`字段，即调用`setBlocker`函数， 之后调用`Unsafe`类的`park`函数，之后再调用`setBlocker`函数。
<a name="VjosF"></a>
##### `park(Object blocker)`函数中要调用两次`setBlocker`函数

- 1、调用`park`函数时，当前线程首先设置好`parkBlocker`字段，然后再调用 `Unsafe`的park函数，此时，当前线程就已经阻塞了，等待该线程的`unpark`函数被调用，所以后面的一个 `setBlocker`函数无法运行，`unpark`函数被调用，该线程获得许可后，就可以继续运行了，也就运行第二个 `setBlocker`，把该线程的`parkBlocker`字段设置为null，这样就完成了整个`park`函数的逻辑。
- 2、如果没有第二个 `setBlocker`，那么之后没有调用`park(Object blocker)`，而直接调用`getBlocker`函数，得到的还是前一个 `park(Object blocker)`设置的`blocker`，显然是不符合逻辑的。总之，必须要保证在`park(Object blocker)`整个函数 执行完后，该线程的`parkBlocker`字段又恢复为null。

所以，`park(Object)`型函数里必须要调用`setBlocker`函数两次。
<a name="aJTmu"></a>
#### 3、`parkNanos(Object blocker, long nanos)`方法 和 `parkNanos(long nanos)`方法
```java
public class LockSupport {

    //阻塞当前线程nanos秒
    public static void parkNanos(Object blocker, long nanos) {
        //先判断nanos是否大于0，小于等于0都代表无限等待
        if (nanos > 0) {
            //获取当前线程
            Thread t = Thread.currentThread();
            //将当前线程的parkBlocker字段设置为blocker
            setBlocker(t, blocker);
            //阻塞当前线程现对时间的nanos秒
            UNSAFE.park(false, nanos);
            //将当前线程的parkBlocker字段设置为null
            setBlocker(t, null);
        }
    }   

    //阻塞当前线程nanos秒，现对时间
    public static void parkNanos(long nanos) {
        if (nanos > 0)
            UNSAFE.park(false, nanos);
    }   
}   
```
<a name="ukZ8X"></a>
#### 4、`parkUntil(Object blocker, long deadline)`方法 和 `parkUntil(long deadline)`方法
```java
public class LockSupport {

    //将当前线程阻塞绝对时间的deadline秒，并且将当前线程的parkBlockerOffset设置为blocker
    public static void parkUntil(Object blocker, long deadline) {
        //获取当前线程
        Thread t = Thread.currentThread();
        //设置当前线程parkBlocker字段设置为blocker
        setBlocker(t, blocker);
        //阻塞当前线程绝对时间的deadline秒
        UNSAFE.park(true, deadline);
        //当前线程parkBlocker字段设置为null
        setBlocker(t, null);
    }

    //将当前线程阻塞绝对时间的deadline秒
    public static void parkUntil(long deadline) {
        UNSAFE.park(true, deadline);
    }   
} 
```
<a name="e468e"></a>
## 总结：
LockSupport 和 CAS 是Java并发包中很多并发工具控制机制的基础，它们底层其实都是依赖Unsafe实现。很多锁的类都是基于LockSupport的park和unpark来实现的，所以了解LockSupport类是非常重要的。
