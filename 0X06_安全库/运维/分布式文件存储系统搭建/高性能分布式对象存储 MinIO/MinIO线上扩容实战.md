MinIO<br />硬件投入肯定是随着业务的增长而增长，这就要求中间件平台必须提供水平伸缩机制，MinIO对象存储服务也不例外，本文就详细介绍MinIO的扩容。<br />Minio支持通过增加新的Server Pool来扩容老的集群。每个Server Pool都是一个相对独立的故障域，一个Server Pool丢失一个或多个驱动器或节点不会影响其他Server Pool的可用性。
<a name="rtwoT"></a>
## 1、扩容前的先决条件
<a name="gGcZV"></a>
### 1.1 网络与防火墙
集群内各个节点之间网络必须是互通的，特别是要注意防火墙规则或者容器环境下的网络互通，例如可以通过如下命令放开Minio的默认Server Port:9000。
```bash
firewall-cmd --permanent --zone=public --add-port=9000/tcp
firewall-cmd --reload
```
注意：minio集群中所有节点必须是监听同一个端口。<br />如果您设置了一个静态MinIO控制台端口(例如:9001)，您还必须授予对该端口的访问权限，以确保来自外部客户端的连接。MinIO强烈建议使用**负载均衡组件(Nginx)**来管理到集群的连接，负载均衡器应该使用“最少连接”算法将请求路由到MinIO部署，因为部署中的任何MinIO节点都可以接收、路由或处理客户端请求。
<a name="cyOQj"></a>
### 1.2 连续的主机名称
MinIO需要使用展开符号{x…y}来表示创建服务器池时的一系列MinIO主机。因此，MinIO需要使用按顺序编号的主机名来表示池中的每个MinIO服务器进程。<br />例如现在有minio5.example.com、minio6.example.com两个节点，可以使用 minio{5...6}.example.com来指定Server Pool中的所有节点。
<a name="FbaAB"></a>
### 1.3 本机JBOD存储与顺序挂载
**MinIO强烈推荐直连的JBOD阵列，xfs格式的磁盘以获得最佳性能。**<br />DAS (Direct-Attached Storage)与网络存储(NAS、SAN、NFS)相比具有显著的性能和一致性优势。<br />使用非xfs文件系统(ext4、btrfs、zfs)的部署往往具有较低的性能，同时ext4等文件存储对单个目录下的文件有个数限制，容易影响可用性。<br />Minio天生具有分布式存储的优势，并且内部采用纠删码，**RAID或类似技术反而不能提供额外的弹性或可用性优势，而且通常会降低系统性能**。<br />**请确保服务器池中的所有节点使用相同类型(NVMe、SSD或HDD)的驱动器，并且容量相同**。MinIO不区分驱动器类型，也不能从混合存储类型中获益。此外。MinIO将每个驱动器的大小限制为部署中最小的驱动器所拥有的容量。例如，如果部署有15个10TB驱动器和1个1TB驱动器，MinIO将每个驱动器的容量限制为1TB。<br />MinIO需要使用展开符号{x…y}用来表示Server Pool中每一个节点的一组磁盘驱动器，并且要求Server Pool中所有节点的磁盘驱动器序列相同。MinIO还要求物理驱动器的顺序在重新启动时保持不变，这样给定的挂载点总是指向相同的格式化驱动器。因此，**MinIO强烈建议使用/etc/fstab或类似的基于文件的挂载配置**。<br />示例如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1681693022773-932fd193-1776-4c55-82f9-7964088dd1f6.png#averageHue=%23e3e5e7&clientId=u831892e2-34b1-4&from=paste&id=u9fa4798f&originHeight=377&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2a8e83a4-492f-4edd-b0a0-a7750b55768&title=)<br />可以使用/mnt/disk{1…4}来指定整个驱动器范围。如果需要为驱动器设置子目录，例如minio，那可以设置为/mnt/disk{1…4}/minio。<br />**MinIO不支持将具有已存储MinIO数据的驱动器任意迁移到新的挂载位置。**<br />MinIO严格的写后读和写后列表一致性模型要求使用本地驱动器文件系统(xfs、ext4等)。如果底层存储卷是NFS或类似的网络连接存储卷，MinIO就不能提供一致性保证。对于需要使用网络连接存储的部署，请使用NFSv4以获得最佳效果。
<a name="EjntV"></a>
### 1.4 Server Pool 纠删码一致原则(对等扩容)
MinIO要求每一个Server Pool满足erasure code settings。例如一个组原先是3台共12块盘，那扩容新增加的组，必须是是原先的倍数。
<a name="JFGTW"></a>
## 2、扩容注意事项
<a name="Pw3Z7"></a>
### 2.1 文件写策略
MinIO不会将对象从旧的Server Pool中自动迁移到新添加的Server Pool中，即不会进行重平衡。但是，MinIO根据存储空闲大小进行加权选择，空闲空间越大的Server Pool会承担更多的写入流量，加权值是Server Pool的空闲空间量除以所有可用池上的空闲空间。<br />例如目前集群有3个Server Pool：

