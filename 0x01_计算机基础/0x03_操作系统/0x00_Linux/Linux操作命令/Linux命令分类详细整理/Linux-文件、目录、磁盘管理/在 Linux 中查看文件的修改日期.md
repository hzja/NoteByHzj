Linux 
<a name="iKuyr"></a>
### 使用stat命令
stat命令可以显示文件属性的详细信息，比如最近一次访问和修改文件的时间、文件大小等信息，使用起来比较简单，命令后面只需要加上文件名就可以：
```
[root@localhost ~]# stat install.sh
  File: ‘install.sh’
  Size: 159             Blocks: 8          IO Block: 4096   regular file
Device: fd01h/64769d    Inode: 2115734     Links: 1
Access: (0755/-rwxr-xr-x)  Uid: (    0/    root)   Gid: (    0/    root)
Access: 2021-01-12 11:04:08.055028755 +0800
Modify: 2021-01-12 11:04:02.610016788 +0800
Change: 2021-01-12 11:04:02.612016793 +0800
 Birth: -
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1611715954853-2d791f45-3155-4135-87c2-df8fa47aca79.png#align=left&display=inline&height=197&originHeight=591&originWidth=3323&size=665272&status=done&style=none&width=1107.6666666666667)<br />从上面的输出中，可以看到文件的访问日期、文件的修改日期、文件权限的修改日期以及其他参数。<br />如果只希望查看文件的修改日期，而不考虑所有其他信息，运行以下命令:
```
[root@localhost ~]# stat -c %y install.sh
2021-01-12 11:04:02.610016788 +0800
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1611716001230-846ce828-f339-4cad-aae2-fa96feb0ec8c.png#align=left&display=inline&height=70&originHeight=211&originWidth=3323&size=179239&status=done&style=none&width=1107.6666666666667)<br />`-c`选项用于指定自定义格式代替默认的输出，而'`％y`'标志显示上次修改时间。对于文件夹，语法保持不变。只需将文件名替换为文件夹名称即可。
<a name="HMO3R"></a>
### 使用`date`命令
`date`命令的用法是显示当前日期。但是当与`-r`选项一起使用时，可以显示文件的最后修改日期，如下所示：
```bash
[root@localhost ~]# date -r date -r install.sh
Tue Jan 12 11:04:02 CST 2021
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1611716056066-4f447824-de04-40b4-b126-da1f3cd7493e.png#align=left&display=inline&height=71&originHeight=214&originWidth=3323&size=180744&status=done&style=none&width=1107.6666666666667)
<a name="KUjcz"></a>
### 使用`ls -l`命令
`ls -l`命令通常用于使用长列表显示有关文件的其他信息，例如文件权限和所有者，大小和创建日期。可以添加`-t`选项，这样就可以按照文件的修改时间来排列：
```bash
[root@localhost ~]# ls -lt
或者
[root@localhost ~]# ll -t
total 288
drwxr-xr-x. 2 root root    177 Oct 16 14:36 b
drwxr-xr-x. 2 root root    177 Oct 16 14:36 a
-rwxr-xr-x. 1 root root    119 Oct 15 19:20 backup_script.sh
-rwxr-xr-x. 1 root root     31 Oct 15 19:07 hello_script.sh
-rw-r--r--. 1 root root    227 Oct 13 16:39 content.txt
-rw-r--r--. 1 root root 277159 Oct 12 14:37 a.txt
drwxr-xr-x. 2 root root    195 Aug  6 14:12 Files
-rw-------. 1 root root   1284 Dec 29  2019 anaconda-ks.cfg
```
<a name="RlJIM"></a>
### 使用`httpie`工具
另一种检查文件的修改日期的方法是使用httpie ，是HTTP命令行客户端工具。该工具通常用于与HTTP服务器和API交互，还可以检查驻留在web服务器上文件的修改时间。<br />首先需要确保安装了Python的pip包管理工具，然后安装httpie工具：<br />在Centos7/RHEL7中，运行以下命令安装httpie：
```bash
[root@localhost ~]# yum -y install python-pip
[root@localhost ~]# pip install --upgrade pip
[root@localhost ~]# pip install httpie
```
在Ubuntu / Deepin / Debian中运行以下命令安装httpie：
```bash
$ sudo apt install httpie
```
安装完成之后，查看web服务器上文件的修改时间的语法如下：
```bash
http -h  [url] | grep 'Last-Modified'
```
例如，从www.fcant.com网站中，查看一张.png格式的图片修改时间：
```bash
[root@localhost ~]# http -h https://www.fcant.com/wp-content/uploads/2020/06/6-1.png | grep -i 'Last-Modified'
Last-Modified: Fri, 05 Jun 2020 14:26:11 GMT
```

