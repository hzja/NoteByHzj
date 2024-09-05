Linux history
<a name="sYmqt"></a>
### 1、使用 HISTTIMEFORMAT 显示时间戳
当在命令行输入`history`时，它将显示序号和命令。出于审计目的，将时间戳与命令一起显示可能会有所帮助。
```bash
[root@localhost ~]# export HISTTIMEFORMAT='%F %T '
[root@localhost ~]# history | less
```
<a name="vJ2df"></a>
### 2、使用Ctrl + r 搜索历史记录
当已经执行了很长的命令时，可以简单地使用关键字搜索历史记录并重新执行同一命令，而不必完全键入它。按Ctrl + r 并键入关键字。在以下示例中，搜索了wget，它在包含wget一词的历史记录中显示了先前的命令“ wget -O /etc/yum.repos.d/CentOS-Base.repo [http://mirrors.aliyun.com/repo/Centos-8.repo](http://mirrors.aliyun.com/repo/Centos-8.repo)”。
```bash
# 当按下ctrl + r键，会出现下面状态。
(reverse-i-search)`': 
# 然后输入 wget ，后面会有命令过滤出匹配的历史命令。如果显示出来的是不想要的命令，可以继续补充命令行。
(reverse-i-search)`wget ': wget -O /etc/yum.repos.d/CentOS-Base.repo http://mirrors.aliyun.com/repo/Centos-8.repo
```
当搜索出想要的命令行，可以按下 `Enter`回车键，就可执行命令了。<br />过滤出匹配的历史命令，想要更改参数，可以按一下tab键、或者方向键，可以退出搜索模式来修改这条历史命令，并执行。
<a name="57be4acf"></a>
### 3、快速重复上一个命令
可以使用`!!`两个叹号，执行上一条命令。也可以使用`!-1`，执行上一条命令。
<a name="RHEtT"></a>
### 4、从history中执行特定命令
可以使用`!`加数字，执行history里面指定行号的命令。
```bash
[root@localhost ~]# !4
ss -tul
Netid     State      Recv-Q     Send-Q         Local Address:Port          Peer Address:Port     Process     
udp       UNCONN     0          0                  127.0.0.1:323                0.0.0.0:*                    
udp       UNCONN     0          0                      [::1]:323                   [::]:*                    
tcp       LISTEN     0          128                  0.0.0.0:ssh                0.0.0.0:*                    
tcp       LISTEN     0          128                     [::]:ssh                   [::]:*
```
<a name="fg5Tw"></a>
### 5、使用HISTSIZE控制历史记录中的总行数
默认情况下Centos8中，history存储1000条历史记录。可以使用下面命令查看：
```bash
[root@localhost ~]# echo $HISTSIZE
1000
```
将以下两行添加到`.bash_profile`，然后执行`source ~/.bash_profile`即可生效。下面命令中，使history存储历史记录少一些，只存储200条。
```bash
[root@localhost ~]# echo "HISTSIZE=200" >> .bash_profile 
[root@localhost ~]# echo "HISFILETSIZE=200" >> .bash_profile 
[root@localhost ~]# source ~/.bash_profile
```
其中`HISTSIZE`变量，控制history命令显示的数量。<br />其中`HISTFILESIZE`变量，控制`.bash_history`文件中，历史记录的数量。
<a name="2gldQ"></a>
### 6、更改历史记录文件名
默认情况下，历史记录的文件名叫`.bash_history`，可以使用`HISTFILE`变量更改它的文件名，可以将HISTFILE变量写入`~/.bash_profile`文件中，以下示例：
```bash
[root@localhost ~]# echo "HISTFILE=/root/.cmd_hist" >> ~/.bash_profile 
[root@localhost ~]# reboot
```
添加完成之后，重启操作系统，可以看到新的历史记录文件已经可以存储历史记录了。
<a name="TNIiK"></a>
### 7、清除历史记录
使用`-c`选项清除历史记录：
```bash
[root@localhost ~]# history -c
```
<a name="md6H8"></a>
### 8、禁用历史记录
如果想全部禁用历史记录，则将HISTSIZE和HISTFILESIZE设置为0，如下所示。
```bash
[root@localhost ~]# echo "HISTSIZE=0" >> .bash_profile 
[root@localhost ~]# echo "HISFILETSIZE=0" >> .bash_profile 
[root@localhost ~]# source ~/.bash_profile
```

