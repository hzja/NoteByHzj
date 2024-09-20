Java<br />随着互联网的发展项目中的业务功能越来越复杂，有一些基础服务不可避免的会去调用一些第三方的接口或者公司内其他项目中提供的服务，但是远程服务的健壮性和网络稳定性都是不可控因素。<br />在测试阶段可能没有什么异常情况，但上线后可能会出现调用的接口因为内部错误或者网络波动而出错或返回系统异常，因此必须考虑加上重试机制。<br />重试机制 可以提高系统的健壮性，并且减少因网络波动依赖服务临时不可用带来的影响，让系统能更稳定的运行。
<a name="G5BX9"></a>
## **1、手动重试**
手动重试：使用 while 语句进行重试：
```java
@Service
public class OrderServiceImpl implements OrderService {
    public void addOrder() {
        int times = 1;
        while (times <= 5) {
            try {
                // 故意抛异常
                int i = 3 / 0;
                // addOrder
            } catch (Exception e) {
                System.out.println("重试" + times + "次");
                Thread.sleep(2000);
                times++;
                if (times > 5) {
                    throw new RuntimeException("不再重试！");
                }
            }
        }
    }
}
```
上述代码看上去可以解决重试问题，但实际上存在一些弊端：

- 由于没有重试间隔，很可能远程调用的服务还没有从网络异常中恢复，所以有可能接下来的几次调用都会失败
- 代码侵入式太高，调用方代码不够优雅
- 项目中远程调用的服务可能有很多，每个都去添加重试会出现大量的重复代码
<a name="ccGuz"></a>
## **2、静态代理**
上面的处理方式由于需要对业务代码进行大量修改，虽然实现了功能，但是对原有代码的侵入性太强，可维护性差。所以需要使用一种更优雅一点的方式，不直接修改业务代码，那要怎么做呢？<br />其实很简单，直接在业务代码的外面再包一层就行了，代理模式在这里就有用武之地了。
```java
@Service
public class OrderServiceProxyImpl implements OrderService {

    @Autowired
    private OrderServiceImpl orderService;

    @Override
    public void addOrder() {
        int times = 1;
        while (times <= 5) {
            try {
                // 故意抛异常
                int i = 3 / 0;
                orderService.addOrder();
            } catch (Exception e) {
                System.out.println("重试" + times + "次");
                try {
                    Thread.sleep(2000);
                } catch (InterruptedException ex) {
                    ex.printStackTrace();
                }
                times++;
                if (times > 5) {
                    throw new RuntimeException("不再重试！");
                }
            }
        }

    }
}
```
这样，重试逻辑就都由代理类来完成，原业务类的逻辑就不需要修改了，以后想修改重试逻辑也只需要修改这个类就行了<br />代理模式虽然要更加优雅，但是如果依赖的服务很多的时候，要为每个服务都创建一个代理类，显然过于麻烦，而且其实重试的逻辑都大同小异，无非就是重试的次数和延时不一样而已。如果每个类都写这么一长串类似的代码，显然，不优雅！
<a name="hJ8r2"></a>
## **3、JDK 动态代理**
这时候，动态代理就闪亮登场了。只需要写一个代理处理类就 ok 了
```java
public class RetryInvocationHandler implements InvocationHandler {

    private final Object subject;

    public RetryInvocationHandler(Object subject) {
        this.subject = subject;
    }

    @Override
    public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
        int times = 1;
        while (times <= 5) {
            try {
                // 故意抛异常
                int i = 3 / 0;
                return method.invoke(subject, args);
            } catch (Exception e) {
                System.out.println("重试【" + times + "】次");
                try {
                    Thread.sleep(2000);
                } catch (InterruptedException ex) {
                    ex.printStackTrace();
                }
                times++;
                if (times > 5) {
                    throw new RuntimeException("不再重试！");
                }
            }
        }
        return null;
    }

    public static Object getProxy(Object realSubject) {
        InvocationHandler handler = new RetryInvocationHandler(realSubject);
        return Proxy.newProxyInstance(handler.getClass().getClassLoader(), realSubject.getClass().getInterfaces(), handler);
    }

}
```
测试：
```java
@RestController
@RequestMapping("/order")
public class OrderController {

    @Qualifier("orderServiceImpl")
    @Autowired
    private OrderService orderService;

    @GetMapping("/addOrder")
    public String addOrder() {
        OrderService orderServiceProxy = (OrderService)RetryInvocationHandler.getProxy(orderService);
        orderServiceProxy.addOrder();
        return "addOrder";
    }

}
```
动态代理可以将重试逻辑都放到一块，显然比直接使用代理类要方便很多，也更加优雅。<br />这里使用的是JDK动态代理，因此就存在一个天然的缺陷，如果想要被代理的类，没有实现任何接口，那么就无法为其创建代理对象，这种方式就行不通了
<a name="rnFqr"></a>
## **4、CGLib 动态代理**
既然已经说到了 JDK 动态代理，那就不得不提 CGLib 动态代理了。使用 JDK 动态代理对被代理的类有要求，不是所有的类都能被代理，而 CGLib 动态代理则刚好解决了这个问题
```java
@Component
public class CGLibRetryProxyHandler implements MethodInterceptor {

    private Object target;

    @Override
    public Object intercept(Object o, Method method, Object[] objects, MethodProxy methodProxy) throws Throwable {
        int times = 1;
        while (times <= 5) {
            try {
                // 故意抛异常
                int i = 3 / 0;
                return method.invoke(target, objects);
            } catch (Exception e) {
                System.out.println("重试【" + times + "】次");
                try {
                    Thread.sleep(2000);
                } catch (InterruptedException ex) {
                    ex.printStackTrace();
                }
                times++;
                if (times > 5) {
                    throw new RuntimeException("不再重试！");
                }
            }
        }
        return null;
    }

    public Object getCglibProxy(Object objectTarget){
        this.target = objectTarget;
        Enhancer enhancer = new Enhancer();
        enhancer.setSuperclass(objectTarget.getClass());
        enhancer.setCallback(this);
        Object result = enhancer.create();
        return result;
    }

}
```
测试：
```java
@GetMapping("/addOrder")
public String addOrder() {
    OrderService orderServiceProxy = (OrderService) cgLibRetryProxyHandler.getCglibProxy(orderService);
    orderServiceProxy.addOrder();
    return "addOrder";
}
```
这样就很棒了，完美的解决了 JDK 动态代理带来的缺陷。优雅指数上涨了不少。<br />但这个方案仍旧存在一个问题，那就是需要对原来的逻辑进行侵入式修改，在每个被代理实例被调用的地方都需要进行调整，这样仍然会对原有代码带来较多修改
<a name="tXM5j"></a>
## **5、手动 Aop**
考虑到以后可能会有很多的方法也需要重试功能，咱们可以将重试这个共性功能通过 AOP 来实现：使用 AOP 来为目标调用设置切面，即可在目标方法调用前后添加一些重试的逻辑
```xml
<dependency>
  <groupId>org.aspectj</groupId>
  <artifactId>aspectjweaver</artifactId>
</dependency>
```
自定义注解：
```java
@Documented
@Target(ElementType.METHOD)
@Retention(RetentionPolicy.RUNTIME)
public @interface MyRetryable {

    // 最大重试次数
    int retryTimes() default 3;
    // 重试间隔
    int retryInterval() default 1;

}
@Slf4j
@Aspect
@Component
public class RetryAspect {

    @Pointcut("@annotation(com.hcr.sbes.retry.annotation.MyRetryable)")
    private void retryMethodCall(){}

    @Around("retryMethodCall()")
    public Object retry(ProceedingJoinPoint joinPoint) throws InterruptedException {
        // 获取重试次数和重试间隔
        MyRetryable retry = ((MethodSignature)joinPoint.getSignature()).getMethod().getAnnotation(MyRetryable.class);
        int maxRetryTimes = retry.retryTimes();
        int retryInterval = retry.retryInterval();

        Throwable error = new RuntimeException();
        for (int retryTimes = 1; retryTimes <= maxRetryTimes; retryTimes++){
            try {
                Object result = joinPoint.proceed();
                return result;
            } catch (Throwable throwable) {
                error = throwable;
                log.warn("调用发生异常，开始重试，retryTimes:{}", retryTimes);
            }
            Thread.sleep(retryInterval * 1000L);
        }
        throw new RuntimeException("重试次数耗尽", error);
    }

}
```
给需要重试的方法添加注解 `@MyRetryable`：
```java
@Service
public class OrderServiceImpl implements OrderService {

    @Override
    @MyRetryable(retryTimes = 5, retryInterval = 2)
    public void addOrder() {
        int i = 3 / 0;
        // addOrder
    }

}
```
这样即不用编写重复代码，实现上也比较优雅了：一个注解就实现重试。
<a name="wLxlO"></a>
## **6、spring-retry**
```xml
<dependency>
  <groupId>org.springframework.retry</groupId>
  <artifactId>spring-retry</artifactId>
</dependency>
```
开启重试功能：在启动类或者配置类上添加 `@EnableRetry` 注解<br />在需要重试的方法上添加 `@Retryable` 注解
```java
@Slf4j
@Service
public class OrderServiceImpl implements OrderService {

    @Override
    @Retryable(maxAttempts = 3, backoff = @Backoff(delay = 2000, multiplier = 2))
    public void addOrder() {
        System.out.println("重试...");
        int i = 3 / 0;
        // addOrder
    }

    @Recover
    public void recover(RuntimeException e) {
        log.error("达到最大重试次数", e);
    }

}
```
该方法调用后会进行重试，最大重试次数为 3，第一次重试间隔为 2s，之后以 2 倍大小进行递增，第二次重试间隔为 4 s，第三次为 8s<br />Spring 的重试机制还支持很多很有用的特性，由三个注解完成：

