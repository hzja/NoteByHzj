Java<br />在SpringBoot项目直接使用okhttp、httpClient或者RestTemplate发起HTTP请求，既繁琐又不方便统一管理。<br />因此，在这里推荐一个适用于SpringBoot项目的轻量级HTTP客户端框架retrofit-spring-boot-starter，使用非常简单方便，同时又提供诸多功能增强。<br />**适用于retrofit的spring-boot-starter，支持快速集成和功能增强**。

1. _Spring Boot 3.x 项目，请使用retrofit-spring-boot-starter 3.x_。
2. _Spring Boot 1.x/2.x 项目，请使用retrofit-spring-boot-starter 2.x_。

Github项目地址：[https://github.com/LianjiaTech/retrofit-spring-boot-starter](https://github.com/LianjiaTech/retrofit-spring-boot-starter)<br />Gitee项目地址：[https://gitee.com/lianjiatech/retrofit-spring-boot-starter](https://gitee.com/lianjiatech/retrofit-spring-boot-starter)
<a name="rToBQ"></a>
## 功能特性

- 自定义OkHttpClient
- 注解式拦截器
- 日志打印
- 请求重试
- 熔断降级
- 错误解码器
- 微服务之间的HTTP调用
- 全局拦截器
- 调用适配器
- 数据转换器
- 元注解
- 其他功能示例
<a name="TL2Pq"></a>
## 快速开始
<a name="G4uLh"></a>
### 引入依赖
```xml
<dependency>
  <groupId>com.github.lianjiatech</groupId>
  <artifactId>retrofit-spring-boot-starter</artifactId>
  <version>3.0.2</version>
</dependency>
```
**如果启动失败，大概率是依赖冲突，烦请引入或者排除相关依赖**。
<a name="RWGnb"></a>
### 定义HTTP接口
**接口必须使用**`**@RetrofitClient**`**注解标记**！
```java
@RetrofitClient(baseUrl = "${test.baseUrl}")
public interface HttpApi {

    @GET("person")
    Result<Person> getPerson(@Query("id") Long id);
}
```
注意：**方法请求路径慎用/开头**。对于Retrofit而言，如果baseUrl=http://localhost:8080/api/test/，方法请求路径如果是person，则该方法完整的请求路径是：http://localhost:8080/api/test/person。而方法请求路径如果是/person，则该方法完整的请求路径是：http://localhost:8080/person。
<a name="wkMSA"></a>
### 注入使用
**将接口注入到其它Service中即可使用！**
```java
@Service
public class TestService {

    @Autowired
    private HttpApi httpApi;

    public void test() {
        // 使用`httpApi`发起HTTP请求
    }
}
```
**默认情况下，自动使用SpringBoot扫描路径进行**`**RetrofitClient**`**注册**。也可以在配置类加上`@RetrofitScan`手工指定扫描路径。
<a name="M7kDO"></a>
## HTTP请求相关注解
HTTP请求相关注解，全部使用了Retrofit原生注解，以下是一个简单说明：

| 注解分类 | 支持的注解 |
| --- | --- |
| 请求方式 | `@GET` `@HEAD` `@POST` `@PUT` `@DELETE` `@OPTIONS` `@HTTP` |
| 请求头 | `@Header` `@HeaderMap` `@Headers` |
| Query参数 | `@Query` `@QueryMap` `@QueryName` |
| path参数 | `@Path` |
| form-encoded参数 | `@Field` `@FieldMap` `@FormUrlEncoded` |
| 请求体 | `@Body` |
| 文件上传 | `@Multipart` `@Part` `@PartMap` |
| url参数 | `@Url` |

<a name="TZXhd"></a>
## 配置属性
组件支持了多个可配置的属性，用来应对不同的业务场景，具体可支持的配置属性及默认值如下：<br />**注意：应用只需要配置要更改的配置项**!
```yaml
retrofit:
   # 全局转换器工厂
   global-converter-factories:
      - com.github.lianjiatech.retrofit.spring.boot.core.BasicTypeConverterFactory
      - retrofit2.converter.jackson.JacksonConverterFactory
   # 全局调用适配器工厂(组件扩展的调用适配器工厂已经内置，这里请勿重复配置)
   global-call-adapter-factories:

   # 全局日志打印配置
   global-log:
      # 启用日志打印
      enable: true
      # 全局日志打印级别
      log-level: info
      # 全局日志打印策略
      log-strategy: basic

   # 全局重试配置
   global-retry:
      # 是否启用全局重试
      enable: false
      # 全局重试间隔时间
      interval-ms: 100
      # 全局最大重试次数
      max-retries: 2
      # 全局重试规则
      retry-rules:
         - response_status_not_2xx
         - occur_io_exception

   # 全局超时时间配置
   global-timeout:
      # 全局读取超时时间
      read-timeout-ms: 10000
      # 全局写入超时时间
      write-timeout-ms: 10000
      # 全局连接超时时间
      connect-timeout-ms: 10000
      # 全局完整调用超时时间
      call-timeout-ms: 0

   # 熔断降级配置
   degrade:
      # 熔断降级类型。默认none，表示不启用熔断降级
      degrade-type: none
      # 全局sentinel降级配置
      global-sentinel-degrade:
         # 是否开启
         enable: false
         # 各降级策略对应的阈值。平均响应时间(ms)，异常比例(0-1)，异常数量(1-N)
         count: 1000
         # 熔断时长，单位为 s
         time-window: 5
         # 降级策略（0：平均响应时间；1：异常比例；2：异常数量）
         grade: 0

      # 全局resilience4j降级配置
      global-resilience4j-degrade:
         # 是否开启
         enable: false
         # 根据该名称从#{@link CircuitBreakerConfigRegistry}获取CircuitBreakerConfig，作为全局熔断配置
         circuit-breaker-config-name: defaultCircuitBreakerConfig
   # 自动设置PathMathInterceptor的scope为prototype
   auto-set-prototype-scope-for-path-math-interceptor: true
```
<a name="qa8Rp"></a>
## 高级功能
<a name="o6pPT"></a>
### 超时时间配置
如果仅仅需要修改`OkHttpClient`的超时时间，可以通过`@RetrofitClient`相关字段修改，或者全局超时配置修改。
<a name="vUouE"></a>
### 自定义`OkHttpClient`
如果需要修改`OkHttpClient`其它配置，可以通过自定义`OkHttpClient`来实现，步骤如下：

1. 实现`SourceOkHttpClientRegistrar`接口，调用`SourceOkHttpClientRegistry#register()`方法注册`OkHttpClient`。
```java
@Slf4j
@Component
public class CustomSourceOkHttpClientRegistrar implements SourceOkHttpClientRegistrar {

    @Override
    public void register(SourceOkHttpClientRegistry registry) {

        // 添加testSourceOkHttpClient
        registry.register("testSourceOkHttpClient", new OkHttpClient.Builder()
                          .connectTimeout(Duration.ofSeconds(3))
                          .writeTimeout(Duration.ofSeconds(3))
                          .readTimeout(Duration.ofSeconds(3))
                          .addInterceptor(chain -> {
                              log.info("============use testSourceOkHttpClient=============");
                              return chain.proceed(chain.request());
                          })
                          .build());
    }
}
```

2. 通过`@RetrofitClient.sourceOkHttpClient`指定当前接口要使用的`OkHttpClient`。
```java
@RetrofitClient(baseUrl = "${test.baseUrl}", sourceOkHttpClient = "testSourceOkHttpClient")
public interface CustomOkHttpTestApi {

    @GET("person")
    Result<Person> getPerson(@Query("id") Long id);
}
```
注意：组件不会直接使用指定的`OkHttpClient`，而是基于该`OkHttpClient`创建一个新的。
<a name="xxip3"></a>
### 注解式拦截器
组件提供了**注解式拦截器**，支持基于url路径匹配拦截，使用的步骤如下：

1. 继承`BasePathMatchInterceptor`
2. 使用`@Intercept`注解指定要使用的拦截器

如果需要使用多个拦截器，在接口上标注多个`@Intercept`注解即可。<br />下面以"给指定请求的url后面拼接timestamp时间戳"为例，介绍下如何使用注解式拦截器。
<a name="YgoRp"></a>
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
默认情况下，组件会自动将`BasePathMatchInterceptor`的`scope`设置为`prototype`。<br />可通过`retrofit.auto-set-prototype-scope-for-path-math-interceptor=false`关闭该功能。关闭之后，需要手动将`scope`设置为`prototype`。
```java
@Component
@Scope("prototype")
public class TimeStampInterceptor extends BasePathMatchInterceptor {

    @Override
    public Response doIntercept(Chain chain) throws IOException {
        // ...
    }
}
```
<a name="MU5rW"></a>
#### 接口上使用`@Intercept`进行标注
```java
@RetrofitClient(baseUrl = "${test.baseUrl}")
@Intercept(handler = TimeStampInterceptor.class, include = {"/api/**"}, exclude = "/api/test/savePerson")
@Intercept(handler = TimeStamp2Interceptor.class) // 需要多个，直接添加即可
public interface HttpApi {

    @GET("person")
    Result<Person> getPerson(@Query("id") Long id);

    @POST("savePerson")
    Result<Person> savePerson(@Body Person person);
}
```
上面的`@Intercept`配置表示：拦截HttpApi接口下/api/**路径下（排除/api/test/savePerson）的请求，拦截处理器使用`TimeStampInterceptor`。
<a name="T4Cd4"></a>
### 自定义拦截注解
有的时候，需要在"拦截注解"动态传入一些参数，然后在拦截的时候使用这些参数。这时候，可以使用"自定义拦截注解"，步骤如下：

1. 自定义注解。必须使用`@InterceptMark`标记，并且注解中必须包括`include`、`exclude`、`handler`字段。
2. 继承`BasePathMatchInterceptor`编写拦截处理器
3. 接口上使用自定义注解

例如，需要"在请求头里面动态加入`accessKeyId`、`accessKeySecret`签名信息才能再发起HTTP请求"，这时候可以自定义`@Sign`注解来实现。
<a name="tnfJU"></a>
#### 自定义`@Sign`注解
```java
@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.TYPE)
@Documented
@InterceptMark
public @interface Sign {

    String accessKeyId();

    String accessKeySecret();

    String[] include() default {"/**"};

    String[] exclude() default {};

    Class<? extends BasePathMatchInterceptor> handler() default SignInterceptor.class;
}
```
在`@Sign`注解中指定了使用的拦截器是`SignInterceptor`。
<a name="JhM45"></a>
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
注意：`accessKeyId`和`accessKeySecret`字段必须提供setter方法。<br />拦截器的`accessKeyId`和`accessKeySecret`字段值会依据`@Sign`注解的`accessKeyId()`和`accessKeySecret()`值自动注入，如果`@Sign`指定的是占位符形式的字符串，则会取配置属性值进行注入。
<a name="FUfHO"></a>
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
<a name="g6BLK"></a>
### 日志打印
组件支持支持全局日志打印和声明式日志打印。
<a name="fnBrW"></a>
#### 全局日志打印
默认情况下，全局日志打印是开启的，默认配置如下：
```yaml
retrofit:
   # 全局日志打印配置
   global-log:
      # 启用日志打印
      enable: true
      # 全局日志打印级别
      log-level: info
      # 全局日志打印策略
      log-strategy: basic
```
四种日志打印策略含义如下：

1. `NONE`：No logs.
2. `BASIC`：Logs request and response lines.
3. `HEADERS`：Logs request and response lines and their respective headers.
4. `BODY`：Logs request and response lines and their respective headers and bodies (if present).
<a name="shOtG"></a>
#### 声明式日志打印
如果只需要部分请求才打印日志，可以在相关接口或者方法上使用`@Logging`注解。
<a name="ZxApM"></a>
#### 日志打印自定义扩展
如果需要修改日志打印行为，可以继承`LoggingInterceptor`，并将其配置成Spring bean。
<a name="X8Aa3"></a>
#### 聚合日志打印
如果需要将同一个请求的日志聚合在一起打印，可配置`AggregateLoggingInterceptor`。
```java
@Bean
public LoggingInterceptor loggingInterceptor(RetrofitProperties retrofitProperties){
    return new AggregateLoggingInterceptor(retrofitProperties.getGlobalLog());
}
```
<a name="fJPow"></a>
### 请求重试
组件支持支持全局重试和声明式重试。
<a name="TyHPo"></a>
#### 全局重试
全局重试默认关闭，默认配置项如下：
```yaml
retrofit:
  # 全局重试配置
  global-retry:
     # 是否启用全局重试
     enable: false
     # 全局重试间隔时间
     interval-ms: 100
     # 全局最大重试次数
     max-retries: 2
     # 全局重试规则
     retry-rules:
        - response_status_not_2xx
        - occur_io_exception
```
重试规则支持三种配置：

1. `RESPONSE_STATUS_NOT_2XX`：响应状态码不是2xx时执行重试
2. `OCCUR_IO_EXCEPTION`：发生IO异常时执行重试
3. `OCCUR_EXCEPTION`：发生任意异常时执行重试
<a name="MJpvC"></a>
#### 声明式重试
如果只有一部分请求需要重试，可以在相应的接口或者方法上使用`@Retry`注解。
<a name="qJncY"></a>
#### 请求重试自定义扩展
如果需要修改请求重试行为，可以继承`RetryInterceptor`，并将其配置成Spring bean。
<a name="BhSWl"></a>
### 熔断降级
熔断降级默认关闭，当前支持sentinel和resilience4j两种实现。
```yaml
retrofit:
   # 熔断降级配置
   degrade:
      # 熔断降级类型。默认none，表示不启用熔断降级
      degrade-type: sentinel
```
<a name="WfQa9"></a>
#### Sentinel
配置`degrade-type=sentinel`开启，然后在相关接口或者方法上声明`@SentinelDegrade`注解即可。<br />记得手动引入Sentinel依赖：
```xml
<dependency>
  <groupId>com.alibaba.csp</groupId>
  <artifactId>sentinel-core</artifactId>
  <version>1.6.3</version>
</dependency>
```
此外，还支持全局Sentinel熔断降级：
```yaml
retrofit:
  # 熔断降级配置
  degrade:
    # 熔断降级类型。默认none，表示不启用熔断降级
    degrade-type: sentinel
    # 全局sentinel降级配置
    global-sentinel-degrade:
      # 是否开启
      enable: true
      # ...其他sentinel全局配置
```
<a name="WLk4d"></a>
#### Resilience4j
配置`degrade-type=resilience4j`开启。然后在相关接口或者方法上声明`@Resilience4jDegrade`即可。<br />记得手动引入Resilience4j依赖：
```xml
<dependency>
  <groupId>io.github.resilience4j</groupId>
  <artifactId>resilience4j-circuitbreaker</artifactId>
  <version>1.7.1</version>
</dependency>
```
通过以下配置可开启全局resilience4j熔断降级：
```yaml
retrofit:
   # 熔断降级配置
   degrade:
      # 熔断降级类型。默认none，表示不启用熔断降级
      degrade-type: resilience4j
      # 全局resilience4j降级配置
      global-resilience4j-degrade:
         # 是否开启
         enable: true
         # 根据该名称从#{@link CircuitBreakerConfigRegistry}获取CircuitBreakerConfig，作为全局熔断配置
         circuit-breaker-config-name: defaultCircuitBreakerConfig
```
熔断配置管理：

1. 实现`CircuitBreakerConfigRegistrar`接口，注册`CircuitBreakerConfig`。
```java
@Component
public class CustomCircuitBreakerConfigRegistrar implements CircuitBreakerConfigRegistrar {
    @Override
    public void register(CircuitBreakerConfigRegistry registry) {

        // 替换默认的CircuitBreakerConfig
        registry.register(Constants.DEFAULT_CIRCUIT_BREAKER_CONFIG, CircuitBreakerConfig.ofDefaults());

        // 注册其它的CircuitBreakerConfig
        registry.register("testCircuitBreakerConfig", CircuitBreakerConfig.custom()
                          .slidingWindowType(CircuitBreakerConfig.SlidingWindowType.TIME_BASED)
                          .failureRateThreshold(20)
                          .minimumNumberOfCalls(5)
                          .permittedNumberOfCallsInHalfOpenState(5)
                          .build());
    }
}
```

2. 通过`circuitBreakerConfigName`指定`CircuitBreakerConfig`。包括`retrofit.degrade.global-resilience4j-degrade.circuit-breaker-config-name`或者`@Resilience4jDegrade.circuitBreakerConfigName`
<a name="CrkaO"></a>
#### 扩展熔断降级
如果用户需要使用其他的熔断降级实现，继承`BaseRetrofitDegrade`，并将其配置Spring Bean。
<a name="la1kU"></a>
#### 配置fallback或者`fallbackFactory` (可选)
如果`@RetrofitClient`不设置fallback或者`fallbackFactory`，当触发熔断时，会直接抛出`RetrofitBlockException`异常。用户可以通过设置`fallback`或者`fallbackFactory`来定制熔断时的方法返回值。<br />注意：`fallback`类必须是当前接口的实现类，`fallbackFactory`必须是`FallbackFactory<T>`实现类，泛型参数类型为当前接口类型。另外，fallback和`fallbackFactory`实例必须配置成Spring Bean。<br />`fallbackFactory`相对于fallback，主要差别在于能够感知每次熔断的异常原因(cause)，参考示例如下：
```java
@Slf4j
@Service
public class HttpDegradeFallback implements HttpDegradeApi {

    @Override
    public Result<Integer> test() {
        Result<Integer> fallback = new Result<>();
        fallback.setCode(100)
        .setMsg("fallback")
        .setBody(1000000);
        return fallback;
    }
}
```
```java
@Slf4j
@Service
public class HttpDegradeFallbackFactory implements FallbackFactory<HttpDegradeApi> {

    @Override
    public HttpDegradeApi create(Throwable cause) {
        log.error("触发熔断了! ", cause.getMessage(), cause);
        return new HttpDegradeApi() {
            @Override
            public Result<Integer> test() {
                Result<Integer> fallback = new Result<>();
                fallback.setCode(100)
                .setMsg("fallback")
                .setBody(1000000);
                return fallback;
            }
        };
    }
}
```
<a name="VWWic"></a>
### 错误解码器
在HTTP发生请求错误(包括发生异常或者响应数据不符合预期)的时候，错误解码器可将HTTP相关信息解码到自定义异常中。可以在`@RetrofitClient`注解的`errorDecoder()`指定当前接口的错误解码器，自定义错误解码器需要实现`ErrorDecoder`接口：
<a name="CBWgC"></a>
### 微服务之间的HTTP调用
<a name="yEqrO"></a>
#### 继承`ServiceInstanceChooser`
用户可以自行实现`ServiceInstanceChooser`接口，完成服务实例的选取逻辑，并将其配置成Spring Bean。对于Spring Cloud应用，可以使用如下实现。
```java
@Service
public class SpringCloudServiceInstanceChooser implements ServiceInstanceChooser {

    private LoadBalancerClient loadBalancerClient;

    @Autowired
    public SpringCloudServiceInstanceChooser(LoadBalancerClient loadBalancerClient) {
        this.loadBalancerClient = loadBalancerClient;
    }

    /**
     * Chooses a ServiceInstance URI from the LoadBalancer for the specified service.
     *
     * @param serviceId The service ID to look up the LoadBalancer.
     * @return Return the uri of ServiceInstance
     */
    @Override
    public URI choose(String serviceId) {
        ServiceInstance serviceInstance = loadBalancerClient.choose(serviceId);
        Assert.notNull(serviceInstance, "can not found service instance! serviceId=" + serviceId);
        return serviceInstance.getUri();
    }
}
```
<a name="CwTSP"></a>
#### 指定serviceId和path
```java
@RetrofitClient(serviceId = "${jy-helicarrier-api.serviceId}", path = "/m/count")
public interface ApiCountService {}
```
<a name="imMQ4"></a>
## 全局拦截器
<a name="uXSJv"></a>
### 全局应用拦截器
如果需要对整个系统的的HTTP请求执行统一的拦截处理，可以实现全局拦截器`GlobalInterceptor`，并配置成spring Bean。
```java
@Component
public class SourceGlobalInterceptor implements GlobalInterceptor {

