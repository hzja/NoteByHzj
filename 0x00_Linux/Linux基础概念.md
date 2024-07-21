Linux<br />Linux 和 UNIX 中的文件系统是一个以 _/_ 为根的树状式文件结构，/ 是 Linux 和 UNIX 中的根目录，同样它也是文件系统的起点。所有的文件和目录都位于 / 路径下，包括经常听到的 /usr、/etc、/bin、/home 等。在早期的 UNIX 系统中，各个厂家都定义了自己文件系统的命名构成，比较混乱，而且难以区分。<br />为了避免在 Linux 系统上也出现这种**命名混乱**的问题，在 1994 年推出了 FSSTND(FileSystem Standard) 的 Linux 文件系统层次结构标准，后来 UNIX 团队把 FSSTND 发扬光大，成为了后来的 FHS(FileSystem Hierarchy Standard) 。<br />FHS 标准使得众多的 Linux distributions(Linux 发行版) 有了统一的文件系统命名标准，换一种说法：FHS 就是一种文件系统的命名标准。一般来说，Linux distributions 都需要遵循 FHS 规定的

- 目录结构和目录内容
- 文件类型的权限

但是，FHS 仅仅定义了两层规范，第一层是 / 下面的各个目录要放什么文件数据，比如 _/etc_ 下面需要放设置文件，_/bin_ 和 _/sbin_ 下面需要放可执行文件等；第二层是针对 _/usr_ 和 _/var_ 这两个目录来定义的，比如 _/usr/share_ 需要放共享数据，_/var/log_需要放系统登录文件等。<br />下面就来为看一下 Linux 在 _/_ 下都有哪些定义和规范。
<a name="EG8vA"></a>
## FHS 规范
下面介绍的这些规范，只是 FHS 文档指定的目录和文件的一部分，完整的标准可在 [http://www.pathname.com/fhs/](http://www.pathname.com/fhs/) 在线获得。<br />首先先来介绍一下根目录的重要性和意义：<br />根目录是整个系统中最重要的一个目录，后续的目录都是以根目录为基础进行衍生，更重要的一点，**根目录与开机、还原、系统修复**有关，由于系统开机需要特定的环境，所以系统出现错误时，根目录也必须要包含有能够修复文件系统的程序才行。所以 FHS 规定不要将根目录的分区大小设置的过大，实际上是越小越好，以减少出错率。<br />FHS 同样规定了根目录下面应该要有这些目录的存在。
<a name="mYDDs"></a>
### /home 目录
_/home_ 目录是系统默认的使用者主文件夹（home directory）。
<a name="I8hmt"></a>
### /boot 目录
/boot 目录包含启动操作系统所需的静态文件，比如 Linux 内核，这些文件对系统的启动至关重要。_Linux Kernel_ 常用的文件名为 _vmlinuz_, 但是如果使用的是 _grub2_ 这个开机程序，还会存在 /boot/grub2 这个目录。<br />boot 下面是这样的。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1645062437839-c40e155d-9466-48d8-a30d-2743a6d7fafa.png#clientId=ud88a5c44-3718-4&from=paste&height=551&id=u5df76a4e&originHeight=1377&originWidth=2182&originalType=binary&ratio=1&rotation=0&showTitle=false&size=302906&status=done&style=none&taskId=ue88f3f58-33d1-4270-8ecb-48921b16f04&title=&width=872.8)
:::danger
千万不要删除 /boot 目录，这样做可能会导致系统无法启动。
:::
<a name="EAyj0"></a>
### /dev 目录
_/dev_ 目录都是一些设备节点，这些设备节点是 Linux 系统中的设备或者是内核提供的虚拟设备。这些设备节点同样也对系统正常运行至关重要。/dev 目录和子目录下的设备是字符设备和块设备。字符设备就是**鼠标、键盘、调制解调器**，块设备就是**硬盘、软盘驱动器**。存储 /dev 目录下的文件就相当于是存储某个设备。<br />比较重要的文件有<br />_/dev/hda_：主 IDE 通道上的主设备，当提到 IDE/ATA 硬盘时，主通道是第一和第二驱动器连接到主板或接口卡的通道。<br />_/dev/hdb_：主 IDE 通道上的从设备。<br />_/dev/tty0_：第一个虚拟控制台。虚拟控制台又被称为 VC，也称为虚拟终端 VT，是用于计算机用户界面的键盘和显示器的概念组合。<br />_/dev/tty1_：第二个虚拟控制台。<br />_/dev/sda_：主 SCSI 和 SATA 通道上的第一个设备。<br />_/dev/lp0_：第一个并行端口。<br />下面是 dev 下面的目录。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1645062496586-a511d2b0-970b-42f0-bf6c-b890f1ecc724.png#clientId=ud88a5c44-3718-4&from=paste&height=357&id=u83aae0c3&originHeight=892&originWidth=2167&originalType=binary&ratio=1&rotation=0&showTitle=false&size=159367&status=done&style=none&taskId=u5346f4e0-eec7-4cc0-bcc2-42d97aa2365&title=&width=866.8)
<a name="ppXsT"></a>
### /etc 目录
/etc 目录是为计算机本地的配置文件保留的，系统主要的配置文件都放在这个目录下，比如账号密码，服务的启停，一般来说，这个目录下面一般用户只有读权限，只有 root 用户具有修改权限<br />/etc 下比较重要的几个文件有<br />_/etc/modprobe.d _内核驱动模块<br />_/etc/passwd _存储用户密码<br />_/etc/fstab_ 存储挂载信息<br />_/etc/issue _存储系统启动显示的内容<br />/etc 目录下不要存放二进制文件，二进制文件最好放在 _/sbin_ 和 _/bin_ 中。<br />/etc 目录下有两个重要的文件，_/etc/x11_ 用于存放 X Window 系统配置文件，例如 xorg.conf，_/etc/opt_ 这个主要存放第三方协力软件 /opt 的相关配置文件。
<a name="b9Ner"></a>
### /lib 目录
系统的函数库有很多，而 /lib 目录就像一个仓库，它用于存放执行 _/bin_ 和 _/sbin_ 中二进制文件所需要的库，这些共享库映像对于系统 boot 和执行根文件系统中的命令特别重要。
<a name="YRedc"></a>
### /media 目录
media 媒体的英文，也就是说 _/media_下可以存放用于可移动媒体（例如 USB、DVD、CD-ROM 和 Zip 磁盘）的安装点和子目录。
<a name="uXjeA"></a>
### /mnt 目录
如果想要暂时挂载某些额外的设备，可以放在 /mnt 这个目录下，其实最早的时候，/mnt 用途和 /media 一样，只是后来有了 /media 之后，这个目录就被用来临时挂载了。
<a name="Hs6X0"></a>
### /opt 目录
_/opt/_ 目录为大多数应用程序软件包提供存储空间，将文件放置在 /opt/ 目录中的包会创建一个与包同名的目录。反过来，该目录保存了原本会分散在整个文件系统中的文件，从而为系统管理员提供了一种简单的方法来确定特定包中每个文件的角色。<br />例如，如果 sample 是位于 /opt/ 目录中的特定软件包的名称，那么所有文件都放置在 /opt/sample/ 目录中的目录中，例如 /opt/sample/bin/ 表示二进制文件 和 /opt/sample/man/ 用于手册页。
<a name="XgMr2"></a>
### /proc 目录
_/proc_ 目录本身是一个**虚拟文件系统(virtual filesystem)**，他放置的数据是在内存中，比如系统核心信息，行程信息，周边设备网络状态等。因为这个目录下的数据都是在内存中的，所以本身不占用磁盘空间。<br />这个目录下比较重要的文件有 _/proc/cpuinfo_、_/proc/dma_、_/proc/interrupts_、_/proc/net/_。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1645062554671-37ec8011-bcb6-48d0-9d9c-5c2b854e8b18.png#clientId=ud88a5c44-3718-4&from=paste&height=377&id=u24caf3a8&originHeight=942&originWidth=2142&originalType=binary&ratio=1&rotation=0&showTitle=false&size=168268&status=done&style=none&taskId=u5a485072-952f-4711-b7e4-6e10d0658de&title=&width=856.8)
<a name="F5TfG"></a>
### /sbin 目录
_/sbin_ 目录存储 root 用户使用的可执行文件。/sbin 中的可执行文件在启动时使用，用于系统管理和执行系统恢复操作。除了 _/bin_ 中的二进制之外，/sbin 还包含启动、恢复和修复系统所需要的二进制文件。<br />/sbin 目录下包含 _arp、clock、halt、init、grub、ifconfig、route、shutdown_ 等。
<a name="UWqna"></a>
### /tmp 目录
这个目录下面存放的都是一些临时文件，这些文件在系统重新启动时被清除。
<a name="QOMZD"></a>
### /sys 目录
这个目录其实和 /proc 非常类似，也是一个虚拟文件系统，主要是记录核心与系统硬件相关信息。这个目录同样也不占用磁盘空间。
<a name="nvwQg"></a>
### /usr 目录
/usr 目录是需要好好聊聊的一个目录了，很多读者都误以为 /usr 是 user 的缩写，其实 usr 是 _Unix Software Resource_ 的缩写，FHS 建议软件开发者应该将数据合理的放置在这个目录的次目录下，不要自己创建软件独立的目录。<br />所有系统默认的软件都会放在 /usr 下面，因此这个目录有点类似 Windows 中的 _C:\Windows\ + C:\Program files\_ 这两个目录的综合，一般来说，系统刚安装完毕后，这个目录会占用较多的磁盘容量，/usr 的次目录建议有下面这些内容<br />*/usr/bin/*一般用户能够使用的指令都放在这个目录下，CentOS 7 开始，/usr/bin 下指令和 /bin 目录下指令一毛一样。<br />*/usr/lib/*基本上，与 /lib 功能相同，所以 /lib 就是链接到这个目录中的。<br />_/usr/local_ 系统管理员在在本机下载的软件都建议安装到这个目录下，便于管理。<br />_/usr/sbin_ 非系统正常运行所需要的系统指令，最常见的就是某些网络服务器软件的服务指令，不过基本功能与 /sbin 差不多，因此目前 /sbin 就是链接到此目录中的。<br />_/usr/share_ 共享文件放置区，这个目录下还有这些次目录 /usr/share/man ，/usr/share/doc，/usr/share/zoneinfo<br />_/usr/games_ 与游戏相关的数据放置处<br />_/usr/include_ C/C++ 等程序语言的 header 和 include 放置处，也是直接链接的。<br />/usr/src 一般源码建议放在这，src 是 source 的意思，至于核心源码则建议放在 /usr/src/linux 目录下。
<a name="y1xrk"></a>
### /run 目录
早期的 FHS 规定系统开机后所产生的各项信息应该要放置到 /var/run 目录下，新版的 FHS 则规范到 /run 下面。
<a name="VMFj6"></a>
### /var 目录
如果说 /usr 是安装时会占用较大磁盘容量的目录，那么 _/var_就是在系统运行后才会渐渐占用磁盘容量的目录。/var 主要包括高速缓存、登录文件以及某些软件运行所产生的的目录，包括程序文件，例如 MySQL 数据库文件等，常见的目录有<br />_/var/cache_ 应用程序缓存目录<br />_/var/crash_ 系统错误信息<br />_/var/log_ 日志记录<br />_/var/run_ 进程的标识数据<br />_/var/tmp_ 临时文件目录<br />_/var/lock_ 文件锁定记录
<a name="XGLAd"></a>
### /srv 目录
srv 可以看做 _service_ 的缩写，是一些网络服务启动之后，这些服务所需要取用的数据目录，常见的服务例如 WWW, FTP 。<br />CentOS 7 在目录的编排上与过去的版本不同，上面已经大致描述过了，这里做个汇总，比较大的差异就是将许多原本应该在根目录里面的目录，将全部数据挪到了 _/usr_ 里面去，然后进行链接设置，这些内容包括

- /bin -> /usr/bin
- /sbin -> /usr/sbin
- /lib -> /usr/lib
- /lib64 -> /usr/lib64
- /var/lock -> /run/lock
- /var/run -> /run

为了方便大家直观的看到这些目录都是做什么的，手绘了一个图，图中的这些目录之间的关系很像一棵树，所以这个目录又叫做目录树。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1645062344283-acf6dc11-19c1-4bfb-ae1f-ddd5f7511c77.webp#clientId=ud88a5c44-3718-4&from=paste&id=ud1bd7e80&originHeight=665&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ucd103be7-0850-48bf-949b-664e88cf9da&title=)<br />这个目录树已经很清晰了。
<a name="WhT1L"></a>
## 绝对路径和相对路径
和其他系统一样，Linux 中的路径也分为绝对路径（absolute）和相对路径（relative），这两种的写法依据如下：

- 由根目录开始的路径都是绝对路径，比如上图目录树中的 /boot、/usr/local 等
- 相对于当前文件路径的写法是相对路径，比如 ../var/log 目录

这里需要注意一点，相对路径是以当前所在路径的相对位置来移动的，比如当前在 /boot 这个目录下，想要移动到 /usr/src 这个目录<br />绝对路径表示法：/usr/src<br />相对路径表示法：../usr/src<br />这里还需要注意两个特殊的目录：

- . ：代表当前目录，也可以使用 ./ 来表示
- .. ：代表上一层目录，也可以用 ../ 来表示

关于绝对路径和相对路径的取舍问题：当文件路径很长时，推荐使用相对路径，因为不用向绝对路径一样，把所有的路径都写一遍，而直接使用 ../ 即可；<br />但是从文件路径的正确性来说，绝对路径要比相对路径要好。<br />如果是在写程序来管理系统，务必使用绝对路径，因为绝对路径不管文件路径在哪，都会找到指定的文件；而相对路径如果文件移动后，就可能会存在找不到文件的情况。
<a name="WnyZi"></a>
## 与目录有关的指令
下面就来看看和目录相关的几条指令
<a name="ejUYN"></a>
### cd
cd 就是 change directory 的缩写，这条指令能够切换当前工作目录，有下面四种切换方式<br />cd /路径名 # 表示切换到指定的工作目录<br />cd ./   # 表示切换到当前工作目录(无任何切换)<br />cd ..   # 表示切换到当前工作目录的上一层目录<br />cd ~    # 表示返回到当前用户目录<br />下面来演示一下这四个 cd 切换方式的操作<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1645062718467-038b0a9d-a463-47c7-bbc1-bc9a16b066ef.png#clientId=ud88a5c44-3718-4&from=paste&height=320&id=u26a0ab38&originHeight=800&originWidth=1450&originalType=binary&ratio=1&rotation=0&showTitle=false&size=104881&status=done&style=none&taskId=u1164dea6-3816-4c72-a959-726b1605426&title=&width=580)<br />可以看到，分别使用 cd /usr/local 切换到了指令目录下；使用 cd ./ 没有切换任何目录；使用 cd .. 切换到了当前工作目录的上一级；使用 cd ~ 切换到了当前登录用户的默认工作目录。<br />这里需要注意的就是 cd ~ ，如果直接使用 cd ，切换效果和 cd ~ 一样，也就是说 "cd ~ == cd "。
<a name="GCddw"></a>
### pwd
上面还有一条指令是 pwd ，这个指令是最常用的指令，它的使用频率几乎和 cd 一样。<br />pwd 是 Print Working Directory 的缩写，它是打印出当前所在目录的指令。<br />但是 pwd 不一定只用于显示当前工作路径，它还可以用来显示指定工作目录下的文件路径
<a name="z65TU"></a>
### mkdir
mkdir 用于创建新目录，它是 make directory 的缩写，一般来说，它后面会跟着两个参数。

- -m ：设置文件的权限，直接设置，不用考虑默认文件的创建权限
- -p ：递归创建目录

在 /tmp 文件目录下创建了一个叫做 test1 的空目录，并打印出了当前的工作目录。<br />如果想要创建不想让其他用户访问的目录，可以使用下面的命令
```bash
mkdir -m 711 test2
```
`ls -ld` 列出文件目录的详细信息，可以看到，详细信息最前面的是文件的权限。<br />一个一个创建目录太麻烦了，现在想要快速创建这些目录，比如想要创建 /test1/test2/test3/test4 这种目录格式，使用了下面代码
```bash
mkdir test1/test2/test3/test4
```
Linux 会提出不能使用这种创建方式。<br />此时就需要 `-p` 这个参数的出现了，这个参数能够允许递归创建目录，如下所示<br />只不过使用 `-p` 这种方式并不推荐使用，因为很容易打错字。
<a name="at62g"></a>
### rmdir
rmdir 用于删除空目录，它的使用和 mkdir 非常类似，`mkdir -p` 用于递归创建目录，同样的，`rmdir -p` 也能够递归删除目录。<br />这里需要注意，rmdir 只是删除空目录，这也意味着目录里面有内容的话是无法删除的，那该如何删除呢？可以直接使用 `rm -r` 来进行删除。
<a name="zzFp5"></a>
### mv
使用 mv 能够修改目录的名称，比如创建了一个叫做 test 的目录，后来觉得这个目录无法体现出来它的所有者，想要改一下它的名字，这时候就可以使用 mv 指令就可以修改。<br />在了解完基本的目录操作之后，下面来想一个问题，为什么能够在各种目录下执行 _ls_ 指令都能够显示内容？按理说 ls 不应该只是在其存放路径下才能够使用吗？<br />首先大家需要先了解一个 _PATH_ 的概念，PATH 是**环境变量**，当在执行一条指令的时候，首先系统会按照 PATH 的设置去每个 PATH 下搜寻文件名为 ls 的可执行文件，如果有多个可执行文件，那么优先搜索到的先被执行。
