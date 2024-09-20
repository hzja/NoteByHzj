Java
<a name="xiX73"></a>
## 主要内容

1. 基本介绍
2. 通过反射获取`Unsafe`实例
3. `Unsafe`中的CAS操作
4. `Unsafe`中原子操作相关方法介绍
5. `Unsafe`中线程调度相关方法
6. `park`和`unpark`示例
7. `Unsafe`锁示例
8. `Unsafe`中保证变量的可见性
9. `Unsafe`中Class相关方法
10. 示例：`staticFieldOffset`、`staticFieldBase`、`staticFieldBase`
11. 示例：`shouldBeInitialized`、`ensureClassInitialized`
12. 对象操作的其他方法
13. 绕过构造方法创建对象
14. 数组相关的一些方法
15. 内存屏障相关操作
<a name="ZPNHr"></a>
## 基本介绍
Java高并发中主要涉及到类位于java.util.concurrent包中，简称juc，juc中大部分类都是依赖于Unsafe来实现的，主要用到了Unsafe中的CAS、线程挂起、线程恢复等相关功能。所以如果打算深入了解JUC原理的，必须先了解一下Unsafe类。<br />先上一幅`Unsafe`类的功能图：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1691855507661-acd34bc9-8f34-4f2d-bcea-b2e0faf99c69.jpeg)<br />Unsafe是位于sun.misc包下的一个类，主要提供一些用于执行低级别、不安全操作的方法，如直接访问系统内存资源、自主管理内存资源等，这些方法在提升Java运行效率、增强Java语言底层资源操作能力方面起到了很大的作用。但由于Unsafe类使Java语言拥有了类似C语言指针一样操作内存空间的能力，这无疑也增加了程序发生相关指针问题的风险。在程序中过度、不正确使用Unsafe类会使得程序出错的概率变大，使得Java这种安全的语言变得不再“安全”，因此对Unsafe的使用一定要慎重。<br />从Unsafe功能图上看出，Unsafe提供的API大致可分为**内存操作**、**CAS**、**Class相关**、**对象操作**、**线程调度**、**系统信息获取**、**内存屏障**、**数组操作**等几类，**本文主要介绍3个常用的操作：CAS、线程调度、对象操作。**<br />看一下UnSafe的原码部分：
```java
public final class Unsafe {
    // 单例对象
    private static final Unsafe theUnsafe;

    private Unsafe() {
    }
    @CallerSensitive
    public static Unsafe getUnsafe() {
        Class var0 = Reflection.getCallerClass();
        // 仅在引导类加载器`BootstrapClassLoader`加载时才合法
        if(!VM.isSystemDomainLoader(var0.getClassLoader())) {    
            throw new SecurityException("Unsafe");
        } else {
            return theUnsafe;
        }
    }
}
```
从代码中可以看出，`Unsafe`类为单例实现，提供静态方法`getUnsafe`获取`Unsafe`实例，内部会判断当前调用者是否是由系统类加载器加载的，如果不是系统类加载器加载的，会抛出`SecurityException`异常。<br />那想使用这个类，如何获取呢？<br />可以把类放在jdk的lib目录下，那么启动的时候会自动加载，这种方式不是很好。<br />通过反射可以获取到`Unsafe`中的`theUnsafe`字段的值，这样可以获取到`Unsafe`对象的实例。
<a name="x6DuQ"></a>
## 通过反射获取`Unsafe`实例
代码如下：
```java
import sun.misc.Unsafe;

import java.lang.reflect.Field;

public class Demo1 {
    static Unsafe unsafe;

    static {
        try {
            Field field = Unsafe.class.getDeclaredField("theUnsafe");
            field.setAccessible(true);
            unsafe = (Unsafe) field.get(null);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        System.out.println(unsafe);
    }
}
```
输出：
```
sun.misc.Unsafe@76ed5528
```
<a name="z3H0T"></a>
## Unsafe中的CAS操作
看一下Unsafe中CAS相关方法定义：
```java
/**
 * CAS 操作
 *
 * @param o        包含要修改field的对象
 * @param offset   对象中某field的偏移量
 * @param expected 期望值
 * @param update   更新值
 * @return true | false
 */
public final native boolean compareAndSwapObject(Object o, long offset, Object expected, Object update);

public final native boolean compareAndSwapInt(Object o, long offset, int expected,int update);

public final native boolean compareAndSwapLong(Object o, long offset, long expected, long update);
```
什么是CAS? 即比较并替换，实现并发算法时常用到的一种技术。CAS操作包含三个操作数——**内存位置、预期原值及新值**。**执行CAS操作的时候，将内存位置的值与预期原值比较，如果相匹配，那么处理器会自动将该位置值更新为新值，否则，处理器不做任何操作，多个线程同时执行cas操作，只有一个会成功**。都知道，CAS是一条CPU的原子指令（cmpxchg指令），不会造成所谓的数据不一致问题，Unsafe提供的CAS方法（如`compareAndSwapXXX`）底层实现即为CPU指令cmpxchg。执行cmpxchg指令的时候，会判断当前系统是否为多核系统，如果是就给总线加锁，只有一个线程会对总线加锁成功，加锁成功之后会执行cas操作，也就是说CAS的原子性实际上是CPU实现的， 其实在这一点上还是有排他锁的，只是比起用`synchronized`， 这里的排他时间要短的多， 所以在多线程情况下性能会比较好。<br />说一下offset，offeset为字段的偏移量，每个对象有个地址，offset是字段相对于对象地址的偏移量，对象地址记为baseAddress，字段偏移量记为offeset，那么字段对应的实际地址就是baseAddress+offeset，所以cas通过对象、偏移量就可以去操作字段对应的值了。<br />CAS在java.util.concurrent.atomic相关类、Java AQS、JUC中并发集合等实现上有非常广泛的应用，看一下`java.util.concurrent.atomic.AtomicInteger`类，这个类可以在多线程环境中对int类型的数据执行高效的原子修改操作，并保证数据的正确性，看一下此类中用到Unsafe cas的地方：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1691855613676-e7f7f880-bc5f-4808-9283-499df9f9dd40.png#averageHue=%23faf9f9&clientId=ubb545977-40f8-4&from=paste&height=493&id=u23276cf9&originHeight=1232&originWidth=2722&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=334448&status=done&style=none&taskId=ucdc4b21e-f757-4bde-b4d7-4b192d5dd50&title=&width=1088.8)<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1691855768284-e43fe5b2-648f-4656-82f1-5a6c13e7afb0.png#averageHue=%23fafaf9&clientId=ubb545977-40f8-4&from=paste&height=503&id=u5c57e8d9&originHeight=1258&originWidth=2548&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=338037&status=done&style=none&taskId=ua42dc6e7-ca73-4e24-8d9d-b995230171e&title=&width=1019.2)<br />JUC中其他地方使用到CAS的地方就不列举了，有兴趣的可以去看一下源码。
<a name="gIAjG"></a>
## Unsafe中原子操作相关方法介绍
5个方法，看一下实现：
```java
/**
 * int类型值原子操作，对var2地址对应的值做原子增加操作(增加var4)
 *
 * @param var1 操作的对象
 * @param var2 var2字段内存地址偏移量
 * @param var4 需要加的值
 * @return
 */
public final int getAndAddInt(Object var1, long var2, int var4) {
    int var5;
    do {
        var5 = this.getIntVolatile(var1, var2);
    } while (!this.compareAndSwapInt(var1, var2, var5, var5 + var4));

    return var5;
}

/**
 * long类型值原子操作，对var2地址对应的值做原子增加操作(增加var4)
 *
 * @param var1 操作的对象
 * @param var2 var2字段内存地址偏移量
 * @param var4 需要加的值
 * @return 返回旧值
 */
public final long getAndAddLong(Object var1, long var2, long var4) {
    long var6;
    do {
        var6 = this.getLongVolatile(var1, var2);
    } while (!this.compareAndSwapLong(var1, var2, var6, var6 + var4));

    return var6;
}

/**
 * int类型值原子操作方法，将var2地址对应的值置为var4
 *
 * @param var1 操作的对象
 * @param var2 var2字段内存地址偏移量
 * @param var4 新值
 * @return 返回旧值
 */
public final int getAndSetInt(Object var1, long var2, int var4) {
    int var5;
    do {
        var5 = this.getIntVolatile(var1, var2);
    } while (!this.compareAndSwapInt(var1, var2, var5, var4));

    return var5;
}

/**
 * long类型值原子操作方法，将var2地址对应的值置为var4
 *
 * @param var1 操作的对象
 * @param var2 var2字段内存地址偏移量
 * @param var4 新值
 * @return 返回旧值
 */
public final long getAndSetLong(Object var1, long var2, long var4) {
    long var6;
    do {
        var6 = this.getLongVolatile(var1, var2);
    } while (!this.compareAndSwapLong(var1, var2, var6, var4));

    return var6;
}

/**
 * Object类型值原子操作方法，将var2地址对应的值置为var4
 *
 * @param var1 操作的对象
 * @param var2 var2字段内存地址偏移量
 * @param var4 新值
 * @return 返回旧值
 */
public final Object getAndSetObject(Object var1, long var2, Object var4) {
    Object var5;
    do {
        var5 = this.getObjectVolatile(var1, var2);
    } while (!this.compareAndSwapObject(var1, var2, var5, var4));

    return var5;
}
```
看一下上面的方法，内部通过自旋的CAS操作实现的，这些方法都可以保证操作的数据在多线程环境中的原子性，正确性。<br />来个示例，还是来实现一个网站计数功能，同时有100个人发起对网站的请求，每个人发起10次请求，每次请求算一次，最终结果是1000次，代码如下：
```java
import sun.misc.Unsafe;

import java.lang.reflect.Field;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

public class Demo2 {
    static Unsafe unsafe;
    //用来记录网站访问量，每次访问+1
    static int count;
    //count在Demo.class对象中的地址偏移量
    static long countOffset;

    static {
        try {
            //获取Unsafe对象
            Field field = Unsafe.class.getDeclaredField("theUnsafe");
            field.setAccessible(true);
            unsafe = (Unsafe) field.get(null);

            Field countField = Demo2.class.getDeclaredField("count");
            //获取count字段在Demo2中的内存地址的偏移量
            countOffset = unsafe.staticFieldOffset(countField);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    //模拟访问一次
    public static void request() throws InterruptedException {
        //模拟耗时5毫秒
        TimeUnit.MILLISECONDS.sleep(5);
        //对count原子加1
        unsafe.getAndAddInt(Demo2.class, countOffset, 1);
    }

    public static void main(String[] args) throws InterruptedException {
        long starTime = System.currentTimeMillis();
        int threadSize = 100;
        CountDownLatch countDownLatch = new CountDownLatch(threadSize);
        for (int i = 0; i < threadSize; i++) {
            Thread thread = new Thread(() -> {
                try {
                    for (int j = 0; j < 10; j++) {
                        request();
                    }
                } catch (InterruptedException e) {
                    e.printStackTrace();
                } finally {
                    countDownLatch.countDown();
                }
            });
            thread.start();
        }

        countDownLatch.await();
        long endTime = System.currentTimeMillis();
        System.out.println(Thread.currentThread().getName() + "，耗时：" + (endTime - starTime) + ",count=" + count);
    }
}
```
输出：
```
main，耗时：114,count=1000
```
代码中在静态块中通过反射获取到了Unsafe类的实例，然后获取Demo2中count字段内存地址偏移量countOffset，main方法中模拟了100个人，每人发起10次请求，等到所有请求完毕之后，输出count的结果。<br />代码中用到了`CountDownLatch`，通过`countDownLatch.await()`让主线程等待，等待100个子线程都执行完毕之后，主线程在进行运行。
<a name="jJ6vn"></a>
## Unsafe中线程调度相关方法
这部分，包括线程挂起、恢复、锁机制等方法。
```java
//取消阻塞线程
public native void unpark(Object thread);
//阻塞线程,isAbsolute：是否是绝对时间，如果为true，time是一个绝对时间，如果为false，time是一个相对时间，time表示纳秒
public native void park(boolean isAbsolute, long time);
//获得对象锁（可重入锁）
@Deprecated
public native void monitorEnter(Object o);
//释放对象锁
@Deprecated
public native void monitorExit(Object o);
//尝试获取对象锁
@Deprecated
public native boolean tryMonitorEnter(Object o);
```
调用park后，线程将被阻塞，直到unpark调用或者超时，如果之前调用过unpark,不会进行阻塞，即`park`和`unpark`不区分先后顺序。`**monitorEnter**`**、**`**monitorExit**`**、**`**tryMonitorEnter**` 3个方法**已过期**，不建议使用了。
<a name="Ugfm3"></a>
## `park`和`unpark`示例
代码如下：
```java
import sun.misc.Unsafe;

import java.lang.reflect.Field;
import java.util.concurrent.TimeUnit;

public class Demo3 {
    static Unsafe unsafe;

    static {
        try {
            Field field = Unsafe.class.getDeclaredField("theUnsafe");
            field.setAccessible(true);
            unsafe = (Unsafe) field.get(null);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * 调用park和unpark，模拟线程的挂起和唤醒
     *
     * @throws InterruptedException
     */
    public static void m1() throws InterruptedException {
        Thread thread = new Thread(() -> {
            System.out.println(System.currentTimeMillis() + "," + Thread.currentThread().getName() + ",start");
            unsafe.park(false, 0);
            System.out.println(System.currentTimeMillis() + "," + Thread.currentThread().getName() + ",end");
        });
        thread.setName("thread1");
        thread.start();

        TimeUnit.SECONDS.sleep(5);
        unsafe.unpark(thread);
    }

    /**
     * 阻塞指定的时间
     */
    public static void m2() {
        Thread thread = new Thread(() -> {
            System.out.println(System.currentTimeMillis() + "," + Thread.currentThread().getName() + ",start");
            //线程挂起3秒
            unsafe.park(false, TimeUnit.SECONDS.toNanos(3));
            System.out.println(System.currentTimeMillis() + "," + Thread.currentThread().getName() + ",end");
        });
        thread.setName("thread2");
        thread.start();
    }

    public static void main(String[] args) throws InterruptedException {
        m1();
        m2();
    }
}
```
输出：
```
1565000238474,thread1,start
1565000243475,thread1,end
1565000243475,thread2,start
1565000246476,thread2,end
```
m1()中thread1调用`park`方法，`park`方法会将**当前线程阻塞**，被阻塞了5秒之后，被主线程调用`unpark`方法给唤醒了，`unpark`方法参数表示需要唤醒的线程。<br />线程中相当于有个许可，许可默认是0，调用`park`的时候，发现是0会阻塞当前线程，调用`unpark`之后，许可会被置为1，并会唤醒当前线程。如果在park之前先调用了`unpark`方法，执行`park`方法的时候，不会阻塞。`park`方法被唤醒之后，许可又会被置为0。多次调用`unpark`的效果是一样的，许可还是1。<br />juc中的`LockSupport`类是通过`unpark`和`park`方法实现的。
<a name="pNbsf"></a>
## `Unsafe`锁示例
代码如下：
```java
import sun.misc.Unsafe;

import java.lang.reflect.Field;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

public class Demo4 {

    static Unsafe unsafe;
    //用来记录网站访问量，每次访问+1
    static int count;

    static {
        try {
            Field field = Unsafe.class.getDeclaredField("theUnsafe");
            field.setAccessible(true);
            unsafe = (Unsafe) field.get(null);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    //模拟访问一次
    public static void request() {
        unsafe.monitorEnter(Demo4.class);
        try {
            count++;
        } finally {
            unsafe.monitorExit(Demo4.class);
        }
    }


    public static void main(String[] args) throws InterruptedException {
        long starTime = System.currentTimeMillis();
        int threadSize = 100;
        CountDownLatch countDownLatch = new CountDownLatch(threadSize);
        for (int i = 0; i < threadSize; i++) {
            Thread thread = new Thread(() -> {
                try {
                    for (int j = 0; j < 10; j++) {
                        request();
                    }
                } finally {
                    countDownLatch.countDown();
                }
            });
            thread.start();
        }

        countDownLatch.await();
        long endTime = System.currentTimeMillis();
        System.out.println(Thread.currentThread().getName() + "，耗时：" + (endTime - starTime) + ",count=" + count);
    }
}
```
输出：
```
main，耗时：64,count=1000
```
`monitorEnter`、`monitorExit`都有1个参数，表示上锁的对象。用法和`synchronized`关键字语义类似。<br />注意：

