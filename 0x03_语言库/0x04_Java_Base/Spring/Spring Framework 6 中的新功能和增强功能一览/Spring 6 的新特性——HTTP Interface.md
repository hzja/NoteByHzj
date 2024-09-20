JavaSpring<br />在对接第三方系统时，还在使用 httpclient、okhttp 等方式进行硬编码的形式实现对接。其实，使用 feign 来实现更为方便快捷。feign 除了服务间的 http 调用外，还支持非服务间的 http 调用。只是一些思维还未转变过来，对 feign 的底层实现也缺乏一定的了解。<br />近期，Spring 6 的第一个 GA 版本发布了，其中带来了一个新的特性——HTTP Interface。这个新特性，可以让开发者将 HTTP 服务，定义成一个包含特定注解标记的方法的 Java 接口，然后通过对接口方法的调用，完成 HTTP 请求。看起来很像使用 Feign 来完成远程服务调用，这可能是 Spring “抛弃” feign 等 http 调用的一种趋势，Spring 要规范和简化这一块的实现了，让开发者更关注聚集于业务。下面参考官方文档来完成一个 Demo。
<a name="AezcH"></a>
## 完成一个 Demo
首先创建一个简单的 HTTP 服务，这一步可以创建一个简单的 Spring Boot 工程来完成。<br />先创建一个实体类：
```java
public class User implements Serializable {

    private int id;
    private String name;
    // 省略构造方法、Getter和Setter
    @Override
    public String toString() {
        return id + ":" + name;
    }
}
```
再写一个简单的 Controller：
```java
@GetMapping("/users")
public List<User> list() {
    return IntStream.rangeClosed(1, 10)
            .mapToObj(i -> new User(i, "User" + i))
            .collect(Collectors.toList());
}
```
确保启动服务之后，能够从http://localhost:8080/users地址获取到一个包含十个用户信息的用户列表。<br />下面新建一个 Spring Boot 工程。<br />这里需要注意，Spring Boot 的版本至少需要是 3.0.0，这样它以来的 Spring Framework 版本才是 6.0 的版本，才能够包含 HTTP Interface 特性，另外，Spring Framework 6.0 和 Spring Boot 3.0 开始支持的 Java 版本最低是 17，因此，需要选择至少是 17 的 Java 版本。<br />另外，需要依赖 Spring Web 和 Spring Reactive Web 依赖，原因下文中会提到。<br />创建好新的 Spring Boot 工程后，首先需要定义一个 HTTP Interface 接口。最简单的定义如下即可：
```java
public interface UserApiService {
    @GetExchange("/users")
    List<User> getUsers();
}
```
然后，可以写一个测试方法。
```java
@Test
void getUsers() {
    WebClient client = WebClient.builder().baseUrl("http://localhost:8080/").build();
    HttpServiceProxyFactory factory = HttpServiceProxyFactory.builder(WebClientAdapter.forClient(client)).build();
    UserApiService service = factory.createClient(UserApiService.class);
    List<User> users = service.getUsers();
    for (User user : users) {
        System.out.println(user);
    }
}
```
最终回打印获取到的是个用户信息：
```
1:User1
2:User2
...
9:User9
10:User10
```
以上是一个最简单的示例，下面看看其中的一些细节。
<a name="ZGosy"></a>
## `GetExchange`（`HttpExchange`）注解
上文例子中的 `GetExchange` 注解代表这个方法代替执行一个 HTTP Get 请求，与此对应，Spring 还包含了其他类似的注解：<br />![GetExchange](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1674199844046-ad25f62d-5236-4e90-877a-fe3d943d2300.jpeg#averageHue=%2343382d&clientId=u0a53beae-c003-4&from=paste&id=u49996cdb&originHeight=485&originWidth=812&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u3a0841f0-232d-4c1f-a582-530ecadfcc7&title=GetExchange "GetExchange")<br />这些注解定义在spring-web模块的org.springframework.web.service.annotation包下，除了 `HttpExchange` 之外，其他的几个都是 `HttpExchange` 的特殊形式，这一点与 Spring MVC 中的 `RequestMapping`/`GetMapping` 等注解非常相似。<br />以下是 `HttpExchange` 的源码：
```java
@Target({ElementType.TYPE, ElementType.METHOD})
@Retention(RetentionPolicy.RUNTIME)
@Documented
@Mapping
@Reflective(HttpExchangeReflectiveProcessor.class)
public @interface HttpExchange {

    @AliasFor("url")
    String value() default "";

    @AliasFor("value")
    String url() default "";

    String method() default "";

    String contentType() default "";

    String[] accept() default {};

}
```
在上面的例子中，只指定了请求的资源路径。
<a name="MZRuT"></a>
## UserApiService 实例的创建
在上面例子中，定义的 HTTP Interface 接口是 UserApiService，在测试方法中，通过 `HttpServiceProxyFactory` 创建了 `UserApiService` 的实例，这是参考了 Spring 的官方文档的写法。<br />也可以将创建的过程写到一个 `@Bean` 方法中，从而可以将创建好的实例注入到其他的组件中。<br />再定义 UserApiService 的时候，只是声明了一个接口，那具体的请求操作是怎么发出的呢，可以通过 DEBUG 模式看得出来，这里创建的 UserApiService 的实例，是一个代理对象：<br />![代理对象](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1674199844023-77bd22aa-fb36-4ba7-b5b4-5e5eb353b51f.jpeg#averageHue=%232c313a&clientId=u0a53beae-c003-4&from=paste&id=ub8ba0c71&originHeight=735&originWidth=1034&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u6c9ee7ed-9eb5-4341-9c55-9b18b92a15e&title=%E4%BB%A3%E7%90%86%E5%AF%B9%E8%B1%A1 "代理对象")<br />目前，Spring 还没有提供更方便的方式来创建这些代理对象，不过，之后的版本肯定会提供，如果感兴趣的话，可以从 `HttpServiceProxyFactory` 的`createClient`方法的源码中看到一些与创建 AOP 代理相似的代码，因此，推测 Spring 之后可能会增加类似的注解来方便地创建代理对象。
<a name="j5Xv9"></a>
## 其他特性
除了上述例子中的简单使用之外，添加了 `HttpExchange` 的方法还支持各种类型的参数，这一点也与 Spring MVC 的 Controller 方法类似，方法的返回值也可以是任意自定义的实体类型（就像上面的例子一样），此外，还支持自定义的异常处理。
<a name="o9KAh"></a>
## 为什么需要 Spring Reactive Web 的依赖
上文中创建工程的时候，引入了 Spring Reactive Web 的依赖，在创建代理的service对象的时候，使用了其中的 `WebClient` 类型。这是因为，HTTP Interface 目前只内置了 `WebClient` 的实现，它属于 Reactive Web 的范畴。Spring 在会在后续版本中推出基于 RestTemplate 的实现。
