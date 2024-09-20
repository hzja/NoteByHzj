Java SpringCloud SpringCloud Gateway CROS
<a name="J17P7"></a>
## 场景
在 SpringCloud 项目中，前后端分离目前很常见，在调试时，会遇到两种情况的跨域：
<a name="I4X6Z"></a>
### | 前端页面通过不同域名或IP访问微服务的后台
例如前端人员会在本地起HttpServer 直连后台开发本地起的服务，此时，如果不加任何配置，前端页面的请求会被浏览器跨域限制拦截，所以，业务服务常常会添加如下代码设置全局跨域：
```java
@Bean
public CorsFilter corsFilter() {
    logger.debug("CORS限制打开");
    CorsConfiguration config = new CorsConfiguration();
    # 仅在开发环境设置为*
    config.addAllowedOrigin("*");
    config.addAllowedHeader("*");
    config.addAllowedMethod("*");
    config.setAllowCredentials(true);
    UrlBasedCorsConfigurationSource configSource = new UrlBasedCorsConfigurationSource();
    configSource.registerCorsConfiguration("/**", config);
    return new CorsFilter(configSource);
}
```
<a name="hQosb"></a>
### | 前端页面通过不同域名或IP访问SpringCloud Gateway
例如前端人员在本地起HttpServer直连服务器的Gateway进行调试。此时，同样会遇到跨域。需要在Gateway的配置文件中增加：
```yaml
spring:
  cloud:
    gateway:
      globalcors:
        cors-configurations:
        # 仅在开发环境设置为*
          '[/**]':
            allowedOrigins: "*"
            allowedHeaders: "*"
            allowedMethods: "*"
```
那么，此时直连微服务和网关的跨域问题都解决了，是不是很完美？<br />**No~ 问题来了，前端仍然会报错：“不允许有多个'Access-Control-Allow-Origin' CORS头”**。
```
Access to XMLHttpRequest at 'http://192.168.2.137:8088/api/two' from origin 'http://localhost:3200' has been blocked by CORS policy: 
The 'Access-Control-Allow-Origin' header contains multiple values '*, http://localhost:3200', but only one is allowed.
```
仔细查看返回的响应头，里面包含了两份`Access-Control-Allow-Origin`头。<br />用客户端版的PostMan做一个模拟，在请求里设置头：`Origin : *` ，查看返回结果的头:<br />不能用Chrome插件版，由于浏览器的限制，插件版设置Origin的Header是无效的<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1646563569648-eaab4ab8-ae05-4233-bd07-3630c23b791f.png#clientId=ud2938b50-0475-4&from=paste&id=u6997a058&originHeight=719&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=udcce5f3d-342b-4290-b2c7-6704fea7285&title=)<br />**发现问题了：**Vary 和 `Access-Control-Allow-Origin` 两个头重复了两次，其中浏览器对后者有唯一性限制！
<a name="zFAsY"></a>
## 分 析
Spring Cloud Gateway是基于SpringWebFlux的，所有web请求首先是交给`DispatcherHandler`进行处理的，将HTTP请求交给具体注册的handler去处理。<br />Spring Cloud Gateway进行请求转发，是在配置文件里配置路由信息，一般都是用url predicates模式，对应的就是`RoutePredicateHandlerMapping` 。所以，`DispatcherHandler`会把请求交给 `RoutePredicateHandlerMapping`。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1646563569701-a19ded3d-e4f3-414d-8a9d-95f6db3122cf.png#clientId=ud2938b50-0475-4&from=paste&id=ua9c52c8c&originHeight=228&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc1fb32fe-01eb-4193-85d5-92eda7e0287&title=)<br />`**RoutePredicateHandlerMapping.getHandler(ServerWebExchange exchange)**`** 方法，默认提供者是其父类 **`**AbstractHandlerMapping**`** ：**
```java
@Override
public Mono<Object> getHandler(ServerWebExchange exchange) {
	return getHandlerInternal(exchange).map(handler -> {
		if (logger.isDebugEnabled()) {
			logger.debug(exchange.getLogPrefix() + "Mapped to " + handler);
		}
		ServerHttpRequest request = exchange.getRequest();
		// 可以看到是在这一行就进行CORS判断，两个条件：
		// 1. 是否配置了CORS，如果不配的话，默认是返回false的
		// 2. 或者当前请求是OPTIONS请求，且头里包含ORIGIN和ACCESS_CONTROL_REQUEST_METHOD
		if (hasCorsConfigurationSource(handler) || CorsUtils.isPreFlightRequest(request)) {
			CorsConfiguration config = (this.corsConfigurationSource != null ? this.corsConfigurationSource.getCorsConfiguration(exchange) : null);
			CorsConfiguration handlerConfig = getCorsConfiguration(handler, exchange);
			config = (config != null ? config.combine(handlerConfig) : handlerConfig);
			//此处交给DefaultCorsProcessor去处理了
			if (!this.corsProcessor.process(config, exchange) || CorsUtils.isPreFlightRequest(request)) {
				return REQUEST_HANDLED_HANDLER;
			}
		}
		return handler;
	});
}
```
> 网上有些关于修改Gateway的CORS设定的方式，是跟前面SpringBoot一样，实现一个`CorsWebFilter`的Bean，靠写代码提供 `CorsConfiguration` ，而不是修改Gateway的配置文件。其实本质，都是将配置交给`corsProcessor`去处理，殊途同归。但靠配置解决永远比hard code来的优雅。

