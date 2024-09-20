Java Spring Bean
<a name="UO1sx"></a>
### 简要
Spring是一个IOC容器框架，拥有DI依赖注入（Dependency Injection），DL依赖查找（Dependency Lookup）等功能。<br />下边是根据源码分析出四个阶段，做出的生命周期解读：

1. 注册阶段
2. 实例化阶段
3. 初始化阶段
4. 销毁阶段

![DM_20230713222629_001.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1689259333196-6a735c4a-9e08-4092-8808-8018009c7d3d.png#averageHue=%23f8f8f8&clientId=ud0fdac15-d652-4&from=ui&id=u0f4caf43&originHeight=1446&originWidth=1020&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=4433445&status=done&style=shadow&taskId=u1a3f5889-803e-4f32-9d8e-5a704c5c471&title=)
<a name="TrSLl"></a>
### 注册阶段
注册阶段主要任务是通过各种`BeanDefinitionReader`读取扫描各种配置来源信息（xml文件、注解等），并转换为`BeanDefinition`的过程。<br />`BeanDefinition`可以理解为类的定义，描述一个类的基本情况，比较像注册某些网站时的基本信息，比如需要填写姓名、住址、出生日期等。<br />最终会将扫描到的类整体注册到一个`DefaultListableBeanFactory`的`Map`容器中，便于之后获取使用。
```java
public class DefaultListableBeanFactory extends AbstractAutowireCapableBeanFactory implements ConfigurableListableBeanFactory, BeanDefinitionRegistry, Serializable {
    //存储注册信息的BeanDefinition
    private final Map<String, BeanDefinition> beanDefinitionMap = new ConcurrentHashMap<>(256);
    //beanDefinitionMap的数据结构是ConcurrentHashMap，因此不能保证顺序，为了记录注册的顺序，这里使用了ArrayList类型beanDefinitionNames用来记录注册顺序
    private volatile List<String> beanDefinitionNames = new ArrayList<>(256);

    //省略部分代码.......
    @Override
    public void registerBeanDefinition(String beanName, BeanDefinition beanDefinition) throws BeanDefinitionStoreException {
        //省略判断代码.......
        // Still in startup registration phase
        this.beanDefinitionMap.put(beanName, beanDefinition);
        this.beanDefinitionNames.add(beanName);
    }
}
```
<a name="ccfrp"></a>
### 实例化阶段
在实例化阶段，Spring主要将`BeanDefinition`转换为实例Bean，并放在包装类`BeanWrapper`中。<br />无论是否设置Bean的懒加载方式，最后都会通过`AbstractBeanFactory.getBean()`方法进行实例化，并进入到`AbstractAutowireCapableBeanFactory.createBean()`方法。
```java
public abstract class AbstractAutowireCapableBeanFactory extends AbstractBeanFactory implements AutowireCapableBeanFactory {
    //省略部分代码......
    @Nullable
    protected Object resolveBeforeInstantiation(String beanName, RootBeanDefinition mbd) {
        Object bean = null;
        if (!Boolean.FALSE.equals(mbd.beforeInstantiationResolved)) {
            // Make sure bean class is actually resolved at this point.
            if (!mbd.isSynthetic() && hasInstantiationAwareBeanPostProcessors()) {
                Class<?> targetType = determineTargetType(beanName, mbd);
                if (targetType != null) {
                    //实例化前处理器
                    bean = applyBeanPostProcessorsBeforeInstantiation(targetType, beanName);
                    if (bean != null) {
                        //实例化后处理器
                        bean = applyBeanPostProcessorsAfterInitialization(bean, beanName);
                    }
                }
            }
            mbd.beforeInstantiationResolved = (bean != null);
        }
        return bean;
    }
    //省略部分代码......
}
```
在实例化阶段`AbstractAutowireCapableBeanFactory.createBeanInstance()`完成`Bean`的创建，并放到`BeanWrapper`中。
<a name="ma3qU"></a>
### 初始化阶段
初始化阶段主要是在返回Bean之前做一些处理，主要由`AbstractAutowireCapableBeanFactory.initializeBean()`方法实现。
```java
public abstract class AbstractAutowireCapableBeanFactory extends AbstractBeanFactory implements AutowireCapableBeanFactory {
    //省略部分代码......
    //真正创建Bean的方法
    protected Object doCreateBean(final String beanName, final RootBeanDefinition mbd, final @Nullable Object[] args)
        throws BeanCreationException {

        //省略部分代码......
        // Initialize the bean instance.
        //Bean对象的初始化，依赖注入在此触发
        //这个exposedObject在初始化完成之后返回作为依赖注入完成后的Bean
        Object exposedObject = bean;
        try {
            //将Bean实例对象封装，并且Bean定义中配置的属性值赋值给实例对象
            populateBean(beanName, mbd, instanceWrapper);
            //初始化Bean对象
            exposedObject = initializeBean(beanName, exposedObject, mbd);
        }
        catch (Throwable ex) {
            if (ex instanceof BeanCreationException && beanName.equals(((BeanCreationException) ex).getBeanName())) {
                throw (BeanCreationException) ex;
            }
            else {
                throw new BeanCreationException(
                    mbd.getResourceDescription(), beanName, "Initialization of bean failed", ex);
            }
        }
        //省略部分代码......
        return exposedObject;
    }

    //初始容器创建的Bean实例对象，为其添加BeanPostProcessor后置处理器
    protected Object initializeBean(final String beanName, final Object bean, @Nullable RootBeanDefinition mbd) {
        //JDK的安全机制验证权限
        if (System.getSecurityManager() != null) {
            //实现PrivilegedAction接口的匿名内部类
            AccessController.doPrivileged((PrivilegedAction<Object>) () -> {
                invokeAwareMethods(beanName, bean);
                return null;
            }, getAccessControlContext());
        }
        else {
            //为Bean实例对象包装相关属性，如名称，类加载器，所属容器等信息
            invokeAwareMethods(beanName, bean);
        }

        Object wrappedBean = bean;
        //对BeanPostProcessor后置处理器的postProcessBeforeInitialization
        //回调方法的调用，为Bean实例初始化前做一些处理
        if (mbd == null || !mbd.isSynthetic()) {
            wrappedBean = applyBeanPostProcessorsBeforeInitialization(wrappedBean, beanName);
        }

        //调用Bean实例对象初始化的方法，这个初始化方法是在Spring Bean定义配置
        //文件中通过init-method属性指定的
        try {
            invokeInitMethods(beanName, wrappedBean, mbd);
        }
        catch (Throwable ex) {
            throw new BeanCreationException(
                (mbd != null ? mbd.getResourceDescription() : null),
                beanName, "Invocation of init method failed", ex);
        }
        //对BeanPostProcessor后置处理器的postProcessAfterInitialization
        //回调方法的调用，为Bean实例初始化之后做一些处理
        if (mbd == null || !mbd.isSynthetic()) {
            wrappedBean = applyBeanPostProcessorsAfterInitialization(wrappedBean, beanName);
        }

        return wrappedBean;
    }
    //省略部分代码......
}
```
<a name="i6dsQ"></a>
### 销毁阶段
一般是在`ApplicationContext`关闭的时候调用，也就是`AbstractApplicationContext.close()`方法。<br />在注册的时候Spring通过适配器模式包装了一个类`DisposableBeanAdapter`，在销毁阶段的时候会获得这个类，进而调用到`DisposableBeanAdapter.destroy()`方法：
```java
class DisposableBeanAdapter implements DisposableBean, Runnable, Serializable {
    //省略部分代码......
    @Override
    public void destroy() {
        if (!CollectionUtils.isEmpty(this.beanPostProcessors)) {
            for (DestructionAwareBeanPostProcessor processor : this.beanPostProcessors) {
                processor.postProcessBeforeDestruction(this.bean, this.beanName);
            }
        }

        if (this.invokeDisposableBean) {
            if (logger.isDebugEnabled()) {
                logger.debug("Invoking destroy() on bean with name '" + this.beanName + "'");
            }
            try {
                if (System.getSecurityManager() != null) {
                    AccessController.doPrivileged((PrivilegedExceptionAction<Object>) () -> {
                        ((DisposableBean) bean).destroy();
                        return null;
                    }, acc);
                }
                else {
                    ((DisposableBean) bean).destroy();
                }
            }
            catch (Throwable ex) {
                String msg = "Invocation of destroy method failed on bean with name '" + this.beanName + "'";
                if (logger.isDebugEnabled()) {
                    logger.warn(msg, ex);
                }
                else {
                    logger.warn(msg + ": " + ex);
                }
            }
        }

        if (this.destroyMethod != null) {
            invokeCustomDestroyMethod(this.destroyMethod);
        }
        else if (this.destroyMethodName != null) {
            Method methodToCall = determineDestroyMethod(this.destroyMethodName);
            if (methodToCall != null) {
                invokeCustomDestroyMethod(methodToCall);
            }
        }
    }
    //省略部分代码......
}
```
销毁阶段主要包括三个销毁途径，按照执行顺序：

1. `@PreDestroy`注解，主要通过`DestructionAwareBeanPostProcessor`实现
2. 实现`DisposableBean`接口，主要通过`DisposableBean.destroy()`实现
3. 自定义销毁方`DisposableBeanAdapter.invokeCustomDestroyMethod()`实现
