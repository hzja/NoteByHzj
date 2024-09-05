Linux<br />实现“自动化”有如下好处：

1. 节省人力，一个脚本就够了。
2. 在夜晚自动执行可以避开网站流量高峰期，不影响网站白天的效率。
3. 准确，设置完善的情况下，不会出差错。
4. 当然最重要的还是省心了，不用频繁的敲某些命令了。
<a name="P1eMg"></a>
## 开机启动
开机启动应该是很经常的需求了，常需要在开机时就自动执行某些命令来开启服务，进程等，有了它就不必再在每次开机时输入同一堆命令。
<a name="Oou1L"></a>
### chkconfig命令
使用chkconfig命令可以在设置在不同启动级别下启动特定的服务或是程序。<br />先说一下linux的运行级别：

- 等级0表示：表示关机
- 等级1表示：单用户模式
- 等级2表示：不带NFS功能的多用户命令行模式
- 等级3表示：有带NFS功能的多用户命令行模式
- 等级4表示：不可用
- 等级5表示：带图形界面的多用户模式
- 等级6表示：重新启动

chkconfig的命令如下：
```bash
chkconfig --list //命令查看已设置的开启自启动列表。
xxxd 0:off 1:off 2:on ... 6:off //list的结果，表示在xxxd服务在启动级别为2 3 4 5 的情况下会自动启动。
chkconfig --add xxxd//向任务列表中添加一个xxxd服务
chkconfig [--level 1/2/../6] xxxd on/off//设置xxxd用服务在n状态为开/关，[]内省略则在2345级别开启
chkconfig --del xxxd //将任务列表中的xxxd服务删除
```
<a name="68202c12"></a>
### rc.d文件的编辑
也可以直接编辑/etc/rc.d/目录下的文件来实现开机自启动。此目录下有很多文件，rcn.d是在启动状态为n的情况下的启动文件夹，rc、rc.sysinit、init.d都是系统的模块或系统设置的自启文件[夹]。<br />用vim rc.local 编辑 rc.local文件，来定制自己的自启计划。命令十分简单，就像平常在操作一样。如/usr/local/apache/bin/apachectl start表示开机自启动apache服务器。
<a name="bZD73"></a>
## at实现定时任务
at是一个简单的功能简单的定时任务程序，它只能进行一次性的定时任务，其用法如下：
```bash
#at time　　　　　　//at加时间启动at命令
at>operation　　　　//输入要执行的操作
at>Ctrl+D　　　　　　//按Ctrl+D退出命令编辑
```
其time的常见形式如下
```bash
at H:m tomorrow     //第二天的H点m分
at now + n minutes/hours/days/weeks  //在n分/时/天/周后
at midnight         //在午夜=-=
at H:m pm/am        //在当天上午/下午的H点m分
```
也可以在/var/spool/at文件中查看at的当前命令。还需要注意的是，linux默认atd进程关闭状态，需要手动打开。
<a name="RLG5y"></a>
## crontab实现定时任务
linux内置的cron进程能实现这些需求，cron搭配shell脚本，非常复杂的指令也没有问题。
<a name="QobfK"></a>
### cron介绍
cron守护进程是一个由实用程序和配置文件组成的小型子系统，在几乎所有类 UNIX 系统上都可以找到某种风格的cron，可以用ps aux|grep cron找到crond这个守护进程。<br />经常使用的是crontab命令是cron table的简写，它是cron的配置文件，也可以叫它作业列表，可以在以下文件夹内找到相关配置文件。

- /var/spool/cron/ 目录下存放的是每个用户包括root的crontab任务，每个任务以创建者的名字命名
- /etc/crontab 这个文件负责调度各种管理和维护任务。<br />/etc/cron.d/ 这个目录用来存放任何要执行的crontab文件或脚本。
- 还可以把脚本放在/etc/con.hourly、/etc/con.daily、/etc/con.weekly、/etc/con.monthly目录中，让它每小时/天/星期、月执行一次。
<a name="K1x8q"></a>
### crontab的使用
常用的命令如下：
```bash
crontab [-u username]　　　　//省略用户表表示操作当前用户的crontab
    -e      (编辑工作表)
    -l      (列出工作表里的命令)
    -r      (删除工作作)
```
用crontab -e进入当前用户的工作表编辑，是常见的vim界面。每行是一条命令。crontab的命令构成为 时间+动作，其时间有分、时、日、月、周五种，操作符有

- `*` 取值范围内的所有数字
- `/` 每过多少个数字
- `-` 从X到Z
- `，` 散列数字

以下是几个例子。
```
时间                  注释
0 0 25 12 *     //在12月25日的0时0分
*/5 * * * *     //每过5分钟
* 4-6 * * *     //每天的4 5 6点
* * * * 2，5     //每周二和周五
```
<a name="0b97T"></a>
### 配合简单的shell脚本
如果命令有逻辑判断等非常复杂的操作时，再直接编辑crontab就有点困难了，这时可以使用shell脚本。其来历，分类定义与题不符，直接说它的用法。用vim /usr/sh/test.sh来使用vim编辑一个shell脚本
```bash
#!/bin/sh           //声明开始shell脚本
a = "hello world"   //定义一个shell变量
echo $a             //熟悉的echo，输出a变量
```
然后`crontab -e`编辑crontab，添加 `*/5 * * * * /usr/sh/test.sh`每隔五分钟运行一次test.sh脚本，也可以用 `/phppath/php /filepath/test.php` 来用php进程来执行php程序。
