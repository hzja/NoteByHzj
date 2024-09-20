<a name="7xLHz"></a>
### Linux读取环境变量
读取环境变量的方法：

- `export`命令显示当前系统定义的所有环境变量
- `echo $PATH`命令输出当前的PATH环境变量的值

这两个命令执行的效果如下
```bash
[root@iZuligp6e1dyzfZ ~ 09:07]# export
declare -x HARDWARE_PLATFORM="x86_64"
declare -x HISTCONTROL="ignoredups"
declare -x HISTSIZE="1000"
declare -x HOME="/root"
declare -x HOSTNAME="iZuligp6e1dyzfZ"
declare -x INFOPATH="/opt/rh/devtoolset-9/root/usr/share/info"
declare -x JAVA_HOME="/local/dev/java/jdk1.8.0_161"
declare -x LANG="en_US.UTF-8"
declare -x LD_LIBRARY_PATH="/opt/rh/devtoolset-9/root/usr/lib64:/opt/rh/devtoolset-9/root/usr/lib:/opt/rh/devtoolset-9/root/usr/lib64/dyninst:/opt/rh/devtoolset-9/root/usr/lib/dyninst:/opt/rh/devtoolset-9/root/usr/lib64:/opt/rh/devtoolset-9/root/usr/lib"
declare -x LESSOPEN="||/usr/bin/lesspipe.sh %s"
declare -x LOGNAME="root"
declare -x LS_COLORS="rs=0:di=38;5;27:ln=38;5;51:mh=44;38;5;15:pi=40;38;5;11:so=38;5;13:do=38;5;5:bd=48;5;232;38;5;11:cd=48;5;232;38;5;3:or=48;5;232;38;5;9:mi=05;48;5;232;38;5;15:su=48;5;196;38;5;15:sg=48;5;11;38;5;16:ca=48;5;196;38;5;226:tw=48;5;10;38;5;16:ow=48;5;10;38;5;21:st=48;5;21;38;5;15:ex=38;5;34:*.tar=38;5;9:*.tgz=38;5;9:*.arc=38;5;9:*.arj=38;5;9:*.taz=38;5;9:*.lha=38;5;9:*.lz4=38;5;9:*.lzh=38;5;9:*.lzma=38;5;9:*.tlz=38;5;9:*.txz=38;5;9:*.tzo=38;5;9:*.t7z=38;5;9:*.zip=38;5;9:*.z=38;5;9:*.Z=38;5;9:*.dz=38;5;9:*.gz=38;5;9:*.lrz=38;5;9:*.lz=38;5;9:*.lzo=38;5;9:*.xz=38;5;9:*.bz2=38;5;9:*.bz=38;5;9:*.tbz=38;5;9:*.tbz2=38;5;9:*.tz=38;5;9:*.deb=38;5;9:*.rpm=38;5;9:*.jar=38;5;9:*.war=38;5;9:*.ear=38;5;9:*.sar=38;5;9:*.rar=38;5;9:*.alz=38;5;9:*.ace=38;5;9:*.zoo=38;5;9:*.cpio=38;5;9:*.7z=38;5;9:*.rz=38;5;9:*.cab=38;5;9:*.jpg=38;5;13:*.jpeg=38;5;13:*.gif=38;5;13:*.bmp=38;5;13:*.pbm=38;5;13:*.pgm=38;5;13:*.ppm=38;5;13:*.tga=38;5;13:*.xbm=38;5;13:*.xpm=38;5;13:*.tif=38;5;13:*.tiff=38;5;13:*.png=38;5;13:*.svg=38;5;13:*.svgz=38;5;13:*.mng=38;5;13:*.pcx=38;5;13:*.mov=38;5;13:*.mpg=38;5;13:*.mpeg=38;5;13:*.m2v=38;5;13:*.mkv=38;5;13:*.webm=38;5;13:*.ogm=38;5;13:*.mp4=38;5;13:*.m4v=38;5;13:*.mp4v=38;5;13:*.vob=38;5;13:*.qt=38;5;13:*.nuv=38;5;13:*.wmv=38;5;13:*.asf=38;5;13:*.rm=38;5;13:*.rmvb=38;5;13:*.flc=38;5;13:*.avi=38;5;13:*.fli=38;5;13:*.flv=38;5;13:*.gl=38;5;13:*.dl=38;5;13:*.xcf=38;5;13:*.xwd=38;5;13:*.yuv=38;5;13:*.cgm=38;5;13:*.emf=38;5;13:*.axv=38;5;13:*.anx=38;5;13:*.ogv=38;5;13:*.ogx=38;5;13:*.aac=38;5;45:*.au=38;5;45:*.flac=38;5;45:*.mid=38;5;45:*.midi=38;5;45:*.mka=38;5;45:*.mp3=38;5;45:*.mpc=38;5;45:*.ogg=38;5;45:*.ra=38;5;45:*.wav=38;5;45:*.axa=38;5;45:*.oga=38;5;45:*.spx=38;5;45:*.xspf=38;5;45:"
declare -x MAIL="/var/spool/mail/root"
declare -x MANPATH="/opt/rh/devtoolset-9/root/usr/share/man:"
declare -x MAVEN_HOME="/local/dev/maven/apache-maven-3.6.3"
declare -x MYSQL="/local/dev/mysql/mysql-8.0.11-linux-glibc2.12-x86_64"
declare -x OLDPWD
declare -x PATH="/opt/rh/devtoolset-9/root/usr/bin:/opt/lampp/bin:/local/dev/node/node-v14.11.0-linux-x64/bin:/local/dev/maven/apache-maven-3.6.3/bin:/local/dev/mysql/mysql-8.0.11-linux-glibc2.12-x86_64/bin:/local/dev/java/jdk1.8.0_161/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/root/bin"
declare -x PCP_DIR="/opt/rh/devtoolset-9/root"
declare -x PKG_CONFIG_PATH="/opt/rh/devtoolset-9/root/usr/lib64/pkgconfig"
declare -x PS1="\\e[0;32m[\\u@\\h \\W \\A]\\\$ \\e[m"
declare -x PWD="/root"
declare -x SHELL="/bin/bash"
declare -x SHLVL="1"
declare -x SSH_CLIENT="180.103.137.122 59647 22"
declare -x SSH_CONNECTION="180.103.137.122 59647 172.16.252.76 22"
declare -x SSH_TTY="/dev/pts/5"
declare -x TERM="xterm-256color"
declare -x USER="root"
declare -x XDG_RUNTIME_DIR="/run/user/0"
declare -x XDG_SESSION_ID="19395"
```
```bash
[root@iZuligp6e1dyzfZ ~ 09:07]# echo $PATH
/opt/rh/devtoolset-9/root/usr/bin:/opt/lampp/bin:/local/dev/node/node-v14.11.0-linux-x64/bin:/local/dev/maven/apache-maven-3.6.3/bin:/local/dev/mysql/mysql-8.0.11-linux-glibc2.12-x86_64/bin:/local/dev/java/jdk1.8.0_161/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/root/bin
```
其中PATH变量定义了运行命令的查找路径，以冒号`:`分割不同的路径，使用`export`定义的时候可加双引号也可不加。
<a name="Fri5e"></a>
### Linux环境变量配置方法一：`export PATH`
使用`export`命令直接修改PATH的值，配置MySQL进入环境变量的方法:
```bash
export PATH=/home/uusama/mysql/bin:$PATH
# 或者把PATH放在前面
export PATH=$PATH:/home/uusama/mysql/bin
```
注意事项：

