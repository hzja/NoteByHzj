Java<br />`Callable`和`DeferredResult`可以用来进行异步请求处理。利用它们，可以异步生成返回值，在具体处理的过程中，直接在controller中返回相应的`Callable`或者`DeferredResult`，在这之后，servlet线程将被释放，可用于其他连接；`DeferredResult`另外会有线程来进行结果处理，并`setResult`。
<a name="TM8M8"></a>
## 基础准备
在正式开始之前，先做一点准备工作，在项目中新建了一个base模块。其中包含一些提供基础支持的java类，在其他模块中可能会用到。
<a name="Suvxb"></a>
### `ResponseMsg`
定义了一个`ResponseMsg`的实体类来作为返回值类型：
```java
@Data
@NoArgsConstructor
@AllArgsConstructor
public class ResponseMsg<T> {

    private int code;

    private String msg;

    private T data;

}
```
非常简单，里面包含了code、msg和data三个字段，其中data为泛型类型。另外类的注解`Data`、`NoArgsConstructor`和`AllArgsConstructor`都是lombok提供的简化开发的，主要功能分别是，为类生成set和get方法，生成无参构造器和生成全参构造器。<br />使用idea进行开发可以装一下lombok的支持插件。另外，lombok的依赖参见：
```xml
<dependency>
  <groupId>org.projectlombok</groupId>
  <artifactId>lombok-maven</artifactId>
  <version>1.16.16.0</version>
  <type>pom</type>
</dependency>
```
<a name="wH3fo"></a>
### `TaskService`
建立了一个`TaskService`，用来为阻塞调用和Callable调用提供实际结果处理的。代码如下：
```java
@Service
public class TaskService {

    private static final Logger log = LoggerFactory.getLogger(TaskService.class);

    public ResponseMsg<String> getResult(){

        log.info("任务开始执行，持续等待中...");

        try {
            Thread.sleep(30000L);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        log.info("任务处理完成");
        return new ResponseMsg<String>(0,"操作成功","success");
    }
}
```
可以看到，里面实际提供服务的是getResult方法，这边直接返回一个`new ResponseMsg(0,"操作成功","success")`。但是其中又特意让它sleep了30秒，模拟一个耗时较长的请求。
<a name="UjfWT"></a>
### 阻塞调用
平时用的最普遍的还是阻塞调用，通常请求的处理时间较短，在并发量较小的情况下，使用阻塞调用问题也不是很大。 阻塞调用实现非常简单，首先新建一个模块blockingtype，里面只包含一个controller类，用来接收请求并利用`TaskService`来获取结果。
```java
@RestController
public class BlockController {

    private static final Logger log = LoggerFactory.getLogger(BlockController.class);

    @Autowired
    private TaskService taskService;

    @RequestMapping(value = "/get", method = RequestMethod.GET)
    public ResponseMsg<String> getResult(){

        log.info("接收请求，开始处理...");
        ResponseMsg<String> result =  taskService.getResult();
        log.info("接收任务线程完成并退出");

        return result;

    }
}
```
请求的是`getResult`方法，其中调用了`taskService`，这个`taskService`是注入得到的。关于怎么跨模块注入的，其实也非常简单，在本模块，加入对其他模块的依赖就可以了。比如这里在`blockingtype`的pom.xml文件中加入对base模块的依赖：
```xml
<dependency>
    <groupId>com.sunny</groupId>
    <artifactId>base</artifactId>
    <version>1.0-SNAPSHOT</version>
</dependency>
```
然后看一下实际调用效果，这里设置端口号为8080，启动日志如下：
```
2018-06-24 19:02:48.514  INFO 11207 --- [           main] com.sunny.BlockApplication               : Starting BlockApplication on xdeMacBook-Pro.local with PID 11207 (/Users/zsunny/IdeaProjects/asynchronoustask/blockingtype/target/classes started by zsunny in /Users/zsunny/IdeaProjects/asynchronoustask)
2018-06-24 19:02:48.519  INFO 11207 --- [           main] com.sunny.BlockApplication               : No active profile set, falling back to default profiles: default
2018-06-24 19:02:48.762  INFO 11207 --- [           main] ationConfigEmbeddedWebApplicationContext : Refreshing org.springframework.boot.context.embedded.AnnotationConfigEmbeddedWebApplicationContext@4445629: startup date [Sun Jun 24 19:02:48 CST 2018]; root of context hierarchy
2018-06-24 19:02:50.756  INFO 11207 --- [           main] s.b.c.e.t.TomcatEmbeddedServletContainer : Tomcat initialized with port(s): 8080 (http)
2018-06-24 19:02:50.778  INFO 11207 --- [           main] o.apache.catalina.core.StandardService   : Starting service [Tomcat]
2018-06-24 19:02:50.780  INFO 11207 --- [           main] org.apache.catalina.core.StandardEngine  : Starting Servlet Engine: Apache Tomcat/8.5.23
2018-06-24 19:02:50.922  INFO 11207 --- [ost-startStop-1] o.a.c.c.C.[Tomcat].[localhost].[/]       : Initializing Spring embedded WebApplicationContext
2018-06-24 19:02:50.922  INFO 11207 --- [ost-startStop-1] o.s.web.context.ContextLoader            : Root WebApplicationContext: initialization completed in 2200 ms
2018-06-24 19:02:51.156  INFO 11207 --- [ost-startStop-1] o.s.b.w.servlet.ServletRegistrationBean  : Mapping servlet: 'dispatcherServlet' to [/]
2018-06-24 19:02:51.162  INFO 11207 --- [ost-startStop-1] o.s.b.w.servlet.FilterRegistrationBean   : Mapping filter: 'characterEncodingFilter' to: [/*]
2018-06-24 19:02:51.163  INFO 11207 --- [ost-startStop-1] o.s.b.w.servlet.FilterRegistrationBean   : Mapping filter: 'hiddenHttpMethodFilter' to: [/*]
2018-06-24 19:02:51.163  INFO 11207 --- [ost-startStop-1] o.s.b.w.servlet.FilterRegistrationBean   : Mapping filter: 'httpPutFormContentFilter' to: [/*]
2018-06-24 19:02:51.163  INFO 11207 --- [ost-startStop-1] o.s.b.w.servlet.FilterRegistrationBean   : Mapping filter: 'requestContextFilter' to: [/*]
2018-06-24 19:02:51.620  INFO 11207 --- [           main] s.w.s.m.m.a.RequestMappingHandlerAdapter : Looking for @ControllerAdvice: org.springframework.boot.context.embedded.AnnotationConfigEmbeddedWebApplicationContext@4445629: startup date [Sun Jun 24 19:02:48 CST 2018]; root of context hierarchy
2018-06-24 19:02:51.724  INFO 11207 --- [           main] s.w.s.m.m.a.RequestMappingHandlerMapping : Mapped "{[/get],methods=[GET]}" onto public com.sunny.entity.ResponseMsg<java.lang.String> com.sunny.controller.BlockController.getResult()
2018-06-24 19:02:51.730  INFO 11207 --- [           main] s.w.s.m.m.a.RequestMappingHandlerMapping : Mapped "{[/error]}" onto public org.springframework.http.ResponseEntity<java.util.Map<java.lang.String, java.lang.Object>> org.springframework.boot.autoconfigure.web.BasicErrorController.error(javax.servlet.http.HttpServletRequest)
2018-06-24 19:02:51.731  INFO 11207 --- [           main] s.w.s.m.m.a.RequestMappingHandlerMapping : Mapped "{[/error],produces=[text/html]}" onto public org.springframework.web.servlet.ModelAndView org.springframework.boot.autoconfigure.web.BasicErrorController.errorHtml(javax.servlet.http.HttpServletRequest,javax.servlet.http.HttpServletResponse)
2018-06-24 19:02:51.780  INFO 11207 --- [           main] o.s.w.s.handler.SimpleUrlHandlerMapping  : Mapped URL path [/webjars/**] onto handler of type [class org.springframework.web.servlet.resource.ResourceHttpRequestHandler]
2018-06-24 19:02:51.780  INFO 11207 --- [           main] o.s.w.s.handler.SimpleUrlHandlerMapping  : Mapped URL path [/**] onto handler of type [class org.springframework.web.servlet.resource.ResourceHttpRequestHandler]
2018-06-24 19:02:51.838  INFO 11207 --- [           main] o.s.w.s.handler.SimpleUrlHandlerMapping  : Mapped URL path [/**/favicon.ico] onto handler of type [class org.springframework.web.servlet.resource.ResourceHttpRequestHandler]
2018-06-24 19:02:52.126  INFO 11207 --- [           main] o.s.j.e.a.AnnotationMBeanExporter        : Registering beans for JMX exposure on startup
2018-06-24 19:02:52.205  INFO 11207 --- [           main] s.b.c.e.t.TomcatEmbeddedServletContainer : Tomcat started on port(s): 8080 (http)
2018-06-24 19:02:52.211  INFO 11207 --- [           main] com.sunny.BlockApplication               : Started BlockApplication in 5.049 seconds (JVM running for 6.118)
```
可以看到顺利启动了，那么就来访问一下：http://localhost:8080/get<br />等待了大概30秒左右，得到json数据：
```json
{"code":0,"msg":"操作成功","data":"success"}
```
然后来看看控制台的日志：
```
2018-06-24 19:04:07.315  INFO 11207 --- [nio-8080-exec-1] com.sunny.controller.BlockController     : 接收请求，开始处理...
2018-06-24 19:04:07.316  INFO 11207 --- [nio-8080-exec-1] com.sunny.service.TaskService            : 任务开始执行，持续等待中...
2018-06-24 19:04:37.322  INFO 11207 --- [nio-8080-exec-1] com.sunny.service.TaskService            : 任务处理完成
2018-06-24 19:04:37.322  INFO 11207 --- [nio-8080-exec-1] com.sunny.controller.BlockController     : 接收任务线程完成并退出
```
可以看到在“`ResponseMsg result = taskService.getResult();`”的时候是阻塞了大约30秒钟，随后才执行它后面的打印语句“`log.info("接收任务线程完成并退出")`;”。
<a name="KvcKt"></a>
### Callable异步调用
涉及到较长时间的请求处理的话，比较好的方式是用异步调用，比如利用`Callable`返回结果。异步主要表现在，接收请求的servlet可以不用持续等待结果产生，而可以被释放去处理其他事情。当然，在调用者来看的话，其实还是表现在持续等待30秒。这有利于服务端提供更大的并发处理量。<br />这里新建一个callabledemo模块，在这个模块中，一样只包含一个`TaskController`，另外也是需要加入base模块的依赖。只不过这里返回值不是`ResponseMsg`类型了，而是一个`Callable`类型。
```java
@RestController
public class TaskController {

    private static final Logger log = LoggerFactory.getLogger(TaskController.class);

    @Autowired
    private TaskService taskService;

    @RequestMapping(value = "/get",method = RequestMethod.GET)
    public Callable<ResponseMsg<String>> getResult(){

        log.info("接收请求，开始处理...");

        Callable<ResponseMsg<String>> result = (()->{
            return taskService.getResult();
        });

        log.info("接收任务线程完成并退出");
        return result;
    }
}
```
在里面，创建了一个`Callable`类型的变量result，并实现了它的`call`方法，在call方法中，也是调用`taskService`的`getResult`方法得到返回值并返回。<br />下一步就运行一下这个模块，这里在模块的application.yml中设置端口号为8081：
```yaml
server:
  port: 8081
```
启动，可以看到控制台的消息：
```
2018-06-24 19:38:14.658  INFO 11226 --- [           main] com.sunny.CallableApplication            : Starting CallableApplication on xdeMacBook-Pro.local with PID 11226 (/Users/zsunny/IdeaProjects/asynchronoustask/callabledemo/target/classes started by zsunny in /Users/zsunny/IdeaProjects/asynchronoustask)
2018-06-24 19:38:14.672  INFO 11226 --- [           main] com.sunny.CallableApplication            : No active profile set, falling back to default profiles: default
2018-06-24 19:38:14.798  INFO 11226 --- [           main] ationConfigEmbeddedWebApplicationContext : Refreshing org.springframework.boot.context.embedded.AnnotationConfigEmbeddedWebApplicationContext@4445629: startup date [Sun Jun 24 19:38:14 CST 2018]; root of context hierarchy
2018-06-24 19:38:16.741  INFO 11226 --- [           main] s.b.c.e.t.TomcatEmbeddedServletContainer : Tomcat initialized with port(s): 8081 (http)
2018-06-24 19:38:16.762  INFO 11226 --- [           main] o.apache.catalina.core.StandardService   : Starting service [Tomcat]
2018-06-24 19:38:16.764  INFO 11226 --- [           main] org.apache.catalina.core.StandardEngine  : Starting Servlet Engine: Apache Tomcat/8.5.23
2018-06-24 19:38:16.918  INFO 11226 --- [ost-startStop-1] o.a.c.c.C.[Tomcat].[localhost].[/]       : Initializing Spring embedded WebApplicationContext
2018-06-24 19:38:16.919  INFO 11226 --- [ost-startStop-1] o.s.web.context.ContextLoader            : Root WebApplicationContext: initialization completed in 2126 ms
2018-06-24 19:38:17.144  INFO 11226 --- [ost-startStop-1] o.s.b.w.servlet.ServletRegistrationBean  : Mapping servlet: 'dispatcherServlet' to [/]
2018-06-24 19:38:17.149  INFO 11226 --- [ost-startStop-1] o.s.b.w.servlet.FilterRegistrationBean   : Mapping filter: 'characterEncodingFilter' to: [/*]
2018-06-24 19:38:17.150  INFO 11226 --- [ost-startStop-1] o.s.b.w.servlet.FilterRegistrationBean   : Mapping filter: 'hiddenHttpMethodFilter' to: [/*]
2018-06-24 19:38:17.150  INFO 11226 --- [ost-startStop-1] o.s.b.w.servlet.FilterRegistrationBean   : Mapping filter: 'httpPutFormContentFilter' to: [/*]
2018-06-24 19:38:17.150  INFO 11226 --- [ost-startStop-1] o.s.b.w.servlet.FilterRegistrationBean   : Mapping filter: 'requestContextFilter' to: [/*]
2018-06-24 19:38:17.632  INFO 11226 --- [           main] s.w.s.m.m.a.RequestMappingHandlerAdapter : Looking for @ControllerAdvice: org.springframework.boot.context.embedded.AnnotationConfigEmbeddedWebApplicationContext@4445629: startup date [Sun Jun 24 19:38:14 CST 2018]; root of context hierarchy
2018-06-24 19:38:17.726  INFO 11226 --- [           main] s.w.s.m.m.a.RequestMappingHandlerMapping : Mapped "{[/get],methods=[GET]}" onto public java.util.concurrent.Callable<com.sunny.entity.ResponseMsg<java.lang.String>> com.sunny.controller.TaskController.getResult()
2018-06-24 19:38:17.731  INFO 11226 --- [           main] s.w.s.m.m.a.RequestMappingHandlerMapping : Mapped "{[/error]}" onto public org.springframework.http.ResponseEntity<java.util.Map<java.lang.String, java.lang.Object>> org.springframework.boot.autoconfigure.web.BasicErrorController.error(javax.servlet.http.HttpServletRequest)
2018-06-24 19:38:17.733  INFO 11226 --- [           main] s.w.s.m.m.a.RequestMappingHandlerMapping : Mapped "{[/error],produces=[text/html]}" onto public org.springframework.web.servlet.ModelAndView org.springframework.boot.autoconfigure.web.BasicErrorController.errorHtml(javax.servlet.http.HttpServletRequest,javax.servlet.http.HttpServletResponse)
2018-06-24 19:38:17.777  INFO 11226 --- [           main] o.s.w.s.handler.SimpleUrlHandlerMapping  : Mapped URL path [/webjars/**] onto handler of type [class org.springframework.web.servlet.resource.ResourceHttpRequestHandler]
2018-06-24 19:38:17.777  INFO 11226 --- [           main] o.s.w.s.handler.SimpleUrlHandlerMapping  : Mapped URL path [/**] onto handler of type [class org.springframework.web.servlet.resource.ResourceHttpRequestHandler]
2018-06-24 19:38:17.825  INFO 11226 --- [           main] o.s.w.s.handler.SimpleUrlHandlerMapping  : Mapped URL path [/**/favicon.ico] onto handler of type [class org.springframework.web.servlet.resource.ResourceHttpRequestHandler]
2018-06-24 19:38:18.084  INFO 11226 --- [           main] o.s.j.e.a.AnnotationMBeanExporter        : Registering beans for JMX exposure on startup
2018-06-24 19:38:18.176  INFO 11226 --- [           main] s.b.c.e.t.TomcatEmbeddedServletContainer : Tomcat started on port(s): 8081 (http)
2018-06-24 19:38:18.183  INFO 11226 --- [           main] com.sunny.CallableApplication            : Started CallableApplication in 4.538 seconds (JVM running for 5.327)
```
完美启动了，然后还是一样，访问一下：http://localhost:8081/get<br />在大约等待了30秒左右，在浏览器上得到json数据：
```json
{"code":0,"msg":"操作成功","data":"success"}
```
和阻塞调用的结果一样——当然一样啦，都是同`taskService`中得到的结果。<br />然后看看控制台的消息：
```
2018-06-24 19:39:07.738  INFO 11226 --- [nio-8081-exec-1] com.sunny.controller.TaskController      : 接收请求，开始处理...
2018-06-24 19:39:07.740  INFO 11226 --- [nio-8081-exec-1] com.sunny.controller.TaskController      : 接收任务线程完成并退出
2018-06-24 19:39:07.753  INFO 11226 --- [      MvcAsync1] com.sunny.service.TaskService            : 任务开始执行，持续等待中...
2018-06-24 19:39:37.756  INFO 11226 --- [      MvcAsync1] com.sunny.service.TaskService            : 任务处理完成
```
很显然，这里的消息出现的顺序和阻塞模式有所不同了，这里在“接收请求，开始处理…”之后直接打印了“接收任务线程完成并退出”。而不是先出现“任务处理完成”后再出现“接收任务线程完成并退出”。<br />这就说明，这里没有阻塞在从taskService中获得数据的地方，controller中直接执行后面的部分（这里可以做其他很多事，不仅仅是打印日志）。
<a name="PyiFi"></a>
### `DeferredResult`异步调用
前面铺垫了那么多，还是主要来说`DeferredResult`的；和`Callable`一样，`DeferredResult`也是为了支持异步调用。两者的主要差异，Sunny觉得主要在`DeferredResult`需要自己用线程来处理结果`setResult`，而`Callable`的话不需要来维护一个结果处理线程。<br />总体来说，`Callable`的话更为简单，同样的也是因为简单，灵活性不够；相对地，`DeferredResult`更为复杂一些，但是又极大的灵活性。在可以用`Callable`的时候，直接用`Callable`；而遇到`Callable`没法解决的场景的时候，可以尝试使用`DeferredResult`。<br />这里Sunny将会设计两个`DeferredResult`使用场景。
<a name="oIgXP"></a>
##### 场景一：

