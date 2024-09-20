Java Spring
<a name="iqLHp"></a>
## 注册后置处理器开启对事务的支持
<a name="F9ZFo"></a>
### `@EnableTransactionManagement`
`@EnableTransactionManagement`注解的主要作用是开启对事务的支持，源码如下：
```java
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@Documented
@Import(TransactionManagementConfigurationSelector.class)
public @interface EnableTransactionManagement {
    boolean proxyTargetClass() default false;
    AdviceMode mode() default AdviceMode.PROXY;
    int order() default Ordered.LOWEST_PRECEDENCE;
}
```
这里最核心的是`TransactionManagementConfigurationSelector`类，这个类主要的作用是通过`ImportSelector`注册了`AutoProxyRegistrar`和`ProxyTransactionManagementConfiguration`2个组件，源码如下：
```java
public class TransactionManagementConfigurationSelector extends AdviceModeImportSelector<EnableTransactionManagement> {
    @Override
    protected String[] selectImports(AdviceMode adviceMode) {
        switch (adviceMode) {
            case PROXY:
                // 注册 InfrastructureAdvisorAutoProxyCreator 后置处理器和事务管理器组件
                return new String[] {AutoProxyRegistrar.class.getName(), ProxyTransactionManagementConfiguration.class.getName()};
            case ASPECTJ:
                return new String[] {TransactionManagementConfigUtils.TRANSACTION_ASPECT_CONFIGURATION_CLASS_NAME};
            default:
                return null;
        }
    }
}
```
<a name="Nc8Pl"></a>
### `AutoProxyRegistrar`
`AutoProxyRegistrar` 的主要作用是将`InfrastructureAdvisorAutoProxyCreator`后置处理器注册到容器，注册这个后置处理器和Spring AOP注册`AnnotationAwareAspectJAutoProxyCreator`后置处理器一样，这里就不在重复说明了。`InfrastructureAdvisorAutoProxyCreator` 他是实现了`BeanPostProcessor` 接口的后置处理器，所以所有 Bean 的初始化都会调用其 `postProcessAfterInitialization` 方法，这个方法的实现是在其父类`AbstractAutoProxyCreator`类中。
<a name="TXHdq"></a>
### `ProxyTransactionManagementConfiguration`
通过`ProxyTransactionManagementConfiguration`来注册事务管理器组件，这个类本身也是一个配置类。在这个配置类中将会注册一下三个组件：

- `BeanFactoryTransactionAttributeSourceAdvisor`：事务增强器，包含了切面组件 `TransactionInterceptor`和标签解析器`TransactionAttributeSource`
- `TransactionAttributeSource`：`@Transaction`注解标签解析器
- `TransactionInterceptor`：保存了事务属性信息，事务管理器；它本身也是一个方法拦截器，在`invoke`方法中进行了事务的处理。
<a name="HwAwf"></a>
## 创建代理Bean
上面说了所以所有 Bean 的初始化都会调用其 `AbstractAutoProxyCreator#postProcessAfterInitialization` 方法来完成Bean的增强，跟进去可以看到这段代码：
```java
@Override
public Object postProcessAfterInitialization(Object bean, String beanName) throws BeansException {
    if (bean != null) {
        Object cacheKey = getCacheKey(bean.getClass(), beanName);
        if (!this.earlyProxyReferences.contains(cacheKey)) {
            return wrapIfNecessary(bean, beanName, cacheKey);
        }
    }
    return bean;
}
```
可以看到生代理对象是在`wrapIfNecessary(bean, beanName, cacheKey);`方法中完成的，源码如下：
```java
protected Object wrapIfNecessary(Object bean, String beanName, Object cacheKey) {

    // 如果存在建言那么久创建代理类
    // 获取拦截链
    Object[] specificInterceptors = getAdvicesAndAdvisorsForBean(bean.getClass(), beanName, null);
    if (specificInterceptors != DO_NOT_PROXY) {
        this.advisedBeans.put(cacheKey, Boolean.TRUE);
        // 使用拦截链创建代理对象，对原有的Bean进行增强
        Object proxy = createProxy(
            bean.getClass(), beanName, specificInterceptors, new SingletonTargetSource(bean));
        this.proxyTypes.put(cacheKey, proxy.getClass());
        return proxy;
    }
    this.advisedBeans.put(cacheKey, Boolean.FALSE);
    return bean;
}
```
找到拦截链的的核心方法是 `BeanFactoryAdvisorRetrievalHelper#findAdvisorBeans`方法
```
findAdvisorBeans:67, BeanFactoryAdvisorRetrievalHelper (org.springframework.aop.framework.autoproxy)
findCandidateAdvisors:102, AbstractAdvisorAutoProxyCreator (org.springframework.aop.framework.autoproxy)
findEligibleAdvisors:88, AbstractAdvisorAutoProxyCreator (org.springframework.aop.framework.autoproxy)
getAdvicesAndAdvisorsForBean:70, AbstractAdvisorAutoProxyCreator (org.springframework.aop.framework.autoproxy)
wrapIfNecessary:346, AbstractAutoProxyCreator (org.springframework.aop.framework.autoproxy)
postProcessAfterInitialization:298, AbstractAutoProxyCreator (org.springframework.aop.framework.autoproxy)
applyBeanPostProcessorsAfterInitialization:423, AbstractAutowireCapableBeanFactory (org.springframework.beans.factory.support)
initializeBean:1638, AbstractAutowireCapableBeanFactory (org.springframework.beans.factory.support)
doCreateBean:555, AbstractAutowireCapableBeanFactory (org.springframework.beans.factory.support)
createBean:483, AbstractAutowireCapableBeanFactory (org.springframework.beans.factory.support)
getObject:312, AbstractBeanFactory$1 (org.springframework.beans.factory.support)
getSingleton:230, DefaultSingletonBeanRegistry (org.springframework.beans.factory.support)
doGetBean:308, AbstractBeanFactory (org.springframework.beans.factory.support)
getBean:197, AbstractBeanFactory (org.springframework.beans.factory.support)
preInstantiateSingletons:761, DefaultListableBeanFactory (org.springframework.beans.factory.support)
finishBeanFactoryInitialization:867, AbstractApplicationContext (org.springframework.context.support)
refresh:543, AbstractApplicationContext (org.springframework.context.support)
<init>:84, AnnotationConfigApplicationContext (org.springframework.context.annotation)
```
源码如下：
```java
public List<Advisor> findAdvisorBeans() {
    // Determine list of advisor bean names, if not cached already.
    String[] advisorNames = null;
    synchronized (this) {
        advisorNames = this.cachedAdvisorBeanNames;
        if (advisorNames == null) {
            // 获取所有增强器的名称
            advisorNames = BeanFactoryUtils.beanNamesForTypeIncludingAncestors(
                this.beanFactory, Advisor.class, true, false);
            this.cachedAdvisorBeanNames = advisorNames;
        }
    }
    if (advisorNames.length == 0) {
        return new LinkedList<Advisor>();
    }
    List<Advisor> advisors = new LinkedList<Advisor>();
    for (String name : advisorNames) {
        if (isEligibleBean(name)) {
            if (this.beanFactory.isCurrentlyInCreation(name)) {
                if (logger.isDebugEnabled()) {
                    logger.debug("Skipping currently created advisor '" + name + "'");
                }
            }
            else {
                try {
                    // 根据名称增强器
                    advisors.add(this.beanFactory.getBean(name, Advisor.class));
                }
                ...
                }
        }
    }
    // 返回拦截链
    return advisors;
}
```
创建代理Bean的核心流程：

