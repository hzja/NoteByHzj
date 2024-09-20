Java
<a name="RoR0R"></a>
## 为什么要用代理
先来看一个案例。<br />有一个接口IService，如下：
```java
package com.javacode2018.lesson001.demo15;

public interface IService {
    void m1();
    void m2();
    void m3();
}
```
接口有2个实现类ServiceA和ServiceB，如下：
```java
package com.javacode2018.lesson001.demo15;

public class ServiceA implements IService {
    @Override
    public void m1() {
        System.out.println("我是ServiceA中的m1方法!");
    }

    @Override
    public void m2() {
        System.out.println("我是ServiceA中的m2方法!");
    }

    @Override
    public void m3() {
        System.out.println("我是ServiceA中的m3方法!");
    }
}
```
```java
package com.javacode2018.lesson001.demo15;

public class ServiceB implements IService {
    @Override
    public void m1() {
        System.out.println("我是ServiceB中的m1方法!");
    }

    @Override
    public void m2() {
        System.out.println("我是ServiceB中的m2方法!");
    }

    @Override
    public void m3() {
        System.out.println("我是ServiceB中的m3方法!");
    }
}
```
来个测试用例来调用上面类的方法，如下：
```java
package com.javacode2018.lesson001.demo15;

import org.junit.Test;

public class ProxyTest {
    @Test
    public void m1() {
        IService serviceA = new ServiceA();
        IService serviceB = new ServiceB();
        serviceA.m1();
        serviceA.m2();
        serviceA.m3();

        serviceB.m1();
        serviceB.m2();
        serviceB.m3();
    }
}
```
上面的代码很简单，就不解释了，运行一下m1()方法，输出：
```
我是ServiceA中的m1方法!
我是ServiceA中的m2方法!
我是ServiceA中的m3方法!
我是ServiceA中的m1方法!
我是ServiceA中的m2方法!
我是ServiceA中的m3方法!
```
**上面是原本的程序，突然领导有个需求：调用IService接口中的任何方法的时候，需要记录方法的耗时。**<br />**此时会怎么做呢？**<br />**IService接口有2个实现类ServiceA和ServiceB，可以在这两个类的所有方法中加上统计耗时的代码，如果IService接口有几十个实现，是不是要修改很多代码，所有被修改的方法需重新测试？是不是非常痛苦，不过上面这种修改代码的方式倒是可以解决问题，只是增加了很多工作量（编码 & 测试）。**<br />**突然有一天，领导又说，要将这些耗时统计发送到监控系统用来做监控报警使用。**<br />**此时是不是又要去一个修改上面的代码？又要去测试？此时的系统是难以维护。**<br />**还有假如上面这些类都是第三方以jar包的方式提供的，此时这些类都是class文件，此时无法去修改源码。**<br />**比较好的方式**：可以为IService接口创建一个代理类，通过这个代理类来间接访问IService接口的实现类，在这个代理类中去做耗时及发送至监控的代码，代码如下：
```java
package com.javacode2018.lesson001.demo15;

// IService的代理类
public class ServiceProxy implements IService {
    //目标对象，被代理的对象
    private IService target;

    public ServiceProxy(IService target) {
        this.target = target;
    }

    @Override
    public void m1() {
        long starTime = System.nanoTime();
        this.target.m1();
        long endTime = System.nanoTime();
        System.out.println(this.target.getClass() + ".m1()方法耗时(纳秒):" + (endTime - starTime));
    }

    @Override
    public void m2() {
        long starTime = System.nanoTime();
        this.target.m1();
        long endTime = System.nanoTime();
        System.out.println(this.target.getClass() + ".m1()方法耗时(纳秒):" + (endTime - starTime));
    }

    @Override
    public void m3() {
        long starTime = System.nanoTime();
        this.target.m1();
        long endTime = System.nanoTime();
        System.out.println(this.target.getClass() + ".m1()方法耗时(纳秒):" + (endTime - starTime));
    }
}
```
`ServiceProxy`是IService接口的代理类，`target`为被代理的对象，即实际需要访问的对象，也实现了IService接口，上面的3个方法中加了统计耗时的代码，当需要访问IService的其他实现类的时候，可以通过`ServiceProxy`来间接的进行访问，用法如下：
```java
@Test
public void serviceProxy() {
    IService serviceA = new ServiceProxy(new ServiceA());//@1
    IService serviceB = new ServiceProxy(new ServiceB()); //@2
    serviceA.m1();
    serviceA.m2();
    serviceA.m3();

    serviceB.m1();
    serviceB.m2();
    serviceB.m3();
}
```
上面代码重点在于@1和@2，创建的是代理对象ServiceProxy，ServiceProxy构造方法中传入了被代理访问的对象，现在访问ServiceA或者ServiceB，都需要经过`ServiceProxy`，运行输出：
```
我是ServiceA中的m1方法!
class com.javacode2018.lesson001.demo15.ServiceA.m1()方法耗时(纳秒):90100
我是ServiceA中的m1方法!
class com.javacode2018.lesson001.demo15.ServiceA.m1()方法耗时(纳秒):31600
我是ServiceA中的m1方法!
class com.javacode2018.lesson001.demo15.ServiceA.m1()方法耗时(纳秒):25800
我是ServiceB中的m1方法!
class com.javacode2018.lesson001.demo15.ServiceB.m1()方法耗时(纳秒):142100
我是ServiceB中的m1方法!
class com.javacode2018.lesson001.demo15.ServiceB.m1()方法耗时(纳秒):35000
我是ServiceB中的m1方法!
class com.javacode2018.lesson001.demo15.ServiceB.m1()方法耗时(纳秒):32900
```
上面实现中没有去修改ServiceA和ServiceB中的方法，只是给IService接口创建了一个代理类，通过代理类去访问目标对象，需要添加的一些共有的功能都放在代理中，当领导有其他需求的时候，只需修改`ServiceProxy`的代码，方便系统的扩展和测试。<br />假如现在需要给系统中所有接口都加上统计耗时的功能，若按照上面的方式，需要给每个接口创建一个代理类，此时代码量和测试的工作量也是巨大的，那么能不能写一个通用的代理类，来满足上面的功能呢？<br />**通用代理的2种实现：**

