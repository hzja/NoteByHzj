Java过滤器拦截器
<a name="OEPWZ"></a>
## 过滤器
过滤器 Filter
<a name="Ft9eY"></a>
### 什么是过滤器
过滤器是 Servlet 的高级特性之一，是实现 Filter 接口的 Java 类。其基本功能就是对 servlet 的调用进行干预，在 Servlet 请求和响应的过程中增加一些特定的功能。可以使用过滤器实现的功能有:统一编码，URL级别的权限访问控制，过滤敏感词汇，压缩请求信息.<br />来通过代码来看 Filter 的执行过程
```java
public interface Filter {
    //用于执行过滤器的初始化工作，web容器会在web项目启动时自动调用该方法
    default void init(FilterConfig filterConfig) throws ServletException {
    }

    //当请求和响应被过滤器拦截后，都会交给doFilter来处理:其中两个参数分别是被拦截request和response对象，可以使用chain的doFliter方法来放行
    void doFilter(ServletRequest var1, ServletResponse var2, FilterChain var3) throws IOException, ServletException;
    //用于释放关闭Filter对象打开的资源，在web项目关闭时，由web容器自动调用该方法
    default void destroy() {
    }
}
```
在 Filter 中可以看到有三个方法，分别是

- `init`
- `doFilter`
- `destroy`

至于每个方法的作用，已经把他们的功能写在了注释上面，他们实际上就是三个步骤：

1. 初始化
2. 处理请求和相应过滤，完成操作
3. 最后释放资源

而实现一个自定义 Filter 也是比较简单的，来实现一个简单的自定义的 Filter。
```java
@Component
@Slf4j
public class MyFilter implements Filter {

    @Override
    public void init(FilterConfig filterConfig) throws ServletException {
        log.info("初始化过滤器：" + filterConfig.getFilterName());
    }

    @Override
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain) throws IOException, ServletException {

        HttpServletRequest httpRequest = (HttpServletRequest) request;
        HttpServletResponse httpResponse = (HttpServletResponse) response;

        // 对请求进行过滤操作处理
        log.info("MyFilter 开始对请求进去过滤操作！");
        String requestURI = httpRequest.getRequestURI();
        log.info("请求地址：" + requestURI);

        // 继续执行下一个 Filter，如果没有其他 Filter 的话就执行 Controller 层代码
        long startTime = System.currentTimeMillis();
        chain.doFilter(request, response);

        // 执行完用户请求后，回到这里对 response 响应内容做一些处理
        long endTime = System.currentTimeMillis();
        log.info("请求处理完毕，所花费时间为：" + (endTime - startTime) + "ms");
    }

    @Override
    public void destroy() {
        log.info("销毁过滤器 MyFilter");
    }

}
```
至于 Filter 的配置的话，都不用解释，现在几乎没有再去配置 xml 的方式了，都是配置类或者注解的方式。<br />注解方式：
```java
@WebFilter(filterName = "myFilter",urlPatterns = {"/*"})
```
配置类方式：
```java
@Configuration
public class MyFilterConfig {

    @Bean
    public FilterRegistrationBean myFilter(){
        FilterRegistrationBean fb = new FilterRegistrationBean();
        //设置filter启动顺序
        fb.setOrder(1);
        fb.setFilter(new MyFilter());
        fb.addInitParameter("phone","183****5510");
        //设置拦截请求规则,这里拦截所有请求
        fb.addUrlPatterns("/*");
        return fb;
    }
}
```
看完了这个过滤器，接下来就得看看这个拦截器了。
<a name="eWSdx"></a>
## 拦截器
拦截器 `Interceptor`
<a name="WEN5Z"></a>
### 什么是拦截器
Spring MVC 中的拦截器`Interceptor`类似于 ServLet 中的过滤器`Filter`，它主要用于拦截用户请求并作出相应的处理。例如通过拦截器可以进行权限验证、记录请求信息的日志、判断用户是否登录等。<br />老规矩，直接看源代码
```java
public interface HandlerInterceptor {
    //预处理方法，本方法在控制器方法（MyController的方法）之前执行，用户的请求最先到达此方法，在这个方法中可以获取请求的信息，验证请求是否符合要求。以验证用户是否登录，验证用户是否有权限访问某个链接地址（url）。如果返回true则放行，返回false则拦截
    default boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler) throws Exception {
        return true;
    }
    //后处理方法。在controller中的方法之后执行的。能够获取到处理器方法的返回值 mv，可以修改mv中的数据和视图。可以影响到最后的执行结果。主要是对原来的执行结果做二次修正
    default void postHandle(HttpServletRequest request, HttpServletResponse response, Object handler, @Nullable ModelAndView modelAndView) throws Exception {
    }
    //最后执行的方法，在页面渲染之后执行。在请求处理完成后执行的，框架中规定是当你的视图处理完成后，对视图进行了forword。就任务请求处理完成。
    default void afterCompletion(HttpServletRequest request, HttpServletResponse response, Object handler, @Nullable Exception ex) throws Exception {
    }
}
```
其实也是三个步骤：

