Shell 一个命令解释器，它接收应用程序/用户命令，然后调用操作系统内核。<br />Shell还是一个功能强大的编程语言，易编写、易调试、灵活性强。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1693060621788-2d5f1c66-1c9f-4e9a-9102-16f855054dba.jpeg)
<a name="K4nMt"></a>
## Linux提供的Shell解析器有
```bash
[root@test001 ~]# cat /etc/shells 
/bin/sh
/bin/bash
/usr/bin/sh
/usr/bin/bash
/bin/tcsh
/bin/csh
```
<a name="UrwSo"></a>
## bash 和 sh的关系
```bash
[root@test001 ~]# ll /bin/ | grep bash
-rwxr-xr-x. 1 root root     964536 4月   1 2020 bash
lrwxrwxrwx. 1 root root          4 5月  15 14:33 sh -> bash
```
<a name="JbDZY"></a>
## Centos默认的Shell解析器是bash
```bash
[root@test001 ~]# echo $SHELL
/bin/bash
```
