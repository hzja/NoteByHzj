JavaSpringMVC
<a name="EVgU8"></a>
## 1、跨域访问报错
当一个请求url的协议、域名、端口三者之间任意一个与当前页面url不同即为跨域。<br />出于安全原因，浏览器禁止Ajax调用驻留在当前原点之外的资源，比如从a.com发送一个ajax请求到b.com，则浏览器控制台会报跨域访问错误。<br />如下图，从http://localhost:63342/站点页面中向ttp://localhost:8080/chat21/cors/test2发送一个ajax请求，则出现了红色的错误信息，错误中包含了Access-Controll-Allow-Origin这样字样的错误，以后看到这个的时候，大家就要一眼看出来这是跨域问题。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684069983867-21e14704-8b82-4e81-8044-5c966c71c4b2.png#averageHue=%23f7f1c4&clientId=ua854d795-1d39-4&from=paste&id=u6db69db2&originHeight=226&originWidth=642&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5b7a0083-28ac-4084-9256-1f1e09cb82b&title=)
<a name="L0o8x"></a>
## 2、同源定义
同源策略是浏览器的一个重要的安全策略，它用于限制一个源的文档或其加载的脚本如何与另外一个源进行交互，它能够隔绝恶意文档，减少被攻击的媒介。<br />如果两个URL的**协议**、**主机名**和**端口号**都是相同的，那么这两个URL就是同源的，否则不同源，不同源的访问就会出现跨域问题，就会出现上面的错误。<br />下表给出了与 URL http://store.company.com/dir/page.html 的源进行对比的示例：

| URL | 结果 | 原因 |
| --- | --- | --- |
| http://store.company.com/dir2/other.html | 同源 | 只有路径不同 |
| https://store.company.com/secure.html | 非同源 | 协议不同 |
| http://store.company.com:81/dir/etc.html | 非同源 | 端口号不同 |
| http://news.company.com/dir/other.html | 非同源 | 主机名不同 |

也就是说当在http://store.company.com/dir/page.html这个网站中向https://store.company.com、http://store.company.com:81和http://news.company.com三个地址发起AXJX请求都会失败并且会报跨域的错误。这就是浏览器的同源策略，只能访问同源的数据。
<a name="glCI0"></a>
## 3、跨域问题如何解决？
跨域问题需要使用CORS来解决，请求端和后端接口需要遵循CORS规则来通信，便可解决跨域访问的问题。<br />CORS全称Cross-Origin Resource Sharing，即跨域资源共享，是一个由一系列HTTP头组成的系统，这些HTTP头决定浏览器是否阻止前端JavaScript代码获取跨域请求的响应。为什么需要CORS ？这是因为浏览器存在同源安全策略，在当前域请求另外一个域的资源时，浏览器默认会阻止脚本读取它的响应，这时CORS就有了用武之地。<br />跨源资源共享（CORS）是由大多数浏览器实现的W3C规范，允许开发者灵活地指定什么样的跨域请求被授权，而不是使用一些不太安全和不太强大的策略，如IFRAME或JSONP等。
<a name="bceF2"></a>
## 4、CORS原理
CORS的原理：简单点说，就是在请求头或响应头中添加了一些配置，通过这些配置来便可轻松解决跨域问题。
<a name="UuoRH"></a>
## 5、SpringMVC中如何解决跨域问题？
SpringMVC内部提供了跨域问题的解决方案，只需要做一些简单的配置，而接口基本上不用做任何修改，便可解决跨域问题。<br />SpringMVC解决跨域问题的原理也就是SpringMVC遵循了CORS通信的规则来解决了跨域的问题，在响应头中添加了一些CORS需要的信息。<br />SpringMVC中提供了3种方案来解决跨域问题，下面一起来了解下。
<a name="uHTpd"></a>
## 6、方案1：方法或者类上标注`@CrossOrigin`注解

- 接口方法上标注`org.springframework.web.bind.annotation.CrossOrigin`注解，如下test1接口上标注了`@CrossOrigin`注解，这个接口就支持跨域访问，`@CrossOrigin`注解中含有更详细的配置，这里就不细说了
- 也可以在类上标注`@CrossOrigin`注解，那么这个类中所有接口会支持跨域访问
- 也可同时在类和方法上标注`@CrossOrigin`注解，最后方法上的跨域访问会取合并后的配置
```java
@RestController
public class CorsController {

    @RequestMapping("/cors/test1")
    @CrossOrigin
    public List<String> test1() {
        List<String> result = Arrays.asList("www.it.com",
                "Spring",
                "SpringMVC",
                "MySQL",
                "高并发");
        return result;
    }

}
```
<a name="XBRbC"></a>
## 7、方案2：全局配置的方式
除了细粒度、基于注释的配置之外，还可能需要定义一些全局CORS配置，这类似于使用筛选器，但可以声明为Spring MVC并结合细粒度`@CrossOrigin`配置。默认情况下，所有origins and GET，HEAD and POST methods是允许的。
```java
@EnableWebMvc
@Configuration
public class MvcConfig implements WebMvcConfigurer {

    @Override
    public void addCorsMappings(CorsRegistry registry) {

        //每次调用registry.addMappin可以添加一个跨域配置，需要多个配置可以多次调用registry.addMapping
        registry.addMapping("/**")
                .allowedOrigins("*") //放行哪些原始域
                .allowedMethods("PUT", "DELETE"，"POST", "GET") //放行哪些请求方式
                .allowedHeaders("header1", "header2", "header3") //放行哪些原始请求头部信息
                .exposedHeaders("header1", "header2") //暴露哪些头部信息
                .allowCredentials(false) //是否发送 Cookie
                .maxAge(3600);

        // Add more mappings...
    }
}
```
<a name="hXhZ9"></a>
## 8、方案3：拦截器的方式`CorsFilter`
```java
//处理跨域的Filter
//1. 添加 CORS配置信息
CorsConfiguration config = new CorsConfiguration();
//放行哪些原始域
config.addAllowedOrigin("*");
//是否发送 Cookie
config.setAllowCredentials(false);
//放行哪些请求方式
config.addAllowedMethod("*");
//放行哪些原始请求头部信息
config.addAllowedHeader("*");
//暴露哪些头部信息
config.addExposedHeader("*");
//2. 添加映射路径
UrlBasedCorsConfigurationSource corsConfigurationSource = new UrlBasedCorsConfigurationSource();
corsConfigurationSource.registerCorsConfiguration("/**",config);
```
