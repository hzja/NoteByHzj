Docker 镜像
<a name="oDewE"></a>
## Docker镜像简述
镜像是一种轻量级、可执行的独立软件包，用来打包软件运行环境和基于运行环境开发的软件，它包含运行某个软件所需的所有内容，包括代码、运行时库、环境变量和配置文件
<a name="rCk6F"></a>
## Docker镜像加载原理
<a name="ySvmQ"></a>
### UnionFs：联合文件系统
UnionFs(联合文件系统)：Union文件系统(UnionFs)是一种分层、轻量级并且高性能的文件系统，它支持对文件系统的修改作为一次提交来一层层的叠加，同时可以将不同目录挂载到同一个虚拟文件系统下，UnionFs联合文件系统是Docker镜像的基础，镜像可以通过分层来进行继承，基于基础镜像（没有父镜像），可以制作各种具体的应用镜像 特性：一次同时加载多个文件系统，但从外面看起来，只能看到一个文件系统，联合加载会把各层文件系统叠加起来，这样最终的文件系统会包含所有底层的文件和目录。
<a name="GLKhd"></a>
### Docker镜像加载原理
Docker的镜像实际上由一层一层的UnionFs文件系统组成 bootfs：主要包含 bootloader和 Kernel，bootloader主要是引导加 kernel，Linux刚启动时会加bootfs文件系统，在 Docker镜像的最底层是bootfs，这一层与典型的Linux/Unix系统是一样的，包含bootfs加载器和内核，当bootfs加载完成之后整个内核就都在内存中了，此时内存的使用权已由 bootfs转交给内核，此时系统也会卸载bootfs。<br />rootfs：在 bootfs之上，包含的就是典型 Linux系统中的/dev、/proc、/bin、/etc等标准目录和文件，rootfs就是各种不同的操作系统发行版，比如：Ubuntu、CentOS等等<br />简单理解：

