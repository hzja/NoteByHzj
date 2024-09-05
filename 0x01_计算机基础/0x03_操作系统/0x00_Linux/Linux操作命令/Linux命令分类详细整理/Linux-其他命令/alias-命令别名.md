Linux Alias<br />alias是为一条命令起别名<br />通常情况下alias命令适合下面场景：

- 简化过长且过于复杂的命令
- 记住复杂名称的命令
- 经常使用的命令节省时间
<a name="HVZpq"></a>
### 1、查看配置的别名
```bash
$ alias
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598582792495-538b71f0-72b7-49b2-b832-43cfeeb59a9f.png#height=271&id=w2P2m&originHeight=813&originWidth=3323&originalType=binary&ratio=1&size=955618&status=done&style=none&width=1107.6666666666667)
<a name="hIhGx"></a>
### 2、为命令配置别名
<a name="PNo5k"></a>
#### A. 编辑/etc/profile配置文件配置别名
```bash
$ vim /etc/profile

# 在配置文件追加添加别名配置
alias mv='mv -i'

# 保存刷新配置
$ source /etc/profile
```
<a name="WPpzJ"></a>
#### B. 通过命令将别名设置写入配置文件
```bash
$ echo "alias mv='mv -i'">>/etc/profile
# 保存刷新配置
$ source /etc/profile
```
<a name="zaEa8"></a>
### 3、为不同的用户配置不同的别名
编辑相应用户目录的.bash_profile文件即可
```bash
echo "alias mv='mv -i'">>~/.bash_profile
source /etc/profile
```
<a name="pvIim"></a>
### 4、创建alias
例如，设置一个别名列出所有文件包括隐藏文件，别名为la：
```bash
[root@server1 ~]# alias la='ls -al'
```
执行一下别名la看一下：
```bash
[root@server1 ~]# la
total 68
dr-xr-x---.  3 root root   216 May 25 13:13 .
drwxr-xr-x. 19 root root   271 May  7 15:12 ..
-rw-------.  1 root root  1178 Dec 29  2019 anaconda-ks.cfg
-rw-------.  1 root root 14798 May 20 01:48 .bash_history
-rw-r--r--.  1 root root    18 May 11  2019 .bash_logout
-rw-r--r--.  1 root root   176 May 11  2019 .bash_profile
-rw-r--r--.  1 root root   176 May 11  2019 .bashrc
drwx------   3 root root    17 Mar 11 11:17 .cache
-rw-r--r--.  1 root root   100 May 11  2019 .cshrc
-rw-------   1 root root    64 May 25 13:13 .lesshst
-rw-r--r--   1 root root   234 May 19 09:52 sample.html
-rw-r--r--.  1 root root   129 May 11  2019 .tcshrc
-rw-r--r--   1 root root  1178 May 19 09:32 test.txt
-rw-------   1 root root 15904 May 25 10:15 .viminfo
```
如果要永久使用，可以将该命令写入`~/.bashrc`文件里面。
```bash
[root@server1 ~]# echo "alias la='ls -al'" >> ~/.bashrc
```
<a name="qoFYO"></a>
### 5、列出alias
使用alias命令列出系统中已设置的所有别名：
```bash
[root@server1 ~]# alias
```
<a name="clruB"></a>
### 6、检查命令类型是否是别名
要检查命令是否为别名，使用`which`命令。如下实例显示的内容就是别名。
```bash
[root@server1 ~]# which la
alias la='ls -al'
/usr/bin/ls
```
<a name="M106Y"></a>
### 7、删除alias
如果需要停用别名，则可以使用`unalias`命令。要使更改永久生效，就需要在`~/.bashrc`文件中删掉对应的别名。
```bash
[root@server1 ~]# unalias la
```
<a name="Vdzf6"></a>
### 8、对常用命令使用alias
对于经常使用的命令，通过使用别名来缩短它们就可以节省更多的时间。在下面显示的别名中，用一个字母清除屏幕，使用`ll`显示目录中的文件，并仅查看最近创建或更新的五个文件：
```bash
[root@server1 ~]# alias c='clear'
[root@server1 ~]# alias ll='ls -al'
[root@server1 ~]# alias new='ls -1tr | tail -5'
```
<a name="BljdA"></a>
### 9、使用alias来更改命令的行为
例如，想让`ping`命令只请求4次，则可以使用此别名确保它仅发出四个ping请求：
```bash
[root@server1 ~]# alias ping='ping -c 4'
```
<a name="GD0ru"></a>
### 10、使用alias避免长字符串的选项
别名对于记住较长的命令选项非常有用。例如，要解压文件可以使用如下别名：
```bash
[root@server1 ~]# alias untar='tar -xvf'
```
<a name="puwgM"></a>
### 11、使用alias查看命令的历史记录
要使用搜索字词查看历史记录，请执行以下操作：
```bash
[root@server1 ~]# alias rec='history | grep'
```
这可以看到在历史记录中保留的相关记录，例如：
```bash
[root@server1 ~]# rec alias
```
<a name="KLMfN"></a>
### 12、使用alias搜索相关的命令
要查找与搜索词相关的命令，使用`apropos`命令，但是该命令有点陌生，可以设置一个别名，使用以下命令：
```bash
[root@server1 ~]# alias ?="apropos"
```
下面使用`?`搜索和`compress`相关的命令：
```bash
[root@server1 ~]# ? compress
```
<a name="tcRDp"></a>
### 13、显示IP地址
```bash
[root@server1 ~]# alias myip='hostname -I'
```
