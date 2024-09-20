Java SpringBoot<br />使用`@Async`注解来创建异步任务，可以用这种方法来实现一些并发操作，以加速任务的执行效率。但是，如果只是那样直接简单的创建来使用，可能还是会碰到一些问题。存在有什么问题呢？先来思考下，下面的这个接口，通过异步任务加速执行的实现，是否存在问题或风险呢？
```java
@RestController
public class HelloController {

    @Autowired
    private AsyncTasks asyncTasks;
        
    @GetMapping("/hello")
    public String hello() {
        // 将可以并行的处理逻辑，拆分成三个异步任务同时执行
        CompletableFuture<String> task1 = asyncTasks.doTaskOne();
        CompletableFuture<String> task2 = asyncTasks.doTaskTwo();
        CompletableFuture<String> task3 = asyncTasks.doTaskThree();
        
        CompletableFuture.allOf(task1, task2, task3).join();
        return "Hello World";
    }
}
```
虽然，从单次接口调用来说，是没有问题的。但当接口被客户端频繁调用的时候，异步任务的数量就会大量增长：3 x n（n为请求数量），如果任务处理不够快，就很可能会出现内存溢出的情况。那么为什么会内存溢出呢？根本原因是由于Spring Boot默认用于异步任务的线程池是这样配置的：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1631926883725-1b41314b-e0b8-48c9-be60-c69811cd2fd7.webp#clientId=u0098bf08-1c14-4&from=paste&id=u5d75c334&originHeight=560&originWidth=790&originalType=url&ratio=1&status=done&style=none&taskId=u7970eb82-fe77-4330-bf13-872637a3c6b)<br />图中标出的两个重要参数是需要关注的：

- `queueCapacity`：缓冲队列的容量，默认为INT的最大值（2的31次方-1）。
- `maxSize`：允许的最大线程数，默认为INT的最大值（2的31次方-1）。

所以，默认情况下，一般任务队列就可能把内存给堆满了。所以，真正使用的时候，还需要对异步任务的执行线程池做一些基础配置，以防止出现内存溢出导致服务不可用的问题。
<a name="FXhFK"></a>
### 配置默认线程池
默认线程池的配置很简单，只需要在配置文件中完成即可，主要有以下这些参数：
```java
spring.task.execution.pool.core-size=2
spring.task.execution.pool.max-size=5
spring.task.execution.pool.queue-capacity=10
spring.task.execution.pool.keep-alive=60s
spring.task.execution.pool.allow-core-thread-timeout=true
spring.task.execution.shutdown.await-termination=false
spring.task.execution.shutdown.await-termination-period=
spring.task.execution.thread-name-prefix=task-
```
具体配置含义如下：

- `spring.task.execution.pool.core-size`：线程池创建时的初始化线程数，默认为8
- `spring.task.execution.pool.max-size`：线程池的最大线程数，默认为int最大值
- `spring.task.execution.pool.queue-capacity`：用来缓冲执行任务的队列，默认为int最大值
- `spring.task.execution.pool.keep-alive`：线程终止前允许保持空闲的时间
- `spring.task.execution.pool.allow-core-thread-timeout`：是否允许核心线程超时
- `spring.task.execution.shutdown.await-termination`：是否等待剩余任务完成后才关闭应用
- `spring.task.execution.shutdown.await-termination-period`：等待剩余任务完成的最大时间
- `spring.task.execution.thread-name-prefix`：线程名的前缀，设置好了之后可以方便在日志中查看处理任务所在的线程池
<a name="uz62C"></a>
### 动手试一试
首先，在没有进行线程池配置之前，可以先执行一下单元测试：
```java
@Test
public void test1() throws Exception {
    long start = System.currentTimeMillis();

    CompletableFuture<String> task1 = asyncTasks.doTaskOne();
    CompletableFuture<String> task2 = asyncTasks.doTaskTwo();
    CompletableFuture<String> task3 = asyncTasks.doTaskThree();

    CompletableFuture.allOf(task1, task2, task3).join();

    long end = System.currentTimeMillis();

    log.info("任务全部完成，总耗时：" + (end - start) + "毫秒");
}
```
由于默认线程池的核心线程数是8，所以3个任务会同时开始执行，日志输出是这样的：
```
2021-09-15 00:30:14.819  INFO 77614 --- [         task-2] com.didispace.chapter76.AsyncTasks       : 开始做任务二
2021-09-15 00:30:14.819  INFO 77614 --- [         task-3] com.didispace.chapter76.AsyncTasks       : 开始做任务三
2021-09-15 00:30:14.819  INFO 77614 --- [         task-1] com.didispace.chapter76.AsyncTasks       : 开始做任务一
2021-09-15 00:30:15.491  INFO 77614 --- [         task-2] com.didispace.chapter76.AsyncTasks       : 完成任务二，耗时：672毫秒
2021-09-15 00:30:19.496  INFO 77614 --- [         task-3] com.didispace.chapter76.AsyncTasks       : 完成任务三，耗时：4677毫秒
2021-09-15 00:30:20.443  INFO 77614 --- [         task-1] com.didispace.chapter76.AsyncTasks       : 完成任务一，耗时：5624毫秒
2021-09-15 00:30:20.443  INFO 77614 --- [           main] c.d.chapter76.Chapter76ApplicationTests  : 任务全部完成，总耗时：5653毫秒
```
接着，可以尝试在配置文件中增加如下的线程池配置
```
spring.task.execution.pool.core-size=2
spring.task.execution.pool.max-size=5
spring.task.execution.pool.queue-capacity=10
spring.task.execution.pool.keep-alive=60s
spring.task.execution.pool.allow-core-thread-timeout=true
spring.task.execution.thread-name-prefix=task-
```
日志输出的顺序会变成如下的顺序：
```
2021-09-15 00:31:50.013  INFO 77985 --- [         task-1] com.didispace.chapter76.AsyncTasks       : 开始做任务一
2021-09-15 00:31:50.013  INFO 77985 --- [         task-2] com.didispace.chapter76.AsyncTasks       : 开始做任务二
2021-09-15 00:31:52.452  INFO 77985 --- [         task-1] com.didispace.chapter76.AsyncTasks       : 完成任务一，耗时：2439毫秒
2021-09-15 00:31:52.452  INFO 77985 --- [         task-1] com.didispace.chapter76.AsyncTasks       : 开始做任务三
2021-09-15 00:31:55.880  INFO 77985 --- [         task-2] com.didispace.chapter76.AsyncTasks       : 完成任务二，耗时：5867毫秒
2021-09-15 00:32:00.346  INFO 77985 --- [         task-1] com.didispace.chapter76.AsyncTasks       : 完成任务三，耗时：7894毫秒
2021-09-15 00:32:00.347  INFO 77985 --- [           main] c.d.chapter76.Chapter76ApplicationTests  : 任务全部完成，总耗时：10363毫秒
```

- 任务一和任务二会马上占用核心线程，任务三进入队列等待
- 任务一完成，释放出一个核心线程，任务三从队列中移出，并占用核心线程开始处理
