JavaSpringBoot<br />介绍一个 Spring Boot 中的组件--`HandlerMethodReturnValueHandler`。<br />对请求/响应数据进行预处理/二次处理，使用 `ResponseBodyAdvice` 和 `RequestBodyAdvice`。其中 `ResponseBodyAdvice` 可以实现对响应数据的二次处理，可以在这里对响应数据进行加密/包装等等操作。不过这不是唯一的方案，介绍一种更加灵活的方案--`HandlerMethodReturnValueHandler`，一起来看看下。
<a name="EYS8r"></a>
## 1、`HandlerMethodReturnValueHandler`
`HandlerMethodReturnValueHandler` 的作用是对处理器的处理结果再进行一次二次加工，这个接口里边有两个方法：
```java
public interface HandlerMethodReturnValueHandler {
    boolean supportsReturnType(MethodParameter returnType);
    void handleReturnValue(@Nullable Object returnValue, MethodParameter returnType,
                           ModelAndViewContainer mavContainer, NativeWebRequest webRequest) throws Exception;
}
```

- `supportsReturnType`：这个处理器是否支持相应的返回值类型。
- `handleReturnValue`：对方法返回值进行处理。

`HandlerMethodReturnValueHandler` 有很多默认的实现类，来看下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1670851884097-d1e6e47b-6b90-43fa-acec-37678eb81905.png#averageHue=%23faf9e0&clientId=u7798d61c-75b6-4&from=paste&id=ubd9a450b&originHeight=800&originWidth=692&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2803accd-4d20-4e58-9145-161cbd147de&title=)<br />接下来来把这些实现类的作用捋一捋：
<a name="g1b0z"></a>
### `ViewNameMethodReturnValueHandler`
这个处理器用来处理返回值为 void 和 String 的情况。如果返回值为 void，则不做任何处理。如果返回值为 String，则将 String 设置给 `mavContainer` 的 `viewName` 属性，同时判断这个 String 是不是重定向的 String，如果是，则设置 `mavContainer` 的 `redirectModelScenario` 属性为 true，这是处理器返回重定向视图的标志。
<a name="TGgJe"></a>
### `ViewMethodReturnValueHandler`
这个处理器用来处理返回值为 View 的情况。如果返回值为 View，则将 View 设置给 `mavContainer` 的 view 属性，同时判断这个 View 是不是重定向的 View，如果是，则设置 `mavContainer` 的 `redirectModelScenario` 属性为 true，这是处理器返回重定向视图的标志。
<a name="evMzR"></a>
### `MapMethodProcessor`
这个处理器用来处理返回值类型为 Map 的情况，具体的处理方案就是将 map 添加到 `mavContainer` 的 model 属性中。
<a name="hZdNq"></a>
### `StreamingResponseBodyReturnValueHandler`
这个用来处理 `StreamingResponseBody` 或者 `ResponseEntity<StreamingResponseBody>` 类型的返回值。
<a name="kHQu5"></a>
### `DeferredResultMethodReturnValueHandler`
这个用来处理 `DeferredResult`、`ListenableFuture` 以及 `CompletionStage` 类型的返回值，用于异步请求。
<a name="jMuNq"></a>
### `CallableMethodReturnValueHandler`
处理 Callable 类型的返回值，也是用于异步请求。
<a name="RIrwl"></a>
### `HttpHeadersReturnValueHandler`
这个用来处理 `HttpHeaders` 类型的返回值，具体处理方式就是将 `mavContainer` 中的 `requestHandled` 属性设置为 true，该属性是请求是否已经处理完成的标志（如果处理完了，就到此为止，后面不会再去找视图了），然后将 `HttpHeaders` 添加到响应头中。
<a name="GGpU4"></a>
### `ModelMethodProcessor`
这个用来处理返回值类型为 Model 的情况，具体的处理方式就是将 Model 添加到 `mavContainer` 的 model 上。
<a name="tyfOz"></a>
### `ModelAttributeMethodProcessor`
这个用来处理添加了 `@ModelAttribute` 注解的返回值类型，如果 `annotaionNotRequired` 属性为 true，也可以用来处理其他非通用类型的返回值。
<a name="L0qrz"></a>
### `ServletModelAttributeMethodProcessor`
同上，该类只是修改了参数解析方式。
<a name="AnPgG"></a>
### `ResponseBodyEmitterReturnValueHandler`
这个用来处理返回值类型为 `ResponseBodyEmitter` 的情况。
<a name="P6n30"></a>
### `ModelAndViewMethodReturnValueHandler`
这个用来处理返回值类型为 `ModelAndView` 的情况，将返回值中的 Model 和 View 分别设置到 `mavContainer` 的相应属性上去。
<a name="IUFjO"></a>
### `ModelAndViewResolverMethodReturnValueHandler`
这个的 `supportsReturnType` 方法返回 true，即可以处理所有类型的返回值，这个一般放在最后兜底。
<a name="wwI3B"></a>
### `AbstractMessageConverterMethodProcessor`
这是一个抽象类，当返回值需要通过 HttpMessageConverter 进行转化的时候会用到它的子类。这个抽象类主要是定义了一些工具方法。
<a name="e7Onz"></a>
### `RequestResponseBodyMethodProcessor`
这个用来处理添加了 `@ResponseBody` 注解的返回值类型。
<a name="dDmbH"></a>
### `HttpEntityMethodProcessor`
这个用来处理返回值类型是 `HttpEntity` 并且不是 `RequestEntity` 的情况。
<a name="zrh76"></a>
### `AsyncHandlerMethodReturnValueHandler`
这是一个空接口，暂未发现典型使用场景。
<a name="qWCAi"></a>
### `AsyncTaskMethodReturnValueHandler`
这个用来处理返回值类型为 `WebAsyncTask` 的情况。
<a name="a55NJ"></a>
### `HandlerMethodReturnValueHandlerComposite`
看 `Composite` 就知道，这是一个组合处理器，没啥好说的。<br />这个就是系统默认定义的 `HandlerMethodReturnValueHandler`。<br />那么在上面的介绍中，大家看到反复涉及到一个组件 `mavContainer`，这个也要介绍一下。
<a name="J9btn"></a>
## 2、`ModelAndViewContainer`
`ModelAndViewContainer` 就是一个数据穿梭巴士，在整个请求的过程中承担着数据传送的工作，从它的名字上可以看出来它里边保存着 Model 和 View 两种类型的数据，但是实际上可不止两种，来看下 `ModelAndViewContainer` 的定义：
```java
public class ModelAndViewContainer {
    private boolean ignoreDefaultModelOnRedirect = false;
    @Nullable
    private Object view;
    private final ModelMap defaultModel = new BindingAwareModelMap();
    @Nullable
    private ModelMap redirectModel;
    private boolean redirectModelScenario = false;
    @Nullable
    private HttpStatus status;
    private final Set<String> noBinding = new HashSet<>(4);
    private final Set<String> bindingDisabled = new HashSet<>(4);
    private final SessionStatus sessionStatus = new SimpleSessionStatus();
    private boolean requestHandled = false;
}
```
把这几个属性理解了，基本上也就整明白 `ModelAndViewContainer` 的作用了：

