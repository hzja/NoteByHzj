<a name="lRZsG"></a>
## 微服务架构发展史
<a name="aj2aq"></a>
### 单体架构
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630760764855-cb809389-280d-49bb-88ee-11e9f3f3bebb.png#clientId=u56c2f987-5109-4&errorMessage=unknown%20error&from=paste&height=254&id=u26504895&originHeight=762&originWidth=2090&originalType=binary&ratio=1&rotation=0&showTitle=false&size=295836&status=error&style=shadow&taskId=uce3ef6c0-9b04-4d3b-b3e8-3a24ed8ac5c&title=&width=696.6666666666666)
<a name="bqEXf"></a>
### 业务垂直划分
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630760794011-ab701916-52d3-42b1-ba49-ce17760b4191.png#clientId=u56c2f987-5109-4&errorMessage=unknown%20error&from=paste&height=467&id=ue90611d6&originHeight=1401&originWidth=2144&originalType=binary&ratio=1&rotation=0&showTitle=false&size=513503&status=error&style=shadow&taskId=udd24b0d0-9596-4ae7-819f-33fb1f455e9&title=&width=714.6666666666666)
<a name="gzmBu"></a>
### 集群-负载均衡
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630760855850-2d6f1ce3-ed80-4adc-b474-58df9ac7dcc2.png#clientId=u56c2f987-5109-4&errorMessage=unknown%20error&from=paste&height=482&id=ud1be2960&originHeight=1445&originWidth=2255&originalType=binary&ratio=1&rotation=0&showTitle=false&size=641662&status=error&style=shadow&taskId=ue3f358dc-1071-4c1a-92bb-527eee02a1b&title=&width=751.6666666666666)
<a name="LreTU"></a>
### SOA架构
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630760923572-19775e98-2dbf-4f2f-8c3a-fa7445c56588.png#clientId=u56c2f987-5109-4&errorMessage=unknown%20error&from=paste&height=235&id=u12a6a9b7&originHeight=706&originWidth=2113&originalType=binary&ratio=1&rotation=0&showTitle=false&size=274632&status=error&style=shadow&taskId=ubaefce1b-e345-44ba-9c08-495d385deb0&title=&width=704.3333333333334)
<a name="V4114"></a>
### 微服务架构时代
（1）微服务架构就是开发的一个独立的系统，只不过这个独立系统通过一系列小的服务组成<br />（2）每个服务都会是一个独立的进程<br />（3）每个服务之间要进行调用，并且调用的方式是轻量级的通信机制，比如像Http协议<br />（4）这些服务通常会采用自动化的运维机制: CICD Continuous Intergration Continuous Delivery DevOps<br />（5）这些服务是去中心化的<br />（6）这些服务可以使用不同的编程语言来完成<br />（7）可以采用不同的数据存储技术
<a name="EJ5dt"></a>
## 单体架构和微服务架构的比较
<a name="Tt50q"></a>
### 单体架构
优势：所有功能代码都是部署在一个war包，方便部署与测试<br />劣势：<br />(1)开发效率<br />(2)代码维护难<br />(3)扩展性也比较低<br />(4)如果挂了，就会没办法给用户提供功能
<a name="FLyxN"></a>
### 微服务架构
优势：<br />独立性、技术栈比较灵活、数据库的选择比较灵活、团队高效等<br />劣势：<br />额外一些工作[DDD领域驱动设计]、数据一致性
<a name="BmwA0"></a>
## 微服务架构需要解决的问题
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630761742763-0c35fb40-ca7b-477d-abb3-24b6d6fcfbd9.png#clientId=u748d399d-0338-4&errorMessage=unknown%20error&from=paste&height=265&id=u614788ae&originHeight=796&originWidth=1011&originalType=binary&ratio=1&rotation=0&showTitle=false&size=156643&status=error&style=shadow&taskId=u99927b97-7a82-4caa-ba50-e60e52b1ee4&title=&width=337)<br />(1)服务调用<br />(2)负载均衡<br />(3)服务调用失败，容错机制:立即返回?等待?过一会时间再访问?<br />(4)网关<br />(5)事务<br />(6) ......<br />![](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630762415336-af20ba43-4d00-410f-9759-e22b59352b5f.jpeg)<br />左边是SpringCloud团队做了接口以及实现类，右边是不同的厂商对于规范做了对应的实现。
<a name="BAnmr"></a>
## 服务的注册与发现
<a name="Gu3em"></a>
### 配置统一的版本管理
```xml
<dependencyManagement>
  <dependencies>
    <dependency>
      <groupId>org.springframework.cloud</groupId>
      <artifactId>spring-cloud-dependencies</artifactId>
      <version>Hoxton.SR12</version>
      <type>pom</type>
      <scope>import</scope>
    </dependency>
    <dependency>
      <groupId>org.springframework.cloud</groupId>
      <artifactId>spring-cloud-alibaba-dependencies</artifactId>
      <version>0.9.0.RELEASE</version>
      <type>pom</type>
      <scope>import</scope>
    </dependency>
    <dependency>
      <groupId>org.springframework.cloud</groupId>
      <artifactId>spring-cloud-netflix-dependencies</artifactId>
      <version>2.2.9.RELEASE</version>
      <type>pom</type>
      <scope>import</scope>
    </dependency>
  </dependencies>
</dependencyManagement>
```
<a name="A4975"></a>
### 基于SpringCloud Netflix Eureka作为注册中心
<a name="RZNA5"></a>
### 基于Nacos进行服务的注册与发现
<a name="hMpTN"></a>
#### 1、引入依赖
```xml
<dependency>
  <groupId>org.springframework.cloud</groupId>
  <artifactId>spring-cloud-alibaba-nacos-discovery</artifactId>
</dependency>
```
<a name="tmNq8"></a>
#### 2、配置Nacos注册中心
```yaml
spring:
  cloud:
    nacos:
      discovery:
      	#指定nacos-server所在的地址
        server-addr: localhost:8848
  application:
    name: order-service
```
<a name="g7RFw"></a>
#### 3、启动子服务即可在日志以及Nacos控制台看到已经成功注册服务
默认的用户名和密码是nacos。
<a name="Edxj8"></a>
#### 4、使用Ribbon做负载均衡
```java
@Resource
private LoadBalancerClient loadBalancerclient;
```
<a name="g5kru"></a>
##### 注解的方式使用Ribbon的负载均衡
```java
@Bean
@LoadBalanced	//表示该RestTemplate就具备了ribbon能力
public RestTemplate restTemplateRibbon(){
	return new RestTemplate();
}
```
<a name="UAaBy"></a>
### 基于Zookeeper作为服务的注册中心
<a name="bMx3H"></a>
## 服务调用组件之OpenFeign
<a name="iDtvK"></a>
### 引入依赖
```xml
<dependency>
  <groupId>org.springframework.cloud</groupId>
  <artifactId>spring-cloud-starter-openfeign</artifactId>
</dependency>
```
<a name="kSMQF"></a>
### 主类添加注解
```xml
@EnableFeignClients
```
<a name="CZdRL"></a>
### 定义服务中远程调用的接口
```java
import org.springframework.cloud.openfeign.Feignclient;
import org.springframework.web.bind.annotation.RequestMapping;

@Feignclient(name = "order-service")
public interface OrderServiceFeignclient{
RequestMapping("/order/query")
	string query();
}
```
<a name="NrIY7"></a>
### 在业务类中进行依赖注入即可使用
<a name="e1wtO"></a>
### 定义一些Feign的配置
<a name="oao0M"></a>
#### 通过JavaConfig
<a name="wZNNY"></a>
#### 通过yaml文件配置
```yaml
# feign的日志级别FULL
feign:
  client:
    config:
      #feign调用的服务名称
      order-service:
      	loggerLevel: full
logging:
  level:
  	com.fcant.userservice.openfeign: debug
```
<a name="DFGK7"></a>
## 配置中心
<a name="irRqu"></a>
### Nacos作为配置中心
<a name="I68Ut"></a>
#### 1、引入Nacos Config包
```xml
<dependency>
  <groupId>com.alibaba.cloud</groupId>
  <artifactId>spring-cloud-alibaba-nacos-config</artifactId>
</dependency>
<!--使bootstrap.yaml文件生效-->
<dependency>
  <groupId>org.springframework.cloud</groupId>
  <artifactId>spring-cloud-context</artifactId>
</dependency>
```
<a name="teRUd"></a>
#### 2、在Nacos Web端添加配置文件
创建一个名称为user-service-dev.yaml的配置文件
```yaml
person:
	username: Fcant
```
<a name="oa9TW"></a>
#### 3、在服务中配置选择对应的配置文件
创建bootstrap.yaml文件，添加主要的配置内容
```yaml
spring:
  cloud:
    nacos:
      config:
        server-addr: localhost:8848
        file-extension: yaml
  application:
  #对应user-service-dev.yaml中的user-service
  	name : user-service
  #配置激活的环境，即文件名后缀-dev
  profiles:
  	active: dev
```
<a name="E8hkD"></a>
#### 4、在对应读取配置文件的Bean上面添加`@RefreshScope`注解
```java
@RestController
@RequestMapping("/user")
@RefreshScope
public class UserController {
    @Value("${person.username}")
    private String personUsername;
    
    @RequestMapping("/readnacosconfig")
    public String readNacosConfig(){
    	return this.personUsername;
    }
}
```
<a name="ziCUt"></a>
### Spring Cloud Config作为配置中心
<a name="NNqbI"></a>
## 服务保护

