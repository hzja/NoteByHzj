Linux logrotate 日志管理<br />logrotate 程序是一个日志文件管理工具。用于分割日志文件，删除旧的日志文件，并创建新的日志文件，起到“转储”作用。可以节省磁盘空间。下面就对 logrotate 日志轮转操作做一梳理记录。
<a name="PEwO1"></a>
## 1、配置文件介绍
Linux系统默认安装logrotate工具，它默认的配置文件在：
```bash
/etc/logrotate.conf
/etc/logrotate.d/
```
logrotate.conf 才主要的配置文件，logrotate.d 是一个目录，该目录里的所有文件都会被主动的读入/etc/logrotate.conf中执行。<br />另外，如果 /etc/logrotate.d/ 里面的文件中没有设定一些细节，则会以/etc/logrotate.conf这个文件的设定来作为默认值。<br />Logrotate是基于CRON来运行的，其脚本是/etc/cron.daily/logrotate，日志轮转是系统自动完成的。实际运行时，Logrotate会调用配置文件/etc/logrotate.conf。可以在/etc/logrotate.d目录里放置自定义好的配置文件，用来覆盖Logrotate的缺省值。
```bash
[root@fcant ~]# cat /etc/cron.daily/logrotate
#!/bin/sh
/usr/sbin/logrotate /etc/logrotate.conf >/dev/null 2>&1
EXITVALUE=$?
if [ $EXITVALUE != 0 ]; then
    /usr/bin/logger -t logrotate "ALERT exited abnormally with [$EXITVALUE]"
fi
exit 0
```
如果等不及cron自动执行日志轮转，想手动强制切割日志，需要加-f参数；不过正式执行前最好通过Debug选项来验证一下（-d参数），这对调试也很重要：
```bash
# /usr/sbin/logrotate -f /etc/logrotate.d/nginx
# /usr/sbin/logrotate -d -f /etc/logrotate.d/nginx
```
logrotate 命令格式：
```
logrotate [OPTION...] <configfile>
-d, --debug ：debug模式，测试配置文件是否有错误。
-f, --force ：强制转储文件。
-m, --mail=command ：压缩日志后，发送日志到指定邮箱。
-s, --state=statefile ：使用指定的状态文件。
-v, --verbose ：显示转储过程。
```
根据日志切割设置进行操作，并显示详细信息：
```bash
[root@fcant ~]# /usr/sbin/logrotate -v /etc/logrotate.conf
[root@fcant ~]# /usr/sbin/logrotate -v /etc/logrotate.d/php
```
根据日志切割设置进行执行，并显示详细信息,但是不进行具体操作，debug模式
```bash
[root@fcant ~]# /usr/sbin/logrotate -d /etc/logrotate.conf
[root@fcant ~]# /usr/sbin/logrotate -d /etc/logrotate.d/nginx
```
查看各log文件的具体执行情况
```bash
[root@fcant ~]# cat /var/lib/logrotate.status
```
<a name="69e80af5"></a>
## 2、切割介绍
比如以系统日志/var/log/message做切割来简单说明下：

- 第一次执行完rotate(轮转)之后，原本的messages会变成messages.1，而且会制造一个空的messages给系统来储存日志；
- 第二次执行之后，messages.1会变成messages.2，而messages会变成messages.1，又造成一个空的messages来储存日志！<br />

