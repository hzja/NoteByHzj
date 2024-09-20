SpringCloud Feign <br />在 Spring Cloud 中 微服务之间的调用会用到Feign，但是在默认情况下，Feign 调用远程服务存在Header请求头丢失问题。
<a name="02hKR"></a>
## 解决方案
首先需要写一个 Feign请求拦截器，通过实现`RequestInterceptor`接口，完成对所有的Feign请求，传递请求头和请求参数。
<a name="zZEM7"></a>
### Feign 请求拦截器
```java
public class FeignBasicAuthRequestInterceptor implements RequestInterceptor {
    private static final Logger logger = LoggerFactory.getLogger(FeignBasicAuthRequestInterceptor.class);
    @Override
    public void apply(RequestTemplate requestTemplate) {
        ServletRequestAttributes attributes = (ServletRequestAttributes) RequestContextHolder
                .getRequestAttributes();
        HttpServletRequest request = attributes.getRequest();
        Enumeration<String> headerNames = request.getHeaderNames();
        if (headerNames != null) {
            while (headerNames.hasMoreElements()) {
                String name = headerNames.nextElement();
                String values = request.getHeader(name);
                requestTemplate.header(name, values);
            }
        }
        Enumeration<String> bodyNames = request.getParameterNames();
        StringBuffer body =new StringBuffer();
        if (bodyNames != null) {
            while (bodyNames.hasMoreElements()) {
                String name = bodyNames.nextElement();
                String values = request.getParameter(name);
                body.append(name).append("=").append(values).append("&");
            }
        }
        if(body.length()!=0) {
            body.deleteCharAt(body.length()-1);
            requestTemplate.body(body.toString());
            logger.info("feign interceptor body:{}",body.toString());
        }
    }
}
```
配置 让所有 `FeignClient`，使用 `FeignBasicAuthRequestInterceptor`
```yaml
feign:
  client:
    config:
      default:
        connectTimeout: 5000
        readTimeout: 5000
        loggerLevel: basic
        requestInterceptors: com.leparts.config.FeignBasicAuthRequestInterceptor
```
也可以配置让 某个 `FeignClient` 使用这个 `FeignBasicAuthRequestInterceptor`
```yaml
feign:
  client:
    config:
      xxxx: # 远程服务名
        connectTimeout: 5000
        readTimeout: 5000
        loggerLevel: basic
        requestInterceptors: com.leparts.config.FeignBasicAuthRequestInterceptor
```
经过测试，上面的解决方案可以正常的使用；但是出现了新的问题。<br />在转发Feign的请求头的时候，如果开启了Hystrix，Hystrix的默认隔离策略是Thread(线程隔离策略)，因此转发拦截器内是无法获取到请求的请求头信息的。<br />可以修改默认隔离策略为信号量模式：
```
hystrix.command.default.execution.isolation.strategy=SEMAPHORE
```
但信号量模式不是官方推荐的隔离策略;另一个解决方法就是自定义Hystrix的隔离策略。
<a name="xLoCH"></a>
### 自定义策略
`HystrixConcurrencyStrategy` 是提供给开发者去自定义hystrix内部线程池及其队列，还提供了包装callable的方法，以及传递上下文变量的方法。所以可以继承了`HystrixConcurrencyStrategy`，用来实现了自己的并发策略。
```java
@Component
public class FeignHystrixConcurrencyStrategy extends HystrixConcurrencyStrategy {
    private static final Logger log = LoggerFactory.getLogger(FeignHystrixConcurrencyStrategy.class);
    private HystrixConcurrencyStrategy delegate;
    public FeignHystrixConcurrencyStrategy() {
        try {
            this.delegate = HystrixPlugins.getInstance().getConcurrencyStrategy();
            if (this.delegate instanceof FeignHystrixConcurrencyStrategy) {
                // Welcome to singleton hell...
                return;
            }
            HystrixCommandExecutionHook commandExecutionHook =
                    HystrixPlugins.getInstance().getCommandExecutionHook();
            HystrixEventNotifier eventNotifier = HystrixPlugins.getInstance().getEventNotifier();
            HystrixMetricsPublisher metricsPublisher = HystrixPlugins.getInstance().getMetricsPublisher();
            HystrixPropertiesStrategy propertiesStrategy =
                    HystrixPlugins.getInstance().getPropertiesStrategy();
            this.logCurrentStateOfHystrixPlugins(eventNotifier, metricsPublisher, propertiesStrategy);
            HystrixPlugins.reset();
            HystrixPlugins instance = HystrixPlugins.getInstance();
            instance.registerConcurrencyStrategy(this);
            instance.registerCommandExecutionHook(commandExecutionHook);
            instance.registerEventNotifier(eventNotifier);
            instance.registerMetricsPublisher(metricsPublisher);
            instance.registerPropertiesStrategy(propertiesStrategy);
        } catch (Exception e) {
            log.error("Failed to register Sleuth Hystrix Concurrency Strategy", e);
        }
    }
    private void logCurrentStateOfHystrixPlugins(HystrixEventNotifier eventNotifier,
                                                 HystrixMetricsPublisher metricsPublisher,
                                                 HystrixPropertiesStrategy propertiesStrategy) {
        if (log.isDebugEnabled()) {
            log.debug("Current Hystrix plugins configuration is [" + "concurrencyStrategy ["
                    + this.delegate + "]," + "eventNotifier [" + eventNotifier + "]," + "metricPublisher ["
                    + metricsPublisher + "]," + "propertiesStrategy [" + propertiesStrategy + "]," + "]");
            log.debug("Registering Sleuth Hystrix Concurrency Strategy.");
        }
    }
    @Override
    public <T> Callable<T> wrapCallable(Callable<T> callable) {
        RequestAttributes requestAttributes = RequestContextHolder.getRequestAttributes();
        return new WrappedCallable<>(callable, requestAttributes);
    }
    @Override
    public ThreadPoolExecutor getThreadPool(HystrixThreadPoolKey threadPoolKey,
                                            HystrixProperty<Integer> corePoolSize,
                                            HystrixProperty<Integer> maximumPoolSize,
                                            HystrixProperty<Integer> keepAliveTime,
                                            TimeUnit unit, BlockingQueue<Runnable> workQueue) {
        return this.delegate.getThreadPool(threadPoolKey, corePoolSize, maximumPoolSize, keepAliveTime,
                unit, workQueue);
    }
    @Override
    public ThreadPoolExecutor getThreadPool(HystrixThreadPoolKey threadPoolKey,
                                            HystrixThreadPoolProperties threadPoolProperties) {
        return this.delegate.getThreadPool(threadPoolKey, threadPoolProperties);
    }
    @Override
    public BlockingQueue<Runnable> getBlockingQueue(int maxQueueSize) {
        return this.delegate.getBlockingQueue(maxQueueSize);
    }
    @Override
    public <T> HystrixRequestVariable<T> getRequestVariable(HystrixRequestVariableLifecycle<T> rv) {
        return this.delegate.getRequestVariable(rv);
    }
    static class WrappedCallable<T> implements Callable<T> {
        private final Callable<T> target;
        private final RequestAttributes requestAttributes;
        WrappedCallable(Callable<T> target, RequestAttributes requestAttributes) {
            this.target = target;
            this.requestAttributes = requestAttributes;
        }
        @Override
        public T call() throws Exception {
            try {
                RequestContextHolder.setRequestAttributes(requestAttributes);
                return target.call();
            } finally {
                RequestContextHolder.resetRequestAttributes();
            }
        }
    }
}
```
至此，Feign调用丢失请求头的问题就解决了 。
