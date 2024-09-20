JavaSpringBoot
<a name="M2jAi"></a>
## http interface
从 Spring 6 和 Spring Boot 3 开始，Spring 框架支持将远程 HTTP 服务代理成带有特定注解的 Java http interface。类似的库，如 OpenFeign 和 Retrofit 仍然可以使用，但 http interface 为 Spring 框架添加内置支持。
<a name="gDTsc"></a>
## 什么是声明式客户端
声明式 http 客户端主旨是使得编写 java http 客户端更容易。为了贯彻这个理念，采用了通过处理注解来**自动生成请求的方式**（官方称呼为声明式、模板化）。通过声明式 http 客户端实现就可以在 java 中像调用一个本地方法一样完成一次 http 请求，大大减少了编码成本，同时提高了代码可读性。

- 举个例子，如果想调用 /tenants 的接口，只需要定义如下的接口类即可
```java
public interface TenantClient {

  @GetExchange("/tenants")
  Flux<User> getAll();
}
```
Spring 会在运行时提供接口的调用的具体实现，如上请求可以如 Java 方法一样调用
```java
@Autowired
TenantClient tenantClient;

tenantClient.getAll().subscribe(

);
```
<a name="RrQ3t"></a>
## 测试使用
<a name="fEfJA"></a>
### 1、Maven 依赖
```xml
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-web</artifactId>
</dependency>

<!-- For webclient support -->
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-webflux</artifactId>
</dependency>
```
目前官方只提供了非阻塞 webclient 的 http interface 实现，所以依赖中需要添加 webflux
<a name="YeUDr"></a>
### 2、创建 Http interface 类型

- 需要再接口类上添加 `@HttpExchange` 声明此类事 http interface 端点
```java
@HttpExchange
public interface DemoApi {

    @GetExchange("/admin/tenant/list")
    String list();
```

- 方法上支持如下注解
```java
@GetExchange:  for HTTP GET requests.
@PostExchange:  for HTTP POST requests.
@PutExchange: for HTTP PUT requests.
@DeleteExchange: for HTTP DELETE requests.
@PatchExchange:  for HTTP PATCH requests.
```

- 方法参数支持的注解
```
@PathVariable: 占位符参数.
@RequestBody: 请求body.
@RequestParam: 请求参数.
@RequestHeader: 请求头.
@RequestPart: 表单请求.
@CookieValue: 请求cookie.
```
<a name="n2lv4"></a>
### 3、注入声明式客户端

- 通过给 `HttpServiceProxyFactory` 注入携带目标接口 baseUrl 的的 webclient，实现 webclient 和 http interface 的关联
```java
@Bean
DemoApi demoApi() {
    WebClient client = WebClient.builder().baseUrl("http://pigx.pigx.vip/").build();
    HttpServiceProxyFactory factory = HttpServiceProxyFactory.builder(WebClientAdapter.forClient(client)).build();
    return factory.createClient(DemoApi.class);
}
```
<a name="wGLC6"></a>
### 4、单元测试调用 http interface
```java
@SpringBootTest
class DemoApplicationTests {
    @Autowired
    private DemoApi demoApi;

    @Test
    void testDemoApi() {
        demoApi.list();
    }
}
```
