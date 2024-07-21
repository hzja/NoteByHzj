分布式对象存储
<a name="RtEpU"></a>
## 一、客户端操作 MinIO Client（mc）
官方文档：[https://docs.min.io/docs/minio-admin-complete-guide.html](https://docs.min.io/docs/minio-admin-complete-guide.html)<br />MinIO Client (mc) 为 UNIX 命令（如 ls、cat、cp、mirror、diff、find 等）提供了现代替代方案。它支持文件系统和兼容 Amazon S3 的云存储服务（AWS Signature v2 和 v4）。
```bash
alias       set, remove and list aliases in configuration file
ls          list buckets and objects
mb          make a bucket
rb          remove a bucket
cp          copy objects
mirror      synchronize object(s) to a remote site
cat         display object contents
head        display first 'n' lines of an object
pipe        stream STDIN to an object
share       generate URL for temporary access to an object
find        search for objects
sql         run sql queries on objects
stat        show object metadata
mv          move objects
tree        list buckets and objects in a tree format
du          summarize disk usage recursively
retention   set retention for object(s)
legalhold   set legal hold for object(s)
diff        list differences in object name, size, and date between two buckets
rm          remove objects
encrypt    manage bucket encryption config
event       manage object notifications
watch       listen for object notification events
undo        undo PUT/DELETE operations
policy      manage anonymous access to buckets and objects
tag         manage tags for bucket(s) and object(s)
ilm         manage bucket lifecycle
version     manage bucket versioning
replicate   configure server side bucket replication
admin       manage MinIO servers
update      update mc to latest release
```
<a name="PNszF"></a>
### **1）mc Shell 自动补全**
如果使用的是 bash、zsh 或 fish。Shell 补全默认嵌入在 mc，安装自动补全使用`mc --autocompletion`。重新启动 shell，mc 将自动完成命令，如下所示。
```bash
# 安装
mc --autocompletion
# 重启shell，即退出shell，重新连接即可
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660315979960-6848ae7b-0d85-43f3-b582-eb0c6df757ed.png#clientId=u940c64fe-ac12-4&from=paste&id=u29324047&originHeight=101&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud005fd24-006a-4078-b276-3e7d6361caf&title=)
<a name="gtfEG"></a>
### **2）查看 mc 版本**
```bash
mc --version
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660315979943-9918087c-af30-48fa-89e2-f8e438bedfb8.png#clientId=u940c64fe-ac12-4&from=paste&id=ue1ad9f88&originHeight=131&originWidth=1000&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ufed4e218-6d0e-42ac-a07d-a32969a3502&title=)
<a name="brnbO"></a>
### **3）列出来自**[https://play.min.io](https://play.min.io/)**的所有存储桶**
```bash
mc ls play
# json格式
mc --json ls play
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660315979973-f4b1327c-9204-4be5-87d8-8298b765748a.png#clientId=u940c64fe-ac12-4&from=paste&id=uebe7d671&originHeight=829&originWidth=914&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u811ddb99-0d13-41f4-a991-87af6c329e6&title=)
<a name="HAPFs"></a>
### **4）创建桶**
mb命令在对象存储上创建一个新存储桶。在文件系统上，它的行为类似于 mkdir -p 命令。Bucket 相当于文件系统中的驱动器或挂载点，不应被视为文件夹。MinIO 对每个用户创建的存储桶数量没有任何限制。
```bash
# 添加MinIO存储服务
cd /opt/bigdata/minio
./mc config host add minio http://local-168-182-110:19000 admin admin123456

# 在自己部署的minio创建新存储桶
mc mb minio/mybucket
# 查看所有桶
mc ls minio
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660315980066-13b4b1fc-d036-4318-ba91-73a95ce5ebf4.png#clientId=u940c64fe-ac12-4&from=paste&id=u9269961a&originHeight=488&originWidth=1034&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2caca241-38d6-4c8e-a7bd-b0984e476d4&title=)<br />web 访问：http://local-168-182-110:19001<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660315979918-34c827e1-be40-4b9d-85e0-1eeb8019e217.png#clientId=u940c64fe-ac12-4&from=paste&id=u475cbb0b&originHeight=330&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u53f72563-db89-42f6-b031-89b49805703&title=)
<a name="YReQe"></a>
### **5）复制文件到 MinIO**
cp命令将数据从一个或多个源复制到目标。
```bash
mc cp wget-log minio/mybucket
# 查看
mc ls wget-log minio/mybucket
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660315980540-bb5a1dc8-fba3-4808-8784-0b77d327c5e0.png#clientId=u940c64fe-ac12-4&from=paste&id=uc9e77f7a&originHeight=153&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9e435527-8cb3-411b-9ab5-e731741f3f6&title=)
<a name="z7t1F"></a>
### **6）日常使用**
可以添加 shell 别名来覆盖常用的 Unix 工具。
```bash
alias ls='mc ls'
alias cp='mc cp'
alias cat='mc cat'
alias mkdir='mc mb'
alias pipe='mc pipe'
alias find='mc find'
alias tree='mc tree'
```
<a name="su2XN"></a>
### **7）MinIO 管理员操作**
MinIO Client (mc) 提供 admin 子命令来对 MinIO 部署执行管理任务。
```bash
service              restart and stop all MinIO servers
update               update all MinIO servers
info                 display MinIO server information
user                 manage users
group                manage groups
policy               manage policies defined in the MinIO server
replicate            manage MinIO site replication
config               manage MinIO server configuration
decommission, decom  manage MinIO server pool decommissioning
heal                 heal disks, buckets and objects on MinIO server
prometheus           manages prometheus config
kms                  perform KMS management operations
bucket               manage buckets defined in the MinIO server
tier                 manage remote tier targets for ILM transition
top                  provide top like statistics for MinIO
trace                show http trace for MinIO server
console              show console logs for MinIO server
```
<a name="qpHu2"></a>
### **1）查看集群信息**
```bash
mc admin info minio
# 为了方便使用，可以设置别名
alias minfo='mc admin info'
minfo minio
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660315980647-c2a06436-dde1-4108-b72e-2a9b6784d33e.png#clientId=u940c64fe-ac12-4&from=paste&id=u7fde1e9c&originHeight=583&originWidth=769&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2546b545-5746-4c36-bd1a-7f5e3da5d3a&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660315980720-1af9be57-fffe-49df-a35f-e7e41d96f570.png#clientId=u940c64fe-ac12-4&from=paste&id=u0970f555&originHeight=584&originWidth=834&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9a3ec77a-c145-40c8-8155-3ae9f2f5fe1&title=)
<a name="W0Cxz"></a>
### **2）全局选项**
<a name="YRUej"></a>
#### 1、选项 [`--debug`]
【示例】显示 info 命令的详细调试输出
```bash
mc admin info --debug minio
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660315980766-10ff960c-59ac-4d28-b46e-48d6a3bf06ef.png#clientId=u940c64fe-ac12-4&from=paste&id=u2418064a&originHeight=518&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9dcb01a1-05b1-4071-8591-24d0edf67c0&title=)
<a name="qL7Zg"></a>
#### 2、选项 [`--json`]
JSON 选项启用 JSON 行格式的可解析输出。<br />【示例】MinIO 服务器信息
```bash
mc admin --json info minio
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660315980994-76cde864-dc0b-4984-8e4e-672f25836165.png#clientId=u940c64fe-ac12-4&from=paste&id=u2c1a0a7c&originHeight=687&originWidth=862&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua26415de-1a6a-49ef-b89e-56232d61901&title=)
<a name="UOTYT"></a>
### **3）命令 service- 重启和停止所有 MinIO 服务器**
service 命令提供了一种重新启动和停止所有 MinIO 服务器的方法。<br />【示例】
```bash
mc admin service restart minio
```
<a name="u1wQx"></a>
### **4）命令 policy- 管理预设策略**
policy 添加、删除、列出策略、获取策略信息以及为 MinIO 服务器上的用户设置策略的命令。
```bash
NAME:
  mc admin policy - manage policies

FLAGS:
  --help, -h                       show help

COMMANDS:
  add      add new policy
  remove   remove policy
  list     list all policies
  info     show info on a policy
  set      set IAM policy on a user or group
```
<a name="Avxvb"></a>
#### 1、【示例】列出 MinIO 上的所有预设策略
```bash
mc admin policy list minio
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660315981219-100b66bc-0db0-43a2-b7bf-967ba751b50a.png#clientId=u940c64fe-ac12-4&from=paste&id=u25e48dc7&originHeight=197&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7706a72f-0462-4c4a-aeb7-78ea09ff3b3&title=)
<a name="Q3Sav"></a>
#### 2、【示例】在 MinIO 上添加新策略“listbucketsonly”
策略来自 /tmp/listbucketsonly.json。当此策略应用于用户时，该用户只能列出顶层存储桶，但不能列出其他任何内容，没有前缀，没有对象。<br />首先使用以下信息创建 json 文件 /tmp/listbucketsonly.json。
```json
{
	"Version": "2012-10-17",
	"Statement": [
		{
			"Effect": "Allow",
			"Action": [
				"s3:ListAllMyBuckets"
			],
			"Resource": [
				"arn:aws:s3:::*"
			]
		}
	]
}
```
将策略作为“listbucketsonly”添加到策略数据库
```bash
mc admin policy add minio listbucketsonly /tmp/listbucketsonly.json

# 删除 MinIO 上的策略“listbucketsonly”
mc admin policy remove minio listbucketsonly
```
<a name="YQTxa"></a>
#### 3、【示例】在用户或组上设置标准策略。'writeonly'
```bash
# 添加一个新用户“newuser”，密码"newuser123"
mc admin user add minio newuser newuser123
# 给用户添加策略
mc admin policy set minio writeonly user=newuser
# 添加组“somegroup ”，把用户“newuser ”添加到组
mc admin group add minio somegroup newuser
# 给组添加策略
mc admin policy set minio writeonly group=somegroup
```
<a name="bljjR"></a>
### **5）命令 user- 管理用户**
<a name="MWU9m"></a>
#### 1、【示例】在 MinIO 上添加一个新用户“newuser”
```bash
# 添加一个新用户“newuser”，密码"newuser123"
mc admin user add minio newuser newuser123
```
<a name="AF9Fz"></a>
#### 2、【示例】在 MinIO 上禁用用户“newuser”
```bash
mc admin user disable minio newuser
```
<a name="sb54g"></a>
#### 3、【示例】在 MinIO 上启用用户“newuser”
```bash
mc admin user enable minio newuser
```
<a name="A9QoT"></a>
#### 4、【示例】在 MinIO 上删除用户“newuser”
```bash
mc admin user remove minio newuser
```
<a name="AJhP0"></a>
#### 5、【示例】列出 MinIO 上的所有用户
```bash
mc admin user list --json minio
```
<a name="gBu0v"></a>
#### 6、【示例】显示用户信息
```bash
mc admin user info minio newuser
```
<a name="izh4i"></a>
### **6）命令 group- 管理组**
group 在 MinIO 服务器上添加、删除、信息、列出、启用、禁用组的命令。
<a name="HT8Yw"></a>
#### 1、【示例】将用户添加到 MinIO 上的组“somegroup”
```bash
# 如果组不存在，则创建组。
mc admin group add minio somegroup newuser
```
<a name="ER9Uz"></a>
#### 2、【示例】从 MinIO 上的组“somegroup”中删除用户
```bash
mc admin group remove minio somegroup newuser
```
<a name="QO2oJ"></a>
#### 3、【示例】在 MinIO 上删除一个组“somegroup”
仅在给定组为空时有效，非空组无法删除
```bash
mc admin group remove minio somegroup
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660315981121-a6747818-47c6-4dfe-88df-1d60b8096425.png#clientId=u940c64fe-ac12-4&from=paste&id=uc0885be8&originHeight=137&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u97ac9eed-1c48-4cb2-9556-939026e5190&title=)
<a name="fSQmo"></a>
#### 4、【示例】在 MinIO 上获取组“somegroup”的信息
```bash
# 添加组
mc admin group add minio somegroup newuser
# 删除
mc admin group info minio somegroup
```
<a name="eHypD"></a>
#### 5、【示例】列出 MinIO 上的所有组
```bash
mc admin group list minio
```
<a name="OrVPt"></a>
#### 6、【示例】在 MinIO 上启用组“somegroup”
```bash
mc admin group enable minio somegroup
```
<a name="zjyA1"></a>
#### 7、【示例】在 MinIO 上禁用组“somegroup”
```bash
mc admin group disable minio somegroup
```
<a name="f4ueW"></a>
### **7）命令 config- 管理服务器配置**
`config`命令来管理 MinIO 服务器配置。
<a name="dKPVs"></a>
#### 1、【示例】获取“etcd”子系统配置
```bash
# 通过引入第三方组件etcd，在不动原有集群的基础上实现动态扩展方案
mc admin config get minio etcd
```
<a name="Klmpb"></a>
#### 2、【示例】在“etcd”子系统上设置特定设置
```bash
mc admin config set minio etcd endpoints=http://[hostname|ip]:2379
```
<a name="vpoPp"></a>
#### 3、【示例】获取 MinIO 服务器/集群的整个服务器配置
```bash
mc admin config export minio > /tmp/my-serverconfig
```
<a name="c4SPX"></a>
#### 4、【示例】设置 MinIO 服务器/集群的整个服务器配置
```bash
mc admin config import minio < /tmp/my-serverconfig
```
这里列出了常用的命令，想了解更多，可以查看官方文档：[https://docs.min.io/docs/minio-admin-complete-guide.html](https://docs.min.io/docs/minio-admin-complete-guide.html)<br />MinIO Client（mc）操作还是非常简单的，但是更多的使用还是通过程序去调用 api 接口的，后续会讲解~
<a name="gH5oo"></a>
## 二、MinIO 分布式集群扩容
参考：[https://docs.min.io/docs/distributed-minio-quickstart-guide.html](https://docs.min.io/docs/distributed-minio-quickstart-guide.html)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660315981475-1149e1b8-e259-4e43-9ac9-26400c56f31e.png#clientId=u940c64fe-ac12-4&from=paste&id=u2eefb569&originHeight=608&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4ea87189-1382-462b-8d9a-ea55cb78b5c&title=)<br />MinIO 集群扩容方法：<br />常见的集群扩容方法可分为两类：**水平扩容**和**垂直扩容**。

- **水平扩容，一般指通过增加节点数扩展系统性能；**
- **而垂直扩容则指提升各节点自身的性能，例如增加节点的磁盘存储空间。直接采用垂直扩容方式扩容 MinIO 集群的节点磁盘空间，会为集群运行带来若干问题，官方也并不推荐。因此本文主要介绍 MinIO 的两种水平扩容方式：对等扩容和联邦扩容。**
<a name="FcXfA"></a>
### **1）对等扩容**
首先，MinIO 的极简设计理念使得 MinIO 分布式集群并**不支持向集群中添加单个节点并进行自动调节的扩容方式**，这是因为加入单个节点后所引发的数据均衡以及纠删组划分等问题会为整个集群带来复杂的调度和处理过程，并不利于维护。因此，MinIO 提供了一种对等扩容的方式，即**要求增加的节点数和磁盘数均需与原集群保持对**等。

- **例如原集群包含2 个节点 2 块磁盘，则在扩容时必须同样增加 2 个节点 2 块磁盘（或为其倍数），以便系统维持相同的数据冗余 SLA，从而极大地降低扩容的复杂性；**
- **如上例，在扩容后，MinIO 集群并不会对全部的 4 个节点进行完全的数据均衡，而是将原本的2 个节点视作一个区域，新加入的 2 节点视作另一区域；**
- **当有新对象上传时，集群将依据各区域的可用空间比例确定存放区域，在各区域内仍旧通过哈希算法确定对应的纠删组进行最终的存放。此外，集群进行一次对等扩容后，还可依据扩容规则继续进行对等扩容，但出于安全性考虑，集群的最大节点数一般不得超过 32 个。**

对等扩容的优点在于配置操作简单易行<br />通过一条命令即可完成扩容（注意：推荐使用连续的节点 IP，并参照 MinIO 官网在扩容命令中使用{}）。而对等扩容的局限性在于：

- **扩容需重启；**
- **扩容存在限制，集群节点数一般不超过 32 个，这是由于 MinIO 集群通过分布式锁保证强一致性，若集群节点数过大，维护强一致性将带来性能问题。**

【温馨提示】这里不沿用之前的集群进行扩容，这里重新部署 2 个节点的集群。
<a name="mEadM"></a>
#### 1、环境准备
| **主机名** | **IP** | **data** | **备注** |
| --- | --- | --- | --- |
| local-168-182-110 | 192.168.182.110 | /opt/bigdata/minio/data/export{1,2} | 原始节点 |
| local-168-182-111 | 192.168.182.111 | /opt/bigdata/minio/data/export{1,2} | 原始节点 |
| local-168-182-112 | 192.168.182.112 | /opt/bigdata/minio/data/export{1,2} | 扩容节点 |
| local-168-182-113 | 192.168.182.113 | /opt/bigdata/minio/data/export{1,2} | 扩容节点 |

启动脚本
```bash
#!/bin/bash
# 创建日志存储目录
mkdir -p /opt/bigdata/minio/logs
# 分别在三个节点上创建存储目录
mkdir -p /opt/bigdata/minio/data/export{1,2,3,4}
# 创建配置目录
mkdir -p /etc/minio
export MINIO_ROOT_USER=admin
export MINIO_ROOT_PASSWORD=admin123456

# 所有节点都得修改重启服务
# --address "0.0.0.0:9000" 挂载9001端口为api端口（如Java客户端）访问的端口
# --console-address ":9000" 挂载9000端口为web端口；
/opt/bigdata/minio/minio server --address 0.0.0.0:9000 --console-address 0.0.0.0:9001 --config-dir /etc/minio \
http://192.168.182.11{0...1}/opt/bigdata/minio/data/export{1...2} > /opt/bigdata/minio/logs/minio_server.log
```
minio 初始集群信息<br />web 访问：http://local-168-182-110:19001<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660315981333-a2c63e71-22b3-4c2d-b817-e2cef62ea205.png#clientId=u940c64fe-ac12-4&from=paste&id=ucc1e56e0&originHeight=583&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub6fee449-10bc-4a05-a490-5d8f2db23c2&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660315981727-d0399cd9-15a5-4a83-8aee-22b8558054c1.png#clientId=u940c64fe-ac12-4&from=paste&id=uad87e3a6&originHeight=525&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u64047dd5-a205-4f91-9355-cf59a3e772c&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660315981772-89ebb0a0-3700-422b-8c25-05c897e8aa9d.png#clientId=u940c64fe-ac12-4&from=paste&id=u720d4d3d&originHeight=827&originWidth=919&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub1373858-a0ff-4d16-bbf7-ed2222616b7&title=)
<a name="StCrU"></a>
#### 1、将配置 copy 一份到要扩容的节点
```bash
# 在local-168-182-110执行
cd /opt/bigdata/minio
scp -r /opt/bigdata/minio local-168-182-113:/opt/bigdata/
scp -r /usr/lib/systemd/system/minio.service local-168-182-113:/usr/lib/systemd/system/minio.service
```
<a name="uAAG6"></a>
#### 2、添加磁盘
这里也添加 2 块 2G 的磁盘
```bash
# 不重启，直接刷新磁盘数据总线，获取新加的磁盘
for host in $(ls /sys/class/scsi_host) ; do echo "- - -" > /sys/class/scsi_host/$host/scan; done

lsblk

# 格式化
mkfs.ext4 /dev/sdb
mkfs.ext4 /dev/sdc
mkfs.ext4 /dev/sdd
mkfs.ext4 /dev/sde

# 先删再创建挂载目录，因为这里面有其它节点的数据
rm -fr /opt/bigdata/minio/data/export{1..4}
mkdir -p /opt/bigdata/minio/data/export{1..4}

# 挂载
mount /dev/sdb /opt/bigdata/minio/data/export1
mount /dev/sdc /opt/bigdata/minio/data/export2
mount /dev/sdd /opt/bigdata/minio/data/export3
mount /dev/sde /opt/bigdata/minio/data/export4
```
<a name="ZeFhQ"></a>
#### 3、修改启动脚本（run.sh）
```bash
#!/bin/bash
# 创建日志存储目录
mkdir -p /opt/bigdata/minio/logs
# 分别在三个节点上创建存储目录
mkdir -p /opt/bigdata/minio/data/export{1,2}
# 创建配置目录
mkdir -p /etc/minio
export MINIO_ROOT_USER=admin
export MINIO_ROOT_PASSWORD=admin123456

# 所有节点都得修改重启服务
# --address "0.0.0.0:9000" 挂载9001端口为api端口（如Java客户端）访问的端口
# --console-address ":9000" 挂载9000端口为web端口；
# 扩容地址：http://192.168.182.11{2...3}/opt/bigdata/minio/data/export{1...2}
/opt/bigdata/minio/minio server --address 0.0.0.0:9000 --console-address 0.0.0.0:9001 --config-dir /etc/minio \
http://192.168.182.11{0...1}/opt/bigdata/minio/data/export{1...2} \
http://192.168.182.11{2...3}/opt/bigdata/minio/data/export{1...2} > /opt/bigdata/minio/logs/minio_server.log
```
可扩容的方式，使用 MINIO 纠删码，如果想要扩容，必须使用这种方式。注意上面是**三个点**。<br />下面的写法多硬盘多节点部署是不可扩容的方式，下面是官方示例：
```bash
export MINIO_ACCESS_KEY=<ACCESS_KEY>
export MINIO_SECRET_KEY=<SECRET_KEY>
minio server http://192.168.1.11/export1 http://192.168.1.11/export2 \
               http://192.168.1.11/export3 http://192.168.1.11/export4 \
               http://192.168.1.12/export1 http://192.168.1.12/export2 \
               http://192.168.1.12/export3 http://192.168.1.12/export4 \
               http://192.168.1.13/export1 http://192.168.1.13/export2 \
               http://192.168.1.13/export3 http://192.168.1.13/export4 \
               http://192.168.1.14/export1 http://192.168.1.14/export2 \
               http://192.168.1.14/export3 http://192.168.1.14/export4
```
<a name="D4Mi0"></a>
#### 4、重启服务
```bash
systemctl restart minio
```
web 访问：http://local-168-182-110:19001<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660315981863-f4d955ce-db71-4971-8e92-56870a50190d.png#clientId=u940c64fe-ac12-4&from=paste&id=u1aa553dd&originHeight=614&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u443688c1-63e8-470e-9db0-9781c98f81c&title=)<br />到这里就完成了对等扩容了。
<a name="lvJlC"></a>
### **2）联邦扩容**
MinIO 官方提供了另一种扩容机制——联邦扩容，即通过引入 etcd，将多个 MinIO 分布式集群在逻辑上组成一个联邦，对外以一个整体提供服务，并提供统一的命名空间。MinIO 联邦集群的架构如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660315982164-1f1382a8-ce86-4fdf-b1ff-2a4ed30913c9.png#clientId=u940c64fe-ac12-4&from=paste&id=u93e89d40&originHeight=304&originWidth=590&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u89d55b8d-dc12-4464-ac7b-af9e74f13b9&title=)

- **其中，etcd是一个开源的分布式键值存储数据库，在联邦中用于记录存储桶 IP 地址。**
- **联邦内的各个集群其数据存储以及一致性维护仍由各集群自行管理，联邦只是对外提供一个整体逻辑视图。**
- **通过连接到联邦中任一集群的任一节点，可以查询并访问联邦内所有集群的全部数据，由此获得了逻辑上的空间扩大感。**
- **但实际上，对于一个外部应用访问，联邦需依赖 etcd 定位到存储桶的实际存储节点，再进行数据访问，联邦则对外屏蔽了桶 IP 查找和定位过程，从而在逻辑上对外形成了一个统一整体。因此，etcd 实际上起到了类似路由寻址的效果。**

MinIO 联邦集群的数据访问机制具体如下：

- **客户端应用向联邦集群发送创建存储桶请求，桶名为 bucket1；**
- **联邦会将 bucket1 实际所在的集群节点 IP 地址写入 etcd 中，例如 bucket1 实际将存储于联邦中的集群 1 上，而集群 1 包含 2 个节点，其节点 IP 地址分别为 192.168.182.112 和 192.168.182.113，则 etcd 中将写入如下两条记录：**
<a name="II4wH"></a>
#### 1、部署 etcd
下载地址：[https://github.com/etcd-io/etcd/releases](https://github.com/etcd-io/etcd/releases)
```bash
mkdir -p /opt/bigdata/etcd ; cd /opt/bigdata/etcd
wget https://github.com/etcd-io/etcd/releases/download/v3.4.20/etcd-v3.4.20-linux-amd64.tar.gz
tar -xf etcd-v3.4.20-linux-amd64.tar.gz
# 创建存储目录
mkdir /opt/bigdata/etcd/data
```
创建配置文件 /usr/local/etcd/conf.yml，三个节点配置文件内容分别如下：<br />etcd01：local-168-182-110
```bash
name: etcd01
data-dir: /opt/bigdata/etcd/data
initial-advertise-peer-urls: http://192.168.182.110:2380
listen-peer-urls: http://192.168.182.110:2380
listen-client-urls: http://192.168.182.110:2379,http://127.0.0.1:2379
advertise-client-urls: http://192.168.182.110:2379
initial-cluster-token: etcd-cluster
initial-cluster: etcd01=http://192.168.182.110:2380,etcd02=http://192.168.182.111:2380,etcd03=http://192.168.182.112:2380
initial-cluster-state: new
```
etcd02：local-168-182-111
```bash
name: etcd02
data-dir: /opt/bigdata/etcd/data
initial-advertise-peer-urls: http://192.168.182.111:2380
listen-peer-urls: http://192.168.182.111:2380
listen-client-urls: http://192.168.182.111:2379,http://127.0.0.1:2379
advertise-client-urls: http://192.168.182.111:2379
initial-cluster-token: etcd-cluster
initial-cluster: etcd01=http://192.168.182.110:2380,etcd02=http://192.168.182.111:2380,etcd03=http://192.168.182.112:2380
initial-cluster-state: new
```
etcd03：local-168-182-112
```bash
name: etcd03
data-dir: /opt/bigdata/etcd/data
initial-advertise-peer-urls: http://192.168.182.112:2380
listen-peer-urls: http://192.168.182.112:2380
listen-client-urls: http://192.168.182.112:2379,http://127.0.0.1:2379
advertise-client-urls: http://192.168.182.112:2379
initial-cluster-token: etcd-cluster
initial-cluster: etcd01=http://192.168.182.110:2380,etcd02=http://192.168.182.111:2380,etcd03=http://192.168.182.112:2380
initial-cluster-state: new
```
【温馨提示】只能写 IP，写主机名会启动失败<br />配置参数解析：

- **name：当前 etcd 节点名称。**
- **data-dir：数据存储目录。**
- **initial-advertise-peer-urls：集群的其他节点通过该地址与当前节点通信。**
- **listen-peer-urls：当前节点通过该地址监听集群其他节点发送的信息。**
- **listen-client-urls：当前节点通过该地址监听客户端发送的信息。**
- **advertise-client-urls：客户端通过该地址与当前节点通信**
- **initial-cluster-token：用于区分不同的集群，同一集群的所有节点配置相同的值。**
- **initial-cluster：当前集群的所有节点信息，当前节点根据此信息与其他节点取得联系。**
- **initial-cluster-state：本次是否为新建集群，有两个取值：new 和 existing。**

**配置服务**
```bash
cat >/usr/lib/systemd/system/etcd.service<<EOF
[Unit]
Description=etcd
After=network.target

[Service]
Type=notify
ExecStart=/opt/bigdata/etcd/etcd-v3.4.20-linux-amd64/etcd --config-file=/usr/local/etcd/conf.yml

[Install]
WantedBy=multi-user.target
EOF
```
**启动服务**
```bash
systemctl daemon-reload
systemctl start etcd.service
```
验证
```bash
# 加软连接
ln -s /opt/bigdata/etcd/etcd-v3.4.20-linux-amd64/etcdctl /usr/local/bin/etcdctl
# 查看集群成员列表
etcdctl member list
# 查看集群成员健康情况
etcdctl endpoint health --endpoints=http://192.168.182.110:2380,http://192.168.182.111:2380,http://192.168.182.112:2380
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660315982187-c530ad7c-fb4a-4ad1-ba0b-4ee1e92a6c4e.png#clientId=u940c64fe-ac12-4&from=paste&id=u379b257d&originHeight=178&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u82a9c4f3-093d-4a10-80bb-960259d5ae4&title=)<br />查看 etcd 版本
```bash
ln -s /opt/bigdata/etcd/etcd-v3.4.20-linux-amd64/etcd /usr/local/bin/etcd
etcd --version
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660315982482-51b21d09-232b-4f53-9b1c-2c5fd852e8d7.png#clientId=u940c64fe-ac12-4&from=paste&id=ubd5a3d19&originHeight=151&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub2713ff4-b59b-44b9-9ed6-40ecd746d9b&title=)
<a name="tooYk"></a>
#### 2、运行多个 MinIO 集群
集群 1
```bash
cd /opt/bigdata/minio
export MINIO_ETCD_ENDPOINTS="http://192.168.182.110:2380,http://192.168.182.111:2380,http://192.168.182.112:2380"
mkdir -p /etc/minio
export MINIO_ROOT_USER=admin
export MINIO_ROOT_PASSWORD=admin123456
export MINIO_PUBLIC_IPS=192.168.182.110,192.168.182.111
./minio server --address 0.0.0.0:8000 --console-address 0.0.0.0:8001  --config-dir /etc/minio http://192.168.182.11{0...1}/opt/bigdata/minio/data/export{3...4}
```
web 访问：http://local-168-182-110:8001/<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660315982380-08c7db21-8bcd-4e12-81e4-06c580141c54.png#clientId=u940c64fe-ac12-4&from=paste&id=u1cbf52ac&originHeight=557&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u99ac22e3-d348-4bd0-97d0-776486bf60f&title=)<br />集群 2
```bash
cd /opt/bigdata/minio
export MINIO_ETCD_ENDPOINTS="http://192.168.182.110:2380,http://192.168.182.111:2380,http://192.168.182.112:2380"
mkdir -p /etc/minio
export MINIO_ROOT_USER=admin
export MINIO_ROOT_PASSWORD=admin123456
export MINIO_PUBLIC_IPS=192.168.182.112,192.168.182.113
./minio server --address 0.0.0.0:8000 --console-address 0.0.0.0:8001  --config-dir /etc/minio http://192.168.182.11{2...3}/opt/bigdata/minio/data/export{3...4}
```
web 访问：http://local-168-182-112:8001/<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660315982522-760eecb6-0a9d-4bae-ad71-d37f66831c2a.png#clientId=u940c64fe-ac12-4&from=paste&id=u4013a8e2&originHeight=539&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3b76d961-ff48-4474-9388-84dbceeae4c&title=)<br />配置 nginx
```nginx
# /etc/nginx/conf.d/minio2.conf
upstream minio_api2 {
	ip_hash;
	server 192.168.182.110:8000;
	server 192.168.182.111:8000;
	server 192.168.182.112:8000;
	server 192.168.182.113:8000;
}

upstream minio_console2 {
	ip_hash;
	server 192.168.182.110:8001;
	server 192.168.182.111:8001;
	server 192.168.182.112:8001;
	server 192.168.182.113:8001;
}

server{
	listen       18000;
	server_name  192.168.182.110;

	ignore_invalid_headers off;
	client_max_body_size 0;
	proxy_buffering off;

	location / {
		proxy_set_header   X-Forwarded-Proto $scheme;
		proxy_set_header   Host              $http_host;
		proxy_set_header   X-Real-IP         $remote_addr;

		proxy_connect_timeout 300;
		proxy_http_version 1.1;
		chunked_transfer_encoding off;
		proxy_ignore_client_abort on;

		proxy_pass http://minio_api2;
	}
}

server{
	listen       18001;
	server_name  192.168.182.110;

	ignore_invalid_headers off;
	client_max_body_size 0;
	proxy_buffering off;

	location / {
		proxy_set_header   X-Forwarded-Proto $scheme;
		proxy_set_header   Host              $http_host;
		proxy_set_header   X-Real-IP         $remote_addr;

		proxy_connect_timeout 300;
		proxy_http_version 1.1;
		chunked_transfer_encoding off;
		proxy_ignore_client_abort on;

		proxy_pass http://minio_console2;
	}
}
```
重新加载 nginx 生效
```bash
systemctl reload nginx
```
web 访问：local-168-182-110:18001/<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660315982531-8ca01de2-2b78-461c-944b-97b14066c966.png#clientId=u940c64fe-ac12-4&from=paste&id=uad2f6dbe&originHeight=534&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u50fb462c-a570-4a97-a344-8661c2d6865&title=)<br />不同的客户端访问会显示不同的节点信息

---

MinIO 的基本操作和 MinIO 扩容实战就到这里了。
