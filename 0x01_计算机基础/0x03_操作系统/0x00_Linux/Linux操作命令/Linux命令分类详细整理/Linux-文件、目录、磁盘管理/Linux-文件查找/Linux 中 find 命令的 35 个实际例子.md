Linux
<a name="Oy37f"></a>
### 1、在当前目录中使用名称查找文件
查找名称为的所有文件rumenz.txt在当前工作目录中。
```bash
find . -name rumenz.txt

./rumenz.txt
```
<a name="f98En"></a>
### 2、在主目录下查找文件
找到下的所有文件/home带名字的目录rumenz.txt
```bash
find /home -name rumenz.txt

/home/rumenz.txt
```
<a name="kZing"></a>
### 3、使用名称和忽略大小写查找文件
查找名称为的所有文件rumenz.txt并包含大写和小写字母/home目录。
```bash
find /home -iname rumenz.txt

./rumenz.txt
./rumenz.txt
```
<a name="V3Tvs"></a>
### 4、使用名称查找目录
查找名称为的所有目录rumenz在/目录。
```bash
find / -type d -name rumenz

/rumenz
```
<a name="gXFmP"></a>
### 5、使用名称查找 PHP 文件
找到所有php文件名是rumenz.php在当前工作目录中。
```bash
find . -type f -name rumenz.php

./rumenz.php
```
<a name="pZahb"></a>
### 6、查找目录中的所有 PHP 文件
找到所有php目录中的文件。
```bash
find . -type f -name "*.php"

./rumenz.php
./login.php
./index.php
```
<a name="ZMpbJ"></a>
### 7、查找具有 777 权限的文件
查找所有权限为777
```bash
find . -type f -perm 0777 -print
```
<a name="xLvH6"></a>
### 8、查找没有 777 权限的文件
未经许可查找所有文件777
```bash
find / -type f ! -perm 777
```
<a name="IM9Xn"></a>
### 9、查找具有 644 权限的 SGID 文件
找到所有的SGID bit权限设置为的文件644
```bash
find / -perm 2644
```
<a name="tg4FB"></a>
### 10、查找具有 551 权限的粘滞位文件
找到所有的Sticky Bit设置权限为551
```bash
find / -perm 551
```
<a name="yBxxw"></a>
### 11、查找 SUID 文件
找到所有SUID设置文件。
```bash
find / -perm /u=s
```
<a name="GVMO0"></a>
### 12、查找 SGID 文件
找到所有SGID设置文件。
```bash
find / -perm /g=s
```
<a name="oIKYi"></a>
### 13、查找只读文件
找到所有Read Only文件。
```bash
find / -perm /u=r
```
<a name="YoM4s"></a>
### 14、查找可执行文件
找到所有Executable文件。
```bash
find / -perm /a=x
```
<a name="FjzEH"></a>
### 15、查找权限为 777 且 chmod 为 644 的文件
找到所有777权限文件和使用chmod命令设置权限644
```bash
find / -type f -perm 0777 -print -exec chmod 644 {} \;
```
<a name="oMVKV"></a>
### 16、查找权限为 777 且 chmod 为 755 的目录
找到所有777权限目录和使用chmod命令设置权限755
```bash
find / -type d -perm 777 -print -exec chmod 755 {} \;
```
<a name="xPPvw"></a>
### 17、查找和删除单个文件
查找名为的单个文件rumenz.txt并将其删除。
```bash
find . -type f -name "rumenz.txt" -exec rm -f {} \;
```
<a name="Huk9w"></a>
### 18、查找和删除多个文件
查找和删除多个文件，例如.mp3要么.txt，然后使用。
```bash
find . -type f -name "*.txt" -exec rm -f {} \;
```
OR
```bash
find . -type f -name "*.mp3" -exec rm -f {} \;
```
<a name="GZPp5"></a>
### 19、查找所有空文件
查找某个路径下的所有空文件。
```bash
find /tmp -type f -empty
```
<a name="sY02i"></a>
### 20、查找所有空目录
将某个路径下的所有空目录归档。
```bash
find /tmp -type d -empty
```
<a name="biegu"></a>
### 21、归档所有隐藏文件
要查找所有隐藏文件，请使用以下命令。
```bash
find /tmp -type f -name ".*"
```
<a name="ns0x9"></a>
### 22、根据用户查找单个文件
查找所有或单个文件rumenz.txt在下面/所有者 root 的根目录。
```bash
find / -user root -name rumenz.txt
```
<a name="sYczC"></a>
### 23、根据用户查找所有文件
查找属于用户的所有文件rumenz在下面/home目录。
```bash
find /home -user rumenz
```
<a name="sZbpg"></a>
### 24、根据组查找所有文件
查找属于该组的所有文件Developer在下面/home目录。
```bash
find /home -group developer
```
<a name="jOop4"></a>
### 25、查找用户的特定文件
查找所有.txt用户文件rumenz在下面/home目录。
```bash
find /home -user rumenz -iname "*.txt"
```
<a name="HMdFy"></a>
### 26、查找最近 50 天修改过的文件
查找所有被修改的文件50几天回来。
```bash
find / -mtime 50
```
<a name="dTcwV"></a>
### 27、查找最近 50 天访问过的文件
查找所有被访问的文件50几天回来。
```bash
find / -atime 50
```
<a name="dMgYZ"></a>
### 28、查找最近 50-100 天修改过的文件
查找所有修改超过的文件50几天前，不到100天。
```bash
find / -mtime +50 –mtime -100
```
<a name="QDu7r"></a>
### 29、查找过去 1 小时内更改过的文件
查找上次更改的所有文件1 hour
```bash
find / -cmin -60
```
<a name="YQOvL"></a>
### 30、查找最近 1 小时内修改过的文件
查找上次修改的所有文件1 hour
```bash
find / -mmin -60
```
<a name="P7HIN"></a>
### 31、查找过去 1 小时内访问过的文件
查找上次访问的所有文件1 hour
```bash
find / -amin -60
```
<a name="ldcgt"></a>
### 32、找到 50MB 的文件
查找所有50MB文件，使用。
```bash
find / -size 50M
```
<a name="M2EPM"></a>
### 33、查找 50MB – 100MB 之间的大小
查找所有大于50MB并且小于100MB
```bash
find / -size +50M -size -100M
```
<a name="E8lF4"></a>
### 34、查找和删除 100MB 文件
查找所有100MB文件并使用一个命令删除它们。
```bash
find / -type f -size +100M -exec rm -f {} \;
```
<a name="Mhrns"></a>
### 35、查找特定文件并删除
找到所有.mp3文件超过10MB并使用一个命令删除它们。
```bash
find / -type f -name *.mp3 -size +10M -exec rm {} \;
```
