Java Spring SpringBoot AOP
<a name="ZtO2h"></a>
## 1、理解AOP
<a name="NyGQK"></a>
### 1.1 什么是AOP
AOP（Aspect Oriented Programming），面向切面思想，是Spring的三大核心思想之一（两外两个：IOC-控制反转、DI-依赖注入）。<br />那么AOP为何那么重要呢？在程序中，经常存在一些系统性的需求，比如权限校验、日志记录、统计等，这些代码会散落穿插在各个业务逻辑中，非常冗余且不利于维护。例如下面这个示意图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1622974532379-c0cadf3e-e9c7-4272-bc29-36b30c3bf800.webp#averageHue=%230a0906&clientId=uaafa8a92-65b6-4&from=paste&id=udbc327fd&originHeight=478&originWidth=513&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6350ff04-023a-421e-9b35-cae1055f7a8&title=)<br />有多少业务操作，就要写多少重复的校验和日志记录代码，这显然是无法接受的。当然，用面向对象的思想，可以把这些重复的代码抽离出来，写成公共方法，就是下面这样：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1622974532136-05f14ed9-b6d5-44ab-8404-49eeee1069c2.webp#averageHue=%23845752&clientId=uaafa8a92-65b6-4&from=paste&id=u758cca84&originHeight=528&originWidth=386&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc6982de4-1eb4-416e-95ce-c6212972808&title=)<br />这样，代码冗余和可维护性的问题得到了解决，但每个业务方法中依然要依次手动调用这些公共方法，也是略显繁琐。有没有更好的方式呢？有的，那就是AOP，AOP将权限校验、日志记录等非业务代码完全提取出来，与业务代码分离，并寻找节点切入业务代码中：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1622974532245-7dea0349-8b44-4418-ab3c-0c93f7744993.webp#averageHue=%23fefefb&clientId=uaafa8a92-65b6-4&from=paste&id=uca376d98&originHeight=346&originWidth=728&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6686ec61-9b22-47f8-96ae-5789d8b8118&title=)
<a name="XRdWx"></a>
### 1.2 AOP体系与概念
简单地去理解，其实AOP要做三类事：

- 在哪里切入，也就是权限校验等非业务操作在哪些业务代码中执行。
- 在什么时候切入，是业务代码执行前还是执行后。
- 切入后做什么事，比如做权限校验、日志记录等。

因此，AOP的体系可以梳理为下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1622974532387-8ad65718-52c9-4ec5-b996-6aba784d99c7.webp#averageHue=%233a3a3a&clientId=uaafa8a92-65b6-4&from=paste&id=ub79710b2&originHeight=591&originWidth=744&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1b1ab051-1227-43a0-946c-76f16ba0171&title=)<br />一些概念详解：

- `Pointcut`：切点，决定处理如权限校验、日志记录等在何处切入业务代码中（即织入切面）。切点分为`execution`方式和`annotation`方式。前者可以用路径表达式指定哪些类织入切面，后者可以指定被哪些注解修饰的代码织入切面。
- Advice：处理，包括处理时机和处理内容。处理内容就是要做什么事，比如校验权限和记录日志。处理时机就是在什么时机执行处理内容，分为前置处理（即业务代码执行前）、后置处理（业务代码执行后）等。
- `Aspect`：切面，即`Pointcut`和`Advice`。
- `Joint point`：连接点，是程序执行的一个点。例如，一个方法的执行或者一个异常的处理。在 Spring AOP 中，一个连接点总是代表一个方法执行。
- `Weaving`：织入，就是通过动态代理，在目标对象方法中执行处理内容的过程。
<a name="ULbcW"></a>
## 2、AOP实例
实践出真知，接下来就写代码来实现一下AOP。<br />使用 AOP，首先需要引入 AOP 的依赖。
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-aop</artifactId>
</dependency>
```
<a name="B9G2B"></a>
### 2.1 第一个实例
接下来，先看一个极简的例子：所有的`get`请求被调用前在控制台输出一句"get请求的advice触发了"。<br />具体实现如下：

1. 创建一个AOP切面类，只要在类上加个 `@Aspect` 注解即可。`@Aspect` 注解用来描述一个切面类，定义切面类的时候需要打上这个注解。`@Component` 注解将该类交给 Spring 来管理。在这个类里实现advice：
```java
import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Before;
import org.aspectj.lang.annotation.Pointcut;
import org.springframework.stereotype.Component;

