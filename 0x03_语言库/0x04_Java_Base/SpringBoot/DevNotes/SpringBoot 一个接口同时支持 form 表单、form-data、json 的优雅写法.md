Java SpringBoot<br />而在 Java 中，一个接口只支持一种 content-type，json 就用 `@RequestBody`，form 表单就用 `@RequestParam` 或不写，form-data 就用 `MultipartFile`。
<a name="wzHqH"></a>
## 兼容版本
如果要把在一个接口中同时兼容三种，比较笨的办法就是获取 `HttpServletRequest`，然后自己再写方法解析。类似如下：
```java
private Map<String, Object> getParams(HttpServletRequest request) {

    String contentType = request.getContentType();
    if (contentType.contains("application/json")) {
        // json 解析...
        return null;
    } else if (contentType.contains("application/x-www-form-urlencoded")) {
        // form 表单解析 ...
        return null;
    } else if (contentType.contains("multipart")) {
        // 文件流解析
        return null;
    } else {
         throw new BizException("不支持的content-type");
    } 

}
```
但是这样写有弊端。

1. 代码很丑，具体到解析代码又臭又长
2. 只能返回固定 map 或者自己重新组装参数类
3. 无法使用 `@Valid` 校验参数，像这种几十个参数都要检验的简直是灾难
<a name="e0dQO"></a>
## 优雅版本
网上有 form 表单和 json 同时兼容的版本，但是没有兼容 form-data，在这做一下补充。
<a name="SGBYV"></a>
### 1、自定义注解
```java
@Target(ElementType.PARAMETER)
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface GamePHP {
}
```
<a name="A6sh8"></a>
### 2、自定义注解解析
```java
public class GamePHPMethodProcessor implements HandlerMethodArgumentResolver {

    private GameFormMethodArgumentResolver formResolver;
    private GameJsonMethodArgumentResolver jsonResolver;

    public GamePHPMethodProcessor() {
        List<HttpMessageConverter<?>> messageConverters = new ArrayList<>();
        PHPMessageConverter PHPMessageConverter = new PHPMessageConverter();
        messageConverters.add(PHPMessageConverter);

        jsonResolver = new GameJsonMethodArgumentResolver(messageConverters);
        formResolver = new GameFormMethodArgumentResolver();
    }

    @Override
    public boolean supportsParameter(MethodParameter parameter) {
        GamePHP ann = parameter.getParameterAnnotation(GamePHP.class);
        return (ann != null);
    }

    @Override
    public Object resolveArgument(MethodParameter methodParameter, ModelAndViewContainer modelAndViewContainer, NativeWebRequest nativeWebRequest, WebDataBinderFactory webDataBinderFactory) throws Exception {
        ServletRequest servletRequest = nativeWebRequest.getNativeRequest(ServletRequest.class);
        String contentType = servletRequest.getContentType();
        if (contentType == null) {
            throw new IllegalArgumentException("不支持contentType");
        }

        if (contentType.contains("application/json")) {
            return jsonResolver.resolveArgument(methodParameter, modelAndViewContainer, nativeWebRequest, webDataBinderFactory);
        }

        if (contentType.contains("application/x-www-form-urlencoded")) {
            return formResolver.resolveArgument(methodParameter, modelAndViewContainer, nativeWebRequest, webDataBinderFactory);
        }

        if (contentType.contains("multipart")) {
            return formResolver.resolveArgument(methodParameter, modelAndViewContainer, nativeWebRequest, webDataBinderFactory);
        }

        throw new IllegalArgumentException("不支持contentType");
    }
}
```
<a name="IY59F"></a>
### 3、添加到 spring configuration
```java
@Bean
public MyMvcConfigurer mvcConfigurer() {
    return new MyMvcConfigurer();
}

public static class MyMvcConfigurer implements WebMvcConfigurer {
    public void addArgumentResolvers(List<HandlerMethodArgumentResolver> resolvers) {
        resolvers.add(new GamePHPMethodProcessor());
    }
}
```
<a name="lw2C7"></a>
### 4、form-data 的特殊处理
引入 jar 包
```xml
<dependency>
	<groupId>commons-fileupload</groupId>
	<artifactId>commons-fileupload</artifactId>
	<version>1.3.1</version>
</dependency>
<dependency>
	<groupId>commons-io</groupId>
	<artifactId>commons-io</artifactId>
	<version>2.4</version>
</dependency>
```
新增解析 bean
```java
@Bean(name = "multipartResolver")
public MultipartResolver multipartResolver(){
    CommonsMultipartResolver resolver = new CommonsMultipartResolver();
    resolver.setDefaultEncoding("UTF-8");
    resolver.setResolveLazily(true);//resolveLazily属性启用是为了推迟文件解析，以在在UploadAction中捕获文件大小异常
    resolver.setMaxInMemorySize(40960);
    resolver.setMaxUploadSize(50*1024*1024);//上传文件大小 50M 50*1024*1024
    return resolver;
}
```
特殊说明，`GameJsonMethodArgumentResolver` 和 `GameFormMethodArgumentResolver` 是自定义的 json 和 form 解析，如果没有自定义的，使用 spring 默认的 `ServletModelAttributeMethodProcessor` 和 `RequestResponseBodyMethodProcessor` 也可以。<br />只需将 `@RequestParam` 注解改为 `@GamePHP`，接口即可同时兼容三种 content-type。<br />其流程为，Spring 启动的时候，`MyMvcConfigurer` 调用 `addArgumentResolvers` 方法将 `GamePHPMethodProcessor` 注入，接到请求时，`supportsParameter` 方法判断是否使用此 resolver，如果为 `true`，则进入 `resolveArgument` 方法执行。
