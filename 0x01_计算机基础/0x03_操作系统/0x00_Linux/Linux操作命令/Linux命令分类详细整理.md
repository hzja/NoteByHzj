Linux 
<a name="PMxMn"></a>
# 1、VIM操作命令

1. 插入：i、a
2. 退出：wq、:q、:q!
3. 拷贝当前行：yy，拷贝当前向下的五行，5yy，并粘贴p
4. 删除当前行，dd，删除当前向下的五行，5dd
5. 在文件中查找某个单词-命令行下：[/查找的关键字，回车查找，输入n就是查找下一个]
6. 命令行下设置文件的行号[:set nu]，取消文件的行号[:set nonu]
7. 到达文件的最首行[G]和最末行[gg]
<a name="oxJH1"></a>
# 2、Linux开机、重启和用户操作
<a name="VJXdP"></a>
## 2.1系统操作
<a name="JpRMr"></a>
### 2.1.1关机和重启
shutdown<br />shutdown -h now：立即关机<br />shutdown -h 1：1分钟后关机<br />shutdown -r now：立即重启<br />halt  直接使用的关机<br />reboot  重启系统<br />syn  把内存数据同步到磁盘（在关机重启系统之前的操作）
<a name="7EYx3"></a>
## 2.2用户操作
<a name="zfehH"></a>
### 2.2.1用户登录和注销
su ：切换操作用户<br />logout ：退出登录
<a name="IGHyM"></a>
### 2.2.2添加用户
useradd 用户名<br />useradd -d 指定目录 新的用户名 ：给新创建的用户指定根目录
<a name="F9GSH"></a>
### 2.2.3指定修改密码
passwd 用户名
<a name="KPNCw"></a>
### 2.2.4删除用户
userdel 用户名
<a name="4XNKy"></a>
### 2.2.5切换用户

1. 从高权限的用户切换到低权限的用户不需要输入密码
2. 返回原来的用户使用exit命令
<a name="JGEvw"></a>
### 2.2.6查看当前用户、登录用户
whoami/ who am i
<a name="pgIqo"></a>
## 2.3用户组操作
groupadd 组名：新增用户组<br />useradd -g 用户组 用户名：添加用户时新增组<br />usermod -g 用户组 用户名：修改用户的用户组
<a name="oIikN"></a>
## 2.4用户和组的配置文件
/etc/passwd文件<br />用户的配置文件，记录用户的各种信息<br />每行的信息：用户名：口令：用户标识号：组标识号：注释性描述：主目录：登录shell<br />/etc/shadow文件<br />口令的配置文件<br />每行的含义：登录名：加密口令：最后一次修改时间：最小时间间隔：最大时间间隔：警告时间：不活动时间：失效时间：标志<br />/etc/group文件<br />组的配置文件，记录Linux包含组的信息<br />每行的含义：组名：口令：组标识号：组内用户列表
<a name="ThtdD"></a>
# 3、实用指令
<a name="eQZ1w"></a>
## 3.1指定运行级别
<a name="R80LV"></a>
### 3.1.1运行级别说明
0：关机<br />1：单用户[找回丢失的密码]<br />2：多用户状态没有网络服务<br />3：多用户状态有网络服务<br />4：系统未使用保留给用户<br />5：图形界面<br />6：系统重启<br />常用的运行级别是3和5，要修改默认的运行级别可改文件<br />/etc/inittab的id:5:initdefault:这一行的数字
<a name="0AWPv"></a>
### 3.1.2切换到指定运行级别的指令
基本语法：init[012356]<br />应用实例：init 3
<a name="eGq8f"></a>
### 3.1.3面试题-找回root密码
分析：进入单用户模式，然后修改密码，因为进入单用户模式，root不用密码就可以登录<br />步骤：

1. 在开机引导是输入 回车键
2. 看到界面输入 e
3. 看到新的界面，选中第二行（编辑内核）再输入 e
4. 在这行最后输入 1
5. 再输入 回车 表示编辑完毕
6. 再次输入 b 进入单用户模式
7. 使用 passwd 命令修改root用户的密码
<a name="rruG4"></a>
## 3.2帮助指令

- man获取帮助信息

