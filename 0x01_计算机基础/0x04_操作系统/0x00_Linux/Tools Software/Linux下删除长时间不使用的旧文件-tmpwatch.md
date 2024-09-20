Linux 删除长时间不使用的旧文件 tmpwatch<br />系统里面常常会有一些忘记删除的长时间不用而且确实没有用处的文件，如果不去处理，这些无用的文件会越来越多，浪费许多系统资源。在不知道文件名的情况下，很难去检查某一目录下到底是哪些文件长时间没有被访问，同时删除这些文件。介绍一款非常实用的工具来完成这一工作——tmpwatch
<a name="uhSa3"></a>
## tmpwatch 是什么
tmpwatch 能够循环地删除指定目录下指定时间内没有被访问的文件，这一命令常常用于清理临时文件目录，比如 `/tmp` 或者 `/var/tmp` 这类目录。<br />它只清除指定目录下的空目录、普通文件和符号链接文件，也不会影响其他目录，而且会避开那些属于root用户的系统相关关键文件的。<br />默认设置下，`tmpwatch` 命令依据文件的 `atime` （access time）而非 `mtime` （modify time）来删除文件。如果想改变它的删除依据，可以在使用这个命令时加上想修改的参数。
:::danger
**注意**：千万不要在根目录底下运行 `tmpwatch` 或者 `tmpreaper` 命令，因为系统可没有任何机制阻止在根目录下运行此命令。
:::
<a name="76760b16"></a>
## 在Linux上安装 tmpwatch
各种 Linux 发行版本的安装命令都有些略微差别，具体的安装方法如下。<br />Linux 发行版本是 REHL/CentOS 6 的话，可以使用 `yum` 命令来安装 tmpwatch，示例如下：
```bash
$ sudo yum install -y tmpwatch
```
如果使用的 Linux 发行版本是 Debian 或者 Ubuntu ，则需要使用 `apt` 或者 `apt-get` 命令来安装 Tmpreaper，示例如下：
```bash
$ sudo apt-get install tmpreaper
```
如果使用的是 openSUSE，则应使用 `zypper` 命令安装 tmpwatch：
```bash
$ sudo zypper install -y tmpwatch
```
如果 Linux 发行版本是 Fedora，则应使用 `dnf` 命令安装 tmpwatch：
```bash
$ sudo dnf install -y tmpwatch
```
:::tips
**说明**：在以 Debian 为基础的 Linux 系统，是用 `tmpreaper` 命令代替 `tmpwatch` 的，若使用的系统是以 Debian 为基础的Linux 版本，则将下文的示例中的 `tmpwatch` 换为 `tmpreaper` 即可。
:::
<a name="OKvUA"></a>
## `tmpwatch` 命令的关键选项和参数

- `atime` (File Last Access Time)：文件最后一次被访问的时间；
- `mtime` (File Last Modify Time)：文件内容最后一次被修改的时间；
- `ctime` (File Last Change Time)：文件元数据最后一次被修改的时间，即文件相关属性被修改的时间，多数情况下 mtime 和 ctime 值相同，但是诸如文件所有者、权限、所属组这类不涉及内容的属性被修改时则只会影响 ctime；
- `dirmtime` (Directory Last modification time)：目录最后一次被修改的时间。

这些时间参数用来设置删除文件的条件阈值：

- `d`：单位为天；
- `h`：单位为小时；
- `m`：单位为分钟；
- `s`：单位为秒。
<a name="5R1Hm"></a>
## 用 `tmpwatch` 命令删除一段时间内没有被访问的文件
如前所述，`tmpwatch` 命令的默认选项是 `atime`，而默认的单位参数则是 `h`，所以如果确实要按以小时为单位计算的访问时间来删除文件，那么不用加任何特殊的选项或则参数后缀，可以直接是用这个命令。如下例所示，即为删除 `/tmp` 目录下过去5小时内没有被访问的文件：
```bash
tmpwatch 5 /tmp
```
下面这个示例是删除 `/home/daygeek/Downloads` 目录下过去十小时内没有修改过内容的文件，注意，如果要按 `mtime` 来删除文件，需要在命令中加上 `-m` 的选项：
```bash
tmpwatch -m 10 /home/daygeek/Downloads
```
<a name="dKn3w"></a>
### 删除以其他单位计算的某段时间内没有被访问的文件
如果要以天为单位，则需要加上 `d` 的后缀，如下为删除30天内没有被访问的文件：
```bash
tmpwatch 30d /home/daygeek/Downloads
```
<a name="hTEUQ"></a>
### 删除一段时间内未被使用的所有文件
如果想不仅仅删除普通文件、符号链接文件、空目录文件，而是想删除指定目录下某段时间内没有被访问的所有文件，则需要加上选项 `-a`，如下为删除指定目录下12小时未被修改内容的所有文件：
```bash
tmpwatch -am 12 /tmp
```
<a name="vjsJr"></a>
### 将某些目录排除在删除操作外
如下命令可以让那些十小时内没有被修改过内容的目录不被删除：
```bash
tmpwatch -am 10 --nodirs /home/daygeek/Downloads
```
<a name="4FWRH"></a>
### 将特定路径排除在删除操作外
下面的命令删除 `/home/daygeek/Downloads` 目录下所有十小时内没有修改内容的文件，但是 `/home/daygeek/Downloads/Movies` 路径下却不受影响，即该路径下十小时内没修改的文件也不会被删除：
```bash
tmpwatch -am 10 --exclude=/home/daygeek/Downloads/Movies /home/daygeek/Downloads
```
<a name="KXMgo"></a>
### 将特定格式的文件排除在删除操作外
下面所示的命令为删除指定文件下的所有10小时内未被改动的文件，除了 `pdf` 格式的文件：
```bash
tmpwatch -am 10 --exclude-pattern='*.pdf' /home/daygeek/Downloads
```
<a name="iG9CV"></a>
### 预演 `tmpwatch` 的效果
下面这条命令即是对 `tmpwatch` 的功能效果进行预演：
```bash
tmpwatch -t 5h /home/daygeek/Downloads
```
<a name="lOFmq"></a>
### 用 `tmpwatch` 设置一个定时任务周期性地执行删除操作
要完成这个任务，会在 `/etc/cron.daily/tmpreaper` 目录下留下一个 `cronjob` 文件，这个文件是按照 `/etc/timereaper.conf` 的设定工作的，可以按自己的需求设置它。<br />如下所示的设置，能在每天上午10点时删除指定目录下，十五天没被访问的文件：
```bash
# crontab -e
0 10 * * * /usr/sbin/tmpwatch 15d /home/daygeek/Downloads
```
