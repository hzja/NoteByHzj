<a name="LR20h"></a>
## 一、Earthly 介绍
开局一张图，功能全靠吹。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1699251818310-48dc3bf0-33b4-4b03-9ac7-ac877876910c.png#averageHue=%23aee3e4&clientId=u7abd99ca-85ee-4&from=paste&id=u48c8bf43&originHeight=556&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue0fbddce-9a02-4180-ae83-bc65b7319ef&title=)<br />Earthly 是一个更加高级的 Docker 镜像构建工具，Earthly 通过自己定义的 Earthfile 来代替传统的 Dockerfile 完成镜像构建；Earthfile 就如同 Earthly 官方所描述：<br />Makefile + Dockerfile = Earthfile<br />在使用 Earthly 进行构建镜像时目前强依赖于 buildkit，Earthly 通过 buildkit 支持了一些 Dockerfile 的扩展语法，同时将 Dockerfile 与 Makefile 整合，使得多平台构建和代码化 Dockerfile 变得更加简单；使用 Earthly 可以更加方便的完成 Dockerfile 的代码复用以及更加友好的 CI 自动集成。
<a name="t3g6k"></a>
## 二、快速开始
<a name="GlVoW"></a>
### 2.1、安装依赖
Earthly 目前依赖于 Docker 和 Git，所以安装 Earthly 前请确保机器已经安装了 Docker 和 Git。
<a name="TlqQP"></a>
### 2.2、安装 Earthly
Earthly 采用 Go 编写，所以主要就一个二进制文件，Linux 下安装可以直接参考官方的安装脚本：
```bash
$ sudo /bin/sh -c 'wget https://github.com/earthly/earthly/releases/latest/download/earthly-linux-amd64 -O /usr/local/bin/earthly && chmod +x /usr/local/bin/earthly && /usr/local/bin/earthly bootstrap --with-autocomplete'
```
安装完成后 Earthly 将会启动一个 buildkitd 容器：earthly-buildkitd。
<a name="OHeDC"></a>
### 2.3、语法高亮
目前 Earthly 官方支持 VS Code、VIM 以及 Sublime Text 三种编辑器的语法高亮，具体如何安装请参考 [官方文档](https://earthly.dev/get-earthly)。
<a name="l3mSR"></a>
### 2.4、基本使用
本示例源于官方 Basic 教程，以下示例以编译 Go 项目为样例：<br />首先创建一个任意名称的目录，目录中存在项目源码文件以及一个 Earthfile 文件；<br />main.go
```go
package main

import "fmt"

func main() {
    fmt.Println("hello world")
}
```
Earthfile
```dockerfile
FROM golang:1.17-alpine
WORKDIR /go-example

build:
    COPY main.go .
    RUN go build -o build/go-example main.go
    SAVE ARTIFACT build/go-example /go-example AS LOCAL build/go-example

docker:
    COPY +build/go-example .
    ENTRYPOINT ["/go-example/go-example"]
    SAVE IMAGE go-example:latest
```
有了 Earthfile 以后就可以使用 Earthly 将其打包为镜像；
```
# 目录结构
~/t/earthlytest ❯❯❯ tree
.
├── Earthfile
└── main.go

0 directories, 2 files

# 通过 earthly 进行构建
~/t/earthlytest ❯❯❯ earthly +docker
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1699251818241-ffa61178-1587-456f-87fc-02533f9c9065.png#averageHue=%23272a37&clientId=u7abd99ca-85ee-4&from=paste&id=u956c26a8&originHeight=691&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u31215198-8f0e-46c5-b850-f50b979738c&title=)<br />构建完成后就可以直接从 docker 的 images 列表中查看刚刚构建的镜像，并运行：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1699251818224-c5055482-d5cc-4d26-a84a-134ea8327499.png#averageHue=%23272e39&clientId=u7abd99ca-85ee-4&from=paste&id=u31367890&originHeight=396&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u1bc38e51-a50d-445c-9796-e15dcfef82b&title=)
<a name="IKjaO"></a>
## 三、进阶使用
<a name="Kc4ce"></a>
### 3.1、多阶段构建
Earthfile 中包含类似 Makefile 一样的 target，不同的 target 之间还可以通过特定语法进行引用，每个 target 都可以被单独执行，执行过程中 earthly 会自动解析这些依赖关系。<br />这种多阶段构建时语法很弹性，可以在每个阶段运行独立的命令以及使用不同的基础镜像；从快速开始中可以看到，始终使用了一个基础镜像(golang:1.17-alpine)，对于 Go 这种编译后自带运行时不依赖其语言 SDK 的应用，事实上可以将 “发布物” 仅放在简单的运行时系统镜像内，从而减少最终镜像体积：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1699251818252-00bc4eef-a39a-4882-8d9b-97ce31a4f765.png#averageHue=%23222935&clientId=u7abd99ca-85ee-4&from=paste&id=u590fab60&originHeight=541&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub663a4e8-7569-4847-9d7b-574c0bf98ed&title=)<br />由于使用了多个 target，所以可以单独的运行 build 这个 target 来验证编译流程，这种多 target 的设计方便构建应用时对编译、打包步骤的细化拆分，同时也方便进行单独的验证。例如单独执行 build 这个 target 来验证编译流程是否正确：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1699251818260-c549c9c6-d8ed-43a8-95c5-f1d9b504a364.png#averageHue=%23262e39&clientId=u7abd99ca-85ee-4&from=paste&id=u5c148757&originHeight=717&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ucb124f56-a499-4ad0-af67-1f70a1dbe05&title=)<br />在其他阶段验证完成后，可以直接运行最终的 target，earthly 会自动识别到这种依赖关系从而自动运行其依赖的 target：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1699251818855-a1216cb9-73b7-485b-9bd4-303c4f64e5a7.png#averageHue=%2329313b&clientId=u7abd99ca-85ee-4&from=paste&id=udbb02cb3&originHeight=632&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5ce87507-a181-4b99-b63b-2c8f1477392&title=)
<a name="eEBH3"></a>
### 3.2、扩展指令
<a name="tFaKO"></a>
#### 3.2.1、SAVE
SAVE 指令是 Earthly 自己的一个扩展指令，实际上分为 SAVE ARTIFACT 和 SAVE IMAGE；其中 SAVE ARTIFACT 指令格式如下：
```bash
SAVE ARTIFACT [--keep-ts] [--keep-own] [--if-exists] [--force] <src> [<artifact-dest-path>] [AS LOCAL <local-path>]
```
SAVE ARTIFACT 指令用于将文件或目录从 build 运行时环境保存到 target 的 artifact 环境；当保存到 artifact 环境后，可以通过 COPY 等命令在其他位置进行引用，类似于 Dockerfile 的 COPY --from... 语法；不同的是 SAVE ARTIFACT 支持 AS LOCAL附加参数，一但指定此参数后，earthly 会同时将文件或目录在宿主机复制一份，一般用于调试等目的。SAVE ARTIFACT 命令在上面的样例中已经展示了，在运行完 earthly +build 命令后实际上会在本地看到被 SAVE 出来的 ARTIFACT：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1699251819531-5314c95f-cc9b-42e1-9632-67abe4dc198f.png#averageHue=%23252c37&clientId=u7abd99ca-85ee-4&from=paste&id=u749c6e65&originHeight=585&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u17491fca-b9a7-4a04-92ea-d05c3f3e8f6&title=)<br />而另一个 SAVE IMAGE 指令则主要用于将当前的 build 环境 SAVE 为一个 IMAGE，如果指定了 --push 选项，同时在执行 earthly +target 命令时也加入 --push 选项，该镜像将会自动被推送到目标 Registry 上。SAVE IMAGE 指令格式如下：
```bash
SAVE IMAGE [--cache-from=<cache-image>] [--push] <image-name>...
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1699251820139-a7025e0e-29d0-43d8-bb16-ff2a80dc50b8.png#averageHue=%2327303a&clientId=u7abd99ca-85ee-4&from=paste&id=u5f3b396a&originHeight=603&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u05b1ca7e-7e2c-4498-8890-01042042ef0&title=)
<a name="xgtO2"></a>
#### 3.2.2、GIT CLONE
GIT CLONE 指令用于将指定 git 仓库 clone 到 build 环境中；与 RUN git clone... 命令不同的是，GIT CLONE 通过宿主机的 git 命令运行，它不依赖于容器内的 git 命令，同时还可以直接为 earthly 配置 git 认证，从而避免将这些安全信息泄漏到 build 环境中；关于如何配置 earthly 的 git 认证请参考 [官方文档](https://docs.earthly.dev/docs/guides/auth)；下面是 GIT CLONE 指令的样例：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1699251820085-2914a1aa-1a99-4906-9dd2-68c929a8b20a.png#averageHue=%2328303b&clientId=u7abd99ca-85ee-4&from=paste&id=u2ed01cc2&originHeight=640&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u6bcc91a1-5fbb-4436-9a2f-c939a16c07d&title=)
<a name="CTld9"></a>
#### 3.2.3、COPY
COPY 指令与标准的 Dockerfile COPY 指令类似，除了支持 Dockerfile 标准的 COPY 功能以外，earthly 中的 COPY 指令可以引用其他 target 环节产生的 artifact，在引用时会自动声明依赖关系；即当在 B target 中存在 COPY +A/xxxxx /path/to/copy 类似的指令时，如果只单纯的执行 earthly +B，那么 earthly 根据依赖分析会得出在 COPY 之前需要执行 target A。COPY 指令的语法格式如下：
```dockerfile
# 与 Dockerfile 相同的使用方式，从上下文复制
COPY [options...] <src>... <dest>

# 扩展支持的从 target 复制方式
COPY [options...] <src-artifact>... <dest>
```
<a name="i8fCz"></a>
#### 3.2.4、RUN
RUN 指令在标准使用上与 Dockerfile 里保持一致，除此之外增加了更多的扩展选项，其指令格式如下：
```dockerfile
# shell 方式运行(/bin/sh -c)
RUN [--push] [--entrypoint] [--privileged] [--secret <env-var>=<secret-ref>] [--ssh] [--mount <mount-spec>] [--] <command>

# exec 方式运行
RUN [[<flags>...], "<executable>", "<arg1>", "<arg2>", ...]
```
其中 `--privileged` 选项允许运行的命令使用 privileged capabilities，但是需要 earthly 在运行 target 时增加 `--allow-privileged` 选项；`--interactive` / `--interactive-keep` 选项用于交互式执行一些命令，在完成交互后 build 继续进行，在交互过程中进行的操作都会被持久化到 镜像中：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1699251820078-93dfe451-bfd5-419a-ac95-bfe14ed248f7.png#averageHue=%23262f3a&clientId=u7abd99ca-85ee-4&from=paste&id=ub920ffaa&originHeight=686&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf872119d-9d09-448c-80d2-34956be87e1&title=)<br />限于篇幅原因，其他的具体指令请查阅官方文档 [Earthfile reference](https://docs.earthly.dev/docs/earthfile)。
<a name="fdOvn"></a>
### 3.3、UDCS
UDCs 全称 “User-defined commands”，即用户定义指令；通过 UDCs 可以将 Earthfile 中特定的命令剥离出来，从而实现更加通用和统一的代码复用；下面是一个定义 UDCs 指令的样例：
```dockerfile
# 定义一个 Command
# ⚠️ 注意: 语法必须满足以下规则
# 1、名称全大写
# 2、名称下划线分割
# 3、首个命令必须为 COMMAND(后面没有冒号)
MY_COPY:
    COMMAND
    ARG src
    ARG dest=./
    ARG recursive=false
    RUN cp $(if $recursive =  "true"; then printf -- -r; fi) "$src" "$dest"

# target 中引用
build:
    FROM alpine:3.13
    WORKDIR /udc-example
    RUN echo "hello" >./foo
    # 通过 DO 关键字引用 UDCs
    DO +MY_COPY --src=./foo --dest=./bar
    RUN cat ./bar # prints "hello"
```
UDCs 不光可以定义在一个 Earthfile 中，UDCs 可以跨文件、跨目录引用：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1699251820186-0e96052a-9e28-408b-ba7c-6812cad8e494.png#averageHue=%23242c37&clientId=u7abd99ca-85ee-4&from=paste&id=u675b5482&originHeight=730&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ueef4e705-f85b-4fd9-bd7f-a655a812e7b&title=)<br />有了 UDCs 以后，可以通过这种方式将对基础镜像的版本统一控制、对特殊镜像的通用处理等操作全部抽象出来，然后每个 Earthfile 根据需要进行引用；关于 UDCs 的使用样例可以参考 [autobuild](https://github.com/mritd/autobuild) 项目，其中的 [udcs](https://github.com/mritd/autobuild/tree/main/earthfiles/udcs) 目录定义了大量的通用 UDCs，这些 UDCs 被其他目标镜的 Earthfile 批量引用。
<a name="hR0aW"></a>
### 3.4、多平台构建
在以前使用 Dockerfile 的时候，需要自己配置然后开启 buildkit 来实现多平台构建；在配置过程中可能会很繁琐，现在使用 earthly 可以默认实现多平台的交叉编译，需要做的仅仅是在 Earthfile 中声明需要支持哪些平台而已：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1699251820326-016d94a3-2c18-4765-9eb1-394ab2c5bc70.png#averageHue=%23232a36&clientId=u7abd99ca-85ee-4&from=paste&id=uef9614da&originHeight=419&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud0f5064e-f847-45b3-9c8d-90e2793fae6&title=)<br />以上 Earthfile 在执行 earthly --push +all 构建时，将会自动构建四个平台的镜像，并保持单个 tag，同时由于使用了 --push 选项还会自动推送到 Docker Hub 上：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1699251821006-ddb43ee2-6390-492a-a5a5-c3930b85ee0d.png#averageHue=%23fefefc&clientId=u7abd99ca-85ee-4&from=paste&id=u2a081337&originHeight=836&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u92981584-4eb5-4be6-b53d-db153f31d6b&title=)
<a name="zK4nz"></a>
## 四、总结
Earthly 弥补了 Dockerfile 的很多不足，解决了很多痛点问题；但同样可能需要一些学习成本，但是如果已经熟悉了 Dockerfile 其实学习成本不高；所以目前还是比较推荐将 Dockerfile 切换为 Earthfile 进行统一和版本化管理的。本文由于篇幅所限(懒)很多地方没有讲，比如共享缓存等，所以关于 Earthly 更多的详细使用等最好还是仔细阅读一下[官方文档](https://docs.earthly.dev/docs/guides)。
<a name="agkkr"></a>
## 引用链接
官方文档: [https://earthly.dev/get-earthly](https://earthly.dev/get-earthly)<br />官方文档: [https://docs.earthly.dev/docs/guides/auth](https://docs.earthly.dev/docs/guides/auth)<br />Earthfile reference: [https://docs.earthly.dev/docs/earthfile](https://docs.earthly.dev/docs/earthfile)<br />autobuild: [https://github.com/mritd/autobuild](https://github.com/mritd/autobuild)<br />udcs: [https://github.com/mritd/autobuild/tree/main/earthfiles/udcs](https://github.com/mritd/autobuild/tree/main/earthfiles/udcs)<br />官方文档: [https://docs.earthly.dev/docs/guides](https://docs.earthly.dev/docs/guides)
