<a name="ycb9Q"></a>
## 1、前言
在日常开发中，Bean之间的循环依赖非常常见，Spring 已经做到使用无感知处理，那么 Spring 是如何实现的呢？
<a name="Ak0LZ"></a>
## 2、循环依赖简介
<a name="L1CjB"></a>
### 2.1 什么是循环依赖
循环依赖是指两个或多个对象存在相互依赖、相互引用的关系，而这种引用形成一个环时，就会出现循环引用，如图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1702354946469-7f3c4316-4411-45b0-bcdf-9f1e952a6928.png#averageHue=%23323232&clientId=u98dcc14b-ff87-4&from=paste&id=ub40eb694&originHeight=163&originWidth=330&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3193cd23-153b-49ed-a7a6-0cd286687d4&title=)
```java
public class PersonA { 
    @Autowired
    private PersonB personB;
}
```
```java
public class PersonB {
    @Autowired
    private PersonA personA;
}
```
<a name="Yx9aV"></a>
### 2.2 Spring 处理循环依赖的前提条件

1. 相互依赖的 Bean 必须为单例；

因为如果每次请求都创建一个 Bean，那么在处理循环依赖的时候，每次都会产生一个新的 Bean 实例，由于没有全局的实例 Bean 缓存，则无法处理循环依赖

2. 依赖注入的方式不能都是构造函数注入的方式。

