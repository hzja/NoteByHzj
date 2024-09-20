JavaSpringBoot
<a name="UZocO"></a>
## 目录

- 异常处理方案分类
   - 基于请求转发
   - 基于异常处理器
   - 基于过滤器
- 常见异常处理实现方案
   1. `BasicExceptionController`
   2. `@ExceptionHandler`
   3. `@ControllerAdvice`+`@ExceptionHandler`
   4. `SimpleMappingExceptionResolver`
   5. `HandlerExceptionResolver`
   6. `Filter`
- 全局异常处理实现方案
   1. 请求转发
   2. 异常处理器+请求转发补充
   3. 过滤器
   4. 异常处理器+过滤器补充
- 注意事项
- 方案推荐
<a name="u2vy9"></a>
## 异常处理方案分类
异常处理主要分为三类：

- 基于请求转发的方式处理异常；
- 基于异常处理器的方式处理异常；
- 基于过滤器的方式处理异常。
<a name="eBc1g"></a>
### 基于请求转发
基于请求转发的异常处理方式是真正的全局异常处理。<br />实现方式有：

- `BasicExceptionController`
<a name="j26s4"></a>
### 基于异常处理器
基于异常处理器的异常处理方式其实并不是真正的全局异常处理，因为它处理不了过滤器等抛出的异常。<br />实现方式有：

- `@ExceptionHandler`
- `@ControllerAdvice`+`@ExceptionHandler`
- `SimpleMappingExceptionResolver`
- `HandlerExceptionResolver`
<a name="yxywI"></a>
### 基于过滤器
基于过滤器的异常处理方式近似与全局异常处理。它能处理过滤器及之后的环节抛出的异常。<br />实现方式有：

- Filter
<a name="Fstyl"></a>
## 常见异常处理实现方案
<a name="pBQWK"></a>
### 1、`BasicExceptionController`
这是SpringBoot默认处理异常方式：一旦程序中出现了异常SpringBoot就会请求/error的url，在SpringBoot中提供了一个叫`BasicExceptionController`的类来处理/error请求，然后跳转到默认显示异常的页面来展示异常信息。显示异常的页面也可以自定义，在目录src/main/resources/templates/下定义一个叫error的文件，可以是jsp也可以是html 。<br />此种方式是通过请求转发实现的，出现异常时，会转发到请求到/error，该接口对异常进行处理返回。是最符合全局异常处理的。<br />可以自定义Controller继承`BasicErrorController`异常处理来实现异常处理的自定义。
```java
@Slf4j
@RestController
public class MyErrorController extends BasicErrorController {

    public MyErrorController() {
        super(new DefaultErrorAttributes(), new ErrorProperties());
    }

    /**
     * produces 设置返回的数据类型：application/json
     * @param request 请求
     * @return 自定义的返回实体类
     */
    @Override
    @RequestMapping(value = "", produces = {MediaType.APPLICATION_JSON_VALUE})
    public ResponseEntity<Map<String, Object>> error(HttpServletRequest request) {
        // 获取错误信息
        Map<String, Object> body = getErrorAttributes(request, isIncludeStackTrace(request, MediaType.ALL));
        HttpStatus status = getStatus(request);
        String code = body.get("status").toString();
        String message = body.get("message").toString();
        return new ResponseEntity(ApiUtil.fail(message), HttpStatus.OK);
    }
}
```
**需要注意**

