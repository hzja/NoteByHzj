JavaSpring<br />关于循环依赖的问题：

1. **什么是循环依赖？**
2. **如何检测是否存在循环依赖？**
3. **如何解决循环依赖？**
4. **多例的情况下，循环依赖问题为什么无法解决？**
5. **单例的情况下，虽然可以解决循环依赖，是否存在其他问题？**
6. **为什么采用三级缓存解决循环依赖？如果直接将早期bean丢到二级缓存可以么？**

下面来一个个突破。
<a name="hzDQ6"></a>
## 什么是循环依赖？
这个很好理解，多个bean之间相互依赖，形成了一个闭环。<br />比如：A依赖于B、B依赖于C、C依赖于A。<br />代码中表示：
```java
public class A{
    B b;
}
public class B{
    C c;
}
public class C{
    A a;
}
```
<a name="pQKI4"></a>
## 如何检测是否存在循环依赖？
检测循环依赖比较简单，使用一个列表来记录正在创建中的bean，bean创建之前，先去记录中看一下自己是否已经在列表中了，如果在，说明存在循环依赖，如果不在，则将其加入到这个列表，bean创建完毕之后，将其再从这个列表中移除。<br />源码方面来看一下，Spring创建单例bean时候，会调用下面方法
```java
protected void beforeSingletonCreation(String beanName) {
    if (!this.inCreationCheckExclusions.contains(beanName) && !this.singletonsCurrentlyInCreation.add(beanName)) {
        throw new BeanCurrentlyInCreationException(beanName);
    }
}
```
`singletonsCurrentlyInCreation`就是用来记录目前正在创建中的bean名称列表，`this.singletonsCurrentlyInCreation.add(beanName)`返回false，说明beanName已经在当前列表中了，此时会抛循环依赖的异常`BeanCurrentlyInCreationException`，这个异常对应的源码：
```java
public BeanCurrentlyInCreationException(String beanName) {
	super(beanName, "Requested bean is currently in creation: Is there an unresolvable circular reference?");
}
```
上面是单例bean检测循环依赖的源码，再来看看非单例bean的情况。<br />以prototype情况为例，源码位于`org.springframework.beans.factory.support.AbstractBeanFactory#doGetBean`方法中，将主要代码列出来看一下：
```java
//检查正在创建的bean列表中是否存在beanName，如果存在，说明存在循环依赖，抛出循环依赖的异常
if (isPrototypeCurrentlyInCreation(beanName)) {
    throw new BeanCurrentlyInCreationException(beanName);
}

//判断scope是否是prototype
if (mbd.isPrototype()) {
    Object prototypeInstance = null;
    try {
        //将beanName放入正在创建的列表中
        beforePrototypeCreation(beanName);
        prototypeInstance = createBean(beanName, mbd, args);
    }
    finally {
        //将beanName从正在创建的列表中移除
        afterPrototypeCreation(beanName);
    }
}
```
<a name="RMkYg"></a>
## Spring如何解决循环依赖的问题
Spring创建bean主要的几个步骤：

1. **步骤1：实例化bean，即调用构造器创建bean实例**
2. **步骤2：填充属性，注入依赖的bean，比如通过set方式、**`**@Autowired**`**注解的方式注入依赖的bean**
3. **步骤3：bean的初始化，比如调用init方法等。**

从上面3个步骤中可以看出，注入依赖的对象，有2种情况：

1. 通过步骤1中构造器的方式注入依赖
2. 通过步骤2注入依赖

