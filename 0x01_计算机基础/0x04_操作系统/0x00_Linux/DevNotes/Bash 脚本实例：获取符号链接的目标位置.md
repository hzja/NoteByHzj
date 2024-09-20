Linux<br />都熟悉 Linux 中的符号链接，通常称为符号链接或软链接，符号链接是指向任何文件系统中的另一个文件或目录的特定文件。这里将介绍 Linux 中符号链接的基础知识，并创建一个简单的 bash 脚本来获取符号链接的目标位置。
<a name="zrIta"></a>
### 符号链接的类型
主要有两种类型的符号链接，即：<br />**硬链接：**硬链接是指向文件系统中的文件或目录的直接指针。硬链接只能在与目标文件相同的文件系统中创建。<br />**软连接：**软链接是文件或目录的间接快捷方式，可以存在于文件系统中的任何位置。软链接可以指向不同文件系统中的文件。
<a name="hRneA"></a>
### 如何创建符号链接
要在 Linux 中创建符号链接，使用 `ln` 命令。执行不带选项的命令会创建指向指定目标文件的硬链接。<br />`ln` 命令的一般语法是：
```bash
ln [OPTIONS]
```
如要创建软链接，需要添加 `-s` 选项：
```bash
ln -s [OPTION]
```
<a name="knXZ3"></a>
#### 实例一：创建文件的软连接
以下命令在主目录中创建指向/var/log/audit/audit.log文件的软链接：
```bash
[root@localhost ~]# ln -s /var/log/audit/audit.log ~/audit.log
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1656728054994-c54b3d82-edce-423c-b95a-19e5d0fe22c9.png#clientId=ucf85f7e6-9667-4&from=paste&id=uc409e728&originHeight=143&originWidth=718&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u552bd7ee-5df4-497b-b535-7005056f41e&title=)
<a name="okYR9"></a>
#### 实例二：创建目录的软连接
以下命令在主目录中创建指向/var/log目录的软链接：
```bash
[root@localhost ~]# ln -s /var/log ~/log
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1656728054928-8ff05613-f29b-49e2-8774-e3b7a401c2f0.png#clientId=ucf85f7e6-9667-4&from=paste&id=uaf7fda96&originHeight=110&originWidth=716&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4541b65b-8071-4ce2-9e36-9b5acfd93a4&title=)
<a name="lgS14"></a>
### 如何删除符号链接
要删除符号链接，使用命令 unlink 后跟要删除的符号链接的路径。
```bash
[root@localhost ~]# unlink ~/log
```
下图可看到，log的快捷方式没有了。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1656728054934-ff59fce8-b89e-49c9-b49f-3082dd8b7971.png#clientId=ucf85f7e6-9667-4&from=paste&id=u2d5629e8&originHeight=113&originWidth=704&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud03e1e2f-5b14-4d7f-b5d6-81c2ed41df8&title=)<br />注意：如果删除目标文件或目录，请记得删除符号链接，因为符号链接不会随着文件的删除而删除。
<a name="b9EUg"></a>
### 如何获取符号链接的目标文件或目录
每个符号链接都会指向一个目标文件或目录（除非损坏）。要获取符号链接的目标文件/目录，使用readlink命令显示符号链接的目标。<br />例如，要获取在前面部分中创建的 audit.log 文件的目标，可以执行以下操作：
```bash
[root@localhost ~]# readlink ~/audit.log 
/var/log/audit/audit.log
```
<a name="FFld6"></a>
### 获取符号链接的 Bash 脚本
可以创建一个简单的 bash 脚本，该脚本接受路径并列出所有符号链接及其目标文件或目录：
```bash
[root@localhost ~]# vim symlink.sh 
#!/bin/bash
read -p "Provide the directory to evaluate: " target_dir
cd $target_dir
links=$(find . -maxdepth 1 -type l -ls | awk '{print $11}')
for link in $links
do
    echo "$link -> $(readlink $link)"
done
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1656728054935-f3ebe0a5-2a68-4602-9021-a6acc67687aa.png#clientId=ucf85f7e6-9667-4&from=paste&id=uce210806&originHeight=194&originWidth=641&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua499b9f7-81d4-4776-af49-f9335cd4f3e&title=)<br />该脚本首先询问用户要获取的目录。然后，脚本转到提供的目录并找到目录中的所有符号链接，并将输出传递给`awk`命令。<br />`awk` 输出并定位符号链接，并将它们保存到一个名为 links 的变量中。<br />然后创建一个循环，抓取links变量中的每个链接，并使用 readlink 命令获取它们的目标位置。<br />最后，显示符号链接和目标目录。下面是一个示例输出：
```bash
[root@localhost ~]# ./symlink.sh 
Provide the directory to evaluate: /etc
./rc.local -> rc.d/rc.local
./redhat-release -> centos-release
./system-release -> centos-release
./init.d -> rc.d/init.d
./rc0.d -> rc.d/rc0.d
./rc1.d -> rc.d/rc1.d
./rc2.d -> rc.d/rc2.d
./rc3.d -> rc.d/rc3.d
./rc4.d -> rc.d/rc4.d
./rc5.d -> rc.d/rc5.d
./rc6.d -> rc.d/rc6.d
./localtime -> ../usr/share/zoneinfo/Asia/Shanghai
./mtab -> ../proc/self/mounts
./yum.conf -> dnf/dnf.conf
./grub2.cfg -> ../boot/grub2/grub.cfg
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1656728054943-7d542a4b-4b28-4618-b693-11b69776264b.png#clientId=ucf85f7e6-9667-4&from=paste&id=u6789845e&originHeight=276&originWidth=552&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8c657c00-e00b-4973-805f-32440aa0ce2&title=)