    @Autowired
    private TestService testService;

    @Override
    public Response intercept(Chain chain) throws IOException {
        Request request = chain.request();
        Request newReq = request.newBuilder()
        .addHeader("source", "test")
        .build();
        testService.test();
        return chain.proceed(newReq);
    }
}
```
<a name="OlIMZ"></a>
### 全局网络拦截器
实现`NetworkInterceptor`接口，并配置成spring Bean。
<a name="cAzfb"></a>
## 调用适配器
Retrofit可以通过`CallAdapterFactory`将`Call<T>`对象适配成接口方法的返回值类型。组件扩展了一些`CallAdapterFactory`实现：

1. `BodyCallAdapterFactory`
   - 同步执行HTTP请求，将响应体内容适配成方法的返回值类型。
   - 任意方法返回值类型都可以使用`BodyCallAdapterFactory`，优先级最低。
1. `ResponseCallAdapterFactory`
   - 同步执行HTTP请求，将响应体内容适配成`Retrofit.Response<T>`返回。
   - 只有方法返回值类型为`Retrofit.Response<T>`，才可以使用`ResponseCallAdapterFactory`。
2. 响应式编程相关`CallAdapterFactory`

**Retrofit会根据方法返回值类型选择对应的**`**CallAdapterFactory**`**执行适配处理**，目前支持的返回值类型如下：

- `String`：将Response Body适配成String返回。
- 基础类型(Long/Integer/Boolean/Float/Double)：将Response Body适配成上述基础类型
- 任意Java类型：将Response Body适配成对应的Java对象返回
- `CompletableFuture<T>`：将Response Body适配成`CompletableFuture<T>`对象返回
- `Void`：不关注返回类型可以使用Void
- `Response<T>`：将Response适配成`Response<T>`对象返回
- `Call<T>`：不执行适配处理，直接返回`Call<T>`对象
- `Mono<T>`：Project Reactor响应式返回类型
- `Single<T>`：Rxjava响应式返回类型（支持Rxjava2/Rxjava3）
- `Completable`：Rxjava响应式返回类型，HTTP请求没有响应体（支持Rxjava2/Rxjava3）
```java
@RetrofitClient(baseUrl = "${test.baseUrl}")
public interface HttpApi {

