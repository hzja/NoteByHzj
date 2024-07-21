Linux
<a name="LMYw7"></a>
## 一、账户安全
```bash
/etc/passwd
/etc/shadow
```
格式：用户名：密码：用户ID：组ID：用户说明：家（home）目录：登陆之后shell<br />注意：无密码只允许本机登陆，远程不允许ssh登陆<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653222246865-8c30bc64-1ccd-4ad7-9455-36db90e2d019.png#clientId=u569a5048-ad40-4&from=paste&id=u4d415c6b&originHeight=502&originWidth=963&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ubac1b6fc-24db-4df9-a590-90ac6932c2f&title=)<br />用户名：加密密码：密码最后一次修改日期：两次密码的修改时间间隔：密码有效期：密码修改到期到的警告天数：密码过期之后的宽限天数：账号失效时间：保留<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653222246934-85406ac2-31ca-4b7a-8d43-fe18b3eeb84a.png#clientId=u569a5048-ad40-4&from=paste&id=uefa48ae5&originHeight=470&originWidth=964&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u29fc0228-a497-422b-bc4f-db5a154410a&title=)
<a name="VxAL2"></a>
### 1.1 查询特权账户 & 组
```bash
awk -F: '$3==0{print $1}' /etc/passwd
awk -F: '$4==0{print $1}' /etc/passwd
```
<a name="vCT6I"></a>
### 1.2 查询可远程登录账户及其它信息
```bash
awk '/\$1|\$6/{print $1}' /etc/shadow   #可以远程登录的账号
cat /etc/passwd | grep /bin/bash #查看哪些用户使用shell
cat /etc/passwd | grep x:0 #查看哪些用户为root权限
cat /etc/passwd | grep -v nologin #查看除了不可登录以外的用户都有哪些，有没有新增的
who #查看当前登录用户（tty 本地登陆  pts 远程登录）
w   #查看系统信息，想知道某一时刻用户的行为
uptime  #查看登陆多久、多少用户，负载
stat /etc/passwd #查看密码文件上一次修改的时间
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653222246877-798c8ba7-ef5b-41ea-910d-87d20bd15063.png#clientId=u569a5048-ad40-4&from=paste&id=uff0c0785&originHeight=384&originWidth=760&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u45115f7f-2c7a-4f5a-b7b7-486cfa73dec&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653222246928-77e14c7b-78ba-48d0-8154-7cb7f6dc277a.png#clientId=u569a5048-ad40-4&from=paste&id=u9faf5618&originHeight=339&originWidth=645&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u10cb432b-e782-4ed5-aa24-3157ffdbf86&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653222246926-188cb24f-a3be-46a7-aa92-b85755db7fa7.png#clientId=u569a5048-ad40-4&from=paste&id=u2213e25d&originHeight=197&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uec2b29b6-74fd-45fb-9a4c-18b36861901&title=)
<a name="C8GrA"></a>
### 1.3 查询 sudo 权限账户
```bash
more /etc/sudoers | grep -v "^#\|^$" | grep "ALL=(ALL)"
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653222247343-bfa59f69-4300-44ac-bf19-aac430243a20.png#clientId=u569a5048-ad40-4&from=paste&id=ue20bbd08&originHeight=229&originWidth=962&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0c243e7f-e515-4859-bc0e-d8919bcadcb&title=)
<a name="GiwuQ"></a>
### 1.4 禁用or删除账户
```bash
usermod -L username
userdel username
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653222247384-307f7c2b-3c43-4d7d-a152-291aa34b6fff.png#clientId=u569a5048-ad40-4&from=paste&id=u32b1dd66&originHeight=928&originWidth=964&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8c41a7a6-fa18-4911-8759-df2ed3130de&title=)
<a name="PH6Mx"></a>
### 1.5 用户历史命令
```bash
cat /home/hack/.bash_history
history #root历史命令

