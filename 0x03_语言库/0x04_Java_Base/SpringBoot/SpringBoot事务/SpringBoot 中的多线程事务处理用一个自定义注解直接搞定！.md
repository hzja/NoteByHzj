JavaSpringBoot
<a name="zyyUq"></a>
## 前言
开发的时候常常会遇到多线程事务的问题。以为添加了`@Transactional`注解就行了，其实加了注解之后会发现事务失效。<br />原因：数据库连接Spring是放在threadLocal里面，多线程场景下，拿到的数据库连接是不一样的，即是属于不同事务。<br />本文是基于SpringBoot的`@Async`注解开启多线程，并通过自定义注解和AOP实现的多线程事务，避免繁琐的手动提交/回滚事务  (CV即用、参数齐全、无需配置)
<a name="O34rS"></a>
## 一、SpringBoot多线程(声明式)的使用方法
1、SpringBoot提供了注解`@Async`来使用线程池，具体使用方法如下：<br />(1) 在启动类(配置类)添加`@EnableAsync`来开启线程池<br />(2) 在需要开启子线程的方法上添加注解`@Async`<br />注意：框架默认 ----->   来一个请求开启一个线程，在高并发下容易内存溢出<br />所以使用时需要配置自定义线程池，如下：
```java
@Configuration
@EnableAsync
public class ThreadPoolTaskConfig {

    @Bean("threadPoolTaskExecutor")//自定义线程池名称
    public ThreadPoolTaskExecutor threadPoolTaskExecutor() {

        ThreadPoolTaskExecutor executor = new ThreadPoolTaskExecutor();

        //线程池创建的核心线程数，线程池维护线程的最少数量，即使没有任务需要执行，也会一直存活
        executor.setCorePoolSize(16);

        //如果设置allowCoreThreadTimeout=true（默认false）时，核心线程会超时关闭
        //executor.setAllowCoreThreadTimeOut(true);

        //阻塞队列 当核心线程数达到最大时，新任务会放在队列中排队等待执行
        executor.setQueueCapacity(124);

        //最大线程池数量，当线程数>=corePoolSize，且任务队列已满时。线程池会创建新线程来处理任务
        //任务队列已满时, 且当线程数=maxPoolSize，，线程池会拒绝处理任务而抛出异常
        executor.setMaxPoolSize(64);

        //当线程空闲时间达到keepAliveTime时，线程会退出，直到线程数量=corePoolSize
        //允许线程空闲时间30秒，当maxPoolSize的线程在空闲时间到达的时候销毁
        //如果allowCoreThreadTimeout=true，则会直到线程数量=0
        executor.setKeepAliveSeconds(30);

        //spring 提供的 ThreadPoolTaskExecutor 线程池，是有setThreadNamePrefix() 方法的。
        //jdk 提供的ThreadPoolExecutor 线程池是没有 setThreadNamePrefix() 方法的
        executor.setThreadNamePrefix("自定义线程池-");

        // rejection-policy：拒绝策略：当线程数已经达到maxSize的时候，如何处理新任务
        // CallerRunsPolicy()：交由调用方线程运行，比如 main 线程；如果添加到线程池失败，那么主线程会自己去执行该任务，不会等待线程池中的线程去执行, (个人推荐)
        // AbortPolicy()：该策略是线程池的默认策略，如果线程池队列满了丢掉这个任务并且抛出RejectedExecutionException异常。
        // DiscardPolicy()：如果线程池队列满了，会直接丢掉这个任务并且不会有任何异常
        // DiscardOldestPolicy()：丢弃队列中最老的任务，队列满了，会将最早进入队列的任务删掉腾出空间，再尝试加入队列
        executor.setRejectedExecutionHandler(new ThreadPoolExecutor.CallerRunsPolicy());

        //设置线程池关闭的时候等待所有任务都完成再继续销毁其他的Bean，这样这些异步任务的销毁就会先于Redis线程池的销毁
        executor.setWaitForTasksToCompleteOnShutdown(true);
        //设置线程池中任务的等待时间，如果超过这个时候还没有销毁就强制销毁，以确保应用最后能够被关闭，而不是阻塞住。
        executor.setAwaitTerminationSeconds(60);

        executor.initialize();
        return executor;
    }
}
```
**开启子线程方法：**在需要开启线程的方法上添加 注解`@Async("threadPoolTaskExecutor")`即可，其中注解中的参数为自定义线程池的名称。
<a name="FlawI"></a>
## 二、自定义注解实现多线程事务控制
<a name="ImWLw"></a>
### 1、自定义注解
本文是使用了两个注解共同作用实现的，主线程当做协调者，各子线程作为参与者
```java
package com.example.anno;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

/**
 * 多线程事务注解: 主事务
 */
@Target({ElementType.METHOD})
@Retention(RetentionPolicy.RUNTIME)
public @interface MainTransaction {
    int value();//子线程数量
}
```
```java
package com.example.anno;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

/**
 * 多线程事务注解: 子事务
 */
@Target({ElementType.METHOD})
@Retention(RetentionPolicy.RUNTIME)
public @interface SonTransaction {
    String value() default "";
}
```
**解释：**<br />两个注解都是用在方法上的，须配合`@Transactional(rollbackFor = Exception.class)`一起使用<br />`**@MainTransaction**`**注解** 用在调用方，其参数为必填，参数值为本方法中调用的方法开启的线程数，如：在这个方法中调用的方法中有2个方法用`@Async`注解开启了子线程，则参数为`@MainTransaction(2)`，另外如果未使用`@MainTransaction`注解，则直接已无多线程事务执行(不影响方法的单线程事务)<br />`**@SonTransaction**`**注解** 用在被调用方(开启线程的方法)，无需传入参数
<a name="CMexd"></a>
### 2、AOP内容
代码如下：
```java
package com.example.aop;

import com.baomidou.mybatisplus.core.toolkit.CollectionUtils;
import com.example.anno.MainTransaction;
import org.aspectj.lang.ProceedingJoinPoint;
import org.aspectj.lang.annotation.Around;
import org.aspectj.lang.annotation.Aspect;
import org.springframework.stereotype.Component;
import org.springframework.transaction.PlatformTransactionManager;
import org.springframework.transaction.TransactionDefinition;
import org.springframework.transaction.TransactionStatus;
import org.springframework.transaction.support.DefaultTransactionDefinition;

import javax.annotation.Resource;
import java.util.HashMap;
import java.util.Map;
import java.util.Vector;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.atomic.AtomicBoolean;

/**
 * 多线程事务
 */
@Aspect
@Component
public class TransactionAop {

    //用来存储各线程计数器数据(每次执行后会从map中删除)
    private static final Map<String, Object> map = new HashMap<>();

    @Resource
    private PlatformTransactionManager transactionManager;

    @Around("@annotation(mainTransaction)")
    public void mainIntercept(ProceedingJoinPoint joinPoint, MainTransaction mainTransaction) throws Throwable {
        //当前线程名称
        Thread thread = Thread.currentThread();
        String threadName = thread.getName();
        //初始化计数器
        CountDownLatch mainDownLatch = new CountDownLatch(1);
        CountDownLatch sonDownLatch = new CountDownLatch(mainTransaction.value());//@MainTransaction注解中的参数, 为子线程的数量
        // 用来记录子线程的运行状态，只要有一个失败就变为true
        AtomicBoolean rollBackFlag = new AtomicBoolean(false);
        // 用来存每个子线程的异常，把每个线程的自定义异常向vector的首位置插入，其余异常向末位置插入，避免线程不安全，所以使用vector代替list
        Vector<Throwable> exceptionVector = new Vector<>();

        map.put(threadName + "mainDownLatch", mainDownLatch);
        map.put(threadName + "sonDownLatch", sonDownLatch);
        map.put(threadName + "rollBackFlag", rollBackFlag);
        map.put(threadName + "exceptionVector", exceptionVector);

        try {
            joinPoint.proceed();//执行方法
        } catch (Throwable e) {
            exceptionVector.add(0, e);
            rollBackFlag.set(true);//子线程回滚
            mainDownLatch.countDown();//放行所有子线程
        }

        if (!rollBackFlag.get()) {
            try {
                // sonDownLatch等待，直到所有子线程执行完插入操作，但此时还没有提交事务
                sonDownLatch.await();
                mainDownLatch.countDown();// 根据rollBackFlag状态放行子线程的await处，告知是回滚还是提交
            } catch (Exception e) {
                rollBackFlag.set(true);
                exceptionVector.add(0, e);
            }
        }
        if (CollectionUtils.isNotEmpty(exceptionVector)) {
            map.remove(threadName + "mainDownLatch");
            map.remove(threadName + "sonDownLatch");
            map.remove(threadName + "rollBackFlag");
            map.remove(threadName + "exceptionVector");
            throw exceptionVector.get(0);
        }
    }

    @Around("@annotation(com.huigu.common.anno.SonTransaction)")
    public void sonIntercept(ProceedingJoinPoint joinPoint) throws Throwable {
        Object[] args = joinPoint.getArgs();
        Thread thread = (Thread) args[args.length - 1];
        String threadName = thread.getName();
        CountDownLatch mainDownLatch = (CountDownLatch) map.get(threadName + "mainDownLatch");
        if (mainDownLatch == null) {
            //主事务未加注解时, 直接执行子事务
            joinPoint.proceed();//这里最好的方式是:交由上面的thread来调用此方法,但我没有找寻到对应api,只能直接放弃事务, 欢迎大神来优化, 留言分享
            return;
        }
        CountDownLatch sonDownLatch = (CountDownLatch) map.get(threadName + "sonDownLatch");
        AtomicBoolean rollBackFlag = (AtomicBoolean) map.get(threadName + "rollBackFlag");
        Vector<Throwable> exceptionVector = (Vector<Throwable>) map.get(threadName + "exceptionVector");

        //如果这时有一个子线程已经出错，那当前线程不需要执行
        if (rollBackFlag.get()) {
            sonDownLatch.countDown();
            return;
        }

        DefaultTransactionDefinition def = new DefaultTransactionDefinition();// 开启事务
        def.setPropagationBehavior(TransactionDefinition.PROPAGATION_REQUIRES_NEW);// 设置事务隔离级别
        TransactionStatus status = transactionManager.getTransaction(def);

        try {
            joinPoint.proceed();//执行方法

            sonDownLatch.countDown();// 对sonDownLatch-1
            mainDownLatch.await();// 如果mainDownLatch不是0，线程会在此阻塞，直到mainDownLatch变为0
            // 如果能执行到这一步说明所有子线程都已经执行完毕判断如果atomicBoolean是true就回滚false就提交
            if (rollBackFlag.get()) {
                transactionManager.rollback(status);
            } else {
                transactionManager.commit(status);
            }
        } catch (Throwable e) {
            exceptionVector.add(0, e);
            // 回滚
            transactionManager.rollback(status);
            // 并把状态设置为true
            rollBackFlag.set(true);
            mainDownLatch.countDown();
            sonDownLatch.countDown();
        }
    }
}
```
**扩展说明：**`**CountDownLatch**`**是什么?**<br />一个同步辅助类

