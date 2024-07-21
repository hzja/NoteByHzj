Docker
<a name="S7Os0"></a>
## docker简介
Docker是一个开源的引擎，可以轻松地为任何应用创建一个轻量级的、可移植的、自给自足的容器。<br />![Docker Logo](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1675129793126-02b23233-f600-4469-baf3-51395b88fa89.jpeg#averageHue=%23f0f3ef&clientId=u34761ad6-ff90-4&from=paste&id=u4782fed8&originHeight=172&originWidth=400&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=ue9cc55cc-9574-4d7f-9374-f352032a958&title=Docker%20Logo "Docker Logo")<br />Docker的英文翻译是码头工人，码头工人一般搬运的都是集装箱（Container），集装箱最大的成功在于其产品的标准化以及由此建立的一整套运输体系。在一艘几十万吨的巨轮上，装满了各种各样满载的集装箱，彼此之间不会相互影响；因此其本身就有标准化、集约化的特性。<br />从Docker的logo也能看出，Docker的思想来自于集装箱；各个应用程序相当于不同的集装箱，每个应用程序有着不同的应用环境，比如python开发的应用需要服务器部署一套python的开发环境，nodejs开发的应用需要服务器部署nodejs的环境，不同环境之间有可能还会彼此冲突，Docker可以隔离不同的环境。<br />有些同学于是就想到了，这不是虚拟机干的活么。是的，虚拟机可以很好地隔离各个环境，可以在windows上运行macos、ubuntu等虚拟机，也可以在macos上安装windows的虚拟机；不过传统的虚拟机技术是虚拟一整套硬件后，在其上运行完整的操作系统，在该系统上再运行所需应用进程，这样导致一台电脑只能运行数量较少的虚拟机。<br />但是Docker使用的容器技术比虚拟机更加的轻便和快捷。容器内的应用进程直接运行于宿主的内核，容器内没有自己的内核，而且也没有进行硬件虚拟。因此容器要比传统虚拟机更为轻便，下图比较了两者的区别：<br />![Docker和虚拟机的区别](https://cdn.nlark.com/yuque/0/2023/png/396745/1675129793075-2af62820-cdeb-4ac0-9a4b-e31b15c0d2bf.png#averageHue=%23ccdfe3&clientId=u34761ad6-ff90-4&from=paste&id=u85e7a8f6&originHeight=501&originWidth=714&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=uafb62356-c36a-408b-ae34-629b13d8b45&title=Docker%E5%92%8C%E8%99%9A%E6%8B%9F%E6%9C%BA%E7%9A%84%E5%8C%BA%E5%88%AB "Docker和虚拟机的区别")<br />对比总结：

| 特性 | 容器 | 虚拟机 |
| --- | --- | --- |
| 启动 | 秒级 | 分钟级 |
| 硬盘使用 | 一般为 MB | 一般为 GB |
| 系统资源 | 0~5% | 5~15% |
| 性能 | 接近原生 | 弱于原生 |
| 系统支持量 | 单机支持上千个容器 | 一般几十个 |

<a name="PCDrV"></a>
### Docker优势
docker有以下优势：

- 更高效地利用系统资源
- 更快速的启动时间
- 一致的运行环境
- 持续交付和部署
- 更轻松的迁移
- 更轻松的维护和扩展

docker通常用于如下场景：

- web应用的自动化打包和发布；
- 自动化测试和持续集成、发布；
- 在服务型环境中部署和调整数据库或其他的后台应用；
- 从头编译或者扩展现有的OpenShift或Cloud Foundry平台来搭建自己的PaaS环境。
<a name="HRBvO"></a>
### 基本概念
在Docker中有三个基本概念：

- 镜像（Image）
- 容器（Container）
- 仓库（Repository）

理解了Docker的基本概念，就理解了Docker的整个生命周期。<br />首先来弄懂镜像的概念，Docker镜像是一个特殊的文件系统，除了提供容器运行时所需的程序、库、资源、配置等文件外，还包含了一些为运行时准备的一些配置参数（如匿名卷、环境变量、用户等）。镜像不包含任何动态数据，其内容在构建之后也不会被改变。<br />如果有装系统经验的童鞋，可以把Docker镜像理解为一个操作系统的镜像（ISO文件），它是一个固定的文件，从一个镜像中，可以装到很多电脑上，变成一个个的操作系统（相当于容器），每个系统都是相同的，不过可以选择定制化安装。<br />和系统镜像不同的是，Docker镜像并不是像ISO文件那样整体打包成一个文件的，而是设计成了分层存储的架构，它并不是由一个文件组成，而是由多层文件联合组成。<br />构建镜像时，会一层层的构建，前面一层是后面一层的基础。每一层构建完就不会再发生改变，后一层上的任何改变只发生在自己这一层。<br />其次是容器的概念，从编程的角度看，镜像和容器的关系更像是类和实例的关系；从一个镜像可以启动一个或者多个容器；镜像是静态的定义，容器是镜像运行时的实体。容器可以被创建、启动、停止、删除、暂停等。<br />![容器和镜像的关系](https://cdn.nlark.com/yuque/0/2023/png/396745/1675129793087-8b6c3bec-f976-48c5-bf54-4813819d24ad.png#averageHue=%23fbfbfb&clientId=u34761ad6-ff90-4&from=paste&id=u51ee6adf&originHeight=188&originWidth=720&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u3346b0ba-c46e-4f43-9160-5072617e584&title=%E5%AE%B9%E5%99%A8%E5%92%8C%E9%95%9C%E5%83%8F%E7%9A%84%E5%85%B3%E7%B3%BB "容器和镜像的关系")<br />前面讲过镜像使用的是分层存储，容器也是如此。每一个容器运行时，是以镜像为基础层，在其上创建一个当前容器的存储层，可以称这个为容器运行时读写而准备的存储层为容器存储层。<br />容器存储层的生存周期和容器一样，容器消亡时，容器存储层也随之消亡。因此，任何保存于容器存储层的信息都会随容器删除而丢失。<br />![容器存储层](https://cdn.nlark.com/yuque/0/2023/png/396745/1675129793103-a73ebeee-7d8b-46ab-8fe0-81b356bd5563.png#averageHue=%23ddd3b3&clientId=u34761ad6-ff90-4&from=paste&id=u95ebd2e5&originHeight=185&originWidth=547&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u4be0ab7f-a541-4d43-a1b9-879d704193d&title=%E5%AE%B9%E5%99%A8%E5%AD%98%E5%82%A8%E5%B1%82 "容器存储层")<br />因此容器不应该向其存储层内写入任何数据，容器存储层要保持无状态化。所有的文件写入操作，都应该使用数据卷（Volume）、或者绑定宿主目录，在这些位置的读写会跳过容器存储层。<br />最后是仓库（Repository）的概念，构建一个镜像后，可以在本地运行，但是如果想要给网络上的其他用户使用，就要一个集中存储和分发镜像的服务器，仓库就是这样一个工具，有点类似Github。<br />镜像仓库Repository是同一类镜像的集合，包含了不同tag（标签）的Docker镜像，比如ubuntu是仓库的名称，它里面有不同的tag，比如16.04、18.04，从镜像仓库中来获取镜像时可以通过<仓库名>:<标签>的格式来指定具体版本的镜像，比如ubuntu18.04；如果忽略标签，用latest作为默认标签。
<a name="UX91k"></a>
## 镜像
上面介绍过，镜像是Docker的三个基本组件之一；运行容器需要本地有相应的镜像，如果没有会从远程仓库下载；那么来看下如何操作镜像。
<a name="W4U2s"></a>
### 查找镜像
可以从Docker Hub来搜索镜像
```bash
docker search ubuntu
```
查找结果：<br />![Docker查找镜像](https://cdn.nlark.com/yuque/0/2023/png/396745/1675129793078-e3e8ea9f-9103-4a84-8323-485a97b41f83.png#averageHue=%230c0a08&clientId=u34761ad6-ff90-4&from=paste&id=u8d0c9f78&originHeight=463&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=ua5b494e0-1ab1-47df-a34d-12af4e3f720&title=Docker%E6%9F%A5%E6%89%BE%E9%95%9C%E5%83%8F "Docker查找镜像")<br />查找的列表中包含了以下几个字段：

- NAME：镜像仓库源的名称
- DESCRIPTION：镜像的描述
- STARS：类似Github里面的 star。
- OFFICIAL：是否docker官方发布
- AUTOMATED：自动构建。
<a name="gkkwm"></a>
### 获取镜像
要获取镜像，可以通过`docker pull`命令，它的格式如下：
```bash
docker pull <repository>:<tag>
```
还是以ubuntu为例：
```bash
$ docker pull ubuntu:16.04

16.04: Pulling from library/ubuntu
58690f9b18fc: Pull complete
b51569e7c507: Pull complete
da8ef40b9eca: Pull complete
fb15d46c38dc: Pull complete
Digest: sha256:0f71fa8d4d2d4292c3c617fda2b36f6dabe5c8b6e34c3dc5b0d17d4e704bd39c
Status: Downloaded newer image for ubuntu:16.04
docker.io/library/ubuntu:16.04
```
可以看到最后一行docker.io显示这是从官方仓库拉取的。<br />从下载过程可以看出上面说的分层存储的概念，即镜像是由多层存储构成；下载也是一层层的去下载，而不是单独一个文件；因此如果下载中有某个层已经被其他镜像下载过，则会显示Already exists。下载过程中给出了每一层的ID的前12位，下载结束后给出镜像完整的sha256摘要。<br />Docker的镜像仓库分为官方仓库和非官方，官方的镜像就是从Docker Hub拉取的；如果想要从第三方的镜像仓库获取，可以在仓库名称前加上仓库的服务地址：
```bash
docker pull <Docker Registry地址:端口号><repository>:<tag>
```
<a name="vT0Ld"></a>
### 列出镜像
通过下面的命令，可以列出本地已经下载的镜像：
```bash
$ docker image ls
```
运行命令出现以下列表：<br />![列出镜像](https://cdn.nlark.com/yuque/0/2023/png/396745/1675129793697-c5182edc-a23b-4ea1-9700-63a843d3caf2.png#averageHue=%230e0c0a&clientId=u34761ad6-ff90-4&from=paste&id=ube358306&originHeight=322&originWidth=1059&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u005a5c7b-bc13-4f40-9fc2-25e3ba6270b&title=%E5%88%97%E5%87%BA%E9%95%9C%E5%83%8F "列出镜像")<br />列表包含了仓库名、标签、镜像ID、创建时间和所占用空间；可以看到有两个mongo的镜像，不过两个镜像有不同的标签。<br />ls命令默认会列出所有的镜像，但是当本地镜像比较多的时候不方便查看，有时候希望列出部分的镜像，除了可以通过linux的grep命令，还可以在ls命令后面跟上参数：
```bash
$ docker image ls mongo

REPOSITORY    TAG           IMAGE ID       CREATED         SIZE
mongo         latest        dfda7a2cf273   2 months ago    693MB
mongo         4.0           e305b5d51c0a   2 months ago    430MB
```
<a name="VF3l9"></a>
### 删除镜像
可以通过rm命令删除本地镜像：
```bash
$ docker image rm [选项] <镜像1> [<镜像2> ...]
```
或者简写为rmi命令：
```bash
$ docker rmi [选项] <镜像1> [<镜像2> ...]
```
这里的<镜像>，可以是镜像短ID、镜像长ID、镜像名或者镜像摘要；`docker image ls`列出来的已经是短ID了，还可以取前三个字符进行删除；比如想要删除上面的mongo:4.0：
```bash
$ docker rmi e30
```
<a name="KebLg"></a>
### 构建镜像
除了使用官方的镜像，可以构建自己的镜像；一般都在其他的镜像基础上进行构建，比如node、nginx等；构建镜像需要用到Dockerfile，它是一个文本文件，文本内容包含了一条条构建镜像所需的指令和说明。<br />在一个空白目录新建一个Dockerfile：
```bash
mkdir mynginx
cd mynginx/
touch Dockerfile
```
向Dockerfile写入以下内容：
```dockerfile
FROM nginx
RUN echo '<h1>Hello, This is My Nginx</h1>' > /usr/share/nginx/html/index.html
```
这里的Dockerfile很简单，就两个命令：FROM和RUN，在Dockerfile里面对命令进行详细的介绍；使用`build`命令构建镜像，它的格式为：
```bash
docker build [选项] <上下文路径/URL/->
```
因此，在Dockerfile所在的目录执行命令：
```bash
$ docker build -t mynginx:v3 .
```
运行命令，可以看到镜像也是按照Dockerfile里面的步骤，分层进行构建的：<br />![构建镜像](https://cdn.nlark.com/yuque/0/2023/png/396745/1675129794369-9fad4fd0-25fb-4ab4-8c5d-b59e970dc7fa.png#averageHue=%230a0807&clientId=u34761ad6-ff90-4&from=paste&id=ubd9afee3&originHeight=203&originWidth=997&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=ud15a3791-0f96-499c-8338-7f4dd38fc0b&title=%E6%9E%84%E5%BB%BA%E9%95%9C%E5%83%8F "构建镜像")<br />构建成功后，列出所有的镜像就能看到刚刚构建的mynginx了。在上面的命令中，可以发现最后有一个`.`，它表示了当前目录，如果不写这个目录会报错提示；如果对应上面的格式，它其实就是上下文路径，那这个上下文路径是做什么用的呢？要理解这个路径的作用，首先要来理解Docker的架构。<br />Docker是一个典型的C/S架构的应用，它可以分为Docker客户端（平时敲的Docker命令）和Docker服务端（Docker守护进程）。Docker客户端通过REST API和服务端进行交互，docker客户端每发送一条指令，底层都会转化成REST API调用的形式发送给服务端，服务端处理客户端发送的请求并给出响应。<br />因此表面上看好像在本机上执行各种Docker的功能，实际上都是都是在Docker服务端完成的，包括Docker镜像的构建、容器创建、容器运行等工作都是Docker服务端来完成的，Docker客户端只是承担发送指令的角色。<br />![Docker架构](https://cdn.nlark.com/yuque/0/2023/png/396745/1675129793583-b80f7051-8906-49a1-a7f8-dca6f48a889f.png#averageHue=%23a8c965&clientId=u34761ad6-ff90-4&from=paste&id=u7b96e78a&originHeight=563&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u1d4bec74-0579-4129-8fa7-636f09a7156&title=Docker%E6%9E%B6%E6%9E%84 "Docker架构")<br />理解了Docker的架构就很容器理解Docker构建镜像的工作原理了，它的流程大致如下：

1. 执行build命令
2. Docker客户端会将构建命令后面指定的上下文路径下的所有文件打包成一个tar包，发送给Docker服务端;
3. Docker服务端收到客户端发送的tar包，然后解压，根据Dockerfile里面的指令进行镜像的分层构建；

因此上下文路径本质上就是指定服务端上Dockerfile中指令工作的目录；比如在Dockerfile中经常需要拷贝代码到镜像中去，因此会这么写：
```dockerfile
COPY ./package.json /app/
```
这里要复制的package.json文件，并不一定在docker build命令执行的目录下，也不一定是在Dockerfile文件同级目录下，而是docker build命令指定的上下文路径目录下的package.json。
<a name="OZ9yk"></a>
## 容器
介绍了镜像，到了Docker第三个核心概念了：容器。容器是镜像的运行时的实例，可以从一个镜像上启动一个或多个容器。<br />对容器的管理包括创建、启动、停止、进入、导入导出和删除等，分别来看下每个操作的具体命令以及效果。
<a name="VBOxd"></a>
### 创建启动容器
新建并启动一个容器用的命令是docker run，它后面有时候会带上有很长很长的选项，不过其基本的语法如下：
```bash
$ docker run [选项] 镜像名称 [命令] [参数...]
```
它可以带上一些常见的选项：

- `-d`：容器在后台运行
- `-t`：为容器重新分配一个伪输入终端，通常与-i同时使用
- `-i`：以交互模式运行容器，通常与-t同时使用
- `-P`：随机端口映射
- `-p`：指定端口映射
- `--name`：为容器指定一个名称
- `-e`：设置环境变量
- `--dns`：指定容器使用的DNS服务器
- `-m`：设置容器使用内存最大值
- `--net="bridge"`：指定容器的网络连接类型，支持 bridge/host/none/container: 四种类型；
- `--link`：链接另一个容器
- `-v`：绑定卷
- `--rm`：退出容器后删除该容器

创建一个hello world容器：
```bash
$ docker run hello-world
```
![运行hello world](https://cdn.nlark.com/yuque/0/2023/png/396745/1675129793672-05dadf9c-37c8-457d-b315-294ed5582e75.png#averageHue=%230b0907&clientId=u34761ad6-ff90-4&from=paste&id=ue1cc4277&originHeight=448&originWidth=873&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u5453393b-817d-4e42-a720-dc50329b055&title=%E8%BF%90%E8%A1%8Chello%20world "运行hello world")<br />但是这样创建的容器只能看到一堆的打印说明，不能对容器进行任何操作，可以加上`-it`选项（-i和-t的简写），来让Docker分配一个终端给这个容器：
```bash
$ docker run -it ubuntu:18.04 /bin/bash
root@fdb133227c9a:/# pwd
root@fdb133227c9a:/# ls
root@fdb133227c9a:/# exit
```
可以在容器内部进行操作了，退出终端可以使用exit命令或者ctrl+d；退出容器后如果查看运行中的容器，发现并没有任何容器信息。<br />一般都是需要让容器在后台运行，因此加上`-d`：
```bash
$ docker run -itd ubuntu:18.04 /bin/bash
ad4d11b6d3b6a2a37fc702345a09fa0a5671f5b3943def7963994535e8600f7b
```
容器不再以命令行的方式呈现了，而是直接丢出一长串的数字字母组合，这是容器的唯一id；再用ps命令查看运行状态的容器，可以看到容器已经在后台默默运行了：<br />![容器后台运行](https://cdn.nlark.com/yuque/0/2023/png/396745/1675129793831-8678255f-9f2c-4470-aa82-7e4618479d4c.png#averageHue=%23050504&clientId=u34761ad6-ff90-4&from=paste&id=u8a470a08&originHeight=83&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u71512134-7562-42fb-bb6a-d897467528f&title=%E5%AE%B9%E5%99%A8%E5%90%8E%E5%8F%B0%E8%BF%90%E8%A1%8C "容器后台运行")<br />当使用run命令创建容器时，Docker在后台进行了如下的操作：

- 检查本地是否存在指定的镜像，不存在就从 registry 下载
- 利用镜像创建并启动一个容器
- 分配一个文件系统，并在只读的镜像层外面挂载一层可读写层
- 从宿主主机配置的网桥接口中桥接一个虚拟接口到容器中去
- 从地址池配置一个 ip 地址给容器
- 执行用户指定的应用程序
- 执行完毕后容器被终止
<a name="KxrQV"></a>
### 终止容器
可以使用stop命令来终止容器的运行；如果容器中的应用终结或者报错时，容器也会自动终止；可以使用ps命令查看到的容器短id来终止对应的容器：
```bash
$ docker stop ad4d11b6d3b6
```
对于终止状态的容器，ps命令已经不能看到它了，可以加上`-a`选项（表示所有）来查看，它的STATUS已经变成了Exited：
```bash
$ docker ps -a
CONTAINER ID   IMAGE         COMMAND       CREATED        STATUS                PORTS  
ad4d11b6d3b6   ubuntu:16.04  "/bin/bash"   2 hours ago    Exited (0) 2 minutes ago
```
终止状态的容器可以使用`docker start [容器id]`来让它重新进入启动状态，运行中的容器也可以使用`docker restart [容器id]`让它重新启动。
<a name="teJRv"></a>
### 进入容器
有时候会需要进入容器进行一些操作，比如进入nginx容器进行平滑重启，可以使用`docker attach`或者`docker exec`进入，不过推荐使用`exec`命令。<br />首先看下如果使用attach命令：
```bash
$ docker attach ad4d11b6d3b6
root@ad4d11b6d3b6:/# exit
```
当从终端exit后，整个容器会停止；而使用exec命令不会导致容器停止。<br />如果只使用`-i`参数，由于没有分配伪终端，界面没有熟悉的Linux命令提示符，但是执行命令仍然可以看到运行结果；当使用`-i`和`-t`参数时，才能看到常见的Linux命令提示符。
```bash
$ docker exec -i ad4d11b6d3b6 bash
ls
bin
boot
dev
etc
home
lib
pwd
/

$ docker exec -it ad4d11b6d3b6 bash
root@ad4d11b6d3b6:/# exit
```
需要注意的是，进入的容器需要是运行状态，如果不是运行状态，则会报错：
```
Error response from daemon: Container ad4d11b6d3b6 is not running
```
<a name="m7kK1"></a>
### 查看容器日志
经常需要对容器运行过程进行一些监测，查看它的运行过程记录的日志情况，以及是否报错等等；使用logs命令获取容器的日志。
```bash
$ docker logs ad4d11b6d3b6
```
它还支持以下几个参数：

- `-f`：跟踪日志输出
- `--since`：显示某个开始时间的所有日志
- `-t`：显示时间戳
- `--tail`：仅列出最新N条容器日志

logs命令会展示从容器启动以来的所有日志，如果容器运行时间久，会列出非常多的日志，可以加tail参数仅展示最新的日志记录：
```bash
# 列出最新的10条日志
$ docker logs --tail=10 ad4d11b6d3b6
```
<a name="GcCVx"></a>
### 分析容器
对于已经创建的容器，可以使用`inspect`来查看容器的底层基础信息，包括容器的id、创建时间、运行状态、启动参数、目录挂载、网路配置等等；另外，该命令也能查看docker镜像的信息，它的格式如下：
```bash
docker inspect [选项] <镜像1> [<镜像2> ...]
```
`inspect`支持以下选项：

- `-f`：指定返回值的模板文件。
- `-s`：显示总的文件大小。
- `--type`：为指定类型返回JSON。

运行后，会通过JSON格式显示容器的基本信息：<br />![容器后台运行](https://cdn.nlark.com/yuque/0/2023/png/396745/1675129794274-b7dd97bc-9e6f-4807-b940-4696d12b0d3c.png#averageHue=%23050504&clientId=u34761ad6-ff90-4&from=paste&id=udfd05c0b&originHeight=83&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u5391eead-2e7d-4555-87eb-0ee2cc8c6a4&title=%E5%AE%B9%E5%99%A8%E5%90%8E%E5%8F%B0%E8%BF%90%E8%A1%8C "容器后台运行")<br />但是这么大段的文字，想要获取有用的信息十分的麻烦；除了用grep进行过滤外（万物皆可grep），还可以通过`-f`参数：
```bash
# 获取容器名
docker inspect -f {{.Name}} <容器ID>
# 获取容器目录挂载信息
docker inspect -f {{.NetworkSettings.Mounts}} <容器ID>
# 获取容器网络设置的相关信息
docker inspect -f {{.NetworkSettings}} <容器ID>
# 获取容器的 IP 的相关信息
docker inspect -f {{.NetworkSettings.IPAddress}} <容器ID>
```
<a name="lgRtm"></a>
### 删除容器
如果一个容器不想再使用了，可以使用rm命令来删除：
```bash
$ docker rm ad4d11b6d3b6
```
如果要删除一个运行中的容器，可以添加-f参数：
```bash
$ docker rm -f ad4d11b6d3b6
```
<a name="DcmYO"></a>
## 数据管理
上面介绍到容器是保持无状态化的，就是随用随删，并不会保留数据记录；在使用docker的时候，经常会用到一些需要保留数据的容器，比如mysql、mongodb，往往需要对容器中的数据进行持久化；或者在多个容器之间进行数据共享，这就涉及到了容器的数据管理，主要有两种方式：

- 数据卷（Data Volumes）
- 挂载主机目录 (Bind mounts)
<a name="l2Ub4"></a>
### 数据卷
数据卷是一个可供一个或多个容器使用的特殊目录，它绕过 UFS，可以提供很多有用的特性：

- 数据卷可以在容器之间共享和重用
- 对数据卷的修改会立马生效
- 对数据卷的更新，不会影响镜像
- 数据卷默认会一直存在，即使容器被删除

首先创建一个数据卷：
```bash
$ docker volume create my-vol
my-vol
```
通过ls可以列出本地所有的数据卷：
```bash
$ docker volume ls
DRIVER    VOLUME NAME
local     my-vol
```
`inspect`命令也可以查看数据卷的具体信息：
```bash
$ docker volume inspect my-vol
[
    {
        "CreatedAt": "",
        "Driver": "local",
        "Labels": {},
        "Mountpoint": "/data/programs/docker/volumes/my-vol/_data",
        "Name": "my-vol",
        "Options": {},
        "Scope": "local"
    }
]
```
在启动容器时，使用`--mount`将数据卷挂载在容器的目录里（可以有多个挂载点）：
```bash
$ docker run -d -P --name web --mount source=my-vol,target=/usr/share/nginx/html nginx
# 通过-v简写形式
$ docker run -d -P --name web -v my-vol:/usr/share/nginx/html nginx
```
借助上面的`inspect`命令查看容器的挂载信息：
```bash
$ docker inspect -f "{{.Mounts}}" web
[{volume my-vol /data/programs/docker/volumes/my-vol/_data /usr/share/nginx/html local z true }]
```
数据卷是被设计用来持久化数据的，它的生命周期独立于容器；因此即使将容器删除了，数据卷的数据依然还是存在的，并且也不存在垃圾回收这样的机制来处理没有任何容器引用的数据卷，可以在删除容器的时候使用`docker rm -v`命令同时删除数据卷，或者手动来删除：
```bash
$ docker volume rm my-vol
```
无主的数据卷可能会占据很多空间，要清理请使用以下命令（谨慎使用！）：
```bash
$ docker volume prune
```
<a name="SUgm4"></a>
### 挂载目录
可以发现上面数据卷挂载的目录都是在docker的安装路径下，不利于进行维护，可以直接挂载自定义的目录。
```bash
$ docker run -d -P --name web --mount source=/home/nginx,target=/usr/share/nginx/html nginx
# 通过-v简写形式
$ docker run -d -P --name web -v /home/nginx:/usr/share/nginx/html nginx
```
挂载的本地目录的路径必须是绝对路径，不能是相对路径。<br />本地路径如果不存在，会自动生成。<br />默认挂载的主机目录的默认权限是读写，可以通过增加readonly指定为只读。
```bash
$ docker run -d -P --name web -v /home/nginx:/usr/share/nginx/html:ro nginx
```
加readonly后，如果在容器内的/usr/share/nginx/html目录下修改文件或者新建文件就会报错。<br />需要注意的是，如果挂载本地目录，需要保证挂载的目录下面有程序运行所需要的文件，比如这里nginx容器需要在在本地目录/home/nginx下有index.html文件，如果没有的话会报403错误。