    @POST("getString")
    String getString(@Body Person person);

    @GET("person")
    Result<Person> getPerson(@Query("id") Long id);

    @GET("person")
    CompletableFuture<Result<Person>> getPersonCompletableFuture(@Query("id") Long id);

    @POST("savePerson")
    Void savePersonVoid(@Body Person person);

    @GET("person")
    Response<Result<Person>> getPersonResponse(@Query("id") Long id);

    @GET("person")
    Call<Result<Person>> getPersonCall(@Query("id") Long id);

    @GET("person")
    Mono<Result<Person>> monoPerson(@Query("id") Long id);

    @GET("person")
    Single<Result<Person>> singlePerson(@Query("id") Long id);

    @GET("ping")
    Completable ping();
}
```
可以通过继承`CallAdapter.Factory`扩展`CallAdapter`。<br />组件支持通过`retrofit.global-call-adapter-factories`配置全局调用适配器工厂：
```yaml
retrofit:
  # 全局转换器工厂(组件扩展的`CallAdaptorFactory`工厂已经内置，这里请勿重复配置)
  global-call-adapter-factories:
    # ...
```
针对每个Java接口，还可以通过`@RetrofitClient.callAdapterFactories`指定当前接口采用的`CallAdapter.Factory`。<br />建议：将`CallAdapter.Factory`配置成Spring Bean
<a name="hiK7K"></a>
### 数据转码器
Retrofit使用`Converter`将`@Body`注解的对象转换成Request Body，将Response Body转换成一个Java对象，可以选用以下几种`Converter`：

- Gson：com.squareup.Retrofit:converter-gson
- Jackson：com.squareup.Retrofit:converter-jackson
- Moshi：com.squareup.Retrofit:converter-moshi
- Protobuf：com.squareup.Retrofit:converter-protobuf
- Wire：com.squareup.Retrofit:converter-wire
- Simple XML：com.squareup.Retrofit:converter-simplexml
- JAXB：com.squareup.retrofit2:converter-jaxb
- fastJson：com.alibaba.fastjson.support.retrofit.Retrofit2ConverterFactory

组件支持通过`retrofit.global-converter-factories`配置全局`Converter.Factory`，默认的是`retrofit2.converter.jackson.JacksonConverterFactory`。<br />如果需要修改Jackson配置，自行覆盖`JacksonConverterFactory`的bean配置即可。
```yaml
retrofit:
   # 全局转换器工厂
   global-converter-factories:
      - com.github.lianjiatech.retrofit.spring.boot.core.BasicTypeConverterFactory
      - retrofit2.converter.jackson.JacksonConverterFactory
