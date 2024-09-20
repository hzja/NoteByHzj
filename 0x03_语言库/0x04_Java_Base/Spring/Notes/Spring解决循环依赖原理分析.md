Java Spring
<a name="Igkmv"></a>
## 1、什么是循环依赖？
![2021-05-18-09-33-07-223398.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621302154595-fa3bcef1-5add-4b9e-8428-c0918880a424.png#clientId=ua43dd86f-4aa4-4&from=ui&id=uc9d66bf3&originHeight=94&originWidth=315&originalType=binary&size=899&status=done&style=shadow&taskId=u60590f12-5d2a-4778-96e1-5ae99651c87)<br />循环依赖就是循环引用，就是两个或多个bean相互之间的持有对方。A类中有一个B类型的成员变量，需要注入BB类中有一个A类型的成员变量，需要注入A
<a name="EFkca"></a>
## 2、循环依赖的场景
<a name="osSbN"></a>
### 2.1、构造器的循环依赖【Spring无法解决】
两个循环依赖的类
```java
//A类
public Class A{
    private B b;

    pubcli A(B b){
        this.b=b;
    }
}
//B类
public Class B{
    private A a;
    public B(A a){
        this. a=a;
    }

}
```
xml配置信息为
```xml
<bean id="a" class="com.make.spring.A">
    <constructor-arg name="b" ref="b"></constructor-arg>
</bean>  
<bean id="b" class="com.make.spring.B">
    <constructor-arg name="a" ref="a"></constructor-arg>
</bean>
```
主方法的内容如下
```java
public static void main(String[] args) {
    ClassPathXmlApplicationContext context = new ClassPathXmlApplicationContext("springConstructor.xml");
    A bean = context.getBean(A.class);
    System.out.println(bean);
}
```
启动报错信息如下：<br />![2021-05-18-09-33-07-305196.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621302164857-6a4c0878-c4d0-4f31-9ca7-41759c12f6b8.png#clientId=ua43dd86f-4aa4-4&from=ui&id=u0a9070a1&originHeight=87&originWidth=1080&originalType=binary&size=32180&status=done&style=none&taskId=u05c65e31-0e9c-4648-a868-6faca16e107)<br />Spring是无法解决构造函数循环依赖注入的，因为这种时候，在实例化A的时候就要获取B的实例化对象，在实例化B的时候要A的实例化对象，所以无法创建任何一个实例。
<a name="O5LRo"></a>
### 2.2、setter方法注入属性，singleton单例模式
```java
public class A {
    private B b;

    public void setB(B b) {
        this.b = b;
    }

    public B getB() {
        return b;
    }
}

public class B {
    private A a;

    public void setA(A a) {
        this.a = a;
    }

    public A getA() {
        return a;
    }
}
```
xml配置信息
```xml
<bean id="a" class="com.make.bean.A">
    <property name="b" ref="b"></property>
</bean>
<bean id="b" class="com.make.bean.B">
    <property name="a" ref="a"></property>
</bean>
```
主方法信息
```java
public static void main(String[] args) {
 ClassPathXmlApplicationContext context=new ClassPathXmlApplicationContext("spring.xml");
 A beanA =context.getBean(A.class);
 System.out.println(beanA.getB());
 B beanB=context.getBean(B.class);
 System.out.println(beanB.getA());
}
```
最后成功的打印信息<br />![2021-05-18-09-33-07-394942.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621302177178-aa5a009d-c0a0-4f53-9f8d-3f4916526c26.png#clientId=ua43dd86f-4aa4-4&from=ui&id=u323c2eb1&originHeight=55&originWidth=353&originalType=binary&size=7071&status=done&style=none&taskId=u64e5a360-e67b-4a00-81b2-b8cd819757e)<br />这种情况下，Spring将A的对象实例化后，还未注入属性的A提前缓存起来，也就是提前暴露A，然后让B的实例化过程拿到A来解决依赖注入。
<a name="p1RgW"></a>
### 2.3、`setter`方式，`prototype`原型模式【Spirng无法解决】
两个java类的内容和主方法的内容和1.2的相同<br />xml配置信息
```xml
<bean id="a" class="com.make.bean.A" scope="prototype">
    <property name="b" ref="b"></property>
</bean>
<bean id="b" class="com.make.bean.B"  scope="prototype">
    <property name="a" ref="a"></property>
</bean>
```
最后显示的错误信息如下：<br />![2021-05-18-09-33-07-506609.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621302231887-09909b23-3efb-4095-80db-98853385561a.png#clientId=ua43dd86f-4aa4-4&from=ui&id=u6265f4d9&originHeight=62&originWidth=1080&originalType=binary&size=31352&status=done&style=none&taskId=u6ef95a3d-0c6b-46fb-873a-2b840160eb6)<br />对于“prototype”作用域Bean，Spring容器无法完成依赖注入，因为“prototype”作用域的Bean，Spring容器不进行缓存，因此无法提前暴露一个创建中的Bean。
<a name="X4Uex"></a>
## 3、单例`setter`方法注入—循环依赖解决方法
<a name="DQIBv"></a>
### 3.1、使用缓存解决循环依赖
整个Spring创建bean对象的步骤主要是如下三三步<br />![2021-05-18-09-33-07-582408.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621302254790-644a4be8-2422-4875-a59d-3b607730e925.png#clientId=ua43dd86f-4aa4-4&from=ui&id=u42da8a35&originHeight=248&originWidth=160&originalType=binary&size=2115&status=done&style=shadow&taskId=ud223f2ed-8256-4b21-9d72-1b6d6341874)<br />然后在和创建A对象的时候，注入属性时，需要获取B对象，在注入B的时候获取A对象，会造成循环<br />![2021-05-18-09-33-07-664189.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621302261893-7e47ee5b-678a-4ca8-adad-05347808e62f.png#clientId=ua43dd86f-4aa4-4&from=ui&id=ua77e250f&originHeight=220&originWidth=710&originalType=binary&size=21623&status=done&style=shadow&taskId=ud4d81d2c-e4ad-449b-ab59-4d8d97cd46f)<br />那么这个时候他Spring提供了一个缓存来解决这种循环的问题<br />![2021-05-18-09-33-07-774894.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621302272067-e0b5577b-26d7-4d06-b772-471bad41b819.png#clientId=ua43dd86f-4aa4-4&from=ui&id=u9c51d9be&originHeight=399&originWidth=801&originalType=binary&size=37770&status=done&style=shadow&taskId=u6f18a203-b110-4db4-aef2-be551af25ab)<br />在A对象实例化完后，就先将A对象存入到缓存中暴露出来，让B可以拿到A，拆除循环。Spring的对于解决循环依赖，设置了三级缓存，用来存储不同时期的对象。
<a name="oVvBT"></a>
### 3.2、三级缓存
Spring的三级循环分别为
```java
 /** 一级缓存
  * 单例对象缓存：bean name --> bean的完整实例*/
 private final Map<String, Object> singletonObjects = new ConcurrentHashMap<>(256);
 
 /** 三级缓存
  * 保存BeanName于创建bean工程之间的缓存：bean name --> 对象工程，lambda表达式*/
 private final Map<String, ObjectFactory<?>> singletonFactories = new HashMap<>(16);

 /**  二级缓存
 * 保存单例对象的半成品：bean name--> 半成品bean（bean实例化好后，没有进行属性填充，没有初始化的半成品对象）*/
 private final Map<String, Object> earlySingletonObjects = new HashMap<>(16);
```
<a name="oaOYG"></a>
### 3.3、代码执行过程示意图
这里的代码运行过程是按照上面1.2的代码进行说明的。<br />![2021-05-18-09-33-07-959399.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621302281906-ebdf4c4d-b098-4579-8a50-4f4f4770059b.png#clientId=ua43dd86f-4aa4-4&from=ui&id=u76b0707f&originHeight=902&originWidth=1080&originalType=binary&size=215114&status=done&style=shadow&taskId=u279b9624-6007-42f7-9401-2a8c32d883e)
<a name="yvnIl"></a>
## 4、依赖注入具体执行方法
<a name="atyWU"></a>
### 4.1、调用`ClassPathXmlApplicationContext`的构造方法
```java
public ClassPathXmlApplicationContext(
    String[] configLocations, boolean refresh, @Nullable ApplicationContext parent)
    throws BeansException {
    
    super(parent);
    //存储spring配置文件的路径到本地
    setConfigLocations(configLocations);
    //判断是否需要需要重新刷新spring容器——ApplicationContext
    if (refresh) {
        //刷新spring容器，加载bean的核心方法，刷新spring上下文信息，定义了上下文加载流程——ApplicationContext
        refresh();
    }
}
```
`refresh ApplicationContext`，重新刷新Spring容器，注入必要的类、加载bean的一些配置、并实例化bean。因为`AbstractApplicationContext`是`ClassPathXmlApplicationContext`的父类，所以调用的是`AbstractApplicationContext`的`refresh`方法<br />![2021-05-18-09-33-08-231672.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621302325919-06c9e669-5bf8-4743-952a-2d10b3c3b000.png#clientId=ua43dd86f-4aa4-4&from=ui&id=u7c19e8d3&originHeight=243&originWidth=820&originalType=binary&size=599073&status=done&style=none&taskId=ud597cc8f-1ad0-4f4f-8c60-8bcae92d2e6)
<a name="Xzox5"></a>
### 4.2、进入到`AbstractApplicationContext`类的`refresh()`方法
```java
@Override
public void refresh() throws BeansException, IllegalStateException {
    synchronized (this.startupShutdownMonitor) {
        /**
          * 1、设置容器的启动时间
          * 2、设置活跃状态为true
          * 3、设置关闭状态为false
          * 4、获取Environment对象，并加载当前系统的属性到Environment对象中
          * 5、准备监听器和时间的结合对象，默认为空的集合
          */
        // Prepare this context for refreshing.
        prepareRefresh();

        //创建容器对象:DefaultListableBeanFactory
        //加载xml文件的属性，最终要的就是BeanDefinition
        ConfigurableListableBeanFactory beanFactory = obtainFreshBeanFactory();

        //beanFacotry的准备工作，对各种属性进行填充
        prepareBeanFactory(beanFactory);
        try {
            //子类覆盖方法做额外的处理，此出我们一般不做任何扩展工作，可以查看web的代码，是有具体实现
            postProcessBeanFactory(beanFactory);

            //调用Bean工厂的后置处理器
            invokeBeanFactoryPostProcessors(beanFactory);

            //注册BeanPostProcessor 自定义以及spring内部的
            registerBeanPostProcessors(beanFactory);

            //  初始化此上下文的消息源
            initMessageSource();

            // 初始化事件监听多路广播器
            initApplicationEventMulticaster();

            // 留给子类来初始化其他的bean
            onRefresh();

            //在所有注册的bean中查找listener bean，注册到消息广播中
            registerListeners();

            //初始化剩下的单实例（非懒加载的）
            finishBeanFactoryInitialization(beanFactory);

            //完成刷新过程，通知生命周期处理器lifecycleProcessor刷新过
            finishRefresh();
        }
    ........
}
```
其中的核心代码为换初始化剩下的单实例（非懒加载的），也就是43行。是整个Spring容器初始化的核心流程。
<a name="n3hxr"></a>
### 4.3、进入到`finishBeanFactoryInitialization`方法
```java
protected void finishBeanFactoryInitialization(ConfigurableListableBeanFactory beanFactory) {
    ......
    // Instantiate all remaining (non-lazy-init) singletons.
    //实例化剩下的单例对象
    beanFactory.preInstantiateSingletons();
}
```
那么这个方法里面涉及到实例化的就是`beanFactory.preInstantiateSingletons()`;——实例化剩下的单例对象。<br />由于`ConfigurableListableBeanFactory`的实现类为`DefaultListableBeanFactory`，所以之间调用`DefaultListableBeanFactory`的`preInstantiateSingletons`方法。
<a name="elatl"></a>
### 4.4、进入到`preInstantiateSingletons`方法
```java
@Override
public void preInstantiateSingletons() throws BeansException {
    if (logger.isDebugEnabled()) {
        logger.debug("Pre-instantiating singletons in " + this);
    }

    //将所有的BeanDefinition的名字创建一个集合，beanNames获取到的是[a,b]
    List<String> beanNames = new ArrayList<>(this.beanDefinitionNames);

    //触发所有非延迟加载单例bean的初始化，遍历集合
    for (String beanName : beanNames) {
        //合并父类BeanDefinition
        //Bean定义的公共抽象类是AbstractBeanDefinition,普通的bean在spring加载bean定义的时候，实例化出来的是GenericBeanDefinition，而Spring上下文
        //所有bean用的AbstractBeanDefintion是RootBeanDefinition，这个时候需要转换一些，将非RootBeanDefinition转成RootBeanDefinition进行后续操作
        RootBeanDefinition bd = getMergedLocalBeanDefinition(beanName);
        //条件判断，抽象，单例，非懒加载
        if (!bd.isAbstract() && bd.isSingleton() && !bd.isLazyInit()) {
            //判断是否实现了FactoryBean接口
            if (isFactoryBean(beanName)) {
                //根据&+beanName来获取具体的对象
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
                    //如果急切的希望初始化，通过beanName获取bean的实例
                    if (isEagerInit) {
                        getBean(beanName);
                    }
                }
            }
            else {
                //如果beanName对应的bean不是FactoryBean，只是普通的bean，通过beanName获取bean实例
                getBean(beanName);
            }
        }
    }

    //遍历beanNames，触发所有的SmartInitializingSingleton的后初始化毁掉
   ......
}
```
这个时候beanNames一共有两个一个是a，一个是b，那么先获取a。
<a name="W5zOu"></a>
#### 4.4.1、第一个判断
a不是抽象，也不是懒加载，是单例，所以会走进
```java
if (!bd.isAbstract() && bd.isSingleton() && !bd.isLazyInit()) 
```
<a name="E0Mlp"></a>
#### 4.4.2、第二个判断
a没有实现FactoryBean接口所以直接执行`getBean(a)`
```java
if (isFactoryBean(beanName)) {
 ......
}
else {
    //如果beanName对应的bean不是FactoryBean，只是普通的bean，通过beanName获取bean实例
    getBean(beanName);
}
```
<a name="CDlzG"></a>
### 4.5、进入到`getBean`方法
```java
@Override
public Object getBean(String name) throws BeansException {
 //此方法获取实际bean的方法，也是触发依赖注入的方法
 return doGetBean(name, null, null, false);
}
```
直接调用`doGetBean`
<a name="t1dO1"></a>
### 4.6、进入到`dogetBean`方法
```java
@SuppressWarnings("unchecked")
protected <T> T doGetBean(final String name, @Nullable final Class<T> requiredType,
                          @Nullable final Object[] args, boolean typeCheckOnly) throws BeansException {
    //提取对应的beanName
    final String beanName = transformedBeanName(name);
    Object bean;
    // Eagerly check singleton cache for manually registered singletons.
    //提前检查单例缓存中是否有手动注册的单例对象，跟循环依赖有关联——6.1
    Object sharedInstance = getSingleton(beanName);
    //如果bean的单例对象找到了，且没有创建bean实例时要使用的参数
    if (sharedInstance != null && args == null) {
  .....
    }
    else {
        //判断我们是不是正在创建这个实例，如果是失败，应该是在循环参考之内
        if (isPrototypeCurrentlyInCreation(beanName)) {
            throw new BeanCurrentlyInCreationException(beanName);
        }

        //检查该工厂中是否存在bean定义
        BeanFactory parentBeanFactory = getParentBeanFactory();
        if (parentBeanFactory != null && !containsBeanDefinition(beanName)) {
            .....
        }
        //如果不是做类型检查，那么就用创建bean，此处在集合中做一个记录
        if (!typeCheckOnly) {
            markBeanAsCreated(beanName);
        }

        try {
            //此处做了BeanDefinition对象的转换，当我们从xml文件中加载BeanDefinition对象的时候，封装的对象时GenericBeanDefinition,
            //此处要做类型转换，如果是子类bean的话，去合并父类的相关属性
            final RootBeanDefinition mbd = getMergedLocalBeanDefinition(beanName);
            checkMergedBeanDefinition(mbd, beanName, args);

            ......

            // Create bean instance.
            //创建bean的实例对象，这里都可mdb就是RootBeanDefinition，也就是bean相关的一些定义信息
            if (mbd.isSingleton()) {
                //返回以beanName的（原始）单例对象，如果尚未注册，使用singletonFactory创建并注册一个对象
                sharedInstance = getSingleton(beanName, () -> {
                    try {
                        //为给定的合并后的BeanDefinition(和参数)创建一个bean实例
                        return createBean(beanName, mbd, args);
                    }
                    catch (BeansException ex) {
                        //显示地从单例缓存中删除实例，可能是由创建过程急切地放在那里，以允许循环引用解析，还要删除
                        //接收该bean临时引用的任何bean
                        //销毁给定的bean，如果找到响应的一次性bean实例，委托给destoryBean
                        destroySingleton(beanName);
                        //重新抛出ex
                        throw ex;
                    }
                });
                bean = getObjectForBeanInstance(sharedInstance, name, beanName, mbd);
            }
            ......
        }
        catch (BeansException ex) {
            cleanupAfterBeanCreationFailure(beanName);
            throw ex;
        }
    }
    .....
    return (T) bean;
}
```
进入到`doGetBean`，这个时候才是真正获取bean的时候
<a name="xu5Ch"></a>
#### 4.6.1、先尝试从缓存中获取bean——`getSingleton(beanName)`
```java
 //提前检查单例缓存中是否有手动注册的单例对象
Object sharedInstance = getSingleton(beanName);
```
具体这块执行的代码是
```java
@Nullable
protected Object getSingleton(String beanName, boolean allowEarlyReference) {
    //从单例对象缓存中获取beanName对应的单例对象<<一级缓存>>
    Object singletonObject = this.singletonObjects.get(beanName);
    //如果单例对象缓存中没有，并且该beanName对应的单例bean正在创建中
    if (singletonObject == null && isSingletonCurrentlyInCreation(beanName)) {
        //如果为空，则锁定全局变量并进行处理
        synchronized (this.singletonObjects) {
            //从早期单例对象缓存中获取单例对象（之所以被称为早期单例对象，是因为earlySingletonObjects里
            // 的所有对象都是通过提前曝光的ObjectFactory创建出来的，还未进行属性填充等操作）
            singletonObject = this.earlySingletonObjects.get(beanName);
            //如果早期单例对象缓存中也没有，并且允许创建早期单例对象引用
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
这个时候一级缓存是没有a对象的，所以`singletonObject`获取的为null然后a也没有在创建过程中，所以直接返回null。
<a name="N4NsA"></a>
#### 4.6.2、开始调用`getSingleton(beanName,singleFacotry)`
```java
if (mbd.isSingleton()) {
    //返回以beanName的（原始）单例对象，如果尚未注册，使用singletonFactory创建并注册一个对象
    sharedInstance = getSingleton(beanName, () -> {
        try {
            //为给定的合并后的BeanDefinition(和参数)创建一个bean实例
            return createBean(beanName, mbd, args);
        }
        ......
    });
    bean = getObjectForBeanInstance(sharedInstance, name, beanName, mbd);
}
```
这块具体执行的为
```java
public Object getSingleton(String beanName, ObjectFactory<?> singletonFactory) {
    //如果beanName为null，抛出异常
    Assert.notNull(beanName, "Bean name must not be null");
    //使用单实例对象的高速缓冲Map作为锁，保证线程同步
    synchronized (this.singletonObjects) {
        //从单例对象的高速缓冲Map中获取beanName对应的单例对象
        Object singletonObject = this.singletonObjects.get(beanName);
        //如果获取到的为空
        if (singletonObject == null) {
            //如果当前在destorySingletons中
            if (this.singletonsCurrentlyInDestruction) {
                ......
            }
            ......
            //创建单例之前的回调，默认实现将单例注册为当前正在创建中
            beforeSingletonCreation(beanName);
            ......
            try {
                //从单例工厂中获取对象
                singletonObject = singletonFactory.getObject();
                //生成了新的单例对象的标记成true，表示生成了新的单例对象
                newSingleton = true;
            }
            ......
            finally {
                //如果没有抑制异常记录
                if (recordSuppressedExceptions) {
                    this.suppressedExceptions = null;
                }
                //创建单例后的回调，默认实现将单例标记为不在创建中
                afterSingletonCreation(beanName);
            }
            //生成了新的单例对象
            if (newSingleton) {
                //将beanName和singletonObject的映射关系添加到该工厂的单例缓存中，也就是一级缓存中
                addSingleton(beanName, singletonObject);
            }
        }
        return singletonObject;
    }
}
```
这个一级缓存获取到的为null，然后将bean的状态变成创建中，之后调用 `singletonFactory.getObject()`方法。也就是进入到传给`getSingleton`方法的`lamabda`的`getBean`方法
<a name="U1Foo"></a>
### 4.7、进入到`getBean`方法
```java
@Override
protected Object createBean(String beanName, RootBeanDefinition mbd, @Nullable Object[] args)
    throws BeanCreationException {
    ......
    try {
        //实际创建bean的调用
        Object beanInstance = doCreateBean(beanName, mbdToUse, args);
        if (logger.isDebugEnabled()) {
            logger.debug("Finished creating instance of bean '" + beanName + "'");
        }
        return beanInstance;
    }
    ......
}
```
这里调用真正创建bean的方法，`doGetBean`
<a name="shm6m"></a>
### 4.8、进入到`doGetBean`
```java
protected Object doCreateBean(final String beanName, final RootBeanDefinition mbd, final @Nullable Object[] args)
   throws BeanCreationException {

    //这个beanWrapper是用来持有创建出来的Bean对象的
    BeanWrapper instanceWrapper = null;
    //获取factoryBean实例缓存
    if (mbd.isSingleton()) {
        //如果是单例对象，从factorybean实例缓存中移除当前bean的定义信息
        instanceWrapper = this.factoryBeanInstanceCache.remove(beanName);
    }
    //如果实例为空
    if (instanceWrapper == null) {
        //实例化bean
        //根据bean使用对应的策略创建新的实例，如：工厂方法，构造函数主动注入，简单初始化
        instanceWrapper = createBeanInstance(beanName, mbd, args);
    }
    //从包装类中获取原始bean
   ......
    
    //运行beanPostProcessor去修改合并的beanDefinition
    ......
    //缓存单例以便处理循环引用，即使是像BeanFactoryAware这样的生命周期接口触发的
    boolean earlySingletonExposure = (mbd.isSingleton() && this.allowCircularReferences &&
                                      isSingletonCurrentlyInCreation(beanName));
    if (earlySingletonExposure) {
       ......
        //添加到缓存当中
        addSingletonFactory(beanName, () -> getEarlyBeanReference(beanName, mbd, bean));
    }

    //初始化bean实例
    Object exposedObject = bean;
    try {
        //注入过程
        populateBean(beanName, mbd, instanceWrapper);
        // 将会触发postProcessBeforeInitialization和postProcessAfterInitialization
        exposedObject = initializeBean(beanName, exposedObject, mbd);
    }
    catch (Throwable ex) {
       ......
    }
    ......
    }
    //注册完成依赖注入的Bean
    try {
        registerDisposableBeanIfNecessary(beanName, bean, mbd);
    }
   .....
    return exposedObject;
}
```
<a name="SdWPY"></a>
#### 4.8.1、开始调用`createBeanInstance(beanName, mbd, args);`
```java
protected BeanWrapper createBeanInstance(String beanName, RootBeanDefinition mbd, @Nullable Object[] args) {
    //解析calss
    Class<?> beanClass = resolveBeanClass(mbd, beanName);

    ......

    // 一个类可能有多个构造器，所以Spring得根据参数个数、类型确定需要调用的构造器
    // 在使用构造器创建实例后，Spring会将解析过后确定下来的构造器或工厂方法保存在缓存中，避免再次创建相同bean时再次解析
    boolean resolved = false;
    boolean autowireNecessary = false;
    if (args == null) {
        synchronized (mbd.constructorArgumentLock) {
            //判断是否有解析的构造方法或者工厂方法
            if (mbd.resolvedConstructorOrFactoryMethod != null) {
                //已经解析过的class构造器
                resolved = true;
                autowireNecessary = mbd.constructorArgumentsResolved;
            }
        }
    }
    //如果解析过
    if (resolved) {
        ......
    }
    //需要根据参数解析、确认构造器的函数（从Bean后处理器确定构造函数）
    Constructor<?>[] ctors = determineConstructorsFromBeanPostProcessors(beanClass, beanName);
    // 解析的构造器不为空 || 注入类型为构造函数自动注入 || bean定义中有构造器参数 || 传入参数不为空
    if (ctors != null || mbd.getResolvedAutowireMode() == AUTOWIRE_CONSTRUCTOR ||
       ......
    }

    // No special handling: simply use no-arg constructor.
    //使用默认构造器
    return instantiateBean(beanName, mbd);
}
```
调用具体的创建bean的方式，有好几种，本例子采用的是//使用默认构造器 `return instantiateBean(beanName, mbd);` 
<a name="IJ7RK"></a>
#### 4.8.2、将实例化后的a放入到缓存中
判断是否符合需要提前暴露对象，加入缓存的条件
```java
boolean earlySingletonExposure = (mbd.isSingleton() && this.allowCircularReferences &&
                                      isSingletonCurrentlyInCreation(beanName));
```

- 目前a单例的
- `allowCircularReferences`是允许循环依赖的，这个字段Spring默认为true，也可以通过改变这个字段，而禁止循环依赖
- `isSingletonCurrentlyInCreation(a)`，a也是在创建过程中的，在2.3.6的②中已经执行了将a设置为创建中的状态均符合要求所以会执行加入缓存的操作
```java
if (earlySingletonExposure) {
    ......
     //添加到缓存当中
     addSingletonFactory(beanName, () -> getEarlyBeanReference(beanName, mbd, bean));
}
```
加入缓存具体执行的方法为：
```java
protected void addSingletonFactory(String beanName, ObjectFactory<?> singletonFactory) {
    Assert.notNull(singletonFactory, "Singleton factory must not be null");
    //使用singletonObjects进行加锁，保证线程安全
    synchronized (this.singletonObjects) {
        //如果单例对象的高速缓存【Bean名称-bean实例】没有beanName对象
        if (!this.singletonObjects.containsKey(beanName)) {
            //将beanName.singletonFactory放入到单例工厂的缓存当中【bean名称-ObjectFactory】
            this.singletonFactories.put(beanName, singletonFactory);
            //从早期单例对象的高速缓存【bean名称-bean实例】，移除beanName的相关缓存对象
            this.earlySingletonObjects.remove(beanName);
            //将beanName添加到已注册的单例集中
            this.registeredSingletons.add(beanName);
        }
    }
}
```
下面就该进行到调用`populateBean`方法为a注入属性
<a name="nndZh"></a>
### 4.9、调用方法populateBean注入属性
```java
protected void populateBean(String beanName, RootBeanDefinition mbd, @Nullable BeanWrapper bw) {
    // 如果bean实例为空（空实例），并且其属性不为空，则抛出错误，否则跳过
    ......
    // 在设置属性之前调用Bean的PostProcessor后置处理器
    if (!mbd.isSynthetic() && hasInstantiationAwareBeanPostProcessors()) {
        for (BeanPostProcessor bp : getBeanPostProcessors()) {
            if (bp instanceof InstantiationAwareBeanPostProcessor) {
                InstantiationAwareBeanPostProcessor ibp = (InstantiationAwareBeanPostProcessor) bp;
                if (!ibp.postProcessAfterInstantiation(bw.getWrappedInstance(), beanName)) {
                    return;
                }
            }
        }
    }
    // 依赖注入开始，首先处理autowire自动装配的注入——因为我这里没有使用autowire所以省略
    ......
    // 检查容器是否持有用于处理单态模式Bean关闭时的后置处理器
    ......
    if (pvs != null) {
        //主线
        // 对属性进行注入操作，解决任何在这个bean工厂运行时其他bean的引用，必须使用深拷贝，所以不会永久修改这个属性
        applyPropertyValues(beanName, mbd, bw, pvs);
    }
}
```
<a name="qGnaN"></a>
#### 4.9.1、 调用设置xml属性的核心方法`applyPropertyValues`
```java
protected void applyPropertyValues(String beanName, BeanDefinition mbd, BeanWrapper bw, PropertyValues pvs) {
    //如果pvsPropertyValue
    if (pvs.isEmpty()) {
        //直接结束方法
        return;
    }
    ......
    List<PropertyValue> deepCopy = new ArrayList<>(original.size());
    boolean resolveNecessary = false;
    //遍历属性，将属性转换成对应类的对应属性类型
    for (PropertyValue pv : original) {
        //如果该属性已经被解析过
        if (pv.isConverted()) {
            //将pv添加到deepCopy中
            deepCopy.add(pv);
        }
        //如果没有
        else {
            //获取属性名
            String propertyName = pv.getName();
            //获取未经类型转换的值
            Object originalValue = pv.getValue();
            //由valueResolver根据pv解析出originalValue所封装的对象
            Object resolvedValue = valueResolver.resolveValueIfNecessary(pv, originalValue);
           ......
        }
    }
    ......
}
```
<a name="yYPHC"></a>
#### 4.9.2、 调用`resolveValueIfNecessary`方法解决属性的值
```java
@Nullable
public Object resolveValueIfNecessary(Object argName, @Nullable Object value) {
    //如果values是RuntimeBeanReference实例
    if (value instanceof RuntimeBeanReference) {
        //将value强转成RuntimeBeanReference对象
        RuntimeBeanReference ref = (RuntimeBeanReference) value;
        //解析出对应的ref所封装的bean元信息（bean名称和类型）的Bean对象
        //处理引用
        return resolveReference(argName, ref);
    }
    ......
}
```
<a name="AkJMF"></a>
#### 4.9.3、 调用解决属性的ref外部引用`resolveReference`方法
```java
@Nullable
private Object resolveReference(Object argName, RuntimeBeanReference ref) {
    try {
        //定义一个存储b对象的变量
        Object bean;
        //获取两一个bean引用的bean类型
        String refName = ref.getBeanName();
        refName = String.valueOf(doEvaluate(refName));
        //如果引用来自父工厂
        if (ref.isToParent()) {
           ......
        }
        else {
            //获取resolvedName的bean对象
            bean = this.beanFactory.getBean(refName);
            //注册beanName与dependentBeanNamed的依赖关系到bean工厂
            this.beanFactory.registerDependentBean(refName, this.beanName);
        }
        if (bean instanceof NullBean) {
            bean = null;
        }
        return bean;
    }
    ......
}
```
执行到下面的getBean，就开始获取b
```java
//获取resolvedName的bean对象
bean = this.beanFactory.getBean(refName);
```
<a name="PtCXR"></a>
### 4.10、创建b的过程

- 创建b的过程会循环前面的4.5—4.9.2，直道注入b的属性时，获取a，也就是运行到4.9.3的getBean(a)开始获取b的属性值a
- 再次到循环4.5获取a对象，也就是b的属性a的value
- 这个时候先执行4.6.1的`getSingleton`方法，这个时候a在一级缓存中没有，但是a是出于创建过程中的，所以可以拿到a并将a放入二级缓存，移出三级缓存
```java
singletonObject = this.earlySingletonObjects.get(beanName);
//如果早期单例对象缓存中也没有，并且允许创建早期单例对象引用
if (singletonObject == null && allowEarlyReference) {
    ObjectFactory<?> singletonFactory = this.singletonFactories.get(beanName);
    if (singletonFactory != null) {
        singletonObject = singletonFactory.getObject();
        this.earlySingletonObjects.put(beanName, singletonObject);
        this.singletonFactories.remove(beanName);
    }
}
```

- 从三级缓存中获取到a后，再执行4.6中的真正获取工厂中的a的`getObjectForBeanInstance`方法
```java
bean = getObjectForBeanInstance(sharedInstance, name, beanName, mbd);
```

- 将拿到的a返回到4.9.3的`getBean`方法中，a的注入属性完成，之后执行4.8的初始化方法
```java
// 将会触发postProcessBeforeInitialization和postProcessAfterInitialization
exposedObject = initializeBean(beanName, exposedObject, mbd);
```

- 将拿到的a返回到4.9.3的`getBean`方法中，a的注入属性完成，之后执行4.8的初始化方法
```java
// 将会触发postProcessBeforeInitialization和postProcessAfterInitialization
exposedObject = initializeBean(beanName, exposedObject, mbd);
```

- b初始化完成后，返回b对象到4.6.2，将b对象放入到一级缓存中
```java
//生成了新的单例对象
if (newSingleton) {
    //将beanName和singletonObject的映射关系添加到该工厂的单例缓存中，也就是一级缓存中
    addSingleton(beanName, singletonObject);
}
//addSingleton方法内部执行
protected void addSingleton(String beanName, Object singletonObject) {
    synchronized (this.singletonObjects) {
        //将映射关系添加到单例对象的高速缓冲中
        this.singletonObjects.put(beanName, singletonObject);
        //移除beanName在三级缓存中的数据——工厂缓存
        this.singletonFactories.remove(beanName);
        //移除beanName在二级缓存中的对象
        this.earlySingletonObjects.remove(beanName);
        //将beanName添加到已经注册的单例集中
        this.registeredSingletons.add(beanName);
    }
}
```
<a name="t9ZYT"></a>
### 4.11、将a创建完成

- b对象创建完成之后，将b对象返回到a的属性注入时获取a的`getBean(b)`方法，将a类似b注入完成后的过程一样，创建完成a