先来看构造器的方式注入依赖的bean，下面两个bean循环依赖
```java
@Component
public class ServiceA {
    private ServiceB serviceB;

    public ServiceA(ServiceB serviceB) {
        this.serviceB = serviceB;
    }
}

@Component
public class ServiceB {
    private ServiceA serviceA;

    public ServiceB(ServiceA serviceA) {
        this.serviceA = serviceA;
    }
}
```
构造器的情况比较容易理解，实例化ServiceA的时候，需要有serviceB，而实例化ServiceB的时候需要有serviceA，构造器循环依赖是无法解决的，大家可以尝试一下使用编码的方式创建上面2个对象，是无法创建成功的！<br />再来看看非构造器的方式注入相互依赖的bean，以set方式注入为例，下面是2个单例的bean：serviceA和serviceB：
```java
@Component
public class ServiceA {
    private ServiceB serviceB;

    @Autowired
    public void setServiceB(ServiceB serviceB) {
        this.serviceB = serviceB;
    }
}


@Component
public class ServiceB {
    private ServiceA serviceA;

    @Autowired
    public void setServiceA(ServiceA serviceA) {
        this.serviceA = serviceA;
    }
}
```
如果采用硬编码的方式创建上面2个对象，过程如下：
```java
//创建serviceA
ServiceA serviceA = new ServiceA();
//创建serviceB
ServiceB serviceB = new ServiceB();
//将serviceA注入到serviceB中
serviceB.setServiceA(serviceA);
//将serviceB注入到serviceA中
serviceA.setServiceB(serviceB);
```
由于单例bean在Spring容器中只存在一个，所以Spring容器中肯定是有一个缓存来存放所有已创建好的单例bean；获取单例bean之前，可以先去缓存中找，找到了直接返回，找不到的情况下再去创建，创建完毕之后再将其丢到缓存中，可以使用一个map来存储单例bean，比如下面这个
```java
Map<String, Object> singletonObjects = new ConcurrentHashMap<>(256);
```
下面来看一下Spring中set方法创建上面2个bean的过程

1. Spring轮询准备创建2个bean：serviceA和serviceB
2. Spring容器发现`singletonObjects`中没有serviceA
3. 调用serviceA的构造器创建serviceA实例
4. serviceA准备注入依赖的对象，发现需要通过setServiceB注入serviceB
5. serviceA向Spring容器查找serviceB
6. Spring容器发现`singletonObjects`中没有serviceB
7. 调用serviceB的构造器创建serviceB实例
8. serviceB准备注入依赖的对象，发现需要通过setServiceA注入serviceA
9. serviceB向Spring容器查找serviceA
10. 此时又进入步骤2了

