Shell
<a name="ZPDTj"></a>
### 1、显示系统一些基本信息
显示信息如下：

- 系统版本
- 系统内核
- 虚拟平台
- 主机名
- ip地址
- 开机信息有没有报错，有的话输出到屏幕

可以将该脚本加入到开机自启动里面，这样开机就会输出基本信息
```shell
#!/bin/bash
info(){
system=$(hostnamectl | grep System | awk '{print $3}')
kernel_release=$(hostnamectl | grep Kernel | awk -F : '{print $2}')
Virtualization=$(hostnamectl | grep Virtualization | awk '{print $2}')
server_name=$(hostname)
ipaddr=$(hostname -I)
echo "当前系统版本是：${system}"
echo "当前系统内核是：${kernel_release}"
echo "当前虚拟平台是：${Virtualization}"
echo "当前主机名是：${server_name}"
echo "当前ip地址：${ipaddr}"
}

checkerrror(){
error_info=$(dmesg | grep error)
if [ -e ${error_info} ]
then
  echo "无错误日志！"
else
  ehcho ${error_info}
fi
}
info
checkerrror
```
<a name="xmF6b"></a>
#### ![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668473493926-3864e83d-7342-49ec-b6bf-8937c7ec5f30.png#averageHue=%232f2e3a&clientId=ud3a0c3d7-3f5a-4&from=paste&id=u270604fb&originHeight=140&originWidth=622&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7b0730f9-7bb7-4571-8902-dc9b719d53a&title=)
<a name="FubNI"></a>
### 2、关闭系统防火墙和SELinux
检查防火墙状态，是否安装防火墙，如果安装则关闭<br />关闭SELinux<br />清空iptables规则
```shell
#!/bin/bash
close_firewalld(){
code=$(systemctl status firewalld)

if [ ${code} -eq 0 ]
then
  systemctl stop firewalld
fi
}

close_selinux(){
sed -i  '/^SELINUX/s/=.*/=disabled/' /etc/selinux/config 
setenforce 0
}

close_iptables(){
iptables -F
service iptables  save
service iptables  restart
}

close_firewalld
close_selinux
close_iptables
```
<a name="kITur"></a>
### 3、定时任务计划：归档备份
打包压缩/var/log/nginx目录下所有内容，存放在/tmp/nginx目录里<br />压缩文件命名规范：yymmdd_logs.tar.gz，只保存七天内的文件，超过七天的文件会进行清理
```shell
#!bin/bash
date="$(date +%Y%m%d)"
dir='/tmp/nginx'
backupfile='yymmdd_logs.tar.gz'

#查看/tmp/nginx是否存在，不存在则创建
checkbak(){
if [ ! -e ${dir} ]
then
  mkdir ${dir}
fi
}

#压缩文件
backup(){
tar -zcvf ${dir}/${backupfile} /var/log/nginx/ > /dev/null 2>&1
echo "${backupfile} Compressed and packaged successfully ！"
}

#清除七天过期文件
cleanup(){
find ${dir} -type f -mtime +7 | xagrs rm -rf
if [ $? -eq 0 ]
then
  echo "Cleaned up successfully！"
else
  echo "data cleaning failed error, please pay attention in time"
fi
}

checkbak
backup
cleanup
#!bin/bash
date="$(date +%Y%m%d)"
dir='/tmp/nginx'
backupfile='yymmdd_logs.tar.gz'

#查看/tmp/nginx是否存在，不存在则创建
checkbak(){
if [ ! -e ${dir} ]
then
  mkdir ${dir}
fi
}

#压缩文件
backup(){
tar -zcvf ${dir}/${backupfile} /var/log/nginx/ > /dev/null 2>&1
echo "${backupfile} Compressed and packaged successfully ！"
}

#清除七天过期文件
cleanup(){
find ${dir} -type f -mtime +7 | xagrs rm -rf
if [ $? -eq 0 ]
then
  echo "Cleaned up successfully！"
else
  echo "data cleaning failed error, please pay attention in time"
fi
}

checkbak
backup
cleanup
```
<a name="rkDYl"></a>
### 4、自动批量创建用户
批量创建user1、user2、user3…..
```shell
#!/bin/bash
#检查用户是否存在，不存在则创建
checkuser(){
for i in $(seq 1 20)
do
id user${i} > /dev/null 2>&1
if [ $? -eq 0 ]
then
  echo "user${i} 已存在！"
else
  useradd user${i} && echo "user${i}" | passwd --stdin user${i} > /dev/null 2>&1
fi
done
}
checkuser
```
<a name="jztar"></a>
### 5、通过位置参数创建用户
`$1`是执行脚本的第一个参数<br />`$2`是执行脚本的第二个参数
```shell
#!/bin/bash  
checkuser(){
id ${1} > /dev/null 2>&1
if [ $? -eq 0 ]
then
  echo "${1} 已存在！"
else
  useradd "$1"   
  echo "$2"  |  passwd  ‐‐stdin  "$1"  
fi
}
```
<a name="pnPN7"></a>
### 6、批量删除用户
批量删除user1…user20
```shell
#!/bin/bash
#检查用户是否存在，存在则删除
checkuser(){
for i in $(seq 1 20)
do
id user${i} > /dev/null 2>&1
if [ $? -eq 0 ]
then
  userdel -r user${i}
else
  echo "user${i} 不存在！"
fi
done
}
checkuser
```
<a name="n7FQD"></a>
### 7、更新系统时间，并写入硬件时间里