- `defaultModel`：默认使用的 Model。当在接口参数重使用 Model、`ModelMap` 或者 Map 时，最终使用的实现类都是 `BindingAwareModelMap`，对应的也都是 `defaultModel`。
- `redirectModel`：重定向时候的 Model，如果在接口参数中使用了 `RedirectAttributes` 类型的参数，那么最终会传入 `redirectModel`。

可以看到，一共有两个 Model，两个 Model 到底用哪个呢？这个在 `getModel` 方法中根据条件返回合适的 Model：
```java
public ModelMap getModel() {
    if (useDefaultModel()) {
        return this.defaultModel;
    }
    else {
        if (this.redirectModel == null) {
            this.redirectModel = new ModelMap();
        }
        return this.redirectModel;
    }
}
private boolean useDefaultModel() {
    return (!this.redirectModelScenario || (this.redirectModel == null && !this.ignoreDefaultModelOnRedirect));
}
```
这里 `redirectModelScenario` 表示处理器是否返回 redirect 视图；`ignoreDefaultModelOnRedirect` 表示是否在重定向时忽略 `defaultModel`，所以这块的逻辑是这样：

1. 如果 `redirectModelScenario` 为 true，即处理器返回的是一个重定向视图，那么使用 `redirectModel`。如果 `redirectModelScenario` 为 false，即处理器返回的不是一个重定向视图，那么使用 `defaultModel`。
2. 如果 `redirectModel` 为 null，并且 `ignoreDefaultModelOnRedirect` 为 false，则使用 `redirectModel`，否则使用 `defaultModel`。

