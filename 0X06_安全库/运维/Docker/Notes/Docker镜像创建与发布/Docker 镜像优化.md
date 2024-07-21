Docker<br />Docker 是一个供软件开发人员和系统管理员使用容器构建、运行和与分享应用程序的平台。容器是在独立环境中运行的进程，它运行在自己的文件系统上，该文件系统是使用 docker 镜像构建的。镜像中包含运行应用程序所需的一切（编译后的代码、依赖项、库等等）。镜像使用 Dockerfile 文件定义。<br />术语 dockerization 或 containerization 通常用于定义创建 Docker 容器的过程。<br />因为容器具备如下优点，所以很受欢迎：

- 灵活性：即使是最复杂的应用程序也可以容器化。
- 轻量化：容器共享主机内核，使得它们远比虚拟机高效。
- 便携性：可以做到本地编译，到处运行。
- 松耦合：容器自我封装，一个容器被替换或升级不会打断别的容器。
- 安全性：容器对进程进行了严格的限制和隔离，而无需用户进行任何配置。

重点讨论如何优化 Docker 镜像以使其轻量化。<br />从一个示例开始，在该示例中，构建了一个 React 应用程序并将其容器化。运行 `npx` 命令并创建 Dockerfile 之后，得到了如图 1 所示的文件结构。
```bash
npx create-react-app app --template typescript
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624343606041-554788ba-6460-43c8-b778-a5eff3d2d7e9.webp#clientId=ufda1dc4a-9731-4&from=paste&id=u35899ded&originHeight=333&originWidth=396&originalType=url&ratio=3&status=done&style=none&taskId=uc5e96bba-c521-404c-980f-2326dcf2bc5)<br />图 1：文件结构<br />如果构建一个基础的 Dockerfile（如下所示），最终会得到一个 1.16 GB 的镜像：
```dockerfile
FROM node:10
WORKDIR /appCOPY app /appRUN npm install -g webserver.localRUN npm install && npm run build
EXPOSE 3000CMD webserver.local -d ./build
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624343605778-bb95eedf-6282-4b9f-b4d5-9f795e14977f.webp#clientId=ufda1dc4a-9731-4&from=paste&id=ue0511b7b&originHeight=529&originWidth=898&originalType=url&ratio=3&status=done&style=none&taskId=u6969cdd8-e58b-4dad-bb93-c4423d8fdea)<br />图 2：镜像的初始大小为 1.16GB
<a name="gxDdQ"></a>
### 第一步优化：使用轻量化基础镜像
在 Docker Hub（公共 Docker 仓库）中，有一些镜像可供下载，每个镜像都有不同的特征和大小。<br />通常，相较于基于其他 Linux 发行版（例如 Ubuntu）的镜像，基于 Alpine 或 BusyBox 的镜像非常小。这是因为 Alpine 镜像和类似的其他镜像都经过了优化，其中仅包含最少的必须的软件包。在下面的图片中，可以看到 Ubuntu、Alpine、Node 和基于 Alpine 的 Node 镜像之间的大小比较。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624343605801-adcc889d-48f6-43e4-9de8-400b8e50fdc3.webp#clientId=ufda1dc4a-9731-4&from=paste&id=u0a1c3e76&originHeight=591&originWidth=1080&originalType=url&ratio=3&status=done&style=none&taskId=ufef21b8e-92ea-4244-9a53-bb191d9c839)<br />图 3：基础镜像的不同大小<br />通过修改 Dockerfile 并使用 Alpine 作为基础镜像，镜像最终大小为 330MB：
```dockerfile
FROM node:10-alpine
WORKDIR /appCOPY app /appRUN npm install -g webserver.localRUN npm install && npm run build
EXPOSE 3000CMD webserver.local -d ./build
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624343605760-e5d76f9f-b17b-4285-b234-1cd73b1781ea.webp#clientId=ufda1dc4a-9731-4&from=paste&id=u8f9cd567&originHeight=567&originWidth=934&originalType=url&ratio=3&status=done&style=none&taskId=u43cef7bf-aaf7-4316-92f6-f7436ad9df3)<br />图 4：经过第一步优化后镜像大小为 330MB
<a name="y0d47"></a>
### 第二步优化：多阶段构建
通过多阶段构建，可以在 Dockerfile 中使用多个基础镜像，并将编译成品、配置文件等从一个阶段复制到另一个阶段，这样就可以丢弃不需要的东西。<br />在本例中，部署 React 应用程序需要的是编译后的代码，不需要源文件，也不需要 node_modules 目录和 package.json 文件等。<br />通过将 Dockerfile 修改为如下内容，最终得到的镜像大小为 91.5MB。请记住，来自第一阶段（第 1-4 行）的镜像不会被自动删除，Docker 将它保存在 cache 中，如果在另一个构建镜像过程中执行了相同的阶段，就可以使镜像构建更快。所以必须手动删除第一阶段镜像。
```dockerfile
FROM node:10-alpine AS buildWORKDIR /appCOPY app /appRUN npm install && npm run build
FROM node:10-alpineWORKDIR /appRUN npm install -g webserver.localCOPY --from=build /app/build ./buildEXPOSE 3000CMD webserver.local -d ./build
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624343605776-b1533efa-579a-485e-89d9-b9890e5fd929.webp#clientId=ufda1dc4a-9731-4&from=paste&id=u4926c115&originHeight=529&originWidth=898&originalType=url&ratio=3&status=done&style=none&taskId=u30b24c2a-318b-442d-a934-dbdc799594f)<br />图 5：第二步优化后的镜像大小为 91.5MB<br />现在有了一个 Dockerfile，它有两个阶段：在第一个阶段中，编译项目，在第二个阶段中，在 web 服务器上部署应用程序。然而，Node 容器并不是提供网页（HTML、CSS 和 JavaScript 文件、图片等）服务的最佳选择，最好的选择是使用像 Nginx 或 Apache 这样的服务。在本例中，将使用 Nginx。<br />通过将 Dockerfile 修改为如下内容，镜像最终大小是 22.4MB，如果运行这个容器，可以看到网页可以正常工作，没有任何问题（图 7）。
```dockerfile
FROM node:10-alpine AS buildWORKDIR /appCOPY app /appRUN npm install && npm run build

FROM nginx:stable-alpineCOPY --from=build /app/build /usr/share/nginx/htmlEXPOSE 80CMD ["nginx", "-g", "daemon off;"]
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624343606423-b1d5e5e1-58a4-4c59-a528-2ad0f66971e2.webp#clientId=ufda1dc4a-9731-4&from=paste&id=uc45d2ff5&originHeight=529&originWidth=898&originalType=url&ratio=3&status=done&style=none&taskId=uda55c3a7-e654-409e-8b5f-9b85adf7d41)<br />图 6：第三步优化后的镜像大小为 22.4MB<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624343606255-4a749bf2-d891-42cb-b14b-c3b0d2348975.png#clientId=ufda1dc4a-9731-4&from=paste&id=u5928533d&originHeight=527&originWidth=898&originalType=url&ratio=3&status=done&style=shadow&taskId=uc2d15a2f-9abc-4c73-bda4-0915b87286e)<br />图 7：最终容器的运行结果<br />参考链接：<br />[https://docs.docker.com/get-started/](https://docs.docker.com/get-started/)<br />[https://docs.docker.com/develop/develop-images/multistage-build/](https://docs.docker.com/develop/develop-images/multistage-build/)
