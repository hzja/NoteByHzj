## 快速启动

~~~ tex
win+R：
cmd:打开命令提示符
compmgmt.msc，打开Windows自带的计算机管理器
lusrmgr.msc（本机用户和用户）：用于设置用户的权限等信息。
devmgmt.msc（设备管理）：计算机的硬件设备以及安装的驱动管理。
diskmgmt.msc（磁盘管理）：可以看到磁盘的分区等信息，也可以自己开辟分区。
services.msc（本地服务设置）：有些程序只有开启了相关的服务才能运行，就在这里设置。
perfomon.msc（性能监视器）：性能监视器，作用不大。
eventvmr（事件查看器）：查看计算机最近执行的事件，如果蓝屏，可以重启后打开这个看是由于什么事件引起的。
regedit.exe 注册表
shutdown /s /t 30倒计时关机命令
shutdown -a 取消关机命令
calc 启动计算器
~~~



## CMD常见命令

![d7dc3666e0898f25162280d51a522c7b](.\photo\d7dc3666e0898f25162280d51a522c7b.jpg)

1. ipconfig用于查看本机的IP信息

   + ipconfig/? 回车，了解ipconfig 命令的基本用法。
   + ipconfig 查看本机IP（在DOS窗体的情况下输入）
   + ipconfig /all 查看本机IP（详细信息）
   + ipconfig/displaydns 显示DNS解析程序缓存的内容	

   

2. `tracert`是路由跟踪实用程序，用于确定 IP 数据包访问目标所采取的路径。

   + `tracert/?` 回车，了解tracert命令的基本用法。

   

3. `pathping`提供有关在源和目标之间的中间跃点处网络滞后和网络丢失的信息。

   + `pathping/?`回车，了解pathping命令的基本用法。

   

4. Netstat是在内核中访问网络连接状态及其相关信息的程序，它能提供TCP连接，TCP和UDP监听，进程内存管理的相关报告。是一个监控TCP/IP网络的非常有用的命令，它可以显示路由表、实际的网络连接以及每一个网络接口设备的状态信息。

   + `netstat/?`回车，了解netstat命令的基本用法。

   

5. arp命令的使用
   arp命令显示并修改Internet到以太网的地址转换表。这个表一般由地址转换协议（ARP）来维护。当只有一个主机名作为参数时，arp显示这个主机的当前ARP条目。如果这个主机不在当前ARP表中那么ARP就会显示一条说明信息。

   + arp/?回车，了解arp命令的基本用法。

     

   + `nslookup/?`回车，了解nslookup命令的基本用法。



6. net命令的使用
   NET命令是功能强大的以命令行方式执行的工具。它包含了管理网络环境、服务、用户、登陆等大部分重要的管理功能。使用它可以轻松的管理本地或者远程计算机的网络环境，以及各种服务程序的运行和配置。或者进行用户管理和登陆管理等。
   + net/?回车，了解net命令的基本用法。
   + net start mysql 控制台启动mysql服务
   + net stop mysql 控制台停止mysql服务



7. route命令的使用

​		route命令用于显示和操作IP路由表。要实现两个不同的子网之间的通信，需要一台连接两个网络的路由器，或者同时位于两个网络		的网关来实现

+ `route/?`回车，了解route命令的基本用法。



8. tomcat安装目录下进入cmd

+ `service.bat install` 安装tomcat服务

- `service.bat remove` 服务名 卸载tomcat服务



## CMD端口查看

~~~ tex
netstat -ano，列出所有端口的情况

查看被占用端口（比如说8080）对应的PID，输入命令：netstat -aon|findstr 8080，回车，记下最后一位数字，即PID，这里是10644。

继续输入tasklist|findstr 10644回车，查看是哪个进程或者程序占用了10644端口，结果是：java.exe。或者是我们打开任务管理器，切换到进程选项卡，在PID一列查看2720对应的进程是谁。

