Linux Shell <br />在 Linux 系统中，有环境变量和 Shell 变量这两种变量。<br />环境变量是在程序及其子程序中全局可用的，常常用来储存像默认的文本编辑器或者浏览器，以及可执行文件的路径等等这样的信息。而 Shell 变量仅在当前 Shell 中可用，可以用来存储当前用户的 ID 等信息。<br />那么什么是环境变量，什么是 Shell 变量，该如何设置和查看这两种变量呢？<br />环境变量<br />环境变量是以键值对的形式实现的，是在整个系统范围内都可用的变量，并由所有派生的子进程和 Shell 继承。而环境变量的名称是区分大小写的，且通常以大写的形式命名 ( MYVAR1, MYVAR2... )<br />单个值的环境变量看起来是这样的：
```bash
KEY=value1
```
如果想要将多个值赋予环境变量，则通常用冒号 ( : ) 作为分隔符。每一对键值对最终看起来是这样子的：
```bash
KEY=value1:value2:value3
```
如果要赋予环境变量的值包含空格，则需要使用引号：
```bash
KEY="value with spaces"
```
<a name="Pyj9i"></a>
### Shell变量
Shell 变量是专门用于设置或定义它们的 Shell 中的变量。每个 Shell ，例如 zsh 和 bash ，都有一组自己内部的 Shell 变量。它们通常用于跟踪临时数据，比如当前工作目录，而用法则与环境变量相同。<br />如果想让 Shell 变量作为全局变量使用，可以使用 export 指令：
```bash
$ export MYVAR=Fclinux.net
$ echo 
$MYVARFclinux.net
$ env | grep MYVAR
MYVAR=Fclinux.net
```
<a name="PYeTA"></a>
### 常见的环境变量与 Shell 变量
一些环境变量和 Shell 变量是非常有用的，并且经常会被引用。以下是一些常见的，以后可能会遇到的环境变量：

| 变量名 | 含义 |
| --- | --- |
| TERM | 这指定运行 Shell 时要模拟的终端类型。可以针对不同的操作需求模拟不同的硬件终端。不过，通常不需要管这个变量。 |
| USER | 当前登录的用户 |
| PWD | 当前的工作目录 |
| OLDPWD | 上一个工作目录，这个变量由 Shell 保存，以便通过执行 cd - 切换回上一个工作目录。 |
| LS_COLORS | 这定义了 ls 指令的颜色输出代码，用于向 ls 指令添加颜色输出。这通常用于区分不同的文件类型并使用户对文件类型等信息一目了然。 |
| MAIL | 当前用户邮箱的路径 |
| PATH | 系统在查找指令时会检查的目录列表。当用户输入一个指令时，系统将按此目录列表的顺序检查目录，以寻找相应的可执行文件。 |
| LANG | 当前的语言和本地化设置，包括字符编码。 |
| HOME | 当前用户的主目录 |
| _ | 上一个被执行的指令 |

除了以上这些环境变量，也可能会经常遇到以下这些 Shell 变量:

| 变量名 | 含义 |
| --- | --- |
| BASHOPTS | 执行 bash 时所启用的选项列表，这对于确定 Shell 环境是否按照预期的方式运行很有帮助。 |
| BASH_VERSION | 人类可读格式的正在执行的 bash 版本 |
| BASH_VERSINFO | 机器可读格式的正在执行的 bash 版本 |
| COLUMNS | 用于设置绘制到屏幕上的输出信息的宽的列数 |
| DIRSTACK | pushd 和 popd 命令可用的目录堆栈。 |
| HISTFILESIZE | 存储到文件中的命令历史记录的行数。默认为 ~/.bash_history 文件的行数。 |
| HISTSIZE | 内存中允许存储的命令历史记录的行数，即 histroy 命令所能打印出来的行数。 |
| HOSTNAME | 计算机的主机名 |
| IFS | 内部字段分隔符，用于分隔命令行上的输入。默认以空格作为分隔符。 |
| PS1 | 定义主命令提示符。这用于定义启动 Shell 会话时命令提示符的外观。而 PS2 用于声明跨越多行的命令的命令提示符。 |
| SHELLOPTS | 可以用 set 命令设置的 Shell 选项。 |
| UID | 当前用户的 UID (用户 ID) |

