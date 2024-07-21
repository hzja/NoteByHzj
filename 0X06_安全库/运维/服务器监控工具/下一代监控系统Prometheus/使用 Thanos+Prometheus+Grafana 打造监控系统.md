![集群容量概览](https://cdn.nlark.com/yuque/0/2021/png/396745/1624008919295-6547d9c1-01f1-40e5-bf38-b109a55e5e9a.png#clientId=ucb161360-38e8-4&from=paste&id=u44626759&originHeight=565&originWidth=1080&originalType=url&ratio=3&rotation=0&showTitle=true&status=done&style=none&taskId=u487de634-5802-491a-8410-8457fddecbe&title=%E9%9B%86%E7%BE%A4%E5%AE%B9%E9%87%8F%E6%A6%82%E8%A7%88 "集群容量概览")
<a name="vOeey"></a>
## 架构
集群上没有可用的持久化存储（所有服务都保持无状态），所以默认的 Prometheus + Thanos sidecar 方法不可用，metric 存储必须置于集群之外。此外，集群之间相互隔离，将 Thanos 组件绑定到一组特定的集群是不可能的，必须从“外部”监控集群。<br />综上所述，考虑到高可用性以及 Thanos 在虚拟机上运行的可能性，最终的架构是这样的：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624008919161-1c7b302c-37e9-4814-bb24-232ecf5e84ba.png#clientId=ucb161360-38e8-4&from=paste&id=ud6a8f134&originHeight=1350&originWidth=1080&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=u80328f6e-f1b4-431f-baf4-eb5a69e2084&title=)<br />如图所示，是多数据中心的架构。其中每个中心都有一组 Grafana + Query 服务器，一组存储服务器和三个 Receive 服务器（集群数量的一半）。<br />Grafana 使用的数据库还有一个 AWS RDS。这个数据库不必很庞大（降低成本），也不需要管理 MySQL。<br />在 Thanos 提供的所有组件中，实现了其中的 4 个：

- Receive：负责 TSDB，还管理所有运行 receive 的服务器和 TSBD 块上传到 S3 之间的复制。
- Query：负责查询 receive 数据库。
- Store：读取 S3 以获取不再存储在 receive 中的长期 metrics。
- Compactor：管理存储在 S3 中的 TSDB 块的数据下采样和压缩。
<a name="lMsAM"></a>
## Data Ingestion
所有集群的 data ingestion 都由集群内运行的专用 Prometheus Pod 管理。它从 control plate（API 服务器、控制器和调度程序）、etcd 集群以及集群内的 Pod 收集指标，这些集群内具有与基础设施和 Kubernetes 本身相关的指标（Kube-proxy、Kubelet、Node Exporter、State Metrics 、Metrics Server 和其他具有 scraping annotation 的 Pod）。<br />Prometheus Pod 然后将信息发送到使用远程存储配置管理 TSDB 的 receive 服务器之一。<br />![data ingestion](https://cdn.nlark.com/yuque/0/2021/png/396745/1624008919183-2a1d41b1-a21d-465a-ab26-3e0695830ee3.png#clientId=ucb161360-38e8-4&from=paste&id=u33047ffe&originHeight=294&originWidth=621&originalType=url&ratio=3&rotation=0&showTitle=true&status=done&style=shadow&taskId=u6dffba3e-8ddb-4a2b-8afe-619adee49b9&title=data%20ingestion "data ingestion")<br />所有数据都发送到单个服务器，然后复制到其他服务器。Prometheus 使用的 DNS 地址是一个 DNS GSLB，它探测每个 receive 服务器并平衡健康的服务器之间的 DNS 解析，在所有服务器之间分担负载，因为 DNS 解析只为每个 DNS 查询提供一个 IP。<br />需要强调一下，数据必须发送到单个 receive 实例并让它管理复制，发送相同的 metric 会导致复制失败和行为异常。<br />在这个层面上，metrics 也会上传到 S3 存储桶进行长期留存。Receive 每 2 小时（当每个 TSDB 块关闭时）上传一次 block，这些 metric 可用于使用 Store 组件进行查询。<br />还可以设置本地数据的保留时间。在这种情况下，所有本地数据都会保留 30 天以供日常使用和故障排除，这样可以加快查询速度。<br />超过 30 天的数据仅在 S3 上可用，最长可保留 1 年，用于长期评估和比较。
<a name="JApQJ"></a>
## 数据查询
数据被收集并存储在 receiver 中以供查询。这部分也设置为多数据中心可用。<br />每台服务器都运行 Grafana 和 Query，如果其中一台（或两台）出现故障，可以更轻松地从负载均衡器中识别并删除。在 Grafana 中，数据源配置为 localhost，因此它始终使用本地 Query 来获取数据。<br />对于查询配置，它必须知道所有存储了 metrics 的服务器（Receiver 和 Store）。query 组件知道哪个服务器在线并且能够从它们收集 metrics。<br />![数据查询](https://cdn.nlark.com/yuque/0/2021/png/396745/1624008919080-b511e846-fbcd-453d-8108-8af01d289413.png#clientId=ucb161360-38e8-4&from=paste&id=u33674a82&originHeight=364&originWidth=527&originalType=url&ratio=3&rotation=0&showTitle=true&status=done&style=shadow&taskId=u3156e52b-e1e0-40f8-a9b7-accc36c9bd8&title=%E6%95%B0%E6%8D%AE%E6%9F%A5%E8%AF%A2 "数据查询")<br />它还管理重复数据删除，因为它查询所有服务器并配置了 replication，所有 metrics 都有多个副本。可以使用分配给 metrics 的标签和查询参数 (`--query.replica-label=QUERY.REPLICA-LABEL`) 来完成。通过这些配置，query 组件知道从 Receiver 和 Store 收集的 metrics 是否重复并仅使用一个数据点。
<a name="hzBLD"></a>
## 长期数据
如前所述，数据在本地最多保留 30 天，其他所有内容都存储在 S3 上。这样可以减少 Receiver 上所需的空间量并降低成本，因为块存储比对象存储更贵。更何况查询超过 30 天的数据不是很常见，主要用于资源使用历史和预测。<br />![远程数据查询](https://cdn.nlark.com/yuque/0/2021/png/396745/1624008919145-aba2f661-f3fb-4a1b-80c1-f48d904a343c.png#clientId=ucb161360-38e8-4&from=paste&id=u268e6c8f&originHeight=432&originWidth=316&originalType=url&ratio=3&rotation=0&showTitle=true&status=done&style=shadow&taskId=u2c6c5f82-914f-4a30-86a2-1d9083a5728&title=%E8%BF%9C%E7%A8%8B%E6%95%B0%E6%8D%AE%E6%9F%A5%E8%AF%A2 "远程数据查询")<br />该 Store 还保留存储在 S3 存储桶上的每个 TSDB 块的索引的本地副本，因此如果需要查询超过 30 天的数据，它知道要下载和使用哪些块来提供数据。
<a name="Sby44"></a>
## 数据情况
考虑到所有集群，该监控方案：

- 监控了 6 个 Kubernetes 集群；
- 收集了 670 个服务的 metrics；
- 使用 Node Exporter 监控了 246 个服务器；
- 每分钟收集约 27w 个指标；
- 每天 ingest 约 7.3 GB 的数据，或每月 ingest 约 226.3 GB 的数据；
- 为 Kubernetes 组件创建了 40 个专用仪表盘；
- 在 Grafana 上创建了 116 个警报。

对于每月费用，由于大部分组件在本地运行，成本降低了 90.61%，从每月 38,421.25 美元降至 3,608.99 美元，其中包括 AWS 服务成本。
<a name="BKPJ7"></a>
## 总结
配置和设置上述架构大约需要一个月左右的时间，包括测试其他一些解决方案、验证架构、实现、在集群上开启收集以及创建所有仪表盘。<br />在第一周，好处是显而易见的。监控集群变得更加容易，仪表盘可以快速构建和定制，收集 metrics 几乎是即插即用的，大多数应用程序以 Prometheus 格式导出 metrics，并根据 annotations 自动收集。<br />此外，通过集成 Grafana 的 LDAP 可以达到更精细的团队权限控制。开发人员和 SRE 可以访问大量仪表盘，其中包含有关其命名空间、ingress 等的相关 metrics。
