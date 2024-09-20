Java Spring
<a name="ysepj"></a>
### 什么是循环依赖？
假设现在有一个对象A里面有一个属性Class B，同样的Class B对象中有一个Class A 的对象属性，那么这两个对象能相互创建成功吗？<br />![2021-09-01-14-15-18-881317.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630477471059-9905e16e-e2f0-4c51-8be0-de0d04f19f03.jpeg#clientId=u98fcf163-9073-4&from=ui&id=u3716d6f5&originHeight=199&originWidth=526&originalType=binary&ratio=1&size=13336&status=done&style=shadow&taskId=u7ce72dfe-c97b-4358-9c46-ab40dccffd4)<br />可能一般的普通代码来说肯定是可以实现
```java
A a = new A()
B b = new B()
a.setB（b）
b.setA (a)
```
看过之前讲的IOC的同学应该知道Spring官方是推荐使用构造器注入的，所以如果是通过构造器注入那就会产生一个无限循环注入的问题了,如下图所示，永远出来不？
```java
A a = new A( new B( new A(new B(......))))
```
所以面试过程中的循环依赖问题其实都是问Setter方式内部如何解决循环依赖的？而不是问的构造器。<br />比较初级的回答可能会说 是通过三层缓存，再好一点的回加上 三层缓存加上 提前暴露对象的方式（半成品）解决循环依赖问题<br />那什么是提前暴露对象呢？说白了就是spring IOC 容器的启动过程 bean 的整个生命周期过程处理的逻辑。<br />这里就直接再画一个流程图<br />![2021-09-01-14-15-18-988315.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630477471065-63b6246e-2117-49ce-87c2-f421643b2e80.jpeg#clientId=u98fcf163-9073-4&from=ui&id=HsNLF&originHeight=699&originWidth=699&originalType=binary&ratio=1&size=52529&status=done&style=shadow&taskId=u289a2d11-ffdd-412d-97ea-1a2d7e41067)<br />上面的这张图其实就是给大家说明了创建对象的时候可以分为两个大步骤，一个实例化，一个初始化。<br />同样的现在接着回到上面的问题，Setter是在哪一步处理缓存依赖的呢？<br />回顾整个流程大致可以按照这个思路来：<br />一个对象的创建 -> 实例化 -> 初始化（设置属性值）<br />那构造器的那种方式在流程中怎么体现出这个环呢？给大家画了一个图如下：<br />![2021-09-01-14-15-19-082315.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630477471062-3404c6d2-45fc-4931-b0d6-d275c0b7b603.jpeg#clientId=u98fcf163-9073-4&from=ui&id=tiotu&originHeight=384&originWidth=984&originalType=binary&ratio=1&size=31083&status=done&style=shadow&taskId=u71deac7a-b17d-4c74-82d7-5e30f4d8078)<br />springIOC容器中的bean默认都是单例的，这个大家应该清楚的。所以在设置属性的时候可以直接在容器中获取，按照上面的创建流程那整个循环依赖就产生了。<br />三层缓存依赖，其实就是先把实例化的对象，放置在缓存中，等后续在根据A对象的引用完成赋值操作。<br />处理完的流程就是如下所示了：<br />![2021-09-01-14-15-19-206319.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630477502696-53f118da-a316-4dda-89bf-46dc8588a16e.jpeg#clientId=u98fcf163-9073-4&from=ui&id=uc643bb4b&originHeight=389&originWidth=984&originalType=binary&ratio=1&size=37102&status=done&style=shadow&taskId=u666486af-bd62-4c56-85c9-98d0e0d83e9)<br />在改进的图中其实已经可以发现，环 已经被打开了。整个可以如下几步：<br />在实例化A对象之后就向容器中添加一个缓存，存放一个实例化但未初始化完成的对象（半成品对象）。<br />在第一次创建A对象中容器已经有一个A对象，但是没有B对象，所以在开始创建B对象时，在完成B对象的实例化之后，开始初始化属性赋值时，此时容器中已经有A对象，所以可以直接通过A的属性赋值，同样的B对象完成初始化之后也就可以再接着完成初始化A对象了，那整个A对象和B对象的创建过程就完成了。<br />直接看Spring中源码来解析一下：
```java
 * @author Juergen Hoeller
 * @since 2.0
 * @see #registerSingleton
 * @see #registerDisposableBean
 * @see org.springframework.beans.factory.DisposableBean
 * @see org.springframework.beans.factory.config.ConfigurableBeanFactory
 */
public class DefaultSingletonBeanRegistry extends SimpleAliasRegistry implements SingletonBeanRegistry {

     /** Cache of singleton objects: bean name to bean instance. */
     private final Map<String, Object> singletonObjects = new ConcurrentHashMap<>(256);

     /** Cache of singleton factories: bean name to ObjectFactory. */
     private final Map<String, ObjectFactory<?>> singletonFactories = new HashMap<>(16);

     /** Cache of early singleton objects: bean name to bean instance. */
     private final Map<String, Object> earlySingletonObjects = new HashMap<>(16);

      // 省略其他的一些方法。。。
}
```

- 一级缓存：`singletonObjects`
- 二级缓存：`earlySingletonObjects`
- 三级缓存：`singletonFactories`，第三级缓存存放的是ObjectFactory-》FunctionalInterface  即函数式接口

那么Spring中是怎么使用这三级缓存去处理依赖呢？<br />为了搞明白这个过程只能是debug源码了，因为整个过程比较长，没办法做成动图的形式，所以只能给大家一步一步说明了。<br />之前跟大家讲SpringIOC中的有个关键方法`refresh()`，这里面包含了13个核心的子方法。<br />在13个子方法中有一个`finishBeanFactoryInitialization(beanFactory)` ；初始化剩下的单实例（非懒加载的）方法。这个就是开始入口了
```java
public void refresh() throws BeansException, IllegalStateException {
    //   添加一个synchronized 防止出现refresh还没有完成出现其他的操作（启动，或者销毁） 
    synchronized (this.startupShutdownMonitor) {
        // 1.准备工作
        // 记录下容器的启动时间、
        // 标记“已启动”状态，关闭状态为false、
        // 加载当前系统属性到环境对象中
        // 准备一系列监听器以及事件集合对象
        prepareRefresh();

        // 2. 创建容器对象：DefaultListableBeanFactory，加载XML配置文件的属性到当前的工厂中（默认用命名空间来解析），就是上面说的BeanDefinition（bean的定义信息）这里还没有初始化，只是配置信息都提取出来了，（包含里面的value值其实都只是占位符）
        ConfigurableListableBeanFactory beanFactory = obtainFreshBeanFactory();

        // 3. BeanFactory的准备工作，设置BeanFactory的类加载器，添加几个BeanPostProcessor，手动注册几个特殊的bean等
        prepareBeanFactory(beanFactory);
        try {
            // 4.子类的覆盖方法做额外的处理，就是我们刚开始说的 BeanFactoryPostProcessor ，具体的子类可以在这步的时候添加一些特殊的BeanFactoryPostProcessor完成对beanFactory修改或者扩展。
            // 到这里的时候，所有的Bean都加载、注册完成了，但是都还没有初始化
            postProcessBeanFactory(beanFactory);
            // 5.调用 BeanFactoryPostProcessor 各个实现类的 postProcessBeanFactory(factory) 方法
            invokeBeanFactoryPostProcessors(beanFactory);

            // 6.注册 BeanPostProcessor  处理器 这里只是注册功能，真正的调用的是getBean方法
            registerBeanPostProcessors(beanFactory);

            // 7.初始化当前 ApplicationContext 的 MessageSource，即国际化处理
            initMessageSource();

            // 8.初始化当前 ApplicationContext 的事件广播器，
            initApplicationEventMulticaster();

            // 9.从方法名就可以知道，典型的模板方法(钩子方法)
            //  具体的子类可以在这里初始化一些特殊的Bean（在初始化 singleton beans 之前）
            onRefresh();

            // 10.注册事件监听器，监听器需要实现 ApplicationListener 接口。这也不是我们的重点，过
            registerListeners();

            // 11.初始化所有的 singleton beans（lazy-init 的除外），重点关注
            finishBeanFactoryInitialization(beanFactory);

            // 12.广播事件，ApplicationContext 初始化完成
            finishRefresh();
        }
        catch (BeansException ex) {
            if (logger.isWarnEnabled()) {
                logger.warn("Exception encountered during context initialization - " +
                            "cancelling refresh attempt: " + ex);
            }
            // 13.销毁已经初始化的 singleton 的 Beans，以免有些 bean 会一直占用资源
            destroyBeans();
            cancelRefresh(ex);
            // 把异常往外抛
            throw ex;
        }
        finally {
            // Reset common introspection caches in Spring's core, since we
            // might not ever need metadata for singleton beans anymore...
            resetCommonCaches();
        }
    }
}
```
因为IOC作为Spring的容器，且默认的都是单例的，所以在创建bean之前都会去`getBean`一把，判断当前是否有，当没有时才会去创建。<br />所以进入`finishBeanFactoryInitialization`方法中找到 `beanFactory.preInstantiateSingletons();`
```java
protected void finishBeanFactoryInitialization(ConfigurableListableBeanFactory beanFactory) {
    // 省略其他干扰代码（判断逻辑）。。。


    // Instantiate all remaining (non-lazy-init) singletons.
    // 实例化剩下的所有的单例对象（非懒加载的）
    beanFactory.preInstantiateSingletons();
}
```
进入到 `preInstantiateSingletons` 方法中，可以看到通过`beanDefinitionNames`（bean的定义信息）来判断当前需要创建的bean信息，所以开始通过`beanName`循环开始走创建流程。<br />因为是创建的普通的bean实例，所以肯定会走到最下面的`getBean(beanName);`方法中，如下代码所示
```java
@Override
public void preInstantiateSingletons() throws BeansException {
    if (logger.isTraceEnabled()) {
        logger.trace("Pre-instantiating singletons in " + this);
    }

    // Iterate over a copy to allow for init methods which in turn register new bean definitions.
    // While this may not be part of the regular factory bootstrap, it does otherwise work fine.
    List<String> beanNames = new ArrayList<>(this.beanDefinitionNames);

    // Trigger initialization of all non-lazy singleton beans...
    for (String beanName : beanNames) {
        RootBeanDefinition bd = getMergedLocalBeanDefinition(beanName);
        if (!bd.isAbstract() && bd.isSingleton() && !bd.isLazyInit()) {
            // 判断是否是工厂bean
            if (isFactoryBean(beanName)) {
                Object bean = getBean(FACTORY_BEAN_PREFIX + beanName);
                if (bean instanceof FactoryBean) {
                    final FactoryBean<?> factory = (FactoryBean<?>) bean;
                    boolean isEagerInit;
                    if (System.getSecurityManager() != null && factory instanceof SmartFactoryBean) {
                        isEagerInit = AccessController.doPrivileged((PrivilegedAction<Boolean>)
                                                                    ((SmartFactoryBean<?>) factory)::isEagerInit,
                                                                    getAccessControlContext());
                    }
                    else {
                        isEagerInit = (factory instanceof SmartFactoryBean &&
                                       ((SmartFactoryBean<?>) factory).isEagerInit());
                    }
                    if (isEagerInit) {
                        getBean(beanName);
                    }
                }
            }
            else {
                // 如果当前beanName对应的bean不是工厂bean，则通过beanName来获取bean的实例
                getBean(beanName);
            }
        }
    }
}
```
进入到这个`getBean(beanName);`方法中有一个`doGetBean`方法，在Spring源码中真正开始干活做事情的都一定会打上`do`的前缀方法。
```java
@Override
public Object getBean(String name) throws BeansException {
    // 实际获取bean的方法，触发依赖注入方法
    return doGetBean(name, null, null, false);
}
```
所以在进到`doGetBean`的方法中，还是会默认先去获取一把，没有则开始创建进入`createBean(beanName, mbd, args)`方法
```java
protected <T> T doGetBean(final String name, @Nullable final Class<T> requiredType,
                          @Nullable final Object[] args, boolean typeCheckOnly) throws BeansException {

    final String beanName = transformedBeanName(name);
    Object bean;

    // Eagerly check singleton cache for manually registered singletons.
    // 确认一下容器中是否已经有了当前bean实例
    Object sharedInstance = getSingleton(beanName);
    if (sharedInstance != null && args == null) {
        if (logger.isTraceEnabled()) {
            if (isSingletonCurrentlyInCreation(beanName)) {
                logger.trace("Returning eagerly cached instance of singleton bean '" + beanName +
                             "' that is not fully initialized yet - a consequence of a circular reference");
            }
            else {
                logger.trace("Returning cached instance of singleton bean '" + beanName + "'");
            }
        }
        bean = getObjectForBeanInstance(sharedInstance, name, beanName, null);
    }

    // 省略其他逻辑代码。。。

    // Create bean instance.
    // 创建Bean的实例对象
    if (mbd.isSingleton()) {
        // 返回以beanName的单例对象，如果没有注册，则使用singletonFactory创建并且注册一个。
        sharedInstance = getSingleton(beanName, () -> {
            try {
                // 为给定的BeanDefinition（和参数）创建一个Bean的实例 重点
                return createBean(beanName, mbd, args);
            }
            catch (BeansException ex) {
                // Explicitly remove instance from singleton cache: It might have been put there
                // eagerly by the creation process, to allow for circular reference resolution.
                // Also remove any beans that received a temporary reference to the bean.
                destroySingleton(beanName);
                throw ex;
            }
        });
        bean = getObjectForBeanInstance(sharedInstance, name, beanName, mbd);
    }

    // 省略其他逻辑代码。。。
    return (T) bean;
}
```
在上面没有获取到bean时候则开始创建bean了，所以直接进到`createBean`的方法中，因为是容器初始化启动所以肯定是没有的，顾一定会进入`createBean`的方法中，所以再进入`createBean`的方法中。
```java
@Override
protected Object createBean(String beanName, RootBeanDefinition mbd, @Nullable Object[] args)
    throws BeanCreationException {
    // 省略其他相关代码。。。。。

    try {
        // 实际创建Bean的调用 重点
        Object beanInstance = doCreateBean(beanName, mbdToUse, args);
        if (logger.isTraceEnabled()) {
            logger.trace("Finished creating instance of bean '" + beanName + "'");
        }
        return beanInstance;
    }
    catch (BeanCreationException | ImplicitlyAppearedSingletonException ex) {
        // A previously detected exception with proper bean creation context already,
        // or illegal singleton state to be communicated up to DefaultSingletonBeanRegistry.
        throw ex;
    }
    catch (Throwable ex) {
        throw new BeanCreationException(
            mbdToUse.getResourceDescription(), beanName, "Unexpected exception during bean creation", ex);
    }
}
```
看到`doCreateBean`方法那说明要开始真正的创建Bean了。
```java
protected Object doCreateBean(final String beanName, final RootBeanDefinition mbd, final @Nullable Object[] args)
    throws BeanCreationException {

    // Instantiate the bean.
    BeanWrapper instanceWrapper = null;
    if (mbd.isSingleton()) {
        // 判断如果是单例对象，则从factoryBean实例缓存汇总移除当前Bean的定义信息
        instanceWrapper = this.factoryBeanInstanceCache.remove(beanName);
    }
    if (instanceWrapper == null) {
        // 根据执行的bean使用的对应的策略创建新的实例。也可以理解实例化对象，在内存总开辟空间
        instanceWrapper = createBeanInstance(beanName, mbd, args);
    }
    final Object bean = instanceWrapper.getWrappedInstance();
    Class<?> beanType = instanceWrapper.getWrappedClass();
    if (beanType != NullBean.class) {
        mbd.resolvedTargetType = beanType;
    }

    // 省略其他的相关代码。。。。。

    // Eagerly cache singletons to be able to resolve circular references
    // even when triggered by lifecycle interfaces like BeanFactoryAware.
    // 判断当前bean是否需要提前曝光，单例&允许循环依赖&当前bean正在创建，检测循环依赖
    boolean earlySingletonExposure = (mbd.isSingleton() && this.allowCircularReferences &&
                                      isSingletonCurrentlyInCreation(beanName));
    if (earlySingletonExposure) {
        if (logger.isTraceEnabled()) {
            logger.trace("Eagerly caching bean '" + beanName +
                         "' to allow for resolving potential circular references");
        }
        // 在bean的初始化完成之前将创建的实例加入ObjectFactory（添加三级缓存），主要是为了防止后期的循环依赖。。。。重点
        addSingletonFactory(beanName, () -> getEarlyBeanReference(beanName, mbd, bean));
    }

    Object exposedObject = bean;
    try {
        // 填充bean属性，假设其中存在依赖于其他的bean的属性，则会递归初始化依赖的bean
        populateBean(beanName, mbd, instanceWrapper);
        //执行初始化逻辑
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

    return exposedObject;
}
```
进入到`doCreateBean`中首先需要核心看的一个方法`createBeanInstance`，这个方法就是真正的创建bean实例例，也就是在内存中开辟空间（实例化），完事之后就开始看第二个重点添加缓存
```java
addSingletonFactory(beanName, () -> getEarlyBeanReference(beanName, mbd, bean));
```
这个方法点进去，其实就是能发现开始添加到第三级缓存中，value值就是一个函数方法`getEarlyBeanReference`，不熟悉的同学可以看下JDK1.8的新特性。同时也标注了当前bean正在注册中。<br />实例化完bean按照bean的生命周期流程那肯定就是开始初始化bean了，填充属性，接着向下看有一个`populateBean`（填充bean属性）
```java
populateBean(beanName, mbd, instanceWrapper);
```
在`populateBean`这个过程中就有很大的逻辑在里面了，比如说获取属性名称，属性值等等一系列操作。但是核心的还是需要看`applyPropertyValues`方法属性赋值，如下所示：
```java
protected void populateBean(String beanName, RootBeanDefinition mbd, @Nullable BeanWrapper bw) {
    // 省略一堆其他判断校验逻辑代码，直接看到最后。。。
    if (pvs != null) {
        // 应用给定的属性值，解决任何在这个bean工厂运行时其他的bean的调用（就是设置属性值）
        applyPropertyValues(beanName, mbd, bw, pvs);
    }
}
```
同样的进入`applyPropertyValues`方法。
```java
protected void applyPropertyValues(String beanName, BeanDefinition mbd, BeanWrapper bw, PropertyValues pvs) {
    // 省略其他的一些校验代码。。。。。

    // Create a deep copy, resolving any references for values.
    List<PropertyValue> deepCopy = new ArrayList<>(original.size());
    boolean resolveNecessary = false;
    // 便利属性，将属性转换为对应类的对应属性类型
    for (PropertyValue pv : original) {
        // 判断当前属性是否已经解析过
        if (pv.isConverted()) {
            deepCopy.add(pv);
        }
        else {
            // 获取属性明层
            String propertyName = pv.getName();
            // 获取属性值
            Object originalValue = pv.getValue();
            // valueResolver处理pv解析出的originalValue封装的对象（是否必要开始去处理属性值了）重点
            Object resolvedValue = valueResolver.resolveValueIfNecessary(pv, originalValue);
            // 默认转换后的值等于解析出来的值   
            Object convertedValue = resolvedValue;
            // 判断转换标记 
            boolean convertible = bw.isWritableProperty(propertyName) &&
                !PropertyAccessorUtils.isNestedOrIndexedProperty(propertyName);
            if (convertible) {
                convertedValue = convertForProperty(resolvedValue, propertyName, bw, converter);
            }
            // 省略其他的代码逻辑。。。。
        }
    }
}
```
`applyPropertyValues`方法中需要注意的是`valueResolver.resolveValueIfNecessary`值处理器
```java
Object resolvedValue = valueResolver.resolveValueIfNecessary(pv, originalValue);
```
这一步主要是判断属性值是否需要处理，因为之前这个value值是存方法接口方法<br />![2021-09-01-14-15-19-315324.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630477518420-28fa3bb9-bbb5-4d42-9969-8ff217a1787c.jpeg#clientId=u98fcf163-9073-4&from=ui&id=u52f6f42a&originHeight=291&originWidth=1080&originalType=binary&ratio=1&size=44351&status=done&style=shadow&taskId=u7d54a45c-cf7c-4137-bb80-cda0e4bc1c4)<br />所以在执行`valueResolver.resolveValueIfNecessary`方法时，一定会去处理，那再看看里面又处理什么逻辑？
```java
public Object resolveValueIfNecessary(Object argName, @Nullable Object value) {
    // We must check each value to see whether it requires a runtime reference
    // to another bean to be resolved.
    // 如果value是RuntimeBeanReference实例 则处理
    if (value instanceof RuntimeBeanReference) {
        RuntimeBeanReference ref = (RuntimeBeanReference) value;
        // 解析出对应的ref 所封装的Bean元信息（Bean的名称，Bean的类型） 的对象
        return resolveReference(argName, ref);
    }
    // 省略其他的逻辑代码
}

```
里面的断点的截图已经可以明确的看到value值是`RuntimeBeanReference`实例，所以接下来就一定会去调用`resolveReference`方法解析ref所封装的bean信息,那就再接着进入`resolveReference`方法看看干了什么？
```java
@Nullable
private Object resolveReference(Object argName, RuntimeBeanReference ref) {
    try {
        Object bean;
        String refName = ref.getBeanName();
        refName = String.valueOf(doEvaluate(refName));
        if (ref.isToParent()) {
            if (this.beanFactory.getParentBeanFactory() == null) {
                throw new BeanCreationException(
                    this.beanDefinition.getResourceDescription(), this.beanName,
                    "Can't resolve reference to bean '" + refName +
                    "' in parent factory: no parent factory available");
            }
            bean = this.beanFactory.getParentBeanFactory().getBean(refName);
        }
        else {
            // 获取resolvedName的Bean对象 重点
            bean = this.beanFactory.getBean(refName);
            // 注册beanName到dependentBeanName的依赖关系到Bean的工中
            this.beanFactory.registerDependentBean(refName, this.beanName);
        }
        if (bean instanceof NullBean) {
            bean = null;
        }
        // 返回解析出来的对用的ref所封装的Bean对象
        return bean;
    }
    catch (BeansException ex) {
        throw new BeanCreationException(
            this.beanDefinition.getResourceDescription(), this.beanName,
            "Cannot resolve reference to bean '" + ref.getBeanName() + "' while setting " + argName, ex);
    }
}
```
上面已经进入到`resolveReference`来处理ref中所以引用的Bean对象，又因为SpringIOC默认都是单例Bean，所以肯定还是在`beanFactory`中去获取Bean
```java
bean = this.beanFactory.getBean(refName);
```
至此又开始循环创建循环依赖的对象，假设还是一开始的A和B两个对象来说，那么开始是创建A对象时，在设置B属性的时候，没有B属性，那么现在刚好就是开始创建B属性了。同样的B对象又开始填充属性A。<br />细心的同学应发现问题了，这不就是无限循环了吗？还怎么处理循环啊？这不是扯淡吗？<br />其实不是的，其实创建B对象想的时候，去获取A的Bean信息时，因为A还是在创建中，所以在接下来中从新走流程中会有一个新的发现，进入缓存中获取对象，如下
```java
bean = this.beanFactory.getBean(refName) ->  doGetBean(name, null, null, false) -> sharedInstance = getSingleton(beanName) -> getSingleton(beanName, true) 
    // 具体点 getSingleton 方法的内部实现
    // 进入getSingleton方法中 isSingletonCurrentlyInCreation 当前的Bean正在创建中
    protected Object getSingleton(String beanName, boolean allowEarlyReference) {
    // 从一级缓存获取BeanName对应的单例对象
    Object singletonObject = this.singletonObjects.get(beanName);
    // 如果没有获取到，但是当前 BeanName对应的单例对象又处于创建中
    if (singletonObject == null && isSingletonCurrentlyInCreation(beanName)) {
        synchronized (this.singletonObjects) {
            // 从二级缓存中获取当前BeanName对应的单例对象
            singletonObject = this.earlySingletonObjects.get(beanName);
            // 二级缓存中没有，但是allowEarlyReference为true，在doCreateBean方法中已经设置，所以这里为true
            if (singletonObject == null && allowEarlyReference) {
                // 从三级缓存中获取
                ObjectFactory<?> singletonFactory = this.singletonFactories.get(beanName);
                if (singletonFactory != null) {
                    // 这里就是三级缓存函数方法，同过Factory创建一个单例对象
                    singletonObject = singletonFactory.getObject();
                    // 添加到二级缓存中，半成品对象
                    this.earlySingletonObjects.put(beanName, singletonObject);
                    // 同时删除三级缓存
                    this.singletonFactories.remove(beanName);
                }
            }
        }
    }
    // 返回当前半成品对象
    return singletonObject;
}
```
现在整个流程中二级缓存已经存放了一个半成品A的对象，因此在创建B对象时，获取A属性填充值从容器缓存中已经可以获取到A对象的单例Bean，对B对象来说其实就是一个完整的单例Bean实例,因此再次getSingleton Bean时候会有一个判断，如果有一个新的完成的单例Bean则会添加到一级缓存中，源码如下：
```java
public Object getSingleton(String beanName, ObjectFactory<?> singletonFactory) {
    Assert.notNull(beanName, "Bean name must not be null");
    synchronized (this.singletonObjects) {
        Object singletonObject = this.singletonObjects.get(beanName);
        if (singletonObject == null) {
            if (this.singletonsCurrentlyInDestruction) {
                throw new BeanCreationNotAllowedException(beanName,
                                                          "Singleton bean creation not allowed while singletons of this factory are in destruction " +
                                                          "(Do not request a bean from a BeanFactory in a destroy method implementation!)");
            }
            if (logger.isDebugEnabled()) {
                logger.debug("Creating shared instance of singleton bean '" + beanName + "'");
            }
            beforeSingletonCreation(beanName);
            boolean newSingleton = false;
            boolean recordSuppressedExceptions = (this.suppressedExceptions == null);
            if (recordSuppressedExceptions) {
                this.suppressedExceptions = new LinkedHashSet<>();
            }
            try {
                singletonObject = singletonFactory.getObject();
                newSingleton = true;
            }
            // 省略其他的代码逻辑

            //判断生成了新的单例对象
            if (newSingleton) {
                // 将添加BeanName和 singletonObject 添加到一级缓存中去
                addSingleton(beanName, singletonObject);
            }
        }
        return singletonObject;
    }
}
```
上面聊到当新的单例对象生成会再调用`addSingleton`方法
```java
protected void addSingleton(String beanName, Object singletonObject) {
    synchronized (this.singletonObjects) {
        // 添加到一级缓存中
        this.singletonObjects.put(beanName, singletonObject);
        // 移除二级缓存中的内容
        this.singletonFactories.remove(beanName);
        // 移除三级缓存中的内容
        this.earlySingletonObjects.remove(beanName);
        // 将完成的BeanName添加到已经注册的单例集合中
        this.registeredSingletons.add(beanName);
    }
}
```
自此整个Spring的循环依赖过程就已经结束了。<br />还是用开始的A，B两个对象来总结一个流程吧<br />当开始创建A对象时，实例化后，添加一步三级缓存，针对属性赋值，因为此时还没有B对象的实例，所以在获取到A对象的B属性的值的ref引用对象B，触发创建B对象的创建，因此在B对象实例化后，在属性赋值时，获取到A属性的ref引用对象，而因为之前A对象已经完成实例化，并且添加到了三级缓存中，所以在B属性创建设置A属性时，因为此时A属性正在被创建，所以可以从第三级缓存中获取到值，同时把获取到的值添加到二级缓存中，同时删除第三级缓存的A对象。<br />在创建B对象中已经能获取到A属性值（半成品），所以B对象可以完成赋值状态，变成一个完整的B对象的实例。所以当新的单例对象生成会再调用`addSingleton`方法添加到一级缓存中，同时删除 二级 三级缓存的值，所以回过头来接着 A对象获取B属性值的时候已经能在一级缓存中获取到。所以也就可以完成属性赋值，自此循环依赖完全打开。<br />循环依赖问题已经完了，在看源码的过程中一定要注意以下的6个方法：<br />![2021-09-01-14-15-19-470343.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630477531802-a0ebb1f7-e8fd-471a-b6cf-1eed12086734.jpeg#clientId=u98fcf163-9073-4&from=ui&id=u5fe085d1&originHeight=157&originWidth=517&originalType=binary&ratio=1&size=10861&status=done&style=shadow&taskId=u308cf0d5-ea08-4087-a0e9-f2f3082e1e8)<br />这六个方法是核心处理流程，按照这个流程，以及上面执行的步骤一步一步断点多走几遍就能加深自己的理解了。
<a name="LfS33"></a>
### 总结
一级二级 三级缓存中分别存放的是什么状态的对象？

- 一级：完整的成品的对象
- 二级：非完整的半成品对象
- 三级：lambada表达式

假设只设计二级缓存能否解决循环依赖？<br />只用二级缓存是可以解决缓存依赖的，（废弃第三级，保留第一第二）但是会有一个问题，在配置AOP切面的时候会出错，因为无法生成代理对象。<br />所以三级缓存是为了处理AOP中的循环依赖。因为当配置了切面之后，在`getEarlyBeanReference`方法中，有可能会把之前的原始对象替换成代理对象，导致Bean的版本不是最终的版本，所以报错。
