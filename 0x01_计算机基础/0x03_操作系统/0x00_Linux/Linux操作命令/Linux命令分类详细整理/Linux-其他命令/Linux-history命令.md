Linux history<br />`history` 命令来查看命令历史记录，但是实际上 `history` 命令并非只有这个功能，`history` 还有很多有用的功能。尤其是 Bash 版本的 `history` 命令，它所提供的功能比所有其他的 Linux Shell `history` 命令所提供的都要多。<br />Bash 的历史悠久，是一个古老的 Shell ，并且它还有一个更古老的前身 the Bourne Shell (sh) 。因此，Bash 的 `history` 命令是所有的 Linux Shell `history` 命令中功能最丰富的。Bash 版本的 `history` 命令不仅支持反向搜索、快速调用，还支持重写历史记录等等功能。<br />善用 Bash `history` 命令以上的这些功能都可以提高工作效率， Bash `history` 命令以及它常用的功能:
<a name="emXmn"></a>
### history 是内置的命令
`history` 命令与许多其他的命令不同。可能习惯于命令都作为可执行文件放置在常见的系统级的位置，例如 `/usr/bin`，`/usr/local/bin` 或 `〜/ bin`。但是，内置的 `history` 命令并不在环境变量 `PATH` 保存的路径中的。<br />实际上，`history`  命令并没有保存在物理位置中：
```bash
which history
/usr/bin/which: no history 
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1599009483057-43bd405f-4a51-4053-a277-4d4e2991682e.png#align=left&display=inline&height=108&originHeight=324&originWidth=3323&size=344438&status=done&style=none&width=1107.6666666666667)<br />`history` 其实是 Shell 本身的一个内置函数：
```bash
[root@iZuligp6e1dyzfZ ~ 09:16]# type history
history is a shell builtin
[root@iZuligp6e1dyzfZ ~ 09:17]# help history
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1599009532776-83b22da1-d5b3-402b-b699-0ebe96ff6bbb.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2100881&status=done&style=none&width=1107.6666666666667)<br />由于 `history` 是 Shell 的内置函数，所以每种 Shell 的 `history` 函数都是独一无二的。因此，在 Bash 中能使用的功能可能无法在 Tcsh，Fish 或 Dash 中使用，同样的，在 Tcsh，Fish 或 Dash 中能使用的功能也可能无法在 Bash 中使用。
<a name="ed86746d"></a>
### 查看Bash 命令历史记录
`history` 命令最基本，最频繁的用法就是查看Shell 会话的命令历史记录：
```bash
history
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1599009658049-5fb6cd32-990b-4d0b-b7ca-e8cc31a6938d.png#align=left&display=inline&height=215&originHeight=646&originWidth=3323&size=716870&status=done&style=none&width=1107.6666666666667)
<a name="QfJO2"></a>
### 事件提示符（命令回放）
事件提示符 (!) 是按事件搜索历史记录的。这里的事件，指的是每一条记录在历史记录里的命令。换句话说，它就是一行命令，并被数字索引标记着以供引用。<br />要重新运行历史记录中的一个命令，用 ! 直接加上 (无空格) 想要运行的命令前面的索引数字即可。例如，假设历史记录中的第一条指令是 `echo Fcant` ，然后想重新运行它：
```bash
[root@iZuligp6e1dyzfZ ~ 09:22]# !1002
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1599009770988-67a9512f-259c-485b-ace1-f58198465fa8.png#align=left&display=inline&height=162&originHeight=486&originWidth=3323&size=520105&status=done&style=none&width=1107.6666666666667)<br />还可以通过从历史记录中的当前位置开始提供负数的行来使用相对定位。例如，返回历史记录中倒数第3条命令:
```bash
 1002  echo "Fcant"
 1003  ll
 1004  cd /
 1005  ll
 1006  history -3
 1007  history
[root@iZuligp6e1dyzfZ / 09:24]# !-6
echo "Fcant"
Fcant
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1599009942436-bdd44d22-8cda-4921-a819-d34f336fbad9.png#align=left&display=inline&height=198&originHeight=595&originWidth=3323&size=632268&status=done&style=none&width=1107.6666666666667)<br />只想返回上一条命令，可以使用简写 !! 来替代 !-1。这整整节省了一次按键的时间
```bash
[root@iZuligp6e1dyzfZ / 09:24]# !!
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1599009992930-cdb80809-238a-400a-a142-04207b16939a.png#align=left&display=inline&height=251&originHeight=754&originWidth=3323&size=830782&status=done&style=none&width=1107.6666666666667)
<a name="lG1qC"></a>
### 字符串搜索
:::danger
若是历史记录中包含目标字符串的命令不止一条，则它只会执行符合条件的命令中最后的一条
:::
<a name="8Bgjz"></a>
#### 命令的精确搜索
也可以通过特定的字符串来搜索历史记录中的命令并运行它。<br />若是想要搜索以特定字符串开头的命令，就用 ! 直接加上 (无空格) 想要搜索的字符串：
```bash
[root@iZuligp6e1dyzfZ / 09:25]# !echo
echo "Fcant"
Fcant
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1599010255107-96c90f25-55cf-4850-a103-38265b897d14.png#align=left&display=inline&height=89&originHeight=268&originWidth=3323&size=237440&status=done&style=none&width=1107.6666666666667)
<a name="Osd7T"></a>
#### 命令的模糊查询
还可以搜索在任意位置包含特定字符串的命令。要做到这点，只需要用 ! 直接加上前后两端都被 ? 包围的特定字符串即可，像这样:
```bash
[root@iZuligp6e1dyzfZ / 09:27]# !?Fcant?
echo "Fcant"
Fcant
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1599010366081-c3d5971b-a609-4067-b225-3edf278c11a0.png#align=left&display=inline&height=143&originHeight=429&originWidth=3323&size=450447&status=done&style=none&width=1107.6666666666667)
<a name="o4Wrk"></a>
#### 命令的匹配搜索（向后匹配）
```bash
[root@iZuligp6e1dyzfZ / 09:27]# !?Fcant
echo "Fcant"
Fcant
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1599010633514-3c2ef6e2-5ea4-49d5-99a8-d7ecea80e79e.png#align=left&display=inline&height=124&originHeight=372&originWidth=3323&size=371871&status=done&style=none&width=1107.6666666666667)
<a name="ZGTfq"></a>
#### 不支持向前匹配搜索
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1599010690305-bec29e01-4d68-400e-9e52-3acdca727a8c.png#align=left&display=inline&height=162&originHeight=487&originWidth=3323&size=528098&status=done&style=none&width=1107.6666666666667)
<a name="HzM0X"></a>
### 字符串替换
可以搜索一个特定的字符串并用新字符串替换它，从而更改命令：
```bash
[root@iZuligp6e1dyzfZ / 09:46]# echo 'Fcant'
Fcant
[root@iZuligp6e1dyzfZ / 09:47]# ^Fcant^Hello
echo 'Hello '
Hello
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1599011434528-05eded81-4f3f-4a10-96d6-452b657cb1eb.png#align=left&display=inline&height=126&originHeight=377&originWidth=3323&size=382482&status=done&style=none&width=1107.6666666666667)<br />但是它只能替换第一次出现的目标字符串，若是命令中出现两次目标字符串，则只有第一次出现的会被替换，像这样：
```bash
[root@iZuligp6e1dyzfZ / 09:48]# echo 'Hello Fc Fc'
Hello Fc Fc
[root@iZuligp6e1dyzfZ / 09:48]# ^Fc^Fcant
echo 'Hello Fcant Fc'
Hello Fcant Fc
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1599011548944-80c2af8c-1ac7-4ad4-9c93-785b60280006.png#align=left&display=inline&height=144&originHeight=432&originWidth=3323&size=457700&status=done&style=none&width=1107.6666666666667)<br />与字符串搜索一样，当历史记录中包含目标字符串的命令不止一条时，只替换并执行最后一条：
```bash
[root@iZuligp6e1dyzfZ / 09:48]# echo 'Hello Fcant Fc'
Hello Fcant Fc
[root@iZuligp6e1dyzfZ / 09:49]# echo 'Hello'
Hello
[root@iZuligp6e1dyzfZ / 09:49]# ^Hello^World
echo 'World'
World
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1599011630798-53b0b2c0-e9d9-4793-8bc4-883c583b67fc.png#align=left&display=inline&height=162&originHeight=486&originWidth=3323&size=529885&status=done&style=none&width=1107.6666666666667)
<a name="Qp86V"></a>
### HISTORY命令配置记录时间
Linux系统下可通过history命令查看用户所有的历史操作记录，在安全应急响应中起着非常重要的作用，但在未进行附加配置情况下，history命令只能查看用户历史操作记录，并不能区分用户以及操作时间，不便于审计分析。<br />当然，一些不好的操作习惯也可能通过命令历史泄露敏感信息。
<a name="nnlYN"></a>
#### 1、命令历史记录中加时间
默认情况下没有命令执行时间，不利于审计分析。<br />通过设置`export HISTTIMEFORMAT='%F %T '`，让历史记录中带上命令执行时间。<br />[注意“`%T`”和后面的“`'`”之间有空格，不然查看历史记录的时候，时间和命令之间没有分割。](http://mp.weixin.qq.com/s?__biz=MzI3ODcxMzQzMw==&mid=2247523441&idx=2&sn=27baea07dd346dc2803a845b5a240a98&chksm=eb501947dc279051c9cc227bc0661280a3a77c6c6bebe4cad2dc234dd7bce48ffcbad3de95c4&scene=21#wechat_redirect)<br />[要一劳永逸，这个配置可以写在/etc/profile中，当然如果要对指定用户做配置，这个配置可以写在/home/$USER/.bash_profile中。](http://mp.weixin.qq.com/s?__biz=MzI3ODcxMzQzMw==&mid=2247523441&idx=2&sn=27baea07dd346dc2803a845b5a240a98&chksm=eb501947dc279051c9cc227bc0661280a3a77c6c6bebe4cad2dc234dd7bce48ffcbad3de95c4&scene=21#wechat_redirect)<br />要使配置立即生效请执行`source /etc/profile`，再查看history记录，可以看到记录中带上了命令执行时间。<br />如果想要实现更细化的记录，比如登陆过系统的用户、IP地址、操作命令以及操作时间一一对应，可以通过在/etc/profile里面加入以下代码实现<br />`export HISTTIMEFORMAT="%F %T who -u am i 2>/dev/null| awk '{print $NF}'|sed -e 's/[()]//g'``whoami "`，注意空格都是必须的。<br />修改/etc/profile并加载后，history记录如下，时间、IP、用户及执行的命令都一一对应。<br />通过以上配置，基本上可以满足日常的审计工作了，但了解系统的朋友应该很容易看出来，这种方法只是设置了环境变量，攻击者unset掉这个环境变量，或者直接删除命令历史，对于安全应急来说，这无疑是一个灾难。<br />针对这样的问题，应该如何应对，下面才是重点，通过修改bash源码，让history记录通过syslog发送到远程logserver中，大大增加了攻击者对history记录完整性破坏的难度。
<a name="oIQ4S"></a>
#### 2、修改bash源码，支持syslog记录
首先下载bash源码，可以从gnu.org下载，系统需要安装gcc等编译环境。这里下载的是bash4.4版本。<br />修改源码：bashhist.c<br />修改源码config-top.h，取消/`#define SYSLOG_HISTORY`/这行的注释<br />编译安装，编译参数为：`./configure --prefix=/usr/local/bash`，安装成功后.<br />此时可以修改/etc/passwd中用户Shell环境，也可以用编译好的文件直接替换原有的bash二进制文件，但最好对原文件做好备份。<br />替换时要注意两点:<br />1、一定要给可执行权限，默认是有的，不过有时候下载到Windows系统后，再上传就没有可执行权限了，这里一定要确定，不然会后悔的；<br />2、替换时原bash被占用，可以修改原用户的bash环境后再进行替换。<br />查看效果，可以发现history记录已经写到了/var/log/message中。<br />如果要写到远程logserver，需要配置syslog服务。<br />通过以上手段，可以有效保证history记录的完整性，避免攻击者登录系统后，通过取消环境变量、删除history记录等方式抹掉操作行为，为安全审计、应急响应等提供了完整的原始数据。
