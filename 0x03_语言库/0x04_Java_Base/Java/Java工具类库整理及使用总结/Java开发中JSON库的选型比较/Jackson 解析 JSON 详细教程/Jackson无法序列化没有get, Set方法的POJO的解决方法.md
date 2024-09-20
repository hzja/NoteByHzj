Java
<a name="ZQmzz"></a>
## 异常
重现代码
```java
@Test
public void testObjectMapper() throws JsonProcessingException {
    ObjectMapper objectMapper = new ObjectMapper();
    Man man = new Man();
    System.out.println(objectMapper.writeValueAsString(man));
}

public static class Man {
    private int age;
    private String name;
}
```
当Jackson遇到没有getter，setter方法的POJO时，会抛出这个异常：
> com.fasterxml.jackson.databind.JsonMappingException: No serializer found for class JUnitTest$Man and no properties discovered to create BeanSerializer (to avoid exception, disable SerializationFeature.FAIL_ON_EMPTY_BEANS)

通过异常的说明，可以知道原因是：对于类Man没有找到序列化器，并且没有发现有相应的属性来创建BeanSerializer序列化器。
<a name="CXtza"></a>
## 解决
异常中，也给出了一个解决方案：禁用 `SerializationFeature.FAIL_ON_EMPTY_BEANS`：
```java
objectMapper.configure(SerializationFeature.FAIL_ON_EMPTY_BEANS, false);
```
通过源码，可看到，它默认是true的：
```java
SerializationFeature.FAIL_ON_EMPTY_BEANS(true)
```
如果这样子配置了的话，则像上面这种Man的POJO，它序列化出来的结果是`{}`开启反射时访问所有访问修饰符的字段：
```java
objectMapper.setVisibility(PropertyAccessor.FIELD, JsonAutoDetect.Visibility.ANY);
```
像这样子的话，就可以直接访问那些private字段，而不需要getter，setter了。(不过，没测试过这种情况下的json序列化性能，如果对性能比较敏感的话，请自行进行基准测试)异常
> com.fasterxml.jackson.databind.JsonMappingException: (was java.lang.NullPointerException) (through reference chain: )

有时，在pojo里添加了一些自定义的方法，但实际上它们并不是属于普通bean的get/set的，这样子的话，它就会报类似上面的异常了。这时可以在相应的方法上，加上`@JsonIgnore`注解来忽略序列化它。免得让Jackson误认为它是getter/setter的方法。
