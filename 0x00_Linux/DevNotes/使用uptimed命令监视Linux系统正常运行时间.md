Linux
<a name="kP4QS"></a>
## 安装`uptimed`工具
目前主流的 Linux 发行版的官方存软件库中已经提供了 `uptimed` 。<br />在 Arch Linux 上安装：
```bash
$ sudo pacman -S uptimed
```
在 Debian，Ubuntu，Linux Mint 上安装：
```bash
$ sudo apt-get install uptimed
```
在 Ubuntu 及其衍生版本上安装前请确保添加了 universe 存储库，没有的话请运行以下命令添加：
```bash
$ sudo add-apt-repository universe
```
在 Fedora 上安装：
```bash
$ sudo dnf install uptimed
```
在 CentOS 7 上安装：<br />因为 CentOS 7 的默认存储库中没有 `uptimed`，所以要先添加 EPEL 存储库：
```bash
$ sudo yum install epel-release
```
然后运行以下命令安装：
```bash
$ sudo yum install uptimed
```
手动安装：<br />对于其他 Linux 系统，或者上述方法安装失败的，可以从发行版页面下载源码，然后手动编译安装：
```bash
# wget https://github.com/rpodgorny/uptimed/archive/v0.4.2.zip -O uptimed.zip

# unzip uptimed.zip

# cd uptimed-0.4.2/

# ./configure

# make

# make install
```
安装完成后，启用并启动服务：
```bash
# systemctl enable uptimed

# systemctl start uptimed
```
检查服务的启动状态：
```bash
# systemctl status uptimed
● uptimed.service - Uptime record tracking daemon
   Loaded: loaded (/usr/lib/systemd/system/uptimed.service; disabled; vendor preset: disabled)
   Active: active (running) since Tue 2020-02-11 14:21:12 IST; 1s ago
     Docs: man:uptimed(8)
           man:uprecords(1)
 Main PID: 1435 (uptimed)
   Status: "Next milestone (five days) at Sun Feb 16 14:13:59 2020"
   CGroup: /system.slice/uptimed.service
           └─1435 /usr/sbin/uptimed -f

Feb 11 14:21:12 server.ostechnix.local systemd[1]: Starting Uptime record tracking daemon...
Feb 11 14:21:12 server.ostechnix.local systemd[1]: Started Uptime record tracking daemon.
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624679296144-d0846ab0-fb63-4f73-9083-0fa469f19d30.png#clientId=u22326cab-3d52-4&from=paste&height=305&id=ucab78362&originHeight=916&originWidth=3323&originalType=binary&ratio=3&size=1102918&status=done&style=none&taskId=u9fb45075-3125-4c36-a9f5-e38767740a8&width=1107.6666666666667)<br />这样，前期的安装工作就算完成了。
<a name="tgfVn"></a>
## 使用uptimed工具
安装完成 `uptimed` 工具之后，运行 `uprecords` 命令就能显示系统运行时间的历史摘要了：
```bash
# uprecords
```
输出的摘要结果：
```bash
    #               Uptime | System                                     Boot up
----------------------------+---------------------------------------------------
     1     0 days, 02:35:51 | Linux 3.10.0-1062.1.1.el  Tue Feb 11 14:22:33 2020
     2     0 days, 00:07:27 | Linux 3.10.0-1062.1.1.el  Tue Feb 11 14:13:59 2020