- 创建对象时：用给定的数字初始化 `CountDownLatch`
- `countDown()` 方法：使计数减1
- await() 方法：阻塞当前线程，直至当前计数到达零。

本文中：<br />用 计数 1 初始化的 mainDownLatch 当作一个简单的开/关锁存器，或入口：在通过调用 `countDown()` 的线程打开入口前，所有调用 await 的线程都一直在入口处等待。<br />用 子线程数量 初始化的 sonDownLatch 可以使一个线程在 N 个线程完成某项操作之前一直等待，或者使其在某项操作完成 N 次之前一直等待。
<a name="Mpo3l"></a>
### 3、注解使用Demo
任务方法：
```java
package com.example.demo.service;

import com.example.demo.anno.SonTransaction;
import org.springframework.scheduling.annotation.Async;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

@Service
public class SonService {

    /**
     * 参数说明:  以下4个方法参数和此相同
     *
     * @param args   业务中需要传递的参数
     * @param thread 调用者的线程, 用于aop获取参数, 不建议以方法重写的方式简略此参数,
     *               在调用者方法中可以以此参数为标识计算子线程的个数作为注解参数,避免线程参数计算错误导致锁表
     *               传参时参数固定为: Thread.currentThread()
     */
    @Transactional(rollbackFor = Exception.class)
    @Async("threadPoolTaskExecutor")
    @SonTransaction
    public void sonMethod1(String args, Thread thread) {
        System.out.println(args + "开启了线程");
    }

    @Transactional(rollbackFor = Exception.class)
    @Async("threadPoolTaskExecutor")
    @SonTransaction
    public void sonMethod2(String args1, String args2, Thread thread) {
        System.out.println(args1 + "和" + args2 + "开启了线程");
    }

    @Transactional(rollbackFor = Exception.class)
    @Async("threadPoolTaskExecutor")
    @SonTransaction
    public void sonMethod3(String args, Thread thread) {
        System.out.println(args + "开启了线程");
    }

    //sonMethod4方法没有使用线程池
    @Transactional(rollbackFor = Exception.class)
    public void sonMethod4(String args) {
        System.out.println(args + "没有开启线程");
    }
}
```
调用方：
```java
package com.example.demo.service;

import com.example.demo.anno.MainTransaction;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import javax.annotation.Resource;

@Service
public class MainService {

    @Resource
    private SonService sonService;

    @MainTransaction(3)//调用的方法中sonMethod1/sonMethod2/sonMethod3使用@Async开启了线程, 所以参数为: 3
    @Transactional(rollbackFor = Exception.class)
    public void test1() {
        sonService.sonMethod1("路飞", Thread.currentThread());
        sonService.sonMethod2("索隆", "山治", Thread.currentThread());
        sonService.sonMethod3("娜美", Thread.currentThread());
        sonService.sonMethod4("罗宾");
    }

    /*
     * 有的业务中存在if的多种可能, 每一种走向调用的方法(开启线程的方法)数量如果不同, 这时可以选择放弃使用@MainTransaction注解避免锁表
     * 这时候如果发生异常会导致多线程不能同时回滚, 可根据业务自己权衡是否使用
     */
    @Transactional(rollbackFor = Exception.class)
    public void test2() {
        sonService.sonMethod1("路飞", Thread.currentThread());
        sonService.sonMethod2("索隆", "山治", Thread.currentThread());
        sonService.sonMethod3("娜美", Thread.currentThread());
        sonService.sonMethod4("罗宾");
    }
}
```
