Linux CentOS GDB
<a name="O7i2T"></a>
## 1、gdb源码下载地址
gdb源码下载地址：[http://ftp.gnu.org/gnu/gdb/](http://ftp.gnu.org/gnu/gdb/)
<a name="t5r29"></a>
## 2、安装相关的依赖
```bash
yum install ncurses-devel texinfo libncurses5-dev
```
<a name="uruTf"></a>
## 3、解压源码包并配置安装
```bash
wget http://ftp.gnu.org/gnu/gdb/gdb-10.2.tar.gz
tar xzf gdb-10.2.tar.gz
cd gdb-11.1
./configure
make && make install
```
<a name="pKSHh"></a>
## 4、检查是否安装成功
```bash
which gdb
/usr/local/bin/gdb -v
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1639036123788-2d586eb3-c9be-4cf2-8ec9-22371e1908ce.png#averageHue=%232f2f27&clientId=ua444162e-b6f1-4&from=paste&height=546&id=uf67eb43a&originHeight=1637&originWidth=2179&originalType=binary&ratio=1&rotation=0&showTitle=false&size=293860&status=done&style=none&taskId=u28975165-a7b9-496a-b022-84fef573fd1&title=&width=726.3333333333334)
<a name="qzAvy"></a>
## 一键式安装脚本
```bash
yum install ncurses-devel texinfo libncurses5-dev
wget http://ftp.gnu.org/gnu/gdb/gdb-10.2.tar.gz
tar xzf gdb-10.2.tar.gz
cd gdb-10.2
./configure
make && make install
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1639023909281-7ca0b413-dbd1-4f3c-aee0-430362e75f61.png#averageHue=%23353331&clientId=u4859ca0c-1ef0-4&from=paste&height=105&id=u27c69331&originHeight=316&originWidth=1910&originalType=binary&ratio=1&rotation=0&showTitle=false&size=73325&status=done&style=none&taskId=u5547874f-ec5d-4353-a2c8-5891039a6b1&title=&width=636.6666666666666)
