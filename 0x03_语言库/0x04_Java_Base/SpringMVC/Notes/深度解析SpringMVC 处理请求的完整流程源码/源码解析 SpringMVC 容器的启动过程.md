JavaSpringMVC<br />通过阅读源码的方式带大家了解 springmv 容器启动的过程，SpringMVC 中的各种组件都是在容器启动的过程中组装的，所以如果熟悉整个过程后，可以随意对 SpringMVC 进行扩展。
<a name="FvsdR"></a>
## 目录

- 1、前言
- 2、回顾全注解方式 2 个关键类
   - 2.1、MVC 初始化类
   - 2.2、MVC 配置类
- 2、SpringMVC 容器的生命周期(9 个阶段)
- 3、阶段 1：Servlet 容器初始化
   - 3.1、`ServletContainerInitializer`
   - 3.2、`SpringServletContainerInitializer`
   - 3.3、`SpringServletContainerInitializer#onStartup` 方法
   - 3.4、`WebApplicationInitializer`：web 应用初始化
   - 3.5、进入 `AbstractDispatcherServletInitializer#onStartup` 方法
- 4、阶段 2：创建父容器
   - 4.1、过程
   - 4.2、①：负责创建父容器
   - 4.2、②：创建 `ContextLoaderListener` 监听器
- 5、阶段 3&4：创建 SpringMVC 容器&注册 `DispatcherServlet`
- 6、阶段 5：启动父容器：`ContextLoaderListener`
   - 6.1、过程
   - 6.2、代码 ①：配置父容器以及启动父容器
   - 6.3、`ApplicationContextInitializer` 接口：容器启动前用来初始化容器
- 7、阶段 6：启动 SpringMVC 容器：`DispatcherServlet#init()`
   - 7.1、`DispatcherServlet` 类图
   - 7.2、`HttpServletBean#init()`
   - 7.3、`FrameworkServlet#initServletBean`
   - 7.4、`FrameworkServlet#initWebApplicationContext`
   - 7.5、`FrameworkServlet#configureAndRefreshWebApplicationContext`
- 8、阶段 7：SpringMVC 容器启动过程中处理`@WebMVC`
   - 8.1、SpringMVC 配置类被处理
   - 8.2、`@EnableWebMvc`：配置 SpringMVC 所需组件
   - 8.3、进入 `DelegatingWebMvcConfiguration` 类
   - 8.4、`WebMvcConfigurationSupport`：配置 SpringMVC 所需所有组件
   - 8.5、`WebMvcConfigurer` 接口
- 9、阶段 8：组装 `DispatcherServlet` 中各种 SpringMVC 需要的组件
   - 9.1、触发 `ContextRefreshListener` 监听器
   - 9.2、进入 `FrameworkServlet.this.onApplicationEvent(event);`
   - 9.3、进入 `DispatcherServlet#onRefresh`
   - 9.4、`DispatcherServlet#initStrategies`：初始化 `DispatcherServlet` 中的组件
   - 9.5、`initHandlerMappings(context);`
   - 9.6、`DispatcherServlet#getDefaultStrategies`：兜底的方案查找组件
- 10、阶段 9：销毁容器
   - 10.1、销毁 SpringMVC 容器：`DispatcherServlet#destroy`
   - 10.2、销毁父容器：`ContextLoaderListener#contextDestroyed`
<a name="frqIb"></a>
## 1、前言
SpringMVC，建议大家使用全注解的方式。
<a name="gCQ2i"></a>
## 2、回顾全注解方式 2 个关键类
全注解的方式重点就在于 2 个类：MVC 初始化类、MVC 配置类
<a name="N3w7v"></a>
### 2.1、MVC 初始化类
代码如下，这个类需要继承 `AbstractAnnotationConfigDispatcherServletInitializer`，会有 web 容器来调用，这个类中有 4 个方法需要实现，干了 4 件事情