1. 预处理
2. 后处理
3. 最后执行

至于每一步的含义和内容，在注释中表明了。<br />同样的，自定义实现一个拦截器也很简单：
```java
public class MyInterceptor implements HandlerInterceptor {
    private static final Logger LOGGER = LoggerFactory.getLogger(MyInterceptor.class);

    private static final ThreadLocal<Long> START_THREAD_LOCAL = new ThreadLocal<>();

    @Override
    public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler)
    throws Exception {
        String uri = request.getRequestURI();
        LOGGER.info(uri + " preHandle");
        Long startTime = System.currentTimeMillis();    //获取开始时间
        START_THREAD_LOCAL.set(startTime);  //线程绑定变量（该数据只有当前请求的线程可见）
        return true;
    }

    @Override
    public void postHandle(HttpServletRequest request, HttpServletResponse response, Object handler,
                           ModelAndView modelAndView) throws Exception {
        String uri = request.getRequestURI();
        LOGGER.info(uri + " postHandle");
        Long startTime = START_THREAD_LOCAL.get();//得到线程绑定的局部变量（开始时间）
        Long endTime = System.currentTimeMillis();  //2、结束时间
        Long time = endTime - startTime;
        LOGGER.info("http request all time: " + time + "ms");
    }

    @Override
    public void afterCompletion(HttpServletRequest request, HttpServletResponse response, Object handler,
                                Exception ex) throws Exception {
        String uri = request.getRequestURI();
        LOGGER.info(uri + " afterCompletion");
        if (START_THREAD_LOCAL != null) {
            START_THREAD_LOCAL.remove();    // 移除ThreadLocal中的局部变量
        }
    }
}
```
自定义一个获取并返回某个静态资源的内容以及整个请求所花费的时间拦截器，一般这个还是比较有用的，而且还可以加一个请求访问的，然后来处理方法执行时间的。<br />配置拦截器其实在项目中是非常的常见
```java
@Configuration
public class MyInterceptorConfig implements WebMvcConfigurer {
 
    /**
     * 配置拦截器
     * @param registry
     */
    @Override
    public void addInterceptors(InterceptorRegistry registry){
        registry.addInterceptor(myInterceptor())
                .addPathPatterns("/api/v1/**")//需要拦截的请求
                .addPathPatterns("/api/v1/**")//需要拦截的请求
                .excludePathPatterns("/api/debug/**")//不拦截的请求
                .excludePathPatterns("api/page/getName");//不拦截的请求
    }
 
    /**
     * 注入拦截器到spring容器
     * @return
     */
    @Bean
    public MyInterceptor myInterceptor(){
        return new MyInterceptor();
    }
}
```
看完了过滤器和拦截器的实现，接下来就得来看看这个过滤器和拦截器之间的区别了。

1. 依赖点不同
- 过滤器依赖与servlet容器，而拦截器不依赖与servlet容器
- 过滤器属于Servlet，而拦截器属于springmvc
2. 实现原理不同
- 过滤器是基于函数回调，而拦截器是基于java的反射机制的
3. 作用域不同
- 过滤器则可以对几乎所有的请求起作用，而拦截器只能对action请求起作用
4. 生命周期不同
- 在action的生命周期中，拦截器可以多次被调用，而过滤器只能在容器初始化时被调用一次

这是区别，其实二者的相同点也是有的，比如，拦截器和过滤器实际上都是 Spring 中 AOP 的体现，都能实现一些权限和日志方面的功能。
