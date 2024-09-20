JavaSpringBoot<br />`AspectJProxyFactory`，可能大家对这个比较陌生，但是`@Aspect`这个注解大家应该很熟悉吧，通过这个注解在Spring环境中实现aop特别的方便。<br />而`AspectJProxyFactory`这个类可以通过解析`@Aspect`标注的类来生成代理aop代理对象，对开发者来说，使创建代理变的更简洁了。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683334967841-d8f04bd8-4d0c-44a8-b2bc-1db91be8c215.png#averageHue=%23fbfaef&clientId=uaaf963a3-61d3-4&from=paste&id=ub81ffcf9&originHeight=170&originWidth=640&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u1b0203aa-291d-4b14-9ab9-4ed7040f21f&title=)
<a name="t0StO"></a>
## 先了解几个概念
文中会涉及几个概念，先了解一下。
<a name="xdy0Y"></a>
### target
用来表示目标对象，即需要通过aop来增强的对象。
<a name="dcGNY"></a>
### proxy
代理对象，target通过aop增强之后生成的代理对象。
<a name="CI72z"></a>
## AspectJ
<a name="POwto"></a>
### AspectJ是什么?
AspectJ是一个面向切面的框架，是目前最好用，最方便的AOP框架，和Spring中的aop可以集成在一起使用，通过Aspectj提供的一些功能实现aop代理变得非常方便。
<a name="U01Dx"></a>
### AspectJ使用步骤

1. 创建一个类，使用`@Aspect`标注
2. `@Aspect`标注的类中，通过`@Pointcut`定义切入点
3. `@Aspect`标注的类中，通过`AspectJ`提供的一些通知相关的注解定义通知
4. 使用`AspectJProxyFactory`结合`@Ascpect`标注的类，来生成代理对象

先来个案例，感受一下AspectJ是多么的方便。<br />来个类
```java
package com.javacode2018.aop.demo9.test1;

public class Service1 {

    public void m1() {
        System.out.println("我是 m1 方法");
    }

    public void m2() {
        System.out.println(10 / 0);
        System.out.println("我是 m2 方法");
    }
}
```
通过AspectJ来对Service1进行增强，来2个通知，一个前置通知，一个异常通知，这2个通知需要对Service1中的所有方法生效，实现如下：
```java
package com.javacode2018.aop.demo9.test1;

import org.aspectj.lang.JoinPoint;
import org.aspectj.lang.annotation.AfterThrowing;
import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Before;
import org.aspectj.lang.annotation.Pointcut;

//@1：这个类需要使用@Aspect进行标注
@Aspect
public class Aspect1 {

    //@2：定义了一个切入点，可以匹配Service1中所有方法
    @Pointcut("execution(* com.javacode2018.aop.demo9.test1.Service1.*(..))")
    public void pointcut1() {
    }

    //@3：定义了一个前置通知，这个通知对刚刚上面我们定义的切入点中的所有方法有效
    @Before(value = "pointcut1()")
    public void before(JoinPoint joinPoint) {
        //输出连接点的信息
        System.out.println("前置通知，" + joinPoint);
    }

    //@4：定义了一个异常通知，这个通知对刚刚上面我们定义的切入点中的所有方法有效
    @AfterThrowing(value = "pointcut1()", throwing = "e")
    public void afterThrowing(JoinPoint joinPoint, Exception e) {
        //发生异常之后输出异常信息
        System.out.println(joinPoint + ",发生异常：" + e.getMessage());
    }

}
```
@1：类上使用`@Aspect`标注<br />@2：通过`@Pointcut`注解标注在方法上面，用来定义切入点<br />@3：使用`@Before`标注在方法上面，定义了一个前置通知，通过value引用了上面已经定义的切入点，表示这个通知会对Service1中的所有方法生效，在通知中可以通过这个`类名.方法名()`引用`@Pointcut`定义的切入点，表示这个通知对这些切入点有效，若`@Before`和`@Pointcut`在一个类的时候，直接通过`方法名()`引用当前类中定义的切入点<br />@4：这个使用`@AfterThrowing`定义了一个异常通知，也是对通过value引用了上面已经定义的切入点，表示这个通知会对Service1中的所有方法生效，若Service1中的方法抛出了Exception类型的异常，都会回调`afterThrowing`方法。<br />来个测试类
```java
package com.javacode2018.aop.demo9;

import com.javacode2018.aop.demo9.test1.Aspect1;
import com.javacode2018.aop.demo9.test1.Service1;
import org.junit.Test;
import org.springframework.aop.aspectj.annotation.AspectJProxyFactory;

public class AopTest9 {
    @Test
    public void test1() {
        try {
            //对应目标对象
            Service1 target = new Service1();
            //创建AspectJProxyFactory对象
            AspectJProxyFactory proxyFactory = new AspectJProxyFactory();
            //设置被代理的目标对象
            proxyFactory.setTarget(target);
            //设置标注了@Aspect注解的类
            proxyFactory.addAspect(Aspect1.class);
            //生成代理对象
            Service1 proxy = proxyFactory.getProxy();
            //使用代理对象
            proxy.m1();
            proxy.m2();
        } catch (Exception e) {
        }
    }
}
```
运行输出
```
前置通知，execution(void com.javacode2018.aop.demo9.test1.Service1.m1())
我是 m1 方法
前置通知，execution(void com.javacode2018.aop.demo9.test1.Service1.m2())
execution(void com.javacode2018.aop.demo9.test1.Service1.m2()),发生异常：/ by zero
```
使用是不是特方便。
<a name="MX8fw"></a>
### `AspectJProxyFactory`原理
`@Aspect`标注的类上，这个类中，可以通过通过`@Pointcut`来定义切入点，可以通过`@Before`、`@Around`、`@After`、`@AfterRunning`、`@AfterThrowing`标注在方法上来定义通知，定义好了之后，将`@Aspect`标注的这个类交给`AspectJProxyFactory`来解析生成`Advisor`链，进而结合目标对象一起来生成代理对象，大家可以去看一下源码，比较简单，这里就不多解释了。<br />本文的重点在`@Aspect`标注的类上，`@Aspect`中有2个关键点比较重要

   - `@Pointcut`：标注在方法上，用来定义切入点，有11种用法，本文主要讲解这11种用法。
   - `@Aspect`类中定义通知：可以通过`@Before`、`@Around`、`@After`、`@AfterRunning`、`@AfterThrowing`标注在方法上来定义通知，这个下一篇介绍。
