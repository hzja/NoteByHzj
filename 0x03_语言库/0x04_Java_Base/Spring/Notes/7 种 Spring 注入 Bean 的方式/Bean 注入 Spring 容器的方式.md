Java Spring
<a name="NF7Tc"></a>
## xml 方式
最早接触Spring的时候，用的还是SSH框架，不知道大家对这个还有印象吗？所有的bean的注入得依靠xml文件来完成。<br />它的注入方式分为：set方法注入、构造方法注入、字段注入，而注入类型分为值类型注入（8种基本数据类型）和引用类型注入（将依赖对象注入）。<br />以下是set方法注入的简单样例
```xml
<bean name="teacher" class="org.springframework.demo.model.Teacher">
	<property name="name" value="阿Q"></property>
</bean>
```
对应的实体类代码
```java
public class Teacher {
	
	private String name;
	
	public void setName(String name) {
		this.name = name;
	}
}
```
**xml方式存在的缺点如下：**

1. xml文件配置起来比较麻烦，既要维护代码又要维护配置文件，开发效率低；
2. 项目中配置文件过多，维护起来比较困难；
3. 程序编译期间无法对配置项的正确性进行验证，只能在运行期发现并且出错之后不易排查；
4. 解析xml时，无论是将xml一次性装进内存，还是一行一行解析，都会占用内存资源，影响性能。
<a name="VNYI4"></a>
## 注解方式
随着Spring的发展，Spring 2.5开始出现了一系列注解，除了经常使用的`@Controller`、`@Service`、`@Repository`、`@Component` 之外，还有一些比较常用的方式，接下来简单了解下。
<a name="wVNnZ"></a>
### `@Configuration` + `@Bean`
当需要引入第三方的jar包时，可以用`@Bean`注解来标注，同时需要搭配`@Configuration`来使用。

- `@Configuration`用来声明一个配置类，可以理解为xml的`<beans>`标签
- `@Bean` 用来声明一个bean，将其加入到Spring容器中，可以理解为xml的`<bean>`标签

**简单样例：将 RedisTemplate 注入 Spring**
```java
@Configuration
public class RedisConfig {
    @Bean
    public RedisTemplate<String, Object> redisTemplate(LettuceConnectionFactory redisConnectionFactory) {
        RedisTemplate<String, Object> redisTemplate = new RedisTemplate<String, Object>();
        ......
        return redisTemplate;
    }
}
```
<a name="GrOQ0"></a>
### `@Import`
在翻看Spring源码的过程中，经常会看到`@Import`注解，它也可以用来将第三方jar包注入Spring，但是它只可以作用在**类**上。<br />例如在注解`EnableSpringConfigured`上就包含了`@Import`注解，用于将`SpringConfiguredConfiguration`配置文件加载进Spring容器。
```java
@Import(SpringConfiguredConfiguration.class)
public @interface EnableSpringConfigured {}
```
`@Import`的value值是一个数组，一个一个注入比较繁琐，因此可以搭配`ImportSelector`接口来使用，用法如下：
```java
@Configuration
@Import(MyImportSelector.class)
public class MyConfig {}

public class MyImportSelector implements ImportSelector {
	@Override
	public String[] selectImports(AnnotationMetadata annotationMetadata) {
		return new String[]{"org.springframework.demo.model.Teacher","org.springframework.demo.model.Student"};
	}
}
```
其中`selectImports`方法返回的数组就会通过`@Import`注解注入到Spring容器中。<br />无独有偶，`ImportBeanDefinitionRegistrar`接口也提供了注入bean的方法。
```java
@Import(AspectJAutoProxyRegistrar.class)
public @interface EnableAspectJAutoProxy {
    ......
}
```
点击`AspectJAutoProxyRegistrar`类，发现它实现了`ImportBeanDefinitionRegistrar`接口，它的`registerBeanDefinitions`方法便是注入bean的过程，可以参考下。<br />如果觉得源代码比较难懂，可以看一下自定义的类
```java
@Configuration
@Import(value = {MyImportBeanDefinitionRegistrar.class})
public class MyConfig {}

public class MyImportBeanDefinitionRegistrar implements ImportBeanDefinitionRegistrar {
    @Override
    public void registerBeanDefinitions(AnnotationMetadata importingClassMetadata,
                                        BeanDefinitionRegistry registry) {
            RootBeanDefinition tDefinition = new RootBeanDefinition(Teacher.class);
            // 注册 Bean，并指定bean的名称和类型
            registry.registerBeanDefinition("teacher", tDefinition);
        }
    }
}
```
这样就把Teacher类注入到Spring容器中了。
<a name="Tamch"></a>
## FactoryBean
提到`FactoryBean`，就不得不与`BeanFactory`比较一番。

- `BeanFactory `: 是 Factory， IOC容器或者对象工厂，所有的Bean都由它进行管理
- `FactoryBean `: 是Bean ，是一个能产生或者修饰对象生成的工厂 Bean，实现与工厂模式和修饰器模式类似

