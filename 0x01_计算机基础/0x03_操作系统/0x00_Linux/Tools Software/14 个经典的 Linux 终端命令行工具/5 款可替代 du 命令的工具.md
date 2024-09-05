Linux<br />在 Linux 环境下，du 命令可以对 Linux 系统中的文件和目录空间使用情况进行计算和汇总。<br />当文件占用磁盘空间 80 % 且在暂时无法扩容空间的情况下，可以找出系统中占比磁盘空间较大的具体目录下的文件进行清理，只需执行如下命令即可查看文件夹占用空间大小。
```bash
du -sh
```
进入占用空间较大的文件夹执行如下命令可以将当前目录下的文件从大到小排序。
```bash
du -sh * |sort -rh
```
将输出后占用空间较大的文件根据实际情况判断是否可以正常删除，从而来释放磁盘空间并进行观察磁盘使用率。<br />当文件占用磁盘空间较多时，du 就会执行很慢，每次会反复执行来找出大文件，效率较低，此时ncdu就可以来解决该问题。
<a name="wGwPl"></a>
### 一、ncdu
ncdu 是一个使用 C 语言和 ncurses 接口开发的简易快速的磁盘用量分析器，可以用来查看目录或文件在本地或远程系统上占用磁盘空间的情况。
<a name="M7HQq"></a>
#### Ubuntu 21.10 安装 ncdu：
```bash
sudo apt-get install ncdu
```
<a name="e0EWc"></a>
#### ncdu 参数选项：

- -h：帮助信息
- -q：静音模式，刷新间为 2 秒
- -v：打印版本
- -x：相同的文件系统
- -e：启用扩展信息
- -r：只读
- -o FILE：将扫描目录导出为文件
- -f FILE：从文件中导入扫描目录
- -0,-1,-2：扫描时使用的 UI (0=none,2=full ncurses)
- --si：使用基数 10 (SI) 前缀而不是基数 2
- --exclude PATTERN：排除匹配 PATTERN 的文件
- -X, --exclude-from FILE：排除与文件中任何模式匹配的文件
- -L, --follow-symlinks：按照符号链接（不包括目录）
- --exclude-caches：排除包含 CACHEDIR.TAG 的目录
- --exclude-kernfs：排除 Linux 伪文件系统（procfs、sysfs、cgroup…）
- --confirm-quit：确认退出
- --color SCHEME：设置配色方案

