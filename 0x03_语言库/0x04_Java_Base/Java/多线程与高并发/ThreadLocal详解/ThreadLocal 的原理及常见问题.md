Java ThreadLocal<br />`ThreadLocal` 是一个线程内部的数据存储类，通过它可以在指定的线程中存储数据，数据存储以后，只有在指定线程中可以获取到存储的数据，对于其他线程来说无法获取到数据。`Looper`、`ActivityThread` 以及 AMS 中都用到了 `ThreadLocal`。
<a name="DFQay"></a>
### 与 `Synchronized` 的比较
`ThreadLocal` 和 `Synchronized` 都用于解决多线程并发访问。可是 `ThreadLocal` 与 `synchronized` 有本质的差别。`synchronized` 是利用锁的机制，使变量或代码块 在某一时该仅仅能被一个线程訪问。而 `ThreadLocal` 为每个线程都提供了变量的副本，使得每个线程在某一时间访问到的并非同一个对象，这样就隔离了多个线程对数据的数据共享。
<a name="K2UhF"></a>
### 使用场景

1. 当某些数据是以线程为作用域并且不同线程具有不同的数据副本的时候，就可以采用 `ThreadLocal`。比如对于 Handler 来说，它需要获取当前线程的 `Looper`，很显然 `Looper` 的作用域就是线程并且不同线程具有不同的 `Looper`，这个时候通过 `ThreadLocal` 就可以轻松实现 `Looper` 在线程中的存取。
2. 复杂逻辑下的对象传递，比如监听器的传递，有些时候一个线程中的任务过于复杂，这可能表现为函数调用栈比较深以及代码入口的多样性，在这种情况下，又需要监听器能够贯穿整个线程的执行过程，这时可以采用 `ThreadLocal`，采用 `ThreadLocal` 可以让监听器作为线程内的全局对象而存在，在线程内部只要通过 get 方法就可以获取到监听器。
<a name="UUqhr"></a>
### 使用方法及原理
`ThreadLocal` 类接口很简单，只有 4 个方法，先来了解一下：

- `void set(Object value)`<br />设置当前线程的线程局部变量的值。
- `public Object get()`<br />该方法返回当前线程所对应的线程局部变量。
- `public void remove()`<br />将当前线程局部变量的值删除，目的是为了减少内存的占用，该方法是 JDK 5.0 新增的方法。需要指出的是，当线程结束后，对应该线程的局部变量将自动被垃圾回收，所以显式调用该方法清除线程的局部变量并不是必须的操作，但它可以加快内存回收的速度。
- `protected Object initialValue()`<br />返回该线程局部变量的初始值，该方法是一个 `protected` 的方法，显然是为了让子类覆盖而设计的。这个方法是一个延迟调用方法，在线程第 1 次调用 `get()` 或 `set(Object)`时才执行，并且仅执行 1 次。ThreadLocal 中的缺省实现直接返回一 个 `null`。