taskkill /f /t /im java.exe: 结束该进程

netstat -ano(!!参数分开打也可以,比如netstat -o就会显示出进程ID)：
显示协议统计信息和当前 TCP/IP 网络连接。

NETSTAT [-a] [-b] [-e] [-n] [-o] [-p proto] [-r] [-s] [-v] [interval]
-a 显示所有连接和监听端口。
-b 显示包含于创建每个连接或监听端口的可执行组件。在某些情况下已知可执行组件拥有多个独立组件，并且在这些情况下包含于创建连接或监听端口的组件序列被显示。这种情况下，可执行组件名在底部的 [] 中，顶部是其调用的组件，等等，直到 TCP/IP 部分。注意此选项可能需要很长时间，如果没有足够权限可能失败。
-e 显示以太网统计信息。此选项可以与 -s
选项组合使用。
-n 以数字形式显示地址和端口号。
-o 显示与每个连接相关的所属进程 ID。
-p proto 显示 proto 指定的协议的连接；proto 可以是下列协议之一: TCP、UDP、TCPv6 或 UDPv6。
如果与 -s 选项一起使用以显示按协议统计信息，proto 可以是下列协议之一:
IP、IPv6、ICMP、ICMPv6、TCP、TCPv6、UDP 或 UDPv6。
-r 显示路由表。
-s 显示按协议统计信息。默认地，显示 IP、IPv6、ICMP、ICMPv6、TCP、TCPv6、UDP 和 UDPv6 的统计信息；
-p 选项用于指定默认情况的子集。
-v 与 -b 选项一起使用时将显示包含于为所有可执行组件创建连接或监听端口的组件。
interval 重新显示选定统计信息，每次显示之间暂停时间间隔(以秒计)。按 CTRL+C 停止重新显示统计信息。如果省略，netstat 显示当前配置信息(只显示一次)
~~~



## CMD命令大全

~~~ tex
pedit.msc-----组策略

sndrec32-------录音机

Nslookup-------IP地址侦测器

explorer-------打开资源管理器

logoff---------注销命令

tsshutdn-------60秒倒计时关机命令

lusrmgr.msc----本机用户和组

services.msc—本地服务设置

oobe/msoobe /a----检查XP是否激活

notepad--------打开记事本

cleanmgr-------垃圾整理

net start messenger----开始信使服务

compmgmt.msc—计算机管理

net stop messenger-----停止信使服务

conf-----------启动netmeeting

dvdplay--------DVD播放器

charmap--------启动字符映射表

diskmgmt.msc—磁盘管理实用程序

calc-----------启动计算器

dfrg.msc-------磁盘碎片整理程序

chkdsk.exe-----Chkdsk磁盘检查

devmgmt.msc— 设备管理器

regsvr32 /u *.dll----停止dll文件运行

drwtsn32------ 系统医生

rononce -p ----15秒关机

dxdiag---------检查DirectX信息

regedt32-------注册表编辑器

Msconfig.exe—系统配置实用程序

rsop.msc-------组策略结果集

mem.exe--------显示内存使用情况

regedit.exe----注册表

winchat--------XP自带局域网聊天

progman--------程序管理器

winmsd---------系统信息

perfmon.msc----计算机性能监测程序

winver---------检查Windows版本

sfc /scannow-----扫描错误并复原

winipcfg-------IP配置

taskmgr-----任务管理器（2000／xp／2003）

command--------cmd

fsmgmt.msc 共享文件夹

netstat -an----查看端口

osk 屏幕键盘

install.asp----修改注册网页

eventvwr.msc 时间查看器

secpol.msc 本地安全设置

services.msc 服务

2K

accwiz.exe > 辅助工具向导

acsetups.exe > acs setup dcom server executable

actmovie.exe > 直接显示安装工具

append.exe > 允许程序打开制定目录中的数据

arp.exe > 显示和更改计算机的ip与硬件物理地址的对应列表

at.exe > 计划运行任务

