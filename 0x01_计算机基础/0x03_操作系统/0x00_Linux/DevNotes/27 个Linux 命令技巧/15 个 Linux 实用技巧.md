Linux<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617773866778-0937cd64-bf79-4bd2-9f25-c7f96e409da1.webp#align=left&display=inline&height=978&originHeight=978&originWidth=525&size=0&status=done&style=none&width=525)
<a name="sEXGz"></a>
## 1、快速清空文件的方法
快速清空一个文件，有 N 种方法
```bash
$ > access.log
```
其它几种最常见的清空文件的方法

- `: > access.log`
- `true > access.log`
- `cat /dev/null > access.log`
- `echo -n "" > access.log`
- `echo > access.log`
- `truncate -s 0 access.log`

简单解释下， `:` 在 shell 中是一个内置命令，表示 `no-op`，大概就是空语句的意思，所以 `:` 的那个用法，就是执行命令后，什么都没有输出，将空内容覆盖到文件。
<a name="kV5lf"></a>
## 2、快速生成大文件
有时候，在 Linux 上，需要一个大文件，用于测试上传或下载的速度，通过 `dd` 命令可以快速生成一个大文件
```bash
$ dd if=/dev/zero of=file.img bs=1M count=1024
```
上述命令，生成一个文件名为 file.img 大小为 1G 的文件。
<a name="b0qDq"></a>
## 3、安全擦除硬盘数据
介绍一种擦除硬盘数据的方法，高效，安全。可以通过 `dd` 命令，轻松实现：
```bash
$ dd if=/dev/urandom of=/dev/sda
```
使用 `/dev/urandom` 生成随机数据，将生成的数据写入 `sda` 硬盘中，相当于安全的擦除了硬盘数据。
<a name="EWjvf"></a>
## 4、快速制作系统盘
在 Linux 下制作系统盘，直接一条命令搞定：
```bash
$ dd if=ubuntu-server-amd64.iso of=/dev/sdb
```
`sdb` 可以 U 盘，也可以是普通硬盘
<a name="hrHOH"></a>
## 5、查看某个进程的运行时间
可能，大部分同学只会使用 `ps aux`，其实可以通过 `-o` 参数，指定只显示具体的某个字段，会得到更清晰的结果。
```bash
$ ps -p 10167 -o etimes,etime
ELAPSED     ELAPSED
1712055 19-19:34:15
```
通过 `etime` 获取该进程的运行时间，可以很直观地看到，进程运行了 19 天<br />同样，可以通过 `-o` 指定 `rss` 可以只获取该进程的内存信息。
```bash
$ ps -p 10167 -o rss
  RSS
 2180
```
<a name="IJvSN"></a>
## 6、动态实时查看日志
通过 `tail` 命令 `-f` 选项，可以动态地监控日志文件的变化，非常实用
```bash
$ tail -f test.log
```
如果想在日志中出现 `Failed` 等信息时立刻停止 tail 监控，可以通过如下命令来实现：
```bash
$ tail -f test.log | sed '/Failed/ q'
```
<a name="grxKw"></a>
## 7、时间戳的快速转换
时间操作，对程序员来说就是家常便饭。有时候希望能够将时间戳，转换为日期时间，在 Linux 命令行上，也可以快速的进行转换：
```bash
$ date -d@1234567890 +"%Y-%m-%d %H:%M:%S"
2009-02-14 07:31:30
```
当然，也可以在命令行上，查看当前的时间戳
```bash
$ date +%s
1617514141
```
<a name="Iyj6g"></a>
## 8、优雅的计算程序运行时间
在 Linux 下，可以通过 `time` 命令，很容易获取程序的运行时间：
```bash
$ time ./test
real    0m1.003s
user    0m0.000s
sys     0m0.000s
```
可以看到，程序的运行时间为: `1.003s`。可以看到 `real` 貌似不等于 `user` + `sys`，而且还远远大于，这是怎么回事呢？<br />先来解释下这三个参数的含义：

- `real`：表示的钟表时间，也就是从程序执行到结束花费的时间；
- `user`：表示运行期间，cpu 在用户空间所消耗的时间；
- `sys`：表示运行期间，cpu 在内核空间所消耗的时间；

