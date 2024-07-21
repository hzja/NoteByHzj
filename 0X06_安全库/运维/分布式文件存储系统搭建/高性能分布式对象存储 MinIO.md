分布式对象存储MinIO
<a name="fuPYj"></a>
## **一、概述**
MinIO 是在 GNU Affero 通用公共许可证 v3.0 下发布的**高性能对象存储**。它与 Amazon S3 云存储服务 API 兼容。使用 MinIO 为机器学习、分析和应用程序数据工作负载构建高性能基础架构。<br />官方文档：[https://docs.min.io/](https://docs.min.io/)<br />中文文档：[http://docs.minio.org.cn/docs/](http://docs.minio.org.cn/docs/)<br />GitHub 地址：[https://github.com/minio/minio](https://github.com/minio/minio)<br />特点：

- **数据保护——分布式 Minio 采用 纠删码来防范多个节点宕机和位衰减 bit rot。分布式 Minio 至少需要 4 个硬盘，使用分布式 Minio 自动引入了纠删码功能。**
- **高可用——单机 Minio 服务存在单点故障，相反，如果是一个有 N 块硬盘的分布式 Minio，只要有 N/2 硬盘在线，数据就是安全的。不过需要至少有 N/2+1 个硬盘来创建新的对象。**

例如，一个 16 节点的 Minio 集群，每个节点 16 块硬盘，就算 8 台服務器宕机，这个集群仍然是可读的，不过需要 9 台服務器才能写数据。<br />【温馨提示】只要遵守分布式 Minio 的限制，可以组合不同的节点和每个节点几块硬盘。比如，可以使用 2 个节点，每个节点 4 块硬盘，也可以使用 4 个节点，每个节点两块硬盘，诸如此类。

- **一致性——Minio 在分布式和单机模式下，所有读写操作都严格遵守 read-after-write 一致性模型。**

MinIO 的优点如下：

- **部署简单，一个二进制文件（minio）即是一切，还可以支持各种平台**
- **支持海量存储，可以按 zone 扩展，支持单个对象最大 5TB**
- **低冗余且磁盘损坏高容忍，标准且最高的数据冗余系数为 2(即存储一个 1M 的数据对象，实际占用磁盘空间为 2M)。但在任意 n/2 块 disk 损坏的情况下依然可以读出数据(n 为一个纠删码集合中的 disk 数量)。并且这种损坏恢复是基于单个对象的，而不是基于整个存储卷的**
- **读写性能优异**
<a name="Q8mus"></a>
## **二、MinIO 基础概念**

- **S3——Simple Storage Service，简单存储服务，这个概念是 Amazon 在 2006 年推出的，对象存储就是从那个时候诞生的。S3 提供了一个简单 Web 服务接口，可用于随时在 Web 上的任何位置存储和检索任何数量的数据。**
- **Object——存储到 Minio 的基本对象，如文件、字节流，Anything…**
- **Bucket——用来存储 Object 的逻辑空间。每个 Bucket 之间的数据是相互隔离的。**
- **Drive——部署 Minio 时设置的磁盘，Minio 中所有的对象数据都会存储在 Drive 里。**
- **Set——一组 Drive 的集合，分布式部署根据集群规模自动划分一个或多个 Set ，每个 Set 中的 Drive 分布在不同位置。**
   - **一个对象存储在一个 Set 上**
   - **一个集群划分为多个 Set**
   - **一个 Set 包含的 Drive 数量是固定的，默认由系统根据集群规模自动计算得出**
   - **一个 SET 中的 Drive 尽可能分布在不同的节点上**

**Set /Drive 的关系**

- **Set /Drive 这两个概念是 MINIO 里面最重要的两个概念，一个对象最终是存储在 Set 上面的。**
- **Set 是另外一个概念，Set 是一组 Drive 的集合，图中，所有蓝色、橙色背景的 Drive（硬盘）的就组成了一个 Set。**

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661494934904-50275857-a6bb-430b-95ee-fc0a02ec6fc1.png#clientId=uda22a1be-5bb3-4&from=paste&id=u8e466c7c&originHeight=687&originWidth=932&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4af2be7d-5e90-49a6-8750-94812f6da8f&title=)
<a name="fCuyU"></a>
## **三、纠删码（Erasure Code）**
纠删码（Erasure Code）简称 EC，是一种数据保护方法，它将数据分割成片段，把冗余数据块扩展、编码，并将其存储在不同的位置，比如磁盘、存储节点或者其它地理位置。

- **纠删码是一种恢复丢失和损坏数据的数学算法，目前，纠删码技术在分布式存储系统中的应用主要有三类，阵列纠删码（Array Code: RAID5、RAID6 等）、RS(Reed-Solomon)里德-所罗门类纠删码和LDPC(LowDensity Parity Check Code)低密度奇偶校验纠删码。**
- **Erasure Code 是一种编码技术，它可以将 n 份原始数据，增加 m 份校验数据，并能通过 n+m 份中的任意 n 份原始数据，还原为原始数据。**
- **即如果有任意小于等于 m 份的校验数据失效，仍然能通过剩下的数据还原出来。**
- **Minio 采用 Reed-Solomon code 将对象拆分成 N/2 数据和 N/2 奇偶校验块。**
- **在同一集群内，MinIO 自己会自动生成若干纠删组（Set），用于分布存放桶数据。一个纠删组中的一定数量的磁盘发生的故障（故障磁盘的数量小于等于校验盘的数量），通过纠删码校验算法可以恢复出正确的数据。**
<a name="E7Ak1"></a>
## **四、MinIO**
<a name="MXc5R"></a>
### **1）单主机，单硬盘模式**
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661494934919-23948c65-dbab-4bde-aaf3-3fd83f5ff792.png#clientId=uda22a1be-5bb3-4&from=paste&id=u5cbbdab9&originHeight=924&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u87731b01-7904-4f62-a835-91d2e467458&title=)<br />该模式下，Minio 只在一台服务器上搭建服务，且数据都存在单块磁盘上，该模式存在单点风险，主要用作开发、测试等使用
<a name="UurES"></a>
### **2）单主机，多硬盘模式**
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661494934904-fbe47652-7dc3-458f-b39c-b6d6c82cdec5.png#clientId=uda22a1be-5bb3-4&from=paste&id=u086836d0&originHeight=982&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u43598c15-572f-4895-85ee-1d06e3502a3&title=)<br />该模式下，Minio 在一台服务器上搭建服务，但数据分散在多块（大于 4 块）磁盘上，提供了数据上的安全保障。
<a name="Qko5j"></a>
### **3）多主机、多硬盘模式（分布式）**
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661494934958-171949a2-3bb0-4727-a6e6-e1496cc16c81.png#clientId=uda22a1be-5bb3-4&from=paste&id=u99e9257a&originHeight=892&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua8e10414-2f1e-484e-9861-be887028bcc&title=)<br />该模式是 Minio 服务最常用的架构，通过共享一个 access_key 和 secret_key,在多台服务器上搭建服务，且数据分散在多块（大于 4 块，无上限）磁盘上，提供了较为强大的数据冗余机制（Reed-Solomon 纠删码）。
<a name="UzUfv"></a>
## **五、MinIO 环境部署（分布式）**
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661494935687-8790ca71-a68e-4466-82fe-e855a30195ac.png#clientId=uda22a1be-5bb3-4&from=paste&id=u2cb6e5ab&originHeight=646&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ucd13ccb5-b7b9-4115-a180-89f209358c0&title=)
<a name="oQQK5"></a>
### **1）环境准备**
```
|        主机名      |       IP        |                  data                   |
|-------------------|-----------------|-----------------------------------------|
| local-168-182-110 | 192.168.182.110 | /opt/bigdata/minio/data/export{1,2,3,4} |
| local-168-182-111 | 192.168.182.111 | /opt/bigdata/minio/data/export{1,2,3,4} |
| local-168-182-112 | 192.168.182.112 | /opt/bigdata/minio/data/export{1,2,3,4} |
```
<a name="k7uSu"></a>
### **2）下载**
```bash
mkdir -p /opt/bigdata/minio ; cd /opt/bigdata/minio
# 下载rpm包进行部署
# wget https://dl.min.io/server/minio/release/linux-amd64/archive/minio-20220802235916.0.0.x86_64.rpm -O minio.rpm
# yum -y install minio.rpm

# 下载二进制包部署
wget https://dl.min.io/server/minio/release/linux-amd64/minio
chmod +x /opt/bigdata/minio
# 加在/etc/profile
export PATH=$PATH:/opt/bigdata/minio

minio --help
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661494935764-afb98b11-df75-4219-af57-812fcd35592b.png#clientId=uda22a1be-5bb3-4&from=paste&id=u0963367b&originHeight=465&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u138a1dda-be3b-4451-9205-6a1980b4882&title=)
<a name="nLpFW"></a>
### **3）每台新加四块磁盘**
```bash
# 不重启，直接刷新磁盘数据总线，获取新加的磁盘
for host in $(ls /sys/class/scsi_host) ; do echo "- - -" > /sys/class/scsi_host/$host/scan; done

lsblk

# 格式化
mkfs.ext4 /dev/sdb
mkfs.ext4 /dev/sdc
mkfs.ext4 /dev/sdd
mkfs.ext4 /dev/sde

# 挂载
mount /dev/sdb /opt/bigdata/minio/data/export1
mount /dev/sdc /opt/bigdata/minio/data/export2
mount /dev/sdd /opt/bigdata/minio/data/export3
mount /dev/sde /opt/bigdata/minio/data/export4
```
【温馨提示】磁盘大小必须>1G，这里添加的是 4*2G 的盘
<a name="seHju"></a>
### **4）配置**
Minio 默认9000端口，在配置文件中加入–address “127.0.0.1:9029” 可更改端口

- `**MINIO_ACCESS_KEY**`**：用户名，长度最小是 5 个字符**
- `**MINIO_SECRET_KEY**`**：密码，密码不能设置过于简单，不然 minio 会启动失败，长度最小是 8 个字符**
- `**–config-dir**`**：指定集群配置文件目录**
- `**–address**`**：api 的端口，默认是9000**
- `**--console-address**`** ：web 端口，默认随机**

编写启动脚本（/opt/bigdata/minio/run.sh）
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

# 在三台机器上都执行该文件，即以分布式的方式启动了MINIO
# --address "0.0.0.0:9000" 挂载9001端口为api端口（如Java客户端）访问的端口
# --console-address ":9000" 挂载9000端口为web端口；
/opt/bigdata/minio/minio server --address 0.0.0.0:9000 --console-address 0.0.0.0:9001 --config-dir /etc/minio \
http://local-168-182-110/opt/bigdata/minio/data/export1 \
http://local-168-182-110/opt/bigdata/minio/data/export2 \
http://local-168-182-110/opt/bigdata/minio/data/export3 \
http://local-168-182-110/opt/bigdata/minio/data/export4 \
http://local-168-182-111/opt/bigdata/minio/data/export1 \
http://local-168-182-111/opt/bigdata/minio/data/export2 \
http://local-168-182-111/opt/bigdata/minio/data/export3 \
http://local-168-182-111/opt/bigdata/minio/data/export4 \
http://local-168-182-112/opt/bigdata/minio/data/export1 \
http://local-168-182-112/opt/bigdata/minio/data/export2 \
http://local-168-182-112/opt/bigdata/minio/data/export3 \
http://local-168-182-112/opt/bigdata/minio/data/export4 > /opt/bigdata/minio/logs/minio_server.log
```
【温馨提示】下面脚本复制时 \ 后不要有空格，还有就是上面的目录是对应的一块磁盘，而非简单的在/opt/bigdata/minio/data 目录下创建四个目录，要不然会报如下错误，看提示以为是 root 权限问题。part of root disk, will not be used (*errors.errorString)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661494936424-a84ad3c3-7e50-495f-8013-6d308cb63ea6.png#clientId=uda22a1be-5bb3-4&from=paste&id=u44d4ae7e&originHeight=590&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2b385b2f-deeb-4026-b002-d39ff164318&title=)
<a name="lUIGV"></a>
### **5）启动服务**
```bash
# 在三台机器上都执行该文件，即以分布式的方式启动了MINIO
sh /opt/bigdata/minio/run.sh
```
添加或修改 minio.service，通过 systemctl 启停服务（推荐）

- **WorkingDirectory：二进制文件目录**
- **ExecStart：指定集群启动脚本**
```bash
# 如果使用rpm安装，minio.service就会自动生成，只要修改就行
cat > /usr/lib/systemd/system/minio.service <<EOF
[Unit]
Description=Minio service
Documentation=https://docs.minio.io/

[Service]
WorkingDirectory=/opt/bigdata/minio
ExecStart=/opt/bigdata/minio/run.sh

Restart=on-failure
RestartSec=5

[Install]
WantedBy=multi-user.target
EOF
```
修改文件权限
```bash
chmod +x /usr/lib/systemd/system/minio.service && chmod +x /opt/bigdata/minio/minio && chmod +x /opt/bigdata/minio/run.sh

# 将文件copy其它节点local-168-182-111，local-168-182-112
scp -r /usr/lib/systemd/system/minio.servicee local-168-182-111:/usr/lib/systemd/system/minio.service
scp -r /opt/bigdata/minio local-168-182-111:/opt/bigdata/

scp -r /usr/lib/systemd/system/minio.service local-168-182-112:/usr/lib/systemd/system/minio.service
scp -r /opt/bigdata/minio local-168-182-112:/opt/bigdata/
```
启动集群
```bash
 #重新加载服务
systemctl daemon-reload
#启动服务
systemctl start minio
#加入自启动
systemctl enable minio
```
[![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661494936152-d2029052-3843-4ffc-a718-45fbb5af6c98.png#clientId=uda22a1be-5bb3-4&from=paste&id=u92b54ec5&originHeight=615&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u746fc63b-47c9-4db0-a22f-efaec44d66a&title=)](http://mp.weixin.qq.com/s?__biz=MjM5MzU5NDYwNA==&mid=2247499085&idx=1&sn=4a38434ad7d8776fd5b76646f94e5251&chksm=a69610ce91e199d81389da2e575b4f28427e5c1032f62e11b4e7cca1f34f00475b0da91384a8&scene=21#wechat_redirect)<br />访问 MinIO，三个节点都可以访问<br />http://local-168-182-110:9001/<br />http://local-168-182-111:9001/<br />http://local-168-182-112:9001/<br />账号密码：admin/admin123456<br />[![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661494936265-84af8d37-13ab-4e32-aa07-75e9ca7c6160.png#clientId=uda22a1be-5bb3-4&from=paste&id=u41b43cd7&originHeight=573&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u46aea3ab-3ea3-47b8-909d-1f442d6e519&title=)](http://mp.weixin.qq.com/s?__biz=MjM5MzU5NDYwNA==&mid=2247500578&idx=1&sn=4b56ad52e9f19f5c3dd955eeeb08a092&chksm=a6962ea191e1a7b7c476fbe4c45827441d6a47a37a5830a45ad57853e588447bab6cb50a97de&scene=21#wechat_redirect)
<a name="A1irr"></a>
### **6）使用 nginx 负载均衡**
单独对每个节点进行访问显然不合理，通过使用 nginx 代理，进行负载均衡则很有必要。简单的配置如下：
```bash
# 安装nginx
yum install epel-release -y
yum install nginx -y
systemctl start nginx
systemctl status nginx
systemctl enable nginx
```
添加配置文件，配置内容如下：
```nginx
vim  /etc/nginx/conf.d/minio.conf

upstream minio_api {
    server 192.168.182.110:9000;
    server 192.168.182.111:9000;
    server 192.168.182.112:9000;
}

upstream minio_console {
    server 192.168.182.110:9001;
    server 192.168.182.111:9001;
    server 192.168.182.112:9001;
}

server{
    listen       19000;
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

        proxy_pass http://minio_api;
    }
}

server{
    listen       19001;
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

        proxy_pass http://minio_console;
    }
}
```
重启加载配置
```bash
nginx -t
nginx -s reload
#或者
systemctl reload nginx
```
访问：http://local-168-182-110:19001<br />[![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661494936285-4a0fab64-be83-4040-b71b-f0c20ca5ab0b.png#clientId=uda22a1be-5bb3-4&from=paste&id=ud0198d20&originHeight=616&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub0dc8727-b033-4219-afa0-e7d2b1be414&title=)](http://mp.weixin.qq.com/s?__biz=MjM5MzU5NDYwNA==&mid=2247500868&idx=1&sn=aeddb4cd77c8de39271558daf62b36ba&chksm=a69629c791e1a0d193c370e6d5be473ecd54e784140690920adeb5756a9b82a9ff9f7bd79d05&scene=21#wechat_redirect)
<a name="cLnkg"></a>
## **六、MinIO 客户端 ( mc)**

- **MinIO Client mc 命令行工具为 UNIX 命令（如 ls、cat、cp、mirror 和）提供了一种现代替代方案，并 diff 支持文件系统和兼容 Amazon S3 的云存储服务。**
- **mc 命令行工具是为与 AWS S3 API 兼容而构建的，并针对预期的功能和行为测试了 MinIO 和 AWS S3。**
- **MinIO 不为其他与 S3 兼容的服务提供任何保证，因为它们的 S3 API 实现是未知的，因此不受支持。虽然 mc 命令可以按文档说明工作，但任何此类使用都需要自担风险。**
<a name="EsGc8"></a>
### **1）下载**
```bash
cd /opt/bigdata/minio/
wget https://dl.min.io/client/mc/release/linux-amd64/mc
chmod +x mc
./mc --help
```
<a name="VqcTC"></a>
### **2）添加 MinIO 存储服务**
MinIO 服务器显示 URL，访问权和秘密密钥。<br />【用法】
```bash
mc config host add <ALIAS> <YOUR-MINIO-ENDPOINT> [YOUR-ACCESS-KEY] [YOUR-SECRET-KEY]
```
【示例】
```bash
cd /opt/bigdata/minio/
# 明文输入
./mc config host add minio http://local-168-182-110:19000 admin admin123456

# 密文输入（推荐）
./mc config host add minio http://local-168-182-110:19000
Enter Access Key: admin
Enter Secret Key: admin123456
```
<a name="lSxLT"></a>
### **3）测试**
```bash
cd /opt/bigdata/minio/
# 获取已配置别名“ minio”的MinIO服务器信息
./mc admin info minio

# 添加外壳别名以获取信息，以便恢复。
alias minfo='/opt/bigdata/minio/mc admin info'
alias mheal='/opt/bigdata/minio/mc admin heal'
```
[![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661494936754-3665d830-5f51-40f8-bffa-e2497005154d.png#clientId=uda22a1be-5bb3-4&from=paste&id=ufa69853b&originHeight=633&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uaf5f279d-1dbe-40fa-909b-41632d6a623&title=)](http://mp.weixin.qq.com/s?__biz=MjM5MzU5NDYwNA==&mid=2247500576&idx=2&sn=3e15321e3e003c94611e8598c2e40952&chksm=a6962ea391e1a7b5ae738441de5da14aa26355e8b1d9b841f8f52d9bd73e76777e05f714a249&scene=21#wechat_redirect)<br />更多示例操作，可以参考官方文档：[http://docs.minio.org.cn/docs/master/minio-admin-complete-guide](http://docs.minio.org.cn/docs/master/minio-admin-complete-guide)<br />【温馨提示】如果有条件，也可以使用腾讯云的cos（Cloud Object Storage：云对象存储）和阿里云的oss（Object Storage Service：对象存储服务）等公有云产品。
