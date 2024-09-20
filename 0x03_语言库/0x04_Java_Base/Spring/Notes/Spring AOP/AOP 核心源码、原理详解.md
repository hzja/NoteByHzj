JavaSpringAOP
<a name="SFljc"></a>
## Spring AOP原理
原理比较简单，主要就是使用jdk动态代理和cglib代理来创建代理对象，通过代理对象来访问目标对象，而代理对象中融入了增强的代码，最终起到对目标对象增强的效果。
<a name="Y68c9"></a>
## aop相关的一些类

1. 连接点（JoinPoint）相关类
2. 通知（Advice）相关的类
3. 切入点（Pointcut）相关的类
4. 切面（Advisor）相关的类
<a name="QU6pT"></a>
## 连接点（JoinPoint）相关类
<a name="SzIZj"></a>
### JoinPoint接口
这个接口表示一个通用的运行时连接点（在AOP术语中）
```java
package org.aopalliance.intercept;

public interface Joinpoint {

    /**
     * 转到拦截器链中的下一个拦截器
     */
    Object proceed() throws Throwable;

    /**
     * 返回保存当前连接点静态部分【的对象】，这里一般指被代理的目标对象
     */
    Object getThis();

    /**
     * 返回此静态连接点  一般就为当前的Method(至少目前的唯一实现是MethodInvocation,所以连接点得静态部分肯定就是本方法)
     */
    AccessibleObject getStaticPart();

}
```
几个重要的子接口和实现类，如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684331293423-a6b76dde-f1f3-4f3b-8142-e0242fc1d423.png#averageHue=%23fbfaef&clientId=u4c06a739-18d6-4&from=paste&id=u63fe40ee&originHeight=513&originWidth=339&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u347dae0c-499f-402e-90e5-a55ece244f6&title=)
<a name="VKwIP"></a>
### `Invocation`接口
此接口表示程序中的调用，调用是一个连接点，可以被拦截器拦截。
```java
package org.aopalliance.intercept;

/**
 * 此接口表示程序中的调用
 * 调用是一个连接点，可以被拦截器拦截。
 */
public interface Invocation extends Joinpoint {

    /**
     * 将参数作为数组对象获取，可以更改此数组中的元素值以更改参数。
     * 通常用来获取调用目标方法的参数
     */
    Object[] getArguments();
}
```
<a name="hddNq"></a>
### `MethodInvocation`接口
用来表示连接点中方法的调用，可以获取调用过程中的目标方法。
```java
package org.aopalliance.intercept;

import java.lang.reflect.Method;

/**
 * 方法调用的描述，在方法调用时提供给拦截器。
 * 方法调用是一个连接点，可以被方法拦截器拦截。
 */
public interface MethodInvocation extends Invocation {

    /**
     * 返回正在被调用得方法~~~  返回的是当前Method对象。
     * 此时，效果同父类的AccessibleObject getStaticPart() 这个方法
     */
    Method getMethod();

}
```
<a name="xcxSk"></a>
### `ProxyMethodInvocation`接口
表示代理方法的调用
```java
public interface ProxyMethodInvocation extends MethodInvocation {

    /**
     * 获取被调用的代理对象
     */
    Object getProxy();

    /**
     * 克隆一个方法调用器MethodInvocation
     */
    MethodInvocation invocableClone();

    /**
     * 克隆一个方法调用器MethodInvocation，并为方法调用器指定参数
     */
    MethodInvocation invocableClone(Object... arguments);

    /**
     * 设置要用于此链中任何通知的后续调用的参数。
     */
    void setArguments(Object... arguments);

    /**
     * 添加一些扩展用户属性，这些属性不在AOP框架内使用。它们只是作为调用对象的一部分保留，用于特殊的拦截器。
     */
    void setUserAttribute(String key, @Nullable Object value);

    /**
     * 根据key获取对应的用户属性
     */
    @Nullable
    Object getUserAttribute(String key);

}
```
通俗点理解：连接点表示方法的调用过程，内部包含了方法调用过程中的所有信息，比如被调用的方法、目标、代理对象、执行拦截器链等信息。<br />上面定义都是一些接口，最终有2个实现。
<a name="GH8x1"></a>
### `ReflectiveMethodInvocation`
当代理对象是采用jdk动态代理创建的，通过代理对象来访问目标对象的方法的时，最终过程是由`ReflectiveMethodInvocation`来处理的，内部会通过递归调用方法拦截器，最终会调用到目标方法。
<a name="Ur3NO"></a>
### `CglibMethodInvocation`
功能和上面的类似，当代理对象是采用cglib创建的，通过代理对象来访问目标对象的方法的时，最终过程是由`CglibMethodInvocation`来处理的，内部会通过递归调用方法拦截器，最终会调用到目标方法。<br />这2个类源码稍后详解。
<a name="cUPlf"></a>
## 通知相关的类
通知用来定义需要增强的逻辑。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684331293477-37cea00a-941f-4c16-9fe9-c661c4f153c1.png#averageHue=%23fdfdfc&clientId=u4c06a739-18d6-4&from=paste&id=u147aa077&originHeight=233&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua010b04c-444a-4b93-ab67-a587d71f588&title=)
<a name="RMLf5"></a>
### `Advice`接口
通知的底层接口
```java
package org.aopalliance.aop;
public interface Advice {
}
```
<a name="J325C"></a>
### `BeforeAdvice`接口
方法前置通知，内部空的
```java
package org.springframework.aop;

public interface BeforeAdvice extends Advice {
}
```
<a name="TTmP4"></a>
### `Interceptor`接口
此接口表示通用拦截器
```java
package org.aopalliance.intercept;

public interface Interceptor extends Advice {
}
```
<a name="LF3pf"></a>
### `MethodInterceptor`接口
方法拦截器，所有的通知均需要转换为`MethodInterceptor`类型的，最终多个`MethodInterceptor`组成一个方法拦截器连。
```java
package org.aopalliance.intercept;

@FunctionalInterface
public interface MethodInterceptor extends Interceptor {
    /**
     * 拦截目标方法的执行，可以在这个方法内部实现需要增强的逻辑，以及主动调用目标方法
     */
    Object invoke(MethodInvocation invocation) throws Throwable;
}
```
<a name="DElnb"></a>
### `AfterAdvice`接口
后置通知的公共标记接口
```java
package org.springframework.aop;

public interface AfterAdvice extends Advice {
}
```
<a name="CbewH"></a>
### `MethodBeforeAdvice`接口
方法执行前通知，需要在目标方法执行前执行一些逻辑的，可以通过这个实现。<br />通俗点说：需要在目标方法执行之前增强一些逻辑，可以通过这个接口来实现。before方法：在调用给定方法之前回调。
```java
package org.springframework.aop;

public interface MethodBeforeAdvice extends BeforeAdvice {

    /**
     * 调用目标方法之前会先调用这个before方法
     * method：需要执行的目标方法
     * args：目标方法的参数
     * target：目标对象
     */
    void before(Method method, Object[] args, @Nullable Object target) throws Throwable;
}
```
如同
```java
public Object invoke(){
    调用MethodBeforeAdvice#before方法
    return 调用目标方法;
}
```
<a name="dPa6j"></a>
### `AfterReturningAdvice`接口
方法执行后通知，需要在目标方法执行之后执行增强一些逻辑的，可以通过这个实现。<br />**不过需要注意一点：目标方法正常执行后，才会回调这个接口，当目标方法有异常，那么这通知会被跳过。**
```java
package org.springframework.aop;

public interface AfterReturningAdvice extends AfterAdvice {

    /**
     * 目标方法执行之后会回调这个方法
     * method：需要执行的目标方法
     * args：目标方法的参数
     * target：目标对象
     */
    void afterReturning(@Nullable Object returnValue, Method method, Object[] args, @Nullable Object target) throws Throwable;

}
```
如同
```java
public Object invoke(){
    Object retVal = 调用目标方法;
    调用AfterReturningAdvice#afterReturning方法
    return retVal;
}
```
<a name="pS1yL"></a>
### `ThrowsAdvice`接口
```java
package org.springframework.aop;

public interface ThrowsAdvice extends AfterAdvice {

}
```
此接口上没有任何方法，因为方法由反射调用，实现类必须实现以下形式的方法，前3个参数是可选的，最后一个参数为需要匹配的异常的类型。
```java
void afterThrowing([Method, args, target], ThrowableSubclass);
```
有效方法的一些例子如下：
```java
public void afterThrowing(Exception ex)
public void afterThrowing(RemoteException)
public void afterThrowing(Method method, Object[] args, Object target, Exception ex)
public void afterThrowing(Method method, Object[] args, Object target, ServletException ex)
```
<a name="b7Zf2"></a>
### 通知包装器
**负责将各种非**`**MethodInterceptor**`**类型的通知(Advice)包装为**`**MethodInterceptor**`**类型。**<br />**刚才有说过：Aop中所有的**`**Advice**`**最终都会转换为**`**MethodInterceptor**`**类型的，组成一个方法调用链，然后执行**<br />3个包装器类

- `MethodBeforeAdviceInterceptor`
- `AfterReturningAdviceInterceptor`
- `ThrowsAdviceInterceptor`
<a name="TSsNy"></a>
### `MethodBeforeAdviceInterceptor`类
这个类实现了`MethodInterceptor`接口，负责将`MethodBeforeAdvice`方法前置通知包装为`MethodInterceptor`类型，创建这个类型的对象的时候需要传递一个`MethodBeforeAdvice`类型的参数，重点是`invoke`方法
```java
package org.springframework.aop.framework.adapter;

@SuppressWarnings("serial")
public class MethodBeforeAdviceInterceptor implements MethodInterceptor, BeforeAdvice, Serializable {

    private final MethodBeforeAdvice advice;

    public MethodBeforeAdviceInterceptor(MethodBeforeAdvice advice) {
        Assert.notNull(advice, "Advice must not be null");
        this.advice = advice;
    }


    @Override
    public Object invoke(MethodInvocation mi) throws Throwable {
        //负责调用前置通知的方法
        this.advice.before(mi.getMethod(), mi.getArguments(), mi.getThis());
        //继续执行方法调用链
        return mi.proceed();
    }

}
```
<a name="E9F6q"></a>
### `AfterReturningAdviceInterceptor`类
这个类实现了`MethodInterceptor`接口，负责将`AfterReturningAdvice`方法后置通知包装为`MethodInterceptor`类型，创建这个类型的对象的时候需要传递一个`AfterReturningAdvice`类型的参数，重点是`invoke`方法
```java
public class AfterReturningAdviceInterceptor implements MethodInterceptor, AfterAdvice, Serializable {

    private final AfterReturningAdvice advice;

    public AfterReturningAdviceInterceptor(AfterReturningAdvice advice) {
        Assert.notNull(advice, "Advice must not be null");
        this.advice = advice;
    }


    @Override
    public Object invoke(MethodInvocation mi) throws Throwable {
        //先执行方法调用链,可以获取目标方法的执行结果
        Object retVal = mi.proceed();
        //执行后置通知
        this.advice.afterReturning(retVal, mi.getMethod(), mi.getArguments(), mi.getThis());
        //返回结果
        return retVal;
    }

}
```
<a name="GjH2f"></a>
### `ThrowsAdviceInterceptor`类
这个类实现了`MethodInterceptor`接口，负责将`ThrowsAdvice`异常通知包装为`MethodInterceptor`类型，创建这个类型的对象的时候需要传递一个`Object`类型的参数，通常这个参数是`ThrowsAdvice`类型的，重点是`invoke`方法
```java
package org.springframework.aop.framework.adapter;

public class ThrowsAdviceInterceptor implements MethodInterceptor, AfterAdvice {

    private static final String AFTER_THROWING = "afterThrowing";

    private final Object throwsAdvice;

    //创建ThrowsAdviceInterceptor
    public ThrowsAdviceInterceptor(Object throwsAdvice) {
        Assert.notNull(throwsAdvice, "Advice must not be null");
        this.throwsAdvice = throwsAdvice;
        //获取异常通知中定义的所有方法（public、默认的、protected、private）
        Method[] methods = throwsAdvice.getClass().getMethods();
        //轮询methods
        for (Method method : methods) {
            //方法名称为afterThrowing && 方法参数为1或者4
            if (method.getName().equals(AFTER_THROWING) &&
                    (method.getParameterCount() == 1 || method.getParameterCount() == 4)) {
                //获取方法的最后一个参数类型
                Class<?> throwableParam = method.getParameterTypes()[method.getParameterCount() - 1];
                //判断方法参数类型是不是Throwable类型的
                if (Throwable.class.isAssignableFrom(throwableParam)) {
                    // 缓存异常处理方法到map中（异常类型->异常处理方法）
                    this.exceptionHandlerMap.put(throwableParam, method);
                }
            }
        }
        //如果exceptionHandlerMap，抛出异常，所以最少要有一个异常处理方法
        if (this.exceptionHandlerMap.isEmpty()) {
            throw new IllegalArgumentException(
                    "At least one handler method must be found in class [" + throwsAdvice.getClass() + "]");
        }
    }


    /**
     * 获取异常通知中自定义的处理异常方法的数量
     */
    public int getHandlerMethodCount() {
        return this.exceptionHandlerMap.size();
    }


    @Override
    public Object invoke(MethodInvocation mi) throws Throwable {
        try {
            //调用通知链
            return mi.proceed();
        }
        catch (Throwable ex) {
            //获取异常通知中自定义的处理异常的方法
            Method handlerMethod = getExceptionHandler(ex);
            //当处理的方法不为空
            if (handlerMethod != null) {
                //调用异常处理方法
                invokeHandlerMethod(mi, ex, handlerMethod);
            }
            //继续向外抛出异常
            throw ex; //@1
        }
    }

    /**
     * 获取throwsAdvice中处理exception参数指定的异常的方法
     */
    @Nullable
    private Method getExceptionHandler(Throwable exception) {
        //获取异常类型
        Class<?> exceptionClass = exception.getClass();
        //从缓存中获取异常类型对应的方法
        Method handler = this.exceptionHandlerMap.get(exceptionClass);
        //来一个循环，查询处理方法，循环条件：方法为空 && 异常类型!=Throwable
        while (handler == null && exceptionClass != Throwable.class) {
            //获取异常的父类型
            exceptionClass = exceptionClass.getSuperclass();
            //从缓存中查找异常对应的处理方法
            handler = this.exceptionHandlerMap.get(exceptionClass);
        }
        //将查找结果返回
        return handler;
    }

    //通过反射调用异常通知中的异常方法
    private void invokeHandlerMethod(MethodInvocation mi, Throwable ex, Method method) throws Throwable {
        //构建方法请求参数
        Object[] handlerArgs;
        //若只有1个参数，参数为：异常对象
        if (method.getParameterCount() == 1) {
            handlerArgs = new Object[] {ex};
        }
        else {
            //4个参数（方法、方法请求参数、目标对象、异常对象）
            handlerArgs = new Object[] {mi.getMethod(), mi.getArguments(), mi.getThis(), ex};
        }
        try {
            //通过反射调用异常通知中的方法
            method.invoke(this.throwsAdvice, handlerArgs);
        }
        catch (InvocationTargetException targetEx) {
            throw targetEx.getTargetException();
        }
    }

}
```
从上面可以看出，异常通知，自定义处理异常的方法有几个特点

