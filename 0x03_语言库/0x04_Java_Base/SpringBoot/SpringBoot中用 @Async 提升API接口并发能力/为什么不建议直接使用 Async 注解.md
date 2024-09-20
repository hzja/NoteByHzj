Java<br />对于异步方法调用，从Spring3开始提供了`@Async`注解，该注解可以被标在方法上，以便异步地调用该方法。调用者将在调用时立即返回，方法的实际执行将提交给Spring TaskExecutor的任务中，由指定的线程池中的线程执行。<br />在项目应用中，`@Async`调用线程池，推荐使用自定义线程池的模式。自定义线程池常用方案：重新实现接口`AsyncConfigurer`。
<a name="PsoYk"></a>
## 应用场景
<a name="GzItz"></a>
### 同步
同步就是整个处理过程顺序执行，当各个过程都执行完毕，并返回结果。
<a name="D5T1B"></a>
### 异步
异步调用则是只是发送了调用的指令，调用者无需等待被调用的方法完全执行完毕；而是继续执行下面的流程。<br />例如， 在某个调用中，需要顺序调用 A, B, C三个过程方法；如他们都是同步调用，则需要将他们都顺序执行完毕之后，方算作过程执行完毕；如B为一个异步的调用方法，则在执行完A之后，调用B，并不等待B完成，而是执行开始调用C，待C执行完毕之后，就意味着这个过程执行完毕了。<br />在Java中，一般在处理类似的场景之时，都是基于创建独立的线程去完成相应的异步调用逻辑，通过主线程和不同的业务子线程之间的执行流程，从而在启动独立的线程之后，主线程继续执行而不会产生停滞等待的情况。
<a name="sWKqM"></a>
## Spring 已经实现的线程池

- `SimpleAsyncTaskExecutor`：不是真的线程池，这个类不重用线程，默认每次调用都会创建一个新的线程。
- `SyncTaskExecutor`：这个类没有实现异步调用，只是一个同步操作。只适用于不需要多线程的地方。
- `ConcurrentTaskExecutor`：`Executor`的适配类，不推荐使用。如果`ThreadPoolTaskExecutor`不满足要求时，才用考虑使用这个类。
- `SimpleThreadPoolTaskExecutor`：是`Quartz`的`SimpleThreadPool`的类。线程池同时被quartz和非quartz使用，才需要使用此类。
- `ThreadPoolTaskExecutor `：最常使用，推荐。其实质是对`java.util.concurrent.ThreadPoolExecutor`的包装。

异步的方法有