@Aspect
@Component
public class LogAdvice {
    // 定义一个切点：所有被GetMapping注解修饰的方法会织入advice
    @Pointcut("@annotation(org.springframework.web.bind.annotation.GetMapping)")
    private void logAdvicePointcut() {}

    // Before表示logAdvice将在目标方法执行前执行
    @Before("logAdvicePointcut()")
    public void logAdvice(){
        // 这里只是一个示例，你可以写任何处理逻辑
        System.out.println("get请求的advice触发了");
    }
}
```

1. 创建一个接口类，内部创建一个get请求：
```java
import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONObject;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping(value = "/aop")
public class AopController {
    @GetMapping(value = "/getTest")
    public JSONObject aopTest() {
        return JSON.parseObject("{\"message\":\"SUCCESS\",\"code\":200}");
    }

    @PostMapping(value = "/postTest")
    public JSONObject aopTest2(@RequestParam("id") String id) {
        return JSON.parseObject("{\"message\":\"SUCCESS\",\"code\":200}");
    }
}
```
项目启动后，请求http://localhost:8085/aop/getTest接口：<br />![2021-06-06-19-07-26-581287.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622977834001-71bac425-32bb-4b56-94c4-5622df7c15cd.png#averageHue=%238d9558&clientId=uaafa8a92-65b6-4&from=ui&id=uf77a4689&originHeight=224&originWidth=996&originalType=binary&ratio=3&rotation=0&showTitle=false&size=670697&status=done&style=none&taskId=ua9ac8ba1-9735-4d1e-a72b-3356f34accc&title=)![2021-06-06-19-07-27-380151.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622977835145-ed9cb04a-4743-4577-8823-3b0a2829c525.png#averageHue=%23f9f9f9&clientId=uaafa8a92-65b6-4&from=ui&id=u64cd1774&originHeight=177&originWidth=1040&originalType=binary&ratio=3&rotation=0&showTitle=false&size=16762&status=done&style=shadow&taskId=u0ae691cf-93b5-4c16-b0db-ddb513e82d2&title=)![2021-06-06-19-07-27-891782.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622977835134-ca014933-2b46-443d-a125-8eb5faf1b25a.png#averageHue=%233e4348&clientId=uaafa8a92-65b6-4&from=ui&id=u4c2a38b6&originHeight=181&originWidth=617&originalType=binary&ratio=3&rotation=0&showTitle=false&size=10210&status=done&style=none&taskId=uf418553c-4e25-46fd-b8d1-b9153ac9917&title=)<br />请求http://localhost:8085/aop/postTest接口，控制台无输出，证明切点确实是只针对被`GetMapping`修饰的方法。
<a name="ta267"></a>
### 2.2 第二个实例
下面将问题复杂化一些，该例的场景是：

1. 自定义一个注解`PermissionsAnnotation`
2. 创建一个切面类，切点设置为拦截所有标注`PermissionsAnnotation`的方法，截取到接口的参数，进行简单的权限校验
3. 将`PermissionsAnnotation`标注在测试接口类的测试接口test上

具体的实现步骤：

1. 使用`@Target`、`@Retention`、`@Documented`自定义一个注解：
```java
@Target(ElementType.METHOD)
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface PermissionAnnotation{
}
```

1. 创建第一个AOP切面类，，只要在类上加个 `@Aspect` 注解即可。`@Aspect` 注解用来描述一个切面类，定义切面类的时候需要打上这个注解。`@Component` 注解将该类交给 Spring 来管理。在这个类里实现第一步权限校验逻辑：
```java
package com.mu.demo.advice;

import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Before;
import org.aspectj.lang.annotation.Pointcut;
import org.springframework.stereotype.Component;

@Aspect
@Component
public class LogAdvice {
    // 定义一个切点：所有被GetMapping注解修饰的方法会织入advice
    @Pointcut("@annotation(org.springframework.web.bind.annotation.GetMapping)")
    private void logAdvicePointcut() {}

