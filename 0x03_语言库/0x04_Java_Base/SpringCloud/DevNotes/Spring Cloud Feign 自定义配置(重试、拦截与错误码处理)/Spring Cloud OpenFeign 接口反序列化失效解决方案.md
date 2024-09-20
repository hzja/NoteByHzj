Java SpringCloud OpenFeign
<a name="PG7SS"></a>
### 1、关于 Spring Boot 无侵入式API接口统一格式变得具有侵入性
可是最近几年随着Spring Cloud的流行，基于`ResponseBodyAdvice`在Spring Cloud OpenFeign的继承模式下 具有了侵入性，而且侵入性还很强大，还导致resfulAPI接口反序列化出现失败的问题。<br />既然发现了问题，那就好好解决一下吧，解决方式有多种，至于选择就看大家喜欢了吧

1. `return object`，简单轻松和谐
2. `return Result<?> `，简单轻松和谐，但是client需要 调用`Result.getData()`方法
3. `feign.codec.Decoder` 不轻松，需要自己去自定义`Decoder`规则

第一方案和第二方案都是硬编码，没有多少可延展性可说的，现在就把`feign.codec.Decoder`如何解决这种侵入性的问题解决。
<a name="aPItU"></a>
### 2、Decoder解决`ResponseBodyAdvice`的侵入性
使用 Spring Cloud 2021.0.1<br />定义feignService api接口，因为篇幅问题，就忽略 Controller实现和client调用了
```java
public interface UserService {

    /** 原生 ResponseBody返回 */
    @RequestMapping(method = RequestMethod.GET, value = "/users/{id}")
    User getUser(@PathVariable("id") long id);

    /** 自定义 ResultResponseBody 统一返回 */
    @RequestMapping(method = RequestMethod.GET, value = "/Resultv1")
    @ResultResponseBody
    User getResultv1();

    /** 硬编码 Result<User> 返回同时带上ResultResponseBody统一返回 */
    @RequestMapping(method = RequestMethod.GET, value = "/Resulv2")
    @ResultResponseBody
    Result<User> getResultv2();
}
```
`Decoder`对Result进行解析
```java
public class ResultDecoder implements Decoder {
    private Decoder decoder;

    public ResultDecoder(Decoder decoder) {
        this.decoder = decoder;
    }

    @Override
    public Object decode(Response response, Type type) throws IOException, FeignException {
        Method method = response.request().requestTemplate().methodMetadata().method();
        boolean isResult = method.getReturnType() != Result.class && method.isAnnotationPresent(ResultResponseBody.class);
        if (isResult) {
            ParameterizedTypeImpl resultType = ParameterizedTypeImpl.make(Result.class, new Type[]{type}, null);
            Result<?> result = (Result<?>) this.decoder.decode(response, resultType);
            return result.getData();
        }
        return this.decoder.decode(response, type);
    }
}
```
注册`ResultDecoder`到feign中
```java
@Configuration(proxyBeanMethods = false)
public class FeignConfiguration {
	
	@Autowired
	private ObjectFactory<HttpMessageConverters> messageConverters;
	
	@Bean
	public Decoder feignDecoder(ObjectProvider<HttpMessageConverterCustomizer> customizers) {
		return new OptionalDecoder(new ResponseEntityDecoder(new ResultDecoder(new SpringDecoder(this.messageConverters, customizers))));
	}
}
```
这样子就解决了在Spring Boot中无侵入性，但是在Spring Cloud中具有侵入性的问题。