- Pool A has 3 TiB of free space
- Pool B has 2 TiB of free space
- Pool C has 5 TiB of free space

Minio分别向各个Pool中写入的概率为：

- Pool A：30% = 3 / (3 + 2 + 5)
- Pool B：20% = 2 / (3 + 2 + 5)
- Pool C：50% = 5 / (3 + 2 + 5)

除了可用空间加权后，如果继续写入文件会使得磁盘使用率超过99%或者空闲的inode计数低于1000，同样不会再往该Pool中写入新文件。<br />**如果需要，你可以使用mc admin rebalance手动启动一个rebalance过程。**关于这块详细的介绍，将在后续文章中介绍。
<a name="UsWRn"></a>
### 2.2 机器配置相同或相似
MinIO强烈建议为新Server Pool的所有节点选择基本相似的硬件配置。确保硬件(CPU、内存、主板、存储适配器)和软件(操作系统、内核设置、系统服务)在池中的所有节点上保持一致。<br />因为新加入的服务集群将在Minio中充当主力**写**，相当于热数据的主要承接，老的Server Pool基本只负责读查询，所以新Server Pool的服务性能将决定整个集群的性能。<br />**当然，新的服务节点的配置，无需与其他组内的配置一致，因为Minio的故障域是Server Pool。**
<a name="MC4L6"></a>
### 2.3 扩容是非破坏性的
MinIO添加新的Server Pool是需要同时重启所有的Minio nodes。**MinIO强烈建议同时重启所有节点。MinIO操作是原子的，并且严格一致。因此，重新启动过程不会中断应用程序和正在进行的操作**。**禁止滚动重启。**
<a name="AEXM3"></a>
## 3、集群扩容
接下来分布详细介绍集群扩容的个个步骤。
<a name="UvHuE"></a>
### Step1：在新的Server Pool中的每一个节点安装MinIO二进制文件
RPM和DEB包自动将MinIO安装到必要的系统路径上，并创建用于自动运行MinIO的systemd服务文件。**MinIO强烈建议使用RPM或DEB安装**。<br />如果操作系统的架构为amd64(Intel or AMD 64-bit processors)，请使用如下命令：
```bash
# RPM
wget https://dl.min.io/server/minio/release/linux-amd64/archive/minio-20230309231613.0.0.x86_64.rpm -O minio.rpm
sudo dnf install minio.rpm

# DEB
wget https://dl.min.io/server/minio/release/linux-amd64/archive/minio_20230309231613.0.0_amd64.deb -O minio.deb
sudo dpkg -i minio.deb

# Binary
wget https://dl.min.io/server/minio/release/linux-amd64/minio
chmod +x minio
sudo mv minio /usr/local/bin
```
如果是ARM架构的操作系统，请下载对应的包。具体的安装命令与AMD64一样，下文的介绍也只基于AMD64介绍。
<a name="NjGZG"></a>
### Step2：添加TLS/SSL证书
Minio启用TLS加密通道的话，会自动检测${USER_HOME}/.minio/certs目录下的证书(.crt)与私钥(.key)。<br />Systemd-managed部署的minio服务，minio.service文件中默认定义的用户名minio-user，故需要提前创建好 /home/minio-user。

- 需要将证书放到/home/minio-user/.minio/certs目录下
- 如果MinIO服务或客户端使用的是未知证书颁发机构（自签名或内部CA）签署的证书，则必须在每一个minio节点上将CA证书放在/home/minio-user/. minio/certs/CAs 目录下。**MinIO拒绝无效的证书(不受信任、过期或格式不正确)。**

