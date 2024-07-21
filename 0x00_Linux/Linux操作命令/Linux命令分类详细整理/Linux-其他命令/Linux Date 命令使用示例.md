Linux <br />date 命令用于显示和设置 Linux 系统上的日期和时间设置。
<a name="feQEs"></a>
## 1、不带选项的Date命令
date命令不加任何命令选项，它显示当前日期和时间，包括星期几，月份，年份，h:m:s格式的时间以及时区，如下所示。
```bash
[root@server1 ~]# date
Mon May 24 14:50:31 CST 2021
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627344365358-477b38bc-66b8-4749-a4c1-1ffa0b609c41.png#clientId=u856f8380-f32e-4&from=paste&height=73&id=uc262f736&originHeight=220&originWidth=1461&originalType=binary&ratio=1&size=87496&status=done&style=none&taskId=ud196ccdf-8287-4fa8-aa51-6b70f4d9a82&width=487)
<a name="okgXH"></a>
## 2、以世界标准时间显示
要以UTC（世界标准时间）显示时间，需要加`-u`选项。
```bash
[root@server1 ~]# date -u
Mon May 24 06:51:21 UTC 2021
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627344412555-407224bd-ab93-4a48-9b46-25fb57d193d2.png#clientId=u856f8380-f32e-4&from=paste&height=74&id=u88eb8e32&originHeight=223&originWidth=1465&originalType=binary&ratio=1&size=89350&status=done&style=none&taskId=u73d9297a-f76f-4399-aa61-2bcba2f3433&width=488.3333333333333)
<a name="TxVEO"></a>
## 3、以字符串格式显示特定日期
可以使用`--date`选项以字符串格式显示特定日期，这不会影响系统的日期和时间，只是将日期格式转换为字符串：
```bash
[root@server1 ~]# date --date="5/20/2021 13:14"
Thu May 20 13:14:00 CST 2021
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627344432673-81260653-68eb-40df-bc8a-904fa2b25cc0.png#clientId=u856f8380-f32e-4&from=paste&height=73&id=u16631a82&originHeight=218&originWidth=1795&originalType=binary&ratio=1&size=106781&status=done&style=none&taskId=u0894f54a-364d-4b51-83d2-a34ed277896&width=598.3333333333334)
<a name="RX0aB"></a>
## 4、使用date命令检查过去的日期
date命令还可以打印相对于当前日期的过去日期和时间。例如查看9天前的日期是多少，请运行命令：
```bash
[root@server1 ~]# date --date="9 days ago"
Sat May 15 14:56:47 CST 2021
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627344450404-7f281973-bb45-4ebe-9263-aae588c67a1d.png#clientId=u856f8380-f32e-4&from=paste&height=73&id=u1ba9fa29&originHeight=219&originWidth=1587&originalType=binary&ratio=1&size=96345&status=done&style=none&taskId=u11175ba3-bdd3-451a-a460-6406fb19f21&width=529)<br />查看三个月前的日期是多少：
```bash
[root@server1 ~]# date --date="3 months ago"
Wed Feb 24 14:57:29 CST 2021
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627344466801-c7733a8e-903d-4fab-bde2-0a48f1691b62.png#clientId=u856f8380-f32e-4&from=paste&height=73&id=ubb219b07&originHeight=218&originWidth=1694&originalType=binary&ratio=1&size=101440&status=done&style=none&taskId=uec42ab4e-f186-48d1-8a11-6daa2251533&width=564.6666666666666)<br />查看两年前的今天日期是多少：
```bash
[root@server1 ~]# date --date="2 years ago"
Fri May 24 15:02:40 CST 2019
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627344482207-b06b3cb2-80a2-4dfe-84e8-fad48a11b65d.png#clientId=u856f8380-f32e-4&from=paste&height=74&id=u2764bd4b&originHeight=223&originWidth=1670&originalType=binary&ratio=1&size=102985&status=done&style=none&taskId=ub255f9cc-5704-44ac-95b4-912a35245ec&width=556.6666666666666)
<a name="facXG"></a>
## 5、使用date命令检查未来的日期
date命令也可以显示将来的日期，例如查看明天的日期是多少：
```bash
[root@server1 ~]# date --date="tomorrow"
Tue May 25 14:58:49 CST 2021
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627344499269-33965280-14ad-4164-91f9-8ab84da89efa.png#clientId=u856f8380-f32e-4&from=paste&height=74&id=u18bf0953&originHeight=221&originWidth=1563&originalType=binary&ratio=1&size=95968&status=done&style=none&taskId=ufc830ed1-a5f3-4d92-a8b2-2a1cab3ac38&width=521)<br />要检查从现在起一周后的确切日期，请运行：
```bash
[root@server1 ~]# date --date="next week"
Mon May 31 14:59:27 CST 2021
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627344517716-3a898e94-cce2-49b1-9050-4dd5b04606fa.png#clientId=u856f8380-f32e-4&from=paste&height=72&id=uca69198c&originHeight=215&originWidth=1608&originalType=binary&ratio=1&size=95120&status=done&style=none&taskId=u40f73ad1-9f47-48ca-8aab-b9b77eee560&width=536)<br />显示3周后的日期：
```bash
[root@server1 ~]# date --date="3 weeks"
Mon Jun 14 15:00:11 CST 2021
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627344534184-838ef04b-d375-4dcd-bae7-3f2761eaf9de.png#clientId=u856f8380-f32e-4&from=paste&height=73&id=u71b887ae&originHeight=219&originWidth=1579&originalType=binary&ratio=1&size=95589&status=done&style=none&taskId=u9456f552-7868-46b4-8b7b-192213d6578&width=526.3333333333334)<br />显示4个月后的日期，请执行以下操作：
```bash
[root@server1 ~]# date --date="4 months"
Fri Sep 24 15:01:42 CST 2021
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627344548481-89891bc0-938f-4808-b52b-68fe5e143433.png#clientId=u856f8380-f32e-4&from=paste&height=73&id=ue1f72db0&originHeight=218&originWidth=1667&originalType=binary&ratio=1&size=99570&status=done&style=none&taskId=u99f06f8c-08d4-4470-82f6-2a2231e4c00&width=555.6666666666666)<br />显示两年后今天的日期：
```bash
[root@server1 ~]# date --date="2 years"
Wed May 24 15:03:32 CST 2023
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627344568423-914e237f-8592-4cd6-939f-a887ff19d945.png#clientId=u856f8380-f32e-4&from=paste&height=72&id=udb5b2c46&originHeight=216&originWidth=1777&originalType=binary&ratio=1&size=103894&status=done&style=none&taskId=u2c8911e4-c34a-4f23-ae17-cd93b920e49&width=592.3333333333334)
<a name="HKo9O"></a>
## 6、date命令的选项
date命令带有许多选项，面列出了一些可用的格式设置选项。

- %D – 日期显示格式为 月/日/年
- %Y – 年份(例如：2021)
- %m – 月份(01-12)
- %B – 月份的完整名称 (例如 January)
- %b – 月份的短名称 (例如 Jan)
- %d – 月份中的哪一天 (例如 01)
- %j – 一年中的第几天(001-366)
- %u – 一个星期中的第几天 (1-7)
- %A – 星期几的全程(例如 Friday)
- %a – 星期几的短名称 (例如 Fri)
- %H – 小时，24小时制 (00-23)
- %I – 小时，12小时制 (01-12)
- %M – 分钟 (00-59)
- %S – 秒 (00-60)

使用date选项的语法非常简单：
```bash
date "+%option"
```
例如，要以yy/mm/dd格式打印日期，请运行：
```bash
[root@server1 ~]# date "+%Y/%m/%d"
2021/05/24
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627344596844-892b43b2-c5d4-436f-8dc8-fb198d1158bf.png#clientId=u856f8380-f32e-4&from=paste&height=73&id=ue5ae3c7a&originHeight=220&originWidth=1691&originalType=binary&ratio=1&size=99041&status=done&style=none&taskId=udea8d806-f07a-4db4-b096-a0d8e7dd6fb&width=563.6666666666666)<br />要打印星期几，月份，日，年份和当前时间，请执行以下操作：
```bash
[root@server1 ~]# date "+%A %B %d %Y %T"
Monday May 24 2021 15:24:07
```
<a name="poS5H"></a>
## 7、如何设置日期和时间
date命令还允许设置日期和时间。例如，要将日期和时间设置为2021年6月25日上午11:15，请运行以下命令：
```bash
[root@server1 ~]# date --set="20210625 11:15"
Fri Jun 25 11:15:00 CST 2021
```
<a name="NW75c"></a>
## 8、在变量中使用date命令
在创建Shell脚本的时候，将date命令保存到一个变量中，然后使用该变量创建日志文件，示例如下所示：
```bash
#!/bin/bash
LOGFILE=/tmp/logs-$(date +%d-%m-%Y_%T)
echo "##Check Cluster for Failed Resources##"  >> $LOGFILE
crm_mon -1 -rf | grep FAILED  >> $LOGFILE
echo -e "\n\n" >> $LOGFILE
echo "##Check Cluster for Stopped Resources##"  >> $LOGFILE
crm_mon -1 -rf | grep -i STOPPED  >> $LOGFILE
echo -e "\n\n" >> $LOGFILE
```