- 创建一个持续在随机间隔时间后从任务队列中获取任务的线程
- 访问controller中的方法，创建一个`DeferredResult`，设定超时时间和超时返回对象
- 设定`DeferredResult`的超时回调方法和完成回调方法
- 将`DeferredResult`放入任务中，并将任务放入任务队列
- 步骤1中的线程获取到任务队列中的任务，并产生一个随机结果返回
- 场景其实非常简单，接下来来看看具体的实现。首先，还是来看任务实体类是怎么样的。
```java
/**
 * 任务实体类
 */
@Data
@NoArgsConstructor
@AllArgsConstructor
public class Task {

    private int taskId;

    private DeferredResult<ResponseMsg<String>> taskResult;

    @Override
    public String toString() {
        return "Task{" +
                "taskId=" + taskId +
                ", taskResult" + "{responseMsg=" + taskResult.getResult() + "}" +
                '}';
    }
}
```
看起来非常简单，成员变量又`taskId`和`taskResult`，前者是int类型，后者为`DeferredResult`类型，它的泛型类型为`ResponseMsg`，注意这里用到`ResponseMsg`，所以也需要导入base模块的依赖。<br />另外注解之前已经说明了，不过这里再提一句，`@Data`注解也包含了toString的重写，但是这里为了知道具体的`ResponseMsg`的内容，Sunny特意手动重写。<br />看完Task类型，再来看看任务队列。
```java
@Component
public class TaskQueue {

    private static final Logger log = LoggerFactory.getLogger(TaskQueue.class);

    private static final int QUEUE_LENGTH = 10;

    private BlockingQueue<Task> queue = new LinkedBlockingDeque<>(QUEUE_LENGTH);

    private int taskId = 0;


    /**
     * 加入任务
     * @param deferredResult
     */
    public void put(DeferredResult<ResponseMsg<String>> deferredResult){
        taskId++;
        log.info("任务加入队列，id为：{}",taskId);
        queue.offer(new Task(taskId,deferredResult));

    }

    /**
     * 获取任务
     * @return
     * @throws InterruptedException
     */
    public Task take() throws InterruptedException {
        Task task = queue.poll();
        log.info("获得任务:{}",task);
        return task;
    }
}
```
这里将它作为一个bean，之后会在其他bean中注入，这里实际的队列为成员变量queue，它是`LinkedBlockingDeque`类型的。还有一个成员变量为taskId，是用于自动生成任务id的，并且在加入任务的方法中实现自增，以确保每个任务的id唯一性。方法的话又put和take方法，分别用于向队列中添加任务和取出任务；其中，对queue的操作，分别用了offer和poll，这样是实现一个非阻塞的操作，并且在队列为空和队列已满的情况下不会抛出异常。<br />另外，大家实现的时候，可以考虑使用`ConcurrentLinkedQueue`来高效处理并发，因为它属于无界非阻塞队列，使用过程中需要考虑可能造成的OOM问题。Sunny这里选择阻塞队列`LinkedBlockingDeque`，它底层使用加锁进行了同步；但是这里使用了`TaskQueue`进行封装，处理过程中有一些额外操作，调用时需要加锁以防发生某些意料之外的问题。<br />然后来看步骤1中的，启动一个持续从任务队列中获取任务的线程的具体实现。
```java
@Component
public class TaskExecute {

    private static final Logger log = LoggerFactory.getLogger(TaskExecute.class);

    private static final Random random = new Random();

    //默认随机结果的长度
    private static final int DEFAULT_STR_LEN = 10;
    
    //用于生成随机结果
    private static final String str = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    @Autowired
    private TaskQueue taskQueue;


    /**
     * 初始化启动
     */
    @PostConstruct
    public void init(){
        log.info("开始持续处理任务");
        new Thread(this::execute).start();
    }


    /**
     * 持续处理
     * 返回执行结果
     */
    private void execute(){
        while (true){
            try {
                //取出任务
                Task task;
                synchronized (taskQueue) {
                    task = taskQueue.take();
                }

                if(task != null) {
                    //设置返回结果
                    String randomStr = getRandomStr(DEFAULT_STR_LEN);
                    ResponseMsg<String> responseMsg = new ResponseMsg<String>(0, "success", randomStr);
                    log.info("返回结果:{}", responseMsg);
                    task.getTaskResult().setResult(responseMsg);
                }
                int time = random.nextInt(10);
                log.info("处理间隔：{}秒",time);
                Thread.sleep(time*1000L);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    /**
     * 获取长度为len的随机串
     * @param len
     * @return
     */
    private String getRandomStr(int len){
        int maxInd = str.length();
        StringBuilder sb = new StringBuilder();
        int ind;
        for(int i=0;i<len;i++){
            ind = random.nextInt(maxInd);
            sb.append(str.charAt(ind));
        }
        return String.valueOf(sb);
    }
}
```
这里，注入了`TaskQueue`，成员变量比较简单并且有注释，不再说明，主要来看方法。先看一下最后一个方法`getRandomStr`，很显然，这是一个获得长度为len的随机串的方法，访问限定为`private`，为类中其他方法服务的。然后看`init`方法，它执行的其实就是开启了一个线程并且执行`execute`方法，注意一下它上面的`@PostContruct`注解，这个注解就是在这个bean初始化的时候就执行这个方法。<br />所以需要关注的实际逻辑在`execute`方法中。可以看到，在`execute`方法中，用了一个`while(true)`来保证线程持续运行。因为是并发环境下，考虑对`taskQueue`加锁，从中取出任务；如果任务不为空，获取用e生成一个随机结果并用`setResult`方法进行返回。<br />最后可以看到，利用random生成来一个`[0,10)`的随机数，并让线程sleep相应的秒数。这里注意一下，需要设定一个时间间隔，否则，先线程持续跑会出现CPU负载过高的情况。<br />接下来就看看controller是如何处理的。
```java
@RestController
public class TaskController {

    private static final Logger log = LoggerFactory.getLogger(TaskController.class);

    //超时结果
    private static final ResponseMsg<String> OUT_OF_TIME_RESULT = new ResponseMsg<>(-1,"超时","out of time");

    //超时时间
    private static final long OUT_OF_TIME = 3000L;

    @Autowired
    private TaskQueue taskQueue;


    @RequestMapping(value = "/get",method = RequestMethod.GET)
    public DeferredResult<ResponseMsg<String>> getResult() {

        log.info("接收请求，开始处理...");
        
        //建立DeferredResult对象，设置超时时间，以及超时返回超时结果
        DeferredResult<ResponseMsg<String>> result = new DeferredResult<>(OUT_OF_TIME, OUT_OF_TIME_RESULT);

        result.onTimeout(() -> {
            log.info("调用超时");
        });

        result.onCompletion(() -> {
            log.info("调用完成");
        });

        //并发，加锁
        synchronized (taskQueue) {
            taskQueue.put(result);
        }
        log.info("接收任务线程完成并退出");
        return result;
    }
}
```
这里同样注入了`taskQueue`。请求方法就只有一个`getResult`，返回值为`DeferredResult<ResponseMsg>`。这里首先创建了`DeferredResult`对象result并且设定超时时间和超时返回结果；随后设定result的`onTimeout`和`onCompletion`方法，其实就是传入两个`Runnable`对象来实现回调的效果；之后就是加锁并且将result加入任务队列中。<br />总体来说，场景不算非常复杂，看到这里大家应该都能基本了解了。然后来跑一下测试一下。<br />在application.yml中设定端口为8082：
```yaml
server:
  port: 8082
```
启动模块，控制台信息如下：
```
2018-06-24 21:49:28.815  INFO 11322 --- [           main] com.sunny.DeferredResultApplication      : Starting DeferredResultApplication on xdeMacBook-Pro.local with PID 11322 (/Users/zsunny/IdeaProjects/asynchronoustask/deferredresultdemo/target/classes started by zsunny in /Users/zsunny/IdeaProjects/asynchronoustask)
2018-06-24 21:49:28.821  INFO 11322 --- [           main] com.sunny.DeferredResultApplication      : No active profile set, falling back to default profiles: default
2018-06-24 21:49:29.010  INFO 11322 --- [           main] ationConfigEmbeddedWebApplicationContext : Refreshing org.springframework.boot.context.embedded.AnnotationConfigEmbeddedWebApplicationContext@5ccddd20: startup date [Sun Jun 24 21:49:28 CST 2018]; root of context hierarchy
2018-06-24 21:49:30.971  INFO 11322 --- [           main] s.b.c.e.t.TomcatEmbeddedServletContainer : Tomcat initialized with port(s): 8082 (http)
2018-06-24 21:49:30.980  INFO 11322 --- [           main] o.apache.catalina.core.StandardService   : Starting service [Tomcat]
2018-06-24 21:49:30.981  INFO 11322 --- [           main] org.apache.catalina.core.StandardEngine  : Starting Servlet Engine: Apache Tomcat/8.5.23
2018-06-24 21:49:31.062  INFO 11322 --- [ost-startStop-1] o.a.c.c.C.[Tomcat].[localhost].[/]       : Initializing Spring embedded WebApplicationContext
2018-06-24 21:49:31.063  INFO 11322 --- [ost-startStop-1] o.s.web.context.ContextLoader            : Root WebApplicationContext: initialization completed in 2066 ms
2018-06-24 21:49:31.207  INFO 11322 --- [ost-startStop-1] o.s.b.w.servlet.ServletRegistrationBean  : Mapping servlet: 'dispatcherServlet' to [/]
2018-06-24 21:49:31.212  INFO 11322 --- [ost-startStop-1] o.s.b.w.servlet.FilterRegistrationBean   : Mapping filter: 'characterEncodingFilter' to: [/*]
2018-06-24 21:49:31.213  INFO 11322 --- [ost-startStop-1] o.s.b.w.servlet.FilterRegistrationBean   : Mapping filter: 'hiddenHttpMethodFilter' to: [/*]
2018-06-24 21:49:31.213  INFO 11322 --- [ost-startStop-1] o.s.b.w.servlet.FilterRegistrationBean   : Mapping filter: 'httpPutFormContentFilter' to: [/*]
2018-06-24 21:49:31.213  INFO 11322 --- [ost-startStop-1] o.s.b.w.servlet.FilterRegistrationBean   : Mapping filter: 'requestContextFilter' to: [/*]
2018-06-24 21:49:31.247  INFO 11322 --- [           main] com.sunny.bean.TaskExecute               : 开始持续处理任务
2018-06-24 21:49:31.249  INFO 11322 --- [       Thread-8] com.sunny.bean.TaskQueue                 : 获得任务:null
2018-06-24 21:49:31.250  INFO 11322 --- [       Thread-8] com.sunny.bean.TaskExecute               : 处理间隔：6秒
2018-06-24 21:49:31.498  INFO 11322 --- [           main] s.w.s.m.m.a.RequestMappingHandlerAdapter : Looking for @ControllerAdvice: org.springframework.boot.context.embedded.AnnotationConfigEmbeddedWebApplicationContext@5ccddd20: startup date [Sun Jun 24 21:49:28 CST 2018]; root of context hierarchy
2018-06-24 21:49:31.572  INFO 11322 --- [           main] s.w.s.m.m.a.RequestMappingHandlerMapping : Mapped "{[/get],methods=[GET]}" onto public org.springframework.web.context.request.async.DeferredResult<com.sunny.entity.ResponseMsg<java.lang.String>> com.sunny.controller.TaskController.getResult()
2018-06-24 21:49:31.576  INFO 11322 --- [           main] s.w.s.m.m.a.RequestMappingHandlerMapping : Mapped "{[/error]}" onto public org.springframework.http.ResponseEntity<java.util.Map<java.lang.String, java.lang.Object>> org.springframework.boot.autoconfigure.web.BasicErrorController.error(javax.servlet.http.HttpServletRequest)
2018-06-24 21:49:31.577  INFO 11322 --- [           main] s.w.s.m.m.a.RequestMappingHandlerMapping : Mapped "{[/error],produces=[text/html]}" onto public org.springframework.web.servlet.ModelAndView org.springframework.boot.autoconfigure.web.BasicErrorController.errorHtml(javax.servlet.http.HttpServletRequest,javax.servlet.http.HttpServletResponse)
2018-06-24 21:49:31.602  INFO 11322 --- [           main] o.s.w.s.handler.SimpleUrlHandlerMapping  : Mapped URL path [/webjars/**] onto handler of type [class org.springframework.web.servlet.resource.ResourceHttpRequestHandler]
2018-06-24 21:49:31.602  INFO 11322 --- [           main] o.s.w.s.handler.SimpleUrlHandlerMapping  : Mapped URL path [/**] onto handler of type [class org.springframework.web.servlet.resource.ResourceHttpRequestHandler]
2018-06-24 21:49:31.628  INFO 11322 --- [           main] o.s.w.s.handler.SimpleUrlHandlerMapping  : Mapped URL path [/**/favicon.ico] onto handler of type [class org.springframework.web.servlet.resource.ResourceHttpRequestHandler]
2018-06-24 21:49:31.811  INFO 11322 --- [           main] o.s.j.e.a.AnnotationMBeanExporter        : Registering beans for JMX exposure on startup
2018-06-24 21:49:31.892  INFO 11322 --- [           main] s.b.c.e.t.TomcatEmbeddedServletContainer : Tomcat started on port(s): 8082 (http)
2018-06-24 21:49:31.897  INFO 11322 --- [           main] com.sunny.DeferredResultApplication      : Started DeferredResultApplication in 3.683 seconds (JVM running for 4.873)
2018-06-24 21:49:37.254  INFO 11322 --- [       Thread-8] com.sunny.bean.TaskQueue                 : 获得任务:null
2018-06-24 21:49:37.254  INFO 11322 --- [       Thread-8] com.sunny.bean.TaskExecute               : 处理间隔：6秒
```
首先程序完美启动，这没有问题，然后注意这几条信息：
```
2018-06-24 21:49:31.247  INFO 11322 --- [           main] com.sunny.bean.TaskExecute               : 开始持续处理任务
2018-06-24 21:49:31.249  INFO 11322 --- [       Thread-8] com.sunny.bean.TaskQueue                 : 获得任务:null
2018-06-24 21:49:31.250  INFO 11322 --- [       Thread-8] com.sunny.bean.TaskExecute               : 处理间隔：6秒
```
这说明`TaskExecute`中已经成功启动了持续获取任务的线程。<br />接着，还是访问一下：http://localhost:8082/get<br />这一回等待了若干秒就出现了结果：
```json
{"code":0,"msg":"success","data":"CEUO2lmMJr"}
```
可以看到随机串是CEUO2lmMJr。再一次请求又会出现不同的随机串。再看一下控制台的相关信息：
```
2018-06-24 21:51:04.303  INFO 11322 --- [nio-8082-exec-1] com.sunny.controller.TaskController      : 接收请求，开始处理...
2018-06-24 21:51:04.304  INFO 11322 --- [nio-8082-exec-1] com.sunny.bean.TaskQueue                 : 任务加入队列，id为：1
2018-06-24 21:51:04.304  INFO 11322 --- [nio-8082-exec-1] com.sunny.controller.TaskController      : 接收任务线程完成并退出
2018-06-24 21:51:04.323  INFO 11322 --- [       Thread-8] com.sunny.bean.TaskQueue                 : 获得任务:Task{taskId=1, taskResult{responseMsg=null}}
2018-06-24 21:51:04.323  INFO 11322 --- [       Thread-8] com.sunny.bean.TaskExecute               : 返回结果:ResponseMsg(code=0, msg=success, data=CEUO2lmMJr)
```
也是符合预期的，请求进来进入队列中，由`TaskExecute`获取请求并进行处理结果返回。
<a name="Hfjr8"></a>
##### 场景二