- `@Retryable`
- `@Backoff`
- `@Recover`

查看 `@Retryable` 注解源码：指定异常重试、次数
```java
public @interface Retryable {

    // 设置重试拦截器的 bean 名称
    String interceptor() default "";

    // 只对特定类型的异常进行重试。默认：所有异常
    Class<? extends Throwable>[] value() default {};

    // 包含或者排除哪些异常进行重试
    Class<? extends Throwable>[] include() default {};
    Class<? extends Throwable>[] exclude() default {};

    // l设置该重试的唯一标志，用于统计输出
    String label() default "";

    boolean stateful() default false;

    // 最大重试次数，默认为 3 次
    int maxAttempts() default 3;


    String maxAttemptsExpression() default "";

    // 设置重试补偿机制，可以设置重试间隔，并且支持设置重试延迟倍数
    Backoff backoff() default @Backoff;

    // 异常表达式，在抛出异常后执行，以判断后续是否进行重试
    String exceptionExpression() default "";

    String[] listeners() default {};
}
```
`**@Backoff**`** 注解：** 指定重试回退策略（如果因为网络波动导致调用失败，立即重试可能还是会失败，最优选择是等待一小会儿再重试。决定等待多久之后再重试的方法。通俗的说，就是每次重试是立即重试还是等待一段时间后重试）<br />`**@Recover**`** 注解：** 进行善后工作：当重试达到指定次数之后，会调用指定的方法来进行日志记录等操作
<a name="ah2YG"></a>
### 注意：

