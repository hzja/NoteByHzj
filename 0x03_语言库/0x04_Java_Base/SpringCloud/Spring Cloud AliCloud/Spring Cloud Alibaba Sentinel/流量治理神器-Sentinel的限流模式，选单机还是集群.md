Java SpringCloud Sentinel <br />Sentinel的限流模式分为两种，分别是单机模式和集群模式。来学习下这两种模式的区别和使用场景。
<a name="d4HVo"></a>
## 单机流控
单机流控就是流控的效果只针对服务的一个实例，比如你的服务部署了三个实例分别在三台机器上。请求访问到了A实例的时候，如果触发了流控，那么只会限制A实例后面的请求，不会影响其他实例上的请求。

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1634482491480-bf44c1ea-3820-4124-adcf-deab116e719d.webp#clientId=u0d683ff8-cf88-4&from=paste&id=u161b0a60&originHeight=531&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=uae0bb7dd-2fea-4fe8-9513-6e5fed4ab65)<br />比如你单身的时候，每月的工资都花个精光。影响的只是你自己，跟别人没关系，因为你本来就是一个人生活，单身跟单机就强行关联在一起了。<br />单机流控相对来说比较简单，不依赖中心化的存储。每个服务内部只需要记录自身的一些访问信息即可判断出是否需要流控操作。<br />像Guava的`RateLimiter`就是典型的单机流控模式，将令牌数据全部存储在本地内存中，不需要有集中式的存储，不需要跟其他服务交互，自身就能完成流控功能。
<a name="JDOco"></a>
## 集群流控
集群流控就是流控的效果针对整个集群，也就是服务的所有的实例，比如服务部署了三个实例分别在三台机器上。总体限流QPS为100，请求访问到了A实例的时候，如果触发了流控，那么此时其他的请求到B实例的时候，也会触发流控。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1634482491502-9374d7c2-de61-48d0-a727-83c5739078ed.webp#clientId=u0d683ff8-cf88-4&from=paste&id=ua9e7a2b2&originHeight=815&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=uad0dd88c-43f8-479c-8afc-b3d685c182a)<br />比如你结婚后，你和你老婆的工资是你们整个家庭的总收入。你每天出去玩，到处乱花钱，把钱花完了，你老婆想买衣服的时候，就被流控了，因为没钱了，这就是集群流控的含义。
<a name="OhBTm"></a>
## 使用场景对比
<a name="sgqvL"></a>
### 保护层面对比
单机流控更适合作为兜底保护的一种方式，比如单机限流总的请求量为2000，如果超过2000开始限流，这样就能保证当前服务在可承受的范围内进行处理。<br />如果用的是集群限流，假设当前集群内有10个节点，如果每个节点能承受2000的请求，那么加起来就是2万的请求。也就是说只要不超过2万个请求都不会触发限流。如果负载均衡策略是轮询的话没什么问题，请求分布到各个节点上都比较均匀。但是如果负载均衡策略不是轮询，如果是随机的话，那么请求很有可能在某个节点上超过2000，这个时候其实这个节点是处理不了那么多请求的，最终会被拖垮，造成连锁反应。
<a name="CyZgs"></a>
### 准确度对比
比如需求是限制总的请求次数为2000，如果是单机流控，那么也就是每个节点超过200就开始限流。还是前面的问题，如果请求分配不均匀的话，其实整体总量还没达到2000，但是某一个节点超过了200，就开始限流了，对用户体验不是很好。<br />所以集群限流适合用在有整体总量限制的场景，比如开放平台的API调用。
<a name="kGqJ6"></a>
## Sentinel集群流控
Sentinel里面集群限流有两种身份：<br />Token Client：集群流控客户端，会向 Token Server 请求 token。集群限流服务端会返回结果，告诉客户端是否限流。<br />Token Server：集群流控服务端，处理来自 Token Client 的请求，根据配置的集群规则判断是否应该允许通过。<br />在部署方面也分为两种形式：
<a name="ddixh"></a>
### 独立部署
首先来看下独立部署的架构图，此图来自Sentinel文档：

<a name="QdmEz"></a>
## ![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1634482491992-df843578-da36-4f40-9091-f5699e4f14f4.webp#clientId=u0d683ff8-cf88-4&from=paste&id=u0249ed98&originHeight=498&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=u36f755a0-95be-4389-8191-5312115b9a4)
就是单独部署一个Token Server，通过这个独立的Token Server来存储所有资源的访问数据，然后再根据配置的规则决定某个资源能否放行，是否需要执行限流操作。<br />因为集群流控必须要有一个中心去存储数据，所以也必须单独部署Token Server，单独部署隔离性好，但是整体架构的复杂度上去了。<br />另外还有一个必须要考虑的问题就是Token Server的高可用性。如果当前的Token Server挂掉了，需要有另一个节点立马接替，其实就是需要实现一个选举的功能。<br />如果Token Server部署多个节点，也给这些节点划分主，从的区别，实现了故障选举的逻辑。但还有另外一个问题需要考虑，就是节点之前的数据需要同步吧，不然故障切换后，新上任的节点没有之前的数据，就只能从零开始了。<br />数据同步如果用AP的形式，那么可以参考Eureka的双向同步机制。一但发生故障切换，还是会有极小的概念丢失数据，因为不是强一致性。<br />如果用CP的形式，那么可以参考Zookeeper里的数据一致性保证方式。
<a name="VB6ZT"></a>
### 嵌入式部署
首先来看下嵌入式部署的架构图，此图来自Sentinel文档：

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1634482491993-4a63fadd-fe0e-4ef5-9c5c-36e57263cef8.webp#clientId=u0d683ff8-cf88-4&from=paste&id=u748ee3b5&originHeight=542&originWidth=1038&originalType=url&ratio=1&status=done&style=shadow&taskId=u89958cd3-3093-4427-b750-16e2098a21c)<br />嵌入式部署跟独立部署的区别在于可以不用单独部署Token Server，而且将Token Server跟应用融合在一起，所以叫内嵌式。<br />嵌入式部署如果是Token Server的那个应用实例挂了，可以通过API将另一个应用切换成Token Server，但是这个动作一定要做成自动的，手动的就比较LOW。<br />另外不足的就是隔离性不好，虽然架构简单。Token Server和应用耦合在一起，如果此时应用的QPS很高，则势必会影响Token Server，反之也是一样。
<a name="GaI8T"></a>
## 总结
集群流控能够精确地控制整个集群的 QPS，结合单机限流兜底，可以更好地发挥流量控制的效果。<br />集群模式有一定的适用场景，同时采用集群模式对整个架构的复杂度也会提高，所以如果没有特别复杂的场景，建议大家直接用单机模式就行了，限流的阀值配置少一点，在压测极限的70%即可。