    // Before表示logAdvice将在目标方法执行前执行
    @Before("logAdvicePointcut()")
    public void logAdvice(){
        // 这里只是一个示例，你可以写任何处理逻辑
        System.out.println("get请求的advice触发了");
    }
}
```

1. 创建接口类，并在目标方法上标注自定义注解 PermissionsAnnotation：
```java
import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONObject;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping(value = "/permission")
public class TestController {
    @RequestMapping(value = "/check", method = RequestMethod.POST)
    // 添加这个注解
    @PermissionsAnnotation()
    public JSONObject getGroupList(@RequestBody JSONObject request) {
        return JSON.parseObject("{\"message\":\"SUCCESS\",\"code\":200}");
    }
}
```
在这里，先进行一个测试。首先，填好请求地址和header：<br />![2021-06-06-19-07-26-705957.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622977834179-605a03b8-273d-4c66-981c-8188b0035768.png#averageHue=%23f7f4f4&clientId=uaafa8a92-65b6-4&from=ui&id=Zd7Yw&originHeight=271&originWidth=1080&originalType=binary&ratio=3&rotation=0&showTitle=false&size=879809&status=done&style=shadow&taskId=ufa44c6a0-890f-4c82-8c67-55bb7e88df2&title=)<br />其次，构造正常的参数：<br />![2021-06-06-19-07-26-823639.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622977833989-c650105f-5d28-42cd-9497-fca32c75f40d.png#averageHue=%23f4f4f4&clientId=uaafa8a92-65b6-4&from=ui&id=PzDnz&originHeight=220&originWidth=938&originalType=binary&ratio=3&rotation=0&showTitle=false&size=620379&status=done&style=shadow&taskId=uc95fdebb-58df-4220-87c2-a77bac02c22&title=)<br />可以拿到正常的响应结果：<br />![2021-06-06-19-07-26-925368.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622977834120-3ae5b713-f71b-43a8-8621-2f5fc1e6d45c.png#averageHue=%23fbfaf9&clientId=uaafa8a92-65b6-4&from=ui&id=Dnj5t&originHeight=245&originWidth=1037&originalType=binary&ratio=3&rotation=0&showTitle=false&size=763755&status=done&style=shadow&taskId=u9b873461-12cf-495e-98d7-6965116941d&title=)<br />然后，构造一个异常参数，再次请求：<br />![2021-06-06-19-07-27-044050.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622977834009-2444474a-7e41-4204-a9d6-9259d982bb5d.png#averageHue=%23f4f3f3&clientId=uaafa8a92-65b6-4&from=ui&id=c2yq2&originHeight=209&originWidth=943&originalType=binary&ratio=3&rotation=0&showTitle=false&size=592508&status=done&style=shadow&taskId=u5301374a-c42b-4416-bf1a-54ae4b9fdc7&title=)<br />响应结果显示，切面类进行了判断，并返回相应结果：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1622974533534-342b4734-4902-4edb-8b51-3eb58dcbdf78.webp#averageHue=%23fafaf9&clientId=uaafa8a92-65b6-4&from=paste&id=u38090d33&originHeight=186&originWidth=1037&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=u606dfbd7-17db-4bbb-be27-32fa715aca5&title=)<br />如果一个接口想设置多个切面类进行校验怎么办？这些切面的执行顺序如何管理？<br />很简单，一个自定义的AOP注解可以对应多个切面类，这些切面类执行顺序由@Order注解管理，该注解后的数字越小，所在切面类越先执行。<br />下面在实例中进行演示：<br />创建第二个AOP切面类，在这个类里实现第二步权限校验：
```java
import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONObject;
import org.aspectj.lang.ProceedingJoinPoint;
import org.aspectj.lang.annotation.Around;
import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Pointcut;
import org.springframework.core.annotation.Order;
import org.springframework.stereotype.Component;

@Aspect
@Component
@Order(0)
public class PermissionSecondAdvice {

    @Pointcut("@annotation(com.example.demo.PermissionsAnnotation)")
    private void permissionCheck() {
    }

