Java SpringMVC<br />与原生的`HttpServletRequest`和`HttpServletResponse`打交道。从`HttpServletRequest`中读取body数据封装成某种数据结构；向`HttpServletResponse`写入数据并响应。传统的写法非常不优雅，这里介绍一种比较优雅的方式。
<a name="SiMoj"></a>
## `HttpMessageConverter`
`HttpMessageConverter`是Spring框架提供的一个消息转换器模型，用于在 HTTP 请求和响应之间进行转换的策略接口。它可以对输入消息`HttpInputMessage`进行读；也可以对输出消息`HttpOutputMessage`进行写。<br />![HttpMessageConverter](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640910265706-dd30e477-0fba-4692-a602-cadc1f3d7957.webp#averageHue=%23f4f3a1&clientId=u84783084-0642-4&from=paste&id=u14a504bc&originHeight=456&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ud3214c66-f85e-477b-843d-4cc533dbf02&title=HttpMessageConverter "HttpMessageConverter")<br />**Spring MVC**的消息转换都是通过这个接口的实现来完成的。`HttpMessageConverter`有很多实现：<br />![HttpMessageConverter常见实现](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640910265594-2fa70900-58fc-4a89-8c17-d78b4604a83a.webp#averageHue=%23f3f3c0&clientId=u84783084-0642-4&from=paste&id=ubb6a1c94&originHeight=633&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u6f166bea-1c3b-4318-8558-456fae417a4&title=HttpMessageConverter%E5%B8%B8%E8%A7%81%E5%AE%9E%E7%8E%B0 "HttpMessageConverter常见实现")<br />通常**Spring MVC**中处理**Form**表单提交、**JSON**、**XML**、字符串、甚至**Protobuf**都由`HttpMessageConverter`的实现来完成，前端传递到后端的**body**参数，后端返回给前端的数据都是由这个接口完成转换的。在**Spring IoC**中(**Spring MVC**环境)还存在一个存放`HttpMessageConverter`的容器`HttpMessageConverters`:
```java
@Bean
@ConditionalOnMissingBean
public HttpMessageConverters messageConverters(ObjectProvider<HttpMessageConverter<?>> converters) {
  return new HttpMessageConverters((Collection)converters.orderedStream().collect(Collectors.toList()));
}
```
可以直接拿来使用。那么到底怎么使用呢？那首先要搞清楚`HttpInputMessage` 和`HttpOutputMessage`是干什么用的。
<a name="GswNZ"></a>
## `HttpInputMessage`
`HttpInputMessage`表示一个 **HTTP** 输入消息，由请求头**headers**和一个可读的请求体**body**组成，通常由服务器端的 **HTTP** 请求句柄或客户端的 **HTTP** 响应句柄实现。<br />![HttpInputMessage](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640910265737-c6a59da6-a75f-4063-b25e-56015221b9ef.webp#averageHue=%23577337&clientId=u84783084-0642-4&from=paste&id=u92dee44d&originHeight=192&originWidth=978&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=ub21c7b47-de71-4dec-b0a3-88b2cbd7e84&title=HttpInputMessage "HttpInputMessage")<br />而`HttpServletRequest`是`ServletRequest`的扩展接口，提供了**HTTP Servlet**的请求信息，也包含了请求头和请求体，所以两者是有联系的。只要找出两者之间的实际关系就能让`HttpMessageConverter`去读取并处理`HttpServletRequest`携带的请求信息。
<a name="Wzcvp"></a>
## `ServletServerHttpRequest`
![ServletServerHttpRequest](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640910265641-a04d4c4c-89aa-4561-898e-481e93000480.webp#averageHue=%23475b37&clientId=u84783084-0642-4&from=paste&id=u281ddd15&originHeight=640&originWidth=1032&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=uaf149cc8-5c40-4cee-a81d-d1668b41f08&title=ServletServerHttpRequest "ServletServerHttpRequest")<br />`ServletServerHttpRequest`不仅仅是`HttpInputMessage`的实现，它还持有了一个`HttpServletRequest`实例属性，`ServletServerHttpRequest`的所有操作都是基于`HttpServletRequest`进行的。可以通过构造为其注入`HttpServletRequest`实例，这样`HttpMessageConverter`就能间接处理`HttpServletRequest`了。
<a name="qoxWY"></a>
### 提取请求体实战
这里聚焦的场景是在Servlet过滤器中使用`HttpMessageConverter`，在Spring MVC中不太建议去操作`HttpServletRequest`。这里选择了`FormHttpMessageConverter`，它通常用来处理`application/x-www-form-urlencoded`请求。编写一个过滤器来拦截请求提取**body**：
```java
/**
 * 处理 application/x-www-form-urlencoded 请求
 */

@Component
public class FormUrlencodedFilter implements Filter {
    private final FormHttpMessageConverter formHttpMessageConverter = new FormHttpMessageConverter();
    private static final Logger log = LoggerFactory.getLogger(FormUrlencodedFilter.class);

    @Override
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain) throws IOException {
        String contentType = request.getContentType();
        MediaType type= StringUtils.hasText(contentType)? MediaType.valueOf(contentType):null;
        ServletServerHttpRequest serverHttpRequest = new ServletServerHttpRequest((HttpServletRequest) request);
        
        if (formHttpMessageConverter.canRead(MultiValueMap.class,type)) {
            MultiValueMap<String, String> read = formHttpMessageConverter.read(null, serverHttpRequest);
             log.info("打印读取到的请求体：{}",read);
        }
    }
}
```
然后执行一个`POST`类型，`Content-Type`为`application/x-www-form-urlencoded`的请求：
```http
POST /ind HTTP/1.1
Host: localhost:8080
Content-Type: application/x-www-form-urlencoded
Content-Length: 20

a=b123&c=d123&e=f123
```
控制台打印：
```
打印读取到的请求体：{a=[b123], c=[d123], e=[f123]}
```
<a name="d5IMx"></a>
## `ServletServerHttpResponse`
有`ServletServerHttpRequest`就有`ServletServerHttpResponse`，大致原理差不多。它正好和`ServletServerHttpRequest`相反，如果需要去处理响应问题，比如想通过`HttpServletResponse`写个JSON响应，大概可以这么写：
```java
ServletServerHttpResponse servletServerHttpResponse = new ServletServerHttpResponse(response);
// 使用json converter
MappingJackson2HttpMessageConverter mappingJackson2HttpMessageConverter = new MappingJackson2HttpMessageConverter();
//  authentication 指的是需要写的对象实例
mappingJackson2HttpMessageConverter.write(authentication, MediaType.APPLICATION_JSON,servletServerHttpResponse);
```
<a name="bRnEq"></a>
## 总结
`HttpMessageConverter`抽象了**HTTP**消息转换的策略，可以优雅地处理一些请求响应的问题。不过有一点需要注意，请求体**body**只能读取一次，即使它包裹在`ServletServerHttpRequest`中，要注意和`HttpServletRequestWrapper`的区别。
