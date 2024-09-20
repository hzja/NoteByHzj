JavaSpringMVC
<a name="M2n7P"></a>
## 目录

- 1、先了解下 SpringMVC 常用的 10 组件
   - 1.1、`DispatcherServlet`：前端控制器
   - 1.2、`HandlerMapping`：处理器映射器
   - 1.3、`HandlerExecutionChain`：处理器执行链
   - 1.4、`handler`：处理器
   - 1.5、`HandlerAdapter`：处理器适配器
   - 1.6、`ModelAndView`：模型和视图
   - 1.7、`ViewResolver`：视图解析器
   - 1.8、View：视图
   - 1.9、`HandlerExceptionResolver`：处理器异常解析器
   - 1.10、`HttpMessageConverter`：http 报文转换器
- 2、处理流程：源码解析
   - 2.1、请求到达入口：`doDispatch`
   - 2.2、①：解析 multipart 类型的请求
   - 2.3、②：根据请求获取 `HandlerExecutionChain` 对象
   - 2.4、③：根据处理器获取 `HandlerAdapter`
   - 2.5、④：调用拦截器的 `preHandle` 方法
   - 2.6、⑤：调用 handler 实际处理请求，获取 `ModelAndView` 对象
   - 2.7、⑥：调用拦截器的 `postHandle` 方法
   - 2.8、⑦：渲染视图
- 3、处理流程：纯文字描述
<a name="MjL6w"></a>
## 1、先了解下 SpringMVC 常用的 10 组件
<a name="Ktqgj"></a>
### 1.1、DispatcherServlet：前端控制器
这个大家是最熟悉的，是一个 servlet，是 SpringMVC 处理请求的入口，不需要咱们开发，由框架提供。<br />作用：统一处理请求和响应，整个流程控制的中心，由它来调用其他组件处理用户的请求。
<a name="cjAGZ"></a>
### 1.2、`HandlerMapping`：处理器映射器
作用：根据请求的信息（如 url、method、header 等）查找请求处理器，即找到自定义的 controller 中处理请求的方法。<br />`HandlerMapping` 接口源码如下，getHandler：根据请求查找请求处理器，会返回一个 `HandlerExecutionChain` 对象。
```java
public interface HandlerMapping {
    HandlerExecutionChain getHandler(HttpServletRequest request) throws Exception;
}
```
常见的实现类：

- `RequestMappingHandlerMapping`：请求映射处理器映射，用来处理`@RequestMapping` 定义的处理器的
<a name="iLusg"></a>
### 1.3、`HandlerExecutionChain`：处理器执行链
`HandlerMapping#getHandler` 方法会根据请求得到一个 `HandlerExecutionChain` 对象。<br />`HandlerExecutionChain` 源码如下，主要包含了 3 个信息

- `handler`：请求处理器，通常就是自定义的 controller 对象及方法
- `interceptorList`：拦截器，当前请求匹配到的拦截器列表
- `interceptorIndex`：拦截器索引，用来记录执行到第几个拦截器了
```java
public class HandlerExecutionChain {

    private final Object handler;

    private final List<HandlerInterceptor> interceptorList = new ArrayList<>();

    private int interceptorIndex = -1;

}
```
<a name="Hphj3"></a>
### 1.4、`handler`：处理器
通常需要自己开发，一般指自定义的 controller，在 `DispatcherServlet` 的控制下 handler 对具体的请求进行处理。
<a name="ujBzJ"></a>
### 1.5、`HandlerAdapter`：处理器适配器
他负责对 handler 的方法进行调用，由于 handler 的类型可能有很多种，每种 handler 的调用过程可能不一样，此时就需要用到适配器 `HandlerAdapter`，适配器对外暴露了统一的调用方式（见其 handle 方法），内部将 handler 的调用过程屏蔽了，`HandlerAdapter` 接口源码如下，主要有 2 个方法需要注意：

- `supports`：当前 `HandlerAdapter` 是否支持 handler，其内部主要就是判 `HandlerAdapter` 是否能够处理 handler 的调用
- `handle`：其内部负责调用 handler 的来处理用户的请求，返回返回一个 `ModelAndView `对象
```java
public interface HandlerAdapter {

    boolean supports(Object handler);

    @Nullable
    ModelAndView handle(HttpServletRequest request, HttpServletResponse response, Object handler) throws Exception;

}
```
常见的实现类：

