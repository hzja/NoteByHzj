JavaSpringBoot
<a name="HPPCX"></a>
## 1、简介
当某个应用程序在生产环境中运行时，监控其运行状况是必要的。通过实时了解应用程序的运行状况，能在问题出现之前得到警告，也可以在客户注意到问题之前解决问题。<br />创建一个Spring Boot应用程序，在Spring Actuator，Micrometer，Prometheus和Grafana的帮助下来监控系统。其中，Spring Actuator和Micrometer是Spring Boot App的一部分。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1653656824992-e29622fe-1e34-4e13-a203-8a867dc9def5.jpeg)<br />简要说明了不同组件的目的：

- Spring Actuator：在应用程序里提供众多 Web 接口，通过它们了解应用程序运行时的内部状况。有关更多信息，请参见Spring Boot 2.0中的Spring Boot Actuator。
- Micrometer：为 Java 平台上的性能数据收集提供了一个通用的 API，它提供了多种度量指标类型（Timers、Guauges、Counters等），同时支持接入不同的监控系统，例如 Influxdb、Graphite、Prometheus 等。Spring Boot Actuator对此提供了支持。
- Prometheus：一个时间序列数据库，用于收集指标。
- Grafana：用于显示指标的仪表板。

下面，将分别介绍每个组件。
<a name="b4iJI"></a>
## 2、创建示例应用
首先要做的是创建一个可以监控的应用程序。通过Spring Initializr，并添加Spring Boot Actuator，Prometheus和Spring Web依赖项， 创建了一个如下所示的Spring MVC应用程序。
```java
@RestController
public class MetricsController {
	
	@GetMapping("/endPoint1")
	public String endPoint1() {
		return "Metrics for endPoint1";
	}
	
	@GetMapping("/endPoint2")
	public String endPoint2() {
		return "Metrics for endPoint2";
	}
}
```
启动应用程序：
```bash
$ mvn spring-boot:run
```
验证接口是否正常：
```bash
$ curl http://localhost:8080/endPoint1Metrics for endPoint1$ curl http://localhost:8080/endPoint2Metrics for endPoint2
```
验证Spring Actuator接口。为了使响应信息方便可读，通过`python -mjson.tool`来格式化信息。
```bash
$ curl http://localhost:8080/actuator | python -mjson.tool
...
{
"_links":{
"self":{
"href":"http://localhost:8080/actuator",
"templated":false
      },
"health":{
"href":"http://localhost:8080/actuator/health",
"templated":false
      },
"health-path":{
"href":"http://localhost:8080/actuator/health/{*path}",
"templated":true
      },
"info":{
"href":"http://localhost:8080/actuator/info",
"templated":false
      }
   }
}
```
默认情况下，会显示以上信息。除此之外，Spring Actuator可以提供更多信息，但是需要启用它。为了启用Prometheus，需要将以下信息添加到application.properties文件中。
```
management.endpoints.web.exposure.include=health,info,prometheus
```
重启应用程序，访问http://localhost:8080/actuator/prometheus从Prometheus拉取数据，返回了大量可用的指标信息。这里只显示输出的一小部分，因为它是一个很长的列表。
```bash
$ curl http://localhost:8080/actuator/prometheus
# HELP jvm_gc_pause_seconds Time spent in GC pause
# TYPE jvm_gc_pause_seconds summary
jvm_gc_pause_seconds_count{action="end of minor GC",cause="G1 Evacuation Pause",} 2.0
jvm_gc_pause_seconds_sum{action="end of minor GC",cause="G1 Evacuation Pause",} 0.009
...
```
如前所述，还需要Micrometer。Micrometer为最流行的监控系统提供了一个简单的仪表板，允许仪表化JVM应用，而无需关心是哪个供应商提供的指标。它的作用和SLF4J类似，只不过它关注的不是Logging（日志），而是application metrics（应用指标）。简而言之，它就是应用监控界的SLF4J。<br />Spring Boot Actuator为Micrometer提供了自动配置。Spring Boot2在spring-boot-actuator中引入了micrometer，对1.x的metrics进行了重构，另外支持对接的监控系统也更加丰富(Atlas、Datadog、Ganglia、Graphite、Influx、JMX、NewRelic、Prometheus、SignalFx、StatsD、Wavefront)。<br />更新后的application.properties文件如下所示：
```
management.endpoints.web.exposure.include=health,info,metrics,prometheus
```
重启应用程序，并从http://localhost:8080/actuator/metrics中检索数据。
```bash
$ curl http://localhost:8080/actuator/metrics | python -mjson.tool
...
{
"names": [
"http.server.requests",
"jvm.buffer.count",
"jvm.buffer.memory.used",
...
```
可以直接通过指标名来检索具体信息。例如，如果查询http.server.requests指标，可以按以下方式检索：
```bash
$ curl http://localhost:8080/actuator/metrics/http.server.requests | python -mjson.tool
...
{
"name": "http.server.requests",
"description": null,
"baseUnit": "seconds",
"measurements": [
        {
"statistic": "COUNT",
"value": 3.0
        },
        {
"statistic": "TOTAL_TIME",
"value": 0.08918682
        },
...
```
<a name="hzygd"></a>
## 3、添加Prometheus
Prometheus是Cloud Native Computing Foundation的一个开源监控系统。由于应用程序中有一个/actuator/Prometheus端点来供 Prometheus 抓取数据，因此现在可以配置Prometheus来监控Spring Boot应用程序。<br />Prometheus有几种安装方法，在本文中，将在Docker容器中运行Prometheus。<br />需要创建一个prometheus.yml文件，以添加到Docker容器中。
```yaml
global:
	scrape_interval: 15s
scrape_configs:
	- job_name: 'myspringmetricsplanet'
	metrics_path: '/actuator/prometheus'
	static_configs:
	- targets: ['HOST:8080']
```