<a name="Nu1vk"></a>
## @Pointcut的12种用法
<a name="G1xfT"></a>
### 作用
用来标注在方法上来定义切入点。
<a name="B4506"></a>
### 定义
格式：@ 注解(value=“表达标签 (表达式格式)”)<br />如：
```
@Pointcut("execution(* com.javacode2018.aop.demo9.test1.Service1.*(..))")
```
<a name="vBu90"></a>
### 表达式标签（10种）

- `execution`：用于匹配方法执行的连接点
- `within`：用于匹配指定类型内的方法执行
- `this`：用于匹配当前AOP代理对象类型的执行方法；注意是AOP代理对象的类型匹配，这样就可能包括引入接口也类型匹配
- `target`：用于匹配当前目标对象类型的执行方法；注意是目标对象的类型匹配，这样就不包括引入接口也类型匹配
- `args`：用于匹配当前执行的方法传入的参数为指定类型的执行方法
- `@within`：用于匹配所以持有指定注解类型内的方法
- `@target`：用于匹配当前目标对象类型的执行方法，其中目标对象持有指定的注解
- `@args`：用于匹配当前执行的方法传入的参数持有指定注解的执行
- `@annotation`：用于匹配当前执行方法持有指定注解的方法
- bean：Spring AOP扩展的，AspectJ没有对于指示符，用于匹配特定名称的Bean对象的执行方法

**10种标签组成了12种用法**
<a name="rProE"></a>
### 1、execution
使用`execution(方法表达式)`匹配方法执行。
<a name="NkbD6"></a>
#### execution格式
```
execution(modifiers-pattern? ret-type-pattern declaring-type-pattern? name-pattern(param-pattern) throws-pattern?)
```

- 其中带 ?号的 modifiers-pattern?，declaring-type-pattern?，hrows-pattern?是可选项
- ret-type-pattern,name-pattern, parameters-pattern是必选项
- modifier-pattern? 修饰符匹配，如public 表示匹配公有方法
- ret-type-pattern 返回值匹配，* 表示任何返回值，全路径的类名等
- declaring-type-pattern? 类路径匹配
- name-pattern 方法名匹配，* 代表所有，set*，代表以set开头的所有方法
- (param-pattern) 参数匹配，指定方法参数(声明的类型)，(..)代表所有参数，(*,String)代表第一个参数为任何值,第二个为String类型，(..,String)代表最后一个参数是String类型
- throws-pattern? 异常类型匹配
<a name="yJHgm"></a>
#### 举例说明
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683334967864-3e340eb0-8d9b-4c73-9ab9-f21ae2d93c15.png#averageHue=%23f6f5f3&clientId=uaaf963a3-61d3-4&from=paste&id=u481d7f69&originHeight=354&originWidth=975&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u71e5eb3e-1a25-4f83-a427-b7d93a515e8&title=)
<a name="HnKO6"></a>
#### 类型匹配语法
很多地方会按照类型的匹配，先来说一下类型匹配的语法。<br />首先来了解下AspectJ类型匹配的通配符：

