JavaSpringBoot
<a name="O3jmC"></a>
# 1、前言
Springboot跨域问题，是当前主流web开发人员都绕不开的难题。但首先要明确以下几点

- 跨域只存在于浏览器端，不存在于安卓/ios/Node.js/python/ java等其它环境
- 跨域请求能发出去，服务端能收到请求并正常返回结果，只是结果被浏览器拦截了。
- 之所以会跨域，是因为受到了同源策略的限制，同源策略要求源相同才能正常进行通信，即协议、域名、端口号都完全一致。

浏览器出于安全的考虑，使用 XMLHttpRequest对象发起 HTTP请求时必须遵守同源策略，否则就是跨域的HTTP请求，默认情况下是被禁止的。换句话说，浏览器安全的基石是同源策略。<br />同源策略限制了从同一个源加载的文档或脚本如何与来自另一个源的资源进行交互。这是一个用于隔离潜在恶意文件的重要安全机制。
<a name="u1neg"></a>
# 2、什么是CORS？
CORS是一个W3C标准，全称是”跨域资源共享”（Cross-origin resource sharing），允许浏览器向跨源服务器，发出XMLHttpRequest请求，从而克服了AJAX只能同源使用的限制。<br />它通过服务器增加一个特殊的`Header[Access-Control-Allow-Origin]`来告诉客户端跨域的限制，如果浏览器支持CORS、并且判断Origin通过的话，就会允许XMLHttpRequest发起跨域请求。<br />**CORS Header**

- Access-Control-Allow-Origin: http://www.xxx.com
- Access-Control-Max-Age：86400
- Access-Control-Allow-Methods：GET, POST, OPTIONS, PUT, DELETE
- Access-Control-Allow-Headers: content-type
- Access-Control-Allow-Credentials: true
| CORS Header属性 | 解释 |
| --- | --- |
| Access-Control-Allow-Origin  | 允许http://www.xxx.com域(自行设置，这里只做示例）发起跨域请求 |
| Access-Control-Max-Age | 设置在86400秒不需要再发送预校验请求 |
| Access-Control-Allow-Methods | 设置允许跨域请求的方法 |
| Access-Control-Allow-Headers | 允许跨域请求包含content-type |
| Access-Control-Allow-Credentials | 设置允许Cookie |

<a name="v6gXl"></a>
# 3、SpringBoot跨域请求处理方式
<a name="m2uey"></a>
### 3.1 方法一、直接采用SpringBoot的注解`@CrossOrigin`（也支持SpringMVC）
简单粗暴的方式，Controller层在需要跨域的类或者方法上加上该注解即可
```java
@RestController
@CrossOrigin
@RequestMapping("/situation")
public class SituationController extends PublicUtilController {

    @Autowired
    private SituationService situationService;
    // log日志信息
    private static Logger LOGGER = Logger.getLogger(SituationController.class);

}
```
但每个Controller都得加，太麻烦了，怎么办呢，加在Controller公共父类（`PublicUtilController`）中，所有Controller继承即可。
```java
@CrossOrigin
public class PublicUtilController {

    /**
     * 公共分页参数整理接口
     *
     * @param currentPage
     * @param pageSize
     * @return
     */
    public PageInfoUtil proccedPageInfo(String currentPage, String pageSize) {

        /* 分页 */
        PageInfoUtil pageInfoUtil = new PageInfoUtil();
        try {
            /*
             * 将字符串转换成整数,有风险, 字符串为a,转换不成整数
             */
            pageInfoUtil.setCurrentPage(Integer.valueOf(currentPage));
            pageInfoUtil.setPageSize(Integer.valueOf(pageSize));
        } catch (NumberFormatException e) {
        }
        return pageInfoUtil;
    }

}
```
当然，这里虽然指SpringBoot，SpringMVC也是同样的，但要求在Spring4.2及以上的版本。另外，如果SpringMVC框架版本不方便修改，也可以通过修改tomcat的web.xml配置文件来处理<br />SpringMVC使用`@CrossOrigin`使用场景要求

- jdk1.8+
- Spring4.2+
<a name="ZkMvZ"></a>
### 3.2 方法二、处理跨域请求的`Configuration`
增加一个配置类，CrossOriginConfig.java。继承`WebMvcConfigurerAdapter`或者实现`WebMvcConfigurer`接口，其他都不用管，项目启动时，会自动读取配置。
```java
@Configuration
public class CorsConfig extends WebMvcConfigurerAdapter {
    static final String ORIGINS[] = new String[] { "GET", "POST", "PUT", "DELETE" };
    @Override
    public void addCorsMappings(CorsRegistry registry) {
        registry.addMapping("/**").allowedOrigins("*").allowCredentials(true).allowedMethods(ORIGINS).maxAge(3600);
    }
}
```
<a name="Io8NL"></a>
### 3.3 方法三、采用过滤器（filter）的方式
同方法二加配置类，增加一个`CORSFilter `类，并实现Filter接口即可，其他都不用管，接口调用时，会过滤跨域的拦截。
```java
@Component
public class CORSFilter implements Filter {

    @Override
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain)
    throws IOException, ServletException {
        HttpServletResponse res = (HttpServletResponse) response;
        res.addHeader("Access-Control-Allow-Credentials", "true");
        res.addHeader("Access-Control-Allow-Origin", "*");
        res.addHeader("Access-Control-Allow-Methods", "GET, POST, DELETE, PUT");
        res.addHeader("Access-Control-Allow-Headers", "Content-Type,X-CAF-Authorization-Token,sessionToken,X-TOKEN");
        if (((HttpServletRequest) request).getMethod().equals("OPTIONS")) {
            response.getWriter().println("ok");
            return;
        }
        chain.doFilter(request, response);
    }
    @Override
    public void destroy() {
    }
    @Override
    public void init(FilterConfig filterConfig) throws ServletException {
    }
}
```
