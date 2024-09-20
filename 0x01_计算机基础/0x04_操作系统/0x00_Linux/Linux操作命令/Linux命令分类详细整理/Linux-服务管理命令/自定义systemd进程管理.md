Linux systemd
<a name="je81c"></a>
## 1、systemd介绍
systemd是目前Linux系统上主要的系统守护进程管理工具，由于init一方面对于进程的管理是串行化的，容易出现阻塞情况，另一方面init也仅仅是执行启动脚本，并不能对服务本身进行更多的管理。所以从CentOS 7开始也由systemd取代了init作为默认的系统进程管理工具。<br />systemd所管理的所有系统资源都称作Unit，通过systemd命令集可以方便的对这些Unit进行管理。比如systemctl、hostnamectl、timedatectl、localctl等命令，这些命令虽然改写了init时代用户的命令使用习惯（不再使用chkconfig、service等命令），但确实也提供了很大的便捷性。
<a name="iowtv"></a>
## 2、systemd特点

1. 最新系统都采用systemd管理（RedHat7，CentOS7，Ubuntu15...）
2. CentOS7 支持开机并行启动服务,显著提高开机启动效率
3. CentOS7关机只关闭正在运行的服务，而CentOS6，全部都关闭一次。
4. CentOS7服务的启动与停止不再使用脚本进行管理，也就是/etc/init.d下不在有脚本。
5. CentOS7使用systemd解决原有模式缺陷，比如原有service不会关闭程序产生的子进程。
<a name="TxyZ2"></a>
## 3、systemd语法
```bash
systemctl [command]      [unit]（配置的应用名称）
command可选项
start：启动指定的unit          systemctl start nginx
stop：关闭指定的unit           systemctl stop nginx
restart：重启指定unit          systemctl restart nginx
reload：重载指定unit           systemctl reload nginx
enable：系统开机时自动启动指定unit，前提是配置文件中有相关配置 systemctl enable nginx
disable：开机时不自动运行指定unit   systemctl disable nginx
status：查看指定unit当前运行状态 systemctl status nginx
```
<a name="oAXHQ"></a>
## 4、systemd配置文件说明

- 每一个Unit都需要有一个配置文件用于告知systemd对于服务的管理方式
- 配置文件存放于/usr/lib/systemd/system/，设置开机启动后会在/etc/systemd/system目录建立软链接文件
- 每个Unit的配置文件配置默认后缀名为.service
- 在/usr/lib/systemd/system/目录中分为system和user两个目录，一般将开机不登陆就能运行的程序存在系统服务里，也就是/usr/lib/systemd/system
- 配置文件使用方括号分成了多个部分，并且区分大小写
<a name="d725d887"></a>
## 5、systemd相关文件
| systemd控制的相关文件 | CentOS6 | CentOS7 |
| --- | --- | --- |
| 服务启动的脚本启动路径 | /etc/init.d | /usr/lib/systemd/system |
| 开机自启服务存放路径 | /etc/rcN.d | /etc/systemd/system/multi-user.target.wants/ |
| 默认运行级别配置文件 | /etc/inittab | /etc/systemd/system/default.target |

