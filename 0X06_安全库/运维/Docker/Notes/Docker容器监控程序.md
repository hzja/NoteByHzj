Linux CentOS Docker 
<a name="wSGGd"></a>
## 1、watchtower：自动更新 Docker 容器
Watchtower 监视运行容器并监视这些容器最初启动时的镜像有没有变动。当 Watchtower 检测到一个镜像已经有变动时，它会使用新镜像自动重新启动相应的容器。
<a name="S4Lyn"></a>
### A.watchtower文档与Github
[https://github.com/v2tec/watchtower/blob/master/README.md](https://github.com/v2tec/watchtower/blob/master/README.md)<br />GitHub 地址：[https://github.com/v2tec/watchtower](https://github.com/v2tec/watchtower)
<a name="949b4fdb"></a>
### B.watchtower的使用
Watchtower 本身被打包为 Docker 镜像，因此可以像运行任何其他容器一样运行它。要运行 Watchtower，需要执行以下命令：
```bash
$ docker run -d --name watchtower --rm -v /var/run/docker.sock:/var/run/docker.sock  v2tec/watchtower --interval 30
```
在上面的命令中，使用一个挂载文件 /var/run/docker.sock 启动了 Watchtower 容器。这么做是有必要的，为的是使 Watchtower 可以与 Docker 守护 API 进行交互。将 30 秒传递给间隔选项 interval。此选项定义了 Watchtower 的轮询间隔。Watchtower 支持更多的选项，可以根据文档中的描述来使用它们。<br />现在启动一个 Watchtower 可以监视的容器。
```bash
$ docker run -p 4000:80 --name friendlyhello fcant/SpringBoot:latest
```
现在，Watchtower 将开始温和地监控这个 friendlyhello 容器。当新镜像推送到 Docker Hub 时，Watchtower 在接下来的运行中将检测到一个新的可用的镜像。它将优雅地停止那个容器并使用这个新镜像启动容器。它将传递之前传递给这条 run 命令的选项。换句话说，该容器将仍然使用 4000:80 发布端口来启动。<br />默认情况下，Watchtower 将轮询 Docker Hub 注册表以查找更新的镜像。通过传递环境变量 REPO_USER 和 REPO_PASS 中的注册表凭据，可以将 Watchtower 配置为轮询私有注册表。
<a name="8YbYh"></a>
## 2、docker-gc：容器和镜像的垃圾回收
Docker-gc 工具通过删除不需要的容器和镜像来清理 Docker 主机。它会删除存在超过一个小时的所有容器。此外，它还删除不属于任何留置容器的镜像。<br />可以将 docker-gc 作为脚本和容器来使用。
<a name="Vht1l"></a>
### A.docker-gc的文档与Github
[https://github.com/spotify/docker-gc/blob/master/README.md](https://github.com/spotify/docker-gc/blob/master/README.md)<br />GitHub 地址：[https://github.com/spotify/docker-gc](https://github.com/spotify/docker-gc)
<a name="45D8F"></a>
### B.docker-gc的使用
以容器的形式运行 docker-gc。若要使用 docker-gc 来查找所有可以删除的容器和镜像，命令如下：
```bash
$ docker run --rm -v /var/run/docker.sock:/var/run/docker.sock -e DRY RUN=1 spotify/docker-gc
```
上述命令中，加载了 docker.sock 文件，以便 docker-gc 能够与 Docker API 交互。传递了一个环境变量 DRY_RUN=1 来查找将被删除的容器和镜像。如果不提供该参数，docker-gc 会删除所有容器和镜像。最好事先确认 docker-gc 要删除的内容。<br />如果认同 docker-gc 清理方案， 可以不使用 DRY_RUN 再次运行 docker-gc 执行清空操作。
```bash
$ docker run --rm -v /var/run/docker.sock:/var/run/docker.sock spotify/docker-gc
```
<a name="nO8jL"></a>
## 3、docker-slim：面向容器的瘦身工具
如果担心Docker 镜像的大小，docker-slim 可以排忧解难。<br />docker-slim 工具使用静态和动态分析方法来为臃肿的镜像瘦身。要使用 docker-slim，可以从 Github 下载 Linux 或者 Mac 的二进制安装包。成功下载之后，将它加入到系统变量 PATH 中。
<a name="sa5t5"></a>
### A.docker-slim的文档以及Github
[https://github.com/docker-slim/docker-slim/blob/master/README.md](https://github.com/docker-slim/docker-slim/blob/master/README.md)<br />GitHub 地址：[https://github.com/docker-slim/docker-slim](https://github.com/docker-slim/docker-slim)
<a name="0OW1G"></a>
### B.docke-slim的使用
```bash
$ docker-slim build --http-probe SpringBoot
```
<a name="V1p6P"></a>
## 4、rocker：突破 Dockerfile 的限制
大多数使用 Docker 的开发人员都使用 Dockerfile 来构建镜像。Dockerfile 是一种声明式的方法，用于定义用户可以在命令行上调用的所有命令，从而组装镜像。
<a name="1db54828"></a>
### A.rocker的文档与Github
Rocker（[https://github.com/grammarly/rocker](https://github.com/grammarly/rocker)）为 Dockerfile 指令集增加了新的指令。<br />GitHub 地址：[https://github.com/grammarly/rocker](https://github.com/grammarly/rocker)
<a name="GoRQL"></a>
### B.Mac安装rocker
```bash
$ brew tap grammarly/tap
$ brew install grammarly/tap/rocker
```
<a name="BiNqy"></a>
### C.rocker的使用
安装完成，就可以通过传递 Rockerfile 使用 Rocker 来构建镜像
```bash
FROM python:2.7-slim
WORKDIR /app
ADD . /app
RUN pip install -r requirements. txt
EXPOSE 80
ENV NAME World
CMD ["python","app.Py"]
TAG shekhargulati/ hello:{{ .VERSION }}
PUSH shekhargulati/hello:{{ .VERSION }}
```
构建一个镜像并将其推送到 Docker Hub，可以运行以下命令：
```bash
$ rocker d build --push -var VERSION-1.0
```
