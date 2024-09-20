JavaSpringBoot
<a name="UoQUp"></a>
## 一、简介
在实际的项目开发过程中，经常需要将某些变量从代码里面抽离出来，放在配置文件里面，以便更加统一、灵活的管理服务配置信息。比如，数据库、eureka、zookeeper、redis、mq、kafka 等服务组件的连接参数配置，还有自定义的项目参数配置变量。<br />当然，实际上根据当前的业务需求，往往会自定义参数，然后注入到代码里面去，以便灵活使用！<br />一起来看看SpringBoot加载配置文件的几种玩法！<br />SpringBoot 项目在启用时，首先会默认加载bootstrap.properties或者bootstrap.yml这两个配置文件（这两个优先级最高）；接着会加载application.properties或application.yml；如果何配置了spring.profiles这个变量，同时还会加载对应的application-{profile}.properties或者application-{profile}.yml文件，profile为对应的环境变量，比如dev，如果没有配置，则会加载profile=default的配置文件。<br />虽然说配置项都写在同一个配置文件没有问题，但是很多时候仍然希望能分开写，这样会比较清晰，比如zookeeper的配置写在zookeeper.properties，数据库相关的配置写在datasource.properties等等，因此就需要设置加载外部配置文件！<br />具体该如何实现呢，一起来看看！
<a name="FbZ6J"></a>
## 二、代码实践
<a name="WKjv9"></a>
### 2.1、通过`@value`注解实现参数加载
想要在某个类里面注入某个变量，通过`@value`注解就可以简单实现参数的注入！<br />例如application.properties文件里，配置一个config.name的变量key，值为zhangsan
```
//参数定义
config.name=zhangsan
```
然后在对应的类里面，通过参数`@value`注入即可！
```java
@RestController
public class HelloController {
    
    @Value("${config.name}")
    private String config;

    @GetMapping("config")
    public String config(){
        return JSON.toJSONString(config);
    }
}
```
使用`@value`注解注入配置，通常情况下有个要求就是，注解里面的变量，必须在application.properties文件里面事先定义好，否则启动报错！<br />当然，如果不想让它抱错报错，可以给它一个缺省值xxx，比如：
```java
@Value("${config.name:xxx}")
private String config;
```
这样，SpringBoot 项目在启用时不会报错！
<a name="wCUd8"></a>
### 2.2、通过`@ConfigurationProperties`注解实现参数加载
某些场景下，`@value`注解并不能满足所有的需求，比如参数配置的数据类型是一个对象或者数组，这个时候才用`@ConfigurationProperties`会是一个比较好的选择！

- 配置一个对象类型的参数

例如在application.properties文件里，配置一个对象类型的参数，可以这样操作！
```
//参数定义
config2.name=demo_1
config2.value=demo_value_1
```
然后，创建一个配置类Config2，用于将定义的变量映射到配置类里面。
```java
@Component
@ConfigurationProperties(prefix = "config2")
public class Config2 {

    public String name;

    public String value;

    //...get、set
}
```
读取数据的方式，也很简单，直接注入到对应的类里面就可以了
```java
@RestController
public class HelloController {
    
    @Autowired
    private Config2 config2;

    @GetMapping("config2")
    public String config2(){
        return JSON.toJSONString(config2);
    }
}
```

- 配置一个Map类型的参数

例如在application.properties文件里，配置一个 Map 类型的参数，可以这样操作！
```
//参数定义
config3.map1.name=demo_id_1_name
config3.map1.value=demo_id_1_value
config3.map2.name=demo_id_2_name
config3.map2.value=demo_id_2_value
```
然后，创建一个配置类Config3，用于将定义的变量映射到配置类里面。
```java
@Component
@ConfigurationProperties(prefix = "config3")
public class Config3 {

    private Map<String, String> map1 = new HashMap<>();

    private Map<String, String> map2 = new HashMap<>();

    //...get、set
}
```
读取数据的方式，与之类似！
```java
@RestController
public class HelloController {
    
    @Autowired
    private Config3 config3;

    @GetMapping("config3")
    public String config3(){
        return JSON.toJSONString(config3);
    }
}
```

- 配置一个List类型的参数

