Linux rsync scp
<a name="XJEc8"></a>
## A.文件的分发-scp命令的使用
<a name="boztA"></a>
### I.scp(secure copy)安全拷贝
 定义：scp可以实现服务器与服务器之间的数据拷贝
<a name="YbxGb"></a>
### II.基本语法
```bash
root@Fcant:/# scp -r $pdir/$fname $user@$host:$pdir/$fname
```

![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564573331513-e07b0e1d-f828-4a56-b3c2-deed5bd0068e.png#align=left&display=inline&height=41&originHeight=114&originWidth=1829&size=30069&status=done&style=none&width=665.0909090909091)
<a name="319Vl"></a>
#### 1.从本机将文件分发
```
[root@iZzvx8pr0tamavZ /]# scp -r /opt/module/ root@39.107.49.210:/opt/module
```

![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564559274245-a03deede-6f07-4725-b3e8-01c0068fd8e1.png#align=left&display=inline&height=57&originHeight=157&originWidth=2368&size=45130&status=done&style=none&width=861.0909090909091)
<a name="mG7nO"></a>
#### 2.从其他主机拉取文件
```
[root@iZzvx8pr0tamavZ opt]# scp -r root@120.79.178.68:/opt/module ./
```

![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564560504034-3f1c99bb-05ab-4bc6-9e9c-32647f4b3ebe.png#align=left&display=inline&height=65&originHeight=179&originWidth=1961&size=41426&status=done&style=none&width=713.0909090909091)
<a name="rX2IG"></a>
#### 3.从一台主机到另一台主机
```
[root@iZ2ze15ls0lgyufgfo49m5Z module]# scp -r root@101.132.167.127:/etc/profile root@120.79.178.68:/etc/profile
```

![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564570020361-04ddab17-4145-45ba-98fa-a5a777c049cd.png#align=left&display=inline&height=72&originHeight=197&originWidth=2998&size=62467&status=done&style=none&width=1090.1818181818182)
<a name="2SQwI"></a>
### III.scp 的可选参数
-p 拷贝文件的时候保留源文件建立的时间。 <br />-q 执行文件拷贝时，不显示任何提示消息。 <br />-r 拷贝整个目录   <br />-v 拷贝文件时，显示提示信息。
<a name="3nJ28"></a>
## B.rsync远程同步工具
<a name="7E6dl"></a>
### 1.rsync的介绍
rsync主要用于备份和镜像。具有速度快、避免复制相同内容和支持符号链接的优点。
<a name="Qdum2"></a>
### 2.rsync和scp的区别
rsync和scp的区别：用rsync做文件的复制要比scp的速度快，rsync只对文件差异做更新，scp是把所有文件都复制过去。
<a name="zfQzA"></a>
### 3.rsync下载
```bash
[root@iZ2ze15ls0lgyufgfo49m5Z home]# yum install rsync -y
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596773748403-16153e5c-6407-4de8-8c80-37d21f83bfcd.png#align=left&display=inline&height=241&originHeight=723&originWidth=3323&size=900384&status=done&style=none&width=1107.6666666666667)
<a name="dEHL8"></a>
### 4.基本语法
```
root@Fcant:/# rsync -rvl $pdir/$fname $user@$host:$pdir/$fname 
```

![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564572403649-9b590475-d093-44a7-b7fd-1bd9f7d5a108.png#align=left&display=inline&height=57&originHeight=157&originWidth=2001&size=38532&status=done&style=none&width=727.6363636363636)

| **选项** | **功能** |
| :---: | :---: |
| -r | 递归 |
| -v | 显示复制过程 |
| -l | 拷贝符号链接 |

<a name="U1gJP"></a>
### 5.rsync的参数
-v, --verbose 详细模式输出<br />-q, --quiet 精简输出模式<br />-c, --checksum 打开校验开关，强制对文件传输进行校验<br />-a, --archive 归档模式，表示以递归方式传输文件，并保持所有文件属性，等于-rlptgoD<br />-r, --recursive 对子目录以递归模式处理<br />-R, --relative 使用相对路径信息<br />-b, --backup 创建备份，也就是对于目的已经存在有同样的文件名时，将老的文件重新命名为~filename。可以使用--suffix选项来指定不同的备份文件前缀。<br />--backup-dir 将备份文件(如~filename)存放在在目录下。<br />-suffix=SUFFIX 定义备份文件前缀<br />-u, --update 仅仅进行更新，也就是跳过所有已经存在于DST，并且文件时间晚于要备份的文件。(不覆盖更新的文件)<br />-l, --links 保留软链结<br />-L, --copy-links 想对待常规文件一样处理软链结<br />--copy-unsafe-links 仅仅拷贝指向SRC路径目录树以外的链结<br />--safe-links 忽略指向SRC路径目录树以外的链结<br />-H, --hard-links 保留硬链结<br />-p, --perms 保持文件权限<br />-o, --owner 保持文件属主信息<br />-g, --group 保持文件属组信息<br />-D, --devices 保持设备文件信息<br />-t, --times 保持文件时间信息<br />-S, --sparse 对稀疏文件进行特殊处理以节省DST的空间<br />-n, --dry-run现实哪些文件将被传输<br />-W, --whole-file 拷贝文件，不进行增量检测<br />-x, --one-file-system 不要跨越文件系统边界<br />-B, --block-size=SIZE 检验算法使用的块尺寸，默认是700字节<br />-e, --rsh=COMMAND 指定使用rsh、ssh方式进行数据同步<br />--rsync-path=PATH 指定远程服务器上的rsync命令所在路径信息<br />-C, --cvs-exclude 使用和CVS一样的方法自动忽略文件，用来排除那些不希望传输的文件<br />--existing 仅仅更新那些已经存在于DST的文件，而不备份那些新创建的文件<br />--delete 删除那些DST中SRC没有的文件<br />--delete-excluded 同样删除接收端那些被该选项指定排除的文件<br />--delete-after 传输结束以后再删除<br />--ignore-errors 及时出现IO错误也进行删除<br />--max-delete=NUM 最多删除NUM个文件<br />--partial 保留那些因故没有完全传输的文件，以是加快随后的再次传输<br />--force 强制删除目录，即使不为空<br />--numeric-ids 不将数字的用户和组ID匹配为用户名和组名<br />--timeout=TIME IP超时时间，单位为秒<br />-I, --ignore-times 不跳过那些有同样的时间和长度的文件<br />--size-only 当决定是否要备份文件时，仅仅察看文件大小而不考虑文件时间<br />--modify-window=NUM 决定文件是否时间相同时使用的时间戳窗口，默认为0<br />-T --temp-dir=DIR 在DIR中创建临时文件<br />--compare-dest=DIR 同样比较DIR中的文件来决定是否需要备份<br />-P 等同于 --partial<br />--progress 显示备份过程<br />-z, --compress 对备份的文件在传输时进行压缩处理<br />--exclude=PATTERN 指定排除不需要传输的文件模式<br />--include=PATTERN 指定不排除而需要传输的文件模式<br />--exclude-from=FILE 排除FILE中指定模式的文件<br />--include-from=FILE 不排除FILE指定模式匹配的文件<br />--version 打印版本信息<br />--address 绑定到特定的地址<br />--config=FILE 指定其他的配置文件，不使用默认的rsyncd.conf文件<br />--port=PORT 指定其他的rsync服务端口<br />--blocking-io 对远程shell使用阻塞IO<br />-stats 给出某些文件的传输状态<br />--progress 在传输时现实传输过程<br />--log-format=formAT 指定日志文件格式<br />--password-file=FILE 从FILE中得到密码<br />--bwlimit=KBPS 限制I/O带宽，KBytes per second<br />-h, --help 显示帮助信息
<a name="PMleS"></a>
### 6.rsync的`--exclude`使用示例
<a name="asPvi"></a>
#### 排除特定文件
要排除特定文件，将文件的相对路径传递给该`--exclude`选项。在以下示例中，文件 src_directory/file.txt 将不会被传输：
```bash
rsync -a --exclude 'file.txt' src_directory/ dst_directory/
```
<a name="o0LNb"></a>
#### 排除特定目录
排除特定目录与排除文件相同，只需将目录的相对路径传递给该`--exclude`选项，如下所示：
```bash
rsync -a --exclude 'dir1' src_directory/ dst_directory/
```
如果要排除目录内容但不排除目录本身，使用`dir1/*`代替`dir1`：
```bash
rsync -a --exclude 'dir1/*' src_directory/ dst_directory/
```
<a name="aXJal"></a>
#### 排除多个文件或目录
要排除多个文件或目录，只需指定多个`--exclude`选项：
```bash
rsync -a --exclude 'file1.txt' --exclude 'dir1/*' --exclude 'dir2' src_directory/ dst_directory/
```
如果希望使用单个`--exclude`选项，则可以用`{}`将要排除的文件和目录列出，并用逗号分隔，如下所示：
```bash
rsync -a --exclude={'file1.txt','dir1/*','dir2'} src_directory/ dst_directory/
```
如果要排除的文件和/或目录的数量很多，则—exclude可以使用要在文件中排除的文件和目录，而不是使用多个选项。只需将文件传递给该—exclude-from选项。下面的命令与上面的命令完全相同：
```bash
rsync -a --exclude-from='exclude-file.txt' src_directory/ dst_directory/
```
在 exclude-file.txt 内存储这些内容：
```bash
file1.txtdir1/*dir2
```
<a name="HiZ31"></a>
#### 根据正则模式排除多个文件或目录
使用rsync，还可以根据与文件或目录名称匹配的模式排除文件和目录。<br />例如，要排除所有`.jpg`文件，运行下面的命令：
```bash
rsync -a --exclude '*.jpg*' src_directory/ dst_directory/
```
排除与特定模式匹配的文件和目录之外的其他所有文件和目录并不困难。假设要排除所有其他文件和目录，但以 .jpg 结尾的文件除外。<br />一种选择是使用以下命令：
```bash
rsync -a -m --include='*.jpg' --include='*/' --exclude='*' src_directory/ dst_directory/
```
使用多个 include/exclude 选项时，将应用第一个匹配规则。参数含义：

- `--include='*.jpg'`-首先，包括所有.jpg文件。
- `--include='*/'`然后，将所有目录都包含在in src_directory目录中。<br />没有这个rsync只会复制*.jpg顶级目录中的文件。
- `-m` -删除空目录

另一个选择是将 find 命令的输出传递给 rsync：
```bash
find src_directory/ -name "*.jpg" -printf %P\\0\\n | rsync -a --files-from=- src_directory/ dst_directory/
```
参数含义：

- `-printf %P\\0\\n` 将从src_directory/文件路径中删除。
- `--files-from=-` 表示仅包含来自标准输入的文件（从find命令传递的文件）。
