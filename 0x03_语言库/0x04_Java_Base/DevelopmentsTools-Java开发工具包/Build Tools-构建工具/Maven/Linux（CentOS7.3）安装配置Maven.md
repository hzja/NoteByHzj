Linux Maven
<a name="kRioS"></a>
## 1、Maven安装包下载
<a name="AO9QI"></a>
### 官网地址
[Maven官网下载地址](http://maven.apache.org/download.cgi)<br />[http://maven.apache.org/download.cgi](http://maven.apache.org/download.cgi)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595813329635-6d006c4b-2c9a-4e07-91d6-a387aded1992.png#averageHue=%23f8f7f7&height=601&id=mRlp5&originHeight=1804&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=654886&status=done&style=shadow&title=&width=1280)
<a name="tTUK8"></a>
### 复制要下载的连接，在Linux使用wget下载
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595813494936-61487eb0-3684-4280-8952-0f49fbe512af.png#averageHue=%23f8f8f7&height=601&id=VaDu4&originHeight=1804&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=834501&status=done&style=shadow&title=&width=1280)
```bash
[root@iZuligp6e1dyzfZ maven]# wget https://mirrors.bfsu.edu.cn/apache/maven/maven-3/3.6.3/binaries/apache-maven-3.6.3-bin.tar.gz
--2020-07-27 09:31:58--  https://mirrors.bfsu.edu.cn/apache/maven/maven-3/3.6.3/binaries/apache-maven-3.6.3-bin.tar.gz
Resolving mirrors.bfsu.edu.cn (mirrors.bfsu.edu.cn)... 39.155.141.16, 2001:da8:20f:4435:4adf:37ff:fe55:2840
Connecting to mirrors.bfsu.edu.cn (mirrors.bfsu.edu.cn)|39.155.141.16|:443... connected.
HTTP request sent, awaiting response... 200 OK
Length: 9506321 (9.1M) [application/octet-stream]
Saving to: ‘apache-maven-3.6.3-bin.tar.gz’

100%[==============================================================================>] 9,506,321   18.3MB/s   in 0.5s

2020-07-27 09:31:59 (18.3 MB/s) - ‘apache-maven-3.6.3-bin.tar.gz’ saved [9506321/9506321]

[root@iZuligp6e1dyzfZ maven]# ll
total 9284
-rw-r--r-- 1 root root 9506321 Nov 20  2019 apache-maven-3.6.3-bin.tar.gz
```
<a name="Zt82O"></a>
## 2、解压安装包
```bash
tar -zxvf apache-maven-3.6.3-bin.tar.gz
```
<a name="DQ4Et"></a>
## 3、配置Maven的conf目录下的setting.xml文件
<a name="um7iB"></a>
### 创建本地仓库目录
```bash
$ mkdir Repo
$ cd Repo/
$ pwd
/local/maven/Repo
```
<a name="ig0Hy"></a>
### 将本地仓库目录配置至Maven解压目录下的conf目录里的settings.xml文件中
```bash
[root@iZuligp6e1dyzfZ apache-maven-3.6.3]# cd conf/
[root@iZuligp6e1dyzfZ conf]# vim settings.xml
```
添加如下内容
```xml
<localRepository>/local/maven/Repo</localRepository>
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595814130866-17138d7d-21ac-4d32-a240-73e0d6dc88c4.png#averageHue=%23383837&height=583&id=SgXeo&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2147672&status=done&style=none&title=&width=1107.6666666666667)
<a name="N37cU"></a>
### 配置私服仓库地址
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595814458901-5c2d6fda-6d32-4084-a40c-c508f99c38ee.png#averageHue=%233a3a39&height=583&id=FHLng&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2160189&status=done&style=none&title=&width=1107.6666666666667)<br />找到mirrors节点，添加如下内容
```xml
<!-- 阿里云仓库 -->
<mirror>
  <id>alimaven</id>
  <mirrorOf>central</mirrorOf>
  <name>aliyun maven</name>
  <url>http://maven.aliyun.com/nexus/content/repositories/central/</url>
</mirror>
<mirror>
  <id>alimaven</id>
  <name>aliyun maven</name>
  <url>http://maven.aliyun.com/nexus/content/groups/public/</url>
  <mirrorOf>central</mirrorOf>
</mirror>
<!-- 中央仓库 -->
<mirror>
  <id>repo</id>
  <mirrorOf>central</mirrorOf>
  <name>Human Readable Name for this Mirror.</name>
  <url>http://repo1.maven.org/maven2/</url>
</mirror>
<mirror>
  <!-- 官方源1-->
  <id>central</id>
  <name>Maven Repository Switchboard</name>
  <url>http://repo1.maven.org/maven2/</url>
  <mirrorOf>central</mirrorOf>
</mirror>
<mirror>
  <!-- 官方源2-->
  <id>repo2</id>
  <mirrorOf>central</mirrorOf>
  <name>Human Readable Name for this Mirror.</name>
  <url>http://repo2.maven.org/maven2/</url>
</mirror>
<mirror>
  <!-- ibiblio镜像源-->
  <id>ibiblio</id>
  <mirrorOf>central</mirrorOf>
  <name>Human Readable Name for this Mirror.</name>
  <url>http://mirrors.ibiblio.org/pub/mirrors/maven2/</url>
</mirror>
<mirror>
  <!-- jboss镜像源-->
  <id>jboss-public-repository-group</id>
  <mirrorOf>central</mirrorOf>
  <name>JBoss Public Repository Group</name>
  <url>http://repository.jboss.org/nexus/content/groups/public</url>
</mirror>
<!-- 中央仓库在中国的镜像 -->
<mirror>
  <id>maven.net.cn</id>
  <name>oneof the central mirrors in china</name>
  <url>http://maven.net.cn/content/groups/public/</url>
  <mirrorOf>central</mirrorOf>
</mirror>
```
<a name="SdRS5"></a>
## 4、将Maven的解压目录配置至系统环境变量
```bash
[root@iZuligp6e1dyzfZ apache-maven-3.6.3]# pwd
/local/maven/apache-maven-3.6.3
[root@iZuligp6e1dyzfZ apache-maven-3.6.3]# vim /etc/profile
```
添加如下配置
```bash
# Maven环境变量配置
export MAVEN_HOME=/local/maven/apache-maven-3.6.3
export PATH=$MAVEN_HOME/bin:$PATH
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595814793538-6750a1a2-7cdd-47a7-9096-089127f9461a.png#averageHue=%23393837&height=583&id=QbW4Y&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2046869&status=done&style=none&title=&width=1107.6666666666667)
<a name="gdITv"></a>
## 5、刷新配置使其生效
```bash
[root@iZuligp6e1dyzfZ apache-maven-3.6.3]# source /etc/profile
```
<a name="wUnTF"></a>
## 6、使用 `mvn -v` 命令检查安装完成
```bash
[root@iZuligp6e1dyzfZ apache-maven-3.6.3]# mvn -v
Apache Maven 3.6.3 (cecedd343002696d0abb50b32b541b8a6ba2883f)
Maven home: /local/maven/apache-maven-3.6.3
Java version: 1.8.0_161, vendor: Oracle Corporation, runtime: /local/jdk/jdk1.8.0_161/jre
Default locale: en_US, platform encoding: UTF-8
OS name: "linux", version: "3.10.0-514.26.2.el7.x86_64", arch: "amd64", family: "unix"
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595815071333-f1453ea1-5315-4407-a58a-4cf4793e7ea8.png#averageHue=%23565656&height=163&id=BUd5z&originHeight=490&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=571924&status=done&style=none&title=&width=1107.6666666666667)
