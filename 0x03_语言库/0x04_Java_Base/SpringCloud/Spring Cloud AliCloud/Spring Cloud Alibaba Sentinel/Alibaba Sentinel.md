JavaSpringCloud Alibaba Sentinel
<a name="SHk2d"></a>
## **Sentinel 是什么？**
随着微服务的流行，服务和服务之间的稳定性变得越来越重要。Sentinel 以流量为切入点，从流量控制、熔断降级、系统负载保护等多个维度保护服务的稳定性。<br />Sentinel 具有以下特征：

- **丰富的应用场景**：Sentinel 承接了阿里巴巴近 10 年的双十一大促流量的核心场景，例如秒杀（即突发流量控制在系统容量可以承受的范围）、消息削峰填谷、集群流量控制、实时熔断下游不可用应用等。
- **完备的实时监控**：Sentinel 同时提供实时的监控功能。可以在控制台中看到接入应用的单台机器秒级数据，甚至 500 台以下规模的集群的汇总运行情况。
- **广泛的开源生态**：Sentinel 提供开箱即用的与其它开源框架/库的整合模块，例如与 Spring Cloud、Dubbo、gRPC 的整合。只需要引入相应的依赖并进行简单的配置即可快速地接入 Sentinel。
- **完善的 SPI 扩展点**：Sentinel 提供简单易用、完善的 SPI 扩展接口。可以通过实现扩展接口来快速地定制逻辑。例如定制规则管理、适配动态数据源等。

Sentinel 的主要特性：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652416007203-6c1744e1-af93-4905-8321-daed5d0ba787.png#clientId=u09d36796-8dbb-4&from=paste&id=ud15365b1&originHeight=335&originWidth=710&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u94f27ce7-518a-49cd-b74a-19f102f06ea&title=)
<a name="DWENM"></a>
## **Sentinel 基本概念**
<a name="IFAUB"></a>
### 资源
资源是 Sentinel 的关键概念。它可以是 Java 应用程序中的任何内容，例如，由应用程序提供的服务，或由应用程序调用的其它应用提供的服务，甚至可以是一段代码。在接下来的文档中，都会用资源来描述代码块。只要通过 Sentinel API 定义的代码，就是资源，能够被 Sentinel 保护起来。大部分情况下，可以使用方法签名，URL，甚至服务名称作为资源名来标示资源。
<a name="LbiNP"></a>
### 规则
围绕资源的实时状态设定的规则，可以包括流量控制规则、熔断降级规则以及系统保护规则。所有规则可以动态实时调整。
<a name="u7cuk"></a>
### Sentinel 功能和设计理念
<a name="fDcR6"></a>
#### **流量控制**
流量控制在网络传输中是一个常用的概念，它用于调整网络包的发送数据。然而，从系统稳定性角度考虑，在处理请求的速度上，也有非常多的讲究。任意时间到来的请求往往是随机不可控的，而系统的处理能力是有限的。需要根据系统的处理能力对流量进行控制。Sentinel 作为一个调配器，可以根据需要把随机的请求调整成合适的形状，如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652416007210-159cd7b2-7532-4131-ba48-9bc9d536812f.png#clientId=u09d36796-8dbb-4&from=paste&id=ub83d0b61&originHeight=293&originWidth=592&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uce35ce8f-18c3-4121-bce7-abd1040e16b&title=)<br />流量控制设计理念，流量控制有以下几个角度：

- 资源的调用关系，例如资源的调用链路，资源和资源之间的关系；
- 运行指标，例如 QPS、线程池、系统负载等；
- 控制的效果，例如直接限流、冷启动、排队等。

Sentinel 的设计理念是自由选择控制的角度，并进行灵活组合，从而达到想要的效果。
<a name="BCjOQ"></a>
#### 熔断降级
除了流量控制以外，降低调用链路中的不稳定资源也是 Sentinel 的使命之一。由于调用关系的复杂性，如果调用链路中的某个资源出现了不稳定，最终会导致请求发生堆积。Sentinel 和 Hystrix 的原则是一致的：当检测到调用链路中某个资源出现不稳定的表现，例如请求响应时间长或异常比例升高的时候，则对这个资源的调用进行限制，让请求快速失败，避免影响到其它的资源而导致级联故障。
<a name="xYsYv"></a>
#### 熔断降级设计理念
在限制的手段上，Sentinel 和 Hystrix 采取了完全不一样的方法。Hystrix 通过 线程池隔离 的方式，来对依赖（在 Sentinel 的概念中对应 资源）进行了隔离。这样做的好处是资源和资源之间做到了最彻底的隔离。缺点是除了增加了线程切换的成本（过多的线程池导致线程数目过多），还需要预先给各个资源做线程池大小的分配。
<a name="cNglU"></a>
#### Sentinel 对这个问题采取了两种手段

