Java Spring<br />在日常开发中，有时候经常需要和第三方接口打交道，有时候是方调用别人的第三方接口，有时候是别人在调用我方的第三方接口，那么为了调用接口的安全性，一般都会对传输的数据进行加密操作，如果每个接口都手动加密和解密，那么工作量太大而且代码冗余。那么有没有简单的方法，借助 Spring 提供的  `RequestBodyAdvice` 和 `ResponseBodyAdvice` 可以实现解密和加密操作。
<a name="Of60F"></a>
## 需求分析

- 后台方法上如果有 `@Encrypt` 注解和 `@RequestBody` 修饰的方法，需要进行参数的解密    
- 后台方法上如果有 `@Encrypt` 注解和 `@ResponseBody` 修饰的方法，需要进行参数的加密
- 加密和解密规则

加密：对返回的值中增加 `-encrypt` 值<br />解密：对传入的值中删除 `-encrypt` 值<br />注：

- `@Encrypt` 为自定义的一个注解。
- 此处为了简单，就使用删除或增加 `-encrypt` 这个，实际情况下可以使用复杂的加解密规则
<a name="rqJnN"></a>
## 1、基本思路介绍
`RequestBodyAdvice`：在 Sping 4.2 新加入的一个接口，它可以使用在 `@RequestBody` 或 `HttpEntity` 修改的参数之前进行参数的处理，比如进行参数的解密。<br />`ResponseBodyAdvice`：在 Spring 4.1 新加入的一个接口，在消息体被 `HttpMessageConverter` 写入之前允许 Controller 中 `@ResponseBody` 修饰的方法或 `ResponseEntity` 调整响应中的内容，比如进行相应的加密。
<a name="MW6wl"></a>
## 2、功能实现步骤
<a name="kHe9j"></a>
### 1. 编写加密注解类（Encrypt）
```java
@Target({ElementType.PARAMETER, ElementType.METHOD})  
@Retention(RetentionPolicy.RUNTIME)  
@Documented  
public @interface Encrypt {  

}  
```
<a name="q8eN8"></a>
### 2. 编写`RequestBodyAdvice`接口实现类，实现数据的解密操作
```java
@Slf4j  
@RestControllerAdvice  
public class ParamEncryptRequestBodyAdvice implements RequestBodyAdvice {  

    @Override  
    public boolean supports(MethodParameter methodParameter, Type type, Class<? extends HttpMessageConverter<?>> aClass) {  
        return methodParameter.hasParameterAnnotation(RequestBody.class);  
    }  

    @Override  
    public Object handleEmptyBody(Object o, HttpInputMessage httpInputMessage, MethodParameter methodParameter, Type type, Class<? extends HttpMessageConverter<?>> aClass) {  
        return o;  
    }  

    @Override  
    public HttpInputMessage beforeBodyRead(HttpInputMessage httpInputMessage, MethodParameter methodParameter, Type type, Class<? extends HttpMessageConverter<?>> aClass) throws IOException {  
        return new HttpInputMessage() {  
            @Override  
            public InputStream getBody() throws IOException {  
                log.info("此处进行解密数据");  
                return new ByteArrayInputStream(IOUtils.toString(httpInputMessage.getBody()).replace("-encrypt", "").getBytes(StandardCharsets.UTF_8));  
            }  

            @Override  
            public HttpHeaders getHeaders() {  
                return httpInputMessage.getHeaders();  
            }  
        };  
    }  

    @Override  
    public Object afterBodyRead(Object o, HttpInputMessage httpInputMessage, MethodParameter methodParameter, Type type, Class<? extends HttpMessageConverter<?>> aClass) {  
        return o;  
    }  
}  
```
<a name="dTU6G"></a>
### 3. 编写ResponseBodyAdvice接口实现类，实现数据的加密操作
```java
@Slf4j  
@RestControllerAdvice  
public class ParamEncryptResponseBodyAdvice implements ResponseBodyAdvice {  

    private final ObjectMapper objectMapper = new ObjectMapper();  

    @Override  
    public boolean supports(MethodParameter returnType, Class converterType) {  
        return returnType.hasMethodAnnotation(ResponseBody.class);  
    }  

    @Override  
    public Object beforeBodyWrite(Object body, MethodParameter returnType, MediaType selectedContentType, Class selectedConverterType, ServerHttpRequest request, ServerHttpResponse response) {  
        log.info("此处进行加密数据");  
        if (null != body) {  
            try {  
                Map map = objectMapper.readValue(objectMapper.writeValueAsString(body), Map.class);  
                map.forEach((key, value) -> map.put(key, value + "-encrypt"));  
                return map;  
            } catch (IOException e) {  
                log.error("加密数据失败.", e);  
            }  
        }  
        return body;  
    }  
}  
```
<a name="pGXFe"></a>
### 4. 编写控制层进行测试
```java
@RestController  
@RequestMapping("user-info")  
@Slf4j  
public class UserInfoController {  

    /** 
     * 添加用户实现返回值加密 
     * 
     * @param userInfo 
     * @return 
     */  
    @PostMapping("add")  
    @Encrypt  
    public UserInfo add(@RequestBody UserInfo userInfo) {  
        log.info("添加新用户:[{}]", userInfo);  
        return userInfo;  
    }  

    /** 
     * 修改实现获取的参数进行解密 
     * 
     * @param userInfo 
     * @return 
     */  
    @PostMapping("update")  
    public UserInfo update(@Encrypt @RequestBody UserInfo userInfo) {  
        log.info("修改用户信息:[{}]", userInfo);  
        return userInfo;  
    }  
}  
```
<a name="Vkl0t"></a>
### 5. 测试参数的解密操作
![2021-05-26-15-41-02-326891.gif](https://cdn.nlark.com/yuque/0/2021/gif/396745/1622015042993-bef363dc-cdc2-461e-b798-f6368d126ca4.gif#clientId=u571f5920-d81e-4&from=ui&id=u22fa5319&originHeight=468&originWidth=1080&originalType=binary&size=815348&status=done&style=none&taskId=uc5309342-4117-49f8-b5b4-a3a515b8daf)<br />可以看到：参数中的 `-encrypt` 传递后后台被后台自动截取了，这样就类似于解密操作。
<a name="gm6rK"></a>
### 6. 测试返回值的加密操作
![2021-05-26-15-41-02-603146.gif](https://cdn.nlark.com/yuque/0/2021/gif/396745/1622015051518-ba555a51-5892-4af2-87b3-d990262e2246.gif#clientId=u571f5920-d81e-4&from=ui&id=u16e8b9d5&originHeight=468&originWidth=1080&originalType=binary&size=797345&status=done&style=none&taskId=u8d21487e-df43-48df-950f-554f97cc62e)<br />可以看到：返回的值后面都有一个 `-encrypt`, 这样就模拟实现了类似于加密操作。