```
针对每个Java接口，还可以通过`@RetrofitClient.converterFactories`指定当前接口采用的`Converter.Factory`。<br />建议：将`Converter.Factory`配置成Spring Bean。
<a name="ov365"></a>
### 元注解
`@RetrofitClient`、`@Retry`、`@Logging`、`@Resilience4jDegrade`等注解支持元注解、继承以及`@AliasFor`。
```java
@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.TYPE)
@Documented
@Inherited
@RetrofitClient(baseUrl = "${test.baseUrl}")
@Logging(logLevel = LogLevel.WARN)
@Retry(intervalMs = 200)
public @interface MyRetrofitClient {

    @AliasFor(annotation = RetrofitClient.class, attribute = "converterFactories")
    Class<? extends Converter.Factory>[] converterFactories() default {GsonConverterFactory.class};

    @AliasFor(annotation = Logging.class, attribute = "logStrategy")
    LogStrategy logStrategy() default LogStrategy.BODY;
}
```
<a name="MxMiy"></a>
## 其他功能示例
<a name="vbuyz"></a>
### form参数
```java
@FormUrlEncoded
@POST("token/verify")
Object tokenVerify(@Field("source") String source,@Field("signature") String signature,@Field("token") String token);


@FormUrlEncoded
@POST("message")
CompletableFuture<Object> sendMessage(@FieldMap Map<String, Object> param);
```
<a name="qoVVM"></a>
### 文件上传
<a name="go4ik"></a>
#### 创建`MultipartBody.Part`
```java
// 对文件名使用URLEncoder进行编码
public ResponseEntity importTerminology(MultipartFile file){
     String fileName=URLEncoder.encode(Objects.requireNonNull(file.getOriginalFilename()),"utf-8");
     okhttp3.RequestBody requestBody=okhttp3.RequestBody.create(MediaType.parse("multipart/form-data"),file.getBytes());
     MultipartBody.Part part=MultipartBody.Part.createFormData("file",fileName,requestBody);
     apiService.upload(part);
     return ok().build();
}
```
<a name="F8rox"></a>
#### HTTP上传接口
```java
@POST("upload")
@Multipart
Void upload(@Part MultipartBody.Part file);
```
<a name="JKJ4z"></a>
### 文件下载
<a name="B9qbB"></a>
#### HTTP下载接口
```java
@RetrofitClient(baseUrl = "https://img.ljcdn.com/hc-picture/")
public interface DownloadApi {