基本语法：man [命令或配置文件]（功能描述：获得帮助信息）

- help指令

基本语法：help 命令
<a name="z5QaW"></a>
## 3.3文件目录指令

- pwd：显示当前工作目录的绝对路径
- ls -ahl：显示当前工作目录下的文件或目录及其文件目录的所有者
- cd：切换文件目录
- mkdir：新建目录
- rmdir：删除目录
- touch file.txt：创建空文件，创建多个文件用空格隔开
- cp -r source dest：递归复制这个源文件到目标目录
- \cp -r source dest：递归复制这个源文件到目标目录，会强制覆盖
- rm -rf file.txt：强制删除且不提示
- mv oldFileName newFileName：重命名
- mv sourcePath destPath：移动文件目录
- cat -n file.txt | more：查看文件内容，只有浏览权限，-n带行号
   - more：打开的文件分页交给管道命令，用空格加载下一页
      - 空格键/enter：向下翻页
      - q：退出浏览
      - Ctrl+F：向下翻页
      - Ctrl+B：向上翻页
      - =：输出当前的行号
      - :f：输出当前的文件名和行号
   - less：分屏显示文件，支持读取大文件，根据需要加载，非一次性加载
      - 空格键/pagedown：向下翻页
      - pageup：向上翻页
      - /字串：向下搜寻字串，n：向下查找，N：向上查找
      - ？字串：向上搜寻字串，n：向下查找，N：向上查找
      - q：退出less程序
- >输出重定向（覆盖形式）和>>追加
   - ls -l > file：列表中的内容写入文件，覆盖写
   - ls -al >> file：列表的内容追加到文件的末尾
   - cat file1 > file2：将file1的内容覆盖到file2
   - echo "string" >> file
- echo：输出内容到控制台
- head -n 5 file：显示文件的开头部分，默认显示前十行
- tail -n 5 file：显示文件的最后的内容，默认显示后十行，-f表示实时追踪该文档
- ln -s [原文件或目录] [软链接名]：软链接也叫符号链接，存放其他链接其他文件的路径
- history：查看已经执行的历史指令
<a name="3T2Yq"></a>
## 3.4时间日期类

- date：显示当前日期
   - date+%Y：显示当前年份
   - date+%m：显示当前月份
   - date+%d：显示当前是那一天
   - date "+%Y-%m-%d %H:%M:%S" ：显示年月日时分秒
   - date -s 字符串时间：设置系统日期时间
- cal [year]：查看某年日历指令
<a name="Dfzar"></a>
## 3.5搜索查找类

- find：将从指定目录向下递归的遍历各个子目录进行查找
   - 基本语法：find [搜索范围] [选项]
   - find [搜索范围] -name <查询方式>：按照指定文件名查找模式查找文件
   - find [搜索范围] -user <用户名>：查找属于指定用户名所有文件
   - find [搜索范围] -size <文件大小+20M（大于20M）>：按照指定的文件大小查找指定的文件
- locate filename：快速定位文件的路径
   - 注意：第一次运行前必须使用updatedb指令创建locate数据库
- grep：过滤查找，管道符，"|"，表示将前一个命令的处理结果输出后传递给后面的命令处理
   - 基本语法：grep [选项] 查找内容 源文件
   - -n ：显示匹配行及行号
   - -i ：忽略字母大小写
   - 示例：cat hello.txt | grep -n yes
- 管道符："|"
<a name="OjK3D"></a>
## 3.6压缩和解压类指令

- gzip类压缩指令
   - gzip filename：压缩文件，只能将文件压缩为*.gz文件，源文件不保存
   - gunzip filename.gz ：解压缩命令
- zip类压缩指令
   - zip [选项] filename.zip [压缩的目录]：压缩文件和目录的命令
      - -r ：递归压缩，压缩目录
   - unzip [选项] filename.zip：解压缩文件
      - -d  <目录>：指定解压后文件的存放目录
- tar打包指令，打包后的文件是.tar.gz
   - 基本语法：tar [选项] filename.tar.gz 打包的内容 
      - -c：产生.tar打包文件
      - -v：显示详细信息
      - -f：指定压缩后的文件名
      - -z：打包同时压缩
      - -x：解包.tar文件
      - -C：指定解压的目录，解压的目标目录必须存在
         - 压缩示例：tar -zcvf a.tar.gz
         - 解压示例：tar -zxvf a.tar.gz -C /opt