1. 方法名称必须为`afterThrowing`
2. 方法参数必须1个或4个，最后一个参数是`Throwable`类型或其子类型
3. 可以在异常处理中记录一些异常信息，这个还是比较有用的，但是注意一点目标方法抛出的异常最后还是会向外继续抛出@1

**光讲源码，大家看着枯燥乏味，来点案例。**<br />先来一个类，用来模拟用户资金操作：充值、提现、查询资金余额；提现的时候余额不足的时候，会抛出异常。
```java
//模拟资金操作
public class FundsService {
    //账户余额
    private double balance = 1000;

    //模拟提现
    double recharge(String userName, double price) {
        System.out.println(String.format("%s提现%s", userName, price));
        balance += price;
        return balance;
    }

    //模拟提现
    double cashOut(String userName, double price) {
        if (balance < price) {
            throw new RuntimeException("余额不足!");
        }
        System.out.println(String.format("%s提现%s", userName, price));
        balance -= price;
        return balance;
    }

    //获取余额
    double getBalance(String userName) {
        return balance;
    }
}
```
<a name="qFOFX"></a>
### 案例1：前置通知拦截非法访问
资金操作的所有方法都需要验证用户名，当用户名不是“路人”的时候，直接抛出非法访问异常。
```java
import org.junit.Test;
import org.springframework.aop.MethodBeforeAdvice;
import org.springframework.aop.framework.ProxyFactory;
import org.springframework.lang.Nullable;

import java.lang.reflect.Method;

public class AopTest4 {

    @Test
    public void test1() {
        //代理工厂
        ProxyFactory proxyFactory = new ProxyFactory(new FundsService());
        //添加一个方法前置通知，判断用户名不是“路人”的时候，抛出非法访问异常
        proxyFactory.addAdvice(new MethodBeforeAdvice() {
            @Override
            public void before(Method method, Object[] args, @Nullable Object target) throws Throwable {
                String userName = (String) args[0];
                //如果不是路人的时候，抛出非法访问异常
                if (!"路人".equals(userName)) {
                    throw new RuntimeException(String.format("[%s]非法访问!", userName));
                }
            }
        });
        //通过代理工厂创建代理
        FundsService proxy = (FundsService) proxyFactory.getProxy();
        //调用代理的方法
        proxy.recharge("路人", 100);
        proxy.recharge("张学友", 100);
    }
}
```
运行输出
```
路人提现100.0

java.lang.RuntimeException: [张学友]非法访问!

    at com.javacode2018.aop.demo4.AopTest4$1.before(AopTest4.java:25)
    at org.springframework.aop.framework.adapter.MethodBeforeAdviceInterceptor.invoke(MethodBeforeAdviceInterceptor.java:55)
```
<a name="LzKwO"></a>
### 案例2：通过异常通知记录异常
通过异常通知来捕获所有方法的运行，发现异常之后，通知开发修复bug。
```java
public static class SendMsgThrowsAdvice implements ThrowsAdvice {
    //注意方法名称必须为afterThrowing
    public void afterThrowing(Method method, Object[] args, Object target, RuntimeException e) {
        //监控到异常后发送消息通知开发者
        System.out.println("异常警报：");
        System.out.println(String.format("method:[%s]，args:[%s]", method.toGenericString(), Arrays.stream(args).collect(Collectors.toList())));
        System.out.println(e.getMessage());
        System.out.println("请尽快修复bug！");
    }
}

@Test
public void test2() {
    //代理工厂
    ProxyFactory proxyFactory = new ProxyFactory(new FundsService());
    //添加一个异常通知，发现异常之后发送消息给开发者尽快修复bug
    proxyFactory.addAdvice(new SendMsgThrowsAdvice());
    //通过代理工厂创建代理
    FundsService proxy = (FundsService) proxyFactory.getProxy();
    //调用代理的方法
    proxy.cashOut("路人", 2000);
}
```
运行输出
```
异常警报：
method:[double com.javacode2018.aop.demo4.FundsService.cashOut(java.lang.String,double)]，args:[[路人, 2000.0]]
余额不足!
请尽快修复bug！

java.lang.RuntimeException: 余额不足!

    at com.javacode2018.aop.demo4.FundsService.cashOut(FundsService.java:18)
```
<a name="GJCpj"></a>
## 切入点(`PointCut`)相关类
通知（Advice）用来指定需要增强的逻辑，但是哪些类的哪些方法中需要使用这些通知呢？这个就是通过切入点来配置的。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684331293431-a615af8b-298a-450f-a976-e70d70cff83c.png#averageHue=%23faf9ea&clientId=u4c06a739-18d6-4&from=paste&id=ub82caace&originHeight=287&originWidth=755&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u083c13d2-bb1d-42f1-9c6d-6d0acb2c0bb&title=)
<a name="iOdme"></a>
### `PointCut`接口
```java
package org.springframework.aop;

public interface Pointcut {

    /**
     * 类过滤器, 可以知道哪些类需要拦截
     */
    ClassFilter getClassFilter();

    /**
     * 方法匹配器, 可以知道哪些方法需要拦截
     */
    MethodMatcher getMethodMatcher();

    /**
     * 匹配所有对象的 Pointcut，内部的2个过滤器默认都会返回true
     */
    Pointcut TRUE = TruePointcut.INSTANCE;

}
```
<a name="FIhjr"></a>
### `ClassFilter`接口
类过滤器。
```java
@FunctionalInterface
public interface ClassFilter {

    /**
     * 用来判断目标类型是否匹配
     */
    boolean matches(Class<?> clazz);

}
```
<a name="HWSmw"></a>
### `MethodMatcher`接口
方法过滤器。
```java
public interface MethodMatcher {

    /**
     * 执行静态检查给定方法是否匹配
     * @param method 目标方法
     * @param targetClass 目标对象类型
     */
    boolean matches(Method method, Class<?> targetClass);

    /**
     * 是否是动态匹配，即是否每次执行目标方法的时候都去验证一下
     */
    boolean isRuntime();

    /**
     * 动态匹配验证的方法，比第一个matches方法多了一个参数args，这个参数是调用目标方法传入的参数
     */
    boolean matches(Method method, Class<?> targetClass, Object... args);


    /**
     * 匹配所有方法，这个内部的2个matches方法任何时候都返回true
     */
    MethodMatcher TRUE = TrueMethodMatcher.INSTANCE;

}
```
看`MethodMatcher`还是有点晕的，为什么需要2个`maches`方法？什么是动态匹配？<br />比如下面一个类
```java
public class UserService{
    public void work(String userName){
        System.out.print(userName+",开始工作了!");
    }
}
```
work方法表示当前用户的工作方法，内部可以实现一些工作的逻辑。<br />希望通过aop对这个类进行增强，调用这个方法的时候，当传入的用户名是路人的粉丝的的时候，需要先进行问候，其他用户的时候，无需问候，将这个问题的代码可以放在`MethodBeforeAdvice`中实现，这种情况就是当参数满足一定的条件了，才会使用这个通知，不满足的时候，通知无效，此时就可以使用上面的动态匹配来实现，`MethodMatcher`类中3个参数的matches方法可以用来对目标方法的参数做校验。<br />来看一下MethodMatcher过滤的整个过程

1. 调用`matches(Method method, Class<?> targetClass)`方法，验证方法是否匹配
2. `isRuntime`方法是否为true，如果为false，则以第一步的结果为准，否则继续向下
3. 调用`matches(Method method, Class<?> targetClass, Object... args)`方法继续验证，这个方法多了一个参数，可以对目标方法传入的参数进行校验。

通过上面的过程，大家可以看出来，如果`isRuntime`为false的时候，只需要对方法名称进行校验，当目标方法调用多次的时候，实际上第一步的验证结果是一样的，所以如果`isRuntime`为false的情况，可以将验证结果放在缓存中，提升效率，而Spring内部就是这么做的，`isRuntime`为false的时候，需要每次都进行校验，效率会低一些，不过对性能的影响基本上可以忽略。
<a name="LpuEG"></a>
## 顾问(`Advisor`)
通知定义了需要做什么，切入点定义了在哪些类的哪些方法中执行通知，那么需要将他们2个组合起来才有效啊。<br />顾问（Advisor）就是做这个事情的。<br />在Spring AOP中，可以将advisor理解为切面，切面中通常有2个关键信息：

1. 需要增强的目标方法列表，这个通过切入点(Pointcut)来指定
2. 需要在目标方法中增强的逻辑，这个通过(Advice)通知来指定

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684331293519-bf747780-979a-44c8-b71e-93ab2541c148.png#averageHue=%23fbfaf9&clientId=u4c06a739-18d6-4&from=paste&id=u94e1a899&originHeight=258&originWidth=493&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u53866736-4901-4823-a9d0-a2d9904f738&title=)
<a name="MRIPD"></a>
### `Advisor`接口
```java
package org.springframework.aop;

import org.aopalliance.aop.Advice;

/**
 * 包含AOP通知（在joinpoint处执行的操作）和确定通知适用性的过滤器（如切入点[PointCut]）的基本接口。
 * 这个接口不是供Spring用户使用的，而是为了支持不同类型的建议的通用性。
 */
public interface Advisor {
    /**
     * 返回引用的通知
     */
    Advice getAdvice();

}
```
上面这个接口通常不会直接使用，这个接口有2个子接口，通常会和这2个子接口来打交道，下面看一下这2个子接口。
<a name="GZdsp"></a>
### `PointcutAdvisor`接口
通过名字就能看出来，这个和`Pointcut`有关，内部有个方法用来获取`Pointcut`，AOP使用到的大部分`Advisor`都属于这种类型的。<br />在目标方法中实现各种增强功能基本上都是通过`PointcutAdvisor`来实现的。
```java
package org.springframework.aop;

/**
 * 切入点类型的Advisor
 */
public interface PointcutAdvisor extends Advisor {

    /**
     * 获取顾问中使用的切入点
     */
    Pointcut getPointcut();

}
```
<a name="Zobai"></a>
### `DefaultPointcutAdvisor`类
`PointcutAdvisor`的默认实现，这是最常用的`Advisor`实现，它可以用于任何`Pointcut`和`Advice`类型，代码相当简单，里面定义了2个属性：pointcut和advisor，由使用者指定。
<a name="tWnOi"></a>
### `IntroductionAdvisor`接口
这个接口，估计大家比较陌生，干什么的呢？<br />**一个Java类，没有实现A接口，在不修改Java类的情况下，使其具备A接口的功能。可以通过**`**IntroductionAdvisor**`**给目标类引入更多接口的功能，这个功能是不是非常牛逼。**<br />下面开始2个重点工作。

