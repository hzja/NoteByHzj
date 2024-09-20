Java<br />在开发中使用线程池去执行异步任务是比较普遍的操作，然而虽然有些异步操作并不十分要求可靠性和实时性，但总归业务还是需要的。如果在每次的服务发版过程中，不去介入线程池的停机逻辑，那么很有可能就会造成线程池中队列的任务还未执行完成，自然就会造成数据的丢失。
<a name="HaA3M"></a>
## 探究
注意，本文所有前提是对进程进行下线时使用的是`kill -15`<br />Spring已经实现了自己的优雅停机方案，详细请参考`org.springframework.context.support.AbstractApplicationContext#registerShutdownHook`，然后主要看调用的`org.springframework.context.support.AbstractApplicationContext#doClose`， 在这个方法里定义了容器销毁的执行顺序
```java
protected void doClose() {
    // Check whether an actual close attempt is necessary...
    if (this.active.get() && this.closed.compareAndSet(false, true)) {
        if (logger.isDebugEnabled()) {
            logger.debug("Closing " + this);
        }

        LiveBeansView.unregisterApplicationContext(this);

        try {
            // Publish shutdown event.
            publishEvent(new ContextClosedEvent(this));
        }
        catch (Throwable ex) {
            logger.warn("Exception thrown from ApplicationListener handling ContextClosedEvent", ex);
        }

        // Stop all Lifecycle beans, to avoid delays during individual destruction.
        if (this.lifecycleProcessor != null) {
            try {
                this.lifecycleProcessor.onClose();
            }
            catch (Throwable ex) {
                logger.warn("Exception thrown from LifecycleProcessor on context close", ex);
            }
        }

        // Destroy all cached singletons in the context's BeanFactory.
        destroyBeans();

        // Close the state of this context itself.
        closeBeanFactory();

        // Let subclasses do some final clean-up if they wish...
        onClose();

        // Reset local application listeners to pre-refresh state.
        if (this.earlyApplicationListeners != null) {
            this.applicationListeners.clear();
            this.applicationListeners.addAll(this.earlyApplicationListeners);
        }

        // Switch to inactive.
        this.active.set(false);
    }
}
```
先主要关注下`destroyBeans`这个方法，看bean的销毁逻辑是什么，然后看到了下面的一个bean的销毁顺序逻辑，具体方法在`org.springframework.beans.factory.support.DefaultSingletonBeanRegistry#destroySingletons`
```java
private final Map<String, Object> disposableBeans = new LinkedHashMap<>();

public void destroySingletons() {
    if (logger.isTraceEnabled()) {
        logger.trace("Destroying singletons in " + this);
    }
    synchronized (this.singletonObjects) {
        this.singletonsCurrentlyInDestruction = true;
    }

    String[] disposableBeanNames;
    synchronized (this.disposableBeans) {
        disposableBeanNames = StringUtils.toStringArray(this.disposableBeans.keySet());
    }
    for (int i = disposableBeanNames.length - 1; i >= 0; i--) {
        destroySingleton(disposableBeanNames[i]);
    }

    this.containedBeanMap.clear();
    this.dependentBeanMap.clear();
    this.dependenciesForBeanMap.clear();

    clearSingletonCache();
}
```
可以看到最至关重要的就是一个属性`disposableBeans`，这个属性是一个`LinkedHashMap`， 因此属性是有序的，所以销毁的时候也是按照某种规则保持和放入一样的顺序进行销毁的，现在就是要确认这个属性里到底存的是什么。<br />经过调试发现，在创建bean的`org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory#doCreateBean`方法中，会调用一个方法`org.springframework.beans.factory.support.AbstractBeanFactory#registerDisposableBeanIfNecessary`， 在这个方法中会调用`org.springframework.beans.factory.support.DefaultSingletonBeanRegistry#registerDisposableBean`然后将当前创建的bean放入到属性`disposableBeans`中，那么现在来看一下放入的逻辑什么？<br />相关代码贴一下
```java
protected void registerDisposableBeanIfNecessary(String beanName, Object bean, RootBeanDefinition mbd) {
    AccessControlContext acc = (System.getSecurityManager() != null ? getAccessControlContext() : null);
    if (!mbd.isPrototype() && requiresDestruction(bean, mbd)) {
        if (mbd.isSingleton()) {
            // Register a DisposableBean implementation that performs all destruction
            // work for the given bean: DestructionAwareBeanPostProcessors,
            // DisposableBean interface, custom destroy method.
            registerDisposableBean(beanName,
                                   new DisposableBeanAdapter(bean, beanName, mbd, getBeanPostProcessors(), acc));
        }
        else {
            // A bean with a custom scope...
            Scope scope = this.scopes.get(mbd.getScope());
            if (scope == null) {
                throw new IllegalStateException("No Scope registered for scope name '" + mbd.getScope() + "'");
            }
            scope.registerDestructionCallback(beanName,
                                              new DisposableBeanAdapter(bean, beanName, mbd, getBeanPostProcessors(), acc));
        }
    }
}
```
org.springframework.beans.factory.support.AbstractBeanFactory#requiresDestruction
```java
protected boolean requiresDestruction(Object bean, RootBeanDefinition mbd) {
    return (bean != null &&
            (DisposableBeanAdapter.hasDestroyMethod(bean, mbd) || (hasDestructionAwareBeanPostProcessors() &&
                                                                   DisposableBeanAdapter.hasApplicableProcessors(bean, getBeanPostProcessors()))));
}
```
经过两个方法可以看到如果一个bean的scope是`singleton`并且这个bean实现了`org.springframework.beans.factory.DisposableBean`这个接口的`destroy()`方法，那么就会满足条件。<br />现在可以确定一点，如果将线程池交给Spring管理，并且实现它的`close`方法，就可以在应用收到下线信号的时候执行这个bean的销毁方法，那么就可以在销毁方法中写线程池的停机逻辑。<br />Spring提供了线程池的封装，在Spring中如果要定义线程池一般会使用`org.springframework.scheduling.concurrent.ThreadPoolTaskExecutor`以及用于任务调度的`org.springframework.scheduling.concurrent.ThreadPoolTaskScheduler`，先来简单看个定义`ThreadPoolTaskExecutor`线程池的例子
```java
@Configuration
public class ThreadConfig {

    @Bean
    public ThreadPoolTaskExecutor testExecutor() {
        ThreadPoolTaskExecutor threadPoolTaskExecutor = new ThreadPoolTaskExecutor();
        threadPoolTaskExecutor.setThreadNamePrefix("test-shutdown-pool-");
        threadPoolTaskExecutor.setCorePoolSize(1);
        threadPoolTaskExecutor.setMaxPoolSize(1);
        threadPoolTaskExecutor.setKeepAliveSeconds(60);
        threadPoolTaskExecutor.setQueueCapacity(1000);
        threadPoolTaskExecutor.setRejectedExecutionHandler(new ThreadPoolExecutor.AbortPolicy());
        return threadPoolTaskExecutor;
    }
}
```
现在来一下线程池的这个类结构，`ThreadPoolTaskExecutor`继承了`org.springframework.scheduling.concurrent.ExecutorConfigurationSupport`，实现了`org.springframework.beans.factory.DisposableBean`，完整结构如下
```java
public class ThreadPoolTaskExecutor extends ExecutorConfigurationSupport
    implements AsyncListenableTaskExecutor, SchedulingTaskExecutor {

    }

public abstract class ExecutorConfigurationSupport extends CustomizableThreadFactory
    implements BeanNameAware, InitializingBean, DisposableBean {
    }
```
从这里就能看到其实线程池类`ThreadPoolTaskExecutor`是满足最开始看到的销毁条件的，那么现在就来看下在父类`ExecutorConfigurationSupport`中定义的`destroy()`方法，将其中关键部分代码摘录下来
```java
public abstract class ExecutorConfigurationSupport extends CustomizableThreadFactory
      implements BeanNameAware, InitializingBean, DisposableBean {
  
    private boolean waitForTasksToCompleteOnShutdown = false;

   private long awaitTerminationMillis = 0;
  
    @Nullable
   private ExecutorService executor;
  
    @Override
   public void destroy() {
       shutdown();
   }

    /**
     * Perform a shutdown on the underlying ExecutorService.
     * @see java.util.concurrent.ExecutorService#shutdown()
     * @see java.util.concurrent.ExecutorService#shutdownNow()
     */
    public void shutdown() {
        if (logger.isInfoEnabled()) {
          logger.info("Shutting down ExecutorService" + (this.beanName != null ? " '" + this.beanName + "'" : ""));
        }
        if (this.executor != null) {
          if (this.waitForTasksToCompleteOnShutdown) {
            this.executor.shutdown();
          }
          else {
            for (Runnable remainingTask : this.executor.shutdownNow()) {
              cancelRemainingTask(remainingTask);
            }
          }
          awaitTerminationIfNecessary(this.executor);
        }
    }

    private void awaitTerminationIfNecessary(ExecutorService executor) {
        if (this.awaitTerminationMillis > 0) {
          try {
            if (!executor.awaitTermination(this.awaitTerminationMillis, TimeUnit.MILLISECONDS)) {
              if (logger.isWarnEnabled()) {
                logger.warn("Timed out while waiting for executor" +
                    (this.beanName != null ? " '" + this.beanName + "'" : "") + " to terminate");
              }
            }
          }
          catch (InterruptedException ex) {
            if (logger.isWarnEnabled()) {
              logger.warn("Interrupted while waiting for executor" +
                  (this.beanName != null ? " '" + this.beanName + "'" : "") + " to terminate");
            }
            Thread.currentThread().interrupt();
          }
        }
    }

    protected void cancelRemainingTask(Runnable task) {
        if (task instanceof Future) {
            ((Future<?>) task).cancel(true);
        }
    }
}
```
整个的逻辑还是比较清晰的， 在容器销毁的时候会调用本地`shutdown()`方法， 在这个方法中会去判断`waitForTasksToCompleteOnShutdown`这个的属性，如果为true，则调用线程池的`shutdown()`方法，这个方法并不会让线程池立即停止，而是不再接受新的任务并继续执行已经在队列中的任务。如果为false，则取消任务队列中的剩余任务。而这个属性的默认值为false。因此默认是不具备我们需要的功能的。<br />然而无论这个值的属性最终是否为TRUE，最终都会调用方法`awaitTerminationIfNecessary()`， 线程的停止无论是`shutdown`还是`shutdownNow`都无法保证线程池能够停止下来，因为需要配合线程池的方法`awaitTermination`使用，在这个方法中指定一个最大等待时间，则能够保证线程池最终一定可以被停止下来。<br />不知道有没有注意到一个细节，上述所有对线程池的操作使用的属性都是`private ExecutorService executor;`，那么这个`executor`是什么时候赋值的呢？<br />毕竟在创建bean的时候是直接new的`ThreadPoolTaskExecutor`，并没有去处理这个属性。还是看线程池的父类`ExecutorConfigurationSupport`，其实现了接口`org.springframework.beans.factory.InitializingBean`，在容器初始化完成后有这样一段代码
```java
@Override
    public void afterPropertiesSet() {
    initialize();
}

/**
 * Set up the ExecutorService.
 */
public void initialize() {
    if (logger.isInfoEnabled()) {
        logger.info("Initializing ExecutorService" + (this.beanName != null ? " '" + this.beanName + "'" : ""));
    }
    if (!this.threadNamePrefixSet && this.beanName != null) {
        setThreadNamePrefix(this.beanName + "-");
    }
    this.executor = initializeExecutor(this.threadFactory, this.rejectedExecutionHandler);
}

protected abstract ExecutorService initializeExecutor(
    ThreadFactory threadFactory, RejectedExecutionHandler rejectedExecutionHandler);
```
线程池bean在初始化完成后会调用父类的`afterPropertiesSet`方法，上面的代码已经很清晰的说明了问题， 最终父类中又定义了抽象方法`initializeExecutor()`，供子类去具体实现如果初始化这个属性executor， 因为线程池的实现除了普通的异步任务线程池`ThreadPoolTaskExecutor`，还有基于定时调度的线程池`ThreadPoolTaskExecutor`， 具体实现这里就不贴出来了，反正已经能够看出来这个属性是如何被赋值的了，所以上述销毁时代码可以直接使用。<br />现在整体总结下来，其实可以发现Spring已经实现了线程池的优雅停机规则，在接收到停机信号时，先拒绝接收新的任务，并继续执行已经接受的任务，在任务执行完成或者到达最大等待时间，完成线程池的关闭。这么一整套逻辑正是我们所需要的，而如果要使用这个逻辑，仅仅需要在配置线程池的时候指定下上面看到的`waitForTasksToCompleteOnShutdown`属性和`awaitTerminationMillis`属性。<br />修改一下上面之前写的线程池定义代码，将`waitForTasksToCompleteOnShutdown`属性设置为true，并指定`awaitTerminationMillis`。
```java
@Configuration
public class ThreadConfig {

    @Bean
    public ThreadPoolTaskExecutor testExecutor() {
        ThreadPoolTaskExecutor threadPoolTaskExecutor = new ThreadPoolTaskExecutor();
        threadPoolTaskExecutor.setThreadNamePrefix("test-shutdown-pool-");
        threadPoolTaskExecutor.setCorePoolSize(1);
        threadPoolTaskExecutor.setMaxPoolSize(1);
        threadPoolTaskExecutor.setKeepAliveSeconds(60);
        threadPoolTaskExecutor.setQueueCapacity(1000);
        threadPoolTaskExecutor.setWaitForTasksToCompleteOnShutdown(true);
        threadPoolTaskExecutor.setAwaitTerminationSeconds(60);
        threadPoolTaskExecutor.setRejectedExecutionHandler(new ThreadPoolExecutor.AbortPolicy());
        return threadPoolTaskExecutor;
    }
}
```
那么到目前这样处理是不是就没有问题了呢？<br />要分情况来看， 如果按照上述操作，是能够保证最初预期目标的。线程池会在接收到下线指令时停止接受新的任务，并继续执行队列中的未完成的任务，直到任务完成或者达到指定的最大等待时间。<br />如果任务是一些不操作其它资源的，只是一些本地计算或者日志什么之类的，那么任务不会出问题。但是如果任务本身依赖各种数据源，比如数据库、缓存等之类的，那么就会出现任务本身会执行，但是却会失败的问题，因为数据源已经早于线程池关闭了。<br />那么，能不能控制数据源和线程池的销毁顺序呢？在上面看到销毁顺序的时候看到了线程池会在放入到`disposableBeans`的原因，其实数据源也是会被放入到这个属性中的，这个原因和Spring的生命周期无关，而是另外一个判断条件。<br />之前没有贴出来具体代码，现在来看下`org.springframework.beans.factory.support.AbstractBeanFactory#requiresDestruction`方法中的调用的另外一个本地方法`org.springframework.beans.factory.support.DisposableBeanAdapter#hasDestroyMethod`
```java
public static boolean hasDestroyMethod(Object bean, RootBeanDefinition beanDefinition) {
    if (bean instanceof DisposableBean || bean instanceof AutoCloseable) {
        return true;
    }
    String destroyMethodName = beanDefinition.getDestroyMethodName();
    if (AbstractBeanDefinition.INFER_METHOD.equals(destroyMethodName)) {
        return (ClassUtils.hasMethod(bean.getClass(), CLOSE_METHOD_NAME) ||
                ClassUtils.hasMethod(bean.getClass(), SHUTDOWN_METHOD_NAME));
    }
    return StringUtils.hasLength(destroyMethodName);
}
```
之前线程池能够执行销毁流程是因为实现了接口`DisposableBean`， 而数据源则是实现了另外一个接口`AutoCloseable`， 因此数据源也是会执行销毁逻辑的。<br />现在只要介入bean的创建优先级即可， 使用`org.springframework.core.annotation.Order`注解来指定线程池创建的高优先级，如下。
```java
@Configuration
@Order(value = Ordered.HIGHEST_PRECEDENCE - 10)
public class ThreadConfig {

    @Bean
    @Order(value = Ordered.HIGHEST_PRECEDENCE - 10)
    public ThreadPoolTaskExecutor testExecutor() {
        ThreadPoolTaskExecutor threadPoolTaskExecutor = new ThreadPoolTaskExecutor();
        threadPoolTaskExecutor.setThreadNamePrefix("test-shutdown-pool-");
        threadPoolTaskExecutor.setCorePoolSize(1);
        threadPoolTaskExecutor.setMaxPoolSize(1);
        threadPoolTaskExecutor.setKeepAliveSeconds(60);
        threadPoolTaskExecutor.setQueueCapacity(2000000);
        threadPoolTaskExecutor.setWaitForTasksToCompleteOnShutdown(true);
        threadPoolTaskExecutor.setAwaitTerminationSeconds(60);
        threadPoolTaskExecutor.setRejectedExecutionHandler(new ThreadPoolExecutor.AbortPolicy());
        return threadPoolTaskExecutor;
    }
}
```
当然实际上的优先级要根据情况调整，但是并没有生效。后来看到一个说法，`org.springframework.core.annotation.Order`注解无法决定bean的创建顺序，只能是bean创建完成后的一些业务的执行时间。这个问题没搞懂，反正`Order`未生效。那么只能抄他的代码然后自己实现了。<br />该如何处理呢？
<a name="AQ7Lu"></a>
#### 自实现
回到最初的代码`org.springframework.context.support.AbstractApplicationContext#doClose`，之前一直在看销毁bean的逻辑，但是其实可以看到在此之前Spring发布了一个`ContextClosedEvent`事件，也就是说这个事件是早于Spring自己的bean销毁逻辑前面的。<br />利用这个机制，可以将线程池的销毁逻辑抄过来，并且监听`ContextClosedEvent`这个事件，然后在这个事件里执行本地自己一些不被Spring管理的线程池的销毁逻辑，正如前面看到的一样。<br />一个简单的例子如下
```java
@Component
@Slf4j
public class ThreadPoolExecutorShutdownDefinition implements ApplicationListener<ContextClosedEvent> {

    private static final List<ExecutorService> POOLS = Collections.synchronizedList(new ArrayList<>(12));

    /**
     * 线程中的任务在接收到应用关闭信号量后最多等待多久就强制终止，其实就是给剩余任务预留的时间， 到时间后线程池必须销毁
     */
    private static final long awaitTermination = 60;

    /**
     * awaitTermination的单位
     */
    private static final TimeUnit timeUnit = TimeUnit.SECONDS;


    /**
     * 注册要关闭的线程池
     * 注意如果调用这个方法的话，而线程池又是由Spring管理的，则必须等待这个bean初始化完成后才可以调用
     * 因为依赖的{@link ThreadPoolTaskExecutor#getThreadPoolExecutor()}必须要在bean的父类方法中定义的
     * 初始化{@link ExecutorConfigurationSupport#afterPropertiesSet()}方法中才会赋值
     *
     * @param threadPoolTaskExecutor
     */
    public static void registryExecutor(ThreadPoolTaskExecutor threadPoolTaskExecutor) {
        POOLS.add(threadPoolTaskExecutor.getThreadPoolExecutor());
    }

    /**
     * 注册要关闭的线程池
     * 注意如果调用这个方法的话，而线程池又是由Spring管理的，则必须等待这个bean初始化完成后才可以调用
     * 因为依赖的{@link ThreadPoolTaskExecutor#getThreadPoolExecutor()}必须要在bean的父类方法中定义的
     * 初始化{@link ExecutorConfigurationSupport#afterPropertiesSet()}方法中才会赋值
     *
     * 重写了{@link ThreadPoolTaskScheduler#initializeExecutor(java.util.concurrent.ThreadFactory, java.util.concurrent.RejectedExecutionHandler)}
     * 来对父类的{@link ExecutorConfigurationSupport#executor}赋值
     *
     * @param threadPoolTaskExecutor
     */
    public static void registryExecutor(ThreadPoolTaskScheduler threadPoolTaskExecutor) {
        POOLS.add(threadPoolTaskExecutor.getScheduledThreadPoolExecutor());
    }

    /**
     * 注册要关闭的线程池， 如果一些线程池未交由线程池管理，则可以调用这个方法
     *
     * @param executor
     */
    public static void registryExecutor(ExecutorService executor) {
        POOLS.add(executor);
    }

    /**
     * 参考{@link org.springframework.scheduling.concurrent.ExecutorConfigurationSupport#shutdown()}
     *
     * @param event the event to respond to
     */
    @Override
    public void onApplicationEvent(ContextClosedEvent event) {
        log.info("容器关闭前处理线程池优雅关闭开始, 当前要处理的线程池数量为: {} >>>>>>>>>>>>>>>>", POOLS.size());
        if (CollectionUtils.isEmpty(POOLS)) {
            return;
        }
        for (ExecutorService pool : POOLS) {
            pool.shutdown();
            try {
                if (!pool.awaitTermination(awaitTermination, timeUnit)) {
                    if (log.isWarnEnabled()) {
                        log.warn("Timed out while waiting for executor [{}] to terminate", pool);
                    }
                }
            }
            catch (InterruptedException ex) {
                if (log.isWarnEnabled()) {
                    log.warn("Timed out while waiting for executor [{}] to terminate", pool);
                }
                Thread.currentThread().interrupt();
            }
        }
    }
}
```
如果想要本地的线程池实现优雅停机，则直接调用上述对应的`registryExecutor()`方法即可，在容器销毁的时候自然会去遍历执行对应逻辑。
<a name="IFmlq"></a>
#### 做一点补充
所谓的优雅停机， 必然是需要各方面的一些配合的。因为一个应用总归最重要的还是外界的流量，上面只是处理了线程池的问题。<br />如果是普通的springboot项目， 停机无法解决流量继续转发进来的问题， 如nginx，只要保证发布时发送kill -15的信号量并且将发布机器从nginx负载中下线。<br />如果是Dubbo项目，则麻烦一些， 问题其实和上述类似，由于Dubbo也注册了关闭的钩子， 则在停机时会同时存在多个钩子要执行的问题。如果Spring的一些容器先销毁了，Dubbo中的一些任务则无法继续执行。<br />`java.lang.Runtime#addShutdownHook`，当存在多个注册的关闭钩子时，虚拟机会以某种未指定的顺序并让它们同时运行。这就是上述问题存在的原因。
```java
public void addShutdownHook(Thread hook) {
    SecurityManager sm = System.getSecurityManager();
    if (sm != null) {
      sm.checkPermission(new RuntimePermission("shutdownHooks"));
    }
    ApplicationShutdownHooks.add(hook);
}
```
Dubbo应用则和上面手动监听容器销毁事件的原理类似， 要让Dubbo的钩子先执行，由于Dubbo已经自己注册了关闭钩子，那么步骤就变成了在Sprign容器启动的时候先移除掉Dubbo的`shutdownHook`， 然后再容器销毁的时候再添加回来。<br />综合上面线程池的逻辑， 还要保证添加Dubbo的`shutdownhook`的`Listener`先执行并且执行完它的停机逻辑之后才执行自己写的处理线程池停机的`Listener`，当然如果线程池全部交由了Spring管理，自己没有按照上面去重写这一块的逻辑， 则不需要注意这个问题。<br />移除和添加Dubbo的`shutdownHook`的逻辑类似如下.
```java
public class DubboShutdownListener implements ApplicationListener, PriorityOrdered {
    private static final Logger LOGGER = LoggerFactory.getLogger(DubboShutdownListener.class);

    public DubboShutdownListener() {
    }

    public void onApplicationEvent(ApplicationEvent event) {
        if (event instanceof ApplicationStartedEvent) {
            Runtime.getRuntime().removeShutdownHook(DubboShutdownHook.getDubboShutdownHook());
            LOGGER.info("Dubbo default shutdown hook has been removed, It will be managed by Spring");
        } else if (event instanceof ContextClosedEvent) {
            LOGGER.info("Start destroy Dubbo Container on Spring close event");
            DubboShutdownHook.getDubboShutdownHook().destroyAll();
            LOGGER.info("Dubbo Container has been destroyed finished");
        }
    }

    public int getOrder() {
        return 0;
    }
}
```