- `@Recover` 注解标记的方法必须和被 `@Retryable` 标记的方法在同一个类中
- 重试方法抛出的异常类型需要与 `recover()` 方法参数类型保持一致
- `recover()` 方法返回值需要与重试方法返回值保证一致
- `recover()` 方法中不能再抛出 Exception，否则会报无法识别该异常的错误

这里还需要再提醒的一点是，由于 Spring Retry 用到了 Aspect 增强，所以就会有使用 Aspect 不可避免的坑——方法内部调用，如果被 `@Retryable`注解的方法的调用方和被调用方处于同一个类中，那么重试将会失效<br />通过以上几个简单的配置，可以看到 Spring Retry 重试机制考虑的比较完善，比自己写AOP实现要强大很多
<a name="nNnXe"></a>
### 弊端：
但也还是存在一定的不足，Spring的重试机制只支持对 异常 进行捕获，而无法对返回值进行校验
```java
@Retryable
public String hello() {
    long current = count.incrementAndGet();
    System.out.println("第" + current +"次被调用");
    if (current % 3 != 0) {
        log.warn("调用失败");
        return "error";
    }
    return "success";
}
```
因此就算在方法上添加 `@Retryable`，也无法实现失败重试<br />除了使用注解外，Spring Retry 也支持直接在调用时使用代码进行重试：
```java
@Test
public void normalSpringRetry() {
    // 表示哪些异常需要重试,key表示异常的字节码,value为true表示需要重试
    Map<Class<? extends Throwable>, Boolean> exceptionMap = new HashMap<>();
    exceptionMap.put(HelloRetryException.class, true);

    // 构建重试模板实例
    RetryTemplate retryTemplate = new RetryTemplate();

    // 设置重试回退操作策略，主要设置重试间隔时间
    FixedBackOffPolicy backOffPolicy = new FixedBackOffPolicy();
    long fixedPeriodTime = 1000L;
    backOffPolicy.setBackOffPeriod(fixedPeriodTime);

    // 设置重试策略，主要设置重试次数
    int maxRetryTimes = 3;
    SimpleRetryPolicy retryPolicy = new SimpleRetryPolicy(maxRetryTimes, exceptionMap);

    retryTemplate.setRetryPolicy(retryPolicy);
    retryTemplate.setBackOffPolicy(backOffPolicy);

    Boolean execute = retryTemplate.execute(
        //RetryCallback
        retryContext -> {
            String hello = helloService.hello();
            log.info("调用的结果:{}", hello);
            return true;
        },
        // RecoverCallBack
        retryContext -> {
            //RecoveryCallback
            log.info("已达到最大重试次数");
            return false;
        }
    );
}
```
此时唯一的好处是可以设置多种重试策略：

