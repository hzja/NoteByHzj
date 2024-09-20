JavaSpring
<a name="lHePI"></a>
## 1、前言
Spring 事务是通过aop的方式添加了一个事务拦截器，事务拦截器会拦截目标方法的执行，在方法执行前后添加了事务控制。<br />那么Spring事务拦截器的顺序如何控制呢，若自己也添加了一些拦截器，此时事务拦截器和自定义拦截器共存的时候，他们的顺序是怎么执行的？如何手动来控制他们的顺序？？<br />控制他们的顺序，这个功能有什么用呢？为什么要学这个<br />学会了这些，可以实现很多厉害的功能，比如<br />1、读写分离<br />2、通用幂等框架<br />3、分布式事务框架
<a name="R8375"></a>
## 2、事务拦截器顺序设置
`@EnableTransactionManagement` 注解有个 order属性，默认值是`Integer.MAX_VALUE`，用来指定事务拦截器的顺序，值越小，拦截器的优先级越高，如：
```java
@EnableTransactionManagement(order = 2)
```
下面来看案例。
<a name="BOqCz"></a>
## 3、案例
自定义2个拦截器：一个放在事务拦截器之前执行，一个放在事务拦截器之后执行

| 拦截器 | 顺序 |
| --- | --- |
| `TransactionInterceptorBefore` | 1 |
| `@EnableTransactionManagement` 事务拦截器 | 2 |
| `TransactionInterceptorAfter` | 3 |

