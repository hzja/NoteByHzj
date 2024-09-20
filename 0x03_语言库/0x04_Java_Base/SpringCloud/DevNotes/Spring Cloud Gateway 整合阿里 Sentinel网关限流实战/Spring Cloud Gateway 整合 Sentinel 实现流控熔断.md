JavaSpringCloudGateWaySentinel
<a name="tpjog"></a>
## 一、什么是网关限流
在微服务架构中，网关层可以屏蔽外部服务直接对内部服务进行调用，对内部服务起到隔离保护的作用，网关限流，顾名思义，就是通过网关层对服务进行限流，从而达到保护后端服务的作用。<br />Sentinel 从 1.6.0 版本开始就提供了 Spring Cloud Gateway 的适配，可以提供两种资源维度的限流：

- **route维度： 即在配置文件中配置的路由条目，资源名为对应的 routeId，这种属于粗粒度的限流，一般是对某个微服务进行限流。**
- **自定义API维度： 用户可以利用 Sentinel 提供的API来自定义一些API分组，这种属于细粒度的限流，针对某一类的uri进行匹配限流，可以跨多个微服务。**
<a name="TMVQd"></a>
## 二、GateWay 整合 Sentinel 实现网关限流
那么接下来就介绍下 spring cloud gateway 如何整合 sentinel<br />1、添加 sentinel 相关依赖：
```xml
<!-- 引入sentinel进行服务降级熔断 -->
<dependency>
  <groupId>com.alibaba.cloud</groupId>
  <artifactId>spring-cloud-starter-alibaba-sentinel</artifactId>
</dependency>
<!-- gateway网关整合sentinel进行限流降级 -->
<dependency>
  <groupId>com.alibaba.cloud</groupId>
  <artifactId>spring-cloud-alibaba-sentinel-gateway</artifactId>
</dependency>
```
2、配置文件中添加 sentinel 控制台的配置：
```
# sentinel看板相关配置
spring.cloud.sentinel.eager = true
spring.cloud.sentinel.transport.dashboard = 172.28.190.101:8999
```
3、启动网关项目：<br />对于网关项目，需要在原启动参数的基础上添加如下启动参数，标记该应用为 API Gateway 类型：
```bash
# 注：通过 Spring Cloud Alibaba Sentinel 自动接入的 API Gateway 整合则无需此参数
-Dcsp.sentinel.app.type=1
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660911792926-9f006223-6c70-40e7-9fa4-f18c95c7fabd.png#clientId=ua413ca1c-922e-4&from=paste&id=u4b916160&originHeight=150&originWidth=706&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u94a14a4d-e6fb-4cc2-8e1a-c91ab6247b0&title=)<br />4、访问 sentinel 控制台：<br />至此，就已经将 Spring Cloud Gateway 与 Sentinel 整合好了，进入 sentinel 控制台就能够看到网关项目被监控了<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660911792978-fea05920-ff4c-45d1-acf7-ca97bfc355ba.png#clientId=ua413ca1c-922e-4&from=paste&id=u87e9f1d2&originHeight=498&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7ee714b3-90c5-4e24-a5c4-e2744d3b3e8&title=)
<a name="c5ZIq"></a>
## 三、sentinel 网关流控规则的介绍
整合好 Spring Cloud Gateway 与 Sentinel 之后，接下来介绍下如何基于 sentinel-dashboard 控制台进行网关流控，如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660911792941-7fedebb3-921f-446c-834c-de4d7c02233d.png#clientId=ua413ca1c-922e-4&from=paste&id=u9da8c975&originHeight=510&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1e6875d9-2aae-494e-bb14-f765e6436ef&title=)
<a name="HKPtx"></a>
### **3.1、网关流控规则：**
网关流控规则 `GatewayFlowRule` 的核心属性如下：<br />**① **`**resourceMode**`：规则是针对 API Gateway 的 route（`RESOURCE_MODE_ROUTE_ID`）还是用户在 Sentinel 中定义的 API 分组（`RESOURCE_MODE_CUSTOM_API_NAME`），默认是 route。<br />**② **`**resource**`：资源名称，可以是网关中的 route 名称或者用户自定义的 API 分组名称。<br />**③ **`**grade**`：限流指标维度，同限流规则的 grade 字段<br />**④ **`**count**`：限流阈值<br />**⑤ **`**intervalSec**`：统计时间窗口，单位是秒，默认是 1 秒<br />**⑥ **`**controlBehavior**`：流量整形的控制效果，目前支持快速失败和匀速排队两种模式，默认是快速失败。<br />**⑦ **`**burst**`：应对突发请求时额外允许的请求数目。<br />**⑧ **`**maxQueueingTimeoutMs**`：匀速排队模式下的最长排队时间，单位是毫秒，仅在匀速排队模式下生效。<br />**⑨ **`**paramItem**`：参数限流配置。若不提供，则代表不针对参数进行限流，该网关规则将会被转换成普通流控规则；否则会转换成热点规则。其中的字段：

- `**parseStrategy**`**：从请求中提取参数的策略，目前支持提取来源 IP（**`**PARAM_PARSE_STRATEGY_CLIENT_IP**`**）、Host（**`**PARAM_PARSE_STRATEGY_HOST**`**）、任意 Header（**`**PARAM_PARSE_STRATEGY_HEADER**`**）和任意 URL 参数（**`**PARAM_PARSE_STRATEGY_URL_PARAM**`**）四种模式。**
- `**fieldName**`**：若提取策略选择 Header 模式或 URL 参数模式，则需要指定对应的 header 名称或 URL 参数名称。**
- `**pattern**`**：参数值的匹配模式，只有匹配该模式的请求属性值会纳入统计和流控；若为空则统计该请求属性的所有值。**
- `**matchStrategy**`**：参数值的匹配策略，目前支持精确匹配（**`**PARAM_MATCH_STRATEGY_EXACT**`**）、子串匹配（**`**PARAM_MATCH_STRATEGY_CONTAINS**`**）和正则匹配（**`**PARAM_MATCH_STRATEGY_REGEX**`**）。**
<a name="gmELL"></a>
### **3.2、API 分组管理：**
API 分组就是对接口进行分组，然后对不同组的接口实施不同的限流策略。
<a name="MUh0E"></a>
#### （1）添加API分组：
如下图，按指定步骤进入自定义API界面：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660911792969-91e1d1d5-6c2a-408e-aff6-9dd8974c631e.png#clientId=ua413ca1c-922e-4&from=paste&id=ucac3c072&originHeight=509&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc9f13508-8c56-41e2-84a5-01ddae9839e&title=)API 分组有三种配置模式：精确、前缀和正则三种模式。

- **精确模式： 指对URL的路径完全匹配时，进行限流。例如，匹配串配置为 /order/1**
- **前缀模式： 指对URL的路径前缀匹配时，进行限流。例如，匹配串配置为 /order/***
- **正则模式： 指对URL的路径符合正则表示式规则时，进行限流。例如，匹配串配置为 \/order\/\d***

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660911792957-7f88740a-5916-4e9e-91e3-897d3bad6cd0.png#clientId=ua413ca1c-922e-4&from=paste&id=udc68b57c&originHeight=198&originWidth=865&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4be0c467-2894-4519-91bd-218167fd10b&title=)
<a name="KA6QN"></a>
#### （2）配置限流规则：
接下来需要对这个API分组添加流控规则，API名称可以选择不同的API分组进行配置，如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660911793238-9f9a2c73-f88f-49d5-acb4-edccd194ac58.png#clientId=ua413ca1c-922e-4&from=paste&id=udaacb535&originHeight=467&originWidth=664&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2bb447ec-1b07-487a-8efb-ee65707aeaf&title=)<br />新增之后，限流规则就会对符合匹配模式的 API 生效了。
<a name="I4GHH"></a>
## 四、Sentinel 网关流控实现的原理
知道如何使用 sentinel-dashboard 对网关进行流控之后，接下来介绍 sentinel 网关流控的实现原理。<br />当通过 `GatewayRuleManager` 加载网关流控规则（`GatewayFlowRule`）时，无论是否针对请求属性进行限流，Sentinel 底层都会将网关流控规则转化为热点参数规则（`ParamFlowRule`），存储在 `GatewayRuleManager` 中，与正常的热点参数规则相隔离。转换时 Sentinel 会根据请求属性配置，为网关流控规则设置参数索引（idx），并同步到生成的热点参数规则中。<br />外部请求进入 API Gateway 时会经过 Sentinel 实现的 filter，其中会依次进行 “路由/API 分组匹配 -> 请求属性解析 和 参数组装"。Sentinel 会根据配置的网关流控规则来解析请求属性，并依照参数索引顺序组装参数数组，最终传入 `SphU.entry(res, args)` 中。<br />Sentinel API Gateway Adapter Common 模块向 Slot Chain 中添加了一个 `GatewayFlowSlot`，专门用来做网关规则的检查。`GatewayFlowSlot` 会从 `GatewayRuleManager` 中提取生成的热点参数规则，根据传入的参数依次进行规则检查。若某条规则不针对请求属性，则会在参数最后一个位置置入预设的常量，达到普通流控的效果。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660911793279-3aee030a-920e-4733-ad7c-57ab98bff832.png#clientId=ua413ca1c-922e-4&from=paste&id=u443a4e7e&originHeight=372&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc9766550-4d7a-4262-9df2-fed477dd40a&title=)
<a name="QnqLi"></a>
## 五、网关限流了，服务就安全了吗？
如果已经在网关层面做了限流，那么躲在身后的服务是否就安全了呢？答案是否定的，微服务架构中，一个独立服务往往会被多方调用，如下图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660911793278-6914a6bf-b77a-43b4-8bff-197b36cbf627.png#clientId=ua413ca1c-922e-4&from=paste&id=u5f513bcc&originHeight=453&originWidth=766&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u419bf94f-2fee-4c34-861b-de5aa1f1571&title=)商品服务不仅仅被网关层调用，还被内部订单服务调用，这时候如果仅仅在网关层限流，一旦大量的请求订单服务，比如大促秒杀，商品服务不做限流还是会被瞬间击垮。<br />因此需要根据公司业务场景对自己负责的服务也要进行限流兜底，最常见的方案：网关层集群限流+内部服务的单机限流兜底，这样才能保证不被流量冲垮。
<a name="mTf2e"></a>
## 六、自定义流控异常消息
网关流控的默认异常返回信息并不够人性化，直接返回：“Block.........”，这种肯定是不能接受的，那么如何自定义配置流控异常信息呢？<br />其实 sentinel 已经实现了自定义流控异常的返回内容。只需要在配置文件中添加配置如下：
```yaml
spring:
  cloud:
    sentinel:
      #配置限流之后的响应内容
      scg:  
        fallback:
          # 两种模式：一种是response返回文字提示信息，一种是redirect，重定向跳转，需要同时配置redirect(跳转的uri)
          mode: response
          # 响应的状态
          response-status: 426
          # 响应体
          response-body: '{"code": 426,"message": "限流了，稍后重试！"}'
```
上述配置中 mode 配置的是 response，一旦被限流了，将会返回 JSON 串。
```json
{
  "code": 426,
  "message": "限流了，稍后重试！"
}
```
重定向的配置如下：
```yaml
spring:
  cloud:
    sentinel:
      #配置限流之后的响应内容
      scg:
        fallback:
          ## 两种模式，一种是response返回文字提示信息，一种是redirect，重定向跳转，需要同时配置redirect(跳转的uri)
          mode: redirect
          ## 跳转的URL
          redirect: http://www.baidu.com
```
一旦被限流，将会直接跳转到：http://www.baidu.com
