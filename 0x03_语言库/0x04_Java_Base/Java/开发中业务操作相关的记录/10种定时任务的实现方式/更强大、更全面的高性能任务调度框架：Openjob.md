<a name="qhybY"></a>
## 介绍
Openjob一款全面的高性能任务调度框架，支持多种定时任务、延迟任务、工作流设计、轻量级计算、无限水平扩容，并具有无可比拟的可伸缩性和容错性，以及完善权限管理、强大的对称监控、原生支持多语言。
<a name="oPGSM"></a>
## 特性
<a name="NZtBn"></a>
### 高可靠
多种无状态设计，采用Master/Worker架构，支持多种数据库(MySQL/PostgreSQL/Oracle)
<a name="XXTow"></a>
### 性能
底层采用一致性分片算法，全程无锁化设计，任务调度精确到秒级，支持水平轻量级计算、无限扩容。
<a name="uIW6w"></a>
### 定时
支持定时任务、固定频率任务、高性能秒级任务、间歇任务定时调度。
<a name="IsiD8"></a>
### 全局计算
单机、广播、Map、MapReduce和分片多种编程模型，轻松实现大数据计算支持。
<a name="sOVZc"></a>
### 延迟任务
基于Redis实现高性能延迟任务，底层实现任务多级存储，提供丰富的统计和报表。
<a name="FKW5Q"></a>
### 工作流程
内置工作流调度引擎，支持可视化DAG设计，简单实现高效复杂任务调度。
<a name="ftWlA"></a>
### 权限管理
完善的用户管理，支持菜单、按钮以及数据权限设置，灵活管理用户权限
<a name="ukNYk"></a>
### 报警监控
全面的监控指标，丰富及时的报警方式，基于运维人员快速定位和解决线上问题。
<a name="EO9ID"></a>
### 跨语言
最初支持 Java/Go/PHP/Python 多语言，以及 Spring Boot、Gin、Swoft 等框架集成。
<a name="sGPUL"></a>
## 使用
<a name="WnlJi"></a>
### Docker 拉取镜像
```bash
docker pull openjob/openjob-server:latest
```
<a name="xJGwR"></a>
### 运行容器
新增一个配置文件 .env，配置正确可用的数据库地址。
```
AKKA_REMOTE_HOSTNAME=LocalIP
OJ_DS_URL=jdbc:mysql://172.20.0.235:3306/openjob?useUnicode=true&characterEncoding=UTF-8&serverTimezone=Asia/Shanghai
OJ_LOG_STORAGE_MYSQL_URL=jdbc:mysql://172.20.0.235:3306/openjob?useUnicode=true&characterEncoding=UTF-8&serverTimezone=Asia/Shanghai
```
**提示**

- 数据库必须手动创建
- 容器运行涉及多项参数配置，使用配置文件更方便，其次也可以启动时通过命令传递参数(环境变量)。
- 配置 AKKA_REMOTE_HOSTNAME 为当前机器 IP，不能是 127.0.0.1，否则会导致网络不通，客户端通过 IP 连接 Server。

**运行命令**
```bash
docker run --env-file .env -it -d -p 8080:8080 -p 25520:25520 openjob/openjob-server:latest /bin/bash
```
运行成功，访问 http://127.0.0.1:8080/

- 账号: openjob
- 密码: openjob.io
<a name="ycSDE"></a>
## 优势对比
| **项目** | **Quartz** | **Elastic-Job** | **XXL-JOB** | **Openjob** |
| --- | --- | --- | --- | --- |
| 定时调度 | Cron | Cron | Cron | <br />1. 定时任务 <br />2. 秒级任务 <br />3. 一次性任务 <br />4. 固定频率<br /> |
| 延时任务 | 不支持 | 不支持 | 不支持 | 基于 Redis 实现分布式高性能延时任务，实现定时与延时一体化 |
| 任务编排 | 不支持 | 不支持 | 不支持 | 通过图形化编排任务(workflow) |
| 分布式计算 | 不支持 | 静态分片 | 广播 | <br />1. 广播 <br />2. Map/MapReduce <br />3. 多语言静态分片<br /> |
| 多语言 | Java | <br />1. Java <br />2. 脚本任务<br /> | <br />1. Java <br />2. 脚本任务<br /> | <br />1. Java <br />2. Go(Gin、beego) <br />3. PHP(Swoft)* Python(Agent) <br />4. 脚本任务 <br />5. HTTP任务<br /> |
| 可视化 | 无 | 弱 | <br />1. 历史记录 <br />2. 运行日志（不支持存储）<br />3. 监控大盘<br /> | <br />1. 历史记 <br />2. 运行日志（支持 H2/Mysql/Elasticsearch）<br />3. 监控大盘 <br />4. 操作记录 <br />5. 查看日志堆栈<br /> |
| 可运维 | 无 | 启用、禁用任务 | <br />1. 启用、禁用任务 <br />2. 手动运行任务 <br />3. 停止任务<br /> | <br />1. 启用、禁用任务 <br />2. 手动运行任务 <br />3. 停止任务<br /> |
| 报警监控 | 无 | 邮件 | 邮件 | <br />1. 邮件 <br />2. webhook <br />3. 企微 <br />4. 飞书<br /> |
| 性能 | 每次调度通过DB抢锁，对DB压力大 | ZooKeeper是性能瓶颈 | 由Master节点调度，Master节点压力大。 | 任务采用分片算法，每个节点都可以调度，无性能瓶颈，支持无限水平扩展，支持海量任务调度。 |

Openjob 适用于业务有一定的定时任务需求，非常适用于定时任务与延时任务一体化解决场景，比如每天凌晨数据清理、生成数据报表。同时适用于轻量级计算，可以使用 Map/MapReduce 实现分布式数据计算。对于复杂的任务流或任务编排场景可以使用可视化工作流，轻松解决。
<a name="gjb62"></a>
## 文档
官方网站：[https://openjob.io/](https://openjob.io/)<br />官方文档：[https://openjob.io/zh-Hans/docs/intro/](https://openjob.io/zh-Hans/docs/intro/)<br />Github：[https://github.com/open-job/openjob](https://github.com/open-job/openjob)
