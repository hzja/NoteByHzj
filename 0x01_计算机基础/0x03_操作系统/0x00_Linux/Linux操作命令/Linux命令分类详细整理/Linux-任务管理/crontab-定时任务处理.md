Linux crontab<br />crontab命令用于设置周期性被执行的指令。该命令从标准输入设备读取指令，并将其存放于“crontab”文件中，以供之后读取和执行。<br />可以使用crontab定时处理离线任务，比如每天凌晨2点更新数据等，经常用于系统任务调度。<br />cron是linux的一个定时任务执行工具，而crondtab是周期性执行命令，crond则是定时任务的守护进程。
<a name="JLgfH"></a>
## crontab服务管理
<a name="Gv1kC"></a>
### 查看服务是否启动
```bash
➜  /etc systemctl status crond
● crond.service - Command Scheduler
   Loaded: loaded (/usr/lib/systemd/system/crond.service; enabled; vendor preset: enabled)
   Active: active (running) since Wed 2019-08-14 19:32:11 CST; 10 months 28 days ago
 Main PID: 456 (crond)
   CGroup: /system.slice/crond.service
           └─456 /usr/sbin/crond -n

Jul 12 21:20:01 fcant-hadoop001 crond[456]: /usr/sbin/sendmail: error while loading shared libraries: libmysqlc...ctory
Jul 12 21:30:01 fcant-hadoop001 crond[456]: /usr/sbin/sendmail: error while loading shared libraries: libmysqlc...ctory
Jul 12 21:40:01 fcant-hadoop001 crond[456]: /usr/sbin/sendmail: error while loading shared libraries: libmysqlc...ctory
Jul 12 21:50:01 fcant-hadoop001 crond[456]: /usr/sbin/sendmail: error while loading shared libraries: libmysqlc...ctory
Jul 12 22:00:01 fcant-hadoop001 crond[456]: /usr/sbin/sendmail: error while loading shared libraries: libmysqlc...ctory
Jul 12 22:10:01 fcant-hadoop001 crond[456]: /usr/sbin/sendmail: error while loading shared libraries: libmysqlc...ctory
Jul 12 22:20:01 fcant-hadoop001 crond[456]: /usr/sbin/sendmail: error while loading shared libraries: libmysqlc...ctory
Jul 12 22:30:01 fcant-hadoop001 crond[456]: /usr/sbin/sendmail: error while loading shared libraries: libmysqlc...ctory
Jul 12 22:40:01 fcant-hadoop001 crond[456]: /usr/sbin/sendmail: error while loading shared libraries: libmysqlc...ctory
Jul 12 22:50:01 fcant-hadoop001 crond[456]: /usr/sbin/sendmail: error while loading shared libraries: libmysqlc...ctory
Hint: Some lines were ellipsized, use -l to show in full.
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594565526786-58ef7e36-e238-4b44-a564-7ce26b7281be.png#averageHue=%233f3f3f&height=366&id=fuVwQ&originHeight=1098&originWidth=3299&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1573018&status=done&style=none&title=&width=1099.6666666666667)
```bash
如果没有启动,执行
systemctl start crond
systemctl enable crond
service crond start     # 启动服务
service crond stop      # 关闭服务
service crond restart   # 重启服务
service crond reload    # 重新载入配置
service crond status    # 查看crontab服务状态