- `*****`：匹配任何数量字符
- `**..**`：匹配任何数量字符的重复，如在类型模式中匹配任何数量子包；而在方法参数模式中匹配任何数量参数（0个或者多个参数）
- `**+**`**：**匹配指定类型及其子类型；仅能作为后缀放在类型模式后边

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683334967872-c68a96bb-28b3-4b6d-b1da-79f15de79812.png#averageHue=%23f7f6f4&clientId=uaaf963a3-61d3-4&from=paste&id=u1d374e34&originHeight=263&originWidth=975&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=udd8f50de-b987-461a-9938-32d125e1fee&title=)
<a name="z9Cty"></a>
### 2、within
<a name="j0759"></a>
#### 用法
`within(类型表达式)`：目标对象target的类型是否和within中指定的类型匹配<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683334967842-b89675a8-ae8a-4a3a-b798-fa84d3e366da.png#averageHue=%23f7f5f3&clientId=uaaf963a3-61d3-4&from=paste&id=u0eca4ac3&originHeight=160&originWidth=973&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u020c7cc5-5a7b-417e-a625-8ed37bcb25e&title=)
<a name="zFZQu"></a>
#### 匹配原则
```
target.getClass().equals(within表达式中指定的类型)
```
<a name="LpOel"></a>
#### 案例
有2个类，父子关系<br />父类C1
```java
package com.javacode2018.aop.demo9.test2;

public class C1 {
    public void m1() {
        System.out.println("我是m1");
    }

    public void m2() {
        System.out.println("我是m2");
    }
}
```
子类C2
```java
package com.javacode2018.aop.demo9.test2;

public class C2 extends C1 {
    @Override
    public void m2() {
        super.m2();
    }

    public void m3() {
        System.out.println("我是m3");
    }
}
```
来个Aspect类
```java
package com.javacode2018.aop.demo9.test2;

import org.aspectj.lang.JoinPoint;
import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Before;
import org.aspectj.lang.annotation.Pointcut;

@Aspect
public class AspectTest2 {

    @Pointcut("within(C1)") //@1
    public void pc() {
    }

    @Before("pc()") //@2
    public void beforeAdvice(JoinPoint joinpoint) {
        System.out.println(joinpoint);
    }

}
```
注意@1匹配的类型是C1，也就是说被代理的对象的类型必须是C1类型的才行，需要和C1完全匹配<br />下面对C2创建代理
```java
@Test
public void test2(){
    C2 target = new C2();
    AspectJProxyFactory proxyFactory = new AspectJProxyFactory();
    proxyFactory.setTarget(target);
    proxyFactory.addAspect(AspectTest2.class);

    C2 proxy = proxyFactory.getProxy();
    proxy.m1();
    proxy.m2();
    proxy.m3();
}
```
运行输出
```
我是m1
我是m2
我是m3
```
原因是目标对象是C2类型的，C2虽然是C1的子类，但是`within`中表达式指定的是要求类型必须是C1类型的才匹配。<br />如果将within表达式修改为下面任意一种就可以匹配了
```java
@Pointcut("within(C1+)") 
@Pointcut("within(C2)")
```
再次运行输出
```
execution(void com.javacode2018.aop.demo9.test2.C1.m1())
我是m1
execution(void com.javacode2018.aop.demo9.test2.C2.m2())
我是m2
execution(void com.javacode2018.aop.demo9.test2.C2.m3())
我是m3
```
<a name="AUUFa"></a>
### 3、this
<a name="v5dph"></a>
#### 用法
this(类型全限定名)：通过aop创建的代理对象的类型是否和this中指定的类型匹配；注意判断的目标是代理对象；this中使用的表达式必须是类型全限定名，不支持通配符。
<a name="jDKHj"></a>
#### 匹配原则
```
如:this(x)，则代理对象proxy满足下面条件时会匹配
x.getClass().isAssignableFrom(proxy.getClass());
```
<a name="Ld4sR"></a>
#### 案例
来个接口
```java
package com.javacode2018.aop.demo9.test3;

public interface I1 {
    void m1();
}
```
来个实现类
```java
package com.javacode2018.aop.demo9.test3;

public class Service3 implements I1 {

    @Override
    public void m1() {
        System.out.println("我是m1");
    }

}
```
来个@Aspect类
```java
package com.javacode2018.aop.demo9.test3;

import org.aspectj.lang.JoinPoint;
import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Before;
import org.aspectj.lang.annotation.Pointcut;

@Aspect
public class AspectTest3 {

    //@1：匹配proxy是Service3类型的所有方法
    @Pointcut("this(Service3)")
    public void pc() {
    }

    @Before("pc()")
    public void beforeAdvice(JoinPoint joinpoint) {
        System.out.println(joinpoint);
    }

}
```
测试代码
```java
@Test
public void test3() {
    Service3 target = new Service3();
    AspectJProxyFactory proxyFactory = new AspectJProxyFactory();
    proxyFactory.setTarget(target);
    //获取目标对象上的接口列表
    Class<?>[] allInterfaces = ClassUtils.getAllInterfaces(target);
    //设置需要代理的接口
    proxyFactory.setInterfaces(allInterfaces);
    proxyFactory.addAspect(AspectTest3.class);
    //获取代理对象
    Object proxy = proxyFactory.getProxy();
    //调用代理对象的方法
    ((I1) proxy).m1();

    System.out.println("proxy是否是jdk动态代理对象：" + AopUtils.isJdkDynamicProxy(proxy));
    System.out.println("proxy是否是cglib代理对象：" + AopUtils.isCglibProxy(proxy));
    //判断代理对象是否是Service3类型的
    System.out.println(Service3.class.isAssignableFrom(proxy.getClass()));
}
```
运行输出
```
我是m1
proxy是否是jdk动态代理对象：true
proxy是否是cglib代理对象：false
false
```
从输出中可以看出m1方法没有被增强，原因：this表达式要求代理对象必须是Service3类型的，输出中可以看出代理对象并不是Service3类型的，此处代理对象proxy是使用jdk动态代理生成的。<br />可以将代码调整一下，使用cglib来创建代理
```java
proxyFactory.setProxyTargetClass(true);
```
再次运行，会发现m2被拦截了，结果如下
```
execution(void com.javacode2018.aop.demo9.test3.Service3.m1())
我是m1
proxy是否是jdk动态代理对象：false
proxy是否是cglib代理对象：true
true
```
<a name="C0UlQ"></a>
### 4、target
<a name="RIKzb"></a>
#### 用法
target(类型全限定名)：判断目标对象的类型是否和指定的类型匹配；注意判断的是目标对象的类型；表达式必须是类型全限定名，不支持通配符。
<a name="h2LKo"></a>
#### 匹配原则
```
如:target(x)，则目标对象target满足下面条件时会匹配
x.getClass().isAssignableFrom(target.getClass());
```
<a name="hQlgD"></a>
#### 案例
```java
package com.javacode2018.aop.demo9.test4;

import org.aspectj.lang.JoinPoint;
import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Before;
import org.aspectj.lang.annotation.Pointcut;

@Aspect
public class AspectTest4 {

    //@1：目标类型必须是Service3类型的
    @Pointcut("target(com.javacode2018.aop.demo9.test3.Service3)")
    public void pc() {
    }

    @Before("pc()")
    public void beforeAdvice(JoinPoint joinpoint) {
        System.out.println(joinpoint);
    }

}
```
测试代码
```java
@Test
public void test4() {
    Service3 target = new Service3();
    AspectJProxyFactory proxyFactory = new AspectJProxyFactory();
    proxyFactory.setProxyTargetClass(true);
    proxyFactory.setTarget(target);
    proxyFactory.addAspect(AspectTest4.class);
    //获取代理对象
    Object proxy = proxyFactory.getProxy();
    //调用代理对象的方法
    ((I1) proxy).m1();
    //判断target对象是否是Service3类型的
    System.out.println(Service3.class.isAssignableFrom(target.getClass()));
}
```
运行输出
```
execution(void com.javacode2018.aop.demo9.test3.Service3.m1())
我是m1
true
```
<a name="H8J5r"></a>
#### `within`、`this`、`target`对比
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683334967843-04338b05-0945-491f-bf9a-e5d90db213be.png#averageHue=%23f7f5f4&clientId=uaaf963a3-61d3-4&from=paste&id=u3e6c92b2&originHeight=163&originWidth=974&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7bdca1e8-adfc-4841-89b6-eb89a4d1f00&title=)
<a name="RK7x2"></a>
### 5、args
<a name="BIU8y"></a>
#### 用法
`args(参数类型列表)`匹配当前执行的方法传入的参数是否为args中指定的类型；注意是匹配传入的参数类型，不是匹配方法签名的参数类型；参数类型列表中的参数必须是类型全限定名，不支持通配符；args属于动态切入点，也就是执行方法的时候进行判断的，这种切入点开销非常大，非特殊情况最好不要使用。
<a name="rxHyt"></a>
#### 举例说明
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683334968624-126e19a4-8010-405b-a50c-58a17a15e411.png#averageHue=%23f8f7f6&clientId=uaaf963a3-61d3-4&from=paste&id=uac0614db&originHeight=165&originWidth=974&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u181d1739-2175-4edd-ad6f-8be5ed5a1e0&title=)
<a name="fkcCo"></a>
#### 案例
下面的m1方法参数是Object类型的。
```java
package com.javacode2018.aop.demo9.test5;

public class Service5 {
    public void m1(Object object) {
        System.out.println("我是m1方法,参数：" + object);
    }
}
```
Aspect类
```java
package com.javacode2018.aop.demo9.test5;

import org.aspectj.lang.JoinPoint;
import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Before;
import org.aspectj.lang.annotation.Pointcut;

import java.util.Arrays;
import java.util.stream.Collectors;

@Aspect
public class AspectTest5 {
    //@1：匹配只有1个参数其类型是String类型的
    @Pointcut("args(String)")
    public void pc() {
    }

    @Before("pc()")
    public void beforeAdvice(JoinPoint joinpoint) {
        System.out.println("请求参数：" + Arrays.stream(joinpoint.getArgs()).collect(Collectors.toList()));
    }
}
```
测试代码，调用2次m1方法，第一次传入一个String类型的，第二次传入一个int类型的，看看效果
```java
@Test
public void test5() {
    Service5 target = new Service5();
    AspectJProxyFactory proxyFactory = new AspectJProxyFactory();
    proxyFactory.setTarget(target);
    proxyFactory.addAspect(AspectTest5.class);
    Service5 proxy = proxyFactory.getProxy();
    proxy.m1("路人");
    proxy.m1(100);
}
```
运行输出
```
请求参数：[路人]
我是m1方法,参数：路人
我是m1方法,参数：100
```
输出中可以看出，m1第一次调用被增强了，第二次没有被增强。<br />**args会在调用的过程中对参数实际的类型进行匹配，比较耗时，慎用。**
<a name="z6Tfy"></a>
### 6、`@within`
<a name="eoAhW"></a>
#### 用法
`@within(注解类型)`：匹配指定的注解内定义的方法。
<a name="dEgtG"></a>
#### 匹配规则
调用目标方法的时候，通过java中`Method.getDeclaringClass()`获取当前的方法是哪个类中定义的，然后会看这个类上是否有指定的注解。
```
被调用的目标方法Method对象.getDeclaringClass().getAnnotation(within中指定的注解类型) != null
```
来看3个案例。
<a name="CgLSO"></a>
#### 案例1
**目标对象上有**`**@within**`**中指定的注解，这种情况时，目标对象的所有方法都会被拦截。**
<a name="MzLQ9"></a>
##### 来个注解
```java
package com.javacode2018.aop.demo9.test9;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.TYPE)
public @interface Ann9 {
}
```
<a name="psfYT"></a>
##### 来个目标类，用@Ann9标注
```java
package com.javacode2018.aop.demo9.test9;

@Ann9
public class S9 {
    public void m1() {
        System.out.println("我是m1方法");
    }
}
```
<a name="wqyB9"></a>
##### 来个Aspect类
```java
package com.javacode2018.aop.demo9.test9;

import org.aspectj.lang.JoinPoint;
import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Before;
import org.aspectj.lang.annotation.Pointcut;

@Aspect
public class AspectTest9 {
    /**
     * 定义目标方法的类上有Ann9注解
     */
    @Pointcut("@within(Ann9)")
    public void pc() {
    }

    @Before("pc()")
    public void beforeAdvice(JoinPoint joinPoint) {
        System.out.println(joinPoint);
    }
}
```
<a name="gMTXW"></a>
##### 测试代码
```java
@Test
public void test9() {
    S9 target = new S9();
    AspectJProxyFactory proxyFactory = new AspectJProxyFactory();
    proxyFactory.setTarget(target);
    proxyFactory.addAspect(AspectTest9.class);
    S9 proxy = proxyFactory.getProxy();
    proxy.m1();
}
```
m1方法在类S9中定义的，S9上面有Ann9注解，所以匹配成功
<a name="oielS"></a>
##### 运行输出
```
execution(void com.javacode2018.aop.demo9.test9.S9.m1())
我是m1方法
```
<a name="K6pTc"></a>
#### 案例2
**定义注解时未使用**`**@Inherited**`**，说明子类无法继承父类上的注解**，这个案例中将定义一个这样的注解，将注解放在目标类的父类上，来看一下效果。
<a name="sZ88O"></a>
##### 定义注解Ann10
```java
package com.javacode2018.aop.demo9.test10;

import java.lang.annotation.*;

@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.TYPE)
@Inherited
public @interface Ann10 {
}
```
<a name="nkmJa"></a>
##### 来2个父子类
注意：<br />S10Parent为父类，并且使用了Anno10注解，内部定义了2个方法大家注意一下<br />而S10位代理的目标类，继承了S10Parent，内部重写了父类的m2方法，并且又新增了一个m3方法
```java
package com.javacode2018.aop.demo9.test10;

@Ann10
class S10Parent {

    public void m1() {
        System.out.println("我是S10Parent.m1()方法");
    }

    public void m2() {
        System.out.println("我是S10Parent.m2()方法");
    }
}

public class S10 extends S10Parent {

    @Override
    public void m2() {
        System.out.println("我是S10.m2()方法");
    }

    public void m3() {
        System.out.println("我是S10.m3()方法");
    }
}
```
<a name="YpbYe"></a>
##### 来个`Aspect`类
```java
package com.javacode2018.aop.demo9.test10;

import org.aspectj.lang.JoinPoint;
import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Before;
import org.aspectj.lang.annotation.Pointcut;

@Aspect
public class AspectTest10 {
    //匹配目标方法声明的类上有@Anno10注解
    @Pointcut("@within(com.javacode2018.aop.demo9.test10.Ann10)")
    public void pc() {
    }

    @Before("pc()")
    public void beforeAdvice(JoinPoint joinPoint) {
        System.out.println(joinPoint);
    }
}
```
<a name="pwID6"></a>
##### 测试用例
S10为目标类，依次执行代理对象的m1、m2、m3方法，最终会调用目标类`target`中对应的方法。
```java
@Test
public void test10() {
    S10 target = new S10();
    AspectJProxyFactory proxyFactory = new AspectJProxyFactory();
    proxyFactory.setTarget(target);
    proxyFactory.addAspect(AspectTest10.class);
    S10 proxy = proxyFactory.getProxy();
    proxy.m1();
    proxy.m2();
    proxy.m3();
}
```
<a name="aJfTy"></a>
##### 运行输出
```
execution(void com.javacode2018.aop.demo9.test10.S10Parent.m1())
我是S10Parent.m1()方法
我是S10.m2()方法
我是S10.m3()方法
```
<a name="XJLs1"></a>
##### 分析结果
从输出中可以看出，只有m1方法被拦截了，其他2个方法没有被拦截。<br />确实是这样的，m1方法的是由S10Parent定义的，这个类上面有Ann10注解。<br />而m2方法虽然也在S10Parent中定义了，但是这个方法被子类S10重写了，所以调用目标对象中的m2方法的时候，此时发现m2方法是由S10定义的，而`S10.class.getAnnotation(Ann10.class)`为空，所以这个方法不会被拦截。<br />同样m3方法也是S10中定义的，也不会被拦截。
<a name="oHoqB"></a>
#### 案例3
对案例2进行改造，在注解的定义上面加上`@Inherited`，此时子类可以继承父类的注解，此时3个方法都会被拦截了。<br />下面上代码，下面代码为案例2代码的一个拷贝，不同地方只是注解的定义上多了`@Inherited`
<a name="BPeO7"></a>
##### 定义注解Ann11
```java
import java.lang.annotation.*;

@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.TYPE)
@Inherited
public @interface Ann11 {
}
```
<a name="IQrHI"></a>
##### 2个父子类
```java
package com.javacode2018.aop.demo9.test11;

@Ann11
class S11Parent {

    public void m1() {
        System.out.println("我是S11Parent.m1()方法");
    }

    public void m2() {
        System.out.println("我是S11Parent.m2()方法");
    }
}

public class S11 extends S11Parent {

    @Override
    public void m2() {
        System.out.println("我是S11.m2()方法");
    }

    public void m3() {
        System.out.println("我是S11.m3()方法");
    }
}
```
<a name="NWXpB"></a>
##### Aspect类
```java
package com.javacode2018.aop.demo9.test11;

import org.aspectj.lang.JoinPoint;
import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Before;
import org.aspectj.lang.annotation.Pointcut;

@Aspect
public class AspectTest11 {

    @Pointcut("@within(com.javacode2018.aop.demo9.test11.Ann11)")
    public void pc() {
    }

    @Before("pc()")
    public void beforeAdvice(JoinPoint joinPoint) {
        System.out.println(joinPoint);
    }
}
```
<a name="nWYeK"></a>
##### 测试用例
```java
@Test
public void test11() {
    S11 target = new S11();
    AspectJProxyFactory proxyFactory = new AspectJProxyFactory();
    proxyFactory.setTarget(target);
    proxyFactory.addAspect(AspectTest11.class);
    S11 proxy = proxyFactory.getProxy();
    proxy.m1();
    proxy.m2();
    proxy.m3();
}
```
<a name="EHFrr"></a>
##### 运行输出
```
execution(void com.javacode2018.aop.demo9.test11.S11Parent.m1())
我是S11Parent.m1()方法
execution(void com.javacode2018.aop.demo9.test11.S11.m2())
我是S11.m2()方法
execution(void com.javacode2018.aop.demo9.test11.S11.m3())
我是S11.m3()方法
```
这次3个方法都被拦截了。
<a name="vCxKK"></a>
### 7、`@target`
<a name="k7isw"></a>
#### 用法
`@target(注解类型)`：判断目标对象target类型上是否有指定的注解；@target中注解类型也必须是全限定类型名。
<a name="GmeRd"></a>
#### 匹配规则
```
target.class.getAnnotation(指定的注解类型) != null
```
2种情况可以匹配

