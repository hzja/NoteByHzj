SpringBoot
<a name="uDSBS"></a>
## 1、通过`@Value`注解读取配置
<a name="ff9yk"></a>
### 1.1 List类型的数据
<a name="pHJrj"></a>
#### 定义参数
`.yml` 文件配置如下：
```yaml
test:  
  list:  
    - aaa  
    - bbb  
    - ccc 
```
`.properties` 文件配置如下：
```
test.list[0]=aaa  
test.list[1]=bbb  
test.list[2]=ccc  
```
<a name="auTtA"></a>
#### 使用参数
使用 `@Value` 注解去读取这个值
```java
@Value("${test.list}")  
private List<String> testList;  
```
程序直接报错了，报错信息如下：
```
java.lang.IllegalArgumentException: Could not resolve placeholder 'test.list' in value "${test.list}"  
```
需要通过一个有List类型的属性的配置类才可以注入
```java
@Configuration  
@ConfigurationProperties("test")  
public class TestListConfig {  
    private List<String> list;  
  
    public List<String> getList() {  
        return list;  
    }  
  
    public void setList(List<String> list) {  
        this.list = list;  
    }  
}  
```
在使用时候。采用自动注入的方式，去获取值：
```java
@Autowired  
private TestListConfig testListConfig;  
  
// testListConfig.getList();  
```
这种方式十分的不方便，最大的问题是配置和代码高耦合了，增加一个配置，还需要对配置类做增减改动。
<a name="UvJMp"></a>
### 1.2 数组类型的数据
```yaml
test:  
  array1: aaa,bbb,ccc  
  array2: 111,222,333  
  array3: 11.1,22.2,33.3  
```
使用方式如下
```java
@Value("${test.array1}")  
private String[] testArray1;  
  
@Value("${test.array2}")  
private int[] testArray2;  
  
@Value("${test.array3}")  
private double[] testArray3;  
```
这样就能够直接使用，如果对应的索引下没有具体的值会报错，可以使用 `:` 来设置默认值
```java
@Value("${test.array1:}")  
private String[] testArray1;  
  
@Value("${test.array2:}")  
private int[] testArray2;  
  
@Value("${test.array3:}")  
private double[] testArray3;  
```
仅仅多了一个 `:` 号，冒号后的值表示当 key 不存在时候使用的默认值，使用默认值时数组的 length = 0。<br />总结下使用数组实现的优缺点：<br />**优点**：

- 不需要写配置类<br />
- 使用逗号分割，一行配置，即可完成多个数值的注入，配置文件更加精简<br />

**缺点**：

- 业务代码中数组使用很少，基本需要将其转换为 List，去做 contains、foreach 等操作。
<a name="vEA9L"></a>
### 1.3 使用 EL 表达式将数组数据转换为List、Set、Map
<a name="kxKZF"></a>
#### 解析 List
```yaml
test:  
  list: aaa,bbb,ccc  
```
在调用时，借助 `EL` 表达式的 `split()` 函数进行切分即可。
```java
@Value("#{'${test.list}'.split(',')}")  
private List<String> testList;  
```
同样，为它加上默认值，避免不配置这个 key 时候程序报错：
```java
@Value("#{'${test.list:}'.split(',')}")  
private List<String> testList;  
```
:::danger
但是这样有个问题，当不配置该 key 值，默认值会为空串，它的 length = 1（不同于数组，length = 0），这样解析出来 list 的元素个数就不是空了。<br />这个问题比较严重，因为它会导致代码中的判空逻辑执行错误。这个问题也是可以解决的，在 `split()` 之前判断下是否为空即可。
:::
```java
@Value("#{'${test.list:}'.empty ? null : '${test.list:}'.split(',')}")  
private List<String> testList;  
```
<a name="OU5UK"></a>
#### 解析 Set
解析 Set 和解析 List 本质上是相同的，唯一的区别是 Set 会做去重操作。
```yaml
test:  
  set: 111,222,333,111  
```
```java
@Value("#{'${test.set:}'.empty ? null : '${test.set:}'.split(',')}")  
private Set<Integer> testSet;  
  
// output: [111, 222, 333]  
```
<a name="F5uxt"></a>
#### 解析 Map
解析 Map 的写法如下所示，value 为该 map 的 JSON 格式，注意这里使用的引号：整个 JSON 串使用引号包裹，value 值使用引号包裹。
```yaml
test:  
  map1: '{"name": "zhangsan", "sex": "male"}'  
  map2: '{"math": "90", "english": "85"}'  
```
在程序中，利用 EL 表达式注入：
```java
@Value("#{${test.map1}}")  
private Map<String,String> map1;  
  
@Value("#{${test.map2}}")  
private Map<String,Integer> map2;  
```
:::danger
注意，使用这种方式，必须得在配置文件中配置 key 及其 value。
:::
如果真的很需要这个功能，就得自己写解析方法了，这里以使用 fastjson 进行解析为例：
<a name="aVMFc"></a>
##### (1) 自定义解析方法
```java
public class MapDecoder {  
    public static Map<String, String> decodeMap(String value) {  
        try {  
            return JSONObject.parseObject(value, new TypeReference<Map<String, String>>(){});  
        } catch (Exception e) {  
            return null;  
        }  
    }  
}  
```
<a name="33wp2"></a>
##### (2) 在程序中指定解析方法
```java
@Value("#{T(com.github.jitwxs.demo.MapDecoder).decodeMap('${test.map1:}')}")  
private Map<String, String> map1;  

@Value("#{T(com.github.jitwxs.demo.MapDecoder).decodeMap('${test.map2:}')}")  
private Map<String, String> map2;  
```
<a name="P9xSM"></a>
### 使用注意
 `@Value` 注解不能和 `@AllArgsConstructor` 注解同时使用，否则会报错
