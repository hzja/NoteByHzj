MinIO<br />这里介绍如何在测试环境中搭建MinIO，这也是构建MinIO学习环境最基本的步骤。
<a name="W4QeC"></a>
## 1、准备实验环境
使用Oracle VM VirtualBox虚拟机，安装一个最小版本的Linux，然后添加4块虚拟盘，用于充当MinIO的虚拟盘。<br />实验环境如下所示：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679275326529-2b535690-bb4c-49ab-8e9a-c685fafa7e33.png#averageHue=%23d7d5d4&clientId=u98f9518e-2c06-4&from=paste&id=u3a093d96&originHeight=709&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u1b0ca49b-dacd-45d3-8b22-11ab88c1a3a&title=)<br />接下来和大家简单介绍一下虚拟环境的搭建。
<a name="s1kk7"></a>
### Step1：具体添加虚拟盘的方式：
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679275326516-be4ab6a3-3207-49a3-b167-05dc29e5b923.png#averageHue=%23e7e6e6&clientId=u98f9518e-2c06-4&from=paste&id=u9f558f0a&originHeight=824&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua6958c2e-354e-4963-8ada-a7887dfc080&title=)<br />操作路径：【设置】-【存储】，在控制SATA添加【加号】<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679275326518-624d47e6-9ca3-4fe6-8ff9-dc74bd99acdf.png#averageHue=%2384f133&clientId=u98f9518e-2c06-4&from=paste&id=u3653ba7c&originHeight=489&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3be32f99-9eb0-48b0-9dc3-b56070e3b64&title=)
<a name="mBxXq"></a>
### Step2：点击【创建】，创建一块磁盘，操作如下图所示：
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679275326456-be3d1f0e-763b-4d50-b5ab-3d672dcd143b.png#averageHue=%23e4b9ac&clientId=u98f9518e-2c06-4&from=paste&id=u7fc7973a&originHeight=652&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ufcf60a6b-9649-4c11-a6a4-6117de1e2ca&title=)<br />创建好四块磁盘后，重启服务器。
<a name="IG285"></a>
### Step3：检验操作系统是否识别到磁盘
首先可以运行lsblk，执行结果如下所示：
```bash
[root@localhost ~]# lsblk
NAME            MAJ:MIN RM  SIZE RO TYPE MOUNTPOINT
sda               8:0    0   50G  0 disk
├─sda1            8:1    0    1G  0 part /boot
└─sda2            8:2    0   49G  0 part
  ├─centos-root 253:0    0 45.1G  0 lvm  /
  └─centos-swap 253:1    0  3.9G  0 lvm  [SWAP]
sdb               8:16   0   10G  0 disk
sdc               8:32   0   10G  0 disk
sdd               8:48   0   10G  0 disk
sde               8:64   0   10G  0 disk
sr0              11:0    1 1024M  0 rom
```
根据创建文件的大小，可以看到创建的四块虚拟盘sdb、sdc、sdd、sde，但并未挂载(MOUNTPOINT为空)<br />但此时使用 `df -h` 命令，无法查看到这些盘，结果如下所示：
```bash
[root@localhost ~]# df -h
文件系统                 容量  已用  可用 已用% 挂载点
devtmpfs                 1.9G     0  1.9G    0% /dev
tmpfs                    1.9G     0  1.9G    0% /dev/shm
tmpfs                    1.9G  8.7M  1.9G    1% /run
tmpfs                    1.9G     0  1.9G    0% /sys/fs/cgroup
/dev/mapper/centos-root   46G  1.3G   44G    3% /
/dev/sda1               1014M  151M  864M   15% /boot
tmpfs                    379M     0  379M    0% /run/user/0
```
但使用`fdisk -l` 命令也能查询到四块虚拟盘信息，如下所示：
```bash
[root@localhost ~]# fdisk -l

磁盘 /dev/sde：10.7 GB, 10737418240 字节，20971520 个扇区
Units = 扇区 of 1 * 512 = 512 bytes
扇区大小(逻辑/物理)：512 字节 / 512 字节
I/O 大小(最小/最佳)：512 字节 / 512 字节


磁盘 /dev/sdd：10.7 GB, 10737418240 字节，20971520 个扇区
Units = 扇区 of 1 * 512 = 512 bytes
扇区大小(逻辑/物理)：512 字节 / 512 字节
I/O 大小(最小/最佳)：512 字节 / 512 字节


磁盘 /dev/sdc：10.7 GB, 10737418240 字节，20971520 个扇区
Units = 扇区 of 1 * 512 = 512 bytes
扇区大小(逻辑/物理)：512 字节 / 512 字节
I/O 大小(最小/最佳)：512 字节 / 512 字节


磁盘 /dev/sda：53.7 GB, 53687091200 字节，104857600 个扇区
Units = 扇区 of 1 * 512 = 512 bytes
扇区大小(逻辑/物理)：512 字节 / 512 字节
I/O 大小(最小/最佳)：512 字节 / 512 字节
磁盘标签类型：dos
磁盘标识符：0x000b5407

设备 Boot      Start         End      Blocks   Id  System
/dev/sda1   *        2048     2099199     1048576   83  Linux
/dev/sda2         2099200   104857599    51379200   8e  Linux LVM

磁盘 /dev/sdb：10.7 GB, 10737418240 字节，20971520 个扇区
Units = 扇区 of 1 * 512 = 512 bytes
扇区大小(逻辑/物理)：512 字节 / 512 字节
I/O 大小(最小/最佳)：512 字节 / 512 字节


磁盘 /dev/mapper/centos-root：48.4 GB, 48444211200 字节，94617600 个扇区
Units = 扇区 of 1 * 512 = 512 bytes
扇区大小(逻辑/物理)：512 字节 / 512 字节
I/O 大小(最小/最佳)：512 字节 / 512 字节


磁盘 /dev/mapper/centos-swap：4160 MB, 4160749568 字节，8126464 个扇区
Units = 扇区 of 1 * 512 = 512 bytes
扇区大小(逻辑/物理)：512 字节 / 512 字节
I/O 大小(最小/最佳)：512 字节 / 512 字节
```
那如何将四块虚拟盘挂载到文件系统呢？
<a name="Jp3vx"></a>
### Step4：创建分区、格式化磁盘，并挂载到文件
具体命令如下：
```bash
[root@localhost ~]# fdisk /dev/sdb
欢迎使用 fdisk (util-linux 2.23.2)。

更改将停留在内存中，直到您决定将更改写入磁盘。
使用写入命令前请三思。

Device does not contain a recognized partition table
使用磁盘标识符 0x2bb131bd 创建新的 DOS 磁盘标签。

命令(输入 m 获取帮助)：m
命令操作
   a   toggle a bootable flag
   b   edit bsd disklabel
   c   toggle the dos compatibility flag
   d   delete a partition
   g   create a new empty GPT partition table
   G   create an IRIX (SGI) partition table
   l   list known partition types
   m   print this menu
   n   add a new partition,创建一个分区
   o   create a new empty DOS partition table
   p   print the partition table
   q   quit without saving changes
   s   create a new empty Sun disklabel
   t   change a partition's system id
   u   change display/entry units
   v   verify the partition table
   w   write table to disk and exit
   x   extra functionality (experts only)

命令(输入 m 获取帮助)：n
Partition type:
   p   primary (0 primary, 0 extended, 4 free)
   e   extended
Select (default p): p
分区号 (1-4，默认 1)：1
起始 扇区 (2048-20971519，默认为 2048)：
将使用默认值 2048
Last 扇区, +扇区 or +size{K,M,G} (2048-20971519，默认为 20971519)：
将使用默认值 20971519
分区 1 已设置为 Linux 类型，大小设为 10 GiB

命令(输入 m 获取帮助)：w
The partition table has been altered!

Calling ioctl() to re-read partition table.
正在同步磁盘。

## 这里格式为ext4，如果是新搭建MinIO集群，文件存储系统强烈推荐使用xfs，对应的命令：mkfs.xfs /dev/sdb
[root@localhost ~]# mkfs.ext4 /dev/sdb
mke2fs 1.42.9 (28-Dec-2013)
/dev/sdb is entire device, not just one partition!
无论如何也要继续? (y,n) y
文件系统标签=
OS type: Linux
块大小=4096 (log=2)
分块大小=4096 (log=2)
Stride=0 blocks, Stripe width=0 blocks
655360 inodes, 2621440 blocks
131072 blocks (5.00%) reserved for the super user
第一个数据块=0
Maximum filesystem blocks=2151677952
80 block groups
32768 blocks per group, 32768 fragments per group
8192 inodes per group
Superblock backups stored on blocks:
 32768, 98304, 163840, 229376, 294912, 819200, 884736, 1605632

Allocating group tables: 完成
正在写入inode表: 完成
Creating journal (32768 blocks): 完成
Writing superblocks and filesystem accounting information: 完成

[root@localhost ~]# mkdir /data1
# 注释：挂载到文件系统 /data1
[root@localhost ~]# mount -t ext4 /dev/sdb /data1
[root@localhost ~]# df -h
文件系统                 容量  已用  可用 已用% 挂载点
devtmpfs                 1.9G     0  1.9G    0% /dev
tmpfs                    1.9G     0  1.9G    0% /dev/shm
tmpfs                    1.9G  8.7M  1.9G    1% /run
tmpfs                    1.9G     0  1.9G    0% /sys/fs/cgroup
/dev/mapper/centos-root   46G  1.3G   44G    3% /
/dev/sda1               1014M  151M  864M   15% /boot
tmpfs                    379M     0  379M    0% /run/user/0
/dev/sdb                 9.8G   37M  9.2G    1% /data1
```
在使用同样的命令将 /dev/sdc、/dev/sdd、/dev/sde创建分区并挂载到/data2,/data3,/data4。<br />最后的执行效果如下：
```bash
[root@localhost ~]# df -h
文件系统                 容量  已用  可用 已用% 挂载点
devtmpfs                 1.9G     0  1.9G    0% /dev
tmpfs                    1.9G     0  1.9G    0% /dev/shm
tmpfs                    1.9G  8.7M  1.9G    1% /run
tmpfs                    1.9G     0  1.9G    0% /sys/fs/cgroup
/dev/mapper/centos-root   46G  1.3G   44G    3% /
/dev/sda1               1014M  151M  864M   15% /boot
tmpfs                    379M     0  379M    0% /run/user/0
/dev/sdb                 9.8G   37M  9.2G    1% /data1
/dev/sdc                 9.8G   37M  9.2G    1% /data2
/dev/sdd                 9.8G   37M  9.2G    1% /data3
/dev/sde                 9.8G   37M  9.2G    1% /data4
```
至此，实验环境准备完成。
<a name="IQ5kb"></a>
## 2、部署MinIO
MinIO支持三种部署架构：

