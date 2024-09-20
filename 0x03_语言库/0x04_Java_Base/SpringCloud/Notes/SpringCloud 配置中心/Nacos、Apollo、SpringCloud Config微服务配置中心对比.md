SpringCloud Nacos Apollo 微服务
<a name="kUOfe"></a>
## 为什么需要配置中心
<a name="kNgk5"></a>
### 配置实时生效：
传统的静态配置方式要想修改某个配置只能修改之后重新发布应用，要实现动态性，可以选择使用数据库，通过定时轮询访问数据库来感知配置的变化。轮询频率低感知配置变化的延时就长，轮询频率高，感知配置变化的延时就短，但比较损耗性能，需要在实时性和性能之间做折中。配置中心专门针对这个业务场景，兼顾实时性和一致性来管理动态配置。
<a name="o6348"></a>
### 配置管理流程：
配置的权限管控、灰度发布、版本管理、格式检验和安全配置等一系列的配置管理相关的特性也是配置中心不可获取的一部分。
<a name="cHa4V"></a>
## 开源配置中心基本介绍
目前市面上用的比较多的配置中心有：（按开源时间排序）
<a name="NeLD3"></a>
### Disconf
2014年7月百度开源的配置管理中心，同样具备配置的管理能力，不过目前已经不维护了，最近的一次提交是两年前了。
<a name="UGcXu"></a>
### Spring Cloud Config
2014年9月开源，Spring Cloud 生态组件，可以和Spring Cloud体系无缝整合。
<a name="CNfai"></a>
### Apollo
2016年5月，携程开源的配置管理中心，具备规范的权限、流程治理等特性。
<a name="Bwrax"></a>
### Nacos
2018年6月，阿里开源的配置中心，也可以做DNS和RPC的服务发现。
<a name="RnVYR"></a>
## 配置中心核心概念的对比
由于Disconf不再维护，下面对比一下Spring Cloud Config、Apollo和Nacos。<br />Spring Cloud Config、Apollo和Nacos在配置管理领域的概念基本相同，但是也存在一些不同的点，使用配置的过程中会涉及到一些比较重要的概念。
<a name="oljK1"></a>
### 应用
应用是客户端系统的基本单位，Spring Cloud Config 将应用名称和对应Git中的文件名称关联起来了，这样可以起到多个应用配置相互隔离的作用。Apollo的配置都是在某个应用下面的（除了公共配置），也起到了多个应用配置相互隔离的作用。Nacos的应用概念比较弱，只有一个用于区分配置的额外属性，不过可以使用 Group 来做应用字段，可以起到隔离作用。
<a name="TjPA8"></a>
### 集群
不同的环境可以搭建不同的集群，这样可以起到物理隔离的作用，Spring Cloud Config、Apollo、Nacos都支持多个集群。
<a name="wEqfv"></a>
### Label Profile & 环境 & 命名空间
Spring Cloud Config可以使用Label和Profile来做逻辑隔离，Label指远程仓库的分支，Profile类似Maven Profile可以区分环境，比如{application}-{profile}.properties。<br />Nacos的命名空间和Apollo的环境一样，是一个逻辑概念，可以作为环境逻辑隔离。Apollo中的命名空间指配置的名称，具体的配置项指配置文件中的一个Property。
<a name="jIMXV"></a>
## 配置管理功能的对比
作为配置中心，配置的整个管理流程应该具备流程化能力。
<a name="azslq"></a>
### 灰度发布
配置的灰度发布是配置中心比较重要的功能，当配置的变更影响比较大的时候，需要先在部分应用实例中验证配置的变更是否符合预期，然后再推送到所有应用实例。<br />Spring Cloud Config支持通过/bus/refresh端点的destination参数来指定要更新配置的机器，不过整个流程不够自动化和体系化。<br />Apollo可以直接在控制台上点灰度发布指定发布机器的IP，接着再全量发布，做得比较体系化。<br />Nacos目前发布到0.9版本，还不支持灰度发布。
<a name="v6eKI"></a>
### 权限管理
配置的变更和代码变更都是对应用运行逻辑的改变，重要的配置变更常常会带来核弹的效果，对于配置变更的权限管控和审计能力同样是配置中心重要的功能。<br />Spring Cloud Config依赖Git的权限管理能力，开源的GitHub权限控制可以分为Admin、Write和Read权限，权限管理比较完善。<br />Apollo通过项目的维度来对配置进行权限管理，一个项目的owner可以授权给其他用户配置的修改发布权限。<br />Nacos目前看还不具备权限管理能力。
<a name="dQE03"></a>
### 版本管理&回滚
当配置变更不符合预期的时候，需要根据配置的发布版本进行回滚。Spring Cloud Config、Apollo和Nacos都具备配置的版本管理和回滚能力，可以在控制台上查看配置的变更情况或进行回滚操作。Spring Cloud Config通过Git来做版本管理，更方便些。
<a name="CtfmJ"></a>
### 配置格式校验
应用的配置数据存储在配置中心一般都会以一种配置格式存储，比如Properties、Json、Yaml等，如果配置格式错误，会导致客户端解析配置失败引起生产故障，配置中心对配置的格式校验能够有效防止人为错误操作的发生，是配置中心核心功能中的刚需。<br />Spring Cloud Config使用Git，目前还不支持格式检验，格式的正确性依赖研发人员自己。<br />Apollo和Nacos都会对配置格式的正确性进行检验，可以有效防止人为错误。
<a name="tUUtZ"></a>
### 监听查询
当排查问题或者进行统计的时候，需要知道一个配置被哪些应用实例使用到，以及一个实例使用到了哪些配置。<br />Spring Cloud Config使用Spring Cloud Bus推送配置变更，Spring Cloud Bus兼容 RabbitMQ、Kafka等，支持查询订阅Topic和Consumer的订阅关系。<br />Apollo可以通过灰度实例列表查看监听配置的实例列表，但实例监听的配置(Apollo称为命名空间)目前还没有展示出来。<br />Nacos可以查看监听配置的实例，也可以查看实例监听的配置情况。<br />基本上，这三个产品都具备监听查询能力，在自己的使用过程中，Nacos使用起来相对简单，易用性相对更好些。
<a name="yTk9f"></a>
### 多环境
在实际生产中，配置中心常常需要涉及多环境或者多集群，业务在开发的时候可以将开发环境和生产环境分开，或者根据不同的业务线存在多个生产环境。如果各个环境之间的相互影响比较小（开发环境影响到生产环境稳定性），配置中心可以通过逻辑隔离的方式支持多环境。<br />Spring Cloud Config支持Profile的方式隔离多个环境，通过在Git上配置多个Profile的配置文件，客户端启动时指定Profile就可以访问对应的配置文件。<br />Apollo也支持多环境，在控制台创建配置的时候就要指定配置所在的环境，客户端在启动的时候指定JVM参数ENV来访问对应环境的配置文件。<br />Nacos通过命名空间来支持多环境，每个命名空间的配置相互隔离，客户端指定想要访问的命名空间就可以达到逻辑隔离的作用。
<a name="cjIAW"></a>
### 多集群
当对稳定性要求比较高，不允许各个环境相互影响的时候，需要将多个环境通过多集群的方式进行物理隔离。<br />Spring Cloud Config可以通过搭建多套Config Server，Git使用同一个Git的多个仓库，来实现物理隔离。<br />Apollo可以搭建多套集群，Apollo的控制台和数据更新推送服务分开部署，控制台部署一套就可以管控多个集群。<br />Nacos控制台和后端配置服务是部署在一起的，可以通过不同的域名切换来支持多集群。
<a name="FuAmA"></a>
## 配置实时推送的对比
当配置变更的时候，配置中心需要将配置实时推送到应用客户端。<br />Nacos和Apollo配置推送都是基于HTTP长轮询，客户端和配置中心建立HTTP长联接，当配置变更的的时候，配置中心把配置推送到客户端。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1630937419146-32f9c024-70e3-4166-afa3-f88de10ad550.webp#averageHue=%23d1e4d8&clientId=u9df10bb9-362f-4&from=paste&id=u73636eb0&originHeight=493&originWidth=900&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua8e8b74a-95be-4989-a6e4-7822f532319&title=)<br />Spring Cloud Config原生不支持配置的实时推送，需要依赖Git的WebHook、Spring Cloud Bus和客户端/bus/refresh端点：

