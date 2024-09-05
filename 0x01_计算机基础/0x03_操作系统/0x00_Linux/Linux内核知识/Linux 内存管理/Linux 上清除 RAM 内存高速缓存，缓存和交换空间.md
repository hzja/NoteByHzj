Linux 清除RAM内存高速缓存 <br />像任何其他的操作系统一样，GNU / Linux已经有效地实施了内存管理甚至更多。但是，如果有任何进程正在消耗内存，就需要清除它，Linux提供了一个方法来刷新或清除RAM缓存。
<a name="Oc41w"></a>
### 在Linux中如何清除缓存？
每一个Linux系统有三个选项来清除缓存而不中断任何进程或服务。
<a name="mFtBC"></a>
#### 1、仅清除缓存页
```bash
sync; echo 1 > /proc/sys/vm/drop_caches
```
<a name="diq0G"></a>
#### 2、清除目录项和inodes
```bash
sync; echo 2 > /proc/sys/vm/drop_caches
```
<a name="5QUhS"></a>
#### 3、清除,缓存页，目录项和inodes
```bash
sync; echo 3 > /proc/sys/vm/drop_caches
```
上述命令的说明：sync将刷新文件系统缓存，命令通过“;”分隔，顺序执行，shell等待终止在序列中的每一个命令执行之前。正如内核文档中提到的，写到drop_cache将清空缓存而不杀死任何应用程序/服务，echo命令做写入文件的工作。如果必须清除磁盘高速缓存，第一个命令在企业和生产环境中是最安全，“…echo 1> …”只会清除页缓存。不建议使用上面第三个选项在生产环境中“…echo 3 >” ，除非明确自己在做什么，因为它会清除缓存页，目录项和inodes。<br />在Linux上释放Buffer和Cache要用到内核是否是个好主意？当请求许多设定想要检查时，如果它实际上是专门实施对I/O 广泛的基准测试，那么可能需要清除缓存。可以如上所示删除缓存，无需重新启动系统即无需停机。Linux被设计成它在寻找磁盘之前到磁盘缓存寻找的方式。如果它发现该资源在缓存中，则该请求不到达磁盘。如果清理缓存，磁盘缓存将没有用处，系统会到磁盘上寻找资源。此外，当清除缓存后它也将减慢系统运行速度，系统会重新加载每一个被请求的资源再次到磁盘缓存中。现在，将通过一个cron任务调度器创建一个shell脚本在每天下午2点自动清除RAM缓存。创建一个shell脚本clearcache.sh并在其中添加以下行：
```bash
#!/bin/bash
# Note, we are using "echo 3", but it is not recommended in production instead use "echo 1"
echo "echo 3 > /proc/sys/vm/drop_caches"
```
给clearcache.sh文件设置执行权限
```bash
$ chmod 755 clearcache.sh
```
现在，当需要清除RAM缓存时只需要调用脚本。现在设置一个定时任务来清除RAM缓存每天在下午2点，打开crontab进行编辑。
```bash
$ crontab -e
```
添加以下行，保存并退出。
```bash
0 3 * * * /path/to/clearcache.sh
```
在生产环境的服务器上自动清除RAM并不是是一个好主意。想想一个情况，当已经预定脚本来清除RAM缓存每天在下午2点。每天下午2点该脚本会执行并刷新RAM缓存。在一天中的任何时候，网站用户的在线量可能会超过预期的，并从服务器请求资源。同时调度器运行着脚本，并在高速缓存中清除一切。当所有的用户都从磁盘读取数据时，这将导致服务器崩溃并损坏数据库。因此，清除缓存仅在必要时并且在预料之中。
<a name="Uws1J"></a>
### 如何清除Linux的交换空间？
如果想清除交换空间，可以运行下面的命令：
```bash
$ swapoff -a && swapon -a
```
此外，了解有关风险后，可能会将上面的命令添加到cron中。现在，将上面两种命令结合成一个命令写出正确的脚本来同时清除RAM缓存和交换空间。
```bash
$ echo 3 > /proc/sys/vm/drop_caches && swapoff -a && swapon -a && printf '\n%s\n' 'Ram-cache and Swap Cleared'
```
或
```bash
su -c 'echo 3 >/proc/sys/vm/drop_caches' && swapoff -a && swapon -a && printf '\n%s\n' 'Ram-cache and Swap Cleared'
```
在测试上面的命令之前，先运行“free -m” 然后执行脚本检查缓存。
