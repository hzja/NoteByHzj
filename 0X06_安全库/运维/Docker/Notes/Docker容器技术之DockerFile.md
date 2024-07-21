Docker DockerFile
<a name="Uo22g"></a>
## Docker file简单介绍
Docker可以使用Dockerfile的内容来自动构建镜像。Dockerfile也是一个文件，其中有创建镜像、运行指令等一系列的命令，且每行只支持一个运行命令。
<a name="zeZAR"></a>
### Docker file分为四部分组成

- 基础镜像信
- 维护者信息
- 镜像操作指令
- 容器启动时执行指令

dockerfile指令忽略大小写，建议大写，`#`作为注释，每行只支持一条指令，指令可以带多个参数。
<a name="gsRMo"></a>
### dockerfile指令有

- 构建指令：用于构建image，其指定的操作不会在运行image的容器中执行。
- 设置指令：用于设置image的属性，其指定的操作会在运行image的容器中执行。
<a name="FlpjD"></a>
## Dockerfile指令
Dockerfile指令一共有以下种：
<a name="QCvnY"></a>
### 1、FROM
用来指定基础镜像，然后通过在基础镜像上构建新的镜像，基础镜像一般有远程或本地仓库。并且Dockerfile文件第一行必须的`FROM`指令，如果一个Dockerfile需要创建多个镜像，可以使用多个`FROM`指令。
```dockerfile
#具体使用用法如下：
FROM < image_name >   #默认是latest版本
FROM <image:version>  #指定版本
```
<a name="W7YfI"></a>
### 2、MAINTAINER
指定镜像的创建者信息
```dockerfile
#具体使用法如下：
MAINTAINER < name >
```
<a name="PeWZY"></a>
### 3、RUN
运行所有基础镜像能支持的命令，同样也可以使用多条`RUN`指令，可以使用`\`来换行
```dockerfile
#具体使用法如下：
RUN < command >
RUN ["executable", "param1", "param2" ... ] (exec form) 

```
<a name="Th4FO"></a>
### 4、CMD
用于容器启动时的指定操作，它可以是命令，也可以是脚本，但只执行一次，如果有多少默认只会执行最后一个。
```dockerfile
#具体使用法如下：
CMD [“executable” ,”Param1”, “param2”]使用exec执行，推荐 
CMD command param1 param2，在/bin/sh上执行 
CMD [“Param1”, “param2”] 提供给ENTRYPOINT做默认参数。
```
<a name="A8vkK"></a>
### 5、EXPOSE
指定容器的端口映射（容器与物理机），运行容器时加上`-p`参数指定`EXPOSE`设置的端口。`EXPOSE`可以设置多个端口号，相应地运行容器配套多次使用`-p`参数。可以通过`docker port` +容器需要映射的端口号和容器ID来参考宿主机的映射端口。
```dockerfile
#具体使用法如下：
EXPOSE <port> [port1 , port2 ............]
```
<a name="MoeXe"></a>
### 6、ENV
在镜像中用于设置环境变量的，然后RUN命令可以使用此设置的环境变量，在容器启动后也以通过`docker inspect`查看环境变量，可以通过`docker run --env key=value`来设置或修改环境变量。
```dockerfile
#具体使用法如下：
ENV <key> <value>
ENV JAVA_HOME /usr/local/jdk
```
<a name="Ae2l2"></a>
### 7、ADD
复制指定的源文件、目录、URL到容器的指定目录中。所有拷贝到container中的文件和文件夹权限为0755，uid和gid为0。

- 如果源是一个目录，那么会将该目录下的所有文件添加到container中，不包括目录；
- 如果源文件是可识别的压缩格式，则docker会帮忙解压缩（注意压缩格式）；
- 如果源是文件且目标目录中不使用斜杠结束，则会将目标目录视为文件，源的内容会写入目标目录；
- 如果源是文件且目标目录中使用斜杠结束，则会源文件拷贝到目标目录下。
```dockerfile
#具体使用法如下：
ADD <源> <目标>
```
<a name="vCnud"></a>
### 8、COPY
复制本地主机的源（默认为Dockerfile所在的目录）到容器中的目标中，目标路径不存在时会自动创建。
```dockerfile
#具体使用法如下：
COPY <源> <目标>
COPY web/index.html  /var/web/
```

- 路径必须是绝对路径，如果不存在，会自动创建对应目录
- 路径必须是Dockerfile 所在路径的相对路径
- 如果是一个目录，只会复制目录下的内容，而目录本身则不会被复制
<a name="JJDGh"></a>
### 9、ENTRYPOINT
指定容器启动后执行的命令，多行只执行最后一行。并且不可被`docker run`提供的参数覆盖。
```dockerfile
#具体使用法如下：
ENTRYPOINT "command" "param1" "param2"
```
<a name="NV7Nh"></a>
### 10、VOLUME
创建一个可以从本地主机或其它容器挂载的挂载点，一般用于存放数据。与`docker run -v`也可以实现此功能。
```dockerfile
#具体使用法如下：
VOLUME  [directory_name]
VOLUME /docker_data
```
<a name="AGZtW"></a>
### 11、USER
指定容器运行时使用的用户或`UID`，后面`RUN`、`CMD`、`ENTRYPIONT`都会使用此用户来运行命令。
```dockerfile
#具体使用法如下：
USER [username/uid]
```
<a name="WXsf3"></a>
### 12、WORKDIR
指定`RUN`、`CMD`、`ENTRYPIONT`指定的命令的运行目录。可以使用多个`WORKDIR`指令，后续参数如果是相对路径，则会基于之前的命令指定的路径。如：`WORKDIR /data WORKDIR work`。最终的路径就是/data/work。path路径也可以是环境变量。
```dockerfile
#具体使用方法如下：
WORKDIR [path]
```
<a name="K54M0"></a>
### 13、ONBUILD
配置当前所创建的镜像作为其它新创建镜像的基础镜像时，所执行的操作指令。就是，这个镜像创建后，如果其它镜像以这个镜像为基础，会先执行这个镜像的`ONBUILD`命令。
```dockerfile
#具体使用法如下：
ONBUILD [INSTRUCTION]
```
<a name="bzVfU"></a>
## 通过Dockerfile快速构建镜像
接下来，通过构建一个Tomcat镜像，来演示Dockerfile的使用方法，前提是安装Docker环境：
```bash
[root@master tomcat]# ll
总用量 190504
-rw-r--r-- 1 root root   9552281 6月   7 15:07 apache-tomcat-8.5.31.tar.gz
-rw-r--r-- 1 root root        32 7月   3 09:41 index.jsp
-rw-r--r-- 1 root root 185515842 9月  20 2017 jdk-8u144-linux-x64.tar.gz
[root@master tomcat]# cat index.jsp 
welcome to mingongge's web site
[root@master tomcat]# pwd
/root/docker/tomcat
[root@master tomcat]# vim Dockerfile
#config file start#
FROM centos
MAINTAINER mingongge <Tomcat>

