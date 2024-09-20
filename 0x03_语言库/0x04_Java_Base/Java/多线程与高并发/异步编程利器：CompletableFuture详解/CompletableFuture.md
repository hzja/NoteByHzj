Java CompletableFuture
<a name="Loa5j"></a>
### 场景说明
查询所有商店某个商品的价格并返回，并且查询商店某个商品的价格的API为同步 一个Shop类，提供一个名为getPrice的同步方法

- 店铺类：Shop.java
```java
public class Shop {
    private Random random = new Random();
    /**
     * 根据产品名查找价格
     * */
    public double getPrice(String product) {
        return calculatePrice(product);
    }

    /**
     * 计算价格
     *
     * @param product
     * @return
     * */
    private double calculatePrice(String product) {
        delay();
        //random.nextDouble()随机返回折扣
        return random.nextDouble() * product.charAt(0) + product.charAt(1);
    }

    /**
     * 通过睡眠模拟其他耗时操作
     * */
    private void delay() {
        try {
            Thread.sleep(1000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
```
查询商品的价格为同步方法，并通过`sleep`方法模拟其他操作。这个场景模拟了当需要调用第三方API，但第三方提供的是同步API，在无法修改第三方API时如何设计代码调用提高应用的性能和吞吐量，这时候可以使用`CompletableFuture`类
<a name="hHcdn"></a>
### `CompletableFuture`使用
`Completable`是`Future`接口的实现类，在JDK1.8中引入

- `**CompletableFuture**`**的创建：**说明：
   - 两个重载方法之间的区别 => 后者可以传入自定义`Executor`，前者是默认的，使用的`ForkJoinPool`
   - `supplyAsync`和`runAsync`方法之间的区别 => 前者有返回值，后者无返回值
   - `Supplier`是函数式接口，因此该方法需要传入该接口的实现类，追踪源码会发现在`run`方法中会调用该接口的方法。因此使用该方法创建`CompletableFuture`对象只需重写`Supplier`中的`get`方法，在`get`方法中定义任务即可。又因为函数式接口可以使用`Lambda`表达式，和`new`创建`CompletableFuture`对象相比代码会**简洁**不少
   - 使用`new`方法
```java
CompletableFuture<Double> futurePrice = new CompletableFuture<>();
```

- 使用`CompletableFuture#completedFuture`静态方法创建
```java
public static <U> CompletableFuture<U> completedFuture(U value) {
    return new CompletableFuture<U>((value == null) ? NIL : value);
}
```
参数的值为任务执行完的结果，一般该方法在实际应用中较少应用

- 使用 `CompletableFuture#supplyAsync`静态方法创建 `supplyAsync`有两个重载方法：
```java
//方法一
public static <U> CompletableFuture<U> supplyAsync(Supplier<U> supplier) {
    return asyncSupplyStage(asyncPool, supplier);
}
//方法二
public static <U> CompletableFuture<U> supplyAsync(Supplier<U> supplier,
                                                   Executor executor) {
    return asyncSupplyStage(screenExecutor(executor), supplier);
}
```

- 使用`CompletableFuture#runAsync`静态方法创建 `runAsync`有两个重载方法
```java
//方法一
public static CompletableFuture<Void> runAsync(Runnable runnable) {
    return asyncRunStage(asyncPool, runnable);
}
//方法二
public static CompletableFuture<Void> runAsync(Runnable runnable, Executor executor) {
    return asyncRunStage(screenExecutor(executor), runnable);
}
```

- **结果的获取：** 对于结果的获取`CompltableFuture`类提供了四种方式
```java
//方式一
public T get()
//方式二
public T get(long timeout, TimeUnit unit)
//方式三
public T getNow(T valueIfAbsent)
//方式四
public T join()
```
说明：<br />示例：

   - `get()`和`get(long timeout, TimeUnit unit)` => 在`Future`中就已经提供了，后者提供超时处理，如果在指定时间内未获取结果将抛出超时异常
   - `getNow` => 立即获取结果不阻塞，结果计算已完成将返回结果或计算过程中的异常，如果未计算完成将返回设定的`valueIfAbsent`值
   - `join` => 方法里不会抛出异常
