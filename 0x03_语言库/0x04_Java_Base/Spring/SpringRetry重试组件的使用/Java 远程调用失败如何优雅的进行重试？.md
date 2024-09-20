Java<br />在日常开发的过程中经常会需要调用第三方组件或者数据库，有的时候可能会因为网络抖动或者下游服务抖动，导致某次查询失败。<br />这种时候往往就会进行重试，当重试几次后依旧还是失败的话才会向上抛出异常进行失败。接下来演示一下通常是如何做的，以及如何更优雅的进行重试。
<a name="mTcra"></a>
## 常规做法
先来看一下常规做法，常规做法首先会设置一个重试次数，然后通过 while 循环的方式进行遍历，当循环次数没有达到重试次数的时候，直到有正确结果后就返回，如果重试依旧失败则会进行睡眠一段时间，再次重试，直到正常返回或者达到重试次数返回。
```java
package com.example.demo.service;

import org.springframework.retry.annotation.Backoff;
import org.springframework.retry.annotation.Retryable;
import org.springframework.stereotype.Service;

import java.util.Random;
import java.util.concurrent.TimeUnit;

@Service
public class HelloService {
    public String sayHello(String name) {
        String result = "";
        int retryTime = 3;
        while (retryTime > 0) {
            try {
                //
                result = name + doSomething();
                return result;
            } catch (Exception e) {
                System.out.println("send message failed. try again in 1's");
                retryTime--;
                try {
                    TimeUnit.SECONDS.sleep(1);
                } catch (InterruptedException ex) {
                    throw new RuntimeException(ex);
                }
            }
        }
        return result;
    }

    private int doSomething() {
        Random random = new Random();
        int i = random.nextInt(3);
        System.out.println("i is " + i);
        return 10 / i;
    }
}
```
这里为了模拟异常的情况，在 `doSomething` 函数里面进行了随机数的生成和使用，当随机出来的值为 0 的时候，则会触发 java.lang.ArithmeticException 异常，因为 0 不能作除数。<br />接下来再对外提供一个接口用于访问，代码如下
```java
package com.example.demo.controller;

import com.example.demo.service.HelloService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class HelloController {

    @Autowired
    private HelloService helloService;

    @GetMapping(value = "/hello")
    public String hello(@RequestParam("name") String name) {
        return helloService.sayHello(name);
    }
}
```
正常启动过后，通过浏览器进行访问<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1663205576551-a0d3312d-acbc-4481-883b-98e437051cbf.jpeg#clientId=ucbdf13a3-34e4-4&from=paste&id=u090fe333&originHeight=341&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub7cc5520-f9ee-44ae-8a7b-35dc519b593&title=)<br />可以看到，第一次方法的时候就成功的达到了想要的效果，随机数就是 0 ，在 1 秒后重试后结果正常。在多试了几次过后，会遇到三次都是 0 的情况，这个时候才会抛出异常，说明服务是真的有问题了。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1663205576492-dbac3fb7-6d9b-4439-8d01-670eeb9e660f.jpeg#clientId=ucbdf13a3-34e4-4&from=paste&id=ua3d7e1f1&originHeight=575&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ueee908e3-50f9-4658-8c96-764c7a18dff&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1663205576453-58ae12ae-d2c2-4caf-a40a-496e78180738.jpeg#clientId=ucbdf13a3-34e4-4&from=paste&id=u6bf10b4f&originHeight=605&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc3557ba0-a7c9-4cb6-a8a6-21d99fa5112&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1663205576560-323edb4b-b888-4054-ae33-36850d6a33b4.jpeg#clientId=ucbdf13a3-34e4-4&from=paste&id=uc5e7ddb7&originHeight=348&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1edda38e-605b-4725-aa1c-68326564253&title=)<br />上面的代码可以看到是有效果了，虽然不是很好看，特别是在还有一些其他逻辑的情况，看上去会很臃肿，但是确实是可以正常使用的，那么有的小伙伴就要问了，有没有一种优雅的方式呢？总不能在很多地方都重复的这样写重试的代码吧。
<a name="gf0W8"></a>
## 注解重试
要知道普通人在日常开发的时候，如果遇到一个问题肯定是别人都遇到过的，什么时候当遇到的问题，没有人遇到过的时候，那说明是很前卫的。<br />因此小伙伴能想到的是不是有简单的方式来进行重试，有的人已经帮我们想好了，可以通过 `@Retryable` 注解来实现一样的效果，接下来演示一下如何使用这个注解。<br />首先需要在启动类上面加入 `@EnableRetry` 注解，表示要开启重试的功能，这个很好理解，就像要开启定时功能需要添加 `@EnableScheduling` 注解一样，Spring 的 `@Enablexxx` 注解也是很有意思的，后面再聊。<br />添加完注解以后，需要加入切面的依赖，如下
```xml
<dependency>
  <groupId>org.aspectj</groupId>
  <artifactId>aspectjweaver</artifactId>
  <version>1.9.2</version>
</dependency>
```
如下不加入这个切面依赖，启动的时候会有如下异常<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1663205576549-7897d0d4-bed3-4b44-96cd-d6d2438ae9b2.jpeg#clientId=ucbdf13a3-34e4-4&from=paste&id=uff46062c&originHeight=525&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ufe239bad-9273-4051-9488-3d1501cc11d&title=)<br />添加的注解和依赖过后，需要改造 HelloService 里面的 `sayHello()` 方法，简化成如下，增加  `@Retryable` 注解，以及设置相应的参数值。
```java
@Retryable(value = Exception.class, maxAttempts = 3, backoff = @Backoff(delay = 1000, multiplier = 2))
public String sayHello(String name){
	return name + doSomething();
}
```
再次通过浏览器访问 http://127.0.0.1:8080/hello?name=ziyou 看到效果如下，跟自己写的重试一样。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1663205576878-e711f28c-d7fb-4f8b-8428-1da110d79ad8.jpeg#clientId=ucbdf13a3-34e4-4&from=paste&id=u286b57e6&originHeight=437&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2adf7a03-1923-4ee4-aca4-84c948910e4&title=)
<a name="qOIly"></a>
## `@Retryable` 详解
```java
//
// Source code recreated from a .class file by IntelliJ IDEA
// (powered by FernFlower decompiler)
//

package org.springframework.retry.annotation;

import java.lang.annotation.Documented;
import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

@Target({ElementType.METHOD, ElementType.TYPE})
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface Retryable {
    String recover() default "";

    String interceptor() default "";

    Class<? extends Throwable>[] value() default {};

    Class<? extends Throwable>[] include() default {};

    Class<? extends Throwable>[] exclude() default {};

    String label() default "";

    boolean stateful() default false;

    int maxAttempts() default 3;

    String maxAttemptsExpression() default "";

    Backoff backoff() default @Backoff;

    String exceptionExpression() default "";

    String[] listeners() default {};
}
```
点到这个注解里面，可以看到这个注解的代码如下，其中有几个参数来解释一下

