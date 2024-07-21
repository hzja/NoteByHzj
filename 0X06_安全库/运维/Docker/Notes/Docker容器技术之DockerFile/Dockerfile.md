Docker
<a name="wz0Gp"></a>
## Dockerfile
Dockerfile是一个用来构建镜像的文本文件，文本内容包含了一条条构建镜像所需的指令和说明；在`docker build`命令中使用-f参数来指向文件中任意位置的Dockerfile：
```bash
$ docker build -f /path/to/Dockerfile
```
<a name="w0oMP"></a>
### FROM指令
FROM指令用来指定一个基础镜像，它决定了Dockerfile构建出的镜像为何物以及怎样的环境；大多数的Dockerfile，都是以FROM指令开始；它的语法如下：
```dockerfile
FROM <image> [AS <name>]
FROM <image>:<tag> [AS <name>]
```
Dockerfile必须以FROM指令开始，不过它支持在FROM之前由ARG指令定义一个变量：
```dockerfile
ARG NG_VERSION=1.19.3
FROM nginx:${NG_VERSION}
CMD /bin/bash
```
<a name="to8Dp"></a>
#### 多阶段构建
在构建镜像时通常会有多个阶段的镜像需要来进行构建，比如vue项目构建镜像时，需要在编译阶段打包出dist文件，还需要在生产运行阶段使用dist文件作为静态资源；如果不使用多阶段构建，通常需要两个Dockerfile文件，构建出两个镜像，这样有一个镜像肯定是浪费的。<br />Docker从17.05开始，支持多阶段构建，就是在Dockerfile中可以使用多个FROM指令，每个FROM指令都可以使用不同的基础镜像，并且每条指令都会开始新阶段的构建；在多阶段构建中，可以将资源从一个阶段复制到另一个阶段，在最终镜像中只保留所需要的内容。
```dockerfile
FROM node

# ...一些操作

FROM nginx

# ...一些操作

COPY --from=0 . .
```
第二个FROM指令开始一个新的构建阶段，`COPY --from=0`代表从上一个阶段（即第一阶段）拷贝文件；默认情况下，构建阶段没有命名，可以使用从0开始的整数编号引用它；可以给FROM指令加上一个`as <Name>`作为构建阶段的命名。
```dockerfile
FROM node as compile

FROM nginx as serve

COPY --from=compile . .
```
在后面的例子中，会来演示如何使用多阶段构建优化构建过程。
<a name="c6TIK"></a>
#### 基础镜像选择
由于基础镜像决定着构建出镜像产物的大小，因此选择一个合适的基础镜像显得十分重要了。如果去hub.docker.com查看node的标签，会发现除了版本号之外，后面还会带着一些看不懂的单词，什么alpine，什么slime了，这些版本号都代表着什么含义呢？简单地了解一下。<br />docker镜像之间的区别在于底层的操作系统<br />首先如果什么都不带，默认latest，那就是完整的镜像版本，如果对其他版本没有什么了解的话，那么选它是肯定不会出错的。<br />其次是slim版本，slim表示最小安装包，仅包含需要运行指定容器的特定工具集。通过省去较少使用的工具，镜像会更小。如果服务器有空间限制且不需要完整版本，就可以使用此镜像。不过使用这个版本时，要进行彻底得测试。<br />然后是经常会看到的alpine版本，alipine镜像基于alpine linux项目，这是一个社区开发的面向安全应用的轻量级Linux发行版。它的优点就是基于的linux操作系统非常轻量，因此构建出来的镜像也非常的轻量；它的缺点也十分的明显，就是不包含一些有可能会用到的包，并且使用的glibc包等都是阉割版；因此如果使用这个版本，也需要进行彻底的测试。<br />ls看下这三个版本，也能发现它们的大小存在着差异：
```bash
$ docker image ls node
REPOSITORY   TAG       IMAGE ID       CREATED        SIZE
node         slim      ffedf4f28439   5 days ago     241MB
node         alpine    d2b383edbff9   3 months ago   170MB
node         latest    a283f62cb84b   3 months ago   993MB
```
其次是一些Debian的发行版，Debian是一个自由的，稳定得无与伦比操作系统；带有下面一些标签的镜像对应Debian发行版本号。