- 注解直接标注在目标类上
- 注解标注在父类上，但是注解必须是可以继承的，即定义注解的时候，需要使用`@Inherited`标注
<a name="sBKjJ"></a>
#### 案例1
**注解直接标注在目标类上，这种情况目标类会被匹配到。**
<a name="JPunP"></a>
##### 自定义一个注解`Ann6`
```java
package com.javacode2018.aop.demo9.test6;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.TYPE)
public @interface Ann6 {
}
```
<a name="nnoHI"></a>
##### 目标类`S6`上直接使用`@Ann1`
```java
package com.javacode2018.aop.demo9.test6;

@Ann6
public class S6 {
    public void m1() {
        System.out.println("我是m1");
    }
}
```
<a name="FJCHY"></a>
##### 来个`Aspect`类
```java
package com.javacode2018.aop.demo9.test6;

import org.aspectj.lang.JoinPoint;
import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Before;
import org.aspectj.lang.annotation.Pointcut;

@Aspect
public class AspectTest6 {
    //@1：目标类上有@Ann1注解
    @Pointcut("@target(Ann1)")
    public void pc() {
    }

    @Before("pc()")
    public void beforeAdvice(JoinPoint joinPoint) {
        System.out.println(joinPoint);
    }
}
```
<a name="cyTw8"></a>
##### 测试代码
```java
@Test
public void test6() {
    S6 target = new S6();
    AspectJProxyFactory proxyFactory = new AspectJProxyFactory();
    proxyFactory.setTarget(target);
    proxyFactory.addAspect(AspectTest6.class);
    S6 proxy = proxyFactory.getProxy();
    proxy.m1();
    System.out.println("目标类上是否有 @Ann6 注解：" + (target.getClass().getAnnotation(Ann6.class) != null));
}
```
<a name="arC06"></a>
##### 运行输出
```
execution(void com.javacode2018.aop.demo9.test6.S6.m1())
我是m1
目标类上是否有 @Ann6 注解：true
```
<a name="jlNEF"></a>
#### 案例2
**注解标注在父类上，注解上没有**`**@Inherited**`**，这种情况下，目标类无法匹配到，下面看代码**
<a name="Ogm0o"></a>
##### 注解Ann7
```java
package com.javacode2018.aop.demo9.test7;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.TYPE)
public @interface Ann7 {
}
```
<a name="LLFq6"></a>
##### 来2个父子类，父类上有`@Ann7`，之类`S7`为目标类
```java
package com.javacode2018.aop.demo9.test7;

import java.lang.annotation.Target;

@Ann7
class S7Parent {
}

public class S7 extends S7Parent {
    public void m1() {
        System.out.println("我是m1");
    }

    public static void main(String[] args) {
        System.out.println(S7.class.getAnnotation(Target.class));
    }
}
```
<a name="RwKy3"></a>
##### 来个`Aspect`类
```java
package com.javacode2018.aop.demo9.test7;

import org.aspectj.lang.JoinPoint;
import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Before;
import org.aspectj.lang.annotation.Pointcut;

@Aspect
public class AspectTest7 {
    /**
     * 匹配目标类上有Ann7注解
     */
    @Pointcut("@target(com.javacode2018.aop.demo9.test7.Ann7)")
    public void pc() {
    }

    @Before("pc()")
    public void beforeAdvice(JoinPoint joinPoint) {
        System.out.println(joinPoint);
    }
}
```
<a name="qAiDZ"></a>
##### 测试代码
```java
@Test
public void test7() {
    S7 target = new S7();
    AspectJProxyFactory proxyFactory = new AspectJProxyFactory();
    proxyFactory.setTarget(target);
    proxyFactory.addAspect(AspectTest7.class);
    S7 proxy = proxyFactory.getProxy();
    proxy.m1();
    System.out.println("目标类上是否有 @Ann7 注解：" + (target.getClass().getAnnotation(Ann7.class) != null));
}
```
<a name="NmHDg"></a>
##### 运行输出
```
我是m1
目标类上是否有 @Ann7 注解：false
```
<a name="S2pJU"></a>
##### 分析结果
`@Ann7`标注在了父类上，但是`@Ann7`定义的时候没有使用`@Inherited`，说明之类无法继承父类上面的注解，所以上面的目标类没有被拦截，下面将`@Ann7`的定义改一下，加上`@Inherited`
```java
package com.javacode2018.aop.demo9.test7;

import java.lang.annotation.*;

@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.TYPE)
@Inherited
public @interface Ann7 {
}
```
<a name="PShPU"></a>
##### 再次运行输出
```
execution(void com.javacode2018.aop.demo9.test7.S7.m1())
我是m1
目标类上是否有 @Ann7 注解：true
```
此时目标对象被拦截了。
<a name="fzto0"></a>
### 8、`@args`
<a name="phcN0"></a>
#### 用法
`@args(注解类型)`：方法参数所属的类上有指定的注解；注意不是参数上有指定的注解，而是参数类型的类上有指定的注解。
<a name="Cv2OE"></a>
#### 案例1
```java
@Pointcut("@args(Ann8)")：匹配方法只有一个参数，并且参数所属的类上有Ann8注解
```
可以匹配下面的代码，m1方法的第一个参数类型是Car类型，Car类型上有注解Ann8
```java
@Ann8
class Car {
}

public void m1(Car car) {
    System.out.println("我是m1");
}
```
<a name="Lm0Jl"></a>
#### 案例2
```java
@Pointcut("@args(*,Ann8)")：匹配方法只有2个参数，且第2个参数所属的类型上有Ann8注解
```
可以匹配下面代码
```java
@Ann8
class Car {
}

public void m1(String name,Car car) {
    System.out.println("我是m1");
}
```
<a name="xreX7"></a>
#### 案例3
```java
@Pointcut("@args(..,com.javacode2018.aop.demo9.test8.Ann8)")：匹配参数数量大于等于1，且最后一个参数所属的类型上有Ann8注解
@Pointcut("@args(*,com.javacode2018.aop.demo9.test8.Ann8,..)")：匹配参数数量大于等于2，且第2个参数所属的类型上有Ann8注解
@Pointcut("@args(..,com.javacode2018.aop.demo9.test8.Ann8,*)")：匹配参数数量大于等于2，且倒数第2个参数所属的类型上有Ann8注解
```
这个案例代码，大家自己写一下，体验一下。
<a name="mCmpn"></a>
### 9、`@annotation`
<a name="hXNRf"></a>
#### 用法
`@annotation(注解类型)`：匹配被调用的方法上有指定的注解。
<a name="qtr1l"></a>
#### 案例
<a name="eauJQ"></a>
##### 定义一个注解，可以用在方法上
```java
package com.javacode2018.aop.demo9.test12;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.METHOD)
public @interface Ann12 {
}
```
<a name="tdWZp"></a>
##### 定义2个类
S12Parent为父类，内部定义了2个方法，2个方法上都有@Ann12注解<br />S12是代理的目标类，也是S12Parent的子类，内部重写了m2方法，重写之后m2方法上并没有@Ann12注解，S12内部还定义2个方法m3和m4，而m3上面有注解@Ann12
```java
package com.javacode2018.aop.demo9.test12;

class S12Parent {

    @Ann12
    public void m1() {
        System.out.println("我是S12Parent.m1()方法");
    }

    @Ann12
    public void m2() {
        System.out.println("我是S12Parent.m2()方法");
    }
}

public class S12 extends S12Parent {

    @Override
    public void m2() {
        System.out.println("我是S12.m2()方法");
    }

    @Ann12
    public void m3() {
        System.out.println("我是S12.m3()方法");
    }

    public void m4() {
        System.out.println("我是S12.m4()方法");
    }
}
```
<a name="Gl0aR"></a>
##### 来个`Aspect`类
当被调用的目标方法上有`@Ann12`注解的时，会被`beforeAdvice`处理。
```java
package com.javacode2018.aop.demo9.test12;

import org.aspectj.lang.JoinPoint;
import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Before;
import org.aspectj.lang.annotation.Pointcut;

@Aspect
public class AspectTest12 {

    @Pointcut("@annotation(com.javacode2018.aop.demo9.test12.Ann12)")
    public void pc() {
    }

    @Before("pc()")
    public void beforeAdvice(JoinPoint joinPoint) {
        System.out.println(joinPoint);
    }
}
```
<a name="DuDf5"></a>
##### 测试用例
S12作为目标对象，创建代理，然后分别调用4个方法
```java
@Test
public void test12() {
    S12 target = new S12();
    AspectJProxyFactory proxyFactory = new AspectJProxyFactory();
    proxyFactory.setTarget(target);
    proxyFactory.addAspect(AspectTest12.class);
    S12 proxy = proxyFactory.getProxy();
    proxy.m1();
    proxy.m2();
    proxy.m3();
    proxy.m4();
}
```
<a name="YHgp9"></a>
##### 运行输出
```
execution(void com.javacode2018.aop.demo9.test12.S12Parent.m1())
我是S12Parent.m1()方法
我是S12.m2()方法
execution(void com.javacode2018.aop.demo9.test12.S12.m3())
我是S12.m3()方法
我是S12.m4()方法
```
<a name="TTr91"></a>
##### 分析结果
m1方法位于S12Parent中，上面有`@Ann12`注解，被连接了，m3方法上有`@Ann12`注解，被拦截了，而m4上没有`@Ann12`注解，没有被拦截，这3个方法的执行结果都很容易理解。<br />重点在于m2方法的执行结果，没有被拦截，m2方法虽然在S12Parent中定义的时候也有`@Ann12`注解标注，但是这个方法被S1给重写了，在S1中定义的时候并没有`@Ann12`注解，代码中实际上调用的是S1中的m2方法，发现这个方法上并没有`@Ann12`注解，所以没有被拦截。
<a name="bpfTi"></a>
### 10、bean
<a name="DZPt6"></a>
#### 用法
bean(bean名称)：这个用在Spring环境中，匹配容器中指定名称的bean。
<a name="tYyKd"></a>
#### 案例
<a name="AdwTv"></a>
##### 来个类BeanService
```java
package com.javacode2018.aop.demo9.test13;

public class BeanService {
    private String beanName;

    public BeanService(String beanName) {
        this.beanName = beanName;
    }

    public void m1() {
        System.out.println(this.beanName);
    }
}
```
<a name="lXpjQ"></a>
##### 来个Aspect类
```java
package com.javacode2018.aop.demo9.test13;

import org.aspectj.lang.JoinPoint;
import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Before;
import org.aspectj.lang.annotation.Pointcut;
import org.springframework.stereotype.Component;

@Aspect
public class Aspect13 {
    //拦截spring容器中名称为beanService2的bean
    @Pointcut("bean(beanService2)")
    public void pc() {
    }

    @Before("pc()")
    public void beforeAdvice(JoinPoint joinPoint) {
        System.out.println(joinPoint);
    }
}
```
<a name="g5f9D"></a>
##### 来个Spring配置类
```java
package com.javacode2018.aop.demo9.test13;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.EnableAspectJAutoProxy;

@Configuration
@EnableAspectJAutoProxy // 这个可以启用通过AspectJ方式自动为符合条件的bean创建代理
public class MainConfig13 {

    //将Aspect13注册到spring容器
    @Bean
    public Aspect13 aspect13() {
        return new Aspect13();
    }

    @Bean
    public BeanService beanService1() {
        return new BeanService("beanService1");
    }

    @Bean
    public BeanService beanService2() {
        return new BeanService("beanService2");
    }
}
```
这个配置类中有个`@EnableAspectJAutoProxy`，这个注解大家可能比较陌生，这个属于aop中自动代理的范围，后面会有文章详细介绍这块，这里大家暂时先不用关注。
<a name="JTY9Z"></a>
##### 测试用例
下面启动Spring容器，加载配置类MainConfig13，然后分别获取beanService1和beanService2，调用他们的m1方法，看看效果
```java
@Test
public void test13() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig13.class);
    //从容器中获取beanService1
    BeanService beanService1 = context.getBean("beanService1", BeanService.class);
    beanService1.m1();
    //从容器中获取beanService2
    BeanService beanService2 = context.getBean("beanService2", BeanService.class);
    beanService2.m1();
}
```
<a name="ayQhp"></a>
##### 运行输出
```
beanService1
execution(void com.javacode2018.aop.demo9.test13.BeanService.m1())
beanService2
```
beanService2的m1方法被拦截了。
<a name="goi6Q"></a>
### 11、reference pointcut
表示引用其他命名切入点。<br />有时，可以将切入专门放在一个类中集中定义。<br />其他地方可以通过引用的方式引入其他类中定义的切入点。<br />语法如下：
```java
@Pointcut("完整包名类名.方法名称()")
```
若引用同一个类中定义切入点，包名和类名可以省略，直接通过方法就可以引用。<br />比如下面，可以将所有切入点定义在一个类中
```java
package com.javacode2018.aop.demo9.test14;

import org.aspectj.lang.annotation.Pointcut;

public class AspectPcDefine {
    @Pointcut("bean(bean1)")
    public void pc1() {
    }

    @Pointcut("bean(bean2)")
    public void pc2() {
    }
}
```
下面顶一个一个`Aspect`类，来引用上面的切入点
```java
package com.javacode2018.aop.demo9.test14;

import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Pointcut;

@Aspect
public class Aspect14 {

    @Pointcut("com.javacode2018.aop.demo9.test14.AspectPcDefine.pc1()")
    public void pointcut1() {
    }

    @Pointcut("com.javacode2018.aop.demo9.test14.AspectPcDefine.pc1() || com.javacode2018.aop.demo9.test14.AspectPcDefine.pc2()")
    public void pointcut2() {
    }

}
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683335050081-12dc8a42-8eb1-4baa-8faf-b52115026071.png#averageHue=%23fbf8f6&clientId=uaaf963a3-61d3-4&from=paste&id=u38b0cafa&originHeight=491&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4ea1916d-ce8b-4361-bebe-bfdf0b4be9b&title=)
<a name="i6PUH"></a>
### 12、组合型的pointcut
Pointcut定义时，还可以使用&&、||、!运算符。

- `&&`：多个匹配都需要满足
- `||`：多个匹配中只需满足一个
- `!`：匹配不满足的情况下
```java
@Pointcut("bean(bean1) || bean(bean2)") //匹配bean1或者bean2
@Pointcut("@target(Ann1) && @Annotation(Ann2)") //匹配目标类上有Ann1注解并且目标方法上有Ann2注解
@Pointcut("@target(Ann1) && !@target(Ann2)") // 匹配目标类上有Ann1注解但是没有Ann2注解
```
<a name="Mxtr4"></a>
## 总结
本文详解了`@Pointcut`的12种用法，案例一定要敲一遍，敲的过程中，会遇到问题，然后解决问题，才能够加深理解。