1. 该种方式能获取到的信息时有限的。一般情况只能获取到下面这几个参数（特殊情况会有补充参数）。

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1692938257733-bf55bceb-874b-42f1-a5d5-a7975a37110b.png#averageHue=%233c4042&clientId=ubc32167b-28e2-4&from=paste&id=u6efbc3ca&originHeight=151&originWidth=537&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub37da040-1aed-4245-817c-0e49b0340cc&title=)<br />现在一般项目需要的响应信息都是自定义统一格式的JSON（code、msg、data）。对于自定义业务错误码code不好得到，对于错误信息msg有时得到的也不一定是你所想要的（简单说就是一些特殊的异常描述信息不好得到）。<br />比如：自定义的参数校验信息
```java
@NotNull(message = "主键不能为空")
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1692938257736-2fd0a480-3880-41b2-a6f3-2cc22f9ff982.png#averageHue=%233c4042&clientId=ubc32167b-28e2-4&from=paste&id=u00f9d9e8&originHeight=186&originWidth=566&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue379a332-88e3-49ff-ad26-59adc7cf934&title=)<br />message参数取到的并不是“主键不能为空”。

2. 当出现抛出两次异常，第一次被异常处理器处理，第二次异常转由`BasicExceptionController`处理。但能取到的异常信息可能是一次的，具体原因下面有分析。
<a name="qQkPG"></a>
### 2、`@ExceptionHandler`
该种方式只能作用于使用`@ExceptionHandler`注解的Controller的异常，对于其他Controller的异常就无能为力了，所以并不不推荐使用。<br />此种方式是通过异常处理器实现的，使用`HandlerExceptionResolverComposite`异常处理器中的`ExceptionHandlerExceptionResolver`异常处理器处理的。
```java
@RestController
public class TestController {

    @GetMapping("test9")
    public FundInfo test9() throws Exception {
        throw new Exception("test9 error");
    }

    @GetMapping("test10")
    public FundInfo test10() throws Exception {
        throw new IOException("test10 error");
    }

    @ExceptionHandler(Exception.class)
    public ApiResult exceptionHandler(Exception e) {
        return ApiUtil.custom(500, e.getMessage());
    }
}
```
注意：如果既在具体Controller使用了`@ExceptionHandler`，也定义了全局异常处理器类（`@ControllerAdvice`+`@ExceptionHandler`），优先使用Controller定义的`@ExceptionHandler`处理。如果处理不了，才会使用全局异常处理器处理。
<a name="fusvf"></a>
### 3、`@ControllerAdvice`+`@ExceptionHandler`
使用 `@ControllerAdvice`+`@ExceptionHandler`注解能够进行近似全局异常处理，这种方式推荐使用。<br />一般说它只能处理控制器中抛出的异常，这种说法并不准确，其实它能处理`DispatcherServlet.doDispatch`方法中`DispatcherServlet.processDispatchResult`方法之前捕捉到的所有异常，包括：拦截器、参数绑定（参数解析、参数转换、参数校验）、控制器、返回值处理等模块抛出的异常。<br />此种方式是通过异常处理器实现的，使用`HandlerExceptionResolverComposite`异常处理器中的`ExceptionHandlerExceptionResolver`异常处理器处理的。
```java
protected void doDispatch(HttpServletRequest request, HttpServletResponse response) throws Exception {
    ......省略代码......
    try {
        ModelAndView mv = null;
        Exception dispatchException = null;
    
        try {
            ......省略代码......
            mappedHandler = getHandler(processedRequest);
            ......省略代码......
            HandlerAdapter ha = getHandlerAdapter(mappedHandler.getHandler());
            ......省略代码......
            if (!mappedHandler.applyPreHandle(processedRequest, response)) {
                return;
            }
                ......省略代码......
            mv = ha.handle(processedRequest, response, mappedHandler.getHandler());
            if (asyncManager.isConcurrentHandlingStarted()) {
                return;
            }
                ......省略代码......
            mappedHandler.applyPostHandle(processedRequest, response, mv);
        }
        catch (Exception ex) {
            dispatchException = ex;
        }
        catch (Throwable err) {
            dispatchException = new NestedServletException("Handler dispatch failed", err);
        }
        processDispatchResult(processedRequest, response, mappedHandler, mv, dispatchException);
    }
    catch (Exception ex) {
        ......省略代码......
    }
    catch (Throwable err) {
        ......省略代码......
    }
    finally {
        ......省略代码......
    }
}
```
**使用方式**<br />定义一个类，使用`@ControllerAdvice`注解该类，使用`@ExceptionHandler`注解方法。`@RestControllerAdvice`注解是`@ControllerAdvice`注解的扩展（`@RestControllerAdvice`=`@ControllerAdvice`+`@ResponseBody`），返回值自动为JSON的形式。
```java
/**
 * 全局异常处理器
 */
@Slf4j
@SuppressWarnings("ALL")
@RestControllerAdvice
public class MyGlobalExceptionHandler {

