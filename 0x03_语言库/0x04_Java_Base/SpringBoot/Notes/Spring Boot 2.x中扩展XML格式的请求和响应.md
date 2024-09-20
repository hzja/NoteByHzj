Java SpringBoot<br />对于XML格式的请求要如何快速的在Controller中包装成对象，以及如何以XML的格式返回一个对象呢？
<a name="s0ASd"></a>
## 实现原理：消息转换器（Message Converter）
在扩展上述问题之前，先要知道Spring Boot中处理HTTP请求的实现是采用的Spring MVC。而在Spring MVC中有一个消息转换器这个概念，它主要负责处理各种不同格式的请求数据进行处理，并包转换成对象，以提供更好的编程体验。<br />在Spring MVC中定义了`HttpMessageConverter`接口，抽象了消息转换器对类型的判断、对读写的判断与操作，具体可见如下定义：
```java
public interface HttpMessageConverter<T> {

    boolean canRead(Class<?> clazz, @Nullable MediaType mediaType);

    boolean canWrite(Class<?> clazz, @Nullable MediaType mediaType);

    List<MediaType> getSupportedMediaTypes();

    T read(Class<? extends T> clazz, HttpInputMessage inputMessage) throws IOException, HttpMessageNotReadableException;

    void write(T t, @Nullable MediaType contentType, HttpOutputMessage outputMessage) throws IOException, HttpMessageNotWritableException;

}
```
众所周知，HTTP请求的`Content-Type`有各种不同格式定义，如果要支持Xml格式的消息转换，就必须要使用对应的转换器。Spring MVC中默认已经有一套采用Jackson实现的转换器`MappingJackson2XmlHttpMessageConverter`。
<a name="ZuOs0"></a>
## 扩展实现
<a name="dnfxp"></a>
### 第一步：引入Xml消息转换器
在传统Spring应用中，可以通过如下配置加入对Xml格式数据的消息转换实现：
```java
@Configuration
public class MessageConverterConfig1 extends WebMvcConfigurerAdapter {
    @Override
    public void configureMessageConverters(List<HttpMessageConverter<?>> converters) {
        Jackson2ObjectMapperBuilder builder = Jackson2ObjectMapperBuilder.xml();
        builder.indentOutput(true);
        converters.add(new MappingJackson2XmlHttpMessageConverter(builder.build()));
    }
}
```
在Spring Boot应用不用像上面这么麻烦，只需要加入jackson-dataformat-xml依赖，Spring Boot就会自动引入`MappingJackson2XmlHttpMessageConverter`的实现：
```xml
<dependency>
    <groupId>com.fasterxml.jackson.dataformat</groupId>
    <artifactId>jackson-dataformat-xml</artifactId>
</dependency>
```
同时，为了配置Xml数据与维护对象属性的关系所要使用的注解也在上述依赖中，所以这个依赖也是必须的。
<a name="GSRqj"></a>
### 第二步：定义对象与Xml的关系
做好了基础扩展之后，下面就可以定义Xml内容对应的Java对象了，比如：
```java
@Data
@NoArgsConstructor
@AllArgsConstructor
@JacksonXmlRootElement(localName = "User")
public class User {

    @JacksonXmlProperty(localName = "name")
    private String name;
    @JacksonXmlProperty(localName = "age")
    private Integer age;

}
```
其中：`@Data`、`@NoArgsConstructor`、`@AllArgsConstructor`是lombok简化代码的注解，主要用于生成get、set以及构造函数。`@JacksonXmlRootElement`、`@JacksonXmlProperty`注解是用来维护对象属性在xml中的对应关系。<br />上述配置的User对象，其可以映射的Xml样例如下（后续可以使用上述xml来请求接口）：
```xml
<User>
 <name>aaaa</name>
 <age>10</age>
</User>
```
<a name="T8srD"></a>
### 第三步：创建接收xml请求的接口
完成了要转换的对象之后，可以编写一个接口来接收xml并返回xml，比如：
```java
@Controller
public class UserController {

    @PostMapping(value = "/user", 
        consumes = MediaType.APPLICATION_XML_VALUE, 
        produces = MediaType.APPLICATION_XML_VALUE)
    @ResponseBody
    public User create(@RequestBody User user) {
        user.setName("didispace.com : " + user.getName());
        user.setAge(user.getAge() + 100);
        return user;
    }

}
```
最后，启动Spring Boot应用，通过POSTMAN等请求工具，尝试一下这个接口，可以看到请求Xml，并且返回了经过处理后的Xml内容。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623907924199-f8eb63cb-5d7a-4b8e-abed-8ec965d7e011.png#clientId=u5dc8fe02-abc8-4&from=paste&id=u38843a00&originHeight=847&originWidth=1080&originalType=url&ratio=3&status=done&style=shadow&taskId=u8c4bea74-56f2-446e-8eae-4de79bf2a79)
