Java CompletableFuture<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1622975609318-f71f9a66-59a2-4df9-9372-7fccebf6bb2d.png#averageHue=%23f6f6f5&clientId=uf417faae-73ad-4&from=paste&id=u9934f14f&originHeight=462&originWidth=1080&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=none&taskId=u80ce5b85-d2d1-43c2-a645-5628ca6a76d&title=)
<a name="BwL7g"></a>
## 一个例子回顾 `Future`
因为`CompletableFuture`实现了`Future`接口，先来回顾`Future`吧。<br />`Future`是Java5新加的一个接口，它提供了一种异步并行计算的功能。如果主线程需要执行一个很耗时的计算任务，就可以通过`future`把这个任务放到异步线程中执行。主线程继续处理其他任务，处理完成后，再通过`Future`获取计算结果。<br />来看个简单例子吧，假设有两个任务服务，一个查询用户基本信息，一个是查询用户勋章信息。如下，
```java
public class UserInfoService {

    public UserInfo getUserInfo(Long userId) throws InterruptedException {
        Thread.sleep(300);//模拟调用耗时
        return new UserInfo("666", "Hello", 27); //一般是查数据库，或者远程调用返回的
    }
}

public class MedalService {

    public MedalInfo getMedalInfo(long userId) throws InterruptedException {
        Thread.sleep(500); //模拟调用耗时
        return new MedalInfo("666", "守护勋章");
    }
}
```
接下来，来演示下，在主线程中是如何使用`Future`来进行异步调用的。
```java
public class FutureTest {

    public static void main(String[] args) throws ExecutionException, InterruptedException {

        ExecutorService executorService = Executors.newFixedThreadPool(10);

        UserInfoService userInfoService = new UserInfoService();
        MedalService medalService = new MedalService();
        long userId =666L;
        long startTime = System.currentTimeMillis();

        //调用用户服务获取用户基本信息
        FutureTask<UserInfo> userInfoFutureTask = new FutureTask<>(new Callable<UserInfo>() {
            @Override
            public UserInfo call() throws Exception {
                return userInfoService.getUserInfo(userId);
            }
        });
        executorService.submit(userInfoFutureTask);

        Thread.sleep(300); //模拟主线程其它操作耗时

        FutureTask<MedalInfo> medalInfoFutureTask = new FutureTask<>(new Callable<MedalInfo>() {
            @Override
            public MedalInfo call() throws Exception {
                return medalService.getMedalInfo(userId);
            }
        });
        executorService.submit(medalInfoFutureTask);

        UserInfo userInfo = userInfoFutureTask.get();//获取个人信息结果
        MedalInfo medalInfo = medalInfoFutureTask.get();//获取勋章信息结果

        System.out.println("总共用时" + (System.currentTimeMillis() - startTime) + "ms");
    }
}
    
```
运行结果:
```java
总共用时806ms
```
如果不使用Future进行并行异步调用，而是在主线程串行进行的话，耗时大约为300+500+300 = 1100 ms。可以发现，future+线程池异步配合，提高了程序的执行效率。<br />但是Future对于结果的获取，不是很友好，只能通过阻塞或者轮询的方式得到任务的结果。

- `Future.get()` 就是阻塞调用，在线程获取结果之前`get`方法会一直阻塞。
- Future提供了一个`isDone`方法，可以在程序中轮询这个方法查询执行结果。

