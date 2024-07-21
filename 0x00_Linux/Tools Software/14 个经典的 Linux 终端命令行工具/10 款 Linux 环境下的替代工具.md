Linux
<a name="ZAqHT"></a>
## 1、bat
Bat是 cat 命令的替代品，具有一些例如语法高亮、 Git 集成和自动分页等非常酷的特性。
<a name="kc4Ul"></a>
### 安装 bat
<a name="DJDux"></a>
#### Arch Linux
bat 可以在 Arch Linux 操作系统的默认软件源中进行获取，可直接使用 pacman 命令在任何基于  Arch 的系统上来安装它。
```bash
pacman -S bat
```
<a name="l3Rco"></a>
#### Fedora
```bash
dnf install bat
```
<a name="SVPu9"></a>
#### macOS
```bash
brew install bat
```
<a name="t7Iru"></a>
#### Ubuntu 21.10
这里是通过 Ubuntu 环境下安装的 bat，从 bat 的发布页面下载`.deb`文件，可执行如下命令进行下载和安装。
```bash
sudo apt-get install wget
wget https://github.com/sharkdp/bat/releases/tag/v0.18.3/bat_0.18.3_amd64.deb
sudo dpkg -i bat_0.18.3_amd64.deb
```
对于其他系统，也许需要从软件源编译并安装，或参考 GitHub 上 bat 的其他操作系统安装方式。
<a name="y6Nom"></a>
### bat 命令的使用
bat 命令的使用与 cat 命令的使用非常相似。<br />使用 bat 命令创建一个新文件：
```bash
bat > jiaoben1.sh
```
使用 bat 命令查看文件内容:
```bash
bat jiaoben1.sh
```
使用 bat 命令同时查看多个文件：
```bash
bat jiaoben1.sh jiaoben2.sh
```
使用 bat 命令将多个文件的内容合并到一个单独的文件中：
```bash
bat jiaoben1.sh jiaoben2.sh > test.txt
```
上述介绍了 bat 创建新文件、查看文件内容、将多个文件的内容合并到一个单独的文件中的方式。<br />接下来，就来看一下 bat 命令一些非常酷的特性。<br />bat 命令支持大多数编程和标记语言的语法高亮，那么就来使用 cat 和 bat 命令展示下 jiaoben1.sh 里面的内容做个鲜明的对比。<br />![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1643944421959-d8f07def-ba49-4074-93b4-ce0ec53bc6b4.gif#clientId=u594a4997-d237-4&from=paste&id=u939c910f&originHeight=649&originWidth=733&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1336f975-376a-4caf-9dea-ee9e7fc7fb8&title=)<br />可以看出，cat 命令以纯文本格式显示文件的内容，而 bat 命令显示了语法高亮和整齐的文本对齐格式。<br />从上面 bat 命令的输出，可以看出是有表格线的，如果只想显示行号（而没有表格）可以使用`-n`参数来实现，虽然 cat 执行也会显示行号，但还是没有 bat 执行输出后的效果美观。
```bash
bat -n jiaoben2.sh
```
![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1643944421914-8d71d2c5-87c1-47ff-8bbb-3fbab84585f4.gif#clientId=u594a4997-d237-4&from=paste&id=ubd0032ac&originHeight=517&originWidth=787&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u64cb9e3e-2627-440b-a1b5-b00f7f96d2b&title=)<br />bat 命令支持 Git 集成，可以轻松查看和编辑 Git 存储库中的文件，bat 与 Git 连接可显示关于索引的修改。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643944422194-395fa221-a399-415f-a980-7d8254bfeeeb.webp#clientId=u594a4997-d237-4&from=paste&id=u3b4364ef&originHeight=327&originWidth=656&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua476fbde-8e20-4c57-b2f9-69595248ef7&title=)<br />bat 支持自动分页，这点特性也是需要值得注意的。<br />支持分页意思也就是说，当文件的输出内容超出屏幕显示的时候，bat 命令会自动将自己输出的内容传到 less 命令中，将可以使用回车键一页一页的查看输出内容，使用空格键可直接跳转至文末最后一行。<br />当使用 bat 命令时这些都是不必要的，bat 命令将自动传输跨越多个页面的文件的输出。
```bash
bat jiaoben3.sh
```
![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1643944422005-ec1792ab-b933-4df0-942d-bdb2531495d2.gif#clientId=u594a4997-d237-4&from=paste&id=u8eb6ebe3&originHeight=432&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u637f377b-88be-4766-9e29-e2201cd87cf&title=)<br />对比下 cat 命令，查看 jiaoben3.sh 跨多个页面文件内容时，提示符会直接跳到文件的最后一页，将无法看到跨多个页面文件开头及中间部分的内容。
```bash
cat jiaoben3.sh
```
![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1643944421937-7f8ace8f-4310-49c9-a181-72610a4b28f9.gif#clientId=u594a4997-d237-4&from=paste&id=u773fc821&originHeight=432&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue33280a4-0f30-464d-8a55-9f365f9b83a&title=)<br />如果想要通过 cat 来实现 bat 命令的效果，则需要去使用 cat 的输出传输到 less 命令中去，这样就可以从开头部分一页一页的查看内容了。
```bash
cat jiaoben3.sh | less
```
![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1643944422888-e2c511df-8198-45b7-b727-56cff0edbcfb.gif#clientId=u594a4997-d237-4&from=paste&id=u77e0f961&originHeight=432&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf48feca2-2ef7-410b-8556-4d164d2ff2e&title=)<br />同样，这种方式也是可以使用回车键一页一页的查看输出内容以及使用空格键可直接跳转至文末最后一行。
<a name="EOOQZ"></a>
### bat 定制主题
目前 bat 定制主题有 24 种，包括：Coldark-Cold、GitHub、Monokai Extended Light、OneHalfDark、Solarized (light)、TwoDark、Visual Studio Dark+、base16、zenburn 等；<br />如果不喜欢默认主题，也是可以根据自己的风格进行修改，只需执行：`bat --list-themes`命令，即可显示可用的主题。
```bash
# bat --list-themes
Theme: 1337

  // Output the square of a number.
  fn print_square(num: f64) {
      let result = f64::powf(num, 2.0);
      println!("The square of {:.2} is {:.2}.", num, result);
  }

......
```
例如要选择Dracula主题，可直接执行：`bat --theme=Dracula 文件名`<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643944422941-565c1332-12b6-4d81-830d-73cb8d96237e.webp#clientId=u594a4997-d237-4&from=paste&id=u65e187e8&originHeight=386&originWidth=742&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u09e5998c-07bb-4056-85c1-874960b0c59&title=)<br />如果想永久改变某一个主题，可使用`export BAT_THEME="Dracula"`在 shell 启动文件，以使更改为永久。<br />可以使用`--style`参数选项来控制`bat`输出的外观，配置元素包括：行号、文件头、网格边框、Git修改来显示除文件内容之外的东西，参数是一个逗号分隔的列表。<br />`--style=numbers,changes`可以使用仅显示 Git修改和行号，而不显示网格和文件标题，设置`BAT_STYLE`环境变量以使这些更改为永久。<br />关于更多 bat 的使用方法可通过执行命令`man bat`或`bat --help`。<br />除此之外，也可以通过 bat 项目的 GitHub 库查看更多详细信息！
<a name="a3Rwq"></a>
## 2、duf
在 Linux 环境下，df 命令是用于显示磁盘分区上可使用的磁盘空间，默认显示单位为 KB，还可以利用该命令来获取硬盘被占用了多少空间，目前还剩下多少空间等信息。
```bash
# df
Filesystem     1K-blocks    Used Available Use% Mounted on
/dev/sda3       18439940 4943264  12559972  29% /
tmpfs             506176      76    506100   1% /dev/shm
/dev/sda1          99150   30003     64027  32% /boot
/dev/sr0         4363088 4363088         0 100% /media/CentOS_6.5_Final
```
除此之外，du 命令也是查看使用空间的，但与 df 命令不同的是 Linux du 命令是对文件和目录磁盘使用的空间的查看，和 df 命令还是有一些区别的。
```bash
# du
4    ./.abrt
8    ./.dbus/session-bus
12    ./.dbus
4    ./.nautilus
40    ./.local/share/gvfs-metadata
24    ./.local/share/applications
68    ./.local/share
72    ./.local
```
接下来介绍一款终端工具，可以理解为传统的 Linux 命令 df 和 du 整合版 —— duf。<br />duf 是一个用 Golang 编写的跨平台磁盘使用情况工具，可以轻松地检查可用磁盘空间，对输出进行分类，并以用户友好的方式进行呈现，适用于 Linux、Windows、macOS、Android、FreeBSD；
<a name="MiTVd"></a>
### duf的安装
<a name="SUggy"></a>
#### Ubuntu 安装方式：
```bash
$ sudo apt install gdebi
#如果报错，Failed to fetch http://security.ubuntu.com......
# sudo apt-get update

$ sudo gdebi duf_0.5.0_linux_amd64.deb
#要先在下载duf_0.5.0_linux_amd64.deb，下载地址为:https://github.com/muesli/duf/releases/download/v0.5.0/duf_0.5.0_linux_amd64.deb
```
<a name="XiFi7"></a>
#### MacOS 安装方式：
```bash
#先安装Homebrew，按照提示安装即可。
/bin/zsh -c "$(curl -fsSL https://gitee.com/cunkai/HomebrewCN/raw/master/Homebrew.sh)"

#安装duf
brew install duf

#或者
sudo port selfupdate && sudo port install duf
```
<a name="K8qtz"></a>
#### Windows 安装方式：
```bash
choco install duf

#或者

scoop install duf
```
<a name="uI9mI"></a>
### duf的使用
安装完毕后，可直接使用 duf，方法也非常简单。<br />不添加任何参数，会输出所有本地设备、已挂载的任何云存储设备以及任何其他特殊设备（包括临时存储位置等）的详细信息；
```bash
$ duf
```
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643944422797-f5bed990-b78f-44f7-972a-8d0184ff49ff.webp#clientId=u594a4997-d237-4&from=paste&id=ufc5cd489&originHeight=463&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0bba54d7-f05a-4f82-a231-b0d33910398&title=)<br />如果只想查看本地连接设备的详细信息，而不想看其他的，可执行：
```bash
$ duf --only local
```
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643944422865-0a91eeac-c7b8-4f54-97f8-4a4cc4d1cec3.webp#clientId=u594a4997-d237-4&from=paste&id=uec23cfbc&originHeight=207&originWidth=1076&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2685a884-bc6f-49eb-bcaa-3a7aacfe083&title=)<br />如果只想根据大小按特定顺序对输出信息进行排序，可执行：
```bash
$ duf --sort size
```
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643944423381-211723f2-b683-4579-9fc4-cdac81f06833.webp#clientId=u594a4997-d237-4&from=paste&id=ueb34e320&originHeight=465&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4587d4c0-a4a2-46b2-902b-7ba9a2d1c6c&title=)<br />更多参数使用方式：
```bash
duf /home /some/file 根据参数，则 duf 将仅列出特定的设备和安装点
duf --all 列出所有内容
duf --hide-network 隐藏网络文件系统，与之对应的 --hide-fuse --hide-special --hide-loops --hide-binds
duf --inodes 列出inodes
duf --output mountpoint,size,usage 指定输出的格式 对应的还有(mountpoint, size, used, avail, usage, inodes, inodes_used, inodes_avail, inodes_usage, type, filesystem)
duf --json 以json格式输出
duf --theme light 如果 duf 无法正确检测终端的颜色，可以设置一个主题
duf --help 查看所有 duf 的可用命令
```
<a name="Nd6E2"></a>
## 3、tldr
对于初学者学习 Linux 来说，遇到最多的问题就是在 Linux 操作系统下有记不完的命令以及包括各种参数的使用方法，其实根本不用死记硬背它，当用到时去查找相关资料或通过`man`命令来查看相关文档，`man`手册输出的信息也比较多，而对于参数较多的命令来说，翻看`man`手册也非常吃力。<br />![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1643944423537-35553d02-946d-4fdc-bbb1-74b5f542e294.gif#clientId=u594a4997-d237-4&from=paste&id=uece00dc9&originHeight=431&originWidth=1079&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ucf256c60-3524-4177-8464-64b87b51ede&title=)<br />这里介绍一款工具：`tldr`，它的全称是：Too Long, Don't Read，中文意思：太长不看的意思。<br />其实跟`man`命令的结果基本上就是太长不看，tldr 这个工具可以理解为简化了 man 命令的帮助文档，把 Linux 命令最常用的一些命令用法列举出来，非常容易看懂。<br />同时，tldr包括非常多语言编写的客户端，比如：C、Go、iOS、Python、nodejs 等等，可以使用不同的客户端来安装；<br />这里的环境是 Ubuntu，需要先安装 Node.js，然后根据下述安装方式操作：
```bash
sudo apt-get install nodejs

sudo apt-get install npm

sudo npm install -g tldr
```
执行完上述三条命令后，还需要再执行`tldr --update`，更新`tldr`至最新的状态，然后就可以直接使用了；
```bash
tldr --update
```
<a name="ennbk"></a>
### tldr 的使用
tldr 命令的使用也非常简单，只需 tldr + 要查询的命令即可；<br />![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1643944423706-3b4cf0ac-ed05-4596-b57f-3b755e98789a.gif#clientId=u594a4997-d237-4&from=paste&id=ufd0630b7&originHeight=431&originWidth=1079&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u778b9fa2-5f3d-4ab3-804a-188a0daf305&title=)
<a name="dPQBi"></a>
## 4、htop
在 Linux 操作系统上显示进程运行状态信息最常用工具是熟悉的 top，它是每位系统管理员的好帮手。<br />htop 可以说是 top 的绝佳替代品，它是用 C 写的，是一个跨平台的交互式的进程监控工具，具有更好的视觉效果，一目了然更容易理解当前系统的状况，允许垂直和水平滚动进程列表以查看它们的完整命令行和相关信息，如内存和 CPU 消耗。还显示了系统范围的信息，例如平均负载或交换使用情况。<br />显示的信息可通过图形设置进行配置，并且可以交互排序和过滤，与进程相关的任务（例如终止和重新处理）可以在不输入其 PID 的情况下进行完成。<br />安装也很简单，只需执行命令：`apt install htop`即可完成。<br />![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1643944423756-6c32a826-3520-40e2-b142-d45a6cafe0cc.gif#clientId=u594a4997-d237-4&from=paste&id=u030477e0&originHeight=433&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9997af37-8407-48fe-b531-74a8af1f103&title=)<br />htop 比起 top，优势很明显，除了功能更加完善以外，它的色彩搭配也很丰富，整体上使用起来更加友好。
<a name="aaWa7"></a>
## 5、glances
Glances 是用 Python 写的一个跨平台的监控工具，旨在通过 curses 或基于 Web 的界面呈现大量系统监控信息，该信息根据用户界面的大小动态调整，是 GNU/Linux、BSD、Mac OS 和 Windows 操作系统的 top/htop 替代品。<br />它可以在客户端/服务器模式下工作，远程监控可以通过终端、Web 界面或 API（XML-RPC 和 RESTful）完成，统计数据也可以导出到文件或外部时间/值数据库。<br />除了列出所有进程及其 CPU 和内存使用情况之外，它还可以显示有关系统的其他信息，比如：

- 网络及磁盘使用情况
- 文件系统已使用的空间和总空间
- 来自不同传感器（例如电池）的数据
- 以及最近消耗过多资源的进程列表

![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1643944423959-f179285a-5f8b-4357-aee3-2bd54d1f2b56.gif#clientId=u594a4997-d237-4&from=paste&id=ua9ab26b6&originHeight=433&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ucac59b5e-450a-4152-a3e2-83cf2b7e7cc&title=)
<a name="eZL7k"></a>
## 6、exa
提到 ls 命令，大家都不陌生，在 Linux 环境下，其主要作用：列出当前目录下所包含的文件及子目录，如果当前目录下文件过多，则使用命令 ls 不是很好，因为这输出出来的结果跟要查找的文件未能达成一致，第一：需要进行二次过滤查找；第二：文件过多时，终端输出结果较慢；<br />EXA 是 Unix 和 Linux 操作系统附带的命令行程序的 ls 现代替代品，赋予它更多功能和更好的默认值。它使用颜色来区分文件类型和元数据。它了解符号链接、扩展属性和 Git。它体积小、速度快，而且只有一个二进制文件。
<a name="kE3Ck"></a>
### EXA 与 LS 有什么不同之处？

- 终端屏幕不再是瓶颈：exa 功能强大，比 ls 要友好，输出结果会更快；
- 颜色：exa 大量使用颜色；
- 不与 ls 命令冲突
<a name="D9PXQ"></a>
### EXA 的特征

- 色彩：文件类型列表以及如何区分文件；
- 网格视图：在与终端窗口大小相同的网格中显示文件；
- 长远：在表中显示文件及其元数据；
- 树视图：在树中显示文件及其子代；
- Git 集成：在存储库中时，查看文件的 Git 状态；
- 筛选：隐藏列表中的文件，显示不可见的文件，并对输出进行排序；
- Xattrs：列出文件的扩展属性；

Ubuntu 21.10 安装只需执行命令：`apt install exa`即可。
<a name="LSk01"></a>
### 显示选项

- `-1`，`--oneline`：每行显示一个条目
- `-G`，`--grid`：将条目显示为网格（默认）
- `-l`，`--long`：显示扩展细节和属性
- `-R`，`--recurse`：递归到目录
- `-T`，`--tree`：作为树递归到目录中
- `-x`，`--across`：对网格进行排序，而不是向下排序
- `-F`，`--classify`：按文件名显示类型指示器
- `--colo[u]r`：何时使用终端颜色
- `--colo[u]r-scale` : 清楚地突出显示文件大小的级别
- `--icons` : 显示图标
- `--no-icons` : 不显示图标（总是覆盖 `--icons`）
<a name="GF7gb"></a>
### 过滤选项

- `-a`，`--all`：显示隐藏和“点”文件
- `-d`，`--list-dirs`：像普通文件一样列出目录
- `-L`，`--level=(depth)`：限制递归深度
- `-r`，`--reverse`：反转排序顺序
- `-s`，`--sort=(field)`：按哪个字段排序
- `--group-directories-first`：在其他文件之前列出目录
- `-D`，`--only-dirs`：只列出目录
- `--git-ignore`：忽略中提到的文件.gitignore
- `-I`，`--ignore-glob=(globs)`：要忽略的文件的全局模式（管道分隔）

传递该`--all`选项两次以同时显示.和..目录。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643944424337-6e166176-e6fe-45f9-b5d6-58837da0ead7.webp#clientId=u594a4997-d237-4&from=paste&id=u31403be2&originHeight=289&originWidth=616&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf99a3d2b-c6b2-4c0f-b285-a6113fc1db6&title=)
<a name="t1whW"></a>
## 7、fd
fd 是一个在文件系统中查找条目的程序，它是 find 命令的一个简单、快速且用户友好的替代品，fd 目的不是取代 find 命令所提供的全部功能，而是在多数用例中提供了合理的默认值，在某些情况下非常有用。<br />fd 输出是彩色的，类似于某些 ls 模式，它是递归的，在包含 Git 存储库的目录中搜索源代码文件时，fd 会自动排除隐藏的文件和目录，包括`.`、`.git`目录，并忽略`.gitignore`文件中的模式。一般来说，搜索速度更快，而且第一次搜索时会提供了更多相关结果。<br />默认情况下，fd 在当前目录中执行不区分大小写的模式搜索，但是，如果搜索模式包含一个大写字母，fd 将以区分大小写的模式进行执行。当然，也可以重写默认值，但在许多情况下还是有利的。同样使用 find 进行相同的搜索需要提供额外的命令行参数。<br />fd 可用于许多 Linux 发行版，执行以下命令，可以使用标准存储库在 Ubuntu 中安装：
```bash
sudo apt install fd-find
```
在 Ubuntu 操作系统中，命令是 fdfind，避免与其他现有的实用程序名称发生冲突，如果希望它是 fd，可以设置一个别名：
```bash
alias fd=fdfind
```
要使别名保持不变，在重启后仍然可用，请将其放入“.bashrc”或“.bash_aliases”文件中。<br />使用不带参数选项的 fd，则类似于 ls，不同之处在于它默认情况下也会将子目录中的文件进行列出。
```bash
fd
bat_0.18.3_amd64.deb
fd_8.3.0_amd64.deb
jiaoben1.sh
jiaoben2.sh
jiaoben3.sh
snap
snap/firefox
snap/firefox/631
snap/firefox/731
snap/firefox/common
snap/firefox/current
snap/snap-store
snap/snap-store/557
snap/snap-store/558
snap/snap-store/common
snap/snap-store/current
test.md
```
在当前目录中搜索所有的 markdown 文件（.md 或.MD ），find 命令如下：
```bash
find . -iname "*.md"
./test.md
```
使用 fd 进行同样的搜索：
```bash
fd *.md
test.md
```
在某些情况下，fd 需要额外的选项；例如，如果想包含隐藏文件和目录，则必须使用选项 `-H`，而这在 find 中是不需要的，关于 fd 更多命令行参数选项的使用方法，可查阅 fd GitHub 存储库。
<a name="NC4tF"></a>
## 8、ag
`ack`和`ag`是两个文本搜索工具，比自带的`grep`要好用得多。<br />在指定目录下搜索文本时，不需要像 grep 那样指定各种命令行选项，输出结果也会包含文件名和行号，并且会高亮显示搜索关键字。<br />`ag`是可以比`grep`、`ack`更快的递归搜索文件内容。<br />Ubuntu 21.10 安装 ag 和 ack
```bash
apt-get install silversearcher-ag
apt install ack
```
<a name="S8gXg"></a>
### ag 命令常用参数

- `ag -g` （文件名）：类似于 find . -name （文件名）
- `ag -i sed`：忽略大小写搜索含 sed 文本
- `ag -A sed`：搜索含 sed 文本，并显示匹配内容之后的 n 行文本
- `ag -B sed`：搜索含 sed 文本，并显示匹配内容之前的 n 行文本
- `ag -C sed`：搜索含 sed 文本，并同时显示匹配内容以及它前后各 n 行文本的内容
- `ag -w sed`：全匹配搜索，只搜索与所搜内容完全匹配的文本
- `ag --java sed`：在 java 文件中搜索含 sed 的文本
- `ag --xml sed`：在 XML 文件中搜索含 sed 的文本
<a name="D3AoA"></a>
### ag 命令的使用
根据文件名进行搜索对应的文件，可使用`-g`参数选项。
```bash
ag -g jiaoben
jiaoben2.sh
jiaoben1.sh
jiaoben3.sh
```
搜索当前目录下有关 sed 关键字的文本。
```bash
ag -w sed
jiaoben1.sh
10:                sed -i '2d' "$new_filename"
```
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643944424552-e67b80ca-c973-4b8f-b0e9-898cbf442528.webp#clientId=u594a4997-d237-4&from=paste&id=ude702840&originHeight=232&originWidth=882&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ucfb5af35-9eab-4deb-8b9d-88219de4c63&title=)<br />更多参数选项，大家可根据自身实际场景进行运用。
<a name="lklFw"></a>
## 9、axel
axel 是命令行多线程下载工具，下载文件时可以替代 curl、wget。
<a name="Nooyx"></a>
### Ubuntu 21.10 安装 axel
```bash
apt-get install axel
```
<a name="F9LEZ"></a>
### axel 参数选项

- `-n`：指定线程数
- `-o`：指定另存为目录
- `-s`：指定每秒的最大比特数
- `-q`：静默模式
<a name="c4Zqk"></a>
### 案例
```bash
# axel -n 10 -o /tmp/ http://mirrors.163.com/ubuntu/ls-lR.gz
# exa /tmp/ls-lR.gz
/tmp/ls-lR.gz
```
可以看出，通过 axel 也是可以把 ls-lR.gz 这个文件正常下载到 /tmp 目录下的。
<a name="kqlX3"></a>
## 10、pydf
在 Linux 系统下，可以使用`df`命令来显示磁盘的相关信息。
```bash
# df
文件系统          1K-块     已用    可用 已用% 挂载点
tmpfs            199180     1660  197520    1% /run
/dev/sda3      19946096 10317864 8589692   55% /
tmpfs            995884        0  995884    0% /dev/shm
tmpfs              5120        4    5116    1% /run/lock
/dev/sda2        524252     5344  518908    2% /boot/efi
tmpfs            199176      100  199076    1% /run/user/1000
/dev/sr0        3043440  3043440       0  100% /media/jacktian/Ubuntu 21.10 amd64
tmpfs            199176       60  199116    1% /run/user/0
```
Ubuntu 21.10 安装 pydf
```bash
apt install pydf
```
pydf 可以说是 df 的替代品，它以更简洁的方式显示磁盘使用状态。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643944424689-b4801f33-2a13-4185-a568-6b2a65e669f8.webp#clientId=u594a4997-d237-4&from=paste&id=uc44bad76&originHeight=345&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2d375ab6-f044-4d3c-a80e-dfad892de1a&title=)
<a name="z3p6k"></a>
## 参考
bat：[https://github.com/sharkdp/bat](https://github.com/sharkdp/bat)<br />duf：[https://github.com/muesli/duf](https://github.com/muesli/duf)<br />tldr：[https://github.com/tldr-pages/tldr](https://github.com/tldr-pages/tldr)<br />htop：[https://github.com/htop-dev/htop](https://github.com/htop-dev/htop)<br />glances：[https://github.com/nicolargo/glances](https://github.com/nicolargo/glances)<br />exa：[https://github.com/ogham/exa](https://github.com/ogham/exa)<br />fd：[https://github.com/sharkdp/fd/](https://github.com/sharkdp/fd/)