- `getRootConfigClasses()`：获取父容器的配置类
- `getServletConfigClasses()`：获取 SpringMVC 容器的配置类，这个配置类相当于 SpringMVC xml 配置文件的功能
- `getServletMappings()`：获取 `DispatcherServlet` 能够处理的 url，相当于 web.xml 中为 servlet 指定的 url-pattern
- `getServletFilters()`：定义所有的 Filter
```java
/**
 * ①：1、创建Mvc初始化类，需要继承AbstractAnnotationConfigDispatcherServletInitializer类
 */
public class MvcInit extends AbstractAnnotationConfigDispatcherServletInitializer {
    /**
     * springmvc容器的父容器spring配置类
     * 实际工作中我们的项目比较复杂，可以将controller层放在springmvc容器中
     * 其他层，如service层、dao层放在父容器了，bean管理起来更清晰一些
     * 也可以没有父容器，将所有bean都放在springmvc容器中
     *
     * @return
     */
    @Override
    protected Class<?>[] getRootConfigClasses() {
        return new Class[0];
    }

    /**
     * ②：2、设置springmvc容器的spring配置类
     *
     * @return
     */
    @Override
    protected Class<?>[] getServletConfigClasses() {
        return new Class[]{MvcConfig.class};
    }

    /**
     * ③：3、配置DispatcherServlet的url-pattern
     *
     * @return
     */
    @Override
    protected String[] getServletMappings() {
        return new String[]{"/"};
    }

    /**
     * ④：4、注册拦截器
     *
     * @return
     */
    @Override
    protected Filter[] getServletFilters() {
        //添加拦截器，解决乱码问题
        CharacterEncodingFilter characterEncodingFilter = new CharacterEncodingFilter();
        characterEncodingFilter.setEncoding("UTF-8");
        characterEncodingFilter.setForceRequestEncoding(true);
        characterEncodingFilter.setForceResponseEncoding(true);
        return new Filter[]{characterEncodingFilter};
    }
}
```
<a name="lHifG"></a>
### 2.2、MVC 配置类
代码如下，这个配置类相当于 SpringMVC xml 配置文件的功能，可以在里面定义 SpringMVC 各种组件
```java
/**
 * 1.开启springmvc注解配置
 * 2、配置视图解析器
 * 3、配置截器
 * 4、配置静态资源访问
 * 5、配置文件上传解析器
 * 6、配置全局异常处理器
 */
@Configuration
@ComponentScan("com.javacode2018.springmvc.chat12")
@EnableWebMvc //1：使用EnableWebMvc开启springmvc注解方式配置
public class MvcConfig implements WebMvcConfigurer {

    /**
     * 2、添加视图解析器（可以添加多个）
     *
     * @param registry
     */
    @Override
    public void configureViewResolvers(ViewResolverRegistry registry) {
        InternalResourceViewResolver resolver = new InternalResourceViewResolver();
        resolver.setPrefix("/WEB-INF/view/");
        resolver.setSuffix(".jsp");
        resolver.setOrder(Ordered.LOWEST_PRECEDENCE);
        registry.viewResolver(resolver);
    }

    @Autowired
    private MyInterceptor myInterceptor;

    /**
     * 3、添加拦截器（可以添加多个）
     *
     * @param registry
     */
    @Override
    public void addInterceptors(InterceptorRegistry registry) {
        registry.addInterceptor(this.myInterceptor).addPathPatterns("/**");
    }


    /**
     * 4、配置静态资源访问处理器
     *
     * @param registry
     */
    @Override
    public void addResourceHandlers(ResourceHandlerRegistry registry) {
        registry.addResourceHandler("/static/**").addResourceLocations("/static/");
    }

    /**
     * 5、配置文件上传解析器
     *
     * @return
     */
    @Bean
    public CommonsMultipartResolver multipartResolver() {
        CommonsMultipartResolver commonsMultipartResolver = new CommonsMultipartResolver();
        //maxUploadSizePerFile:单个文件大小限制（byte）
        //maxUploadSize：整个请求大小限制（byte）
        commonsMultipartResolver.setMaxUploadSizePerFile(10 * 1024 * 1024);
        commonsMultipartResolver.setMaxUploadSize(100 * 1024 * 1024);
        return commonsMultipartResolver;
    }
}
```
<a name="Z2ieZ"></a>
## 2、SpringMVC 容器的生命周期(9 个阶段)

1. 阶段 1：Servlet 容器初始化
2. 阶段 2：创建父容器
3. 阶段 3：创建 SpringMVC 容器
4. 阶段 4：Servlet 容器中注册 `DispatcherServlet`
5. 阶段 5：启动父容器：`ContextLoaderListener`
6. 阶段 6：启动 SpringMVC 容器：`DispatcherServlet#init()`
7. 阶段 7：SpringMVC 容器启动过程中处理`@WebMVC`
8. 阶段 8：组装 `DispatcherServlet` 中各种 SpringMVC 需要的组件
9. 阶段 9：销毁 2 个容器
<a name="cY6FB"></a>
## 3、阶段 1：Servlet 容器初始化
<a name="uxK1U"></a>
### 3.1、`ServletContainerInitializer`
咱们知道 servlet3.0 中新增了一个接口：`ServletContainerInitializer`，这个接口功能特别的牛逼，有了它之后，web.xml 配置文件可要可不要了。
```java
public interface ServletContainerInitializer {
    public void onStartup(Set<Class<?>> c, ServletContext ctx)
        throws ServletException;
}
```
这个接口的实现类，如果满足下面 2 个条件，Servlet 容器启动的过程中会自动实例化这些类，然后调用他们的 `onStartUp` 方法，然后就可以在这些类的 `onStartUp` 方法中干活了，在 web.xml 干的所有事情，都可以在这个方法中干，特别强大：

1. 这个类必须实现 `ServletContainerInitializer` 接口，且非抽象类
2. 这个类的全类名必须要放在META-INF/services/javax.servlet.ServletContainerInitializer这个文件中
<a name="acA5z"></a>
### 3.2、`SpringServletContainerInitializer`
下面重点来了，SpringMVC 提供了一个类`SpringServletContainerInitializer`，满足了上面个条件。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683945793951-5f18c55c-e360-422c-9933-cf1614055169.png#averageHue=%23fbfaf6&clientId=ud44732be-a9d4-4&from=paste&id=u40adc792&originHeight=126&originWidth=963&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uad8b5171-88b7-4d46-85c4-d1b1114d4cb&title=)<br />spring-web-5.3.6.jar!\META-INF\services\javax.servlet.ServletContainerInitializer<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683945794007-d58b02ac-3628-4a81-a438-56002f199c45.png#averageHue=%23f3f0d6&clientId=ud44732be-a9d4-4&from=paste&id=u6b3038d5&originHeight=253&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9c00a785-7049-4568-b90f-45cd98f6662&title=)<br />所以 `SpringServletContainerInitializer` 的 `onStart` 方法会 servlet 容器自动被调用
<a name="c9OEW"></a>
### 3.3、`SpringServletContainerInitializer#onStartup` 方法
这个类的源码，大家先看一下，这个类干的事情：

