Java Sentinel
<a name="xlKof"></a>
## Sentinel简介
<a name="dU6Fo"></a>
### 背景分析
在日常生活中，经常会在淘宝、天猫、京东、拼多多等平台上参与商品的秒杀、抢购以及一些优惠活动，也会在节假日使用12306 手机APP抢火车票、高铁票，甚至有时候还要帮助同事、朋友为他们家小孩拉投票、刷票，这些场景都无一例外的会引起服务器流量的暴涨，导致网页无法显示、APP反应慢、功能无法正常运转，甚至会引起整个网站的崩溃。<br />如何在这些业务流量变化无常的情况下，保证各种业务安全运营，系统在任何情况下都不会崩溃呢？可以在系统负载过高时，采用限流、降级和熔断，三种措施来保护系统，由此一些流量控制中间件诞生。例如Sentinel。
<a name="ohIHJ"></a>
### Sentinel概述
Sentinel (分布式系统的流量防卫兵) 是阿里开源的一套用于服务容错的综合性解决方案。它以流量为切入点，从流量控制、熔断降级、系统负载保护等多个维度来保护服务的稳定性。<br />Sentinel 承接了阿里巴巴近 10 年的双十一大促流量的核心场景，例如秒杀（即突发流量控制在系统容量可以承受的范围）、消息削峰填谷、集群流量控制、实时熔断下游不可用应用等。<br />Sentinel核心分为两个部分：

