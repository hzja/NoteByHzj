JavaSpringBoot<br />在一个 Web 请求中，参数我们无非就是放在地址栏或者请求体中，个别请求可能放在请求头中。<br />放在地址栏中，可以通过如下方式获取参数：
```java
String boy = request.getParameter("name");
```
放在请求体中，如果是 key/value 形式，可以通过如下方式获取参数：
```java
String boy = request.getParameter("name");
```
如果是 JSON 形式，则通过如果如下方式获取到输入流，然后解析成 JSON 字符串，再通过 JSON 工具转为对象：
```java
BufferedReader reader = new BufferedReader(new InputStreamReader(request.getInputStream()));
String json = reader.readLine();
reader.close();
User user = new ObjectMapper().readValue(json, User.class);
```
如果参数放在请求头中，可以通过如下方式获取：
```java
String boy = request.getHeader("name");
```
如果用的是 Jsp/Servlet 那一套技术栈，那么参数获取无外乎这几种方式。<br />如果用了 SpringMVC 框架，有的小伙伴们可能会觉得参数获取方式太丰富了，各种注解如 `@RequestParam`、`@RequestBody`、`@RequestHeader`、`@PathVariable`，参数可以是 key/value 形式，也可以是 JSON 形式，非常丰富！但是，**「无论多么丰富，最底层获取参数的方式无外乎上面几种。」**<br />那有小伙伴要问了，SpringMVC 到底是怎么样从 request 中把参数提取出来直接给我们用的呢？例如下面这个接口：
```
@RestController
public class HelloController {
    @GetMapping("/hello")
    public String hello(String name) {
        return "hello "+name;
    }
}
```
都知道 name 参数是从 `HttpServletRequest` 中提取出来的，到底是怎么提取出来的？这就是要分享的话题。
<a name="OGD3m"></a>
## 1、自定义参数解析器
为了搞清楚这个问题，先来自定义一个参数解析器看看。<br />自定义参数解析器需要实现 `HandlerMethodArgumentResolver` 接口，先来看看该接口：
```java
public interface HandlerMethodArgumentResolver {
    boolean supportsParameter(MethodParameter parameter);
    @Nullable
    Object resolveArgument(MethodParameter parameter, @Nullable ModelAndViewContainer mavContainer,
                           NativeWebRequest webRequest, @Nullable WebDataBinderFactory binderFactory) throws Exception;

}
```
这个接口中就两个方法：

- `supportsParameter`：该方法表示是否启用这个参数解析器，返回 true 表示启用，返回 false 表示不启用。
- `resolveArgument`：这是具体的解析过程，就是从 request 中取出参数的过程，方法的返回值就对应了接口中参数的值。

自定义参数解析器只需要实现该接口即可。<br />假设现在有这样一个需求（实际上在 Spring Security 中获取当前登录用户名非常方便，这里只是为了该案例而做，勿抬杠）：<br />假设我现在系统安全框架使用了 Spring Security，如果在接口的参数上添加了 `@CurrentUserName` 注解，那么该参数的值就是当前登录的用户名，像下面这样：
```java
@RestController
public class HelloController {
    @GetMapping("/hello")
    public String hello(@CurrentUserName String name) {
        return "hello "+name;
    }
}
```
要实现这个功能，非常 easy，首先自定义一个 `@CurrentUserName` 注解，如下：
```java
@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.PARAMETER)
public @interface CurrentUserName {
}
```
这个注解没啥好解释的。<br />接下来我们自定义参数解析器 `CurrentUserNameHandlerMethodArgumentResolver`，如下：
```java
public class CurrentUserNameHandlerMethodArgumentResolver implements HandlerMethodArgumentResolver {
    @Override
    public boolean supportsParameter(MethodParameter parameter) {
        return parameter.getParameterType().isAssignableFrom(String.class)&&parameter.hasParameterAnnotation(CurrentUserName.class);
    }

    @Override
    public Object resolveArgument(MethodParameter parameter, ModelAndViewContainer mavContainer, NativeWebRequest webRequest, WebDataBinderFactory binderFactory) throws Exception {
        User user = (User) SecurityContextHolder.getContext().getAuthentication().getPrincipal();
        return user.getUsername();
    }
}
```

