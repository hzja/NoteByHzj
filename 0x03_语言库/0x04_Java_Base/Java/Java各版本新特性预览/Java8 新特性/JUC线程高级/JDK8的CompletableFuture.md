JavaCompletableFuture<br />来总结一下CompletableFuture的一些API的方法。
<a name="yzJZp"></a>
#### `CompletableFuture`
`CompletableFuture`是java.util.concurrent库在java 8中新增的主要工具，同传统的Future相比，其支持流式计算、函数式编程、完成通知、自定义异常处理等很多新的特性。
<a name="WwCMR"></a>
#### `supplyAsync`
通过该函数创建的`CompletableFuture`实例会异步执行当前传入的计算任务。在调用端，则可以通过`get`或`join`获取最终计算结果。<br />这个有两个不同的实现方式，一种是传自己创建的线程池，然后使用创建的线程池进行操作，还有一种就是不传线程池，让程序是使用默认的线程池进行操作。
```java
//使用默认线程池
public static <U> CompletableFuture<U> supplyAsync(Supplier<U> supplier) {
	return asyncSupplyStage(asyncPool, supplier);
}
//使用自定义线程池    
public static <U> CompletableFuture<U> supplyAsync(Supplier<U> supplier,                                      Executor executor) {
	return asyncSupplyStage(screenExecutor(executor), supplier);
}
```
第一种只需传入一个Supplier实例（一般使用lamda表达式），此时框架会默认使用ForkJoin的线程池来执行被提交的任务。<br />来自定义一个代码看一下
```java
ExecutorService executors = Executors.newFixedThreadPool(5);

CompletableFuture<List<Order>> bFuture = CompletableFuture.supplyAsync(() -> {
	//执行查询
	QueryWrapper<Order> queryWrapper = new QueryWrapper<>();
	queryWrapper.eq("userId",params.getUserId());
	queryWrapper.eq("SHOP_ID",params.getShopId());
	List<Order> list = orderService.list(queryWrapper);
	return list;
}, executors);
```
当执行查询的时候，这时候实际上就属于异步的查询的，可以写多个查询，比如，上面的代码查询的是订单，下面可以查询用户的信息，还是使用同样的线程池。
```java
CompletableFuture<List<User>> aFuture = CompletableFuture.supplyAsync(() -> {
	//执行查询
	QueryWrapper<User> queryWrapper = new QueryWrapper<>();
	queryWrapper.eq("SHOP_ID",params.getShopId());
	List<User> list = userService.list(queryWrapper);
	return list;
}, executors);
```
这时候，就可以获取一下这个返回的结果，那么获取返回结果需要调用什么方法呢？<br />可以通过get或join获取最终计算结果。这时候可以打印一下，获取结果的长度
```java
List<User> list = aFuture.get();
List<Order> list = bFuture.get();
```
这样，就可以使用异步查询来完成对结果集的查询。<br />这就有小伙伴想问，如果想要用第一个的结果，去在第二个数据中去查询，应该怎么做呢？
<a name="uVIP4"></a>
#### `thenApply`
`thenApply`提交的任务类型需遵从Function签名，也就是有入参和返回值，其中入参为前置任务的结果。<br />什么意思呢？其实很简单，这里直接看代码：
```java
CompletableFuture<List<Order>> cFuture = bFuture.thenApply((list) -> {
	List<String> collect = list.stream().map(User::getUserId).collect(Collectors.toList());
	...
		return orderList;
});
```
这实际上，就是根据查询出的所有用户的集合，直接获取到他的userId，然后根据UserId，把这些用户下的订单数据都提取出来，当然，在实际使用中，理论上可以无限连接后续计算任务，从而实现链条更长的流式计算。<br />但是这种链式也不是都非常的好用，毕竟要控制住线程池，大家记得在使用完成之后，可以把自己创建的线程池小回调，调用`shutDown`方法就可以了。再接着往下说。
<a name="e2tGS"></a>
#### `thenAccept`
实际上`thenAccept`的效果，和`thenApply `的效果等同，但是`thenAccept`提交的任务类型需遵从Consumer签名，也就是有入参但是没有返回值，其中入参为前置任务的结果。<br />实际上调用的是和之前一样的，但是就是没有返回值了。
```java
CompletableFuture<void> cFuture = bFuture.thenAccept((list) -> {
	List<String> collect = list.stream().map(User::getUserId).collect(Collectors.toList());
	...
});
```
这就是区别，不需要进行返回了，这种一般用的地方比较特殊，就比如说执行了很多操作，但是在其中需要夹杂一些比如说给某个服务进行通知，但是并不需要这个服务给返回值的结果的时候，实际上完全可以使用这种方式。方便还快捷。当然，想直接调用，也是可以的。
<a name="nHWUP"></a>
#### `whenComplete`
`whenComplete`主要用于注入任务完成时的回调通知逻辑。这个解决了传统future在任务完成时，无法主动发起通知的问题。前置任务会将计算结果或者抛出的异常作为入参传递给回调通知函数。<br />这个方法就是相当于是把前一个任务中的结果，通过第二个方法获取结果，也并不会影响第二个的逻辑。<br />示例代码如下：
```java
CompletableFuture<List<Order>> listCompletableFuture = bFuture.whenComplete((r, e) -> {
	if (e != null) {
		System.out.println("compute failed!");
	} else {
		System.out.println("received result is " + r);
	}
});
```
实际上最后答应的就是接受者的信息。