阻塞的方式和异步编程的设计理念相违背，而轮询的方式会耗费无谓的CPU资源。因此，JDK8设计出`CompletableFuture`。`CompletableFuture`提供了一种观察者模式类似的机制，可以让任务执行完成后通知监听的一方。
<a name="gfITV"></a>
## 一个例子走进`CompletableFuture`
还是基于以上`Future`的例子，改用`CompletableFuture` 来实现
```java
public class FutureTest {

    public static void main(String[] args) throws InterruptedException, ExecutionException, TimeoutException {

        UserInfoService userInfoService = new UserInfoService();
        MedalService medalService = new MedalService();
        long userId =666L;
        long startTime = System.currentTimeMillis();

        //调用用户服务获取用户基本信息
        CompletableFuture<UserInfo> completableUserInfoFuture = CompletableFuture.supplyAsync(() -> userInfoService.getUserInfo(userId));

        Thread.sleep(300); //模拟主线程其它操作耗时

        CompletableFuture<MedalInfo> completableMedalInfoFuture = CompletableFuture.supplyAsync(() -> medalService.getMedalInfo(userId)); 

        UserInfo userInfo = completableUserInfoFuture.get(2,TimeUnit.SECONDS);//获取个人信息结果
        MedalInfo medalInfo = completableMedalInfoFuture.get();//获取勋章信息结果
        System.out.println("总共用时" + (System.currentTimeMillis() - startTime) + "ms");

    }
}
```
可以发现，使用`CompletableFuture`，代码简洁了很多。`CompletableFuture`的`supplyAsync`方法，提供了异步执行的功能，线程池也不用单独创建了。实际上，它`CompletableFuture`使用了默认线程池是`ForkJoinPool.commonPool`。<br />`CompletableFuture`提供了几十种方法辅助异步任务场景。这些方法包括创建异步任务、任务异步回调、多个任务组合处理等方面。
<a name="y3vBl"></a>
## `CompletableFuture`使用场景
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1622975609244-86485f46-ce98-435b-aaf9-76c0d7b75cee.webp#averageHue=%23e7eeeb&clientId=uf417faae-73ad-4&from=paste&id=uc8691761&originHeight=186&originWidth=624&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=none&taskId=uf183ea4a-82af-4dff-a9bf-b0c14cc0bbb&title=)
<a name="x3wjG"></a>
### 创建异步任务
`CompletableFuture`创建异步任务，一般有`supplyAsync`和`runAsync`两个方法<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1622975609368-f9fcba41-d1a2-45d5-ba54-832dd142813a.png#averageHue=%23f7f6f6&clientId=uf417faae-73ad-4&from=paste&id=ud62747d6&originHeight=242&originWidth=1080&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=none&taskId=u4ec0bff0-b129-48f0-968c-26638759846&title=)<br />创建异步任务

- `supplyAsync`执行`CompletableFuture`任务，支持返回值
- `runAsync`执行`CompletableFuture`任务，没有返回值。
<a name="Nge8T"></a>
#### `supplyAsync`方法
```java
//使用默认内置线程池ForkJoinPool.commonPool()，根据supplier构建执行任务
public static <U> CompletableFuture<U> supplyAsync(Supplier<U> supplier)
//自定义线程，根据supplier构建执行任务
public static <U> CompletableFuture<U> supplyAsync(Supplier<U> supplier, Executor executor)
```
<a name="ezDWl"></a>
#### `runAsync`方法
```java
//使用默认内置线程池ForkJoinPool.commonPool()，根据runnable构建执行任务
public static CompletableFuture<Void> runAsync(Runnable runnable) 
//自定义线程，根据runnable构建执行任务
public static CompletableFuture<Void> runAsync(Runnable runnable,  Executor executor)
```
实例代码如下：
```java
public class FutureTest {

    public static void main(String[] args) {
        //可以自定义线程池
        ExecutorService executor = Executors.newCachedThreadPool();
        //runAsync的使用
        CompletableFuture<Void> runFuture = CompletableFuture.runAsync(() -> System.out.println("run,Hello"), executor);
        //supplyAsync的使用
        CompletableFuture<String> supplyFuture = CompletableFuture.supplyAsync(() -> {
                    System.out.print("supply,Hello");
                    return "Hello"; }, executor);
        //runAsync的future没有返回值，输出null
        System.out.println(runFuture.join());
        //supplyAsync的future，有返回值
        System.out.println(supplyFuture.join());
        executor.shutdown(); // 线程池需要关闭
    }
}
//输出
run,Hello
null
supply,Hello
```
<a name="xJ0z2"></a>
### 任务异步回调
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1622975609594-fe0168a3-48f4-4812-b4dd-28a6eb0341e5.png#averageHue=%23f6f6f6&clientId=uf417faae-73ad-4&from=paste&id=u007e3127&originHeight=485&originWidth=1080&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=none&taskId=uf0f83fca-0868-458f-aed0-3bd1af03312&title=)
<a name="Z9pN3"></a>
#### 1、`thenRun`/`thenRunAsync`
```java
public CompletableFuture<Void> thenRun(Runnable action);
public CompletableFuture<Void> thenRunAsync(Runnable action);
```
`CompletableFuture`的`thenRun`方法，通俗点讲就是，做完第一个任务后，再做第二个任务。某个任务执行完成后，执行回调方法；但是前后两个任务没有参数传递，第二个任务也没有返回值
```java
public class FutureThenRunTest {

    public static void main(String[] args) throws ExecutionException, InterruptedException {

        CompletableFuture<String> orgFuture = CompletableFuture.supplyAsync(
                ()->{
                    System.out.println("先执行第一个CompletableFuture方法任务");
                    return "Hello";
                }
        );

        CompletableFuture thenRunFuture = orgFuture.thenRun(() -> {
            System.out.println("接着执行第二个任务");
        });

        System.out.println(thenRunFuture.get());
    }
}
//输出
先执行第一个CompletableFuture方法任务
接着执行第二个任务
null
```
`thenRun`和`thenRunAsync`有什么区别呢？可以看下源码：
```java
private static final Executor asyncPool = useCommonPool ?
    ForkJoinPool.commonPool() : new ThreadPerTaskExecutor();

public CompletableFuture<Void> thenRun(Runnable action) {
    return uniRunStage(null, action);
}

public CompletableFuture<Void> thenRunAsync(Runnable action) {
    return uniRunStage(asyncPool, action);
}
```
如果执行第一个任务的时候，传入了一个自定义线程池：

