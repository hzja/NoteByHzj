Linux<br />Address already in use这个提示，想必大家遇到过，怎么能快速找到问题并解决呢?下面有几种姿势可以了解一下。<br />在对网络连接或特定于应用程序的问题进行故障排除时，首先要检查的事情之一应该是系统上实际使用了哪些端口，以及哪个应用程序正在侦听特定的端口。<br />本文介绍了如何使用netstat，ss和lsof命令找出哪些服务正在侦听哪些端口。该说明适用于所有基于Linux和Unix的操作系统，例如macOS。
<a name="zzY47"></a>
### 什么是监听端口
网络端口由其编号，关联的IP地址和通信协议（例如TCP或UDP）的类型标识。<br />侦听端口是应用程序或进程在其上侦听的网络端口，充当通信端点。<br />每个监听端口都可以使用防火墙打开或关闭（过滤）。一般而言，开放端口是一个网络端口，它接受来自远程位置的传入数据包。<br />不能让两个服务监听同一IP地址上的同一端口。<br />例如，如果正在运行一个监听端口80和443的Apache Web服务器，并且尝试安装Nginx ，则后者将无法启动，因为HTTP和HTTPS端口是已经在使用中。
<a name="NkrOV"></a>
### 用`netstat`检查监听端口
`netstat`是一个命令行工具，可以提供有关网络连接的信息。<br />要列出所有正在侦听的TCP或UDP端口，包括使用端口的服务和套接字状态，请使用以下命令：
```bash
sudo netstat -tunlp
```
此命令中使用的选项具有以下含义：

- `-t` -显示TCP端口。
- `-u` -显示UDP端口。
- `-n` -显示数字地址而不是解析主机。
- `-l` -仅显示监听端口。
- `-p` -显示侦听器进程的PID和名称。仅当以root用户或 sudo 用户身份运行命令时，才会显示此信息。

输出将如下所示：
```bash
Proto Recv-Q Send-Q Local Address   Foreign Address     State       PID/Program name    
tcp        0      0 0:22              0:*               LISTEN      445/sshd            
tcp        0      0 0:25              0:*               LISTEN      929/master          
tcp6       0      0 :::3306           ::*               LISTEN      534/mysqld          
tcp6       0      0 :::80             :::*              LISTEN      515/apache2         
tcp6       0      0 :::22             :::*              LISTEN      445/sshd            
tcp6       0      0 :::25             :::*              LISTEN      929/master          
tcp6       0      0 :::33060          :::*              LISTEN      534/mysqld          
udp        0      0 0:68              0:*                           966/dhclient
```
在案例中，重要的几列是：

- Proto-套接字使用的协议。
- Local Address -进程侦听的IP地址和端口号。
- PID/Program name -PID和进程名称。

如果要过滤结果，请使用 grep命令。例如，要查找在TCP端口22上侦听的进程，可以输入：
```bash
sudo netstat -tnlp | grep :22
```
输出显示此计算机上的端口22被SSH服务器使用：
```bash
tcp        0      0 0:22              0:*               LISTEN      445/sshd
tcp6       0      0 :::22             :::*              LISTEN      445/sshd
```
如果输出为空，则表示端口上没有监听。<br />也可以根据条件过滤列表，例如PID，协议，状态等。<br />`netstat`已过时，被`ss`和 ip 取代，但它仍然是检查网络连接的最常用命令。
<a name="bgsiN"></a>
### 用`ss`
检查监听端口<br />`ss`是新的`netstat`。它缺少`netstat`的某些功能，但是公开了更多的TCP状态，并且速度稍快。命令选项基本相同，因此从`netstat`到`ss`的转换并不困难。<br />要使用`ss`获取所有监听端口的列表，请输入：
```bash
sudo ss -tunlp
```
输出与`netstat`报告的输出几乎相同：
```bash
State    Recv-Q   Send-Q     Local Address:Port      Peer Address:Port                                                                                        
LISTEN   0        128              0:22             0:*      users:(("sshd",pid=445,fd=3))                                                        
LISTEN   0        100              0:25             0:*      users:(("master",pid=929,fd=13))                                                     
LISTEN   0        128                    *:3306                 *:*      users:(("mysqld",pid=534,fd=30))                                                     
LISTEN   0        128                    *:80                   *:*      users:(("apache2",pid=765,fd=4),("apache2",pid=764,fd=4),("apache2",pid=515,fd=4))   
LISTEN   0        128                 [::]:22                [::]:*      users:(("sshd",pid=445,fd=4))                                                        
LISTEN   0        100                 [::]:25                [::]:*      users:(("master",pid=929,fd=14))                                                     
LISTEN   0        70                     *:33060                *:*      users:(("mysqld",pid=534,fd=33))
```
<a name="XAfqt"></a>
### 使用`lsof`
检查监听端口<br />`lsof`是功能强大的命令行应用程序，可提供有关进程打开的文件的信息。<br />在Linux中，所有内容都是文件。可以将套接字视为写入网络的文件。<br />要获取具有`lsof`的所有侦听TCP端口的列表，请输入：
```bash
sudo lsof -nP -iTCP -sTCP:LISTEN
```
使用的选项如下:

- `-n`-不要将端口号转换为端口名称。
- `-p` -不解析主机名，显示数字地址。

`-iTCP -sTCP:LISTEN` -仅显示TCP状态为LISTEN的网络文件。
```bash
COMMAND   PID     USER   FD   TYPE DEVICE SIZE/OFF NODE NAME
sshd      445     root    3u  IPv4  16434      0t0  TCP *:22 (LISTEN)
sshd      445     root    4u  IPv6  16445      0t0  TCP *:22 (LISTEN)
apache2   515     root    4u  IPv6  16590      0t0  TCP *:80 (LISTEN)
mysqld    534    mysql   30u  IPv6  17636      0t0  TCP *:3306 (LISTEN)
mysqld    534    mysql   33u  IPv6  19973      0t0  TCP *:33060 (LISTEN)
apache2   764 www-data    4u  IPv6  16590      0t0  TCP *:80 (LISTEN)
apache2   765 www-data    4u  IPv6  16590      0t0  TCP *:80 (LISTEN)
master    929     root   13u  IPv4  19637      0t0  TCP *:25 (LISTEN)
master    929     root   14u  IPv6  19638      0t0  TCP *:25 (LISTEN)
```
大多数输出列名称都是不言自明的:

- COMMAND，PID，USER-运行与端口关联的程序的名称，PID和用户。
- NAME -端口号。

要查找正在侦听特定端口（例如端口3306）的进程，可以使用：
```bash
sudo lsof -nP -iTCP:3306 -sTCP:LISTEN
```
输出显示MySQL服务器使用端口3306:
```bash
COMMAND PID  USER   FD   TYPE DEVICE SIZE/OFF NODE NAME
mysqld  534 mysql   30u  IPv6  17636      0t0  TCP *:3306 (LISTEN)
```
