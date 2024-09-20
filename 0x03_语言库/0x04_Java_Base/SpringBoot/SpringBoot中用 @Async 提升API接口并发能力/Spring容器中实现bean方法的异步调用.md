JavaSpring
<a name="jA6Y3"></a>
## 1、本文内容
详解 `@EnableAsync` & `@Async`，主要分下面几个点进行介绍。

1. **作用**
2. **用法**
3. **获取异步执行结果**
4. **自定义异步执行的线程池**
5. **自定义异常处理**
6. **线程隔离**
7. **源码 & 原理**
<a name="bk3gz"></a>
## 2、作用
**Spring容器中实现bean方法的异步调用。**<br />比如有个logService的bean，logservice中有个log方法用来记录日志，当调用`logService.log(msg)`的时候，希望异步执行，那么可以通过`@EnableAsync` & `@Async`来实现。
<a name="uIN6W"></a>
## 3、用法
<a name="MoWcs"></a>
### 2步

1. 需要异步执行的方法上面使用`@Async`注解标注，若bean中所有的方法都需要异步执行，可以直接将`@Async`加载类上。
2. 将`@EnableAsync`添加在Spring配置类上，此时`@Async`注解才会起效。
<a name="fJctS"></a>
### 常见2种用法

1. 无返回值的
2. 可以获取返回值的
<a name="pYIvr"></a>
## 4、无返回值的
<a name="Ucg2v"></a>
### 用法
方法返回值不是`Future`类型的，被执行时，会立即返回，并且无法获取方法返回值，如：
```java
@Async
public void log(String msg) throws InterruptedException {
    System.out.println("开始记录日志," + System.currentTimeMillis());
    //模拟耗时2秒
    TimeUnit.SECONDS.sleep(2);
    System.out.println("日志记录完毕," + System.currentTimeMillis());
}
```
<a name="PHNmr"></a>
### 案例
实现日志异步记录的功能。<br />`LogService.log`方法用来异步记录日志，需要使用`@Async`标注
```java
import org.springframework.scheduling.annotation.Async;
import org.springframework.stereotype.Component;

import java.util.concurrent.TimeUnit;

@Component
public class LogService {
    @Async
    public void log(String msg) throws InterruptedException {
        System.out.println(Thread.currentThread() + "开始记录日志," + System.currentTimeMillis());
        //模拟耗时2秒
        TimeUnit.SECONDS.sleep(2);
        System.out.println(Thread.currentThread() + "日志记录完毕," + System.currentTimeMillis());
    }
}
```
来个Spring配置类，需要加上`@EnableAsync`开启bean方法的异步调用。
```java
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.EnableAspectJAutoProxy;
import org.springframework.scheduling.annotation.EnableAsync;

@ComponentScan
@EnableAsync
public class MainConfig1 {
}
```
测试代码
```java
import com.javacode2018.async.demo1.LogService;
import com.javacode2018.async.demo1.MainConfig1;
import org.junit.Test;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

import java.util.concurrent.TimeUnit;

public class AsyncTest {

    @Test
    public void test1() throws InterruptedException {
        AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext();
        context.register(MainConfig1.class);
        context.refresh();
        LogService logService = context.getBean(LogService.class);
        System.out.println(Thread.currentThread() + " logService.log start," + System.currentTimeMillis());
        logService.log("异步执行方法!");
        System.out.println(Thread.currentThread() + " logService.log end," + System.currentTimeMillis());

        //休眠一下，防止@Test退出
        TimeUnit.SECONDS.sleep(3);
    }

}
```
运行输出
```
Thread[main,5,main] logService.log start,1595223990417
Thread[main,5,main] logService.log end,1595223990432
Thread[SimpleAsyncTaskExecutor-1,5,main]开始记录日志,1595223990443
Thread[SimpleAsyncTaskExecutor-1,5,main]日志记录完毕,1595223992443
```
前2行输出，可以看出`logService.log`立即就返回了，后面2行来自于log方法，相差2秒左右。<br />前面2行在主线程中执行，后面2行在异步线程中执行。
<a name="nUUYr"></a>
## 5、获取异步返回值
<a name="OwNQ3"></a>
### 用法
若需取异步执行结果，方法返回值必须为`Future`类型，使用Spring提供的静态方法`org.springframework.scheduling.annotation.AsyncResult#forValue`创建返回值，如：
```java
public Future<String> getGoodsInfo(long goodsId) throws InterruptedException {
    return AsyncResult.forValue(String.format("商品%s基本信息!", goodsId));
}
```
<a name="Q4zKJ"></a>
### 案例
场景：电商中商品详情页通常会有很多信息：商品基本信息、商品描述信息、商品评论信息，通过3个方法来或者这几个信息。<br />这3个方法之间无关联，所以可以采用异步的方式并行获取，提升效率。<br />下面是商品服务，内部3个方法都需要异步，所以直接在类上使用`@Async`标注了，每个方法内部休眠500毫秒，模拟一下耗时操作。
```java
import org.springframework.scheduling.annotation.Async;
import org.springframework.scheduling.annotation.AsyncResult;
import org.springframework.stereotype.Component;

import java.util.Arrays;
import java.util.List;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;

@Async
@Component
public class GoodsService {
    //模拟获取商品基本信息，内部耗时500毫秒
    public Future<String> getGoodsInfo(long goodsId) throws InterruptedException {
        TimeUnit.MILLISECONDS.sleep(500);
        return AsyncResult.forValue(String.format("商品%s基本信息!", goodsId));
    }

    //模拟获取商品描述信息，内部耗时500毫秒
    public Future<String> getGoodsDesc(long goodsId) throws InterruptedException {
        TimeUnit.MILLISECONDS.sleep(500);
        return AsyncResult.forValue(String.format("商品%s描述信息!", goodsId));
    }

    //模拟获取商品评论信息列表，内部耗时500毫秒
    public Future<List<String>> getGoodsComments(long goodsId) throws InterruptedException {
        TimeUnit.MILLISECONDS.sleep(500);
        List<String> comments = Arrays.asList("评论1", "评论2");
        return AsyncResult.forValue(comments);
    }
}
```
来个Spring配置类，需要加上`@EnableAsync`开启bean方法的异步调用。
```java
import org.springframework.context.annotation.ComponentScan;
import org.springframework.scheduling.annotation.EnableAsync;

@ComponentScan
@EnableAsync
public class MainConfig2 {
}
```
测试代码
```java
@Test
public void test2() throws InterruptedException, ExecutionException {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext();
    context.register(MainConfig2.class);
    context.refresh();
    GoodsService goodsService = context.getBean(GoodsService.class);

    long starTime = System.currentTimeMillis();
    System.out.println("开始获取商品的各种信息");

    long goodsId = 1L;
    Future<String> goodsInfoFuture = goodsService.getGoodsInfo(goodsId);
    Future<String> goodsDescFuture = goodsService.getGoodsDesc(goodsId);
    Future<List<String>> goodsCommentsFuture = goodsService.getGoodsComments(goodsId);

    System.out.println(goodsInfoFuture.get());
    System.out.println(goodsDescFuture.get());
    System.out.println(goodsCommentsFuture.get());

    System.out.println("商品信息获取完毕,总耗时(ms)：" + (System.currentTimeMillis() - starTime));

    //休眠一下，防止@Test退出
    TimeUnit.SECONDS.sleep(3);
}
```
运行输出
```
开始获取商品的各种信息
商品1基本信息!
商品1描述信息!
[评论1, 评论2]
商品信息获取完毕,总耗时(ms)：525
```
3个方法总计耗时500毫秒左右。<br />如果不采用异步的方式，3个方法会同步执行，耗时差不多1.5秒，来试试，将`GoodsService`上的`@Async`去掉，然后再次执行测试案例，输出
```
开始获取商品的各种信息
商品1基本信息!
商品1描述信息!
[评论1, 评论2]
商品信息获取完毕,总耗时(ms)：1503
```
这个案例大家可以借鉴一下，**按照这个思路可以去优化一下代码，方法之间无关联的可以采用异步的方式，并行去获取，最终耗时为最长的那个方法，整体相对于同步的方式性能提升不少。**
<a name="RaHDv"></a>
## 6、自定义异步执行的线程池
默认情况下，`@EnableAsync`使用内置的线程池来异步调用方法，不过也可以自定义异步执行任务的线程池。
<a name="kpFGm"></a>
### 有2种方式来自定义异步处理的线程池
<a name="oDpoz"></a>
#### 方式1
**在Spring容器中定义一个线程池类型的bean，bean名称必须是**`**taskExecutor**`
```java
@Bean
public Executor taskExecutor() {
    ThreadPoolTaskExecutor executor = new ThreadPoolTaskExecutor();
    executor.setCorePoolSize(10);
    executor.setMaxPoolSize(100);
    executor.setThreadNamePrefix("my-thread-");
    return executor;
}
```
<a name="r9HCU"></a>
#### 方式2
定义一个bean，实现`AsyncConfigurer`接口中的`getAsyncExecutor`方法，这个方法需要返回自定义的线程池，案例代码：
```java
import com.javacode2018.async.demo1.LogService;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.context.annotation.Bean;
import org.springframework.lang.Nullable;
import org.springframework.scheduling.annotation.AsyncConfigurer;
import org.springframework.scheduling.annotation.EnableAsync;
import org.springframework.scheduling.concurrent.ThreadPoolTaskExecutor;

import java.util.concurrent.Executor;

@EnableAsync
public class MainConfig3 {

    @Bean
    public LogService logService() {
        return new LogService();
    }

    /**
     * 定义一个AsyncConfigurer类型的bean，实现getAsyncExecutor方法，返回自定义的线程池
     *
     * @param executor
     * @return
     */
    @Bean
    public AsyncConfigurer asyncConfigurer(@Qualifier("logExecutors") Executor executor) {
        return new AsyncConfigurer() {
            @Nullable
            @Override
            public Executor getAsyncExecutor() {
                return executor;
            }
        };
    }

    /**
     * 定义一个线程池，用来异步处理日志方法调用
     *
     * @return
     */
    @Bean
    public Executor logExecutors() {
        ThreadPoolTaskExecutor executor = new ThreadPoolTaskExecutor();
        executor.setCorePoolSize(10);
        executor.setMaxPoolSize(100);
        //线程名称前缀
        executor.setThreadNamePrefix("log-thread-"); //@1
        return executor;
    }

}
```
@1自定义的线程池中线程名称前缀为log-thread-，运行下面测试代码
```java
@Test
public void test3() throws InterruptedException {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext();
    context.register(MainConfig3.class);
    context.refresh();
    LogService logService = context.getBean(LogService.class);
    System.out.println(Thread.currentThread() + " logService.log start," + System.currentTimeMillis());
    logService.log("异步执行方法!");
    System.out.println(Thread.currentThread() + " logService.log end," + System.currentTimeMillis());

    //休眠一下，防止@Test退出
    TimeUnit.SECONDS.sleep(3);
}
```
输出
```
Thread[main,5,main] logService.log start,1595228732914
Thread[main,5,main] logService.log end,1595228732921
Thread[log-thread-1,5,main]开始记录日志,1595228732930
Thread[log-thread-1,5,main]日志记录完毕,1595228734931
```
最后2行日志中线程名称是log-thread-，正是自定义线程池中的线程。
<a name="QvUge"></a>
## 7、自定义异常处理
异步方法若发生了异常，如何获取异常信息呢？此时可以通过自定义异常处理来解决。
<a name="E5ShI"></a>
### 异常处理分2种情况

