Java 动态代理
:::info
代理模式的目的是在不修改原有类方法设计的基础上，对方法行为进行增强。
:::
为了好理解，举个实际场景，业务场景中经常有限流的需求，常规操作是在需要限流的接口代码前加入调用次数判断的代码，但是这样每个需要限流的方法都需要加，工作量大不说，一方面不好维护，不能很清晰的知道每个接口限流值，另一方面，限流代码和业务代码堆叠在一起，也影响代码阅读。解法是做一套统一限流，一般好点的会有专门的接口限流平台，配置对应的接口名，设置限流值，直接就可以限流，实现方式就可以用动态代理。不修改原接口的实现，对接口进行增强。<br />动态代理的优势是实现无侵入式的代码扩展，做方法的增强；可以在不用修改源码的情况下，增强一些方法；在方法的前后可以做任何想做的事情（甚至不去执行这个方法就可以）。
<a name="YS5L2"></a>
### 静态代理
既然有动态，那一定有静态，区别，<br />静态：最大的区别是静态是编译期就决定了，在程序运行之前，代理类的.class文件已经存在了。被代理类是什么，代理类实现方式。<br />举个栗子：<br />现在有个接口，是把Json字符串解析成Object 对象，接口如下：
```java
public interface IProvider {

    Object getData(String json);

}
```
接口的实现类如下：
```java
public class SimpleProvider implements IProvider {
    @Override
    public Object getData(String json) {
        //解析json 拿到数据
        return parseJson(json);
    }
}
```
那现在有个需求，需要对 getData 方法做限流，指定用静态代理的方式。<br />需要很简单，就直接贴了：
```java
public class ProviderProxy implements IProvider{

    //持有一个被代理对象的引用（在这里是SimpleProvider）
    IProvider iProvider;

    public StaticProviderProxy(IProvider iProvider){
        this.iProvider = iProvider;
    }

    @Override
    public Object getData(String json) {
        //做限流检查
        if(callSpeed > flowLimt) {
            //流量超限
            throw FlowLimitException();
        }
        Object object = iProvider.getData(json);
        return object;
    }
}
//main 
public static void main(String[] args) {
    IProvider provider = new ProviderProxy(new SimpleProvider());
    provider.getData("{\"data\":{}}");
}
```
这就是静态代理，代理类（ProviderProxy）实现和需要做方法增强的被代理类（SimpleProvider）实现同一个接口（IProvider），方法具体实现上做增强，这里是限流检查。
<a name="MaC8o"></a>
### 动态代理
Java 动态代理

- 动态代理类：在程序运行时，通过反射机制动态生成。
- 动态代理类通常代理接口下的所有类。静态一般指定某个类代理。
- 动态代理事先不知道要代理的是什么，只有在运行的时候才能确定。静态是编译期确定的。

还是以IProvider接口为例，同样是要对SimpleProvider做增强，如下：
```java
public class ProviderHandler implements InvocationHandler {
    Object target;

    public Object bind(Object target){
        this.target = target;
        //这里生成了代理对象
        return Proxy.newProxyInstance(target.getClass().getClassLoader(),
                                      target.getClass().getInterfaces(), this);
    }

    @Override
    public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
        //限流
        flowLimit(args);
        Object obj = method.invoke(target, args);
        //打印日志
        logger.info("print log...");
        return obj;
    }
}
//main
public static void main(String[] args) {
    ProviderHandler providerHandler = new ProviderHandler();
    IProvider iProvider = (IProvider) providerHandler.bind(new SimpleProvider());
    iProvider.getData("weibo.data");
}
```
这里有三个对象：

1. SimpleProvider 对象 , 我们称之为被代理对象
2. ProviderHandler 对象,我们称之为执行者对象
3. Proxy对象 （通过在ProviderHandler `bind`方法中使用`Proxy.newProxyInstance`生成的对象） 称之为代理对象

