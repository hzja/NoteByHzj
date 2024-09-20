JavaSpringCloudZuul
<a name="lhDsF"></a>
## 背景
大家平时接触最多的app就是淘宝、京东了吧！这些大的电商网站，每天处理的访问量都是亿级的。如果，不对系统中的各个接口进行保护，当并发访问量大时，系统就会发生故障。<br />所以，保护好系统，就会用到缓存、降级和限流这三把利器。断路器Hystrix的熔断与降级；缓存可以使用内存里面的缓存队列、消息中间件、分布式缓存；限流可以使用Zuul来实现。通过限流，可以很好地控制系统的qps。<br />每秒查询率QPS是对一个特定的查询服务器在规定时间内所处理流量多少的衡量标准。
<a name="dCty1"></a>
## 服务限流
<a name="Dppn8"></a>
### 方法一：Zuul+`RateLimiter`
`RateLimiter`是Guava提供的基于令牌桶算法的实现类，可以非常简单的完成限流特技，并且根据系统的实际情况来调整生成token的速率。接下来，就用代码的方式来实现Zuul的限流。<br />要用RateLimiter，需要导入相应的约束，所以，就对上篇博文中的网关服务(api-gateway)进行改造，pom中增加约束。
```xml
<dependency>
  <groupId>com.marcosbarbero.cloud</groupId>
  <artifactId>spring-cloud-zuul-ratelimit</artifactId>
  <version>2.1.0.RELEASE</version>
</dependency>
<!--redis-->
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-data-redis</artifactId>
</dependency>
```
注意，这里需要用到Redis，所以也需要把Redis的约束导入。然后在yml中添加相应的配置。
```yaml
  ratelimit:
    enabled: true
    default-policy:
      limit: 10
      quota: 4
      refresh-interval: 5
      type: #可选 限流方式
        - url
```

- `ratelimit.enabled=true`表示开启限流策略。
- `ratelimit.default-policy.limit=10` 表示请求控制在10次以内。
- `ratelimit.default-policy.quota=4` 请求时间不得超过4秒。
- `ratelimit.default-policy.refresh-interval=5` 刷新间隔是5秒。

