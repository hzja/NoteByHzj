JavaThreadLocal
<a name="NVi7T"></a>
## 前言
无论是**工作还是面试中**，都会跟ThreadLocal打交道，来看看ThreadLocal的八个关键知识点

1. **ThreadLocal是什么?为什么要使用ThreadLocal**
2. **一个ThreadLocal的使用案例**
3. **ThreadLocal的原理**
4. **为什么不直接用线程id作为ThreadLocalMap的key**
5. **为什么会导致内存泄漏呢？是因为弱引用吗？**
6. **Key为什么要设计成弱引用呢？强引用不行？**
7. **InheritableThreadLocal保证父子线程间的共享数据**
8. **ThreadLocal的应用场景和使用注意点**
<a name="AcHGt"></a>
## 1、ThreadLocal是什么?为什么要使用ThreadLocal？
**ThreadLocal是什么?**<br />ThreadLocal，即线程本地变量。如果创建了一个ThreadLocal变量，那么访问这个变量的每个线程都会有这个变量的一个本地拷贝，多个线程操作这个变量的时候，实际是在操作自己本地内存里面的变量，从而起到**线程隔离**的作用，避免了并发场景下的线程安全问题。
```java
//创建一个ThreadLocal变量
static ThreadLocal<String> localVariable = new ThreadLocal<>();
```
**为什么要使用ThreadLocal**<br />并发场景下，会存在多个线程同时修改一个共享变量的场景。这就可能会出现**线性安全问题**。<br />为了解决线性安全问题，可以用加锁的方式，比如使用`synchronized` 或者`Lock`。但是加锁的方式，可能会导致系统变慢。加锁示意图如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659314623608-9d3dae31-2d5e-452c-9105-12c65fec8d05.png#clientId=u70d59c9d-dceb-4&from=paste&id=u245842f8&originHeight=443&originWidth=589&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u88760c08-df99-4bb0-9024-1a1387ac4ab&title=)<br />还有另外一种方案，就是使用空间换时间的方式，即使用ThreadLocal。使用ThreadLocal类访问共享变量时，会在每个线程的本地，都保存一份共享变量的拷贝副本。多线程对共享变量修改时，实际上操作的是这个变量副本，从而保证线性安全。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659314623620-75e3dd71-ab35-49cb-a132-322f879fa164.png#clientId=u70d59c9d-dceb-4&from=paste&id=ubdf85346&originHeight=307&originWidth=628&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua14e5062-e0e2-4651-bfea-cfb5bfb966f&title=)
<a name="JMwyy"></a>
## 2、一个ThreadLocal的使用案例
日常开发中，ThreadLocal经常在日期转换工具类中出现，先来看个**反例**：
```java
/**
 * 日期工具类
 */
public class DateUtil {

	private static final SimpleDateFormat simpleDateFormat =
		new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");

	public static Date parse(String dateString) {
		Date date = null;
		try {
			date = simpleDateFormat.parse(dateString);
		} catch (ParseException e) {
			e.printStackTrace();
		}
		return date;
	}
}
```
在多线程环境跑DateUtil这个工具类：
```java
public static void main(String[] args) {
	ExecutorService executorService = Executors.newFixedThreadPool(10);

	for (int i = 0; i < 10; i++) {
		executorService.execute(()->{
			System.out.println(DateUtil.parse("2022-07-24 16:34:30"));
		});
	}
	executorService.shutdown();
}
```
运行后，发现报错了：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659314623619-ddb6a502-8016-4490-afbd-efe4f55c7737.png#clientId=u70d59c9d-dceb-4&from=paste&id=u2246b23b&originHeight=374&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4c9fa4a0-787f-4669-a042-76de36b60ad&title=)<br />如果在DateUtil工具类，加上ThreadLocal，运行则不会有这个问题：
```java
/**
 * 日期工具类
 */
public class DateUtil {

	private static ThreadLocal<SimpleDateFormat> dateFormatThreadLocal =
		ThreadLocal.withInitial(() -> new SimpleDateFormat("yyyy-MM-dd HH:mm:ss"));

	public static Date parse(String dateString) {
		Date date = null;
		try {
			date = dateFormatThreadLocal.get().parse(dateString);
		} catch (ParseException e) {
			e.printStackTrace();
		}
		return date;
	}

	public static void main(String[] args) {
		ExecutorService executorService = Executors.newFixedThreadPool(10);

		for (int i = 0; i < 10; i++) {
			executorService.execute(()->{
				System.out.println(DateUtil.parse("2022-07-24 16:34:30"));
			});
		}
		executorService.shutdown();
	}
}
```
运行结果：
```java
Sun Jul 24 16:34:30 GMT+08:00 2022
Sun Jul 24 16:34:30 GMT+08:00 2022
Sun Jul 24 16:34:30 GMT+08:00 2022
Sun Jul 24 16:34:30 GMT+08:00 2022
Sun Jul 24 16:34:30 GMT+08:00 2022
Sun Jul 24 16:34:30 GMT+08:00 2022
Sun Jul 24 16:34:30 GMT+08:00 2022
Sun Jul 24 16:34:30 GMT+08:00 2022
Sun Jul 24 16:34:30 GMT+08:00 2022
Sun Jul 24 16:34:30 GMT+08:00 2022
```
刚刚**反例中**，为什么会报错呢？这是因为`SimpleDateFormat`不是线性安全的，它以共享变量出现时，并发多线程场景下即会报错。<br />为什么加了ThreadLocal就不会有问题呢？并发场景下，ThreadLocal是如何保证的呢？接下来看看ThreadLocal的核心原理。
<a name="EOVP5"></a>
## 3、ThreadLocal的原理
<a name="w5Qjd"></a>
### 3.1 ThreadLocal的内存结构图
为了有个宏观的认识，先来看下ThreadLocal的内存结构图<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659314623616-5ef25c0b-d727-41e5-9e3d-dfb80850e3ac.png#clientId=u70d59c9d-dceb-4&from=paste&id=uff97cbf6&originHeight=876&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ubb04fd56-34cd-455c-bdc6-a6b05242ebc&title=)<br />从内存结构图，可以看到：

