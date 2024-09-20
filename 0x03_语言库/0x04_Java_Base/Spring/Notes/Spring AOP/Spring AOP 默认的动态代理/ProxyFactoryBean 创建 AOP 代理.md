JavaSpring
<a name="A7lzR"></a>
## AOP创建代理的方式主要分为2大类
<a name="ucf4i"></a>
### 手动方式
也称为手动的方式，需要通过硬编码一个个创建代理。
<a name="LgI5p"></a>
### 自动化的方式
也称为批量的方式，批量的方式用在Spring环境中，通过bean后置处理器来对符合条件的bean创建代理<br />手动的方式基本上是采用硬编码的方式，相对来说更灵活一些，可以脱离Spring环境使用，而自动化的方式主要用在Spring环境中，和Spring集成起来更容易一些，更强大一些。<br />AOP创建代理相关的类<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684286619218-c867aec5-1069-42cb-906a-7b3b17c0cbbb.png#averageHue=%23fcfcfb&clientId=u779f5308-367e-4&from=paste&id=uf599ef4e&originHeight=470&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7a85b7b7-c6f4-4e63-ae7d-71beaad5cfc&title=)<br />左边的`ProxyCreatorSupport`下面的都是手动的方式，有3个类。<br />右边的`AbstractAutoProxyCreator`下面挂的都是自动创建代理的方式，主要有5个实现类。
<a name="FVmmM"></a>
## 手动3种方式
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684286619372-a9aa7582-e020-40e0-91b9-5fc388cec9be.png#averageHue=%23fbfaef&clientId=u779f5308-367e-4&from=paste&id=u5b346d6b&originHeight=170&originWidth=640&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9fa1244b-623e-42a6-a808-26a4d91cc66&title=)
<a name="D8LZf"></a>
### `ProxyFactory`方式
这种是硬编码的方式，可以脱离Spring直接使用，用到的比较多，自动化方式创建代理中都是依靠`ProxyFactory`来实现的，所以这种方式的原理大家一定要了解。
<a name="qXgIk"></a>
### `AspectJProxyFactory`方式
AspectJ是一个面向切面的框架，是目前最好用，最方便的AOP框架，Spring将其集成进来了，通过Aspectj提供的一些功能实现aop代理非常方便。
<a name="ZuEpH"></a>
### `ProxyFactoryBean`方式
Spring环境中给指定的bean创建代理的一种方式，本文主要介绍这个。
<a name="tbBFi"></a>
## `ProxyFactoryBean`
这个类实现了一个接口`FactoryBean`。<br />`ProxyFactoryBean`就是通过`FactoryBean`的方式来给指定的bean创建一个代理对象。<br />创建代理，有3个信息比较关键：

1. 需要增强的功能，这个放在通知（Advice）中实现
2. 目标对象（target）：表示你需要给哪个对象进行增强
3. 代理对象（proxy）：将增强的功能和目标对象组合在一起，然后形成的一个代理对象，通过代理对象来访问目标对象，起到对目标对象增强的效果。

使用`ProxyFactoryBean`也是围绕着3部分来的，`ProxyFactoryBean`使用的步骤：

1. 创建`ProxyFactoryBean`对象
2. 通过`ProxyFactoryBean.setTargetName`设置目标对象的bean名称，目标对象是Spring容器中的一个bean
3. 通过`ProxyFactoryBean`。`setInterceptorNames`添加需要增强的通知
4. 将`ProxyFactoryBean`注册到Spring容器，假设名称为proxyBean
5. 从Spring查找名称为proxyBean的bean，这个bean就是生成好的代理对象

上案例。<br />来个类Service1
```java
public class Service1 {

    public void m1() {
        System.out.println("我是 m1 方法");
    }

    public void m2() {
        System.out.println("我是 m2 方法");
    }
}
```
需求
> 在Spring容器中注册上面这个类的bean，名称为service1，通过代理的方式来对这个bean进行增强，来2个通知
> 一个前置通知：在调用service1中的任意方法之前，输出一条信息：准备调用xxxx方法
> 一个环绕通知：复制统计所有方法的耗时。

