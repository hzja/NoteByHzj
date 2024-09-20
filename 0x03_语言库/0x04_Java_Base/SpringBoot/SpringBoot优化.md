Java SpringBoot<br />在SpringBoot的Web项目中，默认采用的是内置Tomcat，当然也可以配置支持内置的jetty，内置的好处：

1. 方便微服务部署。
2. 方便项目启动，不需要下载Tomcat或者Jetty

针对目前的容器优化，目前来说没有太多地方，需要考虑如下几个点

1. 线程数
2. 超时时间
3. jvm优化

针对上述的优化点来说，首先线程数是一个重点，初始线程数和最大线程数，初始线程数保障启动的时候，如果有大量用户访问，能够很稳定的接受请求， 而最大线程数量用来保证系统的稳定性，而超时时间用来保障连接数不容易被压垮，如果大批量的请求过来，延迟比较高，不容易把线程打满。这种情况在生产中是比较常见的 一旦网络不稳定，宁愿丢包也不愿意把机器压垮。jvm优化一般来说没有太多场景，无非就是加大初始的堆，和最大限制堆,当然也不是无限增大，根据的情况进快速开始<br />在spring boot配置文件中application.yml，添加以下配置
```yaml
server:
  tomcat:
    min-spare-threads: 20
    max-threads: 100
  connection-timeout: 5000
```
这块对tomcat进行了一个优化配置，最大线程数是100，初始化线程是20,超时时间是5000ms
<a name="O8wRG"></a>
## 1、Jvm优化
jvm优化是一个需要场景化的，没有什么太多特定参数，一般来说在server端运行都会指定如下参数 初始内存和最大内存基本会设置成一样的，具体大小根据场景设置，`-server`是一个必须要用的参数， 至于收集器这些使用默认的就可以了，除非有特定需求。
<a name="MDDrj"></a>
### 1.使用-server模式
设置JVM使用server模式。64位JDK默认启动该模式
```bash
java -server -jar springboot-1.0.jar
```
<a name="p132p"></a>
### 2.指定堆参数
这个根据服务器的内存大小，来设置堆参数。<br />-Xms：设置Java堆栈的初始化大小 -Xmx：设置最大的java堆大小
```bash
java -server -Xms512m -Xmx768m  -jar springboot-``1.0``.jar
```
设置初始化堆内存为512MB，最大为768MB。
<a name="xcqyE"></a>
### 3.远程Debug
在服务器上将启动参数修改为：
```bash
java -Djavax.net.debug=ssl -Xdebug -Xnoagent -Djava.compiler=NONE -Xrunjdwp:transport=dt_socket,server=y,suspend=n,address=8888 -jar springboot-1.0.jar
```
这个时候服务端远程Debug模式开启，端口号为8888。<br />在IDEA中，点击Edit Configuration按钮。<br />![2021-08-18-23-37-19-817190.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629301310193-53c57a15-f5bf-4aac-8ed8-43eadc321836.png#clientId=ufae294dd-3537-4&from=ui&id=ue21c9b84&originHeight=678&originWidth=1080&originalType=binary&ratio=1&size=99982&status=done&style=shadow&taskId=u06ab5014-6b64-4286-a127-e6890a4b5fb)<br />出现弹窗，点击+按钮，找到Remote选项。<br />![2021-08-18-23-37-20-121194.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629301310243-820982c6-1c09-463a-9300-7795b8d5f624.png#clientId=ufae294dd-3537-4&from=ui&id=sNSEh&originHeight=678&originWidth=1080&originalType=binary&ratio=1&size=116058&status=done&style=shadow&taskId=uddb10eb0-169c-4657-8ec7-315b2c57bcd)<br />在【1】中填入Remote项目名称，在【2】中填IP地址和端口号，在【3】选择远程调试的项目module，配置完成后点击OK即可<br />如果碰到连接超时的情况，很有可能服务器的防火墙的问题，举例CentOs7,关闭防火墙
```bash
systemctl stop firewalld.service #停止firewall
systemctl disable firewalld.service #禁止firewall开机启动
```
点击debug按钮，IDEA控制台打印信息：<br />![2021-08-18-23-37-20-341304.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629301394035-3abaf4c5-4e72-42db-a16f-f6613a21d956.png#clientId=ufae294dd-3537-4&from=ui&id=ua92d2e58&originHeight=99&originWidth=921&originalType=binary&ratio=1&size=11526&status=done&style=shadow&taskId=u28c99b5f-ad57-4764-9aa9-2d1c8f9270d)<br />说明远程调试成功。
<a name="H6SEy"></a>
## 2、JVM工具远程连接
<a name="CcqEe"></a>
### jconsole与Jvisualvm远程连接
通常的web服务都输部署在服务器上的，在window使用jconsole是很方便的，相对于Linux就有一些麻烦了，需要进行一些设置。
<a name="qs5yU"></a>
### 1.查看hostname,首先使用
```bash
hostname -i
```
查看，服务器的hostname为127.0.0.1，这个是不对的，需要进行修改
<a name="rvpDU"></a>
### 2.修改hostname
修改/etc/hosts文件，将其第一行的“127.0.0.1  localhost.localdomain localhost”，修改为：“192.168.44.128  localhost.localdomain localhost”.“192.168.44.128”为实际的服务器的IP地
<a name="kMsnf"></a>
### 3.重启Linux，在服务器上输入`hostname -i`，查看实际设置的IP地址是否为设置的
<a name="mzlvI"></a>
### 4.启动服务，参数为：
```bash
java -jar -Djava.rmi.server.hostname=192.168.44.128 -Dcom.sun.management.jmxremote -Dcom.sun.management.jmxremote.port=911 -Dcom.sun.management.jmxremote.ssl=false -Dcom.sun.management.jmxremote.authenticate=false jantent-1.0-SNAPSHOT.jar
```
ip为192.168.44.128，端口为911 。
<a name="qkxtV"></a>
### 5.打开Jconsole，进行远程连接,输入IP和端口即可
![2021-08-18-23-37-20-815305.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629301339177-899dcd5c-c2ff-4767-a7cf-a359d09d4855.png#clientId=ufae294dd-3537-4&from=ui&id=u23ccf86a&originHeight=429&originWidth=389&originalType=binary&ratio=1&size=26540&status=done&style=none&taskId=ub39ae1ce-d545-40c9-904d-5cfba7d52f0)<br />点击连接，经过稍稍等待之后，即可完成连接，如下图所示：<br />![2021-08-18-23-37-21-491568.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629301339247-d80f68f1-627b-4c3e-8e69-64799346d6c2.png#clientId=ufae294dd-3537-4&from=ui&id=K50bY&originHeight=750&originWidth=900&originalType=binary&ratio=1&size=63886&status=done&style=none&taskId=uf687a3a5-3837-4eaf-a2c3-d48b15b5a63)<br />同理，JvisualVm的远程连接是同样的，启动参数也是一样.<br />然后在本机JvisualVm输入IP：PORT，即可进行远程连接：如下图所示：<br />![2021-08-18-23-37-21-937800.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629301339334-d27aa293-aa44-4b86-9c00-5f876a91feba.png#clientId=ufae294dd-3537-4&from=ui&id=F1Pxv&originHeight=590&originWidth=900&originalType=binary&ratio=1&size=82148&status=done&style=none&taskId=ub97dae4d-571a-45fd-a4c0-1273703d223)<br />相比较Jvisualvm功能更加强大一下，界面也更美观
