JavaSpringCloudGateWay
<a name="x9hSw"></a>
## 1、介绍
Alibaba Sentinel 支持对 Spring Cloud Gateway、Netflix Zuul 等主流的 API Gateway 进行限流与熔断配置。<br />本文将介绍如何在 Spring Cloud Gateway 中使用 Alibaba Sentinel 进行限流配置，从而代替 Hystrix.
<a name="w3Ijg"></a>
## 2、集成步骤
<a name="BbPm7"></a>
### 2.1. 首先需在Gateway网关模块引入以下依赖配置
（以 Maven 为例）：<br />方案一：只需引入 sentinel-spring-cloud-gateway-adapter 无需引入 spring-cloud-starter-alibaba-sentinel
```xml
<dependency>
	<groupId>com.alibaba.csp</groupId>
	<artifactId>sentinel-spring-cloud-gateway-adapter</artifactId>
	<version>x.y.z</version>
</dependency>
```
方案二：如果需要在Sentinel 控制台管理 网关模块的限流，那么推荐这种配置方式<br />如果想引入 spring-cloud-starter-alibaba-sentinel 那么就把上面那个 adapter 替换成 spring-cloud-alibaba-sentinel-gateway
```xml
<!-- 没添加该依赖的话不能从application配置文件配置sentinel -->
<dependency>
	<groupId>com.alibaba.cloud</groupId>
	<artifactId>spring-cloud-starter-alibaba-sentinel</artifactId>
</dependency>
<dependency>
	<groupId>com.alibaba.cloud</groupId>
	<artifactId>spring-cloud-alibaba-sentinel-gateway</artifactId>
</dependency>
```
<a name="MdPV0"></a>
### 2.2 然后在使用时只需注入对应的 `SentinelGatewayFilter` 实例以及 `SentinelGatewayBlockExceptionHandler` 实例即可。
```java
@Configuration
public class GatewayConfiguration {
	
	private final List<ViewResolver> viewResolvers;
	private final ServerCodecConfigurer serverCodecConfigurer;
	
	public GatewayConfiguration(ObjectProvider<List<ViewResolver>> viewResolversProvider,
								ServerCodecConfigurer serverCodecConfigurer) {
		this.viewResolvers = viewResolversProvider.getIfAvailable(Collections::emptyList);
		this.serverCodecConfigurer = serverCodecConfigurer;
	}
	
	@Bean
	@Order(Ordered.HIGHEST_PRECEDENCE)
	public SentinelGatewayBlockExceptionHandler sentinelGatewayBlockExceptionHandler() {
		// Register the block exception handler for Spring Cloud Gateway.
		return new SentinelGatewayBlockExceptionHandler(viewResolvers, serverCodecConfigurer);
	}
	
	@Bean
	@Order(Ordered.HIGHEST_PRECEDENCE)
	public GlobalFilter sentinelGatewayFilter() {
		return new SentinelGatewayFilter();
	}
}
```
<a name="fZw7m"></a>
### 2.3 现在可以选择在控制台中配置Sentinel限流规则 或者选择直接在Java代码中配置限流规则
<a name="KeFLp"></a>
#### 2.3.1. 如果选择使用 Sentinel 控制台配置限流-熔断规则，那么还需要在 gateway 的配置文件中做出如下配置：
```yaml
spring:
  cloud:
    # Sentinel 控制台连接配置
    sentinel:
      transport:
       # 当前服务与控制台交互的端口号,默认为8719,同一个机器上若有多个应用于控制台交互时需要设置成不同的端口
        port: 8739
        dashboard: 10.1.3.77:9090
      # 服务启动时直接建立心跳连接
      eager: true
      # Sentinel 储存规则的数据源配置(我这里使用的是Nacos来存储Sentinel的限流规则)
      datasource:
        ds:
          nacos:
            # Nacos 服务地址（可配置单点，或者集群的VIP地址）
            server-addr: 10.1.3.76:8848
            dataId: ${spring.application.name}-sentinel
            groupId: DEFAULT_GROUP
            namespace: sms-dev
            rule-type: flow
# Sentinel 控制台鉴权配置
sentinel:
  dashboard:
    auth:
      username: sentinel
      password: sentinel
```
此时就可以在Sentinel控制台页面中配置限流规则，或者在 Nacos 配置中心中添加限流配置文件sms-gateway-sentinel
```json
[
	{
		"resource": "api-service",
		"limitApp": "default",
		"grade": 1,
		"count": 2,
		"strategy": 0,
		"controlBehavior": 0,
		"clusterMode": false
	},
	{
		"resource": "url-proxy-1",
		"limitApp": "default",
		"grade": 1,
		"count": 2,
		"strategy": 0,
		"controlBehavior": 0,
		"clusterMode": false
	}
]
```
<a name="P8TDg"></a>
#### 2.3.2 如果选择在Java代码中配置限流规则
下面是一个详细的配置示例
```java
import com.alibaba.csp.sentinel.adapter.gateway.common.SentinelGatewayConstants;
import com.alibaba.csp.sentinel.adapter.gateway.common.api.ApiDefinition;
import com.alibaba.csp.sentinel.adapter.gateway.common.api.ApiPathPredicateItem;
import com.alibaba.csp.sentinel.adapter.gateway.common.api.ApiPredicateItem;
import com.alibaba.csp.sentinel.adapter.gateway.common.api.GatewayApiDefinitionManager;
import com.alibaba.csp.sentinel.adapter.gateway.common.rule.GatewayFlowRule;
import com.alibaba.csp.sentinel.adapter.gateway.common.rule.GatewayRuleManager;
import com.alibaba.csp.sentinel.adapter.gateway.sc.SentinelGatewayFilter;
import com.alibaba.csp.sentinel.adapter.gateway.sc.callback.BlockRequestHandler;
import com.alibaba.csp.sentinel.adapter.gateway.sc.callback.GatewayCallbackManager;
import com.alibaba.csp.sentinel.adapter.gateway.sc.exception.SentinelGatewayBlockExceptionHandler;
import com.alibaba.csp.sentinel.slots.block.RuleConstant;
import com.alibaba.csp.sentinel.slots.block.degrade.DegradeRule;
import com.alibaba.csp.sentinel.slots.block.degrade.DegradeRuleManager;
import org.springframework.beans.factory.ObjectProvider;
import org.springframework.cloud.gateway.filter.GlobalFilter;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.core.Ordered;
import org.springframework.core.annotation.Order;
import org.springframework.http.HttpStatus;
import org.springframework.http.MediaType;
import org.springframework.http.codec.ServerCodecConfigurer;
import org.springframework.web.reactive.function.BodyInserters;
import org.springframework.web.reactive.function.server.ServerResponse;
import org.springframework.web.reactive.result.view.ViewResolver;
import org.springframework.web.server.ServerWebExchange;
import reactor.core.publisher.Mono;

import javax.annotation.PostConstruct;
import java.util.*;

/**
* @description: 网关限流配置
* @create: 2020-08-26 12:23
**/
@Configuration
public class GatewaySentinelConfiguration {
	private final List<ViewResolver> viewResolvers;
	private final ServerCodecConfigurer serverCodecConfigurer;
	
	public GatewaySentinelConfiguration(ObjectProvider<List<ViewResolver>> viewResolversProvider,
										ServerCodecConfigurer serverCodecConfigurer) {
		this.viewResolvers = viewResolversProvider.getIfAvailable(Collections::emptyList);
		this.serverCodecConfigurer = serverCodecConfigurer;
	}
	
	
	@Bean
	@Order(Ordered.HIGHEST_PRECEDENCE)
	public SentinelGatewayBlockExceptionHandler sentinelGatewayBlockExceptionHandler() {
		// Register the block exception handler for Spring Cloud Gateway.
		return new SentinelGatewayBlockExceptionHandler(viewResolvers, serverCodecConfigurer);
	}
	
	@Bean
	@Order(Ordered.HIGHEST_PRECEDENCE)
	public GlobalFilter sentinelGatewayFilter() {
		return new SentinelGatewayFilter();
	}
	
	/**
	* Spring 容器初始化的时候执行该方法
	*/
	@PostConstruct
	public void doInit() {
		// 加载网关限流规则
		initGatewayRules();
		// 加载自定义限流异常处理器
		initBlockHandler();
	}
	
	/**
	* 网关限流规则
	* 建议直接在 Sentinel 控制台上配置
	*/
	private void initGatewayRules() {
		Set<GatewayFlowRule> rules = new HashSet<>();
		/*
		resource：资源名称，可以是网关中的 route 名称或者用户自定义的 API 分组名称
		count：限流阈值
		intervalSec：统计时间窗口，单位是秒，默认是 1 秒
		*/
		// rules.add(new GatewayFlowRule("order-service")
		//         .setCount(3) // 限流阈值
		//         .setIntervalSec(60)); // 统计时间窗口，单位是秒，默认是 1 秒
		// --------------------限流分组----------start----------
		rules.add(new GatewayFlowRule("url-proxy-1")
				  .setCount(1) // 限流阈值
				  .setIntervalSec(60)); // 统计时间窗口，单位是秒，默认是 1 秒
		rules.add(new GatewayFlowRule("api-service")
				  .setCount(5) // 限流阈值
				  .setIntervalSec(60)); // 统计时间窗口，单位是秒，默认是 1 秒
		// --------------------限流分组-----------end-----------
		
		// 加载网关限流规则
		GatewayRuleManager.loadRules(rules);
		// 加载限流分组
		initCustomizedApis();
		
		// ---------------熔断-降级配置-------------
		DegradeRule degradeRule = new DegradeRule("api-service") // 资源名称
			.setGrade(RuleConstant.DEGRADE_GRADE_EXCEPTION_RATIO) // 异常比率模式(秒级)
			.setCount(0.5) // 异常比率阈值(50%)
			.setTimeWindow(10); // 熔断降级时间(10s)
		// 加载规则.
		DegradeRuleManager.loadRules(Collections.singletonList(degradeRule));
	}
	
	/**
	* 自定义限流异常处理器
	*/
	private void initBlockHandler() {
		BlockRequestHandler blockRequestHandler = new BlockRequestHandler() {
			@Override
			public Mono<ServerResponse> handleRequest(ServerWebExchange serverWebExchange, Throwable throwable) {
				Map<String, String> result = new HashMap<>(3);
				result.put("code", String.valueOf(HttpStatus.TOO_MANY_REQUESTS.value()));
				result.put("message", HttpStatus.TOO_MANY_REQUESTS.getReasonPhrase());
				result.put("x","xx");
				return ServerResponse.status(HttpStatus.TOO_MANY_REQUESTS)
					.contentType(MediaType.APPLICATION_JSON)
					//                        .body(BodyInserters.fromValue(result));
					.body(BodyInserters.fromObject(result));
			}
		};
		
		// 加载自定义限流异常处理器
		GatewayCallbackManager.setBlockHandler(blockRequestHandler);
	}
	
	/**
	* 分组限流
	*/
	private void initCustomizedApis() {
		//demo
		Set<ApiDefinition> definitions = new HashSet<>();
		// product-api 组
		ApiDefinition api1 = new ApiDefinition("product-api")
			.setPredicateItems(new HashSet<ApiPredicateItem>() {{
				// 匹配 /product-service/product 以及其子路径的所有请求
				add(new ApiPathPredicateItem().setPattern("/product-service/product/**")
					.setMatchStrategy(SentinelGatewayConstants.URL_MATCH_STRATEGY_PREFIX));
			}});
		
		// order-api 组
		ApiDefinition api2 = new ApiDefinition("order-api")
			.setPredicateItems(new HashSet<ApiPredicateItem>() {{
				// 只匹配 /order-service/order/index
				add(new ApiPathPredicateItem().setPattern("/order-service/order/index"));
			}});
		definitions.add(api1);
		definitions.add(api2);
		// 加载限流分组
		GatewayApiDefinitionManager.loadApiDefinitions(definitions);
	}
}
```
Tips：这种配置方式只适合固定的限流规则配置，如果需要灵活变动，那么建议使用上面那种方式
<a name="zbFiV"></a>
## 3、总结
如果 Gateway 用 Sentinel

