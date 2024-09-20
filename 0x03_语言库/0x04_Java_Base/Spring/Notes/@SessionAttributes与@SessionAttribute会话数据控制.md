JavaWeb<br />`session`存储也是后端比较重要的一个知识点，面试的时候常常会拿来与cookie对比，还有一些分布式`Session`，单点登录等问题，不过使用Spring的时候，它已经给Session做了很多方便的操作，其中就有`@SessionAttribues`和`@SessionAttribute`注解。
<a name="NJnf9"></a>
## 1、简介
`@SessionAttributes`用于在会话中存储Model的属性，一般作用在类的级别。像下面的代码，model的属性user会被存储到`session`中，因为`@ModelAttribute`与`@SessionAttributes`有相同的注解。
```java
@Controller
@SessionAttributes("user")
public class ModelController {

    @ModelAttribute("user")
    public User initUser(){
        User user = new User();
        user.setName("default");
        return user;
    }

}
```
`@SessionAttribute`是用于获取已经存储的session数据，并且作用在方法的层面上。
```java
@RequestMapping("/session")
public String session(@SessionAttribute("user") User user){
    // do something
    return "index";
}
```
<a name="MChqE"></a>
## 2、实例
这个时候，已经有能运行的项目了。接下来准备代码

1. 准备java文件。先准备`@SessionAttributes`的文件，用于存储session
```java
@Controller
@RequestMapping("/model")
@SessionAttributes("user")
public class ModelController {

    @ModelAttribute("user")
    public User initUser(){
        User user = new User();
        user.setName("default");
        return user;
    }

    @RequestMapping("/parameter")
    public String parameter(
            @ModelAttribute("user") User user
    ) {

        return "index";
    }

}
```

2. 准备`@SessionAttribute`的文件，用于检索`session`，以验证注解是否正确。
```java
@Controller
public class SessionController {
    @RequestMapping("/session")
    public String session(
            @SessionAttribute("user") User user,
            HttpServletRequest request
    ){
        return "index";
    }
}
```
<a name="u8lVP"></a>
## 3、进行测试
不经过`@SessionAttributes`会直接报错<br />先经过`@SessionAttributes`<br />首先访问/model/parameter的url<br />然后访问/session的url。这个地址没有传递任何参数，可以看到从session中获取user对象成功了
