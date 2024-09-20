Java SpringBoot Prometheus Grafana
<a name="jnxkO"></a>
## 一、背景
SpringBoot的应用监控方案比较多，SpringBoot+Prometheus+Grafana是目前比较常用的方案之一。它们三者之间的关系大概如下图：<br />![关系图](https://cdn.nlark.com/yuque/0/2022/png/396745/1647575544783-7e3df340-f653-4cf0-91f2-32878969c9da.png#averageHue=%23fcfbfb&clientId=u21207ffb-6cc8-4&from=paste&id=u57d7d62f&originHeight=624&originWidth=884&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u0cac6797-ba55-4c11-8306-e5c6ba42df2&title=%E5%85%B3%E7%B3%BB%E5%9B%BE "关系图")
<a name="iSxrW"></a>
## 二、开发SpringBoot应用
首先，创建一个SpringBoot项目，pom文件如下：
```xml
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-actuator</artifactId>
</dependency>
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-web</artifactId>
</dependency>

<dependency>
  <groupId>org.projectlombok</groupId>
  <artifactId>lombok</artifactId>
  <optional>true</optional>
</dependency>

<!-- https://mvnrepository.com/artifact/io.prometheus/simpleclient_spring_boot -->
<dependency>
  <groupId>io.prometheus</groupId>
  <artifactId>simpleclient_spring_boot</artifactId>
  <version>0.8.1</version>
</dependency>

<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-security</artifactId>
</dependency>
```
注意： 这里的SpringBoot版本是1.5.7.RELEASE，之所以不用最新的2.X是因为最新的simpleclient_spring_boot只支持1.5.X，不确定2.X版本的能否支持。<br />MonitorDemoApplication启动类增加注解
```java
import io.prometheus.client.spring.boot.EnablePrometheusEndpoint; 
import io.prometheus.client.spring.boot.EnableSpringBootMetricsCollector; 
import org.springframework.boot.SpringApplication; 
import org.springframework.boot.autoconfigure.SpringBootApplication; 
@EnablePrometheusEndpoint 
@EnableSpringBootMetricsCollector 
@SpringBootApplication 
public class MonitorDemoApplication { 
 
    public static void main(String[] args) { 
        SpringApplication.run(MonitorDemoApplication.class, args); 
    } 
 
} 
```
配置文件application.yml
```yaml
server: 
  port: 8848 
spring: 
  application: 
    name: monitor-demo 
 
security: 
  user: 
    name: admin 
    password: 1234 
  basic: 
    enabled: true 
    # 安全路径列表，逗号分隔，此处只针对/admin路径进行认证 
    path: /admin 
 
# actuator暴露接口的前缀 
management: 
  context-path: /admin 
  # actuator暴露接口使用的端口，为了和api接口使用的端口进行分离 
  port: 8888 
  security: 
    enabled: true 
    roles: SUPERUSER
```
测试代码TestController
```java
@RequestMapping("/heap/test")
@RestController
public class TestController {
    
    public static final Map<String, Object> map = new ConcurrentHashMap<>();
    
    @RequestMapping("")
    public String testHeapUsed() {
        for (int i = 0; i < 10000000; i++) {
            map.put(i + "", new Object());
        }
        return "ok";
    }
}
```
这里的逻辑就是在请求这个接口后，创建大量对象保存到map中增加堆内存使用量，方便后面测试邮件报警。<br />启动项目后，可以在IDEA中看到有很多Endpoints，如图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647575544888-bedee4ca-f0b2-4d4e-8950-6a5d5750cce2.png#averageHue=%233c4144&clientId=u21207ffb-6cc8-4&from=paste&id=ub4c3e917&originHeight=378&originWidth=869&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u54248b32-821b-4cb4-b333-ac00599bed6&title=)<br />启动完毕，访问http://localhost:8888/admin/prometheus就可以看到服务暴露的那些监控指标了。<br />![监控指标](https://cdn.nlark.com/yuque/0/2022/png/396745/1647575544849-ed09c2c1-3bfe-43b6-b08b-d238a6d0e84b.png#averageHue=%23f4f4f4&clientId=u21207ffb-6cc8-4&from=paste&id=u547cd964&originHeight=978&originWidth=589&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u5958b740-db92-4305-a75e-e5b51bb24e8&title=%E7%9B%91%E6%8E%A7%E6%8C%87%E6%A0%87 "监控指标")<br />注意：由于开启了安全认证，所以访问这个URL的需要提示输入账号/密码，如果提示404请检查下请求地址是否正确，如果不设置management.context-path则默认地址是http://ip:port/prometheus
<a name="neBgd"></a>
## 三、安装Prometheus
下载地址[https://prometheus.io/download/](https://prometheus.io/download/)，本文下载的是Windows版本prometheus-2.17.2.windows-amd64.tar.gz。<br />解压后修改prometheus.yml文件，配置数据采集的目标信息。
```yaml
scrape_configs: 
  # The job name is added as a label `job=<job_name>` to any timeseries scraped from this config. 
  # - job_name: 'prometheus' 
 
    # metrics_path defaults to '/metrics' 
    # scheme defaults to 'http'. 
 
    # static_configs: 
    # - targets: ['localhost:9090'] 
  - job_name: 'monitor-demo' 
    scrape_interval: 5s # 刮取的时间间隔 
    scrape_timeout: 5s  
    metrics_path: /admin/prometheus 
    scheme: http  
    basic_auth: #认证信息 
      username: admin 
      password: 1234 
    static_configs: 
      - targets: 
        - 127.0.0.1:8888  #此处填写 Spring Boot 应用的 IP + 端口号 
```
更多配置信息请查看官方文档。<br />现在可以启动Prometheus了，命令行输入：`prometheus.exe --config.file=prometheus.yml` 访问http://localhost:9090/targets，查看Spring Boot采集状态是否正常。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647575544870-d4483098-30d7-4b83-9210-a7571bef47b6.png#averageHue=%23fdfcfc&clientId=u21207ffb-6cc8-4&from=paste&id=u3dd138dc&originHeight=322&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=udd0f7009-ec2e-4479-9fde-c93ecf6b300&title=)<br />采集目标信息
<a name="E26PF"></a>
## 四、安装Grafana
下载地址[https://grafana.com/grafana/download](https://grafana.com/grafana/download)，这里用到的是Windows版本grafana-6.3.3.windows-amd64.zip。<br />解压后运行bin目录下的grafana-server.exe启动，游览器访问http://localhost:3000即可看到登录页面，默认账号密码是admin/admin。<br />现在开始创建自己的可视化监控面板。
<a name="WPUie"></a>
### 1、设置数据源
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647575545005-ddbf9ccf-fa08-4762-80ee-3eca48fa6625.png#averageHue=%23181a1c&clientId=u21207ffb-6cc8-4&from=paste&id=ub00e280d&originHeight=822&originWidth=1060&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1cdd1726-2d42-4b4f-acf8-849eccabd8a&title=)
<a name="HTzcg"></a>
### 2、创建一个Dashboard
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647575545264-714b3abb-9fa4-438b-bfda-759f4d9d0d6e.png#averageHue=%231d1d20&clientId=u21207ffb-6cc8-4&from=paste&id=Wo3t1&originHeight=333&originWidth=541&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8969076f-5c79-4899-adca-a86dc938527&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647575545368-05f56fdb-e94f-4e84-b559-85bd59058d3c.png#averageHue=%231f1f22&clientId=u21207ffb-6cc8-4&from=paste&id=u8b32ffe7&originHeight=476&originWidth=986&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8f0c8704-163f-4b09-8ece-8447d41b01d&title=)
<a name="yrv3Q"></a>
### 3、填写采集的指标点
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647575545302-bd741475-9de4-4247-8bac-575068d329d6.png#averageHue=%23171719&clientId=u21207ffb-6cc8-4&from=paste&id=u56562c61&originHeight=579&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ubb5451f6-b5cf-468b-9ff5-ca231e0c933&title=)<br />注意： 这里的指标点不能随便填，必须是已有的可以在 Prometheus看到。<br />![采集指标](https://cdn.nlark.com/yuque/0/2022/png/396745/1647575545339-da62d06d-455a-41e5-b86c-fc77403651c2.png#averageHue=%23fefefe&clientId=u21207ffb-6cc8-4&from=paste&id=u9a8db9f9&originHeight=489&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ud81ef93e-9720-4cb1-ad80-80cb750a0c4&title=%E9%87%87%E9%9B%86%E6%8C%87%E6%A0%87 "采集指标")
<a name="FPEIq"></a>
### 4、选择图表样式
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647575545620-5c68f4ae-5373-43c3-b399-340ec324cf80.png#averageHue=%23151618&clientId=u21207ffb-6cc8-4&from=paste&id=u7cc9c1ab&originHeight=419&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u05290ef2-12c7-4f5e-802e-0a5a92f1c11&title=)
<a name="HRNPk"></a>
### 5、填写标题描述
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647575545646-5f2032ee-99d5-42d3-92cb-88653351524e.png#averageHue=%2319191c&clientId=u21207ffb-6cc8-4&from=paste&id=u47d656b2&originHeight=554&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue88e66b2-15a6-473e-b3e4-0307078993a&title=)<br />最后点击右上角的保存，输入Dashboad的名称即可。<br />![结果展示](https://cdn.nlark.com/yuque/0/2022/png/396745/1647575545668-92b290af-c6cc-4737-97d7-c3fbeb70e679.png#averageHue=%23212124&clientId=u21207ffb-6cc8-4&from=paste&id=ubdcf5b0d&originHeight=479&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u2f65238e-a3f6-4ad7-84e9-f07d1077edc&title=%E7%BB%93%E6%9E%9C%E5%B1%95%E7%A4%BA "结果展示")<br />Tips：这里的图表布局是可以用鼠标拖动的
<a name="NuHmU"></a>
## 五、添加邮件报警
在实际项目中当监控的某的个指标超过阈值（比如CPU使用率过高），希望监控系统自动通过短信、钉钉和邮件等方式报警及时通知运维人员，Grafana就支持该功能。
<a name="D2rTC"></a>
### 第一步：点击[Alerting]——>[Notification channels]添加通知通道
![创建通道](https://cdn.nlark.com/yuque/0/2022/png/396745/1647575545761-3f283651-cce0-4a5e-a650-a7f4766fb29b.png#averageHue=%2318181a&clientId=u21207ffb-6cc8-4&from=paste&id=uf18a21dc&originHeight=330&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=uffd2b705-7be2-400a-af52-ea776694efb&title=%E5%88%9B%E5%BB%BA%E9%80%9A%E9%81%93 "创建通道")<br />![编辑](https://cdn.nlark.com/yuque/0/2022/png/396745/1647575545796-2e5b78b3-3ba3-4e6e-a8d0-07477f1cfe51.png#averageHue=%2317191c&clientId=u21207ffb-6cc8-4&from=paste&id=u50dade02&originHeight=830&originWidth=647&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u17263ce8-ba27-4a7f-af21-3cd91f57d18&title=%E7%BC%96%E8%BE%91 "编辑")<br />这里的Type有很多选项，包括webhook、钉钉等，这里以邮件为例。
<a name="hcwzE"></a>
### 第二步：邮箱配置
Grafana默认使用conf目录下defaults.ini作为配置文件运行，根据官方的建议不要更改defaults.ini而是在同级目录下新建一个配置文件custom.ini。以腾讯企业邮箱为例，配置如下：
```
#################################### SMTP / Emailing #####################
[smtp]
enabled = true
host = smtp.exmail.qq.com:465
user = xxxx@ininin.com
# If the password contains # or ; you have to wrap it with triple quotes. Ex """#password;"""
password = XXX
cert_file =
key_file =
skip_verify = true
from_address = xxxx@ininin.com
from_name = Grafana
ehlo_identity = ininin.com
```
然后需要重启Grafana，命令grafana-server.exe -config=E:\file\grafana-6.3.3\conf\custom.ini
<a name="WObvP"></a>
### 第三步：为指标添加alert
![配置预警规则](https://cdn.nlark.com/yuque/0/2022/png/396745/1647575546039-35019c73-d866-4dd2-8a32-fb88461c208f.png#averageHue=%231a1b1d&clientId=u21207ffb-6cc8-4&from=paste&id=ueeca2662&originHeight=495&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=uf720a68b-96d7-4eb4-beca-ab785e7ea6d&title=%E9%85%8D%E7%BD%AE%E9%A2%84%E8%AD%A6%E8%A7%84%E5%88%99 "配置预警规则")<br />配置预警规则<br />![配置通知方式和信息](https://cdn.nlark.com/yuque/0/2022/png/396745/1647575546105-b7096753-c35d-48fd-899d-423c0eb0d8e4.png#averageHue=%23121215&clientId=u21207ffb-6cc8-4&from=paste&id=ua963a36c&originHeight=511&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u860d252f-8b7e-4848-85d5-73defc06b62&title=%E9%85%8D%E7%BD%AE%E9%80%9A%E7%9F%A5%E6%96%B9%E5%BC%8F%E5%92%8C%E4%BF%A1%E6%81%AF "配置通知方式和信息")<br />配置通知方式和信息
> Evaluate every
> 表示检测评率，这里为了测试效果，改为1秒
> For
> 如果警报规则配置了For，并且查询违反了配置的阈值，那么它将首先从OK变为Pending。从OK到Pending Grafana不会发送任何通知。一旦警报规则的触发时间超过持续时间，它将更改为Alerting并发送警报通知。
> Conditions
> when 表示什么时间，of 表示条件，is above 表示触发值 同时，设置了is above后会有一条红线。
> If no data or all values are null
> 如果没有数据或所有值都为空，这里选择触发报警
> If execution error or timeout
> 如果执行错误或超时，这里选择触发报警

注意： 下一次触发，比如10秒后，它不会再次触发，防止报警风暴产生！
<a name="XS0fU"></a>
### 第四步：测试
请求http://localhost:8848/heap/test接口后，内存升高大于设置的阈值，然后就收到报警邮件。<br />![报警邮件](https://cdn.nlark.com/yuque/0/2022/png/396745/1647575546109-4717c235-9218-43b6-8a5c-b9dd61062146.png#averageHue=%237b7976&clientId=u21207ffb-6cc8-4&from=paste&id=u0e6ba75c&originHeight=493&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u004d8eda-2985-4ed2-aaaa-2ca205e7c3e&title=%E6%8A%A5%E8%AD%A6%E9%82%AE%E4%BB%B6 "报警邮件")
