Java Jackson
<a name="eSvg9"></a>
## Jackson注解一览
<a name="dIMA2"></a>
### `@JacksonAnnotation`
这个注解经常用于Jackson自定义注解中，用来标记这是一个Jackson注解，可以用它来实现自定义的序列化注解。
<a name="auVBx"></a>
### `@JacksonAnnotationsInside`
这个注解用来标记Jackson复合注解，当使用多个Jackson注解组合成一个自定义注解时会用到它。
```java
/**
 * 非空以及忽略未知属性
 **/
@Retention(RetentionPolicy.RUNTIME)
@JacksonAnnotationsInside
@JsonInclude(Include.NON_NULL)
@JsonIgnoreProperties(ignoreUnknown = true)
public @interface NotNullAndIgnoreAnnotation {}
```
<a name="Taqc0"></a>
### `@JacksonInject`
json属性值将在反序列化时可以被注入，先在属性上标记：
```java
@Data
public final class JacksonInjectUser {
    @JacksonInject(value = "dynamic")
    private String name;
    private Integer age;
}
```
然后name的值就可以在反序列化的时候动态化，不再需要去解析、拼字段。
```java
@SneakyThrows
@Test
void jacksonInject() {
    // 这个值动态化了
    String dynamicValue = "some Dynamic value";
    InjectableValues.Std injectableValues = new InjectableValues.Std()
        // 名称和注解中声明的相同才行
        .addValue("dynamic", dynamicValue);
    JacksonInjectUser jacksonInjectUser = objectMapper.setInjectableValues(injectableValues)
        // 空json 最后居然可以赋值
        .readValue("{}", JacksonInjectUser.class);
    Assertions.assertEquals(dynamicValue,jacksonInjectUser.getName());
}
```
注意：`@JacksonInject`中提供了`useInput`参数进行绑定策略控制。
<a name="OUa1f"></a>
### `@JsonAlias`
在反序列化的时候来对Java Bean的属性进行名称绑定，可以绑定多个json的键名。举个例子：
```java
@SneakyThrows
@Test
void jsonAlias(){
    // 两个json的类型结构是相同的 可以定义一个Bean来接收
    String userJson = "{\"name\": \"fcant.cn\",\"age\": 22}";
    String itemJson = "{\"category\": \"coco\", \"count\": 50 }";
    Domain user = objectMapper.readValue(userJson, Domain.class);
    Assertions.assertEquals("fcant.cn",user.getStr());
    Assertions.assertEquals(22,user.getNum());
    Domain item = objectMapper.readValue(itemJson, Domain.class);
    Assertions.assertEquals("coco",item.getStr());
    Assertions.assertEquals(50,item.getNum());
}

@Data
public class Domain{
    @JsonAlias({"name","category"})
    private String str;
    @JsonAlias({"age","count"})
    private Integer num;
}
```
注意：只能用于json反序列化。
<a name="SHOHN"></a>
### `@JsonAnyGetter`
在json序列化时可以将Bean中的java.util.Map类型的属性“平铺展开”，举个例子：<br />某个Java Bean正常的json序列化结果是：
```json
{
    "name": "fcant.cn",
    "age": 22,
    "unMatched": {
        "unknown": "unknown"
        }
}
```
但是需要：
```json
{
  "name": "fcant.cn",
  "age": 22,
  "unknown": "unknown"
}
```
可以对Java Bean这么标记：
```java
@Data
public class MapUser {
    private String name;
    private Integer age;
    private Map<String,Object> unMatched;

    @JsonAnyGetter
    public Map<String, Object> getUnMatched() {
        return unMatched;
    }
}
```
然后来试一试：
```java
@SneakyThrows
@Test
void jsonAnyGetter(){
    MapUser mapUser = new MapUser();

    mapUser.setName("fcant.cn");
    mapUser.setAge(22);
    mapUser.setUnMatched(Collections.singletonMap("unknown","unknown"));

    String json = objectMapper.writeValueAsString(mapUser);
    // 获取json中unknown节点的值
    Object read = JsonPath.parse(json)
            .read(JsonPath.compile("$.unknown"));
    Assertions.assertEquals("unknown",read);
}
```
不过这个注解的使用也是有条件的：

