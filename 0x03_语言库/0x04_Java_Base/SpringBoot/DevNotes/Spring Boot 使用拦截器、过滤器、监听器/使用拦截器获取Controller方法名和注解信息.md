Java Spring 拦截器 <br />在使用SpringMVC进行项目的时候用到了权限验证。<br />表分为:

- 用户表；
- 角色表；
- 资源表。

用户-角色-资源都是多对多的关系，验证无非就是收到请求后，在拦截器循环判断用户是否有权限执行操作。
<a name="ljZ2q"></a>
### 方法一：通过request获得用户的URI，再逐一循环判断是否可以操作。只是这种方法很让人难受。
<a name="o1OQE"></a>
### 方法二：通过用户要访问的方法来判断是否有权限：
`preHandle`方法中`handler`实际为`HandlerMethod`，（有时候不是HandlerMethod），加个`instanceof`验证<br />可以得到方法名：`h.getMethod().getName()`<br />可以得到`RequestMapping`注解中的值：`h.getMethodAnnotation(RequestMapping.class`)<br />这种方法还是不太方便
<a name="HP7YI"></a>
### 方法三：自定义注解
自定义注解代码：
```java
@Retention(RUNTIME)
@Target(METHOD)
public @interface MyOperation {
    String value() default "";//默认为空，因为名字是value，实际操作中可以不写"value="
}
```
Controller代码：
```java
@Controller("testController")
public class TestController {
    @MyOperation("用户修改")//主要看这里
    @RequestMapping("test")
    @ResponseBody
    public String test(String id) {
        return "Hello,2018!"+id;
    }
}
```
拦截器的代码：
```java
@Override
public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler)
        throws Exception {
    System.out.println("进入拦截器");
    if(handler instanceof HandlerMethod) {
        HandlerMethod h = (HandlerMethod)handler;
        System.out.println("用户想执行的操作是:"+h.getMethodAnnotation(MyOperation.class).value());
        //判断后执行操作...
    }
    return HandlerInterceptor.super.preHandle(request, response, handler);
}
```
在每个方法上面加注解太麻烦啦,可以在类上加注解
```java
@Retention(RUNTIME)
@Target(TYPE)
public @interface MyOperation {
    String value() default "";
}

//拦截器中这样获得
h.getMethod().getDeclaringClass().getAnnotation(MyOperation.class);
```
可以获取`requestMapping`，不用创建自定义注解，值得注意的是，不要使用`GetMapping`等，要使用`requestMapping`。
