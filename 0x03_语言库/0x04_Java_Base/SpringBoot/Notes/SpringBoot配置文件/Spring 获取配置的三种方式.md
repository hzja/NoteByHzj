Java Spring
<a name="KrZue"></a>
## Spring 中获取配置的三种方式

1. 通过 `@Value` 方式动态获取单个配置
2. 通过 `@ConfigurationProperties` + 前缀方式批量获取配置
3. 通过 `Environment` 动态获取单个配置
<a name="M1UtM"></a>
### 通过 `@Value` 动态获取单个配置
<a name="nx2fc"></a>
#### 作用

1. 可修饰到任一变量获取，使用较灵活
<a name="w1k8U"></a>
#### 优点

1. 使用简单，且使用关联的链路较短
<a name="tBXJa"></a>
#### 缺点

1. 配置名不能被有效枚举到
2. 每一个配置的使用都需重新定义，使用较为麻烦
3. 项目强依赖配置的定义，配置不存在则会导致项目无法启动
<a name="PFuxJ"></a>
#### 使用场景

1. 项目强依赖该配置的加载，想要从源头避免因配置缺失导致的未知问题
2. 只想使用少数几个配置
<a name="Pq06A"></a>
#### 代码示例
```java
@Configuration
public class ConfigByValueAnnotation {

	@Value("${server.port}")
	private String serverPort;

	public String getServerPort() {
		return serverPort;
	}
}
```
<a name="JcaKJ"></a>
#### 测试代码
```java
@DisplayName("multipart get config")
@SpringBootTest
public class MultipartGetConfigTest {

    private static final Logger log = LoggerFactory.getLogger(MultipartGetConfigTest.class);

    @Autowired
    private ConfigByValueAnnotation configByValueAnnotation;

    @Test
    public void getByValueAnnotation(){
        log.info("get by @Value, value: {}", configByValueAnnotation.getServerPort());
    }
}
```
<a name="xoHqX"></a>
#### 测试结果
```
org.spring.demo.MultipartGetConfigTest   : get by @Value, value: 7100
```
<a name="qWzWk"></a>
### 通过 `@ConfigurationProperties` + 前缀方式批量获取
<a name="DY5QT"></a>
#### 作用

1. 用于配置类的修饰或批量配置的获取
<a name="o6a0b"></a>
#### 优点

1. 使用配置只需确定 key 的前缀即能使用，有利于批量获取场景的使用
2. 因采用前缀匹配，所以在使用新的相同前缀 key 的配置时无需改动代码
<a name="mERae"></a>
#### 缺点

1. 使用复杂，需定义配置类或者手动创建 bean 后引入使用
2. 增加新的前缀相同 key 时可能会引入不稳定因素
<a name="whqtT"></a>
#### 使用场景

1. 需要同时使用多前缀相同 key 的配置
2. 期望增加新配置但不修改代码的 properties 注入
<a name="DZ6HN"></a>
#### 代码示例
```java
@Component
@ConfigurationProperties(prefix = "server", ignoreInvalidFields = true)
public class ConfigByConfigurationProperties {

	private Integer port;

	public Integer getPort() {
		return port;
	}

	public ConfigByConfigurationProperties setPort(Integer port) {
		this.port = port;
		return this;
	}
}


@Configuration
public class ConfigByConfigurationPropertiesV2 {

	@Bean("configByValueAnnotationV2")
	@ConfigurationProperties(prefix = "server2")
	public Properties properties(){
		return new Properties();
	}

}
```
<a name="K8TeD"></a>
#### 测试代码
```java
@DisplayName("multipart get config")
@SpringBootTest
public class MultipartGetConfigTest {

       private static final Logger log = LoggerFactory.getLogger(MultipartGetConfigTest.class);
    
       @Autowired
       private ConfigByConfigurationProperties configByConfigurationProperties;
    
       @Autowired
       @Qualifier("configByValueAnnotationV2")
       private Properties properties;
    
       @Test
       public void getByConfigurationProperties(){
           log.info("get by @ConfigurationProperties, value: {}", configByConfigurationProperties.getPort());
           log.info("get by @ConfigurationProperties and manual create bean, value: {}", properties.getProperty("port"));
       }

}
```
<a name="YeGYT"></a>
#### 测试结果
```
org.spring.demo.MultipartGetConfigTest   : get by @ConfigurationProperties, value: 7100
org.spring.demo.MultipartGetConfigTest   : get by @ConfigurationProperties and manual create bean, value: 7100
```
<a name="egpah"></a>
### 通过 `Environment` 动态获取单个配置
<a name="eE5ZL"></a>
#### 作用

1. 用于动态在程序代码中获取配置，而配置 key 不需提前定义
<a name="LKKi2"></a>
#### 优点

1. 获取的配置的 key 可不提前定义，程序灵活性高
2. 配置 key 可使用枚举统一放置与管理
<a name="GnXmZ"></a>
#### 缺点

1. 使用较复杂，需继承 Environment 接口形成工具类进行获取
2. 获取 key 对应的枚举与 key 定义分离，value 获取链路较长
<a name="LElxf"></a>
#### 使用场景

1. 只需使用少量的配置
2. 获取配置的 key 无提前定义，需要根据对配置的有无进行灵活使用
<a name="tVdiI"></a>
#### 代码示例
```java
@Component
public class ConfigByEnvironment implements EnvironmentAware {

     private static final Logger log = LoggerFactory.getLogger(ConfigByEnvironment.class);
  
     private Environment environment;
  
     public Optional<String> get(String configKey){
         String config = environment.getProperty(configKey);
         return Objects.isNull(config) ? Optional.empty() : Optional.of(config);
     }
  
     public void get(String configKey, Consumer<String> consumer){
         Optional<String> config = get(configKey);
         if(!config.isPresent()){
             log.warn("application config, get config by key fail, key: {}", configKey);
         }
         config.ifPresent(consumer);
     }
  
     @Override
     public void setEnvironment(@NonNull Environment environment) {
         this.environment = environment;
     }
}

public enum ConfigByEnvironmentKey {

     SERVER_PORT("server.port", "server port");
  
     private String key;
  
     private String description;
  
     ConfigByEnvironmentKey(String key, String description) {
         this.key = key;
         this.description = description;
     }
  
     public String getKey() {
         return key;
     }
  
     public String getDescription() {
         return description;
     }
}
```
<a name="sVcvt"></a>
#### 测试代码
```java
@DisplayName("multipart get config")
@SpringBootTest
public class MultipartGetConfigTest {
    
    private static final Logger log = LoggerFactory.getLogger(MultipartGetConfigTest.class);
    
    @Autowired
    private ConfigByEnvironment configByEnvironment;
    
    @Test
    public void getByEnvironment(){
        configByEnvironment.get(ConfigByEnvironmentKey.SERVER_PORT.getKey()).ifPresent(value -> log.info("get by environment, value: {}", value));
    }
    
}
```
<a name="tKzJE"></a>
#### 测试结果
```
org.spring.demo.MultipartGetConfigTest   : get by environment, value: 7100
```
<a name="J6pni"></a>
## 总结
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1646794357278-b56f9223-36b7-4d21-9cf3-f62bf37b5bf7.png#clientId=ucf40010e-f977-4&from=paste&id=u7c009d63&originHeight=524&originWidth=762&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1c359969-944a-453c-bad0-e4ec4ecd00c&title=)
