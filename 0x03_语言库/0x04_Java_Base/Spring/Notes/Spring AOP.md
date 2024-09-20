Java Spring
<a name="DVyvT"></a>
# 基本知识
<a name="xyuex"></a>
## AOP
AOP(Aspect-Oriented Programming)，即 **面向切面编程** , 它与 OOP( Object-Oriented Programming，面向对象编程) 相辅相成，提供了与 OOP 不同的抽象软件结构的视角。 在 OOP 中，以类(class)作为基本单元，而 AOP 中的基本单元是 **Aspect(切面)**
<a name="21x1K"></a>
## 术语
<a name="Wli9j"></a>
### Aspect(切面)
`aspect` 由 `pointcount` 和 `advice` 组成，它既包含了横切逻辑的定义，也包括了连接点的定义。 Spring AOP就是负责实施切面的框架，它将切面所定义的横切逻辑织入到切面所指定的连接点中。 AOP的工作重心在于如何将增强织入目标对象的连接点上，这里包含两个工作:

1. 如何通过 pointcut 和 advice 定位到特定的 joinpoint 上
2. 如何在 advice 中编写切面代码。

**可以简单地认为，使用 **`**@Aspect**`** 注解的类就是切面。**
<a name="Qf9OO"></a>
### advice(增强)
由 aspect 添加到特定的 join point(即满足 point cut 规则的 join point) 的一段代码。许多 AOP框架，包括 Spring AOP，会将 advice 模拟为一个拦截器(interceptor)，并且在 join point 上维护多个 advice，进行层层拦截。例如 HTTP 鉴权的实现，可以为每个使用 RequestMapping 标注的方法织入 advice，当 HTTP 请求到来时，首先进入到 advice 代码中，在这里可以分析这个 HTTP 请求是否有相应的权限，如果有，则执行 Controller，如果没有，则抛出异常。这里的 advice 就扮演着鉴权拦截器的角色了。
<a name="YAv0f"></a>
### 连接点(join point)
> a point during the execution of a program, such as the execution of a method or the handling of an exception. In Spring AOP, a join point always represents a method execution.

程序运行中的一些时间点，例如一个方法的执行，或者是一个异常的处理。在 Spring AOP 中，join point 总是方法的执行点，即只有方法连接点。
<a name="rF7Mo"></a>
### 切点(point cut)
匹配 join point 的谓词(a predicate that matches join points)。Advice 是和特定的 point cut 关联的, 并且在 point cut 相匹配的 join point 中执行。在 Spring 中，所有的方法都可以认为是 joinpoint，但是并不希望在所有的方法上都添加 Advice，而 pointcut 的作用就是提供一组规则(使用 AspectJ pointcut expression language 来描述) 来匹配joinpoint，给满足规则的 joinpoint 添加 Advice。
<a name="i4O2l"></a>
### 关于join point 和 point cut 的区别
在 Spring AOP 中，所有的方法执行都是 join point。而 point cut 是一个描述信息，它修饰的是 join point，通过 point cut，就可以确定哪些 join point 可以被织入 Advice。因此 join point 和 point cut 本质上就是两个不同纬度上的东西。advice 是在 join point 上执行的，而 point cut 规定了哪些 join point 可以执行哪些 advice
<a name="qd2Ld"></a>
### introduction
为一个类型添加额外的方法或字段。Spring AOP 允许为 `目标对象` 引入新的接口(和对应的实现)。例如可以使用 introduction 来为一个 bean 实现 IsModified 接口，并以此来简化 caching 的实现。
<a name="NCEVa"></a>
### 目标对象(Target)
织入 advice 的目标对象。目标对象也被称为 advised object。因为 Spring AOP 使用运行时代理的方式来实现 aspect，因此 adviced object 总是一个代理对象(proxied object)注意，adviced object 指的不是原来的类，而是织入 advice 后所产生的代理类。
<a name="BegHY"></a>
### AOP proxy
一个类被 AOP 织入 advice，就会产生一个结果类，它是融合了原类和增强逻辑的代理类。 在 Spring AOP 中，一个 AOP 代理是一个 JDK 动态代理对象或 CGLIB 代理对象。
<a name="4Pkm4"></a>
### 织入(Weaving)
将 aspect 和其他对象连接起来，并创建 adviced object 的过程。根据不同的实现技术，AOP织入有三种方式:

