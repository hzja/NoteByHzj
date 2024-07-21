<a name="5ad6d630"></a>
## 什么是MFS？

> MooseFS是一个容错，高可用，高性能，扩展，网络分布式文件系统。它将数据分布在多个物理商品服务器上，这些服务器对用户可视为一个虚拟磁盘。它符合POSIX并且像任何其他类Unix文件系统一样支持：
> - 分层结构：文件和文件夹，
> - 文件属性，
> - 特殊文件：管道，插座，块和字符设备，
> - 符号和硬链接，
> - 安全属性和ACL。<br />它适用于需要标准文件系统的所有应用程序。


<a name="bebe3d3f"></a>
## 分布式原理

> 分布式文件系统是指文件系统管理的物理存储资源不一定直接连接在本地节点上，而是通过计算机网络与节点相连。简单来说，就是把一些分散的（分布在局域网内各个计算机上）共享文件夹，集合到一个文件夹内（虚拟共享文件夹）。对于用户来说，要访问这些共享文件时，只要打开这个虚拟共享文件夹，就可以看到所有链接到虚拟共享文件夹内的共享文件夹，用户感觉不到这些共享文件是分布于各个计算机上的。分布式文件系统的好处是集中访问、简化操作、数据容灾，以及提高文件的存取性能。


<a name="06fa7e65"></a>
## MFS原理

MFS是一个具有容错性的网络分布式文件系统，它把数据分散存放在多个物理服务器上，而呈现给用户的则是统一的资源。

<a name="13aa255a"></a>
### MFS文件系统的组成

- 元数据服务器（Master）：在整个体系中负责管理文件系统，维护元数据。
- 元数据日志服务器（Metalogger）：备份Master服务器的变化日志文件，文件类型为changelog_ml.*.mfs。当Master服务器数据丢失或损坏时，可以从日志服务器中取得文件，进行恢复。
- 数据存储服务器（Chunk Server）：真正存储数据的服务器。存储文件时，会把文件分块保存，并在数据服务器间进行复制。数据服务器越多，能使用的“容量”也越大，可靠性越高，性能也就越好。
- 客户端（Client）：可以直接挂载MFS文件系统。

<a name="975f3e39"></a>
### MFS读取数据的处理过程

- 客户端向元数据服务器发出读请求
- 元数据服务器把所需数据存放的位置（Chunk Server的IP地址和Chunk编号）告知客户端
- 客户端向已知的Chunk Server请求发送数据
- Chunk Server向客户端发送数据

<a name="9148c33c"></a>
### MFS写入数据的处理过程

- 客户端向元数据服务器发送写入请求
- 元数据服务器与Chunk Server进行交互（只有当所需的分块Chunk存在的时候才进行交互），但元数据服务器只在某些服务器创建新的分块chunks，创建成功后由Chunk Servers告知元数据服务器操作成功
- 元数据服务器告知客户端，可以在哪个Chunk Server的那些chunks写入数据
- 客户端向指定的Chunk Server写入数据
- 该Chunk Server与其他Chunk Server进行数据同步，同步成功后Chunk Server告知客户端数据写入成功
- 客户端告知元数据服务器本次写入完毕

<a name="1501300b"></a>
## MFS优势

- 高可用性：没有单点故障也称为无SPOF配置。文件系统的元数据在物理冗余服务器上保存为两个或多个副本。用户数据冗余地分布在系统中的存储服务器上。
- 低成本的数据安全：MooseFS使用户能够节省大量硬盘空间，保持相同的数据冗余级别。在大多数常见情况下，将节省至少55％的硬盘空间.
- 可扩展性：存储可以扩展到16字节（~16000千兆字节），这使我们可以存储超过20亿个文件
- 高性能：旨在支持高性能I/O操作。用户数据可以在许多存储节点上同时读取/写入，从而避免单个中央服务器或单个网络连接瓶颈。
<a name="oNHJk"></a>
## MFS集群配置
| 名称 | ip地址 |
| --- | --- |
| master | 192.168.40.11 |
| MetalLogger | 192.168.40.12 |
| Server1 | 192.168.40.23 |
| Server2 | 192.168.40.24 |
| client | 192.168.40.25 |


