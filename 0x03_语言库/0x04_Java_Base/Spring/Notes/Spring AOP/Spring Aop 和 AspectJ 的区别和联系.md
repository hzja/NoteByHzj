JavaSpringAOPAspectJ
<a name="lIIaa"></a>
## 区别
<a name="UV8UC"></a>
### AspectJ
AspectJ是一个面向切面的框架，它扩展了Java语言。AspectJ定义了AOP语法，所以它有一个专门的编译器用来生成遵守Java字节编码规范的Class文件。
<a name="EbWjt"></a>
### Spring aop
Spring提供了四种类型的Aop支持

- 基于经典的SpringAOP
- 纯POJO切面
- `@ASpectJ`注解驱动的切面
- 注入式AspectJ切面（其实与Spring并无多大的关系，这个就是使用AspectJ这个框架实现Aop编程）
<a name="Dn6oQ"></a>
## 基于经典的SpringAop
其使用`ProxyFactoryBean`创建。<br />增强（通知）的类型有：

- 前置通知：`org.springframework.aop.MethodBeforeAdvice`
- 后置通知：`org.springframework.aop.AfterReturningAdvice`
- 环绕通知：`org.aopalliance.intercept.MethodInterceptor`
- 异常通知：`org.springframework.aop.ThrowsAdvice`
```java
public interface IBookDao {
    public int add()
    public int delete();
}
 
public class BookDaoImpl implements IBookDao{
    public int add() {
        System.out.println("正在添加图书...");
        return 0;
    }
    public int delete() {
        System.out.println("正在删除图书...");
        return 0;
    }
}
 
//实现了MethodInterceptor的环绕增强类
public class MyAdvice implements MethodInterceptor{
 
    public Object invoke(MethodInvocation invocation) throws Throwable {
        System.out.println("Around Advice before method invocation");
        Object o = invocation.proceed();
        System.out.println("Around Advice after method invocation");
        return o;
    }
}
```
将每一个连接点都当做切点（拦截每一个方法）
```xml
<bean id="bookDao" class="com.njust.learning.spring.service.BookDaoImpl"></bean>

<bean id="myadvice" class="com.njust.learning.spring.aop.MyAdvice"></bean>

<bean id="bookDaoProxy" class="org.springframework.aop.framework.ProxyFactoryBean">
	<property name="target" ref="bookDao"/>
	<property name="proxyInterfaces" value="com.njust.learning.spring.service.IBookDao"/>
	<property name="interceptorNames" value="myadvice"/>
</bean>
```
使用`RegexMethodPointcutAdvisor`针对某些特定的方法进行拦截增强
```xml
<bean id="bookDao" class="com.njust.learning.spring.service.BookDaoImpl"></bean>

<bean id="myadvice" class="com.njust.learning.spring.aop.MyAdvice"></bean>


<bean id="rmpAdvisor" class="org.springframework.aop.support.RegexpMethodPointcutAdvisor">
	<!--patterns,如果有多个指定的值的话，可以使用，隔开，例如value=".*add,.*delete"-->
	<property name="patterns" value=".*add"/>
	<property name="advice" ref="myadvice"/>
</bean>
<!--使用的时候使用这个id，而不是原始的那个id-->
<bean id="bookDaoProxy" class="org.springframework.aop.framework.ProxyFactoryBean">
	<property name="target" ref="bookDao"/>
	<property name="proxyInterfaces" value="com.njust.learning.spring.service.IBookDao"/>
	<property name="interceptorNames" value="rmpAdvisor"/>
</bean>
```
注意：像上面这样，每定义一个dao都需要定义一个`ProxyFactoryBean`，显得很麻烦，所以引入自动代理，也就是自动创建代理对象。
<a name="t3Pmd"></a>
### `BeanNameAutoProxyCreator`
```xml
<bean id="bookDao" class="com.njust.learning.spring.service.BookDaoImpl"></bean>

<bean id="myadvice" class="com.njust.learning.spring.aop.MyAdvice"></bean>

<bean id="rmpAdvisor" class="org.springframework.aop.support.RegexpMethodPointcutAdvisor">
	<!--patterns,如果有多个指定的值的话，可以使用，隔开，例如value=".*add,.*delete"-->
	<property name="patterns" value=".*add"/>
	<property name="advice" ref="myadvice"/>
</bean>

<bean class="org.springframework.aop.framework.autoproxy.BeanNameAutoProxyCreator">
	<property name="beanNames" value="*Dao"></property>
	<property name="interceptorNames" value="rmpAdvisor"></property>
</bean>
```
<a name="UPQvZ"></a>
### `DefaultAdvisorAutoProxyCreator`
```xml
<bean id="bookDao" class="com.njust.learning.spring.service.BookDaoImpl"></bean>

<bean id="myadvice" class="com.njust.learning.spring.aop.MyAdvice"></bean>

<bean id="rmpAdvisor" class="org.springframework.aop.support.RegexpMethodPointcutAdvisor">
	<!--patterns,如果有多个指定的值的话，可以使用，隔开，例如value=".*add,.*delete"-->
	<property name="patterns" value=".*add"/>
	<property name="advice" ref="myadvice"/>
</bean>

<!--根据切面中生成信息生成代理-->
<bean class="org.springframework.aop.framework.autoproxy.DefaultAdvisorAutoProxyCreator"/>
```
<a name="Mesrh"></a>
### 纯POJO切面，需要使用XML进行配置
```java
public interface IBookDao {
    public int add();
    public int delete();
}
 
public class BookDaoImpl implements IBookDao{
 
    public int add() {
        int a = 1/0;
        System.out.println("正在添加图书...");
        return 0;
    }
 
    public int delete() {
        System.out.println("正在删除图书...");
        return 0;
    }
}
```
```java
public class PojoAdvice {
	public void before(){
		System.out.println("前置通知");
	}
	public void after(Object returnval){
		System.out.println("后置通知"+"，处理后的结果为："+returnval);
	}
	public Object around(ProceedingJoinPoint proceedingJoinPoint) throws Throwable {
		System.out.println("环绕前置增强...");
		Object o = proceedingJoinPoint.proceed();
		System.out.println("环绕后置增强...");
		return o;
	}
	public void afterThrowing(Throwable e){
		System.out.println("异常通知:"+e.getMessage());
	}
}
```
```xml
<bean id="bookDao" class="com.njust.learning.spring.service.BookDaoImpl"></bean>

<bean id="pojoAdvice" class="com.njust.learning.spring.pojoaop.PojoAdvice"></bean>

<aop:config>
	<aop:pointcut id="p" expression="execution (* *.add(..))"/>
	<aop:aspect ref="pojoAdvice">
		<aop:before method="before" pointcut-ref="p"></aop:before>
		<!--通过设置returning来将返回值传递给通知-->
		<aop:after-returning method="after" pointcut-ref="p" returning="returnval"/>
		<aop:around method="around" pointcut-ref="p"/>
		<!--通过设置returning来将异常对象传递给通知-->
		<aop:after-throwing method="afterThrowing" pointcut-ref="p" throwing="e"/>
	</aop:aspect>
</aop:config>
```
<a name="GH33M"></a>
## 联系
借助于Spring Aop的命名空间可以将纯POJO转换为切面，实际上这些POJO只是提供了满足切点的条件时所需要调用的方法，但是，这种技术需要XML进行配置，不能支持注解。<br />所以Spring借鉴了AspectJ的切面，以提供注解驱动的AOP，本质上它依然是Spring基于代理的AOP，只是编程模型与AspectJ完全一致，这种风格的好处就是不需要使用XML进行配置。<br />使用`@Aspect`方式，就可以在类上直接一个`@Aspect`就搞定，不用费事在xml里配了。但是这需要额外的jar包（aspectjweaver.jar）。因为Spring直接使用AspectJ的注解功能，注意只是使用了它 的注解功能而已，并不是核心功能 。<br />SpringAop的底层技术依然是Jdk动态代理和Cglib。
