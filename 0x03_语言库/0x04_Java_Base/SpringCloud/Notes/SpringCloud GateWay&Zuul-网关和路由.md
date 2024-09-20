Java SpringCloud GateWay
<a name="cc2c3b6e"></a>
## 1、网关应用场景

- 认证授权
- 统一外部入口
- 请求路由
- 请求限流
- 请求日志和监控
<a name="6z1TO"></a>
## 2、Spring Cloud Zuul

- 服务路由
- 自定义过滤器：通过实现ZuulFilter类的如下方法
   - `filterType()` ：过滤类型
   - `filterOrder()`：过滤顺序
   - `shouldFilter()`：是否需要过滤
   - `run()`：过滤逻辑
<a name="UvAvI"></a>
### Zuul相关依赖
```xml
<dependency>
		<groupId>org.springframework.cloud</groupId>
		<artifactId>spring-cloud-starter-netflix-zuul</artifactId>
</dependency>
```
<a name="OaBxz"></a>
### 在配置文件配置路由拦截-application.yml
<a name="csH8F"></a>
#### 路由策略配置
`zuul.routes`对应的是多个服务的Map，然后每个服务对应的服务ID和Path等等<br />直接使用服务名，需要将微服务名称暴露给用户，会存在安全性问题。所以，可以自定义路径来替代微服务名称，即自定义路由策略。
```yaml
zuul:
  routes:
    fcant:
      path: /fcant-filter
```
<a name="5e1c7efc"></a>
#### 统一前缀的配置方式
```yaml
zuul:
  prefix: /zuul
```
这样就需要通过 localhost:9000/zuul/consumer1/studentInfo/update 来进行访问了。
<a name="Ptstf"></a>
#### 服务名屏蔽
```yaml
zuul:
  ignore-services: "*"
```
<a name="30pAn"></a>
#### 路径屏蔽
Zuul 还可以指定屏蔽掉的路径 URI，即只要用户请求中包含指定的 URI 路径，那么该请求将无法访问到指定的服务。通过该方式可以限制用户的权限。
```yaml
zuul:
  ignore-patterns: **/auto/**
```
> ** 代表匹配多级任意路径
> *代表匹配一级任意路径