# 可以使用下面的命令加入开机启动
chkconfig --level 345 crond on
```
<a name="y9Nxi"></a>
## crontab命令
语法 :  crontab  参数<br />常用参数  :
```
crontab -u   #指定用户的cron信息
crontab -l   #列出当前用户下的cron服务的信息
crontab -u user -l   #列出指定用户的cron服务的信息
crontab -r   #删除cron服务
crontab -e   #编辑cron服务
crontab -r -u user   #删除指定用户的定时任务
```
操作:
<a name="nFRrD"></a>
### 编辑cron服务
```bash
➜  /etc crontab -e
crontab: installing new crontab
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594565942244-98907e3c-60a1-4255-a379-4c28009795d9.png#averageHue=%233c3c3c&height=58&id=jJYDu&originHeight=175&originWidth=926&originalType=binary&ratio=1&rotation=0&showTitle=false&size=63673&status=done&style=none&title=&width=308.6666666666667)
<a name="0ImAk"></a>
### 查看当前用户下任务内容
```bash
➜  /etc crontab -l
22 22 22 * * mkdir fc22 22 22 * * mkdir fc22 22 22 * * mkdir fc22 22 22 * * mkdir fc22 22 22 * * mkdir fc22 22 22 * * mkdir fc22 22 22 * * mkdir fc22 22 22 * * mkdir fc22 22 22 * * mkdir fc22 22 22 * * mkdir fc22 22 22 * * mkdir fc22 22 22 * * mkdir fc22 22 22 * * mkdir fc22 22 22 * * mkdir fc22 22 22 * * mkdir fc22 22 22 * * mkdir fc22 22 22 * * mkdir fc22 22 22 * * mkdir fc22 22 22 * * mkdir fc22 22 22 * * mkdir fc22 22 22 * * mkdir fc22 22 22 * * mkdir fc
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594566024943-8315f6d8-665f-4053-8318-4e83ec8ec044.png#averageHue=%233e3e3e&height=113&id=exIyX&originHeight=340&originWidth=3299&originalType=binary&ratio=1&rotation=0&showTitle=false&size=469452&status=done&style=none&title=&width=1099.6666666666667)
<a name="eXeOB"></a>
### 删除任务
```bash
➜  /etc crontab -r
➜  /etc crontab -l
no crontab for root
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594566076423-9b8b7a3d-5bac-4f2c-8d26-19d3ee6a43e0.png#averageHue=%233b3b3b&height=79&id=THneQ&originHeight=237&originWidth=616&originalType=binary&ratio=1&rotation=0&showTitle=false&size=71885&status=done&style=none&title=&width=205.33333333333334)
<a name="CZ5hi"></a>
### 查看某个用户下的任务内容
```bash
➜  /etc crontab -u root -l
no crontab for root
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594566164250-7f4cf4bf-94ec-4cb6-ae6e-b586b81cd7c9.png#averageHue=%233b3b3b&height=61&id=aqGsT&originHeight=182&originWidth=783&originalType=binary&ratio=1&rotation=0&showTitle=false&size=60686&status=done&style=none&title=&width=261)<br />删除某个用户下的定时任务
```bash
➜  /etc crontab -u root -r
no crontab for root
➜  /etc crontab -u root -l
no crontab for root
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594566211271-cacde8f3-8cba-4bc2-b7ef-f330756a5180.png#averageHue=%233d3d3d&height=97&id=T9MYe&originHeight=292&originWidth=852&originalType=binary&ratio=1&rotation=0&showTitle=false&size=104291&status=done&style=none&title=&width=284)
<a name="ANT6d"></a>
## 系统任务
系统的任务调度配置文件在/etc/crontab下
```bash
➜  /etc ll /etc/crontab
-rw-r--r-- 1 root root 0 Sep 17  2019 /etc/crontab
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594566338392-ed66fd32-af5d-4a35-8723-28f4f0f0bce2.png#averageHue=%23373737&height=62&id=R0SUn&originHeight=185&originWidth=1447&originalType=binary&ratio=1&rotation=0&showTitle=false&size=121349&status=done&style=none&title=&width=482.3333333333333)<br />可以直接在/etc/crontab下添加系统任务(不建议)
```bash
➜ cat /etc/crontab
SHELL=/bin/bash
PATH=/sbin:/bin:/usr/sbin:/usr/bin
MAILTO=root

# For details see man 4 crontabs

