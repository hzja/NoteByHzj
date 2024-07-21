网络安全<br />拿到一台 Linux 主机普通权限之后，如何获取更高的 root 权限？这是红队成员之必备技能，首先为什么要提权？因为提权之后能看到主机上的所有信息，包括主机管理的账号密码哈希，可以离线破解管理员密码，也许存在通用密码的习惯，来测试梳理下 Linux 提权的基本信息收集命令及作用。当然，也可以直接拿最新或者历史的提权 exp 批量测试，也可满足需求，本文旨在学习一些 Linux 的基础操作和关键信息收集。
<a name="UMmRM"></a>
### 0x01 查看操作系统信息，内核版本等
查看操作系统类型：
```bash
cat /etc/issue 
cat /etc/*-release 
cat /etc/lsb-release
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652336424971-89146189-d161-43e0-98eb-fe0cf96a224a.png#clientId=u17bb71e5-de26-4&from=paste&id=ue5d984d0&originHeight=610&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc14785c8-0cbc-478e-9a21-03e85a685f5&title=)<br />可以看到系统是 debain，那么系统版本是什么？64 位还是 32 位？
```bash
cat /proc/version 
uname -a 
uname -mrs 
rpm -q kernel 
dmesg | grep Linux 
ls /boot | grep vmlinuz-
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652336424936-711077fd-5df2-45b1-a4bd-b7b710d9894c.png#clientId=u17bb71e5-de26-4&from=paste&id=u4a94353a&originHeight=426&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u62e1ae8f-29bc-4ea9-93a2-b0832baadca&title=)<br />可以看到当前系统是 64 位。
<a name="dJEYo"></a>
### 0x02 查看系统环境配置
```bash
cat /etc/profile 
cat /etc/bashrc 
cat ~/.bash_profile 
cat ~/.bashrc 
cat ~/.bash_logout 
env 
set
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652336424885-4a73ba0b-fd04-48eb-a007-175387b138da.png#clientId=u17bb71e5-de26-4&from=paste&id=u79f3398e&originHeight=595&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua28416ef-9009-4756-b914-ac75f964f98&title=)<br />内容比较多，图截不全，还是可以看到一些信息的，比如当前用户的情况：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652336424857-acb8e59e-39de-49a8-a3ce-e268f655c298.png#clientId=u17bb71e5-de26-4&from=paste&id=u670fbacc&originHeight=586&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u048be631-d3f4-4dec-843e-189b6e65beb&title=)
<a name="NmYVm"></a>
### 0x03 查看应用和服务
当前运行了哪些服务？这些服务的用户权限如何？
```bash
ps aux 
ps -ef 
top 
cat /etc/services
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652336424876-320fe764-31a9-408c-bfa3-0bd193c98fc0.png#clientId=u17bb71e5-de26-4&from=paste&id=u5c7c2b2c&originHeight=458&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0703fa72-ca47-4cd0-b602-94d0105cadb&title=)<br />如果只需查看运行用户是 root 的服务，可以使用 grep 提取：
```bash
ps aux | grep root 
ps -ef | grep root
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652336425370-94f76896-db60-4947-816f-36c61d8e388a.png#clientId=u17bb71e5-de26-4&from=paste&id=u8799b32d&originHeight=661&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue120dbe1-c3d6-4b9a-980b-d6b7cbbb846&title=)
<a name="YNgzF"></a>
### 0x04 安装了哪些应用
系统安装了哪些应用，什么版本？
```bash
ls -alh /usr/bin/ 
ls -alh /sbin/ 
dpkg -l 
rpm -qa 
ls -alh /var/cache/apt/archivesO 
ls -alh /var/cache/yum/
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652336425394-0d56583c-d755-4003-a88b-e1b43e9872c9.png#clientId=u17bb71e5-de26-4&from=paste&id=u9a03d783&originHeight=528&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7215eb5d-9e5e-4ca5-959c-b6016d4fe55&title=)<br />对于找应用漏洞有一定的帮助。
<a name="Ktaf2"></a>
### 0x05 常见服务的配置文件
```bash
cat /etc/syslog.conf 
cat /etc/chttp.conf 
cat /etc/lighttpd.conf 
cat /etc/cups/cupsd.conf 
cat /etc/inetd.conf 
cat /etc/apache2/apache2.conf 
cat /etc/my.conf 
cat /etc/httpd/conf/httpd.conf
cat /opt/lampp/etc/httpd.conf
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652336425471-09c5cc3f-5347-4789-9c1d-e62884e55c73.png#clientId=u17bb71e5-de26-4&from=paste&id=u5612372c&originHeight=689&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2789370a-9a49-4048-9ef3-15993049b3f&title=)<br />查看 /etc/ 目录下所有 root 用户的文件：<br />ls -aRl /etc/ | awk '$1 ~ /^._r._/'<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652336425469-574aed66-17dc-4357-b76c-1da3c04b8dec.png#clientId=u17bb71e5-de26-4&from=paste&id=ua2f143f6&originHeight=573&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u07562576-5b5c-4884-b5c7-87f244abbcc&title=)
<a name="RKmqh"></a>
### 0x06 查看系统计划任务
```bash
crontab -l 
ls -alh /var/spool/cron 
ls -al /etc/ | grep cron 
ls -al /etc/cron* 
cat /etc/cron* 
cat /etc/at.allow 
cat /etc/at.deny 
cat /etc/cron.allow 
cat /etc/cron.deny 
cat /etc/crontab
cat /var/spool/cron/crontabs/root
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652336425608-6b9921a0-fea8-4901-9e08-6f26ab97d7c7.png#clientId=u17bb71e5-de26-4&from=paste&id=uc942cf76&originHeight=597&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua10ab685-dcfd-415f-9ef8-7f4edbb23c7&title=)
<a name="q4pIV"></a>
### 0x07 寻找文件中包含关键词 user、pass 等内容
```bash
grep -i user [filename] 
grep -i pass [filename] 
grep -C 5 "password" [filename] 
find . -name "*.php" -print0 | xargs -0 grep -i -n "var $password" # Joomla
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652336425926-ebc36f19-4f5e-4f2a-a132-15df2c4e4f51.png#clientId=u17bb71e5-de26-4&from=paste&id=u2e6835e0&originHeight=169&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua511e8b4-58e0-478a-bf5c-61804194e8a&title=)
<a name="qHBsf"></a>
### 0x08 系统网卡信息，连接的网络信息
```bash
/sbin/ifconfig -a 
cat /etc/network/interfaces 
cat /etc/sysconfig/network
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652336425909-c82d2dfa-fc7e-4589-afcd-dcaefe42ecbc.png#clientId=u17bb71e5-de26-4&from=paste&id=ud97f0ba4&originHeight=531&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud1235376-8abc-4bf0-8349-a52872a67e0&title=)
<a name="BcjUG"></a>
### 0x09 关于网络的配置信息，dns服务器、dhcp 服务器，防火墙配置等
```bash
cat /etc/resolv.conf 
cat /etc/sysconfig/network 
cat /etc/networks 
iptables -L 
hostname 
dnsdomainname
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652336425978-c48ade20-4e6a-4566-aff4-3da9e50bb967.png#clientId=u17bb71e5-de26-4&from=paste&id=u47ddc9c6&originHeight=627&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u39f16528-1cd0-43f2-be49-41d14180d76&title=)
<a name="SDMal"></a>
### 0x0A 与哪些主机有网络连接？当前在线的用户有哪些？
```bash
lsof -i 
lsof -i :80 
grep 80 /etc/services 
netstat -antup 
netstat -antpx 
netstat -tulpn 
chkconfig --list 
chkconfig --list | grep 3:on 
last 
w
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652336426050-2409ed3b-e1a2-41a5-a64c-7c779e52b807.png#clientId=u17bb71e5-de26-4&from=paste&id=u3ce8cf1c&originHeight=627&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u64d0ad02-56f7-467f-978d-24650b70513&title=)
<a name="nt9qB"></a>
### 0x0B 系统网络缓存
```bash
arp -e 
route 
/sbin/route -nee
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652336426071-ad272307-fd8d-4dc0-937a-946cc9882fba.png#clientId=u17bb71e5-de26-4&from=paste&id=uea29ffce&originHeight=392&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue782807c-e19d-443d-aad4-df78cab0ac5&title=)<br />dump 系统流量，分析数据：
```bash
tcpdump tcp dst 192.168.1.7 80 and tcp dst 10.5.5.252 21
# 命令说明，：
tcpdump tcp dst [ip] [port] and tcp dst [ip] [port]
```
<a name="JZs5e"></a>
### 0x0C 用户相关信息
```bash
id 
who 
w 
last 
cat /etc/passwd | cut -d: -f1 
grep -v -E "^#" /etc/passwd | awk -F: '$3 == 0 { print $1}' 
cat /etc/sudoers 
sudo -l
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652336426269-6ab4df35-00fa-4e38-8d15-41dcbdfaa4ca.png#clientId=u17bb71e5-de26-4&from=paste&id=ue1f611ac&originHeight=516&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue60ff524-8a50-4fbd-b267-a3b43526bde&title=)<br />用户相关的文件路径：
```bash
cat /etc/passwd 
cat /etc/group 
cat /etc/shadow 
ls -alh /var/mail/
```
用户目录：
```bash
root => /root/
other => /home/other
```
<a name="JyrtS"></a>
### 0x0D 常见服务默认配置文件
可能包含账号密码信息：
```bash
cat /var/apache2/config.inc 
cat /var/lib/mysql/mysql/user.MYD 
cat /root/anaconda-ks.cfg
```
<a name="V8SFy"></a>
### 0x0E 用户操作历史记录
可能存在明文账号密码之类的信息
```bash
cat ~/.bash_history 
cat ~/.nano_history 
cat ~/.atftp_history 
cat ~/.mysql_history 
cat ~/.php_history
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652336426316-fcb83e58-1048-49b6-986e-bc511dc6440b.png#clientId=u17bb71e5-de26-4&from=paste&id=ub7100ec4&originHeight=783&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u575bdc68-4c13-4dcf-86b7-3157689cfd2&title=)
<a name="yEF1u"></a>
### 0x10 总结
这里整理的也许不全，不过该收集的信息也基本差不多了，在此信息基础上，获取更多其他系统权限或者系统本身的权限，都有一定的知道意义，可以针对性的去寻找可以利用的 EXP 直接提升权限，或者利用系统的配置问题，间接提升权限都是可以的。
