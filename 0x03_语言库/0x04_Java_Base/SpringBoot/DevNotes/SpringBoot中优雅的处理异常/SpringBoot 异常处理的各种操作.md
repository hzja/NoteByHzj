JavaSpringBoot
<a name="g2wcp"></a>
## 1、前言
统一的异常处理对于应用的重要性不言而喻。来介绍一下 **Spring** 如何来进行统一的 **Rest** 异常处理。同时也会简单比较一下它们之间的优劣。
<a name="WJ7QI"></a>
## 2、`@Controller` 结合 `@ExceptionHandler`
在控制器中声明一个方法然后用 `@ExceptionHandler` 注解标记即可：
```java
@Controller
@RequestMapping("/test")
public class TestController {

    @RequestMapping("/err")
    @ResponseBody
    public Object demo1(){
        int i = 1 / 0;
        return new Date();
    }

    @ExceptionHandler({RuntimeException.class})
    public ModelAndView fix(Exception ex){
        System.out.println(ex.getMessage());
        return new ModelAndView("error",new ModelMap("ex",ex.getMessage()));
    }
}
```
优点：

- 优先级最高。
- `@ExceptionHandler` 标记的方法返回值类型支持多种。可以是视图，也可以是 json 等。

缺点：

- 一个 `Controller` 中的 `@ExceptionHandler` 注解上的异常类型不能出现相同的，否则运行时抛异常。
- 需要显式的声明处理的异常类型。
- 作用域仅仅是该 Controller  并不是真正意义上的全局异常。如果要想作用于全局需要将其放入所有控制器的父类中。
<a name="giHbh"></a>
## 3、`@ControllerAdvice` 结合 `@ExceptionHandler`
这是 **2.** 的改进型，通过定义 `@ControllerAdvice` 类并在方法上标记 `@ExceptionHandler` ，达到了全局异常处理的目的：
```java
@ControllerAdvice
public class TestController {


    @ExceptionHandler({RuntimeException.class})
    public ModelAndView fix(Exception ex){
        System.out.println(ex.getMessage());
        return new ModelAndView("error",new ModelMap("ex",ex.getMessage()));
    }
}
```
优点：

- 全局的异常处理。
- 完全控制响应的主体以及状态码
- 将多个异常映射到同一方法，以一起处理，并且它充分利用了更新的 **Restful ResponseEntity** 响应

缺点：

- 一个 Controller 中的 `@ExceptionHandler` 注解上的异常类型不能出现相同的，否则运行时抛异常。
- 需要显式的声明处理的异常类型。

一般情况下也建议使用该方式进行异常处理。大多数情况下都是兼容的。
<a name="emfrp"></a>
## 4、`HandlerExceptionResolver` 接口
实现 `HandlerExceptionResolver` 接口，这里继承其抽象实现 `AbstractHandlerExceptionResolver` :
```java
@Component
public class RestResponseStatusExceptionResolver extends AbstractHandlerExceptionResolver {

    @Override
    protected ModelAndView doResolveException(
        HttpServletRequest request,
        HttpServletResponse response,
        Object handler,
        Exception ex){
        try {
            if (ex instanceof IllegalArgumentException) {
                return handleIllegalArgument((IllegalArgumentException) ex, response, handler);
            }
            //todo more exception
        } catch (Exception handlerException) {
            //todo
        }
        return null;
    }

    private ModelAndView
    handleIllegalArgument(IllegalArgumentException ex, HttpServletResponse response)
    throws IOException {
        response.sendError(HttpServletResponse.SC_CONFLICT);
        String accept = request.getHeader(HttpHeaders.ACCEPT);
        //todo  more  response
        return new ModelAndView();
    }
}
```
优点：

- 这是一个全局的异常处理器。
- 这种方式全局异常处理返回JSP、velocity 等模板视图比较方便。
- 支持多种格式的响应，虽然覆写的方法返回的是 ModelAndView 但是因为参数中有 `HttpServletResponse`， 可以利用它来进行定制响应结果。例如，如果客户端要求输入application/json，那么在出现错误情况时，要确保返回一个以application/json编码的响应。

缺点：

- 需要与低级的 `HttpServletResponse` 交互才能实现各种形式的响应体。
- 优先级比较低
<a name="GCMMP"></a>
## 5、Spring Boot 中的异常处理
如果用的框架是 **Spring Boot** 。 还可以用它独特的处理方式。优点是屏蔽了低级的API，缺点也比较明显，无法捕捉到具体的异常。
<a name="K6dQ4"></a>
### 5.1 实现 ErrorController
**Spring Boot** 在默认情况下，提供了 /error 映射来处理所有错误，在 **Servlet** 容器里注册了全局的错误页面(**Whitelabel Error Page**)并返回客户端。通过实现 **ErrorController** 接口并注册为 **Bean**。这里不再举例。可参考 `BasicErrorController `。
<a name="HWlOM"></a>
### 5.2 添加 ErrorAttributes
也可以添加 **ErrorAttributes** 类型的 **Bean** 来替换替换默认的异常处理。
```java
@Component
public class MyCustomErrorAttributes extends DefaultErrorAttributes {

    @Override
    public Map<String, Object> getErrorAttributes(
        WebRequest webRequest, boolean includeStackTrace){
        Map<String, Object> errorAttributes =
            super.getErrorAttributes(webRequest, includeStackTrace);
        errorAttributes.put("locale", webRequest.getLocale()
                            .toString());
        errorAttributes.remove("error");

        //todo your business

        return errorAttributes;
    }
}
```
<a name="oJlQP"></a>
### 5.3 继承基类 `BasicErrorController`
**Spring Boot** 自动配置还提供了实现 **ErrorController** 接口异常处理的基类 `**BasicErrorController**`，默认是处理 **text/html**类型请求的错误，可以继承该基类自定义处理更多的请求类型，添加公共方法并使用 `**@RequestMapping**` 注解的 **produce**属性指定处理类型。
```java
@Component
public class MyErrorController extends BasicErrorController {

    public MyErrorController(ErrorAttributes errorAttributes) {
        super(errorAttributes, new ErrorProperties());
    }

    @RequestMapping(produces = MediaType.APPLICATION_XML_VALUE)
    public ResponseEntity<Map<String, Object>> xmlError(HttpServletRequest request) {

        //todo your business

    }
}
```
<a name="gweRh"></a>
## 6、Spring 5 的 `ResponseStatusException`
另外在最新的 **Spring 5** 中还可以通过 抛出 `ResponseStatusException` 异常来进行处理。<br />好处：

- 使用比较方便
- 一种类型，多种状态代码：一种异常类型可以导致多种不同的响应。与`@ExceptionHandler`相比，这减少了紧密耦合
- 不必创建那么多的自定义异常类
- 由于可以通过编程方式创建异常，因此可以更好地控制异常处理

缺点：

- 没有统一的异常处理方式，强制执行某些应用程序范围的约定更加困难
- 可能会有大量的重复代码。
