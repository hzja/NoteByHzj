JavaSpringMVC<br />Spring MVC中，通过组合使用注解`@ControllerAdvice`和其他一些注解，可以为开发人员实现的控制器类做一些全局性的定制，具体来讲，可作如下定制 :

- 结合`@ExceptionHandler`使用 ==> 添加统一的异常处理控制器方法
- 结合`@ModelAttribute`使用 ==> 使用共用方法添加渲染视图的数据模型属性
- 结合`@InitBinder`使用 ==> 使用共用方法初始化控制器方法调用使用的数据绑定器数据绑定器涉及到哪些参数/属性需要/不需要绑定，设置数据类型转换时使用的`PropertyEditor`，`Formatter`等。

那么，`@ControllerAdvice`的工作原理又是怎样的呢 ？
<a name="l8Q8B"></a>
## 1、注解`@ControllerAdvice`是如何被发现的 ?
首先，容器启动时，会定义类型为`RequestMappingHandlerAdapter`的bean组件，这是`DispatcherServlet`用于执行控制器方法的`HandlerAdapter`，它实现了接口`InitializingBean`，所以自身在初始化时其方法`afterPropertiesSet`会被调用执行。
```java
@Override
public void afterPropertiesSet() {
    // Do this first, it may add ResponseBody advice beans
    initControllerAdviceCache();

    // 省略掉无关代码 
    // ...
}
```
从以上代码可以看出，`RequestMappingHandlerAdapter` bean组件在自身初始化时调用了`initControllerAdviceCache`，从这个方法的名字上就可以看出，这是一个`ControllerAdvice`相关的初始化函数，而`initControllerAdviceCache`具体又做了什么呢？继续来看 :
```java
private void initControllerAdviceCache() {
    if (getApplicationContext() == null) {
        return;
    }

    // 找到所有使用了注解 @ControllerAdvice 的bean组件 
    List<ControllerAdviceBean> adviceBeans = ControllerAdviceBean.findAnnotatedBeans(getApplicationContext());
    // 排序
    AnnotationAwareOrderComparator.sort(adviceBeans);


    // this. requestResponseBodyAdvice : 
    //   用于记录所有 @ControllerAdvice + RequestBodyAdvice/ResponseBodyAdvice bean 
    // this.modelAttributeAdviceCache : 
    //   用于记录所有 @ControllerAdvice bean组件中的 @ModuleAttribute 方法
    // this.initBinderAdviceCache : 
    //  用于记录所有 @ControllerAdvice bean组件中的 @InitBinder 方法
    // 用于临时记录所有 @ControllerAdvice + RequestResponseBodyAdvice bean
    List<Object> requestResponseBodyAdviceBeans = new ArrayList<>();

    // 遍历每个使用了注解 @ControllerAdvice 的 bean 组件
    for (ControllerAdviceBean adviceBean : adviceBeans) {
        Class<?> beanType = adviceBean.getBeanType();
        if (beanType == null) {
            throw new IllegalStateException("Unresolvable type for ControllerAdviceBean: " + adviceBean);
        }
        // 获取当前  ControllerAdviceBean 中所有使用了 @ModelAttribute 注解的方法
        Set<Method> attrMethods = MethodIntrospector.selectMethods(beanType, MODEL_ATTRIBUTE_METHODS);
        if (!attrMethods.isEmpty()) {
            this.modelAttributeAdviceCache.put(adviceBean, attrMethods);
        }

        // 获取当前 ControllerAdviceBean 中所有使用了 @InitMethod 注解的方法
        Set<Method> binderMethods = MethodIntrospector.selectMethods(beanType, INIT_BINDER_METHODS);
        if (!binderMethods.isEmpty()) {
            this.initBinderAdviceCache.put(adviceBean, binderMethods);
        }
        // 如果当前 ControllerAdviceBean 继承自 RequestBodyAdvice，将其登记到 requestResponseBodyAdviceBeans
        if (RequestBodyAdvice.class.isAssignableFrom(beanType)) {
            requestResponseBodyAdviceBeans.add(adviceBean);
        }

        // 如果当前 ControllerAdviceBean 继承自 ResponseBodyAdvice，将其登记到 requestResponseBodyAdviceBeans  
        if (ResponseBodyAdvice.class.isAssignableFrom(beanType)) {
            requestResponseBodyAdviceBeans.add(adviceBean);
        }
    }

    if (!requestResponseBodyAdviceBeans.isEmpty()) {
        this.requestResponseBodyAdvice.addAll(0, requestResponseBodyAdviceBeans);
    }

    if (logger.isDebugEnabled()) {
        int modelSize = this.modelAttributeAdviceCache.size();
        int binderSize = this.initBinderAdviceCache.size();
        int reqCount = getBodyAdviceCount(RequestBodyAdvice.class);
        int resCount = getBodyAdviceCount(ResponseBodyAdvice.class);
        if (modelSize == 0 && binderSize == 0 && reqCount == 0 && resCount == 0) {
            logger.debug("ControllerAdvice beans: none");
        }
        else {
            logger.debug("ControllerAdvice beans: " + modelSize + " @ModelAttribute, " + binderSize +
                         " @InitBinder, " + reqCount + " RequestBodyAdvice, " + resCount + " ResponseBodyAdvice");
        }
    }
}
```
从以上#`initControllerAdviceCache`方法的实现逻辑来看，它将容器中所有使用了注解`@ControllerAdvice`的bean或者其方法都分门别类做了统计，记录到了`RequestMappingHandlerAdapter`实例的三个属性中 :

