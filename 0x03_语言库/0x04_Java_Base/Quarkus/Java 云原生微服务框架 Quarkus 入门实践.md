JavaQuarkus
<a name="xBG5x"></a>
## 1、概述
<a name="GgPun"></a>
### 1.1 定义
Quarkus定位要做超声速、亚原子的Java框架，使用最好标准为OpenJDK HotSpot和GraalVM量身定制的Kubernetes Native Java栈；从一开始就是针对Kubernetes设计的云原生优化Java应用开发框架;兼容主流的框架开发模式如Jpa、Netty、Undertow、Hibernate、JWT、Spring。最新版本为2.13.3。

- _Quarkus 官网：_[_https://quarkus.io/_](https://quarkus.io/)
- _Quarkus GitHub：_[_https://github.com/quarkusio/quarkus_](https://github.com/quarkusio/quarkus)
<a name="sR1Zp"></a>
### 1.2 GraalVM 简介
GraalVM 是 Oracle 开发的高性能的多语言运行时 JDK，旨在提高 Java 应用程序的性能，同时消耗更少的资源。<br />GraalVM 提供了两种运行Java应用程序的方法：在HotSpot JVM上使用 Graal 即时（JIT）编译器或者作为预先（AOT）编译的本机可执行文件。<br />除了 Java，它还提供了 JavaScript、Ruby、Python 和其他一些流行语言的运行时。GraalVM 的多语言功能使得在一个应用程序中混合编程语言成为可能，同时消除了任何外语调用成本。目前最新版本为 22.3。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667524423786-48ab1db2-fecf-4bfd-bc10-ab0bf79ce57f.png#clientId=ub21bce19-f0ee-4&from=paste&id=u0bdca6a8&originHeight=538&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u70f2d3e3-6e27-4658-bfd8-76518e2e018&title=)

- GraalVM 可以创建原生可执行文件；
- GraalVM 提供的原生镜像（native image）功能可以把Java代码预先编译（Ahead-Of-Time，AOT）成独立的可执行文件。该可执行文件包括了应用本身的代码、所依赖的第三方库和 JDK 本身。该执行文件并不运行在 Java 虚拟机之上，而是名为 Substrate 的虚拟机。与运行在传统的 Java 虚拟机上相比，原生可执行文件在运行时的启动速度更快，所耗费的内存资源更少。可执行文件的体积也更小；
- GraalVM 生成的原生可执行文件与底层平台相关，不能在当前平台之外的其他平台上运行。但对云原生应用来说，这并不是一个问题。云原生应用的设计目标是在容器中运行，所运行的底层平台是固定的。
<a name="OcCkB"></a>
### 1.3 为何使用
使用 Spring Boot 的快速开发企业级应用的微服务启动慢，从一个应用从启动到服务可用，一般是数秒。在对 CPU 核数和内存严格限制的情况下，花数十秒启动的情况也屡见不鲜。这样的问题无法适应需要快速重启或快速扩容的场景。不仅如此，内存消耗也很大。如果限制在 1G 时 FullGC 频率变大，且经常触发 OMM 后 Kill 导致 pod 重启，并且启动时间也会变长。<br />相比之下，集群中使用 GO 语言开发的应用则配置 128M 内存也可正常使用。以 JVM 模式运行的 Java 应用，并不太适合于云原生应用的开发，而拥抱云原生则是未来技术的大趋势。<br />容器云已经为未来主流，也即是软件都是运行在 K8S 这样的容器集群里。而容器环境需要应用具备启动速度快，资源占用小，响应时间短等特性。Quarkus 顺应这种趋势而生的。

- 对于云原生应用来说，平台无关性变得无关紧要。云原生应用都是以容器化的形式运行的，所运行的底层平台是固定的；
- 云原生应用对启动速度的要求比较高。当需要进行水平扩展时，要求这些新的实例必须在足够短的时间内完成启动，从而尽快的处理新增的请求；
- 云原生应用要求在运行时占用尽可能少的资源。尽可能的减少单个实例占用的资源，就意味着可以用同样的成本，支持更多的访问请求；
- 云原生应用要求更小的打包体积。云原生应用以容器镜像的形式打包。应用镜像的尺寸越大，所需要的存储空间也会越大，推送和拉取镜像所耗费的时间也会更长。

Quarkus具备企业级应用开发能力。<br />在 Serverless 服务器架构、微服务、容器、Kubernetes、功能即服务（FaaS）和云环境中运行 Java 而言，考虑了所有这些因素的 Quarkus 堪称是一个有效的解决方案。
<a name="HcT7v"></a>
### 1.4 特性
<a name="SGCBY"></a>
#### 容器优先
构建期即生成云原生镜像执行文件。<br />Quarkus 为 GraalVM 和 HotSpot 定制应用程序。快速启动、低内存消耗、体积小，在 Kubernetes 这样的容器编排平台上提供了近乎实时的扩展和高密度的内存利用率。这就是使用了编译时引导的技术。

- 支持 Graal/SubstrateVM
- 构建时元数据处理
- 尽量减少 JNI 调用
- 减少反射的使用
- 本机映像预启动
<a name="iuEsN"></a>
#### Kubernete 原生
构建期即生成云原生镜像执行文件，可以与 Docker 和 Kubernetes 轻松集成，Quarkus 和 Kubernetes 的组合可以伸缩、快速创建轻量级的应用程序。Quarkus 通过工具、预构建的集成、应用程序服务等显著提高了开发人员的工作效率。
<a name="w6vro"></a>
#### 命令式和响应式
在设计上 Quarkus 能够在开发应用时无缝地结合熟悉的命令式代码和非阻塞、响应式样式。这对于习惯使用命令式模型而不想切换风格的 Java 开发人员以及使用云原生/响应式方法的开发人员都非常有用。
<a name="u9vBX"></a>
#### 社区和标准
拥抱 JavaEE 标准，使用 JavaEE 官方 RESTful、CDI 等标准接口。Quarkus 提供了一个内聚的、轻量的全栈框架，包含超过 50 个使用的最佳类库。
<a name="rYytZ"></a>
#### 对开发者友好
学习成本也低，具有统一的配置和简单的本地可执行文件生成，零配置，实时重新加载，80% 的常用标准和 20% 灵活应用。
<a name="TYFjz"></a>
### 1.5 官方性能数据
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667524423780-bf438c09-576d-4686-8b28-87c4e95be729.png#clientId=ub21bce19-f0ee-4&from=paste&id=u5ebe3583&originHeight=553&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u82c0e798-0cca-44bc-84c2-a6555340a04&title=)<br />从图中可以看出，使用 Quarkus 和 GraalVM 的简单的 REST 应用的启动时间仅为 16 毫秒，占用内存仅 12MB。如果使用传统的基于 Java 虚拟机实现，应用的启动时间需要 4.3 秒，占用内存为 136MB。
<a name="k6fq7"></a>
## 2、实战
入门示例项目步骤：

- 安装 GraalVM
- 创建 Quarkus工程
- IDEA 导入项目
- IDEA 编码运行和调试
- 打包成普通的 jar
- 打包成依赖 GraalVM 的二进制文件
- 打包成不依赖 GraalVM 的二进制文件
- 制作成 Docker 镜像
<a name="B1dH2"></a>
### 2.1 安装 GraalVM
_GraalVM 官方文档：_[_https://www.graalvm.org/22.2/docs/_](https://www.graalvm.org/22.2/docs/)<br />_GraalVM GitHub：_[_https://github.com/graalvm/graalvm-ce-builds/releases_](https://github.com/graalvm/graalvm-ce-builds/releases)<br />了解 GraalVM 的最新版本 22.3 的使用可以查阅 GraalVM 官方文档。先从 GitHub 下载 GraalVM 。这里先以下载windows版本为例，下载后的文件 graalvm-ce-java11-windows-amd64-22.2.0.zip，解压到本地磁盘目录，查看版本信息如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667524423712-1a56a084-9288-45bf-bdce-7e1c78ee5f9a.png#clientId=ub21bce19-f0ee-4&from=paste&id=u35370ae8&originHeight=60&originWidth=765&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf62d263f-734e-4f53-b051-d04b2027c7f&title=)
```bash
# windows
PATH环境变量增加路径：D:\Program Files\Java\graalvm-ce-java11-22.2.0\bin
JAVA_HOME配置环境变量：D:\Program Files\Java\graalvm-ce-java11-22.2.0

# linux
wget https://github.com/graalvm/graalvm-ce-builds/releases/download/vm-22.2.0/graalvm-ce-java17-linux-amd64-22.2.0.tar.gz
tar -xvf graalvm-ce-java17-linux-amd64-22.2.0.tar.gz

export GRAALVM_HOME=/home/commons/graalvm-ce-java17-22.2.0
export PATH=$PATH:$GRAALVM_HOME/bin
export JAVA_HOME=${GRAALVM_HOME}
export PATH=$PATH:$JAVA_HOME/bin
export MAVEN_HOME=/home/commons/apache-maven-3.8.6
export PATH=$PATH:$MAVEN_HOME/bin
```
<a name="k78ZZ"></a>
### 2.2 创建 Quarkus 工程
万丈高楼平地起，首先是创建 Quarkus 项目。与 Spring Boot类似，Quarkus 为我们提供了多种方式创建 Quarkus 工程，下面对常用的 3 种进行介绍。
<a name="XzO36"></a>
#### IDEA 支持创建 Quarkus 项目
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667524423781-f8e981ad-dca9-4a45-96fc-ec09d81adf80.png#clientId=ub21bce19-f0ee-4&from=paste&id=ud94702ee&originHeight=663&originWidth=1024&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u58a1890f-9db6-4730-93fc-a4db3828f99&title=)<br />maven 命令支持创建（使用的是 apache-maven-3.8.6）
```bash
mvn io.quarkus:quarkus-maven-plugin:create -DprojectGroupId=cn.itxs -DprojectArtifactId=quarkus-web-demo-m -DprojectVersion=1.0-SNAPSHOT -DclassName=FirstResource -Dpath=actions
```
通过官方提供 Quarkus 代码生成 [https://code.quarkus.io/](https://code.quarkus.io/) 页面在线创建，这种方法直观且简单（不用记命令），推荐使用。添加自己的 maven 配置，然后下载网站生成的工程包。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667524423846-c14e6ef4-7f88-452a-b9b7-bf14fe59acdf.png#clientId=ub21bce19-f0ee-4&from=paste&id=u9feea01b&originHeight=696&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud2d0bf98-4a1b-446c-8f39-343d8f20798&title=)<br />先以第三种为例说明，通过在线网页生成 quarkus-web-demo.zip。解压后用 tree 命令看看项目结构，是一个标准的 maven 工程，多了个 docker 目录，里面有几个 Dockerfile 文件相关的问题。<br />目录结构如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667524424056-04dbb725-0dd6-48f6-9744-5b0b55312e1f.png#clientId=ub21bce19-f0ee-4&from=paste&id=uc3274258&originHeight=616&originWidth=659&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc012e3f3-367f-49d4-8758-4f6a8c3a810&title=)
<a name="aNE23"></a>
### 2.3 IDEA 导入项目
IDEA 导入上面的 Maven项目，几分钟下载依赖后项目已成功导入。可以看到 Quarkus 项目的 pom 文件核心依赖为 quarkus-bom。Quarkus 官方认为传统的接口就是一个个对外方法的资源，所以在 Quarkus 里面是以 Resource 资源的概念来进行的。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667524424060-4def09dc-8ee6-4de0-89db-3ef57ad70772.png#clientId=ub21bce19-f0ee-4&from=paste&id=u5098786e&originHeight=668&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9ece8478-3253-489c-abb5-c1ce7cce18c&title=)
<a name="ucb6M"></a>
### 2.4 IDEA 运行和调试
在 IDEA 要运行和调试 Quarkus 的项目，首先得在项目添加 SDK，并将路径指向刚才解压的目录。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667524424267-ab155243-38f0-4503-aeb1-ab7dbacca7eb.png#clientId=ub21bce19-f0ee-4&from=paste&id=u44d32e91&originHeight=703&originWidth=986&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub55b25d7-8161-4e9f-a42a-e274565a368&title=)<br />修改项目的 GreetingResource 源文件示例方法返回值内容"Hello ITXS RESTEasy",同样也修改单元测试 GreetingResourceTest 的测试方法的值为上面的内容，在运行菜单中点击添加新的 Quarkus 配置。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667524424214-31604921-683e-4301-ba96-41d802619c09.png#clientId=ub21bce19-f0ee-4&from=paste&id=u779fb08b&originHeight=680&originWidth=1076&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u78fe2603-a32f-4574-acdd-9a8610b8e77&title=)<br />新建后点击运行按钮（mvn compile quarkus:dev ），正常运行输出日志如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667524424313-ae34d1da-8f01-4697-b708-677b3563b172.png#clientId=ub21bce19-f0ee-4&from=paste&id=ua875c5a7&originHeight=542&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1114daae-104e-4601-8c58-e9884f54359&title=)<br />访问本地 http://localhost:8080/hello 后可以看到正确的返回结果，同样也可以直接点击调试按钮进入调试模式。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667524424491-80a17973-41a2-4fa7-8660-319541b91385.png#clientId=ub21bce19-f0ee-4&from=paste&id=u920a8ad7&originHeight=114&originWidth=827&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u399f6d4e-e740-41d0-adae-f09e837ff46&title=)
<a name="H5ozu"></a>
### 2.5 打包成普通的 Jar
通过 maven 的 package 打包后，生成打包目录如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667524424498-28c14093-ad3e-4216-8bfe-23a4d0c7dd3c.png#clientId=ub21bce19-f0ee-4&from=paste&id=u0e168bbd&originHeight=535&originWidth=798&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud88e0fc5-d34f-47c5-a23a-908c7ca6731&title=)<br />可以直接通过 java -jar 运行 quarkus-app 目录下 quarkus-run.jar 和 quarkus-web-demo-1.0.0-SNAPSHOT-native-image-source-jar 目录下的 quarkus-web-demo-1.0.0-SNAPSHOT-runner.jar。不到 1 秒的时间就启动完毕了。<br />访问上面测试地址同样可以看到正确的结果：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667524424564-26d1196d-a882-499d-bcb7-0864bb828555.png#clientId=ub21bce19-f0ee-4&from=paste&id=uaf415327&originHeight=396&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4f2031a9-2763-4584-8e1d-545640968a6&title=)
<a name="bo1Uf"></a>
### 2.6 打包成依赖 GraalVM 二进制文件
此方法打包的二进制文件仍需要依赖 GraalVM，但由于代码已经做了静态编译处理，所以执行效率有大幅提升，比较适合容器化启动。<br />先安装 VS C++依赖，下载 Visual Studio Installer，需要取消中文并选择英文。然后使用 powershell 进入到项目根目录，执行 mvn package -Pnative 命令后报错：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667524424586-8db99cb6-ce48-4c1e-9998-b76f8815edc5.png#clientId=ub21bce19-f0ee-4&from=paste&id=uf598421e&originHeight=505&originWidth=857&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u08bb8a75-6299-43f4-a25d-04eede055a8&title=)<br />根据提示安装 native-image 执行安装命令和确认是否配置了 GRAALVM_HOME、JAVA_HOME 环境变量
```bash
gu install native-image
# 重新执行打包
mvn package -Pnative
```
直接运行生成的可执行文件 quarkus-web-demo-1.0.0-SNAPSHOT-runner，确实飞快，启动时间只要 0.033s，访问hello也正常显示。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667524424813-9a58bc43-0aaa-432c-a2bb-786ab5974e04.png#clientId=ub21bce19-f0ee-4&from=paste&id=ub76ae6fa&originHeight=336&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue3c63fb7-896c-4325-9f7f-fff7a785946&title=)
<a name="T7GWR"></a>
### 2.7 打包成不依赖 GraalVM 的二进制文件
需要本机先装有 docker 的环境，这里用的是 docker desktop。打包有多种方法，可以直接 maven 参数构建，也可以在 appllications.properties 文件中增加构建参数。
```
quarkus.native.container-build=true
quarkus.native.native-image-xmx=4096m
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667524424747-e77ca18b-e54e-4d38-8eca-b35ed7d11c5c.png#clientId=ub21bce19-f0ee-4&from=paste&id=u807e1766&originHeight=350&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud883c213-a1e6-4de2-a53c-44e7f3ac68a&title=)
```bash
# 终端命令中执行打包
mvn clean package -Dnative
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667524424964-e104fc58-262e-427f-839d-9adf87e12de8.png#clientId=ub21bce19-f0ee-4&from=paste&id=u644835d9&originHeight=219&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u00e79df0-be5f-4c63-9221-3bcc302e567&title=)
<a name="ltgBY"></a>
### 2.8 制作 docker 镜像
直接通过 Dockerfile.native 制作 docker 镜像：
```bash
docker build \
-f src/main/docker/Dockerfile.native \
-t bolingcavalry/quarkus-web-demo:0.0.1 .
```
执行 docker 命令生成成功：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667524425029-6b7c3f7c-b653-4a10-96e9-a57f9d6db1f6.png#clientId=ub21bce19-f0ee-4&from=paste&id=u514a5fe0&originHeight=153&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ucf598b9f-2841-4583-b615-95c1b023f5f&title=)<br />查看镜像已经生成：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667524425102-7ac64ad4-295a-4420-92f6-2881d4ac6dd8.png#clientId=ub21bce19-f0ee-4&from=paste&id=u61b1366d&originHeight=39&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6b4b788c-d96b-43a2-a6cc-2c0b3bfa329&title=)<br />通过 `docker run` 命令启动后查看容器的信息：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667524425103-eeb82a9e-ed89-4a6c-88b4-a958df285128.png#clientId=ub21bce19-f0ee-4&from=paste&id=u1b6caefe&originHeight=59&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud796113d-a051-44bc-acd4-7b277f5252e&title=)<br />通过 `**curl -L http://172.17.0.2:8080/hello -w '\n'**` 也可以正常访问。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667524425186-d445c874-1926-42cd-a500-c65e2d25e63c.png#clientId=ub21bce19-f0ee-4&from=paste&id=u721ad29b&originHeight=57&originWidth=903&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue977a9cb-637a-4833-a477-e901a97222a&title=)