如果不使用任何参数选项，可直接在需要分析的目录下执行命令ncdu，等待 ncdu 扫描完毕后，此时就会弹出一个交互式的终端界面。<br />可以清晰的看出每个目录的大小，使用方向键或Enter键可进入或退出所选的具体目录。<br />当发现某个目录或文件占用空间较大时，可根据上述使用键盘情况找到那个文件，切合实际情况确认如果可以删除的话，无需退出 ncdu，选中要删除的目录或文件后，按d键即可删除，若删除完毕，按q键即可退出。<br />![](https://cdn.nlark.com/yuque/0/2021/gif/396745/1638925204540-eaacb7e3-2e22-423b-bdec-0b304cd03070.gif#averageHue=%23000000&clientId=u80802b27-3651-4&from=paste&id=u9d18fbf0&originHeight=435&originWidth=1079&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub80830a7-b6e3-4db2-9a50-322628b0a60&title=)
<a name="wcgtI"></a>
### 二、dust
dust（du + rust = dust）是用 rust 编写的一个免费、开源、更直观的 du 工具，也是一个提供文件类型和元数据的工具。
<a name="MxMOy"></a>
#### Ubuntu 21.10 安装 dust：
du-dust_0.7.1_amd64.deb 是 dust GitHub 存储库中的最新版本，从 dust 的发布页面下载.deb文件，可执行如下命令进行下载和安装。
```bash
wget https://github.com/bootandy/dust/releases/du-dust_0.7.1_amd64.deb
dpkg -i du-dust_0.7.1_amd64.deb
```
对于其他系统，可以参考 GitHub 上 dust 的其他操作系统安装方式。
<a name="jelHq"></a>
#### dust 用法：
```bash
dust [FLAGS] [OPTIONS] [--] [inputs]...
```
<a name="OC13D"></a>
#### dust 参数选项：

- -f：目录“大小”是子文件/目录的数量，而不是磁盘大小
- -s：使用文件长度而不是块
- -p：子目录的路径不会缩短
- -h：帮助信息
- -i：不显示隐藏文件
- -x：仅计算与提供的目录位于同一文件系统上的文件和目录
- -b：不会显示百分比条或百分比
- -c：不会打印颜色（通常最大的目录是彩色的）
- -r：打印树倒置（最大的最高）
- -t：仅显示这些文件类型
- -V：打印版本信息
- -d：显示深度
- -e：仅包含与此正则表达式匹配的文件。对于 png 文件类型：-e ".png$"
- -X：排除任何具有此名称的文件或目录
- -v：排除与此正则表达式匹配的文件。忽略 png 文件类型：-v ".png$"
- -n：要显示的输出行数。这是高度，（但 h 有帮助）[默认值：23]
- -w：指定输出的宽度覆盖终端宽度的自动检测
<a name="u9MJY"></a>
#### dust 使用：
查看当前目录及所有子目录下的文件大小，可以执行以下命令：
```bash
dust
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638925204533-3f97a527-ffc0-4ae7-b9d9-c428a4e4acbf.webp#averageHue=%23070907&clientId=u80802b27-3651-4&from=paste&id=u06cb1be7&originHeight=319&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua1b514e9-10a0-4fed-92b4-94446b63bdf&title=)<br />`-p`参数可以按照从当前目录起始的完整目录进行显示
```bash
dust -p
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638925204659-e776d546-a07c-4554-a134-f07d7d993447.webp#averageHue=%233f3e39&clientId=u80802b27-3651-4&from=paste&id=u287a981c&originHeight=317&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ufdaa698f-7c68-499d-a47d-6ca317a0706&title=)<br />如果只查看多个目录的大小，只需要同时列出这些目录，并用空格分隔开即可：
```bash
dust /bin /etc
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638925204639-d9ec2d1f-9943-4f9b-879a-f0d308f4e466.webp#averageHue=%231f351f&clientId=u80802b27-3651-4&from=paste&id=u511a5042&originHeight=306&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5a9142de-6de1-467e-8c80-d4dde535750&title=)<br />显示文件的长度：
```bash
dust -s
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638925204590-d07b73c4-3e64-4cca-a032-ab62342f0d6b.webp#averageHue=%23191e18&clientId=u80802b27-3651-4&from=paste&id=ud18288c8&originHeight=319&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub678622f-fc9e-4afe-8b41-30c6bb3882c&title=)<br />只显示 10 个目录：
```bash
dust -n 10
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638925205231-ee3908c6-23d3-43df-b0d2-b7c758e29542.webp#averageHue=%230b0d0a&clientId=u80802b27-3651-4&from=paste&id=uf18ca6df&originHeight=158&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua80e1ba4-e157-413d-a75f-80f15dbad1d&title=)
<a name="hdfSI"></a>
### 三、duu
duu 是 Directory Usage Utility（目录使用实用程序）的缩写，基于 Python 语言编写的查看指定目录大小的工具，具有跨平台特性，可以在 Windows、Linux 和 MacOS 操作系统上使用，它是以千字节为单位显示目录磁盘使用情况。
<a name="X74yY"></a>
#### Ubuntu 21.10 安装 duu：
安装 duu 工具之前需要先确保系统有没有安装 Python 3 环境，没有的话需要安装一下，有的话忽略即可，不过目前在大多数 Linux 发行版中默认软件仓库都是有 Python 3 的。<br />v2.22.tar.gz 是 duu GitHub 存储库中最新版本的源码包，从 duu 的发布页面下载.tar.gz文件，可执行如下命令进行下载和安装。
```bash
wget https://github.com/jftuga/duu/archive/refs/tags/v2.22.tar.gz
tar -zxvf v2.22.tar.gz
```
解压完毕后，进入 duu-2.22 文件夹，查看duu.py文件是否存在。
```bash
# ls | grep duu
duu-2.22
# cd duu-2.22/
# ls
duu.py  LICENSE  README.md
```
<a name="DYomv"></a>
#### duu 用法：
```bash
duu.py [-h] [-b] [-e] [-q] [-s STATUS] [-n] [-N] [-f] [-S] [-H] [-T THREADS] [-x EXCLUDE] [-X REGEXPR] [-o OUTPUT] [dname]
```
<a name="kAUgz"></a>
#### duu 参数选项：

- -h：帮助信息
- -b：不打印摘要或统计数据，专门用于排序时使用
- -e：总结文件扩展名
- -q：不显示单个目录
- -s：将处理状态发送到 STDERR，每个 STATUS 目录数
- -n：跳过以“.”开头的目录
- -N：不递归
- -f：显示每个目录中的文件数
- -S：显示均值、中值、众数和标准差文件统计信息
- -H：以更易读的格式显示数字
- -T：并发线程数，考虑 SAN
- -x：排除不分大小写字符串的冒号分隔列表
- -X：排除不分大小写正则表达式的冒号分隔列表
- -o：输出到 CSV 文件
<a name="dsdRa"></a>
#### duu 使用：
查看当前目录的大小，只需要执行如下命令：
```bash
python3 duu.py
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638925205335-36710277-0158-445c-985c-27b0c9fb9a3d.webp#averageHue=%23202c22&clientId=u80802b27-3651-4&from=paste&id=u5b2fec50&originHeight=233&originWidth=627&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9e39e802-508f-4692-8234-99588b4f880&title=)<br />如上图，可以看出 duu 会显示当前目录下文件和目录的数量并按照 Bytes、KB、MB 为单位显示这些文件的总大小，以及每个文件的大小。<br />查看某个目录的大小，只需要把目录的绝对路径加上即可：
```bash
python3 duu.py /etc/init.d/
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638925205482-e5ebd96e-2982-47b9-9f0d-23de64cea24b.webp#averageHue=%234a5357&clientId=u80802b27-3651-4&from=paste&id=ub8705ba8&originHeight=231&originWidth=772&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub8642580-60be-406a-ad56-7c138f64f9c&title=)
<a name="jsv3O"></a>
### 四、diskus
diskus 也是基于 Rust 编写的一个小型、快速、可替代`du -sh`命令的开源工具，diskus 会计算当前目录下所有文件的大小，执行命令`diskus`效果和`du -sh`、`du -sh --bytes`是一样的。<br />根据 diskus GitHub 存储库的开发者表示，在他的 8 核笔记本电脑上，以 hyperfine 命令行基准测试工具将中等大小的文件夹（15GB、100k 目录、400k 文件）进行测试，对于较小的文件夹测试实际上是没有意义的，因为所有程序都会在合理的时间内完成，而不会中断您的工作流程，从而得出的结论是：diskus 比 du 冷磁盘缓存快十倍左右，热磁盘缓存快三倍多。<br />以下是冷磁盘缓存和热磁盘缓存基于`diskus`、`du -sh`、`sn p -d0 -j8`和`dust -d0`命令测试出的结果对比。
<a name="vmC7z"></a>
#### 冷磁盘缓存
| Command | Mean [s] | Min [s] | Max [s] | Relative |
| --- | --- | --- | --- | --- |
| diskus | 1.746 ± 0.017 | 1.728 | 1.770 | 1.00 |
| du -sh | 17.776 ± 0.549 | 17.139 | 18.413 | 10.18 |
| sn p -d0 -j8 | 18.094 ± 0.566 | 17.482 | 18.579 | 10.36 |
| dust -d0 | 21.357 ± 0.328 | 20.974 | 21.759 | 12.23 |

<a name="OADPm"></a>
#### 热磁盘缓存
| Command | Mean [s] | Min [s] | Max [s] | Relative |
| --- | --- | --- | --- | --- |
| diskus | 500.3 ± 17.3 | 472.9 | 530.6 | 1.00 |
| du -sh | 1098.3 ± 10.0 | 1087.8 | 1122.4 | 2.20 |
| sn p -d0 -j8 | 1122.2 ± 18.2 | 1107.3 | 1170.1 | 2.24 |
| dust -d0 | 3532.1 ± 26.4 | 3490.0 | 3563.1 | 7.06 |

<a name="KuRSo"></a>
#### Ubuntu 21.10 安装 diskus：
diskus_0.7.0_amd64.deb 是 diskus GitHub 存储库中的最新版本，从 diskus 的发布页面下载.deb文件，可执行如下命令进行下载和安装。
```bash
wget https://github.com/sharkdp/diskus/releases/download/v0.7.0/diskus_0.7.0_amd64.deb
dpkg -i diskus_0.7.0_amd64.deb
```
对于其他系统，可以参考 GitHub 上 diskus 的其他操作系统安装方式进行安装。
<a name="wLJ8V"></a>
#### diskus 用法：
```bash
diskus [OPTIONS] [path]...
```
<a name="sIx6b"></a>
#### diskus 参数选项：

- -j：设置线程数（默认：3 x num cores），文件大小的输出格式（十进制：MB，二进制：MiB）[默认：十进制] [可能的值：十进制、二进制]
- -v：不隐藏文件系统错误
- -b：计算表观大小而不是磁盘使用量
- -h：帮助信息
- -v：版本信息
<a name="X61wp"></a>
#### diskus 使用：
使用`du -sh 路径`的命令可以查看某个目录的大小，这里的-s参数表示显示总大小。
```bash
# du -sh /etc
16M    /etc
```
如果使用diskus可直接显示当前目录的总大小。
```bash
diskus
```
如果想查看某个目录的大小，可以在`diskus`后面加绝对路径。
```bash
# diskus /etc
16.66 MB (16,662,528 bytes)
```
<a name="hA6Lx"></a>
### 五、tin-summer
tin-summer（sn）是基于 Rust 语言编写的开源工具，也是`du`命令的替代品，可以用于查找占用磁盘空间的文件，以更好的输出、更清晰的命令和默认值，而且由于多线程，它甚至可以在计算大目录下的大小时会比 du 命令运行得更快，tin-summer 与 du 命令之间的区别是前者读取文件的大小，而后者则读取磁盘使用情况。<br />此外，tin-summer 的开发者也针对 du 和 sn 做了鲜明的使用理由对比。
<a name="tfTyl"></a>
#### 使用 du 的理由：

- 可读取磁盘使用情况，而不仅仅是文件大小
- 可选的取消引用符号链接
- 可在小目录上稍微快一点
- 稳定且支持良好
<a name="PASS2"></a>
#### 使用 sn 的理由：

- 在大目录下更快
- 使用正则表达式进行排除，使其在与 --exclude 标志一起使用时比 du 快得多。
- 默认为可读的输出
- 彩色输出
- 通过获得更好的帮助
- 提供排序输出
- 查找构建工件
- 读取文件大小，而不是磁盘使用情况
- 在 Rust 中可扩展

同时，也做了基准测试，从而来对比 sn 和 du 执行不同目录大小的时间结果。

| Directory Size | Tool | Command | Time |
| --- | --- | --- | --- |
| 600MB | sn | sn p | 60.74 ms |
| 600MB | sn | sn d | 99.92 ms |
| 600MB | du | du -hacd2 | 88.28 ms |
| 4GB | sn | sn p | 185.2 ms |
| 4GB | sn | sn d | 271.9 ms |
| 4GB | du | du -hacd2 | 195.5 ms |
| 700MB | sn | sn p | 91.05 ms |
| 700MB | sn | sn d | 176.3 ms |
| 700MB | du | du -hacd2 | 153.8 ms |
| 7MB | sn | sn p | 19.48 ms |
| 7MB | sn | sn d | 12.72 ms |
| 7MB | du | du -hacd2 | 10.13 ms |

<a name="PQ09K"></a>
#### 安装 tin-summer：
```bash
curl -LSfs https://japaric.github.io/trust/install.sh | sh -s -- --git vmchale/tin-summer
```
也可以使用`cargo`软件包管理器安装tin-summer，前提是需要在系统上安装 Rust。如果 Rust 已安装好的话，可执行如下命令：
```bash
cargo install tin-summer
```
如果上述两种安装方式都尚未成功安装 tin-summer，还可以通过 tin-summer GitHub 存储库的发布区下载最新版本的二进制文件进行手动安装。
<a name="S2DIg"></a>
#### tin-summer 的使用：
查看当前目录文件大小，无需添加任何参数，tin-summer 也是默认以易读的格式向用户进行输出，可执行如下命令：
```bash
sn f
```
查看某个特定目录的文件大小，可执行如下命令：
```bash
sn f <path-to-the-directory>
```
在当前目录中搜索包含构建工件的目录，可执行如下命令：
```bash
sn ar
```
查找占用超过 200MB 磁盘空间的工件或包含工件的目录，可执行如下命令：
```bash
sn ar -t200M
```
获取 `$DIR` 中 10 个最大目录的排序列表，可执行如下命令：
```bash
sn sort $DIR -n12
```
sn 实际在操作大目录的时候速度比较快，在操作小目录的时候，速度会相对比较慢一些。
<a name="bgtu5"></a>
### 参考
dust：[https://github.com/bootandy/dust/](https://github.com/bootandy/dust/)<br />duu：[https://github.com/jftuga/duu](https://github.com/jftuga/duu)<br />diskus：[https://github.com/sharkdp/diskus/](https://github.com/sharkdp/diskus/)<br />tin-summer：[https://github.com/vmchale/tin-summer](https://github.com/vmchale/tin-summer)