- **Thread类中，有个**`**ThreadLocal.ThreadLocalMap**`**  的成员变量。**
- **ThreadLocalMap内部维护了Entry数组，每个Entry代表一个完整的对象，key是ThreadLocal本身，value是ThreadLocal的泛型对象值。**
<a name="aXayx"></a>
### 3.2 关键源码分析
对照着几段关键源码来看，更容易理解一点~回到Thread类源码，可以看到成员变量`ThreadLocalMap`的初始值是为null
```java
public class Thread implements Runnable {
	//ThreadLocal.ThreadLocalMap是Thread的属性
	ThreadLocal.ThreadLocalMap threadLocals = null;
}
```
ThreadLocalMap的关键源码如下：
```java
static class ThreadLocalMap {

	static class Entry extends WeakReference<ThreadLocal<?>> {
		/** The value associated with this ThreadLocal. */
		Object value;

		Entry(ThreadLocal<?> k, Object v) {
			super(k);
			value = v;
		}
	}
	//Entry数组
	private Entry[] table;

	// ThreadLocalMap的构造器，ThreadLocal作为key
	ThreadLocalMap(ThreadLocal<?> firstKey, Object firstValue) {
		table = new Entry[INITIAL_CAPACITY];
		int i = firstKey.threadLocalHashCode & (INITIAL_CAPACITY - 1);
		table[i] = new Entry(firstKey, firstValue);
		size = 1;
		setThreshold(INITIAL_CAPACITY);
	}
}
```
ThreadLocal类中的关键`set()`方法：
```java
public void set(T value) {
	Thread t = Thread.currentThread(); //获取当前线程t
	ThreadLocalMap map = getMap(t);  //根据当前线程获取到ThreadLocalMap
	if (map != null)  //如果获取的ThreadLocalMap对象不为空
		map.set(this, value); //K，V设置到ThreadLocalMap中
	else
		createMap(t, value); //创建一个新的ThreadLocalMap
}

ThreadLocalMap getMap(Thread t) {
	return t.threadLocals; //返回Thread对象的ThreadLocalMap属性
}

void createMap(Thread t, T firstValue) { //调用ThreadLocalMap的构造函数
	t.threadLocals = new ThreadLocalMap(this, firstValue); this表示当前类ThreadLocal
}
```
ThreadLocal类中的关键`get()`方法
```java
public T get() {
	Thread t = Thread.currentThread();//获取当前线程t
	ThreadLocalMap map = getMap(t);//根据当前线程获取到ThreadLocalMap
	if (map != null) { //如果获取的ThreadLocalMap对象不为空
		//由this（即ThreadLoca对象）得到对应的Value，即ThreadLocal的泛型值
		ThreadLocalMap.Entry e = map.getEntry(this);
		if (e != null) {
			@SuppressWarnings("unchecked")
			T result = (T)e.value; 
			return result;
		}
	}
	return setInitialValue(); //初始化threadLocals成员变量的值
}

private T setInitialValue() {
	T value = initialValue(); //初始化value的值
	Thread t = Thread.currentThread(); 
	ThreadLocalMap map = getMap(t); //以当前线程为key，获取threadLocals成员变量，它是一个ThreadLocalMap
	if (map != null)
		map.set(this, value);  //K，V设置到ThreadLocalMap中
	else
		createMap(t, value); //实例化threadLocals成员变量
	return value;
}
```
所以怎么回答**ThreadLocal的实现原理**？如下，最好是能结合以上结构图一起说明~

