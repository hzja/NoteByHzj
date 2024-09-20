JavaSpringMVC
<a name="PUylZ"></a>
## 1、来看一个需求
如下，有一个 UserDto 类，表示用户信息
```java
public class UserDto {
    //用户名
    private String name;
    //年龄
    private Integer age;

    //省略getter、setter方法
}
```
要求后台所有接口接受`UserDto`数据时，参数的值格式为：name,age，比如下面接口
```java
@RequestMapping("/convert/test1")
public UserDto test1(@RequestParam("user") UserDto user) {
    System.out.println("name：" + user.getName());
    System.out.println("age：" + user.getAge());
    return user;
}
```
能够接受的请格式：/convert/test1?user=ready,1，这种需求如何实现呢？
<a name="Wp44r"></a>
## 2、需用 Converter 接口来实现
SpringMVC 中提供的另外一个接口`org.springframework.core.convert.converter.Converter`，这个接口用来将一种类型转换为另一种类型，看看其源码如下，调用后端接口的时候，http 传递的参数都是字符串类型的，但是后端却可以使用 Integer、Double 等其他类型来接收，这就是`Converter`实现的。
```java
@FunctionalInterface
public interface Converter<S, T> {

    /**
     * 将source转换为目标T类型
     */
    @Nullable
    T convert(S source);
}
```
Spring 内部提供了很多默认的实现，用于各种类型转换<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684054634583-8a6b0478-0636-4b52-9abe-929df6e758f8.png#averageHue=%23fefee1&clientId=u969a5ac9-cf0f-4&from=paste&id=u2b768953&originHeight=661&originWidth=722&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u6bacee7a-1f3e-4dba-a7ad-f11e0797303&title=)<br />实现开头的需求，需要自定义一个 `Converter<String,UserDto>`，将其添加到 SpringMVC 转换器列表中，他负责将 String 类型转换为 UserDto 类型。下面来看具体代码实现
<a name="QPn5f"></a>
## 3、代码实现

- 代码如下，添加一个配置类，实现`WebMvcConfigurer`接口
- 重写`addFormatters`方法，在这个方法中添加一个自定义的`Converter`，实现其 `convert` 方法，将name,age格式的字符串转换为 UserDto 对象返回
```java
@Configuration
public class MvcConfig implements WebMvcConfigurer {

    @Override
    public void addFormatters(FormatterRegistry registry) {
        registry.addConverter(new Converter<String, UserDto>() {
            @Override
            public UserDto convert(String source) {
                if (source == null) {
                    return null;
                }
                String[] split = source.split(",");
                String name = split[0];
                Integer age = Integer.valueOf(split[1]);
                return new UserDto(name, age);
            }
        });
    }
}
```
接口代码
```java
@RestController
public class ConverterTestController {

    @RequestMapping("/convert/test1")
    public UserDto test1(@RequestParam("user") UserDto user) {
        System.out.println("name：" + user.getName());
        System.out.println("age：" + user.getAge());
        return user;
    }

}
```
访问请求/convert/test1?user=ready,1，效果如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684054634518-a7d28977-17e3-4d18-97ad-0faa5c072e63.png#averageHue=%23fdfdfc&clientId=u969a5ac9-cf0f-4&from=paste&id=ud5fdca08&originHeight=123&originWidth=501&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9786c86f-b7b8-4eaf-984c-978c94be2e2&title=)<br />Tomcat 控制台输出
```
name：ready
age：1
```