- 核心库（Java 客户端）：能够运行于所有 Java 运行时环境，同时对Dubbo /Spring Cloud 等框架也有较好的支持。
- 控制台（Dashboard）：基于 Spring Boot 开发，打包后可以直接运行。
<a name="qMiIf"></a>
### 安装Sentinel服务
Sentinel 提供一个轻量级的控制台，它提供机器发现、单机资源实时监控以及规则管理等功能，其控制台安装步骤如下：<br />第一步：打开sentinel下载网址[https://github.com/alibaba/Sentinel/releases](https://github.com/alibaba/Sentinel/releases)<br />第二步：下载Jar包（可以存储到一个sentinel目录），如图所示：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1639128899159-10fea893-6080-49ed-b3de-6ed60c123138.png#clientId=ubcaa5794-dc3c-4&from=paste&height=190&id=u038121d8&originHeight=570&originWidth=2273&originalType=binary&ratio=1&rotation=0&showTitle=false&size=63507&status=done&style=none&taskId=u07875bea-9c7a-4efd-961c-8e4d1d31fef&title=&width=757.6666666666666)<br />第三步：在sentinel对应目录，打开命令行(cmd)，启动运行sentinel
```bash
java -Dserver.port=8180 -Dcsp.sentinel.dashboard.server=localhost:8180 -Dproject.name=sentinel-dashboard -jar sentinel-dashboard-1.8.1.jar
```
检测启动过程，如图所示：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1639131567324-1f6b5e91-baac-4dd5-85b4-075252a09cec.png#clientId=u9b712036-59bd-4&from=paste&height=587&id=u2600c245&originHeight=1760&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1508424&status=done&style=none&taskId=u3f7c2873-9bef-4692-8124-16a303004a8&title=&width=1107.6666666666667)
<a name="Uw067"></a>
### 访问Sentinal服务
第一步：假如Sentinal启动ok，通过浏览器进行访问测试，如图所示：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1639131656552-cfcb4b18-b79a-4366-abb9-52b193ee549d.png#clientId=u9b712036-59bd-4&from=paste&height=399&id=u8f6d073d&originHeight=1198&originWidth=1624&originalType=binary&ratio=1&rotation=0&showTitle=false&size=71191&status=done&style=shadow&taskId=uc2230f31-c78e-4752-a810-4baacfcdca8&title=&width=541.3333333333334)<br />第二步：登陆sentinel，默认用户和密码都是sentinel，登陆成功以后的界面如图所示：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1639131709887-9eab328e-0e0f-440b-8226-d6ef542bdb18.png#clientId=u9b712036-59bd-4&from=paste&height=594&id=u7a4de1de&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=276024&status=done&style=shadow&taskId=u0570bec0-8f46-4417-b95a-8e8587b75d7&title=&width=1280)
<a name="zR5pV"></a>
## Sentinel限流入门
<a name="JM5kS"></a>
### 概述
系统中的数据库连接池，线程池，nginx的瞬时并发等在使用时都会给定一个限定的值，这本身就是一种限流的设计。限流的目的防止恶意请求流量、恶意攻击，或者防止流量超过系统峰值。
<a name="PZgKP"></a>
### 准备工作
第一步：Sentinel 应用于服务提供方(sca-provider)，在消费方添加依赖如下：
```xml
<dependency>
    <groupId>com.alibaba.cloud</groupId>
    <artifactId>spring-cloud-starter-alibaba-sentinel</artifactId>
</dependency>
```
第二步：打开服务消费方配置文件bootstrap.yml，添加sentinel配置，代码如下：
```yaml
spring:
  cloud:
    sentinel:
      transport:
         dashboard: localhost:8180 # 指定sentinel控制台地址。
```
第三步：创建一个用于演示限流操作的Controller对象，例如：
```java
package com.jt.provider.controller;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("/provider")
public class ProviderSentinelController {
       @GetMapping("/sentinel01")
       public String doSentinel01(){
           return "sentinel 01 test  ...";
       }
}
```
第四步：启动sca-provider服务，然后对指定服务进行访问，如图所示：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637889037807-95e0236d-563d-40d1-a87f-ed6961af818c.webp#clientId=u44fe9ba7-102b-4&from=paste&id=u67c3e4f4&originHeight=135&originWidth=489&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue01b351d-5db1-4c22-a3b5-6d27c9573b0&title=)
:::danger
注意：Sentinel 会在客户端首次调用时候进行初始化，开始向控制台发送心跳包。因此需要确保客户端有访问量，才能在控制台上看到监控数据。
:::
第五步：刷新sentinel 控制台，实时监控信息，如图所示：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637889038526-91e5dd09-992b-4983-a3f6-839abf58d2fc.webp#clientId=u44fe9ba7-102b-4&from=paste&id=ua2f8002f&originHeight=424&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1472d47d-35b4-456d-a03b-6be2623e964&title=)<br />Sentinel的控制台其实就是一个SpringBoot编写的程序，需要将服务注册到控制台上，即在微服务中指定控制台的地址，并且还要在消费端开启一个与sentinel控制台传递数据端的端口，控制台可以通过此端口调用微服务中的监控程序来获取各种信息。
<a name="tpx7t"></a>
### Sentinel限流入门实践
设置一下指定接口的流控(流量控制)，QPS（每秒请求次数）单机阈值为1，代表每秒请求不能超出1次，要不然就做限流处理，处理方式直接调用失败。<br />第一步：选择要限流的链路，如图所示：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637889038461-c3aa7170-8eb3-468e-8102-f4a60f36442d.webp#clientId=u44fe9ba7-102b-4&from=paste&id=u6d644e0a&originHeight=335&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4a05b918-e8cf-4b60-9fc0-ea5a0ff311f&title=)<br />第二步：设置限流策略，如图所示：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637889038498-ca3e3388-0603-44d2-ba36-9d0cc8c008f9.webp#clientId=u44fe9ba7-102b-4&from=paste&id=ud5d90526&originHeight=385&originWidth=669&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4dc84ab7-0955-490b-9f8e-5191538202b&title=)<br />第三步：反复刷新访问消费端端服务，检测是否有限流信息输出，如图所示：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637889038513-11617be9-9193-40ce-822d-281d639a3e86.webp#clientId=u44fe9ba7-102b-4&from=paste&id=ude64ccd0&originHeight=153&originWidth=520&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u99e0bde0-72df-4bad-86d5-5524a198792&title=)
<a name="UESrb"></a>
### 小节面试分析

- Sentinel是什么？(阿里推出一个流量控制平台，防卫兵)
- 类似Sentinel的产品知道有什么？(hystrix-一代微服务产品)
- Sentinel是如何对请求进行限流的?(基于sentinel依赖提供的拦截器)
- 了解哪些限流算法？(计数器、令牌桶、漏斗算法，滑动窗口算法，…)
- Sentinel 默认的限流算法是什么？(滑动窗口算法)
<a name="F7B5Z"></a>
## Sentinel流控规则分析
<a name="GCPAH"></a>
### 阈值类型

- QPS(Queries Per Second)：当调用相关url对应的资源时，QPS达到单机阈值时，就会限流。
- 线程数：当调用相关url对应的资源时，线程数达到单机阈值时，就会限流。
<a name="pNRLf"></a>
### 设置限流模式
Sentinel的流控模式代表的流控的方式，默认【直接】，还有关联，链路。<br />直接模式<br />Sentinel默认的流控处理就是【直接->快速失败】。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637889038490-af4f3ebb-2852-40d8-a6c4-cb86e592eeb8.webp#clientId=u44fe9ba7-102b-4&from=paste&id=u146a19bd&originHeight=539&originWidth=735&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u965d1015-ded3-4b8e-80bc-c2e31792e5f&title=)<br />关联模式<br />当关联的资源达到阈值，就限流自己。例如设置了关联资源为/ur2时，假如关联资源/url2的qps阀值超过1时，就限流/url1接口（是不是感觉很霸道，关联资源达到阀值，是本资源接口被限流了）。这种关联模式有什么应用场景呢？<br />举个例子，订单服务中会有2个重要的接口，一个是读取订单信息接口，一个是写入订单信息接口。在高并发业务场景中，两个接口都会占用资源，如果读取接口访问过大，就会影响写入接口的性能。<br />业务中如果希望写入订单比较重要，要优先考虑写入订单接口。那就可以利用关联模式；在关联资源上面设置写入接口，资源名设置读取接口就行了；这样就起到了优先写入，一旦写入请求多，就限制读的请求。例如<br />第一步：在ProviderSentinelController中添加一个方法，例如：
```java
@GetMapping("/sentinel02")
public String doSentinel02(){
    return "sentinel 02 test  ...";
}
```
第二步：在sentinel中做限流设计，例如<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637889039265-cf817b66-7e81-4118-baed-e579210d689e.webp#clientId=u44fe9ba7-102b-4&from=paste&id=u3d668751&originHeight=536&originWidth=664&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u640337a1-2461-4ae7-824a-4d14e0a9100&title=)<br />第三步：打开两个测试窗口，对/provider/sentinel02进行访问，检查/provider/sentinel01的状态，例如：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637889039279-b9322ef4-ed7b-4ff2-839a-725675877c57.webp#clientId=u44fe9ba7-102b-4&from=paste&id=ue6fa327a&originHeight=360&originWidth=904&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u13a8b905-da3b-4e67-a750-5cab8378ae0&title=)<br />链路模式<br />链路模式只记录指定链路入口的流量。也就是当多个服务对指定资源调用时，假如流量超出了指定阈值，则进行限流。被调用的方法用`@SentinelResource`进行注解，然后分别用不同业务方法对此业务进行调用，假如A业务设置了链路模式的限流，在B业务中是不受影响的。现在对链路模式做一个实践，例如：<br />例如现在设计一个业务对象，代码如下(为了简单，可以直接写在启动类内部)：<br />第一步：在指定包创建一个ResourceService类，代码如下：
```java
package com.jt.provider.service;
@Service
public class ResourceService{
    @SentinelResource("doGetResource")
    public String doGetResource(){
        return "doGetResource";
    }
}
```
第二步：在ProviderSentinelController中添加一个方法，例如：
```java
@Autowired
private ResourceService resourceService;
@GetMapping("/sentinel03")
public String doSentinel03() throws InterruptedException {
  resourceService.doGetResource();
  return "sentinel 03 test";
}
```
第三步：在sentinel中配置限流规则，例如：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637889039010-359e6db7-7af1-45fb-b4c5-f66be2df9e1d.webp#clientId=u44fe9ba7-102b-4&from=paste&id=u9f310b3b&originHeight=374&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u14a5998f-f949-4704-b3fb-1f9915d6a50&title=)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637889039144-3cdb447f-2785-4aa4-923e-ec09f7a01be6.webp#clientId=u44fe9ba7-102b-4&from=paste&id=u93eabf2a&originHeight=536&originWidth=656&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue1e0ac16-c7cb-4453-9ffe-89aa6610a72&title=)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637889039357-d4cd54da-44bc-485c-a816-de6198b01514.webp#clientId=u44fe9ba7-102b-4&from=paste&id=u2dd650cb&originHeight=581&originWidth=741&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4400f5c6-6168-4232-a78b-d752a69d433&title=)<br />设置链路流控规则后，再频繁对限流链路进行访问，检测是否会出现500异常，例如<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637889039708-07c53137-9ce0-470e-b82e-4ac6c52895a5.webp#clientId=u44fe9ba7-102b-4&from=paste&id=u0d3883bf&originHeight=230&originWidth=784&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7472ff3a-c60a-4cf1-98ba-b3f1312b7be&title=)<br />说明，流控模式为链路模式时，假如是sentinel 1.7.2以后版本，Sentinel Web过滤器默认会聚合所有URL的入口为sentinel_spring_web_context，因此单独对指定链路限流会不生效，需要在application.yml添加如下语句来关闭URL PATH聚合，例如：
```yaml
sentinel:
	web-context-unify: false
```
<a name="rQ6G7"></a>
### 小节面试分析

- 了解sentinel中的阈值应用类型吗?（两种-QPS，线程数）
- Sentinel的限流规则中默认有哪些限流模式?(直连，关联，链路)
- Sentinel的限流效果有哪些？(快速失败，预热，排队)
<a name="R3Np8"></a>
## Sentinel降级应用实践
<a name="Fs8IE"></a>
### 概述
除了流量控制以外，对调用链路中不稳定的资源进行熔断降级也是保障高可用的重要措施之一。由于调用关系的复杂性，如果调用链路中的某个资源不稳定，最终会导致请求发生堆积。<br />Sentinel 熔断降级会在调用链路中某个资源出现不稳定状态时（例如调用超时或异常比例升高），对这个资源的调用进行限制，让请求快速失败，避免影响到其它的资源而导致级联错误。当资源被降级后，在接下来的降级时间窗口之内，对该资源的调用都自动熔断（默认行为是抛出 DegradeException）。
<a name="iLDPl"></a>
### 准备工作
在ProviderController 类中添加doSentinel04方法，基于此方法演示慢调用过程下的限流，代码如下：
```java
//AtomicLong 类支持线程安全的自增自减操作
private AtomicLong atomicLong=new AtomicLong(1);
@GetMapping("/sentinel04")
public  String doSentinel04() throws InterruptedException {
    //获取自增对象的值,然后再加1
    long num=atomicLong.getAndIncrement();
    if(num%2==0){//模拟50%的慢调用比例
        Thread.sleep(200);
    }
    return "sentinel 04 test";
}
```
说明，在此方法中设置休眠，目的是为了演示慢调用(响应时间比较长).
<a name="kcWdb"></a>
### Sentinel降级入门
接下来，基于一个请求链路，进行服务降级及应用实践，例如：<br />第一步：服务启动后，选择要降级的链路，如图所示：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637889039954-b8be7fd6-373f-42e2-a4e5-1b67db99f228.webp#clientId=u44fe9ba7-102b-4&from=paste&id=ucbf074ee&originHeight=265&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u22bf4e26-901c-403c-be3c-13b5a2e36fd&title=)<br />第二步：选择要降级的链路，如图所示：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637889039739-8171e1ab-d562-40b6-8b14-babe7db9b022.webp#clientId=u44fe9ba7-102b-4&from=paste&id=ueeca3267&originHeight=385&originWidth=668&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u69dbfeb0-d78f-47b7-89b9-3fadfb56da3&title=)<br />这里表示熔断策略选择"慢调用比例"，表示请求数超过3时，假如平均响应时间超过200毫秒的有30%，则对请求进行熔断，熔断时长为10秒钟，10秒以后恢复正常。<br />第三步：对指定链路进行刷新，多次访问测试，检测页面上是否会出现 Blocked By Sentinel (flow Limiting)内容.<br />也可以进行断点调试，在`DefaultBlockExceptionHandler`中的`handle`方法内部加断点，分析异常类型，假如异常类型`DegradeException`则为降级熔断。
<a name="s9yiz"></a>
### Sentinel 异常处理
系统提供了默认的异常处理机制，假如默认处理机制不满足需求，可以自己进行定义。定义方式上可以直接或间接实现`BlockExceptionHandler`接口，并将对象交给spring管理。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637889039836-d48f30fa-41b8-4c1a-a7c9-248517e8f758.webp#clientId=u44fe9ba7-102b-4&from=paste&id=u5abcac9b&originHeight=218&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua5663efc-1020-45ca-b3d4-1af6b80efd9&title=)
```java
package com.jt.provider.controller;

import com.alibaba.csp.sentinel.adapter.spring.webmvc.callback.BlockExceptionHandler;
import com.alibaba.csp.sentinel.slots.block.BlockException;
import com.fasterxml.jackson.databind.ObjectMapper;
import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Component;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.PrintWriter;
import java.util.HashMap;
import java.util.Map;

/**
 * 自定义限流,降级等异常处理对象
 */
@Slf4j
@Component
public class ServiceBlockExceptionHandler
     implements BlockExceptionHandler {
    /**
     * 用于处理BlockException类型以及子类类型异常
     */
    @Override
    public void handle(HttpServletRequest request,
                       HttpServletResponse response,
                       BlockException e) throws Exception {
        //设置响应数据编码
        response.setCharacterEncoding("utf-8");
        //告诉客户端响应数据的类型,以及客户端显示内容的编码
        response.setContentType("text/html;charset=utf-8");
        //向客户端响应一个json格式的字符串
        //String str="{\"status\":429,\"message\":\"访问太频繁了\"}";
        Map<String,Object> map=new HashMap<>();
        map.put("status", 429);
        map.put("message","访问太频繁了");
        String jsonStr=new ObjectMapper().writeValueAsString(map);
        PrintWriter out = response.getWriter();
        out.print(jsonStr);
        out.flush();
        out.close();
    }
}
```
<a name="hTdJX"></a>
### 小节面试分析

- 何为降级熔断？（让外部应用停止对服务的访问，生活中跳闸，路障设置-此路不通）
- 为什么要进行熔断呢？(平均响应速度越来越慢或经常出现异常，这样可能会导致调用链堆积，最终系统崩溃)
- Sentinel中限流，降级的异常父类是谁？(BlockException)
- Sentinel 出现降级熔断时，系统底层抛出的异常是谁？(DegradeException)
- Sentinel中异常处理接口是谁？（BlockExceptionHandler）
- Sentinel中异常处理接口下默认的实现类为? (DefaultBlockExceptionHandler)
- 假如Sentinel中默认的异常处理规则不满足需求怎么办?(自己定义)
- 如何自己定义Sentinel中异常处理呢？（直接或间接实现BlockExceptionHandler ）
- Sentinel熔断降级策略有哪些?(慢调用比例、异常比例、异常数)
<a name="HfvnN"></a>
## Sentinel热点规则分析（重点）
<a name="y3hYv"></a>
### 概述
何为热点？热点即经常访问的数据。比如：

- 商品 ID 为参数，统计一段时间内最常购买的商品 ID 并进行限制。
- 用户 ID 为参数，针对一段时间内频繁访问的用户 ID 进行限制。

热点参数限流会统计传入参数中的热点数据，并根据配置的限流阈值与模式，对包含热点参数的资源调用进行限流。热点参数限流可以看做是一种特殊的流量控制，仅对包含热点参数的资源调用生效。其中，Sentinel会利用 LRU 策略统计最近最常访问的热点参数，结合令牌桶算法来进行参数级别的流控。
<a name="rR371"></a>
### 快速入门
第一步：在sca-provider中添加如下方法，例如：
```java
@GetMapping("/sentinel/findById")
@SentinelResource("resource")
public String doFindById(@RequestParam("id") Integer id){
     return "resource id is "+id;
}
```
第二步：服务启动后，选择要限流的热点链路，如图所示：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637889040039-90b334a4-980c-42c6-9f2b-3c54c8f60984.webp#clientId=u44fe9ba7-102b-4&from=paste&id=u1030d49a&originHeight=309&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc3e2826e-13c2-4b61-bff6-043c5ca0a5d&title=)<br />第三步：设置要限流的热点，如图所示：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637889040165-f39ab7b1-58e2-43b7-a2a7-57ae247b4e8a.webp#clientId=u44fe9ba7-102b-4&from=paste&id=uf7bb6e6b&originHeight=422&originWidth=739&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u401c67df-2a25-4263-9cbc-a07b0891e61&title=)<br />热点规则的限流模式只有QPS模式（这才叫热点）。参数索引为@SentinelResource注解的方法参数下标，0代表第一个参数，1代表第二个参数。单机阈值以及统计窗口时长表示在此窗口时间超过阈值就限流。<br />第四步：多次访问热点参数方法，前端会出现如下界面，如图所示：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637889040386-612bd868-f1b5-42d8-bc80-a84677a5b756.webp#clientId=u44fe9ba7-102b-4&from=paste&id=ue9308693&originHeight=230&originWidth=776&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3b42ea07-8cc8-46e3-8a7f-84eda9bc08a&title=)<br />然后，在后台出现如下异常表示限流成功。
```java
com.alibaba.csp.sentinel.slots.block.flow.param.ParamFlowException: 2
```
其中，热点参数其实说白了就是特殊的流控，流控设置是针对整个请求的；但是热点参数他可以设置到具体哪个参数，甚至参数针对的值，这样更灵活的进行流控管理。<br />一般应用在某些特殊资源的特殊处理，如：某些商品流量大，其他商品流量很正常，就可以利用热点参数限流的方案。
<a name="rQzeL"></a>
### 特定参数设计
配置参数例外项，如图所示：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637889040615-4c131afb-ab02-40a8-b4f5-8a2adb671d27.webp#clientId=u44fe9ba7-102b-4&from=paste&id=uefde37c1&originHeight=625&originWidth=605&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u377bc783-bf1d-4311-a1e2-f7752f8924b&title=)<br />其中，这里表示参数值为5时阈值为100，其它参数值阈值为1.
<a name="OtR7r"></a>
### 小节面试分析

- 如何理解热点数据？(访问频度比较高的数据，某些商品、谋篇文章、某个视频)
- 热点数据的限流规则是怎样的?(主要是针对参数进行限流设计)
- 热点数据中的特殊参数如何理解？(热点限流中的某个参数值的阈值设计)
- 对于热点数据的访问出现限流以后底层异常是什么？(ParamFlowException)
<a name="Q3bHg"></a>
## Sentinel系统规则（了解）
<a name="h0Q5L"></a>
### 概述
系统在生产环境运行过程中，经常需要监控服务器的状态，看服务器CPU、内存、IO等的使用率；主要目的就是保证服务器正常的运行，不能被某些应用搞崩溃了；而且在保证稳定的前提下，保持系统的最大吞吐量。
<a name="gVFIH"></a>
### 快速入门
Sentinel的系统保护规则是从应用级别的入口流量进行控制，从单台机器的总体 Load（负载）、RT（响应时间）、入口 QPS 、线程数和CPU使用率五个维度监控应用数据，让系统尽可能跑在最大吞吐量的同时保证系统整体的稳定性。如图所示：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637889040473-df204557-80be-42ce-91e4-281408e2d6a1.webp#clientId=u44fe9ba7-102b-4&from=paste&id=u69438f5a&originHeight=308&originWidth=827&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6e83f127-fba4-4803-8a21-de2e78e605a&title=)<br />系统规则是一种全局设计规则，其中，

- Load（仅对 Linux/Unix-like 机器生效）：当系统 load1 超过阈值，且系统当前的并发线程数超过系统容量时才会触发系统保护。系统容量由系统的 maxQps * minRt 计算得出。设定参考值一般是 CPU cores * 2.5。
- CPU使用率：当系统 CPU 使用率超过阈值即触发系统保护（取值范围 0.0-1.0）。
- RT：当单台机器上所有入口流量的平均 RT 达到阈值即触发系统保护，单位是毫秒。
- 线程数：当单台机器上所有入口流量的并发线程数达到阈值即触发系统保护。
- 入口 QPS：当单台机器上所有入口流量的 QPS 达到阈值即触发系统保护。
- 说明，系统保护规则是应用整体维度的，而不是资源维度的，并且仅对入口流量生效。入口流量指的是进入应用的流量（EntryType.IN），比如 Web 服务。
<a name="NZnW0"></a>
### 小节面试分析

- 如何理解sentinel中的系统规则？(是对所有链路的控制规则，是一种系统保护策略)
- Sentinel的常用系统规则有哪些？(RT,QPS,CPU,线程,Load-linux,unix)
- Sentinel系统保护规则被触发以后底层会抛出什么异常？（SystemBlockException）
<a name="szfrw"></a>
## Sentinel授权规则(重要)
<a name="Lbte8"></a>
### 概述
很多时候，需要根据调用方来限制资源是否通过，这时候可以使用 Sentinel 的黑白名单控制的功能。黑白名单根据资源的请求来源（origin）限制资源是否通过，若配置白名单则只有请求来源位于白名单内时才可通过；若配置黑名单则请求来源位于黑名单时不通过，其余的请求通过。例如微信中的黑名单。
<a name="I6ppc"></a>
### 快速入门
sentinel可以基于黑白名单方式进行授权规则设计，如图所示：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637889040569-63ca6bc1-6a4b-4b3b-b626-4bd511413c0d.webp#clientId=u44fe9ba7-102b-4&from=paste&id=u960879c1&originHeight=375&originWidth=839&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u03c603d2-a72e-44e9-95b3-c9471762c14&title=)<br />黑白名单规则（AuthorityRule）非常简单，主要有以下配置项：

- 资源名：即限流规则的作用对象
- 流控应用：对应的黑名单/白名单中设置的规则值，多个值用逗号隔开.
- 授权类型：白名单，黑名单(不允许访问).

案例实现：<br />定义请求解析器，用于对请求进行解析，并返回解析结果，sentinel底层在拦截到用户请求以后，会对请求数据基于此对象进行解析，判定是否符合黑白名单规则，例如：<br />第一步：定义RequestOriginParser接口的实现类，在接口方法中解析请求参数数据并返回，底层会基于此返回值进行授权规则应用。
```java
@Component
public class DefaultRequestOriginParser implements RequestOriginParser {
    @Override
    public String parseOrigin(HttpServletRequest request) {
        String origin = request.getParameter("origin");//这里的参数名会与请求中的参数名一致
        return origin;
    }
}
```
第二步：定义流控规则，如图所示：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637889040632-1ba0038d-0b39-47c1-ac51-f98a500db700.webp#clientId=u44fe9ba7-102b-4&from=paste&id=u9d86e830&originHeight=375&originWidth=839&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub162184b-265e-41ce-a27b-6dfc3f8976a&title=)<br />第三步：执行资源访问，检测授权规则应用，当配置的流控应用值为app1时，假如规则为黑名单，则基于 http://ip:port/path?origin=app1的请求不可以通过，其请求处理流程如图下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637889041056-aef5479b-d9e3-467a-8c77-129788e40a35.webp#clientId=u44fe9ba7-102b-4&from=paste&id=u0510eedc&originHeight=1098&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4ffbc614-bd76-444d-bdb3-a631cb5eef9&title=)<br />拓展：尝试基于请求ip等方式进行黑白名单的规则设计，例如：<br />第一步：修改请求解析器，获取请求ip并返回，例如：
```java
@Component
public class DefaultRequestOriginParser  implements RequestOriginParser {
    //解析请求源数据
    @Override
    public String parseOrigin(HttpServletRequest request) {
        //获取访问请求中的ip地址,基于ip地址进行黑白名单设计（例如在流控应用栏写ip地址）
        String ip= request.getRemoteAddr();
        System.out.println("ip="+ip);
        return ip;
    }//授权规则中的黑白名单的值,来自此方法的返回值
}
```
第二步：在sentinel控制台定义授权规则，例如：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637889041222-b90a10ce-2efe-4f00-94ac-f20689607f70.webp#clientId=u44fe9ba7-102b-4&from=paste&id=u2269a459&originHeight=380&originWidth=836&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u63b85f8f-a655-4793-bf96-542f7dc75e4&title=)<br />第三步：规则定义后以后，基于ip地址，进行访问测试，检测黑白名单效果。
<a name="ksjNA"></a>
### 小节面试分析

- 如何理解Sentinel中的授权规则？(对指定资源的访问给出的一种简易的授权策略)
- Sentinel的授权规则是如何设计的？(白名单和黑名单)
- 如何理解Sentinel中的白名单？（允许访问的资源名单）
- 如何理解Sentinel中的黑名单？（不允许访问的资源名单）、
- Sentinel如何识别白名单和黑名单？(在拦截器中通过调用RequestOriginParser对象的方法检测具体的规则)
- 授权规则中RequestOriginParser类的做用是什么？（对流控应用值进行解析，检查服务访问时传入的值是否与RequestOriginParser的parseOrigin方法返回值是否相同。）
<a name="DUUqt"></a>
## 总结(Summary)
总之，Sentinel可为秒杀、抢购、抢票、拉票等高并发应用，提供API接口层面的流量限制，让突然暴涨而来的流量用户访问受到统一的管控，使用合理的流量放行规则使得用户都能正常得到服务。
<a name="GPXyA"></a>
### 重难点分析

- Sentinel诞生的背景?(计算机的数量是否有限,处理能力是否有限,并发比较大或突发流量比较大)
- 服务中Sentinel环境的集成,初始化?(添加依赖-两个,sentinel配置)
- Sentinel 的限流规则?(阈值类型-QPS&线程数,限流模式-直接,关联,链路)
- Sentinel 的降级(熔断)策略?(慢调用,异常比例,异常数)
- Sentinel 的热点规则设计(掌握)?
- Sentinel 系统规则设计?(了解,全局规则定义,针对所有请求有效)
- Sentinel 授权规则设计?(掌握,黑白名单)
<a name="ynWgg"></a>
### FAQ分析

- 为什么要限流?
- 了解的那些限流框架?(sentinel)
- 常用的限流算法有那些?(计数,令牌桶-电影票,漏桶-漏斗,滑动窗口)
- Sentinel有哪些限流规则?(QPS,线程数)
- Sentinel有哪些限流模式?(直接,关联-创建订单和查询订单,链路限流-北京六环外不限号,但是五环就限号)
- Sentinel 的降级(熔断)策略有哪些?(慢调用-响应时长,异常比例-异常占比,异常数)
- Sentinel 的热点规则中的热点数据?(热卖商品,微博大咖,新上映的电影)
- 如何理解Sentinel 授权规则中的黑白名单?