如果仅设定保留三个日志（即轮转3次）的话，那么执行第三次时，则 messages.3这个档案就会被删除，并由后面的较新的保存日志所取代！也就是会保存最新的几个日志。<br />日志究竟轮换几次，这个是根据配置文件中的dateext 参数来判定的。
<a name="ZrCpz"></a>
### logrotate.conf的配置
```
# cat /etc/logrotate.conf
# 底下的设定是 "logrotate 的默认值" ，如果別的文件设定了其他的值，
# 就会以其它文件的设定为主
weekly          //默认每一周执行一次rotate轮转工作
rotate 4       //保留多少个日志文件(轮转几次).默认保留四个.就是指定日志文件删除之前轮转的次数，0 指没有备份
create         //自动创建新的日志文件，新的日志文件具有和原来的文件相同的权限；因为日志被改名,因此要创建一个新的来继续存储之前的日志
dateext       //这个参数很重要！就是切割后的日志文件以当前日期为格式结尾，如xxx.log-20131216这样,如果注释掉,切割出来是按数字递增,即前面说的 xxx.log-1这种格式
compress      //是否通过gzip压缩转储以后的日志文件，如xxx.log-20131216.gz ；如果不需要压缩，注释掉就行
include /etc/logrotate.d
# 将 /etc/logrotate.d/ 目录中的所有文件都加载进来
/var/log/wtmp {                 //仅针对 /var/log/wtmp 所设定的参数
monthly                    //每月一次切割,取代默认的一周
minsize 1M              //文件大小超过 1M 后才会切割
create 0664 root utmp            //指定新建的日志文件权限以及所属用户和组
rotate 1                    //只保留一个日志.
}
# 这个 wtmp 可记录用户登录系统及系统重启的时间
# 因为有 minsize 的参数，因此不见得每个月一定会执行一次喔.要看文件大小。
```
由这个文件的设定可以知道/etc/logrotate.d其实就是由/etc/logrotate.conf 所规划出来的目录，虽然可以将所有的配置都写入 /etc/logrotate.conf ，但是这样一来这个文件就实在是太复杂了，尤其是当使用很多的服务在系统上面时， 每个服务都要去修改 /etc/logrotate.conf 的设定也似乎不太合理了。<br />所以，如果独立出来一个目录，那么每个要切割日志的服务， 就可以独自成为一个文件，并且放置到 /etc/logrotate.d/ 当中。
<a name="O73s0"></a>
### 其他重要参数说明
```
compress                                   通过gzip 压缩转储以后的日志
nocompress                                不做gzip压缩处理
copytruncate                              用于还在打开中的日志文件，把当前日志备份并截断；是先拷贝再清空的方式，拷贝和清空之间有一个时间差，可能会丢失部分日志数据。
nocopytruncate                           备份日志文件不过不截断
create mode owner group             轮转时指定创建新文件的属性，如create 0777 nobody nobody
nocreate                                    不建立新的日志文件
delaycompress                           和compress 一起使用时，转储的日志文件到下一次转储时才压缩
nodelaycompress                        覆盖 delaycompress 选项，转储同时压缩。
missingok                                 如果日志丢失，不报错继续滚动下一个日志
errors address                           专储时的错误信息发送到指定的Email 地址
ifempty                                    即使日志文件为空文件也做轮转，这个是logrotate的缺省选项。
notifempty                               当日志文件为空时，不进行轮转
mail address                             把转储的日志文件发送到指定的E-mail 地址
nomail                                     转储时不发送日志文件
olddir directory                         转储后的日志文件放入指定的目录，必须和当前日志文件在同一个文件系统
noolddir                                   转储后的日志文件和当前日志文件放在同一个目录下
sharedscripts                           运行postrotate脚本，作用是在所有日志都轮转后统一执行一次脚本。如果没有配置这个，那么每个日志轮转后都会执行一次脚本
prerotate                                 在logrotate转储之前需要执行的指令，例如修改文件的属性等动作；必须独立成行
postrotate                               在logrotate转储之后需要执行的指令，例如重新启动 (kill -HUP) 某个服务！必须独立成行
daily                                       指定转储周期为每天
weekly                                    指定转储周期为每周
monthly                                  指定转储周期为每月
rotate count                            指定日志文件删除之前转储的次数，0 指没有备份，5 指保留5 个备份
dateext                                  使用当期日期作为命名格式
dateformat .%s                       配合dateext使用，紧跟在下一行出现，定义文件切割后的文件名，必须配合dateext使用，只支持 %Y %m %d %s 这四个参数
size(或minsize) log-size            当日志文件到达指定的大小时才转储，log-size能指定bytes(缺省)及KB (sizek)或MB(sizem).
当日志文件 >= log-size 的时候就转储。 以下为合法格式：（其他格式的单位大小写没有试过）
size = 5 或 size 5 （>= 5 个字节就转储）
size = 100k 或 size 100k
size = 100M 或 size 100M
```
<a name="zXzXu"></a>
### 操作示例
小示例：下面一个切割nginx日志的配置
```bash
[root@fcant ~]# vim /etc/logrotate.d/nginx
/usr/local/nginx/logs/*.log {
daily
rotate 7
missingok
notifempty
dateext
sharedscripts
postrotate
    if [ -f /usr/local/nginx/logs/nginx.pid ]; then
        kill -USR1 `cat /usr/local/nginx/logs/nginx.pid`
    fi
endscript
}
```
<a name="LKUG9"></a>
#### nginx日志切割处理脚本
<a name="xTvtr"></a>
##### 1）logrotate日志分割配置
```bash
[root@fcant ~# vim /etc/logrotate.d/nginx
/data/nginx_logs/*.access_log        
{
nocompress                                   
daily                                  
copytruncate                                  
create                              
ifempty                                   
olddir /data/nginx_logs/days           
rotate 0                                        
}
```
<a name="jP1Cq"></a>
##### 2）日志分割脚本
```bash
[root@fcant ~# vim /usr/local/sbin/logrotate-nginx.sh
#!/bin/bash
#创建转储日志压缩存放目录
mkdir -p /data/nginx_logs/days
#手工对nginx日志进行切割转换
/usr/sbin/logrotate -vf /etc/logrotate.d/nginx
#当前时间
time=$(date -d "yesterday" +"%Y-%m-%d")
#进入转储日志存放目录
cd /data/nginx_logs/days
#对目录中的转储日志文件的文件名进行统一转换
for i in $(ls ./ | grep "^\(.*\)\.[[:digit:]]$")
do
mv ${i} ./$(echo ${i}|sed -n 's/^\(.*\)\.\([[:digit:]]\)$/\1/p')-$(echo $time)
done
#对转储的日志文件进行压缩存放，并删除原有转储的日志文件，只保存压缩后的日志文件。以节约存储空间
for i in $(ls ./ | grep "^\(.*\)\-\([[:digit:]-]\+\)$")
do
tar jcvf ${i}.bz2 ./${i}
rm -rf ./${i}
done
#只保留最近7天的压缩转储日志文件
find /data/nginx_logs/days/* -name "*.bz2" -mtime 7 -type f -exec rm -rf {} \;
```
<a name="6uv7I"></a>
##### 3）crontab定时执行
```
[root@fcant ~# crontab -e
#logrotate
0 0 * * * /bin/bash -x /usr/local/sbin/logrotate-nginx.sh > /dev/null 2>
```
手动执行脚本，测试下看看：
```bash
[root@fcant ~# /bin/bash -x /usr/local/sbin/logrotate-nginx.sh
[root@fcant ~# cd /data/nginx_logs/days
[root@fcant days# ls
huantest.access_log-2017-01-18.bz2
```
<a name="Vvj3w"></a>
#### php脚本切割案例
```bash
[root@fcant ~]# cat /etc/logrotate.d/php
/Data/logs/php/*log {
    daily
    rotate 365
    missingok
    notifempty
    compress
    dateext
    sharedscripts
    postrotate
        if [ -f /Data/app/php5.6.26/var/run/php-fpm.pid ]; then
            kill -USR1 `cat /Data/app/php5.6.26/var/run/php-fpm.pid`
        fi
    endscript
    postrotate
        /bin/chmod 644 /Data/logs/php/*gz
    endscript
}
[root@fcant ~]# ll /Data/app/php5.6.26/var/run/php-fpm.pid
-rw-r--r-- 1 root root 4 Dec 28 17:03 /Data/app/php5.6.26/var/run/php-fpm.pid
[root@fcant ~]# cd /Data/logs/php
[root@fcant php]# ll
total 25676
-rw-r--r-- 1 root   root         0 Jun  1  2016 error.log
-rw-r--r-- 1 nobody nobody     182 Aug 30  2015 error.log-20150830.gz
-rw-r--r-- 1 nobody nobody     371 Sep  1  2015 error.log-20150901.gz
-rw-r--r-- 1 nobody nobody     315 Sep  7  2015 error.log-20150907.gz
.........
.........
```
<a name="sn3Sj"></a>
#### nginx日志切割案例
```bash
[root@fcant ~]# cat /etc/logrotate.d/nginx
/Data/logs/nginx/*/*log {
    daily
    rotate 365
    missingok
    notifempty
    compress
    dateext
    sharedscripts
    postrotate
    /etc/init.d/nginx reload
    endscript
}
[root@fcant ~]# ll /Data/logs/nginx/www.huanqiu.com/
..........
-rw-r--r-- 1 root root      1652 Jan  1 00:00 error.log-20170101.gz
-rw-r--r-- 1 root root      1289 Jan  2 00:00 error.log-20170102.gz
-rw-r--r-- 1 root root      1633 Jan  3 00:00 error.log-20170103.gz
-rw-r--r-- 1 root root      3239 Jan  4 00:00 error.log-20170104.gz
```
<a name="6deb3a92"></a>
#### 系统日志切割案例
```bash
[root@fcant ~]# cat /etc/logrotate.d/syslog
/var/log/cron
/var/log/maillog
/var/log/messages
/var/log/secure
/var/log/spooler
{
    sharedscripts
    postrotate
    /bin/kill -HUP `cat /var/run/syslogd.pid 2> /dev/null` 2> /dev/null || true
    endscript
}
[root@fcant ~]# ll /var/log/messages*
-rw------- 1 root root 34248975 Jan 19 18:42 /var/log/messages
-rw------- 1 root root 51772994 Dec 25 03:11 /var/log/messages-20161225
-rw------- 1 root root 51800210 Jan  1 03:05 /var/log/messages-20170101
-rw------- 1 root root 51981366 Jan  8 03:36 /var/log/messages-20170108
-rw------- 1 root root 51843025 Jan 15 03:40 /var/log/messages-20170115
[root@fcant ~]# ll /var/log/cron*
-rw------- 1 root root 2155681 Jan 19 18:43 /var/log/cron
-rw------- 1 root root 2932618 Dec 25 03:11 /var/log/cron-20161225
-rw------- 1 root root 2939305 Jan  1 03:06 /var/log/cron-20170101
-rw------- 1 root root 2951820 Jan  8 03:37 /var/log/cron-20170108
-rw------- 1 root root 3203992 Jan 15 03:41 /var/log/cron-20170115
[root@fcant ~]# ll /var/log/secure*
-rw------- 1 root root  275343 Jan 19 18:36 /var/log/secure
-rw------- 1 root root 2111936 Dec 25 03:06 /var/log/secure-20161225
-rw------- 1 root root 2772744 Jan  1 02:57 /var/log/secure-20170101
-rw------- 1 root root 1115543 Jan  8 03:26 /var/log/secure-20170108
-rw------- 1 root root  731599 Jan 15 03:40 /var/log/secure-20170115
[root@fcant ~]# ll /var/log/spooler*
-rw------- 1 root root 0 Jan 15 03:41 /var/log/spooler
-rw------- 1 root root 0 Dec 18 03:21 /var/log/spooler-20161225
-rw------- 1 root root 0 Dec 25 03:11 /var/log/spooler-20170101
-rw------- 1 root root 0 Jan  1 03:06 /var/log/spooler-20170108
-rw------- 1 root root 0 Jan  8 03:37 /var/log/spooler-20170115
```
<a name="pkNQB"></a>
#### tomcat日志切割案例
```bash
[root@fcant ~]# cat /etc/logrotate.d/tomcat
/Data/app/tomcat-7-huanqiu/logs/catalina.out {
rotate 14
daily
copytruncate
compress
notifempty
missingok
}
[root@fcant ~]# ll /Data/app/tomcat-7-huanqiu/logs/catalina.*
-rw-r--r--. 1 root root     0 Jan 19 19:11 /Data/app/tomcat-7-huanqiu/logs/catalina.out
-rw-r--r--. 1 root root 95668 Jan 19 19:11 /Data/app/tomcat-7-huanqiu/logs/catalina.out.1.gz
```
<a name="hHxkB"></a>
#### 早期用过的nginx日志处理案例
```bash
[root@fcant ~]# vim /letv/sh/cut_nginx_log.sh
#!/bin/bash
# 你的日志文件存放目录
logs_path="/letv/logs/"
# 日志文件的名字，多个需要空格隔开
logs_names=(error access pv_access)
dates=`date -d "yesterday" +"%Y%m%d"`
mkdir -p ${logs_path}$dates/
num=${#logs_names[@]}
for((i=0;i<num;i++));do
mv ${logs_path}${logs_names[i]}.log ${logs_path}$dates/${logs_names[i]}.log
done
#nginx平滑重启
kill -USR1 `cat /letv/logs/nginx/nginx.pid`           
结合crontab定时执行
[root@fcant ~]# crontab -e
#nginx日志切割
00 00 * * * cd /letv/logs;/bin/bash /letv/sh/cut_nginx_log.sh > /dev/null 2>$1
```
<a name="38E6j"></a>
## 3、尝试解决logrotate无法自动轮询日志的办法
现象说明：<br />使用logrotate轮询nginx日志，配置好之后，发现nginx日志连续两天没被切割，这是为什么呢？？<br />然后开始检查日志切割的配置文件是否有问题，检查后确定配置文件一切正常。<br />于是怀疑是logrotate预定的cron没执行，查看了cron的日志，发现有一条Dec 7 04:02:01 www crond[18959]: (root) CMD (run-parts /etc/cron.daily)这样的日志，证明cron在04:02分时已经执行/etc/cron.daily目录下的程序。<br />接着查看/etc /cron.daily/logrotate（这是logrotate自动轮转的脚本）的内容：
```bash
[root@fcant ~]# cat /etc/cron.daily/logrotate
#!/bin/sh
/usr/sbin/logrotate /etc/logrotate.conf >/dev/null 2>&1
EXITVALUE=$?
if [ $EXITVALUE != 0 ]; then
    /usr/bin/logger -t logrotate "ALERT exited abnormally with [$EXITVALUE]"
fi
exit 0
```
没有发现异常，配置好的日志轮转操作都是由这个脚本完成的，一切运行正常，脚本应该就没问题。<br />直接执行命令：
```bash
[root@fcant ~]# /usr/sbin/logrotate /etc/logrotate.conf
```
这些系统日志是正常轮询了，但nginx日志却还是没轮询。<br />接着强行启动记录文件维护操作，纵使logrotate指令认为没有需要，应该有可能是logroate认为nginx日志太小，不进行轮询。<br />故需要强制轮询，即在/etc/cron.daily/logrotate脚本中将 -t 参数替换成 -f 参数
```bash
[root@fcant ~]# cat /etc/cron.daily/logrotate
#!/bin/sh
/usr/sbin/logrotate /etc/logrotate.conf >/dev/null 2>&1
EXITVALUE=$?
if [ $EXITVALUE != 0 ]; then
    /usr/bin/logger -f logrotate "ALERT exited abnormally with [$EXITVALUE]"
fi
exit
```
最后最后重启下cron服务：
```bash
[root@fcant ~]# /etc/init.d/crond restart
Stopping crond: [ OK ]
Starting crond: [ OK ]
```

