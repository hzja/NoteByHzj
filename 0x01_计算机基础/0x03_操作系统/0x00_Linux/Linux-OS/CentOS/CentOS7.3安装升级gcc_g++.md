Linux gcc
<a name="W1kqH"></a>
## 1、通过gcc -v命令检查gcc的安装
```bash
➜  ~ gcc -v
Using built-in specs.
COLLECT_GCC=gcc
COLLECT_LTO_WRAPPER=/usr/libexec/gcc/x86_64-redhat-linux/4.8.5/lto-wrapper
Target: x86_64-redhat-linux
Configured with: ../configure --prefix=/usr --mandir=/usr/share/man --infodir=/usr/share/info --with-bugurl=http://bugzilla.redhat.com/bugzilla --enable-bootstrap --enable-shared --enable-threads=posix --enable-checking=release --with-system-zlib --enable-__cxa_atexit --disable-libunwind-exceptions --enable-gnu-unique-object --enable-linker-build-id --with-linker-hash-style=gnu --enable-languages=c,c++,objc,obj-c++,java,fortran,ada,go,lto --enable-plugin --enable-initfini-array --disable-libgcj --with-isl=/builddir/build/BUILD/gcc-4.8.5-20150702/obj-x86_64-redhat-linux/isl-install --with-cloog=/builddir/build/BUILD/gcc-4.8.5-20150702/obj-x86_64-redhat-linux/cloog-install --enable-gnu-indirect-function --with-tune=generic --with-arch_32=x86-64 --build=x86_64-redhat-linux
Thread model: posix
gcc version 4.8.5 20150623 (Red Hat 4.8.5-11) (GCC)
```
<a name="RCyW6"></a>
## 2、gcc安装升级
```bash
➜  / yum -y install devtoolset-9-gcc devtoolset-9-gcc-c++ devtoolset-9-binutils
➜  / gcc -v
➜  / scl enable devtoolset-9 bash
➜  / echo "source /opt/rh/devtoolset-9/enable" >> /etc/profile
➜  / gcc -v
Using built-in specs.
COLLECT_GCC=gcc
COLLECT_LTO_WRAPPER=/opt/rh/devtoolset-9/root/usr/libexec/gcc/x86_64-redhat-linux/9/lto-wrapper
Target: x86_64-redhat-linux
Configured with: ../configure --enable-bootstrap --enable-languages=c,c++,fortran,lto --prefix=/opt/rh/devtoolset-9/root/usr --mandir=/opt/rh/devtoolset-9/root/usr/share/man --infodir=/opt/rh/devtoolset-9/root/usr/share/info --with-bugurl=http://bugzilla.redhat.com/bugzilla --enable-shared --enable-threads=posix --enable-checking=release --enable-multilib --with-system-zlib --enable-__cxa_atexit --disable-libunwind-exceptions --enable-gnu-unique-object --enable-linker-build-id --with-gcc-major-version-only --with-linker-hash-style=gnu --with-default-libstdcxx-abi=gcc4-compatible --enable-plugin --enable-initfini-array --with-isl=/builddir/build/BUILD/gcc-9.3.1-20200408/obj-x86_64-redhat-linux/isl-install --disable-libmpx --enable-gnu-indirect-function --with-tune=generic --with-arch_32=x86-64 --build=x86_64-redhat-linux
Thread model: posix
gcc version 9.3.1 20200408 (Red Hat 9.3.1-2) (GCC)
```
<a name="07bc858f"></a>
#### 如果遇到安装gcc不能在yum源找到包的问题，可参考如下
[CentOS7.3-yum安装源找不到包解决办法](https://www.yuque.com/fcant/linux/hcmp8l?view=doc_embed)
<a name="fke0w"></a>
## 3、一键式安装升级gcc脚本
```bash
wget http://people.centos.org/tru/devtools-2/devtools-2.repo -O /etc/yum.repos.d/devtools-2.repo
cd /etc/yum.repos.d/
rm -rf devtools-2.repo
yum -y install centos-release-scl
yum -y install devtoolset-9-gcc devtoolset-9-gcc-c++ devtoolset-9-binutils
rpm -qa|grep devtoolset
scl enable devtoolset-9 bash
gcc -v
g++ -v
```
<a name="FbfML"></a>
## 安装后检查gcc版本不正确的问题修复
gcc安装时，只对当前连接的用户生效，在当前执行安装操作的用户查看gcc的安装路径，执行`which gcc`，登录新会话时gcc又显示默认的版本了，可以通过配置别名的方式进行修复
```bash
vim ~/.bashrc

#添加以下配置
alias gcc='/opt/rh/devtoolset-9/root/usr/bin/gcc'
alias g++='/opt/rh/devtoolset-9/root/usr/bin/g++'
```
```bash
source ~/.bashrc 
gcc -v
g++ -v
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1639034454434-20c19f6d-e587-4ae2-8c20-da9b75a39b54.png#clientId=u7a705fed-7af2-4&from=paste&height=488&id=udd0074f8&originHeight=1463&originWidth=2216&originalType=binary&ratio=1&rotation=0&showTitle=false&size=310278&status=done&style=none&taskId=ub14468de-564f-4abf-8d5e-deed1d9a592&title=&width=738.6666666666666)
