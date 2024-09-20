JavaLongAdder
<a name="uEAGS"></a>
### 前言

- `AtomicLong`和`LongAdder`都是可以实现并发计数操作。但实现方式不同，`AtomicLongs`是通过循环cas实 现，`LongAdder`是通过cas+Cell数组分散并发去实现的。
- 两者都能支持非常大的并发，但`AtomicLong`在高并发下竞争比较激烈的情况下，会出现大量的cas操作失败，会造成大量的cas自旋操作，会给cpu带来巨大的消耗。
- `LongAdder`在`AtomicLong`基础上进行了优化处理。`LongAdder`继承了`Striped64`，内部维护了base和Cell数组（都是volatile），在进行操作`LongAdder`的时候，首先会进行base的cas操作，如果操作失败，再进行选取Cell数组的不同的元素进行操作，这样把多个线程分到不同的Cell去操作，大大减少了cas操作失败的可能性。
<a name="S2JTD"></a>
### 源码解析
首先看一下类的定义
```java
public class LongAdder extends Striped64 implements Serializable
```
然后来看一下类的属性和方法<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1656572730113-8ce68a3b-dbdd-4d60-98ec-4102c2299d22.png#clientId=u220b5db5-6964-4&from=paste&id=u96acd245&originHeight=746&originWidth=806&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uccfdad52-64ee-4a57-94a1-ca5913c780f&title=)<br />这里让人困惑的一个问题是`LongAdder`中没有类似于`AtomicLong`中`getAndIncrement()`或者`incrementAndGet()`这样的原子操作，所以只能通过`increment()`方法和`longValue()`方法的组合来实现更新和获取的动作，然而这样不能保证这个组合操作的原子性，猜想也许`LongAdder`就是不具备这样的机制吧。那么就主要看一下`increment()`和`longValue()`方法。<br />`longValue`方法
```java
public long longValue() {
    return sum();
}
```
首先看一下获取值的`longValue()`方法，这里调用了`sum()`
```java
public long sum() {
    Cell[] as = cells; Cell a;
    long sum = base;
    if (as != null) {
        for (int i = 0; i < as.length; ++i) {
            if ((a = as[i]) != null)
                sum += a.value;
        }
    }
    return sum;
}
```
这里出现了一个类`Cell`，这个类是`Striped64`类的静态内部类，因此当`Striped64`对象初始化时并不会连带将`Cell`类初始化，其源码如下
```java
@sun.misc.Contended static final class Cell {
    volatile long value;
    Cell(long x) { value = x; }
    final boolean cas(long cmp, long val) {
        return UNSAFE.compareAndSwapLong(this, valueOffset, cmp, val);
    }


    // Unsafe mechanics
    private static final sun.misc.Unsafe UNSAFE;
    private static final long valueOffset;
    static {
        try {
            UNSAFE = sun.misc.Unsafe.getUnsafe();
            Class<?> ak = Cell.class;
            valueOffset = UNSAFE.objectFieldOffset
                (ak.getDeclaredField("value"));
        } catch (Exception e) {
            throw new Error(e);
        }
    }
}
```
这里有一个`@sun.misc.Contended`标识非常奇怪，查了资料发现是用于解决伪共享（false sharing）问题的，这里就不深入讲解了。这个类只有一个`cas(long cmp, long val)`方法，做的事情也就是简单的CAS操作，接着来看一下cells和base的定义，这两个属性都是Striped64中的
```java
transient volatile Cell[] cells;

transient volatile long base;
```
可以看到全部都是volatile类型的，因此都具有可见性，并且可以推测会遇到并发的操作。<br />回到`sum()`方法就很好理解了，这个方法返回的是base和cells数组中所有元素的和，这里的base像是一个初始值的作用，继续往下分析。<br />`**increment**`**方法**
```java
public void increment() {
    add(1L);
}
```
接着分析`increment()`方法，可以看到该方法就是对`add(long x)`的封装，那么具体来分析一下这个方法
```java
public void add(long x) {
    Cell[] as; long b, v; int m; Cell a;
    if ((as = cells) != null || !casBase(b = base, b + x)) {
        boolean uncontended = true;
        if (as == null || (m = as.length - 1) < 0 ||
            (a = as[getProbe() & m]) == null ||
            !(uncontended = a.cas(v = a.value, v + x)))
            longAccumulate(x, null, uncontended);
    }
}
```
这里重点是`casBase(b = base, b + x)`，来看一下它做了什么
```java
final boolean casBase(long cmp, long val) {
    return UNSAFE.compareAndSwapLong(this, BASE, cmp, val);
}
```
由于此处的x为1，那么该方法就是对Striped64的BASE的值进行累加并返回是否成功，顺带一提这里的BASE是base值所对应的内存偏移量，所以`casBase(b = base, b + x)`就是对base进行CAS操作，执行成功的话操作就结束了，那么什么时候会不成功呢，当然就是并发量大的时候，结合之前分析的`longValue()`方法，这里就可以得出一个结论——当并发不大的时候只对base进行更新，获取值得时候当然也只从base获取即可，这个时候其实和`AtomicLong`的原理几乎一模一样，看来区别就在于后面的分支，继续往下看。<br />首先置标志位`uncontended`为true，从字面意思也可以看出来`uncontended`意思是没有竞争的。当casBase不成功则将进入`add(long x)`的if分支中，分支由四部分组成中间用||连接，必须全部满足才会结束方法执行，否则进入`longAccumulate(x, null, uncontended)`方法，这四部分分别为
```java
as == null，
(m = as.length - 1) < 0
(a = as[getProbe() & m]) == null
!(uncontended = a.cas(v = a.value, v + x))
```
这里的四个条件其实并不是并列的，而是递进式的，1和2判断cells数组是否为空，3取cells数组中的任意一个元素a判断是否为空，4是对a进行cas操作并将执行结果赋值标志位`uncontended`。从这里可以给出第二个结论，当竞争激烈到一定程度无法对base进行累加操作时，会对cells数组中某个元素进行更新。<br />最后来看一下当上述条件只要有一个满足时调用的`**longAccumulate(x, null, uncontended)**`方法
```java
final void longAccumulate(long x, LongBinaryOperator fn,
                          boolean wasUncontended) {
    int h;
    if ((h = getProbe()) == 0) {
        ThreadLocalRandom.current(); // force initialization
        h = getProbe();  //返回当前线程的threadLocalRandomProbe值
        wasUncontended = true;
    }
    boolean collide = false;                // True if last slot nonempty
    for (;;) {
        Cell[] as; Cell a; int n; long v;
        if ((as = cells) != null && (n = as.length) > 0) {
            if ((a = as[(n - 1) & h]) == null) {
                if (cellsBusy == 0) {       // cells数组中对应位置没有数据则插入新对象
                    Cell r = new Cell(x);   
                    if (cellsBusy == 0 && casCellsBusy()) {
                        boolean created = false;
                        try {               // Recheck under lock
                            Cell[] rs; int m, j;
                            if ((rs = cells) != null &&
                                (m = rs.length) > 0 &&
                                rs[j = (m - 1) & h] == null) {
                                rs[j] = r;
                                created = true;
                            }
                        } finally {
                            cellsBusy = 0;
                        }
                        if (created)
                            break;
                        continue;           // Slot is now non-empty
                    }
                }
                collide = false;
            }
            else if (!wasUncontended)       // CAS already known to fail
                wasUncontended = true;      // Continue after rehash
            else if (a.cas(v = a.value, ((fn == null) ? v + x :
                                         fn.applyAsLong(v, x))))         //对该位置的cell元素进行累加
                break;
            else if (n >= NCPU || cells != as)
                collide = false;            // At max size or stale   //判断数组大小是否大于核数
            else if (!collide)
                collide = true;
            else if (cellsBusy == 0 && casCellsBusy()) {         //对cells数组进行扩容，直接扩容为2倍
                try {
                    if (cells == as) {      // Expand table unless stale
                        Cell[] rs = new Cell[n << 1];
                        for (int i = 0; i < n; ++i)
                            rs[i] = as[i];
                        cells = rs;
                    }
                } finally {
                    cellsBusy = 0;
                }
                collide = false;
                continue;                   // Retry with expanded table
            }
            h = advanceProbe(h);
        }
        else if (cellsBusy == 0 && cells == as && casCellsBusy()) {  //cellsBusy这里是做为一个自旋锁来使用的
            boolean init = false;
            try {                           // 初始化cells数组大小为2
                if (cells == as) {
                    Cell[] rs = new Cell[2];
                    rs[h & 1] = new Cell(x);
                    cells = rs;
                    init = true;
                }
            } finally {
                cellsBusy = 0;
            }
            if (init)
                break;
        }
        else if (casBase(v = base, ((fn == null) ? v + x :
                                    fn.applyAsLong(v, x))))      //对base进行CAS操作
            break;                          // Fall back on using base
    }
}
```
这个方法比较长，大致对几个关键点做了注释，该方法主要是用一个死循环对cells数组中的元素进行操作，当要更新的位置的元素为空时插入新的cell元素，否则在该位置进行CAS的累加操作，如果CAS操作失败并且数组大小没有超过核数就扩容cells数组。
<a name="Vc4xZ"></a>
### 总结
`LongAdder`类与`AtomicLong`类的区别在于高并发时前者将对单一变量的CAS操作分散为对数组cells中多个元素的CAS操作，取值时进行求和；而在并发较低时仅对base变量进行CAS操作，与`AtomicLong`类原理相同。不得不说这种分布式的设计还是很巧妙的。