```java
public class AcquireResultTest {
    public static void main(String[] args) throws ExecutionException, InterruptedException {
        //getNow方法测试
        CompletableFuture<String> cp1 = CompletableFuture.supplyAsync(() -> {
            try {
                Thread.sleep(60 * 1000 * 60 );
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            return "hello world";
        });

        System.out.println(cp1.getNow("hello h2t"));

        //join方法测试
        CompletableFuture<Integer> cp2 = CompletableFuture.supplyAsync((()-> 1 / 0));
        System.out.println(cp2.join());

        //get方法测试
        CompletableFuture<Integer> cp3 = CompletableFuture.supplyAsync((()-> 1 / 0));
        System.out.println(cp3.get());
    }
}
```
说明：

- 第一个执行结果为hello h2t，因为要先睡上1分钟结果不能立即获取
- `join`方法获取结果方法里不会抛异常，但是执行结果会抛异常，抛出的异常为`CompletionException`
- `get`方法获取结果方法里将抛出异常，执行结果抛出的异常为`ExecutionException`
- **异常处理：** 使用静态方法创建的`CompletableFuture`对象无需显示处理异常，使用new创建的对象需要调用`completeExceptionally`方法设置捕获到的异常，举例说明：
```java
CompletableFuture completableFuture = new CompletableFuture();
new Thread(() -> {
   try {
       //doSomething，调用complete方法将其他方法的执行结果记录在completableFuture对象中
       completableFuture.complete(null);
   } catch (Exception e) {
       //异常处理
       completableFuture.completeExceptionally(e);
    }
}).start();
```
<a name="cpku4"></a>
### 同步方法`Pick`异步方法查询所有店铺某个商品价格
店铺为一个列表：
```java
private static List<Shop> shopList = Arrays.asList(
        new Shop("BestPrice"),
        new Shop("LetsSaveBig"),
        new Shop("MyFavoriteShop"),
        new Shop("BuyItAll")
);
```
**同步方法：**
```java
private static List<String> findPriceSync(String product) {
    return shopList.stream()
            .map(shop -> String.format("%s price is %.2f",
                    shop.getName(), shop.getPrice(product)))  //格式转换
            .collect(Collectors.toList());
}
```
**异步方法：**
```java
private static List<String> findPriceAsync(String product) {
    List<CompletableFuture<String>> completableFutureList = shopList.stream()
            //转异步执行
            .map(shop -> CompletableFuture.supplyAsync(
                    () -> String.format("%s price is %.2f",
                            shop.getName(), shop.getPrice(product))))  //格式转换
            .collect(Collectors.toList());

    return completableFutureList.stream()
            .map(CompletableFuture::join)  //获取结果不会抛出异常
            .collect(Collectors.toList());
}
```
**性能测试结果：**
```java
Find Price Sync Done in 4141
Find Price Async Done in 1033
```
**异步**执行效率**提高四倍**
<a name="KKb8f"></a>
### 为什么仍需要`CompletableFuture`
在JDK1.8以前，通过调用线程池的`submit`方法可以让任务以异步的方式运行，该方法会返回一个`Future`对象，通过调用`get`方法获取异步执行的结果：
```java
private static List<String> findPriceFutureAsync(String product) {
    ExecutorService es = Executors.newCachedThreadPool();
    List<Future<String>> futureList = shopList.stream().map(shop -> es.submit(() -> String.format("%s price is %.2f",
            shop.getName(), shop.getPrice(product)))).collect(Collectors.toList());

    return futureList.stream()
            .map(f -> {
                String result = null;
                try {
                    result = f.get();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                } catch (ExecutionException e) {
                    e.printStackTrace();
                }

                return result;
            }).collect(Collectors.toList());
}
```
既生瑜何生亮，为什么仍需要引入`CompletableFuture`？对于简单的业务场景使用`Future`完全没有，但是想将多个异步任务的计算结果组合起来，后一个异步任务的计算结果需要前一个异步任务的值等等，使用`Future`提供的那点API就囊中羞涩，处理起来不够优雅，这时候还是让`CompletableFuture`以**声明式**的方式优雅的处理这些需求。而且在`Future`编程中想要拿到Future的值然后拿这个值去做后续的计算任务，只能通过轮询的方式去判断任务是否完成这样非常占CPU并且代码也不优雅，用伪代码表示如下：
```java
while(future.isDone()) {
    result = future.get();
    doSomrthingWithResult(result);
} 
```
但`CompletableFuture`提供了API实现这样的需求
<a name="JyoA4"></a>
### 其他API介绍
<a name="PVMIk"></a>
#### `whenComplete`计算结果的处理：
对前面计算结果进行处理，无法返回新值 提供了三个方法：
```java
//方法一
public CompletableFuture<T> whenComplete(BiConsumer<? super T,? super Throwable> action)
//方法二
public CompletableFuture<T> whenCompleteAsync(BiConsumer<? super T,? super Throwable> action)
//方法三
public CompletableFuture<T> whenCompleteAsync(BiConsumer<? super T,? super Throwable> action, Executor executor)
```
说明：

