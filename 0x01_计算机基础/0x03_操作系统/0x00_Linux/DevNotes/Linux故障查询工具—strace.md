Linux strace
<a name="efedd489"></a>
### strace**
strace是一个非常简单的工具，它可以跟踪系统调用的执行。最简单的方式，它可以从头到尾跟踪binary的执行，然后以一行文本输出系统调用的名字，参数和返回值。<br />其实它可以做的更多：

- 可以对特定的系统调用或者几组系统调用进行过滤
- 可以通过统计特定系统调用的调用次数、耗费的时间、成功和失败的次数来配置(profile)系统调用的使用 I
- 跟踪发送给进程的信号量
- 可以通过pid附着(attach)到任何运行的进程

如果使用的是其它Unix系统，它类似于"truss"。其它更复杂的是Sun的Dtrace.
<a name="Im5Dd"></a>
### strace的使用
<a name="OJgpM"></a>
#### 1) 找出程序在startup的时候读取的哪个config文件
为什么某些程序不去读取认为它应该读取的config文件的问题？
```bash
$ strace php 2>&1 | grep php.ini  
open("/usr/local/bin/php.ini", O_RDONLY) = -1 ENOENT (No such file or directory)  
open("/usr/local/lib/php.ini", O_RDONLY) = 4  
lstat64("/usr/local/lib/php.ini", {st_mode=S_IFLNK|0777, st_size=27, ...}) = 0  
readlink("/usr/local/lib/php.ini", "/usr/local/Zend/etc/php.ini", 4096) = 27  
lstat64("/usr/local/Zend/etc/php.ini", {st_mode=S_IFREG|0664, st_size=40971, ...}) = 0
```
可以看出这个版本的PHP从/usr/local/lib/php.init读取config文件（但是先尝试/usr/locl/bin)<br />如果只关心特定的系统调用，有更精致的方法
```bash
$ strace -e open php 2>&1 | grep php.ini  
open("/usr/local/bin/php.ini", O_RDONLY) = -1 ENOENT (No such file or directory)  
open("/usr/local/lib/php.ini", O_RDONLY) = 4
```
相同的方法适用于很多其它类似的问题。比如说，安装了不同版本的library，不确定实际上加载了哪一个版本。<br />`-e expr -- a qualifying expression: option=[!]all or option=[!]val1[,val2]...`<br />`options: trace, abbrev, verbose, raw, signal, read, write`
<a name="OOBa5"></a>
#### 2) 为什么这个程序没有打开相应的文件？
是否曾经碰到过一个程序拒绝读取它没有权限的文件，对程序跟踪open,access调用，注意失败的情况
```bash
$ strace -e open,access 2>&1 | grep your-filename
```
<a name="62rTE"></a>
#### 3) 某个进程现在在做什么
某个进程突然占用了很多CPU? 或者某个进程看起来像hanging了？<br />找到对应的pid，然后 <br />hang:悬挂,挂起的意思<br />就是一个进程被暂时停止执行.
```bash
root@dev:~# strace -p 15427  
Process 15427 attached - interrupt to quit  
futex(0x402f4900, FUTEX_WAIT, 2, NULL   
Process 15427 detached 
```
这个例子里面，它在调用futex()的时候挂起了。<br />"strace -p"非常有用，它减少了很多猜测工作，也不需要重新启动应用。<br />` -p pid -- trace process with process id PID, may be repeated`
<a name="eIPsm"></a>
#### 4) 查看程序执行的时间
可以重新编译app，打开profiling，以获取精确的信息。但是通常利用strace附着(attach）一个进程以快速地看一下当前时间花费在哪里非常有用。可以看下是否90%的CPU用在真正的工作，或者用在其它方面了。
```bash
root@dev:~# strace -c -p 11084  
Process 11084 attached - interrupt to quit  
Process 11084 detached  
% time     seconds  usecs/call     calls    errors syscall  
------ ----------- ----------- --------- --------- ----------------  
 94.59    0.001014          48        21           select  
  2.89    0.000031           1        21           getppid  
  2.52    0.000027           1        21           time  
------ ----------- ----------- --------- --------- ----------------  
100.00    0.001072                    63           total  
root@dev:~#
```
`-c -- count time, calls, and errors for each syscall and report summary`<br />`-C -- like -c but also print regular output`<br />在执行`strace -c -p`命令以后，等到关注的时间到了后，按ctrl-c退出，strace会列出如上的profiling数据。<br />在这个例子中，程序花了绝大部分时间在等待select()。它在每一个slect()调用这件调用getpid()和time()，这是一种典型的事件循环。<br />也可以运行"start to finish"，这里是"ls"
```bash
root@dev:~# strace -c >/dev/null ls  
% time     seconds  usecs/call     calls    errors syscall  
------ ----------- ----------- --------- --------- ----------------  
 23.62    0.000205         103         2           getdents64  
 18.78    0.000163          15        11         1 open  
 15.09    0.000131          19         7           read  
 12.79    0.000111           7        16           old_mmap  
  7.03    0.000061           6        11           close  
  4.84    0.000042          11         4           munmap  
  4.84    0.000042          11         4           mmap2  
  4.03    0.000035           6         6         6 access  
  3.80    0.000033           3        11           fstat64  
  1.38    0.000012           3         4           brk  
  0.92    0.000008           3         3         3 ioctl  
  0.69    0.000006           6         1           uname  
  0.58    0.000005           5         1           set_thread_area  
  0.35    0.000003           3         1           write  
  0.35    0.000003           3         1           rt_sigaction  
  0.35    0.000003           3         1           fcntl64  
  0.23    0.000002           2         1           getrlimit  
  0.23    0.000002           2         1           set_tid_address  
  0.12    0.000001           1         1           rt_sigprocmask  
------ ----------- ----------- --------- --------- ----------------  
100.00    0.000868                    87        10 total
```
正如预期，它耗费了大部分时间在两次调用来读取目录条目上（因为运行于一个小的目录上，所有只有两次）
<a name="7da11f20"></a>
#### 5) 无法连接到服务器的问题追踪
调试进程无法连接到远端服务器有时候是件非常头痛的事。DNS会失败，connect会挂起，server有可能返回一些意料之外的数据。<br />可以使用tcpdump来分析这些情况，它是一个非常棒的工作。但是有时候strace可以从更简单，耿直借的角度，因为strace只返回进程相关的系统调用产生的数据。<br />如果要从100个连接到统一个数据服务器的运行进程里面找出一个连接所做的事情，用strace就比tcpdump简单得多。<br />下面是跟踪"nc"连接到www.news.com 80端口的例子
```bash
$ strace -e poll,select,connect,recvfrom,sendto nc www.news.com 80  
sendto(3, "\\24\\0\\0\\0\\26\\0\\1\\3\\255\\373NH\\0\\0\\0\\0\\0\\0\\0\\0", 20, 0, {sa_family=AF_NETLINK, pid=0, groups=00000000}, 12) = 20  
connect(3, {sa_family=AF_FILE, path="/var/run/nscd/socket"}, 110) = -1 ENOENT (No such file or directory)  
connect(3, {sa_family=AF_FILE, path="/var/run/nscd/socket"}, 110) = -1 ENOENT (No such file or directory)  
connect(3, {sa_family=AF_INET, sin_port=htons(53), sin_addr=inet_addr("62.30.112.39")}, 28) = 0  
poll([{fd=3, events=POLLOUT, revents=POLLOUT}], 1, 0) = 1  
sendto(3, "\\213\\321\\1\\0\\0\\1\\0\\0\\0\\0\\0\\0\\3www\\4news\\3com\\0\\0\\34\\0\\1", 30, MSG_NOSIGNAL, NULL, 0) = 30  
poll([{fd=3, events=POLLIN, revents=POLLIN}], 1, 5000) = 1  
recvfrom(3, "\\213\\321\\201\\200\\0\\1\\0\\1\\0\\1\\0\\0\\3www\\4news\\3com\\0\\0\\34\\0\\1\\300\\f"..., 1024, 0, {sa_family=AF_INET, sin_port=htons(53), sin_addr=inet_addr("62.30.112.39")}, [16]) = 153  
connect(3, {sa_family=AF_INET, sin_port=htons(53), sin_addr=inet_addr("62.30.112.39")}, 28) = 0  
poll([{fd=3, events=POLLOUT, revents=POLLOUT}], 1, 0) = 1  
sendto(3, "k\\374\\1\\0\\0\\1\\0\\0\\0\\0\\0\\0\\3www\\4news\\3com\\0\\0\\1\\0\\1", 30, MSG_NOSIGNAL, NULL, 0) = 30  
poll([{fd=3, events=POLLIN, revents=POLLIN}], 1, 5000) = 1  
recvfrom(3, "k\\374\\201\\200\\0\\1\\0\\2\\0\\0\\0\\0\\3www\\4news\\3com\\0\\0\\1\\0\\1\\300\\f"..., 1024, 0, {sa_family=AF_INET, sin_port=htons(53), sin_addr=inet_addr("62.30.112.39")}, [16]) = 106  
connect(3, {sa_family=AF_INET, sin_port=htons(53), sin_addr=inet_addr("62.30.112.39")}, 28) = 0  
poll([{fd=3, events=POLLOUT, revents=POLLOUT}], 1, 0) = 1  
sendto(3, "\\\\\\2\\1\\0\\0\\1\\0\\0\\0\\0\\0\\0\\3www\\4news\\3com\\0\\0\\1\\0\\1", 30, MSG_NOSIGNAL, NULL, 0) = 30  
poll([{fd=3, events=POLLIN, revents=POLLIN}], 1, 5000) = 1  
recvfrom(3, "\\\\\\2\\201\\200\\0\\1\\0\\2\\0\\0\\0\\0\\3www\\4news\\3com\\0\\0\\1\\0\\1\\300\\f"..., 1024, 0, {sa_family=AF_INET, sin_port=htons(53), sin_addr=inet_addr("62.30.112.39")}, [16]) = 106  
connect(3, {sa_family=AF_INET, sin_port=htons(80), sin_addr=inet_addr("216.239.122.102")}, 16) = -1 EINPROGRESS (Operation now in progress)  
select(4, NULL, [3], NULL, NULL)        = 1 (out [3])
```
注意到尝试连接/var/run/nscd/socket?这意味着nc首先尝试连接NSCD--the Name Service Cache Daemon--它通常用来基于NIS,YP,LDAP或者类似的目录协议提供域名查询。在这里它失败了。<br />然后它连接DNS（DNS是port 53,所以"sin_port=htons(53)")。然后它调用了"sendto()",发送包含www.news.com的DNS 包。<br />然后读回响应。不知为何，它尝试了三次，最后一次有细微的却别，猜测是它www.news.com十一个CNAME（别名），多次请求可能是nc故意的。<br />最后，它发起一个connect()请求到得到的IP地址，注意到返回值是EINPROGRESS。这意味这connect是非阻塞的，nc希望继续处理，然后它调用slect()，连接建立后，select返回成功。<br />添加"read","write"到过滤系统调用列表中，连接时输入一个字串，可能会得到如下
> Notice the connection attempts to /var/run/nscd/socket? They mean nc first tries to connect to NSCD - the Name Service Cache Daemon - which is usually used in setups that rely on NIS, YP, LDAP or similar directory protocols for name lookups. In this case the connects fails.

