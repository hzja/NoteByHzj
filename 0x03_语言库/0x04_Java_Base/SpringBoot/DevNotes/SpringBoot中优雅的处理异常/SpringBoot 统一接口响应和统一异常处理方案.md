JavaSpringBoot
<a name="SvQWe"></a>
## 一、简介
基于Spring Boot 框架开发的应用程序，大部分都是以提供RESTful接口为主要的目的。前端或者移动端开发人员通过调用后端提供的接口完成数据的交换。<br />在一个项目中RESTful接口响应数据结构是统一的是基本的开发规范。能够减少团队内部不必要的沟通；减轻接口消费者校验数据的负担；降低其他同事接手代码的难度；提高接口的健壮性和可扩展性。
```java
public class GlobalResponseEntity<T>{
	private Boolean success = true;
	private String code = "000000";
	private String message = "request successfully";
	private T data;
}
```
统一的异常处理，是系统完备性的基本象征。通过对全局异常信息的捕获，能够避免将异常信息和系统敏感信息直接抛给客户端；针对特定类型异常捕获之后可以重新对输出数据做编排，提高交互友好度，同时可以记录异常信息以便监控和分析。
<a name="j9zT6"></a>
## 二、如何实现
运用`RestControllerAdvice`或者`ControllerAdvice`注解实现。（`ControllerAdvice`是`RestControllerAdvice`的爸爸）<br />`@ControllerAdvice`是在类上声明的注解，其用法主要有三点：

- **和**`**@ExceptionHandler**`**注解配合使用，**`**@ExceptionHandler**`**标注的方法可以捕获Controller中抛出的的异常，从而达到异常统一处理的目的**
- **和**`**@InitBinder**`**注解配合使用，**`**@InitBinder**`**标注的方法可在请求中注册自定义参数的解析器，从而达到自定义请求参数格式化的目的**
- **和**`**@ModelAttribute**`**注解配合使用，**`**@ModelAttribute**`**标注的方法会在执行目标Controller方法之前执行，可在入参上增加自定义信息**

