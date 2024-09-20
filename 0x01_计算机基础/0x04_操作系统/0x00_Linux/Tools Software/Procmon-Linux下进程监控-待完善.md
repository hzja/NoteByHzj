Linux Procmon 
<a name="4JpRt"></a>
# 1、下载安装Procmon编译必要的依赖
```bash
[root@iZuligp6e1dyzfZ /]# yum -y install bison build-essential flex git libedit-dev \ libllvm6.0 llvm-6.0-dev libclang-6.0-dev python zlib1g-dev libelf-dev
```
安装编译bcc
```bash
$ git clone --branch tag_v0.10.0 https://github.com/iovisor/bcc.git
$ mkdir bcc/build
$ cd bcc/build
$ cmake .. -DCMAKE_INSTALL_PREFIX=/usr
$ make
$ sudo make install
```