- `BiFunction<? super T,? super U,? extends V>` fn参数 => 定义对结果的处理
- `Executor` executor参数 => 自定义线程池
- 以`async`结尾的方法将会在一个新的线程中执行组合操作

示例：
```java
public class WhenCompleteTest {
    public static void main(String[] args) {
        CompletableFuture<String> cf1 = CompletableFuture.supplyAsync(() -> "hello");
        CompletableFuture<String> cf2 = cf1.whenComplete((v, e) ->
                System.out.println(String.format("value:%s, exception:%s", v, e)));
        System.out.println(cf2.join());
    }
}
```
<a name="t9l2t"></a>
#### `thenApply`转换：
将前面计算结果的的`CompletableFuture`传递给`thenApply`，返回`thenApply`处理后的结果。可以认为通过`thenApply`方法实现`CompletableFuture<T>`至`CompletableFuture<U>`的转换。白话一点就是将`CompletableFuture`的计算结果作为`thenApply`方法的参数，返回`thenApply`方法处理后的结果 提供了三个方法：
```java
//方法一
public <U> CompletableFuture<U> thenApply(
    Function<? super T,? extends U> fn) {
    return uniApplyStage(null, fn);
}

//方法二
public <U> CompletableFuture<U> thenApplyAsync(
    Function<? super T,? extends U> fn) {
    return uniApplyStage(asyncPool, fn);
}

//方法三
public <U> CompletableFuture<U> thenApplyAsync(
    Function<? super T,? extends U> fn, Executor executor) {
    return uniApplyStage(screenExecutor(executor), fn);
}
```
说明：

- `Function<? super T,? extends U>` fn参数 => 对前一个`CompletableFuture `计算结果的转化操作
- `Executor` executor参数 => 自定义线程池
- 以`async`结尾的方法将会在一个新的线程中执行组合操作 示例：
```java
public class ThenApplyTest {
    public static void main(String[] args) throws ExecutionException, InterruptedException {
        CompletableFuture<Integer> result = CompletableFuture.supplyAsync(ThenApplyTest::randomInteger).thenApply((i) -> i * 8);
        System.out.println(result.get());
    }

    public static Integer randomInteger() {
        return 10;
    }
}
```
这里将前一个`CompletableFuture`计算出来的结果扩大八倍
<a name="rB594"></a>
#### `thenAccept`结果处理：
`thenApply`也可以归类为对结果的处理，`thenAccept`和`thenApply`的区别就是没有返回值 提供了三个方法：
```java
//方法一
public CompletableFuture<Void> thenAccept(Consumer<? super T> action) {
    return uniAcceptStage(null, action);
}

//方法二
public CompletableFuture<Void> thenAcceptAsync(Consumer<? super T> action) {
    return uniAcceptStage(asyncPool, action);
}

//方法三
public CompletableFuture<Void> thenAcceptAsync(Consumer<? super T> action,
                                               Executor executor) {
    return uniAcceptStage(screenExecutor(executor), action);
}
```
说明：

