JavaSpringMVC<br />当在接口中想获取 cookie 值的时候，怎么写代码更简单呢？<br />此时可以使用 SpringMVC 中的`@CookieValue` 注解来标注参数，下面来看具体的用法。
<a name="OR6H7"></a>
## 1、`@CookieValue`

- 该注释指示应将方法参数绑定到 HTTP cookie。
- 方法参数可以声明为`javax.servlet.http.Cookie`类型，也可以声明为 Cookie 值类型（String、int 等）。
```java
@Target(ElementType.PARAMETER)
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface CookieValue {

    /**
     * cookie名称
     */
    @AliasFor("name")
    String value() default "";

    /**
     * 同value属性
     */
    @AliasFor("value")
    String name() default "";

    /**
     * 是否需要cookie。
     * 默认值为true，如果请求中缺少cookie，则会引发异常。
     * 如果请求中不存在cookie，则希望使用空值，请将此选项切换为false。
     * 或者，提供一个默认值defaultValue，它隐式地将此标志设置为false。
     */
    boolean required() default true;

    /**
     * 默认值
     */
    String defaultValue() default ValueConstants.DEFAULT_NONE;

}
```
<a name="TqkoW"></a>
## 2、2 种用法

- 用法 1：参数类型为非`javax.servlet.http.Cookie`类型，比如（String、int 等类型）
- 用法 2：参数类型为`javax.servlet.http.Cookie`类型
<a name="YgpUJ"></a>
## 3、案例代码
```java
import org.springframework.web.bind.annotation.CookieValue;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import javax.servlet.http.Cookie;
import java.util.LinkedHashMap;
import java.util.Map;

@RestController
public class CookieValueController {

    @RequestMapping("/cookievalue/test1")
    public Map<String, Object> test1(@CookieValue("name") String name,
                                     @CookieValue("age") int age) {
        Map<String, Object> result = new LinkedHashMap<>();
        result.put("name", name);
        result.put("age", age);
        return result;
    }

    /**
     * @param nameCookie
     * @param ageCookie
     * @return
     */
    @RequestMapping("/cookievalue/test2")
    public Map<String, Object> test2(@CookieValue("name") Cookie nameCookie,
                                     @CookieValue("age") Cookie ageCookie) {
        Map<String, Object> result = new LinkedHashMap<>();
        result.put("nameCookie", nameCookie);
        result.put("ageCookie", ageCookie);
        return result;
    }

}
```
测试用例代码
```http
###
POST http://localhost:8080/chat18/cookievalue/test1
Cookie: name=java; age=26


###
POST http://localhost:8080/chat18/cookievalue/test2
Cookie: name=java; age=26
```
运行 2 个用例<br />用例 1 输出
```json
{
  "name": "java",
  "age": 26
}
```
用例 2 输出
```json
{
  "nameCookie": {
    "name": "name",
    "value": "java",
    "version": 0,
    "comment": null,
    "domain": null,
    "maxAge": -1,
    "path": null,
    "secure": false,
    "httpOnly": false
  },
  "ageCookie": {
    "name": "age",
    "value": "26",
    "version": 0,
    "comment": null,
    "domain": null,
    "maxAge": -1,
    "path": null,
    "secure": false,
    "httpOnly": false
  }
}
```
<a name="hVGjV"></a>
## 4、`@CookieValue` 原理
`@CookieValue`标注的参数的值来源于`org.springframework.web.servlet.mvc.method.annotation.ServletCookieValueMethodArgumentResolver`解析器<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684052990822-76c281b0-02bd-4f3a-b108-66eb10d7ad1b.png#averageHue=%23faf9f7&clientId=u7ec69beb-a246-4&from=paste&id=ub43b8718&originHeight=625&originWidth=1076&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue2ba470e-2902-4a62-a0f2-be214452ce2&title=)
