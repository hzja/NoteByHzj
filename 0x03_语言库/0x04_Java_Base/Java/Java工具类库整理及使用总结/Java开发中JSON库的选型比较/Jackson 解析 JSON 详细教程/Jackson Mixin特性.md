Java Jackson<br />Mixin对于前端开发者可不陌生，Vue、React等知名前端框架都使用了Mixin。而对于后端开发，尤其是Java后端开发来说Mixin却是一个很陌生的概念。通过Jackson让后端开发者也来认识一下Mixin。
<a name="ejhS2"></a>
## 场景
比如引用了一个Jar包，其中的某个类在某个场景需要反序列化，但是这个类没有提供默认构造。咋办呢？把原来的项目拉下来，重写一下？下下策! 可以使用Jackson提供的Mixin特性来解决这个问题。
<a name="lXmZi"></a>
## Jackson中的Mixin
Jackson中的Mixin（混入）可以这样解读它：将目标对象无法实现的序列化或反序列化功能通过一个混入对象进行配置，在序列化或反序列化的时候把这些个性化配置混入到目标对象中。混入不改变目标对象本身的任何特性，混入对象和目标对象是映射的关系。接下来实现一个混入的DEMO。
<a name="Zp5nk"></a>
## Mixin的实现
有一个User类，为了演示需要，极端一些，实际开发中不太会出现这种极端情况。这个User没有无参构造，也没有属性的`getter`方法。
```java
public class User {
    private final String name;
    private final Integer age;

    public User(String name, Integer age) {
        this.name = name;
        this.age = age;
    }
    @Override
    public String toString() {
        return "User{" +
                "name='" + name + '\'' +
                ", age=" + age +
                '}';
    }
}
```
<a name="nNKZO"></a>
### 编写Mixin类
对这个极端的User进行序列化和反序列化。按以前的玩法是在User类上加上`@JsonAutoDetect`注解就可以实现序列化了；加上`@JsonDeserialize`注解并指定反序列化类就可以反序列化了。不过不需要对User进行任何更改，只需要编写一个`Mixin`类把上述两个注解配置好就可以了。
```java
@JsonAutoDetect(fieldVisibility = JsonAutoDetect.Visibility.ANY, getterVisibility = JsonAutoDetect.Visibility.NONE,
                isGetterVisibility = JsonAutoDetect.Visibility.NONE)
@JsonIgnoreProperties(ignoreUnknown = true)
@JsonDeserialize(using = UserMixin.UserDeserializer.class)
public abstract class UserMixin {

    /**
     * 反序列化类
     **/
    static class UserDeserializer extends JsonDeserializer<User> {

        @Override
        public User deserialize(JsonParser p, DeserializationContext ctxt) throws IOException {
            ObjectMapper mapper = (ObjectMapper) p.getCodec();
            JsonNode jsonNode = mapper.readTree(p);

            String name = readJsonNode(jsonNode, "name").asText(null);
            String age = readJsonNode(jsonNode, "age").asText(null);
            Integer ageVal = Objects.isNull(age)? null: Integer.valueOf(age);
            return new User(name,ageVal);
        }

        private JsonNode readJsonNode(JsonNode jsonNode, String field) {
            return jsonNode.has(field) ? jsonNode.get(field) : MissingNode.getInstance();
        }
    }

}
```
<a name="YY31w"></a>
### Mixin映射目标类
编写完Mixin类后，通过`ObjectMapper`中的`addMixIn`方法把`UserMixin`和`User`映射起来。并编写一个序列化和反序列化的例子。
```java
ObjectMapper objectMapper = new ObjectMapper();
objectMapper.addMixIn(User.class, UserMixin.class);

User fe = new User("fe", 12);
String json = objectMapper.writeValueAsString(fe);
//{"name":"fe","age":12} 
System.out.println("json = " + json);

String jsonStr = "{\"name\":\"fe\",\"age\":12}";

User user = objectMapper.readValue(jsonStr, User.class);
// User{name='fe', age=12}
System.out.println("user = " + user);
```
这样在不对目标类进行任何改变的情况下实现了个性化的JSON序列化和反序列化。
<a name="ahjKX"></a>
### Jackson中的Module
Jackson还提供了模块化功能，可以将个性化配置进行模块化统一管理，而且可以按需引用，甚至可插拔。它同样能够管理一组Mixin。声明一个Jackson Module非常简单，继承`SimpleModule`覆写它的一些方法即可。针对Mixin可以这样写：
```java
public class UserModule extends SimpleModule {
   public UserModule() {
       super(UserModule.class.getName());
   }

   @Override
   public void setupModule(SetupContext context) {
        context.setMixInAnnotations(User.class,UserMixin.class);
   }
}
```
Module同样可以注册到`ObjectMapper`中，同样也能实现想要的效果：
```java
ObjectMapper objectMapper = new ObjectMapper();
objectMapper.registerModule(new UserModule());
// 省略
```
Module的功能更加强大。平常会使用以下几个Module:

- jackson-module-parameter-names  此模块能够访问构造函数和方法参数的名称
- jackson-datatype-jdk8 除了Java8的时间API外其它新特性的的支持
- jackson-datatype-jsr310  用以支持Java8新增的JSR310时间API

另外Spring Security也提供了Module支持`SecurityJackson2Modules`，它包含了下面的一些模块：
```java
ObjectMapper mapper = new ObjectMapper();
mapper.enableDefaultTyping(ObjectMapper.DefaultTyping.NON_FINAL, JsonTypeInfo.As.PROPERTY);
mapper.registerModule(new CoreJackson2Module());
mapper.registerModule(new CasJackson2Module());
mapper.registerModule(new WebJackson2Module());
mapper.registerModule(new WebServletJackson2Module());
mapper.registerModule(new WebServerJackson2Module());
mapper.registerModule(new OAuth2ClientJackson2Module());
```
建议看一下`SecurityJackson2Modules`源码，研究并模仿一下Module的使用。
