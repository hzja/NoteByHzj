Java<br />分布式链路追踪时异步调用会丢失链路信息，最终的解决方案是使用对应的包装类重新包装一下，如下：

- `**RunnableWrapper**`
- `**CallableWrapper**`
- `**SupplierWrapper**`

还有openFeign异步请求丢失上文的问题，这些问题追根究底都是`**ThreadLocal**`惹得祸。<br />由于ThreadLocal只能保存当前线程的信息，不能实现父子线程的继承。<br />说到这，很多人想到了`**InheritableThreadLocal**`，确实InheritableThreadLocal能够实现父子线程间传递本地变量，但是.....<br />但是程序如果采用线程池，则存在着线程复用的情况，这时就不一定能够实现父子线程间传递了，因为在线程在线程池中的存在不是每次使用都会进行创建，`InheritableThreadlocal`是在线程初始化时`intertableThreadLocals=true`才会进行拷贝传递。<br />所以若本次使用的子线程是已经被池化的线程，从线程池中取出线下进行使用，是没有经过初始化的过程，也就不会进行父子线程的本地变量拷贝。<br />由于在日常应用场景中，绝大多数都是会采用线程池的方式进行资源的有效管理。<br />来看看阿里的`**ThansmittableThreadLocal**`是如何解决线程池中父子线程本地变量传递。
<a name="U5phx"></a>
## `InheritableThreadLocal `的问题
在介绍`ThansmittableThreadLocal`之前先来看一下`InheritableThreadLocal `在线程池中的问题，如下代码：
```java
@Test
public void test() throws Exception {
	//单一线程池
	ExecutorService executorService = Executors.newSingleThreadExecutor();
	//InheritableThreadLocal存储
	InheritableThreadLocal<String> username = new InheritableThreadLocal<>();
	for (int i = 0; i < 10; i++) {
		username.set("Fcant—"+i);
		Thread.sleep(3000);
		CompletableFuture.runAsync(()-> System.out.println(username.get()),executorService);
	}
}
```
上述代码中创建了一个单一线程池，循环异步调用，打印一下username，由于核心线程数是1，势必存在线程的复用。<br />打印信息如下：
```xml
Fcant—0
Fcant—0
Fcant—0
Fcant—0
Fcant—0
Fcant—0
Fcant—0
Fcant—0
Fcant—0
Fcant—0
```
看到了吗？这里并没有实现父子线程间的变量传递，这也就是InheritableThreadLocal 的局限性。
<a name="Wvvwu"></a>
## `TransmittableThreadLocal` 使用
`TransmittableThreadLocal`(TTL)：在使用线程池等会池化复用线程的执行组件情况下，提供`ThreadLocal`值的传递功能，解决异步执行时上下文传递的问题。<br />整个`TransmittableThreadLocal`库的核心功能（用户API与框架/中间件的集成API、线程池`ExecutorService`/`ForkJoinPool`/`TimerTask`及其线程工厂的`Wrapper`）。<br />**需求场景**：

1. 分布式跟踪系统 或 全链路压测（即链路打标）
2. 日志收集记录系统上下文

