Java SpringCloud
<a name="OFzUz"></a>
## 需求
Spring Cloud 的 config 模块，它是用来支持分布式配置的，原来单机配置在使用了 Spring Cloud 之后，可以支持第三方存储配置和配置的动态修改和重新加载，在业务代码里实现配置的重新加载，Spring Cloud 将整个流程抽离为框架，并很好的融入到 Spring 原有的配置和 Bean 模块内。<br />环境基于 Spring 5.0.5、Spring Boot 2.0.1 和 Spring Cloud 2.0.2。
<a name="ZmMNv"></a>
## 背景和问题
服务原来有一批单机的配置，由于同一 key 的配置太长，于是将其配置为数组的形式，并使用 Spring Boot 的`@ConfigurationProperties`和`@Value`注解来解析为 Bean 属性。<br />properties 文件配置像：
```java
test.config.elements[0]=value1
test.config.elements[1]=value2
test.config.elements[2]=value3
```
在使用时：
```java
@ConfigurationProperties(prefix="test.config")
Class Test{
    @Value("${#elements}")
    private String[] elements;
}
```
这样，Spring 会对 Test 类自动注入，将数组 `[value1,value2,value3]` 注入到 `elements` 属性内。<br />而使用 Spring Cloud 自动加载配置的姿势是这样：
```java
@RefreshScope
class Test{
    @Value("${test.config.elements}")
    private String[] elements;
}
```
使用`@RefreshScope`注解的类，在环境变量有变动后会自动重新加载，将最新的属性注入到类属性内，但它却不支持数组的自动注入。<br />目标是能找到一种方式，使其即支持注入数组类型的属性，又能使用 Spring Cloud 的自动刷新配置的特性。
<a name="Asqwt"></a>
## 环境和属性
无论Spring Cloud 的特性如何优秀，在 Spring 的地盘，还是要入乡随俗，和 Spring 的基础组件打成一片。所以为了了解整个流程，就要先了解 Spring 的基础。<br />Spring 是一个大容器，它不光存储 Bean 和其中的依赖，还存储着整个应用内的配置，相对于 `BeanFactory` 存储着各种 Bean，Spring 管理环境配置的容器就是`Environment`，从 `Environment` 内，能根据 key 获取所有配置，还能根据不同的场景（Profile，如 dev,test,prod）来切换配置。<br />但 Spring 管理配置的最小单位并不是属性，而是`PropertySource`(属性源)，可以理解 `PropertySource` 是一个文件，或是某张配置数据表，Spring 在 `Environment` 内维护一个 `PropertySourceList`，当获取配置时，Spring 从这些 `PropertySource` 内查找到对应的值，并使用`ConversionService`将值转换为对应的类型返回。
<a name="ICwbd"></a>
## Spring Cloud 配置刷新机制
<a name="p6vmE"></a>
### 分布式配置
Spring Cloud 内提供了`PropertySourceLocator`接口来对接 Spring 的 `PropertySource` 体系，通过 `PropertySourceLocator`，就拿到一个”自定义”的 `PropertySource`，Spring Cloud 里还有一个实现`ConfigServicePropertySourceLocator`，通过它，可以定义一个远程的 `ConfigService`，通过公用这个 ConfigService 来实现分布式的配置服务。<br />从`ConfigClientProperties`这个配置类可以看得出来，它也为远程配置预设了用户名密码等安全控制选项，还有 label 用来区分服务池等配置。
<a name="Qlpig"></a>
### scope 配置刷新
远程配置有了，接下来就是对变化的监测和基于配置变化的刷新。<br />Spring Cloud 提供了`ContextRefresher`来实现环境的刷新，其主要逻辑在`refreshEnvironment`方法和`scope.refreshAll()`方法，分开来看。<br />先来看 Spring Cloud 支持的 `scope.refreshAll` 方法。
```java
public void refreshAll() {
    super.destroy();
    this.context.publishEvent(new RefreshScopeRefreshedEvent());
}
```
`scope.refreshAll` 则更”野蛮”一些，直接销毁了 scope，并发布了一个 `RefreshScopeRefreshedEvent` 事件，scope 的销毁会导致 scope 内（被 `RefreshScope` 注解）所有的 bean 都会被销毁。而这些被强制设置为 `lazyInit` 的 bean 再次创建时，也就完成了新配置的重新加载。
<a name="LATWT"></a>
### `ConfigurationProperties` 配置刷新
然后再回过头来看 `refreshEnvironment` 方法。
```java
Map<String, Object> before = extract(this.context.getEnvironment().getPropertySources());
addConfigFilesToEnvironment();
Set<String> keys = changes(before,extract(this.context.getEnvironment().getPropertySources())).keySet();
this.context.publishEvent(new EnvironmentChangeEvent(context, keys));
return keys;
```
它读取了环境内所有 `PropertySource` 内的配置后，重新创建了一个 SpringApplication 以刷新配置，再次读取所有配置项并得到与前面保存的配置项的对比，最后将前后配置差发布了一个`EnvironmentChangeEvent`事件。而 `EnvironmentChangeEvent` 的监听器是由 `ConfigurationPropertiesRebinder` 实现的，其主要逻辑在`rebind`方法。
```java
Object bean = this.applicationContext.getBean(name);
if (AopUtils.isAopProxy(bean)) {
    bean = ProxyUtils.getTargetObject(bean);
}
if (bean != null) {
    this.applicationContext.getAutowireCapableBeanFactory().destroyBean(bean);
    this.applicationContext.getAutowireCapableBeanFactory().initializeBean(bean, name);
    return true;
}
```
可以看到它的处理逻辑，就是把其内部存储的`ConfigurationPropertiesBeans`依次执行销毁逻辑，再执行初始化逻辑实现属性的重新绑定。<br />这里可以知道，Spring Cloud 在进行配置刷新时是考虑过 `ConfigurationProperties` 的，经过测试，在 `ContextRefresher` 刷新上下文后，`ConfigurationProperties` 注解类的属性是会进行动态刷新的。
<a name="FBBeA"></a>
## Bean 的创建与环境
接着再来看一下，环境里的属性都是怎么在 Bean 创建时被使用的。<br />Spring 的 Bean 都是在 BeanFactory 内创建的，创建逻辑的入口在`AbstractBeanFactory.doGetBean(name, requiredType, args, false)`方法，而具体实现在`AbstractAutowireCapableBeanFactory.doCreateBean`方法内，在这个方法里，实现了 Bean 实例的创建、属性填充、初始化方法调用等逻辑。<br />在这里，有一个非常复杂的步骤就是调用全局的`BeanPostProcessor`，这个接口是 Spring 为 Bean 创建准备的勾子接口，实现这个接口的类可以对 Bean 创建时的操作进行修改。它是一个非常重要的接口，是能干涉 Spring Bean 创建流程的重要入口。<br />要说的是它的一种具体实现`ConfigurationPropertiesBindingPostProcessor`，它通过调用链`ConfigurationPropertiesBinder.bind()` --> `Binder.bindObject()` --> `Binder.findProperty()`方法查找环境内的属性。
```java
private ConfigurationProperty findProperty(ConfigurationPropertyName name,
                                           Context context) {
    if (name.isEmpty()) {
        return null;
    }
    return context.streamSources()
        .map((source) -> source.getConfigurationProperty(name))
        .filter(Objects::nonNull).findFirst().orElse(null);
}
```
找到对应的属性后，再使用 converter 将属性转换为对应的类型注入到 Bean。
```java
private <T> Object bindProperty(Bindable<T> target, Context context,
                                ConfigurationProperty property) {
    context.setConfigurationProperty(property);
    Object result = property.getValue();
    result = this.placeholdersResolver.resolvePlaceholders(result);
    result = context.getConverter().convert(result, target);
    return result;
}
```
<a name="lpoak"></a>
## 一种 trick 方式
由上面可以看到，Spring 是支持 `@ConfigurationProperties` 属性的动态修改的，但在查询流程时，也找到了一种比较 trick 的方式。<br />先来整理动态属性注入的关键点，再从这些关键点里找可修改点。

