Java SpringBoot
<a name="MDQ3X"></a>
## 1、SpringBoot 实现登录拦截的原理
SpringBoot 通过实现`HandlerInterceptor`接口实现拦截器，通过实现`WebMvcConfigurer`接口实现一个配置类，在配置类中注入拦截器，最后再通过 `@Configuration` 注解注入配置。
<a name="j7SiY"></a>
### 1.1、实现HandlerInterceptor接口
实现`HandlerInterceptor`接口需要实现 3 个方法：`preHandle`、`postHandle`、`afterCompletion`。<br />3 个方法各自的功能如下：
```java
import blog.entity.User;
import org.springframework.web.servlet.HandlerInterceptor;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

public class UserLoginInterceptor implements HandlerInterceptor {

    /***
     * 在请求处理之前进行调用(Controller方法调用之前)
     */
    @Override
    public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler) throws Exception {
        System.out.println("执行了拦截器的preHandle方法");
        try {
            HttpSession session = request.getSession();
            //统一拦截（查询当前session是否存在user）(这里user会在每次登录成功后，写入session)
            User user = (User) session.getAttribute("user");
            if (user != null) {
                return true;
            }
            response.sendRedirect(request.getContextPath() + "login");
        } catch (Exception e) {
            e.printStackTrace();
        }
        return false;
        //如果设置为false时，被请求时，拦截器执行到此处将不会继续操作
        //如果设置为true时，请求将会继续执行后面的操作
    }

    /***
     * 请求处理之后进行调用，但是在视图被渲染之前（Controller方法调用之后）
     */
    @Override
    public void postHandle(HttpServletRequest request, HttpServletResponse response, Object handler, ModelAndView modelAndView) throws Exception {
        System.out.println("执行了拦截器的postHandle方法");
    }

    /***
     * 整个请求结束之后被调用，也就是在DispatchServlet渲染了对应的视图之后执行（主要用于进行资源清理工作）
     */
    @Override
    public void afterCompletion(HttpServletRequest request, HttpServletResponse response, Object handler, Exception ex) throws Exception {
        System.out.println("执行了拦截器的afterCompletion方法");
    }
}
```
`preHandle`在 Controller 之前执行，因此拦截器的功能主要就是在这个部分实现：

1. 检查 session 中是否有user对象存在；
2. 如果存在，就返回true，那么 Controller 就会继续后面的操作；
3. 如果不存在，就会重定向到登录界面<br />就是通过这个拦截器，使得 Controller 在执行之前，都执行一遍preHandle。
<a name="DM3Ej"></a>
### 1.2、实现WebMvcConfigurer接口，注册拦截器
实现`WebMvcConfigurer`接口来实现一个配置类，将上面实现的拦截器的一个对象注册到这个配置类中。
```java
import blog.interceptor.UserLoginInterceptor;
import org.springframework.context.annotation.Configuration;
import org.springframework.web.servlet.config.annotation.InterceptorRegistration;
import org.springframework.web.servlet.config.annotation.InterceptorRegistry;
import org.springframework.web.servlet.config.annotation.WebMvcConfigurer;

@Configuration
public class LoginConfig implements WebMvcConfigurer {

    @Override
    public void addInterceptors(InterceptorRegistry registry) {
        //注册TestInterceptor拦截器
        InterceptorRegistration registration = registry.addInterceptor(new UserLoginInterceptor());
        registration.addPathPatterns("/**"); //所有路径都被拦截
        registration.excludePathPatterns(    //添加不拦截路径
                "/login",                    //登录路径
                "/**/*.html",                //html静态资源
                "/**/*.js",                  //js静态资源
                "/**/*.css"                  //css静态资源
        );
    }
}
```
将拦截器注册到了拦截器列表中，并且指明了拦截哪些访问路径，不拦截哪些访问路径，不拦截哪些资源文件；最后再以 `@Configuration` 注解将配置注入。
<a name="HjPK9"></a>
### 1.3、保持登录状态
只需一次登录，如果登录过，下一次再访问的时候就无需再次进行登录拦截，可以直接访问网站里面的内容了。<br />在正确登录之后，就将user保存到session中，再次访问页面的时候，登录拦截器就可以找到这个user对象，就不需要再次拦截到登录界面了。
```java
@RequestMapping(value = {"", "/", "/index"}, method = RequestMethod.GET)
public String index(Model model, HttpServletRequest request) {
    User user = (User) request.getSession().getAttribute("user");
    model.addAttribute("user", user);
    return "users/index";
}

@RequestMapping(value = {"/login"}, method = RequestMethod.GET)
public String loginIndex() {
    return "users/login";
}

@RequestMapping(value = {"/login"}, method = RequestMethod.POST)
public String login(@RequestParam(name = "username")String username, @RequestParam(name = "password")String password,
                    Model model, HttpServletRequest request) {
    User user = userService.getPwdByUsername(username);
    String pwd = user.getPassword();
    String password1 = MD5Utils.md5Code(password).toUpperCase();
    String password2 = MD5Utils.md5Code(password1).toUpperCase();
    if (pwd.equals(password2)) {
        model.addAttribute("user", user);
        request.getSession().setAttribute("user", user);
        return "redirect:/index";
    } else {
        return "users/failed";
    }
}
```
<a name="U5Jrd"></a>
## 2、代码实现及示例
代码实现如上所示。<br />在登录成功之后，将user信息保存到session中，下一次登录时浏览器根据自己的SESSIONID就可以找到对应的session，就不要再次登录了，可以从 Chrome 浏览器中看到。<br />![2021-07-31-18-24-51-063456.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627727173756-67b7e18e-39ff-4d62-b726-66e96a22e90b.png#clientId=ud993b521-41da-4&from=ui&id=ufa593cf1&originHeight=657&originWidth=1080&originalType=binary&ratio=1&size=2132849&status=done&style=none&taskId=ud0ec9bbe-d934-4392-931c-aa47928d386)
<a name="fsFCc"></a>
## 3、效果验证
<a name="R5pKE"></a>
### 3.1、访问 localhost:8081/index 页面：
![2021-07-31-18-24-51-235454.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627727164109-325af5f5-a037-41ee-ae9f-344bfe8db266.png#clientId=ud993b521-41da-4&from=ui&id=u00e54e56&originHeight=369&originWidth=1080&originalType=binary&ratio=1&size=1197945&status=done&style=shadow&taskId=u6a1e1624-0147-4879-8496-521b26f4396)<br />被重定向到了 localhost:8081/login，实现了登录拦截。
<a name="u9Esh"></a>
### 3.2、正确输入用户名和密码登录
![2021-07-31-18-24-51-391465.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627727181931-01ce86fb-b359-4285-9f34-49cd6335c299.png#clientId=ud993b521-41da-4&from=ui&id=ubee399b5&originHeight=368&originWidth=1080&originalType=binary&ratio=1&size=1194692&status=done&style=shadow&taskId=u2031d709-c265-49bc-b489-43e256efa6b)
<a name="LNq3i"></a>
### 3.3、再次访问 localhost:8081/index
![2021-07-31-18-24-51-529456.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627727191040-1db5503f-9afa-46b3-a284-42005375caa7.png#clientId=ud993b521-41da-4&from=ui&id=u3a528160&originHeight=289&originWidth=1080&originalType=binary&ratio=1&size=938241&status=done&style=shadow&taskId=u4017fef6-13d4-4eac-a179-23d9c93fd9d)<br />没有再次被登录拦截器拦截，证明可以保持登录。