atmadm.exe > 调用管理器统计

attrib.exe > 显示和更改文件和文件夹属性

autochk.exe > 检测修复文件系统

autoconv.exe > 在启动过程中自动转化系统

autofmt.exe > 在启动过程中格式化进程

autolfn.exe > 使用长文件名格式

bootok.exe > boot acceptance application for registry

bootvrfy.exe > 通报启动成功

cacls.exe > 显示和编辑acl

calc.exe > 计算器

cdplayer.exe > cd播放器

change.exe > 与终端服务器相关的查询

charmap.exe > 字符映射表

chglogon.exe > 启动或停用会话记录

chgport.exe > 改变端口（终端服务）

chgusr.exe > 改变用户（终端服务）

chkdsk.exe > 磁盘检测程序

chkntfs.exe > 磁盘检测程序

cidaemon.exe > 组成ci文档服务

cipher.exe > 在ntfs上显示或改变加密的文件或目录

cisvc.exe > 索引内容

ckcnv.exe > 变换cookie

cleanmgr.exe > 磁盘清理

cliconfg.exe > sql客户网络工具

clipbrd.exe > 剪贴簿查看器

clipsrv.exe > 运行clipboard服务

clspack.exe > 建立系统文件列表清单

cluster.exe > 显示域的集群

cmd.exe > 没什么好说的！

cmdl32.exe > 自动下载连接管理

cmmgr32.exe > 连接管理器

cmmon32.exe > 连接管理器监视

cmstp.exe > 连接管理器配置文件安装程序

comclust.exe > 集群

comp.exe > 比较两个文件和文件集的内容＊

compact.exe > 显示或改变ntfs分区上文件的压缩状态

conime.exe > ime控制台

control.exe > 控制面板

convert.exe > 转换文件系统到ntfs

convlog.exe > 转换iis曰志文件格式到ncsa格式

cprofile.exe > 转换显示模式

cscript.exe > 较本宿主版本

csrss.exe > 客户服务器runtime进程

csvde.exe > 曰至格式转换程序

dbgtrace.exe > 和terminal server相关

dcomcnfg.exe > dcom配置属性

dcphelp.exe >

dcpromo.exe > ad安装向导

ddeshare.exe > dde共享

ddmprxy.exe >

debug.exe > 就是debug啦！

dfrgfat.exe > fat分区磁盘碎片整理程序

dfrgntfs.exe > ntfs分区磁盘碎片整理程序

dfs_cmd_.exe > 配置一个dfs树

dfsinit.exe > 分布式文件系统初始化

dfssvc.exe > 分布式文件系统服务器

diantz.exe > 制作cab文件

diskperf.exe > 磁盘性能计数器

dllhost.exe > 所有com 应用软件的主进程

dllhst3g.exe >

dmadmin.exe > 磁盘管理服务

dmremote.exe > 磁盘管理服务的一部分

dns.exe > dns applications dns

doskey.exe > 命令行创建宏

dosx.exe > dos扩展

dplaysvr.exe > 直接运行帮助

drwatson.exe > 华生医生错误检测

drwtsn32.exe > 华生医生显示和配置管理

dtcsetup.exe > installs mdtc

dvdplay.exe > dvd播放

dxdiag.exe > direct-x诊断工具

edlin.exe > 命令行的文本编辑器（历史悠久啊！）

edlin.exe > 命令行的文本编辑器（历史悠久啊！）

esentutl.exe > ms数据库工具

eudcedit.exe > type造字程序

eventvwr.exe > 事件查看器

evnt_cmd_.exe > event to trap translator; configuration tool

evntwin.exe > event to trap translator setup

exe2bin.exe > 转换exe文件到二进制

expand.exe > 解压缩

extrac32.exe > 解cab工具

fastopen.exe > 快速访问在内存中的硬盘文件

faxcover.exe > 传真封面编辑

faxqueue.exe > 显示传真队列