#add jdk and tomcat software
ADD jdk-8u144-linux-x64.tar.gz /usr/local/
ADD apache-tomcat-8.5.31.tar.gz /usr/local/
ADD index.jsp /usr/local/apache-tomcat-8.5.31/webapps/ROOT/

#config java and tomcat ENV
ENV JAVA_HOME /usr/local/jdk1.8.0_144
ENV CLASSPATH $JAVA_HOME/lib/dt.jar:$JAVA_HOME/lib/tools.jar
ENV CATALINA_HOME /usr/local/apache-tomcat-8.5.31/
ENV PATH $PATH:$JAVA_HOME/bin:$CATALINA_HOME/bin

#config listen port of tomcat
EXPOSE 8080

#config startup command of tomcat
CMD /usr/local/apache-tomcat-8.5.31/bin/catalina.sh run

#end of config-file#
```
<a name="x5kp6"></a>
### 构建过程
```bash
[root@master tomcat]# docker build -t tomcat-web .  #这个.不用注释了吧相信懂的人自然懂的
Sending build context to Docker daemon 195.1 MB
Step 1/11 : FROM centos
 ---> 49f7960eb7e4
Step 2/11 : MAINTAINER mingongge <Tomcat>
 ---> Running in afac1e218299
 ---> a404621fac22
Removing intermediate container afac1e218299
Step 3/11 : ADD jdk-8u144-linux-x64.tar.gz /usr/local/
 ---> 4e22dafc2f76
Removing intermediate container b1b23c6f202a
Step 4/11 : ADD apache-tomcat-8.5.31.tar.gz /usr/local/
 ---> 1efe59301d59
Removing intermediate container aa78d5441a0a
Step 5/11 : ADD index.jsp /usr/local/apache-tomcat-8.5.31/webapps/ROOT/
 ---> f09236522370
Removing intermediate container eb54e6eb963a
Step 6/11 : ENV JAVA_HOME /usr/local/jdk1.8.0_144
 ---> Running in 3aa91b03d2d1
 ---> b497c5482fe0
Removing intermediate container 3aa91b03d2d1
Step 7/11 : ENV CLASSPATH $JAVA_HOME/lib/dt.jar:$JAVA_HOME/lib/tools.jar
 ---> Running in f2649b5069be
 ---> 9cedb218a8df
Removing intermediate container f2649b5069be
Step 8/11 : ENV CATALINA_HOME /usr/local/apache-tomcat-8.5.31/
 ---> Running in 39ef620232d9
 ---> ccab256164fe
Removing intermediate container 39ef620232d9
Step 9/11 : ENV PATH $PATH:$JAVA_HOME/bin:$CATALINA_HOME/bin
 ---> Running in a58944d03d4a
 ---> f57de761a759
Removing intermediate container a58944d03d4a
Step 10/11 : EXPOSE 8080
 ---> Running in 30681437d265
 ---> b906dcc26584
Removing intermediate container 30681437d265
Step 11/11 : CMD /usr/local/apache-tomcat-8.5.31/bin/catalina.sh run
 ---> Running in 437790cc642a
 ---> 95204158ee68
Removing intermediate container 437790cc642a
Successfully built 95204158ee68
```
<a name="ToSKV"></a>
### 通过构建的镜像启动容器
```bash
[root@master tomcat]# docker run -d -p 8080:8080 tomcat-web
b5b65bee5aedea2f48edb276c543c15c913166bf489088678c5a44fe9769ef45
[root@master tomcat]# docker ps
CONTAINER ID   IMAGE        COMMAND                  CREATED        STATUS         PORTS                    NAMES
b5b65bee5aed   tomcat-web   "/bin/sh -c '/usr/..."   5 seconds ago  Up 4 seconds   0.0.0.0:8080->8080/tcp   vigilant_heisenberg
```
<a name="v9Kws"></a>
### 访问容器
浏览器输入http://server-ip:8080
