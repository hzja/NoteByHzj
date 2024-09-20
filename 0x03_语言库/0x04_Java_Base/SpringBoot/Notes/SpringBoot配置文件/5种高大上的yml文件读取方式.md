JavaSpringBootyml<br />除了烂大街的`@Value`和`@ConfigurationProperties`外，还能够通过哪些方式，来读取yml配置文件的内容。
<a name="yimKj"></a>
## 1、`Environment`
在Spring中有一个类`Environment`，它可以被认为是当前应用程序正在运行的环境，它继承了`PropertyResolver`接口，因此可以作为一个属性解析器使用。先创建一个yml文件，属性如下：
```yaml
person:
  name: hydra
  gender: male
  age: 18
```
使用起来也非常简单，直接使用`@Autowired`就可以注入到要使用的类中，然后调用它的`getProperty()`方法就可以根据属性名称取出对应的值了。
```java
@RestController
public class EnvironmentController {
    @Autowired
    private Environment environment;

    @GetMapping("envTest")
    private void getEnv(){
        System.out.println(environment.getProperty("person.name"));
        System.out.println(environment.getProperty("person.gender"));

        Integer autoClose = environment
            .getProperty("person.age", Integer.class);
        System.out.println(autoClose);
        String defaultValue = environment
            .getProperty("person.other", String.class, "defaultValue");
        System.out.println(defaultValue);
    }
}
```
在上面的例子中可以看到，除了简单的获取外，`Environment`提供的方法还可以对取出的属性值进行类型转换、以及默认值的设置，调用一下上面的接口，打印结果如下：
```java
hydra
male
18
defaultValue
```
除了获取属性外，还可以用来判断激活的配置文件，先在application.yml中激活pro文件：
```yaml
spring:
  profiles:
    active: pro
```
可以通过`acceptsProfiles`方法来检测某一个配置文件是否被激活加载，或者通过`getActiveProfiles`方法拿到所有被激活的配置文件。测试接口：
```java
@GetMapping("getActiveEnv")
private void getActiveEnv(){
    System.out.println(environment.acceptsProfiles("pro"));
    System.out.println(environment.acceptsProfiles("dev"));

    String[] activeProfiles = environment.getActiveProfiles();
    for (String activeProfile : activeProfiles) {
        System.out.println(activeProfile);
    }
}
```
打印结果：
```java
true
false
pro
```
<a name="T0UbQ"></a>
## 2、`YamlPropertiesFactoryBean`
在Spring中还可以使用`YamlPropertiesFactoryBean`来读取自定义配置的yml文件，而不用再被拘束于application.yml及其激活的其他配置文件。<br />在使用过程中，只需要通过`setResources()`方法设置自定义yml配置文件的存储路径，再通过`getObject()`方法获取`Properties`对象，后续就可以通过它获取具体的属性，下面看一个例子：
```java
@GetMapping("fcTest")
public void ymlProFctest(){
    YamlPropertiesFactoryBean yamlProFb = new YamlPropertiesFactoryBean();
    yamlProFb.setResources(new ClassPathResource("application2.yml"));
    Properties properties = yamlProFb.getObject();
    System.out.println(properties.get("person2.name"));
    System.out.println(properties.get("person2.gender"));
    System.out.println(properties.toString());
}
```
查看运行结果，可以读取指定的application2.yml的内容：
```java
fcant
female
{person2.age=18, person2.gender=female, person2.name=fcant}
```
但是这样的使用中有一个问题，那就是只有在这个接口的请求中能够取到这个属性的值，如果再写一个接口，不使用`YamlPropertiesFactoryBean`读取配置文件，即使之前的方法已经读取过这个yml文件一次了，第二个接口取到的仍然还是空值。来对这个过程进行一下测试：
```java
@Value("${person2.name:null}")
private String name;
@Value("${person2.gender:null}")
private String gender;

@GetMapping("fcTest2")
public void ymlProFctest2(){
    System.out.println(name);
    System.out.println(gender);
}
```
先调用一次`fcTest`接口，再调用`fcTest2`接口时会打印`null`值：
```java
null
null
```
想要解决这个问题也很简单，可以配合`PropertySourcesPlaceholderConfigurer`使用，它实现了`BeanFactoryPostProcessor`接口，也就是一个bean工厂后置处理器的实现，可以将配置文件的属性值加载到一个`Properties`文件中。使用方法如下：
```java
@Configuration
public class PropertyConfig {
    @Bean
    public static PropertySourcesPlaceholderConfigurer placeholderConfigurer() {
        PropertySourcesPlaceholderConfigurer configurer 
            = new PropertySourcesPlaceholderConfigurer();
        YamlPropertiesFactoryBean yamlProFb 
            = new YamlPropertiesFactoryBean();
        yamlProFb.setResources(new ClassPathResource("application2.yml"));
        configurer.setProperties(yamlProFb.getObject());
        return configurer;
    }
}
```
再次调用之前的接口，结果如下，可以正常的取到application2.yml中的属性：
```java
fcant
female
```
除了使用`YamlPropertiesFactoryBean`将yml解析成`Properties`外，其实还可以使用`YamlMapFactoryBean`解析yml成为`Map`，使用方法非常类似：
```java
@GetMapping("fcMapTest")
public void ymlMapFctest(){
    YamlMapFactoryBean yamlMapFb = new YamlMapFactoryBean();
    yamlMapFb.setResources(new ClassPathResource("application2.yml"));
    Map<String, Object> map = yamlMapFb.getObject();
    System.out.println(map);
}
```
打印结果：
```java
{person2={name=fcant, gender=female, age=18}}
```
<a name="LSZ0S"></a>
## 3、监听事件
SpringBoot是通过监听事件的方式来加载和解析的yml文件，那么也可以仿照这个模式，来加载自定义的配置文件。<br />首先，定义一个类实现`ApplicationListener`接口，监听的事件类型为`ApplicationEnvironmentPreparedEvent`，并在构造方法中传入要解析的yml文件名：
```java
public class YmlListener implements 
    ApplicationListener<ApplicationEnvironmentPreparedEvent> {
    private String ymlFilePath;
    public YmlListener(String ymlFilePath){
        this.ymlFilePath = ymlFilePath;
    }
    //...
}
```
自定义的监听器中需要实现接口的`onApplicationEvent()`方法，当监听到`ApplicationEnvironmentPreparedEvent`事件时会被触发：
```java
@Override
public void onApplicationEvent(ApplicationEnvironmentPreparedEvent event) {
    ConfigurableEnvironment environment = event.getEnvironment();
    ResourceLoader loader = new DefaultResourceLoader();
    YamlPropertySourceLoader ymlLoader = new YamlPropertySourceLoader();
    try {
        List<PropertySource<?>> sourceList = ymlLoader
            .load(ymlFilePath, loader.getResource(ymlFilePath));
        for (PropertySource<?> propertySource : sourceList) {
            environment.getPropertySources().addLast(propertySource);
        }
    } catch (IOException e) {
        e.printStackTrace();
    }
}
```
上面的代码中，主要实现了：

