Java SpringBoot 跨域
<a name="PvQVt"></a>
## 一、为什么会出现跨域问题
出于浏览器的同源策略限制。同源策略（Sameoriginpolicy）是一种约定，它是浏览器最核心也最基本的安全功能，如果缺少了同源策略，则浏览器的正常功能可能都会受到影响。可以说Web是构建在同源策略基础之上的，浏览器只是针对同源策略的一种实现。<br />同源策略会阻止一个域的javascript脚本和另外一个域的内容进行交互。所谓同源（即指在同一个域）就是两个页面具有相同的协议（protocol），主机（host）和端口号（port）
<a name="bxwFC"></a>
## 二、什么是跨域
当一个请求url的协议、域名、端口三者之间任意一个与当前页面url不同即为跨域

| 当前页面url | 被请求页面url | 是否跨域 | 原因 |
| --- | --- | --- | --- |
| http://www.test.com/ | http://www.test.com/index.html | 否 | 同源（协议、域名、端口号相同) |
| http://www.test.com/ | https://www.test.com/index.html | 跨域 | 协议不同(http/https) |
| http://www.test.com/ | http://twww.baidu.com/ | 跨域 | 主域名不同(test/baidu) |
| http://www.test.com/ | http://blog.test.com/ | 跨域 | 子域名不同(www/blog) |
| http://www.test.com:8080/ | http://www.test.com:7001/ | 跨域 | 端口号不同(8080/7001) |

<a name="YCrhh"></a>
## 三、非同源限制
【1】无法读取非同源网页的 `Cookie`、`LocalStorage` 和 `IndexedDB`<br />【2】无法接触非同源网页的 DOM<br />【3】无法向非同源地址发送 AJAX 请求
<a name="qrPH9"></a>
## 四、问题背景
Same Origin Policy，译为“同源策略”。它是对于客户端脚本（尤其是JavaScript）的重要安全度量标准，其目的在于防止某个文档或者脚本从多个不同“origin”（源）装载。它认为自任何站点装载的信赖内容是不安全的。<br />当被浏览器半信半疑的脚本运行在沙箱时，它们应该只被允许访问来自同一站点的资源，而不是那些来自其它站点可能怀有恶意的资源。
:::info
注：具有相同的Origin，也即是拥有相同的协议、主机地址以及端口。一旦这三项数据中有一项不同，那么该资源就将被认为是从不同的Origin得来的，进而不被允许访问。
:::
CORS就是为了解决SOP问题而生的，当然CORS不是唯一的解决方案。
<a name="f7f17459"></a>
## 五、CORS简介
> CORS是一个W3C标准，全称是"跨域资源共享”（Cross-origin resource sharing）。它允许浏览器向跨源(协议 + 域名 + 端口)服务器，发出XMLHttpRequest请求，从而克服了AJAX只能同源使用的限制。CORS需要浏览器和服务器同时支持。它的通信过程，都是浏览器自动完成，不需要用户参与。
> 对于开发者来说，CORS通信与同源的AJAX/Fetch通信没有差别，代码完全一样。浏览器一旦发现请求跨源，就会自动添加一些附加的头信息，有时还会多出一次附加的请求，但用户不会有感觉。因此，实现CORS通信的关键是服务器。只要服务器实现了CORS接口，就可以跨源通信。

浏览器将CORS请求分成两类：简单请求（simple request）和非简单请求（not-so-simple request）。
> 浏览器发出CORS简单请求，只需要在头信息之中增加一个Origin字段。
> 浏览器发出CORS非简单请求，会在正式通信之前，增加一次OPTIONS查询请求，称为"预检"请求（preflight）。浏览器先询问服务器，当前网页所在的域名是否在服务器的许可名单之中，以及可以使用哪些HTTP动词和头信息字段。只有得到肯定答复，浏览器才会发出正式的XMLHttpRequest请求，否则就报错。

简单请求就是HEAD、GET、POST请求，并且HTTP的头信息不超出以下几种字段 Accept、Accept-Language、Content-Language、Last-Event-ID、Content-Type 
:::success
注：Content-Type：只限于三个值application/x-www-form-urlencoded、multipart/form-data、text/plain
:::
反之，就是非简单请求。<br />其实实现CORS很简单，就是在服务端加一些响应头，并且这样做对前端来说是无感知的，很方便。
<a name="IkJ4X"></a>
## 六、详解响应头