- `RequestMappingHandlerAdapter`：其内部用来调用`@RequestMapping` 标注的方法
<a name="mAhBU"></a>
### 1.6、`ModelAndView`：模型和视图
这个对象中主要用来存放视图的名称和共享给客户端的数据。
```java
public class ModelAndView {

    /*视图*/
    @Nullable
    private Object view;

    /*模型，用来存放共享给客户端的数据*/
    @Nullable
    private ModelMap model;

}
```
<a name="yrHJL"></a>
### 1.7、`ViewResolver`：视图解析器
这个是框架提供的，不需要咱们自己开发，它负责视图解析，根据视图的名称得到对应的视图对象（View）。<br />`ViewResolver` 接口源码
```java
public interface ViewResolver {

    @Nullable
    View resolveViewName(String viewName, Locale locale) throws Exception;

}
```
这个接口有很多实现类，比如 jsp 的、freemarker、thymeleaf 的等，他们都有各自对应的 `ViewResolver`。<br />而比较常的实现类是`InternalResourceViewResolver`，再回顾一下这个类的配置，如下
```xml
<!-- 添加视图解析器 -->
<bean id="viewResolver" class="org.springframework.web.servlet.view.InternalResourceViewResolver">
  <property name="prefix" value="/WEB-INF/view/"/>
  <property name="suffix" value=".jsp"/>
</bean>
```
`**InternalResourceViewResolver**`** 比较重要，这里说下这个类的 **`**resolveViewName**`** 方法获取视图的过程**，大家也可以去阅读`InternalResourceViewResolver#resolveViewName`方法获得，大致的过程如下：<br />step1：判断视图 viewName 是否以`redirect:`开头，如果是，则返回`RedirectView`类型的视图对象，`RedirectView` 是用来重定向的，`RedirectView` 内部用到的是`response.sendRedirect(url)`进行页面重定向；否则继续向下 step2<br />step2：判断 `viewName` 是否以forward:开头，如果是，则返回`InternalResourceView`类型的视图对象，`InternalResourceView` 是用来做跳转的，`InternalResourceView` 内部用到的是`request.getRequestDispatcher(path).forward(request, response)`进行页面跳转；否则继续向下 step3<br />step3：判断当前项目是否存在 jstl 所需的类，如果是，则返回 JstlView 类型的视图，否则返回 `InternalResourceView` 类型的视图，这两个视图的 render 方法最终会通过`request.getRequestDispatcher(path).forward(request, response)`进行页面的跳转，跳转的路径是：`InternalResourceViewResolver` 的前缀 prefix + viewName+`InternalResourceViewResolver` 的后缀 prefix
<a name="T5xJU"></a>
### 1.8、View：视图
负责将结果展示给用户，View 接口源码如下，render 方法根据指定的模型数据（model）渲染视图，即 render 方法负责将结果输出给客户端。
```java
public interface View {
    void render(@Nullable Map<String, ?> model, HttpServletRequest request, HttpServletResponse response)
    throws Exception;
}
```
View 接口常见的 2 个实现类