1. **通过并发线程数进行限制**：和资源池隔离的方法不同，Sentinel 通过限制资源并发线程的数量，来减少不稳定资源对其它资源的影响。这样不但没有线程切换的损耗，也不需要预先分配线程池的大小。当某个资源出现不稳定的情况下，例如响应时间变长，对资源的直接影响就是会造成线程数的逐步堆积。当线程数在特定资源上堆积到一定的数量之后，对该资源的新请求就会被拒绝。堆积的线程完成任务后才开始继续接收请求。
2. **通过响应时间对资源进行降级：**除了对并发线程数进行控制以外，Sentinel 还可以通过响应时间来快速降级不稳定的资源。当依赖的资源出现响应时间过长后，所有对该资源的访问都会被直接拒绝，直到过了指定的时间窗口之后才重新恢复。
<a name="nFyLh"></a>
#### 系统负载保护
Sentinel 同时提供系统维度的自适应保护能力。防止雪崩，是系统防护中重要的一环。当系统负载较高的时候，如果还持续让请求进入，可能会导致系统崩溃，无法响应。在集群环境下，网络负载均衡会把本应这台机器承载的流量转发到其它的机器上去。如果这个时候其它的机器也处在一个边缘状态的时候，这个增加的流量就会导致这台机器也崩溃，最后导致整个集群不可用。针对这个情况，Sentinel 提供了对应的保护机制，让系统的入口流量和系统的负载达到一个平衡，保证系统在能力范围之内处理最多的请求。<br />Sentinel 的开源生态：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652416007463-aab3807b-081d-42a1-9012-cb6b672db131.png#clientId=u09d36796-8dbb-4&from=paste&id=u7464dd01&originHeight=405&originWidth=716&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u302a5d7f-6573-46f7-ba37-fed7d8eed75&title=)<br />**Sentinel 分为两个部分：**

