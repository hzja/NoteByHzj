JavaSpring
<a name="kRRWH"></a>
## 循环依赖
什么是循环依赖，说到循环依赖，这个实际上是没有那么复杂的，就比如很简单的说，A 引用了 B ，而这个时候 B 也引用了 A ，那么这种情况实际上就是出现了循环依赖的问题了，实际上也可以把循环依赖称之为循环引用，两个或者两个以上的bean互相持有对方，最终形成闭环。<br />这就是循环依赖，也就是循环引用，<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662684616365-6ddd88be-a2ed-48ed-bc17-95c7c29afe19.png#clientId=u53e4cbab-89da-4&from=paste&id=u1cdd22ab&originHeight=357&originWidth=496&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uffcd7eab-8fb1-4fae-8751-9153fa34af1&title=)<br />注意，这里不是函数的循环调用，是对象的相互依赖关系。循环调用其实就是一个死循环，除非有终结条件。否则的话，他就是一个死循环.
<a name="wfp7A"></a>
## Spring 中的循环依赖
那么 Spring 的循环依赖都有什么呢？

- 构造器的循环依赖
- field属性的循环依赖

那么针对这两种循环依赖，Spring 它是如何解决的呢？这就很特殊了，构造器的循环依赖问题实际上算是个无解的操作，只能拋出 BeanCurrentlyInCreationException 异常，也就是说，这个构造器导致的循环依赖，Spring 是没有办法来处理的，也只是给抛出了异常，但是对于 字段属性 的循环依赖，还是有解决办法的。
<a name="dINcw"></a>
## Spring怎么解决循环依赖
这个时候，就得看看 Spring 的对象初始化的过程了，<br />Spring的单例对象的初始化主要分为三步：

- `createBeanInstance` 实例化
- `populateBean` 填充属性
- `initializeBean` 初始化

`createBeanInstance` 实例化实际上就是调用对象的构造方法实例化对象，populateBean 实际上就是对 bean 的依赖属性进行一个赋值填充，而 `initializeBean` 则是调用 Spring xml 中的 `init` 方法。<br />这个时候，看到这个初始化的过程，一般就应该能猜到会发生 循环依赖 的位置是哪一步了，而单从 bean 的初始化来看，循环依赖发生的位置就是在 `createBeanInstance` 实例化 以及 `populateBean` 填充属性 当中，<br />发生的循环依赖也是

- 构造器的循环依赖
- field属性的循环依赖

那么 Spring 又是怎么解决这种单例的循环依赖的问题的呢？
<a name="GfVLX"></a>
### 三级缓存
那么这三级缓存分别是哪三级的缓存呢？又分别代表了什么含义？

- `singletonFactories `：单例对象工厂的cache，用于存放完全初始化好的 bean，从该缓存中取出的 bean 可以直接使用
- `earlySingletonObjects `：提前暴光的单例对象的Cache，存放原始的 bean 对象（尚未填充属性），用于解决循环依赖
- `singletonObjects`：单例对象的cache，存放 bean 工厂对象，用于解决循环依赖
```java
private final Map<String, Object> singletonObjects = new ConcurrentHashMap<>(256); //一级缓存
private final Map<String, Object> earlySingletonObjects = new HashMap<>(16); // 二级缓存
private final Map<String, ObjectFactory<?>> singletonFactories = new HashMap<>(16); // 三级缓存
```
如果要分析这个 三级缓存 如何解决循环依赖，那么势必需要知道 Spring 中对象的创建的过程。<br />对象创建过程，可以大致分为五个步骤，

1. `protected <T> T doGetBean(final String name, @Nullable final Class<T> requiredType, @Nullable final Object[] args, boolean typeCheckOnly)`

`AbstractBeanFactory` 中的 `doGetBean()`方法

2. `protected Object getSingleton(String beanName, boolean allowEarlyReference)`

`DefaultSingletonBeanRegistry` 中的 `getSingleton()`方法

- 在这个方法中，先从一级缓存 `singletonObjects` 中去获取。（如果获取到就直接`return`）
- 如果获取不到，并且对象正在创建中，就再从二级缓存 `earlySingletonObjects` 中获取。
- 如果还是获取不到且允许 `singletonFactories` 通过 `getObject()` 获取，就从三级缓存`singletonFactory.getObject()`(三级缓存)获取
- 如果获取到了则：从 `singletonFactories` 中移除，并放入 `earlySingletonObjects` 中
- 这就相当于 ctrl+x，把三级缓存中的数据剪切到了二级缓存。