<a name="Z3W8t"></a>
## 6、systemd操作实践
<a name="ykzgh"></a>
### 实战一：源码编译安装Nginx，实现systemd管理控制
<a name="mUlma"></a>
#### 安装Nginx编译环境
```bash
yum -y install gcc gcc-c++ openssl-devel pcre-devel gd-devel  iproute net-tools telnet wget curl
wget http://nginx.org/download/nginx-1.15.5.tar.gz
tar zxf nginx-1.15.5.tar.gz &&
cd nginx-1.15.5
./configure --prefix=/usr/local/nginx \
    --with-http_ssl_module \
    --with-http_stub_status_module 
make -j 4 && make install
```
<a name="cGRQj"></a>
#### 通用方式启动Nginx
```bash
/usr/local/nginx/sbin/nginx  #启动
/usr/local/nginx/sbin/nginx -s reload  #重启
/usr/local/nginx/sbin/nginx -s quit   #关闭nginx
```
<a name="6BnyA"></a>
#### systemd管理控制启动模式
```bash
vim /usr/lib/systemd/system/nginx.service

[Unit]
Description=nginx
After=network.target
  
[Service]
Type=forking
ExecStart=/usr/local/nginx/sbin/nginx
ExecReload=/usr/local/nginx/sbin/nginx -s reload
ExecStop=/usr/local/nginx/sbin/nginx -s quit
PrivateTmp=true
  
[Install]
WantedBy=multi-user.target
```
<a name="bN4ni"></a>
#### 参数详解
```bash
systemctl restart nginx
systemctl enable nginx
systemctl stop nginx
```
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1614784500617-bc056e0a-c2b1-4860-99e2-0b11b4b55b1e.png#averageHue=%230b0604&height=331&id=oaFVr&originHeight=331&originWidth=824&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=824)<br />如图所示实现了systemd 管理控制nginx服务
<a name="b9c0f26b"></a>
### 实战二：二进制安装tomcat，实现systemd管理控制
<a name="43HVC"></a>
#### 安装java环境
```bash
wget 120.78.77.38/file/jdk-8u231-linux-x64.rpm
wget 120.78.77.38/file/apache-tomcat-9.0.27.tar.gz
```
```bash
rpm -ivh jdk-8u231-linux-x64.rpm    #rpm直接安装jdk
```
<a name="me4Sn"></a>
#### 配置环境变量
```bash
vim /etc/profile
export JAVA_HOME=/usr/java/jdk1.8.0_231-amd64
export JRE_HOME=${JAVA_HOME}/jre  
export CLASSPATH=.:${JAVA_HOME}/lib:${JRE_HOME}/lib  
export JAVA_PATH=${JAVA_HOME}/bin:${JRE_HOME}/bin
export  PATH=${JAVA_HOME}/bin:$PATH
```
```bash
source   /etc/profile
java -version   #检测环境
```
<a name="hZO3e"></a>
#### 安装tomcat
```bash
tar -xf apache-tomcat-9.0.27  
mv apache-tomcat-9.0.27 /usr/local/tomcat
```
<a name="DKdVH"></a>
#### 启动tomcat
```bash
sh /usr/local/tomcat/bin/startup.sh   #启动
sh /usr/local/tomcat/bin/shutdown.sh #关闭
```
<a name="lqCCr"></a>
#### systemd管理控制启动
```bash
vim /usr/lib/systemd/system/tomcat.service

[Unit]
Description=tomcat server
Wants=network-online.target
After=network.target
[Service]
Type=forking
Environment="JAVA_HOME=/usr/java/jdk1.8.0_231-amd64"
Environment="PATH=$JAVA_HOME/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/root/bin"
Environment="CLASSPATH=.:$JAVA_HOME/lib/dt.jar:$JAVA_HOME/lib/tools.jar"
ExecStart=/usr/local/tomcat/bin/startup.sh
ExecStop=/usr/local/tomcat/bin/shutdown.sh
Restart=on-failure
[Install]
WantedBy=multi-user.target
```
```bash
systemctl restart tomcat  #启动
systemctl enable tomcat   #配置自启
systemctl stop tomcat   #停止服务
systemctl status tomcat  #检测状态
```
以上两个实战nginx和tomcat程序中自带了启动停止脚本，如果启动得程序没有自带脚本则需要自己编写一个类似的启动停止脚本
<a name="9lVT1"></a>
### 实战三：部署jar程序，实现systemd管理控制
实际得项目中会有一些jar程序需要启动 如果手动启动则需要输入一大串命令，停止则需要杀掉进程来停止，很麻烦<br />举一个实际启动得例子切换到jar目录下
```bash
java -jar decode.jar -Dconfig=/usr/local/abc/application.properties
```
<a name="EpBmE"></a>
#### 编写一个jar包启动脚本
```bash
vim  demo.sh

#!/bin/bash
#
source /etc/profile
jarName="abc-web.jar"
workDir="/usr/local/abc"
start(){
    cd ${workDir} && java -jar ${jarName} --spring.profiles.active=prod --server.port=9630 >uams.log 2>&1 &
}
stop(){
    ps -ef | grep -qP "(?<=-jar)\s+${jarName}" && kill $(ps -ef | grep -P "(?<=-jar)\s+${jarName}" | awk '{print $2}')
}
case $1 in
    start)
        start
        ;;
    stop)
        stop
        ;;
    restart)
        stop
        start
        ;;
esac
```
<a name="bLfDY"></a>
#### 编写 systemd配置文件
```bash
vim /usr/lib/systemd/system/abc.service

[Unit]
Description=uams server
Wants=network-online.target
After=network.target
[Service]
Type=forking
WorkingDirectory=/usr/local/abc/
ExecStart=/bin/bash uams.sh start
ExecStop=/bin/bash uams.sh stop
ExecReload=/bin/bash uams.sh restart
Restart=on-failure
[Install]
WantedBy=multi-user.target
```
<a name="Fjrny"></a>
#### systemd管理jar包服务
```
systemctl restart abc  #启动
systemctl enable abc   #配置自启
systemctl stop abc   #停止服务
systemctl status abc  #检测状态
```

