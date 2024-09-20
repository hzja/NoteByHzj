JavaThreadLocal
<a name="jMSoA"></a>
## 前言
当多线程访问共享且可变的数据时，涉及到线程间同步的问题，并不是所有时候，都要用到共享数据，所以就需要ThreadLocal出场了。<br />ThreadLocal又称线程本地变量，使用其能够将数据封闭在各自的线程中，每一个ThreadLocal能够存放一个线程级别的变量且它本身能够被多个线程共享使用，并且又能达到线程安全的目的，且绝对线程安全，其用法如下所示：
```java
public final static ThreadLocal<String> RESOURCE = new ThreadLocal<String>();
```
RESOURCE代表一个能够存放String类型的ThreadLocal对象。此时不论什么一个线程能够并发访问这个变量，对它进行写入、读取操作，都是线程安全的。<br />除了线程安全之外，使用ThreadLocal也能够作为一种“方便传参”的工具，在业务逻辑冗长的代码中，同一个参数需要传入在多个方法之间层层传递，当这种需要传递的参数过多时代码会显得十分臃肿、丑陋；<br />之前做过企微会话存档的功能，就是将企业微信聊天信息拉取下来保存，由于企业微信消息类型很多（至少有三十多种），为了后期便于维护在对消息解析、保存时根据消息类型封分别封装了对应的方法每个消息类型解析、保存时又会进一步细分拆分成多个方法（比如说文件资源的分片拉取、上传到静态资源服务器），这个时候麻烦的事情就来了，每个方法的入参都需要包含企微会话存档的相关配置参数和封装的对话信息参数，导致入参列表非常长，阅读性比较差。<br />实际上可以把企微会话存档的相关配置参数存入到ThreadLocal中，各个方法内需要使用直接从ThreadLocal中获取就可以了，以后有时间了要把这块代码重构一下。<br />后来又做了公司的短信模块的需求，主要是记录短信发送记录、发送统计及短信发送状态，短信发送的接口有多个（单条发送、批量发送、根据模板发送等等），需要记录多个接口的调用情况，当时就抽象出了短信上下文、模板上下文等实体，在调用方法时首先构造对应的上下文并将其保存到ThreadLocal中，在短信余额校验、违禁词过滤、余额不足提醒等业务处理方法中只需要从ThreadLocal中取出对应的上下文即可，而且发送状态是通过切面进行记录的，在切入点记录日志时也是直接从ThreadLocal中直接获取的上下文信息，代码简洁、可读性高。
<a name="TiHfM"></a>
## 原理
先看一下ThreadLocal的结构<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651752198190-d8a19a7d-52d9-408b-a10b-c6a35212742d.png#clientId=ueb48a7ef-75a8-4&from=paste&id=u9a1a48a9&originHeight=602&originWidth=618&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u58a3f77d-1b2f-4f4e-9a92-2addf35702c&title=)<br />需要重点关注的方法有：
<a name="bp2zA"></a>
### set
```java
public void set(T value) {
	Thread t = Thread.currentThread();
	ThreadLocalMap map = getMap(t);
	if (map != null)
		map.set(this, value);
	else
		createMap(t, value);
}
```
如果能够搞懂这块代码，就能够明白ThreadLocal到底是怎么实现的了。这块代码其实很有意思，在向ThreadLocal中存放值时需要先从当前线程中获取ThreadLocalMap，最后实际是要把当前ThreadLocal对象作为key、要存入的值作为value存放到ThreadLocalMap中，那就不得不先看一下ThreadLocalMap的结构。
```java
static class ThreadLocalMap {
    /**
     * 键值对实体的存储结构
     */
    static class Entry extends WeakReference<ThreadLocal<?>> {
        // 当前线程关联的 value，这个 value 并没有用弱引用追踪
        Object value;

        /**
         * 构造键值对
         *
         * @param k k 作 key,作为 key 的 ThreadLocal 会被包装为一个弱引用
         * @param v v 作 value
         */
        Entry(ThreadLocal<?> k, Object v) {
            super(k);
            value = v;
        }
    }

    // 初始容量，必须为 2 的幂
    private static final int INITIAL_CAPACITY = 16;

    // 存储 ThreadLocal 的键值对实体数组，长度必须为 2 的幂
    private Entry[] table;

    // ThreadLocalMap 元素数量
    private int size = 0;

    // 扩容的阈值，默认是数组大小的三分之二
    private int threshold;
}
```
ThreadLocalMap 是 ThreadLocal 的静态内部类，当一个线程有多个 ThreadLocal 时，需要一个容器来管理多个 ThreadLocal，ThreadLocalMap 的作用就是管理线程中多个 ThreadLocal，从源码中看到 ThreadLocalMap 其实就是一个简单的 Map 结构，底层是数组，有初始化大小，也有扩容阈值大小，数组的元素是 Entry，Entry 的 key 就是 ThreadLocal 的引用，value 是 ThreadLocal内存入 的值。ThreadLocalMap 解决 hash 冲突的方式采用的是「线性探测法」，如果发生冲突会继续寻找下一个空的位置。<br />每个Thread内部都持有一个ThreadLoalMap对象
```java
 /* ThreadLocal values pertaining to this thread. This map is maintained
     * by the ThreadLocal class. */
    ThreadLocal.ThreadLocalMap threadLocals = null;
```
至此，都能够明白ThreadLocal存值的过程了，虽然是按照前言中的用法声明了一个全局常量，但是这个常量在每次设置时实际都是向当前线程的ThreadLocalMap内存值，从而确保了数据在不同线程之间的隔离。
<a name="m9QZi"></a>
### get
```java
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
有了上面的铺垫，这段代码就不难理解了，获取ThreadLocal内的值时，实际上是从当前线程的`ThreadLocalMap`中以当前ThreadLocal对象作为key取出对应的值，由于值在保存时时线程隔离的，所以现在取值时只会取得当前线程中的值，所以是绝对线程安全的。
<a name="ehMvd"></a>
### remove
```java
private void remove(ThreadLocal<?> key) {
	Entry[] tab = table;
	int len = tab.length;
	int i = key.threadLocalHashCode & (len-1);
	for (Entry e = tab[i];
		 e != null;
		 e = tab[i = nextIndex(i, len)]) {
		if (e.get() == key) {
			e.clear();
			expungeStaleEntry(i);
			return;
		}
	}
}
```
`remove`将ThreadLocal对象关联的键值对从Entry中移除，正确执行remove方法能够避免使用ThreadLocal出现内存泄漏的潜在风险，`int i = key.threadLocalHashCode & (len-1)`这行代码很有意思，从一个集合中找到一个元素存放位置的最简单方法就是利用该元素的hashcode对这个集合的长度取余，如果能够将集合的长度限制成2的整数次幂就能够将取余运算转换成hashcode与[集合长度-1]的与运算，这样就能够提高查找效率，HashMap中也是这样处理的，这里就不再展开了。<br />下面的一张图很好的解释了ThreadLocal的原理<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651752198209-3e09c331-ee4e-4257-bfbf-8585d17b0465.png#clientId=ueb48a7ef-75a8-4&from=paste&id=ued4e02d7&originHeight=578&originWidth=701&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf49e842f-1c39-4f88-9755-b4488a91aba&title=)
<a name="svYxH"></a>
## ThreadLocal内存泄漏及正确用法
在提及ThreadLocal使用的注意事项时，所有的文章都会指出内存泄漏这一风险，由于ThreadLocalMap中的Entry的key持有的是ThreadLocal对象的弱引用，当这个ThreadLocal对象当且仅当被ThreadLocalMap中的Entry引用时发生了GC，会导致当前ThreadLocal对象被回收；那么 ThreadLocalMap 中保存的 key 值就变成了 null，而Entry 又被 ThreadLocalMap 对象引用，ThreadLocalMap 对象又被 Thread 对象所引用，那么当 Thread 一直不销毁的话，value 对象就会一直存在于内存中，也就导致了内存泄漏，直至 Thread 被销毁后，才会被回收。<br />下面就来验证一下这个情景，在方法内部声明了一个ThreadLocal对象，为了更好的演示内存泄漏的情景在使用这个对象存值后将方法内取消对其的强引用，并且通过`System.gc()`触发了一次垃圾回收（准确的说是希望jvm执行一次垃圾回收，不能保证垃圾回收一定会进行，而且具体什么时候进行是取决于具体的虚拟机的），这样再垃圾回收时会将ThreadLocal对象回收，代码如下所示：
```java
@Test
public void loop() throws Exception {

	for (int i = 0; i < 1; i++) {
		ThreadLocal<SysUser> threadLocal = new ThreadLocal<>();
		threadLocal.set(new SysUser(System.currentTimeMillis(), "李四"));
		// threadLocal = null;
		//System.gc();
		printEntryInfo();
	}

	//System.gc();

	//printEntryInfo();
}

