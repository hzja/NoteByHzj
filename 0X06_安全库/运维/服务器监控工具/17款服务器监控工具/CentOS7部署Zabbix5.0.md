CentOS7 Zabbix
<a name="FyfC4"></a>
### 系统简介
zabbix是一个企业级解决方案，支持实时监控数千台服务器，虚拟机和网络设备采集百万级监控指标。<br />Zabbix的主要特点有：

1. 指标收集：从任何设备、系统、应用程序上进行指标采集；
2. 问题监测：定位智能阀值；
3. 可视化：单一界面管理平台；
4. 告警和修复：确保及时，有效的告警
5. 安全和认证：保护您所有层级的数据
6. 轻松搭建部署：大批模板、开箱即用、节省您宝贵的时间
7. 自动发现：自动监控大型动态环境
8. 分布式监控：无限制扩展
<a name="H0psU"></a>
### 名词术语
Zabbix 系统有一些自己定义的专业术语，为更好的熟悉系统名词，下面主要介绍本文以及zabbix常用的术语。
<a name="aAUiS"></a>
#### 主机（host）
一台想监控的服务器、工作站、交换机等网络设备，用IP或者域名表示。
<a name="rW2gK"></a>
#### 主机组（host group）
多台具有某种相同角色、属性的集合。例如，所有Windows服务器放在一个叫“Windows server”的主机组中。
<a name="UFqqH"></a>
#### 监控项（item）
想要监控、获取主机或者主机组的哪些数据。例如：想要监控所有机器的CPU使用情况，则需要建一个监控项，用于获取所有服务器的CPU使用率。
<a name="k4hwR"></a>
#### 触发器（trigger）
由逻辑表达式组成的按照预先设置好的阀值来评估由监控项采集到的数据。触发器有两种状态，分别为“问题”和“已解决”。例如：在上述通过监控获取了CPU的使用率，例如超过CPU使用超过80%的就预警，则可以创建一个触发器，当监控项获取的值超过80%时就按照预设的情况报警，状态为“问题”，低于80%时认为报警解除，状态恢复为“已解决”。
<a name="NQftE"></a>
#### 事件（event）
单次发生的需要注意的事情，例如上述触发器状态由问题变成了正常或者由正常变成了问题，均可以称为一个事件。事件包括触发器事件、自动发生事件、自动注册事件和内部事件4个部分。
<a name="pC3GO"></a>
#### 动作（action）
一个对事件做出反应的预定义的操作：例如CPU使用超过80%时，触发器状态变成了问题，即产生了一个事件，可以针对此事件预设一个动作（比如执行命令reboot），则系统全自动针对此事件的预设作执行命令reboot。
<a name="aEdrt"></a>
#### Zabbix server
Zabbix 系统实现监控的核心程序，主要功能是与被监控主机、代理机等进行交互、触发器计算、发送告警通知、收集数据并存储等。
<a name="ZBolh"></a>
#### Zabbix agent
一个部署在监控对象上的，能够主动监控本地资源和应用的程序；一般来讲，需要在所有被监控服务器上安装此程序。
<a name="O5jc0"></a>
### 安装步骤
<a name="DuJYj"></a>
#### 1、关闭防火墙和selinux
运行以下命令关闭selinux
```bash
sed -i 's/SELINUX=enforcing/SELINUX=disabled/' /etc/selinux/config
```
运行以下命令关闭防火墙
```bash
systemctl disable --now firewalld
```
重启生效
```bash
reboot
```
<a name="lWltG"></a>
#### 2、安装zabbix rpm源
运行以下命令安装zabbix rpm源
```bash
rpm -Uvh https://mirrors.aliyun.com/zabbix/zabbix/5.5/rhel/7/x86_64/zabbix-release-5.5-1.el7.noarch.rpm
sed -i 's#http://repo.zabbix.com#https://mirrors.aliyun.com/zabbix#' /etc/yum.repos.d/zabbix.repo
yum clean all
```
<a name="jXQlb"></a>
#### 3、安装zabbix server和agent
运行以下命令安装zabbix server和agent
```bash
yum install zabbix-server-mysql zabbix-agent -y
```
<a name="Sj3Q9"></a>
#### 4、安装zabbix前端
运行以下命令 安装Software Collections
```bash
yum install centos-release-scl -y
```
运行以下命令， 将[zabbix-frontend]下的 enabled 改为 1
```bash
vim /etc/yum.repos.d/zabbix.repo

:wq 保存
```
运行以下命令安装zabbix 前端和相关环境
```bash
yum install zabbix-web-mysql-scl zabbix-apache-conf-scl -y
```
<a name="f6Fxy"></a>
#### 5、安装mariadb数据库
运行以下命令安装数据库
```bash
yum install mariadb-server -y
```
运行以下命令启动数据库并配置开机启动
```bash
systemctl enable --now mariadb
```
运行以下命令初始化数据库
```bash
mysql_secure_installation
```
<a name="bK6eR"></a>
#### 6、zabbix数据库配置
运行以下命令创建zabbix数据库及数据库用户
```bash
mysql -u root -p （输入数据库root密码进入数据库）
```
运行以下命令创建zabbix数据库
```bash
create database zabbix default character set utf8 COLLATE utf8_ bin ;
```
运行以下命令创建zabbix数据库用户
```bash
grant all privileges on zabbix.* to zabbix@localhost identified by "zabbix_pwd" ;
```
`quit` 退出<br />运行以下命令导入zabbix数据库
```bash
zcat /usr/share/doc/zabbix-server-mysql*/create.sql.gz | mysql -uzabbix -p zabbix
```
<a name="kYnAO"></a>
#### 7、zabbix配置及前端页面安装
运行以下命令修改zabbix server配置文件里的数据库信息
```bash
vim /etc/zabbix/zabbix_server.conf
```
找到 `DBPassword=password` ，将数据库密码改为zabbix数据库的密码<br />`:wq` 保存<br />运行以下命令修改zabbix php配置文件里的时区
```bash
vim /etc/opt/rh/rh-php72/php-fpm.d/zabbix.conf
```
找到`php_value[date.timezone]`，去掉注释及前面的标点符号，改成
```bash
php_value[date.timezone] = Asia/Shanghai

:wq 保存
```
运行以下命令启动相关服务并配置开机自动启动
```bash
systemctl restart zabbix-server zabbix-agent httpd rh-php72-php-fpm

systemctl enable zabbix-server zabbix-agent httpd rh-php72-php-fpm
```
使用浏览器访问zabbix web页面继续安装<br />http://IP/zabbix<br />![2021-08-26-17-14-50-502573.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629969537102-4ca44de3-daf4-4d1d-9afd-e79705bd6f7c.png#clientId=ud9aeb59c-d6fa-4&from=ui&id=u378d8e68&originHeight=341&originWidth=692&originalType=binary&ratio=1&rotation=0&showTitle=false&size=20773&status=done&style=shadow&taskId=u7ef8661b-7f5f-4346-90c3-01df4658636&title=)<br />点击Next step开始安装<br />![2021-08-26-17-14-50-831569.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629969537108-a6ff14b4-3922-4a82-b776-59a391f01725.png#clientId=ud9aeb59c-d6fa-4&from=ui&id=Bv5pD&originHeight=415&originWidth=692&originalType=binary&ratio=1&rotation=0&showTitle=false&size=31253&status=done&style=shadow&taskId=u5a10d7cb-5166-47ee-b587-c3f7a308e48&title=)<br />自动检查环境，所有选项都为OK则点击Next step进行下一步<br />![2021-08-26-17-14-51-224569.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629969537121-605b2274-10a6-4852-acc1-36eef5dacf1d.png#clientId=ud9aeb59c-d6fa-4&from=ui&id=GhVcg&originHeight=411&originWidth=692&originalType=binary&ratio=1&rotation=0&showTitle=false&size=59064&status=done&style=none&taskId=uec80dc7f-db2d-4bcb-a649-edf82bf8b61&title=)<br />配置数据库连接，port填写0为默认3306端口，配置正确的host、name、password后点击Next step进行下一步<br />![2021-08-26-17-14-51-459595.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629969571839-3535d7f4-f70e-4719-9ef8-9a984ba9e596.png#clientId=ud9aeb59c-d6fa-4&from=ui&id=u592bca5c&originHeight=409&originWidth=692&originalType=binary&ratio=1&rotation=0&showTitle=false&size=50582&status=done&style=none&taskId=uf508d147-1686-4ab9-bc0b-85fab9a75c3&title=)<br />Zabbix serber配置默认无需修改，直接点击Next step进行下一步<br />![2021-08-26-17-14-51-758605.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629969571821-d950791e-2c72-431b-883c-73c909908050.png#clientId=ud9aeb59c-d6fa-4&from=ui&id=GLxPu&originHeight=411&originWidth=692&originalType=binary&ratio=1&rotation=0&showTitle=false&size=37360&status=done&style=none&taskId=u7f2a5317-9437-4f58-a58a-8acab32edfa&title=)<br />检查所有参数，然后点击Next step进行下一步<br />![2021-08-26-17-14-52-096585.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629969571843-5edc2b41-1dc6-4bdf-a295-8e8b04cfb5c5.png#clientId=ud9aeb59c-d6fa-4&from=ui&id=XupQy&originHeight=417&originWidth=692&originalType=binary&ratio=1&rotation=0&showTitle=false&size=56701&status=done&style=none&taskId=uc00d30e7-75bd-4762-b6f9-60103811374&title=)<br />如下界面则zabbix安装完成，点击Finish完成安装<br />![2021-08-26-17-14-52-405569.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629969593311-d5e7ed5c-241a-442d-8395-c136ff1ea571.png#clientId=ud9aeb59c-d6fa-4&from=ui&id=u398f389a&originHeight=412&originWidth=692&originalType=binary&ratio=1&rotation=0&showTitle=false&size=36835&status=done&style=none&taskId=u739a888b-7c1f-47d7-a71b-d2d1920d230&title=)<br />登录zabbix，默认用户名为Admin，密码为zabbix<br />![2021-08-26-17-14-53-298581.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629969593400-c048704f-3f69-42ae-bb00-dc5199a6b2d9.png#clientId=ud9aeb59c-d6fa-4&from=ui&id=pSMIV&originHeight=375&originWidth=692&originalType=binary&ratio=1&rotation=0&showTitle=false&size=24990&status=done&style=shadow&taskId=ueff4158a-d68d-4b1f-a487-b98713fe151&title=)<br />![2021-08-26-17-14-53-623569.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629969593335-48c3d816-4704-420b-86c4-204f811dd746.png#clientId=ud9aeb59c-d6fa-4&from=ui&id=yBZhj&originHeight=377&originWidth=692&originalType=binary&ratio=1&rotation=0&showTitle=false&size=58801&status=done&style=none&taskId=u48550317-823d-45f9-a190-8c64b603c70&title=)<br />配置中文语言<br />点击左下角User settings，选择Language为Chinese（zh_CN）<br />![2021-08-26-17-14-53-917572.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629969593361-86fc59f3-cd9a-4646-a26a-e4d6a4b510c2.png#clientId=ud9aeb59c-d6fa-4&from=ui&id=RIzMF&originHeight=347&originWidth=692&originalType=binary&ratio=1&rotation=0&showTitle=false&size=26359&status=done&style=none&taskId=ud02a89d0-a691-44ca-b274-c35733058d6&title=)