- `**RedirectView**`：负责重定向的，内部通过`response.sendRedirect(url)`进行页面重定向
- `**InternalResourceViewResolver**`：负责页面跳转的，内部通过`request.getRequestDispatcher(path).forward(request, response)`进行页面的跳转
<a name="G4F4e"></a>
### 1.9、`HandlerExceptionResolver`：处理器异常解析器
负责处理异常的，`HandlerExceptionResolver` 接口有个`resolveException`方法，用来解析异常，返回异常情况下对应的 `ModelAndView` 对象
```java
public interface HandlerExceptionResolver {

    @Nullable
    ModelAndView resolveException(
        HttpServletRequest request, HttpServletResponse response, @Nullable Object handler, Exception ex);

}
```
<a name="Z7VdD"></a>
### 1.10、`HttpMessageConverter`：http 报文转换器
将请求报文转换为 Java 对象，或将 Java 对象转换为响应报文，在处理`@RequestBody`、`RequestEntity`、`@ResponseBody`、`ResponseEntity` 的时候会用到
```java
public interface HttpMessageConverter<T> {

    /**
     * 是否可以将请求保温读取给方法参数指定的类型
     */
    boolean canRead(Class<?> clazz, @Nullable MediaType mediaType);

    /**
     * 是否可以将响应的保温转换为方法参数指定的类型输出
     */
    boolean canWrite(Class<?> clazz, @Nullable MediaType mediaType);

    /**
     * 当前转换器支持的类型
     */
    List<MediaType> getSupportedMediaTypes();

    /**
     * 当前转换器支持的类型
     */
    default List<MediaType> getSupportedMediaTypes(Class<?> clazz) {
        return (canRead(clazz, null) || canWrite(clazz, null) ?
                getSupportedMediaTypes() : Collections.emptyList());
    }

    /**
     * 将http保温转换为给定的类型，然后返回
     */
    T read(Class<? extends T> clazz, HttpInputMessage inputMessage)
            throws IOException, HttpMessageNotReadableException;

    /**
     * 将给定的对象t，转换为http报文输出到客户端
     */
    void write(T t, @Nullable MediaType contentType, HttpOutputMessage outputMessage)
            throws IOException, HttpMessageNotWritableException;

}
```
<a name="igr5C"></a>
## 2、处理流程：源码解析
<a name="SWjFz"></a>
### 2.1、请求到达入口：`doDispatch`
SpringMVC 的所有请求，最终都会到达`org.springframework.web.servlet.DispatcherServlet#doDispatch`这个方法，整个请求的大致处理过程都在这个方法中，咱们从这个方法开始分析，源码如下，大家注意代码中的注释，带有标号，比如 ①、②、③ 这样需要的注释，大家需要注意了，这些是关键的步骤，稍后会对这些步骤做详细的说明
```java
protected void doDispatch(HttpServletRequest request, HttpServletResponse response) throws Exception {
    //请求对象
    HttpServletRequest processedRequest = request;
    //处理器执行链对象
    HandlerExecutionChain mappedHandler = null;
    boolean multipartRequestParsed = false;

    //获取异步处理管理器，servlet3.0后支持异步处理，可以在子线程中响应用户请求
    WebAsyncManager asyncManager = WebAsyncUtils.getAsyncManager(request);

    try {
        //模型和视图
        ModelAndView mv = null;
        //异常对象
        Exception dispatchException = null;

        try {
            //①：解析multipart类型的请求，上传文件用的就是multipart类型的请求方式
            processedRequest = checkMultipart(request);
            //用来标记是否是multipart类型的请求
            multipartRequestParsed = (processedRequest != request);

            //②：根据请求获取HandlerExecutionChain对象
            mappedHandler = getHandler(processedRequest);
            //如果没有找到处理器，就404了
            if (mappedHandler == null) {
                noHandlerFound(processedRequest, response);
                return;
            }

            //③：根据处理器获取HandlerAdapter
            HandlerAdapter ha = getHandlerAdapter(mappedHandler.getHandler());

            //④：调用拦截器的preHandle方法，若返回false，处理结束
            if (!mappedHandler.applyPreHandle(processedRequest, response)) {
                return;
            }

            //⑤：调用handler实际处理请求，获取ModelAndView对象，这里会调用HandlerAdapter#handle方法处理请求，其内部会调用handler来处理具体的请求
            mv = ha.handle(processedRequest, response, mappedHandler.getHandler());

            //判断异步请求不是已经开始了，开始了就返回了
            if (asyncManager.isConcurrentHandlingStarted()) {
                return;
            }
            //如果mv对象中没有视图 & DispatcherServlet配置了默认的视图，则给mv安排一个默认的视图
            applyDefaultViewName(processedRequest, mv);

            //⑥：调用拦截器的postHandle方法
            mappedHandler.applyPostHandle(processedRequest, response, mv);
        }
        catch (Exception ex) {
            dispatchException = ex;
        }
        catch (Throwable err) {
            dispatchException = new NestedServletException("Handler dispatch failed", err);
        }
        //⑦：处理分发结果，渲染视图(包含了正常处理和异常情况的处理)，将结果输出到客户端
        processDispatchResult(processedRequest, response, mappedHandler, mv, dispatchException);
    }
    catch (Exception ex) {
        //⑧：调用拦截器的afterCompletion方法
        triggerAfterCompletion(processedRequest, response, mappedHandler, ex);
    }
    catch (Throwable err) {
        //⑧：调用拦截器的afterCompletion方法
        triggerAfterCompletion(processedRequest, response, mappedHandler,
                new NestedServletException("Handler processing failed", err));
    }
    finally {
        //对于异步处理的情况，调用异步处理的拦截器AsyncHandlerInterceptor的afterConcurrentHandlingStarted方法
        if (asyncManager.isConcurrentHandlingStarted()) {
            if (mappedHandler != null) {
                mappedHandler.applyAfterConcurrentHandlingStarted(processedRequest, response);
            }
        }
        else {
            //对于multipart的请求，清理资源，比如文件上传的请求，在上传的过程中文件会被保存到临时文件中，这里就会对这些文件继续清理
            if (multipartRequestParsed) {
                cleanupMultipart(processedRequest);
            }
        }
    }
}
```
下面来对上面带有编号的步骤进行分析。
<a name="u2Kc7"></a>
### 2.2、①：解析 `multipart` 类型的请求
```java
//①：解析multipart类型的请求，上传文件用的就是multipart类型的请求方式
processedRequest = checkMultipart(request);
```
`checkMultipart(request)`源码
```java
protected HttpServletRequest checkMultipart(HttpServletRequest request) throws MultipartException {
    //判断multipartResolver解析器是否存在 && 请求是否是multipart类型
    if (this.multipartResolver != null && this.multipartResolver.isMultipart(request)) {
        //将请求转换为multipart类型的请求对象，通常为MultipartHttpServletRequest类型
        return this.multipartResolver.resolveMultipart(request);
    }
    return request;
}
```
<a name="YnhtY"></a>
### 2.3、②：根据请求获取 `HandlerExecutionChain` 对象
```java
//②：根据请求获取HandlerExecutionChain对象
mappedHandler = getHandler(processedRequest);
```
`getHandler(processedRequest)`源码如下，遍历所有的处理器映射器`HandlerMapping`，调用他们的`getHandler`方法得到能够处理当前请求的`HandlerExecutionChain`对象，这个对象中包含了 3 个信息

