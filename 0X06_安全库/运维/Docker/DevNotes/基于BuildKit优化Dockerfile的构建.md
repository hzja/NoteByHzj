BuildKit DockerFile<br />Docker通过读取Dockerfile中的指令自动构建镜像，Dockerfile是一个文本文件，其中依次包含构建给定镜像所需的所有命令。<br />Dockerfile的使用非常重要，因为它是我们的蓝图，是添加到Docker镜像中的层的记录。<br />利用BuildKit功能，这是Docker v18.09上引入的一组增强功能。集成BuildKit将提供更好的性能，存储管理和安全性。
<a name="Q5KEl"></a>
### 目标
减少构建时间；<br />缩小镜像尺寸；<br />获得可维护性；<br />获得可重复性；<br />了解多阶段Dockerfile;<br />了解BuildKit功能。
<a name="OeFY9"></a>
### 先决条件

- Docker概念知识。
- 已安装Docker（当前使用v19.03）。
- 一个Java应用程序（在这里使用了一个Jenkins Maven示例应用程序）。
<a name="qs5W3"></a>
### 简单的Dockerfile示例
以下是一个包含Java应用程序的未优化Dockerfile的示例。将逐步进行一些优化。
```dockerfile
FROM debian
COPY . /app
RUN apt-get update
RUN apt-get -y install openjdk-11-jdk ssh emacs
CMD ["java", "-jar", "/app/target/my-app-1.0-SNAPSHOT.jar"]
```
在这里，可能会问自己：构建需要多长时间？为了回答这个问题，在本地开发环境上创建该Dockerfile，并让Docker构建镜像。
```bash
# enter your Java app folder
cd simple-java-maven-app-master
# create a Dockerfile
vim Dockerfile
# write content, save and exit
docker pull debian:latest # pull the source image
time docker build --no-cache -t docker-class . # overwrite previous layers
# notice the build time
0,21s user 0,23s system 0% cpu 1:55,17 total
```
此时，构建需要1m55s。<br />如果仅启用BuildKit而没有其他更改，会有什么不同吗？
<a name="SaqJL"></a>
### 启用BuildKit
BuildKit可以通过两种方法启用：<br />在调用`Docker build`命令时设置`DOCKER_BUILDKIT = 1`环境变量，例如：
```bash
time DOCKER_BUILDKIT=1 docker build --no-cache -t docker-class
```
将Docker BuildKit设置为默认开启，需要在/etc/docker/daemon.json进行如下设置，然后重启：
```bash
{ "features": { "buildkit": true } }
```
BuildKit最初的效果
```bash
DOCKER_BUILDKIT=1 docker build --no-cache -t docker-class .
0,54s user 0,93s system 1% cpu 1:43,00 total
```
此时，构建需要1m43s。在相同的硬件上，构建花费的时间比以前少了约12秒。这意味着构建几乎无需费力即可节约10％左右的时间。<br />现在看看是否可以采取一些额外的步骤来进一步改善。
<a name="KlHA5"></a>
### 从最小到最频繁变化的顺序
因为顺序对于缓存很重要，所以将`COPY`命令移到更靠近`Dockerfile`末尾的位置。
```dockerfile
FROM debian
RUN apt-get update
RUN apt-get -y install openjdk-11-jdk ssh emacs
RUN COPY . /app
CMD ["java", "-jar", "/app/target/my-app-1.0-SNAPSHOT.jar"]
```
<a name="jmSm6"></a>
### 避免使用“`COPY .`”
选择更具体的COPY参数，以避免缓存中断。仅复制所需内容。
```dockerfile
FROM debian
RUN apt-get update
RUN apt-get -y install openjdk-11-jdk ssh vim
COPY target/my-app-1.0-SNAPSHOT.jar /app
CMD ["java", "-jar", "/my-app-1.0-SNAPSHOT.jar"]CMD [“java”, “-jar”, “/app/my-app-1.0-SNAPSHOT.jar”]
```
<a name="ojhdW"></a>
### `apt-get update` 和`install`命令一起使用
这样可以防止使用过时的程序包缓存。
```dockerfile
FROM debian
RUN apt-get update && \
    apt-get -y install openjdk-11-jdk ssh vim
COPY target/my-app-1.0-SNAPSHOT.jar /app
CMD ["java", "-jar", "/my-app-1.0-SNAPSHOT.jar"]
```
<a name="G3VhA"></a>
### 删除不必要的依赖
在开始时，不要安装调试和编辑工具，以后可以在需要时安装它们。
```dockerfile
FROM debian
RUN apt-get update && \
    apt-get -y install --no-install-recommends \
    openjdk-11-jdk
COPY target/my-app-1.0-SNAPSHOT.jar /app
CMD ["java", "-jar", "/my-app-1.0-SNAPSHOT.jar"]
```
<a name="cfVAv"></a>
### 删除程序包管理器缓存
镜像不需要此缓存数据。借此机会释放一些空间。
```dockerfile
FROM debian
RUN apt-get update && \
    apt-get -y install --no-install-recommends \
    openjdk-11-jdk && \
    rm -rf /var/lib/apt/lists/*
COPY target/my-app-1.0-SNAPSHOT.jar /app
CMD ["java", "-jar", "/my-app-1.0-SNAPSHOT.jar"]
```
<a name="XateO"></a>
### 尽可能使用官方镜像
使用官方镜像有很多理由，例如减少镜像维护时间和减小镜像尺寸，以及预先配置镜像以供容器使用。
```dockerfile
FROM openjdk
COPY target/my-app-1.0-SNAPSHOT.jar /app
CMD ["java", "-jar", "/my-app-1.0-SNAPSHOT.jar"]
```
<a name="z00bD"></a>
### 使用特定标签
请勿使用latest标签。
```dockerfile
FROM openjdk:8
COPY target/my-app-1.0-SNAPSHOT.jar /app
CMD ["java", "-jar", "/my-app-1.0-SNAPSHOT.jar"]
```
<a name="epQH4"></a>
### 寻找最小的镜像
以下是openjdk镜像列表。选择最适合自己的最轻的那个镜像。<br />REPOSITORY TAG标签 SIZE大小

