Java SpringBoot 全局异常处理
<a name="z2XH3"></a>
## 问题
当项目上线时，用户访问某个url执行到某个controller方法的时候突然遇到异常，这时页面就会出现错误提示，这样用户体验不好。<br />例如：
```java
@Controller
public class UserControllerr {
    @ResponseBody
    @RequestMapping("/java")
    public String Test() {
        int a = 1 / 0; //代码执行到这里会抛出除0异常
        return "asd";
    }
}
```
当用户执行http://localhost:8080/java 出现错误截图：<br />![2021-08-27-17-14-44-506448.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630055694983-f049d894-8167-4bf0-9ebb-212086503c37.png#averageHue=%23f3f2f2&clientId=ub0eba76a-a0d9-4&from=ui&id=u189e46c7&originHeight=414&originWidth=1074&originalType=binary&ratio=1&rotation=0&showTitle=false&size=56396&status=done&style=shadow&taskId=ud80a3c97-3dab-4d4c-a4ad-5923e05c2ea&title=)
<a name="o0gVp"></a>
## 思考
所以当出现异常的时候，能不能对异常进行统一处理，规定返回的json格式或是跳转到一个错误页面？
<a name="k2FpK"></a>
## 操作步骤
1、导入坐标<br />2、创建一个异常处理类，类上方加上`@ControllerAdvice`注解 <br />3、在定义的异常类里面创建一个方法，当出现异常之后的一些操作逻辑 <br />4、在创建的方法上加上 `@ExceptionHandler()`<br />5、在 `@ExceptionHandler()`里面写出要处理的异常类型，表示当出现什么样的异常，就执行该方法。<br />6、`@ExceptionHandler(Exception.class)`，表示出现所有的异常都执行该方法，所以这个就是局异常处理的方式<br />`@ControllerAdvice`，是Spring3.2提供的新注解，它是一个Controller增强器，可对controller中被 `@RequestMapping`注解的方法加一些逻辑处理。最常用的就是异常处理
<a name="gmtgH"></a>
## 操作代码如下
导入坐标
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-hateoas</artifactId>
</dependency>
```
代码实现：
```java
@ControllerAdvice
public class GlobalExceptionHandler {
    @ExceptionHandler(Exception.class)
    @ResponseBody
    public String excetionHandlerTest(){
        return "系统正在维护中！";
    }
}
```
项目运行截图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1630055521526-7c804056-2b5e-4855-8679-14f6ecb25e45.webp#averageHue=%23f9f9f9&clientId=ub0eba76a-a0d9-4&from=paste&id=uafeb0551&originHeight=278&originWidth=716&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc724bc04-5046-45fb-9c31-41466ce8674&title=)
<a name="McO1b"></a>
## 总结
1、`@ExceptionHandler(Exception.class)`，`Exception.class`，表示出现所有的异常都执行该方法。<br />2、如果想具体的异常之行具体的逻辑，可以自定义异常，然后在`@ExceptionHandler("异常类")`括号中写入要处理的异常即可。
