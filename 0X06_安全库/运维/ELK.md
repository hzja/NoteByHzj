<a name="AlWmS"></a>
# 1.为什么用到
      一般我们需要进行日志分析场景：直接在日志文件中grep、awk 就可以获得自己想要的信息。但在规模较大的场景中，此方法效率低下，面临问题包括日志量太大如何归档、文本搜索太慢怎么办、如何多维度查询。需要集中化的日志管理，所有服务器上的日志收集汇总。<br />  <br />      常见解决思路是建立集中式日志收集系统，将所有节点上的日志统一收集，管理，访问。一般大型系统是一个分布式部署的架构，不同的服务模块部署在不同的服务器上，问题出现时，大部分情况需要根据问题暴露的关键信息，定位到具体的服务器和服务模块，构建一套集中式日志系统，可以提高定位问题的效率。

      一个完整的集中式日志系统，需要包含以下几个主要特点：收集－能够采集多种来源的日志数据传输－能够稳定的把日志数据传输到中央系统存储－如何存储日志数据分析－可以支持UI 分析警告－能够提供错误报告，监控机制ELK提供了一整套解决方案，并且都是开源软件，之间互相配合使用，完美衔接，高效的满足了很多场合的应用。目前主流的一种日志系统。

<a name="kbgfY"></a>
# 2.ELK简介
    是三个开源软件的缩写，分别表示：Elasticsearch , Logstash,Kibana , 它们都是开源软件。新增了一个FileBeat，它是一个轻量级的日志收集处理工具(Agent)，Filebeat占用资源少，适合于在各个服务器上搜集日志后传输给Logstash，官方也推荐此工具。

    Elasticsearch是个开源分布式搜索引擎，提供搜集、分析、存储数据三大功能。它的特点有：分布式，零配置，自动发现，索引自动分片，索引副本机制，restful风格接口，多数据源，自动搜索负载等。Logstash 主要是用来日志的搜集、分析、过滤日志的工具，支持大量的数据获取方式。一般工作方式为c/s架构，client端安装在需要收集日志的主机上，server端负责将收到的各节点日志进行过滤、修改等操作在一并发往elasticsearch上去。Kibana 也是一个开源和免费的工具，Kibana可以为Logstash 和ElasticSearch 提供的日志分析友好的Web 界面，可以帮助汇总、分析和搜索重要数据日志。Filebeat隶属于Beats。目前Beats包含四种工具：

Packetbeat（搜集网络流量数据）<br />Topbeat（搜集系统、进程和文件系统级别的CPU 和内存使用情况等数据）<br />Filebeat（搜集文件数据）<br />Winlogbeat（搜集Windows 事件日志数据）

<a name="woLxB"></a>
# 3.实验部署
<a name="xcFqc"></a>
### 1、环境部署
本次部署的是filebeats(客户端)，logstash+elasticsearch+kibana(服务端)组成的架构。业务请求到达nginx-server机器上的Nginx；Nginx响应请求，并在access.log文件中增加访问记录；FileBeat搜集新增的日志，通过LogStash的5044端口上传日志；LogStash将日志信息通过本机的9200端口传入到ElasticSerach；搜索日志的用户通过浏览器访问Kibana，服务器端口是5601；Kibana通过9200端口访问ElasticSerach；

