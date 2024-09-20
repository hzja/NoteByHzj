Java SpringCloud Eureka<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640415543219-fc416acb-3375-4cf8-9042-da06247e7e0a.webp#clientId=ued790134-9090-4&from=paste&id=ucea19a73&originHeight=561&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=udcba8b47-8d45-4d77-bbea-448a2b631b8&title=)
<a name="ULxvx"></a>
## 自我保护背景
首先对Eureka注册中心需要了解的是Eureka各个节点都是平等的，没有ZK中角色的概念， 即使N-1个节点挂掉也不会影响其他节点的正常运行。<br />默认情况下，如果Eureka Server在一定时间内（默认90秒）没有接收到某个微服务实例的心跳，Eureka Server将会移除该实例。但是当网络分区故障发生时，微服务与Eureka Server之间无法正常通信，而微服务本身是正常运行的，此时不应该移除这个微服务，所以引入了自我保护机制。
<a name="TpM4u"></a>
## 自我保护机制
官方对于自我保护机制的定义：<br />[https://github.com/Netflix/eureka/wiki/Understanding-Eureka-Peer-to-Peer-Communication](https://github.com/Netflix/eureka/wiki/Understanding-Eureka-Peer-to-Peer-Communication)<br />自我保护模式正是一种针对网络异常波动的安全保护措施，使用自我保护模式能使Eureka集群更加的健壮、稳定的运行。<br />自我保护机制的工作机制是如果在15分钟内超过85%的客户端节点都没有正常的心跳，那么Eureka就认为客户端与注册中心出现了网络故障，Eureka Server自动进入自我保护机制，此时会出现以下几种情况：<br />1、Eureka Server不再从注册列表中移除因为长时间没收到心跳而应该过期的服务。<br />2、Eureka Server仍然能够接受新服务的注册和查询请求，但是不会被同步到其它节点上，保证当前节点依然可用。<br />3、当网络稳定时，当前Eureka Server新的注册信息会被同步到其它节点中。<br />因此Eureka Server可以很好的应对因网络故障导致部分节点失联的情况，而不会像ZK那样如果有一半不可用的情况会导致整个集群不可用而变成瘫痪。
<a name="R4xli"></a>
## 自我保护开关
Eureka自我保护机制，通过配置 `eureka.server.enable-self-preservation`来true打开/false禁用自我保护机制，默认打开状态，建议生产环境打开此配置。
<a name="nREGG"></a>
## 开发环境配置
开发环境中如果要实现服务失效能自动移除，只需要修改以下配置。
<a name="g3KU4"></a>
### 1、 注册中心关闭自我保护机制，修改检查失效服务的时间。
```yaml
eureka:
  server: 
    enable-self-preservation: false
    eviction-interval-timer-in-ms: 3000
```
<a name="jkuPK"></a>
### 2、 微服务修改减短服务心跳的时间。
```yaml
# 默认90秒
lease-expiration-duration-in-seconds: 10
# 默认30秒
lease-renewal-interval-in-seconds: 3
```
以上配置建议在生产环境使用默认的时间配置。
<a name="bAqqe"></a>
## 自我保护机制实战分析
现在把保护机制开启：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1640415903090-c898972f-28c3-4a0d-bc99-dafa96ce9dc6.png#clientId=ua2b379e4-c598-4&from=paste&height=281&id=u3027f1f4&originHeight=844&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=132148&status=done&style=none&taskId=uae0419a0-429d-4069-975f-2e7abbe7a94&title=&width=1280)<br />关注右上角的两个重要参数：

| 参数 | 说明 |
| --- | --- |
| Renews threshold | Eureka Server 期望每分钟收到客户端实例的总心跳数 |
| Renews (last min) | Eureka Server 最后一分钟收到的总心跳数 |

这里显示的数值如下：
```
Renews threshold    6
Renews (last min)    8
```
**这个 6 和 8 分别是怎么算出来的？**<br />先来看这两个参数的默认设置：<br />`eureka.server.renewal-percent-threshold`：<br />表示 Eureka Server 开启自我保护的系数，默认：0.85。<br />`eureka.instance.lease-renewal-interval-in-seconds`：<br />表示 Eureka Client 向 Eureka Server 发送心跳的频率（默认 30 秒），如果在 `lease-expiration-duration-in-seconds` 指定的时间内未收到心跳，则移除该实例。<br />这里有 4 个注册实例，保护系数：0.85，心跳频率：30秒（每分钟两次），计算公式如下：
```
Renews threshold = 4 * 2 * 0.85 = 6.8（取整为：6）
Renews (last min) = 4 * 2 = 8
```
现在删除一个配置中心实例测试一下：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1640415953410-1d02592d-2d5f-409a-8342-cb551094e722.png#clientId=ua2b379e4-c598-4&from=paste&height=282&id=uffacbc7f&originHeight=847&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=132128&status=done&style=none&taskId=u5b36cc46-e340-4376-97a9-9b04787513f&title=&width=1280)<br />出现警告：<br />**EMERGENCY! EUREKA MAY BE INCORRECTLY CLAIMING INSTANCES ARE UP WHEN THEY'RE NOT. RENEWALS ARE LESSER THAN THRESHOLD AND HENCE THE INSTANCES ARE NOT BEING EXPIRED JUST TO BE SAFE.**<br />说明现在 Eureka Server 已经进行保护模式了，并且删除的那个实例并不会从注册列表中移除，保护机制生效成功。<br />**由此可知：**Eureka Server 在一分钟内如果没有收到 6 个以上的心跳，即：Renews threshold >= Renews (last min)，如果保护机制已开启的情况下，则会开启保护机制。<br />为什么移除一个实例后，Renews threshold 还是 6 呢，算出来应该是 5，这是因为 Eureka Server 还没刷新这个值，默认 15 分钟刷新一次，可以通过设计以下值进行调整：
```
eureka.server.renewal-threshold-update-interval-ms=900000
```
了解了心跳策略和保护机制后，对注册中心的日常维护就会有很多帮助。
