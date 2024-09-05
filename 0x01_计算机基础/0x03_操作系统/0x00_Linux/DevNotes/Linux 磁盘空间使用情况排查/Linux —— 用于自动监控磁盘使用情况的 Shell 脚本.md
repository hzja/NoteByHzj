LinuxShell<br />如果在服务器上运行关键任务，那么监控和通知管理员磁盘使用情况很重要。本文介绍编写一个脚本来自动监控并在达到阈值时将报告发送到自己的邮箱。<br />写一个 shell 脚本，它在 crontab 中指定固定时间间隔监控磁盘使用情况，并在报告达到指定的阈值时通过电子邮件发送报告。首先，需要确保系统安装了mail命令和postfix发送邮件服务。下面是在Centos8中安装的命令：
```bash
[root@localhost ~]# yum -y install mailx postfix
[root@localhost ~]# systemctl enable --now postfix
```
下面使用mail命令发送邮件测试：
```bash
[root@localhost ~]# echo "This message will go into the body of the mail." | mail -s "Testing from the server." root@localhos
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1655789545945-797509d1-6206-4db2-9827-01e75ff8c5da.png#clientId=u25e32f26-4071-4&from=paste&id=u6f6026e6&originHeight=416&originWidth=872&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue6606fe6-ae57-4c2b-be5e-03a194f8b39&title=)<br />这里不讲解如何配置smtp服务。
<a name="b6gnk"></a>
## 测试磁盘使用情况的脚本
以下是测试磁盘使用情况并将报告发送到指定邮箱的脚本。脚本的解释在代码片段之后给出。
```bash
[root@localhost ~]# vim disk-monitor.sh
#!/bin/bash

MAILID="admin@linuxprobe.com"

VALUE=80

SERVERNAME=$(hostname)

MAIL=/bin/mail

for line in $(df -hP | egrep '^/dev/' | awk '{ print $1 "_:_" $5 }')
  do
    FILESYSTEM=$(echo "$line" | awk -F"_:_" '{ print $1 }')
    DISK_USAGE=$(echo "$line" | awk -F"_:_" '{ print $2 }' | cut -d'%' -f1 )

    if [ $DISK_USAGE -ge $VALUE ];
    then
      EMAIL="$SERVERNAME - $(date): $FILESYSTEM Exceeded the threshold VALUE\n"
      EMAIL="$EMAIL\n Usage Details\n Current Usage:($DISK_USAGE%) Threshold value: ($VALUE%)"
      echo -e "$EMAIL" | $MAIL -s ""$SERVERNAME" Disk Usage Alert: Needs Attention!" "$MAILID"
    elif [ $DISK_USAGE -lt $VALUE ];
    then
      EMAIL="$EMAIL\n$FILESYSTEM ($DISK_USAGE%) is lessthan the threshold ($VALUE%)"
      EMAIL="$EMAIL\n"
      echo -e "$EMAIL" | $MAIL -s ""$SERVERNAME" Disk Usage Alert: Threshold Not Reached" "$MAILID"
  fi
done
```
<a name="tj19N"></a>
## 监控脚本说明
上述脚本使用`df -hP`命令获取内存信息。<br />环境变量MAILID保存必须在其中发送报告的邮箱地址。<br />VALUE 保存要设置的阈值。<br />SERVERNAME 保存服务器的主机名，用于电子邮件通知中的服务器标识。<br />使用 for 循环，它将遍历所有具有“/dev”挂载点的文件系统。然后它将文件系统名称和磁盘使用百分比保存到两个变量FILESYSTEM 和 DISK_USAGE。<br />使用上述变量，磁盘使用量等于阈值 VALUE 变量。根据结果，将向指定的邮箱发送一封电子邮件。
<a name="Wgdzh"></a>
## 监控脚本执行
下面需要修改一下disk-monitor.sh脚本的权限，为它添加可执行权限：
```bash
[root@localhost ~]# chmod +x disk-monitor.sh
```
<br />使用下面方式运行脚本：
```bash
[root@localhost ~]# ./disk-monitor.sh
```
<a name="m293V"></a>
## 自动化监控过程
手动运行脚本没有任何意义，因为希望自动运行该过程。自动运行该过程的最佳方法是将脚本添加到 crontab配置文件中。<br />使用下面方式打开crontab：
```bash
[root@localhost ~]# crontab -e
```
将下面内容添加到配置文件中：`*/5 * * * * sh /home/ec2-user/disk-monitor.sh`<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1655789546031-a959827e-beaa-40e9-b60c-3258ccab3f8f.png#clientId=u25e32f26-4071-4&from=paste&id=u1cfae3e3&originHeight=131&originWidth=457&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc571f4a6-8da6-44b1-b549-7757bb7dc8e&title=)