综上所述：所有服务在5秒内只能有10次请求，并且请求的时间总和不得超过4秒。<br />配置完成，接下来，启动服务，进行测试。<br />可以看到，在启动网关服务的时候，出现了异常。上面指出`zuul.ratelimit.repository`这个配置，不能为空。相信大家对repository这个词并不陌生。原来在学Mybatis、JPA、Hibernate的时候经常看到。<br />所以，这个错误就是提示，需要某种仓库存储限流数据。通过点击配置文件，查看源码。
```java
/*
 * Copyright 2012-2018 the original author or authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.marcosbarbero.cloud.autoconfigure.zuul.ratelimit.config.properties;

public enum RateLimitRepository {
    /**
     * Uses Redis as data storage
     */
    REDIS,

    /**
     * Uses Consul as data storage
     */
    CONSUL,

    /**
     * Uses SQL database as data storage
     */
    JPA,

    /**
     * Uses Bucket4j JCache as data storage
     */
    BUCKET4J_JCACHE,

    /**
     * Uses Bucket4j Hazelcast as data storage
     */
    BUCKET4J_HAZELCAST,

    /**
     * Uses Bucket4j Ignite as data storage
     */
    BUCKET4J_IGNITE,

    /**
     * Uses Bucket4j Infinispan as data storage
     */
    BUCKET4J_INFINISPAN,
}
```
可以看到这是一个枚举类。其中，较为熟悉的就是REDIS、CONSUL、JPA。这里，就选择REDIS作为限流数据的存储。这就是pom文件中导入REDIS的原因。重新修改配置文件
```yaml
ratelimit:
  enabled: true
  repository: REDIS  #对应存储类型（用来存储统计信息）
  behind-proxy: true  #代理之后
  default-policy: #可选 - 针对所有的路由配置的策略，除非特别配置了policies
    limit: 10 #可选 - 每个刷新时间窗口对应的请求数量限制
    quota: 4 #可选-  每个刷新时间窗口对应的请求时间限制（秒）
    refresh-interval: 5 # 刷新时间窗口的时间，默认值 (秒)
    type: #可选 限流方式
      - url
```
服务正常启动，接下来进行测试。<br />上面限制5秒之内只能发送10次请求，那就发送11次请求试一试。使用postman工具进行请求的发送。同时，教大家如何使用postman批量发送请求。<br />首先，在地址栏填写接口地址，然后，可以点击send，测试当前地址是否正确。如果一切正常，点击旁边的save按钮。<br />出现上面的浮层，按照箭头的标注，进行操作，点击save按钮后，在左边就能看到已经保存的请求了。<br />然后点击图示的run按钮，进入下面的界面。<br />按照文中给出的注释，进行设定，这里就改一下请求的次数，设置为11次。然后点击run按钮发起请求，看测试结果。<br />可以看到，配置文件中规定请求次数为10次，测试前10次请求都是正常的响应。而第11次请求就被限流策略阻止了。证明了配置生效，接口限流成功。
<a name="Bc9kp"></a>
### 方法二：过滤器限流
Zuul的过滤器功能，不仅可以做权限验证，也可以拿来做限流。所以，新建一个过滤器，让其继承`ZuulFilter`
```java
package com.root.project.apigateway.filter;

import com.google.common.util.concurrent.RateLimiter;
import com.netflix.zuul.ZuulFilter;
import com.netflix.zuul.context.RequestContext;
import com.netflix.zuul.exception.ZuulException;
import org.springframework.http.HttpStatus;
import org.springframework.stereotype.Component;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;

import static org.springframework.cloud.netflix.zuul.filters.support.FilterConstants.PRE_TYPE;

/**
 * @ClassName: InterfaceLimitFilter
 * @Version 1.0
 **/
@Component
public class InterfaceLimitFilter extends ZuulFilter {

 //每秒产生10个令牌
    private static final RateLimiter RATE_LIMITER = RateLimiter.create(10);

    @Override
    public String filterType() {
        return PRE_TYPE;
    }

    @Override
    public int filterOrder() {
        return -10;
    }

    @Override
    public boolean shouldFilter() {
        RequestContext context = RequestContext.getCurrentContext();
        HttpServletRequest request = context.getRequest();
        //只对管理员服务限流
        String admin = "/api/admin/v1.0/admin";
        return admin.equalsIgnoreCase(request.getRequestURI());
    }

    /**
     * 限流逻辑
     * RATE_LIMITER.tryAcquire()尝试获取令牌
     * @return
     * @throws ZuulException
     */
    @Override
    public Object run() throws ZuulException {
        RequestContext context = RequestContext.getCurrentContext();
        //每调用一次tryAcquire()方法，令牌数量减1
        if (!RATE_LIMITER.tryAcquire()) {
            context.setSendZuulResponse(false);
            context.setResponseStatusCode(HttpStatus.TOO_MANY_REQUESTS.value());

            HttpServletResponse response = context.getResponse();
            try {
                response.setContentType("text/html;charset=UTF-8");
                response.getWriter().write("访问上限,限流啦");
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        return null;
    }
}
```
因为Guava是基于令牌桶算法的实现类，所以`RateLimiter.create(10)`可以理解为：每秒往桶里放入10个令牌。`RATE_LIMITER.tryAcquire()`方法尝试获取桶里的令牌，如果有，则返回true，并且，总的令牌数减1。没有则返回false。后面的逻辑和权限验证相似。设置Htpp状态码，然后返回信息到前台展示。<br />注意：在类上还要加入`@Component`注解，将其注入到IOC容器中。<br />启动服务，进行测试。使用postman发送20个请求。<br />奇怪的事情发生了，可以看到第十八个请求被限流了，但是第十九个请求又成功了。其实这也不难理解，当第十八个请求到达网关时，木桶里已经没有令牌可以获取了。所以，直接被限流策略限制。<br />其实，除了网关层的限流，还能在Nginx层上做限流。感兴趣的朋友，可以去看一下Nginx方面的知识。一个高性能的HTTP和反向代理服务器，作为一名开发人员，了解和使用它，也是必不可少的。
