JavaSpringMVC<br />当 http 请求被自定义的 controller 处理时，如何指定响应的页面呢？<br />在 controller 中响应页面有很多种方式，稍后会一一介绍，需要掌握每种方式的用法以及这些方式之间的区别，以后能够灵活使用。<br />本文用到的页面都以 jsp 为例，其他页面模板技术，比如 freemarker、velocity、thymeleaf、enjoy，这些后面专门再讲解。
<a name="fqopZ"></a>
## servlet 中响应页面的 2 种方式
SpringMVC 底层是依靠 servlet 来实现的，所以先回顾下 servlet 中响应页面是如何实现的。<br />servlet 中响应页面有 2 种常见的方式，而 SpringMVC 中通常也是依靠这 2 种方式实现的。
<a name="pCGXD"></a>
### 方式 1：转向
```java
request.getRequestDispatcher(path).forward(request,response);
```
1、path 为转向的地址<br />2、发生在服务器端，浏览器的地址栏不会发生变化<br />3、path 指定的页面，可以共享 request 请求中的数据<br />4、path 必须是服务器端的资源
<a name="u5Wle"></a>
### 方式 2：重定向
```java
response.sendRedirect(location);
```
1、location 为重定向的地址<br />2、重定向发生在客户端（浏览器端），所以会导致浏览器地址栏发生变化，变为 location 指定的地址<br />3、重定向会导致浏览器重新向服务器端发生一次请求，请求地址为 location 指定的地址<br />4、location 可以为本服务器端的资源，也可以为外网可以访问的任意资源，比如：http://www.baidu.com<br />下面来详解 SpringMVC 中响应页面的 5 种方式。
<a name="ytKdw"></a>
## 方式 1：返回 ModelAndView
<a name="fukmA"></a>
### 需求
通过 SpringMVC 实现用户列表功能，如下图<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683904530106-b4026958-46be-410b-81ad-410ffa223bd7.png#averageHue=%23f8f7f7&clientId=u64fc36a1-15ed-4&from=paste&id=udd81f32f&originHeight=550&originWidth=656&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u91301718-e490-4735-a11c-f4d50ef85ef&title=)
<a name="dlbrF"></a>
### 如何实现？
先来看一下如果用 servlet 是如何实现的，代码如下：
```java
1、List<UserDto> userList = new ArrayList();
2、request.setAttribute("userList",userList);
3、request.getRequestDispatcher("/WEB-INF/view/user/list.jsp").forward(request,response);
```
关键代码就这几行，相当简单。<br />对应的 jsp（/WEB-INF/view/user/list.jsp）关键代码如下，一个循环遍历用户列表 userList
```html
<table border="1" cellpadding="10" cellspacing="0">
  <tr>
    <th width="50">id</th>
    <th width="100">name</th>
    <th width="50">age</th>
  </tr>
  <c:forEach items="${userList}" var="user">
    <tr align="center">
      <td>${user.id}</td>
      <td>${user.name}</td>
      <td>${user.age}</td>
    </tr>
  </c:forEach>
</table>
```
<a name="pyQng"></a>
### 使用 SpringMVC 实现
```java
@Controller
public class UserController {
    /**
     * 用户列表(用户id->用户信息)
     */
    Map<Long, UserDto> userDtoMap = new ConcurrentHashMap<>();

    {
        userDtoMap.put(1L, new UserDto(1L, "路人", 30));
        userDtoMap.put(2L, new UserDto(2L, "张三", 20));
        userDtoMap.put(3L, new UserDto(3L, "李四", 18));
    }

    /**
     * 用户列表
     *
     * @return
     */
    @RequestMapping("/user/list.do")
    public ModelAndView list() {
        //1.创建ModelAndView
        ModelAndView modelAndView = new ModelAndView();
        //2.将所有用户信息放到Model中
        modelAndView.addObject("userList", userDtoMap.values());
        //3.设置显示的页面
        modelAndView.setViewName("/WEB-INF/view/user/list.jsp");
        //4.返回ModelAndView
        return modelAndView;
    }

}
```
这里主要看 `list()`这个方法，当调用这个方法的时候，效果和上面 servlet 的效果一样，这里用到了`ModelAndView`。
<a name="JCgTj"></a>
### `ModelAndView`：模型&视图
通常页面都是动态的，客户端看到的页面，基本上都是模板（视图）+数据（数据模型），经过组装之后输出到客户端的。<br />所以响应客户端的请求，需要指定 2 个关键的信息：页面、页面中需要的数据。<br />SpringMVC 中使用 `ModelAndView` 来存放这 2 个信息，通过`modelAndView.addObject`方法添加页面中用到的数据，通过`modelAndView.setViewName("视图名称")`来设置显示的页面。
<a name="g9uuF"></a>
#### `modelAndView.addObject("key","value")`
添加页面中需要用到的数据，效果同：request.setAttribute("key","value");
<a name="ENgUf"></a>
#### `modelAndView.setViewName("视图名称")`
指定需要显示的视图命名，比如 jsp 地址
<a name="IQvl3"></a>
### 小结
如果页面中需要用到一些动态的数据，此时可以使用 ModelAndView 作为返回值，将动态数据放到 ModelAndView 中。
<a name="PKYcs"></a>
## 方式 2：直接返回视图名称
当页面不需要用到后端的数据的时候，就只是显示一个页面，此时可以直接将视图的名称作为返回值就可以了，比如
```java
/**
 * 跳转到新增页面
 *
 * @return
 */
@RequestMapping("/user/add.do")
public String add() {
    //直接返回视图的名称（页面的路径）
    return "/WEB-INF/view/user/add.jsp";
}
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683904530130-46961ebf-22f8-440a-bbc9-1daa6e433248.png#averageHue=%23f8f7f7&clientId=u64fc36a1-15ed-4&from=paste&id=uc3f6f5f3&originHeight=266&originWidth=565&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0e480f16-0bd9-4ddd-a206-62fdec6ab97&title=)
<a name="Ejwwn"></a>
## 方式 3：指定视图解析器
<a name="UkL1F"></a>
### 存在的问题
上面 2 种方式，返回的视图名称，都以/WEB-INF/view/开头，以.jsp结尾对不对。<br />如果项目中规定所有的视图都符合这种规则，即都放在/WEB-INF/view/目录中，都是 jsp 文件，那么可以将视图的名称是不是可以简化一下，怎么做的呢？<br />具体 2 个步骤。
<a name="jn3WM"></a>
### step1：注册视图解析器
在 SpringMVC 配置文件中添加下面配置，来指定视图解析器。<br />这个 bean 会对视图的名称进行处理，有 2 个参数需要指定

- prefix：视图文件前缀
- suffix：视图文件后缀

最终视图的名称 = prefix+controller 中指定的 viewname+suffix
```xml
<!-- 添加视图解析器 -->
<bean id="viewResolver" class="org.springframework.web.servlet.view.InternalResourceViewResolver">
  <property name="prefix" value="/WEB-INF/view/"/>
  <property name="suffix" value=".jsp"/>
