得益于纠删码算法，分布式MinIO集群为多个驱动器或节点故障提供内置容错能力。根据选择的EC:N值，MinIO集群可以容忍多达一半的驱动器或节点丢失并任然提供可写可读服务。<br />下表列出了MinIO部署中的典型故障类型，以及从每种故障中恢复的过程。

| 故障类型 | 说明 |
| --- | --- |
| Drive Failure（驱动器故障 ） | MinIO支持热插拔的方式替换故障的驱动器 |
| Node Failure（节点故障） | 当新的节点重新加入到集群后，MinIO会重新开始修复该节点，原先存储在该机器上的数据会重新存储在该节点上。 |
| Site Failure（集群级故障） | MinIO 集群复制功能，支持bucket、对象、配置项的重新同步。 |

由于MinIO可以在没有显著性能损失的情况下降级运行，管理员可以根据硬件故障的比例安排硬件更换。“正常”故障率(单个驱动器或节点故障)可能允许更合理的更换时间范围，而“关键”故障率(多个驱动器或节点)可能需要更快的响应。<br />如果驱动器发生故障，只要损坏的磁盘数量低于集群的一半，可以在线直接更换驱动器，而不影响读写。<br />本文主要介绍前两种。
<a name="w6TEG"></a>
## 1、驱动器故障恢复
<a name="j4l0y"></a>
### 1.1 官方文档
MinIO支持用新的磁盘更换损坏的磁盘，而不需要重启任何MinIO节点。**MinIO确保数据的一致性和正确性。不要试图手动恢复或将数据从故障驱动器迁移到新的健康驱动器。**<br />接下来将分布介绍如何实现MinIO恢复磁盘。
<a name="ZF4B5"></a>
#### Step1：卸载故障的磁盘
可以使用umount卸载失败的驱动器，例如要卸载/dev/sdb磁盘，命令如下：
```bash
umount /dev/sdb
```
<a name="Zlv2Y"></a>
#### Step2：替换失败的驱动器
更换驱动器必须满足如下要求：

- 格式化磁盘并且磁盘数据为空,一定要求文件格式为XFS？
- 相同的硬盘类型(如HDD、SSD、NVMe)。
- 同等或更高的性能。
- 相等或更大的容量。

注意：使用容量更大的替换驱动器不会增加集群的总存储空间。MinIO使用最小驱动器的容量作为服务器池中所有驱动器的上限。<br />下面的命令将驱动器格式化，并分配一个标签永远匹配失败的驱动器：
```bash
#格式化磁盘为xfs
mkfs.xfs /dev/sdb -L DRIVE1
#格式化磁盘为ext4
mkfs.ext4 /dev/sdb -L DRIVE1
```
MinIO强烈建议使用基于标签的挂载，以确保在系统重新启动时仍然保持一致的驱动器顺序。
<a name="svHDu"></a>
#### Step3：检查和更新fstab
检查/etc/fstab文件并根据需要进行更新，使失败驱动器的条目指向新格式化的替换驱动器。

- 如果使用基于标签的驱动器分配，请确保每个标签都指向正确的新格式化驱动器。
- 如果使用基于UUID的驱动器分配，则基于新格式化的驱动器更新每个点的UUID。您可以使用lsblk来查看驱动器uuid。