使用构造函数注入，Bean 实例在构造函数没有完全被调用时是不会创建的；因为 PersonA 引用 PersonB，PersonB 又引用 PersonA，两者都无法进行初始化，产生了死锁
<a name="DUSsW"></a>
## 3、三级缓存原理
<a name="OyhT2"></a>
### 3.1 什么是三级缓存
Spring 是通过三级缓存的方式处理循环依赖，三级缓存是 Spring Bean 在各个阶段的缓存
<a name="duvgv"></a>
#### 一级缓存（`SingletonObjects`）：
存放已经完全实例化、初始化的bean，实现了单利bean只会实例化和初始化一次
<a name="ymJ21"></a>
#### 二级缓存（`EarlySingletonObjects`）：
存放早期暴露出来的Bean对象，bean的生命周期还未完成(未完成属性注入与初始化的bean)
<a name="W8WWR"></a>
#### 三级缓存（`SingletonFactories`）：
三级缓存中存储的是单利工厂缓存，通过调用该对象的 `**GetObject**` 方法，可以获取到早期暴露出去的 Bean；在该 Bean 要被其他 Bean 引用时，Spring 就会用工厂对象创建出该 Bean 的实例对象，最终当该 Bean 完成构造的所有步骤后就会将该 Bean 放入到一级缓存中
```java
/** 一级缓存 */
private final Map<String, Object> singletonObjects = new ConcurrentHashMap<String, Object>(256);
/** 二级缓存 */ 
private final Map<String, Object> earlySingletonObjects = new HashMap<String, Object>(16); 
/** 三级缓存 */ 
private final Map<String, ObjectFactory<?>> singletonFactories = new HashMap<String, ObjectFactory<?>>(16);
```
<a name="bymLq"></a>
### 3.2 三级缓存流程
![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1702366431029-33828164-5563-438f-9cc1-a9565a0d76d9.jpeg)
<a name="N21uN"></a>
### 3.3 三级缓存源码解析
<a name="ZrNh4"></a>
#### 创建 Bean 主要的方法是 `AbstractBeanFactory.doGetBean` 方法
```java
protected <T> T doGetBean( String name, @Nullable Class<T> requiredType, @Nullable Object[] args, boolean typeCheckOnly) throws BeansException {
    // 获取bean的规范名称
    String beanName = transformedBeanName(name); 
    Object bean;  
    // 从各级缓存中获取bean对象 
    Object sharedInstance = getSingleton(beanName); 
    // 跟factoryBean相关判断
    if (sharedInstance != null && args == null) { 
        ...
    } 
    // 获取factorybean的真是bean
    //若为普通bean则直接返回对象

    bean = getObjectForBeanInstance(sharedInstance, name, beanName, null); 
} 
...... 
// 创建单利bean对象 
if (mbd.isSingleton()) { 
    sharedInstance = getSingleton(beanName, () -> { 
        try {
            // 创建bean
            return createBean(beanName, mbd, args); 
        } catch (BeansException ex) { 
            // Explicitly remove instance from singleton cache: It might have been put there 
            // eagerly by the creation process, to allow for circular reference resolution. 
            // Also remove any beans that received a temporary reference to the bean. destroySingleton(beanName); 
            throw ex; 
        } 
    }); 
    bean = getObjectForBeanInstance(sharedInstance, name, beanName, mbd); 
} 
    ......    
// 返回bean对象 return (T) bean; }
```
<a name="JbBy5"></a>
#### 看两个比较重要获取 Bean 的方法 `GetSingleton()`
```java
// 这个方法主要是三级缓存容器，思路大概是：从一级缓存查询，若找不到去二级缓存查询，还是不存在则去三级缓存，若三级缓存找到了，则将bean放入二级缓存中
protected Object getSingleton(String beanName, boolean allowEarlyReference) {
    // 从一级缓存中查找bean
    Object singletonObject = this.singletonObjects.get(beanName);
    // 判断一级缓存查找不到bean && bean是否处于创建中，成立，则进入循环依赖
    if (singletonObject == null && isSingletonCurrentlyInCreation(beanName)) {
        synchronized (this.singletonObjects) {
            // 从二级缓存中查找
            singletonObject = this.earlySingletonObjects.get(beanName);
            // 二级缓存未查询到 && 是否允许获取早期引用
            if (singletonObject == null && allowEarlyReference) {
                // 从三级缓存查询
                ObjectFactory<?> singletonFactory = this.singletonFactories.get(beanName);
                // 三级缓存存在bean
                if (singletonFactory != null) {
                    // 获取bean实例
                    singletonObject = singletonFactory.getObject();
                    // 从三级缓存升级到二级缓存，
                    this.earlySingletonObjects.put(beanName, singletonObject);
                    // 三级缓存中移除
                    this.singletonFactories.remove(beanName);
                }
            }
        }
    }
    // 返回bean
    return singletonObject;
}
```
```java
public Object getSingleton(String beanName, ObjectFactory<?> singletonFactory) {  
    Assert.notNull(beanName, "Bean name must not be null");  
    synchronized (this.singletonObjects) {        
        // 从一级缓存中获取对应bean 
        Object singletonObject = this.singletonObjects.get(beanName);      
        // 若bean不存在 
        if (singletonObject == null) {  
            // 当前正在销毁bean，不能创建
            if (this.singletonsCurrentlyInDestruction) {  
                throw new BeanCreationNotAllowedException(beanName, "Singleton bean creation not allowed while singletons of this factory are in destruction " + "(Do not request a bean from a BeanFactory in a destroy method implementation!)");  
            }  
            if (logger.isDebugEnabled()) {  
                logger.debug("Creating shared instance of singleton bean '" + beanName + "'");  
            }
            // 创建前检查，记录正在加载状态     
            beforeSingletonCreation(beanName); 
            boolean newSingleton = false;
            boolean recordSuppressedExceptions = (this.suppressedExceptions == null);  
            // 如果当前没有异常，初始化异常集合
            if (recordSuppressedExceptions) {  
                this.suppressedExceptions = new LinkedHashSet<>();  
            }  
            try {
                // 执行匿名内部类方法 
                singletonObject = singletonFactory.getObject(); 
                newSingleton = true; 
            }  
            catch(IllegalStateException ex){  
                // 执行getObject方法创建bean
                singletonObject = this.singletonObjects.get(beanName);  
                if (singletonObject == null) {  
                    throw ex;  
                }  
            } catch(BeanCreationException ex){  
                if (recordSuppressedExceptions) {  
                    or (Exception suppressedException : this.suppressedExceptions) { 
                        ex.addRelatedCause(suppressedException);  
                    }  
                }  
                throw ex;  
            } finally{  
                if (recordSuppressedExceptions) {  
                    this.suppressedExceptions = null;  
                } 
                // 单例bean创建完成后，容器移除bean
                afterSingletonCreation(beanName);  
            }  
            // newSingleton为true时，表示bean创建成功  
            // 判断是否为新的完成整bean
            if (newSingleton) {            
                // 将bean存入一级缓存中
                addSingleton(beanName, singletonObject);  
            }  
        } 
        return singletonObject;  
    }  
}
```
<a name="riMGx"></a>
#### 添加与清理缓存
```java
// 将bean放入一级缓存切清楚二级、三级缓存
protected void addSingleton(String beanName, Object singletonObject) {
    synchronized (this.singletonObjects) {xw
                                          // 添加到一级缓存中 
                                          this.singletonObjects.put(beanName, singletonObject); 
                                          // 从三级缓存中移除 
                                          this.singletonFactories.remove(beanName);
                                          // 从二级缓存中移除 
                                          this.earlySingletonObjects.remove(beanName);
                                          // 放入已注册的单利池中
                                          this.registeredSingletons.add(beanName);
                                         }
}
```
```java
// 添加到三级缓存
protected void addSingletonFactory(String beanName, ObjectFactory<?> singletonFactory) {
    synchronized (this.singletonObjects) {
        // 若一级缓存不存在bean实例
        if (!this.singletonObjects.containsKey(beanName)) {
            // 添加到三级缓存
            this.singletonFactories.put(beanName, singletonFactory);
            // 从第二级缓存删除
            this.earlySingletonObjects.remove(beanName);
            // 放入已注册的单例池里
            this.registeredSingletons.add(beanName);
        }
    }
```
<a name="Whl4M"></a>
## 4、总结
Spring 循环依赖是通过map缓存进行处理的，其中包括一级、二级、三级缓存，作用如下：<br />1、一级缓存`SingletonObjects`实例化、初始化实例。<br />2、二级缓存`EarlySingletonObjects`存放的是早期的 Bean ，半成品还未初始化的 bean。<br />3、三级缓存`SingletonFactories`是一个对象工厂，用于创建对象，然后放入到二级缓存中。同时对象如果存在 Aop 代理，那么返回的对象就是代理对象。
