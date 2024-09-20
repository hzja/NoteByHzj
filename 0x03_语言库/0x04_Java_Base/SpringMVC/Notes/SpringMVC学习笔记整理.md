Java SpringMVC
<a name="kuJbP"></a>
## 思维导图
![SpringMVC笔记.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1565020372778-9b346d1f-852f-495f-a9a5-afa31e1050fe.png#averageHue=%23f1f1f7&height=4980&id=GwE3p&originHeight=4980&originWidth=3738&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1833536&status=done&style=none&title=&width=3738)

附件原件<br />[SpringMVC笔记.png](https://www.yuque.com/attachments/yuque/0/2019/png/396745/1565020382898-f9586a16-cbb2-4ee6-9a22-058bd92ebc66.png?_lake_card=%7B%22src%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2019%2Fpng%2F396745%2F1565020382898-f9586a16-cbb2-4ee6-9a22-058bd92ebc66.png%22%2C%22name%22%3A%22SpringMVC%E7%AC%94%E8%AE%B0.png%22%2C%22size%22%3A1833536%2C%22ext%22%3A%22png%22%2C%22source%22%3A%22%22%2C%22status%22%3A%22done%22%2C%22download%22%3Atrue%2C%22uid%22%3A%22rc-upload-1565020356351-12%22%2C%22type%22%3A%22image%2Fpng%22%2C%22progress%22%3A%7B%22percent%22%3A0%7D%2C%22percent%22%3A0%2C%22mode%22%3A%22title%22%2C%22id%22%3A%22VOw3a%22%2C%22card%22%3A%22file%22%7D)
<a name="P4EKh"></a>
## 1、`@RequestMapping`处理映射请求

1. SpringMvc使用`@RequestMapping`注解为控制器指定可以处理的URL请求，注解可以标记在类上或方法上
2. Dispatcher截获请求后就通过`@RequestMapping`提供的映射信息确定请求所对应的处理方法
3. `@RequestMapping`中可以指定value（请求URL）、method（请求的方法）、params（请求参数）、heads（请求头），他们之间时       与的关系，使用多个条件联合查询，可使请求精确化。
4. 支持Ant风格的Url，Ant风格地址支持三种匹配符：
- ？：匹配文件名中的一个字符
- *：匹配文件名中的任意字符
- **：**匹配多层路径
5. `@PathVariable`映射URL绑定的占位符

通过`@PathVariable`可以将URL中占位符参数绑定到控制器处理方法的入参中<br />请求的URL为：`<a href="helloworld/1">To success</a>`  ，可将“1”传入到方法中
```java
@RequestMapping("/helloworld/{id}")
public String hello(@PathVariable("id") Integer id){
    System.out.println("helloWorld"+id);
    return "success";	
}
```
<a name="lj7vJ"></a>
## 2、映射请求参数、请求头
SpirngMVC通过分析处理方法的签名，将Http请求信息绑定到处理方法的相关入参中
<a name="47Puh"></a>
### 使用`@RequestParam`可以把请求参数传递给请求方法    
value：参数名、required：是否必须<br />请求Url：`<a href="helloworld?username=123">To success</a>`，可将请求参数中的“123”传递给方法。
```java
@RequestMapping("/helloworld")
public String hello(@RequestParam(value="username" required=false) String un){
    System.out.println("helloWorld:"+un);
    return "success";
}
```
<a name="Dn7lk"></a>
### 使用`@RequestHeader`绑定请求报头信息
请求头包含了若干属性，服务器可据此获知客户端的信息。
```java
@RequestMapping("/helloworld")
public String hello(@RequestHeader("Accept-Encoding") String encoding){
    System.out.println("helloWorld:"+encoding);		
    return "success";
}
```
<a name="zhHxf"></a>
### 使用`@CookieValue`绑定请求中的Cookie值
```java
@RequestMapping("/helloworld")
public String hello(@CookieValue("JSESSIONID") String sessionId){
    System.out.println("helloWorld:"+sessionId);		
    return "success";
}
```
<a name="S2heX"></a>
### 使用POJO对象绑定请求参数
SpirngMVC会按照请求参数名和POJO属性名自动进行匹配，自动为该对象填充属性，支持级联属性
<a name="GNAbe"></a>
### MVC的Handler可以接收ServletAPI
`HttpServletRequest`、`HttpServletResponse`、`HttpSession`、`java.security.Principal`、`Locale`、`InputStream`、`OutputStream`、`Reader`、`Writer`
<a name="Nvh5n"></a>
## 3、处理模型数据
<a name="ZT2PE"></a>
### ModelAndView      
控制器处理方法的返回值可以是ModelAndView，其包含模型和数据信息，SpringMVC会将model中的数据放到request域中。<br />添加模型数据：
```java
ModelAndView addObject（String attributeName，Object attributeValue）
ModelAndView addAllObject（Map<String,?> modelMap） 
```
设置视图：
```java
void setView(View view)
void setViewName(String ViewName)  
```
<a name="5Ez9Q"></a>
### Map及Model
也可以传入Map和Model类型，目标方法可以添加Map类型（也可以是Model类型或是ModelMap类型参数）
<a name="6yxTZ"></a>
### `@SessionAttributes` 注意：此注解只能放在类上
若希望在多个请求之间共用某个模型参数，则可以在控制器类上标注一个`@SessionAttributes`，SpringMVC将在模型中对应的属性暂存到HttpSession中。<br />        value属性：通过属性名指定需要放到会话中的属性<br />        types属性：通过模型属性的对象类型指定哪些模型属性需要放到会话中
```java
@SessionAttributes(value= {"user"},types= {String.class})
@Controller
public class HelloWorld {
	 
@RequestMapping("/testSessionAttributes")
    public String testSessionAttributes(Map<String, Object> map) {
        User user = new User("Li", "123456", 24);
        map.put("user", user);
        return "success";
    }
}
```
可在目标页面的session域中得到user
```java
user:<%=session.getAttribute("user") %>
```
<a name="6OjGX"></a>
### `@ModelAttribute` 、`@ModelAttribute` 标记的方法, 会在每个目标方法执行之前被 SpringMVC 调用
`ModelAttribute`的典型应用：模拟修改操作   <br />运行流程：

1. 执行 `@ModelAttribute`注解修饰的方法：从数据库中取出对象，把对象放入到Map中，键为user
2. SpringMVC从Map中取出User对象，并把表单的请求参数赋给User对象的对应属性值。
3. SpringMVC把上述对象传入目标方法的参数。
<a name="ToqBU"></a>
## 5、视图及视图处理器
<a name="hAMoI"></a>
### SpringMVC解析视图
对于那些返回String、view或ModelMap等类型的处理方法，SpringMvc都会将其装配成一个ModelAndView对象。view接口时无状态的，不会有线程安全问题
<a name="O0Ijv"></a>
### 重定向     
如果返回的字符串中带有“`forward:`”或`“redirect`:”前缀时，SpringMVC会对他们进行特殊处理，将其当成指示符，其后的字符串作为URL来处理。
<a name="3aBvo"></a>
## 6、数据的转换、格式化、校验
<a name="WF6Zq"></a>
### 数据转换 
SpringMVC定义了3种类型的转换器接口：<br />`Converter<S,T>`：将S类型转换为T类型对象<br />`ConverterFactory`：将相同系列多个“同质”Conterver封装在一起<br />`GenericConverter`：会根据源类对象及目标所在的宿主主类中的上下文信息进行类型转换  
<a name="sd8tS"></a>
### 数据格式化

1. 在配置文件中添加上`<mvc:annotation-driven ></mvc:annotation-driven>`
2. 在目标类型的属性上配置
```java
@NumberFormat(pattern="#.###")
private double price;
	
@DateTimeFormat(pattern="yyyy-mm-dd")
private Date birth;
```
<a name="WByjj"></a>
### 数据的校验

1. 使用 JSR 303 验证标准，在 SpringMVC 配置文件中添加 `<mvc:annotation-driven />，<mvc:annotation-driven />`会默认装配好一个`LocalValidatorFactoryBean`。
2. 需要在 bean 的属性上添加对应的注解

例如：`@NotNull` ：被注释的元素不能为空`@Past`  ：备注是的元素必须是一个过去的日期    <br /> `@Email`  ：被注释的元素必须是一个email

3. 在目标方法 bean 类型的前面添加 `@Valid` 注解
<a name="U1OFP"></a>
## 7、文件上传
SpringMvc通过`MutipartResolver`实现文件上传

1. 配置`MutipartResolver`
```xml
<!-- 配置 MultipartResolver -->
<bean id="multipartResolver"
      class="org.springframework.web.multipart.commons.CommonsMultipartResolver">
    <property name="defaultEncoding" value="UTF-8"></property>
    <property name="maxUploadSize" value="1024000"></property>	
</bean>
```
<a name="yBSZ3"></a>
## 8、使用拦截器
SpringMvc也可以使用拦截器对请求进行拦截，可以自定义拦截器来实现特定的功能，自定义拦截器必须实现`HandlerInterceptor`接口
<a name="zk1vI"></a>
### 实现`HandlerInterceptor`接口的拦截器类 
```java
public class FirstInterceptor implements HandlerInterceptor {
 
	/**
	 * 渲染视图之后被调用. 释放资源
	 */
	@Override
	public void afterCompletion(HttpServletRequest arg0, HttpServletResponse arg1, Object arg2, Exception arg3)
			throws Exception {
		System.out.println("afterCompletion");
	}
 
	/**
	 * 调用目标方法之后, 但渲染视图之前. 
	 * 可以对请求域中的属性或视图做出修改. 
	 */
	@Override
	public void postHandle(HttpServletRequest arg0, HttpServletResponse arg1, Object arg2, ModelAndView arg3)
			throws Exception {
		System.out.println("postHandle");
	}
 
	/**
	 * 该方法在目标方法之前被调用.
	 * 若返回值为 true, 则继续调用后续的拦截器和目标方法. 
	 * 若返回值为 false, 则不会再调用后续的拦截器和目标方法. 
	 * 
	 * 可以考虑做权限. 日志, 事务等. 
	 */
	@Override
	public boolean preHandle(HttpServletRequest arg0, HttpServletResponse arg1, Object arg2) throws Exception {
		System.out.println("preHandle");
		return true;
	}
}
```
<a name="OdSnq"></a>
### 配置拦截器
```xml
<!-- 配置拦截器 -->
<mvc:interceptors>
    <bean class="springmvc.helloworld.FirstInterceptor"></bean>
</mvc:interceptors>
```