- **Thread线程类有一个类型为ThreadLocal.ThreadLocalMap的实例变量threadLocals，即每个线程都有一个属于自己的ThreadLocalMap。**
- **ThreadLocalMap内部维护着Entry数组，每个Entry代表一个完整的对象，key是ThreadLocal本身，value是ThreadLocal的泛型值。**
- **并发多线程场景下，每个线程Thread，在往ThreadLocal里设置值的时候，都是往自己的ThreadLocalMap里存，读也是以某个ThreadLocal作为引用，在自己的map里找对应的key，从而可以实现了线程隔离。**

了解完这几个核心方法后，有些小伙伴可能会有疑惑，ThreadLocalMap为什么要用ThreadLocal作为key呢？直接用线程Id不一样嘛？
<a name="htHYi"></a>
## 4、为什么不直接用线程id作为ThreadLocalMap的key呢？
举个代码例子，如下：
```java
public class TianLuoThreadLocalTest {

	private static final ThreadLocal<String> threadLocal1 = new ThreadLocal<>();
	private static final ThreadLocal<String> threadLocal2 = new ThreadLocal<>();

}
```
这种场景：一个使用类，有两个共享变量，也就是说用了两个ThreadLocal成员变量的话。如果用线程id作为ThreadLocalMap的key，怎么区分哪个ThreadLocal成员变量呢？因此还是需要使用ThreadLocal作为Key来使用。每个ThreadLocal对象，都可以由threadLocalHashCode属性**唯一区分**的，每一个ThreadLocal对象都可以由这个对象的名字唯一区分（**下面的例子**）。看下ThreadLocal代码：
```java
public class ThreadLocal<T> {
	private final int threadLocalHashCode = nextHashCode();

	private static int nextHashCode() {
		return nextHashCode.getAndAdd(HASH_INCREMENT);
	}
}
```
然后再来看下一个代码例子：
```java
public class TianLuoThreadLocalTest {

    public static void main(String[] args) {
        Thread t = new Thread(new Runnable(){
            public void run(){
                ThreadLocal<TianLuoDTO> threadLocal1 = new ThreadLocal<>();
                threadLocal1.set(new TianLuoDTO("Hello World"));
                System.out.println(threadLocal1.get());
                ThreadLocal<TianLuoDTO> threadLocal2 = new ThreadLocal<>();
                threadLocal2.set(new TianLuoDTO("Hi World"));
                System.out.println(threadLocal2.get());
            }});
        t.start();
    }

}
//运行结果
TianLuoDTO{name='Hello World'}
TianLuoDTO{name='Hi World'}
```
再对比下这个图，可能就更清晰一点啦：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659314623566-479db9a6-dfc1-4758-9c69-7cfd21d926f1.png#clientId=u70d59c9d-dceb-4&from=paste&id=uae0b43b0&originHeight=949&originWidth=886&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u508f9afc-c6f2-47cb-9e8b-203633f1c61&title=)
<a name="eKIjo"></a>
## 5、TreadLocal为什么会导致内存泄漏呢？
<a name="tvuT1"></a>
### 5.1 弱引用导致的内存泄漏呢？
先来看看TreadLocal的引用示意图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659314624265-c81380de-c8a8-4441-b5da-47f24f18f82b.png#clientId=u70d59c9d-dceb-4&from=paste&id=u309db2f8&originHeight=589&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue44a77db-fd27-4224-a691-529a4244ad0&title=)<br />关于ThreadLocal内存泄漏，网上比较流行的说法是这样的：<br />ThreadLocalMap使用ThreadLocal的**弱引用**作为key，当ThreadLocal变量被手动设置为null，即一个ThreadLocal没有外部强引用来引用它，当系统GC时，ThreadLocal一定会被回收。这样的话，`ThreadLocalMap`中就会出现key为null的Entry，就没有办法访问这些key为null的Entry的value，如果当前线程再迟迟不结束的话(比如线程池的核心线程)，这些key为null的Entry的value就会一直存在一条强引用链：Thread变量 -> Thread对象 -> ThreaLocalMap -> Entry -> value -> Object 永远无法回收，造成内存泄漏。<br />当ThreadLocal变量被手动设置为null后的引用链图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659314624104-3da0a85c-967e-49df-b934-1566dc15d2cf.png#clientId=u70d59c9d-dceb-4&from=paste&id=ue2032815&originHeight=570&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1e801916-99bc-4341-b587-b4b225210de&title=)<br />实际上，`ThreadLocalMap`的设计中已经考虑到这种情况。所以也加上了一些防护措施：即在`ThreadLocal`的get,set,remove方法，都会清除线程`ThreadLocalMap`里所有key为null的value。<br />源代码中，是有体现的，如`ThreadLocalMap`的set方法：
```java
private void set(ThreadLocal<?> key, Object value) {

	Entry[] tab = table;
	int len = tab.length;
	int i = key.threadLocalHashCode & (len-1);

	for (Entry e = tab[i];
		 e != null;
		 e = tab[i = nextIndex(i, len)]) {
		ThreadLocal<?> k = e.get();

		if (k == key) {
			e.value = value;
			return;
		}

		//如果k等于null,则说明该索引位之前放的key(threadLocal对象)被回收了,这通常是因为外部将threadLocal变量置为null,
		//又因为entry对threadLocal持有的是弱引用,一轮GC过后,对象被回收。
		//这种情况下,既然用户代码都已经将threadLocal置为null,那么也就没打算再通过该对象作为key去取到之前放入threadLocalMap的value, 因此ThreadLocalMap中会直接替换调这种不新鲜的entry。
		if (k == null) {
			replaceStaleEntry(key, value, i);
			return;
		}
	}

	tab[i] = new Entry(key, value);
	int sz = ++size;
	//触发一次Log2(N)复杂度的扫描,目的是清除过期Entry  
	if (!cleanSomeSlots(i, sz) && sz >= threshold)
		rehash();
}
```
如ThreadLocal的`get`方法：
```java
public T get() {
	Thread t = Thread.currentThread();
	ThreadLocalMap map = getMap(t);
	if (map != null) {
		//去ThreadLocalMap获取Entry，方法里面有key==null的清除逻辑
		ThreadLocalMap.Entry e = map.getEntry(this);
		if (e != null) {
			@SuppressWarnings("unchecked")
			T result = (T)e.value;
			return result;
		}
	}
	return setInitialValue();
}

private Entry getEntry(ThreadLocal<?> key) {
	int i = key.threadLocalHashCode & (table.length - 1);
	Entry e = table[i];
	if (e != null && e.get() == key)
		return e;
	else
		//里面有key==null的清除逻辑
		return getEntryAfterMiss(key, i, e);
}

private Entry getEntryAfterMiss(ThreadLocal<?> key, int i, Entry e) {
	Entry[] tab = table;
	int len = tab.length;

	while (e != null) {
		ThreadLocal<?> k = e.get();
		if (k == key)
			return e;
		// Entry的key为null,则表明没有外部引用,且被GC回收,是一个过期Entry
		if (k == null)
			expungeStaleEntry(i); //删除过期的Entry
		else
			i = nextIndex(i, len);
		e = tab[i];
	}
	return null;
}
```
<a name="Ff5PZ"></a>
### 5.2 key是弱引用，GC回收会影响ThreadLocal的正常工作嘛？
到这里，有些小伙伴可能有疑问，ThreadLocal的key既然是**弱引用**.会不会GC贸然把key回收掉，进而影响ThreadLocal的正常使用？