- `Consumer<? super T>` action参数 => 对前一个`CompletableFuture`计算结果的操作
- `Executor` executor参数 => 自定义线程池
- 同理以`async`结尾的方法将会在一个新的线程中执行组合操作 示例：
```java
public class ThenAcceptTest {
    public static void main(String[] args) {
        CompletableFuture.supplyAsync(ThenAcceptTest::getList).thenAccept(strList -> strList.stream()
                .forEach(m -> System.out.println(m)));
    }

    public static List<String> getList() {
        return Arrays.asList("a", "b", "c");
    }
}
```
将前一个`CompletableFuture`计算出来的结果打印出来
<a name="kGfVo"></a>
#### `thenCompose`异步结果流水化：
`thenCompose`方法可以将两个异步操作进行流水操作 提供了三个方法：
```java
//方法一
public <U> CompletableFuture<U> thenCompose(
    Function<? super T, ? extends CompletionStage<U>> fn) {
    return uniComposeStage(null, fn);
}

//方法二
public <U> CompletableFuture<U> thenComposeAsync(
    Function<? super T, ? extends CompletionStage<U>> fn) {
    return uniComposeStage(asyncPool, fn);
}

//方法三
public <U> CompletableFuture<U> thenComposeAsync(
    Function<? super T, ? extends CompletionStage<U>> fn,
    Executor executor) {
    return uniComposeStage(screenExecutor(executor), fn);
}
```
说明：

- `Function<? super T, ? extends CompletionStage<U>>` fn参数 => 当前`CompletableFuture`计算结果的执行
- `Executor` executor参数 => 自定义线程池
- 同理以`async`结尾的方法将会在一个新的线程中执行组合操作 示例：
```java
public class ThenComposeTest {
    public static void main(String[] args) throws ExecutionException, InterruptedException {
        CompletableFuture<Integer> result = CompletableFuture.supplyAsync(ThenComposeTest::getInteger)
                .thenCompose(i -> CompletableFuture.supplyAsync(() -> i * 10));
        System.out.println(result.get());
    }

    private static int getInteger() {
        return 666;
    }

    private static int expandValue(int num) {
        return num * 10;
    }
}
```
执行流程图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639892110176-3ad4fe47-6a45-40d0-a2de-a2f0c486aae3.webp#clientId=ub93b58a0-15e5-4&from=paste&id=u16529ddd&originHeight=392&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u07948158-fa23-4a32-8e8c-66376851477&title=)
<a name="DA00d"></a>
#### `thenCombine`组合结果：
`thenCombine`方法将两个无关的`CompletableFuture`组合起来，第二个`Completable`并不依赖第一个`Completable`的结果 提供了三个方法：
```java
//方法一
public <U,V> CompletableFuture<V> thenCombine( 
    CompletionStage<? extends U> other,
    BiFunction<? super T,? super U,? extends V> fn) {
    return biApplyStage(null, other, fn);
}
//方法二
public <U,V> CompletableFuture<V> thenCombineAsync(
    CompletionStage<? extends U> other,
    BiFunction<? super T,? super U,? extends V> fn) {
    return biApplyStage(asyncPool, other, fn);
}

//方法三
public <U,V> CompletableFuture<V> thenCombineAsync(
    CompletionStage<? extends U> other,
    BiFunction<? super T,? super U,? extends V> fn, Executor executor) {
    return biApplyStage(screenExecutor(executor), other, fn);
}
```
说明：

- `CompletionStage<? extends U>` other参数 => 新的`CompletableFuture`的计算结果
- `BiFunction<? super T,? super U,? extends V>` fn参数 => 定义了两个`CompletableFuture`对象**完成计算后**如何合并结果，该参数是一个函数式接口，因此可以使用Lambda表达式
- `Executor` executor参数 => 自定义线程池
- 同理以`async`结尾的方法将会在一个新的线程中执行组合操作

示例：
```java
public class ThenCombineTest {
    private static Random random = new Random();
    public static void main(String[] args) throws ExecutionException, InterruptedException {
        CompletableFuture<Integer> result = CompletableFuture.supplyAsync(ThenCombineTest::randomInteger).thenCombine(
                CompletableFuture.supplyAsync(ThenCombineTest::randomInteger), (i, j) -> i * j
        );

        System.out.println(result.get());
    }

    public static Integer randomInteger() {
        return random.nextInt(100);
    }
}
```
将两个线程计算出来的值做一个乘法在返回 执行流程图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639892109887-b91f2419-6894-487d-8b47-3f71d5f03f49.webp#clientId=ub93b58a0-15e5-4&from=paste&id=u7cc5f681&originHeight=392&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5ca0f15d-6dfd-4a7c-8437-d7cc56cea64&title=)
<a name="a2vNe"></a>
#### `allOf`&`anyOf`组合多个`CompletableFuture`：
方法介绍：
```java
//allOf
public static CompletableFuture<Void> allOf(CompletableFuture<?>... cfs) {
    return andTree(cfs, 0, cfs.length - 1);
}
//anyOf
public static CompletableFuture<Object> anyOf(CompletableFuture<?>... cfs) {
    return orTree(cfs, 0, cfs.length - 1);
}
```
说明：

