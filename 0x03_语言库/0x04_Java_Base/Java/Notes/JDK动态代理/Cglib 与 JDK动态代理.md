Java 动态代理 Cglib JDK
<a name="g0P2O"></a>
## AOP 代理的两种实现

- jdk是代理接口，私有方法必然不会存在在接口里，所以就不会被拦截到；
- cglib是子类，`private`的方法照样不会出现在子类里，也不能被拦截。
<a name="ZtMOj"></a>
### JDK 动态代理
具体有如下四步骤：

1. 通过实现 `InvocationHandler` 接口创建自己的调用处理器；
2. 通过为 `Proxy` 类指定 `ClassLoader` 对象和一组 interface 来创建动态代理类；
3. 通过反射机制获得动态代理类的构造函数，其唯一参数类型是调用处理器接口类型；
4. 通过构造函数创建动态代理类实例，构造时调用处理器对象作为参数被传入。
<a name="kPLY8"></a>
#### 示例
创建业务接口
```java
public interface UserService {
    /**
     * 需要增强的方法
     */
    void add();
}
```
创建业务接口 实现类
```java
public class UserServiceImpl implements UserService {
    @Override
    public void add() {
        System.out.println("-------------业务逻辑方法 add ------------");
    }
}
```
创建自定义的 `InvocationHandler`，用于对接口提供的方法进行增强
```java
public class MyInvocationHandler implements InvocationHandler {
    /**
     * 需要增强的对象
     */
    private Object target;
    public MyInvocationHandler(Object target) {
        this.target = target;
    }
    /**
     * 执行目标对象
     */
    @Override
    public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
        System.out.println("-------------方法执行前的增强逻辑 ------------");
        Object result = method.invoke(target, args);
        System.out.println("-------------方法执行后的增强逻辑 ------------");
        return result;
    }
    public Object getProxy() {
        return Proxy.newProxyInstance(Thread.currentThread().getContextClassLoader(), target.getClass().getInterfaces(), this);
    }
}
```
测试类
```java
public class ProxyTest {
    @Test
    public void contextTest() {
        // 实例化目标对象
        UserService userService = new UserServiceImpl();
        // 实例化 InvocationHandler
        MyInvocationHandler invocationHandler = new MyInvocationHandler(userService);
        // 生成代理对象
        UserService proxy = (UserService) invocationHandler.getProxy();
        // 输出代理类的class
        ProxySourceClassUtil.writeClassToDisk("C:\\Users\\Fcant\\Desktop\\" + proxy.getClass().getSimpleName() + ".class", proxy.getClass().getSimpleName(), UserService.class);
        // 调用代理对象的方法
        proxy.add();
    }
}
```
输出结果：
```
Connected to the target VM, address: '127.0.0.1:60738', transport: 'socket'
-------------方法执行前的增强逻辑 ------------
-------------业务逻辑方法 add ------------
-------------方法执行后的增强逻辑 ------------
Disconnected from the target VM, address: '127.0.0.1:60738', transport: 'socket'
```
用起来很简单，其实，这基本上就 AOP 的一个简单实现了，在目标对象的方法执行之前和 执行之后进行了增强。Spring的AOP实现其实也是用了 Proxy 和 `InvocationHandler`。在整个创建过程中`InvocationHandler`的创建最为核心，在自定义的`InvocationHandler`中需要重新3个函数：

