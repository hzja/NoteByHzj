JavaJackson<br />JSON 对于开发者并不陌生，如今的 WEB 服务、移动应用、甚至物联网大多都是以 **JSON** 作为数据交换的格式。学习 JSON 格式的操作工具对开发者来说是必不可少的。这篇文章将介绍如何使用 **Jackson** 开源工具库对 JSON 进行常见操作。<br />![JSON logo](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658555386583-4c72973f-51d1-4b39-a67c-500432b56001.jpeg#clientId=u68054256-5d09-4&from=paste&id=u97b82056&originHeight=203&originWidth=512&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u7892a1fb-42b4-4030-8502-e3c077500c3&title=JSON%20logo "JSON logo")
<a name="aLa11"></a>
## JSON 介绍
什么是 **JSON** ？JSON 是 ”**JavaScript Object Notation**“ 的缩写，JSON 是一种基于文本的格式，可以把它理解为是一个结构化的数据，这个结构化数据中可以包含**键值映射、嵌套对象以及数组**等信息。
```json
{
	"array": [
		1,
		2,
		3
	],
	"boolean": true,
	"color": "gold",
	"null": null,
	"number": 123,
	"object": {
		"a": "b",
		"c": "d"
	},
	"string": "www"
}
```
<a name="zRsoS"></a>
## Jackson 介绍
**Jackson** 和 FastJson 一样，是一个 Java 语言编写的，可以进行 JSON 处理的开源工具库，Jackson 的使用非常广泛，Spring 框架**默认**使用 Jackson 进行 JSON 处理。<br />Jackson 有三个核包，分别是 **Streaming、Databid、Annotations**，通过这些包可以方便的对 JSON 进行操作。

- [Streaming](https://github.com/FasterXML/jackson-core) 在 jackson-core 模块。定义了一些流处理相关的 API 以及特定的 JSON 实现。
- [Annotations](https://github.com/FasterXML/jackson-annotations) 在 jackson-annotations 模块，包含了 Jackson 中的注解。
- [Databind](https://github.com/FasterXML/jackson-databind) 在 jackson-databind 模块， 在 Streaming 包的基础上实现了数据绑定，依赖于 Streaming 和 Annotations 包。

得益于 Jackson 高扩展性的设计，有很多常见的文本格式以及工具都有对 Jackson 的相应适配，如 CSV、XML、YAML 等。
<a name="VE7Za"></a>
## Jackson Maven 依赖
在使用 Jackson 时，大多数情况下只需要添加 jackson-databind 依赖项，就可以使用 Jackson 功能了，它依赖了下面两个包。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658555386579-90a22775-9bb2-45b7-8562-bc083e78e11d.jpeg#clientId=u68054256-5d09-4&from=paste&id=u1e8ea4c9&originHeight=162&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u659e1947-4b7a-4337-961a-23673bd917a&title=)<br />jackson-databind 依赖

- com.fasterxml.jackson.core:jackson-annotations
- com.fasterxml.jackson.core:jackson-core
```xml
<dependency>
	<groupId>com.fasterxml.jackson.core</groupId>
	<artifactId>jackson-databind</artifactId>
	<version>2.13.3</version>
</dependency>
```
为了方便后续的代码演示，同时引入 Junit 进行单元测试和 Lombok 以减少 Get/Set 的代码编写。
```xml
<dependency>
	<groupId>org.junit.jupiter</groupId>
	<artifactId>junit-jupiter</artifactId>
	<version>5.8.2</version>
	<scope>test</scope>
</dependency>
<dependency>
	<groupId>org.projectlombok</groupId>
	<artifactId>lombok</artifactId>
	<version>1.18.22</version>
</dependency>
```
<a name="fX7Xm"></a>
## ObjectMapper 对象映射器
`ObjectMapper` 是 Jackson 库中最常用的一个类，使用它可以进行 Java 对象和 JSON 字符串之间快速转换。如果用过 FastJson，那么 Jackson 中的 `ObjectMapper` 就如同 FastJson 中的 JSON 类。<br />这个类中有一些常用的方法：

- `readValue()` 方法可以进行 JSON 的反序列化操作，比如可以将字符串、文件流、字节流、字节数组等将常见的内容转换成 Java 对象。
- `writeValue()` 方法可以进行 JSON 的序列化操作，可以将 Java 对象转换成 JSON 字符串。

大多数情况下，`ObjectMapper` 的工作原理是通过 Java Bean 对象的 Get/Set 方法进行转换时映射的，所以正确编写 Java 对象的 Get/Set 方法尤为重要，不过 `ObjectMapper` 也提供了诸多配置，比如可以通过配置或者注解的形式对 Java 对象和 JSON 字符串之间的转换过程进行自定义。这些在下面部分都会介绍到。
<a name="uV18q"></a>
## Jackson JSON 基本操作
Jackson 作为一个 Java 中的 JSON 工具库，处理 JSON 字符串和 Java 对象是它最基本最常用的功能，下面通过一些例子来演示其中的用法。
<a name="hA7of"></a>
### Jackson JSON 序列化
编写一个 Person 类，定义三个属性，名称、年龄以及技能。
```java
@Data
public class Person {
    private String name;
    private Integer age;
    private List<String> skillList;
}
```
将 Java 对象转换成 JSON 字符串。
```java
import java.util.Arrays;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

class PersonTest {

    ObjectMapper objectMapper = new ObjectMapper();

    @Test
    void pojoToJsonString() throws JsonProcessingException {
        Person person = new Person();
        person.setName("aLng");
        person.setAge(27);
        person.setSkillList(Arrays.asList("java", "c++"));

        String json = objectMapper.writeValueAsString(person);
        System.out.println(json);
        String expectedJson = "{\"name\":\"aLng\",\"age\":27,\"skillList\":[\"java\",\"c++\"]}";
        Assertions.assertEquals(json, expectedJson);
    }
}
```
输出的 JSON 字符串：
```java
{"name":"aLng","age":27,"skillList":["java","c++"]}
```
Jackson 甚至可以直接把序列化后的 JSON 字符串写入文件或者读取成字节数组。
```java
mapper.writeValue(new File("result.json"), myResultObject);
// 或者
byte[] jsonBytes = mapper.writeValueAsBytes(myResultObject);
// 或者
String jsonString = mapper.writeValueAsString(myResultObject);
```
<a name="Luw7a"></a>
### Jackson JSON 反序列化
直接贴出代码：
```java
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

class PersonTest {

    ObjectMapper objectMapper = new ObjectMapper();

    @Test
    void jsonStringToPojo() throws JsonProcessingException {
        String expectedJson = "{\"name\":\"aLang\",\"age\":27,\"skillList\":[\"java\",\"c++\"]}";
        Person person = objectMapper.readValue(expectedJson, Person.class);
        System.out.println(person);
        Assertions.assertEquals(person.getName(), "aLang");
        Assertions.assertEquals(person.getSkillList().toString(), "[java, c++]");
    }
}
```
输出结果：
```java
Person(name=aLang, age=27, skillList=[java, c++])
```
上面的例子演示了如何使用 Jackson 把一个 JSON 字符串反序列化成 Java 对象，其实 Jackson 对文件中的 JSON 字符串、字节形式的 JSON 字符串反序列化同样简单。<br />比如先准备了一个 JSON 内容文件 Person.json。
```json
{
	"name": "aLang",
	"age": 27,
	"skillList": [
		"java",
		"c++"
	]
}
```
下面进行读取转换。
```java
ObjectMapper objectMapper = new ObjectMapper();

@Test
void testJsonFilePojo() throws IOException {
    File file = new File("src/Person.json");
    Person person = objectMapper.readValue(file, Person.class);
      // 或者
    // person = mapper.readValue(new URL("http://some.com/api/entry.json"), MyValue.class);
    System.out.println(person);
    Assertions.assertEquals(person.getName(), "aLang");
    Assertions.assertEquals(person.getSkillList().toString(), "[java, c++]");
}
```
同样输出了 Person 内容。
```java
Person(name=aLang, age=27, skillList=[java, c++])
```
<a name="e0TkZ"></a>
## JSON 转 List
上面演示 JSON 字符串都是单个对象的，如果 JSON 是一个对象列表那么使用 Jackson 该怎么处理呢？<br />已经存在一个文件 PersonList.json.
```json
[
	{
		"name": "aLang",
		"age": 27,
		"skillList": [
			"java",
			"c++"
		]
	},
	{
		"name": "darcy",
		"age": 26,
		"skillList": [
			"go",
			"rust"
		]
	}
]
```
读取它然后转换成 `List<Person>` 。
```java
ObjectMapper objectMapper = new ObjectMapper();

@Test
void fileToPojoList() throws IOException {
    File file = new File("src/EmployeeList.json");
    List<Person> personList = objectMapper.readValue(file, new TypeReference<List<Person>>() {});
    for (Person person : personList) {
        System.out.println(person);
    }
    Assertions.assertEquals(personList.size(), 2);
    Assertions.assertEquals(personList.get(0).getName(), "aLang");
    Assertions.assertEquals(personList.get(1).getName(), "darcy");
}
```
可以输出对象内容：
```java
Person(name=aLang, age=27, skillList=[java, c++])
Person(name=darcy, age=26, skillList=[go, rust])
```
<a name="OeoTT"></a>
## JSON 转 Map
JSON 转 Map 在没有一个对象的 Java 对象时十分实用，下面演示如何使用 Jackson 把 JSON 文本转成 Map 对象。
```java
ObjectMapper objectMapper = new ObjectMapper();

@Test
void jsonStringToMap() throws IOException {
    String expectedJson = "{\"name\":\"aLang\",\"age\":27,\"skillList\":[\"java\",\"c++\"]}";
    Map<String, Object> employeeMap = objectMapper.readValue(expectedJson, new TypeReference<Map>() {});
    System.out.println(employeeMap.getClass());
    for (Entry<String, Object> entry : employeeMap.entrySet()) {
        System.out.println(entry.getKey() + ":" + entry.getValue());
    }
    Assertions.assertEquals(employeeMap.get("name"), "aLang");
}
```
可以看到 Map 的输出结果：
```java
class java.util.LinkedHashMap
name:aLang
age:27
skillList:[java, c++]
```
<a name="EOpOq"></a>
## Jackson 忽略字段
如果在进行 JSON 转 Java 对象时，JSON 中出现了 Java 类中不存在的属性，那么在转换时会遇到 `com.fasterxml.jackson.databind.exc.UnrecognizedPropertyException` 异常。<br />使用 `objectMapper.configure(DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES, false)` 可以忽略不存在的属性。
```java
ObjectMapper objectMapper = new ObjectMapper();

@Test
void jsonStringToPojoIgnoreProperties() throws IOException {
	// UnrecognizedPropertyException
	String json = "{\"yyy\":\"xxx\",\"name\":\"aLang\",\"age\":27,\"skillList\":[\"java\",\"c++\"]}";
	objectMapper.configure(DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES, false);
	Person person = objectMapper.readValue(json, Person.class);
	System.out.printf(person.toString());
	Assertions.assertEquals(person.getName(), "aLang");
	Assertions.assertEquals(person.getSkillList().toString(), "[java, c++]");
}
```
正常输出：
```java
Person(name=aLang, age=27, skillList=[java, c++])
```
<a name="ccowY"></a>
## Jackson 日期格式化
在 Java 8 之前通常使用 `java.util.Date` 类来处理时间，但是在 Java 8 发布时引入了新的时间类 `java.time.LocalDateTime`。这两者在 Jackson 中的处理略有不同。<br />先创建一个有两种时间类型属性的 Order 类。
```java
import java.time.LocalDateTime;
import java.util.Date;

import com.fasterxml.jackson.annotation.JsonFormat;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class Order {
  
    private Integer id;

    private Date createTime;

    private LocalDateTime updateTime;
}
```
<a name="i5RlL"></a>
### Date 类型
下面新建一个测试用例来测试两种时间类型的 JSON 转换。
```java
import java.time.LocalDateTime;
import java.util.Date;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

class OrderTest {

    ObjectMapper objectMapper = new ObjectMapper();

    @Test
    void testPojoToJson0() throws JsonProcessingException {
        Order order = new Order(1, new Date(), null);
        String json = objectMapper.writeValueAsString(order);
        System.out.println(json);

        order = objectMapper.readValue(json, Order.class);
        System.out.println(order.toString());

        Assertions.assertEquals(order.getId(), 1);
    }

}
```
在这个测试代码中，只初始化了 Date 类型的时间，下面是输出的结果：
```java
{"id":1,"createTime":1658320852395,"updateTime":null}
Order(id=1, createTime=Wed Jul 20 20:40:52 CST 2022, updateTime=null)
```
可以看到正常的进行了 JSON 的序列化与反序列化，但是 JSON 中的时间是一个时间戳格式，可能不是期望的。
<a name="PldvK"></a>
### `LocalDateTime` 类型
为什么没有设置 `LocalDateTime` 类型的时间呢？因为默认情况下进行 LocalDateTime 类的 JSON 转换会遇到报错。
```java
class OrderTest {

    ObjectMapper objectMapper = new ObjectMapper();

    @Test
    void testPojoToJson() throws JsonProcessingException {
        Order order = new Order(1, new Date(), LocalDateTime.now());
        String json = objectMapper.writeValueAsString(order);
        System.out.println(json);

        order = objectMapper.readValue(json, Order.class);
        System.out.println(order.toString());

        Assertions.assertEquals(order.getId(), 1);
    }
}
```
运行后会遇到报错：
```java
com.fasterxml.jackson.databind.exc.InvalidDefinitionException: 
            Java 8 date/time type `java.time.LocalDateTime` not supported by default: 
                add Module "com.fasterxml.jackson.datatype:jackson-datatype-jsr310" 
          to enable handling (through reference chain: com.wdbyte.jackson.Order["updateTime"])
```
这里需要添加相应的数据绑定支持包。<br />添加依赖：
```xml
<dependency>
	<groupId>com.fasterxml.jackson.datatype</groupId>
	<artifactId>jackson-datatype-jsr310</artifactId>
	<version>2.13.3</version>
</dependency>
```
然后在定义 `ObjectMapper` 时通过 `findAndRegisterModules()` 方法来注册依赖。
```java
import java.time.LocalDateTime;
import java.util.Date;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

class OrderTest {

    ObjectMapper objectMapper = new ObjectMapper().findAndRegisterModules();

    @Test
    void testPojoToJson() throws JsonProcessingException {
        Order order = new Order(1, new Date(), LocalDateTime.now());
        String json = objectMapper.writeValueAsString(order);
        System.out.println(json);

        order = objectMapper.readValue(json, Order.class);
        System.out.println(order.toString());

        Assertions.assertEquals(order.getId(), 1);
    }
}
```
运行可以得到正常序列化与反序列化日志，不过序列化后的时间格式依旧奇怪。
```java
{"id":1,"createTime":1658321191562,"updateTime":[2022,7,20,20,46,31,567000000]}
Order(id=1, createTime=Wed Jul 20 20:46:31 CST 2022, updateTime=2022-07-20T20:46:31.567)
```
<a name="DPeIK"></a>
### 时间格式化
通过在字段上使用注解 `@JsonFormat` 来自定义时间格式。
```java
import java.time.LocalDateTime;
import java.util.Date;

import com.fasterxml.jackson.annotation.JsonFormat;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class Order {

    private Integer id;

    @JsonFormat(pattern = "yyyy-MM-dd HH:mm:ss", timezone = "Asia/Shanghai")
    private Date createTime;

    @JsonFormat(pattern = "yyyy-MM-dd HH:mm:ss", timezone = "Asia/Shanghai")
    private LocalDateTime updateTime;
}
```
再次运行上面的列子可以得到时间格式化后的 JSON 字符串。
```java
{"id":1,"createTime":"2022-07-20 20:49:46","updateTime":"2022-07-20 20:49:46"}
Order(id=1, createTime=Wed Jul 20 20:49:46 CST 2022, updateTime=2022-07-20T20:49:46)
```
<a name="SVyP7"></a>
## Jackson 常用注解
<a name="khOZl"></a>
### `@JsonIgnore`
使用 `@JsonIgnore` 可以忽略某个 Java 对象中的属性，它将不参与 JSON 的序列化与反序列化。<br />示例：
```java
import com.fasterxml.jackson.annotation.JsonIgnore;
import lombok.Data;

@Data
public class Cat {

    private String name;

    @JsonIgnore
    private Integer age;
}
```
编写单元测试类。
```java
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

class CatTest {

    ObjectMapper objectMapper = new ObjectMapper();

    @Test
    void testPojoToJson() throws JsonProcessingException {
        Cat cat = new Cat();
        cat.setName("Tom");
        cat.setAge(2);
        String json = objectMapper.writeValueAsString(cat);
        System.out.println(json);

        Assertions.assertEquals(json, "{\"name\":\"Tom\"}");

        cat = objectMapper.readValue(json, Cat.class);
        Assertions.assertEquals(cat.getName(), "Tom");
        Assertions.assertEquals(cat.getAge(), null);
    }

}
```
输出结果中 age 属性为 null。
```java
{"name":"Tom"}
```
<a name="lodWd"></a>
### `@JsonGetter`
使用 `@JsonGetter` 可以在对 Java 对象进行 JSON 序列化时自定义属性名称。
```java
import com.fasterxml.jackson.annotation.JsonGetter;
import com.fasterxml.jackson.annotation.JsonIgnore;
import lombok.Data;

@Data
public class Cat {

    private String name;

    private Integer age;

    @JsonGetter(value = "catName")
    public String getName() {
        return name;
    }
}
```
编写单元测试类进行测试。
```java
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

class CatTest {

    ObjectMapper objectMapper = new ObjectMapper();

    @Test
    void testPojoToJson2() throws JsonProcessingException {
        Cat cat = new Cat();
        cat.setName("Tom");
        cat.setAge(2);
        String json = objectMapper.writeValueAsString(cat);
        System.out.println(json);
        Assertions.assertEquals(json, "{\"age\":2,\"catName\":\"Tom\"}");
    }
}
```
输出结果，name 已经设置成了 catName：
```java
{"age":2,"catName":"Tom"}
```
<a name="GYJGG"></a>
### `@JsonSetter`
使用 `@JsonSetter` 可以在对 JSON 进行反序列化时设置 JSON 中的 key 与 Java 属性的映射关系。
```java
package com.wdbyte.jackson;

import com.fasterxml.jackson.annotation.JsonGetter;
import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonSetter;
import lombok.Data;

@Data
public class Cat {
    @JsonSetter(value = "catName")
    private String name;

    private Integer age;

    @JsonGetter(value = "catName")
    public String getName() {
        return name;
    }
}
```
编写单元测试类进行测试。
```java
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

class CatTest {

    ObjectMapper objectMapper = new ObjectMapper();

    @Test
    void testPojoToJson2() throws JsonProcessingException {
        String json = "{\"age\":2,\"catName\":\"Tom\"}";
        Cat cat = objectMapper.readValue(json, Cat.class);
        System.out.println(cat.toString());
        Assertions.assertEquals(cat.getName(), "Tom");
    }
}
```
输出结果：
```java
Cat(name=Tom, age=2)
```
<a name="R4af3"></a>
### `@JsonAnySetter`
使用 `@JsonAnySetter` 可以在对 JSON 进行反序列化时，对所有在 Java 对象中不存在的属性进行逻辑处理，下面的代码演示把不存在的属性存放到一个 Map 集合中。
```java
import java.util.Map;

import com.fasterxml.jackson.annotation.JsonAnyGetter;
import com.fasterxml.jackson.annotation.JsonAnySetter;
import com.google.common.collect.Maps;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class Student {
    private String name;
    private Integer age;
    private Map<String, Object> diyMap = new HashMap<>();

    @JsonAnySetter
    public void otherField(String key, String value) {
        this.diyMap.put(key, value);
    }
}
```
编写单元测试用例。
```java
import java.util.HashMap;
import java.util.Map;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

class StudentTest {

    private ObjectMapper objectMapper = new ObjectMapper();

    @Test
    void testJsonToPojo() throws JsonProcessingException {
        Map<String, Object> map = new HashMap<>();
        map.put("name", "aLang");
        map.put("age", 18);
        map.put("skill", "java");

        String json = objectMapper.writeValueAsString(map);
        System.out.println(json);

        Student student = objectMapper.readValue(json, Student.class);
        System.out.println(student);

        Assertions.assertEquals(student.getDiyMap().get("skill"), "java");
    }

}
```
输出结果中可以看到 JSON 中的 skill 属性因为不在 Java 类 Student 中，所以被放到了 `diyMap` 集合。
```java
{"skill":"java","name":"aLang","age":18}
Student(name=aLang, age=18, diyMap={skill=java})
```
<a name="WcH9l"></a>
### `@JsonAnyGetter`
使用 `@JsonAnyGetter` 可以在对 Java 对象进行序列化时，使其中的 Map 集合作为 JSON 中属性的来源。下面做个示例。
```java
import java.util.HashMap;
import java.util.Map;

import com.fasterxml.jackson.annotation.JsonAnyGetter;
import com.fasterxml.jackson.annotation.JsonAnySetter;
import com.google.common.collect.Maps;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;
import lombok.ToString;

@ToString
@AllArgsConstructor
@NoArgsConstructor
public class Student {
    @Getter
    @Setter
    private String name;

    @Getter
    @Setter
    private Integer age;
  
    @JsonAnyGetter
    private Map<String, Object> initMap = new HashMap() {{
        put("a", 111);
        put("b", 222);
        put("c", 333);
    }};
}
```
编写单元测试用例。
```java
class StudentTest {

    private ObjectMapper objectMapper = new ObjectMapper();

    @Test
    void testPojoToJsonTest() throws JsonProcessingException {
        Student student = new Student();
        student.setName("aLang");
        student.setAge(20);
        String json = objectMapper.writeValueAsString(student);
        System.out.println(json);
      
       Assertions.assertEquals(json,"{\"name\":\"aLang\",\"age\":20,\"a\":111,\"b\":222,\"c\":333}");
    }

}
```
输出结果：
```java
{"name":"aLang","age":20,"a":111,"b":222,"c":333}
```
<a name="BxcFS"></a>
## Jackson 总结

- Jackson 是 Java 中比较流量的 JSON 处理库之一，它是 Spring 的默认 JSON 工具。
- Jackson 主要有三个模块组成，Streaming API 、Annotations 和 Data Binding 。
- Jackson 中的 ObjectMapper 类十分强大，可以进行 JSON 相关处理，同时可以结合注释以及配置进行自定义转换逻辑。
- Jackson 扩展性很好，如 CSV、XML、YAML 格式处理都对 Jackson 有相应的适配等。
<a name="Jn6ZP"></a>
## 引用链接
Streaming: [https://github.com/FasterXML/jackson-core](https://github.com/FasterXML/jackson-core)<br />Annotations: [https://github.com/FasterXML/jackson-annotations](https://github.com/FasterXML/jackson-annotations)<br />Databind: [https://github.com/FasterXML/jackson-databind](https://github.com/FasterXML/jackson-databind)
