JavaThreadLocal<br />说到内存溢出，相信各位都知道是什么，但是说到内存泄露，而且还是 ThreadLocal，来说一下这个 ThreadLocal 的内存泄露的原因，以及如何从开发中去避免这个问题。
<a name="JeJ4y"></a>
## 什么是内存泄露
说到内存泄露，这个可能对于初中级的程序员来说，还是比较陌生的，为什么这么说，是因为，JVM 有自己的内存回收机制，所以对于初中级的程序员来说，很少有接触到这个的，而内存泄露的意思呢，就是为程序在申请内存后，无法释放已申请的内存空间，一次内存泄露危害可以忽略，但内存泄露堆积后果很严重，无论多少内存，迟早会被占光。<br />都知道，有时候在定义变量的时候，就应该明白，他需要一段内存空间来存储这个数据信息，而这段内存如果一直不被释放，那么就会导致，内存被占用光，而被占用的这个对象，一直不能被回收掉，这就是内存泄漏。<br />在说 ThreadLocal 的内存泄露之前，先说说 ThreadLocal 的实现原理，然后再分析，他是泄露的原因是什么？
```java
private ThreadLocalMap(ThreadLocalMap parentMap) {
        Entry[] parentTable = parentMap.table;
        int len = parentTable.length;
        setThreshold(len);
        table = new Entry[len];

        for (int j = 0; j < len; j++) {
            Entry e = parentTable[j];
            if (e != null) {
                @SuppressWarnings("unchecked")
                ThreadLocal<Object> key = (ThreadLocal<Object>) e.get();
                if (key != null) {
                    Object value = key.childValue(e.value);
                    Entry c = new Entry(key, value);
                    int h = key.threadLocalHashCode & (len - 1);
                    while (table[h] != null)
                        h = nextIndex(h, len);
                    table[h] = c;
                    size++;
                }
            }
        }
    }
```
每一个 ThreadLocal 维护一个 ThreadLocalMap，key为使用 弱引用 的 ThreadLocal 实例，value为线程变量的副本。<br />那么什么又是弱引用呢？<br />引用的话，其实分为了两类，既然有弱引用，那么必然的，就会有强引用，所以得区分开这个强引用和弱引用。<br />**强引用**<br />使用最普遍的引用，一个对象具有强引用，不会被垃圾回收器回收。当内存空间不足，Java虚 拟机宁愿抛出OutOfMemoryError错误，使程序异常终止，也不回收这种对象。<br />如果想取消强引用和某个对象之间的关联，可以显式地将引用赋值为null，这样可以使JVM在合适的时 间就会回收该对象。<br />**弱引用**<br />JVM进行垃圾回收时，无论内存是否充足，都会回收被弱引用关联的对象。在 java 中，用 java.lang.ref.WeakReference 类来表示。可以在缓存中使用弱引用。<br />上面也说了，每一个 ThreadLocal 维护一个 ThreadLocalMap，key为使用 弱引用 的 ThreadLocal 实例，value为线程变量的副本。而他们之间的引用关系如下图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661474007610-34eb9c17-b25c-424d-8c2f-a0401d65fbc8.png#clientId=u260a5079-31ed-4&from=paste&id=u861a3859&originHeight=628&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3cf5d037-e37b-4c86-b4c6-17c4abab04d&title=)<br />在这个图中，<br />实线表示-强引用<br />虚线表示-弱引用<br />从图中，就能分析出这个 ThreadLocal 是怎么出现内存泄露的了，从图中能够看到，ThreadLocalMap 使用 ThreadLocal 的弱引用作为key，如果一个ThreadLocal不存在外部强引用时，Key(ThreadLocal)势必会被GC回收，这样就会导致 ThreadLocalMap 中 key 为null， 而 value 还存在着强引用，只有 Thead 线程退出以后，value 的强引用链条才会断掉。<br />但如果当前线程再迟迟不结束的话，这些key为null的Entry的value就会一直存在一条强引用链：<br />Thread Ref -> Thread -> ThreaLocalMap -> Entry -> value<br />这个时候，永远无法回收，就会造成 ThreadLocal 出现内存泄露的问题了。<br />这个时候就有读者会问，为什么 ThreaLocalMap   使用 ThreadLocal 的弱引用，而不去使用强引用，使用强引用的话，是不是就不会出现这个内存泄露的问题了呢？<br />其实这完全不是一回事，因为如果这时候 ThreaLocalMap 的 key 为强引用回收 `ThreadLocal` 时，因为 ThreadLocalMap 还持有 ThreadLocal 的强引用，如果没有手动删除，`ThreadLocal` 不会被回收，导致Entry内存泄漏。<br />这是使用强引用的时候，那么使用弱引用的时候是什么样的呢？<br />当 `ThreadLocalMap` 的 key 为弱引用回收 `ThreadLocal` 时，由于 `ThreadLocalMap` 持有 ThreadLocal 的弱引用，即使没有手动删除，`ThreadLocal` 也会被回收。当key为null，在下一次ThreadLocalMap调用`set()`,`get()`，`remove()`方法的时候会被清除value值。<br />所以这么看下来，反而使用弱引用，却是更好的为什么呢？<br />因为使用弱引用可以多一层保障：弱引用`ThreadLocal`不会内存泄漏，对应的value在下一次 `ThreadLocalMap`调用`set()`,`get()`,`remove()`的时候会被清除。<br />因此，`ThreadLocal`内存泄漏的根本原因是：由于 `ThreadLocalMap` 的生命周期跟 Thread 一样长，如果没有手动删除对应key就会导致内存泄漏，而不是因为弱引用。<br />那么继续来看看这个 ThreadLocal 中维护的 `ThreadLocalMap` 的源码分析
```java
static class ThreadLocalMap {
    //hreadLocalMap中数据是存储在Entry类型数组的table中的，Entry继承了WeakReference（弱引用）
    static class Entry extends WeakReference<ThreadLocal<?>> {
            Object value;
            Entry(ThreadLocal<?> k, Object v) {
                super(k);
                value = v;
            }
        }

**成员变量**
//初始容量 
private static final int INITIAL_CAPACITY = 16;

//ThreadLocalMap数据真正存储在table中
private Entry[] table;

//ThreadLocalMap条数        
private int size = 0;

//达到这个大小，则扩容       
private int threshold; 
```
<a name="Iu8JM"></a>
## 构造函数
```java
ThreadLocalMap(ThreadLocal<?> firstKey, Object firstValue) {
    //初始化table数组，INITIAL_CAPACITY默认值为16
    table = new Entry[INITIAL_CAPACITY];
    //key和16取得哈希值
    int i = firstKey.threadLocalHashCode & (INITIAL_CAPACITY - 1);
    //创建节点，设置key-value
    table[i] = new Entry(firstKey, firstValue);
    size = 1;
    //设置扩容阈值
    setThreshold(INITIAL_CAPACITY);
}
```
再来看看他的 `remove` 方法
```java
private void remove(ThreadLocal<?> key) {
    Entry[] tab = table;
    int len = tab.length;
    int i = key.threadLocalHashCode & (len-1);
    //如果threadLocalHashCode计算出的下标找到的key和传入key不同，则证明出现哈希冲突，则循环向下查找
    for (Entry e = tab[i];
         e != null;
         e = tab[i = nextIndex(i, len)]) {
        //如果key相同
        if (e.get() == key) {
            //删除当前Entry
            e.clear();
            //清理
            expungeStaleEntry(i);
            return;
        }
    }
}
```
ThreadLocal 中的 `ThreadLocalMap` 里面还有 set 和 getEntry 方法，还有很多就不多介绍了。<br />那么应该如何合理的使用 ThreadLocal 才能保证内存不泄露呢？

- **每次使用完**`**ThreadLocal**`**都调用它的**`**remove()**`**方法清除数据**
- **将**`**ThreadLocal**`**变量定义成**`**private static**`**，这样就一直存在**`**ThreadLocal**`**的强引用，也就能保证任何时候都能通过**`**ThreadLocal**`**的弱引用访问到**`**Entry**`**的value值，进而清除掉 。**