由于 `user` 和 `sys` 只统计 cpu 消耗的时间，程序运行期间会调用 sleep 发生阻塞，也可能会等待网络或磁盘 IO，都会消耗大量时间。因此对于类似情况，`real` 的值就会大于其它两项之和。<br />另外，也会遇到 `real` 远远小于 `user` + `sys` 的场景，这是什么鬼情况？<br />这个更好理解，如果程序在多个 cpu 上并行，那么 `user` 和 `sys` 统计时间是多个 cpu 时间，实际消耗时间 `real` 很可能就比其它两个之和要小了
<a name="q9lN2"></a>
## 9、命令行查看ascii码
在开发过程中，通常需要查看 `ascii` 码，通过 Linux 命令行就可以轻松查看，而不用去 Google 或 Baidu
```bash
$ man ascii
```
<a name="0JjvS"></a>
## 10、优雅的删除乱码的文件
在 Linux 系统中，会经常碰到名称乱码的文件。想要删除它，却无法通过键盘输入名字，有时候复制粘贴乱码名称，终端可能识别不了，该怎么办？<br />不用担心，下边来展示下 `find` 是如何优雅的解决问题的。
```bash
$ ls  -i
138957 a.txt  138959 T.txt  132395 ڹ��.txt
$ find . -inum 132395 -exec rm {} \;
```
命令中，`-inum` 指定的是文件的 `inode` 号，它是系统中每个文件对应的唯一编号，find 通过编号找到后，执行删除操作。
<a name="TDck3"></a>
## 11、Linux上获取公网IP地址
在办公或家庭环境，虚拟机或服务器上配置的通常是内网 IP 地址，如何知道，在与外网通信时，公网出口 IP 是什么呢？<br />这个在 Linux 上非常简单，一条命令搞定
```bash
$ curl ip.sb
$ curl ifconfig.me
```
上述两条命令都可以
<a name="1ldJ6"></a>
## 12、如何批量下载网页资源
有时，同事会通过网页的形式分享文件下载链接，在 Linux 系统，通过 `wget` 命令可以轻松下载，而不用写脚本或爬虫
```bash
$ wget -r -nd -np --accept=pdf http://fast.dpdk.org/doc/pdf-guides/
# --accept：选项指定资源类型格式 pdf
```
<a name="UPoud"></a>
## 13、历史命令使用技巧
分享几个历史命令的使用技巧，能够提高工作效率。

- `!!`：重复执行上条命令；
- `!N`：重复执行 history 历史中第 N 条命令，N 可以通过 history 查看；
- `!pw`：重复执行最近一次，以`pw`开头的历史命令，这个非常有用，使用非常高频；
- `!$`：表示最近一次命令的最后一个参数；

大部分同学没用过 `!$`，这里简单举个例子，感受一下它的高效用法
```bash
$ vim /root/sniffer/src/main.c
$ mv !$ !$.bak
# 相当于
$ mv /root/sniffer/src/main.c /root/sniffer/src/main.c.bak
```
当前工作目录是 root，想把 main.c 改为 main.c.bak。正常情况可能需要敲 2 遍包含 main.c 的长参数，当然也可能会选择直接复制粘贴。<br />通过使用 `!$` 变量，可以很轻松优雅的实现改名。
<a name="OiHac"></a>
## 14、快速搜索历史命令
在 Linux 下经常会敲很多的命令，怎么快速查找并执行历史命令呢？<br />通过_上下键_来翻看历史命令，No No No，可以通过执行 `Ctrl + r`，然后键入要所搜索的命令关键词，进行搜索，回车就可以执行，非常高效。
<a name="sfjI7"></a>
## 15、真正的黑客不能忽略技巧
最后，再分享一个真正的黑客不能忽略技巧。在所要执行的命令前，加一个**空格**，那这条命令就不会被 `history` 保存到历史记录。有时候，执行的命令中包含敏感信息，这个小技巧就显得非常实用了，也不会再因为忘记执行 `history -c` 而烦恼了。
