Java SpringCloud Eureka
<a name="Kl3u1"></a>
# 1、Spring Cloud Eureka

- Eureka Client：服务注册
- Eureka Server：服务发现
<a name="OxAVE"></a>
# 2、Eureka Server
<a name="qY5c3"></a>
## A.Eureka Server依赖
```xml
<dependency>
		<groupId>org.springframework.cloud</groupId>
		<artifactId>spring-cloud-starter-netflix-eureka-server</artifactId>
</dependency>
```
<a name="x3qsP"></a>
## B.在启动类添加Eureka Server服务端注解
```java
package com.fcant.springcloudeurekasever;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.cloud.netflix.eureka.server.EnableEurekaServer;

/**
 * SpringCloudEurekaSeverApplication 
 * 启用EurekaServer服务端注解
 *
 * encoding:UTF-8
 * @author Fcant 22:42 2019/12/8
 */
@EnableEurekaServer
@SpringBootApplication
public class SpringCloudEurekaSeverApplication {

	public static void main(String[] args) {
		SpringApplication.run(SpringCloudEurekaSeverApplication.class, args);
	}

}
```
<a name="86GSs"></a>
## C.在配置文件配置服务端端口和Eureka服务相关配置
```yaml
server:
  port: 8000
spring:
  application:
    name: eureka-server
eureka:
  client:
    # 此应用为注册中心，false：不向注册中心注册自己
    register-with-eureka: false
    # 注册中心职责是维护服务实例，false：不检索服务
    fetch-registry: false
    service-url:
      defaultZone: http://127.0.0.1:8000/eureka
```
如果不配置eureka.client则会出现如下报错<br />[语雀内容](https://www.yuque.com/fcant/java/knntqt?view=doc_embed)
<a name="pdjOg"></a>
## D.输入服务端访问的URL可以查看提供的Web管理的内容
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1575817634852-26797219-1260-4ad8-af30-0234de03e810.png#align=left&display=inline&height=727&originHeight=727&originWidth=1366&size=98922&status=done&style=none&width=1366)
<a name="VsETx"></a>
# 3、GateWay Eureka Client
<a name="ArJzA"></a>
## A.Eureka Client相关依赖
```xml
<dependency>
    <groupId>org.springframework.cloud</groupId>
    <artifactId>spring-cloud-starter-netflix-eureka-client</artifactId>
</dependency>
```
<a name="srw7I"></a>
## B.在客户端启动类添加Eureka Client注解
<a name="cIgks"></a>
### I.直接在原有注解上添加`@EnableEurekaClient`注解
```java
package com.fcant.springcloudgateway;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.cloud.netflix.eureka.EnableEurekaClient;
import org.springframework.cloud.netflix.zuul.EnableZuulProxy;

/**
 * SpringCloudGateWayApplication
 *
 * encoding:UTF-8
 * @author Fcant 21:50 2019/12/8
 */
@EnableZuulProxy
@EnableEurekaClient
@SpringBootApplication
public class SpringCloudGateWayApplication {

	public static void main(String[] args) {
		SpringApplication.run(SpringCloudGateWayApplication.class, args);
	}

}

```
<a name="H9iry"></a>
### II.使用`@SpringCloudApplication`注解替代`@EnableEurekaClient`和`@SpringBootApplication`注解
```java
package com.fcant.springcloudgateway;

import org.springframework.boot.SpringApplication;
import org.springframework.cloud.client.SpringCloudApplication;
import org.springframework.cloud.netflix.zuul.EnableZuulProxy;

/**
 * SpringCloudGateWayApplication
 *
 * encoding:UTF-8
 * @author Fcant 21:50 2019/12/8
 */
@EnableZuulProxy
@SpringCloudApplication
public class SpringCloudGateWayApplication {

	public static void main(String[] args) {
		SpringApplication.run(SpringCloudGateWayApplication.class, args);
	}

}
```
<a name="YPaBI"></a>
## C.在配置文件配置客户端内容-application.yml
```yaml
zuul:
  routes:
    fcant:
      path: /fcant-filter
spring:
  application:
    name: gateway-service
eureka:
  client:
    service-url:
      defaultZone: http://127.0.0.1:8000/eureka
```
<a name="cUn5F"></a>
## D.启动客户端、刷新Server端程序
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1575819454593-88213b27-5f75-45ed-a383-01518e5d87d6.png#align=left&display=inline&height=728&originHeight=728&originWidth=1366&size=185525&status=done&style=none&width=1366)<br />![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1575819392585-7212990c-b165-439e-9c3f-9028858110fa.png#align=left&display=inline&height=728&originHeight=728&originWidth=1366&size=126609&status=done&style=none&width=1366)
<a name="USsXF"></a>
# 4、Consul&Eureka对比

- Consul ：保证强一致性 
   - 服务注册相比Eureka会稍慢一些，Consul要求过半的节点都写入成功。
   -  Leader 挂掉时，重新选举期整个Consul不可用。
- Eureka ：保证高可用 
   - 服务注册快，不需要等待注册信息复制到其他节点，也不保证复制成功。
   - 当注册信息不相同时，每个Eureka节点依然能够正常对外提供服务。
