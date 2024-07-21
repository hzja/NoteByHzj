Linux
<a name="t64lk"></a>
## 一、使用 NTP 服务时间同步
<a name="i9VGO"></a>
### 安装 ntp
```bash
[root@node ~]# yum -y install ntp
```
<a name="ACxyI"></a>
### 启动 ntp 服务
```bash
[root@node ~]# systemctl start  ntpd
[root@node ~]# systemctl enable  ntpd
Created symlink from /etc/systemd/system/multi-user.target.wants/ntpd.service to /usr/lib/systemd/system/ntpd.service.
```
<a name="qKk4J"></a>
### 查看 ntp 服务
```bash
[root@node ~]# systemctl status ntpd
● ntpd.service - Network Time Service
   Loaded: loaded (/usr/lib/systemd/system/ntpd.service; enabled; vendor preset: disabled)
   Active: active (running) since Thu 2022-08-04 08:00:47 CST; 7h left
 Main PID: 20746 (ntpd)
    Tasks: 1
   Memory: 768.0K
   CGroup: /system.slice/ntpd.service
           └─20746 /usr/sbin/ntpd -u ntp:ntp -g

Aug 04 08:00:47 node ntpd[20746]: Listen normally on 6 lo ::1 UDP 123
Aug 04 08:00:47 node ntpd[20746]: Listen normally on 7 ens33 fe80::8a83:7297:7cb8:9934 UDP 123
Aug 04 08:00:47 node ntpd[20746]: Listening on routing socket on fd #24 for interface updates
Aug 04 08:00:47 node systemd[1]: Started Network Time Service.
Aug 04 08:00:47 node ntpd[20746]: 0.0.0.0 c016 06 restart
Aug 04 08:00:47 node ntpd[20746]: 0.0.0.0 c012 02 freq_set kernel 0.000 PPM
Aug 04 08:00:47 node ntpd[20746]: 0.0.0.0 c011 01 freq_not_set
Aug 04 08:00:56 node ntpd[20746]: 0.0.0.0 c61c 0c clock_step -28798.353463 s
Aug 04 00:00:57 node ntpd[20746]: 0.0.0.0 c614 04 freq_mode
Aug 04 00:00:58 node ntpd[20746]: 0.0.0.0 c618 08 no_sys_peer
```
<a name="nqRfK"></a>
### 修改 ntp.conf 文件
```bash
[root@node ~]# vim /etc/ntp.conf
[root@node ~]# grep server /etc/ntp.conf 
# Use public servers from the pool.ntp.org project.
#server 0.centos.pool.ntp.org iburst
server ntp.aliyun.com iburst
#broadcast 192.168.1.255 autokey # broadcast server
#broadcast 224.0.1.1 autokey  # multicast server
#manycastserver 239.255.254.254  # manycast server
```
<a name="MgbOg"></a>
### 重启服务
```bash
[root@node ~]# systemctl restart ntpd
```
<a name="XU0ol"></a>
### 检查同步状态
```bash
[root@node ~]# ntpq -p
remote           refid      st t when poll reach   delay   offset  jitter
==============================================================================
203.107.6.88    100.107.25.114   2 u    2   64    1   30.975  149.499   0.393
```
<a name="vG8Y9"></a>
## 二、使用 ntpdate 同步
<a name="cZ40M"></a>
### 使用 ntpd 命令同步
```bash
[root@node ~]# ntpdate ntp.aliyun.com
 4 Aug 00:07:17 ntpdate[20924]: adjust time server 203.107.6.88 offset -0.001543 sec
```
<a name="Lhqqw"></a>
### 查看时间
```bash
[root@node ~]# date
Thu Aug  4 00:07:46 CST 2022
```
<a name="Eucxd"></a>
## 三、处理 ntpdate 同步失败报错方法
<a name="UWwy7"></a>
### 报错信息
```bash
cna02:~ # ntpdate ntp1.aliyun.com
4 Aug 07:23:41 ntpdate[18694]: the NTP socket is in use, exiting
```
<a name="zh9h2"></a>
### 处理方法
```bash
cna02:~ # service ntpd stop
Redirecting to /bin/systemctl stop ntpd.service
```
<a name="iQrwi"></a>
### 重新同步
```bash
cna02:~ # ntpdate ntp1.aliyun.com
 3 Aug 23:22:36 ntpdate[28980]: step time server 120.25.115.20 offset -28911.609110 sec
```
<a name="GZikY"></a>
### 查看时间是否正确
```bash
cna02:~ # date
Wed Aug  3 23:22:41 CST 2022
```
<a name="hMuQn"></a>
## 四、使用 chrony 服务时间同步
<a name="MjDaQ"></a>
### 安装 chrony
```bash
[root@node ~]# yum -y install chrony
Loaded plugins: fastestmirror, langpacks
Loading mirror speeds from cached hostfile
 * base: mirrors.bfsu.edu.cn
 * extras: mirrors.bfsu.edu.cn
 * updates: mirrors.bfsu.edu.cn
Resolving Dependencies
--> Running transaction check
---> Package chrony.x86_64 0:3.2-2.el7 will be updated
---> Package chrony.x86_64 0:3.4-1.el7 will be an update
--> Finished Dependency Resolution

Dependencies Resolved

===============================================================================================================================================================================
 Package                                  Arch                                     Version                                        Repository                              Size
===============================================================================================================================================================================
Updating:
 chrony                                   x86_64                                   3.4-1.el7                                      base                                   251 k

Transaction Summary
===============================================================================================================================================================================
Upgrade  1 Package

Total download size: 251 k
Downloading packages:
No Presto metadata available for base
chrony-3.4-1.el7.x86_64.rpm                                                                                                                             | 251 kB  00:00:00     
Running transaction check
Running transaction test
Transaction test succeeded
Running transaction
  Updating   : chrony-3.4-1.el7.x86_64                                                                                                                                     1/2 
  Cleanup    : chrony-3.2-2.el7.x86_64                                                                                                                                     2/2 
  Verifying  : chrony-3.4-1.el7.x86_64                                                                                                                                     1/2 
  Verifying  : chrony-3.2-2.el7.x86_64                                                                                                                                     2/2 

Updated:
  chrony.x86_64 0:3.4-1.el7                                                                                                                                                    

Complete!
```
<a name="Crlng"></a>
### 启动 chrony 服务
```bash
[root@node ~]# systemctl enable --now chronyd
Created symlink from /etc/systemd/system/multi-user.target.wants/chronyd.service to /usr/lib/systemd/system/chronyd.service.
```
<a name="ZPlqk"></a>
### 查看 chrony 状态
```bash
[root@node ~]# systemctl status chronyd
● chronyd.service - NTP client/server
   Loaded: loaded (/usr/lib/systemd/system/chronyd.service; enabled; vendor preset: enabled)
   Active: active (running) since Thu 2022-08-04 00:12:27 CST; 29s ago
     Docs: man:chronyd(8)
           man:chrony.conf(5)
  Process: 21136 ExecStartPost=/usr/libexec/chrony-helper update-daemon (code=exited, status=0/SUCCESS)
  Process: 21132 ExecStart=/usr/sbin/chronyd $OPTIONS (code=exited, status=0/SUCCESS)
 Main PID: 21134 (chronyd)
    Tasks: 1
   Memory: 364.0K
   CGroup: /system.slice/chronyd.service
           └─21134 /usr/sbin/chronyd

Aug 04 00:12:27 node systemd[1]: Starting NTP client/server...
Aug 04 00:12:27 node chronyd[21134]: chronyd version 3.4 starting (+CMDMON +NTP +REFCLOCK +RTC +PRIVDROP +SCFILTER +SIGND +ASYNCDNS +SECHASH +IPV6 +DEBUG)
Aug 04 00:12:27 node systemd[1]: Started NTP client/server.
Aug 04 00:12:35 node chronyd[21134]: Selected source 193.182.111.141
Aug 04 00:12:35 node chronyd[21134]: Source 78.46.102.180 replaced with 94.237.64.20
```
<a name="rsual"></a>
### 修改配置文件
```bash
[root@node ~]# grep server /etc/chrony.conf 
# Use public servers from the pool.ntp.org project.
#server 0.centos.pool.ntp.org iburst
#server 1.centos.pool.ntp.org iburst
#server 3.centos.pool.ntp.org iburst
server ntp1.aliyun.com iburst
```
<a name="yUPXj"></a>
### 重启服务
```bash
[root@node ~]# systemctl restart chronyd
```
<a name="wEbcu"></a>
### 检查时间同步状态
```bash
[root@node ~]# chronyc sources
210 Number of sources = 1
MS Name/IP address         Stratum Poll Reach LastRx Last sample               
===============================================================================
^* 120.25.115.20                 2   6    71    26   +751us[+1549us] +/-   19ms
```
<a name="xGsoo"></a>
## 五、手动修改时间
<a name="NmrTc"></a>
### 修改时间方法 1
```bash
[root@node ~]# date -s "2012-05-23 01:01:01"
Wed May 23 01:01:01 CST 2012
[root@node ~]# date
Wed May 23 01:01:05 CST 2012
```
<a name="O6WwW"></a>
### 修改时间方法 2
修改时间顺序为月日时分年. 秒
```bash
[root@tianyi ~]# date 090621282021.28
Mon Sep  6 21:28:28 CST 2021
```
<a name="xIvAU"></a>
### 硬件时间向系统时间同步
```bash
[root@node ~]# hwclock -w
```
<a name="GTlxE"></a>
### 按格式输出时间
```bash
[root@node ~]# date "+%Y-%m-%d %H:%M:%S"
2012-05-23 01:04:10
```
<a name="LGBlK"></a>
### 设置时区
```bash
[root@tianyi backups]# timedatectl list-timezones |grep Shanghai
Asia/Shanghai
[root@tianyi backups]# timedatectl set-timezone Asia/Shanghai
```