- bullseye：Debian 11
- buster：Debian 10
- stretch：Debian 9
- jessie：Debian 8
<a name="q2mYW"></a>
### RUN指令
RUN指令用于在镜像容器中执行命令，其有以下两种执行方式：
```dockerfile
# shell 执行
RUN <command>
# exec 格式
RUN ["可执行文件", "参数1", "参数2"]
```
RUN指令常见的用法就是安装包用apt-get，假设需要在镜像安装curl：
```dockerfile
FROM ubuntu:18.04

RUN apt-get update

RUN apt-get install -y curl
```
Dockerfile的指令是分层构建的，每一层都有缓存，假设下次添加了一个包wget：
```dockerfile
FROM ubuntu:18.04

RUN apt-get update

RUN apt-get install -y curl wget
```
如果下次再次构建时，`apt-get update`指令也不会执行，使用之前缓存的镜像；而install由于update指令没有执行，可能安装过时的curl和wget版本。<br />因此通常会把update和install写在一条指令，确保Dockerfiles每次安装的都是包的最新的版本；同时也可以减少镜像层数，减少包的体积：
```dockerfile
RUN apt-get update && apt-get install -y curl wget
```
<a name="PYqMe"></a>
### WORKDIR工作目录
WORKDIR指令可以用来指定工作目录，以后各层的当前目录就被改为指定的工作目录；如果该目录不存在，WORKDIR会自动创建目录。<br />很多童鞋把Dockerfile当成Shell脚本来写，因此可能会导致下面的错误：
```dockerfile
FROM node:10.15.3-alpine

RUN mkdir /app && cd /app

RUN echo "hello" > world.txt
```
这里echo的作用是将字符串hell重定向写入到world.txt中；如果把这个Dockerfile构建镜像运行后，会发现找不到/app/world.txt；由于在Shell脚本中两次连续运行的命令是同一个进程执行环境，前一行命令运行影响后一个命令；而由于Dockerfile分层构建的原因，两个RUN命令执行的环境是两个完全不同的容器。<br />因此如果需要改变以后每层的工作目录的位置，可以使用WORKDIR指令，建议在WORKDIR指令中使用绝对路径：
```dockerfile
FROM node:10.15.3-alpine

WORKDIR /app

RUN echo "hello" > world.txt
```
这样生成的world.txt就在/app目录下面了。
<a name="KI5eR"></a>
### COPY复制
COPY指令用于从构建上下文目录中复制文件到镜像内的目标路径中，类似linux的cp命令，它的语法格式如下：
```dockerfile
COPY [--chown=<user>:<group>] <源路径>... <目标路径>
COPY [--chown=<user>:<group>] ["<源路径1>",... "<目标路径>"]
```
复制的文件可以是一个文件、多个文件或者通配符匹配的文件：
```dockerfile
COPY package.json /app

COPY package.json app.js /app

COPY src/*.js /app
```
但需要注意的是，COPY指令只能复制文件夹下的文件，而不能复制文件夹本身，和linux的cp命令有区别；比如下面复制src文件夹：
```dockerfile
COPY src /app
```
运行后可以发现src文件夹下面的文件都拷贝到/app目录下了，没有拷贝src文件夹本身，因此需要这样来写：
```dockerfile
COPY src /app/src
```
<a name="DSU6D"></a>
### CMD指令
CMD指令用于执行目标镜像中包含的软件，可以指定参数，它也有两种语法格式：
```dockerfile
CMD <命令>
CMD ["可执行文件", "参数1", "参数2"...]
```
可以发现CMD和RUN都可以用来执行命令的，很相似，那他们两者有什么区别么？首先发现RUN是用来执行docker build构建镜像过程中要执行的命令，比如创建文件夹mkdir、安装程序apt-get等等。<br />而CMD指令在docker run时运行而非`docker build`，也就是启动容器的时候，它的首要目的在于为启动的容器指定默认要运行的程序，程序运行结束，容器也就结束。<br />而容器在run的时候只能创建一次，因此一个Dockerf中也只能有一个CMD指令；比如容器运行node程序，最后需要启动程序：
```dockerfile
CMD ["node", "app.js"]
# 或者
CMD npm run start
```
<a name="pnnAT"></a>
### ENTRYPOINT入口点
ENTRYPOINT指令的作用和CMD一样，也是在指定容器启动程序和参数；一个Dockerfile同样也只能有一个ENTRYPOINT指令；当指定了ENTRYPOINT后，CMD指令的含义发生了改变，不再是直接的运行其命令，而是将 CMD 的内容作为参数传给ENTRYPOINT指令，相当于：
```dockerfile
<ENTRYPOINT> "<CMD>"
```
那么这样的好处是啥呢？看一个使用的例子，在Docker中使用curl命令来获取公网的IP地址：
```dockerfile
FROM ubuntu:18.04

# 切换ubuntu源
RUN  sed -i s@/archive.ubuntu.com/@/mirrors.aliyun.com/@g /etc/apt/sources.list
RUN  apt-get clean

RUN apt-get update \
    && apt-get install -y curl \
    && rm -rf /var/lib/apt/lists/*
    
CMD [ "curl", "-s", "http://myip.ipip.net" ]
```
然后使用`docker build -t myip .`来构建myip的镜像；当想要查询ip的时候，只需要执行如下命令：
```bash
$ docker run --rm myip
当前 IP：218.4.251.37  来自于：中国 江苏 苏州  电信
```
这样就实现了把镜像当成命令使用，不过如果想要同时显示HTTP头信息，就需要加上`-i`参数：
```bash
$ docker run --rm myip -i
docker: Error response from daemon: failed to create shim: OCI runtime create failed: container_linux.go:380: starting container process caused: exec: "-i": executable file not found in $PATH: unknown.
```
但是这个`-i`参数加上后并不会传给CMD指令，而是传给了`docker run`，但是`docker run`并没有`-t`参数，因此报错；如果想要加入`-i`，就需要重新完整的输入这个命令；而使用ENTRYPOINT指令就可以解决这个问题：
```dockerfile
FROM ubuntu:18.04

RUN  sed -i s@/archive.ubuntu.com/@/mirrors.aliyun.com/@g /etc/apt/sources.list
RUN  apt-get clean

RUN apt-get update \
    && apt-get install -y curl \
    && rm -rf /var/lib/apt/lists/*
    
ENTRYPOINT  [ "curl", "-s", "http://myip.ipip.net" ]
```
重新尝试加入`-i`参数：
```
$ docker run --rm myip -i
HTTP/1.1 200 OK
Date: Fri, 01 Apr 2022 07:24:21 GMT
Content-Type: text/plain; charset=utf-8
Content-Length: 67
Connection: keep-alive
X-Via-JSL: fdc330b,-
Set-Cookie: __jsluid_h=9f0775bbcb4cc97b161093b4c66dd766; max-age=31536000; path=/; HttpOnly
X-Cache: bypass

当前 IP：218.4.251.37  来自于：中国 江苏 苏州  电信
```
就可以发现http的头部信息也展示出来了。
<a name="nPN1I"></a>
### VOLUME数据卷
VOLUME指令用于暴露任何数据库存储文件，配置文件，或容器创建的文件和目录；其语法格式如下：
```dockerfile
VOLUME ["<路径1>", "<路径2>"...]
VOLUME <路径>
```
可以事先指定某些目录挂载为匿名卷，这样在运行时如果用户不指定挂载，其应用也可以正常运行，不会向容器存储层写入大量数据，比如：
```dockerfile
VOLUME /data
```
这里的/data目录就会在容器运行时自动挂载为匿名卷，任何向/data中写入的信息都不会记录进容器存储层，从而保证了容器存储层的无状态化。
```bash
$ docker run -d -v mydata:/data xxxx
```
运行容器时可以本地目录覆盖挂载的匿名卷；**「需要注意」**的是，在Windows下挂载目录和Linux环境（以及Macos）挂载目录有一些区别，在Linux环境下由于是树状目录结构，挂载时直接找到目录即可，如果目录不存在，docker还会自动创建：
```bash
$ docker run -d -v /home/root/docker-data:/data xxxx
```
windows环境下则需要对应盘符下的目录：
```bash
$ docker run -d -v d:/docker-data:/data xxxx
```
<a name="BQ4q6"></a>
### EXPOSE端口
EXPOSE指令是声明容器运行时提供服务的端口，这只是一个声明，在容器运行时并不会因为这个声明应用就会开启这个端口的服务；其语法如下：
```dockerfile
EXPOSE <端口1> [<端口2>...]
```
在Dockerfile中写入这样的声明有两个好处，一个是帮助镜像使用者理解这个镜像服务的守护端口，以方便配置映射；另一个好处则是在运行时使用随机端口映射时，也就是 docker run -P 时，会自动随机映射 EXPOSE 的端口。
<a name="PuZMY"></a>
### ENV指令
ENV指令用于设置环境变量，其语法有两种，支持多种变量的设置：
```dockerfile
ENV <key> <value>
ENV <key1>=<value1> <key2>=<value2>...
```
这里的环境变量无论是后面的指令，如RUN指令，还是运行时的应用，都可以直接使用环境变量：
```dockerfile
ENV NODE_VERSION 7.2.0

RUN curl -SLO "https://nodejs.org/dist/v$NODE_VERSION/node-v$NODE_VERSION-linux-x64.tar.xz" \
  && curl -SLO "https://nodejs.org/dist/v$NODE_VERSION/SHASUMS256.txt.asc"
```
这里定义了环境变量NODE_VERSION，后面的RUN指令中可以多次使用变量进行操作；因此如果后续想要升级node版本，只需要更新7.2.0即可
<a name="PFrdT"></a>
### ARG指令
ARG指令和ENV一样，也是设置环境变量的，所不同的是，ARG设置的是构建环境的环境变量，在以后容器运行时是不会存在的。<br />![通俗解释指令的意义](https://cdn.nlark.com/yuque/0/2023/png/396745/1675212721800-65fca228-c7f1-4530-a8b0-3ce8ef0e5f6c.png#averageHue=%23e3f7f7&clientId=u7b17b7dc-8126-4&from=paste&id=ufabbd28a&originHeight=296&originWidth=563&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u96f8330e-da5d-4177-a2b7-d7610fd8f2e&title=%E9%80%9A%E4%BF%97%E8%A7%A3%E9%87%8A%E6%8C%87%E4%BB%A4%E7%9A%84%E6%84%8F%E4%B9%89 "通俗解释指令的意义")
<a name="nvMX7"></a>
## 部署前端项目
<a name="ZPOxx"></a>
### vue项目
当在本地开发完一个前端项目后，肯定要部署在服务器上让别人来进行访问页面的，一般都是让运维在服务器上配置nginx来将项目打包后作为静态资源；这里自己配置nginx文件，结合docker来部署项目。<br />首先在项目目录创建nginx的配置文件default.conf：
```nginx
server {
    listen 80;
    server_name _;
    location / {
        root /usr/share/nginx/html;
        index index.html inde.htm;
        try_files $uri $uri/ /index.html =404;
    }
    error_page 500 502 503 504 /50x.html;
    location = /50x.html {
        root /usr/share/nginx/html;
    }
}
```
该配置文件定义了打包后静态资源的目录为/usr/share/nginx/html，因此需要将dist文件夹拷贝到该目录；同时使用了try_files来匹配vue的history路由模式。<br />在项目目录再创建一个Dockerfile文件，写入以下内容：
```dockerfile
FROM nginx:latest

COPY default.conf /etc/nginx/conf.d/

COPY dist/ /usr/share/nginx/html/

EXPOSE 80
```
在项目打包生成dist文件后就可以构建镜像了：
```bash
$ docker build -t vue-proj .
```
![构建镜像](https://cdn.nlark.com/yuque/0/2023/png/396745/1675212721779-c07cef39-04b8-4ce2-ac2b-055c3ff44f9b.png#averageHue=%230a0807&clientId=u7b17b7dc-8126-4&from=paste&id=u8396a474&originHeight=846&originWidth=1033&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u6687b3ad-5dbe-43cc-906c-dac71d836a3&title=%E6%9E%84%E5%BB%BA%E9%95%9C%E5%83%8F "构建镜像")<br />接下来基于该镜像启动服务器：
```bash
$ docker run -itd -p 8080:80 vue-proj
```
这样程序就起来了，访问http://localhost:8080端口就可以看到部署的网站了。
<a name="Ha9kM"></a>
### express项目
还有一些node项目，比如expree、eggjs或者nuxt，也可以使用docker进行部署，不过需要把所有的项目文件都拷贝到镜像中去。<br />首先模拟一个简单的express的入口文件app.js
```javascript
const express = require("express");

const app = express();
const PORT = 8080;

app.get("/", (req, res) => {
  res.send("hello express");
});

app.listen(PORT, () => {
  console.log(`listen on port:${PORT}`);
});
```
由于下面需要拷贝整个项目的文件，因此可以通过.dockerignore文件来忽略某些文件：
```
.git
node_modules
```
然后编写Dockerfile：
```dockerfile
FROM node:10.15.3-alpine

WORKDIR /app

COPY package*.json ./

RUN npm install --registry https://registry.npm.taobao.org

COPY . .

EXPOSE 8080

CMD npm run start
```
看到上面的流程是先拷贝package*.json文件，安装依赖后再拷贝整个项目，那么为什么这么做呢？聪明的童鞋大概已经猜到了，大概率又双叒叕是跟docker的分层构建和缓存有关。<br />不错，如果把package*.json和代码程序一起拷贝，如果只更改了代码而没有新增依赖，但docker仍然会安装依赖；但是如果把它单独拿出来的话，就能够提高缓存的命中率。后面的构建镜像和启动容器也就不再赘述了。
<a name="kQWw2"></a>
### vue项目多阶段构建
上面在vue项目中手动打包生成了dist文件，然后再通过docker进行部署；在FROM指令中也提到了多阶段构建，那么来看下如果使用多阶段构建如何来进行优化。<br />还是在项目中准备好nginx的配置文件default.conf，但是这次不再手动生成dist文件，而是将构建的过程放到Dockerfile中：
```dockerfile
FROM node:12 as compile

WORKDIR /app

COPY package.json ./

RUN npm i --registry=https://registry.npm.taobao.org

COPY . .

RUN npm run build

FROM nginx:latest as serve

COPY default.conf /etc/nginx/conf.d/

COPY --from=compile /app/dist /usr/share/nginx/html/

EXPOSE 80
```
可以看到在上面第一个compile阶段，通过`npm run build`命令生成了dist文件；而第二个阶段中再把dist文件拷贝到nginx的文件夹中即可；最后构建的产物依然是最后FROM指令的nginx服务器。<br />多阶段构建用到的命令比较多，很多童鞋会想最后的镜像会不会很大；通过ls命令查看构建后的镜像：
```bash
$ docker images
REPOSITORY            TAG                  IMAGE ID       CREATED          SIZE
multi-compile         latest               a37e4d71562b   11 seconds ago   157MB
```
可以看到它的大小和单独用nginx构建差不多。
