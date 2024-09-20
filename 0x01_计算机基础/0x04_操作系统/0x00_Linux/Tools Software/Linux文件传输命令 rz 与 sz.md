Linux rz sz<br />将本地的文件上传到Linux服务器或者从服务器Linux上下载文件到本地，rz / sz命令很方便的实现了这个功能，但是很多Linux系统初始并没有这两个命令。
<a name="FKVXD"></a>
### 1、源码安装
使用 root 用户登陆后，依次执行以下命令：
```bash
cd /tmp
wget https://www.ohse.de/uwe/releases/lrzsz-0.12.20.tar.gz
tar zxvf lrzsz-0.12.20.tar.gz
cd lrzsz-0.12.20
./configure --prefix=/usr/local/lrzsz
make 
make install
```
上面安装过程默认把lsz和lrz安装到了/usr/local/bin/目录下，这样的话并不能直接使用rz / sz 命令，下面创建软链接，并命名为 rz / sz<br />
```bash
cd /usr/bin
ln -s /usr/local/bin/lrz rz
ln -s /usr/local/bin/lsz sz
```
<a name="d6f4c679"></a>
### 2、yum安装
使用 root 用户登陆后，执行以下命令：
```bash
yum install -y lrzsz
```
CentOS使用 yum 命令安装rz / sz 最快捷方便。其他Linux操作系统安装响应的二进制包即可。例如Red Hat Linux安装lrzsz-0.12.20-36.el7.x86_64.rpm包<br />
<a name="HZ6Xi"></a>
### 3、使用说明
<a name="W0v3x"></a>
#### sz 命令发送文件到本地
```bash
# sz filename
```
<a name="t6ENs"></a>
#### rz 命令上传本地文件到服务器
```
# rz
```
执行该命令后，在弹出框中选择要上传的文件即可。另外，打开SecureCRT软件 -> Options -> session options -> X/Y/Zmodem 下可以设置上传和下载的目录。
:::danger
注意：使用的图形化界面最好是SecureCRT和XShell，PUTTY并这些开源简单的ssh工具并不能使用rz 和 sz命令，因为弹不出对应的对话框
:::