<a name="awKfN"></a>
#### 敏感请求头屏蔽
默认情况下，像 Cookie、Set-Cookie 等敏感请求头信息会被 zuul 屏蔽掉，可以将这些默认屏蔽去掉，当然，也可以添加要屏蔽的请求头。
<a name="rBz3n"></a>
### 在启动类添加启用Zuul代理的注解
```java
package com.fcant.springcloudgateway;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.cloud.netflix.zuul.EnableZuulProxy;

@EnableZuulProxy
@SpringBootApplication
public class SpringCloudGateWayApplication {

	public static void main(String[] args) {
		SpringApplication.run(SpringcloudgatewayApplication.class, args);
	}

}
```
<a name="gQnwl"></a>
### Zuul 的过滤功能-认证授权路由范例-AuthorizeFilter.java
实现自定义的 Filter 只需要继承 ZuulFilter 然后将这个过滤器类以 `@Component` 注解加入 Spring 容器中就行了。<br />过滤器类型：Pre、Routing、Post。前置Pre就是在请求之前进行过滤，Routing路由过滤器就是路由策略，而Post后置过滤器就是在 Response 之前进行过滤的过滤器。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596172780171-74b4e9a0-9ed1-4ac9-8a82-1a0beca1440b.png#averageHue=%23f5efee&height=333&id=ddRv7&originHeight=998&originWidth=1687&originalType=binary&ratio=1&rotation=0&showTitle=false&size=128677&status=done&style=shadow&title=&width=562.3333333333334)
```java
package com.fcant.springcloudgateway.filter;

import com.netflix.zuul.ZuulFilter;
import com.netflix.zuul.context.RequestContext;
import com.netflix.zuul.exception.ZuulException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.http.HttpStatus;
import org.springframework.stereotype.Component;

import javax.servlet.http.HttpServletRequest;
import java.util.Objects;
import java.util.UUID;

/**
 * AuthorizeFilter
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 19:23 2019/12/8
 */
@Component
public class AuthorizeFilter extends ZuulFilter {

    private static final Logger LOGGER = LoggerFactory.getLogger(AuthorizeFilter.class);
    private static String accessToken;

    public AuthorizeFilter() {
        accessToken = UUID.randomUUID().toString();
        LOGGER.info("==> accessToken : {}", accessToken);
    }

    /**
     * 外部请求 -> zuul:pre -> 选择路由的服务:routing -> 请求服务:post -> zuul
     * pre: 在请求路由之前执行
     * routing: 在请求路由之后执行
     * post: 在请求到路由对应的服务之后执行
     * error: 在其他阶段发生错误时执行
     *
     * @return filterType
     * @author Fcant 20:59 2019/12/8
     */
    @Override
    public String filterType() {
        return FilterConstants.PRE_TYPE;
    }

    /**
     * 过滤器执行的顺序：数字越小优先级约高
     *
     * @return 返回代表过滤器执行顺序等级的数字
     * @author Fcant 21:07 2019/12/8
     */
    @Override
    public int filterOrder() {
        return 0;
    }

    /**
     * 需不需要执行此过滤器
     *
     * @return True为执行
     * @author Fcant 21:07 2019/12/8
     */
    @Override
    public boolean shouldFilter() {
        return true;
    }

    /**
     * 具体的过滤逻辑
     *
     * @return 返回值被忽略
     * @author Fcant 21:08 2019/12/8
     */
    @Override
    public Object run() throws ZuulException {
        // 获取请求的上下文，注意导入时导入zuul包下的，而非apache包下的
        RequestContext currentContext = RequestContext.getCurrentContext();
        HttpServletRequest request = currentContext.getRequest();
        String accessToken = request.getParameter("access_token");
        // 模拟accessToken验证授权
        // 使用Objects.equals(Object1, Object2)比较两个对象的一直性更具安全性
        if (Objects.equals(accessToken, AuthorizeFilter.accessToken)) {
            currentContext.setResponseStatusCode(HttpStatus.OK.value());
            currentContext.setResponseBody("Authorized");
            // 此处设置停止路由，因为模拟请求未做其他API转发处理
            currentContext.setSendZuulResponse(false);
        } else {
            // 如果验证不通过就返回springframework.http.HttpStatus提供的HTTP状态码
            currentContext.setResponseStatusCode(HttpStatus.UNAUTHORIZED.value());
            currentContext.setResponseBody(HttpStatus.UNAUTHORIZED.getReasonPhrase());
            // 此处设置停止路由转发，因为模拟请求未做其他API转发处理
            currentContext.setSendZuulResponse(false);
        }
        return null;
    }
}
```
<a name="AFs9A"></a>
### 使用配置的服务拦截路径访问-[http://localhost:8080/fcant-filter](http://localhost:8080/fcant-filter)
此时返回的是未授权<br />![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1575814557032-2c996f3b-97b9-48e2-b7f8-71aa91c6a7a1.png#averageHue=%23fdfcfb&height=168&id=whCeL&originHeight=168&originWidth=418&originalType=binary&ratio=1&rotation=0&showTitle=false&size=11920&status=done&style=shadow&title=&width=418)
<a name="nAmx1"></a>
### 为请求添加参数打印的随机Token访问状态为Authorized
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1575814614684-dc218f9a-40eb-490f-a8e2-6641bc8fa2cb.png#averageHue=%23e9c890&height=191&id=xWW8u&originHeight=191&originWidth=583&originalType=binary&ratio=1&rotation=0&showTitle=false&size=14589&status=done&style=shadow&title=&width=583)
<a name="ee08ce96"></a>
### 令牌桶限流
首先会定义一个桶，如果桶里面没有满那么就会以一定固定的速率 会往里面放令牌，一个请求过来首先要从桶中获取令牌，如果没有获取到，那么这个请求就拒绝，如果获取到那么就放行。
```java
@Component
@Slf4j
public class RouteFilter extends ZuulFilter {
    // 定义一个令牌桶，每秒产生2个令牌，即每秒最多处理2个请求
    private static final RateLimiter RATE_LIMITER = RateLimiter.create(2);
    @Override
    public String filterType() {
        return FilterConstants.PRE_TYPE;
    }

    @Override
    public int filterOrder() {
        return -5;
    }

    @Override
    public Object run() throws ZuulException {
        log.info("放行");
        return null;
    }

    @Override
    public boolean shouldFilter() {
        RequestContext context = RequestContext.getCurrentContext();
        if(!RATE_LIMITER.tryAcquire()) {
            log.warn("访问量超载");
            // 指定当前请求未通过过滤
            context.setSendZuulResponse(false);
            // 向客户端返回响应码429，请求数量过多
            context.setResponseStatusCode(429);
            return false;
        }
        return true;
    }
}
```
<a name="9U8rR"></a>
## 3、网关的跨域配置
<a name="zMBwq"></a>
### SpringCloud GateWay网关配置跨域
[SpringCloud GateWay通过参考官网跨域配置](https://docs.spring.io/spring-cloud-gateway/docs/current/reference/html/#cors-configuration)在yml文件配置即可生效<br />[https://docs.spring.io/spring-cloud-gateway/docs/current/reference/html/#cors-configuration](https://docs.spring.io/spring-cloud-gateway/docs/current/reference/html/#cors-configuration)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1613920564263-a559c669-c4d9-4d15-a282-a36a22423de3.png#averageHue=%23f7f6f6&height=654&id=sNfZN&originHeight=1962&originWidth=3798&originalType=binary&ratio=1&rotation=0&showTitle=false&size=721740&status=done&style=shadow&title=&width=1266)
```yaml
spring:
  application:
    name: gateway
  cloud:
    gateway:
      globalcors:
        cors-configurations:
          '[/**]':
            # 允许携带认证信息
            # 允许跨域的源(网站域名/ip)，设置*为全部
            # 允许跨域请求里的head字段，设置*为全部
            # 允许跨域的method， 默认为GET和OPTIONS，设置*为全部
            # 跨域允许的有效期
            allow-credentials: true
            allowed-origins: "*"
            allowed-headers: "*"
            allowed-methods: "*"
            max-age: 3600
        add-to-simple-url-handler-mapping: true
```
:::info
如果需要支持HTTP的预请求需要将 `spring.cloud.gateway.globalcors.add-to-simple-url-handler-mapping` 属性为 `true` 。
:::
<a name="uowoT"></a>
### SpringCloud Zuul网关配置跨域
因为spring-cloud-starter-netflix-zuul依赖默认引入了spring-boot-starter-web依赖<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1613920285744-5003a4b4-94ba-4478-8039-4188abe33380.png#averageHue=%23efefef&height=197&id=RBk6v&originHeight=590&originWidth=1507&originalType=binary&ratio=1&rotation=0&showTitle=false&size=59582&status=done&style=shadow&title=&width=502.3333333333333)<br />所以配置跨域需要通过配置类才能生效
```java
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.web.cors.CorsConfiguration;
import org.springframework.web.cors.UrlBasedCorsConfigurationSource;
import org.springframework.web.filter.CorsFilter;

/**
 * CorsAutoConfiguration
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 上午 10:22 2021/1/5/0005
 */
@Configuration
public class CorsAutoConfiguration {

    @Bean
    public CorsFilter corsFilter(){
        //配置初始化对象
        CorsConfiguration configuration=new CorsConfiguration();
        //允许跨域的域名，如果要携带cookie，不能写* ，*：代表所有的域名都可以访问
        configuration.addAllowedOrigin("*");
        configuration.setAllowCredentials(true);
        configuration.addAllowedMethod("*");            //代表所有的请求方法
        configuration.addAllowedHeader("*");        //允许携带任何头信息

        //初始化cors配置源对象
        UrlBasedCorsConfigurationSource configurationSource=new UrlBasedCorsConfigurationSource();
        configurationSource.registerCorsConfiguration("/**",configuration);

        //返回corsFilter实例，参数:cors配置源对象
        return new CorsFilter(configurationSource);
    }
}
```