- 最简单的异步调用，返回值为`void`
- 带参数的异步调用，异步方法可以传入参数
- 存在返回值，常调用返回`Future`
<a name="qF0xk"></a>
## Spring中启用`@Async`
![2021-09-07-10-52-13-189521.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630983407881-79374cb2-de2b-49c2-81ab-e69e638b956b.jpeg#clientId=u2575e060-0c09-4&from=ui&id=u6147b3f9&originHeight=640&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=99912&status=done&style=none&taskId=u99099890-e257-44df-a42d-65ee154496a&title=)
```java
// 基于Java配置的启用方式：
@Configuration
@EnableAsync
public class SpringAsyncConfig { ... }

// Spring boot启用：
@EnableAsync
@EnableTransactionManagement
public class SettlementApplication {
    public static void main(String[] args) {
        SpringApplication.run(SettlementApplication.class, args);
    }
}
```
<a name="vuAf8"></a>
## `@Async`应用默认线程池
Spring应用默认的线程池，指在`@Async`注解在使用时，不指定线程池的名称。查看源码，`@Async`的默认线程池为`SimpleAsyncTaskExecutor`。
<a name="V8K0p"></a>
### 无返回值调用
基于`@Async`无返回值调用，直接在使用类，使用方法（建议在使用方法）上，加上注解。若需要抛出异常，需手动new一个异常抛出。<br />![2021-09-07-10-52-13-273840.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630983407819-be19e938-52d2-48a2-af50-3e73e88fce1e.jpeg#clientId=u2575e060-0c09-4&from=ui&id=muNzq&originHeight=513&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=94172&status=done&style=none&taskId=ub735c628-0bbb-4b88-b730-b9f692af931&title=)
```java
/**
 * 带参数的异步调用 异步方法可以传入参数
 *  对于返回值是void，异常会被AsyncUncaughtExceptionHandler处理掉
 * @param s
 */
@Async
public void asyncInvokeWithException(String s) {
    log.info("asyncInvokeWithParameter, parementer={}", s);
    throw new IllegalArgumentException(s);
}
```
<a name="lbeO2"></a>
### 有返回值`Future`调用
![2021-09-07-10-52-13-374842.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630983407836-7637a291-fecf-45df-8394-101b37c59677.jpeg#clientId=u2575e060-0c09-4&from=ui&id=A4lQi&originHeight=750&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=173712&status=done&style=none&taskId=ua83c7eab-2c04-4eb3-b12c-7b794a2bb3f&title=)
```java
/**
 *  异常调用返回Future
 *  对于返回值是Future，不会被AsyncUncaughtExceptionHandler处理，需要我们在方法中捕获异常并处理
 *  或者在调用方在调用Futrue.get时捕获异常进行处理
 *
 * @param i
 * @return
 */
@Async
public Future<String> asyncInvokeReturnFuture(int i) {
    log.info("asyncInvokeReturnFuture, parementer={}", i);
    Future<String> future;
    try {
        Thread.sleep(1000 * 1);
        future = new AsyncResult<String>("success:" + i);
        throw new IllegalArgumentException("a");
    } catch (InterruptedException e) {
        future = new AsyncResult<String>("error");
    } catch(IllegalArgumentException e){
        future = new AsyncResult<String>("error-IllegalArgumentException");
    }
    return future;
}
```
<a name="Rt2Sw"></a>
### 有返回值`CompletableFuture`调用
`CompletableFuture `并不使用`@Async`注解，可达到调用系统线程池处理业务的功能。<br />JDK5新增了`Future`接口，用于描述一个异步计算的结果。虽然 Future 以及相关使用方法提供了异步执行任务的能力，但是对于结果的获取却是很不方便，只能通过阻塞或者轮询的方式得到任务的结果。阻塞的方式显然和异步编程的初衷相违背，轮询的方式又会耗费无谓的 CPU 资源，而且也不能及时地得到计算结果。

- `CompletionStage`代表异步计算过程中的某一个阶段，一个阶段完成以后可能会触发另外一个阶段
- 一个阶段的计算执行可以是一个`Function`，`Consumer`或者`Runnable`。比如：
```java
stage.thenApply(x -> square(x)).thenAccept(x -> System.out.print(x)).thenRun(() -> System.out.println())
```

- 一个阶段的执行可能是被单个阶段的完成触发，也可能是由多个阶段一起触发

在Java8中，`CompletableFuture` 提供了非常强大的`Future`的扩展功能，可以简化异步编程的复杂性，并且提供了函数式编程的能力，可以通过回调的方式处理计算结果，也提供了转换和组合 `CompletableFuture` 的方法。

- 它可能代表一个明确完成的Future，也有可能代表一个完成阶段（ CompletionStage ），它支持在计算完成以后触发一些函数或执行某些动作。
- 它实现了`Future`和`CompletionStage`接口

![2021-09-07-10-52-13-464880.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630983452826-4754ce2a-7820-4da9-bcca-214b8c873e06.jpeg#clientId=u2575e060-0c09-4&from=ui&id=ube5b08f1&originHeight=567&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=140781&status=done&style=none&taskId=ud869759b-534d-4e6c-b743-05bfc548259&title=)
```java
/**
 * 数据查询线程池
 */
private static final ThreadPoolExecutor SELECT_POOL_EXECUTOR = new ThreadPoolExecutor(10, 20, 5000,
        TimeUnit.MILLISECONDS, new LinkedBlockingQueue<>(1024), new ThreadFactoryBuilder().setNameFormat("selectThreadPoolExecutor-%d").build());

// tradeMapper.countTradeLog(tradeSearchBean)方法表示，获取数量，返回值为int
// 获取总条数
CompletableFuture<Integer> countFuture = CompletableFuture
        .supplyAsync(() -> tradeMapper.countTradeLog(tradeSearchBean), SELECT_POOL_EXECUTOR);
// 同步阻塞
CompletableFuture.allOf(countFuture).join();
// 获取结果
int count = countFuture.get();
```
<a name="BsJtX"></a>
### 默认线程池的弊端
在线程池应用中，参考阿里巴巴java开发规范：线程池不允许使用`Executors`去创建，不允许使用系统默认的线程池，推荐通过`ThreadPoolExecutor`的方式，这样的处理方式让开发的工程师更加明确线程池的运行规则，规避资源耗尽的风险。<br />`Executors`各个方法的弊端：

- `newFixedThreadPool`和`newSingleThreadExecutor`：主要问题是堆积的请求处理队列可能会耗费非常大的内存，甚至OOM。
- `newCachedThreadPool`和`newScheduledThreadPool`：要问题是线程数最大数是`Integer.MAX_VALUE`，可能会创建数量非常多的线程，甚至OOM。

`@Async`默认异步配置使用的是`SimpleAsyncTaskExecutor`，该线程池默认来一个任务创建一个线程，若系统中不断的创建线程，最终会导致系统占用内存过高，引发`OutOfMemoryError`错误。针对线程创建问题，`SimpleAsyncTaskExecutor`提供了限流机制，通过`concurrencyLimit`属性来控制开关，当`concurrencyLimit>=0`时开启限流机制，默认关闭限流机制即`concurrencyLimit=-1`，当关闭情况下，会不断创建新的线程来处理任务。基于默认配置，`SimpleAsyncTaskExecutor`并不是严格意义的线程池，达不到线程复用的功能。
<a name="O9dQk"></a>
## `@Async`应用自定义线程池
自定义线程池，可对系统中线程池更加细粒度的控制，方便调整线程池大小配置，线程执行异常控制和处理。在设置系统自定义线程池代替默认线程池时，虽可通过多种模式设置，但替换默认线程池最终产生的线程池有且只能设置一个（不能设置多个类继承`AsyncConfigurer`）自定义线程池有如下模式：

- 重新实现接口`AsyncConfigurer`
- 继承`AsyncConfigurerSupport`
- 配置由自定义的`TaskExecutor`替代内置的任务执行器

通过查看Spring源码关于`@Async`的默认调用规则，会优先查询源码中实现`AsyncConfigurer`这个接口的类，实现这个接口的类为`AsyncConfigurerSupport`。但默认配置的线程池和异步处理方法均为空，所以，无论是继承或者重新实现接口，都需指定一个线程池。且重新实现 `public Executor getAsyncExecutor()`方法。
<a name="jaElj"></a>
### 实现接口`AsyncConfigurer`
![2021-09-07-10-52-13-644845.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630983452909-7bcf9db1-5a5c-49a7-a87a-5593db1e1224.jpeg#clientId=u2575e060-0c09-4&from=ui&id=XjDly&originHeight=1220&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=307998&status=done&style=none&taskId=u39d90380-8a1a-4291-b551-831f8c08793&title=)
```java
@Configuration
public class AsyncConfiguration implements AsyncConfigurer {
    @Bean("kingAsyncExecutor")
    public ThreadPoolTaskExecutor executor() {
        ThreadPoolTaskExecutor executor = new ThreadPoolTaskExecutor();
        int corePoolSize = 10;
        executor.setCorePoolSize(corePoolSize);
        int maxPoolSize = 50;
        executor.setMaxPoolSize(maxPoolSize);
        int queueCapacity = 10;
        executor.setQueueCapacity(queueCapacity);
        executor.setRejectedExecutionHandler(new ThreadPoolExecutor.CallerRunsPolicy());
        String threadNamePrefix = "kingDeeAsyncExecutor-";
        executor.setThreadNamePrefix(threadNamePrefix);
        executor.setWaitForTasksToCompleteOnShutdown(true);
        // 使用自定义的跨线程的请求级别线程工厂类19         int awaitTerminationSeconds = 5;
        executor.setAwaitTerminationSeconds(awaitTerminationSeconds);
        executor.initialize();
        return executor;
    }

    @Override
    public Executor getAsyncExecutor() {
        return executor();
    }

    @Override
    public AsyncUncaughtExceptionHandler getAsyncUncaughtExceptionHandler() {
        return (ex, method, params) -> ErrorLogger.getInstance().log(String.format("执行异步任务'%s'", method), ex);
    }
}
```
<a name="NgoBX"></a>
### 继承`AsyncConfigurerSupport`
![2021-09-07-10-52-13-745876.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630983452876-9cbc89e8-71da-4fa5-8b95-a831aa343a87.jpeg#clientId=u2575e060-0c09-4&from=ui&id=jN8za&originHeight=892&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=195041&status=done&style=none&taskId=u026f1c84-2779-4ef3-9a89-86eace27bd4&title=)
```java
@Configuration  
@EnableAsync  
class SpringAsyncConfigurer extends AsyncConfigurerSupport {  

    @Bean  
    public ThreadPoolTaskExecutor asyncExecutor() {  
        ThreadPoolTaskExecutor threadPool = new ThreadPoolTaskExecutor();  
        threadPool.setCorePoolSize(3);  
        threadPool.setMaxPoolSize(3);  
        threadPool.setWaitForTasksToCompleteOnShutdown(true);  
        threadPool.setAwaitTerminationSeconds(60 * 15);  
        return threadPool;  
    }  

    @Override  
    public Executor getAsyncExecutor() {  
        return asyncExecutor;  
    }  

    @Override  
    public AsyncUncaughtExceptionHandler getAsyncUncaughtExceptionHandler() {
        return (ex, method, params) -> ErrorLogger.getInstance().log(String.format("执行异步任务'%s'", method), ex);
    }
}
```
<a name="AHNbI"></a>
### 配置自定义的TaskExecutor
由于AsyncConfigurer的默认线程池在源码中为空，Spring通过`beanFactory.getBean(TaskExecutor.class)`先查看是否有线程池，未配置时，通过`beanFactory.getBean(DEFAULT_TASK_EXECUTOR_BEAN_NAME, Executor.class)`，又查询是否存在默认名称为`TaskExecutor`的线程池。所以可在项目中，定义名称为`TaskExecutor`的bean生成一个默认线程池。也可不指定线程池的名称，申明一个线程池，本身底层是基于TaskExecutor.class便可。<br />比如：
```java
Executor.class:ThreadPoolExecutorAdapter->ThreadPoolExecutor->AbstractExecutorService->ExecutorService->Executor
```
这样的模式，最终底层为Executor.class，在替换默认的线程池时，需设置默认的线程池名称为`TaskExecutor`
```java
TaskExecutor.class:ThreadPoolTaskExecutor->SchedulingTaskExecutor->AsyncTaskExecutor->TaskExecutor
```
这样的模式，最终底层为TaskExecutor.class，在替换默认的线程池时，可不指定线程池名称。<br />![2021-09-07-10-52-13-847841.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630983452919-f0865462-bd9a-4bdc-88aa-f47b883016c8.jpeg#clientId=u2575e060-0c09-4&from=ui&id=xmKmQ&originHeight=1326&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=303107&status=done&style=none&taskId=uf2fcee10-332b-443b-88f7-d92c3a604c2&title=)
```java
@EnableAsync
@Configuration
public class TaskPoolConfig {
    @Bean(name = AsyncExecutionAspectSupport.DEFAULT_TASK_EXECUTOR_BEAN_NAME)
    public Executor taskExecutor() {
        ThreadPoolTaskExecutor executor = new ThreadPoolTaskExecutor();
        //核心线程池大小
        executor.setCorePoolSize(10);
        //最大线程数
        executor.setMaxPoolSize(20);
        //队列容量
        executor.setQueueCapacity(200);
        //活跃时间
        executor.setKeepAliveSeconds(60);
        //线程名字前缀
        executor.setThreadNamePrefix("taskExecutor-");
        executor.setRejectedExecutionHandler(new ThreadPoolExecutor.CallerRunsPolicy());
        return executor;
    }
    @Bean(name = "new_task")
    public Executor taskExecutor() {
        ThreadPoolTaskExecutor executor = new ThreadPoolTaskExecutor();
        //核心线程池大小
        executor.setCorePoolSize(10);
        //最大线程数
        executor.setMaxPoolSize(20);
        //队列容量
        executor.setQueueCapacity(200);
        //活跃时间
        executor.setKeepAliveSeconds(60);
        //线程名字前缀
        executor.setThreadNamePrefix("taskExecutor-");
        executor.setRejectedExecutionHandler(new ThreadPoolExecutor.CallerRunsPolicy());
        return executor;
    }
}
```
<a name="evUJ9"></a>
### 多个线程池
`@Async`注解，使用系统默认或者自定义的线程池（代替默认线程池）。可在项目中设置多个线程池，在异步调用时，指明需要调用的线程池名称，如`@Async("new_task")`。
<a name="LORml"></a>
## `@Async`部分重要源码解析
源码-获取线程池方法<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1636613126573-70a58822-04c2-48b5-96f8-a305bfafc3e5.png#clientId=u246de597-052d-4&from=paste&height=427&id=u86c5d6c0&originHeight=1280&originWidth=3702&originalType=binary&ratio=1&rotation=0&showTitle=false&size=420643&status=done&style=shadow&taskId=u338ab52e-2d4f-4e1c-9e4b-bbcfd74ee65&title=&width=1234)<br />源码-设置默认线程池`defaultExecutor`，默认是空的，当重新实现接口`AsyncConfigurer`的`getAsyncExecutor()`时，可以设置默认的线程池。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1636613168294-8e429347-097e-4a44-92fb-d63d59e4e0d4.png#clientId=u246de597-052d-4&from=paste&height=231&id=ua44bca59&originHeight=692&originWidth=2946&originalType=binary&ratio=1&rotation=0&showTitle=false&size=139702&status=done&style=shadow&taskId=u0b1e842d-463c-4283-8ec6-01161df1445&title=&width=982)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1636613359935-67bf0618-1b12-4c2b-9b65-fd3ee7bf6122.png#clientId=u246de597-052d-4&from=paste&height=571&id=u5971136d&originHeight=1714&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=700764&status=done&style=none&taskId=u32685130-225c-4ab9-9246-3eb81e89ec5&title=&width=1280)<br />源码-都没有找到项目中设置的默认线程池时，采用Spring 默认的线程池<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1636613583430-83682939-6fc8-4309-9f72-a714ac0e2ccf.png#clientId=u246de597-052d-4&from=paste&height=97&id=u1b0266ca&originHeight=292&originWidth=3655&originalType=binary&ratio=1&rotation=0&showTitle=false&size=110721&status=done&style=shadow&taskId=u4578327c-1929-45b9-bb1c-89f943b1fc7&title=&width=1218.3333333333333)
