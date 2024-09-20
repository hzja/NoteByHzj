JavaSpringBootAOP<br />都知道，AOP 底层是动态代理，而 Java 中的动态代理有两种实现方式：

- 基于 JDK 的动态代理
- 基于 Cglib 的动态代理

这两者最大的区别在于基于 JDK 的动态代理需要被代理的对象有接口，而基于 Cglib 的动态代理并不需要被代理对象有接口。<br />那么小伙伴们不禁要问，Spring 中的 AOP 是怎么实现的？是基于 JDK 的动态代理还是基于 Cglib 的动态代理？
<a name="IZzxL"></a>
## 1、Spring
先来说结论，Spring 中的动态代理，具体用哪种，分情况：

- 如果代理对象有接口，就用 JDK 动态代理，否则就是 Cglib 动态代理。
- 如果代理对象没有接口，那么就直接是 Cglib 动态代理。

来看看这段来自官方文档的说辞：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662096691614-ae22eec0-8802-4564-b3d2-f27ca0350013.png#clientId=u0b915948-c8ee-4&from=paste&id=ufc2ae960&originHeight=368&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ucdf97043-29d6-4a3e-9ee8-cbf49328623&title=)<br />可以看到，即使在最新版的 Spring 中，依然是如上策略不变。即能用 JDK 做动态代理就用 JDK，不能用 JDK 做动态代理就用 Cglib，即首选 JDK 做动态代理。
<a name="yXad0"></a>
## 2、Spring Boot
Spring Boot 和 Spring 一脉相承，那么在动态代理这个问题上是否也是相同的策略呢？抱歉，这个还真不一样。<br />Spring Boot 中对这个问题的处理，以 Spring Boot2.0 为节点，前后不一样。<br />在 Spring Boot2.0 之前，关于 Aop 的自动化配置代码是这样的（Spring Boot 1.5.22.RELEASE）：
```java
@Configuration
@ConditionalOnClass({ EnableAspectJAutoProxy.class, Aspect.class, Advice.class })
@ConditionalOnProperty(prefix = "spring.aop", name = "auto", havingValue = "true", matchIfMissing = true)
public class AopAutoConfiguration {

@Configuration
@EnableAspectJAutoProxy(proxyTargetClass = false)
@ConditionalOnProperty(prefix = "spring.aop", name = "proxy-target-class", havingValue = "false",
matchIfMissing = true)
public static class JdkDynamicAutoProxyConfiguration {

}

@Configuration
@EnableAspectJAutoProxy(proxyTargetClass = true)
@ConditionalOnProperty(prefix = "spring.aop", name = "proxy-target-class", havingValue = "true",
matchIfMissing = false)
public static class CglibAutoProxyConfiguration {

}

}
```
可以看到，这个自动化配置主要是在讨论 application.properties 配置文件中的 `spring.aop.proxy-target-class` 属性的值。<br />具体起作用的是 `@ConditionalOnProperty` 注解，关于这个注解中的几个属性，松哥也来稍微说下：

- prefix：配置文件的前缀。
- name：配置文件的名字，和 prefix 共同组成配置的 key。
- having：期待配置的值，如果实际的配置和 having 的值相同，则这个配置就会生效，否则不生效。
- matchIfMissing：如果开发者没有在 application.properties 中进行配置，那么这个配置类是否生效。

基于如上介绍，很容易看出：

- 如果开发者设置了 `spring.aop.proxy-target-class` 为 false，则使用 JDK 代理。
- 如果开发者设置了 `spring.aop.proxy-target-class` 为 true，则使用 Cglib 代理。
- 如果开发者一开始就没配置 `spring.aop.proxy-target-class` 属性，则使用 JDK 代理。

