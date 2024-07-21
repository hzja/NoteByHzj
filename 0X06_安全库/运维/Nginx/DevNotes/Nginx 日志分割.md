Nginx<br />nginx 默认没有提供对日志文件的分割功能，所以随着时间的增长，access.log 和 error.log 文件会越来越大，尤其是 access.log，其日志记录量比较大，更容易增长文件大小。影响日志写入性能分割 nginx 日志的方法有很多，这里推荐利用 Logrotate 来完成。
<a name="r0tv1"></a>
## Logrotate 用法
<a name="UoHOt"></a>
### 1、安装
logrotate 是一个 Linux系统日志的管理工具。可以对单个日志文件或者某个目录下的文件按时间 / 大小进行切割，压缩操作；指定日志保存数量；还可以在切割之后运行自定义命令。<br />logrotate 是基于 crontab 运行的，所以这个时间点是由 crontab 控制的，具体可以查询 crontab 的配置文件 /etc/anacrontab。系统会按照计划的频率运行 logrotate，通常是每天。在大多数的 Linux 发行版本上，计划每天运行的脚本位于 /etc/cron.daily/logrotate。<br />主流 Linux 发行版上都默认安装有 logrotate 包，如果 Linux 系统中找不到 logrotate, 可以使用 apt-get 或 yum 命令来安装。
```bash
yum install -y logrotate
```
一般 Linux 都已经自带 logrotate，下列命令可以查看是否已安装。
```bash
rpm -ql logrotate
```
![DM_20230201101259_001.PNG](https://cdn.nlark.com/yuque/0/2023/png/396745/1675217796104-51c4339d-6d80-4d12-801d-ad748da89d05.png#averageHue=%2324394b&clientId=u4a9543be-8313-4&from=ui&id=uf08b931c&originHeight=231&originWidth=399&originalType=binary&ratio=1&rotation=0&showTitle=false&size=27641&status=done&style=none&taskId=u7ad108a3-f239-4b32-9513-83e28ad86d0&title=)
<a name="rwvVH"></a>
### 2、基本用法详解
<a name="ZSmrO"></a>
#### 2.1 入门
/etc/logrotate.conf 这个文件是 logrotate 的主配置文件。
```nginx
# see "man logrotate" for details
# rotate log files weekly
weekly

# keep 4 weeks worth of backlogs
rotate 4

# create new (empty) log files after rotating old ones
create

# use date as a suffix of the rotated file
dateext

# uncomment this if you want your log files compressed
#compress

# 包含自定义配置目录
include /etc/logrotate.d

# no packages own wtmp and btmp -- we'll rotate them here
/var/log/wtmp {
    monthly
    create 0664 root utmp
        minsize 1M
    rotate 1
}

/var/log/btmp {
    missingok
    monthly
    create 0600 root utmp
    rotate 1
}

# system-specific logs may be also be configured here.
```
这个主配置文件中定义了日志文件分割的通用参数。并且 include /etc/logrotate.d 表示其会加载 /etc/logrotate.d 的所有自定义配置文件，自定义配置文件中的配置可以覆盖掉通用配置。<br />来到自定义配置文件的目录/etc/logrotate.d。<br />![DM_20230201101259_002.PNG](https://cdn.nlark.com/yuque/0/2023/png/396745/1675217812923-f684f88b-1f4b-427a-8995-2431a74d5e44.png#averageHue=%2324394b&clientId=u4a9543be-8313-4&from=ui&id=u88d5eaa6&originHeight=307&originWidth=499&originalType=binary&ratio=1&rotation=0&showTitle=false&size=56161&status=done&style=none&taskId=u247fb5fe-0b99-4a95-a1ca-662581ade9f&title=)<br />尝试在该目录中创建一个日志分割配置 test，对 /opt/logtest 目录中所有以 .log 结尾的文件进行分割。
```bash
vim test

# test配置文件的内容
/opt/logtest/*.log {
    daily
    rotate 2
    copytruncate
    missingok
}
```
test 配置的第一行指定要对哪个路径的哪些文件进行分割，然后携带的 4 个参数解释如下：

- **「daily：」** 按天切割。触发切割时如果时间不到一天不会执行切割。除了 daily，还可以选 monthly，weekly，yearly；
- **「rotate：」** 对于同一个日志文件切割后最多保留的文件个数；
- **「copytruncate：」** 将源日志文件切割成新文件后，清空并保留源日志文件。默认如果不启用该配置，分割后源日志文件将被删除。设置该值，以便分割后可以继续在源日志文件写入日志，等待下次分割；
- **「missingok：」** 切割中遇到日志错误忽略。

创建好配置以后，系统会在每天利用 cron 定时执行 logrotate 日志分割指令。这里为了看到效果，不等到系统自动执行，可以手动强制执行一次日志分割。强制执行会立即进行一次日志。
```bash
# -v:显示执行日志
# -f:强制执行分割
logrotate -vf /etc/logrotate.d/test
```
执行前，事先在日志所在目录中创建 2 个测试日志文件。
```bash
touch test1.log
touch test2.log
```
手动执行一次日志分割，观察执行日志可以发现，过程如下：

- 先将源日志内容拷贝到分割后的文件
- 清空源文件

![DM_20230201101259_003.PNG](https://cdn.nlark.com/yuque/0/2023/png/396745/1675217813214-860e0bb8-e4ea-4545-b766-6b62f2071446.png#averageHue=%2324384a&clientId=u4a9543be-8313-4&from=ui&id=MMY2b&originHeight=704&originWidth=808&originalType=binary&ratio=1&rotation=0&showTitle=false&size=134963&status=done&style=none&taskId=u83154261-7336-4e48-ad0f-c618dc88e8b&title=)此时分割后的文件名为源文件名后面加上 . 序号，序号从 1 开始。<br />![DM_20230201101259_004.PNG](https://cdn.nlark.com/yuque/0/2023/png/396745/1675217836007-f29d2af6-b7b2-404a-8dc8-accf1fb0fe68.png#averageHue=%2324384a&clientId=u4a9543be-8313-4&from=ui&id=u9bfdc002&originHeight=117&originWidth=434&originalType=binary&ratio=1&rotation=0&showTitle=false&size=7449&status=done&style=none&taskId=u81ba0d44-1c8f-4785-90f6-275b4c5d07a&title=)<br />然后再次手动执行一次分割，此时执行过程如下：

1. 将第一次分割后的日志文件 test1.log.1 重命名为 test1.log.2；
2. 将源日志文件拷贝到此次分割后的文件，命名序号重新从 1 开始，为 test1.log.1；
3. 清空源日志文件。

![DM_20230201101259_005.PNG](https://cdn.nlark.com/yuque/0/2023/png/396745/1675217836057-2e920ea8-381d-4185-ad84-3376ee9b15fd.png#averageHue=%2324384a&clientId=u4a9543be-8313-4&from=ui&id=PtoMe&originHeight=666&originWidth=837&originalType=binary&ratio=1&rotation=0&showTitle=false&size=133290&status=done&style=none&taskId=ua73e2f6e-7f3a-421b-89b6-944c15e3315&title=)<br />![DM_20230201101259_006.PNG](https://cdn.nlark.com/yuque/0/2023/png/396745/1675217851405-da0902b9-75ac-4bdc-8b05-c3a609063842.png#averageHue=%2324384a&clientId=u4a9543be-8313-4&from=ui&id=ub48fa50a&originHeight=149&originWidth=450&originalType=binary&ratio=1&rotation=0&showTitle=false&size=8800&status=done&style=none&taskId=ue84bf8cc-e632-4d0f-bf4c-0067dd7a235&title=)<br />接着再次手动执行一次分割，此时执行过程如下：

1. 将之前分割后的日志文件 test1.log.2 重命名为 test1.log.3，test1.log.1 重命名为 test1.log.2；
2. 分割源日志文件，拷贝其内容到 test1.log.1；
3. 由于设置了 rotate 为 2，即最多保留 2 个日志文件，所以此时要删除最早分割出的那个日志文件，即 test1.log.3。

![DM_20230201101259_007.PNG](https://cdn.nlark.com/yuque/0/2023/png/396745/1675217851447-605b314a-91b3-4415-afdf-8cae14027c91.png#averageHue=%2324384a&clientId=u4a9543be-8313-4&from=ui&id=fFcVC&originHeight=633&originWidth=797&originalType=binary&ratio=1&rotation=0&showTitle=false&size=115356&status=done&style=none&taskId=u96ea188a-13d1-4e47-8c6a-002a590295a&title=)<br />![DM_20230201101259_008.PNG](https://cdn.nlark.com/yuque/0/2023/png/396745/1675217865749-246c45fb-f7df-435f-a8f0-35f034eca958.png#averageHue=%2324384a&clientId=u4a9543be-8313-4&from=ui&id=u0d68f6af&originHeight=146&originWidth=471&originalType=binary&ratio=1&rotation=0&showTitle=false&size=20484&status=done&style=none&taskId=u7854cb8d-7677-4f3d-95f5-d79dd5218bd&title=)<br />总结一下 logrotate 日志分割的步骤：

1. 默认分割后日志的命名为源日志名称 +. 序号（从 1 开始）。分割之前将所有之前分割出的日志文件重命名，序号往后移一位；
2. 执行分割，将源日志文件分割为源日志文件名 .1。这样就保证了所有分割后的文件中，序号最小的是最新分割出的，序号最大的是最早分割出的；
3. 根据 rotate 设置，如果此次分割后文件数量大于 rotate 设置，那么删除序号最大的那个分割文件，也就是最旧的分割日志。

实际开发中可以使用 create 代替 copytruncate，它们的区别如下：

- copytruncate 先将源文件内容拷贝到分割后文件，再清空源文件，拷贝和清空之间有时间差，可能会丢失部分日志。另外拷贝操作在源文件比较大时消耗性能；
- create 直接将源文件重命名为分割后文件，再创建一个与源文件名称相同的新文件，用于后续日志写入。
```
/opt/logtest/*.log {
    daily
    rotate 2
    create  # 创建与源文件名称相同的新文件，用于后续日志写入，新文件的归属用户、权限与源文件相同
    missingok
}
```
![DM_20230201101259_009.PNG](https://cdn.nlark.com/yuque/0/2023/png/396745/1675217865790-80fb9da6-7b3f-4e24-8089-ce66195273be.png#averageHue=%2324384b&clientId=u4a9543be-8313-4&from=ui&id=e7oUe&originHeight=436&originWidth=802&originalType=binary&ratio=1&rotation=0&showTitle=false&size=68506&status=done&style=none&taskId=uf6c04d19-5b83-44f6-a0fa-b08c550a743&title=)但是要注意 create 即使创建新的文件后，如果没有主动通知应用程序，那么应用程序仍然会往旧的文件(即被重命名的那个分割后的文件)写入日志。所以此时在分割后要通知应用程序重新打开新的日志文件进行写入。<br />以通知 nginx 为例，配置如下：
```nginx
/var/log/nginx/*.log {
    daily                      
    rotate 30              
    create
    sharedscripts              # 所有的文件切割之后只执行一次下面脚本，通知nginx重新打开新的日志文件进行后续写入
    postrotate
        if [ -f /run/nginx.pid ]; then
            kill -USR1 `cat /run/nginx.pid`  # 通过USER1信号通知nginx重新打开日志文件
        fi
    endscript
}
```
综上，一般情况下如果应用程序提供了通知其打开新的日志文件的接口，那么推荐使用 create 续写日志；否则推荐使用 copytruncate 续写日志。<br />注意：

- /etc/logrotate.d 中的自定义配置中，如果不配置 rotate、daily 等参数在强制手动执行时并不会继承默认的主配置 /etc/logrotate.conf。比如不配置 rotate 时，并不会继承保留 4 个分割文件，而是等价于 0，即不保留任何分割文件。所以自定义配置中推荐显式指定这些参数；
- 在系统crontab定时任务自动执行logrotate时，自定义配置会继承主配置文件中的参数；
- logrotate 执行分割的时机要依赖于 crontab 定时任务，也就是说 crontab 定时任务每日触发时，logrotate 才会读取相应配置，检查是否满足分割的条件决定是否执行分割。这意味着，在默认 crontab 每日触发 logrotate 的定时任务情况下，即便 logrotate 配置文件中配置的分割频率小于1天，也将按照 1 天的频率触发分割，除非修改 crontab 定时任务，将 logrotate 的任务触发频率修改为小于 1 天。
<a name="BbakY"></a>
#### 2.2 分割文件压缩
可以通过如下设置对分割后的日志文件开启压缩：
```
/opt/logtest/*.log {
    daily
    rotate 2
    copytruncate
    missingok
    compress           # 以gzip方式压缩
    nodelaycompress    # 所有分割后的文件都进行压缩
}
```
此时删除原有所有文件，重新创建测试日志文件 test1.log 和 test2.log，然后手动执行分割，可以生成压缩后的 .gz 文件。<br />![DM_20230201101259_010.PNG](https://cdn.nlark.com/yuque/0/2023/png/396745/1675217896226-7ce3bc45-e221-4f19-8e93-c4a10543adb2.png#averageHue=%2324384b&clientId=u4a9543be-8313-4&from=ui&id=u0deae645&originHeight=113&originWidth=484&originalType=binary&ratio=1&rotation=0&showTitle=false&size=15839&status=done&style=none&taskId=u679b2e14-e66d-4c5e-aadf-e862a1f5843&title=)<br />一般可以将 nodelaycompress 改为 delaycompress，这样分割后对最新的序号为1的文件不会进行压缩，对其他序号的文件进行压缩，这样可以方便查看最新的分割日志。<br />![DM_20230201101259_011.PNG](https://cdn.nlark.com/yuque/0/2023/png/396745/1675217896238-31c1db35-0b87-4e2a-b5b8-7efd5caeb5f3.png#averageHue=%2324384b&clientId=u4a9543be-8313-4&from=ui&id=jFNba&originHeight=172&originWidth=504&originalType=binary&ratio=1&rotation=0&showTitle=false&size=26828&status=done&style=none&taskId=u24d67356-baf1-4c82-b1b0-465d0b27027&title=)
<a name="VXE18"></a>
#### 2.3 按照时间分割
按照时间分割可以定时分割出一个日志，比如每天分割一次，配合其他参数可以完成保留最近 n 天日志的功能。以下配置可以实现每天分割一次日志，并且保留最近 30 天的分割日志。
```
/opt/logtest/*.log {
    daily      # 每天分割一次
    rotate 30  # 保留最近30个分割后的日志文件
    copytruncate
    missingok
    dateext  # 切割后的文件添加日期作为后缀
    dateyesterday # 配合dateext使用，添加前一天的日期作为分割后日志的后缀
}
```
还是先删除原来的所有文件，重新创建。
```
touch test{1,2}.log
```
再手动执行分割，此时生成的分割后的文件将不再以序号作为文件名结尾，而是以昨天的日期作为结尾。<br />![DM_20230201101259_012.PNG](https://cdn.nlark.com/yuque/0/2023/png/396745/1675217919309-c90883ef-30ba-4b64-833a-896e58176e02.png#averageHue=%2324384a&clientId=u4a9543be-8313-4&from=ui&id=u3753e115&originHeight=110&originWidth=503&originalType=binary&ratio=1&rotation=0&showTitle=false&size=15537&status=done&style=none&taskId=u5a9ff11f-c651-4183-ad77-ddbbf260415&title=)<br />并且如果马上再手动执行一次分割，由于日期相同，不会像原来一样生成序号递增的新日志文件，此时相当于没有执行任何分割操作。即同一天只能分割一次，第二天再次执行才会分割出新的日期结尾的文件，所以此时设置 rotate 的值即为保留最近多少天日志的意思。![DM_20230201101259_013.PNG](https://cdn.nlark.com/yuque/0/2023/png/396745/1675217919328-2d23c9d7-8f15-4c00-91e8-e5134d317576.png#averageHue=%2324384a&clientId=u4a9543be-8313-4&from=ui&id=X3aXu&originHeight=386&originWidth=691&originalType=binary&ratio=1&rotation=0&showTitle=false&size=63189&status=done&style=none&taskId=uccfb6060-fd41-4f72-a631-11908395f25&title=)<br />此外，默认添加的日期后缀格式为 yyyyMMdd，可以用 dateformat 自定义。
```
/opt/logtest/*.log {
    daily      # 每天分割一次
    rotate 30  # 保留最近30个分割后的日志文件
    copytruncate
    missingok
    dateext  # 切割后的文件添加日期作为后缀
    dateyesterday # 配合dateext使用，添加前一天的日期作为分割后日志的后缀
    dateformat -%Y-%m-%d  # 格式为2022-02-08
}
```
<a name="sHrvF"></a>
#### 2.4 按照文件大小分割
可以利用 size 配置指定当日志文件达到多大体积时才进行分割。以下配置指定了每天执行分割，但是只有当日志文件大于 5M 时才真正执行分割操作。
```
/opt/logtest/*.log {
    daily      # 每天分割一次
    size 5M    # 源文件小于5M时不分割
    rotate 30  # 保留最近30个分割后的日志文件
    create
    missingok
    dateext  # 切割后的文件添加日期作为后缀
    dateyesterday # 配合dateext使用，添加前一天的日期作为分割后日志的后缀
}
```
注意：这个配置并不是说日志文件达到指定大小就自动执行分割，它还是要遵循定时任务。比如配置了daily只有到每天指定时间执行分割任务时，才会检查文件大小，对超过指定大小的文件进行分割。
<a name="xxYYG"></a>
#### 2.5 自定义每小时分割
logrotate 实现每日定时执行日志分割的原理是通过 cron 定时任务，默认在 /etc/cron.daily 中包含 logrotate 可执行命令，所以系统每天会定时启动 logrotate，然后它会根据配置中具体分割频率（daily、weekly 等）以及其他条件（比如 size）决定是否要真正执行分割操作。<br />![DM_20230201101259_014.PNG](https://cdn.nlark.com/yuque/0/2023/png/396745/1675218001187-fc2100b3-c319-4e41-b731-4365cc37bc28.png#averageHue=%2324384a&clientId=u4a9543be-8313-4&from=ui&id=uee88ef69&originHeight=116&originWidth=460&originalType=binary&ratio=1&rotation=0&showTitle=false&size=15950&status=done&style=none&taskId=u48f01c81-ab32-4822-ad92-9b05e2624e4&title=)<br />如果想要实现每小时进行一次分割，需要如下步骤：

1. logrotate 配置文件中指定分割频率为 hourly；
2. 配置完以后，还需要在 cron 的每小时定时任务中加入 logrotate，因为默认情况下只有 /etc/cron.daily 中包含 logrotate 可执行命令，要将它往 /etc/cron.hourly 中也拷贝一份，这样系统才会每小时调用一次 logrotate 去执行分割。
```bash
cp /etc/cron.daily/logrotate /etc/cron.hourly/
```
<a name="Zt0n5"></a>
#### 2.6 自定义分割执行时间
logrotate 是基于 cron 运行的，所以这个时间是由 cron 控制的，具体可以查询 cron 的配置文件 /etc/crontab 。旧版 CentOS 的 cron 的配置文件是 /etc/crontab ，新版CentOS 改为 /etc/anacrontab。![DM_20230201101259_015.PNG](https://cdn.nlark.com/yuque/0/2023/png/396745/1675218001241-f26ca64a-45e6-4229-a6e7-b418ff5c9fa7.png#averageHue=%2324384c&clientId=u4a9543be-8313-4&from=ui&id=O7UYA&originHeight=315&originWidth=668&originalType=binary&ratio=1&rotation=0&showTitle=false&size=43714&status=done&style=none&taskId=u5550f74e-9a5c-4984-a4e3-40635e5ee79&title=)<br />从上面的内容可以看出：

- 如果机器 没有关机，默认 logrotate（配置文件里设置的是 cron.daily）一般会在每天的 3 点 05 分到 3 点 50 分之间执行， 真实的延迟时间是 RANDOM_DELAY + delay in minute；
- 如果在 3-22 这个时间段内服务器处于 关机状态，则 logrotate 会在机器开机 5 分钟后执行分割日志的操作。

如果觉得每天凌晨 3 点多执行日志分割不合适，那么可以自定义分割执行时间。实现方式可以是：

- 修改 /etc/anacrontab 中的定时执行时间，实际不推荐，可能会影响系统其他定时任务；
- 在 /etc/logrotate.d 以外的其他目录创建 logrotate 配置文件，然后利用 crontab 自定义 cron 表达式来执行 logrotate 对该配置进行分割操作。

推荐采用 crontab 方式自定义执行时间，步骤如下：

1. 在非 /etc/logrotate.d 目录创建 logrotate 配置文件，这是为了避免被系统的定时任务扫描到该配置而导致重复执行分割。
2. 添加 crontab 计划任务，在 root 用户下执行 crontab -e 进入 vim 模式，进行编辑。
```
crontab -e
# 每天 23点59分进行日志切割
59 23 * * * /usr/sbin/logrotate -f /etc/logrotate_mytime/nginx
```

1. 重启 crontab。
```bash
# centos6: 
service crond restart

# centos7: 
systemctl restart crond
```
<a name="i1zFG"></a>
## nginx 日志分割步骤
在 /etc/logrotate.d 中创建文件 nginx，作为 nginx 日志分割的配置文件。指定每天执行一次分割，并且当文件大于 5M 时才进行分割。同时指定 notifempty，当日志文件为空时不分割。
```
/opt/docker-ws/nginx/logs/*.log {
    daily      # 每天分割一次
    size 5M    # 源文件小于5M时不分割
    rotate 30  # 保留最近30个分割后的日志文件
    copytruncate
    notifempty # 当日志文件为空时不分割
    missingok
    dateext  # 切割后的文件添加日期作为后缀
}
```
这样配置了以后系统会在凌晨3点多执行分割操作，执行结果会保存到/var/spool/mail/root中<br />![DM_20230201101259_016.PNG](https://cdn.nlark.com/yuque/0/2023/png/396745/1675218001301-41f1390a-0406-4a40-a300-645ccbd151ba.png#averageHue=%2324394c&clientId=u4a9543be-8313-4&from=ui&id=jvafu&originHeight=360&originWidth=591&originalType=binary&ratio=1&rotation=0&showTitle=false&size=62109&status=done&style=none&taskId=u3700975e-0a09-40a6-b211-9b170674ba9&title=)<br />查看结果发现提示权限不够而分割失败，这是因为开启了 selinux 导致，解决方案有如下 2 种：

- 关闭 selinux
- 利用 semanage 修改待分割的日志文件所在目录的权限
```bash
# 开放/opt/logtest目录的权限
semanage fcontext -a -t var_log_t "/opt/logtest(/.*)?"
restorecon -Rv /opt/logtest
```