接下来还剩下如下一些参数：

- `view`：返回的视图。
- `status`：HTTP 状态码。
- `noBinding`：是否对 `@ModelAttribute(binding=true/false)` 声明的数据模型的相应属性进行绑定。
- `bindingDisabled`：不需要进行数据绑定的属性。
- `sessionStatus`：`SessionAttribute` 使用完成的标识。
- `requestHandled`：请求处理完成的标识（例如添加了 `@ResponseBody` 注解的接口，这个属性为 true，请求就不会再去找视图了）。

这个 `ModelAndViewContainer` 小伙伴们权且做一个了解，在后面的源码分析中还会聊到这个组件。<br />接下来也来自定义一个 `HandlerMethodReturnValueHandler`，来感受一下 `HandlerMethodReturnValueHandler` 的基本用法。
<a name="zvRFu"></a>
## 3、API 接口数据包装
假设有这样一个需求：想在原始的返回数据外面再包裹一层，举个简单例子，本来接口是下面这样：
```java
@RestController
public class UserController {
    @GetMapping("/user")
    public User getUserByUsername(String username) {
        User user = new User();
        user.setUsername(username);
        user.setAddress("www.javaboy.org");
        return user;
    }
}
```
返回的数据格式是下面这样：
```json
{"username":"java","address":"www.java.org"}
```
现在希望返回的数据格式变成下面这样：
```
{"status":"ok","data":{"username":"java","address":"www.java.org"}}
```
就这样一个简单需求，一起来看下怎么实现。
<a name="uIVhO"></a>
### 3.1 `RequestResponseBodyMethodProcessor`
在开始定义之前，先给大家介绍一下 `RequestResponseBodyMethodProcessor`，这是 `HandlerMethodReturnValueHandler` 的实现类之一，这个主要用来处理返回 JSON 的情况。<br />来稍微看下：
```java
@Override
public boolean supportsReturnType(MethodParameter returnType) {
    return (AnnotatedElementUtils.hasAnnotation(returnType.getContainingClass(), ResponseBody.class) ||
    returnType.hasMethodAnnotation(ResponseBody.class));
}
@Override
public void handleReturnValue(@Nullable Object returnValue, MethodParameter returnType,
                              ModelAndViewContainer mavContainer, NativeWebRequest webRequest)
    throws IOException, HttpMediaTypeNotAcceptableException, HttpMessageNotWritableException {
    mavContainer.setRequestHandled(true);
    ServletServerHttpRequest inputMessage = createInputMessage(webRequest);
    ServletServerHttpResponse outputMessage = createOutputMessage(webRequest);
    writeWithMessageConverters(returnValue, returnType, inputMessage, outputMessage);
}
```

- `supportsReturnType`：从这个方法中可以看到，这里支持有 `@ResponseBody` 注解的接口。
- `handleReturnValue`：这是具体的处理逻辑，首先 mavContainer 中设置 `requestHandled` 属性为 true，表示这里处理完成后就完了，以后不用再去找视图了，然后分别获取 `inputMessage` 和 `outputMessage`，调用 `writeWithMessageConverters` 方法进行输出，`writeWithMessageConverters` 方法是在父类中定义的方法，这个方法比较长，核心逻辑就是调用确定输出数据、确定 MediaType，然后通过 `HttpMessageConverter` 将 JSON 数据写出去即可。