->   3     0 days, 00:02:11 | Linux 3.10.0-1062.1.1.el  Tue Feb 11 17:02:29 2020
----------------------------+---------------------------------------------------
1up in     0 days, 00:05:17 | at                        Tue Feb 11 17:09:56 2020
no1 in     0 days, 02:33:41 | at                        Tue Feb 11 19:38:20 2020
    up     0 days, 02:45:29 | since                     Tue Feb 11 14:13:59 2020
  down     0 days, 00:05:12 | since                     Tue Feb 11 14:13:59 2020
   %up               96.953 | since                     Tue Feb 11 14:13:59 2020
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624679313770-886aea15-fc24-484c-b018-ba6d9033da1a.png#clientId=u22326cab-3d52-4&from=paste&height=217&id=ua7c9ad50&originHeight=650&originWidth=3323&originalType=binary&ratio=3&size=733861&status=done&style=none&taskId=uc4ad6b1d-e8e7-42b0-be89-de86cc24719&width=1107.6666666666667)<br />这里的 `->` 符号表示定位到了系统最近的一次启动时间。
<a name="Gii4B"></a>
### 使用 `-b` 按启动时间正序输出：
```bash
# uprecords -b
     #               Uptime | System                                     Boot up
----------------------------+---------------------------------------------------
     1     0 days, 00:07:27 | Linux 3.10.0-1062.1.1.el  Tue Feb 11 14:13:59 2020
     2     0 days, 02:35:51 | Linux 3.10.0-1062.1.1.el  Tue Feb 11 14:22:33 2020
->   3     0 days, 00:30:14 | Linux 3.10.0-1062.1.1.el  Tue Feb 11 17:02:29 2020
```
<a name="hzGey"></a>
### 使用 `-B` 按启动时间倒序输出：
```bash
# uprecords -B
     #               Uptime | System                                     Boot up
----------------------------+---------------------------------------------------
->   1     0 days, 00:14:51 | Linux 3.10.0-1062.1.1.el  Tue Feb 11 17:02:29 2020
     2     0 days, 02:35:51 | Linux 3.10.0-1062.1.1.el  Tue Feb 11 14:22:33 2020
     3     0 days, 00:07:27 | Linux 3.10.0-1062.1.1.el  Tue Feb 11 14:13:59 2020
```
<a name="MVl3V"></a>
### 使用 `-m` 自定义输出记录数：
不加参数的话结果默认输出前 10 条记录，想要输出前 20 条记录，请运行：
```bash
# uprecords -m 20
```
<a name="gYnaS"></a>
### 使用 `-w` 输出完整信息：
上面的结果中关于 Linux 的内核信息是被省略了部分内容的，想要输出完整的内核信息请用 -w 进行宽输出：
```bash
# uprecords -w
     #               Uptime | System                                     Boot up
----------------------------+---------------------------------------------------
     1     0 days, 02:35:51 | Linux 3.10.0-1062.1.1.el7.x86_64  Tue Feb 11 14:22:33 2020
->   2     0 days, 00:16:35 | Linux 3.10.0-1062.1.1.el7.x86_64  Tue Feb 11 17:02:29 2020
     3     0 days, 00:07:27 | Linux 3.10.0-1062.1.1.el7.x86_64  Tue Feb 11 14:13:59 2020
----------------------------+---------------------------------------------------
no1 in     0 days, 02:19:17 | at                        Tue Feb 11 19:38:21 2020
    up     0 days, 02:59:53 | since                     Tue Feb 11 14:13:59 2020
  down     0 days, 00:05:12 | since                     Tue Feb 11 14:13:59 2020
   %up               97.190 | since                     Tue Feb 11 14:13:59 2020
```
<a name="wGMt4"></a>
### 使用 `-d` 显示关机时间：
`-d` 可以将上面的内核信息替换为关机时间信息：
```bash
# uprecords -d
     #               Uptime |        Last downtime                       Boot up
----------------------------+---------------------------------------------------
     1     0 days, 02:35:51 |     0 days, 00:01:07      Tue Feb 11 14:22:33 2020
->   2     0 days, 00:13:02 |     0 days, 00:04:05      Tue Feb 11 17:02:29 2020
     3     0 days, 00:07:27 |     0 days, 00:00:00      Tue Feb 11 14:13:59 2020
----------------------------+---------------------------------------------------
no1 in     0 days, 02:22:50 | at                        Tue Feb 11 19:38:21 2020
    up     0 days, 02:56:20 | since                     Tue Feb 11 14:13:59 2020
  down     0 days, 00:05:12 | since                     Tue Feb 11 14:13:59 2020
   %up               97.136 | since                     Tue Feb 11 14:13:59 2020
```
查看更多的命令选项：
```bash
# uprecords -?
usage: uprecords [OPTION]...

  -?             this help
  -a             do not print ansi codes
  -b             sort by boottime
  -B             reverse sort by boottime
  -k             sort by sysinfo
  -K             reverse sort by sysinfo
  -d             print downtime seen before every uptimes instead of system
  -c             do not show current entry if not in top entries
  -f             run continously in a loop
  -s             do not print extra statistics
  -w             wide output (more than 80 cols per line)
  -i INTERVAL    use INTERVAL seconds for loop instead of 5, implies -f
  -m COUNT       show a maximum of top COUNT entries instead of 10
  -M             show next milestone
  -v             version information
```
上面所举的例子都是日常使用中十分常见的，如果想要了解更多关于 `uptimed` 的用法，可以参考它的帮助手册。
```bash
$ man uprecords
```
