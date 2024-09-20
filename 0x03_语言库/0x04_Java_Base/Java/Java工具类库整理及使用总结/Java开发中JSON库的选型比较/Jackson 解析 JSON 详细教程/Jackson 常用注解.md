JavaJackson
<a name="NUVop"></a>
### `@JacksonInject`
通过 `@JacksonInject` 注解可以在 Jackson 反序列化的时候为空值字段动态赋值，当反序列化的对应属性值不存在时，可通过该注解为其动态设置值。
```java
@Getter
@Setter
public class User {

	@JacksonInject(value = "dynamic")
	private String name;

	private Integer age;
}
```
对其进行反序列化：
```java
public class JacksonInjectTest {

	private static final ObjectMapper MAPPER = new ObjectMapper();

	public static void main(String[] args) throws JsonProcessingException {
		// 名称和注解中声明的相同才行
		InjectableValues.Std injectableValues = new InjectableValues.Std()
			.addValue("dynamic", "some dynamic value");
		MAPPER.setInjectableValues(injectableValues);
		// 反序列化空对象
		User u = MAPPER.readValue("{}", User.class);
		System.out.println(u.getName());
	}

}
// 输出结果：some dynamic value
```
<a name="lrvrl"></a>
### `@JsonAlias`
通过 `@JsonAlias` 注解可以为对象属性定义一个或多个别名，然后在反序列化时根据别名进行属性映射。但要注意，别名不能与其他属性名冲突，否则反序列化时会抛异常。
```java
@Getter
@Setter
public class User {

	@JsonAlias(value = "nm")
	private String name;

	private Integer age;

}
```
对其进行反序列化：
```java
public static void main(String[] args) throws JsonProcessingException {
    User u = MAPPER.readValue("{\"nm\":\"张三\"}", User.class);
    System.out.println(u.getName());
}
// 输出结果：张三
```
<a name="wc9Is"></a>
### `@JsonAnyGetter`、`@JsonAnySetter`
`@JsonAnyGetter` 注解可用于将一组键值对平铺展开到常规属性中，比如某个对象正常序列化的结果为：
```json
{
	"name": "张三",
	"info": {
		"address": "china",
		"age": "25"
	}
}
```
通过该注解修饰后，序列化后的结果为：
```json
{
  "name": "张三",
  "address": "china",
  "age": "25"
}
```
不过这个注解的使用也是有条件限制的：

- 被修饰的方法不能是静态方法
- 被修饰的方法必须是无参方法
- 被修饰的方法返回值必须是 Map 类型
- 同一实体中只能有一个方法使用该注解

