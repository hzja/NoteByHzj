SpringBoot 拦截器<br />拦截器的原理很简单，是 AOP 的一种实现，专门拦截对动态资源的后台请求，即拦截对控制层的请求。使用场景比较多的是判断用户是否有权限请求后台，更拔高一层的使用场景也有，比如拦截器可以结合 WebSocket 一起使用，用来拦截 websocket 请求，然后做相应的处理等等。拦截器不会拦截静态资源，Spring Boot 的默认静态目录为 `resources/static`，该目录下的静态页面、js、css、图片等等，不会被拦截（也要看如何实现，有些情况也会拦截）。
<a name="pF7Bn"></a>
## 1、拦截器的快速使用
使用拦截器很简单，只需要两步即可：定义拦截器和配置拦截器。在配置拦截器中，Spring Boot 2.0 以后的版本和之前的版本有所不同。
<a name="QaIQl"></a>
### 1.1 定义拦截器
定义拦截器，只需要实现 `HandlerInterceptor` 接口，`HandlerInterceptor` 接口是所有自定义拦截器或者 Spring Boot 提供的拦截器的鼻祖，所以，首先来了解下该接口。该接口中有三个方法： `preHandle(……)`、`postHandle(……)` 和 `afterCompletion(……)` 。
> `preHandle(……)` 方法：该方法的执行时机是，当某个 url 已经匹配到对应的 Controller 中的某个方法，且在这个方法执行之前。所以 `preHandle(……)` 方法可以决定是否将请求放行，这是通过返回值来决定的，返回 true 则放行，返回 false 则不会向后执行。
> `postHandle(……)` 方法：该方法的执行时机是，当某个 url 已经匹配到对应的 Controller 中的某个方法，且在执行完了该方法，但是在 DispatcherServlet 视图渲染之前。所以在这个方法中有个 ModelAndView 参数，可以在此做一些修改动作。
> `afterCompletion(……)` 方法：顾名思义，该方法是在整个请求处理完成后（包括视图渲染）执行，这时做一些资源的清理工作，这个方法只有在 `preHandle(……)` 被成功执行后并且返回 true 才会被执行。

