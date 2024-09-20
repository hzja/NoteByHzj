Java SpringBoot
<a name="hCW4J"></a>
## 前言
开发SpringBoot项目过程中，不可避免的需要处理各种异常，SpringMVC架构中各层会出现大量的`try {...} catch {...} finally {...}`代码块，不仅有大量的冗余代码，而且还影响代码的可读性。这样就需要定义个全局统一异常处理器，以便业务层再也不必处理异常。
<a name="exV6z"></a>
## 推荐理由

- 代码复制到项目中通过简单的配置即可实现
- 可以灵活的根据自己的业务异常进行更细粒度的扩展
<a name="ApnkV"></a>
## 实践
<a name="NMHeQ"></a>
### 1、封装统一返回结果类
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650115962917-0e008a99-a27b-4ed7-9ffc-db9aa8ca1a80.png#averageHue=%23faf9f9&clientId=u1aaedbf3-9077-4&from=paste&id=u4350873b&originHeight=654&originWidth=1054&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc4368d89-49c2-4468-9647-f11b2c49c1c&title=)<br />源代码
```java
public class AjaxResult {  
	//是否成功  
	private Boolean success;  
	//状态码  
	private Integer code;  
	//提示信息  
	private String msg;  
	//数据  
	private Object data;  
	public AjaxResult() {  
		
	}  
	//自定义返回结果的构造方法  
	public AjaxResult(Boolean success,Integer code, String msg,Object data) {  
		this.success = success;  
		this.code = code;  
		this.msg = msg;  
		this.data = data;  
	}  
	//自定义异常返回的结果  
	public static AjaxResult defineError(BusinessException de){  
		AjaxResult result = new AjaxResult();  
		result.setSuccess(false);  
		result.setCode(de.getErrorCode());  
		result.setMsg(de.getErrorMsg());  
		result.setData(null);  
		return result;  
	}  
	//其他异常处理方法返回的结果  
	public static AjaxResult otherError(ErrorEnum errorEnum){  
		AjaxResult result = new AjaxResult();  
		result.setMsg(errorEnum.getErrorMsg());  
		result.setCode(errorEnum.getErrorCode());  
		result.setSuccess(false);  
		result.setData(null);  
		return result;  
	}  
	public Boolean getSuccess() {  
		return success;  
	}  
	public void setSuccess(Boolean success) {  
		this.success = success;  
	}  
	public Integer getCode() {  
		return code;  
	}  
	public void setCode(Integer code) {  
		this.code = code;  
	}  
	public String getMsg() {  
		return msg;  
	}  
	public void setMsg(String msg) {  
		this.msg = msg;  
	}  
	public Object getData() {  
		return data;  
	}  
	public void setData(Object data) {  
		this.data = data;  
	}  
	
}
```
<a name="HUvdB"></a>
### 2、自定义异常封装类
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650115962959-54d936a4-bdc8-482c-9693-839ec97018ed.png#averageHue=%23fdfcfc&clientId=u1aaedbf3-9077-4&from=paste&id=uaaa07f98&originHeight=655&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u57195f5a-4012-4dc0-a534-5e1d5fb7ddc&title=)<br />源码：
```java
public class BusinessException extends RuntimeException {
    private static final long serialVersionUID = 1L;
    /**
     * 错误状态码  
     */
    protected Integer errorCode;
    /**
     * 错误提示  
     */
    protected String errorMsg;

    public BusinessException(){

    }

    public BusinessException(Integer errorCode, String errorMsg) {
        this.errorCode = errorCode;
        this.errorMsg = errorMsg;
    }

    public Integer getErrorCode() {
        return errorCode;
    }

    public void setErrorCode(Integer errorCode) {
        this.errorCode = errorCode;
    }

    public String getErrorMsg() {
        return errorMsg;
    }

    public void setErrorMsg(String errorMsg) {
        this.errorMsg = errorMsg;
    }
}
```
<a name="E2HtC"></a>
### 3、错误枚举，拒绝硬编码
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650115963009-9613ae82-3cb4-42a5-b512-434ec1dc2ac7.png#averageHue=%23fdfdfd&clientId=u1aaedbf3-9077-4&from=paste&id=u97729a9e&originHeight=681&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uee3fbcda-dfff-407e-8a5b-54024ee3a2c&title=)<br />源码
```java
public enum ErrorEnum {  
	// 数据操作错误定义  
	SUCCESS(200, "成功"),  
	NO_PERMISSION(403,"无权限"),  
	NO_AUTH(401,"未登录"),  
	NOT_FOUND(404, "未找到该资源!"),  
	INTERNAL_SERVER_ERROR(500, "服务器异常请联系管理员"),  
	;  
	
	/** 错误码 */  
	private Integer errorCode;  
	
	/** 错误信息 */  
	private String errorMsg;  
	
	ErrorEnum(Integer errorCode, String errorMsg) {  
		this.errorCode = errorCode;  
		this.errorMsg = errorMsg;  
	}  
	
	public Integer getErrorCode() {  
		return errorCode;  
	}  
	
	public String getErrorMsg() {  
		return errorMsg;  
	}  
}
```
<a name="BuCdL"></a>
### 4、全局异常处理类
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650115963019-c0c91362-c034-48eb-98cd-77bfbacda03b.png#averageHue=%23fbfaf9&clientId=u1aaedbf3-9077-4&from=paste&id=u95083a1c&originHeight=611&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0ca276c4-cc14-42a2-a902-953ebf6bf8d&title=)<br />源码
```java
/**  
 * 全局异常处理器  
 *   
 */  
@RestControllerAdvice  
public class GlobalExceptionHandler {  
	private static final Logger log = LoggerFactory.getLogger(GlobalExceptionHandler.class);  
	
	/**  
	 * 处理自定义异常  
	 *  
	 */  
	@ExceptionHandler(value = BusinessException.class)  
	public AjaxResult bizExceptionHandler(BusinessException e) {  
		log.error(e.getMessage(), e);  
		return AjaxResult.defineError(e);  
	}  
	
	/**  
	 *处理其他异常  
	 *  
	 */  
	@ExceptionHandler(value = Exception.class)  
	public AjaxResult exceptionHandler( Exception e) {  
		log.error(e.getMessage(), e);  
		return AjaxResult.otherError(ErrorEnum.INTERNAL_SERVER_ERROR); 
	}
}
```
<a name="SxI3M"></a>
### 5、测试
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650115962960-44eb6c1d-8bf6-4abd-8880-6a113ae61b81.png#averageHue=%23fcfbfa&clientId=u1aaedbf3-9077-4&from=paste&id=u64720228&originHeight=455&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1ffd2b68-d876-4f13-a539-1791901581a&title=)