官方源： [https://moosefs.com/download/](https://moosefs.com/download/)

<a name="D1xhH"></a>
### 一、前提准备

每个节点执行

```bash
[root@master ~]# systemctl stop firewall.service
[root@master ~]# setenforce 0
[root@master ~]#
[root@master ~]# curl "https://ppa.moosefs.com/RPM-GPG-KEY-MooseFS" > /etc/pki/rpm-gpg/RPM-GPG-KEY-MooseFS
[root@master ~]# curl "http://ppa.moosefs.com/MooseFS-3-el7.repo" > /etc/yum.repos.d/MooseFS.repo
```

<a name="80baG"></a>
### 二、master

```bash
[root@master ~]# yum install moosefs-master moosefs-cgi moosefs-cgiserv moosefs-cli -y
[root@master ~]# systemctl start moosefs-master
[root@master ~]# systemctl enable moosefs-master
Created symlink from /etc/systemd/system/multi-user.target.wants/moosefs-master.service to /usr/lib/systemd/system/moosefs-master.service.
[root@master ~]# netstat -ntap | grep mfs
tcp        0      0 0.0.0.0:9419            0.0.0.0:*               LISTEN      6527/mfsmaster
tcp        0      0 0.0.0.0:9420            0.0.0.0:*               LISTEN      6527/mfsmaster
tcp        0      0 0.0.0.0:9421            0.0.0.0:*               LISTEN      6527/mfsmaster
[root@master ~]#
```

<a name="mxt2b"></a>
### 三、搭建MetalLogger Server

```bash
[root@metallogger ~]# yum install moosefs-metalogger -y
[root@metallogger ~]# vim /etc/mfs/mfsmetalogger.cfg
[root@metallogger ~]# grep -i "master_host" /etc/mfs/mfsmetalogger.cfg
MASTER_HOST = 192.168.40.11
[root@metallogger ~]# systemctl start moosefs-metalogger
[root@metallogger ~]# systemctl enable moosefs-metalogger
Created symlink from /etc/systemd/system/multi-user.target.wants/moosefs-metalogger.service to /usr/lib/systemd/system/moosefs-metalogger.service.
[root@metallogger ~]# netstat -ntap | grep mfs
tcp        0      0 192.168.40.12:35318     192.168.40.11:9419      ESTABLISHED 4892/mfsmetalogger
[root@metallogger ~]#
```

<a name="msUfJ"></a>
### 四、搭建Chunk Server

项目中的两台Chunk Server的搭建步骤是完全相同

```bash
[root@server1 ~]# yum install moosefs-chunkserver -y
[root@server1 ~]# grep -i "master_host" /etc/mfs/mfschunkserver.cfg
MASTER_HOST = 192.168.40.11
[root@server1 ~]# tail -1 /etc/mfs/mfshdd.cfg
/jiang
[root@server1 ~]# mkdir /jiang
[root@server1 ~]# chown mfs:mfs /jiang
[root@server1 ~]# systemctl start moosefs-chunkserver
[root@server1 ~]# systemctl enable moosefs-chunkserver
Created symlink from /etc/systemd/system/multi-user.target.wants/moosefs-chunkserv                                                                                                       er.service to /usr/lib/systemd/system/moosefs-chunkserver.service.
[root@server1 ~]# netstat -ntap | grep mfs
tcp        0      0 0.0.0.0:9422            0.0.0.0:*               LISTEN      22                                                                                                       770/mfschunkserve
tcp        0      0 192.168.40.23:47333     192.168.40.11:9420      ESTABLISHED 22                                                                                                       770/mfschunkserve
[root@server1 ~]#
```

备注：可以添加硬盘分区格式化将磁盘挂载到目录下 /etc/mfs/mfshdd.cfg指定挂载的文件路径，这样就可以实现底层的存储设备支撑mfs集群的存储。
<a name="gO1oV"></a>
### 五、client

```bash
[root@client ~]# yum install moosefs-client -y
[root@client ~]# modprobe fuse
[root@client ~]# mkdir /mfsclient -p
[root@client ~]# mfsmount /mfsclient  -H 192.168.40.11
[root@client ~]# df -hT
df: ‘/mfs/jiang’: Transport endpoint is not connected
Filesystem              Type      Size  Used Avail Use% Mounted on
/dev/mapper/centos-root xfs        36G  2.3G   34G   7% /
devtmpfs                devtmpfs  1.9G     0  1.9G   0% /dev
tmpfs                   tmpfs     1.9G     0  1.9G   0% /dev/shm
tmpfs                   tmpfs     1.9G  8.5M  1.9G   1% /run
tmpfs                   tmpfs     1.9G     0  1.9G   0% /sys/fs/cgroup
/dev/sda1               xfs       497M  125M  373M  26% /boot
tmpfs                   tmpfs     378M     0  378M   0% /run/user/0
tmpfs                   tmpfs     1.9G  156K  1.9G   1% /tmp
192.168.40.11:9421      fuse.mfs   73G  9.6G   64G  14% /mfsclient
[root@client ~]#
[root@client ~]# cd /mfsclient/
[root@client mfsclient]# dd if=/dev/zero of=mfs count=1M bs=10
1048576+0 records in
1048576+0 records out
10485760 bytes (10 MB) copied, 12.1155 s, 865 kB/s
[root@client mfsclient]# mfsfileinfo mfs
mfs:
        chunk 0: 0000000000000013_00000001 / (id:19 ver:1)
                copy 1: 192.168.40.23:9422 (status:VALID)
                copy 2: 192.168.40.24:9422 (status:VALID)

```

**备注：**可以看到创建的集群文件在每一台server机器上传都有存储，这个是默认的方式，可以通过修改配置文件改变文件的默认副本数。
<a name="APjok"></a>
### 六、master节点开启web视图

```bash
[root@master ~]# mfscgiserv
lockfile created and locked
starting simple cgi server (host: any , port: 9425 , rootpath: /usr/share/mfscgi)
[root@master ~]#
```

![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599830228728-36b46368-fbe0-40aa-adb9-0982ed8260f5.png#align=left&display=inline&height=101&originHeight=202&originWidth=997&size=61549&status=done&style=none&width=498.5)

<a name="e83Ky"></a>
### 七、MFS维护操作

```bash
[root@client jiang]# mfsfileinfo 1.txt
1.txt:
        chunk 0: 0000000000000002_00000001 / (id:2 ver:1)
                copy 1: 192.168.40.23:9422 (status:VALID)
                copy 2: 192.168.40.24:9422 (status:VALID)
[root@client jiang]#
```

<a name="pTMZT"></a>
### 八、回收机制演示
<a name="gM3iB"></a>
####  回收站机制
```bash
[root@client mfsclient]# cat mfs.txt
这个文件是测试mfs的回收站机制
[root@client mfsclient]# rm -rf mfs.txt
[root@client mfsclient]# cd
[root@client ~]# mkdir /mfsback
[root@client ~]# mfsmount -m /mfsback -H 192.168.40.11
mfsmaster accepted connection with parameters: read-write,restricted_ip
[root@client ~]# cd /mfsback/
[root@client mfsback]# ls
sustained  trash
[root@client mfsback]# yum install -y -q tree
Package tree-1.6.0-10.el7.x86_64 already installed and latest version
[root@client mfsback]# cd trash/
[root@client trash]# tree . > /tmp/mfs.txt
[root@client trash]# grep "mfs.txt" /tmp/mfs.txt
│   ├── 0000006D|mfs.txt~
│   ├── 0000006E|.mfs.txt.swp
│   ├── 0000006F|.mfs.txt.swx
│   ├── 00000070|.mfs.txt.swp
│   ├── 00000072|mfs.txt
[root@client trash]# mv 072/00000072\|mfs.txt undel/
[root@client trash]# ll /mfsclient/
total 10241
-rw-r--r--. 1 root root 10485760 Sep 11  2020 mfs
-rw-r--r--. 1 root root       43 Sep 11  2020 mfs.txt
[root@client trash]# cat /mfsclient/mfs.txt
这个文件是测试mfs的回收站机制
[root@client trash]#
```
**备注：**基本上的恢复文件都是查看trash文件中的信息通过tree命令+重定向到执行文件下找对应的文件信息，然后将它移动到undel文件即可恢复文件信息<br />**注意：**并不是所有的文件都能恢复，默认的误删除时间是86400秒也就是一天
```bash
[root@client trash]# mfsgettrashtime /mfsclient/mfs.txt
/mfsclient/mfs.txt: 86400
[root@client trash]#
[root@client trash]# mfssettrashtime 8640000 /mfsclient/mfs.txt  ##修改默认回收时间
/mfsclient/mfs.txt: 8640000
[root@client trash]# mfsgettrashtime /mfsclient/mfs.txt
/mfsclient/mfs.txt: 8640000
[root@client trash]#
```
<a name="OWKIZ"></a>
#### 快照
与复制的区别是他不会占用两份的存储空间，相当于是一个软连接
```bash
[root@client mfsclient]# mfsmakesnapshot mfs.txt back/
[root@client mfsclient]# cd back/
[root@client back]# cat mfs.txt
这个文件是测试mfs的回收站机制

```
<a name="NiP3S"></a>
#### 副本数
```bash
[root@client mfsclient]# mfssetgoal 1 mfs3.txt
mfs3.txt: goal: 1
[root@client mfsclient]# mfsfileinfo mfs3.txt
mfs3.txt:
        chunk 0: 0000000000000017_00000001 / (id:23 ver:1)
                copy 1: 192.168.40.24:9422 (status:VALID)
[root@client mfsclient]# mfssetgoal 2 mfs3.txt
mfs3.txt: goal: 2
[root@client mfsclient]# mfsfileinfo mfs3.txt
mfs3.txt:
        chunk 0: 0000000000000017_00000001 / (id:23 ver:1)
                copy 1: 192.168.40.23:9422 (status:VALID)
                copy 2: 192.168.40.24:9422 (status:VALID)
[root@client mfsclient]#
```
<a name="EOOHT"></a>
#### 高可用测试
```bash
[root@server1 ~]# mfschunkserver stop
sending SIGTERM to lock owner (pid:30549)
waiting for termination terminated
[root@server1 ~]#
[root@client mfsclient]# cat mfs.txt
这个文件是测试mfs的回收站机制
[root@client mfsclient]# cat mfs3.txt
jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj
[root@client mfsclient]#
[root@server1 ~]# mfschunkserver stop
sending SIGTERM to lock owner (pid:30549)
waiting for termination terminated
[root@server1 ~]#
[root@server1 ~]# mfschunkserver start
open files limit has been set to: 16384
working directory: /var/lib/mfs
lockfile created and locked
setting glibc malloc arena max to 4
setting glibc malloc arena test to 4
initializing mfschunkserver modules ...
hdd space manager: path to scan: /jiang/
hdd space manager: start background hdd scanning (searching for available chunks)
main server module: listen on *:9422
stats file has been loaded
mfschunkserver daemon initialized properly
[root@server1 ~]#
```
总结：在mfs集群当中其中一台存储server宕机是不会影响服务的中断
<a name="NDhC2"></a>
#### 
<a name="0cKhB"></a>
#### master宕机演示

停止master节点
```bash
[root@master mfs]# mfsmaster stop
sending SIGTERM to lock owner (pid:4750)
waiting for termination terminated
[root@master mfs]#
```
客户端查看
```bash
[root@client mfsclient]# ls
                               #直接处于僵死的状态是不能做任何的操作
```
开启master节点
```bash
[root@master mfs]# mfsmaster start
open files limit has been set to: 16384
working directory: /var/lib/mfs
lockfile created and locked
initializing mfsmaster modules ...
exports file has been loaded
topology file has been loaded
loading metadata ...
loading sessions data ... ok (0.0000)
loading storage classes data ... ok (0.0000)
loading objects (files,directories,etc.) ... ok (0.0289)
loading names ... ok (0.1340)
loading deletion timestamps ... ok (0.0000)
loading quota definitions ... ok (0.0000)
loading xattr data ... ok (0.0000)
loading posix_acl data ... ok (0.0000)
loading open files data ... ok (0.0000)
loading flock_locks data ... ok (0.0000)
loading posix_locks data ... ok (0.0000)
loading chunkservers data ... ok (0.0000)
loading chunks data ... ok (0.0353)
checking filesystem consistency ... ok
connecting files and chunks ... ok
all inodes: 20
directory inodes: 2
file inodes: 18
chunks: 23
metadata file has been loaded
stats file has been loaded
master <-> metaloggers module: listen on *:9419
master <-> chunkservers module: listen on *:9420
main master server module: listen on *:9421
mfsmaster daemon initialized properly

```
客户端访问
```bash
[root@client mfsclient]# ls
back  mfs  mfs3.txt  mfs.txt
[root@client mfsclient]#
```
<a name="XiRNt"></a>
##### 模拟故障发生
```bash
[root@master mfs]# mfsmaster stop
sending SIGTERM to lock owner (pid:22934)
waiting for termination terminated
[root@master mfs]# pwd
/var/lib/mfs
[root@master mfs]# rm -rf *
[root@master mfs]# ll
total 0
```
<a name="XwZHR"></a>
##### metallogger
```bash
[root@metallogger ~]# cd /var/lib/mfs/
[root@metallogger mfs]# ls
changelog_ml.0.mfs  changelog_ml.1.mfs  changelog_ml.4.mfs  changelog_ml.5.mfs  changelog_ml_back.0.mfs  changelog_ml_back.1.mfs  metadata_ml.mfs.back
[root@metallogger mfs]# scp * root@192.168.40.11:`pwd`
root@192.168.40.11's password:
changelog_ml.0.mfs                                                                                                                                     100% 5176     6.5MB/s   00:00
changelog_ml.1.mfs                                                                                                                                     100% 5395     6.8MB/s   00:00
changelog_ml.4.mfs                                                                                                                                     100% 1021    42.2KB/s   00:00
changelog_ml.5.mfs                                                                                                                                     100%   12KB   6.4MB/s   00:00
changelog_ml_back.0.mfs                                                                                                                                100%    0     0.0KB/s   00:00
changelog_ml_back.1.mfs                                                                                                                                100%    0     0.0KB/s   00:00
metadata_ml.mfs.back                                                                                                                                   100% 4163     5.9MB/s   00:00
[root@metallogger mfs]#
```
<a name="LKUAg"></a>
##### master
```bash
[root@master mfs]# ls
changelog_ml.0.mfs  changelog_ml.1.mfs  changelog_ml.4.mfs  changelog_ml.5.mfs  changelog_ml_back.0.mfs  changelog_ml_back.1.mfs  metadata_ml.mfs.back
[root@master mfs]# mfsmaster -a
open files limit has been set to: 16384
working directory: /var/lib/mfs
lockfile created and locked
initializing mfsmaster modules ...
exports file has been loaded
topology file has been loaded
loading metadata ...
loading sessions data ... ok (0.0000)
loading storage classes data ... ok (0.0000)
loading objects (files,directories,etc.) ... ok (0.0321)
loading names ... ok (0.0314)
loading deletion timestamps ... ok (0.0000)
loading quota definitions ... ok (0.0000)
loading xattr data ... ok (0.0000)
loading posix_acl data ... ok (0.0000)
loading open files data ... ok (0.0000)
loading flock_locks data ... ok (0.0000)
loading posix_locks data ... ok (0.0000)
loading chunkservers data ... ok (0.0000)
loading chunks data ... ok (0.0557)
checking filesystem consistency ... ok
connecting files and chunks ... ok
all inodes: 6
directory inodes: 1
file inodes: 5
chunks: 2
metadata file has been loaded
no charts data file - initializing empty charts
master <-> metaloggers module: listen on *:9419
master <-> chunkservers module: listen on *:9420
main master server module: listen on *:9421
mfsmaster daemon initialized properly
[root@master mfs]#
```
客户端访问
```bash
[root@client mfsclient]# cat mfs3.txt
jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj
[root@client mfsclient]#
```