1. 构造函数，将代理的对象传入。
2. `invoke`方法，此方法中实现了AOP增强的所有逻辑。
3. `getProxy`方法，此方千篇一律，但是必不可少。
<a name="7qkDZ"></a>
#### 反编译代理类结果
```java
package com.fcant.aop.jdk;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;
import java.lang.reflect.UndeclaredThrowableException;
// 代理类会去实现我们的接口
public final class Proxy4 extends Proxy implements UserService {
    // 解析所有的方法
    private static Method m1;
    private static Method m2;
    private static Method m3;
    private static Method m0;
    public Proxy4() throws {
        super(paramInvocationHandler);
    }
    public final boolean equals() throws {
        try {
            return ((Boolean) this.h.invoke(this, m1, new Object[]{paramObject})).booleanValue();
        } catch (RuntimeException localRuntimeException) {
            throw localRuntimeException;
        } catch (Throwable localThrowable) {
            throw new UndeclaredThrowableException(localThrowable);
        }
    }
    public final String toString() throws {
        try {
            return ((String) this.h.invoke(this, m2, null));
        } catch (RuntimeException localRuntimeException) {
            throw localRuntimeException;
        } catch (Throwable localThrowable) {
            throw new UndeclaredThrowableException(localThrowable);
        }
    }
    public final void add() throws {
        try {
            // 通过InvocationHandler的invoke方法去执行增强和原有方法逻辑
            this.h.invoke(this, m3, null);
            return;
        } catch (RuntimeException localRuntimeException) {
            throw localRuntimeException;
        } catch (Throwable localThrowable) {
            throw new UndeclaredThrowableException(localThrowable);
        }
    }
    public final int hashCode() throws {
        try {
            return ((Integer) this.h.invoke(this, m0, null)).intValue();
        } catch (RuntimeException localRuntimeException) {
            throw localRuntimeException;
        } catch (Throwable localThrowable) {
            throw new UndeclaredThrowableException(localThrowable);
        }
    }
    static {
        try {
            // 解析所有的method出来
            m1 = Class.forName("java.lang.Object").getMethod("equals", new Class[]{Class.forName("java.lang.Object")});
            m2 = Class.forName("java.lang.Object").getMethod("toString", new Class[0]);
            m3 = Class.forName("com.fcant.aop.jdk.UserService").getMethod("add", new Class[0]);
            m0 = Class.forName("java.lang.Object").getMethod("hashCode", new Class[0]);
            return;
        } catch (NoSuchMethodException localNoSuchMethodException) {
            throw new NoSuchMethodError(localNoSuchMethodException.getMessage());
        } catch (ClassNotFoundException localClassNotFoundException) {
            throw new NoClassDefFoundError(localClassNotFoundException.getMessage());
        }
    }
}
```
从结果可以看出，代理类会实现接口，并会实现接口中的方法。然后通过`InvocationHandler`的`invoke`方法去执行增强逻辑和原有方法逻辑。在`invoke`中可以使用反射去执行原有逻辑，并在方法执行前后加上自己的增强逻辑。
<a name="ctxkC"></a>
#### `JdkDynamicAopProxy`
`JdkDynamicAopProxy`是Spring中JDK动态代理的实现，它也实现了`InvocationHandler`接口。核心方法：

1. `getProxy`：获取代理对象，这个和上面的示例几乎是一样的
2. `invoke`：实现了AOP的核心逻辑

