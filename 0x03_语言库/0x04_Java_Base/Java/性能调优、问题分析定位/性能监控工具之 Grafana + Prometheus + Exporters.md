性能监控 Grafana Prometheus Exporters<br />在性能监控图谱中，有操作系统、应用服务器、中间件、队列、缓存、数据库、网络、前端、负载均衡、Web 服务器、存储、代码等很多需要监控的点。显然这些监控点不能在一个专栏中全部覆盖并一一细化，只能找最常用的几个，做些逻辑思路的说明，同时也把具体的实现描述出来。如果遇到了其他的组件，也需要一一实现这些监控。<br />主要说明下图的这个监控逻辑。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623658488625-d03ae586-4a4b-46ed-ad8c-77986bcbfee1.webp#clientId=u96d7966a-334f-4&from=paste&id=uaef24fab&originHeight=583&originWidth=1080&originalType=url&ratio=3&status=done&style=shadow&taskId=u25912124-2158-4ad4-91c6-695bb0c0278)<br />这应该是现在最流行的一套监控逻辑了。把常见的使用 Grafana、Prometheus、InfluxDB、Exporters 的数据展示方式说一下，如果刚进入性能测试领域，也能有一个感性的认识。<br />有测试工具，有监控工具，才能做后续的性能分析和瓶颈定位，所以有必要把这些工具的逻辑摆一摆。<br />所有做性能的人都应该知道一点，不管数据以什么样的形式展示，最要紧的还是看数据的来源和含义，以便做出正确的判断。<br />先说明一下 JMeter 和 node_exporter 到 Grafana 的数据展示逻辑。至于其他的 Exporter，就不再解释这个逻辑了，只说监控分析的部分。
<a name="yKPq2"></a>
## JMeter+InfluxDB+Grafana 的数据展示逻辑
一般情况下，用 JMeter 做压力测试时，都是使用 JMeter 的控制台来查看结果。如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623658488749-c31d7f19-e4c8-4506-bd7e-77601fba280f.webp#clientId=u96d7966a-334f-4&from=paste&id=u862c32f8&originHeight=361&originWidth=1080&originalType=url&ratio=3&status=done&style=none&taskId=u622ace64-8556-4434-81cb-3dd38d61153)<br />或者装个插件来看结果：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623658488994-6d9e871e-7527-48e2-b207-7a00a790cb01.png#clientId=u96d7966a-334f-4&from=paste&id=ufcfca845&originHeight=460&originWidth=1080&originalType=url&ratio=3&status=done&style=shadow&taskId=u7d4e4eaf-c5bd-4668-a902-def028b3e94)<br />或者用 JMeter 来生成 HTML：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623658488956-f063490a-ccac-453f-96a6-92ce36d7f686.png#clientId=u96d7966a-334f-4&from=paste&id=u7dcd2ada&originHeight=555&originWidth=1001&originalType=url&ratio=3&status=done&style=shadow&taskId=u725b6222-c967-4fb6-a0f3-46bf1fe13c6)<br />这样看都没有问题，在前面也强调过，对于压力工具来说，最多只关心三条曲线的数据：TPS（T 由测试目标定义）、响应时间、错误率。这里的错误率还只是辅助排查问题的曲线，没有问题时，只看 TPS 和响应时间即可。<br />不过采取以上三种方式有几个方面的问题。<br />整理结果时比较浪费时间。<br />在 GUI 用插件看曲线，做高并发时并不现实。<br />在场景运行时间比较长的时候，采用生成 HTML 的方式，会出现消耗内存过大的情况，而实际上，在生成的结果图中，有很多生成的图并不是那么关注。<br />生成的结果保存之后再查看比较麻烦，还要一个个去找。<br />那么如何解决这几个问题呢？<br />用 JMeter 的 Backend Listener 实时发送数据到 InfluxDB 或 Graphite 可以解决这样的问题。<br />Graphite Backend Listener 的支持是在 JMeter 2.13 版本，InfluxdDB Backend Listener 的支持是在 JMeter 3.3 的版本，它们都是用异步的方式把数据发送出来，以便查看。<br />其实有这个 JMeter 发送给 InfluxDB 的数据之后，不需要看上面的那些 HTML 数据，也可以直观地看到系统性能的性能趋势。<br />并且这样保存下来的数据，在测试结束后想再次查看也比较方便比对。<br />JMeter+InfluxDB+Grafana 的结构如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623658488700-6fa7cfe2-01f1-4127-9ce3-7b9a00997cc0.webp#clientId=u96d7966a-334f-4&from=paste&id=ucf7d9738&originHeight=501&originWidth=1080&originalType=url&ratio=3&status=done&style=shadow&taskId=uf4145bc5-63c0-4bea-95a1-0f131aba296)<br />在这个结构中，JMeter 发送压力到服务器的同时，统计下 TPS、响应时间、线程数、错误率等信息。默认每 30 秒在控制台输出一次结果（在 jmeter.properties 中有一个参数 #summariser.interval=30 可以控制）。<br />配置了 Backend Listener 之后，将统计出的结果异步发送到 InfluxDB 中。最后在 Grafana 中配置 InfluxDB 数据源和 JMeter 显示模板。<br />然后就可以实时查看 JMeter 的测试结果了，这里看到的数据和控制台的数据是一样。<br />但如果这么简单就说完了，这篇文章也就没价值了。下面来说一下，数据的传输和展示逻辑。
<a name="wIuY6"></a>
## JMeter 中 Backend Listener 的配置
下面就 InfluxDB 的 Backend Listener 做个说明。它的配置比较简单，在脚本中加上即可。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623658489724-54781047-f4ed-4c12-b612-80c1e19a590b.png#clientId=u96d7966a-334f-4&from=paste&id=ucfe4b807&originHeight=274&originWidth=1080&originalType=url&ratio=3&status=done&style=none&taskId=u37507677-2735-4a05-b991-2ce8a775324)<br />先配置好 influxdb Url、application 等信息，application 这个配置可以看成是场景名。<br />那么 JMeter 如何将数据发给 InfluxDB 呢？请看源码中的关键代码，如下所示：
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
从这段代码可以看出，站在全局统计的视角来看，这里把 JMeter 运行的统计结果，比如事务的 Total 请求、发送接收字节、平均值、最大值、最小值等，都加到 metric 中，同时也会把成功和失败的事务信息添加到 metric 中去。<br />在源码中，还有更多的添加 metric 的步骤，有兴趣的话，也可以看一下 JMeter 源码中的InfluxdbBackendListenerClient.java。<br />保存了 metric 之后，再使用 InfluxdbMetricsSender 发送到 Influxdb 中去。发送关键代码如下：
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
通过 writeAndSendMetrics，就将所有保存的 metrics 都发给了 InfluxDB。
<a name="nDfsU"></a>
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
<a name="LnpQo"></a>
## Grafana 中的配置
有了 JMeter 发送到 InfluxDB 中的数据，下面就来配置一下 Grafana 中的展示。首先，要配置一个 InfluxDB 数据源。如下所示：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623658489881-661925a3-2678-4ded-9414-50b2afd65b9a.png#clientId=u96d7966a-334f-4&from=paste&id=u6029059f&originHeight=926&originWidth=875&originalType=url&ratio=3&status=done&style=none&taskId=u4f3eebed-aa1e-4209-8043-1d6f5b65ee1)<br />在这里配置好 URL、Database、User、Password 之后，直接点击保存即可。<br />然后添加一个 JMeter dashboard，常用的 dashboard 是 Grafana 官方 ID 为 5496 的模板。导入进来后，选择好对应的数据源。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623658489877-0277f0e1-ce47-4f4e-a6aa-9b3f426d203d.webp#clientId=u96d7966a-334f-4&from=paste&id=ub3d73bd4&originHeight=438&originWidth=880&originalType=url&ratio=3&status=done&style=none&taskId=u47421f0a-9e19-47d4-97e3-fc2a8a65aa5)<br />然后就看到界面了。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623658490175-8a44d751-363c-45b0-a845-d3c65ce4718d.webp#clientId=u96d7966a-334f-4&from=paste&id=u7f1b054b&originHeight=455&originWidth=1080&originalType=url&ratio=3&status=done&style=none&taskId=u3f0f5dd9-cb60-4b8f-90cc-4d7a49bbc9d)<br />这时还没有数据，稍后做个示例，看下 JMeter 中的数据怎么和这个界面的数据对应起来。先看下图中两个重要的数据查询语句吧。<br />TPS 曲线：
```sql
SELECT last("count") / $send_interval FROM "$measurement_name" WHERE ("transaction" =~ /^$transaction$/ AND "statut" = 'ok') AND $timeFilter GROUP BY time($__interval)
```
上面这个就是 Total TPS 了，在这里称为 throughput。<br />概念的使用在团队中要有统一的认识，不要受行业内一些传统信息的误导。<br />这里取的数据来自 MEASUREMENTS 中成功状态的所有事务。<br />响应时间曲线：
```sql
SELECT mean("pct95.0") FROM "$measurement_name" WHERE ("application" =~ /^$application$/) AND $timeFilter GROUP BY "transaction", time($__interval) fill(null)
```
这里是用 95 pct 内的响应时间画出来的曲线。<br />整体展示出来的效果如下：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623658490531-469b1af8-c55e-4391-867d-fd3cc7e0b913.png#clientId=u96d7966a-334f-4&from=paste&id=u47939c88&originHeight=455&originWidth=1080&originalType=url&ratio=3&status=done&style=none&taskId=uca020568-05eb-4269-a5ce-cdd08804fa4)
<a name="q0F5f"></a>
## 数据比对
首先，在 JMeter 中配置一个简单的场景。10 个线程，每个线程迭代 10 次，以及两个 HTTP 请求。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623658490859-8cfcc403-c82d-478b-bbe4-5895332227b1.webp#clientId=u96d7966a-334f-4&from=paste&id=u9748eb92&originHeight=282&originWidth=719&originalType=url&ratio=3&status=done&style=none&taskId=u922759b9-7acd-41be-81a3-98958748fff)<br />也就是说，这时会产生 10x10x2=200 次请求。用 JMeter 跑起来看一下。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623658490828-a6444fb3-59ed-4d1c-8553-73baf7180c18.webp#clientId=u96d7966a-334f-4&from=paste&id=uf96ea288&originHeight=71&originWidth=1004&originalType=url&ratio=3&status=done&style=none&taskId=u11440a6e-02fe-4969-a6a5-c0f7c150e6f)<br />看到了吧，这个请求数和预想的一样。下面看一下 Grafana 中展示出来的结果。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623658490796-403577cb-0712-42ed-9745-ca08a1076e45.webp#clientId=u96d7966a-334f-4&from=paste&id=udcd167d8&originHeight=487&originWidth=1080&originalType=url&ratio=3&status=done&style=none&taskId=u95c7e7f9-c8c7-47be-95db-0826cf7df16)<br />还有针对每个事务的统计情况。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623658491138-a8b21211-f34a-4ab0-b9c6-31cd94414055.webp#clientId=u96d7966a-334f-4&from=paste&id=ue06e5e52&originHeight=455&originWidth=1080&originalType=url&ratio=3&status=done&style=none&taskId=u3e17beb8-a5a1-48c5-b3d9-82f25b3d255)<br />至此，JMeter 到 Grafana 的展示过程就完成了。以后就不用再保存 JMeter 的执行结果了，也不用等着 JMeter 输出 HTML 了。
<a name="s6rfz"></a>
## node_exporter+Prometheus+Grafana 的数据展示逻辑
对性能测试来说，在常用的 Grafana+Prometheus+Exporter 的逻辑中，第一步要看的就是操作系统资源了。以 node_exporter 为例来说明一下操作系统抽取数据的逻辑，以便知道监控数据的来源。<br />首先，还是要画一个图。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623658491387-f79d5674-f91f-4c52-a9cd-ed08fe54d2f4.webp#clientId=u96d7966a-334f-4&from=paste&id=u427a67c8&originHeight=1246&originWidth=744&originalType=url&ratio=3&status=done&style=shadow&taskId=u663939f5-168a-4f84-8205-87e4415a366)<br />现在 node_exporter 可以支持很多个操作系统了。官方列表如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623658491357-cc12fc02-9e96-416a-8127-a196fc720cef.webp#clientId=u96d7966a-334f-4&from=paste&id=u747ff530&originHeight=703&originWidth=1080&originalType=url&ratio=3&status=done&style=shadow&taskId=uf585102d-98cd-451e-9fc1-e21f4008680)<br />当然不是说只支持这些，也可以扩展自己的 Exporter。<br />配置 node_exporter<br />node_exporter 目录如下：
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
<a name="nZMNV"></a>
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
在prome<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623658491587-3f4106c1-3551-4965-a6b5-2b1ea52240c4.webp#clientId=u96d7966a-334f-4&from=paste&id=ud2763331&originHeight=680&originWidth=492&originalType=url&ratio=3&status=done&style=none&taskId=u0159f20a-caa7-47ba-9195-b2c4b8f7c23)<br />再配置一个 node_exporter 的模板，比如这里选择了官方模板（ID：11074），展示如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623658491614-892e4676-60ca-4929-b31e-cbe105dbba47.webp#clientId=u96d7966a-334f-4&from=paste&id=u2da1d6e1&originHeight=576&originWidth=1080&originalType=url&ratio=3&status=done&style=none&taskId=u3f788855-1a6a-4242-8141-f1274c57da3)<br />数据逻辑说明<br />说明完上面的过程之后，做性能测试和分析的人来说，最重要的，就是要知道数据的来源和含义了。<br />拿上面图中的 CPU 使用率来说吧（因为 CPU 使用率是非常重要的一个计数器，所以先拿它来开刀）。<br />先点一下 title 上的 edit，看一下它的 query 语句。
```bash
avg(irate(node_cpu_seconds_total{instance=~"$node",mode="system"}[30m])) by (instance)
avg(irate(node_cpu_seconds_total{instance=~"$node",mode="user"}[30m])) by (instance)
avg(irate(node_cpu_seconds_total{instance=~"$node",mode="iowait"}[30m])) by (instance)
1 - avg(irate(node_cpu_seconds_total{instance=~"$node",mode="idle"}[30m])) by (instance)
```
这些都是从 Prometheus 中取出来的数据，查询语句读了 Prometheus 中node_cpu_seconds_total的不同的模块数据。<br />下面来看一下，node_exporter暴露出来的计数器。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623658491872-b2273eee-dde1-41bf-94b2-3bdd658883ba.webp#clientId=u96d7966a-334f-4&from=paste&id=u0b9c51fc&originHeight=412&originWidth=682&originalType=url&ratio=3&status=done&style=shadow&taskId=ub2a60943-9d50-421b-9a69-dfea6f5f57c)<br />这些值和 top 一样，都来自于/proc/目录。下面这张图是 top 数据，可以比对一下。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623658492396-e59263f4-6f44-4ecf-a6c8-127842263d54.webp#clientId=u96d7966a-334f-4&from=paste&id=uacd3f935&originHeight=393&originWidth=673&originalType=url&ratio=3&status=done&style=shadow&taskId=uf431e14c-86ac-4933-a4b4-e31c94a3a5d)<br />到此，就了解到了操作系统中监控数据的取值逻辑了，也就是从操作系统本身的计数器中取出值来，然后传给 Prometheus，再由 Grafana 中的 query 语句查出相应的数据，最后由 Grafana 展示在界面上。
<a name="imEKD"></a>
## 总结
为什么要解释数据的逻辑呢？因为有人觉得有了 Prometheus+Grafana+Exportor 这样的组合工具之后，基本上都不再用手工执行什么命令了。但要了解的是，对于监控平台来说，它取的所有的数据必然是被监控者可以提供的数据，像 node_exporter 这样小巧的监控收集器，它可以获取的监控数据，并不是整个系统全部的性能数据，只是取到了常见的计数器而已。这些计数器不管是用命令查看，还是用这样炫酷的工具查看，它的值本身都不会变。所以不管是在监控平台上看到的数据，还是在命令行中看到的数据，最重要的是要知道含义以及这些值的变化对性能测试和分析的下一步骤的影响。