- 核心库（Java 客户端）不依赖任何框架/库，能够运行于所有 Java 运行时环境，同时对 Dubbo / Spring Cloud 等框架也有较好的支持。
- 控制台（Dashboard）基于 Spring Boot 开发，打包后可以直接运行，不需要额外的 Tomcat 等应用容器。
<a name="QgZF0"></a>
## **Sentinel 基本使用**
<a name="SxZuw"></a>
### 1、首先需要导入依赖
```xml
<dependency>
	<groupId>com.alibaba.csp</groupId>
	<artifactId>sentinel-core</artifactId>
	<version>1.6.3</version>
</dependency>
```
<a name="ItvR5"></a>
### 2、定义资源限流规则
**资源** 是 Sentinel 中的核心概念之一。最常用的资源是代码中的 Java 方法。当然，也可以更灵活的定义资源
```java
//初始化规则
private static void initFlowRules() {
	List<FlowRule> rules = new ArrayList<>(); //限流规则的集合
	FlowRule flowRule = new FlowRule();//限流规则
	flowRule.setResource("ruleTest");//资源(可以是方法名称、接口）
	//线程数(FLOW_GRADE_THREAD)与QPS (FLOW_GRADE_QPS)
	flowRule.setGrade(RuleConstant.FLOW_GRADE_QPS); //限流的阈值的类型
	flowRule.setCount(18);// QPS数
	rules.add(flowRule);
	FlowRuleManager.loadRules(rules);
}
```
<a name="QePtd"></a>
### 3、启动测试类，官网提供了以下代码做演示：
```java
public static void main(String[] args) {
	initFlowRules(); //初始化一个规则
	while(true){
		Entry entry=null;
		try{
			entry= SphU.entry("ruleTest");
			System.out.println("Hello Word");
		}catch (BlockException e){//如果被限流了，那么会抛出这个异常
			e.printStackTrace();
		}finally {
			if(entry!=null){
				entry.exit();// 释放
			}
		}
	}
}
```
Demo 运行之后，可以在日志 ~/logs/csp/${appName}-metrics.log.xxx 里看到下面的输出：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652416007231-59c75366-2f32-4758-8f43-0891ea5e3e60.png#clientId=u09d36796-8dbb-4&from=paste&id=u83e02f67&originHeight=213&originWidth=591&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u30572362-1946-4d52-b96b-a664deba8e1&title=)<br />其中 p 代表通过的请求，block 代表被阻止的请求，s 代表成功执行完成的请求个数，e 代表用户自定义的异常，rt 代表平均响应时长。可以看到，这个程序每秒稳定输出 "hello world" 18 次，和规则中预先设定的阈值是一样的。
<a name="qtWiu"></a>
#### Sentinel 控制台
下载 [https://github.com/alibaba/Sentinel/releases](https://github.com/alibaba/Sentinel/releases) ，下载 sentinel-dashboard-1.6.3.jar 。(启动 Sentinel 控制台需要 JDK 版本为 1.8 及以上版本。)<br />启动控制台：
```bash
java -Dserver.port=8080 -Dcsp.sentinel.dashboard.server=localhost:8080 -Dproject.name=sentinel-dashboard -jar sentinel-dashboard.jar
```
其中 `-Dserver.port=8080` 用于指定 Sentinel 控制台端口为 8080。从 Sentinel 1.6.0 起，Sentinel 控制台引入基本的**登录**功能，默认用户名和密码都是 sentinel。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652416007154-cbcfd93a-45a8-42a8-b08f-c2f576e48014.png#clientId=u09d36796-8dbb-4&from=paste&id=u2754f00c&originHeight=289&originWidth=510&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ubde3ef86-162e-474b-8aa1-8099b8739d1&title=).
<a name="NcfhQ"></a>
#### 客户端接入控制台
将刚刚演示的Demo 接入控制台，首秀按要导入依赖：
```xml
<dependency>
	<groupId>com.alibaba.csp</groupId>
	<artifactId>sentinel-transport-simple-http</artifactId>
	<version>1.6.3</version>
</dependency>
```
启动时加入 JVM 参数 `-Dcsp.sentinel.dashboard.server=consoleIp:port` 指定控制台地址和端口。若启动多个应用，则需要通过 `-Dcsp.sentinel.api.port=xxxx` 指定客户端监控 API 的端口（默认是 8719）。其他配置项见：[https://github.com/alibaba/Sentinel/wiki/%E5%90%AF%E5%8A%A8%E9%85%8D%E7%BD%AE%E9%A1%B9](https://github.com/alibaba/Sentinel/wiki/%E5%90%AF%E5%8A%A8%E9%85%8D%E7%BD%AE%E9%A1%B9) <br />配置如下：
```bash
-Dcsp.sentinel.dashboard.server=localhost:8080 -Dproject.name=sentinel-demo
```
这样子启动Demo，客户端配置好了与控制台的连接参数之后，并不会主动连接上控制台，需要触发一次客户端的规则才会开始进行初始化，并向控制台发送心跳和客户端规则等信息。就能再控制台上看到效果如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652416007626-fba8b6b6-5bea-4d6b-ace9-464a88a21b57.png#clientId=u09d36796-8dbb-4&from=paste&id=u521f2f1d&originHeight=331&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ufa544b43-3bdc-4dfa-b057-f861834c2c1&title=)<br />其他具体相关可以查看github介绍。
<a name="wS9EG"></a>
## **Sentinel 整合SpringBoot**
Sentinel 提供了 `@SentinelResource` 注解用于定义资源，并提供了 AspectJ 的扩展用于自动定义资源、处理 `BlockException` 等。首先需要导入依赖：
```xml
<dependency>
	<groupId>com.alibaba.csp</groupId>
	<artifactId>sentinel-core</artifactId>
	<version>1.6.3</version>
</dependency>
<dependency>
	<groupId>com.alibaba.csp</groupId>
	<artifactId>sentinel-transport-simple-http</artifactId>
	<version>1.6.3</version>
</dependency>
<dependency>
	<groupId>com.alibaba.csp</groupId>
	<artifactId>sentinel-annotation-aspectj</artifactId>
	<version>1.6.3</version>
</dependency>
```
在主启动类中添加规则：
```java
@SpringBootApplication
public class SentinelDemoApplication {
	
	public static void main(String[] args) {
		initFlowRules();
		SpringApplication.run(SentinelDemoApplication.class, args);
	}
	
	//初始化规则
	private static void initFlowRules(){
		List<FlowRule> rules=new ArrayList<>(); //限流规则的集合
		FlowRule flowRule=new FlowRule();
		flowRule.setResource("sayHello");//资源(方法名称、接口）
		flowRule.setGrade(RuleConstant.FLOW_GRADE_QPS); //限流的阈值的类型
		flowRule.setCount(2);
		rules.add(flowRule);
		FlowRuleManager.loadRules(rules);
	}
	
}
```
还需要通过配置的方式将 `SentinelResourceAspect` 注册为一个 Spring Bean：
```java
@Configuration
public class AopConfiguration {

    @Bean
    public SentinelResourceAspect sentinelResourceAspect(){
        return new SentinelResourceAspect();
    }
}
```
此刻可以通过注解 `@SentinelResource` 来定义资源 ：
```java
@RestController
public class SentinelController {
    
    //针对方法级别的限流
    @SentinelResource(value = "sayHello", blockHandler = "exceptionHandler", fallback = "fallbackHandler")
    @GetMapping("/say")
    public String sayHello(String name) {
        System.out.println("hello world");
        if (1 == 1) throw new RuntimeException("Hello");
        return "hello world";
    }

    // 该方法的参数除了 BlockException，包括参数、返回值，需要与原来的资源方法一致
    public String exceptionHandler(String name, BlockException ex) {
        return "blockHandler：" + name + ex.getMessage();
    }

    //用于在抛出异常的时候提供 fallback处理逻辑。
    public String fallbackHandler(String name) {
        return "fallbackHandler：" + name;
    }
}
```
启动服务就可以看到效果了。这里需要加入到Sentinel-dashboard中：`-Dcsp.sentinel.dashboard.server=localhost:8080 -Dproject.name=sentinel-demo`
