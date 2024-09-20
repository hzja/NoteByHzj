Java SpringBoot
<a name="vHM0z"></a>
### `@RestController`和`@Controller`注解
都知道RestController默认都只提供Rest风格接口返回值，针对不需要返回页面的Controller都采用RestController进行注解，下面根据源码简单分析一下两者处理上的区别。<br />`@RestController`源码如下。
```java
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@Documented
@Controller
@ResponseBody
public @interface RestController {
   /**
    * The value may indicate a suggestion for a logical component name,
    * to be turned into a Spring bean in case of an autodetected component.
    * @return the suggested component name, if any
    * @since 4.0.1
    */
   String value() default "";
}
```
`@RestController`的编写方式依赖注解组合，`@RestController`被`@Controller`和`@ResponseBody`标注，表示`@RestController`具有两者的注解语义，因此在注解处理时`@RestController`比`@Controller`多具有一个`@ResponseBody`语义，这就是`@RestController`和`@Controller`的区别，也是`@RestController`的返回值为何都是经过转换的json的原因。<br />所以小结就是：`@RestController` = `@Controller` + `@ResponseBody`；
<a name="pvbxh"></a>
### `@ResponseBody`注解的处理过程
既然知道`@RestController`与`@Controller`的区别是多了一个`@ResponseBody`语义，不妨了解一下`@ResponseBody`的处理过程。<br />首先，可以知道，`@ResponseBody`是一个针对方法返回值进行处理的注解。如果熟悉Spring MVC处理过程的话，可以知道在根据`requesturl`映射获取到`HandlerMethod`之后，根据`HandlerMethod`调度请求方法的对象是`HandlerAdapter`，方法调用结束，返回值处理的调度对象也是`HandlerAdapter`。所以，`@ResponseBody`注解的处理应该也是在`HandlerAdapter`中完成。<br />在`RequestMappingHandlerAdapter#invokeHandlerMethod`方法里面，有下面几句比较重要的代码
```java
//创建方法调用对象
ServletInvocableHandlerMethod invocableMethod = createInvocableHandlerMethod(handlerMethod);
//......
//设置返回值处理器
invocableMethod.setHandlerMethodReturnValueHandlers(this.returnValueHandlers);
//......
//调用方法
invocableMethod.invokeAndHandle(webRequest, mavContainer);
```
`returnValueHandlers`这个变量听名字就像返回值处理，实际上也是对返回值处理的处理器集合。首先创建一个调用方法的对象，然后注入处理器，最后调用方法，这就是完整的一个流程。<br />可以再分析一下这里面的处理器初始化时有没有针对`@ResponseBody`注解的处理器。
<a name="ZmfyC"></a>
### `@ResponseBody`注解处理器初始化
搜索一下`returnValueHandlers`初始化的地方，可以看到是这么个调用链：

- `RequestMappingHandlerAdapter#afterPropertiesSet`
   - `handlers = RequestMappingHandlerAdapter#getDefaultReturnValueHandlers`
