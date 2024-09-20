JavaSpringCloudFeign<br />在不同的服务之间通过Feign进行远程调用，但是，在尝试使调用可重试时遇到了一个小问题，Feign框架本身可以配置的自己的重试机制，但是它是一刀切的方式，所有的调用都是同样的机制，没有办法像我们希望的那样在每个方法的基础上配置。不过在项目中探索除了一种新的写法，通过spring-retry框架集合Feign去实现重试机制，可以为每个调用实现不同的重试机制，那究竟是如何做到的呢，继续往下看。
<a name="XUBjN"></a>
## 自定义注解`@FeignRetry`
为了解决上面提到的问题，让Feign调用的每个接口单独配置不同的重试机制。使用了面向切面编程并编写了一个自定义注解：`@FeignRetry`。此注释的工作方式类似于`@Retryable`的包装器，并与其共享相同的规范以避免混淆。
```java
@Target({ElementType.METHOD, ElementType.TYPE})
@Retention(RetentionPolicy.RUNTIME)
public @interface FeignRetry {

    Backoff backoff() default @Backoff();
    int maxAttempt() default 3;
    Class<? extends Throwable>[] include() default {};
}

@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
public @interface Backoff {
    long delay() default 1000L;;
    long maxDelay() default 0L;
    double multiplier() default 0.0D;;
}
```
`FeignRetryAspect`切面处理`@FeignRetry`注解。
```java
Slf4j
@Aspect
@Component
public class FeignRetryAspect {

    @Around("@annotation(FeignRetry)")
    public Object retry(ProceedingJoinPoint joinPoint) throws Throwable {
        Method method = getCurrentMethod(joinPoint);
        FeignRetry feignRetry = method.getAnnotation(FeignRetry.class);

        RetryTemplate retryTemplate = new RetryTemplate();
        retryTemplate.setBackOffPolicy(prepareBackOffPolicy(feignRetry));
        retryTemplate.setRetryPolicy(prepareSimpleRetryPolicy(feignRetry));

        // 重试
        return retryTemplate.execute(arg0 -> {
            int retryCount = arg0.getRetryCount();
            log.info("Sending request method: {}, max attempt: {}, delay: {}, retryCount: {}",
                    method.getName(),
                    feignRetry.maxAttempt(),
                    feignRetry.backoff().delay(),
                    retryCount
            );
            return joinPoint.proceed(joinPoint.getArgs());
        });
    }

    private BackOffPolicy prepareBackOffPolicy(FeignRetry feignRetry) {
        if (feignRetry.backoff().multiplier() != 0) {
            ExponentialBackOffPolicy backOffPolicy = new ExponentialBackOffPolicy();
            backOffPolicy.setInitialInterval(feignRetry.backoff().delay());
            backOffPolicy.setMaxInterval(feignRetry.backoff().maxDelay());
            backOffPolicy.setMultiplier(feignRetry.backoff().multiplier());
            return backOffPolicy;
        } else {
            FixedBackOffPolicy fixedBackOffPolicy = new FixedBackOffPolicy();
            fixedBackOffPolicy.setBackOffPeriod(feignRetry.backoff().delay());
            return fixedBackOffPolicy;
        }
    }


    private SimpleRetryPolicy prepareSimpleRetryPolicy(FeignRetry feignRetry) {
        Map<Class<? extends Throwable>, Boolean> policyMap = new HashMap<>();
        policyMap.put(RetryableException.class, true);  // Connection refused or time out
        policyMap.put(ClientException.class, true);     // Load balance does not available (cause of RunTimeException)
        if (feignRetry.include().length != 0) {
            for (Class<? extends Throwable> t : feignRetry.include()) {
                policyMap.put(t, true);
            }
        }
        return new SimpleRetryPolicy(feignRetry.maxAttempt(), policyMap, true);
    }

    private Method getCurrentMethod(JoinPoint joinPoint) {
        MethodSignature signature = (MethodSignature) joinPoint.getSignature();
        return signature.getMethod();
    }
}
```
捕获`FeignRetry`注解的方法，将配置传递给Spring RetryTemplate，根据配置调用服务。
<a name="rNXvt"></a>
## `@FeignRetry` 的使用
用法很简单，只需将注解放在希望重试机制处于活动状态的 Feign Client方法上即可。自定义切面的用法类似于Spring自带的`@Retryable`注解。
```java
@GetMapping
@FeignRetry(maxAttempt = 3, backoff = @Backoff(delay = 500L))
ResponseEntity<String> retrieve1();

@GetMapping
@FeignRetry(maxAttempt = 6, backoff = @Backoff(delay = 500L, maxDelay = 20000L, multiplier = 4))
ResponseEntity<String> retrieve2();
```
另外还需要在应用程序类中使用 `@EnableRetry` 注释来启动重试，比如可以加载SpringBoot的启动类中。
<a name="RqWW8"></a>
## 总结
Feign重试其实是一个很常见的场景，本文通过了自定义了一个`@FeignRetry`注解来实现可重试的机制，针对不同的Feign接口还可以使用不同的重试策略，是不是很方便，