- 基于Git的WebHook，配置变更触发server端refresh
- Server端接收到请求并发送给Spring Cloud Bus
- Spring Cloud Bus接到消息并通知给客户端
- 客户端接收到通知，请求Server端获取最新配置

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1630937418935-f29c79fe-583e-4142-a386-c94bb444401e.webp#averageHue=%23f5fafa&clientId=u9df10bb9-362f-4&from=paste&id=u84dd5a65&originHeight=651&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u473fde6b-a29c-425a-84bb-2d10e2c5692&title=)<br />整体比较下来，Nacos和Apollo在配置实时推送链路上是比较简单高效的，Spring Cloud Config的配置推送引入Spring Cloud Bus，链路较长，比较复杂。
<a name="vpelY"></a>
## 部署结构 & 高可用的对比
<a name="gzWzG"></a>
### Spring Cloud Config
Spring Cloud Config包含config-server、Git和Spring Cloud Bus三大组件：

- config-server提供给客户端获取配置;
- Git用于存储和修改配置;
- Spring Cloud Bus通知客户端配置变更;

本地测试模式下，Spring Cloud Bus和config-server需要部署一个节点，Git使用GitHub就可以。在生产环境中，Spring Cloud Config，config-server需要部署至少两个节点。Spring Cloud Bus如果使用RabbitMQ，普通集群模式至少需要两个节点。<br />Git服务如果使用GitHub就不用考虑高可用问题，如果考虑到安全性要自建Git私有仓库，整体的成本比较高。Web服务可以部署多节点支持高可用，由于Git有数据的一致性问题，可以通过以下的方式来支持高可用：