1. **jdk动态代理**
2. **cglib代理**
<a name="Dy6lc"></a>
## jdk动态代理详解
jdk中为实现代理提供了支持，主要用到2个类：
```
java.lang.reflect.Proxy
java.lang.reflect.InvocationHandler
```
**jdk自带的代理使用上面有个限制，只能为接口创建代理类，如果需要给具体的类创建代理类，需要用后面要说的cglib**
<a name="HJmyp"></a>
### `java.lang.reflect.Proxy`
这是jdk动态代理中主要的一个类，里面有一些静态方法会经常用到，来熟悉一下：
<a name="rqAb4"></a>
#### `getProxyClass`方法
为指定的接口创建代理类，返回代理类的Class对象
```java
public static Class<?> getProxyClass(ClassLoader loader,
                                     Class<?>... interfaces)
```
参数说明：<br />loader：定义代理类的类加载器<br />interfaces：指定需要实现的接口列表，创建的代理默认会按顺序实现interfaces指定的接口
<a name="R29AQ"></a>
#### `newProxyInstance`方法
创建代理类的实例对象
```java
public static Object newProxyInstance(ClassLoader loader,
                                      Class<?>[] interfaces,
                                      InvocationHandler h)
```
这个方法先为指定的接口创建代理类，然后会生成代理类的一个实例，最后一个参数比较特殊，是`InvocationHandler`类型的，这个接口方法如下：
```java
public Object invoke(Object proxy, Method method, Object[] args) throws Throwable;
```
上面方法会返回一个代理对象，当调用代理对象的任何方法的时候，会就被`InvocationHandler`接口的`invoke`方法处理，所以主要代码需要卸载`invoke`方法中，稍后会有案例细说。
<a name="xjqCo"></a>
#### `isProxy`方法
判断指定的类是否是一个代理类
```java
public static boolean isProxyClass(Class<?> cl)
```
<a name="UxCRG"></a>
#### `getInvocationHandler`方法
获取代理对象的`InvocationHandler`对象
```java
public static InvocationHandler getInvocationHandler(Object proxy) throws IllegalArgumentException
```
**上面几个方法大家熟悉一下，下面来看创建代理具体的2种方式。**
<a name="Ku1b7"></a>
### 创建代理：方式一
<a name="ahsrZ"></a>
#### 步骤