上面过程死循环了，怎么才能终结？<br />可以在第3步后加一个操作：将实例化好的serviceA丢到`singletonObjects`中，此时问题就解决了。<br />Spring中也采用类似的方式，稍微有点区别，上面使用了一个缓存，而Spring内部采用了3级缓存来解决这个问题，一起来细看一下。<br />3级缓存对应的代码：
```java
/** 第一级缓存：单例bean的缓存 */
private final Map<String, Object> singletonObjects = new ConcurrentHashMap<>(256);

/** 第二级缓存：早期暴露的bean的缓存 */
private final Map<String, Object> earlySingletonObjects = new HashMap<>(16);

/** 第三级缓存：单例bean工厂的缓存 */
private final Map<String, ObjectFactory<?>> singletonFactories = new HashMap<>(16);
```
下面来看Spring中具体的过程，一起来分析源码<br />开始的时候，获取serviceA，会调用下面代码
```java
// org.springframework.beans.factory.support.AbstractBeanFactory#doGetBean

protected <T> T doGetBean(final String name, @Nullable final Class<T> requiredType,
                          @Nullable final Object[] args, boolean typeCheckOnly) throws BeansException {
    //1.查看缓存中是否已经有这个bean了
    Object sharedInstance = getSingleton(beanName); //@1
    if (sharedInstance != null && args == null) {
        bean = getObjectForBeanInstance(sharedInstance, name, beanName, null);
    }else {
        //若缓存中不存在，准备创建这个bean
        if (mbd.isSingleton()) {
            //2.下面进入单例bean的创建过程
            sharedInstance = getSingleton(beanName, () -> {
                try {
                    return createBean(beanName, mbd, args);
                }
                catch (BeansException ex) {
                    throw ex;
                }
            });
            bean = getObjectForBeanInstance(sharedInstance, name, beanName, mbd);
        }
    }
    return (T) bean;
}
```
@1：查看缓存中是否已经有这个bean了，如下：
```java
public Object getSingleton(String beanName) {
    return getSingleton(beanName, true);
}
```
然后进入下面方法，会依次尝试从3级缓存中查找bean，注意下面的第2个参数，为ture的时候，才会从第3级中查找，否则只会查找1、2级缓存
```java
//allowEarlyReference:是否允许从三级缓存singletonFactories中通过getObject拿到bean
protected Object getSingleton(String beanName, boolean allowEarlyReference) {
    //1.先从一级缓存中找
    Object singletonObject = this.singletonObjects.get(beanName);
    if (singletonObject == null && isSingletonCurrentlyInCreation(beanName)) {
        synchronized (this.singletonObjects) {
            //2.从二级缓存中找
            singletonObject = this.earlySingletonObjects.get(beanName);
            if (singletonObject == null && allowEarlyReference) {
                //3.二级缓存中没找到 && allowEarlyReference为true的情况下,从三级缓存中找
                ObjectFactory<?> singletonFactory = this.singletonFactories.get(beanName);
                if (singletonFactory != null) {
                    //三级缓存返回的是一个工厂，通过工厂来获取创建bean
                    singletonObject = singletonFactory.getObject();
                    //将创建好的bean丢到二级缓存中
                    this.earlySingletonObjects.put(beanName, singletonObject);
                    //从三级缓存移除
                    this.singletonFactories.remove(beanName);
                }
            }
        }
    }
    return singletonObject;
}
```
刚开始，3个缓存中肯定是找不到的，会返回null，接着会执行下面代码准备创建serviceA
```java
if (mbd.isSingleton()) {
    sharedInstance = getSingleton(beanName, () -> { //@1
        try {
            return createBean(beanName, mbd, args);
        }
        catch (BeansException ex) {
            destroySingleton(beanName);
            throw ex;
        }
    });
}
```
@1：进入`getSingleton`方法，而`getSingleton`方法代码比较多，为了方便大家理解，无关的代码剔除了，如下：
```java
public Object getSingleton(String beanName, ObjectFactory<?> singletonFactory) {
    synchronized (this.singletonObjects) {
        Object singletonObject = this.singletonObjects.get(beanName);
        if (singletonObject == null) {
            //单例bean创建之前调用，将其加入正在创建的列表中，上面有提到过，主要用来检测循环依赖用的
            beforeSingletonCreation(beanName);
            boolean newSingleton = false;

            try {
                //调用工厂创建bean
                singletonObject = singletonFactory.getObject();//@1
                newSingleton = true;
            }
            finally {
                //单例bean创建之前调用,主要是将其从正在创建的列表中移除
                afterSingletonCreation(beanName);
            }
            if (newSingleton) {
                //将创建好的单例bean放入缓存中
                addSingleton(beanName, singletonObject);//@2
            }
        }
        return singletonObject;
    }
}
```
上面@1和@2是关键代码，先来看一下@1，这个是一个`ObjectFactory`类型的，从外面传入的，如下<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1689257237737-959aa097-ad97-4bd6-af00-2f0bf9622cd4.png#averageHue=%23fdfaf8&clientId=u147c8c8b-782b-4&from=paste&id=u2146a0bf&originHeight=434&originWidth=841&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue6c00efa-e391-4869-bb0e-c15b77ff7aa&title=)<br />红框中的`createBean`最终会调用下面这个方法
```
org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory#doCreateBean
```
其内部主要代码如下：
```java
BeanWrapper instanceWrapper = null;

if (instanceWrapper == null) {
    //通过反射调用构造器实例化serviceA
    instanceWrapper = createBeanInstance(beanName, mbd, args);
}
//变量bean：表示刚刚同构造器创建好的bean示例
final Object bean = instanceWrapper.getWrappedInstance();

//判断是否需要暴露早期的bean，条件为（是否是单例bean && 当前容器允许循环依赖 && bean名称存在于正在创建的bean名称清单中）
boolean earlySingletonExposure = (mbd.isSingleton() && this.allowCircularReferences &&
                                  isSingletonCurrentlyInCreation(beanName));
if (earlySingletonExposure) {
    //若earlySingletonExposure为true，通过下面代码将早期的bean暴露出去
    addSingletonFactory(beanName, () -> getEarlyBeanReference(beanName, mbd, bean));//@1
}
```
**这里需要理解一下什么是早期bean？**<br />**刚刚实例化好的bean就是早期的bean，此时bean还未进行属性填充，初始化等操作**<br />@1：通过`addSingletonFactory`用于将早期的bean暴露出去，主要是将其丢到第3级缓存中，代码如下：
```java
protected void addSingletonFactory(String beanName, ObjectFactory<?> singletonFactory) {
    Assert.notNull(singletonFactory, "Singleton factory must not be null");
    synchronized (this.singletonObjects) {
        //第1级缓存中不存在bean
        if (!this.singletonObjects.containsKey(beanName)) {
            //将其丢到第3级缓存中
            this.singletonFactories.put(beanName, singletonFactory);
            //后面的2行代码不用关注
            this.earlySingletonObjects.remove(beanName);
            this.registeredSingletons.add(beanName);
        }
    }
}
```
上面的方法执行之后，serviceA就被丢到第3级的缓存中了。<br />后续的过程serviceA开始注入依赖的对象，发现需要注入serviceB，会从容器中获取serviceB，而serviceB的获取又会走上面同样的过程实例化serviceB，然后将serviceB提前暴露出去，然后serviceB开始注入依赖的对象，serviceB发现自己需要注入serviceA，此时去容器中找serviceA，找serviceA会先去缓存中找，会执行`getSingleton("serviceA",true)`，此时会走下面代码：
```java
protected Object getSingleton(String beanName, boolean allowEarlyReference) {
    //1.先从一级缓存中找
    Object singletonObject = this.singletonObjects.get(beanName);
    if (singletonObject == null && isSingletonCurrentlyInCreation(beanName)) {
        synchronized (this.singletonObjects) {
            //2.从二级缓存中找
            singletonObject = this.earlySingletonObjects.get(beanName);
            if (singletonObject == null && allowEarlyReference) {
                //3.二级缓存中没找到 && allowEarlyReference为true的情况下,从三级缓存中找
                ObjectFactory<?> singletonFactory = this.singletonFactories.get(beanName);
                if (singletonFactory != null) {
                    //三级缓存返回的是一个工厂，通过工厂来获取创建bean
                    singletonObject = singletonFactory.getObject();
                    //将创建好的bean丢到二级缓存中
                    this.earlySingletonObjects.put(beanName, singletonObject);
                    //从三级缓存移除
                    this.singletonFactories.remove(beanName);
                }
            }
        }
    }
    return singletonObject;
}
```
上面的方法走完之后，serviceA会被放入二级缓存`earlySingletonObjects`中，会将serviceA返回，此时serviceB中的serviceA注入成功，serviceB继续完成创建，然后将自己返回给serviceA，此时serviceA通过set方法将serviceB注入。<br />serviceA创建完毕之后，会调用`addSingleton`方法将其加入到缓存中，这块代码如下：
```java
protected void addSingleton(String beanName, Object singletonObject) {
    synchronized (this.singletonObjects) {
        //将bean放入第1级缓存中
        this.singletonObjects.put(beanName, singletonObject);
        //将其从第3级缓存中移除
        this.singletonFactories.remove(beanName);
        //将其从第2级缓存中移除
        this.earlySingletonObjects.remove(beanName);
    }
}
```
到此，serviceA和serviceB之间的循环依赖注入就完成了。<br />下面捋一捋整个过程：

