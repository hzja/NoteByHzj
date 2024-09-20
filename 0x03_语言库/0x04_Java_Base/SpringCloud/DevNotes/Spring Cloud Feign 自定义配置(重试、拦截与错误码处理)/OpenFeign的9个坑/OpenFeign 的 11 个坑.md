JavaOpenFeign
<a name="WIN1u"></a>
## 默认未启用连接池
OpenFeign 默认使用 jdk 自带的 `HttpURLConnection`，HttpURLConnection 没有连接池、性能和效率比较低，如果采用默认，很可能会遇到性能问题导致系统故障。
```
// 启用 httpclient
feign.httpclient.enabled=true
// 或者启用 okhttp
feign.okhttp.enabled=true
```
<a name="G6D9I"></a>
## 未指定服务名称或URL
不正确地使用`@FeignClient`注解，例如未指定服务名称或 URL。
```java
// 错误示例，未指定服务名称或URL
@FeignClient
public interface UserService {
    // ...
}
```
正确示例：
```java
// 正确示例，指定服务名称
@FeignClient(name = "user-service")
public interface UserService {
    // ...
}

// 正确示例，指定URL
@FeignClient(url = "http://localhost:8080")
public interface UserService {
    // ...
}
```
<a name="rBZ7p"></a>
## 忽略对请求参数的编码
忽略对请求参数的编码，可能导致请求失败或数据丢失。
```java
// 错误示例，未对请求参数进行编码
@FeignClient(name = "user-service")
public interface UserService {
    @GetMapping("/users/{id}")
    User getUserById(@PathVariable Long id, 
                     @RequestParam String name);
}

// 正确示例，对请求参数进行编码
@FeignClient(name = "user-service")
public interface UserService {
    @GetMapping("/users/{id}")
    User getUserById(@PathVariable Long id, 
                     @RequestParam("name") String name);
}
```
<a name="cYELQ"></a>
## 自动解析为类对象
忽略对响应结果的解码，可能导致无法正确处理服务端返回的数据。
```java
// 错误示例，未对响应结果进行解码
@FeignClient(name = "user-service")
public interface UserService {
    @GetMapping("/users/{id}")
    String getUserById(@PathVariable Long id);
}

// 正确示例，对响应结果进行解码
@FeignClient(name = "user-service")
public interface UserService {
    @GetMapping("/users/{id}")
    User getUserById(@PathVariable Long id);
}
```
<a name="HOdam"></a>
## 忽略对请求头的设置
忽略对请求头的设置，可能导致服务端无法正确识别请求来源或无法正确处理请求。
```java
// 错误示例，未设置请求头
@FeignClient(name = "user-service")
public interface UserService {
    @GetMapping("/users/{id}")
    User getUserById(@PathVariable Long id);
}

// 正确示例，设置请求头
@FeignClient(name = "user-service")
public interface UserService {
    @GetMapping("/users/{id}")
    User getUserById(@PathVariable Long id, 
                     @RequestHeader("Authorization") String token);
}
```
<a name="NKbMy"></a>
## Feign实例的使用
将Feign客户端的实现类直接暴露给其他模块使用，导致代码耦合度过高和返回错误数据。
```java
// 错误示例，将Feign客户端的实现类直接暴露给其他模块使用
public class UserController {
    private UserService userService = new UserServiceFallback();

    // ...
}

// 正确示例，使用接口注入Feign客户端
public class UserController {
    private UserService userService;

    public UserController(UserService userService) {
        this.userService = userService;
    }

    // ...
}
```
<a name="H0vnD"></a>
## 忽略对错误处理的设置
忽略对错误处理的设置，可能导致无法正确处理服务端返回的错误信息。
```java
// 错误示例，未对错误进行处理
@FeignClient(name = "user-service")
public interface UserService {
    @GetMapping("/users/{id}")
    User getUserById(@PathVariable Long id);
}

// 正确示例，对错误进行处理
@FeignClient(name = "user-service", fallback = UserServiceFallback.class)
public interface UserService {
    @GetMapping("/users/{id}")
    User getUserById(@PathVariable Long id);
}

@Component
class UserServiceFallback implements UserService {
    @Override
    public User getUserById(Long id) {
        // 返回默认值或抛出异常
    }
}
```
<a name="neKj6"></a>
## 超时时间设置
默认是连接超时 10s，读超时 60s，在源码`feign.Request`的内部类`Options`中定义。<br />超时时间全局设置：
```
feign.client.config.default.connectTimeout=2000
feign.client.config.default.readTimeout=60000
```
或者硬编码设置。
```java
// 错误示例，未对超时进行配置
@FeignClient(name = "user-service")
public interface UserService {
    @GetMapping("/users/{id}")
    User getUserById(@PathVariable Long id);
}

// 正确示例，对超时进行配置
@FeignClient(name = "user-service", configuration = UserServiceConfig.class)
public interface UserService {
    @GetMapping("/users/{id}")
    User getUserById(@PathVariable Long id);
}

@Configuration
class UserServiceConfig {
    @Bean
    public Request.Options options() {
        return new Request.Options(5000, 5000);
    }
}
```
<a name="YJQ1M"></a>
## 熔断设置
默认熔断是没有启用的。
```
feign.hystrix.enabled=true
// 或者
feign.sentinel.enabled=true
```
<a name="VV7XY"></a>
## 忽略对断路器的配置
这是很多新人会犯的错误。
```java
// 错误示例，未对断路器进行配置
@FeignClient(name = "user-service")
public interface UserService {
    @GetMapping("/users/{id}")
    User getUserById(@PathVariable Long id);
}

// 正确示例，对断路器进行配置
@FeignClient(name = "user-service", fallbackFactory = UserServiceFallbackFactory.class)
public interface UserService {
    @GetMapping("/users/{id}")
    User getUserById(@PathVariable Long id);
}

@Component
class UserServiceFallbackFactory implements FallbackFactory<UserService> {
    @Override
    public UserService create(Throwable cause) {
        return new UserServiceFallback(cause);
    }
}

class UserServiceFallback implements UserService {
    private Throwable cause;

    public UserServiceFallback(Throwable cause) {
        this.cause = cause;
    }

    @Override
    public User getUserById(Long id) {
        // 返回默认值或抛出异常
    }
}
```
如果不开启熔断，就没问题。但是熔断配置已开启，系统就无法启动了。
<a name="uD1xo"></a>
## 重试默认不开启
OpenFeign 默认是不支持重试的，可以在源代码`FeignClientsConfiguration`中`feignRetryer`中看出。<br />默认值：
```java
@Bean
@ConditionalOnMissingBean
public Retryer feignRetryer() {
    return Retryer.NEVER_RETRY;
}
```
正确用法：
```java
@Bean
@ConditionalOnMissingBean
public Retryer feignRetryer() {
    return new Retryer.Default(100, 1000, 2);
}
```