下面是代码的实现
```java
import org.aopalliance.intercept.MethodInterceptor;
import org.aopalliance.intercept.MethodInvocation;
import org.springframework.aop.MethodBeforeAdvice;
import org.springframework.aop.framework.ProxyFactoryBean;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.lang.Nullable;

import java.lang.reflect.Method;

@Configuration
public class MainConfig1 {
    //注册目标对象
    @Bean
    public Service1 service1() {
        return new Service1();
    }

    //注册一个前置通知
    @Bean
    public MethodBeforeAdvice beforeAdvice() {
        MethodBeforeAdvice advice = new MethodBeforeAdvice() {
            @Override
            public void before(Method method, Object[] args, @Nullable Object target) throws Throwable {
                System.out.println("准备调用：" + method);
            }
        };
        return advice;
    }

    //注册一个后置通知
    @Bean
    public MethodInterceptor costTimeInterceptor() {
        MethodInterceptor methodInterceptor = new MethodInterceptor() {
            @Override
            public Object invoke(MethodInvocation invocation) throws Throwable {
                long starTime = System.nanoTime();
                Object result = invocation.proceed();
                long endTime = System.nanoTime();
                System.out.println(invocation.getMethod() + ",耗时(纳秒)：" + (endTime - starTime));
                return result;
            }
        };
        return methodInterceptor;
    }

    //注册ProxyFactoryBean
    @Bean
    public ProxyFactoryBean service1Proxy() {
        //1.创建ProxyFactoryBean
        ProxyFactoryBean proxyFactoryBean = new ProxyFactoryBean();
        //2.设置目标对象的bean名称
        proxyFactoryBean.setTargetName("service1");
        //3.设置拦截器的bean名称列表，此处2个（advice1和advice2)
        proxyFactoryBean.setInterceptorNames("beforeAdvice", "costTimeInterceptor");
        return proxyFactoryBean;
    }
}
```
下面启动Spring容器，并获取代理对象
```java
AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig1.class);
//获取代理对象，代理对象bean的名称为注册ProxyFactoryBean的名称，即：service1Proxy
Service1 bean = context.getBean("service1Proxy", Service1.class);
System.out.println("----------------------");
//调用代理的方法
bean.m1();
System.out.println("----------------------");
//调用代理的方法
bean.m2();
```
运行输出
```
----------------------
准备调用：public void com.javacode2018.aop.demo8.test1.Service1.m1()
我是 m1 方法
public void com.javacode2018.aop.demo8.test1.Service1.m1(),耗时(纳秒)：8680400
----------------------
准备调用：public void com.javacode2018.aop.demo8.test1.Service1.m2()
我是 m2 方法
public void com.javacode2018.aop.demo8.test1.Service1.m2(),耗时(纳秒)：82400
```
从输出中可以看到，目标对象service1已经被增强了。
<a name="PzAX8"></a>
## `ProxyFactoryBean`中的`interceptorNames`
`interceptorNames`用来指定拦截器的bean名称列表，常用的2种方式。