1. 单例Bean初始化完成后执行后置处理器 `AbstractAutoProxyCreator#postProcessAfterInitialization` 方法
2. 在容器中找`Advisor`类型的所有增强器名称，这就会将与事务相关的增强器`BeanFactoryTransactionAttributeSourceAdvisor`找出来
3. 根据增强器名称获取对应的实例，并生成拦截链
4. 判断代理类型
5. 根据不同的代理类型和拦截链创建代理对象
<a name="c9BcW"></a>
## 执行业务方法进行拦截
前面AOP说过不管理是`JdkDynamicAopProxy`还是`CglibAopProxy`代理，他们的执行最终都会去调用`MethodInterceptor.invoke()`方法，而事务对应的方法拦截器是`TransactionInterceptor`类。也就是说对事务的增强起始是在`TransactionInterceptor`的`invoke`方法中。源码如下：
```java
@Override
public Object invoke(final MethodInvocation invocation) throws Throwable {
    ...
        return invokeWithinTransaction(invocation.getMethod(), targetClass, new InvocationCallback() {
            @Override
            public Object proceedWithInvocation() throws Throwable {
                return invocation.proceed();
            }
        });
}
protected Object invokeWithinTransaction(Method method, Class<?> targetClass, final InvocationCallback invocation)
    throws Throwable {
    // 获取事务属性
    final TransactionAttribute txAttr = getTransactionAttributeSource().getTransactionAttribute(method, targetClass);
    // 获取事务管理器
    final PlatformTransactionManager tm = determineTransactionManager(txAttr);
    // 构造方法唯一标示
    final String joinpointIdentification = methodIdentification(method, targetClass, txAttr);
    // 声明式事务
    if (txAttr == null || !(tm instanceof CallbackPreferringPlatformTransactionManager)) {
        // 创建事务
        TransactionInfo txInfo = createTransactionIfNecessary(tm, txAttr, joinpointIdentification);
        Object retVal = null;
        try {
            // 执行被增强的方法
            retVal = invocation.proceedWithInvocation();
        }
        catch (Throwable ex) {
            // 异常回滚
            completeTransactionAfterThrowing(txInfo, ex);
            throw ex;
        }
        finally {
            // 清除信息
            cleanupTransactionInfo(txInfo);
        }
        // 提交事务
        commitTransactionAfterReturning(txInfo);
        return retVal;
    }
    // 编程式事务
    ...
}
```
从上面的源码可以看出，一个事务处理的标准流程：

1. `createTransactionIfNecessary` 创建一个事务
2. `invocation.proceedWithInvocation();` 执行业务方法
3. `completeTransactionAfterThrowing(txInfo, ex);` 如果遇到异常，事务回滚
4. `commitTransactionAfterReturning(txInfo);` 如果没有异常就提交事务

在创建，回滚和提交事务方法中还有的很多对嵌套事务的逻辑，比如事务的传递性，事务回滚的条件判断等。
