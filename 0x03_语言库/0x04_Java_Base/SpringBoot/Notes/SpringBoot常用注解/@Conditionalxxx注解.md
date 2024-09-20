JavaSpringBoot<br />`@Conditionalxxx`这类注解表示某种判断条件成立时才会执行相关操作。掌握该类注解，有助于日常开发，框架的搭建。来介绍一下该类注解。
<a name="tM5tc"></a>
## **Spring Boot 版本**
本文基于的Spring Boot的版本是2.3.4.RELEASE。
<a name="DntQS"></a>
## `**@Conditional**`
`@Conditional`注解是从Spring4.0才有的，可以用在任何类型或者方法上面，通过`@Conditional`注解可以配置一些条件判断，当所有条件都满足的时候，被`@Conditional`标注的目标才会被Spring容器处理。<br />`@Conditional`的使用很广，比如控制某个Bean是否需要注册，在Spring Boot中的变形很多，比如`@ConditionalOnMissingBean`、`@ConditionalOnBean`等等，如下：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1654223317027-034a0ecb-2ffa-4370-9a57-fe0accc7dc88.png#clientId=u2e5339d4-a94e-4&from=paste&height=640&id=u152406a2&originHeight=1600&originWidth=1040&originalType=binary&ratio=1&rotation=0&showTitle=false&size=142592&status=done&style=none&taskId=udec2f7e2-e1cc-45e0-9bfe-099f2e57851&title=&width=416)<br />该注解的源码其实很简单，只有一个属性value，表示判断的条件（一个或者多个），是`org.springframework.context.annotation.Condition`类型，源码如下：
```java
@Target({ElementType.TYPE, ElementType.METHOD})
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface Conditional {

	/**
  * All {@link Condition} classes that must {@linkplain Condition#matches match}
  * in order for the component to be registered.
  */
	Class<? extends Condition>[] value();
}
```
`@Conditional`注解实现的原理很简单，就是通过`org.springframework.context.annotation.Condition`这个接口判断是否应该执行操作。
<a name="IQCe8"></a>
## `**Condition**`**接口**
`@Conditional`注解判断条件与否取决于value属性指定的`Condition`实现，其中有一个`matches()`方法，返回true表示条件成立，反之不成立，接口如下：
```java
@FunctionalInterface
public interface Condition {
	boolean matches(ConditionContext context, AnnotatedTypeMetadata metadata);
}
```
**matches中的两个参数如下：**

1. `**context**`**：条件上下文，**`**ConditionContext**`**接口类型的，可以用来获取容器中上下文信息。**
2. `**metadata**`**：用来获取被**`**@Conditional**`**标注的对象上的所有注解信息**
<a name="YEEiL"></a>
### ConditionContext接口
这个接口很重要，能够从中获取Spring上下文的很多信息，比如`ConfigurableListableBeanFactory`，源码如下：
```java
public interface ConditionContext {

	/**
     * 返回bean定义注册器，可以通过注册器获取bean定义的各种配置信息
     */
	BeanDefinitionRegistry getRegistry();

	/**
     * 返回ConfigurableListableBeanFactory类型的bean工厂，相当于一个ioc容器对象
     */
	@Nullable
	ConfigurableListableBeanFactory getBeanFactory();

	/**
     * 返回当前spring容器的环境配置信息对象
     */
	Environment getEnvironment();

	/**
     * 返回资源加载器
     */
	ResourceLoader getResourceLoader();

	/**
     * 返回类加载器
     */
	@Nullable
	ClassLoader getClassLoader();
}
```
<a name="t08Gi"></a>
## **如何自定义Condition？**
举个栗子：**假设有这样一个需求，需要根据运行环境注入不同的Bean，Windows环境和Linux环境注入不同的Bean。**<br />实现很简单，分别定义不同环境的判断条件，实现`org.springframework.context.annotation.Condition`即可。<br />**windows环境的判断条件源码如下**：
```java
/**
 * 操作系统的匹配条件，如果是windows系统，则返回true
 */
public class WindowsCondition implements Condition {
    @Override
    public boolean matches(ConditionContext conditionContext, AnnotatedTypeMetadata metadata) {
        //获取当前环境信息
        Environment environment = conditionContext.getEnvironment();
        //获得当前系统名
        String property = environment.getProperty("os.name");
        //包含Windows则说明是windows系统，返回true
        if (property.contains("Windows")){
            return true;
        }
        return false;

    }
}
```
**Linux环境判断源码如下**：
```java
/**
 * 操作系统的匹配条件，如果是windows系统，则返回true
 */
public class LinuxCondition implements Condition {
	@Override
	public boolean matches(ConditionContext conditionContext, AnnotatedTypeMetadata metadata) {
		Environment environment = conditionContext.getEnvironment();

		String property = environment.getProperty("os.name");
		if (property.contains("Linux")){
			return true;
		}
		return false;

	}
}
```
**配置类中结合**`**@Bean**`**注入不同的Bean，如下**：
```java
@Configuration
public class CustomConfig {

	/**
     * 在Windows环境下注入的Bean为winP
     * @return
     */
	@Bean("winP")
	@Conditional(value = {WindowsCondition.class})
	public Person personWin(){
		return new Person();
	}

	/**
     * 在Linux环境下注入的Bean为LinuxP
     * @return
     */
	@Bean("LinuxP")
	@Conditional(value = {LinuxCondition.class})
	public Person personLinux(){
		return new Person();
	}
}
```
**简单的测试一下，如下**：
```java
@SpringBootTest
class SpringbootInterceptApplicationTests {

	@Autowired(required = false)
	@Qualifier(value = "winP")
	private Person winP;

	@Autowired(required = false)
	@Qualifier(value = "LinuxP")
	private Person linP;

	@Test
	void contextLoads() {
		System.out.println(winP);
		System.out.println(linP);
	}
}
```
**Windows环境下执行单元测试，输出如下**：
```java
com.example.springbootintercept.domain.Person@885e7ff
null
```
很显然，判断生效了，Windows环境下只注入了WINP。
<a name="lYCEU"></a>
## **条件判断在什么时候执行？**
条件判断的执行分为两个阶段，如下：

