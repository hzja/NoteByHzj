Java
<a name="gp6TN"></a>
### 一、为什么引入 AtomicInteger ？
谈到线程安全，会首先想到了synchronized 和 Lock，但是这种方式又有一个名字，叫做互斥锁，一次只能有一个持有锁的线程进入，再加上还有不同线程争夺锁这个机制，效率比较低，所以又称 悲观锁<br />与之相对应，就有了 乐观锁 的概念：它不加锁去完成某项操作，如果因为冲突失败就重试，直到成功为止。<br />AtomicInteger 保证线程安全就是使用了乐观锁，所以相对于悲观锁，效率更高。<br />在有多个线程同时使用CAS操作一个变量时，只有一个会胜出并成功更新，其余均会失败。失败的线程不会被挂起，仅被告知失败，并且允许再次尝试，当然，也允许失败的线程放弃操作
<a name="LqkbR"></a>
### 二、AtomicInteger 原理分析
<a name="YTS7z"></a>
#### 1. 具体使用
假如想实现一个功能来统计网页访问量，可以使用 `count++` 来统计访问量，但是这个自增操作不是线程安全的。<br />加锁实现：
```java
class Counter {
    private volatile int count = 0;

    public synchronized void increment() {
        count++;
    }

    public int getCount() {
        return count;
    }
}
```
AtomicInteger 实现：
```java
class Counter {
    //使用AtomicInteger之后，不需要加锁，也可以实现线程安全。
    private AtomicInteger count = new AtomicInteger();

    public void increment() {
        count.incrementAndGet();
    }
    
    public int getCount() {
        return count.get();
    }
}
```
<a name="fgwmt"></a>
#### 2. 原理分析
AtomicInteger 类中定义的属性：
```java
// setup to use Unsafe.compareAndSwapInt for updates
private static final Unsafe unsafe = Unsafe.getUnsafe();
private static final long valueOffset;

static {
    try {
        valueOffset = unsafe.objectFieldOffset
            (AtomicInteger.class.getDeclaredField("value"));
    } catch (Exception ex) { throw new Error(ex); }
}
```
Unsafe是JDK内部的工具类，主要实现了平台相关的操作<br />`sun.misc.Unsafe` 是JDK内部用的工具类。它通过暴露一些Java意义上说“不安全”的功能给Java层代码，来让JDK能够更多的使用Java代码来实现一些原本是平台相关的、需要使用native语言（例如C或C++）才可以实现的功能。该类不应该在JDK核心类库之外使用。<br />简单来说：这段代码是为了获取value在堆内存中的偏移量<br />Value的定义：
```java
private volatile int value;
```
volatile 主要特性有两点：防止重排序；实现内存可见性<br />内存可见性的作用是当一个线程修改了共享变量时，另一个线程可以读取到这个修改后的值<br />用CAS操作实现原子性：<br />`AtomicInteger`中有很多方法，例如

- `incrementAndGet()` 相当于i++ ；
- `getAndAdd()` 相当于i+=n 。

从源码中可以看出这几种方法的实现很相似，这里主要分析`incrementAndGet()`方法的源码
```java
public final int incrementAndGet() {
    for (;;) {
        int current = get();
        int next = current + 1;
        if (compareAndSet(current, next))
            return next;
    }
}

public final boolean compareAndSet(int expect, int update) {
    return unsafe.compareAndSwapInt(this, valueOffset, expect, update);
}
```
`incrementAndGet()` 方法实现了自增的操作。<br />核心实现是先获取当前值和目标值（也就是current +1），如果`compareAndSet(current, next)` 返回成功则该方法返回目标值。<br />那么`compareAndSet`是做CAS操作<br />每次从内存中根据内存偏移量（valueOffset）取出数据，将取出的值跟expect 比较，如果数据一致就把内存中的值改为update，如果数据不一致说明内存中的数据已经有过更新，此时就进行回滚(`expect`值不生效)操作。<br />这样使用CAS的方法就保证了原子操作<br />Java中的 `AtomicLong`、`AtomicBoolean` 等方法的基本原理和思想跟`AtomicInteger`基本相同
<a name="uxmDy"></a>
### 三、简单解释
（1）用`volatile`关键字修饰 value 字段，保证了 value 字段对各个线程的可见性，各线程读取value字段时，会先从主从把数据同步到工作内存中，这样保证可见性<br />（2）Unsafe 实现操作原子性，用户在使用时无需增加额外的同步操作，`compareAndSetInt`方法是一个CAS操作，用`native`关键字修饰<br />原理：先比较内存中的值与`expected`是否一致，一致的前提下才赋予新的值x，此时返回true，否则返回false