- Git+Keepalived冷备模式，当主Git挂了可以马上切到备Git;
- Git多节点部署，存储使用网络文件系统或者通过DRBD实现多个Git节点的数据同步;
<a name="rUjvZ"></a>
### Apollo
Apollo分为MySQL，Config Service，Admin Service，Portal四个模块：

- MySQL存储Apollo元数据和用户配置数据;
- Config Service提供配置的读取、推送等功能，客户端请求都是落到Config Service上;
- Admin Service提供配置的修改、发布等功能，Portal操作的服务就是Admin Service;
- Portal提供给用户配置管理界面;

本地测试Config Service，Admin Service，Portal三个模块可以合并一起部署，MySQL单独安装并创建需要的表结构。在生产环境使用Apollo，Portal可以两个节点单独部署，稳定性要求没那么高的话，Config Service和Admin Service可以部署在一起，数据库支持主备容灾。
<a name="GtL6A"></a>
### Nacos
Nacos部署需要Nacos Service和MySQL：

- Nacos对外提供服务，支持配置管理和服务发现;
- MySQL提供Nacos的数据持久化存储;

单机模式下，Nacos可以使用嵌入式数据库部署一个节点，就能启动。如果对MySQL比较熟悉，想要了解整体数据流向，可以安装MySQL提供给Nacos数据持久化服务。生产环境使用Nacos，Nacos服务需要至少部署三个节点，再加上MySQL主备。
<a name="bAzHR"></a>
### 整体来看
Nacos的部署结构比较简单，运维成本较低。Apollo部署组件较多，运维成本比Nacos高。Spring Cloud Config生产高可用的成本最高。
<a name="qqJvd"></a>
## 多语言支持的对比
一个公司的各个系统可能语言不尽相同，现在使用的比较多的比如C++，Java，PHP，Python，Nodejs，还有Go等。引入配置中心之后，配置中心要想让多语言的系统都能享受到动态配置的能力，需要支持多语言生态。
<a name="vlaPa"></a>
### 多语言支持
Spring Cloud服务于Java生态，一开始只是针对Java微服务应用，对于非Java应用的微服务调用，可以使用Sidecar提供了HTTP API，但动态配置方面还不能很好的支持。<br />Apollo已经支持了多种语言，并且提供了open API。其他不支持的语言，Apollo的接入成本相对较低。<br />Nacos支持主流的语言，例如Java、Go、Python、Nodejs、PHP等，也提供了open API。
<a name="pnar7"></a>
### 迁移支持
国内主流的互联网公司仍是以Java为主，除了原生Java SDK，在对整个Java生态，比如Spring Boot和Spring Cloud的支持上，三个产品都是支持的。<br />Spring Cloud Config原生就支持Spring Boot和Spring Cloud，Nacos通过Spring Cloud for Alibaba支持Spring Boot和Spring Cloud生态，符合Spring生态中的标准实现方式，可以无缝从Spring Cloud Conig迁移到Nacos。<br />Apollo支持Spring Boot和Spring Cloud项目，但是实现方式不同于标准，无法做无缝迁移，从Spring Cloud迁移到Apollo，存在代码改造和兼容性成本。
<a name="s4xhh"></a>
### 性能对比
性能也是配置中心绕不过的一环，在同样的机器规格下，如果能支撑更大的业务量，势必能替公司节省更多的资源成本，提高资源利用率。应用客户端对配置中心的接口操作有读、写和变更通知，由于变更通知需要大量的客户端实例，不好模拟测试场景，下面仅对读和写操作做了测试。
<a name="JGUtn"></a>
#### 硬件环境
Nacos和Apollo使用同样的数据库（32C128G），部署Server服务的机器使用的8C16G配置的容器，磁盘是100G SSD。
<a name="shNAv"></a>
#### 版本
Spring Cloud Config使用2.0.0.M9版本，Apollo使用1.2.0 release版本，Nacos使用0.5版本。
<a name="fzRUD"></a>
#### 单机读场景
客户端测试程序通过部署多台机器，每台机器开启多个线程从配置中心读取不同的配置（3000个）。Nacos QPS可以达到15000，Apollo分为读内存缓存和从数据库中读两种方式，从数据库中读能达到7500，从内存读缓存性能可以达到9000QPS。Spring Cloud Config使用jGit读写Git，由于有客户端限制，单机读能力被限制在7QPS。
<a name="Dy5zC"></a>
#### 3节点读场景
将配置中心的压测节点数都部署成3个节点。Nacos QPS可以达到45000 QPS，Apollo读内存缓存可以达到27000 QPS。Nacos和Apollo由于读场景各个节点是独立的，基本就是单机读场景的3倍关系。Spring Cloud Config三个节点读能力可以到达21QPS。
<a name="sC8He"></a>
#### 单机写场景
同样的方式，多台机器同时在配置中心修改不同的配置。Nacos QPS可以达到1800，Apollo未使用默认的数据库连接池（10）QPS只能达到800 QPS（CPU未压满），调整连接池至100可以达到1100 QPS（CPU压满）。Git在提交同一个项目的时候会加锁，单机Git写能在5QPS左右，Spring Cloud Config在使用的时候以一个项目作为数据源，写能力受到Git限制。
<a name="Y2ugD"></a>
#### 3节点写场景
同样的方式，将配置中心的压测节点数都部署成3个节点。Nacos QPS可以达到6000，Apollo可以达到3300 QPS（CPU压满），此时MySQL数据库因为配置较高，未成为性能瓶颈。Spring Cloud Config三个节点时候，Git也是一个节点，写QPS为5。<br />整体上来看，Nacos的读写性能最高，Apollo次之，Spring Cloud Config的依赖Git场景不适合开放的大规模自动化运维API。
<a name="Q5FAy"></a>
## 功能特性对比总结
这里列一个表格总结一下三个产品的功能特点。

