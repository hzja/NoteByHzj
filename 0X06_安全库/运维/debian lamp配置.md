<a name="tY5uA"></a>
## 1、动态网卡地址配置
```bash
root@debian:~# cat /etc/network/interfaces
# This file describes the network interfaces available on your system
# and how to activate them. For more information, see interfaces(5).

source /etc/network/interfaces.d/*

# The loopback network interface
auto lo
iface lo inet loopback
auto ens33
iface eth0 inet dhcp
root@debian:~# dhclient
root@debian:~# dhclient
RTNETLINK answers: File exists
root@debian:~# ping www.baidu.com
PING www.a.shifen.com (14.215.177.38) 56(84) bytes of data.
64 bytes from 14.215.177.38 (14.215.177.38): icmp_seq=1 ttl=128 time=40.1 ms
64 bytes from 14.215.177.38 (14.215.177.38): icmp_seq=2 ttl=128 time=48.8 ms

--- www.a.shifen.com ping statistics ---
3 packets transmitted, 2 received, 33.3333% packet loss, time 6ms
rtt min/avg/max/mdev = 40.149/44.469/48.789/4.320 ms
root@debian:~#

```
<a name="mMZb6"></a>
## 2、配置仓库安装Apache
```bash
root@debian:~# grep -Ev "#|^$" /etc/apt/sources.list
deb http://security.debian.org/debian-security buster/updates main contrib
deb-src http://security.debian.org/debian-security buster/updates main contrib
deb https://mirrors.tuna.tsinghua.edu.cn/debian/ buster main contrib non-free
deb https://mirrors.tuna.tsinghua.edu.cn/debian/ buster-updates main contrib non-free
deb https://mirrors.tuna.tsinghua.edu.cn/debian/ buster-backports main contrib non-free
deb https://mirrors.tuna.tsinghua.edu.cn/debian-security buster/updates main contrib non-free
root@debian:~#

root@debian:~# apt install -y apache2
Reading package lists... Done
Building dependency tree
Reading state information... Done
apache2 is already the newest version (2.4.38-3+deb10u4).
0 upgraded, 0 newly installed, 0 to remove and 171 not upgraded.
root@debian:~#

```
<a name="Ptrbf"></a>
## 3、启动服务
```bash
root@debian:~# systemctl start apache2
root@debian:~# systemctl status apache2.service
● apache2.service - The Apache HTTP Server
   Loaded: loaded (/lib/systemd/system/apache2.service; enabled; vendor preset: enabled)
   Active: active (running) since Mon 2021-04-26 06:48:10 EDT; 4min 51s ago
     Docs: https://httpd.apache.org/docs/2.4/
  Process: 528 ExecStart=/usr/sbin/apachectl start (code=exited, status=0/SUCCESS)
 Main PID: 551 (apache2)
    Tasks: 55 (limit: 2318)
   Memory: 11.5M
   CGroup: /system.slice/apache2.service
           ├─551 /usr/sbin/apache2 -k start
           ├─553 /usr/sbin/apache2 -k start
           └─554 /usr/sbin/apache2 -k start


```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1619434437406-c8e9865a-5fd2-4e72-b486-351e3dd311d3.png#clientId=u8510da6d-ef54-4&from=paste&height=288&id=uc5886795&originHeight=575&originWidth=1332&originalType=binary&size=129093&status=done&style=none&taskId=ucddefc80-0720-4017-be9b-fc1b5797645&width=666)
<a name="AlaUc"></a>
## 4、安装mariadb
```c
root@debian:~# apt install -y mariadb-server
root@debian:~# systemctl restart mariadb.service
root@debian:~# mysql_secure_installation
Set root password? [Y/n] y
New password:
Re-enter new password:
Password updated successfully!
Reloading privilege tables..
 ... Success!
 root@debian:~# mysql -uroot -p123
MariaDB [(none)]> CREATE DATABASE wordpress;
MariaDB [(none)]> GRANT ALL ON wordpress.* TO 'wordpress'@'%' IDENTIFIED BY 'wordpress' WITH GRANT OPTION;
MariaDB [(none)]> flush privileges;
MariaDB [(none)]> quit;

```
<a name="UOt6l"></a>
## 5、安装PHP
```bash
root@debian:~# apt install php libapache2-mod-php php-mysql
Reading package lists... Done
Building dependency tree
Reading state information... Done
libapache2-mod-php is already the newest version (2:7.3+69).
php is already the newest version (2:7.3+69).
php-mysql is already the newest version (2:7.3+69).
0 upgraded, 0 newly installed, 0 to remove and 170 not upgraded.
root@debian:~#

```
如果要安装其他PHP模块，可以使用apt-cache命令和grep命令的组合进行搜索和安装，如图所示。
```bash
root@debian:~#  apt-cache search php | egrep 'module' | grep default
libapache2-mod-php - server-side, HTML-embedded scripting language (Apache 2 module) (default)
php-bcmath - Bcmath module for PHP [default]
php-bz2 - bzip2 module for PHP [default]
php-curl - CURL module for PHP [default]
php-dev - Files for PHP module development (default)
php-enchant - Enchant module for PHP [default]
php-gd - GD module for PHP [default]
php-gmp - GMP module for PHP [default]
php-imap - IMAP module for PHP [default]
php-interbase - Interbase module for PHP [default]
php-intl - Internationalisation module for PHP [default]
php-json - JSON module for PHP [default]
php-ldap - LDAP module for PHP [default]
php-mbstring - MBSTRING module for PHP [default]
php-mysql - MySQL module for PHP [default]
php-odbc - ODBC module for PHP [default]
php-pgsql - PostgreSQL module for PHP [default]
php-pspell - pspell module for PHP [default]
php-readline - readline module for PHP [default]
php-recode - recode module for PHP [default]
php-snmp - SNMP module for PHP [default]
php-soap - SOAP module for PHP [default]
php-sqlite3 - SQLite3 module for PHP [default]
php-sybase - Sybase module for PHP [default]
php-tidy - tidy module for PHP [default]
php-xml - DOM, SimpleXML, WDDX, XML, and XSL module for PHP [default]
php-xmlrpc - XMLRPC-EPI module for PHP [default]
php-zip - Zip module for PHP [default]

```
<a name="IkNAE"></a>
## 6、添加测试文件查看验证PHP
```bash
<?php
phpinfo();
?>
root@debian:~# cd /var/www/html/
root@debian:/var/www/html# vim info.php
root@debian:/var/www/html# cat info.php
<? php
phpinfo();
?>
root@debian:/var/www/html# systemctl reload ap
apache2.service   apparmor.service
root@debian:/var/www/html# systemctl reload apache2.service
root@debian:/var/www/html#

```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1619435401582-2861476f-d2da-4deb-976a-0349a6e5a9d1.png#clientId=u8510da6d-ef54-4&from=paste&height=236&id=u4a3777ab&originHeight=472&originWidth=1577&originalType=binary&size=77600&status=done&style=none&taskId=u84dc4151-e59f-47a9-97f1-b42dcd54f86&width=788.5)
<a name="NxRca"></a>
## 7、安装wordoress测试
[https://cn.wordpress.org/download/](https://cn.wordpress.org/download/)
```bash
root@debian:~# cd /var/www/html/
root@debian:/var/www/html#
root@debian:/var/www/html# root@debian:~# wget https://cn.wordpress.org/latest-zh_CN.tar.gz
```
解压之后打开给一个755的权限然后打开浏览器<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1619436070070-ba651dad-14f2-43a9-9e2b-8d8833f56893.png#clientId=ue6961f15-72e3-4&from=paste&height=387&id=u765b65bd&originHeight=773&originWidth=1142&originalType=binary&size=93966&status=done&style=none&taskId=u1b7874dd-cdbb-4236-9783-1cca404f570&width=571)![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1619436089499-1325b676-02cf-4cc6-a4fc-74ac539a8822.png#clientId=ue6961f15-72e3-4&from=paste&height=384&id=u76a3c690&originHeight=768&originWidth=1124&originalType=binary&size=76690&status=done&style=none&taskId=u189a60fd-5863-4288-ba9f-9ed050c0580&width=562)![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1619436110320-2ac7a5f5-1ec5-4a4f-866e-856a82ac32dc.png#clientId=ue6961f15-72e3-4&from=paste&height=419&id=u7366e15d&originHeight=838&originWidth=1052&originalType=binary&size=62574&status=done&style=none&taskId=ufc1696d9-f03d-4b16-85e6-3d157b317b3&width=526)<br />将内容写入到文件当中
```bash
root@debian:/var/www/html# cat wp-config.php
<?php
/**
 * The base configuration for WordPress
 *
 * The wp-config.php creation script uses this file during the
 * installation. You don't have to use the web site, you can
 * copy this file to "wp-config.php" and fill in the values.
 *
 * This file contains the following configurations:
 *
 * * MySQL settings
 * * Secret keys
 * * Database table prefix
 * * ABSPATH
 *
 * @link https://wordpress.org/support/article/editing-wp-config-php/
 *
 * @package WordPress
 */


```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1619436274022-72bf425a-2b22-4bac-83ef-f21150ae7803.png#clientId=ue6961f15-72e3-4&from=paste&height=426&id=u5f2ee4db&originHeight=851&originWidth=870&originalType=binary&size=78493&status=done&style=none&taskId=uf7549f1d-9c60-4724-8a5d-4764741ba3a&width=435)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1619436306620-f9e62846-4902-40ca-8235-7d04298d4cae.png#clientId=ue6961f15-72e3-4&from=paste&height=427&id=u142769d3&originHeight=854&originWidth=864&originalType=binary&size=78449&status=done&style=none&taskId=u2103d62f-2d7d-4bfc-96a4-37d81c5bbc3&width=432)

密码： m3ZnPA(howdPNmR^6L<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1619436367389-25374c35-1e08-4d30-b630-e2ae234a2f60.png#clientId=ue6961f15-72e3-4&from=paste&height=285&id=u4aa73bb6&originHeight=569&originWidth=565&originalType=binary&size=22622&status=done&style=none&taskId=u96334621-303d-4447-9acb-a442d69d948&width=282.5)<br />注意：要是打开的主页是apace2的默认首页在虚拟主机文件中添加索引文件，然后重启服务即可解决。

```bash
root@debian:/etc/apache2/sites-available# pwd
/etc/apache2/sites-available
root@debian:/etc/apache2/sites-available# grep -Ev "#|^$" 000-default.conf
<VirtualHost *:80>
        ServerAdmin webmaster@localhost
        DocumentRoot /var/www/html
        DirectoryIndex  index.php		#添加为index.php
        ErrorLog ${APACHE_LOG_DIR}/error.log
        CustomLog ${APACHE_LOG_DIR}/access.log combined
</VirtualHost>
root@debian:/etc/apache2/sites-available#

```
<a name="keDRe"></a>
## 8、添加域名解析
由于没有做dns解析我们添加hosts文件做dns
```bash
root@debian:/etc/apache2/sites-enabled# cat /etc/hosts
127.0.0.1       localhost
127.0.0.1       debian.debian   debian

# The following lines are desirable for IPv6 capable hosts
::1     localhost ip6-localhost ip6-loopback
ff02::1 ip6-allnodes
ff02::2 ip6-allrouters
127.0.0.1       www.example.com	添加此行
root@debian:/etc/apache2/sites-enabled#
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1619512083973-18aafc8d-aedd-42e5-b336-e6d27c9de066.png#clientId=u6ee1d8e6-3e48-4&from=paste&height=183&id=u5036c057&originHeight=366&originWidth=790&originalType=binary&size=28931&status=done&style=none&taskId=u05b99555-a674-4970-b87a-5dfbaf8dd32&width=395)
<a name="LMsd5"></a>
## 9、做https访问
```bash
root@debian:~# mkdir /etc/pki/tls/certs/ -p
root@debian:~# cd /etc/pki/tls/certs/
root@debian:/etc/pki/tls/certs# openssl req -x509 -nodes -newkey rsa:2048 -keyout example.com.key -out example.com.crt
Generating a RSA private key
.................................................................+++++
.....................................................................................+++++
writing new private key to 'example.com.key'
-----
You are about to be asked to enter information that will be incorporated
into your certificate request.
What you are about to enter is what is called a Distinguished Name or a DN.
There are quite a few fields but you can leave some blank
For some fields there will be a default value,
If you enter '.', the field will be left blank.
-----
Country Name (2 letter code) [AU]:cn
State or Province Name (full name) [Some-State]:cn
Locality Name (eg, city) []:cd
Organization Name (eg, company) [Internet Widgits Pty Ltd]:oldjiang
Organizational Unit Name (eg, section) []:cd
Common Name (e.g. server FQDN or YOUR name) []:www.example.com
Email Address []:857@qq.com
root@debian:/etc/pki/tls/certs#


root@debian:/etc/apache2/sites-enabled# cat example.conf
<IfModule mod_ssl.c>
        <VirtualHost _default_:443>
                ServerAdmin 857@qq.com

                DocumentRoot /var/www/html
                ServerName www.example.com

                ErrorLog ${APACHE_LOG_DIR}/error.log
                CustomLog ${APACHE_LOG_DIR}/access.log combined

                DirectoryIndex  index.php
                SSLEngine on
                # 注意，需要添加这两行
                SSLCertificateFile /etc/pki/tls/certs/example.com.crt
                SSLCertificateKeyFile /etc/pki/tls/certs/example.com.key

                <FilesMatch "\.(cgi|shtml|phtml|php)$">
                                SSLOptions +StdEnvVars
                </FilesMatch>
                <Directory /usr/lib/cgi-bin>
                                SSLOptions +StdEnvVars
                </Directory>
        </VirtualHost>
</IfModule>
root@debian:/etc/apache2/sites-enabled#

root@debian:/etc/apache2/sites-enabled# apachectl configtest
AH00558: apache2: Could not reliably determine the server's fully qualified domain name, using 127.0.0.1. Set the 'ServerName' directive globally to suppress this message
Syntax OK

root@debian:/etc/apache2/sites-enabled# tail -1 /etc/apache2/apache2.conf
ServerName localhost:80 #添加这一行

root@debian:/etc/apache2/sites-enabled# apachectl configtest
Syntax OK

root@debian:/etc/apache2/sites-enabled# systemctl restart apache2.service
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1619513343229-ba5d7536-953b-4ca7-9797-056657385996.png#clientId=u6ee1d8e6-3e48-4&from=paste&height=371&id=u351308d2&originHeight=741&originWidth=1085&originalType=binary&size=60606&status=done&style=none&taskId=u060ec100-9bb9-4d81-9d7e-d0a934416b4&width=542.5)
<a name="xtQbv"></a>
## 10、解决有时候域名跳转为ip
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1619522925293-b3444954-668f-44b5-8cff-8dc4e10bcaa6.png#clientId=u37891f58-458d-4&from=paste&height=234&id=u36f833db&originHeight=467&originWidth=956&originalType=binary&size=46044&status=done&style=none&taskId=u47838650-09be-47ea-9cd2-175c2245d5b&width=478)<br />将ip换为你使用的域名就可以正常使用了
<a name="uLE7g"></a>
## 11、解决ftp下载主题的问题

  网上很多的方法都是雷同、都是在wp-content创建一个tmp的文件夹然后在修改<br />例如：[https://www.fujieace.com/wordpress/ftp-sftp.html](https://www.fujieace.com/wordpress/ftp-sftp.html)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1619523077828-1c951ec4-b5ab-46ad-a47e-1141d771bf22.png#clientId=u37891f58-458d-4&from=paste&height=299&id=uaf46cd39&originHeight=598&originWidth=542&originalType=binary&size=60095&status=done&style=none&taskId=ud1f41311-8317-4607-bdf8-bbab339dc44&width=271)<br />经过我的实验发现好像都没有生效，在上面的实验都做了没有实现之后，在完成下面的文件的添加
```bash
root@debian:/var/www/html/wp-content# pwd
/var/www/html/wp-content
root@debian:/var/www/html/wp-content# tail -3 ../wp-config.php
define("FS_METHOD", "direct");		#添加这三行
define("FS_CHMOD_DIR", 0777);			#添加这三行
define("FS_CHMOD_FILE", 0777);		#添加
root@debian:/var/www/html/wp-content#
```
然后就解决了
