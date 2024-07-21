Podman Desktop<br />Podman 是一个 RedHat 公司发布的开源容器管理工具，初衷就是 Docker 的替代品，在使用上与 Docker 相似，但又有着很大的不同。它与 Docker 的最大区别是架构，Docker 是以 C/S 架构运行的，平时使用的 docker 命令只是一个命令行前端，它需要调用 dockerd 来完成实际的操作，而 dockerd 默认是一个有 root 权限的守护进程。Podman 不需要守护进程，直接通过 fork/exec 的形式启动容器，不需要 root 权限。
<a name="arfBq"></a>
## Podman Desktop
Podman Desktop 能够轻松地使用本地环境中的容器，Podman Desktop 利用 Podman Engine 提供轻量级且无守护程序的容器工具。该工具允许浏览、管理容器的生命周期、检查容器、来自不同容器引擎的镜像等。
<a name="TzXKi"></a>
### 「又快又轻」

- 无守护进程，并使用最快的技术使 Podman Desktop 更快。
- 对从环境中使用的资源反应灵敏。
<a name="J3w9j"></a>
### 「开放」

- Podman Desktop 在专注于将 Podman 作为默认打包的容器引擎的同时，还兼容了其他容器引擎。
- 这为最终用户提供了一个工具来独立于使用的引擎来管理他们的所有容器。
<a name="SVIYe"></a>
### 「简单」

- 在本地管理容器引擎可能很困难。
- 在本地环境中运行 Kubernetes 需要大量知识。
- Podman Desktop 处理了这种复杂性，使开发人员能够专注于他们的代码。
<a name="a1wM8"></a>
### 「可扩展的工作流」

- Podman Desktop 提供扩展点，使社区能够构建更多功能。
- 这允许与其他工具和技术进行更深入的集成，以扩展工具的工作流程和功能。
- 从而为开发人员提供了一个工具来独立于所使用的引擎来管理他们的所有容器。
<a name="VY0KY"></a>
## 特性
Podman Desktop 具有以下一些特性。
<a name="Mn0Al"></a>
### 「1. 管理容器」
列出、搜索、检查、连接、运行和停止容器。![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1656294159241-75a08d08-a7a2-4a96-80dd-2661a5a4d434.jpeg#clientId=u7803ef99-73b5-4&from=paste&id=u31455ffa&originHeight=577&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ubb7f932a-9bac-4063-b302-1b5feb4c33f&title=)
<a name="snNfO"></a>
### 「2. 构建、拉取和推送镜像」

- 从该工具构建镜像
- 通过管理仓库来拉取和推送镜像
- 从这些镜像运行容器

![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1656294159307-454f222f-f462-4651-ad3a-9a3116555038.jpeg#clientId=u7803ef99-73b5-4&from=paste&id=uc5980dd4&originHeight=776&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u98fcb044-440c-4fd8-847c-954fc2d0768&title=)
<a name="Zndsa"></a>
### 「3. 管理 Podman 资源」

- 查看分配的内存、CPU 和存储
- 如果需要，创建新机器

![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1656294159270-827f55b9-208a-4684-bff2-b91e2a0520a6.jpeg#clientId=u7803ef99-73b5-4&from=paste&id=u149661cb&originHeight=913&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ueae595b4-24b6-4b6e-b8a2-21492382062&title=)
<a name="JasTY"></a>
### 「4. 兼容Docker desktop扩展」

- 指定 Docker desktop扩展的 OCI 镜像以将其导入。
- 例如：安全扫描器部署到 OpenShift 扩展。

![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1656294159399-69952255-798c-4a27-a22a-ce77045d101b.jpeg#clientId=u7803ef99-73b5-4&from=paste&id=u8249e43b&originHeight=772&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua5218110-69be-4a19-b0af-1e3585b8223&title=)
<a name="lNIko"></a>
## 扩展
Podman Desktop 支持使用 Docker Desktop 的扩展功能，Podman Desktop 能够通过添加包装器来拦截 API 调用来使用 Docker Desktop UI 插件。通过添加 Docker Desktop 扩展，可以扩展 Podman Desktop 的功能。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1656294160284-ffdf2c70-6775-49af-8ce0-e31f150fd09e.jpeg#clientId=u7803ef99-73b5-4&from=paste&id=ufc677143&originHeight=477&originWidth=529&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u93f8a39f-4aff-41dc-b352-2ba6a554395&title=)<br />此外 Podman Desktop 还可以在后台使用插件来管理不同的容器引擎。通过添加新插件，可以扩展 Podman Desktop 的功能，例如插入一个新的容器引擎，如 Podman、Docker、Lima 等。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1656294159757-4b88e655-47e6-47d1-8797-baf9a8f77f70.jpeg#clientId=u7803ef99-73b5-4&from=paste&id=u78a1c59a&originHeight=648&originWidth=596&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u10f9ff47-6c4b-48b6-a049-0f86c9d345a&title=)<br />扩展是用使用 JavaScript/Typescript 来编写的。
<a name="GHI1s"></a>
## 安装
首先安装 Podman 4.x，这里是 Mac 系统，直接使用 `brew install podman` (Mac m1可能会有问题)即可一键安装。安装完成后接下来创建并启动第一台 Podman 机器：
```bash
podman machine init
podman machine start
```
启动后可以使用下面的命令来查看安装信息：
```bash
podman info
```
Podman 和对应的 machine 启动后，接下来就可以安装 Podman Desktop 了，直接在 [https://github.com/containers/podman-desktop/releases/](https://github.com/containers/podman-desktop/releases/) 上下载适用于 Windows、MacOS 和 Linux 的最新二进制文件，直接解压安装即可，安装后就可以直接使用了。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1656294159782-f205f2e5-5c2a-4dcc-a87d-8821472cf564.png#clientId=u7803ef99-73b5-4&from=paste&id=uee7369b0&originHeight=617&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u40c6adf0-107f-4ce6-aca5-9996dd12bff&title=)<br />Git仓库：[https://github.com/containers/podman-desktop](https://github.com/containers/podman-desktop)
