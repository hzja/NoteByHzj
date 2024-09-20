SpringCloud Feign<br />基于 spring-boot-starter-parent 2.1.9.RELEASE, spring-cloud-openfeign 2.1.3.RELEASE
<a name="d6oEo"></a>
## 引子
Feign 是一个声明式、模板化的 HTTP 客户端，简化了系统发起 Http 请求。创建它时，只需要创建一个接口，然后加上 `FeignClient` 注解,使用它时，就像调用本地方法一样，作为开发者完全感知不到这是在调用远程的方法，也感知不到背后发起了 HTTP 请求：
```java
/**
 * @author fcant
 * @suammry 客户端
 */
@FeignClient(value = "xxClient",url = "${xx.host:www.axin.com}")
public interface DemoClient {
    @PostMapping(value = "/xxx/url", headers = "Content-Type=application/json"})
    yourResponse requestHTTP(@RequestBody JSONObject param);
}
```
上述的代码就是一个定义一个 Feign HTTP 客户端，在其他类中只需要 `@Autowired` DemoClient，就可以像调用本地方法一样发起 HTTP 请求。<br />因为 FeignClient 将发起 HTTP 请求与解析返回报文都做了包装，如果业务场景需要定制一些调用机制，比如：

1. 在发起请求响应超时失败时自动重试 —— 自定义重试机制
2. 单独对某些异常的 HTTP 状态码特殊处理 —— 自定义 ErrorDecoder
3. 服务端接口需要验证签名，所以在发起请求时要生成签名然后传过去 —— 定义 Fegin 拦截器

基于此，本文就以上述 3 个需求场景为例来介绍如何自定义 FeignClient 的配置
<a name="hCUUf"></a>
## FeignClient 的默认配置类
> Feign Client 默认的配置类为 `FeignClientsConfiguration`， 这个类在 spring-cloud-netflix-core 的 jar 包下。

默认注入了很多 Feign 相关的配置 Bean，包括 `FeignRetryer`、 `FeignLoggerFactory` 和 `FormattingConversionService` 等。另外，`Decoder`、`Encoder` 和 `Contract` 这 3 个类在没有 Bean 被注入的情况下，会自动注入默认配置的 Bean，即 ResponseEntity Decoder、`SpringEncoder` 和 `SpringMvcContract`。<br />如果不知道如何自己定义配置时，不仿参考默认配置是如何实现的。
<a name="P1Zdz"></a>
## `FeignClient` 注解参数
每个注解参数都做了注释，详情请见下方源码：
```java
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface FeignClient {
    /**
    * 指定FeignClient的名称，如果项目使用了Ribbon，name属性会作为微服务的名称，用于服务发现
    */
    @AliasFor("name")
    String value() default "";
    @Deprecated
    String serviceId() default "";
    @AliasFor("value")
    String name() default "";
    /**
    * Sets the <code>@Qualifier</code> value for the feign client.
    * 这个bean在应用上下文中的名字为接口的全限定名，也可以使用 @FeignClient 注解中的 qualifier 属性给bean指定一个别名
    */
    String qualifier() default "";
    /**
    * url地址
    */
    String url() default "";
    /**
    * 当发生404错误，如果该字段为true，会调用decoder进行解码，否则抛出FeignException
    */
    boolean decode404() default false;
    /**
    * 指定FeignClient 的配置类，优先级最高，默认的配置类为 FeignClientsConfiguration类
    */
    Class<?>[] configuration() default {};
    /**
    * 配置熔断器的处理类
    */
    Class<?> fallback() default void.class;
    /**
    * 工厂类，用于生产fallback类示例，通过这个属性可以实现每个接口通用的容错逻辑，减少重复代码
    */
    Class<?> fallbackFactory() default void.class;
    /**
    * 定义统一的路径前缀
    */
    String path() default "";
    /**
    * Whether to mark the feign proxy as a primary bean. Defaults to true.
    */
    boolean primary() default true;
}
```
<a name="bjbbZ"></a>
## 自定义 Feign 配置类
在Spring Cloud 中，可以通过 `@FeignClient` 注解声明额外的配置(比 `FeignClientsConfiguration` 级别高)去控制 feign 客户端，以一开始的 feign 接口为例:
```java
/**
 * @author fcant
 * @suammry xx 客户端
 */
@FeignClient(value = "xxClient",url = "${xx.host:www.axin.com}",configuration = MyConfiguration.class)
public interface DemoClient {
    @PostMapping(value = "/xxx/url", headers = "Content-Type=application/json"})
    yourResponse requestHTTP(@RequestBody JSONObject param);
}
```
在上面这个示例中，feign 客户端在 MyConfiguration 中的配置将会覆盖 `FeignClientsConfiguration` 中的配置。<br />**要注意的是**：MyConfiguration 不需要使用`@Configuration` 注解。如果加上了，它将**全局生效**。
<a name="gtlgm"></a>
### Retryer-重试机制的自定义
```java
/**
 * @author fcant
 * @summary fegin 客户端的自定义配置
 */
public class MyConfiguration {
    /**
     * 自定义重试机制
     * @return
     */
    @Bean
    public Retryer feignRetryer() {
        //fegin提供的默认实现，最大请求次数为5，初始间隔时间为100ms，下次间隔时间1.5倍递增，重试间最大间隔时间为1s，
        return new Retryer.Default();
    }
}
```
<a name="s9lr2"></a>
### ErrorDecoder-错误解码器的自定义
> 当 feign 调用返回 HTTP 报文时，会触发这个方法，方法内可以获得 HTTP 状态码，可以用来定制一些处理逻辑等等。

