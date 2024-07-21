Docker<br />Docker 镜像的大小对于系统的 CI/CD 等都有影响，尤其是云部署场景。在生产实践中都会做瘦身的操作，尽最大的可能使用 Size 小的镜像完成功能。下文是一个简单的 ReactJS 程序上线的瘦身体验，希望可以帮助大家找到镜像瘦身的方向和灵感。<br />如果正在做Web开发相关工作，那么你可能已经知道容器化的概念，以及知道它强大的功能等等。<br />但在使用Docker时，镜像大小至关重要。从create-react-app（[https://reactjs.org/docs/create-a-new-react-app.html](https://reactjs.org/docs/create-a-new-react-app.html)）获得的样板项目通常都超过1.43 GB。<br />将容器化一个 ReactJS 应用程序，并学习一些关于如何减少镜像大小并提高性能的技巧。<br />以 ReactJS 为例，但它适用于任何类型的 NodeJS 应用程序。
<a name="L998Q"></a>
## 步骤1：创建项目
1、借助脚手架通过命令行模式创建 React 项目
```bash
$ npx create-react-app docker-image-test
```
2、命令执行成功后将生成一个基础 React 应用程序架构<br />3、可以进入项目目录安装依赖并运行项目
```bash
$ cd docker-image-test  
$ yarn install  
$ yarn start
```
4、通过访问 http://localhost:3000 可以访问已经启动的应用程序<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675406656377-a0a2fa8f-e2f9-4fe2-9a29-ea7e82a27a31.png#averageHue=%232e333b&clientId=u0d4bd09f-7e65-4&from=paste&id=u9f036f50&originHeight=364&originWidth=700&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ucd0343c0-5e58-4e71-a7f8-bc008c73f1f&title=)
<a name="nwRmz"></a>
## 步骤2：构建第一个镜像
1、在项目的根目录中创建一个名为 Dockerfile 的文件，并粘贴以下代码：
```dockerfile
FROM node:12  
WORKDIR /app  
COPY package.json ./  
RUN yarn install  
COPY . .  
EXPOSE 3000  
CMD \["yarn", "start"\]
```
2、注意，这里从 Docker 仓库获得基础镜像 Node:12，然后安装依赖项并运行基本命令。（不会在这里讨论 Docker 命令的细节）<br />3、现在可以通过终端为容器构建镜像
```bash
$ docker build -t docker-image-test .
```
4、Docker 构建镜像完成之后，可以使用此命令查看已经构建的镜像：
```bash
$ docker images
```
在查询结果列表的顶部，是新创建的图像，在最右边，可以看到图像的大小。目前是 1.43GB。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675406656404-8d35ca39-bc65-4806-b865-f8443fc3b915.png#averageHue=%232e333b&clientId=u0d4bd09f-7e65-4&from=paste&id=u1a8535b1&originHeight=75&originWidth=700&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6af63985-ae7f-471c-8a68-a59615cc119&title=)<br />5、使用以下命令运行镜像
```bash
$ docker run --rm -it -p 3000:3000/tcp docker-image-test:latest
```
打开浏览器并且刷新页面验证其可以正常运行。
<a name="Fb7UC"></a>
## 步骤3：修改基础镜像
1、先前的配置中用 node:12 作为基础镜像。但是传统的 Node 镜像是基于 Ubuntu 的，对于简单的 React 应用程序来说这大可不必。<br />2、从 DockerHub（官方Docker镜像注册表）中可以看到，基于 alpine-based 的 Node 镜像比基于 Ubuntu 的镜像小得多，而且它们的依赖程度非常低。<br />3、下面显示了这些基本图像的大小比较<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675406656740-ebf1aceb-ea87-46d2-b706-2d02ce54920e.png#averageHue=%232f343c&clientId=u0d4bd09f-7e65-4&from=paste&id=u00145de4&originHeight=52&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9d52eed1-2932-4c57-98e6-af8acbd9439&title=)<br />现在将使用 node:12-alpine 作为基础镜像，看看会发生什么。
```dockerfile
FROM node:12-alpine  
WORKDIR /app  
COPY package.json ./  
RUN yarn install  
COPY . .  
EXPOSE 3000  
CMD \["yarn", "start"\]
```
然后以此构建镜像，并与之前做对比。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675406656409-a190f06f-d1b1-41a1-9192-7c63d308895c.png#averageHue=%232e333b&clientId=u0d4bd09f-7e65-4&from=paste&id=u15a5f96a&originHeight=96&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ufef4b750-88c4-496b-99c7-0a77ee0bc2d&title=)<br />镜像大小减少到只有 580MB，这是一个很大的进步。但还能做得更好吗？
<a name="QaJcN"></a>
## 步骤4：多级构建
1、在之前的配置中，会将所有源代码也复制到工作目录中。<br />2、但这大可不必，因为从发布和运行来看只需要构建好的运行目录即可。因此，现在将引入多级构建的概念，以减少不必要的代码和依赖于最终镜像。<br />3、配置是这样的：
```dockerfile
# STAGE 1  
  
FROM node:12-alpine AS build  
WORKDIR /app  
COPY package.json ./  
RUN yarn  install  
COPY . /app  
RUN yarn build  
  
# STAGE 2  
  
FROM node:12-alpine  
WORKDIR /app  
RUN npm install -g webserver.local  
COPY --from=build /app/build ./build  
EXPOSE 3000  
CMD webserver.local -d ./build
```
4、在第一阶段，安装依赖项并构建项目<br />5、在第二阶段，复制上一阶段构建产物目录，并使用它来运行应用程序。<br />6、这样在最终的镜像中就不会有不必要的依赖和代码。<br />接下来，构建镜像成功后并从列表中查看镜像<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675406656400-312bd8fb-b6cb-49f7-a19d-34d175a68083.png#averageHue=%232e333b&clientId=u0d4bd09f-7e65-4&from=paste&id=uf18f2085&originHeight=47&originWidth=700&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u732b0729-901e-4c90-a830-8a495f9f2b1&title=)<br />现在镜像大小只有 97.5MB。这简直太棒了。
<a name="ZIwTr"></a>
## 步骤5：使用Nginx
1、正在使用 Node 服务器运行 ReactJS 应用程序的静态资源，但这不是静态资源运行的最佳选择。<br />2、尝试使用 Nginx 这类更高效、更轻量级的服务器来运行资源应用程序，也可以尽可能提高其性能，并且减少镜像的量。<br />3、最终的 Docker 配置文件看起来像这样
```dockerfile
# STAGE 1  
  
FROM node:12-alpine AS build  
WORKDIR /app  
COPY package.json ./  
RUN yarn  install  
COPY . /app  
  
RUN yarn build  
  
# STAGE 2  
  
FROM nginx:stable-alpine  
COPY --from=build /app/build /usr/share/nginx/html  
EXPOSE 80  
CMD \["nginx", "-g", "daemon off;"\]
```
4、正在改变 Docker 配置的第二阶段，以使用 Nginx 来服务应用程序。<br />5、然后使用当前配置构建镜像。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675406660713-f8d2b8ed-0979-4be3-bea0-061c79581547.png#averageHue=%232f343c&clientId=u0d4bd09f-7e65-4&from=paste&id=uc7797076&originHeight=47&originWidth=700&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1e0d2541-1f37-431c-b4ec-87125c0147f&title=)<br />6、镜像大小减少到只有 22.4 MB！<br />7、同时，正在使用一个性能更好的服务器来服务出色的应用程序。<br />8、可以使用以下命令验证应用程序是否仍在工作。
```bash
$ docker run --rm  -it -p 3000:80/tcp docker-image-test:latest
```
9、注意，将容器的 80 端口暴露给外部，因为默认情况下，Nginx 将在容器内部的 80端口上可用。<br />所以这些是一些简单的技巧，可以应用到任何 NodeJS 项目，以大幅减少镜像大小。<br />现在，容器确实更加便携和高效了。