    @GET("{fileKey}")
    Response<ResponseBody> download(@Path("fileKey") String fileKey);
}
```
<a name="FTYqF"></a>
#### HTTP下载使用
```java
@SpringBootTest(classes = RetrofitTestApplication.class)
@RunWith(SpringRunner.class)
public class DownloadTest {
    @Autowired
    DownloadApi downLoadApi;

    @Test
    public void download() throws Exception {
        String fileKey = "6302d742-ebc8-4649-95cf-62ccf57a1add";
        Response<ResponseBody> response = downLoadApi.download(fileKey);
        ResponseBody responseBody = response.body();
        // 二进制流
        InputStream is = responseBody.byteStream();

        // 具体如何处理二进制流，由业务自行控制。这里以写入文件为例
        File tempDirectory = new File("temp");
        if (!tempDirectory.exists()) {
            tempDirectory.mkdir();
        }
        File file = new File(tempDirectory, UUID.randomUUID().toString());
        if (!file.exists()) {
            file.createNewFile();
        }
        FileOutputStream fos = new FileOutputStream(file);
        byte[] b = new byte[1024];
        int length;
        while ((length = is.read(b)) > 0) {
            fos.write(b, 0, length);
        }
        is.close();
        fos.close();
    }
}
```
<a name="wLsXH"></a>
### 动态URL
使用`@Url`注解可实现动态URL。此时，baseUrl配置任意合法url即可。例如：http://github.com/ 。运行时只会根据`@Url`地址发起请求。<br />注意：`@Url`必须放在方法参数的第一个位置，另外，`@GET`、`@POST`等注解上，不需要定义端点路径。
```java
@GET
Map<String, Object> test3(@Url String url,@Query("name") String name);
```
<a name="aOFhE"></a>
### DELETE请求添加请求体
```java
@HTTP(method = "DELETE", path = "/user/delete", hasBody = true)
```
<a name="Cr1h0"></a>
### GET请求添加请求体
okhttp3自身不支持GET请求添加请求体，源码如下：<br />![DM_20230806133459_001.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1691300456683-941e04a0-1756-49b9-85ae-7cd05e600e42.png#averageHue=%233c3431&clientId=u1e993f32-b9a2-4&from=ui&id=u43ad36a5&originHeight=211&originWidth=745&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=472639&status=done&style=none&taskId=u5a1ce1f7-1cc6-434c-a8a6-1448449a728&title=)![DM_20230806133516_001.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1691300285034-41fb3ca1-5631-4291-8322-9900ffe90641.png#averageHue=%23373432&clientId=u1e993f32-b9a2-4&from=ui&id=uf1280a41&originHeight=61&originWidth=703&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=128994&status=done&style=none&taskId=udeb4f7cb-644d-4daa-8a0c-e2bd6518733&title=)<br />作者给出了具体原因，可以参考：[https://github.com/square/okhttp/issues/3154](https://github.com/square/okhttp/issues/3154)<br />但是，如果实在需要这么做，可以使用：`@HTTP(method = "get", path = "/user/get", hasBody = true)`，使用小写get绕过上述限制。
