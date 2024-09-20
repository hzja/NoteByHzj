Java Spring Bean<br />在使用Spring时，Bean之间会有些依赖，比如一个Bean A实例化时需要用到Bean B,那么B应该在A之前实例化好。很多时候Spring智能地做好了这些工作，但某些情况下可能不是，比如SpringBoot的`@AutoConfigureAfter`注解，手动的指定Bean的实例化顺序。<br />了解Spring内Bean的解析，加载和实例化顺序机制有助于更好的使用Spring/SpringBoot，避免手动的去干预Bean的加载过程，搭建更优雅的框架。<br />Spring容器在实例化时会加载容器内所有非延迟加载的单例类型Bean，看如下源码：
```java
public abstract class AbstractApplicationContext extends DefaultResourceLoader
implements ConfigurableApplicationContext, DisposableBean {

	//刷新Spring容器，相当于初始化
	public void refresh() throws BeansException, IllegalStateException {
		......
		// Instantiate all remaining (non-lazy-init) singletons.
		finishBeanFactoryInitialization(beanFactory);
	}
}
public class DefaultListableBeanFactory extends AbstractAutowireCapableBeanFactory
implements ConfigurableListableBeanFactory, BeanDefinitionRegistry, Serializable {

	/** List of bean definition names, in registration order */
	private volatile List<String> beanDefinitionNames = new ArrayList<String>(256);
	public void preInstantiateSingletons() throws BeansException {
		List<String> beanNames = new ArrayList<String>(this.beanDefinitionNames);
		for (String beanName : beanNames) {
			......
			getBean(beanName);  //实例化Bean
		}
	}
}
```
`ApplicationContext`内置一个`BeanFactory`对象，作为实际的Bean工厂，和Bean相关业务都交给`BeanFactory`去处理。<br />在`BeanFactory`实例化所有非延迟加载的单例Bean时，遍历`beanDefinitionNames` 集合，按顺序实例化指定名称的Bean。`beanDefinitionNames` 属性是Spring在加载Bean Class生成的`BeanDefinition`时，为这些Bean预先定义好的名称，看如下代码：
```java
public class DefaultListableBeanFactory extends AbstractAutowireCapableBeanFactory
implements ConfigurableListableBeanFactory, BeanDefinitionRegistry, Serializable {

    public void registerBeanDefinition(String beanName, BeanDefinition beanDefinition)
    throws BeanDefinitionStoreException {
        ......
        this.beanDefinitionNames.add(beanName);
    }
}
```
BeanFactory在加载一个`BeanDefinition`（也就是加载Bean Class）时，将相应的beanName存入`beanDefinitionNames`属性中，在加载完所有的`BeanDefinition`后，执行Bean实例化工作，此时会依据`beanDefinitionNames`的顺序来有序实例化Bean，也就是说Spring容器内Bean的加载和实例化是有顺序的，而且近似一致，当然仅是近似。<br />Spring在初始化容器时，会先解析和加载所有的Bean Class，如果符合要求则通过Class生成`BeanDefinition`，存入BeanFactory中，在加载完所有Bean Class后，开始有序的通过`BeanDefinition`实例化Bean。<br />先看加载Bean Class过程，零配置下Spring Bean的加载起始于`ConfigurationClassPostProcessor`的`postProcessBeanDefinitionRegistry`（`BeanDefinitionRegistry`）方法，总结了如下其加载解析Bean Class的流程：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1614604555297-3a568acd-be84-478e-abd3-f82a8ef9d635.png#averageHue=%23fafaf9&height=420&id=JlU5T&originHeight=1259&originWidth=2224&originalType=binary&ratio=1&rotation=0&showTitle=false&size=196265&status=done&style=shadow&title=&width=741.3333333333334)<br />配置类可以是Spring容器的起始配置类，也可以是通过`@ComponentScan`扫描得到的类，也可以是通过`@Import`引入的类。如果这个类上含有`@Configuration`，`@Component`，`@ComponentScan`，`@Import`，`@ImportResource`注解中的一个，或者内部含有`@Bean`标识的方法，那么这个类就是一个配置类，Spring就会按照一定流程去解析这个类上的信息。<br />在解析的第一步会校验当前类是否已经被解析过了，如果是，那么需要按照一定的规则处理（`@ComponentScan`得到的Bean能覆盖`@Import`得到的Bean，`@Bean`定义的优先级最高）。<br />如果未解析过，那么开始解析：

