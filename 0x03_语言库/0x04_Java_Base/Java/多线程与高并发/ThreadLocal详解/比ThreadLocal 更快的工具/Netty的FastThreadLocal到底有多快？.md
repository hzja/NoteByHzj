JavaNettyFastThreadLocal<br />这里使用 Netty 源码版本为 netty-4.1.36.Final，在了解 FastThreadLocal 之前，可以先了解 ThreadLocal。
<a name="r9zRW"></a>
## 解决的问题
在多线程环境下访问共享变量？大家都能想到的是通过加锁串行化处理可以解决，但是在高并发的场景下，加锁操作是不是就存在瓶颈了？<br />JDK 自带的 ThreadLocal 的出现，可以保证多线程下无锁化的线程安全；可以发现很多开源框架中大量使用了 ThreadLocal 解决这一类问题。<br />FastThreadLocal 为什么出现？离开了具体场景谈技术的应用难免有些不妥，再好的东西也有它最适用的范围。<br />基于 Netty 的应用场景，演化出了更适用于它的 FastThreadLocal 来访问控制共享变量。
<a name="dRU5P"></a>
## 对比 JDK 自带的 ThreadLocal
对于 ThreadLocal 这种线程间数据隔离方案，真实的数据维护其实是由线程自己维护并引用。<br />比如 JDK 的 ThreadLocal 实现方案是由线程内部维护了一张 Map 结构存储这些数据，用的时候直接取就可以。<br />JDK自带的 ThreadLocal，Thread 内部维护了 `ThreadLocal.ThreadLocalMap` 这样一个引用关系，使用的 hash + 线性探测解决冲突的一套方案。<br />这套方案在大多场景下性能都是很 OK 的，毕竟一个 Thread 内部也不会存在很多 ThreadLocal 对象，换句话说，存在 hash 冲突的情况就很小，那读写操作都是 O(1) 自然性能很好，又能回收并复用过期的 hash 槽，空间效率也很 OK。<br />但对于一个线程内部可能出现很多的 ThreadLocal 对象的场景，JDK 那套 hash 冲突的解决方案可能就不那么美妙了。<br />对于 Netty 这套网络通信框架而言，即有底层通信层的封装、也有编/解码的处理、更很多业务层的逻辑，可能存在一个线程中维护很多 threadLocal 对象的场景，因此 Netty 从 JDK 的 TreadLocal 中衍生出适合自己业务的 FastThreadLocal。<br />主要的改进场景在于：线程维护的结构由 Map 变成了 Array 数组结构，最大的好处是可以 O(1) 读写，但也存在数据很大的场景；典型的空间换时间思想的应用。
<a name="ivZF7"></a>
## FastThreadLocal 为什么快
<a name="hT9KH"></a>
### 重要结构
FastThreadLocal 的实现与 ThreadLocal 非常类似，Netty 为 FastThreadLocal 量身打造了 FastThreadLocalThread 和 InternalThreadLocalMap 两个重要的类。<br />FastThreadLocalThread 是对 Thread 类做了一层扩展，也就是通过扩展的 InternalThreadLocalMap 结构维护自己的一套数据。<br />只有 FastThreadLocal 和 FastThreadLocalThread 组合使用时，才能发挥 FastThreadLocal 的性能优势。<br />因为不使用 FastThreadLocalThread 就没有扩展结构存储数据。
```java
public class FastThreadLocalThread extends Thread {
	private InternalThreadLocalMap threadLocalMap;
	// 省略其他代码
}
```
有了自己扩展的结构 InternalThreadLocalMap，就可以不再使用 Thread 中的 ThreadLocalMap。<br />所以想知道 FastThreadLocalThread 高性能的奥秘，必须要了解 InternalThreadLocalMap 的设计原理。
<a name="jN3DG"></a>
### 提升的关键
前面讲到 ThreadLocal 的一个重要缺点，就是 ThreadLocalMap 采用线性探测法解决 Hash 冲突性能较慢，那么 InternalThreadLocalMap 又是如何优化的呢？<br />先来看看 InternalThreadLocalMap 的内部构造：
```java
public final class InternalThreadLocalMap extends UnpaddedInternalThreadLocalMap {

    private static final InternalLogger logger = InternalLoggerFactory.getInstance(InternalThreadLocalMap.class);

    private static final int DEFAULT_ARRAY_LIST_INITIAL_CAPACITY = 8;
    private static final int STRING_BUILDER_INITIAL_SIZE;
    private static final int STRING_BUILDER_MAX_SIZE;

    public static final Object UNSET = new Object();

    private BitSet cleanerFlags;

    // ...

    public static InternalThreadLocalMap get() {
        Thread thread = Thread.currentThread();
        if (thread instanceof FastThreadLocalThread) {
            return fastGet((FastThreadLocalThread) thread);
        } else {
            // 兜底操作, 如果不是FastThreadLocalThread，就使用JDK的ThreadLocal处理
            return slowGet();
        }
    }

    public static void remove() {
        Thread thread = Thread.currentThread();
        if (thread instanceof FastThreadLocalThread) {
            ((FastThreadLocalThread) thread).setThreadLocalMap(null);
        } else {
            slowThreadLocalMap.remove();
        }
    }

    // 通过线程内部的AtomicInteger原子性递增的获取数据下标
    // 随着逐步递增，下标也越来越大，也就是数组越来越大，这是它最大的缺点之一
    public static int nextVariableIndex() {
        int index = nextIndex.getAndIncrement();
        if (index < 0) {
            nextIndex.decrementAndGet();
            throw new IllegalStateException("too many thread-local indexed variables");
        }
        return index;
    }

    // ...
```
在 FastThreadLocal 初始化的时候分配一个数组索引 index，index 的值采用原子类 AtomicInteger 保证顺序递增，通过调用 `InternalThreadLocalMap.nextVariableIndex()` 方法获得。<br />然后在读写数据的时候通过数组下标 index 直接定位到 FastThreadLocal 的位置，时间复杂度为 O(1)。<br />如果数组下标递增到非常大，那么数组也会比较大，所以 FastThreadLocal 是通过空间换时间的思想提升读写性能。<br />InternalThreadLocalMa p数组下标 0 存的非实质性数据，是一个 `Set<FastThreadLocal<?>>` 类型，也就是所有的 FastThreadLocal 引用集合，用于扩展 remove 操作。<br />从数组下标 1 开始都是直接存储的 value 数据，不再采用 ThreadLocal 的键值对形式进行存储。<br />InternalThreadLocalMap、index 和 FastThreadLocal 之间的关系如下图：<br />![2022-06-08-14-18-09-261821.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1654669254249-f8dd3947-2d54-4a94-8099-7984c569daa9.png#clientId=u2596ff18-4b8d-4&from=ui&id=uddcbae6b&originHeight=750&originWidth=934&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2105726&status=done&style=shadow&taskId=uc18e9b30-9829-4c05-8e65-aeda594475e&title=)
<a name="c2isp"></a>
## FastThreadLocal 源码分析
<a name="OhAR4"></a>
### 使用差异
从基本的使用上来看和 ThreadLocal 基本没有差异，只需要把代码中 Thread、ThreadLocal 替换为 FastThreadLocalThread 和 FastThreadLocal 即可。<br />下面重点对示例中用得到 `FastThreadLocal.set()`/`get()` 方法做深入分析。
<a name="Au44n"></a>
### 重点实现
<a name="rKjxd"></a>
#### `FastThreadLocal.set()`：
```java
/**
 * Set the value for the current thread.
 */
public final void set(V value) {
	if (value != InternalThreadLocalMap.UNSET) {
		InternalThreadLocalMap threadLocalMap = InternalThreadLocalMap.get();
		setKnownNotUnset(threadLocalMap, value);
	} else {
		remove();
	}
}
```
判断 value 是否为缺省值，如果等于缺省值，那么直接调用 `remove()` 方法。如果 value 不等于缺省值，接下来会获取当前线程的 `InternalThreadLocalMap`。然后将 `InternalThreadLocalMap` 中对应数据替换为新的 value。
<a name="giFKY"></a>
#### `InternalThreadLocalMap.get()`：
```java
public static InternalThreadLocalMap get() {
	Thread thread = Thread.currentThread();
	// 如果是FastThreadLocalThread类型，则从FastThreadLocalThread中拿去
	if (thread instanceof FastThreadLocalThread) {
		return fastGet((FastThreadLocalThread) thread);
	} else {
		// 兜底操作
		// 从JDK的ThreadLocal中获取
		return slowGet();
	}
}

private static InternalThreadLocalMap fastGet(FastThreadLocalThread thread) {
	InternalThreadLocalMap threadLocalMap = thread.threadLocalMap();
	// 如果此时 InternalThreadLocalMap 不存在，直接创建一个返回
	if (threadLocalMap == null) {
		thread.setThreadLocalMap(threadLocalMap = new InternalThreadLocalMap());
	}
	return threadLocalMap;
}

private static InternalThreadLocalMap slowGet() {
	ThreadLocal<InternalThreadLocalMap> slowThreadLocalMap = UnpaddedInternalThreadLocalMap.slowThreadLocalMap;
	// 从JDK的ThreadLocal中获取InternalThreadLocalMap对象
	InternalThreadLocalMap ret = slowThreadLocalMap.get();
	if (ret == null) {
		ret = new InternalThreadLocalMap();
		slowThreadLocalMap.set(ret);
	}
	return ret;
}
```
`InternalThreadLocalMap` 初始化，它会初始化一个长度为 32 的 Object 数组，数组中填充着 32 个缺省对象 UNSET 的引用。<br />`slowGet()` 是针对非 `FastThreadLocalThread` 类型的线程发起调用时的一种兜底方案。<br />如果当前线程不是 `FastThreadLocalThread`，内部是没有 InternalThreadLocalMap 属性的。<br />Netty 在 `UnpaddedInternalThreadLocalMap` 中保存了一个 JDK 原生的 ThreadLocal。<br />ThreadLocal 中存放着 `InternalThreadLocalMap`，此时获取 `InternalThreadLocalMap` 就退化成 JDK 原生的 ThreadLocal 获取
<a name="jRFlA"></a>
#### `setKnownNotUnset()`：
```java
/**
 * @return see {@link InternalThreadLocalMap#setIndexedVariable(int, Object)}.
 */
private void setKnownNotUnset(InternalThreadLocalMap threadLocalMap, V value) { 
	// 给指定的下标赋值，如果存在旧值，则直接覆盖
	if (threadLocalMap.setIndexedVariable(index, value)) {
		// 将 FastThreadLocal 对象保存到待清理的 Set 中
		addToVariablesToRemove(threadLocalMap, this);
	}
}
```
<a name="KH3Qa"></a>
#### `threadLocalMap.setIndexedVariable()`：
```java
/**
 * @return {@code true} if and only if a new thread-local variable has been created
 */
public boolean setIndexedVariable(int index, Object value) {
	// indexedVariables就是InternalThreadLocalMap中用于存放数据的数组
	Object[] lookup = indexedVariables;
	if (index < lookup.length) {
		Object oldValue = lookup[index];
		// 直接将数组 index 位置设置为 value，时间复杂度为 O(1)
		lookup[index] = value;
		return oldValue == UNSET;
	} else {
		// 如果容量不够，先扩容，再新增
		expandIndexedVariableTableAndSet(index, value);
		return true;
	}
}
```
<a name="hikAn"></a>
#### `addToVariablesToRemove()`：
```java
private static void addToVariablesToRemove(InternalThreadLocalMap threadLocalMap, FastThreadLocal<?> variable) {
	// variablesToRemoveIndex, 数组下标 固定值0
	Object v = threadLocalMap.indexedVariable(variablesToRemoveIndex);
	Set<FastThreadLocal<?>> variablesToRemove;
	// 逻辑是，要拿到Set<FastThreadLocal<?>>集合，如果没有就创建并塞进indexedVariable[0]中
	if (v == InternalThreadLocalMap.UNSET || v == null) {
		variablesToRemove = Collections.newSetFromMap(new IdentityHashMap<FastThreadLocal<?>, Boolean>());
		threadLocalMap.setIndexedVariable(variablesToRemoveIndex, variablesToRemove);
	} else {
		variablesToRemove = (Set<FastThreadLocal<?>>) v;
	}

	variablesToRemove.add(variable);
}
```
<a name="jYCKz"></a>
#### 将 FastThreadLocal 对象保存到待清理的 Set 中。
这里就解释了 `InternalThreadLocalMap` 的 value 数据为什么是从下标为 1 的位置开始存储了，因为 0 的位置已经被 Set 集合占用了。<br />为什么 `InternalThreadLocalMap` 要在数组下标为 0 的位置存放一个 `FastThreadLocal` 类型的 Set 集合呢？
```java
/**
 * Sets the value to uninitialized; a proceeding call to get() will trigger a call to initialValue().
 */
public final void remove() {
	remove(InternalThreadLocalMap.getIfSet());
}

public static InternalThreadLocalMap getIfSet() {
	Thread thread = Thread.currentThread();
	if (thread instanceof FastThreadLocalThread) {
		return ((FastThreadLocalThread) thread).threadLocalMap();
	}
	return slowThreadLocalMap.get();
}

/**
 * Sets the value to uninitialized for the specified thread local map;
 * a proceeding call to get() will trigger a call to initialValue().
 * The specified thread local map must be for the current thread.
 */
public final void remove(InternalThreadLocalMap threadLocalMap) {
	if (threadLocalMap == null) {
		return;
	}
	// index是当前FastThreadLocal对应在数组中的下标位置
	// 将下标fastThreadLocal对应value置为UNSET
	Object v = threadLocalMap.removeIndexedVariable(index);
	// 删除当前FastThreadLocal对象
	removeFromVariablesToRemove(threadLocalMap, this);

	if (v != InternalThreadLocalMap.UNSET) {
		try {
			// 空操作，子类可扩展
			onRemoval((V) v);
		} catch (Exception e) {
			PlatformDependent.throwException(e);
		}
	}
}
```
因此 Set 集合是为了保存 `FastThreadLocal`对象，好处有几点：

- 删除 `FastThreadLocal` 留扩展接口
- 提高 `removeAll` 的删除效率，不需要去遍历膨胀的数组
- 可以更好地做内存泄露的管理
<a name="nvD4t"></a>
#### `get()`操作：
```java
public final V get(InternalThreadLocalMap threadLocalMap) {
	Object v = threadLocalMap.indexedVariable(index);
	if (v != InternalThreadLocalMap.UNSET) {
		return (V) v;
	}
	// 如果获取到的数组元素是缺省对象，执行初始化操作
	return initialize(threadLocalMap);
}

private V initialize(InternalThreadLocalMap threadLocalMap) {
	V v = null;
	try {
		// 扩展实现, 用户可实现初始化操作
		v = initialValue();
	} catch (Exception e) {
		PlatformDependent.throwException(e);
	}

	threadLocalMap.setIndexedVariable(index, v);
	addToVariablesToRemove(threadLocalMap, this);
	return v;
}
```
如果能拿到对象，就直接返回，拿不到就通过 initialize 初始化对象。构造完用户对象数据之后，接下来就会将它填充到数组 index 的位置，然后再把当前 FastThreadLocal 对象保存到待清理的 Set 中。
<a name="Rt16s"></a>
## 总结
对比`ThreadLocal` 和 `FastThreadLocal`，简单总结下 `FastThreadLocal` 的优势：<br />①高效查找，`FastThreadLocal` 通过 index 直接定位在数组中的位置，O(1) 操作；`ThreadLocal` 内部使用 hash+线性探测，有可能出现冲突，要往后线性查找合适的位置。<br />②安全性更高，JDK 原生的 ThreadLocal 使用不当可能造成内存泄漏。<br />而 `FastThreadLocal` 不仅提供了 `remove()` 主动清除对象的方法，而且在线程池场景中 Netty 还封装了 `FastThreadLocalRunnable`，`FastThreadLocalRunnable` 最后会执行 `FastThreadLocal.removeAll()` 将 Set 集合中所有 `FastThreadLocal` 对象都清理掉。<br />③更高效的扩容，`FastThreadLocal` 相比 ThreadLocal 数据扩容更加简单高效，`FastThreadLocal` 以 index 为基准向上取整到 2 的次幂作为扩容后容量，然后把原数据拷贝到新数组。<br />而 `ThreadLocal` 由于采用的哈希表，所以在扩容后需要再做一轮 rehash。<br />缺点：

- 数组 index 不会复用，会持续增长，空间消耗较大。
- 需要结合 `FastThreadLocalThread` 使用，否则也会退化成 ThreadLocal 处理。
