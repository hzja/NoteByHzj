Java Sentinel<br />做业务开发，需要考虑业务的扩展性。做基础框架开发，需要考虑如何让业务方接入，使用简单，尽量不要耦合在业务代码中。 <br />Sentinel里面是如何做到让业务方接入简单，使用方便的呢？来剖析下Sentinel的那些适配是如何实现的。
<a name="jwcDk"></a>
## 基本使用
基本使用可以直接用SphU类对资源进行保护，使用方式如下：
```java
public static void main(String[] args) {
    // 配置规则.
    initFlowRules();
    while (true) {
        // 1.5.0 版本开始可以直接利用 try-with-resources 特性
        try (Entry entry = SphU.entry("HelloWorld")) {
            // 被保护的逻辑
            System.out.println("hello world");
     } catch (BlockException ex) {
            // 处理被流控的逻辑
      System.out.println("blocked!");
     }
    }
}
```
这种方式其实就违背了前面说的耦合问题，跟业务代码耦合在了一起，扩展性不好。
<a name="omzs1"></a>
## 注解使用
可以在基本使用的基础上优化下，不在业务代码中出现SphU相关的代码，比如说定义一个注解在来实现这个功能。<br />使用方式如下：
```java
@SentinelResource("HelloWorld")
public void helloWorld() {
    // 资源中的逻辑
    System.out.println("hello world");
}
```
通过注解和切面就可以将逻辑收拢，不会在散落在各个业务代码中，就算有一天限流方式改成了其他框架，注解都不用变，直接将切面里面的逻辑更新即可。
<a name="mrOCE"></a>
## 适配Dubbo
当需要对Dubbo的接口进行限流时，使用原生的代码方式和注解方式都可以，但是这样就需要在每个调用的地方进行改造，那么能不能做成自动适配的方式，连注解都不用加呢？<br />在Sentinel中有一个sentinel-apache-dubbo-adapter的模块就是专门用于适配dubbo的。原理就是通过Dubbo的Filter机制来实现通用的适配逻辑。<br />有consumer Filter和provider Filter，业务方只需要依赖这个包，就自动适配dubbo了，然后通过Sentinel的控制台进行配置，就可以达到限流和熔断的效果了。
<a name="Tm62V"></a>
## 适配Feign
Dubbo都适配了，怎么能少的了Feign呢，其实原理都是一样，也是通过Feign的`Filter`机制来适配。不过Feign的适配整合放在了Spring Cloud Alibaba中。<br />通过定义`SentinelInvocationHandler`，在`invoke`方法中适配Sentinel的逻辑。详细代码在com.alibaba.cloud.sentinel.feign.SentinelInvocationHandler中。
<a name="syJxf"></a>
## 适配Zuul
对于Zuul的适配同样有一个单独的模块sentinel-zuul-adapter。原理呢还是一样，Zuul也有`Filter`，既然是限流在Zuul中肯定是用pre filter。<br />实现类是com.alibaba.csp.sentinel.adapter.gateway.zuul.filters.SentinelZuulPreFilter，贴一小段代码给大家看下：
```java
public Object run() throws ZuulException {
    RequestContext ctx = RequestContext.getCurrentContext();
    String origin = this.parseOrigin(ctx.getRequest());
    String routeId = (String)ctx.get("proxy");
    Deque<AsyncEntry> asyncEntries = new ArrayDeque();
    String fallBackRoute = routeId;
    try {
        if (StringUtil.isNotBlank(routeId)) {
            ContextUtil.enter("sentinel_gateway_context$$route$$" + routeId, origin);
            this.doSentinelEntry(routeId, 0, ctx, asyncEntries);
        }
        Set<String> matchingApis = this.pickMatchingApiDefinitions(ctx);
        if (!matchingApis.isEmpty() && ContextUtil.getContext() == null) {
            ContextUtil.enter("zuul_default_context", origin);
        }
        Iterator var14 = matchingApis.iterator();
        while(var14.hasNext()) {
            String apiName = (String)var14.next();
            this.doSentinelEntry(apiName, 1, ctx, asyncEntries);
        }
    } catch (BlockException var12) {
        ZuulBlockFallbackProvider zuulBlockFallbackProvider = ZuulBlockFallbackManager.getFallbackProvider(fallBackRoute);
        BlockResponse blockResponse = zuulBlockFallbackProvider.fallbackResponse(fallBackRoute, var12);
        ctx.setRouteHost((URL)null);
        ctx.set("serviceId", (Object)null);
        ctx.setResponseBody(blockResponse.toString());
        ctx.setResponseStatusCode(blockResponse.getCode());
        ctx.getResponse().setContentType("application/json; charset=utf-8");
    } finally {
        if (!asyncEntries.isEmpty()) {
            ctx.put("_sentinel_entries", asyncEntries);
        }
    }
    return null;
}
```
<a name="IEYs6"></a>
## 总结
在开发一个底层框架的时候，需要考虑的问题。这个问题就是使用起来越简单越好，这才是好的框架该有的样子。<br />当然，Sentinel还适配了其他很多的框架，比如httpclient，Spring Cloud Gateway等，底层思想都是相同的，都是利用扩展机制进行统一处理。
