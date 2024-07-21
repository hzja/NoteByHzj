CentOS make
<a name="I4bPR"></a>
### 1.下载
```bash
wget http://ftp.gnu.org/gnu/make/make-4.2.tar.gz
```
<a name="zHl05"></a>
### 2.解压
```bash
tar zxf make-4.2.tar.gz
```
<a name="E5JJA"></a>
### 3.编译安装
```bash
cd make-4.2
```
<a name="3qVs6"></a>
### 4.建立编译目录
```bash
mkdir build
cd build
```
<a name="03d0T"></a>
### 5.执行编译
```bash
../configure --prefix=/usr
```
执行完，这里需要编译，会生成build.sh文件，然后执行这个文件
```bash
sh build.sh
```
<a name="C1JJ2"></a>
### 6.安装
```bash
make install
```
<a name="9217bfaa"></a>
### 7.安装完成-查看版本
```bash
make -v
```

