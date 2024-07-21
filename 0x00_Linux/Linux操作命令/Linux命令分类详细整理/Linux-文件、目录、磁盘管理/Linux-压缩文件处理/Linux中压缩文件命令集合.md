Linux
<a name="UTwS5"></a>
## 文件压缩意义
文件压缩可以节约硬盘，磁盘的储存空间。在网络上传输压缩文件可以极大的节约服务器带宽。下面介绍一些压缩命令。
<a name="zYLUK"></a>
## 1、tar 文件压缩
tar是Linux管理员最流行的压缩和解压缩工具，用于以gzip、bzip2或lzip等多种文件格式存档。
<a name="X4Rci"></a>
### 在Debian/Ubuntu 中安装 tar
```bash
> apt-get install tar
```
<a name="H33dK"></a>
### 在 CentOS 中安装 tar
```bash
> yum install tar
```
<a name="Ckk35"></a>
### 例子
<a name="J5Lcn"></a>
#### 创建一个 512MB 大小的文件
```
> fallocate -l 512M um.img
```
<a name="TbQm1"></a>
#### 创建存档
```
> tar -cvfz um.tar.gz um.img
```

- `-c` – 创建存档
- `-v` – 详细信息
- `-f`  用于存档文件
- `-z` 以 gzip 格式压缩文件
<a name="e11ni"></a>
## 2、gzip文件压缩
gzip 工具是 Linux 中最流行和最快速的文件压缩应用程序。Gzip 工具保留原始文件名、压缩文件的扩展名 .gz 和时间戳。
<a name="bcZDM"></a>
### Ubuntu安装
```
> apt-get install gzip
```
<a name="ycUZU"></a>
### CentOS安装
```
> yum install gzip
```
<a name="DfrIz"></a>
### 例子
```bash
> gzip examplefile
```
<a name="hj2uU"></a>
#### 获取有关压缩文件的详细信息
```bash
> gzip -l examplefile.gz 
compressed uncompressed ratio uncompressed_name
203559 209715200 9% examplefile
```
<a name="tCOMS"></a>
#### 解压压缩文件
```bash
> gzip -d examplefile.gz
```
<a name="bQqb7"></a>
#### 递归压缩所有文件和目录
```bash
> gzip -r examplefile
```
<a name="zgHCr"></a>
## 3、lzma文件压缩
Lzma 是类似于 zip 或 tar 的压缩工具，但与 bzip 相比，它的执行速度更快，它是所有 Linux 发行版的内置工具。<br />要创建存档，请使用以下命令
```bash
> lzma -c --stdout examplefile> examplefile.lzma
```
<a name="YG5rk"></a>
### 提取数据
```
> lzma -d --stdout examplefile.lzma >examplefile
```
<a name="oIWnT"></a>
## 4.xz文件压缩
XZ 是 lzma 应用程序的继承者，它只能压缩单个文件，但不能在单个命令中压缩多个文件。2009 年推出，因此可能无法在较旧的 Linux 版本上支持，但所有最新的 Linux 操作系统都默认安装，它会自动为压缩文件添加 .xz 扩展名。
<a name="PZ8W7"></a>
### 例子
<a name="c5Axp"></a>
#### 压缩文件
```bash
> xz examplefile
```
<a name="cPEB7"></a>
#### 解压缩文件
```bash
> xz -d examplefile.xz
```
<a name="Aju9p"></a>
## 5、bzip2文件压缩
Bzip2 应用程序比 gzip 执行得更快，它更紧凑地压缩文件和文件夹。在压缩文件时需要更多的 RAM，以减少内存消耗，在选项中传递 -s 参数。
<a name="p7Hbv"></a>
### 例子
```bash
> bzip2 examplefile
```
<a name="KmpMR"></a>
#### 实用 `-s` 参数
```bash
> bzip2 -s examplefile
```
<a name="cOSRP"></a>
#### 解压 .bz2 压缩文件
```bash
> bzip2 -d examplefile.bz2
```
**或者**
```bash
> bunzip2 examplefile.bz2
```
<a name="ZSx36"></a>
#### 使用 `-v` 选项详细说明
```bash
> bzip2 -v examplefile
examplefile: 11848638:1, 000 bits/byte, 100% saved, 209715200 in, 177 out.
```
<a name="E8nTz"></a>
## 6、pax文件压缩
Pax 是 Linux 中鲜为人知的归档工具，它执行速度快而且不仅仅是一个压缩器，它可以真正归档它可以远程复制文件。
<a name="ONDCh"></a>
### Debian/Ubuntu安装
```bash
> apt-get install pax
```
<a name="PDvXb"></a>
### 例子
```bash
# pax -wf examplefile.tar examplefile
```
`w`写入存档的选项，`f`用于文件。<br />要以 bzip 或 gzip 压缩文件，请使用 -z 选项
```bash
# pax -wf examplefile.tar.gz examplefile
```
<a name="IRW5U"></a>
#### 列出存档内容
```bash
> pax -f examplefile.tar
```
<a name="cBgzk"></a>
#### 提取档案内容
```bash
> pax -r <examplefile.tar
```
<a name="pNXmr"></a>
## 7、Peazip文件压缩器
Peazip 是一个跨平台的压缩工具，它基于 7zip、p7zip、freearch 等开源工具，支持超过 180 种文件格式。WinRar 的一个很好的替代品。
<a name="T28QY"></a>
### 安装
<a name="IxIew"></a>
#### 下载包并安装
```bash
> wget sourceforge.net/projects/peazip/files/1/peazip_LINUX.Qt-2_i3deb
```
<a name="cKLWb"></a>
#### 安装
```bash
> dpkg -i peazip_LINUX.Qt-2_i3deb
```
<a name="GMEFL"></a>
## 8、7zip文件压缩器
7Zip文件压缩器是最初为Microsoft Windows开发的开源应用程序，它支持多种文件压缩格式并以高文件压缩而闻名，它可以用于通过单个命令压缩多个文件。
<a name="gNOZI"></a>
### 安装
```bash
> apt-get install p7zip-full p7zip-rar
```
<a name="d2T6o"></a>
### 例子
```bash
> 7z  a examplefile.7z examplefile
```
<a name="SW8dI"></a>
#### 提取文件
```bash
> 7z  e examplefile.7z
```
<a name="yH8G2"></a>
## 9、shar文件压缩
Shar 是一个命令行工具，可以用来压缩测试文件，shar 可以定义为shell 存档。一个简单而快速的文件归档应用程序对于获取 shell 脚本的归档很有用。
<a name="aPHfC"></a>
### 安装
```bash
> apt-get install sharutils
```
<a name="p5tQj"></a>
### 例子
<a name="K6GlK"></a>
#### 压缩文件
```bash
> shar examplefile > examplefile.shar
```
<a name="k1ShV"></a>
#### 解压文件
```bash
> unshar examplefile.shar
```
<a name="cQdMx"></a>
## 10、cpio文件压缩
可以定义为复制输入和输出，它在输入中逐行读取文件名列表，在输出中归档文件。它是一个经典的命令，不需要安装。
<a name="Jvez3"></a>
### 例子
```bash
> cd /tmp
```
```bash
ls | cpio -ov >/home/username/backup.cpio
```
<a name="AZFAu"></a>
#### 解压存档
```bash
> cpio -idv <backup.cpio
```
<a name="e5YjJ"></a>
## 11、ar
rar 的祖先或前身，在 Debian 及其衍生产品中仍然使用，一个简单的归档工具，但现在不怎么流行。
<a name="gWpDg"></a>
### 例子
<a name="c7EZa"></a>
#### 创建存档
```bash
> ar cvsr examplefile.a examplefile
```
<a name="zGTWf"></a>
#### 解压存档
```bash
> ar -xv examplefile.a
```
<a name="w0oPN"></a>
## 12、ISO
用于获取虚拟磁盘映像或光驱的 .iso 映像，以后可用于刻录新磁盘或作为基于超版本虚拟化的一些安装介质，是 Linux 管理员非常有用的工具。
<a name="yH0ad"></a>
### 例子
```bash
> dd if=/media/dvd of=/home/username/filename.iso
```
<a name="X5Zb1"></a>
## 13、kgb
Kgb 是一种鲜为人知但很有效的归档工具，它能够将多个文件压缩成一个小的高压缩比归档。它支持所有常见的文件存档格式，如 zip、gzip、7zip 和 rar。
<a name="xNn2L"></a>
### 安装
```bash
> apt-get install kgb
```
<a name="xMW3s"></a>
### 例子
```bash
> kgb examplefile.kgb examplefile
```
<a name="XNkwn"></a>
#### 提取包
```bash
> kgb  x examplefile.kgb
```
<a name="GdWPQ"></a>
## 14、Zpac
能够处理大文件，轻量级，它是由 Matt Mahoney 于 2010 年 6 月 16 日编写的，它提出了高标准的数据压缩存档，它基于 PAQ 上下文混合算法。
<a name="ZeFRW"></a>
### 安装
```bash
> apt-get install zpac
```
<a name="L4d1S"></a>
### 例子
```bash
> zpaq c examplefile.tar examplefile
```
<a name="KiUeR"></a>
## 15、File Roller
File Roller是Linux GUI 中的归档管理器，它可以创建或修改归档，它可以显示归档的内容，是7zip 或winrar 的一个很好的替代品。支持几乎所有文件格式，如 .iso、.lsa、7zip、gzip、zip、zz 等。在 Mint 和 Ubuntu 发行版默认安装。
