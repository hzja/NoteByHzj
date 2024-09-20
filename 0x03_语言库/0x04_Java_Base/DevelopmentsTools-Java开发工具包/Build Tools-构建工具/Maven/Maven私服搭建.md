Maven
<a name="FGxJY"></a>
## 私服
私服也是远程仓库中的一种，为什么需要私服呢？<br />如果一个团队中有几百个人在开发一些项目，都是采用Maven的方式来组织项目，那么每个人都需要从远程仓库中把需要依赖的构件下载到本地仓库，这对公司的网络要求也比较高，为了节省这个宽带和加快下载速度，在公司内部局域网内部可以架设一台服务器，这台服务器起到一个代理的作用，公司里面的所有开发者去访问这个服务器，这台服务器将需要的构件返回给开发者，如果这台服务器中也没有需要的构件，那么这个代理服务器会去远程仓库中查找，然后将其先下载到代理服务器中，然后再返回给开发者本地的仓库。<br />还有公司内部有很多项目之间会相互依赖，你可能是架构组的，需要开发一些jar包给其他组使用，此时，可以将自己jar发布到私服中给其他同事使用，如果没有私服，可能需要手动发给别人或者上传到共享机器中，不过管理起来不是很方便。<br />**总体上来说私服有以下好处：**

1. 加速Maven构件的下载速度
2. 节省宽带，加速项目构建速度
3. 方便部署自己的构件以供他人使用
4. 提高Maven的稳定性，中央仓库需要本机能够访问外网，而如果采用私服的方式，只需要本机可以访问内网私服就可以了

有3种专门的Maven仓库管理软件可以用来搭建私服：

1. Apache基金会的archiva