- 通过源码介绍aop中代理创建过程
- 通过源码介绍代理方法的调用执行过程
<a name="rupUv"></a>
## 代理创建过程源码解析
<a name="YoDHC"></a>
### 先看一段代码
```java
//代理工厂
ProxyFactory proxyFactory = new ProxyFactory(new FundsService());
//添加一个方法前置通知，判断用户名不是“路人”的时候，抛出非法访问异常
proxyFactory.addAdvice(new MethodBeforeAdvice() {
    @Override
    public void before(Method method, Object[] args, @Nullable Object target) throws Throwable {
        String userName = (String) args[0];
        //如果不是路人的时候，抛出非法访问异常
        if (!"路人".equals(userName)) {
            throw new RuntimeException(String.format("[%s]非法访问!", userName));
        }
    }
});
//通过代理工厂创建代理
FundsService proxy = (FundsService) proxyFactory.getProxy();
```
将上面代码拆分一下，变成下面这样
```java
//1.创建代理所需参数配置（如：采用什么方式的代理、通知列表等）
AdvisedSupport advisedSupport = new AdvisedSupport();
//如：添加一个前置通知
advisedSupport.addAdvice(new MethodBeforeAdvice() {
    @Override
    public void before(Method method, Object[] args, @Nullable Object target) throws Throwable {
        String userName = (String) args[0];
        //如果不是路人的时候，抛出非法访问异常
        if (!"路人".equals(userName)) {
            throw new RuntimeException(String.format("[%s]非法访问!", userName));
        }
    }
});
//设置被代理的目标对象
FundsService target = new FundsService();
advisedSupport.setTarget(target);

//2.根据配置信息获取AopProxy对象，AopProxy用来负责创建最终的代理对象
// AopProxy接口有2个实现类（JDK动态代理、cglib代理）
// 具体最终会使用哪种方式，需要根据AdvisedSupport中指定的参数来判断
// 创建AopProxy使用了简单工厂模式
AopProxyFactory aopProxyFactory = new DefaultAopProxyFactory();
//通过AopProxy工厂获取AopProxy对象
AopProxy aopProxy = aopProxyFactory.createAopProxy(advisedSupport);

//3.通过AopProxy创建代理对象
Object proxy = aopProxy.getProxy();
```
从上面可以看出创建代理有3个步骤。
<a name="QkB32"></a>
### 创建代理3大步骤

1. 创建代理所需参数配置
2. 根据代理参数获取`AopProxy`对象
3. 通过`AopProxy`获取代理对象
<a name="trWFf"></a>
### 创建代理所需参数配置
创建代理所需参数配置主要是通过`AdvisedSupport`这个类来做的，看一下类图，下面一个个来介绍。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684331311790-85c751b5-13bd-4937-9cf4-5c662a302699.png#averageHue=%23fafaef&clientId=u4c06a739-18d6-4&from=paste&id=ue1b456dc&originHeight=253&originWidth=361&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub24d59d7-11a9-4131-b6f1-1001a0b142f&title=)
<a name="b3JLc"></a>
### 根据代理参数获取`AopProxy`对象
<a name="RuwFz"></a>
#### `TargetClassAware`接口
比较简单的一个接口，定义了一个方法，用来获取目标对象类型。<br />所谓目标对象：就是被代理对象，比如上面的`fundsService`对象。
```java
package org.springframework.aop;

public interface TargetClassAware {
    @Nullable
    Class<?> getTargetClass();
}
```
<a name="zLCqQ"></a>
#### `ProxyConfig`类
这个类比较关键了，代理配置类，内部包含了创建代理时需要配置的各种参数。
```java
package org.springframework.aop.framework;

/**
 * 对外提供统一的代理参数配置类，以确保所有代理创建程序具有一致的属性
 */
public class ProxyConfig implements Serializable {

    // 标记是否直接对目标类进行代理，而不是通过接口产生代理
    private boolean proxyTargetClass = false;

    // 标记是否对代理进行优化。启动优化通常意味着在代理对象被创建后，增强的修改将不会生效，因此默认值为false。
    // 如果exposeProxy设置为true，即使optimize为true也会被忽略。
    private boolean optimize = false;

    // 标记是否需要阻止通过该配置创建的代理对象转换为Advised类型，默认值为false，表示代理对象可以被转换为Advised类型
    boolean opaque = false;

    // 标记代理对象是否应该被aop框架通过AopContext以ThreadLocal的形式暴露出去。
    // 当一个代理对象需要调用它自己的另外一个代理方法时，这个属性将非常有用。默认是是false，以避免不必要的拦截。
    boolean exposeProxy = false;

    // 标记该配置是否需要被冻结，如果被冻结，将不可以修改增强的配置。
    // 当我们不希望调用方修改转换成Advised对象之后的代理对象时，这个配置将非常有用。
    private boolean frozen = false;


    //省略了属性的get set方法
}
```
<a name="m72kQ"></a>
#### `Advised`接口
这个接口中定义了操作Aop代理配置的各种方法（比如指定被代理的目标对象、添加通知、添加顾问等等）。<br />所有由Spring AOP创建的代理对象默认都会实现这个接口。
```java
public interface Advised extends TargetClassAware {

    /**
     * 返回配置是否已冻结，被冻结之后，无法修改已创建好的代理对象中的通知
     */
    boolean isFrozen();

    /**
     * 是否对目标类直接创建代理，而不是对接口创建代理，通俗点讲：如果是通过cglib创建代理，此方法返回true，否则返回false
     */
    boolean isProxyTargetClass();

    /**
     * 获取配置中需要代理的接口列表
     */
    Class<?>[] getProxiedInterfaces();

    /**
     * 判断某个接口是否被代理
     */
    boolean isInterfaceProxied(Class<?> intf);

    /**
     * 设置被代理的目标源，创建代理的时候，通常需要传入被代理的对象，最终被代理的对象会被包装为TargetSource类型的
     */
    void setTargetSource(TargetSource targetSource);

    /**
     * 返回被代理的目标源
     */
    TargetSource getTargetSource();

    /**
     * 设置是否需要将代理暴露在ThreadLocal中，这样可以在线程中获取到被代理对象，这个配置挺有用的，稍后会举例说明使用场景
     */
    void setExposeProxy(boolean exposeProxy);

    /**
     * 返回exposeProxy
     */
    boolean isExposeProxy();

    /**
     * 设置此代理配置是否经过预筛选，以便它只包含适用的顾问(匹配此代理的目标类)。
     * 默认设置是“假”。如果已经对advisor进行了预先筛选，则将其设置为“true”
     * 这意味着在为代理调用构建实际的advisor链时可以跳过ClassFilter检查。
     */
    void setPreFiltered(boolean preFiltered);

    /**
     * 返回preFiltered
     */
    boolean isPreFiltered();

    /**
     * 返回代理配置中干掉所有Advisor列表
     */
    Advisor[] getAdvisors();

    /**
     * 添加一个Advisor
     */
    void addAdvisor(Advisor advisor) throws AopConfigException;

    /**
     * 指定的位置添加一个Advisor
     */
    void addAdvisor(int pos, Advisor advisor) throws AopConfigException;

    /**
     * 移除一个Advisor
     */
    boolean removeAdvisor(Advisor advisor);

    /**
     * 移除指定位置的Advisor
     */
    void removeAdvisor(int index) throws AopConfigException;

    /**
     * 查找某个Advisor的位置
     */
    int indexOf(Advisor advisor);

    /**
     * 对advisor列表中的a替换为b
     */
    boolean replaceAdvisor(Advisor a, Advisor b) throws AopConfigException;

    /**
     * 添加一个通知
     */
    void addAdvice(Advice advice) throws AopConfigException;

    /**
     * 向指定的位置添加一个通知
     */
    void addAdvice(int pos, Advice advice) throws AopConfigException;

    /**
     * 移除一个通知
     */
    boolean removeAdvice(Advice advice);

    /**
     * 获取通知的位置
     */
    int indexOf(Advice advice);

    /**
     * 将代理配置转换为字符串，这个方便排错和调试使用的
     */
    String toProxyConfigString();

}
```
<a name="RkDv9"></a>
#### `AdvisedSupport`类
这个类是个重点，AOP代理配置管理器的基类，继承了`ProxyConfig`并且实现了`Advised`接口，创建aop代理之前，所有需要配置的信息都是通过这个类来操作的。<br />比如：设置是否为目标类创建代理、设置目标对象、配置通知列表等等。
```java
package org.springframework.aop.framework;

public class AdvisedSupport extends ProxyConfig implements Advised {

    public static final TargetSource EMPTY_TARGET_SOURCE = EmptyTargetSource.INSTANCE;

    TargetSource targetSource = EMPTY_TARGET_SOURCE;

    /** 建议器是否已经针对特定的目标类进行筛选 */
    private boolean preFiltered = false;

    /** 调用链工厂，用来获取目标方法的调用链 */
    AdvisorChainFactory advisorChainFactory = new DefaultAdvisorChainFactory();

    /** 方法调用链缓存：以方法为键，以顾问链表为值的缓存。 */
    private transient Map<MethodCacheKey, List<Object>> methodCache;

    //代理对象需要实现的接口列表。保存在列表中以保持注册的顺序，以创建具有指定接口顺序的JDK代理。
    private List<Class<?>> interfaces = new ArrayList<>();

    //配置的顾问列表。所有添加的Advise对象都会被包装为Advisor对象
    private List<Advisor> advisors = new ArrayList<>();

    //数组更新了对advisor列表的更改，这更容易在内部操作。
    private Advisor[] advisorArray = new Advisor[0];


    //无参构造方法
    public AdvisedSupport() {
        this.methodCache = new ConcurrentHashMap<>(32);
    }

    //有参构造方法，参数为：代理需要实现的接口列表
    public AdvisedSupport(Class<?>... interfaces) {
        this();
        setInterfaces(interfaces);
    }

    //设置需要被代理的目标对象，目标对象会被包装为TargetSource格式的对象
    public void setTarget(Object target) {
        setTargetSource(new SingletonTargetSource(target));
    }

    //设置被代理的目标源
    @Override
    public void setTargetSource(@Nullable TargetSource targetSource) {
        this.targetSource = (targetSource != null ? targetSource : EMPTY_TARGET_SOURCE);
    }

    //获取被代理的目标源
    @Override
    public TargetSource getTargetSource() {
        return this.targetSource;
    }

    //设置被代理的目标类
    public void setTargetClass(@Nullable Class<?> targetClass) {
        this.targetSource = EmptyTargetSource.forClass(targetClass);
    }

    //获取被代理的目标类型
    @Override
    @Nullable
    public Class<?> getTargetClass() {
        return this.targetSource.getTargetClass();
    }

    /** 
     * 设置此代理配置是否经过预筛选，这个什么意思呢：通过目标方法调用代理的时候，
     * 需要通过匹配的方式获取这个方法上的调用链列表，查找过程需要2个步骤：
     * 第一步：类是否匹配，第二步：方法是否匹配，当这个属性为true的时候，会直接跳过第一步，这个懂了不
     */
    @Override
    public void setPreFiltered(boolean preFiltered) {
        this.preFiltered = preFiltered;
    }

    // 返回preFiltered
    @Override
    public boolean isPreFiltered() {
        return this.preFiltered;
    }

    /**
     * 设置顾问链工厂，当调用目标方法的时候，需要获取这个方法上匹配的Advisor列表，
     * 获取目标方法上匹配的Advisor列表的功能就是AdvisorChainFactory来负责的
     */
    public void setAdvisorChainFactory(AdvisorChainFactory advisorChainFactory) {
        Assert.notNull(advisorChainFactory, "AdvisorChainFactory must not be null");
        this.advisorChainFactory = advisorChainFactory;
    }

    // 返回顾问链工厂对象
    public AdvisorChainFactory getAdvisorChainFactory() {
        return this.advisorChainFactory;
    }


    //设置代理对象需要实现的接口
    public void setInterfaces(Class<?>... interfaces) {
        Assert.notNull(interfaces, "Interfaces must not be null");
        this.interfaces.clear();
        for (Class<?> ifc : interfaces) {
            addInterface(ifc);
        }
    }

    //为代理对象添加需要实现的接口
    public void addInterface(Class<?> intf) {
        Assert.notNull(intf, "Interface must not be null");
        if (!intf.isInterface()) {
            throw new IllegalArgumentException("[" + intf.getName() + "] is not an interface");
        }
        if (!this.interfaces.contains(intf)) {
            this.interfaces.add(intf);
            adviceChanged();
        }
    }

    //移除代理对象需要实现的接口
    public boolean removeInterface(Class<?> intf) {
        return this.interfaces.remove(intf);
    }

    //获取代理对象需要实现的接口列表
    @Override
    public Class<?>[] getProxiedInterfaces() {
        return ClassUtils.toClassArray(this.interfaces);
    }

    //判断代理对象是否需要实现某个接口
    @Override
    public boolean isInterfaceProxied(Class<?> intf) {
        for (Class<?> proxyIntf : this.interfaces) {
            if (intf.isAssignableFrom(proxyIntf)) {
                return true;
            }
        }
        return false;
    }

    //获取配置的所有顾问列表
    @Override
    public final Advisor[] getAdvisors() {
        return this.advisorArray;
    }

    //添加顾问
    @Override
    public void addAdvisor(Advisor advisor) {
        int pos = this.advisors.size();
        addAdvisor(pos, advisor);
    }

    //指定的位置添加顾问
    @Override
    public void addAdvisor(int pos, Advisor advisor) throws AopConfigException {
        //这块先忽略，以后讲解
        if (advisor instanceof IntroductionAdvisor) {
            validateIntroductionAdvisor((IntroductionAdvisor) advisor);
        }
        addAdvisorInternal(pos, advisor);
    }

    //移除指定的顾问
    @Override
    public boolean removeAdvisor(Advisor advisor) {
        int index = indexOf(advisor);
        if (index == -1) {
            return false;
        }
        else {
            removeAdvisor(index);
            return true;
        }
    }

    //移除指定位置的顾问
    @Override
    public void removeAdvisor(int index) throws AopConfigException {
        //当配置如果是冻结状态，是不允许对顾问进行修改的，否则会抛出异常
        if (isFrozen()) {
            throw new AopConfigException("Cannot remove Advisor: Configuration is frozen.");
        }
        if (index < 0 || index > this.advisors.size() - 1) {
            throw new AopConfigException("Advisor index " + index + " is out of bounds: " +
                    "This configuration only has " + this.advisors.size() + " advisors.");
        }
        //移除advisors中的顾问
        Advisor advisor = this.advisors.remove(index);
        if (advisor instanceof IntroductionAdvisor) {
            IntroductionAdvisor ia = (IntroductionAdvisor) advisor;
            // We need to remove introduction interfaces.
            for (Class<?> ifc : ia.getInterfaces()) {
                removeInterface(ifc);
            }
        }
        //更新advisorArray
        updateAdvisorArray();
        //通知已改变，内部会清除方法调用链缓存信息。
        adviceChanged();
    }

    @Override
    public int indexOf(Advisor advisor) {
        Assert.notNull(advisor, "Advisor must not be null");
        return this.advisors.indexOf(advisor);
    }

    @Override
    public boolean replaceAdvisor(Advisor a, Advisor b) throws AopConfigException {
        Assert.notNull(a, "Advisor a must not be null");
        Assert.notNull(b, "Advisor b must not be null");
        int index = indexOf(a);
        if (index == -1) {
            return false;
        }
        removeAdvisor(index);
        addAdvisor(index, b);
        return true;
    }

    //批量添加顾问
    public void addAdvisors(Advisor... advisors) {
        addAdvisors(Arrays.asList(advisors));
    }

    //批量添加顾问
    public void addAdvisors(Collection<Advisor> advisors) {
        //配置如果是冻结状态，会抛出异常
        if (isFrozen()) {
            throw new AopConfigException("Cannot add advisor: Configuration is frozen.");
        }
        if (!CollectionUtils.isEmpty(advisors)) {
            for (Advisor advisor : advisors) {
                if (advisor instanceof IntroductionAdvisor) {
                    validateIntroductionAdvisor((IntroductionAdvisor) advisor);
                }
                Assert.notNull(advisor, "Advisor must not be null");
                this.advisors.add(advisor);
            }
            updateAdvisorArray();
            adviceChanged();
        }
    }

    //此方法先忽略，用来为目标类引入接口的
    private void validateIntroductionAdvisor(IntroductionAdvisor advisor) {
        advisor.validateInterfaces();
        // If the advisor passed validation, we can make the change.
        Class<?>[] ifcs = advisor.getInterfaces();
        for (Class<?> ifc : ifcs) {
            addInterface(ifc);
        }
    }

    //指定的位置添加顾问
    private void addAdvisorInternal(int pos, Advisor advisor) throws AopConfigException {
        Assert.notNull(advisor, "Advisor must not be null");
        if (isFrozen()) {
            throw new AopConfigException("Cannot add advisor: Configuration is frozen.");
        }
        if (pos > this.advisors.size()) {
            throw new IllegalArgumentException(
                    "Illegal position " + pos + " in advisor list with size " + this.advisors.size());
        }
        this.advisors.add(pos, advisor);
        updateAdvisorArray();
        adviceChanged();
    }

    //将advisorArray和advisors保持一致
    protected final void updateAdvisorArray() {
        this.advisorArray = this.advisors.toArray(new Advisor[0]);
    }

    //获取顾问列表
    protected final List<Advisor> getAdvisorsInternal() {
        return this.advisors;
    }

    //添加通知
    @Override
    public void addAdvice(Advice advice) throws AopConfigException {
        int pos = this.advisors.size();
        addAdvice(pos, advice);
    }

    //指定的位置添加通知
    @Override
    public void addAdvice(int pos, Advice advice) throws AopConfigException {
        //此处会将advice通知包装为DefaultPointcutAdvisor类型的Advisor
        addAdvisor(pos, new DefaultPointcutAdvisor(advice));
    }

    //移除通知
    @Override
    public boolean removeAdvice(Advice advice) throws AopConfigException {
        int index = indexOf(advice);
        if (index == -1) {
            return false;
        }
        else {
            removeAdvisor(index);
            return true;
        }
    }

    //获取通知的位置
    @Override
    public int indexOf(Advice advice) {
        Assert.notNull(advice, "Advice must not be null");
        for (int i = 0; i < this.advisors.size(); i++) {
            Advisor advisor = this.advisors.get(i);
            if (advisor.getAdvice() == advice) {
                return i;
            }
        }
        return -1;
    }

    //是否包含某个通知
    public boolean adviceIncluded(@Nullable Advice advice) {
        if (advice != null) {
            for (Advisor advisor : this.advisors) {
                if (advisor.getAdvice() == advice) {
                    return true;
                }
            }
        }
        return false;
    }

    //获取当前配置中某种类型通知的数量
    public int countAdvicesOfType(@Nullable Class<?> adviceClass) {
        int count = 0;
        if (adviceClass != null) {
            for (Advisor advisor : this.advisors) {
                if (adviceClass.isInstance(advisor.getAdvice())) {
                    count++;
                }
            }
        }
        return count;
    }


    //基于当前配置，获取给定方法的方法调用链列表（即org.aopalliance.intercept.MethodInterceptor对象列表）
    public List<Object> getInterceptorsAndDynamicInterceptionAdvice(Method method, @Nullable Class<?> targetClass) {
        MethodCacheKey cacheKey = new MethodCacheKey(method);
        //先从缓存中获取
        List<Object> cached = this.methodCache.get(cacheKey);
        //缓存中没有时，从advisorChainFactory中获取
        if (cached == null) {
            cached = this.advisorChainFactory.getInterceptorsAndDynamicInterceptionAdvice(
                    this, method, targetClass);
            this.methodCache.put(cacheKey, cached);
        }
        return cached;
    }

    //通知更改时调用，会清空当前方法调用链缓存
    protected void adviceChanged() {
        this.methodCache.clear();
    }

    //将other中的配置信息复制到当前对象中
    protected void copyConfigurationFrom(AdvisedSupport other) {
        copyConfigurationFrom(other, other.targetSource, new ArrayList<>(other.advisors));
    }

    //将other中的配置信息复制到当前对象中
    protected void copyConfigurationFrom(AdvisedSupport other, TargetSource targetSource, List<Advisor> advisors) {
        copyFrom(other);
        this.targetSource = targetSource;
        this.advisorChainFactory = other.advisorChainFactory;
        this.interfaces = new ArrayList<>(other.interfaces);
        for (Advisor advisor : advisors) {
            if (advisor instanceof IntroductionAdvisor) {
                validateIntroductionAdvisor((IntroductionAdvisor) advisor);
            }
            Assert.notNull(advisor, "Advisor must not be null");
            this.advisors.add(advisor);
        }
        updateAdvisorArray();
        adviceChanged();
    }

    //构建此AdvisedSupport的仅配置副本，替换TargetSource。
    AdvisedSupport getConfigurationOnlyCopy() {
        AdvisedSupport copy = new AdvisedSupport();
        copy.copyFrom(this);
        copy.targetSource = EmptyTargetSource.forClass(getTargetClass(), getTargetSource().isStatic());
        copy.advisorChainFactory = this.advisorChainFactory;
        copy.interfaces = this.interfaces;
        copy.advisors = this.advisors;
        copy.updateAdvisorArray();
        return copy;
    }
}
```
上面几个类有几个结论，这里说一下。

