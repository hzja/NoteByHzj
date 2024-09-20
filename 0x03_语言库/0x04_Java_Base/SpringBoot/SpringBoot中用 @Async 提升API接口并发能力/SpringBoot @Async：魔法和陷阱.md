JavaSpringBoot<br />`@Async`注解就像是springboot项目中性能优化的秘密武器。也可以手动创建自己的执行器和线程池，但`@Async`使事情变得更简单、更神奇。<br />`@Async`注解允许在后台运行代码，因此主线程可以继续运行，而无需等待较慢的任务完成。但是，就像所有秘密武器一样，明智地使用它并了解它的局限性非常重要。<br />在这里将深入探讨`@Async` 的魔力以及在 Spring Boot 项目中使用它时应该注意的问题。<br />首先学习如何在应用程序中使用 `@Async` 的基础知识。<br />需要在 Spring Boot 应用程序中启用`@Async` 。为此，需要将`@EnableAsync`注释添加到配置类或主应用程序文件中。这将为应用程序中使用`@Async`注释的所有方法启用异步行为。
```java
@SpringBootApplication
@EnableAsync
public class BackendAsjApplication {
}
```
还需要创建一个 Bean，指定使用 `@Async` 注释的方法的配置。可以设置最大线程池大小、队列大小等。不过，添加这些配置时要小心。否则，可能很快就会耗尽内存。通常还会添加一个日志，以在队列大小已满并且没有更多线程来接收新传入任务时发出警告。
```java
@Bean
public ThreadPoolTaskExecutor taskExecutor() {
    ThreadPoolTaskExecutor executor = new ThreadPoolTaskExecutor();
    executor.setCorePoolSize(2);
    executor.setMaxPoolSize(2);
    executor.setQueueCapacity(500);
    executor.setThreadNamePrefix("MyAsyncThread-");
    executor.setRejectedExecutionHandler((r, executor1) -> log.warn("Task rejected, thread pool is full and queue is also full"));
    executor.initialize();
    return executor;
}
```
现在，使用它。假设有一个服务类，其中包含想要异步的方法。使用`@Async`注释此方法。
```java
@Service
public class EmailService {
    @Async
    public void sendEmail() {

    }
}
```
在代码示例中，会看到多次提到EmailService和PurchaseService。这些只是示例。不想将所有内容都命名为“MyService”。因此，将其命名为更有意义的名称。在电子商务应用程序中，当然希望您EmailService 是异步的，这样客户请求就不会被阻止<br />现在，当调用此方法时，它将立即返回，从而释放调用线程（通常是主线程）以继续执行其他任务。该方法将继续在后台执行，稍后将结果返回给调用线程。由于在这里用 void 标记了 `@Async` 方法，因此对它何时完成并不真正感兴趣。<br />非常简单而且非常强大，对吧？（当然，可以做更多配置，但上面的代码足以运行完全异步的任务）<br />但是，在开始使用 `@Async` 注释所有方法之前，需要注意一些问题。
<a name="stzKP"></a>
## 1、`@Async`方法需要位于不同的类中
使用 `@Async` 注释时，请务必注意，不能从同一类中调用 `@Async` 方法。这是因为这样做会导致无限循环并导致应用程序挂起。<br />以下是不应该做的事情的示例：
```java
@Service
public class PurchaseService {

    public void purchase(){
        sendEmail();
    }

    @Async
    public void sendEmail(){
        // Asynchronous code
    }
}
```
相反，应该为异步方法使用单独的类或服务。
```java
@Service
public class EmailService {

    @Async
    public void sendEmail(){
        // Asynchronous code
    }
}

@Service
public class PurchaseService {

    public void purchase(){
        emailService.sendEmail();
    }

    @Autowired
    private EmailService emailService;
}
```
现在您可能想知道，可以从另一个异步方法中调用异步方法吗？最简洁的答案是不。当调用异步方法时，它会在不同的线程中执行，并且调用线程会继续执行下一个任务。如果调用线程本身是异步方法，则它无法等待被调用的异步方法完成后再继续，这可能会导致意外行为。
<a name="sX9lv"></a>
## 2、`@Async` 和 `@Transcational` 配合不佳
`@Transactional` 注释用于指示方法或类应该参与事务。它用于确保一组数据库操作作为单个工作单元执行，并且在发生任何故障时数据库保持一致状态。<br />当一个方法被`@Transactional`注解时，Spring会在该方法周围创建一个代理，并且该方法内的所有数据库操作都在事务上下文中执行。Spring 还负责在调用方法之前启动事务，并在方法返回后提交事务，或者在发生异常时回滚事务。<br />但是，当您使用 `@Async` 注释使方法异步时，该方法将在与主应用程序线程不同的单独线程中执行。这意味着该方法不再在 Spring 启动的事务上下文中执行。因此，`@Async`方法内的数据库操作不会参与事务，并且在出现异常时数据库可能会处于不一致的状态。
```java
@Service
public class EmailService {

    @Transactional
    public void transactionalMethod() {
        //database operation 1
        asyncMethod();
        //database operation 2
    }

    @Async
    public void asyncMethod() {
        //database operation 3
    }
}
```
在此示例中，数据库操作 1 和数据库操作 2 在 Spring 启动的事务上下文中执行。但是，数据库操作 3 是在单独的线程中执行的，并且不是事务的一部分。<br />因此，如果在执行数据库操作3之前发生异常，则数据库操作1和数据库操作2将按预期回滚，但数据库操作3不会回滚。这可能会使数据库处于不一致的状态。<br />当然，有很多方法可以解决这个问题，即使用 TransactionTemplate 之类的东西来管理事务，但开箱即用，如果从转换方法调用异步方法，最终会出现问题。
<a name="NvgQt"></a>
## 3、`@Async` 阻塞问题
假设这是 `@Async` 线程池的配置：
```java
@Bean
public ThreadPoolTaskExecutor taskExecutor() {
    ThreadPoolTaskExecutor executor = new ThreadPoolTaskExecutor();
    executor.setCorePoolSize(2);
    executor.setMaxPoolSize(2);
    executor.setQueueCapacity(500);
    executor.setThreadNamePrefix("MyAsyncThread-");
    executor.setRejectedExecutionHandler((r, executor1) -> log.warn("Task rejected, thread pool is full and queue is also full"));
    executor.initialize();
    return executor;
}
```
这意味着在任何特定时刻，最多将运行 2 个 `@Async` 任务。如果有更多任务进来，它们将排队，直到队列大小达到 500。<br />但现在假设，`@Async` 任务之一执行起来花费了太多时间，或者只是由于外部依赖而被阻止。这意味着所有其他任务将排队并且执行速度不够快。根据应用程序类型，这可能会导致延迟。<br />解决此问题的一种方法是为长时间运行的任务使用单独的线程池，为更紧急且不需要大量处理时间的任务使用单独的线程池。可以这样做：
```java
@Primary
@Bean(name = "taskExecutorDefault")
public ThreadPoolTaskExecutor taskExecutorDefault() {
    ThreadPoolTaskExecutor executor = new ThreadPoolTaskExecutor();
    executor.setCorePoolSize(2);
    executor.setMaxPoolSize(2);
    executor.setQueueCapacity(500);
    executor.setThreadNamePrefix("Async-1-");
    executor.initialize();
    return executor;
}

@Bean(name = "taskExecutorForHeavyTasks")
public ThreadPoolTaskExecutor taskExecutorRegistration() {
    ThreadPoolTaskExecutor executor = new ThreadPoolTaskExecutor();
    executor.setCorePoolSize(2);
    executor.setMaxPoolSize(2);
    executor.setQueueCapacity(500);
    executor.setThreadNamePrefix("Async2-");
    executor.initialize();
    return executor;
}
```
然后要使用它，只需在 `@Async` 声明中添加执行器的名称即可：
```java
@Service
public class EmailService {
    @Async("taskExecutorForHeavyTasks")
    public void sendEmailHeavy() {
        //method implementation
    }
}
```
但是，请注意，不应该在调用`Thread.sleep()`或的方法上使用`@Async Object.wait()`，因为它会阻塞线程，并且使用`@Async`的目的将落空。
<a name="fxzSQ"></a>
## 4、`@Async` 中的异常
另一件需要记住的事情是 `@Async` 方法不会向调用线程抛出异常。这意味着需要在 `@Async` 方法中正确处理异常，否则它们将丢失。<br />以下是不应该做的事情的示例：
```java
@Service
public class EmailService {

    @Async
    public void sendEmail() throws Exception{
        throw new Exception("Oops, cannot send email!");
    }
}

@Service
public class PurchaseService {

    @Autowired
    private EmailService emailService;

    public void purchase(){
        try{
            emailService.sendEmail();
        }catch (Exception e){
            System.out.println("Caught exception: " + e.getMessage());
        }
    }
}
```
在上面的代码中，异常在 `asyncMethod()` 中抛出，但不会被调用线程捕获，并且 catch 块不会被执行。<br />为了正确处理 `@Async` 方法中的异常，可以结合使用 Future 和 try-catch 块。这是一个例子：
```java
@Service
public class EmailService {

    @Async
    public Future<String> sendEmail() throws Exception{
        throw new Exception("Oops, cannot send email!");
    }
}

@Service
public class PurchaseService {

    @Autowired
    private EmailService emailService;

    public void purchase(){
        try{
            Future<String> future = emailService.sendEmail();
            String result = future.get();
            System.out.println("Result: " + result);
        }catch (Exception e){
            System.out.println("Caught exception: " + e.getMessage());
        }
    }
}
```
通过返回 Future 对象并使用 try-catch 块，可以正确处理和捕获 `@Async` 方法中引发的异常。<br />总之，Spring Boot中的`@Async`注释是提高应用程序性能和可伸缩性的强大工具。但是，小心使用它并注意它的局限性是很重要的。通过理解这些陷阱并使用`CompletableFuture`和`Executor`等技术，可以充分利用`@Async`注释并将应用程序提升到下一个级别。
