KubernetesContainerd
<a name="N37Cf"></a>
## **一、概述**
作为接替 Docker 运行时的 Containerd 在早在 Kubernetes1.7 时就能直接与 Kubelet 集成使用，只是大部分时候因熟悉 Docker，在部署集群时采用了默认的 dockershim。在V1.24起的版本的 kubelet 就彻底移除了dockershim，改为默认使用Containerd了，当然也使用 cri-dockerd 适配器来将 Docker Engine 与 Kubernetes 集成。可以参考官方文档：<br />[https://kubernetes.io/zh-cn/docs/setup/production-environment/container-runtimes/#docker](https://kubernetes.io/zh-cn/docs/setup/production-environment/container-runtimes/#docker)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661751624081-0eef4bf5-49b6-4b38-9db7-ea083c297024.png#clientId=u6513bad7-ae01-4&from=paste&id=u92fd354e&originHeight=292&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u93add55d-d56f-41b7-89fe-ab6e0ec5004&title=)
<a name="XlBfY"></a>
## **二、Containerd 常见命令操作**
更换 Containerd 后，以往常用的 docker 命令也不再使用，取而代之的分别是 crictl 和 ctr 两个命令客户端。

- **crictl 是遵循 CRI 接口规范的一个命令行工具，通常用它来检查和管理kubelet节点上的容器运行时和镜像。**
- **ctr 是 containerd 的一个客户端工具。**
- **ctr -v 输出的是 containerd 的版本，crictl -v 输出的是当前 k8s 的版本，从结果显而易见可以认为 crictl 是用于 k8s 的。**
- **一般来说某个主机安装了 k8s 后，命令行才会有 crictl 命令。而 ctr 是跟 k8s 无关的，主机安装了 containerd 服务后就可以操作 ctr 命令。**

使用crictl命令之前，需要先配置/etc/crictl.yaml如下：
```yaml
runtime-endpoint: unix:///run/containerd/containerd.sock
image-endpoint: unix:///run/containerd/containerd.sock
timeout: 10
debug: false
```
也可以通过命令进行设置：
```
crictl config runtime-endpoint unix:///run/containerd/containerd.sock
crictl config image-endpoint unix:///run/containerd/containerd.sock
```
| **命令** | **docker** | **ctr（containerd）** | **crictl（kubernetes）** |
| --- | --- | --- | --- |
| 查看运行的容器 | docker ps | ctr task ls/ctr container ls | crictl ps |
| 查看镜像 | docker images | ctr image ls | crictl images |
| 查看容器日志 | docker logs | 无 | crictl logs |
| 查看容器数据信息 | docker inspect | ctr container info | crictl inspect |
| 查看容器资源 | docker stats | 无 | crictl stats |
| 启动/关闭已有的容器 | docker start/stop | ctr task start/kill | crictl start/stop |
| 运行一个新的容器 | docker run | ctr run | 无（最小单元为 pod） |
| 打标签 | docker tag | ctr image tag | 无 |
| 创建一个新的容器 | docker create | ctr container create | crictl create |
| 导入镜像 | docker load | ctr image import | 无 |
| 导出镜像 | docker save | ctr image export | 无 |
| 删除容器 | docker rm | ctr container rm | crictl rm |
| 删除镜像 | docker rmi | ctr image rm | crictl rmi |
| 拉取镜像 | docker pull | ctr image pull | ctictl pull |
| 推送镜像 | docker push | ctr image push | 无 |
| 登录或在容器内部执行命令 | docker exec | 无 | crictl exec |
| 清空不用的容器 | docker image prune | 无 | crictl rmi --prune |

更多命令操作，可以直接在命令行输入命令查看帮助。
```
docker --help
ctr --help
crictl --help
```
由于 Containerd 也有 namespaces 的概念，对于上层编排系统的支持，ctr 客户端 主要区分了 3 个命名空间分别是k8s.io、moby和default，以上用crictl操作的均在k8s.io命名空间，使用ctr 看镜像列表就需要加上-n 参数。crictl 是只有一个k8s.io命名空间，但是没有-n 参数。<br />【温馨提示】ctr images pull 拉取的镜像默认放在default，而 crictl pull 和 kubelet 默认拉取的镜像都在 k8s.io 命名空间下。所以通过ctr导入镜像的时候特别注意一点，最好指定命名空间。
```
# 注意-n不能放在命令最后面，下面几行查看的镜像是一样的
ctr -n=k8s.io image ls
ctr -n k8s.io image ls

# crictl 没有-n参数，操作都在`k8s.io`命名空间下。
crictl image ls
crictl images
# crictl image list = ctr -n=k8s.io image list
# crictl image ls = ctr -n=k8s.io image ls
# crictl images = ctr -n=k8s.io image list
# crictl images = ctr -n=k8s.io image ls

# 使用ctr命令指定命名空间导入镜像
ctr -n=k8s.io image import dashboard.tar

#查看镜像，可以看到可以查询到了
crictl images
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661751624176-9425a16b-ea07-4464-aa7c-c78e7255704c.png#clientId=u6513bad7-ae01-4&from=paste&id=u206d6619&originHeight=459&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uecb89f7d-afb2-4a6f-8980-763eba7b90a&title=)
<a name="Tv2iC"></a>
## **三、containerd 客户端工具 nerdctl**
推荐使用 nerdctl，使用效果与 docker 命令的语法一致，github 下载链接：<br />[https://github.com/containerd/nerdctl/releases](https://github.com/containerd/nerdctl/releases)

- **精简 (nerdctl--linux-amd64.tar.gz): 只包含 nerdctl**
- **完整 (nerdctl-full--linux-amd64.tar.gz): 包含 containerd, runc, and CNI 等依赖**

nerdctl 的目标并不是单纯地复制 docker 的功能，它还实现了很多 docker 不具备的功能，例如延迟拉取镜像（lazy-pulling）、镜像加密（imgcrypt）等。具体看 nerdctl。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661751624085-8e1d0cdd-1e21-4392-a2aa-fe5c95857029.png#clientId=u6513bad7-ae01-4&from=paste&id=ude9cc042&originHeight=375&originWidth=920&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uce55f402-870a-49f5-b7fb-c26a10d7056&title=)<br />延迟拉取镜像功能可以参考这篇文章：Containerd 使用 Stargz Snapshotter 延迟拉取镜像<br />[https://icloudnative.io/posts/startup-containers-in-lightning-speed-with-lazy-image-distribution-on-containerd/](https://icloudnative.io/posts/startup-containers-in-lightning-speed-with-lazy-image-distribution-on-containerd/)
<a name="RTNkw"></a>
### **1）安装 nerdctl（精简版）**
```bash
wget https://github.com/containerd/nerdctl/releases/download/v0.22.2/nerdctl-0.22.2-linux-amd64.tar.gz
# 解压
tar -xf nerdctl-0.22.2-linux-amd64.tar.gz

ln -s /opt/k8s/nerdctl/nerdctl /usr/local/bin/nerdctl
```
<a name="Y5m2Q"></a>
### **2）安装 nerdctl（完整版，这里不装）**
```bash
wget https://github.com/containerd/nerdctl/releases/download/v0.22.2/nerdctl-full-0.22.2-linux-amd64.tar.gz
tar -xf nerdctl-full-0.16.0-linux-amd64.tar.gz -C /usr/local/

cp /usr/local/lib/systemd/system/*.service /etc/systemd/system/
```
启动服务 buildkit
```bash
systemctl enable  buildkit containerd --now
systemctl status buildkit containerd
```
<a name="NNgMK"></a>
### **3）安装 buildkit 支持构建镜像**
buildkit GitHub 地址：[https://github.com/moby/buildkit](https://github.com/moby/buildkit)<br />使用**精简版 nerdctl 无法直接通过 containerd 构建镜像**，需要与 buildkit 组全使用以实现镜像构建。当然也可以安装上面的完整 nerdctl；buildkit 项目是 Docker 公司开源出来的一个构建工具包，支持 OCI 标准的镜像构建。它主要包含以下部分:

- **服务端 buildkitd，当前支持 runc 和 containerd 作为 worker，默认是 runc；**
- **客户端 buildctl，负责解析 Dockerfile，并向服务端 buildkitd 发出构建请求。**

buildkit 是典型的**C/S 架构**，client 和 server 可以不在一台服务器上。而 nerdctl 在构建镜像方面也可以作为 buildkitd 的客户端。
```bash
# https://github.com/moby/buildkit/releases
wget https://github.com/moby/buildkit/releases/download/v0.10.4/buildkit-v0.10.4.linux-amd64.tar.gz

tar -xf buildkit-v0.10.4.linux-amd64.tar.gz  -C /usr/local/
```
配置 buildkit 的启动文件，可以从这里下载：<br />https://github.com/moby/buildkit/tree/master/examples/systemd<br />buildkit 需要配置两个文件

- **/usr/lib/systemd/system/buildkit.socket**
```bash
cat > /usr/lib/systemd/system/buildkit.socket <<EOF
[Unit]
Description=BuildKit
Documentation=https://github.com/moby/buildkit
[Socket]
ListenStream=%t/buildkit/buildkitd.sock
SocketMode=0660
[Install]
WantedBy=sockets.target
EOF
```

- **/usr/lib/systemd/system/buildkit.service**
```bash
cat > /usr/lib/systemd/system/buildkit.service << EOF
[Unit]
Description=BuildKit
Requires=buildkit.socket
After=buildkit.socket
Documentation=https://github.com/moby/buildkit
[Service]
# Replace runc builds with containerd builds
ExecStart=/usr/local/bin/buildkitd --addr fd://
[Install]
WantedBy=multi-user.target
EOF
```
启动 buildkit
```bash
systemctl daemon-reload
systemctl enable buildkit --now
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661751624099-3822042d-db7d-4527-9331-37b40db13e71.png#clientId=u6513bad7-ae01-4&from=paste&id=ufc4ce84b&originHeight=549&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2a254a18-0a8b-40a3-8928-731acd7e490&title=)
<a name="W5Tgs"></a>
## **四、实战操作**
<a name="dlAoI"></a>
### **1）修改 containerd 配置文件**
```bash
containerd config default > /etc/containerd/config.toml
```
配置如下：
```
[plugins."io.containerd.grpc.v1.cri".registry]
      config_path = ""

      [plugins."io.containerd.grpc.v1.cri".registry.auths]

      [plugins."io.containerd.grpc.v1.cri".registry.configs]
        [plugins."io.containerd.grpc.v1.cri".registry.configs."myharbor-minio.com".tls]
          insecure_skip_verify = true  #跳过认证
          ca_file = "/etc/containerd/myharbor-minio.com/ca.crt"
        [plugins."io.containerd.grpc.v1.cri".registry.configs."myharbor-minio.com".auth]
          username = "admin"
          password = "Harbor12345"

      [plugins."io.containerd.grpc.v1.cri".registry.headers]

      [plugins."io.containerd.grpc.v1.cri".registry.mirrors]
        [plugins."io.containerd.grpc.v1.cri".registry.mirrors."myharbor-minio.com"]
          endpoint = ["https://myharbor-minio.com"]
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661751624839-19e94a8e-353e-44f8-b4ae-00208ecc07f2.png#clientId=u6513bad7-ae01-4&from=paste&id=u4fb7ae8c&originHeight=659&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0bfdc4b0-91ed-4e30-b388-b7222c7c020&title=)<br />重启 containerd
```bash
#重新加载配置
systemctl daemon-reload
#重启containerd
systemctl restart containerd
```
注意：这个配置文件是给crictl和kubelet使用，ctr是不可以用这个配置文件的，ctr 不使用 CRI，因此它不读取 plugins."io.containerd.grpc.v1.cri"配置。
<a name="ocoNY"></a>
### **2）ctr 拉取推送镜像**
```bash
# 推送镜像到harbor
ctr --namespace=k8s.io images push myharbor-minio.com/bigdata/minio:2022.8.22-debian-11-r0 --skip-verify --user admin:Harbor12345

# --namespace=k8s.io 指定命名空间，不是必须，根据环境而定
# --skip-verify 跳过认证
# --user 指定harbor用户名及密码

ctr  images pull --user admin:Harbor12345  --tlscacert=/etc/containerd/myharbor-minio.com/ca.crt myharbor-minio.com/bigdata/minio:2022.8.22-debian-11-r0
```
不想`-u user:password` 每次必须使用 ctr pull/ctr push， 可以使用`nerdctl `。
<a name="ge2u3"></a>
### **3）镜像构建**
```bash
cat > Dockerfile <<EOF
FROM nginx:alpine
RUN echo 'Hello Nerdctl From Containerd' > /usr/share/nginx/html/index.html
EOF
```
然后在文件所在目录执行镜像构建命令：
```bash
# 不加-n指定命名空间，crictl看不到，kubelet也不能使用它，默认在default命名空间下
nerdctl -n k8s.io build -t nginx:nerctl -f ./Dockerfile .
### 参数解释
# -t：指定镜像名称
# . ：当前目录Dockerfile
# -f：指定Dockerfile路径
#  --no-cache：不缓存
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661751624912-5563a529-f0f7-4eb6-ae4d-c43502a9fed6.png#clientId=u6513bad7-ae01-4&from=paste&id=u32d1ef33&originHeight=416&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u774c1cfa-567f-400c-ae5e-b147f3a29a2&title=)
<a name="l0P2Q"></a>
### **4）打标签 tag**
```bash
# crictl没有tag命令，只能使用nerdctl和ctr，必须指定命名空间，要不然kubelet无法使用。
ctr -n k8s.io i tag
nerdctl -n k8s.io tag nginx:nerctl myharbor-minio.com/bigdata/nginx:nerctl
# ctr -n k8s.io tag nginx:nerctl myharbor-minio.com/bigdata/nginx:nerctl
# 查看镜像
nerdctl  -n k8s.io  images myharbor-minio.com/bigdata/nginx:nerctl
```
<a name="d1r9F"></a>
### **5）将镜像推送到 Harbor**
第一种情况：http方式，配置如下：
```bash
# 以下两个哪个都可以
# mkdir -p /etc/docker/certs.d/myharbor-minio.com:443
mkdir -p /etc/containerd/certs.d/myharbor-minio.com:443

cat > /etc/containerd/certs.d/myharbor-minio.com\:443/hosts.toml <<EOF
server = "https://docker.io"

[host."http://myharbor-minio.com:80"]
  capabilities = ["pull", "resolve","push"]
  #skip_verify = true
  #ca = "ca.crt"   #相对路径
  #ca = "/opt/auth/ca.crt"  #绝对路径
  #ca = ["/opt/auth/ca.crt"]
  #ca = ["ca.crt"]
  #client = [["/opt/auth/nginx.cclinux.cn.crt", "/opt/auth/nginx.cclinux.cn.key"]]

EOF
```
第一种情况：https方式，配置如下：
```bash
# 以下两个哪个都可以
# mkdir -p /etc/docker/certs.d/myharbor-minio.com:443
mkdir -p /etc/containerd/certs.d/myharbor-minio.com:443

cat > /etc/containerd/certs.d/myharbor-minio.com\:443/hosts.toml <<EOF
server = "https://docker.io"

[host."https://myharbor-minio.com:443"]
  capabilities = ["pull", "resolve","push"]
  skip_verify = true
  #ca = "ca.crt"   #相对路径
  #ca = "/opt/auth/ca.crt"  #绝对路径
  #ca = ["/opt/auth/ca.crt"]
  ca = ["/etc/containerd/myharbor-minio.com/ca.crt"]
  #client = [["/opt/auth/nginx.cclinux.cn.crt", "/opt/auth/nginx.cclinux.cn.key"]]
EOF
```
通过 nerdctl 登录 harbor
```bash
echo Harbor12345 | nerdctl login --username "admin" --password-stdin  myharbor-minio.com:443

# nerdctl login --username "admin" --password Harbor12345 myharbor-minio.com:443

# 登出
# nerdctl logout
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661751625459-23de3021-35ba-44e4-ad55-f3d1f0b2005a.png#clientId=u6513bad7-ae01-4&from=paste&id=u319efeae&originHeight=339&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0bdb4a27-f843-4cc7-8bc6-6b0f35b16ca&title=)<br />开始将镜像推送到 harbor
```bash
### 推送到Harbor
# --insecure-registry        skips verifying HTTPS certs, and allows falling back to plain HTTP
nerdctl --insecure-registry --namespace=k8s.io push myharbor-minio.com/bigdata/nginx:nerctl
# ctr --namespace=k8s.io images push myharbor-minio.com/bigdata/nginx:nerctl --skip-verify --user admin:Harbor12345

# --namespace=k8s.io 指定命名空间，跟-n一样，不是必须，根据环境而定
# --skip-verify 跳过认证
# --user 指定harbor用户名及密码
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661751625937-ee5672c0-5abf-4266-9b9f-f536e7009148.png#clientId=u6513bad7-ae01-4&from=paste&id=ub6f61cc5&originHeight=154&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3b9da725-9981-4105-9533-fec07772163&title=)<br />Containerd ctr，crictl，nerdctl 客户端命令介绍与实战操作就到这里了。