这三个对象是什么关系呢？<br />Proxy是真正的代理类，SimpleProvider是被代理类，ProviderHandler是执行方法增强的执行者。<br />为了增强SimpleProvider （被代理对象）的`getData`方法，就Proxy对象来代理被代理对象的执行，Proxy不亲自来做这件事，而是交给执行者对象ProviderHandler 来实现增加的目录，执行调用前的限流校验。<br />实际怎么实现的呢？
<a name="Xx628"></a>
### newProxyInstance源码
```java
public static Object newProxyInstance(ClassLoader loader,
                                      Class<?>[] interfaces,
                                      InvocationHandler h)
    throws IllegalArgumentException
    {
        //对 Invocationhandler做判空处理
        Objects.requireNonNull(h);
        //复制[IProvider接口]
        final Class<?>[] intfs = interfaces.clone();

       //根据IProvider的类加载器IProvider接口生成了Proxy类,关键：根据类加载器和接口对象在JVM缓存中生成一个类对象
        Class<?> cl = getProxyClass0(loader, intfs);
        //获取构造器
        final Constructor<?> cons = cl.getConstructor(constructorParams);
        //保存InvocationHandler的引用
        final InvocationHandler ih = h;
        //通过构造器实例化Proxy代理对象
        return cons.newInstance(new Object[]{h});
    }
```
代码注释写的很清晰。<br />可能这个地方大家都会疑惑，生成的Proxy对象是怎样调用执行者的invoke函数的。<br />这个地方通过这段代码将Proxy0的class字节码输出到文件。
```java
byte[] classFile = ProxyGenerator.generateProxyClass("$Proxy0", WeiboProvider.class.getInterfaces());
String path = "C:**/IdeaProjects/study/out/production/study/SimpleProxy.class";
try(FileOutputStream fos = new FileOutputStream(path)) {
    fos.write(classFile);
    fos.flush();
    System.out.println("代理类class文件写入成功");
} catch (Exception e) {
    System.out.println("写文件错误");
}
```
反编译Proxy0如下：
```java
//Proxy0 是动态生成的类，继承自Proxy，实现了IProvider接口
public final class $Proxy0 extends Proxy implements IProvider {
    private static Method m1;
    private static Method m2;
    private static Method m3;
    private static Method m0;

    public $Proxy0(InvocationHandler var1) throws  {
        super(var1);
    }

    public final boolean equals(Object var1) throws  {
        try {
            return ((Boolean)super.h.invoke(this, m1, new Object[]{var1})).booleanValue();
        } catch (RuntimeException | Error var3) {
            throw var3;
        } catch (Throwable var4) {
            throw new UndeclaredThrowableException(var4);
        }
    }

    public final String toString() throws  {
        try {
            return (String)super.h.invoke(this, m2, (Object[])null);
        } catch (RuntimeException | Error var2) {
            throw var2;
        } catch (Throwable var3) {
            throw new UndeclaredThrowableException(var3);
        }
    }

    public final String getData(String var1) throws  {
        try {
            //m3就是IProvider 接口的getData方法 
            //super.h 是父类java.lang.reflect.Proxy的属性 InvocationHandler
            return (String)super.h.invoke(this, m3, new Object[]{var1});
        } catch (RuntimeException | Error var3) {
            throw var3;
        } catch (Throwable var4) {
            throw new UndeclaredThrowableException(var4);
        }
    }

    public final int hashCode() throws  {
        try {
            return ((Integer)super.h.invoke(this, m0, (Object[])null)).intValue();
        } catch (RuntimeException | Error var2) {
            throw var2;
        } catch (Throwable var3) {
            throw new UndeclaredThrowableException(var3);
        }
    }

    static {
        try {
            m1 = Class.forName("java.lang.Object").getMethod("equals", new Class[]{Class.forName("java.lang.Object")});
            m2 = Class.forName("java.lang.Object").getMethod("toString", new Class[0]);
            //m3就是IProvider 接口的getData方法
            m3 = Class.forName("aop.IProvider").getMethod("getData", new Class[]{Class.forName("java.lang.String")});
            m0 = Class.forName("java.lang.Object").getMethod("hashCode", new Class[0]);
        } catch (NoSuchMethodException var2) {
            throw new NoSuchMethodError(var2.getMessage());
        } catch (ClassNotFoundException var3) {
            throw new NoClassDefFoundError(var3.getMessage());
        }
    }
}
```
重点在`return (String)super.h.invoke(this, m3, new Object[]{var1});`代码。<br />`$Proxy0`继承`Proxy`类，实现了IProvider接口，所以也有`getData()`函数，而getData函数调用的是执行者InvocationHandler的invoke方法，m3是通过反射拿到的Method对象，所以看getData调用invoke传递的。三个参数，第一个是Proxy对象，第二个是getData方法对象，第三个是参数。<br />总结一下：

- 动态代理的本质就是，生成一个继承自Proxy，实现被代理接口(IProvider)的类 - Proxy0。
- Proxy0 持有InvocationHandler实例，InvocationHandler 持有SimpleProvider实例。Proxy0调用接口 getData方法时，先传递给InvocationHandler，InvocationHandler再传递给SimpleProvider实例。

动态代理实际上就是帮我们在JVM内存中直接重新生成了代理类class和对应类对象，然后通过执行者InvocationHandler调用被代理对象SimpleProvider。
<a name="uy6Qj"></a>
### Spring AOP中的代理
Spring代理其实是对JDK动态代理和CGLIB代理进行了封装，并且引入了AOP的概念，同时引入了AspectJ中的一些注解：`@pointCut` `@After` 等。
```java
public AopProxy createAopProxy(AdvisedSupport config) throws AopConfigException {
    if (config.isOptimize() || config.isProxyTargetClass() || hasNoUserSuppliedProxyInterfaces(config)) {
        Class<?> targetClass = config.getTargetClass();
        if (targetClass == null) {
            throw new AopConfigException("TargetSource cannot determine target class: " +
                                         "Either an interface or a target is required for proxy creation.");
        }
        // 如果是接口，使用jdk代理 
        if (targetClass.isInterface() || Proxy.isProxyClass(targetClass)) {
            return new JdkDynamicAopProxy(config);
        }
        //否则使用cglib
        return new ObjenesisCglibAopProxy(config);
    }
    else {
        return new JdkDynamicAopProxy(config);
    }
}
```
