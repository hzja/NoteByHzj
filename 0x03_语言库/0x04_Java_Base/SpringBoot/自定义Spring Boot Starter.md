SpringBoot SpringBoot Starter<br />下面定义一个starter，它将实现，在日志中打印方法执行时间。
<a name="ZpUoB"></a>
### 1、创建Maven项目
<a name="JBNY1"></a>
#### 项目命名规则
:::info
spring-boot-starter-XX是springboot官方的starter<br />XX-spring-boot-starter是第三方扩展的starter
:::
<a name="aIEHK"></a>
#### 项目的pom依赖文件
```xml
<?xml version="1.0" encoding="UTF-8" ?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <groupId>com.fcant</groupId>
    <artifactId>aspectlog-spring-boot-starter</artifactId>
    <version>1.0.2</version>
    <parent>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-parent</artifactId>
        <version>2.1.15.RELEASE</version>
    </parent>
    <dependencies>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-autoconfigure</artifactId>
        </dependency>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-aop</artifactId>
        </dependency>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-configuration-processor</artifactId>
            <optional>true</optional>
        </dependency>
    </dependencies>
</project>
```
关于spring-boot-configuration-processor的说明，引自springBoot官方文档：
> Spring Boot uses an annotation processor to collect the conditions on auto-configurations in a metadata file ( META-INF/spring-autoconfigure-metadata.properties ). If that file is present, it is used to eagerly filter auto-configurations that do not match, which will improve startup time. It is recommended to add the following dependency in a module that contains auto-configurations:

```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-autoconfigure-processor</artifactId>
    <optional>true</optional>
</dependency>
```
简单说就是 写starter时，在pom中配置spring-boot-autoconfigure-processor， 在编译时会自动收集配置类的条件，写到一个META-INF/spring-autoconfigure-metadata.properties中
<a name="Pbg6G"></a>
### 2、写自动配置逻辑
各种condition

| 类型 | 注解 | 说明 |
| --- | --- | --- |
| Class Conditions类条件注解 | `@ConditionalOnClass` | 当前classpath下有指定类才加载 |
| <br /> | `@ConditionalOnMissingClass` | 当前classpath下无指定类才加载 |
| Bean ConditionsBean条件注解 | `@ConditionalOnBean` | 当期容器内有指定bean才加载 |
|  | `@ConditionalOnMissingBean` | 当期容器内无指定bean才加载 |
| Property Conditions环境变量条件注解（含配置文件） | `@ConditionalOnProperty` | prefix 前缀name 名称havingValue 用于匹配配置项值matchIfMissing 没找指定配置项时的默认值 |
| ResourceConditions 资源条件注解 | `@ConditionalOnResource` | 有指定资源才加载 |
| Web Application Conditionsweb条件注解 | `@ConditionalOnWebApplication` | 是web才加载 |
|  | `@ConditionalOnNotWebApplication` | 不是web才加载 |
| SpEL Expression Conditions | `@ConditionalOnExpression` | 符合SpEL 表达式才加载 |

选用 `@ConditionalOnProperty` 。即配置文件中有 `aspectLog.enable=true` ，才加载我们的配置类。下面开始写自动配置类
<a name="Zgn4P"></a>
#### 2.1定义AspectLog注解，该注解用于标注需要打印执行时间的方法
```java
import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

/**
 * class_name: ScheduleManage
 * describe:   用于控制定时任务的开启与关闭
 * 对应切面
 **/

@Target(ElementType.METHOD)@ Retention(RetentionPolicy.RUNTIME)
public@ interface AspectLog {}
```
<a name="Vmv39"></a>
#### 2.2定义配置文件对应类
```java
import org.springframework.boot.context.properties.ConfigurationProperties;@
ConfigurationProperties("aspectLog")
public class AspectLogProperties {
    private boolean enable;
    public boolean isEnable() {
        return enable;
    }
    public void setEnable(boolean enable) {
        this.enable = enable;
    }
}
```
<a name="3c10f957"></a>
#### 2.3定义自动配置类
```java
import org.aspectj.lang.ProceedingJoinPoint;
import org.aspectj.lang.annotation.Around;
import org.aspectj.lang.annotation.Aspect;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.boot.autoconfigure.condition.*;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.EnableAspectJAutoProxy;
import org.springframework.core.PriorityOrdered;

@Aspect
@EnableAspectJAutoProxy(exposeProxy = true, proxyTargetClass = true)
@Configuration
@ConditionalOnProperty(prefix = "aspectLog", name = "enable",
                     havingValue = "true", matchIfMissing = true)
public class AspectLogAutoConfiguration implements PriorityOrdered {

    protected Logger logger = LoggerFactory.getLogger(getClass());

@Around("@annotation(com.fcant.autoconfiguration.aspectlog.AspectLog) ")
    public Object isOpen(ProceedingJoinPoint thisJoinPoint) 
                                        throws Throwable {
        //执行方法名称 
        String taskName = thisJoinPoint.getSignature()
            .toString().substring(
                thisJoinPoint.getSignature()
                    .toString().indexOf(" "), 
                    thisJoinPoint.getSignature().toString().indexOf("("));
        taskName = taskName.trim();
        long time = System.currentTimeMillis();
        Object result = thisJoinPoint.proceed();
        logger.info("method:{} run :{} ms", taskName, 
                            (System.currentTimeMillis() - time));
        return result;
    }
    @Override
    public int getOrder() {
        //保证事务等切面先执行
        return Integer.MAX_VALUE;
    }
}
```
配置类简要说明：<br />`@ConditionalOnProperty(prefix = "aspectLog", name = "enable",havingValue = "true", matchIfMissing = true)`<br />当配置文件有 `aspectLog.enable=true` 时开启，如果配置文件没有设置aspectLog.enable也开启。
<a name="Wh69z"></a>
### 3、META-INF/spring.factories
META-INF/spring.factories是spring的工厂机制，在这个文件中定义的类，都会被自动加载。多个配置使用逗号分割，换行用\
```java
org.springframework.boot.autoconfigure.EnableAutoConfiguration=\
com.fcant.autoconfiguration.aspectlog.AspectLogAutoConfiguration
```
<a name="Gi8do"></a>
### 4、执行打包测试
使用 `mvn intall` 命令对工程进行打包
```bash
mvn intall
```
打包完成后，在其他项目中的pom中引入进行测试
