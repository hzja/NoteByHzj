SpringCloud GateWay 动态路由<br />网关作为微服务中非常重要的一部分，是必须要掌握的!<br />记录一下如何使用Gateway搭建网关服务及实现动态路由的，了解路由相关配置，鉴权的流程及业务处理。
<a name="dpD8R"></a>
## 搭建服务

- SpringBoot 2.1
```xml
<parent>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-parent</artifactId>
  <version>2.1.0.RELEASE</version>
</parent>
```

- Spring-cloud-gateway-core
```xml
<dependency>
  <groupId>org.springframework.cloud</groupId>
  <artifactId>spring-cloud-gateway-core</artifactId>
</dependency>
```

- common-lang3
```xml
<dependency>
  <groupId>org.apache.commons</groupId>
  <artifactId>commons-lang3</artifactId>
</dependency>
```
<a name="AJrtu"></a>
## 路由配置
网关作为请求统一入口，路由就相当于是每个业务系统的入口，通过路由规则则可以匹配到对应微服务的入口，将请求命中到对应的业务系统中
```yaml
server:
  port: 8080
 
spring:
  cloud:
    gateway:
      enabled: true
      routes:
      - id: demo-server
        uri: http://localhost:8081
        predicates:
        - Path=/demo-server/**
        filters:
          - StripPrefix= 1
```
routes的配置如下:

| 配置项 | 描述 |
| --- | --- |
| id | 路由唯一id，使用服务名称即可 |
| uri | 路由服务的访问地址 |
| predicates | 路由断言 |
| filters | 过滤规则 |

<a name="hwBJU"></a>
### 配置解读

- 现在有一个服务demo-server部署在本机，地址和端口为127.0.0.1:8081，所以路由配置uri为http://localhost:8081
- 使用网关服务路由到此服务，`predicates -Path=/demo-server/**`，网关服务的端口为8080，启动网关服务，访问localhost:8080/demo-server，路由断言就会将请求路由到demo-server
- 直接访问demo-server的接口localhost:8081/api/test，通过网关的访问地址则为localhost:8080/demo-server/api/test，predicates配置将请求断言到此路由，`filters-StripPrefix=1`代表将地址中/后的第一个截取，所以demo-server就截取掉了