- **弱引用:具有弱引用的对象拥有更短暂的生命周期。如果一个对象只有弱引用存在了，则下次GC将会回收掉该对象（不管当前内存空间足够与否）**

其实不会的，因为有ThreadLocal变量引用着它，是不会被GC回收的，除非手动把ThreadLocal变量设置为null，可以跑个demo来验证一下：
```java
public class WeakReferenceTest {
	public static void main(String[] args) {
		Object object = new Object();
		WeakReference<Object> testWeakReference = new WeakReference<>(object);
		System.out.println("GC回收之前，弱引用："+testWeakReference.get());
		//触发系统垃圾回收
		System.gc();
		System.out.println("GC回收之后，弱引用："+testWeakReference.get());
		//手动设置为object对象为null
		object=null;
		System.gc();
		System.out.println("对象object设置为null，GC回收之后，弱引用："+testWeakReference.get());
	}
}
运行结果：
GC回收之前，弱引用：java.lang.Object@7b23ec81
GC回收之后，弱引用：java.lang.Object@7b23ec81
对象object设置为null，GC回收之后，弱引用：null
```
结论就是，小伙伴放下这个疑惑了~
<a name="ZlDM0"></a>
### 5.3 ThreadLocal内存泄漏的demo
给大家来看下一个内存泄漏的例子，其实就是用线程池，一直往里面放对象
```java
public class ThreadLocalTestDemo {

    private static ThreadLocal<TianLuoClass> tianLuoThreadLocal = new ThreadLocal<>();


    public static void main(String[] args) throws InterruptedException {

        ThreadPoolExecutor threadPoolExecutor = new ThreadPoolExecutor(5, 5, 1, TimeUnit.MINUTES, new LinkedBlockingQueue<>());

        for (int i = 0; i < 10; ++i) {
            threadPoolExecutor.execute(new Runnable() {
                @Override
                public void run() {
                    System.out.println("创建对象：");
                    TianLuoClass tianLuoClass = new TianLuoClass();
                    tianLuoThreadLocal.set(tianLuoClass);
                    tianLuoClass = null; //将对象设置为 null，表示此对象不在使用了
                   // tianLuoThreadLocal.remove();
                }
            });
            Thread.sleep(1000);
        }
    }

    static class TianLuoClass {
        // 100M
        private byte[] bytes = new byte[100 * 1024 * 1024];
    }
}


创建对象：
创建对象：
创建对象：
创建对象：
Exception in thread "pool-1-thread-4" java.lang.OutOfMemoryError: Java heap space
 at com.example.dto.ThreadLocalTestDemo$TianLuoClass.<init>(ThreadLocalTestDemo.java:33)
 at com.example.dto.ThreadLocalTestDemo$1.run(ThreadLocalTestDemo.java:21)
 at java.util.concurrent.ThreadPoolExecutor.runWorker(ThreadPoolExecutor.java:1149)
 at java.util.concurrent.ThreadPoolExecutor$Worker.run(ThreadPoolExecutor.java:624)
 at java.lang.Thread.run(Thread.java:748)
```
运行结果出现了OOM，`tianLuoThreadLocal.remove();`加上后，则不会OOM。
```java
创建对象：
创建对象：
创建对象：
创建对象：
创建对象：
创建对象：
创建对象：
创建对象：
......
```
这里**没有手动设置**`tianLuoThreadLocal`变量为null，但是还是**会内存泄漏**。因为使用了线程池，线程池有很长的生命周期，因此线程池会一直持有`tianLuoClass`对象的value值，即使设置`tianLuoClass = null;`引用还是存在的。这就好像，把一个个对象object放到一个list列表里，然后再单独把object设置为null的道理是一样的，列表的对象还是存在的。
```java
public static void main(String[] args) {
	List<Object> list = new ArrayList<>();
	Object object = new Object();
	list.add(object);
	object = null;
	System.out.println(list.size());
}
//运行结果
1
```
所以内存泄漏就这样发生啦，最后内存是有限的，就抛出了OOM了。如果加上`threadLocal.remove();`，则不会内存泄漏。为什么呢？因为`threadLocal.remove();`会清除`Entry`，源码如下：
```java
private void remove(ThreadLocal<?> key) {
	Entry[] tab = table;
	int len = tab.length;
	int i = key.threadLocalHashCode & (len-1);
	for (Entry e = tab[i];
		 e != null;
		 e = tab[i = nextIndex(i, len)]) {
		if (e.get() == key) {
			//清除entry
			e.clear();
			expungeStaleEntry(i);
			return;
		}
	}
}
```
有些小伙伴说，既然内存泄漏不一定是因为弱引用，那为什么需要设计为弱引用呢？来探讨下：
<a name="lpAO1"></a>
## 6、Entry的Key为什么要设计成弱引用呢？
通过源码，可以看到`Entry`的Key是设计为弱引用的(`ThreadLocalMap`使用`ThreadLocal`的弱引用作为Key的)。为什么要设计为弱引用呢？<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659314624151-dce968fe-11e9-4956-855d-8ca6ffd1bb14.png#clientId=u70d59c9d-dceb-4&from=paste&id=u722ccfc9&originHeight=312&originWidth=693&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u28ea3c6a-a2bc-42a1-bd93-4fea19acf7c&title=)<br />先来回忆一下四种引用：

