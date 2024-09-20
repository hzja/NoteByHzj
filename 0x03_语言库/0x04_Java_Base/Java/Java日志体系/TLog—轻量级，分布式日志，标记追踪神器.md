分布式日志 TLog 
<a name="1ZjFc"></a>
## TLog能解决什么痛点
随着微服务盛行，很多公司都把系统按照业务边界拆成了很多微服务，在排错查日志的时候。因为业务链路贯穿着很多微服务节点，导致定位某个请求的日志以及上下游业务的日志会变得有些困难。<br />这时候很多开发者会开始考虑上SkyWalking，Pinpoint等分布式追踪系统来解决，基于OpenTracing规范，而且通常都是无侵入性的，并且有相对友好的管理界面来进行链路Span的查询。<br />但是搭建分布式追踪系统，熟悉以及推广到全公司的系统需要一定的时间周期，而且当中涉及到链路span节点的存储成本问题，全量采集还是部分采集？如果全量采集，就以SkyWalking的存储来举例，ES集群搭建至少需要5个节点。这就需要增加服务器成本。况且如果微服务节点多的话，一天下来产生几十G上百G的数据其实非常正常。如果想保存时间长点的话，也需要增加服务器磁盘的成本。<br />当然分布式追踪系统是一个最终的解决方案，如果您的公司已经上了分布式追踪系统，那TLog并不适用。<br />TLog提供了一种最简单的方式来解决日志追踪问题，它不收集日志，也不需要另外的存储空间，它只是自动的对日志进行打标签，自动生成TraceId贯穿微服务的一整条链路。并且提供上下游节点信息。适合中小型企业以及想快速解决日志追踪问题的公司项目使用。<br />TLog适配了三大日志框架，支持自动检测适配。支持dubbo，dubbox，spring cloud三大RPC框架，更重要的是的项目接入TLog，可能连十分钟就不需要）
<a name="ZK1LT"></a>
## 项目地址
官网地址：[https://yomahub.com/tlog](https://yomahub.com/tlog)
<a name="gkAlc"></a>
## 项目特性
目前TLog的支持的特性如下：

- 通过对日志打标签完成轻量级微服务日志追踪
- 对业务代码无侵入式设计，使用简单，10分钟即可接入
- 支持常见的log4j，log4j2，logback三大日志框架，并提供自动检测，完成适配
- 支持dubbo，dubbox，springcloud三大RPC框架
- 支持日志标签的自定义模板的配置，提供多个系统级埋点标签的选择
- 提供spanId来表示本次调用在整个调用链路树中的位置
- 支持方法级别的标签自定义埋入
- 天然支持异步线程的追踪
- 几乎无性能损耗
<a name="NW5cO"></a>
## 快速开始
TLog支持了SpringBoot的自动装配，在SpringBoot环境下，只需要以下两步就可以接入！
<a name="GlOq7"></a>
### 依赖
```xml
<dependency>
  <groupId>com.yomahub</groupId>
  <artifactId>tlog-all-spring-boot-starter</artifactId>
  <version>1.1.0</version>
</dependency>
```
目前jar包已上传中央仓库，可以直接依赖到
<a name="5e6a5fb2"></a>
### 日志框架适配
只需要在启动类中加入一行代码，即可以自动进行探测项目所使用的Log框架，并进行增强。
```java
@SpringBootApplication
public class Runner {
    static {AspectLogEnhance.enhance();}//进行日志增强，自动判断日志框架
    public static void main(String[] args) {
        SpringApplication.run(Runner.class, args);
    }
}
```
因为这里是用javassist实现，需要在jvm加载对应日志框架的类之前，进行字节码增强。所以这里用static块。并且Springboot/Spring的启动类中不能加入log定义，否则会不生效。
<a name="99d40a04"></a>
### RPC框架的适配
在SpringBoot环境下，TLog会自动探测RPC框架，自动进行适配。
<a name="JKaKG"></a>
### 最终效果
只需要以上这2步，就可以把SpringBoot项目快速接入了，这里以dubbo+log4j为例。
<a name="9UYXR"></a>
#### Consumer端代码
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1614662994504-16a4def8-fc13-44ac-af97-1585f4368577.png#align=left&display=inline&height=619&originHeight=619&originWidth=1080&size=0&status=done&style=none&width=1080)
<a name="yBBB3"></a>
#### 日志打印
```
2020-09-16 18:12:56,748 [WARN] [TLOG]重新生成traceId[7161457983341056]  >> com.yomahub.tlog.web.TLogWebInterceptor:39
2020-09-16 18:12:56,763 [INFO] <7161457983341056> logback-dubbox-consumer:invoke method sayHello,name=jack  >> com.yomahub.tlog.example.dubbox.controller.DemoController:22
2020-09-16 18:12:56,763 [INFO] <7161457983341056> 测试日志aaaa  >> com.yomahub.tlog.example.dubbox.controller.DemoController:23
2020-09-16 18:12:56,763 [INFO] <7161457983341056> 测试日志bbbb  >> com.yomahub.tlog.example.dubbox.controller.DemoController:24
```
<a name="envvX"></a>
#### Provider代码
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1614662994576-3e515e88-fcce-4dd8-90fc-b6004660d7d3.png#align=left&display=inline&height=469&originHeight=469&originWidth=1080&size=0&status=done&style=none&width=1080)
<a name="o4HSA"></a>
#### 日志打印
```
2020-09-16 18:12:56,854 [INFO] <7161457983341056> logback-dubbox-provider:invoke method sayHello,name=jack  >> com.yomahub.tlog.example.dubbo.service.impl.DemoServiceImpl:15
2020-09-16 18:12:56,854 [INFO] <7161457983341056> 测试日志cccc  >> com.yomahub.tlog.example.dubbo.service.impl.DemoServiceImpl:16
2020-09-16 18:12:56,854 [INFO] <7161457983341056> 测试日志dddd  >> com.yomahub.tlog.example.dubbo.service.impl.DemoServiceImpl:17
```
可以看到，经过简单接入后，各个微服务之间每个请求有一个全局唯一的traceId贯穿其中，对所有的日志输出都能生效，这下定位某个请求的日志链就变得轻松了。