用法举例：
```java
// 这里@RestControllerAdvice等同于@ControllerAdvice + @ResponseBody
@RestControllerAdvice
public class GlobalHandler {
	private final Logger logger = LoggerFactory.getLogger(GlobalHandler.class);
	// 这里@ModelAttribute("loginUserInfo")标注的modelAttribute()方法表示会在Controller方法之前
	// 执行，返回当前登录用户的UserDetails对象
	@ModelAttribute("loginUserInfo")
	public UserDetails modelAttribute() {
		return (UserDetails) SecurityContextHolder.getContext().getAuthentication().getPrincipal();
	}
	// @InitBinder标注的initBinder()方法表示注册一个Date类型的类型转换器，用于将类似这样的2019-06-10
	// 日期格式的字符串转换成Date对象
	@InitBinder
	protected void initBinder(WebDataBinder binder) {
		SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");
		dateFormat.setLenient(false);
		binder.registerCustomEditor(Date.class, new CustomDateEditor(dateFormat, false));
	} 
	// 这里表示Controller抛出的MethodArgumentNotValidException异常由这个方法处理
	@ExceptionHandler(MethodArgumentNotValidException.class)
	public Result exceptionHandler(MethodArgumentNotValidException e) {
		Result result = new Result(BizExceptionEnum.INVALID_REQ_PARAM.getErrorCode(),
								   BizExceptionEnum.INVALID_REQ_PARAM.getErrorMsg());
		logger.error("req params error", e);
		return result;
	}
	// 这里表示Controller抛出的BizException异常由这个方法处理
	@ExceptionHandler(BizException.class)
	public Result exceptionHandler(BizException e) {
		BizExceptionEnum exceptionEnum = e.getBizExceptionEnum();
		Result result = new Result(exceptionEnum.getErrorCode(), exceptionEnum.getErrorMsg());
		logger.error("business error", e);
		return result;
	}
	// 这里就是通用的异常处理器了,所有预料之外的Exception异常都由这里处理
	@ExceptionHandler(Exception.class)
	public Result exceptionHandler(Exception e) {
		Result result = new Result(1000, "网络繁忙,请稍后再试");
		logger.error("application error", e);
		return result;
	}

}
```
在Controller里取出`@ModelAttribute`标注的方法返回的UserDetails对象：
```java
RestController
@RequestMapping("/json/exam")
@Validated
public class ExamController {
    @Autowired
    private IExamService examService;
    // ......
    @PostMapping("/getExamListByOpInfo")
    public Result<List<GetExamListResVo>> getExamListByOpInfo( @NotNull Date examOpDate,
                                                              @ModelAttribute("loginUserInfo") UserDetails userDetails) {
        List<GetExamListResVo> resVos = examService.getExamListByOpInfo(examOpDate, userDetails);
        Result<List<GetExamListResVo>> result = new Result(resVos);
        return result;
    }

}
```
当入参为`examOpDate=2019-06-10`时，Spring会使用上面`@InitBinder`注册的类型转换器将2019-06-10转换examOpDate对象：
```java
@PostMapping("/getExamListByOpInfo")
public Result<List<GetExamListResVo>> getExamListByOpInfo(@NotNull Date examOpDate,
														  @ModelAttribute("loginUserInfo") UserDetails userDetails) {
	List<GetExamListResVo> resVos = examService.getExamListByOpInfo(examOpDate, userDetails);
	Result<List<GetExamListResVo>> result = new Result(resVos);
	return result;
}
```
`@ExceptionHandler`标注的多个方法分别表示只处理特定的异常。这里需要注意的是当Controller抛出的某个异常多个`@ExceptionHandler`标注的方法都适用时，Spring会选择最具体的异常处理方法来处理，也就是说`@ExceptionHandler(Exception.class)`这里标注的方法优先级最低，只有当其它方法都不适用时，才会来到这里处理。
<a name="TEwgA"></a>
## 三、统一的响应处理
工程目录结构如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658052223033-b05bff72-06e0-403a-baf0-9bbf4a64198a.png#clientId=ucd67b2d6-86f6-4&from=paste&id=u35afd7b9&originHeight=1004&originWidth=934&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ufb1b7cd8-c95d-4f29-8c33-8393ef3a694&title=)<br />GlobalResponse是一个处理器类（handle），用来处理统一响应，代码如下:
```java
package com.naylor.globalresponsebody.handler.response;

import com.alibaba.fastjson.JSON;
import com.naylor.globalresponsebody.handler.GlobalResponseEntity;
import org.springframework.core.MethodParameter;
import org.springframework.core.io.Resource;
import org.springframework.http.MediaType;
import org.springframework.http.converter.HttpMessageConverter;
import org.springframework.http.server.ServerHttpRequest;
import org.springframework.http.server.ServerHttpResponse;
import org.springframework.web.bind.annotation.RestControllerAdvice;
import org.springframework.web.servlet.mvc.method.annotation.ResponseBodyAdvice;

/**
 * @BelongsProject: debris-app
 * @BelongsPackage: com.naylor.globalresponsebody.response
 * @Description: 全局响应
 */

@RestControllerAdvice("com.naylor")
public class GlobalResponse implements ResponseBodyAdvice<Object> {

    /**
     * 拦截之前业务处理，请求先到supports再到beforeBodyWrite
     * <p>
     * 用法1：自定义是否拦截。若方法名称（或者其他维度的信息）在指定的常量范围之内，则不拦截。
     *
     * @param methodParameter
     * @param aClass
     * @return 返回true会执行拦截；返回false不执行拦截
     */
    @Override
    public boolean supports(MethodParameter methodParameter, Class<? extends HttpMessageConverter<?>> aClass) {
        //TODO 过滤
        return true;
    }

    /**
     * 向客户端返回响应信息之前的业务逻辑处理
     * <p>
     * 用法1：无论controller返回什么类型的数据，在写入客户端响应之前统一包装，客户端永远接收到的是约定的格式
     * <p>
     * 用法2：在写入客户端响应之前统一加密
     *
     * @param responseObject     响应内容
     * @param methodParameter
     * @param mediaType
     * @param aClass
     * @param serverHttpRequest
     * @param serverHttpResponse
     * @return
     */
    @Override
    public Object beforeBodyWrite(Object responseObject, MethodParameter methodParameter,
                                  MediaType mediaType,
                                  Class<? extends HttpMessageConverter<?>> aClass,
                                  ServerHttpRequest serverHttpRequest, ServerHttpResponse serverHttpResponse) {
        //responseObject是否为null
        if (null == responseObject) {
            return new GlobalResponseEntity<>("55555", "response is empty.");
        }
        //responseObject是否是文件
        if (responseObject instanceof Resource) {
            return responseObject;
        }
        //该方法返回值类型是否是void
        //if ("void".equals(methodParameter.getParameterType().getName())) {
        //  return new GlobalResponseEntity<>("55555", "response is empty.");
        //}
        if (methodParameter.getMethod().getReturnType().isAssignableFrom(Void.TYPE)) {
            return new GlobalResponseEntity<>("55555", "response is empty.");
        }
        //该方法返回值类型是否是GlobalResponseEntity。若是直接返回，无需再包装一层
        if (responseObject instanceof GlobalResponseEntity) {
            return responseObject;
        }
        //处理string类型的返回值
        //当返回类型是String时，用的是StringHttpMessageConverter转换器，无法转换为Json格式
        //必须在方法体上标注RequestMapping(produces = "application/json; charset=UTF-8")
        if (responseObject instanceof String) {
            String responseString = JSON.toJSONString(new GlobalResponseEntity<>(responseObject));
            return responseString;
        }
        //该方法返回的媒体类型是否是application/json。若不是，直接返回响应内容
        if (!mediaType.includes(MediaType.APPLICATION_JSON)) {
            return responseObject;
        }

        return new GlobalResponseEntity<>(responseObject);
    }
}
```