- 调用`thenRun`方法执行第二个任务时，则第二个任务和第一个任务是共用同一个线程池。
- 调用`thenRunAsync`执行第二个任务时，则第一个任务使用的是你自己传入的线程池，第二个任务使用的是ForkJoin线程池

TIPS: 后面介绍的`thenAccept`和`thenAcceptAsync`，`thenApply`和`thenApplyAsync`等，它们之间的区别也是这个。
<a name="PxaFH"></a>
#### 2、`thenAccept`/`thenAcceptAsync`
`CompletableFuture`的`thenAccept`方法表示，第一个任务执行完成后，执行第二个回调方法任务，会将该任务的执行结果，作为入参，传递到回调方法中，但是回调方法是没有返回值的。
```java
public class FutureThenAcceptTest {

    public static void main(String[] args) throws ExecutionException, InterruptedException {

        CompletableFuture<String> orgFuture = CompletableFuture.supplyAsync(
                ()->{
                    System.out.println("原始CompletableFuture方法任务");
                    return "Hello";
                }
        );

        CompletableFuture thenAcceptFuture = orgFuture.thenAccept((a) -> {
            if ("Hello".equals(a)) {
                System.out.println("World");
            }

            System.out.println("Hi");
        });

        System.out.println(thenAcceptFuture.get());
    }
}
```
<a name="mYGMA"></a>
#### 3、`thenApply`/`thenApplyAsync`
`CompletableFuture`的`thenApply`方法表示，第一个任务执行完成后，执行第二个回调方法任务，会将该任务的执行结果，作为入参，传递到回调方法中，并且回调方法是有返回值的。
```java
public class FutureThenApplyTest {

    public static void main(String[] args) throws ExecutionException, InterruptedException {

        CompletableFuture<String> orgFuture = CompletableFuture.supplyAsync(
                ()->{
                    System.out.println("原始CompletableFuture方法任务");
                    return "Hello";
                }
        );

        CompletableFuture<String> thenApplyFuture = orgFuture.thenApply((a) -> {
            if ("Hello".equals(a)) {
                return "World";
            }

            return "Hi";
        });

        System.out.println(thenApplyFuture.get());
    }
}
//输出
原始CompletableFuture方法任务
关注了
```
<a name="pZ08B"></a>
#### 4、`exceptionally`
`CompletableFuture`的`exceptionally`方法表示，某个任务执行异常时，执行的回调方法；并且有抛出异常作为参数，传递到回调方法。
```java
public class FutureExceptionTest {

    public static void main(String[] args) throws ExecutionException, InterruptedException {

        CompletableFuture<String> orgFuture = CompletableFuture.supplyAsync(
                ()->{
                    System.out.println("当前线程名称：" + Thread.currentThread().getName());
                    throw new RuntimeException();
                }
        );

        CompletableFuture<String> exceptionFuture = orgFuture.exceptionally((e) -> {
            e.printStackTrace();
            return "你的程序异常啦";
        });

        System.out.println(exceptionFuture.get());
    }
}
//输出
当前线程名称：ForkJoinPool.commonPool-worker-1
java.util.concurrent.CompletionException: java.lang.RuntimeException
 at java.util.concurrent.CompletableFuture.encodeThrowable(CompletableFuture.java:273)
 at java.util.concurrent.CompletableFuture.completeThrowable(CompletableFuture.java:280)
 at java.util.concurrent.CompletableFuture$AsyncSupply.run(CompletableFuture.java:1592)
 at java.util.concurrent.CompletableFuture$AsyncSupply.exec(CompletableFuture.java:1582)
 at java.util.concurrent.ForkJoinTask.doExec(ForkJoinTask.java:289)
 at java.util.concurrent.ForkJoinPool$WorkQueue.runTask(ForkJoinPool.java:1056)
 at java.util.concurrent.ForkJoinPool.runWorker(ForkJoinPool.java:1692)
 at java.util.concurrent.ForkJoinWorkerThread.run(ForkJoinWorkerThread.java:157)
Caused by: java.lang.RuntimeException
 at cn.eovie.future.FutureWhenTest.lambda$main$0(FutureWhenTest.java:13)
 at java.util.concurrent.CompletableFuture$AsyncSupply.run(CompletableFuture.java:1590)
 ... 5 more
你的程序异常啦
```
<a name="yjGou"></a>
#### 5、`whenComplete`方法
`CompletableFuture`的`whenComplete`方法表示，某个任务执行完成后，执行的回调方法，无返回值；并且`whenComplete`方法返回的`CompletableFuture`的`result`是上个任务的结果。
```java
public class FutureWhenTest {

    public static void main(String[] args) throws ExecutionException, InterruptedException {

        CompletableFuture<String> orgFuture = CompletableFuture.supplyAsync(
                ()->{
                    System.out.println("当前线程名称：" + Thread.currentThread().getName());
                    try {
                        Thread.sleep(2000L);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    return "Hello";
                }
        );

        CompletableFuture<String> rstFuture = orgFuture.whenComplete((a, throwable) -> {
            System.out.println("当前线程名称：" + Thread.currentThread().getName());
            System.out.println("上个任务执行完啦，还把" + a + "传过来");
            if ("Hello".equals(a)) {
                System.out.println("666");
            }
            System.out.println("233333");
        });

        System.out.println(rstFuture.get());
    }
}
//输出
当前线程名称：ForkJoinPool.commonPool-worker-1
当前线程名称：ForkJoinPool.commonPool-worker-1
上个任务执行完啦，还把Hello传过来
666
233333
Hello
```
<a name="KRLP3"></a>
#### 6、`handle`方法
`CompletableFuture`的`handle`方法表示，某个任务执行完成后，执行回调方法，并且是有返回值的；并且`handle`方法返回的`CompletableFuture`的`result`是回调方法执行的结果。
```java
public class FutureHandlerTest {

    public static void main(String[] args) throws ExecutionException, InterruptedException {

        CompletableFuture<String> orgFuture = CompletableFuture.supplyAsync(
                ()->{
                    System.out.println("当前线程名称：" + Thread.currentThread().getName());
                    try {
                        Thread.sleep(2000L);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    return "Hello";
                }
        );

        CompletableFuture<String> rstFuture = orgFuture.handle((a, throwable) -> {

            System.out.println("上个任务执行完啦，还把" + a + "传过来");
            if ("Hello".equals(a)) {
                System.out.println("666");
                return "关注了";
            }
            System.out.println("233333");
            return null;
        });

        System.out.println(rstFuture.get());
    }
}
//输出
当前线程名称：ForkJoinPool.commonPool-worker-1
上个任务执行完啦，还把Hello传过来
666
关注了
```
<a name="DCqLZ"></a>
### 多个任务组合处理
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1622975609608-3bce9b22-5235-47cd-b913-a2b7108e53ac.png#averageHue=%23f6f6f6&clientId=uf417faae-73ad-4&from=paste&id=uce67c027&originHeight=455&originWidth=1080&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=none&taskId=u6c99f74d-12b3-4ef8-9a66-f5a586265e5&title=)
<a name="nQL1E"></a>
#### AND组合关系
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1622975610283-5ea8ef33-1e83-40da-ab78-42ae661113d2.png#averageHue=%23f6f5f4&clientId=uf417faae-73ad-4&from=paste&id=ud5df63ae&originHeight=193&originWidth=648&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=none&taskId=u8d11944d-4ea4-41ac-93b0-3d0b017cb3c&title=)<br />`thenCombine` / `thenAcceptBoth` / `runAfterBoth`都表示：将两个`CompletableFuture`组合起来，只有这两个都正常执行完了，才会执行某个任务。<br />区别在于：