root/.bash_profile
```
<a name="a7YuF"></a>
## 二、异常端口进程服务文件
<a name="s14es"></a>
### 2.1 端口
```bash
netstat -atnlp
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653222247241-445af90c-3a3e-44a0-90a3-755ee4965414.png#clientId=u569a5048-ad40-4&from=paste&id=u9014d309&originHeight=308&originWidth=958&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9f7b2d64-96a3-4dcf-872e-f2dde325009&title=)
```bash
cat /proc/PID/exe | more
ls -l /proc/PID/exe
netstat -antlp | grep 异常IP | awk '{print $7}' | cut -f1 -d"/"
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653222247384-6a23aae1-c29b-4767-a439-d97904556426.png#clientId=u569a5048-ad40-4&from=paste&id=u6458965b&originHeight=448&originWidth=958&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3809e68f-ca27-4ed5-8cf4-6a1925f51b6&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653222247421-d6228ef4-7482-420c-a28a-05fdafbca7bb.png#clientId=u569a5048-ad40-4&from=paste&id=u5ff8e418&originHeight=576&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8df927b2-cff8-4839-a8af-3c391d7e64b&title=)
<a name="HPJO3"></a>
### 2.2 进程
```bash
ps aux | grep PIDnet
file /proc/PID/exe
/proc/7252/exe: symbolic link to '/usr/bin/php'
ps aux --sort=pcpu | head -10   #查看cpu占用率前十的进程
ps -ef --sort -pcpu #按CPU 使用率从高到底排序
ps -ef --sort -pmem  #按内存从高到低
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653222247482-d2f9f544-75e4-4292-afaf-2eca7d42c1b7.png#clientId=u569a5048-ad40-4&from=paste&id=u3dc0229f&originHeight=559&originWidth=1065&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u39eb756e-901e-4139-8b8c-5d957cc55f8&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653222247620-1aca7cec-6423-43ea-ba99-36e9bef2aad8.png#clientId=u569a5048-ad40-4&from=paste&id=u0ec47700&originHeight=442&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u440b7eb8-a0b5-4db1-83d0-43d4ccef2d8&title=)
<a name="LxZWR"></a>
### 2.3 使用 lsof 分析进程
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653222247784-ff5b9092-d3b1-40d2-8ba9-3cfc7a95e37b.png#clientId=u569a5048-ad40-4&from=paste&id=u0d94a862&originHeight=546&originWidth=1003&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u393392be-61b8-4450-ac3e-64b856fc874&title=)<br />使用 ps 查看启动 时间并 杀掉危险进程
```bash
ps -p 7224 -o lstart
kill -9 7224
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653222247990-c48417eb-e49f-4415-adc1-516b7d830488.png#clientId=u569a5048-ad40-4&from=paste&id=ud6d926a2&originHeight=239&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ucebe9a50-ffcb-47b0-8e61-b9a11615833&title=)
<a name="gr34S"></a>
### 2.4 服务
```bash
chkconfig #查看开机启动项目
chkconfig  --list #查看服务自启状态
systemctl list-unit-files |grep enabled #ubuntu，centos及使用 systemctl 控制的系统
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653222247827-a4da5d03-acc1-48b9-a93b-bb70a7904205.png#clientId=u569a5048-ad40-4&from=paste&id=u6fff241b&originHeight=577&originWidth=444&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u93150bfc-5917-4813-b761-ab78a798d99&title=)
<a name="DYtal"></a>
### 2.5 文件
很多时候会遇到无法常看文件权限或是病毒在一直向某个文件写入程序，可尝试如下命令：
```bash
lsattr 文件 查看权限 
chattr -i 文件 接触文件后删除
```
<a name="zQ213"></a>
### 2.6 其它
host 文件 /etc/hosts find 命令 md5sum 命令 grep 命令 diff 命令<br />很多情况下，存在ps、netstat等一些常见命令被替换，可利用stat查看该状态，查看其修改时间
```bash
stat /bin/netstat
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653222248061-ef0ed9ec-efdb-4770-b0a9-dc6f5596033a.png#clientId=u569a5048-ad40-4&from=paste&id=u99bac82b&originHeight=214&originWidth=669&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u312fd2ac-a1e9-487d-be68-828977216ff&title=)
<a name="cmBKS"></a>
## 三、启动项与定时任务
系统运行级别：

1. 0 所有进程将被终止，机器将有序的停止，关机时系统处于这个运行级别
2. 1 单用户模式。用于系统维护，只有少数进程运行，同时所有服务也不启动
3. 2多用户模式。和运行级别3一样，只是网络文件系统(NFS)服务没被启动
4. 3多用户模式。允许多用户登录系统，是系统默认的启动级别
5. 4留给用户自定义的运行级别
6. 5多用户模式，并且在系统启动后运行X-Window，给出一个图形化的登录窗口
7. 6所有进程被终止，系统重新启动
> 默认 级别 /etc/inittab

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653222248144-77105a3b-f335-45f6-83d6-cafdda0220d9.png#clientId=u569a5048-ad40-4&from=paste&id=u94294155&originHeight=510&originWidth=966&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6fd47dfd-15d2-4c66-bfce-d2b636ae311&title=)
<a name="tdjlI"></a>
### 3.1 开机启动文件
```bash
/etc/rc.local
/etc/rc.d/rc[0~6].d
/etc/rc.d/rc3.d/
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653222248154-c0f0f316-63ed-430a-9daa-1101bae85faa.png#clientId=u569a5048-ad40-4&from=paste&id=ue0ada5f7&originHeight=502&originWidth=954&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u08425c83-2a3b-4db1-80e3-78e39a0918b&title=)<br />排查环境变量<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653222248191-5c67bc58-3d85-45fa-a27f-ec3aba8fa151.png#clientId=u569a5048-ad40-4&from=paste&id=u3d68c455&originHeight=455&originWidth=652&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8eebb7ea-ff55-447f-bce7-fb3fc01716f&title=)
<a name="IeycY"></a>
### 3.2 定时任务
计划任务文件
> /var/spool/cron/* #centos<br />/var/spool/cron/crontabs/* #ubuntu的<br />/etc/crontab<br />/etc/cron.d/*<br />/etc/cron.daily/* <br />/etc/cron.hourly/* <br />/etc/cron.monthly/*<br />/etc/cron.weekly/<br />/etc/anacrontab #异步定时<br />/var/spool/anacron/*
> /etc/cron.daily/*  查看目录下所有文件

```bash
crontab  -l #查看定时任务
crontab –e #编辑定时任务
crontab -u root –l  #查看root用户任务计划
ls /var/spool/cron/  #查看每个用户自己的执行计划

sed '/gcc.sh/d' /etc/crontab && chmod 0000 /etc/crontab && chattr +i /etc/crontab   #删除计划任务且控制计划任务不能写东西，慎用。
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653222248451-c8209d66-8581-4637-8bc0-a944d00c9a87.png#clientId=u569a5048-ad40-4&from=paste&id=ua382a703&originHeight=375&originWidth=956&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud14bcadc-e7c8-4550-b5de-968ea1fc5e4&title=)
<a name="aMiUI"></a>
### 3.3 日志
系统常见的日志目录

| 文件名 | 说明 |
| --- | --- |
| cron | 记录系统定时任务相关 |
| dmesg | 记录了系统在开机时内核自检的信息 |
| btmp | 记录二进制错误登录日志，使用lastb查看 |
| lastlog | 记录二进制系统中所有用户最后一次登录时间的日志 |
| secure | 记录验证和授权方面的信息，只要涉及账号和密码的程序都会记录，比如SSH登录，su切换用户，sudo授权，telnet，ftp等服务登录信息 |
| message | 记录系统启动后的信息和错误日志 |
| wtmp | 登录进入，退出，数据交换、关机和重启纪录 |

默认日志位置：var/log<br />/var/log/auth.log 包含系统授权信息，包括用户登录和使用的权限机制等信息 /var/log/lastlog 记录登录的用户，可以使用命令lastlog查看 /var/log/secure 记录大多数应用输入的账号与密码，登录成功与否 日志一般信息量巨大，根据自己的环境索引相关的有效的 信息 /var/log/cron 记录crontab命令是否被正确的执行
```bash
grep "Failed password for root" /var/log/secure | awk '{print $11}' | sort | uniq -c | sort -nr | more
```
<a name="ksk67"></a>
#### 定位有哪些IP在爆破：
```bash
grep "Failed password" /var/log/secure|grep -E -o "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)"|uniq -c 
```
<a name="pTjFp"></a>
#### 爆破用户名字典是什么？
```bash
grep "Failed password" /var/log/secure|perl -e 'while($_=<>){ /for(.*?) from/; print "$1\n";}'|uniq -c|sort -nr 
```
<a name="pjvrV"></a>
#### 登录成功的IP有哪些：
```bash
grep "Accepted " /var/log/secure | awk '{print $11}' | sort | uniq -c | sort -nr | more
```
<a name="FVUbk"></a>
#### 登录成功的日期、用户名、IP：
```bash
grep "Accepted " /var/log/secure | awk '{print $1,$2,$3,$9,$11}'
```
<a name="Mvxtv"></a>
#### 日志中的敏感特征
<a name="j3COz"></a>
##### 文件内容中的恶意函数
PHP：eval(、system(、assert(<br />JSP：getRunTime(、 FileOutputStream(<br />ASP：eval(、execute(、 ExecuteGlobal（
<a name="tWHCl"></a>
##### webshell特征
Darkblade：goaction=login<br />JspSpy：o=login<br />PhpSpy：action=phpinfo<br />Regeorg：cmd=connect<br />Other：cmd=
<a name="OHPMC"></a>
##### Linux日志分析工具
[https://github.com/wpsec/GScan](https://github.com/wpsec/GScan)
<a name="DjL9H"></a>
### 3.4 自动化辅助工具
[https://github.com/grayddq/GScan](https://github.com/grayddq/GScan)<br />[https://github.com/ppabc/security_check](https://github.com/ppabc/security_check)<br />[https://github.com/T0xst/linux](https://github.com/T0xst/linux)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653222248497-e87fe5e1-4d7a-4ce3-b148-c975fcbf12ad.png#clientId=u569a5048-ad40-4&from=paste&id=u9c664bcc&originHeight=661&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7752b254-c5b1-4865-9608-75da0153dc3&title=)
<a name="TxcrG"></a>
### 3.5 勒索病毒引擎
深信服 [https://edr.sangfor.com.cn/#/information/ransom_search](https://edr.sangfor.com.cn/#/information/ransom_search)<br />360 [https://lesuobingdu.360.cn/](https://lesuobingdu.360.cn/)<br />奇安信 [https://lesuobingdu.qianxin.com/](https://lesuobingdu.qianxin.com/)<br />腾讯 [https://guanjia.qq.com/pr/ls/](https://guanjia.qq.com/pr/ls/)<br />VenusEye [https://lesuo.venuseye.com.cn/](https://lesuo.venuseye.com.cn/)
<a name="XkVgq"></a>
### 3.6 反勒索病毒工具
腾讯 [https://habo.qq.com/tool/index](https://habo.qq.com/tool/index)<br />金山毒霸 [https://www.duba.net/dbt/wannacry.html](https://www.duba.net/dbt/wannacry.html)<br />瑞星 [http://it.rising.com.cn/fanglesuo/index.html](http://it.rising.com.cn/fanglesuo/index.html)<br />Avast [https://www.avast.com/zh-cn/ransomware-decryption-tools](https://www.avast.com/zh-cn/ransomware-decryption-tools)<br />Github tools [https://github.com/jiansiting/Decryption-Tools](https://github.com/jiansiting/Decryption-Tools)
