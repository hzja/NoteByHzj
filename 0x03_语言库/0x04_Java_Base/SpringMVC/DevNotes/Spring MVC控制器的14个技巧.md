Java SpringMVC<br />通常，在Spring MVC中，编写一个控制器类来处理来自客户端的请求。然后，控制器调用业务类来处理与业务相关的任务，然后将客户端重定向到逻辑视图名称，该名称由Spring的调度程序Servlet解析，以呈现结果或输出。这样就完成了典型的请求-响应周期的往返。
<a name="bOAxI"></a>
## 1、使用`@Controller`构造型
这是创建可以处理一个或多个请求的控制器类的最简单方法。仅通过用构造型注释一个类`@Controller`，例如：
```java
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
@Controller
public class HomeController {
    @RequestMapping("/")
    public String visitHome() {
      
        return "home";
    }
}
```
`visitHome()`方法通过重定向到名为home的视图来处理来自应用程序上下文路径(/)的请求。<br />注意：`@Controller`原型只能在Spring的配置文件中启用注解驱动时使用：
```xml
<annotation-driven />
```
启用注释驱动时，Spring容器自动在以下语句指定的包下扫描类：
```xml
<context:component-scan base-package="net.codejava.spring" />
```
由`@Controller`注释注释的类被配置为控制器。这是最可取的，因为它很简单：无需在配置文件中为控制器声明bean。<br />注意：通过使用`@Controller`注解，可以拥有一个多动作控制器类，该类能够处理多个不同的请求。例如：
```java
@Controller
public class MultiActionController {
    @RequestMapping("/listUsers")
    public ModelAndView listUsers() {
    }
    @RequestMapping("/saveUser")
    public ModelAndView saveUser(User user) {
    }
    @RequestMapping("/deleteUser")
    public ModelAndView deleteUser(User user) {
    }
}
```
正如可以在上面的控制器类看，有处理三种不同的请求3种处理方法`/listUsers`，`/saveUser`，和`/deleteUser`分别。
<a name="m2Ot1"></a>
## 2、实现控制器接口
在Spring MVC中创建控制器的另一种（也许是经典的）方法是让类实现  Controller 接口。例如：
```java
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.springframework.web.servlet.ModelAndView;
import org.springframework.web.servlet.mvc.Controller;
public class MainController implements Controller {
    @Override
    public ModelAndView handleRequest(HttpServletRequest request,
            HttpServletResponse response) throws Exception {
        System.out.println("Welcome main");
        return new ModelAndView("main");
    }
}
```
实现类必须重写该  `handleRequest()` 方法，当匹配请求进入时，该方法将由Spring调度程序Servlet调用。此控制器处理的请求URL模式在Spring的上下文配置文件中定义如下：
```xml
<bean name="/main" class="net.codejava.spring.MainController" />
```
但是，此方法的缺点是控制器类无法处理多个请求URL。
<a name="xMBp0"></a>
## 3、扩展`AbstractController`类
如果要轻松控制受支持的HTTP方法，会话和内容缓存。扩展控制器`AbstractController`类是理想的选择。请考虑以下示例：
```java
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.springframework.web.servlet.ModelAndView;
import org.springframework.web.servlet.mvc.AbstractController;
public class BigController extends AbstractController {
    @Override
    protected ModelAndView handleRequestInternal(HttpServletRequest request,
            HttpServletResponse response) throws Exception {
        System.out.println("You're big!");
        return new ModelAndView("big");
    }
}
```
这将创建具有有关受支持的方法，会话和缓存的配置的单动作控制器，然后可以在控制器的bean声明中指定这些配置。例如：
```xml
<bean name="/big" class="net.codejava.spring.BigController">
    <property name="supportedMethods" value="POST"/>
</bean>
```
此配置指示POST 此控制器的`hander`方法仅支持该方法。<br />Spring MVC还提供了几种针对特定目的而设计的控制器类，包括：