- `handler`：请求处理器，通常就是自定义的 controller 对象及方法
- `interceptorList`：拦截器，当前请求匹配到的拦截器列表
- `interceptorIndex`：拦截器索引，用来记录执行到第几个拦截器了
```java
protected HandlerExecutionChain getHandler(HttpServletRequest request) throws Exception {
    if (this.handlerMappings != null) {
        for (HandlerMapping mapping : this.handlerMappings) {
            HandlerExecutionChain handler = mapping.getHandler(request);
            if (handler != null) {
                return handler;
            }
        }
    }
    return null;
}
```
有兴趣的可以去看一下`RequestMappingHandlerMapping`这个类的源码，也是最常用的一个 `HandlerMapping`，它会根据@RequestMapping来找到能够处当前请求的处理器，`RequestMappingHandlerMapping#getHandler` 方法查找得到的 `HandlerExecutionChain` 对象中的 handler 类型为`HandlerMethod`，代码在下面这个位置
```
org.springframework.web.servlet.handler.AbstractHandlerMethodMapping#getHandlerInternal
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683910612906-0f3b4f8c-64de-4d29-a853-9757f3382962.png#averageHue=%23fcfaf8&clientId=uf8dd0d40-b960-4&from=paste&id=u72c3b216&originHeight=280&originWidth=920&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u1f25ab78-6092-4eba-b0ae-1a4e8bcb2db&title=)<br />`HandlerMethod` 对象中包含了能够处理请求的 bean 及方法信息<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683910612965-5b686615-806d-4c71-8d12-68d86d81561a.png#averageHue=%23fdfcfb&clientId=uf8dd0d40-b960-4&from=paste&id=u8127a8fa&originHeight=540&originWidth=855&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue7b51724-aa9f-4d88-9e50-747ffdebfe5&title=)
<a name="zJT5n"></a>
### 2.4、③：根据处理器获取 `HandlerAdapter`
```java
//③：根据处理器获取HandlerAdapter
HandlerAdapter ha = getHandlerAdapter(mappedHandler.getHandler());
```
`getHandlerAdapter`方法源码，遍历`HandlerAdapter`列表，找到能够处理当前 handler 的`HandlerAdapter`，如果没找到会报错
```java
protected HandlerAdapter getHandlerAdapter(Object handler) throws ServletException {
    if (this.handlerAdapters != null) {
        for (HandlerAdapter adapter : this.handlerAdapters) {
            if (adapter.supports(handler)) {
                return adapter;
            }
        }
    }

    throw new ServletException("No adapter for handler [" + handler +
                               "]: The DispatcherServlet configuration needs to include a HandlerAdapter that supports this handler");
}
```
此方法通常返回的是`RequestMappingHandlerAdapter`类型的对象，`RequestMappingHandlerAdapter`这个类会根据`HandlerMethod`提供的信息，通过反射调用`@RequestMapping` 标注的方法。
<a name="yBcoE"></a>
### 2.5、④：调用拦截器的 `preHandle` 方法
```java
//④：调用拦截器的preHandle方法，若返回false，处理结束
if (!mappedHandler.applyPreHandle(processedRequest, response)) {
    return;
}
```
`mappedHandler.applyPreHandle`源码如下，主要干了 3 个事情

- 循环调用拦截器的`preHandle`方法
- 如果某个拦截器的`preHandle`方法返回 false，则反向依次调用那些 `preHandle` 方法返回 ture 的拦截器的 `afterCompletion` 方法；这句话有点绕，比如有 3 个拦截器，1、2 的 `preHandler` 返回了 true，而 3 返回的是 false，那么这里将按照 2、1 的顺序调用他们的 `afterCompletion` 方法
- 记录拦截器的执行位置
```java
boolean applyPreHandle(HttpServletRequest request, HttpServletResponse response) throws Exception {
    for (int i = 0; i < this.interceptorList.size(); i++) {
        HandlerInterceptor interceptor = this.interceptorList.get(i);
        //调用拦截器的preHandle方法
        if (!interceptor.preHandle(request, response, this.handler)) {
            //如果拦截器返回false，则反向依次调用那些preHandle方法返回ture的拦截器的afterCompletion方法
            triggerAfterCompletion(request, response, null);
            return false;
        }
        //记录当前拦截器执行的位置
        this.interceptorIndex = i;
    }
    return true;
}
```
`triggerAfterCompletion`方法源码如下，通过拦截器当前执行的位置`interceptorIndex`逆向调用拦截器的`afterCompletion`方法
```java
void triggerAfterCompletion(HttpServletRequest request, HttpServletResponse response, @Nullable Exception ex) {
    for (int i = this.interceptorIndex; i >= 0; i--) {
        HandlerInterceptor interceptor = this.interceptorList.get(i);
        try {
            interceptor.afterCompletion(request, response, this.handler, ex);
        }
        catch (Throwable ex2) {
            logger.error("HandlerInterceptor.afterCompletion threw exception", ex2);
        }
    }
}
```
<a name="s3l2A"></a>
### 2.6、⑤：调用 `handler` 实际处理请求，获取 `ModelAndView` 对象
<a name="zYVJS"></a>
#### 2.6.1、过程
```java
//⑤：调用handler实际处理请求，获取ModelAndView对象，这里会调用HandlerAdapter#handle方法处理请求，其内部会调用handler来处理具体的请求
mv = ha.handle(processedRequest, response, mappedHandler.getHandler());
```
`ha.handler` 方法内部通通过程会走到`RequestMappingHandlerAdapter#invokeHandlerMethod`方法，这个方法内部会通过反射调用`@RequestMapping` 标注的方法，这个方法内部代码比较复杂，就不进去了，这里说一下这个方法主要做了 3 个非常重要的事情：

- step1：组装目标方法需要的参数
- step2：通过反射调用处理请求的目标方法，获取方法的返回值
- step3：对方法的返回值进行处理