- 编译器织入，这要求有特殊的Java编译器。
- 类装载期织入，这需要有特殊的类装载器。
- 动态代理织入，在运行期为目标类添加增强(Advice)生成子类的方式。Spring 采用动态代理织入，而AspectJ采用编译器织入和类装载期织入。
<a name="hG7Bz"></a>
## advice 的类型

- before advice，在 join point 前被执行的 advice。虽然 before advice 是在 join point 前被执行，但是它并不能够阻止 join point 的执行，除非发生了异常(即在 before advice 代码中，不能人为地决定是否继续执行 join point 中的代码)
- after return advice，在一个 join point 正常返回后执行的 advice
- after throwing advice，当一个 join point 抛出异常后执行的 advice
- after(final) advice, 无论一个 join point 是正常退出还是发生了异常，都会被执行的 advice。
- around advice，在 join point 前和 joint point 退出后都执行的 advice。这个是最常用的 advice。
<a name="eZUcv"></a>
## 关于 AOP Proxy
Spring AOP 默认使用标准的 JDK 动态代理(dynamic proxy)技术来实现 AOP 代理，通过它，可以为任意的接口实现代理。如果需要为一个类实现代理，那么可以使用 CGLIB 代理。当一个业务逻辑对象没有实现接口时，那么Spring AOP 就默认使用 CGLIB 来作为 AOP 代理了。即如果需要为一个方法织入 advice，但是这个方法不是一个接口所提供的方法，则此时 Spring AOP 会使用 CGLIB 来实现动态代理。鉴于此，Spring AOP 建议基于接口编程，对接口进行 AOP 而不是类。
<a name="Ry4pb"></a>
# `@AspectJ` 支持
`**@AspectJ**` 是一种使用 Java 注解来实现 AOP 的编码风格。`@AspectJ` 风格的 AOP 是 AspectJ Project 在 AspectJ 5 中引入的，并且 Spring 也支持`@AspectJ` 的 AOP 风格。
<a name="zQvOT"></a>
## 使用 `@AspectJ` 支持
`@AspectJ` 可以以 XML 的方式或以注解的方式来使能，并且不论以哪种方式使能`@ASpectJ`，都必须保证 aspectjweaver.jar 在 classpath 中。
<a name="wr7ye"></a>
### 使用 Java Configuration 方式使用`@AspectJ`
```java
@Configuration
@EnableAspectJAutoProxy
public class AppConfig {
}
```
<a name="TV06G"></a>
### 使用 XML 方式定义`@AspectJ`
```xml
<aop:aspectj-autoproxy/>
```
<a name="hWbku"></a>
## 定义 aspect(切面)
当使用注解 `**@Aspect**` 标注一个 Bean 后，那么 Spring 框架会自动收集这些 Bean，并添加到 Spring AOP 中，例如:
```java
@Component
@Aspect
public class MyTest {
}
```
注意：仅仅使用`@Aspect` 注解，并不能将一个 Java 对象转换为 Bean，因此还需要使用类似 `@Component` 之类的注解。<br />注意：如果一个 类被`@Aspect` 标注，则这个类就不能是其他 aspect 的 **advised object** 了，因为使用 `@Aspect` 后，这个类就会被排除在 auto-proxying 机制之外。
<a name="S0ikh"></a>
## 声明 pointcut
一个 pointcut 的声明由两部分组成:

- 一个方法签名，包括方法名和相关参数
- 一个 pointcut 表达式，用来指定哪些方法执行(即因此可以织入 advice)。