    @ExceptionHandler(BindException.class)
    @ResponseStatus(HttpStatus.OK)
    public ApiResult bindException(HttpServletRequest request,
                                   HttpServletResponse response,
                                   BindException exception) {
        return ApiUtil.fail(exception.getBindingResult().getFieldError().getDefaultMessage());
    }

    @ExceptionHandler(org.springframework.web.bind.MethodArgumentNotValidException.class)
    @ResponseStatus(HttpStatus.OK)
    public ApiResult methodArgumentNotValidException(HttpServletRequest request,
                                                     HttpServletResponse response,
                                                     MethodArgumentNotValidException exception) {
        return ApiUtil.fail(exception.getBindingResult().getFieldError().getDefaultMessage());
    }

    @ExceptionHandler(MissingServletRequestParameterException.class)
    @ResponseStatus(HttpStatus.OK)
    public ApiResult methodArgumentNotValidException(HttpServletRequest request,
                                                     HttpServletResponse response,
                                                     MissingServletRequestParameterException exception) {
        return ApiUtil.fail(exception.getMessage());
    }

    @ExceptionHandler(ConstraintViolationException.class)
    @ResponseStatus(HttpStatus.OK)
    public ApiResult methodArgumentNotValidException(HttpServletRequest request,
                                                     HttpServletResponse response,
                                                     ConstraintViolationException exception) {
        System.out.println(exception.getLocalizedMessage());
        Iterator<ConstraintViolation<?>> iterator = exception.getConstraintViolations().iterator();
        if (iterator.hasNext()) {
            ConstraintViolationImpl next = (ConstraintViolationImpl)iterator.next();
            return ApiUtil.fail(next.getMessage());
        }
        return ApiUtil.fail(exception.getMessage());
    }

    @ExceptionHandler(Exception.class)
    @ResponseStatus(HttpStatus.OK)
    public ApiResult exception(HttpServletRequest request,
                               HttpServletResponse response,
                               Exception exception) {
        return ApiUtil.fail(exception.getMessage());
    }
}
```
`@ResponseStatus`注解<br />作用：指定http状态码，正确执行时返回该状态码，但方法执行报错时，该返回啥状态码就是啥状态码，指定的状态码无效。
<a name="xmWai"></a>
### 4、`SimpleMappingExceptionResolver`
使用简单映射异常处理器处理异常，通过配置`SimpleMappingExceptionResolver`类也是进行近似全局异常处理，但该种方式不能得到具体的异常信息，且返回的是视图，不推荐使用。<br />此种方式是通过异常处理器实现的，使用`SimpleMappingExceptionResolver`异常处理器处理的。
```java
@Configuration
public class GlobalExceptionConfig {

    @Bean
    public SimpleMappingExceptionResolver getSimpleMappingExceptionResolver(){
        SimpleMappingExceptionResolver resolver = new SimpleMappingExceptionResolver();
        /**
        * 参数一：异常的类型，这里必须要异常类型的全名
        * 参数二：要跳转的视图名称
        */
        Properties mappings = new Properties();
        mappings.put("java.lang.ArithmeticException", "error1");
        mappings.put("java.lang.NullPointerException", "error1");
        mappings.put("java.lang.Exception", "error1");
        mappings.put("java.io.IOException", "error1");
        // 设置异常与视图的映射信息
        resolver.setExceptionMappings(mappings);
        return resolver;
    }
}
```
<a name="xmTtR"></a>
### 5、`HandlerExceptionResolver`
实现`HandlerExceptionResolver`接口来处理异常，该种方式是近似全局异常处理。<br />此种方式是通过异常处理器实现的，使用自定义的异常处理器（实现`HandlerExceptionResolver`接口）处理的。
```java
public class MyExceptionResolver extends AbstractHandlerExceptionResolver {

    /**
     * 异常解析器的顺序， 数值越小，表示优先级越高
     * @return
     */
    @Override
    public int getOrder() {
        return -999999;
    }

