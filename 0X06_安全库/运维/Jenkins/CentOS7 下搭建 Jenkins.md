CentOS7 Jenkins
<a name="aec807aa"></a>
### Jenkins简介
Jenkins 提供了软件开发的持续集成服务，运行在 Servlet 容器中(例如 Apache Tomcat )。它支持软件配置管理( SCM )工具(包括 AccuRev SCM、CVS、Subversion、Git、Perforce、Clearcase 和 RTC )，可以执行基于 Apache Ant 和 Apache Maven 的项目，以及任意的 Shell 脚本和 Windows 批处理命令<br />Jenkins 的存在是为了简化开发流程，比如往 git 上提交了代码， Jenkins 就会自动拉取最新的代码进行部署
<a name="E3QCd"></a>
### CentOS7 下 Jenkins 搭建过程
:::danger
Jenkins 需要 jdk 环境
:::
安装完 jdk 环境之后，就可以准备安装 Jenkins ，使用以下命令进行安装即可（#后面为注释内容）：
```bash
#下载Jenkins库
wget -O /etc/yum.repos.d/jenkins.repo https://pkg.jenkins.io/redhat-stable/jenkins.repo 
#导入key
rpm --import https://pkg.jenkins.io/redhat-stable/jenkins.io.key
#安装Jenkins
yum install -y jenkins
#启动Jenkins服务
systemctl start jenkins
```
到这里， CentOS 下 Jenkins 搭建便是完成了<br />此时可以通过 ip:port 的方式，访问到 Jenkins<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1603696080336-f857c477-b636-442e-b8d9-040ed670214d.png#align=left&display=inline&height=685&originHeight=2056&originWidth=3840&size=489340&status=done&style=shadow&width=1280)<br />点击Continue之后，然后根据自己的需求，进行安装即可。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1603696220523-bc15dcff-c8cb-47ec-afbd-0278acb306a7.png#align=left&display=inline&height=687&originHeight=2060&originWidth=3840&size=691353&status=done&style=shadow&width=1280)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1603696578690-a23ef2a4-2716-4e59-91d5-45e3b1244156.png#align=left&display=inline&height=686&originHeight=2057&originWidth=3840&size=558691&status=done&style=shadow&width=1280)<br />到此， CentOS 下 Jenkins 搭建便是完成了
<a name="b07e7c0e"></a>
### 可能遇到的问题
<a name="CKPuz"></a>
#### 1、Starting jenkins (via systemctl): Job for jenkins.service failed because the control process exited with error code.
运行命令: `systemctl status jenkins.service` 查看错误详细信息<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1603677278569-39bdc3cf-c5e3-4ddb-ae2a-655d46fe4487.png#align=left&display=inline&height=361&originHeight=1082&originWidth=3323&size=1425913&status=done&style=none&width=1107.6666666666667)<br />请注意图中标注出来的两个地方，第一个地方是 Jenkins 加载的路径，第二个地方是 Jenkins 的错误详细信息： `Failed to start LSB: Jenkins Automation Server`<br />这是因为 Jenkins 未加载到 java 环境的问题，直接修改 Jenkins 的启动文件，并在 candiddates 参数内追加 java 的环境变量即可<br />Jenkins的启动文件，在图中第一个地方已经做了标注，所以运行以下命令：`vim /etc/rc.d/init.d/jenkins`<br />具体修改见下图:<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1603677736837-295d1cd4-dcfa-4187-8fae-8c8c42d8add3.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2272586&status=done&style=none&width=1107.6666666666667)<br />关于 java 环境变量这部分，因为每个人的配置不同，所以需要根据自己的实际情况做修改。修改完之后再做以下步骤即可（#后内容为注释内容）：
```bash
#重新加载修改的配置文件
systemctl daemon-reload
#重新启动Jenkins服务
systemctl restart jenkins.service
#查看Jenkins服务,可以看到服务已经起来了
systemctl status jenkins.service
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1603677774232-38f0295b-aa17-43f7-b68a-7bc694e763a5.png#align=left&display=inline&height=270&originHeight=810&originWidth=3323&size=1041756&status=done&style=none&width=1107.6666666666667)
<a name="RGtHk"></a>
#### 2、在 CentOS 环境下， Jenkins 已经安装好了，但是在外部访问不到。这可能是因为防火墙的问题
出于安全的考虑，不建议直接将防火墙关掉的。开启 Jenkins 需要的端口即可（以开启 8080 端口为例，具体可根据自己需求更改）：
```bash
开端口命令：firewall-cmd --zone=public --add-port=8080/tcp --permanent
重启防火墙：systemctl restart firewalld.service
命令含义：
--zone #作用域
--add-port=80/tcp  #添加端口，格式为：端口/通讯协议
--permanent   #永久生效，没有此参数重启后失效
```
因为 Jenkins 默认端口是 8080 ，可能会导致端口冲突。修改 Jenkins 的默认端口即可： 
```bash
vim /etc/sysconfig/jenkins
```
在该配置文件中，可以看到 JENKINS_PORT 这一项，根据需求修改即可