<a name="a9sde"></a>
# 4、组管理和权限管理
<a name="11KJk"></a>
## 4.1组用户管理

- groupadd groupname ：添加组
- useradd username：添加用户
- username -g groupname username新建用户并添加到组
<a name="UpsZ4"></a>
### 4.1.1修改文件的所有者

-  chown username filename
<a name="I71QK"></a>
### 4.1.2修改文件所在的组

- chgrp groupname filename
<a name="frTU6"></a>
### 4.1.3改变用户所在组

- usermod -g groupname username
- usermod -d 目录名 用户名 改变用户登录的初始目录
<a name="lfhJr"></a>
## 4.2权限的管理
<a name="JbSMl"></a>
### 4.2.1权限的基本介绍 
<a name="DE45X"></a>
## ![](https://cdn.nlark.com/yuque/0/2019/png/396745/1564284975303-301ed7f8-d026-482e-8866-de0c67a2cc0c.png#height=138&id=VKaPu&originHeight=138&originWidth=1920&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=1920)

- 第一个符号表示文件的类型
   - -：普通文件
   - d：目录
   - l：软链接
   - c：字符设备，键盘，鼠标类
   - b：块文件，硬盘
- 2-4个字符表示用户对该目录或文件所拥有的权限
   - r：读
   - w：写
   - x：可执行，进入该目录
- 5-7个字符表示用户组对该文件或目录所拥有的权限
- 8-10个字符表示其他组用户对该文件或目录所拥有的权限
- 第十一个数字
   - 文件则表示硬连接数
   - 目录则表示子目录的个数
<a name="xRlL6"></a>
### 4.2.2修改权限-chmod

- +、-、=变更权限
   - u：所有者、g：所有组、o：其他人、a：所有人（u、g、o的总和）
   - chmod u=rwx，g=rx，o=x 文件目录名
   - chmod o+w 文件目录名
   - chmod a-x 文件目录名
- 通过数字变更
   - r=4、w=2、x=1  rwx=4+2+1=7
   - chmod 751 文件目录名
- 修改文件的所有者
   - chown owner file：改变文件的所有者
   - chown owner：groupname file 改变用户的所有者和所有组
   - -R：目录的递归生效
<a name="jXq29"></a>
# 5、任务调度器crond
<a name="qNwDS"></a>
## 5.1简述
概述：任务调度，是指系统在某个时间执行的特定的命令或程序<br />任务调度分类：

   1. 系统工作：系统工作需要周而复始的执行
   2. MySQL数据库的定时备份

基本语法：crontab [选项]<br />常用选项：

      - -e：编辑crontab定时任务
      - -l：查询crontab任务
      - -r：删除当前用户的所有的crontab任务
<a name="Erx9B"></a>
## 5.2快速入门
设置任务调度文件：/etc/crontab<br />设置个人任务调度，执行crontab -e命令<br />接着输入任务到调度文件：*/1**** ls -l /etc/ > /tmp/to.txt<br />每小时的每分钟执行ls -l /etc/ > /tmp/to.txt命令
<a name="bveFc"></a>
## 5.3参数细节说明
|   | 含义 | 范围 |
| --- | --- | --- |
| 第一个"*" | 一个小时当中的第几分钟 | 0-59 |
| 第二个"*" | 一天当中的第几个小时 | 0-23 |
| 第三个"*" | 一个月当中的第几天 | 1-31 |
| 第四个"*" | 一年当中的第几个月 | 1-12 |
| 第五个"*" | 一周当中的星期几 | 0-7（0和7都代表星期日） |

<a name="z3P2a"></a>
## 5.4特殊符号说明
| 特殊符号 | 含义 |
| --- | --- |
| * | 代表任何时间 |
| ， | 代表不连续的时间 |
| - | 代表范围 |
| */n | 代表每隔多久执行一次 |

<a name="zKIIG"></a>
## 5.5crond相关指令