源码：
```java
final class JdkDynamicAopProxy implements AopProxy, InvocationHandler, Serializable {
    ...
    /** Config used to configure this proxy(对增强器的支持) */
    private final AdvisedSupport advised;
    ...
    public JdkDynamicAopProxy(AdvisedSupport config) throws AopConfigException {
        Assert.notNull(config, "AdvisedSupport must not be null");
        if (config.getAdvisors().length == 0 && config.getTargetSource() == AdvisedSupport.EMPTY_TARGET_SOURCE) {
            throw new AopConfigException("No advisors and no TargetSource specified");
        }
        this.advised = config;
    }
    @Override
    public Object getProxy() {
        return getProxy(ClassUtils.getDefaultClassLoader());
    }
    @Override
    public Object getProxy(ClassLoader classLoader) {
        if (logger.isDebugEnabled()) {
            logger.debug("Creating JDK dynamic proxy: target source is " + this.advised.getTargetSource());
        }
        // 获取目标类接口
        Class<?>[] proxiedInterfaces = AopProxyUtils.completeProxiedInterfaces(this.advised, true);
        findDefinedEqualsAndHashCodeMethods(proxiedInterfaces);
        // 返回代理对象
        return Proxy.newProxyInstance(classLoader, proxiedInterfaces, this);
    }
    ...
    /**
      * Implementation of {@code InvocationHandler.invoke}.
      * <p>Callers will see exactly the exception thrown by the target,
      * unless a hook method throws an exception.
      */
        @Override
        public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
        MethodInvocation invocation;
        Object oldProxy = null;
        boolean setProxyContext = false;
        TargetSource targetSource = this.advised.targetSource;
        Class<?> targetClass = null;
        Object target = null;
        try {
            // equals 方法处理
            if (!this.equalsDefined && AopUtils.isEqualsMethod(method)) {
                // The target does not implement the equals(Object) method itself.
                return equals(args[0]);
            }
            // hashCode 方法处理
            else if (!this.hashCodeDefined && AopUtils.isHashCodeMethod(method)) {
                // The target does not implement the hashCode() method itself.
                return hashCode();
            }
            else if (method.getDeclaringClass() == DecoratingProxy.class) {
                // There is only getDecoratedClass() declared -> dispatch to proxy config.
                return AopProxyUtils.ultimateTargetClass(this.advised);
            }
            else if (!this.advised.opaque && method.getDeclaringClass().isInterface() &&
                     method.getDeclaringClass().isAssignableFrom(Advised.class)) {
                // Service invocations on ProxyConfig with the proxy config...
                return AopUtils.invokeJoinpointUsingReflection(this.advised, method, args);
            }
            Object retVal;

            // 有时候目标对象内部的自我调用无法实施切面中的增强则需要通过此属性暴露代理
            if (this.advised.exposeProxy) {
                // Make invocation available if necessary.
                oldProxy = AopContext.setCurrentProxy(proxy);
                setProxyContext = true;
            }
            // May be null. Get as late as possible to minimize the time we "own" the target,
            // in case it comes from a pool.
            target = targetSource.getTarget();
            if (target != null) {
                targetClass = target.getClass();
            }
            // 获取当前方法的拦截链
            List<Object> chain = this.advised.getInterceptorsAndDynamicInterceptionAdvice(method, targetClass);
            if (chain.isEmpty()) {
                // 如果没有拦截链那么直接调用切点方法
                Object[] argsToUse = AopProxyUtils.adaptArgumentsIfNecessary(method, args);
                retVal = AopUtils.invokeJoinpointUsingReflection(target, method, argsToUse);
            }
            else {
                // 将拦截器封装在ReflectiveMethodInvocation中，以便使用proceed方法执行拦截链
                invocation = new ReflectiveMethodInvocation(proxy, target, method, args, targetClass, chain);
                // 执行拦截链
                retVal = invocation.proceed();
            }
            // Massage return value if necessary.
            Class<?> returnType = method.getReturnType();
            if (retVal != null && retVal == target &&
                returnType != Object.class && returnType.isInstance(proxy) &&
                !RawTargetAccess.class.isAssignableFrom(method.getDeclaringClass())) {
                // Special case: it returned "this" and the return type of the method
                // is type-compatible. Note that we can't help if the target sets
                // a reference to itself in another returned object.
                retVal = proxy;
            }
            else if (retVal == null && returnType != Void.TYPE && returnType.isPrimitive()) {
                throw new AopInvocationException(
                    "Null return value from advice does not match primitive return type for: " + method);
            }
            return retVal;
        }
        finally {
            if (target != null && !targetSource.isStatic()) {
                // Must have come from TargetSource.
                targetSource.releaseTarget(target);
            }
            if (setProxyContext) {
                // Restore old proxy.
                AopContext.setCurrentProxy(oldProxy);
            }
        }
    }
    ...
}
```
<a name="M6XO6"></a>
### GCLIB代理
cglib（Code Generation Library）是一个强大的,高性能,高质量的Code生成类库。它可以在运行期扩展Java类与实现Java接口。