- 不能是静态方法。
- 必须是无参方法。
- 方法的返回值必须是java.util.Map。
- 一个实体中只能使用一个该注解。
<a name="HBurb"></a>
### `@JsonAnySetter`
正好和`@JsonAnyGetter`相反，这里就不介绍了。
<a name="H7msy"></a>
### `@JsonAutoDetect`
一般情况下，认为Jackson序列化对象的前提是有无参构造并且有Getter方法。事实上下面这个类依然可以序列化成json:
```java
@JsonAutoDetect(fieldVisibility = JsonAutoDetect.Visibility.ANY)
public class ConstructUser {
    private final String name;
    private final Integer age;

    public ConstructUser(String name, Integer age) {
        this.name = name;
        this.age = age;
    }
}
```
可以通过调整Java Bean中属性、`getter`方法、`isGetter`方法、`setter`方法、初始化实例的方法。可见级别可以分为：

- `DEFAULT`：需要根据上下文来判断，一般基于父类的可见性。
- `ANY`：任何级别的都可以自动识别。
- `NONE`：所有级别都不可以自动识别。
- `NON_PRIVATE`：非`private`修饰的可以自动识别。
- `PROTECTED_AND_PUBLIC`：被`protected`和`public`修饰的可以被自动识别。
- `PUBLIC_ONLY`：只有被`public`修饰的才可以被自动识别。
<a name="ltlVj"></a>
### `@JsonBackReference`
这个注解经常和另一个注解`@JsonManagedReference`成对出现，它为了解决递归的问题，例如两个类互相持有对方：
```java
Info info = new Info();
Player player = new Player();
player.setId(1);
info.setPlayer(player);
player.setInfo(info);
// 直接无限递归了
String InfiniteRecursionError = objectMapper.writeValueAsString(player);
```
json序列化的时候直接无限递归了。如果想得到下面的序列化结果：<br />// player
```json
{"id":1,"info":{"id":0}}
```
就需要在类`Player`的Info属性上标记`@JsonManagedReference`，同时在Info类中的Player属性上标记`@JsonBackReference`注解。<br />如果想在序列化Player时直接忽略掉Info属性，即期望得到`{"id":1}`，只需要在Player的Info属性上标记`@JsonBackReference`注解。
<a name="HZyeI"></a>
### `@JsonClassDescription`
Jackson对json schemas的支持，用来生成整个json的描述信息。
<a name="wtceV"></a>
### `@JsonCreator`
Jackson在反序列化时默认会去找Java Bean的无参构造，但是有些Bean没有无参构造，这时`@JsonCreator`就派上用场了。可以将它标记在构造方法或静态工厂方法上，通常它还需要同`@JsonProperty`或`@JacksonInject`配合，就像这样：
```java
@Getter
public class DescriptionUser {
    private final String name;
    private final Integer age;

    @JsonCreator
    public DescriptionUser(@JsonProperty("name") String name, 
                           @JsonProperty("age") Integer age) {
        this.name = name;
        this.age = age;
    }
}
```
对应的单元测试：
```java
@SneakyThrows
@Test
void jsonCreator() {
    String json = "{\"name\": \"fcant.cn\",\"age\": 22}";
    DescriptionUser user = objectMapper.readValue(json, DescriptionUser.class);
    Assertions.assertEquals("fcant.cn", user.getName());
}
```
可以在静态初始化实例工厂方法上试试这个注解。
<a name="gYHcH"></a>
### `@JsonEnumDefaultValue`
在定义性别枚举时往往只定义了男和女两个性别。不能指望用户守规矩。科学的方法是定义一个枚举用来兜底。就像这样：
```java
public enum Gender {
    /**
     * Female gender.
     */
    FEMALE,
    /**
     * Male gender.
     */
    MALE,
    /**
     * Unknown gender.
     */
    UNKNOWN
}
```
当用户乱填的时候都定义为未知。在jackson反序列化支持设置一个默认值来兜底。可以在`Gender#UNKNOWN`上标记`@JsonEnumDefaultValue`，然后反序列化：
```java
@SneakyThrows
@Test
void jsonEnumDefaultValue(){
    // 开启未知枚举值使用默认值特性
    objectMapper.enable(DeserializationFeature.READ_UNKNOWN_ENUM_VALUES_USING_DEFAULT_VALUE);
    String maleJson = "{\"name\": \"fcant.cn\",\"age\": 22,\"gender\":\"MALE\"}";

    EnumUser male = objectMapper.readValue(maleJson, EnumUser.class);
    Assertions.assertEquals(Gender.MALE,male.getGender());

    String unknownJson = "{\"name\": \"fcant.cn\",\"age\": 22,\"gender\":\"notClear\"}";
    EnumUser unknownGender = objectMapper.readValue(unknownJson, EnumUser.class);
    Assertions.assertEquals(Gender.UNKNOWN,unknownGender.getGender());
}
```
注意：必须手动jackson开启未知枚举值使用默认值特性。
<a name="ltJvq"></a>
### `@JsonFilter`
同一个实体类根据不同的场景可能需要不同的序列化策略。比如对于A用户实体的某些字段可见，对于B用户另一些字段可见，实现动态的数据字段权限。这种情况下，jackson中其它一些静态注解就很难实现，借助于`@JsonFilter`反而简单了，下面是实现方法：
```java
// 只序列化age的策略
@JsonFilter("role_a")
public class OnlyAge extends FilterUser{
}
// 不序列化age的策略 
@JsonFilter("role_b")
public class OnlyNameAndGender extends FilterUser{
}
```
接下来定义role_a和role_b的策略：
```java
@SneakyThrows
@Test
void jsonFilter() {
    SimpleFilterProvider simpleFilterProvider = new SimpleFilterProvider();
    // role_a只展示age  
    SimpleBeanPropertyFilter onlyAgeFilter = SimpleBeanPropertyFilter.filterOutAllExcept("age");
    // role_b只排除age
    SimpleBeanPropertyFilter exceptAgeFilter = SimpleBeanPropertyFilter.serializeAllExcept("age");
    simpleFilterProvider.addFilter("role_a", onlyAgeFilter);
    simpleFilterProvider.addFilter("role_b", exceptAgeFilter);
    objectMapper.setFilterProvider(simpleFilterProvider);

    //被JsonFilter标记的类
    OnlyAge onlyAgeUser = new OnlyAge();
    onlyAgeUser.setName("fcant.cn");
    onlyAgeUser.setGender(Gender.MALE);
    onlyAgeUser.setAge(22);

    OnlyNameAndGender onlyNameAndGenderUser = new OnlyNameAndGender();
    onlyNameAndGenderUser.setName("fcant.cn");
    onlyNameAndGenderUser.setGender(Gender.MALE);
    onlyNameAndGenderUser.setAge(22);

    String onlyAge = objectMapper.writeValueAsString(onlyAgeUser);
    // 序列化的json中找不到name节点会抛出PathNotFoundException异常
    Assertions.assertThrows(PathNotFoundException.class, () -> JsonPath.parse(onlyAge)
            .read(JsonPath.compile("$.name")));
    String onlyNameAndGender = objectMapper.writeValueAsString(onlyNameAndGenderUser);
    // 序列化的json中找不到age节点会抛出PathNotFoundException异常
    Assertions.assertThrows(PathNotFoundException.class, () -> JsonPath.parse(onlyNameAndGender)
            .read(JsonPath.compile("$.age")));
}
```
思考：结合AOP甚至是Spring Security是不是有新的实现？
<a name="hqLW6"></a>
### `@JsonFormat`
用于序列化和反序列化中特定格式的数据。虽然经常使用它来格式化时间，但是它不单单能格式化时间。
<a name="SkCN2"></a>
#### 格式化时间
这种比较常用，主要用于格式化旧时间API：
```java
@Data
public class JsonFormatUser {

    @JsonFormat(shape = JsonFormat.Shape.NUMBER)
    private Date number;
    @JsonFormat(shape = JsonFormat.Shape.STRING, pattern = "yyyy-MM-dd HH:mm:ss",timezone = "GMT+8")
    private Date yyyymmdd;
    @JsonFormat(locale = "zh_CN")
    private Date cnDate;

}
```
三种shape分别输出时间戳，根据时区和既定格式格式化、本地化：
```json
{
  "number" : 1626706386340,
  "yyyymmdd" : "2021-07-19 22:53:06",
  "cnDate" : "2021-07-19T14:53:06.340+00:00"
}
```
说实话，现在都使用新的时间API，这个注解并不推荐使用。
:::tips
注意：格式化时间需要带时区。
:::
<a name="TR6iP"></a>
#### 格式化枚举
```java
public enum GenderEnum {

    /**
     * Female gender.
     */
    FEMALE("0","女"),
    /**
     * Male gender.
     */
    MALE("1","男"),
    /**
     * Unknown gender.
     */
    @JsonEnumDefaultValue
    UNKNOWN("-1","未知");

    private final String value;
    private final String description;

    GenderEnum(String value, String description) {
        this.value = value;
        this.description = description;
    }

    public String getValue() {
        return value;
    }

    public String getDescription() {
        return description;
    }

}
```
上面这种枚举类只能格式化成枚举名称，很多时候期望能够获取键值对的枚举格式，例如`GenderEnum.FEMALE`：
```json
{"value":"0","description":"女"}
```
只需要使用`@JsonFormat`的`shape`特性：
```java
@JsonFormat(shape = JsonFormat.Shape.OBJECT)
public enum GenderEnum {
// 省略
}
```
<a name="dVvyb"></a>
### `@JsonGetter`和`@JsonGetter`
json序列化和反序列化时指定属性的Getter和Setter方法。特别针对有些不正规的方法，同时还可以指定别名，例子：
```java
public class GetterAndSetter {
    private String name;

    @JsonGetter("n")
    public String name(){
        return this.name;
    }

    @JsonSetter("name")
    public void name(String name){
        this.name= name;
    }
}
```
断言测试：
```java
GetterAndSetter getterAndSetter = new GetterAndSetter();
getterAndSetter.name("fcant.cn");

String s = objectMapper.writeValueAsString(getterAndSetter);
Object n = JsonPath.parse(s)
        .read(JsonPath.compile("$.n"));
Assertions.assertEquals("fcant.cn",n);

String json = "{\"name\":\"fcant.cn\"}";

GetterAndSetter getAndSet = objectMapper.readValue(json, GetterAndSetter.class);

Assertions.assertEquals("fcant.cn",getAndSet.name());
```
大部分情况下这两个注解比`JsonProperty`注解更加通用。
<a name="booNN"></a>
### `@JsonIdentityInfo`
这个作用于类或属性上，被用来在序列化/反序列化时为该对象或字段添加一个对象识别码，比如`@id`或者`Class`对象名，主要解决字段循环嵌套的问题，例如数据库中的多对多关系，Bean嵌套依赖。开发ORM的相关功能时会用到。
:::info
扩展：`@JsonIdentityReference` 具有类似的功能，强调了使用id作为标识。
:::
<a name="YDrSt"></a>
### `@JsonIgnore`
这个也是常用的一个注解。在序列化/反序列化时忽略被该注解标记的属性。这个注解和前面介绍的`@JsonFilter`提供的功能差不多。不过该注解是静态标记。
:::tips
注意：`JsonProperty`注解的`access`也可以实现该注解的功能，不建议两个注解混用，这样可能发生冲突。
:::
<a name="SLm21"></a>
### `@JsonIgnoreProperties`
这个也经常使用。在序列化/反序列化时忽略多个属性，标记在类上。例如忽略`internalId`和`secretKey`属性：
```java
@JsonIgnoreProperties({ "internalId", "secretKey" }
```
如果有些属性不太确定也可以通过该注解过滤掉，避免未知属性异常：
```java
@JsonIgnoreProperties(ignoreUnknown=true)
```
<a name="fghyf"></a>
### `@JsonIgnoreType`
在序列化/反序列化时如果希望忽略掉某种特定类型可以借助于该注解：
```java
@JsonIgnoreType
class Credentials {
    public String password;
}

class Settings {
    public int userId;
    public String name;
    public Credentials pwd;
}
```
在`Settings`进行序列化和反序列化时`Credentials`将会被忽略掉。主要用来对一些数据敏感的对象进行忽略，比如用户的凭据。
<a name="IDzGi"></a>
### `@JsonInclude`
用于指示属性何时可以被序列化，可以把该注解标记到属性字段上，也可以通过`setSerializationInclusion` 方法统一设置。常用的`JsonInclude.Include.NON_NULL`可以过滤空值：
```java
Player player = new Player();
player.setId(1);
player.setName(null);
```
对应：
```json
{
  "id":1
}
```
其它策略参见`JsonInclude.Include`。
:::info
扩展：使用CUSTOM策略时可以实现自定义测过滤方法。
:::
<a name="GAOpU"></a>
### `@JsonIncludeProperties`
这个注解机制有点类似`@JsonIgnoreProperties`，只不过它的功能和`@JsonIgnoreProperties`相反。如果一个类标记了这个注解：
```java
@JsonIncludeProperties({ "internalId", "secretKey" })
```
除了`internalId`和`secretKey`属性，其它属性都不参与序列化和反序列化。
<a name="ajExO"></a>
### `@JsonProperty`
`@JsonProperty`也是常用注解。用来标记属性或者属性的`getter`和`setter`方法上，用于指定属性的json名称，类似`@JsonAlias`的效果，同时配合其`Access`枚举可以实现那些属性可以序列化，那些属性可以反序列化（类似忽略的效果）。
```java
@Data
public class MapUser {
    @JsonProperty(value = "myname")
    private String name;
    @JsonProperty(value = "a")
    private Integer age;
}

// {"myname":"fcant.cn","a":22,"}
```
<a name="tE2Am"></a>
## 小结
Jackson是一款非常优秀的json类库，提供了丰富的注解来满足各种场景的需要。根据日常一些场景的需要结合这些注解设计了不少动态的、可扩展的、通用的序列化和反序列化功能，用起来非常方便顺手。
