Docker
<a name="QGAop"></a>
## 一、讲个故事
为了更好的理解 Docker 是什么，先来讲个故事：<br />需要盖一个房子，于是搬石头、砍木头、画图纸、盖房子。一顿操作，终于把这个房子盖好了。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663030850596-5fd6dbe5-6976-4224-97a6-280aa428ed65.png#clientId=ue786c592-20cb-4&from=paste&id=u58ab17bd&originHeight=482&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3110a816-2020-4f9b-8b59-d7ac0aa4879&title=)<br />结果，住了一段时间，心血来潮想搬到海边去。这时候按以往的办法，只能去海边，再次搬石头、砍木头、画图纸、盖房子。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663030850605-b214d67b-0914-46e8-b873-54ead0df07bd.png#clientId=ue786c592-20cb-4&from=paste&id=ue43fefae&originHeight=663&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud8bebb31-35d8-4770-9cf1-42d09700a86&title=)<br />烦恼之际，跑来一个魔法师教会我一种魔法。这种魔法可以把我盖好的房子复制一份，做成「镜像」，放在背包里。<br />![黑魔法](https://cdn.nlark.com/yuque/0/2022/png/396745/1663030850802-cb45eca5-32ac-4278-8658-e4b3c6411c37.png#clientId=ue786c592-20cb-4&from=paste&id=udfa0ae60&originHeight=608&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u83022601-8798-42bb-a178-2837a505068&title=%E9%BB%91%E9%AD%94%E6%B3%95 "黑魔法")<br />等到了海边，就用这个「镜像」，复制一套房子，拎包入住。<br />是不是很神奇？对应到我们的项目中来，房子就是项目本身，镜像就是项目的复制，背包就是镜像仓库。如果要动态扩容，从仓库中取出项目镜像，随便复制就可以了。Build once，Run anywhere!<br />不用再关注版本、兼容、部署等问题，彻底解决了「上线即崩，无休止构建」的尴尬。
<a name="LHaDc"></a>
## 二、虚拟机与容器
开始之前，来做一些基础知识的储备：
<a name="mUuOw"></a>
### 1、虚拟机：虚拟化硬件
虚拟机 Virtual Machine 指通过软件模拟的**具有完整硬件系统功能的、运行在一个完全隔离环境中的完整计算机系统**。在实体计算机中能够完成的工作在虚拟机中都能够实现。<br />在计算机中创建虚拟机时，需要将实体机的部分硬盘和内存容量作为虚拟机的硬盘和内存容量。**每个虚拟机都有独立的 CMOS、硬盘和操作系统，可以像使用实体机一样对虚拟机进行操作**。在容器技术之前，业界的网红是虚拟机。<br />虚拟机技术的代表，是 VMWare 和 OpenStack。
<a name="vTbfw"></a>
### 2、容器：将操作系统层虚拟化，是一个标准的软件单元

- 随处运行：容器可以将代码与配置文件和相关依赖库进行打包，从而确保在任何环境下的运行都是一致的。
- 高资源利用率：容器提供进程级的隔离，因此可以更加精细地设置 CPU 和内存的使用率，进而更好地利用服务器的计算资源。
- 快速扩展：每个容器都可作为单独的进程予以运行，并且可以共享底层操作系统的系统资源，这样一来可以加快容器的启动和停止效率。
<a name="VPM4G"></a>
### 3、区别与联系

- 虚拟机虽然可以隔离出很多「子电脑」，但占用空间更大，启动更慢。虚拟机软件可能还要花钱，例如VMWare；
- 容器技术不需要虚拟出整个操作系统，只需要虚拟一个小规模的环境，类似「沙箱」；
- 运行空间，虚拟机一般要几 GB 到 几十 GB 的空间，而容器只需要 MB 级甚至 KB 级；

