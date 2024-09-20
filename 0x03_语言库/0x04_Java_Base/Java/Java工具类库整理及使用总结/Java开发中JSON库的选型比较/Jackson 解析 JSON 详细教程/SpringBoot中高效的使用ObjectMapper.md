<a name="pAETv"></a>
## 每次new一个
在SpringBoot项目中要实现对象与Json字符串的互转，每次都需要像如下一样new 一个ObjectMapper对象：
```java
public UserEntity string2Obj(String json) throws JsonProcessingException {
    ObjectMapper objectMapper = new ObjectMapper();
    return objectMapper.readValue(json, UserEntity.class);
}

public String obj2String(UserEntity userEntity) throws JsonProcessingException {
    ObjectMapper objectMapper = new ObjectMapper();
    return objectMapper.writeValueAsString(car)
}
```
**这样的代码到处可见，有问题吗？**<br />要说他有问题吧，确实能正常执行；可你要说没问题吧，在追求性能的同学眼里，这属实算是十恶不赦的代码了。<br />首先，用JMH对这段代码做一个基准测试，让大家对其性能有个大概的了解。<br />基准测试是指通过设计科学的测试方法、测试工具和测试系统，实现对一类测试对象的某项性能指标进行定量的和可对比的测试。而JMH是一个用来构建，运行，分析Java或其他运行在JVM之上的语言的 纳秒/微秒/毫秒/宏观 级别基准测试的工具。
```java
@BenchmarkMode(Mode.Throughput)
@OutputTimeUnit(TimeUnit.SECONDS)
@State(Scope.Thread)
@Fork(1)
@Warmup(iterations = 5, time = 1)
@Measurement(iterations = 3, time = 1)
public class JsonJMHTest {

    String json = "{\"id\":122345667,\"email\":\"jianzh5@163.com\",\"price\":12.25}";
    UserEntity userEntity = new UserEntity(13345L,"jianzh5@163.com", BigDecimal.valueOf(12.25));
    /**
     * 测试String to Object
     */
    @Benchmark
    public UserEntity objectMapper2ObjTest() throws JsonProcessingException {
        ObjectMapper objectMapper = new ObjectMapper();
        return objectMapper.readValue(json, UserEntity.class);
    }

    /**
     * 测试Object to String
     */
    @Benchmark
    public String objectMapper2StringTest() throws JsonProcessingException {
        ObjectMapper objectMapper = new ObjectMapper();
        return objectMapper.writeValueAsString(userEntity);
    }

    public static void main(String[] args) throws RunnerException {
        Options opt = new OptionsBuilder()
        .include(JsonJMHTest.class.getSimpleName())
        .build();
        new Runner(opt).run();
    }
}
```
**测试环境**
```
# JMH version: 1.36
# VM version: JDK 17.0.3, OpenJDK 64-Bit Server VM, 17.0.3+7-LTS
# Mac AppleM1/16GB
```
**测试结果**
```
Benchmark                             Mode  Cnt       Score        Error  Units
JsonJMHTest.objectMapper2ObjTest     thrpt    3  234237.984 ±   3329.192  ops/s
JsonJMHTest.objectMapper2StringTest  thrpt    3   27211.693 ±   2693.997  ops/s
```
通过测试结果可以看出，每次new一个ObjectMapper，在实现字符串转对象时每秒可以完成23万多次，而实现对象转Json字符串每秒仅可完成2.7万次。<br />那该如何优化，提升性能呢？
<a name="Hk1wU"></a>
## 单例化
在创建工具类时要将工具类设置成单例的，这样不仅可以保证线程安全，也可以保证在系统全局只能创建一个对象，避免频繁创建对象的成本。<br />所以，可以在项目中构建一个ObjectMapper的单例类。
```java
@Getter
public enum ObjectMapperInstance {

    INSTANCE;

    private final ObjectMapper objectMapper = new ObjectMapper();

    ObjectMapperInstance() {

    }
}
```
再次使用JMH对其测试：
```java
@Benchmark
public UserEntity singleten2ObjTest() throws JsonProcessingException {
    ObjectMapper objectMapper = ObjectMapperInstance.INSTANCE.getObjectMapper();
    return objectMapper.readValue(json, UserEntity.class);
}

@Benchmark
public String singleten2StringTest() throws JsonProcessingException {
    ObjectMapper objectMapper = ObjectMapperInstance.INSTANCE.getObjectMapper();
    return objectMapper.writeValueAsString(userEntity);
}
```
测试结果如下：
```
Benchmark                             Mode  Cnt        Score         Error  Units
JsonJMHTest.objectMapper2ObjTest     thrpt    3   234237.984 ±    3329.192  ops/s
JsonJMHTest.objectMapper2StringTest  thrpt    3    27211.693 ±    2693.997  ops/s
JsonJMHTest.singleten2ObjTest        thrpt    3  4230619.941 ±  162319.616  ops/s
JsonJMHTest.singleten2StringTest     thrpt    3  8383980.615 ±   66412.168  ops/s
```
可以看到，使用单例模式，String转对象的方法每秒可以执行420多万次，比`new ObjectMapper`的方式快了18倍；而对象转String的方法每秒可以执行830万次，性能提升了300倍！！！！
<a name="d1e2v"></a>
## 个性化配置
当然，在项目中使用ObjectMapper时，有时候还需要做一些个性化配置，比如将Long和BigDemical类型的属性都通过字符串格式进行转换，防止前端使用时丢失数值精度。<br />这些类型转换的格式映射都可以在单例类中配置，代码如下：
```java
@Getter
public enum ObjectMapperInstance {

    INSTANCE;

    private final ObjectMapper objectMapper;

    ObjectMapperInstance() {
        objectMapper = new ObjectMapper();
        // 注册自定义模块
        initialize();
    }

    private void initialize() {
        CustomJsonModule customJsonModule = new CustomJsonModule();
        objectMapper.registerModule(customJsonModule);
    }
}
```
在`initialize()`方法中给`ObjectMapper`注册自定义序列化转换器。
```
{"id":"122345667","email":"jianzh5@163.com","price":"12.25"}
{"id":122345667,"email":"jianzh5@163.com","price":12.25}
```
第一行是使用注册自定义序列换转换器后的效果，给id和price字段都加上了引号。<br />再来一次JMH测试：
```
Benchmark                          Mode  Cnt        Score         Error  Units
JsonJMHTest.singleten2ObjTest     thrpt    3  4240033.973 ±  259868.179  ops/s
JsonJMHTest.singleten2StringTest  thrpt    3  7438855.337 ±   55298.179  ops/s
```
可以看到，给ObjectMapper额外注册转换类型以后性能会受到一定的影响，但对业务影响不大。（啥业务能这么高的请求~）
<a name="XGH0S"></a>
## 小结
通过上面的测试，结论已经很清晰了。使用单例模式进行字符串转对象时性能可以提升18倍，而对象转String性能快了惊人的290万倍。
