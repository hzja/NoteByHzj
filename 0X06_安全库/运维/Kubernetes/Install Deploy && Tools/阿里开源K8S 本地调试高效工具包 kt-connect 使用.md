Kubernetes
<a name="YCrSB"></a>
## 背景
注：背景有点啰嗦，讲讲一路走来研发本地调试的变化，嫌烦的可以直接跳过，不影响阅读。
<a name="zAtti"></a>
### 2019 年
当时公司只有两个 Java 应用，还都跑在一个 Tomcat Servlet 容器。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666332621808-4f4bea3d-ffe6-4012-abef-1fe24b392fd8.png#clientId=u1320ecc5-6259-4&from=paste&id=u090f0015&originHeight=252&originWidth=701&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2da8083d-9860-4e89-9854-515735d9ecc&title=)<br />当时是如何本地调试？都是研发自己电脑装个 Mysql，装个 Tomcat，自己电脑运行调试，好处嘛就是后端研发互不干扰，想怎么改就怎么改，APP 端研发就直连后端的笔记本调试。上线部署嘛就是一个研发手动编译个 Jar 包丢到云服务器上面，大体就是个草台班子，能干活，但是也就那样。
<a name="MY2fT"></a>
### 2020 年
到了 2020 年，公司买了一台服务器，Centos 的系统，给装上了 Mysql、Tomcat，用上了 Redis 缓存，RabbitMQ 消息队列，有了独立的测试环境，用上了 Jenkins 自动打包并部署应用，也算鸟枪换炮，起码不用自己打包了。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666332621869-e9470abb-bceb-478e-a0c8-af9c257f41fb.png#clientId=u1320ecc5-6259-4&from=paste&id=u6e26330d&originHeight=237&originWidth=1062&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua454646a-682a-4be8-9320-a81d90c1718&title=)<br />这个时候是如何本地调试呢？起码不用自己电脑装 Mysql 了，后面框架由 SpringMVC 和 Struts2 都改成 Spring Boot，外置的 Tomcat 也可以去掉了。后端研发本地运行 Spring Boot 时直连服务器的 Mysql 进行调试，APP 端再也不用连后端研发的笔记本了，有了相对稳定的调试环境。代价就是各个后端的数据库更新结构要保持兼容性，避免影响他人。
<a name="op5v0"></a>
### 2021 年
随着业务增长，后端框架由 Spring Boot 进化为 Spring Cloud 全家桶，应用运行环境由 Linux 直接运行改为了 Docker 镜像部署，各类中间件同样也使用了 Docker 镜像。产品线增加，单一的开发分支已经不能满足需求，为此又开辟了另外一条后端代码分支，同样的开发测试环境也多了一份。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666332621898-7d40a468-5a1c-45bf-b64d-ef7569c86f86.png#clientId=u1320ecc5-6259-4&from=paste&id=ue90c2d1f&originHeight=280&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3127c854-9b03-4fa3-b4ad-753d383de6a&title=)<br />这个时候的本地调试，对于 APP 端来说变化不大，区别连接后端不同环境使用不同域名而已。对于后端的研发同学就不一样了，每次本地调试自己电脑要常驻一个 Eureka 和一个 Config Server，如果本地调试的微服务依赖比较多，没个大内存真是顶不住。
<a name="gabp9"></a>
### 2022 年
业务量继续增加，产品同事数量增加了，那个需求量真是堆积如山，两个分支已经不能满足要求了，又开了第三个分支，还是不够。每次增加新的分支运行环境，后端研发同学也很痛苦，一堆环境和第三方平台回调需要配置。为了能动态扩容缩容，Spring Cloud 全家桶继续演进，抛弃了 Zuul 网关和 Eureka，改为使用 Spring Cloud Kubernetes，运行环境全面向 K8S 靠拢。在此期间公司又采购了一台服务器用于开发测试，内存 CPU 磁盘满上!<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666332621905-17a19130-370b-47d0-bb09-19c846ce984d.png#clientId=u1320ecc5-6259-4&from=paste&id=u18dbcf01&originHeight=471&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u263860cb-e740-4164-ae87-66a0419ee84&title=)<br />进入 K8S 时代，后端研发本地的电脑没办法随意连接 Linux 服务器上面的各种中间件，每个新分支环境里面的每个 POD 都是一个新的 ip，也不可能像之前那样开放指定几个中间件的端口给后端连接，那么多环境每个都做设置的话，运维同学整天不用干别的事了。也由此引出了今天要说的 kt-connect 工具，通过这个工具，后端研发本地的电脑可以代理访问到各个分支环境，也就是 K8S 里面的命名空间的所有服务，并且只需要启动需要调试的服务，大大节省了电脑 CPU 内存占用。
<a name="nRCHV"></a>
## 选型
在选择代理访问 K8S 环境以便于本地调试的工具中，网上有几种。
<a name="FNARb"></a>
### 1、端口转发
使用 Ingress、NodePort、LoadBalancer 之类的将流量转发到指定端口，如上文所说，会让运维同学工作量比较大，也不便于分支环境的自动创建和回收，只适合需要暴露端口数量不多的场景。
<a name="Yz1qB"></a>
### 2、VPN
通过在 K8S 每个命名空间里面设置一个运行有 VPN 服务的 POD，后端研发笔记本通过 VPN 客户端连接代理进入到指定命名空间，可以正常访问和解析集群内各类服务，基本能满足日常的要求，缺点是每个命名空间都常驻了一个 VPN 服务的运行资源。
<a name="f3NIn"></a>
### 3、Telepresence
在搜索的过程中发现了这个代理工具，几乎可以说 9 成的中英文技术文章都推荐使用这个工具，功能非常强大，不但提供了 VPN 所具有的代理功能，可以访问到命名空间内所有服务，还能指定各种规则拦截指定服务的流量到本地机器，相当于本地机器也能作为一个普通的 POD 提供对外服务。大体设计原理如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666332621879-2ba687fe-5fc0-40fb-a3fb-dabbf9ed5ff1.png#clientId=u1320ecc5-6259-4&from=paste&id=u3c5d7ee3&originHeight=318&originWidth=779&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ufacb1a75-0383-44a9-bc5d-93236b8c887&title=)<br />在研发本地电脑执行如下命令
```bash
telepresence helm install --kubeconfig .kubeconfig
telepresence connect ---kubeconfig .kubeconfig
```
就会自动在 K8S 集群创建一个命名空间 ambassador，并且部署一个 traffic-manager 的 pod，用于流量管理，而在研发笔记本本地则会启动 2 个 daemon 服务，其中一个叫 Root Daemon，用于建立一条双向代理通道，并管理本地电脑与 K8S 集群之间的流量，另外一个 User Daemon 则是负责与 Traffic Manager 通信，设置拦截规则，如果登录后还负责与 Ambassador Cloud 进行通信。<br />通过配置拦截规则，拦截的 POD 里面会安装一个 traffic-agent，官方文档说明是类似 K8S 集群的 sidecar 模式，对注入 POD 进行流量劫持，所有流量出入通过 traffic-manager 进行重新路由。<br />The Traffic Agent is a sidecar container that facilitates intercepts. When an intercept is first started, the Traffic Agent container is injected into the workload's pod(s).<br />虽然他的功能很强大，但是在目前 2.5 版本的使用过程中，为了使用他的拦截和 Preview Url 功能必须在他家的商业云平台 Ambassador Cloud 进行注册登陆(注：不知道为什么网上技术文章都没提到这点，测试的时候非得要登录他家云平台)，并且拦截规则的配置是通过云平台的网页进行操作的，联网的要求，包括可能存在的安全，泄露之类的隐患，我觉得是不可接受，也因此不得不放弃使用这个工具。<br />还有一个不得不说的缺点就是，老版本使用后可以清理掉自动创建的命名空间（namespace）和 pod、拦截 agent 的功能（telepresence uninstall）也没了，在 2.5 版本的命令参数里面完全消失了，这就导致每次使用后，如果想保持环境干净，还得麻烦运维同学去清理掉，非常麻烦，简直逼死洁癖患者。
<a name="BFzDN"></a>
### 4、kt-connect
所幸开源社区又找到了另外一款类似 Telepresence 的工具，名为 kt-connect：<br />[_https://github.com/alibaba/kt-connect_](https://github.com/alibaba/kt-connect)<br />使用版本为 v0.3.6（顺便说下使用的 K8S 版本是 1.24），并且它无需联网登陆什么账号，结束命令执行默认还会自动清理。阿里出品，不确定是不是又一个 KPI 开源项目，但是至少这一刻对这个工具是非常满意的。
<a name="WwcIH"></a>
## 原理
同 Telepresence 类似，但不同的是，kt-connect 只会在指定连接的命名空间（namespace）里面新建一个自用的 pod，然后部署一个 kt-connect-shadow 的镜像。相比 Telepresence，它在模式进行了细分扩展，分为四大模式：
<a name="MOvsm"></a>
### 1、Connect 模式
```
ktctl.exe connect --kubeconfig .kubeconfig --namespace feature-N --debug
```
这个模式下，kt-connect 起到的是一个类似于 VPN 的作用，研发本地电脑可以访问到连接的命名空间(namespace)内的所有服务，但是并没有加到集群里面其他服务里面，其他服务的流量并不会转发到本地电脑。<br />**注 1：** 与 telepresence 类似，kt-connect 所有命令都要带上--kubeconfig，确保有足够权限和能正确连接 K8S 集群的 API Server，很多文章都很少提到这点，假如 K8S 集群限制权限，或者与研发不在同一个网络，必须确保使用运维同学提供的有足够权限的授权文件 kubeconfig 来进行连接。<br />**注 2：**
```
Failed to setup port forward local:28344 -> pod kt-connect-shadow-gseak:53 error="error upgrading connection: error sending request: Post "[https://10.0.8.101:8443/api/v1/namespaces/feature-N/pods/kt-connect-shadow-gseak/portforward](https://10.0.8.101:8443/api/v1/namespaces/feature-N/pods/kt-connect-shadow-gseak/portforward)": dial tcp 10.0.8.101:8443: connectex: A socket operation was attempted to an unreachable host."，
```
如果出现以上报错的话，有可能是 kt-connect 路由 BUG，可能本地电脑的路由与新加的通往 API Server 的路由有冲突，增加参数--excludeIps 10.0.8.101/32 即可，如果网段冲突比较多，可以扩大网段范围，例如--excludeIps 10.0.8.0/24 参考 issue-302:<br />[_https://github.com/alibaba/kt-connect/issues/302_](https://github.com/alibaba/kt-connect/issues/302)
```
ktctl.exe connect --kubeconfig .kubeconfig --namespace feature-N --excludeIps 10.0.8.101/32 --debug
```
<a name="gqeXU"></a>
### 2、Exchange 模式
```
ktctl.exe exchange serviceA --kubeconfig .kubeconfig --namespace feature-N --expose 12001 --debug
```
这个模式类似于 Telepresence 拦截模式，将指定服务的所有流量拦截下来转发到研发本地电脑的端口，使用这个模式能对环境里的访问请求直接进行调试。<br />具体原理就是将 service 里面的 pod 替换成一个 serviceA-kt-exchange 的 pod。<br />**注 1：** Exchange 模式的流量方向是单向的，并不会将本地电脑主动发起的请求代理过去，如果 K8S 集群跟研发本地电脑不在一个网段内，需要另外开一个命令行运行 Connect 模式，确保本地服务可以正常连接 K8S 集群的其他服务，参考 issue-216：<br />[_https://github.com/alibaba/kt-connect/issues/216_](https://github.com/alibaba/kt-connect/issues/216)<br />**注 2：** Exchange 模式是通过拦截 service 进行流量转发，假如集群的请求没有经过 service，例如直接解析到 pod 之类，可能就会出现拦截失败的情况（同理 Mesh 模式也是如此），所以出现问题记得跟运维同学确认 K8S 集群内的路由情况。
<a name="OBv0V"></a>
### 3、Mesh 模式
```bash
kctl.exe mesh serviceA --kubeconfig .kubeconfig --namespace feature-N --expose 12001 --debug
```
执行命令后可以看到输出日志里面包含类似文字：
```
2:30PM INF Now you can access your service by header 'VERSION: xxxxx'
```
这个模式本地电脑的服务和 K8S 集群里面相同的服务同时对外响应请求，但是只有通过指定的 http 请求头 VERSION: xxxx 的请求才会转发到本地电脑，相比 Exchange 模式，保证了其他人服务正常使用，同时研发又能进行本地调试。每次生成的请求头 VERSION 的值都是动态生成的，如果要固定这个值，可以通过参数--versionMark 写死，例如固定值为 test-version，命令如下：
```bash
kctl.exe mesh serviceA --kubeconfig .kubeconfig --namespace feature-N --expose 12001 --debug --versionMark test-version
```
具体原理就是将 serviceA 里面的 Pod 替换成一个 serviceA-kt-router 的路由镜像，负责根据请求头进行流量代理转发，另外生成一个 serviceA-kt-stuntman 服务，这个就是线上正常运行的 serviceA，还有一个 serviceA-kt-mesh-xxxxx 服务，这个就负责将代理流量到本地电脑。
<a name="o5RBf"></a>
### 4、Preview 模式
```bash
kctl.exe preview serviceB --kubeconfig .kubeconfig --namespace feature-N --expose 12001
```
不同于 Exchange 和 Mesh 模式要求 K8S 集群有一个在运行的服务，Preview 模式可以将本地电脑运行的程序部署到 K8S 集群中作为一个全新的 Service 对外提供服务，非常便于新建服务的开发调试、预览等作用。