例如在application.properties文件里，配置一个 List 类型的参数，可以这样操作！
```
//参数定义
config4.userList[0].enable=maillist_1_enable
config4.userList[0].name=maillist_1_name
config4.userList[0].value=maillist_1_value

config4.userList[1].enable=maillist_2_enable
config4.userList[1].name=maillist_2_name
config4.userList[1].value=maillist_2_value

config4.userList[2].enable=maillist_3_enable
config4.userList[2].name=maillist_3_name
config4.userList[2].value=maillist_3_value
```
然后，创建一个配置类Config4，用于将定义的变量映射到配置类里面。
```java
@Component
@ConfigurationProperties(prefix = "config4")
public class Config4 {

    private List<UserEntity> userList;

    public List<UserEntity> getUserList() {
        return userList;
    }

    public void setUserList(List<UserEntity> userList) {
        this.userList = userList;
    }
}
```
```java
public class UserEntity {

    private String enable;
    private String name;
    private String value;

    //...get、set
}
```
读取数据的方式，与之类似！
```java
@RestController
public class HelloController {
    
    @Autowired
    private Config4 config4;

    @GetMapping("config4")
    public String config4(){
        return JSON.toJSONString(config4);
    }
}
```
<a name="OUAiw"></a>
### 2.3、通过`@PropertySource`注解实现配置文件加载
正如最开始所介绍的，很多时间，希望将配置文件分开写，比如zookeeper组件对应的服务配置文件是zookeeper.properties，redis组件对应的服务配置文件是redis.properties等等。<br />这种自定义的配置文件，应该如何加载到Spring容器里面呢？<br />其实方法也很简单，通过`@PropertySource`就可以实现！<br />首先，在resources资源文件夹下，创建两个配置文件test.properties和bussiness.properties，内容如下！<br />test.properties文件内容：
```
aaa.a1=aa1123
aaa.a2=aa2123
aaa.a3=aa3123
aaa.a4=aa4123
```
bussiness.properties文件内容：
```
bbbb.a1=bb1123
bbbb.a2=bb2123
bbbb.a3=bb3123
bbbb.a4=bb4123
```
在SpringBoot启动类上加载配置文件即可！
```java
@SpringBootApplication
@PropertySource(value = {"test.properties","bussiness.properties"})
public class PropertyApplication {

    public static void main(String[] args) {
        SpringApplication.run(PropertyApplication.class, args);
    }
}
```
读取数据的方式，与之类似！
```java
@RestController
public class HelloController {
    
    @Value("${aaa.a2}")
    private String a2;

    @Value("${bbbb.a1}")
    private String bbbbA1;
    
    @GetMapping("a2")
    public String a2(){
        return JSON.toJSONString(a2);
    }

    @GetMapping("bbbbA1")
    public String bbbbA1(){
        return JSON.toJSONString(bbbbA1);
    }
}
```
如果只是在业务中需要用到自定义配置文件的值，这样引入并没有什么问题；但是如果某些自定义的变量，在项目启动的时候需要用到，这种方式会存在一些问题，原因如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663894455989-ad81b4b1-eafb-4bed-8a27-e87a2ded9073.png#clientId=u2a4b2b19-a3b9-4&from=paste&id=u15141f13&originHeight=139&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0036917a-4f6b-4d8b-a677-2dc43b09940&title=)<br />翻译过来的意思就是说：<br />虽然在`@SpringBootApplication`上使用`@PropertySource`似乎是在环境中加载自定义资源的一种方便而简单的方法，但不推荐使用它，因为SpringBoot在刷新应用程序上下文之前就准备好了环境。使用`@PropertySource`定义的任何键都加载得太晚，无法对自动配置产生任何影响。<br />因此，如果某些参数是启动项变量，建议将其定义在application.properties或application.yml文件里面，这样就不会有问题！<br />或者，采用【**自定义环境处理类**】来实现配置文件的加载！
<a name="BRqCn"></a>
### 2.4、通过自定义环境处理类，实现配置文件的加载
实现方法也很简单，首先，创建一个实现自`EnvironmentPostProcessor`接口的类，然后自行加载配置文件。
```java
public class MyEnvironmentPostProcessor implements EnvironmentPostProcessor {


    @Override
    public void postProcessEnvironment(ConfigurableEnvironment environment, SpringApplication application) {
        //自定义配置文件
        String[] profiles = {
                "test.properties",
                "bussiness.properties",
                "blog.yml"
        };

        //循环添加
        for (String profile : profiles) {
            //从classpath路径下面查找文件
            Resource resource = new ClassPathResource(profile);
            //加载成PropertySource对象，并添加到Environment环境中
            environment.getPropertySources().addLast(loadProfiles(resource));
        }
    }

    //加载单个配置文件
    private PropertySource<?> loadProfiles(Resource resource) {
        if (!resource.exists()) {
            throw new IllegalArgumentException("资源" + resource + "不存在");
        }
        if(resource.getFilename().contains(".yml")){
            return loadYaml(resource);
        } else {
            return loadProperty(resource);
        }
    }

    /**
     * 加载properties格式的配置文件
     * @param resource
     * @return
     */
    private PropertySource loadProperty(Resource resource){
        try {
            //从输入流中加载一个Properties对象
            Properties properties = new Properties();
            properties.load(resource.getInputStream());
            return new PropertiesPropertySource(resource.getFilename(), properties);
        }catch (Exception ex) {
            throw new IllegalStateException("加载配置文件失败" + resource, ex);
        }
    }

    /**
     * 加载yml格式的配置文件
     * @param resource
     * @return
     */
    private PropertySource loadYaml(Resource resource){
        try {
            YamlPropertiesFactoryBean factory = new YamlPropertiesFactoryBean();
            factory.setResources(resource);
            //从输入流中加载一个Properties对象
            Properties properties = factory.getObject();
            return new PropertiesPropertySource(resource.getFilename(), properties);
        }catch (Exception ex) {
            throw new IllegalStateException("加载配置文件失败" + resource, ex);
        }
    }
}
```
接着，在resources资源目录下，还需要创建一个文件META-INF/spring.factories，通过spi方式，将自定义环境处理类加载到Spring处理器里面，当项目启动时，会自动调用这个类！
```
#启用我们的自定义环境处理类
org.springframework.boot.env.EnvironmentPostProcessor=com.example.property.env.MyEnvironmentPostProcessor
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663894456052-c9ebe95d-f964-4e0c-b459-d9eaf62d0542.png#clientId=u2a4b2b19-a3b9-4&from=paste&id=uc414d9f3&originHeight=411&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2b0812b4-9e18-4251-be09-9a924cb7fca&title=)<br />这种自定义环境处理类方式，相对会更佳灵活，首先编写一个通用的配置文件解析类，支持properties和yml文件的读取，然后将其注入到Spring容器里面，基本上可以做到一劳永逸！
<a name="yoXik"></a>
### 2.5、最后，来介绍一下yml文件读取
在上文中，大部分都是以properties为案例进行介绍，可能有的人已经踩过坑了，**在项目中使用**`**@PropertySource**`**注解来加载yml文件，结果启动直接报错，原因是**`**@PropertySource**`**不支持直接解析yml文件，只能解析properties文件**。<br />**那如果想单独解析yml文件，也不想弄一个【自定义环境处理类】这种方式来读取文件，应该如何处理呢**？<br />操作方式也很简单，以自定义的blog.yml文件为例！<br />blog.yml文件内容：
```yaml
pzblog:
  name: helloWorld
```
然后，创建一个读取yml文件的配置类
```java
@Configuration
public class ConfigYaml {

    /**
     * 加载YML格式自定义配置文件
     * @return
     */
    @Bean
    public static PropertySourcesPlaceholderConfigurer properties() {
        PropertySourcesPlaceholderConfigurer configurer = new PropertySourcesPlaceholderConfigurer();
        YamlPropertiesFactoryBean yaml = new YamlPropertiesFactoryBean();
        yaml.setResources(new ClassPathResource("blog.yml"));
        configurer.setProperties(yaml.getObject());
        return configurer;
    }
}
```
读取数据的方式，与之类似！
```java
@RestController
public class HelloController {
    
    @Value("${pzblog.name}")
    private String pzblogName;

    @GetMapping("pzblogName")
    public String pzblogName(){
        return JSON.toJSONString(pzblogName);
    }
}
```
<a name="qzUJC"></a>
## 三、小结
主要围绕 SpringBoot 加载配置文件的几种实现方式，做了一次内容总结。