1. 当返回值是`Future`的时候，方法内部有异常的时候，异常会向外抛出，可以对`Future.get`采用try..catch来捕获异常
2. 当返回值不是`Future`的时候，可以自定义一个bean，实现`AsyncConfigurer`接口中的`getAsyncUncaughtExceptionHandler`方法，返回自定义的异常处理器
<a name="POX5J"></a>
### 情况1：返回值为`Future`类型
<a name="dtmXB"></a>
#### 用法
通过try..catch来捕获异常，如下
```java
try {
    Future<String> future = logService.mockException();
    System.out.println(future.get());
} catch (ExecutionException e) {
    System.out.println("捕获 ExecutionException 异常");
    //通过e.getCause获取实际的异常信息
    e.getCause().printStackTrace();
} catch (InterruptedException e) {
    e.printStackTrace();
}
```
<a name="sFsR1"></a>
#### 案例
`LogService`中添加一个方法，返回值为`Future`，内部抛出一个异常，如下：
```java
@Async
public Future<String> mockException() {
    //模拟抛出一个异常
    throw new IllegalArgumentException("参数有误!");
}
```
测试代码如下
```java
@Test
public void test5() throws InterruptedException {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext();
    context.register(MainConfig1.class);
    context.refresh();
    LogService logService = context.getBean(LogService.class);
    try {
        Future<String> future = logService.mockException();
        System.out.println(future.get());
    } catch (ExecutionException e) {
        System.out.println("捕获 ExecutionException 异常");
        //通过e.getCause获取实际的异常信息
        e.getCause().printStackTrace();
    } catch (InterruptedException e) {
        e.printStackTrace();
    }
    //休眠一下，防止@Test退出
    TimeUnit.SECONDS.sleep(3);
}
```
运行输出
```
java.lang.IllegalArgumentException: 参数有误!
捕获 ExecutionException 异常
 at com.javacode2018.async.demo1.LogService.mockException(LogService.java:23)
 at com.javacode2018.async.demo1.LogService$$FastClassBySpringCGLIB$$32a28430.invoke(<generated>)
 at org.springframework.cglib.proxy.MethodProxy.invoke(MethodProxy.java:218)
```
<a name="us1yp"></a>
### 情况2：无返回值异常处理
<a name="u6uTE"></a>
#### 用法
当返回值不是Future的时候，可以自定义一个bean，实现`AsyncConfigurer`接口中的`getAsyncUncaughtExceptionHandler`方法，返回自定义的异常处理器，当目标方法执行过程中抛出异常的时候，此时会自动回调`AsyncUncaughtExceptionHandler#handleUncaughtException`这个方法，可以在这个方法中处理异常，如下：
```java
@Bean
public AsyncConfigurer asyncConfigurer() {
    return new AsyncConfigurer() {
        @Nullable
        @Override
        public AsyncUncaughtExceptionHandler getAsyncUncaughtExceptionHandler() {
            return new AsyncUncaughtExceptionHandler() {
                @Override
                public void handleUncaughtException(Throwable ex, Method method, Object... params) {
                    //当目标方法执行过程中抛出异常的时候，此时会自动回调这个方法，可以在这个方法中处理异常
                }
            };
        }
    };
}
```
<a name="xGGZp"></a>
#### 案例
`LogService`中添加一个方法，内部抛出一个异常，如下：
```java
@Async
public void mockNoReturnException() {
    //模拟抛出一个异常
    throw new IllegalArgumentException("无返回值的异常!");
}
```
来个Spring配置类，通过`AsyncConfigurer`来自定义异常处理器`AsyncUncaughtExceptionHandler`
```java
import com.javacode2018.async.demo1.LogService;
import org.springframework.aop.interceptor.AsyncUncaughtExceptionHandler;
import org.springframework.context.annotation.Bean;
import org.springframework.lang.Nullable;
import org.springframework.scheduling.annotation.AsyncConfigurer;
import org.springframework.scheduling.annotation.EnableAsync;

import java.lang.reflect.Method;
import java.util.Arrays;

@EnableAsync
public class MainConfig4 {

    @Bean
    public LogService logService() {
        return new LogService();
    }

    @Bean
    public AsyncConfigurer asyncConfigurer() {
        return new AsyncConfigurer() {
            @Nullable
            @Override
            public AsyncUncaughtExceptionHandler getAsyncUncaughtExceptionHandler() {
                return new AsyncUncaughtExceptionHandler() {
                    @Override
                    public void handleUncaughtException(Throwable ex, Method method, Object... params) {
                        String msg = String.format("方法[%s],参数[%s],发送异常了，异常详细信息:", method, Arrays.asList(params));
                        System.out.println(msg);
                        ex.printStackTrace();
                    }
                };
            }
        };
    }

}
```
运行输出
```
方法[public void com.javacode2018.async.demo1.LogService.mockNoReturnException()],参数[[]],发送异常了，异常详细信息:
java.lang.IllegalArgumentException: 无返回值的异常!
 at com.javacode2018.async.demo1.LogService.mockNoReturnException(LogService.java:29)
 at com.javacode2018.async.demo1.LogService$$FastClassBySpringCGLIB$$32a28430.invoke(<generated>)
 at org.springframework.cglib.proxy.MethodProxy.invoke(MethodProxy.java:218)
```
<a name="NpDkP"></a>
## 8、线程池隔离
<a name="VCFTQ"></a>
### 什么是线程池隔离？
一个系统中可能有很多业务，比如充值服务、提现服务或者其他服务，这些服务中都有一些方法需要异步执行，默认情况下他们会使用同一个线程池去执行，如果有一个业务量比较大，占用了线程池中的大量线程，此时会导致其他业务的方法无法执行，那么可以采用线程隔离的方式，对不同的业务使用不同的线程池，相互隔离，互不影响。<br />`@Async`注解有个value参数，用来指定线程池的bean名称，方法运行的时候，就会采用指定的线程池来执行目标方法。
<a name="PgCvW"></a>
### 使用步骤

