Nginx 负载均衡
<a name="ArsaN"></a>
## 负载均衡
Nginx的负载均衡使用upstream定义服务器组，后面跟着组名，组名后面是大括号包起来的服务器列表，每个服务器使用server开头，后面跟定义的服务器名字、服务器ip:port、参数；一个基本的upstream模块如下：
```nginx
upstream [服务器组名称]{
　　server [IP地址]:[端口号];
　　server [IP地址]:[端口号];
　　....
}
```
在upstream模块定义完成后，要让指定的访问反向代理到服务器列表，格式如下：
```nginx
location ~ .*$ {
　　index index.jsp index.html;
　　proxy_pass http://[服务器组名称];
}
```
<a name="Ol803"></a>
### SpringBoot项目准备
在进行Nginx负载均衡配置前，先搭建一个简单的后端应用项目：nginx-upstream
<a name="LUZmK"></a>
#### pom.xml文件
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 https://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <parent>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-parent</artifactId>
        <version>2.3.4.RELEASE</version>
        <relativePath/> <!-- lookup parent from repository -->
    </parent>
    <groupId>com.fcant</groupId>
    <artifactId>nginxloadbalance</artifactId>
    <version>0.0.1-SNAPSHOT</version>
    <name>NginxLoadBalance</name>
    <description>Demo project for Spring Boot</description>

    <properties>
        <java.version>1.8</java.version>
    </properties>

    <dependencies>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-web</artifactId>
        </dependency>

        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-test</artifactId>
            <scope>test</scope>
            <exclusions>
                <exclusion>
                    <groupId>org.junit.vintage</groupId>
                    <artifactId>junit-vintage-engine</artifactId>
                </exclusion>
            </exclusions>
        </dependency>
    </dependencies>

    <build>
        <plugins>
            <plugin>
                <groupId>org.springframework.boot</groupId>
                <artifactId>spring-boot-maven-plugin</artifactId>
            </plugin>
        </plugins>
    </build>

</project>
```
<a name="wRvYX"></a>
#### 控制器controller
```java
 package com.fcant.nginxloadbalance.controller;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

/**
 * HelloController
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 上午 10:21 2020/9/21/0021
 */
@RestController
public class HelloController {

    @GetMapping("/hello")
    public String hello() {
        return "Hello, I'm 8087";
    }
}

```
<a name="jLyPR"></a>
#### application.yml配置文件
```yaml
server:
  port: 8086
#  port: 8087
spring:
  application:
    name: nginx-upstream