1. **配置类解析阶段(**`**ConfigurationPhase.PARSE_CONFIGURATION**`**)：在这个阶段会得到一批配置类的信息和一些需要注册的Bean。**
2. **Bean注册阶段(**`**ConfigurationPhase.REGISTER_BEAN**`**)：将配置类解析阶段得到的配置类和需要注册的Bean注入到容器中。**

默认都是配置解析阶段，其实也就够用了，但是在Spring Boot中使用了`ConfigurationCondition`，这个接口可以自定义执行阶段，比如`@ConditionalOnMissingBean`都是在Bean注册阶段执行，因为需要从容器中判断Bean。<br />**这个两个阶段有什么不同呢？**：其实很简单的，配置类解析阶段只是将需要加载配置类和一些Bean（被`@Conditional`注解过滤掉之后）收集起来，而Bean注册阶段是将的收集来的Bean和配置类注入到容器中，**如果在配置类解析阶段执行Condition接口的**`**matches()**`**接口去判断某些Bean是否存在IOC容器中，这个显然是不行的，因为这些Bean还未注册到容器中**。<br />**什么是配置类，有哪些？**：类上被`@Component`、 `@ComponentScan`、`@Import`、`@ImportResource`、`@Configuration`标注的以及类中方法有`@Bean`的方法。如何判断配置类，在源码中有单独的方法：`org.springframework.context.annotation.ConfigurationClassUtils#isConfigurationCandidate`。
<a name="SWbaK"></a>
## `**ConfigurationCondition**`**接口**
这个接口相比于`@Condition`接口就多了一个`getConfigurationPhase()`方法，可以自定义执行阶段。源码如下：
```java
public interface ConfigurationCondition extends Condition {

	/**
     * 条件判断的阶段，是在解析配置类的时候过滤还是在创建bean的时候过滤
     */
	ConfigurationPhase getConfigurationPhase();


	/**
     * 表示阶段的枚举：2个值
     */
	enum ConfigurationPhase {

		/**
         * 配置类解析阶段，如果条件为false，配置类将不会被解析
         */
		PARSE_CONFIGURATION,

		/**
         * bean注册阶段，如果为false，bean将不会被注册
         */
		REGISTER_BEAN
	}
}
```
这个接口在需要指定执行阶段的时候可以实现，比如需要根据某个Bean是否在IOC容器中来注入指定的Bean，则需要指定执行阶段为**Bean的注册阶段**（`ConfigurationPhase.REGISTER_BEAN`）。
<a name="Jh1A9"></a>
## **多个**`**Condition**`**的执行顺序**
`@Conditional`中的`Condition`判断条件可以指定多个，默认是按照先后顺序执行，如下：
```java
class Condition1 implements Condition {
	@Override
	public boolean matches(ConditionContext context, AnnotatedTypeMetadata metadata) {
		System.out.println(this.getClass().getName());
		return true;
	}
}

class Condition2 implements Condition {
	@Override
	public boolean matches(ConditionContext context, AnnotatedTypeMetadata metadata) {
		System.out.println(this.getClass().getName());
		return true;
	}
}

class Condition3 implements Condition {
	@Override
	public boolean matches(ConditionContext context, AnnotatedTypeMetadata metadata) {
		System.out.println(this.getClass().getName());
		return true;
	}
}

@Configuration
@Conditional({Condition1.class, Condition2.class, Condition3.class})
public class MainConfig5 {
}
```
**上述例子会依次按照Condition1、Condition2、Condition3执行。**<br />默认按照先后顺序执行，但是需要指定顺序呢？很简单，有如下三种方式：