下面来细说一下这 3 个步骤，这些地方有好东西，大家集中注意力了。
<a name="jE3hw"></a>
#### 2.6.2、step1：组装目标方法需要的参数：`HandlerMethodArgumentResolver`
处理器的方法需要的参数有各种类型的，所以组装这些参数是比较关键的地方，组装参数的源码位于下面这个位置
```
org.springframework.web.method.support.InvocableHandlerMethod#getMethodArgumentValues
```
获取方法需要的参数值，会用到`HandlerMethodArgumentResolver`这个对象，叫做：处理器方法参数解析器，用来解析请求，得到方法需要的参数，大家看一下这个接口，源码如下，主要有 2 个方法

- `supportsParameter`：是否能够解析 parameter 指定的参数
- `resolveArgument`：通过请求和 parameter 参数解析得到参数的值
```java
public interface HandlerMethodArgumentResolver {

    //判断当前解析器是否能处理这个parameter这个参数，也就是说是否能够将请求中的数据转换为parameter指定的参数的值
    boolean supportsParameter(MethodParameter parameter);

    //解析参数：从http请求中解析出控制器需要的参数的值
    Object resolveArgument(MethodParameter parameter, @Nullable ModelAndViewContainer mavContainer,
                           NativeWebRequest webRequest, @Nullable WebDataBinderFactory binderFactory) throws Exception;

}
```
这个接口有很多实现类，列几个比较熟悉的，当大家想知道 SpringMVC 可以接收哪些类型的参数，以及这些参数有什么特点的时候，可以去看看这些类的源码，会秒懂的

| 实现类 | 对应的控制器参数 | 说明 |
| --- | --- | --- |
| `PathVariableMapMethodArgumentResolver` | `@PathVariable` 标注参数 | 从 url 中提取参数的值 |
| `RequestHeaderMethodArgumentResolver` | `@RequestHeader` 标注参数 | 从 http 头中提取参数值 |
| `RequestParamMethodArgumentResolver` | `@RequestParam` 标注参数 | http 请求参数中获取值 |
| `RequestResponseBodyMethodProcessor` | `@RequestBody` 标注参数 | 提取 body 数据，转换为参数类型 |
| `ServletResponseMethodArgumentResolver` | `ServletResponse`、`OutputStream`、`Writer` 这 3 种类型的参数 | 这几种类型用来控制 http 请求的响应输出流 |
| `HttpEntityMethodProcessorHttpEntity` | `HttpEntity` 类型的参数 | `HttpEntity` 中包含了 http 请求头和 body 的所有信息 |
| `ExpressionValueMethodArgumentResolver` | `@Value` 标注的参数 | spel 表达式，从 spring 容器中获取值 |
| `MapMethodProcessor` | 参数为 `Map` 或者子类型 | - |
| `ModelMethodProcessor` | 参数为 `org.springframework.ui.Model` 或子类型 | - |
| `ModelAttributeMethodProcessor` | `@ModelAttribute` 标注的参数 | - |

<a name="Mybfe"></a>
#### 2.6.3、step2：通过反射调用目标方法
也就是调用 Controller 中的`@RequestMapping` 标注的方法，代码位置
```
org.springframework.web.servlet.mvc.method.annotation.ServletInvocableHandlerMethod#invokeAndHandle
```
对应的源码如下，这个方法 SpringMVC 框架中主要有 2 个地方会调用

- 第 1 个地方是：调用处理请求的实际方法的时候
- 第 2 个地方是：方法有异常的时候，异常解析器里面也会用到这个方法，稍后后面会讲
```java
public void invokeAndHandle(ServletWebRequest webRequest, ModelAndViewContainer mavContainer,
                            Object... providedArgs) throws Exception {
    //1.通过反射调用目标方法，内部会组装目标方法需要的参数
    Object returnValue = invokeForRequest(webRequest, mavContainer, providedArgs);

    //如果返回值为空，表示目标方法中已经完成了请求的所有处理，表示请求处理结束了，将执行mavContainer.setRequestHandled(true)标记请求处理完毕
    if (returnValue == null) {
        if (isRequestNotModified(webRequest) || getResponseStatus() != null || mavContainer.isRequestHandled()) {
            mavContainer.setRequestHandled(true);
            return;
        }
    }
        //若getResponseStatusReason()不为空，表示请求已经处理过了
    else if (StringUtils.hasText(getResponseStatusReason())) {
        mavContainer.setRequestHandled(true);
        return;
    }
    //走到这里，说明有返回值，标记请求未处理完毕
    mavContainer.setRequestHandled(false);
    //对返回值进行处理
    this.returnValueHandlers.handleReturnValue(
        returnValue, getReturnValueType(returnValue), mavContainer, webRequest);
}
```
<a name="SssNI"></a>
#### 2.6.4、step3：处理方法返回值：`HandlerMethodReturnValueHandler`
大家注意，上面代码中这部分代码，如下，会对反射调用的结果 `returnValue` 进行处理
```java
//对返回值进行处理
this.returnValueHandlers.handleReturnValue(
    returnValue, getReturnValueType(returnValue), mavContainer, webRequest);
```
进入`handleReturnValue`方法内部去看一下，最终代码在下面这个位置
```
org.springframework.web.method.support.HandlerMethodReturnValueHandlerComposite#handleReturnValue
```
这个方法的源码如下
```java
public void handleReturnValue(@Nullable Object returnValue, MethodParameter returnType,
                              ModelAndViewContainer mavContainer, NativeWebRequest webRequest) throws Exception {
    //根据返回值找到HandlerMethodReturnValueHandler
    HandlerMethodReturnValueHandler handler = selectHandler(returnValue, returnType);
    if (handler == null) {
        throw new IllegalArgumentException("Unknown return value type: " + returnType.getParameterType().getName());
    }
    //调用HandlerMethodReturnValueHandler#handleReturnValue处理返回值
    handler.handleReturnValue(returnValue, returnType, mavContainer, webRequest);
}

@Nullable
private HandlerMethodReturnValueHandler selectHandler(@Nullable Object value, MethodParameter returnType) {
    //根据返回值判断是否是异步请求
    boolean isAsyncValue = isAsyncReturnValue(value, returnType);
    for (HandlerMethodReturnValueHandler handler : this.returnValueHandlers) {
        if (isAsyncValue && !(handler instanceof AsyncHandlerMethodReturnValueHandler)) {
            continue;
        }
        if (handler.supportsReturnType(returnType)) {
            return handler;
        }
    }
    return null;
}
```
这里关键的信息要看`HandlerMethodReturnValueHandler`接口，这个接口用来处理返回值，看一下其源码，包含 2 个方法

