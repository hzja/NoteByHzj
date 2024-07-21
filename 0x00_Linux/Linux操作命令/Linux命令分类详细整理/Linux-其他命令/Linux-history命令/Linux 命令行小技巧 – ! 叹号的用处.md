Linux<br />bash 的历史记录里保留了输入的命令行记录。以下是如何充分利用该记录和`!`符号的使用教程。
<a name="ZOmsL"></a>
## history 的基础
HISTSIZE 变量值设置保存在历史列表中的命令数。默认情况下，该值为 500。这些先前发出的命令（称为历史列表）存储在历史文件中。它的默认位置是~/.bash_history，这个位置存储在shell变量HISTFILE中。<br />这里使用`!`命令将 bash 历史记录列表中的命令引入到输入中。此功能使使用者可以轻松地快速重复命令、替换文本、操作参数和修复先前命令中的拼写错误。
<a name="QGBOV"></a>
## 命令重复实例
以下是可以使用`!` 执行操作的一些示例。
<a name="NNvib"></a>
### 重复匹配字符串开头的最后一个命令
`!`后跟与要运行的命令匹配的第一个字符（或者字符串）将重复该命令的实例：
```bash
[root@localhost ~]# ls /root/
anaconda-ks.cfg  centos2ol.sh  employee.json  list_users  new.txt  wsbkuplog.txt
[root@localhost ~]# !l
ls /root/
anaconda-ks.cfg  centos2ol.sh  employee.json  list_users  new.txt  wsbkuplog.txt
[root@localhost ~]# !ls
ls /root/
anaconda-ks.cfg  centos2ol.sh  employee.json  list_users  new.txt  wsbkuplog.txt
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1655014720511-63f98596-03b8-4463-ab0a-eb694f35b7a7.png#clientId=uf9878ad8-9a2a-4&from=paste&id=u98b14eb6&originHeight=179&originWidth=754&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8be98137-e5b3-45e8-b638-909bc3a8a6e&title=)
<a name="MPFX9"></a>
### 重复匹配字符串中任意位置的最后一个命令
`!?`格式与上述相同，但历史命令不必是命令的开头：
```bash
[root@localhost ~]# cat employee.json 
[{"name": "John Brooks","id": "003"},{"name": "Randy Park","id": "053"},{"name": "Todd Gray","id": "009"}]
[root@localhost ~]# !?employee
cat employee.json 
[{"name": "John Brooks","id": "003"},{"name": "Randy Park","id": "053"},{"name": "Todd Gray","id": "009"}]
[root@localhost ~]# !?json
cat employee.json 
[{"name": "John Brooks","id": "003"},{"name": "Randy Park","id": "053"},{"name": "Todd Gray","id": "009"}]
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1655014720641-5163229a-9bf9-40bb-86fe-cd0af7145568.png#clientId=uf9878ad8-9a2a-4&from=paste&id=u6a92e0bc&originHeight=199&originWidth=891&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc79ad5cc-3dbf-4cf2-a6ed-bf3b89c090c&title=)
<a name="FcrgV"></a>
### 重复历史记录中的第 n 个命令
重复 bash 历史记录中的第 n 个命令：
```bash
[root@localhost ~]# !772
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1655014720637-dc75a58a-40e8-4d03-8881-6db4fee90e56.png#clientId=uf9878ad8-9a2a-4&from=paste&id=u1e865816&originHeight=461&originWidth=595&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf02424f1-a0f7-49bd-86f2-ac6865d4f77&title=)
<a name="mJMer"></a>
### 重复最后一条命令
如果有一个一直使用的命令，那就是`!!`。重复历史列表中的最后一个命令，其行为与 `!-1` 相同：
```bash
[root@localhost ~]# cat employee.json 
[{"name": "John Brooks","id": "003"},{"name": "Randy Park","id": "053"},{"name": "Todd Gray","id": "009"}]
[root@localhost ~]# !!
cat employee.json 
[{"name": "John Brooks","id": "003"},{"name": "Randy Park","id": "053"},{"name": "Todd Gray","id": "009"}]
[root@localhost ~]# !-1
cat employee.json 
[{"name": "John Brooks","id": "003"},{"name": "Randy Park","id": "053"},{"name": "Todd Gray","id": "009"}]
[root@localhost ~]# 
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1655014720677-0329611a-d542-4f76-bc70-53929ebe9e4c.png#clientId=uf9878ad8-9a2a-4&from=paste&id=u3439c3a6&originHeight=176&originWidth=889&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u90917a92-68b1-4c65-9039-da9435de411&title=)<br />如果上一条命令忘记添加sudo来执行，可以结合`!!`来使用。还可以后面结合管道符使用。
```bash
$ yum update
Loaded plugins: priorities, update-motd, upgrade-helper
You need to be root to perform this command.
$ sudo !!
sudo yum update
Loaded plugins: priorities, update-motd, upgrade-helper
$ ls
dir  dir1  dir2  file  file1  file2  hello.txt
$ !! | grep file
ls | grep file
file
file1
file2
```
<a name="hbY90"></a>
### 重复并替换字符串
经常发输入较长的命令，然后用不同的参数重新输入。或者，需要重新发出一个命令，因为之前的命令中有一个错字。字符串替换而无需重新输入整个长命令即可完成此操作。下面是语法：
```bash
!!:s^oldstring^newstring
```
命令中的`^`是分隔符，用后面的newstring字符串替换前面的oldstring字符串。
```bash
[root@localhost ~]# ll /etc/httpd/conf.d
total 16
-rw-r--r-- 1 root root 2926 Nov  4  2020 autoindex.conf
-rw-r--r-- 1 root root  400 Nov  4  2020 README
-rw-r--r-- 1 root root 1252 Nov  4  2020 userdir.conf
-rw-r--r-- 1 root root  764 Nov  4  2020 welcome.conf
[root@localhost ~]# !!:s^conf.d^conf
ll /etc/httpd/conf
total 28
-rw-r--r-- 1 root root 11899 Nov  4  2020 httpd.conf
-rw-r--r-- 1 root root 13064 Nov  4  2020 magic
[root@localhost ~]# 
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1655014720778-6c59b201-4939-4584-9059-f2b67c1d7c27.png#clientId=uf9878ad8-9a2a-4&from=paste&id=ue836d2e4&originHeight=222&originWidth=507&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1074216c-b4a1-4c73-ad9c-4b0b339f341&title=)
