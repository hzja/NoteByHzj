Docker GUI ctop lazydocker Portainer
<a name="Portainer"></a>
# Portainer
Portainer是一款Web应用程序基于开源Zlib许可证。支持Linux，Mac OS X，Windows操作系统。Portainer完全支持以下Docker版本：Docker 1.10到最新版本。<br />独立的Docker Swarm1.2.3以上版本。需要提醒大家的是，由于Docker引入了内置的Swarm模式，所以不鼓励使用独立的Docker Swarm。旧版本的Portainer支持独立Docker Swarm，而Portainer 1.17.0和更新版本不支持它。但是完全支持Docker内置的Swarm模式。<br />对以下Docker版本的部分支持(某些功能可能不可用)：Docker 1.9。<br />![2021-10-01-18-17-40-621517.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633083470354-492733de-e1c1-416b-b4f0-e94461886e30.png#clientId=uca422c6e-467c-4&from=ui&id=u920c2e52&originHeight=567&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=123332&status=done&style=shadow&taskId=ud5e716bc-c8d4-47f1-9d43-ecb44d26528&title=)<br />测试一个现场演示(admin/tryportainer)。Portainer可以与Docker轻松安装在独立的Linux/Windows服务器/集群上。<br />可以管理注册表，网络、卷、镜像和容器。还可以保存配置(可以在实时演示中找到alertmanager和Prometheus的示例)，并配置Docker Swarm和堆栈。Portainer可以检查容器是否健康。<br />除了需要使用容器的基本操作，例如运行，停止，恢复，终止，删除等之外，还可以检查容器，查看日志，可视化基本统计信息，附加并打开某些容器的控制台。<br />另外，还可以获得基于角色的访问系统，并且可以安装扩展。<br />结论：Portainer是强大的GUI工具，可用于具有本地或远程容器，Docker堆栈或Docker Swarm的团队项目。但是，对于一般需求，Portainer可能并不适合。界面也可能不方便，特别是如果同时使用多个项目。
<a name="qGD7V"></a>
## Portainer的安装
[Docker安装Portainer](https://www.yuque.com/go/doc/23583444?view=doc_embed)
<a name="tOMjK"></a>
# DockStation
DockStation是一卷桌面应用程序;支持Linux、Mac、Windows操作系统。<br />![2021-10-01-18-17-40-844522.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633083493634-ccb05bec-8046-473f-8617-d46fab7ebd50.png#clientId=uca422c6e-467c-4&from=ui&id=u6341439f&originHeight=678&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=266951&status=done&style=shadow&taskId=uf148e5e3-fe86-4580-9034-3623b7edd86&title=)<br />DockStation免费的全功能桌面应用程序，可满足使用docker和docker-compose。可以通过docker-compose.yml使用本机Docker Compose CLI命令帮助生成一个干净的本机文件，甚至在应用程序外部也可以使用。它还可以帮助管理容器和服务，包括远程和本地的容器，并对其进行监控，比如日志监控，搜索日志，分组，运行工具以及获取容器信息。还有其他工具可用于对容器资源进行通用，多个和单个的监控。<br />使用DockStation，可以轻松跟踪CPU，内存，网络I/O的使用情况和开放端口。所有工作都可以组织成项目，可以在其中检查每个容器的状态，构建图形化的方案，能够可视化项目中的每个镜像以及它们之间的关系。此外，DockStation在Docker Hub上十分的受欢迎。
<a name="YHSkI"></a>
# Docker Desktop
Docker Desktop是一款桌面应用程序，由于Docker-toolbox(带有Kitematic)已被弃用，建议所有用户在Mac上使用Docker Desktop，在Windows上使用Docker Desktop。<br />![2021-10-01-18-17-40-914519.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1633083526302-dd6086b1-6498-4d28-8398-c27dc714e34d.jpeg#clientId=uca422c6e-467c-4&from=ui&id=u90711895&originHeight=714&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=59015&status=done&style=shadow&taskId=uf736f8fb-65d5-4c0a-b325-85ea395c981&title=)<br />该工具能够为Docker设置资源限制，比如内存，CPU，磁盘镜像大小，文件共享，代理和网络等，配置Docker引擎，命令行和Kubernetes。<br />使用仪表板，不仅可以使用基本的容器操作，还可以查看日志，基本统计信息并检查容器。所有这些都可以通过上下文菜单或状态栏中的指示器来调用。
<a name="qdmx6"></a>
# Lazydocker(UI终端)
Lazydocker是一款开源的UI终端，支持Linux、OSX、Windows操作系统。要求GO 1.8版本以上，Docker1.13(API 1.25以上)版本，Docker-Compose1.23.2以上版本。<br />Lazydocker可以满足鼠标和键盘的接入。对于某些元素，上下文菜单可用，可以在其中找到所有带有快捷键的常用命令。而且不仅拥有基本的命令来操作容器，基本的统计信息，日志和检查，而且还具有基本功能。还可以使用图形显示主要指标，默认情况下有CPU和内存使用情况和进程。此外，还可以为几乎所有所需的指标进行配置。<br />对于选定的镜像，可以查看Dockerfile中运行时执行的命令以及继承的层。除了修改可用命令和添加新命令之外，还提供了对未使用的容器，镜像，卷的清理。<br />Lazydocker提供极简的终端界面，对一些不太复杂的项目确实很有帮助。<br />lazydocker 的具体特性如下：

- 全视野查看 Docker 或 docker-compose 容器环境的状态
- 查看容器或服务的日志
- 查看容器指标的 ascii 图表，这样你会更像个开发者
- 自定义图表以测量不同指标
- 附加到容器/服务
- 重启、删除与重新构建容器/服务
- 查看给定镜像的祖先图层
- 修剪占用磁盘空间的容器、镜像或卷
<a name="qSEsZ"></a>
## A.lazydocker的官网
[lazydocker官网](https://github.com/jesseduffield/lazydocker)<br />[https://github.com/jesseduffield/lazydocker](https://github.com/jesseduffield/lazydocker)
<a name="mjLPX"></a>
## B.lazydocker的安装
<a name="rl5ML"></a>
### 安装包安装
[lazydocker_0.10_Linux_x86_64.tar.gz](https://www.yuque.com/attachments/yuque/0/2020/gz/396745/1608395299861-99137cf1-e5ab-4881-b0d4-cfa0287f3434.gz?_lake_card=%7B%22src%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2020%2Fgz%2F396745%2F1608395299861-99137cf1-e5ab-4881-b0d4-cfa0287f3434.gz%22%2C%22name%22%3A%22lazydocker_0.10_Linux_x86_64.tar.gz%22%2C%22size%22%3A4088964%2C%22type%22%3A%22application%2Fx-gzip%22%2C%22ext%22%3A%22gz%22%2C%22status%22%3A%22done%22%2C%22uid%22%3A%221608172392668-0%22%2C%22progress%22%3A%7B%22percent%22%3A99%7D%2C%22percent%22%3A0%2C%22refSrc%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2020%2Fgz%2F396745%2F1608172394875-33537edf-0de6-4d11-aa98-9f0164df52a1.gz%22%2C%22id%22%3A%22gMXMh%22%2C%22card%22%3A%22file%22%7D)
```bash
tar zxf lazydocker_0.10_Linux_x86_64.tar.gz
```
<a name="mOIla"></a>
### Linux/OSX脚本安装（官方提供的URL已失效）
```bash
# Binary Release (Linux/OSX)
[root@iZuligp6e1dyzfZ ~]# curl https://raw.githubusercontent.com/jesseduffield/lazydocker/master/scripts/install_update_linux.sh | bash
```
<a name="XH6BT"></a>
### Homebrew
```bash
# Homebrew
brew tap jesseduffield/lazydocker
brew install lazydocker
```
<a name="bq1uV"></a>
### Go
```bash
# Go
# Required Go version >= 1.8
go get github.com/jesseduffield/lazydocker
```
<a name="fsqj8"></a>
### Zsh
```bash
# zsh config
echo "alias lzd='lazydocker'" >> ~/.zshrc
```
<a name="Ta9GW"></a>
### Docker
```dockerfile
# docker build -t lazydocker .
# docker run -it lazydocker:latest /bin/sh -l

FROM golang:alpine
WORKDIR /go/src/github.com/jesseduffield/lazydocker/
COPY ./ .
RUN CGO_ENABLED=0 GOOS=linux go build

FROM alpine:latest
RUN apk add -U git xdg-utils
WORKDIR /go/src/github.com/jesseduffield/lazydocker/
COPY --from=0 /go/src/github.com/jesseduffield/lazydocker /go/src/github.com/jesseduffield/lazydocker
COPY --from=0 /go/src/github.com/jesseduffield/lazydocker/lazydocker /bin/
RUN echo "alias gg=lazydocker" >> ~/.profile
```
<a name="SGTNy"></a>
## C.lazydocker的使用
```bash
[root@iZuligp6e1dyzfZ /]# lazydocker
```
<a name="uAOHc"></a>
### lazydocker的管理界面
使用左右键进行左边菜单内容的切换，使用上下键进行菜单内的切换<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596515322721-d427cc0b-16a3-4a32-8102-7bc39752c411.png#height=593&id=d8vja&originHeight=1780&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2183177&status=done&style=none&title=&width=1107.6666666666667)
<a name="25aae002"></a>
#### ①容器查看（使用左右键进入Containers Tab，然后使用上下键选择对应的容器，通过"["和"]"键进行切换显示容器的日志、状态、配置、内存信息）
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596525249288-503a0bb4-5f07-4b93-b988-d524fc037595.png#height=583&id=YX7oQ&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2116586&status=done&style=none&title=&width=1107.6666666666667)
<a name="mKN1z"></a>
#### ②容器管理（使用英文x键进入菜单，进行容器管理菜单）
可以进入容器的删除、重启、停止、日志查看等操作<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596526700353-f0e3d1af-e0f6-46c8-a716-227c6952be3b.png#height=583&id=tqqxA&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2150821&status=done&style=none&title=&width=1107.6666666666667)
<a name="86qqO"></a>
#### ③镜像信息查看
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596525068091-5d18a27f-3b06-4bf8-b84d-fe8f0e6957b2.png#height=583&id=LOVaA&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2222728&status=done&style=none&title=&width=1107.6666666666667)
<a name="ShkCA"></a>
#### ④镜像管理（使用英文x键进入菜单，进行容器管理菜单）
可以进行镜像的删除管理<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596526837888-40e79ff7-3b1d-4132-88da-9c279a90ee19.png#height=583&id=TV6A9&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2205650&status=done&style=none&title=&width=1107.6666666666667)
<a name="SeSP0"></a>
#### ⑤镜像与容器的挂载信息
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596515727499-e40aaa45-3405-41ca-a0c4-64f6b2ad37eb.png#height=593&id=JTy6A&originHeight=1780&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2122129&status=done&style=none&title=&width=1107.6666666666667)
<a name="t541W"></a>
### 键盘绑定
快捷键使用起来更方便
<a name="roaTD"></a>
#### Project
```bash
e: edit lazydocker config
o: open lazydocker config
[: previous tab
]: next tab
m: view logs
enter: focus main panel
```
<a name="CCzgV"></a>
#### Containers
```bash
[: previous tab
]: next tab
d: remove
e: Hide/Show stopped containers
s: stop
r: restart
a: attach
D: prune exited containers
m: view logs
c: run predefined custom command
enter: focus main panel
```
<a name="tDlwA"></a>
#### Services
```bash
d: remove containers
s: stop
r: restart
a: attach
m: view logs
[: previous tab
]: next tab
R: view restart options
c: run predefined custom command
enter: focus main panel
```
<a name="DcnAi"></a>
#### Images
```bash
[: previous tab
]: next tab
d: remove image
D: prune unused images
enter: focus main panel
```
<a name="ohj0H"></a>
#### Volumes
```bash
[: previous tab
]: next tab
d: remove volume
D: prune unused volumes
enter: focus main panel
```
<a name="KyEZk"></a>
#### Main
```bash
esc: return
```
<a name="HdQs4"></a>
# Docui
Github仓库地址：[https://github.com/skanehira/docui](https://github.com/skanehira/docui)<br />Docui也是一款UI终端，支持Mac、Linux操作系统。要求GO 1.11.4以上版本，Docker引擎在18.06.1以上，以及Git。<br />Docui是为了方便创建和配置新的容器/服务，可以在其中找到许多所有必要操作的键绑定。<br />可以使用镜像的搜索、保存导入、检查过滤等;容器的创建删除、启动停止、检查和重命名等;卷的创建和删除、检查和过滤，以及网络的删除等功能。
<a name="ug6fr"></a>
## 1.查看dockerui镜像
```bash
docker search dockerui
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1639982172301-e58bf388-a65a-442b-8349-2e7ca379e896.png#clientId=u7f8831d0-4b78-4&from=paste&height=435&id=u6749526e&originHeight=1306&originWidth=2504&originalType=binary&ratio=1&rotation=0&showTitle=false&size=262744&status=done&style=none&taskId=u85d0e7ac-b2d5-4cc7-8287-0e4e0be1a88&title=&width=834.6666666666666)
<a name="XmqM5"></a>
## 2.选择喜欢的dockerui风格镜像，下载
```bash
[root@localhost ~]# docker pull abh1nav/dockerui
```
<a name="mRbD0"></a>
## 3.启动dockerui容器，这里需要注意带上`privileged`参数，提升权限
```bash
[root@localhost ~]# docker run -d --privileged --name dockerui -p 9000:9000 -v /var/run/docker.sock:/var/run/docker.sock abh1nav/dockerui
```
前往网页查看之前，你需要打开服务器的9000端口：
```bash
firewall-cmd --permanent --zone=public --add-port=9000/tcpfirewall-cmd --reload
```
<a name="hsk8S"></a>
## 4.浏览器查看dockerui：http://192.168.2.119:9000   或者 `curl http://192.168.2.119:9000`
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639982046709-ef58f1ce-84b6-412f-81c0-96036c39ccdf.webp#clientId=u7f8831d0-4b78-4&from=paste&id=u5bdc40ea&originHeight=634&originWidth=831&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5adfbf78-b02d-4db7-8d96-4b2dac16444&title=)
<a name="rCJCC"></a>
# ctop
Github仓库地址：[https://github.com/bcicen/ctop](https://github.com/bcicen/ctop)<br />实现了类 top 命令展示效果的 docker 容器监控工具
<a name="BwMqz"></a>
## A.ctop官网
[ctop官网](https://github.com/bcicen/ctop)<br />[https://github.com/bcicen/ctop](https://github.com/bcicen/ctop)
<a name="22eeb05f"></a>
## B.ctop的安装
```bash
$ wget https://github.com/bcicen/ctop/releases/download/v0.7.3/ctop-0.7.3-linux-amd64 -O /usr/local/bin/ctop
$ chmod +x /usr/local/bin/ctop
```
`ctop`是 Docker 容器的顶级接口。它可以：

- 展示正在运行和已停止的容器列表。
- 展示统计信息，例如内存、CPU 使用率以及针对每个容器的其他详细信息窗口（例如绑定的端口等其他信息）。
- 提供快捷菜单，方便快速停止、杀掉指定容器进程或显示给定容器的日志。

这比从`docker ps`命令中找出所有这些信息要方便多了。
<a name="01d6f8ce"></a>
## C.ctop的参数Options
| Option | Description |
| --- | --- |
| `-a` | show active containers only |
| `-f <string>` | set an initial filter string |
| `-h` | display help dialog |
| `-i` | invert default colors |
| `-r` | reverse container sort order |
| `-s` | select initial container sort field |
| `-scale-cpu` | show cpu as % of system total |
| `-v` | output version information and exit |
| `-shell` | specify shell (default: sh) |

<a name="adad9369"></a>
## D.ctop的使用
<a name="b02qH"></a>
### ①查看容器的运行情况
```bash
[root@iZuligp6e1dyzfZ /]# ctop
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596522202474-310a7f22-9b27-4a89-bd07-442200a795cd.png#height=583&id=pWOfs&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1897177&status=done&style=none&title=&width=1107.6666666666667)
<a name="9RMoJ"></a>
### ②容器管理
在当前选中的容器按Enter，会显示容器管理菜单<br />[o] single view    视图查看<br />[l] log view         日志查看<br />[s] stop               停止容器的运行<br />[p] pause<br />[r] restart            容器重启<br />[e] exec shell       进入容器的Shell<br />[c] cancel            取消菜单<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596522804772-2204b88b-1875-4b99-82b8-ad07e4d80ea0.png#height=583&id=KJURO&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1921134&status=done&style=none&title=&width=1107.6666666666667)
<a name="YKGNi"></a>
#### 容器运行查看（使用上下键进行翻页，左右键或者Enter返回上一级）
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596523037282-132ec18b-c74d-4081-9dc6-969e21e7cb48.png#height=583&id=sizpy&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1953051&status=done&style=none&title=&width=1107.6666666666667)
<a name="34634ac9"></a>
#### 查看容器运行日志（使用上下，左右键或者Enter返回上一级）
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596523147595-2ba5071e-8051-4d87-9b27-9dd1bb8b334c.png#height=583&id=DgP8O&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2231659&status=done&style=none&title=&width=1107.6666666666667)
<a name="eLL1j"></a>
# Dive
Dive 是一个用 Go 语言编写的自由开源工具。Dive 工具不仅仅是一个 Docker 镜像分析工具，它还可以帮助使用者用于构建镜像。
<a name="oHbE9"></a>
## 运行截图
![2021-05-29-21-49-28-671544.gif](https://cdn.nlark.com/yuque/0/2021/gif/396745/1622296199087-0a99dbc8-bfce-474a-8c7f-738f4f15d961.gif#clientId=u1ad8d0b5-7326-4&from=ui&id=ue1343d49&originHeight=1083&originWidth=1734&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1910465&status=done&style=none&taskId=ud09a1aac-d132-4b26-b6fd-da99a26d52a&title=)<br />GitHub：[https://github.com/wagoodman/dive/blob/master/README.md](https://github.com/wagoodman/dive/blob/master/README.md)<br />安装说明：[https://github.com/wagoodman/dive/blob/master/README.md](https://github.com/wagoodman/dive/blob/master/README.md)<br />下载地址：[https://github.com/wagoodman/dive/releases](https://github.com/wagoodman/dive/releases)
<a name="cJmo1"></a>
# Shipyard
轻量级图形页面管理之Shipyard
```bash
docker search Shipyard
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1639982297767-552f1247-a5d4-4db6-859d-08d2fed957ed.png#clientId=u7f8831d0-4b78-4&from=paste&height=469&id=ud0d92c5d&originHeight=1406&originWidth=2503&originalType=binary&ratio=1&rotation=0&showTitle=false&size=263771&status=done&style=none&taskId=u9e3fd536-4c1b-47d9-9285-f5a8d1f00c3&title=&width=834.3333333333334)
