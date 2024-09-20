SpringBoot 拦截器 过滤器 监听器
<a name="R65OF"></a>
## 过滤器
过滤器的英文名称为 Filter，是 [Servlet](http://mp.weixin.qq.com/s?__biz=MzI3ODcxMzQzMw==&mid=2247489342&idx=1&sn=20b3163e16f5a6b8abb770d022b22aff&chksm=eb539208dc241b1eb685b3f15ea4d769fb5765344ce2a80b3c901098f5094f4f9fc70e089d8b&scene=21#wechat_redirect) 技术中最实用的技术。<br />如同它的名字一样，过滤器是处于客户端和服务器资源文件之间的一道过滤网，过滤掉一些不符合要求的请求，通常用作 Session 校验，判断用户权限，如果不符合设定条件，则会被拦截到特殊的地址或者基于特殊的响应。
<a name="jvo25"></a>
### 过滤器的使用
首先需要实现 `Filter`接口然后重写它的三个方法

- `init` 方法：在容器中创建当前过滤器的时候自动调用
- `destory` 方法：在容器中销毁当前过滤器的时候自动调用
- `doFilter` 方法：过滤的具体操作

先引入 [Maven](http://mp.weixin.qq.com/s?__biz=MzI3ODcxMzQzMw==&mid=2247489209&idx=2&sn=8d740da1ccfa7090f6e7443b864bda06&chksm=eb53938fdc241a990dcf121baab3df982efe788010a5f2f034a5eb5930bdcdd5f2e1a3cb7201&scene=21#wechat_redirect) 依赖，其中 lombok 是用来避免每个文件创建 Logger 来打印日志
```xml
<dependency>
    <groupId>org.projectlombok</groupId>
    <artifactId>lombok</artifactId>
</dependency>
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-web</artifactId>
</dependency>
```
首先实现接口，重写三个方法，对包含要求的四个请求予以放行，将其它请求拦截重定向至`/online`，只要在将MyFilter实例化后即可，在后面整合代码中一起给出。
```java
import lombok.extern.log4j.Log4j2;
import org.springframework.stereotype.Component;
import javax.servlet.*;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpServletResponseWrapper;
import java.io.IOException;
@Log4j2
public class MyFilter implements Filter {
    @Override
    public void init(FilterConfig filterConfig) throws ServletException {
        log.info("初始化过滤器");
    }
  
    @Override
    public void doFilter(ServletRequest servletRequest, ServletResponse response, FilterChain filterChain) throws IOException, ServletException {
        HttpServletRequest request = (HttpServletRequest)servletRequest;
        HttpServletResponseWrapper wrapper = new HttpServletResponseWrapper((HttpServletResponse) response);
        String requestUri = request.getRequestURI();
        log.info("请求地址是："+requestUri);
        if (requestUri.contains("/addSession")
            || requestUri.contains("/removeSession")
            || requestUri.contains("/online")
            || requestUri.contains("/favicon.ico")) {
            filterChain.doFilter(servletRequest, response);
        } else {
            wrapper.sendRedirect("/online");
        }
    }
  
    @Override
    public void destroy() {
        //在服务关闭时销毁
        log.info("销毁过滤器");
    }
}
```
有了过滤器类以后，以前的web项目可以在web.xml中进行配置，但是SpringBoot项目并没有web.xml这个文件，那怎么配置？在SpringBoot中，需要`FilterRegistrationBean`来完成配置。其实现过程如下：
```java
@Configuration
public class FilterConfig {
    @Bean
    public FilterRegistrationBean registFilter() {
        FilterRegistrationBean registration = new FilterRegistrationBean();
        registration.setFilter(new LogCostFilter());
        registration.addUrlPatterns("/*");
        registration.setName("MyFilter");
        registration.setOrder(1);
        return registration;
    }
}
```
<a name="CkAP6"></a>
### 基于注解的形式实现过滤器
直接用`@WebFilter`就可以进行配置，同样，可以设置url匹配模式，过滤器名称等。这里需要注意一点的是`@WebFilter`这个注解是Servlet3.0的规范，并不是SpringBoot提供的。除了这个注解以外，还需在配置类中加另外一个注解：`@ServletComponetScan`，指定扫描的包。
```java
@SpringBootApplication
@MapperScan("com.fcant.blog.dao")
@ServletComponentScan("com.fcant.blog.filters")
public class Application {
    public static void main(String[] args) throws Exception {
        SpringApplication.run(Application.class, args);
    }
}
```
首先需要写一个方法继承`Filter`类，写如下两个自己的`Filter`类，首先是FirstFilter类，其中`@Order`里边的数字越小代表越先被该`Filter`过滤，`@WebFilter`代表这是个Filter类并把这个类注入到容器中：
```java
import java.io.IOException;
 
import javax.servlet.Filter;
import javax.servlet.FilterChain;
import javax.servlet.FilterConfig;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.annotation.WebFilter;
 
import org.springframework.core.annotation.Order;
 
@Order(1)
@WebFilter(filterName="firstFilter", urlPatterns="/*")
public class FirstFilter implements Filter {
 
    @Override
    public void init(FilterConfig filterConfig) throws ServletException {
        
    }
 
    @Override
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain)
            throws IOException, ServletException {
        System.out.println("first filter 1");
        chain.doFilter(request, response);
        System.out.println("first filter 2");
    }
 
    @Override
    public void destroy() {
        
    }
}
```
<a name="wvs4K"></a>
## 拦截器
Java中的拦截器是动态拦截 action 调用的对象，然后提供了可以在 action 执行前后增加一些操作，也可以在 action 执行前停止操作，功能与过滤器类似，但是标准和实现方式不同。

- 登录认证：在一些应用中，可能会通过拦截器来验证用户的登录状态，如果没有登录或者登录失败，就会给用户一个友好的提示或者返回登录页面，当然大型项目中都不采用这种方式，都是调单点登录系统接口来验证用户。
- 记录系统日志：在常见应用中，通常要记录用户的请求信息，比如请求 ip，方法执行时间等，通过这些记录可以监控系统的状况，以便于对系统进行信息监控、信息统计、计算 PV、性能调优等。
- 通用处理：在应用程序中可能存在所有方法都要返回的信息，这是可以利用拦截器来实现，省去每个方法冗余重复的代码实现。
<a name="pEUPq"></a>
### 使用拦截器
需要实现 `HandlerInterceptor` 接口，并且重写三个方法：

- `preHandle`：在 Controoler 处理请求之前被调用，返回值是 `boolean`类型，如果是`true`就进行下一步操作；若返回`false`，则证明不符合拦截条件，在失败的时候不会包含任何响应，此时需要调用对应的`response`返回对应响应。
- `postHandler`：在 Controoler 处理请求执行完成后、生成视图前执行，可以通过`ModelAndView`对视图进行处理，当然`ModelAndView`也可以设置为 null。
- `afterCompletion`：在 `DispatcherServlet` 完全处理请求后被调用，通常用于记录消耗时间，也可以对一些资源进行处理。
```java
import lombok.extern.log4j.Log4j2;
import org.springframework.stereotype.Component;
import org.springframework.web.servlet.HandlerInterceptor;
import org.springframework.web.servlet.ModelAndView;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
@Log4j2
@Component
public class MyInterceptor implements HandlerInterceptor {
    @Override
    public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler) throws Exception {
        log.info("【MyInterceptor】调用了:{}", request.getRequestURI());
        request.setAttribute("requestTime", System.currentTimeMillis());
        return true;
    }
    @Override
    public void postHandle(HttpServletRequest request, HttpServletResponse response,
                           Object handler, ModelAndView modelAndView) throws Exception {
        if (!request.getRequestURI().contains("/online")) {
            HttpSession session = request.getSession();
            String sessionName = (String) session.getAttribute("name");
            if ("haixiang".equals(sessionName)) {
                log.info("【MyInterceptor】当前浏览器存在 session:{}",sessionName);
            }
        }
    }
    @Override
    public void afterCompletion(HttpServletRequest request, HttpServletResponse response,
                                Object handler, Exception ex) throws Exception {
        long duration = (System.currentTimeMillis() - (Long)request.getAttribute("requestTime"));
        log.info("【MyInterceptor】[{}]调用耗时:{}ms",request.getRequestURI(), duration);
    }
}
```
<a name="5c70a7ac"></a>
## 监听器
监听器通常用于监听 Web 应用程序中对象的创建、销毁等动作的发送，同时对监听的情况作出相应的处理，最常用于统计网站的在线人数、访问量等。<br />监听器大概分为以下几种：

- `ServletContextListener`：用来监听 `ServletContext` 属性的操作，比如新增、修改、删除。
- `HttpSessionListener`：用来监听 Web 应用种的 Session 对象，通常用于统计在线情况。
- `ServletRequestListener`：用来监听 Request 对象的属性操作。
<a name="gDmla"></a>
### 监听器的使用
通过 `HttpSessionListener`来统计当前在线人数、ip等信息，为了避免并发问题使用原子int来计数。<br />`ServletContext`，是一个全局的储存信息的空间，它的生命周期与Servlet容器也就是服务器保持一致，服务器关闭才销毁。<br />request，一个用户可有多个；<br />session，一个用户一个；而`servletContext`，所有用户共用一个。所以，为了节省空间，提高效率，`ServletContext`中，要放必须的、重要的、所有用户需要共享的线程又是安全的一些信息。<br />因们这里用`ServletContext`来存储在线人数`sessionCount`最为合适。<br />来统计当前在线人数：
```java
import lombok.extern.log4j.Log4j2;
import javax.servlet.http.HttpSessionEvent;
import javax.servlet.http.HttpSessionListener;
import java.util.concurrent.atomic.AtomicInteger;
@Log4j2
public class MyHttpSessionListener implements HttpSessionListener {
    public static AtomicInteger userCount = new AtomicInteger(0);
    @Override
    public synchronized void sessionCreated(HttpSessionEvent se) {
        userCount.getAndIncrement();
        se.getSession().getServletContext().setAttribute("sessionCount", userCount.get());
        log.info("【在线人数】人数增加为:{}",userCount.get());
      
        //此处可以在ServletContext域对象中为访问量计数，然后传入过滤器的销毁方法
        //在销毁方法中调用数据库入库，因为过滤器生命周期与容器一致
    }
    @Override
    public synchronized void sessionDestroyed(HttpSessionEvent se) {
        userCount.getAndDecrement();
        se.getSession().getServletContext().setAttribute("sessionCount", userCount.get());
        log.info("【在线人数】人数减少为:{}",userCount.get());
    }
}
```
<a name="94duf"></a>
## 过滤器、拦截器、监听器注册
<a name="MZr78"></a>
### 实例化三器
```java
import com.anqi.tool.sanqi.filter.MyFilter;
import com.anqi.tool.sanqi.interceptor.MyInterceptor;
import com.anqi.tool.sanqi.listener.MyHttpRequestListener;
import com.anqi.tool.sanqi.listener.MyHttpSessionListener;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.web.servlet.FilterRegistrationBean;
import org.springframework.boot.web.servlet.ServletListenerRegistrationBean;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.web.servlet.config.annotation.InterceptorRegistry;
import org.springframework.web.servlet.config.annotation.WebMvcConfigurer;
@Configuration
public class WebConfig implements WebMvcConfigurer {
    @Autowired
    MyInterceptor myInterceptor;
    @Override
    public void addInterceptors(InterceptorRegistry registry) {
        registry.addInterceptor(myInterceptor);
    }
    /**
     * 注册过滤器
     * @return
     */
    @Bean
    public FilterRegistrationBean filterRegistrationBean(){
        FilterRegistrationBean filterRegistration = new FilterRegistrationBean();
        filterRegistration.setFilter(new MyFilter());
        filterRegistration.addUrlPatterns("/*");
        return filterRegistration;
    }
    /**
     * 注册监听器
     * @return
     */
    @Bean
    public ServletListenerRegistrationBean registrationBean(){
        ServletListenerRegistrationBean registrationBean = new ServletListenerRegistrationBean();
        registrationBean.setListener(new MyHttpRequestListener());
        registrationBean.setListener(new MyHttpSessionListener());
        return registrationBean;
    }
}
```
<a name="rjNHM"></a>
### 测试
```java
import com.anqi.tool.sanqi.listener.MyHttpSessionListener;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;
@RestController
public class TestController {
    @GetMapping("addSession")
    public String addSession(HttpServletRequest request) {
        HttpSession session = request.getSession();
        session.setAttribute("name", "haixiang");
        return "当前在线人数" + session.getServletContext().getAttribute("sessionCount") + "人";
    }
    @GetMapping("removeSession")
    public String removeSession(HttpServletRequest request) {
        HttpSession session = request.getSession();
        session.invalidate();
        return "当前在线人数" + session.getServletContext().getAttribute("sessionCount") + "人";
    }
    @GetMapping("online")
    public String online() {
        return "当前在线人数" + MyHttpSessionListener.userCount.get() + "人";
    }
}
```
以下是监听请求的监听器
```java
import javax.servlet.ServletRequestEvent;
import javax.servlet.ServletRequestListener;
import javax.servlet.http.HttpServletRequest;
public class MyHttpRequestListener implements ServletRequestListener {
    @Override
    public void requestDestroyed(ServletRequestEvent sre) {
        System.out.println("request 监听器被销毁");
    }
    @Override
    public void requestInitialized(ServletRequestEvent sre) {
        HttpServletRequest req = (HttpServletRequest) sre.getServletRequest();
        String requestURI = req.getRequestURI();
        System.out.println(requestURI+"--"+"被调用");
    }
}
```
<a name="fcdsT"></a>
## 拦截器与过滤器的区别
<a name="411111d5"></a>
### 1.参考标准

- 过滤器是 JavaEE 的标准，依赖于 [Servlet](http://mp.weixin.qq.com/s?__biz=MzI3ODcxMzQzMw==&mid=2247489342&idx=1&sn=20b3163e16f5a6b8abb770d022b22aff&chksm=eb539208dc241b1eb685b3f15ea4d769fb5765344ce2a80b3c901098f5094f4f9fc70e089d8b&scene=21#wechat_redirect) 容器，生命周期也与容器一致，利用这一特性可以在销毁时释放资源或者数据入库。
- 拦截器是[SpringMVC](http://mp.weixin.qq.com/s?__biz=MzI3ODcxMzQzMw==&mid=2247492894&idx=1&sn=bf97928fa07cf7fb2407298b4c4aec08&chksm=eb506028dc27e93e8d1e99c71687e69e1a93db4635a3006a2daeb3bd6c08f628ece61f9e90e6&scene=21#wechat_redirect)中的内容，依赖于web框架，通常用于验证用户权限或者记录日志，但是这些功能也可以利用 [AOP](http://mp.weixin.qq.com/s?__biz=MzI3ODcxMzQzMw==&mid=2247484078&idx=1&sn=cffb3397452fb781156120d27c960e02&chksm=eb538798dc240e8e2811bc5ea557ad50ff1133042898b0db6b04d87d5cb40b59071cca328ea5&scene=21#wechat_redirect) 来代替。
<a name="81daa82b"></a>
### 2.实现方式

- 过滤器是基于回调函数实现，无法注入 [ioc](http://mp.weixin.qq.com/s?__biz=MzI3ODcxMzQzMw==&mid=2247494583&idx=2&sn=91f2a54471721774ace59eee6c7cb2cd&chksm=eb506e81dc27e797916217007314823647ca72d64f7a178f7123a53975fd14a4d1963bee241d&scene=21#wechat_redirect) 容器中的 bean。
- 拦截器是基于反射来实现，因此拦截器中可以注入 [ioc](http://mp.weixin.qq.com/s?__biz=MzI3ODcxMzQzMw==&mid=2247494583&idx=2&sn=91f2a54471721774ace59eee6c7cb2cd&chksm=eb506e81dc27e797916217007314823647ca72d64f7a178f7123a53975fd14a4d1963bee241d&scene=21#wechat_redirect) 容器中的 bean，例如注入 Redis 的业务层来验证用户是否已经登录。
