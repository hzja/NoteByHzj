JavaSpringBootMDC<br />出现故障时，经常需要获取一次请求流程里的所有日志进行**定位**。<br />如果请求只在一个线程里处理，则可以通过线程ID来**过滤日志**，但如果请求包含异步线程的处理，那么光靠线程ID就显得捉襟见肘了。<br />华为IoT平台，提供了接收设备上报数据的能力， 当数据到达平台后，平台会进行一些复杂的业务逻辑处理，如数据存储，规则引擎，数据推送，命令下发等等。由于这个逻辑之间没有强耦合的关系，所以通常是异步处理。如何将一次数据上报请求中包含的所有业务日志快速过滤出来，就是本文要介绍的。
<a name="QoG3l"></a>
## 1、正文
SLF4J日志框架提供了一个MDC(Mapped Diagnostic Contexts)工具类，谷歌翻译为**映射的诊断上下文** ，从字面上很难理解，可以先实战一把。
```java
public class Main {

    private static final String KEY = "requestId";
    private static final Logger logger = LoggerFactory.getLogger(Main.class);

    public static void main(String[] args) {

        // 入口传入请求ID
        MDC.put(KEY, UUID.randomUUID().toString());

        // 打印日志
        logger.debug("log in main thread 1");
        logger.debug("log in main thread 2");
        logger.debug("log in main thread 3");

        // 出口移除请求ID
        MDC.remove(KEY);

    }

}
```
在main函数的入口调用`MDC.put()`方法传入请求ID，在出口调用`MDC.remove()`方法移除请求ID。配置好**log4j2.xml** 文件后，运行main函数，可以在控制台看到以下日志输出：
```
2018-02-17 13:19:52.606 {requestId=f97ea0fb-2a43-40f4-a3e8-711f776857d0} [main] DEBUG cn.wudashan.Main - log in main thread 1
2018-02-17 13:19:52.609 {requestId=f97ea0fb-2a43-40f4-a3e8-711f776857d0} [main] DEBUG cn.wudashan.Main - log in main thread 2
2018-02-17 13:19:52.609 {requestId=f97ea0fb-2a43-40f4-a3e8-711f776857d0} [main] DEBUG cn.wudashan.Main - log in main thread 3
```
从日志中可以明显地看到花括号中包含了 （映射的）请求ID(requestId)，这其实就是定位（诊断）问题的关键字（上下文） 。有了MDC工具，只要在接口或切面植入`put()`和`remove()`代码，在现网定位问题时，就可以通过grep requestId=xxx *.log快速的过滤出某次请求的所有日志。
<a name="x9YVA"></a>
## 2、进阶
然而，MDC工具真的有这么方便吗？回到开头，一次请求可能涉及多线程异步处理，那么在多线程异步的场景下，它是否还能正常运作呢？Talk is cheap, show me the code。
```java
public class Main {

    private static final String KEY = "requestId";
    private static final Logger logger = LoggerFactory.getLogger(Main.class);

    public static void main(String[] args) {

        // 入口传入请求ID
        MDC.put(KEY, UUID.randomUUID().toString());

        // 主线程打印<font style="color: #1e6bb8;word-wrap: break-word;font-weight: bold;border-bottom: 1px solid">日志</font>
        logger.debug("log in main thread");

        // 异步线程打印<font style="color: #1e6bb8;word-wrap: break-word;font-weight: bold;border-bottom: 1px solid">日志</font>
        new Thread(new Runnable() {
            @Override
            public void run() {
                logger.debug("log in other thread");
            }
        }).start();

        // 出口移除请求ID
        MDC.remove(KEY);

    }

}
```
代码里新起了一个异步线程，并在匿名对象`Runnable`的`run()`方法打印日志。运行main函数，可以在控制台看到以下日志输出：
```
2018-02-17 14:05:43.487 {requestId=e6099c85-72be-4986-8a28-de6bb2e52b01} [main] DEBUG cn.wudashan.Main - log in main thread
2018-02-17 14:05:43.490 {} [Thread-1] DEBUG cn.wudashan.Main - log in other thread
```
不幸的是，请求ID在异步线程里不打印了。这是怎么回事呢？<br />要解决这个问题，就得知道MDC的实现原理。MDC之所以在异步线程中不生效是因为底层采用**ThreadLocal** 作为数据结构，调用`MDC.put()`方法传入的请求ID只在当前线程有效。<br />可以自己深入一下代码细节。<br />知道了原理那么解决这个问题就轻而易举了，可以使用**装饰器模式** ，新写一个MDCRunnable类对Runnable接口进行一层装饰。<br />在创建MDCRunnable类时保存当前线程的MDC值，在执行`run()`方法时再将保存的MDC值拷贝到异步线程中去。<br />代码实现如下：
```java
public class MDCRunnable implements Runnable {

    private final Runnable runnable;

    private final Map<String, String> map;

    public MDCRunnable(Runnable runnable) {
        this.runnable = runnable;
        // 保存当前线程的MDC值
        this.map = MDC.getCopyOfContextMap();
    }

    @Override
    public void run() {
        // 传入已保存的MDC值
        for (Map.Entry<String, String> entry : map.entrySet()) {
            MDC.put(entry.getKey(), entry.getValue());
        }
        // 装饰器模式，执行run方法
        runnable.run();
        // 移除已保存的MDC值
        for (Map.Entry<String, String> entry : map.entrySet()) {
            MDC.remove(entry.getKey());
        }
    }

}
```
接着，需要对main函数里创建的Runnable实现类进行装饰：
```java
public class Main {

    private static final String KEY = "requestId";
    private static final Logger logger = LoggerFactory.getLogger(Main.class);
    private static final ExecutorService EXECUTOR = Executors.newSingleThreadExecutor();

    public static void main(String[] args) {

        // 入口传入请求ID
        MDC.put(KEY, UUID.randomUUID().toString());

        // 主线程打印日志
        logger.debug("log in main thread");

        // 异步线程打印日志，用MDCRunnable装饰Runnable
        new Thread(new MDCRunnable(new Runnable() {
            @Override
            public void run() {
                logger.debug("log in other thread");
            }
        })).start();

        // 异步线程池打印日志，用MDCRunnable装饰Runnable
        EXECUTOR.execute(new MDCRunnable(new Runnable() {
            @Override
            public void run() {
                logger.debug("log in other thread pool");
            }
        }));
        EXECUTOR.shutdown();

        // 出口移除请求ID
        MDC.remove(KEY);

    }

}
```
执行main函数，将会输出以下**日志**：
```
2018-03-04 23:44:05.343 {requestId=5ee2a117-e090-41d8-977b-cef5dea09d34} [main] DEBUG cn.wudashan.Main - log in main thread
2018-03-04 23:44:05.346 {requestId=5ee2a117-e090-41d8-977b-cef5dea09d34} [Thread-1] DEBUG cn.wudashan.Main - log in other thread
2018-03-04 23:44:05.347 {requestId=5ee2a117-e090-41d8-977b-cef5dea09d34} [pool-2-thread-1] DEBUG cn.wudashan.Main - log in other thread pool
```
**Congratulations！** 经过不懈的努力，最终在异步线程和线程池中都有requestId打印了！
<a name="CvJVx"></a>
## 3、总结
讲述了如何使用MDC工具来快速过滤一次请求的所有**日志**，并通过装饰器模式使得MDC工具在异步线程里也能生效。<br />有了MDC，再通过AOP技术对所有的切面植入requestId，就可以将整个系统的任意流程的日志过滤出来。<br />使用MDC工具，在开发自测阶段，可以极大地节省定位问题的时间，提升开发效率；在运维维护阶段，可以快速地收集相关日志信息，加快分析速度。
