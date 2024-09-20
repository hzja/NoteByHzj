Java Spring
<a name="bLZD8"></a>
## 一、什么是循环依赖
就是有两个服务，A服务，B服务，然后在A里注入了B，然后在B里注入了A，这就是循环依赖了，这种情况如果不解决的话，那就会出现一个相互依赖注入的死循环。
<a name="vuqyR"></a>
## 二、循环依赖的解决方案 - 三级缓存
<a name="zxQFI"></a>
### 2.1 什么是三级缓存
```java
/** 一级缓存 单例缓存池 用于保存我们所有的单实例bean */
private final Map<String, Object> singletonObjects = new ConcurrentHashMap<>(256);

/** 二级缓存 保存半成品bean实例，当对象需要被AOP切面代时，保存代理bean的实例beanProxy*/
private final Map<String, Object> earlySingletonObjects = new ConcurrentHashMap<>(16);

/** 三级缓存 存放ObjectFactory，传入的是匿名内部类，ObjectFactory.getObject() 方法最终会
      调用getEarlyBeanReference()进行处理，返回创建bean实例化的lambda表达式。*/
private final Map<String, ObjectFactory<?>> singletonFactories = new HashMap<>(16);
```
<a name="U18Bb"></a>
### 2.2 三级缓存如何解决循环依赖的问题
前置知识：Spring的单例对象的初始化主要分为三步：<br />（1）`createBeanInstance`：实例化，其实也就是调用对象的构造方法实例化对象<br />（2）`populateBean`：填充属性，这一步主要是多bean的依赖属性进行填充<br />（3）`initializeBean`：调用spring xml中的init 方法。
<a name="npHhA"></a>
#### 2.2.1 重点方法
循环依赖涉及的重点方法是`DefaultSingletonBeanRegistry#getSingleton(java.lang.String, boolean)`
```java
//一个纯bean获取流程，这里不进行创建
protected Object getSingleton(String beanName, boolean allowEarlyReference) {
    // 从一级缓存singletonObjects获取bean
    Object singletonObject = this.singletonObjects.get(beanName);
    // 一级缓存没有，判断该bean是否在创建中，通过Set的contains来判断
    if (singletonObject == null && isSingletonCurrentlyInCreation(beanName)) {
        synchronized (this.singletonObjects) {
            // 从二级缓存中获取bean
            singletonObject = this.earlySingletonObjects.get(beanName);
            // 二级缓存没有&&允许提前引用
            if (singletonObject == null && allowEarlyReference) {
                // 从三级缓存中获取lambda表达式
                ObjectFactory<?> singletonFactory = this.singletonFactories.get(beanName);
               // 如果获取到该lambda表达式，进行回调填充
                if (singletonFactory != null) {
                    // 调用三级缓存的lambda表示获取早期不完整对象
                    singletonObject = singletonFactory.getObject();
                    // 写入二级缓存
                    this.earlySingletonObjects.put(beanName, singletonObject);
                    // 三级缓存移除该bean的lambda表达式
                    this.singletonFactories.remove(beanName);
                }
            }
        }
    }
    return singletonObject;
}

public boolean isSingletonCurrentlyInCreation(String beanName) {
    return this.singletonsCurrentlyInCreation.contains(beanName);
}
```
<a name="SqM3W"></a>
#### 2.2.2 提前透知下AB存在循环依赖的情况大概是怎样一个加载流程，源码解读放下面
现在来分析一下A B循环依赖的情况

- A `doCreateBean()`实例化，由于还未创建，从一级缓存查不到，且不是正在创建，这时候调用bean创建流程，将不完整对象以及`BeanDefinition`代表的lambda表达式写入三级缓存`singletonFactories`;
- A `populateBean()`进行属性注入时候发现自己需要B对象，但是三级缓存中未发现B，就去创建B同样把创建B bean的lambda表达式放入`singletonFactories`;
- B `populateBean()` 发现自己需要A对象，从一级缓存`singletonObjects`和二级缓存`earlySingletonObjects`中未发现A，但是在三级缓存singletonFactories中发现A，执行`singletonFactories` 里A的回调函数`getEarlyBeanReference`(),创建不完整的A bean，将其放入二级缓存`earlySingletonObjects`，同时从三级缓存删除；
- 将A注入到对象B中，B完成属性填充，执行初始化方法，将自己放入第一级缓存中（此时B是一个完整的对象）；
- 返回，A得到对象B，将B注入到A中，A完成属性填充，初始化，并放入到一级缓存中。
- 在创建过程中，都是从三级缓存(对象工厂里创建不完整对象)，将提前暴露的对象放入到二级缓存，从二级缓存拿到后，完成初始化，放入一级缓存。
<a name="iGMja"></a>
## 三、原码分析
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639442852164-8fe60e5f-c38c-45ef-aa9e-ce0a6aa83ec1.webp#clientId=u7fa7d4ef-f290-4&from=paste&id=u77aca1ff&originHeight=396&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u23c7eec3-64d1-477e-91fd-abd3488e937&title=)
<a name="hsNfo"></a>
### 3.1. BeanServiceA的创建:
在创建bean时，会调用`doGetBean`方法，首先通过`getSingleton`方法从缓存中看是否能获取到该bean