- `requestResponseBodyAdvice`
- 用于记录所有`@ControllerAdvice` + `RequestBodyAdvice`/`ResponseBodyAdvice` bean组件
- `modelAttributeAdviceCache`
- 用于记录所有 `@ControllerAdvice` bean组件中的 `@ModuleAttribute` 方法
- `initBinderAdviceCache`
- 用于记录所有`@ControllerAdvice` bean组件中的 `@InitBinder` 方法

到此为止，使用注解`@ControllerAdvice`的bean中的信息被提取出来了，但是，这些信息又是怎么使用的呢 ？继续来看。
<a name="qNvYY"></a>
## 2、`@ControllerAdvice` 定义信息的使用
<a name="IlVkm"></a>
### 2.1 `requestResponseBodyAdvice`的使用
```java
/**
 * Return the list of argument resolvers to use including built-in resolvers
 * and custom resolvers provided via {@link #setCustomArgumentResolvers}.
 */
private List<HandlerMethodArgumentResolver> getDefaultArgumentResolvers() {
    List<HandlerMethodArgumentResolver> resolvers = new ArrayList<>();

    // ... 省略无关代码        
    resolvers.add(new RequestResponseBodyMethodProcessor(getMessageConverters(), 
                                                         this.requestResponseBodyAdvice));
    // ... 省略无关代码
    resolvers.add(new RequestPartMethodArgumentResolver(getMessageConverters(), 
                                                        this.requestResponseBodyAdvice));  
    // ... 省略无关代码
    resolvers.add(new HttpEntityMethodProcessor(getMessageConverters(), 
                                                this.requestResponseBodyAdvice));
    // ... 省略无关代码

    return resolvers;
}
```
#`getDefaultArgumentResolvers`方法用于准备`RequestMappingHandlerAdapter`执行控制器方法过程中缺省使用的`HandlerMethodArgumentResolver`，从上面代码可见，`requestResponseBodyAdvice`会被传递给`RequestResponseBodyMethodProcessor`/`RequestPartMethodArgumentResolver`/`HttpEntityMethodProcessor`这三个参数解析器，不难猜测，它们在工作时会使用到该`requestResponseBodyAdvice`，但具体怎么使用，为避免过深细节影响理解，这里不继续展开。<br />方法`getDefaultArgumentResolvers`也在`RequestMappingHandlerAdapter`初始化方法中被调用执行，如下所示 :
```java
@Override
public void afterPropertiesSet() {
    // Do this first, it may add ResponseBody advice beans
    initControllerAdviceCache();

    if (this.argumentResolvers == null) {
        List<HandlerMethodArgumentResolver> resolvers = getDefaultArgumentResolvers(); // <==
        this.argumentResolvers = new HandlerMethodArgumentResolverComposite()
            .addResolvers(resolvers);
    }

    // 省略无关代码
}
```
<a name="DunBd"></a>
### 2.2 `modelAttributeAdviceCache`的使用
```java
private ModelFactory getModelFactory(HandlerMethod handlerMethod, WebDataBinderFactory binderFactory) {
    SessionAttributesHandler sessionAttrHandler = getSessionAttributesHandler(handlerMethod);
    Class<?> handlerType = handlerMethod.getBeanType();
    Set<Method> methods = this.modelAttributeCache.get(handlerType);
    if (methods == null) {
        // 获取当前控制器类中使用了 @ModelAttribute 的方法
        methods = MethodIntrospector.selectMethods(handlerType, MODEL_ATTRIBUTE_METHODS);
        this.modelAttributeCache.put(handlerType, methods);
    }
    List<InvocableHandlerMethod> attrMethods = new ArrayList<>();
    // Global methods first
    // 遍历@ControllerAdvice bean中所有使用了 @ModelAttribute 的方法，
    // 将其包装成 InvocableHandlerMethod 放到 attrMethods
    // ********* 这里就是 modelAttributeAdviceCache 被使用到的地方了 ************
    this.modelAttributeAdviceCache.forEach((clazz, methodSet) -> {
        if (clazz.isApplicableToBeanType(handlerType)) {
            Object bean = clazz.resolveBean();
            for (Method method : methodSet) {
                attrMethods.add(createModelAttributeMethod(binderFactory, bean, method));
            }
        }
    });

    // 遍历当前控制器类中中所有使用了 @ModelAttribute 的方法，
    // 也将其包装成 InvocableHandlerMethod 放到 attrMethods        
    for (Method method : methods) {
        Object bean = handlerMethod.getBean();
        attrMethods.add(createModelAttributeMethod(binderFactory, bean, method));
    }

    // 此时  attrMethods 包含了两类 InvocableHandlerMethod， 分别来自于 :
    // 1. @ControllerAdvice bean 中所有使用了 @ModelAttribute 的方法
    // 2. 当前控制器类中中所有使用了 @ModelAttribute 的方法
    return new ModelFactory(attrMethods, binderFactory, sessionAttrHandler);
}

// 从指定 bean 的方法 method ，其实是一个使用了注解 @ModelAttribute 的方法，
// 构造一个 InvocableHandlerMethod 对象
private InvocableHandlerMethod createModelAttributeMethod(WebDataBinderFactory factory, 
                                                          Object bean, Method method) {
    InvocableHandlerMethod attrMethod = new InvocableHandlerMethod(bean, method);
    if (this.argumentResolvers != null) {
        attrMethod.setHandlerMethodArgumentResolvers(this.argumentResolvers);
    }
    attrMethod.setParameterNameDiscoverer(this.parameterNameDiscoverer);
    attrMethod.setDataBinderFactory(factory);
    return attrMethod;
}
```
从此方法可以看到，`getModelFactory`方法使用到了`modelAttributeAdviceCache`，它会根据其中每个元素构造成一个`InvocableHandlerMethod`，最终传递给要创建的`ModelFactory`对象。而`getModelFactory`又在什么时候被使用呢 ? 它会在`RequestMappingHandlerAdapter`执行一个控制器方法的准备过程中被调用，如下所示 :
```java
@Nullable
protected ModelAndView invokeHandlerMethod(HttpServletRequest request,
                                           HttpServletResponse response, HandlerMethod handlerMethod) throws Exception {

    ServletWebRequest webRequest = new ServletWebRequest(request, response);
    try {
        // 构造调用 handlerMethod 所要使用的数据绑定器工厂  
        WebDataBinderFactory binderFactory = getDataBinderFactory(handlerMethod);
        // 构造调用 handlerMethod 所要使用的数据模型工厂  
        ModelFactory modelFactory = getModelFactory(handlerMethod, binderFactory);
        // 省略无关代码 ...            
    }
```
<a name="M5grz"></a>
### 2.3 `initBinderAdviceCache`的使用
```java
private WebDataBinderFactory getDataBinderFactory(HandlerMethod handlerMethod) throws Exception {
    Class<?> handlerType = handlerMethod.getBeanType();
    Set<Method> methods = this.initBinderCache.get(handlerType);
    if (methods == null) {
        // 获取当前控制器类中使用了 @InitBinder 的方法  
        methods = MethodIntrospector.selectMethods(handlerType, INIT_BINDER_METHODS);
        this.initBinderCache.put(handlerType, methods);
    }
    List<InvocableHandlerMethod> initBinderMethods = new ArrayList<>();
    // Global methods first
    // 遍历@ControllerAdvice bean中所有使用了 @InitBinder 的方法，
    // 将其包装成 InvocableHandlerMethod 放到 initBinderMethods
    // ********* 这里就是 initBinderAdviceCache 被使用到的地方了 ************        
    this.initBinderAdviceCache.forEach((clazz, methodSet) -> {
        if (clazz.isApplicableToBeanType(handlerType)) {
            Object bean = clazz.resolveBean();
            for (Method method : methodSet) {
                initBinderMethods.add(createInitBinderMethod(bean, method));
            }
        }
    });

    // 遍历当前控制器类中所有使用了 @InitBinder 的方法，
    // 将其包装成 InvocableHandlerMethod 放到 initBinderMethods        
    for (Method method : methods) {
        Object bean = handlerMethod.getBean();
        initBinderMethods.add(createInitBinderMethod(bean, method));
    }

    // 此时  initBinderMethods 包含了两类 InvocableHandlerMethod， 分别来自于 :
    // 1. @ControllerAdvice bean 中所有使用了 @InitBinder 的方法
    // 2. 当前控制器类中中所有使用了 @InitBinder 的方法        
    return createDataBinderFactory(initBinderMethods);
}

/ 从指定 bean 的方法 method ，其实是一个使用了注解 @InitBinder 的方法，
    / 构造一个 InvocableHandlerMethod 对象
    private InvocableHandlerMethod createInitBinderMethod(Object bean, Method method) {
        InvocableHandlerMethod binderMethod = new InvocableHandlerMethod(bean, method);
        if (this.initBinderArgumentResolvers != null) {
            binderMethod.setHandlerMethodArgumentResolvers(this.initBinderArgumentResolvers);
        }
        binderMethod.setDataBinderFactory(new DefaultDataBinderFactory(this.webBindingInitializer));
        binderMethod.setParameterNameDiscoverer(this.parameterNameDiscoverer);
        return binderMethod;
    }

/**
* Template method to create a new InitBinderDataBinderFactory instance.
* <p>The default implementation creates a ServletRequestDataBinderFactory.
* This can be overridden for custom ServletRequestDataBinder subclasses.
* @param binderMethods {@code @InitBinder} methods
* @return the InitBinderDataBinderFactory instance to use
* @throws Exception in case of invalid state or arguments
*/
protected InitBinderDataBinderFactory createDataBinderFactory(
    List<InvocableHandlerMethod> binderMethods)
    throws Exception {

    return new ServletRequestDataBinderFactory(binderMethods, getWebBindingInitializer());
}
```
从此方法可以看到，`getDataBinderFactory`方法使用到了`initBinderAdviceCache`，它会根据其中每个元素构造成一个`InvocableHandlerMethod`，最终传递给要创建的`InitBinderDataBinderFactory`对象。而`getDataBinderFactory`又在什么时候被使用呢 ? 它会在`RequestMappingHandlerAdapter`执行一个控制器方法的准备过程中被调用，如下所示 :
```java
@Nullable
protected ModelAndView invokeHandlerMethod(HttpServletRequest request,
                                           HttpServletResponse response, HandlerMethod handlerMethod) throws Exception {

    ServletWebRequest webRequest = new ServletWebRequest(request, response);
    try {
        // 构造调用 handlerMethod 所要使用的数据绑定器工厂  
        WebDataBinderFactory binderFactory = getDataBinderFactory(handlerMethod);
        // 构造调用 handlerMethod 所要使用的数据模型工厂  
        ModelFactory modelFactory = getModelFactory(handlerMethod, binderFactory);
        // 省略无关代码 ...            
    }
```
到此为止，基本上可以看到，通过`@ControllerAdvice`注解的bean组件所定义的`@ModelAttribute`/`@InitBinder`方法，或者`RequestBodyAdvice`/`ResponseBodyAdvice`，是如何被`RequestMappingHandlerAdapter`提取和使用的了。虽然并未深入到更细微的组件研究它们最终的使用，不过结合这些组件命名以及这些更深一层的使用者组件的名称，即便是猜测，相信你也不难理解猜到它们如何被使用了。<br />不知道你注意到没有，关于`@ControllerAdvice`和`@ExceptionHandler`这一组合，在上面提到的`RequestMappingHandlerAdapter`逻辑中，并未涉及到。那如果使用了这种组合，又会是怎样一种工作机制呢 ？事实上，`@ControllerAdvice`和`@ExceptionHandler`这一组合所做的定义，会被`ExceptionHandlerExceptionResolver`消费使用。