- `supportsReturnType`：是否能够处理 `returnType` 参数指定的返回值
- `handleReturnValue`：处理返回值
```java
public interface HandlerMethodReturnValueHandler {

    boolean supportsReturnType(MethodParameter returnType);

    void handleReturnValue(@Nullable Object returnValue, MethodParameter returnType,
                           ModelAndViewContainer mavContainer, NativeWebRequest webRequest) throws Exception;

}
```
此接口有很多实现类，如下图，图下的表格中会列出常见的一些及说明，建议大家抽空，都点开看看其源码<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683910630973-3e68abd8-0d02-4fd9-adab-4e6caf6edfdf.png#averageHue=%23fcfadc&clientId=uf8dd0d40-b960-4&from=paste&id=uf3dc4f49&originHeight=513&originWidth=804&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5d73689b-ca73-4c41-b9c4-29aad22147c&title=)

| 实现类 | 说明 |
| --- | --- |
| `ViewNameMethodReturnValueHandler` | 返回值为视图名称时的解析器 |
| `MapMethodProcessor` | 返回值为 Map 的解析器 |
| `StreamingResponseBodyReturnValueHandler` | 返回值为 `ResponseEntity` 类型时的解析器 |
| `DeferredResultMethodReturnValueHandler` | 返回值为 `DeferredResult` 类型时的解析器，表示异步请求 |
| `CallableMethodReturnValueHandler` | 返回值为 Callable 类型时的解析器，表示异步请求 |
| `ModelMethodProcessor` | 返回值为 Model 类型时的解析器 |
| `ModelAndViewMethodReturnValueHandler` | 返回值为 `ModelAndView` 类型时的解析器 |
| `RequestResponseBodyMethodProcessor` | 方法上标注有`@ResponseBody` 注解时返回值的解析器 |
| `HttpEntityMethodProcessor` | 返回值为 `HttpEntity` 类型但是非 `RequestEntity` 类型时的解析器 |