    @Around("permissionCheck()")
    public Object permissionCheckSecond(ProceedingJoinPoint joinPoint) throws Throwable {
        System.out.println("===================第二个切面===================：" + System.currentTimeMillis());

        //获取请求参数，详见接口类
        Object[] objects = joinPoint.getArgs();
        Long id = ((JSONObject) objects[0]).getLong("id");
        String name = ((JSONObject) objects[0]).getString("name");
        System.out.println("id->>>>>>>>>>>>>>>>>>>>>>" + id);
        System.out.println("name->>>>>>>>>>>>>>>>>>>>>>" + name);

        // name不是管理员则抛出异常
        if (!name.equals("admin")) {
            return JSON.parseObject("{\"message\":\"not admin\",\"code\":403}");
        }
        return joinPoint.proceed();
    }
}
```
重启项目，继续测试，构造两个参数都异常的情况：<br />![2021-06-06-19-07-27-264460.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622977835471-ccfb643c-3179-4aff-ade4-47c7f944c434.png#averageHue=%23f4f4f4&clientId=uaafa8a92-65b6-4&from=ui&id=PDDLF&originHeight=214&originWidth=946&originalType=binary&ratio=3&rotation=0&showTitle=false&size=608608&status=done&style=shadow&taskId=u8303216a-aec5-4df7-b178-436cd98b719&title=)<br />响应结果，表面第二个切面类执行顺序更靠前：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1622974534243-4c7d394a-563a-4094-b197-0faa7972fbc0.webp#averageHue=%23f4e1d5&clientId=uaafa8a92-65b6-4&from=paste&id=uc6fc596e&originHeight=177&originWidth=1040&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=u94e24c59-2e55-421b-9ad1-29266e5afcf&title=)
<a name="mFJn0"></a>
## 3、AOP相关注解
上面的案例中，用到了诸多注解，下面针对这些注解进行详解。
<a name="dQnq0"></a>
### 3.1 `@Pointcut`
`@Pointcut` 注解，用来定义一个切面，即上文中所关注的某件事情的入口，切入点定义了事件触发时机。
```java
@Aspect
@Component
public class LogAspectHandler {

    /**
     * 定义一个切面，拦截 com.itcodai.course09.controller 包和子包下的所有方法
     */
    @Pointcut("execution(* com.mutest.controller..*.*(..))")
    public void pointCut() {}
}
```
`@Pointcut` 注解指定一个切面，定义需要拦截的东西，这里介绍两个常用的表达式：一个是使用 `execution()`，另一个是使用 `annotation()`。<br />`execution`表达式：<br />以 `execution(* * com.mutest.controller..*.*(..)))` 表达式为例：

- 第一个 `*` 号的位置：表示返回值类型，`*` 表示所有类型。
- 包名：表示需要拦截的包名，后面的两个句点表示当前包和当前包的所有子包，在本例中指 com.mutest.controller包、子包下所有类的方法。
- 第二个 `*` 号的位置：表示类名，`*` 表示所有类。
- `*(..)`：这个星号表示方法名，`*` 表示所有的方法，后面括弧里面表示方法的参数，两个句点表示任何参数。

`annotation()` 表达式：<br />`annotation()` 方式是针对某个注解来定义切面，比如对具有 `@PostMapping` 注解的方法做切面，可以如下定义切面：
```java
@Pointcut("@annotation(org.springframework.web.bind.annotation.PostMapping)")
public void annotationPointcut() {}
```
然后使用该切面的话，就会切入注解是 `@PostMapping` 的所有方法。这种方式很适合处理 `@GetMapping`、`@PostMapping`、`@DeleteMapping`不同注解有各种特定处理逻辑的场景。<br />还有就是如上面案例所示，针对自定义注解来定义切面。
```java
@Pointcut("@annotation(com.example.demo.PermissionsAnnotation)")
private void permissionCheck() {}
```
<a name="WPls9"></a>
### 3.2 `@Around`
`@Around`注解用于修饰`Around`增强处理，`Around`增强处理非常强大，表现在：

1. `@Around`可以自由选择增强动作与目标方法的执行顺序，也就是说可以在增强动作前后，甚至过程中执行目标方法。这个特性的实现在于，调用`ProceedingJoinPoint`参数的`procedd()`方法才会执行目标方法。
2. `@Around`可以改变执行目标方法的参数值，也可以改变执行目标方法之后的返回值。

`Around`增强处理有以下特点：

1. 当定义一个`Around`增强处理方法时，该方法的第一个形参必须是 `ProceedingJoinPoint` 类型（至少一个形参）。在增强处理方法体内，调用`ProceedingJoinPoint`的`proceed`方法才会执行目标方法：这就是`@Around`增强处理可以完全控制目标方法执行时机、如何执行的关键；如果程序没有调用`ProceedingJoinPoint`的`proceed`方法，则目标方法不会执行。
2. 调用`ProceedingJoinPoint`的`proceed`方法时，还可以传入一个`Object[ ]`对象，该数组中的值将被传入目标方法作为实参——这就是`Around`增强处理方法可以改变目标方法参数值的关键。这就是如果传入的`Object[ ]`数组长度与目标方法所需要的参数个数不相等，或者`Object[ ]`数组元素与目标方法所需参数的类型不匹配，程序就会出现异常。

`@Around`功能虽然强大，但通常需要在线程安全的环境下使用。因此，如果使用普通的`Before`、`AfterReturning`就能解决的问题，就没有必要使用`Around`了。如果需要目标方法执行之前和之后共享某种状态数据，则应该考虑使用`Around`。尤其是需要使用增强处理阻止目标的执行，或需要改变目标方法的返回值时，则只能使用`Around`增强处理了。<br />下面，在前面例子上做一些改造，来观察`@Around`的特点。<br />自定义注解类不变。首先，定义接口类：
```java
import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONObject;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping(value = "/permission")
public class TestController {
    @RequestMapping(value = "/check", method = RequestMethod.POST)
    @PermissionsAnnotation()
    public JSONObject getGroupList(@RequestBody JSONObject request) {
        return JSON.parseObject("{\"message\":\"SUCCESS\",\"code\":200,\"data\":" + request + "}");
    }
}
```
唯一切面类（前面案例有两个切面类，这里只需保留一个即可）：
```java
import com.alibaba.fastjson.JSONObject;
import org.aspectj.lang.ProceedingJoinPoint;
import org.aspectj.lang.annotation.Around;
import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Pointcut;
import org.springframework.core.annotation.Order;
import org.springframework.stereotype.Component;


