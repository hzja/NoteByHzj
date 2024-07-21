Kubernetes
<a name="biKDH"></a>
## Kuboard-Spray
Kuboard-Spray 是一款可以在图形界面引导下完成 Kubernetes 高可用集群离线安装的工具，开源仓库的地址为 [Kuboard-Spray](https://github.com/eip-work/kuboard-spray)<br />安装后的集群版本为

- Kubernetes v1.23.1
<a name="rgJcW"></a>
## 配置要求
对于 Kubernetes 初学者，在搭建K8S集群时，推荐采购如下配置：（也可以使用自己的虚拟机、私有云等最容易获得的 Linux 环境）

- 至少 2 台 2核4G 的服务器
- 本文档中，CPU 必须为 x86 架构，暂时未适配 arm 架构的 CPU
- CentOS 7.8、 CentOS 7.9 或 Ubuntu 20.04
<a name="QEZ8G"></a>
### 操作系统兼容性
| 操作系统版本 | 本文档是否兼容 | 备注 |
| --- | --- | --- |
| CentOS7.9  | 是 | 已验证 |
| CentOS7.8 | 是 | 已验证 |
| Ubuntu20.04 | 是 | 已验证 |

<a name="uIhTm"></a>
## 安装 Kuboard-Spray
取一台服务器或虚拟机，执行一条命令，即可完成 Kuboard-Spray 的安装。<br />对这台服务器的最低要求为：

- 1核2G
- 不少于 10G 磁盘空余空间
- 已经安装好 docker

待执行的命令如下：
```bash
docker run -d \
--restart=unless-stopped \
--name=kuboard-spray \
-p 80:80/tcp \
-v /var/run/docker.sock:/var/run/docker.sock \
-v /root/kuboard-spray-data:/data \
eipwork/kuboard-spray:v1.0.0-alpha.1-amd64
```
稍后会看到：
```bash
# docker ps -a
CONTAINER ID   IMAGE                                        COMMAND             CREATED          STATUS          PORTS                               NAMES
41b1a241a852   eipwork/kuboard-spray:v1.0.0-alpha.1-amd64   "./kuboard-spray"   26 minutes ago   Up 25 minutes   0.0.0.0:80->80/tcp, :::80->80/tcp   kuboard-spray
```
<a name="PMweQ"></a>
### 持久化

- KuboardSpray 的信息保存在容器的 /data 路径，请将其映射到一个安全的地方，上面的命令中，将其映射到了 /root/kuboard-spray-data 路径；
- 只要此路径的内容不受损坏，重启、升级、重新安装 Kuboard-Spray，或者将数据及 Kuboard-Spray 迁移到另外一台机器上，都可以找回到原来的信息。

在浏览器打开地址 http://这台机器的IP，输入默认密码 Kuboard123，即可登录 Kuboard-Spray 界面。
<a name="OXrEN"></a>
## 加载离线资源包

- 在 Kuboard-Spray 界面中，导航到 系统设置 --> 资源包管理 界面，可以看到已经等候多时的 Kuboard-Spray 离线资源包，如下图所示：

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641256202333-e598a433-a73b-4986-9f3c-959b578bcb59.webp#clientId=u7d8dee2a-f082-4&from=paste&id=ub12f5d84&originHeight=608&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5125806c-415b-4be6-9545-bd454da7012&title=)

- 点击 导 入 按钮，在界面的引导下完成资源包的加载。

离线导入：<br />如果处在内网环境，上图中的列表默认将是空的，请注意其中的 离线加载资源包 按钮，它可以引导完成资源包的离线加载过程。<br />任务日志如下：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641256202362-39065ab5-f7b5-4b27-8cb1-ad453dd7e258.webp#clientId=u7d8dee2a-f082-4&from=paste&id=u3a1b8ccf&originHeight=629&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u12bfcf0a-0df2-4bf8-a329-1c6ffa362c3&title=)<br />任务已经完成：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641256202375-c02aaabd-9fde-48ab-82db-305732b8e099.webp#clientId=u7d8dee2a-f082-4&from=paste&id=u1e2dfb2e&originHeight=295&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud40715ba-4936-434e-a44d-52afeb1f516&title=)
<a name="UB1ob"></a>
## 规划并安装集群
在 Kuboard-Spray 界面中，导航到 集群管理 界面，点击界面中的 添加集群安装计划 按钮，填写表单如下：

- 集群名称：自定义名称，本文中填写为 kuboard123，此名称不可以修改；
- 资源包：选择前面步骤中导入的离线资源包。

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641256202344-179b6f0a-6580-4b43-8d21-63711e591963.webp#clientId=u7d8dee2a-f082-4&from=paste&id=ucef21a00&originHeight=608&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ufe86a43c-088f-424e-bfac-6bf4a11e09b&title=)<br />点击上图对话框中的 确定 按钮后，将进入集群规划页面，在该界面中添加每个集群节点的连接参数并设置节点的角色，如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641256202506-9de7b9ae-cfc8-4877-b147-c7c894f38303.webp#clientId=u7d8dee2a-f082-4&from=paste&id=uf2e6c944&originHeight=545&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf79748d2-2e47-409f-a84b-65b3579e114&title=)
<a name="W2y2h"></a>
### 注意事项

- 最少的节点数量是 1 个；
- ETCD 节点、控制节点的总数量必须为奇数；
- 在 全局设置 标签页，可以设置节点的通用连接参数，例如所有的节点都使用相同的 ssh 端口、用户名、密码，则共同的参数只在此处设置即可；
- 在节点标签页，如果该节点的角色包含 etcd 则必须填写 ETCD 成员名称 这个字段；
- 如果 KuboardSpray 所在节点不能直接访问到 Kubernetes 集群的节点，可以设置跳板机参数，使 KuboardSpray 可以通过 ssh 访问集群节点。
- 集群安装过程中，除了已经导入的资源包以外，还需要使用 yum 或 apt 指令安装一些系统软件，例如 curl, rsync, ipvadm, ipset, ethtool 等，此时要用到操作系统的 apt 软件源或者 yum 软件源。全局设置 标签页中，可以完成 apt / yum 软件源的设置，可以：
   - 使用节点操作系统已经事先配置的 apt / yum 源，或者
   - 在安装过程中自动配置节点的操作系统使用指定的软件源
- 如果使用 docker 作为集群的容器引擎，还需要在 全局设置 标签页指定安装 docker 用的 apt / yum 源。
- 如果使用 containerd 作为容器引擎，则无需配置 docker 的 apt / yum 源，containerd 的安装包已经包含在 KuboardSpray 离线资源包中。

点击上图的 保存 按钮，再点击 执行 按钮，可以启动集群的离线安装过程，如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641256202881-587a2762-30a5-4cc0-b854-643ad5ee3389.webp#clientId=u7d8dee2a-f082-4&from=paste&id=uda925867&originHeight=608&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8d9f2a84-bd9b-4bfb-bb2a-f93fcecad6a&title=)<br />取决于机器的性能和网络访问速度，大概喝一杯茶的功夫，集群就安装好了，安装成功时，日志界面的显示如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641256202808-c790c2d0-6450-4e10-8748-eefbbc04ddcd.webp#clientId=u7d8dee2a-f082-4&from=paste&id=u000ad45a&originHeight=608&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ucd0aea50-6171-4abc-9f78-df32d67e6e2&title=)
<a name="JIf6G"></a>
## 访问集群
如果集群日志界面提示集群已经安装成功，此时可以返回到集群规划页面，此界面将自动切换到 访问集群 标签页，如下图所示：<br />界面给出了三种方式可以访问 kubernetes 集群：

- 在集群主节点上执行 kubectl 命令
- 获取集群的 .kubeconfig 文件
- 将集群导入到 kuboard管理界面

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641256203057-2db8ac31-4949-4ded-b148-93a79ffe37a9.webp#clientId=u7d8dee2a-f082-4&from=paste&id=u87e8a915&originHeight=608&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue4668277-c08d-49c3-9b5f-7505bdba69f&title=)
<a name="WaBgU"></a>
## 参考资料
Kuboard-Spray: [https://github.com/eip-work/kuboard-spray](https://github.com/eip-work/kuboard-spray)