    @Override
    protected ModelAndView doResolveException(HttpServletRequest request, HttpServletResponse response, Object handler, Exception ex) {
        response.setContentType(MediaType.APPLICATION_JSON_VALUE);
        try {
            response.getWriter().write(JSON.toJSONString(ApiUtil.fail(ex.getMessage())));
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }
}
```
`AbstractHandlerExceptionResolver`类实现了`HandlerExceptionResolver`接口。
<a name="NIJyd"></a>
### 6、Filter
基于过滤器的异常处理方式，比异常处理器处理的范围要大一些（能处理到Filter过滤器抛出的异常），更近似全局异常处理。使用自定义过滤器进行异常处理时，该过滤器应该放到过滤链的第一个位置，这样才能保证能处理到后续过滤器抛出的异常。
```java
@Bean
ExceptionFilter exceptionFilter() {
    return new ExceptionFilter();
}

@Bean
public FilterRegistrationBean exceptionFilterRegistration() {
    FilterRegistrationBean registration = new FilterRegistrationBean();
    registration.setFilter(exceptionFilter());
    registration.setName("exceptionFilter");
    //此处尽量小，要比其他Filter靠前
    registration.setOrder(-1);
    return registration;
}
```
```java
/**
 * 自定义异常过滤器
 * 用于处理Controller外抛出的异常（如Filter抛出的异常）
 */
@Slf4j
public class ExceptionFilter extends OncePerRequestFilter {

    @Override
    protected void doFilterInternal(HttpServletRequest httpServletRequest, HttpServletResponse httpServletResponse, FilterChain filterChain) throws ServletException,IOException {
        try {
            filterChain.doFilter(httpServletRequest, httpServletResponse);
        } catch (IOException e) {
            httpServletResponse.getWriter().write(JSON.toJSONString(ApiUtil.fail(e.getMessage())));
        }
    }
}
```
上面的写法其实还是有一定问题的，如果进入了catch就会重复写入httpServletResponse，可能会导致产生一些列的问题。<br />举例一个问题来说明，通过上面的写法，这样的对响应的写入一般是累加的，可能会导致返回的数据格式有问题，比如：当异常处理器处理了Controller抛出的异常，写入了响应，然后过滤器又抛出了异常，被ExceptionFilter给catch到，这就有一次处理了异常，写入了响应，最后的到的响应数据可能是这样的：
```json
{
  "code": 500,
  "msg": "Controller error"
}{
  "code": 505,
  "msg": "Filter error"
}
```
这个时候一般会使用代理类来再次封装Response，`filterChain.doFilter`传递的是封装后的代理类。<br />Response代理类
```java
/**
 * Response代理类
 */
public class ResponseWrapper extends HttpServletResponseWrapper {
    private ByteArrayOutputStream outputStream = new ByteArrayOutputStream();

    private PrintWriter printWriter = new PrintWriter(outputStream);

    public ResponseWrapper(HttpServletResponse response) {
        super(response);
    }

    @Override
    public ServletOutputStream getOutputStream() throws IOException {
        return new ServletOutputStream() {
            @Override
            public boolean isReady() {
                return false;
            }

            @Override
            public void setWriteListener(WriteListener writeListener) {

            }

            @Override
            public void write(int b) throws IOException {
                outputStream.write(b);
            }

            @Override
            public void write(byte[] b) throws IOException {
                outputStream.write(b);
            }

            @Override
            public void write(byte[] b, int off, int len) throws IOException {
                outputStream.write(b, off, len);
            }

            @Override
            public void flush() throws IOException {
                outputStream.flush();
            }
        };
    }

    @Override
    public PrintWriter getWriter() throws IOException {
        return printWriter;
    }