![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1600089134869-5eed93c7-563a-4fd5-930e-693dbb81bd90.png#align=left&display=inline&height=189&originHeight=377&originWidth=765&size=55113&status=done&style=none&width=382.5)

**实验环境：**本次部署的是单点ELK用了两台机器(CentOS-7.5)

| 主机名 | IP地址 | 角色 |
| --- | --- | --- |
| elk-server | 192.168.40.23 | elk |
| client | 192.168.40.24 | nginx |


<a name="iCDo5"></a>
### 2、下载并安装软件包：

下载以下四个软件包

| 1、Elasticsearch  | 服务器端 | wget [https://mirrors.huaweicloud.com/elasticsearch/7.8.1/elasticsearch-7.8.1-x86_64.rpm](https://mirrors.huaweicloud.com/elasticsearch/7.8.1/elasticsearch-7.8.1-x86_64.rpm) |
| --- | --- | --- |
| 2、Logstash | 服务器端 | wget [https://mirrors.huaweicloud.com/logstash/7.8.0/logstash-7.8.0.rpm](https://mirrors.huaweicloud.com/logstash/7.8.0/logstash-7.8.0.rpm) |
| 3、Kibana  | 服务器端 | wget [https://mirrors.huaweicloud.com/kibana/7.8.0/kibana-7.8.0-x86_64.rpm](https://mirrors.huaweicloud.com/kibana/7.8.0/kibana-7.8.0-x86_64.rpm) |
| 4、Filebeat | 客户端 | wget [https://mirrors.huaweicloud.com/filebeat/7.8.0/filebeat-7.8.0-x86_64.rpm](https://mirrors.huaweicloud.com/filebeat/7.8.0/filebeat-7.8.0-x86_64.rpm) |
| 5、jdk1.8 | 所有节点 | yum -y install java-1.8* |

<a name="wxUnN"></a>
### 3、安装部署
```c
[root@elk-server elk]# ll -h
total 821M
-rw-r--r-- 1 root root 305M Jun 18 00:46 elasticsearch-7.8.0-x86_64.rpm
-rw-r--r-- 1 root root  28M Jun 18 00:38 filebeat-7.8.0-x86_64.rpm
-rw-r--r-- 1 root root 329M Jun 18 00:50 kibana-7.8.0-x86_64.rpm
-rw-r--r-- 1 root root 160M Jun 18 00:52 logstash-7.8.0.rpm
[root@elk-server elk]# yum install -y java-1.8*
[root@elk-server elk]# rpm -ivh elasticsearch-7.8.0-x86_64.rpm
Preparing...                          ################################# [100%]
Updating / installing...
   1:elasticsearch-0:7.8.0-1          ################################# [100%]
### NOT starting on installation, please execute the following statements to configure elasticsearch service to start automatically using systemd
 sudo systemctl daemon-reload
 sudo systemctl enable elasticsearch.service
### You can start elasticsearch service by executing
 sudo systemctl start elasticsearch.service
Created elasticsearch keystore in /etc/elasticsearch/elasticsearch.keystore
[root@elk-server elk]# rpm -ivh kibana-7.8.0-x86_64.rpm
Preparing...                          ################################# [100%]
Updating / installing...
   1:kibana-7.8.0-1                   ################################# [100%]
[root@elk-server elk]# rpm -ivh filebeat-7.8.0-x86_64.rpm
Preparing...                          ################################# [100%]
Updating / installing...
   1:filebeat-7.8.0-1                 ################################# [100%]
[root@elk-server elk]#

```

<a name="k02Wp"></a>
### 4、配置elasticsearch地址
```c
[root@elk-server ~]# grep -v "#"  /etc/elasticsearch/elasticsearch.yml
path.data: /var/lib/elasticsearch
path.logs: /var/log/elasticsearch
network.host: 192.168.40.23
cluster.initial_master_nodes: ["192.168.40.23"]
[root@elk-server ~]#
[root@elk-server elk]# systemctl start elasticsearch.service
[root@elk-server elk]# systemctl enable elasticsearch.service
Created symlink from /etc/systemd/system/multi-user.target.wants/elasticsearch.service to /usr/lib/systemd/system/elasticsearch.service.
[root@elk-server elk]# systemctl status elasticsearch.service
● elasticsearch.service - Elasticsearch
   Loaded: loaded (/usr/lib/systemd/system/elasticsearch.service; enabled; vendor preset: disabled)
   Active: active (running) since Tue 2020-09-15 04:40:33 EDT; 25s ago
     Docs: https://www.elastic.co
 Main PID: 22906 (java)
   CGroup: /system.slice/elasticsearch.service
           ├─22906 /usr/share/elasticsearch/jdk/bin/java -Xshare:auto -Des.networkaddress.cache.ttl=60 -Des.networkaddress.cache.negative.ttl=10 -XX:+AlwaysPreT...
           └─23062 /usr/share/elasticsearch/modules/x-pack-ml/platform/linux-x86_64/bin/controller

Sep 15 04:40:09 elk-server systemd[1]: Starting Elasticsearch...
Sep 15 04:40:33 elk-server systemd[1]: Started Elasticsearch.
[root@elk-server elk]# curl 192.168.40.23:9200
{
  "name" : "elk-server",
  "cluster_name" : "elasticsearch",
  "cluster_uuid" : "8tdOFSMhQ_uB8ZUc3qweIQ",
  "version" : {
    "number" : "7.8.0",
    "build_flavor" : "default",
    "build_type" : "rpm",
    "build_hash" : "757314695644ea9a1dc2fecd26d1a43856725e65",
    "build_date" : "2020-06-14T19:35:50.234439Z",
    "build_snapshot" : false,
    "lucene_version" : "8.5.1",
    "minimum_wire_compatibility_version" : "6.8.0",
    "minimum_index_compatibility_version" : "6.0.0-beta1"
  },
  "tagline" : "You Know, for Search"
}

```
<a name="TbB14"></a>
### 5、配置kibana
```c
[root@elk-server ~]# grep -Ev "^$|#" /etc/kibana/kibana.yml
server.host: "192.168.40.23"
elasticsearch.hosts: ["http://192.168.40.23:9200"]
[root@elk-server elk]# systemctl restart kibana
```
访问：[http://192.168.40.23:5601](http://192.168.40.23:5601)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1600167208516-170011b8-74fb-46c4-ad79-95659b68da50.png#align=left&display=inline&height=289&originHeight=578&originWidth=1740&size=94268&status=done&style=none&width=870)
<a name="5xMRo"></a>
### 6、配置filebeat.service
```bash
[root@client ~]# rpm -ivh filebeat-7.8.0-x86_64.rpm
warning: filebeat-7.8.0-x86_64.rpm: Header V4 RSA/SHA512 Signature, key ID d88e42b4: NOKEY
Preparing...                          ################################# [100%]
Updating / installing...
   1:filebeat-7.8.0-1                 ################################# [100%]
[root@client ~]# yum install -y -q nginx
[root@client ~]# grep -A 5 "filebeat.inputs" /etc/filebeat/filebeat.yml
filebeat.inputs:

- type: log
  enabled: true
  paths:
    - /var/log/*.log
[root@client ~]# grep -A 5  "^output" /etc/filebeat/filebeat.yml
output.elasticsearch:
  # Array of hosts to connect to.
  hosts: ["192.168.40.23:9200"]			#修改服务器的ip地址

  # Protocol - either `http` (default) or `https`.
  #protocol: "https"
[root@client ~]# systemctl restart filebeat.service
[root@client ~]# systemctl status filebeat.service
● filebeat.service - Filebeat sends log files to Logstash or directly to Elasticsearch.
   Loaded: loaded (/usr/lib/systemd/system/filebeat.service; disabled; vendor preset: disabled)
   Active: active (running) since Tue 2020-09-15 05:19:08 EDT; 4s ago
     Docs: https://www.elastic.co/products/beats/filebeat
 Main PID: 8708 (filebeat)
   CGroup: /system.slice/filebeat.service
           └─8708 /usr/share/filebeat/bin/filebeat -environment systemd -c /etc/filebeat/filebeat.yml -path.home /usr/share/filebeat -path.config /etc/filebeat -pa

Sep 15 05:19:08 client filebeat[8708]: 2020-09-15T05:19:08.156-0400        INFO        log/harvester.go:297        Harvester started for file: /var/log/Xorg.0.log
Sep 15 05:19:08 client filebeat[8708]: 2020-09-15T05:19:08.157-0400        INFO        log/harvester.go:297        Harvester started for file: /var/log/boot.log
Sep 15 05:19:08 client filebeat[8708]: 2020-09-15T05:19:08.159-0400        INFO        log/harvester.go:297        Harvester started for file: /var/log/yum.log
Sep 15 05:19:08 client filebeat[8708]: 2020-09-15T05:19:08.159-0400        INFO        log/harvester.go:297        Harvester started for file: /var/log/pm-powersav
Sep 15 05:19:08 client filebeat[8708]: 2020-09-15T05:19:08.160-0400        INFO        cfgfile/reload.go:224        Loading of config files completed.
Sep 15 05:19:11 client filebeat[8708]: 2020-09-15T05:19:11.146-0400        INFO        [add_cloud_metadata]        add_cloud_metadata/add_cloud_metadata.go:89
Sep 15 05:19:12 client filebeat[8708]: 2020-09-15T05:19:12.147-0400        INFO        [publisher_pipeline_output]        pipeline/output.go:144        Connecting
Sep 15 05:19:12 client filebeat[8708]: 2020-09-15T05:19:12.147-0400        INFO        [publisher]        pipeline/retry.go:221        retryer: send unwait signal
Sep 15 05:19:12 client filebeat[8708]: 2020-09-15T05:19:12.147-0400        INFO        [publisher]        pipeline/retry.go:225          done
Sep 15 05:19:12 client filebeat[8708]: 2020-09-15T05:19:12.171-0400        INFO        [esclientleg]        eslegclient/connection.go:306        Attempting to conn
Hint: Some lines were ellipsized, use -l to show in full.
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1600167512264-5d697796-9396-4fb8-8bbf-740dcd72d1e9.png#align=left&display=inline&height=325&originHeight=650&originWidth=1063&size=130584&status=done&style=none&width=531.5)

