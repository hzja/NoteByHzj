SpringMVC
<a name="Whzkf"></a>
### 拼接在url后面的传参形式
<a name="w8rN7"></a>
#### `@PathVariable`
简介：`@PathVariable`用于接收restful方式的请求。<br />eg：http://ip:port/qumy/test/1
```java
@GetMapping("/test/{id}")
public String test(@PathVariable("id") Long id) {
    return null;
}
```
<a name="3XNVB"></a>
#### `@RequestParam`
简介：`@RequestParam`用于接收params方式的请求。<br />eg：http://ip:port/qumy/test?id=1
```java
 @GetMapping("/test")
public String test(@RequestParam Long id) {
    return null;
}
```
<a name="QT8gH"></a>
#### `@Validated`
简介：`@Validated`注解可以用一个模型来接收地址栏中的参数。<br />eg：http://ip:port/qumy/test?id=1
```java
@GetMapping(value = "/test")
public String test(@Validated User user) {
    return null;
}
```
<a name="tEXKt"></a>
### 接收request header部分的参数
<a name="W5Ivd"></a>
#### `@RequestHeader`
简介：`@RequestHeader` 注解，可以把Request请求header部分的值绑定到方法的参数上。<br />request header(请求头)：
```http
Host                    localhost:8080
Accept                  text/html,application/xhtml+xml,application/xml;q=0.9
Accept-Language         fr,en-gb;q=0.7,en;q=0.3
Accept-Encoding         gzip,deflate
Accept-Charset          ISO-8859-1,utf-8;q=0.7,*;q=0.7
Keep-Alive              300
```
```java
@GetMapping("/test")
public String test(@RequestHeader("Accept-Encoding") String encoding)  {
    return null;
}
```
<a name="99qbW"></a>
#### `@CookieValue`
简介：`@CookieValue` 可以把Request header中关于cookie的值绑定到方法的参数上。<br />cookie：
```java
JSESSIONID=415A4AC178C59DACE0B2C9CA727CDD84
```
```java
@GetMapping("/test")
public String test(@CookieValue("JSESSIONID") String cookie)  {
    return null;
}
```
<a name="7AUrs"></a>
### 接收request body部分的参数
<a name="DanrO"></a>
#### `@RequestBody`
简介：`@RequestBody`注解用来接收request的body中的参数，`@RequestBody`可以将多个参数放入到一个实体类或者Map中。
```java
// 实体类
@PostMapping(value = "/test")
public String test(@RequestBody ParameterModel parameterModel) {
    return null;
}
// Map
@PostMapping(value = "/test)
public String test(@RequestBody Map<String, Object> paramMap) {
    return null;
}
```