```
Consider defining a bean of type 'java.lang.String' in your configuration  
```
这种做法唯一不优雅的地方就是，这样写出来的 `@Value` 的内容都很长，既不美观，也不容易阅读。
<a name="uaSOh"></a>
## 2、通过 `@ConfigurationProperties` 注解读取配置属性
随着业务复杂度的增加，一个项目中可能会有越来越多的微服务，某个模块可能需要调用多个微服务获取不同的信息，那么就需要在配置文件中配置多个微服务的地址。可是，在需要调用这些微服务的代码中，如果一个个去使用 `@Value` 注解引入相应的微服务地址的话，太过于繁琐，也不科学。<br />所以，在实际项目中，业务繁琐，逻辑复杂的情况下，需要考虑封装一个或多个配置类。假如在当前服务中，某个业务需要同时调用订单微服务、用户微服务和购物车微服务，分别获取订单、用户和购物车相关信息，然后对这些信息做一定的逻辑处理。那么在配置文件中，需要将这些微服务的地址都配置好：
```yaml
# 配置多个微服务的地址
url:
  # 订单微服务的地址
  orderUrl: http://localhost:8002
  # 用户微服务的地址
  userUrl: http://localhost:8003
  # 购物车微服务的地址
  shoppingUrl: http://localhost:8004
```
也许实际业务中，远远不止这三个微服务，甚至十几个都有可能。对于这种情况，可以先定义一个 `MicroServiceUrl` 类来专门保存微服务的 url，如下：
```java
@Component
@ConfigurationProperties(prefix = "url")
public class MicroServiceUrl {
    private String orderUrl;
    private String userUrl;
    private String shoppingUrl;
    // 省去get和set方法
}
```
这里使用 `@ConfigurationProperties` 注解并且使用 prefix 来指定一个前缀，然后该类中的属性名就是配置中去掉前缀后的名字，一一对应即可。即：前缀名 + 属性名就是配置文件中定义的 key。同时，该类上面需要加上 `@Component` 注解，把该类作为组件放到Spring容器中，让 Spring 去管理，使用的时候直接注入即可。<br />需要注意的是，使用 `@ConfigurationProperties` 注解需要导入它的依赖：
```xml
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-configuration-processor</artifactId>
  <optional>true</optional>
</dependency>
```
直接通过 `@Resource` 注解将刚刚写好配置类注入进来即可使用了，非常方便。如下：
```java
@RestController
@RequestMapping("/test")
public class TestController {
    private static final Logger LOGGER = LoggerFactory.getLogger(TestController.class);
    @Resource
    private MicroServiceUrl microServiceUrl;
    
    @RequestMapping("/config")
    public String testConfig() {
        LOGGER.info("=====获取的订单服务地址为：{}", microServiceUrl.getOrderUrl());
        LOGGER.info("=====获取的用户服务地址为：{}", microServiceUrl.getUserUrl());
        LOGGER.info("=====获取的购物车服务地址为：{}", microServiceUrl.getShoppingUrl());
        return "success";
    }
}
```
启动项目，可以看到，控制台打印出如下信息，说明配置文件生效，同时正确获取配置文件内容：
```
=====获取的订单服务地址为：http://localhost:8002
=====获取的订单服务地址为：http://localhost:8002
=====获取的用户服务地址为：http://localhost:8003
=====获取的购物车服务地址为：http://localhost:8004
```
<a name="OUZY6"></a>
## 3、通过Environment Bean读取
<a name="0DeMY"></a>
### 3.1 定义配置内容
```yaml
test:
	msg: aaa
```
<a name="SXn3F"></a>
### 3.2 读取配置内容
```java
@RestController
@RequestMapping("/test")
public class TestController {
	@Autowired
    private Environment env

    @RequestMapping(value = "index2", method = RequestMethod.GET)
    public String index2() {
        System.out.println(env.getProperty("test.msg"));
        return "The Way 2 : "+ env.getProperty("test.msg");
    }
}     
```