1. 解析内部类，查看内部类是否应该被定义成一个Bean，如果是，递归解析。
2. 解析`@PropertySource`，也就是解析被引入的Properties文件。
3. 解析配置类上是否有`@ComponentScan`注解，如果有则执行扫描动作，通过扫描得到的Bean Class会被立即解析成`BeanDefinition`，添加进`beanDefinitionNames`属性中。之后查看扫描到的Bean Class是否是一个配置类（大部分情况是，因为标识`@Component`注解），如果是则递归解析这个Bean Class。
4. 解析`@Import`引入的类，如果这个类是一个配置类，则递归解析。
5. 解析`@Bean`标识的方法，此种形式定义的Bean Class不会被递归解析
6. 解析父类上的`@ComponentScan`，`@Import`，`@Bean`，父类不会被再次实例化，因为其子类能够做父类的工作，不需要额外的Bean了。

在1，3，4，6中都有递归操作，也就是在解析一个Bean Class A时，发现其上能够获取到其他Bean Class B信息，此时会递归的解析Bean Class B，在解析完Bean Class B后再接着解析Bean Class A，可能在解析B时能够获取到C，那么也会先解析C再解析B，就这样不断的递归解析。<br />在第3步中，通过`@ComponentScan`扫描直接得到的Bean Class会被立即加载入`beanDefinitionNames`中，但`@Import`和`@Bean`形式定义的Bean Class则不会，也就是说正常情况下面`@ComponentScan`直接得到的Bean其实例化时机比其他两种形式的要早。<br />通过`@Bean`和`@Import`形式定义的Bean Class不会立即加载，他们会被放入一个`ConfigurationClass`类中，然后按照解析的顺序有序排列，就是图片上的 “将配置类有序排列”。一个`ConfigurationClass`代表一个配置类，这个类可能是被`@ComponentScan`扫描到的，则此类已经被加载过了；也可能是被`@Import`引入的，则此类还未被加载；此类中可能含有`@Bean`标识的方法。<br />Spring在解析完了所有Bean Class后，开始加载`ConfigurationClass`。如果这个`ConfigurationClass`是被Import的，也就是说在加载`@ComponentScan`时其未被加载，那么此时加载`ConfigurationClass`代表的Bean Class。然后加载`ConfigurationClass`内的`@Bean`方法。
> 顺序总结：`@ComponentScan` > `@Import` > `@Bean`

下面看实际的启动流程：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1614604885813-4cd5d24f-9e44-4018-85d1-35c314fdee22.png#averageHue=%23fbfbfb&height=390&id=thSNo&originHeight=1171&originWidth=2059&originalType=binary&ratio=1&rotation=0&showTitle=false&size=132029&status=done&style=shadow&title=&width=686.3333333333334)<br />Bean Class的结构图如上所示，A是配置类的入口，通过A能直接或间接的引入一个模块。<br />此时启动Spring容器，将A引入容器内。<br />如果A是通过`@ComponentScan`扫描到的，那么此时的加载顺序是：
> A > D > F > B > E > G > C

如果A是通过`@Import`形式引入的，那么此时的加载顺序是：
> D > F > B > E > G > A > C

当然以上仅仅代表着加载Bean Class的顺序，实际实例化Bean的顺序和加载顺序大体相同，但还是会有一些差别。<br />Spring在通过`getBean(beanName)`形式实例化Bean时，会通过`BeanDefinition`去生成Bean对象。在这个过程中，如果`BeanDefinition`的DependsOn不为空，从字面理解就是依赖某个什么，其值一般是某个或多个beanName，也就是说依赖于其他Bean，此时Spring会将DependsOn指定的这些名称的Bean先实例化，也就是先调用`getBean(dependsOn)`方法。可以通过在Bean Class或者`@Bean`的方法上标识`@DependsOn`注解，来指定当前Bean实例化时需要触发哪些Bean的提前实例化。<br />当一个Bean A内部通过`@Autowired`或者`@Resource`注入Bean B，那么在实例化A时会触发B的提前实例化，此时会注册A>B的dependsOn依赖关系，实质和`@DependsOn`一样，这个是Spring自动处理好。<br />了解Spring Bean的解析，加载及实例化的顺序机制能够加深对Spring的理解，搭建更优雅简介的Spring框架。

