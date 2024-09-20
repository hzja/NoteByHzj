Java SpringBoot @Async<br />异步任务的执行背后有一个线程池来管理执行任务。为了控制异步任务的并发不影响到应用的正常运作，必须要对线程池做好相应的配置，防止资源的过渡使用。除了默认线程池的配置之外，还有一类场景，也是很常见的，那就是多任务情况下的线程池隔离。
<a name="Zbj5e"></a>
### 什么是线程池的隔离，为什么要隔离
先来看看下面的场景案例：
```java
@RestController
public class HelloController {

    @Autowired
    private AsyncTasks asyncTasks;
        
    @GetMapping("/api-1")
    public String taskOne() {
        CompletableFuture<String> task1 = asyncTasks.doTaskOne("1");
        CompletableFuture<String> task2 = asyncTasks.doTaskOne("2");
        CompletableFuture<String> task3 = asyncTasks.doTaskOne("3");
        
        CompletableFuture.allOf(task1, task2, task3).join();
        return "";
    }
    
    @GetMapping("/api-2")
    public String taskTwo() {
        CompletableFuture<String> task1 = asyncTasks.doTaskTwo("1");
        CompletableFuture<String> task2 = asyncTasks.doTaskTwo("2");
        CompletableFuture<String> task3 = asyncTasks.doTaskTwo("3");
        
        CompletableFuture.allOf(task1, task2, task3).join();
        return "";
    }
    
}
```
上面的代码中，有两个API接口，这两个接口的具体执行逻辑中都会把执行过程拆分为三个异步任务来实现。<br />好了，思考一分钟，想一下。如果这样实现，会有什么问题吗？

---

上面这段代码，在API请求并发不高，同时如果每个任务的处理速度也够快的时候，是没有问题的。但如果并发上来或其中某几个处理过程扯后腿了的时候。这两个提供不相干服务的接口可能会互相影响。比如：假设当前线程池配置的最大线程数有2个，这个时候/api-1接口中task1和task2处理速度很慢，阻塞了；那么此时，当用户调用api-2接口的时候，这个服务也会阻塞！<br />造成这种现场的原因是：默认情况下，所有用`@Async`创建的异步任务都是共用的一个线程池，所以当有一些异步任务碰到性能问题的时候，是会直接影响其他异步任务的。<br />为了解决这个问题，就需要对异步任务做一定的线程池隔离，让不同的异步任务互不影响。
<a name="n6eKh"></a>
### 不同异步任务配置不同线程池
下面，就来实际操作一下！
<a name="Yq5qv"></a>
#### 第一步：初始化多个线程池
比如下面这样：
```java
@EnableAsync
@Configuration
public class TaskPoolConfig {

    @Bean
    public Executor taskExecutor1() {
        ThreadPoolTaskExecutor executor = new ThreadPoolTaskExecutor();
        executor.setCorePoolSize(2);
        executor.setMaxPoolSize(2);
        executor.setQueueCapacity(10);
        executor.setKeepAliveSeconds(60);
        executor.setThreadNamePrefix("executor-1-");
        executor.setRejectedExecutionHandler(new ThreadPoolExecutor.CallerRunsPolicy());
        return executor;
    }

    @Bean
    public Executor taskExecutor2() {
        ThreadPoolTaskExecutor executor = new ThreadPoolTaskExecutor();
        executor.setCorePoolSize(2);
        executor.setMaxPoolSize(2);
        executor.setQueueCapacity(10);
        executor.setKeepAliveSeconds(60);
        executor.setThreadNamePrefix("executor-2-");
        executor.setRejectedExecutionHandler(new ThreadPoolExecutor.CallerRunsPolicy());
        return executor;
    }
}
```
注意：这里特地用`executor.setThreadNamePrefix`设置了线程名的前缀，这样可以方便观察后面具体执行的顺序。
<a name="lp7Hm"></a>
#### 第二步：创建异步任务，并指定要使用的线程池名称
```java
@Slf4j
@Component
public class AsyncTasks {

    public static Random random = new Random();

    @Async("taskExecutor1")
    public CompletableFuture<String> doTaskOne(String taskNo) throws Exception {
        log.info("开始任务：{}", taskNo);
        long start = System.currentTimeMillis();
        Thread.sleep(random.nextInt(10000));
        long end = System.currentTimeMillis();
        log.info("完成任务：{}，耗时：{} 毫秒", taskNo, end - start);
        return CompletableFuture.completedFuture("任务完成");
    }

    @Async("taskExecutor2")
    public CompletableFuture<String> doTaskTwo(String taskNo) throws Exception {
        log.info("开始任务：{}", taskNo);
        long start = System.currentTimeMillis();
        Thread.sleep(random.nextInt(10000));
        long end = System.currentTimeMillis();
        log.info("完成任务：{}，耗时：{} 毫秒", taskNo, end - start);
        return CompletableFuture.completedFuture("任务完成");
    }

}
```
这里`@Async`注解中定义的`taskExecutor1`和`taskExecutor2`就是线程池的名字。由于在第一步中，没有具体写两个线程池Bean的名称，所以默认会使用方法名，也就是`taskExecutor1`和`taskExecutor2`。
<a name="QONBr"></a>
#### 第三步：写个单元测试来验证下
比如下面这样：
```java
@Slf4j
@SpringBootTest
public class Chapter77ApplicationTests {

    @Autowired
    private AsyncTasks asyncTasks;

    @Test
    public void test() throws Exception {
        long start = System.currentTimeMillis();

        // 线程池1
        CompletableFuture<String> task1 = asyncTasks.doTaskOne("1");
        CompletableFuture<String> task2 = asyncTasks.doTaskOne("2");
        CompletableFuture<String> task3 = asyncTasks.doTaskOne("3");

        // 线程池2
        CompletableFuture<String> task4 = asyncTasks.doTaskTwo("4");
        CompletableFuture<String> task5 = asyncTasks.doTaskTwo("5");
        CompletableFuture<String> task6 = asyncTasks.doTaskTwo("6");

        // 一起执行
        CompletableFuture.allOf(task1, task2, task3, task4, task5, task6).join();

        long end = System.currentTimeMillis();

        log.info("任务全部完成，总耗时：" + (end - start) + "毫秒");
    }

}
```
在上面的单元测试中，一共启动了6个异步任务，前三个用的是线程池1，后三个用的是线程池2。<br />先不执行，根据设置的核心线程2和最大线程数2，来分析一下，大概会是怎么样的执行情况？

