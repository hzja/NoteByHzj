JavaCAS<br />CAS 操作是高并发场景下，性能如此之高的一个重要优化。
<a name="gkhIO"></a>
## 背景
在高并发的业务场景下，线程安全问题是必须考虑的，在JDK5之前，可以通过`synchronized`或`Lock`来保证同步，从而达到线程安全的目的。但`synchronized`或`Lock`方案属于互斥锁的方案，比较重量级，加锁、释放锁都会引起性能损耗问题。<br />而在某些场景下，是可以通过JUC提供的CAS机制实现无锁的解决方案，或者说是它基于类似于乐观锁的方案，来达到非阻塞同步的方式保证线程安全。<br />CAS机制不仅是面试中会高频出现的面试题，而且也是高并发实践中必须掌握的知识点。
<a name="GtXe3"></a>
## 什么是CAS？
CAS是Compare And Swap的缩写，直译就是**比较并交换**。CAS是现代CPU广泛支持的一种对内存中的共享数据进行操作的一种特殊指令，这个指令会对内存中的共享数据做原子的读写操作。其作用是让CPU比较内存中某个值是否和预期的值相同，如果相同则将这个值更新为新值，不相同则不做更新。<br />本质上来讲CAS是一种无锁的解决方案，也是一种基于乐观锁的操作，可以保证在多线程并发中保障共享资源的原子性操作，相对于`synchronized`或`Lock`来说，是一种轻量级的实现方案。<br />Java中大量使用了CAS机制来实现多线程下数据更新的原子化操作，比如`AtomicInteger`、`CurrentHashMap`当中都有CAS的应用。但Java中并没有直接实现CAS，CAS相关的实现是借助C/C++调用CPU指令来实现的，效率很高，但Java代码需通过JNI才能调用。比如，Unsafe类提供的CAS方法（如`compareAndSwapXXX`）底层实现即为CPU指令cmpxchg。
<a name="fovbO"></a>
## CAS的基本流程
下面用一张图来了解一下CAS操作的基本流程。<br />![CAS操作流程图](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1660697618784-2d4f8b11-0253-4024-9695-568154b2d57d.jpeg#clientId=u77848716-e5ed-4&from=paste&id=odbVo&originHeight=506&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u09550f7e-4b3d-4fa5-b068-f9e8fdf0cfc&title=CAS%E6%93%8D%E4%BD%9C%E6%B5%81%E7%A8%8B%E5%9B%BE "CAS操作流程图")<br />在上图中涉及到三个值的比较和操作：修改之前获取的（待修改）值A，业务逻辑计算的新值B，以及待修改值对应的内存位置的C。<br />整个处理流程中，假设内存中存在一个变量i，它在内存中对应的值是A（第一次读取），此时经过业务处理之后，要把它更新成B，那么在更新之前会再读取一下i现在的值C，如果在业务处理的过程中i的值并没有发生变化，也就是A和C相同，才会把i更新（交换）为新值B。如果A和C不相同，那说明在业务计算时，i的值发生了变化，则不更新（交换）成B。最后，CPU会将旧的数值返回。而上述的一系列操作由CPU指令来保证是原子的。<br />在《Java并发编程实践》中对CAS进行了更加通俗的描述：我认为原有的值应该是什么，如果是，则将原有的值更新为新值，否则不做修改，并告诉我原来的值是多少。<br />在上述路程中，可以很清晰的看到乐观锁的思路，而且这期间并没有使用到锁。因此，相对于synchronized等悲观锁的实现，效率要高非常多。
<a name="XcSWZ"></a>
## 基于CAS的`AtomicInteger`使用
关于CAS的实现，最经典最常用的当属`AtomicInteger`了，马上就来看一下`AtomicInteger`是如何利用CAS实现原子性操作的。为了形成更新鲜明的对比，先来看一下如果不使用CAS机制，想实现线程安全通常如何处理。<br />在没有使用CAS机制时，为了保证线程安全，基于`synchronized`的实现如下：
```java
public class ThreadSafeTest {

    public static volatile int i = 0;

    public synchronized void increase() {
        i++;
    }
}
```
至于上面的实例具体实现，这里不再展开，很多相关的文章专门进行讲解，只需要知道为了保证i++的原子操作，在`increase`方法上使用了重量级的锁`synchronized`，这会导致该方法的性能低下，所有调用该方法的操作都需要同步等待处理。<br />那么，如果采用基于CAS实现的`AtomicInteger`类，上述方法的实现便变得简单且轻量级了：
```java
public class ThreadSafeTest {

    private final AtomicInteger counter = new AtomicInteger(0);

    public int increase(){
        return counter.addAndGet(1);
    }

}
```
之所以可以如此安全、便捷地来实现安全操作，便是由于`AtomicInteger`类采用了CAS机制。下面，就来了解一下`AtomicInteger`的功能及源码实现。
<a name="GF0Gs"></a>
## CAS的`AtomicInteger`类
`AtomicInteger`是java.util.concurrent.atomic 包下的一个原子类，该包下还有`AtomicBoolean`, `AtomicLong`,`AtomicLongArray`, `AtomicReference`等原子类，主要用于在高并发环境下，保证线程安全。
<a name="t0BKh"></a>
### `AtomicInteger`常用API
`AtomicInteger`类提供了如下常见的API功能：
```java
public final int get()：获取当前的值
public final int getAndSet(int newValue)：获取当前的值，并设置新的值
public final int getAndIncrement()：获取当前的值，并自增
public final int getAndDecrement()：获取当前的值，并自减
public final int getAndAdd(int delta)：获取当前的值，并加上预期的值
void lazySet(int newValue): 最终会设置成newValue,使用lazySet设置值后，可能导致其他线程在之后的一小段时间内还是可以读到旧的值。
```
上述方法中，`getAndXXX`格式的方法都实现了原子操作。具体的使用方法参考上面的`addAndGet`案例即可。
<a name="Ni6p6"></a>
### `AtomicInteger`核心源码
下面看一下`AtomicInteger`代码中的核心实现代码：
```java
public class AtomicInteger extends Number implements java.io.Serializable {
    private static final Unsafe unsafe = Unsafe.getUnsafe();
    private static final long valueOffset;
    static {
        try {
            // 用于获取value字段相对当前对象的“起始地址”的偏移量
            valueOffset = unsafe.objectFieldOffset(AtomicInteger.class.getDeclaredField("value"));
        } catch (Exception ex) { throw new Error(ex); }
    }

    private volatile int value;

    //返回当前值
    public final int get() {
        return value;
    }

    //递增加detla
    public final int getAndAdd(int delta) {
        // 1、this：当前的实例 
        // 2、valueOffset：value实例变量的偏移量 
        // 3、delta：当前value要加上的数(value+delta)。
        return unsafe.getAndAddInt(this, valueOffset, delta);
    }

    //递增加1
    public final int incrementAndGet() {
        return unsafe.getAndAddInt(this, valueOffset, 1) + 1;
    }
...
}
```
上述代码以`AtomicInteger#incrementAndGet`方法为例展示了`AtomicInteger`的基本实现。其中，在static静态代码块中，基于Unsafe类获取value字段相对当前对象的“起始地址”的偏移量，用于后续`Unsafe`类的处理。<br />在处理自增的原子操作时，使用的是Unsafe类中的`getAndAddInt`方法，CAS的实现便是由Unsafe类的该方法提供，从而保证自增操作的原子性。<br />同时，在`AtomicInteger`类中，可以看到value值通过`volatile`进行修饰，保证了该属性值的线程可见性。在多并发的情况下，一个线程的修改，可以保证到其他线程立马看到修改后的值。<br />通过源码可以看出， `AtomicInteger` 底层是通过**volatile**变量和CAS两者相结合来保证更新数据的原子性。其中关于Unsafe类对CAS的实现，下面详细介绍。
<a name="cGaKB"></a>
## CAS的工作原理
CAS的实现原理简单来说就是由**Unsafe类**和其中的**自旋锁**来完成的，下面针对源代码来看一下这两块的内容。
<a name="pTS3i"></a>
### UnSafe类
在`AtomicInteger`核心源码中，已经看到CAS的实现是通过Unsafe类来完成的，先来了解一下Unsafe类的作用。<br />sun.misc.Unsafe是JDK内部用的工具类。它通过暴露一些Java意义上说“不安全”的功能给Java层代码，来让JDK能够更多的使用Java代码来实现一些原本是平台相关的、需要使用native语言（例如C或C++）才可以实现的功能。该类不应该在JDK核心类库之外使用，这也是命名为Unsafe（不安全）的原因。<br />JVM的实现可以自由选择如何实现Java对象的“布局”，也就是在内存里Java对象的各个部分放在哪里，包括对象的实例字段和一些元数据之类。<br />Unsafe里关于对象字段访问的方法把对象布局抽象出来，它提供了`objectFieldOffset()`方法用于获取某个字段相对Java对象的“起始地址”的偏移量，也提供了`getInt`、`getLong`、`getObject`之类的方法可以使用前面获取的偏移量来访问某个Java对象的某个字段。在`AtomicInteger`的`static`代码块中便使用了`objectFieldOffset()`方法。<br />Unsafe类的功能主要分为内存操作、CAS、Class相关、对象操作、数组相关、内存屏障、系统相关、线程调度等功能。这里只需要知道其功能即可，方便理解CAS的实现，注意不建议在日常开发中使用。
<a name="RyLGY"></a>
### Unsafe与CAS
`AtomicInteger`调用了`Unsafe#getAndAddInt`方法：
```java
public final int incrementAndGet() {
    return unsafe.getAndAddInt(this, valueOffset, 1) + 1;
}
```
上述代码等于是`AtomicInteger`调用UnSafe类的CAS方法，JVM实现出汇编指令，从而实现原子操作。<br />在`Unsafe`中`getAndAddInt`方法实现如下：
```java
public final int getAndAddInt(Object var1, long var2, int var4) {
    int var5;
    do {
        var5 = this.getIntVolatile(var1, var2);
    } while(!this.compareAndSwapInt(var1, var2, var5, var5 + var4));
    return var5;
}
```
`getAndAddInt`方法有三个参数：

- **第一个参数表示当前对象，也就是**`**new**`**的那个**`**AtomicInteger**`**对象；**
- **第二个表示内存地址；**
- **第三个表示自增步伐，在**`**AtomicInteger#incrementAndGet**`**中默认的自增步伐是1。**

`getAndAddInt`方法中，首先把当前对象主内存中的值赋给val5，然后进入while循环。判断当前对象此刻主内存中的值是否等于val5，如果是，就自增（交换值），否则继续循环，重新获取val5的值。<br />在上述逻辑中核心方法是`compareAndSwapInt`方法，它是一个`native`方法，这个方法汇编之后是CPU原语指令，原语指令是连续执行不会被打断的，所以可以保证原子性。<br />在`getAndAddInt`方法中还涉及到一个实现**自旋锁**。所谓的自旋，其实就是上面`getAndAddInt`方法中的do while循环操作。当预期值和主内存中的值不等时，就重新获取主内存中的值，这就是自旋。<br />这里可以看到CAS实现的一个缺点：内部使用**自旋**的方式进行**CAS**更新（while循环进行CAS更新，如果更新失败，则循环再次重试）。如果长时间都不成功的话，就会造成CPU极大的开销。<br />另外，Unsafe类还支持了其他的CAS方法，比如`compareAndSwapObject`、 `compareAndSwapInt`、`compareAndSwapLong`。
<a name="v4v5f"></a>
## CAS的缺点
CAS高效地实现了原子性操作，但在以下三方面还存在着一些缺点：

- **循环时间长，开销大；**
- **只能保证一个共享变量的原子操作；**
- **ABA问题；**

下面就这个三个问题详细讨论一下。
<a name="vQOdj"></a>
### 循环时间长开销大
在分析Unsafe源代码的时候已经提到，在Unsafe的实现中使用了自旋锁的机制。在该环节如果CAS操作失败，就需要循环进行CAS操作(do while循环同时将期望值更新为最新的)，如果长时间都不成功的话，那么会造成CPU极大的开销。如果JVM能支持处理器提供的pause指令那么效率会有一定的提升。
<a name="nGpU1"></a>
### 只能保证一个共享变量的原子操作
在最初的实例中，可以看出是针对一个共享变量使用了CAS机制，可以保证原子性操作。但如果存在多个共享变量，或一整个代码块的逻辑需要保证线程安全，CAS就无法保证原子性操作了，此时就需要考虑采用加锁方式（悲观锁）保证原子性，或者有一个取巧的办法，把多个共享变量合并成一个共享变量进行CAS操作。
<a name="Y7gWf"></a>
### ABA问题
虽然使用CAS可以实现非阻塞式的原子性操作，但是会产生ABA问题，ABA问题出现的基本流程：

- **进程P1在共享变量中读到值为A；**
- **P1被抢占了，进程P2执行；**
- **P2把共享变量里的值从A改成了B，再改回到A，此时被P1抢占；**
- **P1回来看到共享变量里的值没有被改变，于是继续执行；**

虽然P1以为变量值没有改变，继续执行了，但是这个会引发一些潜在的问题。ABA问题最容易发生在lock free的算法中的，CAS首当其冲，因为CAS判断的是指针的地址。如果这个地址被重用了呢，问题就很大了（地址被重用是很经常发生的，一个内存分配后释放了，再分配，很有可能还是原来的地址）。<br />维基百科上给了一个形象的例子：你拿着一个装满钱的手提箱在飞机场，此时过来了一个火辣性感的美女，然后她很暖昧地挑逗着你，并趁你不注意，把用一个一模一样的手提箱和你那装满钱的箱子调了个包，然后就离开了，你看到你的手提箱还在那，于是就提着手提箱去赶飞机去了。<br />ABA问题的解决思路就是使用版本号：在变量前面追加上版本号，每次变量更新的时候把版本号加1，那么A->B->A就会变成1A->2B->3A。<br />另外，从Java 1.5开始，JDK的Atomic包里提供了一个类`AtomicStampedReference`来解决ABA问题。这个类的compareAndSet方法的作用是首先检查当前引用是否等于预期引用，并且检查当前标志是否等于预期标志，如果全部相等，则以原子方式将该引用和该标志的值设置为给定的更新值。