- scrape_interval：Prometheus多久轮询一次应用程序的指标
- job_name：轮询任务名称
- metrics_path：指标的URL的路径
- targets：主机名和端口号。使用时，替换HOST为主机的IP地址

如果在Linux上查找IP地址有困难，则可以使用以下命令：
```bash
$ ip -f inet -o addr show docker0 | awk '{print $4}' | cut -d '/' -f 1
```
启动Docker容器并将本地prometheus.yml文件，映射到Docker容器中的文件。
```bash
$ docker run \
    -p 9090:9090 \
    -v /path/to/prometheus.yml:/etc/prometheus/prometheus.yml \
    prom/prometheus
```
成功启动Docker容器后，首先验证Prometheus是否能够通过 http://localhost:9090/targets收集数据。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1653656321530-108d8baf-0a46-4b77-86e1-5c75ae1f9619.jpeg#clientId=u8c03af43-b8a9-4&from=paste&id=u2f4ebb48&originHeight=84&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1fa075f5-7730-4392-80b3-3143bdf1404&title=)<br />如上图所示，遇到context deadline exceeded错误，造成Prometheus无法访问主机上运行的Spring Boot应用程序。如何解决呢？<br />可以通过将Docker容器添加到主机网络来解决此错误，这将使Prometheus能够访问Spring Boot应用程序。
```bash
$ docker run \
    --name prometheus \
    --network host \
    -v /path/to/prometheus.yml:/etc/prometheus/prometheus.yml \
    -d \
    prom/prometheus
```
再次验证，状态指示为UP。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1653656321564-b26c1ccf-bcfe-4358-bc4f-f9cf23bb3c7c.jpeg#clientId=u8c03af43-b8a9-4&from=paste&id=ufa6e662b&originHeight=66&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua403f394-bb45-47d9-bfc7-263927943a5&title=)<br />现在可以显示Prometheus指标。通过访问http://localhost:9090/graph，在搜索框中输入http_server_requests_seconds_max并单击“执行”按钮，将提供请求期间的最长执行时间。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653656321597-398e3973-ca7f-4e24-a734-a7227513f9f7.png#clientId=u8c03af43-b8a9-4&from=paste&id=u28ed6b3e&originHeight=463&originWidth=1024&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u801f7a63-b831-4961-8717-29ad1dfb89c&title=)
<a name="Kzejp"></a>
## 4、添加Grafana
最后添加的组件是Grafana。尽管Prometheus可以显示指标，但Grafana可以在更精美的仪表板中显示指标。Grafana也支持几种安装方式，也将在Docker容器中运行它。
```bash
$ docker run --name grafana -d -p 3000:3000 grafana/grafana
```
点击 http://localhost:3000/，就可以访问Grafana。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1653657711634-c63ebb2d-9035-43f9-bf49-9f0257009840.png#clientId=u8c03af43-b8a9-4&from=paste&height=622&id=u1e4573cd&originHeight=1556&originWidth=1697&originalType=binary&ratio=1&rotation=0&showTitle=false&size=853861&status=done&style=shadow&taskId=u65ad0d7e-bc54-4fdd-9e84-955f5ed1d70&title=&width=678.8)<br />默认的用户名/密码为admin/admin。单击“登录”按钮后，需要更改默认密码。<br />接下来要做的是添加一个数据源。单击左侧边栏中的“配置”图标，然后选择“Data Sources(数据源)”。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1653657812500-5909b949-bd80-496f-8e2c-a85b25dd2182.png#clientId=u8c03af43-b8a9-4&from=paste&height=713&id=uc261f4c1&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=435176&status=done&style=shadow&taskId=u0f1bc678-dae7-4cfd-a921-fa50ef56456&title=&width=1536)<br />单击Add data source（添加数据源）按钮。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1653657906713-f6ef6080-01dd-447f-83aa-220c025548a1.png#clientId=u8c03af43-b8a9-4&from=paste&height=713&id=uf09d7689&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=155790&status=done&style=shadow&taskId=uaaa7460c-e44d-4699-8fb9-ceb59a3d123&title=&width=1536)<br />Prometheus在列表的顶部，选择Prometheus。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1653657940863-1de93301-af64-49a4-aa94-60e82b4b0cab.png#clientId=u8c03af43-b8a9-4&from=paste&height=713&id=ue5b87b1e&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=220394&status=done&style=shadow&taskId=ua7db7cb2-2c37-4296-899c-52e25b78c4b&title=&width=1536)<br />填写可访问Prometheus的URL，将HTTP Access设置为Browser，然后单击页面底部的Save＆Test按钮。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1653657972984-000adfbb-e171-436e-83ba-06ad3431af79.png#clientId=u8c03af43-b8a9-4&from=paste&height=713&id=ubab7630c&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=204761&status=done&style=shadow&taskId=u10f6f993-9791-46ff-a43c-1eed0d924f5&title=&width=1536)<br />一切正常后，将显示绿色的通知标语，指示数据源正在工作。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1653657989647-1c025833-d7fd-4716-897f-dbbe1c90b4b7.png#clientId=u8c03af43-b8a9-4&from=paste&height=713&id=u3304b8f0&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=193627&status=done&style=shadow&taskId=uf4049808-31d6-42b4-9acf-a9be860111d&title=&width=1536)<br />现在该创建仪表板了。可以自定义一个，但也可以使用开源的仪表板。用于显示Spring Boot指标的一种常用仪表板是JVM仪表板。<br />在左侧边栏中，点击+号，然后选择导入。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1653658038795-02b3033c-12eb-4bb4-9ec4-ae80a5309b58.png#clientId=u8c03af43-b8a9-4&from=paste&height=713&id=ud78168f7&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=166948&status=done&style=shadow&taskId=u27146256-6f05-4d20-af3b-f883e5febd4&title=&width=1536)<br />输入JVM仪表板的URL https://grafana.com/grafana/dashboards/4701，然后单击“Load(加载)”按钮。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1653658079796-130501d7-236d-4bff-a12a-6b4ea7ef5ee8.png#clientId=u8c03af43-b8a9-4&from=paste&height=713&id=ue6da7f70&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=137711&status=done&style=shadow&taskId=u978844d8-e3a5-41c2-99ee-3ec4011e0ab&title=&width=1536)<br />为仪表板输入一个有意义的名称（例如MySpringMonitoringPlanet），选择Prometheus作为数据源，然后单击Import按钮。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1653658132007-bc165c42-19d1-4c72-bdd6-8e37d83f90cd.png#clientId=u8c03af43-b8a9-4&from=paste&height=713&id=u32991bde&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=200715&status=done&style=shadow&taskId=u3afa2402-8a01-47aa-855a-968d5e9a63e&title=&width=1536)<br />到目前为止，就可以使用一个很酷的Grafana仪表板。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1653656322600-a39e8594-70a3-4aa4-95ec-71e9da29b896.jpeg#clientId=u8c03af43-b8a9-4&from=paste&id=ude465099&originHeight=311&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u507501cb-5c9e-4f3f-8597-a16c825aebb&title=)<br />也可以将自定义面板添加到仪表板。在仪表板顶部，单击Add panel（添加面板）图标。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1653658189292-a4d01409-479a-4705-927e-d9724144ac86.png#clientId=u8c03af43-b8a9-4&from=paste&height=125&id=u990608c9&originHeight=313&originWidth=1421&originalType=binary&ratio=1&rotation=0&showTitle=false&size=23446&status=done&style=shadow&taskId=u5ec326a9-b78f-42e5-8348-4c6fe130a9f&title=&width=568.4)<br />单击Add new panel（添加新面板）。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1653658216168-0013d48a-aac1-4762-9ba0-d604f0fc6b32.png#clientId=u8c03af43-b8a9-4&from=paste&height=417&id=uaedfe098&originHeight=1042&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=121954&status=done&style=shadow&taskId=uf133f794-8efe-4ec9-992f-698de771105&title=&width=1536)<br />在Metrics 字段中，输入http_server_requests_seconds_max，在右侧栏中的Panel title字段中，可以输入面板的名称。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1653656322752-bab2abb1-d3f8-4b59-861c-6d714b136e30.jpeg#clientId=u8c03af43-b8a9-4&from=paste&id=u47b7e44e&originHeight=277&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc5b5b297-8fb3-4998-98dd-770d87b6630&title=)<br />最后，单击右上角的Apply 按钮，面板将添加到仪表板。不要忘记保存仪表板。<br />为应用程序设置一些负载，并查看仪表板上的http_server_requests_seconds_max指标发生了什么。
```bash
$ watch -n 5 curl http://localhost:8080/endPoint1$ watch -n 10 curl http://localhost:8080/endPoint2
```
![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1653656322868-34175e3a-6672-46bd-9687-28b229d2953f.jpeg#clientId=u8c03af43-b8a9-4&from=paste&id=u1ae60b07&originHeight=222&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u728b8b9f-da01-4996-9a70-e0541e4fe05&title=)
<a name="fgvFd"></a>
## 5、结论
学习了如何为Spring Boot应用程序添加一些基本监控。这非常容易，只需要通过将Spring Actuator，Micrometer，Prometheus和Grafana组合使用。当然，这只是一个起点，但是从这里开始，可以为Spring Boot应用程序扩展和配置更多、更具体的指标。