1. 类上有`@HandlesTypes(WebApplicationInitializer.class)` 这个注解，注解的值为`WebApplicationInitializer.class`，所以 onStartup 方法的第一个参数是`WebApplicationInitializer`类型的集合，这个集合由 web 容器自动扫描获取，然后传入进来
2. 实例化 `WebApplicationInitializer` 集合
3. 对 `WebApplicationInitializer` 集合进行排序
4. 循环调用 `WebApplicationInitializer` 的 `onStartup` 方法
```java
@HandlesTypes(WebApplicationInitializer.class) //@1
public class SpringServletContainerInitializer implements ServletContainerInitializer {

    @Override
    public void onStartup(@Nullable Set<Class<?>> webAppInitializerClasses, ServletContext servletContext)
    throws ServletException {

        List<WebApplicationInitializer> initializers = Collections.emptyList();

        if (webAppInitializerClasses != null) {
            initializers = new ArrayList<>(webAppInitializerClasses.size());
            for (Class<?> waiClass : webAppInitializerClasses) {
                // Be defensive: Some servlet containers provide us with invalid classes,
                // no matter what @HandlesTypes says...
                if (!waiClass.isInterface() && !Modifier.isAbstract(waiClass.getModifiers()) &&
                    WebApplicationInitializer.class.isAssignableFrom(waiClass)) {
                    try {
                        initializers.add((WebApplicationInitializer)
                                          ReflectionUtils.accessibleConstructor(waiClass).newInstance());
                    }
                    catch (Throwable ex) {
                        throw new ServletException("Failed to instantiate WebApplicationInitializer class", ex);
                    }
                }
            }
        }

        if (initializers.isEmpty()) {
            servletContext.log("No Spring WebApplicationInitializer types detected on classpath");
            return;
        }

        servletContext.log(initializers.size() + " Spring WebApplicationInitializers detected on classpath");
        AnnotationAwareOrderComparator.sort(initializers);
        for (WebApplicationInitializer initializer : initializers) {
            initializer.onStartup(servletContext);
        }
    }

}
```
下面重点要看`WebApplicationInitializer`接口了。
<a name="C35Q2"></a>
### 3.4、`WebApplicationInitializer`：web 应用初始化
接口比较简单，就一个方法，参数是 servlet 上下文对象，有了个对象，可以干 web.xml 中的一切事情了，比如注册 servlet、filter、监听器等等
```java
public interface WebApplicationInitializer {

    void onStartup(ServletContext servletContext) throws ServletException;

}
```
如下图，看一下类的继承关系，MvcInit 就实现了这个接口，所以 MvcInit 的 `onStartup` 方法会被调费用<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683945794098-2c08624e-b2eb-4081-adee-3d98b8bf5e87.png#averageHue=%23faf9f2&clientId=ud44732be-a9d4-4&from=paste&id=uc9084573&originHeight=392&originWidth=592&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=udc1e12d6-dd36-44aa-b6d9-61d8a8d50c3&title=)<br />关键代码在这 3 个类中<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683945793998-baf0e086-ca16-4c7a-8d48-c70f2830d861.png#averageHue=%23faf9f2&clientId=ud44732be-a9d4-4&from=paste&id=ua44c99cd&originHeight=412&originWidth=606&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9812b7f4-3b82-4196-9f0d-214eb629b77&title=)
<a name="Qav3k"></a>
### 3.5、进入 `AbstractDispatcherServletInitializer#onStartup` 方法
```java
public void onStartup(ServletContext servletContext) throws ServletException {
    super.onStartup(servletContext);
    registerDispatcherServlet(servletContext);
}
```
**这里是重点：这个方法中干了 4 件事情**

1. 创建父容器，只是实例化了，并未启动
2. 创建了监听器 `ContextLoaderListener`，这是一个 `ServletContextListener` 类型的监听器，稍后会在这个监听器中启动父容器
3. 创建 SpringMVC 容器，只是实例化了，并未启动，启动的事情会在 `DispatcherServlet#init` 中做，稍后会说
4. Servlet 容器中注册 `DispatcherServlet`

下面，咱们来详细看这几个步骤，把这几个步骤作为阶段来解读。
<a name="AMl6M"></a>
## 4、阶段 2：创建父容器
父容器可有可无，并不是必须的，为了更好的管理 bean，SpringMVC 建议用父子容器，Controller 之外的 bean，比如 service，dao 等，建议放到父容器中，Controller 层的和 SpringMVC 相关的一些 bean 放在 SpringMVC 容器中，咱们继续。
<a name="a1DyA"></a>
### 4.1、过程
`AbstractDispatcherServletInitializer#onStartup`方法中会调用父类的`onStartup`，即`AbstractContextLoaderInitializer#onStartup`，进到这个方法中，代码如下图，干了 2 个事情

