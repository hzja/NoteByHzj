Java SpringBoot 日志<br />某些业务需求需要追踪接口访问情况，也就是把请求和响应记录下来。基本的记录维度包含了请求入参（路径query参数，请求体）、请求路径（uri）、请求方法(method)、请求头（headers）以及响应状态、响应头、甚至包含了敏感的响应体等等。总结了几种方法，可以按需选择。
<a name="CtlEX"></a>
## 请求追踪的实现方式
<a name="WCN5n"></a>
### 网关层
很多网关设施都具有httptrace的功能，可以帮助集中记录请求流量的情况。Orange、Kong、Apache Apisix这些基于Nginx的网关都具有该能力，就连Nginx本身也提供了记录httptrace日志的能力。<br />优点是可以集中的管理httptrace日志，免开发；缺点是技术要求高，需要配套的分发、存储、查询的设施。
<a name="hfXpG"></a>
### Spring Boot Actuator
在Spring Boot中，其实提供了简单的追踪功能。只需要集成：
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-actuator</artifactId>
</dependency>
```
开启/actuator/httptrace：
```yaml
management:
  endpoints:
    web:
      exposure:
        include: 'httptrace'
```
就可以通过http://server:port/actuator/httptrace获取最近的Http请求信息了。<br />不过在最新的版本中可能需要显式的声明这些追踪信息的存储方式，也就是实现`HttpTraceRepository`接口并注入Spring IoC。<br />例如放在内存中并限制为最近的100条（不推荐生产使用）：
```java
@Bean
public HttpTraceRepository httpTraceRepository(){
    return new InMemoryHttpTraceRepository();
}
```
追踪日志以json格式呈现：<br />![2021-08-11-20-04-33-543883.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1628683498530-9826bcb3-ba9b-451c-9317-5d293d8965ee.png#clientId=u55666ce8-e890-4&from=ui&id=ued6659f7&originHeight=347&originWidth=632&originalType=binary&ratio=1&size=37052&status=done&style=none&taskId=u9b25eb3a-9681-481a-8a06-a5c2dd2b007)<br />Spring Boot Actuator记录的httptrace<br />记录的维度不多，当然如果够用的话可以试试。<br />优点在于集成起来简单，几乎免除开发；缺点在于记录的维度不多，而且需要搭建缓冲消费这些日志信息的设施。
<a name="mDfqx"></a>
### `CommonsRequestLoggingFilter`
Spring Web模块还提供了一个过滤器`CommonsRequestLoggingFilter`，它可以对请求的细节进行日志输出。配置起来也比较简单：
```java
@Bean
CommonsRequestLoggingFilter  loggingFilter(){
    CommonsRequestLoggingFilter loggingFilter = new CommonsRequestLoggingFilter();
    // 记录 客户端 IP信息
    loggingFilter.setIncludeClientInfo(true);
    // 记录请求头
    loggingFilter.setIncludeHeaders(true);
    // 如果记录请求头的话，可以指定哪些记录，哪些不记录
    // loggingFilter.setHeaderPredicate();
    // 记录 请求体  特别是POST请求的body参数
    loggingFilter.setIncludePayload(true);
    // 请求体的大小限制 默认50
    loggingFilter.setMaxPayloadLength(10000);
    //记录请求路径中的query参数 
    loggingFilter.setIncludeQueryString(true);
    return loggingFilter;
}
```
而且必须开启对`CommonsRequestLoggingFilter`的`debug`日志：
```yaml
logging:
  level:
    org:
      springframework:
        web:
          filter:
            CommonsRequestLoggingFilter: debug
```
一次请求会输出两次日志，一次是在第一次经过过滤器前；一次是完成过滤器链后。<br />![2021-08-11-20-04-33-620885.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1628683488175-2ccdd560-0073-41de-8e81-f7cc0d31e009.png#clientId=u55666ce8-e890-4&from=ui&id=ufc6b2c5c&originHeight=176&originWidth=1080&originalType=binary&ratio=1&size=97915&status=done&style=none&taskId=ucaff936c-d491-452f-8d9d-1bdc69ce314)<br />CommonsRequestLoggingFilter记录请求日志<br />这里多说一句其实可以改造成输出json格式的。<br />优点是灵活配置、而且对请求追踪的维度全面，缺点是只记录请求而不记录响应。
<a name="CQCVi"></a>
### ResponseBodyAdvice
Spring Boot统一返回体其实也能记录，需要自行实现。这里借鉴了`CommonsRequestLoggingFilter`解析请求的方法。响应体也可以获取了，不过响应头和状态因为生命周期还不清楚，这里获取还不清楚是否合适，不过这是一个思路。
```java
@Slf4j
@RestControllerAdvice(basePackages = {"cn.fcant.logging"})
public class RestBodyAdvice implements ResponseBodyAdvice<Object> {
    private static final int DEFAULT_MAX_PAYLOAD_LENGTH = 10000;
    public static final String REQUEST_MESSAGE_PREFIX = "Request [";
    public static final String REQUEST_MESSAGE_SUFFIX = "]";
    private ObjectMapper objectMapper = new ObjectMapper();

    @Override
    public boolean supports(MethodParameter returnType, Class<? extends HttpMessageConverter<?>> converterType) {
        return true;
    }