有了上面的知识储备之后，接下来就可以自己实现了。
<a name="D9goD"></a>
### 3.2 具体实现
首先自定义一个 `HandlerMethodReturnValueHandler`：
```java
public class MyHandlerMethodReturnValueHandler implements HandlerMethodReturnValueHandler {
    private HandlerMethodReturnValueHandler handler;

    public MyHandlerMethodReturnValueHandler(HandlerMethodReturnValueHandler handler) {
        this.handler = handler;
    }

    @Override
    public boolean supportsReturnType(MethodParameter returnType) {
        return handler.supportsReturnType(returnType);
    }

    @Override
    public void handleReturnValue(Object returnValue, MethodParameter returnType, ModelAndViewContainer mavContainer, NativeWebRequest webRequest) throws Exception {
        Map<String, Object> map = new HashMap<>();
        map.put("status", "ok");
        map.put("data", returnValue);
        handler.handleReturnValue(map, returnType, mavContainer, webRequest);
    }
}
```
由于要做的功能其实是在 `RequestResponseBodyMethodProcessor` 基础之上实现的，因为支持 `@ResponseBody`，输出 JSON 那些东西都不变，只是在输出之前修改一下数据而已。所以这里直接定义了一个属性 `HandlerMethodReturnValueHandler`，这个属性的实例就是 `RequestResponseBodyMethodProcessor`，`supportsReturnType` 方法就按照 `RequestResponseBodyMethodProcessor` 的要求来，在 `handleReturnValue` 方法中，先对返回值进行一个预处理，然后调用 `RequestResponseBodyMethodProcessor#handleReturnValue` 方法继续输出 JSON 即可。<br />接下来就是配置 `MyHandlerMethodReturnValueHandler` 使之生效了。由于 SpringMVC 中 `HandlerAdapter` 在加载的时候已经配置了 `HandlerMethodReturnValueHandler`，所以可以通过如下方式对已经配置好的 `RequestMappingHandlerAdapter` 进行修改，如下：
```java
@Configuration
public class ReturnValueConfig implements InitializingBean {
    @Autowired
    RequestMappingHandlerAdapter requestMappingHandlerAdapter;
    @Override
    public void afterPropertiesSet() throws Exception {
        List<HandlerMethodReturnValueHandler> originHandlers = requestMappingHandlerAdapter.getReturnValueHandlers();
        List<HandlerMethodReturnValueHandler> newHandlers = new ArrayList<>(originHandlers.size());
        for (HandlerMethodReturnValueHandler originHandler : originHandlers) {
            if (originHandler instanceof RequestResponseBodyMethodProcessor) {
                newHandlers.add(new MyHandlerMethodReturnValueHandler(originHandler));
            }else{
                newHandlers.add(originHandler);
            }
        }
        requestMappingHandlerAdapter.setReturnValueHandlers(newHandlers);
    }
}
```
自定义 `ReturnValueConfig` 实现 `InitializingBean` 接口，`afterPropertiesSet` 方法会被自动调用，在该方法中，将 `RequestMappingHandlerAdapter` 中已经配置好的 `HandlerMethodReturnValueHandler` 拎出来挨个检查，如果类型是 `RequestResponseBodyMethodProcessor`，则重新构建，用自定义的 `MyHandlerMethodReturnValueHandler` 代替它，最后给 `requestMappingHandlerAdapter` 重新设置 `HandlerMethodReturnValueHandler` 即可。<br />最后再提供一个测试接口：
```java
@RestController
public class UserController {
    @GetMapping("/user")
    public User getUserByUsername(String username) {
        User user = new User();
        user.setUsername(username);
        user.setAddress("www.javaboy.org");
        return user;
    }
}
public class User {
    private String username;
    private String address;
    //省略其他
}
```
配置完成后，就可以启动项目啦。
