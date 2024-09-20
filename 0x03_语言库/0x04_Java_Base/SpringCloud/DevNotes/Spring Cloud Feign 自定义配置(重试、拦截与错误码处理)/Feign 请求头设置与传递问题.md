Java Feign<br />Feign其实也是通过HTTP请求来实现的通信。那么自然绕不开HTTP相关的东西，比如很多系统中权限校验都是通过Header中的参数来实现，需要将前端传过来的header转发到目标服务，这里记录一下关于Header的设置。<br />下面提到的同步/异步 只是记录一下遇到问题的情景， 异步方法的实现方式同样适用于同步方法
<a name="RTDWd"></a>
## 同步
<a name="Js3xP"></a>
### 同步方法一: 通过拦截器
这种方法是比较通用而且也比较常见的，通过实现Feign的`RequestInterceptor`接口，重写里面的`apply`方法，为`RequestTemplate`添加请求头信息<br />代码如下
```java
@Component
public class FeignClientsConfigurationCustom implements RequestInterceptor {
    @Override
    public void apply(RequestTemplate requestTemplate) {
        // 此种方式是线程安全的
        ServletRequestAttributes attributes = (ServletRequestAttributes) RequestContextHolder
            .getRequestAttributes();
        // 不为空时取出请求中的header 原封不动的设置到feign请求中
        if (null != attributes) {
            HttpServletRequest request = attributes.getRequest();
            if (null != request) {
                // 遍历设置 也可从request取出整个Header 写到RequestTemplate 中
                Enumeration<String> headerNames = request.getHeaderNames();
                if (headerNames != null) {
                    while (headerNames.hasMoreElements()) {
                        String name = headerNames.nextElement();
                        String values = request.getHeader(name);
                        requestTemplate.header(name, values);
                    }
                }
            }
        }
    }
}
```
完成上述代码之后 在`FeignClient`注解中加入 `configuration = FeignClientsConfigurationCustom.class` 即可<br />如
```java
@FeignClient(name = "testClient", configuration = FeignClientsConfigurationCustom.class)
public interface testServer{
}
```
<a name="Ziy1J"></a>
### 同步方法二：`@Headers`注解
这种方法比较适用于一些不变的参数，如Content-Type等
```java
@FeignClient(name = "testClient", configuration = FeignClientsConfigurationCustom.class)
public interface testServer{

    @GetMapping("/test")
    @Headers({"Content-Type: application/json","Accept: application/json"})
    String test(@RequestParam String param);
}
```
<a name="dJDIM"></a>
## 异步
此处说的异步场景是通过Spring中的`@Async`实现的<br />如
```java
// 带有@Async注解，异步调用Feign
@Autowired
private TestService testService;

@GetMapping("/test")
public String test(){
    // 此方法是一个异步方法， 在该方法中调用了Feign服务
    testService.testMethod();
    return "Hello world!";
}
```
此时的业务场景是 前端发送一个服务，异步地调用其他微服务的方法， 由于此方法执行耗时可能会比较长，而且对用户来说没有下一步操作，所以直接`return`掉，那么主线程因`return`而关闭，此时在刚刚的FeignConfig中就无法获取到请求头了。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638885413196-dda7defb-ddb8-4324-9458-184cb6d89119.webp#clientId=ua28c9a60-f0c9-4&from=paste&id=u5473c9d0&originHeight=398&originWidth=750&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1fa0afb1-6f4d-4e15-b22d-840ff20a2e7&title=)<br />如图所示，红色方框圈起来的地方都不为null，但是最终获取到的Header是一个空的Map。<br />所以猜测是因为主线程退出触发了JVM的回收机制。<br />那么此时的情况将是 主线程已经退出，子线程没有执行完<br />所以显然，此时不能通过这种方式传递Header了。
<a name="arLqQ"></a>
### 异步方法一：线程私有变量ThreadLocal。
既然无法直接通过获取`HttpServletRequest`来获取Header，那么可以稍微改造一下，在原来的基础上添加一个拦截器。<br />所有的请求过来的时候，在拦截器中将`Header`先取出来，然后设置到本线程私有的Map中。<br />原来的`apply`方法在提交请求的时候再通过`ThreadLocal`提供的`remove`方法，清除掉。<br />只要把对该`Map`的操作封装一个工具类，工具类中实现get/set方法即可。<br />其实这种方式就是换了一个地方保存请求头，因此实用性与便捷性都还可以。
<a name="e4NLt"></a>
### 异步方法二：通过传参
该方式是在方法执行前，先将需要的参数取出来，比如需要一个token 就在Header中取出token，需要一个Content-Type就取出Content-Type。<br />然后将取出来的值作为参数传递到待执行的方法中。<br />该方法所调用的Feign接口需要做一个改造，在参数中添加带有`@RequestHeader`的注解，该注解表示将变量放在请求头，而不是请求的参数或者请求体里面。<br />如
```java
@FeignClient(name = "testClient", configuration = FeignClientsConfigurationCustom.class)
public interface testServer{

    @GetMapping("/test")
    String test(@RequestParam String param,@RequestHeader String token);
}
```
这种方法对于原来代码的改动较小，如果异步的场景比较少的话可以选择这种方法。<br />但是如果项目中用到了较多的异步方法，那么就需要异步方法一里面的拦截器 + ThreadLocal + RequestInterceptor了。
