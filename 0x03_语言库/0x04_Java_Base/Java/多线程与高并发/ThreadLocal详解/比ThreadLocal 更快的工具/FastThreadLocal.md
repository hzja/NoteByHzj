Java FastThreadLocal
<a name="s7h3w"></a>
## 1、FastThreadLocal的引入背景和原理简介
既然jdk已经有ThreadLocal，为何netty还要自己造个`FastThreadLocal`？`FastThreadLocal`快在哪里？<br />这需要从jdk ThreadLocal的本身说起。如下图：<br />![2021-09-15-07-52-23-661518.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631663657155-a95199b0-7d7c-49e5-921d-05d0533e4ed8.png#clientId=u3dea7f31-2312-4&from=ui&id=u48f46fbe&originHeight=506&originWidth=1080&originalType=binary&ratio=1&size=1642680&status=done&style=shadow&taskId=uafa72ae3-2586-4fbb-86f1-0bd9dd1d2f2)<br />在java线程中，每个线程都有一个`ThreadLocalMap`实例变量（如果不使用`ThreadLocal`，不会创建这个Map，一个线程第一次访问某个`ThreadLocal`变量时，才会创建）。<br />该Map是使用线性探测的方式解决hash冲突的问题，如果没有找到空闲的slot，就不断往后尝试，直到找到一个空闲的位置，插入entry，这种方式在经常遇到hash冲突时，影响效率。<br />`FastThreadLocal`(下文简称ftl)直接使用数组避免了hash冲突的发生，具体做法是：每一个FastThreadLocal实例创建时，分配一个下标index；分配index使用AtomicInteger实现，每个FastThreadLocal都能获取到一个不重复的下标。<br />当调用`ftl.get()`方法获取值时，直接从数组获取返回，如`return array[index]`，如下图：<br />![2021-09-15-07-52-24-285117.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631663618813-c983841b-7d20-4db3-aa95-bcf37a828fb8.png#clientId=u3dea7f31-2312-4&from=ui&id=y8a7F&originHeight=506&originWidth=1080&originalType=binary&ratio=1&size=52308&status=done&style=shadow&taskId=u22f61484-4441-44a9-b4fa-3315dcc3f4c)
<a name="BRzgm"></a>
## 2、实现源码分析
根据上文图示可知，ftl的实现，涉及到`InternalThreadLocalMap`、`FastThreadLocalThread`和`FastThreadLocal`几个类，自底向上，先从`InternalThreadLocalMap`开始分析。<br />`InternalThreadLocalMap`类的继承关系图如下：<br />![2021-09-15-07-52-24-973110.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631663619048-a04ac8ae-181a-4e2e-968b-e17f347c646b.png#clientId=u3dea7f31-2312-4&from=ui&id=WJJXN&originHeight=296&originWidth=588&originalType=binary&ratio=1&size=523360&status=done&style=none&taskId=u5380c200-95c0-4d7e-8696-22e3fa72257)
<a name="NKArO"></a>
### 2.1 `UnpaddedInternalThreadLocalMap`的主要属性
```java
static final ThreadLocal<InternalThreadLocalMap> slowThreadLocalMap = new ThreadLocal<InternalThreadLocalMap>();
static final AtomicInteger nextIndex = new AtomicInteger();
Object[] indexedVariables;
```
数组`indexedVariables`就是用来存储ftl的value的，使用下标的方式直接访问。`nextIndex`在ftl实例创建时用来给每个ftl实例分配一个下标，`slowThreadLocalMap`在线程不是ftlt时使用到。
<a name="y7dkW"></a>
### 2.2 `InternalThreadLocalMap`分析
`InternalThreadLocalMap`的主要属性：
```java
// 用于标识数组的槽位还未使用
public static final Object UNSET = new Object();
/**
 * 用于标识ftl变量是否注册了cleaner
 * BitSet简要原理：
 * BitSet默认底层数据结构是一个long[]数组，开始时长度为1，即只有long[0],而一个long有64bit。
 * 当BitSet.set(1)的时候，表示将long[0]的第二位设置为true，即0000 0000 ... 0010（64bit）,则long[0]==2
 * 当BitSet.get(1)的时候，第二位为1，则表示true；如果是0，则表示false
 * 当BitSet.set(64)的时候，表示设置第65位，此时long[0]已经不够用了，扩容处long[1]来，进行存储
 *
 * 存储类似 {index:boolean} 键值对，用于防止一个FastThreadLocal多次启动清理线程
 * 将index位置的bit设为true，表示该InternalThreadLocalMap中对该FastThreadLocal已经启动了清理线程
 */
private BitSet cleanerFlags; 

private InternalThreadLocalMap() {
    super(newIndexedVariableTable());
}

private static Object[] newIndexedVariableTable() {
    Object[] array = new Object[32];
    Arrays.fill(array, UNSET);
    return array;
}
```
比较简单，`newIndexedVariableTable()`方法创建长度为32的数组，然后初始化为`UNSET`，然后传给父类。之后ftl的值就保存到这个数组里面。<br />注意，这里保存的直接是变量值，不是entry，这是和jdk ThreadLocal不同的。`InternalThreadLocalMap`就先分析到这，其他方法在后面分析ftl再具体说。
<a name="Neuay"></a>
### 2.3 ftlt的实现分析
要发挥ftl的性能优势，必须和ftlt结合使用，否则就会退化到jdk的ThreadLocal。ftlt比较简单，关键代码如下：
```java
public class FastThreadLocalThread extends Thread {
    // This will be set to true if we have a chance to wrap the Runnable.
    private final boolean cleanupFastThreadLocals;

    private InternalThreadLocalMap threadLocalMap;

    public final InternalThreadLocalMap threadLocalMap() {
        return threadLocalMap;
    }
    public final void setThreadLocalMap(InternalThreadLocalMap threadLocalMap) {
        this.threadLocalMap = threadLocalMap;
    }
}  
```
ftlt的诀窍就在`threadLocalMap`属性，它继承java Thread，然后聚合了自己的`InternalThreadLocalMap`。后面访问ftl变量，对于ftlt线程，都直接从`InternalThreadLocalMap`获取变量值。
<a name="EG9I6"></a>
### 2.4 ftl实现分析
ftl实现分析基于netty-4.1.34版本，特别地声明了版本，是因为在清除的地方，该版本的源码已经注释掉了`ObjectCleaner`的调用，和之前的版本有所不同。
<a name="AJFTV"></a>
#### 2.4.1 ftl的属性和实例化
```java
private final int index;

public FastThreadLocal() {
    index = InternalThreadLocalMap.nextVariableIndex();
}
```
非常简单，就是给属性index赋值，赋值的静态方法在`InternalThreadLocalMap`：
```java
public static int nextVariableIndex() {
    int index = nextIndex.getAndIncrement();
    if (index < 0) {
        nextIndex.decrementAndGet();
        throw new IllegalStateException("too many thread-local indexed variables");
    }
    return index;
}
```
可见，每个ftl实例以步长为1的递增序列，获取index值，这保证了`InternalThreadLocalMap`中数组的长度不会突增。
<a name="tZ9Ww"></a>
#### 2.4.2 `get()`方法实现分析
```java
public final V get() {
    InternalThreadLocalMap threadLocalMap = InternalThreadLocalMap.get(); // 1
    Object v = threadLocalMap.indexedVariable(index); // 2
    if (v != InternalThreadLocalMap.UNSET) {
        return (V) v;
    }

    V value = initialize(threadLocalMap); // 3
    registerCleaner(threadLocalMap);  // 4
    return value;
}
```
1.先来看看`InternalThreadLocalMap.get()`方法如何获取`threadLocalMap`：<br />=======================InternalThreadLocalMap=======================  

```java
public static InternalThreadLocalMap get() {
    Thread thread = Thread.currentThread();
    if (thread instanceof FastThreadLocalThread) {
        return fastGet((FastThreadLocalThread) thread);
    } else {
        return slowGet();
    }
}

private static InternalThreadLocalMap fastGet(FastThreadLocalThread thread) {
    InternalThreadLocalMap threadLocalMap = thread.threadLocalMap();
    if (threadLocalMap == null) {
        thread.setThreadLocalMap(threadLocalMap = new InternalThreadLocalMap());
    }
    return threadLocalMap;
}    
```
因为结合`FastThreadLocalThread`使用才能发挥`FastThreadLocal`的性能优势，所以主要看`fastGet`方法。该方法直接从ftlt线程获取`threadLocalMap`，还没有则创建一个`InternalThreadLocalMap`实例并设置进去，然后返回。<br />2.`threadLocalMap.indexedVariable(index)`就简单了，直接从数组获取值，然后返回：
```java
public Object indexedVariable(int index) {
    Object[] lookup = indexedVariables;
    return index < lookup.length? lookup[index] : UNSET;
}
```
3.如果获取到的值不是`UNSET`，那么是个有效的值，直接返回。如果是`UNSET`，则初始化。<br />`initialize(threadLocalMap)`方法：
```java
private V initialize(InternalThreadLocalMap threadLocalMap) {
    V v = null;
    try {
        v = initialValue();
    } catch (Exception e) {
        PlatformDependent.throwException(e);
    }

    threadLocalMap.setIndexedVariable(index, v); // 3-1
    addToVariablesToRemove(threadLocalMap, this); // 3-2
    return v;
}
```
3.1.获取ftl的初始值，然后保存到ftl里的数组，如果数组长度不够则扩充数组长度，然后保存，不展开。<br />3.2.`addToVariablesToRemove(threadLocalMap, this)`的实现，是将ftl实例保存在`threadLocalMap`内部数组第0个元素的Set集合中。<br />此处不贴代码，用图示如下：<br />![2021-09-15-07-52-25-627108.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631663656214-9c8a8394-49ad-46da-85fd-c52bda959f5a.png#clientId=u3dea7f31-2312-4&from=ui&id=f9frR&originHeight=846&originWidth=1018&originalType=binary&ratio=1&size=70340&status=done&style=shadow&taskId=u9fc967b8-6efe-4b18-97c6-49616b3aa14)<br />4.`registerCleaner(threadLocalMap)`的实现，netty-4.1.34版本中的源码：
```java
private void registerCleaner(final InternalThreadLocalMap threadLocalMap) {
    Thread current = Thread.currentThread();
    if (FastThreadLocalThread.willCleanupFastThreadLocals(current) || threadLocalMap.isCleanerFlagSet(index)) {
        return;
    }

    threadLocalMap.setCleanerFlag(index);

    // TODO: We need to find a better way to handle this.
    /*
        // We will need to ensure we will trigger remove(InternalThreadLocalMap) so everything will be released
        // and FastThreadLocal.onRemoval(...) will be called.
        ObjectCleaner.register(current, new Runnable() {
            @Override
            public void run() {
                remove(threadLocalMap);

                // It's fine to not call InternalThreadLocalMap.remove() here as this will only be triggered once
                // the Thread is collected by GC. In this case the ThreadLocal will be gone away already.
            }
        });
        */
}
```
由于`ObjectCleaner.register`这段代码在该版本已经注释掉，而余下逻辑比较简单，因此不再做分析。
<a name="MzNGB"></a>
### 2.5 普通线程使用ftl的性能退化
随着`get()`方法分析完毕，`set(value)`方法原理也呼之欲出，限于篇幅，不再单独分析。<br />前文说过，ftl要结合ftlt才能最大地发挥其性能，如果是其他的普通线程，就会退化到jdk的`ThreadLocal`的情况，因为普通线程没有包含`InternalThreadLocalMap`这样的数据结构，接下来看如何退化。<br />从`InternalThreadLocalMap`的`get()`方法看起：<br />=======================InternalThreadLocalMap=======================  

```java
public static InternalThreadLocalMap get() {
    Thread thread = Thread.currentThread();
    if (thread instanceof FastThreadLocalThread) {
        return fastGet((FastThreadLocalThread) thread);
    } else {
        return slowGet();
    }
}

private static InternalThreadLocalMap slowGet() {
    // 父类的类型为jdk ThreadLocald的静态属性，从该threadLocal获取InternalThreadLocalMap
    ThreadLocal<InternalThreadLocalMap> slowThreadLocalMap = UnpaddedInternalThreadLocalMap.slowThreadLocalMap;
    InternalThreadLocalMap ret = slowThreadLocalMap.get();
    if (ret == null) {
        ret = new InternalThreadLocalMap();
        slowThreadLocalMap.set(ret);
    }
    return ret;
}
```
从ftl看，退化操作的整个流程是：从一个jdk的`ThreadLocal`变量中获取`InternalThreadLocalMap`，然后再从`InternalThreadLocalMap`获取指定数组下标的值，对象关系示意图：<br />![2021-09-15-07-52-26-171166.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631663711911-122161db-fe47-497d-8a7a-11421c9bf57e.png#clientId=u3dea7f31-2312-4&from=ui&id=u50a72d89&originHeight=470&originWidth=1080&originalType=binary&ratio=1&size=45248&status=done&style=shadow&taskId=ue87bf714-e3f8-41a3-970a-adbd98f4c82)
<a name="m02jK"></a>
## 3、ftl的资源回收机制
在netty中对于ftl提供了三种回收机制：<br />自动： 使用ftlt执行一个被`FastThreadLocalRunnable `wrap的`Runnable`任务，在任务执行完毕后会自动进行ftl的清理。<br />手动： ftl和`InternalThreadLocalMap`都提供了`remove`方法，在合适的时候用户可以（有的时候也是必须，例如普通线程的线程池使用ftl）手动进行调用，进行显示删除。<br />自动： 为当前线程的每一个ftl注册一个`Cleaner`，当线程对象不强可达的时候，该Cleaner线程会将当前线程的当前ftl进行回收。（netty推荐如果可以用其他两种方式，就不要再用这种方式，因为需要另起线程，耗费资源，而且多线程就会造成一些资源竞争，在netty-4.1.34版本中，已经注释掉了调用`ObjectCleaner`的代码。）
<a name="YVD8L"></a>
## 4、ftl在netty中的使用
ftl在netty中最重要的使用，就是分配`ByteBuf`。基本做法是：每个线程都分配一块内存(`PoolArena`)，当需要分配`ByteBuf`时，线程先从自己持有的`PoolArena`分配，如果自己无法分配，再采用全局分配。<br />但是由于内存资源有限，所以还是会有多个线程持有同一块`PoolArena`的情况。不过这种方式已经最大限度地减轻了多线程的资源竞争，提高程序效率。<br />具体的代码在`PoolByteBufAllocator`的内部类`PoolThreadLocalCache`中：
```java
final class PoolThreadLocalCache extends FastThreadLocal<PoolThreadCache> {

    @Override
    protected synchronized PoolThreadCache initialValue() {
        final PoolArena<byte[]> heapArena = leastUsedArena(heapArenas);
        final PoolArena<ByteBuffer> directArena = leastUsedArena(directArenas);

        Thread current = Thread.currentThread();
        if (useCacheForAllThreads || current instanceof FastThreadLocalThread) {
            // PoolThreadCache即为各个线程持有的内存块的封装  
            return new PoolThreadCache(
                heapArena, directArena, tinyCacheSize, smallCacheSize, normalCacheSize,
                DEFAULT_MAX_CACHED_BUFFER_CAPACITY, DEFAULT_CACHE_TRIM_INTERVAL);
        }
        // No caching so just use 0 as sizes.
        return new PoolThreadCache(heapArena, directArena, 0, 0, 0, 0, 0);
    }
}   
```