1. 线程池1的三个任务，task1和task2会先获得执行线程，然后task3因为没有可分配线程进入缓冲队列
2. 线程池2的三个任务，task4和task5会先获得执行线程，然后task6因为没有可分配线程进入缓冲队列
3. 任务task3会在task1或task2完成之后，开始执行
4. 任务task6会在task4或task5完成之后，开始执行

分析好之后，执行下单元测试，看看是否是这样的：
```
2021-09-15 23:45:11.369  INFO 61670 --- [   executor-1-1] com.didispace.chapter77.AsyncTasks       : 开始任务：1
2021-09-15 23:45:11.369  INFO 61670 --- [   executor-2-2] com.didispace.chapter77.AsyncTasks       : 开始任务：5
2021-09-15 23:45:11.369  INFO 61670 --- [   executor-2-1] com.didispace.chapter77.AsyncTasks       : 开始任务：4
2021-09-15 23:45:11.369  INFO 61670 --- [   executor-1-2] com.didispace.chapter77.AsyncTasks       : 开始任务：2
2021-09-15 23:45:15.905  INFO 61670 --- [   executor-2-1] com.didispace.chapter77.AsyncTasks       : 完成任务：4，耗时：4532 毫秒
2021-09-15 23:45:15.905  INFO 61670 --- [   executor-2-1] com.didispace.chapter77.AsyncTasks       : 开始任务：6
2021-09-15 23:45:18.263  INFO 61670 --- [   executor-1-2] com.didispace.chapter77.AsyncTasks       : 完成任务：2，耗时：6890 毫秒
2021-09-15 23:45:18.263  INFO 61670 --- [   executor-1-2] com.didispace.chapter77.AsyncTasks       : 开始任务：3
2021-09-15 23:45:18.896  INFO 61670 --- [   executor-2-2] com.didispace.chapter77.AsyncTasks       : 完成任务：5，耗时：7523 毫秒
2021-09-15 23:45:19.842  INFO 61670 --- [   executor-1-2] com.didispace.chapter77.AsyncTasks       : 完成任务：3，耗时：1579 毫秒
2021-09-15 23:45:20.551  INFO 61670 --- [   executor-1-1] com.didispace.chapter77.AsyncTasks       : 完成任务：1，耗时：9178 毫秒
2021-09-15 23:45:24.117  INFO 61670 --- [   executor-2-1] com.didispace.chapter77.AsyncTasks       : 完成任务：6，耗时：8212 毫秒
2021-09-15 23:45:24.117  INFO 61670 --- [           main] c.d.chapter77.Chapter77ApplicationTests  : 任务全部完成，总耗时：12762毫秒
```
