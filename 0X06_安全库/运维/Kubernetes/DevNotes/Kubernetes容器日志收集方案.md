Kubernetes 日志
<a name="ICzFY"></a>
## 概述
<a name="zw6Vo"></a>
### 关于容器日志
Docker的日志分为两类，一类是Docker引擎日志；另一类是容器日志。引擎日志一般都交给了系统日志，不同的操作系统会放在不同的位置。本文主要介绍容器日志，容器日志可以理解是运行在容器内部的应用输出的日志，默认情况下，docker logs显示当前运行的容器的日志信息，内容包含 STOUT（标准输出）和STDERR（标准错误输出）。日志都会以json-file的格式存储于 /var/lib/docker/containers/<容器id>/<容器id>-json.log，不过这种方式并不适合放到生产环境中。

- 默认方式下容器日志并不会限制日志文件的大小，容器会一直写日志，导致磁盘爆满，影响系统应用。（docker log-driver支持log文件的rotate）
- Docker Daemon收集容器的标准输出，当日志量过大时会导致Docker Daemon成为日志收集的瓶颈，日志的收集速度受限。
- 日志文件量过大时，利用docker logs -f查看时会直接将Docker Daemon阻塞住，造成docker ps等命令也不响应。

Docker提供了logging drivers配置，用户可以根据自己的需求去配置不同的log-driver，可参考官网[Configure logging drivers](https://docs.docker.com/v17.09/engine/admin/logging/overview/)。但是上述配置的日志收集也是通过Docker Daemon收集，收集日志的速度依然是瓶颈。
```bash
log-driver 日志收集速度
syslog 14.9 MB/s
json-file 37.9 MB/s
```
能不能找到不通过Docker Daemon收集日志直接将日志内容重定向到文件并自动rotate的工具呢？答案是肯定的采用[S6](http://skarnet.org/software/s6/)基底镜像。<br />S6-log将CMD的标准输出重定向到/.../default/current，而不是发送到 Docker Daemon，这样就避免了Docker Daemon收集日志的性能瓶颈。本文就是采用S6基底镜像构建应用镜像形成统一日志收集方案。
<a name="Ml2j0"></a>
### 关于Kubernetes日志
Kubernetes日志收集方案分成三个级别：
<a name="xUy3F"></a>
#### 应用（Pod）级别
Pod级别的日志，默认是输出到标准输出和标志输入，实际上跟Docker容器的一致。使用`kubectl logs pod-name -n namespace`查看，具体参考：[https://kubernetes.io/docs/reference/generated/kubectl/kubectl-commands#logs](https://kubernetes.io/docs/reference/generated/kubectl/kubectl-commands#logs)。
<a name="Avlt3"></a>
#### 节点级别
Node级别的日志 , 通过配置容器的log-driver来进行管理，这种需要配合logrotare来进行，日志超过最大限制，自动进行rotate操作。<br />![2021-06-25-21-55-18-613577.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624629404752-070b8e22-1c4e-4d71-a76e-72bc9d33b881.png#clientId=ud196cb23-1726-4&from=ui&id=u4bc129ba&originHeight=267&originWidth=500&originalType=binary&ratio=3&size=401492&status=done&style=shadow&taskId=u53fdcbff-11aa-48f4-9a51-c3fe0c0a93d)
<a name="nKf8Y"></a>
#### 集群级别
集群级别的日志收集，有三种。<br />节点代理方式，在Node级别进行日志收集。一般使用DaemonSet部署在每个Node中。这种方式优点是耗费资源少，因为只需部署在节点，且对应用无侵入。缺点是只适合容器内应用日志必须都是标准输出。<br />![2021-06-25-21-55-18-730265.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624629414952-19a8334b-2c30-4402-a787-dae84656e6bb.png#clientId=ud196cb23-1726-4&from=ui&id=u59ae4506&originHeight=326&originWidth=500&originalType=binary&ratio=3&size=490193&status=done&style=shadow&taskId=ub6f3031b-a679-4e42-833c-6e88205d4e3)<br />使用sidecar container作为容器日志代理，也就是在Pod中跟随应用容器起一个日志处理容器，有两种形式：<br />一种是直接将应用容器的日志收集并输出到标准输出（叫做Streaming sidecar container），但需要注意的是，这时候，宿主机上实际上会存在两份相同的日志文件：一份是应用自己写入的；另一份则是sidecar的stdout和stderr对应的JSON文件。这对磁盘是很大的浪费，所以说，除非万不得已或者应用容器完全不可能被修改。<br />![2021-06-25-21-55-18-830996.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1624629430002-fc160b24-cf54-4013-a63b-0b20dfe9e912.jpeg#clientId=ud196cb23-1726-4&from=ui&id=u27e6550c&originHeight=384&originWidth=500&originalType=binary&ratio=3&size=22894&status=done&style=shadow&taskId=u0534a53e-2b32-444b-a031-a5035dddf4d)<br />另一种是每一个Pod中都起一个日志收集agent（比如Logstash或Fluebtd）也就是相当于把方案一里的logging agent放在了Pod里。但是这种方案资源消耗（CPU，内存）较大，并且日志不会输出到标准输出，kubectl logs会看不到日志内容。<br />![2021-06-25-21-55-18-949678.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1624629441538-28fa11e0-8416-4c04-8fd3-2759f0ca7e9e.jpeg#clientId=ud196cb23-1726-4&from=ui&id=u3c882291&originHeight=195&originWidth=500&originalType=binary&ratio=3&size=13038&status=done&style=shadow&taskId=u9f0a5156-b79f-498d-8db8-19c32cc9a2f)<br />应用容器中直接将日志推到存储后端，这种方式就比较简单了，直接在应用里面将日志内容发送到日志收集服务后端。<br />![2021-06-25-21-55-19-061382.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1624629449855-5b6df7ad-c882-4c07-90a5-fc9ddfaeeff3.jpeg#clientId=ud196cb23-1726-4&from=ui&id=udd6745bd&originHeight=131&originWidth=500&originalType=binary&ratio=3&size=10061&status=done&style=shadow&taskId=u552a64c3-9bd8-42ef-ad53-bd617558ae0)
<a name="QQwAn"></a>
## 日志架构
通过上文对Kubernetes日志收集方案的介绍，要想设计一个统一的日志收集系统，可以采用节点代理方式收集每个节点上容器的日志，日志的整体架构如图所示：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624629112344-b3af1cd3-027e-4754-848f-28cd94cf71d8.webp#clientId=ud196cb23-1726-4&from=paste&id=u5444ba78&originHeight=429&originWidth=1074&originalType=url&ratio=3&status=done&style=shadow&taskId=u878f86b4-2650-4d67-97f9-abcf7c2c77b)<br />解释如下：

1. 所有应用容器都是基于S6基底镜像的，容器应用日志都会重定向到宿主机的某个目录文件下比如/data/logs/namespace/appname/podname/log/xxxx.log
2. log-agent内部包含Filebeat，Logrotate等工具，其中Filebeat是作为日志文件收集的agent
3. 通过Filebeat将收集的日志发送到Kafka
4. Kafka在讲日志发送的ES日志存储/kibana检索层
5. Logstash作为中间工具主要用来在ES中创建index和消费Kafka的消息

整个流程很好理解，但是需要解决的是：

1. 用户部署的新应用，如何动态更新Filebeat配置
2. 如何保证每个日志文件都被正常的rotate
3. 如果需要更多的功能则需要二次开发Filebeat，使Filebeat支持更多的自定义配置
<a name="RCqq0"></a>
## 付诸实践
解决上述问题，就需要开发一个log-agent应用以DaemonSet形式运行在Kubernetes集群的每个节点上，应用内部包含Filebeat，Logrotate和需要开发的功能组件。<br />第一个问题，如何动态更新Filebeat配置，可以利用[http://github.com/fsnotify/fsnotify](http://github.com/fsnotify/fsnotify)工具包监听日志目录变化create、delete事件，利用模板渲染的方法更新Filebeat配置文件。<br />第二个问题，利用[http://github.com/robfig/cron](http://github.com/robfig/cron)工具包创建CronJob，定期rotate日志文件，注意应用日志文件所属用户，如果不是root用户所属，可以在配置中设置切换用户。
```bash
/var/log/xxxx/xxxxx.log {
  su www-data www-data
  missingok
  notifempty
  size 1G
  copytruncate
} 
```
<a name="a7ZWr"></a>
## 参考链接

1. [https://docs.docker.com/v17.09/engine/admin/logging/overview/](https://docs.docker.com/v17.09/engine/admin/logging/overview/)
2. [http://skarnet.org/software/s6/](http://skarnet.org/software/s6/)