- `NeverRetryPolicy`：只允许调用`RetryCallback`一次，不允许重试
- `AlwaysRetryPolicy`：允许无限重试，直到成功，此方式逻辑不当会导致死循环
- `SimpleRetryPolicy`：固定次数重试策略，默认重试最大次数为3次，`RetryTemplate`默认使用的策略
- `TimeoutRetryPolicy`：超时时间重试策略，默认超时时间为1秒，在指定的超时时间内允许重试
- `ExceptionClassifierRetryPolicy`：设置不同异常的重试策略，类似组合重试策略，区别在于这里只区分不同异常的重试
- `CircuitBreakerRetryPolicy`：有熔断功能的重试策略，需设置3个参数openTimeout、resetTimeout和delegate
- `CompositeRetryPolicy`：组合重试策略，有两种组合方式，乐观组合重试策略是指只要有一个策略允许即可以重试，悲观组合重试策略是指只要有一个策略不允许即可以重试，但不管哪种组合方式，组合中的每一个策略都会执行
<a name="u0nri"></a>
## **7、guava-retry**
和 Spring Retry 相比，Guava Retry 具有更强的灵活性，并且能够根据 返回值 来判断是否需要重试
```xml
<dependency>
    <groupId>com.github.rholder</groupId>
    <artifactId>guava-retrying</artifactId>
    <version>2.0.0</version>
</dependency>
```
```java
@Override
public String guavaRetry(Integer num) {
    Retryer<String> retryer = RetryerBuilder.<String>newBuilder()
            //无论出现什么异常，都进行重试
            .retryIfException()
            //返回结果为 error时，进行重试
            .retryIfResult(result -> Objects.equals(result, "error"))
            //重试等待策略：等待 2s 后再进行重试
            .withWaitStrategy(WaitStrategies.fixedWait(2, TimeUnit.SECONDS))
            //重试停止策略：重试达到 3 次
            .withStopStrategy(StopStrategies.stopAfterAttempt(3))
            .withRetryListener(new RetryListener() {
                @Override
                public <V> void onRetry(Attempt<V> attempt) {
                    System.out.println("RetryListener: 第" + attempt.getAttemptNumber() + "次调用");
                }
            })
            .build();
    try {
        retryer.call(() -> testGuavaRetry(num));
    } catch (Exception e) {
        e.printStackTrace();
    }
    return "test";
}
```
先创建一个Retryer实例，然后使用这个实例对需要重试的方法进行调用，可以通过很多方法来设置重试机制：

- `retryIfException()`：对所有异常进行重试
- `retryIfRuntimeException()`：设置对指定异常进行重试
- `retryIfExceptionOfType()`：对所有 RuntimeException 进行重试
- `retryIfResult()`：对不符合预期的返回结果进行重试

还有五个以 withXxx 开头的方法，用来对重试策略/等待策略/阻塞策略/单次任务执行时间限制/自定义监听器进行设置，以实现更加强大的异常处理：

- `withRetryListener()`：设置重试监听器，用来执行额外的处理工作
- `withWaitStrategy()`：重试等待策略
- `withStopStrategy()`：停止重试策略
- `withAttemptTimeLimiter`：设置任务单次执行的时间限制，如果超时则抛出异常
- `withBlockStrategy()`：设置任务阻塞策略，即可以设置当前重试完成，下次重试开始前的这段时间做什么事情
<a name="EDm2V"></a>
## **总结**
从手动重试，到使用 Spring AOP 自己动手实现，再到站在巨人肩上使用特别优秀的开源实现 Spring Retry 和 Google guava-retrying，经过对各种重试实现方式的介绍，可以看到以上几种方式基本上已经满足大部分场景的需要：

- 如果是基于 Spring 的项目，使用 Spring Retry 的注解方式已经可以解决大部分问题
- 如果项目没有使用 Spring 相关框架，则适合使用 Google guava-retrying：自成体系，使用起来更加灵活强大