---

logrotate默认自动切割生效时间
```bash
Logrotate是基于CRON来运行的，其脚本是/etc/cron.daily/logrotate，实际运行时，Logrotate会调用配置文件/etc/logrotate.conf。
[root@fcant ~]# cat /etc/cron.daily/logrotate
#!/bin/sh
/usr/sbin/logrotate /etc/logrotate.conf
EXITVALUE=$?
if [ $EXITVALUE != 0 ]; then
    /usr/bin/logger -t logrotate "ALERT exited abnormally with [$EXITVALUE]"
fi
exit 0
Logrotate是基于CRON运行的，所以这个时间是由CRON控制的，具体可以查询CRON的配置文件/etc/anacrontab（老版本的文件是/etc/crontab）
[root@fcant ~]# cat /etc/anacrontab
# /etc/anacrontab: configuration file for anacron
# See anacron(8) and anacrontab(5) for details.
SHELL=/bin/sh
PATH=/sbin:/bin:/usr/sbin:/usr/bin
MAILTO=root
# the maximal random delay added to the base delay of the jobs
RANDOM_DELAY=45                                                                  //这个是随机的延迟时间，表示最大45分钟
# the jobs will be started during the following hours only
START_HOURS_RANGE=3-22                                                           //这个是开始时间
#period in days   delay in minutes   job-identifier   command
1 5 cron.daily    nice run-parts /etc/cron.daily
7 25  cron.weekly   nice run-parts /etc/cron.weekly
@monthly 45 cron.monthly    nice run-parts /etc/cron.monthly
第一个是Recurrence period
第二个是延迟时间
所以cron.daily会在3:22+(5,45)这个时间段执行，/etc/cron.daily是个文件夹
通过默认/etc/anacrontab文件配置，会发现logrotate自动切割日志文件的默认时间是凌晨3点多。
==================================================================================================
现在需要将切割时间调整到每天的晚上12点，即每天切割的日志是前一天的0-24点之间的内容。
操作如下：
[root@fcant ~]# mv /etc/anacrontab /etc/anacrontab.bak          //取消日志自动轮转的设置
[root@fcant logrotate.d]# cat nstc_nohup.out
/data/nstc/nohup.out {
rotate 30
dateext
daily
copytruncate
compress
notifempty
missingok
}
[root@fcant logrotate.d]# cat syslog
/var/log/cron
/var/log/maillog
/var/log/messages
/var/log/secure
/var/log/history
{
    sharedscripts
    compress
    rotate 30
    daily
    dateext
    postrotate
    /bin/kill -HUP `cat /var/run/syslogd.pid 2> /dev/null` 2> /dev/null || true
    endscript
}
结合crontab进行自定义的定时轮转操作
[root@fcant ~]# crontab -l
#log logrotate
59 23 * * * /usr/sbin/logrotate -f /etc/logrotate.d/syslog >/dev/null 2>&1
59 23 * * * /usr/sbin/logrotate -f /etc/logrotate.d/nstc_nohup.out >/dev/null 2>&1
[root@fcant ~]# ll /data/nstc/nohup.out*
-rw------- 1 app app 33218 1月  25 09:43 /data/nstc/nohup.out
-rw------- 1 app app 67678 1月  25 23:59 /data/nstc/nohup.out-20180125.gz
```
除了利用自带的Logrotate工具实现日志切割之外，还可以编写python脚本或shell脚本以实现日志切割。下面就简单列出几个实例说明下：
<a name="a0LjE"></a>
## 4、通过Python以及Shell脚本实现日志切割
<a name="yAN9a"></a>
### 一、Python脚本实现日志切割
```bash
实例1：对jumpserver日志进行切割
[root@fcant mnt]# cat log_rotate.py
#!/usr/bin/env python
import datetime,os,sys,shutil
log_path = '/opt/jumpserver/logs/'
log_file = 'jumpserver.log'
yesterday = (datetime.datetime.now() - datetime.timedelta(days = 1))
try:
    os.makedirs(log_path + yesterday.strftime('%Y') + os.sep + \
                yesterday.strftime('%m'))
except OSError,e:
    print
    print e
    sys.exit()
shutil.move(log_path + log_file,log_path \
            + yesterday.strftime('%Y') + os.sep \
            + yesterday.strftime('%m') + os.sep \
            + log_file + '_' + yesterday.strftime('%Y%m%d') + '.log')
os.popen("sudo /opt/jumpserver/service.sh restart")
手动执行这个脚本：
[root@fcant mnt]# chmod 755 log_rotate.py
[root@fcant mnt]# python log_rotate.py
查看日志切割后的效果：
[root@fcant mnt]# ls /opt/jumpserver/logs/
2017  jumpserver.log 
[root@fcant mnt]# ls /opt/jumpserver/logs/2017/
09
[root@fcant mnt]# ls /opt/jumpserver/logs/2017/09/
jumpserver.log_20170916.log
然后做每日的定时切割任务：
[root@fcant mnt]# crontab -e
30 1 * * * /usr/bin/python /mnt/log_rotate.py > /dev/null 2>&1
--------------------------------------------------------------------------------------
实例2:对nginx日志进行切割
[root@fcant mnt]# vim log_rotate.py
#!/usr/bin/env python
import datetime,os,sys,shutil
log_path = '/app/nginx/logs/'
log_file = 'www_access.log'
yesterday = (datetime.datetime.now() - datetime.timedelta(days = 1))
try:
    os.makedirs(log_path + yesterday.strftime('%Y') + os.sep + \
                yesterday.strftime('%m'))
except OSError,e:
    print
    print e
    sys.exit()
shutil.move(log_path + log_file,log_path \
            + yesterday.strftime('%Y') + os.sep \
            + yesterday.strftime('%m') + os.sep \
            + log_file + '_' + yesterday.strftime('%Y%m%d') + '.log')
os.popen("sudo kill -USR1 `cat /app/nginx/logs/nginx.pid`")
--------------------------------------------------------------------------------------
其他业务日志的切割脚本跟上面做法相同
```
<a name="6MwXC"></a>
### 二、shell脚本实现日志切割
```bash
[root@fcant ~]# cat /app/script/log_rotate.sh
#!/bin/sh
function rotate() {
logs_path=$1
echo Rotating Log: $1
cp ${logs_path} ${logs_path}.$(date -d "yesterday" +"%Y%m%d")
> ${logs_path}
    rm -f ${logs_path}.$(date -d "7 days ago" +"%Y%m%d")
}
for i in $*
do
        rotate $i
done
--------------------------------------------------------------------------------------------------------------
每天定时切割日志的任务制定（比如对python的一个业务/data/log/xcspam/下的日志进行切割，0K的日志不进行切割）：
[root@fcant ~]# crontab -e
#xcspam 日志切割
30 0 * * * find /data/log/xcspam/ -size +0 -name '*.log' | xargs /app/script/log_rotate.sh
手动执行切割：
[root@fcant ~]# find /data/log/xcspam/ -size +0 -name '*.log' | xargs /app/script/log_rotate.sh
切割后的日志效果：
[root@fcant ~]# ls /data/log/xcspam/
xcspam_error.log  xcspam_error.log-20170926
--------------------------------------------------------------------------------------------------------------
比如对maridb日志进行切割
[root@fcant ~]# crontab -e
#xcspam 日志切割
30 0 * * * find /var/log/mariadb/ -size +0 -name '*.log' | xargs /app/script/log_rotate.sh
[root@fcant ~]# find /var/log/mariadb/ -size +0 -name '*.log' | xargs /app/script/log_rotate.sh
[root@fcant ~]# ll /var/log/mariadb/
总用量 8
-rw-r-----. 1 mysql mysql    0 9月  17 20:31 mariadb.log
-rw-r-----. 1 root  root  4532 9月  17 20:31 mariadb.log.20170916
--------------------------------------------------------------------------------------------------------------
```