- 生效时间：立即生效
- 生效期限：当前终端有效，窗口关闭后无效
- 生效范围：仅对当前用户有效
- 配置的环境变量中不要忘了加上原来的配置，即$PATH部分，避免覆盖原来配置
<a name="4nwvV"></a>
### Linux环境变量配置方法二：`vim ~/.bashrc`
通过修改用户目录下的`~/.bashrc`文件进行配置：
```bash
vim ~/.bashrc
# 在最后一行加上
export PATH=$PATH:/home/uusama/mysql/bin
```
注意事项：

- 生效时间：使用相同的用户打开新的终端时生效，或者手动`source ~/.bashrc`生效
- 生效期限：永久有效
- 生效范围：仅对当前用户有效
- 如果有后续的环境变量加载文件覆盖了PATH定义，则可能不生效
<a name="TZC5b"></a>
### Linux环境变量配置方法三：`vim ~/.bash_profile`
和修改`~/.bashrc`文件类似，也是要在文件最后加上新的路径即可：
```bash
vim ~/.bash_profile
# 在最后一行加上
export PATH=$PATH:/home/uusama/mysql/bin
```
注意事项：

- 生效时间：使用相同的用户打开新的终端时生效，或者手动`source ~/.bash_profile`生效
- 生效期限：永久有效
- 生效范围：仅对当前用户有效
- 如果没有`~/.bash_profile`文件，则可以编辑`~/.profile`文件或者新建一个
<a name="UUlnf"></a>
### Linux环境变量配置方法四：`vim /etc/bashrc`
该方法是修改系统配置，需要管理员权限（如root）或者对该文件的写入权限：
```bash
# 如果/etc/bashrc文件不可编辑，需要修改为可编辑
chmod -v u+w /etc/bashrc
vim /etc/bashrc
# 在最后一行加上
export PATH=$PATH:/home/uusama/mysql/bin
```
注意事项：