- `thenCombine`：会将两个任务的执行结果作为方法入参，传递到指定方法中，且有返回值
- `thenAcceptBoth`: 会将两个任务的执行结果作为方法入参，传递到指定方法中，且无返回值
- `runAfterBoth `不会把执行结果当做方法入参，且没有返回值。
```java
public class ThenCombineTest {

    public static void main(String[] args) throws InterruptedException, ExecutionException, TimeoutException {

        CompletableFuture<String> first = CompletableFuture.completedFuture("第一个异步任务");
        ExecutorService executor = Executors.newFixedThreadPool(10);
        CompletableFuture<String> future = CompletableFuture
                //第二个异步任务
                .supplyAsync(() -> "第二个异步任务", executor)
                // (w, s) -> System.out.println(s) 是第三个任务
                .thenCombineAsync(first, (s, w) -> {
                    System.out.println(w);
                    System.out.println(s);
                    return "两个异步任务的组合";
                }, executor);
        System.out.println(future.join());
        executor.shutdown();

    }
}
//输出
第一个异步任务
第二个异步任务
两个异步任务的组合
```
<a name="kxA5Y"></a>
#### OR 组合的关系
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1622975610222-4a93d76a-ef22-458e-be8e-7acb9c554811.png#averageHue=%23f6f5f4&clientId=uf417faae-73ad-4&from=paste&id=u44a1b849&originHeight=182&originWidth=613&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=none&taskId=ue025ce9f-d29b-44b9-ae48-0566f12ec4f&title=)<br />`applyToEither` / `acceptEither` / `runAfterEither` 都表示：将两个`CompletableFuture`组合起来，只要其中一个执行完了，就会执行某个任务。<br />区别在于：

