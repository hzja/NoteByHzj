Shell<br />文件监控可以配合rsync实现文件自动同步，例如监听某个目录，当文件变化时，使用rsync命令将变化的文件同步。（可用于代码自动发布）。<br />实现文件自动同步，例如监听某个目录，当文件变化时，使用rsync命令将变化的文件同步。（可用于代码自动发布）。
<a name="ZK5DD"></a>
### inotify
inotify 是linux内核的一个特性，在内核 2.6.13 以上都可以使用。<br />如果在shell环境下，可以通过下面的命令进行安装 
```bash
yum install inotify-tools
```
安装以后有两个命令可以用`inotifywait` 和 `inotifywatch`，`inotifywait` 是需要使用的命令。<br />监听/usr/local/src 目录：
```bash
/usr/local/bin/inotifywait -mrq --timefmt '%d/%m/%y %H:%M'
--format '%T %w %f %e' -e modify,delete,create,attrib 
/usr/local/src
```
参数：

| 参数 | 说明 |
| --- | --- |
| -m | 持续监听 |
| -r | 使用递归形式监视目录 |
| -q | 减少冗余信息，只打印出需要的信息 |
| -e | 指定要监视的事件，多个时间使用逗号隔开 |
| --timefmt | 时间格式 |
| --format | 监听到的文件变化的信息 |

<a name="hNThj"></a>
#### --timefmt 说明
ymd分别表示年月日，H表示小时，M表示分钟
<a name="wBpOn"></a>
#### --format 说明
| 参数 | 说明 |
| --- | --- |
| %w | 表示发生事件的目录 |
| %f | 表示发生事件的文件 |
| %e | 表示发生的事件 |
| %Xe | 事件以“X”分隔 |
| %T | 使用由–timefmt定义的时间格式 |

执行上面的命令之后，在监听的目录下创建一个1.txt文件，得到如下结果：<br />执行上面的命令之后，在监听的目录下创建一个1.txt文件，得到如下结果：
```bash
22/03/18 17:22 /usr/local/src/ 1.txt CREATE
22/03/18 17:22 /usr/local/src/ 1.txt ATTRIB
```
这个脚本的功能是循环监听文件或目录的增删改事件，当事件发生执行设置的脚本文件。
```shell
#!/bin/sh
# 监视的文件或目录
filename=$1
# 监视发现有增、删、改时执行的脚本
script=$2

inotifywait -mrq --format '%e' --event create,delete,modify  $filename | while read event
do
  case $event in MODIFY|CREATE|DELETE) bash $script ;;
  esac
done
```
<a name="VUi0X"></a>
### shell脚本后台执行
使用nohup，其中test.sh为所执行的脚本，out.txt为输出信息的地方。<br />
```bash
nohup sh test.sh>out.txt &
```
