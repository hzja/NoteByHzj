Linux fd 
<a name="viajL"></a>
### fd仓库地址
[fd仓库地址](https://github.com/sharkdp/fd/)<br />[https://github.com/sharkdp/fd/](https://github.com/sharkdp/fd/)
<a name="uwrY7"></a>
### fd简述
fd 是 find 命令的一个简单而快速的替代品。它的目的不是取代 find 提供的全部功能；相反，它提供了一些合理的默认值，在某些情况下非常有用。<br />例如，在包含 Git 存储库的目录中搜索源代码文件时，fd 会自动排除隐藏的文件和目录，包括.git 目录，并忽略.gitignore 文件中的模式。一般来说，它的搜索速度更快，而且第一次搜索时提供了更多相关的结果。<br />默认情况下，fd 在当前目录中执行不区分大小写的模式搜索，输出是彩色的。使用 find 进行相同的搜索需要提供额外的命令行参数。例如，在当前目录中搜索所有的 markdown 文件（.md 或.MD ）
<a name="6890f8bc"></a>
### fd 与 find 的区别？
fd命令并不意味着要取代传统的find命令，后者在Linux上一直存在。相反，fd试图以更直接的方式满足find的大多数常用用法，而且它通常比find快八到九倍。您可以在项目的GitHub存储库页面上看到它的一些基准测试。<br />fd具有彩色输出，类似于某些ls模式。它是递归的，但默认情况下不搜索隐藏的目录。它了解Git，还将自动忽略“.gitignore”文件中的任何模式。<br />fd默认不区分大小写。但是，如果您的搜索模式包含一个大写字母，fd将以区分大小写的模式运行。当然，您可以重写默认值，但是在许多情况下，它们对您是有利的。
<a name="kz9Yi"></a>
### 安装fd
<a name="84doI"></a>
#### 在Ubuntu上安装fd
从Ubuntu 19.04（Disco Dingo）开始，可以通过使用apt-get调用官方维护的软件包直接安装fd。如果运行的是旧版Ubuntu，请查看GitHub页面上的安装说明。输入以下命令：
```bash
sudo apt-get install fd-find
```
在Ubuntu中，命令是fdfind，以避免与另一个现有的实用程序发生名称冲突。如果希望它是fd，可以设置一个别名：
```bash
alias fd=fdfind
```
要使别名保持不变，以便在重新启动后仍然可用，请将其放入“ .bashrc”或“ .bash_aliases”文件中。
<a name="6CKfU"></a>
#### 在Fedora上安装fd
请键入以下命令：sudo dfn install fd-find
<a name="lTo3q"></a>
#### 在Manjaro上安装fd
键入以下内容：sudo pacman -Syu fd
<a name="qAKrZ"></a>
### fd使用
find 命令如下：
```bash
$ find . -iname "*.md"
```
使用 fd 进行同样的搜索：
```bash
$ fd .md
```
在某些情况下，fd 需要额外的选项；例如，如果想包含隐藏文件和目录，则必须使用选项 -H，而这在 find 中是不需要的。<br />fd 可用于许多 Linux 发行版。执行以下命令，可以使用标准存储库在 Fedora 中安装：
```bash
$ sudo dnf install fd-find
```

如果使用不带命令行选项的fd，则其行为类似于ls，不同之处在于它默认情况下在子目录中列出文件。
```bash
$ fdfind
```
对于不同的文件类型和目录，输出以不同的颜色显示。要查看特定类型的文件，请使用-e（扩展名）选项。请注意，不必在扩展名前加句点（.），也不区分大小写。例如，可以键入以下内容：
```bash
$ fdfind -e png
```
现在，列出的唯一文件是PNG图像文件。要查找单个文件，请在命令行上键入其名称，如下所示：
```bash
$ fdfind fcant.png
```
找到该文件，并且该文件恰好在子目录中。不必告诉fd进行递归搜索。要在特定目录中开始搜索，请在命令行上包含文件路径。以下命令将在“/etc”目录中开始搜索，并查找文件名中包含“passwd”的文件：
```bash
$ fdfind passwd /etc
```
在这里，搜索所有C源代码文件的名称中包含“fcant”的文件:
```bash
$ fdfind -e c fcant
```
<a name="7h1oz"></a>
#### 文件类型和区分大小写
可以要求fd查找目录，文件（包括可执行文件和空文件）以及符号链接。可以使用-t（type类型）选项，然后使用以下字母之一进行操作：

- f: 文件<br />
- d: 目录<br />
- l: 符号链接<br />
- x: 可执行文件<br />
- e: 空文件<br />

下面寻找一个名为images的目录:
```bash
$ fdfind -td images
```
找到匹配项，在当前目录下的一个子目录中。<br />看一下区分大小写如何与搜索模式一起工作。键入以下内容首先搜索文件名中包含“linux”的文件，然后搜索文件名中包含“Linux”的文件：
```bash
$ fdfind -tf linux
$ fdfind -tf Linux
```
在第一个命令中，使用了小写搜索模式，这导致fd以不区分大小写的方式进行操作。这意味着“linux”和“Linux”都是有效匹配。<br />第二个命令包含一个大写字符，这导致fd以区分大小写的方式进行操作。这意味着只有“Linux”是一个有效匹配。
<a name="ndwmw"></a>
#### 命令执行
fd命令允许您启动另一个命令并在找到的每个文件上执行它。<br />假设源代码目录树中某处有一个Zip文件。可以使用以下命令来查找它，该命令搜索具有ZIP扩展名的文件：
```bash
$ fdfind -e zip
```
使用-x（exec）选项，可以将找到的每个文件传递给另一个命令，以供其处理。例如，可以键入以下内容来调用unzip实用程序以解压缩ZIP文件（“ {}”是代表找到的文件的占位符）：
```bash
$ fdfind -e zip -x unzip {}
```
这将解压缩当前工作目录中的文件。如果我们想把它解压缩到包含ZIP文件的目录中，可以使用以下占位符之一:

- {} – 一个占位符，它将随着搜索结果的路径改变(wp-content/uploads/01.jpg)。<br />
- {.} – 类似于{}，但不使用文件扩展名(wp-content/uploads/01)。<br />
- {/}:将被搜索结果的基本名称 (01.jpg)所取代的占位符。<br />
- {//}:发现路径的父目录(wp-content/uploading)。<br />
- {/.}:只有基本名称，没有扩展名(01)。<br />

为了找到ZIP文件并将其解压缩到包含它的目录中，可以使用unzip -d（目录）选项，并传入父目录占位符（{//}）：
```bash
$ fdfind -e zip -x unzip {} -d {//}
```
然后将ZIP文件定位并解压缩到其父目录中。
<a name="QOvlj"></a>
### 总结
这是对fd命令的简要回顾，fd命令更容易使用，也更快。fd并不是完全替代find，而是提供简单的用法、更容易的搜索和更好的性能。fd不需要太多的空间，是一个很好的工具。