- `Access-Control-Allow-Origin` 该字段必填。它的值要么是请求时Origin字段的具体值，要么是一个*，表示接受任意域名的请求。
- `Access-Control-Allow-Methods` 该字段必填。它的值是逗号分隔的一个具体的字符串或者*，表明服务器支持的所有跨域请求的方法。注意，返回的是所有支持的方法，而不单是浏览器请求的那个方法。这是为了避免多次"预检"请求。
- `Access-Control-Expose-Headers` 该字段可选。CORS请求时，XMLHttpRequest对象的`getResponseHeader()`方法只能拿到6个基本字段：Cache-Control、Content-Language、Content-Type、Expires、Last-Modified、Pragma。如果想拿到其他字段，就必须在Access-Control-Expose-Headers里面指定。
- `Access-Control-Allow-Credentials` 该字段可选。它的值是一个布尔值，表示是否允许发送Cookie.默认情况下，不发生Cookie，即：false。对服务器有特殊要求的请求，比如请求方法是PUT或DELETE，或者Content-Type字段的类型是application/json，这个值只能设为true。如果服务器不要浏览器发送Cookie，删除该字段即可。
- `Access-Control-Max-Age` 该字段可选，用来指定本次预检请求的有效期，单位为秒。在有效期间，不用发出另一条预检请求。
:::warning
如果在开发中，发现每次发起请求都是两条，一次OPTIONS，一次正常请求，注意是每次，那么就需要配置Access-Control-Max-Age，避免每次都发出预检请求。
:::
<a name="wmfU6"></a>
## 七、Java 后端 实现 CORS 跨域请求的方式
对于 CORS的跨域请求，主要有以下几种方式可供选择：

1. 返回新的`CorsFilter`
2. 重写 `WebMvcConfigurer`
3. 使用注解 `@CrossOrigin`
4. 手动设置响应头 (HttpServletResponse)
5. 自定web filter 实现跨域

注意:

- `CorFilter` / `WebMvConfigurer` / `@CrossOrigin` 需要 SpringMVC 4.2以上版本才支持，对应SpringBoot 1.3版本以上
- 上面前两种方式属于全局 CORS 配置，后两种属于局部 CORS配置。如果使用了局部跨域是会覆盖全局跨域的规则，所以可以通过 `@CrossOrigin` 注解来进行细粒度更高的跨域资源控制。
- 其实无论哪种方案，最终目的都是修改响应头，向响应头中添加浏览器所要求的数据，进而实现跨域
<a name="LZ1I6"></a>
## 八、解决办法
<a name="28a59534"></a>
### 1、局部跨域配置
<a name="e97c2ae5"></a>
#### 直接添加`@CrossOrigin`注解
直接添加`@CrossOrigin`注解后，所有主机和端口可以访问。配置修改属性后只有指定的主机可以访问
```java
@CrossOrigin
@RestController
@RequestMapping("/file")
public class FileController {

    @RequestMapping("/down")
    public void fileDownload() {
        
    }
}
```
`@CrossOrigin`注解指定相关属性
```java
@RestController
@RequestMapping("/file")
@CrossOrigin(origins = "http://www.fcant.com:8081", maxAge = 3600)
public class FileController {

    @RequestMapping("/down")
    public void fileDownload() {
        
    }
}
```
<a name="7n3f3"></a>
#### 手动设置响应头
使用 `HttpServletResponse` 对象添加响应头(Access-Control-Allow-Origin)来授权原始域，这里 Origin的值也可以设置为 “*”,表示全部放行。
```java
@RequestMapping("/index")
public String index(HttpServletResponse response) {
    response.addHeader("Access-Allow-Control-Origin","*");
    return "index";
}
```
<a name="cqGsn"></a>
### 2、设置全局配置类
<a name="IFIsQ"></a>
#### 方式一：`new WebMvcConfigurer()`
```java
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.web.servlet.config.annotation.CorsRegistry;
import org.springframework.web.servlet.config.annotation.WebMvcConfigurer;

import static org.springframework.web.cors.CorsConfiguration.ALL;

/**
 * CustomCORSConfiguration
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant
 * @date 上午 10:33 2019-09-17/0017
 */
@Configuration
public class CustomCORSConfiguration {
    @Bean
    public WebMvcConfigurer corsConfigurer() {
        return new WebMvcConfigurer() {
            @Override
            public void addCorsMappings(CorsRegistry registry) {
                // 限制了路径和域名的访问
                /*registry.addMapping("/api*").allowedOrigins("http://localhost:8081");*/
                registry.addMapping("/**")
                        .allowedOrigins(ALL)
                        .allowedMethods(ALL)
                        .allowedHeaders(ALL)
                        .allowCredentials(true);
            }
        };
    }
}
```
<a name="O3jTu"></a>
#### 方式二：重写`WebMvcConfigurer`
```java
@Configuration
public class CorsConfig implements WebMvcConfigurer {
    @Override
    public void addCorsMappings(CorsRegistry registry) {
        registry.addMapping("/**")
            //是否发送Cookie
            .allowCredentials(true)
            //放行哪些原始域
            .allowedOrigins("*")
            .allowedMethods(new String[]{"GET", "POST", "PUT", "DELETE"})
            .allowedHeaders("*")
            .exposedHeaders("*");
    }
}
```
<a name="K28Hl"></a>
#### 方式三：继承`WebMvcConfigurerAdapter`
```java
@Configuration
public class WebMvcConfig extends WebMvcConfigurerAdapter {
    @Override
    public void addCorsMappings(CorsRegistry registry) {
        registry.addMapping("/**")
                .allowedOrigins("*")
                .allowedMethods("POST", "GET", "PUT", "OPTIONS", "DELETE")
                .maxAge(3600)
                .allowCredentials(true);
    }
}
```
<a name="06gVx"></a>
#### 方式四：`new CorsFilter(configurationSource);`
```java
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.web.cors.CorsConfiguration;
import org.springframework.web.cors.UrlBasedCorsConfigurationSource;
import org.springframework.web.filter.CorsFilter;

/**
 * CorsAutoConfiguration
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 上午 10:22 2021/1/5/0005
 */
@Configuration
public class CorsAutoConfiguration {

    @Bean
    public CorsFilter corsFilter(){
        //1. 添加 CORS配置信息
        CorsConfiguration config = new CorsConfiguration();
        //放行哪些原始域
        config.addAllowedOrigin("*");
        //是否发送 Cookie
        config.setAllowCredentials(true);
        //放行哪些请求方式
        config.addAllowedMethod("*");
        //放行哪些原始请求头部信息
        config.addAllowedHeader("*");
        //暴露哪些头部信息
        config.addExposedHeader("*");
        //2. 添加映射路径
        UrlBasedCorsConfigurationSource corsConfigurationSource = new UrlBasedCorsConfigurationSource();
        corsConfigurationSource.registerCorsConfiguration("/**",config);
        //3. 返回新的CorsFilter
        return new CorsFilter(corsConfigurationSource);
    }
}
```
<a name="mPy2M"></a>
### 3、通过自定义Filter配置
```java
import java.io.IOException;
import javax.servlet.Filter;
import javax.servlet.FilterChain;
import javax.servlet.FilterConfig;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.http.HttpServletResponse;
import org.springframework.stereotype.Component;

@Component
@WebFilter(urlPatterns = "/*", filterName = "authFilter") //这里的“/*” 表示的是需要拦截的请求路径
public class PassHttpFilter implements Filter {
    @Override
    public void init(FilterConfig filterConfig) throws ServletException { }
    @Override
    public void doFilter(ServletRequest servletRequest, ServletResponse servletResponse, FilterChain filterChain) throws IOException, ServletException {
        HttpServletResponse httpResponse = (HttpServletResponse)servletResponse;
        httpResponse.setHeader("Access-Control-Allow-Headers","Origin, X-Requested-With, Content-Type, Accept");
        httpResponse.setHeader("Access-Control-Allow-Credentials", "true");
        httpResponse.setHeader("Access-Control-Max-Age", "3600");
        httpResponse.setHeader("Access-Control-Allow-Methods", "POST, GET, PATCH, DELETE, PUT");
        httpResponse.setHeader("Access-Control-Allow-Origin", "http://127.0.0.1:8080/*");
        filterChain.doFilter(servletRequest, httpResponse);
    }
    @Override
    public void destroy() { }
}
```
在web.xml中配置这个过滤器，使其生效
```xml
<!-- 跨域访问 START-->  
<filter>  
  <filter-name>CorsFilter</filter-name>  
  <filter-class>com.mesnac.aop.MyCorsFilter</filter-class>  
</filter>  
<filter-mapping>  
  <filter-name>CorsFilter</filter-name>  
  <url-pattern>/*</url-pattern>  
</filter-mapping>  
<!-- 跨域访问 END  -->
```
