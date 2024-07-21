<a name="iMZvO"></a>
# 1、背景
<a name="DczAq"></a>
## A.系统镜像
**服务器镜像：CentOS7**
<a name="1hAXg"></a>
## B.应用安装
**安装的应用：Hadoop、jdk**
<a name="zwTVA"></a>
## C.查看YRAN启动日志

```
[root@iZuligp6e1dyzfZ /]# cd opt/module/hadoop-2.7.7/logs/
[root@iZuligp6e1dyzfZ logs]# ll
total 952
-rw-r--r-- 1 root root  60929 Aug  2 07:01 hadoop-root-datanode-iZuligp6e1dyzfZ.log
-rw-r--r-- 1 root root    715 Jul 27 23:43 hadoop-root-datanode-iZuligp6e1dyzfZ.out
-rw-r--r-- 1 root root  55250 Aug  2 07:01 hadoop-root-namenode-iZuligp6e1dyzfZ.log
-rw-r--r-- 1 root root    715 Jul 27 23:41 hadoop-root-namenode-iZuligp6e1dyzfZ.out
-rw-r--r-- 1 root root 230253 Aug  2 08:51 mapred-root-historyserver-iZuligp6e1dyzfZ.log
-rw-r--r-- 1 root root   1477 Jul 30 08:12 mapred-root-historyserver-iZuligp6e1dyzfZ.out
-rw-r--r-- 1 root root   1484 Jul 29 23:30 mapred-root-historyserver-iZuligp6e1dyzfZ.out.1
-rw-r--r-- 1 root root      0 Jul 27 23:41 SecurityAuth-root.audit
drwxr-xr-x 2 root root   4096 Aug  2 08:51 userlogs
-rw-r--r-- 1 root root 318193 Aug  2 08:11 yarn-root-nodemanager-iZuligp6e1dyzfZ.log
-rw-r--r-- 1 root root   1508 Jul 30 08:11 yarn-root-nodemanager-iZuligp6e1dyzfZ.out
-rw-r--r-- 1 root root    701 Jul 29 23:58 yarn-root-nodemanager-iZuligp6e1dyzfZ.out.1
-rw-r--r-- 1 root root   1508 Jul 28 21:50 yarn-root-nodemanager-iZuligp6e1dyzfZ.out.2
-rw-r--r-- 1 root root 229749 Aug  2 08:26 yarn-root-resourcemanager-iZuligp6e1dyzfZ.log
-rw-r--r-- 1 root root   1524 Jul 30 08:11 yarn-root-resourcemanager-iZuligp6e1dyzfZ.out
-rw-r--r-- 1 root root    701 Jul 30 07:55 yarn-root-resourcemanager-iZuligp6e1dyzfZ.out.1
-rw-r--r-- 1 root root    701 Jul 29 23:57 yarn-root-resourcemanager-iZuligp6e1dyzfZ.out.2
-rw-r--r-- 1 root root    701 Jul 29 23:56 yarn-root-resourcemanager-iZuligp6e1dyzfZ.out.3
-rw-r--r-- 1 root root   1524 Jul 28 21:46 yarn-root-resourcemanager-iZuligp6e1dyzfZ.out.4
[root@iZuligp6e1dyzfZ logs]# 
```
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564707383539-ff282e77-e8fc-4f14-8c22-038077105be2.png#align=left&display=inline&height=483&originHeight=1328&originWidth=2652&size=348328&status=done&style=none&width=964.3636363636364)
<a name="1alVq"></a>
## D.找到漏洞入侵
**![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564652831418-4208cfb2-0b12-4982-ba7f-c7151087fe46.png#align=left&display=inline&height=676&originHeight=1860&originWidth=3797&size=932066&status=done&style=none&width=1380.7272727272727)![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564652910199-4c28568c-7f86-4a37-bce9-f0147fdb3310.png#align=left&display=inline&height=677&originHeight=1863&originWidth=3797&size=985899&status=done&style=none&width=1380.7272727272727)![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564652940210-e890377e-842d-431d-9013-11432d828c3c.png#align=left&display=inline&height=676&originHeight=1860&originWidth=3797&size=948265&status=done&style=none&width=1380.7272727272727)**<br />**
<a name="FvnKM"></a>
# 2、查看并解决步骤
<a name="HY9uV"></a>
## A.系统CPU占比情况
**使用top命令查看系统进程情况**

```
[root@iZuligp6e1dyzfZ etc]# top
top - 16:08:10 up 14 days,  6:44,  2 users,  load average: 1.65, 1.37, 1.36
Tasks:  80 total,   2 running,  76 sleeping,   0 stopped,   2 zombie
%Cpu(s): 37.3 us, 62.0 sy,  0.0 ni,  0.7 id,  0.0 wa,  0.0 hi,  0.0 si,  0.0 st
KiB Mem :  1883724 total,    75020 free,  1291468 used,   517236 buff/cache
KiB Swap:        0 total,        0 free,        0 used.   360252 avail Mem 

  PID USER      PR  NI    VIRT    RES    SHR S %CPU %MEM     TIME+ COMMAND                                                
20160 root      20   0  136688  21164    744 S 98.0  1.1   4:28.76 networkservice                                         
 9775 systemd+  20   0 1376136 406916      0 S  0.3 21.6  56:04.50 mysqld                                                 
10978 root       0 -20   32788   4036   2336 S  0.3  0.2   3:48.46 AliYunDunUpdate                                        
11012 root       0 -20  132124  11788   4724 S  0.3  0.6  42:09.87 AliYunDun                                              
23562 root      20   0 2787660 149464   6468 S  0.3  7.9   5:19.77 java                                                   
    1 root      20   0   43216   2916   1696 S  0.0  0.2   0:10.91 systemd                                                
    2 root      20   0       0      0      0 S  0.0  0.0   0:00.02 kthreadd                                               
    3 root      20   0       0      0      0 S  0.0  0.0   0:09.31 ksoftirqd/0                                            
    5 root       0 -20       0      0      0 S  0.0  0.0   0:00.00 kworker/0:0H                                           
    7 root      rt   0       0      0      0 S  0.0  0.0   0:00.00 migration/0                                            
    8 root      20   0       0      0      0 S  0.0  0.0   0:00.00 rcu_bh                                                 
    9 root      20   0       0      0      0 R  0.0  0.0   1:15.81 rcu_sched                                              
   10 root      rt   0       0      0      0 S  0.0  0.0   0:04.66 watchdog/0                                             
   12 root      20   0       0      0      0 S  0.0  0.0   0:00.00 kdevtmpfs                                              
   13 root       0 -20       0      0      0 S  0.0  0.0   0:00.00 netns                                                  
   14 root      20   0       0      0      0 S  0.0  0.0   0:00.45 khungtaskd                                             
   15 root       0 -20       0      0      0 S  0.0  0.0   0:00.00 writeback                                              
   16 root       0 -20       0      0      0 S  0.0  0.0   0:00.00 kintegrityd                                            
   17 root       0 -20       0      0      0 S  0.0  0.0   0:00.00 bioset                                                 
   18 root       0 -20       0      0      0 S  0.0  0.0   0:00.00 kblockd                                                
   19 root       0 -20       0      0      0 S  0.0  0.0   0:00.00 md                                                     
   25 root      20   0       0      0      0 S  0.0  0.0   0:01.32 kswapd0  
```

![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564647719054-a71d216b-d461-44d0-955f-6646b7e6f98c.png#align=left&display=inline&height=605&originHeight=1664&originWidth=2447&size=339357&status=done&style=none&width=889.8181818181819)
<a name="8HYQo"></a>
## B.(ex-特殊情况)CPU占用高而未列出的进程找到相应的PID
**使用下面命令找到被隐藏的挖矿进程**
```
[root@iZuligp6e1dyzfZ etc]# ps -aux --sort=-pcpu|head -10
```

<a name="YSAXj"></a>
## C.kill这个占高CPU的进程
**kill这个进程并查看CPU占用情况**
```
[root@iZuligp6e1dyzfZ etc]# kill -9 20160
[root@iZuligp6e1dyzfZ etc]# top
top - 16:08:28 up 14 days,  6:44,  2 users,  load average: 1.49, 1.36, 1.36
Tasks:  79 total,   1 running,  76 sleeping,   0 stopped,   2 zombie
%Cpu(s):  0.7 us,  0.7 sy,  0.0 ni, 98.7 id,  0.0 wa,  0.0 hi,  0.0 si,  0.0 st
KiB Mem :  1883724 total,    97552 free,  1271076 used,   515096 buff/cache
KiB Swap:        0 total,        0 free,        0 used.   382588 avail Mem 

  PID USER      PR  NI    VIRT    RES    SHR S %CPU %MEM     TIME+ COMMAND                                                
 9775 systemd+  20   0 1376136 406916      0 S  0.3 21.6  56:04.55 mysqld                                                 
11012 root       0 -20  132124  11788   4724 S  0.3  0.6  42:09.93 AliYunDun                                              
20120 root      20   0       0      0      0 S  0.3  0.0   0:00.16 kworker/0:0                                            
    1 root      20   0   43216   2916   1696 S  0.0  0.2   0:10.91 systemd                                                
    2 root      20   0       0      0      0 S  0.0  0.0   0:00.02 kthreadd                                               
    3 root      20   0       0      0      0 S  0.0  0.0   0:09.31 ksoftirqd/0                                            
    5 root       0 -20       0      0      0 S  0.0  0.0   0:00.00 kworker/0:0H                                           
    7 root      rt   0       0      0      0 S  0.0  0.0   0:00.00 migration/0                                            
    8 root      20   0       0      0      0 S  0.0  0.0   0:00.00 rcu_bh                                                 
    9 root      20   0       0      0      0 S  0.0  0.0   1:15.82 rcu_sched                                              
   10 root      rt   0       0      0      0 S  0.0  0.0   0:04.66 watchdog/0                                             
   12 root      20   0       0      0      0 S  0.0  0.0   0:00.00 kdevtmpfs                                              
   13 root       0 -20       0      0      0 S  0.0  0.0   0:00.00 netns                                                  
   14 root      20   0       0      0      0 S  0.0  0.0   0:00.45 khungtaskd                                             
   15 root       0 -20       0      0      0 S  0.0  0.0   0:00.00 writeback                                              
   16 root       0 -20       0      0      0 S  0.0  0.0   0:00.00 kintegrityd                                            
   17 root       0 -20       0      0      0 S  0.0  0.0   0:00.00 bioset                                                 
   18 root       0 -20       0      0      0 S  0.0  0.0   0:00.00 kblockd                                                
   19 root       0 -20       0      0      0 S  0.0  0.0   0:00.00 md                                                     
   25 root      20   0       0      0      0 S  0.0  0.0   0:01.32 kswapd0                                                
   26 root      25   5       0      0      0 S  0.0  0.0   0:00.00 ksmd  
```
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564648323858-fb62ad5a-8ade-4d3e-a7c3-2db57461671e.png#align=left&display=inline&height=605&originHeight=1664&originWidth=2314&size=327090&status=done&style=none&width=841.4545454545455)<br />**CPU高占用占用情况暂时解决了
<a name="F8Vu4"></a>
## D.定时任务重启了木马脚本
**查看系统定时任务可以看到有一个每隔一分钟执行/etc/update.sh的脚本的任务**

```
[root@iZuligp6e1dyzfZ etc]# crontab -l
* 1 * * * * * /etc/update.sh
```
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564972748267-819caef8-296d-4dd1-abb4-e61e31d5deea.png#align=left&display=inline&height=48&originHeight=113&originWidth=1105&size=16595&status=done&style=none&width=469)

**清除系统定时任务再次查看定时的任务**

```
[root@iZuligp6e1dyzfZ etc]# crontab -r
[root@iZuligp6e1dyzfZ etc]# crontab -l
```
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564648796235-d95bf2e6-1752-414b-8322-b8cd39ef0af7.png#align=left&display=inline&height=40&originHeight=110&originWidth=1114&size=23536&status=done&style=none&width=405.09090909090907)<br />![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564648842032-a98b439a-7fd0-4e0a-82e6-201d5d71f004.png#align=left&display=inline&height=43&originHeight=117&originWidth=1158&size=19807&status=done&style=none&width=421.09090909090907)
<a name="GBs9M"></a>
## E.使用命令查看高CPU进程对应的程序
**通过该命令可以看到该程序位于/etc/networkservice（进程名称为伪装名称）**
```
[root@iZuligp6e1dyzfZ etc]# ll /proc/20160 | grep exe

```
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564972809597-f0bbe5db-4e91-491d-841c-4e4feeeeadb0.png#align=left&display=inline&height=110&originHeight=121&originWidth=1850&size=27697&status=done&style=none&width=1681.8181453657553)
<a name="bcGyz"></a>
## F.进入目录查看异常启动程序
**如下列出了矿机的异常程序**

```
[root@iZuligp6e1dyzfZ /]# cd /etc/
[root@iZuligp6e1dyzfZ etc]# ll
total 6708
-rwxrwxrwx   1 root root         0 Jul 31 16:00 config.json
-rwxrwxrwx   1 root root   2584072 Jul 31 13:13 networkservice
-rwxrwxrwx   1 root root   1929480 Jul 31 13:13 sysguard
-rwxrwxrwx   1 root root    854364 Jul 31 13:30 sysupdate
-rwxrwxrwx   1 root root         0 Jul 31 16:00 update.sh
```
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564649370064-fab42cfb-00e9-4783-a2ae-18c95024712e.png#align=left&display=inline&height=605&originHeight=1664&originWidth=2156&size=353999&status=done&style=none&width=784)<br />![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564649401539-28cf4715-d2d7-4fd6-ac59-d0a16d867666.png#align=left&display=inline&height=605&originHeight=1664&originWidth=2223&size=359328&status=done&style=none&width=808.3636363636364)<br />![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564649454183-9c687742-c029-45ee-ada6-60cfe0917f16.png#align=left&display=inline&height=605&originHeight=1664&originWidth=2297&size=375739&status=done&style=none&width=835.2727272727273)

<a name="S7dIL"></a>
## G.删除上面的异常程序发现没有权限或者文件被加了锁

```
[root@iZuligp6e1dyzfZ etc]# rm -rf networkservice
rm: cannot remove 'networkservice':Operation not permitted
[root@iZuligp6e1dyzfZ etc]# lsattr networkservice
----i--------e-- networkservice
```
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564650179519-0e78ed07-0909-4109-b9fc-b6b947466b7f.png#align=left&display=inline&height=42&originHeight=115&originWidth=1443&size=18821&status=done&style=none&width=524.7272727272727)<br />**_lsattr 命令介绍<br />发现 I 权限如下：<br />不能被删除、改名、设定连结、写入或新增数据；_**
<a name="RjS9F"></a>
## H.使用命令给异常程序或者文件撤销i权限并查看文件列表是否删除

```
[root@iZuligp6e1dyzfZ etc]# chattr -i networkservice 
[root@iZuligp6e1dyzfZ etc]# rm -rf networkservice 
[root@iZuligp6e1dyzfZ etc]# ll
```
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564650253768-bbb00bf7-9a81-406c-b266-189571b8055c.png#align=left&display=inline&height=83&originHeight=228&originWidth=1563&size=38923&status=done&style=none&width=568.3636363636364)

<a name="RZ1mz"></a>
## I.(ex-特殊情况)chattr命令不能找到Bash需要在yum下载

```
[root@iZuligp6e1dyzfZ etc]# yum -y install e2fsprogs
```
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564650545149-016acc61-b2af-439d-beca-e279fa528cf2.png#align=left&display=inline&height=26&originHeight=72&originWidth=1492&size=12729&status=done&style=none&width=542.5454545454545)

<a name="7z4El"></a>
## G. 按照以上操作给其他异常文件撤销i权限并删除异常文件

```
[root@iZuligp6e1dyzfZ etc]# lsattr config.json 
----i--------e-- config.json
[root@iZuligp6e1dyzfZ etc]# chattr -i config.json 
[root@iZuligp6e1dyzfZ etc]# cat config.json 
[root@iZuligp6e1dyzfZ etc]# rm -rf config.json 
```
<a name="p09Ou"></a>
## K.再次使用top命令查看CPU使用情况，如果三分钟内没有异常暴增情况则说明已完成病毒清理
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564651331205-1e76f890-11f4-466f-bca1-01ffbc327499.png#align=left&display=inline&height=605&originHeight=1664&originWidth=2361&size=335437&status=done&style=none&width=858.5454545454545)
<a name="n0Nva"></a>
## L.最后检查/var/spool/cron清理异常脚本任务

```xml
[root@iZuligp6e1dyzfZ /]# cd var/spool/cron
[root@iZuligp6e1dyzfZ cron]# ll
total 4
-rw------- 1 root root 47 Aug  2 04:18 root
[root@iZuligp6e1dyzfZ cron]# cat root
*/30 * * * * sh /etc/update.sh >/dev/null 2>&1
[root@iZuligp6e1dyzfZ cron]# vim root

```
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564705163802-dac2cc55-7a14-420f-ba4d-2bb43cd09f68.png#align=left&display=inline&height=177&originHeight=488&originWidth=1526&size=79050&status=done&style=none&width=554.9090909090909)
