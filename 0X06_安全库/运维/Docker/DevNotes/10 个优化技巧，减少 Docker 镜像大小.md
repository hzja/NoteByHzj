Docker
<a name="c2wkw"></a>
## 什么是 Docker？
Docker 是一种容器引擎，可以在容器内运行一段代码。Docker 镜像是在任何地方运行应用程序而无需担心应用程序依赖性的方式。<br />要构建镜像，docker 使用一个名为 Dockerfile 的文件。Dockerfile 是一个包含许多指令（RUN、COPY、EXPOSE 等）的文件。成功执行这些命令后，docker 将创建一个镜像供我们在任何地方使用。
<a name="CNMvM"></a>
## 为什么要减小 Docker 镜像大小？

1. 安装不必要的软件包会增加攻击面，从而增加安全风险。
2. 镜像传输需要更多时间。
3. 部署大镜像需要更多时间。

必须以某种方式创建 Dockerfile，以便从该 Dockerfile 构建的镜像在大小方面得到优化。<br />在这里将讨论 10 种减少 docker 镜像大小的有效方法。
<a name="bTKRL"></a>
## 1、最小化镜像层
可以减少 Dockerfile 中的层数。<br />dockerfile 中的每个 FROM、RUN、COPY 命令都会创建一个单独的层，并增加镜像的整体大小和构建时间。<br />要减小 docker 镜像大小，请在单个 RUN 或 COPY 指令中执行多个命令来最小化 Dockerfile 中的层数。
```dockerfile
FROM ubuntu:latest
RUN apt update  - y
RUN apt install unzip - y
RUN apt install curl - y
RUN apt install python3 - y
```
与其对每个命令使用单独的指令，不如将它们组合起来：
```dockerfile
FROM ubuntu:latest
RUN apt update  -y && \
apt install unzip -y && \
apt install curl -y && \
apt install python3 -y
```
从下图中可以看出，通过减少层数，可以减少一些 MB 的大小。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676335307621-fd81f3ac-bcc2-4568-b939-17fdd5fea043.png#averageHue=%2344423a&clientId=u674e5e43-60f3-4&from=paste&id=uaf4c399d&originHeight=94&originWidth=794&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u26c84548-e6c6-41d3-8c32-9134efddea4&title=)
<a name="Jk1RL"></a>
## 2、使用 Docker Squash 减小镜像大小
Docker 在构建镜像时创建了很多层。压缩有助于在逻辑层中组织镜像。可以控制镜像的结构，而不是让镜像具有多个不必要的层。<br />可以使用以下命令安装 docker-squash。
```bash
pip install docker-squash
```
可以运行以下命令来减小镜像的大小。
```bash
docker-squash image:old -t image:new
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676335307720-7731edd0-7466-4b42-93b6-934f10f1acce.png#averageHue=%23060504&clientId=u674e5e43-60f3-4&from=paste&id=u56e4098a&originHeight=101&originWidth=1008&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uedde87da-df53-497a-be2f-9d576221907&title=)
<a name="VEe6v"></a>
## 3、使用较小的基础镜像
减小 docker 镜像大小最明显的方法是使用较小的基础镜像。<br />如果希望为 python 应用程序创建镜像，请考虑使用 python:3.9-slim 镜像而不是 python:3.9。<br />python:3.9 的大小约为 1.3 GB，而 python:3.9-slim 的大小仅为 1 GB 左右。<br />可以使用 alpine 版本进一步减少镜像。alpine 镜像是专门为作为容器运行而设计的，而且体积非常小。python:3.9-alpine 镜像只有 49 MB。
<a name="f2Ben"></a>
## 4、使用多阶段构建来减小大小
为了显着减小大小，可以使用 docker 多阶段构建的概念。这里使用不同的 images/Dockerfile 来构建和打包应用代码。<br />它将 Dockerfile 分成多个阶段，并将所需的工件从一个阶段传递到另一个阶段，然后在最后一个阶段交付缩小镜像大小的最终镜像。它显着减小镜像尺寸。
```dockerfile
# Official docker build image, Using  node:14.17-alpine3.14 image for stage-1.
# Stage-1
FROM node:14.17-alpine3.14 as build
# Copy Required files
COPY public /home/app/public/
COPY src /home/app/src/
# dockerfile install multiple packages
RUN apk add g++ make python2
RUN npm install --silent
# Create Build
RUN npm run build
RUN apk --purge del python2
#Run the build by copying the files form previous stage.
# Stage-2
FROM nginx:stable-alpine
COPY nginx.conf /etc/nginx/conf.d/default.conf
COPY --from=build /home/app/build /usr/share/nginx/html
EXPOSE 80
CMD ["nginx", "-g", "daemon off;"]
```
这里使用两个阶段从 docker 文件创建镜像。在 Stage-1 中，复制代码并构建它，在 stage-2 中，使用在 stage-1 中构建的代码在 Nginx 中运行。
<a name="JKLvG"></a>
## 5、apt 安装中使用 `--no-install-recommends` 标志
当运行 `apt install` 命令来安装某些包时，它会安装一些不需要的推荐包。使用 `--no-install-recommends` 标志可以显着减小镜像大小。
```dockerfile
FROM ubuntu:latest
RUN apt update -y && \
apt install unzip -y --no-install-recommends && \
apt install curl --no-install-recommends -y && \
apt install python3 -y --no-install-recommends
```
如下图所示，带有 new 标签的镜像由于添加了此标志而减少了 5MB。当要安装多个包时，这将非常有帮助。![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676335307640-6136c8ac-d016-4398-be47-8881dfbd4e16.png#averageHue=%230b0806&clientId=u674e5e43-60f3-4&from=paste&id=uddcc0dd0&originHeight=73&originWidth=785&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5faa1917-a828-4d58-9eae-3e91b4a98c9&title=)<br />可以在 `apk add` 命令中添加 `--no-cache`。
<a name="LN0pq"></a>
## 6、在 `apt install` 命令后添加 `rm -rf /var/lib/apt/lists/*`
可以在 `apt install` 之后添加这个命令来减少 docker 镜像的大小。
```dockerfile
FROM ubuntu:latest
RUN apt update -y && \
apt install unzip -y --no-install-recommends && \
apt install curl --no-install-recommends -y && \
apt install python3 -y --no-install-recommends && \
rm -rf /var/lib/apt/lists/*
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676335307863-73cc09c4-ca39-4747-870f-f94a51fb6f8f.png#averageHue=%230a0805&clientId=u674e5e43-60f3-4&from=paste&id=u03255f05&originHeight=98&originWidth=734&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua1a51bdc-025b-4618-a92b-15a2251fe75&title=)<br />从上图中可以看出，已将 docker 镜像的大小减少了约 41 MB。
<a name="SIFtD"></a>
## 7、使用 .dockerignore 文件
如果不想将某些文件复制到 docker 镜像，那么使用 .dockerignore 文件可以节省一些空间。<br />在构建上下文中有一些隐藏的文件/文件夹，可以使用 ADD 或 COPY 命令（如 .git 等）将其传输到镜像。包含一个 .dockerignore 文件以减小 docker 镜像大小是一个很好的做法。<br />.dockerignore 文件示例。
```
ignorethisfile.txt
logs/
ignorethisfolder/
.git
.cache
*.md
```
<a name="D52D7"></a>
## 8、在 RUN 之后放置 COPY
在某些情况下，对代码进行了细微的更改，并且需要反复从 dockerfile 构建镜像。<br />在这种情况下，将 COPY 命令放在 RUN 命令之后将有助于减小镜像大小，因为在这种情况下 docker 将能够更好地使用缓存功能。<br />它将为安装了依赖项的镜像创建缓存，每次更改代码时，docker 都会使用该缓存并创建镜像。它还将减少 docker 构建时间。
```dockerfile
#Dockerfile-1
FROM ubuntu:latest
RUN apt update -y && \
apt install unzip -y --no-install-recommends && \
apt install curl --no-install-recommends -y && \
apt install python3 -y --no-install-recommends && \
rm -rf /var/lib/apt/lists/*
COPY file /home/ubuntu
```
```dockerfile
#Dockerfile-2
FROM ubuntu:latest
COPY file /home/ubuntu
RUN apt update -y && \
apt install unzip -y --no-install-recommends && \
apt install curl --no-install-recommends -y && \
apt install python3 -y --no-install-recommends && \
rm -rf /var/lib/apt/lists/*
```
在上述情况下，dockerfile-1 将能够比 dockerfile-2 表现得更好。
<a name="eXOj8"></a>
## 9、安装后删除软件包
如果需要在 docker 镜像中安装一些包，并且是从外部下载它们，那么最好在安装后删除这些包。<br />例如，如果希望从 zip 文件安装 AWS CLI V2，那么在成功安装后请记住也删除该 zip 文件。
```dockerfile
FROM ubuntu:latest
RUN curl "https://awscli.amazonaws.com/awscli-exe-linux-x86_64.zip" -o "awscliv2.zip" && \
unzip awscliv2.zip && \
sudo ./aws/install && \
rm awscliv2.zip
```
<a name="OGpph"></a>
## 10、使用 Docker 镜像缩容工具
有几个**dockerfile 优化工具**可以减少 docker 镜像的大小。下面列出了其中一些。

1. **Dive**：Dive 是一个开源工具，用于探索 Docker 镜像及其层内容，然后发现缩小 Docker/OCI 镜像大小的方法。[https://github.com/wagoodman/dive](https://github.com/wagoodman/dive)
2. **fromlatest.io**：此工具将检查 Dockerfile 并检查可以执行的更多步骤以减小镜像大小。[https://www.fromlatest.io/](https://www.fromlatest.io/)
3. Docker Slim：它让你的容器更好、更小、更安全。可以使用**dockerslim 来最小化容器镜像**。[https://github.com/slimtoolkit/slim](https://github.com/slimtoolkit/slim)
