Docker Humpback<br />Docker容器作为微服务的重要组成部分，伴随着微服务的流行，在很多公司已经落地并投入生产。不过，docker为dev和ops带来便利的同时，docker的编排和创建管理也带来了一些问题。纯手动操作docker，一个两个还好，多了估计能逼疯dev和ops，尤其是ops，用kubernetes， 仅仅是安装配置就够烦人，很多镜像都在google上，国内的网络环境大家都懂，并且kubernetes要分布式，官网给的例子都是一个master，两个node，有够复杂。<br />不过，幸好找到了一个轻量级的容器管理工具—Humpback。它的思路跟常用的操作系统监控差不多，在各个被操作的机器上安装完docker后再安装它的agent,然后在一台master上部署它的web系统就可以了，实现起来简单方便，单机多节点都可以，非常适合中小公司尤其是创业公司来使用。
<a name="J5hJ1"></a>
## 1、Humpback能做什么？
说了这么多，那么Humpback都能做什么呢，大概能实现如下操作。

- 以 Web界面 的方式，来进行容器的创建/运行以及管理（启动，停止，重启，监控，应用版本升级/回退）。
- 镜像构建，将镜像所需文件和Dockfile打包为 .tar.gz 文件，就可以打包为一个镜像。
- 管理私有仓库，将自己部署好的私有仓库，纳入到humpback的管理中。
- 集群容器调度，当部署一个app时，只需要告诉humpback，要部署多少个实例，humpback会自动根据配置进行集群部署，并通过WebHook通知部署结果。
- 对容器管理进行权限设定。
<a name="k4Mbi"></a>
## 2、安装Humpback和Agent
Humpback和Agent都是基于Docker进行安装，因此在安装前需要先安装docker。<br />安装完docker后执行如下命令安装Humpback，docker会自动查找镜像进行安装:
```bash
docker run -d --net=host --restart=always -e HUMPBACK_LISTEN_PORT=8000 -v /var/local/software/dockerimage/humpback-web/dbFiles:/humpback-web/dbFiles --name humpback-web humpbacks/humpback-web:1.0.0
```
安装agent:
```bash
docker run -d -ti --net=host --restart=always -e DOCKER_API_VERSION=v1.21 -v /var/run/:/var/run/:rw --name=humpback-agent humpbacks/humpback-agent:1.0.0
```
都执行完后在浏览器访问IP:8000即可看到登录界面，输入默认用户名admin默认密码：123456进行登录。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1636333826262-10449b3a-45ef-4372-a74a-29d4152c2321.png#clientId=ub73a8b61-b4e4-4&from=paste&height=594&id=u3bbe2e21&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&size=230042&status=done&style=none&taskId=ua05ef1f3-c9e5-4c65-ad18-a96c170b233&width=1280)
<a name="l3XeW"></a>
## 3、容器管理
登录web界面后需要先创建一个Group。并将部署了 agent 的机器IP，添加到Servers属性中。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1636333886881-61918575-0610-49a7-a807-02d4325f6494.png#clientId=ub73a8b61-b4e4-4&from=paste&height=594&id=u8aaebefe&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&size=477902&status=done&style=none&taskId=ub3d9931b-8317-4e68-8bf0-5d64494e350&width=1280)<br />创建完Group后，在左侧按钮点击Group，就可以看到创建的group名称，点击名称下边的节点IP名就可以看到agent机器上所有的docker应用。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1636337396034-b4fc76aa-4a6e-4c48-8d72-a9839d287d6c.png#clientId=ua5c767cf-d059-4&from=paste&height=594&id=u3cf888a5&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&size=331840&status=done&style=none&taskId=ud26cf7a9-d143-4e39-ad14-24b2292a899&width=1280)<br />点击单个容器右侧的操作，就可以来操作容器了，比如要启动一个已经存在的容器。如下图机器上只有humpback和agent两个容器启动。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1636334005170-ce4bf9c4-ac9e-43fc-a725-f4ef27debb38.png#clientId=ub73a8b61-b4e4-4&from=paste&height=161&id=u183afd2e&originHeight=482&originWidth=3840&originalType=binary&ratio=1&size=630067&status=done&style=none&taskId=u5c36f497-32c1-428f-841f-613a9894181&width=1280)<br />启动firstDocker 容器。稍微等一分钟会弹出成功提示，如果不成功则需要进入机器查看日志原因。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1636337450315-06717fff-6c8c-4c6b-86fc-8c550632a4f8.png#clientId=ua5c767cf-d059-4&from=paste&height=594&id=u7d629750&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&size=327921&status=done&style=none&taskId=u272862e3-a338-4350-85d5-a71495f946f&width=1280)
<a name="wm70Z"></a>
### 新建Docker容器
有两种方式。新建一个全新的或者基于某个已存在容器做细微修改后新建一个容器。如图所示：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1636337481519-eda7b749-2e95-4756-b55d-d8a694788cbf.png#clientId=ua5c767cf-d059-4&from=paste&height=594&id=ua9f9eeb7&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&size=345533&status=done&style=none&taskId=u3d34455b-8856-46d9-9537-addcf0564a1&width=1280)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1636337618563-67113a48-b406-47c9-8785-7a9595c5fe55.png#clientId=ua5c767cf-d059-4&from=paste&height=594&id=u92213f35&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&size=291482&status=done&style=none&taskId=uf881c9a3-38eb-4a6d-851f-7e48d523fd3&width=1280)
<a name="mrIfj"></a>
### 克隆容器
选择要克隆的容器进行该容器的管理界面<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1636337597320-ec3fcf35-0c08-4acb-8a54-cd451473c4b3.png#clientId=ua5c767cf-d059-4&from=paste&height=594&id=u787f5f8b&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&size=344537&status=done&style=none&taskId=u83cf5816-7776-48da-8aa3-4b62a8039ff&width=1280)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1636337666432-6ec1881f-4da3-4453-8e41-a0adad5f7a3d.png#clientId=ua5c767cf-d059-4&from=paste&height=594&id=u568de606&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&size=549169&status=done&style=none&taskId=u1cf77485-f258-4f4b-a862-b3d6f03c49a&width=1280)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1636337679260-aa7e4e7f-e754-4f08-b70b-97be26391200.png#clientId=ua5c767cf-d059-4&from=paste&height=594&id=u5448037f&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&size=318781&status=done&style=none&taskId=u76394ac8-2245-425e-b5d9-9c0f363d329&width=1280)