1. 调用`Proxy.getProxyClass`方法获取代理类的Class对象
2. 使用`InvocationHandler`接口创建代理类的处理器
3. 通过代理类和`InvocationHandler`创建代理对象
4. 上面已经创建好代理对象了，接着就可以使用代理对象了
<a name="H6OGm"></a>
#### 案例
<a name="Qqo2s"></a>
##### 先来个接口IService
```java
package com.javacode2018.lesson001.demo16;

public interface IService {
    void m1();
    void m2();
    void m3();
}
```
<a name="Ul5NC"></a>
##### 创建IService接口的代理对象
```java
@Test
public void m1() throws NoSuchMethodException, IllegalAccessException, InvocationTargetException, InstantiationException {
    // 1. 获取接口对应的代理类
    Class<IService> proxyClass = (Class<IService>) Proxy.getProxyClass(IService.class.getClassLoader(), IService.class);
    // 2. 创建代理类的处理器
    InvocationHandler invocationHandler = new InvocationHandler() {
        @Override
        public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
            System.out.println("我是InvocationHandler，被调用的方法是：" + method.getName());
            return null;
        }
    };
    // 3. 创建代理实例
    IService proxyService = proxyClass.getConstructor(InvocationHandler.class).newInstance(invocationHandler);
    // 4. 调用代理的方法
    proxyService.m1();
    proxyService.m2();
    proxyService.m3();
}
```
<a name="ewxs7"></a>
##### 运行输出
```
我是InvocationHandler，被调用的方法是：m1
我是InvocationHandler，被调用的方法是：m2
我是InvocationHandler，被调用的方法是：m3
```
<a name="D0nxW"></a>
### 创建代理：方式二
创建代理对象有更简单的方式。
<a name="UusoA"></a>
#### 步骤

1. 使用`InvocationHandler`接口创建代理类的处理器
2. 使用`Proxy`类的静态方法`newProxyInstance`直接创建代理对象
3. 使用代理对象
<a name="hRUYE"></a>
#### 案例
```java
@Test
public void m2() throws NoSuchMethodException, IllegalAccessException, InvocationTargetException, InstantiationException {
    // 1. 创建代理类的处理器
    InvocationHandler invocationHandler = new InvocationHandler() {
        @Override
        public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
            System.out.println("我是InvocationHandler，被调用的方法是：" + method.getName());
            return null;
        }
    };
    // 2. 创建代理实例
    IService proxyService = (IService) Proxy.newProxyInstance(IService.class.getClassLoader(), new Class[]{IService.class}, invocationHandler);
    // 3. 调用代理的方法
    proxyService.m1();
    proxyService.m2();
    proxyService.m3();
}
```
运行输出：
```
我是InvocationHandler，被调用的方法是：m1
我是InvocationHandler，被调用的方法是：m2
我是InvocationHandler，被调用的方法是：m3
```
<a name="ei6F3"></a>
### 案例：任意接口中的方法耗时统计
**下面通过jdk动态代理实现一个通用的代理，解决统计所有接口方法耗时的问题。**<br />主要的代码在代理处理器`InvocationHandler`实现上面，如下：
```java
package com.javacode2018.lesson001.demo16;

import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;

public class CostTimeInvocationHandler implements InvocationHandler {

    private Object target;

    public CostTimeInvocationHandler(Object target) {
        this.target = target;
    }

    @Override
    public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
        long starTime = System.nanoTime();
        Object result = method.invoke(this.target, args);//@1
        long endTime = System.nanoTime();
        System.out.println(this.target.getClass() + ".m1()方法耗时(纳秒):" + (endTime - starTime));
        return result;
    }

    /**
     * 用来创建targetInterface接口的代理对象
     *
     * @param target          需要被代理的对象
     * @param targetInterface 被代理的接口
     * @param <T>
     * @return
     */
    public static <T> T createProxy(Object target, Class<T> targetInterface) {
        if (!targetInterface.isInterface()) {
            throw new IllegalStateException("targetInterface必须是接口类型!");
        } else if (!targetInterface.isAssignableFrom(target.getClass())) {
            throw new IllegalStateException("target必须是targetInterface接口的实现类!");
        }
        return (T) Proxy.newProxyInstance(target.getClass().getClassLoader(), target.getClass().getInterfaces(), new CostTimeInvocationHandler(target));
    }
}
```
上面主要是`createProxy`方法用来创建代理对象，2个参数：<br />`target`：目标对象，需要实现`targetInterface`接口<br />`targetInterface`：需要创建代理的接口<br />`invoke`方法中通过`method.invoke(this.target, args)`调用目标方法，然后统计方法的耗时。<br />**测试用例**
```java
@Test
public void costTimeProxy() {
    IService serviceA = CostTimeInvocationHandler.createProxy(new ServiceA(), IService.class);
    IService serviceB = CostTimeInvocationHandler.createProxy(new ServiceB(), IService.class);
    serviceA.m1();
    serviceA.m2();
    serviceA.m3();

    serviceB.m1();
    serviceB.m2();
    serviceB.m3();
}
```
**运行输出**
```
我是ServiceA中的m1方法!
class com.javacode2018.lesson001.demo16.ServiceA.m1()方法耗时(纳秒):61300
我是ServiceA中的m2方法!
class com.javacode2018.lesson001.demo16.ServiceA.m1()方法耗时(纳秒):22300
我是ServiceA中的m3方法!
class com.javacode2018.lesson001.demo16.ServiceA.m1()方法耗时(纳秒):18700
我是ServiceB中的m1方法!
class com.javacode2018.lesson001.demo16.ServiceB.m1()方法耗时(纳秒):54700
我是ServiceB中的m2方法!
class com.javacode2018.lesson001.demo16.ServiceB.m1()方法耗时(纳秒):27200
我是ServiceB中的m3方法!
class com.javacode2018.lesson001.demo16.ServiceB.m1()方法耗时(纳秒):19800
```
**再来的接口，也需要统计耗时的功能，此时无需去创建新的代理类即可实现同样的功能，如下：**<br />**IUserService接口**
```java
package com.javacode2018.lesson001.demo16;

public interface IUserService {
    /**
     * 插入用户信息
     * @param name
     */
    void insert(String name);
}
```
**IUserService接口实现类：**
```java
package com.javacode2018.lesson001.demo16;

public class UserService implements IUserService {
    @Override
    public void insert(String name) {
        System.out.println(String.format("用户[name:%s]插入成功!", name));
    }
}
```
**测试用例**
```java
@Test
public void userService() {
    IUserService userService = CostTimeInvocationHandler.createProxy(new UserService(), IUserService.class);
    userService.insert("Java");
}
```
**运行输出：**
```
用户[name:Java]插入成功!
class com.javacode2018.lesson001.demo16.UserService.m1()方法耗时(纳秒):193000
```
**上面当创建一个新的接口的时候，不需要再去新建一个代理类了，只需要使用**`**CostTimeInvocationHandler.createProxy**`**创建一个新的代理对象就可以了，方便了很多。**
<a name="bjxFd"></a>
### `Proxy`使用注意