使用gateway通过配置文件即可完成路由的配置，非常方便，只要充分的了解配置项的含义及规则就可以了；但是这些配置如果要修改则需要重启服务，重启网关服务会导致整个系统不可用，这一点是无法接受的，下面介绍如何通过Nacos实现动态路由
<a name="f8hTv"></a>
## 动态路由
使用nacos结合gateway-server实现动态路由，需要先部署一个nacos服务，可以使用docker部署或下载源码在本地启动，具体操作可以参考官方文档即可
<a name="n1FDR"></a>
### Nacos配置
![2022-11-16-08-54-14.423268000.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1668560365095-2b3e2e45-9fea-4178-95b5-7f6c2c0d3ad0.png#averageHue=%23939393&clientId=u68f19756-6fec-4&from=ui&id=u4d4cd031&originHeight=567&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1840699&status=done&style=none&taskId=u441ab23b-d929-4584-9f50-bbe80b0f380&title=)

- groupId: 使用网关服务名称即可
- dataId: routes
- 配置格式：json
```json
[{
  "id": "xxx-server",
  "order": 1, #优先级
  "predicates": [{ #路由断言
    "args": {
      "pattern": "/xxx-server/**"
    },
    "name": "Path"
  }],
  "filters":[{ #过滤规则
    "args": {
      "parts": 0 #k8s服务内部访问容器为http://xxx-server/xxx-server的话,配置0即可
    },
    "name": "StripPrefix" #截取的开始索引
  }],
  "uri": "http://localhost:8080/xxx-server" #目标地址
}]
```
json格式配置项与yaml中对应，需要了解配置在json中的写法
<a name="jOBAk"></a>
### 比对一下json配置与yaml配置
![2022-11-16-08-54-15.545916500.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1668560367028-2ca1594f-8a45-4d1e-a718-959c820620b3.png#averageHue=%23f9f9f8&clientId=u68f19756-6fec-4&from=ui&id=ePU7U&originHeight=1255&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=4074111&status=done&style=none&taskId=ue472c523-3d06-469e-8a3a-345f3240950&title=)
<a name="Eap6r"></a>
### 代码实现
Nacos实现动态路由的方式核心就是通过Nacos配置监听，配置发生改变后执行网关相关api创建路由<br />![2022-11-16-08-54-16.574272400.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1668560387230-b116a2ef-f801-44ab-b8cd-4bb67162526c.png#averageHue=%23f5f5f5&clientId=u68f19756-6fec-4&from=ui&id=u142acde6&originHeight=424&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1376477&status=done&style=none&taskId=ub7674d21-e494-49af-87be-e1e7190f4b7&title=)
```java
@Component
public class NacosDynamicRouteService implements ApplicationEventPublisherAware {
 
    private static final Logger LOGGER = LoggerFactory.getLogger(NacosDynamicRouteService.class);
 
    @Autowired
    private RouteDefinitionWriter routeDefinitionWriter;
 
    private ApplicationEventPublisher applicationEventPublisher;
 
    /** 路由id */
    private static List<String> routeIds = Lists.newArrayList();
 
    /**
     * 监听nacos路由配置，动态改变路由
     * @param configInfo
     */
    @NacosConfigListener(dataId = "routes", groupId = "gateway-server")
    public void routeConfigListener(String configInfo) {
        clearRoute();
        try {
            List<RouteDefinition> gatewayRouteDefinitions = JSON.parseArray(configInfo, RouteDefinition.class);
            for (RouteDefinition routeDefinition : gatewayRouteDefinitions) {
                addRoute(routeDefinition);
            }
            publish();
            LOGGER.info("Dynamic Routing Publish Success");
        } catch (Exception e) {
            LOGGER.error(e.getMessage(), e);
        } 
    }
    /**
     * 清空路由
     */
    private void clearRoute() {
        for (String id : routeIds) {
            routeDefinitionWriter.delete(Mono.just(id)).subscribe();
        }
        routeIds.clear();
    }
 
    @Override
    public void setApplicationEventPublisher(ApplicationEventPublisher applicationEventPublisher) {
        this.applicationEventPublisher = applicationEventPublisher;
    }
    /**
     * 添加路由
     * 
     * @param definition
     */
    private void addRoute(RouteDefinition definition) {
        try {
            routeDefinitionWriter.save(Mono.just(definition)).subscribe();
            routeIds.add(definition.getId());
        } catch (Exception e) {
            LOGGER.error(e.getMessage(), e);
        }
    }
    /**
     * 发布路由、使路由生效
     */
    private void publish() {
        this.applicationEventPublisher.publishEvent(new RefreshRoutesEvent(this.routeDefinitionWriter));
    }
}
```
<a name="vbBHQ"></a>
## 过滤器
gateway提供`GlobalFilter`及`Ordered`两个接口用来定义过滤器，自定义过滤器只需要实现这个两个接口即可

- GlobalFilter `filter()` 实现过滤器业务
- Ordered `getOrder()` 定义过滤器执行顺序

通常一个网关服务的过滤主要包含 鉴权（是否登录、是否黑名单、是否免登录接口...） 限流（ip限流等等）功能，我们今天简单介绍鉴权过滤器的流程实现
<a name="GC1S3"></a>
### 鉴权过滤器
需要实现鉴权过滤器，先得了解登录及鉴权流程，如下图所示<br />![2022-11-16-08-54-17.196582200.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1668560388229-f2cd09d0-7eb7-4c2b-85a6-d0dee4ef274e.png#averageHue=%23f4f3f3&clientId=u68f19756-6fec-4&from=ui&id=ykO2n&originHeight=927&originWidth=1044&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2909063&status=done&style=none&taskId=u8abfc91e-1fdf-4476-8c0f-99578dcf0a1&title=)<br />由图可知，鉴权过滤核心就是验证token是否有效，所以网关服务需要与业务系统在同一个redis库，先给网关添加redis依赖及配置
```xml
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-data-redis-reactive</artifactId>
</dependency>
```
```yaml
spring:
  redis:
    host: redis-server
    port: 6379
    password:
    database: 0
```
代码实现

1. 定义过滤器AuthFilter
2. 获取请求对象 从请求头或参数或cookie中获取token(支持多种方式传token对于客户端更加友好，比如部分web下载请求会新建一个页面，在请求头中传token处理起来比较麻烦)
3. 没有token，返回401
4. 有token，查询redis是否有效
5. 无效则返回401，有效则完成验证放行
6. 重置token过期时间、添加内部请求头信息方便业务系统权限处理
```java
@Component
public class AuthFilter implements GlobalFilter, Ordered {
    @Autowired
    private RedisTemplate<String, String> redisTemplate;
    private static final String TOKEN_HEADER_KEY = "auth_token";
    @Override
    public Mono<Void> filter(ServerWebExchange exchange, GatewayFilterChain chain) {
        // 1.获取请求对象
        ServerHttpRequest request = exchange.getRequest();
        // 2.获取token
        String token = getToken(request);
        ServerHttpResponse response = exchange.getResponse();
        if (StringUtils.isBlank(token)) {
            // 3.token为空 返回401
            response.setStatusCode(HttpStatus.UNAUTHORIZED);
            return response.setComplete();
        }
        // 4.验证token是否有效
        String userId = getUserIdByToken(token);
        if (StringUtils.isBlank(userId)) {
            // 5.token无效 返回401
            response.setStatusCode(HttpStatus.UNAUTHORIZED);
            return response.setComplete();
        }
        // token有效，后续业务处理
        // 从写请求头，方便业务系统从请求头获取用户id进行权限相关处理
        ServerHttpRequest.Builder builder = exchange.getRequest().mutate();
        request = builder.header("user_id", userId).build();
        // 延长缓存过期时间-token缓存用户如果一直在操作就会一直重置过期
        // 这样避免用户操作过程中突然过期影响业务操作及体验，只有用户操作间隔时间大于缓存过期时间才会过期
        resetTokenExpirationTime(token, userId);
        // 完成验证
        return chain.filter(exchange);
    }
 
    @Override
    public int getOrder() {
        // 优先级 越小越优先
        return 0;
    }
 
    /**
     * 从redis中获取用户id
     * 在登录操作时候 登陆成功会生成一个token, redis得key为auth_token:token 值为用户id
     *
     * @param token
     * @return
     */
    private String getUserIdByToken(String token) {
        String redisKey = String.join(":", "auth_token", token);
        return redisTemplate.opsForValue().get(redisKey);
    }
 
    /**
     * 重置token过期时间
     *
     * @param token
     * @param userId
     */
    private void resetTokenExpirationTime(String token, String userId) {
        String redisKey = String.join(":", "auth_token", token);
        redisTemplate.opsForValue().set(redisKey, userId, 2, TimeUnit.HOURS);
    }
 
    /**
     * 获取token
     *
     * @param request
     * @return
     */
    private static String getToken(ServerHttpRequest request) {
        HttpHeaders headers = request.getHeaders();
        // 从请求头获取token
        String token = headers.getFirst(TOKEN_HEADER_KEY);
        if (StringUtils.isBlank(token)) {
            // 请求头无token则从url获取token
            token = request.getQueryParams().getFirst(TOKEN_HEADER_KEY);
        }
        if (StringUtils.isBlank(token)) {
            // 请求头和url都没有token则从cookies获取
            HttpCookie cookie = request.getCookies().getFirst(TOKEN_HEADER_KEY);
            if (cookie != null) {
                token = cookie.getValue();
            }
        }
        return token;
    }
}
```
<a name="FQRWH"></a>
## 总结
Gateway通过配置项可以实现路由功能，整合Nacos及配置监听可以实现动态路由，实现GlobalFilter，Ordered两个接口可以快速实现一个过滤器，文中也详细的介绍了登录后的请求鉴权流程。