- `**GlobalResponse**`**类需要实现**`**ResponseBodyAdvice**`**接口**
- **重写supports方法，可对响应进行过滤。实际开发中不一定所有的方法返回值都是相同的模板，这里可以根据**`**MethodParameter**`**进行过滤，此方法返回true则会走过滤，即会调用**`**beforeBodyWrite**`**方法，否则不会调用。**
- **重写**`**beforeBodyWrite**`**方法，编写具体的响应数据逻辑**

`GlobalResponseEntity`是一个实体类，用来封装统一响应和统一异常处理的返回值模板，具体代码如下：
```java
@Data
@Accessors(chain = true)
public class GlobalResponseEntity<T> implements Serializable {

    private Boolean success = true;
    private String code = "000000";
    private String message = "request successfully";
    private T data;

    public GlobalResponseEntity() {
        super();
    }

    public GlobalResponseEntity(T data) {
        this.data = data;
    }

    public GlobalResponseEntity(String code, String message) {
        this.code = code;
        this.message = message;
        this.data = null;
    }

    public GlobalResponseEntity(String code, String message, T data) {
        this.code = code;
        this.message = message;
        this.data = data;
    }

    public GlobalResponseEntity(Boolean success, String code, String message) {
        this.success = success;
        this.code = code;
        this.message = message;
    }

    public GlobalResponseEntity(Boolean success, String code, String message, T data) {
        this.success = success;
        this.code = code;
        this.message = message;
        this.data = data;
    }

    public static GlobalResponseEntity<?> badRequest(String code, String message) {
        return new GlobalResponseEntity<>(false, code, message);
    }

    public static GlobalResponseEntity<?> badRequest() {
        return new GlobalResponseEntity<>(false, "404", "无法找到您请求的资源");
    }

}
```