如果**minio.service**中指定了其他用户账号，将会从该账号对应的主目录去加载证书，也可以用 minio server --certs-dir 命令来指定证书所在的目录，亦或者修改/etc/defaults/minio文件中的MINIO_OPTS变量中设置cert_dir。<br />**运行MinIO服务器进程的systemd用户必须具有指定目录的读取和ls权限。**
<a name="SDfPI"></a>
### Step3：创建systemd服务文件
deb或rpm包会自动将systemd服务文件安装在/etc/systemd/system/minio.service。如果是基于二进制文件安装的话，需要手动在所有的minio节点上创建该文件。<br />minio.service文件的内容如下：
```
[Unit]
Description=MinIO
Documentation=https://min.io/docs/minio/linux/index.html
Wants=network-online.target
After=network-online.target
AssertFileIsExecutable=/usr/local/bin/minio

[Service]
WorkingDirectory=/usr/local

User=minio-user
Group=minio-user
ProtectProc=invisible

EnvironmentFile=-/etc/default/minio
ExecStartPre=/bin/bash -c "if [ -z \"${MINIO_VOLUMES}\" ]; then echo \"Variable MINIO_VOLUMES not set in /etc/default/minio\"; exit 1; fi"
ExecStart=/usr/local/bin/minio server $MINIO_OPTS $MINIO_VOLUMES

# Let systemd restart this service always
Restart=always

# Specifies the maximum file descriptor number that can be opened by this process
LimitNOFILE=65536

# Specifies the maximum number of threads this process can create
TasksMax=infinity

# Disable timeout logic and wait until process is stopped
TimeoutStopSec=infinity
SendSIGKILL=no

[Install]
WantedBy=multi-user.target

# Built for ${project.name}-${project.version} (${project.name})
```
minio.service文件默认创建的用户名、组都为minio-user。可以使用groupadd、useradd命令创建用户和组。下面是创建用户、组以及设置项目目录权限的示范命令：
```bash
groupadd -r minio-user
useradd -M -r -g minio-user minio-user
chown minio-user:minio-user /mnt/disk1 /mnt/disk2 /mnt/disk3 /mnt/disk4
```
其中/mnt/disk1为minio将要使用的磁盘目录。
<a name="MGmqV"></a>
### Step4：创建服务环境文件
可以在/etc/default/minio目录创建一个服务环境文件，Minio Server、minio.service文件会使用该目录作为所有环境变量的输入源，示例如下：
```
# Set the hosts and volumes MinIO uses at startup
# The command uses MinIO expansion notation {x...y} to denote a
# sequential series，
#
# The following example starts the MinIO server with two server pools.
#
# The space delimiter indicates a seperate server pool
#
# The second set of hostnames and volumes is the newly added pool.
# The pool has sufficient stripe size to meet the existing erasure code
# parity of the deployment (2 x EC:4)
#
# The command includes the port on which the MinIO servers listen for each
# server pool.
# 多个Server Pool用空格隔开
MINIO_VOLUMES="https://minio{1...4}.example.net:9000/mnt/disk{1...4}/minio https://minio{5...12}.example.net:9000/mnt/disk{1...8}/minio"

# Set all MinIO server options
#
# The following explicitly sets the MinIO Console listen address to
# port 9001 on all network interfaces. The default behavior is dynamic
# port selection.

MINIO_OPTS="--console-address :9001"

# Set the root username. This user has unrestricted permissions to
# perform S3 and administrative API operations on any resource in the
# deployment.
#
# Defer to your organizations requirements for superadmin user name.

MINIO_ROOT_USER=minioadmin

# Set the root password
#
# Use a long, random, unique string that meets your organizations
# requirements for passwords.

MINIO_ROOT_PASSWORD=minio-secret-key-CHANGE-ME

# Set to the URL of the load balancer for the MinIO deployment 
# This value *must* match across all MinIO servers. If you do
# not have a load balancer, set this value to to any *one* of the
# MinIO hosts in the deployment as a temporary measure.
# 设置负载均衡器的URL，这个必须在所有节点上一致
MINIO_SERVER_URL="https://minio.example.net:9000"
```
关于更多的环境变量，可以参考官方文档：[https://min.io/docs/minio/linux/reference/minio-server/minio-server.html#id5](https://min.io/docs/minio/linux/reference/minio-server/minio-server.html#id5) 后续也会再详细介绍。
<a name="cd9lN"></a>
### Step5：基于扩容后的配置重新启动Minio
在部署的每一个节点上同时执行如下命令启动minio：
```bash
sudo systemctl restart minio.service
```
使用如下命令确认一下服务在线并且正常：
```bash
sudo systemctl status minio.service
# 收集stsytemctl命令运行的日志
journalctl -f -u minio.service
```
MinIO强烈建议同时重启所有节点。MinIO操作是原子的，并且严格一致。因此，重新启动过程不会中断应用程序和正在进行的操作。<br />**不要执行“滚动”(例如一次一个节点)重启**。
<a name="seJ7m"></a>
### Step6：更新负载均衡器中upstream(路由机制)
更新负责均衡层的配置，使得客户端流量可以路由到Minio集群中新加入的组。当然Minio内部可以自动识别新加入的组并进行负载，但毕竟增加了网络跳数。<br />最后可以通过MinIO Console去确认一下对象上传与下载性能。