1. `conrtab -r`：终止任务调度
2. `crontab -l`：列出当前有哪些任务调度
3. `service crond restart` [重启任务调度]
<a name="yWfri"></a>
# 6、Linux磁盘分区，挂载
<a name="RRjPC"></a>
## 6.1分区的方式
<a name="pHhP4"></a>
### 6.1.1mbr分区

1. 最多支持四个主分区
2. 系统只能安装在主分区
3. 扩展分区要占一个主分区
4. MBR最大只支持2TB，但拥有最好的兼容性
<a name="9xbo6"></a>
### 6.1.2gtp分区

1. 支持无限多个主分区（但操作系统可限制，比如Window下最多128个分区）
2. 最大支持18EB的大容量（EB=1024PB，PB=1024TB）
3. Window7 64位以后支持gtp
<a name="0D9UU"></a>
## 6.2硬盘
<a name="YlRZb"></a>
### 6.2.1 IDE硬盘

1. 驱动器标识符：hdx~
   1. hd表示分区所在设备类型
   2. x是盘号
      1. a：基本盘
      2. b：基本从属盘
      3. c：辅助主盘
      4. d：辅助从属盘
   3. ~是分区
      1. 前四个分区用数字1-4表示，为主分区或扩展分区
      2. 从5开始就是逻辑分区
<a name="agsqG"></a>
### 6.2.2 SCSI硬盘（常用）

1. 标识符：sdx~
2. 其余和IDE表示相同
<a name="m95N7"></a>
### 6.2.3查看所有设备挂载情况
命令：lsblk -f
<a name="sy51S"></a>
### 6.2.4增加硬盘

- Linux主机添加硬盘
- 分区命令：fdisk /dev/sdb
- 格式化分区：mkfs -t ext4 /dev/sdb1
- 挂载 ：先创建挂载目录 /home/disk1，再将分区和挂载目录管理：mount /dev/sdb1 /home/disk1
- 取消挂载：umount 设备名称 或 挂载目录、umount /dev/sdb1 或 umount /disk1
- 设置自动挂载（永久挂载），编辑挂载的配置文件在系统启动时自动挂载           vim /etc/fstab
- 执行mount -a永久生效
<a name="dNuEa"></a>
## 6.3磁盘情况查询
命令：df -h
<a name="kJVDt"></a>
### 6.3.1查询指定目录的磁盘占用情况

- 基本语法：du -h /目录，默认当前目录
   - -s ：指定目录占用大小汇总
   - 带计量单位
   - 含文件
   - --max-depth=1 子目录深度为1
   - -c：列出子目录的同时增加汇总值
<a name="XaTk4"></a>
### 6.3.2磁盘情况查询实例

- 统计/home文件下文件的个数
   - ls -l /home | grep "^-" | wc -l
- 统计/home文件下目录的个数
   - ls -l /home | grep "^d" | wc -l
- 统计/home文件下文件的个数，包括子文件夹
   - ls -lR /home | grep "^-" | wc -l
- 统计文件夹下目录的个数，包括子文件夹
   - ls -lR /home | grep "^d" | wc -l
- 以树状显示目录结构
   - tree
<a name="Cp7KK"></a>
# 7、网络配置
<a name="H1GO4"></a>
## 7.1网络环境配置

1. 直接修改配置文件指定IP：vi /etc/sysconfig/network-scripts/ifcfg-eth0

ONBOOT=yes                   //启用boot配置yes<br />BOOTPROTO=static             //以静态方式获取ip<br />IPADDR=192.168.184.02        //指定ip<br />GATEWAY=192.168.184.2        //网关<br />DNS1=192.168.184.2           //dns和网关保持一致

1. 修改服务后一定要重启服务：service network restart
2. 重启系统生效：reboot
<a name="jP7Xw"></a>
## 7.2修改主机名

1. 查看当前主机名：hostname
2. 修改Linux主机的映射文件：vim /etc/sysconfig/network

NETWORKING=yes<br />NETWORKING_IPV6=no<br />HOSTNAME=fcscanf       //主机名不能有下划线

1. 修改/etc/hosts增加主机和ip的映射

192.168.148.10 fcscanf

