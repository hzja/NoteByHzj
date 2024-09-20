Java Spring 
<a name="dYo4V"></a>
## JavaConfig 配置扩展
从Spring3.0开始Spring提供了JavaConfig的方式可以用来代替以前XML的这种方式，原来在XML配置里的都可以通过注解来一一替换实现。主要通过`@Configuration`，`@Bean`， `@Import`，和`@DependsOn`这几个注解来搭配实现的。这种方式也是SpringBoot所使用的。
<a name="ctza4"></a>
### `@Configuration`
`@Configuration`只能标记在类上，表示该类为JavaConfig类，使其可以被Spring IOC容器扫描识别并创建Bean加入到容器中。`@Configuration`类就相当于以往的一个xml文件。下面看一个官网提供的例子：
```java
@Configuration
public class AppConfig {
    @Bean
    public MyService myService() {
        return new MyServiceImpl();
    }
}
```
这个定义的JavaConfig就相当于原来如下XML的配置：
```xml
<beans>
    <bean id="myService" class="cn.javajr.services.MyServiceImpl"/>
</beans>
```
<a name="j7nj4"></a>
### `@Bean`
`@Bean`只能被标记在方法上，表示该方法返回一个Spring Bean，可以被IOC容器托管，相当于以前在xml文件中写的元素。

- name：指定一个或者多个bean的名字，当没有设置name时，Spring容器会默认将`@Bean`方法名作为bean name，当设置了name后，就不会再使用方法名，同时设置多个name时，除第一个name外，其他的都会作为bean的别名。相当于xml配置中的name属性。
- `initMethod`：指定容器在初始化完bean后调用的方法。相当于xml配置中的init-method属性。
- `destroyMethod`：指定在容器在销毁bean前调用的方法。相当于xml配置中的 destroy-method。
- `autowire`：指定bean在自动装配时依赖注入使用的策略，取值可以参考`Enum`类Autowire 的三个常量：`Autowire.BY_NAME`，`Autowire.BY_TYPE`，`Autowire.NO`。
<a name="pG9g7"></a>
### `@Import`
XML配置中的标签，基于JavaConfig提供了`@Import`来组合模块化的配置类，使用方式如下所示：
```java
@Configuration()  
@Import({ApplicationContextConfig.class})  
public class ApplicationContextConfig {
```
上面就比较简单的介绍了几种通过JavaConfig注解来替换XML形式的注解，使用起来还是非常简单的，如果对以前的XML配置文件都比较了解的话，使用JavaConfig就更加简单方便了。
<a name="IHr0D"></a>
## Dubbo的JavaConfig
Dubbo是如何通过JavaConfig来替代XML形式的扩展的。看看dubbo的服务提供者是如何通过注解来实现的
```java
@Configuration
@EnableDubbo(scanBasePackages = "org.apache.dubbo.samples.annotation.impl")
@PropertySource("classpath:/spring/dubbo-provider.properties")
static class ProviderConfiguration {
}
```
`@Configuration`这个注解上面已经介绍过了，重点看下`@EnableDubbo`这个注解 `@EnableDubbo` 其实又是`@EnableDubboConfig` `@DubboComponentScan`是通过这两个组合注解来实现的， `@EnableDubboConfig`注解实现如下：
```java
@Target({ElementType.TYPE})
@Retention(RetentionPolicy.RUNTIME)
@Inherited
@Documented
@Import(DubboConfigConfigurationRegistrar.class)
public @interface EnableDubboConfig {
```
这个注解使用了`@Import(DubboConfigConfigurationRegistrar.class)` 所以Spring 在处理 `@EnableDubboConfig` 注解 的时候就会去实例化`DubboConfigConfigurationRegistrar` 并且调用它的`registerBeanDefinitions`方法,这个方法主要是对`propties`文件进行解析并根据不同的配置 项生成对应类型的Bean对象。
<a name="H02kD"></a>
## 总结

- 通过基于XML和基于Java的配置扩展，可以使用户通过Spring使用自己研发的组件，提供很好的易用性。
- 虽然现在大多数都是采用JavaConfig这种方式了，但是还是有人会比较喜欢xml这种方式 xml可以让配置集中化，所有的组件并不是分散的，因此使你对beans有一个很好的概览，比如mybais配置文件、SpingMvc配置文件，都放在一起，如果需要分割文件，Spring可以实现。然后（Spring）会通过内部标签进行重新组合或者外部上下文文件进行聚合。
- xml和JavaConfig 当然也是可以混合使用的，至于使用哪种方式还是看个人的编程习惯，没有哪种方式是绝对的好，各有千秋。