faxsend.exe > 发送传真向导

faxsvc.exe > 启动传真服务

fc.exe > 比较两个文件的不同

find.exe > 查找文件中的文本行

findstr.exe > 查找文件中的行

finger.exe > 一个用户并显示出统计结果

fixmapi.exe > 修复mapi文件

flattemp.exe > 允许或者禁用临时文件目录

fontview.exe > 显示字体文件中的字体

forcedos.exe > forces a file to start in dos mode. 强制文件在dos模式下运行

freecell.exe > popular windows game 空当接龙

ftp.exe > file transfer protocol used to transfer files over a network conne

ction 就是ftp了

gdi.exe > graphic device interface 图形界面驱动

grovel.exe >

grpconv.exe > program manager group convertor 转换程序管理员组

help.exe > displays help for windows 2000 commands 显示帮助

hostname.exe > display hostname for machine. 显示机器的hostname

ie4uinit.exe > ie5 user install tool ie5用户安装工具

ieshwiz.exe > customize folder wizard 自定义文件夹向导

iexpress.exe > create and setup packages for install 穿件安装包

iisreset.exe > restart iis admin service 重启iis服务

internat.exe > keyboard language indicator applet 键盘语言指示器

ipconfig.exe > windows 2000 ip configuration. 察看ip配置

ipsecmon.exe > ip security monitor ip安全监视器

ipxroute.exe > ipx routing and source routing control program ipx路由和源路由

控制程序

irftp.exe > setup ftp for wireless communication 无线连接

ismserv.exe > intersite messaging service 安装或者删除service control manage

r中的服务

jdbgmgr.exe > microsoft debugger for java 4 java4的调试器

jetconv.exe > convert a jet engine database 转换jet engine数据库

jetpack.exe > compact jet database. 压缩jet数据库

jview.exe > command-line loader for java java的命令行装载者

krnl386.exe > core component for windows 2000 2000的核心组件

label.exe > change label for drives 改变驱动器的卷标

lcwiz.exe > license compliance wizard for local or remote systems. 许可证符合

向导

ldifde.exe > ldif cmd line manager ldif目录交换命令行管理

licmgr.exe > terminal server license manager 终端服务许可协议管理

lights.exe > display connection status lights 显示连接状况

llsmgr.exe > windows 2000 license manager 2000许可协议管理

llssrv.exe > start the license server 启动许可协议服务器

lnkstub.exe >

locator.exe > rpc locator 远程定位

lodctr.exe > load perfmon counters 调用性能计数

logoff.exe > log current user off. 注销用户

lpq.exe > displays status of a remote lpd queue 显示远端的lpd打印队列的状态，

显示被送到基于unix的服务器的打印任务

lpr.exe > send a print job to a network printer. 重定向打印任务到网络中的打印 机。通常用于unix客户打印机将打印任务发送给连接了打印设备的nt的打印机服务器。

lsass.exe > lsa executable and server dll 运行lsa和server的dll

lserver.exe > specifies the new dns domain for the default server 指定默认se

rver新的dns域

os2.exe > an os/2 warp server (os2 /o) os/2

os2srv.exe > an os/2 warp server os/2

os2ss.exe > an os/2 warp server os/2

osk.exe > on screen keyboard 屏幕键盘

packager.exe > windows 2000 packager manager 对象包装程序

pathping.exe > combination of ping and tracert 包含ping和tracert的程序

pax.exe > is a posix program and path names used as arguments must be specif

ied in posix format. use ‘//c/users/default’ instead of ‘c:usersdefault.’

启动便携式存档互换 (pax) 实用程序

Pent nt.exe > used to check the pentium for the floating point division error

. 检查pentium的浮点错误

perfmon.exe > starts windows performance monitor 性能监视器

ping.exe > packet internet groper 验证与远程计算机的连接

posix.exe > used for backward compatibility with unix 用于兼容unix

print.exe > cmd line used to print files 打印文本文件或显示打印队列的内容。