```bash
日志压缩脚本：
[root@fcant ~]# ls /var/log/fss/nginx/
nginx.20190506.log  nginx.20190507.log  nginx.20190508.log
[root@fcant ~]# cat /root/log_clean.sh
#!/usr/bin/sh
#根据系统/服务/日志保留天数三个参数压缩日志
#usage: sh clearlog.sh sysname appname keepdays
sysName=$1
appName=$2
keepDay=$3
logDir=/var/log/${sysName}/${appName}
logFile=${appName}.*[0-9][0-9].log
cd ${logDir}
find ./ -name "${logFile}" -mtime -${keepDay} -exec gzip {} \;
[root@fcant ~]# sh /root/log_clean.sh fss nginx 3
[root@fcant ~]# ls /var/log/fss/nginx/
nginx.20190506.log.gz  nginx.20190507.log.gz  nginx.20190508.log.gz
还可以针对日志保留策略，调整成日志清理脚本。
```
推荐用的Nginx日志轮转方法   [部署在nginx的日志目录下]
```bash
#!/bin/bash
yesterday=`date -d "-1 days" +'%Y%m%d'`
cd `dirname $0`
basedir=`pwd`
logdir="${basedir}/bak"
bindir="${basedir%/*}/sbin"
mkdir -p ${logdir}
for log in `ls *.log 2>/dev/null`
do
    mv ${log} ${logdir}/${log}.${yesterday}.bak
    # gzip ${logdir}/${log}.${yesterday}
done
${bindir}/nginx -s reload
cd ${logdir}
find . -type f -name "*.bak" -mtime +7 | xargs rm -f
```

