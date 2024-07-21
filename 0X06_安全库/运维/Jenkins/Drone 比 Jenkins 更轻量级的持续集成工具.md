持续集成工具
<a name="kJDfb"></a>
## 什么是 Drone？
Drone 是一个面向忙碌的开发团队的自助持续集成和持续交付平台。<br />官网地址：[https://www.drone.io/](https://www.drone.io/)<br />同时，**Drone** 是使用 **Golang** 语言进行编写。所有的编译、测试的流程都在 Docker 容器中执行。**Drone** 通过使用简单的 **YAML** 配置文件，就可以轻松的定义出一条流水线，并且每一个构建操作都是在一个临时的 **Docker** 容器中执行，能够完全控制其构建环境并保证隔离。最后，开发人员只需要在项目中引入 **.drone.yml** 文件，将代码推送到 Git 仓库中，即可自动化的完成**编译**、**测试** 和 **发布**。<br />简单来说，**Drone** 其实就是一款轻量级的 **Jenkins** ，可以占用更少的资源，实现软件的流水线操作，并且可以轻松的和 **Gitlab**、**Gitee**、**Github** 相结合。<br />![Drone官网](https://cdn.nlark.com/yuque/0/2022/png/396745/1662533059680-4c75aa70-ca11-471d-aace-2124f1844a8f.png#clientId=uf2f7c7d5-6176-4&from=paste&id=ufd70c2da&originHeight=753&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ud2ecb197-5535-40fa-a73f-d582dd13f43&title=Drone%E5%AE%98%E7%BD%91 "Drone官网")
<a name="Aqvaq"></a>
## 创建 OAuth2应用
由于**蘑菇博客**是部署在 **Gitee** 中的，因此本文将介绍 **Drone** 如何实现：代码提交到 **Gitee** 上，自动触发 **Drone** 流水线，完成项目的打包和部署<br />首先，打开 **Gitee** 上的设置页面，找到 **第三方应用**，然后选择创建应用<br />创建地址：[https://gitee.com/oauth/applications](https://gitee.com/oauth/applications)<br />![创建Gitee应用](https://cdn.nlark.com/yuque/0/2022/png/396745/1662533059889-b5a28567-d92b-4b67-bc1b-63e8bbea5ca6.png#clientId=uf2f7c7d5-6176-4&from=paste&id=u49a821c7&originHeight=787&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u41254b58-4bd9-4bc9-baa8-e32295cb89b&title=%E5%88%9B%E5%BB%BAGitee%E5%BA%94%E7%94%A8 "创建Gitee应用")<br />然后填写相关的信息，需要注意的是，这里的 **ip** 地址，需要换成自己服务器的

- **应用主页**：服务器地址+端口号
- **应用回调地址**：服务器地址+端口号/login
- **权限**：把这四个都勾选上，否则后面登录可能会报错

![配置应用信息](https://cdn.nlark.com/yuque/0/2022/png/396745/1662533059793-de032c2b-6045-44f2-b38f-6b313fc981f1.png#clientId=uf2f7c7d5-6176-4&from=paste&id=u1ab63f1b&originHeight=788&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=uac966114-769e-494a-8c33-fc9929eb402&title=%E9%85%8D%E7%BD%AE%E5%BA%94%E7%94%A8%E4%BF%A1%E6%81%AF "配置应用信息")<br />点击创建应用，就会得到 **ClientKey** 和 **ClientSecret** ，需要保存好，后续在编写 **docker-compose.yml** 文件的时候会用到<br />![获取ClientId](https://cdn.nlark.com/yuque/0/2022/png/396745/1662533059875-36a4ec78-88ab-47f1-8461-b00b62fda42a.png#clientId=uf2f7c7d5-6176-4&from=paste&id=u271a5bd2&originHeight=382&originWidth=813&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=uaccb2a49-dc87-44aa-9dae-af3067f441c&title=%E8%8E%B7%E5%8F%96ClientId "获取ClientId")
<a name="l9vll"></a>
## 生成RPC秘钥
由于 **drone server** 和 **drone runner** 通信需要使用秘钥，因此可以使用 openssl 命令生成
```bash
$ openssl rand -hex 16
bea26a2221fd8090ea38720fc445eca6
```
可以也需要保存改秘钥，在下面需要使用
<a name="PNawM"></a>
## 编写 drone.yml 文件
首先，需要编写 **drone** 的 **docker-compose** 文件，用来创建 **drone** 容器<br />创建 **drone.yml** 文件，并修改以下的内容

- **DRONE_GITEE_CLIENT_ID**：上面的 **Gtiee** 的 **Client ID** 值
- **DRONE_GITEE_CLIENT_SECRET**：**Gitee OAuth2** 客户端密钥（上面的 **Client Secret** 值）
- **DRONE_RPC_SECRET**：**Drone** 的共享密钥（生成 **RPC** 密钥）
- **DRONE_SERVER_HOST**：**Drone** 的主机名(改成自己的域名获得 ip+端口(注意是drome的))
- **DRONE_USER_CREATE**：创建管理员账户，这里对应为 **Gitee** 的用户名(也就是登录的账号,不是昵称)(填错了回导致自动化部署失败)
```yaml
version: '3'
networks:
  mogu:
    external: false
services:
  # 容器名称
  drone-server:
    container_name: drone
    # 构建所使用的镜像
    image: drone/drone
    # 映射容器内80端口到宿主机的8611端口8611端口,若修改的话，那么上面Gitee上也需要进行修改
    ports:
      - 8611:80
    # 映射容器内/data目录到宿主机的目录
    volumes:
      - /usr/local/bin/drone:/data
    # 容器随docker自动启动
    restart: always
    privileged: true
    networks:
      - mogu
    environment:
      # Gitee 服务器地址如果github就把GITEE改成GITHUB和https://gitee.com改成https://github.com
      - DRONE_GITEE_SERVER=https://gitee.com
      # Gitee OAuth2客户端ID
      # - DRONE_GITEA_CLI（上面的Client ID值）
      - DRONE_GITEE_CLIENT_ID=0a334xxxxxxxxc711e40af
      # Gitee OAuth2客户端密钥（上面的Client Secret值）
      - DRONE_GITEE_CLIENT_SECRET=79173f5367xxxxx000899
      # drone的共享密钥（生成rpc密钥）
      - DRONE_RPC_SECRET=bea26a2221fd8090ea38720fc445eca6
      # drone的主机名(改成自己的域名获得ip+端口(注意是drome的))
      - DRONE_SERVER_HOST=192.168.11.1:8611
      # 外部协议方案根据你的域名判断是http还是https(ip加端口是http)
      - DRONE_SERVER_PROTO=http
      - DRONE_GIT_ALWAYS_AUTH=false
     # 创建管理员账户，这里对应为gitee的用户名(也就是登录的账号,不是昵称)(填错了回导致自动化部署失败)
      - DRONE_USER_CREATE=username:xxx@qq.com,admin:true

  docker-runner:
    container_name: drone-runner
    image: drone/drone-runner-docker
    restart: always
    privileged: true
    networks:
      - mogu
    depends_on:
      - drone-server
    volumes:
      - /var/run/docker.sock:/var/run/docker.sock
      - /sync/drone/agent/drone.key:/root/drone.key
    environment:
      # 用于连接到Drone服务器的协议。该值必须是http或https。(同上)
      - DRONE_RPC_PROTO=http
      # 用于连接到Drone服务器的主机名(同上)
      - DRONE_RPC_HOST=81.70.100.87:8611
      # Drone服务器进行身份验证的共享密钥，和上面设置一样（生成rpc密钥）
      - DRONE_RPC_SECRET=bea26a2221fd8090ea38720fc445eca6
      # 限制运行程序可以执行的并发管道数
      - DRONE_RUNNER_CAPACITY=2
        # docker runner 名称
      - DRONE_RUNNER_NAME=docker-runner
      - DRONE_DEBUG=true                      # 调试相关，部署的时候建议先打开
      - DRONE_LOGS_DEBUG=true                 # 调试相关，部署的时候建议先打开
      - DRONE_LOGS_TRACE=true                 # 调试相关，部署的时候建议先打开
      - TZ=Asia/Shanghai
```
填写完毕后，将 **drone.yml** 文件拷贝到服务器上，使用下面的命令运行
```bash
docker-compose -f drone.yml up -d
```
这里需要小伙伴们提前下载好 **docker-compose**，如果不清楚什么是 **docker-compose** 的小伙伴，下面有一个简单的介绍<br />Docker Compose 是用来定义和运行多个Docker应用程序的工具。通过Compose，可以使用YAML文件来配置应用程序需要的所有服务，然后使用一个命令即可从YML文件配置中创建并启动所有服务<br />首先到 **Github release**中下载 **docker-compose**<br />[https://github.com/docker/compose/releases](https://github.com/docker/compose/releases)<br />然后选择 **Linux** 版本下载<br />![下载 docker-compose](https://cdn.nlark.com/yuque/0/2022/png/396745/1662533059955-c2873742-5b2e-44da-853d-fd47eb526fff.png#clientId=uf2f7c7d5-6176-4&from=paste&id=u1ba355a4&originHeight=708&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u9f347934-f7c3-493a-9e80-f966d33ffab&title=%E4%B8%8B%E8%BD%BD%20docker-compose "下载 docker-compose")<br />把下载到的文件使用 **Xftp** 工具，拷贝到 /usr/local/bin/ 目录下
```bash
# 重命名
mv docker-compose-Linux-x86_64  docker-compose
# 加入执行权限
sudo chmod +x /usr/local/bin/docker-compose
# 查看docker-compose版本
docker-compose -v
```
安装完成后，再次使用上面的命令，安装 **drone**，安装完成后，使用 `**docker ps -a**` 即可查看到安装的 **drone** 了<br />![运行的drone容器](https://cdn.nlark.com/yuque/0/2022/png/396745/1662533060376-9f288483-e860-4ae3-ac4d-f4981e0dc97d.png#clientId=uf2f7c7d5-6176-4&from=paste&id=uf604a490&originHeight=104&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ubd2e605a-e21b-44e9-bc2d-a2d4cdeadc6&title=%E8%BF%90%E8%A1%8C%E7%9A%84drone%E5%AE%B9%E5%99%A8 "运行的drone容器")<br />下面两个运行的容器的作用分别如下：

- **drone**：为 **Drone** 的管理提供了 **Web** 页面，用于管理从 **Git** 上获取的仓库中的流水线任务
- **drone-runner**：一个单独的守护进程，会轮询 **Server**，获取需要执行的流水线任务，之后执行

如果正确的启动了上述的两个容器，那么你打开浏览器，输入**IP:8611** 可以进入到 **Drone** 主服务的 **Web** 管理界面的登录界面，<br />![drone登录页](https://cdn.nlark.com/yuque/0/2022/png/396745/1662533060332-1f750b6a-a65c-4ec0-abe9-b5b76b204520.png#clientId=uf2f7c7d5-6176-4&from=paste&id=ub6b2482f&originHeight=575&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ubdf27de4-f4ee-4301-9c76-4b994f7d92f&title=drone%E7%99%BB%E5%BD%95%E9%A1%B5 "drone登录页")<br />点击 **Continue** 后，即可跳转到 **Gitee** 的 **OAuth** 授权页面，这里直接点击 **同意授权**<br />![Gitee授权登录](https://cdn.nlark.com/yuque/0/2022/png/396745/1662533060347-e8acdb32-9cf0-4717-8fbd-a6ec1b2c4d12.png#clientId=uf2f7c7d5-6176-4&from=paste&id=u9d126a85&originHeight=492&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ucd0e1f6b-ced7-4b67-a78a-6c8b8cae0d5&title=Gitee%E6%8E%88%E6%9D%83%E7%99%BB%E5%BD%95 "Gitee授权登录")<br />授权通过后，在仓库的管理页面，即可看到新增了一条 **WebHook** 记录，当 **Gitee** 有新的代码提交请求后，就会通过调用下面的地址，从而让 **Drone** 能够启动流水线操作。<br />![新增Webhook记录](https://cdn.nlark.com/yuque/0/2022/png/396745/1662533060482-a36ebf1b-933e-44bb-be1f-1f03451f13bc.png#clientId=uf2f7c7d5-6176-4&from=paste&id=ucec34bf3&originHeight=530&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u8abe73d5-e2a2-4015-9a07-760f1d5db66&title=%E6%96%B0%E5%A2%9EWebhook%E8%AE%B0%E5%BD%95 "新增Webhook记录")<br />登录成功后，即可跳转到 **Drone** 的主页，在这里是能够看到 **Gitee** 上全部的项目的<br />![查看所有项目](https://cdn.nlark.com/yuque/0/2022/png/396745/1662533060803-f07278c9-bce9-4912-9dda-7acfb927c5c3.png#clientId=uf2f7c7d5-6176-4&from=paste&id=u85abce53&originHeight=556&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u85393faa-c11c-4147-9d4f-4221ed01d08&title=%E6%9F%A5%E7%9C%8B%E6%89%80%E6%9C%89%E9%A1%B9%E7%9B%AE "查看所有项目")<br />找到需要构建流水线的项目，然后进入后点击 **激活仓库**<br />这里是有BUG的，如果项目是中文名称的话，进去是会 404<br />![激活项目](https://cdn.nlark.com/yuque/0/2022/png/396745/1662533060738-089efa3d-ab86-4177-959a-3bbd8ab54f10.png#clientId=uf2f7c7d5-6176-4&from=paste&id=uf7b7406b&originHeight=604&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ua30cfb05-f433-4426-b359-7a75fee974d&title=%E6%BF%80%E6%B4%BB%E9%A1%B9%E7%9B%AE "激活项目")<br />然后把下面几个开关进行打开，主要是开启容器特权，以及自动取消构建<br />![开启开关](https://cdn.nlark.com/yuque/0/2022/png/396745/1662533060935-4ce55152-5dd5-4be4-9337-86a8959c9f89.png#clientId=uf2f7c7d5-6176-4&from=paste&id=uf87ab224&originHeight=672&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u9d6177d0-c5d7-4587-8551-0ef28412908&title=%E5%BC%80%E5%90%AF%E5%BC%80%E5%85%B3 "开启开关")
<a name="L8rbm"></a>
## 创建一个 **SpringBoot** 项目
首先，在 Gitee 上创建一个私有的仓库 **hello-mogu**<br />[https://gitee.com/projects/new](https://gitee.com/projects/new)<br />![新建一个仓库](https://cdn.nlark.com/yuque/0/2022/png/396745/1662533060894-63ff806b-f59f-4709-8bdc-b587e7bd27aa.png#clientId=uf2f7c7d5-6176-4&from=paste&id=u12bc0206&originHeight=661&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ufd5b33fd-3922-49f0-8117-0916a9cee17&title=%E6%96%B0%E5%BB%BA%E4%B8%80%E4%B8%AA%E4%BB%93%E5%BA%93 "新建一个仓库")<br />然后打开网站 **start.spring.io** 初始化一个最简单的 **SpringBoot** 项目<br />![初始化一个SpringBoot项目](https://cdn.nlark.com/yuque/0/2022/png/396745/1662533061016-7771e6b2-b767-480c-b6e8-fd8faeef7830.png#clientId=uf2f7c7d5-6176-4&from=paste&id=u9ad30c6c&originHeight=586&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u2c16c187-fa0c-4e68-b6c7-464561e61b7&title=%E5%88%9D%E5%A7%8B%E5%8C%96%E4%B8%80%E4%B8%AASpringBoot%E9%A1%B9%E7%9B%AE "初始化一个SpringBoot项目")<br />然后下载后解压，即可看到以下的目录<br />![解压项目](https://cdn.nlark.com/yuque/0/2022/png/396745/1662533061109-511ca2a1-42c1-46e6-a0b4-7eb6712a66af.png#clientId=uf2f7c7d5-6176-4&from=paste&id=u4d3e2e0e&originHeight=377&originWidth=838&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ud34ed1a9-d74f-460f-b664-25a731ccd0c&title=%E8%A7%A3%E5%8E%8B%E9%A1%B9%E7%9B%AE "解压项目")<br />然后使用以下命令，将该项目和刚刚创建的Gitee仓库关联上
```bash
git init
git commit -m "first commit"
git remote add origin https://gitee.com/moxi159753/hello-mogu.git
git push -u origin "master"
```
下面即可看到代码成功推送到 **Gitee** 上了<br />![推送到Gitee](https://cdn.nlark.com/yuque/0/2022/png/396745/1662533061365-57e2566d-2430-4e37-8987-45513ba6e504.png#clientId=uf2f7c7d5-6176-4&from=paste&id=u8616438d&originHeight=540&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=uf046b6be-7e03-4886-8790-76d6a8a329d&title=%E6%8E%A8%E9%80%81%E5%88%B0Gitee "推送到Gitee")
<a name="DUXoi"></a>
## Drone 流水线命令
接下来，需要通过将 **.drone.yml** 文件创建到 **Git** 存储库的根目录来配置管道。在这个文件中，定义了每次收到 **Webhook** 时执行的一系列步骤。<br />下面这个是最简单的一个 **drone** 流水线，创建 **.drone.yml** 文件，写上下面的内容
```yaml
kind: pipeline  # kind 属性定义了对象的种类。此示例定义了一个管道对象。
type: docker    # type 属性定义管道的类型。此示例定义了一个 Docker 管道，其中每个管道步骤都在 Docker 容器内执行。
name: default   # name 属性定义了管道的名称。您可以为您的项目定义一个或多个管道

steps: # 步骤部分定义了一系列串行执行的管道步骤。如果管道中的任何步骤失败，管道将立即退出
- name: greeting # name 属性定义管道步骤的名称
  image: alpine # image 属性定义了一个执行 shell 命令的 Docker 镜像。您可以使用来自任何 DockerHub 中的任何 Docker镜像。
  commands: # commands 属性将在 Docker 容器内执行的 shell 命令列表定义为容器入口点。如果任何命令返回非零退出代码，则管道步骤将失败。
  - echo hello
  - echo world
```
写完后，再次将该提交提交到远程仓库，然后找到刚刚创建的仓库（如果没有，先执行 **SYNC** 操作）<br />![Drone中找到刚刚创建的仓库](https://cdn.nlark.com/yuque/0/2022/png/396745/1662533061402-34f25bc0-fc8f-47a5-9eba-cbf28db46c8d.png#clientId=uf2f7c7d5-6176-4&from=paste&id=ud33317a4&originHeight=244&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ue23d940a-9e96-489d-9507-a6653421a63&title=Drone%E4%B8%AD%E6%89%BE%E5%88%B0%E5%88%9A%E5%88%9A%E5%88%9B%E5%BB%BA%E7%9A%84%E4%BB%93%E5%BA%93 "Drone中找到刚刚创建的仓库")<br />点击仓库内，设置好配置，点击保存<br />![修改配置并保存](https://cdn.nlark.com/yuque/0/2022/png/396745/1662533061500-43ce8d6b-440e-4df1-97d5-9b1dd7f1f355.png#clientId=uf2f7c7d5-6176-4&from=paste&id=u001c5156&originHeight=599&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ub5d1985c-6b8b-404d-b7cd-13b6a0b56fe&title=%E4%BF%AE%E6%94%B9%E9%85%8D%E7%BD%AE%E5%B9%B6%E4%BF%9D%E5%AD%98 "修改配置并保存")<br />然后切换到构建页面，点击创建<br />![启动构建](https://cdn.nlark.com/yuque/0/2022/png/396745/1662533061436-b0112c3c-84c9-410a-a0b2-d94eac120a0c.png#clientId=uf2f7c7d5-6176-4&from=paste&id=ud0e87d82&originHeight=496&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u85d579ab-1b6e-4ba9-8adf-03886973511&title=%E5%90%AF%E5%8A%A8%E6%9E%84%E5%BB%BA "启动构建")<br />创建完成后，项目就会进行流水线构建【以后可以设置代码提交，主动触发】<br />![构建记录](https://cdn.nlark.com/yuque/0/2022/png/396745/1662533061769-e44f8582-e889-410f-8310-2807de27c379.png#clientId=uf2f7c7d5-6176-4&from=paste&id=ufd09aea5&originHeight=444&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u9c1a0cbb-b070-45ed-8495-1d94f54cce2&title=%E6%9E%84%E5%BB%BA%E8%AE%B0%E5%BD%95 "构建记录")<br />点击记录详情页，可以看到打印出来的 **hello world**<br />![构建详情页](https://cdn.nlark.com/yuque/0/2022/png/396745/1662533061752-b74ce697-6b2f-440f-bf64-8a22262eac00.png#clientId=uf2f7c7d5-6176-4&from=paste&id=ub6a090d7&originHeight=491&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u5ccbd1ec-4cca-4bef-8629-10f97bf8ceb&title=%E6%9E%84%E5%BB%BA%E8%AF%A6%E6%83%85%E9%A1%B5 "构建详情页")
<a name="iXI4W"></a>
## 更多流水线操作
例如，可以将两个步骤串连起来，第一个步骤输出 **hello world**、第一个输出 **bonjour monde**
```yaml
kind: pipeline
type: docker
name: greeting

steps:
- name: en
	image: alpine
	commands:
	- echo hello world

- name: fr
	image: alpine
	commands:
	- echo bonjour monde
```
在推送代码后，就可以看到流水线已经正常输出内容了<br />![流水线运行成功](https://cdn.nlark.com/yuque/0/2022/png/396745/1662533061925-183bbb1b-9924-4764-92d7-6fc935f72cb7.png#clientId=uf2f7c7d5-6176-4&from=paste&id=u7d286218&originHeight=579&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u45eb4cb3-6190-43db-8892-eaa89b7c7ed&title=%E6%B5%81%E6%B0%B4%E7%BA%BF%E8%BF%90%E8%A1%8C%E6%88%90%E5%8A%9F "流水线运行成功")<br />同时，也可以定义多个管道，串联的去执行
```yaml
kind: pipeline
type: docker
name: en

steps:
- name: greeting
	image: alpine
	commands:
	- echo hello world

---
kind: pipeline
type: docker
name: fr

steps:
- name: greeting
	image: alpine
	commands:
	- echo bonjour monde
```
同时，通过增加 **trigger** 可以设置管道触发的方式，例如，push：代码提交后触发，pull_request：代码PR后触发
```yaml
kind: pipeline
type: docker
name: en

steps:
- name: greeting
	image: alpine
	commands:
	- echo hello world

trigger:
  event:
  - push
  - pull_request
  - tag
  - promote
  - rollback
```
下面来完整的测试一下 **hello-mogu**<br />首先，在刚刚的 **SpringBoot** 项目中，创建一个 **Controller** 文件，写上一个 hello 蘑菇的方法<br />![hello mogu](https://cdn.nlark.com/yuque/0/2022/png/396745/1662533061986-c61c9843-cdfc-462c-b33e-90981eac0682.png#clientId=uf2f7c7d5-6176-4&from=paste&id=uc680161d&originHeight=530&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u81920df7-600d-4c52-baaa-26a47aecb59&title=hello%20mogu "hello mogu")
```java
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class HelloMogu {
    @GetMapping("/hello-mogu")
    public String helloMogu() {
        return "hello-mogu";
    }
}
```
然后编辑 .drone.yml 文件，编写流水线代码
```yaml
kind: pipeline # 定义对象类型，还有secret和signature两种类型
type: docker # 定义流水线类型，还有kubernetes、exec、ssh等类型
name: hello-mogu # 定义流水线名称

steps: # 定义流水线执行步骤，这些步骤将顺序执行
  - name: build-package # 流水线名称
    image: maven:3.8.5-openjdk-8 # 定义创建容器的Docker镜像
    volumes: # 将容器内目录挂载到宿主机，仓库需要开启Trusted设置
      - name: maven-build
        path: /root/hello-mogu # 将应用打包好的Jar和执行脚本挂载出来
    commands:
      - mvn clean package -DskipTests=true
      # 将打包后的jar包，拷贝到 /root/hello-mogu 目录
      - cp /drone/src/target/hello-mogu-0.0.1-SNAPSHOT.jar  /root/hello-mogu

volumes: # 定义流水线挂载目录，用于共享数据
  - name: maven-build
    host:
      path: /root/hello-mogu   #jar包目录可以修改从宿主机中挂载的目录
```
这里使用了 **build-package** 的镜像进行构建，里面包含了 **Java** 和 **Mavne** 环境。<br />同时，为了方便将构建好的 **Jar** 包挂载出来，使用了 **volumes** ，需要指定容器内的地址 以及 挂载到宿主机的位置<br />将修改后的代码推送到 Gitee 中，可以看到流水线正常运行，并且在开始下载依赖进行构建 **jar** 包<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662533062047-25e32605-ac69-4493-80dd-b5998a0fe553.png#clientId=uf2f7c7d5-6176-4&from=paste&id=ubbc2b041&originHeight=538&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8a39060c-b2d0-4771-8373-3c2ec974a48&title=)<br />构建详情页<br />执行成功后，打开自己的服务器，在 /root/hello-mogu 目录，就可以看到刚刚打包后的 **jar** 包了<br />![打包成功的jar](https://cdn.nlark.com/yuque/0/2022/png/396745/1662533062223-f75722a7-35cc-4be8-9924-cedf5fe71bd3.png#clientId=uf2f7c7d5-6176-4&from=paste&id=u2a62c808&originHeight=118&originWidth=332&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u44a91978-8102-4769-aab6-72e24cfeaa1&title=%E6%89%93%E5%8C%85%E6%88%90%E5%8A%9F%E7%9A%84jar "打包成功的jar")<br />如果服务器有 java 环境，可以直接用下面的命令启动
```bash
java -jar hello-mogu-0.0.1-SNAPSHOT.jar
```
下面，可以继续编写流水线，创建项目的 **Dockerfile** 文件，<br />Dockerfile的主要作用是用来构建镜像的
```dockerfile
FROM registry.cn-shenzhen.aliyuncs.com/mogu-zh/jdk:8-mogu-alpine
ENV LANG C.UTF-8
ENV TZ Asia/Shanghai
VOLUME /tmp
ADD hello-mogu-0.0.1-SNAPSHOT.jar hello-mogu-0.0.1-SNAPSHOT.jar
ENTRYPOINT ["java","-Xms256m","-Xmx256m","-jar","/hello-mogu-0.0.1-SNAPSHOT.jar"]
```
存放位置如下所示，主要拉取了带着 jdk8 环境的镜像，然后设置启动参数<br />![编写Dockerfile](https://cdn.nlark.com/yuque/0/2022/png/396745/1662533062200-e9e61cee-0c0d-427f-87f8-768d21800f60.png#clientId=uf2f7c7d5-6176-4&from=paste&id=ud9521492&originHeight=365&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u9815caf9-c000-46f4-8d0e-0dcb23bd08f&title=%E7%BC%96%E5%86%99Dockerfile "编写Dockerfile")<br />继续编写 .drone.yml 文件，这里除了需要拷贝 **jar** 文件外，还需要把刚刚写的 **Dockerfile** 文件也拷贝到宿主机上<br />同时，引入 **appleboy/drone-ssh** 镜像，听名字就可以知道，这个命令是用来远程 **SSH** 连接服务器的<br />这里有两个变量：**TEST_SERVER_IP** 和 **TEST_SERVER_PASSWORD**，分别是服务器的 **ip** 和 **密码**。为了防止信息泄露，需要配置到 **secret**<br />![新增秘钥](https://cdn.nlark.com/yuque/0/2022/png/396745/1662533062307-99a41cea-df1a-42e8-9707-2dd014f2a54f.png#clientId=uf2f7c7d5-6176-4&from=paste&id=u8254210a&originHeight=451&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u2597abb5-4433-4247-853a-f2e1a15ab28&title=%E6%96%B0%E5%A2%9E%E7%A7%98%E9%92%A5 "新增秘钥")<br />然后填写秘钥的名称和值，保存即可<br />![创建秘钥](https://cdn.nlark.com/yuque/0/2022/png/396745/1662533062395-bc67f4fd-e0ae-4c17-977d-cf525c819053.png#clientId=uf2f7c7d5-6176-4&from=paste&id=uc8f4e04c&originHeight=588&originWidth=1030&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=udb2ecb1f-2698-4c0e-9b30-c705d0d4077&title=%E5%88%9B%E5%BB%BA%E7%A7%98%E9%92%A5 "创建秘钥")<br />配置完成后，一共包含以下两个值<br />![创建成功](https://cdn.nlark.com/yuque/0/2022/png/396745/1662533062637-d4ee9732-ba63-4965-9581-788380e23ee6.png#clientId=uf2f7c7d5-6176-4&from=paste&id=u8d8a5479&originHeight=390&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=udc93cb73-592f-4eaf-ba28-cc7e3888dc8&title=%E5%88%9B%E5%BB%BA%E6%88%90%E5%8A%9F "创建成功")<br />完整的流水线代码如下：
```yaml
kind: pipeline # 定义对象类型，还有secret和signature两种类型
type: docker # 定义流水线类型，还有kubernetes、exec、ssh等类型
name: hello-mogu # 定义流水线名称

steps: # 定义流水线执行步骤，这些步骤将顺序执行

  - name: build-package # 流水线名称
    image: maven:3.8.5-openjdk-8 # 定义创建容器的Docker镜像
    volumes: # 将容器内目录挂载到宿主机，仓库需要开启Trusted设置
      - name: maven-build
        path: /root/hello-mogu # 将应用打包好的Jar和执行脚本挂载出来
    commands:
      - mvn clean package -DskipTests=true
      # 将打包后的jar包，拷贝到挂载目录
      - cp /drone/src/target/hello-mogu-0.0.1-SNAPSHOT.jar  /root/hello-mogu
      # 将Dockerfile拷贝到挂载目录
      - cp /drone/src/target/classes/Dockerfile /root/hello-mogu

  - name: ssh
    image: appleboy/drone-ssh
    settings:
      # 你服务器ip地址
      host:
        from_secret: TEST_SERVER_IP
      # 服务器账号
      username: root
      # 密码登入写法
      password:
        from_secret: TEST_SERVER_PASSWORD
      port: 22
      script:
        - cd /root/hello-mogu
        - ls
        - docker build  -t hello-mogu:latest .
        - docker run -p 8080:8080 -d hello-mogu:latest

volumes: # 定义流水线挂载目录，用于共享数据
  - name: maven-build
    host:
      path: /root/hello-mogu   #jar包目录可以修改从宿主机中挂载的目录
```
核心操作就是：在 jar 打包完成后，会通过 ssh 进入到服务器中，通过 Dockerfile 构建 **hello-mogu** 镜像，同时使用 `**docker run**` 启动镜像，完成最简单的一个流水线工作，以下是流水线运行成功的截图：<br />![构建成功](https://cdn.nlark.com/yuque/0/2022/png/396745/1662533062648-145fa281-8d4a-4b51-8f23-5a24a7a8f0d3.png#clientId=uf2f7c7d5-6176-4&from=paste&id=u29b2c860&originHeight=516&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u072459fb-f258-4dff-bfcc-f5b88a7cc74&title=%E6%9E%84%E5%BB%BA%E6%88%90%E5%8A%9F "构建成功")<br />下面，去服务器中，使用 `docker images` 命令，即可查看到制作完成的镜像了<br />![查看容器](https://cdn.nlark.com/yuque/0/2022/png/396745/1662533062672-957ae076-8ba2-492f-b119-8de5c0fd5dac.png#clientId=uf2f7c7d5-6176-4&from=paste&id=u618aeed3&originHeight=65&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u6f22f83f-69b5-430e-82d9-fba5ef22c2f&title=%E6%9F%A5%E7%9C%8B%E5%AE%B9%E5%99%A8 "查看容器")<br />通过使用 `docker ps -a` ，可以看到目前 hello-mogu 容器正在运行<br />![查看运行的容器](https://cdn.nlark.com/yuque/0/2022/png/396745/1662533062787-bd5a18a0-ed6e-4634-9dec-d851433c8c87.png#clientId=uf2f7c7d5-6176-4&from=paste&id=ua17ac06f&originHeight=110&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ub2b9f3dd-a945-4425-8776-43f2d3926a4&title=%E6%9F%A5%E7%9C%8B%E8%BF%90%E8%A1%8C%E7%9A%84%E5%AE%B9%E5%99%A8 "查看运行的容器")<br />最后，访问服务器：http://81.70.100.87:8080/hello-mogu，久违的 **hello-mogu** 终于出现了<br />![hello mogu](https://cdn.nlark.com/yuque/0/2022/png/396745/1662533062823-601cb284-77e5-47b5-b3c0-6f26dec0ed57.png#clientId=uf2f7c7d5-6176-4&from=paste&id=u11643e0b&originHeight=161&originWidth=519&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u1853136d-d9c2-4f32-8806-55606ff1bc4&title=hello%20mogu "hello mogu")<br />同时，**Drone** 还提供了很多插件，可以打开 [https://plugins.drone.io/](https://plugins.drone.io/) 进行查看<br />![drone插件](https://cdn.nlark.com/yuque/0/2022/png/396745/1662533062969-f5eea3a3-7f95-4ad3-837c-2763f8fbc0dc.png#clientId=uf2f7c7d5-6176-4&from=paste&id=u688549f0&originHeight=538&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ud40b1d61-13e3-4ef1-97bc-6eacaf8ab9d&title=drone%E6%8F%92%E4%BB%B6 "drone插件")<br />在这里，可以下载别人做好的插件，例如在构建成功后，发送邮件通知，这里用到了 **Email** 插件<br />![Email插件](https://cdn.nlark.com/yuque/0/2022/png/396745/1662533063034-bb50a022-770c-4eda-95fd-4508cb794379.png#clientId=uf2f7c7d5-6176-4&from=paste&id=ub2a7e370&originHeight=670&originWidth=882&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u0f6462ee-038c-45c4-8728-4cd3af5b7dd&title=Email%E6%8F%92%E4%BB%B6 "Email插件")<br />或者使用 Drone Cache 插件，将中间结果缓存到**云存储**中<br />![Drone缓存插件](https://cdn.nlark.com/yuque/0/2022/png/396745/1662533063071-259c0e23-9b7c-4811-9296-692481f6a5d4.png#clientId=uf2f7c7d5-6176-4&from=paste&id=ubc76a22a&originHeight=846&originWidth=1041&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u7b6a2245-2d75-48ba-a2a2-b1ec68d8b5f&title=Drone%E7%BC%93%E5%AD%98%E6%8F%92%E4%BB%B6 "Drone缓存插件")
