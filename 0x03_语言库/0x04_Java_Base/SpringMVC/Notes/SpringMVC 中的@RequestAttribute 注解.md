JavaSpringMVC
<a name="lqmaJ"></a>
## 1、`@RequestAttribute` 注解
<a name="fzFLj"></a>
### 1.1、作用
用来标注在接口的参数上，参数的值来源于 request 作用域。
<a name="xCgGG"></a>
### 1.2、用法
如下代码，site 参数上使用了`@RequestAttribute("site")`注解，site 参数的值等于`request.getAttribute("site")`
```java
@ResponseBody
public String test2(@RequestAttribute("site") String site) {
    return site;
}
```
这个注解的源码如下
```java
@Target(ElementType.PARAMETER)
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface RequestAttribute {

    /**
     * 指定request作用域中属性的名称
     */
    @AliasFor("name")
    String value() default "";

    /**
     * 同value属性
     */
    @AliasFor("value")
    String name() default "";

    /**
     * 属性是不是必须的，如果是true，request中没有取到时，则会抛出异常
     * 此时可以将required设置为false，或者使用java8中的Option类型来修饰参数解决
     */
    boolean required() default true;

}
```
<a name="nOzbf"></a>
## 2、案例
下面代码中有 2 个接口方法

- 第一个方法 test1 中向 request 域中丢了一个 site 属性，然后进行了跳转，跳转到第二个方法，最后将 site 作为响应体输出
- 第二个方的 site 参数上标注了`@RequestAttribute("site")`，所以会拿到 request 中 site 的值，然后输出
```java
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestAttribute;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

import javax.servlet.http.HttpServletRequest;

@Controller
public class RequestAttributeController {

    @RequestMapping("/requestattribute/test1")
    public String test1(HttpServletRequest request) {
        request.setAttribute("site",
                             "<a href='http://www.baidu.com'>百度一下</a>");
        return "forward:/requestattribute/test2";
    }

    @RequestMapping(value = "/requestattribute/test2", produces = "text/html;charset=UTF-8")
    @ResponseBody
    public String test2(@RequestAttribute("site") String site) {
        return site;
    }
}
```
浏览器中访问第一个接口/requestattribute/test1，输出百度一下的跳转链接。<br />若调整一下接口 1 中代码，将 site 的值置为空
```
request.setAttribute("site", null);
```
此时再次访问接口会报 400 错误，原因：request 域中没有找到 site 这个属性对应的值，即 `request.getAttribute("site")`为 null。<br />**2 种解决方案**

- 方案 1：将`@RequestAttribute` 的 required 属性设置为 false，常用这种方式
- 方案 2：将`@RequestAttribute` 标注的参数类型调整为 java8 中的 java.util.Optional 类型，上面的接口 2 的 site 参数类型可以调整为`Optional<String>`类型，即可解决问题
<a name="oOR5T"></a>
## 3、`@RequestAttribute` 注解原理
`@RequestAttribute` 注解标注的参数的值来源于`org.springframework.web.servlet.mvc.method.annotation.RequestAttributeMethodArgumentResolver`解析器，源码
```java
public class RequestAttributeMethodArgumentResolver extends AbstractNamedValueMethodArgumentResolver {

    @Override
    public boolean supportsParameter(MethodParameter parameter) {
        return parameter.hasParameterAnnotation(RequestAttribute.class);
    }

    @Override
    protected NamedValueInfo createNamedValueInfo(MethodParameter parameter) {
        RequestAttribute ann = parameter.getParameterAnnotation(RequestAttribute.class);
        Assert.state(ann != null, "No RequestAttribute annotation");
        return new NamedValueInfo(ann.name(), ann.required(), ValueConstants.DEFAULT_NONE);
    }

    @Override
    @Nullable
    protected Object resolveName(String name, MethodParameter parameter, NativeWebRequest request){
        return request.getAttribute(name, RequestAttributes.SCOPE_REQUEST);
    }

    @Override
    protected void handleMissingValue(String name, MethodParameter parameter) throws ServletException {
        throw new ServletRequestBindingException("Missing request attribute '" + name +
                                                 "' of type " +  parameter.getNestedParameterType().getSimpleName());
    }

}
```