- cglib封装了asm，可以在运行期动态生成新的class（子类）。
- cglib用于AOP，jdk中的proxy必须基于接口，cglib却没有这个限制。
<a name="4A88w"></a>
#### 示例
需要代理的类
```java
public class EnhancerDemo {
    public void test() {
        System.out.println("-------------业务逻辑方法 test ------------");
    }
}
```
`MethodInterceptor`的实现类（增强器）
```java
import org.springframework.cglib.proxy.MethodInterceptor;
import org.springframework.cglib.proxy.MethodProxy;
import java.lang.reflect.Method;
public class MethodInterceptorImpl implements MethodInterceptor {
    @Override
    public Object intercept(Object object, Method method, Object[] args, MethodProxy methodProxy) throws Throwable {
        System.out.println("-------------方法执行前的增强逻辑 ------------" + method);
        Object result = methodProxy.invokeSuper(object, args);
        System.out.println("-------------方法执行后的增强逻辑 ------------" + method);
        return result;
    }
}
```
测试类：
```java
public class EnhancerTest {
    @Test
    public void contextTest() {
        System.setProperty(DebuggingClassWriter.DEBUG_LOCATION_PROPERTY, "D:\\class");
        Enhancer enhancer = new Enhancer();
        enhancer.setSuperclass(EnhancerDemo.class);
        enhancer.setCallback(new MethodInterceptorImpl());
        EnhancerDemo proxy = (EnhancerDemo) enhancer.create();
        proxy.test();
        System.out.println(proxy);
    }
}
```
执行结果
```
Connected to the target VM, address: '127.0.0.1:61217', transport: 'socket'
-------------方法执行前的增强逻辑 ------------public void com.fcant.aop.cglib.EnhancerDemo.test()
-------------业务逻辑方法 test ------------
-------------方法执行后的增强逻辑 ------------public void com.fcant.aop.cglib.EnhancerDemo.test()
-------------方法执行前的增强逻辑 ------------public java.lang.String java.lang.Object.toString()
-------------方法执行前的增强逻辑 ------------public native int java.lang.Object.hashCode()
-------------方法执行后的增强逻辑 ------------public native int java.lang.Object.hashCode()
-------------方法执行后的增强逻辑 ------------public java.lang.String java.lang.Object.toString()
com.fcant.aop.cglib.EnhancerDemo$$EnhancerByCGLIB$$da6c48f9@7a187f14
Disconnected from the target VM, address: '127.0.0.1:61217', transport: 'socket'
```
可以看到 `System.out.println(porxy)`; 首先调用了 `toString()` 方法，然后又调用了 `hashCode()` ，生成的对象为 `EnhancerDemo$$EnhancerByCGLIB$$da6c48f9@7a187f14` 的实例，这个类是运行时由 CGLIB 产生。
<a name="qSdSf"></a>
#### 反编译代理结果
在测试类的第一行加上下面语句，那么在cglib中生成的class文件将会持久化到硬盘。
```java
// 该设置用于输出cglib动态代理产生的类
System.setProperty(DebuggingClassWriter.DEBUG_LOCATION_PROPERTY, "D:\\class");  
// 该设置用于输出jdk动态代理产生的类
System.getProperties().put("sun.misc.ProxyGenerator.saveGeneratedFiles", "true");
```
反编译结果：
```java
//
// Source code recreated from a .class file by IntelliJ IDEA
// (powered by Fernflower decompiler)
//
package com.fcant.aop.cglib;
import java.lang.reflect.Method;
import org.springframework.cglib.core.ReflectUtils;
import org.springframework.cglib.core.Signature;
import org.springframework.cglib.proxy.Callback;
import org.springframework.cglib.proxy.Factory;
import org.springframework.cglib.proxy.MethodInterceptor;
import org.springframework.cglib.proxy.MethodProxy;
public class EnhancerDemo$$EnhancerByCGLIB$$da6c48f9 extends EnhancerDemo implements Factory {
    private boolean CGLIB$BOUND;
    public static Object CGLIB$FACTORY_DATA;
    private static final ThreadLocal CGLIB$THREAD_CALLBACKS;
    private static final Callback[] CGLIB$STATIC_CALLBACKS;
    private MethodInterceptor CGLIB$CALLBACK_0;
    private static Object CGLIB$CALLBACK_FILTER;
    private static final Method CGLIB$test$0$Method;
    private static final MethodProxy CGLIB$test$0$Proxy;
    private static final Object[] CGLIB$emptyArgs;
    private static final Method CGLIB$equals$1$Method;
    private static final MethodProxy CGLIB$equals$1$Proxy;
    private static final Method CGLIB$toString$2$Method;
    private static final MethodProxy CGLIB$toString$2$Proxy;
    private static final Method CGLIB$hashCode$3$Method;
    private static final MethodProxy CGLIB$hashCode$3$Proxy;
    private static final Method CGLIB$clone$4$Method;
    private static final MethodProxy CGLIB$clone$4$Proxy;
    static void CGLIB$STATICHOOK1() {
        CGLIB$THREAD_CALLBACKS = new ThreadLocal();
        CGLIB$emptyArgs = new Object[0];
        Class var0 = Class.forName("com.fcant.aop.cglib.EnhancerDemo$$EnhancerByCGLIB$$da6c48f9");
        Class var1;
        Method[] var10000 = ReflectUtils.findMethods(new String[]{"equals", "(Ljava/lang/Object;)Z", "toString", "()Ljava/lang/String;", "hashCode", "()I", "clone", "()Ljava/lang/Object;"}, (var1 = Class.forName("java.lang.Object")).getDeclaredMethods());
        CGLIB$equals$1$Method = var10000[0];
        CGLIB$equals$1$Proxy = MethodProxy.create(var1, var0, "(Ljava/lang/Object;)Z", "equals", "CGLIB$equals$1");
        CGLIB$toString$2$Method = var10000[1];
        CGLIB$toString$2$Proxy = MethodProxy.create(var1, var0, "()Ljava/lang/String;", "toString", "CGLIB$toString$2");
        CGLIB$hashCode$3$Method = var10000[2];
        CGLIB$hashCode$3$Proxy = MethodProxy.create(var1, var0, "()I", "hashCode", "CGLIB$hashCode$3");
        CGLIB$clone$4$Method = var10000[3];
        CGLIB$clone$4$Proxy = MethodProxy.create(var1, var0, "()Ljava/lang/Object;", "clone", "CGLIB$clone$4");
        CGLIB$test$0$Method = ReflectUtils.findMethods(new String[]{"test", "()Ljava/lang/String;"}, (var1 = Class.forName("com.fcant.aop.cglib.EnhancerDemo")).getDeclaredMethods())[0];
        CGLIB$test$0$Proxy = MethodProxy.create(var1, var0, "()Ljava/lang/String;", "test", "CGLIB$test$0");
    }
    final String CGLIB$test$0() {
        return super.test();
    }
    public final String test() {
        MethodInterceptor var10000 = this.CGLIB$CALLBACK_0;
        if (var10000 == null) {
            CGLIB$BIND_CALLBACKS(this);
            var10000 = this.CGLIB$CALLBACK_0;
        }
        return var10000 != null ? (String)var10000.intercept(this, CGLIB$test$0$Method, CGLIB$emptyArgs, CGLIB$test$0$Proxy) : super.test();
    }
    final boolean CGLIB$equals$1(Object var1) {
        return super.equals(var1);
    }
    public final boolean equals(Object var1) {
        MethodInterceptor var10000 = this.CGLIB$CALLBACK_0;
        if (var10000 == null) {
            CGLIB$BIND_CALLBACKS(this);
            var10000 = this.CGLIB$CALLBACK_0;
        }
        if (var10000 != null) {
            Object var2 = var10000.intercept(this, CGLIB$equals$1$Method, new Object[]{var1}, CGLIB$equals$1$Proxy);
            return var2 == null ? false : (Boolean)var2;
        } else {
            return super.equals(var1);
        }
    }
    final String CGLIB$toString$2() {
        return super.toString();
    }
    public final String toString() {
        MethodInterceptor var10000 = this.CGLIB$CALLBACK_0;
        if (var10000 == null) {
            CGLIB$BIND_CALLBACKS(this);
            var10000 = this.CGLIB$CALLBACK_0;
        }
        return var10000 != null ? (String)var10000.intercept(this, CGLIB$toString$2$Method, CGLIB$emptyArgs, CGLIB$toString$2$Proxy) : super.toString();
    }
    final int CGLIB$hashCode$3() {
        return super.hashCode();
    }
    public final int hashCode() {
        MethodInterceptor var10000 = this.CGLIB$CALLBACK_0;
        if (var10000 == null) {
            CGLIB$BIND_CALLBACKS(this);
            var10000 = this.CGLIB$CALLBACK_0;
        }
        if (var10000 != null) {
            Object var1 = var10000.intercept(this, CGLIB$hashCode$3$Method, CGLIB$emptyArgs, CGLIB$hashCode$3$Proxy);
            return var1 == null ? 0 : ((Number)var1).intValue();
        } else {
            return super.hashCode();
        }
    }
    final Object CGLIB$clone$4() throws CloneNotSupportedException {
        return super.clone();
    }
    protected final Object clone() throws CloneNotSupportedException {
        MethodInterceptor var10000 = this.CGLIB$CALLBACK_0;
        if (var10000 == null) {
            CGLIB$BIND_CALLBACKS(this);
            var10000 = this.CGLIB$CALLBACK_0;
        }
        return var10000 != null ? var10000.intercept(this, CGLIB$clone$4$Method, CGLIB$emptyArgs, CGLIB$clone$4$Proxy) : super.clone();
    }
    public static MethodProxy CGLIB$findMethodProxy(Signature var0) {
        String var10000 = var0.toString();
        switch(var10000.hashCode()) {
        case -508378822:
            if (var10000.equals("clone()Ljava/lang/Object;")) {
                return CGLIB$clone$4$Proxy;
            }
            break;
        case 225925469:
            if (var10000.equals("test()Ljava/lang/String;")) {
                return CGLIB$test$0$Proxy;
            }
            break;
        case 1826985398:
            if (var10000.equals("equals(Ljava/lang/Object;)Z")) {
                return CGLIB$equals$1$Proxy;
            }
            break;
        case 1913648695:
            if (var10000.equals("toString()Ljava/lang/String;")) {
                return CGLIB$toString$2$Proxy;
            }
            break;
        case 1984935277:
            if (var10000.equals("hashCode()I")) {
                return CGLIB$hashCode$3$Proxy;
            }
        }
        return null;
    }
    public EnhancerDemo$$EnhancerByCGLIB$$da6c48f9() {
        CGLIB$BIND_CALLBACKS(this);
    }
    public static void CGLIB$SET_THREAD_CALLBACKS(Callback[] var0) {
        CGLIB$THREAD_CALLBACKS.set(var0);
    }
    public static void CGLIB$SET_STATIC_CALLBACKS(Callback[] var0) {
        CGLIB$STATIC_CALLBACKS = var0;
    }
    private static final void CGLIB$BIND_CALLBACKS(Object var0) {
        EnhancerDemo$$EnhancerByCGLIB$$da6c48f9 var1 = (EnhancerDemo$$EnhancerByCGLIB$$da6c48f9)var0;
        if (!var1.CGLIB$BOUND) {
            var1.CGLIB$BOUND = true;
            Object var10000 = CGLIB$THREAD_CALLBACKS.get();
            if (var10000 == null) {
                var10000 = CGLIB$STATIC_CALLBACKS;
                if (var10000 == null) {
                    return;
                }
            }
            var1.CGLIB$CALLBACK_0 = (MethodInterceptor)((Callback[])var10000)[0];
        }
    }
    public Object newInstance(Callback[] var1) {
        CGLIB$SET_THREAD_CALLBACKS(var1);
        EnhancerDemo$$EnhancerByCGLIB$$da6c48f9 var10000 = new EnhancerDemo$$EnhancerByCGLIB$$da6c48f9();
        CGLIB$SET_THREAD_CALLBACKS((Callback[])null);
        return var10000;
    }
    public Object newInstance(Callback var1) {
        CGLIB$SET_THREAD_CALLBACKS(new Callback[]{var1});
        EnhancerDemo$$EnhancerByCGLIB$$da6c48f9 var10000 = new EnhancerDemo$$EnhancerByCGLIB$$da6c48f9();
        CGLIB$SET_THREAD_CALLBACKS((Callback[])null);
        return var10000;
    }
    public Object newInstance(Class[] var1, Object[] var2, Callback[] var3) {
        CGLIB$SET_THREAD_CALLBACKS(var3);
        EnhancerDemo$$EnhancerByCGLIB$$da6c48f9 var10000 = new EnhancerDemo$$EnhancerByCGLIB$$da6c48f9;
        switch(var1.length) {
        case 0:
            var10000.<init>();
            CGLIB$SET_THREAD_CALLBACKS((Callback[])null);
            return var10000;
        default:
            throw new IllegalArgumentException("Constructor not found");
        }
    }
    public Callback getCallback(int var1) {
        CGLIB$BIND_CALLBACKS(this);
        MethodInterceptor var10000;
        switch(var1) {
        case 0:
            var10000 = this.CGLIB$CALLBACK_0;
            break;
        default:
            var10000 = null;
        }
        return var10000;
    }
    public void setCallback(int var1, Callback var2) {
        switch(var1) {
        case 0:
            this.CGLIB$CALLBACK_0 = (MethodInterceptor)var2;
        default:
        }
    }
    public Callback[] getCallbacks() {
        CGLIB$BIND_CALLBACKS(this);
        return new Callback[]{this.CGLIB$CALLBACK_0};
    }
    public void setCallbacks(Callback[] var1) {
        this.CGLIB$CALLBACK_0 = (MethodInterceptor)var1[0];
    }
    static {
        CGLIB$STATICHOOK1();
    }
}
```
从结果上可以看出Cglib是使用继承的方式实现的动态代理。关键代码：
```java
public final String test() {
    MethodInterceptor var10000 = this.CGLIB$CALLBACK_0;
    if (var10000 == null) {
        CGLIB$BIND_CALLBACKS(this);
        var10000 = this.CGLIB$CALLBACK_0;
    }
    return var10000 != null ? (String)var10000.intercept(this, CGLIB$test$0$Method, CGLIB$emptyArgs, CGLIB$test$0$Proxy) : super.test();
}
```
从上面的代码可以发现方法的执行会委托给`MethodInterceptor`的`intercept`方法。从而实现对方法的增强。
:::tips
**由于 Cglib 是使用继承方式，所有**`**final**`**类是不能使用cglib代理的，会直接抛出异常，**`**final**`**方法也将不会被代理，因为不能覆盖，不会直接抛出异常**
:::
<a name="RKsKc"></a>
#### `CglibAopProxy`
`CglibAopProxy`是Spring中Cglib动态代理的实现，在`getProxy`方法中完成了对`Enhancer`的创建和封装。在`getProxy`方法中`getCallbacks(rootClass);`方法会将拦截链封装成`DynamicAdvisedInterceptor`对象，并放到Cglib的回调函数中。源码如下：
```java
@Override
public Object getProxy(ClassLoader classLoader) {
    if (logger.isDebugEnabled()) {
        logger.debug("Creating CGLIB proxy: target source is " + this.advised.getTargetSource());
    }
    try {
        Class<?> rootClass = this.advised.getTargetClass();
        Assert.state(rootClass != null, "Target class must be available for creating a CGLIB proxy");
        Class<?> proxySuperClass = rootClass;
        if (ClassUtils.isCglibProxyClass(rootClass)) {
            proxySuperClass = rootClass.getSuperclass();
            Class<?>[] additionalInterfaces = rootClass.getInterfaces();
            for (Class<?> additionalInterface : additionalInterfaces) {
                this.advised.addInterface(additionalInterface);
            }
        }
        // 验证Class
        validateClassIfNecessary(proxySuperClass, classLoader);
        // 创建以及配置Enhancer
        Enhancer enhancer = createEnhancer();
        if (classLoader != null) {
            enhancer.setClassLoader(classLoader);
            if (classLoader instanceof SmartClassLoader &&
                ((SmartClassLoader) classLoader).isClassReloadable(proxySuperClass)) {
                enhancer.setUseCache(false);
            }
        }
        enhancer.setSuperclass(proxySuperClass);
        enhancer.setInterfaces(AopProxyUtils.completeProxiedInterfaces(this.advised));
        enhancer.setNamingPolicy(SpringNamingPolicy.INSTANCE);
        enhancer.setStrategy(new ClassLoaderAwareUndeclaredThrowableStrategy(classLoader));
        // 将拦截链放到回调中
        Callback[] callbacks = getCallbacks(rootClass);
        Class<?>[] types = new Class<?>[callbacks.length];
        for (int x = 0; x < types.length; x++) {
            types[x] = callbacks[x].getClass();
        }
        // fixedInterceptorMap only populated at this point, after getCallbacks call above
        enhancer.setCallbackFilter(new ProxyCallbackFilter(
            this.advised.getConfigurationOnlyCopy(), this.fixedInterceptorMap, this.fixedInterceptorOffset));
        enhancer.setCallbackTypes(types);
        // 生成代理类并创建代理实例
        return createProxyClassAndInstance(enhancer, callbacks);
    }
    ...
}

private Callback[] getCallbacks(Class<?> rootClass) throws Exception {
    // Parameters used for optimization choices...
    boolean exposeProxy = this.advised.isExposeProxy();
    boolean isFrozen = this.advised.isFrozen();
    boolean isStatic = this.advised.getTargetSource().isStatic();
    // Choose an "aop" interceptor (used for AOP calls).
    // 将拦截链封装到DynamicAdvisedInterceptor中
    Callback aopInterceptor = new DynamicAdvisedInterceptor(this.advised);
    // Choose a "straight to target" interceptor. (used for calls that are
    // unadvised but can return this). May be required to expose the proxy.
    Callback targetInterceptor;
    if (exposeProxy) {
        targetInterceptor = isStatic ?
            new StaticUnadvisedExposedInterceptor(this.advised.getTargetSource().getTarget()) :
        new DynamicUnadvisedExposedInterceptor(this.advised.getTargetSource());
    }
    else {
        targetInterceptor = isStatic ?
            new StaticUnadvisedInterceptor(this.advised.getTargetSource().getTarget()) :
        new DynamicUnadvisedInterceptor(this.advised.getTargetSource());
    }
    // Choose a "direct to target" dispatcher (used for
    // unadvised calls to static targets that cannot return this).
    Callback targetDispatcher = isStatic ?
        new StaticDispatcher(this.advised.getTargetSource().getTarget()) : new SerializableNoOp();
    Callback[] mainCallbacks = new Callback[] {
        // 将拦截链加入到CallBack中
        aopInterceptor,  // for normal advice
        targetInterceptor,  // invoke target without considering advice, if optimized
        new SerializableNoOp(),  // no override for methods mapped to this
        targetDispatcher, this.advisedDispatcher,
        new EqualsInterceptor(this.advised),
        new HashCodeInterceptor(this.advised)
        };
    Callback[] callbacks;
    // If the target is a static one and the advice chain is frozen,
    // then we can make some optimizations by sending the AOP calls
    // direct to the target using the fixed chain for that method.
    if (isStatic && isFrozen) {
        Method[] methods = rootClass.getMethods();
        Callback[] fixedCallbacks = new Callback[methods.length];
        this.fixedInterceptorMap = new HashMap<String, Integer>(methods.length);
        // TODO: small memory optimization here (can skip creation for methods with no advice)
        for (int x = 0; x < methods.length; x++) {
            List<Object> chain = this.advised.getInterceptorsAndDynamicInterceptionAdvice(methods[x], rootClass);
            fixedCallbacks[x] = new FixedChainStaticTargetInterceptor(
                chain, this.advised.getTargetSource().getTarget(), this.advised.getTargetClass());
            this.fixedInterceptorMap.put(methods[x].toString(), x);
        }
        // Now copy both the callbacks from mainCallbacks
        // and fixedCallbacks into the callbacks array.
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
<a name="EDRzs"></a>
## 原理区别
java动态代理是利用反射机制生成一个实现代理接口的匿名类，在调用具体方法前调用`InvokeHandler`来处理。而cglib动态代理是利用asm开源包，对代理对象类的class文件加载进来，通过修改其字节码生成子类来处理。

1. 如果目标对象实现了接口，默认情况下会采用JDK的动态代理实现AOP
2. 如果目标对象实现了接口，可以强制使用CGLIB实现AOP
3. 如果目标对象没有实现了接口，必须采用CGLIB库，Spring会自动在JDK动态代理和CGLIB之间转换
<a name="gvDlV"></a>
## Cglib 与 JDK动态代理的运行性能比较
结论：从 jdk6 到 jdk7、jdk8 ，动态代理的性能得到了显著的提升，尤其是JDK7和JDK8性能表现都比cglib好，而 cglib 的表现并未跟上，甚至可能会略微下降。所以建议尽量使用 JDK 的动态代理。