- Single-Node Single-Drive (SNSD or “Standalone”) 单节点单驱动器
- Single-Node Multi-Drive (SNMD or “Standalone Multi-Drive”) 单节点多驱动器，伪分布式，能使用纠错码等生产特性。
- Multi-Node Multi-Drive (MNMD or “Distributed”) 多节点多驱动器，即分布式部署架构，强烈推荐用于生产环境。

由于测试资源有限，本文采用Single-Node Multi-Drive进行搭建。<br />SNMD部署架构支持驱动器(磁盘)的容错。生产环境强烈推荐使用MNMD架构。
<a name="SRkoR"></a>
### 2.1 安装前准备
MinIO强烈推荐直连的JBOD阵列，xfs格式的磁盘以获得最佳性能。<br />DAS (Direct-Attached Storage)与网络存储(NAS、SAN、NFS)相比具有显著的性能和一致性优势。<br />使用非xfs文件系统(ext4、btrfs、zfs)的部署往往具有较低的性能，同时ext4等文件存储对单个目录下的文件有个数限制，容易影响可用性。<br />Minio天生具有分布式存储的优势，并且内部采用纠删码，RAID或类似技术反而不能提供额外的弹性或可用性优势，而且通常会降低系统性能。<br />请确保服务器池中的所有节点使用相同类型(NVMe、SSD或HDD)的驱动器，并且容量相同。MinIO不区分驱动器类型，也不能从混合存储类型中获益。此外。MinIO将每个驱动器的大小限制为部署中最小的驱动器所拥有的容量。例如，如果部署有15个10TB驱动器和1个1TB驱动器，MinIO将每个驱动器的容量限制为1TB。<br />MinIO需要使用展开符号{x…y}用来表示Server Pool中每一个节点的一组磁盘驱动器，并且要求Server Pool中所有节点的磁盘驱动器序列相同。MinIO还要求物理驱动器的顺序在重新启动时保持不变，这样给定的挂载点总是指向相同的格式化驱动器。
<a name="dM7iv"></a>
### 2.2 如何一步一步安装MinIO
<a name="gRLc5"></a>
#### Step1：根据不同的操作系统与CPU架构，下载对应的安装包
例如当前虚拟机是amd，下载对应的RPM包，命令如下：
```bash
wget https://dl.min.io/server/minio/release/linux-amd64/archive/minio-20230313194617.0.0.x86_64.rpm -O minio.rpm
sudo dnf install minio.rpm
```
通过该命令安装后，有关MinIO相关文件：