private void printEntryInfo() throws Exception {
	Thread currentThread = Thread.currentThread();
	Class<? extends Thread> clz = currentThread.getClass();
	Field field = clz.getDeclaredField("threadLocals");
	field.setAccessible(true);
	Object threadLocalMap = field.get(currentThread);
	Class<?> tlmClass = threadLocalMap.getClass();
	Field tableField = tlmClass.getDeclaredField("table");
	tableField.setAccessible(true);
	Object[] arr = (Object[]) tableField.get(threadLocalMap);
	for (Object o : arr) {
		if (o != null) {
			Class<?> entryClass = o.getClass();
			Field valueField = entryClass.getDeclaredField("value");
			Field referenceField = entryClass.getSuperclass().getSuperclass().getDeclaredField("referent");
			valueField.setAccessible(true);
			referenceField.setAccessible(true);
			System.out.println(String.format("弱引用key:%s,值:%s", referenceField.get(o), valueField.get(o)));
		}
	}
}
```
在不发生GC时，控制台输出如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651752198232-65e185f4-c4ed-40b4-95fe-d15e3bb1db83.png#clientId=ueb48a7ef-75a8-4&from=paste&id=u08132f85&originHeight=269&originWidth=996&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf4e03093-4e8e-419e-a3ba-bf93f835c5a&title=)<br />ThreadLocal对象并未被回收，将`System.gc();`放开，控制台输入如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651752198262-ff5186c5-b91b-4f4d-b3fd-10a21eadd2c1.png#clientId=ueb48a7ef-75a8-4&from=paste&id=u6ed72b8b&originHeight=274&originWidth=1004&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u92387b99-21c8-4d2a-959b-a2d4eb9c0fe&title=)<br />可以看出key确实变成了null值，而Entry内会一直持有对value的引用，导致value无法被回收，如果当前线程一直在执行未被销毁，则确实会出现内存泄漏（在使用线程池时更容易出现这样的问题）。分析一下上面的为什么会出现内存泄漏的原因，在上面的代码里，在方法内部声明了一个ThreadLocal对象，该ThreadLocal对象仅有一个方法内部的强引用且的生命周期很短，当该方法执行完成之后此ThreadLocal对象在下一次gc时就会被回收，当然可以在方法结束前手动执行一个该对象的`remove`方法，但是这样就失去了使用ThreadLocal的意义。<br />由此，出现内存泄漏的原因是失去了对ThreadLocal对象的强引用，避免内存泄漏最简单的方法就是始终保持对ThreadLocal对象的强引用，为每个线程声明一个对ThreadLocal对象的强引用显然是不合适的（太麻烦且缺乏声明的时机），所以，可以将ThreadLocal对象声明为一个全局常量，所有的线程均使用这一常量即可，例如：
```java
private static final ThreadLocal<String> RESOURCE = new ThreadLocal<>();