progman.exe > program manager 程序管理器

proquota.exe > profile quota program

psxss.exe > posix subsystem application posix子系统应用程序

qappsrv.exe > displays the available application terminal servers on the net

work

在网络上显示终端服务器可用的程序

qprocess.exe > display information about processes local or remote 在本地或远

程显示进程的信息（需终端服务）

query.exe > query termserver user process and sessions 查询进程和对话

quser.exe > display information about a user logged on 显示用户登陆的信息（需

终端服务）

qwinsta.exe > display information about terminal sessions. 显示终端服务的信息

rasadmin.exe > start the remote access admin service 启动远程访问服务

rasautou.exe > creates a ras connection 建立一个ras连接

rasdial.exe > dial a connection 拨号连接

ras.exe > starts a ras connection 运行ras连接

rcp.exe > copies a file from and to a rcp service. 在 windows 2000 计算机和运

行远程外壳端口监控程序 rshd 的系统之间复制文件

rdpclip.exe > rdpclip allows you to copy and paste files between a terminal

session and client console session. 再终端和本地复制和粘贴文件

recover.exe > recovers readable information from a bad or defective disk 从坏

的或有缺陷的磁盘中恢复可读取的信息。

redir.exe > starts the redirector service 运行重定向服务

regedt32.exe > 32-bit register service 32位注册服务

regini.exe > modify registry permissions from within a script 用脚本修改注册

许可

register.exe > register a program so it can have special execution character

istics. 注册包含特殊运行字符的程序

regsvc.exe >

regsvr32.exe > registers and unregister’s dll’s. as to how and where it regi

ster’s them i dont know. 注册和反注册dll

regtrace.exe > options to tune debug options for applications failing to dum

p trace statements

trace 设置

regwiz.exe > registration wizard 注册向导

remrras.exe >

replace.exe > replace files 用源目录中的同名文件替换目标目录中的文件。

reset.exe > reset an active section 重置活动部分

rexec.exe > runs commands on remote hosts running the rexec service. 在运行

rexec 服务的远程计算机上运行命令。rexec 命令在执行指定命令前，验证远程计算机

上的用户名，只有安装了 tcp/ip 协议后才可以使用该命令。

risetup.exe > starts the remote installation service wizard. 运行远程安装向导

服务

route.exe > display or edit the current routing tables. 控制网络路由表

routemon.exe > no longer supported 不再支持了！

router.exe > router software that runs either on a dedicated dos or on an os

. 检查pentium的浮点错误

perfmon.exe > starts windows performance monitor 性能监视器

ping.exe > packet internet groper 验证与远程计算机的连接

posix.exe > used for backward compatibility with unix 用于兼容unix

print.exe > cmd line used to print files 打印文本文件或显示打印队列的内容。

progman.exe > program manager 程序管理器

proquota.exe > profile quota program

psxss.exe > posix subsystem application posix子系统应用程序

qappsrv.exe > displays the available application terminal servers on the net

work

在网络上显示终端服务器可用的程序

qprocess.exe > display information about processes local or remote 在本地或远

程显示进程的信息（需终端服务）

query.exe > query termserver user process and sessions 查询进程和对话

quser.exe > display information about a user logged on 显示用户登陆的信息（需

终端服务）

qwinsta.exe > display information about terminal sessions. 显示终端服务的信息

rasadmin.exe > start the remote access admin service 启动远程访问服务

rasautou.exe > creates a ras connection 建立一个ras连接

rasdial.exe > dial a connection 拨号连接

ras.exe > starts a ras connection 运行ras连接

rcp.exe > copies a file from and to a rcp service. 在 windows 2000 计算机和运

行远程外壳端口监控程序 rshd 的系统之间复制文件

rdpclip.exe > rdpclip allows you to copy and paste files between a terminal

session and client console session. 再终端和本地复制和粘贴文件

recover.exe > recovers readable information from a bad or defective disk 从坏