1. 从容器中获取serviceA
2. 容器尝试从3个缓存中找serviceA，找不到
3. 准备创建serviceA
4. 调用serviceA的构造器创建serviceA，得到serviceA实例，此时serviceA还未填充属性，未进行其他任何初始化的操作
5. 将早期的serviceA暴露出去：即将其丢到第3级缓存singletonFactories中
6. serviceA准备填充属性，发现需要注入serviceB，然后向容器获取serviceB
7. 容器尝试从3个缓存中找serviceB，找不到
8. 准备创建serviceB
9. 调用serviceB的构造器创建serviceB，得到serviceB实例，此时serviceB还未填充属性，未进行其他任何初始化的操作
10. 将早期的serviceB暴露出去：即将其丢到第3级缓存`singletonFactories`中
11. serviceB准备填充属性，发现需要注入serviceA，然后向容器获取serviceA
12. 容器尝试从3个缓存中找serviceA，发现此时serviceA位于第3级缓存中，经过处理之后，serviceA会从第3级缓存中移除，然后会存到第2级缓存中，然后将其返回给serviceB，此时serviceA通过serviceB中的setServiceA方法被注入到serviceB中
13. serviceB继续执行后续的一些操作，最后完成创建工作，然后会调用`addSingleton`方法，将自己丢到第1级缓存中，并将自己从第2和第3级缓存中移除
14. serviceB将自己返回给serviceA
15. serviceA通过setServiceB方法将serviceB注入进去
16. serviceB继续执行后续的一些操作，最后完成创建工作，然后会调用`addSingleton`方法，将自己丢到第1级缓存中，并将自己从第2和第3级缓存中移除
<a name="AFHIJ"></a>
## 循环依赖无法解决的情况
**只有单例的bean会通过三级缓存提前暴露来解决循环依赖的问题，而非单例的bean，每次从容器中获取都是一个新的对象，都会重新创建，所以非单例的bean是没有缓存的，不会将其放到三级缓存中。**<br />那就会有下面几种情况需要注意。<br />还是以2个bean相互依赖为例：serviceA和serviceB
<a name="zrzUk"></a>
### 情况1
<a name="rldXt"></a>
#### 条件
serviceA：多例<br />serviceB：多例
<a name="cLIDx"></a>
#### 结果
此时不管是任何方式都是无法解决循环依赖的问题，最终都会报错，因为每次去获取依赖的bean都会重新创建。
<a name="mPdBT"></a>
### 情况2
<a name="DoEVx"></a>
#### 条件
serviceA：单例<br />serviceB：多例
<a name="ynQ3Z"></a>
#### 结果
若使用构造器的方式相互注入，是无法完成注入操作的，会报错。<br />若采用set方式注入，所有bean都还未创建的情况下，若去容器中获取serviceB，会报错，为什么？来看一下过程：