了解了该接口，接下来自定义一个拦截器。
```java
public class MyInterceptor implements HandlerInterceptor {
    private static final Logger logger = LoggerFactory.getLogger(MyInterceptor.class);
    @Override
    public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler) throws Exception {
        HandlerMethod handlerMethod = (HandlerMethod) handler;
        Method method = handlerMethod.getMethod();
        String methodName = method.getName();
        logger.info("====拦截到了方法：{}，在该方法执行之前执行====", methodName);
        // 返回true才会继续执行，返回false则取消当前请求
        return true;
    }
    @Override
    public void postHandle(HttpServletRequest request, HttpServletResponse response, Object handler, ModelAndView modelAndView) throws Exception {
        logger.info("执行完方法之后进执行(Controller方法调用之后)，但是此时还没进行视图渲染");
    }
    @Override
    public void afterCompletion(HttpServletRequest request, HttpServletResponse response, Object handler, Exception ex) throws Exception {
        logger.info("整个请求都处理完咯，DispatcherServlet也渲染了对应的视图，此时可以做一些清理的工作了");
    }
}
```
拦截器已经定义完成，接下来就是对该拦截器进行拦截配置。
<a name="yx19o"></a>
### 1.2 配置拦截器
在 Spring Boot 2.0 之前，都是直接继承 `WebMvcConfigurerAdapter` 类，然后重写 `addInterceptors` 方法来实现拦截器的配置。但是在 Spring Boot 2.0 之后，该方法已经被废弃了（当然，也可以继续用），取而代之的是 `WebMvcConfigurationSupport` 方法，如下：
```java
@Configuration
public class MyInterceptorConfig extends WebMvcConfigurationSupport {
    @Override
    protected void addInterceptors(InterceptorRegistry registry) {
        registry.addInterceptor(new MyInterceptor()).addPathPatterns("/**");
        super.addInterceptors(registry);
    }
}
```
在该配置中重写 `addInterceptors` 方法，将上面自定义的拦截器添加进去，`addPathPatterns` 方法是添加要拦截的请求，这里拦截所有的请求。这样就配置好拦截器了，接下来写一个 Controller 测试一下：
```java
@Controller
@RequestMapping("/interceptor")
public class InterceptorController {
    @RequestMapping("/test")
    public String test() {
        return "hello";
    }
}
```
让其跳转到 hello.html 页面，直接在 hello.html 中输出 hello interceptor 即可。启动项目，在浏览器中输入 `localhost:8080/interceptor/test` 看一下控制台的日志：
```
====拦截到了方法：test，在该方法执行之前执行====  
执行完方法之后进执行(Controller方法调用之后)，但是此时还没进行视图渲染  
整个请求都处理完咯，DispatcherServlet也渲染了对应的视图，此时可以做一些清理的工作了
```
可以看出拦截器已经生效，并能看出其执行顺序。
<a name="JoMUM"></a>
### 1.3 解决静态资源被拦截问题
上面定义的拦截器在使用的时候会发现静态资源被拦截了。可以在 `resources/static/` 目录下放置一个图片资源或者 html 文件，然后启动项目直接访问，即可看到无法访问的现象。<br />也就是说，虽然 Spring Boot 2.0 废弃了`WebMvcConfigurerAdapter`，但是 `WebMvcConfigurationSupport` 又会导致默认的静态资源被拦截，这就需要手动将静态资源放开。<br />除了在 MyInterceptorConfig 配置类中重写 `addInterceptors` 方法外，还需要再重写一个方法：`addResourceHandlers`，将静态资源放开：
```java
/**
 * 用来指定静态资源不被拦截，否则继承WebMvcConfigurationSupport这种方式会导致静态资源无法直接访问
 * @param registry
 */
@Override
protected void addResourceHandlers(ResourceHandlerRegistry registry) {
    registry.addResourceHandler("/**").addResourceLocations("classpath:/static/");
    super.addResourceHandlers(registry);
}
```
这样配置好之后，重启项目，静态资源也可以正常访问了。但是，还有更方便的方式来配置。<br />不继承 `WebMvcConfigurationSupport` 类，直接实现 `WebMvcConfigurer` 接口，然后重写 `addInterceptors` 方法，将自定义的拦截器添加进去即可，如下：
```java
@Configuration
public class MyInterceptorConfig implements WebMvcConfigurer {
    @Override
    public void addInterceptors(InterceptorRegistry registry) {
        // 实现WebMvcConfigurer不会导致静态资源被拦截
        registry.addInterceptor(new MyInterceptor()).addPathPatterns("/**");
    }
}
```
这样就非常方便了，实现 `WebMvcConfigure` 接口的话，不会拦截 Spring Boot 默认的静态资源。<br />这两种方式都可以，由于这两种方式的不同，继承 `WebMvcConfigurationSupport` 类的方式可以用在前后端分离的项目中，后台不需要访问静态资源（就不需要放开静态资源了）；实现 `WebMvcConfigure` 接口的方式可以用在非前后端分离的项目中，因为需要读取一些图片、css、js文件等等。
<a name="hNh6B"></a>
## 2、拦截器使用实例
<a name="2Zlia"></a>
### 2.1 判断用户有没有登录
一般用户登录功能可以这么做，要么往 session 中写一个 user，要么针对每个 user 生成一个 token，第二种要更好一点，那么针对第二种方式，如果用户登录成功了，每次请求的时候都会带上该用户的 token，如果未登录，则没有该 token，服务端可以检测这个 token 参数的有无来判断用户有没有登录，从而实现拦截功能。改造一下 `preHandle` 方法，如下：
```java
@Override
public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler) throws Exception {
    HandlerMethod handlerMethod = (HandlerMethod) handler;
    Method method = handlerMethod.getMethod();
    String methodName = method.getName();
    logger.info("====拦截到了方法：{}，在该方法执行之前执行====", methodName);
    // 判断用户有没有登陆，一般登陆之后的用户都有一个对应的token
    String token = request.getParameter("token");
    if (null == token || "".equals(token)) {
        logger.info("用户未登录，没有权限执行……请登录");
        return false;
    }
    // 返回true才会继续执行，返回false则取消当前请求
    return true;
}
```
重启项目，在浏览器中输入 `localhost:8080/interceptor/test` 后查看控制台日志，发现被拦截，如果在浏览器中输入 `localhost:8080/interceptor/test?token=123` 即可正常往下走。
<a name="rqpBh"></a>
### 2.2 取消拦截操作
根据上文，如果要拦截所有 `/admin` 开头的 url 请求的话，需要在拦截器配置中添加这个前缀，但是在实际项目中，可能会有这种场景出现：某个请求也是 `/admin` 开头的，但是不能拦截，比如 `/admin/login` 等等，这样的话又需要去配置。那么，可不可以做成一个类似于开关的东西，哪里不需要拦截，就在哪里弄个开关上去，做成这种灵活的可插拔的效果呢？<br />实际上是可以的，可以定义一个注解，该注解专门用来取消拦截操作，如果某个 Controller 中的方法不需要拦截掉，即可在该方法上加上自定义的注解即可，下面先定义一个注解：
```java
/**
 * 该注解用来指定某个方法不用拦截
 */
@Target(ElementType.METHOD)
@Retention(RetentionPolicy.RUNTIME)
public @interface UnInterception {
}
```
然后在 Controller 中的某个方法上添加该注解，在拦截器处理方法中添加该注解取消拦截的逻辑，如下：
```java
@Override
public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler) throws Exception {
    HandlerMethod handlerMethod = (HandlerMethod) handler;
    Method method = handlerMethod.getMethod();
    String methodName = method.getName();
    logger.info("====拦截到了方法：{}，在该方法执行之前执行====", methodName);
    // 通过方法，可以获取该方法上的自定义注解，然后通过注解来判断该方法是否要被拦截
    // @UnInterception 是自定义的注解
    UnInterception unInterception = method.getAnnotation(UnInterception.class);
    if (null != unInterception) {
        return true;
    }
    // 返回true才会继续执行，返回false则取消当前请求
    return true;
}
```
Controller 中的方法代码可以参见源码，重启项目在浏览器中输入 `http://localhost:8080/interceptor/test2?token=123` 测试一下，可以看出，加了该注解的方法不会被拦截。
<a name="Y1svv"></a>
### 2.3 基于URL实现的拦截器
```java
public class LoginInterceptor extends HandlerInterceptorAdapter{
 /**
     * 在请求处理之前进行调用（Controller方法调用之前）
     * 基于URL实现的拦截器
     * @param request
     * @param response
     * @param handler
     * @return
     * @throws Exception
     */
    @Override
    public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler) throws Exception {
        String path = request.getServletPath();
        if (path.matches(Const.NO_INTERCEPTOR_PATH)) {
         //不需要的拦截直接过
            return true;
        } else {
         // 这写拦截后需要做的事儿，比如取缓存，SESSION，权限判断等
            System.out.println("====================================");
            return true;
        }
    }
}
```
关键代码：`path.matches(Const.NO_INTERCEPTOR_PATH` 就是基于正则匹配的url。
```java
/**
 * @explain 常量类
 */
public class Const {

    public static final String SUCCESS = "SUCCESS";
    public static final String ERROR = "ERROR";
    public static final String FIALL = "FIALL";
    /**********************对象和个体****************************/
    public static final String SESSION_USER = "loginedAgent"; // 用户对象
    public static final String SESSION_LOGINID = "sessionLoginID"; // 登录ID
    public static final String SESSION_USERID = "sessionUserID"; // 当前用户对象ID编号

    public static final String SESSION_USERNAME = "sessionUserName"; // 当前用户对象ID编号
    public static final Integer PAGE = 10; // 默认分页数
    public static final String SESSION_URL = "sessionUrl"; // 被记录的url
    public static final String SESSION_SECURITY_CODE = "sessionVerifyCode"; // 登录页验证码
    // 时间 缓存时间
    public static final int TIMEOUT = 1800;// 秒
 public static final String ON_LOGIN = "/logout.htm";
 public static final String LOGIN_OUT = "/toLogout";
    // 不验证URL anon：不验证/authc：受控制的
    public static final String NO_INTERCEPTOR_PATH =".*/((.css)|(.js)|(images)|(login)|(anon)).*";
}
```
<a name="tQqcL"></a>
### 2.4 基于注解的拦截器
<a name="B35Vk"></a>
#### ①创建注解
```java
/**
 * 在需要登录验证的Controller的方法上使用此注解
 */
@Target({ElementType.METHOD})// 可用在方法名上
@Retention(RetentionPolicy.RUNTIME)// 运行时有效
public @interface LoginRequired {

}
```
<a name="Iadbh"></a>
#### ②创建拦截器
```java
public class AuthorityInterceptor extends HandlerInterceptorAdapter{

  @Override
  public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler) throws Exception {
   // 如果不是映射到方法直接通过
        if (!(handler instanceof HandlerMethod)) {
            return true;
        }
        // ①:START 方法注解级拦截器
        HandlerMethod handlerMethod = (HandlerMethod) handler;
        Method method = handlerMethod.getMethod();
        // 判断接口是否需要登录
        LoginRequired methodAnnotation = method.getAnnotation(LoginRequired.class);
        // 有 @LoginRequired 注解，需要认证
        if (methodAnnotation != null) {
            // 这写你拦截需要干的事儿，比如取缓存，SESSION，权限判断等
            System.out.println("====================================");
            return true;
        }
        return true;
 }
}
```
<a name="bbHuk"></a>
#### ③把拦截器添加到配置中，相当于SpringMVC时的配置文件干的事
```java
/**
 * 和springmvc的webmvc拦截配置一样
 */
@Configuration
public class WebConfigurer implements WebMvcConfigurer {
    @Override
    public void addInterceptors(InterceptorRegistry registry) {
        // 拦截所有请求，通过判断是否有 @LoginRequired 注解 决定是否需要登录
        registry.addInterceptor(LoginInterceptor()).addPathPatterns("/**");
        registry.addInterceptor(AuthorityInterceptor()).addPathPatterns("/**");
    }

    @Bean
    public LoginInterceptor LoginInterceptor() {
        return new LoginInterceptor();
    }

    @Bean
    public AuthorityInterceptor AuthorityInterceptor() {
        return new AuthorityInterceptor();
    }
}
```
1、一定要加`@Configuration` 这个注解，在启动的时候在会被加载。<br />2、有一些教程是用的“`WebMvcConfigurerAdapter`”，不过在spring5.0版本后这个类被丢弃了 `WebMvcConfigurerAdapter`，虽然还可以用，但是看起来不好。<br />3、也有一些教程使用的`WebMvcConfigurationSupport`，使用后发现，`classpath:/META/resources/`，`classpath:/resources/`，`classpath:/static/`，`classpath:/public/`）不生效。具体可以原因，可以看下源码因为：`WebMvcAutoConfiguration`上有个条件注解：
```java
@ConditionalOnMissingBean(WebMvcConfigurationSupport.class)
```
所以还是建议使用`WebMvcConfigurer`。
<a name="4PTAT"></a>
## 3、总结
Spring Boot 2.0 之后拦截器的配置支持两种方式，可以根据实际情况选择不同的配置方式。