那么`FactoryBean`是如何实现bean注入的呢？<br />先定义实现了`FactoryBean`接口的类
```java
public class TeacherFactoryBean implements FactoryBean<Teacher> {
	
	/**
	* 返回此工厂管理的对象实例
	**/
	@Override
	public Teacher getObject() throws Exception {
		return new Teacher();
	}
	
	/**
	* 返回此 FactoryBean 创建的对象的类型
	**/
	@Override
	public Class<?> getObjectType() {
		return Teacher.class;
	}
	
}
```
然后通过 `@Configuration` + `@Bean`的方式将`TeacherFactoryBean`加入到容器中
```java
@Configuration
public class MyConfig {
	@Bean
	public TeacherFactoryBean teacherFactoryBean(){
		return new TeacherFactoryBean();
	}
}
```
注意：没有向容器中注入Teacher，而是直接注入的`TeacherFactoryBean`，然后从容器中拿`Teacher`这个类型的bean，成功运行。
<a name="Qg7w6"></a>
## BDRegistryPostProcessor
<a name="Z2vob"></a>
### 源码
```java
public interface BeanDefinitionRegistryPostProcessor extends BeanFactoryPostProcessor {
	// 注册bean到spring容器中
	void postProcessBeanDefinitionRegistry(BeanDefinitionRegistry registry) throws BeansException;
}

@FunctionalInterface
public interface BeanFactoryPostProcessor {
	void postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory) throws BeansException;
}
```
`BeanFactoryPostProcessor`接口是`BeanFactory`的后置处理器，方法`postProcessBeanFactory`对bean的定义进行控制。重点来看看`postProcessBeanDefinitionRegistry`方法：它的参数是`BeanDefinitionRegistry`，顾名思义就是与`BeanDefinition`注册相关的。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650801090954-e1d55101-a0a5-4fa2-a4e5-7c275fc57786.png#clientId=uec609ff2-201d-4&from=paste&id=ue698bd98&originHeight=259&originWidth=535&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u706009e2-315e-4836-8af9-595e2ab98ee&title=)<br />通过观察该类，可以发现它里边包含了`registerBeanDefinition`方法，这个不就是想要的吗？为了能更好的使用该接口来达到注入bean的目的，先来看看Spring是如何操作此接口的。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650801090946-8a54671b-95ec-4cbb-98c9-6223c384e356.png#clientId=uec609ff2-201d-4&from=paste&id=uc478ffe7&originHeight=143&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc6a14229-d2d7-4526-aab5-651db887c02&title=)<br />看下`invokeBeanFactoryPostProcessors`方法，会发现没有实现`PriorityOrdered`和`Ordered`的bean（这种跟自定义的实现类有关）会执行以下代码。
```java
while (reiterate) {
    ......
    invokeBeanDefinitionRegistryPostProcessors(currentRegistryProcessors, registry);
    ......
}
```
进入该方法
```java
private static void invokeBeanDefinitionRegistryPostProcessors(
    Collection<? extends BeanDefinitionRegistryPostProcessor> postProcessors, 
    BeanDefinitionRegistry registry) {

    for (BeanDefinitionRegistryPostProcessor postProcessor : postProcessors) {
        postProcessor.postProcessBeanDefinitionRegistry(registry);
    }
}
```
会发现实现了`BeanDefinitionRegistryPostProcessor`接口的bean，其`postProcessBeanDefinitionRegistry`方法会被调用，也就是说如果自定义接口实现该接口，它的`postProcessBeanDefinitionRegistry`方法也会被执行。
<a name="vGl5N"></a>
### 实战
话不多说，直接上代码。自定义接口实现类
```java
public class MyBeanDefinitionRegistryPostProcessor implements BeanDefinitionRegistryPostProcessor {
	
	/**
	* 初始化过程中先执行
	**/
	@Override
	public void postProcessBeanDefinitionRegistry(BeanDefinitionRegistry registry) throws BeansException {
		RootBeanDefinition rootBeanDefinition = new RootBeanDefinition(Teacher.class);
		//Teacher 的定义注册到spring容器中
		registry.registerBeanDefinition("teacher", rootBeanDefinition);
	}
	
	/**
	* 初始化过程中后执行
	**/
	@Override
	public void postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory) throws BeansException {}
}
```
启动类代码
```java
public static void main(String[] args) {
	AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext();
	MyBeanDefinitionRegistryPostProcessor postProcessor = new MyBeanDefinitionRegistryPostProcessor();
	//将自定义实现类加入 Spring 容器
	context.addBeanFactoryPostProcessor(postProcessor);
	context.refresh();
	Teacher bean = context.getBean(Teacher.class);
	System.out.println(bean);
}
```
启动并打印结果
```java
org.springframework.demo.model.Teacher@2473d930
```
发现已经注入到Spring容器中了。以上就是总结的几种将bean注入Spring容器的方式。