- `supportsParameter`：如果参数类型是 String，并且参数上有 `@CurrentUserName` 注解，则使用该参数解析器。
- `resolveArgument`：该方法的返回值就是参数的具体值，当前登录用户名从 `SecurityContextHolder` 中获取即可。

最后，再将自定义的参数解析器配置到 HandlerAdapter 中，配置方式如下：
```java
@Configuration
public class WebConfig implements WebMvcConfigurer {
    @Override
    public void addArgumentResolvers(List<HandlerMethodArgumentResolver> resolvers) {
        resolvers.add(new CurrentUserNameHandlerMethodArgumentResolver());
    }
}
```
至此，就算配置完成了。<br />接下来启动项目，用户登录成功后，访问 /hello 接口，就可以看到返回当前登录用户数据了。<br />这就是自定义的一个参数类型解析器。可以看到，非常 Easy。<br />在 SpringMVC 中，默认也有很多 `HandlerMethodArgumentResolver` 的实现类，他们处理的问题也都类似，再举个例子。
<a name="fNBBQ"></a>
## 2、`PrincipalMethodArgumentResolver`
如果在项目中使用了 Spring Security，可以通过如下方式获取当前登录用户信息：
```java
@GetMapping("/hello2")
public String hello2(Principal principal) {
    return "hello " + principal.getName();
}
```
即直接在当前接口的参数中添加 `Principal` 类型的参数即可，该参数描述了当前登录用户信息。<br />那么这个功能是怎么实现的呢？当然就是 `PrincipalMethodArgumentResolver` 在起作用了！<br />一起来看下这个参数解析器：
```java
public class PrincipalMethodArgumentResolver implements HandlerMethodArgumentResolver {

    @Override
    public boolean supportsParameter(MethodParameter parameter) {
        return Principal.class.isAssignableFrom(parameter.getParameterType());
    }

    @Override
    public Object resolveArgument(MethodParameter parameter, @Nullable ModelAndViewContainer mavContainer,
                                  NativeWebRequest webRequest, @Nullable WebDataBinderFactory binderFactory) throws Exception {

        HttpServletRequest request = webRequest.getNativeRequest(HttpServletRequest.class);
        if (request == null) {
            throw new IllegalStateException("Current request is not of type HttpServletRequest: " + webRequest);
        }

        Principal principal = request.getUserPrincipal();
        if (principal != null && !parameter.getParameterType().isInstance(principal)) {
            throw new IllegalStateException("Current user principal is not of type [" +
                                            parameter.getParameterType().getName() + "]: " + principal);
        }

        return principal;
    }

}
```

- `supportsParameter`：这个方法主要是判断参数类型是不是 `Principal`，如果参数类型是 `Principal`，就支持。
- `resolveArgument`：这个方法的逻辑很简单，首先获取原生的请求，再从请求中获取 `Principal` 对象返回即可。

