业务监控 javamelody
<a name="ae5c5774"></a>
## 业务监控
说明：任何一个业务系统都需要采用业务监控，抛异常，有error日志，短信预警，推送等等

1. Java内存
2. JavaCPU使用情况
3. 用户Session数量
4. JDBC连接数
5. http请求、sql请求、jsp页面与业务接口方法（EJB3、Spring、 Guice）的执行数量，平均执行时间，错误百分比等

**最终，业务代码中采用了Spring AOP进行日志拦截，把请求方法超过了1500秒的方法进行了error日志的输出：**
<a name="3jGk8"></a>
### 业务代码如下
```java
import org.apache.commons.lang.time.StopWatch;
import org.aspectj.lang.JoinPoint;
import org.aspectj.lang.ProceedingJoinPoint;
import org.aspectj.lang.annotation.After;
import org.aspectj.lang.annotation.AfterReturning;
import org.aspectj.lang.annotation.AfterThrowing;
import org.aspectj.lang.annotation.Around;
import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Before;
import org.aspectj.lang.annotation.Pointcut;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Component;
/**
 * 声明一个切面,记录每个Action的执行时间
 */
@Aspect
@Component
public class LogAspect {

    private static final Logger logger=LoggerFactory.getLogger(LogAspect.class);

    /**
     * 切入点：表示在哪个类的哪个方法进行切入。配置有切入点表达式
     */
    @Pointcut("execution(* com.netcai.admin.controller.*.*.*(..))")
    public void pointcutExpression() {
        logger.debug("配置切入点");
    }

    /**
     * 1 前置通知
     * @param joinPoint
     */
    @Before("pointcutExpression()")
    public void beforeMethod(JoinPoint joinPoint) {
        logger.debug("前置通知执行了");
    }

    /**
     * 2 后置通知
     * 在方法执行之后执行的代码. 无论该方法是否出现异常
     */
    @After("pointcutExpression()") 
    public void afterMethod(JoinPoint joinPoint) {
        logger.debug("后置通知执行了，有异常也会执行");
    }

    /**
     * 3 返回通知
     * 在方法法正常结束受执行的代码
     * 返回通知是可以访问到方法的返回值的!
     * @param joinPoint
     * @param returnValue
     */
    @AfterReturning(value = "pointcutExpression()", returning = "returnValue")
    public void afterRunningMethod(JoinPoint joinPoint, Object returnValue) {
        logger.debug("返回通知执行，执行结果：" + returnValue);
    }
    /**
     * 4 异常通知
     * 在目标方法出现异常时会执行的代码.
     * 可以访问到异常对象; 且可以指定在出现特定异常时在执行通知代码
     * @param joinPoint
     * @param e
     */
    @AfterThrowing(value = "pointcutExpression()", throwing = "e")
    public void afterThrowingMethod(JoinPoint joinPoint, Exception e)
    {
        logger.debug("异常通知, 出现异常 " + e);
    }

    /**
     * 环绕通知需要携带 ProceedingJoinPoint 类型的参数. 
     * 环绕通知类似于动态代理的全过程: ProceedingJoinPoint 类型的参数可以决定是否执行目标方法.
     * 且环绕通知必须有返回值, 返回值即为目标方法的返回值
     */
    @Around("pointcutExpression()")
    public Object aroundMethod(ProceedingJoinPoint pjd)
    {
        StopWatch clock = new StopWatch();
        //返回的结果
        Object result = null;
        //方法名称
        String className=pjd.getTarget().getClass().getName();

        String methodName = pjd.getSignature().getName();

        try 
        {
            // 计时开始
            clock.start(); 
            //前置通知
            //执行目标方法
            result = pjd.proceed();
            //返回通知
            clock.stop();
        } catch (Throwable e) 
        {
            //异常通知
            e.printStackTrace();
        }
        //后置通知
        if(!methodName.equalsIgnoreCase("initBinder"))
        {
            long constTime=clock.getTime();

            logger.info("["+className+"]"+"-" +"["+methodName+"]"+" 花费时间： " +constTime+"ms");

            if(constTime>500)
            {
                logger.error("["+className+"]"+"-" +"["+methodName+"]"+" 花费时间过长，请检查: " +constTime+"ms");
            }
        }
        return result;
    }
}
```
补充说明：这个方法记录那个类，那个方法执行的时间多少，超过设置的阀值，那么就打印error日志，需要每天进行查看与针对性的优化。
<a name="C7IIK"></a>
### 引入依赖
```xml
<!-- 系统监控 -->
<dependency>
  <groupId>net.bull.javamelody</groupId>
  <artifactId>javamelody-core</artifactId>
  <version>1.68.1</version>
</dependency>
<dependency>
  <groupId>org.jrobin</groupId>
  <artifactId>jrobin</artifactId>
  <version>1.5.9</version>
</dependency>
```
<a name="fPDxP"></a>
### web.xml文件中处理
```xml
<context-param>
  <param-name>contextConfigLocation</param-name>
  <param-value>
    classpath*:config/applicationContext.xml
    classpath*:net/bull/javamelody/monitoring-spring.xml
    classpath*:net/bull/javamelody/monitoring-spring-datasource.xml
    classpath*:net/bull/javamelody/monitoring-spring-aspectj.xml
  </param-value>
</context-param>
```
```xml
<!--javamelody -->
<filter>
  <filter-name>monitoring</filter-name>
  <filter-class>net.bull.javamelody.MonitoringFilter</filter-class>
  <async-supported>true</async-supported>
  <init-param>
    <param-name>logEnabled</param-name>
    <param-value>true</param-value>
  </init-param>
</filter>
<filter-mapping>
  <filter-name>monitoring</filter-name>
  <url-pattern>/*</url-pattern>
</filter-mapping>
<listener>
  <listener-class>net.bull.javamelody.SessionListener</listener-class>
</listener>
```
最终运营效果如下：<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1601709686831-4dde3d4e-751d-4986-a2c8-db5069374256.png#align=left&display=inline&height=507&originHeight=507&originWidth=855&size=0&status=done&style=shadow&width=855)<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1601709686861-eadf7a3b-39c1-48fa-8843-8164fb1303c2.png#align=left&display=inline&height=415&originHeight=415&originWidth=1080&size=0&status=done&style=shadow&width=1080)![](https://cdn.nlark.com/yuque/0/2020/png/396745/1601709686878-be594a54-a590-405e-a8d1-d20e66030865.png#align=left&display=inline&height=364&originHeight=364&originWidth=1080&size=0&status=done&style=shadow&width=1080)