- 查看是否安装ntpdate工具
- 创建上海时区文件的软链接
- 更新时间并写入到硬件时间里
```shell
#!/bin/bash
package="ntpdate"
info=$(rpm -q ${package})
check_pkgs(){
if [ ! -e ${info} ]
then
  echo "ntpdate already exists！"
else
  echo "start installation！"
  yum clean all > /dev/null 2>&1
fi
  yum update -y && yum install -y ${package} > /dev/null 2>&1
fi
}

modify_time(){
  echo "开始修改时间"
  rm -rf /etc/localtime && ln -s /usr/share/zoneinfo/Asia/Shanghai /etc/localtime
  /usr/sbin/ntpdate cn.pool.ntp.org > /dev/null 2>&1 && hwclock -w
}

check_pkgs
modify_time
```
<a name="xb7YO"></a>
### 8、检查服务运行状态
检查某一服务是否正常运行，执行脚本的时候第一个参数为服务名
```shell
#!/bin/bash
result=$(pidof $1 | wc -l)
echo ${result}
if [ ${result} -eq 0 ]
then
  echo "service does not exist ！"
else
  echo "Service is running normally ！"
fi
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668473493938-c47801f6-25d8-4550-aa31-50a6b62f4b0d.png#averageHue=%2332313e&clientId=ud3a0c3d7-3f5a-4&from=paste&id=uddb39c92&originHeight=121&originWidth=619&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud1fa3336-efe4-4fce-9eb0-7c6d970050f&title=)
<a name="Uhm4g"></a>
### 9、对目标主机进行心跳检测
ping目标主机看是否ping得通，三次ping通表示主机正常运行<br />将目标主机的ip地址作为第一个参数传进去
```shell
#!/bin/bash
ipaddr=$1
echo ${ipaddr}
ping_status(){
if ping -c 1 ${ipaddr} > /dev/null 2>&1
then
  echo "ping ${ipaddr} is successful!"
  continue
fi
}

for i in $(seq 1 3)
do
  ping_status
  echo "ping ${ipaddr} is failure!"
done
```

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668473493999-cb3f3fc2-64c2-4e48-989a-564d79136278.png#averageHue=%2332313c&clientId=ud3a0c3d7-3f5a-4&from=paste&id=ubb5e7cbd&originHeight=214&originWidth=759&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9abee7f8-ae32-4971-aa5a-0b85abf9a6f&title=)<br />进阶版：对ip地址池里的主机分别进行心跳检测
```shell
ipaddr=(192.168.149.131 192.168.149.130 192.168.149.132 192.168.149.133)
for i in ${ipaddr[*]}
do
echo ".... begin to ping ${i} ....."
if ping -c 3 ${i} > /dev/null 2>&1
then
  echo "ping ${i} is successful!"
else
  echo "ping ${i} is failure!"
fi
done
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668473493937-9610a528-9b6c-46e8-b6ee-82eb7ac9b1d8.png#averageHue=%23363643&clientId=ud3a0c3d7-3f5a-4&from=paste&id=ubd564fed&originHeight=191&originWidth=552&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ueb4062c1-ada3-4d2c-9c1b-92f76db187e&title=)
<a name="rwFOy"></a>
### 10、系统磁盘内存容量告警
根分区剩余空间小于20%（即使用空间大于80%） 输出告警信息<br />内存使用空间大于80% 输出告警信息<br />配合crond每5分钟检查一次
```shell
#!/bin/bash
disk_letfspace=$(df -Th | grep -w / | awk '{print$6}' | cut -d % -f 1)
mem_used=$(free -m | grep Mem | awk '{print$3}')
mem_total=$(free -m | grep Mem | awk '{print$2}')
mem_letfspace=$[${mem_used}*100/${mem_total}]
if [ ${disk_letfspace} -gt 80 ]
then
  echo "Disk free space is less than 20%!"
else
  echo "${disk_letfspace}% of disk space left"
fi
if [ ${mem_letfspace} -gt 80 ]
then
  echo "memory space is less than 20%!"
else
  echo "${mem_letfspace}% of memory space left"
fi
```
```shell
crontab -l
*/5 * * * * /root/check_space.sh
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668473494028-2498e7a8-f250-40cf-9e7a-fac31409ea1d.png#averageHue=%2331303b&clientId=ud3a0c3d7-3f5a-4&from=paste&id=ubf153ebb&originHeight=73&originWidth=667&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2bb1526d-4ea7-4be5-a537-37aea2c8c40&title=)
