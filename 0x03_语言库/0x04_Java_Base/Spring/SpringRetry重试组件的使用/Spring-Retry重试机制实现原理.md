Spring Retry
<a name="C64E7"></a>
## 概要
Spring实现了一套重试机制，功能简单实用。Spring Retry是从Spring Batch独立出来的一个功能，已经广泛应用于Spring Batch，Spring Integration，Spring for Apache Hadoop等Spring项目。这里讲述如何使用Spring Retry及其实现原理。
<a name="wuEr0"></a>
## 背景
重试其实其实很多时候都需要的，为了保证容错性，可用性，一致性等。一般用来应对外部系统的一些不可预料的返回、异常等，特别是网络延迟，中断等情况。还有在现在流行的微服务治理框架中，通常都有自己的重试与超时配置，比如dubbo可以设置retries=1，timeout=500调用失败只重试1次，超过500ms调用仍未返回则调用失败。如果要做重试，要为特定的某个操作做重试功能，则要硬编码，大概逻辑基本都是写个循环，根据返回或异常，计数失败次数，然后设定退出条件。这样做，且不说每个操作都要写这种类似的代码，而且重试逻辑和业务逻辑混在一起，给维护和扩展带来了麻烦。从面向对象的角度来看，应该把重试的代码独立出来。
<a name="kxK4k"></a>
## 使用介绍
<a name="MoC4T"></a>
### 基本使用
先举个例子：
```java
@Configuration  
@EnableRetry  
public class Application {  

    @Bean  
    public RetryService retryService(){  
        return new RetryService();  
    }  

    public static void main(String[] args) throws Exception{  
        ApplicationContext applicationContext = new AnnotationConfigApplicationContext("springretry");  
        RetryService service1 = applicationContext.getBean("service", RetryService.class);  
        service1.service();  
    }  
}  

@Service("service")  
public class RetryService {  

    @Retryable(value = IllegalAccessException.class, maxAttempts = 5,  
               backoff= @Backoff(value = 1500, maxDelay = 100000, multiplier = 1.2))  
    public void service() throws IllegalAccessException {  
        System.out.println("service method...");  
        throw new IllegalAccessException("manual exception");  
    }  

    @Recover  
    public void recover(IllegalAccessException e){  
        System.out.println("service retry after Recover => " + e.getMessage());  
    }  
}
```
`@EnableRetry` - 表示开启重试机制 `@Retryable` - 表示这个方法需要重试，它有很丰富的参数，可以满足对重试的需求 `@Backoff` - 表示重试中的退避策略 `@Recover` - 兜底方法，即多次重试后还是失败就会执行这个方法Spring-Retry 的功能丰富在于其重试策略和退避策略，还有兜底，监听器等操作。
<a name="8YHIw"></a>
### 重试策略
看一下Spring Retry自带的一些重试策略，主要是用来判断当方法调用异常时是否需要重试。

