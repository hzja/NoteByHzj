Linux
<a name="cPWf1"></a>
## whereis 命令
whereis 命令是一个命令行程序，可以找出任何二进制可执行文件、源文件或手册页的路径或位置。<br />在告诉你如何使用 whereis 命令之前，先看看其语法。
<a name="RBJXE"></a>
### 语法
以下是 whereis 命令的语法：
```bash
whereis [OPTIONS] FILE_NAME
```
[whereis](https://mp.weixin.qq.com/s?__biz=MjM5NjQ4MjYwMQ==&mid=2664673790&idx=3&sn=d60290ddad9ba199836047751669d575&chksm=bdcf8ab88ab803ae2ce28efe1c2a76cd338104727c9d565d3611b54b38cdd46d33536d64a57a&mpshare=1&scene=23&srcid=0115eFekNDoMdgFBZbf1vS22&sharer_sharetime=1673785614263&sharer_shareid=6b928c9c43cd9f15cbb6e88fe257eae4) 命令的参数是要搜索的程序名或文件名。该参数是必须的。<br />默认情况下，它在环境变量（如 HOME、USER、SHELL 等）中定义的路径中搜索程序。<br />看下面一些例子。
<a name="XPukN"></a>
## Linux 和 BSD 中 whereis 命令的例子
下面是 [whereis](https://mp.weixin.qq.com/s?__biz=MjM5NjQ4MjYwMQ==&mid=2664673790&idx=3&sn=d60290ddad9ba199836047751669d575&chksm=bdcf8ab88ab803ae2ce28efe1c2a76cd338104727c9d565d3611b54b38cdd46d33536d64a57a&mpshare=1&scene=23&srcid=0115eFekNDoMdgFBZbf1vS22&sharer_sharetime=1673785614263&sharer_shareid=6b928c9c43cd9f15cbb6e88fe257eae4) 命令的一个简单例子，试图搜索 firefox。在下面的输出中，可以看到包含 firefox 文件或可执行文件的路径列表。
```bash
$ whereis firefox

firefox: /usr/bin/firefox /usr/lib64/firefox /etc/firefox /usr/share/man/man1/firefox.1.gz
```
![Linux 中 whereis 命令的简单例子](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1673786268674-f625d126-daef-482e-a3e4-3f3ede7ab434.jpeg#averageHue=%23090909&clientId=u71737949-cda4-4&from=paste&id=u069746da&originHeight=319&originWidth=773&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=ue4013ce3-3f92-463a-bbc6-18d987bb3e7&title=Linux%20%E4%B8%AD%20whereis%20%E5%91%BD%E4%BB%A4%E7%9A%84%E7%AE%80%E5%8D%95%E4%BE%8B%E5%AD%90 "Linux 中 whereis 命令的简单例子")<br />带有选项 `-l` 的命令会显示其搜索的路径列表。比如:
```bash
$ whereis -l

bin: /usr/bin
bin: /usr/sbin
bin: /usr/lib
bin: /usr/lib64
bin: /etc
bin: /usr/games
bin: /usr/local/bin
bin: /usr/local/sbin
bin: /usr/local/etc
bin: /usr/local/lib
bin: /usr/local/games
```
如果 [whereis](https://mp.weixin.qq.com/s?__biz=MjM5NjQ4MjYwMQ==&mid=2664673790&idx=3&sn=d60290ddad9ba199836047751669d575&chksm=bdcf8ab88ab803ae2ce28efe1c2a76cd338104727c9d565d3611b54b38cdd46d33536d64a57a&mpshare=1&scene=23&srcid=0115eFekNDoMdgFBZbf1vS22&sharer_sharetime=1673785614263&sharer_shareid=6b928c9c43cd9f15cbb6e88fe257eae4) 命令没有找到任何东西，它只显示参数的名称。例如，如果在 Linux 中搜索 nano，它没有安装，它的输出如下：
```bash
$ whereis nano
```
```
nano:
```
如果想搜索更多的参数，可以随时添加多个参数。例如，下面的命令同时搜索 bash 和 nano，输出结果是这样的：
```bash
$ whereis bash nano

bash: /usr/bin/bash /usr/share/man/man1/bash.1.gz /usr/share/info/bash.info.gz
nano: /usr/bin/nano /usr/share/nano /usr/share/man/man1/nano.1.gz /usr/share/info/nano.info.gz
```
也可以使用 `-b` 选项搜索特定的文件类型，比如二进制文件。下面的命令只告诉你 nano 的二进制路径。
```bash
$ whereis -b nano

nano: /usr/bin/nano /usr/share/nano
```
同样，`-s` 选项可以搜索源文件，而 `-m` 选项可以搜索手册页。
```bash
$ whereis -m nano

nano: /usr/share/man/man1/nano.1.gz /usr/share/info/nano.info.gz
```
也可以结合上面的选项来进行更广泛的搜索。例如，下面的命令可以搜索 nano 和 firefox 的二进制、手册页；而对于 bash，只搜索手册页。
```bash
$ whereis -bm nano firefox -m bash

nano: /usr/bin/nano /usr/share/nano /usr/share/man/man1/nano.1.gz /usr/share/info/nano.info.gz
firefox-m:
bash: /usr/bin/bash /usr/share/man/man1/bash.1.gz /usr/share/info/bash.info.gz
```
下面是选项的摘要：

| **选项** | **描述** |
| --- | --- |
| -b | 只搜索二进制文件。 |
| -m | 只搜索手册页部分。 |
| -s | 只搜索源码。 |
| -u | 搜索不寻常的条目。如果一个文件没有所要求的每种类型的条目，就被称为不寻常。因此，`whereis -m -u *`<br /> 会查询当前目录中没有文档的那些文件。 |
| -B | 改变或限制 whereis<br /> 搜索二进制文件的地方。 |
| -M | 更改或限制 whereis<br /> 搜索手册的位置。 |
| -S | 更改或以其他方式限制 whereis<br /> 搜索源码的位置。 |
| -f | 终止上一个目录列表并指示文件名的开始，并且必须在使用任何 `-B`、`-M` 或 `-S` 选项时使用。 |

