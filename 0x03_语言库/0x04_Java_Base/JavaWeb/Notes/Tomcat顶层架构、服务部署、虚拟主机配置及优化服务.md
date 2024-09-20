JavaWeb Tomcat
<a name="trAmI"></a>
## 一、Tomcat概述

- Tomcat是Java语言开发的，Tomcat服务器是一个免费的开放源代码的Web应用服务器，是Apache软件基金会的Jakarta项目中的一个核心项目，由Apache、Sun和其他一些公司及个人共同开发而成。
- Tomcat属于轻量级应用服务器，在中小型系统和并发访问用户不是很多的场合下被普遍使用，是开发和调试JSP程序的首选。
- 一般来说，Tomcat虽然和Apzche或者Nginx这些Web服务器一样，具有处理HTML页面的功能，然而由于七处理静态HTML的能力远不及Apache或者Nginx，所以Tomcat通常是作为一个Servlet和JSP容器，单独运行在后端。
<a name="ctxxv"></a>
### 1、Tomcat介绍

- 免费的、开放源代码的Web应用服务器
- Apache软件基金会(Apache Software Foundation)Jakarta项目中的一个核心项目
- 由Apache、Sun和一些公司及个人共同开发而成
- 深受Java爱好者的喜爱，并得到部分软件开发商的认可
- 目前比较流行的Web应用服务器
<a name="TAwI7"></a>
### 2、Tomcat三个核心组件

- Tomcat 由一系列的组件构成，其中核心的组件有三个
| 名称 | 功能 |
| --- | --- |
| Web 容器 | 完成 Web 服务器的功能 |
| Servlet 容器 | 名字为 catalina，用于处理 Servlet 代码 |
| JSP 容器 | 用于将 JSP 动态网页翻译成 Servlet 代码 |