1. _对于Docker安装OS来说：就是Docker使用了Linux本身的bootfs，只需要安装自己所需的rootfs。_
2. _对于Docker安装普通镜像来说：就是Docker本身是分层下载镜像，所以可以提取出公共层镜像，进行复用。_
<a name="K8wdz"></a>
## Docker镜像的特点
Docker镜像都是只读的，当容器启动时，一个新的可写层加载到镜像的顶部<br />这一层就是通常说的容器层，容器之下的都叫镜像层<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597567940384-209a1782-60ff-490f-aa4c-95db11f2d28d.png#height=225&id=m6eB0&originHeight=449&originWidth=1676&originalType=binary&ratio=1&rotation=0&showTitle=false&size=68330&status=done&style=shadow&title=&width=838)
<a name="dEhpI"></a>
## Commit镜像
```bash
# 提交本地镜像 
# -a：作者信息 -m：描述信息 容器ID 镜像名称:版本信息
docker commit -a="test" -m="test" 容器id tomcat01:1.0
```
<a name="Ubgji"></a>
## Docker容器数据卷
<a name="Vnr5b"></a>
### 容器数据卷
容器之间可以有一个数据共享的技术！Docker容器中产生的数据，同步到本地！<br />这就是卷技术！目录的挂载，将容器内的目录，挂载到Linux上面！
<a name="o1Owz"></a>
### 容器数据卷使用命令
```bash
# 命令
docker run -it -v 主机目录:容器内目录 /bin/bash
```
<a name="SsrXm"></a>
### 挂载MySQL数据库到Liunx宿主机
```bash
# 1. 下载MySQL
docker pull mysql
# 2. 启动并挂载 -e：特别注意需要设置密码
docker run -d -p 3306:3306 -v /home/conf:/etc/mysql/conf.d -v /home/logs:/logs -v /home/data:/var/lib/mysql -e MYSQL_ROOT_PASSWORD=123456 mysql
```
此时使用远程连接报错<br />![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1568621447565-781ca372-7e0f-44a5-8d09-704681bb9caf.png#height=850&id=o7lv6&originHeight=1699&originWidth=2441&originalType=binary&ratio=1&rotation=0&showTitle=false&size=330806&status=done&style=none&title=&width=1220.5)
```bash
# 解决报错
# 1. 进入容器内
docker exec -it 容器ID /bin/bash
# 2. 进入MySQL
mysql -uroot -p123456
# 3. 授权
mysql> GRANT ALL ON *.* TO 'root'@'%';
# 4. 刷新权限：
mysql> flush privileges;
# 5. 更新加密规则：
mysql> ALTER USER 'root'@'localhost' IDENTIFIED BY 'password' PASSWORD EXPIRE NEVER;
# 6. 更新root用户密码：
mysql> ALTER USER 'root'@'%' IDENTIFIED WITH mysql_native_password BY '123456';
# 7. 刷新权限：
mysql> flush privileges;
```
<a name="UsidV"></a>
### 具名和匿名挂载
```basic
-v 容器内路径          # 匿名挂载
-v 卷名:容器内路径      # 具名挂载
-v 宿主机路径:容器内路径 # 指定路径挂载
Docker容器内的卷，在没有指定目录的情况下都在/var/lib/docker/volumes/xxx/_data下
```
<a name="shaTu"></a>
### 拓展：绑定权限
```bash
# 通过 -v 容器内路径：ro rw 改变读写权限
ro # readonly 只读
rw # readwrite 可读可写
docker run -d nginx01 -v nginxdemo:/etc/nginx:ro nginx
docker run -d nginx01 -v nginxdemo:/etc/nginx:rw nginx
# ro：只要看到ro就说明这个路径只能通过宿主机来操作，容器内部是无法操作
```
<a name="I5WIb"></a>
### 数据卷容器挂载
目的：实现多个容器间的数据同步(多个MySQL间共享数据)
```bash
# 使用 --volumes-from 容器名称 此命令实现数据卷容器挂载
docker run -d --name mysql02 -p 3345:3306 --volumes-from mysql01 -e MYSQL_ROOT_PASSWORD=123456 mysql
```
<a name="UaauE"></a>
## DockerFile
`DockerFile`是用来构建Docker镜像的文件(命令参数脚本)
<a name="BPoIt"></a>
### DockerFile构建指令
```dockerfile
FROM                # 基础镜像，一切从这里开始构建
MAINTAINER          # 镜像的作者， 姓名+邮箱
RUN                 # 镜像构建的时候需要运行的命令
ADD                 # 步骤，tomcat镜像，这个tomcat压缩包！添加内容 添加同目录
WORKDIR             # 镜像的工作目录
VOLUME              # 挂载的目录
EXPOSE              # 保留端口配置
CMD                 # 指定这个容器启动的时候要运行的命令，只有最后一个会生效，可被替代
ENTRYPOINT          # 指定这个容器启动的时候要运行的命令，可以追加命令
COPY                # 类似ADD，将文件拷贝到镜像中
ENV                 # 构建的时候设置环境变量！
```
<a name="wkYVm"></a>
### 创建一个自己的CentOS镜像
<a name="B0ZtP"></a>
#### A、创建DockerFile
```bash
vim mycentos
```
<a name="cdWYy"></a>
#### B、编写DockerFile
```dockerfile
FROM centos
MAINTAINER Fcant<fcscanf@outlook.com>
ENV MYPATH /usr/local
WORKEDIR $MYPATH
RUN yum -y install vim
EXPOSE 80
CMD /bin/bash
```
<a name="iozxN"></a>
#### C、构建自己的镜像
```bash
docker build -f mycentos -t mycentosdemodo:1.0 .
```
<a name="AN2ej"></a>
#### D、查看镜像生成历史
```bash
docker history 镜像ID
```
<a name="GVmpA"></a>
## 创建Tomcat镜像
<a name="R4xKH"></a>
### 1、准备镜像文件：Tomcat和JDK的压缩包
<a name="Gh0Ay"></a>
### 2、编写DockerFile（文件名为Dockerfile）
注意ADD命令后面的文件可以从绝对路径或者相对路径读取，如果无路径则表明从当前目录读取添加的文件
```dockerfile
FROM centos
MAINTAINER Fcant<fcscanf@outlook.com>
COPY README.txt /local/dev/README.txt
ADD jdk-8u161-linux-x64.tar.gz /local/dev/java
ADD apache-tomcat-9.0.37.tar.gz /local/dev/tomcat
RUN yum -y install vim
ENV MYPATH /usr/local
WORKDIR $MYPATH
ENV JAVA_HOME /usr/local/jdk1.8.0_161
ENV CLASSPATH $JAVA_HOME/lib/dt.jar:$JAVA_HOME/lib/tools.jar
ENV CATALINA_HOME /usr/local/apache-tomcat-9.0.37
ENV CATALINA_BASH /usr/local/apache-toacat-9.0.37
ENV PATH $PATH:$JAVA_HOME/bin:$CATALINA_HOME/lib:$CATALINA_HOME/bin
EXPOSE 8080
CMD ["/usr/local/apache-tomcat-9.0.37/bin/catalina.sh", "run"]
```
<a name="v75pH"></a>
### 3、打包镜像
```bash
# 这里使用的是Dockerfile文件来构建的脚本，所以可以省略-f参数对Dockerfile文件进行指定
docker build -t tomcat .
```
打包执行步骤<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598328694839-6118be63-19d5-4093-80cc-a6f67235c979.png#height=583&id=fEVWj&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2088522&status=done&style=none&title=&width=1107.6666666666667)![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598328746662-c9fb0f0b-d439-4b8c-89de-e000bdf9b781.png#height=583&id=oU1es&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2144934&status=done&style=none&title=&width=1107.6666666666667)
<a name="Loo9z"></a>
### 4、启动镜像
```bash
docker run -d -p 8080:8080 --name mttomcat -v /home/fortuneteller/tomcat/test:/usr/local/apache-tomcat-9.0.37/webapps/test -v /home/fortuneteller/tomcat/logs:/usr/local/apache-tomcat-9.0.37/logs mytomcat
```
<a name="v0ofH"></a>
### 5、测试运行
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598346956092-b3cc5286-e0ba-4cba-9d0b-51e20f91969a.png#height=687&id=UoSZU&originHeight=2060&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=932955&status=done&style=shadow&title=&width=1280)
<a name="PjScQ"></a>
### 6、在宿主机的`/home/fortuneteller/tomcat/test`目录下创建`WEB-INF`目录与`index.html`
```html
<!DOCCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>Tomcat From Docker</title>
  </head>
  <body>
    <h2>Hello, I'm Form Docker</h2>
  </body>
</html>
```
<a name="Ne8ef"></a>
### 7、在`WEB-INF`目录下编写`web.xml`文件
```xml
<?xml version="1.0" encoding="UTF-8"?>
<web-app xmlns="http://xmlns.jcp.org/xml/ns/javaee"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://xmlns.jcp.org/xml/ns/javaee http://xmlns.jcp.org/xml/ns/javaee/web-app_3_1.xsd"
         version="3.1">
    <display-name>test</display-name>
</web-app>
```
<a name="em5CZ"></a>
### 8、测试访问
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598348609148-334b295f-441e-4f85-b366-0a64506ec647.png#height=684&id=pqxaU&originHeight=2053&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=283310&status=done&style=shadow&title=&width=1280)
<a name="AqzFj"></a>
## 本地镜像上传阿里云

1. 登陆阿里云容器镜像服务
2. 创建命名空间

![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598349114987-9ad665aa-4809-4ec9-87c3-350279689442.png#height=686&id=qAHhG&originHeight=2057&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=545108&status=done&style=shadow&title=&width=1280)

3. 创建仓库

![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598349092017-a64298d2-e307-4d88-8356-98d62e2b59d0.png#height=686&id=DmOsJ&originHeight=2059&originWidth=3836&originalType=binary&ratio=1&rotation=0&showTitle=false&size=619836&status=done&style=shadow&title=&width=1278.6666666666667)

4. 设置固定密码

![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597998745049-b8d8a9d9-42a8-4b94-bb90-2e7d48f8f910.png#height=601&id=gkOi2&originHeight=1804&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=492985&status=done&style=shadow&title=&width=1280)

5. 查看自己的推送命令

![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598349266107-9be2a59a-d99a-4b27-a260-aad3bfb24414.png#height=687&id=ZAMVq&originHeight=2060&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=645601&status=done&style=shadow&title=&width=1280)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598349307548-3514e662-7570-4bfb-b89b-b2c91b492490.png#height=687&id=J1L98&originHeight=2060&originWidth=3798&originalType=binary&ratio=1&rotation=0&showTitle=false&size=749725&status=done&style=shadow&title=&width=1266)

6. 使用推送命令完成上传

![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598349516951-1e6f161b-e4db-4a36-baf2-ae38502280f3.png#height=241&id=Gxire&originHeight=722&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=927257&status=done&style=none&title=&width=1107.6666666666667)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598355593216-814bd9fd-9fc3-4523-aedd-6029c09a98a8.png#height=600&id=akTvI&originHeight=1801&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=403791&status=done&style=shadow&title=&width=1280)

7. 拉取时同理根据阿里云提示命令完成即可
<a name="OTnL9"></a>
## Docker总结
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598370600722-3d9b095f-bafd-4178-95d4-9610a541874b.png#height=501&id=eB5dP&originHeight=1502&originWidth=1865&originalType=binary&ratio=1&rotation=0&showTitle=false&size=400985&status=done&style=shadow&title=&width=621.6666666666666)
<a name="Egvs5"></a>
## Docker部署SpringBoot项目

1. 使用Maven构建获得jar包
2. 编写Dockerfile
```dockerfile
FROM java:8
COPY *.jar /app.jar
CMD ["--server.port=8080"]
EXPOSE 8080
ENTRYPOINT ["java", "jar", "app.jar"]
```

3. 上传Docker与jar包到Linux服务器
4. 使用命令获得镜像
```bash
docker build -t ideatest .
```

5. 使用命令查看获取到镜像编号
```bash
docker images
```

6. 启动镜像
```bash
docker run -d -p 3344:8080 ideatest  # 3344端口需要提前在阿里云放开
```

7. 测试访问

**在浏览器输入：服务器ip:3344**
