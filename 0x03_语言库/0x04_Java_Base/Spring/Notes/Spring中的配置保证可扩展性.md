Java Spring<br />项目引用了一个依赖jar，配置封装太封闭了，不能扩展。业务变动一次那个jar就要跟着升级一次，而且不同的项目还引用了这个jar的不同版本。所以通过可扩展定制化实现可以很好的提高效率。<br />原本的配置类似是这样的：
```java
@Configuration(proxyBeanMethods = false)
public class MyConfiguration {

    /**
     * bean
     */
    @Bean
    ConfigBean configBean(Config config)  {
        //todo 逻辑
     return new ConfigBean(config)
    }     
}
```
如果想根据项目的不同定制不同的`ConfigBean`就不太好弄了。如果能在`Config`对象传入`ConfigBean`构造之前放一个修改`Config`的口子就好了。这样`ConfigBean`的初始化生命周期也变成了
:::info
发现Config对象-> 修改Config对象-> 初始化ConfigBean
:::
于是定义了一个可以修改`Config`对象的接口：
```java
@FunctionalInterface
public interface ConfigCustomizer {

    /**
     * Customize.
     *
     * @param config the config
     */
    void customize(Config config);
}
```
上面整个配置就变成这样的了：
```java
@Configuration(proxyBeanMethods = false)
public class MyConfiguration {
    private List<ConfigCustomizer> configCustomizers = Collections.emptyList();
    /**
     * bean
     */
    @Bean
    ConfigBean configBean(Config config)  {
        
        // 其它公共逻辑省略
        
        // 最后定制逻辑注入
        configCustomizers
                .forEach(configCustomizer -> configCustomizer.customize(config));
     return new ConfigBean(config)
    }
    
    @Autowired(required = false)
    void setConfigCustomizers(List<ConfigCustomizer> configCustomizers) {
        this.configCustomizers = configCustomizers;
    }
}
```
这样需要改动配置时只需要声明一个`ConfigCustomizerBean`即可，它会被`setConfigCustomizers`自动发现并执行自定义的方法。<br />这里会有朋友说`@ConditionalOnMissingBean`系列注解也能干这个事啊，没错！这样完全可以声明一个新的`ConfigBean`取而代之。但是这是两种策略：**一种是修修补补就能用；一种是推倒重来**。在封装组件的时候要合理利用这些策略，该开口子的要开口子，不该开放的保持封闭，另外保证组件的扩展性也是很重要的。