- `allOf` => 所有的`CompletableFuture`都执行完后执行计算。
- `anyOf` => 任意一个`CompletableFuture`执行完后就会执行计算

示例：

- `allOf`方法测试
```java
public class AllOfTest {
  public static void main(String[] args) throws ExecutionException, InterruptedException {
      CompletableFuture<Void> future1 = CompletableFuture.supplyAsync(() -> {
          System.out.println("hello");
          return null;
      });
      CompletableFuture<Void> future2 = CompletableFuture.supplyAsync(() -> {
          System.out.println("world"); return null;
      });
      CompletableFuture<Void> result = CompletableFuture.allOf(future1, future2);
      System.out.println(result.get());
  }
}
```
`allOf`方法没有返回值，适合没有返回值并且需要前面所有任务执行完毕才能执行后续任务的应用场景

- `anyOf`方法测试
```java
public class AnyOfTest {
  private static Random random = new Random();
  public static void main(String[] args) throws ExecutionException, InterruptedException {
      CompletableFuture<String> future1 = CompletableFuture.supplyAsync(() -> {
          randomSleep();
          System.out.println("hello");
          return "hello";});
      CompletableFuture<String> future2 = CompletableFuture.supplyAsync(() -> {
          randomSleep();
          System.out.println("world");
          return "world";
      });
      CompletableFuture<Object> result = CompletableFuture.anyOf(future1, future2);
      System.out.println(result.get());
 }
  
  private static void randomSleep() {
      try {
          Thread.sleep(random.nextInt(10));
      } catch (InterruptedException e) {
          e.printStackTrace();
      }
  }
}
```
两个线程都会将结果打印出来，但是`get`方法只会返回最先完成任务的结果。该方法比较适合只要有一个返回值就可以继续执行其他任务的应用场景
<a name="zLYGx"></a>
#### 注意点
很多方法都提供了异步实现【带`async`后缀】，但是需小心谨慎使用这些异步方法，因为异步意味着存在上下文切换，可能性能不一定比同步好。如果需要使用异步的方法，**先做测试**，用测试数据说话！！！
<a name="jBMvB"></a>
### `CompletableFuture`的应用场景
存在IO密集型的任务可以选择`CompletableFuture`，IO部分交由另外一个线程去执行。Logback、Log4j2异步日志记录的实现原理就是新起了一个线程去执行IO操作，这部分可以以`CompletableFuture.runAsync(()->{ioOperation();})`的方式去调用。如果是CPU密集型就不推荐使用了推荐使用并行流
<a name="gqCQj"></a>
### 优化空间
`supplyAsync`执行任务底层实现：
```java
public static <U> CompletableFuture<U> supplyAsync(Supplier<U> supplier) {
    return asyncSupplyStage(asyncPool, supplier);
}
static <U> CompletableFuture<U> asyncSupplyStage(Executor e, Supplier<U> f) {
    if (f == null) throw new NullPointerException();
    CompletableFuture<U> d = new CompletableFuture<U>();
    e.execute(new AsyncSupply<U>(d, f));
    return d;
}
```
底层调用的是线程池去执行任务，而`CompletableFuture`中默认线程池为`ForkJoinPool`
```java
private static final Executor asyncPool = useCommonPool ?
        ForkJoinPool.commonPool() : new ThreadPerTaskExecutor();
```
`ForkJoinPool`线程池的大小取决于CPU的核数。CPU密集型任务线程池大小配置为CPU核心数就可以了，但是IO密集型，线程池的大小由CPU数量 * CPU利用率 * (1 + 线程等待时间/线程CPU时间)确定。而`CompletableFuture`的应用场景就是IO密集型任务，因此默认的`ForkJoinPool`一般无法达到最佳性能，需自己根据业务创建线程池