- openjdk 8 634MB
- openjdk 8-jre 443MB
- openjdk 8-jre-slim 204MB
- openjdk 8-jre-alpine 83MB
<a name="VwVhz"></a>
### 在一致的环境中从源构建
如果不需要整个JDK，则可以使用Maven Docker镜像作为构建基础。
```dockerfile
FROM maven:3.6-jdk-8-alpine
WORKDIR /app
COPY pom.xml .
COPY src ./src
RUN mvn -e -B package
CMD ["java", "-jar", "/my-app-1.0-SNAPSHOT.jar"]
```
<a name="zurWk"></a>
### 在单独的步骤中获取依赖项
可以缓存–用于获取依赖项的Dockerfile命令。缓存此步骤将加快构建速度。
```dockerfile
FROM maven:3.6-jdk-8-alpine
WORKDIR /app
COPY pom.xml .
RUN mvn -e -B dependency:resolve
COPY src ./src
RUN mvn -e -B package
CMD ["java", "-jar", "/my-app-1.0-SNAPSHOT.jar"]
```
<a name="WI7vi"></a>
### 多阶段构建：删除构建依赖项
为什么要使用多阶段构建？<br />将构建与运行时环境分开<br />DRY方式<br />具有开发，测试等环境的不同详细信息<br />线性化依赖关系<br />具有特定于平台的阶段
```dockerfile
FROM maven:3.6-jdk-8-alpine AS builder
WORKDIR /app
COPY pom.xml .
RUN mvn -e -B dependency:resolve
COPY src ./src
RUN mvn -e -B package

FROM openjdk:8-jre-alpine
COPY --from=builder /app/target/my-app-1.0-SNAPSHOT.jar /
CMD ["java", "-jar", "/my-app-1.0-SNAPSHOT.jar"]
```
如果此时构建应用程序，
```bash
time DOCKER_BUILDKIT=1 docker build --no-cache -t docker-class .
0,41s user 0,54s system 2% cpu 35,656 total
```
应用程序构建需要大约35.66秒的时间。这是一个令人愉快的进步。<br />下面介绍其他场景的功能。
<a name="YPF8f"></a>
### 多阶段构建：不同的镜像风格
下面的Dockerfile显示了基于Debian和基于Alpine的镜像的不同阶段。
```dockerfile
FROM maven:3.6-jdk-8-alpine AS builder
…
FROM openjdk:8-jre-jessie AS release-jessie
COPY --from=builder /app/target/my-app-1.0-SNAPSHOT.jar /
CMD [“java”, “-jar”, “/my-app-1.0-SNAPSHOT.jar”]

FROM openjdk:8-jre-alpine AS release-alpine
COPY --from=builder /app/target/my-app-1.0-SNAPSHOT.jar /
CMD ["java", "-jar", "/my-app-1.0-SNAPSHOT.jar"]
```
要构建特定的镜像，可以使用–target参数：
```bash
time docker build --no-cache --target release-jessie .
```
<a name="nKv1m"></a>
### 不同的镜像风格（DRY /全局ARG）
```dockerfile
ARG flavor=alpine
FROM maven:3.6-jdk-8-alpine AS builder
…
FROM openjdk:8-jre-$flavor AS release
COPY --from=builder /app/target/my-app-1.0-SNAPSHOT.jar /
CMD ["java", "-jar", "/my-app-1.0-SNAPSHOT.jar"]
```
ARG命令可以指定要构建的镜像。在上面的例子中，指定alpine为默认的镜像，但也可以在`docker build`命令中，通过`–build-arg flavor=参数指定镜像`。
```bash
time docker build --no-cache --target release --build-arg flavor=jessie .
```
<a name="Auu1O"></a>
### 并发
并发在构建Docker镜像时很重要，因为它会充分利用可用的CPU线程。在线性Dockerfile中，所有阶段均按顺序执行。通过多阶段构建，可以让较小的依赖阶段准备就绪，以供主阶段使用它们。<br />BuildKit甚至带来了另一个性能上的好处。如果在以后的构建中不使用该阶段，则在结束时将直接跳过这些阶段，而不是对其进行处理和丢弃。<br />下面是一个示例Dockerfile，其中网站的资产是在一个assets阶段中构建的：
```dockerfile
FROM maven:3.6-jdk-8-alpine AS builder
…
FROM tiborvass/whalesay AS assets
RUN whalesay “Hello DockerCon!” > out/assets.html

FROM openjdk:8-jre-alpine AS release
COPY --from=builder /app/my-app-1.0-SNAPSHOT.jar /
COPY --from=assets /out /assets
CMD ["java", "-jar", "/my-app-1.0-SNAPSHOT.jar"]
```
这是另一个Dockerfile，其中分别编译了C和C ++库，并在builder以后使用该阶段。
```dockerfile
FROM maven:3.6-jdk-8-alpine AS builder-base
…

FROM gcc:8-alpine AS builder-someClib
…
RUN git clone … ./configure --prefix=/out && make && make install

FROM g++:8-alpine AS builder-some CPPlib
…
RUN git clone … && cmake …

FROM builder-base AS builder
COPY --from=builder-someClib /out /
COPY --from=builder-someCpplib /out /
```
<a name="MahFF"></a>
### BuildKit应用程序缓存
BuildKit具有程序包管理器缓存的特殊功能。以下是一些缓存文件夹位置的示例：<br />包管理器 路径
```bash
apt /var/lib/apt/lists
go ~/.cache/go-build
go-modules $GOPATH/pkg/mod
npm ~/.npm
pip ~/.cache/pip
```
可以将此Dockerfile与上面介绍的在一致的环境中从源代码构建中介绍的Dockerfile进行比较。这个较早的Dockerfile没有特殊的缓存处理。可以使用`–mount=type=cache`来做到这一点。
```dockerfile
FROM maven:3.6-jdk-8-alpine AS builder
WORKDIR /app
RUN --mount=target=. --mount=type=cache,target /root/.m2 \
    && mvn package -DoutputDirectory=/

FROM openjdk:8-jre-alpine
COPY --from=builder /app/target/my-app-1.0-SNAPSHOT.jar /
CMD ["java", "-jar", "/my-app-1.0-SNAPSHOT.jar"]
```
<a name="dhjgs"></a>
### BuildKit的安全功能
BuildKit具有安全功能，下面的示例中，使用了`–mount=type=secret`隐藏了一些机密文件，例如~/.aws/credentials。
```dockerfile
FROM <baseimage>
RUN …
RUN --mount=type=secret,id=aws,target=/root/.aws/credentials,required \
./fetch-assets-from-s3.sh
RUN ./build-scripts.sh
```
要构建此Dockerfile，需要使用`–secret`参数：
```bash
docker build --secret id=aws,src=~/.aws/credentials
```
还有为了提高安全性，避免使用诸如`COPY ./keys/private.pem /root .ssh/private.pem`之类的命令，可以使用BuildKit中的ssh解决此问题：
```dockerfile
FROM alpine
RUN apk add --no-cache openssh-client
RUN mkdir -p -m 0700 ~/.ssh && ssh-keyscan github.com >> ~/.ssh/known_hosts
ARG REPO_REF=19ba7bcd9976ef8a9bd086187df19ba7bcd997f2
RUN --mount=type=ssh,required git clone git@github.com:org/repo /work && cd /work && git checkout -b $REPO_REF
```
要构建此Dockerfile，需要在ssh-agent中加载到SSH私钥。
```bash
eval $(ssh-agent)
ssh-add ~/.ssh/id_rsa # this is the SSH key default location
docker build --ssh=default .
```
<a name="u9HDA"></a>
### 结论
介绍了使用Docker BuildKit优化Dockerfile，并因此加快了镜像构建时间。这些速度的提高，可以提高效率和节省计算能力。
