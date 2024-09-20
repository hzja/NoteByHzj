Linux JDK
<a name="ny7Oe"></a>
## 1、Linux版本jdk安装包下载
<a name="SYnq3"></a>
### A.Oracle-JDK官网下载发布页面
[Oracle-JDK官网下载发布页](https://www.oracle.com/cn/java/technologies/javase/javase-jdk8-downloads.html)<br />[https://www.oracle.com/cn/java/technologies/javase/javase-jdk8-downloads.html](https://www.oracle.com/cn/java/technologies/javase/javase-jdk8-downloads.html)
<a name="Ebk2p"></a>
### B.Linux版本jdk安装包（直接下载）
<a name="ej2ih"></a>
#### jdk-8u161-linux-x64
[jdk-8u161-linux-x64.tar.gz](https://www.yuque.com/attachments/yuque/0/2020/gz/396745/1595936523439-aa3a1652-b278-4bbf-83f3-7863b6db3100.gz?_lake_card=%7B%22src%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2020%2Fgz%2F396745%2F1595936523439-aa3a1652-b278-4bbf-83f3-7863b6db3100.gz%22%2C%22name%22%3A%22jdk-8u161-linux-x64.tar.gz%22%2C%22size%22%3A189756259%2C%22type%22%3A%22application%2Fgzip%22%2C%22ext%22%3A%22gzip%22%2C%22status%22%3A%22done%22%2C%22uid%22%3A%22rc-upload-1567129082210-12%22%2C%22progress%22%3A%7B%22percent%22%3A0%7D%2C%22percent%22%3A0%2C%22refSrc%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2019%2Fgz%2F396745%2F1567132448457-651202ec-8c20-4eb0-a2b5-5209764bea86.gz%22%2C%22id%22%3A%22Eq6n9%22%2C%22card%22%3A%22file%22%7D)<br />下载后使用ftp工具上传到服务器的目录
<a name="DqMnR"></a>
### C.通过wget下载
```bash
$ wget https://repo.huaweicloud.com/java/jdk/8u201-b09/jdk-8u201-linux-x64.tar.gz
```
<a name="g9PHi"></a>
## 2、解压jdk安装包
```bash
[root@iZuligp6e1dyzfZ jdk]# tar -zxvf jdk-8u161-linux-x64.tar.gz
```
<a name="kV8B1"></a>
## 3、配置环境变量
<a name="aZska"></a>
### ①进入解压目录获取当前路径
```bash
[root@iZuligp6e1dyzfZ jdk]# ll
total 185316
drwxr-xr-x 8   10  143      4096 Dec 20  2017 jdk1.8.0_161
-rw-r--r-- 1 root root 189756259 Jul 27 01:20 jdk-8u161-linux-x64.tar.gz
[root@iZuligp6e1dyzfZ jdk]# cd jdk1.8.0_161/
[root@iZuligp6e1dyzfZ jdk1.8.0_161]# pwd
/local/jdk/jdk1.8.0_161
[root@iZuligp6e1dyzfZ jdk1.8.0_161]#
```
<a name="B5q67"></a>
### ②修改/etc/profile 文件添加Java环境变量
编辑配置文件
```bash
[root@iZuligp6e1dyzfZ jdk1.8.0_161]# vim /etc/profile
```
添加的内容
```bash
export JAVA_HOME=/local/jdk/jdk1.8.0_161
export PATH=$JAVA_HOME/bin:$PATH
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595784829853-0dcb228e-c2a9-451b-9713-004e3ccb5183.png#height=583&id=gwsKQ&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&size=2066715&status=done&style=none&width=1107.6666666666667)
<a name="ebu7Y"></a>
## 4、刷新配置使其生效
```bash
[root@iZuligp6e1dyzfZ jdk1.8.0_161]# source /etc/profile
```
<a name="QGMoQ"></a>
## 5、通过 `java -version` 命令检查是否完成安装配置，成功输出版本信息则安装成功
```bash
[root@iZuligp6e1dyzfZ jdk1.8.0_161]# java -version
java version "1.8.0_161"
Java(TM) SE Runtime Environment (build 1.8.0_161-b12)
Java HotSpot(TM) 64-Bit Server VM (build 25.161-b12, mixed mode)
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595785002465-91e00d1a-857c-4a34-b8ff-88c87653516f.png#height=127&id=kezqH&originHeight=381&originWidth=3323&originalType=binary&ratio=1&size=408764&status=done&style=none&width=1107.6666666666667)