在`@AspectJ` 风格的 AOP 中，使用一个方法来描述 pointcut，即:
```java
@Pointcut("execution(* com.xys.service.UserService.*(..))") // 切点表达式
private void dataAccessOperation() {} // 切点前面
```
这个方法必须无返回值.这个方法本身就是 pointcut signature，pointcut 表达式使用`@Pointcut` 注解指定。上面简单地定义了一个 pointcut，这个 pointcut 所描述的是: 匹配所有在包 `**com.xys.service.UserService**` 下的所有方法的执行。
<a name="vcqCs"></a>
### 切点标志符(designator)
AspectJ5 的切点表达式由标志符(designator)和操作参数组成。如 `"execution( _greetTo(..))"_`_ 的切点表达式，**execution** 就是 标志符，而圆括号里的_ `greetTo(..)` 就是操作参数
<a name="fdrvW"></a>
#### `execution`
匹配 join point 的执行，例如 `"execution(* hello(..))"` 表示匹配所有目标类中的 `hello()` 方法。这个是最基本的 pointcut 标志符。
<a name="Wm5YB"></a>
#### `within`
匹配特定包下的所有 join point，例如 `within(com.xys.*)` 表示 com.xys 包中的所有连接点，即包中的所有类的所有方法。而 `within(com.xys.service.*Service)` 表示在 com.xys.service 包中所有以 Service 结尾的类的所有的连接点。
<a name="4b3V8"></a>
#### `this` 与 `target`
this 的作用是匹配一个 bean，这个 bean(Spring AOP proxy) 是一个给定类型的实例(instance of)。而 target 匹配的是一个目标对象(target object，即需要织入 advice 的原始的类)，此对象是一个给定类型的实例(instance of)。
<a name="L44eh"></a>
#### `bean`
匹配 bean 名字为指定值的 bean 下的所有方法，例如:
```java
bean(*Service) // 匹配名字后缀为 Service 的 bean 下的所有方法
bean(myService) // 匹配名字为 myService 的 bean 下的所有方法
```
<a name="NhW8n"></a>
#### args
匹配参数满足要求的的方法。例如:
```java
@Pointcut("within(com.xys.demo2.*)")
public void pointcut2() {
}
@Before(value = "pointcut2()  &&  args(name)")
public void doSomething(String name) {
    logger.info("---page: {}---", name);
}
@Service
public class NormalService {
    private Logger logger = LoggerFactory.getLogger(getClass());
    public void someMethod() {
        logger.info("---NormalService: someMethod invoked---");
    }
    public String test(String name) {
        logger.info("---NormalService: test invoked---");
        return "服务一切正常";
    }
}
```
当 NormalService.test 执行时，则 advice `doSomething` 就会执行，test 方法的参数 name 就会传递到 `doSomething` 中。<br />常用例子:
```java
// 匹配只有一个参数 name 的方法
@Before(value = "aspectMethod()  &&  args(name)")
public void doSomething(String name) {
}
// 匹配第一个参数为 name 的方法
@Before(value = "aspectMethod()  &&  args(name, ..)")
public void doSomething(String name) {
}
// 匹配第二个参数为 name 的方法
@Before(value = "aspectMethod()  &&  args(*, name, ..)")
public void doSomething(String name) {
}
```
<a name="6UnzM"></a>
#### `@annotation`
匹配由指定注解所标注的方法，例如:
```java
@Pointcut("@annotation(com.xys.demo1.AuthChecker)")
public void pointcut() {
}
```
则匹配由注解 `AuthChecker` 所标注的方法.
<a name="t4aUX"></a>
### 常见的切点表达式
<a name="FZaZR"></a>
#### 匹配方法签名
```java
// 匹配指定包中的所有的方法
execution(* com.xys.service.*(..))
// 匹配当前包中的指定类的所有方法
execution(* UserService.*(..))
// 匹配指定包中的所有 public 方法
execution(public * com.xys.service.*(..))
// 匹配指定包中的所有 public 方法, 并且返回值是 int 类型的方法
execution(public int com.xys.service.*(..))
// 匹配指定包中的所有 public 方法, 并且第一个参数是 String, 返回值是 int 类型的方法
execution(public int com.xys.service.*(String name, ..))
```
<a name="04ymI"></a>
#### 匹配类型签名
```java
// 匹配指定包中的所有的方法, 但不包括子包
within(com.xys.service.*)
// 匹配指定包中的所有的方法, 包括子包
within(com.xys.service..*)
// 匹配当前包中的指定类中的方法
within(UserService)
// 匹配一个接口的所有实现类中的实现的方法
within(UserDao+)
```
<a name="g3Pvi"></a>
#### 匹配 Bean 名字
```
// 匹配以指定名字结尾的 Bean 中的所有方法
bean(*Service)
```
<a name="by8vV"></a>
#### 切点表达式组合
```java
// 匹配以 Service 或 ServiceImpl 结尾的 bean
bean(*Service || *ServiceImpl)
// 匹配名字以 Service 结尾, 并且在包 com.xys.service 中的 bean
bean(*Service) && within(com.xys.service.*)
```
<a name="hztAz"></a>
## 声明 advice
advice 是和一个 pointcut 表达式关联在一起的，并且会在匹配的 join point 的方法执行的前/后/周围 运行。pointcut 表达式可以是简单的一个 pointcut 名字的引用，或者是完整的 pointcut 表达式。下面以几个简单的 advice 为例子，来看一下一个 advice 是如何声明的。
<a name="J1fOM"></a>
### Before advice
```java
@Component
@Aspect
public class BeforeAspectTest {
    // 定义一个 Pointcut, 使用 切点表达式函数 来描述对哪些 Join point 使用 advise.
    @Pointcut("execution(* com.xys.service.UserService.*(..))")
    public void dataAccessOperation() {
    }
}
@Component
@Aspect
public class AdviseDefine {
    // 定义 advise
    @Before("com.xys.aspect.PointcutDefine.dataAccessOperation()")
    public void doBeforeAccessCheck(JoinPoint joinPoint) {
        System.out.println("*****Before advise, method: " + joinPoint.getSignature().toShortString() + " *****");
    }
}
```
这里，`**@Before**` 引用了一个 pointcut，即 "`com.xys.aspect.PointcutDefine.dataAccessOperation()`" 是一个 pointcut 的名字。如果在 advice 在内置 pointcut，则可以:
```java
@Component
@Aspect
public class AdviseDefine {
    // 将 pointcut 和 advice 同时定义
    @Before("within(com.xys.service..*)")
    public void doAccessCheck(JoinPoint joinPoint) {
        System.out.println("*****doAccessCheck, Before advise, method: " + joinPoint.getSignature().toShortString() + " *****");
    }
}
```
<a name="OL8Ul"></a>
### around advice
around advice 比较特别，它可以在一个方法的之前之前和之后添加不同的操作，并且甚至可以决定何时，如何，是否调用匹配到的方法。
```java
@Component
@Aspect
public class AdviseDefine {
    // 定义 advise
    @Around("com.xys.aspect.PointcutDefine.dataAccessOperation()")
    public Object doAroundAccessCheck(ProceedingJoinPoint pjp) throws Throwable {
        StopWatch stopWatch = new StopWatch();
        stopWatch.start();
        // 开始
        Object retVal = pjp.proceed();
        stopWatch.stop();
        // 结束
        System.out.println("invoke method: " + pjp.getSignature().getName() + ", elapsed time: " + stopWatch.getTotalTimeMillis());
        return retVal;
    }
}
```
around advice 和前面的 before advice 差不多，只是把注解 `**@Before**` 改为了 `**@Around**` 了。