的或有缺陷的磁盘中恢复可读取的信息。

redir.exe > starts the redirector service 运行重定向服务

regedt32.exe > 32-bit register service 32位注册服务

regini.exe > modify registry permissions from within a script 用脚本修改注册

许可

register.exe > register a program so it can have special execution character

istics. 注册包含特殊运行字符的程序

regsvc.exe >

regsvr32.exe > registers and unregister’s dll’s. as to how and where it regi

ster’s them i dont know. 注册和反注册dll

regtrace.exe > options to tune debug options for applications failing to dum

p trace statements

trace 设置

regwiz.exe > registration wizard 注册向导

remrras.exe >

replace.exe > replace files 用源目录中的同名文件替换目标目录中的文件。

reset.exe > reset an active section 重置活动部分

rexec.exe > runs commands on remote hosts running the rexec service. 在运行

rexec 服务的远程计算机上运行命令。rexec 命令在执行指定命令前，验证远程计算机

上的用户名，只有安装了 tcp/ip 协议后才可以使用该命令。

risetup.exe > starts the remote installation service wizard. 运行远程安装向导

服务

route.exe > display or edit the current routing tables. 控制网络路由表

routemon.exe > no longer supported 不再支持了！

router.exe > router software that runs either on a dedicated dos or on an os

5 开始→运行（cmd）命令大全（绝对经典）

/2 system. route软件在 dos或者是os/2系统

rsh.exe > runs commands on remote hosts running the rsh service 在运行 rsh 服

务的远程计算机上运行命令

rsm.exe > mounts and configures remote system media 配置远程系统媒体

rsnotify.exe > remote storage notification recall 远程存储通知回显

rsvp.exe > resource reservation protocol 源预约协议

runas.exe > run a program as another user 允许用户用其他权限运行指定的工具和

程序

rundll32.exe > launches a 32-bit dll program 启动32位dll程序

runonce.exe > causes a program to run during startup 运行程序再开始菜单中

rwinsta.exe > reset the session subsystem hardware and software to known ini

tial values 重置会话子系统硬件和软件到最初的值

savedump.exe > does not write to e:winntuser.dmp 不写入user.dmp中

scardsvr.exe > smart card resource management server 子能卡资源管理服务器

schupgr.exe > it will read the schema update files (.ldf files) and upgrade

the schema. (part of adsi) 读取计划更新文件和更新计划

secedit.exe > starts security editor help 自动安全性配置管理

services.exe > controls all the services 控制所有服务

sethc.exe > set high contrast - changes colours and display mode logoff to s

et it back to normal 设置高对比

setreg.exe > shows the software publishing state key values 显示软件发布的国

家语言

setup.exe > gui box prompts you to goto control panel to configure system co

mponents 安装程序（转到控制面板）

setver.exe > set version for files 设置 ms-dos 子系统向程序报告的 ms-dos 版本

号

sfc.exe > system file checker test and check system files for integrity 系统

文件检查

sfmprint.exe > print services for macintosh 打印macintosh服务

sfmpsexe.exe >

sfmsvc.exe >

shadow.exe > monitor another terminal services session. 监控另外一台中端服务

器会话

share.exe > windows 2000 和 ms-dos 子系统不使用该命令。接受该命令只是为了与

ms-dos 文件兼容

shmgrate.exe >

shrpubw.exe > create and share folders 建立和共享文件夹

sigverif.exe > file signature verification 文件签名验证

skeys.exe > serial keys utility 序列号制作工具

smlogsvc.exe > performance logs and alerts 性能曰志和警报

smss.exe >

sndrec32.exe > starts the windows sound recorder 录音机

sndvol32.exe > display the current volume information 显示声音控制信息

snmp.exe > simple network management protocol used for network mangement 简单

网络管理协议

snmptrap.exe > utility used with snmp snmp工具

sol.exe > windows solitaire game 纸牌
~~~

