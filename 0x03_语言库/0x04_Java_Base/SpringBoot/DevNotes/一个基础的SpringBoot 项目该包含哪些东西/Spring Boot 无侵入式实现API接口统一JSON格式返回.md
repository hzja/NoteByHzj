SpringBoot JSON API
<a name="sLO3U"></a>
## 无侵入式统一返回JSON格式
<a name="ht7Eh"></a>
### 定义JSON格式
后端返回给前端一般情况下使用JSON格式，定义如下
```json
{
    "code": 200,
    "message": "OK",
    "data": {
    }
}
```

- `code`：返回状态码
- `message`：返回信息的描述
- `data`：返回值
<a name="cL6JL"></a>
### 定义JavaBean字段
<a name="aBKTV"></a>
#### 定义状态码枚举类
```java
@ToString
@Getter
public enum ResultStatus {
    SUCCESS(HttpStatus.OK, 200, "OK"),
    BAD_REQUEST(HttpStatus.BAD_REQUEST, 400, "Bad Request"),
    INTERNAL_SERVER_ERROR(HttpStatus.INTERNAL_SERVER_ERROR, 500, "Internal Server Error"),;
    /** 返回的HTTP状态码,  符合http请求 */
    private HttpStatus httpStatus;
    /** 业务异常码 */
    private Integer code;
    /** 业务异常信息描述 */
    private String message;
    ResultStatus(HttpStatus httpStatus, Integer code, String message) {
        this.httpStatus = httpStatus;
        this.code = code;
        this.message = message;
    }
}
```
状态码和信息以及http状态码就能一一对应了便于维护。
```java
@Getter
@ToString
public class Result<T> {
    /** 业务错误码 */
    private Integer code;
    /** 信息描述 */
    private String message;
    /** 返回参数 */
    private T data;
    private Result(ResultStatus resultStatus, T data) {
        this.code = resultStatus.getCode();
        this.message = resultStatus.getMessage();
        this.data = data;
    }
    /** 业务成功返回业务代码和描述信息 */
    public static Result<Void> success() {
        return new Result<Void>(ResultStatus.SUCCESS, null);
    }
    /** 业务成功返回业务代码,描述和返回的参数 */
    public static <T> Result<T> success(T data) {
        return new Result<T>(ResultStatus.SUCCESS, data);
    }
    /** 业务成功返回业务代码,描述和返回的参数 */
    public static <T> Result<T> success(ResultStatus resultStatus, T data) {
        if (resultStatus == null) {
            return success(data);
        }
        return new Result<T>(resultStatus, data);
    }
    /** 业务异常返回业务代码和描述信息 */
    public static <T> Result<T> failure() {
        return new Result<T>(ResultStatus.INTERNAL_SERVER_ERROR, null);
    }
    /** 业务异常返回业务代码,描述和返回的参数 */
    public static <T> Result<T> failure(ResultStatus resultStatus) {
        return failure(resultStatus, null);
    }
    /** 业务异常返回业务代码,描述和返回的参数 */
    public static <T> Result<T> failure(ResultStatus resultStatus, T data) {
        if (resultStatus == null) {
            return new Result<T>(ResultStatus.INTERNAL_SERVER_ERROR, null);
        }
        return new Result<T>(resultStatus, data);
    }
}
```
因为使用构造方法进行创建对象太麻烦了，使用静态方法来创建对象这样简单明了
<a name="h7UGN"></a>
### `Result`实体返回测试
```java
@RestController
@RequestMapping("/hello")
public class HelloController {
    private static final HashMap<String, Object> INFO;
    static {
        INFO = new HashMap<>();
        INFO.put("name", "galaxy");
        INFO.put("age", "70");
    }
    @GetMapping("/hello")
    public Map<String, Object> hello() {
        return INFO;
    }
    @GetMapping("/result")
    @ResponseBody
    public Result<Map<String, Object>> helloResult() {
        return Result.success(INFO);
    }
}
```
到这里已经简单的实现了统一JSON格式了，但是发现了一个问题了，想要返回统一的JSON格式需要返回`Result<Object>`才可以，明明返回Object可以了，为什么要重复劳动，有没有解决方法，下面开始优化代码
<a name="4Mbk0"></a>
## 统一返回JSON格式进阶-全局处理(`@RestControllerAdvice`)
使用`@ResponseBody`注解会把返回`Object`序列化成JSON字符串，在序列化前把`Object`赋值给`Result<Object>`就可以了，可以参考`org.springframework.web.servlet.mvc.method.annotation.ResponseBodyAdvice`和`org.springframework.web.bind.annotation.ResponseBody`
<a name="FgK88"></a>
### `@ResponseBody`继承类
从`@ResponseBody`注解入手了就创建一个注解类继承`@ResponseBody`，`@ResponseResultBody` 可以标记在类和方法上这样就可以跟自由的进行使用了
```java
@Retention(RetentionPolicy.RUNTIME)
@Target({ElementType.TYPE, ElementType.METHOD})
@Documented
@ResponseBody
public @interface ResponseResultBody {
}
```
<a name="25loD"></a>
### `ResponseBodyAdvice`继承类
```java
@RestControllerAdvice
public class ResponseResultBodyAdvice implements ResponseBodyAdvice<Object> {
    private static final Class<? extends Annotation> ANNOTATION_TYPE = ResponseResultBody.class;
    /**
     * 判断类或者方法是否使用了 @ResponseResultBody
     */
    @Override
    public boolean supports(MethodParameter returnType, Class<? extends HttpMessageConverter<?>> converterType) {
        return AnnotatedElementUtils.hasAnnotation(returnType.getContainingClass(), ANNOTATION_TYPE) || returnType.hasMethodAnnotation(ANNOTATION_TYPE);
    }
    /**
     * 当类或者方法使用了 @ResponseResultBody 就会调用这个方法
     */
    @Override
    public Object beforeBodyWrite(Object body, MethodParameter returnType, MediaType selectedContentType, Class<? extends HttpMessageConverter<?>> selectedConverterType, ServerHttpRequest request, ServerHttpResponse response) {
        // 防止重复包裹的问题出现
        if (body instanceof Result) {
            return body;
        }
        return Result.success(body);
    }
}
```
<a name="n2vOC"></a>
### `RestControllerAdvice`返回测试
```java
@RestController
@RequestMapping("/helloResult")
@ResponseResultBody
public class HelloResultController {
    private static final HashMap<String, Object> INFO;
    static {
        INFO = new HashMap<String, Object>();
        INFO.put("name", "galaxy");
        INFO.put("age", "70");
    }
    @GetMapping("hello")
    public HashMap<String, Object> hello() {
        return INFO;
    }
    /** 测试重复包裹 */
    @GetMapping("result")
    public Result<Map<String, Object>> helloResult() {
        return Result.success(INFO);
    }
    @GetMapping("helloError")
    public HashMap<String, Object> helloError() throws Exception {
        throw new Exception("helloError");
    }
    @GetMapping("helloMyError")
    public HashMap<String, Object> helloMyError() throws Exception {
        throw new ResultException();
    }
}
```
直接返回`Object`就可以统一`JSON`格式了，就不用每个返回都返回`Result<T>`对象了，直接让SpringMVC进行统一的管理。
<a name="KD3Gm"></a>
## 统一返回JSON格式进阶-异常处理(`@ExceptionHandler`)
```java
@Configuration
public class MyExceptionHandler implements HandlerExceptionResolver {
    public ModelAndView resolveException(HttpServletRequest request, HttpServletResponse response,
                                         Object handler, Exception ex) {
        PrintWriter out = getPrintWrite(response);
        if (ex instanceof XXXException) {
            out.write(JsonUtil.formatJson(ResultEnum.PAY_ERROR.getCode(), ex.getMessage()));
        } else {
            out.write(JsonUtil.formatJson(ResultEnum.FAIL.getCode(), "服务器异常"));
        }
        if (null != out) {
            out.close();
        }
        return mav;
    }
    private PrintWriter getPrintWrite(HttpServletResponse response) {
        PrintWriter out = null;
        try {
            response.setHeader("Content-type", "text/html;charset=UTF-8");
            response.setCharacterEncoding("UTF-8");
            out = response.getWriter();
        } catch (IOException e) {
            log.error("PrintWriter is exception", e);
        }
        return out;
    }
}
```
上面的代码看看还是没有问题的，异常处理`@ResponseStatus`(不推荐)，`@ResponseStatus`用法如下，可用在`Controller`类和`Controller`方法上以及`Exception`类上但是这样的工作量还是挺大的。
```java
@RestController
@RequestMapping("/error")
@ResponseStatus(value = HttpStatus.INTERNAL_SERVER_ERROR, reason = "Java的异常")
public class HelloExceptionController {
    private static final HashMap<String, Object> INFO;
    static {
        INFO = new HashMap<String, Object>();
        INFO.put("name", "galaxy");
        INFO.put("age", "70");
    }
    @GetMapping()
    public HashMap<String, Object> helloError() throws Exception {
        throw new Exception("helloError");
    }
    @GetMapping("helloJavaError")
    @ResponseStatus(value = HttpStatus.INTERNAL_SERVER_ERROR, reason = "Java的异常")
    public HashMap<String, Object> helloJavaError() throws Exception {
        throw new Exception("helloError");
    }
    @GetMapping("helloMyError")
    public HashMap<String, Object> helloMyError() throws Exception {
        throw new MyException();
    }
}
@ResponseStatus(value = HttpStatus.INTERNAL_SERVER_ERROR, reason = "自己定义的异常")
class MyException extends Exception {
}
```
<a name="ecP4i"></a>
### 全局异常处理`@ExceptionHandler`(推荐)
把`ResponseResultBodyAdvice`类进行改造一下，主要参考`org.springframework.web.servlet.mvc.method.annotation.ResponseEntityExceptionHandler#handleException()`方法，可以参考一下
```java
@Slf4j
@RestControllerAdvice
public class ResponseResultBodyAdvice implements ResponseBodyAdvice<Object> {
    private static final Class<? extends Annotation> ANNOTATION_TYPE = ResponseResultBody.class;
    /** 判断类或者方法是否使用了 @ResponseResultBody */
    @Override
    public boolean supports(MethodParameter returnType, Class<? extends HttpMessageConverter<?>> converterType) {
        return AnnotatedElementUtils.hasAnnotation(returnType.getContainingClass(), ANNOTATION_TYPE) || returnType.hasMethodAnnotation(ANNOTATION_TYPE);
    }
    /** 当类或者方法使用了 @ResponseResultBody 就会调用这个方法 */
    @Override
    public Object beforeBodyWrite(Object body, MethodParameter returnType, MediaType selectedContentType, Class<? extends HttpMessageConverter<?>> selectedConverterType, ServerHttpRequest request, ServerHttpResponse response) {
        if (body instanceof Result) {
            return body;
        }
        return Result.success(body);
    }
    /**
     * 提供对标准Spring MVC异常的处理
     *
     * @param ex      the target exception
     * @param request the current request
     */
    @ExceptionHandler(Exception.class)
    public final ResponseEntity<Result<?>> exceptionHandler(Exception ex, WebRequest request) {
        log.error("ExceptionHandler: {}", ex.getMessage());
        HttpHeaders headers = new HttpHeaders();
        if (ex instanceof ResultException) {
            return this.handleResultException((ResultException) ex, headers, request);
        }
        // TODO: 2019/10/05 galaxy 这里可以自定义其他的异常拦截
        return this.handleException(ex, headers, request);
    }
    /** 对ResultException类返回返回结果的处理 */
    protected ResponseEntity<Result<?>> handleResultException(ResultException ex, HttpHeaders headers, WebRequest request) {
        Result<?> body = Result.failure(ex.getResultStatus());
        HttpStatus status = ex.getResultStatus().getHttpStatus();
        return this.handleExceptionInternal(ex, body, headers, status, request);
    }
    /** 异常类的统一处理 */
    protected ResponseEntity<Result<?>> handleException(Exception ex, HttpHeaders headers, WebRequest request) {
        Result<?> body = Result.failure();
        HttpStatus status = HttpStatus.INTERNAL_SERVER_ERROR;
        return this.handleExceptionInternal(ex, body, headers, status, request);
    }
    /**
     * org.springframework.web.servlet.mvc.method.annotation.ResponseEntityExceptionHandler#handleExceptionInternal(java.lang.Exception, java.lang.Object, org.springframework.http.HttpHeaders, org.springframework.http.HttpStatus, org.springframework.web.context.request.WebRequest)
     * <p>
     * A single place to customize the response body of all exception types.
     * <p>The default implementation sets the {@link WebUtils#ERROR_EXCEPTION_ATTRIBUTE}
     * request attribute and creates a {@link ResponseEntity} from the given
     * body, headers, and status.
     */
    protected ResponseEntity<Result<?>> handleExceptionInternal(
            Exception ex, Result<?> body, HttpHeaders headers, HttpStatus status, WebRequest request) {
        if (HttpStatus.INTERNAL_SERVER_ERROR.equals(status)) {
            request.setAttribute(WebUtils.ERROR_EXCEPTION_ATTRIBUTE, ex, WebRequest.SCOPE_REQUEST);
        }
        return new ResponseEntity<>(body, headers, status);
    }
}
```