具体如下例所示：
```java
@Getter
@Setter
public class User {

	private String name;

	private Map<String, String> info;

	@JsonAnyGetter
	public Map<String, String> getInfo() {
		return info;
	}
}
```
对该对象进行序列化：
```java
public static void main(String[] args) throws JsonProcessingException {
    Map<String, String> info = new HashMap<>();
    info.put("age", "25");
    info.put("address", "china");
    User user = new User();
    user.setName("张三");
    user.setInfo(info);
    System.out.println(MAPPER.writeValueAsString(user));
}
// 输出结果：{"name":"张三","address":"china","age":"25"}
```
<a name="Bkm6f"></a>
### `@JsonAnySetter`
注解的功能与 `@JsonAnyGetter` 正好相反，使用示例如下：
```java
@Getter
@Setter
public class User {

	private String name;


	private String address;

	private String age;

	@JsonAnySetter
	public void setInfo(Map<String, String> info) {
		this.address = info.get("address");
		this.age = info.get("age");
	}

}
```
对其进行反序列化：
```java
public static void main(String[] args) throws JsonProcessingException {
    User user = MAPPER.readValue("{\"name\":\"张三\",\"info\":{\"address\":\"china\",\"age\":\"25\"}}", User.class);
    System.out.println(user.getAddress());
}
// 输出结果：china
```
<a name="Z9Vos"></a>
### `@JsonAutoDetect`
`@JsonAutoDetect` 注解可以用于强制序列化私有属性，即使没有提供 Getter 方法：
```java
@Setter
@JsonAutoDetect(fieldVisibility = JsonAutoDetect.Visibility.ANY)
public class User {

	private String name;

	private String age;

}
```
没有为 User 提供 Getter 方法，对其进行序列化：
```java
public static void main(String[] args) throws JsonProcessingException {
	User user = new User();
	user.setName("张三");
	user.setAge("25");
	System.out.println(MAPPER.writeValueAsString(user));

}
// 输出结果：{"name":"张三","age":"25"}
```
<a name="mN9fh"></a>
### `@JsonBackReference`、`@JsonManagedReference`
`@JsonManagedReference` 注解和 `@JsonBackReference` 注解用于处理父子关系并解决对象间的循环引用，比如两个类互相持有对方的场景：
```java
@Getter
@Setter
public class User {

	private String name;
	@JsonManagedReference
	private Friend friend;

}

@Getter
@Setter
public class Friend {

	private String number;
	@JsonBackReference
	private User user;

}
```
对其进行序列化：
```java
public static void main(String[] args) throws JsonProcessingException {
    User user = new User();
    user.setName("张三");
    Friend friend = new Friend();
    friend.setNumber("001");
    friend.setUser(user);
    user.setFriend(friend);
    System.out.println(MAPPER.writeValueAsString(user));
}
// 输出结果：{"name":"张三","friend":{"number":"001"}}
```
<a name="jQxXF"></a>
### `@JsonCreator`
Jackson 在反序列化时默认会通过对象的无参构造方法创建对象，如果想要通过自定义的构造方法创建对象，需要通过 `@JsonCreator` 来指定构造方法，并通过 `@JsonProperty` 设置构造方法中参数对应的 JSON 属性名：
```java
@Getter
public class User {

	private final String name;

	private final String age;

	@JsonCreator
	public User(@JsonProperty("name") String name, @JsonProperty("age") String age)
		this.name = name;
	this.age = age;
}
```
反序列化：
```java
public static void main(String[] args) throws JsonProcessingException {
	User user = MAPPER.readValue("{\"name\":\"张三\",\"age\":\"25\"}", User.class);
	System.out.println(user.getName());
}

// 输出结果：张三
```
<a name="OTBHf"></a>
### `@JsonEnumDefaultValue`
反序列化时，如果对应的枚举值不存在 Jackson 默认会抛出异常。可以通过 `@JsonEnumDefaultValue` 注解为未知的枚举类型赋一个默认值来兜底，但要记得在 `ObjectMapper` 中手动开启该功能。
```java
@Setter
@Getter
public static class User {
	private String name;
	private Sex sex;
}

private enum Sex {
	MAN,
	WOMAN,
	@JsonEnumDefaultValue
	UNKNOWN
}
```
反序列化：
```java
public static void main(String[] args) throws JsonProcessingException {
    MAPPER.enable(DeserializationFeature.READ_UNKNOWN_ENUM_VALUES_USING_DEFAULT_VALUE);
    User user = MAPPER.readValue("{\"name\":\"张三\",\"sex\":\"NONE\"}", User.class);
    System.out.println(user.getSex());
}
// 输出结果：UNKNOWN
```
<a name="kNx8s"></a>
### `@JsonFormat`
默认序列化会将 Date 类型解析成时间戳，如果是 Java 8 提供的 `LocalDateTime` 则默认不支持序列化，需要额外注册 Module 支持，可以通过 `@JsonFormat` 注解在序列化过程中对数据进行格式化输出：<br />格式化时间（只支持 Date 类型）：
```java
@Setter
@Getter
public static class User {

	@JsonFormat(shape = JsonFormat.Shape.STRING, pattern = "yyyy-MM-dd HH:mm:ss", timezone = "GMT+8")
	private Date birthday;

}
// 序列化输出：{"birthday":"2021-09-06 21:13:42"}
```
格式化枚举类型（默认只输出枚举名称，如果有枚举属性会忽略）：
```java
@Getter
@JsonFormat(shape = JsonFormat.Shape.OBJECT)
public enum Sex {
	MAN(1, "男生"),
	WOMAN(2, "女生");

	private final Integer code;

	private final String description;

	Sex(Integer code, String description) {
		this.code = code;
		this.description = description;
	}

}
// 序列化输出：{"sex":{"code":1,"description":"男生"}}
```
<a name="AxE5R"></a>
### `@JsonGetter`、`@JsonSetter`
`@JsonGetter`、`@JsonSetter` 注解用于在序列化和反序列化时指定属性的 Getter 和 Setter 方法。特别针对有些不正规的方法：
```java
public class User {

	private String name;

	@JsonGetter("name")
	public String returnName() {
		return name;
	}

	@JsonSetter("name")
	public void putName(String name) {
		this.name = name;
	}

}
```
User 类没有提供标准的 Setter、Getter 方法，对其进行序列化和反序列化：
```java
public static void main(String[] args) throws JsonProcessingException {
	User user = new User();
	user.putName("张三");
	System.out.print(MAPPER.writeValueAsString(user));
	User userD = MAPPER.readValue("{\"name\":\"张三\"}", User.class);
	System.out.print(userD.returnName());
}
// 输出结果：{"name":"张三"}张三
```
<a name="crIGo"></a>
### `@JsonIdentityInfo`
`@JsonIdentityInfo` 注解作用于类或属性上，在序列化、反序列化时可以为该对象或字段添加一个对象识别码，比如 `@id` 或者 Class 对象名，主要用于解决字段循环嵌套的问题。
```java
@Getter
@Setter
public class User {

	private String name;

	@JsonIdentityInfo(generator = ObjectIdGenerators.UUIDGenerator.class)
	private User friend;

}
```
循环嵌套后进行序列化：
```java
public static void main(String[] args) throws JsonProcessingException {
	User userA = new User();
	userA.setName("张三");
	User userB = new User();
	userB.setName("李四");
	// 循环引用
	userA.setFriend(userB);
	userB.setFriend(userA);
	System.out.println(MAPPER.writeValueAsString(userA));
}
// 输出结果：
{
	"name": "张三",
	"friend": {
		"@id": "5eba3199-ed97-4642-81b8-ad7b61ed9fd8",
		"name": "李四",
		"friend": {
			"@id": "fddcb9fa-c934-4735-8fec-2923675976cc",
			"name": "张三",
			"friend": "5eba3199-ed97-4642-81b8-ad7b61ed9fd8"
			}
	}
}
```
<a name="GvDpd"></a>
### `@JsonIgnore`
`@JsonIgnore` 也是常用的一个注解，在序列化、反序列化时会忽略被该注解标记的属性。
```java
@Getter
@Setter
public class User {

	@JsonIgnore
	private String name;
	private String age;

}
```
对其进行序列化和反序列化：
```java
public static void main(String[] args) throws JsonProcessingException {
	User user = new User();
	user.setName("张三");
	user.setAge("25");
	System.out.print(MAPPER.writeValueAsString(user));
	User userD = MAPPER.readValue("{\"name\":\"张三\",\"age\":\"25\"}", User.class);
	System.out.print(userD.getName());
}
// 输出结果：{"age":"25"}null
```
<a name="rMfB2"></a>
### `@JsonIgnoreProperties`
`@JsonIgnoreProperties` 注解可以在序列化、反序列化时忽略多个属性，通常标记在类上。
```java
@Getter
@Setter
@JsonIgnoreProperties({"name", "age"})
public class User {

	private String name;

	private String age;

	private String sex;

}
// 序列化结果：{"sex":"男"}
```
如果有些属性不太确定也可以通过该注解过滤掉，避免未知属性异常：
```java
@JsonIgnoreProperties(ignoreUnknown = true)
```
此外，还提供了 allowGetters、allowSetters 属性用于仅忽略 Setter 或 Getter：
```java
@JsonIgnoreProperties(allowGetters = true, allowSetters = true)
```
<a name="lgaeO"></a>
### `@JsonIgnoreType`
`@JsonIgnoreType` 注解用于在序列化、反序列化时忽略掉某种特定类型，作用在类上，引用这个类的其他对象在序列化、反序列化时会忽略这个类，主要用来对一些数据敏感对象进行忽略。
```java
@Getter
@Setter
public class User {

	private String name;

	private Hobby hobby;

}

@Getter
@Setter
@JsonIgnoreType
public class Hobby {

	private String item;

}
```
对其进行序列化、反序列化：
```java
public static void main(String[] args) throws JsonProcessingException {
    User user = new User();
    user.setName("张三");﻿
    Hobby hobby = new Hobby();
    hobby.setItem("basketball");
    user.setHobby(hobby);
    System.out.print(MAPPER.writeValueAsString(user));
    User userD = MAPPER.readValue("{\"name\":\"张三\",\"hobby\":{\"item\":\"basketball\"}}", User.class);
    System.out.print(userD.getHobby() == null);
}
// 输出结果：{"name":"张三"}true
```
<a name="l9njV"></a>
### `@JsonInclude`
`@JsonInclude` 注解用于标识对象属性何时可以被序列化，可以把该注解标记在属性字段上，也可以通过 `ObjectMapper` 的 `setSerializationInclusion` 方法进行统一设置。Jackson 默认会把空值字段序列化为 null，可以通过 `JsonInclude.Include.NON_NULL` 来过滤掉空值字段：
```java
@Getter
@Setter
public class User {

	private String name;

	@JsonInclude(JsonInclude.Include.NON_NULL)
	private String age;

}
```
对其进行序列化：
```java
public static void main(String[] args) throws JsonProcessingException {
	User user = new User();
	user.setName("张三");
	System.out.println(MAPPER.writeValueAsString(user));
}
// 输出结果：{"name":"张三"}
```
<a name="IbAt3"></a>
### `@JsonIncludeProperties`
`@JsonIncludeProperties` 注解与 `@JsonIgnoreProperties` 注解正好相反，它只会在序列化、反序列化包含在注解中的属性，其它属性都不参与序列化和反序列化。
```java
@Getter
@Setter
@JsonIncludeProperties({"name", "age"})
public class User {

	private String name;
	private String age;
	private String sex;
}
```
对其进行序列化：
```java
public static void main(String[] args) throws JsonProcessingException {
	User user = new User();
	user.setName("张三");
	user.setAge("25");
	user.setSex("男");
	System.out.println(MAPPER.writeValueAsString(user));
}
// 输出结果：{"name":"张三","age":"25"}
```
<a name="hLE8u"></a>
### `@JsonProperty`
`@JsonProperty` 也是常用注解，它包含了很多其他注解的功能，因此最好不要与功能重复的注解同时使用，以免产生干扰。该注解通常标记在属性或属性的 Getter、Setter 方法上，功能如下：<br />可以在反序列化时指定属性的名称，类似 `@JsonAlias` 的效果。
```java

@Getter
@Setter
public class User {

	@JsonProperty("nm")
	private String name;

	private String age;

}
// 反序列化时可以将nm对应的值赋值到name字段上
```
通过 `Access` 枚举可以控制哪些属性可以进行序列化、反序列化，类似 `@JsonIgnore` 的效果。
```java

@Getter
@Setter
public class User {

	@JsonProperty(access = JsonProperty.Access.READ_ONLY)
	private String name;


	private String age;

}
// 因为name属性被设置为只读，所以反序列化时会忽略name属性
```
<a name="ZV4gf"></a>
### `@JsonRawValue`
`@JsonRawValue` 注解会将字符串形式的 JSON 也尝试序列化为对象，示例如下：
```java
@Getter
@Setter
public class User {

	private String name;

	@JsonRawValue
	private String json;

}
```
对其进行序列化：
```java

public static void main(String[] args) throws JsonProcessingException {
	User user = new User();
	user.setName("张三");
	user.setJson("{\"age\":\"25\"}");
	System.out.println(MAPPER.writeValueAsString(user));
}
// 输出结果：
{
	"name": "张三",
	"json": {
		"age": "25"
		}
}
// 如果没有该注解修饰则输出：
{
	"name": "张三",
	"json": "{\"age\":\"25\"}"
}
```
<a name="BPvNw"></a>
### `@JsonUnwrapped`
`@JsonUnwrapped` 注解可以在序列化时把一个对象中嵌套对象的属性平铺展开，放到同一层级：
```java
@Getter
@Setter
public static class User {

	private String name;

	@JsonUnwrapped
	private Identity identity;

}

@Getter
@Setter
public static class Identity {

	private String age;

	private String address;

}
```
对其进行序列化：
```java

public static void main(String[] args) throws JsonProcessingException {

	User user = new User();

	user.setName("张三");

	Identity identity = new Identity();

	identity.setAge("25");

	identity.setAddress("china");

	user.setIdentity(identity);

	System.out.println(MAPPER.writeValueAsString(user));

}
// 输出结果：{"name":"张三","age":"25","address":"china"}
```
<a name="nPWWd"></a>
### `@JsonFilter`
`@JsonFilter` 注解用于指定在序列化期间要使用的过滤器：
```java

@Getter
@Setter
@JsonFilter("customFilter")
public static class User {

	private String name;
	private String age;
	private String cardCode;
	private String address;
}
```
对 User 对象进行序列化：
```java
public static void main(String[] args) throws JsonProcessingException {
    SimpleBeanPropertyFilter propertyFilter = SimpleBeanPropertyFilter.filterOutAllExcept("cardCode", "address");
    FilterProvider provider = new SimpleFilterProvider().addFilter("customFilter", propertyFilter);
    User user = new User();
    user.setName("张三");
    user.setAge("25");
    user.setAddress("china");
    user.setCardCode("123456");
    System.out.println(MAPPER.writer(provider).writeValueAsString(user));
}
// 输出结果：{"cardCode":"123456","address":"china"}
```
<a name="nQTdT"></a>
### `@JsonAppend`
`@JsonAppend` 注解可以在序列化时额外添加指定的属性：
```java
@Getter
@Setter
@JsonAppend(attrs = {
    @JsonAppend.Attr(value = "version")
    })
public class User {

     private String name;
     private String age;
}
```
可以在序列化时手动增加 version=1.0 的属性值：
```java
public static void main(String[] args) throws JsonProcessingException {
    User user = new User();
    user.setName("张三");
    user.setAge("25");
    String str = MAPPER.writerFor(User.class).withAttribute("version", "1.0").writeValueAsString(user);
    System.out.println(str);
}
// 输出结果：{"name":"张三","age":"25","version":"1.0"}
```