![2021-06-14-19-00-36-487158.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1623669600937-69971972-7286-4c80-975e-fc04defc56e2.png#clientId=ue4fe2917-5186-4&from=ui&id=u1bb7f818&originHeight=584&originWidth=1080&originalType=binary&ratio=3&size=1895885&status=done&style=shadow&taskId=u74e3f52e-c4bc-41fc-8a33-08369d6a25e)
<a name="laMxQ"></a>
### 3、Java Servlet

- 运行在 Web 服务器或应用服务器上的程序，它是作为来自 Web 浏览器或其他 HTTP 客户端的请求和 HTTP
- 服务器上的数据库或应用程序之间的中间层。使用Servlet，可以收集来自网页表单的用户输入，呈现来自数据库或者其他源的记录，还可以动态创建网页。与CGI（公共网关接口）功能相类似。
<a name="IZEKF"></a>
### 4、JSP

- JSP全称Java Server Pages，一种动态网页开发技术。它使用JSP标签在HTML网页中插入Java代码。标签通常以<%开头以%>结束。
- JSP是一种Java servlet，主要用于实现Java web应用程序的用户界面部分。
- JSP通过网页表单获取用户输入数据、访问数据库及其他数据源，然后动态地创建网页。
<a name="dx6bP"></a>
## 二、Tomcat顶层架构
1、Tomcat中最顶层的容器是Server， 代表着整个服务器，一 个Server可以包含至少一 个Service，用于具体提供服务。<br />2、Service主要包含两个部分：Connector和Container。Tomcat的心脏就是这两个组件，这两个组件的作用: Connector用于处理连接相关的事情，并提供Socket与Request和Response相关的转化;Container用于封装和管理Servlet， 以及具体处理Request请求。<br />3、一个Tomcat中只有一-个Server， 一个Server可以包含多个Service， 一个Service只有一个Container， 但是可以有多个Connectors， 因为一个服务可以有多个连接，如同时提供Http和Https链接， 也可以提供向相同协议不同端口的连接。<br />4、多个Connector 和一个Container 就形成了一个Service， 有了Service 就可以对外提供服务了，但是Service还要一个生存的环境，必须要有人能够给她生命、掌握其生死大权，那就非Server 莫属了！所以整个Tomcat的生命周期由Server控制。另外，上述的包含关系或者说是父子关系，都可以在tomcat的conf目录下的server.xml配置文件（主配置文件）中看出。<br />5、server . xml中定义 `<Server port="8005" shutdown=" SHUTDOWN">`<br />这会让Tomcat6启动一 个server实例(即一 个JVM)，它监听在8005端口以接收“SHUTDOWN"命令，如果接收到了就会关闭Tomcat。各Server的定义不能使用同一个端口，这意味着如果在同一个物理机上启动了多个Server实例，必须配置它们使用不同的端口。<br />![2021-06-14-19-00-36-735626.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1623669588994-0bbefba7-3c68-4672-84f7-0419841c7b0b.png#clientId=ue4fe2917-5186-4&from=ui&id=u4f972e31&originHeight=740&originWidth=825&originalType=binary&ratio=3&size=1835275&status=done&style=shadow&taskId=ufe40cfda-f000-4fa2-ac75-13711385721)
<a name="Vsuyd"></a>
### Container容器

- Container用于封装和管理Servlet，以及具体处理Request请求，在Connector内部把喊了4个子容器。
- 4个子容器的作用分别是：
| 子容器名称 | 作用 |
| --- | --- |
| Engine | 引擎，用来管理多个站点，一个Service最多只能由一个Engine |
| Host | 代表一个站点，也可以叫虚拟主机，通过配置Host就可以添加站点 |
| Context | 代表一个应用程序，对应着平时开发的一套程序， 或者一个WEB-INE目录以及下面的web. xml文件 |
| Wrapper | 每一Wrapper封装着一 个Servlet |

<a name="fi880"></a>
## 三、Tomcat请求过程
1、用户在浏览器中输入该网址，请求被发送到本机端口8080，被在那里监听的CoyoteHTTP/1.1Connector获得;<br />2、Connector把该请求交给它所在的Service的Engine (Container) 来处理，并等待Engine的回应;<br />3、Engine获得请求localhost/test/ index. jsp， 匹配所有的虚拟主机Host;<br />4、Engine匹配到名为localhost的Host (即使匹配不到也把请求交给该Host处理，因为该Host被定义为该Engine的默认主机)，名为localhost的Host获得请求/ test/ index. jsp， 匹配它所拥有的所有Context。Host匹配到路径为/test的Context (如果匹配不到就把该请求交给路<br />径名为“”的Context去处理) ; .<br />5、path="/test"的Context获得请求/ index. jsp， 在它的mapping table中寻找出对应的Servlet。Context匹配到URL<br />Pattern为* . j sp的Servlet， 对应于JspServlet类;<br />6、构造HttpServletRequest对象和HttpServletResponse对象，作为参数调用JspServlet的doGet ()或doPost(),执行业务逻辑、数据存储等<br />7、Context把执行完之后的HttpServletResponse对象返回给Host;<br />8、Host把HttpServletResponse对象返回给Engine;<br />9、Engine把HttpServletResponse对象返回Connector;<br />10、Connector把HttpServletResponse对象返回给客户Browser<br />![2021-06-14-19-00-36-987951.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1623669578636-e97b5508-2e5b-425c-813d-1c033c628dfd.png#clientId=ue4fe2917-5186-4&from=ui&id=ua98ce553&originHeight=589&originWidth=1080&originalType=binary&ratio=3&size=1912119&status=done&style=none&taskId=u04be0ca0-4d65-494b-ae70-91c4b51a0ee)
<a name="dcDgd"></a>
## 四、Tomcat服务部署
1、下载并安装JDK<br />2、安装并启动Tomcat<br />3、配置虚拟主机<br />在部署 Tomcat 之前必须安装好 jdk，因为 jdk 是 Tomcat 运行的必要环境。
<a name="fkP9M"></a>
### 1、关闭防火墙，将安装 Tomcat 所需软件包传到/opt目录下
```bash
systemctl stop firewalld
systemctl disable firewalld
setenforce 0
```
<a name="MoozY"></a>
### 2、安装JDK
```bash
cd /opt
rpm -qpl jdk-8u201-linux-x64.rpm 
rpm -ivh jdk-8u201-linux-x64.rpm 
java -version
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623668418318-5f734d70-a5fc-4a13-851a-b1d9ba6a4ebf.webp#clientId=ue4fe2917-5186-4&from=paste&id=u90e48835&originHeight=54&originWidth=916&originalType=url&ratio=3&status=done&style=none&taskId=u44feb596-30d2-4a47-a994-52571c94b36)<br />![2021-06-14-19-00-37-320400.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1623669564712-685d8221-4a77-4607-94b7-62711b2a6bdf.png#clientId=ue4fe2917-5186-4&from=ui&id=u0272ef08&originHeight=218&originWidth=588&originalType=binary&ratio=3&size=385466&status=done&style=none&taskId=udbfc2c54-edf4-4497-bc9a-6807ecbb8f3)![2021-06-14-19-00-37-448101.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1623669565094-bfb24afc-cc9d-4034-9e62-b6c2d6fbd5db.png#clientId=ue4fe2917-5186-4&from=ui&id=u669f8eb0&originHeight=325&originWidth=917&originalType=binary&ratio=3&size=895932&status=done&style=none&taskId=u3962660e-8be0-49cf-8913-a81679610dd)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623668420507-34549ffd-4042-4d69-bed8-5d25273ed6b0.webp#clientId=ue4fe2917-5186-4&from=paste&id=u40aee4b4&originHeight=115&originWidth=913&originalType=url&ratio=3&status=done&style=none&taskId=ud3daf67a-78b5-4bd0-b1f5-dcb743fb55b)
<a name="A3zvR"></a>
### 3、设置JDK环境变量
```bash
vim /etc/profile.d/java.sh
export JAVA_HOME=/usr/java/jdk1.8.0_201-amd64
export CLASSPATH=.:$JAVA_HOME/lib/tools.jar:$JAVA_HOME/lib/dt.jar
export PATH=$JAVA_HOME/bin:$PATH

source /etc/profile.d/java.sh
java -version
```
![2021-06-14-19-00-37-627169.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1623669530473-80912ee1-8879-45b9-918b-b7e4e2977379.png#clientId=ue4fe2917-5186-4&from=ui&id=u060a0f94&originHeight=223&originWidth=981&originalType=binary&ratio=3&size=657656&status=done&style=none&taskId=u4d0efcbc-8016-4751-80c2-edd7fc65c46)![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623668420946-5536f75f-0087-4005-8a7e-9ee3dbc823af.webp#clientId=ue4fe2917-5186-4&from=paste&id=uddc9b2f0&originHeight=196&originWidth=980&originalType=url&ratio=3&status=done&style=none&taskId=ud9a0f8a1-2b02-4cb4-b3de-a9e64bf963d)
<a name="uyUTX"></a>
#### 小知识
| CLASSPATH | 编译、运行Java程序时，JRE会去该变量指定的路径中搜索所需的类（.class）文件。 |
| --- | --- |
| dt.jar | 是关于运行环境的类库，主要是可视化的 swing 的包。 |
| tools.jar | 主要是一些jdk工具的类库，包括javac、java、javap（jdk自带的一个反编译工具）、javadoc等。 |
| JDK | java development kit （java开发工具） |
| JRE | java runtime environment （java运行时环境） |
| JVM | java virtuak machine （java虚拟机），使java程序可以在多种平台上运行class文件。 |

首先使用文本工具编写java源代码，比如 Hello.java ；<br />在命令行中，输入命令：javac Hello.java，对源代码进行编译，生成 class 字节码文件；<br />编译完成后，如果没有报错信息，输入命令：java Hello，运行 class 字节码文件，由 JVM 对字节码进行解释和运行，打印 “Hello World”。
```bash
vim hello.java
#类名、接口名命令：英文大小写字母、数字字符、$和_，不能使用关键字和数字开头；
一个单词命名时第一个单词的首字母要大写；多单词组成时，所有单词的首字母大写：XxxYyyZzz（大驼峰命名法）
public class hello {
        public static void main(String[] args){
                System.out.println("Hello world!");
        }
}

javac hello.java
java hello
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623668421014-230e9db9-2c60-4732-8238-cbee7206ee54.webp#clientId=ue4fe2917-5186-4&from=paste&id=u045b4dc7&originHeight=183&originWidth=854&originalType=url&ratio=3&status=done&style=none&taskId=u247a03f2-8044-49fb-a2fa-1a38440e59b)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623668421433-b9e7d65b-ee42-4c78-9525-a11e902aa5ba.webp#clientId=ue4fe2917-5186-4&from=paste&id=uca6e3f12&originHeight=134&originWidth=937&originalType=url&ratio=3&status=done&style=none&taskId=ufaa220b4-8170-4052-91b8-8b5aadb4a18)
<a name="e2vql"></a>
### 4、安装启动Tomcat
```bash
cd /opt
tar zxvf apache-tomcat-9.0.16.tar.gz
mv apache-tomcat-9.0.16 /usr/local/tomcat
##启动tomcat ##
#后台启动
/usr/local/tomcat/bin/startup.sh 
或
/usr/local/tomcat/bin/catalina.sh start 
 
#前台启动
/usr/local/tomcat/bin/catalina.sh run  

netstat -natp | grep 8080
```
浏览器访问Tomcat的默认主页 http://192.168.184.70:8080<br />![2021-06-14-19-00-38-081930.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1623669355141-6f28b56f-3f22-4f3d-b546-ac2d24779fc5.png#clientId=ue4fe2917-5186-4&from=ui&id=u58eee3c5&originHeight=316&originWidth=1080&originalType=binary&ratio=3&size=1025895&status=done&style=none&taskId=u3289e436-52d5-49b7-9f97-5eb2bb5befb)![2021-06-14-19-00-38-187635.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1623669354422-71b4e2ff-d9d5-4398-b5e7-f36b9a6ce987.png#clientId=ue4fe2917-5186-4&from=ui&id=u92ed9a3e&originHeight=74&originWidth=1080&originalType=binary&ratio=3&size=35040&status=done&style=none&taskId=u1331d0fd-d1e3-4f0d-96dd-2e50565472f)![2021-06-14-19-00-38-728405.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1623669355991-19ca32de-e7f0-40f1-88ca-8c33e88f7b6a.png#clientId=ue4fe2917-5186-4&from=ui&id=ub940d40b&originHeight=673&originWidth=1080&originalType=binary&ratio=3&size=2184799&status=done&style=shadow&taskId=u6a257748-9b3a-4d4f-9a92-a5d9a4897aa)
<a name="gbCxs"></a>
### 5、优化tomcat启动速度
第一次启动tomcat可能会发现 Tomcat 启动很慢，默认情况下可能会需要几十秒，可以修改jdk参数进行改。
```bash
vim /usr/java/jdk1.8.0_201-amd64/jre/lib/security/java.security
--117行--修改
securerandom.source=file:/dev/urandom

-------------说明-------------
/dev/urandom是/dev/random的非阻塞版本，即当熵池中没有可取的随机数时，读取/dev/urandom设备仍然会返回一个随机数，只是该随机数的安全性理论上不高。如果应用对安全性要求很高，那么应该使用/dev/random。


/usr/local/tomcat/bin/shutdown.sh 
/usr/local/tomcat/bin/startup.sh 

ll /usr/local/tomcat/
```
![2021-06-14-19-00-38-826321.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1623669337447-29ebdc4d-3da8-448f-890b-887132d5fa7a.png#clientId=ue4fe2917-5186-4&from=ui&id=u295559d8&originHeight=48&originWidth=1080&originalType=binary&ratio=3&size=28742&status=done&style=none&taskId=u0983a037-9f3f-474d-acb0-4e3c04b32fd)![2021-06-14-19-00-38-909272.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1623669337448-e6577d2c-4da3-45d4-b3c5-5bb774f08dfb.png#clientId=ue4fe2917-5186-4&from=ui&id=ubda68883&originHeight=181&originWidth=894&originalType=binary&ratio=3&size=7374&status=done&style=none&taskId=uf4f286d8-8535-433e-a047-87d1b5071ee)<br />![2021-06-14-19-00-39-168089.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1623669327640-84f964c5-6b17-4715-a80a-9be5df6cb039.png#clientId=ue4fe2917-5186-4&from=ui&id=ue226ed0c&originHeight=608&originWidth=1080&originalType=binary&ratio=3&size=1973792&status=done&style=none&taskId=uc2f5b0a4-6742-4e8b-b741-e573622e7ad)<br />主要目录说明

| 目录 | 说明 |
| --- | --- |
| bin | 存放启动和关闭 Tomcat 的脚本文件，比较常用的是 catalina.sh、startup.sh、shutdown.sh 三个文件 |
| conf | 存放 Tomcat 服务器的各种配置文件，比较常用的是 server.xml、context.xml、tomcat-users.xml、web.xml 四个文件。 |
| lib | 存放 Tomcat 服务器的 jar 包，一般不作任何改动，除非连接第三方服务，比如 redis，那就需要添加相对应的 jar 包 |
| logs | 存放 Tomcat 日志 |
| temp | 存放 Tomcat 运行时产生的文件 |
| webapps | 存放项目资源的目录 |
| work | Tomcat 工作目录，一般清除 Tomcat 缓存的时候会使用到 |

<a name="STaR5"></a>
## 五、Tomcat 虚拟主机配置
很多时候公司会有多个项目需要运行，那么肯定不可能是一台服务器上运行多个Tomcat服务，这样会消耗太多的系统资源。此时，就需要使用到 Tomcat 虚拟主机。例如现在新增两个域名 www.lic.com 和 www.accp.com，希望通过这两个域名访问到不同的项目内容。
<a name="qrNsG"></a>
### 1、创建 ljm 和 lucien 项目目录和文件
```bash
mkdir /usr/local/tomcat/webapps/ljm
mkdir /usr/local/tomcat/webapps/lucien
echo "This is ljm page\!" > /usr/local/tomcat/webapps/ljm/index.jsp
echo "This is lucien page\!" > /usr/local/tomcat/webapps/lucien/index.jsp
```
<a name="FNu2J"></a>
### 2、修改 Tomcat 主配置文件
```bash
vim /usr/local/tomcat/conf/server.xml
--165行前--插入
<Host name="www.ljm.com" appBase="webapps" unpackWARs="true" autoDeploy="true" xmlValidation="false" xmlNamespaceAware="false">
 <Context docBase="/usr/local/tomcat/webapps/ljm" path="" reloadable="true" />
</Host>

<Host name="www.lucien.com" appBase="webapps" unpackWARs="true" autoDeploy="true" xmlValidation="false" xmlNamespaceAware="false">
 <Context docBase="/usr/local/tomcat/webapps/lucien" path="" reloadable="true" />
</Host>

/usr/local/tomcat/bin/shutdown.sh
/usr/local/tomcat/bin/startup.sh
```
| Host name | 主机名 |
| --- | --- |
| appBase | Tomcat程序工作目录，相对路径为webapps，绝对路径为/usr/local/tomcat/webapps |
| unpackWARs | 是否解压war包 |
| autoDeploy | 指示Tomcat运行时，如有新的WEB应用是否允许自动部署 |
| xmlValidation | 是否验证xml文件执行有效性检验的标志 |
| xmlNamespaceAware | 是否启用xml命名空间，设置该值与xmlValidation为true，表示对web.xml文件执行有效性检验 |
| docBase | WEB应用的目录 |
| path | 设置访问的URI为WEB应用的根目录 |
| reloadable | 是否在程序有改动时重新载入 |

<a name="jW2mH"></a>
### 3、客户端浏览器访问验证
```bash
echo "192.168.80.10 www.ljm.com www.lucien.com" >> /etc/hosts

浏览器访问 http://www.ljm.top:8080   页面显示This is ljm page\! 
浏览器访问 http://www.lucien.top:8080   页面显示This is lucien page\!
```
<a name="TKLeD"></a>
## 六、Tomcat 优化
Tomcat默认安装下的缺省配置并不适合生产环境，它可能会频繁出现假死现象需要重启，只有通过不断压测优化才能让它最高效率稳定的运行。优化主要包括三方面，分别为操作系统优化（内核参数优化），Tomcat配置文件参数优化，Java虚拟机（JVM）调优。
```bash
vim /usr/local/tomcat/conf/server.xml
......
<Connector port="8080" protocol="HTTP/11.1" 
connectionTimeout="20000" 
redirectPort="8443" 

--71行--插入
minSpareThreads="50" 
enableLookups="false" 
disableUploadTimeout="true" 
acceptCount="300" 
maxThreads="500" 
processorCache="500"
URIEncoding="UTF-8" 
compression="on" 
compressionMinSize="2048" 
compressableMimeType="text/html,text/xml,text/javascript,text/css,text/plain,image/gif,image /jpg,image/png"/>
```
<a name="o6lMp"></a>
## Tomcat常用优化选项参数说明
| 选项名称 | 参数设定 |
| --- | --- |
| maxThreads | Tomcat 使用线程来处理接收的每个请求，这个值表示 Tomcat 可创建的最大的线程数，默认值是 200。 |
| minSpareThreads | 最小空闲线程数，Tomcat 启动时的初始化的线程数，表示即使没有人使用也开这么多空线程等待，默认值是 10。 |
| maxSpareThreads | 最大备用线程数，一旦创建的线程超过这个值，Tomcat就会关闭不再需要的socket线程。默认值是-1（无限制）。一般不需要指定。 |
| URIEncoding | 指定 Tomcat 容器的 URL 编码格式，语言编码格式这块倒不如其它 Web服务器软件配置方便，需要分别指定。 |
| connnectionTimeout | 网络连接超时，单位：毫秒，设置为 0 表示永不超时，这样设置有隐患的。通常默认 20000 毫秒就可以。 |
| enableLookups | 是否反查域名，以返回远程主机的主机名，取值为：true 或 false，如果设置为 false，则直接返回 IP 地址，为了提高处理能力，应设置为 false。 |
| disableUploadTimeout | 上传时是否使用超时机制。应设置为 true。 |
| connectionUploadTimeout | 上传超时时间，毕竟文件上传可能需要消耗更多的时间，这个根据自己的业务需要自己调，以使Servlet有较长的时间来完成它的执行，需要与上一个参数一起配合使用才会生效。 |
| acceptCount | 指定当所有可以使用的处理请求的线程数都被使用时，可传入连接请求的最大队列长度，超过这个数的请求将不予处理，默认为 100 个。 |
| compression | 是否对响应的数据进行GZIP压缩，off：表示禁止压缩；on：表示允许压缩（文本将被压缩）、force：表示所有情况下都进行压缩，默认值为 off，压缩数据后可以有效的减少页面的大小，一般可以减小 1/3 左右，节省带宽。 |
| compressionMinSize | 表示压缩响应的最小值，只有当响应报文大小大于这个值的时候才会对报文进行压缩，如果开启了压缩功能，默认值就是 2048。 |
| compressableMimeType | 压缩类型，指定对哪些类型的文件进行数据压缩。 |
| noCompressionUserAgents=“gozilla, traviata” | 对于以下的浏览器，不启用压缩 |

