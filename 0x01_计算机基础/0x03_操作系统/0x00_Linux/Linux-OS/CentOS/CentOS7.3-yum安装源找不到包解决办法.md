Linux CentOS yum
<a name="lOqny"></a>
### 报错如下
```bash
[root@iZuligp6e1dyzfZ ~]# yum -y install devtoolset-9-gcc devtoolset-9-gcc-c++ devtoolset-9-binutils
Loaded plugins: fastestmirror
Loading mirror speeds from cached hostfile
No package devtoolset-9-gcc available.
No package devtoolset-9-gcc-c++ available.
No package devtoolset-9-binutils available.
Error: Nothing to do
[root@iZuligp6e1dyzfZ ~]#
```
<a name="zAuLg"></a>
### 解决步骤
```bash
[root@iz2zebvvjcih5wazteoepuz ~]# yum -y install devtoolset-9-gcc devtoolset-9-gcc-c++ devtoolset-9-binutils
[root@iz2zebvvjcih5wazteoepuz ~]# yum repolist all
[root@iz2zebvvjcih5wazteoepuz ~]# wget http://people.centos.org/tru/devtools-2/devtools-2.repo -O /etc/yum.repos.d/devtools-2.repo
[root@iz2zebvvjcih5wazteoepuz ~]# cd /etc/yum.repos.d/
[root@iz2zebvvjcih5wazteoepuz yum.repos.d]# rm -rf devtools-2.repo
[root@iz2zebvvjcih5wazteoepuz yum.repos.d]# yum -y install centos-release-scl
[root@iz2zebvvjcih5wazteoepuz yum.repos.d]# yum -y install devtoolset-9-gcc devtoolset-9-gcc-c++ devtoolset-9-binutils
[root@iz2zebvvjcih5wazteoepuz yum.repos.d]# rpm -qa|grep devtoolset
devtoolset-9-gcc-9.3.1-2.el7.x86_64
devtoolset-9-runtime-9.1-0.el7.x86_64
devtoolset-9-gcc-c++-9.3.1-2.el7.x86_64
devtoolset-9-libstdc++-devel-9.3.1-2.el7.x86_64
devtoolset-9-binutils-2.32-16.el7.x86_64
[root@iz2zebvvjcih5wazteoepuz yum.repos.d]# scl enable devtoolset-9 bash
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1607997139006-83c57d1a-4f7e-4c67-8d45-f74fc0ce15ce.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2265525&status=done&style=none&width=1107.6666666666667)
<a name="p5kQV"></a>
### 一键式解决脚本
```bash
wget http://people.centos.org/tru/devtools-2/devtools-2.repo -O /etc/yum.repos.d/devtools-2.repo
cd /etc/yum.repos.d/
rm -rf devtools-2.repo
yum -y install centos-release-scl
yum -y install devtoolset-9-gcc devtoolset-9-gcc-c++ devtoolset-9-binutils
rpm -qa|grep devtoolset
scl enable devtoolset-9 bash
gcc -v
```