1. 图中编号 ①：创建父容器，只是实例化了，并未启动
2. 图中编号 ②：创建了一个监听器 `ContextLoaderListener`，这是一个 `ServletContextListener` 类型的监听器，稍后会在这个监听器中启动父容器

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683945793951-252227e7-e280-4604-9a36-1fd67b1e34ea.png#averageHue=%23fbfaf7&clientId=ud44732be-a9d4-4&from=paste&id=u9569f962&originHeight=565&originWidth=846&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8e4cc1cd-1253-4dbb-8eb8-43fc1de975a&title=)<br />下面来分别来细说下上面 2 段代码干的活。
<a name="h7Vrv"></a>
### 4.2、①：负责创建父容器
`AbstractAnnotationConfigDispatcherServletInitializer#createRootApplicationContext`，只是创建了一个`AnnotationConfigWebApplicationContext`对象，并将父容器配置类 `rootConfigClass` 注册到容器中，并没有启动这个容器，若 `rootConfigClass` 为空，父容器不会被创建，所以父容器可有可无。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683945794741-81778d4f-f2c6-4772-910a-0f3fca64ef2b.png#averageHue=%23fcfcfa&clientId=ud44732be-a9d4-4&from=paste&id=u16185685&originHeight=304&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uce081055-688c-4864-a363-e37d5982362&title=)
<a name="amjKn"></a>
### 4.2、②：创建 `ContextLoaderListener` 监听器
代码如下，创建的时候将父容器对象 `rootAContext` 传进去了。
```java
ContextLoaderListener listener = new ContextLoaderListener(rootAppContext);
//getRootApplicationContextInitializers()返回置为ApplicationContextInitializer数组，是个函数式接口，在父容器初始化的过程中，会作为一个扩展点预留给开发者用
listener.setContextInitializers(getRootApplicationContextInitializers());
servletContext.addListener(listener);
```
`ContextLoaderListener`，这是一个 `ServletContextListener` 类型的监听器，所以在 web 容器启动和销毁的过程中会被调用，如下图，这个监听器干了 2 件事

