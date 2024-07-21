Linux<br />实际工作中，crontab出现的问题是多种多样的，下面就深入介绍下crontab在具体工作中容易出现的问题和解决问题的办法。
<a name="DlxMD"></a>
## 一、crontab能干啥
crond是linux下用来周期性的执行某种任务或等待处理某些事件的一个守护进程，与windows下的计划任务类似，当安装完成操作系统后，默认会安装此服务工具，并且会自动启动crond进程，crond进程每分钟会定期检查是否有要执行的任务，如果有要执行的任务，则自动执行该任务。<br />Linux下的任务调度分为两类，系统任务调度和用户任务调度。

- 系统任务调度：系统周期性所要执行的工作，比如写缓存数据到硬盘、日志清理等。
- 用户任务调度：用户定期要执行的工作，比如用户数据备份、定时邮件提醒等。用户可以使用 crontab 工具来定制自己的计划任务。所有用户定义的crontab 文件都被保存在 /var/spool/cron目录中。其文件名与用户名一致。

关于crontab的用途，在企业实际应用中非常广泛，常见的有定时数据备份、定时系统检测、定时数据收集、定时更新配置、定时生成报表等等。
<a name="oSWQx"></a>
## 二、crontab应用实例
<a name="sdEHI"></a>
### 1、crontab使用格式
crontab常用的使用格式有如下两种：
```
crontab [-u user] [file]
crontab [-u user] [-e|-l|-r |-i]
```
选项含义如下：

- -u user：用来设定某个用户的crontab服务，例如，“-u ixdba”表示设定ixdba用户的crontab服务，此参数一般有root用户来运行。
- file：file是命令文件的名字，表示将file做为crontab的任务列表文件并载入crontab。如果在命令行中没有指定这个文件，crontab命令将接受标准输入（键盘）上键入的命令，并将它们载入crontab。
- -e：编辑某个用户的crontab文件内容。如果不指定用户，则表示编辑当前用户的crontab文件。
- -l：显示某个用户的crontab文件内容，如果不指定用户，则表示显示当前用户的crontab文件内容。
- -r：从/var/spool/cron目录中删除某个用户的crontab文件，如果不指定用户，则默认删除当前用户的crontab文件。
- -i：在删除用户的crontab文件时给确认提示。
<a name="TavM3"></a>
### 2、crontab文件语法
用户所建立的crontab文件中，每一行都代表一项任务，每行的每个字段代表一项设置，它的格式共分为六个字段，前五段是时间设定段，第六段是要执行的命令段，格式如下：
```bash
minute hour day month week command
```
其中：

- minute：表示分钟，可以是从0到59之间的任何整数。
- hour：表示小时，可以是从0到23之间的任何整数。
- day：表示日期，可以是从1到31之间的任何整数。
- month：表示月份，可以是从1到12之间的任何整数。
- week：表示星期几，可以是从0到7之间的任何整数，这里的0或7代表星期日。
- command：要执行的命令，可以是系统命令，也可以是自己编写的脚本文件。

在以上各个字段中，还可以使用以下特殊字符：

