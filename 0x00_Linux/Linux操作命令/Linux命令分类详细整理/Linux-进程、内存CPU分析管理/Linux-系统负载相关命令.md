Linux 
<a name="rL9Hk"></a>
## w—查看登录者的信息、系统负载等信息
<a name="diAFe"></a>
### 命令简介
w 命令是一个非常常用的命令，用来查看登录者的信息及他们的行为动作、系统负载等信息。
<a name="vrMjo"></a>
### 语法格式
```
w [OPTIONS][用户名称]
```
<a name="2ef0fb62"></a>
### 选项说明
```
-f  #开启或关闭显示用户从何处登录到系统
-h  #不显示各栏位的标题信息列
-l  #使用详细格式列表（默认）
-s  #使用简洁格式列表
-u  #忽略执行程序的名称，以及该程序耗费CPU时间的信息
-V  #打印版本信息
```
<a name="2wLjr"></a>
### 应用举例
```bash
#显示当前用户及系统负载信息
[root@centos7 ~]# w
 08:48:55 up 1 day, 10:02,  2 users,  load average: 0.09, 0.04, 0.05
USER     TTY      FROM             LOGIN@   IDLE   JCPU   PCPU WHAT
root     pts/0    192.168.1.93     08:26   20:15   0.04s  0.04s -bash
root     pts/1    192.168.1.93     08:32    7.00s  0.25s  0.18s w
#不显示用户登录位置信息
[root@centos7 ~]# w -f
 08:48:58 up 1 day, 10:02,  2 users,  load average: 0.08, 0.04, 0.05
USER     TTY        LOGIN@   IDLE   JCPU   PCPU WHAT
root     pts/0     08:26   20:18   0.04s  0.04s -bash
root     pts/1     08:32    2.00s  0.08s  0.01s w -f
#以精简格式显示
[root@centos7 ~]# w -s
 08:49:12 up 1 day, 10:03,  2 users,  load average: 0.06, 0.04, 0.05
USER     TTY      FROM              IDLE WHAT
root     pts/0    192.168.1.93     20:32  -bash
root     pts/1    192.168.1.93      0.00s w -s
#不显示列的标题信息
[root@centos7 ~]# w -h
root     pts/0    192.168.1.93     08:26   20:36   0.04s  0.04s -bash
root     pts/1    192.168.1.93     08:32    4.00s  0.07s  0.00s w -h
```
<a name="HcT4z"></a>
## uptime—显示系统运行时间及负载
<a name="SFTMo"></a>
### 命令简介
uptime命令用于显示系统运行时间及负载。<br />uptime 命令可以打印出系统总共运行了多长时间和系统的平均负载。uptime 命令显示的信息显示依次为：现在时间、系统已经运行了多长时间、目前有多少登陆用户、系统在过去的 1 分钟、5 分钟和 15 分钟内的平均负载。
```bash
[root@centos7 ~]# uptime
 19:48:14 up 21:02,  1 user,  load average: 0.00, 0.01, 0.05
```
<a name="PfWyJ"></a>
### 命令语法
```
uptime [options]
```
<a name="h6gmv"></a>
### 选项说明
```
-p #以好看的格式显示正常运行时间
-s #自系统启动来的时间
-h #打印帮助信息
-V #打印版本信息。
```
<a name="3QFDI"></a>
### 应用举例
```bash
[root@centos7 ~]# uptime -V
uptime from procps-ng 3.3.10
[root@centos7 ~]# uptime 
 19:50:29 up 21:04,  1 user,  load average: 0.03, 0.03, 0.05
 
[root@centos7 ~]# uptime -p
up 21 hours, 4 minutes
[root@centos7 ~]# uptime -s
2021-01-15 22:46:06
[root@centos7 ~]# uptime -h
Usage:
 uptime [options]
Options:
 -p, --pretty   show uptime in pretty format
 -h, --help     display this help and exit
 -s, --since    system up since
 -V, --version  output version information and exit
For more details see uptime(1).
```
文件存放目录
```bash
/var/run/utmp #当前登录者的信息
[root@centos7 ~]# cat /var/run/utmp
~~~reboot3.10.0-1127.18.2.el7.x86_64󞞗/vtty1tty1LOGINv `S3~~~runlevel3.10.0-1127.18.2.el7.x86_64 `pts
```
<a name="wQ5om"></a>
## dmesg—显示系统开机信息
<a name="ZQ4GW"></a>
### 命令简介
dmesg 命令用于显示系统开机信息，可用于诊断系统故障。<br />内核会将系统开机信息存储在ring buffer中，可以使用dmesg命令来查看，开机信息保存在/var/log/dmesg文件中。
<a name="miWxh"></a>
### 命令语法
```
dmesg [options]
```
<a name="lPXu5"></a>
### 选项说明
```
-c  #显示信息后，清除ring buffer中的内容
-s<缓冲区大小>  #默认值为8196，刚好等于ring buffer的大小
-n  #设置记录信息的层级
-D  #禁用打印消息到控制台
-E  #启用打印消息到控制台
-h  #打印帮助文本并退出
-k  #打印内核消息
-n  #设置将消息记录到控制台的级别
-r  #打印原始消息缓冲区
-s  #使用多少大小的缓冲区来查询内核环缓冲区。 默认情况下为16392
-T  #打印人类可读时间戳
-t  #不打印内核的时间戳
-u  #打印用户空间消息
-V  #输出版本信息并退出
-x  #将设施和级别（优先级）编号解码为可读的前缀
```
<a name="179mr"></a>
### 应用举例
<a name="JF6fb"></a>
#### 查看前20行开机信息
```bash
[root@centos7 ~]# dmesg | head -n 20
[    0.000000] Initializing cgroup subsys cpuset
[    0.000000] Initializing cgroup subsys cpu
[    0.000000] Initializing cgroup subsys cpuacct
[    0.000000] Linux version 3.10.0-1127.18.2.el7.x86_64 (mockbuild@kbuilder.bsys.centos.org) (gcc version 4.8.5 20150623 (Red Hat 4.8.5-39) (GCC) ) #1 SMP Sun Jul 26 15:27:06 UTC 2020
[    0.000000] Command line: BOOT_IMAGE=/vmlinuz-3.10.0-1127.18.2.el7.x86_64 root=/dev/mapper/centos-root ro crashkernel=auto spectre_v2=retpoline rd.lvm.lv=centos/root rd.lvm.lv=centos/swap rhgb quiet LANG=en_US.UTF-8
[    0.000000] Disabled fast string operations
[    0.000000] e820: BIOS-provided physical RAM map:
[    0.000000] BIOS-e820: [mem 0x0000000000000000-0x000000000009ebff] usable
[    0.000000] BIOS-e820: [mem 0x000000000009ec00-0x000000000009ffff] reserved
[    0.000000] BIOS-e820: [mem 0x00000000000dc000-0x00000000000fffff] reserved
[    0.000000] BIOS-e820: [mem 0x0000000000100000-0x000000003fedffff] usable
[    0.000000] BIOS-e820: [mem 0x000000003fee0000-0x000000003fefefff] ACPI data
[    0.000000] BIOS-e820: [mem 0x000000003feff000-0x000000003fefffff] ACPI NVS
[    0.000000] BIOS-e820: [mem 0x000000003ff00000-0x000000003fffffff] usable
[    0.000000] BIOS-e820: [mem 0x00000000f0000000-0x00000000f7ffffff] reserved
[    0.000000] BIOS-e820: [mem 0x00000000fec00000-0x00000000fec0ffff] reserved
[    0.000000] BIOS-e820: [mem 0x00000000fee00000-0x00000000fee00fff] reserved
[    0.000000] BIOS-e820: [mem 0x00000000fffe0000-0x00000000ffffffff] reserved
[    0.000000] NX (Execute Disable) protection: active
[    0.000000] SMBIOS 2.4 present.
```
<a name="JcrEw"></a>
#### 查看与内存相关的开机信息
```bash
[root@centos7 ~]# dmesg | grep -i memory
[    0.000000] Base memory trampoline at [ffff9102c0098000] 98000 size 24576
[    0.000000] crashkernel=auto resulted in zero bytes of reserved memory.
[    0.000000] Early memory node ranges
[    0.000000] PM: Registered nosave memory: [mem 0x0009e000-0x0009efff]
[    0.000000] PM: Registered nosave memory: [mem 0x0009f000-0x0009ffff]
[    0.000000] PM: Registered nosave memory: [mem 0x000a0000-0x000dbfff]
[    0.000000] PM: Registered nosave memory: [mem 0x000dc000-0x000fffff]
[    0.000000] PM: Registered nosave memory: [mem 0x3fee0000-0x3fefefff]
[    0.000000] PM: Registered nosave memory: [mem 0x3feff000-0x3fefffff]
[    0.000000] Memory: 972140k/1048576k available (7784k kernel code, 524k absent, 75912k reserved, 5958k data, 1980k init)
[    0.000000] please try 'cgroup_disable=memory' option if you don't want memory cgroups
[    0.755288] Initializing cgroup subsys memory
[    1.619645] x86/mm: Memory block size: 128MB
[    3.669071] Freeing initrd memory: 20628k freed
[    3.933907] Non-volatile memory driver v1.3
[    3.935079] crash memory driver: version 1.1
[    4.000261] Freeing unused kernel memory: 1980k freed
[    4.001692] Freeing unused kernel memory: 396k freed
[    4.003171] Freeing unused kernel memory: 540k freed
[    5.956205] [drm] Max dedicated hypervisor surface memory is 0 kiB
[    5.956206] [drm] Maximum display memory size is 32768 kiB
[    5.968934] [TTM] Zone  kernel: Available graphics memory: 497842 kiB
```
<a name="fMPYR"></a>
#### 查看与磁盘相关的开机信息
```bash
[root@centos7 ~]# dmesg | grep -i disk
[    0.000000] RAMDISK: [mem 0x357a7000-0x36bcbfff]
[    3.738914] VFS: Disk quotas dquot_6.5.2
[    4.013333] systemd[1]: Running in initial RAM disk.
[    6.295432] sd 0:0:0:0: [sda] Attached SCSI disk
[root@centos7 ~]# dmesg | grep -i sda
[    6.291698] sd 0:0:0:0: [sda] 41943040 512-byte logical blocks: (21.4 GB/20.0 GiB)
[    6.291830] sd 0:0:0:0: [sda] Write Protect is off
[    6.291831] sd 0:0:0:0: [sda] Mode Sense: 61 00 00 00
[    6.292032] sd 0:0:0:0: [sda] Cache data unavailable
[    6.292033] sd 0:0:0:0: [sda] Assuming drive cache: write through
[    6.294046]  sda: sda1 sda2
[    6.295432] sd 0:0:0:0: [sda] Attached SCSI disk
[   15.843965] XFS (sda1): Mounting V5 Filesystem
[   16.778908] XFS (sda1): Starting recovery (logdev: internal)
[   16.801659] XFS (sda1): Ending recovery (logdev: internal)
```