源码如下：
```java
protected Object getSingleton(String beanName, boolean allowEarlyReference) {
    Object singletonObject = this.singletonObjects.get(beanName);
    if (singletonObject == null && isSingletonCurrentlyInCreation(beanName)) {
        synchronized (this.singletonObjects) {
            singletonObject = this.earlySingletonObjects.get(beanName);
            if (singletonObject == null && allowEarlyReference) {
                ObjectFactory<?> singletonFactory = this.singletonFactories.get(beanName);
                if (singletonFactory != null) {
                    singletonObject = singletonFactory.getObject();
                    this.earlySingletonObjects.put(beanName, singletonObject);
                    this.singletonFactories.remove(beanName);
                }
            }
        }
    }
    return singletonObject;
}
```

3. `protected Object createBean(String beanName, RootBeanDefinition mbd, @Nullable Object[] args)`

`AbstractAutowireCapableBeanFactory` 中的 `doCreateBean()` 方法
```java
//添加到三级缓存
if (earlySingletonExposure) {
    if (logger.isTraceEnabled()) {
        logger.trace("Eagerly caching bean '" + beanName +
                     "' to allow for resolving potential circular references");
    }
    addSingletonFactory(beanName, () -> getEarlyBeanReference(beanName, mbd, bean));
}
```

4. `protected void populateBean(String beanName, RootBeanDefinition mbd, @Nullable BeanWrapper bw)`

`AbstractAutowireCapableBeanFactory` 中的 `populateBean()` 方法进行属性赋值

5. `protected Object initializeBean(final String beanName, final Object bean, @Nullable RootBeanDefinition mbd)`

`AbstractAutowireCapableBeanFactory` 中的 `initializeBean()` 初始化对象<br />源码部分就不再往上贴那么多了，大家找源码肯定很简单，内部也有具体方法的注释，<br />Spring 解决循环依赖的诀窍就在于 `singletonFactories` 这个三级cache。<br />这个 cache 的类型是 `ObjectFactory`。这里就是解决循环依赖的关键，发生在`createBeanInstance`之后，也就是说单例对象此时已经被创建出来(调用了构造器)。<br />这个对象已经被生产出来了，虽然还不完美（还没有进行初始化的第二步和第三步），但是已经能被人认出来了（根据对象引用能定位到堆中的对象），所以Spring此时将这个对象提前曝光出来让大家认识，让大家使用。<br />如果能在面试的时候，回答成这个样子，那么这个问题，至少已经算是回答的比较好了。<br />但是如果问到这里，面试官有意想要继续深挖一下，既然知道使用三级缓存解决了这个循环依赖的问题了，那么是不是必须三级缓存才能解决，二级缓存不能解决吗？<br />这就另外又引申出一个问题来了，二级缓存到底能不能解决呢？<br />其实，二级缓存也是能够实现的，如果自己想要实现，那么就得去改写 `AbstractAutowireCapableBeanFactory` 的 `doCreateBean` 的方法了，
```java
//添加到三级缓存
if (earlySingletonExposure) {
    if (logger.isTraceEnabled()) {
        logger.trace("Eagerly caching bean '" + beanName +
                     "' to allow for resolving potential circular references");
    }
    addSingletonFactory(beanName, () -> getEarlyBeanReference(beanName, mbd, bean));
    //从三级缓存中取出立刻放入二级缓存
    getSingleton(beanName, true);
}
```
如果要使用二级缓存解决循环依赖，意味着Bean在构造完后就创建代理对象，这样违背了Spring设计原则。<br />Spring结合AOP跟Bean的生命周期，是在Bean创建完全之后通过`AnnotationAwareAspectJAutoProxyCreator`这个后置处理器来完成的，在这个后置处理的`postProcessAfterInitialization`方法中对初始化后的Bean完成AOP代理。<br />如果出现了循环依赖，那没有办法，只有给Bean先创建代理，但是没有出现循环依赖的情况下，设计之初就是让Bean在生命周期的最后一步完成代理而不是在实例化后就立马完成代理。<br />所以，知道为什么不使用二级缓存直接来处理了，而是增加了三级缓存来处理这个循环依赖了吧！
