Java
<a name="cp6YQ"></a>
## 一、Get请求自动转化成POST的问题
<a name="GwON4"></a>
### 1、client 请求参数没有加上 `@RequestParam` 注解
<a name="DZQkc"></a>
#### 问题代码：
```java
@GetMapping("/showName")
String showName(String name);
```
<a name="apZAP"></a>
#### 错误提示：
```json
[{"timestamp":"2022-12-14T09:46:22.370+00:00","status":405,"error":"Method Not Allowed","path":"/showName"}]
```
<a name="rFWkp"></a>
#### 修改建议：
在对应的请求参数上加上 `@RequestParam` 注解，代码示例如下所示：
```java
@GetMapping("/showName")
String showName(@RequestParam("name") String name);
```
<a name="YZNQZ"></a>
### 2、client 请求参数中使用了 `@RequestBody` 注解
<a name="tiwCm"></a>
#### 问题代码：
```java
@GetMapping("/showBody")
String showBody(@RequestBody String body);
```
<a name="lxZYV"></a>
#### 错误提示：
```json
[{"timestamp":"2022-12-14T10:04:08.943+00:00","status":405,"error":"Method Not Allowed","path":"/showBody"}]
```
<a name="S04E5"></a>
#### 原因：
okhttp 和 `HttpURLConnection` 不支持 `@RequestBody` + `RequestMethod.GET`，只有 httpclient 支持，默认使用 `HttpURLConnection`。
<a name="YxEPV"></a>
#### 修改建议：
1、最佳建议：要求提供方，更改为Post请求<br />2、使用 httpclient<br />引入 httpclient 包
```xml
<dependency>
  <groupId>io.github.openfeign</groupId>
  <artifactId>feign-httpclient</artifactId>
</dependency>
```
修改配置：
```
feign.httpclient.enabled = true
```
<a name="qikCt"></a>
## 二、Feign Hystrix 熔断、线程使用坑点
hystrx 官方配置解释地址：[https://github.com/Netflix/Hystrix/wiki/Configuration](https://github.com/Netflix/Hystrix/wiki/Configuration)
<a name="iGQM2"></a>
### 线程池队列配置问题
常用配置：
```
# 核心线程池大小，默认值为：10
hystrix.threadpool.default.coreSize = 10

# 调用超时时间，默认值为 1000ms
hystrix.command.default.execution.isolation.thread.timeoutInMilliseconds = 15000

# 最大线程池大小，这是在不开始拒绝的情况下可以支持的最大并发量。默认值为10。
hystrix.threadpool.default.maximumSize = 50

# 队列大小拒绝阈值，默认值为 5。即使 maxQueueSize 未达到也会发生拒绝。在 maxQueueSize == -1 时不生效。
hystrix.threadpool.default.queueSizeRejectionThreshold = 100

# maximumSize 配置是否生效，默认值为 false。maximumSize 可以等于或高于 coreSize。
# 设置 coreSize < maximumSize 创建一个可以维持maximumSize并发性的线程池；
# 但会在相对不活动期间将线程返回给系统。（受限于keepAliveTimeInMinutes）。
hystrix.threadpool.default.allowMaximumSizeToDivergeFromCoreSize = true

# 最大队列大小，默认值为 -1。值为-1时：使用 thenSynchronousQueue；值为正值时：使用 LinkedBlockingQueue。
hystrix.threadpool.default.maxQueueSize = 50
```
<a name="zmiu6"></a>
#### 错误配置①：
```
hystrix.command.default.execution.isolation.thread.timeoutInMilliseconds=1000
```
feign使用的是懒加载，第一次调用时，会初始化各种bean，速度很慢，默认1秒很容易超时。
<a name="oHq2r"></a>
#### 错误配置②：
```
hystrix.threadpool.default.coreSize = 10
hystrix.threadpool.default.maxQueueSize = 1000
hystrix.threadpool.default.queueSizeRejectionThreshold = 20
```
因为 `queueSizeRejectionThreshold` 太小，实际上在并发达到 30 以上的时候，就会拒绝后面的请求了。
<a name="fkLua"></a>
#### 错误配置③：
```
hystrix.threadpool.default.coreSize = 10
hystrix.threadpool.default.maxQueueSize = 20
hystrix.threadpool.default.queueSizeRejectionThreshold = 1000
```
因为 `maxQueueSize` 太小，实际上在并发达到 30 以上的时候，就会拒绝后面的请求了。
