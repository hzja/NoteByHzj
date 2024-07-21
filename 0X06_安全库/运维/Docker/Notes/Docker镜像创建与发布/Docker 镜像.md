Docker<br />Docker 镜像，已经是 IT 工程师工作中，不可或缺的一部分，可以说是工作的基础，但是 docker 镜像本质上，到底是什么东西？生成一个 docker 镜像到底做了什么操作呢？
<a name="n8Inn"></a>
## 1、Dockerfile 生成 docker 镜像
如想要生成一个自己的 docker 镜像，可以先编写自己的 dockerfile 文件，然后基于此文件使用 docker build 生成镜像，那这个过程中到底发生了什么呢？
<a name="NyQXu"></a>
### 发送 Build context
在执行 `docker build` 命令时，会在末尾加上一个 “.”，这个点就是 docker 的构建上下文，在 linux 下 “.”即代表当前目录；docker 构建镜像需要使用到构建上下文里的文件，所以需要将 build context 下的文件遍历发送给 docker 守护进程，这样就可以在构建开始的日志信息中，看到如下信息：
> Sending build context to Docker daemon  xxx.xx MB 

这条信息是在告知需要发送给 docker daemon 的文件有多少 MB 大小。
<a name="OSWDX"></a>
### 校验 dockerfile 命令并执行
Docker daemon 在执行 dockerfile 的命令之前都会先预校验一下命令是不是符合语法，不符合的将返回一个错误；命令都没问题后 docker daemon 会逐条执行命令，执行的过程如下：在原来的镜像上启动一个容器，在容器内执行命令，执行完写操作，然后 docker daemon 执行一次 commit，提交一个新的镜像，这里就产生了新的一层镜像，紧接着继续执行下面的命令，直至执行完成生成镜像。
<a name="quCiX"></a>
## 2、Docker镜像的结构
按照 OCI (Open Container Initiative) 规范中的容器镜像标准，Docker 镜像其实本质是文件目录，包括 index 索引文件 (可选) 、配置文件、清单文件、一组文件系统层。配置文件里包含环境变量、挂载卷、暴露的端口等；清单文件中列出了构成镜像的层的信息。可以通过 `docker manifest inspect` 命令来查看镜像的配置文件、清单文件里的信息。<br />下面就来详细看一看：
<a name="LKbWM"></a>
### (1) index 索引文件
以下是查看一个 openjdk 镜像元数据信息的示例：
```bash
docker manifest inspect openjdk:8-alpine
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650814450832-c2d2821e-c6e1-49da-8385-181a242beaec.png#clientId=udb266188-3d2c-4&from=paste&id=u98f5dfe2&originHeight=482&originWidth=900&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u42bd20d1-b11c-43a3-9012-50684055f1f&title=)<br />以上 JSON 文件就是一个镜像的 index 索引文件，这个文件的作用是标记不同的平台该使用哪个镜像 manifest 文件，其中的 digest 字段就是 openjdk 镜像的 manifest 文件 ID，可以根据这个文件 ID (文件指纹) 来查看manifest 文件信息。
<a name="WdBC0"></a>
### (2) 清单文件
以下是查看清单文件的示例：
```bash
docker manifest inspect openjdk@sha256:44b3cea369c947527e266275cee85c71a81f20fc5076f6ebb5a13f19015dce71
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650814450784-063ac2f2-7dc5-4dea-b87b-37018a587011.png#clientId=udb266188-3d2c-4&from=paste&id=u822a996d&originHeight=525&originWidth=900&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uce99c207-abd7-4891-84a4-5efeb780c5e&title=)<br />从上图可以看出 manifest 文件中由配置文件和很多的层信息组成，每个层信息包含类型、文件ID (文件指纹) 、大小信息。拉取镜像时先获得 manifest 文件，再根据此文件中的元数据信息，去获取相应的配置文件、层文件，要注意的是获取层文件时，会对比文件指纹。但如果本地已存在层文件，就不需要再拉取了，直接使用本地缓存，推送镜像时也是如此，如果在 docker registry 中已存在了层数据，不需再次推送，只会推送改变了的层。
<a name="yMyeX"></a>
### (3) 配置文件
配置文件的位置在以下目录：<br />/var/lib/docker/image/overlay2/imagedb/content/sha256 <br />而 manifest 文件中的 config.digest 字段就是配置文件的名称，查看文件的内容：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650814450798-c653e500-00a6-4c49-8478-4d33b8a8ecc7.png#clientId=udb266188-3d2c-4&from=paste&id=u1f476405&originHeight=478&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7f4db276-94f5-4e47-b43b-3bd00e98d74&title=)<br />配置文件中配置了容器运行时，需要的环境变量、入口命令、挂载卷等信息。<br />至此，已经了解了 **docker 镜像的分层结构**，那分层结构有哪些好处呢？**主要好处在于资源可共享、可复用**，当多个镜像间存在相同的层时可直接复用，无需再拉取，**极大地节约了存储、网络带宽资源**。
<a name="Dio1K"></a>
## **3**、Dockerfile最佳实践
通过以上内容，可以知道，一个 docker 镜像产生和dockerfile 里的命令密不可分，因此 dockerfile 不同的写法会影响到镜像的大小、构建的速度等。Docker 官方有 dockerfile 编写的最佳实践 (网址如下)。<br />[https://docs.docker.com/develop/develop-images/dockerfile_best-practices/](https://docs.docker.com/develop/develop-images/dockerfile_best-practices/)<br />它建议从以下几点优化：

- 清晰当前的构建上下文，只包含需要的资源，排除其余多余的资源，可以使用 `docker build -f xx/Dockerfile` xx/context 来指定 Dockerfile 和构建上下文。
- 使用 .dockerignore 文件排除不需要的文件，只保留需要的，保证构建上下文尽可能的小。
- 使用多阶段构建，由于镜像是在构建过程的最后阶段构建生成的，因此可以将初始化构建镜像所需要的环境、依赖项等放在开始阶段，最后阶段只需要获取构建产物、添加入口命令，这样就得到了一个单层的镜像。
- 尽量减少镜像的层数，其中 RUN、COPY、ADD 命令会创建新的层，多个 RUN 命令时，可以检查是否可以合并成一条命令。

以上是一些常用的优化配置，更多详细的配置可查询 docker 官网上的 dockerfile 最佳实践。
<a name="gzpOq"></a>
## 4、如何提高构建 docker 镜像的速度
到此已梳理了一个 docker 镜像产生的过程、一个docker镜像的结构、以及如何编写一个较好的 dockerfile。那如何提高 docker 镜像构建的速度呢，可以从以下几点出发：
<a name="WTw6u"></a>
### 1. 构建上下文
构建上下文是构建镜像的基础，最好只包含 `docker build` 构建过程需要的资源，这里可以参考 dockerfile 的最佳实践做出优化。
<a name="BFd1w"></a>
### 2. Base image
在构建镜像过程中，自己的程序其实是很小的，但程序运行的环境占了很大的空间，所以可以选择比较小巧的底包，这样可以大大减小镜像的大小，从而缩短镜像构建过程中拉取和推送的时间；也可以选择更小巧的 Linux 发行版，比如 Alpine，或者 Google 的 Distroless 镜像。<br />**Alpine **是一个小巧、安全、简单、功能完备的 linux 发行版，大小只有几 M，非常适合用于制作镜像，现在很多官方镜像都已经有 Alpine 的版本了，使用 Alpine 版本的 base image 可以极大减小构建出来的镜像大小。<br />**Distroless **是 Google 的一个镜像构建文件，专门在安全漏洞方面做了优化，只包含应用程序及其运行时所需的依赖，不包含软件包管理器、shell 和其他 GNU 二进制文件这些几乎用不到的功能，大大降低了被攻击的风险，并减少了漏洞，所以 Distroless 较 Alpine 更加的安全，不过 gcr.io 对国内用户稍微不是很友好，镜像拉取不了。
<a name="AZWNA"></a>
### 3. 其他优秀的镜像分层技术
镜像的结构如果是满足 OCI 的标准规范的话，就可以在 OCI 的运行时中运行；换句话说只要能构建出满足 OCI 标准的镜像文件目录，就是一个标准的 docker 镜像；现在也有了很多优秀的镜像分层技术，他们满足 OCI 标准，并且解决了 docker 的一些缺点；合理的分层，可以使构建过程使用上大量的缓存，无需重复拉取，从而加快镜像的构建，下面看看一些比较流行的镜像分层技术：
<a name="uFs5i"></a>
#### Podman
Podman 提供与 Docker 非常相似的功能。可以说podman就是为了替代 docker 的，podman 解决 docker 的一些痛点，比如 docker daemon 是一个守护进程、并需要 root 权限，但 podman 它不需要在系统上运行任何守护进程，并且还可以在没有 root 权限的情况下运行。<br />Podman 可以管理和运行任何符合 OCI 规范的容器和容器镜像；podman 的命令和 docker 的命令，基本上是相同的，只需要将 docker 换为 podman，即可兼容 docker 的基本常用命令，podman 也可以根据用户提供的 dockerfile 文件构建镜像，不过一般不推荐使用 podman build 构建镜像，因为 podman 构建速度超慢，并且默认情况下使用 vfs 存储驱动程序会耗尽大量磁盘空间，一般使用 podman 的构建工具 Buildah。
<a name="bAppg"></a>
#### Buildah
Buildah 是一个专注于构建 OCI 容器镜像的工具，Buildah 构建速度非常快并使用覆盖存储驱动程序，可以节约大量的空间。<br />Buildah 也支持使用 dockerfile 构建镜像：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650814450852-46f4a487-e000-4dc4-9762-bb14fc5566e9.png#clientId=udb266188-3d2c-4&from=paste&id=u68469f86&originHeight=341&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u916cee3b-c84d-40eb-8444-7493479645f&title=)<br />Buildah 使用 dockerfile 构建时是在构建的最后一步进行的 commit，这样构建的镜像就只有一层，无法使用到缓存，也就是要做一些重复的拉取工作；如果使用 buildah 的原生命令构建镜像的话，分层会变得更加的灵活，可以自定义缓存点，在认为需要缓存的地方加上 commit 命令就能提交一层新的镜像。Buildah 的原生命令就是一个 bash 脚本，下面展示了 buildah 构建的一个简单脚本：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650814450789-aeaf4cce-481c-461d-9c2a-02ca1b0e7a7d.png#clientId=udb266188-3d2c-4&from=paste&id=u6a742ff8&originHeight=445&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1f233a33-e5e1-49f0-bb41-5d737fdb448&title=)<br />当不使用 Dockerfile 而是使用 Buildah 命令构建镜像时，可以使用 commit 命令来随时决定提交缓存。在上例中，所有的变更是一起提交的；但其实可以增加中间提交，这样就能自由标记 缓存点 (cache point)：例如，可以在安装完一些构建需要的工具后就提交一次，这样下一次构建可以直接使用这个缓存。<br />Buildah mount 命令可以将容器的根目录挂载到主机的一个挂载点上；这使得可以使用主机上的工具进行构建和安装软件，不用将这些构建工具打包到容器镜像本身中。
<a name="IptGA"></a>
#### Google 的 jib 构建工具
Jib 是 Google 的一个 java 构建镜像的工具，将原来一层docekr 镜像拆分得更细，在原来 COPY springboot 项目的 jar 文件构建成一个 java 的镜像，但是 springboot 的 jar 压缩文件中，有很多的文件在每次编译时，基本上是不会改变的，比如说第三方的依赖 jar、以及资源文件夹、配置文件等，改变代码后编译出的文件中只有 class 文件是不一样的，所以 jib 将 springboot 项目分为三层，分别为第三方依赖 lib、资源文件 resources、字节码 class 文件，体现在 dockerfile 文件里就如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650814451105-b2134bf2-d6ef-4809-bdec-314c1fab828d.png#clientId=udb266188-3d2c-4&from=paste&id=u59069a65&originHeight=224&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7d3dde4a-3055-4d68-b6ad-a3f2dc9d475&title=)<br />前两层在大多数情况下，都是可以复用的，仅仅需要构建最后一层即可，越是大型的项目越能体现出 jib 的优势。<br />下面看一下它们的优缺点：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1650814451121-d4f25a56-443c-49a6-862a-2d88b8f33fca.jpeg#clientId=udb266188-3d2c-4&from=paste&id=u752bfca7&originHeight=788&originWidth=1068&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5c0b7e39-2c69-40cb-9f44-e6e8e71148c&title=)<br />镜像分层技术现在已经是很普遍的存在，除了上述提到的外，还有谷歌的 Kaniko、Buildkit、Source-To-Image (S2I)、Bazel 等，它们都有各自的一些特性，使用于一些特定的场景，可以视情况选择使用。
<a name="lgzos"></a>
#### 项目中的实践 - jib
目前「DaoCloud 道客」有些项目中已使用 jib 作为 JAVA项目的镜像构建工具，明显能感觉到对项目开展效率的提升：

1. **更方便地构建、调试。**Jib 可以在仅有 JAVA 的环境下完成镜像构建，可以在本地构建镜像到远程仓库，而不触发 CICD，这样便于更快速地进行线上程序联调。
2. **配置更简洁。**不需要写 dockerfile，更不需要考虑dockerfile的最佳实践，只需要简单的几行配置即可。
3. **构建速度更快。**即使进行 CICD 构建，jib 对镜像的特殊分层，也会让构建过程中使用到更多的缓存，受低带宽的影响也会更小。
<a name="sgTvT"></a>
## 总结
得益于 OCI 规范的存在，只要构建出的镜像遵守 OCI 规范，就可以交给遵守 OCI 规范的容器运行时去运行，这样就使得容器技术的发展，更加多元化，也不必再拘泥于一款工具使用，可以按需选择那些能提高工作效率的工具。