1. **实现**`**PriorityOrdered**`**接口，指定优先级**
2. **实现**`**Ordered**`**接口接口，指定优先级**
3. **使用**`**@Order**`**注解来指定优先级**

例子如下：
```java
@Order(1) 
class Condition1 implements Condition {
	@Override
	public boolean matches(ConditionContext context, AnnotatedTypeMetadata metadata) {
		System.out.println(this.getClass().getName());
		return true;
	}
}

class Condition2 implements Condition, Ordered { 
	@Override
	public boolean matches(ConditionContext context, AnnotatedTypeMetadata metadata) {
		System.out.println(this.getClass().getName());
		return true;
	}

	@Override
	public int getOrder() { 
		return 0;
	}
}

class Condition3 implements Condition, PriorityOrdered {
	@Override
	public boolean matches(ConditionContext context, AnnotatedTypeMetadata metadata) {
		System.out.println(this.getClass().getName());
		return true;
	}

	@Override
	public int getOrder() {
		return 1000;
	}
}

@Configuration
@Conditional({Condition1.class, Condition2.class, Condition3.class})
public class MainConfig6 {
}
```
根据排序的规则，`PriorityOrdered`的会排在前面，然后会再按照order升序，最后可以顺序是：Condtion3->Condtion2->Condtion1
<a name="qBnI7"></a>
## **Spring Boot中常用的一些注解**
Spring Boot中大量使用了这些注解，常见的注解如下：

1. `**@ConditionalOnBean**`**：当容器中有指定Bean的条件下进行实例化。**
2. `**@ConditionalOnMissingBean**`**：当容器里没有指定Bean的条件下进行实例化。**
3. `**@ConditionalOnClass**`**：当classpath类路径下有指定类的条件下进行实例化。**
4. `**@ConditionalOnMissingClass**`**：当类路径下没有指定类的条件下进行实例化。**
5. `**@ConditionalOnWebApplication**`**：当项目是一个Web项目时进行实例化。**
6. `**@ConditionalOnNotWebApplication**`**：当项目不是一个Web项目时进行实例化。**
7. `**@ConditionalOnProperty**`**：当指定的属性有指定的值时进行实例化。**
8. `**@ConditionalOnExpression**`**：基于SpEL表达式的条件判断。**
9. `**@ConditionalOnJava**`**：当JVM版本为指定的版本范围时触发实例化。**
10. `**@ConditionalOnResource**`**：当类路径下有指定的资源时触发实例化。**
11. `**@ConditionalOnJndi**`**：在JNDI存在的条件下触发实例化。**
12. `**@ConditionalOnSingleCandidate**`**：当指定的Bean在容器中只有一个，或者有多个但是指定了首选的Bean时触发实例化。**

比如在WEB模块的自动配置类`WebMvcAutoConfiguration`下有这样一段代码：
```java
@Bean
@ConditionalOnMissingBean
public InternalResourceViewResolver defaultViewResolver() {
	InternalResourceViewResolver resolver = new InternalResourceViewResolver();
	resolver.setPrefix(this.mvcProperties.getView().getPrefix());
	resolver.setSuffix(this.mvcProperties.getView().getSuffix());
	return resolver;
}
```
常见的`@Bean`和`@ConditionalOnMissingBean`注解结合使用，意思是当容器中没有`InternalResourceViewResolver`这种类型的Bean才会注入。这样写有什么好处呢？好处很明显，可以让开发者自定义需要的视图解析器，如果没有自定义，则使用默认的，这就是Spring Boot为自定义配置提供的便利。
<a name="DGdXx"></a>
## **总结**
`@Conditional`注解在Spring Boot中演变的注解很多，需要着重了解，特别是后期框架整合的时候会大量涉及。
