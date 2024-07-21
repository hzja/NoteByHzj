Linux命令
<a name="MOSQb"></a>
### crypt
crypt不是存放垃圾文件的地宫或墓坑，而是加密文件内容的命令。<br />如今，“crypt”通常作为一个脚本而实现：通过调用一个名为mcrypt的二进制程序，模拟较旧的crypt命令，从而发挥其功用。直接使用mycrypt命令是一种更好的选择。
```bash
$ mcrypt x
Enter the passphrase (maximum of 512 characters)
Please use a combination of upper and lower case letters and numbers.
Enter passphrase:
Enter passphrase:
File x was encrypted.
```
:::tips
注意：mcrypt命令会创建第二个文件，其扩展名是“.nc”。
:::
它并不覆盖加密的文件。mcrypt命令有密钥大小和加密算法方面的选项。还可以指定密钥作为一个选项，不过mcrypt命令不鼓励这么做。
<a name="LbK6a"></a>
### kill
kill命令用来终止进程的命令，终止的方式有多强硬取决于需要什么来适当地终止它们。当然，Linux并不就此止步。相反，它有各种各样的kill命令来用于命令行操作。<br />有kill、pkill、killall、killpg、rfkill、skill（参阅es-kill）、tgkill、tkill和xkill。
```bash
$ killall runme
[1] Terminated ./runme
[2] Terminated ./runme
[3]- Terminated ./runme
[4]+ Terminated ./runme
```
<a name="SM1Q1"></a>
### shred
Linux系统还支持一种名为shred的命令。shred命令覆盖文件以隐藏以前的内容，并确保无法使用硬盘恢复工具来恢复它们。请记住：rm命令基本上只是删除目录文件中的文件引用，但不一定从磁盘上擦除内容或覆盖内容。shred命令才是覆盖文件的内容。
```bash
$ shred dupes.txt
$ more dupes.txt
▒oΛ▒▒9▒lm▒▒▒▒▒o▒1־▒▒f▒f▒▒▒i▒▒h^}&▒▒▒{▒▒
```
<a name="G4ueD"></a>
### zombies
虽然zombies（僵尸进程）不是一个命令，但它在Linux系统上却根深蒂固。zombies基本上是没有被完全清除的死亡进程的残骸。<br />进程不应该以这种方式运行——任由死亡进程逗留，而不是仅仅让它们死亡、进入数字天堂，所以zombies的存在表明了留下它们的进程存在某种缺陷。<br />检查系统是否有僵尸进程残留下来，一个简单的方法就是查看top命令的标题行。
```bash
$ top
top - 18:50:38 up 6 days, 6:36, 2 users, load average: 0.00, 0.00, 0.00
Tasks: 171 total, 1 running, 167 sleeping, 0 stopped, 3 zombie <==
%Cpu(s): 0.0 us, 0.0 sy, 0.0 ni, 99.9 id, 0.1 wa, 0.0 hi, 0.0 si, 0.0 st
KiB Mem : 2003388 total, 250840 free, 545832 used, 1206716 buff/cache
KiB Swap: 9765884 total, 9765764 free, 120 used. 1156536 avail Mem
```
<a name="qHiT7"></a>
### at midnight
Linux让用户可以借助“at midnight”命令跟踪是否已离开。at用来安排下一次指定时间到来时就运行的任务，工作方式类似一次性计划任务（cron）。
```bash
$ at midnight
warning: commands will be executed using /bin/sh
at> echo 'the spirits of the dead have left'
at> <EOT>
job 3 at Thu Oct 31 00:00:00 2017
```
<a name="I8bK7"></a>
### daemons
Linux系统还高度依赖守护进程（daemon）——这种进程在后台运行，提供了系统的许多功能。许多守护进程的名称以“d”结尾。这个“d”代表“daemon”，它表明该进程一直运行，支持某种重要功能。另一些守护进程则将“daemon”这个单词拼出来。
```bash
$ ps -ef | grep sshd
root 1142 1 0 Oct19 ? 00:00:00 /usr/sbin/sshd -D
root 25342 1142 0 18:34 ? 00:00:00 sshd: shs [priv]
$ ps -ef | grep daemon | grep -v grep
message+ 790 1 0 Oct19 ? 00:00:01 /usr/bin/dbus-daemon --system --address=systemd: --nofork --nopidfile --systemd-activation
root 836 1 0 Oct19 ? 00:00:02 /usr/lib/accountsservice/accounts-daemon
```