<a name="38aDD"></a>
### 查看 Shell 变量与环境变量
在 Linux 系统中，有以下几种命令可以查看环境变量：<br />env — 该命令可以在自定义的环境中运行程序，并且不需要改变当前的环境。当不带参数使用 env 命令时，它会打印出当前的环境变量列表。<br />printenv — 可以打印出所有的或者指定的环境变量。<br />set — 该命令可以设置或者删除 Shell 变量。当不带参数使用 set 命令时，它将打印出包括环境变量与 Shell 变量在内的所有变量以及 Shell 函数的列表。<br />默认状态下，env 和 printenv 的功能是完全相同的：
```bash
[root@iZuligp6e1dyzfZ ~ 09:50]# printenv
XDG_SESSION_ID=1707
HOSTNAME=iZuligp6e1dyzfZ
HARDWARE_PLATFORM=x86_64
TERM=xterm-256color
SHELL=/bin/bash
HISTSIZE=1000
SSH_CLIENT=121.239.106.21 61242 22
SSH_TTY=/dev/pts/0
USER=root
LS_COLORS=rs=0:di=38;5;27:ln=38;5;51:mh=44;38;5;15:pi=40;38;5;11:so=38;5;13:do=38;5;5:bd=48;5;232;38;5;11:cd=48;5;232;38;5;3:or=48;5;232;38;5;9:mi=05;48;5;232;38;5;15:su=48;5;196;38;5;15:sg=48;5;11;38;5;16:ca=48;5;196;38;5;226:tw=48;5;10;38;5;16:ow=48;5;10;38;5;21:st=48;5;21;38;5;15:ex=38;5;34:*.tar=38;5;9:*.tgz=38;5;9:*.arc=38;5;9:*.arj=38;5;9:*.taz=38;5;9:*.lha=38;5;9:*.lz4=38;5;9:*.lzh=38;5;9:*.lzma=38;5;9:*.tlz=38;5;9:*.txz=38;5;9:*.tzo=38;5;9:*.t7z=38;5;9:*.zip=38;5;9:*.z=38;5;9:*.Z=38;5;9:*.dz=38;5;9:*.gz=38;5;9:*.lrz=38;5;9:*.lz=38;5;9:*.lzo=38;5;9:*.xz=38;5;9:*.bz2=38;5;9:*.bz=38;5;9:*.tbz=38;5;9:*.tbz2=38;5;9:*.tz=38;5;9:*.deb=38;5;9:*.rpm=38;5;9:*.jar=38;5;9:*.war=38;5;9:*.ear=38;5;9:*.sar=38;5;9:*.rar=38;5;9:*.alz=38;5;9:*.ace=38;5;9:*.zoo=38;5;9:*.cpio=38;5;9:*.7z=38;5;9:*.rz=38;5;9:*.cab=38;5;9:*.jpg=38;5;13:*.jpeg=38;5;13:*.gif=38;5;13:*.bmp=38;5;13:*.pbm=38;5;13:*.pgm=38;5;13:*.ppm=38;5;13:*.tga=38;5;13:*.xbm=38;5;13:*.xpm=38;5;13:*.tif=38;5;13:*.tiff=38;5;13:*.png=38;5;13:*.svg=38;5;13:*.svgz=38;5;13:*.mng=38;5;13:*.pcx=38;5;13:*.mov=38;5;13:*.mpg=38;5;13:*.mpeg=38;5;13:*.m2v=38;5;13:*.mkv=38;5;13:*.webm=38;5;13:*.ogm=38;5;13:*.mp4=38;5;13:*.m4v=38;5;13:*.mp4v=38;5;13:*.vob=38;5;13:*.qt=38;5;13:*.nuv=38;5;13:*.wmv=38;5;13:*.asf=38;5;13:*.rm=38;5;13:*.rmvb=38;5;13:*.flc=38;5;13:*.avi=38;5;13:*.fli=38;5;13:*.flv=38;5;13:*.gl=38;5;13:*.dl=38;5;13:*.xcf=38;5;13:*.xwd=38;5;13:*.yuv=38;5;13:*.cgm=38;5;13:*.emf=38;5;13:*.axv=38;5;13:*.anx=38;5;13:*.ogv=38;5;13:*.ogx=38;5;13:*.aac=38;5;45:*.au=38;5;45:*.flac=38;5;45:*.mid=38;5;45:*.midi=38;5;45:*.mka=38;5;45:*.mp3=38;5;45:*.mpc=38;5;45:*.ogg=38;5;45:*.ra=38;5;45:*.wav=38;5;45:*.axa=38;5;45:*.oga=38;5;45:*.spx=38;5;45:*.xspf=38;5;45:
MAIL=/var/spool/mail/root
PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/root/bin
PWD=/root
LANG=en_US.UTF-8
PS1=[\u@\h \W \A]\$
HISTCONTROL=ignoredups
SHLVL=1
HOME=/root
LOGNAME=root
SSH_CONNECTION=121.239.106.21 61242 172.16.252.76 22
LESSOPEN=||/usr/bin/lesspipe.sh %s
XDG_RUNTIME_DIR=/run/user/0
_=/usr/bin/printenv
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597629025610-cddb5fae-f000-4496-8b68-57725dc46eb4.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2221359&status=done&style=none&width=1107.6666666666667)<br />只有在更具体的功能上才能体现出 env 命令与 printenv 命令的区别。例如，使用 printenv 命令，可以请求单个变量的值：
```bash
[root@iZuligp6e1dyzfZ ~ 09:50]# printenv SHELL
/bin/bash
[root@iZuligp6e1dyzfZ ~ 09:51]# printenv HOME
/root
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597629131307-699a9f69-4130-4ecc-9db1-c78fbdc51f3c.png#align=left&display=inline&height=108&originHeight=325&originWidth=3323&size=309582&status=done&style=none&width=1107.6666666666667)<br />env 命令可通过将一组变量传递给命令来修改程序运行的环境：
```bash
env FCVAR=Fclinux.net command_to_run command_options
```
printenv 与 env 命令只能打印出环境变量，而如果想打印出所有变量或者 Shell 函数的列表，可以使用 set 指令。
```bash
[root@iZuligp6e1dyzfZ ~ 09:51]# set
BASH=/bin/bash
BASHOPTS=checkwinsize:cmdhist:expand_aliases:extquote:force_fignore:histappend:hostcomplete:interactive_comments:login_shell:progcomp:promptvars:sourcepath
BASH_ALIASES=()
BASH_ARGC=()
BASH_ARGV=()
BASH_CMDS=()
BASH_LINENO=()
BASH_SOURCE=()
BASH_VERSINFO=([0]="4" [1]="2" [2]="46" [3]="1" [4]="release" [5]="x86_64-redhat-linux-gnu")
BASH_VERSION='4.2.46(1)-release'
COLUMNS=120
DIRSTACK=()
EUID=0
GROUPS=()
HARDWARE_PLATFORM=x86_64
HISTCONTROL=ignoredups
HISTFILE=/root/.bash_history
HISTFILESIZE=1000
HISTSIZE=1000
HOME=/root
HOSTNAME=iZuligp6e1dyzfZ
HOSTTYPE=x86_64
ID=0
IFS=$' \t\n'
LANG=en_US.UTF-8
LESSOPEN='||/usr/bin/lesspipe.sh %s'
LINES=30
LOGNAME=root
LS_COLORS='rs=0:di=38;5;27:ln=38;5;51:mh=44;38;5;15:pi=40;38;5;11:so=38;5;13:do=38;5;5:bd=48;5;232;38;5;11:cd=48;5;232;38;5;3:or=48;5;232;38;5;9:mi=05;48;5;232;38;5;15:su=48;5;196;38;5;15:sg=48;5;11;38;5;16:ca=48;5;196;38;5;226:tw=48;5;10;38;5;16:ow=48;5;10;38;5;21:st=48;5;21;38;5;15:ex=38;5;34:*.tar=38;5;9:*.tgz=38;5;9:*.arc=38;5;9:*.arj=38;5;9:*.taz=38;5;9:*.lha=38;5;9:*.lz4=38;5;9:*.lzh=38;5;9:*.lzma=38;5;9:*.tlz=38;5;9:*.txz=38;5;9:*.tzo=38;5;9:*.t7z=38;5;9:*.zip=38;5;9:*.z=38;5;9:*.Z=38;5;9:*.dz=38;5;9:*.gz=38;5;9:*.lrz=38;5;9:*.lz=38;5;9:*.lzo=38;5;9:*.xz=38;5;9:*.bz2=38;5;9:*.bz=38;5;9:*.tbz=38;5;9:*.tbz2=38;5;9:*.tz=38;5;9:*.deb=38;5;9:*.rpm=38;5;9:*.jar=38;5;9:*.war=38;5;9:*.ear=38;5;9:*.sar=38;5;9:*.rar=38;5;9:*.alz=38;5;9:*.ace=38;5;9:*.zoo=38;5;9:*.cpio=38;5;9:*.7z=38;5;9:*.rz=38;5;9:*.cab=38;5;9:*.jpg=38;5;13:*.jpeg=38;5;13:*.gif=38;5;13:*.bmp=38;5;13:*.pbm=38;5;13:*.pgm=38;5;13:*.ppm=38;5;13:*.tga=38;5;13:*.xbm=38;5;13:*.xpm=38;5;13:*.tif=38;5;13:*.tiff=38;5;13:*.png=38;5;13:*.svg=38;5;13:*.svgz=38;5;13:*.mng=38;5;13:*.pcx=38;5;13:*.mov=38;5;13:*.mpg=38;5;13:*.mpeg=38;5;13:*.m2v=38;5;13:*.mkv=38;5;13:*.webm=38;5;13:*.ogm=38;5;13:*.mp4=38;5;13:*.m4v=38;5;13:*.mp4v=38;5;13:*.vob=38;5;13:*.qt=38;5;13:*.nuv=38;5;13:*.wmv=38;5;13:*.asf=38;5;13:*.rm=38;5;13:*.rmvb=38;5;13:*.flc=38;5;13:*.avi=38;5;13:*.fli=38;5;13:*.flv=38;5;13:*.gl=38;5;13:*.dl=38;5;13:*.xcf=38;5;13:*.xwd=38;5;13:*.yuv=38;5;13:*.cgm=38;5;13:*.emf=38;5;13:*.axv=38;5;13:*.anx=38;5;13:*.ogv=38;5;13:*.ogx=38;5;13:*.aac=38;5;45:*.au=38;5;45:*.flac=38;5;45:*.mid=38;5;45:*.midi=38;5;45:*.mka=38;5;45:*.mp3=38;5;45:*.mpc=38;5;45:*.ogg=38;5;45:*.ra=38;5;45:*.wav=38;5;45:*.axa=38;5;45:*.oga=38;5;45:*.spx=38;5;45:*.xspf=38;5;45:'
MACHTYPE=x86_64-redhat-linux-gnu
MAIL=/var/spool/mail/root
MAILCHECK=60
OPTERR=1
OPTIND=1
OSTYPE=linux-gnu
PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/root/bin
PIPESTATUS=([0]="0")
PPID=12801
PROMPT_COMMAND='printf "\033]0;%s@%s:%s\007" "${USER}" "${HOSTNAME%%.*}" "${PWD/#$HOME/~}"'
PS1='[\u@\h \W \A]\$ '
PS2='> '
PS4='+ '
PWD=/root
SHELL=/bin/bash
SHELLOPTS=braceexpand:emacs:hashall:histexpand:history:interactive-comments:monitor
SHLVL=1
SSH_CLIENT='121.239.106.21 61242 22'
SSH_CONNECTION='121.239.106.21 61242 172.16.252.76 22'
SSH_TTY=/dev/pts/0
TERM=xterm-256color
UID=0
USER=root
XDG_RUNTIME_DIR=/run/user/0
XDG_SESSION_ID=1707
_=HOME
colors=/root/.dircolors
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597629150110-a3158d00-4a67-4147-a3ce-8790db5f3ecf.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2032736&status=done&style=none&width=1107.6666666666667)<br />该命令将显示一个包含所有变量的大列表，可能会希望将输出传递给 less 命令。
```bash
$ set | less
```
<a name="Igu05"></a>
### 设置 Shell 变量与环境变量
Linux 系统中可以设置环境变量的命令有以下几种：<br />set — 该命令可以设置或者取消设置 Shell 变量。当不带参数使用 set 命令时，它将打印出包括环境变量与 Shell 变量在内的所有变量以及 Shell 函数的列表。<br />unset — 该命令可以删除 Shell 变量以及环境变量。<br />export — 该命令可以设置环境变量。<br />为了更好地理解 Shell 变量和环境变量之间的区别，先从设置 Shell 变量开始说，然后再说环境变量。<br />先从在当前会话中定义一个 Shell 变量开始：
```bash
$ FCVAR=Fclinux
```
可以用 echo $MYVAR 来验证变量是否已设置：
```bash
[root@iZuligp6e1dyzfZ ~ 09:53]# echo FCVAR
FCVAR
```
使用 printenv 命令检验该变量是否为环境变量：
```bash
[root@iZuligp6e1dyzfZ ~ 09:53]# printenv FCVAR
```
没有输出返回，这就意味着 MYVAR 变量并不是环境变量。<br />export 命令可以用来设置环境变量。要创建环境变量，只需用 export 命令将 Shell 变量导出为环境变量：
```bash
[root@iZuligp6e1dyzfZ ~ 09:54]# export FCVAR
```
可以用以下语句检验：
```bash
[root@iZuligp6e1dyzfZ ~ 09:54]# printenv FCVAR
```
当然，也可以只用一行代码来设置环境变量：
```bash
$ export FCNEWVAR="My New Variable"
```
但是以这种方式创建的环境变量只能用于当前会话，如果打开一个新的 Shell 会话，或者退出登陆，所有的变量都将会丢失。<br />也可以将环境变量还原为 Shell 变量，或者将其完全删除：<br />MYVAR 变量被定义为环境变量，可以通过输入以下代码将其还原为 Shell 变量:
```bash
$ export -n FCVAR
```
这样，MYVAR 变量就不再是环境变量了，但依然还是 Shell 变量。<br />无论是 Shell 变量还是环境变量，想要完全删除变量，都可以使用 unset 命令来删除：
```bash
$ unset FCVAR
```
可以通过以下语句来验证 MYVAR 变量已经删除：
```bash
$ echo $FCVAR
```
由于变量已经删除了，所以没有任何返回。
<a name="ngVuJ"></a>
### 环境变量的持久化
许多程序都需要使用环境变量来决定具体的执行方式，但是，并不希望每次启动新的 Shell 会话时都必须重新设置重要的变量，因此需要将重要的环境变量写入配置文件中。<br />Shell 会话有不同的启动方式，比如，连接到终端的交互式 Shell 与未连接到终端的非交互式 Shell，还有登录式 Shell 与非登录式 Shell， 而 bash Shell 会根据会话不同的启动方式而读取不同的配置文件。<br />但是，在大多数 Linux 发行版中，当启动一个新的 Shell 会话时，一般都会从以下文件中读取环境变量:<br />/etc/environment — 使用此文件来设置系统范围内可用的环境变量。<br />/etc/profile — 每当 bash 登录 Shell 时，都会加载此文件中设置的变量。<br />~/.bashrc — 每个用户特定的 Shell 配置文件。例如， 如果使用的是Bash，就可以在其中声明变量。<br />如果要将新的环境变量加载到当前的 Shell 会话中，可以使用source命令：
```bash
$ source ~/.bashrc
```
如果想要设置环境变量，就可以考虑将其添加到 /etc/profile 、/etc/bash.bashrc 或 /etc/environment 文件中。