# Example of job definition:
# .---------------- minute (0 - 59)
# |  .------------- hour (0 - 23)
# |  |  .---------- day of month (1 - 31)
# |  |  |  .------- month (1 - 12) OR jan,feb,mar,apr ...
# |  |  |  |  .---- day of week (0 - 6) (Sunday=0 or 7) OR sun,mon,tue,wed,thu,fri,sat
# |  |  |  |  |
# *  *  *  *  * user-name  command to be executed
```
<a name="CmPBe"></a>
### 定时任务相关的目录
```bash
➜ ll cr
cron.d/       
cron.deny/
cron.monthly/ 
cron.weekly/
cron.daily/   
cron.hourly/  
crontab       
crypttab
```

- `cron.daily`是每天执行一次的job
- `cron.weekly`是每个星期执行一次的job
- `cron.monthly`是每月执行一次的job
- `cron.hourly`是每个小时执行一次的job
- `cron.d`是系统自动定期需要做的任务
- `crontab`是设定定时任务执行文件
- `cron.deny`文件就是用于控制不让哪些用户使用Crontab的功能

每个用户都有自己的`crontab`配置文件，使用`crontab -e`命令进行编辑。保存后系统会自动存放与`/var/spool/cron/`目录中，文件以用户名命名。<br />Linux的`crontab`服务每隔一分钟去读取一次`/var/spool/cron`，`/etc/crontab`，`/etc/cron.d`下面所有的内容。
<a name="mJj6v"></a>
### 定时任务规则
首先来看下任务的格式(如图):<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594566630785-d8f50e00-72ed-4643-84b9-dedf46d31371.png#averageHue=%23c7d1d1&height=89&id=QjAuy&originHeight=266&originWidth=1594&originalType=binary&ratio=1&rotation=0&showTitle=false&size=235158&status=done&style=none&title=&width=531.3333333333334)<br />时间的一些特殊符号:<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594566664721-a3dd5f74-d45f-4564-b438-d348fc91029a.png#averageHue=%2396bad6&height=163&id=AYUmU&originHeight=489&originWidth=1594&originalType=binary&ratio=1&rotation=0&showTitle=false&size=94465&status=done&style=none&title=&width=531.3333333333334)
<a name="D6wde"></a>
### 使用案例
```bash
#每晚的20:30重加载nginx
30 20 * * * /usr/local/nginx/sbin/nginx -s reload

#每月1、10、15日的5:30重加载nginx
30 5 1,10,15 * * /usr/local/nginx/sbin/nginx  -s reload

#每天20: 00至22 : 00之间每隔30分钟重加载nginx
0,30 20-22 * * * /usr/local/nginx/sbin/nginx -s reload

每星期六的10 : 00 pm 重加载nginx
0 22 * * 6 /usr/local/nginx/sbin/nginx -s reload

#每一小时重加载一次nginx
0 */1 * * * /usr/local/nginx/sbin/nginx -s reload
```
<a name="sdnpe"></a>
## 查看定时任务操作日志
```bash
tail -f /var/log/cron
```
<a name="eqdLG"></a>
## crontab创建任务会遇到的问题
不要假定cron知道所需要的特殊环境，它其实并不知道。所以要保证在shelll脚本中提供所有必要的路径和环境变量，除了一些自动设置的全局变量。需要注意以下几点：<br />1）脚本中涉及文件路径时写全局路径；<br />2）脚本执行要用到java或其他环境变量时，通过`source`命令引入环境变量，如：
```bash
cat start.sh