@Test
public void multiThread() {
	Thread thread1 = new Thread(() -> {
		RESOURCE.set("thread1");
		System.gc();
		try {
			printEntryInfo();
		} catch (Exception e) {
			e.printStackTrace();
		}
	});
	Thread thread2 = new Thread(() -> {
		RESOURCE.set("thread2");
		System.gc();
		try {
			printEntryInfo();
		} catch (Exception e) {
			e.printStackTrace();
		}
	});
	thread1.start();
	thread2.start();
}
```
按照上面的方式声明ThreadLocal对象后，所有的线程共用此对象，在使用此对象存值时会把此对象作为key然后把对应的值作为value存入到当前线程的`ThreadLocalMap`中，由于此对象始终存在着一个全局的强引用，所以其不会被垃圾回收，调用remove方法后就能够将此对象关联的Entry清除。<br />验证一下：
```java
弱引用key:java.lang.ThreadLocal@10c6d8a7,值:thread1
弱引用key:java.lang.ThreadLocal@10c6d8a7,值:thread2
```
可以看出两个线程内对应的Entry的key为同一个对象且即使发生了垃圾回收该对象也不会被回收。<br />那么是不是说将ThreadLocal对象声明为一个全局常量后使用就没有问题了呢，当然不是，需要确保在每次使用完ThreadLocal对象后确保要执行一下该对象的remove方法，清除当前线程保存的信息，这样当此线程再被利用时不会取到错误的信息（使用线程池极易出现）；<br />项目之前就出现过这种场景，从线程池中获取线程，并在每次请求时在当前线程记录下对应的用户信息，结果有一天出现了串号的问题，B用户访问时使用了A用户的信息，这就是在每次请求结束后没有执行`remove`方法，线程复用时内部还保存着上一个用户的信息，贴上一份使用ThreadLocal的正确姿势：
```java
package com.cube.share.thread.config;

