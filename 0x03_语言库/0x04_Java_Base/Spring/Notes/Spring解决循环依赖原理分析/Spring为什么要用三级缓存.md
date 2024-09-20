JavaSpring<br />**本文未指明 bean scope 默认情况下，所有 bean 都是单例的，即 scope 是 singleton，即下面所有问题都是在单例的情况下分析的。**
<a name="fE3Jl"></a>
## 1、循环依赖相关问题
1、什么是循环依赖？<br />2、循环依赖的注入对象的 2 种方式：构造器的方式、setter 的方式<br />3、构造器的方式详解<br />4、Spring 是如何知道有循环依赖的？<br />5、setter 方式详解<br />6、需注意循环依赖注入的是半成品<br />7、为什么必须用三级缓存？
<a name="FnU2i"></a>
## 2、什么是循环依赖？
A 依赖于 B，B 依赖于 A，比如下面代码
```java
public class A {
    private B b;
}

public class B {
    private A a;
}
```
<a name="UzcSu"></a>
## 3、循环依赖注入对象的 2 种方式
<a name="lRJ0i"></a>
### 3.1、构造器的方式
通过构造器相互注入对方，代码如下
```java
public class A {
    private B b;

    public A(B b) {
        this.b = b;
    }
}

public class B {
    private A a;

    public B(A a) {
        this.a = a;
    }
}
```
<a name="jczR9"></a>
### 3.2、setter 的方式
通过 setter 方法注入对方，代码如下
```java
public class A {
    private B b;

    public B getB() {
        return b;
    }

    public void setB(B b) {
        this.b = b;
    }
}

public class B {
    private A a;

    public A getA() {
        return a;
    }

    public void setA(A a) {
        this.a = a;
    }
}
```
<a name="MdEH7"></a>
## 4、构造器的方式详解
<a name="cL2T0"></a>
### 4.1、构造器的方式知识点
1、构造器的方式如何注入？<br />2、循环依赖，构造器的方式，Spring 的处理过程是什么样的？<br />3、循环依赖构造器的方式案例代码解析
<a name="Q0sCd"></a>
### 4.2、构造器的方式如何注入？
再来看一下下面这 2 个类，相互依赖，通过构造器的方式相互注入对方。
```java
public class A {
    private B b;

    public A(B b) {
        this.b = b;
    }
}

public class B {
    private A a;

    public B(A a) {
        this.a = a;
    }
}
```
大家来思考一个问题：**2 个类都只能创建一个对象，大家试试着用硬编码的方式看看可以创建这 2 个类的对象么？**<br />一眼就看出来了，无法创建。<br />创建 A 的时候需要先有 B，而创建 B 的时候需要先有 A，导致无法创建成功。
<a name="ligib"></a>
### 4.3、循环依赖，构造器的方式，Spring 的处理过程是什么样的？
Spring 在创建 bean 之前，会将当前正在创建的 bean 名称放在一个列表中，这个列表就叫做 `singletonsCurrentlyInCreation`，用来记录正在创建中的 bean 名称列表，创建完毕之后，会将其从 `singletonsCurrentlyInCreation` 列表中移除，并且会将创建好的 bean 放到另外一个单例列表中，这个列表叫做 `singletonObjects`，下面看一下这两个集合的代码，如下：
```java
代码位于org.springframework.beans.factory.support.DefaultSingletonBeanRegistry类中

//用来存放正在创建中的bean名称列表
private final Set<String> singletonsCurrentlyInCreation =
Collections.newSetFromMap(new ConcurrentHashMap<>(16));

//用来存放已经创建好的单例bean，key为bean名称，value为bean的实例
private final Map<String, Object> singletonObjects = new ConcurrentHashMap<>(256);
```
下面来看下面 2 个 bean 的创建过程
```java
@Compontent
public class A {
    private B b;

    public A(B b) {
        this.b = b;
    }
}

@Compontent
public class B {
    private A a;

    public B(A a) {
        this.a = a;
    }
}
```
过程如下<br />1、从`singletonObjects`查看是否有a，此时没有<br />2、准备创建a<br />3、判断a是否在`singletonsCurrentlyInCreation`列表，此时明显不在，则将a加入`singletonsCurrentlyInCreation`列表<br />4、调用a的构造器A(B b)创建A<br />5、Spring发现A的构造器需要用到b<br />6、则向Spring容器查找b，从`singletonObjects`查看是否有b，此时没有<br />7、Spring准备创建b<br />8、判断b是否在`singletonsCurrentlyInCreation`列表，此时明显不在，则将b加入`singletonsCurrentlyInCreation`列表<br />9、调用b的构造器B(A a)创建b<br />10、Spring发现B的构造器需要用到a，则向Spring容器查找a<br />11、则向Spring容器查找a，从`singletonObjects`查看是否有a，此时没有<br />12、准备创建a<br />13、判断a是否在`singletonsCurrentlyInCreation`列表，上面第3步中a被放到了这个列表，此时a在这个列表中，走到这里了，说明a已经存在创建列表中了，此时程序又来创建a，说明这么一直走下去会死循环，此时Spring会弹出异常，终止bean的创建操作。
<a name="b5tu2"></a>
### 4.4、通过这个过程，得到了 2 个结论
1、循环依赖如果是构造器的方式，bean 无法创建成功，这个前提是 bean 都是单例的，bean 如果是多例的，大家自己可以分析分析。<br />2、**Spring 是通过 **`**singletonsCurrentlyInCreation**`** 这个列表来发现循环依赖的，这个列表会记录创建中的 bean，当发现 bean 在这个列表中存在了，说明有循环依赖，并且这个循环依赖是无法继续走下去的，如果继续走下去，会进入死循环，此时 Spring 会抛出异常让系统终止。**<br />判断循环依赖的源码在下面这个位置，`singletonsCurrentlyInCreation`是 Set 类型的，Set 的 add 方法返回 false，说明被 add 的元素在 Set 中已经存在了，然后会抛出循环依赖的异常。
```java
org.springframework.beans.factory.support.DefaultSingletonBeanRegistry#beforeSingletonCreation

private final Set<String> singletonsCurrentlyInCreation =
  Collections.newSetFromMap(new ConcurrentHashMap<>(16));

protected void beforeSingletonCreation(String beanName) {
    //bean名称已经存在创建列表中，则抛出循环依赖异常
    if (!this.inCreationCheckExclusions.contains(beanName) && !this.singletonsCurrentlyInCreation.add(beanName)) {
        //抛出循环依赖异常
        throw new BeanCurrentlyInCreationException(beanName);
    }
}

//循环依赖异常
public BeanCurrentlyInCreationException(String beanName) {
    super(beanName,
          "Requested bean is currently in creation: Is there an unresolvable circular reference?");
}
```
<a name="neaVx"></a>
### 4.5、Spring 构造器循环依赖案例
创建类 A
```java
package com.javacode2018.cycledependency.demo1;

import org.springframework.stereotype.Component;

@Component
public class A {
    private B b;

    public A(B b) {
        this.b = b;
    }
}
```
创建类 B
```java
package com.javacode2018.cycledependency.demo1;

import org.springframework.stereotype.Component;

@Component
public class B {
    private A a;

    public B(A a) {
        this.a = a;
    }
}
```
启动类
```java
package com.javacode2018.cycledependency.demo1;

import org.springframework.context.annotation.AnnotationConfigApplicationContext;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;

@Configuration
@ComponentScan
public class MainConfig {
    public static void main(String[] args) {
        AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext();
        context.register(MainConfig.class);
        //刷新容器上下文，触发单例bean创建
        context.refresh();
        //关闭上下文
        context.close();
    }
}
```
运行上面的 main 方法，产生了异常，部分异常信息如下，说明创建 beana的时候出现了循环依赖，导致创建 bean 无法继续进行，以后大家遇到这个错误了，应该可以很快定位到问题了。
```
Caused by: org.springframework.beans.factory.BeanCurrentlyInCreationException: Error creating bean with name 'a': Requested bean is currently in creation: Is there an unresolvable circular reference?
 at org.springframework.beans.factory.support.DefaultSingletonBeanRegistry.beforeSingletonCreation(DefaultSingletonBeanRegistry.java:347)
```
<a name="ckeZV"></a>
## 5、setter 方式详解
再来看看 setter 的 2 个类的源码
```java
public class A {
    private B b;

    public B getB() {
        return b;
    }

    public void setB(B b) {
        this.b = b;
    }
}

public class B {
    private A a;

    public A getA() {
        return a;
    }

    public void setA(A a) {
        this.a = a;
    }
}
```
大家试试通过硬编码的方式来相互注入，很简单吧，如下面这样
```java
A a = new A();
B b = new B();
a.setB(b);
b.setA(a);
```
通过硬编码的方式可以搞成功的，Spring 肯定也可以搞成功，确实，setter 循环依赖，Spring 可以正常执行。<br />下面来看 Spring 中 setter 循环依赖注入的流程。
<a name="keStb"></a>
## 6、Spring 中 setter 循环依赖注入流程
Spring 在创建单例 bean 的过程中，会用到三级缓存，所以需要先了解三级缓存。
<a name="a1yd6"></a>
### 6.1、三级缓存是哪三级？
Spring 中使用了 3 个 map 来作为三级缓存，每一级对应一个 map