> It then moves on to DNS (DNS is port 53, hence the "sin_port=htons(53)" in the following connect. You can see it then does a "sendto()" call, sending a DNS packet that contains www.news.com. It then reads back a packet. For whatever reason it tries three times, the last with a slightly different request. My best guess why in this case is that www.news.com is a CNAME (an "alias"), and the multiple requests may just be an artifact of how nc deals with that.

> Then in the end, it finally issues a connect() to the IP it found. Notice it returns EINPROGRESS. That means the connect was non-blocking - nc wants to go on processing. It then calls select(), which succeeds when the connection was successful.

> Try adding "read" and "write" to the list of syscalls given to strace and enter a string when connected, and you'll get something like this:

```
read(0, "test\\n", 1024)                 = 5  
write(3, "test\\n", 5)                   = 5  
poll([{fd=3, events=POLLIN, revents=POLLIN}, {fd=0, events=POLLIN}], 2, -1) = 1  
read(3, "
```
这表示从标准输入读入"test"+换行符，并写到网络连接中，然后调用poll等待响应，读取响应，写回标准输出。<br />一切看起来都正常工作。
<a name="bgMg7"></a>
### strace使用示例
看一台高负载服务器的 top 结果：<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1607434724356-6de3ba54-9e20-438f-ba39-7e6764467973.webp#align=left&display=inline&height=590&originHeight=590&originWidth=690&size=0&status=done&style=none&width=690)<br />技巧：运行 top 时，按「1」打开 CPU 列表，按「shift+p」以 CPU 排序。<br />在本例中大家很容易发现 CPU 主要是被若干个 PHP 进程占用了，同时 PHP 进程占用的比较多的内存，不过系统内存尚有结余，SWAP 也不严重，这并不是问题主因。<br />不过在 CPU 列表中能看到 CPU 主要消耗在内核态「sy」，而不是用户态「us」，和经验不符。Linux 操作系统有很多用来跟踪程序行为的工具，内核态的函数调用跟踪用「strace」，用户态的函数调用跟踪用「ltrace」，所以这里应该用「strace」：
```bash
shell> strace -p <PID>
```
不过如果直接用 strace 跟踪某个进程的话，那么往往是满屏翻滚的字符，想从这里看出问题的症结并不是一件容易的事情，好在 strace 可以按操作汇总时间：
```bash
shell> strace -cp <PID>
```
通过「c」选项用来汇总各个操作的总耗时，运行后的结果大概如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1607434724377-99255016-ba41-4883-9df5-c98e747647bf.webp#align=left&display=inline&height=383&originHeight=383&originWidth=690&size=0&status=done&style=none&width=690)<br />很明显，能看到 CPU 主要被 clone 操作消耗了，还可以单独跟踪一下 clone：
```bash
shell> strace -T -e clone -p <PID>
```
通过「T」选项可以获取操作实际消耗的时间，通过「e」选项可以跟踪某个操作：<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1607434724356-3455ae7a-1c1a-42be-bc3b-acdedb533442.webp#align=left&display=inline&height=163&originHeight=163&originWidth=690&size=0&status=done&style=none&width=690)<br />很明显，一个 clone 操作需要几百毫秒，至于 clone 的含义，参考 man 文档：
> clone() creates a new process, in a manner similar to fork(2). It is actually a library function layered on top of the underlying clone() system call, hereinafter referred to as sys_clone. A description of sys_clone is given towards the end of this page.

> Unlike fork(2), these calls allow the child process to share parts of its execution context with the calling process, such as the memory space, the table of file descriptors, and the table of signal handlers. (Note that on this manual page, “calling process” normally corresponds to “parent process”. But see the description of CLONE_PARENT below.)

简单来说，就是创建一个新进程。那么在 PHP 里什么时候会出现此类系统调用呢？查询业务代码看到了 exec 函数，通过如下命令验证它确实会导致 clone 系统调用：
```bash
shell> strace -eclone php -r 'exec("ls");
```
最后再思考一个问题：如果用 strace 跟踪一个进程，输出结果很少，是不是说明进程很空闲？其实试试 ltrace，可能会发现别有洞天。记住有内核态和用户态之分。
<a name="HBD97"></a>
### `strace -f -e trace=read,write -p 17151 -o log` 跟踪进程17151及子进程中read和write系统调用，输出到log文件.
-e expr <br />指定一个表达式,用来控制如何跟踪.格式如下: <br />[qualifier=][!]value1[,value2]... <br />qualifier 只能是 trace,abbrev,verbose,raw,signal,read,write其中之一.<br />value是用来限定的符号或数字.<br />默认的 qualifier是 trace. <br />感叹号是否定符号.<br />例如: <br />`-e open`等价于 `-e trace=open`,表示只跟踪open调用.<br />而-etrace!=open表示跟踪除了open以外的其他调用.<br />有两个特殊的符号 all 和 none. <br />注意有些shell使用!来执行历史记录里的命令,所以要使用\\. <br />-e trace= <br />只跟踪指定的系统 调用.例如:-e trace=open,close,rean,write表示只跟踪这四个系统调用.默认的为set=all. <br />-e trace=file <br />只跟踪有关文件操作的系统调用. <br />-e trace=process 只跟踪有关进程控制的系统调用. <br />只跟踪有关进程控制的系统调用. <br />-e trace=network 跟踪与网络有关的所有系统调用. <br />跟踪与网络有关的所有系统调用. <br />-e strace=signal 跟踪所有与系统信号有关的 系统调用 <br />跟踪所有与系统信号有关的 系统调用 <br />-e trace=ipc 跟踪所有与进程通讯有关的系统调用 <br />跟踪所有与进程通讯有关的系统调用 <br />-e abbrev= 设定 strace输出的系统调用的结果集.-v 等与 abbrev=none.默认为abbrev=all. <br />设定 strace输出的系统调用的结果集.-v 等与 abbrev=none.默认为abbrev=all. <br />-e raw= 将指 定的系统调用的参数以十六进制显示. <br />将指 定的系统调用的参数以十六进制显示. <br />-e signal= 指定跟踪的系统信号.默认为all.如 signal=!SIGIO(或者signal=!io),表示不跟踪SIGIO信号. <br />指定跟踪的系统信号.默认为all.如 signal=!SIGIO(或者signal=!io),表示不跟踪SIGIO信号. <br />-e read= 输出从指定文件中读出 的数据.例如: <br />输出从指定文件中读出 的数据.例如: <br />-e read=, <br />-e write= <br />通用的完整用法：
```bash
strace -o output.txt -T -tt -e trace=all -p 28979
```
上面的含义是 跟踪28979进程的所有系统调用（-e trace=all），并统计系统调用的花费时间，以及开始时间（并以可视化的时分秒格式显示），最后将记录结果存在output.txt文件里面。<br />语法<br />`strace [ -dffhiqrtttTvxx ] [ -acolumn ] [ -eexpr ] ... [ -ofile ] [-ppid ] ... [ -sstrsize ] [ -uusername ] [ -Evar=val ] ... [ -Evar ]... [ command [ arg ... ] ]`<br />`strace -c [ -eexpr ] ... [ -Ooverhead ] [ -Ssortby ] [ command [ arg... ] ]`
