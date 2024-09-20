Java Spring AOP
<a name="W9NW9"></a>
## Spring AOP概述
在使用Spring框架的时候，经常需要和Spring的2大特性，IOC和AOP打交道。
<a name="dmxyJ"></a>
### 「AOP这种设计理念常见的概念如下」
![2021-09-28-08-53-51-605159.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632790895743-ca0c42d8-85d1-45d7-a4b1-49e856c723dd.png#clientId=ua13bcd06-0c02-4&from=ui&id=ue36f7612&originHeight=308&originWidth=1080&originalType=binary&ratio=1&size=119626&status=done&style=none&taskId=u8174faf3-f925-4f6b-b0c7-eabdc652ea8)
<a name="PhdxD"></a>
### 「AOP的主要应用场景如下」
![2021-09-28-08-53-51-907150.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632790895755-76ff7de3-5fa7-416b-8f7d-9abefdf7f727.png#clientId=ua13bcd06-0c02-4&from=ui&id=OuaLX&originHeight=514&originWidth=1080&originalType=binary&ratio=1&size=93543&status=done&style=none&taskId=u230fb2a9-7c20-4fee-961b-14afcebf94c)
<a name="sYOQk"></a>
### 「Spring AOP的实现主要经历了2代」
第一代：spring1.x版本，自己实现了AOP的功能 第二代：spring2.x版本，Spring集成了AspectJ的实现
<a name="wdjkX"></a>
## Spring AOP一代
「当要基于现成的实现增加横切逻辑时，首先需要找到哪些地方增强，就用Pointcut来进行筛选吧」<br />先写一个Service方便后面的演示
```java
public interface EchoService {

    String echo(String message);
}

public class DefaultEchoService implements EchoService {

    @Override
    public String echo(String message) {
        return message;
    }
}
```
<a name="vIDo2"></a>
### Pointcut
Pointcut接口定义如下
```java
public interface Pointcut {

    // 通过类过滤
    ClassFilter getClassFilter();

    // 通过方法过滤
    MethodMatcher getMethodMatcher();

    Pointcut TRUE = TruePointcut.INSTANCE;

}
```
「当想筛选出EchoService的echo方法时，就可以定义如下的Pointcut」
```java
public class EchoPointcut implements Pointcut {

    @Override
    public ClassFilter getClassFilter() {
        return new ClassFilter() {
            @Override
            public boolean matches(Class<?> clazz) {
                return EchoService.class.isAssignableFrom(clazz);
            }
        };
    }

    @Override
    public MethodMatcher getMethodMatcher() {
        return new MethodMatcher() {
            @Override
            public boolean matches(Method method, Class<?> targetClass) {
                return "echo".equals(method.getName()) &&
                        method.getParameterTypes().length == 1 &&
                        Objects.equals(String.class, method.getParameterTypes()[0]);
            }

            @Override
            public boolean isRuntime() {
                return false;
            }

            @Override
            public boolean matches(Method method, Class<?> targetClass, Object... args) {
                return false;
            }
        };
    }
}
```
看起来还是很麻烦的，因此Spring内置了很多实现，一般情况下用内置的实现即可，不用自己定义，上面的筛选过程就可以改为如下
```java
// 方法名为 echo 会被拦截
NameMatchMethodPointcut pointcut = new NameMatchMethodPointcut();
pointcut.setMappedName("echo");
```
Spring提供的部分Pointcut实现如下<br />![2021-09-28-08-53-51-907150.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632790952324-61d49961-bc97-4133-af79-c94b53abe2ba.png#clientId=ua13bcd06-0c02-4&from=ui&id=ud250402c&originHeight=514&originWidth=1080&originalType=binary&ratio=1&size=93543&status=done&style=none&taskId=u306a0f71-ddd9-4aea-b545-a3d3687316b)
<a name="S5k83"></a>
### Jointpoint
「通过Pointcut筛选出来的要增加横切逻辑的地方就是Jointpoint。」 在AOP理念中，很多地方可以增加横切逻辑，如方法执行，字段设置等。但是「Spring只支持方法执行这一种Joinpoint」，因为这种类型的Jointpoint基本上已经满足80%的场景了<br />Joinpoint类型中 「方法调用优于方法执行」<br />![2021-09-28-08-53-52-532155.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632790952331-d3486818-cb81-48ba-943f-176f3b0db9c7.png#clientId=ua13bcd06-0c02-4&from=ui&id=TMncV&originHeight=662&originWidth=948&originalType=binary&ratio=1&size=72922&status=done&style=none&taskId=u2c112220-3a20-4441-9b36-1ba8dfaaeb7)<br />因为Spring中只支持方法执行这一种Joinpoint，所以可以从Joinpoint实现类中获取增强的方法信息<br />![2021-09-28-08-53-52-810151.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632790952330-6c8f5916-3481-471d-84c9-37c726f767d2.png#clientId=ua13bcd06-0c02-4&from=ui&id=I58cQ&originHeight=964&originWidth=744&originalType=binary&ratio=1&size=44613&status=done&style=none&taskId=uf9f640fe-b793-4703-a4cc-9b456a20a37)
<a name="cFVeB"></a>
### Advice
当筛选出Jointpoint时，就需要在这些`Jointpoint`上增加横切逻辑，这些横切逻辑被称为Advice<br />![2021-09-28-08-53-53-138160.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632791009315-1f51fb20-d7e2-409a-a96e-3fd41caa6188.png#clientId=ua13bcd06-0c02-4&from=ui&id=Tm5Xh&originHeight=366&originWidth=1080&originalType=binary&ratio=1&size=75149&status=done&style=none&taskId=u6cc501f4-c731-4e2b-8864-3cc57180b90)<br />在Spring中实现横切逻辑的方式有两类

1. 实现`Advice`接口
2. 实现`IntroductionInfo`接口

实现`Advice`接口的方式最常用。实现`IntroductionInfo`接口的方式基本不会用，这里演示一下具体的用法，方便理解整个AOP API的设计理念<br />「`IntroductionInfo`主要是通过给目标类实现特定接口来增加新功能」
```java
public interface SayName {

    String getName();
}

public class DefaultSayName implements SayName {

    @Override
    public String getName() {
        return "I am service";
    }
}

public static void main(String[] args) {
    SayName sayName = new DefaultSayName();
    EchoService echoService = new DefaultEchoService();
    // IntroductionInfo接口的内置实现
    DelegatingIntroductionInterceptor interceptor =
            new DelegatingIntroductionInterceptor(sayName);
    Advisor advisor = new DefaultIntroductionAdvisor(interceptor, SayName.class);
    ProxyFactory proxyFactory = new ProxyFactory(echoService);
    proxyFactory.addAdvisor(advisor);
    // hello world
    EchoService proxyService = (EchoService) proxyFactory.getProxy();
    System.out.println(proxyService.echo("hello world"));
    // I am service
    SayName proxySayName = (SayName) proxyFactory.getProxy();
    System.out.println(proxySayName.getName());
}
```
可能对这个例子中的`Advisor`和`ProxyFactory`比较陌生，不知道起了啥作用，后面会详细分析这2个类的作用<br />「实现`Advice`接口的方式，应该是Spring AOP一代中最常见的使用方式了」<br />「对`HashMap`的`put`方法增加执行前的横切逻辑」, 打印放入`HashMap`的key和value的值
```java
public static void main(String[] args) {
    JdkRegexpMethodPointcut pointcut = new JdkRegexpMethodPointcut();
    pointcut.setPattern(".*put.*");
    DefaultPointcutAdvisor advisor = new DefaultPointcutAdvisor();
    advisor.setPointcut(pointcut);
    advisor.setAdvice(new MethodBeforeAdvice() {
        @Override
        public void before(Method method, Object[] args, Object target) throws Throwable {
            System.out.printf("当前存放的key为 %s，值为 %s", args[0], args[1]);
        }
    });

    ProxyFactory proxyFactory = new ProxyFactory(new HashMap());
    proxyFactory.addAdvisor(advisor);
    Map<String, String> proxyMap = (Map<String, String>) proxyFactory.getProxy();
    // 当前存放的key为 a，值为 a
    proxyMap.put("a", "a");
}
```
<a name="BQaY9"></a>
### Advisor
前面说过在AOP设计理念中，用Aspect来声明切面，每个Aspect可以包含多个`Pointcut`和`Advice`。<br />「在Spring AOP一代中，Aspect对应的实现为Advisor」。即`Advisor`是`Pointcut`和`Advice`的容器，但是一个Advisor只能包含一个`Pointcut`和`Advice`![2021-09-28-08-53-53-454155.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632791009321-c875c1ea-ac57-4953-9260-9c5f27b64e69.png#clientId=ua13bcd06-0c02-4&from=ui&id=u9536dbaa&originHeight=235&originWidth=1080&originalType=binary&ratio=1&size=57114&status=done&style=none&taskId=uc3551aa7-443c-49b6-b8b1-06585f22022)<br />因为`Advice`的实现方式有两类，因此对应的`Advisor`也可以分为两类
<a name="bgbaB"></a>
### 织入
「在Spring中将Advice织入到`Jointpoint`的过程是通过动态代理来实现的」。当然织入的方式有很多种，不仅仅只有动态代理这一种实现<br />![2021-09-28-08-53-53-758154.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632791009335-6d9b9a1a-c168-4d36-99d5-58e6688fe709.png#clientId=ua13bcd06-0c02-4&from=ui&id=l6Oje&originHeight=281&originWidth=1080&originalType=binary&ratio=1&size=69283&status=done&style=none&taskId=ud14f4090-3048-4805-b88d-d2a5a4c41c0)<br />Spring用了jdk动态代理和cglib来实现动态代理。生成代理对象用了工厂模式。从api中就可以很清晰的看出来<br />![2021-09-28-08-53-53-987153.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632791076747-7dfe944f-d7d0-45ec-aee0-5f5fa5fda6e1.png#clientId=ua13bcd06-0c02-4&from=ui&id=u64c6129b&originHeight=331&originWidth=675&originalType=binary&ratio=1&size=19967&status=done&style=shadow&taskId=uf5158422-6341-49f3-96c0-1bdbfa735b4)
<a name="uVOWa"></a>
#### 「jdk动态代理」
```java
public class CostInvocationHandler implements InvocationHandler {

    private Object target;

    public CostInvocationHandler(Object target) {
        this.target = target;
    }

    @Override
    public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
        long startTime = System.currentTimeMillis();
        Object result = method.invoke(target, args);
        long cost = System.currentTimeMillis() - startTime;
        System.out.println("cost " + cost);
        return result;
    }
}

public static void main(String[] args) {
    ClassLoader classLoader = Thread.currentThread().getContextClassLoader();
    Object proxy = Proxy.newProxyInstance(classLoader,
            new Class[]{EchoService.class},
            new CostInvocationHandler(new DefaultEchoService()));
    EchoService echoService = (EchoService) proxy;
    // cost 0
    // hello world
    System.out.println(echoService.echo("hello world"));
}
```
<a name="iZtzP"></a>
#### 「cglib」
```java
public static void main(String[] args) {
    Enhancer enhancer = new Enhancer();
    enhancer.setSuperclass(DefaultEchoService.class);
    enhancer.setInterfaces(new Class[] {EchoService.class});
    enhancer.setCallback(new MethodInterceptor() {
        @Override
        public Object intercept(Object source, Method method, Object[] args, MethodProxy methodProxy) throws Throwable {
            long startTime = System.currentTimeMillis();
            Object result = methodProxy.invokeSuper(source, args);
            long cost = System.currentTimeMillis() - startTime;
            System.out.println("cost " + cost);
            return result;
        }
    });
    EchoService echoService = (EchoService) enhancer.create();
    // cost 29
    // hello world
    System.out.println(echoService.echo("hello world"));
}
```
<a name="MF43e"></a>
### Spring AOP的自动动态代理
上面一直通过API的形式来演示，当然也可以把这些对象放入Spring容器，让Spring来管理，并且对Spring容器中的Bean生成代理对象<br />上面的Demo可以改为如下形式，变化基本不大
<a name="SL3tC"></a>
#### 「手动配置」
```java
public class ProxyConfig {

    // 创建代理对象
    @Bean
    public EchoService echoService() {
        return new DefaultEchoService();
    }

    // 创建advice
    @Bean
    public CostMethodInterceptor costInterceptor() {
        return new CostMethodInterceptor();
    }

    // 使用pointcut和advice创建advisor
    @Bean
    public Advisor advisor() {
        NameMatchMethodPointcutAdvisor advisor = new NameMatchMethodPointcutAdvisor();
        advisor.setMappedName("echo");
        advisor.setAdvice(costInterceptor());
        return advisor;
    }

    // 创建代理对象
    @Bean("echoProxy")
    public ProxyFactoryBean proxyFactoryBean(EchoService echoService) {
        ProxyFactoryBean proxyFactoryBean = new ProxyFactoryBean();
        proxyFactoryBean.setTarget(echoService);
        proxyFactoryBean.setInterceptorNames("advisor");
        return proxyFactoryBean;
    }
}

public static void main(String[] args) {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(ProxyConfig.class);
    // 获取代理对象
    EchoService echoService = (EchoService) context.getBean("echoProxy");
    // cost 0
    // hello world
    System.out.println(echoService.echo("hello world"));
}
```
「可以看到对每个生成的代理对象都要配置对应的`ProxyFactoryBean`，然后从容器中获取代理对象来使用」。当代理对象很少时还能应付，当代理对象很多时，那还不得累到吐血。有没有什么简单的办法呢？<br />Spring肯定也想到了这个问题，所以他提供了如下一个类`DefaultAdvisorAutoProxyCreator`来实现自动代理，将这个类放入Spring容器即可，如下所示
<a name="ag14o"></a>
#### 「自动配置」
```java
public class AutoProxyConfig {

    // 创建代理对象
    @Bean
    public EchoService echoService() {
        return new DefaultEchoService();
    }

    // 创建advice
    @Bean
    public CostMethodInterceptor costInterceptor() {
        return new CostMethodInterceptor();
    }

    // 使用pointcut和advice创建advisor
    @Bean
    public Advisor advisor() {
        NameMatchMethodPointcutAdvisor advisor = new NameMatchMethodPointcutAdvisor();
        advisor.setMappedName("echo");
        advisor.setAdvice(costInterceptor());
        return advisor;
    }

    @Bean
    public DefaultAdvisorAutoProxyCreator autoProxyCreator() {
        return new DefaultAdvisorAutoProxyCreator();
    }
}

public static void main(String[] args) {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(AutoProxyConfig.class);
    EchoService echoService = context.getBean(EchoService.class);
    // cost 0
    // hello world
    System.out.println(echoService.echo("hello world"));
}
```
从容器中获取的对象直接就是被代理后的对象，非常方便。「Spring AOP提供了很多类来实现自动代理，但他们有一个共同的父类`AbstractAutoProxyCreator`，看来自动代理的秘密就在这个`AbstractAutoProxyCreator`类中」![2021-09-28-08-53-54-228159.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1632791076749-cc15a147-7fc0-4737-9ee1-92ac250a78be.jpeg#clientId=ua13bcd06-0c02-4&from=ui&id=ClvRC&originHeight=210&originWidth=1080&originalType=binary&ratio=1&size=18817&status=done&style=none&taskId=u1d56f346-dd43-45f7-b8dd-40aa67239e2)
<a name="Ru6Et"></a>
## Spring AOP自动动态代理的实现方式
如果让自己实现对象的自动代理，会怎么做呢？<br />当然是通过`BeanPostProcessor`来干预Bean的声明周期，聪明！Spring就是这么干的，来验证一下想法![2021-09-28-08-53-54-507150.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632791076764-76c1e81b-7c47-4d0e-ad0f-5c031ffe3724.png#clientId=ua13bcd06-0c02-4&from=ui&id=TtQsG&originHeight=964&originWidth=1052&originalType=binary&ratio=1&size=61273&status=done&style=none&taskId=uaa327abe-bfc8-4a35-907a-d74bb26fc07)<br />看这个类的继承关系，基本上就验证了。只要看看他重写了`BeanPostProcessor`的哪些方法即可？<br />「`AbstractAutoProxyCreator`重写了如下2个重要的方法」`postProcessBeforeInstantiation`（Bean实例化前阶段执行） `postProcessAfterInitialization`（Bean初始化后阶段执行）<br />![2021-09-28-08-53-54-870182.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632791109803-a77be9c3-4de2-472e-b900-52b712017192.png#clientId=ua13bcd06-0c02-4&from=ui&id=uacfc1c38&originHeight=1581&originWidth=878&originalType=binary&ratio=1&size=309212&status=done&style=shadow&taskId=u85b3b3e7-2a79-4c49-9d29-154c8b9c32e)
<a name="CzHZ7"></a>
### 「`postProcessBeforeInstantiation`（Bean实例化前阶段执行）」
![2021-09-28-08-53-55-208167.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632791109797-b69fee4f-846c-4047-a5e1-aed712de2b09.png#clientId=ua13bcd06-0c02-4&from=ui&id=hNqWr&originHeight=785&originWidth=1080&originalType=binary&ratio=1&size=222982&status=done&style=none&taskId=u47474425-0272-4a4c-8ed3-ab96cecf660)<br />当用户自定义了`TargetSource`的实现时，会从`TargetSource`中获取目标对象生成代理。但是一般情况下很少会自定义`TargetSource`的实现。所以这部分就不再分析了。直接看`postProcessAfterInitialization`
<a name="OfoJR"></a>
### 「`postProcessAfterInitialization`（Bean初始化后阶段执行）」
![2021-09-28-08-53-55-565154.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632791109759-60b08085-82da-4f18-8f05-d74ba2119ed2.png#clientId=ua13bcd06-0c02-4&from=ui&id=IWeGM&originHeight=474&originWidth=1080&originalType=binary&ratio=1&size=112015&status=done&style=none&taskId=u7a469edb-6acf-4005-8362-52446a3246d)<br />如果没有经过代理的化就会进入`wrapIfNecessary`方法<br />![2021-09-28-08-53-55-918195.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632791140174-f444052d-1091-44f4-ba8c-8ce1fe7b9995.png#clientId=ua13bcd06-0c02-4&from=ui&id=u0dfeb2d7&originHeight=764&originWidth=1080&originalType=binary&ratio=1&size=218821&status=done&style=none&taskId=u4f131413-8c78-45ec-b53e-bd6f79496ff)<br />思路很简单，就是根据Bean获取对应的Advisor，然后创建其代理对象，并返回。<br />![2021-09-28-08-53-56-149161.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632791140108-d0dcfdbf-c5fe-41d0-b4d8-8a6ce339b0d3.png#clientId=ua13bcd06-0c02-4&from=ui&id=m2Tup&originHeight=241&originWidth=711&originalType=binary&ratio=1&size=42038&status=done&style=shadow&taskId=ub84bff48-0b86-4924-b779-417088ae68d)<br />「所以当面试官问Spring AOP和IOC是如何结合在一起的时候，是不是知道该如何回答了？」<br />在Bean生命周期的Bean初始化后阶段，如果这个Bean需要增加横切逻辑，则会在这个阶段生成对应的代理对象
<a name="QZdpQ"></a>
## Spring AOP二代（集成了AspectJ）
当Spring 2.0发布以后，Spring AOP增加了新的使用方式，Spring AOP集成了AspectJ。最常用的就是这个版本的Spring AOP<br />主要有如下变化

1. 可以用POJO来定义Aspect和Adivce，并提供了一系列相应的注解，如`@Aspect`和`@Around`等。而不用像1.x版本中实现相应的接口
2. 支持aspectj中的`pointcut`的表达方式

![2021-09-28-08-53-56-409186.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632791140140-48d369b1-b7d5-415b-9e73-8a0cc5aa3922.png#clientId=ua13bcd06-0c02-4&from=ui&id=ZjrxH&originHeight=458&originWidth=1066&originalType=binary&ratio=1&size=80499&status=done&style=none&taskId=u4d29aa61-35b5-48d8-881c-098d84e5f32)<br />演示一下2.0版本中aop的使用方式<br />定义切面
```java
@Aspect
public class AspectDefine {

    @Pointcut("execution(* com.javashitang.proxy.EchoService.echo(..))")
    public void pointcutName() {}

    @Around("pointcutName()")
    public Object calCost(ProceedingJoinPoint joinPoint) throws Throwable {
        long startTime = System.currentTimeMillis();
        Object result = joinPoint.proceed();
        long cost = System.currentTimeMillis() - startTime;
        System.out.println("cost " + cost);
        return result;
    }

    @Before("pointcutName()")
    public void beforeMethod() {
        System.out.println("beforeMethod");
    }
}
```
增加配置，注入实现类
```java
@EnableAspectJAutoProxy
public class AspectJConfig {

    @Bean
    public EchoService echoService() {
        return new DefaultEchoService();
    }
}

public static void main(String[] args) {
    AnnotationConfigApplicationContext context =
            new AnnotationConfigApplicationContext(AspectJConfig.class, AspectDefine.class);
    EchoService echoService = context.getBean(EchoService.class);
    // beforeMethod
    // cost 0
    // hello world
    System.out.println(echoService.echo("hello world"));
    context.close();
}
```
「虽然spring2.0之后spring aop集成了AspectJ，但实际上只是拿AspectJ的“皮大衣“用了一下，因为底层的实现和织入方式还是1.x原先的实现体系」
<a name="Qh3UA"></a>
### `@EnableAspectJAutoProxy`有啥用？
「当使用2.0版本的aop时，必须在配置类上加上`@EnableAspectJAutoProxy`注解，那么这个注解有啥作用呢？」
```java
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@Documented
@Import(AspectJAutoProxyRegistrar.class)
public @interface EnableAspectJAutoProxy {

 boolean proxyTargetClass() default false;

 boolean exposeProxy() default false;

}
```
可以看到很重要的一句
```java
@Import(AspectJAutoProxyRegistrar.class)
```
通过`@Import`注入bean，「通过`@Import`注解注入Bean的方式有如下三种」

1. 基于`Configuration Class`
2. 基于`ImportSelector`接口
3. 基于`ImportBeanDefinitionRegistrar`接口

![2021-09-28-08-53-56-709152.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632791167082-f4582409-80b6-44d7-b46b-18b0f31423e2.png#clientId=ua13bcd06-0c02-4&from=ui&id=u466207dc&originHeight=652&originWidth=1080&originalType=binary&ratio=1&size=180417&status=done&style=none&taskId=u67c13f99-e6ab-4dd5-9ddf-0b6b3448148)<br />这个代码主要做了2个事情

1. 往容器中注入`AnnotationAwareAspectJAutoProxyCreator`
2. 当`@EnableAspectJAutoProxy`注解中的`proxyTargetClass`或者`exposeProxy`属性为`true`的时候，将`AnnotationAwareAspectJAutoProxyCreator`中的`proxyTargetClass`或者`exposeProxy`属性改为`true`

「`proxyTargetClass`和`exposeProxy`保存在`AnnotationAwareAspectJAutoProxyCreator`类的父类`ProxyConfig`中，这个类存了一些配置，用来控制代理对象的生成过程」<br />`proxyTargetClass：true`使用CGLIB基于类创建代理；false使用java接口创建代理 `exposeProxy：true`将代理对象保存在`AopContext`中，否则不保存<br />第一个属性比较容易理解，那么第二个属性有啥作用呢？演示一下
```java
@Service
public class SaveSevice {

    public void method1() {
        System.out.println("method1 executed");
        method2();
    }

    public void method2() {
        System.out.println("method2 executed");
    }
}

@Aspect
public class AspectDefine {

    @Pointcut("execution(* com.javashitang.invalid.SaveSevice.method2(..))")
    public void pointcutName() {}

    @Around("pointcutName()")
    public Object calCost(ProceedingJoinPoint joinPoint) throws Throwable {
        System.out.println("开启事务");
        return joinPoint.proceed();
    }
}

@EnableAspectJAutoProxy
public class InvalidDemo {

    public static void main(String[] args) {
        AnnotationConfigApplicationContext context =
                new AnnotationConfigApplicationContext(SaveSevice.class,
                        AspectDefine.class, InvalidDemo.class);
        SaveSevice saveSevice = context.getBean(SaveSevice.class);
        saveSevice.method1();
        System.out.println("--");
        saveSevice.method2();
    }
}
```
结果为
```
method1 executed
method2 executed
--
开启事务
method2 executed
```
「可以看到通过method1调用method2时，aop没有生效。直接调用method2时，aop才会生效。事务方法自调用失效就是因为这个原因，因为调用的不是代理对象的方法」<br />解决方法有很多种，例如重新从`ApplicationContext`中取一下代理对象，然后调用代理对象的方法。另一种就是通过`AopContext`获取代理对象，实现原理就是当方法调用时会将代理对象放到`ThreadLocal`中
```java
@Service
public class SaveSevice {

    public void method1() {
        System.out.println("method1 executed");
        ((SaveSevice) AopContext.currentProxy()).method2();
    }

    public void method2() {
        System.out.println("method2 executed");
    }
}
```
将`exposeProxy`属性改为`true`
```java
@EnableAspectJAutoProxy(exposeProxy = true)
```
```
method1 executed
开启事务
method2 executed
--
开启事务
method2 executed
```
可以看到aop成功生效。「当使用`@Transactional`注解，分布式事务框架时一定要注意子调用这个问题，不然很容易造成事务失效」<br />往容器中注入`AnnotationAwareAspectJAutoProxyCreator`，那么这个类有啥作用呢？![2021-09-28-08-53-56-982151.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632791167063-c7a57ce7-2fbd-47bd-ad20-5148fc6be341.png#clientId=ua13bcd06-0c02-4&from=ui&id=xkRI1&originHeight=1128&originWidth=1052&originalType=binary&ratio=1&size=74135&status=done&style=none&taskId=uf1099ede-5b71-4a00-a6c4-6583e70b730)<br />看这继承关系是不是和上面分析的`DefaultAdvisorAutoProxyCreator`类很相似，这不就是为了开启自动代理吗？
<a name="FxrKy"></a>
### 切点表达式
「Spring AOP用`AspectJExpressionPointcut`桥接了Aspect的筛选能力」。其实Aspect有很多种类型的切点表达式，但是Spring AOP只支持如下10种，因为Aspect支持很多种类型的`JoinPoint`，但是Spring AOP只支持方法执行这一种`JoinPoint`，所以其余的表达式就没有必要了。<br />![2021-09-28-08-53-57-482155.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632791167178-7236d243-0a4f-48ab-a1e9-7f3d4ff714c6.png#clientId=ua13bcd06-0c02-4&from=ui&id=j5703&originHeight=419&originWidth=1080&originalType=binary&ratio=1&size=428788&status=done&style=none&taskId=uacf3831d-f82a-446e-accc-940a75cd7c6)<br />因为AspectJ提供的表达式在工作中经常被使用，结合Demo演示一下具体的用法

| 表达式类型 | 解释 |
| --- | --- |
| execution | 匹配方法表达式，首选方式 |
| within | 限定类型 |
| this | 代理对象是指定类型 ，所有方法都会被拦截 |
| target | 目标对象是指定类型，所有方法都会被拦截 |
| args | 匹配方法中的参数 |
| @target | 目标对象有指定的注解，所有方法都会被拦截 |
| @args | 方法参数所属类型上有指定注解 |
| @within | 调用对象上有指定的注解，所有方法都会被拦截 |
| @annotation | 有指定注解的方法 |

<a name="iL3Lm"></a>
#### 「`execution`」
匹配方法表达式，首选方式
```java
execution(modifiers-pattern? ret-type-pattern declaring-type-pattern?name-pattern(param-pattern)
                throws-pattern?)
```
拦截`Performance`类的`perform`方法的切点表达式如<br />![2021-09-28-08-53-57-783164.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632791197487-f622a8a4-7890-43db-ba4b-0ae38db93626.png#clientId=ua13bcd06-0c02-4&from=ui&id=u7a85d69b&originHeight=321&originWidth=1080&originalType=binary&ratio=1&size=63251&status=done&style=shadow&taskId=u2733df57-9c32-4ba1-9bb1-2c437fcaad9)<br />放几个官方的Demo
```java
// The execution of any public method:
execution(public * *(..))

// The execution of any method with a name that begins with set
execution(* set*(..))

// The execution of any method defined by the AccountService interface
execution(* com.xyz.service.AccountService.*(..))

// The execution of any method defined in the service package:
execution(* com.xyz.service.*.*(..))
```
<a name="Xkra9"></a>
#### 「`within`」限定类型
```java
// 拦截service包中任意类的任意方法
within(com.xyz.service.*)

// 拦截service包及子包中任意类的任意方法
within(com.xyz.service..*)
```
<a name="IwWhZ"></a>
#### 「`this`」
代理对象是指定类型，所有方法都会被拦截<br />举个例子说明一下
```java
@Configuration
@EnableAspectJAutoProxy
public class ThisDemo {

    public static void main(String[] args) {
        AnnotationConfigApplicationContext context =
                new AnnotationConfigApplicationContext(ThisDemo.class, AspectDefine.class);
        Name name = context.getBean(Name.class);
        name.getName();
        System.out.println(name instanceof Student);
    }


    @Aspect
    public class AspectDefine {
        @Before("this(com.javashitang.aspectjPointcut.thisDemo.ThisDemo.Student)")
        public void before() {
            System.out.println("before");
        }
    }

    @Bean
    public Student student() {
        return new Student();
    }

    public class Student implements Name {

        @Override
        public String getName() {
            return null;
        }
    }

    public interface Name {
        String getName();
    }
}
```
输出为
```
false
```
有接口时会使用jdk动态代理，因此代理对象为Proxy，不会拦截<br />当设置为jdk动态代理为，代理对象为Student，正常拦截<br />将注解改为如下形式 `@EnableAspectJAutoProxy(proxyTargetClass = true)`<br />输出为
```
before
true
```
「target」目标对象是指定类型，所有方法都会被拦截
```java
// 目标对象为AccountService类型的会被代理
target(com.xyz.service.AccountService)
```
this 和 target 的不同点「this作用于代理对象，target作用于目标对象」
<a name="IesEX"></a>
#### 「`args`」匹配方法中的参数
```java
// 匹配只有一个参数，且类型为com.ms.aop.args.demo1.UserModel
@Pointcut("args(com.ms.aop.args.demo1.UserModel)")

// 匹配多个参数
args(type1,type2,typeN)

// 匹配第一个参数类型为com.ms.aop.args.demo1.UserModel的所有方法, .. 表示任意个参数
@Pointcut("args(com.ms.aop.args.demo1.UserModel,..)")
```
<a name="sKIvR"></a>
#### 「`@target`」目标对象有指定的注解，所有方法都会被拦截
```java
// 目标对象中包含com.ms.aop.jtarget.Annotation1注解，调用该目标对象的任意方法都会被拦截
@target(com.ms.aop.jtarget.Annotation1)
```
<a name="UNxPQ"></a>
#### 「`@args`」方法参数所属类型上有指定注解
```java
// 匹配1个参数，且第1个参数所属的类中有Anno1注解
@args(com.ms.aop.jargs.demo1.Anno1)
// 匹配多个参数，且多个参数所属的类型上都有指定的注解
@args(com.ms.aop.jargs.demo1.Anno1,com.ms.aop.jargs.demo1.Anno2)
// 匹配多个参数，且第一个参数所属的类中有Anno1注解
@args(com.ms.aop.jargs.demo2.Anno1,…)
```
<a name="FEkOv"></a>
#### 「`@within`」调用对象上有指定的注解，所有方法都会被拦截
```java
// 声明有com.ms.aop.jwithin.Annotation1注解的类中的所有方法都会被拦截
@within(com.ms.aop.jwithin.Annotation1)
```
「`@target` 和 `@within` 的不同点」`@target`关注的是被调用的对象，`@within`关注的是调用的对象<br />「`@annotation`」有指定注解的方法
```java
// 被调用方法上有Annotation1注解
@annotation(com.ms.aop.jannotation.demo2.Annotation1)
```
<a name="Kolf6"></a>
## Adivce之间的顺序关系
一个方法被一个aspect类拦截时的执行顺序如下<br />`@Around`->`@Before`->方法执行->`@Around-`>`@After`->`@AfterReturning`/`@AfterThrowing`<br />当方法正常结束时，执行`@AfterReturning`。方法异常结束时，执行`@AfterThrowing`。两者不会同时执行哈![2021-09-28-08-53-58-040157.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632791197493-bdf63e5d-b29f-428d-98bf-ec2b6b382662.png#clientId=ua13bcd06-0c02-4&from=ui&id=gf2De&originHeight=482&originWidth=261&originalType=binary&ratio=1&size=18901&status=done&style=shadow&taskId=u48e67445-bd69-42e1-9a1f-11096737de6)<br />一个方法被多个aspect类拦截时的执行顺序如下<br />![2021-09-28-08-53-58-400152.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632791197530-3b9043cd-d601-464e-9e3b-a78f7c11dc6b.png#clientId=ua13bcd06-0c02-4&from=ui&id=e3RLG&originHeight=449&originWidth=965&originalType=binary&ratio=1&size=154871&status=done&style=shadow&taskId=uf0494a1f-00b0-43c2-9329-c43feaea31d)<br />「多个aspect的执行顺序可以通过`@Order`注解或者实现`Oreder`接口来控制」<br />「`Adivce`的顺序一定要梳理清楚，不然有时候产生的很多魔幻行为都不知道怎么发生的」