- a. 先从一级缓存`singletonObjects`中获取，发现获取不到，然后看是否在创建中，显然初次创建时不成立，即`getSingleton`返回null
- b. 调用第14行的`getSingleton`方法触发`createBean`回调，进行bean的生命周期
```java
    protected <T> T doGetBean(
            String name, @Nullable Class<T> requiredType, @Nullable Object[] args, boolean typeCheckOnly)
            throws BeansException {
        ... ... ...
        //先从一级缓存singletonObjects中获取，发现获取不到，然后看是否在创建中，显然初次创建时不成立，即getSingleton返回null
        Object sharedInstance = getSingleton(beanName);
        if (sharedInstance != null && args == null) {
            ... ... ... 
        }else {
            ... ... ...
            try {
                ... ... ...
                if (mbd.isSingleton()) {
                    //getSingleton方法触发createBean回调，进行bean的生命周期  
                    //这里会将当前beanName放入singletonsCurrentlyInCreation，表示当前bean正在创建
                    sharedInstance = getSingleton(beanName, () -> {
                        try {
                            return createBean(beanName, mbd, args);
                        }
            ... ... ...
        }
        return (T) bean;
    }

public Object getSingleton(String beanName, ObjectFactory<?> singletonFactory) {
        Assert.notNull(beanName, "Bean name must not be null");
        synchronized (this.singletonObjects) {
            Object singletonObject = this.singletonObjects.get(beanName);
            if (singletonObject == null) {
                ... ... ...
                // 会将当前beanName放入singletonsCurrentlyInCreation，表示当前bean正在创建
                beforeSingletonCreation(beanName);
                ... ... ...
                try {
                    // lambda表达式回到createBean
                    singletonObject = singletonFactory.getObject();
                    newSingleton = true;
                }
            ... ... ... 
            }
            return singletonObject;
        }
    }
```
<a name="VIaYG"></a>
#### 3.1.1. 实例化BeanServiceA不完整对象:

- 一般通过`createBeanInstancec`实例化不完整的`BeanServiceA`对象
- 将不完整对象以及`BeanDefinition`代表的lambda表达式写入三级缓存
- 属性填充BeanServiceB
- 初始化BeanServiceA时调用AOP后置处理器进行AOP处理
- 处理提前暴露的场景，保证返回同一个代理对象
```java
protected Object doCreateBean(String beanName, RootBeanDefinition mbd, @Nullable Object[] args)
        throws BeanCreationException {

    ... ... ...
    if (instanceWrapper == null) {
        // a. 一般通过createBeanInstancec实例化不完整的BeanServiceA对象
        instanceWrapper = createBeanInstance(beanName, mbd, args);
    }
    ... ... ...
    // 默认单例&&默认循环引用&&该bean正在创建，条件成立
    boolean earlySingletonExposure = (mbd.isSingleton() && this.allowCircularReferences &&
            isSingletonCurrentlyInCreation(beanName));
    if (earlySingletonExposure) {
        ... ... ...
        // b. 将不完整对象以及BeanDefinition代表的lambda表达式写入三级缓存
        addSingletonFactory(beanName, () -> getEarlyBeanReference(beanName, mbd, bean));
    }

    // Initialize the bean instance.
    Object exposedObject = bean;
    try {
        // c. 属性填充BeanServiceB
        populateBean(beanName, mbd, instanceWrapper);
        // d.初始化时调用AOP后置处理器进行AOP处理
        exposedObject = initializeBean(beanName, exposedObject, mbd);
    }
    ... ... ...
    // e.处理提前暴露的场景，保证返回同一个代理对象
    if (earlySingletonExposure) {
        // 见上文第1节的节速，由于第二个参数是false，所以只会查到第二季缓存
        // 所以这里就是查看第二级缓存能不能取到值，取到就意味着涉及提前AOP
        Object earlySingletonReference = getSingleton(beanName, false);
        // 涉及提前AOP，从二级缓存中获取提前AOP的代理对象
        if (earlySingletonReference != null) {
            if (exposedObject == bean) {
                // 保证循环依赖且涉及AOP时，返回同一个代理对象，下文有结束
                exposedObject = earlySingletonReference;
            }
            ... ... ...
        }
    }
    return exposedObject;
}
```
上面 `addSingletonFactory`将不完整对象以及`BeanDefinition`代表的lambda表达式写入三级缓存
```java
protected void addSingletonFactory(String beanName, ObjectFactory<?> singletonFactory) {
    Assert.notNull(singletonFactory, "Singleton factory must not be null");
    synchronized (this.singletonObjects) {
        if (!this.singletonObjects.containsKey(beanName)) {
            // 将不完整对象以及BeanDefinition代表的lambda表达式写入三级缓存
            this.singletonFactories.put(beanName, singletonFactory);
            this.earlySingletonObjects.remove(beanName);
            this.registeredSingletons.add(beanName);
        }
    }
}
```
<a name="xFDwi"></a>
#### 3.1.2. 属性填充BeanServiceB:
关于属性填充方法的详细介绍可参考链接：属性填充`populateBean`<br />当填充BeanServiceB会重复上文第1节中的内容：