import com.cube.share.thread.entity.SysUser;

/**
 * 线程当前用户信息
 */
public class CurrentUser {

    private static final ThreadLocal<SysUser> USER = new ThreadLocal<>();

    private static final ThreadLocal<Long> USER_ID = new ThreadLocal<>();

    private static final InheritableThreadLocal<SysUser> INHERITABLE_USER = new InheritableThreadLocal<>();

    private static final InheritableThreadLocal<Long> INHERITABLE_USER_ID = new InheritableThreadLocal<>();

    public static void setUser(SysUser sysUser) {
        USER.set(sysUser);
        INHERITABLE_USER.set(sysUser);
    }

    public static void setUserId(Long id) {
        USER_ID.set(id);
        INHERITABLE_USER_ID.set(id);
    }

    public static SysUser user() {
        return USER.get();
    }

    public static SysUser inheritableUser() {
        return INHERITABLE_USER.get();
    }

    public static Long inheritableUserId() {
        return INHERITABLE_USER_ID.get();
    }

    public static Long userId() {
        return USER_ID.get();
    }

    public static void removeAll() {
        USER.remove();
        USER_ID.remove();
        INHERITABLE_USER.remove();
        INHERITABLE_USER_ID.remove();
    }
}
```
可以通过切面或者请求监听器在请求结束时将当前线程保存的ThreadLocal信息清除
```java
/**
 * ServletRequest请求监听器
 */
@Component
@Slf4j
public class ServletRequestHandledEventListener implements ApplicationListener<ServletRequestHandledEvent> {

    @Override
    public void onApplicationEvent(ServletRequestHandledEvent event) {
        CurrentUser.removeAll();
        log.debug("清除当前线程用户信息,uri = {},method = {},servletName = {},clientAddress = {}", event.getRequestUrl(),
                event.getMethod(), event.getServletName(), event.getClientAddress());
    }
}
```
<a name="TMzRN"></a>
## 可传递给子线程的`InheritableThreadLocal`
如果在当前线程中开辟新的子线程并希望子线程获取父线程保存的线程本地变量要怎么做呢，在子线程中声明ThreadLocal对象并将父线程中对应的值存入自然是可以的，但是大可不必如此繁琐，jdk已经提供了一种可传递给子线程的`InheritableThreadLocal`，实现的原理也很简单，可以在Thread中一窥究竟。
```java
//持有了一个可传递给子线程的ThreadLocalMap
ThreadLocal.ThreadLocalMap inheritableThreadLocals = null;

//线程创建时都会执行这个初始化方法，inheritThreadLocals表示是否需要在构造时从父线程中继承thread-locals，默认为true
private void init(ThreadGroup g, Runnable target, String name,
				  long stackSize, AccessControlContext acc,
				  boolean inheritThreadLocals) {
	//忽略了一部分代码

	setPriority(priority);
	//从父线程中继承thread-locals
	if (inheritThreadLocals && parent.inheritableThreadLocals != null)
		this.inheritableThreadLocals =
		ThreadLocal.createInheritedMap(parent.inheritableThreadLocals);
	/* Stash the specified stack size in case the VM cares */
	this.stackSize = stackSize;

	/* Set thread ID */
	tid = nextThreadID();
}
```
<a name="kofQw"></a>
## 使用场景
ThreadLocal 的特性也导致了应用场景比较广泛，主要的应用场景如下：

- 线程间数据隔离，各线程的 ThreadLocal 互不影响
- 方便同一个线程使用某一对象，避免不必要的参数传递
- 全链路追踪中的 traceId 或者流程引擎中上下文的传递一般采用 ThreadLocal
- Spring 事务管理器采用了 ThreadLocal
- Spring MVC 的 `RequestContextHolder` 的实现使用了 ThreadLocal
<a name="P2TnC"></a>
## 总结
主要从源码的角度解析了 ThreadLocal，并分析了发生内存泄漏的原因及正确用法，最后对它的应用场景进行了简单介绍。ThreadLocal还有其他变种例如`FastThreadLocal`和`TransmittableThreadLocal`，`FastThreadLocal`主要解决了伪共享的问题比ThreadLocal拥有更好的性能，`TransmittableThreadLocal`主要解决了线程池中线程复用导致后续提交的任务并不会继承到父线程的线程变量的问题。
