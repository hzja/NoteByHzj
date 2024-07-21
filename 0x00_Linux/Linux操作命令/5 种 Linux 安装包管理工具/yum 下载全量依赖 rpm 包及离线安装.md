<a name="kixgA"></a>
### 1、简介
通常生产环境由于安全原因都无法访问互联网。此时就需要进行离线安装，主要有两种方式：源码编译、rpm包安装。源码编译耗费时间长且缺乏编译环境，所以一般都选择使用离线 rpm 包安装。
<a name="ttKjN"></a>
### 2、验证环境
Centos 7.8
<a name="ISAob"></a>
### 3、查看依赖包
可以使用“yum deplist”命令来查找 rpm 包的依赖列表。例如，要查找“ansible”rpm的依赖包：
```bash
$ yum deplist ansible
软件包：ansible.noarch 2.9.3-1.el7
   依赖：/usr/bin/env
   provider: coreutils.x86_64 8.22-24.el7
   依赖：/usr/bin/python2
   provider: python.x86_64 2.7.5-86.el7
   依赖：PyYAML
   provider: PyYAML.x86_64 3.10-11.el7
   依赖：python(abi) = 2.7
   provider: python.x86_64 2.7.5-86.el7
   依赖：python-httplib2
   provider: python-httplib2.noarch 0.9.2-1.el7
   依赖：python-jinja2
   provider: python-jinja2.noarch 2.7.2-4.el7
   依赖：python-paramiko
   provider: python-paramiko.noarch 2.1.1-9.el7
   依赖：python-setuptools
   provider: python-setuptools.noarch 0.9.8-7.el7
   依赖：python-six
   provider: python-six.noarch 1.9.0-2.el7
   依赖：python2-cryptography
   provider: python2-cryptography.x86_64 1.7.2-2.el7
   依赖：python2-jmespath
   provider: python2-jmespath.noarch 0.9.0-3.el7
   依赖：sshpass
   provider: sshpass.x86_64 1.06-2.el7

```
<a name="n3C7Y"></a>
### 4、方案一（推荐）：repotrack
```bash
# 安装yum-utils
$ yum -y install yum-utils

# 下载 ansible 全量依赖包
$ repotrack ansible
```
<a name="VVmyV"></a>
### 5、方案二：yumdownloader
```bash
# 安装yum-utils
$ yum -y install yum-utils

# 下载 ansible 依赖包
$ yumdownloader --resolve --destdir=/tmp ansible
```
参数说明：

- --destdir：指定 rpm 包下载目录（不指定时，默认为当前目录）
- --resolve：下载依赖的 rpm 包。
> **注意：仅会将主软件包和基于现在的操作系统所缺少的依赖关系包一并下载。**

<a name="zmjUz"></a>
### 6、方案三：yum 的 downloadonly 插件
```bash
# 安装插件
$ yum -y install yum-download

# 下载 ansible 依赖包
$ yum -y install ansible --downloadonly --downloaddir=/tmp
```
> **注意：与 yumdownloader 命令一样，也是仅会将主软件包和基于现在的操作系统所缺少的依赖关系包一并下载。**

<a name="gcAMx"></a>
### 7、方案四：离线安装 rpm
自己下载相关包然后使用rpm命令离线安装 
```bash
$ rpm -Uvh --force --nodeps *.rpm
```