| 功能点 | Spring Cloud Config | Apollo | Nacos |
| --- | --- | --- | --- |
| 开源时间 | 2014.9 | 2016.5 | 2018.6 |
| 配置实时推送 | 支持（SpringCloud Bus） | 支持（HTTP长轮询1S内） | 支持（HTTP长轮询1S内） |
| 版本管理 | 支持（Git） | 支持 | 支持 |
| 配置回滚 | 支持（Git） | 支持 | 支持 |
| 灰度发布 | 支持 | 支持 | 待支持 |
| 权限管理 | 支持 | 支持 | 待支持 |
| 多集群 | 支持 | 支持 | 支持 |
| 多环境 | 支持 | 支持 | 支持 |
| 监听查询 | 支持 | 支持 | 支持 |
| 多语言 | 只支持Java | Go，C++，Python，PHP，Java（原生SDK Spring SpringBoot），.Net，OpenAPI | Python，Java（原生SDK Spring SpringBoot），NodeJS，OpenAPI |

| 功能点 | Spring Cloud Config | Apollo | Nacos |
| --- | --- | --- | --- |
| 单机部署 | Config-Server+Git+SpringCloud Bus（支持配置实时推送） | Apollo-quickstart+MySQL | Nacos单节点 |
| 分布式部署 | Config Server（2）+Git+MQ（部署复杂） | Config（2）+Admin（2）+Portal（2）+MySQL（部署复杂） | Nacos（3）+MySQL（部署简单） |
| 配置格式校验 | 不支持 | 支持 | 支持 |
| 通信协议 | HTTP和AMQP | HTTP | HTTP |
| 数据一致性 | Git保证数据一致性，Config-Server从Git读数据 | 数据库模拟消息队列，Apollo定时读消息 | HTTP异步通知 |
| 单机读 | 7（限流所致） | 9000 | 1500 |
| 单机写 | 5（限流所致） | 1100 | 1800 |
| 3节点读 | 21（限流所致） | 27000 | 45000 |
| 3节点写 | 5（限流所致） | 3300 | 5600 |

总的来说，Apollo和Nacos相对于Spring Cloud Config的生态支持更广，在配置管理流程上做的更好。Apollo相对于Nacos在配置管理做的更加全面，不过使用起来也要麻烦一些。Nacos使用起来相对比较简洁，在对性能要求比较高的大规模场景更适合。<br />此外，Nacos除了提供配置中心的功能，还提供了动态服务发现、服务共享与管理的功能，降低了服务化改造过程中的难度。<br />以上，从产品功能、使用体验、实施过程和性能 4 个纬度对Spring Cloud Config、Apollo和Nacos进行对比。但对于一个开源项目的选型，除了以上这4个方面，项目上的人力投入（迭代进度、文档的完整性）、社区的活跃度（issue的数量和解决速度、Contributor数量、社群的交流频次等）、社区的规范程度（免责说明、安全性说明等），这些可能才是用户更关注的内容。
<a name="lMqtZ"></a>
## 参考文档
[https://springcloud.cc/spring-cloud-config.html](https://springcloud.cc/spring-cloud-config.html)<br />[https://github.com/ctripcorp/apollo](https://github.com/ctripcorp/apollo)<br />[https://nacos.io/](https://nacos.io/)
