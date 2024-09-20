JavaSpringCloud Alibaba Nacos
<a name="dPM2O"></a>
## 配置中心
业务上的配置，功能开关，服务治理上对弱依赖的降级，甚至数据库的密码等，都可能用到动态配置中心。<br />在没有专门的配置中心组件时，使用硬编码、或配置文件、或数据库、缓存等方式来解决问题。<br />硬编码修改配置时需要重新编译打包，配置文件需要重启应用，数据库受限于性能，缓存丧失了及时性。<br />可能都不完美，但能从中总结出配置中心的需求，相对来说还是比较明确：

- 能够存储、获取并监听配置（必须）
- 配置变更时能及时推送给监听者（必须）
- 有一个可视化的查看变更配置的控制台（必须）
- 配置变更可灰度执行（加分）
- 配置变更可回滚（加分）

目前使用最多的配置中心可能是携程开源的Apollo，还有Spring Cloud Config、阿里开源的Nacos、百度的Disconf等。
<a name="p5PRP"></a>
## Nacos配置中心
Nacos是Naming and Configuration Service的缩写，从名字上能看出它重点关注的两个领域是Naming即注册中心和Configuration配置中心。<br />本文讲解nacos的配置中心的架构设计和实现原理，基于2.0.0版本（注：2.0.0版本与1.x版本区别较大）
<a name="p6V6y"></a>
## Nacos调试环境搭建

- 先从github上clone代码（网速比较慢，加上`--depth=1`参数）
```bash
git clone --depth=1 https://github.com/alibaba/nacos.git
```

- 导入IDE，看代码，调试更方便
- 启动Server端：运行console模块下的Nacos.main()，这个类扫描的路径最广，能启动所有的模块
   - JVM参数可带上`-Dnacos.standalone=true -Dnacos.functionMode=config`，指定单机模式，且只启动config模块
   - `--spring.config.additional-location=nacos/distribution/conf`，程序参数指定配置文件目录
   - 正常启动，console打印出Ncos控制台地址，进入Nacos控制台，输入用户名密码（默认均为nacos）即可登录

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657026968750-135f9822-609a-42b4-9c14-cadb79528e47.png#clientId=ue27dc94a-95e9-4&from=paste&id=u7911a3d2&originHeight=354&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uef74eab8-5059-413f-a751-bc29c20fdf9&title=)

- 使用client进行测试，example模块下有configExample可进行config的测试，为了不动源代码，可copy一份configExample进行修改测试
<a name="mt9Nc"></a>
## Nacos配置模型
namespace + group + dataId  唯一确定一个配置

- namespace：与client绑定，一个clinet对应到一个namespace，可用来隔离环境或区分租户
- group：分组，区分业务
- dataId：配置的id

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657026968748-8c6f2df5-fe97-44ad-83c3-123e5ae8506e.png#clientId=ue27dc94a-95e9-4&from=paste&id=u37fae2b6&originHeight=527&originWidth=577&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4965d113-29f0-475e-b050-9b24550b1eb&title=)
<a name="x4OKz"></a>
## 客户端启动流程
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657026968872-629e58be-f14c-4423-be7c-17e4425405e0.png#clientId=ue27dc94a-95e9-4&from=paste&id=ub1bc9a32&originHeight=366&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4c244b5a-8dc2-4dbc-8f0f-ade1e86c8e9&title=)