- 用户发送请求到`TaskController`的`getResult`方法，该方法接收到请求，创建一个`DeferredResult`，设定超时时间和超时返回对象
- 设定`DeferredResult`的超时回调方法和完成回调方法，超时和完成都会将本次请求产生的`DeferredResult`从集合中remove
- 将`DeferredResult`放入集合中
- 另有一个`TaskExecuteController`，访问其中一个方法，可取出集合中的等待返回的`DeferredResult`对象，并将传入的参数设定为结果

首先来看看`DeferredResult`的集合类：
```java
@Component
@Data
public class TaskSet {

    private Set<DeferredResult<ResponseMsg<String>>> set = new HashSet<>();

}
```
非常简单，只包含了一个HashSet的成员变量。这里可以考虑用`ConcurrentHashMap`来实现高效并发，Sunny这里简单实用HashSet，配合加锁实现并发处理。<br />然后看看发起调用的Controller代码：
```java
@RestController
public class TaskController {

    private Logger log = LoggerFactory.getLogger(TaskController.class);

    //超时结果
    private static final ResponseMsg<String> OUT_OF_TIME_RESULT = new ResponseMsg<>(-1,"超时","out of time");

    //超时时间
    private static final long OUT_OF_TIME = 60000L;

    @Autowired
    private TaskSet taskSet;

    @RequestMapping(value = "/get",method = RequestMethod.GET)
    public DeferredResult<ResponseMsg<String>> getResult(){

        log.info("接收请求，开始处理...");

        //建立DeferredResult对象，设置超时时间，以及超时返回超时结果
        DeferredResult<ResponseMsg<String>> result = new DeferredResult<>(OUT_OF_TIME, OUT_OF_TIME_RESULT);

        result.onTimeout(() -> {
            log.info("调用超时，移除任务，此时队列长度为{}",taskSet.getSet().size());
            
            synchronized (taskSet.getSet()) {
                taskSet.getSet().remove(result);
            }
        });

        result.onCompletion(() -> {
            log.info("调用完成，移除任务，此时队列长度为{}",taskSet.getSet().size());
            
            synchronized (taskSet.getSet()) {
                taskSet.getSet().remove(result);
            }
        });

        //并发，加锁
        synchronized (taskSet.getSet()) {
            taskSet.getSet().add(result);

        }
        log.info("加入任务集合，集合大小为:{}",taskSet.getSet().size());
        log.info("接收任务线程完成并退出");
        return result;

    }
}
```
和场景一中有些类似，但是注意这里在`onTimeout`和`onCompletion`中都多了一个移除元素的操作，这也就是每次调用结束，需要将集合中的`DeferredResult`对象移除，即集合中保存的都是等待请求结果的`DeferredResult`对象。<br />然后看处理请求结果的Controller：
```java
@RestController
public class TaskExecuteController {

    private static final Logger log = LoggerFactory.getLogger(TaskExecuteController.class);

    @Autowired
    private TaskSet taskSet;

    @RequestMapping(value = "/set/{result}",method = RequestMethod.GET)
    public String setResult(@PathVariable("result") String result){
        ResponseMsg<String> res = new ResponseMsg<>(0,"success",result);
        log.info("结果处理开始，得到输入结果为:{}",res);
        Set<DeferredResult<ResponseMsg<String>>> set = taskSet.getSet();

        synchronized (set){
            set.forEach((deferredResult)->{deferredResult.setResult(res);});
        }
        return "Successfully set result: " + result;
    }
}
```
看起来非常简单，只是做了两个操作，接收得到的参数并利用参数生成一个`ResponseMsg`对象，随后将集合中的所有`DeferredResult`都设定结果为根据参数生成的`ResponseMsg`对象。最后返回一个提示：成功设置结果…<br />来启动测试验证一下。说一下验证的过程，同时打开两个请求，然后再设定一个结果，最后两个请求都会得到这个结果。当然同时多个或者一个请求也是一样。这里有一个地方需要注意一下：<br />浏览器可能会对相同的url请求有缓存策略，也就是同时两个标签向同一个url发送请求，浏览器只会先发送一个请求，等一个请求结束才会再发送另外一个请求。<br />这样，考虑从两个浏览器中发送请求：localhost:8083/get<br />然后随便找其中一个，发送请求来设置结果：http://localhost:8083/set/aaa<br />首先先启动模块，可以从控制台中看到完美启动管理了：
```
2018-06-25 00:18:44.379  INFO 12688 --- [           main] com.sunny.DeferredResultApplication      : Starting DeferredResultApplication on xdeMacBook-Pro.local with PID 12688 (/Users/zsunny/IdeaProjects/asynchronoustask/deferredresultdemo2/target/classes started by zsunny in /Users/zsunny/IdeaProjects/asynchronoustask)
2018-06-25 00:18:44.382  INFO 12688 --- [           main] com.sunny.DeferredResultApplication      : No active profile set, falling back to default profiles: default
2018-06-25 00:18:44.489  INFO 12688 --- [           main] ationConfigEmbeddedWebApplicationContext : Refreshing org.springframework.boot.context.embedded.AnnotationConfigEmbeddedWebApplicationContext@96def03: startup date [Mon Jun 25 00:18:44 CST 2018]; root of context hierarchy
2018-06-25 00:18:45.650  INFO 12688 --- [           main] s.b.c.e.t.TomcatEmbeddedServletContainer : Tomcat initialized with port(s): 8083 (http)
2018-06-25 00:18:45.658  INFO 12688 --- [           main] o.apache.catalina.core.StandardService   : Starting service [Tomcat]
2018-06-25 00:18:45.659  INFO 12688 --- [           main] org.apache.catalina.core.StandardEngine  : Starting Servlet Engine: Apache Tomcat/8.5.23
2018-06-25 00:18:45.722  INFO 12688 --- [ost-startStop-1] o.a.c.c.C.[Tomcat].[localhost].[/]       : Initializing Spring embedded WebApplicationContext
2018-06-25 00:18:45.723  INFO 12688 --- [ost-startStop-1] o.s.web.context.ContextLoader            : Root WebApplicationContext: initialization completed in 1241 ms
2018-06-25 00:18:45.817  INFO 12688 --- [ost-startStop-1] o.s.b.w.servlet.ServletRegistrationBean  : Mapping servlet: 'dispatcherServlet' to [/]
2018-06-25 00:18:45.821  INFO 12688 --- [ost-startStop-1] o.s.b.w.servlet.FilterRegistrationBean   : Mapping filter: 'characterEncodingFilter' to: [/*]
2018-06-25 00:18:45.821  INFO 12688 --- [ost-startStop-1] o.s.b.w.servlet.FilterRegistrationBean   : Mapping filter: 'hiddenHttpMethodFilter' to: [/*]
2018-06-25 00:18:45.821  INFO 12688 --- [ost-startStop-1] o.s.b.w.servlet.FilterRegistrationBean   : Mapping filter: 'httpPutFormContentFilter' to: [/*]
2018-06-25 00:18:45.821  INFO 12688 --- [ost-startStop-1] o.s.b.w.servlet.FilterRegistrationBean   : Mapping filter: 'requestContextFilter' to: [/*]
2018-06-25 00:18:46.150  INFO 12688 --- [           main] s.w.s.m.m.a.RequestMappingHandlerAdapter : Looking for @ControllerAdvice: org.springframework.boot.context.embedded.AnnotationConfigEmbeddedWebApplicationContext@96def03: startup date [Mon Jun 25 00:18:44 CST 2018]; root of context hierarchy
2018-06-25 00:18:46.197  INFO 12688 --- [           main] s.w.s.m.m.a.RequestMappingHandlerMapping : Mapped "{[/get],methods=[GET]}" onto public org.springframework.web.context.request.async.DeferredResult<com.sunny.entity.ResponseMsg<java.lang.String>> com.sunny.controller.TaskController.getResult()
2018-06-25 00:18:46.199  INFO 12688 --- [           main] s.w.s.m.m.a.RequestMappingHandlerMapping : Mapped "{[/set/{result}],methods=[GET]}" onto public java.lang.String com.sunny.controller.TaskExecuteController.setResult(java.lang.String)
2018-06-25 00:18:46.202  INFO 12688 --- [           main] s.w.s.m.m.a.RequestMappingHandlerMapping : Mapped "{[/error]}" onto public org.springframework.http.ResponseEntity<java.util.Map<java.lang.String, java.lang.Object>> org.springframework.boot.autoconfigure.web.BasicErrorController.error(javax.servlet.http.HttpServletRequest)
2018-06-25 00:18:46.202  INFO 12688 --- [           main] s.w.s.m.m.a.RequestMappingHandlerMapping : Mapped "{[/error],produces=[text/html]}" onto public org.springframework.web.servlet.ModelAndView org.springframework.boot.autoconfigure.web.BasicErrorController.errorHtml(javax.servlet.http.HttpServletRequest,javax.servlet.http.HttpServletResponse)
2018-06-25 00:18:46.237  INFO 12688 --- [           main] o.s.w.s.handler.SimpleUrlHandlerMapping  : Mapped URL path [/webjars/**] onto handler of type [class org.springframework.web.servlet.resource.ResourceHttpRequestHandler]
2018-06-25 00:18:46.238  INFO 12688 --- [           main] o.s.w.s.handler.SimpleUrlHandlerMapping  : Mapped URL path [/**] onto handler of type [class org.springframework.web.servlet.resource.ResourceHttpRequestHandler]
2018-06-25 00:18:46.262  INFO 12688 --- [           main] o.s.w.s.handler.SimpleUrlHandlerMapping  : Mapped URL path [/**/favicon.ico] onto handler of type [class org.springframework.web.servlet.resource.ResourceHttpRequestHandler]
2018-06-25 00:18:46.362  INFO 12688 --- [           main] o.s.j.e.a.AnnotationMBeanExporter        : Registering beans for JMX exposure on startup
2018-06-25 00:18:46.467  INFO 12688 --- [           main] s.b.c.e.t.TomcatEmbeddedServletContainer : Tomcat started on port(s): 8083 (http)
2018-06-25 00:18:46.472  INFO 12688 --- [           main] com.sunny.DeferredResultApplication      : Started DeferredResultApplication in 2.675 seconds (JVM running for 3.623)
```
完美启动，接下来Sunny在火狐中发起一个请求<br />可以看到正在等待请求结果。随后在谷歌浏览器中发起请求<br />两个请求同时处于等待状态，这时候看一下控制台信息：
```
2018-06-25 00:22:34.642  INFO 12688 --- [nio-8083-exec-6] com.sunny.controller.TaskController      : 接收请求，开始处理...
2018-06-25 00:22:34.642  INFO 12688 --- [nio-8083-exec-6] com.sunny.controller.TaskController      : 加入任务集合，集合大小为:1
2018-06-25 00:22:34.642  INFO 12688 --- [nio-8083-exec-6] com.sunny.controller.TaskController      : 接收任务线程完成并退出
2018-06-25 00:22:37.332  INFO 12688 --- [nio-8083-exec-7] com.sunny.controller.TaskController      : 接收请求，开始处理...
2018-06-25 00:22:37.332  INFO 12688 --- [nio-8083-exec-7] com.sunny.controller.TaskController      : 加入任务集合，集合大小为:2
2018-06-25 00:22:37.332  INFO 12688 --- [nio-8083-exec-7] com.sunny.controller.TaskController      : 接收任务线程完成并退出
```
可以看到两个请求都已经接收到了，并且加入了队列。这时候，再发送一个设置结果的请求。<br />随后查看两个调用请求的页面，发现页面已经不在等待状态中了，都已经得到了结果。<br />另外，再给大家展示一下超时的结果，即发起调用请求，但是不发起设置结果的请求，等待时间结束。<br />查看控制台信息：
```
2018-06-25 00:26:15.898  INFO 12688 --- [nio-8083-exec-4] com.sunny.controller.TaskController      : 接收请求，开始处理...
2018-06-25 00:26:15.898  INFO 12688 --- [nio-8083-exec-4] com.sunny.controller.TaskController      : 加入任务集合，集合大小为:1
2018-06-25 00:26:15.898  INFO 12688 --- [nio-8083-exec-4] com.sunny.controller.TaskController      : 接收任务线程完成并退出
2018-06-25 00:27:16.014  INFO 12688 --- [nio-8083-exec-5] com.sunny.controller.TaskController      : 调用超时，移除任务，此时队列长度为1
2018-06-25 00:27:16.018  INFO 12688 --- [nio-8083-exec-5] com.sunny.controller.TaskController      : 调用完成，移除任务，此时队列长度为0
```
