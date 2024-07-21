网络<br />gping 是基于 Rust 编写可视化工具，具有动态图形化界面显示。<br />![](https://cdn.nlark.com/yuque/0/2021/gif/396745/1640596404301-e18dc980-7916-41d5-aaf8-277ec9fb7a1d.gif#clientId=ua55de7ef-d715-4&from=paste&id=u2fb05492&originHeight=519&originWidth=1079&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2bf0c40a-c7a8-421f-b8d5-2cffacfbf37&title=)<br />Github仓库地址：[https://github.com/orf/gping/releases](https://github.com/orf/gping/releases)
<a name="DasG2"></a>
## 安装
[gping-Windows-x86_64.zip](https://www.yuque.com/attachments/yuque/0/2021/zip/396745/1640596752197-dfaea79b-2f2f-433c-8c82-880ca1df708d.zip?_lake_card=%7B%22src%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2021%2Fzip%2F396745%2F1640596752197-dfaea79b-2f2f-433c-8c82-880ca1df708d.zip%22%2C%22name%22%3A%22gping-Windows-x86_64.zip%22%2C%22size%22%3A445299%2C%22type%22%3A%22application%2Fx-zip-compressed%22%2C%22ext%22%3A%22zip%22%2C%22status%22%3A%22done%22%2C%22taskId%22%3A%22ub5f1d5ce-bb6d-4c36-a154-5da53fd48fc%22%2C%22taskType%22%3A%22upload%22%2C%22id%22%3A%22u41d7d17c%22%2C%22card%22%3A%22file%22%7D)<br />下载系统所对应的安装包<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1640596447439-e023a3be-0b4d-40cf-a558-3377757847ca.png#clientId=ua55de7ef-d715-4&from=paste&height=490&id=ubc7ebce7&originHeight=1471&originWidth=2271&originalType=binary&ratio=1&rotation=0&showTitle=false&size=203092&status=done&style=none&taskId=uf59076a0-df93-40fd-9f21-8520079cbee&title=&width=757)<br />本文以CentOS7系统为例，其它系统请参考官方文档。
```bash
[root@centos7 ~]# wget https://github.com/orf/gping/releases/download/gping-v1.2.6/gping-Linux-x86_64.tar.gz
[root@centos7 ~]# ll gping-Linux-x86_64.tar.gz 
-rw-r--r-- 1 root root 889865 Dec  7 22:20 gping-Linux-x86_64.tar.gz
[root@centos7 ~]# mkdir /usr/local/gping
[root@centos7 ~]# tar zxf gping-Linux-x86_64.tar.gz -C /usr/local/gping
```
如果安装完后，运行命令出现下述错误：
```bash
./gping: /lib64/libc.so.6: version `GLIBC_2.18' not found (required by ./gping)
```
解决方案如下：
```bash
curl -O http://ftp.gnu.org/gnu/glibc/glibc-2.18.tar.gz
tar zxf glibc-2.18.tar.gz 
cd glibc-2.18/
mkdir build
cd build/
../configure --prefix=/usr
make -j2
make install
```
<a name="ALoW5"></a>
## 使用
```bash
[root@centos7 gping]# ./gping --help
gping 1.2.6
Ping, but with a graph.

USAGE:
    gping [FLAGS] [OPTIONS] <hosts-or-commands>...

FLAGS:
        --cmd                Graph the execution time for a list of commands rather than pinging hosts
    -h, --help               Prints help information
    -4                       Resolve ping targets to IPv4 address
    -6                       Resolve ping targets to IPv6 address
    -s, --simple-graphics    Uses dot characters instead of braille
    -V, --version            Prints version information

OPTIONS:
    -b, --buffer <buffer>                    Determines the number of seconds to display in the graph. [default: 30]
    -n, --watch-interval <watch-interval>    Watch interval seconds (provide partial seconds like '0.5'). Default for
                                             ping is 0.2, default for cmd is 0.5.

ARGS:
    <hosts-or-commands>...    Hosts or IPs to ping, or commands to run if --cmd is provided.
```
它还可以同时对多个目标地址进行操作。<br />![](https://cdn.nlark.com/yuque/0/2021/gif/396745/1640596404368-77b21e2f-4ed8-4290-9e33-5b0834c3b866.gif#clientId=ua55de7ef-d715-4&from=paste&id=u42dca1e5&originHeight=519&originWidth=1079&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u96dff868-cb8b-4283-a887-d808d61d25a&title=)