下面通过例子来说明，首先定义一个 ThreadLocal 对象，选择 Boolean 类型，如下所示
```java
private ThreadLocal<Boolean> mThreadLocal = new ThreadLocal<>();
```
然后分别在主线程、子线程1和子线程2中设置和访问它的值
```java
private void threadLocal() {
    mThreadLocal.set(true);
    Log.d(TAG, "[Thread#main]threadLocal=" + mThreadLocal.get());
    new Thread() {
        @Override
        public void run() {
            super.run();
            mThreadLocal.set(false);
            Log.d(TAG, "[Thread#1]threadLocal=" + mThreadLocal.get());
        }
    }.start();

    new Thread() {
        @Override
        public void run() {
            super.run();
            Log.d(TAG, "[Thread#2]threadLocal=" + mThreadLocal.get());
        }
    }.start();
}
```
日志如下<br />![2021-08-27-13-15-59-046325.jpg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630042309074-eabd791d-7382-4a7d-ba53-3277ede2d356.jpeg#clientId=u804a968c-43b6-4&from=ui&id=u2c478866&originHeight=57&originWidth=640&originalType=binary&ratio=1&size=6403&status=done&style=none&taskId=ubd6a82b8-abb3-4a83-bc85-20aa1366e0a)<br />从上面的日志可以看出，虽然在不同的线程中访问的是同一个 `ThreadLocal` 对象，但是通过 `ThreadLocal` 获取到的值是不一样的。`ThreadLocal` 之所以有这样的效果，是因为不同线程访问同一个 `ThreadLocal` 的 `get` 方法，`ThreadLocal` 内部会从各自的线程中取出一个数组，然后在从数组中根据当前 `ThreadLocal` 的索引去查找出对应的 value 值，很显然，不同线程中的数组是不同的，这就是为什么通过 `ThreadLocal` 可以在不同的线程中维护一套数据的副本并且彼此互不干扰。<br />`ThreadLocal` 是一个泛型类型，它的定义为 `public class ThreadLocal`，只要弄清楚 `ThreadLocal` 的 `get` 和 `set` 方法就可以明白它的工作原理。<br />首先看 `ThreadLocal` 的 `set` 方法
```java
/**
 * Sets the current thread's copy of this thread-local variable
 * to the specified value.  Most subclasses will have no need to
 * override this method, relying solely on the {@link #initialValue}
 * method to set the values of thread-locals.
 *
 * @param value the value to be stored in the current thread's copy of
 *        this thread-local.
 */
public void set(T value) {
    Thread t = Thread.currentThread();
    ThreadLocalMap map = getMap(t);
    if (map != null)
        map.set(this, value);
    else
        createMap(t, value);
}
```
在上面的 `set` 方法中，首先会获取当前线程，通过 `getMap(Thread t)` 来获取 `ThreadLocalMap` ，如果这个 `map` 不为空的话，就将 `ThreadLocal` 和要存放的 `value` 设置进去，不然的话就创建一个 `ThreadLocalMap` 然后再进行设置。
```java
/**
 * Get the map associated with a ThreadLocal. Overridden in
 * InheritableThreadLocal.
 *
 * @param  t the current thread
 * @return the map
 */
ThreadLocalMap getMap(Thread t) {
    return t.threadLocals;
}

/**
 * Create the map associated with a ThreadLocal. Overridden in
 * InheritableThreadLocal.
 *
 * @param t the current thread
 * @param firstValue value for the initial entry of the map
 */
void createMap(Thread t, T firstValue) {
    t.threadLocals = new ThreadLocalMap(this, firstValue);
}
```
`ThreadLocalMap` 是 `ThreadLocal` 里面的静态内部类，而每一个 `Thread` 都有一个对应的 `ThreadLocalMap`，所以 `getMap` 是直接返回 Thread 的成员，在 Thread 类中有一个成员专门用于存储线程的 `ThreadLocal` 的数据如下所示
```java
public class Thread implements Runnable {
   /* ThreadLocal values pertaining to this thread. This map is maintained
    * by the ThreadLocal class.
    */
    ThreadLocal.ThreadLocalMap threadLocals = null;
}
```
在 `threadLocals` 内部有一个数组：`private Entry[] table`，ThreadLocal 的值就是存在这个 table 数组中<br />看下 ThreadLocal 的内部类 ThreadLocalMap 源码：
```java
static class ThreadLocalMap {

    /**
     * The entries in this hash map extend WeakReference, using
     * its main ref field as the key (which is always a
     * ThreadLocal object).  Note that null keys (i.e. entry.get()
     * == null) mean that the key is no longer referenced, so the
     * entry can be expunged from table.  Such entries are referred to
     * as "stale entries" in the code that follows.
     */
    static class Entry extends WeakReference<ThreadLocal> {
        /** The value associated with this ThreadLocal. */
        Object value;
        // 类似于map的key，value结构，key就是ThreadLocal，value就是需要隔离访问的变量
        Entry(ThreadLocal> k, Object v) {
            super(k);
            value = v;
        }
    }
    /**
     * The table, resized as necessary.
     * table.length MUST always be a power of two.
     */
    // 用数组保存了 Entry，因为可能有多个变量需要线程隔离访问
    private Entry[] table;
}
```
可以看到有个 Entry 内部静态类，它继承了 `WeakReference`，总之它记录了两个信息，一个是 `ThreadLocal`类型，一个是 Object 类型的值。`getEntry` 方法则是获取某个 `ThreadLocal` 对应的值，`set` 方法就是更新或赋值相应的 `ThreadLocal` 对应的值。<br />下面看 threadLocals 是如何使用 `set` 方法将 `ThreadLocal` 的值存储到 table 数组中的，如下所示
```java
/**
 * Set the value associated with key.
 *
 * @param key the thread local object
 * @param value the value to be set
 */
private void set(ThreadLocal key, Object value) {

    // We don't use a fast path as with get() because it is at
    // least as common to use set() to create new entries as
    // it is to replace existing ones, in which case, a fast
    // path would fail more often than not.

    Entry[] tab = table;
    int len = tab.length;
    int i = key.threadLocalHashCode & (len-1);

    for (Entry e = tab[i];
         e != null;
         e = tab[i = nextIndex(i, len)]) {
        ThreadLocal k = e.get();

        if (k == key) {
            e.value = value;
            return;
        }

        if (k == null) {
            replaceStaleEntry(key, value, i);
            return;
        }
    }

    tab[i] = new Entry(key, value);
    int sz = ++size;
    if (!cleanSomeSlots(i, sz) && sz >= threshold)
        rehash();
}
```
上面分析了`ThreadLocal`的`set`方法，这里分析下它的`get`方法，如下所示：
```java
/**
 * Returns the value in the current thread's copy of this
 * thread-local variable.  If the variable has no value for the
 * current thread, it is first initialized to the value returned
 * by an invocation of the {@link #initialValue} method.
 *
 * @return the current thread's value of this thread-local
 */
public T get() {
    Thread t = Thread.currentThread();
    ThreadLocalMap map = getMap(t);
    if (map != null) {
        ThreadLocalMap.Entry e = map.getEntry(this);
        if (e != null) {
            @SuppressWarnings("unchecked")
            T result = (T)e.value;
            return result;
        }
    }
    return setInitialValue();
}
```
回顾 `get` 方法，其实就是拿到每个线程独有的 `ThreadLocalMap` 然后再用 `ThreadLocal` 的当前实例，拿到 Map 中的相应的 `Entry`，然后就可以拿到相应的值返回出去。当然，如果 `Map` 为空，还会先进行 `map` 的创建，初 始化等工作。<br />`ThreadLocal` 的 `get()` 方法的逻辑也比较清晰，它同样是取出当前线程的 `threadLocals` 对象，如果这个对象为 null，就调用 `setInitialValue()` 方法
```java
/**
 * Variant of set() to establish initialValue. Used instead
 * of set() in case user has overridden the set() method.
 *
 * @return the initial value
 */
private T setInitialValue() {
    T value = initialValue();
    Thread t = Thread.currentThread();
    ThreadLocalMap map = getMap(t);
    if (map != null)
        map.set(this, value);
    else
        createMap(t, value);
    return value;
}
```
在 `setInitialValue()` 方法中，将 `initialValue()` 的值赋给想要的值，默认情况下，`initialValue()` 的值为 null，当然也可以重写这个方法。
```java
protected T initialValue() {
   return null;
}
```
从 ThreadLocal 的 `set()` 和 `get()` 方法可以看出，他们所操作的对象都是当前线程的 `threalLocals` 对象的 table 数组，因此在不同的线程中访问同一个 `ThreadLocal` 的 `set()` 和 `get()` 方法，他们对 ThreadLocal 所做的 读 / 写 操作权限仅限于各自线程的内部，这就是为什么可以在多个线程中互不干扰地存储和修改数据。
<a name="czVAs"></a>
### 总结
`ThreadLocal` 是线程内部的数据存储类，每个线程中都会保存一个`ThreadLocal.ThreadLocalMap threadLocals = null;`，`ThreadLocalMap` 是 `ThreadLocal` 的静态内部类，里面保存了一个 `private Entry[] table` 数组，这个数组就是用来保存 ThreadLocal 中的值。通过这种方式，就能在多个线程中互不干扰地存储和修改数据。
<a name="ejhle"></a>
### ThreadLocal 引发的内存泄漏分析
预备知识<br />引用 `Object o = new Object();`<br />这个 o，可以称之为对象引用，而 `new Object()` 可以称之为在内存中产生了一个对象实例。<br />![2021-08-27-13-15-59-137351.jpg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630042414491-dbcfad0a-ec5d-418e-b9ad-5436d5e8f76d.jpeg#clientId=u804a968c-43b6-4&from=ui&id=ua3761450&originHeight=321&originWidth=640&originalType=binary&ratio=1&size=12754&status=done&style=shadow&taskId=ud0d338c4-a7e1-4371-8bcc-8c47d6d6190)<br />当写下 `o=null` 时，只是表示 o 不再指向堆中 object 的对象实例，不代表这个对象实例不存在了。<br />强引用就是指在程序代码之中普遍存在的，类似“`Object obj=new Object()`” 这类的引用，只要强引用还存在，垃圾收集器永远不会回收掉被引用的对象实例。<br />软引用是用来描述一些还有用但并非必需的对象。对于软引用关联着的对象， 在系统将要发生内存溢出异常之前，将会把这些对象实例列进回收范围之中进行第二次回收。如果这次回收还没有足够的内存，才会抛出内存溢出异常。在 JDK 1.2 之后，提供了 `SoftReference` 类来实现软引用。<br />弱引用也是用来描述非必需对象的，但是它的强度比软引用更弱一些，被弱 引用关联的对象实例只能生存到下一次垃圾收集发生之前。当垃圾收集器工作时， 无论当前内存是否足够，都会回收掉只被弱引用关联的对象实例。在 JDK 1.2 之 后，提供了 `WeakReference` 类来实现弱引用。<br />虚引用也称为幽灵引用或者幻影引用，它是最弱的一种引用关系。一个对象 实例是否有虚引用的存在，完全不会对其生存时间构成影响，也无法通过虚引用 来取得一个对象实例。为一个对象设置虚引用关联的唯一目的就是能在这个对象 实例被收集器回收时收到一个系统通知。在 JDK 1.2 之后，提供了 `PhantomReference` 类来实现虚引用。<br />内存泄漏的现象<br />代码示例：
```java
/**
 * 类说明：ThreadLocal造成的内存泄漏演示
 */
public class ThreadLocalOOM {
    private static final int TASK_LOOP_SIZE = 500;

    final static ThreadPoolExecutor poolExecutor
            = new ThreadPoolExecutor(5, 5, 1,
            TimeUnit.MINUTES,
            new LinkedBlockingQueue<>());

    static class LocalVariable {
        private byte[] a = new byte[1024*1024*5];/*5M大小的数组*/
    }

    final static ThreadLocal localVariable
            = new ThreadLocal<>();

    public static void main(String[] args) throws InterruptedException {
        Object o = new Object();
        /*5*5=25*/
        for (int i = 0; i < TASK_LOOP_SIZE; ++i) {
            poolExecutor.execute(new Runnable() {
                public void run() {
                    //localVariable.set(new LocalVariable());
                    new LocalVariable();
                    System.out.println("use local varaible");
                    //localVariable.remove();
                }
            });

            Thread.sleep(100);
        }
        System.out.println("pool execute over");
    }
}
```
执行上面 `ThreadLocalOOM`，并将堆内存大小设 置为`-Xmx256m`，启用一个线程池，大小固定为 5 个线程
```java
final static ThreadPoolExecutor poolExecutor = new ThreadPoolExecutor(5, 5,  1,TimeUnit.MINUTES, new LinkedB1ockingQueue<>());
```
首先只简单的在每个任务中 new 出一个数组
```java
static class LocalVariable {
    private byte[] a = new byte[1024*1024*5];
}
final static ThreadLocal<LocalVariable> localVariable = new ThreadLocal<> ();
public static void main (String[] args) throws InterruptedExceptio{
    for (int i =0;i < TASK LOOP_SIZE;++i){
        poolExecutor.execute (new Runnable(){
            public void run() {
                new LocalVariable();
                //localVariable.set (new LocalVariable());
                System.out.println ("use local varaible");
                //localvariable.remove();
            }
        });
```
可以看到内存的实际使用控制在 25M 左右：因为每个任务中会不断 new 出 一个 5M 的数组，5*5=25M，这是很合理的。<br />![2021-08-27-13-15-59-559317.jpg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630042434140-0c985076-8d94-4a1b-8f3b-37e59dd33acc.jpeg#clientId=u804a968c-43b6-4&from=ui&id=luSHV&originHeight=290&originWidth=640&originalType=binary&ratio=1&size=19545&status=done&style=none&taskId=u6cf06311-5f47-4a0a-aa36-068f2d8b85b)<br />当启用了 ThreadLocal 以后：<br />![2021-08-27-13-15-59-697315.jpg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630042434161-90f9ea87-cc6b-4889-9e3c-f93143eab6b6.jpeg#clientId=u804a968c-43b6-4&from=ui&id=Dp6Oe&originHeight=253&originWidth=640&originalType=binary&ratio=1&size=20668&status=done&style=shadow&taskId=u63a58e88-3722-49f3-8f1b-d9cdac4ad9a)<br />![2021-08-27-13-15-59-771316.jpg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630042852781-4630d9be-ea29-418d-969b-e48398bc492e.jpeg#clientId=u804a968c-43b6-4&from=ui&id=u1f85bab8&originHeight=257&originWidth=640&originalType=binary&ratio=1&size=16650&status=done&style=shadow&taskId=ud8f591ad-d6a5-4af6-8509-300aae5228d)<br />内存占用最高升至 150M，一般情况下稳定在 90M 左右，那么加入一个 ThreadLocal 后，内存的占用真的会这么多？<br />于是，加入一行代码：<br />![2021-08-27-13-15-59-875316.jpg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630042852791-9d0fda54-2c09-488c-8eb8-201fc25681e2.jpeg#clientId=u804a968c-43b6-4&from=ui&id=ouxhZ&originHeight=188&originWidth=640&originalType=binary&ratio=1&size=18397&status=done&style=shadow&taskId=u33464605-380c-414d-86d0-0fccaaadafa)<br />再执行，看看内存情况:<br />![2021-08-27-13-16-00-035317.jpg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630042852796-4cf98ddd-b776-4091-b356-f2570808d892.jpeg#clientId=u804a968c-43b6-4&from=ui&id=rQb0g&originHeight=271&originWidth=640&originalType=binary&ratio=1&size=15247&status=done&style=shadow&taskId=uba727c9a-ce1e-40a4-abb7-4176b2a95c3)<br />可以看见最高峰的内存占用也在 25M 左右，和不加 ThreadLocal 表现完全一样。<br />这就充分说明，确实发生了内存泄漏。
<a name="Hso0p"></a>
### 分析
根据前面对 ThreadLocal 的分析，可以知道每个 Thread 维护一个 `ThreadLocalMap`，这个映射表的 key 是 ThreadLocal 实例本身，value 是真正需 要存储的 Object，也就是说 `ThreadLocal` 本身并不存储值，它只是作为一个 key 来让线程从 `ThreadLocalMap` 获取 value。仔细观察 `ThreadLocalMap`，这个 map 是使用 `ThreadLocal` 的弱引用作为 Key 的，弱引用的对象在 GC 时会被回收。<br />因此使用了 ThreadLocal 后，引用链如图所示<br />![2021-08-27-13-16-00-158316.jpg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630042880817-c6861f93-0cd1-4c90-946f-68092d785459.jpeg#clientId=u804a968c-43b6-4&from=ui&id=u7911fbb5&originHeight=292&originWidth=640&originalType=binary&ratio=1&size=17954&status=done&style=shadow&taskId=u50cf0465-5312-4169-ae0c-8e68d7380ee)<br />图中的虚线表示弱引用。<br />这样，当把 ThreadLocal 变量置为 null 以后，没有任何强引用指向 ThreadLocal 实例，所以 ThreadLocal 将会被 GC 回收。这样一来，`ThreadLocalMap` 中就会出现 key 为 null 的 Entry，就没有办法访问这些 key 为 null 的 Entry 的 value，如果当前线程再迟迟不结束的话，这些 key 为 null 的 Entry 的 value 就会一直存在一条强 引用链：Thread Ref -> Thread -> ThreadLocalMap -> Entry -> value，而这块 value 永远不会被访问到了，所以存在着内存泄露。<br />只有当前 thread 结束以后，current thread 就不会存在栈中，强引用断开， Current Thread、Map value 将全部被 GC 回收。最好的做法是不在需要使用 ThreadLocal 变量后，都调用它的 `remove()`方法，清除数据。<br />其实考察 ThreadLocal 的实现，可以看见，无论是 `get()`、`set()`在某些时 候，调用了 `expungeStaleEntry` 方法用来清除 Entry 中 Key 为 null 的 Value，但是这是不及时的，也不是每次都会执行的，所以一些情况下还是会发生内存泄露。 只有 `remove()` 方法中显式调用了 `expungeStaleEntry` 方法。<br />从表面上看内存泄漏的根源在于使用了弱引用，但是另一个问题也同样值得 思考：为什么使用弱引用而不是强引用？<br />下面分两种情况讨论：<br />key 使用强引用：引用 `ThreadLocal` 的对象被回收了，但是 `ThreadLocalMap` 还持有 `ThreadLocal` 的强引用，如果没有手动删除，`ThreadLocal` 的对象实例不会被回收，导致 `Entry` 内存泄漏。<br />key 使用弱引用：引用的 `ThreadLocal` 的对象被回收了，由于 `ThreadLocalMap` 持有 `ThreadLocal` 的弱引用，即使没有手动删除，`ThreadLocal` 的对象实例也会被回收。value 在下一次 `ThreadLocalMap` 调用 `set`，`get`，`remove` 都有机会被回收。<br />比较两种情况，可以发现：由于 `ThreadLocalMap` 的生命周期跟 Thread 一样长，如果都没有手动删除对应 key，都会导致内存泄漏，但是使用弱引用可 以多一层保障。<br />因此，`ThreadLocal` 内存泄漏的根源是：由于 `ThreadLocalMap` 的生命周期跟 Thread 一样长，如果没有手动删除对应 key 就会导致内存泄漏，而不是因为弱引用。<br />总结

- JVM 利用设置 `ThreadLocalMap` 的 `Key` 为弱引用，来避免内存泄露。
- JVM 利用调用 `remove`、`get`、`set` 方法的时候，回收弱引用。
- 当 T`hreadLocal` 存储很多 Key 为 null 的 `Entry` 的时候，而不再去调用 `remove`、 `get`、`set` 方法，那么将导致内存泄漏。
- 使用线程池+` ThreadLocal` 时要小心，因为这种情况下，线程是一直在不断的重复运行的，从而也就造成了 value 可能造成累积的情况。
<a name="QrSeB"></a>
### 错误使用 `ThreadLocal` 导致线程不安全
```java
/**
 * 类说明：ThreadLocal的线程不安全演示
 */
public class ThreadLocalUnsafe implements Runnable {

    public static Number number = new Number(0);

    public void run() {
        //每个线程计数加一
        number.setNum(number.getNum() + 1);
        //将其存储到ThreadLocal中
        value.set(number);
        SleepTools.ms(2);
        //输出num值
        System.out.println(Thread.currentThread().getName() + "=" + value.get().getNum());
    }

    public static ThreadLocal value = new ThreadLocal() {
    };

    public static void main(String[] args) {
        for (int i = 0; i < 5; i++) {
            new Thread(new ThreadLocalUnsafe()).start();
        }
    }

    private static class Number {
        public Number(int num) {
            this.num = num;
        }

        private int num;

        public int getNum() {
            return num;
        }

        public void setNum(int num) {
            this.num = num;
        }

        @Override
        public String toString() {
            return "Number [num=" + num + "]";
        }
    }
}
```
运行结果：<br />![2021-08-27-13-16-00-307321.jpg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630042892331-28d952bf-5f5e-4169-b0e5-b014218cbec6.jpeg#clientId=u804a968c-43b6-4&from=ui&id=ud63f6d46&originHeight=306&originWidth=640&originalType=binary&ratio=1&size=10417&status=done&style=none&taskId=u1394a4c0-63ba-489d-9ce7-bdb9007a44a)<br />为什么每个线程都输出 5？难道他们没有独自保存自己的 Number 副本吗？ 为什么其他线程还是能够修改这个值？仔细考察 ThreadLocal 和 Thead 的代码， 可以发现 `ThreadLocalMap` 中保存的其实是对象的一个引用，这样的话，当有其 他线程对这个引用指向的对象实例做修改时，其实也同时影响了所有的线程持有 的对象引用所指向的同一个对象实例。这也就是为什么上面的程序为什么会输出一样的结果：5 个线程中保存的是同一 Number 对象的引用，在线程睡眠的时候， 其他线程将 num 变量进行了修改，而修改的对象 Number 的实例是同一份，因此它们最终输出的结果是相同的。<br />而上面的程序要正常的工作，应该的用法是让每个线程中的 `ThreadLocal` 都应该持有一个新的 Number 对象。去掉 `public static Number number = new Number(0);`中的 `static` 即可正常工作。
