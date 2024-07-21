Kubernetes
<a name="tAaNj"></a>
### 一、节点配额和内核参数调整
对于公有云上的 Kubernetes 集群，规模大了之后很容器碰到配额问题，需要提前在云平台上增大配额。这些需要增大的配额包括：

- 虚拟机个数
- vCPU 个数
- 内网 IP 地址个数
- 公网 IP 地址个数
- 安全组条数
- 路由表条数
- 持久化存储大小

参考gce随着node节点的增加master节点的配置：

- 1-5 nodes: n1-standard-1
- 6-10 nodes: n1-standard-2
- 11-100 nodes: n1-standard-4
- 101-250 nodes: n1-standard-8
- 251-500 nodes: n1-standard-16
- more than 500 nodes: n1-standard-32

参考阿里云配置：
```
# max-file 表示系统级别的能够打开的文件句柄的数量， 一般如果遇到文件句柄达到上限时，会碰到"Too many open files"或者Socket/File: Can’t open so many files等错误。
fs.file-max=1000000

# 配置arp cache 大小
net.ipv4.neigh.default.gc_thresh1=1024
# 存在于ARP高速缓存中的最少层数，如果少于这个数，垃圾收集器将不会运行。缺省值是128。

# 保存在 ARP 高速缓存中的最多的记录软限制。垃圾收集器在开始收集前，允许记录数超过这个数字 5 秒。缺省值是 512。
net.ipv4.neigh.default.gc_thresh2=4096

# 保存在 ARP 高速缓存中的最多记录的硬限制，一旦高速缓存中的数目高于此，垃圾收集器将马上运行。缺省值是1024。
net.ipv4.neigh.default.gc_thresh3=8192

# 以上三个参数，当内核维护的arp表过于庞大时候，可以考虑优化

# 允许的最大跟踪连接条目，是在内核内存中netfilter可以同时处理的“任务”（连接跟踪条目）
net.netfilter.nf_conntrack_max=10485760

# 哈希表大小（只读）（64位系统、8G内存默认 65536，16G翻倍，如此类推）
net.core.netdev_max_backlog=10000
# 每个网络接口接收数据包的速率比内核处理这些包的速率快时，允许送到队列的数据包的最大数目。
net.netfilter.nf_conntrack_tcp_timeout_established=300
net.netfilter.nf_conntrack_buckets=655360

# 关于conntrack的详细说明：https://testerhome.com/topics/7509

# 默认值: 128 指定了每一个real user ID可创建的inotify instatnces的数量上限
fs.inotify.max_user_instances=524288

# 默认值: 8192 指定了每个inotify instance相关联的watches的上限
fs.inotify.max_user_watches=524288
```
<a name="pHPY3"></a>
### 二、Etcd 数据库
1、搭建高可用的etcd集群，集群规模增大时可以自动增加etcd节点；<br />目前的解决方案是使用etcd operator来搭建etcd 集群，operator是CoreOS推出的旨在简化复杂有状态应用管理的框架，它是一个感知应用状态的控制器，通过扩展Kubernetes API来自动创建、管理和配置应用实例。<br />etcd operator 有如下特性：

- ceate/destroy: 自动部署和删除 etcd 集群，不需要人额外干预配置。
- resize：可以动态实现 etcd 集群的扩缩容。
- backup：支持etcd集群的数据备份和集群恢复重建
- upgrade：可以实现在升级etcd集群时不中断服务。

2、配置etcd使用ssd固态盘存储；<br />3、设置 —quota-backend-bytes 增大etcd的存储限制。默认值是 2G；<br />4、需要配置单独的 Etcd 集群存储 kube-apiserver 的 event。
<a name="r4CUN"></a>
### 四、Kube APIServer 配置
node节点数量 >= 3000， 推荐设置如下配置：
```bash
--max-requests-inflight=3000
--max-mutating-requests-inflight=1000
```
node节点数量在 1000 — 3000， 推荐设置如下配置：
```bash
--max-requests-inflight=1500
--max-mutating-requests-inflight=500
```
内存配置选项和node数量的关系，单位是MB：
```bash
--target-ram-mb=node_nums * 60
```
<a name="UhAxV"></a>
### 五、Pod 配置
在运行 Pod 的时候也需要注意遵循一些最佳实践，比如:<br />1、为容器设置资源请求和限制，尤其是一些基础插件服务
```
spec.containers[].resources.limits.cpu
spec.containers[].resources.limits.memory
spec.containers[].resources.requests.cpu
spec.containers[].resources.requests.memory
spec.containers[].resources.limits.ephemeral-storage
spec.containers[].resources.requests.ephemeral-storage
```
在k8s中，会根据pod不同的limit 和 requests的配置将pod划分为不同的qos类别：

- Guaranteed
- Burstable
- BestEffort

当机器可用资源不够时，kubelet会根据qos级别划分迁移驱逐pod。被驱逐的优先级：BestEffort > Burstable > Guaranteed<br />2、对关键应用使用 nodeAffinity、podAffinity 和 podAntiAffinity 等保护，使其调度分散到不同的node上。比如kube-dns 配置：
```yaml
affinity:
 podAntiAffinity:
   requiredDuringSchedulingIgnoredDuringExecution:
   - weight: 100
     labelSelector:
       matchExpressions:
       - key: k8s-app
         operator: In
         values:
         - kube-dns
     topologyKey: kubernetes.io/hostname
```
3、尽量使用控制器来管理容器（如 Deployment、StatefulSet、DaemonSet、Job 等）<br />Kube-scheduler 配置<br />设置 `--kube-api-qps=100` 默认值是 50<br />Kube-controller-manager 配置<br />设置 `--kube-api-qps=100` 默认值是20<br />设置 `--kube-api-burst=100` 默认值是30
