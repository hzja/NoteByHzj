Java SpringMVC
<a name="FSwSF"></a>
## 1、自定义拦截器实现`HandlerInterceptor`接口
```java
package com.fcant.ssmcom.interceptor;

import com.fcant.ssmcom.bean.User;
import org.springframework.web.servlet.HandlerInterceptor;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

/**
 * LoginHandlerIntercepter
 * <p>
 * encoding:UTF-8
 *
 * @author Fcscanf
 * @description
 * @date 下午 17:01 2019-08-07/0007
 */
public class LoginHandlerInterceptor implements HandlerInterceptor {

    /**
     * 配置不要拦截的URL
     */
    private final String[] URI = {"/page/login", "/user/login"};

    @Override
    public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler) throws Exception {
        StringBuffer requestURL = request.getRequestURL();
        //URL:除了login.jsp是可以公开访问的，其余的URL都要进行拦截控制
        if (requestURL.indexOf(URI[0]) > 0 || requestURL.indexOf(URI[1]) > 0) {
            return true;
        }
        //获取session
        HttpSession session = request.getSession();
        User user = (User) session.getAttribute("user");
        //判断session里是不是有登录信息
        if (user != null) {
            return true;
        }
        request.setAttribute("msg", "你还没有登录，请先登录！");
        request.getRequestDispatcher("/WEB-INF/jsp/login.jsp").forward(request, response);
        return false;
    }

    @Override
    public void postHandle(HttpServletRequest request, HttpServletResponse response, Object handler, ModelAndView modelAndView) throws Exception {

    }

    @Override
    public void afterCompletion(HttpServletRequest request, HttpServletResponse response, Object handler, Exception ex) throws Exception {

    }
}

```
<a name="iX8Lj"></a>
## 2、编写Controller在Session保存用户信息
```java
package com.fcant.ssmcom.controller;

import com.fcant.ssmcom.bean.User;
import com.fcant.ssmcom.service.UserService;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpSession;

/**
 * UserController
 * <p>
 * encoding:UTF-8
 *
 * @author Fcscanf
 * @description
 * @date 下午 19:35 2019-08-06/0006
 */
@Controller
@RequestMapping("/user")
@Slf4j
public class UserController {

    @Autowired
    UserService userService;

    @RequestMapping("/login")
    public ModelAndView regUser(User user, HttpSession session) {
        ModelAndView modelAndView = new ModelAndView();
        if (user.getNumber() == null || "".equals(user.getNumber())) {
            modelAndView.addObject("msg", "请输入正确的工号/学号！");
            modelAndView.setViewName("login");
            return modelAndView;
        } else {
            // 查询符合条件的用户-检查用户的存在性
            User selectByNumber = userService.selectByNumber(user);
            if (selectByNumber == null) {
                modelAndView.addObject("msg", "未找到符合条件的用户，请输入正确的工号/学号！");
                modelAndView.setViewName("login");
                return modelAndView;
            }else {
                // 校验用户的密码信息
                if (selectByNumber.getPassword().equals(user.getPassword())) {
                    // 用户名密码正确则根据用户类型跳转至不同的管理页面
                    // 将用户信息保存至Session中
                    session.setAttribute("user", selectByNumber);
                    switch (selectByNumber.getrId()) {
                        case 1:
                            log.info("Student Login ：" + selectByNumber);
                            modelAndView.setViewName("student");
                            return modelAndView;
                        case 2:
                            log.info("Teacher Login ：" + selectByNumber);
                            modelAndView.setViewName("teacher");
                            return modelAndView;
                        default :
                            log.info("Admin Login ：" + selectByNumber);
                            modelAndView.setViewName("admin");
                            return modelAndView;
                    }
                }else {
                    modelAndView.addObject("msg", "请输入正确的密码！");
                    modelAndView.setViewName("login");
                    return modelAndView;
                }
            }
        }
    }

    @RequestMapping(value = "/signOut")
    public String signOut(HttpSession session, Model model) {
        model.addAttribute("msg", "您已退出登录！");
        session.removeAttribute("user");
        return "login";
    }
}

```
<a name="BgB2m"></a>
## 3、在spring-mvc.xml配置拦截器
```xml
<!-- 配置登陆拦截器 -->
<mvc:interceptors>
  <mvc:interceptor>
    <mvc:mapping path="/**"/>
    <bean class="com.fcant.ssmcom.interceptor.LoginHandlerInterceptor" />
  </mvc:interceptor>
</mvc:interceptors>
```