- `AbstractUrlViewController`
- `MultiActionController`
- `ParameterizableViewController`
- `ServletForwardingController`
- `ServletWrappingController`
- `UrlFilenameViewController`
<a name="Nf8TS"></a>
## 4、为处理程序方法指定URL映射
这是编码控制器类时必须执行的强制性任务，该控制器类旨在处理一个或多个特定请求。Spring MVC提供了`@RequestMapping`注释，该注解用于指定URL映射。例如：
```java
@RequestMapping("/login")
```
这映射了/login要由带注解的方法或类处理的URL模式。当在类级别使用此注解时，该类将成为单动作控制器。例如：
```java
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
@Controller
@RequestMapping("/hello")
public class SingleActionController {
    @RequestMapping(method = RequestMethod.GET)
    public String sayHello() {
        return "hello";
    }
}
```
当`@RequestMapping`注解在方法级别使用的，可以有一个多动作控制器。例如：
```java
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
@Controller
public class UserController {
    @RequestMapping("/listUsers")
    public String listUsers() {
        return "ListUsers";
    }
    @RequestMapping("/saveUser")
    public String saveUser() {
        return "EditUser";
    }
    @RequestMapping("/deleteUser")
    public String deleteUser() {
        return "DeleteUser";
    }
}
```
`@RequestMapping`注释还可以用于指定一个方法要处理的多个URL模式。例如：
```java
@RequestMapping({"/hello", "/hi", "/greetings"})
```
此外，此注解还具有在某些情况下可能有用的其他属性，例如method。
<a name="bgX79"></a>
## 5、为处理程序方法指定HTTP请求方法
可以使用注解的method属性指定处理程序方法支持哪种HTTP方法（GET，POST，PUT等）`@RequestMapping`。这是一个例子：
```java
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
@Controller
public class LoginController {
    @RequestMapping(value = "/login", method = RequestMethod.GET)
    public String viewLogin() {
        return "LoginForm";
    }
    @RequestMapping(value = "/login", method = RequestMethod.POST)
    public String doLogin() {
        return "Home";
    }
}
```
此控制器有两个处理相同URL模式的方法/login，但前者用于GET方法，而后者用于POST方法。<br />有关使用`@RequestMapping`注解的更多信息，请参见`@RequestMapping`注解。
<a name="RkLDe"></a>
## 6、将请求参数映射到处理程序方法
Spring MVC的很酷的功能之一是，可以使用`@RequestParam`注解将请求参数作为处理程序方法的常规参数进行检索。这是将控制器`HttpServletRequest`与Servlet API 的接口分离的好方法。
```java
@RequestMapping(value = "/login", method = RequestMethod.POST)
public String doLogin(@RequestParam String username,
                      @RequestParam String password) {
}
```
Spring将方法参数用户名和密码绑定到具有相同名称的HTTP请求参数。这意味着可以按以下方式调用URL（如果请求方法是GET）：
```http
http://localhost8080/spring/loginusername=Fcant＆password=tiger
```
类型转换也是自动完成的。例如，如果声明integer如下类型的参数：
```java
@RequestParam int securityNumber
```
然后，Spring将在处理程序方法中自动将请求参数（字符串）的值转换为指定的类型（整数）。<br />如果参数名称与变量名称不同，则可以如下指定参数的实际名称：
```java
@RequestParam("SSN") int securityNumber
```
该`@RequestParam`注解也有两个额外的属性，这可能是在某些情况下是有用的。该属性指定参数是否为必需。例如：required
```java
@RequestParam(required = false) String country
```
这意味着该参数  country 是可选的；因此，它可能会从请求中丢失。在上面的示例中，country如果请求中不存在此类参数，则变量  将为null。<br />另一个属性是`defaultValue`，可以在请求参数为空时用作后备值。例如：
```java
@RequestParam(defaultValue = "18") int age
```
Map如果方法参数是`type`，Spring还允许将所有参数作为对象访问`Map<String, String>`。例如：
```java
doLogin(@RequestParam Map<String, String> params)
```
然后，映射参数包含键-值对形式的所有请求参数。有关使用`@RequestParam`注释的更多信息，请参见`@RequestParam`注解。
<a name="PrkvS"></a>
## 7、返回模型和视图
处理完业务逻辑后，处理程序方法应返回一个视图，然后由Spring的调度程序servlet对其进行解析。Spring允许`ModelAndView` 从handler方法中返回String或对象。在以下示例中，该handler方法返回一个String并表示一个名为的视图LoginForm：
```java
@RequestMapping(value = "/login", method = RequestMethod.GET)
public String viewLogin() {
    return "LoginForm";
}
```
这是返回视图名称的最简单方法。但是，如果要将其他数据发送到视图，则必须返回一个ModelAndView对象。考虑以下处理程序方法：
```java
@RequestMapping("/listUsers")
public ModelAndView listUsers() {
    List<User> listUser = new ArrayList<>();
    // 从DAO获取用户列表…
    ModelAndView modelView = new ModelAndView("UserList");
    modelView.addObject("listUser", listUser);
    return modelView;
}
```
此处理程序方法返回一个ModelAndView保存视图名称UserList的User对象和一个可在视图中使用的对象集合。<br />Spring也非常灵活，可以将`ModelAndView`对象声明为处理程序方法的参数，而不用创建一个新的对象。因此，以上示例可以重写如下：
```java
@RequestMapping("/listUsers")
public ModelAndView listUsers(ModelAndView modelView) {
    List<User> listUser = new ArrayList<>();
    //从DAO获取用户列表…
    modelView.setViewName("UserList");
    modelView.addObject("listUser", listUser);
    return modelView;
}
```
了解有关该类的更多信息，请参见：ModelAndView class。
<a name="mvzfY"></a>
## 8、将对象放入模型
在遵循MVC架构的应用程序中，控制器（C）应该将数据传递到模型（M）中，然后在视图（V）中使用该模型。正如在前面的示例中看到的那样，该类的`addObject()`方法`ModelAndView`是以名称-值对的形式将对象放入模型中：
```java
modelView.addObject("listUser", listUser);
modelView.addObject("siteName", new String("CodeJava.net"));
modelView.addObject("users", 1200000);
```
同样，Spring非常灵活。可以Map在处理程序方法中声明类型的参数。Spring使用此映射存储模型的对象。看另一个例子：
```java
@RequestMapping(method = RequestMethod.GET)
public String viewStats(Map<String, Object> model) {
    model.put("siteName", "CodeJava.net");
    model.put("pageviews", 320000);
    return "Stats";
}
```
这比使用`ModelAndView`对象还要简单。根据喜好，可以使用Map或使用`ModelAndView`对象。在这里要感谢Spring的灵活性。
<a name="Vix6O"></a>
## 9、处理程序方法中的重定向
如果希望在满足条件的情况下将用户重定向到另一个URL，请`redirect:/`在URL之前追加。以下代码段给出了一个示例：
```java
// 检查登录状态....
if (!isLogin) {
    return new ModelAndView("redirect:/login");
}
// 返回用户列表
```
在上面的代码中，/login 如果未登录，用户将被重定向到该URL。
<a name="kKQG1"></a>
## 10、处理表格提交和表格验证
通过提供`@ModelAttribute`用于将表单字段绑定到表单支持对象的注解以及`BindingResult`用于验证表单字段的界面，Spring使处理表单提交变得容易。下面的代码片段显示了一种典型的处理程序方法，该方法负责处理和验证表单数据：
```java
@Controller
public class RegistrationController {
    @RequestMapping(value = "/doRegister", method = RequestMethod.POST)
    public String doRegister(
        @ModelAttribute("userForm") User user, BindingResult bindingResult) {
        if (bindingResult.hasErrors()) {
            // 表单验证错误
        } else {
            // 表单输入没问题
        }
        // 注册过程……
        return "Success";
    }
}
```
从Spring的官方文档中了解有关`@ModelAttribute`注释和`BindingResult`接口的更多信息：

