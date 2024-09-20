<a name="WMI52"></a>
## 简介
SpringBoot 是一个非常流行的 Java 框架，它可以帮助开发者快速构建应用程序。他不仅继承了 Spring 框架原有的优秀特性，而且还通过简化配置来进一步简化了 Spring 应用的整个搭建和开发过程。<br />找到了35款基于 SpringBoot/ SpringCloud 技术体系的一些开源框架。这些项目大都具备一些很完善的功能，并且均采用了当下比较流行的技术栈。无论是日常学习，还是当做自己项目搭建的脚手架，都是非常合适的。
<a name="ej00o"></a>
## 一、开发脚手架
<a name="pZFBn"></a>
### 1.1 Cloud-Platform
项目地址：[https://gitee.com/geek_qi/cloud-platform](https://gitee.com/geek_qi/cloud-platform)<br />Cloud-Platform 是国内首个基于Spring Cloud微服务化开发平台，具有统一授权、认证后台管理系统，其中包含具备用户管理、资源权限管理、网关API 管理等多个模块，支持多业务系统并行开发，可以作为后端服务的开发脚手架。<br />代码简洁，架构清晰，适合学习和直接项目中使用。核心技术采用Spring Boot 2.1.2以及Spring Cloud (Greenwich.RELEASE) 相关核心组件，采用Nacos注册和配置中心，集成流量卫兵Sentinel，前端采用vue-element-admin组件。
<a name="gjb8t"></a>
### 1.2 SpringCloud

- 项目地址：[https://github.com/zhoutaoo/SpringCloud](https://github.com/zhoutaoo/SpringCloud)

基于 SpringCloud2.1 的微服务开发脚手架，整合了spring-security-oauth2、nacos、feign、sentinel、springcloud-gateway等。服务治理方面引入elasticsearch、skywalking、springboot-admin、zipkin等，让项目开发快速进入业务开发，而不需过多时间花费在架构搭建上。
<a name="Ypc0B"></a>
### 1.3 spring-boot-api-project-seed
项目地址：[https://github.com/lihengming/spring-boot-api-project-seed](https://github.com/lihengming/spring-boot-api-project-seed)<br />spring-boot-api-project-seed 是一个基于Spring Boot & MyBatis的种子项目，用于快速构建中小型API、RESTful API项目，该种子项目已经有过多个真实项目的实践，稳定、简单、快速，摆脱那些重复劳动，专注于业务代码的编写，减少加班。
<a name="h4KEg"></a>
### 1.4 Roses
项目地址：[https://gitee.com/stylefeng/roses](https://gitee.com/stylefeng/roses)<br />Roses 基于Spring Boot 2和Spring Cloud Finchley.SR2，更符合企业级的分布式和服务化解决方案，Roses拥有高效率的开发体验，提供可靠消息最终一致性分布式事务解决方案，提供基于调用链的服务治理，提供可靠的服务异常定位方案（Log + Trace）等等，一个分布式框架不仅需要构建高效稳定的底层开发框架，更需要解决分布式带来的种种挑战。
<a name="RpnBN"></a>
### 1.5 Pig
项目地址：[https://gitee.com/log4j/pig](https://gitee.com/log4j/pig)

- 基于 Spring Cloud Hoxton 、Spring Boot 2.2、 OAuth2 的RBAC权限管理系统；
- 基于数据驱动视图的理念封装 element-ui，即使没有 vue 的使用经验也能快速上手；
- 提供对常见容器化支持 Docker、Kubernetes、Rancher2 支持；
- 提供 lambda 、stream api 、webflux 的生产实践；
<a name="wmDWZ"></a>
### 1.6 RuoYi/RouYi-Cloud
项目地址：[https://gitee.com/y_project/RuoYi](https://gitee.com/y_project/RuoYi)<br />基于Spring Boot、Spring Cloud & Alibaba的分布式微服务架构权限管理系统，同时提供了 Vue3 的版本， 核心技术采用Spring、MyBatis、Shiro没有任何其它重度依赖。

- 提供了多种版本：单体、前后端分离、微服务（即将开源）；
- 提供的功能齐全，覆盖大部分场景需求；
- 提供的文档丰富便于上手和学习；
- 生态系统丰富提供了多种版本；
- 采用主流框架比如 SpringBoot、Shiro、Thymeleaf、Vue、Bootstrap；
- 可以用于所有的 Web 应用程序，如网站管理后台，网站会员中心，CMS，CRM，OA；

内置以下17个功能：
<a name="jiYUn"></a>
### 1.7 JeecgBoot
项目地址：[https://gitee.com/jeecg/jeecg-boot](https://gitee.com/jeecg/jeecg-boot)<br />JeecgBoot 基于代码生成器的低代码开发平台，开源界“小普元”超越传统商业开发平台！前后端分离架构：SpringBoot 2.x，Ant Design&Vue，Mybatis-plus，Shiro，JWT。强大的代码生成器让前后端代码一键生成，无需写任何代码!<br />引领新开发模式(OnlineCoding-> 代码生成-> 手工MERGE)，帮助Java项目解决70%重复工作，让开发更关注业务逻辑，既能快速提高开发效率，帮助公司节省成本，同时又不失灵活。<br />JeecgBoot 可以应用在任何 J2EE 项目的开发中，尤其适合企业信息管理系统（MIS）、内部办公系统（OA）、企业资源计划系统（ERP）、客户关系管理系统（CRM）等，其半智能手工 Merge 的开发方式，可以显著提高开发效率 70%以上，极大降低开发成本。
<a name="dBwJ1"></a>
### 1.8 iBase4J
项目地址：[https://gitee.com/iBase4J/iBase4J](https://gitee.com/iBase4J/iBase4J)<br />iBase4J 是Java语言的分布式系统架构，基于SpringBoot 2.0，SpringMVC，Mybatis，mybatis-plus，motan/dubbo分布式，Redis缓存，Shiro权限管理，redis管理Session，Quartz分布式集群调度，Restful服务。<br />系统包括4个子系统：系统管理Service、系统管理Web、业务Service、业务Web。<br />系统管理：包括用户管理、权限管理、数据字典、系统参数管理等等；支持QQ/微信登录，App token登录，微信/支付宝支付；日期转换、数据类型转换、序列化、汉字转拼音、身份证号码验证、数字转人民币、发送短信、发送邮件、加密解密、图片处理、excel导入导出、FTP/SFTP/fastDFS上传下载、二维码、XML读写、高精度计算、系统配置工具类等等。<br />可以无限的扩展子系统，子系统之间使用Dubbo或MQ进行通信。
<a name="lDPIu"></a>
### 1.9 renren
项目地址：[https://www.renren.io](https://www.renren.io)<br />renren 下面一共开源了两个 Java 项目开发脚手架，分别是：

- renren-security：采用 Spring、MyBatis、Shiro 框架，开发的一套轻量级权限系统，极低门槛，拿来即用；
- renren-fast：一个轻量级的 Java 快速开发平台，能快速开发项目并交付【接私活利器】；

renren-security 相比于 renren-fast 在后端功能的区别主要在于：renren-security 提供了权限管理功能，另外还额外提供了数据字典和代码生成器。<br />注意：微服务版 renren-cloud和 renren-security 需要收费才能正常使用，renren-fast 属于完全免费并且提供了详细的文档，不过，完整文档需要捐赠 80 元才能获取到。
<a name="Xpmqx"></a>
### 1.10 SpringBlade
项目地址：[https://bladex.vip](https://bladex.vip)<br />SpringBlade 是一个由商业级项目升级优化而来的 SpringCloud 分布式微服务架构、SpringBoot 单体式微服务架构并存的综合型项目，采用 Java8 API 重构了业务代码，完全遵循阿里巴巴编码规范。<br />采用 Spring Boot 2 、Spring Cloud Hoxton 、Mybatis 等核心技术，同时提供基于 React 和 Vue 的两个前端框架用于快速搭建企业级的 SaaS 多租户微服务平台。

- 允许免费用于学习、毕设、公司项目、私活等。如果商用的话，需要授权，并且功能更加完善；
- 前后端分离，后端采用 SpringCloud 全家桶，单独开源出一个框架：BladeTool （感觉很厉害）；
- 集成 Sentinel 从流量控制、熔断降级、系统负载等多个维度保护服务的稳定性；
- 借鉴 OAuth2，实现了多终端认证系统，可控制子系统的 token 权限互相隔离；
- 借鉴 Security，封装了 Secure 模块，采用 JWT 做 Token 认证，可拓展集成 Redis 等细颗粒度控制方案；
- 项目分包明确，规范微服务的开发模式，使包与包之间的分工清晰；
<a name="bN9qS"></a>
### 1.11 COLA
项目地址：[https://github.com/alibaba/COLA](https://github.com/alibaba/COLA)<br />很多公司的项目都是基于 COLA 进行开发的，相比于其他快速开发脚手架，COLA 并不提供什么已经开发好的功能，它提供的主要是一个干净的架构，然后可以在此基础上进行开发。<br />COLA 既是框架，也是架构。创建 COLA 的主要目的是为应用架构提供一套简单的可以复制、可以理解、可以落地、可以控制复杂性的”指导和约束"。

- 框架部分主要是以二方库的形式被应用依赖和使用。
- 架构部分主要是提供了创建符合 COLA 要求的应用 Archetype。
<a name="WrBiE"></a>
### 1.12 SpringBoot_v2
Github 地址 ：[https://github.com/fuce1314/Springboot_v2](https://github.com/fuce1314/Springboot_v2)<br />SpringBoot_v2 项目是努力打造 springboot 框架的极致细腻的脚手架。原生纯净，可在线生成 controller、mapperxml、dao、service、html、sql 代码，极大减少开发难度，增加开发进度神器脚手架。

- 没有基础版、没有 vip 版本、没有付费群、没有收费二维码。
- 对新手友好，配置好数据库连接即可运行。
- 满足一般中小企业的基本需求。
- 功能简单，无其他杂七杂八的功能
<a name="KzVoz"></a>
### 1.13 lamp-cloud
项目地址：[https://gitee.com/zuihou111/lamp-cloud](https://gitee.com/zuihou111/lamp-cloud)<br />Iamp-cloud 是一个微服务中后台快速开发平台，可以通过插件无缝切换是否启用SaaS模式、通过配置切换SaaS模式采用独立数据库模式还是字段模式。<br />她具备SaaS模式切换、完备的RBAC功能、网关统一鉴权、灰度发布、数据权限、可插拔缓存、统一封装缓存的key、表单校验前后端统一验证、字典数据自动回显、Xss防跨站攻击、自动生成前后端代码、多种存储系统、分布式事务、分布式定时任务等多个功能和模块， 支持多业务系统并行开发， 支持多服务并行开发，是中后台系统开发脚手架的最佳选择。代码简洁，注释齐全，架构清晰，非常适合学习和企业作为基础框架使用。<br />核心技术采用Spring Cloud Alibaba、SpringBoot、Mybatis、Seata、Sentinel、RabbitMQ、FastDFS/MinIO、SkyWalking等主要框架和中间件。希望能努力打造一套从 JavaWeb基础框架 - 分布式微服务架构 - 持续集成 - 系统监测 的解决方案。项目旨在实现基础能力，不涉及具体业务。
<a name="ZyPJt"></a>
### 1.14 microservices-platform
项目地址：[https://gitee.com/zlt2000/microservices-platform](https://gitee.com/zlt2000/microservices-platform)<br />基于SpringBoot2.x、SpringCloud和SpringCloudAlibaba并采用前后端分离的企业级微服务多租户系统架构。并引入组件化的思想实现高内聚低耦合，项目代码简洁注释丰富上手容易，适合学习和企业中使用。<br />真正实现了基于RBAC、jwt和oauth2的无状态统一权限认证的解决方案，面向互联网设计同时适合B端和C端用户，支持CI/CD多环境部署，并提供应用管理方便第三方系统接入；同时还集合各种微服务治理功能和监控功能。<br />模块包括:企业级的认证系统、开发平台、应用监控、慢sql监控、统一日志、单点登录、Redis分布式高速缓存、配置中心、分布式任务调度、接口文档、代码生成等等。
<a name="RcO9V"></a>
### 1.15 MCMS
项目地址：[https://gitee.com/mingSoft/MCMS](https://gitee.com/mingSoft/MCMS)<br />完整开源的 CMS。基于SpringBoot 2架构，前端基于vue、element ui。每月28定期更新版本，为开发者提供上百套免费模板,同时提供适用的插件（文章、商城、微信、论坛、会员、评论、支付、积分、工作流、任务调度等…），一套简单好用的开源系统、一整套优质的开源生态内容体系。<br />铭飞的使命就是降低开发成本提高开发效率，提供全方位的企业级开发解决方案。
<a name="CaURx"></a>
## 二、后台管理系统
<a name="adEnV"></a>
### 2.1 EL-ADMIN
项目地址：[https://github.com/elunez/eladmin](https://github.com/elunez/eladmin)<br />EL-ADMIN 项目基于 Spring Boot 2.1.0 、 Jpa、 Spring Security、redis、Vue的前后端分离的后台管理系统，项目采用分模块开发方式， 权限控制采用 RBAC，支持数据字典与数据权限管理，支持一键生成前后端代码，支持动态路由。<br />主要功能：

- **用户管理**：提供用户的相关配置，新增用户后，默认密码为123456
- **角色管理**：对权限与菜单进行分配，可根据部门设置角色的数据权限
- **菜单管理**：已实现菜单动态路由，后端可配置化，支持多级菜单
- **部门管理**：可配置系统组织架构，树形表格展示
- **岗位管理**：配置各个部门的职位
- **字典管理**：可维护常用一些固定的数据，如：状态，性别等
- **操作日志**：记录用户操作的日志
- **异常日志**：记录异常日志，方便开发人员定位错误
- **SQL监**控：采用druid 监控数据库访问性能，默认用户名admin，密码123456
- **定时任务**：整合Quartz做定时任务，加入任务日志，任务运行情况一目了然
- **代码生成**：高灵活度一键生成前后端代码，减少百分之80左右的工作任务
- **邮件工具**：配合富文本，发送html格式的邮件
- **免费图床**：使用sm.ms图床，用作公共图片上传使用，该图床不怎么稳定，不太建议使用
- **七牛云存储**：可同步七牛云存储的数据到系统，无需登录七牛云直接操作云数据
- **支付宝支付**：整合了支付宝支付并且提供了测试账号，可自行测试
<a name="Bbsz8"></a>
### 2.2 jeeSpringCloud
项目地址：[https://gitee.com/JeeHuangBingGui/jeeSpringCloud](https://gitee.com/JeeHuangBingGui/jeeSpringCloud)<br />jeeSpringCloud 基于SpringBoot2.0的后台权限管理系统界面简洁美观敏捷开发系统架构。核心技术采用Spring、MyBatis、Shiro没有任何其它重度依赖。互联网云快速开发框架，微服务分布式代码生成的敏捷开发系统架构。<br />项目代码简洁，注释丰富，上手容易，还同时集中分布式、微服务，同时包含许多基础模块和监控、服务模块。<br />模块包括：定时任务调度、服务器监控、平台监控、平台设置、开发平台、单点登录、Redis分布式高速缓存、会员、营销、在线用户、日志、在线人数、访问次数、调用次数、直接集群、接口文档、生成模块、代码实例、安装视频、教程文档 代码生成(单表、主附表、树表、列表和表单、redis高速缓存对接代码、图表统计、地图统计、vue.js)、dubbo、springCloud、SpringBoot、mybatis、spring、springmvc。
<a name="XVxNT"></a>
### 2.3 springboot-plus
项目地址：[https://gitee.com/xiandafu/springboot-plus](https://gitee.com/xiandafu/springboot-plus)<br />springboot-plus 是一个基于SpringBoot 2 的管理后台系统，包含了用户管理，组织机构管理，角色管理，功能点管理，菜单管理，权限分配，数据权限分配，代码生成等功能 相比其他开源的后台系统，SpringBoot-Plus 具有一定的复杂度。系统基于Spring Boot2.1技术，前端采用了Layui2.4。<br />数据库以MySQL/Oracle/Postgres/SQLServer为实例，理论上是跨数据库平台。
<a name="R97a3"></a>
### 2.4 Timo
项目地址：[https://gitee.com/aun/Timo](https://gitee.com/aun/Timo)<br />TIMO 后台管理系统，基于SpringBoot2.0 + Spring Data Jpa + Thymeleaf + Shiro 开发的后台管理系统，采用分模块的方式便于开发和维护，支持前后台模块分别部署，目前支持的功能有：权限管理、部门管理、字典管理、日志记录、文件上传、代码生成等，为快速开发后台系统而生的脚手架！
<a name="by3VS"></a>
### 2.5 Guns
项目地址：[https://gitee.com/stylefeng/guns](https://gitee.com/stylefeng/guns)<br />Guns 基于Spring Boot2，致力于做更简洁的后台管理系统。包含系统管理，代码生成，多数据库适配，SSO单点登录，工作流，短信，邮件发送，OAuth2登录，任务调度，持续集成，docker部署等功。支持Spring Cloud Alibaba微服务。
<a name="cqeli"></a>
## 三、电商系统
<a name="LtF4u"></a>
### 3.1 mall
项目地址：[https://github.com/macrozheng/mall](https://github.com/macrozheng/mall)<br />mall 项目是一套电商系统，包括前台商城系统及后台管理系统，基于SpringBoot+MyBatis实现，采用Docker容器化部署。<br />前台商城系统包含首页门户、商品推荐、商品搜索、商品展示、购物车、订单流程、会员中心、客户服务、帮助中心等模块。<br />后台管理系统包含商品管理、订单管理、会员管理、促销管理、运营管理、内容管理、统计报表、财务管理、权限管理、设置等模块。
<a name="alNLu"></a>
### 3.2 mall-swarm
项目地址：[https://github.com/macrozheng/mall-swarm](https://github.com/macrozheng/mall-swarm)<br />mall-swarm 是一套微服务商城系统，采用了 Spring Cloud Greenwich、Spring Boot 2、MyBatis、Docker、Elasticsearch等核心技术，同时提供了基于Vue的管理后台方便快速搭建系统。mall-swarm在电商业务的基础集成了注册中心、配置中心、监控中心、网关等系统功能。
<a name="KV6gL"></a>
### 3.3 newbee-mall
项目地址：[https://github.com/newbee-ltd/newbee-mall](https://github.com/newbee-ltd/newbee-mall)<br />newbee-mall 项目（新蜂商城）是一套电商系统，包括 newbee-mall 商城系统及 newbee-mall-admin 商城后台管理系统，基于 Spring Boot 2.X 及相关技术栈开发。<br />前台商城系统包含首页门户、商品分类、新品上线、首页轮播、商品推荐、商品搜索、商品展示、购物车、订单结算、订单流程、个人订单管理、会员中心、帮助中心等模块。后台管理系统包含数据面板、轮播图管理、商品管理、订单管理、会员管理、分类管理、设置等模块。
<a name="D8Ekb"></a>
### 3.4 onemall
项目地址：[https://github.com/YunaiV/onemall](https://github.com/YunaiV/onemall)<br />onemall 商城，基于微服务的思想，构建在 B2C 电商场景下的项目实战。核心技术栈是 Spring Boot + Dubbo 。
<a name="QRkLq"></a>
### 3.5 litemall
项目地址：[https://github.com/linlinjava/litemall](https://github.com/linlinjava/litemall)<br />litemall 是 一个商城项目，包括Spring Boot后端 + Vue管理员前端 + 微信小程序用户前端 + Vue用户移动端。
<a name="Gsyv4"></a>
### 3.6 xbin-store
项目地址：[https://github.com/xubinux/xbin-store](https://github.com/xubinux/xbin-store)<br />xbin-store 模仿国内知名B2C网站，实现的一个分布式B2C商城 使用Spring Boot 自动配置 Dubbox / MVC / MyBatis / Druid / Solr / Redis 等，它有Spring Cloud版本和Dubbox版本。
<a name="yA0Cr"></a>
### 3.7 zscat_sw
项目地址：[https://gitee.com/catshen/zscat_sw](https://gitee.com/catshen/zscat_sw)<br />zscat_sw 是基于springboot dubbo构建的一个商城项目，包括前端，后端和h5应用，小程序，作为zscat应用实践的模板项目。包含sso登录、API网关、流量控制、自定义协议包装、、自动crud、自动缓存、读写分离、分布式缓存、分布式定时任务、分布式锁、消息队列、事件机制、oauth2.0登录、全文搜索、集成qiniu文件服务、集成dubbo、集成springcboot等功能。
<a name="Fz3Gp"></a>
## 四、人力资源管理系统
<a name="YdL6h"></a>
### 4.1 vhr - 微人事
项目地址：[https://github.com/lenve/vhr](https://github.com/lenve/vhr)<br />微人事 是一个前后端分离的人力资源管理系统，项目采用 SpringBoot + Vue 开发。权限管理相关的模块主要有两个，分别是 [系统管理->基础信息设置->权限组] 可以管理角色和资源的关系， [系统管理->操作员管理] 可以管理用户和角色的关系。
<a name="AIGiE"></a>
### 4.2 oasys-OA自动化办公系统
项目地址：[https://gitee.com/aaluoxiang/oa_system](https://gitee.com/aaluoxiang/oa_system)<br />办公自动化（OA）是面向组织的日常运作和管理，员工及管理者使用频率最高的应用系统，极大提高公司的办公效率。<br />oasys是一个OA办公自动化系统，使用Maven进行项目管理，基于springboot框架开发的项目，mysql底层数据库，前端采用freemarker模板引擎，Bootstrap作为前端UI框架，集成了jpa、mybatis等框架。
<a name="PMQxu"></a>
## 五、支付案例
<a name="cRbjw"></a>
### 5.1 spring-boot-pay
项目地址：[https://gitee.com/52itstyle/spring-boot-pay](https://gitee.com/52itstyle/spring-boot-pay)<br />spring-boot-pay 是一个支付案例，提供了包括支付宝、微信、银联在内的详细支付代码案例，对于有支付需求的小伙伴来说，这个项目再合适不过了。
<a name="COENA"></a>
## 六、秒杀系统
<a name="MinVh"></a>
### 6.1 spring-boot-seckill
项目地址：[https://gitee.com/52itstyle/spring-boot-seckill](https://gitee.com/52itstyle/spring-boot-seckill)<br />从0到1构建分布式秒杀系统，脱离案例讲架构都是耍流氓。

- 妹子图小程序：[https://gitee.com/52itstyle/mzitu](https://gitee.com/52itstyle/mzitu)
- 快速开发：[https://gitee.com/52itstyle/SPTools](https://gitee.com/52itstyle/SPTools)
- 支付服务：[https://gitee.com/52itstyle/spring-boot-pay](https://gitee.com/52itstyle/spring-boot-pay)
- 任务调度：[https://gitee.com/52itstyle/spring-boot-quartz](https://gitee.com/52itstyle/spring-boot-quartz)
- 邮件服务：[https://gitee.com/52itstyle/spring-boot-mail](https://gitee.com/52itstyle/spring-boot-mail)
- 搜索服务：[https://gitee.com/52itstyle/spring-boot-elasticsearch](https://gitee.com/52itstyle/spring-boot-elasticsearch)
<a name="Y94fG"></a>
## 七、博客管理系统
<a name="O89ed"></a>
### 7.1 VBlog
项目地址：[https://github.com/lenve/VBlog](https://github.com/lenve/VBlog)<br />V部落，Vue+SpringBoot实现的多用户博客管理平台！
<a name="Pt9Dw"></a>
### 7.2 halo
项目地址：[https://github.com/halo-dev/halo](https://github.com/halo-dev/halo)<br />Halo 是一款现代化的个人独立博客系统，给习惯写博客的同学多一个选择。一个优秀的开源博客发布应用。
<a name="Fh9Xw"></a>
### 7.3 NiceFish
项目地址：[https://gitee.com/mumu-osc/NiceFish](https://gitee.com/mumu-osc/NiceFish)<br />NiceFish（美人鱼） 是一个系列项目，目标是示范前后端分离的开发模式：前端浏览器、移动端、Electron 环境中的各种开发模式；后端有两个版本：SpringBoot 版本和 SpringCloud 版本。
<a name="zbVlq"></a>
## 八、ERP系统
<a name="tiWUZ"></a>
### 8.1 jshERP
项目地址：[https://gitee.com/jishenghua/JSH_ERP](https://gitee.com/jishenghua/JSH_ERP)<br />华夏ERP 基于SpringBoot框架和SaaS模式，立志为中小企业提供开源好用的ERP软件，目前专注进销存+财务功能。<br />主要模块有零售管理、采购管理、销售管理、仓库管理、财务管理、报表查询、系统管理等。支持预付款、收入支出、仓库调拨、组装拆卸、订单等特色功能。拥有库存状况、出入库统计等报表。同时对角色和权限进行了细致全面控制，精确到每个按钮和菜单。
