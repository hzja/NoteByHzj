对于分析磁盘使用情况，有两个非常好用的命令：du 和 df 。简单来说，这两个命令的作用是这样的：

- du 命令：它是英文单词 disk usage 的简写，主要用于查看文件与目录占用多少磁盘空间；
- df 命令：它是英文单词 disk free 的简写，主要用于查看磁盘被使用了多少空间、剩余多少空间，等等。

特别是在磁盘爆满的情况下，这两个命令结合起来非常好用。<br />但是，这两个命令毕竟是分开的，要是能整合起来该多好，毕竟都是同类型的命令。<br />别说，还真有人把这两个命令整合起来，它就是：duf 命令！<br />duf 是一个用 Golang 编写的跨磁盘使用情况分手工具，它可以以表格（自适应）的形式输出磁盘使用情况，而且还可以根据需求对结果进行排序，使用非常方便！
<a name="Y8sc8"></a>
## 1、duf命令的安装
以 Ubuntu 为例，不能直接使用 apt-get 命令安装，需要手动安装。<br />首先，从 GitHub 上下载 duf 命令的安装包：
```bash
$ wget https://github.com/muesli/duf/releases/download/v0.8.1/duf_0.8.1_linux_amd64.deb
```
然后，再使用 dpkg 命令安装：
```bash
$ dpkg -i duf_0.8.1_linux_amd64.deb
```
MacOS 平台安装：
```bash
$ brew install duf
# 或者
$ sudo port selfupdate && sudo port install duf
```
Windows 平台安装：
```bash
choco install duf
# 或者
scoop install duf
```
<a name="UIYYJ"></a>
## 2、duf命令的使用
最基本的使用，就是直接 duf 三个字母，不加任何参数，非常简单。这种用法会输出所有本地设备、已挂载的任何云存储设备以及任何其他特殊设备（包括临时存储位置等）的详细信息。
```bash
$ duf
```
![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1650803578173-8895114e-d8f3-4a62-bc10-6c409da3d126.jpeg#clientId=ue355ea90-cd18-4&from=paste&id=u9d574332&originHeight=727&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u89ca1688-ce14-4bf9-890e-603489d66d3&title=)<br />可以看到，它是以彩色及可视化符号形式显示磁盘使用情况（在 Ubuntu 自带终端工具下），非常直观，也非常养眼。<br />这里信息毕竟有点多，如果只想查看本地设备磁盘使用信息，可以加上 `--only local` 选项：
```bash
$ duf --only local
```
![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1650803578126-0cc26e16-51c7-4cd3-95e7-782d0838966d.jpeg#clientId=ue355ea90-cd18-4&from=paste&id=u1a7334cf&originHeight=308&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8a17462b-3cd2-4238-840e-4147a1f4308&title=)<br />当磁盘爆满的时候，肯定比较关心哪些磁盘用量比较大，所以可以根据磁盘使用量进行排序，这时需要加上 `--sort used` 选项：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1650803578184-204966c9-b2b3-4715-98c1-ff2b83501839.jpeg#clientId=ue355ea90-cd18-4&from=paste&id=u5465a1fe&originHeight=915&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uaa5a9c69-629a-415f-a405-319c82f86e5&title=)<br />同理，还可以使用 `--sort size` 对磁盘容量进行排序。<br />如果想知道所有文件系统的使用情况（包括不可见文件系统），可以加上 `--all` 选项：
```bash
$ duf --all
```
（结果有点长，就不截图了）<br />也可以直接针对某个文件、设备或者挂载点进行查看：
```bash
$ duf /home
```
![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1650803578199-617326f8-2c1b-4c20-9804-4d24c958e8f2.jpeg#clientId=ue355ea90-cd18-4&from=paste&id=u79dadb15&originHeight=289&originWidth=1078&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua9506785-c9ac-422e-8bf0-8a4a6695ca8&title=)<br />如果想获得 JSON 格式的输出，还可以加上 `--json` 选项：
```bash
$ duf --json
```
![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1650803578232-8956f12d-d4d1-495c-a49d-ebe45b19ed43.jpeg#clientId=ue355ea90-cd18-4&from=paste&id=ub6e46d92&originHeight=1047&originWidth=797&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ucbc9e1d3-98df-45ee-9253-6c7fd9467a4&title=)<br />除了查看块使用情况，还可以通过 `--inodes` 选项查看 inodes 用量：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1650803578413-9b2d4970-ed17-4b30-a851-00baf39a95ef.jpeg#clientId=ue355ea90-cd18-4&from=paste&id=u23bc9ae8&originHeight=728&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua523d395-cd1b-4a0a-b5c6-8c91dabefba&title=)
<a name="zLxeG"></a>
## 3、小结
duf 命令是 du 和 df 的整合版，可以查看更多信息，同时提供自适应表格及可视化视图，使用体验比原生 df 或 du 命令更佳。