- a. 先从一级缓存`singletonObjects`中获取，发现获取不到，然后看是否在创建中，显然初次创建时不成立，即getSingleton返回null
- b. 调用第14行的`getSingleton`方法触发`createBean`回调，进行bean的生命周期
- c. 实例化BeanServiceB的不完整对象，并将lambda写入三级缓存
- d. 属性填充BeanServiceA（见下文1.1.2.1）
- e. `initializeBean`初始化BeanServiceB
<a name="Y8ojJ"></a>
##### 3.1.2.1. 上面的d，循环依赖处理属性填充BeanServiceA:
同样，会重复上文第1节的内容，但此时会有不一样的处理：

- a. 先从一级缓存`singletonObjects`中获取，发现获取不到，然后看是否在创建中,显然正在创建，`singletonsCurrentlyInCreation`有beanServiceA
- b. 从二级缓存中获取 → 获取不到 → 从三级缓存获取 → lambda表达式回调（见下文1.1.2.1.1）
- c. 将不完整对象BeanServiceA写入二级缓存，三级缓存删除该对象lambda表达式（上文1中getSingleton方法）
- d. 返回不完整的BeanServiceA对象
<a name="Xxud0"></a>
###### 3.1.1.2.1.1. `AbstractAutoProxyCreator#getEarlyBeanReference`:
这里涉及Aop
```java
protected Object getEarlyBeanReference(String beanName, RootBeanDefinition mbd, Object bean) {
    Object exposedObject = bean;
    if (!mbd.isSynthetic() && hasInstantiationAwareBeanPostProcessors()) {
        //遍历后置处理器
        for (BeanPostProcessor bp : getBeanPostProcessors()) {
            if (bp instanceof SmartInstantiationAwareBeanPostProcessor) {
                SmartInstantiationAwareBeanPostProcessor ibp = (SmartInstantiationAwareBeanPostProcessor) bp;
                // 调用后置处理器的getEarlyBeanReference进行提前暴露bean
                exposedObject = ibp.getEarlyBeanReference(exposedObject, beanName);
            }
        }
    }
    return exposedObject;
}
```
lambda回调会调用后置处理器的`getEarlyBeanReference`，来获取不完整的bean

- 如果当前依赖的bean不涉及Aop，则返回实例化时创建的不完整bean对象
- 如果当前依赖的bean涉及Aop，则返回一个代理该不完整bean的代理对象
- 本处返回实例化创建的不完整beanServiceA对象
```java
public Object getEarlyBeanReference(Object bean, String beanName) {
    Object cacheKey = getCacheKey(bean.getClass(), beanName);
    // 写入earlyProxyReferences，在后面postProcessAfterInitialization会用到
    this.earlyProxyReferences.put(cacheKey, bean);
    // 判断是否创建代理对象
    return wrapIfNecessary(bean, beanName, cacheKey);
}
```
<a name="mN9u2"></a>
##### 3.1.2.2 初始化BeanServiceB时调用AOP后置处理器进行AOP处理
属性填充beanServiceA后，此时填充的beanServiceA是不完整的对象,<br />在`initializeBean`初始化调用后置处理器的`postProcessAfterInitialization`方法<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1639443633434-7d8e20c4-2e37-4818-b04e-ca08416fd19f.png#clientId=u7fa7d4ef-f290-4&from=paste&height=275&id=u99d532c5&originHeight=825&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=206272&status=done&style=shadow&taskId=ub1904ad0-7d9d-43cb-a8ae-ba7a1cd8d36&title=&width=1280)<br />由于BeanServiceB不涉及AOP，所以返回原始的B对象，此时填充的beanServiceA还是不完整的对象

