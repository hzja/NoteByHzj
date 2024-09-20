JavaSpringBootDocker
<a name="afuo7"></a>
## **一、容器化部署的好处**
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690777499146-a68cd758-87ae-42ad-a17e-4993dc7af914.png#averageHue=%231a3421&clientId=u4c82a5ba-20bc-4&from=paste&id=u7502767a&originHeight=331&originWidth=740&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u450e82b2-e73e-4fdd-8cf8-dec60157f1b&title=)<br />Docker 作为一种新兴的虚拟化方式，它可以更高效的利用系统资源，不需要进行硬件虚拟以及运行完整操作系统等额外开销。<br />传统的虚拟机技术启动应用服务往往需要数分钟，而 Docker 容器应用，由于直接运行宿主内核，无需启动完整的操作系统，因此可以做到秒级、甚至毫秒级的启动时间。大大的节约了开发、测试、部署的时间。<br />最重要的是一致的运行环境。Docker 的镜像提供了除内核外完整的运行时环境，确保了应用运行环境一致性。从而达到真正意义上的 一次构建，到处执行。
<a name="TGNA1"></a>
## **二、构建镜像**
<a name="PE23q"></a>
### 2.1、Dockerfile
开始使用 Dockerfile 来定制镜像，从而达到容器化的目的。Dockerfile 的本质就是将一系列修改、安装、构建、操作的命令整合到一起，去构建一个个性化的镜像，以达到**一次构建，到处运行。**接下来就来构建自定义镜像。<br />在my-project-server模块下创建docker目录，在docker目录下创建Dockerfile。代码如下：
```dockerfile
FROM openjdk:8-jre

MAINTAINER Micromaple <micromaple@qq.com>

RUN mkdir /app

COPY my-project-server-1.0.0-SNAPSHOT.jar /app/app.jar

ENTRYPOINT ["java", "-Djava.security.egd=file:/dev/./urandom", "-jar", "/app/app.jar", "--spring.profiles.active=prod,druid-prod"]

EXPOSE 8899
```

- FROM：指定基础镜像，项目是使用jdk8开发的项目，所以指定的基础镜像为openjdk:8-jre
- MAINTAINER：项目维护人员
- RUN：执行命令，在根目录下创建app目录
- COPY：将宿主机当前目录下的my-project-server-1.0.0-SNAPSHOT.jar文件拷贝到app目录下，并重命名为app.jar
- ENTRYPOINT：指定容器启动程序及参数
- EXPOSE：指定运行时容器提供服务的端口
<a name="X74sk"></a>
### 2.2、开始构建
在虚拟机创建目录
```bash
mkdir -p /usr/local/docker/my-project/docker
```
将打包后的my-project-server-1.0.0-SNAPSHOT.jar文件和Dockerfile文件上传至该目录。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690777499168-fcad2189-f2ac-4166-944e-1f8183a79cf7.png#averageHue=%230c0704&clientId=u4c82a5ba-20bc-4&from=paste&id=uecd08fec&originHeight=94&originWidth=585&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u69270ad1-87c7-43c9-a5e5-f5fc7b39a8b&title=)<br />执行镜像构建命令
```bash
docker build -t my-project-server:v1 .
```
构建成功
```
$ docker images
REPOSITORY          TAG                 IMAGE ID            CREATED             SIZE
my-project-server   v1                  ed30386b06d2        11 seconds ago      334MB
openjdk             8-jre               26ac3f63d29f        9 months ago        273MB
```
<a name="WsOXw"></a>
## **三、搭建私服**
<a name="m0umJ"></a>
### 3.1、搭建 Docker Registry
创建目录
```bash
mkdir -p /usr/local/docker/registry
```
在该目录下创建Docker Registry 的 docker-compose.yml。内容如下：
```bash
cd /usr/local/docker/registry
```
```yaml
version: '3.1'
services:
  registry:
    image: registry
    restart: always
    container_name: registry
    ports:
      - 5000:5000
    volumes:
      - ./data:/var/lib/registry
```
启动容器
```bash
docker-compose up -d
```
<a name="FYc6T"></a>
### 3.2、搭建 Docker Registry WebUI
创建目录
```bash
mkdir -p /usr/local/docker/docker-registry-frontend
```
在该目录下创建Docker Registry 的 docker-compose.yml。内容如下：
```bash
cd /usr/local/docker/docker-registry-frontend
```
```yaml
version: '3.1'
services:
  frontend:
    image: konradkleine/docker-registry-frontend:v2
    ports:
      - 8080:80
    volumes:
      - ./certs/frontend.crt:/etc/apache2/server.crt:ro
      - ./certs/frontend.key:/etc/apache2/server.key:ro
    environment:
      - ENV_DOCKER_REGISTRY_HOST=192.168.110.158(Docker仓库的IP)
      - ENV_DOCKER_REGISTRY_PORT=5000
```
需要将`ENV_DOCKER_REGISTRY_HOST`的值更改为自己搭建的 `DockerRegistry` 服务的IP<br />启动容器
```bash
docker-compose up -d
```
浏览器访问http://192.168.110.158:8080/，效果如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690777499205-f857d11a-ada1-41aa-9052-dad489114c0d.png#averageHue=%23fdfdfd&clientId=u4c82a5ba-20bc-4&from=paste&id=u428f46bf&originHeight=641&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7db43b86-7f66-45f7-82a3-3a328670c62&title=)
<a name="S40o6"></a>
### 3.3、客户端配置
在需要上传 Docker 镜像的客户端需配置daemon.json，完整路径在 /etc/docker/daemon.json
```bash
vi /etc/docker/daemon.json
```
需增加如下内容：
```json
"insecure-registries": [
  "Docker仓库的IP:5000"
]
```
完整daemon.json内容：
```json
{
  "registry-mirrors": [
    "https://xxx.mirror.aliyuncs.com"
  ],
  "insecure-registries": [
    "192.168.110.158:5000"
  ]
}
```

