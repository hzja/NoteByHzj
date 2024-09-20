JavaSpring<br />使用Spring的过程中，可能需要对大量bean创建代理，比如需拦截所有的service的方法，打印耗时日志，对大量service bean做权限校验，做事务处理等等，这些功能都可以通过aop的方式来实现，若采用硬编码的方式一个个创建，那是相当难受的事情。<br />Spring中提供了批量的方式，为容器中符合条件的bean，自动创建代理对象，也就是这里要说的`@EnableAspectJAutoProxy`。
<a name="ZWENh"></a>
## 1、`@EnableAspectJAutoProxy`自动为bean创建代理对象
`@EnableAspectJAutoProxy`可以自动为Spring容器中符合条件的bean创建代理对象，`@EnableAspectJAutoProxy`需要结合`@Aspect`注解一起使用。用法比较简单，下面通过案例来看一下。<br />先在com.javacode2018.aop.demo11.test1包中定义2个bean<br />UserService bean
```java
import org.springframework.stereotype.Component;

@Component
public class UserService {
    public void say(){
        System.out.println("我是UserService");
    }
}
```
CarService bean
```java
import org.springframework.stereotype.Component;

@Component
public class CarService {
    public void say() {
        System.out.println("我是CarService");
    }
}
```
通过Aspect来定义一个前置通知，需要拦截上面2个bean的所有方法，在方法执行之前输出一行日志
```java
import org.aspectj.lang.JoinPoint;
import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Before;
import org.aspectj.lang.annotation.Pointcut;
import org.springframework.stereotype.Component;

@Component //@1
@Aspect //@2
public class Aspect1 {

    @Pointcut("execution(* com.javacode2018.aop.demo11.test1..*(..))") //@3
    public void pc() {
    }

    @Before("com.javacode2018.aop.demo11.test1.Aspect1.pc()") //@4
    public void before(JoinPoint joinPoint) {
        System.out.println("我是前置通知,target:" + joinPoint.getTarget()); //5
    }
}
```
Aspect1中有4个关键信息<br />@1：使用 `@Component` 将这个类注册到Spring容器；<br />@2：使用 `@Aspect` 标注着是一个 AspectJ 来定义通知的配置类；<br />@3：定义切入点，目前的配置，会拦截test1包及其子包中所有类的所有方法，而CarService和UserService刚好满足，所以会被拦截；<br />@4：定义一个前置通知，这个通知会对@3定义的切入点起效；<br />@5：目标方法执行执行，输出一行日志；<br />下面来一个Spring配置类
```java
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.EnableAspectJAutoProxy;

@ComponentScan //@1
@EnableAspectJAutoProxy //@2
public class MainConfig1 {
}
```
@1：`@ComponentScan` 注解的作用会扫描当前包中的类，将标注有 `@Component` 的类注册到Spring容器；<br />@2：`@EnableAspectJAutoProxy` 这个注解比较关键，用来启用自动代理的创建，简单点理解：会找到容器中所有标注有`@Aspect`注解的bean以及`Advisor`类型的bean，会将他们转换为`Advisor`集合，Spring会通过`Advisor`集合对容器中满足切入点表达式的bean生成代理对象，整个都是Spring容器启动的过程中自动完成的，原理稍后介绍。<br />下面来测试用例代码，启动Spring容器，加载配置类，验证
```java
import com.javacode2018.aop.demo11.test1.CarService;
import com.javacode2018.aop.demo11.test1.MainConfig1;
import com.javacode2018.aop.demo11.test1.UserService;
import org.junit.Test;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

public class AspectTest11 {

    @Test
    public void test1() {
        AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext();
        context.register(MainConfig1.class);
        context.refresh();
        UserService userService = context.getBean(UserService.class);
        userService.say();
        CarService carService = context.getBean(CarService.class);
        carService.say();
    }

}
```
运行输出
```
我是前置通知,target:com.javacode2018.aop.demo11.test1.UserService@dc7df28
我是UserService
我是前置通知,target:com.javacode2018.aop.demo11.test1.CarService@821330f
我是CarService
```
<a name="L7qK4"></a>
## 2、通知执行顺序
`@EnableAspectJAutoProxy` 允许Spring容器中通过`Advisor` 、`@Aspect` 来定义通知，当Spring容器中存在多个`Advisor`、`@Aspect`时，组成的拦截器调用链顺序是什么样的呢？在介绍这个之前，需要先回顾一下aop中4种通知相关知识。
<a name="fixFZ"></a>
### Spring AOP中4种通知（Advice）
```
org.aopalliance.intercept.MethodInterceptor
org.springframework.aop.MethodBeforeAdvice
org.springframework.aop.AfterReturningAdvice
org.springframework.aop.ThrowsAdvice
```
所有的通知最终都需要转换为`MethodInterceptor`类型的通知，然后组成一个`MethodInterceptor`列表，称之为方法调用链或者拦截器链，上面列表中后面3通过下面的转换器将其包装为`MethodInterceptor`类型的通知：
```
org.springframework.aop.MethodBeforeAdvice -> org.springframework.aop.framework.adapter.MethodBeforeAdviceInterceptor
org.springframework.aop.AfterReturningAdvice -> org.springframework.aop.framework.adapter.AfterReturningAdviceInterceptor
org.springframework.aop.ThrowsAdvice -> org.springframework.aop.framework.adapter.ThrowsAdviceInterceptor
```
下面再来看一下4种通知的用法和执行过程，以方便理解其执行顺序。
<a name="cxbgU"></a>
#### `org.aopalliance.intercept.MethodInterceptor`：方法拦截器
方法拦截器，这个比较强大，可以在方法执行前后执行一些增强操作，其他类型的通知最终都会被包装为 `MethodInterceptor` 来执行。<br />下面自定义一个`MethodInterceptor`
```java
class MyMethodInterceptor implements MethodInterceptor {
    @Override
    public Object invoke(MethodInvocation invocation) throws Throwable {
        System.out.println("我是MethodInterceptor start");
        //调用invocation.proceed()执行下一个拦截器
        Object result = invocation.proceed();
        System.out.println("我是MethodInterceptor end");
        //返回结果
        return result;
    }
}
```
<a name="OptJR"></a>
#### `org.springframework.aop.MethodBeforeAdvice`：方法前置通知
方法前置通知，可以在方法之前定义增强操作。<br />下面自定义一个`MethodBeforeAdvice`
```java
class MyMethodBeforeAdvice implements MethodBeforeAdvice {

    @Override
    public void before(Method method, Object[] args, @Nullable Object target) throws Throwable {
        System.out.println("我是MethodBeforeAdvice");
    }
}
```
`MethodBeforeAdvice`最终会被包装为`MethodBeforeAdviceInterceptor`类型，然后放到拦截器链中去执行，通过`MethodBeforeAdviceInterceptor`代码可以理解`MethodBeforeAdvice`的执行过程
```java
public class MethodBeforeAdviceInterceptor implements MethodInterceptor, BeforeAdvice, Serializable {

    private final MethodBeforeAdvice advice;

    public MethodBeforeAdviceInterceptor(MethodBeforeAdvice advice) {
        this.advice = advice;
    }


    @Override
    public Object invoke(MethodInvocation mi) throws Throwable {
        //调用MethodBeforeAdvice的before方法，执行前置通知
        this.advice.before(mi.getMethod(), mi.getArguments(), mi.getThis());
        //执行下一个拦截器
        return mi.proceed();
    }

}
```
<a name="GIWLm"></a>
#### `org.springframework.aop.AfterReturningAdvice`：方法返回通知
方法返回通知，用来在方法执行完毕之后执行一些增强操作。<br />下面自定义一个`AfterReturningAdvice`
```java
class MyAfterReturningAdvice implements AfterReturningAdvice {

    @Override
    public void afterReturning(@Nullable Object returnValue, Method method, Object[] args, @Nullable Object target) throws Throwable {
        System.out.println("我是AfterReturningAdvice");
    }
}
```
`AfterReturningAdvice`最终会被包装为`AfterReturningAdviceInterceptor`类型，然后放到拦截器链中去执行，通过`AfterReturningAdviceInterceptor`代码可以理解`AfterReturningAdvice`的执行过程
```java
public class AfterReturningAdviceInterceptor implements MethodInterceptor, AfterAdvice, Serializable {

    private final AfterReturningAdvice advice;

    public AfterReturningAdviceInterceptor(AfterReturningAdvice advice) {
        this.advice = advice;
    }


    @Override
    public Object invoke(MethodInvocation mi) throws Throwable {
        //执行下一个拦截器，可以获取目标方法的返回结果
        Object retVal = mi.proceed();
        //调用方法返回通知的afterReturning方法，会传入目标方法的返回值等信息
        this.advice.afterReturning(retVal, mi.getMethod(), mi.getArguments(), mi.getThis());
        return retVal;
    }

}
```
<a name="AkkQc"></a>
#### `org.springframework.aop.ThrowsAdvice`：异常通知
当目标方法发生异常时，可以通过 `ThrowsAdvice` 来指定需要回调的方法，在此可以记录一些异常信息，或者将异常信息发送到监控系统等。<br />下面自定义一个`ThrowsAdvice`
```java
/**
 * 用来定义异常通知
 * 方法名必须是afterThrowing，格式参考下面2种定义
 * 1. public void afterThrowing(Exception ex)
 * 2. public void afterThrowing(Method method, Object[] args, Object target, Exception ex)
 */
class MyThrowsAdvice implements ThrowsAdvice {
    public void afterThrowing(Method method, Object[] args, Object target, Exception ex) {
        System.out.println("我是ThrowsAdvice");
    }
}
```
`ThrowsAdvice`最终会被包装为`ThrowsAdviceInterceptor`类型，然后放到拦截器链中去执行，通过`ThrowsAdviceInterceptor`代码可以理解`ThrowsAdvice`的执行过程，`ThrowsAdviceInterceptor` 构造参数传入一个自定义的 `ThrowsAdvice` 对象
```java
public class ThrowsAdviceInterceptor implements MethodInterceptor, AfterAdvice {

    private final Object throwsAdvice;

    public ThrowsAdviceInterceptor(Object throwsAdvice) {
        this.throwsAdvice = throwsAdvice;
    }

    @Override
    public Object invoke(MethodInvocation mi) throws Throwable {
        try {
            return mi.proceed();
        } catch (Throwable ex) {
            //调用 ThrowsAdvice 中的 afterThrowing 方法来处理异常
            this.throwsAdvice.afterThrowing(。。。。);
            //将异常继续往外抛
            throw ex;
        }
    }
}
```
<a name="bcgVB"></a>
### 拦截器链执行过程
假如目标方法上面有好几个通知，调用目标方法执行，Spring会将所有的通知转换得到一个`MethodInterceptor`列表，然后依次按照下面的方式执行，会先调用第一个拦截器的`MethodInterceptor#invoke(MethodInvocation invocation)`方法，会传递一个`MethodInvocation`类型的参数，在此方法中，可以调用`MethodInvocation#processd`方法去执行第二个拦截器，然后依次按照这样的过程执行，到了最后一个`MethodInterceptor`中，再次调用`MethodInvocation#processd`时，会调用目标方法。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684250987788-c3b8a5a7-75b8-4944-a689-4a3f5e3c01fc.png#averageHue=%23f3f3f3&clientId=ufe7571b1-2804-4&from=paste&id=u02252e81&originHeight=501&originWidth=624&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7679f59c-ded3-4e02-b948-276fc12114e&title=)
<a name="Po2Vm"></a>
### 4种通知的执行顺序
结合上面的过程，假如目标方法上面依次添加了下面4种通知，来分析一下他们的执行过程
```java
class MyMethodInterceptor implements MethodInterceptor {
    @Override
    public Object invoke(MethodInvocation invocation) throws Throwable {
        System.out.println("我是MethodInterceptor start");
        //调用invocation.proceed()执行下一个拦截器
        Object result = invocation.proceed();
        System.out.println("我是MethodInterceptor end");
        //返回结果
        return result;
    }
}

class MyMethodBeforeAdvice implements MethodBeforeAdvice {

    @Override
    public void before(Method method, Object[] args, @Nullable Object target) throws Throwable {
        System.out.println("我是MethodBeforeAdvice");
    }
}

class MyAfterReturningAdvice implements AfterReturningAdvice {

    @Override
    public void afterReturning(@Nullable Object returnValue, Method method, Object[] args, @Nullable Object target) throws Throwable {
        System.out.println("我是AfterReturningAdvice");
    }
}

class MyThrowsAdvice implements ThrowsAdvice {
    public void afterThrowing(Method method, Object[] args, Object target, Exception ex) {
        System.out.println("我是ThrowsAdvice");
    }
}
```
根据通知的规定，非`MethodInterceptor`类型的通知，都会被包装为`MethodInterceptor`类型的，上面除了第一个之外，其他3个都会被转换为`MethodInterceptor`，转换之后变成了下面这样：
```java
class MyMethodInterceptor implements MethodInterceptor {
    @Override
    public Object invoke(MethodInvocation mi) throws Throwable {
        System.out.println("我是MethodInterceptor start");
        //调用mi.proceed()执行下一个拦截器
        Object retVal = mi.proceed();
        System.out.println("我是MethodInterceptor end");
        //返回结果
        return retVal;
    }
}

class MyMethodBeforeAdvice implements MethodInterceptor {
    @Override
    public Object invoke(MethodInvocation mi) throws Throwable {
        System.out.println("我是MethodBeforeAdvice");
        //调用mi.proceed()执行下一个拦截器
        Object retVal = mi.proceed();
        return retVal;
    }
}

class MyAfterReturningAdvice implements MethodInterceptor {
    @Override
    public Object invoke(MethodInvocation mi) throws Throwable {
        //调用mi.proceed()执行下一个拦截器
        Object retVal = mi.proceed();
        System.out.println("我是AfterReturningAdvice");
        return retVal;
    }
}

class MyThrowsAdvice implements MethodInterceptor {
    @Override
    public Object invoke(MethodInvocation mi) throws Throwable {
        try {
            //调用mi.proceed()执行下一个拦截器
            return mi.proceed();
        } catch (Throwable ex) {
            System.out.println("我是ThrowsAdvice");
            throw ex;
        }
    }
}
```
根据通知链的执行过程，最终变成了下面这样：
```java
System.out.println("我是MethodInterceptor start");
System.out.println("我是MethodBeforeAdvice");
Object retVal = null;
try {
    retVal = 通过反射调用目标方法获取返回值;
} catch (Throwable ex) {
    System.out.println("我是ThrowsAdvice");
    throw ex;
}
System.out.println("我是AfterReturningAdvice");
System.out.println("我是MethodInterceptor end");
return retVal;
```
将上面4个通知用到下面目标对象中
```java
public static class Service3 {
    public String say(String name) {
        return "你好：" + name;
    }
}
```
执行下面代码生成代理，然后通过代理调用say方法
```java
Service3 target = new Service3();
Service3 proxy = 对target通过aop生成代理对象;
System.out.println(proxy.say("Hi"));
```
被4个拦截器链包裹之后，`System.out.println(proxy.say("Hi"));`执行过程变成了下面这样
```java
System.out.println("我是MethodInterceptor start");
System.out.println("我是MethodBeforeAdvice");
Object retVal = null;
try {
    retVal = target.say("Hi");
} catch (Throwable ex) {
    System.out.println("我是ThrowsAdvice");
    throw ex;
}
System.out.println("我是AfterReturningAdvice");
System.out.println("我是MethodInterceptor end");
System.out.println(retVal);
```
再次简化
```java
System.out.println("我是MethodInterceptor start");
System.out.println("我是MethodBeforeAdvice");
Object retVal = null;
try {
    retVal = "你好：" + name;
} catch (Throwable ex) {
    System.out.println("我是ThrowsAdvice");
    throw ex;
}
System.out.println("我是AfterReturningAdvice");
System.out.println("我是MethodInterceptor end");
System.out.println(retVal);
```
最终会输出
```
我是MethodInterceptor start
我是MethodBeforeAdvice
我是AfterReturningAdvice
我是MethodInterceptor end
你好：Hi
```
上案例代码，来看一下最终的执行结果是不是和分析的一样，下面为需要被代理的类Service3以及需要使用的4个通知。
```java
import org.aopalliance.intercept.MethodInterceptor;
import org.aopalliance.intercept.MethodInvocation;
import org.springframework.aop.AfterReturningAdvice;
import org.springframework.aop.MethodBeforeAdvice;
import org.springframework.aop.ThrowsAdvice;
import org.springframework.lang.Nullable;

import java.lang.reflect.Method;

public class MoreAdvice {

    public static class Service3 {
        public String say(String name) {
            return "你好：" + name;
        }
    }

    public static class MyMethodInterceptor implements MethodInterceptor {
        @Override
        public Object invoke(MethodInvocation invocation) throws Throwable {
            System.out.println("我是MethodInterceptor start");
            //调用invocation.proceed()执行下一个拦截器
            Object result = invocation.proceed();
            System.out.println("我是MethodInterceptor end");
            //返回结果
            return result;
        }
    }

    public static class MyMethodBeforeAdvice implements MethodBeforeAdvice {

        @Override
        public void before(Method method, Object[] args, @Nullable Object target) throws Throwable {
            System.out.println("我是MethodBeforeAdvice");
        }
    }

    public static class MyAfterReturningAdvice implements AfterReturningAdvice {

        @Override
        public void afterReturning(@Nullable Object returnValue, Method method, Object[] args, @Nullable Object target) throws Throwable {
            System.out.println("我是AfterReturningAdvice");
        }
    }

    public static class MyThrowsAdvice implements ThrowsAdvice {
        public void afterThrowing(Method method, Object[] args, Object target, Exception ex) {
            System.out.println("我是ThrowsAdvice");
        }
    }
}
```
对应测试代码
```java
@Test
public void test3() {
    //创建目标对象
    MoreAdvice.Service3 target = new MoreAdvice.Service3();
    //创建代理工厂，通过代理工厂来创建代理对象
    ProxyFactory proxyFactory = new ProxyFactory();
    proxyFactory.setTarget(target);
    //依次为目标对象添加4种通知
    proxyFactory.addAdvice(new MoreAdvice.MyMethodInterceptor());
    proxyFactory.addAdvice(new MoreAdvice.MyMethodBeforeAdvice());
    proxyFactory.addAdvice(new MoreAdvice.MyAfterReturningAdvice());
    proxyFactory.addAdvice(new MoreAdvice.MyThrowsAdvice());
    //获取到代理对象
    MoreAdvice.Service3 proxy = (MoreAdvice.Service3) proxyFactory.getProxy();
    //通过代理对象访问目标方法say
    System.out.println(proxy.say("Hi"));
}
```
运行输出
```
我是MethodInterceptor start
我是MethodBeforeAdvice
我是AfterReturningAdvice
我是MethodInterceptor end
你好：Hi
```
和上面分析的确实一模一样。
<a name="rNQ78"></a>
## 3、单个`@Aspect`中多个通知的执行顺序
`@Aspect`标注的类中可以使用下面5种注解来定义通知
```
@Before
@Around
@After
@AfterReturning
@AfterThrowing
```
当单个`@Aspect`中定义了多种类型的通知时，`@EnableAspectJAutoProxy`内部会对其进行排序，排序顺序如下
```
@AfterThrowing
@AfterReturning
@After
@Around
@Before
```
下面来个`@Aspect`类，同时定义5种通知，然后来一步步分析一下其执行的属性。
```java
import org.aspectj.lang.ProceedingJoinPoint;
import org.aspectj.lang.annotation.*;
import org.springframework.stereotype.Component;

@Component
@Aspect
public class Aspect4 {
    @Pointcut("execution(* com.javacode2018.aop.demo11.test4.Service4.*(..))")
    public void pc() {
    }

    @Before("pc()")
    public void before() {
        System.out.println("@Before通知!");
    }

    @Around("pc()")
    public Object around(ProceedingJoinPoint joinPoint) throws Throwable {
        System.out.println("@Around通知start");
        Object result = joinPoint.proceed();
        System.out.println("@Around绕通知end");
        return result;
    }

    @After("pc()")
    public void after() throws Throwable {
        System.out.println("@After通知!");
    }

    @AfterReturning("pc()")
    public void afterReturning() throws Throwable {
        System.out.println("@AfterReturning通知!");
    }

    @AfterThrowing("pc()")
    public void afterThrowing() {
        System.out.println("@AfterThrowing通知!");
    }

}
```
上面会拦截com.javacode2018.aop.demo11.test4.Service4这个类中的所有方法，下面是Service4的源码。
```java
import org.springframework.stereotype.Component;

@Component
public class Service4 {
    public String say(String name) {
        return "你好：" + name;
    }
}
```
来个Spring的配置类，使用`@EnableAspectJAutoProxy`标注
```java
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.EnableAspectJAutoProxy;

@EnableAspectJAutoProxy
@ComponentScan
public class MainConfig4 {
}
```
测试代码
```java
@Test
public void test4(){
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext();
    context.register(MainConfig4.class);
    context.refresh();
    Service4 service4 = context.getBean(Service4.class);
    System.out.println(service4.say("Hi"));
}
```
运行依次输出如下
```
@Around通知start
@Before通知!
@Around绕通知end
@After通知!
@AfterReturning通知!
你好：Hi
```
卧槽，这输出好像和上面说的不一样的，上面说的会按照下面的顺序执行，这到底是什么情况？
```
@AfterThrowing
@AfterReturning
@After
@Around
@Before
```
别急，排序规则和输出结果都没有问题，慢慢分析，下面的分析非常重要，注意看了
<a name="GfJNd"></a>
## 4、`@Aspect`中5种通知回顾
<a name="yoOBY"></a>
### 5种通知对应的`Advice`类
`@Aspect`中通过5中注解来定义通知，这些注解最终都需要转换为`Advice`去执行，转换关系如下