- /usr/local/bin/ MinIO 可执行文件所在的目录，该目录下就只有一个可执行文件 minio。
- /etc/systemd/system/minio.service MinIO系统服务文件
- /etc/default/minio MinIO环境变量文件

如果是使用二进制文件直接安装，其实就是要创建如上三个文件。
<a name="ETJYr"></a>
#### Step2：创建systemd系统服务文件
如果是基于rpm或者deb包进行安装的话，会自动创建systemd系统服务文件，如果是二级制安装模式，就需要自动创建该文件，文件的全路径为/etc/systemd/system/minio.service，具体的内容如下：
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
默认情况下MinIO使用minio-user用户与minio-user用户组启动MinIO进程，故需要创建对应的用户与组，具体命令如下：
```bash
[root@localhost bin]# groupadd -r minio-user
[root@localhost bin]# useradd -M -r -g minio-user minio-user
# 主要是将minio需要用到的磁盘驱动器设置为 minio-user:minio-user
[root@localhost bin]# chown minio-user:minio-user /data1 /data2 /data3 /data4
```
<a name="SUc8o"></a>
#### Step3：创建环境变量
默认需要创建 /etc/default/minio 文件，用于设置minio相关的环境变量（配置项）。关于MinIO的配置详细说明可以参考官方网站：[https://min.io/docs/minio/linux/operations/install-deploy-manage/deploy-minio-single-node-multi-drive.html#id6](https://min.io/docs/minio/linux/operations/install-deploy-manage/deploy-minio-single-node-multi-drive.html#id6)<br />例如创建的最小化配置文件如下：
```
# MINIO_ROOT_USER and MINIO_ROOT_PASSWORD sets the root account for the MinIO server.
# This user has unrestricted permissions to perform S3 and administrative API operations on any resource in the deployment.
# Omit to use the default values 'minioadmin:minioadmin'.
# MinIO recommends setting non-default values as a best practice, regardless of environment.

MINIO_ROOT_USER=myminioadmin
MINIO_ROOT_PASSWORD=minio-secret-key-change-me

# MINIO_VOLUMES sets the storage volumes or paths to use for the MinIO server.
# The specified path uses MinIO expansion notation to denote a sequential series of drives between 1 and 4, inclusive.
# All drives or paths included in the expanded drive list must exist *and* be empty or freshly formatted for MinIO to start successfully.

MINIO_VOLUMES="/data{1...4}"

# MINIO_SERVER_URL sets the hostname of the local machine for use with the MinIO Server.
# MinIO assumes your network control plane can correctly resolve this hostname to the local machine.

# Uncomment the following line and replace the value with the correct hostname for the local machine.

#MINIO_SERVER_URL="http://minio.example.net"
```
其中MINIO_VOLUMES参数用于指定MinIO集群的磁盘，这里由于是单节点，所有省略了HOSTNAME。<br />Step4：启动MinIO<br />通过在节点上使用如下命令启动MinIO(如果分布式环境下，分别在不同节点执行该命令)。
```bash
systemctl start minio.service
```
命令执行后，可以使用如下命令查看minio是否启动成功
```bash
systemctl status minio.service
```
该命令运行的截图如下所示：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679275344510-ce9a4cb6-866c-4bcf-97af-cb62e0cf81d5.png#averageHue=%23111111&clientId=u98f9518e-2c06-4&from=paste&id=uc1780a58&originHeight=318&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=udec7e9df-02f9-4130-877b-99a7b40d3a2&title=)<br />其中Active(running)表示运行正常，并且可以看到Console的地址，可以在浏览器中输入console地址，打开MinIO内置的控制台：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679275344527-552f857a-0680-49ac-9b6a-1b20cfe633b8.png#averageHue=%23b4d1df&clientId=u98f9518e-2c06-4&from=paste&id=u174c4ae5&originHeight=602&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0479f7d4-56f5-4079-ab8f-a748d695283&title=)<br />用户名、密码是配置在MinIO环境变量配置文件中，输入用户名、密码即可登录。<br />当然MinIO也提供了对应mc 客户端命令访问服务端，关于运维命令，将会在后续文章介绍。<br />并基于MinIO Console成功创建了Bucket并成功上传了文件，效果如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679275344668-a06a89a0-8ac7-4110-a9df-5f3b8da942a9.png#averageHue=%23fefdfd&clientId=u98f9518e-2c06-4&from=paste&id=u6f86b3cf&originHeight=558&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc2593101-fb2b-494b-957d-f26997336a3&title=)