1. 配置中添加的`Advice`对象最终都会被转换为`DefaultPointcutAdvisor`对象，此时`DefaultPointcutAdvisor`未指定pointcut，大家可以去看一下`DefaultPointcutAdvisor`中pointcut有个默认值，默认会匹配任意类的任意方法。
2. 当配置被冻结的时候，即frozen为true的时，此时配置中的`Advisor`列表是不允许修改的。
3. 上面的`getInterceptorsAndDynamicInterceptionAdvice`方法，通过代理调用目标方法的时候，最后需要通过方法和目标类的类型，从当前配置中会获取匹配的方法拦截器列表，获取方法拦截器列表是由`AdvisorChainFactory`负责的。`getInterceptorsAndDynamicInterceptionAdvice`会在调用代理的方法时会执行，稍后在执行阶段会详解。
4. 目标方法和其关联的方法拦截器列表会被缓存在`methodCache`中，当顾问列表有变化的时候，`methodCache`缓存会被清除。

**配置阶段完成之后，下面进入**`**AopProxy**`**获取阶段。**
<a name="Aggwn"></a>
### 根据配置获取`AopProxy`
这个阶段对应的代码：
```java
// 创建AopProxy使用了简单工厂模式
AopProxyFactory aopProxyFactory = new DefaultAopProxyFactory();
//通过AopProxy工厂获取AopProxy对象
AopProxy aopProxy = aopProxyFactory.createAopProxy(advisedSupport);
```
此阶段会根据`AdvisedSupport`中配置信息，判断具体是采用cglib的方式还是采用jdk动态代理的方式获取代理对象，先看一下涉及到的一些类。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684331311727-de3fc96c-167d-4daa-845f-c395787138e3.png#averageHue=%23fcfbfa&clientId=u4c06a739-18d6-4&from=paste&id=u36b04a59&originHeight=439&originWidth=506&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u81ee9ca8-eec8-42ef-a199-9c8734c85db&title=)
<a name="ideQy"></a>
#### `AopProxy`接口
这个接口定义了一个方法，用来创建最终的代理对象，这个接口有2个实现类

- `CglibAopProxy`：采用cglib的方式创建代理对象
- `JkdDynamicAopProxy`：采用jdk动态代理的方式创建代理对象
```java
package org.springframework.aop.framework;

public interface AopProxy {

    /**
     * 创建一个新的代理对象
     */
    Object getProxy();

    /**
     * 创建一个新的代理对象
     */
    Object getProxy(@Nullable ClassLoader classLoader);

}
```
`AopProxy`的2个实现类，实现了上面定义的2个方法，稍后在代理的创建阶段详细介绍。
<a name="IOnHi"></a>
#### `AopProxyFactory`接口
通过名称就可以看出来，是一个工厂，负责创建`AopProxy`，使用的是简单工厂模式。<br />接口中定义了一个方法，会根据Aop的配置信息`AdvisedSupport`来获取`AopProxy`对象，主要是判断采用cglib的方式还是采用jdk动态代理的方式。
```java
package org.springframework.aop.framework;

public interface AopProxyFactory {
    /**
     * 根据aop配置信息获取AopProxy对象
     */
    AopProxy createAopProxy(AdvisedSupport config) throws AopConfigException;
}
```
<a name="hwd3D"></a>
#### `DefaultAopProxyFactory`类
`AopProxyFactory`接口的默认实现，代码比较简单，来细看一下
```java
package org.springframework.aop.framework;

/**
 * 默认AopProxyFactory实现，创建CGLIB代理或JDK动态代理。
 * 对于给定的AdvisedSupport实例，以下条件为真，则创建一个CGLIB代理:
 * optimize = true
 * proxyTargetClass = true
 * 未指定代理接口
 * 通常，指定proxyTargetClass来强制执行CGLIB代理，或者指定一个或多个接口来使用JDK动态代理。
 */
public class DefaultAopProxyFactory implements AopProxyFactory, Serializable {

    @Override
    public AopProxy createAopProxy(AdvisedSupport config) throws AopConfigException {
        // optimize==true || proxyTargetClass 为true || 配置中没有需要代理的接口
        if (config.isOptimize() || config.isProxyTargetClass() || hasNoUserSuppliedProxyInterfaces(config)) {
            //获取需要被代理的类
            Class<?> targetClass = config.getTargetClass();
            if (targetClass == null) {
                throw new AopConfigException("TargetSource cannot determine target class: " +
                                             "Either an interface or a target is required for proxy creation.");
            }
            //如果被代理的类为接口 或者 被代理的类是jdk动态代理创建代理类，则采用JdkDynamicAopProxy的方式，否则采用cglib代理的方式
            if (targetClass.isInterface() || Proxy.isProxyClass(targetClass)) {
                //采用jdk动态代理的方式
                return new JdkDynamicAopProxy(config);
            }
            //采用cglib代理的方式
            return new ObjenesisCglibAopProxy(config);
        }
        else {
            //采用jdk动态代理的方式
            return new JdkDynamicAopProxy(config);
        }
    }

    /**
     * 确定所提供的AdvisedSupport是否只指定了SpringProxy接口(或者根本没有指定代理接口)
     */
    private boolean hasNoUserSuppliedProxyInterfaces(AdvisedSupport config) {
        Class<?>[] ifcs = config.getProxiedInterfaces();
        return (ifcs.length == 0 || (ifcs.length == 1 && SpringProxy.class.isAssignableFrom(ifcs[0])));
    }

}
```
<a name="hpH8y"></a>
### 代理创建阶段
到目前为止已经根据aop配置信息得到了`AopProxy`对象了，下面就可以调用`AopProxy.getProxy`方法获取代理对象了。<br />`AopProxy.createAopProxy`方法返回的结果有2种情况

- `JdkDynamicAopProxy`：以jdk动态代理的方式创建代理
- `ObjenesisCglibAopProxy`：以cglib的方式创建动态代理

