Linux Vim <br />Vim 7.x 开始，netrw.vim 就被设置为默认安装的标准插件了。这个插件允许用户通过 ftp、rcp、scp、http 等协议读写、编辑和浏览远程文件。
<a name="6QZXU"></a>
# 插件 netrw 
`netrw`（面向网络的读写和浏览  Network oriented reading, writing and browsing）插件不仅支持本地和远程终端跨网络编辑、读写文件，还能支持浏览本地和远程终端目录。需要了解这个插件的更多细节信息，在 Vim 会话输入 `help netrw` 即可查看。<br />用 Vim 从本地系统读写存储在远程 Linux 系统上的文件。
```bash
[root@iZuligp6e1dyzfZ /]# vim scp://user@127.0.0.1//tmp/info.log
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596001299795-db7482d2-79e7-4a37-a4bc-263ab2af27f2.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2140747&status=done&style=none&width=1107.6666666666667)<br />命令背后的运行过程实际上是用 scp 命令将远程文件拷贝到本地系统的 `/tmp` 目录下，然后再打开文件编辑的。在编辑完后，scp 命令再次将本地已编辑文件拷贝回远程系统。<br />要从本地查看远程文件内容是否真的改动过用这一条命令：
```bash
$ ssh user@127.0.0.1 cat info.txt
```
用远程终端目录的绝对路径，应使用双斜杠访问：
```bash
$ vim scp://user@127.0.0.1//home/cirdan/Documents/info.txt
```
如果已经因为安全原因改变了SSH端口，则应当显式地说明SSH端口号，如下所示：
```bash
$ vim scp://user@127.0.0.1:2200/info.txt
```
如果没有 ssh/scp 的通道，也可以用其他协议来替代，如下所示：
```bash
$ vim ftp://user@127.0.0.1/path/to/file
```
<a name="c3bbb5f4"></a>
# 在Vim会话里读写远程文件
如果已经进入了一个 Vim 会话，则可以使用 Nread（NetRead）和 Nwrite（NetWrite）命令来读写远程文件。<br />现在使用如下命令打开本地系统的 Vim 编辑器：
```bash
$ vim
```
接着进入了 Vim 会话，要在本地新缓存中的 Vim 会话中读一个远程文件，只需要运行如下命令：
```
:e scp://user@127.0.0.1/info.txt
```
除此之外，还可以用 `Nread` 命令如下所示：
```bash
:Nread scp://user@127.0.0.1/info.txt
```
或者，这样输入：
```bash
:Nread "scp://cirdan@127.0.0.1/info.txt"
```
想要知道这个命令的详细信息，请在 Vim 会话中输入如下命令：
```bash
:Nread ?
```
读完说明文件后，输入 `:q` 退出文件即可。<br />与之相似，要写入远程文件也应先使用如下命令：
```bash
:e scp://user@127.0.0.1/info.txt
```
按 `i` 键进入插入模式就可以写入和修改文件了。<br />也可以用 `:w` 创建并写文件，但是这个命令只能创建一个新的空文件：
```bash
:w scp://user@127.0.0.1/info.txt
```
写完之后，按 `ESC` 键退出编辑，然后输入`:wq`保存并退出文件。<br />除此之外，还可以用 `Nwrite` 命令创建并写入文件，其用法示例如下：
```bash
:Nwrite scp://user@127.0.0.1/info.txt
```
关于 `Nwrite` 命令的详细信息，在Vim会话中输入如下信息即可：
```bash
:Nwrite ?
```

