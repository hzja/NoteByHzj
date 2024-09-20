JDK 动态代理<br />先从JDK的源代码说起，动态代理这部分源码，Oracle版本和OpenJDK的源码是不太一样的，这里就挑选OpenJDK的。<br />回顾一下JDK动态代理，先说宏观原理，相信都懂，使用JDK动态代理最常见，Spring的AOP部分，并且是AOP部分的声明式事务部分。<br />a、定义一个接口Car：
```java
public interface Car {
    void drive(String driverName, String carName);
}
```
b、定义接口Car的一个实现类Audi：
```java
public class Audi implements Car {

    @Override
    public void drive(String driverName, String carName) {
        System.err.println("Audi is driving... " + "driverName: " + driverName + ", carName" + carName);
    }
}
```
c、定义一个动态调用的控制器CarHandler：
```java
public class CarHandler implements InvocationHandler {
    
    private Car car;
    
    public CarHandler(Car car) {
        this.car = car;
    }
    
    @Override
    public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
        System.err.println("before");
        method.invoke(car, args);
        System.err.println("after");
        return null;
    }

}
```
d、测试类ProxyTest：
```java
public class ProxyTest {
    
    @Test
    public void proxyTest() throws Exception {
        Car audi = (Car) Proxy.newProxyInstance(Car.class.getClassLoader(), new Class<?>[] {Car.class}, new CarHandler(new Audi()));
        audi.drive("name1", "audi");
    }
}
```
e、输出结果：
```java
before
Audi is driving... driverName: name1, carNameaudi
after
```
上面这段，相信大家都懂，也明白原理，这就是所谓的知其然，但是不一定都能知其所以然。接下来就解释下“知其所以然”。<br />进入到`Proxy`类的`newProxyInstance`方法：
```java
public static Object newProxyInstance(ClassLoader loader,
                                      Class<?>[] interfaces,
                                      InvocationHandler h)
    throws IllegalArgumentException
    {
        if (h == null) {
            throw new NullPointerException();
        }

        /*
         * Look up or generate the designated proxy class.
         */
        Class<?> cl = getProxyClass(loader, interfaces);

        /*
         * Invoke its constructor with the designated invocation handler.
         */
        try {
            Constructor cons = cl.getConstructor(constructorParams);
            return cons.newInstance(new Object[] { h });
        } catch (NoSuchMethodException e) {
            throw new InternalError(e.toString());
        } catch (IllegalAccessException e) {
            throw new InternalError(e.toString());
        } catch (InstantiationException e) {
            throw new InternalError(e.toString());
        } catch (InvocationTargetException e) {
            throw new InternalError(e.toString());
        }
    }
```
关键的3行：
```java
// 创建代理类
Class<?> cl = getProxyClass(loader, interfaces);
// 实例化代理对象
Constructor cons = cl.getConstructor(constructorParams);
```
返回的是代理类的实例化对象。接下来的调用就很清晰了。<br />那么，JDK动态代理最核心的关键就是这个方法：
```java
Class<?> cl = getProxyClass(loader, interfaces);
```
进入该方法，这个方法很长，前面很多都是铺垫，在方法的最后调用了一个方法：
```java
byte[] proxyClassFile = ProxyGenerator.generateProxyClass(
                    proxyName, interfaces);
```
这个方法就是产生代理对象的方法。先不看前后，只关注这一个方法，自己来写一个该方法：
```java
public class ProxyTest {
    
    @SuppressWarnings("resource")
    @Test
    public void proxyTest() throws Exception {
        byte[] bs = ProxyGenerator.generateProxyClass("AudiImpl", new Class<?>[] {Car.class});
        new FileOutputStream(new File("d:/AudiImpl.class")).write(bs);
    }
}
```
于是，就在D盘里面看到了一个叫做AudiImpl.class的文件，对该文件进行反编译，得到下面这个类：
```java
public final class AudiImpl extends Proxy implements Car {

    private static final long serialVersionUID = 5351158173626517207L;

    private static Method m1;
    private static Method m3;
    private static Method m0;
    private static Method m2;

    public AudiImpl(InvocationHandler paramInvocationHandler) {
        super(paramInvocationHandler);
    }

    public final boolean equals(Object paramObject) {
        try {
            return ((Boolean) this.h.invoke(this, m1, new Object[] { paramObject })).booleanValue();
        } catch (Error | RuntimeException localError) {
            throw localError;
        } catch (Throwable localThrowable) {
            throw new UndeclaredThrowableException(localThrowable);
        }
    }

    public final void drive(String paramString1, String paramString2) {
        try {
            this.h.invoke(this, m3, new Object[] { paramString1, paramString2 });
            return;
        } catch (Error | RuntimeException localError) {
            throw localError;
        } catch (Throwable localThrowable) {
            throw new UndeclaredThrowableException(localThrowable);
        }
    }

    public final int hashCode() {
        try {
            return ((Integer) this.h.invoke(this, m0, null)).intValue();
        } catch (Error | RuntimeException localError) {
            throw localError;
        } catch (Throwable localThrowable) {
            throw new UndeclaredThrowableException(localThrowable);
        }
    }

    public final String toString() {
        try {
            return (String) this.h.invoke(this, m2, null);
        } catch (Error | RuntimeException localError) {
            throw localError;
        } catch (Throwable localThrowable) {
            throw new UndeclaredThrowableException(localThrowable);
        }
    }

    static {
        try {
            m1 = Class.forName("java.lang.Object").getMethod("equals",
                    new Class[] { Class.forName("java.lang.Object") });
            m3 = Class.forName("com.mook.core.service.Car").getMethod("drive",
                    new Class[] { Class.forName("java.lang.String"), Class.forName("java.lang.String") });
            m0 = Class.forName("java.lang.Object").getMethod("hashCode", new Class[0]);
            m2 = Class.forName("java.lang.Object").getMethod("toString", new Class[0]);
        } catch (NoSuchMethodException localNoSuchMethodException) {
            throw new NoSuchMethodError(localNoSuchMethodException.getMessage());
        } catch (ClassNotFoundException localClassNotFoundException) {
            throw new NoClassDefFoundError(localClassNotFoundException.getMessage());
        }
    }
}
```
这个时候，JDK动态代理所有的秘密都暴露从出来了，当调用`drive`方法的时候，实际上是把方法名称传给控制器，然后执行控制器逻辑。这就实现了动态代理。<br />Spring AOP有两种方式实现动态代理，如果基于接口编程，默认就是JDK动态代理，否则就是cglib方式，另外Spring的配置文件里面也可以设置使用cglib来做动态代理。<br />事实上，如果明白这一点，去阅读Mybatis源码的时候是很有帮助的，Mybatis的接口方式做方法查询就充分利用了这里的JDK动态代理。否则如果不明白原理，看Mybatis的源码的接口方式是很费劲的，当然了，这只是Mybatis利用动态代理的冰山一角，要完全看懂Mybatis源码还有其他的许多难点，比如Mybatis是以xml文件来配置sql语句的。