</bean>
```
<a name="sKyO9"></a>
### step2：调整 controller 中视图的名称
| viewName 旧值 | viewName 新值 |
| --- | --- |
| /WEB-INF/view/user/add.jsp | user/add |
| /WEB-INF/view/user/list.jsp | user/list |

```java
@RequestMapping("/user/add.do")
public String add() {
    //直接返回视图的名称（页面的路径）
    return "user/add";
}
```
此时代码是不是简单多了。
<a name="ARloT"></a>
## SpringMVC 实现重定向 2 种方式
<a name="L0ZLR"></a>
### 需求
有时候，请求之后，需要做重定向操作，比如发送删除用户信息的请求/user/del/{用户id}.do，后端处理成功之后，需重定向到用户列表页面/user/list.do<br />这里就需要用到重定向的操作了，在 servlet 的中对应代码是
```java
response.sendRedirect(location);
```
SpringMVC 中有好几种实现，这里主要掌握 2 种。
<a name="akcuc"></a>
### 方式 1：返回 String 类型
SpringMVC 中实现重定向比较简单，视图的名称必须需要以`redirect:`开头，比如下面代码，处理删除用户的请求，删除成功之后，重定向到用户列表页面
```java
/**
 * 删除用户信息，删除成功之后重定向到用户列表页
 *
 * @param userId 用户id
 * @return
 */
@GetMapping("/user/del/{userId}.do")
public String del(@PathVariable("userId") Long userId, HttpServletRequest request) {
    //删除用户信息
    this.userDtoMap.remove(userId);
    //重定向到用户列表页面，此时浏览器地址会发生变化，变为http://localhost:8080/chat05/user/list.do
    return "redirect:/user/list.do";
}
```
<a name="dtYAg"></a>
### 方式 2：返回 `ModelAndView` 类型
如果重定向的时候，需要向重定向的页面携带参数，一般可以这么做，代码如下：
```java
return "redirect:/user/list.do?在这里拼参数";
```
比如
```java
return "redirect:/user/list.do?p1=v1&p2=v2";
```
如果遇到了这种请求，参数比较少的情况，按照上面拼接是可以的。<br />SpringMVC 中提供了更简单的方式，代码如下，最终 SpringMVC 会指定将 `ModelAndView` 中添加的数据，拼接到重定向的 url 中
```java
@GetMapping("/user/del1/{userId}.do")
public ModelAndView del1(@PathVariable("userId") Long userId) {
    //删除用户记录
    this.userDtoMap.remove(userId);

    /**
     * 重定向到用户列表页面，此时浏览器地址会发生变化，
     * 变为http://localhost:8080/chat05/user/list.do?p1=v1&p2=v2
     */
    ModelAndView modelAndView = new ModelAndView();
    modelAndView.addObject("p1", "v1");
    modelAndView.addObject("p2", "v2");
    modelAndView.setViewName("redirect:/user/list.do");
    return modelAndView;
}
```
<a name="AQka6"></a>
## 总结

- 响应页面通常有 2 种方式，第 1 种返回 `ModelAndView`，这种方式比较适合页面中需要后端传递数据的，第 2 种方式直接返回视图的名称，这种适合无需传递数据的。
- SpringMVC 容器中配置 `InternalResourceViewResolver` 视图解析器，用来简化 Controller 中视图的名称
- 掌握重新的 2 种方式，重定向的关键点是视图名称要以redirect:开头，这样 SpringMVC 才知道你需要 SpringMVC 来执行重定向操作。
