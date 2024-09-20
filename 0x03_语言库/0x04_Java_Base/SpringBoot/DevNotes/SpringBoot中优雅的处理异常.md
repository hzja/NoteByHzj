Java SpringBoot 异常
<a name="dG90J"></a>
## 使用异常
使用异常的方式很简单，Spring提供了两个注解：`@ControllerAdvice`和`@ExceptionHandler`

- `@ControllerAdvice` 控制器增强，使`@ExceptionHandler`、`@InitBinder`、`@ModelAttribute`注解的方法应用到所有的 `@RequestMapping`注解的方法
- `@ExceptionHandler` 异常处理器，此注解的作用是当出现其定义的异常时进行处理的方法
<a name="GxT6l"></a>
## 创建统一异常类
```java
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.TypeMismatchException;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.boot.autoconfigure.web.AbstractErrorController;
import org.springframework.boot.autoconfigure.web.ErrorAttributes;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpStatus;
import org.springframework.stereotype.Controller;
import org.springframework.web.HttpRequestMethodNotSupportedException;
import org.springframework.web.bind.MissingServletRequestParameterException;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.multipart.MultipartException;
import org.springframework.web.servlet.NoHandlerFoundException;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.sql.SQLException;
import java.util.Map;

/**
 * 全局异常处理
 * 一般情况下，方法都有异常处理机制，但不能排除有个别异常没有处理，导致返回到前台，因此在这里做一个异常拦截，统一处理那些未被处理过的异常
 *
 * @author ${author} on ${date}
 */
@ControllerAdvice
@Controller
@RequestMapping
public class GlobalExceptionHandler extends AbstractErrorController {

    private static final Logger LOGGER = LoggerFactory.getLogger(GlobalExceptionHandler.class);

    public GlobalExceptionHandler(ErrorAttributes errorAttributes) {
        super(errorAttributes);
    }

    @Value("${server.error.path:${error.path:/error}}")
    private static String errorPath = "/error";


    /**
     * sql异常
     *
     * @param req
     * @param rsp
     * @param ex
     * @return
     * @throws Exception
     */
    @ResponseBody
    @ExceptionHandler(SQLException.class)
    public Result<String> sqlException(HttpServletRequest req, HttpServletResponse rsp, Exception ex) {
        LOGGER.error("!!! request uri:{} from {} server exception:{}", req.getRequestURI(), RequestUtil.getIpAddress(req), ex == null ? null : ex);
        return ResponseMsgUtil.builderResponse(1002, ex == null ? null : ex.getMessage(), null);
    }


    /**
     * 500错误.
     *
     * @param req
     * @param rsp
     * @param ex
     * @return
     * @throws Exception
     */
    @ResponseBody
    @ResponseStatus(code = HttpStatus.INTERNAL_SERVER_ERROR)
    @ExceptionHandler(Exception.class)
    public Result<String> serverError(HttpServletRequest req, HttpServletResponse rsp, Exception ex) throws Exception {
        LOGGER.error("!!! request uri:{} from {} server exception:{}", req.getRequestURI(), RequestUtil.getIpAddress(req), ex == null ? null : ex);
        return ResponseMsgUtil.builderResponse(1002, ex == null ? null : ex.getMessage(), null);
    }


    /**
     * 404的拦截.
     *
     * @param request
     * @param response
     * @param ex
     * @return
     * @throws Exception
     */
    @ResponseBody
    @ResponseStatus(code = HttpStatus.NOT_FOUND)
    @ExceptionHandler(NoHandlerFoundException.class)
    public Result<String> notFound(HttpServletRequest request, HttpServletResponse response, Exception ex) throws Exception {
        LOGGER.error("!!! request uri:{} from {} not found exception:{}", request.getRequestURI(), RequestUtil.getIpAddress(request), ex);
        return ResponseMsgUtil.builderResponse(404, ex == null ? null : ex.getMessage(), null);
    }

    @ExceptionHandler(MissingServletRequestParameterException.class)
    @ResponseBody
    public Result<String> paramException(MissingServletRequestParameterException ex) {
        LOGGER.error("缺少请求参数:{}", ex.getMessage());
        return ResponseMsgUtil.builderResponse(99999, "缺少参数:" + ex.getParameterName(), null);
    }

    //参数类型不匹配
    //getPropertyName()获取数据类型不匹配参数名称
    //getRequiredType()实际要求客户端传递的数据类型
    @ExceptionHandler(TypeMismatchException.class)
    @ResponseBody
    public Result<String> requestTypeMismatch(TypeMismatchException ex) {
        LOGGER.error("参数类型有误:{}", ex.getMessage());
        return ResponseMsgUtil.builderResponse(99999, "参数类型不匹配,参数" + ex.getPropertyName() + "类型应该为" + ex.getRequiredType(), null);
    }

    @ExceptionHandler(HttpRequestMethodNotSupportedException.class)
    @ResponseBody
    public Result<String> requestMethod(HttpRequestMethodNotSupportedException ex) {
        LOGGER.error("请求方式有误：{}", ex.getMethod());
        return ResponseMsgUtil.builderResponse(99999, "请求方式有误:" + ex.getMethod(), null);
    }

    @ExceptionHandler(MultipartException.class)
    @ResponseBody
    public Result<String> fileSizeLimit(MultipartException m) {
        LOGGER.error("超过文件上传大小限制");
        return ResponseMsgUtil.builderResponse(99999, "超过文件大小限制,最大10MB", null);
    }


    /**
     * 重写/error请求, ${server.error.path:${error.path:/error}} IDEA报红无需处理，作用是获取spring底层错误拦截
     *
     * @param request
     * @param response
     * @return
     * @throws Exception
     */
    @ResponseBody
    @RequestMapping(value = "${server.error.path:${error.path:/error}}")
    public Result<String> handleErrors(HttpServletRequest request, HttpServletResponse response) throws Exception {
        HttpStatus status = getStatus(request);
        if (status == HttpStatus.NOT_FOUND) {
            throw new NoHandlerFoundException(request.getMethod(), request.getRequestURL().toString(), new HttpHeaders());
        }
        Map<String, Object> body = getErrorAttributes(request, true);
        return ResponseMsgUtil.builderResponse(Integer.parseInt(body.get("status").toString()), body.get("message").toString(), null);
    }


    @Override
    public String getErrorPath() {
        return errorPath;
    }
}
```
从上面可以看出来，定义了sql异常，500异常，404异常该做的事情，通过`@ExceptionHandler`注解来拦截程序中的异常，比如执行SQL时，抛出了异常，就会被统一异常给拦截，然后返回想要返回的数据<br />`@ResponseStatus`注解可加可不加，就是对响应码进行拦截，如代码上，对404响应码进行了拦截<br />最下面的`handleErrors`方法，是对Spring底层访问/error的时候进行了一次拦截，获取当前请求码，如果是404.抛出404的异常
<a name="C4HF6"></a>
### 优化处理异常没有自定义返回数据
```java
import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.serializer.SerializerFeature;

/**
 * 返回数据结果集合
 */
public class Result<T> {
    private Integer code;
    private String resMsg;
    private T data;

    public Result() {
    }

    public Integer getCode() {
        return this.code;
    }

    public void setCode(Integer resCode) {
        this.code = resCode;
    }

    public String getResMsg() {
        return this.resMsg;
    }

    public void setResMsg(String resMsg) {
        this.resMsg = resMsg;
    }

    public T getData() {
        return this.data;
    }

    public void setData(T data) {
        this.data = data;
    }

    public String toJson() {
        return this.data == null ? JSON.toJSONString(this) : this.toJson(SerializerFeature.WriteNullListAsEmpty, SerializerFeature.WriteNullStringAsEmpty, SerializerFeature.WriteMapNullValue);
    }

    public String toJson(SerializerFeature... features) {
        return features == null ? this.toJson() : JSON.toJSONString(this, features);
    }

    public String toString() {
        return "Result{code=" + this.code + ", resMsg='" + this.resMsg + '\'' + ", data=" + this.data + '}';
    }
}

/**
 * 返回数据结果集合
 */
public class ResponseMsgUtil {
    public ResponseMsgUtil() {
    }

    public static <T> Result<T> builderResponse(int code, String msg, T data) {
        Result<T> res = new Result();
        res.setCode(code);
        res.setResMsg(msg);
        res.setData(data);
        return res;
    }

    public static <T> Result<T> success(String msg) {
        return builderResponse(0, msg, null);
    }
    public static <T> Result<T> success(String msg, T data) {
        return builderResponse(0, msg, data);
    }
    public static <T> Result<T> success(T data) {
        return builderResponse(0, "Success", data);
    }
    public static <T> Result<T> success() {
        return builderResponse(0, "Success", null);
    }

    public static <T> Result<T> failure() {
        return builderResponse(1, "Failure", null);
    }
    public static <T> Result<T> failure(String msg) {
        return builderResponse(1, msg, null);
    }
    public static <T> Result<T> failure(T date) {
        return builderResponse(-1, "Failure", date);
    }

    public static <T> Result<T> illegalRequest() {
        return builderResponse(1008, "Illegal request", (T) null);
    }

    public static <T> Result<T> exception() {
        return builderResponse(1002, "request exception", (T) null);
    }

    public static <T> Result<T> paramsEmpty() {
        return builderResponse(1009, "the input parameter is null", (T) null);
    }
}
```
<a name="dSVZ9"></a>
## 测试
将这些准备都做好以后，项目跑起来，访问一个接口，故意不传某个必填项，就会被统一异常拦截，如下
```json
{
  code: 1002,
  data: null,
  msg: "Required String parameter 'id' is not present"
}
```