可以使用如下命令进行检查：
```bash
$ cat /etc/fstab

  # <file system>  <mount point>  <type>  <options>         <dump>  <pass>
  LABEL=DRIVE1     /mnt/drive1    xfs     defaults,noatime  0       2
  LABEL=DRIVE2     /mnt/drive2    xfs     defaults,noatime  0       2
  LABEL=DRIVE3     /mnt/drive3    xfs     defaults,noatime  0       2
  LABEL=DRIVE4     /mnt/drive4    xfs     defaults,noatime  0       2
```
根据前面的示例命令，不需要对fstab进行更改，因为/mnt/drive1处的替换驱动器使用了与故障驱动器相同的drive1标签。
<a name="c0DG1"></a>
#### Step4：重新加载磁盘
使用 `mount -a` 命令加载/etc/fstab中定义的所有磁盘。
```bash
mount -a
```
<a name="DryVe"></a>
#### Step5：监控MinIO驱动器检测和修复
执行完`mount -a`后，MinIO集群会自行开始恢复。<br />使用mc管理控制台命令或`journalctl -u minio`监视重新挂载驱动器后的服务器日志输出。输出应该包括标识每个格式化驱动器和空驱动器的消息。<br />使用`mc admin heal`来监控部署中的整体修复状态。MinIO积极地修复替换的驱动器，以确保从降级状态中快速恢复。
<a name="zs8mv"></a>
### 1.2 上机实践
本次实验的MinIO集群为1NMD，集群的统计信息如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682297620683-9d4973f6-1ba9-4ce8-956b-3c88fae6d702.png#averageHue=%23fdfcfc&clientId=u12961177-5de5-4&from=paste&id=u9d55532f&originHeight=440&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u911a4643-234a-46a4-a7f4-bb444b05bd6&title=)<br />通过卸载/dev/sdb，即第一块磁盘来模拟第一块磁盘故障。<br />执行卸载磁盘命令：
```bash
[root@localhost ~]# umount /data1
[root@localhost ~]# df -h
文件系统                 容量  已用  可用 已用% 挂载点
devtmpfs                 1.9G     0  1.9G    0% /dev
tmpfs                    1.9G     0  1.9G    0% /dev/shm
tmpfs                    1.9G  8.6M  1.9G    1% /run
tmpfs                    1.9G     0  1.9G    0% /sys/fs/cgroup
/dev/mapper/centos-root   46G  1.6G   44G    4% /
/dev/sda1               1014M  151M  864M   15% /boot
tmpfs                    379M     0  379M    0% /run/user/0
/dev/sdc                 9.8G   38M  9.2G    1% /data2
/dev/sdd                 9.8G   38M  9.2G    1% /data3
/dev/sde                 9.8G   38M  9.2G    1% /data4
```
执行完后，集群的监控信息如下所示，在线磁盘驱动器由4变为3。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682297620592-7f5c2f81-e9c5-4dfd-afd8-5c6bf98c8485.png#averageHue=%23fdfcfc&clientId=u12961177-5de5-4&from=paste&id=uf1070f2a&originHeight=602&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4e163fe8-cac8-4833-89c7-0fbe478b3d5&title=)<br />在尝试进行驱动器修复之前，先来验证一下文件上传可以正常上传下载。<br />**文件下载验证：通过。如下图所示：**<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682297620602-233f1828-aa3f-464d-9d66-da385e6e9acf.png#averageHue=%23fcfbfb&clientId=u12961177-5de5-4&from=paste&id=uabe4ea97&originHeight=590&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u440cb8c8-ffde-47f1-8990-87724d1f7b6&title=)<br />**文件上传验证：通过，如下图所示：**<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682297620602-41085704-7c52-40c6-99a3-3ea787de598a.png#averageHue=%23fcfbfb&clientId=u12961177-5de5-4&from=paste&id=u9cbeae52&originHeight=560&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u45628a6d-184f-4a6e-bb02-901583c15ee&title=)
<a name="kgmcp"></a>
## 2、节点故障恢复
MinIO同样支持节点级数据恢复，也就是说一个节点上所有的磁盘损坏或数据丢失，只要满足EC:N的数据恢复要求，数据一样可以被自动修复。<br />具体的做法是跟换一台新的主机，或者是原先主机上重新放置新的磁盘（如果是磁盘损坏），然后重新安装部署一个MinIO即可，其配置与原先节点所在的Server Pool中其他节点保持一致。<br />**如果是更换的主机，请记得使用原先节点的hostname，确保正确的路由即可。**