这是 Spring Boot 2.0 之前的情况。<br />再来看看 Spring Boot 2.0（含）之后的情况（Spring Boot 2.0.0.RELEASE）：
```java
@Configuration
@ConditionalOnClass({ EnableAspectJAutoProxy.class, Aspect.class, Advice.class,
  AnnotatedElement.class })
@ConditionalOnProperty(prefix = "spring.aop", name = "auto", havingValue = "true", matchIfMissing = true)
public class AopAutoConfiguration {

 @Configuration
 @EnableAspectJAutoProxy(proxyTargetClass = false)
 @ConditionalOnProperty(prefix = "spring.aop", name = "proxy-target-class", havingValue = "false", matchIfMissing = false)
 public static class JdkDynamicAutoProxyConfiguration {

 }

 @Configuration
 @EnableAspectJAutoProxy(proxyTargetClass = true)
 @ConditionalOnProperty(prefix = "spring.aop", name = "proxy-target-class", havingValue = "true", matchIfMissing = true)
 public static class CglibAutoProxyConfiguration {

 }

}
```
可以看到，大部分配置都是一样的，有一个地方不太相同，那就是 matchIfMissing 属性的值。**可以看到，从 Spring Boot2.0 开始，如果用户什么都没有配置，那么默认情况下使用的是 Cglib 代理。**
<a name="oOLf7"></a>
## 3、实践
最后写一个简单的例子验证一下想法。<br />首先创建一个 Spring Boot 项目（本案例使用最新版 Spring Boot，即默认使用 Cglib 代理），加入三个依赖即可，如下：
```xml
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-web</artifactId>
</dependency>
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter</artifactId>
</dependency>
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-aop</artifactId>
</dependency>
```
接下来创建一个 IUserService 接口，如下：
```java
public interface IUserService {
    void hello();
}
```
然后再来创建一个该接口的实现类：
```java
@Service
public class UserServiceImpl implements IUserService {
    @Override
    public void hello() {

    }
}
```
方法不用实现。<br />再来一个简单的切面：
```java
@EnableAspectJAutoProxy
@Aspect
@Component
public class LogAspect {
    @Before("execution(* org.javaboy.demo.UserServiceImpl.*(..))")
    public void before(JoinPoint jp) {
        System.out.println("jp.getSignature().getName() = " + jp.getSignature().getName());
    }
}
```
最后再来一个简单的测试方法，注入 IUserService 实例：
```java
@RestController
public class HelloController {
    @Autowired
    IUserService iUserService;
    @GetMapping("/hello")
    public void hello() {
        iUserService.hello();
    }
}
```
DBUEG 运行一下，就可以看到 IUserService 是通过 Cglib 来代理的。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662096691617-613cdded-65fb-4ab8-97a2-32a417e5739c.png#clientId=u0b915948-c8ee-4&from=paste&id=u8ac95ab9&originHeight=721&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2d6c3110-ca1a-4cff-881b-5dbe3f72d5b&title=)<br />如果想用 JDK 来代理，那么只需要在 application.properties 中添加如下配置即可：
```
spring.aop.proxy-target-class=false
```
添加完成后，重新 DEBUG，如下图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662096691614-569b5190-d8ef-4fc3-bec0-b56cb2035ae5.png#clientId=u0b915948-c8ee-4&from=paste&id=ub8b3f647&originHeight=617&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uda4a0c2d-6c9c-4876-8d04-27b37a3c9b1&title=)<br />可以看到，已经使用了 JDK 动态代理了。<br />如果用的是 Spring Boot 1.5.22.RELEASE 这个版本，那么即使不在 application.properties 中添加配置，默认也是 JDK 代理，这个就不测试了，可以自己来试试。
<a name="RqXGX"></a>
## 4、小结
总结一下：

1. Spring 中的 AOP，有接口就用 JDK 动态代理，没有接口就用 Cglib 动态代理。
2. Spring Boot 中的 AOP，2.0 之前和 Spring 一样；2.0 之后首选 Cglib 动态代理，如果用户想要使用 JDK 动态代理，需要自己手动配置。
