Java
<a name="RcQmW"></a>
## 1、概述
基于JDK1.8。<br />`Unsafe`类位于rt.jar包，`Unsafe`类提供了硬件级别的原子操作，类中的方法都是native方法，它们使用JNI的方式访问本地C++实现库。由此提供了一些绕开JVM的更底层功能，可以提高程序效率。<br />JNI：`Java Native Interface`。使得Java 与 本地其他类型语言（如C、C++）直接交互。<br />Unsafe 是用于扩展 Java 语言表达能力、便于在更高层（Java 层）代码里实现原本要在更低层（C 层）实现的核心库功能用的。这些功能包括直接内存的申请/释放/访问，低层硬件的 `atomic`/`volatile` 支持，创建未初始化对象，通过偏移量操作对象字段、方法、实现线程无锁挂起和恢复等功能。<br />所谓Java对象的“布局”就是在内存里Java对象的各个部分放在哪里，包括对象的实例字段和一些元数据之类。`Unsafe`里关于对象字段访问的方法把对象布局抽象出来，它提供了`objectFieldOffset()`方法用于获取某个字段相对Java对象的“起始地址”的偏移量，也提供了`getInt`、`getLong`、`getObject`之类的方法可以使用前面获取的偏移量来访问某个Java对象的某个字段。<br />`Unsafe`作用可以大致归纳为：

