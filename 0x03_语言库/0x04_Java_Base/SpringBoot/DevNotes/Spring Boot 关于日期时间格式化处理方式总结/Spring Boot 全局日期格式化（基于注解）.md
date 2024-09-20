SpringBoot 全局日期格式 注解
<a name="RtHdz"></a>
### 目标
快速学会通过注解 `@JsonComponent`自定义日期格式化的序列化器。
<a name="wN0TB"></a>
### 使用教程
> 根据官方文档 Custom JSON Serializers and Deserializers ，想要接管Jackson的JSON的序列化和反序列化，只需通过注解 `@JsonComponent`来声明其静态内部类即可。

首先根据项目要求提供自定义的日期序列化器和反序列化器，其中包括：

- `DateJsonSerializerextendsJsonSerializer<Date>` 表示将Date格式化为日期字符串。
- `DateJsonDeserializerextendsJsonDeserializer<Date>` 表示将日期字符串解析为Date日期。
```java
/**
 * 全局日期格式化
 */
@JsonComponent
public class DateFormatConfig {
    private static SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    /**
     * 日期格式化
     */
    public static class DateJsonSerializer extends JsonSerializer<Date> {
        @Override
        public void serialize(Date date, JsonGenerator jsonGenerator, SerializerProvider serializerProvider) throws IOException {
            jsonGenerator.writeString(dateFormat.format(date));
        }
    }
    /**
     * 解析日期字符串
     */
    public static class DateJsonDeserializer extends JsonDeserializer<Date> {
        @Override
        public Date deserialize(JsonParser jsonParser, DeserializationContext deserializationContext) throws IOException, JsonProcessingException {
            try {
                return dateFormat.parse(jsonParser.getText());
            } catch (ParseException e) {
                throw new RuntimeException(e);
            }
        }
    }
}
```
然后提供相应的测试信息，这里以查询用户为例：
```java
/**
 * 查询用户信息
 */
@RestController
public class UserController {
    @GetMapping("/")
    public User get() {
        return new User("1", "socks", "123456", new Date(), "GMT");
    }
}
/**
 * 用户信息
 */
public class User {
    private String userId;
    private String username;
    private String password;
    private Date createTime;
    private String timezone;
    public User(String userId, String username, String password, Date createTime, String timezone) {
        this.userId = userId;
        this.username = username;
        this.password = password;
        this.createTime = createTime;
        this.timezone = timezone;
    }
   //省略getters&setters
}
```
接下来启动应用并访问 http://127.0.0.1:8080 ，可以拿到正确结果：
```java
{
  "userId": "1",
  "username": "socks",
  "password": "123456",
  "createTime": "2018-12-26 01:03:25"
}
```
除了日期格式化解析之外，还可以在 `DateFormatConfig` 注入业务变量，例如根据当前登录人的所属时区（虽然 `SimpleDateFormat`默认读取了当地时区，但在实际的国际化系统中，用户的所属时区是指其在系统录入的所属时区，而不是指当地时区。例如Tony这个用户账号挂在GMT+0时区，但此时他出差在香港使用，系统仍需要按照GMT+0时区来显示时间），为了解决这个问题，此时可以在 `DateFormatConfig` 注入当前登录人然后改变日期工具类的 `TimeZone`来动态修改时区。根据当前登录人动态展示时区：
```java
/**
 * 全局日期格式化
 */
@JsonComponent
public class DateFormatConfig {
    private static SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss Z") {
        @Override
        public Date parse(String source) throws ParseException {
            try {
                if (StringUtils.isEmpty(source)) {
                    return null;
                }
                return super.parse(source);
            } catch (Exception e) {
                return new StdDateFormat().parse(source);
            }
        }
    };
    private static UserController userController;//这里是指获取当前登录人的工具类
    @Autowired
    public void setUserController(UserController userController) {
        DateFormatConfig.userController = userController;
    }
    /**
     * 日期格式化
     */
    public static class DateJsonSerializer extends JsonSerializer<Date> {
        @Override
        public void serialize(Date date, JsonGenerator jsonGenerator, SerializerProvider serializerProvider) throws IOException {
            //获取当前登录人的所属时区
            dateFormat.setTimeZone(TimeZone.getTimeZone(userController.get().getTimezone()));
            //格式化日期
            jsonGenerator.writeString(dateFormat.format(date));
        }
    }
    /**
     * 解析日期字符串
     */
    public static class DateJsonDeserializer extends JsonDeserializer<Date> {
        @Override
        public Date deserialize(JsonParser jsonParser, DeserializationContext deserializationContext) throws IOException, JsonProcessingException {
            try {
                //获取当前登录人的所属时区
                dateFormat.setTimeZone(TimeZone.getTimeZone(userController.get().getTimezone()));
                //解析日期
                return dateFormat.parse(jsonParser.getText());
            } catch (ParseException e) {
                throw new RuntimeException(e);
            }
        }
    }
}
```
修改完后重新启动应用并访问 http://127.0.0.1:8080 ，可以拿到正确结果：
```json
{
  "userId": "1",
  "username": "socks",
  "password": "123456",
  "createTime": "2018-12-25 17:35:50 +0000",
  "timezone": "GMT"
}
```
<a name="DgdvP"></a>
### 小结
1、使用注解 `@JsonComponent` 可以快速自定义日期格式化的序列化器，免除传统通过模块注册的烦恼。<br />2、使用注解 `@JsonComponent` 实现与当地无关的动态时区的精髓就在于将获取当前等人的方法写在解析日期和格式化日期的代码里。<br />3、使用注解 `@JsonComponent` 是直接处理String和Date的相互转换的，所以要注意空串问题。例如`dateFormat.parse()`要预防空串。
