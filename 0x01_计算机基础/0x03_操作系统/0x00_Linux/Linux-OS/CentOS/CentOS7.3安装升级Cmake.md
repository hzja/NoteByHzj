Linux CentOS Cmake
<a name="c5feL"></a>
## 1、检查当前的cmake的版本
```bash
[root@iZuligp6e1dyzfZ tools-software]# cmake --version
cmake version 2.8.12.2
```
<a name="vyLY6"></a>
## 2、下载可执行文件进行配置
[cmake-3.25.0-linux-x86_64.tar.gz.zip](https://www.yuque.com/attachments/yuque/0/2022/zip/396745/1669169744833-aa1cc57b-861e-4ebf-8ad0-58549ba12c24.zip?_lake_card=%7B%22src%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2022%2Fzip%2F396745%2F1669169744833-aa1cc57b-861e-4ebf-8ad0-58549ba12c24.zip%22%2C%22name%22%3A%22cmake-3.25.0-linux-x86_64.tar.gz.zip%22%2C%22size%22%3A47904941%2C%22type%22%3A%22application%2Fx-zip-compressed%22%2C%22ext%22%3A%22zip%22%2C%22source%22%3A%22%22%2C%22status%22%3A%22done%22%2C%22mode%22%3A%22title%22%2C%22download%22%3Atrue%2C%22taskId%22%3A%22u5baef94e-75fe-48e4-b8d2-e964c26ac61%22%2C%22taskType%22%3A%22upload%22%2C%22__spacing%22%3A%22both%22%2C%22id%22%3A%22afbUe%22%2C%22margin%22%3A%7B%22top%22%3Atrue%2C%22bottom%22%3Atrue%7D%2C%22card%22%3A%22file%22%7D)下载后重命名将.zip后缀去掉<br />[https://cmake.org/download/](https://cmake.org/download/)<br />在二进制包处选择对应系统的可执行包<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669170430042-62d4bf9e-4377-46ac-8e05-229fff2363c4.png#averageHue=%23f9f9f9&clientId=uc6d5acd5-51b9-4&from=paste&height=713&id=u9ff15d3e&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=309492&status=done&style=none&taskId=u06d797e5-3846-4815-9a3e-6a7af036510&title=&width=1536)
<a name="k8Tni"></a>
## 3、从Cmake源码进行编译安装
[cmake官网](https://cmake.org/)<br />[https://cmake.org/download/](https://cmake.org/download/)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596613232637-01dfc3e5-e71d-4d17-9a4f-29c45c848847.png#averageHue=%23f8f7f7&height=687&id=MELBe&originHeight=2060&originWidth=3798&originalType=binary&ratio=1&rotation=0&showTitle=false&size=742056&status=done&style=shadow&title=&width=1266)
<a name="NNpe2"></a>
### A、使用wget下载源码
```bash
[root@iZuligp6e1dyzfZ tools-software]# wget https://github.com/Kitware/CMake/releases/download/v3.18.1/cmake-3.18.1.tar.gz
```
**国内访问比较慢时，将安装包上传至服务器**<br />[cmake-3.22.1.tar.gz.zip](https://www.yuque.com/attachments/yuque/0/2021/zip/396745/1639041464843-cb739d38-76d1-4913-be0b-98e683fece36.zip?_lake_card=%7B%22src%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2021%2Fzip%2F396745%2F1639041464843-cb739d38-76d1-4913-be0b-98e683fece36.zip%22%2C%22name%22%3A%22cmake-3.22.1.tar.gz.zip%22%2C%22size%22%3A9778031%2C%22type%22%3A%22application%2Fx-zip-compressed%22%2C%22ext%22%3A%22zip%22%2C%22source%22%3A%22%22%2C%22status%22%3A%22done%22%2C%22mode%22%3A%22title%22%2C%22download%22%3Atrue%2C%22taskId%22%3A%22uff0226a3-ce8d-419b-a33f-702895940de%22%2C%22taskType%22%3A%22upload%22%2C%22id%22%3A%22z0oIZ%22%2C%22card%22%3A%22file%22%7D)下载后重命名将.zip后缀去掉<br />[cmake-3.18.1.tar.gz](https://www.yuque.com/attachments/yuque/0/2020/gz/396745/1596615265016-b070f74e-a93d-4a50-b880-2fcb9b317a8c.gz?_lake_card=%7B%22src%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2020%2Fgz%2F396745%2F1596615265016-b070f74e-a93d-4a50-b880-2fcb9b317a8c.gz%22%2C%22name%22%3A%22cmake-3.18.1.tar.gz%22%2C%22size%22%3A8969366%2C%22type%22%3A%22application%2Fx-gzip%22%2C%22ext%22%3A%22gz%22%2C%22source%22%3A%22%22%2C%22status%22%3A%22done%22%2C%22mode%22%3A%22title%22%2C%22download%22%3Atrue%2C%22uid%22%3A%221596615261497-0%22%2C%22progress%22%3A%7B%22percent%22%3A99%7D%2C%22percent%22%3A0%2C%22id%22%3A%22IoV9d%22%2C%22card%22%3A%22file%22%7D)

<a name="pINmc"></a>
### B、解压源码安装包进行安装
```bash
[root@iZuligp6e1dyzfZ tools-software]# tar -zxvf cmake-3.18.1.tar.gz
[root@iZuligp6e1dyzfZ tools-software]# cd cmake-3.18.1
[root@iZuligp6e1dyzfZ cmake-3.18.1]# ./configure
```
<a name="NtXsp"></a>
#### ①报错-缺少SSL
```bash
-- Could NOT find OpenSSL, try to set the path to OpenSSL root folder in the system variable OPENSSL_ROOT_DIR (missing: OPENSSL_CRYPTO_LIBRARY OPENSSL_INCLUDE_DIR)
CMake Error at Utilities/cmcurl/CMakeLists.txt:485 (message):
  Could not find OpenSSL.  Install an OpenSSL development package or
  configure CMake with -DCMAKE_USE_OPENSSL=OFF to build without OpenSSL.


-- Configuring incomplete, errors occurred!
See also "/local/tools-software/cmake-3.18.1/CMakeFiles/CMakeOutput.log".
See also "/local/tools-software/cmake-3.18.1/CMakeFiles/CMakeError.log".
---------------------------------------------
Error when bootstrapping CMake:
Problem while running initial CMake
---------------------------------------------
```
<a name="mIfxI"></a>
##### 解决办法
```bash
[root@iZuligp6e1dyzfZ cmake-3.18.1]# rm -rf CMakeCache.txt
[root@iZuligp6e1dyzfZ cmake-3.18.1]# yum -y install ncurses-devel
[root@iZuligp6e1dyzfZ cmake-3.18.1]# yum install openssl-devel
```
<a name="Xv8Tj"></a>
#### ②报错-C++编译版本太低
```bash
[root@iZuf68qk0bio1l786kylamZ cmake-3.18.1]# ./configure 
---------------------------------------------
CMake 3.18.1, Copyright 2000-2020 Kitware, Inc. and Contributors
C compiler on this system is: cc   
---------------------------------------------
Error when bootstrapping CMake:
Cannot find a C++ compiler that supports both C++11 and the specified C++ flags.
Please specify one using environment variable CXX.
The C++ flags are "".
They can be changed using the environment variable CXXFLAGS.
See cmake_bootstrap.log for compilers attempted.
---------------------------------------------
Log of errors: /root/cmake-3.18.1/Bootstrap.cmk/cmake_bootstrap.log
---------------------------------------------
[root@iZuf68qk0bio1l786kylamZ cmake-3.18.1]# gcc --version
gcc (GCC) 4.8.5 20150623 (Red Hat 4.8.5-36)
Copyright (C) 2015 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1638972573317-c2573ca1-1dbf-43b3-b0e3-3bccca90620b.png#averageHue=%23313129&clientId=ue6268d5f-ec7c-4&from=paste&height=459&id=u27816191&originHeight=917&originWidth=1988&originalType=binary&ratio=1&rotation=0&showTitle=false&size=188972&status=done&style=none&taskId=u72914953-276b-4f1a-856f-7483908d9fa&title=&width=994)
<a name="YEEbY"></a>
##### 解决办法
升级g++，参考：<br />[CentOS7.3安装升级gcc](https://www.yuque.com/fcant/linux/vv50z7?view=doc_embed)
<a name="OGMqv"></a>
#### ③再次执行./configure
```bash
[root@iZuligp6e1dyzfZ cmake-3.18.1]# ./configure
```
配置完成输出
```bash
-- Performing Test run_pic_test - Success
-- Performing Test run_inlines_hidden_test
-- Performing Test run_inlines_hidden_test - Success
-- Configuring done
-- Generating done
-- Build files have been written to: /local/tools-software/cmake-3.18.1
---------------------------------------------
CMake has bootstrapped.  Now run gmake.
```
<a name="GiPuz"></a>
### C、安装
```bash
[root@iZuligp6e1dyzfZ cmake-3.18.1]# make && make install
```
<a name="JPmIP"></a>
### D、执行 hash -r
```bash
[root@iZuligp6e1dyzfZ cmake-3.18.1]# hash -r
```
<a name="SdPn0"></a>
### E、执行 `cmake --version` 查看安装的版本
```bash
[root@iZuligp6e1dyzfZ cmake-3.18.1]# cmake --version
cmake version 3.18.1

CMake suite maintained and supported by Kitware (kitware.com/cmake).
```
<a name="PJuPJ"></a>
### F、一键式源码包安装脚本
```bash
yum -y install ncurses-devel
yum install openssl-devel
tar -zxvf cmake-3.22.1.tar.gz
cd cmake-3.22.1
./configure
make && make install
hash -r
cmake --version

```