```java
returnValueHandlers = new HandlerMethodReturnValueHandlerComposite().addHandlers(handlers)
```
所以是在`RequestMappingHandlerAdapter`的bean初始化完成时，就会进行返回值处理器的初始化，在`RequestMappingHandlerAdapter#getDefaultReturnValueHandlers`方法内部执行，代码如下。
```java
private List<HandlerMethodReturnValueHandler> getDefaultReturnValueHandlers() {
	List<HandlerMethodReturnValueHandler> handlers = new ArrayList<HandlerMethodReturnValueHandler>();
	
	// Single-purpose return value types
	handlers.add(new ModelAndViewMethodReturnValueHandler());
	handlers.add(new ModelMethodProcessor());
	handlers.add(new ViewMethodReturnValueHandler());
	handlers.add(new ResponseBodyEmitterReturnValueHandler(getMessageConverters()));
	handlers.add(new StreamingResponseBodyReturnValueHandler());
	handlers.add(new HttpEntityMethodProcessor(getMessageConverters(),
											   this.contentNegotiationManager, this.requestResponseBodyAdvice));
	handlers.add(new HttpHeadersReturnValueHandler());
	handlers.add(new CallableMethodReturnValueHandler());
	handlers.add(new DeferredResultMethodReturnValueHandler());
	handlers.add(new AsyncTaskMethodReturnValueHandler(this.beanFactory));
	
	// Annotation-based return value types
	handlers.add(new ModelAttributeMethodProcessor(false));
	//@ResponseBody注解处理器
	handlers.add(new RequestResponseBodyMethodProcessor(getMessageConverters(),
														this.contentNegotiationManager, this.requestResponseBodyAdvice));
	
	// Multi-purpose return value types
	handlers.add(new ViewNameMethodReturnValueHandler());
	handlers.add(new MapMethodProcessor());
	// Custom return value types
	if (getCustomReturnValueHandlers() != null) {
		handlers.addAll(getCustomReturnValueHandlers());
	}
	
	// Catch-all
	if (!CollectionUtils.isEmpty(getModelAndViewResolvers())) {
		handlers.add(new ModelAndViewResolverMethodReturnValueHandler(getModelAndViewResolvers()));
	}
	else {
		handlers.add(new ModelAttributeMethodProcessor(true));
	}
	return handlers;
}
```
可以看到非常对处理器，`RequestResponseBodyMethodProcessor`就是`@ResponseBody`的处理器。
<a name="lGQtC"></a>
### `@ResponseBody`注解处理器调用
进入调用方法`invocableMethod.invokeAndHandle(webRequest, mavContainer)/ServletInvocableHandlerMethod#invokeAndHandle`，继续进行调用，跟踪调用链如下。

- `ServletInvocableHandlerMethod#invokeAndHandle`
   - `this.returnValueHandlers.handleReturnValue/HandlerMethodReturnValueHandlerComposite#handleReturnValue`

`HandlerMethodReturnValueHandlerComposite#handleReturnValue`代码如下所示。
```java
public void handleReturnValue(Object returnValue, MethodParameter returnType,
      ModelAndViewContainer mavContainer, NativeWebRequest webRequest) throws Exception {
//选择一个合适的处理器
   HandlerMethodReturnValueHandler handler = selectHandler(returnValue, returnType);
   if (handler == null) {
      throw new IllegalArgumentException("Unknown return value type: " + returnType.getParameterType().getName());
   }
   //处理返回值
   handler.handleReturnValue(returnValue, returnType, mavContainer, webRequest);
}
```
所以基本就是从所有处理器中选出目标处理器，处理返回值。进入`HandlerMethodReturnValueHandlerComposite#selectHandler`
```java
private HandlerMethodReturnValueHandler selectHandler(Object value, MethodParameter returnType) {
    boolean isAsyncValue = isAsyncReturnValue(value, returnType);
    for (HandlerMethodReturnValueHandler handler : this.returnValueHandlers) {
        if (isAsyncValue && !(handler instanceof AsyncHandlerMethodReturnValueHandler)) {
            continue;
        }
        //判断处理器是否支持
        if (handler.supportsReturnType(returnType)) {
            return handler;
        }
    }
    return null;
}
```
`RequestResponseBodyMethodProcessor#supportsReturnType`，代码如下。
```java
public boolean supportsReturnType(MethodParameter returnType) {
   return (AnnotatedElementUtils.hasAnnotation(returnType.getContainingClass(), ResponseBody.class) ||
         returnType.hasMethodAnnotation(ResponseBody.class));
}
```
明显如果类上有`@ResponseBody`或者方法上有的话，就能适配处理器，`@RestController`具有`@ResponseBody`语义能够适配，所以进行`RequestResponseBodyMethodProcessor#handleReturnValue`进行返回值处理。