- registry-mirrors：为镜像加速地址，这里为自己申请的加速地址。大家可以自己申请一个，也可以在网上找一个。
- insecure-registries：Docker仓库的IP。

重新启动服务
```bash
systemctl daemon-reload
systemctl restart docker
```
<a name="LIOIQ"></a>
### 3.4、上传私服
搭建以及配置完成后，需要将之前构建的镜像上传至自己的Docker仓库。<br />查看镜像
```bash
$ docker images
REPOSITORY          TAG                 IMAGE ID            CREATED             SIZE
my-project-server   v1                  6af7d633afb7        5 seconds ago       334MB
openjdk             8-jre               26ac3f63d29f        9 months ago        273MB
```
<a name="lk12j"></a>
#### 1)、镜像标记
使用 docker tag 将 my-project-server:v1 这个镜像标记为 192.168.110.158:5000/my-project-server:v1。<br />192.168.110.158为私有Docker仓库的IP。<br />格式为：`docker tag IMAGE[:TAG] [REGISTRY_HOST[:REGISTRY_PORT]/]REPOSITORY[:TAG]`<br />命令如下：
```bash
docker tag my-project-server:v1 192.168.110.158:5000/my-project-server:v1
```
标记后，查看镜像
```bash
$ docker images
REPOSITORY                               TAG                 IMAGE ID            CREATED             SIZE
192.168.110.158:5000/my-project-server   v1                  6af7d633afb7        3 minutes ago       334MB
my-project-server                        v1                  6af7d633afb7        3 minutes ago       334MB
openjdk                                  8-jre               26ac3f63d29f        9 months ago        273MB
```
<a name="tIfLP"></a>
#### 2)、镜像上传
使用 docker push 上传镜像。
```bash
$ docker push 192.168.110.158:5000/my-project-server
The push refers to repository [192.168.110.158:5000/my-project-server]
5b9e874b9f9c: Pushed 
e87c042d22f8: Pushed 
b4cfcb8385a8: Pushed 
2b730cf18c09: Pushed 
edeaba958753: Pushed 
8bf42db0de72: Pushed 
31892cc314cb: Pushed 
11936051f93b: Pushed 
v1: digest: sha256:5c8a0efff409649a389d0bc74dda94ca96e67e87c92673b4c7dad0078657af40 size: 2000
```
<a name="u75qe"></a>
#### 3)、查看镜像
使用 curl 查看 192.168.110.158:5000/v2/_catalog 地址，可以看到上传成功的镜像
```bash
$ curl 192.168.110.158:5000/v2/_catalog
{"repositories":["my-project-server"]}
```
这里可以成功的看到上传的镜像。<br />也可以在刚刚搭建的WebUI上可视化的查看镜像。效果图如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690777499204-f6090e39-8553-4363-95ae-45bc1f67d948.png#averageHue=%23fdfbfa&clientId=u4c82a5ba-20bc-4&from=paste&id=ud49b2d20&originHeight=640&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8e5f0753-3c5f-47bc-a947-2315c67aa73&title=)
<a name="aSSlU"></a>
#### 4)、验证
此时将本地镜像删除，尝试从私有仓库下载这个镜像<br />先删除已有镜像
```bash
docker rmi my-project-server:v1 192.168.110.158:5000/my-project-server:v1
```
下载镜像
```bash
$ docker pull 192.168.110.158:5000/my-project-server:v1
v1: Pulling from my-project-server
0e29546d541c: Already exists 
9b829c73b52b: Already exists 
cb5b7ae36172: Already exists 
99ce012bef04: Already exists 
22dc2a72d098: Already exists 
9c69a57e10d9: Already exists 
776f54050ab5: Pull complete 
65a83a9a7871: Pull complete 
Digest: sha256:5c8a0efff409649a389d0bc74dda94ca96e67e87c92673b4c7dad0078657af40
Status: Downloaded newer image for 192.168.110.158:5000/my-project-server:v1
192.168.110.158:5000/my-project-server:v1
$ docker images
REPOSITORY                               TAG                 IMAGE ID            CREATED             SIZE
192.168.110.158:5000/my-project-server   v1                  6af7d633afb7        15 minutes ago      334MB
openjdk                                  8-jre               26ac3f63d29f        9 months ago        273MB
```
可以看到可以正常拉取。
<a name="bnjyt"></a>
## **四、容器启动**
镜像构建并上传至完成后，可以直接使用Docker Compose来启动容器。实现一次构建到处运行。<br />创建目录
```bash
mkdir -p /usr/local/docker/my-project
```
在该目录下创建Docker Registry 的 docker-compose.yml。内容如下：
```bash
cd /usr/local/docker/my-project
```
```yaml
version: '3.1'
services:
  my_project_server:
    image: 192.168.110.158:5000/my-project-server:v1
    container_name: my-project-server
    restart: always
    ports:
      - 8899:8899
    volumes:
      - ./logs:/logs
    environment:
      TZ: Asia/Shanghai
```
启动容器
```bash
docker-compose up -d
```
查看容器启动状态
```bash
docker ps -a
```
访问查询所有用户接口

- http://ip:8899/sys-user/get/all

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690777499255-c6b1dc64-3b35-4fe3-8a44-bc083d6bd65f.png#averageHue=%23f7f7fd&clientId=u4c82a5ba-20bc-4&from=paste&id=uc5d8ac96&originHeight=914&originWidth=610&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8fc3774d-dcc3-415e-a011-3f773720d23&title=)