1. `**monitorEnter**`**、**`**monitorExit**`**、**`**tryMonitorEnter**`** 3个方法已过期，不建议使用了**
2. `**monitorEnter**`**、**`**monitorExit**`**必须成对出现，出现的次数必须一致，也就是说锁了n次，也必须释放n次，否则会造成死锁**
<a name="X2TiL"></a>
## Unsafe中保证变量的可见性
Java中操作内存分为主内存和工作内存，共享数据在主内存中，线程如果需要操作主内存的数据，需要先将主内存的数据复制到线程独有的工作内存中，操作完成之后再将其刷新到主内存中。如线程A要想看到线程B修改后的数据，需要满足：线程B修改数据之后，需要将数据从自己的工作内存中刷新到主内存中，并且A需要去主内存中读取数据。<br />被关键字`volatile`修饰的数据，有2点语义：

1. 如果一个变量被volatile修饰，读取这个变量时候，会强制从主内存中读取，然后将其复制到当前线程的工作内存中使用
2. 给`volatile`修饰的变量赋值的时候，会强制将赋值的结果从工作内存刷新到主内存

上面2点语义保证了被`volatile`修饰的数据在多线程中的可见性。<br />Unsafe中提供了和`volatile`语义一样的功能的方法，如下：
```java
//设置给定对象的int值，使用volatile语义，即设置后立马更新到内存对其他线程可见
public native void  putIntVolatile(Object o, long offset, int x);
//获得给定对象的指定偏移量offset的int值，使用volatile语义，总能获取到最新的int值。
public native int getIntVolatile(Object o, long offset);
```
`putIntVolatile`方法，2个参数：<br />o：表示需要操作的对象<br />offset：表示操作对象中的某个字段地址偏移量<br />x：将offset对应的字段的值修改为x，并且立即刷新到主存中<br />调用这个方法，会强制将工作内存中修改的数据刷新到主内存中。<br />`getIntVolatile`方法，2个参数<br />o：表示需要操作的对象<br />offset：表示操作对象中的某个字段地址偏移量<br />每次调用这个方法都会强制从主内存读取值，将其复制到工作内存中使用。<br />其他的还有几个`putXXXVolatile`、`getXXXVolatile`方法和上面2个类似。<br />本文主要讲解这些内容，希望您能有所收获，谢谢。
<a name="d8cwb"></a>
## Unsafe中Class相关方法
此部分主要提供Class和它的静态字段的操作相关方法，包含静态字段内存定位、定义类、定义匿名类、检验&确保初始化等。
```java
//获取给定静态字段的内存地址偏移量，这个值对于给定的字段是唯一且固定不变的
public native long staticFieldOffset(Field f);
//获取一个静态类中给定字段的对象指针
public native Object staticFieldBase(Field f);
//判断是否需要初始化一个类，通常在获取一个类的静态属性的时候（因为一个类如果没初始化，它的静态属性也不会初始化）使用。 当且仅当ensureClassInitialized方法不生效时返回false。
public native boolean shouldBeInitialized(Class<?> c);
//检测给定的类是否已经初始化。通常在获取一个类的静态属性的时候（因为一个类如果没初始化，它的静态属性也不会初始化）使用。
public native void ensureClassInitialized(Class<?> c);
//定义一个类，此方法会跳过JVM的所有安全检查，默认情况下，ClassLoader（类加载器）和ProtectionDomain（保护域）实例来源于调用者
public native Class<?> defineClass(String name, byte[] b, int off, int len, ClassLoader loader, ProtectionDomain protectionDomain);
//定义一个匿名类
public native Class<?> defineAnonymousClass(Class<?> hostClass, byte[] data, Object[] cpPatches);
```
<a name="u5lCo"></a>
### 示例：`staticFieldOffset`、`staticFieldBase`、`staticFieldBase`
```java
import lombok.extern.slf4j.Slf4j;
import sun.misc.Unsafe;

import java.lang.reflect.Field;
import java.util.Arrays;
import java.util.List;
import java.util.concurrent.TimeUnit;

@Slf4j
public class Demo7 {

    static Unsafe unsafe;
    //静态属性
    private static Object v1;
    //实例属性
    private Object v2;

    static {
        //获取Unsafe对象
        try {
            Field field = Unsafe.class.getDeclaredField("theUnsafe");
            field.setAccessible(true);
            unsafe = (Unsafe) field.get(null);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) throws NoSuchFieldException {
        Field v1Field = Demo7.class.getDeclaredField("v1");
        Field v2Field = Demo7.class.getDeclaredField("v2");

        System.out.println(unsafe.staticFieldOffset(v1Field));
        System.out.println(unsafe.objectFieldOffset(v2Field));

        System.out.println(unsafe.staticFieldBase(v1Field)==Demo7.class);
    }
}
```
输出：
```
112
12
true
```
可以看出`staticFieldBase`返回的就是Demo2的class对象。
<a name="KOck0"></a>
### 示例：`shouldBeInitialized`、`ensureClassInitialized`
```java
import lombok.extern.slf4j.Slf4j;
import sun.misc.Unsafe;

import java.lang.reflect.Field;
import java.util.concurrent.TimeUnit;

public class Demo8 {

    static Unsafe unsafe;

    static {
        //获取Unsafe对象
        try {
            Field field = Unsafe.class.getDeclaredField("theUnsafe");
            field.setAccessible(true);
            unsafe = (Unsafe) field.get(null);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    static class C1 {
        private static int count;

        static {
            count = 10;
            System.out.println(System.currentTimeMillis() + "," + Thread.currentThread().getName() + "，C1 static init.");
        }
    }

    static class C2 {
        private static int count;

        static {
            count = 11;
            System.out.println(System.currentTimeMillis() + "," + Thread.currentThread().getName() + "，C2 static init.");
        }
    }

    public static void main(String[] args) throws NoSuchFieldException {
        //判断C1类是需要需要初始化，如果已经初始化了，会返回false，如果此类没有被初始化过，返回true
        if (unsafe.shouldBeInitialized(C1.class)) {
            System.out.println("C1需要进行初始化");
            //对C1进行初始化
            unsafe.ensureClassInitialized(C1.class);
        }

        System.out.println(C2.count);
        System.out.println(unsafe.shouldBeInitialized(C1.class));
    }
}
```
输出：
```
C1需要进行初始化
1565069660679,main，C1 static init.
1565069660680,main，C2 static init.
11
false
```
代码中C1未被初始化过，所以`unsafe.shouldBeInitialized(C1.class)`返回true，然后调用`unsafe.ensureClassInitialized(C1.class)`进行初始化。<br />代码中执行`C2.count`会触发C2进行初始化，所以`shouldBeInitialized(C1.class)`返回false
<a name="ov5rs"></a>
## 对象操作的其他方法
```java
//返回对象成员属性在内存地址相对于此对象的内存地址的偏移量
public native long objectFieldOffset(Field f);
//获得给定对象的指定地址偏移量的值，与此类似操作还有：getInt，getDouble，getLong，getChar等
public native Object getObject(Object o, long offset);
//给定对象的指定地址偏移量设值，与此类似操作还有：putInt，putDouble，putLong，putChar等
public native void putObject(Object o, long offset, Object x);
//从对象的指定偏移量处获取变量的引用，使用volatile的加载语义
public native Object getObjectVolatile(Object o, long offset);
//存储变量的引用到对象的指定的偏移量处，使用volatile的存储语义
public native void putObjectVolatile(Object o, long offset, Object x);
//有序、延迟版本的putObjectVolatile方法，不保证值的改变被其他线程立即看到，只有在field被volatile修饰符修饰时有效
public native void putOrderedObject(Object o, long offset, Object x);
//绕过构造方法、初始化代码来创建对象
public native Object allocateInstance(Class<?> cls) throws InstantiationException;
```
`getObject`相当于获取对象中字段的值，`putObject`相当于给字段赋值，有兴趣的可以自己写个例子看看效果。
<a name="wEZmb"></a>
### 绕过构造方法创建对象
介绍一下`allocateInstance`，这个方法可以绕过构造方法来创建对象，示例代码如下：
```java
import sun.misc.Unsafe;

import java.lang.reflect.Field;

public class Demo9 {

    static Unsafe unsafe;

    static {
        //获取Unsafe对象
        try {
            Field field = Unsafe.class.getDeclaredField("theUnsafe");
            field.setAccessible(true);
            unsafe = (Unsafe) field.get(null);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    static class C1 {
        private String name;

        private C1() {
            System.out.println("C1 default constructor!");
        }

        private C1(String name) {
            this.name = name;
            System.out.println("C1 有参 constructor!");
        }
    }

    public static void main(String[] args) throws InstantiationException {
        System.out.println(unsafe.allocateInstance(C1.class));
    }
}
```
输出：
```
com.example.chat21.Demo9$C1@782830e
```
看一下类C1中有两个构造方法，都是`private`的，通过new、反射的方式都无法创建对象。但是可以通过Unsafe的`allocateInstance`方法绕过构造函数来创建C1的实例，输出的结果中可以看出创建成功了，并且没有调用构造方法。
<a name="D4Hfm"></a>
### 典型应用

