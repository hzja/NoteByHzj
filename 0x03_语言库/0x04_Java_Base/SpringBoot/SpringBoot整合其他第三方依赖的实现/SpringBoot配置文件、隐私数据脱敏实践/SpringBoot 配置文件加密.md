JavaSpringBoot
<a name="lZ028"></a>
## 1、前景
在使用Springboot时，通常很多信息都是在application.yml中直接明文配置的，比如数据库链接信息，redis链接信息等等。但是这样是不安全的。<br />所以需要对敏感数据进行加密，这样防止密码泄露<br />Jasypt这个库解决了这个问题，实现了SpringBoot配置的自定加密加密
<a name="z9EXn"></a>
## 2、简单使用
源码对应地址：[http://gitlab.sea-clouds.cn/csdn/spring-boot-csdn/-/tree/master/05-spring-boot-jasypt](http://gitlab.sea-clouds.cn/csdn/spring-boot-csdn/-/tree/master/05-spring-boot-jasypt)
<a name="mygzK"></a>
### 2.1 引入依赖
```xml
<properties>
  <maven.compiler.source>11</maven.compiler.source>
  <maven.compiler.target>11</maven.compiler.target>
</properties>

<dependencyManagement>
  <dependencies>
    <dependency>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-dependencies</artifactId>
      <version>2.4.0</version>
      <type>pom</type>
      <scope>import</scope>
    </dependency>
  </dependencies>
</dependencyManagement>

<dependencies>
  <!-- web 和 测试 -->
  <dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-web</artifactId>
  </dependency>
  <dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-test</artifactId>
  </dependency>
  <dependency>
    <groupId>junit</groupId>
    <artifactId>junit</artifactId>
    <scope>test</scope>
  </dependency>
  <!-- jdbc -->
  <dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-jdbc</artifactId>
  </dependency>
  <dependency>
    <groupId>mysql</groupId>
    <artifactId>mysql-connector-java</artifactId>
  </dependency>

  <!-- jasypt 加密 -->
  <dependency>
    <groupId>com.github.ulisesbocchio</groupId>
    <artifactId>jasypt-spring-boot-starter</artifactId>
    <version>3.0.3</version>
  </dependency>
  <dependency>
    <groupId>org.projectlombok</groupId>
    <artifactId>lombok</artifactId>
  </dependency>
</dependencies>
```
<a name="VlXgc"></a>
### 2.2 配置application信息
jasypt配置
```yaml
jasypt:
  encryptor:
    # 加密算法
    algorithm: PBEWITHHMACSHA512ANDAES_256
    # 加密使用的盐
    password: jaspyt_password
```
<a name="GI0ig"></a>
### 2.3 加密解密测试
```java
/**
 * @description: 加密解密测试
 */
@SpringBootTest
@RunWith(SpringRunner.class)
public class JasyptTest {

    @Autowired
    private StringEncryptor stringEncryptor;

    /**
     * 加密解密测试
     */
    @Test
    public void jasyptTest() {
        // 加密
        System.out.println(stringEncryptor.encrypt("root"));    // JSrINYe4IBotHndGjX1hnmY3mtPNUJlXjP12cx1+pHqUz2FNXGPu3Frnajh3QCXg
        // 解密
        System.out.println(stringEncryptor.decrypt("JSrINYe4IBotHndGjX1hnmY3mtPNUJlXjP12cx1+pHqUz2FNXGPu3Frnajh3QCXg"));    // root
    }

    /**
     * 手动测试
     */
    @Test
    public void test() {
        PooledPBEStringEncryptor encryptor = new PooledPBEStringEncryptor();
        SimpleStringPBEConfig config = new SimpleStringPBEConfig();
        config.setPassword("jaspyt_password");
        config.setAlgorithm("PBEWITHHMACSHA512ANDAES_256");
        config.setKeyObtentionIterations("1000");
        config.setPoolSize("1");
        config.setProviderName("SunJCE");
        config.setSaltGeneratorClassName("org.jasypt.salt.RandomSaltGenerator");
        config.setIvGeneratorClassName("org.jasypt.iv.RandomIvGenerator");
        config.setStringOutputType("base64");
        encryptor.setConfig(config);
        System.out.println(encryptor.encrypt("root"));    // JSrINYe4IBotHndGjX1hnmY3mtPNUJlXjP12cx1+pHqUz2FNXGPu3Frnajh3QCXg
    }

}
```
<a name="MvgkR"></a>
## 3、使用Jasypt加密后的字符串代替数据库密码
<a name="MKdaP"></a>
### 3.1 使用加密类进行加密
密码 root 加密之后 XjYnpGd3JGICnxumpFcfRP8J83m265yC/r1FiwLr9Yo1PNbPXQ2xykLHPpy02CZ1
```java
/**
 * 数据库密码加密
 */
@Test
public void encryptPasswored() {
    // 加密
    System.out.println(stringEncryptor.encrypt("root"));    // XjYnpGd3JGICnxumpFcfRP8J83m265yC/r1FiwLr9Yo1PNbPXQ2xykLHPpy02CZ1
    // 解密
    System.out.println(stringEncryptor.decrypt("XjYnpGd3JGICnxumpFcfRP8J83m265yC/r1FiwLr9Yo1PNbPXQ2xykLHPpy02CZ1"));    // root
}
```
<a name="q9Yhv"></a>
### 3.2 替换数据库配置
```yaml
spring:
  datasource:
    driver-class-name: com.mysql.cj.jdbc.Driver
    url: jdbc:mysql://192.168.10.31/mp
    username: root
    # 使用ENC()包裹，标识为加密之后的，否则无法解密，会报错
    password: ENC(R2H69h1aEgJ3EDPLXAVQ5CxZJWtl8EvqIJUtlATRt6om4w46/J+blu2JAvkR7Yvp)
```
<a name="NJTLX"></a>
### 3.3 测试
```java
@Autowired
private DataSource dataSource;
/**
 * 测试加密之后的数据源使用是否正常
 *  查看是否能正常获取链接
 */
@Test
public void datasourceTest() throws SQLException {
    Connection connection = dataSource.getConnection();
    System.out.println(connection);     // HikariProxyConnection@1487059223 wrapping com.mysql.cj.jdbc.ConnectionImpl@48904d5a
    connection.close();
}
```
<a name="zZkAL"></a>
## 4、Jasypt配置详解
所有配置都在`JasyptEncryptorConfigurationProperties`类中定义，只需要在yml中配置属性，即可达到重写的目的<br />Jasypt使用`StringEncryptor`来解密属性。如果Spring上下文中找不到自定义的`StringEncryptor`，就会自动创建一个，可以通过以下属性进行配置<br />![2022-11-19-15-15-51.042218400.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1668848441816-c02ffbbc-b157-4a62-9f35-7295e2d13c88.png#averageHue=%23f8f7f6&clientId=u2a7da23d-7574-4&from=ui&id=ub75d3673&originHeight=430&originWidth=962&originalType=binary&ratio=1&rotation=0&showTitle=false&size=31322&status=done&style=none&taskId=uf237db9c-a2ef-4be6-a94e-ce043d2843f&title=)<br />唯一需要的属性是加密的盐，其余的可以使用默认值。虽然所有这些属性都可以在属性文件中生命，但加密所使用的盐不应该存储在属性文件中，而是应该通过系统属性、命令行参数或者环境变量传递，只要他的名称是`jasypt.encryptor.password`，它就可以工作。<br />倒数第二个属性`jasypt.encryptor.proxyPropertySources`用于只是jasypt spring boot如何拦截属性值进行解密。默认值false使用`PropertySource`、`EnumerablePropertySource`和`MapPropertySource`的自定义包装器实现。当为true时，拦截机制将在每个特定的`PropertySource`实现上使用CGLib代理。在某些必须保留原始`PropertySource`类型的场景中，这可能很有用。
<a name="LNfis"></a>
## 5、自定义加密
默认情况下，bean容器会配置`LazyJasyptSringEncryptor`
<a name="me91B"></a>
### 5.1 官方配置
官方配置的Bean都是在`EncryptablePropertyResolverConfiguration`中进行注入的
```java
@Bean(
    name = {"lazyJasyptStringEncryptor"}
)
public StringEncryptor stringEncryptor(EnvCopy envCopy, BeanFactory bf) {
    String customEncryptorBeanName = envCopy.get().resolveRequiredPlaceholders(ENCRYPTOR_BEAN_PLACEHOLDER);
    boolean isCustom = envCopy.get().containsProperty("jasypt.encryptor.bean");
    return new DefaultLazyEncryptor(envCopy.get(), customEncryptorBeanName, isCustom, bf);
}
```
<a name="e8rMu"></a>
### 5.2 自定义加密
可以在Spring上下文中共自定义自己的StringEncryptor Bean，默认的加密程序将被忽略
:::danger
注意：自定义Bean的名称必须为 `jasyptStringEncryptor`，否则解密不生效
:::
自定义注入bean
```java
/**
 * 加入 StringEncryptor 加密解密类
 *      beanName 必须为 jasyptStringEncryptor 才能是自定义的生效
 *      configProps 为jasypt框架中读取的配置类，就不用自己读取了
 */
@Bean("jasyptStringEncryptor")
public StringEncryptor jasyptStringEncryptor(Singleton<JasyptEncryptorConfigurationProperties> configProps) {
    PooledPBEStringEncryptor encryptor = new PooledPBEStringEncryptor();
    JasyptEncryptorConfigurationProperties jasyptProperties = configProps.get();
    SimpleStringPBEConfig config = new SimpleStringPBEConfig();
    config.setPassword(jasyptProperties.getPassword());
    config.setAlgorithm(jasyptProperties.getAlgorithm());
    config.setKeyObtentionIterations(jasyptProperties.getKeyObtentionIterations());
    config.setPoolSize(jasyptProperties.getPoolSize());
    config.setProviderName(jasyptProperties.getProviderName());
    config.setSaltGeneratorClassName(jasyptProperties.getSaltGeneratorClassname());
    config.setIvGeneratorClassName(jasyptProperties.getIvGeneratorClassname());
    config.setStringOutputType(jasyptProperties.getStringOutputType());
    encryptor.setConfig(config);
    return encryptor;
}
```
<a name="ng1hK"></a>
## 6、自定义属性探测器
属性探测器为判断一个属性值是否为加密后的字符串，并且截取真实字符串
<a name="iIrKv"></a>
### 6.1 官方处理流程
<a name="tMr3k"></a>
#### 6.1.2 注入
在`EncryptablePropertyResolverConfiguration`类中
```java
@Bean(
    name = {"lazyEncryptablePropertyDetector"}
)
public EncryptablePropertyDetector encryptablePropertyDetector(EnvCopy envCopy, BeanFactory bf) {
    String customDetectorBeanName = envCopy.get().resolveRequiredPlaceholders(DETECTOR_BEAN_PLACEHOLDER);
    boolean isCustom = envCopy.get().containsProperty("jasypt.encryptor.property.detector-bean");
    return new DefaultLazyPropertyDetector(envCopy.get(), customDetectorBeanName, isCustom, bf);
}
```
<a name="otdiR"></a>
#### 6.1.2 `DefaultLazyPropertyDetector`
默认实现是`DefaultLazyPropertyDetector`，具体代码是
```java
@Slf4j
public class DefaultLazyPropertyDetector implements EncryptablePropertyDetector {

    // 属性探测器
    private Singleton<EncryptablePropertyDetector> singleton;

    public DefaultLazyPropertyDetector(ConfigurableEnvironment environment, String customDetectorBeanName, boolean isCustom, BeanFactory bf) {
        singleton = new Singleton<>(() ->
                Optional.of(customDetectorBeanName)
                        .filter(bf::containsBean)
                        .map(name -> (EncryptablePropertyDetector) bf.getBean(name))
                        .map(tap(bean -> log.info("Found Custom Detector Bean {} with name: {}", bean, customDetectorBeanName)))
                        .orElseGet(() -> {
                            if(isCustom) {
                                throw new IllegalStateException(String.format("Property Detector custom Bean not found with name '%s'", customDetectorBeanName));
                            }
                            log.info("Property Detector custom Bean not found with name '{}'. Initializing Default Property Detector", customDetectorBeanName);
                            return createDefault(environment);
                        }));
    }

    public DefaultLazyPropertyDetector(ConfigurableEnvironment environment) {
        // 创建一个属性探测器
        singleton = new Singleton<>(() -> createDefault(environment));
    }

    private DefaultPropertyDetector createDefault(ConfigurableEnvironment environment) {
        // 读取所有的属性
        JasyptEncryptorConfigurationProperties props = JasyptEncryptorConfigurationProperties.bindConfigProps(environment);
        // 创建一个默认的属性探测器，读取配置文件中的前缀和后缀
        return new DefaultPropertyDetector(props.getProperty().getPrefix(), props.getProperty().getSuffix());
    }

    /**
      * 是否为解密格式字符串
      */
    @Override
    public boolean isEncrypted(String property) {
        return singleton.get().isEncrypted(property);
    }

    /**
      * 获取真是的加密后的字符串
      */
    @Override
    public String unwrapEncryptedValue(String property) {
        return singleton.get().unwrapEncryptedValue(property);
    }
}
```
在其中是创建了一个`DefaultPropertyDetector`对象<br />**6.1.3 DefaultPropertyDetector**
```java
public class DefaultPropertyDetector implements EncryptablePropertyDetector {

    // 默认前缀和后缀
    private String prefix = "ENC(";
    private String suffix = ")";

    public DefaultPropertyDetector() {
    }

    public DefaultPropertyDetector(String prefix, String suffix) {
        Assert.notNull(prefix, "Prefix can't be null");
        Assert.notNull(suffix, "Suffix can't be null");
        this.prefix = prefix;
        this.suffix = suffix;
    }

    @Override
    public boolean isEncrypted(String property) {
        if (property == null) {
            return false;
        }
        final String trimmedValue = property.trim();
        return (trimmedValue.startsWith(prefix) &&
                trimmedValue.endsWith(suffix));
    }

    // 去掉前缀和后缀
    @Override
    public String unwrapEncryptedValue(String property) {
        return property.substring(
            prefix.length(),
            (property.length() - suffix.length()));
    }
}
```
<a name="yueKp"></a>
### 6.2 自定义规则探测器
两种方式自定义

- 提供一个名为`encryptablePropertyDetector`的`EncryptablePropertyDetector`类型的Bean来覆盖默认的实现
- 如果提供的bean名称不为`encryptablePropertyDetector`，可以通过修改yml中的属性`jasypt.encryptor.property.detector-Bean`为自己的bean的名称。

方式

- 要么自定义类
- 要么修改yml中的前缀和后缀
<a name="Y3ZvV"></a>
#### 6.2.1 自定义属性探测器，加入容器
```java
/**
 * 自定义属性探测器
 *  beanName为 encryptablePropertyDetector
 */
@Bean(name = "encryptablePropertyDetector")
public EncryptablePropertyDetector encryptablePropertyDetector() {
    return new MyEncryptablePropertyDetector();
}


/**
 * @description: 自定义的属性探测器
 */
public class MyEncryptablePropertyDetector implements EncryptablePropertyDetector {

    /**
     * 是否为可以解密的字符串
     * @param value 全部的字符串
     * @return 是否是解密的字符串，true，是，false，否
     */
    @Override
    public boolean isEncrypted(String value) {
        if (value != null) {
            return value.startsWith("ENC@");    // 自定义规则为 ENC@开头
        }
        return false;
    }

    /**
     * 截取到除了标识之后的值
     * @param value 带前缀
     * @return string 去掉标识符的字符串
     */
    @Override
    public String unwrapEncryptedValue(String value) {
        return value.substring("ENC@".length());        // 截取ENC@之后的字符串
    }
}
```
yml中的配置
```yaml
jasypt:
  encryptor:
    # 加密算法
    algorithm: PBEWITHHMACSHA512ANDAES_256
    # 加密使用的盐
    password: jaspyt_password
    property:
       # 修改默认的前缀和后缀，如果自定义属性探测器，那么此项配置不起作用
       # prefix: ENC_(
       # suffix: )
       # 自定义的属性探测器，如果这个是自定义的，那么上述的前缀后缀不生效
       detector-bean: encryptablePropertyDetector
```
<a name="X8SMR"></a>
#### 6.2.2 修改yml中的配置
```yaml
spring:
  datasource:
    driver-class-name: com.mysql.cj.jdbc.Driver
    url: jdbc:mysql://192.168.10.31/mp
    username: root
    # 使用ENC()包裹，标识为加密之后的，否则无法解密，会报错
    # 自定义规则之后，使用ENC@开头
    password: ENC@JSrINYe4IBotHndGjX1hnmY3mtPNUJlXjP12cx1+pHqUz2FNXGPu3Frnajh3QCXg
```
<a name="GKVif"></a>
## 7、自定义规则的前缀和后缀
在上述说明中，在`DefaultLazyPropertyDetector`中是默认是通过配置文件中的规则进行匹配的。默认规则是以ENC(开头，以)结尾，可以复写配置来自定义前缀和后缀<br />上面第6条是自定义了属性探测器，包括了定义规则和过滤字符串<br />如果只是想自定义前缀和后缀，那么可以直接修改yml中的配置来修改自定义的前缀和后缀
```yaml
jasypt:
  encryptor:
    # 加密算法
    algorithm: PBEWITHHMACSHA512ANDAES_256
    # 加密使用的盐
    password: jaspyt_password
    property:
      # 修改默认的前缀和后缀，如果自定义属性探测器，那么此项配置不起作用
      prefix: ENC(
      suffix: )
```
<a name="Tcd8i"></a>
## 8、直接自定义解密规则
上述6和7自定义了解密字符串的规则和解密字符串的过滤，但是真正的解析处理还是Jasypt框架来负责的。也可以直接自定义解密的一系列流程。
<a name="GkeQ6"></a>
### 8.1 官方处理流程
<a name="LstDW"></a>
#### 8.1.1 官方的注入
在`EncryptablePropertyResolverConfiguration`类中
```java
@Bean(
    name = {"lazyEncryptablePropertyResolver"}
)
public EncryptablePropertyResolver encryptablePropertyResolver(@Qualifier("lazyEncryptablePropertyDetector") EncryptablePropertyDetector propertyDetector, @Qualifier("lazyJasyptStringEncryptor") StringEncryptor encryptor, BeanFactory bf, EnvCopy envCopy, ConfigurableEnvironment environment) {
    String customResolverBeanName = envCopy.get().resolveRequiredPlaceholders(RESOLVER_BEAN_PLACEHOLDER);
    boolean isCustom = envCopy.get().containsProperty("jasypt.encryptor.property.resolver-bean");
    return new DefaultLazyPropertyResolver(propertyDetector, encryptor, customResolverBeanName, isCustom, bf, environment);
}
```
默认注入的是`DefaultLazyPropertyResolver`但是在其中创建的是`EncryptablePropertyResolver`对象
<a name="M58xF"></a>
#### 8.1.2 `EncryptablePropertyResolver`

1. 官方默认是通过`EncryptablePropertyResolver`接口来处理解析字符串的
```java
public interface EncryptablePropertyResolver {

    /**
     * 处理所有属性的解密处理
     * 如果为检测到加密规则，那么返回实际为相同的字符创
     *
     * @param value 属性值
     * @return 如果值未加密，返回原值，如果加密，返回加密之后的值
     */
    String resolvePropertyValue(String value);
}
```

1. 其真实性使用的实现类是`DefaultPropertyResolver`用来真正处理解析。就是通过调用上文中的`StringEncryptor`处理解密，使用`EncryptablePropertyDetector`定义的解密字符串规则定义是否为加密的字符串
```java
public class DefaultPropertyResolver implements EncryptablePropertyResolver {

    private final Environment environment;
    // 默认的或者自定义的StringEncryptor，用来解密
    private StringEncryptor encryptor;
    // 默认的或者自定义的EncryptablePropertyDetector，用来定义是否为加密的字符串
    private EncryptablePropertyDetector detector;

    public DefaultPropertyResolver(StringEncryptor encryptor, Environment environment) {
        this(encryptor, new DefaultPropertyDetector(), environment);
    }

    public DefaultPropertyResolver(StringEncryptor encryptor, EncryptablePropertyDetector detector, Environment environment) {
        this.environment = environment;
        Assert.notNull(encryptor, "String encryptor can't be null");
        Assert.notNull(detector, "Encryptable Property detector can't be null");
        this.encryptor = encryptor;
        this.detector = detector;
    }

    @Override
    public String resolvePropertyValue(String value) {
        return Optional.ofNullable(value)
            .map(environment::resolvePlaceholders)
            .filter(detector::isEncrypted)  // 如果经过属性探测器确认的，才继续
            .map(resolvedValue -> {
                try {
                    String unwrappedProperty = detector.unwrapEncryptedValue(resolvedValue.trim()); // 过滤加密规则后的字符串
                    String resolvedProperty = environment.resolvePlaceholders(unwrappedProperty); 
                    return encryptor.decrypt(resolvedProperty); // 解密
                } catch (EncryptionOperationNotPossibleException e) {
                    throw new DecryptionException("Unable to decrypt: " + value + ". Decryption of Properties failed,  make sure encryption/decryption " +
                                                  "passwords match", e);
                }
            })
            .orElse(value);
    }
}
```
<a name="WLWwa"></a>
### 8.2 自定义的解密逻辑
编写自己的解密逻辑类<br />加入spring容器，命名为`encryptablePropertyResolver`，或者通过yml方式配置自定义bean名称
```java
@Bean("encryptablePropertyResolver")
public EncryptablePropertyResolver encryptablePropertyResolver(
        StringEncryptor jasyptStringEncryptor, EncryptablePropertyDetector encryptablePropertyDetector) {
    return new MyEncryptablePropertyResolver(jasyptStringEncryptor, encryptablePropertyDetector);
}

/**
 * @description: 直接自定义解密规则
 */
public class MyEncryptablePropertyResolver implements EncryptablePropertyResolver {

    // 处理解密
    private final StringEncryptor encryptor;
    // 属性探测器
    private final EncryptablePropertyDetector detector;

    public MyEncryptablePropertyResolver(StringEncryptor encryptor, EncryptablePropertyDetector detector) {
        this.encryptor = encryptor;
        this.detector = detector;
    }

    /**
     * 处理真正的解密逻辑
     * @param value 原始值
     * @return 如果值未加密，返回原值，如果加密，返回加密之后的值
     */
    @Override
    public String resolvePropertyValue(String value) {
        return Optional.ofNullable(value)
                .filter(detector::isEncrypted)  // 如果经过属性探测器确认的，才继续
                .map(resolvedValue -> {
                    try {
                        String unwrappedProperty = detector.unwrapEncryptedValue(resolvedValue.trim()); // 过滤加密规则后的字符串
                        return encryptor.decrypt(unwrappedProperty); // 解密
                    } catch (EncryptionOperationNotPossibleException e) {
                        throw new DecryptionException("Unable to decrypt: " + value + ". Decryption of Properties failed,  make sure encryption/decryption " +
                                "passwords match", e);
                    }
                })
                .orElse(value);
    }
}
```
yml配置
```yaml
jasypt:
  encryptor:
    # 加密算法
    algorithm: PBEWITHHMACSHA512ANDAES_256
    # 加密使用的盐
    password: jaspyt_password
    property:
       # 修改默认的前缀和后缀，如果自定义属性探测器，那么此项配置不起作用
       # prefix: ENC_(
       # suffix: )
       # 自定义的属性探测器，如果这个是自定义的，那么上述的前缀后缀不生效
       detector-bean: encryptablePropertyDetector
       # 自定义解密逻辑类 如果配置了，默认的解析器将不工作
       resolver-bean: encryptablePropertyResolver
```
<a name="h2OqI"></a>
## 9、自定义过滤器
在Jasypt-spring-boot中，引入了过滤器<br />过滤器filter允许过滤某些属性，不进行解密。默认情况下，jasypt.encryptor开头的所有属性都会将从检查项中排除掉。这是为了配置Bean，在加载时循环依赖
<a name="opiHY"></a>
### 9.1 默认处理流程
<a name="CO7ar"></a>
#### 9.1.1 官方的注入
在`EncryptablePropertyResolverConfiguration`类中
```java
@Bean(
    name = {"lazyEncryptablePropertyFilter"}
)
public EncryptablePropertyFilter encryptablePropertyFilter(EnvCopy envCopy, ConfigurableBeanFactory bf) {
    String customFilterBeanName = envCopy.get().resolveRequiredPlaceholders(FILTER_BEAN_PLACEHOLDER);
    boolean isCustom = envCopy.get().containsProperty("jasypt.encryptor.property.filter-bean");
    return new DefaultLazyPropertyFilter(envCopy.get(), customFilterBeanName, isCustom, bf);
}
```
于上面的逻辑一样，在`DefaultLazyPropertyFilter`中其实是新建了一个`EncryptablePropertyFilter`对象，默认实现类是`DefaultPropertyFilter`
<a name="rq3t5"></a>
#### 9.1.2 `DefaultPropertyFilter`
```java
public class DefaultPropertyFilter implements EncryptablePropertyFilter {

    // 过滤的和包含的，优先读取配置文件的
    private final List<String> includeSourceNames;
    private final List<String> excludeSourceNames;
    private final List<String> includePropertyNames;
    private final List<String> excludePropertyNames;

    public DefaultPropertyFilter() {
        includeSourceNames = null;
        includePropertyNames = null;
        excludeSourceNames = null;
        excludePropertyNames = null;
    }

    public DefaultPropertyFilter(List<String> includeSourceNames, List<String> excludeSourceNames, List<String> includePropertyNames, List<String> excludePropertyNames) {
        this.includeSourceNames = includeSourceNames;
        this.excludeSourceNames = excludeSourceNames;
        this.includePropertyNames = includePropertyNames;
        this.excludePropertyNames = excludePropertyNames;
    }

    // 是否拦截
    @Override
    public boolean shouldInclude(PropertySource<?> source, String name) {
        // 如果上述四个都没有配置，那么全部放行
        if (isIncludeAll()) {
            return true;
        }

        // 如果是不包含的，返回false，就过滤掉了
        if (isMatch(source.getName(), excludeSourceNames) || isMatch(name, excludePropertyNames)) {
            return false;
        }

        // 如果是包含的，就放行
        return isIncludeUnset() || isMatch(source.getName(), includeSourceNames) || isMatch(name, includePropertyNames);

    }

    private boolean isIncludeAll() {
        return isIncludeUnset() && isExcludeUnset();
    }

    private boolean isIncludeUnset() {
        return isEmpty(includeSourceNames) && isEmpty(includePropertyNames);
    }

    private boolean isExcludeUnset() {
        return isEmpty(excludeSourceNames) && isEmpty(excludePropertyNames);
    }

    private boolean isEmpty(List<String> patterns) {
        return patterns == null || patterns.isEmpty();
    }

    // 传递的配置其实是正则，进行正则匹配
    private boolean isMatch(String name, List<String> patterns) {
        return name != null && !isEmpty(patterns) && patterns.stream().anyMatch(name::matches);
    }
}
```
<a name="XvZ0P"></a>
### 9.2 自定义过滤器
方式

- 要么自定义过滤器
- 要么修改`jasypt.encryptor.property.include-names`或者`jasypt.encryptor.property.exclude-names`配置拦截和放行的资源key

自定义过滤器类<br />加入spring容器，命名为`encryptablePropertyFilter`
```java
/**
 * 自定义的属性拦截器
 * @param configProps Jasypt官方读取的配置集合
 * @return 自定义属性拦截器
 */
@Bean(name="encryptablePropertyFilter")
public EncryptablePropertyFilter encryptablePropertyFilter(
        Singleton<JasyptEncryptorConfigurationProperties> configProps) {
    return new MyEncryptablePropertyFilter(configProps.get());
}

/**
 * @description: 自定义的属性过滤器
 */
public class MyEncryptablePropertyFilter implements EncryptablePropertyFilter {

    /** jasypt 的所有配置*/
    JasyptEncryptorConfigurationProperties jasyptProperties;

    public MyEncryptablePropertyFilter(JasyptEncryptorConfigurationProperties jasyptProperties) {
        this.jasyptProperties = jasyptProperties;
    }

    @Override
    public boolean shouldInclude(PropertySource<?> source, String name) {
        List<String> excludeNames = jasyptProperties.getProperty().getFilter().getExcludeNames();
        List<String> includeNames = jasyptProperties.getProperty().getFilter().getIncludeNames();
        if (CollectionUtils.isEmpty(includeNames) && CollectionUtils.isEmpty(excludeNames)) {
            return true;
        }

        if (isMatch(source.getName(), excludeNames) || isMatch(source.getName(), excludeNames)) {
            return false;
        }

        return CollectionUtils.isEmpty(includeNames) ||
                isMatch(source.getName(), includeNames) ||
                isMatch(name, includeNames);

    }

    /**
     * 正则判断，如果满足，返回true，如果不满足，返回false
     * @param name 配置的key
     * @param patterns 正则列表
     * @return 如果满足，返回true，如果不满足，返回false
     */
    private boolean isMatch(String name, List<String> patterns) {
        return name != null && !CollectionUtils.isEmpty(patterns) && patterns.stream().anyMatch(name::matches);
    }

}
```
yml配置
```yaml
jasypt:
  encryptor:
    # 加密算法
    algorithm: PBEWITHHMACSHA512ANDAES_256
    # 加密使用的盐
    password: jaspyt_password
    property:
      # 修改默认的前缀和后缀，如果自定义属性探测器，那么此项配置不起作用
      # prefix: ENC_(
      # suffix: )
      # 自定义的属性探测器，如果这个是自定义的，那么上述的前缀后缀不生效
      detector-bean: encryptablePropertyDetector
      # 自定义解密逻辑类 如果配置了，默认的解析器将不工作
      resolver-bean: encryptablePropertyResolver
      # 过滤器的bean
      filter-bean: encryptablePropertyFilter
      # 过滤器配置，正则
      filter:
        # 默认包含的
        include-names:
        # 默认拦截的，默认拦截jasypt.encryptor的配置
        exclude-names:
          - ^jasypt\.encryptor\.*
```
<a name="A6voZ"></a>
## 10、使用mvn插件加密解密
使用代码的方式比较不方便，还需要编码实现，如果不想编码，简单的进行加密解密，就可以使用maven的插件，使用mvn命令进行加密解密
<a name="m3hBr"></a>
### 10.1 引入Jasypt的maven插件
```xml
<build>
  <plugins>
    <!-- Jasypt 的maven插件 -->
    <plugin>
      <groupId>com.github.ulisesbocchio</groupId>
      <artifactId>jasypt-maven-plugin</artifactId>
      <version>3.0.2</version>
    </plugin>
  </plugins>
</build>
```
<a name="OoemX"></a>
### 10.2 加密
使用jasypt-maven-plugin插件加密明文密码：（如果配置项是默认值，可以不指定）
```bash
mvn jasypt:encrypt-value -Djasypt.encryptor.password="jaspyt_password" -Djasypt.plugin.value="root" -Djasypt.encryptor.algorithm="PBEWITHHMACSHA512ANDAES_256"
```

- `jasypt.encryptor.password` 是秘钥，尽量复杂！不能放在代码和配置文件里面！不能泄漏
- `jasypt.plugin.value` 是要加密的明文密码
- `jasypt.encryptor.algorithm`默认加密算法是`PBEWITHHMACSHA512ANDAES_256`，需要有JCE（Java Cryptography Extension）支持，如果不想安装JCE，可以使用`PBEWithMD5AndDES`算法。windows下的jdk自带

进入项目所在的目录，输入命令，成功加密<br />![2022-11-19-15-15-51.143799200.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1668848277259-b4178ef3-052b-406f-9780-e3daf625f6c4.png#averageHue=%2312100f&clientId=u6b2f8a6a-f0fd-4&from=ui&id=sKxhX&originHeight=836&originWidth=1873&originalType=binary&ratio=1&rotation=0&showTitle=false&size=180708&status=done&style=none&taskId=u22132897-9969-49ea-b46a-725f9b51360&title=)
<a name="i3fo3"></a>
### 10.3 解密
使用jasypt-maven-plugin插件解密密文密码：（如果配置项是默认值，可以不指定）
```bash
mvn jasypt:decrypt-value -Djasypt.encryptor.password="jaspyt_password" -Djasypt.plugin.value="pqsp6kvVfBcKoEltxP9MilGGRo8EE506mDWAuTFIKePDXMeArta13bT6Hl8QqVlC" -Djasypt.encryptor.algorithm="PBEWITHHMACSHA512ANDAES_256"
```

- `jasypt.encryptor.password` 是秘钥，尽量复杂！不能放在代码和配置文件里面！不能泄漏
- `jasypt.plugin.value` 是要加密的明文密码，有ENC()包裹或者不包裹都可以
- `jasypt.encryptor.algorithm`默认加密算法是`PBEWITHHMACSHA512ANDAES_256`，需要有JCE（Java Cryptography Extension）支持，如果不想安装JCE，可以使用`PBEWithMD5AndDES`算法。windows下的jdk自带

进入项目所在的目录，输入命令，成功加密<br />![2022-11-19-15-15-51.339690700.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1668848277255-5cee3843-08c5-4fd0-95c9-6c987b544821.png#averageHue=%2312100f&clientId=u6b2f8a6a-f0fd-4&from=ui&id=ZeLQT&originHeight=838&originWidth=1900&originalType=binary&ratio=1&rotation=0&showTitle=false&size=182976&status=done&style=none&taskId=u86285d7f-8d28-4735-a43e-733c0888cda&title=)
<a name="KZ4sW"></a>
## 11、思维导图
最后再来一张思维导图<br />![2022-11-19-15-15-51.545214200.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1668848277795-67faee29-8cd0-45ff-9391-b839f393e57b.png#averageHue=%23f4f4f4&clientId=u6b2f8a6a-f0fd-4&from=ui&id=u60d241b4&originHeight=1658&originWidth=3341&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1175447&status=done&style=none&taskId=ucf88ee4e-50f0-4d67-88a2-474cdd87179&title=)
