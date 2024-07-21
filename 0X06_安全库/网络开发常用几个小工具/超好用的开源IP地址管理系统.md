网络
<a name="IKqj7"></a>
## 一、前言：
对于运维管理人员，ip地址进行管理很重要，很多公司都是采用电子文档的形式，以手工更新为主，对ip地址和子网的实际使用情况无法进行有效的实时监控和统计，随着网络变得越来越大，ip设备越来越多，手工IP地址管理将会成为网络管理和扩展的瓶颈。
<a name="EjybK"></a>
## 二、软件介绍：
phpIPAM是一个开源的Web IP地址管理应用程序(IPAM)。其目标是提供轻松，现代和有用的IP地址管理。它是基于php的应用程序，带有MySQL数据库后端，使用jQuery库，ajax和HTML5 / CSS3功能。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650944351512-c506ce46-1861-46a9-9fa9-de0c9efdb5c5.png#clientId=u83261eb6-d0e1-4&from=paste&id=ue5ebdc98&originHeight=338&originWidth=1020&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2045fd89-178d-44b1-bacb-d1524a960d0&title=)
<a name="apzuC"></a>
## 三、搭建系统：
官方安装教程指南链接：[https://phpipam.net/documents/installation/](https://phpipam.net/documents/installation/)
<a name="L2Keh"></a>
### 1、设置系统环境和安装依赖包
<a name="N0jHA"></a>
#### ①禁用SELINUX，并重启服务器
```bash
sed -i '/SELINUX/s/enforcing/disabled/' /etc/selinux/config && reboot
```
<a name="yJVMW"></a>
#### ②关闭防火墙
```bash
systemctl stop firewalld
```
<a name="UfqQe"></a>
#### ③安装依赖包
```bash
yum install epel-release -y
yum install wget vim net-tools httpd mariadb-server php php-cli php-gd php-common php-ldap php-pdo php-pear php-snmp php-xml php-mysql php-mbstring git -y
```
<a name="UN0bK"></a>
### 2、配置和运行 Apache
<a name="UQ8Jx"></a>
#### ①修改apache配置文件/etc/httpd/conf/httpd.conf，主要是修改以下两个地方
```bash
ServerName localhost:80
<Directory "/var/www/html">    #修改目录设置以允许mod_rewrite URL重写
  Options FollowSymLinks
  AllowOverride all
  Order allow,deny
  Allow from all
  #Require all granted
</Directory>
```
<a name="J5VHQ"></a>
#### ②检查配置文件，显示 OK 状态为正常
```bash
httpd -t -f /etc/httpd/conf/httpd.conf
```
<a name="yHBWf"></a>
#### ③修改/etc/php.ini配置文件的时区为Asia/Shanghai
```
date.timezone = Asia/Shanghai
```
<a name="VedCx"></a>
#### ④启动httpd并配置开机启动
```bash
#开机启动Apache
systemctl enable httpd
#启动Apache
systemctl start httpd
```
<a name="p8ELt"></a>
### 3、初始化配置mariaDB
<a name="Ca4aD"></a>
#### ①启动mariadb数据库
```bash
#启动mariadb
systemctl start mariadb
#开机启动mariadb
systemctl enable mariadb
```
<a name="LK5pE"></a>
#### ②初始化mariaDB，配置好密码
```
mysql_secure_installation 
```
<a name="fhE5P"></a>
### 4、下载和配置phpipam
<a name="Jb0FH"></a>
#### ①下载phpipam文件代码到/var/www/html
```bash
cd /var/www/html/
git clone https://github.com/phpipam/phpipam.git .
git checkout 1.4
```
<a name="EakkO"></a>
#### ②文件权限配置
```bash
chown apache:apache -R /var/www/html/
```
<a name="cHrLc"></a>
#### ③拷贝必要文件到相关目录
```bash
cp /var/www/html/config.dist.php /var/www/html/config.dist.php.bak
mv /var/www/html/config.dist.php /var/www/html/config.php
```
<a name="hFXB5"></a>
#### ④编辑/var/www/html/config.php文件
```bash
define('BASE', "/phpipam");   #修改这行
```
<a name="XjAq7"></a>
#### ⑤重启httpd服务
```bash
systemctl restart httpd
```
<a name="eN0vc"></a>
### 5、配置phpipam
打开地址：http://服务器IP/phpipam
<a name="mqvCk"></a>
#### ①选择新的phpipam安装
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650944351506-81465663-80ff-4d4a-b72e-1bf12b909046.png#clientId=u83261eb6-d0e1-4&from=paste&id=uc3e40c6a&originHeight=656&originWidth=978&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u074f875c-9709-42e8-93f4-73c16217689&title=)
<a name="LFGXS"></a>
#### ② 安装pfpipam数据库
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650944351672-b8db81d8-083e-4f9a-89f3-b65845dc8fe9.png#clientId=u83261eb6-d0e1-4&from=paste&id=u2b20be26&originHeight=589&originWidth=859&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u388b0a5a-b022-4c6f-904c-fca05a5f9d3&title=)
<a name="MZ2Jz"></a>
#### ③设置数据库
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650944351644-d8938d2c-6175-40e0-bd70-8aa1c9aa35ea.png#clientId=u83261eb6-d0e1-4&from=paste&id=ud6ee6528&originHeight=576&originWidth=879&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u13c5f69f-f915-4839-aa49-d2d9cfefded&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650944351508-7b4ccaee-4da3-41e5-88af-d2aaaeb1d5cb.png#clientId=u83261eb6-d0e1-4&from=paste&id=u24d89149&originHeight=585&originWidth=663&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ubb2b2d4f-771f-4abc-bd0b-75e16066587&title=)
<a name="HSyFu"></a>
#### ④填写系统初始化信息
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650944351814-9473ffcf-eaf9-4778-9438-84b9dc6e8229.png#clientId=u83261eb6-d0e1-4&from=paste&id=u06c6b2df&originHeight=472&originWidth=604&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uccd02aec-b829-4224-ba20-590301ea9f4&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650944352243-732587cc-3dbb-4c1e-87e3-a5558aad4fbc.png#clientId=u83261eb6-d0e1-4&from=paste&id=uc161e46f&originHeight=551&originWidth=656&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1171cfbc-87e2-4b6c-9e1e-b0dd0d96bdc&title=)
<a name="o4U99"></a>
#### ⑤登录系统
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650944352095-5388e18f-f5d2-4969-8cf6-08ded09bf6cd.png#clientId=u83261eb6-d0e1-4&from=paste&id=u7e99e711&originHeight=534&originWidth=917&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u706e8876-fb75-417a-bc91-0d96e82602a&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650944352351-c6153dd3-13d4-4430-b33e-e281aac8e4d1.png#clientId=u83261eb6-d0e1-4&from=paste&id=uc4d9aa2f&originHeight=533&originWidth=705&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8a789885-a0ab-4710-a388-a21437e0f8f&title=)
<a name="QTWlB"></a>
## 四、使用教程：
<a name="PZR5e"></a>
### ①登录成功后会自动跳转到主界面
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650944352396-975b7f69-2c58-4a5a-b931-d1e9ef2a335c.png#clientId=u83261eb6-d0e1-4&from=paste&id=u0a2490bf&originHeight=481&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uada682ae-8545-48b7-ae80-7d5669f4e29&title=)
<a name="R90Dm"></a>
### ②修改 admin 语言，改成中文（如图操作），修改后需要退出重新登陆
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650944352362-0ca54ae4-da28-404a-b6e1-82ea0b2bb776.png#clientId=u83261eb6-d0e1-4&from=paste&id=u7ef72baf&originHeight=649&originWidth=614&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uee421198-0a7f-4cb2-b995-1b1ef004837&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650944352768-9bd12ef4-e3ec-46c9-8b55-d805ae0ad145.png#clientId=u83261eb6-d0e1-4&from=paste&id=u05a0e64e&originHeight=711&originWidth=1026&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub8fb0c4e-1e13-49aa-ba21-b174053a55c&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650944352837-3bf3463f-55f2-4346-a717-9aa7e18178cc.png#clientId=u83261eb6-d0e1-4&from=paste&id=u3345ede2&originHeight=667&originWidth=852&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ufe9494ff-6f9a-4b04-89f6-b4e81a3f2cf&title=)
<a name="JqkJz"></a>
### ③子网创建（添加），默认会自带一些测试子网，可以手动删除
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650944353003-fde45365-ff09-43e8-b17a-299375e26955.png#clientId=u83261eb6-d0e1-4&from=paste&id=u185f3c5b&originHeight=477&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub612745b-164d-4614-8b93-4a7b7d30461&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650944353072-c0eea288-0af4-4364-848c-74bd705698cb.png#clientId=u83261eb6-d0e1-4&from=paste&id=ufee77e53&originHeight=395&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u54b59cf4-9cbb-4120-bcc5-f2ab16b6a77&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650944353005-e11c503a-45e1-43fd-b378-0eaa201fe08a.png#clientId=u83261eb6-d0e1-4&from=paste&id=ucc4bcc3e&originHeight=444&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uddf58ce7-35cc-460b-ae29-1752a82c584&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650944353421-368311ee-2bdf-4658-8314-e4d06976c830.png#clientId=u83261eb6-d0e1-4&from=paste&id=uf31aa3d0&originHeight=479&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4110be98-40cc-4808-b161-05fdcb19161&title=)
<a name="mTq8w"></a>
### ④子网页面预览
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650944353603-fdeeffcb-b121-4d52-90ca-7a88f70a42b9.png#clientId=u83261eb6-d0e1-4&from=paste&id=u1dbb3c0e&originHeight=497&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9df4f810-f3e8-42ee-8d78-cef3b228b7b&title=)
<a name="qbaqh"></a>
### ⑤选中对应的 IP 可以进行编辑
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650944353665-d887e32c-ac8f-431b-9923-63492b662618.png#clientId=u83261eb6-d0e1-4&from=paste&id=u5a629b0a&originHeight=552&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0db4a7a8-310e-46a3-8295-b0ecb6b936f&title=)
