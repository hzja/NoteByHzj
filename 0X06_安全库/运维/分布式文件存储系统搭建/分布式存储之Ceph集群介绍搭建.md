Ceph 分布式存储
<a name="g5zzy"></a>
### 块存储：典型设备，磁盘阵列，硬盘
**作用**：主要是将裸磁盘空间映射给主机使用，磁盘阵列（内含多块硬盘）做**RAID**操作或者**LVM**操作，逻辑划分出N块Logic盘并映射给主机使用，操作系统会识别到N块硬盘，但是无法感知到底是逻辑盘还是物理盘，所以还是会对其进行分区和格式化（无法共享数据）。每块逻辑盘实际上是由多块硬盘共同组成，在读写数据的时候，几块物理硬盘可以并行寻址和操作，大大提高了IO效率。<br />**优点**：对块磁盘组成逻辑盘，读写效率增加<br />**缺点**：相当于自己主机的硬盘，无法进行数据共享
<a name="NepSO"></a>
### 文件存储
**典型设备**：`FTP、NFS服务器`<br />**优点**：克服了块存储文件无法共享的问题<br />**缺点**：读写、传输速率低<br />**场景**：日志、邮箱、目录结构文件存储<br />**备注**：常见的存储中 NFS、NAS也是属于文件系统存储
<a name="J8CFq"></a>
### 对象存储
**对象存储**:`也叫做基于对象的存储，是用来描述解决和处理离散单元的方法的通用术语，这些离散单元被称作为对象。`<br />对象存储经常被比作在一家高级餐厅代客停车。当一个顾客需要代客停车时，他就把钥匙交给别人，换来一张收据。这个顾客不用知道他的车被停在哪，也不用知道在他用餐时服务员会把他的车移动多少次。在这个比喻中，一个存储对象的唯一标识符就代表顾客的收据。<br />**优点**：综合了块读写高速、文件共享特性<br />**存储数据类型**：指非结构化数据，如图片、音视频、文档等<br />**应用场景**：即一次写入多次读取。
<a name="LU7XN"></a>
### 分布式存储
**分布式存储**:是将数据分散存储在多台独立的设备上。传统的网络存储系统采用集中的存储服务器存放所有数据，存储服务器成为系统性能的瓶颈，也是可靠性和安全性的焦点，不能满足大规模存储应用的需要。分布式网络存储系统采用可扩展的系统结构，利用多台存储服务器分担存储负荷，利用位置服务器定位存储信息，它不但提高了系统的可靠性、可用性和存取效率，还易于扩展。<br />1、高可用性，副本数灵活控制，支持故障域分割，数据强一致性，没有单点故障<br />2、高可扩展性<br />3、特性丰富<br />**简介**：不管是想为云平台提供**Ceph 对象存储**和/或 **Ceph 块设备**，还是想部署一个 **Ceph 文件系**统或者把 Ceph 作为他用，所有 Ceph 存储集群的部署都始于部署一个个 Ceph 节点、网络和 Ceph 存储集群。Ceph 存储集群至少需要一个 Ceph Monitor 和两个 OSD 守护进程。而运行 Ceph 文件系统客户端时，则必须要有元数据服务器（ Metadata Server ）
<a name="d5Y7f"></a>
### Ceph简介
Ceph是一个统一的分布式存储系统，设计初衷是提供较好的性能、可靠性和可扩展性。Ceph 存储集群至少需要一个 Ceph Monitor 和两个 OSD 守护进程。而运行 Ceph 文件系统客户端时，则必须要有元数据服务器（ Metadata Server ）
<a name="pN9hI"></a>
### Ceph优点
<a name="ZKNqm"></a>
#### 1、CRUSH算法
ceph摒弃了传统的集中式存储元数据寻址的方案，转而使用CRUSH算法完成数据的寻址操作。CRUSH在一致性哈希基础上很好的考虑了容灾域的隔离，能够实现各类负载的副本放置规则
<a name="fv2XO"></a>
#### 2、高可用
Ceph中的数据副本数量可以由管理员自行定义，并可以通过CRUSH算法指定副本的物理存储位置以分隔故障域，支持数据强一致性；ceph可以忍受多种故障场景并自动尝试并行修复。
<a name="zZWon"></a>
#### 3、高扩展性
Ceph不同于swift，客户端所有的读写操作都要经过代理节点。一旦集群并发量增大时，代理节点很容易成为单点瓶颈。Ceph本身并没有主控节点，扩展起来比较容易，并且理论上，它的性能会随着磁盘数量的增加而线性增长。
<a name="MNwAQ"></a>
#### 4、特性丰富
Ceph支持三种调用接口：对象存储，块存储，文件系统挂载。三种方式可以一同使用。在国内一些公司的云环境中，通常会采用ceph作为openstack的唯一后端存储来提升数据转发效率。<br />Ceph的基本组成结构如下图：<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1608046840575-e77745a5-56a0-4351-8cbe-80b656d8a11e.png#averageHue=%23e4e7e2&height=647&id=FZKqe&originHeight=647&originWidth=862&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=862)<br />Ceph OSDs: Ceph OSD 守护进程（ Ceph OSD ）的功能是存储数据，处理数据的复制、恢复、回填、再均衡，并通过检查其他OSD 守护进程的心跳来向 Ceph Monitors 提供一些监控信息。当 Ceph 存储集群设定为有2个副本时，至少需要2个 OSD 守护进程，集群才能达到 active+clean 状态（ Ceph 默认有3个副本，但你可以调整副本数）。<br />Monitors: Ceph Monitor维护着展示集群状态的各种图表，包括监视器图、 OSD 图、归置组（ PG ）图、和 CRUSH 图。Ceph 保存着发生在Monitors 、 OSD 和 PG上的每一次状态变更的历史信息（称为 epoch ）。<br />MDSs: Ceph 元数据服务器（ MDS ）为 Ceph 文件系统存储元数据（也就是说，Ceph 块设备和 Ceph 对象存储不使用MDS ）。元数据服务器使得 POSIX 文件系统的用户们，可以在不对 Ceph 存储集群造成负担的前提下，执行诸如 ls、find 等基本命令。
<a name="92d8x"></a>
### 部署工作
[官网指导部署](https://docs.ceph.com/en/latest/cephadm/install/)<br />[https://docs.ceph.com/en/latest/cephadm/install/](https://docs.ceph.com/en/latest/cephadm/install/)
<a name="WCyps"></a>
#### 1、配置基础环境
准备三台服务器
```
192.168.106.100     ceph-admin 
192.168.106.101     ceph-node1
192.168.106.1002    ceph-node2
```
<a name="ezAeE"></a>
#### 2、三台服务器关闭防火墙、核心防护
```bash
systemctl stop firewalld
systemctl disable firewalld
setenforce 0
sed -i '7s/enforcing/disabled/' /etc/selinux/config
```
<a name="aqkX0"></a>
#### 3、配置hosts
```bash
cat   /etc/hosts
192.168.106.100     ceph-admin 
192.168.106.101     ceph-node1
192.168.106.1002    ceph-node2
```
<a name="hkYjW"></a>
#### 4、免密登录
配置互信（各个节点）
```bash
ssh-keygen  -t  rsa
ssh-copy-id    ceph-node1
ssh-copy-id    ceph-node2
```
<a name="6YBbT"></a>
#### 5、配置初始环境
```bash
yum -y install wget curl net-tools bash-completion
wget -O /etc/yum.repos.d/CentOS-Base.repo http://mirrors.aliyun.com/repo/Centos-7.repo
wget -O /etc/yum.repos.d/epel.repo http://mirrors.aliyun.com/repo/epel-7.repo
```
配置ceph源
```bash
cat << EOF > /etc/yum.repos.d/ceph.repo
[ceph]
name=Ceph packages for
baseurl=https://mirrors.aliyun.com/ceph/rpm-mimic/el7/$basearch
enabled=1
gpgcheck=1
type=rpm-md
gpgkey=https://mirrors.aliyun.com/ceph/keys/release.asc
priority=1
[ceph-noarch]
name=Ceph noarch packages
baseurl=https://mirrors.aliyun.com/ceph/rpm-mimic/el7/noarch/
enabled=1
gpgcheck=1
type=rpm-md
gpgkey=https://mirrors.aliyun.com/ceph/keys/release.asc
priority=1
[ceph-source]
name=Ceph source packages
baseurl=https://mirrors.aliyun.com/ceph/rpm-mimic/el7/SRPMS/
enabled=1
gpgcheck=1
type=rpm-md
gpgkey=https://mirrors.aliyun.com/ceph/keys/release.asc
priority=1
EOF
```
<a name="eoVc2"></a>
#### 6、配置NTP时间同步
服务端:
```bash
vim  /etc/chrony.conf
allow 192.168/16 
local stratum 10
systemctl restart chronyd.service
```
客户端
```bash
vim  /etc/chrony.conf
server 192.168.106.100 iburst
systemctl restart chronyd.service
chronyc sources
```
<a name="SvISg"></a>
#### 7、安装docker 环境
```bash
yum  -y install  docker-ce
systemctl  restart  docker
systemctl  enable  docker
docker pull ceph/daemon:v3.0.5-stable-3.0-luminous-centos-7-x86_64   #各个节点安装
```
首先启动主节点mon（ceph-admin节点）
```bash
docker run -d --net=host --name=mon -v /etc/ceph:/etc/ceph -v /var/lib/ceph:/var/lib/ceph -e MON_IP=192.168.106.100 -e CEPH_PUBLIC_NETWORK=192.168.106.0/24 ceph/daemon:v3.0.5-stable-3.0-luminous-centos-7-x86_64  mon
```
若直接使用非xfs文件系统的硬盘，需要在配置文件中加以下配置：
```bash
vim /etc/ceph/ceph.conf
osd max object name len = 256 
osd max object namespace len = 64
```
<a name="CJ0xI"></a>
#### 8、拷贝配置文件
```bash
scp   -r  /etc/ceph/    ceph-node1:/etc/
scp   -r  /etc/ceph/    ceph-node2:/etc/
scp -r /var/lib/ceph ceph-node1:/var/lib/
scp -r /var/lib/ceph ceph-node2:/var/lib/
```
ceph-node1执行
<a name="FmQ7b"></a>
#### 9、创建mon
```bash
docker run -d --net=host --name=mon -v /etc/ceph:/etc/ceph -v /var/lib/ceph:/var/lib/ceph -e MON_IP=192.168.106.101 -e CEPH_PUBLIC_NETWORK=192.168.106.0/24 ceph/daemon:v3.0.5-stable-3.0-luminous-centos-7-x86_64  mon
```
ceph-node2执行
```bash
docker run -d --net=host --name=mon -v /etc/ceph:/etc/ceph -v /var/lib/ceph:/var/lib/ceph -e MON_IP=192.168.106.102 -e CEPH_PUBLIC_NETWORK=192.168.106.0/24  ceph/daemon:v3.0.5-stable-3.0-luminous-centos-7-x86_64  mon
```
<a name="GXttD"></a>
#### 10、挂载osd  (三台服务器都要执行格式化和挂载)
```bash
mkdir  /osd0
mkfs.xfs /dev/sdb
mount /dev/sdb  /osd0
```
<a name="WEVvC"></a>
#### 11、启动OSD服务
```bash
docker run -d --net=host --name=osd1 -v /etc/ceph:/etc/ceph -v /var/lib/ceph:/var/lib/ceph -v /dev:/dev -v /osd0:/var/lib/ceph/osd --privileged=true ceph/daemon:v3.0.5-stable-3.0-luminous-centos-7-x86_64 osd_directory
```
<a name="KJTd2"></a>
#### 12、在node1启动mgr:
```bash
docker run -d --net=host \
-v /etc/ceph:/etc/ceph \
-v /var/lib/ceph/:/var/lib/ceph/ \
ceph/daemon:v3.0.5-stable-3.0-luminous-centos-7-x86_64 mgr
```
<a name="sl8V4"></a>
#### 13、在ceph中创建一个pool存储池
rdb是存储池名字，64是pg数量，pg值计算参考[https://ceph.com/pgcalc/](https://ceph.com/pgcalc/)
```bash
docker exec mon ceph osd pool create rbd 64
```
<a name="3SGUq"></a>
#### 14、配置crushmap，根据osd数目，0.15做相应调整，整体之和不大于1
```bash
docker exec mon ceph osd crush add osd.0 0.15 host=admin
docker exec mon ceph osd crush add osd.1 0.15 host=admin
```
检查osd tree
```bash
docker exec mon ceph osd tree
```
<a name="b0EE3"></a>
#### 15、更新crushmap使得节点都归属于root default
```bash
docker exec mon ceph osd crush move node0 root=default
docker exec mon ceph osd crush move node1 root=default
```
<a name="rhyuD"></a>
#### 16、检查ceph运行情况
```bash
docker exec mon ceph -s
```
![](https://cdn.nlark.com/yuque/0/2020/png/396745/1608046840606-bb7cef1c-b3c3-471a-a62d-a0ad025dafa3.png#averageHue=%23070503&height=250&id=jX3rq&originHeight=250&originWidth=512&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=512)
<a name="8lCpI"></a>
#### 17、测试
测试ceph集群在块存储下镜像的创建和文件的上传，如果成功才能说明ceph集群安装成功
```bash
docker exec mon rbd create rbd/test-image --size 100M
docker exec mon rbd info rbd/test-image
docker exec mon rados -p rbd ls
```
![](https://cdn.nlark.com/yuque/0/2020/png/396745/1608046840627-518f88ed-797b-4611-bd62-f78c9c79ba26.png#averageHue=%23080604&height=283&id=AfxVF&originHeight=283&originWidth=663&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=663)