- B不涉及提前AOP，所以`earlyProxyReferences`没有beanServiceB（见上文3.1.2.1.1）
- remove返回null，条件成立，执行wrapIfNecessary方法
- 由于beanServiceB不涉及AOP，所以返回原始的B对象
<a name="xprqY"></a>
##### 3.1.2.3. 处理提前暴露的场景，保证返回同一个代理对象
BeanServiceB不涉及AOP，getSingleton返回null，所以直接返回原始对象exposedObject<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639442852084-f2b7d34d-9730-42b6-ad78-93373b7f5c6f.webp#clientId=u7fa7d4ef-f290-4&from=paste&id=ud2a5d5ad&originHeight=420&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uab46485d-000b-46ca-b80a-8d0c2defb54&title=)
<a name="eOz3d"></a>
##### 3.1.2.4. BeanServiceB对象写入一级缓存，移除二、三级缓存
beanServiceB生命周期执行完，返回到3.1节第14行的`getSingleton`方法，此时返回的还是不完整的beanServiceB对象<br />`singletonsCurrentlyInCreation`移除bean，表明不再是正在创建的bean
```java
public Object getSingleton(String beanName, ObjectFactory<?> singletonFactory) {
    Assert.notNull(beanName, "Bean name must not be null");
    synchronized (this.singletonObjects) {
        Object singletonObject = this.singletonObjects.get(beanName);
        if (singletonObject == null) {
            ... ... ...
                // 会将当前beanName放入singletonsCurrentlyInCreation，表示当前bean正在创建
                beforeSingletonCreation(beanName);
            ... ... ...
                try {
                    // lambda表达式回到createBean
                    singletonObject = singletonFactory.getObject();
                    newSingleton = true;
                    ... ... ..
                    }
            finally {
                if (recordSuppressedExceptions) {
                    this.suppressedExceptions = null;
                }
                // singletonsCurrentlyInCreation中移除beanName，表示该bean不是正在创建了
                afterSingletonCreation(beanName);
            }
            if (newSingleton) {
                // 将不完整的beanServiceB对象写入一级缓存，移除二、三级缓存
                addSingleton(beanName, singletonObject);
            }
        }
        return singletonObject;
    }
}
```
beanServiceB“不完整对象”写入一级缓存，移除二、三级缓存
```java
protected void addSingleton(String beanName, Object singletonObject) {
    synchronized (this.singletonObjects) {
        // 写入一级缓存
        this.singletonObjects.put(beanName, singletonObject);
        // 移除二、三级缓存
        this.singletonFactories.remove(beanName);
        this.earlySingletonObjects.remove(beanName);
        this.registeredSingletons.add(beanName);
    }
}
```
<a name="hFpol"></a>
#### 3.1.3. BeanServiceA属性填充BeanServiceB（完整bean）
此时ioc容器已经有了beanServiceB，虽然暂时还是不完整的，因为A还没填充初始化完<br />当beanServiceA填充完beanServiceB后，使得互相循环依赖对方，此时二者都变成了完整的bean<br />此时一级缓存的beanServiceB也由不完整的bean变成了完整的bean，因为是同一个地址<br />A涉及AOP，所以循环依赖时，A会进行提前AOP，所以B中填充的是A的代理对象<br />当A填充完B时，构成互相循环依赖对方
<a name="Wyh6m"></a>
#### 3.1.4. 初始化BeanServiceA时调用AOP后置处理器进行AOP处理
由于beanServiceA提前AOP了，所以所以`earlyProxyReferences`有beanServiceA（见上文3.1.2.1.1）<br />remove返回原始的bean，即`earlyProxyReferences`对应的value，条件不成立，直接==返回原始bean ==
<a name="jCtDR"></a>
#### 3.1.5. 处理提前暴露的场景，保证返回同一个代理对象
beanServiceA涉及提前AOP，getSingleton返回保存在二级缓存中提前AOP的代理对象A<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639442851960-106c0bf8-8fb2-41bb-b742-720cc3c98bff.webp#clientId=u7fa7d4ef-f290-4&from=paste&id=u394535eb&originHeight=429&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uba788bb3-fd66-4df5-8449-b9e1245c653&title=)<br />这里面有`exposedObject == bean`判断，这就是为什么上述3.1.4中AOP后置处理器返回原始bean的原因<br />条件成立，将代理对象赋值放回，保证返回同一个代理对象，即B中的A和A都是同一个代理对象
<a name="kxpYu"></a>
### 3.2. BeanServiceA完整对象写入一级缓存，移除二、三级缓存
同3.1.2.4节一样，beanServiceA生命周期执行完，会将beanServiceA写入一级缓存，移除二、三级缓存
<a name="H9oBp"></a>
## 四、整体流程
<a name="LJvJ6"></a>
### 4.1 涉及循环依赖&&涉及AOP的场景
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639442852031-dd5817ae-ed83-4a48-805d-14c361997e7d.webp#clientId=u7fa7d4ef-f290-4&from=paste&id=u7043d862&originHeight=174&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u93364f9a-b93f-466e-85ea-2a2f60148c8&title=)
<a name="FFsl8"></a>
### 4.2 不涉及AOP的循环依赖场景

