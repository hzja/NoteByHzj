Linux df du mknod
<a name="SbrBU"></a>
## df–显示磁盘空间使用情况
df命令的英文全称即“DiskFree”，顾名思义功能是用于显示系统上可使用的磁盘空间。默认显示单位为KB，建议使用“df -h”的参数组合，根据磁盘容量自动变换合适的单位，更利于阅读。<br />日常普遍用该命令可以查看磁盘被占用了多少空间、还剩多少空间等信息。<br />语法格式：df [参数] [指定文件]<br />常用参数：

| -a | 显示所有系统文件 |
| --- | --- |
| -B <块大小> | 指定显示时的块大小 |
| -t <文件系统类型> | 只显示指定类型的文件系统 |
| -- -sync | 在取得磁盘使用信息前，先执行sync命令 |

![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600052545770-15023496-c232-4287-9643-d7ca2ed30469.png#height=324&id=YKcjq&originHeight=971&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1200148&status=done&style=none&title=&width=1107.6666666666667)
<a name="hqHFV"></a>
### 1、检查文件系统磁盘空间使用情况
这 df 命令显示文件系统上的设备名称、总块数、总磁盘空间、已用磁盘空间、可用磁盘空间和挂载点信息。
```bash
[root@local ~]# df

Filesystem           1K-blocks      Used Available Use% Mounted on
/dev/cciss/c0d0p2     78361192  23185840  51130588  32% /
/dev/cciss/c0d0p5     24797380  22273432   1243972  95% /home
/dev/cciss/c0d0p3     29753588  25503792   2713984  91% /data
/dev/cciss/c0d0p1       295561     21531    258770   8% /boot
tmpfs                   257476         0    257476   0% /dev/shm
```
<a name="uMg0N"></a>
### 2、显示所有文件系统磁盘空间使用信息
与上面相同，但它还显示虚拟文件系统的信息以及所有文件系统磁盘使用情况及其内存使用情况。
```bash
[root@local ~]# df -a

Filesystem           1K-blocks      Used Available Use% Mounted on
/dev/cciss/c0d0p2     78361192  23186116  51130312  32% /
proc                         0         0         0   -  /proc
sysfs                        0         0         0   -  /sys
devpts                       0         0         0   -  /dev/pts
/dev/cciss/c0d0p5     24797380  22273432   1243972  95% /home
/dev/cciss/c0d0p3     29753588  25503792   2713984  91% /data
/dev/cciss/c0d0p1       295561     21531    258770   8% /boot
tmpfs                   257476         0    257476   0% /dev/shm
none                         0         0         0   -  /proc/sys/fs/binfmt_misc
sunrpc                       0         0         0   -  /var/lib/nfs/rpc_pipefs
```
<a name="Cuqoj"></a>
### 3、以人类可读的格式显示磁盘空间使用情况
有没有注意到，上面的命令以字节为单位显示信息，根本不可读，因为习惯于以兆字节、千兆字节等形式读取大小，这样很容易理解和记忆。<br />这df命令提供了一个选项来显示尺寸Human Readable格式通过使用'-h'（以人类可读的格式打印结果（例如，1K 2M 3G))。
```bash
[root@local ~]# df -h

Filesystem            Size  Used Avail Use% Mounted on
/dev/cciss/c0d0p2      75G   23G   49G  32% /
/dev/cciss/c0d0p5      24G   22G  1.2G  95% /home
/dev/cciss/c0d0p3      29G   25G  2.6G  91% /data
/dev/cciss/c0d0p1     289M   22M  253M   8% /boot
tmpfs                 252M     0  252M   0% /dev/shm
```
<a name="DeOUd"></a>
### 4、显示 / home 文件系统信息
查看唯一设备的信息/home人类可读格式的文件系统使用以下命令。
```bash
[root@local ~]# df -hT /home

Filesystem  Type    Size  Used Avail Use% Mounted on
/dev/cciss/c0d0p5 ext3     24G   22G  1.2G  95% /home
```
<a name="RtusM"></a>
### 5、以字节为单位显示文件系统信息
显示所有文件系统信息和使用情况1024-byte块，使用选项'`-k`'（例如`--block-size=1K`）如下。
```bash
[root@local ~]# df -k

Filesystem           1K-blocks      Used Available Use% Mounted on
/dev/cciss/c0d0p2     78361192  23187212  51129216  32% /
/dev/cciss/c0d0p5     24797380  22273432   1243972  95% /home
/dev/cciss/c0d0p3     29753588  25503792   2713984  91% /data
/dev/cciss/c0d0p1       295561     21531    258770   8% /boot
tmpfs                   257476         0    257476   0% /dev/shm
```
<a name="XwyOT"></a>
### 6、以MB为单位显示文件系统信息
显示所有文件系统使用信息MB(Mega Byte) 将该选项用作 `-m`。
```bash
[root@local ~]# df -m

Filesystem           1M-blocks      Used Available Use% Mounted on
/dev/cciss/c0d0p2        76525     22644     49931  32% /
/dev/cciss/c0d0p5        24217     21752      1215  95% /home
/dev/cciss/c0d0p3        29057     24907      2651  91% /data
/dev/cciss/c0d0p1          289        22       253   8% /boot
tmpfs                      252         0       252   0% /dev/shm
```
<a name="y6SuV"></a>
### 7、以 GB 为单位显示文件系统信息
显示所有文件系统统计信息GB(Gigabyte) 将该选项用作 '`df -h`'。
```bash
[root@local ~]# df -h

Filesystem            Size  Used Avail Use% Mounted on
/dev/cciss/c0d0p2      75G   23G   49G  32% /
/dev/cciss/c0d0p5      24G   22G  1.2G  95% /home
/dev/cciss/c0d0p3      29G   25G  2.6G  91% /data
/dev/cciss/c0d0p1     289M   22M  253M   8% /boot
tmpfs                 252M     0  252M   0% /dev/shm
```
<a name="EJGsU"></a>
### 8、显示文件系统 inode
使用 `-i` 开关将显示文件系统的已使用 inode 数量及其百分比信息。
```bash
[root@local ~]# df -i

Filesystem            Inodes   IUsed   IFree IUse% Mounted on
/dev/cciss/c0d0p2    20230848  133143 20097705    1% /
/dev/cciss/c0d0p5    6403712  798613 5605099   13% /home
/dev/cciss/c0d0p3    7685440 1388241 6297199   19% /data
/dev/cciss/c0d0p1      76304      40   76264    1% /boot
tmpfs                  64369       1   64368    1% /dev/shm
```
<a name="jNvkl"></a>
### 9、显示文件系统类型
如果注意到上述所有命令输出，将看到结果中没有提到Linux 文件系统类型。要检查系统的文件系统类型，请使用选项 T。它将显示文件系统类型以及其他信息。
```bash
[root@local ~]# df -T

Filesystem  Type   1K-blocks  Used      Available Use% Mounted on
/dev/cciss/c0d0p2 ext3    78361192  23188812  51127616  32%   /
/dev/cciss/c0d0p5 ext3    24797380  22273432  1243972   95%   /home
/dev/cciss/c0d0p3 ext3    29753588  25503792  2713984   91%   /data
/dev/cciss/c0d0p1 ext3    295561     21531    258770    8%    /boot
tmpfs   tmpfs   257476         0    257476    0%   /dev/shm
```
<a name="cQvdW"></a>
### 10、包括某些文件系统类型
如果要显示某些文件系统类型，请使用 `-t` 选项。例如下面的命令只会显示 ext3 文件系统。
```bash
[root@local ~]# df -t ext3

Filesystem           1K-blocks      Used Available Use% Mounted on
/dev/cciss/c0d0p2     78361192  23190072  51126356  32% /
/dev/cciss/c0d0p5     24797380  22273432   1243972  95% /home
/dev/cciss/c0d0p3     29753588  25503792   2713984  91% /data
/dev/cciss/c0d0p1       295561     21531    258770   8% /boot
```
<a name="d2t2W"></a>
### 11、排除某些文件系统类型
如果要显示不属于的文件系统类型ext3键入使用选项作为 -x。例如，以下命令将只显示除ext3。
```bash
[root@local ~]# df -x ext3

Filesystem           1K-blocks      Used Available Use% Mounted on
tmpfs                   257476         0    257476   0% /dev/shm
```
<a name="pIV29"></a>
### 12、显示 df 命令的信息。
使用'--help' 开关将显示与df命令。
```bash
[root@local ~]# df --help

Usage: df [OPTION]... [FILE]...
Show information about the file system on which each FILE resides,
or all file systems by default.

Mandatory arguments to long options are mandatory for short options too.
-a, --all             include dummy file systems
-B, --block-size=SIZE use SIZE-byte blocks
-h, --human-readable  print sizes in human readable format (e.g., 1K 234M 2G)
-H, --si              likewise, but use powers of 1000 not 1024
-i, --inodes          list inode information instead of block usage
-k                    like --block-size=1K
-l, --local           limit listing to local file systems
--no-sync         do not invoke sync before getting usage info (default)
-P, --portability     use the POSIX output format
--sync            invoke sync before getting usage info
-t, --type=TYPE       limit listing to file systems of type TYPE
-T, --print-type      print file system type
-x, --exclude-type=TYPE   limit listing to file systems not of type TYPE
-v                    (ignored)
--help     display this help and exit
--version  output version information and exit

SIZE may be (or may be an integer optionally followed by) one of following:
kB 1000, K 1024, MB 1000*1000, M 1024*1024, and so on for G, T, P, E, Z, Y.

Report bugs to <bug-coreutils@gnu.org>.
```
<a name="HmEbt"></a>
## du–查看空间
du命令的英文全称是“DiskUsage”，即用于查看磁盘占用空间的意思。但是与df命令不同的是du命令是对文件和目录磁盘使用的空间的查看，而不是某个分区。<br />语法格式：du [参数] [文件]<br />常用参数：