来看一下对比数据：<br />与虚拟机相比，容器更轻量且速度更快，因为它利用了 Linux 底层操作系统在隔离的环境中运行。虚拟机的 Hypervisor 创建了一个非常牢固的边界，以防止应用程序突破它，而容器的边界不那么强大。<br />物理机部署不能充分利用资源，造成资源浪费。虚拟机方式部署，虚拟机本身会占用大量资源，导致资源浪费，另外虚拟机性能也很差。而容器化部署比较灵活，且轻量级，性能较好。<br />虚拟机属于虚拟化技术，而 Docker 这样的容器技术，属于轻量级的虚拟化。
<a name="Q0d1u"></a>
## 三、认识 Docker
![Docker](https://cdn.nlark.com/yuque/0/2022/png/396745/1663030850680-712cd9ce-218b-4704-aa4c-55fa866b3725.png#clientId=ue786c592-20cb-4&from=paste&id=u53c1a73d&originHeight=423&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u5ad273b2-8899-4a82-9bd6-cba6d6e14b3&title=Docker "Docker")
<a name="KD9M8"></a>
### 1、概念
Docker 是一个开源的应用容器引擎，让开发者可以打包他们的应用以及依赖包到一个可移植的容器中，然后发布到任何流行的 Linux 机器上，也可以实现虚拟化。容器是完全使用沙箱机制，相互之间不会有任何接口。<br />Docker 技术的**三大核心概念**，分别是：镜像 Image、容器 Container、仓库 Repository。
<a name="b6vGe"></a>
### 2、Docker 轻量级的原因？
为什么 Docker 启动快？如何做到和宿主机共享内核？<br />当请求 Docker 运行容器时，Docker 会在计算机上设置一个资源隔离的环境。然后将打包的应用程序和关联的文件复制到 Namespace 内的文件系统中，此时环境的配置就完成了。之后 Docker 会执行预先指定的命令，运行应用程序。<br />镜像不包含任何动态数据，其内容在构建之后也不会被改变。
<a name="K9ktB"></a>
## 四、核心概念

1. Build, Ship and Run（搭建、运输、运行）；
2. Build once, Run anywhere（一次搭建，处处运行）；
3. Docker 本身并不是容器，它是创建容器的工具，是应用容器引擎；
4. Docker 三大核心概念，分别是：镜像 Image，容器 Container、仓库 Repository；
5. Docker 技术使用 Linux 内核和内核功能（例如 Cgroups 和 namespaces）来分隔进程，以便各进程相互独立运行。
6. 由于 Namespace 和 Cgroups 功能仅在 Linux 上可用，因此容器无法在其他操作系统上运行。那么 Docker 如何在 macOS 或 Windows 上运行？ Docker 实际上使用了一个技巧，并在非 Linux 操作系统上安装 Linux 虚拟机，然后在虚拟机内运行容器。
7. 镜像是一个可执行包，其包含运行应用程序所需的代码、运行时、库、环境变量和配置文件，容器是镜像的**运行时实例**。
<a name="MZxy3"></a>
## 五、安装 Docker
<a name="Z8q0p"></a>
### 1、命令行安装
Homebrew 的 Cask 已经支持 Docker for Mac，因此可以很方便的使用 Homebrew Cask 来进行安装，执行如下命令：
```bash
brew cask install docker
```
更多安装方式，请查看官方文档：[https://www.docker.com/get-started](https://www.docker.com/get-started)
<a name="ZsOYu"></a>
### 2、查看版本
```bash
docker -v
```
<a name="SHNNE"></a>
### 3、配置镜像加速
设置 Docker Engine 写入配置：
```json
{
  "registry-mirrors": [
    "http://hub-mirror.c.163.com/",
    "https://registry.docker-cn.com"
  ],
  "insecure-registries":[],
  "experimental": false,
  "debug": true
}
```
<a name="wWkaY"></a>
### 4、安装桌面端
![Docker 桌面端](https://cdn.nlark.com/yuque/0/2022/png/396745/1663030850694-d9b6cd6e-7417-41de-9f4e-d8324f346d84.png#clientId=ue786c592-20cb-4&from=paste&id=ua95eab63&originHeight=644&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ubd890146-6eae-4e7b-a4f2-9a591db885d&title=Docker%20%E6%A1%8C%E9%9D%A2%E7%AB%AF "Docker 桌面端")<br />桌面端操作非常简单，先去官网下载。通过 Docker 桌面端，可以方便的操作：

1. clone：克隆一个项目
2. build：打包镜像
3. run：运行实例
4. share：共享镜像

好了，准备工作就绪，下面可以大展身手了！
<a name="NZ1Oq"></a>
## 六、快速开始
安装完 Docker 之后，先打个实际项目的镜像，边学边用。
<a name="LiKpg"></a>
### 1、首先需要大致了解一下将会用到的 11 个命令
![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1663030851027-2feba3b5-1e47-4a14-a019-d0df71ba427a.jpeg#clientId=ue786c592-20cb-4&from=paste&id=u53ab721b&originHeight=500&originWidth=711&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9e48fb23-79df-4d74-acd2-2caf106146d&title=)
<a name="Y7jWK"></a>
### 2、新建项目
为了快捷，直接使用Vue 脚手架构建项目：
```bash
vue create docker-demo
```
尝试启动一下：
```bash
yarn serve
```
访问地址：http://localhost:8080/。项目就绪，接着为项目打包：
```bash
yarn build
```
这时候，项目目录下的 Dist 就是要部署的静态资源了，继续下一步。<br />**需要注意**：前端项目一般分两类，一类直接 Nginx 静态部署，一类需要启动 Node 服务。这里只考虑第一种。
<a name="Giav0"></a>
### 3、新建 Dockerfile
```bash
cd docker-demo && touch Dockerfile
```
此时的项目目录如下：
```bash
.  
├── Dockerfile  
├── README.md  
├── babel.config.js  
├── dist  
├── node_modules  
├── package.json  
├── public  
├── src  
└── yarn.lock  
```
可以看到已经在 docker-demo 目录下成功创建了 Dockerfile 文件。
<a name="a2bk5"></a>
### 4、准备 Nginx 镜像
运行 Docker 桌面端，就会默认启动实例，在控制台拉取 Nginx 镜像：
```bash
docker pull nginx
```
控制台会出现如下信息：
```bash
Using default tag: latest
latest: Pulling from library/nginx
8559a31e96f4: Pull complete
8d69e59170f7: Pull complete
3f9f1ec1d262: Pull complete
d1f5ff4f210d: Pull complete
1e22bfa8652e: Pull complete
Digest: sha256:21f32f6c08406306d822a0e6e8b7dc81f53f336570e852e25fbe1e3e3d0d0133
Status: Downloaded newer image for nginx:latest
docker.io/library/nginx:latest
```
如果出现这样的异常，请确认 Docker 实例是否正常运行。
```bash
Cannot connect to the Docker daemon at unix:///var/run/docker.sock. Is the docker daemon running?
```
镜像准备 OK，在根目录创建 Nginx 配置文件：
```bash
touch default.conf
```
写入：
```nginx
server {
    listen       80;
    server_name  localhost;

    #charset koi8-r;
    access_log  /var/log/nginx/host.access.log  main;
    error_log  /var/log/nginx/error.log  error;

    location / {
        root   /usr/share/nginx/html;
        index  index.html index.htm;
    }

    error_page   500 502 503 504  /50x.html;
    location = /50x.html {
        root   /usr/share/nginx/html;
    }
}
```
<a name="a58AR"></a>
### 5、配置镜像
打开 Dockerfile ，写入如下内容：
```dockerfile
FROM nginx
COPY dist/ /usr/share/nginx/html/
COPY default.conf /etc/nginx/conf.d/default.conf
```
逐行解释一下代码：

- `FROM nginx` 指定该镜像是基于 nginx:latest 镜像而构建的；
- `COPY dist/ /usr/share/nginx/html/` 命令的意思是将项目根目录下 dist 文件夹中的所有文件复制到镜像中 /usr/share/nginx/html/ 目录下；
- `COPY default.conf /etc/nginx/conf.d/default.conf` 将 default.conf 复制到 etc/nginx/conf.d/default.conf，用本地的 default.conf 配置来替换 Nginx 镜像里的默认配置。
<a name="Dl2JH"></a>
### 6、构建镜像
Docker 通过 build 命令来构建镜像：
```bash
docker build -t jartto-docker-demo .
```
按照惯例，解释一下上述代码：

- `-t` 参数给镜像命名 jartto-docker-demo
- . 是基于当前目录的 Dockerfile 来构建镜像

执行成功后，将会输出：
```bash
Sending build context to Docker daemon  115.4MB
Step 1/3 : FROM nginx
 ---> 2622e6cca7eb
Step 2/3 : COPY dist/ /usr/share/nginx/html/
 ---> Using cache
 ---> 82b31f98dce6
Step 3/3 : COPY default.conf /etc/nginx/conf.d/default.conf
 ---> 7df6efaf9592
Successfully built 7df6efaf9592
Successfully tagged jartto-docker-demo:latest
```
镜像制作成功！来查看一下容器：
```bash
docker image ls | grep jartto-docker-demo
```
可以看到，打出了一个 133MB 的项目镜像：
```bash
jartto-docker-demo latest 7df6efaf9592 About a minute ago 133MB  
```
镜像也有好坏之分，后续将介绍如何优化，这里可以先暂时忽略。
<a name="zUHUo"></a>
### 7、运行容器
```bash
docker run -d -p 3000:80 --name docker-vue jartto-docker-demo
```
这里解释一下参数：

- `-d` 设置容器在后台运行
- `-p` 表示端口映射，把本机的 3000 端口映射到 container 的 80 端口（这样外网就能通过本机的 3000 端口访问了
- `--name` 设置容器名 docker-vue
- jartto-docker-demo 是上面构建的镜像名字

补充一点：<br />在控制台，可以通过 `docker ps` 查看刚运行的 Container 的 ID：
```bash
docker ps -a
```
控制台会输出：
```bash
CONTAINER ID IMAGE              COMMAND                  CREATED       STATUS PORTS  NAMES
ab1375befb0b jartto-docker-demo "/docker-entrypoint.…"   8 minutes ago Up 7 minutes  0.0.0.0:3000->80/tcp  docker-vue
```
如果使用桌面端，那么打开 Docker Dashboard 就可以看到容器列表了，如下图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663030851154-c7613852-cf5b-46fc-aa69-35688b9cb259.png#clientId=ue786c592-20cb-4&from=paste&id=ud227f5fe&originHeight=301&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc0133a7e-e511-481a-9a48-1e0818b2ae2&title=)
<a name="VFqm4"></a>
### 8、访问项目
因为映射了本机 3000 端口，所以执行：
```bash
curl -v -i localhost:3000  
```
或者打开浏览器，访问：localhost:3000
<a name="OUaMR"></a>
### 9、发布镜像
如果想为社区贡献力量，那么需要将镜像发布，方便其他开发者使用。<br />发布镜像需要如下步骤：

- 登陆 [dockerhub]([https://hub.docker.com](https://hub.docker.com))，注册账号；
- 命令行执行 docker login，之后输入账号密码，进行登录；
- 推送镜像之前，需要打一个 Tag，执行 `docker tag <image> <username>/<repository>:<tag>`

全流程结束，以后要使用，再也不需要「搬石头、砍木头、画图纸、盖房子」了，拎包入住。**这也是 docker 独特魅力所在。**
<a name="e59WD"></a>
## 七、DockerFile常规命令简介
到这里，已经完成了 Docker 的入门项目！如果还想继续深入，不妨接着往下看看。
<a name="bpZZS"></a>
### 1、参数使用
<a name="WUfZY"></a>
#### `FROM`：指定基础镜像，所有构建的镜像都必须有一个基础镜像，且 `FROM` 命令必须是 Dockerfile 的第一个命令
`FROM <image> [AS <name>]` 指定从一个镜像构建起一个新的镜像名字<br />`FROM <image>[:<tag>] [AS <name>]` 指定镜像的版本 Tag<br />示例：`FROM mysql:5.0 AS database`
<a name="KGqOL"></a>
#### `MAINTAINER`：镜像维护人的信息
```dockerfile
MAINTAINER <name>
```
示例：
```dockerfile
MAINTAINER Jartto Jartto@qq.com
```
<a name="eZutL"></a>
#### `RUN`：构建镜像时要执行的命令
```dockerfile
RUN <command>
```
示例：`RUN ["executable", "param1", "param2"]`
<a name="KQOGm"></a>
#### `ADD`：将本地的文件添加复制到容器中去，压缩包会解压，可以访问网络上的文件，会自动下载
```dockerfile
ADD <src> <dest>
```
示例：`ADD *.js /app` 添加 js 文件到容器中的 app 目录下
<a name="Y7dca"></a>
#### `COPY`：功能和 ADD 一样，只是复制，不会解压或者下载文件
<a name="vFG5v"></a>
#### `CMD`：启动容器后执行的命令，和 RUN 不一样，RUN 是在构建镜像是要运行的命令
当使用 `docker run` 运行容器的时候，这个可以在命令行被覆盖<br />示例：
```dockerfile
CMD ["executable", "param1", "param2"]
```
<a name="gVWnr"></a>
#### `ENTRYPOINT`：也是执行命令，和 CMD 一样，只是这个命令不会被命令行覆盖
```dockerfile
ENTRYPOINT ["executable", "param1", "param2"]
```
示例：
```dockerfile
ENTRYPOINT ["donnet", "myapp.dll"]
```
<a name="zQLH3"></a>
#### `LABEL`：为镜像添加元数据，key-value 形式
```dockerfile
LABEL <key>=<value> <key>=<value> ...
```
示例：
```dockerfile
LABEL version="1.0" description="这是一个web应用"
```
<a name="IaZv7"></a>
#### `ENV`：设置环境变量，有些容器运行时会需要某些环境变量
`ENV <key> <value>` 一次设置一个环境变量<br />`ENV <key>=<value> <key>=<value> <key>=<value>` 设置多个环境变量<br />示例：
```dockerfile
ENV JAVA_HOME /usr/java1.8/
```
<a name="cTjgf"></a>
#### `EXPOSE`：暴露对外的端口（容器内部程序的端口，虽然会和宿主机的一样，但是其实是两个端口）
```dockerfile
EXPOSE <port>
```
示例：
```dockerfile
EXPOSE 80
```
容器运行时，需要用 `-p` 映射外部端口才能访问到容器内的端口
<a name="bF29g"></a>
#### `VOLUME`：指定数据持久化的目录，官方语言叫做挂载
`VOLUME /var/log` 指定容器中需要被挂载的目录，会把这个目录映射到宿主机的一个随机目录上，实现数据的持久化和同步。<br />`VOLUME ["/var/log","/var/test".....]` 指定容器中多个需要被挂载的目录，会把这些目录映射到宿主机的多个随机目录上，实现数据的持久化和同步<br />`VOLUME /var/data var/log` 指定容器中的 `var/log` 目录挂载到宿主机上的 `/var/data` 目录，这种形式可以手动指定宿主机上的目录
<a name="LZaA3"></a>
#### `WORKDIR`：设置工作目录，设置之后 ，RUN、CMD、COPY、ADD 的工作目录都会同步变更
```dockerfile
WORKDIR <path>
```
示例：
```dockerfile
WORKDIR /app/test
```
<a name="R6plE"></a>
#### `USER`：指定运行命令时所使用的用户，为了安全和权限起见，根据要执行的命令选择不同用户
```dockerfile
USER <user>:[<group>]
```
示例：
```dockerfile
USER test
```
<a name="ZMZ0U"></a>
#### `ARG`：设置构建镜像是要传递的参数
```dockerfile
ARG <name>[=<value>]
ARG name=sss
```
更多操作，请移步官方使用文档：[https://docs.docker.com/](https://docs.docker.com/)
<a name="tOyiK"></a>
## 八、最佳实践
在掌握 Docker 常规操作之后，很容易就可以打出自己想要的项目镜像。然而不同的操作打出的镜像也是千差万别。<br />究竟是什么原因导致镜像差异，不妨继续探索。<br />以下是在应用 Docker 过程中整理的**最佳实践**，请尽量遵循如下准则：

1. Require 明确：需要什么镜像
2. 步骤精简：变化较少的 Step 优先
3. 版本明确：镜像命名明确
4. 说明文档：整个镜像打包步骤可以重现

推荐如下两篇文章：

- [https://www.docker.com/blog/intro-guide-to-dockerfile-best-practices/](https://www.docker.com/blog/intro-guide-to-dockerfile-best-practices/)
- [https://docs.docker.com/develop/develop-images/dockerfile_best-practices/](https://docs.docker.com/develop/develop-images/dockerfile_best-practices/)
<a name="kIhNR"></a>
## 九、总结
容器化技术必将是云时代不可或缺的技能之一，而 Docker 只是沧海一粟。随之而来的还有集群容器管理 K8s、Service Mesh 、Istio 等技术。打开 Docker 的大门，不断**抽丝剥茧，逐层深入**，你将感受到容器化的无穷魅力。