- `applyToEither`：会将已经执行完成的任务，作为方法入参，传递到指定方法中，且有返回值
- `acceptEither`: 会将已经执行完成的任务，作为方法入参，传递到指定方法中，且无返回值
- `runAfterEither`：不会把执行结果当做方法入参，且没有返回值。
```java
public class AcceptEitherTest {
    public static void main(String[] args) {
        //第一个异步任务，休眠2秒，保证它执行晚点
        CompletableFuture<String> first = CompletableFuture.supplyAsync(()->{
            try{

                Thread.sleep(2000L);
                System.out.println("执行完第一个异步任务");}
                catch (Exception e){
                    return "第一个任务异常";
                }
            return "第一个异步任务";
        });
        ExecutorService executor = Executors.newSingleThreadExecutor();
        CompletableFuture<Void> future = CompletableFuture
                //第二个异步任务
                .supplyAsync(() -> {
                            System.out.println("执行完第二个任务");
                            return "第二个任务";}
                , executor)
                //第三个任务
                .acceptEitherAsync(first, System.out::println, executor);

        executor.shutdown();
    }
}
//输出
执行完第二个任务
第二个任务
```
<a name="MZI1Q"></a>
### `AllOf`
所有任务都执行完成后，才执行 `allOf`返回的`CompletableFuture`。如果任意一个任务异常，`allOf`的`CompletableFuture`，执行`get`方法，会抛出异常
```java
public class allOfFutureTest {
    public static void main(String[] args) throws ExecutionException, InterruptedException {

        CompletableFuture<Void> a = CompletableFuture.runAsync(()->{
            System.out.println("我执行完了");
        });
        CompletableFuture<Void> b = CompletableFuture.runAsync(() -> {
            System.out.println("我也执行完了");
        });
        CompletableFuture<Void> allOfFuture = CompletableFuture.allOf(a, b).whenComplete((m,k)->{
            System.out.println("finish");
        });
    }
}
//输出
我执行完了
我也执行完了
finish
```
<a name="PAliM"></a>
### `AnyOf`
任意一个任务执行完，就执行`anyOf`返回的`CompletableFuture`。如果执行的任务异常，`anyOf`的`CompletableFuture`，执行`get`方法，会抛出异常
```java
public class AnyOfFutureTest {
    public static void main(String[] args) throws ExecutionException, InterruptedException {

        CompletableFuture<Void> a = CompletableFuture.runAsync(()->{
            try {
                Thread.sleep(3000L);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println("我执行完了");
        });
        CompletableFuture<Void> b = CompletableFuture.runAsync(() -> {
            System.out.println("我也执行完了");
        });
        CompletableFuture<Object> anyOfFuture = CompletableFuture.anyOf(a, b).whenComplete((m,k)->{
            System.out.println("finish");
//            return "Hello";
        });
        anyOfFuture.join();
    }
}
//输出
我也执行完了
finish
```
<a name="NdGR2"></a>
### `thenCompose`
`thenCompose`方法会在某个任务执行完成后，将该任务的执行结果，作为方法入参，去执行指定的方法。该方法会返回一个新的`CompletableFuture`实例