1. 从容器中获取serviceB
2. serviceB由于是多例的，所以缓存中肯定是没有的
3. 检查serviceB是在正在创建的bean名称列表中，没有
4. 准备创建serviceB
5. 将serviceB放入正在创建的bean名称列表中
6. 实例化serviceB（由于serviceB是多例的，所以不会提前暴露，必须是单例的才会暴露）
7. 准备填充serviceB属性，发现需要注入serviceA
8. 从容器中查找serviceA
9. 尝试从3级缓存中找serviceA，找不到
10. 准备创建serviceA
11. 将serviceA放入正在创建的bean名称列表中
12. 实例化serviceA
13. 由于serviceA是单例的，将早期serviceA暴露出去，丢到第3级缓存中
14. 准备填充serviceA的属性，发现需要注入serviceB
15. 从容器中获取serviceB
16. 先从缓存中找serviceB，找不到
17. 检查serviceB是在正在创建的bean名称列表中，发现已经存在了，抛出循环依赖的异常

这个有演示的源码，位置：
```
com.javacode2018.lesson003.demo2.CircleDependentTest#test2
```
<a name="KoTpE"></a>
## 探讨：为什么需要用3级缓存
<a name="ysOMo"></a>
### 问题
**如果只使用2级缓存，直接将刚实例化好的bean暴露给二级缓存出是否可以否？**<br />先下结论：不行。
<a name="M8TME"></a>
### 原因
**这样做是可以解决：早期暴露给其他依赖者的bean和最终暴露的bean不一致的问题。**<br />若将刚刚实例化好的bean直接丢到二级缓存中暴露出去，如果后期这个bean对象被更改了，比如可能在上面加了一些拦截器，将其包装为一个代理了，那么暴露出去的bean和最终的这个bean就不一样的，将自己暴露出去的时候是一个原始对象，而自己最终却是一个代理对象，最终会导致被暴露出去的和最终的bean不是同一个bean的，将产生意向不到的效果，而三级缓存就可以发现这个问题，会报错。<br />下面通过代码来演示一下效果。
<a name="bYB0F"></a>
### 案例
下面来2个bean，相互依赖，通过set方法相互注入，并且其内部都有一个m1方法，用来输出一行日志。
<a name="oQHKk"></a>
#### Service1
```java
package com.javacode2018.lesson003.demo2.test3;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

@Component
public class Service1 {
    public void m1() {
        System.out.println("Service1 m1");
    }

    private Service2 service2;

    @Autowired
    public void setService2(Service2 service2) {
        this.service2 = service2;
    }

}
```
<a name="XPytB"></a>
#### Service2
```java
package com.javacode2018.lesson003.demo2.test3;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

@Component
public class Service2 {

    public void m1() {
        System.out.println("Service2 m1");
        this.service1.m1();//@1
    }

    private Service1 service1;

    @Autowired
    public void setService1(Service1 service1) {
        this.service1 = service1;
    }

    public Service1 getService1() {
        return service1;
    }
}
```
注意上面的@1，service2的m1方法中会调用service1的m1方法。
<a name="Ky6zQ"></a>
#### 需求
在service1上面加个拦截器，要求在调用service1的任何方法之前需要先输出一行日志
```
你好,service1
```
<a name="MLqPg"></a>
#### 实现
新增一个Bean后置处理器来对service1对应的bean进行处理，将其封装为一个代理暴露出去。
```java
package com.javacode2018.lesson003.demo2.test3;

import org.springframework.aop.MethodBeforeAdvice;
import org.springframework.aop.framework.ProxyFactory;
import org.springframework.beans.BeansException;
import org.springframework.beans.factory.config.BeanPostProcessor;
import org.springframework.lang.Nullable;
import org.springframework.stereotype.Component;

import java.lang.reflect.Method;

@Component
public class MethodBeforeInterceptor implements BeanPostProcessor {
    @Nullable
    @Override
    public Object postProcessAfterInitialization(Object bean, String beanName) throws BeansException {
        if ("service1".equals(beanName)) {
            //代理创建工厂，需传入被代理的目标对象
            ProxyFactory proxyFactory = new ProxyFactory(bean);
            //添加一个方法前置通知，会在方法执行之前调用通知中的before方法
            proxyFactory.addAdvice(new MethodBeforeAdvice() {
                @Override
                public void before(Method method, Object[] args, @Nullable Object target) throws Throwable {
                    System.out.println("你好,service1");
                }
            });
            //返回代理对象
            return proxyFactory.getProxy();
        }
        return bean;
    }
}
```
上面的`postProcessAfterInitialization`方法内部会在service1初始化之后调用，内部会对service1这个bean进行处理，返回一个代理对象，通过代理来访问service1的方法，访问service1中的任何方法之前，会先输出：你好，service1。<br />代码中使用了ProxyFactory，这块不熟悉的没关系，后面介绍aop的时候会细说。
<a name="QP3Cn"></a>
#### 来个配置类
```java
@ComponentScan
public class MainConfig3 {

}
```
<a name="w39Bh"></a>
#### 来个测试用例
```java
@Test
public void test3() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext();
    context.register(MainConfig3.class);
    context.refresh();
}
```
<a name="BATrk"></a>
#### 运行：报错了
```
org.springframework.beans.factory.BeanCurrentlyInCreationException: Error creating bean with name 'service1': Bean with name 'service1' has been injected into other beans [service2] in its raw version as part of a circular reference, but has eventually been wrapped. This means that said other beans do not use the final version of the bean. This is often the result of over-eager type matching - consider using 'getBeanNamesOfType' with the 'allowEagerInit' flag turned off, for example.

    at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.doCreateBean(AbstractAutowireCapableBeanFactory.java:624)
```
可以看出是AbstractAutowireCapableBeanFactory.java:624这个地方整出来的异常，将这块代码贴出来给大家看一下：
```java
if (earlySingletonExposure) {
    //@1
    Object earlySingletonReference = getSingleton(beanName, false);
    if (earlySingletonReference != null) {
        //@2
        if (exposedObject == bean) {
            exposedObject = earlySingletonReference;
        }
        //@3
        else if (!this.allowRawInjectionDespiteWrapping && hasDependentBean(beanName)) {
            String[] dependentBeans = getDependentBeans(beanName);
            Set<String> actualDependentBeans = new LinkedHashSet<>(dependentBeans.length);
            for (String dependentBean : dependentBeans) {
                if (!removeSingletonIfCreatedForTypeCheckOnly(dependentBean)) {
                    actualDependentBeans.add(dependentBean);
                }
            }
            if (!actualDependentBeans.isEmpty()) {
                throw new BeanCurrentlyInCreationException(beanName,
                                                           "Bean with name '" + beanName + "' has been injected into other beans [" +
                                                           StringUtils.collectionToCommaDelimitedString(actualDependentBeans) +
                                                           "] in its raw version as part of a circular reference, but has eventually been " +
                                                           "wrapped. This means that said other beans do not use the final version of the " +
                                                           "bean. This is often the result of over-eager type matching - consider using " +
                                                           "'getBeanNamesOfType' with the 'allowEagerInit' flag turned off, for example.");
            }
        }
    }
}
```
上面代码主要用来判断当有循环依赖的情况下，早期暴露给别人使用的bean是否和最终的bean不一样的情况下，会抛出一个异常。<br />再来通过代码级别的来解释上面代码：<br />@1：调用`getSingleton(beanName, false)`方法，这个方法用来从3个级别的缓存中获取bean，但是注意了，这个地方第二个参数是false，此时只会尝试从第1级和第2级缓存中获取bean，如果能够获取到，说明了什么？说明了第2级缓存中已经有这个bean了，而什么情况下第2级缓存中会有bean？说明这个bean从第3级缓存中已经被别人获取过，然后从第3级缓存移到了第2级缓存中，说明这个早期的bean被别人通过`getSingleton(beanName, true)`获取过<br />@2：这个地方用来判断早期暴露的bean和最终Spring容器对这个bean走完创建过程之后是否还是同一个bean，上面的service1被代理了，所以这个地方会返回false，此时会走到@3<br />@3：`allowRawInjectionDespiteWrapping`这个参数用来控制是否允许循环依赖的情况下，早期暴露给被人使用的bean在后期是否可以被包装，通俗点理解就是：是否允许早期给别人使用的bean和最终bean不一致的情况，这个值默认是false，表示不允许，也就是说暴露给别人的bean和最终的bean需要是一致的，给别人的是1，后面不能将其修改成2了，不一样了，还怎么用。<br />而上面代码注入到service2中的service1是早期的service1，而最终Spring容器中的service1变成一个代理对象了，早期的和最终的不一致了，而`allowRawInjectionDespiteWrapping`又是false，所以报异常了。<br />那么如何解决这个问题：<br />很简单，将`allowRawInjectionDespiteWrapping`设置为true就可以了，下面改一下代码如下：
```java
@Test
public void test4() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext();
    //创建一个BeanFactoryPostProcessor：BeanFactory后置处理器
    context.addBeanFactoryPostProcessor(beanFactory -> {
        if (beanFactory instanceof DefaultListableBeanFactory) {
            //将allowRawInjectionDespiteWrapping设置为true
            ((DefaultListableBeanFactory) beanFactory).setAllowRawInjectionDespiteWrapping(true);
        }
    });
    context.register(MainConfig3.class);
    context.refresh();

    System.out.println("容器初始化完毕");
}
```
上面代码中将`allowRawInjectionDespiteWrapping`设置为true了，是通过一个`BeanFactoryPostProcessor`来实现的，目前只需要知道`BeanFactoryPostProcessor`可以在bean创建之前用来干预`BeanFactory`的创建过程，可以用来修改`BeanFactory`中的一些配置。
<a name="qLIxd"></a>
#### 再次输出
```
容器初始化完毕
```
此时正常了，继续看看加在service1上的拦截器起效了没有，上面代码中加入下面代码：
```java
//获取service1
Service1 service1 = context.getBean(Service1.class);
//获取service2
Service2 service2 = context.getBean(Service2.class);

System.out.println("----A-----");
service2.m1(); //@1
System.out.println("----B-----");
service1.m1(); //@2
System.out.println("----C-----");
System.out.println(service2.getService1() == service1);
```
上面为了区分结果，使用了----格式的几行日志将输出结果分开了，来运行一下，输出：
```
容器初始化完毕
----A-----
Service2 m1
Service1 m1
----B-----
你好,service1
Service1 m1
----C-----
false
```
从输出中可以看出。<br />`service2.m1()`对应输出：
```
Service2 m1
Service1 m1
```
`service1.m1()`对应输出：
```
你好,service1
Service1 m1
```
而service2.m1方法中调用了service1.m1，这个里面拦截器没有起效，但是单独调用service1.m1方法，却起效了，说明service2中注入的service1不是代理对象，所以没有加上拦截器的功能，那是因为service2中注入的是早期的service1，注入的时候service1还不是一个代理对象，所以没有拦截器中的功能。<br />再看看最后一行输出为false，说明service2中的service1确实和Spring容器中的service1不是一个对象了。<br />ok，那么这种情况是不是很诧异，如何解决这个问题？<br />既然最终service1是一个代理对象，那么提前暴露出去的时候，注入到service2的时候，也必须得是个代理对象，需要确保给别人和最终是同一个对象。<br />这个怎么整？继续看暴露早期bean的源码，注意了下面是重点：
```java
addSingletonFactory(beanName, () -> getEarlyBeanReference(beanName, mbd, bean));
```
注意有个`getEarlyBeanReference`方法，来看一下这个方法是干什么的，源码如下：
```java
protected Object getEarlyBeanReference(String beanName, RootBeanDefinition mbd, Object bean) {
    Object exposedObject = bean;
    if (!mbd.isSynthetic() && hasInstantiationAwareBeanPostProcessors()) {
        for (BeanPostProcessor bp : getBeanPostProcessors()) {
            if (bp instanceof SmartInstantiationAwareBeanPostProcessor) {
                SmartInstantiationAwareBeanPostProcessor ibp = (SmartInstantiationAwareBeanPostProcessor) bp;
                exposedObject = ibp.getEarlyBeanReference(exposedObject, beanName);
            }
        }
    }
    return exposedObject;
}
```
从3级缓存中获取bean的时候，会调用上面这个方法来获取bean，这个方法内部会看一下容器中是否有`SmartInstantiationAwareBeanPostProcessor`这种处理器，然后会依次调用这种处理器中的`getEarlyBeanReference`方法，那么思路来了，可以自定义一个`SmartInstantiationAwareBeanPostProcessor`，然后在其`getEarlyBeanReference`中来创建代理不就可以了，聪明，来试试将`MethodBeforeInterceptor`代码改成下面这样：
```java
@Component
public class MethodBeforeInterceptor implements SmartInstantiationAwareBeanPostProcessor {
    @Override
    public Object getEarlyBeanReference(Object bean, String beanName) throws BeansException {
        if ("service1".equals(beanName)) {
            //代理创建工厂，需传入被代理的目标对象
            ProxyFactory proxyFactory = new ProxyFactory(bean);
            //添加一个方法前置通知，会在方法执行之前调用通知中的before方法
            proxyFactory.addAdvice(new MethodBeforeAdvice() {
                @Override
                public void before(Method method, Object[] args, @Nullable Object target) throws Throwable {
                    System.out.println("你好,service1");
                }
            });
            //返回代理对象
            return proxyFactory.getProxy();
        }
        return bean;
    }
}
```
对应测试用例
```java
@Test
public void test5() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig4.class);
    System.out.println("容器初始化完毕");

    //获取service1
    com.javacode2018.lesson003.demo2.test4.Service1  service1 = context.getBean(com.javacode2018.lesson003.demo2.test4.Service1.class);
    //获取service2
    com.javacode2018.lesson003.demo2.test4.Service2 service2 = context.getBean(com.javacode2018.lesson003.demo2.test4.Service2.class);

    System.out.println("----A-----");
    service2.m1(); //@1
    System.out.println("----B-----");
    service1.m1(); //@2
    System.out.println("----C-----");
    System.out.println(service2.getService1() == service1);
}
```
运行输出
```
容器初始化完毕
----A-----
Service2 m1
你好,service1
Service1 m1
----B-----
你好,service1
Service1 m1
----C-----
true
```
<a name="W67ir"></a>
## 单例bean解决了循环依赖，还存在什么问题？
循环依赖的情况下，由于注入的是早期的bean，此时早期的bean中还未被填充属性，初始化等各种操作，也就是说此时bean并没有被完全初始化完毕，此时若直接拿去使用，可能存在有问题的风险。