项目详解这2个类的源码 。
<a name="aNLoR"></a>
#### `JdkDynamicAopProxy`类
**作用：采用jdk动态代理的方式创建代理对象，并处理代理对象的所有方法调用。**
```java
final class JdkDynamicAopProxy implements AopProxy, InvocationHandler, Serializable {

    //代理的配置信息
    private final AdvisedSupport advised;

    //需要被代理的接口中是否定义了equals方法
    private boolean equalsDefined;

    //需要被代理的接口中是否定义了hashCode方法
    private boolean hashCodeDefined;

    //通过AdvisedSupport创建实例
    public JdkDynamicAopProxy(AdvisedSupport config) throws AopConfigException {
        Assert.notNull(config, "AdvisedSupport must not be null");
        if (config.getAdvisors().length == 0 && config.getTargetSource() == AdvisedSupport.EMPTY_TARGET_SOURCE) {
            throw new AopConfigException("No advisors and no TargetSource specified");
        }
        this.advised = config;
    }

    //生成一个代理对象
    @Override
    public Object getProxy() {
        return getProxy(ClassUtils.getDefaultClassLoader());
    }

    //生成一个代理对象
    @Override
    public Object getProxy(@Nullable ClassLoader classLoader) {
        if (logger.isTraceEnabled()) {
            logger.trace("Creating JDK dynamic proxy: " + this.advised.getTargetSource());
        }
        //@0：根据advised的信息获取代理需要被代理的所有接口列表
        Class<?>[] proxiedInterfaces = AopProxyUtils.completeProxiedInterfaces(this.advised, true); 
        //查找被代理的接口中是否定义了equals、hashCode方法
        findDefinedEqualsAndHashCodeMethods(proxiedInterfaces);
        /**
         * 这个大家应该很熟悉吧，通过jdk动态代理创建代理对象，注意最后一个参数是this
         * 表示当前类，当前类是InvocationHandler类型的，当调用代理对象的任何方法的时候
         * 都会被被当前类的 invoke 方法处理
         */
        return Proxy.newProxyInstance(classLoader, proxiedInterfaces, this);
    }

    //判断需要代理的接口中是否定义了这几个方法（equals、hashCode）
    private void findDefinedEqualsAndHashCodeMethods(Class<?>[] proxiedInterfaces) {
        for (Class<?> proxiedInterface : proxiedInterfaces) {
            //获取接口中定义的方法
            Method[] methods = proxiedInterface.getDeclaredMethods();
            for (Method method : methods) {
                //是否是equals方法
                if (AopUtils.isEqualsMethod(method)) {
                    this.equalsDefined = true;
                }
                //是否是hashCode方法
                if (AopUtils.isHashCodeMethod(method)) {
                    this.hashCodeDefined = true;
                }
                //如果发现这2个方法都定义了，结束循环查找
                if (this.equalsDefined && this.hashCodeDefined) {
                    return;
                }
            }
        }
    }


    // 这个方法比较关键了，当在程序中调用代理对象的任何方法，最终都会被下面这个invoke方法处理
    public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
        //旧的代理对象
        Object oldProxy = null;
        //用来标记是否需要将代理对象暴露在ThreadLocal中
        boolean setProxyContext = false;
        //获取目标源
        TargetSource targetSource = this.advised.targetSource;
        //目标对象
        Object target = null;

        //下面进入代理方法的处理阶段
        try {
            // 处理equals方法：被代理的接口中没有定义equals方法 && 当前调用是equals方法
            if (!this.equalsDefined && AopUtils.isEqualsMethod(method)) {
                // 直接调用当前类中的equals方法
                return equals(args[0]);
            }
            // 处理hashCode方法：被代理的接口中没有定义hashCode方法 && 当前调用是hashCode方法
            else if (!this.hashCodeDefined && AopUtils.isHashCodeMethod(method)) {
                // 直接调用当前类中的hashCode方法
                return hashCode();
            }
            /**
             * 方法来源于 DecoratingProxy 接口，这个接口中定义了一个方法
             * 用来获取原始的被代理的目标类，主要是用在嵌套代理的情况下（所谓嵌套代理：代理对象又被作为目标对象进行了代理）
             */
            else if (method.getDeclaringClass() == DecoratingProxy.class) {
                // 调用AopProxyUtils工具类的方法，内部通过循环遍历的方式，找到最原始的被代理的目标类
                return AopProxyUtils.ultimateTargetClass(this.advised);
            }
            // 方法来源于 Advised 接口，代理对象默认情况下会实现 Advised 接口，可以通过代理对象来动态向代理对象中添加通知等
            else if (!this.advised.opaque && method.getDeclaringClass().isInterface() &&
                    method.getDeclaringClass().isAssignableFrom(Advised.class)) {
                // this.advised是AdvisedSupport类型的，AdvisedSupport实现了Advised接口中的所有方法
                // 所以最终通过通过反射方式交给this.advised来响应当前调用
                return AopUtils.invokeJoinpointUsingReflection(this.advised, method, args);
            }
            // 用来记录方法返回值
            Object retVal;

            //是否需要在threadLocal中暴露代理对象
            if (this.advised.exposeProxy) {
                // 将代理对象暴露在上线文中，即暴露在threadLocal中，那么在当前线程中可以通过静态方法
                // AopContext#currentProxy获取当前被暴露的代理对象，这个是非常有用的，稍后用案例来讲解，瞬间就会明白
                oldProxy = AopContext.setCurrentProxy(proxy);
                // 将setProxyContext标记为true
                setProxyContext = true;
            }

            // 通过目标源获取目标对象
            target = targetSource.getTarget();
            // 获取目标对象类型
            Class<?> targetClass = (target != null ? target.getClass() : null);

            // @1：获取当前方法的拦截器链
            List<Object> chain = this.advised.getInterceptorsAndDynamicInterceptionAdvice(method, targetClass); 

            // 拦截器链为空的情况下，表示这个方法上面没有找到任何增强的通知，那么会直接通过反射直接调用目标对象
            if (chain.isEmpty()) {
                // 获取方法请求的参数（有时候方法中有可变参数，所谓可变参数就是带有省略号(...)这种格式的参数，传入的参数类型和这种类型不一样的时候，会通过下面的adaptArgumentsIfNecessary方法进行转换）
                Object[] argsToUse = AopProxyUtils.adaptArgumentsIfNecessary(method, args);
                //通过反射直接调用目标方法
                retVal = AopUtils.invokeJoinpointUsingReflection(target, method, argsToUse);
            }
            else {
                // 创建一个方法调用器（包含了代理对象、目标对象、调用的方法、参数、目标类型、方法拦截器链）
                MethodInvocation invocation =
                        new ReflectiveMethodInvocation(proxy, target, method, args, targetClass, chain);
                // @3：通过拦截器链一个个调用最终到目标方法的调用
                retVal = invocation.proceed();
            }

            // 下面会根据方法返回值的类型，做一些处理，比如方法返回的类型为自己，则最后需要将返回值置为代理对象
            Class<?> returnType = method.getReturnType();
            if (retVal != null && retVal == target &&
                    returnType != Object.class && returnType.isInstance(proxy) &&
                    !RawTargetAccess.class.isAssignableFrom(method.getDeclaringClass())) {
                // 将返回值设置为代理对象
                retVal = proxy;
            }
            // 方法的返回值类型returnType为原始类型（即int、byte、double等这种类型的） && retVal为null，
            // 此时如果将null转换为原始类型会报错，所以此处直接抛出异常
            else if (retVal == null && returnType != Void.TYPE && returnType.isPrimitive()) {
                throw new AopInvocationException(
                        "Null return value from advice does not match primitive return type for: " + method);
            }
            // 返回方法调用结果
            return retVal;
        }
        finally {
            // 目标对象不为null && 目标源不是静态的
            //所谓静态的，你可以理解为是否是单例的
            // isStatic为true，表示目标对象是单例的，同一个代理对象中所有方法共享一个目标对象
            // isStatic为false的时候，通常每次调用代理的方法，target对象是不一样的，所以方法调用万之后需要进行释放，可能有些资源清理，连接的关闭等操作
            if (target != null && !targetSource.isStatic()) {
                // 必须释放来自TargetSource中的目标对象
                targetSource.releaseTarget(target);
            }
            // setProxyContext为ture
            if (setProxyContext) {
                // 需要将旧的代理再放回到上线文中
                AopContext.setCurrentProxy(oldProxy);
            }
        }
    }

}
```
关于上面代码，有几点细说一下
<a name="bbrDk"></a>
##### @0：`completeProxiedInterfaces`方法
@0处的代码如下，根据代理配置信息，获取需要被代理的所有接口
```java
Class<?>[] proxiedInterfaces = AopProxyUtils.completeProxiedInterfaces(this.advised, true);
```
`AopProxyUtils.completeProxiedInterfaces`方法源码如下
```java
static Class<?>[] completeProxiedInterfaces(AdvisedSupport advised, boolean decoratingProxy) {
    //获取代理配置中需要被代理的接口
    Class<?>[] specifiedInterfaces = advised.getProxiedInterfaces();
    // 需要被代理的接口数量为0
    if (specifiedInterfaces.length == 0) {
        // 获取需要被代理的目标类型
        Class<?> targetClass = advised.getTargetClass();
        //目标类型不为空
        if (targetClass != null) {
            //目标类型为接口
            if (targetClass.isInterface()) {
                //将其添加到需要代理的接口中
                advised.setInterfaces(targetClass);
            }
                // 目标类型为jdk动态代理创建的代理对象
            else if (Proxy.isProxyClass(targetClass)) {
                // 获取目标类型上的所有接口，将其添加到需要被代理的接口中
                advised.setInterfaces(targetClass.getInterfaces());
            }
            //再次获取代理配置中需要被代理的接口
            specifiedInterfaces = advised.getProxiedInterfaces();
        }
    }
    //判断SpringProxy接口是否已经在被代理的接口中
    boolean addSpringProxy = !advised.isInterfaceProxied(SpringProxy.class);
    //判断Advised接口是否已经在被代理的接口中
    boolean addAdvised = !advised.isOpaque() && !advised.isInterfaceProxied(Advised.class);
    //判断DecoratingProxy接口是否已经在被代理的接口中
    boolean addDecoratingProxy = (decoratingProxy && !advised.isInterfaceProxied(DecoratingProxy.class));
    //一个计数器，会根据上面三个boolean值做递增
    int nonUserIfcCount = 0;
    if (addSpringProxy) {
        nonUserIfcCount++;
    }
    if (addAdvised) {
        nonUserIfcCount++;
    }
    if (addDecoratingProxy) {
        nonUserIfcCount++;
    }
    // 下面就是构建所有需要被代理的接口
    Class<?>[] proxiedInterfaces = new Class<?>[specifiedInterfaces.length + nonUserIfcCount];
    System.arraycopy(specifiedInterfaces, 0, proxiedInterfaces, 0, specifiedInterfaces.length);
    int index = specifiedInterfaces.length;
    if (addSpringProxy) {
        proxiedInterfaces[index] = SpringProxy.class;
        index++;
    }
    if (addAdvised) {
        proxiedInterfaces[index] = Advised.class;
        index++;
    }
    if (addDecoratingProxy) {
        proxiedInterfaces[index] = DecoratingProxy.class;
    }
    return proxiedInterfaces;
}
```
上面的方法执行完毕之后，会得到一个被代理的接口列表，默认情况下会得到下面的一个列表
```
[开发者硬编码指定的需要被代理的接口列表,SpringProxy,Advised,DecoratingProxy]
```
最终创建出来的代理对象，默认会实现上面列的所有接口，后面3个接口是aop中自动加上的。
<a name="hFa02"></a>
##### @1：`getInterceptorsAndDynamicInterceptionAdvice`
这个方法位于`AdvisedSupport`中，根据方法和目标类型获取方法上面匹配的拦截器链
```java
public List<Object> getInterceptorsAndDynamicInterceptionAdvice(Method method, @Nullable Class<?> targetClass) {
    //会先尝试从还中获取，如果获取不到，会从advisorChainFactory中获取，然后将其丢到缓存中
    MethodCacheKey cacheKey = new MethodCacheKey(method);
    List<Object> cached = this.methodCache.get(cacheKey);
    if (cached == null) {
        cached = this.advisorChainFactory.getInterceptorsAndDynamicInterceptionAdvice(
            this, method, targetClass);
        this.methodCache.put(cacheKey, cached);
    }
    return cached;
}
```
**从**`**advisorChainFactory**`**中获取拦截器链稍后细说，把这个阶段叫做连接器链的获取阶段。**
<a name="aAzcS"></a>
##### @3：`ReflectiveMethodInvocation.proceed()`
这个是一次会调用拦截器链，最终会调用到目标方法，获得目标方法的返回值，里面的细节见后面的代理方法调用处理阶段
<a name="xnq2A"></a>
#### `JdkDynamicAopProxy`小结

1. 被创建的代理对象默认会实现`SpringProxy`,`Advised`,`DecoratingProxy` 3个接口
2. `SpringProxy`这个接口中没有任何方法，只是起一个标记作用，用来标记代理对象是使用Spring AOP创建的
3. 代理对象默认都会实现`Advised`接口，所以可以通过这个接口动态变更代理对象中的通知
4. `DecoratingProxy`接口中定义了一个方法`getDecoratedClass`，用来获取被代理的原始目标对象的类型