[http://archiva.apache.org/](http://archiva.apache.org/)

2. JFrog的Artifactory

[https://jfrog.com/artifactory/](https://jfrog.com/artifactory/)

3. Sonatype的Nexus

[https://my.sonatype.com/](https://my.sonatype.com/)<br />这些都是开源的私服软件，都可以自由使用。用的最多的是第三种Nexus，这里主要以这个来讲解，其他2种有兴趣的朋友可以去研究一下。
<a name="R6DVI"></a>
## Windows10中安装Nexus私服
nexus是java开发的，所以运行的时候需要有java环境的支持。
<a name="vHtA5"></a>
### 安装jdk
<a name="gx3Ck"></a>
### 下载nexus
官网下载地址：[https://help.sonatype.com/repomanager3/download](https://help.sonatype.com/repomanager3/download)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690178171286-a64af355-4be8-4ab0-ab7b-4c5b3d4ef65d.png#averageHue=%23edd6b5&clientId=ud491625e-e301-4&from=paste&id=ue9a7b2b6&originHeight=234&originWidth=814&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7a94d579-06a5-48c7-8b29-df9ba8092fa&title=)
<a name="C6aTh"></a>
### 解压latest-win64.zip
latest-win64.zip解压之后会产生两个文件目录nexus-3.19.1-01和sonatyp-work<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690178171298-9990bf5c-147b-452b-a1bf-4b51c52f1057.png#averageHue=%23fbf6f5&clientId=ud491625e-e301-4&from=paste&id=u0e4ef0b4&originHeight=213&originWidth=665&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u6d68e55d-ab7c-4c3e-98ce-20bd3a2df82&title=)
<a name="v0zHN"></a>
### 启动nexus
cmd中直接运行nexus-3.19.1-01/bin/nexus.exe /run ，如下：
```bash
D:\installsoft\maven\nexus\nexus-3.19.1-01\bin>nexus.exe /run
```
如果输出中出现了下面的异常请忽略
```
java.io.UnsupportedEncodingException: Encoding GBK is not supported yet (feel free to submit a patch)
```
浏览器中打开http://localhost:8081/访问<br />效果如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690178171236-cbd66b8b-7291-4f2e-bb8c-278b79f7f9fa.png#averageHue=%23e9e9e9&clientId=ud491625e-e301-4&from=paste&id=u691080d9&originHeight=678&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub1c759b8-b9c8-4492-8ac9-06dc78a57b0&title=)
<a name="UgwPp"></a>
### 登录Nexus
点击上图右上角的Sign in，输入用户名和密码，nexus默认用户名是admin<br />nexus这个版本的密码是第一次启动的时候生成的，密码位于下面的文件中：
```
安装目录/sonatype-work/nexus3/admin.password
```
登录成功后会弹出一些设置，如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690178171243-fb7de451-c834-4c49-9535-e1e5a522df92.png#averageHue=%23e3e3e3&clientId=ud491625e-e301-4&from=paste&id=u88684a4c&originHeight=166&originWidth=666&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubd399be4-b833-474b-9e11-91c2cdae4a6&title=)<br />点击Next，设置新的登录密码（新密码要保存好），如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690178171276-a4ff2672-e784-48e2-8078-4fbf67a94f1e.png#averageHue=%23eeedec&clientId=ud491625e-e301-4&from=paste&id=u42e4def7&originHeight=259&originWidth=653&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u74bbcc08-920d-40f7-81b1-640cd0470a7&title=)<br />点击Next->Finish完成设置。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690178171926-4cfc7c85-95ed-41e2-bca5-45c0915a43c9.png#averageHue=%23e2e2e2&clientId=ud491625e-e301-4&from=paste&id=ub7ceba09&originHeight=678&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubeb901b7-8acf-4eb1-b954-566ad4265a6&title=)
<a name="k7xtj"></a>
### 其他一些常见的操作
<a name="yxjDn"></a>
#### 停止Nexus的命令
启动的cmd窗口中按：ctrl+c，可以停止Nexus。
<a name="AFRbw"></a>
#### 修改启动端口
默认端口是8081，如果和本机有冲突，可以在下面的文件中修改：
```
nexus-3.19.1-01\etc\nexus-default.properties
```
nexus使用java开发的web项目，内置了jetty web容器，所以可以直接运行。
<a name="CZBMI"></a>
## Linux安装Nexus私服
<a name="qUZsw"></a>
### 下载安装包
从官网[https://help.sonatype.com/repomanager3/download](https://help.sonatype.com/repomanager3/download)下载Linux版本的nexus安装包，选择latest-unix.tar.gz文件。<br />将下载好的的安装包放在/opt/nexus/目录。
<a name="gfGuH"></a>
### 解压
```bash
[root@test1117 nexus]# tar -zvxf latest-unix.tar.gz
[root@test1117 nexus]# ls
latest-unix.tar.gz  nexus-3.19.1-01  sonatype-work
```
<a name="RkI8g"></a>
### 启动
```bash
[root@test1117 bin]# /opt/nexus/nexus-3.19.1-01/bin/nexus start
WARNING: ************************************************************
WARNING: Detected execution as "root" user.  This is NOT recommended!
WARNING: ************************************************************
Starting nexus
```
上面使用的是root用户操作的，为了安全性，最好自己创建个用户来操作。
<a name="QNvMh"></a>
### 开放端口
在/etc/sysconfig/iptables文件中加入下面内容：
```
-A INPUT -p tcp -m state --state NEW -m tcp --dport 8081 -j ACCEPT
```
执行下面命令，让上面配置生效：
```bash
[root@test1117 bin]# service iptables restart
Redirecting to /bin/systemctl restart  iptables.service
```
<a name="FS7HH"></a>
### 验证效果
访问http://nexus私服所在的机器ip:8081/<br />出现下面效果表示一切ok。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690178171956-573a4063-bbf2-46f8-849a-d8a268eaa9a5.png#averageHue=%23e6e5e5&clientId=ud491625e-e301-4&from=paste&id=u8c8f3246&originHeight=659&originWidth=946&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u55645027-9b90-451c-9582-293dfcbcae1&title=)
<a name="mJrUJ"></a>
### 登录
用户名为admin，密码在
```
/opt/nexus/sonatype-work/nexus3/admin.password
```
登录之后请请立即修改密码。
<a name="mS8nf"></a>
## Nexus中仓库分类
前面说过，用户可以通过nexus去访问远程仓库，可以将本地的构件发布到nexus中，nexus是如何支撑这些操作的呢？<br />nexus中有个仓库列表，里面包含了各种各样的仓库，有被代理的第三方远程仓库，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690178171972-28c47656-e217-4441-8bc3-b85456d88a0a.jpeg#averageHue=%23e4e2e2&clientId=ud491625e-e301-4&from=paste&id=u2b0d2c8b&originHeight=678&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u6068954d-c7c8-4e16-a56e-ff9bf42c942&title=)<br />上图中是nexus安装好默认自带的仓库列表，主要有3种类型：

1. 代理仓库
2. 宿主仓库
3. 仓库组
<a name="DWIaV"></a>
### 代理仓库
代理仓库主要是让使用者通过代理仓库来间接访问外部的第三方远程仓库的，如通过代理仓库访问Maven中央仓库、阿里的Maven仓库等等。代理仓库会从被代理的仓库中下载构件，缓存在代理仓库中以供Maven用户使用。<br />在nexus中创建一个阿里云的Maven代理仓库来看下过程如下。<br />Nexus仓库列表中点击Create repository按钮，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690178171935-9f1a196b-9dd3-4ae4-ae14-5a1c1b668bf8.png#averageHue=%23dddbdb&clientId=ud491625e-e301-4&from=paste&id=u5da50247&originHeight=418&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3af18b56-a4c2-40d4-91ed-b3da90be8f2&title=)<br />进入添加页面，选择maven2(proxy)，这个表示代理仓库，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690178171934-c5ce4dc8-e7d9-4c4a-b434-e357dab24d29.jpeg#averageHue=%23e7e7e7&clientId=ud491625e-e301-4&from=paste&id=u54f1c814&originHeight=678&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7c0e9ad0-462f-4615-a3f9-d81ca77856b&title=)<br />输入远程仓库的信息，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690178173367-3e8f36e5-9e2d-4657-8f7f-df2bdf17ee9d.png#averageHue=%23fcfbfa&clientId=ud491625e-e301-4&from=paste&id=u0806491a&originHeight=816&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u6fe19379-267d-44f7-9334-675741f9c95&title=)
> 第一个红框中输入仓库名称：maven-aliyun
> 第二个红框选择：Release，表示从这个仓库中下载稳定版的构件
> 第三个红框输入阿里云仓库地址：https://maven.aliyun.com/repository/public

点击底部的Create repository按钮，创建完成，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690178173142-1eb49b67-0753-4a4f-acff-f06606db8f99.jpeg#averageHue=%23e3e2e2&clientId=ud491625e-e301-4&from=paste&id=u2125a282&originHeight=678&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud247fa5c-fa6b-4816-8189-f2e0f160242&title=)
<a name="Tcvhu"></a>
### 宿主仓库
宿主仓库主要是自己用的，主要有2点作用

1. 将私有的一些构件通过nexus中网页的方式上传到宿主仓库中给其他同事使用
2. 将自己开发好一些构件发布到nexus的宿主仓库中以供其他同事使用

上面这2种操作，一会稍后会详解以及演示操作过程。
<a name="Wtmtd"></a>
### 仓库组
Maven用户可以从代理仓库和宿主仓库中下载构件至本地仓库，为了方便从多个代理仓库和宿主仓库下载构件，Maven提供了仓库组，仓库组中可以有多个代理仓库和宿主仓库，而Maven用户只用访问一个仓库组就可以间接的访问这个组内所有的仓库，仓库组中多个仓库是有顺序的，当Maven用户从仓库组下载构件时，仓库组会按顺序依次在组内的仓库中查找组件，查找到了立即返回给本地仓库，所以一般情况会将速度快的放在前面。<br />仓库组内部实际上是没有构件内容的，他只是起到一个请求转发的作用，将Maven用户下载构件的请求转发给组内的其他仓库处理。<br />nexus默认有个仓库组maven-public，如下：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690178173105-360368af-f46e-4036-b13e-09a2c90e409b.jpeg#averageHue=%23e4e4e3&clientId=ud491625e-e301-4&from=paste&id=u3ee37377&originHeight=678&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2f8436f0-6ff4-4f60-8d11-3c579825988&title=)<br />点击一下maven-public这行记录，进去看一下，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690178173243-9daa0296-88f6-4107-8df4-c88f5b6b79ee.png#averageHue=%23fbfaf9&clientId=ud491625e-e301-4&from=paste&id=u642e5acb&originHeight=862&originWidth=907&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2f98ab0d-6b51-4f9d-b434-a0ff54b2070&title=)<br />上图中第一个红框是这个仓库组对外的一个url，本地的Maven可以通过这个url来从仓库组中下载构件至本地仓库。<br />第二个红框中是这个仓库组中的成员，目前包含了3个仓库，第1个是宿主的releases版本仓库，第1个是宿主快照版本的仓库，第3个是代理仓库（Maven社区中央仓库的代理）。<br />刚才新增的maven-aliyun在左边，将其也加到右边的仓库成员（Members）列表，然后将maven-aliyun这个仓库放在第3个位置，这个仓库的速度比maven-central要快一些，能加速下载Maven构件的速度，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690178173230-8bd399be-c15d-41fb-aef7-62518f8581cd.png#averageHue=%23fcfafa&clientId=ud491625e-e301-4&from=paste&id=u08ec8e7c&originHeight=370&originWidth=838&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u196b6885-51d2-47ee-8030-3a58d8864c5&title=)
<a name="CmC1A"></a>
## 配置本地Maven从nexus下载构件
介绍2种方式
<a name="VfzHu"></a>
### 方式1：pom.xml的方式
从nexus默认仓库组中下载构件，先获取仓库组对外的地址，点击下图中的copy按钮，获取仓库组的地址：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690178214856-bb3fc4a6-259c-4d5d-a42c-011ae3563c37.jpeg#averageHue=%23e0dddd&clientId=ud491625e-e301-4&from=paste&id=u590b0d80&originHeight=546&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u17f01555-aa4a-45f9-98cd-9b38b1d8af4&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690178214876-7e36fe41-8310-4ff0-a222-fbf1eb77aefc.png#averageHue=%23e5e5e5&clientId=ud491625e-e301-4&from=paste&id=u2e45903b&originHeight=129&originWidth=489&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uac5ef863-32b4-4f93-a312-d97c7103300&title=)<br />修改pom.xml，加入如下内容：<br />注意下面url的地址为上面复制的地址。
```xml
<repositories>
  <repository>
    <id>maven-nexus</id>
    <url>http://localhost:8081/repository/maven-public/</url>
    <releases>
      <enabled>true</enabled>
    </releases>
    <snapshots>
      <enabled>true</enabled>
    </snapshots>
  </repository>
</repositories>
```
由于nexus私服需要有用户名和密码登录才能访问，所以需要有凭证，凭证需要在settings.xml文件中配置，在~/.m2/settings.xml文件的servers元素中加入如下内容：
```xml
<server>
  <id>maven-nexus</id>
  <username>admin</username>
  <password>admin123</password>
</server>
```
注意上面的server->id的值和pom.xml中repository->id的值一致，通过这个id关联找到凭证的。<br />server元素中的username和password根据自己的去编辑，这边密码设置的是admin123
<a name="Ekujs"></a>
### 方式1示例
创建一个Maven项目，打开idea，点击File->New->Project，选择Maven，如下：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690178214786-0b632acc-5d77-48b4-9e53-cfce847ef639.jpeg#averageHue=%23dcc5be&clientId=ud491625e-e301-4&from=paste&id=u9e0a75de&originHeight=696&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u271649c1-0e2a-41db-9934-031ae48899d&title=)<br />点击Next，输入项目坐标信息，如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690178214891-798707c1-be7f-423e-96f0-3f135b46182a.png#averageHue=%23e9e7e7&clientId=ud491625e-e301-4&from=paste&id=u593cb1a9&originHeight=696&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u1c5ef046-f9ff-45f2-b79a-6bd8d73da8d&title=)<br />点击Next，输入Project name 为maven-chat05，如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690178215049-ad1994cc-80b1-4dba-83c7-287634939f35.png#averageHue=%23e8e6e6&clientId=ud491625e-e301-4&from=paste&id=u01836e15&originHeight=696&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue9635aaa-c6f2-48d2-84de-5f57eb3d1b0&title=)<br />点击Finish，创建成功，如下：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690178216168-d0aa32c3-dd8f-405f-a659-e6d76a991c3e.jpeg#averageHue=%23f5f5f4&clientId=ud491625e-e301-4&from=paste&id=u6809f3c5&originHeight=712&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4a14a5f4-3133-4b89-9e2d-e645a5b77b4&title=)<br />配置一下idea的Maven环境，点击File->Settings，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690178216611-dee68950-36aa-4667-af27-a3a181608698.jpeg#averageHue=%23d3cfc9&clientId=ud491625e-e301-4&from=paste&id=ucaf16ca2&originHeight=734&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud4ad8966-2aac-4e03-bcc0-f7650d4ad3e&title=)<br />点击上面的OK完成配置。<br />还原~/.m2/settings.xml的配置到初始状态，操作如下：
```
将M2_HOME/conf/settings.xml复制熬~/.m2/settings.xml目录，如果存在先备份一个，然后进行覆盖。
```
修改上面idea项目中的pom.xml文件，内容如下：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>

  <groupId>com.javacode2018</groupId>
  <artifactId>maven-chat05</artifactId>
  <version>1.0-SNAPSHOT</version>

  <dependencies>
    <dependency>
      <groupId>com.alibaba</groupId>
      <artifactId>fastjson</artifactId>
      <version>1.2.62</version>
    </dependency>
  </dependencies>

  <repositories>
    <repository>
      <id>maven-nexus</id>
      <url>http://localhost:8081/repository/maven-public/</url>
      <releases>
        <enabled>true</enabled>
      </releases>
      <snapshots>
        <enabled>true</enabled>
      </snapshots>
    </repository>
  </repositories>

</project>
```
删除本地仓库中以下几个目录：
```
~\.m2\repository\com\alibaba
```
maven-chat05项目目录中打开cmd运行：
```
mvn compile
```
输出如下：
```
D:\code\IdeaProjects\maven-chat05>mvn compile
[INFO] Scanning for projects...
[INFO]
[INFO] -------------------< com.javacode2018:maven-chat05 >--------------------
[INFO] Building maven-chat05 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
Downloading from maven-nexus: http://localhost:8081/repository/maven-public/com/alibaba/fastjson/1.2.62/fastjson-1.2.62.pom
Downloaded from maven-nexus: http://localhost:8081/repository/maven-public/com/alibaba/fastjson/1.2.62/fastjson-1.2.62.pom (9.7 kB at 5.1 kB/s)
Downloading from maven-nexus: http://localhost:8081/repository/maven-public/com/alibaba/fastjson/1.2.62/fastjson-1.2.62.jar
Downloaded from maven-nexus: http://localhost:8081/repository/maven-public/com/alibaba/fastjson/1.2.62/fastjson-1.2.62.jar (658 kB at 70 kB/s)
[INFO]
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ maven-chat05 ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ maven-chat05 ---
[INFO] Nothing to compile - all classes are up to date
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  16.083 s
[INFO] Finished at: 2019-11-13T15:45:44+08:00
[INFO] ------------------------------------------------------------------------
```
从输出中可以看出fastjson这个jar包从maven-nexus中下载了，下载地址正是nexus私服中的那个地址。
<a name="uOLXH"></a>
### 方式2：镜像方式
镜像的方式主要修改~/.m2/setting.xml文件，需要修改2处理。
<a name="SIbMy"></a>
#### 第1处：setting.xml的mirrors元素中加入如下配置：
```xml
<mirror>
  <id>mirror-nexus</id>
  <mirrorOf>*</mirrorOf>
  <name>nexus镜像</name>
  <url>http://localhost:8081/repository/maven-public/</url>
</mirror>
```
上面mirrorOf配置的`*`，说明所有远程仓库都通过该镜像下载构件。<br />url：这个为nexus中仓库组的地址，上面方式一中有说过。
<a name="yCwwl"></a>
#### 第2处：由于nexus的url是需要用户名和密码才可以访问的，所以需要配置访问凭证，在`~/.m2/settings.xml`文件的`servers`元素中加入如下内容：
```xml
<server>
  <id>mirror-nexus</id>
  <username>admin</username>
  <password>admin123</password>
</server>
```
注意上面的server->id的值和mirror->id的值需要一致，这样才能找到对应的凭证。
<a name="DvL6e"></a>
### 方式2示例
还是以方式1中的Maven项目maven-chat05为例。<br />修改pom.xml，内容如下：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>

  <groupId>com.javacode2018</groupId>
  <artifactId>maven-chat05</artifactId>
  <version>1.0-SNAPSHOT</version>

  <dependencies>
    <dependency>
      <groupId>com.alibaba</groupId>
      <artifactId>fastjson</artifactId>
      <version>1.2.62</version>
    </dependency>
  </dependencies>

</project>
```
~/.m2/settings.xml的servers元素中加入下面内容：
```xml
<server>
  <id>mirror-nexus</id>
  <username>admin</username>
  <password>admin123</password>
</server>
```
注意上面username、password根据自己的进行配置。<br />~/.m2/settings.xml的mirrors元素中加入下面内容：
```xml
<mirror>
  <id>mirror-nexus</id>
  <mirrorOf>*</mirrorOf>
  <name>nexus镜像</name>
  <url>http://localhost:8081/repository/maven-public/</url>
</mirror>
```
删除本地仓库中以下几个目录：
```
~\.m2\repository\com\alibaba
```
maven-chat05项目目录中打开cmd运行：
```
mvn compile
```
输出如下：
```
D:\code\IdeaProjects\maven-chat05>mvn compile
[INFO] Scanning for projects...
[INFO]
[INFO] -------------------< com.javacode2018:maven-chat05 >--------------------
[INFO] Building maven-chat05 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
Downloading from mirror-nexus: http://localhost:8081/repository/maven-public/org/apache/maven/plugins/maven-resources-plugin/2.6/maven-resources-plugin-2.6.pom
Downloaded from mirror-nexus: http://localhost:8081/repository/maven-public/org/apache/maven/plugins/maven-resources-plugin/2.6/maven-resources-plugin-2.6.pom (0 B at 0 B/s)
Downloading from mirror-nexus: http://localhost:8081/repository/maven-public/org/apache/maven/plugins/maven-plugins/23/maven-plugins-23.pom
Downloaded from mirror-nexus: http://localhost:8081/repository/maven-public/org/apache/maven/plugins/maven-plugins/23/maven-plugins-23.pom (0 B at 0 B/s)
Downloading from mirror-nexus: http://localhost:8081/repository/maven-public/org/apache/maven/maven-parent/22/maven-parent-22.pom
Downloaded from mirror-nexus: http://localhost:8081/repository/maven-public/org/apache/maven/maven-parent/22/maven-parent-22.pom (0 B at 0 B/s)
Downloading from mirror-nexus: http://localhost:8081/repository/maven-public/org/apache/apache/11/apache-11.pom
Downloaded from mirror-nexus: http://localhost:8081/repository/maven-public/org/apache/apache/11/apache-11.pom (0 B at 0 B/s)
Downloading from mirror-nexus: http://localhost:8081/repository/maven-public/org/apache/maven/plugins/maven-resources-plugin/2.6/maven-resources-plugin-2.6.jar
Downloaded from mirror-nexus: http://localhost:8081/repository/maven-public/org/apache/maven/plugins/maven-resources-plugin/2.6/maven-resources-plugin-2.6.jar (0 B at 0 B/s)
Downloading from mirror-nexus: http://localhost:8081/repository/maven-public/org/apache/maven/plugins/maven-compiler-plugin/3.1/maven-compiler-plugin-3.1.pom
Downloaded from mirror-nexus: http://localhost:8081/repository/maven-public/org/apache/maven/plugins/maven-compiler-plugin/3.1/maven-compiler-plugin-3.1.pom (0 B at 0 B/s)
[INFO] Nothing to compile - all classes are up to date
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  02:23 min
[INFO] Finished at: 2019-11-13T16:05:43+08:00
[INFO] ------------------------------------------------------------------------
```
输出内容比较多，只截取了部分输出，从输出中可以看出fastjson这个jar包从mirror-nexus中下载了，下载地址正是nexus私服中的那个地址，镜像的方式起效了！
<a name="HTVgL"></a>
## 本地构件发布到私服
<a name="bAI83"></a>
### 经常用到的有2种

1. **使用Maven部署构件至nexus私服**
2. **手动部署第三方构件至nexus私服**：比如第三方发给第三方的一个包，比如短信发送商的jar包，这个包远程仓库是不存在的，要把这个包上传到私服供所有开发使用。

下面来看一下这两种如何操作。
<a name="KdRS3"></a>
### 使用Maven部署构件至nexus私服
创建Maven项目的时候，会有一个pom.xml文件，里面有个version元素，这个是这个构件的版本号，可以去看一下上面刚创建的maven-chat05，默认是1.0-SNAPSHOT，这个以-SNAPSHOT结尾的表示是个快照版本，叫做SNAPSHOT版本，快照版本一般是不稳定的，会反复发布、测试、修改、发布。而最终会有一个稳定的可以发布的版本，是没有-SNAPSHOT后缀的，这个叫做release版本。<br />而nexus私服中存储用户的构件是使用的宿主仓库，nexus私服中提供了两个默认的宿主仓库分别用来存放SNAPSHOT版本和release版本，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690178216480-8e3adc16-d156-4c1d-8638-aa8e222cb8ea.png#averageHue=%23dedddd&clientId=ud491625e-e301-4&from=paste&id=uf7d1df87&originHeight=556&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u15cf9273-b383-40d9-bc9b-40657068968&title=)<br />上图中第1个红框的maven-releases宿主仓库用来存放用户自己release版本的构件。<br />第2个红框的maven-snapshots宿主仓库用来存放用户snapshot版本的构件。<br />上面两个仓库的地址可以点击后面的copy按钮获取。
<a name="NtoQf"></a>
#### 操作分为3步
<a name="iuIKD"></a>
##### 第一步：修改pom.xml配置
需要将本地Maven项目的构件发布到上面宿主仓库中，需要修改项目中pom.xml的配置，加入下面内容：
```xml
<distributionManagement>
  <repository>
    <id>release-nexus</id>
    <url>http://localhost:8081/repository/maven-releases/</url>
    <name>nexus私服中宿主仓库->存放/下载稳定版本的构件</name>
  </repository>
  <snapshotRepository>
    <id>snapshot-nexus</id>
    <url>http://localhost:8081/repository/maven-snapshots/</url>
    <name>nexus私服中宿主仓库->存放/下载快照版本的构件</name>
  </snapshotRepository>
</distributionManagement>
```
上面2个url分别是上图中两个宿主仓库的地址。
<a name="RakC8"></a>
##### 第二步：修改settings.xml
上面地址需要登录才可以访问，所以需要配置凭证，这个需要在~/.m2/settings.xml中进行配置，在这个文件的servers元素中加入：
```xml
<server>
  <id>release-nexus</id>
  <username>admin</username>
  <password>admin123</password>
</server>

<server>
  <id>snapshot-nexus</id>
  <username>admin</username>
  <password>admin123</password>
</server>
```
注意上面第1个server->id的值需要和pom.xml中的distributionManagement->repository->id的值一致。<br />第2个server->id的值需要和pom.xml中的distributionManagement->snapshotRepository->id的值一致。
<a name="JrprY"></a>
##### 第三步：执行`mvn deploy`命令
执行这个命令的时候，会对构件进行打包，然后上传到私服中。<br />这命令的原理，后面的文章中会具体介绍。
<a name="oEOvJ"></a>
####  示例效果
来感受一下效果。<br />按照上面的配置修改maven-chat03/pom.xml文件和本地的~/.m2/settings.xml文件。<br />maven-chat05项目目录中打开cmd运行：
```
mvn deploy
```
见证奇迹的时刻，输出如下：
```
D:\code\IdeaProjects\maven-chat05>mvn deploy
[INFO] Scanning for projects...
[INFO]
[INFO] -------------------< com.javacode2018:maven-chat05 >--------------------
[INFO] Building maven-chat05 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ maven-chat05 ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ maven-chat05 ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ maven-chat05 ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] skip non existing resourceDirectory D:\code\IdeaProjects\maven-chat05\src\test\resources
[INFO]
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ maven-chat05 ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ maven-chat05 ---
[INFO] No tests to run.
[INFO]
[INFO] --- maven-jar-plugin:2.4:jar (default-jar) @ maven-chat05 ---
[INFO]
[INFO] --- maven-install-plugin:2.4:install (default-install) @ maven-chat05 ---
[INFO] Installing D:\code\IdeaProjects\maven-chat05\target\maven-chat05-1.0-SNAPSHOT.jar to C:\Users\Think\.m2\repository\com\javacode2018\maven-chat05\1.0-SNAPSHOT\maven-chat05-1.0-SNAPSHOT.jar
[INFO] Installing D:\code\IdeaProjects\maven-chat05\pom.xml to C:\Users\Think\.m2\repository\com\javacode2018\maven-chat05\1.0-SNAPSHOT\maven-chat05-1.0-SNAPSHOT.pom
[INFO]
[INFO] --- maven-deploy-plugin:2.7:deploy (default-deploy) @ maven-chat05 ---
Downloading from snapshot-nexus: http://localhost:8081/repository/maven-snapshots/com/javacode2018/maven-chat05/1.0-SNAPSHOT/maven-metadata.xml
Uploading to snapshot-nexus: http://localhost:8081/repository/maven-snapshots/com/javacode2018/maven-chat05/1.0-SNAPSHOT/maven-chat05-1.0-20191113.083820-1.jar
Uploaded to snapshot-nexus: http://localhost:8081/repository/maven-snapshots/com/javacode2018/maven-chat05/1.0-SNAPSHOT/maven-chat05-1.0-20191113.083820-1.jar (1.8 kB at 11 kB/s)
Uploading to snapshot-nexus: http://localhost:8081/repository/maven-snapshots/com/javacode2018/maven-chat05/1.0-SNAPSHOT/maven-chat05-1.0-20191113.083820-1.pom
Uploaded to snapshot-nexus: http://localhost:8081/repository/maven-snapshots/com/javacode2018/maven-chat05/1.0-SNAPSHOT/maven-chat05-1.0-20191113.083820-1.pom (1.2 kB at 10 kB/s)
Downloading from snapshot-nexus: http://localhost:8081/repository/maven-snapshots/com/javacode2018/maven-chat05/maven-metadata.xml
Uploading to snapshot-nexus: http://localhost:8081/repository/maven-snapshots/com/javacode2018/maven-chat05/1.0-SNAPSHOT/maven-metadata.xml
Uploaded to snapshot-nexus: http://localhost:8081/repository/maven-snapshots/com/javacode2018/maven-chat05/1.0-SNAPSHOT/maven-metadata.xml (772 B at 9.2 kB/s)
Uploading to snapshot-nexus: http://localhost:8081/repository/maven-snapshots/com/javacode2018/maven-chat05/maven-metadata.xml
Uploaded to snapshot-nexus: http://localhost:8081/repository/maven-snapshots/com/javacode2018/maven-chat05/maven-metadata.xml (286 B at 3.4 kB/s)
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  6.156 s
[INFO] Finished at: 2019-11-13T16:38:20+08:00
[INFO] ------------------------------------------------------------------------
```
输出内容中有Uploading to snapshot-nexus、Uploaded to snapshot-nexus ，snapshot-nexus正是在pom.xml配置的快照版本的地址，上面输出内容中有具体的地址，和快照的地址也是一样的，上传成功了。<br />去nexus私服中看一下，访问nexus私服中快照版本仓库的地址：http://localhost:8081/repository/maven-snapshots/<br />出现如下页面：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690178216542-eabfb64a-bb68-4a2e-95d4-160a137306ff.png#averageHue=%23cecac8&clientId=ud491625e-e301-4&from=paste&id=u368e25da&originHeight=207&originWidth=564&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uaa737c0f-c6a4-4a6b-a351-c65e232820f&title=)<br />点击上面的bowse连接，如下图，构件上传成功了：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690178216498-a059a518-16fc-47a7-9ba6-4b8c7e1ca1c9.png#averageHue=%23e2dfdf&clientId=ud491625e-e301-4&from=paste&id=u2d4c3533&originHeight=598&originWidth=1015&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua3e9230b-abc5-4fae-a4b8-6e14fc5b659&title=)<br />如果其他同事需要使用上面这个构件，只需要点击下图中的pom文件，右边会显示构件的坐标，然后可以拿去使用了，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690178217880-11b858a2-f9c8-4204-9911-39478872d13b.jpeg#averageHue=%23e5e3e3&clientId=ud491625e-e301-4&from=paste&id=u4fc8e745&originHeight=899&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub444bdcc-5463-4e51-85b6-4b34ea3561b&title=)<br />上面是将快照版本的发布到了nexus私服的快照宿主仓库了，下面再来操作一下将release稳定版本的发布到nexus私服，如下：<br />修改pom.xml文件的版本号，将-SNAPSHOT后缀去掉，去掉之后表示是release版本的了，如下：
```xml
<version>1.0</version>
```
cmd命令中执行：
```
mvn deploy
```
输出：
```
D:\code\IdeaProjects\maven-chat05>mvn deploy
[INFO] Scanning for projects...
[INFO]
[INFO] -------------------< com.javacode2018:maven-chat05 >--------------------
[INFO] Building maven-chat05 1.0
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ maven-chat05 ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ maven-chat05 ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ maven-chat05 ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] skip non existing resourceDirectory D:\code\IdeaProjects\maven-chat05\src\test\resources
[INFO]
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ maven-chat05 ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ maven-chat05 ---
[INFO] No tests to run.
[INFO]
[INFO] --- maven-jar-plugin:2.4:jar (default-jar) @ maven-chat05 ---
[INFO] Building jar: D:\code\IdeaProjects\maven-chat05\target\maven-chat05-1.0.jar
[INFO]
[INFO] --- maven-install-plugin:2.4:install (default-install) @ maven-chat05 ---
[INFO] Installing D:\code\IdeaProjects\maven-chat05\target\maven-chat05-1.0.jar to C:\Users\Think\.m2\repository\com\javacode2018\maven-chat05\1.0\maven-chat05-1.0.jar
[INFO] Installing D:\code\IdeaProjects\maven-chat05\pom.xml to C:\Users\Think\.m2\repository\com\javacode2018\maven-chat05\1.0\maven-chat05-1.0.pom
[INFO]
[INFO] --- maven-deploy-plugin:2.7:deploy (default-deploy) @ maven-chat05 ---
Uploading to release-nexus: http://localhost:8081/repository/maven-releases/com/javacode2018/maven-chat05/1.0/maven-chat05-1.0.jar
Uploaded to release-nexus: http://localhost:8081/repository/maven-releases/com/javacode2018/maven-chat05/1.0/maven-chat05-1.0.jar (1.7 kB at 9.6 kB/s)
Uploading to release-nexus: http://localhost:8081/repository/maven-releases/com/javacode2018/maven-chat05/1.0/maven-chat05-1.0.pom
Uploaded to release-nexus: http://localhost:8081/repository/maven-releases/com/javacode2018/maven-chat05/1.0/maven-chat05-1.0.pom (1.2 kB at 12 kB/s)
Downloading from release-nexus: http://localhost:8081/repository/maven-releases/com/javacode2018/maven-chat05/maven-metadata.xml
Uploading to release-nexus: http://localhost:8081/repository/maven-releases/com/javacode2018/maven-chat05/maven-metadata.xml
Uploaded to release-nexus: http://localhost:8081/repository/maven-releases/com/javacode2018/maven-chat05/maven-metadata.xml (304 B at 4.3 kB/s)
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  5.646 s
[INFO] Finished at: 2019-11-13T16:48:44+08:00
[INFO] ------------------------------------------------------------------------
```
输出中有release-nexus，说明使用了pom.xml中的distributionManagement->repository->id的值，上传地址是http://localhost:8081/repository/maven-releases。<br />打开nexus私服中release地址：http://localhost:8081/repository/maven-releases/<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690178217951-18ddc3e5-c5f6-465b-b42c-bfae81314437.png#averageHue=%23d0cecd&clientId=ud491625e-e301-4&from=paste&id=u82e528b4&originHeight=250&originWidth=675&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4d5f3814-aeb1-473a-bbc3-ffab64881cb&title=)<br />点击上面的browse连接，如下：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690178217924-f02b4646-e3ca-49c3-91ab-8a443a6c3c54.jpeg#averageHue=%23e5e3e3&clientId=ud491625e-e301-4&from=paste&id=u974c151d&originHeight=777&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9dfbc1e5-01f6-451d-b844-991c3c4c23e&title=)<br />成功了，是不是感觉很爽。
<a name="ji3DT"></a>
### 手动部署构件至nexus私服
<a name="IYF9t"></a>
#### 操作步骤
**手动上传只支持发布稳定版本的构件**，操作过程如下图：<br />登录nexus，按照下图的步骤依次点击：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690178218046-f0731643-0b37-4637-be96-5304e4051b30.png#averageHue=%23e1dede&clientId=ud491625e-e301-4&from=paste&id=u8e4ce657&originHeight=730&originWidth=1044&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u19bdb575-3dba-4968-ba50-20294308a3e&title=)<br />图中第一行maven-releases宿主仓库就是存放用户自己构件的仓库，点击上图中列表中的第一行，进入上传页面，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690178217844-03009a6e-b9c5-44fc-b3bf-6f3d23cd103a.png#averageHue=%23e6e4e4&clientId=ud491625e-e301-4&from=paste&id=u554eb392&originHeight=730&originWidth=1044&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=udb2df4f8-a3cd-4fb4-bc0e-3992abc68c5&title=)<br />上面页面中点击Browse选择本地的构件，然后输入其他坐标信息，点击Upload完成上传操作。
<a name="mRIdP"></a>
#### 示例效果
把maven-chat05\target\maven-chat05-1.0.jar上传上去，如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690178219143-6b0ee06d-f1eb-424b-8c97-6eccdfc8c9a0.png#averageHue=%23fbf6f6&clientId=ud491625e-e301-4&from=paste&id=uc730b270&originHeight=577&originWidth=805&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u784ab1d2-b7c0-4446-a750-c7d03f99d99&title=)<br />点击Upload完成上传操作。<br />访问如下地址：http://localhost:8081/#browse/browse:maven-releases<br />可以看到上传好的构件，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690178219428-d70f548a-ce6e-4cd6-9a42-7798c038e85c.jpeg#averageHue=%23e5e5e4&clientId=ud491625e-e301-4&from=paste&id=u3ba94ddb&originHeight=698&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0372fd45-f8c4-44fb-8f83-7857769d0f3&title=)