    @SneakyThrows
    @Override
    public Object beforeBodyWrite(Object body,
                                  MethodParameter returnType,
                                  MediaType selectedContentType,
                                  Class<? extends HttpMessageConverter<?>> selectedConverterType,
                                  ServerHttpRequest request,
                                  ServerHttpResponse response) {

        ServletServerHttpRequest servletServerHttpRequest = (ServletServerHttpRequest) request;

        log.debug(createRequestMessage(servletServerHttpRequest.getServletRequest(), REQUEST_MESSAGE_PREFIX, REQUEST_MESSAGE_SUFFIX));
        Rest<Object> objectRest;
        if (body == null) {
            objectRest = RestBody.okData(Collections.emptyMap());
        } else if (Rest.class.isAssignableFrom(body.getClass())) {
            objectRest = (Rest<Object>) body;
        }
        else if (checkPrimitive(body)) {
            return RestBody.okData(Collections.singletonMap("result", body));
        }else {
            objectRest = RestBody.okData(body);
        }
        log.debug("Response Body ["+ objectMapper.writeValueAsString(objectRest) +"]");
        return objectRest;
    }


    private boolean checkPrimitive(Object body) {
        Class<?> clazz = body.getClass();
        return clazz.isPrimitive()
                || clazz.isArray()
                || Collection.class.isAssignableFrom(clazz)
                || body instanceof Number
                || body instanceof Boolean
                || body instanceof Character
                || body instanceof String;
    }


    protected String createRequestMessage(HttpServletRequest request, String prefix, String suffix) {
        StringBuilder msg = new StringBuilder();
        msg.append(prefix);
        msg.append(request.getMethod()).append(" ");
        msg.append(request.getRequestURI());


        String queryString = request.getQueryString();
        if (queryString != null) {
            msg.append('?').append(queryString);
        }


        String client = request.getRemoteAddr();
        if (StringUtils.hasLength(client)) {
            msg.append(", client=").append(client);
        }
        HttpSession session = request.getSession(false);
        if (session != null) {
            msg.append(", session=").append(session.getId());
        }
        String user = request.getRemoteUser();
        if (user != null) {
            msg.append(", user=").append(user);
        }

        HttpHeaders headers = new ServletServerHttpRequest(request).getHeaders();
        msg.append(", headers=").append(headers);

        String payload = getMessagePayload(request);
        if (payload != null) {
            msg.append(", payload=").append(payload);
        }

        msg.append(suffix);
        return msg.toString();
    }

    protected String getMessagePayload(HttpServletRequest request) {
        ContentCachingRequestWrapper wrapper =
                WebUtils.getNativeRequest(request, ContentCachingRequestWrapper.class);
        if (wrapper != null) {
            byte[] buf = wrapper.getContentAsByteArray();
            if (buf.length > 0) {
                int length = Math.min(buf.length, DEFAULT_MAX_PAYLOAD_LENGTH);
                try {
                    return new String(buf, 0, length, wrapper.getCharacterEncoding());
                } catch (UnsupportedEncodingException ex) {
                    return "[unknown]";
                }
            }
        }
        return null;
    }
}
```
别忘记配置`ResponseBodyAdvice`的logging级别为`DEBUG`。
<a name="P97jE"></a>
### logstash-logback-encoder
这个是logstash的logback编码器，可以结构化输出httptrace为json。引入：
```xml
<dependency>
    <groupId>net.logstash.logback</groupId>
    <artifactId>logstash-logback-encoder</artifactId>
    <version>6.6</version>
</dependency>
```
配置logback的`ConsoleAppender`为`LogstashEncoder`:
```xml
<configuration>
    <appender name="jsonConsoleAppender" class="ch.qos.logback.core.ConsoleAppender">
        <encoder class="net.logstash.logback.encoder.LogstashEncoder"/>
    </appender>
    <root level=" INFO">
        <appender-ref ref="jsonConsoleAppender"/>
    </root>
</configuration>
```
然后同样实现一个解析的`Filter`:
```java
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.slf4j.MDC;
import org.springframework.core.annotation.Order;
import org.springframework.stereotype.Component;

import javax.servlet.*;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.util.UUID;

@Order(1)
@Component
public class MDCFilter implements Filter {

    private final Logger LOGGER = LoggerFactory.getLogger(MDCFilter.class);
    private final String X_REQUEST_ID = "X-Request-ID";

    @Override
    public void doFilter(ServletRequest request,
                         ServletResponse response,
                         FilterChain chain) throws IOException, ServletException {
        HttpServletRequest req = (HttpServletRequest) request;
        HttpServletResponse res = (HttpServletResponse) response;
        try {
            addXRequestId(req);
            LOGGER.info("path: {}, method: {}, query {}",
                    req.getRequestURI(), req.getMethod(), req.getQueryString());
            res.setHeader(X_REQUEST_ID, MDC.get(X_REQUEST_ID));
            chain.doFilter(request, response);
        } finally {
            LOGGER.info("statusCode {}, path: {}, method: {}, query {}",
                    res.getStatus(), req.getRequestURI(), req.getMethod(), req.getQueryString());
            MDC.clear();
        }
    }

    private void addXRequestId(HttpServletRequest request) {
        String xRequestId = request.getHeader(X_REQUEST_ID);
        if (xRequestId == null) {
            MDC.put(X_REQUEST_ID, UUID.randomUUID().toString());
        } else {
            MDC.put(X_REQUEST_ID, xRequestId);
        }
    }

}
```
这里解析方式其实还可以更加精细一些。<br />然后所有的日志都可以结构化为json了：
```json
{
  "@timestamp":"2021-08-10T23:48:51.322+08:00",
  "@version":"1",
  "message":"statusCode 200, path: /log/get, method: GET, query foo=xxx&bar=ooo",
  "logger_name":"cn.fcant.logging.MDCFilter",
  "thread_name":"http-nio-8080-exec-1",
  "level":"INFO",
  "level_value":20000,
  "X-Request-ID":"7c0db56c-b1f2-4d85-ad9a-7ead67660f96"
}
```