1. 重启设备生效
<a name="b8Hi7"></a>
# 8、进程管理
<a name="9zyCJ"></a>
## 8.1显示系统执行的进程
基本语法：ps

   - -a：显示当前终端的所有进程的信息
   - -u：以用户的格式显示进程的信息
   - -x：显示后台进程运行的参数
<a name="log2F"></a>
## 8.2终止进程
基本语法

   - kill [选项] 进程号（通过进程号杀死进程）
      - -9：表示强制进程立即停止
   - killall 进程名 （通过进程名称杀死进程，支持通配符，在系统负载过大时有用）
<a name="PWiCM"></a>
## 8.3查看进程树
基本语法：pstree [选项]

   - -p：显示进程的PID
   - -u：显示进程的所属用户
<a name="gEv9x"></a>
## 8.4服务管理
<a name="bi1PX"></a>
### 8.4.1service管理命令
service 服务名 [start | stop | restart | reload | status]<br />systemctl<br />启动防火墙：service iptables restart<br />查看防火墙状态：service iptables status

      - 通过服务关闭或启用防火墙立即生效。【telent IP：端口】测试端口服务
      - 这种方式只是临时生效，重启系统后回归以前的服务状态
      - 持久性的进行服务启用或关闭使用chkconfig命令
<a name="mKOXt"></a>
### 8.4.2chkconfig指令
基本语法<br />查看服务：chkconfig --list | grep 服务名<br />chkconfig 服务名 --list<br />修改服务的等级：chkconfig --level 5服务名 on/off
<a name="uCFNa"></a>
## 8.5动态监控进程
基本语法：top [选项]

   - -d秒数：指定top命令每隔几秒更新，默认3秒在top命令的交互模式当中可以执行的命令
   - -i：使top不显示任何闲置或者僵死的进程
      - -p：通过指定监控进程ID来仅仅监控某个进程的状态
- 交互操作说明
   - P：以CPU使用率排序，默认此项
   - M：以内存的使用率排序
   - N：以PID排序
   - q：退出top
<a name="b8DF5"></a>
## 8.6监控网络状态
基本语法：netstat [选项]

   - -an：按一定的顺序排列输出
   - -p：显示在调用的进程
<a name="zlZHG"></a>
## 8.7检测主机连接命令
基本语法：`ping ip`
<a name="c9biI"></a>
# 9、RPM与YUM（CentOS系统安装方式）
<a name="VVbGA"></a>
## 9.1rpm包的管理
<a name="gnUhc"></a>
### 9.1.1rpm指令
查询已安装的rpm列表

- `rpm -qa | grep applicationname`
- `rpm -qa | more`
- `rpm -qa | grep X [rpm -qa | grep firefox]`

查询软件包是否安装
```bash
rpm -q
```
查询软件包信息
```bash
rpm -qi 软件包名称
```
查询软件包中的文件
```bash
rpm -ql 软件包名称
```
查询文件所属的软件包
```bash
rpm -qf 文件全路径
```
<a name="CF3HP"></a>
### 9.1.2rpm包名的基本格式
firefox-45.0.1-1.el6.centos.x86_64.rpm<br />名称-版本号-适用的操作系统（noarch表示通用）
<a name="yfp2H"></a>
### 9.1.3rpm卸载
基本语法：rpm -e RPM包的名称

   - --nodeps 强制删除
<a name="8zmsu"></a>
### 9.1.4rpm包的安装
基本语法：rpm -ivh RPM包的全路径名称

   - i=install 安装
   - v=verbose 提示
   - h=hash 进度条
- 安装步骤
   - 先挂载CentOS的ios文件
   - 在/media/目录下找rpm包
   - 复制rpm包到目标目录
   - 切换到目标目录
   - 执行安装步骤
<a name="kudwa"></a>
## 9.2yum包安装管理
<a name="UXo4q"></a>
### 9.2.1yum包基本指令

- 查询安装的软件包列表：yum list | grep 包名称
- 安装指定的yum包：yum install 包名称
<a name="wVtGp"></a>
# 10、APT（Ubuntu系统安装应用包的方式）
<a name="qapDj"></a>
## 10.1apt指令
查询软件包的安装列表：apt list<br />安装指定的apt包：apt install 包名称
