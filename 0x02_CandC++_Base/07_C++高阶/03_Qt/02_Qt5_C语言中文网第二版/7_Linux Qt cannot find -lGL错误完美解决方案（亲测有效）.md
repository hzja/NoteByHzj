对于很多 Linux 发行版本，[Qt](https://c.biancheng.net//qt/) 安装完成后如果直接编译或者运行项目，会出现“cannot find -lGL”错误，如下图所示：

![cannot find -lGL](https://c.biancheng.net//uploads/allimg/190531/1-1Z531142I3161.gif)

这是因为 Qt 找不到 OpenGL 的动态链接库（libGL.so）。在《[Linux Qt安装](https://c.biancheng.net//view/3886.html)》一节中我们说过，OpenGL 在大部分 [Linux 发行版](https://c.biancheng.net//view/710.html)中都是默认安装的，包括 Ubuntu、CentOS 等，找不到该链接库一般都是路径不对。

Qt 默认在 /usr/lib/ 目录下查找动态链接库，但是很多 Linux 发行版将 OpenGL 链接库放在其它目录，例如我使用的是 CentOS 7，OpenGL 链接库位于 /usr/lib64/ 目录，而对于 Ubuntu，OpenGL 链接库位于 /usr/lib/i386-linux-gnu/mesa/ 目录。只要我们把 libGL.so 拷贝到 /usr/lib/ 目录，或者在 /usr/lib/ 目录下为 libGL.so 创建一个链接，就能解决问题。显然第二种办法更好。

另外，Linux 发行版自带的 OpenGL 链接库在后缀中添加了版本号，例如 libGL.so.1、libGL.so.1.2.0、libGL.so.1.3.1 等，但是 Qt 在链接阶段查找的 OpenGL 链接库是不带版本号的。

总起来说，我们需要在 /usr/lib/ 目录下为 OpenGL 链接库创建一个链接，并去掉版本号。

如果你不知道当前 Linux 系统中 libGL.so 的具体路径，可以使用locate libGL命令或find /usr -name libGL*命令查找，然后使用ln -s创建链接。请看下面的演示：

	#查找 libGL 所在位置
	[root@localhost ~]# locate libGL
	/usr/lib64/libGL.so
	/usr/lib64/libGL.so.1
	/usr/lib64/libGL.so.1.2.0
	/usr/share/doc/mesa-libGL-9.2.5
	/usr/share/doc/mesa-libGL-9.2.5/COPYING

	#创建链接
	[root@localhost ~]# ln -s /usr/lib64/libGL.so.1 /usr/lib/libGL.so
Linux 系统中可能存在多个版本的 libGL.so，为任意一个版本创建链接即可。普通用户没有权限创建链接，所以我使用了 root 用户。

完成以上操作，再次启动 Qt，然后编译或者运行，就不会出现“cannot find -lGL”错误了。