1. **jdk中的**`**Proxy**`**只能为接口生成代理类，如果想给某个类创建代理类，那么**`**Proxy**`**是无能为力的，此时需要用到下面要说的cglib了。**
2. `**Proxy**`**类中提供的几个常用的静态方法大家需要掌握**
3. **通过Proxy创建代理对象，当调用代理对象任意方法时候，会被**`**InvocationHandler**`**接口中的**`**invoke**`**方法进行处理，这个接口内容是关键**
<a name="RLEX0"></a>
## cglib代理详解
<a name="ypjUf"></a>
### 什么是cglib
jdk动态代理只能为接口创建代理，使用上有局限性。实际的场景中的类不一定有接口，此时如果想为普通的类也实现代理功能，就需要用到cglib来实现了。<br />cglib是一个强大、高性能的字节码生成库，它用于在运行时扩展Java类和实现接口；本质上它是通过动态的生成一个子类去覆盖所要代理的类（非`final`修饰的类和方法）。`Enhancer`可能是CGLIB中最常用的一个类，和jdk中的`Proxy`不同的是，`Enhancer`既能够代理普通的class，也能够代理接口。`Enhancer`创建一个被代理对象的子类并且拦截所有的方法调用（包括从`Object`中继承的`toString`和`hashCode`方法）。`Enhancer`不能够拦截`final`方法，例如`Object.getClass()`方法，这是由于Java `final`方法语义决定的。基于同样的道理，`Enhancer`也不能对`final`类进行代理操作。<br />CGLIB作为一个开源项目，其代码托管在github，地址为：[https://github.com/cglib/cglib](https://github.com/cglib/cglib)
<a name="pCz6o"></a>
### cglib组成结构
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1689833113937-b553630e-6027-4eca-85ef-b3a5cc838c54.png#averageHue=%23667c70&clientId=u8cfe0704-605f-4&from=paste&id=u2ef980d6&originHeight=279&originWidth=450&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc1c5c375-adcf-46da-919c-0476f7d411a&title=)<br />CGLIB底层使用了ASM（一个短小精悍的字节码操作框架）来操作字节码生成新的类。除了CGLIB库外，脚本语言（如Groovy和BeanShell）也使用ASM生成字节码。ASM使用类似SAX的解析器来实现高性能。不鼓励直接使用ASM，因为它需要对Java字节码的格式足够的了解。<br />Spring已将第三方cglib jar包中所有的类集成到Spring自己的jar包中，为了方便，直接使用Spring内部已集成的来讲解
<a name="tOXus"></a>
### 5个案例来演示cglib常见的用法
<a name="hRi6K"></a>
#### 案例1：拦截所有方法（`MethodInterceptor`）
创建一个具体的类，如下：
```java
package com.javacode2018.lesson001.demo17;

public class Service1 {
    public void m1() {
        System.out.println("我是m1方法");
    }

    public void m2() {
        System.out.println("我是m2方法");
    }
}
```
下面为这个类创建一个代理，代理中实现打印每个方法的调用日志。
```java
package com.javacode2018.lesson001.demo17;

import org.junit.Test;
import org.springframework.cglib.proxy.Enhancer;
import org.springframework.cglib.proxy.MethodInterceptor;
import org.springframework.cglib.proxy.MethodProxy;

import java.lang.reflect.Method;

public class CglibTest {

    @Test
    public void test1() {
        //使用Enhancer来给某个类创建代理类，步骤
        //1.创建Enhancer对象
        Enhancer enhancer = new Enhancer();
        //2.通过setSuperclass来设置父类型，即需要给哪个类创建代理类
        enhancer.setSuperclass(Service1.class);
        /*3.设置回调，需实现org.springframework.cglib.proxy.Callback接口，
        此处使用的是org.springframework.cglib.proxy.MethodInterceptor，也是一个接口，实现了Callback接口，
        当调用代理对象的任何方法的时候，都会被MethodInterceptor接口的invoke方法处理*/
        enhancer.setCallback(new MethodInterceptor() {
            /**
             * 代理对象方法拦截器
             * @param o 代理对象
             * @param method 被代理的类的方法，即Service1中的方法
             * @param objects 调用方法传递的参数
             * @param methodProxy 方法代理对象
             * @return
             * @throws Throwable
             */
            @Override
            public Object intercept(Object o, Method method, Object[] objects, MethodProxy methodProxy) throws Throwable {
                System.out.println("调用方法:" + method);
                //可以调用MethodProxy的invokeSuper调用被代理类的方法
                Object result = methodProxy.invokeSuper(o, objects);
                return result;
            }
        });
        //4.获取代理对象,调用enhancer.create方法获取代理对象，这个方法返回的是Object类型的，所以需要强转一下
        Service1 proxy = (Service1) enhancer.create();
        //5.调用代理对象的方法
        proxy.m1();
        proxy.m2();
    }
}
```
上面代码中的注释很详细，列出了给指定的类创建代理的具体步骤，整个过程中主要用到了`Enhancer`类和`MethodInterceptor`接口。<br />`enhancer.setSuperclass`用来设置代理类的父类，即需要给哪个类创建代理类，此处是Service1<br />`enhancer.setCallback`传递的是`MethodInterceptor`接口类型的参数，`MethodInterceptor`接口有个`intercept`方法，这个方法会拦截代理对象所有的方法调用。<br />还有一个重点是`Object result = methodProxy.invokeSuper(o, objects);`可以调用被代理类，也就是Service1类中的具体的方法，从方法名称的意思可以看出是调用父类，实际对某个类创建代理，cglib底层通过修改字节码的方式为Service1类创建了一个子类。<br />运行输出：
```
调用方法:public void com.javacode2018.lesson001.demo17.Service1.m1()
我是m1方法
调用方法:public void com.javacode2018.lesson001.demo17.Service1.m2()
我是m2方法
```
从输出中可以看出Service1中的2个方法都被`MethodInterceptor`中的`invoke`拦截处理了。
<a name="TyicP"></a>
#### 案例2：拦截所有方法（`MethodInterceptor`）
在创建一个类，如下：
```java
package com.javacode2018.lesson001.demo17;


public class Service2 {
    public void m1() {
        System.out.println("我是m1方法");
        this.m2(); //@1
    }

    public void m2() {
        System.out.println("我是m2方法");
    }
}
```
这个类和上面的Service1类似，有点不同是@1，在m1方法中调用了m2方法。<br />下面来采用案例1中同样的方式来给Service2创建代理，如下：
```java
@Test
public void test2() {
    Enhancer enhancer = new Enhancer();
    enhancer.setSuperclass(Service2.class);
    enhancer.setCallback(new MethodInterceptor() {
        @Override
        public Object intercept(Object o, Method method, Object[] objects, MethodProxy methodProxy) throws Throwable {
            System.out.println("调用方法:" + method);
            Object result = methodProxy.invokeSuper(o, objects);
            return result;
        }
    });
    Service2 proxy = (Service2) enhancer.create();
    proxy.m1(); //@1
}
```
注意上面@1的代码，只调用了m1方法，看一下输出效果：
```
调用方法:public void com.javacode2018.lesson001.demo17.Service2.m1()
我是m1方法
调用方法:public void com.javacode2018.lesson001.demo17.Service2.m2()
我是m2方法
```
从输出中可以看出m1和m2方法都被拦截器处理了，而m2方法是在Service1的m1方法中调用的，也被拦截处理了。<br />Spring中的`@Configuration`注解就是采用这种方式实现的，上个`@Configuration`案例眼熟一下：
```java
package com.javacode2018.lesson001.demo17;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class Config {

    @Bean
    public C1 c1(){
        return new C1();
    }
    @Bean
    public C2 c2(){
        C1 c1 = this.c1(); //@1
        return new C2(c1);
    }
    @Bean
    public C3 c3(){
        C1 c1 = this.c1(); //@2
        return new C3(c1);
    }

    public static class C1{}

    public static class C2{
        private C1 c1;

        public C2(C1 c1) {
            this.c1 = c1;
        }
    }
    public static class C3{
        private C1 c1;

        public C3(C1 c1) {
            this.c1 = c1;
        }
    }

}
```
上面代码中C2和C3依赖于C1，都是通过构造器注入C1，注意代码中的@1和@2都是调用c1方法获取容器中的C1，如何确保多次获取的C1都是一个的？这个地方就是使用cglib代理拦截@Bean注解的方法来实现的。
<a name="lL8hR"></a>
#### 案例3：拦截所有方法并返回固定值（`FixedValue`）
当调用某个类的任何方法的时候，都希望返回一个固定的值，此时可以使用`FixedValue`接口，如下：
```java
enhancer.setCallback(new FixedValue() {
    @Override
    public Object loadObject() throws Exception {
        return "Tom";
    }
});
```
上面创建的代理对象，调用其任意方法返回的都是"Tom"。<br />案例代码如下：<br />创建一个类Service3，如下：
```java
package com.javacode2018.lesson001.demo17;

public class Service3 {
    public String m1() {
        System.out.println("我是m1方法");
        return "hello:m1";
    }

    public String m2() {
        System.out.println("我是m2方法");
        return "hello:m2";
    }
}
```
对用的测试用例：
```java
@Test
public void test3() {
    Enhancer enhancer = new Enhancer();
    enhancer.setSuperclass(Service3.class);
    enhancer.setCallback(new FixedValue() {
        @Override
        public Object loadObject() throws Exception {
            return "Tom";
        }
    });
    Service3 proxy = (Service3) enhancer.create();
    System.out.println(proxy.m1());//@1
    System.out.println(proxy.m2()); //@2
    System.out.println(proxy.toString());//@3
}
```
@1、@2、@3调用了代理对象的3个方法，运行输出：<br />运行输出：
```
Tom
Tom
Tom
```
可以看出输出的都是一个相同的值。
<a name="aastX"></a>
#### 案例4：直接放行，不做任何操作（`NoOp.INSTANCE`）
`Callback`接口下面有个子接口`org.springframework.cglib.proxy.NoOp`，将这个作为`Callback`的时候，被调用的方法会直接放行，像没有任何代理一样，感受一下效果：
```java
@Test
public void test6() {
    Enhancer enhancer = new Enhancer();
    enhancer.setSuperclass(Service3.class);
    enhancer.setCallback(NoOp.INSTANCE);
    Service3 proxy = (Service3) enhancer.create();
    System.out.println(proxy.m1());
    System.out.println(proxy.m2());
}
```
运行输出：
```
我是m1方法
hello:m1
我是m2方法
hello:m2
```
从输出中可以看出，被调用的方法没有被代理做任何处理，直接进到目标类Service3的方法中了。
<a name="h3aMd"></a>
#### 案例5：不同的方法使用不同的拦截器（`CallbackFilter`）
有个类如下：
```java
package com.javacode2018.lesson001.demo17;

public class Service4 {
    public void insert1() {
        System.out.println("我是insert1");
    }

    public void insert2() {
        System.out.println("我是insert2");
    }

    public String get1() {
        System.out.println("我是get1");
        return "get1";
    }

    public String get2() {
        System.out.println("我是get2");
        return "get2";
    }
}
```
**需求，给这个类创建一个代理需要实现下面的功能：**

1. **以**`**insert**`**开头的方法需要统计方法耗时**
2. **以**`**get**`**开头的的方法直接返回固定字符串`Hi`**

下来看代码，然后再解释：
```java
@Test
public void test4() {
    Enhancer enhancer = new Enhancer();
    enhancer.setSuperclass(Service4.class);
    //创建2个Callback
    Callback[] callbacks = {
        //这个用来拦截所有insert开头的方法
        new MethodInterceptor() {
            @Override
            public Object intercept(Object o, Method method, Object[] objects, MethodProxy methodProxy) throws Throwable {
                long starTime = System.nanoTime();
                Object result = methodProxy.invokeSuper(o, objects);
                long endTime = System.nanoTime();
                System.out.println(method + "，耗时(纳秒):" + (endTime - starTime));
                return result;
            }
        },
        //下面这个用来拦截所有get开头的方法，返回固定值的
        new FixedValue() {
            @Override
            public Object loadObject() throws Exception {
                return "Hi";
            }
        }
    };
    enhancer.setCallbackFilter(new CallbackFilter() {
        @Override
        public int accept(Method method) {
            return 0;
        }
    });
    //调用enhancer的setCallbacks传递Callback数组
    enhancer.setCallbacks(callbacks);
    /**
     * 设置过滤器CallbackFilter
     * CallbackFilter用来判断调用方法的时候使用callbacks数组中的哪个Callback来处理当前方法
     * 返回的是callbacks数组的下标
     */
    enhancer.setCallbackFilter(new CallbackFilter() {
        @Override
        public int accept(Method method) {
            //获取当前调用的方法的名称
            String methodName = method.getName();
            /**
             * 方法名称以insert开头，
             * 返回callbacks中的第1个Callback对象来处理当前方法，
             * 否则使用第二个Callback处理被调用的方法
             */
            return methodName.startsWith("insert") ? 0 : 1;
        }
    });
    Service4 proxy = (Service4) enhancer.create();
    System.out.println("---------------");
    proxy.insert1();
    System.out.println("---------------");
    proxy.insert2();
    System.out.println("---------------");
    System.out.println(proxy.get1());
    System.out.println("---------------");
    System.out.println(proxy.get2());

}
```
运行输出：
```
---------------
我是insert1
public void com.javacode2018.lesson001.demo17.Service4.insert1()，耗时(纳秒):15396100
---------------
我是insert2
public void com.javacode2018.lesson001.demo17.Service4.insert2()，耗时(纳秒):66200
---------------
Hi
---------------
Hi
```
代码说明：<br />由于需求中要对不同的方法做不同的处理，所以需要有2个`Callback`对象，当调用代理对象的方法的时候，具体会走哪个Callback呢，此时会通过`CallbackFilter`中的`accept`来判断，这个方法返回`callbacks`数组的索引。<br />上面这个案例还有一种简单的实现，见案例6
<a name="uyZiF"></a>
#### 案例6：对案例5的优化（`CallbackHelper`）
cglib中有个`CallbackHelper`类，可以对案例5的代码进行有环，`CallbackHelper`类相当于对一些代码进行了封装，方便实现案例5的需求，实现如下：
```java
@Test
public void test5() {
    Enhancer enhancer = new Enhancer();
    //创建2个Callback
    Callback costTimeCallback = (MethodInterceptor) (Object o, Method method, Object[] objects, MethodProxy methodProxy) -> {
        long starTime = System.nanoTime();
        Object result = methodProxy.invokeSuper(o, objects);
        long endTime = System.nanoTime();
        System.out.println(method + "，耗时(纳秒):" + (endTime - starTime));
        return result;
    };
    //下面这个用来拦截所有get开头的方法，返回固定值的
    Callback fixdValueCallback = (FixedValue) () -> "Hi";
    CallbackHelper callbackHelper = new CallbackHelper(Service4.class, null) {
        @Override
        protected Object getCallback(Method method) {
            return method.getName().startsWith("insert") ? costTimeCallback : fixdValueCallback;
        }
    };
    enhancer.setSuperclass(Service4.class);
    //调用enhancer的setCallbacks传递Callback数组
    enhancer.setCallbacks(callbackHelper.getCallbacks());
    /**
     * 设置CallbackFilter,用来判断某个方法具体走哪个Callback
     */
    enhancer.setCallbackFilter(callbackHelper);
    Service4 proxy = (Service4) enhancer.create();
    System.out.println("---------------");
    proxy.insert1();
    System.out.println("---------------");
    proxy.insert2();
    System.out.println("---------------");
    System.out.println(proxy.get1());
    System.out.println("---------------");
    System.out.println(proxy.get2());

}
```
运行输出：
```
---------------
我是insert1
public void com.javacode2018.lesson001.demo17.Service4.insert1()，耗时(纳秒):9777500
---------------
我是insert2
public void com.javacode2018.lesson001.demo17.Service4.insert2()，耗时(纳秒):50600
---------------
Hi
---------------
Hi
```
输出效果和案例4一模一样的，上面重点在于`CallbackHelper`，里面做了一些封装，有兴趣的可以去看一下源码，比较简单。
<a name="WlNUc"></a>
#### 案例7：实现通用的统计任意类方法耗时代理类
直接上代码，比较简单，如下：
```java
package com.javacode2018.lesson001.demo17;


import org.springframework.cglib.proxy.Enhancer;
import org.springframework.cglib.proxy.MethodInterceptor;
import org.springframework.cglib.proxy.MethodProxy;

import java.lang.reflect.Method;

public class CostTimeProxy implements MethodInterceptor {
    //目标对象
    private Object target;

    public CostTimeProxy(Object target) {
        this.target = target;
    }

    @Override
    public Object intercept(Object o, Method method, Object[] objects, MethodProxy methodProxy) throws Throwable {
        long starTime = System.nanoTime();
        //调用被代理对象（即target）的方法，获取结果
        Object result = method.invoke(target, objects); //@1
        long endTime = System.nanoTime();
        System.out.println(method + "，耗时(纳秒)：" + (endTime - starTime));
        return result;
    }

    /**
     * 创建任意类的代理对象
     *
     * @param target
     * @param <T>
     * @return
     */
    public static <T> T createProxy(T target) {
        CostTimeProxy costTimeProxy = new CostTimeProxy(target);
        Enhancer enhancer = new Enhancer();
        enhancer.setCallback(costTimeProxy);
        enhancer.setSuperclass(target.getClass());
        return (T) enhancer.create();
    }
}
```
可以直接使用上面的静态方法`createProxy`来为目标对象target创建一个代理对象，被代理的对象自动实现方法调用耗时统计。<br />@1：调用被代理对象的方法获取真正的结果。<br />使用非常简单，来个测试用例，如下：
```java
@Test
public void test7() {
    //创建Service1代理
    Service1 service1 = CostTimeProxy.createProxy(new Service1());
    service1.m1();

    //创建Service3代理
    Service3 service3 = CostTimeProxy.createProxy(new Service3());
    System.out.println(service3.m1());
}
```
运行输出：
```
我是m1方法
public void com.javacode2018.lesson001.demo17.Service1.m1()，耗时(纳秒)：53200
我是m1方法
public java.lang.String com.javacode2018.lesson001.demo17.Service3.m1()，耗时(纳秒)：49200
hello:m1
```
<a name="ktJnP"></a>
## CGLIB和Java动态代理的区别

1. **Java动态代理只能够对接口进行代理，不能对普通的类进行代理（因为所有生成的代理类的父类为**`**Proxy**`**，Java类继承机制不允许多重继承）；CGLIB能够代理普通类；**
2. **Java动态代理使用Java原生的反射API进行操作，在生成类上比较高效；CGLIB使用ASM框架直接对字节码进行操作，在类的执行过程中比较高效**