下面来看另外一个类：`ObjenesisCglibAopProxy`，这个继承了`CglibAopProxy`，大部分逻辑都在`CglibAopProxy`中，所以主要看`CglibAopProxy`中代码。
<a name="QP3er"></a>
#### `CglibAopProxy`类
**作用：采用cglib代理的方式创建代理对象，并处理代理对象的所有方法调用。**<br />以`getProxy`方法为入口，通过方法一个个来解说。
<a name="vCgKv"></a>
##### `getProxy`方法
```java
public Object getProxy(@Nullable ClassLoader classLoader) {
    // 获取被代理的类
    Class<?> rootClass = this.advised.getTargetClass();

    // 代理对象的父类（cglib是采用继承的方式是创建代理对象的，所以将被代理的类作为代理对象的父类）
    Class<?> proxySuperClass = rootClass;
    // 判断被代理的类是不是cglib创建的类，如果是cblib创建的类，会将其父类作为被代理的类
    if (rootClass.getName().contains(ClassUtils.CGLIB_CLASS_SEPARATOR)) {
        proxySuperClass = rootClass.getSuperclass();
        //添加需要被代理的接口
        Class<?>[] additionalInterfaces = rootClass.getInterfaces();
        for (Class<?> additionalInterface : additionalInterfaces) {
            this.advised.addInterface(additionalInterface);
        }
    }

    // 开始cglib创建代理，这个大家对cglib比较熟悉的一看就懂
    Enhancer enhancer = createEnhancer();
    // 设置被代理的父类
    enhancer.setSuperclass(proxySuperClass);
    // 设置被代理的接口[开发者硬编码指定的需要被代理的接口列表,SpringProxy,Advised]，这个比jdk动态代理的方式少了一个DecoratingProxy接口
    enhancer.setInterfaces(AopProxyUtils.completeProxiedInterfaces(this.advised));
    // 设置代理类类名生成策略
    enhancer.setNamingPolicy(SpringNamingPolicy.INSTANCE);
    // 设置字节码的生成策略
    enhancer.setStrategy(new ClassLoaderAwareGeneratorStrategy(classLoader));

    // @1：获取Callback列表，这个稍后详解
    Callback[] callbacks = getCallbacks(rootClass);
    Class<?>[] types = new Class<?>[callbacks.length];
    for (int x = 0; x < types.length; x++) {
        types[x] = callbacks[x].getClass();
    }
    // @2：设置CallbackFilter，CallbackFilter内部会判断被代理对象中的方法最终会被callbacks列表中的那个Callback来处理
    enhancer.setCallbackFilter(new ProxyCallbackFilter(
        this.advised.getConfigurationOnlyCopy(), this.fixedInterceptorMap, this.fixedInterceptorOffset));
    enhancer.setCallbackTypes(types);

    // 获取代理对象（内部会先创建代理类，然后会根据代理类生成一个代理对象）
    return createProxyClassAndInstance(enhancer, callbacks);
}
```
上面方法中有2个点比较难，需要说明，分别是@1：`getCallbacks`方法和@2:创建`ProxyCallbackFilter`对象
<a name="r1uUG"></a>
##### @1：`getCallbacks`方法
通过被代理的类来获取`Callback`列表，`Callback`是用来处理代理对象的方法调用的，代理对象中可能有很多方法，每个方法可能采用不同的处理方式，所以会有多个`Callback`
```java
private Callback[] getCallbacks(Class<?> rootClass) throws Exception {
    // 是否需要将代理暴露在threadLocal中
    boolean exposeProxy = this.advised.isExposeProxy();
    // 配置是否是冻结的
    boolean isFrozen = this.advised.isFrozen();
    // 被代理的目标对象是否是动态的（是否是单例的）
    boolean isStatic = this.advised.getTargetSource().isStatic();

    // 当方法上有需要执行的拦截器的时候，会用这个来处理
    Callback aopInterceptor = new DynamicAdvisedInterceptor(this.advised);

    // 当方法上没有需要执行的拦截器的时候，会使用targetInterceptor来处理，内部会通过反射直接调用目标对象的方法
    Callback targetInterceptor;
    /**
     * 这块根据是否需要暴露代理到threadLocal中以及目标对象是否是动态的，会创建不同的Callback
     * isStatic为true的时候，同一个代理的不同方法可能都是新的目标对象，所以当代理方法执行完毕之后，需要对目标对象进行释放
     */
    if (exposeProxy) {
        targetInterceptor = (isStatic ?
                             new StaticUnadvisedExposedInterceptor(this.advised.getTargetSource().getTarget()) :
                             new DynamicUnadvisedExposedInterceptor(this.advised.getTargetSource()));
    }
    else {
        targetInterceptor = (isStatic ?
                             new StaticUnadvisedInterceptor(this.advised.getTargetSource().getTarget()) :
                             new DynamicUnadvisedInterceptor(this.advised.getTargetSource()));
    }

    // targetDispatcher会直接调用目标方法
    Callback targetDispatcher = (isStatic ?
                                 new StaticDispatcher(this.advised.getTargetSource().getTarget()) : new SerializableNoOp());

    Callback[] mainCallbacks = new Callback[] {
        aopInterceptor,  // 处理匹配到拦截器的方法
        targetInterceptor,  // 处理未匹配到拦截器的方法
        new SerializableNoOp(), 
        targetDispatcher,  // 处理未匹配到拦截器的方法，和targetInterceptor有何不同呢？目标方法如果返回值的结果是目标对象类型的，会使用 targetInterceptor 处理，内部会返回代理对象
        this.advisedDispatcher, // 处理Advised接口中定义的方法
        new EqualsInterceptor(this.advised), // 处理equals方法
        new HashCodeInterceptor(this.advised) // 处理hashCode方法
    };

    Callback[] callbacks;

    // 如果被代理的对象是单例的 && 配置是冻结的，此时会进行优化，怎么优化呢？
    // 配置冻结的情况下，生成好的代理中通知是无法修改的，所以可以提前将每个方法对应的拦截器链找到给缓存起来
    // 调用方法的时候，就直接从缓存中可以拿到方法对应的缓存信息，效率会高一些
    if (isStatic && isFrozen) {
        Method[] methods = rootClass.getMethods();
        Callback[] fixedCallbacks = new Callback[methods.length];
        this.fixedInterceptorMap = new HashMap<>(methods.length);

        // 获取每个方法的调用链，然后给缓存在fixedInterceptorMap中
        for (int x = 0; x < methods.length; x++) {
            Method method = methods[x];
            List<Object> chain = this.advised.getInterceptorsAndDynamicInterceptionAdvice(method, rootClass);
            fixedCallbacks[x] = new FixedChainStaticTargetInterceptor(
                chain, this.advised.getTargetSource().getTarget(), this.advised.getTargetClass());
            this.fixedInterceptorMap.put(method, x);
        }
        callbacks = new Callback[mainCallbacks.length + fixedCallbacks.length];
        System.arraycopy(mainCallbacks, 0, callbacks, 0, mainCallbacks.length);
        System.arraycopy(fixedCallbacks, 0, callbacks, mainCallbacks.length, fixedCallbacks.length);
                         this.fixedInterceptorOffset = mainCallbacks.length;
                         }
                         else {
                         callbacks = mainCallbacks;
                         }
                         return callbacks;
                         }
```
<a name="cvrwd"></a>
##### @2:创建`ProxyCallbackFilter`对象
```java
enhancer.setCallbackFilter(new ProxyCallbackFilter(
    this.advised.getConfigurationOnlyCopy(), this.fixedInterceptorMap, this.fixedInterceptorOffset));
```
这块重点在于`ProxyCallbackFilter`中的`accept`方法，这个方法会根据目标放，获取目标对方最后会让callbacks列表中的哪个Callback处理，大家可以看一下源码，比较简单。<br />上面`getCallbacks`方法中涉及到了5个类如下

- `DynamicAdvisedInterceptor`
- `StaticUnadvisedExposedInterceptor`
- `StaticUnadvisedInterceptor`
- `DynamicUnadvisedInterceptor`
- `StaticDispatcher`