#!/bin/sh
source /etc/profile
export RUN_CONF=/home/d139/conf/platform/cbp/cbp_jboss.conf
/usr/local/jboss-4.0.5/bin/run.sh -c mev &
```
3）当手动执行脚本OK，但是crontab死活不执行时。这时必须大胆怀疑是环境变量的问题，并可以尝试在crontab中直接引入环境变量解决问题。如：
```bash
0 * * * * . /etc/profile;/bin/sh /var/www/java/audit_no_count/bin/restart_audit.sh
```
<a name="GVzBr"></a>
## crontab创建任务时其他应该注意的问题
1）新创建的cron job，不会马上执行，至少要过2分钟才执行。如果重启cron则马上执行。<br />2）每条 JOB 执行完毕之后，系统会自动将输出发送邮件给当前系统用户。日积月累，非常的多，甚至会撑爆整个系统。所以每条 JOB 命令后面进行重定向处理是非常必要的： >/dev/null 2>&1 。前提是对 Job 中的命令需要正常输出已经作了一定的处理, 比如追加到某个特定日志文件。<br />3）当crontab突然失效时，可以尝试/etc/init.d/crond restart解决问题。或者查看日志看某个job有没有执行/报错 `tail -f /var/log/cron` 。<br />4）千万别乱运行 `crontab -r` 。它从Crontab目录（/var/spool/cron）中删除用户的Crontab文件。删除了该用户的所有crontab都没了。<br />5）在crontab中%是有特殊含义的，表示换行的意思。如果要用的话必须进行转义\%，如经常用的date '+%Y%m%d'在crontab里是不会执行的，应该换成date '+\%Y\%m\%d'。
<a name="5DNQF"></a>
## 定制执行任务命令的注意事项
<a name="whozJ"></a>
### 输入输出重定向
在没有配置输出重定向的时候，定时任务会见错误输出到下面的文件：`/var/log/mail/{$user}`，这是很不好的，往往需要把不同的脚本输出到不同的日志文件，方便查看脚本的执行情况，这个时候需要使用输出重定向。
```bash
58 23 * * * sh /home/work/update.sh >> /home/work/log/update.log 2>&1
```
Linux中使用`0-3`作为文件描述符来表示标准流。

| 名称 | 类型 | 文件描述符 | 操作 |
| --- | --- | --- | --- |
| stdin 标准输入 | standard input | 0 | `<`,`<<` |
| stdout 标准输出 | standard output | 1 | `>`,`>>` |
| stderr 标准错误输出 | standard error output | 2 | `2>`,`2>>` |

在上面的定时任务脚本中，`update.sh`后面的`>>`表示将输出追加到`update.log`文件中，`2>&1`表示标准错误输出重定向等同于标准输出。
<a name="zJsZK"></a>
### 忽略输入`nohup`
有时候还会看到在定时任务脚本前面有一个`nohup`的命令，该命令用于指定后面的脚本忽略输入。
```bash
58 23 * * * nohup sh /home/work/update.sh >> /home/work/log/update.log 2>&1
```
<a name="bGNJs"></a>
### 后台执行`&`
无论是在控制台直接执行，还是在定时任务脚本中，在某一条命令的最后面加上`&`符号，表示当前命令在后台运行，不占用控制台。
```bash
58 23 * * * nohup sh /home/work/update.sh >> /home/work/log/update.log 2>&1 &
```
这里需要注意，如果执行的命令有等待输入的交互，把这个命令使用`&`放在后台运行时，它会一直等待输入，但是有没有输入，就卡住不动了。
<a name="zfK2e"></a>
### 时间处理
再回顾上面的更新任务，它是将每一天的运行结果都重定向到`update.log`文件，如果输出比较多，就需要每一天输出的不同的文件，可以通过在输出目录中指定日期用来分割。如下：
```bash
58 23 * * * nohup sh /home/work/update.sh >> /home/work/log/`update_"date +\%Y\%m\%d".log` 2>&1 &
# 或者下面的形式
58 23 * * * nohup sh /home/work/update.sh >> "/home/work/log/update_"date +\%Y\%m\%d".log" 2>&1 &
```
这里使用`date`函数，需要注意的是，crontab里面的脚本命令和直接在命令行运行的脚本可能是不一样的，尤其是`date`函数中的`%`需要转义。<br />可以直接在命令行运行下面命令：
```bash
sh /home/work/update.sh >> /home/work/log/update_`date +"%Y%m%d".log` 2>&1 &
```
达到和定时任务里面相同的效果，都是输出到`update_20201120.log`的文件。<br />但是如果直接在crontab中这样配置，则无法识别，不会执行该定时任务。下面的crontab任务是不会执行的，date函数将会报错。
```bash
58 23 * * * nohup sh /home/work/update.sh >> /home/work/log/`update_"date +%Y%m%d".log` 2>&1 &
```