官网地址：[https://github.com/alibaba/transmittable-thread-local](https://github.com/alibaba/transmittable-thread-local)<br />下面就以上面的例子改造成`**TransmittableThreadLocal**`试一下效果。<br />首选需要引入对应的依赖，如下：
```xml
<dependency>
	<groupId>com.alibaba</groupId>
	<artifactId>transmittable-thread-local</artifactId>
</dependency
```
改造后的代码如下：
```java
@Test
public void test() throws Exception {
	//单一线程池
	ExecutorService executorService = Executors.newSingleThreadExecutor();
	//需要使用TtlExecutors对线程池包装一下
	executorService=TtlExecutors.getTtlExecutorService(executorService);
	//TransmittableThreadLocal创建
	TransmittableThreadLocal<String> username = new TransmittableThreadLocal<>();
	for (int i = 0; i < 10; i++) {
		username.set("Fcant—"+i);
		Thread.sleep(3000);
		CompletableFuture.runAsync(()-> System.out.println(username.get()),executorService);
	}
}
```
需要注意的是需要使用`TtlExecutors`对线程池进行包装，代码如下：
```java
executorService=TtlExecutors.getTtlExecutorService(executorService);
```
运行效果如下：
```java
Fcant—1
Fcant—2
Fcant—3
Fcant—4
Fcant—5
Fcant—6
Fcant—7
Fcant—8
Fcant—9
```
可以看到已经能够实现了线程池中的父子线程的数据传递。<br />在每次调用任务的时，都会将当前的主线程的TTL数据copy到子线程里面，执行完成后，再清除掉。同时子线程里面的修改回到主线程时其实并没有生效。这样可以保证每次任务执行的时候都是互不干涉。
<a name="f3DV4"></a>
## 简单应用
在 Spring Security 往往需要存储用户登录的详细信息，这样在业务方法中能够随时获取用户的信息。<br />在Spring Cloud Gateway整合OAuth2.0实现统一认证鉴权中将用户信息直接存储在Request中，这样每次请求都能获取到对应的信息。<br />其实Request中的信息存储也是通过`ThreadLocal`完成的，在异步执行的时候还是需要重新转存，这样一来代码就变得复杂。<br />那么了解了`**TransmittableThreadLocal** `之后，完全可以使用这个存储用户的登录信息，实现如下：
```java
/**
 * @description 使用TransmittableThreadLocal存储用户身份信息LoginVal
 */
public class SecurityContextHolder {

	//使用TTL存储身份信息
	private static final TransmittableThreadLocal<LoginVal> THREAD_LOCAL = new TransmittableThreadLocal<>();

	public static void set(LoginVal loginVal){
		THREAD_LOCAL.set(loginVal);
	}

	public static LoginVal get(){
		return THREAD_LOCAL.get();
	}

	public static void remove(){
		THREAD_LOCAL.remove();
	}

}
```
由于mvc中的一次请求对应一个线程，因此只需要在拦截器中的设置和移除TransmittableThreadLocal中的信息，代码如下：
```java
/**
 * @description 拦截器，在preHandle中解析请求头的中的token信息，将其放入SecurityContextHolder中
 *                      在afterCompletion方法中移除对应的ThreadLocal中信息
 *                      确保每个请求的用户信息独立
 */
@Component
public class AuthInterceptor implements AsyncHandlerInterceptor {
	/**
     * 在执行controller方法之前将请求头中的token信息解析出来，放入SecurityContextHolder中（TransmittableThreadLocal）
     */
	@Override
	public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler) {
		if (!(handler instanceof HandlerMethod))
			return true;
		//获取请求头中的加密的用户信息
		String token = request.getHeader(OAuthConstant.TOKEN_NAME);
		if (StrUtil.isBlank(token))
			return true;
		//解密
		String json = Base64.decodeStr(token);
		//将json解析成LoginVal
		LoginVal loginVal = TokenUtils.parseJsonToLoginVal(json);
		//封装数据到ThreadLocal中
		SecurityContextHolder.set(loginVal);
		return true;
	}

	/**
     * 在视图渲染之后执行，意味着一次请求结束，清除TTL中的身份信息
     */
	@Override
	public void afterCompletion(HttpServletRequest request, HttpServletResponse response, Object handler, Exception ex){
		SecurityContextHolder.remove();
	}
}
```
<a name="B1Nej"></a>
## 原理
从定义来看，`TransimittableThreadLocal`继承于`InheritableThreadLocal`，并实现`TtlCopier`接口，它里面只有一个`copy`方法。所以主要是对`InheritableThreadLocal`的扩展。
```java
public class TransmittableThreadLocal<T> extends InheritableThreadLocal<T> implements TtlCopier<T> 
```
在`TransimittableThreadLocal`中添加`holder`属性。这个属性的作用就是被标记为具备线程传递资格的对象都会被添加到这个对象中。<br />**要标记一个类，比较容易想到的方式，就是给这个类新增一个Type字段，还有一个方法就是将具备这种类型的的对象都添加到一个静态全局集合中。之后使用时，这个集合里的所有值都具备这个标记。**
```java
// 1. holder本身是一个InheritableThreadLocal对象
// 2. 这个holder对象的value是WeakHashMap<TransmittableThreadLocal<Object>, ?>
//   2.1 WeekHashMap的value总是null,且不可能被使用。
//    2.2 WeekHasshMap支持value=null
private static InheritableThreadLocal<WeakHashMap<TransmittableThreadLocal<Object>, ?>> holder = new InheritableThreadLocal<WeakHashMap<TransmittableThreadLocal<Object>, ?>>() {
	@Override
	protected WeakHashMap<TransmittableThreadLocal<Object>, ?> initialValue() {
		return new WeakHashMap<TransmittableThreadLocal<Object>, Object>();
	}

	/**
   * 重写了childValue方法，实现上直接将父线程的属性作为子线程的本地变量对象。
   */
	@Override
	protected WeakHashMap<TransmittableThreadLocal<Object>, ?> childValue(WeakHashMap<TransmittableThreadLocal<Object>, ?> parentValue) {
		return new WeakHashMap<TransmittableThreadLocal<Object>, Object>(parentValue);
	}
};
```
应用代码是通过`TtlExecutors`工具类对线程池对象进行包装。工具类只是简单的判断，输入的线程池是否已经被包装过、非空校验等，然后返回包装类`ExecutorServiceTtlWrapper`。根据不同的线程池类型，有不同和的包装类。
```java
@Nullable
public static ExecutorService getTtlExecutorService(@Nullable ExecutorService executorService) {
	if (TtlAgent.isTtlAgentLoaded() || executorService == null || executorService instanceof TtlEnhanced) {
		return executorService;
	}
	return new ExecutorServiceTtlWrapper(executorService);
}
```
进入包装类`ExecutorServiceTtlWrapper`。可以注意到不论是通过`ExecutorServiceTtlWrapper#submit`方法或者是`ExecutorTtlWrapper#execute`方法，都会将线程对象包装成`TtlCallable`或者`TtlRunnable`，用于在真正执行`run`方法前做一些业务逻辑。
```java
/**
 * 在ExecutorServiceTtlWrapper实现submit方法
 */
@NonNull
@Override
public <T> Future<T> submit(@NonNull Callable<T> task) {
	return executorService.submit(TtlCallable.get(task));
}

/**
 * 在ExecutorTtlWrapper实现execute方法
 */
@Override
public void execute(@NonNull Runnable command) {
	executor.execute(TtlRunnable.get(command));
}
```
所以，重点的核心逻辑应该是在`TtlCallable#call()`或者`TtlRunnable#run()`中。以下以`TtlCallable`为例，`TtlRunnable`同理类似。在分析`call()`方法之前，先看一个类`Transmitter`
```java
public static class Transmitter {
	/**
    * 捕获当前线程中的是所有TransimittableThreadLocal和注册ThreadLocal的值。
    */
	@NonNull
	public static Object capture() {
		return new Snapshot(captureTtlValues(), captureThreadLocalValues());
	}

	/**
    * 捕获TransimittableThreadLocal的值,将holder中的所有值都添加到HashMap后返回。
    */
	private static HashMap<TransmittableThreadLocal<Object>, Object> captureTtlValues() {
		HashMap<TransmittableThreadLocal<Object>, Object> ttl2Value = 
			new HashMap<TransmittableThreadLocal<Object>, Object>();
		for (TransmittableThreadLocal<Object> threadLocal : holder.get().keySet()) {
			ttl2Value.put(threadLocal, threadLocal.copyValue());
		}
		return ttl2Value;
	}

	/**
    * 捕获注册的ThreadLocal的值,也就是原本线程中的ThreadLocal,可以注册到TTL中，在
    * 进行线程池本地变量传递时也会被传递。
    */
	private static HashMap<ThreadLocal<Object>, Object> captureThreadLocalValues() {
		final HashMap<ThreadLocal<Object>, Object> threadLocal2Value = 
			new HashMap<ThreadLocal<Object>, Object>();
		for(Map.Entry<ThreadLocal<Object>,TtlCopier<Object>>entry:threadLocalHolder.entrySet()){
			final ThreadLocal<Object> threadLocal = entry.getKey();
			final TtlCopier<Object> copier = entry.getValue();
			threadLocal2Value.put(threadLocal, copier.copy(threadLocal.get()));
		}
		return threadLocal2Value;
	}

	/**
    * 将捕获到的本地变量进行替换子线程的本地变量，并且返回子线程现有的本地变量副本backup。
    * 用于在执行run/call方法之后，将本地变量副本恢复。
    */
	@NonNull
	public static Object replay(@NonNull Object captured) {
		final Snapshot capturedSnapshot = (Snapshot) captured;
		return new Snapshot(replayTtlValues(capturedSnapshot.ttl2Value), 
							replayThreadLocalValues(capturedSnapshot.threadLocal2Value));
	}

	/**
    * 替换TransmittableThreadLocal
    */
	@NonNull
	private static HashMap<TransmittableThreadLocal<Object>, Object> replayTtlValues(@NonNull HashMap<TransmittableThreadLocal<Object>, Object> captured) {
		// 创建副本backup
		HashMap<TransmittableThreadLocal<Object>, Object> backup = 
			new HashMap<TransmittableThreadLocal<Object>, Object>();

		for (final Iterator<TransmittableThreadLocal<Object>> iterator = holder.get().keySet().iterator(); iterator.hasNext(); ) {
			TransmittableThreadLocal<Object> threadLocal = iterator.next();
			// 对当前线程的本地变量进行副本拷贝
			backup.put(threadLocal, threadLocal.get());

			// 若出现调用线程中不存在某个线程变量，而线程池中线程有，则删除线程池中对应的本地变量
			if (!captured.containsKey(threadLocal)) {
				iterator.remove();
				threadLocal.superRemove();
			}
		}
		// 将捕获的TTL值打入线程池获取到的线程TTL中。
		setTtlValuesTo(captured);
		// 是一个扩展点，调用TTL的beforeExecute方法。默认实现为空
		doExecuteCallback(true);
		return backup;
	}

	private static HashMap<ThreadLocal<Object>, Object> replayThreadLocalValues(@NonNull HashMap<ThreadLocal<Object>, Object> captured) {
		final HashMap<ThreadLocal<Object>, Object> backup = 
			new HashMap<ThreadLocal<Object>, Object>();
		for (Map.Entry<ThreadLocal<Object>, Object> entry : captured.entrySet()) {
			final ThreadLocal<Object> threadLocal = entry.getKey();
			backup.put(threadLocal, threadLocal.get());
			final Object value = entry.getValue();
			if (value == threadLocalClearMark) threadLocal.remove();
			else threadLocal.set(value);
		}
		return backup;
	}

	/**
    * 清除单线线程的所有TTL和TL，并返回清除之气的backup
    */
	@NonNull
	public static Object clear() {
		final HashMap<TransmittableThreadLocal<Object>, Object> ttl2Value = 
			new HashMap<TransmittableThreadLocal<Object>, Object>();

		final HashMap<ThreadLocal<Object>, Object> threadLocal2Value = 
			new HashMap<ThreadLocal<Object>, Object>();
		for(Map.Entry<ThreadLocal<Object>,TtlCopier<Object>>entry:threadLocalHolder.entrySet()){
			final ThreadLocal<Object> threadLocal = entry.getKey();
			threadLocal2Value.put(threadLocal, threadLocalClearMark);
		}
		return replay(new Snapshot(ttl2Value, threadLocal2Value));
	}

	/**
    * 还原
    */
	public static void restore(@NonNull Object backup) {
		final Snapshot backupSnapshot = (Snapshot) backup;
		restoreTtlValues(backupSnapshot.ttl2Value);
		restoreThreadLocalValues(backupSnapshot.threadLocal2Value);
	}

	private static void restoreTtlValues(@NonNull HashMap<TransmittableThreadLocal<Object>, Object> backup) {
		// 扩展点，调用TTL的afterExecute
		doExecuteCallback(false);

		for (final Iterator<TransmittableThreadLocal<Object>> iterator = holder.get().keySet().iterator(); iterator.hasNext(); ) {
			TransmittableThreadLocal<Object> threadLocal = iterator.next();

			if (!backup.containsKey(threadLocal)) {
				iterator.remove();
				threadLocal.superRemove();
			}
		}

		// 将本地变量恢复成备份版本
		setTtlValuesTo(backup);
	}

	private static void setTtlValuesTo(@NonNull HashMap<TransmittableThreadLocal<Object>, Object> ttlValues) {
		for (Map.Entry<TransmittableThreadLocal<Object>, Object> entry : ttlValues.entrySet()) {
			TransmittableThreadLocal<Object> threadLocal = entry.getKey();
			threadLocal.set(entry.getValue());
		}
	}

	private static void restoreThreadLocalValues(@NonNull HashMap<ThreadLocal<Object>, Object> backup) {
		for (Map.Entry<ThreadLocal<Object>, Object> entry : backup.entrySet()) {
			final ThreadLocal<Object> threadLocal = entry.getKey();
			threadLocal.set(entry.getValue());
		}
	}

	/**
   * 快照类，保存TTL和TL
   */
	private static class Snapshot {
		final HashMap<TransmittableThreadLocal<Object>, Object> ttl2Value;
		final HashMap<ThreadLocal<Object>, Object> threadLocal2Value;

		private Snapshot(HashMap<TransmittableThreadLocal<Object>, Object> ttl2Value,
						 HashMap<ThreadLocal<Object>, Object> threadLocal2Value) {
			this.ttl2Value = ttl2Value;
			this.threadLocal2Value = threadLocal2Value;
		}
	}
}
```
进入`TtlCallable#call()`方法。
```java
@Override
public V call() throws Exception {
	Object captured = capturedRef.get();
	if (captured == null || releaseTtlValueReferenceAfterCall && 
		!capturedRef.compareAndSet(captured, null)) {
		throw new IllegalStateException("TTL value reference is released after call!");
	}
	// 调用replay方法将捕获到的当前线程的本地变量，传递给线程池线程的本地变量，
	// 并且获取到线程池线程覆盖之前的本地变量副本。
	Object backup = replay(captured);
	try {
		// 线程方法调用
		return callable.call();
	} finally {
		// 使用副本进行恢复。
		restore(backup);
	}
}
```
到这基本上线程池方式传递本地变量的核心代码已经大概看完了。总的来说在创建`TtlCallable`对象是，调用`capture()`方法捕获调用方的本地线程变量，在`call()`执行时，将捕获到的线程变量，替换到线程池所对应获取到的线程的本地变量中，并且在执行完成之后，将其本地变量恢复到调用之前。
