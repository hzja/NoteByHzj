Linux 邮件
<a name="cFpKe"></a>
### Msmtp官网
[https://marlam.de/msmtp/](https://marlam.de/msmtp/download/)<br />Msmtp下载地址：[https://marlam.de/msmtp/download/](https://marlam.de/msmtp/download/)
<a name="ISO7w"></a>
### Msmtp的下载安装
<a name="v81qy"></a>
#### 1、下载源码解压并编译
```bash
wget https://marlam.de/msmtp/releases/msmtp-1.8.12.tar.xz
xz -d msmtp-1.8.12.tar.xz
tar -xf msmtp-1.8.12.tar
cd msmtp-1.8.12
./configure --prefix=/usr/local/msmtp
make && make install
```
<a name="5kmPY"></a>
#### 2、创建msmtp配置文件和日志文件（host为邮件域名，邮件用户名test，密码123456）
```bash
$ vim ~/.msmtprc

account default 
host 126.com 
from test@126.com 
auth login 
user test 
password 123456 
logfile ~/.msmtp.log 

$ chmod 600 ~/.msmtprc
$ touch ~/.msmtp.log
```
<a name="zrKnH"></a>
#### 3、mutt安装配置：（一般linux下有默认安装mutt）
```bash
$ yum install mutt
$ vim ~/.muttrc

set sendmail="/usr/local/msmtp/bin/msmtp" 
set use_from=yes 
set realname="memory" 
set from=test@126.com 
set envelope_from=yes 
set rfc2047_parameters=yes 
set charset="utf-8" 
```
<a name="5oWt0"></a>
### 测试邮件发送
```bash
echo "邮件内容123456" | mutt -s "邮件标题测试邮件" -a /scripts/test.txt test@126.com
```