@Aspect
@Component
@Order(1)
public class PermissionAdvice {

    @Pointcut("@annotation(com.example.demo.PermissionsAnnotation)")
    private void permissionCheck() {
    }


    @Around("permissionCheck()")
    public Object permissionCheck(ProceedingJoinPoint joinPoint) throws Throwable {
        System.out.println("===================开始增强处理===================");

        //获取请求参数，详见接口类
        Object[] objects = joinPoint.getArgs();
        Long id = ((JSONObject) objects[0]).getLong("id");
        String name = ((JSONObject) objects[0]).getString("name");
        System.out.println("id1->>>>>>>>>>>>>>>>>>>>>>" + id);
        System.out.println("name1->>>>>>>>>>>>>>>>>>>>>>" + name);

        // 修改入参
        JSONObject object = new JSONObject();
        object.put("id", 8);
        object.put("name", "lisi");
        objects[0] = object;

        // 将修改后的参数传入
        return joinPoint.proceed(objects);
    }
}
```
同样使用JMeter调用接口，传入参数：`{"id":-5,"name":"admin"}`，响应结果表明：`@Around`截取到了接口的入参，并使接口返回了切面类中的结果。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1622974534348-ccb3c250-4dbb-4159-afd6-eac801a55cba.webp#averageHue=%2341464a&clientId=uaafa8a92-65b6-4&from=paste&id=udd5eb234&originHeight=181&originWidth=617&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=none&taskId=u0fbfdcc1-02e4-430f-9256-0184bc1c4de&title=)
<a name="F2PP6"></a>
### 3.3 `@Before`
`@Before` 注解指定的方法在切面切入目标方法之前执行，可以做一些 Log 处理，也可以做一些信息的统计，比如获取用户的请求 URL 以及用户的 IP 地址等等，这个在做个人站点的时候都能用得到，都是常用的方法。例如下面代码：
```java
@Aspect
@Component
@Slf4j
public class LogAspectHandler {
    /**
     * 在上面定义的切面方法之前执行该方法
     * @param joinPoint jointPoint
     */
    @Before("pointCut()")
    public void doBefore(JoinPoint joinPoint) {
        log.info("====doBefore方法进入了====");

        // 获取签名
        Signature signature = joinPoint.getSignature();
        // 获取切入的包名
        String declaringTypeName = signature.getDeclaringTypeName();
        // 获取即将执行的方法名
        String funcName = signature.getName();
        log.info("即将执行方法为: {}，属于{}包", funcName, declaringTypeName);

        // 也可以用来记录一些信息，比如获取请求的 URL 和 IP
        ServletRequestAttributes attributes = (ServletRequestAttributes) RequestContextHolder.getRequestAttributes();
        HttpServletRequest request = attributes.getRequest();
        // 获取请求 URL
        String url = request.getRequestURL().toString();
        // 获取请求 IP
        String ip = request.getRemoteAddr();
        log.info("用户请求的url为：{}，ip地址为：{}", url, ip);
    }
}
```
`JointPoint` 对象很有用，可以用它来获取一个签名，利用签名可以获取请求的包名、方法名，包括参数（通过 `joinPoint.getArgs()` 获取）等。
<a name="ujymW"></a>
### 3.4 `@After`
`@After` 注解和 `@Before` 注解相对应，指定的方法在切面切入目标方法之后执行，也可以做一些完成某方法之后的 Log 处理。
```java
@Aspect
@Component
@Slf4j
public class LogAspectHandler {
    /**
     * 定义一个切面，拦截 com.mutest.controller 包下的所有方法
     */
    @Pointcut("execution(* com.mutest.controller..*.*(..))")
    public void pointCut() {}