1. `PropertySourceLocator` 将 `PropertySource` 从远程数据源引入，如果这时能修改数据源的结果就能达到目的，可是 Spring Cloud 的远程资源定位器 `ConfigServicePropertySourceLocator` 和 远程调用工具 RestTemplate 都是实现类，如果生硬地对其继承并修改，代码很不优雅。
2. Bean 创建时会依次使用 `BeanPostProcessor` 对上下文进行操作。这时添加一个 `BeanPostProcessor`，可以手动实现对 Bean 属性的修改。但这种方式 实现起来很复杂，而且由于每一个 `BeanPostProcessor` 在所有 Bean 创建时都会调用，可能会有安全问题。
3. Spring 会在解决类属性注入时，使用 `PropertyResolver` 将配置项解析为类属性指定的类型。这时候添加属性解析器 `PropertyResolver` 或类型转换器 `ConversionService` 可以插手属性的操作。但它们都只负责处理一个属性，由于目标是”多个”属性变成一个属性，它们也无能为力。

这里能想到的方式是借用 Spring 自动注入的能力，把 Environment Bean 注入到某个类中，然后在类的初始化方法里对 `Environment` 内的 `PropertySource` 里进行修改，也可以达成目的，这里贴一下伪代码。
```java
@Component
@RefreshScope  // 借用 Spring Cloud 实现此 Bean 的刷新
public class ListSupportPropertyResolver {
    @Autowired
    ConfigurableEnvironment env; // 将环境注入到 Bean 内是修改环境的重要前提

    @PostConstruct
    public void init() {
        // 将属性键值对从环境内取出
        Map<String, Object> properties = extract(env.getPropertySources());

        // 解析环境里的数组，抽取出其中的数组配置
        Map<String, List<String>> listProperties = collectListProperties(properties)
            Map<String, Object> propertiesMap = new HashMap<>(listProperties);

        MutablePropertySources propertySources = env.getPropertySources();
        // 把数组配置生成一个 PropertySource 并放到环境的 PropertySourceList 内
        propertySources.addFirst(new MapPropertySource("modifiedProperties", propertiesMap));
    }
}
```
这样，在创建 Bean 时，就能第一优先级使用修改过的 PropertySource 了。<br />当然了，有了比较“正规”的方式后，不必要对 PropertySource 进行修改，毕竟全局修改等于未知风险或埋坑。