<a name="F6M2B"></a>
### 3.1、准备sql
```sql
DROP DATABASE IF EXISTS javacode2018;
CREATE DATABASE if NOT EXISTS javacode2018;

USE javacode2018;
DROP TABLE IF EXISTS t_user;
CREATE TABLE t_user(
  id int PRIMARY KEY AUTO_INCREMENT,
  name varchar(256) NOT NULL DEFAULT '' COMMENT '姓名'
);
```
<a name="cJINk"></a>
### 3.2、Spring配置类MainConfig10
@1：开启了事务管理功能，并且设置了事务拦截器的顺序是2，Spring事务拦截器完整类名是
```
org.springframework.transaction.interceptor.TransactionInterceptor
```
@2：开启aop功能
```java
package com.javacode2018.tx.demo10;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.EnableAspectJAutoProxy;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.datasource.DataSourceTransactionManager;
import org.springframework.transaction.PlatformTransactionManager;
import org.springframework.transaction.annotation.EnableTransactionManagement;

import javax.sql.DataSource;

@Configuration //说明当前类是一个配置类
@ComponentScan //开启bean自动扫描注册功能
@EnableTransactionManagement(order = 2) //@1：设置事务拦截器的顺序是2
@EnableAspectJAutoProxy // @2：开启@Aspect Aop功能
public class MainConfig10 {
    @Bean
    public DataSource dataSource() {
        org.apache.tomcat.jdbc.pool.DataSource dataSource = new org.apache.tomcat.jdbc.pool.DataSource();
        dataSource.setDriverClassName("com.mysql.jdbc.Driver");
        dataSource.setUrl("jdbc:mysql://localhost:3306/javacode2018?characterEncoding=UTF-8");
        dataSource.setUsername("root");
        dataSource.setPassword("root123");
        dataSource.setInitialSize(5);
        return dataSource;
    }

    //定义一个jdbcTemplate
    @Bean
    public JdbcTemplate jdbcTemplate(DataSource dataSource) {
        return new JdbcTemplate(dataSource);
    }

    //定义事务管理器transactionManager
    @Bean
    public PlatformTransactionManager transactionManager(DataSource dataSource) {
        return new DataSourceTransactionManager(dataSource);
    }
}
```
<a name="vCT3T"></a>
### 3.3、定义一个有事务的Service类
addUser方法上面添加了`@Transactional`注解，表示使用Spring来管理事务，方法内部向db中插入了一条数据，为了方便分析结果，方法内部输出了2行日志
```java
package com.javacode2018.tx.demo10;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Component;
import org.springframework.transaction.annotation.Transactional;

@Component
public class UserService {
    @Autowired
    private JdbcTemplate jdbcTemplate;

    @Transactional
    public void addUser() {
        System.out.println("--------UserService.addUser start");
        this.jdbcTemplate.update("insert into t_user(name) VALUES (?)", "张三");
        System.out.println("--------UserService.addUser end");
    }
}
```
<a name="yettx"></a>
### 3.4、自定义第1个拦截器，放在事务拦截器之前执行
下面通过Aspect的方式定义了一个拦截器，顺序通过`@Order(1)`设置的是1，那么这个拦截器会在事务拦截器之前执行。
```java
package com.javacode2018.tx.demo10;

import org.aopalliance.intercept.Joinpoint;
import org.aspectj.lang.ProceedingJoinPoint;
import org.aspectj.lang.annotation.Around;
import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Pointcut;
import org.springframework.core.annotation.Order;
import org.springframework.stereotype.Component;

@Component
@Aspect
@Order(1) //@1
public class TransactionInterceptorBefore {

    @Pointcut("execution(* com.javacode2018.tx.demo10.UserService.*(..))")
    public void pointcut() {
    }

    @Around("pointcut()")
    public Object tsBefore(ProceedingJoinPoint joinPoint) throws Throwable {
        System.out.println("--------before start!!!");
        Object result = joinPoint.proceed();
        System.out.println("--------before end!!!");
        return result;
    }
}
```
<a name="jUDWt"></a>
### 3.5、自定义第2个拦截器，放在事务拦截器后面执行
这个拦截器的order是3，会在事务拦截器后面执行。
```java
package com.javacode2018.tx.demo10;

import org.aspectj.lang.ProceedingJoinPoint;
import org.aspectj.lang.annotation.Around;
import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Pointcut;
import org.springframework.core.annotation.Order;
import org.springframework.stereotype.Component;

@Component
@Aspect
@Order(2)
public class TransactionInterceptorAfter {

    @Pointcut("execution(* com.javacode2018.tx.demo10.UserService.*(..))")
    public void pointcut() {
    }

    @Around("pointcut()")
    public Object tsAfter(ProceedingJoinPoint joinPoint) throws Throwable {
        System.out.println("--------after start!!!");
        Object result = joinPoint.proceed();
        System.out.println("--------after end!!!");
        return result;
    }
}
```
<a name="D0iRl"></a>
### 3.6、添加测试类
```java
package com.javacode2018.tx.demo10;

import org.junit.Before;
import org.junit.Test;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;
import org.springframework.jdbc.core.JdbcTemplate;

public class Demo10Test {

    private UserService userService;

    private JdbcTemplate jdbcTemplate;

    @Before
    public void before() {
        AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig10.class);
        userService = context.getBean(UserService.class);
        this.jdbcTemplate = context.getBean("jdbcTemplate", JdbcTemplate.class);
        jdbcTemplate.update("truncate table t_user");
    }

    @Test
    public void test1() {
        this.userService.addUser();
    }
}
```
<a name="TeQ2J"></a>
### 3.7、分析test1方法代码执行顺序
先不执行，下分析一下test1方法执行顺序，test1方法内部会调用userService的`addUser`方法，这个方法会被3个拦截器拦截。<br />自定义的2个拦截器和事务拦截器`TransactionInterceptor`拦截，执行顺序如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690041513312-a7315ac6-42cd-474f-a63b-7a7c686c6661.png#averageHue=%23f3f3f3&clientId=u27bbc899-379f-4&from=paste&id=ud8d0f255&originHeight=719&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u998ef050-fdac-46c6-a62b-b23c4135a4e&title=)<br />下面来运行一下，看看结果和分析的是否一致。
<a name="fPnaa"></a>
### 3.8、运行test1输出
```
--------before start!!!
--------after start!!!
--------UserService.addUser start
--------UserService.addUser end
--------after end!!!
--------before end!!!
```
结果和上图中一致，大家可以在3个拦截器中设置一下断点，调试一下可以看到更详细的信息，可加深理解。
<a name="QAi8o"></a>
## 4、总结
掌握如何设置事务拦截器的顺序，`@EnableTransactionManagement` 有个 order属性，默认值是`Integer.MAX_VALUE`，用来指定事务拦截器的顺序，值越小，拦截器的优先级越高。<br />会通过这个功能实现读写分离，通用幂等性的功能。
