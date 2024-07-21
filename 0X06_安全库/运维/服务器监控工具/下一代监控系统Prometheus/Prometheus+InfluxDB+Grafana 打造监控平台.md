运维 监控平台 Prometheus InfluxDB Grafana<br />在性能监控图谱中，有操作系统、应用服务器、中间件、队列、缓存、数据库、网络、前端、负载均衡、Web 服务器、存储、代码等很多需要监控的点。显然这些监控点不能在一个专栏中全部覆盖并一一细化，只能找最常用的几个，做些逻辑思路的说明，同时也把具体的实现描述出来。如果遇到了其他的组件，也需要一一实现这些监控。<br />主要想说明白下图的这个监控逻辑。<br />![2021-08-28-08-17-50-998749.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630110332945-45a6b29f-c9f8-4578-9174-fc26e468a88e.png#clientId=ubd68d543-5179-4&from=ui&id=ub7638be3&originHeight=583&originWidth=1080&originalType=binary&ratio=1&size=67893&status=done&style=shadow&taskId=ub2790103-2b6a-4f8f-b2d9-891d15c9e75)<br />这应该是现在最流行的一套监控逻辑了吧。把常见的使用 Grafana、Prometheus、InfluxDB、Exporters 的数据展示方式说一下，如果刚进入性能测试领域，也能有一个感性的认识。<br />有测试工具，有监控工具，才能做后续的性能分析和瓶颈定位，所以有必要把这些工具的逻辑理一理。<br />所有做性能的人都应该知道一点，不管数据以什么样的形式展示，最要紧的还是看数据的来源和含义，以便做出正确的判断。<br />先说明一下 JMeter 和 node_exporter 到 Grafana 的数据展示逻辑。至于其他的 Exporter，就不再解释这个逻辑了，只说监控分析的部分。
<a name="wjCrm"></a>
## JMeter+InfluxDB+Grafana 的数据展示逻辑
一般情况下，用 JMeter 做压力测试时，都是使用 JMeter 的控制台来查看结果。如下图所示：<br />![2021-08-28-08-17-52-458064.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630110352285-f46f7e61-ec61-42c9-bd79-3ffb625123d3.png#clientId=ubd68d543-5179-4&from=ui&id=u46ca5441&originHeight=361&originWidth=1080&originalType=binary&ratio=1&size=535576&status=done&style=none&taskId=u1648d5c6-e18a-4e2c-95d9-5602a9b51c7)<br />或者装个插件来看结果：<br />![2021-08-28-08-17-52-694082.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630110351877-8a4a7ecd-a37d-4b11-8526-7df1b461f37b.png#clientId=ubd68d543-5179-4&from=ui&id=HZ3ue&originHeight=460&originWidth=1080&originalType=binary&ratio=1&size=426873&status=done&style=shadow&taskId=u356fa45c-af2e-4c85-a960-3713778fc99)<br />或者用 JMeter 来生成 HTML：<br />![2021-08-28-08-17-53-461067.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630110350577-88cfdbc1-463f-4ffb-825a-3204f7ea1d46.png#clientId=ubd68d543-5179-4&from=ui&id=ACsNu&originHeight=555&originWidth=1001&originalType=binary&ratio=1&size=165082&status=done&style=shadow&taskId=u8829ae16-c086-4126-91d9-084a6a99157)<br />这样看都没有问题，在前面也强调过，对于压力工具来说，最多只关心三条曲线的数据：TPS（T 由测试目标定义）、响应时间、错误率。这里的错误率还只是辅助排查问题的曲线，没有问题时，只看 TPS 和响应时间即可。<br />不过采取以上三种方式有几个方面的问题。<br />整理结果时比较浪费时间。<br />在 GUI 用插件看曲线，做高并发时并不现实。<br />在场景运行时间比较长的时候，采用生成 HTML 的方式，会出现消耗内存过大的情况，而实际上，在生成的结果图中，有很多生成的图并不是那么关注。<br />生成的结果保存之后再查看比较麻烦，还要一个个去找。<br />那么如何解决这几个问题呢？<br />用 JMeter 的 Backend Listener 实时发送数据到 InfluxDB 或 Graphite 可以解决这样的问题。<br />Graphite Backend Listener 的支持是在 JMeter 2.13 版本，InfluxdDB Backend Listener 的支持是在 JMeter 3.3 的版本，它们都是用异步的方式把数据发送出来，以便查看。<br />其实有这个 JMeter 发送给 InfluxDB 的数据之后，不需要看上面的那些 HTML 数据，也可以直观地看到系统性能的性能趋势。<br />并且这样保存下来的数据，在测试结束后想再次查看也比较方便比对。<br />JMeter+InfluxDB+Grafana 的结构如下：<br />![2021-08-28-08-17-53-550063.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630110372673-991ec010-7689-4326-b19e-1e39f18e7e2e.png#clientId=ubd68d543-5179-4&from=ui&id=u9b35f553&originHeight=501&originWidth=1080&originalType=binary&ratio=1&size=56558&status=done&style=shadow&taskId=ub0467255-9ab0-475e-9ce3-993fc69cf20)<br />在这个结构中，JMeter 发送压力到服务器的同时，统计下 TPS、响应时间、线程数、错误率等信息。默认每 30 秒在控制台输出一次结果（在 jmeter.properties 中有一个参数 #summariser.interval=30 可以控制）。<br />配置了 Backend Listener 之后，将统计出的结果异步发送到 InfluxDB 中。最后在 Grafana 中配置 InfluxDB 数据源和 JMeter 显示模板。<br />然后就可以实时查看 JMeter 的测试结果了，这里看到的数据和控制台的数据是一样。<br />下面来说一下，数据的传输和展示逻辑。
<a name="PV3OS"></a>
## JMeter 中 Backend Listener 的配置
下面就 InfluxDB 的 Backend Listener 做个说明。它的配置比较简单，在脚本中加上即可。<br />![2021-08-28-08-17-53-700065.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630110384886-abbb6558-0118-4aa4-a6b7-4801d5e62846.png#clientId=ubd68d543-5179-4&from=ui&id=uf360a036&originHeight=274&originWidth=1080&originalType=binary&ratio=1&size=103528&status=done&style=shadow&taskId=ufee7f77e-7ccf-4cf3-bff5-5afb93f7b02)<br />先配置好 influxdb Url、application 等信息，application 这个配置可以看成是场景名。<br />那么 JMeter 如何将数据发给 InfluxDB 呢？请看源码中的关键代码，如下所示：
```java
private void addMetrics(String transaction, SamplerMetric metric) {
    // FOR ALL STATUS
    addMetric(transaction, metric.getTotal(), metric.getSentBytes(), metric.getReceivedBytes(), TAG_ALL, metric.getAllMean(), metric.getAllMinTime(),
              metric.getAllMaxTime(), allPercentiles.values(), metric::getAllPercentile);
    // FOR OK STATUS
    addMetric(transaction, metric.getSuccesses(), null, null, TAG_OK, metric.getOkMean(), metric.getOkMinTime(),
              metric.getOkMaxTime(), okPercentiles.values(), metric::getOkPercentile);
    // FOR KO STATUS
    addMetric(transaction, metric.getFailures(), null, null, TAG_KO, metric.getKoMean(), metric.getKoMinTime(),
              metric.getKoMaxTime(), koPercentiles.values(), metric::getKoPercentile);


    metric.getErrors().forEach((error, count) -> addErrorMetric(transaction, error.getResponseCode(),
                                                                error.getResponseMessage(), count));
}
```
从这段代码可以看出，站在全局统计的视角来看，这里把 JMeter 运行的统计结果，比如事务的 Total 请求、发送接收字节、平均值、最大值、最小值等，都加到 metric 中，同时也会把成功和失败的事务信息添加到 metric 中去。<br />在源码中，还有更多的添加 metric 的步骤，也可以看一下 JMeter 源码中的InfluxdbBackendListenerClient.java。<br />保存了 metric 之后，再使用 InfluxdbMetricsSender 发送到 Influxdb 中去。发送关键代码如下：
```java
@Override
    public void writeAndSendMetrics() {
 ........
        if (!copyMetrics.isEmpty()) {
            try {
                if(httpRequest == null) {
                    httpRequest = createRequest(url);
                }
                StringBuilder sb = new StringBuilder(copyMetrics.size()*35);
                for (MetricTuple metric : copyMetrics) {
                    // Add TimeStamp in nanosecond from epoch ( default in InfluxDB )
                    sb.append(metric.measurement)
                        .append(metric.tag)
                        .append(" ") //$NON-NLS-1$
                        .append(metric.field)
                        .append(" ")
                        .append(metric.timestamp+"000000") 
                        .append("\n"); //$NON-NLS-1$
                }


                StringEntity entity = new StringEntity(sb.toString(), StandardCharsets.UTF_8);

                httpRequest.setEntity(entity);
                lastRequest = httpClient.execute(httpRequest, new FutureCallback<HttpResponse>() {
                    @Override
                    public void completed(final HttpResponse response) {
                        int code = response.getStatusLine().getStatusCode();
                        /*
                         * HTTP response summary 2xx: If your write request received
                         * HTTP 204 No Content, it was a success! 4xx: InfluxDB
                         * could not understand the request. 5xx: The system is
                         * overloaded or significantly impaired.
                         */
                        if (MetricUtils.isSuccessCode(code)) {
                            if(log.isDebugEnabled()) {
                                log.debug("Success, number of metrics written: {}", copyMetrics.size());
                            } 
                        } else {
                            log.error("Error writing metrics to influxDB Url: {}, responseCode: {}, responseBody: {}", url, code, getBody(response));
                        }
                    }
                    @Override
                    public void failed(final Exception ex) {
                        log.error("failed to send data to influxDB server : {}", ex.getMessage());
                    }
                    @Override
                    public void cancelled() {
                        log.warn("Request to influxDB server was cancelled");
                    }
                });               
 ........
            }
        }
    }
```
通过 `writeAndSendMetrics`，就将所有保存的 metrics 都发给了 InfluxDB。
<a name="qIFz5"></a>
## InfluxDB 中的存储结构
然后再来看下 InfluxDB 中如何存储：
```sql
> show databases
name: databases
name
----
_internal
jmeter
> use jmeter
Using database jmeter
>
> show MEASUREMENTS
name: measurements
name
----
events
jmeter
> select * from events where application='7ddemo'
name: events
time application text title
---- ----------- ---- -----
1575255462806000000 7ddemo Test Cycle1 started ApacheJMeter
1575256463820000000 7ddemo Test Cycle1 ended ApacheJMeter
..............
n> select * from jmeter where application='7ddemo' limit 10
name: jmeter
time application avg count countError endedT hit max maxAT meanAT min minAT pct90.0 pct95.0 pct99.0 rb responseCode responseMessage sb startedT statut transaction
---- ----------- --- ----- ---------- ------ --- --- ----- ------ --- ----- ------- ------- ------- -- ------------ --------------- -- -------- ------ -----------
1575255462821000000 7ddemo 0 0 0 0 0 internal
1575255467818000000 7ddemo 232.82352941176472 17 0 17 849 122 384.9999999999996 849 849 0 0 all all
1575255467824000000 7ddemo 232.82352941176472 17 849 122 384.9999999999996 849 849 0 0 all 0_openIndexPage
1575255467826000000 7ddemo 232.82352941176472 17 849 122 384.9999999999996 849 849 ok 0_openIndexPage
1575255467829000000 7ddemo 0 1 1 1 1 internal
1575255472811000000 7ddemo 205.4418604651163 26 0 26 849 122 252.6 271.4 849 0 0 all all
1575255472812000000 7ddemo 0 1 1 1 1 internal
1575255472812000000 7ddemo 205.4418604651163 26 849 122 252.6 271.4 849 ok 0_openIndexPage
1575255472812000000 7ddemo 205.4418604651163 26 849 122 252.6 271.4 849 0 0 all 0_openIndexPage
1575255477811000000 7ddemo 198.2142857142857 27 0 27 849 117 263.79999999999995 292.3500000000001 849 0 0 all all
```
这段代码也就是说，在 InfluxDB 中，创建了两个 MEASUREMENTS，分别是 events 和 jmeter。这两个各自存了数据，在界面中配置的 testtile 和 eventTags 放在了 events 这个 MEASUREMENTS 中。在模板中这两个值暂时都是不用的。<br />在 jmeter 这个 MEASUREMENTS 中，可以看到 application 和事务的统计信息，这些值和控制台一致。在 Grafana 中显示的时候，就是从这个表中取出的数据，根据时序做的曲线。
<a name="gNb6E"></a>
## Grafana 中的配置
有了 JMeter 发送到 InfluxDB 中的数据，下面就来配置一下 Grafana 中的展示。首先，要配置一个 InfluxDB 数据源。如下所示：<br />![2021-08-28-08-17-53-852064.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630110400717-9ef7ab37-3e8e-40aa-9c65-1e45d016ffa5.png#clientId=ubd68d543-5179-4&from=ui&id=u44ba1f6d&originHeight=926&originWidth=875&originalType=binary&ratio=1&size=198389&status=done&style=none&taskId=ub2497217-d13f-4623-a753-c0f6da336d0)<br />在这里配置好 URL、Database、User、Password 之后，直接点击保存即可。<br />然后添加一个 JMeter dashboard，常用的 dashboard 是 Grafana 官方 ID 为 5496 的模板。导入进来后，选择好对应的数据源。<br />![2021-08-28-08-17-53-963070.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630110414073-0c4bd485-9412-451e-bcdc-c65a89379ae3.png#clientId=ubd68d543-5179-4&from=ui&id=u0ba6f330&originHeight=438&originWidth=880&originalType=binary&ratio=1&size=91279&status=done&style=none&taskId=u1b0b59fa-a3ef-480f-9bd1-a248f13f9fb)<br />然后就看到界面了。<br />![2021-08-28-08-17-54-384075.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630110414065-9e6924cd-b96b-4f4c-8cda-ac592594bc51.png#clientId=ubd68d543-5179-4&from=ui&id=PpCSF&originHeight=455&originWidth=1080&originalType=binary&ratio=1&size=80357&status=done&style=none&taskId=ue1b946e5-895a-4c5b-be7d-43d0b84ee44)<br />这时还没有数据，稍后做个示例，看下 JMeter 中的数据怎么和这个界面的数据对应起来。先看下图中两个重要的数据查询语句吧。
<a name="eU0Ur"></a>
### TPS 曲线：
```sql
SELECT last("count") / $send_interval FROM "$measurement_name" WHERE ("transaction" =~ /^$transaction$/ AND "statut" = 'ok') AND $timeFilter GROUP BY time($__interval)
```
上面这个就是 Total TPS 了，在这里称为 throughput。<br />关于这个概念，在第一篇中就已经有了说明，这里再次提醒，概念的使用在团队中要有统一的认识，不要受行业内一些传统信息的误导。<br />这里取的数据来自 MEASUREMENTS 中成功状态的所有事务。
<a name="x12k4"></a>
### 响应时间曲线：
```sql
SELECT mean("pct95.0") FROM "$measurement_name" WHERE ("application" =~ /^$application$/) AND $timeFilter GROUP BY "transaction", time($__interval) fill(null)
```
这里是用 95 pct 内的响应时间画出来的曲线。<br />整体展示出来的效果如下：<br />![2021-08-28-08-17-54-567065.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630110414071-9525a393-6a5a-418e-9dd4-8e21fcaffd79.png#clientId=ubd68d543-5179-4&from=ui&id=afUvv&originHeight=455&originWidth=1080&originalType=binary&ratio=1&size=133454&status=done&style=none&taskId=u7d95d6b6-40cf-4a0b-881c-4e9b777fe46)
<a name="Ldc9p"></a>
## 数据比对
首先，在 JMeter 中配置一个简单的场景。10 个线程，每个线程迭代 10 次，以及两个 HTTP 请求。<br />![2021-08-28-08-17-54-913068.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630110447498-d21b3abb-867e-413a-9e78-e0a157361a7f.png#clientId=ubd68d543-5179-4&from=ui&id=ua74efeef&originHeight=282&originWidth=719&originalType=binary&ratio=1&size=90135&status=done&style=none&taskId=u9d5c7912-763f-4bef-a099-50b9dd1cb4d)<br />也就是说，这时会产生 10x10x2=200 次请求。用 JMeter 跑起来看一下。<br />![2021-08-28-08-17-55-065070.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630110447472-8b1f931b-4af9-4423-87e3-95d49eb79e49.png#clientId=ubd68d543-5179-4&from=ui&id=KXkKS&originHeight=71&originWidth=1004&originalType=binary&ratio=1&size=53055&status=done&style=none&taskId=u8f382221-f658-49fa-871f-e95a589cbb2)<br />看到了吧，这个请求数和预想的一样。下面看一下 Grafana 中展示出来的结果。<br />![2021-08-28-08-17-55-219063.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630110447515-004cae09-59ae-4c2a-95f4-344aa80c90fc.png#clientId=ubd68d543-5179-4&from=ui&id=ZL3gQ&originHeight=487&originWidth=1080&originalType=binary&ratio=1&size=122983&status=done&style=none&taskId=u7226da5a-36a5-4a92-91fe-71cf2e2a998)<br />还有针对每个事务的统计情况。<br />![2021-08-28-08-17-55-418061.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630110471034-641cee77-0c9e-4f5c-9749-96f9286326ac.png#clientId=ubd68d543-5179-4&from=ui&id=ue7e1bca3&originHeight=455&originWidth=1080&originalType=binary&ratio=1&size=133514&status=done&style=none&taskId=ub817fc0d-047c-4760-85f1-c3c6036fd20)<br />至此，JMeter 到 Grafana 的展示过程就完成了。以后就不用再保存 JMeter 的执行结果了，也不用等着 JMeter 输出 HTML 了。
<a name="cwMop"></a>
## node_exporter+Prometheus+Grafana 的数据展示逻辑
对性能测试来说，在常用的 Grafana+Prometheus+Exporter 的逻辑中，第一步要看的就是操作系统资源了。所以在这一篇中，将以 node_exporter 为例来说明一下操作系统抽取数据的逻辑，以便知道监控数据的来源，至于数据的含义，在后续的文章中继续描述。<br />首先，还是要画一个图。<br />![2021-08-28-08-17-55-601065.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630110485786-930885e4-38ee-480b-8ac5-c65afab8b812.png#clientId=ubd68d543-5179-4&from=ui&id=u87274b30&originHeight=1246&originWidth=744&originalType=binary&ratio=1&size=77335&status=done&style=shadow&taskId=u8152928e-0c6b-45d1-a1e4-a7b702337cf)<br />现在 node_exporter 可以支持很多个操作系统了。官方列表如下：<br />![2021-08-28-08-17-55-978082.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630110485964-36a89506-cafd-4f0f-ba07-12ad5253e5bc.png#clientId=ubd68d543-5179-4&from=ui&id=VjUGu&originHeight=703&originWidth=1080&originalType=binary&ratio=1&size=479786&status=done&style=shadow&taskId=u72bcf96c-666f-45d3-a0ca-531f98eac69)<br />当然不是说只支持这些，也可以扩展自己的 Exporter。
<a name="K0cXJ"></a>
## 配置 node_exporter
node_exporter 目录如下：
```bash
[root@7dgroup2 node_exporter-0.18.1.linux-amd64]# ll
total 16524
-rw-r--r-- 1 3434 3434 11357 Jun 5 00:50 LICENSE
-rwxr-xr-x 1 3434 3434 16878582 Jun 5 00:41 node_exporter
-rw-r--r-- 1 3434 3434 463 Jun 5 00:50 NOTICE
```
启动：
```bash
[root@7dgroup2 node_exporter-0.18.1.linux-amd64]#./node_exporter --web.listen-address=:9200 &
```
是不是很简洁？如果想看更多的功能 ，可以查看下它的帮助。
<a name="Qen0j"></a>
## 配置 Prometheus
先下载 Prometheus：
```bash
[root@7dgroup2 data]# wget -c https://github.com/prometheus/prometheus/releases/download/v2.14.0/prometheus-2.14.0.linux-amd64.tar.gz
..........
100%[=============================================================================================>] 58,625,125 465KB/s in 6m 4s
2019-11-29 15:40:16 (157 KB/s) - ‘prometheus-2.14.0.linux-amd64.tar.gz’ saved [58625125/58625125]
[root@7dgroup2 data]
```
解压之后，可以看到目录结构如下：
```bash
[root@7dgroup2 prometheus-2.11.1.linux-amd64]# ll
total 120288
drwxr-xr-x. 2 3434 3434 4096 Jul 10 23:26 console_libraries
drwxr-xr-x. 2 3434 3434 4096 Jul 10 23:26 consoles
drwxr-xr-x. 3 root root 4096 Nov 30 12:55 data
-rw-r--r--. 1 3434 3434 11357 Jul 10 23:26 LICENSE
-rw-r--r--. 1 root root 35 Aug 7 23:19 node.yml
-rw-r--r--. 1 3434 3434 2770 Jul 10 23:26 NOTICE
-rwxr-xr-x. 1 3434 3434 76328852 Jul 10 21:53 prometheus
-rw-r--r-- 1 3434 3434 1864 Sep 21 09:36 prometheus.yml
-rwxr-xr-x. 1 3434 3434 46672881 Jul 10 21:54 promtool
[root@7dgroup2 prometheus-2.11.1.linux-amd64]#
```
![2021-08-28-08-17-56-161062.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630110485812-67adb78e-26a1-4d0e-b822-3e2a367f5da0.png#clientId=ubd68d543-5179-4&from=ui&id=XKDH7&originHeight=680&originWidth=492&originalType=binary&ratio=1&size=98853&status=done&style=none&taskId=u3830ebc6-ebb2-497b-89aa-a149d91c023)<br />再配置一个 node_exporter 的模板，比如这里选择了官方模板（ID：11074），展示如下：<br />![2021-08-28-08-17-56-668062.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630110530539-c17a5617-312b-4e93-bf23-fdee2371444e.png#clientId=ubd68d543-5179-4&from=ui&id=u6bf68d26&originHeight=576&originWidth=1080&originalType=binary&ratio=1&size=398948&status=done&style=none&taskId=ubad1a961-f44d-4e15-8245-66dec51d06c)
<a name="LePvC"></a>
## 数据逻辑说明
说明完上面的过程之后，对做性能测试和分析的人来说，最重要的，就是要知道数据的来源和含义了。<br />拿上面图中的 CPU 使用率来说吧（因为 CPU 使用率是非常重要的一个计数器）。<br />先点一下 title 上的 edit，看一下它的 query 语句。
```sql
avg(irate(node_cpu_seconds_total{instance=~"$node",mode="system"}[30m])) by (instance)
avg(irate(node_cpu_seconds_total{instance=~"$node",mode="user"}[30m])) by (instance)
avg(irate(node_cpu_seconds_total{instance=~"$node",mode="iowait"}[30m])) by (instance)
1 - avg(irate(node_cpu_seconds_total{instance=~"$node",mode="idle"}[30m])) by (instance)
```
这些都是从 Prometheus 中取出来的数据，查询语句读了 Prometheus 中`node_cpu_seconds_total`的不同的模块数据。<br />下面来看一下，`node_exporter`暴露出来的计数器。<br />![2021-08-28-08-17-57-151066.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630110530548-47c98615-cc70-45b9-b337-889ce777bc4a.png#clientId=ubd68d543-5179-4&from=ui&id=E3BFN&originHeight=412&originWidth=682&originalType=binary&ratio=1&size=422972&status=done&style=shadow&taskId=ud55750b8-877a-4a75-8f52-599a5669347)<br />这些值和 top 一样，都来自于/proc/目录。下面这张图是 top 数据，可以比对一下。<br />![2021-08-28-08-17-57-451062.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630110530386-c1bbd464-4168-4dcc-bbc0-98474ebd1e50.png#clientId=ubd68d543-5179-4&from=ui&id=w35M0&originHeight=393&originWidth=673&originalType=binary&ratio=1&size=298697&status=done&style=shadow&taskId=ub196d171-8b2b-4dc5-8af0-0f11bd70a17)<br />到此，就了解到了操作系统中监控数据的取值逻辑了，也就是从操作系统本身的计数器中取出值来，然后传给 Prometheus，再由 Grafana 中的 query 语句查出相应的数据，最后由 Grafana 展示在界面上。
<a name="tsZBk"></a>
## 总结
为什么要解释数据的逻辑呢？因为最近在工作中遇到一些情况，有人觉得有了 Prometheus+Grafana+Exportor 这样的组合工具之后，基本上都不再用手工执行什么命令了。但要了解的是，对于监控平台来说，它取的所有的数据必然是被监控者可以提供的数据，像 node_exporter 这样小巧的监控收集器，它可以获取的监控数据，并不是整个系统全部的性能数据，只是取到了常见的计数器而已。这些计数器不管是用命令查看，还是用这样炫酷的工具查看，它的值本身都不会变。所以不管是在监控平台上看到的数据，还是在命令行中看到的数据，最重要的是要知道含义以及这些值的变化对性能测试和分析的下一步骤的影响。
