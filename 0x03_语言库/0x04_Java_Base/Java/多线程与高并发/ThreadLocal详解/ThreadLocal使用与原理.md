Java ThreadLocal<br />在处理多线程并发安全的方法中，最常用的方法，就是使用锁，通过锁来控制多个不同线程对临界区的访问。<br />但是，无论是什么样的锁，乐观锁或者悲观锁，都会在并发冲突的时候对性能产生一定的影响。<br />那有没有一种方法，可以彻底避免竞争呢？<br />答案是肯定的，这就是ThreadLocal。<br />从字面意思上看，ThreadLocal可以解释成线程的局部变量，也就是说一个ThreadLocal的变量只有当前自身线程可以访问，别的线程都访问不了，那么自然就避免了线程竞争。<br />因此，ThreadLocal提供了一种与众不同的线程安全方式，它不是在发生线程冲突时想办法解决冲突，而是彻底的避免了冲突的发生。
<a name="Bw62b"></a>
### ThreadLocal的基本使用
创建一个ThreadLocal对象：
```java
private ThreadLocal<Integer> localInt = new ThreadLocal<>();
```
上述代码创建一个localInt变量，由于ThreadLocal是一个泛型类，这里指定了localInt的类型为整数。<br />下面展示了如果设置和获取这个变量的值：
```java
public int setAndGet(){
    localInt.set(8);
    return localInt.get();
}
```
上述代码设置变量的值为8，接着取得这个值。<br />由于ThreadLocal里设置的值，只有当前线程自己看得见，这意味着不可能通过其他线程为它初始化值。为了弥补这一点，ThreadLocal提供了一个`withInitial()`方法统一初始化所有线程的ThreadLocal的值：
```java
private ThreadLocal<Integer> localInt = ThreadLocal.withInitial(() -> 6);
```
上述代码将ThreadLocal的初始值设置为6，这对全体线程都是可见的。
<a name="yoYZ2"></a>
### ThreadLocal的实现原理
ThreadLocal变量只在单个线程内可见，那它是如何做到的呢？先从最基本的`get()`方法说起：
```java
public T get() {
    //获得当前线程
    Thread t = Thread.currentThread();
    //每个线程 都有一个自己的ThreadLocalMap，
    //ThreadLocalMap里就保存着所有的ThreadLocal变量
    ThreadLocalMap map = getMap(t);
    if (map != null) {
        //ThreadLocalMap的key就是当前ThreadLocal对象实例，
        //多个ThreadLocal变量都是放在这个map中的
        ThreadLocalMap.Entry e = map.getEntry(this);
        if (e != null) {
            @SuppressWarnings("unchecked")
            //从map里取出来的值就是我们需要的这个ThreadLocal变量
            T result = (T)e.value;
            return result;
        }
    }
    // 如果map没有初始化，那么在这里初始化一下
    return setInitialValue();
}
```
可以看到，所谓的ThreadLocal变量就是保存在每个线程的map中的。这个map就是Thread对象中的threadLocals字段。如下：
```java
ThreadLocal.ThreadLocalMap threadLocals = null;
```
`ThreadLocal.ThreadLocalMap`是一个比较特殊的Map，它的每个`Entry`的key都是一个弱引用：
```java
static class Entry extends WeakReference<ThreadLocal<?>> {
    /** The value associated with this ThreadLocal. */
    Object value;
    //key就是一个弱引用
    Entry(ThreadLocal<?> k, Object v) {
        super(k);
        value = v;
    }
}
```
这样设计的好处是，如果这个变量不再被其他对象使用时，可以自动回收这个ThreadLocal对象，避免可能的内存泄露（注意，Entry中的value，依然是强引用，如何回收，见下文分解）。
<a name="uVAwh"></a>
### 理解ThreadLocal中的内存泄漏问题
虽然ThreadLocalMap中的key是弱引用，当不存在外部强引用的时候，就会自动被回收，但是Entry中的value依然是强引用。这个value的引用链条如下：<br />![2021-05-06-16-57-39-154136.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620292105689-1b7ebd4d-1807-4450-b89c-ace283ed7246.jpeg#clientId=ub943089d-db51-4&from=ui&id=ue17fb0de&originHeight=98&originWidth=649&originalType=binary&size=7801&status=done&style=shadow&taskId=ud4cce189-5db5-4f8b-80ec-5deba07c252)<br />可以看到，只有当Thread被回收时，这个value才有被回收的机会，否则，只要线程不退出，value总是会存在一个强引用。但是，要求每个Thread都会退出，是一个极其苛刻的要求，对于线程池来说，大部分线程会一直存在在系统的整个生命周期内，那样的话，就会造成value对象出现泄漏的可能。处理的方法是，在ThreadLocalMap进行`set()`,`get()`,`remove()`的时候，都会进行清理：<br />以`getEntry()`为例：
```java
private Entry getEntry(ThreadLocal<?> key) {
    int i = key.threadLocalHashCode & (table.length - 1);
    Entry e = table[i];
    if (e != null && e.get() == key)
        //如果找到key，直接返回
        return e;
    else
        //如果找不到，就会尝试清理，如果总是访问存在的key，那么这个清理永远不会进来
        return getEntryAfterMiss(key, i, e);
}
```
下面是`getEntryAfterMiss()`的实现：
```java
private Entry getEntryAfterMiss(ThreadLocal<?> key, int i, Entry e) {
    Entry[] tab = table;
    int len = tab.length;

    while (e != null) {
        // 整个e是entry ，也就是一个弱引用
        ThreadLocal<?> k = e.get();
        //如果找到了，就返回
        if (k == key)
            return e;
        if (k == null)
            //如果key为null，说明弱引用已经被回收了
            //那么就要在这里回收里面的value了
            expungeStaleEntry(i);
        else
            //如果key不是要找的那个，那说明有hash冲突，这里是处理冲突，找下一个entry
            i = nextIndex(i, len);
        e = tab[i];
    }
    return null;
}
```
真正用来回收value的是`expungeStaleEntry()`方法，在`remove()`和`set()`方法中，都会直接或者间接调用到这个方法进行value的清理：<br />从这里可以看到，ThreadLocal为了避免内存泄露，也算是花了一番大心思。不仅使用了弱引用维护key，还会在每个操作上检查key是否被回收，进而再回收value。<br />但是从中也可以看到，ThreadLocal并不能100%保证不发生内存泄漏。<br />比如，很不幸的，`get()`方法总是访问固定几个一直存在的ThreadLocal，那么清理动作就不会执行，如果没有机会调用`set()`和`remove()`，那么这个内存泄漏依然会发生。
:::success
因此，一个良好的习惯依然是：当不需要这个ThreadLocal变量时，主动调用`remove()`，这样对整个系统是有好处的。
:::
<a name="h1UNE"></a>
### ThreadLocalMap中的Hash冲突处理
ThreadLocalMap作为一个HashMap和java.util.HashMap的实现是不同的。对于java.util.HashMap使用的是链表法来处理冲突：<br />![2021-05-06-16-57-39-219959.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620292182761-bdeb88e9-ce6b-4bf3-b96d-1a6c4ac83d1a.jpeg#clientId=ub943089d-db51-4&from=ui&id=ud673e68f&originHeight=189&originWidth=436&originalType=binary&size=12005&status=done&style=shadow&taskId=ueea08941-b098-4248-9916-909ffa457fc)<br />但是，对于ThreadLocalMap，它使用的是简单的线性探测法，如果发生了元素冲突，那么就使用下一个槽位存放：<br />![2021-05-06-16-57-39-280537.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620292194853-f7d0b1d4-5ad2-4dd1-b49a-6ec13cd4c8aa.jpeg#clientId=ub943089d-db51-4&from=ui&id=u17d6edf1&originHeight=175&originWidth=436&originalType=binary&size=8424&status=done&style=shadow&taskId=u6874ff75-1fca-4760-a6af-a49ad70b03d)<br />具体来说，整个`set()`的过程如下：<br />![2021-05-06-16-57-39-365326.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620292206777-d5847868-ce30-42b3-ab8a-5395bae44d6e.jpeg#clientId=ub943089d-db51-4&from=ui&id=ue31615ac&originHeight=1075&originWidth=1080&originalType=binary&size=92264&status=done&style=none&taskId=u6ea3e075-071c-4a1e-9e0c-de0cfc44206)
<a name="YpbgB"></a>
### 可以被继承的ThreadLocal——InheritableThreadLocal
在实际开发过程中，可能会遇到这么一种场景。主线程开了一个子线程，但是希望在子线程中可以访问主线程中的ThreadLocal对象，也就是说有些数据需要进行父子线程间的传递。比如像这样：
```java
public static void main(String[] args) {
    ThreadLocal threadLocal = new ThreadLocal();
    IntStream.range(0,10).forEach(i -> {
        //每个线程的序列号，希望在子线程中能够拿到
        threadLocal.set(i);
        //这里来了一个子线程，我们希望可以访问上面的threadLocal
        new Thread(() -> {
            System.out.println(Thread.currentThread().getName() + ":" + threadLocal.get());
        }).start();
        try {
            Thread.sleep(1000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    });
}
```
执行上述代码，可以看到：
```java
Thread-0:null
Thread-1:null
Thread-2:null
Thread-3:null
```
因为在子线程中，是没有threadLocal的。如果希望子线可以看到父线程的ThreadLocal，那么就可以使用InheritableThreadLocal。顾名思义，这就是一个支持线程间父子继承的ThreadLocal，将上述代码中的threadLocal使用InheritableThreadLocal：
```java
InheritableThreadLocal threadLocal = new InheritableThreadLocal();
```
再执行，就能看到：
```java
Thread-0:0
Thread-1:1
Thread-2:2
Thread-3:3
Thread-4:4
```
可以看到，每个线程都可以访问到从父进程传递过来的一个数据。虽然InheritableThreadLocal看起来挺方便的，但是依然要注意以下几点：

1. 变量的传递是发生在线程创建的时候，如果不是新建线程，而是用了线程池里的线程，就不灵了
2. 变量的赋值就是从主线程的map复制到子线程，它们的value是同一个对象，如果这个对象本身不是线程安全的，那么就会有线程安全问题