```
<a name="ENitx"></a>
#### SpringBoot启动类
```java
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
@SpringBootApplication
public class Application {
    public static void main(String[] args) {
        SpringApplication.run(Application.class, args);
    }
}
```
因为只有一台电脑，为了演示Nginx负载均衡，后文会通过修改application.yml配置文件的端口和控制器controller的返回值来标识不同的应用服务器。<br />验证项目是否搭建成功
```http
http://127.0.0.1:8086/hello
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600665800662-db49c2bb-abe1-4e01-953e-61bf7d800da7.png#height=139&id=BEHZq&originHeight=417&originWidth=1230&originalType=binary&ratio=1&rotation=0&showTitle=false&size=54959&status=done&style=shadow&title=&width=410)
```http
http://127.0.0.1:8087/hello
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600665776682-c8c0a0f6-7a14-40a8-97b7-5c26ea12a53e.png#height=170&id=HfrZ3&originHeight=511&originWidth=1233&originalType=binary&ratio=1&rotation=0&showTitle=false&size=55263&status=done&style=shadow&title=&width=411)
<a name="sMxwp"></a>
### 配置负载均衡
以下命令均在root用户下操作<br />进入Nginx的配置文件目录
```bash
cd /usr/local/nginx/conf/
vim nginx.conf
```
<a name="o7DFa"></a>
#### 增加upstream模块配置
```nginx
upstream myserver {
     server 192.168.56.1:8086;
     server 192.168.56.1:8087;
}
```
注意在http模块添加upstream<br />upstream可以有多个，名称不同即可。
<a name="GwPZK"></a>
#### 配置location转发规则
```nginx
location / {   #首页负载之后端服务器
     proxy_pass  http://myserver;  #通过upstrean定义的服务器组名调用后端服务器
     proxy_set_header X-Real-IP $remote_addr;  #传递客户端的ip地址
}
```
配置完成后可以使用如下命令启动Nginx
```bash
/usr/local/nginx/sbin/nginx -c \
/usr/local/nginx/conf/nginx.conf
```
<a name="2W6IZ"></a>
### 验证
访问http://192.168.56.9/hello<br />返回出现的结果是随意的，有可能访问192.168.56.1:8086，可有可能192.168.56.1:8087。
<a name="mlVtc"></a>
### Nginx的负载均衡算法
这样就完成了最基本的负载均衡，但是这并不能满足实际需求。目前Nginx的upstream模块支持6种方式的负载均衡策略（算法）：<br />轮询（默认方式）、<br />weight（权重方式）、<br />ip_hash（依据ip分配方式）、<br />least_conn（最少连接方式）、<br />fair（第三方提供的响应时间方式）、<br />url_hash（第三方通过的依据URL分配方式）。
<a name="J6PeT"></a>
#### 1）轮询
最基本的配置方法，是upstream模块默认的负载均衡策略。每个请求会按时间顺序平均分配到不同的后端服务器。有如下参数：

- fail_timeout: 与max_fails结合使用
- max_fails: 在fail_timeout参数设置的时间内最大失败次数。如果在这个时间内，所有该服务器的请求都失败了，那么认为该服务器停机
- fail_time: 服务器被认为停机的时长，默认10s（被认为停机的服务器尝试间隔？）
- backup: 标记该服务器为备用服务器。当主服务器停止时，请求会被发送到它这里
- down: 标记服务器永久停机
:::danger
注意：

- down标记的服务器会自动剔除；
- 缺省就是轮询；
- 此策略适合服务器配置无状态且短平块的服务使用
:::
<a name="dut7Z"></a>
#### 2）weight
权重方式，在轮询策略的基础上指定轮询的几率。也可以认为是在轮询的基础上新增了一个weight的参数，此参数指定轮询的几率，值为number。upstream模块配置模板如下：
```nginx
upstream [服务器组名称]{
　　server [IP地址]:[端口号] weight=2;
　　server [IP地址]:[端口号];
　　....
}
```
在该例子中，没有weight参数的服务器默认为1，weight的数值与访问比例成正比，所有weight值的总和为一个循环单位，服务器自身的weight值为循环单位内的轮询次数。
:::danger
注意：

- 权重越高分配到的请求越多；
- 此策略可以和least_conn策略、iphash策略结合使用；
- 此策略比较适合服务器硬件配置差距较大的情况。
:::
<a name="gYdiA"></a>
#### 3）ip_hash
依据ip分配方式，指定负载均衡器按照基于客户端IP的分配方式，这个方法确保了相同的客户端请求一致发送到相同的服务器，以保证session会话。这样每个访客都固定访问一个后端服务器，可以解决session不能跨服务器的问题。upstream模块配置模板如下：
```nginx
upstream [服务器组名称]{
　　ip_hash;
　　server [IP地址]:[端口号] weight=2;
　　server [IP地址]:[端口号];
　　....
}
```
:::danger
注意：

- nginx1.3.1之前的版本不能在ip_hash中使用权重（weight）；
- ip_hash不能与backup同时使用；
- 此策略适合有状态服务的程序，比如session；
- 当有服务器需要剔除，必须手动down掉。
:::
<a name="JIVZq"></a>
#### 4）least_conn
　　最少连接方式，把请求发给链接数最少的后端服务器。轮询是把请求平均分配给各个后端，使它们的负载大致相同。但是，有些请求占用的时间很长，会导致其所在的后端负载较高。这种情况下，least_conn这种方式就可以达到更好的负载均衡效果。upstream模块配置模板如下：
```nginx
upstream [服务器组名称]{
　　least_conn;
　　server [IP地址]:[端口号] weight=2;
　　server [IP地址]:[端口号];
　　....
}
```
:::danger
注意：此策略适合请求处理时间长短不一造成的服务器过载情况;
:::
<a name="rWR6P"></a>
#### 5）fair
　　响应时间方式，按照服务器端的响应时间来分配请求，响应时间短的优先分配。upstream模块配置模板如下：
```nginx
upstream [服务器组名称]{
　　server [IP地址]:[端口号] weight=2;
　　server [IP地址]:[端口号];
　　....
　　fair;
}
```
:::danger
注意：需要安装第三方插件;
:::
<a name="YqBe7"></a>
#### 6）url_hash
url分配方式，按访问url的hash结果来分配请求，使每个url定向到同一个后端服务器，要配合缓存命中来使用。同一个资源多次请求可能会到达不同的服务器上，导致不必要的多次下载，缓存命中率不高，以及一些资源时间的浪费。而使用url_hash，可以使得同一个url（也就是同一个资源请求）会到达同一台服务器，一旦缓存住了资源，再次收到请求，就可以在缓存中读取。upstream模块配置模板如下：
```nginx
upstream [服务器组名称]{
　　hash $request_uri;
　　server [IP地址]:[端口号] weight=2;
　　server [IP地址]:[端口号];
　　....
}
```
:::danger
注意：

- 需要安装第三方插件；
- uri，是i，不是小写的L。
:::
<a name="wkxQ9"></a>
### upstream相关变量
<a name="PvSxT"></a>
#### 变量名：`$upstream_addr`
功能：该变量表示了处理该请求的 upstream 中 server 的地址
<a name="argOz"></a>
#### 变量名：`$upstream_cache_status`
功能：该变量出现在 Nginx 0.8.3 版本中， 可能的值如下：

- MISS - 缓存中未被命中
- EXPIRED - 生存期期满，请求被传递到后端服务器
- UPDATING - 生存期满，陈旧的响应被使用，因为proxy/fastcgi_cache_use_stale 升级
- STALE - 生存期期满，陈旧的响应被使用，因为 proxy/fastcgi_cache_use_stale
- HIT - 缓存命中
<a name="dA6f4"></a>
#### 变量名：`$upstream_status`
功能：该变量为 upstream 中 server 的响应状态
<a name="tTocS"></a>
#### 变量名：`$upstream_response_time`
功能：upstream server 响应的时间，单位为秒，能够精准到毫秒。如果有多个 server 响应回答，那么会用逗号和冒号分隔开
<a name="IpzbQ"></a>
#### 变量名：`$upstream_http_$HEADER`
功能：HTTP 协议头。例如：`$upstream_http_host`
<a name="hcIUe"></a>
## 负载均衡时记录处理流量的服务
使用`$upstream_addr`变量可以获取负载服务的地址
```nginx
http {
    #设定mime类型,类型由mime.type文件定义
    include    mime.types;
    default_type  application/octet-stream;
    #设定日志格式
    log_format  main  '$remote_addr - $remote_user [$time_local] "$request" '
                      '$status $body_bytes_sent "$http_referer" '
                      '"$http_user_agent" "$http_x_forwarded_for" "$upstream_addr"';

    access_log  logs/access.log  main;
```