- 建议在 Sentinel 控制台对 网关模块 进行具体的限流，熔断降级配置。
- 否则还是推荐直接用 Gateway 内置的 `RequestRateLimiter` 跟 `Hystrix` 进行熔断限流配置。

还可以参考：[https://github.com/alibaba/Sentinel/wiki/Guideline](https://github.com/alibaba/Sentinel/wiki/Guideline)：-从-Hystrix-迁移到-Sentinel<br />从 1.6.0 版本开始，Sentinel 提供了 Spring Cloud Gateway 的适配模块，可以提供两种资源维度的限流：

- **route 维度：** 即在 Spring 配置文件中配置的路由条目，资源名为对应的 routeId* 在网关上用 Sentinel , 那么默认情况下一个 RouteId 对应的服务就是一个资源。
- **GatewayFlowRule：** 网关限流规则，针对 API Gateway 的场景定制的限流规则，可以针对不同 route 或自定义的 API 分组进行限流，支持针对请求中的参数、Header、来源 IP 等进行定制化的限流。

**自定义 API 维度：** 用户可以利用 Sentinel 提供的 API 来自定义一些 API 分组<br />ApiDefinition：用户自定义的 API 定义分组，可以看做是一些 URL 匹配的组合。比如可以定义一个 API 叫 my_api，请求 path 模式为 `/foo/**` 和 `/baz/**` 的都归到 my_api 这个 API 分组下面。限流的时候可以针对这个自定义的 API 分组维度进行限流。<br />官方Demo 示例：[https://github.com/alibaba/Sentinel/tree/master/sentinel-demo/sentinel-demo-spring-cloud-gateway](https://github.com/alibaba/Sentinel/tree/master/sentinel-demo/sentinel-demo-spring-cloud-gateway)