- **强引用：平时**`**new**`**了一个对象就是强引用，例如 **`**Object obj = new Object();**`**即使在内存不足的情况下，JVM宁愿抛出OutOfMemory错误也不会回收这种对象。**
- **软引用：如果一个对象只具有软引用，则内存空间足够，垃圾回收器就不会回收它；如果内存空间不足了，就会回收这些对象的内存。**
- **弱引用：具有弱引用的对象拥有更短暂的生命周期。如果一个对象只有弱引用存在了，则下次GC将会回收掉该对象（不管当前内存空间足够与否）。**
- **虚引用：如果一个对象仅持有虚引用，那么它就和没有任何引用一样，在任何时候都可能被垃圾回收器回收。虚引用主要用来跟踪对象被垃圾回收器回收的活动。**

先来看看官方文档，为什么要设计为弱引用：
> To help deal with very large and long-lived usages, the hash table entries use WeakReferences for keys.<br />为了应对非常大和长时间的用途，哈希表使用弱引用的 key。

再把ThreadLocal的引用示意图搬过来：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659314624489-957fc015-7437-4dd2-ae93-4a86b6a876b2.png#clientId=u70d59c9d-dceb-4&from=paste&id=u2158bfdc&originHeight=589&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue296f694-be6f-4c98-86d8-d8e18dda4ca&title=)<br />下面分情况讨论：