| 第几级缓存 | 对应的 map | 说明 |
| --- | --- | --- |
| 第 1 级 | `Map<String, Object> singletonObjects` | 用来存放已经完全创建好的单例 bean<br />beanName->bean 实例 |
| 第 2 级 | `Map<String, Object> earlySingletonObjects` | 用来存放早期的 bean<br />beanName->bean 实例 |
| 第 3 级 | `Map<String, ObjectFactory<?>> singletonFactories` | 用来存放单例 bean 的 `ObjectFactory`<br />beanName->`ObjectFactory` 实例 |

这 3 个 map 的源码位于`org.springframework.beans.factory.support.DefaultSingletonBeanRegistry`类中。
<a name="sMzd3"></a>
### 6.2、单例 bean 创建过程源码解析
代码入口
```
org.springframework.beans.factory.support.AbstractBeanFactory#doGetBean
```
<a name="LLkTF"></a>
#### **step1：**`**doGetBean**`
如下，这个方法首先会调用 `getSingleton` 获取 bean，如果可以获取到，就会直接返回，否则会执行创建 bean 的流程<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690127046653-f1dc163f-0b84-4b13-92d8-9cd335ca6943.png#averageHue=%23f8f6f2&clientId=u0f109dc5-70e0-4&from=paste&id=u0c211229&originHeight=462&originWidth=965&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ufc616435-405b-4510-bf47-4ab3099132d&title=)
<a name="Sl7Ll"></a>
#### **step2：**`**getSingleton(beanName, true)**`
源码如下，这个方法内部会调用`getSingleton(beanName, true)`获取 bean，注意第二个参数是true，这个表示是否可以获取早期的 bean，这个参数为 true，会尝试从三级缓存`singletonFactories`中获取 bean，然后将三级缓存中获取到的 bean 丢到二级缓存中。
```java
public Object getSingleton(String beanName) {
return getSingleton(beanName, true);
}

protected Object getSingleton(String beanName, boolean allowEarlyReference) {
    //从第1级缓存中获取bean
    Object singletonObject = this.singletonObjects.get(beanName);
    //第1级中没有,且当前beanName在创建列表中
    if (singletonObject == null && isSingletonCurrentlyInCreation(beanName)) {
        synchronized (this.singletonObjects) {
            //从第2级缓存汇总获取bean
            singletonObject = this.earlySingletonObjects.get(beanName);
            //第2级缓存中没有 && allowEarlyReference为true，也就是说2级缓存中没有找到bean且beanName在当前创建列表中的时候，才会继续想下走。
            if (singletonObject == null && allowEarlyReference) {
                //从第3级缓存中获取bean
                ObjectFactory<?> singletonFactory = this.singletonFactories.get(beanName);
                //第3级中有获取到了
                if (singletonFactory != null) {
                    //3级缓存汇总放的是ObjectFactory，所以会调用其getObject方法获取bean
                    singletonObject = singletonFactory.getObject();
                    //将3级缓存中的bean丢到第2级中
                    this.earlySingletonObjects.put(beanName, singletonObject);
                    //将bean从三级缓存中干掉
                    this.singletonFactories.remove(beanName);
                }
            }
        }
    }
    return singletonObject;
}
```
<a name="rmb3h"></a>
#### **step3：**`**getSingleton(String beanName, ObjectFactory<?> singletonFactory)**`
上面调用`getSingleton(beanName, true)`没有获取到 bean，所以会继续走 bean 的创建逻辑，会走到下面代码，如下<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690127046716-be0ed15f-e90a-475a-ae4c-f19b85019a97.png#averageHue=%23fafaf9&clientId=u0f109dc5-70e0-4&from=paste&id=u062a6b62&originHeight=495&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ucb338eab-bed9-4159-9bd8-325920d8e49&title=)<br />进入`getSingleton(String beanName, ObjectFactory<?> singletonFactory)`，源码如下，只留了重要的部分
```java
public Object getSingleton(String beanName, ObjectFactory<?> singletonFactory) {
    //从第1级缓存中获取bean，如果可以获取到，则自己返回
    Object singletonObject = this.singletonObjects.get(beanName);
    if (singletonObject == null) {
        //将beanName加入当前创建列表中
        beforeSingletonCreation(beanName);
        //①：创建单例bean
        singletonObject = singletonFactory.getObject();
        //将beanName从当前创建列表中移除
        afterSingletonCreation(beanName);
        //将创建好的单例bean放到1级缓存中,并将其从2、3级缓存中移除
        addSingleton(beanName, singletonObject);
    }
    return singletonObject;
}
```
注意代码①，会调用`singletonFactory.getObject()`创建单例 bean，回头看看`singletonFactory`这个变量的内容，如下图，可以看出主要就是调用`createBean`这个方法<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690127046759-a9016711-2df5-49da-9278-dd81546dafe2.png#averageHue=%23fcfbf9&clientId=u0f109dc5-70e0-4&from=paste&id=ue0cc0f10&originHeight=417&originWidth=800&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud71589a2-0608-4b0d-aa93-b92ba7463af&title=)<br />下面进入`createBean`方法，这个内部最终会调用`doCreateBean`来创建 bean，所以主要看`doCreateBean`。
<a name="Ib2I3"></a>
#### **step4：**`**doCreateBean**`
```java
protected Object doCreateBean(final String beanName, final RootBeanDefinition mbd, final @Nullable Object[] args)
        throws BeanCreationException {

    // ①：创建bean实例，通过反射实例化bean，相当于new X()创建bean的实例
    BeanWrapper instanceWrapper = createBeanInstance(beanName, mbd, args);

    // bean = 获取刚刚new出来的bean
    Object bean = instanceWrapper.getWrappedInstance();

    // ②：是否需要将早期的bean暴露出去，所谓早期的bean相当于这个bean就是通过new的方式创建了这个对象，但是这个对象还没有填充属性，所以是个半成品
    // 是否需要将早期的bean暴露出去，判断规则（bean是单例 && 是否允许循环依赖 && bean是否在正在创建的列表中）
    boolean earlySingletonExposure = (mbd.isSingleton() && this.allowCircularReferences &&
            isSingletonCurrentlyInCreation(beanName));

    if (earlySingletonExposure) {
        //③：调用addSingletonFactory方法，这个方法内部会将其丢到第3级缓存中，getEarlyBeanReference的源码大家可以看一下，内部会调用一些方法获取早期的bean对象，比如可以在这个里面通过aop生成代理对象
        addSingletonFactory(beanName, () -> getEarlyBeanReference(beanName, mbd, bean));
    }

    // 这个变量用来存储最终返回的bean
    Object exposedObject = bean;
    //填充属性，这里面会调用setter方法或者通过反射将依赖的bean注入进去
    populateBean(beanName, mbd, instanceWrapper);
    //④：初始化bean，内部会调用BeanPostProcessor的一些方法，对bean进行处理，这里可以对bean进行包装，比如生成代理
    exposedObject = initializeBean(beanName, exposedObject, mbd);


    //早期的bean是否被暴露出去了
    if (earlySingletonExposure) {
        /**
         *⑤：getSingleton(beanName, false)，注意第二个参数是false，这个为false的时候，
         * 只会从第1和第2级中获取bean，此时第1级中肯定是没有的（只有bean创建完毕之后才会放入1级缓存）
         */
        Object earlySingletonReference = getSingleton(beanName, false);
        /**
         * ⑥：如果earlySingletonReference不为空，说明第2级缓存有这个bean，二级缓存中有这个bean，说明了什么？
         * 大家回头再去看看上面的分析，看一下什么时候bean会被放入2级缓存?
         * （若 bean存在三级缓存中 && beanName在当前创建列表的时候，此时其他地方调用了getSingleton(beanName, false)方法，那么bean会从三级缓存移到二级缓存）
         */
        if (earlySingletonReference != null) {
            //⑥：exposedObject==bean，说明bean创建好了之后，后期没有被修改
            if (exposedObject == bean) {
                //earlySingletonReference是从二级缓存中获取的，二级缓存中的bean来源于三级缓存，三级缓存中可能对bean进行了包装，比如生成了代理对象
                //那么这个地方就需要将 earlySingletonReference 作为最终的bean
                exposedObject = earlySingletonReference;
            } else if (!this.allowRawInjectionDespiteWrapping && hasDependentBean(beanName)) {
                //回头看看上面的代码，刚开始exposedObject=bean，
                // 此时能走到这里，说明exposedObject和bean不一样了，他们不一样了说明了什么？
                // 说明initializeBean内部对bean进行了修改
                // allowRawInjectionDespiteWrapping（默认是false）：是否允许早期暴露出去的bean(earlySingletonReference)和最终的bean不一致
                // hasDependentBean(beanName)：表示有其他bean以利于beanName
                // getDependentBeans(beanName)：获取有哪些bean依赖beanName
                String[] dependentBeans = getDependentBeans(beanName);
                Set<String> actualDependentBeans = new LinkedHashSet<>(dependentBeans.length);
                for (String dependentBean : dependentBeans) {
                    //判断dependentBean是否已经被标记为创建了，就是判断dependentBean是否已经被创建了
                    if (!removeSingletonIfCreatedForTypeCheckOnly(dependentBean)) {
                        actualDependentBeans.add(dependentBean);
                    }
                }
                /**
                 *
                 * 能走到这里，说明早期的bean被别人使用了，而后面程序又将exposedObject做了修改
                 * 也就是说早期创建的bean是A，这个A已经被有些地方使用了，但是A通过initializeBean之后可能变成了B，比如B是A的一个代理对象
                 * 这个时候就坑了，别人已经用到的A和最终容器中创建完成的A不是同一个A对象了，那么使用过程中就可能存在问题了
                 * 比如后期对A做了增强（Aop），而早期别人用到的A并没有被增强
                 */
                if (!actualDependentBeans.isEmpty()) {
                    //弹出异常（早期给别人的bean和最终容器创建的bean不一致了，弹出异常）
                    throw new BeanCurrentlyInCreationException(beanName,"异常内容见源码。。。。。");
                }
            }
        }
    }

    return exposedObject;
}
```
上面的 step1~step4，大家要反复看几遍，下面这几个问题搞清楚之后，才可以继续向下看，不懂的结合源码继续看上面几个步骤<br />**1、什么时候 bean 被放入 3 级缓存？**<br />早期的 bean 被放入 3 级缓存<br />**2、什么时候 bean 会被放入 2 级缓存？**<br />当 beanX 还在创建的过程中，此时被加入当前 beanName 创建列表了，但是这个时候 bean 并没有被创建完毕（bean 被丢到一级缓存才算创建完毕），此时 bean 还是个半成品，这个时候其他 bean 需要用到 beanX，此时会从三级缓存中获取到 beanX，beanX 会从三级缓存中丢到 2 级缓存中。<br />**3、什么时候 bean 会被放入 1 级缓存？**<br />bean 实例化完毕，初始化完毕，属性注入完毕，bean 完全组装完毕之后，才会被丢到 1 级缓存。<br />**4、populateBean 方法是干什么的？**<br />填充属性的，比如注入依赖的对象。
<a name="q53d3"></a>
### 6.3、下面来看 A、B 类 setter 循环依赖的创建过程
1、`getSingleton("a", true)` 获取 a：会依次从 3 个级别的缓存中找 a，此时 3 个级别的缓存中都没有 a<br />2、将 a 丢到正在创建的 beanName 列表中（`Set<String> singletonsCurrentlyInCreation`）<br />3、实例化 `a：A a = new A();`这个时候 a 对象是早期的 a，属于半成品<br />4、将早期的 a 丢到三级缓存中（`Map<String, ObjectFactory<?> > singletonFactories`）<br />5、调用 `populateBean` 方法，注入依赖的对象，发现 setB 需要注入 b<br />6、调用 `getSingleton("b", true)` 获取 b：会依次从 3 个级别的缓存中找 a，此时 3 个级别的缓存中都没有 b<br />7、将 b 丢到正在创建的 beanName 列表中<br />8、实例化 b：`B b = new B();`这个时候 b 对象是早期的 b，属于半成品<br />9、将早期的 b 丢到三级缓存中（`Map<String, ObjectFactory<?> > singletonFactories`）<br />10、调用 `populateBean` 方法，注入依赖的对象，发现 setA 需要注入 a<br />11、调用 `getSingleton("a", true)` 获取 a：此时 a 会从第 3 级缓存中被移到第 2 级缓存，然后将其返回给 b 使用，此时 a 是个半成品（属性还未填充完毕）<br />12、b 通过 setA 将 11 中获取的 a 注入到 b 中<br />13、b 被创建完毕，此时 b 会从第 3 级缓存中被移除，然后被丢到 1 级缓存<br />14、b 返回给 a，然后 b 被通过 A 类中的 setB 注入给 a<br />15、a 的 populateBean 执行完毕，即：完成属性填充，到此时 a 已经注入到 b 中了<br />16、调用`a= initializeBean("a", a, mbd)`对 a 进行处理，这个内部可能对 a 进行改变，有可能导致 a 和原始的 a 不是同一个对象了<br />17、调用`getSingleton("a", false)`获取 a，注意这个时候第二个参数是 false，这个参数为 false 的时候，只会从前 2 级缓存中尝试获取 a，而 a 在步骤 11 中已经被丢到了第 2 级缓存中，所以此时这个可以获取到 a，这个 a 已经被注入给 b 了<br />18、此时判断注入给 b 的 a 和通过`initializeBean`方法产生的 a 是否是同一个 a，不是同一个，则弹出异常<br />**从上面的过程中可以得到一个非常非常重要的结论**<br />**当某个 bean 进入到 2 级缓存的时候，说明这个 bean 的早期对象被其他 bean 注入了，也就是说，这个 bean 还是半成品，还未完全创建好的时候，已经被别人拿去使用了，所以必须要有 3 级缓存，2 级缓存中存放的是早期的被别人使用的对象，如果没有 2 级缓存，是无法判断这个对象在创建的过程中，是否被别人拿去使用了。**<br />3 级缓存是为了解决一个非常重要的问题：早期被别人拿去使用的 bean 和最终成型的 bean 是否是一个 bean，如果不是同一个，则会产生异常，所以以后面试的时候被问到为什么需要用到 3 级缓存的时候，只需要这么回答就可以了：**三级缓存是为了判断循环依赖的时候，早期暴露出去已经被别人使用的 bean 和最终的 bean 是否是同一个 bean，如果不是同一个则弹出异常，如果早期的对象没有被其他 bean 使用，而后期被修改了，不会产生异常，如果没有三级缓存，是无法判断是否有循环依赖，且早期的 bean 被循环依赖中的 bean 使用了。**<br />Spring 容器默认是不允许早期暴露给别人的 bean 和最终的 bean 不一致的，但是这个配置可以修改，而修改之后存在很大的分享，所以不要去改，通过下面这个变量控制
```java
org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory#allowRawInjectionDespiteWrapping

private boolean allowRawInjectionDespiteWrapping = false;
```
<a name="Q2IYD"></a>
### 6.4、模拟 `BeanCurrentlyInCreationException` 异常
**来个登录接口 ILogin**
```java
package com.javacode2018.cycledependency.demo2;

//登录接口
public interface ILogin {
}
```
来 2 个实现类<br />**LoginA**<br />这个上面加上`@Component`注解，且内部需要注入X
```java
package com.javacode2018.cycledependency.demo2;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

@Component
public class LoginA implements ILogin {
    @Autowired
    private X x;

    public X getX() {
        return x;
    }

    public void setX(X x) {
        this.x = x;
    }
}
```
**LoginC**，不需要 Spring 来管理
```java
package com.javacode2018.cycledependency.demo2;

//代理
public class LoginC implements ILogin {

    private ILogin target;

    public LoginC(ILogin target) {
        this.target = target;
    }
}
```
**X 类**，有`@Component`，且需要注入 Ilogin 对象，这个地方会注入 LoginA，此时 LoginA 和 X 会参数循环依赖
```java
package com.javacode2018.cycledependency.demo2;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

@Component
public class X {

    @Autowired
    private ILogin login;

    public ILogin getLogin() {
        return login;
    }

    public void setLogin(ILogin login) {
        this.login = login;
    }

}
```
添加一个 `BeanPostProcessor` 类，实现`postProcessAfterInitialization`方法，这个方法发现 bean 是 loginA 的时候，将其包装为 LoginC 返回，这个方法会在 bean 创建的过程中调用`initializeBean`时候被调用
```java
package com.javacode2018.cycledependency.demo2;

import org.springframework.beans.BeansException;
import org.springframework.beans.factory.config.BeanPostProcessor;
import org.springframework.stereotype.Component;

@Component
public class MyBeanPostProcessor implements BeanPostProcessor {
    @Override
    public Object postProcessAfterInitialization(Object bean, String beanName) throws BeansException {
        if (beanName.equals("loginA")) {
            //loginA实现了ILogin
            return new LoginC((ILogin) bean);
        } else {
            return bean;
        }
    }
}
```
**Spring 配置类**
```java
package com.javacode2018.cycledependency.demo2;

import org.springframework.context.annotation.AnnotationConfigApplicationContext;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;

@Configuration
@ComponentScan
public class MainConfig {
    public static void main(String[] args) {
        AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext();
        context.register(MainConfig.class);
        context.refresh();
        context.close();
    }
}
```
运行输出，产生了`BeanCurrentlyInCreationException`异常，是因为注入给 x 的是 LoginA 这个类的对象，而最后容器中 beanname：loginA 对应的是 LoginC 了，导致注入给别人的对象和最终的对象不一致了，产生了异常。
```
Exception in thread "main" org.springframework.beans.factory.BeanCurrentlyInCreationException: Error creating bean with name 'loginA': Bean with name 'loginA' has been injected into other beans [x] in its raw version as part of a circular reference, but has eventually been wrapped. This means that said other beans do not use the final version of the bean. This is often the result of over-eager type matching - consider using 'getBeanNamesForType' with the 'allowEagerInit' flag turned off, for example.
```
<a name="ovzLN"></a>
## 7、案例：若只使用 2 级缓存会产生什么后果？
下面来个案例，通过在源码中设置断点的方式，来模拟二级缓存产生的后果。
<a name="rzhQr"></a>
### 添加 A 类
希望 loginA 在 A 类之前被创建好，所以这里用到了`@DependsOn` 注解。
```java
package com.javacode2018.cycledependency.demo3;

import org.springframework.context.annotation.DependsOn;
import org.springframework.stereotype.Component;

@Component
@DependsOn("loginA") //类A依赖于loginA，但是又不想通过属性注入的方式强依赖
public class A {

}
```
<a name="uBERv"></a>
### 接口 ILogin
```java
package com.javacode2018.cycledependency.demo3;

//登录接口
public interface ILogin {
}
```
来 2 个实现类，LoginA 需要 Spring 管理，LoginC 不需要 Spring 管理。
<a name="a1XkO"></a>
### LoginA
```java
package com.javacode2018.cycledependency.demo3;

import org.springframework.stereotype.Component;

@Component
public class LoginA implements ILogin {

}
```
<a name="q1s8n"></a>
### LoginC
```java
package com.javacode2018.cycledependency.demo3;

//代理
public class LoginC implements ILogin {

    private ILogin target;

    public LoginC(ILogin target) {
        this.target = target;
    }
}
```
<a name="j05AP"></a>
### `MyBeanPostProcessor`
负责将 loginA 包装为 LoginC
```java
package com.javacode2018.cycledependency.demo3;

import org.springframework.beans.BeansException;
import org.springframework.beans.factory.config.BeanPostProcessor;
import org.springframework.stereotype.Component;

@Component
public class MyBeanPostProcessor implements BeanPostProcessor {
    @Override
    public Object postProcessAfterInitialization(Object bean, String beanName) throws BeansException {
        if (beanName.equals("loginA")) {
            //loginA实现了ILogin
            return new LoginC((ILogin) bean);
        } else {
            return bean;
        }
    }
}
```
<a name="YbnSd"></a>
### 启动类 MainConfig
```java
package com.javacode2018.cycledependency.demo3;

import org.springframework.context.annotation.AnnotationConfigApplicationContext;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;

@Configuration
@ComponentScan
public class MainConfig {
    public static void main(String[] args) {
        AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext();
        context.register(MainConfig.class);
        context.refresh();
        context.close();
    }
}
```
<a name="oB77W"></a>
### 下面模拟只使用二级缓存的情况
在 bean 被放到三级缓存之后，下面的一行代码处设置断点，操作如下<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690127046794-e151ce23-f8de-4591-93ea-51ea7e84b09a.png#averageHue=%23f9f7f4&clientId=u0f109dc5-70e0-4&from=paste&id=u2413bb92&originHeight=533&originWidth=1069&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubfd6c328-62c5-4ad5-b31a-35bedfc4ff5&title=)<br />会弹出一个框，然后填入下面配置，这个配置表示满足条件的时候，这个断点才会起效<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690127046603-16bb646e-f5ad-481c-bb04-59edd9c5b742.png#averageHue=%23ecebe9&clientId=u0f109dc5-70e0-4&from=paste&id=ua60e5e4e&originHeight=263&originWidth=450&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u48ef8ffe-29b6-41da-afb3-1076cae01bd&title=)
<a name="CxkBw"></a>
### debug 方式运行程序
走到了这个断点的位置，此时 loginA 已经被放到第 3 级缓存中，此时如果调用`this.getSingleton(beanName,true)`，loginA 会从第 3 级缓存移到第 3 级，这个时候就相当于只有 2 级缓存了，操作如下<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690127047434-4eb9cea0-4995-4f36-b075-5bc692e3216f.png#averageHue=%23f7f6f2&clientId=u0f109dc5-70e0-4&from=paste&id=ua2d4dd31&originHeight=340&originWidth=790&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u89e7abdb-ddef-4ffb-a8af-66ab8dee2c1&title=)<br />点击下面的按钮，会弹出一个窗口，可以在窗口中执行代码，执行`this.getSingleton(beanName,true)`，即将loginA从三级缓存放到 2 级缓存，这样相当于没有 3 级缓存了。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690127047817-e462389d-d59c-4d88-acda-3a5227b2fd7e.png#averageHue=%23f8f1d1&clientId=u0f109dc5-70e0-4&from=paste&id=ud03212f6&originHeight=358&originWidth=699&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf5d9a920-9778-41e2-9de5-e7c27a5ff6f&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690127047766-86e6fdae-efdc-44ec-9f0d-fb1f3660cc9f.png#averageHue=%23f6f5f5&clientId=u0f109dc5-70e0-4&from=paste&id=uf9ea6dbb&originHeight=498&originWidth=608&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8538f27a-9ea0-440a-8734-b458c3cc04b&title=)<br />运行结果，最终也产生了`BeanCurrentlyInCreationException`异常，实际上这个程序并没有出现循环依赖的情况，但是如果只用了二级缓存，也出现了早期被暴露的 bean 和最终的 bean 不一致的问题所参数的异常。
```
Exception in thread "main" org.springframework.beans.factory.BeanCurrentlyInCreationException
```
这个程序如果不进行干预，直接运行，是可以正常运行的，只有在 3 级缓存的情况才可以正常运行。