```java
/**
 * @author fcant
 * @summary fegin 客户端的自定义配置
 */
@Slf4j
public class MyConfiguration {
    /**
     * 自定义重试机制
     * @return
     */
    @Bean
    public Retryer feignRetryer() {
        //最大请求次数为5，初始间隔时间为100ms，下次间隔时间1.5倍递增，重试间最大间隔时间为1s，
        return new Retryer.Default();
    }
    @Bean
    public ErrorDecoder feignError() {
        return (key, response) -> {
            if (response.status() == 400) {
                log.error("请求xxx服务400参数错误,返回:{}", response.body());
            }
            if (response.status() == 409) {
                log.error("请求xxx服务409异常,返回:{}", response.body());
            }
            if (response.status() == 404) {
                log.error("请求xxx服务404异常,返回:{}", response.body());
            }
            // 其他异常交给Default去解码处理
            // 这里使用单例即可，Default不用每次都去new
            return new ErrorDecoder.Default().decode(key, response);
        };
    }
}
```
采用了 lambda 的写法，response 变量是 Response 类型，通过 `status()`方法可以拿到返回的 HTTP 状态码，`body()`可以获得到响应报文。
<a name="55sly"></a>
### Feign 拦截器实践
> 拦截器在请求发出之前执行，在拦截器代码里可以修改请求参数，header 等等，如果有签名生成的需求，可以放在拦截器中来实现

```java
/**
 * @author fcant
 * @summary fegin 客户端的自定义配置
 */
@Slf4j
public class MyConfiguration {
    /**
     * 自定义重试机制
     * @return
     */
    @Bean
    public Retryer feignRetryer() {
        //最大请求次数为5，初始间隔时间为100ms，下次间隔时间1.5倍递增，重试间最大间隔时间为1s，
        return new Retryer.Default();
    }
    @Bean
    public ErrorDecoder feignError() {
        return (key, response) -> {
            if (response.status() == 400) {
                log.error("请求xxx服务400参数错误,返回:{}", response.body());
            }
            if (response.status() == 409) {
                log.error("请求xxx服务409异常,返回:{}", response.body());
            }
            if (response.status() == 404) {
                log.error("请求xxx服务404异常,返回:{}", response.body());
            }
            // 其他异常交给Default去解码处理
            // 这里使用单例即可，Default不用每次都去new
            return new ErrorDecoder.Default().decode(key, response);
        };
    }
     /**
     * fegin 拦截器
     * @return
     */
    @Bean
    public RequestInterceptor cameraSign() {
        return template -> {
            // 如果是get请求
            if (template.method().equals(Request.HttpMethod.GET.name())) {
                //获取到get请求的参数
                Map<String, Collection<String>> queries = template.queries();
            }
            //如果是Post请求
            if (template.method().equals(Request.HttpMethod.POST.name())) {
                //获得请求body
                String body = template.requestBody().asString();
                JSONPObject request = JSON.parseObject(body, JSONPObject.class);
            }
            //Do what you want... 例如生成接口签名
            String sign = "根据请求参数生成的签名";
            //放入url？之后
            template.query("sign", sign);
            //放入请求body中
            String newBody = "原有body" + sign;
            template.body(Request.Body.encoded(newBody.getBytes(StandardCharsets.UTF_8), StandardCharsets.UTF_8));
        };
    }
}
```
可以看到代码中给了如何获取请求参数和修改请求参数的示例。