1. `contextInitialized` 方法：这个方法会在 web 容器启动时被调用，内部负责启动父容器
2. 在 `contextDestroyed` 方法：这个方法会在 web 容器销毁时被调用，内部负责关闭父容器

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683945794922-cafefdd5-a1a3-4cd4-a7d1-bd2cf648f304.png#averageHue=%23fbfaf6&clientId=ud44732be-a9d4-4&from=paste&id=ud3a9e0a8&originHeight=579&originWidth=798&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u033dec55-6034-46b2-8481-feff93d1033&title=)
<a name="Uzzl7"></a>
## 5、阶段 3&4：创建 SpringMVC 容器&注册 `DispatcherServlet`
在回到`AbstractDispatcherServletInitializer#onStartup`，看这个方法的第二行，如下图<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683945794824-72c7d8a5-f3fe-4601-9f89-bcaea4838e22.png#averageHue=%23fcfaf7&clientId=ud44732be-a9d4-4&from=paste&id=u34623968&originHeight=149&originWidth=850&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u870c2ae9-a578-46cf-8e6b-862680a67f1&title=)<br />`registerDispatcherServlet`源码如下
```java
protected void registerDispatcherServlet(ServletContext servletContext) {
    //①：DispatcherServlet的servlet名称，默认为：dispatcher
    String servletName = getServletName();

    //②：创建springmvc容器
    WebApplicationContext servletAppContext = createServletApplicationContext();

    //③：创建DispatcherServlet,注意这里将springmvc容器对象做为参数传递给DispatcherServlet了
    FrameworkServlet dispatcherServlet = createDispatcherServlet(servletAppContext);
    //设置ApplicationContextInitializer列表，可以对springmvc容器在启动之前进行定制化
    dispatcherServlet.setContextInitializers(getServletApplicationContextInitializers());

    //④：将 dispatcherServlet 注册到servlet上下文中
    ServletRegistration.Dynamic registration = servletContext.addServlet(servletName, dispatcherServlet);
    registration.setLoadOnStartup(1);
    registration.addMapping(getServletMappings());
    registration.setAsyncSupported(isAsyncSupported());

    //⑤：注册Filter
    Filter[] filters = getServletFilters();
    if (!ObjectUtils.isEmpty(filters)) {
        for (Filter filter : filters) {
            registerServletFilter(servletContext, filter);
        }
    }
    //⑥：这个方法预留给咱们自己去实现，可以对dispatcherServlet做一些特殊的配置
    customizeRegistration(registration);
}

protected FrameworkServlet createDispatcherServlet(WebApplicationContext servletAppContext) {
    return new DispatcherServlet(servletAppContext);
}
```
<a name="bKGJm"></a>
## 6、阶段 5：启动父容器：`ContextLoaderListener`
<a name="mXc1P"></a>
### 6.1、过程
上面的`onStartup`方法执行完毕之后，会执行监听器`ContextLoaderListener`的初始化，会进入到他的`contextInitialized`方法中<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683945794883-c8fd3b8e-1373-463b-b3ac-b8dfafae49ec.png#averageHue=%23faf9f7&clientId=ud44732be-a9d4-4&from=paste&id=u907f1402&originHeight=234&originWidth=756&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9c7ef56c-cbca-4064-8522-0666cbe92a5&title=)<br />`initWebApplicationContext`源码如下，截取了主要的几行
```java
public WebApplicationContext initWebApplicationContext(ServletContext servletContext) {
    //this.context就是父容器对象
    ConfigurableWebApplicationContext cwac = (ConfigurableWebApplicationContext) this.context;
    //①：配置及启动父容器
    configureAndRefreshWebApplicationContext(cwac, servletContext);
    //将父容器丢到servletContext中进行共享，方便其他地方获取
    servletContext.setAttribute(WebApplicationContext.ROOT_WEB_APPLICATION_CONTEXT_ATTRIBUTE, this.context);
}
```
<a name="wct8u"></a>
### 6.2、代码 ①：配置父容器以及启动父容器
```java
//①：配置及启动父容器
configureAndRefreshWebApplicationContext(cwac, servletContext);
```
`configureAndRefreshWebApplicationContext`方法关键代码如下
```java
protected void configureAndRefreshWebApplicationContext(ConfigurableWebApplicationContext wac, ServletContext sc) {
    //①：定制上线文，这里主要是遍历ApplicationContextInitializer列表，调用每个ApplicationContextInitializer#initialize方法来对容器进行定制，相当于一个扩展点，可以有程序员自己控制
    customizeContext(sc, wac);
    //②：刷新容器，就相当于启动容器了，此时就会组装里面的bean了
    wac.refresh();
}
```
`customizeContext`方法，进去看一下，这里涉及到了一个新的类，所以有必要去看一下，混个脸熟，源码如下，这是给开发者留的一个扩展点，通过`ApplicationContextInitializer`这个来做扩展，这是一个函数式接口，下面代码会遍历`ApplicationContextInitializer`列表，然后调用其`initialize`方法，可以在这个方法中对 spring 上线文进行定制
```java
protected void customizeContext(ServletContext sc, ConfigurableWebApplicationContext wac) {
    List<Class<ApplicationContextInitializer<ConfigurableApplicationContext>>> initializerClasses =
    determineContextInitializerClasses(sc);

    for (Class<ApplicationContextInitializer<ConfigurableApplicationContext>> initializerClass : initializerClasses) {
        Class<?> initializerContextClass =
        GenericTypeResolver.resolveTypeArgument(initializerClass, ApplicationContextInitializer.class);
        if (initializerContextClass != null && !initializerContextClass.isInstance(wac)) {
            throw new ApplicationContextException(String.format(
                "Could not apply context initializer [%s] since its generic parameter [%s] " +
                "is not assignable from the type of application context used by this " +
                "context loader: [%s]", initializerClass.getName(), initializerContextClass.getName(),
                wac.getClass().getName()));
        }
        this.contextInitializers.add(BeanUtils.instantiateClass(initializerClass));
    }

    AnnotationAwareOrderComparator.sort(this.contextInitializers);
    for (ApplicationContextInitializer<ConfigurableApplicationContext> initializer : this.contextInitializers) {
        initializer.initialize(wac);
    }
}
```
<a name="qx6xn"></a>
### 6.3、`ApplicationContextInitializer` 接口：容器启动前用来初始化容器
是个函数式接口，在容器启动之前用来对容器进行定制，作为一个扩展点预留给开发者用，父容器和 SpringMVC 容器都用到了。
```java
@FunctionalInterface
public interface ApplicationContextInitializer<C extends ConfigurableApplicationContext> {

     /**
      * 初始化给定的spring容器
      * @param applicationContext the application to configure
      */
     void initialize(C applicationContext);

}
```
<a name="I31EP"></a>
## 7、阶段 6：启动 SpringMVC 容器：`DispatcherServlet#init()`
到目前为止父容器已经启动完毕了，此时 `DispatcherServlet` 会被初始化，会进入到他的 `init()`方法中。
<a name="rl2RP"></a>
### 7.1、DispatcherServlet 类图
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683945840247-edf1fa61-cc42-460d-a4ce-6a0f3074037f.png#averageHue=%23fbfaf4&clientId=ud44732be-a9d4-4&from=paste&id=u0afb7eef&originHeight=498&originWidth=302&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u823ceb8e-d46d-4d83-b39a-d0a62b21523&title=)
<a name="yvysb"></a>
### 7.2、`HttpServletBean#init()`
这个方法会调用`initServletBean()`这个方法，其他的先不看<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683945840248-2db8ed49-aa03-45ac-b76d-1aadf25f344e.png#averageHue=%23fdfdfc&clientId=ud44732be-a9d4-4&from=paste&id=u0a13b3c3&originHeight=282&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u92003c7c-7891-45f6-9228-1fa27da72db&title=)
<a name="sNVBv"></a>
### 7.3、`FrameworkServlet#initServletBean`
提取了关键的代码，就 2 行
```java
@Override
protected final void initServletBean() throws ServletException {
    //初始化springmvc容器，就是启动springmvc容器
    this.webApplicationContext = initWebApplicationContext();
    //这个方法内部是空的，预留给子类去实现的，目前没啥用
    initFrameworkServlet();
}
```
下面进到`initWebApplicationContext`方法中去。
<a name="JDqTe"></a>
### 7.4、`FrameworkServlet#initWebApplicationContext`
关键代码如下，干了 3 件事情：