后面4个比较简单，大家可以去看一下源码，主要来看第一个类，基本上代理对象中的大部分自定义的方法都会进入到这个类的`intercept`方法中进行处理，代码如下
<a name="kcW5q"></a>
##### `DynamicAdvisedInterceptor`类
```java
private static class DynamicAdvisedInterceptor implements MethodInterceptor, Serializable {
    //代理配置信息
    private final AdvisedSupport advised;
    //构造器，需要一个AdvisedSupport
    public DynamicAdvisedInterceptor(AdvisedSupport advised) {
        this.advised = advised;
    }

    //这个方法是关键，用来处理代理对象中方法的调用
    public Object intercept(Object proxy, Method method, Object[] args, MethodProxy methodProxy) throws Throwable {
        //被暴露在threadLocal中旧的代理对象
        Object oldProxy = null;
        //用来标记代理对象是否被暴露在threadLocal中
        boolean setProxyContext = false;
        //目标对象
        Object target = null;
        //目标源
        TargetSource targetSource = this.advised.getTargetSource();
        try {
            //代理配置中是否需要将代理暴露在threadLocal中
            if (this.advised.exposeProxy) {
                //将代理对象暴露出去
                oldProxy = AopContext.setCurrentProxy(proxy);
                //将setProxyContext置为true
                setProxyContext = true;
            }
            //获取目标对象（即被代理的对象）
            target = targetSource.getTarget();
            Class<?> targetClass = (target != null ? target.getClass() : null);
            //@1：获取当前方法的拦截器链
            List<Object> chain = this.advised.getInterceptorsAndDynamicInterceptionAdvice(method, targetClass);
            //记录方法返回值
            Object retVal;
            //拦截器链不为空 && 方法是public类型的
            if (chain.isEmpty() && Modifier.isPublic(method.getModifiers())) {
                //获取方法调用参数
                Object[] argsToUse = AopProxyUtils.adaptArgumentsIfNecessary(method, args);
                // 直接调用目标对象的方法
                retVal = methodProxy.invoke(target, argsToUse);
            }
            else {
                // 创建一个方法调用器（包含了代理对象、目标对象、调用的方法、参数、目标类型、方法拦截器链）
                // @2：并执行方法调用器的processd()方法，此方法会一次执行方法调用链，最终会调用目标方法，获取返回结果
                retVal = new CglibMethodInvocation(proxy, target, method, args, targetClass, chain, methodProxy).proceed();
            }
            // 处理方法返回结果：会根据方法返回值的类型，做一些处理，比如方法返回的类型为自己，则最后需要将返回值置为代理对象
            retVal = processReturnType(proxy, target, method, retVal);
            return retVal;
        }
        finally {
            // 目标对象不为null && 目标源不是静态的
            //所谓静态的，你可以理解为是否是单例的
            // isStatic为true，表示目标对象是单例的，同一个代理对象中所有方法共享一个目标对象
            // isStatic为false的时候，通常每次调用代理的方法，target对象是不一样的，所以方法调用万之后需要进行释放，可能有些资源清理，连接的关闭等操作
            if (target != null && !targetSource.isStatic()) {
                targetSource.releaseTarget(target);
            }
            // setProxyContext为ture
            if (setProxyContext) {
                // 需要将旧的代理再放回到上线文中
                AopContext.setCurrentProxy(oldProxy);
            }
        }
    }
}
```
上面代码中2个重点：@1和@2<br />@1：获取当前方法的拦截器链，这个在`JdkDynamicAopProxy`的也有，稍后说。<br />@2：调用`CglibMethodInvocation.proceed()`，内部会一次调用方法拦截器链，最终会调用目标方法，获取目标方法返回值，这个稍后放在代理方法处理阶段详解。<br />下面来看一下方法拦截器链的获取。
<a name="xzAg0"></a>
#### 方法拦截器链的获取
**在创建代理的时候，增强的代码通常都放在Advise通知中，但是最终调用方法的时候，这些通知都会被转换为**`**MethodInterceptor**`**来执行，调用方法的过程中，需要先获取方法上匹配的所有方法连接器连，然后依次执行，最终会调用到目标方法。**<br />获取方法对应的拦截器链，对应下面这段代码
```java
// org.springframework.aop.framework.AdvisedSupport#getInterceptorsAndDynamicInterceptionAdvice

AdvisorChainFactory advisorChainFactory = new DefaultAdvisorChainFactory();

public List<Object> getInterceptorsAndDynamicInterceptionAdvice(Method method, @Nullable Class<?> targetClass) {
    MethodCacheKey cacheKey = new MethodCacheKey(method);
    List<Object> cached = this.methodCache.get(cacheKey);
    if (cached == null) {
        cached = this.advisorChainFactory.getInterceptorsAndDynamicInterceptionAdvice(
            this, method, targetClass);
        this.methodCache.put(cacheKey, cached);
    }
    return cached;
}
```
会调用`DefaultAdvisorChainFactory#getInterceptorsAndDynamicInterceptionAdvice`方法获取方法上匹配的拦截器链。<br />涉及到的类<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684331335310-a0365f89-4079-4788-a51c-4d34e33b2152.png#averageHue=%23fbfaf9&clientId=u4c06a739-18d6-4&from=paste&id=u270dbd5a&originHeight=544&originWidth=922&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u077c9335-bb4b-4ba2-8073-37fc3b66faf&title=)
<a name="z8PNk"></a>
##### `AdvisorChainFactory`接口
拦截器链工厂接口，定义了一个方法，用来获取方法匹配的拦截器链列表
```java
package org.springframework.aop.framework;

public interface AdvisorChainFactory {

    /**
     * 获取方法匹配的拦截器链列表
     * @param config：代理配置信息，里面包含了创建代理的所有信息，如：Advisor列表，此方法会从Advisor列表中找到和mehod匹配的
     * @param targetClass：目标类
     */
    List<Object> getInterceptorsAndDynamicInterceptionAdvice(Advised config, Method method, @Nullable Class<?> targetClass);
}
```
<a name="LPSrw"></a>
##### `DefaultAdvisorChainFactory`类
`AdvisorChainFactory`接口的默认实现。
```java
public class DefaultAdvisorChainFactory implements AdvisorChainFactory, Serializable {

    @Override
    public List<Object> getInterceptorsAndDynamicInterceptionAdvice(
        Advised config, Method method, @Nullable Class<?> targetClass) {

        // 获取Advisor适配器注册器，前面有提到过一个知识点：所有的Advisor最终都会转换为MethodInterceptor类型的，
        // 然后注册方法调用链去执行，AdvisorAdapterRegistry就是搞这个事情的,
        // 其内部会将非MethodInterceptor类型通知通过适配器转换为MethodInterceptor类型
        AdvisorAdapterRegistry registry = GlobalAdvisorAdapterRegistry.getInstance();
        //获取配置中的Advisor列表
        Advisor[] advisors = config.getAdvisors();
        List<Object> interceptorList = new ArrayList<>(advisors.length);
        //获取被调用方法所在类实际的类型
        Class<?> actualClass = (targetClass != null ? targetClass : method.getDeclaringClass());
        Boolean hasIntroductions = null;
        //遍历Advisor列表，找到和actualClass和方法匹配的所有方法拦截器（MethodInterceptor）链列表
        for (Advisor advisor : advisors) {
            //判断是否是PointcutAdvisor类型的，这种类型的匹配分为2个阶段，先看类是否匹配，然后再看方法是否匹配
            if (advisor instanceof PointcutAdvisor) {
                PointcutAdvisor pointcutAdvisor = (PointcutAdvisor) advisor;
                // 如果isPreFiltered为ture，表示类以及匹配过，不需要看类是否匹配了
                if (config.isPreFiltered() || pointcutAdvisor.getPointcut().getClassFilter().matches(actualClass)) {
                    MethodMatcher mm = pointcutAdvisor.getPointcut().getMethodMatcher();
                    boolean match;
                    if (mm instanceof IntroductionAwareMethodMatcher) {
                        if (hasIntroductions == null) {
                            hasIntroductions = hasMatchingIntroductions(advisors, actualClass);
                        }
                        match = ((IntroductionAwareMethodMatcher) mm).matches(method, actualClass, hasIntroductions);
                    }
                    else {
                        //方法是否匹配
                        match = mm.matches(method, actualClass);
                    }
                    //方法匹配
                    if (match) {
                        // 通过AdvisorAdapterRegistry的getInterceptors将advisor转换为MethodInterceptor列表
                        MethodInterceptor[] interceptors = registry.getInterceptors(advisor);
                        //方法是否动态匹配
                        if (mm.isRuntime()) {
                            //轮询连接器，将其包装为InterceptorAndDynamicMethodMatcher对象，后续方法调用的时候可以做动态匹配
                            for (MethodInterceptor interceptor : interceptors) {
                                interceptorList.add(new InterceptorAndDynamicMethodMatcher(interceptor, mm));
                            }
                        }
                        else {
                            interceptorList.addAll(Arrays.asList(interceptors));
                        }
                    }
                }
            }
            else if (advisor instanceof IntroductionAdvisor) {
                IntroductionAdvisor ia = (IntroductionAdvisor) advisor;
                                          if (config.isPreFiltered() || ia.getClassFilter().matches(actualClass)) {
                                          Interceptor[] interceptors = registry.getInterceptors(advisor);
                                          interceptorList.addAll(Arrays.asList(interceptors));
                                          }
                                          }
                                          else {
                                          Interceptor[] interceptors = registry.getInterceptors(advisor);
                                          interceptorList.addAll(Arrays.asList(interceptors));
                                          }
                                          }

                                          return interceptorList;
                                          }
                                          }
```
下面来看`AdvisorAdapterRegistry`这个接口。
<a name="s5lfy"></a>
##### `AdvisorAdapterRegistry`接口
`AdvisorAdapter`注册器，`AdvisorAdapter`可以将`Advisor`中的`Advice`适配为`MethodInterceptor`
```java
package org.springframework.aop.framework.adapter;

public interface AdvisorAdapterRegistry {

    //将一个通知（Advice）包装为Advisor对象
    Advisor wrap(Object advice) throws UnknownAdviceTypeException;

    //根据Advisor获取方法MethodInterceptor列表
    MethodInterceptor[] getInterceptors(Advisor advisor) throws UnknownAdviceTypeException;

    //注册AdvisorAdapter，AdvisorAdapter可以将Advisor中的Advice适配为MethodInterceptor
    void registerAdvisorAdapter(AdvisorAdapter adapter);

}
```
<a name="xhQFl"></a>
##### `DefaultAdvisorAdapterRegistry`类
`AdvisorAdapterRegistry`的默认实现，目前里面做的事情主要是将负责将前置通知，异常通知，后置通知转换为`MethodInterceptor`类型的，源码比较简单，大家看一下就懂了。
```java
public class DefaultAdvisorAdapterRegistry implements AdvisorAdapterRegistry, Serializable {

    //AdvisorAdapter转换器列表，AdvisorAdapter负责将Advisor中的Advice转换为MethodInterceptor类型的
    private final List<AdvisorAdapter> adapters = new ArrayList<>(3);

    //默认会注册3个AdvisorAdapter，这3个负责将前置通知，异常通知，后置通知转换为MethodInterceptor类型的
    public DefaultAdvisorAdapterRegistry() {
        registerAdvisorAdapter(new MethodBeforeAdviceAdapter());
        registerAdvisorAdapter(new AfterReturningAdviceAdapter());
        registerAdvisorAdapter(new ThrowsAdviceAdapter());
    }


    @Override
    public Advisor wrap(Object adviceObject) throws UnknownAdviceTypeException {
        if (adviceObject instanceof Advisor) {
            return (Advisor) adviceObject;
        }
        if (!(adviceObject instanceof Advice)) {
            throw new UnknownAdviceTypeException(adviceObject);
        }
        Advice advice = (Advice) adviceObject;
        if (advice instanceof MethodInterceptor) {
            // So well-known it doesn't even need an adapter.
            return new DefaultPointcutAdvisor(advice);
        }
        //轮询adapters
        for (AdvisorAdapter adapter : this.adapters) {
            //adapter是否支持适配advice这个通知
            if (adapter.supportsAdvice(advice)) {
                return new DefaultPointcutAdvisor(advice);
            }
        }
        throw new UnknownAdviceTypeException(advice);
    }

    //将Advisor对象转换为MethodInterceptor列表，不过通常情况下一个advisor会返回一个MethodInterceptor
    @Override
    public MethodInterceptor[] getInterceptors(Advisor advisor) throws UnknownAdviceTypeException {
        List<MethodInterceptor> interceptors = new ArrayList<>(3);
        Advice advice = advisor.getAdvice();
        if (advice instanceof MethodInterceptor) {
            interceptors.add((MethodInterceptor) advice);
        }
        //轮询adapters
        for (AdvisorAdapter adapter : this.adapters) {
            //先看一下adapter是否支持适配advice这个通知
            if (adapter.supportsAdvice(advice)) {
                //如果匹配，这调用适配器的getInterceptor方法将advisor转换为MethodInterceptor
                interceptors.add(adapter.getInterceptor(advisor));
            }
        }
        if (interceptors.isEmpty()) {
            throw new UnknownAdviceTypeException(advisor.getAdvice());
        }
        return interceptors.toArray(new MethodInterceptor[0]);
    }

    @Override
    public void registerAdvisorAdapter(AdvisorAdapter adapter) {
        this.adapters.add(adapter);
    }

}
```
<a name="DVk4H"></a>
##### `AdvisorAdapter`接口
```java
package org.springframework.aop.framework.adapter;

public interface AdvisorAdapter {
    //判断这个适配器支持advice这个通知么
    boolean supportsAdvice(Advice advice);

    //获取advisor对应的MethodInterceptor
    MethodInterceptor getInterceptor(Advisor advisor);
}
```
<a name="GQhBr"></a>
##### `MethodBeforeAdviceAdapter`类
适配`MethodBeforeAdvice`前置通知，负责将`MethodBeforeAdvice`类型的通知转换为`MethodBeforeAdviceInterceptor`类型的
```java
class MethodBeforeAdviceAdapter implements AdvisorAdapter, Serializable {

    @Override
    public boolean supportsAdvice(Advice advice) {
        return (advice instanceof MethodBeforeAdvice);
    }

    @Override
    public MethodInterceptor getInterceptor(Advisor advisor) {
        MethodBeforeAdvice advice = (MethodBeforeAdvice) advisor.getAdvice();
        return new MethodBeforeAdviceInterceptor(advice);
    }

}
```
<a name="NuHTz"></a>
##### `MethodBeforeAdviceInterceptor`类
将`MethodBeforeAdvice`通知适配为`MethodInterceptor`类型的，代码很简单，大家一看就懂。
```java
package org.springframework.aop.framework.adapter;

public class MethodBeforeAdviceInterceptor implements MethodInterceptor, BeforeAdvice, Serializable {

    private final MethodBeforeAdvice advice;

    public MethodBeforeAdviceInterceptor(MethodBeforeAdvice advice) {
        Assert.notNull(advice, "Advice must not be null");
        this.advice = advice;
    }


    @Override
    public Object invoke(MethodInvocation mi) throws Throwable {
        //先调用前置通知
        this.advice.before(mi.getMethod(), mi.getArguments(), mi.getThis());
        //然后继续处理连接器连，内部会调用目标方法
        return mi.proceed();
    }

}
```
<a name="dsBip"></a>
##### `AfterReturningAdviceAdapter`类
适配`AfterReturningAdvice`后置通知，负责将`AfterReturningAdvice`类型的通知转换为`AfterReturningAdviceInterceptor`类型的
```java
class AfterReturningAdviceAdapter implements AdvisorAdapter, Serializable {

    @Override
    public boolean supportsAdvice(Advice advice) {
        return (advice instanceof AfterReturningAdvice);
    }

    @Override
    public MethodInterceptor getInterceptor(Advisor advisor) {
        AfterReturningAdvice advice = (AfterReturningAdvice) advisor.getAdvice();
        return new AfterReturningAdviceInterceptor(advice);
    }

}
```
<a name="xbDNN"></a>
##### `AfterReturningAdviceInterceptor`类
将`AfterReturningAdvice`通知适配为`MethodInterceptor`类型的，代码很简单，大家一看就懂。
```java
public class AfterReturningAdviceInterceptor implements MethodInterceptor, AfterAdvice, Serializable {

    private final AfterReturningAdvice advice;

    public AfterReturningAdviceInterceptor(AfterReturningAdvice advice) {
        Assert.notNull(advice, "Advice must not be null");
        this.advice = advice;
    }


    @Override
    public Object invoke(MethodInvocation mi) throws Throwable {
        //先调用拦截器链，内部会调用目标方法
        Object retVal = mi.proceed();
        //然后执行后置通知
        this.advice.afterReturning(retVal, mi.getMethod(), mi.getArguments(), mi.getThis());
        //返回结果
        return retVal;
    }

}
```
<a name="M3FGE"></a>
##### `ThrowsAdviceAdapter`类
适配`ThrowsAdvice`前置通知，负责将`MethodBeforeAdvice`类型的通知转换为`MethodBeforeAdviceInterceptor`类型的
```java
class ThrowsAdviceAdapter implements AdvisorAdapter, Serializable {

    @Override
    public boolean supportsAdvice(Advice advice) {
        return (advice instanceof ThrowsAdvice);
    }

    @Override
    public MethodInterceptor getInterceptor(Advisor advisor) {
        return new ThrowsAdviceInterceptor(advisor.getAdvice());
    }

}
```
<a name="aVf5r"></a>
##### `ThrowsAdviceInterceptor`类
将`ThrowsAdvice`通知适配为`MethodInterceptor`类型的，代码很简单，大家一看就懂。
```java
package org.springframework.aop.framework.adapter;

public class ThrowsAdviceInterceptor implements MethodInterceptor, AfterAdvice {

    private static final String AFTER_THROWING = "afterThrowing";

    private final Object throwsAdvice;

    //创建ThrowsAdviceInterceptor
    public ThrowsAdviceInterceptor(Object throwsAdvice) {
        Assert.notNull(throwsAdvice, "Advice must not be null");
        this.throwsAdvice = throwsAdvice;
        //获取异常通知中定义的所有方法（public、默认的、protected、private）
        Method[] methods = throwsAdvice.getClass().getMethods();
        //轮询methods
        for (Method method : methods) {
            //方法名称为afterThrowing && 方法参数为1或者4
            if (method.getName().equals(AFTER_THROWING) &&
                (method.getParameterCount() == 1 || method.getParameterCount() == 4)) {
                //获取方法的最后一个参数类型
                Class<?> throwableParam = method.getParameterTypes()[method.getParameterCount() - 1];
                //判断方法参数类型是不是Throwable类型的
                if (Throwable.class.isAssignableFrom(throwableParam)) {
                    // 缓存异常处理方法到map中（异常类型->异常处理方法）
                    this.exceptionHandlerMap.put(throwableParam, method);
                }
            }
        }
        //如果exceptionHandlerMap，抛出异常，所以最少要有一个异常处理方法
        if (this.exceptionHandlerMap.isEmpty()) {
            throw new IllegalArgumentException(
                "At least one handler method must be found in class [" + throwsAdvice.getClass() + "]");
        }
    }


    /**
     * 获取异常通知中自定义的处理异常方法的数量
     */
    public int getHandlerMethodCount() {
        return this.exceptionHandlerMap.size();
    }


    @Override
    public Object invoke(MethodInvocation mi) throws Throwable {
        try {
            //调用通知链
            return mi.proceed();
        }
        catch (Throwable ex) {
            //获取异常通知中自定义的处理异常的方法
            Method handlerMethod = getExceptionHandler(ex);
            //当处理的方法不为空
            if (handlerMethod != null) {
                //调用异常处理方法
                invokeHandlerMethod(mi, ex, handlerMethod);
            }
            //继续向外抛出异常
            throw ex; //@1
        }
    }

    /**
     * 获取throwsAdvice中处理exception参数指定的异常的方法
     */
    @Nullable
    private Method getExceptionHandler(Throwable exception) {
        //获取异常类型
        Class<?> exceptionClass = exception.getClass();
        //从缓存中获取异常类型对应的方法
        Method handler = this.exceptionHandlerMap.get(exceptionClass);
        //来一个循环，查询处理方法，循环条件：方法为空 && 异常类型!=Throwable
        while (handler == null && exceptionClass != Throwable.class) {
            //获取异常的父类型
            exceptionClass = exceptionClass.getSuperclass();
            //从缓存中查找异常对应的处理方法
            handler = this.exceptionHandlerMap.get(exceptionClass);
        }
        //将查找结果返回
        return handler;
    }

    //通过反射调用异常通知中的异常方法
    private void invokeHandlerMethod(MethodInvocation mi, Throwable ex, Method method) throws Throwable {
        //构建方法请求参数
        Object[] handlerArgs;
        //若只有1个参数，参数为：异常对象
        if (method.getParameterCount() == 1) {
        handlerArgs = new Object[] {ex};
    }
        else {
        //4个参数（方法、方法请求参数、目标对象、异常对象）
        handlerArgs = new Object[] {mi.getMethod(), mi.getArguments(), mi.getThis(), ex};
    }
        try {
        //通过反射调用异常通知中的方法
        method.invoke(this.throwsAdvice, handlerArgs);
    }
        catch (InvocationTargetException targetEx) {
        throw targetEx.getTargetException();
    }
    }
    }
```
<a name="DqmhE"></a>
## 代理方法的调用过程（拦截器链的执行）
<a name="OhoJg"></a>
### 拦截器链执行过程
到目前，已经获取到代理对象，接着会开始使用这个代理对象，在代理对象上执行一些方法调用，此时会依次调用此方法上的所有`MethodInterceptor`，最终会调用到目标上对应的方法，执行过程如下图<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684331335244-cb873035-bc36-4b7f-a5b5-0794ea8ddb3e.png#averageHue=%23eeeeee&clientId=u4c06a739-18d6-4&from=paste&id=u3940c60f&originHeight=541&originWidth=619&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uad6afb7c-7e4a-4669-8a33-6596dec17b1&title=)<br />jdk动态代理方式创建代理最终会调用`ReflectiveMethodInvocation#proceed`方法。<br />cglib方式创建的代理最终会调用`CglibAopProxy.CglibMethodInvocation#proceed`方法。<br />下面来看一下这个两个类的代码。
<a name="nviwO"></a>
### `ReflectiveMethodInvocation`类
```java
public class ReflectiveMethodInvocation implements ProxyMethodInvocation, Cloneable {

    //生成的代理对象
    protected final Object proxy;
    //被代理的目标对象
    protected final Object target;
    //被调用的方法
    protected final Method method;
    //调用方法传入参数
    protected Object[] arguments;
    //目标对象类型
    private final Class<?> targetClass;

    /**
     * 当期被调用的方法上匹配的 MethodInterceptor and InterceptorAndDynamicMethodMatcher 列表
     * 即方法调用链列表
     */
    protected final List<?> interceptorsAndDynamicMethodMatchers;

    //当前正在调用的连接器索引
    private int currentInterceptorIndex = -1;

    //构造器
    protected ReflectiveMethodInvocation(
        Object proxy, @Nullable Object target, Method method, @Nullable Object[] arguments,
        @Nullable Class<?> targetClass, List<Object> interceptorsAndDynamicMethodMatchers) {
        this.proxy = proxy;
        this.target = target;
        this.targetClass = targetClass;
        //获取桥接方法，关于什么是桥接方法，比较简单，百度一下，这里不做说明
        this.method = BridgeMethodResolver.findBridgedMethod(method);
        this.arguments = AopProxyUtils.adaptArgumentsIfNecessary(method, arguments);
        this.interceptorsAndDynamicMethodMatchers = interceptorsAndDynamicMethodMatchers;
    }

    //这里是重点，用来处理被调用的方法，会递归进行调用，所有的拦截器都执行完毕之后，会通过反射调用目标方法
    public Object proceed() throws Throwable {
        // 拦截器都执行完毕之后，通过反射调用目标对象中的方法
        if (this.currentInterceptorIndex == this.interceptorsAndDynamicMethodMatchers.size() - 1) {
            return invokeJoinpoint();
        }

        //获取++this.currentInterceptorIndex指定的拦截器
        Object interceptorOrInterceptionAdvice =
        this.interceptorsAndDynamicMethodMatchers.get(++this.currentInterceptorIndex);
        //判断拦截器是否是InterceptorAndDynamicMethodMatcher，这种表示是动态拦截器，
        // 所谓动态拦截器就是要根据方法的参数的值来判断拦截器是否需要执行
        if (interceptorOrInterceptionAdvice instanceof InterceptorAndDynamicMethodMatcher) {
            InterceptorAndDynamicMethodMatcher dm =
            (InterceptorAndDynamicMethodMatcher) interceptorOrInterceptionAdvice;
            Class<?> targetClass = (this.targetClass != null ? this.targetClass : this.method.getDeclaringClass());
            //判断动态拦截器是否需要执行
            if (dm.methodMatcher.matches(this.method, targetClass, this.arguments)) {
                //执行当前拦截器的调用
                return dm.interceptor.invoke(this);
            }
            else {
                //如果不匹配，直接递归进入下一个拦截器的调用
                return proceed();
            }
        }
        else {
            //执行拦截器的调用
            return ((MethodInterceptor) interceptorOrInterceptionAdvice).invoke(this);
        }
    }

    //通过反射调用目标方法
    @Nullable
    protected Object invokeJoinpoint() throws Throwable {
        return AopUtils.invokeJoinpointUsingReflection(this.target, this.method, this.arguments);
    }
}
```
<a name="nojQ3"></a>
## `ProxyFactory`简化代理的创建
上面代理的整个创建过程和使用过程还是挺负载的，Spring在`AdvisedSupport`类的基础上又添加2个子类