- `recover`:  当前类中的回滚方法名称；
- `interceptor`: 重试的拦截器名称，重试的时候可以配置一个拦截器；
- `value`：需要重试的异常类型，跟下面的 include 一致；
- `include`：包含的重试的异常类型；
- `exclude`：不包含的重试异常类型；
- `label`：用于统计的唯一标识；
- `stateful`：标志表示重试是有状态的，也就是说，异常被重新抛出，重试策略是否会以相同的策略应用于具有相同参数的后续调用。如果是 false，那么可重试的异常就不会被重新抛出。
- `maxAttempts`：重试次数；
- `backoff`：指定用于重试此操作的属性；
- `listeners`：重试监听器 bean 名称；

配合上面的一些属性的使用，就可以达到通过注解简单来实现方法调用异常后的自动重试，非常好用。可以在执行重试方法的时候设置自定义的重试拦截器，如下所示，自定义重试拦截器需要实现 `MethodInterceptor` 接口并实现 `invoke` 方法，不过要注意，如果使用了拦截器的话，那么方法上的参数就会被覆盖。
```java
package com.example.demo.pid;

import org.aopalliance.intercept.MethodInterceptor;
import org.aopalliance.intercept.MethodInvocation;
import org.springframework.retry.interceptor.RetryInterceptorBuilder;
import org.springframework.retry.interceptor.RetryOperationsInterceptor;
import org.springframework.retry.policy.SimpleRetryPolicy;
import org.springframework.stereotype.Component;

@Component
public class CustomRetryInterceptor implements MethodInterceptor {

  @Override
  public Object invoke(MethodInvocation invocation) throws Throwable {
    RetryOperationsInterceptor build = RetryInterceptorBuilder.stateless()
      .maxAttempts(2).backOffOptions(3000, 2, 1000).build();
    return build.invoke(invocation);
  }
}
```
自定义回滚方法，还可以在重试几次依旧错误的情况，编写自定义的回滚方法。
```java
@Retryable(value = Exception.class,
    recover = "recover", maxAttempts = 2,
    backoff = @Backoff(delay = 1000, multiplier = 2))
public String sayHello(String name){
    return name + doSomething();
}

@Recover
public String recover(Exception e, String name) {
	System.out.println("recover");
	return "recover";
}
```
![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1663205576970-6a75a9ef-1c32-4336-9973-5b71cd553782.jpeg#clientId=ucbdf13a3-34e4-4&from=paste&id=u890370fe&originHeight=510&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uab3bffae-a39b-4b10-bddc-35a28edf067&title=)<br />要注意：

- 重试方法必须要使用  `@Recover` 注解；
- 返回值必须和被重试的函数返回值一致；
- 参数中除了第一个是触发的异常外，后面的参数需要和被重试函数的参数列表一致；

上面代码中的 `@Backoff(delay = 1000, multiplier = 2)` 表示第一次延迟 1000ms 重试，后面每次重试的延迟时间都翻倍。
<a name="mzDqW"></a>
## 总结
介绍了一下 Spring 的 `@Retryable` 注解使用，并通过几个 demo 来编写了自己重试拦截器以及回滚方法的时候，其中还有很多细节，只有自己真正的使用过才能体会到。