| -a | 显示目录中所有文件大小 |
| --- | --- |
| -k | 以KB为单位显示文件大小 |
| -m | 以MB为单位显示文件大小 |
| -g | 以GB为单位显示文件大小 |

![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600052511377-af9c5a75-c325-47c0-a722-240e456bcf96.png#height=288&id=Jlckx&originHeight=865&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1005335&status=done&style=none&title=&width=1107.6666666666667)
<a name="MZgVb"></a>
## mknod–创建字符设备文件和块设备文件
全称为“Make Node”。Linux一切皆文件，系统与设备通信之前，要建立一个存放在/dev目录下的设备文件，默认情况下就已经生成了很多设备文件，有时候自己手动新建一些设备文件，这就会用到mknod。<br />语法格式：mknod [参数]<br />常用参数：

| -Z | 设置安全的上下文 |
| --- | --- |
| -m | 设置权限模式 ，缺省为读写 |
| b | 块设备 |
| c | 字符设备 |

<a name="GR8PQ"></a>
## lsblk-查看磁盘挂载和使用情况
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600052297101-9b9c6722-af1f-46aa-b2f4-cd84c51a6c3b.png#height=108&id=g6PU8&originHeight=325&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=313827&status=done&style=none&title=&width=1107.6666666666667)
<a name="tmnqN"></a>
## findmnt 命令查找已挂载的文件系统
findmnt命令用于列出Linux中已挂载的文件系统。该命令将在/etc/fstab, /etc/mtab或者/proc/self/mountinfo中查找特定的文件系统。<br />findmnt命令包含在`util-linux`软件包中，该软件包附带了其他一些工具，例如hwclock。<br />Centos系统中使用下面命令安装：
```bash
[root@localhost ~]# yum -y install util-linux
```
这个安装包在系统中默认已安装。
<a name="yySy2"></a>
### Findmnt默认选项
如果未指定设备或者设备的挂载点 ，则默认情况下，此命令将提供所有已安装文件系统的列表。
```bash
[root@localhost ~]# findmnt
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629876983199-3f68aaae-05ee-4491-afc8-f5cf0818e4bb.png#clientId=ufd6d8d6d-afd0-4&from=paste&height=583&id=u8e1a491b&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2249572&status=done&style=none&taskId=ua97dd9e8-3a33-4983-928b-ff56d30ef49&title=&width=1107.6666666666667)
<a name="d1RWs"></a>
### Findmnt的实时监视模式
findmnt命令可用于监视/proc/self/mountinfo文件中的更改。下面实例中，挂载光盘，可以看到findmnt捕获到了挂载的动作：
```bash
[root@localhost ~]# findmnt -p
ACTION     TARGET SOURCE   FSTYPE  OPTIONS
mount      /mnt   /dev/sr0 iso9660 ro,relatime,nojoliet,check=s,map=n,blocksize=2048
```
对/proc/self/mountinfo文件所做的任何更改都将在终端上自动更新。上图中挂载或者卸载光盘，都会捕获到动作。
<a name="l4x8d"></a>
### 类似于df输出
如果使用`df`命令，则`findmnt`命令也可以用来模拟输出。可以通过使用带有`--df`选项来实现此目的，如以下示例所示：
```bash
[root@localhost ~]# findmnt --df
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629877071011-3bec6f92-793d-4336-bbb6-3e8e8fe497a3.png#clientId=ufd6d8d6d-afd0-4&from=paste&height=361&id=u7677d092&originHeight=1084&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1324817&status=done&style=none&taskId=u2ef3ee07-59cd-4d3a-86c2-b0500ebdc49&title=&width=1107.6666666666667)<br />可以看到`findmnt --df`和`df -hT`输出的信息很相似。
<a name="GAYSW"></a>
### Findmnt列表选项
如果不喜欢默认树状输出，则可以使用`--list`或`-l`选项。这将以列表格式输出。
```bash
[root@localhost ~]# findmnt --list
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629877087325-1e7c807d-4626-4e61-a42e-60b5299a3940.png#clientId=ufd6d8d6d-afd0-4&from=paste&height=583&id=ua3cd3ddc&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2245831&status=done&style=none&taskId=u994169a6-ba32-4e6f-9947-481421e0cde&title=&width=1107.6666666666667)
<a name="qsCd3"></a>
### 从/etc/fstab文件中列出挂载信息
使用`--fstab`选项，将会从`/etc/fstab`文件中获取挂载信息。<br />`--evaluate`选项会将所有标签（例如LABEL，UUID，PARTUUID或PARTLABEL）转换为实际设备名称。该选项可以与`--fstab`选项结合使用，以打印文件中的所有文件系统。fstab选项以列表格式输出，因此使用`--fstab`时无需调用`--list`选项。
```bash
[root@iZuf671hry2744olton71qZ ~]# findmnt --fstab
TARGET SOURCE                                    FSTYPE OPTIONS
/      UUID=1114fe9e-2309-4580-b183-d778e6d97397 ext4   defaults
[root@iZuf671hry2744olton71qZ ~]# findmnt --fstab --evaluate
TARGET SOURCE    FSTYPE OPTIONS
/      /dev/vda1 ext4   defaults
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629877210351-413f9987-8eb4-454a-8bbc-d88ed16131e0.png#clientId=ufd6d8d6d-afd0-4&from=paste&height=144&id=ue339b3bf&originHeight=432&originWidth=2034&originalType=binary&ratio=1&rotation=0&showTitle=false&size=291944&status=done&style=none&taskId=u55b966a4-10e4-4515-87f0-e5dffdec602&title=&width=678)
<a name="TDPkl"></a>
### 显示指定类型的文件系统
此选项可以限制打印文件系统的集合。可以指定多个文件系统，并以逗号分隔。在此示例中，该命令将搜索ext4，xfs和vfat文件系统。
```bash
[root@localhost ~]# findmnt -t ext4,xfs,vfat
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629877225199-b5792e74-5ffd-4225-8775-2a5cc97b6d97.png#clientId=ufd6d8d6d-afd0-4&from=paste&height=126&id=u2cc5ac01&originHeight=379&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=415684&status=done&style=none&taskId=uee5593c5-7b51-4032-92c7-600fd8e4bef&title=&width=1107.6666666666667)
<a name="jX2nh"></a>
### 总结
可以根据需要尝试不同的findmnt选项。`findmnt`命令用于列出Linux中已挂载的文件系统。该命令将在/etc/fstab, /etc/mtab或者/proc/self/mountinfo中查找特定的文件系统。