1. 从 servlet 上线文对象中找到父容器
2. 为 SpringMVC 容器指定父容器
3. 调用 `configureAndRefreshWebApplicationContext` 方法配置 SpringMVC 容器以及启动容器，这个是关键咯
```java
protected WebApplicationContext initWebApplicationContext() {
    //①：从servlet上线文中获取父容器
    WebApplicationContext rootContext =
    WebApplicationContextUtils.getWebApplicationContext(getServletContext());
    WebApplicationContext wac = null;
    //②：this.webApplicationContext就是springmvc容器，此时这个对对象不为null，所以满足条件
    if (this.webApplicationContext != null) {
        wac = this.webApplicationContext;
        if (wac instanceof ConfigurableWebApplicationContext) {
            ConfigurableWebApplicationContext cwac = (ConfigurableWebApplicationContext) wac;
            //springmvc容器未启动
            if (!cwac.isActive()) {
                //springmvc容器未设置父容器，则给其设置父容器，此时rootContext可能为空，这个没什么关系
                if (cwac.getParent() == null) {
                    cwac.setParent(rootContext);
                }
                //③：配置springmvc容器以及启动springmvc容器
                configureAndRefreshWebApplicationContext(cwac);
            }
        }
    }
    //这里省略了一部分代码，如果springmvc采用配置文件的方式会走这部分代码
    ......
    //返回容器
    return wac;
}
```
<a name="yYG2b"></a>
### 7.5、`FrameworkServlet#configureAndRefreshWebApplicationContext`
为了让大家看清楚，如下代码，这里只提取了关键代码，主要干了 3 件事情

1. 代码 ①：向 SpringMVC 容器中添加了一个 `ContextRefreshListener` 监听器，这个监听器非常非常重要，SpringMVC 容器启动完毕之后会被调用，**会出现在阶段 8 中**
2. 代码 ②：给开发者预留的一个扩展点，通过 `ApplicationContextInitializer#initialize` 方法对容器进行定制
3. 代码 ③：启动容器
```java
protected void configureAndRefreshWebApplicationContext(ConfigurableWebApplicationContext wac) {
    //①：向springmvc容器中添加了一个监听器对象，这个监听器特别重要，稍后在
    wac.addApplicationListener(new SourceFilteringListener(wac, new ContextRefreshListener()));
    //②：扩展点：循环遍历ApplicationContextInitializer列表，调用其initialize方法对容器进行定制
    applyInitializers(wac);
    //③：刷新容器，相当于启动容器
    wac.refresh();
}
```
<a name="jzwfh"></a>
## 8、阶段 7：SpringMVC 容器启动过程中处理`@WebMVC`
<a name="fjZ8n"></a>
### 8.1、SpringMVC 配置类被处理
此时 SpringMVC 容器启动了，此时注意下`MvcConfig`这个类，由于其上有`@Conguration` 注解，所以会被当做一个配置类被处理，这个类有 2 个非常重要的特征。

1. 标注了`@EnableWebMvc` 注解
2. 实现了 `WebMvcConfigurer` 接口

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683945840247-6c5203b1-fa57-48d5-a50d-185f0e72e5f3.png#averageHue=%23faf9f5&clientId=ud44732be-a9d4-4&from=paste&id=uf03a9fbf&originHeight=165&originWidth=647&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue47629f3-9983-48ba-8558-8d924e04015&title=)<br />下面来说说这 2 个特征的作用。
<a name="ZmNIL"></a>
### 8.2、`@EnableWebMvc`：配置 SpringMVC 所需组件
看一下这个注解的源码，如下，重点在于它上面的`@Import(DelegatingWebMvcConfiguration.class)`注解。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683945840277-7b8be30d-8866-4be4-b1b8-9487b59b109f.png#averageHue=%23faf9f4&clientId=ud44732be-a9d4-4&from=paste&id=ue90b6164&originHeight=184&originWidth=569&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uac64c4cf-1032-4728-9b88-9721fff3bc0&title=)
<a name="TRlei"></a>
### 8.3、进入 `DelegatingWebMvcConfiguration` 类
代码如下，先注意下面 3 个特征