- 内存管理，包括分配内存、释放内存等。
- 非常规的对象实例化。
- 操作类、对象、变量。
- 自定义超大数组操作。
- 多线程同步。包括锁机制、CAS操作等。
- 线程挂起与恢复。
- 内存屏障。
<a name="N7iG9"></a>
## 2、API详解
`Unsafe`中一共有82个`public native`修饰的方法，还有几十个基于这82个`public native`方法的其他方法，一共有114个方法。
<a name="lGYGp"></a>
### 2.1 初始化方法
可以直接在源码里面看到，`Unsafe`是单例模式的类：
```java
private static final Unsafe theUnsafe;
//构造器私有
private Unsafe() {
}
//静态块初始化
static {
    Reflection.registerMethodsToFilter(Unsafe.class, new String[]{"getUnsafe"});
    theUnsafe = new Unsafe();
}
//静态方法获取实例
@CallerSensitive
public static Unsafe getUnsafe() {
    Class var0 = Reflection.getCallerClass();
    if (!VM.isSystemDomainLoader(var0.getClassLoader())) {
        throw new SecurityException("Unsafe");
    } else {
        return theUnsafe;
    }
}
```
从上面的代码知道，好像是可以通过`getUnsafe()`方法获取实例，但是如果调用该方法会得到一个异常:
```java
java.lang.SecurityException: Unsafe

 at sun.misc.Unsafe.getUnsafe(Unsafe.java:90)
 //……………………
```
实际上可以看到`getUnsafe()`方法上有个 `@CallerSensitive` 注解，就是因为这个注解，在执行时候需要做权限判断：只有由主类加载器(`BootStrap classLoader`)加载的类才能调用这个类中的方法（比如rt.jar中的类，就可以调用该方法，原因从类名可以看出来，它是“不安全的”，怎能随意调用，至于有哪些隐患后面会讲）。显然类是由`AppClassLoader`加载的，所以这里直接抛出了异常。<br />因此最简单的使用方式是基于反射获取`Unsafe`实例，代码如下：
```java
Field f = Unsafe.class.getDeclaredField("theUnsafe");
f.setAccessible(true);
Unsafe unsafe = (Unsafe) f.get(null);
```
<a name="Gqaek"></a>
### 2.2 类、对象和变量相关方法
主要包括基于偏移地址获取或者设置变量的值、基于偏移地址获取或者设置数组元素的值、class初始化以及对象非常规的创建等。
<a name="Rmdqi"></a>
##### 2.2.1 对象操作
```java
/*对象操作*/

/*获取对象字段的值*/

//通过给定的Java变量获取引用值。这里实际上是获取一个Java对象o中，获取偏移地址为offset的属性的值，此方法可以突破修饰符的抑制，也就是无视private、protected和default修饰符。
// 类似的方法有getInt、getDouble等等。
public native Object getObject(Object o, long offset);

//此方法和上面的getObject功能类似，不过附加了'volatile'加载语义，也就是强制从主存中获取属性值。类似的方法有getIntVolatile、getDoubleVolatile等等。
// 这个方法要求被使用的属性被volatile修饰，否则功能和getObject方法相同。
public native Object getObjectVolatile(Object o, long offset);

/*修改对象字段的值*/

//设置Java对象o中偏移地址为offset的属性的值为x，此方法可以突破修饰符的抑制，也就是无视private、protected和default修饰符。用于修改修改非基本数据类型的值。
//类似的方法有putInt、putDouble等等,用于修改基本数据类型的值，再次不再赘述。
public native void putObject(Object o, long offset, Object x);


//此方法和上面的putObject功能类似，不过附加了'volatile'加载语义，也就是设置值的时候强制(JMM会保证获得锁到释放锁之间所有对象的状态更新都会在锁被释放之后)更新到主存，从而保证这些变更对其他线程是可见的。
// 类似的方法有putIntVolatile、putDoubleVolatile等等。这个方法要求被使用的属性被volatile修饰，否则功能和putObject方法相同。
public native void putObjectVolatile(Object o, long offset, Object x);

//设置o对象中offset偏移地址offset对应的Object型field的值为指定值x。这是一个有序或者有延迟的putObjectVolatile方法，并且不保证值的改变被其他线程立即看到。
// 只有在field被volatile修饰并且期望被修改的时候使用才会生效。类似的方法有putOrderedInt和putOrderedLong。
// 最终会设置成x，但是可能导致其他线程在之后的一小段时间内还是可以读到旧的值。关于该方法的更多信息可以参考并发编程网翻译的一篇文章《AtomicLong.lazySet是如何工作的？》，文章地址是“http://ifeve.com/how-does-atomiclong-lazyset-work/”。
public native void putOrderedObject(Object o, long offset, Object x);


/*获取对象的字段相对该对象地址的偏移量*/

//返回给定的静态属性在它的类的存储分配中的位置(偏移地址)。即相对于 className.class 的偏移量,通过这个偏移量可以快速定位字段.
// 注意：这个方法仅仅针对静态属性，使用在非静态属性上会抛异常。
public native long staticFieldOffset(Field f);

//返回给定的非静态属性在它的类的存储分配中的位置(偏移地址)。即字段到对象头的偏移量,通过这个偏移量可以快速定位字段.
// 注意：这个方法仅仅针对非静态属性，使用在静态属性上会抛异常。
public native long objectFieldOffset(Field f);

//返回给定的静态属性的位置，配合staticFieldOffset方法使用。实际上，这个方法返回值就是静态属性所在的Class对象的一个内存快照
// 注释中说到，此方法返回的Object有可能为null，它只是一个'cookie'而不是真实的对象，不要直接使用的它的实例中的获取属性和设置属性的方法，它的作用只是方便调用上面提到的像getInt(Object,long)等等的任意方法。
public native Object staticFieldBase(Field f);

/*创建对象*/   
//绕过构造方法、初始化代码来非常规的创建对象
public native Object allocateInstance(Class<?> cls) throws InstantiationException;
```
<a name="lU1Z6"></a>
##### 2.2.2 class 相关
```java
//检测给定的类是否需要初始化。通常需要使用在获取一个类的静态属性的时候(因为一个类如果没初始化，它的静态属性也不会初始化)。
//此方法当且仅当ensureClassInitialized方法不生效的时候才返回false。
public native boolean shouldBeInitialized(Class<?> c);

//检测给定的类是否已经初始化。通常需要使用在获取一个类的静态属性的时候(因为一个类如果没初始化，它的静态属性也不会初始化)。
public native void ensureClassInitialized(Class<?> c);

//定义一个类，返回类实例，此方法会跳过JVM的所有安全检查。默认情况下，ClassLoader(类加载器)和ProtectionDomain(保护域)实例应该来源于调用者。
public native Class<?> defineClass(String name, byte[] b, int off, int len, ClassLoader loader, ProtectionDomain protectionDomain);


///定义一个匿名类,与Java8的lambda表达式相关,会用到该方法实现相应的函数式接口的匿名类,可以看结尾文章链接。
public native Class<?> defineAnonymousClass(Class<?> hostClass, byte[] data, Object[] cpPatches);
```
<a name="zkf7V"></a>
##### 2.2.3 数组元素相关
```java
//返回数组类型的第一个元素的偏移地址(基础偏移地址)。如果arrayIndexScale方法返回的比例因子不为0，你可以通过结合基础偏移地址和比例因子访问数组的所有元素。
// Unsafe中已经初始化了很多类似的常量如ARRAY_BOOLEAN_BASE_OFFSET等。
public native int arrayBaseOffset(Class<?> arrayClass);

//返回数组单个元素的大小，数组中的元素的地址是连续的。
// Unsafe中已经初始化了很多类似的常量如ARRAY_BOOLEAN_INDEX_SCALE等。
public native int arrayIndexScale(Class<?> arrayClass);
```
<a name="TaQ8C"></a>
### 2.3 内存管理
该部分包括了`allocateMemory`（分配内存）、`reallocateMemory`（重新分配内存）、`copyMemory`（拷贝内存）、`freeMemory`（释放内存 ）、`getAddress`（获取内存地址）、`addressSize`、`pageSize`、`getInt`（获取内存地址指向的整数）、`getIntVolatile`（获取内存地址指向的整数，并支持`volatile`语义）、`putInt`（将整数写入指定内存地址）、`putIntVolatile`（将整数写入指定内存地址，并支持`volatile`语义）、`putOrderedInt`（将整数写入指定内存地址、有序或者有延迟的方法）等方法。`getXXX`和`putXXX`包含了各种基本类型的操作。<br />利用`copyMemory`方法，可以实现一个通用的对象拷贝方法，无需再对每一个对象都实现`clone`方法，当然这通用的方法只能做到对象浅拷贝。<br />`Unsafe`分配的内存，不受`Integer.MAX_VALUE`的限制，并且分配在非堆内存，使用它时，需要非常谨慎：忘记手动回收时，会产生内存泄露，可以通过`Unsafe#freeMemory`方法手动回收；非法的地址访问时，会导致JVM崩溃。在需要分配大的连续区域、实时编程（不能容忍JVM延迟）时，可以使用它，因为直接内存的效率会更好，详细介绍可以去看看Java的NIO源码，NIO中使用了这一技术。<br />JDK nio包中通过`ByteBuffer#allocateDirect`方法分配直接内存时，`DirectByteBuffer`的构造函数中就使用到了Unsafe的`allocateMemory`和`setMemory`方法：通过`Unsafe.allocateMemory`分配内存、`Unsafe.setMemory`进行内存初始化，而后构建一个虚引用`Cleaner`对象用于跟踪`DirectByteBuffer`对象的垃圾回收，以实现当`DirectByteBuffer`被垃圾回收时，分配的堆外内存一起被释放（通过在`Cleaner`中调用`Unsafe#freeMemory`方法）。
```java
//获取本地指针的大小(单位是byte)，通常值为4(32位系统)或者8(64位系统)。常量ADDRESS_SIZE就是调用此方法。
public native int addressSize();

//获取本地内存的页数，此值为2的幂次方。
//java.nio下的工具类Bits中计算待申请内存所需内存页数量的静态方法，其依赖于Unsafe中pageSize方法获取系统内存页大小实现后续计算逻辑
public native int pageSize();

//分配一块新的本地内存，通过bytes指定内存块的大小(单位是byte)，返回新开辟的内存的地址。可以通过freeMemory方法释放内存块，或者通过reallocateMemory方法调整内存块大小。
//bytes值为负数或者过大会抛出IllegalArgumentException异常，如果系统拒绝分配内存会抛出OutOfMemoryError异常。
public native long allocateMemory(long bytes);

//通过指定的内存地址address重新调整本地内存块的大小，调整后的内存块大小通过bytes指定(单位为byte)。可以通过freeMemory方法释放内存块，或者通过reallocateMemory方法调整内存块大小。
//bytes值为负数或者过大会抛出IllegalArgumentException异常，如果系统拒绝分配内存会抛出OutOfMemoryError异常。
public native long reallocateMemory(long address, long bytes);

//在给定的内存块中设置值。内存块的地址由对象引用o和偏移地址共同决定，如果对象引用o为null，offset就是绝对地址。第三个参数就是内存块的大小，如果使用allocateMemory进行内存开辟的话，这里的值应该和allocateMemory的参数一致。value就是设置的固定值，一般为0(这里可以参考netty的DirectByteBuffer)。
//一般而言，o为null，所以有个重载方法是public native void setMemory(long offset, long bytes, byte value);，等效于setMemory(null, long offset, long bytes, byte value);。
public native void setMemory(Object o, long offset, long bytes, byte value);

//释放内存
public native void freeMemory(long address);
```
<a name="SPZUI"></a>
### 2.4 多线程同步
主要包括监视器锁定、解锁以及CAS相关的方法。这部分包括了`monitorEnter`、`tryMonitorEnter`、`monitorExit`、`compareAndSwapInt`、`compareAndSwap`等方法。其中`monitorEnter`、`tryMonitorEnter`、`monitorExit`已经被标记为`deprecated`，不建议使用。<br />`Unsafe`类的CAS操作可能是用的最多的，它为Java的锁机制提供了一种新的解决办法，比如`AtomicInteger`等类都是通过该方法来实现的。这是一种乐观锁，通常认为在大部分情况下不出现竞态条件，如果操作失败，会不断重试直到成功。
```java
//锁定对象，必须通过monitorExit方法才能解锁。此方法经过实验是可以重入的，也就是可以多次调用，然后通过多次调用monitorExit进行解锁。
@Deprecated
public native void monitorEnter(Object o);

//解锁对象，前提是对象必须已经调用monitorEnter进行加锁，否则抛出IllegalMonitorStateException异常。
@Deprecated
public native void monitorExit(Object o);

//尝试锁定对象，如果加锁成功返回true，否则返回false。必须通过monitorExit方法才能解锁。
@Deprecated
public native boolean tryMonitorEnter(Object o);

//针对Object对象进行CAS操作。即是对应Java变量引用o，原子性地更新o中偏移地址为offset的属性的值为x，当且仅的偏移地址为offset的属性的当前值为expected才会更新成功返回true，否则返回false。
//o：目标Java变量引用。offset：目标Java变量中的目标属性的偏移地址。expected：目标Java变量中的目标属性的期望的当前值。x：目标Java变量中的目标属性的目标更新值。
//类似的方法有compareAndSwapInt和compareAndSwapLong，在Jdk8中基于CAS扩展出来的方法有getAndAddInt、getAndAddLong、getAndSetInt、getAndSetLong、getAndSetObject，它们的作用都是：通过CAS设置新的值，返回旧的值。
public final native boolean compareAndSwapObject(Object o, long offset, Object expected, Object x);

//获取对象obj 中偏移量为offset 的变量volatile语义的当前值，并设置变量volatile 语义的值为update
long getAndSetLong(Object obj, long offset, long update)

//获取对象obj同中偏移量为offset 的变量volatile语义的当前值，并设置变量值为原始值＋addValue
long getAndAddLong(Object obj, long offset, long addValue)
```
<a name="QMXNh"></a>
### 2.5 线程的挂起和恢复
这部分包括了`park`、`unpark`等方法。<br />将一个线程进行挂起是通过`park`方法实现的，调用 `park`后，线程将一直阻塞直到超时或者中断等条件出现。`unpark`可以终止一个挂起的线程，使其恢复正常。整个并发框架中对线程的挂起操作被封装在 `LockSupport`类中，`LockSupport`类中有各种版本pack方法，但最终都调用了`Unsafe.park()`方法。<br />Java8的新锁`StampedLock`使用该系列方法。
```java
//释放被park阻塞的线程,也可以被使用来终止一个先前调用park导致的阻塞,即这两个方法的调用顺序可以是先unpark再park。
public native void unpark(Object thread);

//阻塞当前线程直到一个unpark方法出现(被调用)、一个用于unpark方法已经出现过(在此park方法调用之前已经调用过)、线程被中断或者time时间到期(也就是阻塞超时)。
// 在time非零的情况下，如果isAbsolute为true，time是相对于新纪元之后的毫秒，否则time表示纳秒。
public native void park(boolean isAbsolute, long time);
```
<a name="Z54GM"></a>
### 2.6 内存屏障
这部分包括了`loadFence`、`storeFence`、`fullFence`等方法。这是在Java 8新引入的，用于定义内存屏障，避免代码重排序。如果了解JVM的`volatile`、锁的内存寓意，那么理解“内存屏障”这几个字应该不会太难，这里只是把它包装成了Java代码。<br />`loadFence()` 表示该方法之前的所有load操作在内存屏障之前完成。同理`storeFence()`表示该方法之前的所有`store`操作在内存屏障之前完成。`fullFence()`表示该方法之前的所有`load`、`store`操作在内存屏障之前完成。
```java
//在该方法之前的所有读操作，一定在load屏障之前执行完成。
public native void loadFence();

//在该方法之前的所有写操作，一定在store屏障之前执行完成
public native void storeFence();

//在该方法之前的所有读写操作，一定在full屏障之前执行完成，这个内存屏障相当于上面两个(load屏障和store屏障)的合体功能。
public native void fullFence();
```
<a name="TYe6F"></a>
### 2.7 其他
```java
//获取系统的平均负载值，loadavg这个double数组将会存放负载值的结果，nelems决定样本数量，nelems只能取值为1到3，分别代表最近1、5、15分钟内系统的平均负载。
//如果无法获取系统的负载，此方法返回-1，否则返回获取到的样本数量(loadavg中有效的元素个数)。实验中这个方法一直返回-1，其实完全可以使用JMX中的相关方法替代此方法。
public native int getLoadAverage(double[] loadavg, int nelems);

//绕过检测机制直接抛出异常。这让我们可以做些特别的事。
public native void throwException(Throwable ee);
```
<a name="ETODx"></a>
## 3、应用
<a name="ZO7FT"></a>
### 3.0 根据偏移量（指针）修改属性值
```java
public class TestUnSafe {
    static final Unsafe UNSAFE;

    //要更新的字段
    private volatile long state;
    
    //记录字段的偏移量
    private static final long stateOffset;

    /**
     * 静态块初始化unsafe,并且获取state字段的偏移量
     */
    static {
        try {
            //反射获取unsafe
            Field f = Unsafe.class.getDeclaredField("theUnsafe");
            f.setAccessible(true);
            UNSAFE = (Unsafe) f.get(null);
            //获取偏移量
            stateOffset = UNSAFE.objectFieldOffset(TestUnSafe.class.getDeclaredField("state"));
        } catch (Exception ex) {
            throw new Error(ex);
        }
    }

    public TestUnSafe(long state) {
        this.state = state;
    }

    public static void main(String[] args) {
        TestUnSafe testUnSafe = new TestUnSafe(0);
        //尝试更改变量值
        boolean b = UNSAFE.compareAndSwapLong(testUnSafe, stateOffset, testUnSafe.state, 2);
        System.out.println(b);
        System.out.println(testUnSafe.state);
    }
}
```
<a name="Vs1cq"></a>
### 3.1 对象的非常规实例化
通常所用到的创建对象的方式，有直接new创建、也有反射创建，其本质都是调用相应的构造器，而使用有参构造函数时，必须传递相应个数的参数才能完成对象实例化。<br />而`Unsafe`中提供`allocateInstance`方法，仅通过Class对象就可以创建此类的实例对象，而且不需要调用其构造函数、初始化代码、JVM安全检查等。并且它抑制修饰符检测，也就是即使构造器是private修饰的也能通过此方法实例化，只需提类对象即可创建相应的对象。<br />由于这种特性，`allocateInstance`在java.lang.invoke、`Objenesis`（提供绕过类构造器的对象生成方式）、Gson（反序列化时用到）中都有相应的应用。在Gson反序列化时，如果类有默认构造函数，则通过反射调用默认构造函数创建实例，否则通过`UnsafeAllocator`来实现对象实例的构造，`UnsafeAllocator`通过调用`Unsafe`的`allocateInstance`实现对象的实例化，保证在目标类无默认构造函数时，反序列化不够影响。<br />案例：
```java
public class UnsafeTest {
    private static Unsafe UNSAFE;

    static {
        try {
            Field field = Unsafe.class.getDeclaredField("theUnsafe");
            field.setAccessible(true);
            UNSAFE = (Unsafe) field.get(null);
        } catch (Exception ignored) {
        }
    }

    public static void main(String[] args) {
        //reflect();
        unsafe();
    }

    /**
     * 反射测试,注释掉无参构造器,方法报错;开放注释,方法执行成功,type字段有值。
     */
    public static void reflect() {
        /*如果没有无参构造器,该反射会抛出异常，其内部还是使用的new关键字*/
        try {
            Class<?> aClass = Class.forName("com.thread.test.juc.unsafe.User");
            Constructor<?> constructor = aClass.getDeclaredConstructor();
            constructor.setAccessible(true);
            User o = (User) constructor.newInstance(null);
            System.out.println(o);
            /*值为vip，正常*/
            System.out.println(o.type);
            System.out.println(o.age);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * UNSAFE测试，注释掉无参构造器，照样成功构造对象，但是type字段为null。这就是没有走构造器的后果之一：没有对字段进行初始化
     */
    public static void unsafe() {
        try {
            /*不需要相应的构造器即可创建对象*/
            User user = (User) UNSAFE.allocateInstance(User.class);
            user.setName("user1");
            System.out.println("instance: " + user);
            user.test();
            /*通过unsafe设置属性值*/
            Field name = user.getClass().getDeclaredField("name");
            UNSAFE.putObject(user, UNSAFE.objectFieldOffset(name), "user2");
            user.test();

            /*值为null,说明unsafe并没有初始化字段。*/
            System.out.println(user.type);
            System.out.println(user.age);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}

class User {
    public String type = "VIP";
    public int age = 20;
    private String name;

    public void setName(String name) {
        this.name = name;
    }

    public void test() {
        System.err.println("hello,world " + name);
    }

    /*private User() {
        System.out.println("constructor");
    }*/

    private User(String name) {
        this.name = name;
    }
}
```
注意：`UNSAFE`测试时，其vip字段并没有获取到值。实际上一个new操作，编译成指令后（`javap -v xx.class`）是3条：<br />![2021-09-27-09-02-29-311984.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632704566765-5c0cca88-1a2e-4432-aade-f4362a976d39.png#clientId=u194f1c0a-69b6-4&from=ui&id=u72bc3030&originHeight=68&originWidth=947&originalType=binary&ratio=1&size=3347&status=done&style=none&taskId=u43742ab4-578c-4ffd-928a-50aaf9af589)

- 第一条指令的意思是根据类型分配一块内存区域
- 第二条指令是把第一条指令返回的内存地址压入操作数栈顶
- 第三条指令是调用类的构造函数，对字段进行显示初始化操作。

`Unsafe.allocateInstance()`方法只做了第一步和第二步，即分配内存空间，返回内存地址，没有做第三步调用构造函数。所以`Unsafe.allocateInstance()`方法创建的对象都是只有初始值，没有默认值也没有构造函数设置的值，因为它完全没有使用`new`机制，直接操作内存创建了对象。
<a name="gZEfb"></a>
### 3.2 超长数组操作
前面讲的`arrayBaseOffset`与`arrayIndexScale`配合起来使用，就可以定位数组中每个元素在内存中的位置。`putByte`和`getByte`则可以获取指定位置的byte数据。<br />常规Java的数组最大值为`Integer.MAX_VALUE`，但是使用`Unsafe`类的内存分配方法可以实现超大数组。实际上这样的数据就可以认为是C数组，因此需要注意在合适的时间释放内存。<br />下例创建分配一段连续的内存（数组），它的容量是Java允许最大容量的两倍（有可能造成JVM崩溃）：
```java
class SuperArray {
    private final static int BYTE = 1;
    private long size;
    private long address;
    private static Unsafe unsafe;

    static {
        try {
            Field field = Unsafe.class.getDeclaredField("theUnsafe");
            field.setAccessible(true);
            unsafe = (Unsafe) field.get(null);
        } catch (Exception e) {
        }
    }

    public SuperArray(long size) {
        this.size = size;
        //得到分配内的起始地址
        address = unsafe.allocateMemory(size * BYTE);
    }

    public void set(long i, byte value) {
        //设置值
        unsafe.putByte(address + i * BYTE, value);
    }

    public int get(long idx) {
        //获取值
        return unsafe.getByte(address + idx * BYTE);
    }

    public long size() {
        return size;
    }

    public static void main(String[] args) {
        //两倍Integer.MAX_VALUE长度
        long SUPER_SIZE = (long) Integer.MAX_VALUE * 2;
        SuperArray array = new SuperArray(SUPER_SIZE);
        System.out.println("Array size:" + array.size()); // 4294967294
        int sum = 0;
        for (int i = 0; i < 100; i++) {
            array.set((long) Integer.MAX_VALUE + i, (byte) 3);
            sum += array.get((long) Integer.MAX_VALUE + i);
        }
        System.out.println("Sum of 100 elements:" + sum);  // 300
    }
}
```
<a name="EiZQz"></a>
### 3.3 包装受检异常为运行时异常
```java
unsafe.throwException(new IOException());
```
<a name="NpB8y"></a>
### 3.4 运行时动态创建类
标准的动态加载类的方法是`Class.forName()`(在编写jdbc程序时，记忆深刻)，使用`Unsafe`也可以动态加载java 的class文件。操作方式就是将`.class`文件读取到字节数据组中，并将其传到`defineClass`方法中。
```java
public class CreateClass {
    private static Unsafe unsafe;
    static {
        try {
            Field field = Unsafe.class.getDeclaredField("theUnsafe");
            field.setAccessible(true);
            unsafe = (Unsafe) field.get(null);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    //Method to read .class file
    private static byte[] getClassContent() throws Exception {
        File f = new File("target/classes/com/thread/test/juc/unsafe/A.class");
        FileInputStream input = new FileInputStream(f);
        byte[] content = new byte[(int) f.length()];
        input.read(content);
        input.close();
        return content;
    }
    public static void main(String[] args) throws Exception {
        //Sample code to creat classes
        byte[] classContents = getClassContent();
        Class c = unsafe.defineClass(null, classContents, 0, classContents.length, CreateClass.class.getClassLoader(), null);
        c.getMethod("a").invoke(c.newInstance());   //aaaa
    }
}
class A {
    public void a() {
        System.out.println("aaaa");
    }
}

```
<a name="aSbS0"></a>
### 3.5 实现浅克隆
使用直接获取内存的方式实现浅克隆。把一个对象的字节码拷贝到内存的另外一个地方，然后再将这个对象转换为被克隆的对象类型。为了表述方便，用S代表要克隆的对象，D表示克隆后的对象，SD表示S的内存地址，DD表示D的内存地址，SIZE表示该对象在内存中的大小。

- 获取原对象的所在的内存地址SD。
- 计算原对象在内存中的大小SIZE。
- 新分配一块内存，大小为原对象大小SIZE，记录新分配内存的地址DD。
- 从原对象内存地址SD处复制大小为SIZE的内存，复制到DD处。
- DD处的SIZE大小的内存就是原对象的浅克隆对象，强制转换为源对象类型就可以了。
<a name="g2XM4"></a>
## 4 总结和注意
从上面的介绍中，可以看到Unsafe非常强大和有趣的功能，但是实际上官方是不推荐在代码中直接使用Unsafe类的。甚至从命名就能看出来"Unsafe"——那肯定就是不安全的意思啦。那么什么不安全呢？都知道C或C++是可以直接操作指针的，指针操作是非常不安全的，这也是Java“去除”指针的原因。<br />回到Unsafe类，类中包含大量操作指针偏移量的方法，偏移量要自己计算，如若使用不当，会对程序带来许多不可控的灾难，JVM直接崩溃亏。因此对它的使用需要慎之又慎，生产级别的代码就更不应该使用Unsafe类了。<br />另外Unsafe类还有很多自主操作内存的方法，这些都是直接内存，而使用的这些内存不受JVM管理(无法被GC)，需要手动管理，一旦出现疏忽很有可能成为内存泄漏的源头。<br />尽管Unsafe是“不安全的”，但是它的“应用”却很广泛。Unsafe在JUC(java.util.concurrent)包中大量使用(主要是CAS)，在netty中方便使用直接内存，还有一些高并发的交易系统为了提高CAS的效率也有可能直接使用到Unsafe，比如Hadoop、Kafka、akka。
