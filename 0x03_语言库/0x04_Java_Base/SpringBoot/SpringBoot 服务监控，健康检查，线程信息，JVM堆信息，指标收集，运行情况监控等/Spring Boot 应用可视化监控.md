Java SpringBoot<br />![2021-08-25-16-46-12-226693.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629881200496-9e72295b-3ab4-47f9-921f-073428ebed20.png#clientId=u1a1197f7-185f-4&from=ui&id=ua417a1f0&originHeight=532&originWidth=1080&originalType=binary&ratio=1&size=90629&status=done&style=shadow&taskId=ua9578cb3-2075-4dcc-bb84-62b8480f68c)
<a name="mMCAH"></a>
#### 1、Spring Boot 应用暴露监控指标【版本 1.5.7.RELEASE】
首先，添加依赖如下依赖：
```xml
<dependency>       
   <groupId>org.springframework.boot</groupId>     
   <artifactId>spring-boot-starter-actuator</artifactId>  
</dependency>   
```
采集应用的指标信息，我们使用的是prometheus,相应的我们引入包:
```xml
<dependency>          
  <groupId>io.prometheus</groupId>    
  <artifactId>simpleclient_spring_boot</artifactId>                     
  <version>0.0.26</version>    
</dependency>
```
然后，在启动类 Application.java 添加如下注解：
```java
@SpringBootApplication
@EnablePrometheusEndpoint
@EnableSpringBootMetricsCollector
public class Application {  
    public static void main(String[] args) {        
        SpringApplication.run(Application.class, args);                                 
    }
}
```
最后，配置默认的登录账号和密码，在 application.yml 中：
```yaml
security:
  user:
    name:user
    password: pwd
```
启动应用程序后，会看到如下一系列的 Mappings<br />![2021-08-25-16-46-12-403691.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1629881213182-7e9038e4-86c6-42bb-a449-27440ba03798.jpeg#clientId=u1a1197f7-185f-4&from=ui&id=u83aca41a&originHeight=594&originWidth=1080&originalType=binary&ratio=1&size=92953&status=done&style=none&taskId=u29f34122-af78-4aa8-a5aa-d3274953eb8)<br />利用账号密码访问 http://localhost:8080/application/prometheus ，可以看到 Prometheus 格式的指标数据 <br />![2021-08-25-16-46-12-571700.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1629881213189-a76cae13-fc64-4d29-98e1-1994965ff7b9.jpeg#clientId=u1a1197f7-185f-4&from=ui&id=J9NaP&originHeight=582&originWidth=1080&originalType=binary&ratio=1&size=54937&status=done&style=shadow&taskId=uf3ae8a67-8dc7-491b-8547-34b45d7b956)
<a name="ePhFa"></a>
#### 2、Prometheus 采集 Spring Boot 指标数据
首先，获取 Prometheus 的 Docker 镜像：
```bash
$ docker pull prom/prometheus
```
然后，编写配置文件 prometheus.yml ：
```yaml
global:
  scrape_interval: 10s
  scrape_timeout: 10s
  evaluation_interval: 10m
scrape_configs:
  - job_name: spring-boot
    scrape_interval: 5s
    scrape_timeout: 5s
    metrics_path: /application/prometheus
    scheme: http
    basic_auth:
      username: admin
      password: 123456
    static_configs:
      - targets:
        - 192.168.11.54:8099 #此处填写 Spring Boot 应用的 IP + 端口号
```
接着，启动 Prometheus :
```bash
docker run -d --name prometheus -p 9090:9090 -v D:\test\actuator\prometheus\prometheus.yml:/etc/prometheus/prometheus.yml prom/prometheus
```
:::danger
请注意，D:\test\actuator\prometheus\prometheus.yml ，是配置文件存放地址，需要将它放到容器里面去，所以用了-v来做文件映射。/etc/prometheus/prometheus.yml这个是容器启动的时候去取的默认配置，这里是直接覆盖掉了它。prom/prometheus这是镜像，如果本地没有，就设置好的镜像仓库去取。
:::
启动完成后用`docker ps`看下是否已经启动成功，之后打开浏览器输入：<br />http://localhost:9090/targets，检查 Spring Boot 采集状态是否正常,如果看到下图就是成功了。<br />![2021-08-25-16-46-12-708691.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1629881213177-c9c50d7d-eed6-490d-9fc7-80abb7ec7935.jpeg#clientId=u1a1197f7-185f-4&from=ui&id=yqrpN&originHeight=214&originWidth=1080&originalType=binary&ratio=1&size=17074&status=done&style=shadow&taskId=u29855902-16a4-45af-9941-b1b7106cb94)
<a name="Z5xkM"></a>
#### 3、Grafana 可视化监控数据
首先，获取 Grafana 的 Docker 镜像：
```bash
$ docker pull grafana/grafana1
```
然后，启动 Grafana：
```bash
$ docker run --name grafana -d -p 3000:3000 grafana/grafana1
```
接着，访问 http://localhost:3000/ 配置 Prometheus 数据源：<br />Grafana 登录账号 admin 密码 admin

1. 先配置数据源.

![2021-08-25-16-46-12-922692.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629881251511-c7dfde0d-675a-465d-ab86-9a86126d58e7.png#clientId=u1a1197f7-185f-4&from=ui&id=u85841c8b&originHeight=886&originWidth=1080&originalType=binary&ratio=1&size=191549&status=done&style=none&taskId=uce92b0a5-6959-456b-bec3-3f882e09b18)

2. 配置单个指标的可视化监控面板：

![2021-08-25-16-46-13-044692.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1629881251478-4963c305-0bc9-4748-b4f2-a622ebfec6b9.jpeg#clientId=u1a1197f7-185f-4&from=ui&id=uSGSE&originHeight=316&originWidth=1080&originalType=binary&ratio=1&size=29249&status=done&style=none&taskId=uc7fcb3eb-9083-49a4-9dbb-6b9ae81aaf4)<br />![2021-08-25-16-46-13-149696.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1629881251502-67819607-40ad-4c5a-a202-94cc468b89d5.jpeg#clientId=u1a1197f7-185f-4&from=ui&id=duaJ8&originHeight=274&originWidth=1080&originalType=binary&ratio=1&size=18388&status=done&style=none&taskId=u48e4d263-92b8-4688-b6f1-57cc422197b)<br />![2021-08-25-16-46-13-267695.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1629881251505-59c7eb37-e0fb-4258-aac5-bf26f153baec.jpeg#clientId=u1a1197f7-185f-4&from=ui&id=P7qdE&originHeight=546&originWidth=1080&originalType=binary&ratio=1&size=43788&status=done&style=none&taskId=u29402129-7944-4fb4-ad2d-bb9be16d14b)<br />prometh采集的数据<br />![2021-08-25-16-46-13-440699.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629881251518-7906e62b-bb44-4c69-95d1-9ee97efabfb8.png#clientId=u1a1197f7-185f-4&from=ui&id=jCZBo&originHeight=464&originWidth=1080&originalType=binary&ratio=1&size=55456&status=done&style=shadow&taskId=u07f10b03-f051-4f3e-b44b-1973d96e5bd)<br />![2021-08-25-16-46-13-596697.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1629881252258-dc4bb1e3-8b2f-4ca4-8b9f-940e49a480d5.jpeg#clientId=u1a1197f7-185f-4&from=ui&id=o0kqb&originHeight=570&originWidth=1080&originalType=binary&ratio=1&size=58019&status=done&style=none&taskId=ua69e1628-356f-4e5e-adda-b48af37955f)