1. 代码编号 ①：标注有`@Configuration` 注解，说明是一个配置类
2. 代码编号 ②：继承了 `WebMvcConfigurationSupport` 类，这个类中有很多`@Bean` 标注的方法，用来定义了 SpringMVC 需要的所有组件
3. 代码编号 ③：注入了`WebMvcConfigurer`列表，注意下，`WebConfig` 类就实现了 `WebMvcConfigurer` 这个接口，内部提供了很多方法可以用来对 SpringMVC 的组件进行自定义配置

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683945840178-62b825ff-157e-4ef1-b479-cfdcc00a2414.png#averageHue=%23fcfbf9&clientId=ud44732be-a9d4-4&from=paste&id=u61b004cb&originHeight=346&originWidth=996&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8a1c6545-9651-4a1a-a732-b61b1a8d4ea&title=)<br />先来看看 `WebMvcConfigurationSupport` 这个类。
<a name="dvgBY"></a>
### 8.4、`WebMvcConfigurationSupport`：配置 SpringMVC 所需所有组件
这个类中会定义 SpringMVC 需要的所有组件，比如：`RequestMapping`、`HandlerAdapter`、`HandlerInterceptor`、`HttpMessageConverter`、`HandlerMethodArgumentResolver`、`HandlerMethodReturnValueHandler` 等等，所以如果感觉`@WebMVC` 注解满足不了需求时，可以直接继承这个类进行扩展。<br />这个类的源码就不贴了，截几个图给大家看看<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683945840800-415caac2-0360-428e-a7a8-2b5b927c939d.png#averageHue=%23fbf9f6&clientId=ud44732be-a9d4-4&from=paste&id=u39ff5c8a&originHeight=524&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u94a55dec-d747-4fcf-8531-82507f9868b&title=)
<a name="CzBlf"></a>
### 8.5、`WebMvcConfigurer` 接口
这个接口就是用来对 SpringMVC 容器中的组件进行定制的，`WebMvcConfigurationSupport` 中创建 SpringMVC 组件的时候，会自动调用 `WebMvcConfigurer` 中对应的一些方法，来对组件进行定制，比如可以在 `WebMvcConfigurer` 中添加拦截器、配置默认 servlet 处理器、静态资源处理器等等，这个接口的源码如下
```java
public interface WebMvcConfigurer {

    /**
     * 配置PathMatchConfigurer
     */
    default void configurePathMatch(PathMatchConfigurer configurer) {
    }

    /**
     * 配置ContentNegotiationConfigurer
     */
    default void configureContentNegotiation(ContentNegotiationConfigurer configurer) {
    }

    /**
     * 异步处理配置
     */
    default void configureAsyncSupport(AsyncSupportConfigurer configurer) {
    }

    /**
     * 配置默认servlet处理器
     */
    default void configureDefaultServletHandling(DefaultServletHandlerConfigurer configurer) {
    }

    /**
     * 配置Formatter
     */
    default void addFormatters(FormatterRegistry registry) {
    }

    /**
     * 添加拦截器
     */
    default void addInterceptors(InterceptorRegistry registry) {
    }

    /**
     * 静态资源配置
     */
    default void addResourceHandlers(ResourceHandlerRegistry registry) {
    }

    /**
     * 跨越的配置
     */
    default void addCorsMappings(CorsRegistry registry) {
    }

    /**
     * 配置ViewController
     */
    default void addViewControllers(ViewControllerRegistry registry) {
    }

    /**
     * 注册视图解析器（ViewResolverRegistry）
     */
    default void configureViewResolvers(ViewResolverRegistry registry) {
    }

    /**
     * 注册处理器方法参数解析器（HandlerMethodArgumentResolver）
     */
    default void addArgumentResolvers(List<HandlerMethodArgumentResolver> resolvers) {
    }

    /**
     * 注册处理器方法返回值处理器（HandlerMethodReturnValueHandler）
     */
    default void addReturnValueHandlers(List<HandlerMethodReturnValueHandler> handlers) {
    }

    /**
     * 注册http报文转换器（HttpMessageConverter）
     */
    default void configureMessageConverters(List<HttpMessageConverter<?>> converters) {
    }

    /**
     * 扩展报文转换器
     */
    default void extendMessageConverters(List<HttpMessageConverter<?>> converters) {
    }

    /**
     * 配置异常解析器（HandlerExceptionResolver）
     */
    default void configureHandlerExceptionResolvers(List<HandlerExceptionResolver> resolvers) {
    }

    /**
     * 扩展异常解析器（HandlerExceptionResolver）
     */
    default void extendHandlerExceptionResolvers(List<HandlerExceptionResolver> resolvers) {
    }

    /**
     * 获得验证器
     */
    @Nullable
    default Validator getValidator() {
        return null;
    }

    /**
     * 获得MessageCodesResolver
     */
    @Nullable
    default MessageCodesResolver getMessageCodesResolver() {
        return null;
    }

}
```
<a name="G50Cn"></a>
## 9、阶段 8：组装 `DispatcherServlet `中各种 SpringMVC 需要的组件
<a name="sKWKY"></a>
### 9.1、触发 `ContextRefreshListener` 监听器
大家回头看一下 8.5 中，有这样一段代码，注册了一个监听器`ContextRefreshListener`<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683945840697-ece651bf-505a-4c34-8d4f-b589f0925d16.png#averageHue=%23f3f0ed&clientId=ud44732be-a9d4-4&from=paste&id=ub19d82bf&originHeight=185&originWidth=883&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue01b4ef9-39dd-4be2-9063-b055c840699&title=)<br />再来看看这个监听器的源码，如下图，包含 2 点信息