- `SimpleRetryPolicy` 默认最多重试3次
- `TimeoutRetryPolicy` 默认在1秒内失败都会重试
- `ExpressionRetryPolicy` 符合表达式就会重试
- `CircuitBreakerRetryPolicy` 增加了熔断的机制，如果不在熔断状态，则允许重试
- `CompositeRetryPolicy` 可以组合多个重试策略
- `NeverRetryPolicy` 从不重试（也是一种重试策略）
- `AlwaysRetryPolicy` 总是重试
<a name="ccnUe"></a>
### 退避策略
看一下退避策略，退避是指怎么去做下一次的重试，在这里其实就是等待多长时间。![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1608081416585-2a67094c-bf2c-405b-9759-65db3296fb1e.webp#height=419&id=Yqd71&originHeight=419&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=1080)

- `FixedBackOffPolicy` 默认固定延迟1秒后执行下一次重试
- `ExponentialBackOffPolicy` 指数递增延迟执行重试，默认初始0.1秒，系数是2，那么下次延迟0.2秒，再下次就是延迟0.4秒，如此类推，最大30秒。
- `ExponentialRandomBackOffPolicy` 在上面那个策略上增加随机性
- `UniformRandomBackOffPolicy` 这个跟上面的区别就是，上面的延迟会不停递增，这个只会在固定的区间随机
- `StatelessBackOffPolicy` 这个说明是无状态的，所谓无状态就是对上次的退避无感知，从它下面的子类也能看出来
<a name="B8GmB"></a>
## 原理
原理部分，一是重试机制的切入点，即它是如何使得代码实现重试功能的；二是重试机制的详细，包括重试的逻辑以及重试策略和退避策略的实现。
<a name="TWu3u"></a>
### 切入点
<a name="0v9A3"></a>
#### `@EnableRetry`
```java
@Target(ElementType.TYPE)  
@Retention(RetentionPolicy.RUNTIME)  
@EnableAspectJAutoProxy(proxyTargetClass = false)  
@Import(RetryConfiguration.class)  
@Documented  
public @interface EnableRetry {  

    /**  
     * Indicate whether subclass-based (CGLIB) proxies are to be created as opposed  
     * to standard Java interface-based proxies. The default is {@code false}.  
     *  
     * @return whether to proxy or not to proxy the class  
     */  
    boolean proxyTargetClass() default false;  

}
```
可以看到`@EnableAspectJAutoProxy(proxyTargetClass = false)`这个并不陌生，就是打开Spring AOP功能。重点看看`@Import(RetryConfiguration.class)`，`@Import`相当于注册这个Bean，看看这个`RetryConfiguration`![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1608081416536-a0a57d0d-bdfa-4379-800e-885b3595577d.webp#height=328&id=yoIU3&originHeight=328&originWidth=998&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=998)它是一个`AbstractPointcutAdvisor`，它有一个pointcut和一个advice。在IOC过程中会根据`PointcutAdvisor`类来对Bean进行`Pointcut`的过滤，然后生成对应的AOP代理类，用advice来加强处理。看看`RetryConfiguration`的初始化:
```java
@PostConstruct  
public void init() {  
    Set<Class<? extends Annotation>> retryableAnnotationTypes = new LinkedHashSet<Class<? extends Annotation>>(1);  
    retryableAnnotationTypes.add(Retryable.class);  
    //创建pointcut  
    this.pointcut = buildPointcut(retryableAnnotationTypes);  
    //创建advice  
    this.advice = buildAdvice();  
    if (this.advice instanceof BeanFactoryAware) {  
        ((BeanFactoryAware) this.advice).setBeanFactory(beanFactory);  
    }  
}  

protected Pointcut buildPointcut(Set<Class<? extends Annotation>> retryAnnotationTypes) {  
    ComposablePointcut result = null;  
    for (Class<? extends Annotation> retryAnnotationType : retryAnnotationTypes) {  
        Pointcut filter = new AnnotationClassOrMethodPointcut(retryAnnotationType);  
        if (result == null) {  
            result = new ComposablePointcut(filter);  
        }  
        else {  
            result.union(filter);  
        }  
    }  
    return result;  
}
```
上面代码用到了`AnnotationClassOrMethodPointcut`，其实它最终还是用到了`AnnotationMethodMatcher`来根据注解进行切入点的过滤。这里就是`@Retryable`注解了。
```java
//创建advice对象，即拦截器  
protected Advice buildAdvice() {  
    //下面关注这个对象  
    AnnotationAwareRetryOperationsInterceptor interceptor = new AnnotationAwareRetryOperationsInterceptor();  
    if (retryContextCache != null) {  
        interceptor.setRetryContextCache(retryContextCache);  
    }  
    if (retryListeners != null) {  
        interceptor.setListeners(retryListeners);  
    }  
    if (methodArgumentsKeyGenerator != null) {  
        interceptor.setKeyGenerator(methodArgumentsKeyGenerator);  
    }  
    if (newMethodArgumentsIdentifier != null) {  
        interceptor.setNewItemIdentifier(newMethodArgumentsIdentifier);  
    }  
    if (sleeper != null) {  
        interceptor.setSleeper(sleeper);  
    }  
    return interceptor;  
}
```
<a name="Irky4"></a>
#### `AnnotationAwareRetryOperationsInterceptor`
![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1608081417007-c92d691a-1fab-42dd-8d6f-90dfddc72b83.webp#height=290&id=qYgyf&originHeight=290&originWidth=1046&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=1046)可以看出`AnnotationAwareRetryOperationsInterceptor`是一个`MethodInterceptor`，在创建AOP代理过程中如果目标方法符合`pointcut`的规则，它就会加到`interceptor`列表中，然后做增强，看看`invoke`方法做了什么增强。
```java
@Override  
public Object invoke(MethodInvocation invocation) throws Throwable {  
    MethodInterceptor delegate = getDelegate(invocation.getThis(), invocation.getMethod());  
    if (delegate != null) {  
        return delegate.invoke(invocation);  
    }  
    else {  
        return invocation.proceed();  
    }  
}
```
这里用到了委托，主要是需要根据配置委托给具体“有状态”的`interceptor`还是“无状态”的`interceptor`。
```java
private MethodInterceptor getDelegate(Object target, Method method) {  
    if (!this.delegates.containsKey(target) || !this.delegates.get(target).containsKey(method)) {  
        synchronized (this.delegates) {  
            if (!this.delegates.containsKey(target)) {  
                this.delegates.put(target, new HashMap<Method, MethodInterceptor>());  
            }  
            Map<Method, MethodInterceptor> delegatesForTarget = this.delegates.get(target);  
            if (!delegatesForTarget.containsKey(method)) {  
                Retryable retryable = AnnotationUtils.findAnnotation(method, Retryable.class);  
                if (retryable == null) {  
                    retryable = AnnotationUtils.findAnnotation(method.getDeclaringClass(), Retryable.class);  
                }  
                if (retryable == null) {  
                    retryable = findAnnotationOnTarget(target, method);  
                }  
                if (retryable == null) {  
                    return delegatesForTarget.put(method, null);  
                }  
                MethodInterceptor delegate;  
                //支持自定义MethodInterceptor，而且优先级最高  
                if (StringUtils.hasText(retryable.interceptor())) {  
                    delegate = this.beanFactory.getBean(retryable.interceptor(), MethodInterceptor.class);  
                }  
                else if (retryable.stateful()) {  
                    //得到“有状态”的interceptor  
                    delegate = getStatefulInterceptor(target, method, retryable);  
                }  
                else {  
                    //得到“无状态”的interceptor  
                    delegate = getStatelessInterceptor(target, method, retryable);  
                }  
                delegatesForTarget.put(method, delegate);  
            }  
        }  
    }  
    return this.delegates.get(target).get(method);  
}
```
`getStatefulInterceptor`和`getStatelessInterceptor`都是差不多，先看看比较简单的`getStatelessInterceptor`。
```java
private MethodInterceptor getStatelessInterceptor(Object target, Method method, Retryable retryable) {  
    //生成一个RetryTemplate  
    RetryTemplate template = createTemplate(retryable.listeners());  
    //生成retryPolicy  
    template.setRetryPolicy(getRetryPolicy(retryable));  
    //生成backoffPolicy  
    template.setBackOffPolicy(getBackoffPolicy(retryable.backoff()));  
    return RetryInterceptorBuilder.stateless()  
        .retryOperations(template)  
        .label(retryable.label())  
        .recoverer(getRecoverer(target, method))  
        .build();  
}
```
具体生成`retryPolicy`和`backoffPolicy`的规则，等下再回头来看。`RetryInterceptorBuilder`其实就是为了生成`RetryOperationsInterceptor`。`RetryOperationsInterceptor`也是一个`MethodInterceptor`，来看看它的`invoke`方法。
```java
public Object invoke(final MethodInvocation invocation) throws Throwable {  

    String name;  
    if (StringUtils.hasText(label)) {  
        name = label;  
    } else {  
        name = invocation.getMethod().toGenericString();  
    }  
    final String label = name;  

    //定义了一个RetryCallback，其实看它的doWithRetry方法，调用了invocation的proceed()方法，是不是有点眼熟，这就是AOP的拦截链调用，如果没有拦截链，那就是对原来方法的调用。  
    RetryCallback<Object, Throwable> retryCallback = new RetryCallback<Object, Throwable>() {  

        public Object doWithRetry(RetryContext context) throws Exception {  

            context.setAttribute(RetryContext.NAME, label);  

            /*  
             * If we don't copy the invocation carefully it won't keep a reference to  
             * the other interceptors in the chain. We don't have a choice here but to  
             * specialise to ReflectiveMethodInvocation (but how often would another  
             * implementation come along?).  
             */  
            if (invocation instanceof ProxyMethodInvocation) {  
                try {  
                    return ((ProxyMethodInvocation) invocation).invocableClone().proceed();  
                }  
                catch (Exception e) {  
                    throw e;  
                }  
                catch (Error e) {  
                    throw e;  
                }  
                catch (Throwable e) {  
                    throw new IllegalStateException(e);  
                }  
            }  
            else {  
                throw new IllegalStateException(  
                    "MethodInvocation of the wrong type detected - this should not happen with Spring AOP, " +  
                    "so please raise an issue if you see this exception");  
            }  
        }  

    };  

    if (recoverer != null) {  
        ItemRecovererCallback recoveryCallback = new ItemRecovererCallback(  
            invocation.getArguments(), recoverer);  
        return this.retryOperations.execute(retryCallback, recoveryCallback);  
    }  
    //最终还是进入到retryOperations的execute方法，这个retryOperations就是在之前的builder set进来的RetryTemplate。  
    return this.retryOperations.execute(retryCallback);  

}
```
无论是`RetryOperationsInterceptor`还是`StatefulRetryOperationsInterceptor`，最终的拦截处理逻辑还是调用到`RetryTemplate`的`execute`方法，从名字也看出来，`RetryTemplate`作为一个模板类，里面包含了重试统一逻辑。
<a name="6pWTT"></a>
### 重试逻辑及策略实现
上面介绍了Spring Retry利用了AOP代理使重试机制对业务代码进行“入侵”。下面继续看看重试的逻辑做了什么。`RetryTemplate`的`doExecute`方法。
```java
protected <T, E extends Throwable> T doExecute(RetryCallback<T, E> retryCallback,  
                                               RecoveryCallback<T> recoveryCallback, RetryState state)  
    throws E, ExhaustedRetryException {  

    RetryPolicy retryPolicy = this.retryPolicy;  
    BackOffPolicy backOffPolicy = this.backOffPolicy;  

    //新建一个RetryContext来保存本轮重试的上下文  
    RetryContext context = open(retryPolicy, state);  
    if (this.logger.isTraceEnabled()) {  
        this.logger.trace("RetryContext retrieved: " + context);  
    }  

    // Make sure the context is available globally for clients who need  
    // it...  
    RetrySynchronizationManager.register(context);  

    Throwable lastException = null;  

    boolean exhausted = false;  
    try {  

        //如果有注册RetryListener，则会调用它的open方法，给调用者一个通知。  
        boolean running = doOpenInterceptors(retryCallback, context);  

        if (!running) {  
            throw new TerminatedRetryException(  
                "Retry terminated abnormally by interceptor before first attempt");  
        }  

        // Get or Start the backoff context...  
        BackOffContext backOffContext = null;  
        Object resource = context.getAttribute("backOffContext");  

        if (resource instanceof BackOffContext) {  
            backOffContext = (BackOffContext) resource;  
        }  

        if (backOffContext == null) {  
            backOffContext = backOffPolicy.start(context);  
            if (backOffContext != null) {  
                context.setAttribute("backOffContext", backOffContext);  
            }  
        }  

        //判断能否重试，就是调用RetryPolicy的canRetry方法来判断。  
        //这个循环会直到原方法不抛出异常，或不需要再重试  
        while (canRetry(retryPolicy, context) && !context.isExhaustedOnly()) {  

            try {  
                if (this.logger.isDebugEnabled()) {  
                    this.logger.debug("Retry: count=" + context.getRetryCount());  
                }  
                //清除上次记录的异常  
                lastException = null;  
                //doWithRetry方法，一般来说就是原方法  
                return retryCallback.doWithRetry(context);  
            }  
            catch (Throwable e) {  
                //原方法抛出了异常  
                lastException = e;  

                try {  
                    //记录异常信息  
                    registerThrowable(retryPolicy, state, context, e);  
                }  
                catch (Exception ex) {  
                    throw new TerminatedRetryException("Could not register throwable",  
                                                       ex);  
                }  
                finally {  
                    //调用RetryListener的onError方法  
                    doOnErrorInterceptors(retryCallback, context, e);  
                }  
                //再次判断能否重试  
                if (canRetry(retryPolicy, context) && !context.isExhaustedOnly()) {  
                    try {  
                        //如果可以重试则走退避策略  
                        backOffPolicy.backOff(backOffContext);  
                    }  
                    catch (BackOffInterruptedException ex) {  
                        lastException = e;  
                        // back off was prevented by another thread - fail the retry  
                        if (this.logger.isDebugEnabled()) {  
                            this.logger  
                                .debug("Abort retry because interrupted: count="  
                                       + context.getRetryCount());  
                        }  
                        throw ex;  
                    }  
                }  

                if (this.logger.isDebugEnabled()) {  
                    this.logger.debug(  
                        "Checking for rethrow: count=" + context.getRetryCount());  
                }  

                if (shouldRethrow(retryPolicy, context, state)) {  
                    if (this.logger.isDebugEnabled()) {  
                        this.logger.debug("Rethrow in retry for policy: count="  
                                          + context.getRetryCount());  
                    }  
                    throw RetryTemplate.<E>wrapIfNecessary(e);  
                }  

            }  

            /*  
             * A stateful attempt that can retry may rethrow the exception before now,  
             * but if we get this far in a stateful retry there's a reason for it,  
             * like a circuit breaker or a rollback classifier.  
             */  
            if (state != null && context.hasAttribute(GLOBAL_STATE)) {  
                break;  
            }  
        }  

        if (state == null && this.logger.isDebugEnabled()) {  
            this.logger.debug(  
                "Retry failed last attempt: count=" + context.getRetryCount());  
        }  

        exhausted = true;  
        //重试结束后如果有兜底Recovery方法则执行，否则抛异常  
        return handleRetryExhausted(recoveryCallback, context, state);  

    }  
    catch (Throwable e) {  
        throw RetryTemplate.<E>wrapIfNecessary(e);  
    }  
    finally {  
        //处理一些关闭逻辑  
        close(retryPolicy, context, state, lastException == null || exhausted);  
        //调用RetryListener的close方法  
        doCloseInterceptors(retryCallback, context, lastException);  
        RetrySynchronizationManager.clear();  
    }  
}
```
主要核心重试逻辑就是上面的代码了，看上去还是挺简单的。在上面，漏掉了`RetryPolicy`的`canRetry`方法和`BackOffPolicy的backOff`方法，以及这两个Policy是怎么来的。回头看看`getStatelessInterceptor`方法中的`getRetryPolicy`和`getRetryPolicy`方法。
```java
private RetryPolicy getRetryPolicy(Annotation retryable) {  
    Map<String, Object> attrs = AnnotationUtils.getAnnotationAttributes(retryable);  
    @SuppressWarnings("unchecked")  
    Class<? extends Throwable>[] includes = (Class<? extends Throwable>[]) attrs.get("value");  
    String exceptionExpression = (String) attrs.get("exceptionExpression");  
    boolean hasExpression = StringUtils.hasText(exceptionExpression);  
    if (includes.length == 0) {  
        @SuppressWarnings("unchecked")  
        Class<? extends Throwable>[] value = (Class<? extends Throwable>[]) attrs.get("include");  
        includes = value;  
    }  
    @SuppressWarnings("unchecked")  
    Class<? extends Throwable>[] excludes = (Class<? extends Throwable>[]) attrs.get("exclude");  
    Integer maxAttempts = (Integer) attrs.get("maxAttempts");  
    String maxAttemptsExpression = (String) attrs.get("maxAttemptsExpression");  
    if (StringUtils.hasText(maxAttemptsExpression)) {  
        maxAttempts = PARSER.parseExpression(resolve(maxAttemptsExpression), PARSER_CONTEXT)  
            .getValue(this.evaluationContext, Integer.class);  
    }  
    if (includes.length == 0 && excludes.length == 0) {  
        SimpleRetryPolicy simple = hasExpression ? new ExpressionRetryPolicy(resolve(exceptionExpression))  
            .withBeanFactory(this.beanFactory)  
            : new SimpleRetryPolicy();  
        simple.setMaxAttempts(maxAttempts);  
        return simple;  
    }  
    Map<Class<? extends Throwable>, Boolean> policyMap = new HashMap<Class<? extends Throwable>, Boolean>();  
    for (Class<? extends Throwable> type : includes) {  
        policyMap.put(type, true);  
    }  
    for (Class<? extends Throwable> type : excludes) {  
        policyMap.put(type, false);  
    }  
    boolean retryNotExcluded = includes.length == 0;  
    if (hasExpression) {  
        return new ExpressionRetryPolicy(maxAttempts, policyMap, true, exceptionExpression, retryNotExcluded)  
            .withBeanFactory(this.beanFactory);  
    }  
    else {  
        return new SimpleRetryPolicy(maxAttempts, policyMap, true, retryNotExcluded);  
    }  
}
```
这里简单做一下总结。就是通过`@Retryable`注解中的参数，来判断具体使用哪个重试策略，是`SimpleRetryPolicy`还是`ExpressionRetryPolicy`等。
```java
private BackOffPolicy getBackoffPolicy(Backoff backoff) {  
    long min = backoff.delay() == 0 ? backoff.value() : backoff.delay();  
    if (StringUtils.hasText(backoff.delayExpression())) {  
        min = PARSER.parseExpression(resolve(backoff.delayExpression()), PARSER_CONTEXT)  
            .getValue(this.evaluationContext, Long.class);  
    }  
    long max = backoff.maxDelay();  
    if (StringUtils.hasText(backoff.maxDelayExpression())) {  
        max = PARSER.parseExpression(resolve(backoff.maxDelayExpression()), PARSER_CONTEXT)  
            .getValue(this.evaluationContext, Long.class);  
    }  
    double multiplier = backoff.multiplier();  
    if (StringUtils.hasText(backoff.multiplierExpression())) {  
        multiplier = PARSER.parseExpression(resolve(backoff.multiplierExpression()), PARSER_CONTEXT)  
            .getValue(this.evaluationContext, Double.class);  
    }  
    if (multiplier > 0) {  
        ExponentialBackOffPolicy policy = new ExponentialBackOffPolicy();  
        if (backoff.random()) {  
            policy = new ExponentialRandomBackOffPolicy();  
        }  
        policy.setInitialInterval(min);  
        policy.setMultiplier(multiplier);  
        policy.setMaxInterval(max > min ? max : ExponentialBackOffPolicy.DEFAULT_MAX_INTERVAL);  
        if (this.sleeper != null) {  
            policy.setSleeper(this.sleeper);  
        }  
        return policy;  
    }  
    if (max > min) {  
        UniformRandomBackOffPolicy policy = new UniformRandomBackOffPolicy();  
        policy.setMinBackOffPeriod(min);  
        policy.setMaxBackOffPeriod(max);  
        if (this.sleeper != null) {  
            policy.setSleeper(this.sleeper);  
        }  
        return policy;  
    }  
    FixedBackOffPolicy policy = new FixedBackOffPolicy();  
    policy.setBackOffPeriod(min);  
    if (this.sleeper != null) {  
        policy.setSleeper(this.sleeper);  
    }  
    return policy;  
}
```
通过`@Backoff`注解中的参数，来判断具体使用哪个退避策略，是`FixedBackOffPolicy`还是`UniformRandomBackOffPolicy`等。那么每个`RetryPolicy`都会重写`canRetry`方法，然后在`RetryTemplate`判断是否需要重试。看看`SimpleRetryPolicy`的
```java
@Override  
public boolean canRetry(RetryContext context) {  
    Throwable t = context.getLastThrowable();  
    //判断抛出的异常是否符合重试的异常  
    //还有，是否超过了重试的次数  
    return (t == null || retryForException(t)) && context.getRetryCount() < maxAttempts;  
}  
```
同样，看看`FixedBackOffPolicy`的退避方法。
```java
protected void doBackOff() throws BackOffInterruptedException {  
    try {  
        //就是sleep固定的时间  
        sleeper.sleep(backOffPeriod);  
    }  
    catch (InterruptedException e) {  
        throw new BackOffInterruptedException("Thread interrupted while sleeping", e);  
    }  
}
```
至此，重试的主要原理以及逻辑大概就是这样了。
<a name="Ob12D"></a>
### `RetryContext`
先看看`RetryContext`的继承关系。![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1608081416592-f510f8fc-e885-4e07-813b-d9b8ca76e42c.webp#height=324&id=NN5Iz&originHeight=324&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=1080)可以看出对每一个策略都有对应的Context。在Spring Retry里，其实每一个策略都是单例来的。Spring Retry采用了一个更加轻量级的做法，就是针对每一个需要重试的方法只new一个上下文Context对象，然后在重试时，把这个Context传到策略里，策略再根据这个Context做重试，而且Spring Retry还对这个Context做了cache。这样就相当于对重试的上下文做了优化。
<a name="Na6pB"></a>
## 总结
Spring Retry通过AOP机制来实现对业务代码的重试”入侵“，`RetryTemplate`中包含了核心的重试逻辑，还提供了丰富的重试策略和退避策略。