- 在方法参数上使用`@ModelAttribute`
- 在方法上使用`@ModelAttribute`
- 接口绑定结果
<a name="EgEcD"></a>
## 11、处理文件上传
通过自动将上传数据绑定到`CommonsMultipartFile`对象数组，Spring还使在处理程序方法中处理文件上传变得容易。Spring使用Apache Commons FileUpload作为基础的多部分解析器。<br />以下代码段显示了从客户端上传文件有多么容易
```java
@RequestMapping(value = "/uploadFiles", method = RequestMethod.POST)
public String handleFileUpload(
        @RequestParam CommonsMultipartFile[] fileUpload) throws Exception {
    for (CommonsMultipartFile aFile : fileUpload){
        // 存储上传的文件
        aFile.transferTo(new File(aFile.getOriginalFilename()));
    }
    return "Success";
}
```
<a name="x1HDq"></a>
## 12、在控制器中自动装配业务类
控制器应将业务逻辑的处理委托给相关的业务类。为此，可以使用`@Autowired`注解让Spring自动将业务类的实际实现注入控制器。考虑以下控制器类的代码段：
```java
@Controller
public class UserController {
    @Autowired
    private UserDAO userDAO;
    public String listUser() {
        // 列出所有用户的处理方法
        userDAO.list();
    }
    public String saveUser(User user) {
        // 保存/更新用户的处理方法
        userDAO.save(user);
    }
    public String deleteUser(User user) {
        // 删除用户的处理方法
        userDAO.delete(user);
    }
    public String getUser(int userId) {
        // 获取用户的处理方法
        userDAO.get(userId);
    }
}
```
在此，与用户管理有关的所有业务逻辑都由该UserDAO接口的实现提供。例如：
```java
interface UserDAO {
    List<User> list();
    void save(User user);
    void checkLogin(User user);
}
```
有关`@Autowired`注解的更多信息，请参见注释类型自动装配。
<a name="aw2jb"></a>
## 13、访问`HttpServletRequest`和`HttpServletResponse`
在某些情况下，需要直接在处理程序方法中访问`HttpServletRequest`或`HttpServletResponse`对象。通过Spring的灵活性，只需在处理方法中添加相关参数即可。例如：
```java
@RequestMapping("/download")
public String doDownloadFile(
        HttpServletRequest request, HttpServletResponse response) {
    // 访问请求
    // 访问响应
    return "DownloadPage";
}
```
Spring检测并自动将`HttpServletRequest`和`HttpServletResponse`对象注入方法中。然后，可以访问请求和响应如获取`InputStream`，`OutputStream`或返回一个特定的HTTP代码。
<a name="tIQnd"></a>
## 14、遵循单一责任原则
最后，在设计和编写Spring MVC控制器时，有两个很好的实践是应该遵循的：

- 控制器类不应执行业务逻辑。相反，它应该将业务处理委托给相关的业务类别。这使控制器始终专注于其设计职责是控制应用程序的工作流程。例如：
```java
@Controller
public class UserController {
    @Autowired
    private UserDAO userDAO;
    public String listUser() {
        userDAO.list();
    }
    public String saveUser(User user) {
        userDAO.save(user);
    }
    public String deleteUser(User user) {
        userDAO.delete(user);
    }
    public String getUser(int userId) {
        userDAO.get(userId);
    }
}
```

- 为每个业务域创建每个单独的控制器。例如，UserController用于控制用户管理的OrderController工作流程，  用于控制订单处理的工作流程等。例如：
```java
@Controller
public class UserController {
}
@Controller
public class ProductController {
}
@Controller
public class OrderController {
}
@Controller
public class PaymentController {
}
```