    /**
     * 在上面定义的切面方法之后执行该方法
     * @param joinPoint jointPoint
     */
    @After("pointCut()")
    public void doAfter(JoinPoint joinPoint) {

        log.info("==== doAfter 方法进入了====");
        Signature signature = joinPoint.getSignature();
        String method = signature.getName();
        log.info("方法{}已经执行完", method);
    }
}
```
到这里，来写个 Controller 测试一下执行结果，新建一个 AopController 如下：
```java
@RestController
@RequestMapping("/aop")
public class AopController {

    @GetMapping("/{name}")
    public String testAop(@PathVariable String name) {
        return "Hello " + name;
    }
}
```
启动项目，在浏览器中输入：localhost:8080/aop/test，观察一下控制台的输出信息：
```java
====doBefore 方法进入了====  
即将执行方法为: testAop，属于com.itcodai.mutest.AopController包  
用户请求的 url 为：http://localhost:8080/aop/name，ip地址为：0:0:0:0:0:0:0:1  
==== doAfter 方法进入了====  
方法 testAop 已经执行完
```
从打印出来的 Log 中可以看出程序执行的逻辑与顺序，可以很直观的掌握 `@Before` 和 `@After` 两个注解的实际作用。
<a name="umnoX"></a>
### 3.5 `@AfterReturning`
`@AfterReturning` 注解和 `@After` 有些类似，区别在于 `@AfterReturning` 注解可以用来捕获切入方法执行完之后的返回值，对返回值进行业务逻辑上的增强处理，例如：
```java
@Aspect
@Component
@Slf4j
public class LogAspectHandler {
    /**
     * 在上面定义的切面方法返回后执行该方法，可以捕获返回对象或者对返回对象进行增强
     * @param joinPoint joinPoint
     * @param result result
     */
    @AfterReturning(pointcut = "pointCut()", returning = "result")
    public void doAfterReturning(JoinPoint joinPoint, Object result) {

        Signature signature = joinPoint.getSignature();
        String classMethod = signature.getName();
        log.info("方法{}执行完毕，返回参数为：{}", classMethod, result);
        // 实际项目中可以根据业务做具体的返回值增强
        log.info("对返回参数进行业务上的增强：{}", result + "增强版");
    }
}
```
需要注意的是，在 `@AfterReturning` 注解 中，属性 `returning` 的值必须要和参数保持一致，否则会检测不到。该方法中的第二个入参就是被切方法的返回值，在 `doAfterReturning` 方法中可以对返回值进行增强，可以根据业务需要做相应的封装。重启一下服务，再测试一下：
```java
方法 testAop 执行完毕，返回参数为：Hello 
对返回参数进行业务上的增强：Hello 增强版
```
<a name="WMlTK"></a>
### 3.6 `@AfterThrowing`
当被切方法执行过程中抛出异常时，会进入 `@AfterThrowing` 注解的方法中执行，在该方法中可以做一些异常的处理逻辑。要注意的是 `throwing` 属性的值必须要和参数一致，否则会报错。该方法中的第二个入参即为抛出的异常。
```java
@Aspect
@Component
@Slf4j
public class LogAspectHandler {
    /**
     * 在上面定义的切面方法执行抛异常时，执行该方法
     * @param joinPoint jointPoint
     * @param ex ex
     */
    @AfterThrowing(pointcut = "pointCut()", throwing = "ex")
    public void afterThrowing(JoinPoint joinPoint, Throwable ex) {
        Signature signature = joinPoint.getSignature();
        String method = signature.getName();
        // 处理异常的逻辑
        log.info("执行方法{}出错，异常为：{}", method, ex);
    }
}
```