这里找一个比较有代表性的，带大家看一下，就以`RequestResponseBodyMethodProcessor`来说一下，这个会处理`@RequestBody`标注的方法，抽取其 2 个关键方法的代码，如下
```java
//判断类上或者目标方法上是否有@ResponseBody注解
@Override
public boolean supportsReturnType(MethodParameter returnType) {
    return (AnnotatedElementUtils.hasAnnotation(returnType.getContainingClass(), ResponseBody.class) ||
            returnType.hasMethodAnnotation(ResponseBody.class));
}

//处理返回值
@Override
public void handleReturnValue(@Nullable Object returnValue, MethodParameter returnType,
                              ModelAndViewContainer mavContainer, NativeWebRequest webRequest)
        throws IOException, HttpMediaTypeNotAcceptableException, HttpMessageNotWritableException {
    //1：标注为请求已处理，因为当前handleReturnValue方法会直接将结果输出到客户端，所以后续就不需要再进行视图渲染了，表示请求已经被处理了
    mavContainer.setRequestHandled(true);
    ServletServerHttpRequest inputMessage = createInputMessage(webRequest);
    ServletServerHttpResponse outputMessage = createOutputMessage(webRequest);

    //2：将结果输出到客户端
    writeWithMessageConverters(returnValue, returnType, inputMessage, outputMessage);
}
```
上面代码中，这里大家需要注意`handleReturnValue`方法，这个方法内部会直接将结果输出，后续就没有视图渲染的事情了，所以这里会调用`mavContainer.setRequestHandled(true)`，表示请求已经处理了。
<a name="nDRns"></a>
### 2.7、⑥：调用拦截器的 `postHandle` 方法
```java
//⑥：调用拦截器的postHandle方法
mappedHandler.applyPostHandle(processedRequest, response, mv);
```
`mappedHandler.applyPostHandle`源码如下，逆序调用拦截器的`postHandle`方法`org.springframework.web.servlet.HandlerExecutionChain#applyPostHandle`
```java
void applyPostHandle(HttpServletRequest request, HttpServletResponse response, @Nullable ModelAndView mv)
    throws Exception {

    for (int i = this.interceptorList.size() - 1; i >= 0; i--) {
        HandlerInterceptor interceptor = this.interceptorList.get(i);
        interceptor.postHandle(request, response, this.handler, mv);
    }
}
```
<a name="cj74W"></a>
### 2.8、⑦：渲染视图
<a name="EdT9J"></a>
#### 2.8.1、过程
```java
//⑦：处理分发结果，渲染视图(包含了正常处理和异常情况的处理)，将结果输出到客户端
processDispatchResult(processedRequest, response, mappedHandler, mv, dispatchException);
```
`processDispatchResult`源码如下
```java
// org.springframework.web.servlet.DispatcherServlet#processDispatchResult

private void processDispatchResult(HttpServletRequest request, HttpServletResponse response,
                                   @Nullable HandlerExecutionChain mappedHandler, @Nullable ModelAndView mv,
                                   @Nullable Exception exception) throws Exception {
    boolean errorView = false;

    if (exception != null) {
        Object handler = (mappedHandler != null ? mappedHandler.getHandler() : null);
        //⑦-1：如果有异常，进行全局异常处理
        mv = processHandlerException(request, response, handler, exception);
        errorView = (mv != null);
    }

    if (mv != null && !mv.wasCleared()) {
        //⑦-2：渲染视图
        render(mv, request, response);
        if (errorView) {
            //调用request.removeAttribute方法清理request中错误信息
            WebUtils.clearErrorRequestAttributes(request);
        }
    }

    if (mappedHandler != null) {
        //⑦-3：调用拦截器的afterCompletion方法
        mappedHandler.triggerAfterCompletion(request, response, null);
    }
}
```
这个方法主要干了 3 个事情

- step1：⑦-1：如果有异常，进行全局异常处理
- step2：⑦-2：渲染视图
- step3：⑦-3：调用拦截器的 `afterCompletion` 方法

下面来解析这 3 个步骤
<a name="r9Xun"></a>
#### 2.8.2、step1：⑦-1：如果有异常，进行全局异常处理
```java
if (exception != null) {
    Object handler = (mappedHandler != null ? mappedHandler.getHandler() : null);
    //⑦-1：如果有异常，进行全局异常处理
    mv = processHandlerException(request, response, handler, exception);
    errorView = (mv != null);
}
```
`processHandlerException`方法源码，主要是遍历异常处理器`HandlerExceptionResolver`的`resolveException`来处理异常，稍后会说一下这个接口
```java
// org.springframework.web.servlet.DispatcherServlet#processHandlerException

protected ModelAndView processHandlerException(HttpServletRequest request, HttpServletResponse response,
                                               @Nullable Object handler, Exception ex) throws Exception {

    // 调用处理器异常解析器解析异常，得到ModelAndView
    ModelAndView exMv = null;
    if (this.handlerExceptionResolvers != null) {
        for (HandlerExceptionResolver resolver : this.handlerExceptionResolvers) {
            exMv = resolver.resolveException(request, response, handler, ex);
            if (exMv != null) {
                break;
            }
        }
    }
    if (exMv != null) {
        //暴露异常信息到request对象中（request.setAttribute）
        WebUtils.exposeErrorRequestAttributes(request, ex, getServletName());
        return exMv;
    }

    throw ex;
}
```
`HandlerExceptionResolver` 接口：处理器异常解析器，内部就只有一个方法，用来解析异常的，得到一个 `ModelAndView` 对象。
```java
public interface HandlerExceptionResolver {

    @Nullable
    ModelAndView resolveException(
        HttpServletRequest request, HttpServletResponse response, @Nullable Object handler, Exception ex);

}
```
这个接口有好几个实现类，主要关注下`ExceptionHandlerExceptionResolver`这个类，注解方式处理全局异常（即使用`@ControllerAdvice` 和`@ExceptionHandler` 实现全局异常处理处理），最终这俩注解定义的异常处理会被`ExceptionHandlerExceptionResolver`这个类进行处理，这个类的源码就不细讲了，比较简单，大家可以去看看，就是一个异常类型匹配处理方法的过程。
<a name="oCtoF"></a>
#### 2.8.3、step2：⑦-2：渲染视图
```java
//⑦-2：渲染视图
render(mv, request, response);
```
`render`方法源码如下
```java
// org.springframework.web.servlet.DispatcherServlet#render

protected void render(ModelAndView mv, HttpServletRequest request, HttpServletResponse response) throws Exception {
    View view;
    String viewName = mv.getViewName();
    if (viewName != null) {
        //⑦-2-1：调用视图解析器解析视图名称得到视图View对象
        view = resolveViewName(viewName, mv.getModelInternal(), locale, request);
    } else {
        view = mv.getView();
    }

    //⑦-2-2：调用视图的render方法渲染视图，将结果输出到客户端
    view.render(mv.getModelInternal(), request, response);
}
```
此方法干了 2 件事