1. 在Spring容器中，自定义线程池相关的bean
2. `@Async("线程池bean名称")`
<a name="xt2Ol"></a>
### 案例
模拟2个业务：异步充值、异步提现；2个业务都采用独立的线程池来异步执行，互不影响。
<a name="ZjP6x"></a>
#### 异步充值服务
```java
package com.javacode2018.async.demo5;

import org.springframework.scheduling.annotation.Async;
import org.springframework.stereotype.Component;

@Component
public class RechargeService {
    //模拟异步充值
    @Async(MainConfig5.RECHARGE_EXECUTORS_BEAN_NAME)
    public void recharge() {
        System.out.println(Thread.currentThread() + "模拟异步充值");
    }
}
```
<a name="NwXXf"></a>
#### 异步提现服务
```java
import org.springframework.scheduling.annotation.Async;
import org.springframework.stereotype.Component;

@Component
public class CashOutService {
    //模拟异步提现
    @Async(MainConfig5.CASHOUT_EXECUTORS_BEAN_NAME)
    public void cashOut() {
        System.out.println(Thread.currentThread() + "模拟异步提现");
    }
}
```
<a name="QXDqb"></a>
#### Spring配置类
注意@0、@1、@2、@3、@4这几个地方的代码，采用线程池隔离的方式，注册了2个线程池，分别用来处理上面的2个异步业务。
```java
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.scheduling.annotation.EnableAsync;
import org.springframework.scheduling.concurrent.ThreadPoolTaskExecutor;

import java.util.concurrent.Executor;

@EnableAsync //@0：启用方法异步调用
@ComponentScan
public class MainConfig5 {

    //@1：值业务线程池bean名称
    public static final String RECHARGE_EXECUTORS_BEAN_NAME = "rechargeExecutors";
    //@2：提现业务线程池bean名称
    public static final String CASHOUT_EXECUTORS_BEAN_NAME = "cashOutExecutors";

    /**
     * @3：充值的线程池，线程名称以recharge-thread-开头
     * @return
     */
    @Bean(RECHARGE_EXECUTORS_BEAN_NAME)
    public Executor rechargeExecutors() {
        ThreadPoolTaskExecutor executor = new ThreadPoolTaskExecutor();
        executor.setCorePoolSize(10);
        executor.setMaxPoolSize(100);
        //线程名称前缀
        executor.setThreadNamePrefix("recharge-thread-");
        return executor;
    }

    /**
     * @4: 充值的线程池，线程名称以cashOut-thread-开头
     *
     * @return
     */
    @Bean(CASHOUT_EXECUTORS_BEAN_NAME)
    public Executor cashOutExecutors() {
        ThreadPoolTaskExecutor executor = new ThreadPoolTaskExecutor();
        executor.setCorePoolSize(10);
        executor.setMaxPoolSize(100);
        //线程名称前缀
        executor.setThreadNamePrefix("cashOut-thread-");
        return executor;
    }
}
```
<a name="ISZwB"></a>
#### 测试代码
```java
@Test
public void test7() throws InterruptedException {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext();
    context.register(MainConfig5.class);
    context.refresh();

    RechargeService rechargeService = context.getBean(RechargeService.class);
    rechargeService.recharge();
    CashOutService cashOutService = context.getBean(CashOutService.class);
    cashOutService.cashOut();

    //休眠一下，防止@Test退出
    TimeUnit.SECONDS.sleep(3);
}
```
<a name="HyAcm"></a>
#### 运行输出
```
Thread[recharge-thread-1,5,main]模拟异步充值
Thread[cashOut-thread-1,5,main]模拟异步提现
```
输出中可以看出2个业务使用的是不同的线程池执行的。
<a name="voIfB"></a>
## 9、源码 & 原理
内部使用aop实现的，`@EnableAsync`会引入一个bean后置处理器：`AsyncAnnotationBeanPostProcessor`，将其注册到Spring容器，这个bean后置处理器在所有bean创建过程中，判断bean的类上是否有`@Async`注解或者类中是否有`@Async`标注的方法，如果有，会通过aop给这个bean生成代理对象，会在代理对象中添加一个切面：`org.springframework.scheduling.annotation.AsyncAnnotationAdvisor`，这个切面中会引入一个拦截器：`AnnotationAsyncExecutionInterceptor`，方法异步调用的关键代码就是在这个拦截器的`invoke`方法中实现的，可以去看一下。
