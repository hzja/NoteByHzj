Java Spring AOP
<a name="Cfslo"></a>
## ①导入Jar
```xml
<dependencies>
  <dependency>
    <groupId>org.aspectj</groupId>
    <artifactId>aspectjweaver</artifactId>
    <version>1.9.4</version>
  </dependency>
</dependencies>
```
<a name="mo7UM"></a>
## ②配置文件开启aop文件头引用约束
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:aop="http://www.springframework.org/schema/aop"
       xsi:schemaLocation="http://www.springframework.org/schema/beans
       http://www.springframework.org/schema/beans/spring-beans.xsd
       http://www.springframework.org/schema/aop
       http://www.springframework.org/schema/aop/spring-aop.xsd">
</beans>
```
<a name="fxTIX"></a>
## ③抽象接口
```java
public interface UserService {
    void add();
    void delete();
    void update();
    void select();
}
```
<a name="JYylx"></a>
## ④实现类
```java
public class UserServiceImpl implements UserService {
    @Override
    public void add() {
        System.out.println("增加用户");
    }

    @Override
    public void delete() {
        System.out.println("删除用户");
    }

    @Override
    public void update() {
        System.out.println("更新用户");
    }

    @Override
    public void select() {
        System.out.println("查询用户");
    }
}
```
<a name="R0rf6"></a>
### 一、通过 Spring API 实现
<a name="SBFLT"></a>
#### ①添加前置后置方法
```java
public class AopBeforeConfigLog implements MethodBeforeAdvice {

    /**
     * method : 要执行的目标对象的方法
     * args : 被调用的方法的参数
     * target : 目标对象
     */
    @Override
    public void before(Method method, Object[] args, Object target) throws Throwable {
        System.out.println(target.getClass()+"===="+method.getName()+"被执行了");
    }
}
```
<a name="eeUzI"></a>
#### ②beans.xml
```xml
<!--注册bean-->
<bean id="userService" class="service.UserServiceImpl"/>
<bean id="afterLog" class="aoptest.AopAfterConfigLog"/>
<bean id="beforeAop" class="aoptest.AopBeforeConfigLog"/>

<aop:config>
  <!--切入点  expression:表达式匹配要执行的方法-->
  <aop:pointcut id="cut" expression="execution(* service.UserServiceImpl.*(..))"/>

  <!--执行环绕; advice-ref执行方法 . pointcut-ref切入点-->
  <aop:advisor advice-ref="afterLog" pointcut-ref="cut"/>
  <aop:advisor advice-ref="beforeAop" pointcut-ref="cut"/>
</aop:config>
```
<a name="SYSnU"></a>
#### ③测试类
```java
public class AopAfterConfigLog implements AfterReturningAdvice {

    /**
     * method : 要执行的目标对象的方法
     * args : 被调用的方法的参数
     * result : 返回值
     * target : 被调用的目标对象
     */

    @Override
    public void afterReturning(Object result, Method method, Object[] args, Object target){
        System.out.println(target.getClass()+"===="+method.getName()+"被执行了"+"===返回值"+result);
    }
}
```
<a name="P4GdV"></a>
#### ④执行结果
<a name="iEYz3"></a>
#### ⑤总结：

- 切面：横切的点
- 通知：切面完成的工作，其实就是一个方法
- 目标：被通知的对象
- `expression="execution(* service.UserServiceImpl.*(..))"` 这是一套固定的公式 *代表所有 这句话的意思就是`service.UserServiceImpl`下的所有方法都被切面了！
<a name="cqWoZ"></a>
### 二、自定义类来实现
<a name="sMdvi"></a>
#### ①自定义被切入类
```java
/*
* 自定义类
* */
public class MyDIYAopCut {
    public void before(){
        System.out.println("方法执行前前前前前前前前前前");
    }
    public void after(){
        System.out.println("方法执行后后后后后后后后后后");
    }
}
```
<a name="SoTDA"></a>
#### ②beans.xml
```xml
<!--注册bean-->
<bean id="userService" class="service.UserServiceImpl"/>
<bean id="Mydiycut" class="diyaop.MyDIYAopCut"/>

<aop:config>
  <!--这里的ref指定被 切入 的类是哪一个-->
  <aop:aspect ref="Mydiycut">
    <!--切入点-->
    <aop:pointcut id="cut" expression="execution(* service.UserServiceImpl.*(..))"/>

    <!--切入点之前执行,这里的方法名即是我们自定义类中的方法名-->
    <aop:before method="before" pointcut-ref="cut"/>
    <!--切入点之后执行,这里的方法名即是我们自定义类中的方法名-->
    <aop:before method="after" pointcut-ref="cut"/>
  </aop:aspect>
</aop:config>
```
<a name="b3iWn"></a>
#### ③测试类不变，执行结果
![2021-08-31-23-18-37-009092.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630423178087-f3f6d1aa-df4a-4c0c-aba1-0064ab29e9ad.jpeg#clientId=uccf9371d-b338-4&from=ui&id=u9d5a4214&originHeight=341&originWidth=388&originalType=binary&ratio=1&size=19504&status=done&style=none&taskId=u069d5dde-c42a-4aba-b486-85683910d3e)
<a name="caUy2"></a>
#### ④总结：

- 测试类的方法即是xml中method的方法名

![2021-08-31-23-18-37-121093.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630423131812-9dd4cd61-a1c4-4f39-839b-1f494e6cd103.jpeg#clientId=uccf9371d-b338-4&from=ui&id=u565fc29b&originHeight=302&originWidth=640&originalType=binary&ratio=1&size=16074&status=done&style=none&taskId=u4a9d268d-9475-469a-93cc-0fabfa89d1d)

- 其他见xml中的注释！
<a name="X4Z0w"></a>
### 三、使用注解实现
<a name="TN9Yw"></a>
#### ①自定义增强注解实现类
```java

@Aspect
public class MyAnnotationAopCut {
    @Before("execution(* service.UserServiceImpl.*(..))")
    public void before(){
        System.out.println("方法执行前");
    }
    @After("execution(* service.UserServiceImpl.*(..))")
    public void after(){
        System.out.println("方法执行后");
    }
    @Around("execution(* service.UserServiceImpl.*(..))")
    public void around(ProceedingJoinPoint point) throws Throwable {
        System.out.println("环绕前=="+point.getSignature());
        //执行
        Object proceed = point.proceed();
        System.out.println("环绕后=="+point.getSignature());
    }
}
```
<a name="ojx36"></a>
#### ②xml
```xml
<!--注册bean-->
<bean id="userService" class="service.UserServiceImpl"/>
<bean id="myAnnotationAopCut" class="diyaop.MyAnnotationAopCut"/>
<!--声明自动为spring容器中那些配置@aspectJ切面的bean创建代理，织入切面-->
<aop:aspectj-autoproxy proxy-target-class="false"/>
```
<a name="EFV2Y"></a>
#### ③测试类一致，执行结果
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630423096305-182a4381-b171-4564-9cc3-913f61903cd5.png#clientId=uccf9371d-b338-4&from=paste&height=368&id=uc8d74781&originHeight=1103&originWidth=2553&originalType=binary&ratio=1&size=350655&status=done&style=shadow&taskId=uc0e529a7-089f-4d2d-99bb-dd0b4397423&width=851)