- 生效时间：新开终端生效，或者手动`source /etc/bashrc`生效
- 生效期限：永久有效
- 生效范围：对所有用户有效
<a name="2CZ9j"></a>
### Linux环境变量配置方法五：`vim /etc/profile`
该方法修改系统配置，需要管理员权限或者对该文件的写入权限，和`vim /etc/bashrc`类似：
```bash
# 如果/etc/profile文件不可编辑，需要修改为可编辑
chmod -v u+w /etc/profile
vim /etc/profile
# 在最后一行加上
export PATH=$PATH:/home/uusama/mysql/bin
```
注意事项：

- 生效时间：新开终端生效，或者手动`source /etc/profile`生效
- 生效期限：永久有效
- 生效范围：对所有用户有效
<a name="O5iKJ"></a>
### Linux环境变量配置方法六：`vim /etc/environment`
该方法是修改系统环境配置文件，需要管理员权限或者对该文件的写入权限：
```bash
# 如果/etc/bashrc文件不可编辑，需要修改为可编辑
chmod -v u+w /etc/environment
vim /etc/profile
# 在最后一行加上
export PATH=$PATH:/home/uusama/mysql/bin
```
注意事项：

- 生效时间：新开终端生效，或者手动`source /etc/environment`生效
- 生效期限：永久有效
- 生效范围：对所有用户有效
<a name="nfnL3"></a>
### Linux环境变量加载原理解析
上面列出了环境变量的各种配置方法，那么Linux是如何加载这些配置的呢？是以什么样的顺序加载的呢？<br />特定的加载顺序会导致相同名称的环境变量定义被覆盖或者不生效。
<a name="tgZfk"></a>
#### 环境变量的分类
环境变量可以简单的分成用户自定义的环境变量以及系统级别的环境变量。

