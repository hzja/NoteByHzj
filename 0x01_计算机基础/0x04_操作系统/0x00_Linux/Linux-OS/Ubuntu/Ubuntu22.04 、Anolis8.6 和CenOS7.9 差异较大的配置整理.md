<a name="yYs0q"></a>
# 需求
随着CentOS停服，各个行业的运维都在寻找各自的替代方案，考虑的出发点有：

- 新操作系统是否兼容CentOS，避免太大的操作差异；
- 新操作系统是否为信创，其具体收费情况如何；
- 新操作系统是否支持基础工具、中间件、数据库等整套开源方案；

除了以上几点，现在的CentOS7.9 由于不再维护，很多组件新特性已经不再支持，如cgroupsv2、rootless container，因此要提前去适应新操作系统，为接下来的操作系统替换做好储备。<br />在此，特将Ubuntu22.04 、Anolis8.6 和CenOS7.9 差异较大的配置习惯进行整理总结。
<a name="BBTkP"></a>
# Ubuntu22.04
<a name="ZdVB3"></a>
## DNS设置
[https://learnubuntu.com/change-dns-server/](https://learnubuntu.com/change-dns-server/)
<a name="PUajJ"></a>
### 1、查看当前DNS配置
DNS 全局配合和网卡局部配置 systemd 239中systemd-resolve已被重命名为resolvectl
```bash
$ resolvectl status
Global（全局配置）
       Protocols: -LLMNR -mDNS -DNSOverTLS DNSSEC=no/unsupported
resolv.conf mode: stub

Link 2 (enp1s0)(某网卡局部配置)
    Current Scopes: DNS
         Protocols: +DefaultRoute +LLMNR -mDNS -DNSOverTLS DNSSEC=no/unsupported
Current DNS Server: 8.8.8.8
       DNS Servers: 8.8.8.8 8.4.4.8
```
<a name="bvwDs"></a>
### 2、临时设置DNS
```bash
# vim /etc/resolv.conf
nameserver 1.1.1.2
nameserver 1.0.0.2
```
<a name="rLfrr"></a>
### 3、永久设置DNS
<a name="RD2mB"></a>
#### 1.最简单的方法
使用resolvconf命令设置，通过此命令永久设置dns
```bash
# 1.安装resolvconf
# apt install resolvconf

# 2. 修改head文件（此文件由resolvconf生成）
# vim /etc/resolvconf/resolv.conf.d/head
nameserver 1.1.1.2
nameserver 1.0.0.2

# 4.配置shengxiao 
# resolvconf -u

# 3.开机启动服务
systemctl enable --now resolvconf.service
```
注意：

- 修改完成后，必须通过`resolvconf -u`生效，此时会在/etc/resolv.conf中添加dns配置并生效。
- 生效后，通过`resolvectl status`查询不到此配置生效情况，但是通过`netplan apply`生效可在全局配置中找到。

最终以/etc/resolv.conf中的dns配置生效为准。
<a name="Cd7Ua"></a>
#### 2.非简单方法
修改/etc/netplan/目录下的yml文件
```bash
# 1.修改相关配置文件
# vim /etc/netplan/xxx.yml
network:
  ethernets:
    enp1s0:
      dhcp4: true
      nameservers:
        addresses: [8.8.8.8, 8.4.4.8]
  version: 2

# 2.配置生效
# netplay apply
```
注意：由于此配置是针对enp1s0网卡，因此dns只针对网卡局部生效。另外通过`netplay apply`生效后，dns配置不会加载到/etc/resolv.conf中。
<a name="w8V4p"></a>
#### 3.配置生效总结
`netplay apply`应用后，resolvconf和netplan的配置通过`resolvectl status`查到，但是只有resolvconf的配置会写入/etc/resolv.conf。
```bash
$ resolvectl status
Global（全局配置）
       Protocols: -LLMNR -mDNS -DNSOverTLS DNSSEC=no/unsupported
resolv.conf mode: foreign
Current DNS Server: 1.1.1.2
        DNS Server: 1.0.0.2 

Link 2 (enp1s0)(某网卡局部配置)
    Current Scopes: DNS
         Protocols: +DefaultRoute +LLMNR -mDNS -DNSOverTLS DNSSEC=no/unsupported
Current DNS Server: 8.8.8.8
       DNS Servers: 8.8.8.8 8.4.4.8

$ cat /etc/resolv.conf
nameserver 1.1.1.2
nameserver 1.0.0.2
```
<a name="ftES9"></a>
## 时间同步
Ubuntu22.04 :

- timedatectl 替代了老旧的 ntpdate。默认情况下，timedatectl 在系统启动的时候会立刻同步时间，并在稍后网络连接激活后通过 socket 再次检查一次。
- timesyncd 替代了 ntpd 的客户端的部分。默认情况下 timesyncd 会定期检测并同步时间。它还会在本地存储更新的时间，以便在系统重启时做时间单步调整。

timesyncd  和 ntpd 区别：

- ntpd是步进式平滑的逐渐调整时间
- timesyncd是断点式更新时间，也就是时间不同会立即更新

注意：断点式更新会对某些服务产生影响，所以在生产环境要慎用！
```bash
# 1. 配置时间同步
# vi /etc/systemd/timesyncd.conf
[Time]
# 一个空格分隔的NTP服务器列表
#NTP=     
# 一个空格分隔的NTP服务器列表，用作备用NTP服务器
#FallbackNTP=ntp.ubuntu.com
# 最大可接受的"root distance"秒数(最大误差)，默认值为 5 秒
#RootDistanceMaxSec=5
# NTP消息的 最小/最大轮询间隔秒数
#PollIntervalMinSec=32
#PollIntervalMaxSec=2048

# 2.查看时间同步
# timedatectl
```
<a name="zbXTn"></a>
## 安全基线
<a name="GcasG"></a>
### 1、设置过期时间
```bash
# vim /etc/login.defs
PASS_MAX_DAYS 90
PASS_MIN_DAYS 0
PASS_WARN_AGE 10
```
密码长度已不在此处，而是在相关pam中进一步设置。
<a name="IvvBG"></a>
### 2、设置密码复杂度、长度、过期时间、密码尝试次数
通过pwquality控制，Ubuntu22.04默认没有此模块，需要提前安装。
```bash
# 安装libpam-pwquality
apt install libpam-pwquality

# 设置密码复杂度
vim /etc/security/pwquality.conf
minlen = 8
dcredit = -1
lcredit = -1
ocredit = -1
ucredit = -1

# 设置密码尝试3次
vim /etc/pam.d/common-password
password requisite pam_pwquality.so try_first_pass retry=3

# 禁止重复使用旧密码
vim /etc/pam.d/common-password
password  [success=1 default=ignore]  pam_unix.so obsecure use_authtok try_first_passs yescrypt  remember=5
```
<a name="HMzYY"></a>
### 3、防暴力破解
Ubuntu22.04中，pam_tally2已被移除，可以使用pam_faillock代替。
```bash
# faillock 包含在libpam-modules中，无需安装

# faillock参数配置
# grep -v '#' /etc/security/faillock.conf
dir = /var/run/faillock
audit
silent
deny = 3
fail_interval = 900
unlock_time = 120
```
[https://askubuntu.com/questions/1403438/how-do-i-set-up-pam-faillock](https://askubuntu.com/questions/1403438/how-do-i-set-up-pam-faillock)<br />关于faillock的配置，涉及以下两个pam文件

- /etc/pam.d/common-auth
- /etc/pam.d/common-account
```bash
# 用户账户锁定时连续身份验证失败必须达到的间隔长度默认为 900 秒
# 用户账户锁定时间默认为 600 秒， root用户不受管控

# vim /etc/pam.d/common-auth
auth    required pam_faillock.so preauth audit silent deny=5 unlock_time=900
auth    [success=1 default=ignore]      pam_unix.so nullok
auth    [default=die] pam_faillock.so authfail audit deny=5 unlock_time=900
auth    sufficient pam_faillock.so authsucc audit deny=5 unlock_time=900
auth    requisite                       pam_deny.so
auth    required                        pam_permit.so
auth    optional                        pam_cap.so


# vim /etc/pam.d/common-account
account [success=1 new_authtok_reqd=done default=ignore]        pam_unix.so
account requisite                       pam_deny.so
account required                        pam_permit.so
account    required pam_faillock.so
```
<a name="TLRTD"></a>
## Selinux
[https://linuxconfig.org/how-to-disable-enable-selinux-on-ubuntu-22-04-jammy-jellyfish-linux](https://linuxconfig.org/how-to-disable-enable-selinux-on-ubuntu-22-04-jammy-jellyfish-linux)<br />Ubuntu22.04默认没有安装Selinux，进一步操作：
```bash
# 1.安装selinux
apt update
apt install policycoreutils selinux-utils selinux-basics

# 2.启动selinux
selinux-activate

# 3.enforce selinux，需重启生效
selinux-config-enforcing

# 4.查看selinux 状态
setstatus
SELinux status:  disabled

# 5.关闭selinux，需重启生效
vim /etc/selinux/config
SELINUX=enforcing
或
SELINUX=disabled

# 6.临时设置selinux
setenforce 0
或
setenforce 1
```
<a name="NdMFX"></a>
## 防火墙
Ubuntu22.04 默认使用 ufw管理防火墙，默认处于禁用状态。
```bash
# 1.安装ufw
apt install ufw

# 2.防火墙状态
ufw status verbose
```
<a name="yfGk8"></a>
## 内核
在做内核参数优化时，提示报错sysctl: cannot stat /proc/sys/net/ipv4/tcp_tw_recycle: No such file or directory。Ubuntu22.04 使用5.15.0-60-generic，经过进一步了解到从4.10内核开始，官方修改了时间戳的生成机制，在此参考[小米技术](https://mp.weixin.qq.com/s?__biz=MzUxMDQxMDMyNg==&mid=2247484841&idx=1&sn=7e0923ea9204e126003e263dc8414261&scene=21#wechat_redirect)的总结的建议：

- tcp_tw_recycle 选项在4.10内核之前还只是不适用于NAT/LB的情况(其他情况下，也非常不推荐开启该选项)，但4.10内核后彻底没有了用武之地，并且在4.12内核中被移除。
- tcp_tw_reuse 选项仍然可用。在服务器上面，启用该选项对于连入的TCP连接来说不起作用，但是对于客户端(比如服务器上面某个服务以客户端形式运行，比如nginx反向代理)等是一个可以考虑的方案。
- 修改TCP_TIMEWAIT_LEN是非常不建议的行为。
<a name="xWi7v"></a>
# Anolis8.6
由于Anolis8.6 虽兼容Centos7.9，但在时间同步方面有点差异。
<a name="cJSOY"></a>
## 时间同步
Anolis8.6默认不再支持ntp软件包，时间同步将由chrony来实现。
```bash
# 通过配置文件实现时间同步
vim /etc/chrony.conf
server 192.168.20.17 iburst
# 重启服务
systemctl restart chronyd.service

# 检查chrony是否同步，其中"Update interval"参数, 说明最后两次更新的时间间隔
chronyc tracking

# 显示所有 NTP 源服务器的信息
chronyc sources -v

# 查看 NTP 服务器的在线和离线状态
chronyc activity

# 手动添加一台新的 NTP 服务器
chronyc add server XXXX

# 强制同步系统时间
chronyc -a makestep
```
<a name="D7K0b"></a>
# 总结
Ubuntu22.04 和 Anolis8.6 只是众多替代方案中的两种，大家有兴趣的话还可以考虑其他方案：

- Oracle  Linux
- OpenEuler
- 统信UOS
- 中标麒麟
- 银河麒麟
- Rocky Linux