- `**GlobalResponseEntity**`**类为一个泛型类，T为接口具体的返回数据**
- **success表示接口响应是否成功，更多情况下这个是业务叫法，和http状态码关系不大**
- **code表示接口响应状态码，可以根据特定业务场景自己定义**
- **message是描述信息**
- **实际开发中code和mesage的具体值可以用枚举来维护**
<a name="YLWwP"></a>
## 四、统一的异常处理
新增`GlobalException`类，编写统一异常处理。类上面添加<br />`@RestControllerAdvice("com.naylor")`和`@ResponseBody`注解，`@ResponseBody`用来对响应内容进行编排，如http状态码。代码如下：
```java
@RestControllerAdvice("com.naylor")
@ResponseBody
@Slf4j
public class GlobalException {
	
	/**
	* 捕获一般异常
	* 捕获未知异常
	*
	* @param e
	* @return
	*/
	@ExceptionHandler(Exception.class)
	public ResponseEntity<Object> handleException(Exception e) {
		return new ResponseEntity<>(
			new GlobalResponseEntity<>(false, "555",
									   e.getMessage() == null ? "未知异常" : e.getMessage()),
			HttpStatus.INTERNAL_SERVER_ERROR);
	}
	
	/**
	* 处理404异常
	*
	* @return
	*/
	@ExceptionHandler(NoHandlerFoundException.class)
	public ResponseEntity<Object> handleNoHandlerFoundException(NoHandlerFoundException e) {
		return new ResponseEntity<>(
			new GlobalResponseEntity(false, "4040",
									 e.getMessage() == null ? "请求的资源不存在" : e.getMessage()),
			HttpStatus.NOT_FOUND);
	}
	
	/**
	* 捕获运行时异常
	*
	* @param e
	* @return
	*/
	@ExceptionHandler(RuntimeException.class)
	public ResponseEntity<Object> handleRuntimeException(RuntimeException e) {
		log.error("handleRuntimeException:", e);
		return new ResponseEntity<>(
			new GlobalResponseEntity(false, "rt555",
									 e.getMessage() == null ? "运行时异常" : e.getMessage().replace("java.lang.RuntimeException: ", "")),
			HttpStatus.INTERNAL_SERVER_ERROR);
	}
	
	
	/**
	* 捕获业务异常
	* 捕获自定义异常
	*
	* @param e
	* @return
	*/
	@ExceptionHandler(BizServiceException.class)
	public ResponseEntity<Object> handleBizServiceException(BizServiceException e) {
		return new ResponseEntity<>(
			new GlobalResponseEntity(false, e.getErrorCode(), e.getMessage()),
			HttpStatus.INTERNAL_SERVER_ERROR);
	}
	
	
	/**
	* 捕获参数校验异常
	* javax.validation.constraints
	*
	* @param e
	* @return
	*/
	@ExceptionHandler({MethodArgumentNotValidException.class})
	public ResponseEntity<Object> handleMethodArgumentNotValidException(MethodArgumentNotValidException e) {
		String msg = "参数校验失败";
		List<FieldFailedValidate> fieldFailedValidates = this.extractFailedMessage(e.getBindingResult().getFieldErrors());
		if (null != fieldFailedValidates && fieldFailedValidates.size() > 0) {
			msg = fieldFailedValidates.get(0).getMessage();
		}
		return new ResponseEntity<>(
			new GlobalResponseEntity<>(false, "arg555", msg, null),
			HttpStatus.BAD_REQUEST);
	}
	
	/**
	* 组装validate错误信息
	*
	* @param fieldErrors
	* @return
	*/
	private List<FieldFailedValidate> extractFailedMessage(List<FieldError> fieldErrors) {
		List<FieldFailedValidate> fieldFailedValidates = new ArrayList<>();
		if (null != fieldErrors && fieldErrors.size() > 0) {
			FieldFailedValidate fieldFailedValidate = null;
			for (FieldError fieldError : fieldErrors) {
				fieldFailedValidate = new FieldFailedValidate();
				fieldFailedValidate.setMessage(fieldError.getDefaultMessage());
				fieldFailedValidate.setName(fieldError.getField());
				
				fieldFailedValidates.add(fieldFailedValidate);
			}
		}
		
		return fieldFailedValidates;
	}
}
```
