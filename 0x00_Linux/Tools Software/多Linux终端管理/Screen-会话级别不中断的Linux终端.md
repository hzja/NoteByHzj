Linux Screen <br />很多时候需要执行一些需要很长时间的任务。如果这时候，网络连接突然断开了，那么之前所做的所有工作可能都会丢失，所做的工作可能都要重做一遍，这会浪费许多的时间，非常影响工作。使用 `screen` 命令。`screen` 命令允许用户在一个窗口内使用多个终端会话，可以断开连接，也可以重新连接已断开连接的会话。每个会话都可以恢复连接，这样就算会话断开了，用户也不必担心数据丢失。
<a name="gNXlD"></a>
### 1、安装Screen
<a name="sz1wO"></a>
#### ①检查当前Linux系统是否预安装了Screen
```bash
[root@iZuligp6e1dyzfZ ~ 16:56]# screen --version
Screen version 4.01.00devel (GNU) 2-May-06
```
<a name="SlMlI"></a>
#### ②根据不同系统选择不同命令进行安装
```bash
# Ubuntu系统安装Screen
$ sudo apt install screen
# CentOS安装Screen
$ sudo yum install -y screen
```
<a name="6c73f567"></a>
### 2、启动Screen
```bash
$ screen
```
输入 `screen` 命令后，会看到一个布满版权信息的界面，只需按回车或者空格键就可以跳过这个界面。跳过之后，就可以看到终端界面了。这个终端和原来的终端并没有什么不同，一样可以浏览文件夹，打开文件，在原来的终端中能做到的事情，它也可以做到。<br />也可以在启动 Screen 会话时，使用 `-S` 参数给会话起名字。例如，启动一个叫 `Fcant` 的 Screen 会话：
```bash
$ screen -S Fcant
```
如果需要启动多个 Screen 会话时，可以用来区分其他 Screen 会话。<br />进入 Screen 终端后，按 `Ctrl a+?` 快捷键，可以查看 Screen 所附带的所有命令：
<a name="R6qSB"></a>
### 3、查看执行中的Screen会话
<a name="jkADr"></a>
#### `screen -r`
```bash
[root@iZuligp6e1dyzfZ ~ 22:10]# screen -r
There are several suitable screens on:
        985.pts-0.iZuligp6e1dyzfZ       (Detached)
        31673.Fcant     (Detached)
Type "screen [-d] -r [pid.]tty.host" to resume one of them.
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597328097582-fa3b28b6-e310-4bfe-a898-54aaf9250022.png#align=left&display=inline&height=236&originHeight=707&originWidth=3323&size=800555&status=done&style=none&width=1107.6666666666667)
<a name="3S6F3"></a>
#### `screen -ls`
```bash
[root@iZuligp6e1dyzfZ ~ 22:09]# screen -ls
There are screens on:
        985.pts-0.iZuligp6e1dyzfZ       (Detached)
        31673.Fcant     (Detached)
2 Sockets in /var/run/screen/S-root.
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597328105285-cf8bb881-ac63-4378-93f4-a5000a9f693f.png#align=left&display=inline&height=236&originHeight=707&originWidth=3323&size=800555&status=done&style=none&width=1107.6666666666667)
<a name="K4Klv"></a>
### 4、嵌套Screen会话
在当前会话再次执行screen命令
```bash
[root@iZuligp6e1dyzfZ ~ 17:05]# screen
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597309853621-b48a4265-d871-4ee4-8869-24dbfe92cef2.png#align=left&display=inline&height=116&originHeight=349&originWidth=3323&size=352251&status=done&style=none&width=1107.6666666666667)
<a name="boZ4N"></a>
### 5、按 `Ctrl a+d` 快捷键分离screen会话回到原始终端
<a name="bTFAK"></a>
### 6、 `screen -r 会话ID/会话名称` ：切换Screen会话
```bash
[root@iZuligp6e1dyzfZ ~ 17:11]# screen -r Fcant
```
<a name="XYNPH"></a>
### 7、 `exit` ：关闭Screen会话
```bash
$ exit
```
<a name="nSXjo"></a>
### 8、Screen分屏操作

- 创建一个 screen 会话
```bash
it@serverc:~$ screen -S test
```

- 按Ctrl+A，然后按Shift+S开启分屏；
- 按Ctrl+A，然后再按Tab，切换到下面屏幕
- 但此时下面屏幕什么还没有，需要通过Ctrl+A，然后按C在下面屏幕中创建终端

这样就可以在下面屏幕执行命令了，如果需要在不同的屏幕之间切换，可以通过 按Ctrl+A，然后再按Tab来切换。
<a name="LK51G"></a>
### 9、Screen命令的参数

- -A 　将所有的视窗都调整为目前终端机的大小。
- -d<作业名称> 　将指定的screen作业离线。
- -h<行数> 　指定视窗的缓冲区行数。
- -m 　即使目前已在作业中的screen作业，仍强制建立新的screen作业。
- -r<作业名称> 　恢复离线的screen作业。
- -R 　先试图恢复离线的作业。若找不到离线的作业，即建立新的screen作业。
- -s<shell> 　指定建立新视窗时，所要执行的shell。
- -S<作业名称> 　指定screen作业的名称。
- -v 　显示版本信息。
- -x 　恢复之前离线的screen作业。
- -ls或--list 　显示目前所有的screen作业。
- -wipe 　检查目前所有的screen作业，并删除已经无法使用的screen作业