| 通知 | 对应的Advice类 |
| --- | --- |
| `@AfterThrowing` | org.springframework.aop.aspectj.AspectJAfterThrowingAdvice |
| `@AfterReturning` | org.springframework.aop.aspectj.AspectJAfterReturningAdvice |
| `@After` | org.springframework.aop.aspectj.AspectJAfterAdvice |
| `@Around` | org.springframework.aop.aspectj.AspectJAroundAdvice |
| `@Before` | org.springframework.aop.aspectj.AspectJMethodBeforeAdvice |

重点就在于表格右边的`Advice`类，当了解这些`Advice`的源码之后，他们的执行顺序大家就可以理解了，来看一下这些类的源码，重点看`invoke`方法
<a name="qWmyT"></a>
### `@AfterThrowing`：`AspectJAfterThrowingAdvice`
```java
public class AspectJAfterThrowingAdvice implements MethodInterceptor {

    @Override
    public Object invoke(MethodInvocation mi) throws Throwable {
        try {
            //执行下一个拦截器
            return mi.proceed();
        } catch (Throwable ex) {
            //通过反射调用@AfterThrowing标注的方法
            //继续抛出异常
            throw ex;
        }
    }
}
```
`AspectJAfterThrowingAdvice` 实现了 `MethodInterceptor` 接口，不需要进行包装。
<a name="LDTDv"></a>
### `@AfterReturning`：`AspectJAfterReturningAdvice`
`AspectJAfterReturningAdvice` 源码：
```java
public class AspectJAfterReturningAdvice implements AfterReturningAdvice {
    @Override
    public void afterReturning(@Nullable Object returnValue, Method method, Object[] args, @Nullable Object target) throws Throwable {
        // 调用@AfterReturning标注的方法
    }
}
```
`AspectJAfterReturningAdvice` 实现了 `AfterReturningAdvice` 接口，是一个方法返回通知，不是`MethodInterceptor`类型的，所以最终需包装为`MethodInterceptor`类型，变成下面这样
```java
public class AspectJAfterReturningAdvice implements MethodInterceptor {
    @Override
    public Object invoke(MethodInvocation mi) throws Throwable {
        //执行下一个拦截器
        Object retVal = mi.proceed();
        //调用@AfterReturning标注的方法
        return retVal;
    }
}
```
<a name="rIeeq"></a>
### `@After`：`AspectJAfterAdvice`
`AspectJAfterAdvice` 源码：
```java
public class AspectJAfterAdvice implements MethodInterceptor {
    @Override
    public Object invoke(MethodInvocation mi) throws Throwable {
        try {
            //执行下一个拦截器
            return mi.proceed();
        } finally {
            //调用@After标注的方法
        }
    }
}
```
`AspectJAfterAdvice` 实现了 `MethodInterceptor`接口，所以最终执行的时候不需要进行包装。<br />注意 `invoke` 方法内部使用了 `try...finally` 的方式，`@After`方法的调用放在了`finally`中，所以不管是否有异常，`@After`类型的通知都会被执行。
<a name="e49aj"></a>
### `@Around`：`AspectJAroundAdvice`
`AspectJAroundAdvice` 源码：
```java
public class AspectJAroundAdvice extends AbstractAspectJAdvice implements MethodInterceptor, Serializable {
    @Override
    public Object invoke(MethodInvocation mi) throws Throwable {
        return 调用 @Around标注的方法 ;
    }
}
```
`AspectJAroundAdvice`  实现了 `MethodInterceptor`接口，最终执行的时候也不需要进行包装。
<a name="t2nj4"></a>
### `@Before`：`AspectJMethodBeforeAdvice`
`AspectJMethodBeforeAdvice` 源码：
```java
public class AspectJMethodBeforeAdvice implements MethodBeforeAdvice, Serializable {
    @Override
    public void before(Method method, Object[] args, @Nullable Object target) throws Throwable {
        invokeAdviceMethod(getJoinPointMatch(), null, null);
    }
}
```
`AspectJMethodBeforeAdvice` 实现了`MethodBeforeAdvice`接口，是一个前置通知，不是`MethodInterceptor`类型的，所以最终需包装为`MethodInterceptor`类型，变成下面这样
```java
public class AspectJMethodBeforeAdvice implements MethodInterceptor {
    @Override
    public Object invoke(MethodInvocation mi) throws Throwable {
        //调用@Before标注的方法
        //执行下一个拦截器
        return mi.proceed();
    }
}
```
<a name="Em5Nh"></a>
## 5、分析单个`@Aspect`中多个通知执行顺序
大家对`@Aspect`中5种通知内容理解之后，再回头看一下代码Aspect4中定义的5个通知
```java
public class Aspect4 {
    @Pointcut("execution(* com.javacode2018.aop.demo11.test4.Service4.*(..))")
    public void pc() {
    }

    @Before("pc()")
    public void before() {
        System.out.println("@Before通知!");
    }

    @Around("pc()")
    public Object around(ProceedingJoinPoint joinPoint) throws Throwable {
        System.out.println("@Around通知start");
        Object result = joinPoint.proceed();
        System.out.println("@Around绕通知end");
        return result;
    }

    @After("pc()")
    public void after() throws Throwable {
        System.out.println("@After通知!");
    }

    @AfterReturning("pc()")
    public void afterReturning() throws Throwable {
        System.out.println("@AfterReturning通知!");
    }

    @AfterThrowing("pc()")
    public void afterThrowing() {
        System.out.println("@AfterThrowing通知!");
    }

}
```
结论是，会按照下面的顺序执行
```
@AfterThrowing
@AfterReturning
@After
@Around
@Before
```
按照上面的顺序，一步步来分析。<br />先执行第1个通知`@AfterThrowing`，变成下面这样
```java
try {
    //执行下一个拦截器
    return mi.proceed();
} catch (Throwable ex) {
    System.out.println("@AfterThrowing通知!");
    //继续抛出异常
    throw ex;
}
```
`mi.processed()`会执行第2个通知`@AfterReturning`，变成了下面这样
```java
try {
    //执行下一个拦截器
    Object retVal = mi.proceed();
    System.out.println("@AfterReturning通知!");
    return retVal;
} catch (Throwable ex) {
    System.out.println("@AfterThrowing通知!");
    //继续抛出异常
    throw ex;
}
```
继续`mi.proceed()`执行第3个通知`@After`，变成了下面这样
```java
try {
    Object result = null;
    try {
        //执行下一个拦截器
        result = mi.proceed();
    } finally {
        System.out.println("@After通知!");
    }
    System.out.println("@AfterReturning通知!");
    return retVal;
} catch (Throwable ex) {
    System.out.println("@AfterThrowing通知!");
    //继续抛出异常
    throw ex;
}
```
继续`mi.proceed()`执行第4个通知`@Around`，变成了下面这样
```java
try {
    Object result = null;
    try {
        System.out.println("@Around通知start");
        result = joinPoint.proceed();
        System.out.println("@Around绕通知end");
        return result;
    } finally {
        System.out.println("@After通知!");
    }
    System.out.println("@AfterReturning通知!");
    return retVal;
} catch (Throwable ex) {
    System.out.println("@AfterThrowing通知!");
    //继续抛出异常
    throw ex;
}
```
继续`joinPoint.proceed()`执行第5个通知`@Before`，变成了下面这样
```java
try {
    Object result = null;
    try {
        System.out.println("@Around通知start");
        System.out.println("@Before通知!");
        result = mi.proceed();
        System.out.println("@Around绕通知end");
        return result;
    } finally {
        System.out.println("@After通知!");
    }
    System.out.println("@AfterReturning通知!");
    return retVal;
} catch (Throwable ex) {
    System.out.println("@AfterThrowing通知!");
    //继续抛出异常
    throw ex;
}
```
继续`joinPoint.proceed()`会调用目标方法，变成了下面这样
```java
try {
    Object result = null;
    try {
        System.out.println("@Around通知start");
        System.out.println("@Before通知!");
        result = // 通过反射调用目标方法; //@1
        System.out.println("@Around绕通知end");
        return result;
    } finally {
        System.out.println("@After通知!");
    }
    System.out.println("@AfterReturning通知!");
    return retVal;
} catch (Throwable ex) {
    System.out.println("@AfterThrowing通知!");
    //继续抛出异常
    throw ex;
}
```
将上面的@1替换为目标方法的调用，就变成下面这样了
```java
try {
    Object result = null;
    try {
        System.out.println("@Around通知start");
        System.out.println("@Before通知!");
        result = service4.say("Hi");
        System.out.println("@Around绕通知end");
        return result;
    } finally {
        System.out.println("@After通知!");
    }
    System.out.println("@AfterReturning通知!");
    return retVal;
} catch (Throwable ex) {
    System.out.println("@AfterThrowing通知!");
    //继续抛出异常
    throw ex;
}
```
所以最终输出
```
@Around通知start
@Before通知!
@Around绕通知end
@After通知!
@AfterReturning通知!
你好：Hi
```
<a name="bPCpX"></a>
## 6、`@EnableAspectJAutoProxy`中为通知指定顺序
`@EnableAspectJAutoProxy`用在Spring环境中，可以通过`@Aspect`以及`Advisor`来定义多个通知，当Spring容器中有多个`@Aspect`、`Advisor`时，他们的顺序是什么样的呢？<br />先看一下如何为`@Aspect`、自定义`Advisor`指定顺序。
<a name="VuLAt"></a>
### 为`@Aspect`指定顺序：用`@Order`注解
需要在`@Aspect`标注的类上使用`@org.springframework.core.annotation.Order`注解，值越小，通知的优先级越高。
```java
@Aspect
@Order(1)
public class AspectOrder1{}
```
<a name="Mx4hw"></a>
### 为`Advisor`指定顺序：实现`Ordered`接口
自定义的`Advisor`通过org.springframework.core.Ordered接口来指定顺序，这个接口有个`public int getOrder()`方法，用来返回通知的顺序。<br />Spring提供了一个`Advisor`类型的抽象类`org.springframework.aop.support.AbstractPointcutAdvisor`，这个类实现了`Ordered`接口，Spring中大部分`Advisor`会是继承`AbstractPointcutAdvisor`，若需要自定义`Advisor`，也可以继承这个类，这个类的`getOrder`方法比较关键，来看一下
```java
public abstract class AbstractPointcutAdvisor implements PointcutAdvisor, Ordered, Serializable {

    @Nullable
    private Integer order;

    public void setOrder(int order) {
        this.order = order;
    }

    @Override
    public int getOrder() {
        //若当前Advisor指定了order，则直接返回
        if (this.order != null) {
            return this.order;
        }
        //获取当前类中配置的通知对象Advice
        Advice advice = getAdvice();
        //若advice实现了Ordered接口，这从advice中获取通知的顺序
        if (advice instanceof Ordered) {
            return ((Ordered) advice).getOrder();
        }
        //否则通知的优先级最低，Integer.MAX_VALUE
        return Ordered.LOWEST_PRECEDENCE;
    }
}
```
Spring提供了一个默认的`Advisor`类：`DefaultPointcutAdvisor`，这个类就继承了`AbstractPointcutAdvisor`，通常可以直接使用`DefaultPointcutAdvisor`来自定义通知。
<a name="gHCJ1"></a>
## 7、多个`@Aspect`、`Advisor`排序规则
<a name="jka5M"></a>
### 排序规则
1、在Spring容器中获取`@Aspect`、`Advisor`类型的所有bean，得到一个列表 list1<br />2、对list1按照order的值升序排序，得到结果list2<br />3、然后再对list2中`@Aspect`类型的bean内部的通知进行排序，规则
```
@AfterThrowing
@AfterReturning
@After
@Around
@Before
```
4、最后运行的时候会得到上面排序产生的方法调用链列表去执行。
<a name="yfWZP"></a>
### 案例
下面定义2个`@Aspect`类，一个`Advisor`类，并且给这3个都指定，然后来验证一下通知执行的顺序。
<a name="Q5RHY"></a>
#### 先定义目标类
```java
import org.springframework.stereotype.Component;

@Component
public class Service2 {
    public String say(String name) {
        return "你好：" + name;
    }
}
```
<a name="eUS34"></a>
#### Aspect1：第2个`@Aspect`
```java
import org.aspectj.lang.ProceedingJoinPoint;
import org.aspectj.lang.annotation.*;
import org.springframework.core.annotation.Order;
import org.springframework.stereotype.Component;

@Aspect
@Order(1)
@Component
public class MyAspect1 {

    @Pointcut("execution(* com.javacode2018.aop.demo11.test2.Service2.*(..))")
    public void pc() {
    }

    @Before("pc()")
    public void before() {
        System.out.println("MyAspect1 @Before通知!");
    }

    @Around("pc()")
    public Object around(ProceedingJoinPoint joinPoint) throws Throwable {
        System.out.println("MyAspect1 @Around通知start");
        Object result = joinPoint.proceed();
        System.out.println("MyAspect1 @Around绕通知end");
        return result;
    }

    @After("pc()")
    public void after() throws Throwable {
        System.out.println("MyAspect1 @After通知!");
    }

    @AfterReturning("pc()")
    public void afterReturning() throws Throwable {
        System.out.println("MyAspect1 @AfterReturning通知!");
    }

    @AfterThrowing("pc()")
    public void afterThrowing() {
        System.out.println("MyAspect1 @AfterThrowing通知!");
    }

}
```
<a name="aHDpf"></a>
#### Aspect1：第2个@Aspect
```java
import org.aspectj.lang.ProceedingJoinPoint;
import org.aspectj.lang.annotation.*;
import org.springframework.core.annotation.Order;
import org.springframework.stereotype.Component;

@Aspect
@Order(3)
@Component
public class MyAspect2 {

    @Pointcut("execution(* com.javacode2018.aop.demo11.test2.Service2.*(..))")
    public void pc() {
    }

    @Before("pc()")
    public void before() {
        System.out.println("MyAspect2 @Before通知!");
    }

    @Around("pc()")
    public Object around(ProceedingJoinPoint joinPoint) throws Throwable {
        System.out.println("MyAspect2 @Around通知start");
        Object result = joinPoint.proceed();
        System.out.println("MyAspect2 @Around绕通知end");
        return result;
    }

    @After("pc()")
    public void after() throws Throwable {
        System.out.println("MyAspect2 @After通知!");
    }

    @AfterReturning("pc()")
    public void afterReturning() throws Throwable {
        System.out.println("MyAspect2 @AfterReturning通知!");
    }

    @AfterThrowing("pc()")
    public void afterThrowing() {
        System.out.println("MyAspect2 @AfterThrowing通知!");
    }

}
```
<a name="SEj2i"></a>
#### 自定义一个Advisor
```java
import org.aopalliance.intercept.MethodInterceptor;
import org.aopalliance.intercept.MethodInvocation;
import org.springframework.aop.support.DefaultPointcutAdvisor;
import org.springframework.stereotype.Component;

@Component
public class Advisor1 extends DefaultPointcutAdvisor {

    public Advisor1() {
        MethodInterceptor methodInterceptor = new MethodInterceptor() {
            @Override
            public Object invoke(MethodInvocation invocation) throws Throwable {
                System.out.println("Advisor1 start");
                Object result = invocation.proceed();
                System.out.println("Advisor1 end");
                return result;
            }
        };
        this.setAdvice(methodInterceptor);
    }

    @Override
    public int getOrder() {
        return 2;
    }
}
```
<a name="S8w4F"></a>
#### 来个Spring配置类
标注`@EnableAspectJAutoProxy`来启用自动化的aop功能
```java
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.EnableAspectJAutoProxy;

@ComponentScan
@EnableAspectJAutoProxy
public class MainConfig2 {
}
```
<a name="OdRTU"></a>
#### 测试代码
```java
@Test
public void test2() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext();
    context.register(MainConfig2.class);
    context.refresh();
    Service2 service2 = context.getBean(Service2.class);
    System.out.println(service2.say("Hi"));
}
```
<a name="MRNLr"></a>
#### 运行输出
```
MyAspect1 @Around通知start
MyAspect1 @Before通知!
Advisor1 start
MyAspect2 @Around通知start
MyAspect2 @Before通知!
MyAspect2 @Around绕通知end
MyAspect2 @After通知!
MyAspect2 @AfterReturning通知!
Advisor1 end
MyAspect1 @Around绕通知end
MyAspect1 @After通知!
MyAspect1 @AfterReturning通知!
你好：Hi
```
<a name="LPjRg"></a>
#### 结果分析
下面一步步来推出结果为什么是上面这样。<br />先获取Spring容器中`@Aspect`、`Advisor`类型的所有bean，根据其order升序排序，得到：
```
Aspect1：顺序是1
Advisor1：顺序是2
MyAspect2：顺序是3
```
然后对每个Aspect内部的通知进行排序，根据单个`@Aspect`内部通知排序规则，可以得到：
```
Aspect1：顺序是1
 @AfterThrowing
    @AfterReturning
    @After
    @Around
    @Before

Advisor1：顺序是2

MyAspect2：顺序是3
 @AfterThrowing
    @AfterReturning
    @After
    @Around
    @Before
```
下面将代码拿过来一步步填充。<br />先对Aspect1进行填充，得到：
```java
try {
    Object result = null;
    try {
        System.out.println("MyAspect1 @Around通知start");
        System.out.println("MyAspect1 @Before通知!");
        result = mi.proceed(); //@1
        System.out.println("MyAspect1 @Around绕通知end");
        return result;
    } finally {
        System.out.println("MyAspect1 @After通知!");
    }
    System.out.println("MyAspect1 @AfterReturning通知!");
    return retVal;
} catch (Throwable ex) {
    System.out.println("MyAspect1 @AfterThrowing通知!");
    //继续抛出异常
    throw ex;
}
```
@1执行`mi.proceed()`会调用下一个拦截器，即Advisor1中定义的拦截器，然后会得到下面代码：
```java
try {
    Object result = null;
    try {
        System.out.println("MyAspect1 @Around通知start");
        System.out.println("MyAspect1 @Before通知!");
        System.out.println("Advisor1 start");
        result = invocation.proceed(); //@2
        System.out.println("Advisor1 end");
        System.out.println("MyAspect1 @Around绕通知end");
        return result;
    } finally {
        System.out.println("MyAspect1 @After通知!");
    }
    System.out.println("MyAspect1 @AfterReturning通知!");
    return retVal;
} catch (Throwable ex) {
    System.out.println("MyAspect1 @AfterThrowing通知!");
    //继续抛出异常
    throw ex;
}
```
@2执行`mi.proceed()`会调用下一个拦截器，即Aspect2中定义的拦截器，而Aspect2和Aspect1类似，然后会得到下面代码：
```java
try {
    Object result = null;
    try {
        System.out.println("MyAspect1 @Around通知start");
        System.out.println("MyAspect1 @Before通知!");
        System.out.println("Advisor1 start");
        try {
            try {
                System.out.println("MyAspect2 @Around通知start");
                System.out.println("MyAspect2 @Before通知!");
                result = mi.proceed(); //@3
                System.out.println("MyAspect2 @Around绕通知end");
                return result;
            } finally {
                System.out.println("MyAspect2 @After通知!");
            }
            System.out.println("MyAspect2 @AfterReturning通知!");
        } catch (Throwable ex) {
            System.out.println("MyAspect2 @AfterThrowing通知!");
            //继续抛出异常
            throw ex;
        }
        System.out.println("Advisor1 end");
        System.out.println("MyAspect1 @Around绕通知end");
        return result;
    } finally {
        System.out.println("MyAspect1 @After通知!");
    }
    System.out.println("MyAspect1 @AfterReturning通知!");
    return retVal;
} catch (Throwable ex) {
    System.out.println("MyAspect1 @AfterThrowing通知!");
    //继续抛出异常
    throw ex;
}
```
@3继续执行`mi.proceed()`，此时会调用目标方法`say("Hi")`，然后就进化成下面这样了
```java
try {
    Object result = null;
    try {
        System.out.println("MyAspect1 @Around通知start");
        System.out.println("MyAspect1 @Before通知!");
        System.out.println("Advisor1 start");
        try {
            try {
                System.out.println("MyAspect2 @Around通知start");
                System.out.println("MyAspect2 @Before通知!");
                result = "你好：Hi";
                System.out.println("MyAspect2 @Around绕通知end");
                return result;
            } finally {
                System.out.println("MyAspect2 @After通知!");
            }
            System.out.println("MyAspect2 @AfterReturning通知!");
        } catch (Throwable ex) {
            System.out.println("MyAspect2 @AfterThrowing通知!");
            //继续抛出异常
            throw ex;
        }
        System.out.println("Advisor1 end");
        System.out.println("MyAspect1 @Around绕通知end");
        return result;
    } finally {
        System.out.println("MyAspect1 @After通知!");
    }
    System.out.println("MyAspect1 @AfterReturning通知!");
    return retVal;
} catch (Throwable ex) {
    System.out.println("MyAspect1 @AfterThrowing通知!");
    //继续抛出异常
    throw ex;
}
```
再来和输出结果对比一下，是完全一致的。
```
MyAspect1 @Around通知start
MyAspect1 @Before通知!
Advisor1 start
MyAspect2 @Around通知start
MyAspect2 @Before通知!
MyAspect2 @Around绕通知end
MyAspect2 @After通知!
MyAspect2 @AfterReturning通知!
Advisor1 end
MyAspect1 @Around绕通知end
MyAspect1 @After通知!
MyAspect1 @AfterReturning通知!
你好：Hi
```
<a name="ifX5t"></a>
## 8、`@EnableAspectJAutoProxy`另外2个功能
这个注解还有2个参数，大家看一下下面的注释，比较简单，就不用案例演示了。
```java
public @interface EnableAspectJAutoProxy {

    /**
     * 是否基于类来创建代理，而不是基于接口来创建代理
     * 当为true的时候会使用cglib来直接对目标类创建代理对象
     * 默认为 false：即目标bean如果有接口的会采用jdk动态代理来创建代理对象，没有接口的目标bean，会采用cglib来创建代理对象
     */
    boolean proxyTargetClass() default false;

    /**
     * 是否需要将代理对象暴露在ThreadLocal中，当为true的时候
     * 可以通过org.springframework.aop.framework.AopContext#currentProxy获取当前代理对象
     */
    boolean exposeProxy() default false;

}
```
<a name="Nk5q1"></a>
## 9、`@EnableAspectJAutoProxy`原理
`@EnableAspectJAutoProxy`会在Spring容器中注册一个bean
```
org.springframework.aop.aspectj.annotation.AnnotationAwareAspectJAutoProxyCreator
```
`AnnotationAwareAspectJAutoProxyCreator`是`BeanPostProcessor`类型的，`BeanPostProcessor`大家应该比较熟悉了，bean后置处理器，可以在bean声明周期中对bean进行操作，比如对bean生成代理等；而`AnnotationAwareAspectJAutoProxyCreator`就是对符合条件的bean，自动生成代理对象，有兴趣的可以从`postProcessAfterInitialization`方法看，比较简单。