- **常规对象实例化方式**：通常所用到的创建对象的方式，从本质上来讲，都是通过new机制来实现对象的创建。但是，new机制有个特点就是当类只提供有参的构造函数且无显示声明无参构造函数时，则必须使用有参构造函数进行对象构造，而使用有参构造函数时，必须传递相应个数的参数才能完成对象实例化。
- **非常规的实例化方式**：而`Unsafe`中提供`allocateInstance`方法，仅通过Class对象就可以创建此类的实例对象，而且不需要调用其构造函数、初始化代码、JVM安全检查等。它抑制修饰符检测，也就是即使构造器是`private`修饰的也能通过此方法实例化，只需提类对象即可创建相应的对象。由于这种特性，`allocateInstance`在java.lang.invoke、`Objenesis`（提供绕过类构造器的对象生成方式）、Gson（反序列化时用到）中都有相应的应用。
<a name="HKXO5"></a>
## 数组相关的一些方法
这部分主要介绍与数据操作相关的`arrayBaseOffset`与`arrayIndexScale`这两个方法，两者配合起来使用，即可定位数组中每个元素在内存中的位置。
```java
//返回数组中第一个元素的偏移地址
public native int arrayBaseOffset(Class<?> arrayClass);
//返回数组中一个元素占用的大小
public native int arrayIndexScale(Class<?> arrayClass);
```
这两个与数据操作相关的方法，在java.util.concurrent.atomic 包下的`AtomicIntegerArray`（可以实现对`Integer`数组中每个元素的原子性操作）中有典型的应用，如下图`AtomicIntegerArray`源码所示，通过Unsafe的`arrayBaseOffset`、`arrayIndexScale`分别获取数组首元素的偏移地址base及单个元素大小因子scale。后续相关原子性操作，均依赖于这两个值进行数组中元素的定位，如下图二所示的`getAndAdd`方法即通过`checkedByteOffset`方法获取某数组元素的偏移地址，而后通过CAS实现原子性操作。<br />**数组元素定位：**<br />Unsafe类中有很多以`BASE_OFFSET`结尾的常量，比如`ARRAY_INT_BASE_OFFSET`，`ARRAY_BYTE_BASE_OFFSET`等，这些常量值是通过`arrayBaseOffset`方法得到的。`arrayBaseOffset`方法是一个本地方法，可以获取数组第一个元素的偏移地址。`Unsafe`类中还有很多以`INDEX_SCALE`结尾的常量，比如 `ARRAY_INT_INDEX_SCALE` ， `ARRAY_BYTE_INDEX_SCALE`等，这些常量值是通过`arrayIndexScale`方法得到的。`arrayIndexScale`方法也是一个本地方法，可以获取数组的转换因子，也就是数组中元素的增量地址。将`arrayBaseOffset`与`arrayIndexScale`配合使用，可以定位数组中每个元素在内存中的位置。
<a name="eaRks"></a>
## 内存屏障相关操作
在Java 8中引入，用于定义内存屏障（也称内存栅栏，内存栅障，屏障指令等，是一类同步屏障指令，是CPU或编译器在对内存随机访问的操作中的一个同步点，使得此点之前的所有读写操作都执行后才可以开始执行此点之后的操作），避免代码重排序。
```java
//内存屏障，禁止load操作重排序。屏障前的load操作不能被重排序到屏障后，屏障后的load操作不能被重排序到屏障前
public native void loadFence();
//内存屏障，禁止store操作重排序。屏障前的store操作不能被重排序到屏障后，屏障后的store操作不能被重排序到屏障前
public native void storeFence();
//内存屏障，禁止load、store操作重排序
public native void fullFence();
```
Unsafe相关的就介绍这么多！