- `ProxyCreatorSupport`
- `ProxyFactory`

通过这2个子类，将步骤稍微简化了一些，这2个类的代码比较简单，上面的如果理解了，看这2个类的代码会非常的轻松，这里就不细说了。<br />`ProxyCreatorSupport`用来对代理的创建提供支持，内部添加了`AopProxyFactory`对象的引用，将代理的创建过程给简化了。
```java
AopProxyFactory aopProxyFactory;
```
`ProxyFactory`类继承了`ProxyCreatorSupport`，让创建代理的过程更简单了，如果采用硬编码的方式，通常会使用`ProxyFactory`来创建代理对象，代码只需要下面几行了
```java
//通过spring提供的代理创建工厂来创建代理
ProxyFactory proxyFactory = new ProxyFactory();
//ProxyFactory继承了AdvisedSupport类，所以可以直接可以通过ProxyFactory来设置创建代理需要的参数
//为工厂指定目标对象
proxyFactory.setTarget(target);
//添加顾问
proxyFactory.addAdvisor(advisor);
//调用proxyFactory.getProxy();创建代理
Object proxy = proxyFactory.getProxy();
```
<a name="eDTHk"></a>
## 案例
下面来一些案例，通过案例理解会更容易一些。
<a name="Oyat0"></a>
### 案例1
这个案例主要看一下生成的代理对象的一些信息。
```java
import com.javacode2018.aop.demo4.FundsService;
import org.springframework.aop.MethodBeforeAdvice;
import org.springframework.aop.framework.ProxyFactory;
import org.springframework.aop.support.DefaultPointcutAdvisor;
import org.springframework.lang.Nullable;

import java.lang.reflect.Method;

public class AopTest5 {
    public static void main(String[] args) {
        ProxyFactory proxyFactory = new ProxyFactory();
        proxyFactory.setTarget(new FundsService());
        proxyFactory.addAdvisor(new DefaultPointcutAdvisor(new MethodBeforeAdvice() {
            @Override
            public void before(Method method, Object[] args, @Nullable Object target) throws Throwable {
                System.out.println(method);
            }
        }));
        //创建代理对象
        Object proxy = proxyFactory.getProxy();
        System.out.println("代理对象的类型：" + proxy.getClass());
        System.out.println("代理对象的父类：" + proxy.getClass().getSuperclass());
        System.out.println("代理对象实现的接口列表");
        for (Class<?> cf : proxy.getClass().getInterfaces()) {
            System.out.println(cf);
        }
    }
}
```
运行输出
```
代理对象的类型：class com.javacode2018.aop.demo4.FundsService$$EnhancerBySpringCGLIB$$ad14161a
代理对象的父类：class com.javacode2018.aop.demo4.FundsService
代理对象实现的接口列表
interface org.springframework.aop.SpringProxy
interface org.springframework.aop.framework.Advised
interface org.springframework.cglib.proxy.Factory
```
输出中可以看出默认实现了3个接口[SpringProxy,Advised,Factory]
<a name="MCxSX"></a>
### 案例2：代理接口
有接口的情况默认会通过jdk动态代理的方式生成代理，下面来看一下。<br />来个接口
```java
public interface IService {
    void say(String name);
}
```
实现类
```java
public class Service implements IService {
    @Override
    public void say(String name) {
        System.out.println("hello：" + name);
    }
}
```
测试案例
```java
import org.junit.Test;
import org.springframework.aop.MethodBeforeAdvice;
import org.springframework.aop.framework.ProxyFactory;
import org.springframework.lang.Nullable;

import java.lang.reflect.Method;

public class AopTest6 {
    @Test
    public void test1() {
        Service target = new Service();

        ProxyFactory proxyFactory = new ProxyFactory();
        //设置需要被代理的对象
        proxyFactory.setTarget(target);
        //设置需要代理的接口
        proxyFactory.addInterface(IService.class);

        proxyFactory.addAdvice(new MethodBeforeAdvice() {
            @Override
            public void before(Method method, Object[] args, @Nullable Object target) throws Throwable {
                System.out.println(method);
            }
        });

        IService proxy = (IService) proxyFactory.getProxy();
        System.out.println("代理对象的类型：" + proxy.getClass());
        System.out.println("代理对象的父类：" + proxy.getClass().getSuperclass());
        System.out.println("代理对象实现的接口列表");
        for (Class<?> cf : proxy.getClass().getInterfaces()) {
            System.out.println(cf);
        }
        //调用代理的方法
        System.out.println("\n调用代理的方法");
        proxy.say("spring aop");
    }
}
```
运行输出
```
代理对象的类型：class com.sun.proxy.$Proxy4
代理对象的父类：class java.lang.reflect.Proxy
代理对象实现的接口列表
interface com.javacode2018.aop.demo6.IService
interface org.springframework.aop.SpringProxy
interface org.springframework.aop.framework.Advised
interface org.springframework.core.DecoratingProxy

调用代理的方法
public abstract void com.javacode2018.aop.demo6.IService.say(java.lang.String)
hello：spring aop
```
从第一行输出中可以看出是采用jdk动态代理方式创建的代理<br />第二行验证了，所有通过jdk动态代理方式创建的代理对象都是Proxy的子类<br />输出的接口列表中可以看出，默认实现了3个接口[`SpringAop`, `Advised`, `DecoratingProxy`]
<a name="IjzEa"></a>
### 案例3：强制使用cglib代理
在案例2中加入下面代码，设置`proxyTargetClass`为true，会强制使用cglib代理。
```java
//强制使用cglib代理
proxyFactory.setProxyTargetClass(true);
```
<a name="vLn1w"></a>
### 案例4：将代理暴露在threadLocal中
先来看一段代码，Service类中有2个方法，m1方法中会调用m2，通过aop代理对这个类创建了一个代理，通过代理来统计所有调用方法的耗时
```java
class Service {
    public void m1() {
        System.out.println("m1");
        this.m2();
    }

    public void m2() {
        System.out.println("m2");
    }
}

public class AopTest7 {
    @Test
    public void test1() {
        Service target = new Service();

        ProxyFactory proxyFactory = new ProxyFactory();
        proxyFactory.setTarget(target);

        proxyFactory.addAdvice(new MethodInterceptor() {
            @Override
            public Object invoke(MethodInvocation invocation) throws Throwable {
                long startTime = System.nanoTime();
                Object result = invocation.proceed();
                long endTime = System.nanoTime();
                System.out.println(String.format("%s方法耗时(纳秒):%s", invocation.getMethod().getName(), endTime - startTime));
                return result;
            }
        });

        Service proxy = (Service) proxyFactory.getProxy();
        proxy.m1();
    }
}
```
运行输出
```
m1
m2
m1方法耗时(纳秒):11056000
```
**为什么没有输出m2方法的耗时?**<br />原因：m2方法是在m1方法中通过this的方式来调用的，this实际上指向的是上面代码中的target对象。<br />那么如何能让此处的m2也能被增强，需要通过代理来调用m2方法才可以，可以将代理对象暴露在threadLocal中，然后在m1方法中获取到threadLoca中的代理对象，通过代理对象来调用m2就可以了。<br />需要调整改动2处。<br />第1处：配置代理创建时，将其暴露出去
```java
proxyFactory.setExposeProxy(true);
```
第2处：m1中调用m2的方法需要修改为下面这样
```java
((Service)AopContext.currentProxy()).m2();
```
再次执行，结果如下
```
m1
m2
m2方法耗时(纳秒):51200
m1方法耗时(纳秒):12794500
```
这个功能还是挺有用的，估计以后大家是可以用到的。
