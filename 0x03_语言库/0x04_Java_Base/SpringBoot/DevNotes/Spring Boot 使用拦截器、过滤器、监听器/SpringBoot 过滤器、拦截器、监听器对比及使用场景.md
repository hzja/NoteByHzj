Java SpringBoot 过滤器 拦截器 监听器
<a name="m4Hcc"></a>
# 一、关系图理解
![2021-09-20-21-57-49-551589.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632146304518-e23b499e-561f-4a47-b3f0-cf18243b937d.png#clientId=uec1752ec-576e-4&from=ui&id=u93f3f81f&originHeight=886&originWidth=1004&originalType=binary&ratio=1&size=311670&status=done&style=none&taskId=ucbe75563-9df4-44c0-a845-1b705cdf1cf)
<a name="MKSYs"></a>
# 二、区别
<a name="SRzPn"></a>
## 1.过滤器

- 过滤器是在web应用启动的时候初始化一次, 在web应用停止的时候销毁
- 可以对请求的URL进行过滤, 对敏感词过滤
- 挡在拦截器的外层
- 实现的是 javax.servlet.Filter 接口 ，是 Servlet 规范的一部分
- 在请求进入容器后，但在进入servlet之前进行预处理，请求结束是在servlet处理完以后
- 依赖Web容器
- 会多次执行
<a name="IghGK"></a>
### 1.1 `HttpServletRequestWrapper`
在请求到达之前对 request 进行修改
```java
import lombok.extern.slf4j.Slf4j;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletRequestWrapper;
import java.util.Arrays;

/**
 *  在请求到达之前对 request 进行修改
 */
@Slf4j
public class RequestWrapper extends HttpServletRequestWrapper {
    public RequestWrapper(HttpServletRequest request) {
        super(request);
        log.info("RequestWrapper");
    }

    @Override
    public String getParameter(String name) {
        // 可以对请求参数进行过滤
        return super.getParameter(name);
    }

    @Override
    public String[] getParameterValues(String name) {
        // 对请求参数值进行过滤
        //        String[] values =super.getRequest().getParameterValues(name);
        //        return super.getParameterValues(name);
        return "t e s t".split(" ");
    }


}
```
<a name="fsTX2"></a>
### 1.2 `OncePerRequestFilter`
`OncePerRequestFilter`，顾名思义，它能够确保在一次请求中只通过一次filter
```java
import lombok.extern.slf4j.Slf4j;
import org.springframework.web.filter.OncePerRequestFilter;

import javax.servlet.FilterChain;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Arrays;

/**
 * 请求过滤器
 * OncePerRequestFilter:
 * OncePerRequestFilter，顾名思义，它能够确保在一次请求中只通过一次filter.
 * 大家常识上都认为，一次请求本来就只filter一次，为什么还要由此特别限定呢，往往我们的常识和实际的实现并不真的一样，经过一番资料的查阅，此方法是为了兼容不同的web container，
 * 也就是说并不是所有的container都入我们期望的只过滤一次，servlet版本不同，执行过程也不同，
 * 因此，为了兼容各种不同运行环境和版本，默认filter继承OncePerRequestFilter是一个比较稳妥的选择。
 *
 */
@Slf4j
public class RequestFilter extends OncePerRequestFilter {


    @Override
    public void destroy() {
        super.destroy();
        log.info("RequestFilter destroy");
    }

    /*
            OncePerRequestFilter.doFilter方法中通过request.getAttribute判断当前过滤器是否已执行
            若未执行过，则调用doFilterInternal方法，交由其子类实现
        */
    @Override
    protected void doFilterInternal(HttpServletRequest httpServletRequest, HttpServletResponse httpServletResponse, FilterChain filterChain) throws ServletException, IOException {
        try {
            RequestWrapper requestWrapper = new RequestWrapper(httpServletRequest);
            filterChain.doFilter(requestWrapper, httpServletResponse);
            log.info("RequestFilter");
            log.info(Arrays.toString(requestWrapper.getParameterValues("name")));
        } catch (Exception exception) {
            httpServletResponse.setCharacterEncoding("utf-8");
            httpServletResponse.setContentType("application/json; charset=utf-8");
            PrintWriter writer = httpServletResponse.getWriter();
            writer.write(exception.toString());
        }
    }
}
```
<a name="sMRE1"></a>
### 1.3 配置
```java
import com.dingwen.lir.filter.RequestFilter;
import com.dingwen.lir.filter.RequestWrapper;
import org.springframework.boot.web.servlet.FilterRegistrationBean;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

import javax.servlet.Filter;

/**
 * 过滤器配置类
 *
 */
@Configuration
public class FilterConfig {

    @Bean
    public RequestFilter requestFilter(){
        return new RequestFilter();
    }
    @Bean
    public FilterRegistrationBean<RequestFilter> registrationBean() {
        FilterRegistrationBean<RequestFilter> registrationBean = new FilterRegistrationBean<>();
        registrationBean.setFilter(requestFilter());
        registrationBean.addUrlPatterns("/filter/*");
        registrationBean.setName("RequestFilter");
        //过滤器的级别，值越小级别越高越先执行
        registrationBean.setOrder(1);
        return registrationBean;
    }
}
```
<a name="Row5t"></a>
## 2.拦截器

- 实现 `org.springframework.web.servlet.HandlerInterceptor` 接口，动态代理
- 拦截器应用场景, 性能分析, 权限检查, 日志记录
- 是一个Spring组件，并由Spring容器管理，并不
- 依赖Tomcat等容器，是可以单独使用的。不仅能应用在web程序中，也可以用于Application、Swing等程序中
- 是在请求进入servlet后，在进入Controller之前进行预处理的，Controller 中渲染了对应的视图之后请求结束
<a name="AyAGq"></a>
### 2.1登录拦截
```java
import com.dingwen.lir.entity.User;
import org.springframework.stereotype.Component;
import org.springframework.util.ObjectUtils;
import org.springframework.web.servlet.HandlerInterceptor;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;

/**
 * 登录拦截
 *
 */
@Component
public class PageInterceptor implements HandlerInterceptor {
    @Override
    public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler) throws Exception {
        User user = (User)request.getSession().getAttribute("user");
        if (!ObjectUtils.isEmpty(user)) {
            return true;
        } else {
            // 不管是转发还是重定向，必须返回false。否则出现多次提交响应的错误
            redirect(request, response);
            return false;
        }
    }

    /*
     * 对于请求是ajax请求重定向问题的处理方法
     * @param request
     * @param response
     *
     */
    public void redirect(HttpServletRequest request, HttpServletResponse response) throws IOException {

        if("XMLHttpRequest".equals(request.getHeader("X-Requested-With"))){// ajax
            //获取当前请求的路径
            response.setHeader("Access-Control-Expose-Headers", "REDIRECT,CONTENT_PATH");
            //告诉ajax我是重定向
            response.setHeader("REDIRECT", "REDIRECT");
            //告诉ajax我重定向的路径
            StringBuffer url = request.getRequestURL();
            String contextPath = request.getContextPath();
            response.setHeader("CONTENT_PATH", url.replace(url.indexOf(contextPath) + contextPath.length(), url.length(), "/").toString());
        }else{// http
            response.sendRedirect( "/page/login");
        }

        response.getWriter().write(403);
        response.setStatus(HttpServletResponse.SC_FORBIDDEN);
    }
}
```
<a name="Vy4sj"></a>
### 2.2配置
```java
import com.dingwen.lir.interceptor.PageInterceptor;
import org.springframework.context.annotation.Configuration;
import org.springframework.web.servlet.config.annotation.InterceptorRegistry;
import org.springframework.web.servlet.config.annotation.ResourceHandlerRegistry;
import org.springframework.web.servlet.config.annotation.ViewControllerRegistry;
import org.springframework.web.servlet.config.annotation.WebMvcConfigurer;

/**
 * mvc 控制器配置
 * MyWebMvcConfigurer: Springboot2.x以后版本使用
 *
 */
@Configuration
public class MyWebMvcConfigurer implements WebMvcConfigurer {

    /*
     * 拦截器依赖于Spring容器，此处拦截了所有，需要对静态资源进行放行
     */
    @Override
    public void addInterceptors(InterceptorRegistry registry) {
        // 拦截器默认的执行顺序，就是它的注册顺序，也可以通过Order手动设置控制，值越小越先执行。
        //        registry.addInterceptor(new PageInterceptor()).addPathPatterns("/**").order()
        registry.addInterceptor(new PageInterceptor()).addPathPatterns("/**")
            .excludePathPatterns("/page/login", "/user/login","/page/ajax","/static/**");
    }


    /*
     * 不要要写控制器即可完成页面跳转访问
     * @param registry
     */
    @Override
    public void addViewControllers(ViewControllerRegistry registry) {
        registry.addViewController("/page/ajax").setViewName("ajax");
    }


    /*
     * 自定义静态资源映射
        Spring Boot 默认为我们提供了静态资源映射：
                classpath:/META-INF/resources
                classpath:/resources
                classpath:/static
                classpath:/public
              优先级：META-INF/resources > resources > static > public
     * @param registry
     *
     */
    //    @Override
    //    public void addResourceHandlers(ResourceHandlerRegistry registry) {
    registry.addResourceHandler("/static/**").addResourceLocations("classpath:/static/");
    registry.addResourceHandler("/static/**").addResourceLocations("file:E:/static/");
    //    }
}
```
<a name="BCzdN"></a>
## 3.监听器

- 实现 `javax.servlet.ServletRequestListener`, `javax.servlet.http.HttpSessionListener`, `javax.servlet.ServletContextListener` 等等接口
- 主要用来监听对象的创建与销毁的发生, 比如 session 的创建销毁, request 的创建销毁, ServletContext 创建销毁
<a name="AVxIv"></a>
# 三、注意
<a name="AHhci"></a>
## 1.静态资源问题
SpringBoot2.x以后版本拦截器也会拦截静态资源，在配置拦截器是需要将姿态资源放行。
```java
/*
* 拦截器依赖于Spring容器，此处拦截了所有，需要对静态资源进行放行
*/
@Override
public void addInterceptors(InterceptorRegistry registry) {
    registry.addInterceptor(new PageInterceptor()).addPathPatterns("/**")
        .excludePathPatterns("/page/login", "/user/login","/page/ajax","/static/**");
}
```
SpringBoot2.x 自定义静态资源映射
```yaml
spring:
  mvc:
    static-path-pattern: /static/**
```
默认目录 `classpath:/META-INF/resources` `classpath:/resources` `classpath:/static` `classpath:/public` 优先级：META-INF/resources > resources > static > public
<a name="kfBto"></a>
## 2.登录拦截ajax重定向
由于ajax是异步的，还在当前页面进行的局部请求。当拦截到登录请求时，即使重定向也无法生效。需采用服务端给地址由前端进行跳转。详细见登录拦截器代码。
```html
// 前端处理
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>AJAX</title>
    <script src="https://code.jquery.com/jquery-3.0.0.min.js"></script>
</head>
<body>
    <button>USER</button>
</body>
</html>

<script>
    $.ajaxSetup({
        complete:function(xhr,status){
            //拦截器实现超时跳转到登录页面
            let win = window;
            // 通过xhr取得响应头
            let REDIRECT = xhr.getResponseHeader("REDIRECT");
            //如果响应头中包含 REDIRECT 则说明是拦截器返回的需要重定向的请求
            if (REDIRECT === "REDIRECT")
            {
                while (win !== win.top)
                {
                    win = win.top;
                }
                win.location.href = xhr.getResponseHeader("CONTEXTPATH");
            }
        }
    });
    $("button").click(function(){
        $.get("/page/user", function(result){
            $("div").html(result);
        });
    });
</script>
```