- 用户级别环境变量定义文件：`~/.bashrc`、`~/.profile`（部分系统为：`~/.bash_profile`）
- 系统级别环境变量定义文件：`/etc/bashrc`、`/etc/profile`(部分系统为：`/etc/bash_profile`）、`/etc/environment`

另外在用户环境变量中，系统会首先读取~/.bash_profile（或者~/.profile）文件，如果没有该文件则读取~/.bash_login，根据这些文件中内容再去读取~/.bashrc。
<a name="dfa5y"></a>
#### 测试Linux环境变量加载顺序的方法
为了测试各个不同文件的环境变量加载顺序，在每个环境变量定义文件中的第一行都定义相同的环境变量UU_ORDER，该变量的值为本身的值连接上当前文件名称。<br />需要修改的文件如下：

- /etc/environment
- /etc/profile
- /etc/profile.d/test.sh，新建文件，没有文件夹可略过
- /etc/bashrc，或者/etc/bash.bashrc
- ~/.bash_profile，或者~/.profile
- ~/.bashrc

在每个文件中的第一行都加上下面这句代码，并相应的把冒号后的内容修改为当前文件的绝对文件名。
```bash
export UU_ORDER="$UU_ORDER:~/.bash_profile"
```
修改完之后保存，新开一个窗口，然后echo $UU_ORDER观察变量的值：
```bash
uusama@ubuntu:~$ echo $UU_ORDER
$UU_ORDER:/etc/environment:/etc/profile:/etc/bash.bashrc:/etc/profile.d/test.sh:~/.profile:~/.bashrc
```
可以推测出Linux加载环境变量的顺序如下：<br />1、/etc/environment<br />2、/etc/profile<br />3、/etc/bash.bashrc<br />4、/etc/profile.d/test.sh<br />5、~/.profile<br />6、~/.bashrc
<a name="XUJXB"></a>
### Linux环境变量文件加载详解
由上面的测试可容易得出Linux加载环境变量的顺序如下：<br />系统环境变量 -> 用户自定义环境变量 /etc/environment -> /etc/profile -> ~/.profile<br />打开/etc/profile文件可以发现，该文件的代码中会加载/etc/bash.bashrc文件，然后检查/etc/profile.d/目录下的.sh文件并加载。
```bash
# /etc/profile: system-wide .profile file for the Bourne shell (sh(1))
# and Bourne compatible shells (bash(1), ksh(1), ash(1), ...).
if [ "$PS1" ]; then
  if [ "$BASH" ] && [ "$BASH" != "/bin/sh" ]; then
    # The file bash.bashrc already sets the default PS1.
    # PS1='\h:\w\$ '
    if [ -f /etc/bash.bashrc ]; then
      . /etc/bash.bashrc
    fi
  else
    if [ "`id -u`" -eq 0 ]; then
      PS1='# '
    else
      PS1='$ '
    fi
  fi
fi
if [ -d /etc/profile.d ]; then
  for i in /etc/profile.d/*.sh; do
    if [ -r $i ]; then
      . $i
    fi
  done
  unset i
fi
```
其次再打开~/.profile文件，会发现该文件中加载了~/.bashrc文件。
```bash
# if running bash
if [ -n "$BASH_VERSION" ]; then
    # include .bashrc if it exists
    if [ -f "$HOME/.bashrc" ]; then
  . "$HOME/.bashrc"
    fi
fi
# set PATH so it includes user's private bin directories
PATH="$HOME/bin:$HOME/.local/bin:$PATH"
```
从`~/.profile`文件中代码不难发现，`/.profile`文件只在用户登录的时候读取一次，而`/.bashrc`会在每次运行Shell脚本的时候读取一次。
<a name="2KaUW"></a>
### 一些小技巧
可以自定义一个环境变量文件，比如在某个项目下定义fcant.profile，在这个文件中使用`export`定义一系列变量，然后在`~/.profile`文件后面加上：`sourc fcant.profile`，这样每次登陆都可以在Shell脚本中使用自己定义的一系列变量。<br />也可以使用alias命令定义一些命令的别名，比如`alias rm="rm -i"`（双引号必须），并把这个代码加入到`~/.profile`中，这样每次使用`rm`命令的时候，都相当于使用`rm -i`命令，非常方便。
<a name="cx2XX"></a>
### Shell中常用的环境变量
<a name="GeYAT"></a>
#### HOME—用户的主工作目录
即为用户登录到Linux系统中时的默认目录。
```bash
$ whoami
root
$ echo $HOME
/root
```
<a name="EHSQG"></a>
#### HISTSIZE—保存历史命令的条数
输入的指令都会被系统保存下来，这个环境变量记录的就是保持指令的条数。一般为1000。
```bash
$ echo $HISTSIZE
1000
$ HISTSIZE=1001
$ echo $HISTSIZE
1001
```
历史命令是被保存在内存中的，当退出或者登录shell时，会自动保存或读取可以通过history命令来查看。可以使用符号 `!` 执行指定序号的历史命令。例如，要执行第2个历史命令，则输入!2。
```bash
$ history 5
   59  ls
   60  who
   61  history | head -n 5
   62  who am i
   63  history 5
$ !62
who am i
root     pts/0        2018-04-04 11:00 (101.81.92.18)
```
<a name="93BmT"></a>
#### LOGNAME—当前用户登录名
```bash
$ echo $LOGNAME
root
```
<a name="LGk7I"></a>
#### HOSTNAME —主机名称
```bash
$ echo $HOSTNAME
iZuligp6e1dyzfZ
```
<a name="Y4bVu"></a>
#### SHELL—当前用户使用的shell种类
```bash
$ echo $SHELL
/bin/bash
```
