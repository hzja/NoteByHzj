Linux<br />对于linux下误删的文件，是否真的无法通过软件进行恢复呢?
<a name="EouvG"></a>
### 误删文件还原可以分为两种情况

- 一种是删除以后在进程存在删除信息
- 一种是删除以后进程都找不到，只有借助于工具还原。
<a name="YbQIe"></a>
### 这里只分析文件被删除后，相关进程还存在的情况
这种一般是有活动的进程存在持续标准输入或输出，到时文件被删除后，进程PID依旧存在。这也是有些服务器删除一些文件但是磁盘不释放的原因
<a name="kJr2A"></a>
### 案例演示
<a name="tUHgJ"></a>
#### 创建一个文件
```bash
> vim  rumenz.txt
123
//保存退出
> cat rumenz.txt
123
```
<a name="gghTc"></a>
#### 用tail -f打开rumenz.txt文件
目的是让rumenz.txt被删除后，进程依然存在
```bash
> tail -f rumenz.txt
```
<a name="PnoGE"></a>
#### 新开一个终端删除rumenz.txt
```bash
> rm -f rumenz.txt
```
<a name="IstrE"></a>
#### 找到占用rumenz.txt的进程

- lsof查看删除的文件进程是否还存在。
- 如没有安装请自行`yum install lsof`或者`apt-get install lsof`
```bash
> lsof | grep delete | grep rumenz
tail      10222          root    3r      REG              253,1          4   70911074 /root/test/rumenz.txt (deleted)
```
进程ID是10222，从上面可以看出，当前文件状态为已删除（deleted）
<a name="j2EJi"></a>
#### 恢复文件

- /proc/10222/fd：进程操作的文件描述符目录
```bash
> cd /proc/10222/fd
> ls -al
dr-x------ 2 root root  0 May 11 21:41 .
dr-xr-xr-x 9 root root  0 May 11 21:41 ..
lrwx------ 1 root root 64 May 11 21:41 0 -> /dev/pts/1
lrwx------ 1 root root 64 May 11 21:41 1 -> /dev/pts/1
lrwx------ 1 root root 64 May 11 21:41 2 -> /dev/pts/1
lr-x------ 1 root root 64 May 11 21:41 3 -> /root/test/rumenz.txt (deleted)
lr-x------ 1 root root 64 May 11 21:41 4 -> anon_inode:inotify
```
开始恢复文件
```bash
> cp 3 /root/test/rumenz.txt
> cat /root/test/rumenz.txt
123
```
<a name="Acels"></a>
#### 能恢复的根本原因
当程序运行时，操作系统会专门开辟一块内存区域，提供给当前进程使用，对于依赖的文件，操作系统会发放一个文件描述符，以便读写文件，当执行`rm -f`删除文件时，其实只是删除了文件的目录索引节点，对于文件系统不可见，但是对于打开它的进程依然可见，即仍然可以使用先前发放的文件描述符读写文件，正是利用这样的原理，所以可以使用I/O重定向的方式来恢复文件。
