Java SpringBoot Retrofit<br />okhttp是一款由square公司开源的java版本http客户端工具。实际上，square公司还开源了基于okhttp进一步封装的retrofit工具，用来支持通过接口的方式发起http请求。<br />如果项目中还在直接使用RestTemplate或者okhttp，或者基于它们封装的HttpUtils，那么可以尝试使用Retrofit。<br />retrofit-spring-boot-starter实现了Retrofit与spring-boot框架快速整合，并且支持了部分功能增强，从而极大的简化spring-boot项目下http接口调用开发。接下来直接通过retrofit-spring-boot-starter，来看看spring-boot项目发送http请求有多简单。<br />retrofit官方并没有提供与spring-boot快速整合的starter。retrofit-spring-boot-starter是开源大佬封装的，已在生产环境使用，非常稳定。<br />[https://github.com/LianjiaTech/retrofit-spring-boot-starter](https://github.com/LianjiaTech/retrofit-spring-boot-starter)
<a name="EDWeh"></a>
### 引入依赖
```xml
<dependency>
    <groupId>com.github.lianjiatech</groupId>
    <artifactId>retrofit-spring-boot-starter</artifactId>
    <version>2.0.2</version>
</dependency>
```
<a name="YPsvq"></a>
### 配置`@RetrofitScan`注解
可以给带有 `@Configuration` 的类配置`@RetrofitScan`，或者直接配置到spring-boot的启动类上，如下：
```java
@SpringBootApplication
@RetrofitScan("com.github.lianjiatech.retrofit.spring.boot.test")
public class RetrofitTestApplication {

    public static void main(String[] args) {
        SpringApplication.run(RetrofitTestApplication.class, args);
    }
}
```
<a name="MlBCf"></a>
### 定义http接口
接口必须使用`@RetrofitClient`注解标记！<br />http相关注解可参考官方文档：<br />[https://square.github.io/retrofit/](https://square.github.io/retrofit/)
```java
@RetrofitClient(baseUrl = "${test.baseUrl}")
public interface HttpApi {

    @GET("person")
    Result<Person> getPerson(@Query("id") Long id);
}
```
<a name="qvLfr"></a>
### 注入使用
将接口注入到其它Service中即可使用。
```java
@Service
public class TestService {

    @Autowired
    private HttpApi httpApi;

    public void test() {
        // 通过httpApi发起http请求
    }
}
```
只要通过上述几个步骤，就能实现通过接口发送http请求了，真的很简单。如果在spring-boot项目里面使用过Mybatis，相信对这种使用方式会更加熟悉。<br />接下来继续介绍一下retrofit-spring-boot-starter更高级一点的功能。
<a name="hW9Mi"></a>
### 注解式拦截器
很多时候，希望某个接口下的某些http请求执行统一的拦截处理逻辑。这个时候可以使用注解式拦截器。使用的步骤主要分为2步：

- 继承`BasePathMatchInterceptor`编写拦截处理器；
- 接口上使用`@Intercept`进行标注。

下面以给指定请求的url后面拼接timestamp时间戳为例，介绍下如何使用注解式拦截器。
<a name="j1skK"></a>
#### 继承`BasePathMatchInterceptor`编写拦截处理器
```java
@Component
public class TimeStampInterceptor extends BasePathMatchInterceptor {

    @Override
    public Response doIntercept(Chain chain) throws IOException {
        Request request = chain.request();
        HttpUrl url = request.url();
        long timestamp = System.currentTimeMillis();
        HttpUrl newUrl = url.newBuilder()
                .addQueryParameter("timestamp", String.valueOf(timestamp))
                .build();
        Request newRequest = request.newBuilder()
                .url(newUrl)
                .build();
        return chain.proceed(newRequest);
    }
}
```
<a name="n1suy"></a>
#### 接口上使用`@Intercept`进行标注
```java
@RetrofitClient(baseUrl = "${test.baseUrl}")
@Intercept(handler = TimeStampInterceptor.class, include = {"/api/**"}, exclude = "/api/test/savePerson")
public interface HttpApi {

    @GET("person")
    Result<Person> getPerson(@Query("id") Long id);

    @POST("savePerson")
    Result<Person> savePerson(@Body Person person);
}
```
上面的`@Intercept`配置表示：拦截HttpApi接口下`/api/**`路径下（排除/api/test/savePerson）的请求，拦截处理器使用`TimeStampInterceptor`。
<a name="FW3Lg"></a>
### 扩展注解式拦截器
有的时候，需要在拦截注解动态传入一些参数，然后再执行拦截的时候需要使用这个参数。这种时候，可以扩展实现自定义拦截注解。<br />自定义拦截注解必须使用`@InterceptMark`标记，并且注解中必须包括`include()`、`exclude()`、`handler()`属性信息。使用的步骤主要分为3步：

- 自定义拦截注解
- 继承`BasePathMatchInterceptor`编写拦截处理器
- 接口上使用自定义拦截注解；

例如需要在请求头里面动态加入`accessKeyId`、`accessKeySecret`签名信息才能正常发起http请求，这个时候可以自定义一个加签拦截器注解`@Sign`来实现。下面以自定义`@Sign`拦截注解为例进行说明。
<a name="WPBlt"></a>
#### 自定义`@Sign`注解
```java
@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.TYPE)
@Documented
@InterceptMark
public @interface Sign {
    /**
     * 密钥key
     * 支持占位符形式配置。
     *
     * @return
     */
    String accessKeyId();

    /**
     * 密钥
     * 支持占位符形式配置。
     *
     * @return
     */
    String accessKeySecret();

    /**
     * 拦截器匹配路径
     *
     * @return
     */
    String[] include() default {"/**"};

    /**
     * 拦截器排除匹配，排除指定路径拦截
     *
     * @return
     */
    String[] exclude() default {};

    /**
     * 处理该注解的拦截器类
     * 优先从spring容器获取对应的Bean，如果获取不到，则使用反射创建一个！
     *
     * @return
     */
    Class<? extends BasePathMatchInterceptor> handler() default SignInterceptor.class;
}
```
扩展自定义拦截注解有以下2点需要注意：

- 自定义拦截注解必须使用`@InterceptMark`标记。
- 注解中必须包括`include()`、`exclude()`、`handler()`属性信息。
<a name="xF05s"></a>
#### 实现`SignInterceptor`
```java
@Component
public class SignInterceptor extends BasePathMatchInterceptor {

    private String accessKeyId;

    private String accessKeySecret;

    public void setAccessKeyId(String accessKeyId) {
        this.accessKeyId = accessKeyId;
    }

    public void setAccessKeySecret(String accessKeySecret) {
        this.accessKeySecret = accessKeySecret;
    }

    @Override
    public Response doIntercept(Chain chain) throws IOException {
        Request request = chain.request();
        Request newReq = request.newBuilder()
                .addHeader("accessKeyId", accessKeyId)
                .addHeader("accessKeySecret", accessKeySecret)
                .build();
        return chain.proceed(newReq);
    }
}
```
上述`accessKeyId`和`accessKeySecret`字段值会依据`@Sign`注解的`accessKeyId()`和`accessKeySecret()`值自动注入，如果`@Sign`指定的是占位符形式的字符串，则会取配置属性值进行注入。<br />另外，`accessKeyId`和`accessKeySecret`字段必须提供`setter`方法。
<a name="v0cJq"></a>
#### 接口上使用`@Sign`
```java
@RetrofitClient(baseUrl = "${test.baseUrl}")
@Sign(accessKeyId = "${test.accessKeyId}", accessKeySecret = "${test.accessKeySecret}", exclude = {"/api/test/person"})
public interface HttpApi {

    @GET("person")
    Result<Person> getPerson(@Query("id") Long id);

    @POST("savePerson")
    Result<Person> savePerson(@Body Person person);
}
```
这样就能在指定url的请求上，自动加上签名信息了。
<a name="er47L"></a>
### 连接池管理
默认情况下，所有通过Retrofit发送的http请求都会使用`max-idle-connections=5 keep-alive-second=300`的默认连接池。<br />当然，也可以在配置文件中配置多个自定义的连接池，然后通过`@RetrofitClient`的`poolName`属性来指定使用。比如要让某个接口下的请求全部使用`poolName=test1`的连接池，代码实现如下：
<a name="Nh7mi"></a>
#### 1、配置连接池
```yaml
retrofit:
    # 连接池配置
    pool:
        test1:
        max-idle-connections: 3
        keep-alive-second: 100
        test2:
        max-idle-connections: 5
        keep-alive-second: 50
```
<a name="egKrL"></a>
#### 2、通过`@RetrofitClient`的poolName属性来指定使用的连接池
```java
@RetrofitClient(baseUrl = "${test.baseUrl}", poolName="test1")
public interface HttpApi {

    @GET("person")
    Result<Person> getPerson(@Query("id") Long id);
}
```
<a name="BeJP5"></a>
### 日志打印
很多情况下，希望将http请求日志记录下来。通过`@RetrofitClient`的`logLevel`和`logStrategy`属性，可以指定每个接口的日志打印级别以及日志打印策略。<br />retrofit-spring-boot-starter支持了5种日志打印级别(`ERROR`, `WARN`, `INFO`, `DEBUG`, `TRACE`)，默认INFO；支持了4种日志打印策略（`NONE`, `BASIC`, `HEADERS`, `BODY`），默认`BASIC`。<br />4种日志打印策略含义如下：

- NONE：No logs.
- BASIC：Logs request and response lines.
- HEADERS：Logs request and response lines and their respective headers.
- BODY：Logs request and response lines and their respective headers and bodies (if present).

retrofit-spring-boot-starter默认使用了`DefaultLoggingInterceptor`执行真正的日志打印功能，其底层就是okhttp原生的`HttpLoggingInterceptor`。<br />当然，也可以自定义实现自己的日志打印拦截器，只需要继承`BaseLoggingInterceptor`（具体可以参考`DefaultLoggingInterceptor`的实现），然后在配置文件中进行相关配置即可。
```yaml
retrofit:
  # 日志打印拦截器
  logging-interceptor: com.github.lianjiatech.retrofit.spring.boot.interceptor.DefaultLoggingInterceptor
```
<a name="YS5hC"></a>
### Http异常信息格式化器
当出现http请求异常时，原始的异常信息可能阅读起来并不友好，因此retrofit-spring-boot-starter提供了Http异常信息格式化器，用来美化输出http请求参数，默认使用`DefaultHttpExceptionMessageFormatter`进行请求数据格式化。<br />也可以进行自定义，只需要继承`BaseHttpExceptionMessageFormatter`，再进行相关配置即可。
```yaml
retrofit:
  # Http异常信息格式化器
  http-exception-message-formatter: com.github.lianjiatech.retrofit.spring.boot.interceptor.DefaultHttpExceptionMessageFormatter
```
<a name="f8Vvq"></a>
### 调用适配器 `CallAdapter`
Retrofit可以通过调用适配器`CallAdapterFactory`将`Call<T>`对象适配成接口方法的返回值类型。retrofit-spring-boot-starter扩展2种`CallAdapterFactory`实现：<br />`BodyCallAdapterFactory`

- 默认启用，可通过配置`retrofit.enable-body-call-adapter=false`关闭
- 同步执行http请求，将响应体内容适配成接口方法的返回值类型实例。
- 除了`Retrofit.Call<T>`、`Retrofit.Response<T>`、`java.util.concurrent.CompletableFuture<T>`之外，其它返回类型都可以使用该适配器。

`ResponseCallAdapterFactory`

- 默认启用，可通过配置`retrofit.enable-response-call-adapter=false`关闭
- 同步执行http请求，将响应体内容适配成`Retrofit.Response<T>`返回。
- 如果方法的返回值类型为`Retrofit.Response<T>`，则可以使用该适配器。

`Retrofit`自动根据方法返回值类型选用对应的`CallAdapterFactory`执行适配处理！加上`Retrofit`默认的`CallAdapterFactory`，可支持多种形式的方法返回值类型：

- `Call<T>`：不执行适配处理，直接返回`Call<T>`对象
- `CompletableFuture<T>`：将响应体内容适配成`CompletableFuture<T>`对象返回
- `Void`：不关注返回类型可以使用`Void`。如果http状态码不是2xx，直接抛错！
- `Response<T>`：将响应内容适配成`Response<T>`对象返回
- 其他任意Java类型：将响应体内容适配成一个对应的Java类型对象返回，如果http状态码不是2xx，直接抛错！
```java
/**
 * Call<T>
 * 不执行适配处理，直接返回Call<T>对象
 * @param id
 * @return
 */
@GET("person")
Call<Result<Person>> getPersonCall(@Query("id") Long id);

/**
 *  CompletableFuture<T>
 *  将响应体内容适配成CompletableFuture<T>对象返回
 * @param id
 * @return
 */
@GET("person")
CompletableFuture<Result<Person>> getPersonCompletableFuture(@Query("id") Long id);

/**
 * Void
 * 不关注返回类型可以使用Void。如果http状态码不是2xx，直接抛错！
 * @param id
 * @return
 */
@GET("person")
Void getPersonVoid(@Query("id") Long id);

/**
 *  Response<T>
 *  将响应内容适配成Response<T>对象返回
 * @param id
 * @return
 */
@GET("person")
Response<Result<Person>> getPersonResponse(@Query("id") Long id);

/**
 * 其他任意Java类型
 * 将响应体内容适配成一个对应的Java类型对象返回，如果http状态码不是2xx，直接抛错！
 * @param id
 * @return
 */
@GET("person")
Result<Person> getPerson(@Query("id") Long id);
```
也可以通过继承`CallAdapter.Factory`扩展实现自己的`CallAdapter`；然后将自定义的`CallAdapterFactory`配置成Spring的bean！<br />自定义配置的`CallAdapter.Factory`优先级更高！
<a name="y97ZY"></a>
### 数据转码器 `Converter`
Retrofi使用`Converter`将`@Body`注解标注的对象转换成请求体，将响应体数据转换成一个Java对象，可以选用以下几种`Converter`：

- Gson：com.squareup.Retrofit:converter-gson
- Jackson：com.squareup.Retrofit:converter-jackson
- Moshi：com.squareup.Retrofit:converter-moshi
- Protobuf：com.squareup.Retrofit:converter-protobuf
- Wire：com.squareup.Retrofit:converter-wire
- Simple XML：com.squareup.Retrofit:converter-simplexml

retrofit-spring-boot-starter默认使用的是jackson进行序列化转换！如果需要使用其它序列化方式，在项目中引入对应的依赖，再把对应的ConverterFactory配置成Spring的bean即可。<br />也可以通过继承`Converter.Factory`扩展实现自己的`Converter`；然后将自定义的`Converter.Factory`配置成spring的bean！<br />自定义配置的`Converter.Factory`优先级更高！
<a name="dWTBG"></a>
### 全局拦截器 BaseGlobalInterceptor
如果需要对整个系统的的http请求执行统一的拦截处理，可以自定义实现全局拦截器`BaseGlobalInterceptor`, 并配置成Spring中的bean！例如需要在整个系统发起的http请求，都带上来源信息。
```java
@Component
public class SourceInterceptor extends BaseGlobalInterceptor {
    @Override
    public Response doIntercept(Chain chain) throws IOException {
        Request request = chain.request();
        Request newReq = request.newBuilder()
                .addHeader("source", "test")
                .build();
        return chain.proceed(newReq);
    }
}
```
<a name="RxYfG"></a>
### 结语
至此，spring-boot项目下最优雅的http客户端工具介绍就结束了，更多详细信息可以参考官方文档：retrofit以及retrofit-spring-boot-starter。<br />[https://github.com/LianjiaTech/retrofit-spring-boot-starter](https://github.com/LianjiaTech/retrofit-spring-boot-starter)
