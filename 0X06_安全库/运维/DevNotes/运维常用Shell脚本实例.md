Shell<br />**Shell 脚本部分实例：**SVN 完整备份、Zabbix 监控用户密码过期、构建本地 YUM 以及上篇文章中有读者的需求（负载高时，查出占用比较高的进程脚本并存储或推送通知）；
<a name="NKfDL"></a>
### SVN 完整备份
通过 **hotcopy** 进行 SVN 完整备份，备份保留 7 天。
```shell
#!/bin/bash
# Filename   :  svn_backup_repos.sh
# Date       :  2020/12/14
# Author     :  Fcant      
# Crontab    :  59 23 * * * /bin/bash $BASE_PATH/svn_backup_repos.sh >/dev/null 2>&1
# Notes      :  将脚本加入crontab中，每天定时执行
# Description:  SVN完全备份
set -e
SRC_PATH="/opt/svndata"
DST_PATH="/data/svnbackup"
LOG_FILE="$DST_PATH/logs/svn_backup.log"
SVN_BACKUP_C="/bin/svnadmin hotcopy"
SVN_LOOK_C="/bin/svnlook youngest"
TODAY=$(date +'%F')
cd $SRC_PATH
ALL_REPOS=$(find ./ -maxdepth 1 -type d ! -name 'httpd' -a ! -name 'bak' | tr -d './')
# 创建备份目录，备份脚本日志目录
test -d $DST_PATH || mkdir -p $DST_PATH
test -d $DST_PATH/logs || mkdir $DST_PATH/logs
test -d $DST_PATH/$TODAY || mkdir $DST_PATH/$TODAY
# 备份repos文件
for repo in $ALL_REPOS
do
    $SVN_BACKUP_C $SRC_PATH/$repo $DST_PATH/$TODAY/$repo
    # 判断备份是否完成
    if $SVN_LOOK_C $DST_PATH/$TODAY/$repo;then
        echo "$TODAY: $repo Backup Success" >> $LOG_FILE 
    else
        echo "$TODAY: $repo Backup Fail" >> $LOG_FILE
    fi
done
# # 备份用户密码文件和权限文件
cp -p authz access.conf $DST_PATH/$TODAY
# 日志文件转储
mv $LOG_FILE $LOG_FILE-$TODAY
# 删除七天前的备份
seven_days_ago=$(date -d "7 days ago" +'%F')
rm -rf $DST_PATH/$seven_days_ago
```
<a name="yYTCY"></a>
### zabbix 监控用户密码过期
用于 Zabbix 监控 Linux 系统用户（shell 为 /bin/bash 和 /bin/sh）密码过期，密码有效期剩余 7 天触发加自动发现用户。
```shell
#!/bin/bash
diskarray=(`awk -F':' '$NF ~ /\/bin\/bash/||/\/bin\/sh/{print $1}' /etc/passwd`)
length=${#diskarray[@]}
printf "{\n"
printf  '\t'"\"data\":["
for ((i=0;i<$length;i++))
do
    printf '\n\t\t{'
    printf "\"{#USER_NAME}\":\"${diskarray[$i]}\"}"
    if [ $i -lt $[$length-1] ];then
            printf ','
    fi
done
printf  "\n\t]\n"
printf "}\n"
检查用户密码过期
#!/bin/bash
export LANG=en_US.UTF-8
SEVEN_DAYS_AGO=$(date -d '-7 day' +'%s')
user="$1"
# 将Sep 09, 2018格式的时间转换成unix时间
expires_date=$(sudo chage -l $user | awk -F':' '/Password expires/{print $NF}' | sed -n 's/^ //p')
if [[ "$expires_date" != "never" ]];then
    expires_date=$(date -d "$expires_date" +'%s')
    if [ "$expires_date" -le "$SEVEN_DAYS_AGO" ];then
        echo "1"
    else
        echo "0"
    fi
else
    echo "0"
fi
```
<a name="zc7uW"></a>
### 构建本地YUM
通过 rsync 的方式同步 yum，通过 Nginx 只做 http yum 站点；<br />但是 centos6 的镜像最近都不能用了，国内貌似都禁用了，如果找到合适的自行更换地址。
```shell
#!/bin/bash
# 更新yum镜像
RsyncCommand="rsync -rvutH -P --delete --delete-after --delay-updates --bwlimit=1000"
DIR="/app/yumData"
LogDir="$DIR/logs"
Centos6Base="$DIR/Centos6/x86_64/Base"
Centos7Base="$DIR/Centos7/x86_64/Base"
Centos6Epel="$DIR/Centos6/x86_64/Epel"
Centos7Epel="$DIR/Centos7/x86_64/Epel"
Centos6Salt="$DIR/Centos6/x86_64/Salt"
Centos7Salt="$DIR/Centos7/x86_64/Salt"
Centos6Update="$DIR/Centos6/x86_64/Update"
Centos7Update="$DIR/Centos7/x86_64/Update"
Centos6Docker="$DIR/Centos6/x86_64/Docker"
Centos7Docker="$DIR/Centos7/x86_64/Docker"
Centos6Mysql5_7="$DIR/Centos6/x86_64/Mysql/Mysql5.7"
Centos7Mysql5_7="$DIR/Centos7/x86_64/Mysql/Mysql5.7"
Centos6Mysql8_0="$DIR/Centos6/x86_64/Mysql/Mysql8.0"
Centos7Mysql8_0="$DIR/Centos7/x86_64/Mysql/Mysql8.0"
MirrorDomain="rsync://rsync.mirrors.ustc.edu.cn"
# 目录不存在就创建
check_dir(){
    for dir in $*
    do
        test -d $dir || mkdir -p $dir
    done
}
# 检查rsync同步结果
check_rsync_status(){
    if [ $? -eq 0 ];then
        echo "rsync success" >> $1
    else
        echo "rsync fail" >> $1
    fi
}
check_dir $DIR $LogDir $Centos6Base $Centos7Base $Centos6Epel $Centos7Epel $Centos6Salt $Centos7Salt $Centos6Update $Centos7Update $Centos6Docker $Centos7Docker $Centos6Mysql5_7 $Centos7Mysql5_7 $Centos6Mysql8_0 $Centos7Mysql8_0
# Base yumrepo
#$RsyncCommand "$MirrorDomain"/repo/centos/6/os/x86_64/ $Centos6Base >> "$LogDir/centos6Base.log" 2>&1
# check_rsync_status "$LogDir/centos6Base.log"
$RsyncCommand "$MirrorDomain"/repo/centos/7/os/x86_64/ $Centos7Base >> "$LogDir/centos7Base.log" 2>&1
check_rsync_status "$LogDir/centos7Base.log"
# Epel yumrepo
# $RsyncCommand "$MirrorDomain"/repo/epel/6/x86_64/ $Centos6Epel >> "$LogDir/centos6Epel.log" 2>&1
# check_rsync_status "$LogDir/centos6Epel.log"
$RsyncCommand "$MirrorDomain"/repo/epel/7/x86_64/ $Centos7Epel >> "$LogDir/centos7Epel.log" 2>&1
check_rsync_status "$LogDir/centos7Epel.log"
# SaltStack yumrepo
# $RsyncCommand "$MirrorDomain"/repo/salt/yum/redhat/6/x86_64/ $Centos6Salt >> "$LogDir/centos6Salt.log" 2>&1
# ln -s $Centos6Salt/archive/$(ls $Centos6Salt/archive | tail -1) $Centos6Salt/latest
# check_rsync_status "$LogDir/centos6Salt.log"
$RsyncComman "$MirrorDomain"/repo/salt/yum/redhat/7/x86_64/ $Centos7Salt >> "$LogDir/centos7Salt.log" 2>&1
check_rsync_status "$LogDir/centos7Salt.log"
# ln -s $Centos7Salt/archive/$(ls $Centos7Salt/archive | tail -1) $Centos7Salt/latest
# Docker yumrepo
$RsyncCommand "$MirrorDomain"/repo/docker-ce/linux/centos/7/x86_64/stable/ $Centos7Docker >> "$LogDir/centos7Docker.log" 2>&1
check_rsync_status "$LogDir/centos7Docker.log"
# centos update yumrepo
# $RsyncCommand "$MirrorDomain"/repo/centos/6/updates/x86_64/ $Centos6Update >> "$LogDir/centos6Update.log" 2>&1
# check_rsync_status "$LogDir/centos6Update.log"
$RsyncCommand "$MirrorDomain"/repo/centos/7/updates/x86_64/ $Centos7Update >> "$LogDir/centos7Update.log" 2>&1
check_rsync_status "$LogDir/centos7Update.log"
# mysql 5.7 yumrepo
# $RsyncCommand "$MirrorDomain"/repo/mysql-repo/yum/mysql-5.7-community/el/6/x86_64/ "$Centos6Mysql5_7" >> "$LogDir/centos6Mysql5.7.log" 2>&1
# check_rsync_status "$LogDir/centos6Mysql5.7.log"
$RsyncCommand "$MirrorDomain"/repo/mysql-repo/yum/mysql-5.7-community/el/7/x86_64/ "$Centos7Mysql5_7" >> "$LogDir/centos7Mysql5.7.log" 2>&1
check_rsync_status "$LogDir/centos7Mysql5.7.log"
# mysql 8.0 yumrepo
# $RsyncCommand "$MirrorDomain"/repo/mysql-repo/yum/mysql-8.0-community/el/6/x86_64/ "$Centos6Mysql8_0" >> "$LogDir/centos6Mysql8.0.log" 2>&1
# check_rsync_status "$LogDir/centos6Mysql8.0.log"
$RsyncCommand "$MirrorDomain"/repo/mysql-repo/yum/mysql-8.0-community/el/7/x86_64/ "$Centos7Mysql8_0" >> "$LogDir/centos7Mysql8.0.log" 2>&1
check_rsync_status "$LogDir/centos7Mysql8.0.log"
```
<a name="erUrR"></a>
### 负载高时，查出占用比较高的进程脚本并存储或推送通知
```bash
#!/bin/bash

# 物理cpu个数
physical_cpu_count=$(egrep 'physical id' /proc/cpuinfo | sort | uniq | wc -l)
# 单个物理cpu核数
physical_cpu_cores=$(egrep 'cpu cores' /proc/cpuinfo | uniq | awk '{print $NF}')
# 总核数
total_cpu_cores=$((physical_cpu_count*physical_cpu_cores))

# 分别是一分钟、五分钟、十五分钟负载的阈值，其中有一项超过阈值才会触发
one_min_load_threshold="$total_cpu_cores"
five_min_load_threshold=$(awk 'BEGIN {print '"$total_cpu_cores"' * "0.8"}')
fifteen_min_load_threshold=$(awk 'BEGIN {print '"$total_cpu_cores"' * "0.7"}')

# 分别是分钟、五分钟、十五分钟负载平均值
one_min_load=$(uptime | awk '{print $(NF-2)}' | tr -d ',')
five_min_load=$(uptime | awk '{print $(NF-1)}' | tr -d ',')
fifteen_min_load=$(uptime | awk '{print $NF}' | tr -d ',')

# 获取当前cpu 内存 磁盘io信息，并写入日志文件
# 如果需要发送消息或者调用其他，请自行编写函数即可
get_info(){
log_dir="cpu_high_script_log"
test -d "$log_dir" || mkdir "$log_dir"
ps -eo user,pid,%cpu,stat,time,command --sort -%cpu | head -10 > "$log_dir"/cpu_top10.log
ps -eo user,pid,%mem,rss,vsz,stat,time,command --sort -%mem | head -10 > "$log_dir"/mem_top10.log
iostat -dx 1 10 > "$log_dir"/disk_io_10.log
}


export -f get_info

echo "$one_min_load $one_min_load_threshold $five_min_load $five_min_load_threshold $fifteen_min_load $fifteen_min_load_threshold" | \
awk '{ if ($1>=$2 || $3>=$4 || $5>=$6) system("get_info") }'
```
