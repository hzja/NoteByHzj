SpringCloud 统一异常处理<br />在启动应用时会发现在控制台打印的日志中出现了两个路径为 {[/error]} 的访问地址，当系统中发送异常错误时，Spring Boot 会根据请求方式分别跳转到以 JSON 格式或以界面显示的 /error 地址中显示错误信息。
```
2018-12-18 09:36:24.627  INFO 19040 --- [           main] s.w.s.m.m.a.RequestMappingHandlerMapping : Mapped "{[/error]}" ...
2018-12-18 09:36:24.632  INFO 19040 --- [           main] s.w.s.m.m.a.RequestMappingHandlerMapping : Mapped "{[/error],produces=[text/html]}" ...
```
<a name="RZAsy"></a>
## 默认异常处理
<a name="GvvVH"></a>
### 使用 AJAX 方式请求时返回的 JSON 格式错误信息。
```json
{
    "timestamp": "2018-12-18T01:50:51.196+0000",
    "status": 404,
    "error": "Not Found",
    "message": "No handler found for GET /err404",
    "path": "/err404"
}
```
<a name="O9j2c"></a>
### 使用浏览器请求时返回的错误信息界面。
![2021-05-13-23-35-38-993132.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620920193419-ed478dda-19a0-430c-9f6d-849704b86e27.png#clientId=ubde4fee8-a980-4&from=ui&id=u06ebe6ed&originHeight=177&originWidth=691&originalType=binary&ratio=1&rotation=0&showTitle=false&size=14432&status=done&style=shadow&taskId=u76da7c92-592f-4ad0-acfd-307863c4b6b&title=)
<a name="pzuUa"></a>
## 自定义异常处理
<a name="LZqLP"></a>
### 引入依赖
```xml
<dependency>
    <groupId>com.alibaba</groupId>
    <artifactId>fastjson</artifactId>
    <version>1.2.54</version>
</dependency>
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-freemarker</artifactId>
</dependency>
```
fastjson 是 JSON 序列化依赖， spring-boot-starter-freemarker 是一个模板引擎，用于我们设置错误输出模板。
<a name="mDCHm"></a>
### 增加配置
properties
```
# 出现错误时, 直接抛出异常(便于异常统一处理，否则捕获不到404)
spring.mvc.throw-exception-if-no-handler-found=true
# 不要为工程中的资源文件建立映射
spring.resources.add-mappings=false
```
yml
```yaml
spring:
  # 出现错误时, 直接抛出异常(便于异常统一处理，否则捕获不到404)
  mvc:
    throw-exception-if-no-handler-found: true
  # 不要为工程中的资源文件建立映射
  resources:
    add-mappings: false
```
<a name="y4j2H"></a>
### 新建错误信息实体
```java
/**
 * 信息实体
 */
public class ExceptionEntity implements Serializable {

    private static final long serialVersionUID = 1L;

    private String message;

    private int    code;

    private String error;

    private String path;

    @JSONField(format = "yyyy-MM-dd hh:mm:ss")
    private Date timestamp = new Date();

    public static long getSerialVersionUID() {
        return serialVersionUID;
    }

    public String getMessage() {
        return message;
    }

    public void setMessage(String message) {
        this.message = message;
    }

    public int getCode() {
        return code;
    }

    public void setCode(int code) {
        this.code = code;
    }

    public String getError() {
        return error;
    }

    public void setError(String error) {
        this.error = error;
    }

    public String getPath() {
        return path;
    }

    public void setPath(String path) {
        this.path = path;
    }

    public Date getTimestamp() {
        return timestamp;
    }

    public void setTimestamp(Date timestamp) {
        this.timestamp = timestamp;
    }
}
```
<a name="vTIm1"></a>
### 新建自定义异常
```java
/**
 * 自定义异常
 */
public class BasicException extends RuntimeException {

    private static final long serialVersionUID = 1L;

    private int code = 0;

    public BasicException(int code, String message) {
        super(message);
        this.code = code;
    }

    public int getCode() {
        return this.code;
    }
}

/**
 * 业务异常
 */
public class BusinessException extends BasicException {

    private static final long serialVersionUID = 1L;

    public BusinessException(int code, String message) {
        super(code, message);
    }
}
```
BasicException 继承了 RuntimeException ，并在原有的 Message 基础上增加了错误码 code 的内容。而 BusinessException 则是在业务中具体使用的自定义异常类，起到了对不同的异常信息进行分类的作用。
<a name="OduEv"></a>
### 新建 error.ftl 模板文件
位置：/src/main/resources/templates/ 用于显示错误信息
```html
<!DOCTYPE html>
<html>
<head>
    <meta name="robots" content="noindex,nofollow" />
    <meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no">
    <style>
        h2{
            color: #4288ce;
            font-weight: 400;
            padding: 6px 0;
            margin: 6px 0 0;
            font-size: 18px;
            border-bottom: 1px solid #eee;
        }

        /* Exception Variables */
        .exception-var table{
            width: 100%;
            max-width: 500px;
            margin: 12px 0;
            box-sizing: border-box;
            table-layout:fixed;
            word-wrap:break-word;
        }
        .exception-var table caption{
            text-align: left;
            font-size: 16px;
            font-weight: bold;
            padding: 6px 0;
        }
        .exception-var table caption small{
            font-weight: 300;
            display: inline-block;
            margin-left: 10px;
            color: #ccc;
        }
        .exception-var table tbody{
            font-size: 13px;
            font-family: Consolas,"Liberation Mono",Courier,"微软雅黑";
        }
        .exception-var table td{
            padding: 0 6px;
            vertical-align: top;
            word-break: break-all;
        }
        .exception-var table td:first-child{
            width: 28%;
            font-weight: bold;
            white-space: nowrap;
        }
        .exception-var table td pre{
            margin: 0;
        }
    </style>
</head>
<body>

<div class="exception-var">
    <h2>Exception Datas</h2>
    <table>
        <tbody>
        <tr>
            <td>Code</td>
            <td>
                ${(exception.code)!}
            </td>
        </tr>
        <tr>
            <td>Time</td>
            <td>
                ${(exception.timestamp?datetime)!}
            </td>
        </tr>
        <tr>
            <td>Path</td>
            <td>
                ${(exception.path)!}
            </td>
        </tr>
        <tr>
            <td>Exception</td>
            <td>
                ${(exception.error)!}
            </td>
        </tr>
        <tr>
            <td>Message</td>
            <td>
                ${(exception.message)!}
            </td>
        </tr>
        </tbody>
    </table>
</div>
</body>
</html>
```
<a name="et4im"></a>
### 编写全局异常控制类
```java
/**
 * 全局异常控制类
 */
@ControllerAdvice
public class GlobalExceptionHandler {

    /**
     * 404异常处理
     */
    @ExceptionHandler(value = NoHandlerFoundException.class)
    @ResponseStatus(HttpStatus.NOT_FOUND)
    public ModelAndView errorHandler(HttpServletRequest request, NoHandlerFoundException exception, HttpServletResponse response) {
        return commonHandler(request, response,
                exception.getClass().getSimpleName(),
                HttpStatus.NOT_FOUND.value(),
                exception.getMessage());
    }

    /**
     * 405异常处理
     */
    @ExceptionHandler(HttpRequestMethodNotSupportedException.class)
    public ModelAndView errorHandler(HttpServletRequest request, HttpRequestMethodNotSupportedException exception, HttpServletResponse response) {
        return commonHandler(request, response,
                exception.getClass().getSimpleName(),
                HttpStatus.METHOD_NOT_ALLOWED.value(),
                exception.getMessage());
    }

    /**
     * 415异常处理
     */
    @ExceptionHandler(HttpMediaTypeNotSupportedException.class)
    public ModelAndView errorHandler(HttpServletRequest request, HttpMediaTypeNotSupportedException exception, HttpServletResponse response) {
        return commonHandler(request, response,
                exception.getClass().getSimpleName(),
                HttpStatus.UNSUPPORTED_MEDIA_TYPE.value(),
                exception.getMessage());
    }

    /**
     * 500异常处理
     */
    @ExceptionHandler(value = Exception.class)
    public ModelAndView errorHandler (HttpServletRequest request, Exception exception, HttpServletResponse response) {
        return commonHandler(request, response,
                exception.getClass().getSimpleName(),
                HttpStatus.INTERNAL_SERVER_ERROR.value(),
                exception.getMessage());
    }

    /**
     * 业务异常处理
     */
    @ExceptionHandler(value = BasicException.class)
    private ModelAndView errorHandler (HttpServletRequest request, BasicException exception, HttpServletResponse response) {
        return commonHandler(request, response,
                exception.getClass().getSimpleName(),
                exception.getCode(),
                exception.getMessage());
    }

    /**
     * 表单验证异常处理
     */
    @ExceptionHandler(value = BindException.class)
    @ResponseBody
    public ExceptionEntity validExceptionHandler(BindException exception, HttpServletRequest request, HttpServletResponse response) {
        List<FieldError> fieldErrors = exception.getBindingResult().getFieldErrors();
        Map<String,String> errors = new HashMap<>();
        for (FieldError error:fieldErrors) {
            errors.put(error.getField(), error.getDefaultMessage());
        }
        ExceptionEntity entity = new ExceptionEntity();
        entity.setMessage(JSON.toJSONString(errors));
        entity.setPath(request.getRequestURI());
        entity.setCode(HttpStatus.INTERNAL_SERVER_ERROR.value());
        entity.setError(exception.getClass().getSimpleName());
        response.setStatus(HttpStatus.INTERNAL_SERVER_ERROR.value());
        return entity;
    }

    /**
     * 异常处理数据处理
     */
    private ModelAndView commonHandler (HttpServletRequest request, HttpServletResponse response,
                                            String error, int httpCode, String message) {
        ExceptionEntity entity = new ExceptionEntity();
        entity.setPath(request.getRequestURI());
        entity.setError(error);
        entity.setCode(httpCode);
        entity.setMessage(message);
        return determineOutput(request, response, entity);
    }

    /**
     * 异常输出处理
     */
    private ModelAndView determineOutput(HttpServletRequest request, HttpServletResponse response, ExceptionEntity entity) {
        if (!(
                request.getHeader("accept").contains("application/json")
                || (request.getHeader("X-Requested-With") != null && request.getHeader("X-Requested-With").contains("XMLHttpRequest"))
        )) {
            ModelAndView modelAndView = new ModelAndView("error");
            modelAndView.addObject("exception", entity);
            return modelAndView;
        } else {
            response.setStatus(HttpStatus.INTERNAL_SERVER_ERROR.value());
            response.setCharacterEncoding("UTF8");
            response.setHeader("Content-Type", "application/json");
            try {
                response.getWriter().write(ResultJsonTools.build(
                        ResponseCodeConstant.SYSTEM_ERROR,
                        ResponseMessageConstant.APP_EXCEPTION,
                        JSONObject.parseObject(JSON.toJSONString(entity))
                ));
            } catch (IOException e) {
                e.printStackTrace();
            }
            return null;
        }
    }
}
```
<a name="Fi90t"></a>
#### `@ControllerAdvice`
作用于类上，用于标识该类用于处理全局异常。
<a name="jd9GP"></a>
#### `@ExceptionHandler`
作用于方法上，用于对拦截的异常类型进行处理。value 属性用于指定具体的拦截异常类型，如果有多个 ExceptionHandler 存在，则需要指定不同的 value 类型，由于异常类拥有继承关系，所以 ExceptionHandler 会首先执行在继承树中靠前的异常类型。
<a name="wGXqH"></a>
#### BindException
该异常来自于表单验证框架 Hibernate validation，当字段验证未通过时会抛出此异常。
<a name="NwYxX"></a>
### 编写测试 Controller
```java
@RestController
public class TestController {

    @RequestMapping(value = "err")
    public void error(){
        throw new BusinessException(400, "业务异常错误信息");
    }

    @RequestMapping(value = "err2")
    public void error2(){
        throw new NullPointerException("手动抛出异常信息");
    }

    @RequestMapping(value = "err3")
    public int error3(){
        int a = 10 / 0;
        return a;
    }
}
```
<a name="Kc8rG"></a>
#### 使用 AJAX 方式请求时返回的 JSON 格式错误信息。
```json
# /err
{
    "msg": "应用程序异常",
    "code": -1,
    "status_code": 0,
    "data": {
        "path": "/err",
        "code": 400,
        "error": "BusinessException",
        "message": "业务异常错误信息",
        "timestamp": "2018-12-18 11:09:00"
    }
}

# /err2
{
    "msg": "应用程序异常",
    "code": -1,
    "status_code": 0,
    "data": {
        "path": "/err2",
        "code": 500,
        "error": "NullPointerException",
        "message": "手动抛出异常信息",
        "timestamp": "2018-12-18 11:15:15"
    }
}

# /err3
{
    "msg": "应用程序异常",
    "code": -1,
    "status_code": 0,
    "data": {
        "path": "/err3",
        "code": 500,
        "error": "ArithmeticException",
        "message": "/ by zero",
        "timestamp": "2018-12-18 11:15:46"
    }
}

# /err404
{
    "msg": "应用程序异常",
    "code": -1,
    "status_code": 0,
    "data": {
        "path": "/err404",
        "code": 404,
        "error": "NoHandlerFoundException",
        "message": "No handler found for GET /err404",
        "timestamp": "2018-12-18 11:16:11"
    }
}
```
<a name="xmagr"></a>
#### 使用浏览器请求时返回的错误信息界面。
![2021-05-13-23-35-39-133753.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620920149448-0cdea5f7-e923-4838-bf41-5dcdc6e98704.png#clientId=ubde4fee8-a980-4&from=ui&id=u93c2b37f&originHeight=153&originWidth=300&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2911&status=done&style=shadow&taskId=u26b35c7e-8697-4838-bda1-930efc2a920&title=)<br />![2021-05-13-23-35-39-228495.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620920149434-60f818d2-56d8-4500-8a31-8544519353c5.png#clientId=ubde4fee8-a980-4&from=ui&id=u19854085&originHeight=160&originWidth=306&originalType=binary&ratio=1&rotation=0&showTitle=false&size=3026&status=done&style=shadow&taskId=u05fa99fa-e8cb-4a39-9230-503bd7265d9&title=)<br />![2021-05-13-23-35-39-329228.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620920149450-49dfec91-4ecd-4f45-bf5b-356845ef1e15.png#clientId=ubde4fee8-a980-4&from=ui&id=u2dde00ff&originHeight=154&originWidth=304&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2719&status=done&style=shadow&taskId=u1741463f-da72-4c38-b994-0e5783d6d51&title=)<br />![2021-05-13-23-35-39-423972.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620920149464-3e139749-f0fe-4791-8051-71c1717bfe18.png#clientId=ubde4fee8-a980-4&from=ui&id=u52b00e95&originHeight=156&originWidth=386&originalType=binary&ratio=1&rotation=0&showTitle=false&size=3240&status=done&style=shadow&taskId=u3075bb4c-ea1b-481d-9d1f-a11229f8ebc&title=)
