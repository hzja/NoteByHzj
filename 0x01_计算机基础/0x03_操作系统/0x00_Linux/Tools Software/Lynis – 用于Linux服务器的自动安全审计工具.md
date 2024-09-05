Linux Lynis 服务器安全审计工具<br />Lynis是Unix/Linux等操作系统的一款安全审计工具，它可以发现基于Linux系统中的恶意软件和安全漏洞。Lynis是免费开源的服务器审计工具，一旦审计完成，可以审查结果、警告和建议，然后可以根据它实现服务器的安全策略。它将显示一个报告，该报告可以被分成几个部分。<br />通常在Linux服务器上运行很多东西，比如网络服务、数据库服务、电子邮件服务、FTP服务等等。通过在所有Linux机器上进行自动的安全审计和渗透测试，Lynis可以简化管理员的工作。
<a name="k8Q4W"></a>
### 环境

- Centos 8<br />
- lynis-3.0.0<br />
<a name="nFmeG"></a>
### 安装Lynix
Lynis是一款轻量级的软件，它不会破坏系统，也不会影响Linux系统上的任何应用程序或服务。首先，将为Lynis的安装创建一个目录，进入该目录，wget下载最新的Lynis源代码：
```bash
[root@localhost ~]# mkdir /usr/local/lynis
[root@localhost ~]# cd /usr/local/lynis/
[root@localhost lynis]# wget https://downloads.cisofy.com/lynis/lynis-3.0.0.tar.gz
```
解压压缩包，进入解压出来的目录
```bash
[root@localhost lynis]# tar xvf lynis-3.0.0.tar.gz 
[root@localhost lynis]# cd lynis
```
运行lynis显示帮助信息。具有管理员权限的用户可以运行该脚本，所有日志和输出将保存在`/var/log/lynis.log`文件中：
```
[root@localhost lynis]# ./lynis
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1602771344210-903b4edd-81ab-4e90-ad6f-c58861079f13.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2119898&status=done&style=none&width=1107.6666666666667)
<a name="ZoTnI"></a>
### 开始审计并查找漏洞
执行本地安全扫描，所以使用 `audit system` 参数来扫描整个系统。运行下面的命令来启动对整个系统的审计：
```bash
[root@localhost lynis]# ./lynis audit system
或者
[root@localhost lynis]# ./lynis audit system --wait
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1602772285729-48c0420a-a3d5-4699-a1c6-859549539b56.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2139884&status=done&style=none&width=1107.6666666666667)<br />`--wait`选项：等待用户按回车键显示下一节的报告。<br />有时不想扫描或审计整个系统的应用程序或服务，所以可以按类别审计自定义应用程序。可以先列出所有的组，然后选择需要审计或扫描的组。
```
[root@localhost lynis]# ./lynis show groups
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1602772347377-b4490bbf-db3a-46eb-8681-b8bfce7e8753.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2084604&status=done&style=none&width=1107.6666666666667)<br />现在将对"kernel"和"firewalls"进行简单的审计，将使用下面的命令。
```bash
[root@localhost lynis]# ./lynis --tests-from-group "kernel firewalls"
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1602772463865-3b111b90-601a-4978-88ed-1ba1e0e7a050.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2176814&status=done&style=none&width=1107.6666666666667)<br />要查看更多的lynis命令选项，请参考它的手册页：
```
[root@localhost lynis]# ./lynis --man
```
<a name="etA8N"></a>
### 总结
Lynis是Unix/Linux等操作系统的一款安全审计工具，它可以发现基于Linux系统中的恶意软件和安全漏洞。