- 限流
- 降级
<a name="D0Y6p"></a>
### SpringCloud Alibaba Sentinel
<a name="RACKC"></a>
#### 1、导入依赖
```xml
<dependency>
  <groupId>com.alibaba.cloud</groupId>
  <artifactId>spring-cloud-alibaba-sentinel</artifactId>
</dependency>
```
<a name="k3nXo"></a>
#### 2、配置
```yaml
spring:
  cloud :
    nacos:
      discovery:
      	server-addr: localhost:8848
    sentinel:
      transport:
        #配置sentinel-server所在的地址
        dashboard: localhost:8080
```
<a name="H6fcM"></a>
#### 3、服务的接口以及监控管理
:::tips
注意：服务启动成功后，并不能看到对应的服务名，因为Sentinel是懒加载的，所以只有通过访问的请求，才能在Web端查看到。
:::
<a name="ifLVg"></a>
### SpringCloud Netflix Hystrix
<a name="lXFoH"></a>
## 消息中间件的整合
<a name="VuLKy"></a>
### Kafka
<a name="QESVr"></a>
### Pulsar
<a name="ojwLn"></a>
### ActiveMQ
<a name="Kulmr"></a>
### RocketMQ
官网：[https://rocketmq.apache.org/](https://rocketmq.apache.org/)
<a name="entwq"></a>
#### 1、RocketMQ安装部署启动
Window上需要先添加RocketMQ安装包的环境变量
```bash
start mqnamesrv.cmd
start mqbroker.cmd -n 127.0.0.1:9876 autoCreateTopicEnab1e=true
```
<a name="NEzRa"></a>
#### 2、RocketMQ Web管理控制台
[https://github.com/apache/rocketmq-dashboard](https://github.com/apache/rocketmq-dashboard)<br />如果启动端口冲突则需要修改配置
<a name="iFBJv"></a>
#### 3、服务中整合RocketMQ的依赖
```xml
<dependency>
  <groupId>org.apache.rocketmq</groupId>
  <artifactId>rocketmq-spring-boot-starter</artifactId>
  <version>2.2.0</version>
</dependency>
```
<a name="N28Qm"></a>
#### 4、配置broker以及服务对应的producer
```yaml
# rocket-server的配置
rocketmq:
  name-server: 127.0.0.1:9876
  producer:
    group: fcant-producer
```
<a name="IvHDL"></a>
#### 5、生产消息示例
```java
@Resource
RocketMQTemplate rocketMQTemplate;

@RequestMapping("/produce")
public void produce() {
    rocketMQTemplate.convertAndSend("user-service-topic", "Hello World");
}
```
<a name="DDDQi"></a>
#### 6、消费者服务配置RocketMQ Server
```yaml
# rocket-server的配置
rocketmq:
  name-server: 127.0.0.1:9876
```
<a name="lG2Vw"></a>
#### 7、消费者消费消息
```java
import org.apache.rocketmq.spring.annotation.RocketMQMessageListener;
import org.apache.rocketmq.spring.core.RocketMQListener;
import org.springframework.stereotype.Service;

/**
 * ConsumerListener
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 上午 10:41 2021/9/5/0005
 */
@Service
@RocketMQMessageListener(consumerGroup = "fcant-consumer",topic = "user-service-topic")
public class ConsumerListener implements RocketMQListener<String> {
    @Override
    public void onMessage(String s) {
        System.out.println("Get Message: " + s);
    }
}
```
<a name="Ua6wn"></a>
## 微服务网关
<a name="kHnhL"></a>
### SpringCloud Netflix Zuul第一代网关
Zuul网关采取的是BIO（同步阻塞IO），`new ServerSocket(8090);`
<a name="LGF9V"></a>
### SpringCloud GateWay第二代网关
GateWay采取的是NIO（同步非阻塞IO），`new ServerSocketChannel(8090);`，GateWay更适合高并发的场景。<br />网关可以做的事：

1. 服务的发现以及自定义
2. 熔断
3. 负载均衡
4. 限流
5. 认证授权
<a name="crJeR"></a>
#### 1、创建GateWay网关模块
<a name="VcjEj"></a>
#### 2、引入GateWay依赖（这里的注册中心还是Nacos）
```xml
<dependency>
  <groupId>org.springframework.cloud</groupId>
  <artifactId>spring-cloud-alibaba-nacos-discovery</artifactId>
</dependency>
<dependency>
  <groupId>org.springframework.cloud</groupId>
  <artifactId>spring-cloud-starter-gateway</artifactId>
</dependency>
```
<a name="aCKkr"></a>
#### 3、配置yaml
```yaml
server:
	port: 8090
spring:
  cloud :
    nacos:
      discovery:
      	server-addr: localhost:8848
    gateway:
      discovery:
      	# 开启网关服务发现能力
        locator:
        	enabled: true
application:
	name: my-gateway
```
<a name="FYbIN"></a>
#### 4、自定义配置路由规则
```yaml
spring:
  cloud:
  nacos:
    discovery:
    	server-addr: localhost:8848
  gateway:
    discovery:
      locator:
        #开启网关服务发现的能力
        enabled: true
  #自定义的路由规则
  routes:
    #路由匹配的条件
    - predicates:
    	- Path=/fcant/**
    filters:
      #表示过滤器进行操作
      - StripPrefix=1
		uri: http://localhost:8081
```
<a name="BDt8o"></a>
## 链路追踪
把微服务的调用关系记录下来，方便后期问题排查。
<a name="j81s4"></a>
### SpringCloud Sleuth（可视化界面使用zipkin）
默认的采样率为10%，即调用十次，记录一次。
<a name="xD6Dh"></a>
#### 1、引入依赖
在需要进行链路追踪的模块里面引入依赖，zipkin已经包含了sleuth的依赖
```xml
<dependency>
  <groupId>org.springframework.cloud</groupId>
  <artifactId>spring-cloud-starter-zipkin</artifactId>
</dependency>
```
<a name="ksjrd"></a>
#### 2、服务模块配置zipkin
这里设置采用率为百分之百，方便进行观察。默认0.1。
```yaml
spring:
	zipkin: 
  	base-url: http:localhost:9411
  sleuth:
  	sampler:
    	probability: 1.0
```
<a name="KEZmU"></a>
### SkyWalking（用的比较多）
<a name="h3Sqh"></a>
### 阿里鹰眼
<a name="m0dsY"></a>
### 大众点评Cat
<a name="rNmkt"></a>
## 分布式事务
<a name="cTVNn"></a>
### SEATA