- 参数准备时，如果配置了nacos服务端地址，则直接使用；如果配置了endpoint，则从endpoint中获取nacos服务端地址，这样有个好处是服务端地址变更，扩缩容都无需重启client，更详细可参考[https://nacos.io/en-us/blog/namespace-endpoint-best-practices.html](https://nacos.io/en-us/blog/namespace-endpoint-best-practices.html)
- 在客户端第一次与服务端交互时创建GRPC连接，随机挑选一台server建立连接，后续都使用该连接，请求失败都会有重试，针对请求级别也有限流；重试失败或者服务端主动断开连接，则会重新挑选一台server进行建链
<a name="NgEFJ"></a>
## 请求模型
从gRPC的proto文件能看出请求与返回的定义比较统一
```protobuf
message Metadata {
  string type = 3;
  string clientIp = 8;
  map<string, string> headers = 7;
}

message Payload {
  Metadata metadata = 2;
  google.protobuf.Any body = 3;
}

service Request {
  // Sends a commonRequest
  rpc request (Payload) returns (Payload) {
  }
}
```

- type是请求/返回类的类名
- clientIp是客户端ip
- headers是携带的header信息
- Playload中的body以json格式编码

在com.alibaba.nacos.api.config.ConfigService中可以找到所有配置中心能使用的接口<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657026968775-31dc0fb5-9df9-4f47-ac55-9b33eed2a893.png#clientId=ue27dc94a-95e9-4&from=paste&id=u33b50f08&originHeight=490&originWidth=1006&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc64a4e81-edc6-44e7-875b-83418a0e4de&title=)<br />重点关注这几个接口：

- `getConfig`：读取配置
- `publishConfig`：发布配置
- `publishConfigCas`：原子的发布配置，若有被其他线程变更则发布失败，类似java中的CAS
- `removeConfig`：删除配置
- `addListener`：监听配置
- `removeListener`：移除配置的监听
<a name="gTLT7"></a>
## 变更推送
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657026968703-e7b9ac64-396e-4346-9295-6f2e71ef40f7.png#clientId=ue27dc94a-95e9-4&from=paste&id=u367152a9&originHeight=721&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub213abd2-119e-4bfe-ade0-ab2b14fec37&title=)<br />采取推拉结合的方式，既保证时效性，又保证数据一致性
<a name="xQkSR"></a>
## 数据存储
Nacos配置中心的数据存储支持内嵌的derby数据库，也支持外部数据库mysql，内嵌数据库主要是为了单机测试时使用。<br />其中上文提及的`publishConfigCas`的实现是利用了数据库`update ${table} set ${xx}=${zz} where md5=${old_md5}`来实现，如果已经这条数据被变更，则这次publish会失败。
<a name="Yj7RF"></a>
## 灰度和回滚
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657026969155-d7534938-07d8-4da6-9fd2-f49f1ed58b45.png#clientId=ue27dc94a-95e9-4&from=paste&id=u524dee6d&originHeight=562&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4f45a45f-aec7-4a6a-8f6c-6a88b0b3b1c&title=)<br />当勾选灰度发布时可填写灰度的ip进行推送，不在灰度列表内的ip则不会接受到变更推送，并且灰度和正式是区分开的。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657026969241-d06ed2f6-0da6-4d3f-9a2e-234352da5cb2.png#clientId=ue27dc94a-95e9-4&from=paste&id=u74627553&originHeight=578&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7accf597-2410-4bfa-9120-e0a8785bbce&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657026969164-e846ec7d-c452-4314-b3a4-d2a6d61d40a0.png#clientId=ue27dc94a-95e9-4&from=paste&id=uaaed1b43&originHeight=442&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2beecf2d-499f-4786-81d7-3cd5ce8e917&title=)灰度的实现是记录下了每次的发布，回滚到指定版本即可。<br />配置中心的核心是配置的创建、读取、推送。<br />注册中心的核心比配置中心多一个服务探活模块，他俩的相似度非常高，甚至阿里内部的注册中心就叫ConfigServer。<br />Nacos注册中心打算分成几个模块来分析，本文重点在于概要设计，基于2.0.0版本。
<a name="AlFTO"></a>
## 环境搭建
用Nacos的源码来搭建源码阅读和调试环境。<br />其中 JVM参数可以指定只启动Naming模块，也可以不指定，默认全都启动。<br />example模块下将NamingExample复制一份进行测试。
<a name="ecVtj"></a>
## 服务发现模型
客户端视角的服务发现模型（注意：服务端视角的模型定义与客户端视角有区别）包含以下几点内容：

- Service：服务
- Cluster：集群
- Instance：实例

代码注释：We introduce a 'service --> cluster --> instance' model, in which service stores a list of clusters, which contains a list of instances<br />他们的关系如下<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657026866521-b656c524-fdc5-4668-a9f3-d27afe0a13e6.png#clientId=ue27dc94a-95e9-4&from=paste&id=u1f098af7&originHeight=589&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u03866c2f-77ad-4a34-80e7-0b89ea5b5c7&title=)
<a name="GYOzV"></a>
### Service
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657026866480-a8f1e020-31c2-462f-b9b0-dbffa8096938.png#clientId=ue27dc94a-95e9-4&from=paste&id=u1f157735&originHeight=440&originWidth=708&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5bcbd7b2-4024-4e7f-a3c8-16ec2e8d45f&title=)

- name：服务名
- protectThreshold：保护阈值，限制了实例被探活摘除的最大比例
- appName：服务的应用名，暂无实际用处
- groupName：分组名
- metadata：元数据
<a name="Osmzh"></a>
### Cluster
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657026866392-487279d4-7448-4a7a-92dd-ab895b25dc12.png#clientId=ue27dc94a-95e9-4&from=paste&id=u5566f244&originHeight=456&originWidth=756&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub7c3f0df-1c4a-48f3-b47d-4e717a2f0e8&title=)

- serviceName：所属服务名
- name：集群名
- healthChecker：服务探活配置，此处仅对服务端主动探活生效，有TCP、HTTP、MySQL、None几种方式，默认TCP
- defaultPort：默认端口
- defaultCheckPort：默认探活端口
- useIPPort4Check：是否使用port进行探活
- metadata：元数据
<a name="XyFbY"></a>
### Instance
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657026866403-89ba01e7-f808-4a38-af5f-e4dccad1765c.png#clientId=ue27dc94a-95e9-4&from=paste&id=u80cbd3c5&originHeight=864&originWidth=782&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud145e0ad-247c-44e1-a683-3396c926b6a&title=)

- instanceId：实例id，唯一标志，Nacos提供了simple和snowflake两种算法来生成，默认是simple，其生成方式为ip#port#clusterName#serviceName
- ip：实例ip
- port：实例port
- weight：实例权重
- healthy：实例健康状态
- clusterName：所属集群名
- serviceName：所属服务名
- metadata：元数据
- enabled：是否接收请求，可用于临时禁用或摘流等场景
- ephemeral：是否为临时实例，后文会介绍该参数

---

- `getInstanceHeartBeatInterval`：获取实例心跳上报间隔时间，默认5秒，可配置
- `getInstanceHeartBeatTimeOut`：获取心跳超时时间，15秒，配置
- `getIpDeleteTimeout`：获取ip被删除的超时时间，默认30秒，可配置
- `getInstanceIdGenerator`：获取id生成器

除了上述的三层模型外，Nacos注册中心和配置中心有着一样的namespace设计，与client绑定，可隔离环境，租户。
<a name="gttia"></a>
## 接口设计

- `registerInstance`：注册实例
- `deregisterInstance`：注销实例
- `getAllInstances`：获取一个服务的所有实例（包括不健康）
- `selectInstances`：根据条件获取一个服务的实例
- `selectOneHealthyInstance`：根据负载均衡策略获取服务的一个健康的实例
- `subscribe`：订阅服务
- `unsubscribe`：取消订阅服务
- `getServicesOfServer`：根据条件分页获取所有服务
<a name="HHxlX"></a>
## 交互流程
Nacos 2.0 为ephemeral不同的实例提供了两套流程：

- ephemeral=false，永久实例，与server端的交互采用http请求，server节点间数据同步采用了raft协议，健康检查采用了server端主动探活的机制
- ephemeral=true，临时实例，与server端的交互采用grpc请求，server节点间数据同步采用了distro协议，健康检查采用了TCP连接的KeepAlive模式
<a name="xM7Li"></a>
### 临时实例的交互流程

- client初始化，与server建立连接
   - 只与其中一台server节点建立长连接
- client 注册服务，将serviceName+ip+port+clusterName等数据打包发送grpc请求
   - 同时客户端缓存已注册过的服务，当client与server连接断开重连时，client重新将这些数据注册到server端
- server端接收到client的注册请求，将注册信息存入client对象（用于保存client的所有数据）中，并触发`ClientChangedEvent`、`ClientRegisterServiceEvent`、`InstanceMetadataEvent`
   - `ClientChangedEvent`触发server节点之间的数据同步（distro协议）
   - `ClientRegisterServiceEvent`触发更新publisherIndexes（保存service => clientId的Map<Service, Set>，即哪些客户端注册了这个服务的索引），同时也触发一个`ServiceChangedEvent`，该事件负责向监听该服务的客户端进行推送
   - `InstanceMetadataEvent`，处理元数据，Nacos在2.0中将元数据与基础数据拆分开，分为不同的处理流程
- client订阅服务
   - 根据serviceName、groupName、clusters信息生成key，创建eventListener，同时向server端发送订阅请求，并缓存订阅信息，用于连接断开重连后再次向server端发送信息
- server端接收到client的订阅请求
   - 将订阅信息打包为subscribers，并存入client对象中，触发`ClientSubscribeServiceEvent`事件
   - `ClientSubscribeServiceEvent`事件更新`subscriberIndexes`（保存service => clientId的`Map<Service, Set>`，即哪些客户端订阅了这个服务的索引），同时触发`ServiceSubscribedEvent`事件
   - `ServiceSubscribedEvent`事件会延时500ms向该client推送该服务的最新数据
- 反向的操作如注销、取消订阅与正向操作类似，不再赘述
<a name="j3jei"></a>
## 最后
本文从总体上分析了Nacos 2.0的模型设计、接口设计以及交互流程，读完后对Nacos的服务发现有一个整体上的认识。后续篇幅会从细节入手，如dubbo Nacos扩展、一致性协议、探活、CMDB扩展等逐一进行分析。
