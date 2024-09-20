Java SpringBoot
<a name="auDf4"></a>
## 1、通过 application.yml
配置属性说明：<br />spring.jackson.date-format 指定日期格式，比如 yyyy-MM-dd HH:mm:ss，或者具体的格式化类的全限定名。<br />spring.jackson.deserialization 是否开启 Jackson 的反序列化。<br />spring.jackson.generator 是否开启 json 的 generators。<br />spring.jackson.joda-date-time-format 指定 Joda date/time 的格式，比如 (yyyy-MM-dd HH:mm:ss)。如果没有配置的话，dateformat 会作为 backup。<br />spring.jackson.locale 指定 json 使用的 Locale。<br />spring.jackson.mapper 是否开启 Jackson 通用的特性。<br />spring.jackson.parser 是否开启 jackson 的 parser 特性。<br />spring.jackson.property-naming-strategy指定 `PropertyNamingStrategy(CAMEL_CASE_TO_LOWER_CASE_WITH_UNDERSCORES)` 或者指定 PropertyNamingStrategy 子类的全限定类名。<br />spring.jackson.serialization 是否开启 jackson 的序列化。<br />spring.jackson.serialization-inclusion 指定序列化时属性的 inclusion 方式，具体查看 `JsonInclude.Include` 枚举。<br />spring.jackson.time-zone 指定日期格式化时区，比如 America/Los_Angeles 或者 GMT+10。
```yaml
spring:
  jackson:
    #日期格式化
    date-format: yyyy-MM-dd HH:mm:ss
    serialization:
      #格式化输出 
      indent_output: true
      #忽略无法转换的对象
      fail_on_empty_beans: false
    #设置空如何序列化
    defaultPropertyInclusion: NON_EMPTY
    deserialization:
      #允许对象忽略json中不存在的属性
      fail_on_unknown_properties: false
    parser:
      #允许出现特殊字符和转义符
      allow_unquoted_control_chars: true
      #允许出现单引号
      allow_single_quotes: true
```
<a name="kUwxn"></a>
## 2、使用重新注入 `ObjectMapper`
在配置 bean 中使用下面的配置
```java
@Bean
@Primary
@ConditionalOnMissingBean(ObjectMapper.class)
public ObjectMapper jacksonObjectMapper(Jackson2ObjectMapperBuilder builder)
{
    ObjectMapper objectMapper = builder.createXmlMapper(false).build();

    // 通过该方法对mapper对象进行设置，所有序列化的对象都将该规则进行序列化
    // Include.Include.ALWAYS 默认
    // Include.NON_DEFAULT 属性为默认值不序列化
    // Include.NON_EMPTY 属性为 空（""） 或者为 NULL 都不序列化，则返回的json是没有这个字段的。这样对移动端会更省流量
    // Include.NON_NULL 属性为NULL 不序列化

    objectMapper.setSerializationInclusion(JsonInclude.Include.NON_EMPTY);
    objectMapper.configure(DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES, false);
    // 允许出现特殊字符和转义符
    objectMapper.configure(JsonParser.Feature.ALLOW_UNQUOTED_CONTROL_CHARS, true);
    // 允许出现单引号
    objectMapper.configure(JsonParser.Feature.ALLOW_SINGLE_QUOTES, true);
    // 字段保留，将null值转为""
    objectMapper.getSerializerProvider().setNullValueSerializer(new JsonSerializer<Object>()
                                                                {
                                                                    @Override
                                                                    public void serialize(Object o, JsonGenerator jsonGenerator,
                                                                                          SerializerProvider serializerProvider)
                                                                        throws IOException{
                                                                        jsonGenerator.writeString("");
                                                                    }
                                                                });
    return objectMapper;
}
```
另外，如果没有进行全局配置。后台编写的接口向前端返回数据时，如果返回的是某个数据库对应的实体类，有可能出现日期类型数据不是 yyyy-MM-dd HH:mm:ss 格式。SpringBoot 提供了 `@JsonFormat` 注解就可以适用于这种情况。使用方式：加在实体类属上添加该注解
```java
/**
 * 创建时间
 */
@JsonFormat(pattern = "yyyy-MM-dd HH:mm:ss")
@TableField(value = "create_date")
private Date createDate;
```
类似功能的注解还有 `@DateTimeFormat`。 