- 获取当前环境`Environment`，当`ApplicationEnvironmentPreparedEvent`事件被触发时，已经完成了`Environment`的装载，并且能够通过`event`事件获取
- 通过`YamlPropertySourceLoader`加载、解析配置文件
- 将解析完成后的`OriginTrackedMapPropertySource`添加到`Environment`中

修改启动类，在启动类中加入这个监听器：
```java
public static void main(String[] args) {
    SpringApplication application = new SpringApplication(MyApplication.class);
    application.addListeners(new YmlListener("classpath:/application2.yml"));
    application.run(args);
}
```
在向`environment`中添加`propertySource`前加一个断点，查看环境的变化。<br />执行完成后，可以看到配置文件源已经被添加到了环境中。<br />启动完成后再调用一下接口，查看结果：
```java
Fcant
female
```
能够正确的取到配置文件中的值，说明自定义的监听器已经生效。
<a name="Xg52l"></a>
## 4、SnakeYml
前面介绍的几种方式，在Spring环境下无需引入其他依赖就可以完成的，接下来要介绍的SnakeYml在使用前需要引入依赖，但是同时也可以脱离Spring环境单独使用。先引入依赖坐标：
```xml
<dependency>
    <groupId>org.yaml</groupId>
    <artifactId>snakeyaml</artifactId>
    <version>1.23</version>
</dependency>
```
准备一个yml配置文件：
```yaml
person1:
  name: hydra
  gender: male
person2:
  name: fcant
  gender: female
```
在使用`SnakeYml`解析yml时，最常使用的就是`load`、`loadlAll`、`loadAs`方法，这三个方法可以加载yml文件或字符串，最后返回解析后的对象。先从基础的`load`方法开始演示：
```java
public void test1(){
    Yaml yaml=new Yaml();
    Map<String, Object> map =
            yaml.load(getClass().getClassLoader()
                    .getResourceAsStream("snake1.yml"));
    System.out.println(map);
}
```
运行上面的代码，打印Map中的内容：
```java
{person1={name=hydra, gender=male}, person2={name=fcant, gender=female}}
```
接下来看一下loadAll方法，它可以用来加载yml中使用---连接符连接的多个文档，将上面的yml文件进行修改：
```yaml
person1:
  name: hydra
  gender: male
---
person2:
  name: fcant
  gender: female
```
在添加了连接符后，尝试再使用`load`方法进行解析，报错显示发现了另一段yml文档从而无法正常解析。<br />这时候修改上面的代码，使用`loadAll`方法：
```java
public void test2(){
    Yaml yaml=new Yaml();
    Iterable<Object> objects = 
        yaml.loadAll(getClass().getClassLoader()
            .getResourceAsStream("snake2.yml"));
    for (Object object : objects) {
        System.out.println(object);
    }
}
```
执行结果如下：
```java
{person1={name=hydra, gender=male}}
{person2={name=fcant, gender=female}}
```
可以看到，`loadAll`方法返回的是一个对象的迭代，里面的每个对象对应yml中的一段文档，修改后的yml文件就被解析成了两个独立的Map。<br />接下来再来看一下`loadAs`方法，它可以在yml解析过程中指定类型，直接封装成一个对象。直接复用上面的snake1.yml，在解析前先创建两个实体类对象用于接收：
```java
@Data
public class Person {
    SinglePerson person1;
    SinglePerson person2;
}

@Data
public class SinglePerson {
    String name;
    String gender;
}
```
下面使用`loadAs`方法加载yml，注意方法的第二个参数，就是用于封装yml的实体类型。
```java
public void test3(){
    Yaml yaml=new Yaml();
    Person person = 
        yaml.loadAs(getClass().getClassLoader().
            getResourceAsStream("snake1.yml"), Person.class);
    System.out.println(person.toString());
}
```
查看执行结果：
```java
Person(person1=SinglePerson(name=hydra, gender=male), person2=SinglePerson(name=fcant, gender=female))
```
实际上，如果想要将yml封装成实体对象，也可以使用另一种方法。在创建Yaml对象的时候，传入一个指定实体类的构造器对象，然后直接调用`load`方法就可以实现：
```java
public void test4(){
    Yaml yaml=new Yaml(new Constructor(Person.class));
    Person person = yaml.load(getClass().getClassLoader().
            getResourceAsStream("snake1.yml"));
    System.out.println(person.toString());
}
```
执行结果与上面相同：
```java
Person(person1=SinglePerson(name=hydra, gender=male), person2=SinglePerson(name=fcant, gender=female))
```
SnakeYml其实实现了非常多的功能，这里就不一一列举了，有兴趣的小伙伴可以自己查看一下文档。其实在SpringBoot的底层，也是借助了SnakeYml来进行的yml的解析操作。
<a name="a2KXI"></a>
## 5、jackson-dataformat-yaml
相比大家平常用jackson比较多的场景是用它来处理json，其实它也可以用来处理yml，使用前需要引入依赖：
```xml
<dependency>
    <groupId>com.fasterxml.jackson.dataformat</groupId>
    <artifactId>jackson-dataformat-yaml</artifactId>
    <version>2.12.3</version>
</dependency>
```
使用jackson读取yml也非常简单，这里用到了常用的`ObjectMapper`，在创建`ObjectMapper`对象时指定使用YAML工厂，之后就可以简单的将yml映射到实体：
```java
public void read() throws IOException {
    ObjectMapper objectMapper = new ObjectMapper(new YAMLFactory());
    InputStream input =
        new FileInputStream("F:\\Work\\yml\\src\\main\\resources\\snake1.yml");
    Person person = objectMapper.readValue(input, Person.class);
    System.out.println(person.toString());
}
```
运行结果：
```java
Person(person1=SinglePerson(name=hydra, gender=male), person2=SinglePerson(name=fcant, gender=female))
```
如果想要生成yml文件的话，可以调用`ObjectMapper`的`writeValue`方法实现：
```java
public void write() throws IOException {
    Map<String,Object> map=new HashMap<>();
    SinglePerson person1 = new SinglePerson("Trunks", "male");
    SinglePerson person2 = new SinglePerson("Goten", "male");
    Person person=new Person(person1,person2);
    map.put("person",person);

    ObjectMapper objectMapper = new ObjectMapper(new YAMLFactory());
    objectMapper
            .writeValue(new File("F:\\Work\\yml\\src\\main\\resources\\jackson-gen.yml"),map);
}
```
查看生成的yml文件，可以看到jackson对字符串类型严格的添加了引号，还在文档的开头添加了yml的链接符。至于其他jackson读写yml的复杂功能，可以在工作中自己去探索使用。
<a name="uefzG"></a>
## 总结
这里介绍了5种读取yml配置文件的方式，前3种依赖于Spring环境，而`SnakeYml`和`Jackson`则可以脱离环境独立使用，可以说它们是对`@Value`和`@ConfigurationProperties`注解使用的补充。这几种方法的使用场景不同，也各有各的有优点，各自具备一些特殊的用法，而在工作中更多情况下，要根据具体的用途进行一种方案的选取或多种的搭配使用。