- 不涉及AOP，那么A进行lambda表达式回调后返回就A的原始对象，保存到二级缓存中，所以B属性填充后的A也是A的原始对象
- 当A属性填充完后，A和B相互依赖，使得二者都是完整的对象，可见上文3.1.3的图
<a name="HQffN"></a>
### 4.3 不涉及循环依赖的AOP场景

- 不涉及循环依赖，也就不涉及提前AOP，正常A经过实例化–属性填充–初始化
- 在初始化时通过AOP后置处理器创建代理对象返回，在上文1.1.5节，二级缓存返回null，最终直接返回代理对象A
<a name="FSDni"></a>
### 4.4 不涉及循环依赖&&不涉及AOP场景

- 不涉及循环依赖，也就不涉及提前AOP，正常A经过实例化–属性填充–初始化
- 不涉及AOP，则在初始化时通过AOP后置处理器直接返回原始A对象，在上文1.1.5节，二级缓存返回null，最终返回原始A对象
<a name="PcJdw"></a>
## 五、疑问：
<a name="izfSi"></a>
### 5.1 为什么三级缓存用HashMap，而不像一级缓存使用ConcurrentHashMap
现有逻辑是，三级缓存的操作是在synchronized代码块里面操作的，是安全的<br />那为什么要用synchronized而不直接用ConcurrentHashMap来保证线程安全呢？<br />二级缓存put的同时要保证三级缓存remove；三级缓存put时要保证二级缓存remove，也就是说二三级缓存操作要保证原子性<br />因为要保证同一个bean是单例的，不然都会lambda回调创建bean，就不是单例的了<br />如果使用ConcurrentHashMap并不能保证二三级缓存操作的原子性，所以要用synchronized<br />这三级缓存都是在synchronized内操作的，至于一级缓存为什么用ConcurrentHashMap，可能其他场景的原因。
<a name="AJrHU"></a>
### 5.2 什么要第三级缓存？
主要用于循环依赖的bean需要AOP时提前AOP<br />如果没有第三级缓存，那么getSingleton就返回null，就会再次传教A，导致一直循环创建，现有逻辑就不对.<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639442852718-4c6eb282-1bee-477c-b543-1a203a2247cf.webp#clientId=u7fa7d4ef-f290-4&from=paste&id=uc0f70882&originHeight=58&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u913910c3-2239-4541-a7e1-8fddd3c374e&title=)<br />那如果将实例化的原始对象放入二级缓存呢？<br />没有第三级缓存，就无法提前AOP，则B属性填充完的A为A原始对象<br />而A在属性填充完B后，需要进行AOP，则经过AOP后置处理器会去创建代理对象A返回<br />这就导致B的属性A不是代理对象，而A却是代理对象，这与Spring的单例bean是矛盾的。
<a name="KEBtG"></a>
### 5.3代码里如果出现了循环依赖怎么处理比较好？

1. 使用`@Lazy`注解，延迟加载
2. 使用`@DependsOn`注解，指定加载先后关系
3. 修改文件名称，改变循环依赖类的加载顺序
