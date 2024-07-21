Docker
<a name="PLTG4"></a>
## Docker 的总体架构图
Docker 是一个 C/S 模式的架构，后端是一个松耦合架构，模块各司其职。下图是它的总体架构图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666437226018-2842881d-8714-4cc2-a1c1-155bce626554.png#clientId=ub218caa3-224c-4&from=paste&id=ue8151c9e&originHeight=1492&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u99d0b63e-8727-4555-9f2c-ec3e600e761&title=)<br />1、用户是使用 Docker Client 与 Docker Daemon 建立通信，并发送请求给后者。<br />2、Docker Daemon 作为 Docker 架构中的主体部分，首先提供 Docker Server 的功能使其可以接受 Docker Client 的请求。<br />3、Docker Engine 执行 Docker 内部的一系列工作，每一项工作都是以一个 Job 的形式的存在。<br />4、Job 的运行过程中，当需要容器镜像时，则从 Docker Registry 中下载镜像，并通过镜像管理驱动 Graphdriver 将下载镜像以 Graph 的形式存储。<br />5、当需要为 Docker 创建网络环境时，通过网络管理驱动 Networkdriver 创建并配置 Docker容器网络环境。<br />6、当需要限制 Docker 容器运行资源或执行用户指令等操作时，则通过 Execdriver 来完成。<br />7、Libcontainer 是一项独立的容器管理包，Networkdriver 以及 Execdriver 都是通过 Libcontainer 来实现具体对容器进行的操作。
<a name="b4Ni3"></a>
## Docker 各模块组件分析
<a name="xGky7"></a>
### 一、Docker Client「发起请求」
1、Docker Client 是 和 Docker Daemon 建立通信的客户端。用户使用的可执行文件为 docker（一个命令行可执行文件），docker 命令使用后接参数的形式来实现一个完整的请求命令（例如：docker images，docker 为命令不可变，images 为参数可变）。<br />2、Docker Client 可以通过以下三种方式和 Docker Daemon 建立通信：`tcp://host:port`、`unix://pathtosocket` 和 `fd://socketfd`<br />3、Docker Client 发送容器管理请求后，由 Docker Daemon 接受并处理请求，当 Docker Client 接收到返回的请求相应并简单处理后，Docker Client 一次完整的生命周期就结束了。(一次完整的请求：发送请求→处理请求→返回结果)，与传统的 C/S 架构请求流程并无不同。
<a name="egunw"></a>
### 二、Docker Daemon（后台守护进程）
Docker daemon 架构图：<br />![640.jpg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1618884995834-e75248c8-eef7-4117-9555-1dd25bbacb45.jpeg#clientId=ue7ee0379-edcf-4&errorMessage=unknown%20error&from=ui&id=GcWZh&originHeight=454&originWidth=530&originalType=binary&ratio=1&rotation=0&showTitle=false&size=70250&status=error&style=shadow&taskId=u043d8525-8dd8-49c9-9c1d-d3fa60d788b&title=)<br />Docker Server 架构图：<br />![640.jpg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1618892968175-de75ecdc-e12b-4c1b-9362-f9b12bdae564.jpeg#clientId=uc190c83a-3d72-4&errorMessage=unknown%20error&from=ui&id=aINrc&originHeight=377&originWidth=519&originalType=binary&ratio=1&rotation=0&showTitle=false&size=63592&status=error&style=shadow&taskId=ue1f22e84-28d1-4eb2-b140-96b029c9dd2&title=)<br />1、Docker Server 相当于 C/S 架构的服务端。功能为接受并调度分发 Docker Client 发送的请求。接受请求后，Docker Server 通过路由与分发调度，找到相应的 Handler 来执行请求。<br />2、在 Docker 的启动过程中，通过包 gorilla/mux 创建了一个 `mux.Router` 来提供请求的路由功能。在 Golang 中 gorilla/mux 是一个强大的 URL 路由器以及调度分发器。该 `mux.Router` 中添加了众多的路由项，每一个路由项由 HTTP 请求方法（PUT、POST、GET 或DELETE）、URL、Handler 三部分组成。<br />3、创建完 `mux.Router` 之后，Docker 将 Server 的监听地址以及 `mux.Router` 作为参数来创建一个 `httpSrv=http.Server{}`，最终执行 `httpSrv.Serve()` 为请求服务。<br />4、在 Docker Server 的服务过程中，Docker Server 在 listener 上接受 Docker Client 的访问请求，并创建一个全新的 goroutine 来服务该请求。在 goroutine 中，首先读取请求内容并做解析工作，接着找到相应的路由项并调用相应的 Handler 来处理该请求，最后 Handler 处理完请求之后回复该请求。
<a name="C3QNS"></a>
### 三、Docker Engine
1、Docker Engine 是 Docker 架构中的运行引擎，同时也 Docker 运行的核心模块。它扮演 Docker Container 存储仓库的角色，并且通过执行 Job 的方式来操纵管理这些容器。<br />2、在 Docker Engine 数据结构的设计与实现过程中，有一个 Handler 对象。该 Handler 对象存储的都是关于众多特定 Job 的 Handler 处理访问。举例说明：Docker Engine 的Handler 对象中有一项为：`{"create": daemon.ContainerCreate,}`，则说明当名为”create” 的 Job 在运行时，执行的是 `daemon.ContainerCreate` 的 Handler。
<a name="OdGy3"></a>
##### Job
1、一个 Job 可以认为是 Docker 架构中 Docker Engine 内部最基本的工作执行单元。Docker 可以做的每一项工作，都可以抽象为一个 Job。例如：在容器内部运行一个进程，这是一个 Job；创建一个新的容器，这是一个 Job。Docker Server 的运行过程也是一个 Job，名为 ServeApi。<br />2、Job 的设计者，把 Job 设计得与 Unix 进程相仿。比如说：Job 有一个名称、有参数、有环境变量、有标准的输入输出、有错误处理，有返回状态等。
<a name="xgnxf"></a>
### 四、Docker Registry(镜像注册中心)
1、Docker Registry 是一个存储容器镜像的仓库（注册中心），可理解为云端镜像仓库。按 Repository 来分类，docker pull 按照 [repository]:[tag] 来精确定义一个具体的 Image。<br />2、在 Docker 的运行过程中，Docker Daemon 会与 Docker Registry 通信，并实现搜索镜像、下载镜像、上传镜像三个功能，这三个功能对应的 Job 名称分别为：“search”、”pull” 与 “push”。<br />3 Docker Registry 可分为公有仓库（ Docker Hub）和私有仓库。
<a name="c0iZV"></a>
### 五、Graph 「Docker 内部数据库」
Graph 架构图：<br />![2022-10-22-19-14-20.553142400.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1666437382698-26f84ed8-fd1c-4c96-94a0-7810a7eb86e4.png#clientId=ub218caa3-224c-4&from=ui&id=uf8d6728f&originHeight=332&originWidth=514&originalType=binary&ratio=1&rotation=0&showTitle=false&size=513184&status=done&style=shadow&taskId=u32796530-9484-490f-ad03-1cfbaf0b6d5&title=)<br />Repository<br />1、已下载镜像的保管者（包括下载的镜像和通过 Dockerfile 构建的镜像）。<br />2、一个 Repository 表示某类镜像的仓库（例如：Ubuntu），同一个 Repository 内的镜像用 Tag 来区分（表示同一类镜像的不同标签或版本）。一个 Registry 包含多个Repository，一个 Repository 包含同类型的多个 Image。<br />3、镜像的存储类型有 Aufs、Devicemapper、Btrfs、Vfs等。其中 CentOS 系统 7.x 以下版本使用 Devicemapper 的存储类型。<br />4、同时在 Graph 的本地目录中存储有关于每一个的容器镜像具体信息，包含有：该容器镜像的元数据、容器镜像的大小信息、以及该容器镜像所代表的具体 rootfs。<br />GraphDB<br />1、已下载容器镜像之间关系的记录者。<br />2、GraphDB 是一个构建在 SQLite 之上的小型数据库，实现了节点的命名以及节点之间关联关系的记录。
<a name="NvzZE"></a>
### 六、Driver 「执行部分」
Driver 是 Docker 架构中的驱动模块。通过 Driver 驱动，Docker 可以实现对 Docker 容器执行环境的定制。即 Graph 负责镜像的存储，Driver 负责容器的执行。
<a name="fIsBY"></a>
#### Graphdriver
Graphdriver 架构图：<br />![640.jpg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1618885238824-783542f0-e1d2-49bb-a9b8-774a8a209213.jpeg#clientId=u97d2461c-9f0c-4&errorMessage=unknown%20error&from=ui&id=FWbF0&originHeight=440&originWidth=521&originalType=binary&ratio=1&rotation=0&showTitle=false&size=60096&status=error&style=shadow&taskId=u63b65887-5ae3-4ab3-b65a-bdcf73cee63&title=)<br />1、Graphdriver 主要用于完成容器镜像的管理，包括存储与获取。<br />2、存储：`docker pull` 下载的镜像由 Graphdriver 存储到本地的指定目录( Graph 中 )。<br />3、获取：`docker run（create）`用镜像来创建容器的时候由 Graphdriver 到本地 Graph中获取镜像。
<a name="gR8BX"></a>
#### Networkdriver
Networkdriver 架构图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666437422272-60f669e0-1e64-457a-9e9d-5d8c17251cb9.png#clientId=ub218caa3-224c-4&from=paste&id=u2ea3ce36&originHeight=428&originWidth=526&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud7ed91d7-533e-439b-9dd5-f43dcd8630f&title=)<br />Networkdriver 的用途是完成 Docker 容器网络环境的配置，其中包括：

- Docker 启动时为 Docker 环境创建网桥。
- Docker 容器创建时为其创建专属虚拟网卡设备。
- Docker 容器分配IP、端口并与宿主机做端口映射，设置容器防火墙策略等。
<a name="HXkzV"></a>
#### Execdriver
Execdriver 架构图：<br />![640.jpg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1618885356383-9841f35c-1852-40d9-9efa-dd87bfda67fc.jpeg#clientId=u50607733-fd06-4&errorMessage=unknown%20error&from=ui&id=IMic7&originHeight=461&originWidth=527&originalType=binary&ratio=1&rotation=0&showTitle=false&size=57430&status=error&style=shadow&taskId=u94ba2614-b5fb-4ee8-b1c9-9a093386986&title=)<br />1、Execdriver 作为 Docker 容器的执行驱动，负责创建容器运行命名空间、容器资源使用的统计与限制、容器内部进程的真正运行等。<br />2、现在 Execdriver 默认使用 Native 驱动，不依赖于 LXC。
<a name="q7xZB"></a>
### 七、Libcontainer 「函数库」
Libcontainer 架构图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666437437390-eda2d7d7-2453-427e-90f7-d9feb25f0251.png#clientId=ub218caa3-224c-4&from=paste&id=u8504038e&originHeight=367&originWidth=539&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u89285ef7-4a1f-46bd-a56c-8b3ba991de1&title=)<br />1、Libcontainer 是 Docker 架构中一个使用 Go 语言设计实现的库，设计初衷是希望该库可以不依靠任何依赖，直接访问内核中与容器相关的 API。<br />2、Docker 可以直接调用 Libcontainer 来操纵容器的 Namespace、Cgroups、Apparmor、网络设备以及防火墙规则等。<br />3、Libcontainer 提供了一整套标准的接口来满足上层对容器管理的需求。或者说 Libcontainer 屏蔽了 Docker 上层对容器的直接管理。
<a name="aDZnE"></a>
### 八、Docker Container 「服务交付的最终形式」
Docker Container 架构：<br />![2022-10-22-19-14-20.827886700.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1666437846170-600d1883-bc0e-4536-bc19-d7c431250042.png#clientId=ub218caa3-224c-4&from=ui&id=ufe89a841&originHeight=472&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1532298&status=done&style=shadow&taskId=uab0c2d9f-7ed7-480a-a873-f0bf84a80b5&title=)<br />1、Docker Container（ Docker 容器 ）是 Docker 架构中服务交付的最终体现形式。<br />2、Docker 按照用户的需求与指令，订制相应的 Docker 容器：

- 用户通过指定容器镜像，使得 Docker 容器可以自定义 rootfs 等文件系统。
- 用户通过指定计算资源的配额，使得 Docker 容器使用指定的计算资源。
- 用户通过配置网络及其安全策略，使得 Docker 容器拥有独立且安全的网络环境。
- 用户通过指定运行的命令，使得 Docker 容器执行指定的工作。
