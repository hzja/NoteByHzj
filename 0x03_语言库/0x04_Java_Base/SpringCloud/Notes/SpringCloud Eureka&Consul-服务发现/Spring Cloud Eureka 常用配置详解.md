Java SpringCloud Eureka<br />Spring Cloud Eureka 主要分为下面三个模块的参数：

- Eureka Server
- Eureka Client
- Eureka Instance
<a name="AWxjt"></a>
## Eureka Server
Eureka Server 的配置参数格式：eureka.server.xxx。
<a name="fFFRg"></a>
### `enable-self-preservation`
表示注册中心是否开启服务的自我保护能力。
<a name="hoDbM"></a>
### `renewal-percent-threshold`
表示 Eureka Server 开启自我保护的系数，默认：0.85。
<a name="qzSTi"></a>
### `eviction-interval-timer-in-ms`
表示 Eureka Server 清理无效节点的频率，默认 60000 毫秒（60 秒）。<br />更多 Eureka Server 参数配置可以看一下这个类：<br />org.springframework.cloud.netflix.eureka.server.EurekaServerConfigBean
<a name="C6imm"></a>
## Eureka Instance
Eureka Instance 的配置参数格式：eureka.instance.xxx。
<a name="WzVj0"></a>
### `instance-id`
表示实例在注册中心注册的唯一ID。
<a name="m9O4m"></a>
### `prefer-ip-address`

- true：实例以 IP 的形式注册
- false：实例以机器 HOSTNAME 形式注册
<a name="ddAwz"></a>
### `lease-expiration-duration-in-seconds`
表示 Eureka Server 在接收到上一个心跳之后等待下一个心跳的秒数（默认 90 秒），若不能在指定时间内收到心跳，则移除此实例，并禁止此实例的流量。

- 此值设置太长，即使实例不存在，流量也能路由到该实例
- 此值设置太小，由于网络故障，实例会被取消流量

需要设置为至少高于 lease-renewal-interval-in-seconds 的值，不然会被误移除了。
<a name="EHt9r"></a>
### `lease-renewal-interval-in-seconds`
表示 Eureka Client 向 Eureka Server 发送心跳的频率（默认 30 秒），如果在 lease-expiration-duration-in-seconds 指定的时间内未收到心跳，则移除该实例。<br />更多 Eureka Instance 参数配置可以看一下这个类：<br />org.springframework.cloud.netflix.eureka.EurekaInstanceConfigBean
<a name="AMPj1"></a>
## Eureka Client
Eureka Client 的配置参数格式：eureka.client.xxx。
<a name="De8Zw"></a>
### `register-with-eureka`
表示此实例是否注册到 Eureka Server 以供其他实例发现。在某些情况下，如果你不想自己的实例被发现，而只想发现其他实例，配置为 false 即可。
<a name="AbSLp"></a>
### `fetch-registry`
表示客户端是否从 Eureka Server 获取实例注册信息。
<a name="gfT6C"></a>
### `serviceUrl.defaultZone`
表示客户端需要注册的 Eureka Server 的地址。<br />更多 Eureka Client 参数配置可以看一下这个类：<br />org.springframework.cloud.netflix.eureka.EurekaClientConfigBean
<a name="QwdT9"></a>
## 用到的其他参数
<a name="u0gq2"></a>
### `spring.application.name`
表示应用名称，在注册中心中显示的服务注册名称。
<a name="CzuW9"></a>
### `spring.cloud.client.ip-address`
获取客户端的 IP 地址。<br />上面讲的 Eureka 某些参数可以在 Eureka 控制台上面找到。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1640415439225-e077e24e-bdd2-4c25-bb76-c05cb2de6b87.png#clientId=u1b82c353-2692-4&from=paste&height=594&id=u931423b9&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=229067&status=done&style=shadow&taskId=uadb83da5-06d5-445e-a88b-82edcc6761f&title=&width=1280)<br />Eureka 控制台上面的其他参数都可以定制。