- 如果该`CompletableFuture`实例的`result`不为null，则返回一个基于该`result`新的`CompletableFuture`实例；
- 如果该`CompletableFuture`实例为`null`，然后就执行这个新任务
```java
public class ThenComposeTest {
    public static void main(String[] args) throws ExecutionException, InterruptedException {

        CompletableFuture<String> f = CompletableFuture.completedFuture("第一个任务");
        //第二个异步任务
        ExecutorService executor = Executors.newSingleThreadExecutor();
        CompletableFuture<String> future = CompletableFuture
                .supplyAsync(() -> "第二个任务", executor)
                .thenComposeAsync(data -> {
                    System.out.println(data); return f; //使用第一个任务作为返回
                }, executor);
        System.out.println(future.join());
        executor.shutdown();

    }
}
//输出
第二个任务
第一个任务
```
<a name="jitAy"></a>
## `CompletableFuture`使用有哪些注意点
`CompletableFuture` 使异步编程更加便利的、代码更加优雅的同时，也要关注下它的一些注意点。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1622975610518-61437457-43d0-4031-8f33-3e7696e57847.png#averageHue=%23f6f6f5&clientId=uf417faae-73ad-4&from=paste&id=ub8acc50d&originHeight=257&originWidth=1021&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=none&taskId=u029b173e-d34c-4fa1-beb3-dae3c7b020e&title=)
<a name="OS2RG"></a>
### 1、`Future`需要获取返回值，才能获取异常信息
```java
ExecutorService executorService = new ThreadPoolExecutor(5, 10, 5L,
    TimeUnit.SECONDS, new ArrayBlockingQueue<>(10));
CompletableFuture<Void> future = CompletableFuture.supplyAsync(() -> {
      int a = 0;
      int b = 666;
      int c = b / a;
      return true;
   },executorService).thenAccept(System.out::println);
   
 //如果不加 get()方法这一行，看不到异常信息
 //future.get();
```
`Future`需要获取返回值，才能获取到异常信息。如果不加 `get()`/`join()`方法，看不到异常信息。使用的时候，注意一下，考虑是否加`try...catch...`或者使用`exceptionally`方法。
<a name="WXNpC"></a>
### 2、`CompletableFuture`的`get()`方法是阻塞的。
`CompletableFuture`的`get()`方法是阻塞的，如果使用它来获取异步调用的返回值，需要添加超时时间~
```java
//反例
CompletableFuture.get();
//正例
CompletableFuture.get(5, TimeUnit.SECONDS);
```
<a name="C7bCH"></a>
### 3、默认线程池的注意点
`CompletableFuture`代码中又使用了默认的线程池，处理的线程个数是电脑CPU核数-1。在大量请求过来的时候，处理逻辑复杂的话，响应会很慢。一般建议使用自定义线程池，优化线程池配置参数。
<a name="X0B10"></a>
### 4、自定义线程池时，注意饱和策略
`CompletableFuture`的`get()`方法是阻塞的，一般建议使用`future.get(3, TimeUnit.SECONDS)`。并且一般建议使用自定义线程池。<br />但是如果线程池拒绝策略是`DiscardPolicy`或者`DiscardOldestPolicy`，当线程池饱和时，会直接丢弃任务，不会抛弃异常。因此建议，`CompletableFuture`线程池策略最好使用`AbortPolicy`，然后耗时的异步线程，做好线程池隔离。