- 批量的方式
- 非批量的方式
<a name="lwAAm"></a>
### 批量的方式
<a name="N53hA"></a>
#### 使用方法
```java
proxyFactoryBean.setInterceptorNames("需要匹配的bean名称*");
```
需要匹配的bean名称后面跟上一个`*`，可以用来批量的匹配，如：`interceptor*`，此时Spring会从容器中找到下面2中类型的所有bean，bean名称以`interceptor`开头的将作为增强器
```
org.springframework.aop.Advisor
org.aopalliance.intercept.Interceptor
```
这个地方使用的时候需要注意，批量的方式注册的时候，如果增强器的类型不是上面2种类型的，比如下面3种类型通知，需要将其包装为`Advisor`才可以，而`MethodInterceptor`是`Interceptor`类型的，可以不用包装为`Advisor`类型的。
```
MethodBeforeAdvice（方法前置通知）
AfterReturningAdvice（方法后置通知）
ThrowsAdvice（异常通知）
```
下面来个案例感受一下。
<a name="n7kN0"></a>
#### 案例
下面批量注册2个增强器。
```java
import com.javacode2018.aop.demo8.test1.Service1;
import org.aopalliance.intercept.MethodInterceptor;
import org.aopalliance.intercept.MethodInvocation;
import org.springframework.aop.Advisor;
import org.springframework.aop.MethodBeforeAdvice;
import org.springframework.aop.framework.ProxyFactoryBean;
import org.springframework.aop.support.DefaultPointcutAdvisor;
import org.springframework.context.annotation.Bean;
import org.springframework.lang.Nullable;

import java.lang.reflect.Method;

public class MainConfig2 {
    //注册目标对象
    @Bean
    public Service1 service1() {
        return new Service1();
    }

    //定义一个增强器：interceptor1，内部是一个前置通知，需要将其包装为Advisor类型的
    @Bean
    public Advisor interceptor1() {
        MethodBeforeAdvice advice = new MethodBeforeAdvice() {
            @Override
            public void before(Method method, Object[] args, @Nullable Object target) throws Throwable {
                System.out.println("准备调用：" + method);
            }
        };
        DefaultPointcutAdvisor advisor = new DefaultPointcutAdvisor();
        advisor.setAdvice(advice);
        return advisor;
    }

    //定义一个增强器：interceptor2
    @Bean
    public MethodInterceptor interceptor2() {
        MethodInterceptor methodInterceptor = new MethodInterceptor() {
            @Override
            public Object invoke(MethodInvocation invocation) throws Throwable {
                long starTime = System.nanoTime();
                Object result = invocation.proceed();
                long endTime = System.nanoTime();
                System.out.println(invocation.getMethod() + ",耗时(纳秒)：" + (endTime - starTime));
                return result;
            }
        };
        return methodInterceptor;
    }

    //注册ProxyFactoryBean
    @Bean
    public ProxyFactoryBean service1Proxy() {
        //1.创建ProxyFactoryBean
        ProxyFactoryBean proxyFactoryBean = new ProxyFactoryBean();
        //2.设置目标对象的bean名称
        proxyFactoryBean.setTargetName("service1");
        //3.设置拦截器的bean名称列表，此处批量注册
        proxyFactoryBean.setInterceptorNames("interceptor*"); //@1
        return proxyFactoryBean;
    }
}
```
上面定义了2个增强器：<br />interceptor1：前置通知，包装为`Advisor`类型了<br />interceptor2：环绕通知，`MethodInterceptor`类型的<br />测试代码
```java
@Test
public void test2() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig2.class);
    //获取代理对象，代理对象bean的名称为注册ProxyFactoryBean的名称，即：service1Proxy
    Service1 bean = context.getBean("service1Proxy", Service1.class);
    System.out.println("----------------------");
    //调用代理的方法
    bean.m1();
    System.out.println("----------------------");
    //调用代理的方法
    bean.m2();
}
```
运行输出
```
----------------------
准备调用：public void com.javacode2018.aop.demo8.test1.Service1.m1()
我是 m1 方法
public void com.javacode2018.aop.demo8.test1.Service1.m1(),耗时(纳秒)：10326200
----------------------
准备调用：public void com.javacode2018.aop.demo8.test1.Service1.m2()
我是 m2 方法
public void com.javacode2018.aop.demo8.test1.Service1.m2(),耗时(纳秒)：52000
```
<a name="oqA7l"></a>
### 非批量的方式
<a name="jkTf7"></a>
#### 用法
非批量的方式，需要注册多个增强器，需明确的指定多个增强器的bean名称，多个增强器按照参数中指定的顺序执行，如
```java
proxyFactoryBean.setInterceptorNames("advice1","advice2");
```
advice1、advice2对应的bean类型必须为下面列表中指定的类型，类型这块比匹配的方式范围广一些
```
MethodBeforeAdvice（方法前置通知）
AfterReturningAdvice（方法后置通知）
ThrowsAdvice（异常通知）
org.aopalliance.intercept.MethodInterceptor（环绕通知）
org.springframework.aop.Advisor（顾问）
```
下面来个案例。
<a name="wvp3Q"></a>
#### 案例
这次给service1来3个通知：前置、环绕、后置
```java
import com.javacode2018.aop.demo8.test1.Service1;
import org.aopalliance.intercept.MethodInterceptor;
import org.aopalliance.intercept.MethodInvocation;
import org.springframework.aop.AfterReturningAdvice;
import org.springframework.aop.MethodBeforeAdvice;
import org.springframework.aop.framework.ProxyFactoryBean;
import org.springframework.context.annotation.Bean;
import org.springframework.lang.Nullable;

import java.lang.reflect.Method;

public class MainConfig3 {
    //注册目标对象
    @Bean
    public Service1 service1() {
        return new Service1();
    }

    //定义一个前置通知
    @Bean
    public MethodBeforeAdvice methodBeforeAdvice() {
        MethodBeforeAdvice methodBeforeAdvice = new MethodBeforeAdvice() {
            @Override
            public void before(Method method, Object[] args, @Nullable Object target) throws Throwable {
                System.out.println("准备调用：" + method);
            }
        };
        return methodBeforeAdvice;
    }

    //定义一个环绕通知
    @Bean
    public MethodInterceptor methodInterceptor() {
        MethodInterceptor methodInterceptor = new MethodInterceptor() {
            @Override
            public Object invoke(MethodInvocation invocation) throws Throwable {
                long starTime = System.nanoTime();
                Object result = invocation.proceed();
                long endTime = System.nanoTime();
                System.out.println(invocation.getMethod() + ",耗时(纳秒)：" + (endTime - starTime));
                return result;
            }
        };
        return methodInterceptor;
    }

    //定义一个后置通知
    @Bean
    public AfterReturningAdvice afterReturningAdvice() {
        AfterReturningAdvice afterReturningAdvice = new AfterReturningAdvice() {
            @Override
            public void afterReturning(@Nullable Object returnValue, Method method, Object[] args, @Nullable Object target) throws Throwable {
                System.out.println(method + "，执行完毕!");
            }
        };
        return afterReturningAdvice;
    }

    //注册ProxyFactoryBean
    @Bean
    public ProxyFactoryBean service1Proxy() {
        //1.创建ProxyFactoryBean
        ProxyFactoryBean proxyFactoryBean = new ProxyFactoryBean();
        //2.设置目标对象的bean名称
        proxyFactoryBean.setTargetName("service1");
        //3.设置拦截器的bean名称列表，此处批量注册
        proxyFactoryBean.setInterceptorNames("methodBeforeAdvice", "methodInterceptor", "afterReturningAdvice");
        return proxyFactoryBean;
    }
}
```
测试代码
```java
@Test
public void test3() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig3.class);
    //获取代理对象，代理对象bean的名称为注册ProxyFactoryBean的名称，即：service1Proxy
    Service1 bean = context.getBean("service1Proxy", Service1.class);
    System.out.println("----------------------");
    //调用代理的方法
    bean.m1();
    System.out.println("----------------------");
    //调用代理的方法
    bean.m2();
}
```
运行输出
```
----------------------
准备调用：public void com.javacode2018.aop.demo8.test1.Service1.m1()
我是 m1 方法
public void com.javacode2018.aop.demo8.test1.Service1.m1()，执行完毕!
public void com.javacode2018.aop.demo8.test1.Service1.m1(),耗时(纳秒)：12724100
----------------------
准备调用：public void com.javacode2018.aop.demo8.test1.Service1.m2()
我是 m2 方法
public void com.javacode2018.aop.demo8.test1.Service1.m2()，执行完毕!
public void com.javacode2018.aop.demo8.test1.Service1.m2(),耗时(纳秒)：76700
```
<a name="Z48Xy"></a>
## 源码解析
重点在于下面这个方法
```
org.springframework.aop.framework.ProxyFactoryBean#getObject
```
源码：
```java
public Object getObject() throws BeansException {
    //初始化advisor(拦截器)链
    initializeAdvisorChain();
    //是否是单例
    if (isSingleton()) {
        //创建单例代理对象
        return getSingletonInstance();
    }
    else {
        //创建多例代理对象
        return newPrototypeInstance();
    }
}
```
`initializeAdvisorChain`方法，用来初始化`advisor`(拦截器)链，是根据`interceptorNames`配置，找到Spring容器中符合条件的拦截器，将其放入创建aop代理的配置中
```java
private synchronized void initializeAdvisorChain() throws AopConfigException, BeansException {
    if (!ObjectUtils.isEmpty(this.interceptorNames)) {
        // 轮询 interceptorNames
        for (String name : this.interceptorNames) {
            //批量注册的方式：判断name是否以*结尾
            if (name.endsWith(GLOBAL_SUFFIX)) {
                //@1：从容器中匹配查找匹配的增强器，将其添加到aop配置中
                addGlobalAdvisor((ListableBeanFactory) this.beanFactory,
                                 name.substring(0, name.length() - GLOBAL_SUFFIX.length()));
            }
            else {
                //非匹配的方式：按照name查找bean，将其包装为Advisor丢到aop配置中
                Object advice;
                //从容器中查找bean
                advice = this.beanFactory.getBean(name);
                //@2：将advice添加到拦截器列表中
                addAdvisorOnChainCreation(advice, name);
            }
        }
    }
}
```
@1：`addGlobalAdvisor`批量的方式`Advisor`，看一下源码，比较简单
```java
/**
 * 添加所有全局拦截器和切入点，
 * 容器中所有类型为Advisor/Interceptor的bean，bean名称prefix开头的都会将其添加到拦截器链中
 */
private void addGlobalAdvisor(ListableBeanFactory beanFactory, String prefix) {
    //获取容器中所有类型为Advisor的bean
    String[] globalAdvisorNames =
        BeanFactoryUtils.beanNamesForTypeIncludingAncestors(beanFactory, Advisor.class);
    //获取容器中所有类型为Interceptor的bean
    String[] globalInterceptorNames =
        BeanFactoryUtils.beanNamesForTypeIncludingAncestors(beanFactory, Interceptor.class);
    List<Object> beans = new ArrayList<>(globalAdvisorNames.length + globalInterceptorNames.length);
    Map<Object, String> names = new HashMap<>(beans.size());
    for (String name : globalAdvisorNames) {
        Object bean = beanFactory.getBean(name);
        beans.add(bean);
        names.put(bean, name);
    }
    for (String name : globalInterceptorNames) {
        Object bean = beanFactory.getBean(name);
        beans.add(bean);
        names.put(bean, name);
    }
    //对beans进行排序，可以实现Ordered接口，排序规则：order asc
    AnnotationAwareOrderComparator.sort(beans);
    for (Object bean : beans) {
        String name = names.get(bean);
        //判断bean是否已prefix开头
        if (name.startsWith(prefix)) {
            //将其添加到拦截器链中
            addAdvisorOnChainCreation(bean, name);
        }
    }
}
```
@2：`addAdvisorOnChainCreation`
```java
private void addAdvisorOnChainCreation(Object next, String name) {
    //namedBeanToAdvisor用来将bean转换为advisor
    Advisor advisor = namedBeanToAdvisor(next);
    //将advisor添加到拦截器链中
    addAdvisor(advisor);
}
```
`namedBeanToAdvisor`方法
```java
private AdvisorAdapterRegistry advisorAdapterRegistry = new DefaultAdvisorAdapterRegistry();

private Advisor namedBeanToAdvisor(Object next) {
    //将对象包装为Advisor对象
    return this.advisorAdapterRegistry.wrap(next);
}
```
`advisorAdapterRegistry#wrap`方法将`adviceObject`包装为`Advisor`对象，代码如下，比较简单
```java
public Advisor wrap(Object adviceObject) throws UnknownAdviceTypeException {
    if (adviceObject instanceof Advisor) {
        return (Advisor) adviceObject;
    }
    if (!(adviceObject instanceof Advice)) {
        throw new UnknownAdviceTypeException(adviceObject);
    }
    Advice advice = (Advice) adviceObject;
    if (advice instanceof MethodInterceptor) {
        return new DefaultPointcutAdvisor(advice);
    }
    for (AdvisorAdapter adapter : this.adapters) {
        if (adapter.supportsAdvice(advice)) {
            return new DefaultPointcutAdvisor(advice);
        }
    }
    throw new UnknownAdviceTypeException(advice);
}
```
<a name="ZwxRv"></a>
## 总结

1. Spring中创建代理主要分为2种：手动方式和自动化的方式
2. 手动方式采用硬编码的方式，一次只能给一个目标对象创建代理对象，相对来说灵活一下，对开发者来说更灵活一些，通常可以独立Spring环境使用；自动化的方式主要在Spring环境中使用，通常是匹配的方式来为符合条件的目标bean创建代理，使用起来更简单一些
3. 本文介绍的`ProxyFactoryBean`用来在Spring环境中给指定的bean创建代理对象，用到的不是太多，大家可以作为了解即可
