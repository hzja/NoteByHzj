SpringMVC ResponseEntity
<a name="qxV12"></a>
## `ResponseEntity`
`ResponseEntity`对象是`Spring`对请求响应的封装。它继承了`HttpEntity`对象，包含了Http的响应码（`httpstatus`）、响应头（`header`）、响应体(`body`)三个部分。一个获取用户信息的Spring MVC接口通常直接返回实体即可（配合`@RestController`）:
```java
@GetMapping("/user")
public User userinfo() {
    User user = new User();
    user.setUsername("fcant.cn");
    return user;
}
```
等同于使用`ResponseEntity`作为控制器接口的返回值:
```java
@GetMapping("/userinfo")
public ResponseEntity<User> user() {
    User user = new User();
    user.setUsername("fcant.cn");
    return ResponseEntity.ok(user);
}
```
但是使用`ResponseEntity`时可以做更多事情。
<a name="up4mA"></a>
### 自定义响应码
上面的`ResponseEntity.ok`已经包含了返回200Http响应码，还可以通过`ResponseEntity.status(HttpStatus|int)`来自定义返回的响应码。
<a name="bjuGD"></a>
### 自定义响应体
放置响应的响应体，通常就是接口的数据，这里是一个例子：
```java
ResponseEntity.status(HttpStatus.OK)
               .body(Object)
```
<a name="aVDWI"></a>
### 响应头
通常指定Spring MVC接口的响应头是通过`@RequestMapping`和其Restful系列注解中的`header()`、`consumes`、`produces()`这几个属性设置。如果使用了`ResponseEntity`，可以通过链式调用来设置：
```java
ResponseEntity.status(HttpStatus.OK)
               .allow(HttpMethod.GET)
               .contentType(MediaType.APPLICATION_JSON)
               .contentLength(1048576)
               .header("My-Header","fcant.cn")
               .build();
```
所有的标准请求头都有对应的设置方法，也可以通过`header(String headerName, String... headerValues)`设置自定义请求头。
<a name="bAwlf"></a>
### 大致原理
来看一个用来处理Spring MVC控制器接口返回值的抽象接口`HandlerMethodReturnValueHandler`：
```java
public interface HandlerMethodReturnValueHandler {

    /**
     * 支持的返回值类型
     */
    boolean supportsReturnType(MethodParameter returnType);

    /**
     *  将数据绑定到视图，并设置处理标志以指示已直接处理响应，后续的其它方法就不处理了，优先级非常高
     */
    void handleReturnValue(@Nullable Object returnValue, MethodParameter returnType,
                           ModelAndViewContainer mavContainer, NativeWebRequest webRequest) throws Exception;

}
```
它的一个重要实现`HttpEntityMethodProcessor`就是处理返回类型为`HttpEntity`的控制器方法的处理器。它会把`ResponseEntity`携带的三种信息交给`ServletServerHttpResponse`对象渲染视图，并设置处理标志以指示已直接处理响应，后续的其它方法就不处理了，优先级非常高。
<a name="jKLjy"></a>
## 实战运用
通常让写个下载文件接口都是拿到`HttpServletResponse`对象，然后配置好`Content-Type`往里面写流。如果用`ResponseEntity`会更加简单优雅。
```java
@GetMapping("/download")
public ResponseEntity<Resource> load() {
    ClassPathResource classPathResource = new ClassPathResource("application.yml");
    String filename = classPathResource.getFilename();
    HttpHeaders httpHeaders = new HttpHeaders();
    httpHeaders.setContentDisposition(ContentDisposition.inline().filename(filename, StandardCharsets.UTF_8).build());
    return ResponseEntity.ok()
            .headers(httpHeaders)
            .body(classPathResource);
}
```
上面是一个把Spring Boot配置文件 application.yml下载下来的例子。主要分为三步：

- 将要下载的文件封装成`org.springframework.core.io.Resource`对象，它有很多实现。这里用了`ClassPathResource`，其它`InputStreamResource`、`PathResource`都是常用的实现。
- 然后配置下载文件请求头`Content-Disposition`。针对下载它有两种模式: `inline`表示在浏览器直接展示文件内容；`attachment`表示下载为文件。另外下载后的文件名也在这里指定，请不要忘记文件扩展名，例如这里application.yml。如果不指定`Content-Disposition`，需要根据文件扩展名设置对应的`Content-Type`，会麻烦一些。
- 最后是组装`ResponseEntity<Resource>`返回。

原理参见`org.springframework.http.converter.ResourceHttpMessageConverter`<br />`inline`模式下载文件对应的可以显示文件的内容。