- ⑦-2-1：调用视图解析器解析视图名称得到视图 View 对象
- ⑦-2-2：调用视图的 `render` 方法渲染视图，将结果输出到客户端

下面进去细看一下
<a name="qltmc"></a>
##### ⑦-2-1：调用视图解析器解析视图名称得到视图 View 对象
```java
//⑦-2-1：调用视图解析器解析视图名称得到视图View对象
view = resolveViewName(viewName, mv.getModelInternal(), locale, request);
```
`resolveViewName`方法源码如下，遍历视图解析器，解析视图名称，得到视图对象 View
```java
protected View resolveViewName(String viewName, @Nullable Map<String, Object> model,
                               Locale locale, HttpServletRequest request) throws Exception {

    if (this.viewResolvers != null) {
        for (ViewResolver viewResolver : this.viewResolvers) {
            View view = viewResolver.resolveViewName(viewName, locale);
            if (view != null) {
                return view;
            }
        }
    }
    return null;
}
```
<a name="ZEoMG"></a>
##### ⑦-2-2：调用视图的 render 方法渲染视图，将结果输出到客户端
```java
//⑦-2-2：调用视图的render方法渲染视图，将结果输出到客户端
view.render(mv.getModelInternal(), request, response);
```
这里以 `InternalResourceView` 为例，进到其 render 方法中，看看里面干了什么，最终会进到其`renderMergedOutputModel`方法中，源码如下，这里代码就非常亲切了，不多解释，看注释
```java
protected void renderMergedOutputModel(
    Map<String, Object> model, HttpServletRequest request, HttpServletResponse response) throws Exception {

    // 将model中的数据遍历后放在request中（request.setAttribute(name,value)）
    exposeModelAsRequestAttributes(model, request);

    // 获取跳转的页面的路径
    String dispatcherPath = prepareForRendering(request, response);

    // 调用request.getRequestDispatcher(path)得到RequestDispatcher对象
    RequestDispatcher rd = getRequestDispatcher(request, dispatcherPath);

    //实现页面跳转
    if (useInclude(request, response)) {
        rd.include(request, response);
    }else {
        rd.forward(request, response);
    }
}
```
<a name="o5jM5"></a>
#### 2.8.3、step3：⑦-3：调用拦截器的 `afterCompletion` 方法
```java
⑦-3：调用拦截器的afterCompletion方法
mappedHandler.triggerAfterCompletion(request, response, null);
```
`mappedHandler.triggerAfterCompletion`方法的源码如下，反向调用拦截器的`afterCompletion`方法
```java
void triggerAfterCompletion(HttpServletRequest request, HttpServletResponse response, @Nullable Exception ex) {
    for (int i = this.interceptorIndex; i >= 0; i--) {
        HandlerInterceptor interceptor = this.interceptorList.get(i);
        try {
            interceptor.afterCompletion(request, response, this.handler, ex);
        }
        catch (Throwable ex2) {
            logger.error("HandlerInterceptor.afterCompletion threw exception", ex2);
        }
    }
}
```
过程到这里就结束了，需要大家结合源码多看几遍，还是比较容易的。
<a name="HIEEU"></a>
## 3、处理流程：纯文字描述
1、用户向服务器发送请求，请求被 SpringMVC 前端控制器 `DispatcherServlet` 捕获<br />2、`DispatcherServlet` 根据该 URI，调用 `HandlerMapping` 获得该 Handler 配置的所有相关的对象（包括 Handler 对象以及 Handler 对象对应的拦截器），最后以 `HandlerExecutionChain` 执行链对象的形式返回<br />4、`DispatcherServlet` 根据获得的 Handler，选择一个合适的 `HandlerAdapter`<br />5、如果成功获得 `HandlerAdapter`，此时将开始执行拦截器的 `preHandler(…)`方法【正向】<br />6、提取 Request 中的模型数据，填充 Handler 入参，开始执行 Handler（Controller)方法，处理请求，在填充 Handler 的入参过程中，根据配置，Spring 将做一些额外的工作：

1. `HttpMessageConveter`：将请求消息（如 Json、xml 等数据）转换成一个对象，将对象转换为指定的类型信息
2. 数据转换：对请求消息进行数据转换。如 String 转换成 Integer、Double 等
3. 数据格式化：对请求消息进行数据格式化。如将字符串转换成格式化数字或格式化日期等
4. 数据验证：验证数据的有效性（长度、格式等），验证结果存储到 `BindingResult` 或 Error 中

7、Handler 执行完成后，向 `DispatcherServlet` 返回一个 `ModelAndView` 对象。<br />8、此时将开始执行拦截器的 `postHandle(...)`方法【逆向】<br />9、根据返回的 `ModelAndView`（此时会判断是否存在异常：如果存在异常，则执行 `HandlerExceptionResolver` 进行异常处理）选择一个适合的 `ViewResolver` 进行视图解析，根据 Model 和 View，来渲染视图<br />10、渲染视图完毕执行拦截器的 `afterCompletion(…)`方法【逆向】<br />11、将渲染结果返回给客户端
