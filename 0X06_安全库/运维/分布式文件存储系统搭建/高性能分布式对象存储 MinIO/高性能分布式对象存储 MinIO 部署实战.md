MinIO
<a name="iq3sl"></a>
## 概述
MinIO 是在 GNU Affero 通用公共许可证 v3.0 下发布的高性能对象存储。它与 Amazon S3 云存储服务 API 兼容。使用 MinIO 为机器学习、分析和应用程序数据工作负载构建高性能基础架构。

- 官方文档：[https://docs.min.io/](https://docs.min.io/)
- 中文文档：[http://docs.minio.org.cn/docs/](http://docs.minio.org.cn/docs/)
- GitHub地址：[https://github.com/minio/minio](https://github.com/minio/minio)
<a name="mUWyD"></a>
### 特点
**数据保护**——分布式Minio采用 纠删码来防范多个节点宕机和位衰减bit rot。分布式Minio至少需要4个硬盘，使用分布式Minio自动引入了纠删码功能。<br />**高可用**——单机Minio服务存在单点故障，相反，如果是一个有N块硬盘的分布式Minio，只要有N/2硬盘在线，数据就是安全的。不过需要至少有N/2+1个硬盘来创建新的对象。<br />例如，一个16节点的Minio集群，每个节点16块硬盘，就算8台服務器宕机，这个集群仍然是可读的，不过需要9台服務器才能写数据。<br />【温馨提示】只要遵守分布式Minio的限制，可以组合不同的节点和每个节点几块硬盘。比如，可以使用2个节点，每个节点4块硬盘，也可以使用4个节点，每个节点两块硬盘，诸如此类。<br />**一致性**——Minio在分布式和单机模式下，所有读写操作都严格遵守read-after-write一致性模型。
<a name="g30UX"></a>
### MinIO的优点如下

- **部署简单**，一个二进制文件（minio）即是一切，还可以支持各种平台
- **支持海量存储**，可以按zone扩展，支持单个对象最大5TB
- **低冗余且磁盘损坏高容忍**，标准且最高的数据冗余系数为2(即存储一个1M的数据对象，实际占用磁盘空间为2M)。但在任意n/2块disk损坏的情况下依然可以读出数据(n为一个纠删码集合中的disk数量)。并且这种损坏恢复是基于单个对象的，而不是基于整个存储卷的
- **读写性能优异**
<a name="t3VtC"></a>
## MinIO 基础概念

- **S3——Simple Storage Service**，简单存储服务，这个概念是Amazon在2006年推出的，对象存储就是从那个时候诞生的。S3提供了一个简单Web服务接口，可用于随时在Web上的任何位置存储和检索任何数量的数据。
- **Object——存储到 Minio 的基本对象**，如文件、字节流，Anything...
- **Bucket——用来存储 Object 的逻辑空间**。每个 Bucket 之间的数据是相互隔离的。
- **Drive——部署 Minio 时设置的磁盘**，Minio 中所有的对象数据都会存储在 Drive 里。
- **Set——一组 Drive 的集合**，分布式部署根据集群规模自动划分一个或多个 Set ，每个 Set 中的 Drive 分布在不同位置。
   - 一个对象存储在一个Set上
   - 一个集群划分为多个Set
   - 一个Set包含的Drive数量是固定的，默认由系统根据集群规模自动计算得出
   - 一个SET中的Drive尽可能分布在不同的节点上
<a name="kPsEL"></a>
### Set /Drive 的关系

- Set /Drive 这两个概念是 MINIO 里面最重要的两个概念，一个对象最终是存储在 Set 上面的。
- Set 是另外一个概念，Set 是一组 Drive 的集合，图中，所有蓝色、橙色背景的Drive（硬盘）的就组成了一个 Set。

![DM_20221208100316_001.PNG](https://cdn.nlark.com/yuque/0/2022/png/396745/1670466137931-48e047c6-b09f-458d-9c8d-9d4dbbb1f316.png#averageHue=%23f8f7f6&clientId=ub8d18b02-3fb5-4&from=ui&id=u3002cbef&originHeight=687&originWidth=932&originalType=binary&ratio=1&rotation=0&showTitle=false&size=46636&status=done&style=none&taskId=ucceab400-cc0b-4a41-b997-0432c4b21a1&title=)
<a name="Y8LvZ"></a>
## 纠删码（Erasure Code）
**纠删码（Erasure Code）简称EC**，是一种数据保护方法，它将数据分割成片段，把冗余数据块扩展、编码，并将其存储在不同的位置，比如磁盘、存储节点或者其它地理位置。<br />纠删码是一种恢复丢失和损坏数据的数学算法，目前，纠删码技术在分布式存储系统中的应用主要有三类，阵列纠删码（Array Code: RAID5、RAID6等）、RS(Reed-Solomon)里德-所罗门类纠删码和LDPC(LowDensity Parity Check Code)低密度奇偶校验纠删码。<br />Erasure Code是一种编码技术，它可以将n份原始数据，增加m份校验数据，并能通过n+m份中的任意n份原始数据，还原为原始数据。<br />即如果有任意小于等于m份的校验数据失效，仍然能通过剩下的数据还原出来。<br />Minio采用Reed-Solomon code将对象拆分成N/2数据和N/2 **奇偶校验**块。<br />在同一集群内，MinIO 自己会自动生成若干纠删组（Set），用于分布存放桶数据。一个纠删组中的一定数量的磁盘发生的故障（故障磁盘的数量小于等于校验盘的数量），通过纠删码校验算法可以恢复出正确的数据。
<a name="MKBtQ"></a>
## MinIO 架构
<a name="kAqTE"></a>
### 单主机，单硬盘模式
![DM_20221208100316_002.PNG](https://cdn.nlark.com/yuque/0/2022/png/396745/1670466138324-454ecba9-0af3-4963-b467-51323de6ad17.png#averageHue=%23f3eae9&clientId=ub8d18b02-3fb5-4&from=ui&id=MhyqJ&originHeight=980&originWidth=1146&originalType=binary&ratio=1&rotation=0&showTitle=false&size=62008&status=done&style=none&taskId=u62fe5aa0-1c10-4b38-a3a1-aaa2c157ef3&title=)该模式下，Minio只在一台服务器上搭建服务，且数据都存在单块磁盘上，该模式存在单点风险，主要用作开发、测试等使用
<a name="KHyE1"></a>
### 单主机，多硬盘模式
![DM_20221208100316_003.PNG](https://cdn.nlark.com/yuque/0/2022/png/396745/1670466166296-feef6b14-273e-4c2d-a3bf-bfc4e6eb81fc.png#averageHue=%23f4ebe9&clientId=ub8d18b02-3fb5-4&from=ui&id=ub862585a&originHeight=1042&originWidth=1146&originalType=binary&ratio=1&rotation=0&showTitle=false&size=93164&status=done&style=none&taskId=u75bf8c81-e145-40f8-8b7a-2c60db75ed0&title=)<br />该模式下，Minio在一台服务器上搭建服务，但数据分散在多块（大于4块）磁盘上，提供了数据上的安全保障。
<a name="NNhqg"></a>
### 多主机、多硬盘模式（分布式）
![DM_20221208100316_004.PNG](https://cdn.nlark.com/yuque/0/2022/png/396745/1670466166252-d2197bc2-080d-481a-a3cc-72b1b31b2069.png#averageHue=%23f1e6e4&clientId=ub8d18b02-3fb5-4&from=ui&id=cOPaV&originHeight=1721&originWidth=2084&originalType=binary&ratio=1&rotation=0&showTitle=false&size=194493&status=done&style=none&taskId=uc179ecdd-a3a4-4375-b9c4-0f49d1e156b&title=)该模式是Minio服务最常用的架构，通过共享一个access_key和secret_key,在多台服务器上搭建服务，且数据分散在多块（大于4块，无上限）磁盘上，提供了较为强大的数据冗余机制（Reed-Solomon纠删码）。
<a name="Kkt3r"></a>
## MinIO 环境部署（分布式）
![DM_20221208100316_005.PNG](https://cdn.nlark.com/yuque/0/2022/png/396745/1670466197082-20221759-9b54-4629-9051-49b90d39e2f6.png#averageHue=%23122833&clientId=ub8d18b02-3fb5-4&from=ui&id=u44e486d3&originHeight=754&originWidth=1261&originalType=binary&ratio=1&rotation=0&showTitle=false&size=71009&status=done&style=none&taskId=u0bac2279-3cab-47e4-87d5-4ca6f07cd7a&title=)
<a name="XTulw"></a>
### 环境准备
| 主机名 | IP | data |
| --- | --- | --- |
| local-168-182-110 | 192.168.182.110 | /opt/bigdata/minio/data/export{1,2,3,4} |
| local-168-182-111 | 192.168.182.111 | /opt/bigdata/minio/data/export{1,2,3,4} |
| local-168-182-112 | 192.168.182.112 | /opt/bigdata/minio/data/export{1,2,3,4} |

<a name="pElKz"></a>
### 下载
```bash
mkdir -p /opt/bigdata/minio ; cd /opt/bigdata/minio
# 下载rpm包进行部署
#wget https://dl.min.io/server/minio/release/linux-amd64/archive/minio-20220802235916.0.0.x86_64.rpm -O minio.rpm
# yum -y install minio.rpm

# 下载二进制包部署
wget https://dl.min.io/server/minio/release/linux-amd64/minio
chmod +x /opt/bigdata/minio
# 加在/etc/profile
export PATH=$PATH:/opt/bigdata/minio

minio --help
```
![DM_20221208100316_006.PNG](https://cdn.nlark.com/yuque/0/2022/png/396745/1670466612725-a941ec63-d823-4546-a8c0-5dd2e62ecd34.png#averageHue=%23201f1e&clientId=ub8d18b02-3fb5-4&from=ui&id=u64fff4a4&originHeight=531&originWidth=1233&originalType=binary&ratio=1&rotation=0&showTitle=false&size=52590&status=done&style=none&taskId=u2651a116-dd49-4ad0-957c-e53495cddf2&title=)
<a name="pZ6Kh"></a>
### 每台新加四块磁盘
```bash
# 不重启，直接刷新磁盘数据总线，获取新加的磁盘
for host in $(ls /sys/class/scsi_host) ; do echo "- - -" > /sys/class/scsi_host/$host/scan; done

lsblk

# 格式化
mkfs.ext4 /dev/sdb
mkfs.ext4 /dev/sdc
mkfs.ext4 /dev/sdd
mkfs.ext4 /dev/sde

# 挂载
mount /dev/sdb /opt/bigdata/minio/data/export1
mount /dev/sdc /opt/bigdata/minio/data/export2
mount /dev/sdd /opt/bigdata/minio/data/export3
mount /dev/sde /opt/bigdata/minio/data/export4
```
【**温馨提示**】磁盘大小必须>1G，这里添加的是4*2G的盘
<a name="fTjgt"></a>
### 配置
Minio默认9000端口，在配置文件中加入`–address "127.0.0.1:9029"` 可更改端口
```
MINIO_ACCESS_KEY：#用户名，长度最小是5个字符
MINIO_SECRET_KEY：#密码，密码不能设置过于简单，不然minio会启动失败，长度最小是8个字符
–config-dir：#指定集群配置文件目录
–address： #api的端口，默认是9000
--console-address ：#web端口，默认随机
```
编写启动脚本（/opt/bigdata/minio/run.sh）
```bash
#!/bin/bash
# 创建日志存储目录
mkdir -p /opt/bigdata/minio/logs
# 分别在三个节点上创建存储目录
mkdir -p /opt/bigdata/minio/data/export{1,2,3,4}
# 创建配置目录
mkdir -p /etc/minio
export MINIO_ROOT_USER=admin
export MINIO_ROOT_PASSWORD=admin123456

# 在三台机器上都执行该文件，即以分布式的方式启动了MINIO
# --address "0.0.0.0:9000" 挂载9001端口为api端口（如Java客户端）访问的端口
# --console-address ":9000" 挂载9000端口为web端口； 
/opt/bigdata/minio/minio server --address 0.0.0.0:9000 --console-address 0.0.0.0:9001 --config-dir /etc/minio \
http://local-168-182-110/opt/bigdata/minio/data/export1 \
http://local-168-182-110/opt/bigdata/minio/data/export2 \
http://local-168-182-110/opt/bigdata/minio/data/export3 \
http://local-168-182-110/opt/bigdata/minio/data/export4 \
http://local-168-182-111/opt/bigdata/minio/data/export1 \
http://local-168-182-111/opt/bigdata/minio/data/export2 \
http://local-168-182-111/opt/bigdata/minio/data/export3 \
http://local-168-182-111/opt/bigdata/minio/data/export4 \
http://local-168-182-112/opt/bigdata/minio/data/export1 \
http://local-168-182-112/opt/bigdata/minio/data/export2 \
http://local-168-182-112/opt/bigdata/minio/data/export3 \
http://local-168-182-112/opt/bigdata/minio/data/export4 > /opt/bigdata/minio/logs/minio_server.log
```
【**温馨提示**】下面脚本复制时 \ 后不要有空格，还有就是上面的目录是对应的一块磁盘，而非简单的在/opt/bigdata/minio/data目录下创建四个目录，要不然会报如下错误，看提示以为是root权限问题。part of root disk, will not be used (*errors.errorString)<br />![DM_20221208100316_007.PNG](https://cdn.nlark.com/yuque/0/2022/png/396745/1670466612727-20025032-e31a-49c7-8cae-c32f7c7458ce.png#averageHue=%2323201f&clientId=ub8d18b02-3fb5-4&from=ui&id=kkY8i&originHeight=764&originWidth=1398&originalType=binary&ratio=1&rotation=0&showTitle=false&size=112531&status=done&style=none&taskId=u2ff56297-6ba3-408c-a26d-367d1bd9389&title=)
<a name="Hl0Ew"></a>
### 启动服务
```bash
# 在三台机器上都执行该文件，即以分布式的方式启动了MINIO
sh /opt/bigdata/minio/run.sh
```
添加或修改minio.service，通过systemctl启停服务（推荐）

- WorkingDirectory：二进制文件目录
- ExecStart：指定集群启动脚本
```bash
# 如果使用rpm安装，minio.service就会自动生成，只要修改就行
cat > /usr/lib/systemd/system/minio.service <<EOF
[Unit]
Description=Minio service
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
<a name="EDFBO"></a>
### 修改文件权限
```bash
chmod +x /usr/lib/systemd/system/minio.service && chmod +x /opt/bigdata/minio/minio && chmod +x /opt/bigdata/minio/run.sh

# 将文件copy其它节点local-168-182-111，local-168-182-112
scp -r /usr/lib/systemd/system/minio.servicee local-168-182-111:/usr/lib/systemd/system/minio.service
scp -r /opt/bigdata/minio local-168-182-111:/opt/bigdata/

scp -r /usr/lib/systemd/system/minio.service local-168-182-112:/usr/lib/systemd/system/minio.service
scp -r /opt/bigdata/minio local-168-182-112:/opt/bigdata/
```
<a name="Ykxvl"></a>
### 启动集群
```bash
#重新加载服务
systemctl daemon-reload
#启动服务
systemctl start minio
#加入自启动
systemctl enable minio
```
![DM_20221208100316_008.PNG](https://cdn.nlark.com/yuque/0/2022/png/396745/1670466667703-54a8e819-501c-46eb-a303-a839ccb70101.png#averageHue=%23747271&clientId=ub8d18b02-3fb5-4&from=ui&id=u665bd15e&originHeight=931&originWidth=1636&originalType=binary&ratio=1&rotation=0&showTitle=false&size=125140&status=done&style=none&taskId=ue1a9551d-4ccd-4dc6-83f1-a23e0a981ed&title=)
<a name="eTz6V"></a>
###### 访问MinIO，三个节点都可以访问

- http://local-168-182-110:9001
- http://local-168-182-111:9001
- http://local-168-182-112:9001

账号密码：admin/admin123456![DM_20221208100316_009.PNG](https://cdn.nlark.com/yuque/0/2022/png/396745/1670466667730-f8bfedac-0df5-48d9-a048-669e8c4d1a28.png#averageHue=%23e9cb90&clientId=ub8d18b02-3fb5-4&from=ui&id=WR81w&originHeight=899&originWidth=1695&originalType=binary&ratio=1&rotation=0&showTitle=false&size=252996&status=done&style=none&taskId=u90342a13-d805-49e1-9e84-a4d3220985b&title=)
<a name="RQ2rs"></a>
### 使用 Nginx 负载均衡
单独对每个节点进行访问显然不合理，通过使用 Nginx 代理，进行负载均衡则很有必要。简单的配置如下：
```bash
# 安装nginx
yum install epel-release -y
yum install nginx -y
systemctl start nginx
systemctl status nginx
systemctl enable nginx
```
添加配置文件，配置内容如下：
```bash
vi  /etc/nginx/conf.d/minio.conf
```
```nginx
upstream minio_api {
  server 192.168.182.110:9000;
  server 192.168.182.111:9000;
  server 192.168.182.112:9000;
}

upstream minio_console {
  server 192.168.182.110:9001;
  server 192.168.182.111:9001;
  server 192.168.182.112:9001;
}

server{
  listen       19000;
  server_name  192.168.182.110;

  ignore_invalid_headers off;
  client_max_body_size 0;
  proxy_buffering off;

  location / {
    proxy_set_header   X-Forwarded-Proto $scheme;
    proxy_set_header   Host              $http_host;
    proxy_set_header   X-Real-IP         $remote_addr;

    proxy_connect_timeout 300;
    proxy_http_version 1.1;
    chunked_transfer_encoding off;
    proxy_ignore_client_abort on;

    proxy_pass http://minio_api;
  }
}

server{
  listen       19001;
  server_name  192.168.182.110;

  ignore_invalid_headers off;
  client_max_body_size 0;
  proxy_buffering off;

  location / {
    proxy_set_header   X-Forwarded-Proto $scheme;
    proxy_set_header   Host              $http_host;
    proxy_set_header   X-Real-IP         $remote_addr;

    proxy_connect_timeout 300;
    proxy_http_version 1.1;
    chunked_transfer_encoding off;
    proxy_ignore_client_abort on;

    proxy_pass http://minio_console;
  }
}
```
重启加载配置
```bash
nginx -t
nginx -s reload
#或者 
systemctl reload nginx
```
访问：http://local-168-182-110:19001<br />![DM_20221208100316_010.PNG](https://cdn.nlark.com/yuque/0/2022/png/396745/1670466707509-3122448a-1ed0-42c1-8a97-a763689d94f4.png#averageHue=%23e8ca8f&clientId=ub8d18b02-3fb5-4&from=ui&id=u2b7b936d&originHeight=916&originWidth=1605&originalType=binary&ratio=1&rotation=0&showTitle=false&size=253171&status=done&style=none&taskId=u976129e4-557c-4af3-8312-c8badea2057&title=)
<a name="ZPfRe"></a>
## MinIO 客户端 ( mc)
MinIO Client mc命令行工具为 UNIX 命令（如ls、cat、cp、mirror和）提供了一种现代替代方案，并diff支持文件系统和兼容 Amazon S3 的云存储服务。<br />mc命令行工具是为与 AWS S3 API 兼容而构建的，并针对预期的功能和行为测试了 MinIO 和 AWS S3。<br />MinIO 不为其他与 S3 兼容的服务提供任何保证，因为它们的 S3 API 实现是未知的，因此不受支持。虽然mc 命令可以按文档说明工作，但任何此类使用都需要自担风险。
<a name="rapoc"></a>
### 下载
```bash
cd /opt/bigdata/minio/
wget https://dl.min.io/client/mc/release/linux-amd64/mc
chmod +x mc
./mc --help
```
<a name="VRB60"></a>
### 添加MinIO存储服务
MinIO服务器显示URL，访问权和秘密密钥。<br />【用法】
```bash
mc config host add <ALIAS> <YOUR-MINIO-ENDPOINT> [YOUR-ACCESS-KEY] [YOUR-SECRET-KEY]
```
【示例】
```bash
cd /opt/bigdata/minio/
# 明文输入
./mc config host add minio http://local-168-182-110:19000 admin admin123456

# 密文输入（推荐）
./mc config host add minio http://local-168-182-110:19000
Enter Access Key: admin
Enter Secret Key: admin123456
```
<a name="lD1IZ"></a>
### 测试
```bash
cd /opt/bigdata/minio/
# 获取已配置别名“ minio”的MinIO服务器信息
./mc admin info minio

# 添加外壳别名以获取信息，以便恢复。
alias minfo='/opt/bigdata/minio/mc admin info'
alias mheal='/opt/bigdata/minio/mc admin heal'
```
![DM_20221208100316_011.PNG](https://cdn.nlark.com/yuque/0/2022/png/396745/1670466707478-0df84558-debc-4889-a2ab-e1c840551da1.png#averageHue=%231f1e1d&clientId=ub8d18b02-3fb5-4&from=ui&id=Ar8Rt&originHeight=706&originWidth=1204&originalType=binary&ratio=1&rotation=0&showTitle=false&size=64150&status=done&style=none&taskId=uac585431-d7c4-42b1-a55e-5821a1de60c&title=)更多示例操作，可以参考官方文档：[http://docs.minio.org.cn/docs/master/minio-admin-complete-guide](http://docs.minio.org.cn/docs/master/minio-admin-complete-guide)<br />【温馨提示】如果有条件，也可以使用腾讯云的cos（Cloud Object Storage：云对象存储）和阿里云的oss（Object Storage Service：对象存储服务）等公有云产品。
