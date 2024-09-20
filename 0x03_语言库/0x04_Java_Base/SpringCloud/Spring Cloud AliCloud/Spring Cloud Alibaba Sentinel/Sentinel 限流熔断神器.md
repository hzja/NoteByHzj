JavaSpringCloud AlibabaSentinel
<a name="cCWhz"></a>
## 一、限流熔断神器 sentinel
<a name="cta0z"></a>
### 1、什么是 sentinel：
在基于 SpringCloud 构建的微服务体系中，服务间的调用链路会随着系统的演进变得越来越长，这无疑会增加了整个系统的不可靠因素。<br />在并发流量比较高的情况下，由于网络调用之间存在一定的超时时间，链路中的某个服务出现宕机都会大大增加整个调用链路的响应时间，而瞬间的流量洪峰则会导致这条链路上所有服务的可用线程资源被打满，从而造成整体服务的不可用，这也就是常说的 “雪崩效应”。<br />而在微服务系统设计的过程中，为了应对这样的糟糕情况，最常用的手段就是进行 ”流量控制“ 以及对网络服务的调用实现“熔断降级”。因此，Sentinel 就因运而生了。<br />Sentinel 是一款面向分布式服务架构的轻量级流量控制组件，主要以流量为切入点，从流量控制、熔断降级、系统自适应保护等多个维度来保障服务的稳定性，核心思想是：**根据对应资源配置的规则来为资源执行相应的流控/降级/系统保护策略**，Sentinel 的主要特性如下图：<br />![2022-08-20-19-18-06.745570800.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1660997475903-c1a174a8-ac47-41e9-9931-ef69ab106697.png#clientId=u7dd39c7a-3298-4&errorMessage=unknown%20error&from=ui&id=u2299aa6f&originHeight=329&originWidth=662&originalType=binary&ratio=1&rotation=0&showTitle=false&size=100297&status=error&style=shadow&taskId=u76093cf7-055c-4a25-ad11-9581abc0046&title=)
<a name="todmr"></a>
### 2、主流限流熔断组件对比：
![2022-08-20-19-18-06.818123600.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1660997475934-ef90b3c3-2356-4620-8474-ca0769dbfd86.png#clientId=u7dd39c7a-3298-4&errorMessage=unknown%20error&from=ui&id=g9dlE&originHeight=526&originWidth=692&originalType=binary&ratio=1&rotation=0&showTitle=false&size=71646&status=error&style=shadow&taskId=u00725aa0-9948-4e7b-9535-a9c06ad6a0a&title=)<br />从三者的对比来看，Hystrix 功能相对较少且官方已宣布停止维护，官方推荐 resilience4j 作为 Hystrix 的替代品。而对比 resilience4j 和 sentinel，可明显看出 sentinel 支持的功能、场景更丰富，灵活性可用性更高。
<a name="J7A3W"></a>
### 3、sentinel-dashboard 控制台的部署与使用说明：
（1）进入官网下载 sentinel-dashboard 的 jar 包：[Releases · alibaba/Sentinel · GitHub](https://github.com/alibaba/Sentinel)<br />（2）使用以下命令直接运行 jar 包（JDK 版本必须≥ 1.8）：
```bash
java -jar -Dserver.port=9999 sentinel-dashboard-1.8.2.jar
```
（3）启动参数说明：

- `**-Dserver.port**`**：指定启动的端口，默认8080**
- `**-Dproject.name**`**：指定本服务的名称**
- `**-Dcsp.sentinel.dashboard.server**`**：指定sentinel控制台的地址，用于将自己注册进入实现监控自己**
- `**-Dsentinel.dashboard.auth.username=sentinel**`** 用于指定控制台的登录用户名为 "sentinel"，默认值为 “sentinel”**
- `**-Dsentinel.dashboard.auth.password=123456**`** 用于指定控制台的登录密码为 "123456"，默认值为 "sentinel"**
- `**-Dserver.servlet.session.timeout=7200**`** 用于指定 Spring Boot 服务端 session 的过期时间，如 7200 表示 7200 秒；60m 表示 60 分钟，默认为 30 分钟，需要注意的是，部署多台控制台时，session 默认不会在各实例之间共享，这一块需要自行改造。**

更多具体启动参数配置如下图：<br />![2022-08-20-19-18-07.043129700.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1660997542797-97baf68a-92ca-441d-84dd-b157931e9883.png#clientId=u7dd39c7a-3298-4&errorMessage=unknown%20error&from=ui&id=uc7189d6d&originHeight=921&originWidth=853&originalType=binary&ratio=1&rotation=0&showTitle=false&size=369392&status=error&style=shadow&taskId=u41c97031-122d-4dc0-9480-9c955df8c10&title=)<br />官方文档地址：[https://github.com/alibaba/Sentinel/wiki/](https://github.com/alibaba/Sentinel/wiki/)<br />（4）控制台访问地址：http://localhost:9999，默认账号密码都为 sentinel/sentinel<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1660999960433-4550740e-448a-4815-b10f-fe95b7eb65e6.png#clientId=u7dd39c7a-3298-4&errorMessage=unknown%20error&from=paste&height=421&id=u8f348477&originHeight=1052&originWidth=1646&originalType=binary&ratio=1&rotation=0&showTitle=false&size=61358&status=error&style=shadow&taskId=u94c5ef2b-5084-4fdb-b5b2-1ca0436bd87&title=&width=658.4)
<a name="oUetf"></a>
## 二、SpringBoot 整合 sentinel-dashboard
接下来介绍下 SpringBoot 集成 sentinel-dashboard 控制台进行规则配置。那为什么要集成 sentinel-dashboard 控制台，sentinel 不是提供了相关的API吗？<br />其实 SpringBoot 官方一直提倡 约定>配置>编码 的规则，能够不硬编码何乐而不为呢？因此本文后续内容主要还是结合 sentinel-dashboard 控制台进行讲解，关于API的使用大家可以按照官方文档学习，讲解的非常清楚，这里就不过多介绍了。
<a name="MA6qJ"></a>
#### 1、引入依赖：
```xml
<dependency>
  <groupId>com.alibaba.cloud</groupId>
  <artifactId>spring-cloud-starter-alibaba-sentinel</artifactId>
</dependency>
```
<a name="BOxwr"></a>
#### 2、添加配置：
```
# sentinel看板的地址
spring.cloud.sentinel.transport.dashboard = localhost:9999
# 开启对sentinel看板的饥饿式加载。sentinel默认是懒加载机制，只有访问过一次的资源才会被监控，通过关闭懒加载，在项目启动时就连接sentinel控制台
spring.cloud.sentinel.eager = true
```
<a name="R6Svn"></a>
#### 3、新建测试接口：
```java
@RestController
@RequestMapping("/sentinel")
public class FlowLimitController {
    @GetMapping("/test")
    public String test(){
        return "接收到一条消息--------";
    }
}
```
<a name="rXZ0w"></a>
#### 4、启动微服务：
启动微服务，然后浏览器输入：http://localhost:9999/sentinel/test，此时查看sentinel控制台，将会看见 sentinel-service 这个服务已经被监控了，如下图：<br />![2022-08-20-19-18-07.464997400.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1660997919517-63f7a356-6539-4be1-a5f4-e87999abb6f8.png#clientId=u7dd39c7a-3298-4&errorMessage=unknown%20error&from=ui&id=u3cd9986f&originHeight=537&originWidth=1621&originalType=binary&ratio=1&rotation=0&showTitle=false&size=181579&status=error&style=shadow&taskId=uc8c8bcbd-eceb-4cdd-956c-06ff2f370e9&title=)
<a name="Ept1t"></a>
## 三、Sentinel-dashboard的使用与规则配置
前面基于 SpringBoot 整合了 Sentinel-dashboard，这一部分就详细介绍下 sentinel-dashboard 控制台的使用。
<a name="pUaBW"></a>
### 1、资源与规则：
Sentinel 可以简单分为 Sentinel 核心库和 Dashboard，核心库不依赖 Dashboard，但是结合 Dashboard 可以获得更好的效果。使用 Sentinel 来进行资源保护，主要分为几个步骤:<br />**（1）定义资源：** 资源可以是程序中的任何内容，例如一个服务，服务里的方法，甚至是一段代码。<br />**（2）定义规则：** Sentinel 支持以下几种规则：流量控制规则、熔断降级规则、系统保护规则、来源访问控制规则 和 热点参数规则。<br />**（3）检验规则是否生效**<br />由于 Sentinel 中所有的规则都可以在动态地查询和修改，并且修改后立即生效，并且 Sentinel 中资源定义和规则的配置是分离的。因此在编码时，先把需要保护的资源定义好（埋点），之后便可以在需要的时候动态配置规则了。也可以理解为，只要有了资源，就能在任何时候灵活地定义各种规则。
<a name="pHnXP"></a>
#### 1.1、定义资源：
对于资源的定义有两种，一种是硬编码的方式，一种是通过 `@SentinelResource` 注解的方式。<br />**（1）硬编码方式(不推荐)：**<br />但由于硬编码的方式对代码的侵入性太强，不推荐使用，所以在下文只简单介绍下了如何使用<br />① 抛出异常的方式定义资源：SphU<br />使用 `SphU.entry("资源名")`定义资源的方式主要用于 try-catch，将需要保护的代码使用 `SphU.entry("资源名")` 和 `entry.exit()` 包围起来，当 catch 到 `BlockException` 时执行异常处理<br />代码示例：
```java
Entry entry = null;
try {
    // 定义一个sentinel保护的资源，
    entry = SphU.entry(resourceName);
    // 被保护的业务逻辑
} catch (BlockException e) {
    // 如果被保护的资源被限流或者降级了，就会抛出BlockException
    log.warn("资源被限流或降级了", e);
} catch (InterruptedException e) {
    log.error("发生InterruptedException",e);
} finally {
    if (entry != null) {
        entry.exit();
    }
}
```
在下面的例子中， 用 try-with-resources 来定义资源。参考代码如下:
```java
public static void main(String[] args) {
    // 配置规则.
    initFlowRules();
 
    // 1.5.0 版本开始可以直接利用 try-with-resources 特性
    try (Entry entry = SphU.entry(resourceName)) {
        // 被保护的逻辑
        System.out.println("hello world");
    } catch (BlockException ex) {
        // 处理被流控的逻辑
        System.out.println("blocked!");
    }
}
```
② 返回布尔值的方式定义资源：<br />使用 `SphO.entry("资源名")` 定义资源的方式主要用于 if-else，当资源发生触发 sentinel 规则之后就会返回 false，这时可以根据返回值，进行限流降级的处理逻辑。<br />示例代码如下：
```java
// 资源名可使用任意有业务语义的字符串
if (SphO.entry("自定义资源名")) {
  // 务必保证finally会被执行
  try {
    /**
    * 被保护的业务逻辑
    */
  } finally {
    SphO.exit();
  }
} else {
  // 资源访问阻止，被限流或被降级
  // 进行相应的处理操作
}
```
注意：`SphO.entry(xxx)` 需要与 `SphO.exit()` 方法成对出现，匹配调用，位置正确，否则会导致调用链记录异常，抛出 ErrorEntryFreeException 异常。<br />**（2）**`**@SentinelResource**`**注解方式(推荐)：**<br />使用 `@SentinelResource` 注解可以更加的灵活的定义资源，如下示例就定义了一个名为 “HelloWorld” 的资源
```java
@SentinelResource("HelloWorld")
public void helloWorld() {
    // 资源中的逻辑
    System.out.println("hello world");
}
```
需要注意的是，`@SentinelResource` 注解用于定义资源埋点，但不支持 private 方法。默认情况下，Sentinel 对控制资源的保护处理是直接抛出异常，这样对用户不友好，所以需要通过可选的异常处理 `blockHandler` 和 `fallback` 配置项处理一下异常信息。`@SentinelResource` 注解包含以下属性：<br />**① **`**value**`**：** 资源名称，必需项<br />**② **`**entryType**`**：** entry 类型，可选项（默认为 `EntryType.OUT`）<br />**③ **`**blockHandler**`**/**`**blockHandlerClass**`**：**`blockHandler` 指定函数负责处理 `BlockException` 异常，可选项。`blockHandler` 函数默认需要和原方法在同一个类中，通过指定 `blockHandlerClass` 为对应类的 Class 对象，则可以指定其他类中的函数，但注意对应的函数必需为 static 函数，否则无法解析。<br />`blockHandler` 函数访问范围需要是 public，返回类型需要与原方法相匹配，参数类型必须和原方法一致并且最后加一个类型为 BlockException 的异常参数。<br />**④ **`**fallback**`**/**`**fallbackClass**`**：** `fallback` 指定的函数负责处理业务运行的异常，可选项，`fallback` 函数可以针对所有类型的异常（除了`exceptionsToIgnore`里面排除掉的异常类型）进行处理。`fallback` 函数默认需要和原方法在同一个类中，通过指定 `fallbackClass` 为对应类的 Class 对象，则可以指定指定为其他类的函数，但注意对应的函数必需为 static 函数，否则无法解析。<br />`fallback` 函数的返回值类型必须与原函数返回值类型一致；方法参数列表需要和原函数一致，或者可以额外多一个 `Throwable` 类型的参数用于接收对应的异常。<br />**⑤ **`**defaultFallback**`**（since 1.6.0）：** 默认的 `fallback` 函数名称，可选项，通常用于通用的 fallback 逻辑。`defaultFallback` 函数默认需要和原方法在同一个类中，通过指定 `fallbackClass` 为对应类的 Class 对象，则可以指定指定为其他类的函数，但注意对应的函数必需为 static 函数，否则无法解析。`defaultFallback` 函数可以针对所有类型的异常（除了 `exceptionsToIgnore` 里面排除掉的异常类型）进行处理。若同时配置了 `fallback`和 `defaultFallback`，则只有 `fallback`会生效。<br />`defaultFallback`函数的返回值类型必须与原函数返回值类型一致；方法参数列表需要为空，或者可以额外多一个 `Throwable` 类型的参数用于接收对应的异常。<br />**⑥ **`**exceptionsToIgnore**`**（since 1.6.0）：** 用于指定哪些异常被排除掉，不会计入异常统计中，也不会进入 `fallback` 逻辑中，而是会原样抛出。<br />特别地，若 `blockHandler` 和 `fallback` 都进行了配置，则被限流降级而抛出 `BlockException` 时只会进入 `blockHandler` 处理逻辑。若未配置 `blockHandler`、`fallback` 和 `defaultFallback`，则被限流降级时会将 `BlockException` 直接抛出（若方法本身未定义 `throws BlockException` 则会被 JVM 包装一层 `UndeclaredThrowableException`）<br />`@SentinelResource` 的 `fallback` 负责业务运行的异常，`blockHandler` 负责 sentinel 配置的违规。
<a name="yewGE"></a>
#### 1.2、定义规则：
Sentinel 支持以下几种规则：流量控制规则、熔断降级规则、系统保护规则、来源访问控制规则 和 热点参数规则。而定义规则同样支持两种方式：一种是硬编码的方式，一种是通过 sentinel-dashboard 控制台直接配置，这部分在下文会继续介绍。
<a name="qFoea"></a>
### 2、流控规则：
<a name="WEzPj"></a>
#### 2.1、流控规则的核心属性：
流量控制，原理是监控应用流量的 QPS 或 并发线程数 等指标，当达到指定阈值时对流量进行控制，避免系统被瞬时的流量高峰冲垮，保障应用高可用性。同一个资源可以创建多条限流规则，一条限流规则由以下属性组成：<br />**① **`**resource**`**：** 资源名，即限流规则的作用对象，默认请求路径。<br />**② **`**limitApp**`**：** 流控针对的调用来源，若为 default 则不区分调用来源，默认值default<br />**③ **`**count**`**：** 限流阈值<br />**④ **`**grade**`**：** 限流阈值类型（1代表 QPS，0 代表并发线程数），默认值QPS<br />**⑤ **`**strategy**`**：** 流控模式

- **直接拒绝（默认）： 接口达到限流条件时，直接限流**
- **关联： 当关联的资源达到阈值时，就限流自己（适合做应用让步）**
- **链路： 只记录指定链路上的流量，指定资源从入口资源进来的流量，如果达到阈值，就可以限流**

**⑥ **`**controlBehavior**`**：** 流控效果

- **快速失败（默认）： 当 QPS 超过任意规则的阈值后，新的请求就会被立即拒绝，拒绝方式为抛出**`**FlowException**`
- **排队等待： 这种方式严格控制了请求通过的间隔时间，也即是让请求以均匀的速度通过，对应的是漏桶算法。**
- **Warm Up： 该方式主要用于系统长期处于低水位的情况下，当流量突然增加时，直接把系统拉升到高水位可能瞬间把系统压垮。通过"冷启动"，让通过的流量缓慢增加，在一定时间内逐渐增加到阈值上限，给冷系统一个预热的时间，避免冷系统被压垮的情况。预热底层是根据令牌桶算法实现的。**

以上几种属性在 sentinel-dashboard 控制台对应的规则如下图：<br />![2022-08-20-19-18-07.740205300.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1660998346959-1435750d-5d27-434b-a673-2951871ef108.png#clientId=u7dd39c7a-3298-4&errorMessage=unknown%20error&from=ui&id=u82f4ebfc&originHeight=489&originWidth=677&originalType=binary&ratio=1&rotation=0&showTitle=false&size=112700&status=error&style=shadow&taskId=u5b484ed2-865a-46a9-ab83-9963817df6f&title=)
<a name="Wfd0f"></a>
#### 2.2、阈值类型：
**（1）基于 QPS 的流控：**<br />QPS，每秒请求数，即在不断向服务器发送请求的情况下，服务器每秒能够处理的请求数量。<br />**（2）基于并发线程数的流控：**<br />并发数控制用于保护业务线程池不被慢调用耗尽。例如，当应用所依赖的下游应用由于某种原因导致服务不稳定、响应延迟增加，对于调用者来说，意味着吞吐量下降和更多的线程数占用，极端情况下甚至导致线程池耗尽。为应对太多线程占用的情况，业内有使用隔离的方案，比如：

- **通过不同业务逻辑使用不同线程池来隔离业务自身之间的资源争抢（线程池隔离），这种隔离方案虽然隔离性比较好，但是代价就是线程数目太多，线程上下文切换的 overhead 比较大，特别是对低延时的调用有比较大的影响。**
- **使用信号量来控制同时请求的个数（信号量隔离），这种隔离方案虽然能够控制线程数量，但无法控制请求排队时间，当请求过多时排队也是无益的，直接拒绝能够迅速降低系统压力。**

Sentinel 并发线程数限流不负责创建和管理线程池，而是简单统计当前请求上下文的线程个数（正在执行的调用数目），如果超出阈值，新的请求会被立即拒绝，效果类似于信号量隔离。并发数控制通常在调用端进行配置。
<a name="rfuIj"></a>
#### 2.3、流控效果：
当系统的流量超过设定的阈值时，sentinel 则采取措施进行流量控制，流控效果总共分为三种：**快速失败、Warm Up、排队等待**。对应的 FlowRule 中的 `controlBehavior` 字段。

- **快速失败（默认）： 当 QPS 超过任意规则的阈值后，新的请求就会被立即拒绝，拒绝方式为抛出**`**FlowException**`
- **排队等待： 这种方式严格控制了请求通过的间隔时间，也即是让请求以均匀的速度通过，对应的是漏桶算法。**
- **Warm Up： 该方式主要用于系统长期处于低水位的情况下，当流量突然增加时，直接把系统拉升到高水位可能瞬间把系统压垮。通过"冷启动"，让通过的流量缓慢增加，在一定时间内逐渐增加到阈值上限，给冷系统一个预热的时间，避免冷系统被压垮的情况。预热底层是根据令牌桶算法实现的。**

注意：若使用除了快速失败之外的流量控制效果，则调用关系限流策略（`strategy`）会被忽略。<br />**（1）快速失败：**<br />默认的流量控制方式，当 QPS 超过任意规则的阈值后，新的请求就会被立即拒绝，拒绝方式为抛出`FlowException`。这种方式适用于对系统处理能力确切已知的情况下，比如通过压测确定了系统的准确水位时<br />**（2）Warm Up：**<br />注意：该方式只针对 QPS 流控，对并发线程数流控不支持<br />即预热/冷启动方式，该方式主要用于系统长期处于低水位的情况下，当流量突然增加时，直接把系统拉升到高水位可能瞬间把系统压垮。通过"冷启动"，让通过的流量缓慢增加，在一定时间内逐渐增加到阈值上限，给冷系统一个预热的时间，避免冷系统被压垮的情况。<br />预热底层是根据令牌桶算法实现的，源码对应得类在 `com.alibaba.csp.sentinel.slots.block.flow.controller.WarmUpController` 中，算法中有一个冷却因子`coldFactor`，默认值是3，即请求 QPS 从 `threshold`(阈值) / 3 开始，经预热时长逐渐升至设定的 QPS 阈值。<br />比如通过 sentinel-dashboard 设定 `testWarmUP` 资源的 QPS 阈值为，流控效果为 warm up，预热时长为5秒，如下图所示，`testWarmUP` 资源刚开始限流的阈值为 20/3=7，但经过10秒的预热后，慢慢将阈值升至20。<br />![2022-08-20-19-18-07.924926500.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1660998455217-734d692b-e3ed-4b38-a517-dca6cc9ae4b1.png#clientId=u7dd39c7a-3298-4&errorMessage=unknown%20error&from=ui&id=ufc6aa50b&originHeight=392&originWidth=508&originalType=binary&ratio=1&rotation=0&showTitle=false&size=36855&status=error&style=shadow&taskId=u4baecd51-ea33-4699-9a7a-f986914f2da&title=)<br />通常冷启动的过程系统允许通过的 QPS 曲线如下图所示：<br />![2022-08-20-19-18-08.257561300.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1660998455258-2fd17304-c3cf-40b3-88ab-72af5bde96b0.png#clientId=u7dd39c7a-3298-4&errorMessage=unknown%20error&from=ui&id=hJHE8&originHeight=479&originWidth=576&originalType=binary&ratio=1&rotation=0&showTitle=false&size=77496&status=error&style=shadow&taskId=u6a70a5de-6ae9-4e21-9e55-a72e0dac90b&title=)<br />比如秒杀系统在开启瞬间，会有很多流量上来，很可能把系统打死，预热方式就是为了保护系统，可慢慢的把流量放进来，慢慢的把阈值增长到设置的阈值。<br />**（3）排队等待：**<br />注意：这一效果只针对QPS流控，并发线程数流控不支持。<br />排队等待的方式会以匀速排队方式严格控制请求通过的间隔时间，也就是让请求以均匀的速度通过，其余的排队等待，它还会让设置一个超时时间，当请求超过超时间时间还未处理，则会被丢弃。<br />对应的是漏桶算法，源码对应得类：`com.alibaba.csp.sentinel.slots.block.flow.controller.RateLimiterController`。该方式的作用如下图：<br />![2022-08-20-19-18-08.303579500.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1660998493571-038d4547-9444-477a-a572-06443bd30f29.png#clientId=u7dd39c7a-3298-4&errorMessage=unknown%20error&from=ui&id=u49434390&originHeight=245&originWidth=655&originalType=binary&ratio=1&rotation=0&showTitle=false&size=62093&status=error&style=shadow&taskId=u059137be-7d53-4dc3-bb77-32697bc325d&title=)<br />该方式主要用于处理间隔性突发的流量。假设某时刻来了大流量的请求，如果此时要处理所有请求，很可能会导致系统负载过高，影响稳定性。<br />但其实接下来几秒可能系统处于空闲状态，若直接把多余的请求丢弃则没有充分利用系统的处理能力，所以希望系统能够在接下来的空闲期间逐渐处理这些请求，而不是在第一秒直接拒绝多余的请求。<br />Sentinel 的 Rate Limiter 模式能在某一段时间间隔内以匀速方式处理这样的请求, 充分利用系统的处理能力, 也就是削峰填谷, 保证资源的稳定性。<br />Sentinel会以固定的间隔时间让请求通过, 访问资源，当请求到来时，如果当前请求距离上个通过的请求通过的时间间隔不小于预设值，则让当前请求通过；否则，计算当前请求的预期通过时间，如果该请求的预期通过时间大于规则预设的 timeout 时间，则该请求会等待直到预设时间到来通过；反之，则马上抛出阻塞异常。<br />下图能很形象的展示这种场景的削峰填谷的作用：X轴代表时间，Y轴代表系统处理的请求<br />![2022-08-20-19-18-08.474539200.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1660998687390-61b146df-9835-48c6-90f1-084da5d749a2.png#clientId=u7dd39c7a-3298-4&errorMessage=unknown%20error&from=ui&id=u48928ed0&originHeight=325&originWidth=552&originalType=binary&ratio=1&rotation=0&showTitle=false&size=719163&status=error&style=shadow&taskId=uadb48ff0-14af-4e8b-926d-04cf3e14cb4&title=)<br />比如通过 sentinel-dashboard 对 service 资源设置限流阈值为10，流控效果为排队等候，每秒10次请求时，再有请求就排队等候，等待超时时间为 10000ms，超时过后，请求将被踢出排队队列，返回限流异常。<br />![2022-08-20-19-18-09.053098400.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1660998687660-c80044ef-e1c7-43ef-aec7-1cf2314e2bdf.png#clientId=u7dd39c7a-3298-4&errorMessage=unknown%20error&from=ui&id=B7QAX&originHeight=588&originWidth=706&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1663858&status=error&style=shadow&taskId=u0c5e4cd5-e42a-494a-8cb0-1873de87bfc&title=)
<a name="X8FkP"></a>
#### 2.4、流控模式：
调用关系包括调用方、被调用方；一个方法又可能会调用其它方法，形成一个调用链路的层次关系。Sentinel 通过 `NodeSelectorSlot` 建立不同资源间的调用的关系，并且通过 `ClusterBuilderSlot` 记录每个资源的实时统计信息。有了调用链路的统计信息，可以衍生出多种流量控制模式，总共分为三种，对应属性为 `strategy`：

- **直接拒绝（默认）： 接口达到限流条件时，直接限流**
- **关联： 当关联的资源达到阈值时，就限流自己（适合做应用让步）**
- **链路： 只记录指定链路上的流量，指定资源从入口资源进来的流量，如果达到阈值，就可以限流**

**（1）直接拒绝：**<br />默认的流控模式，当QPS超过任意规则的阈值后，新的请求就会被立即拒绝，拒绝方式为抛出 `FlowException`，这里不再详细介绍。<br />**（2）关联：**<br />当两个资源之间具有资源争抢或者依赖关系的时候，这两个资源便具有了关联，该流控模式的作用是当关联的资源达到阈值时，就限流自己，适合做应用让步。<br />比如对数据库同一个字段的读操作和写操作存在争抢，读的速度过高会影响写得速度，写的速度过高会影响读的速度，如果放任读写操作争抢资源，则争抢本身带来的开销会降低整体的吞吐量，这时可使用关联限流来避免具有关联关系的资源之间过度的争抢。<br />举例来说，read_db 和 write_db 这两个资源分别代表数据库读写，可以给 read_db 设置限流规则来达到写优先的目的<br />还有一个例子，电商的 下订单 和 支付 两个操作，一旦支付接口达到了阈值，那么需要优先保障支付操作， 那么订单接口就应用被限流，从而保护支付的目的。
```java
@RestController
@RequestMapping("/sentinel")
public class FlowLimitController {
 
    /**
     * 下单接口
     */
    @GetMapping("/order")
    public String order()  {
        return "下单成功..........";
    }
 
    /**
     * 支付接口
     */
    @GetMapping("/pay")
    public String pay()  {
        return "支付成功..........";
    }
}
```
此时的流控规则配置如下图：<br />![2022-08-20-19-18-09.099036000.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1660998765727-0626ea80-6638-4c33-9302-e397d1bd4d22.png#clientId=u7dd39c7a-3298-4&errorMessage=unknown%20error&from=ui&id=ub704e36c&originHeight=526&originWidth=661&originalType=binary&ratio=1&rotation=0&showTitle=false&size=104700&status=error&style=shadow&taskId=udf45509f-74c1-46a9-94bc-85c996982fd&title=)<br />注意：关联之后，这里设置的限流规则是针对被关联资源，也就是 /sentinel/pay 这个资源，但是真正被限流则是 /sentinel/order。只需要不断的请求 /sentinel/pay 达到阈值，然后在请求 /sentinel/order，就可以看到订单接口被限流了<br />![2022-08-20-19-18-09.276080700.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1660998765726-83505982-8a57-4ff9-afff-6d376cad1dec.png#clientId=u7dd39c7a-3298-4&errorMessage=unknown%20error&from=ui&id=l8O2a&originHeight=81&originWidth=677&originalType=binary&ratio=1&rotation=0&showTitle=false&size=15277&status=error&style=shadow&taskId=ua8a2d448-56f2-4a3b-8f4d-cc8286762de&title=)<br />**（3）链路：**<br />该流控模式针对资源链路上的接口进行限流，例如：A、B两个接口都调用某一资源C，A -> C、B -> C 可以看成两个简单的链路，此时可以针对C配置链路限流，比如限制A调用C，而B调用C则不受影响，它的功能有点类似于针对来源配置项，但链路流控是针对上级接口，它的粒度更细。
```java
@RestController
public class TestController  {
 
    @Autowired
    TestService testService;
 
    @GetMapping("/test")
    public String test() throws InterruptedException {
        testService.test();
        System.out.println("链路流控模式限流示例");
        return "test";
    }
 
    @GetMapping("/test1")
    public String test1(){
        testService.test();
        System.out.println("正常链路接口");
        return "test";
    }
}
```
```java
@Service
public class TestService {
 
    @SentinelResource(value = "service")
    public void test() {
        System.out.println("service");
    }
}
```
添加配置：
```
spring.cloud.sentinel.web-context-unify=false
```
此时的流控规则配置如下图：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661000254459-619ce0b1-f08f-4825-939e-ac299948e47d.png#clientId=u7dd39c7a-3298-4&errorMessage=unknown%20error&from=paste&height=606&id=ud520a2d9&originHeight=1516&originWidth=1901&originalType=binary&ratio=1&rotation=0&showTitle=false&size=265037&status=error&style=shadow&taskId=ud757f664-c70b-408e-8ffc-89e172bff37&title=&width=760.4)<br />不断访问 test 和 test1 两个接口，发现即使 test 接口被限流了，test1 接口也不受影响。
<a name="PfrJJ"></a>
#### 2.5、流控规则的硬编码方式：
前面的示例都是基于 sentinel-dashboard 控制台去定义限流规则，也可以通过调用 `FlowRuleManager.loadRules()` 方法来用硬编码的方式定义流量控制规则，比如：
```java
private void initFlowQpsRule() {
    List<FlowRule> rules = new ArrayList<>();
    FlowRule rule = new FlowRule(resourceName);
    // set limit qps to 20
    rule.setCount(20);
    rule.setGrade(RuleConstant.FLOW_GRADE_QPS);
    rule.setLimitApp("default");
    rules.add(rule);
    FlowRuleManager.loadRules(rules);
}
```
官网示例链接：[https://github.com/alibaba/Sentinel/wiki/如何使用#流量控制规则-flowrule](https://github.com/alibaba/Sentinel/wiki/如何使用#流量控制规则-flowrule)
<a name="YjwkN"></a>
### 3、熔断降级规则：
<a name="FXYLp"></a>
#### 3.1、熔断降级规则的核心属性：
熔断机制可以防止应用程序不断执行可能失败的操作，快速地拒绝可能导致错误的调用，当感知到下游服务的资源出现不稳定状态（调用超时或异常比例升高时），暂时切断对下游服务的调用，而不是一直阻塞等待服务响应，阻止级联失败导致的雪崩效应，保证系统的可用性；尤其是后端太忙的时候，使用熔断设计可以保护后端不会过载。<br />另外，在微服务架构下，一般都会设置超时与重试机制，但如果错误太多，或是在短时间内得不到修复，那么重试也没有意义了，这时也需要使用熔断机制快速返回结果。当资源被熔断降级后，在接下来的降级时间窗口之内，对该资源的调用都会自动返回降级数据（默认行为是抛出 `DegradeException`），当检测到该节点的服务调用响应正常后，则恢复调用链路。<br />熔断降级规则包含以下核心属性：<br />![2022-08-20-19-18-09.470119600.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1660999310436-078fd403-af4d-4d34-b458-5804ffa08034.png#clientId=u7dd39c7a-3298-4&errorMessage=unknown%20error&from=ui&id=uc3db3aaf&originHeight=377&originWidth=902&originalType=binary&ratio=1&rotation=0&showTitle=false&size=53658&status=error&style=shadow&taskId=u2b66bb8e-daf1-4637-b517-250916addea&title=)<br />以上几种属性在 sentinel-dashboard 控制台对应的规则如下图：<br />![2022-08-20-19-18-09.744426600.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1660999310445-9ae45c76-a489-4d60-84c8-d62795072ce4.png#clientId=u7dd39c7a-3298-4&errorMessage=unknown%20error&from=ui&id=j4Rfv&originHeight=252&originWidth=546&originalType=binary&ratio=1&rotation=0&showTitle=false&size=27622&status=error&style=shadow&taskId=u421e8dc4-6c47-4f1a-9866-1aa180aded7&title=)
<a name="AaBX9"></a>
#### 3.2、熔断策略：
sentinel 提供了以下几种荣熔断策略：<br />**（1）平均响应时间 ：**<br />`DEGRADE_GRADE_RT`，当资源的平均响应时间超过阈值（`DegradeRule` 中的 `count`，以 ms 为单位）之后，资源会进入降级状态，那么在接下的时间窗口（`DegradeRule` 中的 `timeWindow`，以 s 为单位）之内，对这个方法的调用都会自动地熔断（抛出 `DegradeException`）。<br />注意：Sentinel 默认统计的 RT 上限是 4900 ms，超出此阈值的都会算作 4900 ms，若需要变更此上限可以通过启动配置项 `-Dcsp.sentinel.statistic.max.rt=xxx` 来配置。<br />**（2）异常比例 ：**<br />`DEGRADE_GRADE_EXCEPTION_RATIO`，当资源的每秒异常总数占通过量的比值超过阈值（`DegradeRule` 中的 count）之后，资源进入降级状态，即在接下的时间窗口（`DegradeRule` 中的 `timeWindow`，以 s 为单位）之内，对这个方法的调用都会自动地熔断。<br />异常比率的阈值范围是 [0.0, 1.0]，代表 0% - 100%。<br />**（3）异常数 ：**<br />`DEGRADE_GRADE_EXCEPTION_COUNT`，当资源近 1 分钟的异常数目超过阈值之后会进行熔断。<br />注意由于统计时间窗口是分钟级别的，若 `timeWindow` 小于 60s，则结束熔断状态后仍可能再进入熔断状态。
<a name="hfkCP"></a>
#### 3.3、熔断降级规则的硬编码方式：
在 sentinel-dashboard 控制台去定义熔断规则比较简单，就不详细展开了。除此之外，也可以通过调用 `DegradeRuleManager.loadRules()` 方法来用硬编码的方式定义流量控制规则，比如：
```java
@PostConstruct
public void initSentinelRule(){
    //熔断规则：5s内调用接口出现异常次数超过5的时候, 进行熔断
    List<DegradeRule> degradeRules = new ArrayList<>();
    DegradeRule rule = new DegradeRule();
    rule.setResource("queryGoodsInfo");
    rule.setCount(5);
    //熔断规则
    rule.setGrade(RuleConstant.DEGRADE_GRADE_EXCEPTION_COUNT);
    rule.setTimeWindow(5);
    degradeRules.add(rule);
    DegradeRuleManager.loadRules(degradeRules);
}
```
官方示例链接：[https://github.com/alibaba/Sentinel/wiki/如何使用#熔断降级规则-degraderule](https://github.com/alibaba/Sentinel/wiki/如何使用#熔断降级规则-degraderule)
<a name="NpOXC"></a>
### 4、热点参数规则：
热点就是经常访问的数据，很多时候肯定希望统计某个访问频次 Top K 数据并对其进行限流，比如：

- **商品 ID 为参数，统计一段时间内最常购买的商品 ID 并进行限制**
- **用户 ID 为参数，针对一段时间内频繁访问的用户 ID 进行限制**

热点参数限流会统计传入参数中的热点参数，并根据配置的限流阈值与模式，对包含热点参数的资源调用进行限流。热点参数限流可以看做是一种特殊的流量控制，仅对包含热点参数的资源调用生效。<br />Sentinel 利用 LRU 策略统计最近最常访问的热点参数，结合令牌桶算法来进行参数级别的流控。<br />热点参数限流只针对QPS。
<a name="Bky1i"></a>
#### 4.1、热点参数规则的核心属性：
热点参数规则（`ParamFlowRule`）类似于流量控制规则（`FlowRule`）：<br />![2022-08-20-19-18-09.789201700.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1660999432411-8d75a65b-4f3f-466d-aec9-bebb5c78757e.png#clientId=u7dd39c7a-3298-4&errorMessage=unknown%20error&from=ui&id=u1706ffb9&originHeight=491&originWidth=908&originalType=binary&ratio=1&rotation=0&showTitle=false&size=68519&status=error&style=shadow&taskId=u7d0c7b6e-b4d1-441e-b2b6-ac9cc6a02b0&title=)
<a name="xIKTp"></a>
#### 4.2、热点参数规则的硬编码方式：
**（1）定义资源：**
```java
@GetMapping("/byHotKey")
@SentinelResource(value = "byHotKey",blockHandler = "userAccessError")
public String byHotKeyTest(@RequestParam(value = "userId", required = false) String userId,
                    @RequestParam(value = "goodId", required = false) int goodId){
    log.info(Thread.currentThread().getName() + "\t" + "...byHotKey");
    return "-----------by HotKey：UserId";
}
```
可以看到，上边的代码示例有两个参数，分别是 userId、goodId<br />**（2）添加热点参数限流规则：**`*`<br />可以通过 `ParamFlowRuleManager` 的 `loadRules` 方法配置热点参数规则，案例的具体限流代码如下：
```java
ParamFlowRule pRule = new ParamFlowRule("byHotKey")
    .setParamIdx(1)
    .setCount(1);
// 针对 goodId 为1000 的参数值，单独设置限流 QPS 阈值为 5，而不是全局的阈值 1.
ParamFlowItem item = new ParamFlowItem().setObject(String.valueOf(1000))
                .setClassType(int.class.getName())
                .setCount(5);
pRule.setParamFlowItemList(Collections.singletonList(item));
 
ParamFlowRuleManager.loadRules(Collections.singletonList(pRule));
```
官方示例链接 ：[https://github.com/alibaba/Sentinel/wiki/热点参数限流](https://github.com/alibaba/Sentinel/wiki/热点参数限流)
<a name="xLp4X"></a>
#### 4.3、热点参数的控制台配置方式：
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661000427867-ed27e729-edca-4a41-8af7-43018d13ec7c.png#clientId=u7dd39c7a-3298-4&errorMessage=unknown%20error&from=paste&height=551&id=u11044f48&originHeight=1377&originWidth=1367&originalType=binary&ratio=1&rotation=0&showTitle=false&size=183823&status=error&style=shadow&taskId=ub737363b-1904-4000-afd6-a36a325a332&title=&width=546.8)<br />上述配置的具体含义，当 “byHotKey” 这个资源的第1个参数（这里参数索引是从0开始），即 goodId 的 QPS 超过1秒1次将会被限流，需要注意的是，只有包含指定索引的参数请求才会被限流，否则不影响。<br />另外，sentinel 提供了 参数例外项 这个配置，从上图配置中，将参数 goodId 的参数值等于1000 时，限流阈值设置成了 5，也就是说 goodId = 1000 这个请求 QPS 放宽到每秒 5 次 以上才会被限流。
<a name="iSIES"></a>
### 5、系统保护规则：
Sentinel 系统保护的目标是在系统不被拖垮的情况下，提高系统的吞吐率，系统自适应限流从整体维度对应用入口流量进行控制，从单台机器的 Load、CPU 使用率、总体平均 RT、入口 QPS 和 并发线程数 等几个维度的监控指标，通过自适应的流控策略，让系统的入口流量和系统的负载达到一个平衡，让系统尽可能跑在最大吞吐量的同时保证系统整体的稳定性。
<a name="cWgrh"></a>
#### 5.1、系统保护规则的核心属性：
系统保护规则包含下面几个重要的属性：<br />![2022-08-20-19-18-10.016631500.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661000485969-061c4243-a549-4643-9f8d-bf558f0a3349.png#clientId=u7dd39c7a-3298-4&errorMessage=unknown%20error&from=ui&id=u4b0d7582&originHeight=233&originWidth=597&originalType=binary&ratio=1&rotation=0&showTitle=false&size=27909&status=error&style=shadow&taskId=ubd673e53-097f-4405-a730-f3f759d057b&title=)<br />所以，sentinel 的系统保护支持以下几种模式：

- **Load 自适应：（仅对 Linux/Unix-like 机器生效）系统的 load1 作为触发指标，进行自适应系统保护。当系统 load1 超过设定的触发值，且系统当前的并发线程数超过估算的系统容量时才会触发系统保护（BBR 阶段）。系统容量由系统的 maxQps * minRt 估算得出。设定参考值一般是 CPU cores * 2.5。**
- **CPU usage：（1.5.0+ 版本）当系统 CPU 使用率超过阈值即触发系统保护（取值范围 0.0-1.0），比较灵敏。**
- **平均 RT： 当单台机器上所有入口流量的平均 RT 达到阈值即触发系统保护，单位是毫秒。**
- **并发线程数： 当单台机器上所有入口流量的并发线程数达到阈值即触发系统保护。**
- **入口 QPS： 当单台机器上所有入口流量的 QPS 达到阈值即触发系统保护。**

以上几种模式在 sentinel-dashboard 控制台对应的配置如下图：<br />![2022-08-20-19-18-10.156421600.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661000503245-300e46ec-0091-4a3b-9a20-00321efcccb2.png#clientId=u7dd39c7a-3298-4&errorMessage=unknown%20error&from=ui&id=u174c46a4&originHeight=256&originWidth=675&originalType=binary&ratio=1&rotation=0&showTitle=false&size=31243&status=error&style=shadow&taskId=u1daa5ea6-d47d-46d0-9503-e64ff695a96&title=)<br />注意：系统保护规则是应用整体维度的，而不是资源维度的，并且仅对入口流量生效。入口流量指的是进入应用的流量，比如 Web 服务或 Dubbo 服务端接收的请求，都属于入口流量。
<a name="Lj4LN"></a>
#### 5.2、系统保护规则的硬编码方式：
除了在 sentinel 控制台配置系统保护规则，也可以通过调用 `SystemRuleManager.loadRules()` 方法来用硬编码的方式定义流量控制规则。
```java
private void initSystemRule() {
    List<SystemRule> rules = new ArrayList<>();
    SystemRule rule = new SystemRule();
    rule.setHighestSystemLoad(10);
    rules.add(rule);
    SystemRuleManager.loadRules(rules);
}
```
官方示例链接：[https://github.com/alibaba/Sentinel/wiki/如何使用#系统保护规则-systemrule](https://github.com/alibaba/Sentinel/wiki/如何使用#系统保护规则-systemrule)
<a name="PVbFo"></a>
### 6、来源访问控制规则：
来源访问控制，就是根据调用来源判断该次请求是否允许放行，也就是黑白名单控制。若配置白名单则只有请求来源位于白名单内时才可通过；若配置黑名单则请求来源位于黑名单时不通过，其余的请求通过。
<a name="EKaXU"></a>
#### 6.1、来源访问控制的核心配置：
来源访问控制规则（`AuthorityRule`）非常简单，主要有以下配置项：

- `**resource**`**： 资源名，即限流规则的作用对象。**
- `**limitApp**`**： 对应的黑名单/白名单，不同来源用 “,” 分隔，如 “appA,appB”。**
- `**strategy**`**： 限制模式，**`**AUTHORITY_WHITE**`** 为白名单模式，**`**AUTHORITY_BLACK**`** 为黑名单模式，默认为白名单模式。**
<a name="U4xBR"></a>
#### 6.2、来源访问规则的硬编码方式：
比如希望控制对资源 test 的访问设置白名单，只有来源为 appA 和 appB 的请求才可通过，则可以配置如下白名单规则：<br />官方示例链接：[https://github.com/alibaba/Sentinel/wiki/如何使用#访问控制规则-authorityrule](https://github.com/alibaba/Sentinel/wiki/如何使用#访问控制规则-authorityrule)
<a name="E4Dy2"></a>
#### 6.3、来源访问规则的控制台配置方式：
![2022-08-20-19-18-10.233585100.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661000615421-711663f1-e376-42e7-abc3-119fbef13b46.png#clientId=u7dd39c7a-3298-4&errorMessage=unknown%20error&from=ui&id=ub1c52012&originHeight=592&originWidth=1917&originalType=binary&ratio=1&rotation=0&showTitle=false&size=87533&status=error&style=shadow&taskId=ufbce1ec5-c251-450d-9b5d-df76c17f2d1&title=)
<a name="x7opb"></a>
### 7、集群流控：
<a name="jG1HL"></a>
#### 7.1、为什么需要集群流控：
有了单机流控，还需要集群流控的原因如下：

- **微服务要保证高可用，必须是集群，假设有100个集群，那么想要设置流控规则，就要每个微服务都设置一遍，这样的维护成本太高了**
- **单体流控容易造成流量不均匀的问题，出现总流控阈值没有达到，但某些微服务却已经被限流了，这个是非常糟糕的问题，因此实际生产中对于集群不推荐单体流控。**

所以 sentinel 提供了集群流控规则解决上述的问题，思想很简单，就是提供一个专门的 server 来统计调用的总量，其他的实例都与 server 保持通信。通过集群流控可以精确地控制整个集群的调用总量，结合单机限流兜底，可以更好地发挥流量控制的效果。集群流控中共有两种身份：

- **Token Client： 集群流控客户端，用于向所属 Token Server 通信请求 token。集群限流服务端会返回给客户端结果，决定是否限流。**
- **Token Server： 即集群流控服务端，处理来自 Token Client 的请求，根据配置的集群规则判断是否应该发放 token（是否允许通过）。**

sentinel的集群限流有两种模式，分别如下：

- **独立模式（Alone）： 即作为独立的 token server 进程启动，独立部署，隔离性好，但是需要额外的部署操作。独立模式适合作为 Global Rate Limiter 给集群提供流控服务。**
- **嵌入模式（Embedded）： 即作为内置的 token server 与服务在同一进程中启动。在此模式下，集群中各个实例都是对等的，token server 和 client 可以随时进行转变，因此无需单独部署，灵活性比较好。但是隔离性不佳，需要限制 token server 的总 QPS，防止影响应用本身。嵌入模式适合某个应用集群内部的流控。**
<a name="PqtfR"></a>
#### 7.2、集群流控的控制台配置方式：
下面就以嵌入模式为例介绍一下如何配置。就以 sentinel-openfeign-provider 这个模块作为演示，直接启动三个集群，端口分别为 9009、9011、9013，如下图：<br />![2022-08-20-19-18-10.433728100.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661000615439-519299e6-f955-44ab-864e-10b9459df191.png#clientId=u7dd39c7a-3298-4&errorMessage=unknown%20error&from=ui&id=lYugZ&originHeight=194&originWidth=630&originalType=binary&ratio=1&rotation=0&showTitle=false&size=103395&status=error&style=shadow&taskId=ufe81c703-e080-4660-af62-42cf4ce3993&title=)<br />此时只需要在控制台指定一个服务为token server，其他的为token client，集群流控->新增token server，操作如下图：<br />![2022-08-20-19-18-10.655326400.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661000682845-d6ebc73e-15ab-450c-879c-e063e228d9fd.png#clientId=u7dd39c7a-3298-4&errorMessage=unknown%20error&from=ui&id=uabb0a04d&originHeight=663&originWidth=1618&originalType=binary&ratio=1&rotation=0&showTitle=false&size=225735&status=error&style=shadow&taskId=uf2e519bb-3b71-4a17-9f05-170a1673c0e&title=)<br />选取一个作为服务端，另外两个作为客户端，此时就已经配置好了，如下图：<br />![2022-08-20-19-18-10.914346100.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661000682847-5b3ef841-d45c-4da6-a65b-5236b18c7098.png#clientId=u7dd39c7a-3298-4&errorMessage=unknown%20error&from=ui&id=kyj2U&originHeight=555&originWidth=1623&originalType=binary&ratio=1&rotation=0&showTitle=false&size=184125&status=error&style=shadow&taskId=ub67a5699-60d1-4105-8b79-c904406e4cb&title=)此时就可以添加集群流控规则了，可以在sentinel控制台直接添加，如下图：<br />![2022-08-20-19-18-11.048382900.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661000706553-c00401cc-3a24-4d2a-bf58-58d30f76e4cc.png#clientId=u7dd39c7a-3298-4&errorMessage=unknown%20error&from=ui&id=u151af321&originHeight=311&originWidth=1614&originalType=binary&ratio=1&rotation=0&showTitle=false&size=141944&status=error&style=shadow&taskId=u89286306-b9c4-4e60-acdd-1a3a3d1bb4c&title=)<br />至此集群流控到这儿就介绍完了
<a name="p6Dm1"></a>
## 四、sentinel 整合 openFeign
上文介绍的都是在单个模块间的进行 fallback 和 blockhandler 测试，但在实际的 SpringCloud 微服务开发场景中肯定会遇到服务间远程服务调用的问题，而目前最主流的远程调用组件就是 openFeign 了，那接下来看看 sentinel 如何整合 openFeign 进行熔断降级。
<a name="pbhUj"></a>
#### 1、引入 openFeign 的依赖：
```xml
<dependency>
  <groupId>org.springframework.cloud</groupId>
  <artifactId>spring-cloud-starter-openfeign</artifactId>
</dependency>
```
2、配置文件中开启 `epenFeign` 对 sentinel 熔断降级的支持：
```
feign.sentinel.enabled=true
```
3、消费者主启动类添加注解 `@EnableFeignClients` 激活 openFeign<br />4、消费者声明接口，标识要调用提供方模块的哪个方法，并在 `@FeignClient` 注解中使用 `fallback` 属性指定熔断降级类：
```java
@FeignClient(value = "payment-provider", fallback = PaymentServiceImpl.class)
public interface PaymentService {
 
    @GetMapping("/payment/get/{id}")
    public CommonResult paymentSql(@PathVariable("id")Long id);
}
```
5、添加降级类：实现第4步的定义的接口，并添加降级逻辑：
```java
@Component
public class PaymentServiceFallback implements PaymentService {
 
    @Override
    public CommonResult paymentSql(Long id) {
        return new CommonResult(414, "open-feign 整合 sentinel 实现的全局服务降级策略",null);
    }
}
```
`PaymentServiceFallback` 是 `PaymentService` 的降级回调类，一旦`PaymentService` 中对应得接口出现了异常，则会调用这个类中对应得方法进行降级处理。
<a name="MgFCl"></a>
## 五、Sentinel 所存在的问题
通过 Sentinel-Dashboard 控制台配置的规则依然要推送到微服务应用中 Sentinel 客户端本身才能生效，而微服务之间的调用链路等指标信息也需要推送给Sentinel控制台，才能比较方便地使用Sentinel提供的一些能力，因此在开源的架构版本中需要微服务应用本身开启独立端口与 sentinel-dashboard 进行通信，从而获取配置规则以及上送微服务应用的各类指标信息。<br />而这显然也会占用微服务额外的资源，并且由于 sentinel-dashboard 在此条件下并不具备集群部署能力，因此也会形成一个单节点问题，但是有一套控制台总好过于没有，如果希望比较方便快速地应用 Sentinel 这也是一种代价。此时的 Sentinel 架构如下图所示：<br />![2022-08-20-19-18-11.317488800.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661000822850-3067d704-914b-4f90-9db5-cc751517402a.png#clientId=u7dd39c7a-3298-4&errorMessage=unknown%20error&from=ui&id=u0f3c3d7a&originHeight=625&originWidth=1019&originalType=binary&ratio=1&rotation=0&showTitle=false&size=215425&status=error&style=shadow&taskId=u5ddb2158-cced-4cb1-8b61-c160337be4f&title=)<br />在开源版本架构中，通过 sentinel-dashboard 控制台配置的限流、熔断降级等规则都是存储于 Sentinel-Dashboard 控制台服务的内存之中，如果控制台服务重启或者微服务应用重启都会导致规则丢失，而这在生产环境下显然是不能接受的，因此 Sentinel 官方推荐在生产架构中使用第三方数据源作为永久存储中心，比如 nacos、apollo、zookeeper，这样各个微服务的限流、降级规则都可以永久存储。
