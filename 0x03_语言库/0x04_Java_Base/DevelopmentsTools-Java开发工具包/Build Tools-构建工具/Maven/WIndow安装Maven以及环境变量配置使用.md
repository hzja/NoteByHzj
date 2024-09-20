Window Maven
<a name="pvLGx"></a>
## 1、Maven的安装包
<a name="FvLmM"></a>
### A、官网下载地址
[Maven官网地址](http://maven.apache.org/download.cgi)<br />[http://maven.apache.org/download.cgi](http://maven.apache.org/download.cgi)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595813162801-ec0e4437-452d-4ace-a6cc-26e399908d54.png#averageHue=%23f8f7f7&height=601&id=fLGTs&originHeight=1804&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=653624&status=done&style=shadow&title=&width=1280)
<a name="bpLU4"></a>
### B、安装包下载
[apache-maven-3.6.1-bin.zip](https://www.yuque.com/attachments/yuque/0/2020/zip/396745/1595936519616-6e21a3b2-4c44-4a58-b3c1-0a83fee32d0b.zip?_lake_card=%7B%22src%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2020%2Fzip%2F396745%2F1595936519616-6e21a3b2-4c44-4a58-b3c1-0a83fee32d0b.zip%22%2C%22name%22%3A%22apache-maven-3.6.1-bin.zip%22%2C%22size%22%3A9181276%2C%22type%22%3A%22application%2Fzip%22%2C%22ext%22%3A%22zip%22%2C%22source%22%3A%22%22%2C%22status%22%3A%22done%22%2C%22mode%22%3A%22title%22%2C%22download%22%3Atrue%2C%22uid%22%3A%22rc-upload-1566962782153-7%22%2C%22progress%22%3A%7B%22percent%22%3A0%7D%2C%22percent%22%3A0%2C%22id%22%3A%227417z%22%2C%22card%22%3A%22file%22%7D)
<a name="eaBv0"></a>
## 2、settings.xml文件配置
<a name="VK9vr"></a>
### A、使用配置好的settings.xml文件
<a name="w2RZL"></a>
#### ①下载下面的settings.xml文件并将其保存至个人配置目录
[settings.xml](https://www.yuque.com/attachments/yuque/0/2020/xml/396745/1595936519743-1f092e38-e37b-4027-943b-5a1336a90fb6.xml?_lake_card=%7B%22src%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2020%2Fxml%2F396745%2F1595936519743-1f092e38-e37b-4027-943b-5a1336a90fb6.xml%22%2C%22name%22%3A%22settings.xml%22%2C%22size%22%3A13457%2C%22type%22%3A%22text%2Fxml%22%2C%22ext%22%3A%22xml%22%2C%22source%22%3A%22%22%2C%22status%22%3A%22done%22%2C%22mode%22%3A%22title%22%2C%22download%22%3Atrue%2C%22uid%22%3A%22rc-upload-1566962782153-12%22%2C%22progress%22%3A%7B%22percent%22%3A0%7D%2C%22percent%22%3A0%2C%22id%22%3A%22VH84w%22%2C%22card%22%3A%22file%22%7D)
<a name="AwFwK"></a>
#### ②修改自己的本地仓库地址
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1566966713474-92e6ca37-211b-4052-a14b-1aa6ef2a0116.png#averageHue=%23f3ebd8&height=413&id=RJzgX&originHeight=1136&originWidth=3261&originalType=binary&ratio=1&rotation=0&showTitle=false&size=266421&status=done&style=none&title=&width=1185.8181818181818)
<a name="NrewV"></a>
### B、从安装包中conf目录下的settings.xml配置
<a name="nKXUZ"></a>
#### ①配置私服仓库
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595812369539-d608626a-fc78-4d32-90a7-9993e3b69d5d.png#averageHue=%23fcfcfc&height=582&id=oPMyr&originHeight=1746&originWidth=3789&originalType=binary&ratio=1&rotation=0&showTitle=false&size=423220&status=done&style=shadow&title=&width=1263)
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
<a name="guEa7"></a>
#### ②配置本地仓库地址
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1566966713474-92e6ca37-211b-4052-a14b-1aa6ef2a0116.png#averageHue=%23f3ebd8&height=413&id=DKzOK&originHeight=1136&originWidth=3261&originalType=binary&ratio=1&rotation=0&showTitle=false&size=266421&status=done&style=none&title=&width=1185.8181818181818)
<a name="rp1VY"></a>
## 4、配置系统的Maven环境变量
<a name="Favam"></a>
### A、新建`MAVEN_HOME`变量
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1566967003655-60e87763-1112-4633-9f30-5d2b6a709b9d.png#averageHue=%23f5f5f4&height=750&id=vwsaA&originHeight=2063&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=800557&status=done&style=shadow&title=&width=1396.3636363636363)
<a name="mGSI8"></a>
### B、添加变量至系统Path变量
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1566967201054-99aa9785-3cb3-4a60-95bc-3798b536ba53.png#averageHue=%23f3f2f1&height=748&id=qEced&originHeight=2058&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=900844&status=done&style=shadow&title=&width=1396.3636363636363)
<a name="FuGCf"></a>
## 5、在IDEA中配置Maven
<a name="vaE5J"></a>
### A、在当前项目配置Maven
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1566967351873-004f506a-a631-4bdb-9954-ec300c6f4ed4.png#averageHue=%23eeeded&height=687&id=iFiyL&originHeight=1889&originWidth=3033&originalType=binary&ratio=1&rotation=0&showTitle=false&size=449973&status=done&style=shadow&title=&width=1102.909090909091)
<a name="R33FT"></a>
### 2、在IDEA中对新项目配置Maven
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1566967415366-3cd31f8b-f6be-4799-96b1-809b7d5e7649.png#averageHue=%23eeedec&height=539&id=j5RWj&originHeight=1482&originWidth=1526&originalType=binary&ratio=1&rotation=0&showTitle=false&size=329844&status=done&style=none&title=&width=554.9090909090909)<br />![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1566967351873-004f506a-a631-4bdb-9954-ec300c6f4ed4.png#averageHue=%23eeeded&height=687&id=mDUXY&originHeight=1889&originWidth=3033&originalType=binary&ratio=1&rotation=0&showTitle=false&size=449973&status=done&style=shadow&title=&width=1102.909090909091)