1. 会监听 `ContextRefreshedEvent` 事件
2. 监听到事件之后将执行`FrameworkServlet.this.onApplicationEvent(event);`，稍后会具体说这个代码

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683945840698-f8e3f4ee-3811-42e3-b4b6-d5b8c1bfd5f3.png#averageHue=%23fcf9f6&clientId=ud44732be-a9d4-4&from=paste&id=u42a1134d&originHeight=222&originWidth=1048&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u96c4bb1a-6fa3-4465-8f02-2f1a992b290&title=)<br />如下代码，SpringMVC 容器启动完毕之后，会发布一个`ContextRefreshedEvent`事件，会触发上面这个监听器的执行<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683945840791-5ad92782-cbec-4386-8ad0-f04bd2030169.png#averageHue=%23f9f8f5&clientId=ud44732be-a9d4-4&from=paste&id=uf7e20572&originHeight=608&originWidth=849&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u1cf1867f-ec53-433f-a52b-f3fa4468554&title=)
<a name="DFdnm"></a>
### 9.2、进入 `FrameworkServlet.this.onApplicationEvent(event);`
```java
public void onApplicationEvent(ContextRefreshedEvent event) {
    //标记已收到刷新事件
    this.refreshEventReceived = true;
    synchronized (this.onRefreshMonitor) {
        onRefresh(event.getApplicationContext());
    }
}
```
上面的`onRefresh(event.getApplicationContext());`会进到`DispatcherServlet#onRefresh`方法中。
<a name="V75Vm"></a>
### 9.3、进入 `DispatcherServlet#onRefresh`
```java
protected void onRefresh(ApplicationContext context) {
    initStrategies(context);
}
```
里面会调用`initStrategies`方法。
<a name="OPl0z"></a>
### 9.4、`DispatcherServlet#initStrategies`：初始化 `DispatcherServlet` 中的组件
代码如下，这里面会初始化 `DispatcherServlet` 中的各种组件，这里的所有方法初始化的过程基本上差不多，就是先从 SpringMVC 容器中找这个组件，如果找不到一般会有一个兜底的方案
```java
protected void initStrategies(ApplicationContext context) {
    //初始化MultipartResolver
    initMultipartResolver(context);
    //初始化LocaleResolver
    initLocaleResolver(context);
    //初始化ThemeResolver
    initThemeResolver(context);
    //初始化HandlerMappings
    initHandlerMappings(context);
    //初始化HandlerAdapters
    initHandlerAdapters(context);
    //初始化HandlerExceptionResolvers
    initHandlerExceptionResolvers(context);
    //初始化RequestToViewNameTranslator
    initRequestToViewNameTranslator(context);
    //初始化视图解析器ViewResolvers
    initViewResolvers(context);
    //初始化FlashMapManager
    initFlashMapManager(context);
}
```
下面以`initHandlerMappings(context);`为例来看一下是如何初始化这些组件的。
<a name="sCHX9"></a>
### 9.5、`initHandlerMappings(context);`
源码如下，就是先从容器中找，找不到走兜底的方案。
```java
private void initHandlerMappings(ApplicationContext context) {
    this.handlerMappings = null;
    //是否需要查找所有的HandlerMapping，默认为true
    if (this.detectAllHandlerMappings) {
        //从容器中查找所有的HandlerMapping
        Map<String, HandlerMapping> matchingBeans =
                BeanFactoryUtils.beansOfTypeIncludingAncestors(context, HandlerMapping.class, true, false);
        //对HandlerMapping列表进行排序
        if (!matchingBeans.isEmpty()) {
            this.handlerMappings = new ArrayList<>(matchingBeans.values());
            AnnotationAwareOrderComparator.sort(this.handlerMappings);
        }
    }
    else {
        try {
            //查找名称为handlerMapping的HandlerMapping
            HandlerMapping hm = context.getBean("handlerMapping", HandlerMapping.class);
            this.handlerMappings = Collections.singletonList(hm);
        }
        catch (NoSuchBeanDefinitionException ex) {
        }
    }

    // 如果没有找到HandlerMapping，则走兜底的方案
    if (this.handlerMappings == null) {
        this.handlerMappings = getDefaultStrategies(context, HandlerMapping.class);
    }
}
```
下面来看一下兜底的代码如何走的，进入 `getDefaultStrategies` 方法
<a name="ZnTjB"></a>
### 9.6、`DispatcherServlet#getDefaultStrategies`：兜底的方案查找组件
这个方法的源码就不贴出来了，这里只说一下兜底的处理过程，SpringMVC 有个配置文件：spring-webmvc-5.3.6.jar!\org\springframework\web\servlet\DispatcherServlet.properties，properties 格式的文件，key 为组件的完整类名，value 为多个实现类的列表，在这个配置文件中指定了每种类型的组件兜底的情况下对应的实现类，比如没有找到 RequestMapping 的情况下，如下图红框的部分，有 3 个兜底的实现类，然后 SpringMVC 会实例化这 3 个类作为 RequestMapping。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683945840856-49417e09-37ca-464a-964f-f6245bdadd95.png#averageHue=%23f2f2d8&clientId=ud44732be-a9d4-4&from=paste&id=u4a24149e&originHeight=376&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u635a2666-66aa-4663-883b-4b71ddfc28e&title=)
<a name="Ttj0q"></a>
## 10、阶段 9：销毁容器
<a name="Piizh"></a>
### 10.1、销毁 SpringMVC 容器：`DispatcherServlet#destroy`
`DispatcherServlet` 销毁的时候会关闭 SpringMVC 容器
```java
public void destroy() {
    if (this.webApplicationContext instanceof ConfigurableApplicationContext && !this.webApplicationContextInjected) {
        ((ConfigurableApplicationContext) this.webApplicationContext).close();
    }
}
```
<a name="DIcXO"></a>
### 10.2、销毁父容器：`ContextLoaderListener#contextDestroyed`
父容器是在监听器中启动的，所以销毁的也是监听器负责的
```java
public void contextDestroyed(ServletContextEvent event) {
    closeWebApplicationContext(event.getServletContext());
    ContextCleanupListener.cleanupAttributes(event.getServletContext());
}
```