    public void flush(){
        try {
            printWriter.flush();
            printWriter.close();
            outputStream.flush();
            outputStream.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public byte[] getContent() {
        flush();
        return outputStream.toByteArray();
    }
}
```
自定义过滤器类修改为
```java
@Slf4j
public class ExceptionFilter extends OncePerRequestFilter {

    @Override
    protected void doFilterInternal(HttpServletRequest httpServletRequest, HttpServletResponse httpServletResponse, FilterChain filterChain) throws ServletException,IOException {
        try {
            // 封装Response，得到代理对象
            ResponseWrapper responseWrapper = new ResponseWrapper(httpServletResponse);
            // 使用代理对象
            filterChain.doFilter(httpServletRequest, responseWrapper);
            // 读取响应内容
            byte[] bytes = responseWrapper.getContent();
            // 这里可以对响应内容进行修改等操作

            // 模拟Filter抛出异常
            if (true) {
                throw new IOException("Filter error");
            }

            // 内容重新写入原响应对象中
            httpServletResponse.getOutputStream().write(bytes);

        } catch (Exception e) {
            httpServletResponse.setContentType(MediaType.APPLICATION_JSON_VALUE);
            httpServletResponse.getOutputStream().write(JSON.toJSONString(ApiUtil.fail(e.getMessage())).getBytes());
        }
    }
}
```
<a name="ZSR6A"></a>
## 全局异常处理实现方案
要想实现正在的全局异常处理，显然只通过异常处理器的方式处理是不够的，这种方案处理不了过滤器等抛出的异常。<br />全局异常处理的几种实现方案：

- 基于请求转发；
- 基于异常处理器+请求转发补充；
- 基于过滤器；
- 基于异常处理器+过滤器补充。
<a name="dbUw0"></a>
### 1、请求转发
该方案貌似不好获取到特殊的异常描述信息（没仔细研究），如参数校验中的message属性信息：
```java
@NotNull(message = "主键不能为空")
```
本方案通过自定义错误处理Controller继承`BasicExceptionController`来实现。
<a name="t4m3f"></a>
### 2、异常处理器+请求转发补充
**1）自定义异常处理**`**Controller**`**实现**`**BasicExceptionController**`<br />具体实现参考：常用异常处理实现方案1。<br />**2）异常处理器实现**<br />方式1：`@ControllerAdvice`+`@ExceptionHandler`（推荐使用）<br />具体实现参考：常用异常处理实现方案3。<br />方式2：`SimpleMappingExceptionResolver`<br />具体实现参考：常用异常处理实现方案4。<br />方式3：`HandlerExceptionResolver`<br />具体实现参考：常用异常处理实现方案5。
<a name="ypidm"></a>
### 3、过滤器
具体实现参考：常用异常处理实现方案6。
<a name="B0oIF"></a>
### 4、异常处理器+过滤器补充
创建自定义过滤器bean
```java
@Bean
ExceptionFilter exceptionFilter() {
    return new ExceptionFilter();
}

@Bean
public FilterRegistrationBean exceptionFilterRegistration() {
    FilterRegistrationBean registration = new FilterRegistrationBean();
    registration.setFilter(exceptionFilter());
    registration.setName("exceptionFilter");
    //此处尽量小，要比其他Filter靠前
    registration.setOrder(-1);
    return registration;
}
```
**方式1：**`**@ControllerAdvice**`**+**`**@ExceptionHandler**`**+Filter（推荐使用）**<br />`@ControllerAdvice`+`@ExceptionHandler`的实现参考：常用异常处理实现方案3。<br />Filter实现：

- 方式1：参考常用异常处理实现方案6。
- 方式2：借助异常处理器处理异常。
```java
@Slf4j
public class ExceptionFilter extends OncePerRequestFilter {

    /**
     * 遇到的坑，ExceptionFilter对象的创建没有交给Spring容器（直接new的），导致@Autowired注入不会生效
     */
    @Autowired
    private HandlerExceptionResolver handlerExceptionResolver;

    @Override
    protected void doFilterInternal(HttpServletRequest httpServletRequest, HttpServletResponse httpServletResponse, FilterChain filterChain) throws ServletException,IOException {
        try {
            // 封装Response，得到代理对象
            ResponseWrapper responseWrapper = new ResponseWrapper(httpServletResponse);
            // 使用代理对象
            filterChain.doFilter(httpServletRequest, responseWrapper);
            // 读取响应内容
            byte[] bytes = responseWrapper.getContent();
            // 这里可以对响应内容进行修改等操作

            // 模拟Filter抛出异常
            if (true) {
                throw new IOException("Filter error");
            }

            // 内容重新写入原响应对象中
            httpServletResponse.getOutputStream().write(bytes);

        } catch (Exception e) {
            handlerExceptionResolver.resolveException(httpServletRequest, httpServletResponse, null, e);
        }
    }
}
```
注入的`HandlerExceptionResolver`其实是`HandlerExceptionResolverComposite`异常处理器，最终是使用异常处理器中的`ExceptionHandlerExceptionResolver`异常处理器处理的。<br />**方式2：**`**HandlerExceptionResolver**`**+Filter**<br />`HandlerExceptionResolver`的实现参考：常用异常处理实现方案5。<br />Filter的实现：

- 方式1：参考常用异常处理实现方案6。
- 方式2：借助异常处理器处理异常。
```java
@Slf4j
public class ExceptionFilter extends OncePerRequestFilter {

    @Autowired
    private MyExceptionResolver myExceptionResolver;

    @Override
    protected void doFilterInternal(HttpServletRequest httpServletRequest, HttpServletResponse httpServletResponse, FilterChain filterChain) throws ServletException,IOException {
        try {
            // 封装Response，得到代理对象
            ResponseWrapper responseWrapper = new ResponseWrapper(httpServletResponse);
            // 使用代理对象
            filterChain.doFilter(httpServletRequest, responseWrapper);
            // 读取响应内容
            byte[] bytes = responseWrapper.getContent();
            // 这里可以对响应内容进行修改等操作

            // 模拟Filter抛出异常
            if (true) {
                throw new IOException("Filter error");
            }

            // 内容重新写入原响应对象中
            httpServletResponse.getOutputStream().write(bytes);

        } catch (Exception e) {
            myExceptionResolver.resolveException(httpServletRequest, httpServletResponse, null, e);
        }
    }
}
```
注入的`MyExceptionResolver`是自定义的异常处理器。
<a name="XG0Y1"></a>
#### 注意事项
2、4两种通过组合的方式进行异常处理需要考虑到的问题：对于一个请求，如果两个地方都捕捉到了异常，要考虑两次异常处对response响应信息的重复写入问题。<br />比如：异常处理器处理了控制器抛出的异常，写入响应；过滤器处理了过滤器抛出的异常，写入响应。这就会出现响应被写入了两次的问题或者第二次写入响应直接报错。<br />一些处理思路：考虑使用Response代理类。第一次处理时，异常处理器写入的响应信息是写入到Response代理对象的，并可以从Response代理类中得到写入的响应信息；第二次处理，过滤器等写入的响应写入到Response原对象中的。<br />过程中发现一个问题：通过`BasicExceptionController`+异常处理器处理异常的方式时。Controller抛出了异常，被异常处理器处理，返回的过程中，Filter又抛出了一个异常，被`BasicExceptionController`处理，但`BasicExceptionController`的到的异常信息却是Controller产生的异常信息，而不是Filter产生的异常信息。但是调到`BasicExceptionController`去处理异常又却是是因为Filter抛出异常产生的。<br />个人猜想：异常处理器在处理异常时，不仅是把响应内容部部分写入了Response，还把异常信息写入了Response。当因为异常跳转到`BasicExceptionController`进行处理，`BasicExceptionController`在获取异常信息时，会先从Response获取异常信息，获取不到才会从异常中获取异常信息。
<a name="QBogk"></a>
## 方案推荐
**「请求转发（推荐）。」**完全统一的全局异常处理，自定义异常处理Controller能达到自定义统一响应信息格式目的。<br />但是，现在一般项目需要的响应信息都是自定义统一格式的JSON（code、msg、data）。但对于自定义业务错误码code不好得到，对于错误信息msg有时得到的也不一定是你所想要的。<br />但感觉通过自定义的扩展是能得到业务状态码和特殊异常描述信息的（没详细研究）。<br />**「异常处理+请求转发补充（最推荐）。」** 推荐使用`@ControllerAdvice`+`@ExceptionHandler`+`BasicExceptionController`的方式。<br />异常处理器能自定义处理大多异常（包括特殊的异常），剩余处理不到的异常交给异常处理控制器处理。<br />**「过滤器（不推荐）。」** 异常处理全需要手写代码实现，自己的代码肯定不会太完美，可能有没考虑到的情况，容易出问题；且过滤器之前抛出的异常处理不到。<br />**「异常处理器+过滤器补充（不太推荐）。」** 推荐使用`@ControllerAdvice`+`@ExceptionHandler`+Filter（借助异常处理器处理异常）的方式，但过滤器之前抛出的异常处理不到。