- 星号（*）：代表所有可能的值，例如month字段如果是星号，则表示在满足其它字段的制约条件后每月都执行该命令操作。
- 逗号（,）：可以用逗号隔开的值指定一个列表范围，例如，“1,2,5,7,8,9”
- 中杠（-）：可以用整数之间的中杠表示一个整数范围，例如“2-6”表示“2,3,4,5,6”
- 正斜线（/）：可以用正斜线指定时间的间隔频率，例如“0-23/2”表示每两小时执行一次。同时正斜线可以和星号一起使用，例如/10，如果用在minute字段，表示每十分钟执行一次。
<a name="gK6iA"></a>
### 3、几个crontab例子
```bash
0 /3 /usr/local/apache2/apachectl restart
```
表示每隔3个小时重启apache服务一次。
```bash
30 3 6 /webdata/bin/backup.sh
```
表示每周六的3点30分执行/webdata/bin/backup.sh脚本的操作。
```bash
0 0 1,20 fsck /dev/sdb8
```
表示每个月的1号和20号检查/dev/sdb8磁盘设备。
```bash
10 5 /5 * echo "">/usr/local/apache2/log/access_log
```
表示每个月的5号、10号、15号、20号、25号、30号的5点10分执行清理apache日志操作。
<a name="aX2zB"></a>
## 三、系统级任务调度/etc/crontab
在/etc目录下有一个crontab文件，这个就是系统任务调度的配置文件。<br />/etc/crontab文件包括下面几行：
```
SHELL=/bin/bash
PATH=/sbin:/bin:/usr/sbin:/usr/bin
MAILTO=root
HOME=/
# run-parts
01 * * * * root run-parts /etc/cron.hourly
02 4 * * * root run-parts /etc/cron.daily
22 4 * * 0 root run-parts /etc/cron.weekly
42 4 1 * * root run-parts /etc/cron.monthly
```
从上面的示例文件可看出，crontab的任务列表主要由两部分组成：环境变量配置与定时任务配置。可能大家在工作中更多是只用到了任务配置部分。<br />前四行是用来配置crond任务运行的环境变量，第一行SHELL变量指定了系统要使用哪个shell，这里是bash，第二行PATH变量指定了系统执行命令的路径，第三行MAILTO变量指定了crond的任务执行信息将通过电子邮件发送给root用户，如果MAILTO变量的值为空，则表示不发送任务执行信息给用户，第四行的HOME变量指定了在执行命令或者脚本时使用的主目录。第六至九行就是crontab执行格式的具体写法。
<a name="avFBL"></a>
## 四、crontab调试解析神器
通常在使用crontab添加任务时，会依靠自己已有知识编写定时语句。当需要测试语句是否正确时，还需要在服务器上不断调试，，这种方式太不高效了。有没有一款工具，只要给出语句，就能告诉具体执行时间以及对错呢？还真有，下面介绍一款老外开发的crontab在线解析工具。<br />工具地址：[https://crontab.guru](https://crontab.guru)<br />给出这个工具的截图如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675819697369-38831c13-9228-4ab8-b23c-b2bb302cd1ea.png#averageHue=%23181817&clientId=u7fd36616-650f-4&from=paste&id=u2727f15a&originHeight=360&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6f79d484-3972-4470-bf05-34a5e96637b&title=)<br />好用不好用，试试就知道。
<a name="f7sDG"></a>
## 五、crontab使用的各种坑
<a name="LvsYQ"></a>
### 1、环境变量问题
刚使用crontab时，运维老鸟们一般会告知所有命令尽量都使用绝对路径，以防错误。这是为什么？这就和下面要谈的环境变量有关了。<br />首先，获取shell终端环境变量，内容如下：
```bash
[root@SparkWorker1 dylogs]# env
XDG_SESSION_ID=1629
HOSTNAME=SparkWorker1
TERM=linux
SHELL=/bin/bash
HISTSIZE=1000
SSH_CLIENT=172.16.213.132 50080 22
HADOOP_PREFIX=/opt/hadoop/current
CATALINA_BASE=/opt/hadoop/current/share/hadoop/httpfs/tomcat
SSH_TTY=/dev/pts/1
QT_GRAPHICSSYSTEM_CHECKED=1
USER=root
MAIL=/var/spool/mail/root
PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/usr/java/default/bin:/opt/hadoop/current/bin:/opt/hadoop/current/sbin:/root/bin
PWD=/data/dylogs
LANG=zh_CN.UTF-8
HOME=/root
```
要获取crontab环境变量信息，可以设置如下计划任务：
```bash
* * * * * /usr/bin/env > /tmp/env.txt
```
等待片刻，env.txt输出内容如下：
```bash
[root@SparkWorker1 dylogs]# cat /tmp/env.txt
XDG_SESSION_ID=1729
SHELL=/bin/sh
USER=root
PATH=/usr/bin:/bin
PWD=/root
LANG=zh_CN.UTF-8
SHLVL=1
HOME=/root
LOGNAME=root
XDG_RUNTIME_DIR=/run/user/0
_=/usr/bin/env
```
从上面输出结果可知，shell命令行的PATH值为
```
PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/usr/java/default/bin:/opt/hadoop/current/bin:/opt/hadoop/current/sbin:/root/bin
```
而crontab中的PATH值为：
```
PATH=/usr/bin:/bin
```
对比crontab环境变量与shell终端环境变量的输出，可以发现两者的差异很大。大家可能遇到过，在shell命令行执行脚本都没有问题，而放到crontab后却执行异常，或者执行失败，此时，就需要考虑是否命令涉及的环境变量在crontab和shell命令行间存在差异。<br />例如，在crontab中执行了如下定时任务：
```
20 16 * * * php autosave.php
```
而如果php是安装在/usr/local/bin/目录下的话，那么上面这个定时任务由于无法找到php命令，会运行失败。<br />那么，知道了环境变量问题，可能导致计划任务无法正常执行，怎么才能避免这个问题呢，这个交给大家一个终极大招，可以在crontab中加入如下配置，保证计划任务执行不会出现环境变量问题：
```
* * * * * source /$HOME/.bash_profile && command
```
这个其实是在执行计划任务命令之前，先加载了用户环境变量信息，由此可保证所有环境变量都可正常加载。
<a name="JcSKB"></a>
### 2、定时时间配置误区
时间是crontab的核心，稍微配置不当，就会出现问题，先看在整点时间设置时可能出现的错误，例如，设定每天2点执行一次任务，很多朋友可能这么写过：
```
* 2 * * * command
```
很明显，这个时间写法是错误的，当听到每天2点执行一次某任务时，很多人会把重点放在2点，而忽略了执行一次的需求。上面这个定时任务他会在2点开始执行，每分钟执行一次，总共执行60次。<br />正确的写法应该是这样的：
```
0 2 * * * command
```
这个才表示每天2点0分执行command对应的任务。
<a name="HAasi"></a>
### 3、特殊符号%问题
%在crontab中是特殊符号，具体含义如下：<br />第一个%表示标准输入的开始，其余%表示换行符，看下面两个例子：
```
* * * * * cat >> /tmp/cat.txt 2>&1 % stdin out
```
查看/tmp/cat.txt的内容为：
```
stdin out
```
再看下面这个例子：
```
* * * * * cat >> /tmp/cat1.txt 2>&1 % stdin out 1 % stdin out 2 % stdin out 3
```
查看 /tmp/cat1.txt的内容如下：
```
stdin out 1
stdin out 2
stdin out 3
```
有输出内容可知，第一个%表示标准输入的开始，其余%表示换行符。<br />既然"%"是特殊字符，那么在crontab中使用时，就要特别注意，怎么使用这些特殊字符呢，很明显，使用转移字符即可，例如：
```
* * * * * cat >> /tmp/cat2.txt 2>&1 % Special character escape \%.
```
查看输出/tmp/cat2.txt 输出内容如下：
```
Special character escape %.
```
可以看到，执行成功了，并成功避开这个坑了。
<a name="LQAek"></a>
### 4、关于crontab的输出重定向
在crontab执行的计划任务中，有些任务如果不做输出重定向，那么原本会输出到屏幕的信息，会以邮件的形式输出到某个文件中，例如，执行下面这个计划任务：
```
* * * * * /bin/date
```
这个计划任务是没有做输出重定向的，他的主要用途是输出时间，由于没有配置输出重定向，那么这个时间信息默认将以邮件的形式输出到/var/spool/mail/$USER（这个$USER对应的是系统用户，这里是root用户）文件中，大致内容如下：
```bash
From root@SparkWorker1.localdomain Fri Sep 21 12:58:02 2022
Return-Path: <root@SparkWorker1.localdomain>
X-Original-To: root
Delivered-To: root@SparkWorker1.localdomain
Received: by SparkWorker1.localdomain (Postfix, from userid 0)
id F2745192AE; Fri, 21 Sep 2022 12:58:01 +0800 (CST)
From: "(Cron Daemon)" <root@SparkWorker1.localdomain>
To: root@SparkWorker1.localdomain
Subject: Cron <root@SparkWorker1> /bin/date
Content-Type: text/plain; charset=UTF-8
Auto-Submitted: auto-generated
Precedence: bulk
X-Cron-Env: <XDG_SESSION_ID=1820>
X-Cron-Env: <XDG_RUNTIME_DIR=/run/user/0>
X-Cron-Env: <LANG=zh_CN.UTF-8>
X-Cron-Env: <SHELL=/bin/sh>
X-Cron-Env: <HOME=/root>
X-Cron-Env: <PATH=/usr/bin:/bin>
X-Cron-Env: <LOGNAME=root>
X-Cron-Env: <USER=root>
Message-Id: <20220921045801.F2745192AE@SparkWorker1.localdomain>
Date: Fri, 21 Sep 2022 12:58:01 +0800 (CST)
2022年 09月 21日 星期五 12:58:01 CST
```
由此可见，输出内容还是很多的，如遇到任务有大量输出的话，会占用大量磁盘空间，显然，这个邮件输出最好关闭，怎么关闭呢，只需设置MAILTO环境变量为空即可，上面的计划任务，可做如下修改：
```
MAILTO=""
* * * * * /bin/date
```
这样，就不会发邮件信息到/var/spool/mail/$USER下了，但是问题并没有彻底解决，关闭mail功能后，输出内容将继续写入到/var/spool/clientmqueue中，长期下去，可能占满分区的inode资源，导致任务无法执行。<br />为了避免此类问题发生，建议任务都加上输出重定向，例如，可以在crontab文件中设置如下形式，忽略日志输出：
```bash
0 */3 * * * /usr/local/apache2/apachectl restart >/dev/null 2>&1
```
其中，“/dev/null 2>&1”表示先将标准输出重定向到/dev/null，然后将标准错误重定向到标准输出，由于标准输出已经重定向到了/dev/null，因此标准错误也会重定向到/dev/null，这样日志输出问题就解决了。
<a name="bhy7x"></a>
### 5、调试crontab问题的一般思路
要解决crontab相关异常问题，可按照如下思路进行调试：<br />（1）通过/var/log/cron日志确认任务是否执行<br />（2）如未执行则分析定时语句，是否是环境变量问题、特殊字符问题、时间配置问题、权限问题等。<br />（3）确认crond服务开启，如果定时语句也正确，检查crond服务是否开启。<br />Systemd方式(centos7及以上)
```bash
[root@SparkWorker1 spool]# systemctl status crond.service
```
SysVinit方式(centos7以下)
```bash
[root@SparkWorker1 spool]# service crond status
```
（4）确认定时任务中命令是否执行成功<br />这个问题可通过输出获取错误信息进行调试，方法就是利用重定向获取输出，然后进行分析。举例如下：
```bash
* * * * * python /usr/local/dyserver/dypos.py >> /tmp/dypos.log 2>&1
```
通过加上“/tmp/dypos.log 2>&1”，就可以很快定位问题，因为这个dypos.py脚本在执行的时候会把错误信息都输出到dypos.log 中，接着查看dypos.log文件，问题一目了然：
```
[root@SparkWorker1 spool]# cat /tmp/dypos.log
/bin/sh: python: 未找到命令
/bin/sh: python: 未找到命令
```
显示Python命令没有找到，很明显的就可以确定是环境变量的问题。这种方式定位问题非常有效。