- **如果Key使用强引用：当ThreadLocal的对象被回收了，但是ThreadLocalMap还持有ThreadLocal的强引用的话，如果没有手动删除，ThreadLocal就不会被回收，会出现Entry的内存泄漏问题。**
- **如果Key使用弱引用：当ThreadLocal的对象被回收了，因为ThreadLocalMap持有ThreadLocal的弱引用，即使没有手动删除，ThreadLocal也会被回收。value则在下一次ThreadLocalMap调用set,get，remove的时候会被清除。**

因此可以发现，使用弱引用作为Entry的Key，可以多一层保障：弱引用ThreadLocal不会轻易内存泄漏，对应的value在下一次ThreadLocalMap调用set,get,remove的时候会被清除。<br />实际上，内存泄漏的根本原因是，不再被使用的Entry，没有从线程的ThreadLocalMap中删除。一般删除不再使用的Entry有这两种方式：

- **一种就是，使用完ThreadLocal，手动调用remove()，把Entry从ThreadLocalMap中删除**
- **另外一种方式就是：ThreadLocalMap的自动清除机制去清除过期Entry.（ThreadLocalMap的get(),set()时都会触发对过期Entry的清除）**
<a name="RwGbT"></a>
## 7、`InheritableThreadLocal`保证父子线程间的共享数据
ThreadLocal是线程隔离的，如果希望父子线程共享数据，如何做到呢？可以使用`InheritableThreadLocal`。先来看看demo：
```java
public class InheritableThreadLocalTest {

   public static void main(String[] args) {
       ThreadLocal<String> threadLocal = new ThreadLocal<>();
       InheritableThreadLocal<String> inheritableThreadLocal = new InheritableThreadLocal<>();

       threadLocal.set("Hello World");
       inheritableThreadLocal.set("Hello");

       Thread thread = new Thread(()->{
           System.out.println("ThreadLocal value " + threadLocal.get());
           System.out.println("InheritableThreadLocal value " + inheritableThreadLocal.get());
       });
       thread.start();
       
   }
}
//运行结果
ThreadLocal value null
InheritableThreadLocal value Hello
```
可以发现，在子线程中，是可以获取到父线程的 `InheritableThreadLocal `类型变量的值，但是不能获取到 ThreadLocal 类型变量的值。<br />获取不到ThreadLocal 类型的值，很好理解，因为它是线程隔离的嘛。InheritableThreadLocal 是如何做到的呢？原理是什么呢？<br />在Thread类中，除了成员变量threadLocals之外，还有另一个成员变量：inheritableThreadLocals。它们两类型是一样的：
```java
public class Thread implements Runnable {
	ThreadLocalMap threadLocals = null;
	ThreadLocalMap inheritableThreadLocals = null;
}
```
Thread类的init方法中，有一段初始化设置：
```java
private void init(ThreadGroup g, Runnable target, String name,
				  long stackSize, AccessControlContext acc,
				  boolean inheritThreadLocals) {

	......
		if (inheritThreadLocals && parent.inheritableThreadLocals != null)
			this.inheritableThreadLocals =
			ThreadLocal.createInheritedMap(parent.inheritableThreadLocals);
	/* Stash the specified stack size in case the VM cares */
	this.stackSize = stackSize;

	/* Set thread ID */
	tid = nextThreadID();
}
static ThreadLocalMap createInheritedMap(ThreadLocalMap parentMap) {
	return new ThreadLocalMap(parentMap);
}
```
可以发现，当parent的`inheritableThreadLocals`不为null时，就会将parent的`inheritableThreadLocals`，赋值给前线程的`inheritableThreadLocals`。说白了，就是如果当前线程的`inheritableThreadLocals`不为null，就从父线程哪里拷贝过来一个过来，类似于另外一个ThreadLocal，数据从父线程那里来的。有兴趣的小伙伴们可以在去研究研究源码~
<a name="AxzAf"></a>
## 8、ThreadLocal的应用场景和使用注意点
ThreadLocal的**很重要一个注意点**，就是使用完，要手动调用`remove()`。<br />而ThreadLocal的应用场景主要有以下这几种：

- **使用日期工具类，当用到**`**SimpleDateFormat**`**，使用ThreadLocal保证线性安全**
- **全局存储用户信息（用户信息存入**`**ThreadLocal**`**，那么当前线程在任何地方需要时，都可以使用）**
- **保证同一个线程，获取的数据库连接Connection是同一个，使用ThreadLocal来解决线程安全的问题**
- **使用MDC保存日志信息。**
