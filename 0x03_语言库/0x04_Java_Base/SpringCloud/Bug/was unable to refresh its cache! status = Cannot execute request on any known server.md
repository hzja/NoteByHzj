<a name="sS9OO"></a>
# 1、报错如下
```java
2020-07-31 13:57:30.792 ERROR 23180 --- [freshExecutor-0] com.netflix.discovery.DiscoveryClient    : DiscoveryClient_GATEWAYZUUL/Fcant:GatewayZuul:8762 - was unable to refresh its cache! status = Cannot execute request on any known server

com.netflix.discovery.shared.transport.TransportException: Cannot execute request on any known server
	at com.netflix.discovery.shared.transport.decorator.RetryableEurekaHttpClient.execute(RetryableEurekaHttpClient.java:112) ~[eureka-client-1.9.21.jar:1.9.21]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator.getApplications(EurekaHttpClientDecorator.java:134) ~[eureka-client-1.9.21.jar:1.9.21]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator$6.execute(EurekaHttpClientDecorator.java:137) ~[eureka-client-1.9.21.jar:1.9.21]
	at com.netflix.discovery.shared.transport.decorator.SessionedEurekaHttpClient.execute(SessionedEurekaHttpClient.java:77) ~[eureka-client-1.9.21.jar:1.9.21]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator.getApplications(EurekaHttpClientDecorator.java:134) ~[eureka-client-1.9.21.jar:1.9.21]
	at com.netflix.discovery.DiscoveryClient.getAndStoreFullRegistry(DiscoveryClient.java:1081) [eureka-client-1.9.21.jar:1.9.21]
	at com.netflix.discovery.DiscoveryClient.fetchRegistry(DiscoveryClient.java:995) [eureka-client-1.9.21.jar:1.9.21]
	at com.netflix.discovery.DiscoveryClient.refreshRegistry(DiscoveryClient.java:1517) [eureka-client-1.9.21.jar:1.9.21]
	at com.netflix.discovery.DiscoveryClient$CacheRefreshThread.run(DiscoveryClient.java:1484) [eureka-client-1.9.21.jar:1.9.21]
	at java.util.concurrent.Executors$RunnableAdapter.call(Executors.java:511) [na:1.8.0_192]
	at java.util.concurrent.FutureTask.run$$$capture(FutureTask.java:266) [na:1.8.0_192]
	at java.util.concurrent.FutureTask.run(FutureTask.java) [na:1.8.0_192]
	at java.util.concurrent.ThreadPoolExecutor.runWorker(ThreadPoolExecutor.java:1149) [na:1.8.0_192]
	at java.util.concurrent.ThreadPoolExecutor$Worker.run(ThreadPoolExecutor.java:624) [na:1.8.0_192]
	at java.lang.Thread.run(Thread.java:748) [na:1.8.0_192]

```
<a name="H6ENa"></a>
# 2、问题分析及解决
<a name="NVJzG"></a>
## A.客户端指定的注册中心地址不一致导致无法注册至注册中心
<a name="5F4Q5"></a>
### 错误情况
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596175510123-c6e32c1c-fb70-4455-82d1-6486b6fd4b0b.png#align=left&display=inline&height=687&originHeight=2060&originWidth=3840&size=824384&status=done&style=shadow&width=1280)
<a name="LhbKj"></a>
### 正例情况
服务端URL
```yaml
server:
  port: 8761
eureka:
  instance:
    #eureka实例的主机名
    hostname: localhost
  client:
    register-with-eureka: false     #是否把服务注册中心注册到eureka
    fetch-registry: false           #是否从服务注册中心注册eureka获取注册信息
    service-url:
      defaultZone: http://${eureka.instance.hostname}:${server.port}/eureka/
```
客户端指定注册中心的URL
```yaml
server:
  port: 8762
eureka:
  client:
    service-url:
      # 将网关注册到注册中心
      defaultZone: http://127.0.0.1:8761/eureka/
```
<a name="pPqNF"></a>
## B.客户端与服务端都需要指定 /eureka/ 接口，否则也无法进行注册
<a name="GB2Vt"></a>
## C.服务端指定了**server.****servlet.****context-path****，而客户端没有添加该访问的接口也会无法成功注册**
<a name="BymOg"></a>
### 错误示例
**![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596175844699-f0d82683-0179-499d-84eb-a9e7d31f4ad1.png#align=left&display=inline&height=687&originHeight=2060&originWidth=3840&size=856739&status=done&style=shadow&width=1280)**
<a name="bL8wB"></a>
### 正确示例
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596175925341-362c2d11-f7d3-41f7-b8e8-9112b57128a9.png#align=left&display=inline&height=687&originHeight=2060&originWidth=3840&size=824336&status=done&style=shadow&width=1280)