该方法把Gateway里定义的所有的 `GlobalFilter` 加载进来，作为handler返回，但在返回前，先进行CORS校验，获取配置后，交给`corsProcessor`去处理，即`DefaultCorsProcessor`类。<br />**看下**`**DefaultCorsProcessor**`**的**`**process**`**方法**
```java
@Override
public boolean process(@Nullable CorsConfiguration config, ServerWebExchange exchange) {

    ServerHttpRequest request = exchange.getRequest();
    ServerHttpResponse response = exchange.getResponse();
    HttpHeaders responseHeaders = response.getHeaders();

    List<String> varyHeaders = responseHeaders.get(HttpHeaders.VARY);
    if (varyHeaders == null) {
        // 第一次进来时，肯定是空，所以加了一次VERY的头，包含ORIGIN, ACCESS_CONTROL_REQUEST_METHOD和ACCESS_CONTROL_REQUEST_HEADERS
        responseHeaders.addAll(HttpHeaders.VARY, VARY_HEADERS);
    }
    else {
        for (String header : VARY_HEADERS) {
            if (!varyHeaders.contains(header)) {
                responseHeaders.add(HttpHeaders.VARY, header);
            }
        }
    }

    if (!CorsUtils.isCorsRequest(request)) {
        return true;
    }

    if (responseHeaders.getFirst(HttpHeaders.ACCESS_CONTROL_ALLOW_ORIGIN) != null) {
        logger.trace("Skip: response already contains \"Access-Control-Allow-Origin\"");
        return true;
    }

    boolean preFlightRequest = CorsUtils.isPreFlightRequest(request);
    if (config == null) {
        if (preFlightRequest) {
            rejectRequest(response);
            return false;
        }
        else {
            return true;
        }
    }

    return handleInternal(exchange, config, preFlightRequest);
}

// 在这个类里进行实际的CORS校验和处理
protected boolean handleInternal(ServerWebExchange exchange,
                                 CorsConfiguration config, boolean preFlightRequest) {

    ServerHttpRequest request = exchange.getRequest();
    ServerHttpResponse response = exchange.getResponse();
    HttpHeaders responseHeaders = response.getHeaders();

    String requestOrigin = request.getHeaders().getOrigin();
    String allowOrigin = checkOrigin(config, requestOrigin);
    if (allowOrigin == null) {
        logger.debug("Reject: '" + requestOrigin + "' origin is not allowed");
        rejectRequest(response);
        return false;
    }

    HttpMethod requestMethod = getMethodToUse(request, preFlightRequest);
    List<HttpMethod> allowMethods = checkMethods(config, requestMethod);
    if (allowMethods == null) {
        logger.debug("Reject: HTTP '" + requestMethod + "' is not allowed");
        rejectRequest(response);
        return false;
    }

    List<String> requestHeaders = getHeadersToUse(request, preFlightRequest);
    List<String> allowHeaders = checkHeaders(config, requestHeaders);
    if (preFlightRequest && allowHeaders == null) {
        logger.debug("Reject: headers '" + requestHeaders + "' are not allowed");
        rejectRequest(response);
        return false;
    }
    //此处添加了AccessControllAllowOrigin的头
    responseHeaders.setAccessControlAllowOrigin(allowOrigin);

    if (preFlightRequest) {
        responseHeaders.setAccessControlAllowMethods(allowMethods);
    }

    if (preFlightRequest && !allowHeaders.isEmpty()) {
        responseHeaders.setAccessControlAllowHeaders(allowHeaders);
    }

    if (!CollectionUtils.isEmpty(config.getExposedHeaders())) {
        responseHeaders.setAccessControlExposeHeaders(config.getExposedHeaders());
    }

    if (Boolean.TRUE.equals(config.getAllowCredentials())) {
        responseHeaders.setAccessControlAllowCredentials(true);
    }

    if (preFlightRequest && config.getMaxAge() != null) {
        responseHeaders.setAccessControlMaxAge(config.getMaxAge());
    }

    return true;
}
```
可以看到，在`DefaultCorsProcessor` 中，根据在appliation.yml 中的配置，给Response添加了 Vary 和 `Access-Control-Allow-Origin` 的头。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1646563569658-13fc8db3-9cc3-4176-8188-1cb8423c1bcd.png#clientId=ud2938b50-0475-4&from=paste&id=u554acb26&originHeight=454&originWidth=651&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u78718ed5-db3f-4491-af6b-9b569f30f9b&title=)<br />**再接下来就是进入各个**`**GlobalFilter**`**进行处理了，其中**`**NettyRoutingFilter**`** 是负责实际将请求转发给后台微服务，并获取Response的，重点看下代码中filter的处理结果的部分：**<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1646563569656-d9d04aa2-aa14-4e74-87eb-6a6883c4fab2.png#clientId=ud2938b50-0475-4&from=paste&id=u09266861&originHeight=902&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u611fa2e9-0df3-40bd-b01e-22ee35a66b9&title=)其中以下几种header会被过滤掉的：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1646563569703-9d78d4b8-69a2-49c6-936d-201c916eafc4.png#clientId=ud2938b50-0475-4&from=paste&id=ub512a6a7&originHeight=336&originWidth=526&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1cd98cb8-d811-491b-ad80-4fc9e0d6f84&title=)<br />很明显，在图里的第3步中，如果后台服务返回的header里有 `Vary` 和 `Access-Control-Allow-Origin` ，这时由于是`putAll`，没有做任何去重就加进去了，必然会重复，看看DEBUG结果验证一下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1646563570044-2a0eb740-e63f-4ec5-9a49-7ab9683719cd.png#clientId=ud2938b50-0475-4&from=paste&id=u4a676e84&originHeight=686&originWidth=969&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u16dd8e02-2e02-4929-8e74-35064ba85f8&title=)验证了前面的发现。
<a name="bHs96"></a>
## 解决方案
解决的方案有两种：
<a name="U17ue"></a>
### | 利用`DedupeResponseHeader`配置
```yaml
spring:
    cloud:
        gateway:
          globalcors:
            cors-configurations:
              '[/**]':
                allowedOrigins: "*"
                allowedHeaders: "*"
                allowedMethods: "*"
          default-filters:
          - DedupeResponseHeader=Vary Access-Control-Allow-Origin Access-Control-Allow-Credentials, RETAIN_FIRST
```
`DedupeResponseHeader`加上以后会启用`DedupeResponseHeaderGatewayFilterFactory` 在其中，`dedupe`方法可以按照给定策略处理值。
```java
private void dedupe(HttpHeaders headers, String name, Strategy strategy) {
	List<String> values = headers.get(name);
	if (values == null || values.size() <= 1) {
		return;
	}
	switch (strategy) {
			// 只保留第一个
		case RETAIN_FIRST:
			headers.set(name, values.get(0));
			break;
			// 保留最后一个        
		case RETAIN_LAST:
			headers.set(name, values.get(values.size() - 1));
			break;
			// 去除值相同的
		case RETAIN_UNIQUE:
			headers.put(name, values.stream().distinct().collect(Collectors.toList()));
			break;
		default:
			break;
	}
 }
```
如果请求中设置的Origin的值与自己设置的是同一个，例如生产环境设置的都是自己的域名xxx.com或者开发测试环境设置的都是`*`（浏览器中是无法设置Origin的值，设置了也不起作用，浏览器默认是当前访问地址），那么可以选用`RETAIN_UNIQUE`策略，去重后返回到前端。<br />如果请求中设置的Oringin的值与自己设置的不是同一个，`RETAIN_UNIQUE`策略就无法生效，比如“`*`”和 “xxx.com”是两个不一样的Origin，最终还是会返回两个`Access-Control-Allow-Origin` 的头。此时，看代码里，response的header里，先加入的是自己配置的`Access-Control-Allow-Origin`的值，所以，可以将策略设置为`RETAIN_FIRST `，只保留自己设置的。<br />大多数情况下，想要返回的是自己设置的规则，所以直接使用`RETAIN_FIRST` 即可。实际上，`DedupeResponseHeader` 可以针对所有头，做重复的处理。
<a name="aCEWg"></a>
### | 手动写一个 `CorsResponseHeaderFilter` 的 `GlobalFilter` 去修改Response中的头
此处有两个地方要注意：
```java
@Component
public class CorsResponseHeaderFilter implements GlobalFilter, Ordered {

    private static final Logger logger = LoggerFactory.getLogger(CorsResponseHeaderFilter.class);

    private static final String ANY = "*";

    @Override
    public int getOrder() {
        // 指定此过滤器位于NettyWriteResponseFilter之后
        // 即待处理完响应体后接着处理响应头
        return NettyWriteResponseFilter.WRITE_RESPONSE_FILTER_ORDER + 1;
    }

    @Override
    @SuppressWarnings("serial")
    public Mono<Void> filter(ServerWebExchange exchange, GatewayFilterChain chain) {
        return chain.filter(exchange).then(Mono.fromRunnable(() -> {
            exchange.getResponse().getHeaders().entrySet().stream()
                    .filter(kv -> (kv.getValue() != null && kv.getValue().size() > 1))
                    .filter(kv -> (kv.getKey().equals(HttpHeaders.ACCESS_CONTROL_ALLOW_ORIGIN)
                            || kv.getKey().equals(HttpHeaders.ACCESS_CONTROL_ALLOW_CREDENTIALS)
                            || kv.getKey().equals(HttpHeaders.VARY)))
                    .forEach(kv ->
                    {
                        // Vary只需要去重即可
                        if(kv.getKey().equals(HttpHeaders.VARY))
                            kv.setValue(kv.getValue().stream().distinct().collect(Collectors.toList()));
                        else{
                            List<String> value = new ArrayList<>();
                            if(kv.getValue().contains(ANY)){  //如果包含*，则取*
                                value.add(ANY);
                                kv.setValue(value);
                            }else{
                                value.add(kv.getValue().get(0)); // 否则默认取第一个
                                kv.setValue(value);
                            }
                        }
                    });
        }));
    }
}
```

1. 根据下图可以看到，在取得返回值后，Filter的Order 值越大，越先处理Response，而真正将Response返回到前端的，是 `NettyWriteResponseFilter`，要想在它之前修改Response，则Order 的值必须比`NettyWriteResponseFilter.WRITE_RESPONSE_FILTER_ORDER` 大。

![spring-cloud-gateway-fliter-order](https://cdn.nlark.com/yuque/0/2022/png/396745/1646563570116-0d5baeb7-da59-4aef-b138-2666fd0deb40.png#clientId=ud2938b50-0475-4&from=paste&id=uf2e029cf&originHeight=1091&originWidth=768&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u6af8cdf7-24bd-40ca-a339-3a7383a60ed&title=spring-cloud-gateway-fliter-order "spring-cloud-gateway-fliter-order")

2. 修改后置filter时，网上有些博客使用的是 `Mono.defer`去做的，这种做法，会从此`filter`开始，重新执行一遍它后面的其他`filter`，一般会添加一些认证或鉴权的 `GlobalFilter `，就需要在这些filter里用`ServerWebExchangeUtils.isAlreadyRouted(exchange)` 方法去判断是否重复执行，否则可能会执行二次重复操作，所以建议使用fromRunnable 避免这种情况。