是不是很简单，有了这个，就可以随时加载到当前登录用户信息了。
<a name="wNPS4"></a>
## 3、`RequestParamMapMethodArgumentResolver`
再举个例子：
```java
@RestController
public class HelloController {
    @PostMapping("/hello")
    public void hello(@RequestParam MultiValueMap map) throws IOException {
        //省略...
    }
}
```
这个接口很多小伙伴可能都写过，使用 Map 去接收前端传来的参数，那么这里用到的参数解析器就是 `RequestParamMapMethodArgumentResolver`。
```java
public class RequestParamMapMethodArgumentResolver implements HandlerMethodArgumentResolver {

    @Override
    public boolean supportsParameter(MethodParameter parameter) {
        RequestParam requestParam = parameter.getParameterAnnotation(RequestParam.class);
        return (requestParam != null && Map.class.isAssignableFrom(parameter.getParameterType()) &&
                !StringUtils.hasText(requestParam.name()));
    }

    @Override
    public Object resolveArgument(MethodParameter parameter, @Nullable ModelAndViewContainer mavContainer,
                                  NativeWebRequest webRequest, @Nullable WebDataBinderFactory binderFactory) throws Exception {

        ResolvableType resolvableType = ResolvableType.forMethodParameter(parameter);

        if (MultiValueMap.class.isAssignableFrom(parameter.getParameterType())) {
            // MultiValueMap
            Class<?> valueType = resolvableType.as(MultiValueMap.class).getGeneric(1).resolve();
            if (valueType == MultipartFile.class) {
                MultipartRequest multipartRequest = MultipartResolutionDelegate.resolveMultipartRequest(webRequest);
                return (multipartRequest != null ? multipartRequest.getMultiFileMap() : new LinkedMultiValueMap<>(0));
            }
            else if (valueType == Part.class) {
                HttpServletRequest servletRequest = webRequest.getNativeRequest(HttpServletRequest.class);
                if (servletRequest != null && MultipartResolutionDelegate.isMultipartRequest(servletRequest)) {
                    Collection<Part> parts = servletRequest.getParts();
                    LinkedMultiValueMap<String, Part> result = new LinkedMultiValueMap<>(parts.size());
                    for (Part part : parts) {
                        result.add(part.getName(), part);
                    }
                    return result;
                }
                return new LinkedMultiValueMap<>(0);
            }
            else {
                Map<String, String[]> parameterMap = webRequest.getParameterMap();
                MultiValueMap<String, String> result = new LinkedMultiValueMap<>(parameterMap.size());
                parameterMap.forEach((key, values) -> {
                    for (String value : values) {
                        result.add(key, value);
                    }
                });
                return result;
            }
        }

        else {
            // Regular Map
            Class<?> valueType = resolvableType.asMap().getGeneric(1).resolve();
            if (valueType == MultipartFile.class) {
                MultipartRequest multipartRequest = MultipartResolutionDelegate.resolveMultipartRequest(webRequest);
                return (multipartRequest != null ? multipartRequest.getFileMap() : new LinkedHashMap<>(0));
            }
            else if (valueType == Part.class) {
                HttpServletRequest servletRequest = webRequest.getNativeRequest(HttpServletRequest.class);
                if (servletRequest != null && MultipartResolutionDelegate.isMultipartRequest(servletRequest)) {
                    Collection<Part> parts = servletRequest.getParts();
                    LinkedHashMap<String, Part> result = CollectionUtils.newLinkedHashMap(parts.size());
                    for (Part part : parts) {
                        if (!result.containsKey(part.getName())) {
                            result.put(part.getName(), part);
                        }
                    }
                    return result;
                }
                return new LinkedHashMap<>(0);
            }
            else {
                Map<String, String[]> parameterMap = webRequest.getParameterMap();
                Map<String, String> result = CollectionUtils.newLinkedHashMap(parameterMap.size());
                parameterMap.forEach((key, values) -> {
                    if (values.length > 0) {
                        result.put(key, values[0]);
                    }
                });
                return result;
            }
        }
    }

}
```

- `supportsParameter`：参数类型是 Map，并且使用了 `@RequestParam` 注解，并且 `@RequestParam` 注解中没有配置 name 属性，就可以使用该参数解析器。
- `resolveArgument`：具体解析分为两种情况：`MultiValueMap` 和其他 Map，前者中又分三种情况：`MultipartFile`、Part 或者其他普通请求，前两者可以处理文件上传，第三个就是普通参数。如果是普通 Map，则直接获取到原始请求参数放到一个 Map 集合中返回即可。
<a name="bz6CX"></a>
## 4、小结
前面和大家聊的都是几种简单的情况，还有复杂的如 `PathVariableMethodArgumentResolver` 和 `RequestParamMethodArgumentResolver` 。
