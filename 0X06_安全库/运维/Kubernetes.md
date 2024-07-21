<a name="cca227a4"></a>
## 概念知识

- apiserver: 所有的服务访问接口
- control-manager：维持副本的期待数目
- scheduler： 负责介绍任务，调度分配任务
- etcd：键值对数据库 存储K8s集群中重要的数据（持久化的数据）
- kubelet： 直接与容器引擎交互实现容器的周期管理
- kube-proxy： 负责写入规则到iptables ipvs 实现服务访问映射
- coredns:  可以为集群中的SVC创建一个域名的对应解析关系
- dashboard: 给k8s集群提供一个B/s的访问界面
- ingress controllers： 官方的只能实现四层的代理 ingress可以实现七层的代理
- fedetaction: 提供一个可以集群中心的多k8s集群统一管理的功能
- Prometheus：提供一个集群的监控能力
- ELK： 提供集群日志的统一分析接入平台

<a name="60nPB"></a>
## 0、安装需知


kubeadm是官方社区推出的一个用于快速部署kubernetes集群的工具。

这个工具能通过两条指令完成一个kubernetes集群的部署：

```bash
# 创建一个 Master 节点
$ kubeadm init

# 将一个 Node 节点加入到当前集群中
$ kubeadm join <Master节点的IP和端口 >
```

<a name="d9bc8687"></a>
## 1. 安装要求

在开始之前，部署Kubernetes集群机器需要满足以下几个条件：

- 一台或多台机器，操作系统 CentOS7.x-86_x64
- 硬件配置：2GB或更多RAM，2个CPU或更多CPU，硬盘30GB或更多
- 集群中所有机器之间网络互通
- 可以访问外网，需要拉取镜像
- 禁止swap分区

<a name="067b3d5b"></a>
## 2. 学习目标

1. 在所有节点上安装Docker和kubeadm
2. 部署Kubernetes Master
3. 部署容器网络插件
4. 部署 Kubernetes Node，将节点加入Kubernetes集群中
5. 部署Dashboard Web页面，可视化查看Kubernetes资源

<a name="a9421500"></a>
## 3. 准备环境
<a name="acOTa"></a>
### 组件功能方式1

![](https://cdn.nlark.com/yuque/0/2020/jpeg/2476579/1599998095671-75dcf3d9-2be0-4c42-890e-303c786b1788.jpeg#align=left&display=inline&height=580&originHeight=580&originWidth=763&size=0&status=done&style=none&width=763)
<a name="kq13X"></a>
### 组件工作模式2
![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1600346408025-b6b83cfd-022f-4eef-aa4b-a8a8b4351cd6.png#align=left&display=inline&height=425&originHeight=849&originWidth=1116&size=113231&status=done&style=none&width=558)
<a name="pSEEF"></a>
### 完整的生命过程
![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1600346462642-7548ea67-5744-46f7-b15d-0020d49b9150.png#align=left&display=inline&height=338&originHeight=676&originWidth=967&size=155405&status=done&style=none&width=483.5)<br />**解释说明：**

1. 用户准备一个资源文件（记录了业务应用的名称、镜像地址等信息），通过调用APIServer执行创建Pod
2. APIServer收到用户的Pod创建请求，将Pod信息写入到etcd中
3. 调度器通过list-watch的方式，发现有新的pod数据，但是这个pod还没有绑定到某一个节点中
4. 调度器通过调度算法，计算出最适合该pod运行的节点，并调用APIServer，把信息更新到etcd中
5. kubelet同样通过list-watch方式，发现有新的pod调度到本机的节点了，因此调用容器运行时，去根据pod的描述信息，拉取镜像，启动容器，同时生成事件信息
6. 同时，把容器的信息、事件及状态也通过APIServer写入到etcd中

| 角色 | IP |
| --- | --- |
| k8s-master | 192.168.40.10 |
| k8s-node1 | 192.168.40.11 |
| k8s-node2 | 192.168.40.12 |
| k8s.harbor.com | 192.168.40.13 |


```c
关闭防火墙：
$ systemctl stop firewalld
$ systemctl disable firewalld

关闭selinux：
$ sed -i 's/enforcing/disabled/' /etc/selinux/config  # 永久
$ setenforce 0  # 临时

关闭swap：
$ swapoff -a  # 临时
$ vim /etc/fstab  # 永久

设置主机名：
$ hostnamectl set-hostname <hostname>

在master添加hosts：
$ cat >> /etc/hosts << EOF
192.168.40.10 k8s-master
192.168.40.11 k8s-node1
192.168.40.12 k8s-node2
EOF

将桥接的IPv4流量传递到iptables的链：
$ cat > /etc/sysctl.d/k8s.conf << EOF
net.bridge.bridge-nf-call-ip6tables = 1
net.bridge.bridge-nf-call-iptables = 1
EOF
$ sysctl --system  # 生效

时间同步：
$ yum install ntpdate -y
$ ntpdate time.windows.com
```

<a name="e9c11f4d"></a>
## 4. 所有节点安装Docker/kubeadm/kubelet

Kubernetes默认CRI（容器运行时）为Docker，因此先安装Docker。

<a name="53057bed"></a>
### 4.1 安装Docker

```bash
$ wget https://mirrors.aliyun.com/docker-ce/linux/centos/docker-ce.repo -O /etc/yum.repos.d/docker-ce.repo
$ yum -y install docker-ce-18.06.1.ce-3.el7
$ systemctl enable docker && systemctl start docker
$ docker --version
Docker version 18.06.1-ce, build e68fc7a
```

```bash
# cat > /etc/docker/daemon.json << EOF
{
  "registry-mirrors": ["https://b9pmyelo.mirror.aliyuncs.com"]
}
EOF
```

<a name="5f610077"></a>
### 4.2 添加阿里云YUM软件源

```bash
$ cat > /etc/yum.repos.d/kubernetes.repo << EOF
[kubernetes]
name=Kubernetes
baseurl=https://mirrors.aliyun.com/kubernetes/yum/repos/kubernetes-el7-x86_64
enabled=1
gpgcheck=0
repo_gpgcheck=0
gpgkey=https://mirrors.aliyun.com/kubernetes/yum/doc/yum-key.gpg https://mirrors.aliyun.com/kubernetes/yum/doc/rpm-package-key.gpg
EOF
```

<a name="d581ebe2"></a>
### 4.3 安装kubeadm，kubelet和kubectl

由于版本更新频繁，这里指定版本号部署：

```bash
$ yum install -y kubelet-1.17.0 kubeadm-1.17.0 kubectl-1.17.0
$ systemctl enable kubelet
```

<a name="2ab38ac1"></a>
## 5. 部署Kubernetes Master

在192.168.40.10（Master）执行。

```bash
$ kubeadm init \
  --apiserver-advertise-address=192.168.40.10 \
  --image-repository registry.aliyuncs.com/google_containers \
  --kubernetes-version v1.17.0 \
  --service-cidr=10.96.0.0/12 \
  --pod-network-cidr=10.244.0.0/16
```

由于默认拉取镜像地址k8s.gcr.io国内无法访问，这里指定阿里云镜像仓库地址。

使用kubectl工具：

```bash
mkdir -p $HOME/.kube
sudo cp -i /etc/kubernetes/admin.conf $HOME/.kube/config
sudo chown $(id -u):$(id -g) $HOME/.kube/config
$ kubectl get nodes
```

<a name="72a0eda4"></a>
## 6. 安装Pod网络插件（CNI）

```bash
$ kubectl apply -f https://raw.githubusercontent.com/coreos/flannel/master/Documentation/kube-flannel.yml
```

确保能够访问到quay.io这个registery。

如果Pod镜像下载失败，可以改成这个镜像地址：lizhenliang/flannel:v0.11.0-amd64

注意：要是访问失败的话多半的可能是互联网长城，开启vpn

<a name="fe838581"></a>
## 7. 加入Kubernetes Node

way1打印出token sha256

```bash
kubeadm token create --print-join-command
```

way2

```
openssl x509 -in /etc/kubernetes/pki/ca.crt -noout -pubkey | openssl rsa -pubin -outform DER 2>/dev/null | sha256sum | cut -d' ' -f1
```

在192.168.40.11/12（Node）执行。

向集群添加新节点，执行在kubeadm init输出的kubeadm join命令：

```
$ kubeadm join 192.168.40.10:6443 --token g0z2fr.0y2eki4v750i4iy9  --discovery-token-ca-cert-hash sha256:7cb7b7dcef1d0811cf5259cfa094ff4a0a54a0be0378fe0fbfc6ea2c99c3daf2
```

<a name="03b77485"></a>
## 8. 测试kubernetes集群

在Kubernetes集群中创建一个pod，验证是否正常运行：

```yaml
$ kubectl create deployment nginx --image=nginx
$ kubectl expose deployment nginx --port=80 --type=NodePort
$ kubectl get pod,svc
```

访问地址：[http://NodeIP](http://NodeIP):Port

012
<a name="51bce07d"></a>
## 9.1. 部署 Dashboard
```c
[root@k8s-master ~]# kubectl apply -f https://kuboard.cn/install-script/kuboard.yaml
[root@k8s-master ~]# kubectl apply -f https://addons.kuboard.cn/metrics-server/0.3.7/metrics-server.yaml
[root@k8s-master ~]# echo $(kubectl -n kube-system get secret $(kubectl -n kube-system get secret | grep kuboard-user | awk '{print $1}') -o go-template='{{.data.token}}' | base64 -d)
[root@k8s-master ~]# kubectl get pods -l k8s.kuboard.cn/name=kuboard -n kube-system
NAME                       READY   STATUS    RESTARTS   AGE
kuboard-5bc5658d74-p5j26   1/1     Running   0          53d
[root@k8s-master ~]#
```
[https://kuboard.cn/install/install-dashboard-offline.html#%E5%87%86%E5%A4%87kuboard%E9%95%9C%E5%83%8F](https://kuboard.cn/install/install-dashboard-offline.html#%E5%87%86%E5%A4%87kuboard%E9%95%9C%E5%83%8F)
<a name="13096a6f"></a>
## 9.2harbor仓库搭建
```c
[root@harbor harbor]# systemctl status docker
● docker.service - Docker Application Container Engine
   Loaded: loaded (/usr/lib/systemd/system/docker.service; enabled; vendor preset: disabled)
   Active: active (running) since Fri 2020-09-18 15:51:48 EDT; 15min ago
     Docs: https://docs.docker.com
 Main PID: 9297 (dockerd)
    Tasks: 57
   Memory: 182.1M
   CGroup: /system.slice/docker.service
           ├─9297 /usr/bin/dockerd -H fd:// --containerd=/run/containerd/containerd.soc...
           ├─9682 /usr/bin/docker-proxy -proto tcp -host-ip 0.0.0.0 -host-port 443 -con...
           ├─9695 /usr/bin/docker-proxy -proto tcp -host-ip 0.0.0.0 -host-port 80 -cont...
           └─9711 /usr/bin/docker-proxy -proto tcp -host-ip 127.0.0.1 -host-port 1514 -...

Sep 18 15:51:41 harbor dockerd[9297]: time="2020-09-18T15:51:41.560229627-04:00" lev....."
Sep 18 15:51:41 harbor dockerd[9297]: time="2020-09-18T15:51:41.590250810-04:00" lev...ss"
Sep 18 15:51:47 harbor dockerd[9297]: time="2020-09-18T15:51:47.209833054-04:00" lev...e."
Sep 18 15:51:47 harbor dockerd[9297]: time="2020-09-18T15:51:47.351123463-04:00" lev....13
Sep 18 15:51:47 harbor dockerd[9297]: time="2020-09-18T15:51:47.351298879-04:00" lev...on"
Sep 18 15:51:48 harbor dockerd[9297]: time="2020-09-18T15:51:48.196996210-04:00" lev...ck"
Sep 18 15:51:48 harbor systemd[1]: Started Docker Application Container Engine.
Sep 18 15:51:57 harbor dockerd[9297]: time="2020-09-18T15:51:57.164037995-04:00" lev...te"
Sep 18 15:51:57 harbor dockerd[9297]: time="2020-09-18T15:51:57.656713201-04:00" lev...te"
Sep 18 15:51:58 harbor dockerd[9297]: time="2020-09-18T15:51:58.234188114-04:00" lev...te"
Hint: Some lines were ellipsized, use -l to show in full.
[root@harbor harbor]# docker-compose -v
docker-compose version 1.25.1, build a82fef07
[root@harbor local]# ll -h harbor-offline-installer-v2.1.0.tgz
-rw-r--r--. 1 root root 531M Sep 17 20:28 harbor-offline-installer-v2.1.0.tgz
[root@harbor local]#
[root@harbor local]# tar -xf harbor-offline-installer-v2.1.0.tgz
[root@harbor harbor]# ls
cert    common.sh  docker-compose.yml    harbor.yml  LICENSE
common  data       harbor.v2.1.0.tar.gz  install.sh  prepare
[root@harbor harbor]#
[root@harbor harbor]# egrep -v "#|^$" harbor.yml |head -20
hostname: k8s.harbor.com
http:
  port: 80
https:
    port: 443
    certificate: /usr/local/harbor/cert/server.crt
    private_key: /usr/local/harbor/cert/server.key
harbor_admin_password: Harbor-pass
database:
  password: root123
  max_idle_conns: 50
  max_open_conns: 1000
data_volume: /usr/src/harbor/data
[root@harbor harbor]# cd cert/
[root@harbor cert]# pwd
/usr/local/harbor/cert
[root@harbor cert]#
[root@harbor cert]# openssl genrsa -des3 -out server.key 2048
Generating RSA private key, 2048 bit long modulus
.......................+++
...............+++
e is 65537 (0x10001)
Enter pass phrase for server.key:
Verifying - Enter pass phrase for server.key:
[root@harbor cert]# openssl req -new -key server.key -out server.csr
Enter pass phrase for server.key:
You are about to be asked to enter information that will be incorporated
into your certificate request.
What you are about to enter is what is called a Distinguished Name or a DN.
There are quite a few fields but you can leave some blank
For some fields there will be a default value,
If you enter '.', the field will be left blank.
-----
Country Name (2 letter code) [XX]:CN
State or Province Name (full name) []:SC
Locality Name (eg, city) [Default City]:cd
Organization Name (eg, company) [Default Company Ltd]:k8s
Organizational Unit Name (eg, section) []:k8s
Common Name (eg, your name or your server's hostname) []:k8s.harbor.com
Email Address []:admin@qq.com

Please enter the following 'extra' attributes
to be sent with your certificate request
A challenge password []:
An optional company name []:
[root@harbor cert]#
[root@harbor cert]# cp server.key server.key.org
[root@harbor cert]# openssl rsa -in server.key.org -out server.key
Enter pass phrase for server.key.org:
writing RSA key
[root@harbor cert]# openssl x509 -req -days 365 -in server.csr -signkey server.key -out server.crt
Signature ok
subject=/C=CN/ST=SC/L=cd/O=k8s/OU=k8s/CN=k8s.harbor.com/emailAddress=admin@qq.com
Getting Private key
[root@harbor cert]# chmod a+x *
[root@harbor cert]#
[root@harbor cert]# cd /usr/local/harbor/
[root@harbor harbor]# ./install.sh
[root@harbor harbor]# docker ps -a
CONTAINER ID        IMAGE                                COMMAND                  CREATED             STATUS                    PORTS                                         NAMES
ad33d7d65260        goharbor/nginx-photon:v2.1.0         "nginx -g 'daemon of…"   19 hours ago        Up 32 minutes (healthy)   0.0.0.0:80->8080/tcp, 0.0.0.0:443->8443/tcp   nginx
12ff5bf719cf        goharbor/harbor-jobservice:v2.1.0    "/harbor/entrypoint.…"   19 hours ago        Up 32 minutes (healthy)                                                 harbor-jobservice
a2686a395c23        goharbor/harbor-core:v2.1.0          "/harbor/entrypoint.…"   19 hours ago        Up 32 minutes (healthy)                                                 harbor-core
466ec8ba64b8        goharbor/harbor-portal:v2.1.0        "nginx -g 'daemon of…"   19 hours ago        Up 32 minutes (healthy)                                                 harbor-portal
b0ce3c0cb2de        goharbor/harbor-registryctl:v2.1.0   "/home/harbor/start.…"   19 hours ago        Up 32 minutes (healthy)                                                 registryctl
50a2e495c592        goharbor/redis-photon:v2.1.0         "redis-server /etc/r…"   19 hours ago        Up 32 minutes (healthy)                                                 redis
654ef2c2d941        goharbor/registry-photon:v2.1.0      "/home/harbor/entryp…"   19 hours ago        Up 32 minutes (healthy)                                                 registry
b6e6b637ac6a        goharbor/harbor-db:v2.1.0            "/docker-entrypoint.…"   19 hours ago        Up 32 minutes (healthy)                                                 harbor-db
b2a19a41cb98        goharbor/harbor-log:v2.1.0           "/bin/sh -c /usr/loc…"   19 hours ago        Up 32 minutes (healthy)   127.0.0.1:1514->10514/tcp                     harbor-log
[root@harbor harbor]#

```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1600428192291-f42ba621-282b-40af-8830-1a0631ebe144.png#align=left&display=inline&height=354&originHeight=707&originWidth=1122&size=46141&status=done&style=none&width=561)<br />**用户名：admin**<br />**密码：Harbor-pass**<br />**<br />命令行登录报错
```c
[root@k8s-node2 ~]# docker tag busybox:latest https://k8s.harbor.com/library/busybox:v1
Error parsing reference: "https://k8s.harbor.com/library/busybox:v1" is not a valid repository/tag: invalid reference format
```
解决方案
```bash
[root@k8s-node1 ~]# cat /etc/docker/daemon.json
{
         "insecure-registries": ["k8s.harbor.com"]
}
[root@k8s-node1 ~]#
[root@k8s-node1 ~]# systemctl reload docker
[root@k8s-node1 ~]# docker login -u admin -p Harbor-pass https://k8s.harbor.com
WARNING! Using --password via the CLI is insecure. Use --password-stdin.
WARNING! Your password will be stored unencrypted in /root/.docker/config.json.
Configure a credential helper to remove this warning. See
https://docs.docker.com/engine/reference/commandline/login/#credentials-store

Login Succeeded

```
**![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1600486983991-fdccbc03-c277-4566-9d06-3277c8fd476e.png#align=left&display=inline&height=362&originHeight=724&originWidth=551&size=54974&status=done&style=none&width=275.5)**
```c
[root@k8s-node1 ~]# docker tag nginx:latest k8s.harbor.com/library/nginx:v1
[root@k8s-node1 ~]# docker push k8s.harbor.com/library/nginx:v1
The push refers to repository [k8s.harbor.com/library/nginx]
98b4c818e603: Pushed
1698c1b7e3e6: Pushed
227442bb48dc: Pushed
d899691659b0: Pushing [=====>                                             ]  6.503MB/63.25MB
95ef25a32043: Pushing [=============>                                     ]  18.45MB/69.19MB

d899691659b0: Pushed
95ef25a32043: Pushed
v1: digest: sha256:deb724a427ea79face617392a5a471fdcb4cdb57f971ee6b7e492b90fecb199f size: 1362
[root@k8s-node1 ~]#

```
**![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1600487429833-c29b5f78-cd54-4525-b462-a6297aead04c.png#align=left&display=inline&height=201&originHeight=402&originWidth=1568&size=41141&status=done&style=none&width=784)**<br />**harbor端配置**
```c
[root@harbor harbor]# cat /etc/docker/daemon.json
{
  "registry-mirrors": ["https://b9pmyelo.mirror.aliyuncs.com"],
  "insecure-registries": ["k8s.harbor.com"]
}
[root@harbor harbor]# systemctl restart docker
[root@harbor harbor]# docker login -u admin -p Harbor-pass https://k8s.harbor.com
WARNING! Using --password via the CLI is insecure. Use --password-stdin.
WARNING! Your password will be stored unencrypted in /root/.docker/config.json.
Configure a credential helper to remove this warning. See
https://docs.docker.com/engine/reference/commandline/login/#credentials-store

Login Succeeded
[root@harbor harbor]# docker tag busybox:latest k8s.harbor.com/library/busybox:v1
[root@harbor harbor]# docker push k8s.harbor.com/library/busybox:v1
The push refers to repository [k8s.harbor.com/library/busybox]
be8b8b42328a: Pushed
v1: digest: sha256:2ca5e69e244d2da7368f7088ea3ad0653c3ce7aaccd0b8823d11b0d5de956002 size: 527
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1600488119845-e54c7014-b0f5-410a-ab0c-60c1614cc31b.png#align=left&display=inline&height=208&originHeight=415&originWidth=1593&size=45872&status=done&style=none&width=796.5)<br />下载测试
```bash
[root@k8s-node1 ~]# docker pull k8s.harbor.com/library/busybox:v1
v1: Pulling from library/busybox
df8698476c65: Pull complete
Digest: sha256:2ca5e69e244d2da7368f7088ea3ad0653c3ce7aaccd0b8823d11b0d5de956002
Status: Downloaded newer image for k8s.harbor.com/library/busybox:v1
[root@k8s-node1 ~]#

```
<a name="pcDaM"></a>
## 10基础命令

<a name="d017189c"></a>
#### 运行一个deployment类型的pod控制器,有一个副本数

```bash
[root@k8s-master ~]# kubectl run nginx-dep1 --image=nginx:1.8 --replicas=1
```

<a name="f5423e7d"></a>
#### 查看一个deployment的pod控制器

```bash
[root@k8s-master ~]# kubectl get deployment
NAME         READY   UP-TO-DATE   AVAILABLE   AGE
nginx        1/1     1            1           2d6h
nginx-dep1   1/1     1            1           51s
```

<a name="5ca1cc42"></a>
#### 查看pod

```bash
[root@k8s-master ~]# kubectl get pod
NAME                          READY   STATUS    RESTARTS   AGE
nginx-86c57db685-qfvkz        1/1     Running   0          2d6h
nginx-dep1-6dd5d75f8b-hzp6k   1/1     Running   0          3m20s
```

<a name="Hzj2A"></a>
## 11Kubernetes帮助信息

```bash
[root@k8s-master ~]# kubectl explain --help
```

可以简单的查看Kubernetes支持的字段对象

格式如下

```bash
[root@k8s-master ~]# kubectl explain pods
KIND:     Pod
VERSION:  v1

DESCRIPTION:
     Pod is a collection of containers that can run on a host. This resource is
     created by clients and scheduled onto hosts.
[root@k8s-master ~]# kubectl explain pods.metadata
KIND:     Pod
VERSION:  v1

RESOURCE: metadata <Object>

DESCRIPTION:
     Standard object's metadata. More info:
     https://git.k8s.io/community/contributors/devel/sig-architecture/api-conventions.md#metadata

     ObjectMeta is metadata that all persisted resources must have, which
     includes all objects users must create.

FIELDS:
   annotations  <map[string]string>
     Annotations is an unstructured key value map stored with a resource that
     may be set by external tools to store and retrieve arbitrary metadata. They
     are not queryable and should be preserved when modifying objects. More
     info: http://kubernetes.io/docs/user-guide/annotations
```

查看pod字段相关信息

```bash
[root@k8s-master ~]#  kubectl explain pods --recursive
    fc     <Object>
            fsType      <string>
            lun <integer>
            readOnly    <boolean>
            targetWWNs  <[]string>
            wwids       <[]string>
         flexVolume     <Object>
            driver      <string>
            fsType      <string>
            options     <map[string]string>
            readOnly    <boolean>
            secretRef   <Object>
               name     <string>
         flocker        <Object>
            datasetName <string>
            datasetUUID <string>
         gcePersistentDisk      <Object>
            fsType      <string>
            partition   <integer>
            pdName      <string>
            readOnly    <boolean>
```

<a name="c7df99a3"></a>
#### yaml创建deployment指定副本数

```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: ngx-dep2
  labels:
    app: ngx
    type: webserver
spec:
  replicas: 1
  selector:
    matchLabels:
      app: ngx
  template:
    metadata:
      labels:
        app: ngx
    spec:
      containers:
      - name: nginx
        image: nginx:1.8
        imagePullPolicy: IfNotPresent
```

<a name="d23161cd"></a>
#### 运行yaml文件

```bash
[root@k8s-master ~]# kubectl apply -f nginx.yaml
deployment.apps/ngx-dep2 created
```

<a name="f5423e7d-1"></a>
#### 查看一个deployment的pod控制器

```bash
[root@k8s-master ~]# kubectl get deployment
NAME         READY   UP-TO-DATE   AVAILABLE   AGE
ngx-dep2     1/1     1            1           2m35s
```

<a name="5ca1cc42-1"></a>
#### 查看pod

```bash
[root@k8s-master ~]# kubectl get pods
NAME                          READY   STATUS    RESTARTS   AGE
ngx-dep2-64cfcc9ddc-87rmz     1/1     Running   0          3m38s
[root@k8s-master ~]#
```

<a name="9b5d0d42"></a>
#### 查看k8s对象状态

```bash
[root@k8s-master ~]#  kubectl get --help
[root@k8s-master ~]#  kubectl get pods -o wide
```

<a name="c5e49cf8"></a>
#### 查看名称空间-------默认查看的资源类型是default下的名称空间

```bash
[root@k8s-master ~]# kubectl get namespace
NAME              STATUS   AGE
default           Active   2d7h
kube-node-lease   Active   2d7h
kube-public       Active   2d7h
kube-system       Active   2d7h
[root@k8s-master ~]#
```

<a name="2a5b773c"></a>
#### 查看执行名称空间的pod --------------------- -n指定名称空间

```bash
[root@k8s-master ~]# kubectl get pod -n kube-system			#### -n 指定名称空间
NAME                                 READY   STATUS    RESTARTS   AGE
coredns-9d85f5447-b9tkc              1/1     Running   0          2d7h
coredns-9d85f5447-kzrlk              1/1     Running   0          2d7h
etcd-k8s-master                      1/1     Running   0          2d7h
kube-apiserver-k8s-master            1/1     Running   0          2d7h
kube-controller-manager-k8s-master   1/1     Running   0          2d7h
kube-flannel-ds-amd64-2xkzt          1/1     Running   0          2d7h
```

<a name="54cd6a01"></a>
#### 查看所有名称空间的pod

```bash
[root@k8s-master ~]# kubectl get pod -A
NAMESPACE     NAME                                 READY   STATUS    RESTARTS   AGE
default       nginx-86c57db685-qfvkz               1/1     Running   0          2d7h
default       nginx-dep1-6dd5d75f8b-hzp6k          1/1     Running   0          45m
default       ngx-dep2-64cfcc9ddc-87rmz            1/1     Running   0          21m
kube-system   coredns-9d85f5447-b9tkc              1/1     Running   0          2d7h
kube-system   coredns-9d85f5447-kzrlk              1/1     Running   0          2d7h
kube-system   etcd-k8s-master                      1/1     Running   0          2d7h
kube-system   kube-apiserver-k8s-master            1/1     Running   0          2d7h
kube-system   kube-controller-manager-k8s-master   1/1     Running   0          2d7h
```

<a name="30347ee4"></a>
#### 显示详细信息 -o wide

```bash
[root@k8s-master ~]# kubectl get pod -A -o wide
NAMESPACE     NAME                                 READY   STATUS    RESTARTS   AGE    IP              NODE         NOMINATED NODE   READINESS GATES
default       nginx-86c57db685-qfvkz               1/1     Running   0          2d7h   10.244.1.2      k8s-node1    <none>           <none>
default       nginx-dep1-6dd5d75f8b-hzp6k          1/1     Running   0          46m    10.244.1.6      k8s-node1    <none>           <none>
default       ngx-dep2-64cfcc9ddc-87rmz            1/1     Running   0          23m    10.244.1.7      k8s-node1    <none>           <none>
kube-system   coredns-9d85f5447-b9tkc              1/1     Running   0          2d7h   10.244.2.3      k8s-node2    <none>           <none>
kube-system   coredns-9d85f5447-kzrlk              1/1     Running   0          2d7h   10.244.2.2      k8s-node2    <none>           <none>
```

<a name="60804195"></a>
#### describe查看指定的资源类型的详细信息

```bash
[root@k8s-master ~]# kubectl describe pod nginx-dep1-6dd5d75f8b-hzp6k
Events:
  Type    Reason     Age   From                Message
  ----    ------     ----  ----                -------
  Normal  Scheduled  51m   default-scheduler   Successfully assigned default/nginx-dep1-6dd5d75f8b-hzp6k to k8s-node1
  Normal  Pulled     51m   kubelet, k8s-node1  Container image "nginx:1.8" already present on machine
  Normal  Created    51m   kubelet, k8s-node1  Created container nginx-dep1
  Normal  Started    51m   kubelet, k8s-node1  Started container nginx-dep1
```

<a name="0aae5dbf"></a>
#### 查看所有的资源类型

```bash
[root@k8s-master ~]# kubectl api-resources
NAME                              SHORTNAMES   APIGROUP                       NAMESPACED   KIND
bindings                                                                      true         Binding
componentstatuses                 cs                                          false        ComponentStatus
configmaps                        cm                                          true         ConfigMap
endpoints                         ep                                          true         Endpoints
events                            ev                                          true         Event
limitranges                       limits                                      true         LimitRange
namespaces                        ns                                          false        Namespace
nodes                             no                                          false        Node
```

<a name="d50e37b8"></a>
#### 查看指定的名称空间

```bash
[root@k8s-master ~]# kubectl describe pod  kube-proxy-85cl6 -n kube-system
Name:                 kube-proxy-85cl6
Namespace:            kube-system
Priority:             2000001000
Priority Class Name:  system-node-critical
Node:                 k8s-node2/192.168.40.12
Start Time:           Mon, 20 Jul 2020 22:50:07 -0400
Labels:               controller-revision-hash=bdc8bbd47
                      k8s-app=kube-proxy
                      pod-template-generation=1
Annotations:          <none>
Status:               Running
IP:                   192.168.40.12
IPs:
  IP:           192.168.40.12
```

<a name="edbb1673"></a>
#### 查看node相关的信息

```bash
[root@k8s-master ~]# kubectl describe node k8s-node1
Name:               k8s-node1
Roles:              <none>
Labels:             beta.kubernetes.io/arch=amd64
                    beta.kubernetes.io/os=linux
                    kubernetes.io/arch=amd64
                    kubernetes.io/hostname=k8s-node1
                    kubernetes.io/os=linux
Annotations:        flannel.alpha.coreos.com/backend-data: {"VtepMAC":"ae:61:0a:4b:51:53"}
                    flannel.alpha.coreos.com/backend-type: vxlan
                    flannel.alpha.coreos.com/kube-subnet-manager: true
                    flannel.alpha.coreos.com/public-ip: 192.168.40.11
                    kubeadm.alpha.kubernetes.io/cri-socket: /var/run/dockershim.sock
                    node.alpha.kubernetes.io/ttl: 0
                    volumes.kubernetes.io/controller-managed-attach-detach: true
CreationTimestamp:  Mon, 20 Jul 2020 22:48:47 -0400
```

<a name="c7ccbdb9"></a>
#### 查看node基础信息

```bash
[root@k8s-master ~]# kubectl get node -o wide
NAME         STATUS   ROLES    AGE    VERSION   INTERNAL-IP     EXTERNAL-IP   OS-IMAGE                KERNEL-VERSION                CONTAINER-RUNTIME
k8s-master   Ready    master   2d7h   v1.17.0   192.168.40.10   <none>        CentOS Linux 7 (Core)   3.10.0-1127.13.1.el7.x86_64   docker://18.6.1
k8s-node1    Ready    <none>   2d7h   v1.17.0   192.168.40.11   <none>        CentOS Linux 7 (Core)   3.10.0-1127.13.1.el7.x86_64   docker://18.6.1
k8s-node2    Ready    <none>   2d7h   v1.17.0   192.168.40.12   <none>        CentOS Linux 7 (Core)   3.10.0-327.el7.x86_64         docker://18.6.1
[root@k8s-master ~]#
```

<a name="828ce375"></a>
#### 查看资源的日志信息

查看pod中一个容器在运行过程中产生的日志信息

```bash
[root@k8s-master ~]# kubectl logs nginx-86c57db685-qfvkz
/docker-entrypoint.sh: /docker-entrypoint.d/ is not empty, will attempt to perform configuration
/docker-entrypoint.sh: Looking for shell scripts in /docker-entrypoint.d/
/docker-entrypoint.sh: Launching /docker-entrypoint.d/10-listen-on-ipv6-by-default.sh
10-listen-on-ipv6-by-default.sh: Getting the checksum of /etc/nginx/conf.d/default.conf
10-listen-on-ipv6-by-default.sh: Enabled listen on IPv6 in /etc/nginx/conf.d/default.conf
/docker-entrypoint.sh: Launching /docker-entrypoint.d/20-envsubst-on-templates.sh
/docker-entrypoint.sh: Configuration complete; ready for start up
10.244.0.0 - - [21/Jul/2020:02:56:14 +0000] "GET / HTTP/1.1" 200 612 "-" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/84.0.4147.89 Safari/537.36 Edg/84.0.522.40" "-"
```

<a name="af70fd62"></a>
#### 以交互的方式进入容器

```bash
[root@k8s-master ~]# kubectl get pods
NAME                          READY   STATUS    RESTARTS   AGE
nginx-86c57db685-qfvkz        1/1     Running   0          2d7h
[root@k8s-master ~]# kubectl exec -it nginx-86c57db685-qfvkz /bin/bash
root@nginx-86c57db685-qfvkz:/#
```

<a name="36013a1f"></a>
## 12发布服务

<a name="2e3d1c2a"></a>
#### 用service来实现的

```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: ngx-dep2
  labels:
    app: ngx
    type: webserver
spec:
  replicas: 1
  selector:
    matchLabels:
      app: ngx
  template:
    metadata:
      labels:
        app: ngx
    spec:
      containers:
      - name: nginx
        image: nginx:1.8
```

```yaml
apiVersion: v1
kind: Service
metadata:
  name: ngx-svc
  labels:
    app:  
spec:
  selector:
    app: ngx
  ports:
  - name: nginx-ports
    protocol: TCP
    port: 80
    nodePort: 32002
    targetPort: 80
  type: NodePort
```

<a name="d23161cd-1"></a>
#### 运行yaml文件

```bash
[root@k8s-master ~]# kubectl apply -f nginx-ser.yaml
service/ngx-svc created
[root@k8s-master ~]# kubectl get svc
NAME         TYPE        CLUSTER-IP      EXTERNAL-IP   PORT(S)        AGE
kubernetes   ClusterIP   10.96.0.1       <none>        443/TCP        2d7h
nginx        NodePort    10.96.51.110    <none>        80:31018/TCP   2d7h
ngx-svc      NodePort    10.96.137.115   <none>        80:32002/TCP   10s
```

<a name="81011c74"></a>
#### 访问测试在任意的一个节点访问都可以

![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599997832715-099e5a46-fe0b-4b3e-a9f2-4b952f984562.png#align=left&display=inline&height=311&originHeight=622&originWidth=1046&size=173236&status=done&style=none&width=523)
<a name="4fa14e01"></a>
## 13web页面

```bash
[root@k8s-master ~]#  docker pull eipwork/kuboard:latest
[root@k8s-master ~]#  docker images | grep kuboard
eipwork/kuboard                                                   latest              9fccf2a94412        3 weeks ago         182MB
[root@k8s-master ~]#
docker save 9fccf2a94412 > kuboard.tar
```

<a name="f64d3342"></a>
#### 将tar包传到k8s-node1节点上面

```bash
docker load < kuboard.tar
docker tag 0146965e6475 eipwork/kuboard:latest
```

<a name="9cbadf18"></a>
#### 在主节点写入如下的yaml文件---------------------------------------注意在第二十六行写你要在那个节点上面创建kuborad

```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: kuboard
  namespace: kube-system
  annotations:
    k8s.kuboard.cn/displayName: kuboard
    k8s.kuboard.cn/ingress: "true"
    k8s.kuboard.cn/service: NodePort
    k8s.kuboard.cn/workload: kuboard
  labels:
    k8s.kuboard.cn/layer: monitor
    k8s.kuboard.cn/name: kuboard
spec:
  replicas: 1
  selector:
    matchLabels:
      k8s.kuboard.cn/layer: monitor
      k8s.kuboard.cn/name: kuboard
  template:
    metadata:
      labels:
        k8s.kuboard.cn/layer: monitor
        k8s.kuboard.cn/name: kuboard
    spec:
      nodeName: your-node-name
      containers:
      - name: kuboard
        image: eipwork/kuboard:latest
        imagePullPolicy: IfNotPresent
      tolerations:
      - key: node-role.kubernetes.io/master
        effect: NoSchedule

---
apiVersion: v1
kind: Service
metadata:
  name: kuboard
  namespace: kube-system
spec:
  type: NodePort
  ports:
  - name: http
    port: 80
    targetPort: 80
    nodePort: 32567
  selector:
    k8s.kuboard.cn/layer: monitor
    k8s.kuboard.cn/name: kuboard

---
apiVersion: v1
kind: ServiceAccount
metadata:
  name: kuboard-user
  namespace: kube-system

---
apiVersion: rbac.authorization.k8s.io/v1
kind: ClusterRoleBinding
metadata:
  name: kuboard-user
roleRef:
  apiGroup: rbac.authorization.k8s.io
  kind: ClusterRole
  name: cluster-admin
subjects:
- kind: ServiceAccount
  name: kuboard-user
  namespace: kube-system

---
apiVersion: v1
kind: ServiceAccount
metadata:
  name: kuboard-viewer
  namespace: kube-system

---
apiVersion: rbac.authorization.k8s.io/v1
kind: ClusterRoleBinding
metadata:
  name: kuboard-viewer
roleRef:
  apiGroup: rbac.authorization.k8s.io
  kind: ClusterRole
  name: view
subjects:
- kind: ServiceAccount
  name: kuboard-viewer
  namespace: kube-system
```

<a name="d23161cd-2"></a>
#### 运行yaml文件

```bash
kubectl apply -f kuboard.yaml
[root@k8s-master ~]# kubectl get pod -n kube-system
NAME                                 READY   STATUS    RESTARTS   AGE
kuboard-5bc5658d74-fskmw             1/1     Running   0          21m
[root@k8s-master ~]# kubectl get svc -n kube-system
NAME       TYPE        CLUSTER-IP      EXTERNAL-IP   PORT(S)                  AGE
kube-dns   ClusterIP   10.96.0.10      <none>        53/UDP,53/TCP,9153/TCP   2d11h
kuboard    NodePort    10.96.148.162   <none>        80:32567/TCP             22m
[root@k8s-master ~]#
```

<a name="ed711b12"></a>
#### 浏览器访问验证

![](Kubernetes%E5%9F%BA%E7%A1%80%E5%91%BD%E4%BB%A4.assets/1595514225405.png#alt=1595514225405)

<a name="d7d0865b"></a>
#### 获取token

```bash
[root@k8s-master ~]# echo $(kubectl -n kube-system get secret $(kubectl -n kube-system get secret | grep kuboard-user | awk '{print $1}') -o go-template='{{.data.token}}' | base64 -d)
```

![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599997864365-cb24c73c-b167-49ce-8f47-cb0e61c1c020.png#align=left&display=inline&height=163&originHeight=326&originWidth=992&size=97180&status=done&style=none&width=496)

<a name="8782ea7e"></a>
### 以默认的deployment创建一个pod

```bash
[root@k8s-master ~]# kubectl run nginx-dep1 --image=nginx:1.8 --replicas=1
[root@k8s-master ~]# kubectl get deployments.apps
NAME         READY   UP-TO-DATE   AVAILABLE   AGE
nginx        1/1     1            1           2d23h
nginx-dep1   1/1     1            1           11s
[root@k8s-master ~]# kubectl get deployments.apps -o wide
NAME         READY   UP-TO-DATE   AVAILABLE   AGE     CONTAINERS   IMAGES      SELECTOR
nginx        1/1     1            1           2d23h   nginx        nginx       app=nginx
nginx-dep1   1/1     1            1           19s     nginx-dep1   nginx:1.8   run=nginx-dep1
```

<a name="42f9222f"></a>
## 14、自动补全

<a name="da19b30b"></a>
#### 部署方法

```bash
echo "source <(kubectl completion bash)">> /root/.bashrc
source /root/.bashrc
```

<a name="202f4469"></a>
#### 正常这样就已经可以了，但是我的环境报错

```bash
kubectl get-bash: _get_comp_words_by_ref: command not found
```

<a name="fec8e945"></a>
#### 需要安装bash-completion组件

```bash
yum install bash-completion -y
```

<a name="7d976419"></a>
#### 重新执行部署方法

```bash
echo "source <(kubectl completion bash)">> /root/.bashrc
source /root/.bashrc
```

现在就可以自动补全了

<a name="bdd9e186"></a>
## 15、集群service的访问方式

![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599997883177-0d122188-c4bc-400d-8777-f0907c345b38.png#align=left&display=inline&height=182&originHeight=364&originWidth=936&size=58888&status=done&style=none&width=468)

<a name="672f616c"></a>
#### 创建一个deployment

```yaml
[root@k8s-master ~]# vim cluser.yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: myapp-deploy
  namespace: default
spec:
  replicas: 3
  selector:
    matchLabels:
      app: myapp
      release: stabel
  template:
    metadata:
      labels:
        app: myapp
        release: stabel
        env: test
    spec:
      containers:
      - name: myapp
        image: nginx
        imagePullPolicy: IfNotPresent
        ports:
        - name: http
          containerPort: 80
```

<a name="f1a73613"></a>
#### 在线修改副本数量

```bash
[root@k8s-master ~]# kubectl scale deployment nginx-rc --replicas=4
deployment.apps/nginx-rc scaled
[root@k8s-master ~]# kubectl get pod -o wide
NAME                         READY   STATUS    RESTARTS   AGE     IP            NODE        NOMINATED NODE   READINESS GATES
nginx-rc-65f5b8cb6d-7w2vc    1/1     Running   0          105s    10.244.1.18   k8s-node1   <none>           <none>
nginx-rc-65f5b8cb6d-dxhxz    1/1     Running   0          5s      10.244.2.9    k8s-node2   <none>           <none>
nginx-rc-65f5b8cb6d-hqlnv    1/1     Running   0          6m45s   10.244.2.8    k8s-node2   <none>           <none>
nginx-rc-65f5b8cb6d-qx45d    1/1     Running   0          6m45s   10.244.1.16   k8s-node1   <none>           <none>
web-nginx-5d4b4d8598-rdpkq   1/1     Running   0          31m     10.244.1.13   k8s-node1   <none>           <none>
[root@k8s-master ~]#
```

<a name="af058fa7"></a>
#### 创建一个clusterIP的访问方式

```bash
[root@k8s-master ~]# kubectl apply -f cluser.yaml
deployment.apps/myapp-deploy created
[root@k8s-master ~]#
[root@k8s-master ~]# kubectl get deployments.apps
NAME           READY   UP-TO-DATE   AVAILABLE   AGE
myapp-deploy   3/3     3            3           8s
nginx          1/1     1            1           4d1h
[root@k8s-master ~]# kubectl get pod
NAME                           READY   STATUS    RESTARTS   AGE
myapp-deploy-5954fc89f-7ndb2   1/1     Running   0          21s
myapp-deploy-5954fc89f-p5j26   1/1     Running   0          21s
myapp-deploy-5954fc89f-pgkfr   1/1     Running   0          21s
nginx-86c57db685-qfvkz         1/1     Running   0          4d1h
```

<a name="dda42293"></a>
#### 创建headless的service

```yaml
[root@k8s-master ~]# vim headless.yaml
apiVersion: v1
kind: Service
metadata:
  name: myapp-headless
  namespace: default
spec:
  selector:
    app: myapp
  clusterIP: "None"
  ports:
  - port: 80
    targetPort: 80
[root@k8s-master ~]# kubectl apply -f headless.yaml
service/myapp-headless created
```

<a name="607e7a4f"></a>
#### 查看

```bash
[root@k8s-master ~]# kubectl get svc
NAME             TYPE        CLUSTER-IP     EXTERNAL-IP   PORT(S)        AGE
kubernetes       ClusterIP   10.96.0.1      <none>        443/TCP        4d2h
myapp            ClusterIP   10.96.143.40   <none>        80/TCP         57m
myapp-headless   ClusterIP   None           <none>        80/TCP         117s
nginx            NodePort    10.96.51.110   <none>        80:31018/TCP   4d2h
[root@k8s-master ~]#
```

<a name="99b935d7"></a>
## 16、数据卷

<a name="b1651b6d"></a>
#### 使用的是hostPath方式实现的数据卷的挂载

注意现在所有的节点都创建目录mkdir /data

```yaml
[root@k8s-master ~]# vim volumn.yaml
apiVersion: v1
kind: Pod
metadata:
  name: test-pd
spec:
  containers:
  - image: nginx:1.8
    name: test-container
    volumeMounts:
    - mountPath: /test-pd
      name: test-volume
  volumes:
  - name: test-volume
    hostPath:
      path: /data
      type: Directory
```

<a name="bae6dcbc"></a>
#### 应用当前的文件

```bash
[root@k8s-master ~]# kubectl create -f volumn.yaml
pod/test-pd created
[root@k8s-master ~]# kubectl get pod -o wide
NAME                     READY   STATUS    RESTARTS   AGE    IP            NODE        NOMINATED NODE   READINESS GATES
nginx-86c57db685-qfvkz   1/1     Running   0          5d3h   10.244.1.2    k8s-node1   <none>           <none>
test-pd                  1/1     Running   0          56s    10.244.1.17   k8s-node1   <none>           <none>
[root@k8s-master ~]# kubectl exec -it test-pd /bin/bash
root@test-pd:/# cd /test-pd/
root@test-pd:/test-pd# ls
root@test-pd:/test-pd# date > index.html
root@test-pd:/test-pd# cat index.html
Sun Jul 26 06:20:16 UTC 2020
```

<a name="7ac11a22"></a>
#### 在创建pod节点上面查看并验证

```bash
[root@k8s-node1 ~]# cat /data/index.html
Sun Jul 26 06:20:16 UTC 2020
[root@k8s-node1 ~]#
```

<a name="d910d865"></a>
## 17、持久化存储

<a name="4100ce91"></a>
#### 部署nfs

```bash
[root@k8s-master ~]# cat /etc/exports
/nfs *(rw,no_root_squash,no_all_squash,sync)
[root@k8s-master ~]# showmount -e 192.168.40.10
Export list for 192.168.40.10:
/nfs *
```

<a name="0a4e974b"></a>
#### 部署pv

```yaml
[root@k8s-master ~]# vim pv.yaml
apiVersion: v1
kind: PersistentVolume
metadata:
  name: nfspv1
spec:
  capacity:
    storage: 1Gi
  accessModes:
  - ReadWriteOnce
  persistentVolumeReclaimPolicy: Retain
  storageClassName: nfs
  nfs:
    path: /nfs
    server: 192.168.40.10
```

<a name="5a19226c"></a>
#### 应用pv

```bash
[root@k8s-master ~]# kubectl create -f pv.yaml
persistentvolume/nfspv1 created
[root@k8s-master ~]# kubectl get pv
NAME     CAPACITY   ACCESS MODES   RECLAIM POLICY   STATUS      CLAIM   STORAGECLASS   REASON   AGE
nfspv1   1Gi        RWO            Retain           Available           nfs                     6s
[root@k8s-master ~]#
```

<a name="be25573e"></a>
#### 创建PVC

```yaml
[root@k8s-master ~]# vim pvc.yaml
apiVersion: v1
kind: Service
metadata:
  name: nginx
  labels:
    app: nginx
spec:
  ports:
  - port: 80
    name: web
  clusterIP: None
  selector :
    app: nginx
---
apiVersion: apps/v1
kind: StatefulSet
metadata:
  name: web
spec:
  selector :
    matchLabels:
      app: nginx
  serviceName: "nginx"
  replicas: 1
  template:
    metadata:
      labels:
        app: nginx
    spec:
      containers:
      - name: nginx
        image: nginx:1.8
        ports:
        - containerPort: 80
          name: web
        volumeMounts:
        - name: www
          mountPath: /usr/share/nginx/html
  volumeClaimTemplates:
  - metadata:
      name: www
    spec:
      accessModes: [ "ReadWriteOnce" ]
      storageClassName: "nfs"
      resources:
        requests:
          storage: 1Gi
```

<a name="TniW1"></a>
## 18、pod 与node的亲和性

<a name="842aee0b"></a>
#### 查看节点上的标签

```bash
[root@k8s-master ~]# kubectl get nodes --show-labels
NAME         STATUS   ROLES    AGE     VERSION   LABELS
k8s-master   Ready    master   5d22h   v1.17.0   beta.kubernetes.io/arch=amd64,beta.kubernetes.io/os=linux,kubernetes.io/arch=amd64,kubernetes.io/hostname=k8s-master,kubernetes.io/os=linux,node-role.kubernetes.io/master=
k8s-node1    Ready    <none>   5d22h   v1.17.0   beta.kubernetes.io/arch=amd64,beta.kubernetes.io/os=linux,kubernetes.io/arch=amd64,kubernetes.io/hostname=k8s-node1,kubernetes.io/os=linux
k8s-node2    Ready    <none>   5d22h   v1.17.0   beta.kubernetes.io/arch=amd64,beta.kubernetes.io/os=linux,kubernetes.io/arch=amd64,kubernetes.io/hostname=k8s-node2,kubernetes.io/os=linux
[root@k8s-master ~]#
```

<a name="87aac2ed"></a>
#### 使用硬策略在指定的节点创建pod

```yaml
[root@k8s-master ~]# cat pod.yaml
apiVersion: v1
kind: Pod
metadata:
  name: affinity
  labels:
    app: node-affinity-pod
spec:
  containers:
  - name: with-node-affinity
    image: nginx:1.8
  affinity:
    nodeAffinity:
      requiredDuringSchedulingIgnoredDuringExecution:
        nodeSelectorTerms:
        - matchExpressions:
          - key: kubernetes.io/hostname
            operator: NotIn
            values:
            - k8s-node02
[root@k8s-master ~]# kubectl get pod -o wide
NAME                     READY   STATUS    RESTARTS   AGE     IP            NODE        NOMINATED NODE   READINESS GATES
affinity                 1/1     Running   0          48s     10.244.1.18   k8s-node1   <none>           <none>
```

<a name="ba8d1dca"></a>
#### 注意：

要是value的值是不存在的那么创建的pod将一直处于pending的状态

```yaml
[root@k8s-master ~]# cat pod.yaml
apiVersion: v1
kind: Pod
metadata:
  name: affinity
  labels:
    app: node-affinity-pod
spec:
  containers:
  - name: with-node-affinity
    image: nginx:1.8
  affinity:
    nodeAffinity:
      requiredDuringSchedulingIgnoredDuringExecution:
        nodeSelectorTerms:
        - matchExpressions:
          - key: kubernetes.io/hostname
            operator: In
            values:
            - k8s-node03
[root@k8s-master ~]# kubectl get pod -o wide
NAME       READY   STATUS    RESTARTS   AGE   IP       NODE     NOMINATED NODE   READINESS GATES
affinity   0/1     Pending   0          3s    <none>   <none>   <none>           <none>
[root@k8s-master ~]#
```

<a name="75ca3fd2"></a>
#### 软策略

```yaml
[root@k8s-master ~]# cat pod2.yaml
apiVersion: v1
kind: Pod
metadata:
  name: affinity
  labels:
    app: node-affinity-pod
spec:
  containers :
  - name: with-node-affinity
    image: nginx:1.8
  affinity:
    nodeAffinity:
      preferredDuringSchedulingIgnoredDuringExecution:
      - weight: 1
        preference:
          matchExpressions:
          - key: source
            operator: In
            values:
            - k8s-node10
[root@k8s-master ~]# kubectl get pod -o wide
NAME       READY   STATUS    RESTARTS   AGE   IP            NODE        NOMINATED NODE   READINESS GATES
affinity   1/1     Running   0          11s   10.244.1.23   k8s-node1   <none>           <none>
```

**总结**：软策略在条件中要是不匹配的话会在资源相对合理的节点上面创建pod，要是硬策略的就会一直显示pending的状态

另外可以将软策略和硬策略放在一起

<a name="2020-7-30"></a>
# 2020-7-30

<a name="9473142d"></a>
#### 实现pod的暴露直接访问容器

```yaml
apiVersion: v1
kind: Pod
metadata:
  name: webapp
  labels:
    app: webapp
spec:
  containers:
  - name: webapp
    image: nginx:1.8
    imagePullPolicy: IfNotPresent
    ports:
    - containerPort: 80			#容器运行时使用的端口
      hostPort: 80				#访问时使用的端口
```

![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599997926837-b0f9de1a-5663-41a3-a670-24211dc430b5.png#align=left&display=inline&height=142&originHeight=284&originWidth=977&size=70390&status=done&style=none&width=488.5)

<a name="9d4c1145"></a>
#### Pod级别的hostNetwork的设置

```yaml
[root@k8s-master ~]# cat pod-host.yaml
apiVersion: v1
kind: Pod
metadata:
  name: webapp
  labels:
    app: webapp
spec:
  hostNetwork: true
  containers:
  - name: webapp
    image: nginx:1.8
    imagePullPolicy: IfNotPresent
    ports:
    - containerPort: 80
```

查看创建的pod

```bash
[root@k8s-master ~]# kubectl get pod -o wide|grep web
webapp1                         1/1     Running   0          94s   192.168.40.11   k8s-node1   <none>           <none>
```

![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599997944604-0c62ceac-7422-491e-b9bf-ca76032305e6.png#align=left&display=inline&height=145&originHeight=290&originWidth=991&size=79149&status=done&style=none&width=495.5)

备注： 通过设置Pod级别的hostNetwork=true，该Pod中所有容器的端口号都将被直接映射到物理机上。在设置hostNetwork=true时需要注意，在容器的ports定义部分如果不指定hostPort，则默认hostPort等于containerPort，如果指定了hostPort，则hostPort必须等于containerPort的值

<a name="295ce181"></a>
# 马哥Kubernetes

使用命令创建deployment控制器

```bash
[root@k8s-master yaml]# kubectl create deployment nginx --image=nginx -o yaml --dry-run > my-deployment.yaml
```

只是测试的环境可以将配置文件导出也可以去掉 --dry-run 直接运行

```shell
[root@k8s-master yaml]# kubectl create deployment nginx --image=nginx:1.18
deployment.apps/nginx created
[root@k8s-master yaml]# kubectl get deployments.apps
NAME         READY   UP-TO-DATE   AVAILABLE   AGE
nginx        1/1     1            1           6s
```

创建service

```bash
[root@k8s-master yaml]# kubectl expose deployment nginx --name=nginx --port=80 --target-port=80 --protocol=TCP
service/nginx exposed
[root@k8s-master yaml]# kubectl get svc
NAME         TYPE        CLUSTER-IP      EXTERNAL-IP   PORT(S)   AGE
kubernetes   ClusterIP   10.96.0.1       <none>        443/TCP   39d
nginx        ClusterIP   10.96.189.17    <none>        80/TCP    5s
```

访问测试

```bash
[root@k8s-master yaml]# curl 10.96.189.17
<!DOCTYPE html>
<html>
<head>
<title>Welcome to nginx!</title>
..............
<p><em>Thank you for using nginx.</em></p>
</body>
</html>
[root@k8s-master yaml]#
```

<a name="196ac100"></a>
#### 滚动更新

<a name="7d2849e1"></a>
##### step1配置

```bash
[root@k8s-master ~]# kubectl run myapp --image=ikubernetes/myapp:v1 --replicas=2
[root@k8s-master ~]# kubectl expose deployment myapp --name=myapp --port=80
service/myapp exposed
[root@k8s-master ~]# kubectl get svc
NAME         TYPE        CLUSTER-IP      EXTERNAL-IP   PORT(S)   AGE
kubernetes   ClusterIP   10.96.0.1       <none>        443/TCP   39d
myapp        ClusterIP   10.96.88.100    <none>        80/TCP    5s
[root@k8s-master ~]# curl 10.96.88.100
Hello MyApp | Version: v1 | <a href="hostname.html">Pod Name</a>
[root@k8s-master ~]# curl 10.96.88.100/hostname.html
myapp-7c468db58f-xrmqx
[root@k8s-master ~]# curl 10.96.88.100/hostname.html
myapp-7c468db58f-rdpkq
[root@k8s-master ~]#
```

总结：clusterIP的service类型只能在集群内部访问，而且访问的方式是轮询

<a name="f7990382"></a>
##### step2增加副本

```bash
[root@k8s-master ~]# kubectl scale --replicas=5 deployment myapp
deployment.apps/myapp scaled
[root@k8s-master ~]# kubectl get pod
NAME                         READY   STATUS    RESTARTS   AGE
myapp-7c468db58f-84c4k       1/1     Running   0          8s
myapp-7c468db58f-dvll2       1/1     Running   0          8s
myapp-7c468db58f-qx45d       1/1     Running   0          8s
myapp-7c468db58f-rdpkq       1/1     Running   0          11m
myapp-7c468db58f-xrmqx       1/1     Running   0          11m
```

<a name="f48f0b9c"></a>
##### step3减少副本

```bash
[root@k8s-master ~]# kubectl scale --replicas=3 deployment myapp
deployment.apps/myapp scaled
[root@k8s-master ~]# kubectl get pod
NAME                         READY   STATUS    RESTARTS   AGE
myapp-7c468db58f-dvll2       1/1     Running   0          60s
myapp-7c468db58f-rdpkq       1/1     Running   0          12m
myapp-7c468db58f-xrmqx       1/1     Running   0          12m
```

<a name="1b2ed547"></a>
##### step4镜像的更新

```bash
[root@k8s-master ~]# kubectl set image deployments.apps myapp myapp=ikubernetes/myapp:v2
deployment.apps/myapp image updated
[root@k8s-master ~]# kubectl get pod
NAME                         READY   STATUS    RESTARTS   AGE
myapp-64758bffd4-7w2vc       1/1     Running   0          5s
myapp-64758bffd4-frgpq       1/1     Running   0          6s
[root@k8s-master ~]# kubectl describe pod myapp-64758bffd4-7w2vc |grep Image
    Image:          ikubernetes/myapp:v2
[root@k8s-master ~]# curl 10.96.88.100
Hello MyApp | Version: v2 | <a href="hostname.html">Pod Name</a>
```

<a name="d6be8857"></a>
##### step5回滚镜像

```bash
[root@k8s-master ~]# kubectl rollout undo deployment myapp
deployment.apps/myapp rolled back
[root@k8s-master ~]# kubectl get pod
NAME                         READY   STATUS    RESTARTS   AGE
myapp-7c468db58f-9dksr       1/1     Running   0          17s
myapp-7c468db58f-dxhxz       1/1     Running   0          20s
myapp-7c468db58f-tmfxh       1/1     Running   0          19s
[root@k8s-master ~]# curl 10.96.88.100
Hello MyApp | Version: v1 | <a href="hostname.html">Pod Name</a>
```

<a name="NodePort"></a>
##### NodePort

```bash
[root@k8s-master ~]# kubectl edit service myapp
# Please edit the object below. Lines beginning with a '#' will be ignored,
# and an empty file will abort the edit. If an error occurs while saving this file will be
# reopened with the relevant failures.
#
apiVersion: v1
kind: Service
metadata:
  creationTimestamp: "2020-08-29T07:06:48Z"
  labels:
    run: myapp
  name: myapp
  namespace: default
  resourceVersion: "79947"
  selfLink: /api/v1/namespaces/default/services/myapp
  uid: 9f9e448b-82a0-4c7f-b8bd-46ca89ef28df
spec:
  clusterIP: 10.96.88.100
  externalTrafficPolicy: Cluster
  ports:
  - nodePort: 31051
    port: 80
    protocol: TCP
    targetPort: 80
  selector:
    run: myapp
  sessionAffinity: None
  type: NodePort
status:
  loadBalancer: {}
```

查看暴露的端口

```bash
[root@k8s-master ~]# kubectl get svc
NAME         TYPE        CLUSTER-IP      EXTERNAL-IP   PORT(S)        AGE
kubernetes   ClusterIP   10.96.0.1       <none>        443/TCP        39d
myapp        NodePort    10.96.88.100    <none>        80:31051/TCP   27m
```

![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599997971446-4c12d722-924c-4778-8574-022956e98397.png#align=left&display=inline&height=193&originHeight=386&originWidth=970&size=105292&status=done&style=none&width=485)


<a name="8c6a57f3"></a>
#### 标签的使用

```bash
[root@k8s-master ~]# kubectl label -h
Update the labels on a resource.

  *  A label key and value must begin with a letter or number, and may contain letters, numbers, hyphens, dots, and
underscores, up to  63 characters each.
  *  Optionally, the key can begin with a DNS subdomain prefix and a single '/', like example.com/my-app
  *  If --overwrite is true, then existing labels can be overwritten, otherwise attempting to overwrite a label will
result in an error.
  *  If --resource-version is specified, then updates will use this resource version, otherwise the existing
resource-version will be used.

Examples:
  # Update pod 'foo' with the label 'unhealthy' and the value 'true'.
  kubectl label pods foo unhealthy=true

  # Update pod 'foo' with the label 'status' and the value 'unhealthy', overwriting any existing value.
  kubectl label --overwrite pods foo status=unhealthy
[root@k8s-master ~]# kubectl label pod nginx-86c57db685-qfvkz app=nginx --overwrite
pod/nginx-86c57db685-qfvkz not labeled
[root@k8s-master ~]# kubectl get pod  --show-labels
NAME                     READY   STATUS    RESTARTS   AGE   LABELS
nginx-86c57db685-qfvkz   1/1     Running   0          39d   app=nginx,pod-template-hash=86c57db685
```

匹配键值对的方式

```bash
[root@k8s-master ~]# kubectl get pod -l "app in (nginx)"
NAME                     READY   STATUS    RESTARTS   AGE
nginx-86c57db685-qfvkz   1/1     Running   0          39d
[root@k8s-master ~]# kubectl get pod -l "app notin (nginx)"
No resources found in default namespace.
[root@k8s-master ~]#
```

给节点打标签

```bash
[root@k8s-master ~]# kubectl label nodes k8s-node1 disktype=sshd
node/k8s-node1 labeled
[root@k8s-master ~]# kubectl get nodes -l disktype
NAME        STATUS   ROLES    AGE   VERSION
k8s-node1   Ready    <none>   39d   v1.17.0
[root@k8s-master ~]#
```

创建pod的时候可以通过nodeSelector字段指定创建到指定标签的节点上面

<a name="ReplicaSet"></a>
##### ReplicaSet

```yaml
[root@k8s-master yaml]# cat rc-demo.yaml
apiVersion: apps/v1
kind: ReplicaSet
metadata:
  name: myapp
  namespace: default
  labels:
    app: myapp
spec:
  replicas: 2
  selector:
    matchLabels:
      app: myapp
      release: canary
  template:
    metadata:
      name: myapp-pod
      labels:
        app: myapp
        release: canary
        environment: qa
    spec:
      containers:
      - name: myapp-container
        image: ikubernetes/myapp:v1
        imagePullPolicy: IfNotPresent
        ports:
        - name: http
          containerPort: 80
```

<a name="e6efac8c"></a>
##### 访问测试

```bash
[root@k8s-master yaml]# kubectl get pod -o wide
NAME                     READY   STATUS    RESTARTS   AGE    IP            NODE        NOMINATED NODE   READINESS GATES
myapp-7ndb2              1/1     Running   0          5m7s   10.244.1.16   k8s-node1   <none>           <none>
myapp-p2bxh              1/1     Running   0          5m7s   10.244.2.4    k8s-node2   <none>           <none>
nginx-86c57db685-qfvkz   1/1     Running   0          40d    10.244.1.2    k8s-node1   <none>           <none>
[root@k8s-master yaml]# curl 10.244.1.16
Hello MyApp | Version: v1 | <a href="hostname.html">Pod Name</a>
[root@k8s-master yaml]# curl 10.244.2.4
Hello MyApp | Version: v1 | <a href="hostname.html">Pod Name</a>
[root@k8s-master yaml]# curl 10.244.2.4/hostname.html
myapp-p2bxh
[root@k8s-master yaml]# curl 10.244.1.16/hostname.html
myapp-7ndb2
[root@k8s-master yaml]#
```

创建了之后发现有两个pod副本要是将其他pod的标签该成为当前的Pod的副本结果将删除其中的一个pod副本

<a name="deployment"></a>
##### deployment

```bash
[root@k8s-master yaml]# cat ds.yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: myapp-deloy
  namespace: default
spec:
  replicas: 2
  selector:
    matchLabels:
      app: myapp
      release: canary
  template:
    metadata:
      labels:
        app: myapp
        release: canary
    spec:
      containers:
      - name: myapp
        image: ikubernetes/myapp:v1
        ports:
        - name: http
          containerPort: 80
[root@k8s-master yaml]# kubectl apply -f ds.yaml     #apply是申明的方式创建好处修改文件之后还可以使用 kubectl apply -f ds.yaml
deployment.apps/myapp-deloy created
[root@k8s-master yaml]# kubectl get pods -o wide
NAME                           READY   STATUS    RESTARTS   AGE   IP            NODE        NOMINATED NODE   READINESS GATES
myapp-deloy-7d574d56c7-b5rbj   1/1     Running   0          54s   10.244.1.18   k8s-node1   <none>           <none>
myapp-deloy-7d574d56c7-rlmkj   1/1     Running   0          54s   10.244.2.6    k8s-node2   <none>           <none>
nginx-86c57db685-qfvkz         1/1     Running   0          40d   10.244.1.2    k8s-node1   <none>           <none>
[root@k8s-master yaml]# curl 10.244.1.18
Hello MyApp | Version: v1 | <a href="hostname.html">Pod Name</a>
[root@k8s-master yaml]# curl 10.244.2.6
Hello MyApp | Version: v1 | <a href="hostname.html">Pod Name</a>
[root@k8s-master yaml]# kubectl get rs
NAME                     DESIRED   CURRENT   READY   AGE
myapp-deloy-7d574d56c7   2         2         2       74s
nginx-86c57db685         1         1         1       40d
```

![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599997990741-58b16315-90a4-4231-96dc-1cb1c13a269b.png#align=left&display=inline&height=175&originHeight=349&originWidth=987&size=150593&status=done&style=none&width=493.5)

创建deployment会自动创建replicasets，前面的部分是deployment的名称中间的模板的hash值后面的是的pod的hash值

<a name="08578395"></a>
##### 将版本换为v2的版本进行测试

```bash
[root@k8s-master yaml]# kubectl apply -f ds.yaml
deployment.apps/myapp-deloy configured
[root@k8s-master yaml]# kubectl get pod -o wide
NAME                           READY   STATUS    RESTARTS   AGE   IP            NODE        NOMINATED NODE   READINESS GATES
myapp-deloy-798dc9b584-rdpkq   1/1     Running   0          28s   10.244.1.19   k8s-node1   <none>           <none>
myapp-deloy-798dc9b584-xrmqx   1/1     Running   0          25s   10.244.2.7    k8s-node2   <none>           <none>
nginx-86c57db685-qfvkz         1/1     Running   0          40d   10.244.1.2    k8s-node1   <none>           <none>
[root@k8s-master yaml]# curl 10.244.1.19
Hello MyApp | Version: v2 | <a href="hostname.html">Pod Name</a>
[root@k8s-master yaml]# kubectl get rs -o wide
NAME                     DESIRED   CURRENT   READY   AGE     CONTAINERS   IMAGES                 SELECTOR
myapp-deloy-798dc9b584   2         2         2       4m10s   myapp        ikubernetes/myapp:v2   app=myapp,pod-template-hash=798dc9b584,release=canary
myapp-deloy-7d574d56c7   0         0         0       23m     myapp        ikubernetes/myapp:v1   app=myapp,pod-template-hash=7d574d56c7,release=canary
nginx-86c57db685         1         1         1       40d     nginx        nginx                  app=nginx,pod-template-hash=86c57db685
```

备注：可以发现版本是成功的更新上去了，RS保留的一份历史版本作为回滚的保留

<a name="d68f7da0"></a>
##### 查看滚动信息

```bash
[root@k8s-master yaml]# kubectl rollout --help #查看更新的帮助信息
Manage the rollout of a resource.

 Valid resource types include:

  *  deployments
  *  daemonsets
  *  statefulsets
[root@k8s-master yaml]# kubectl rollout history deployment myapp-deloy
deployment.apps/myapp-deloy
REVISION  CHANGE-CAUSE
1         <none>
2         <none>
```

<a name="9c3860fe"></a>
##### 更新补丁

可以通过打补丁或者修改配置文件的方式更新副本数      运行之后在修改配置文件实现修改副本的方式在生产的环境当中式不建议的可能会导致应用程序的终端

```bash
[root@k8s-master yaml]# kubectl patch -h
Update field(s) of a resource using strategic merge patch, a JSON merge patch,
or a JSON patch.

 JSON and YAML formats are accepted.

Examples:
  # Partially update a node using a strategic merge patch. Specify the patch as
JSON.
  kubectl patch node k8s-node-1 -p '{"spec":{"unschedulable":true}}'
----------------------------------------------------
[root@k8s-master yaml]# kubectl patch deployments.apps myapp-deloy -p '{"spec":{"replicas":5}}'                                                                                          deployment.apps/myapp-deloy patched
[root@k8s-master yaml]# kubectl get pod
NAME                           READY   STATUS    RESTARTS   AGE
myapp-deloy-798dc9b584-84c4k   1/1     Running   0          78s
myapp-deloy-798dc9b584-dvll2   1/1     Running   0          78s
myapp-deloy-798dc9b584-qx45d   1/1     Running   0          78s
myapp-deloy-798dc9b584-rdpkq   1/1     Running   0          18m
myapp-deloy-798dc9b584-xrmqx   1/1     Running   0          18m
```

修改maxSurge为1表示的是在更新的时候最多可以有指定的副本+1个数不可用的副本为0个

```bash
[root@k8s-master yaml]# kubectl patch deployments.apps myapp-deloy -p '{"spec":{"strategy":{"rollingUpdate":{"maxSurge":1,"maxUnavailable":0}}}}'
deployment.apps/myapp-deloy patched
[root@k8s-master yaml]# kubectl describe deployments.apps myapp-deloy |grep RollingUpdateStrategy
RollingUpdateStrategy:  0 max unavailable, 1 max surge
[root@k8s-master yaml]#
```

<a name="159a3a80"></a>
##### 更新镜像

```bash
[root@k8s-master yaml]# kubectl set image deployments myapp-deloy myapp=ikubernetes/myapp:v2
deployment.apps/myapp-deloy image updated
[root@k8s-master ~]# kubectl rollout status deployment myapp-deloy    #可以看到更新的相信信息
Waiting for deployment "myapp-deloy" rollout to finish: 1 out of 5 new replicas have been updated...
have been updated...
Waiting for deployment "myapp-deloy" rollout to finish: 3 out of 5 new replicas have been updated...
Waiting for deployment "myapp-deloy" rollout to finish: 3 out of 5 new replicas have been updated...
Waiting for deployment "myapp-deloy" rollout to finish: 4 out of 5 new replicas have been updated...
Waiting for deployment "myapp-deloy" rollout to finish: 4 out of 5 new replicas have been updated...
Waiting for deployment "myapp-deloy" rollout to finish: 4 out of 5 new replicas have been updated...
Waiting for deployment "myapp-deloy" rollout to finish: 1 old replicas are pending termination...
Waiting for deployment "myapp-deloy" rollout to finish: 1 old replicas are pending termination...
deployment "myapp-deloy" successfully rolled out
[root@k8s-master ~]#
```

<a name="a39cf1ca"></a>
##### 存储

**1、empty**

```bash
[root@k8s-master yaml]# cat pod-demo-vol.yaml
apiVersion: v1
kind: Pod
metadata:
  name: pod-demo
  namespace: default
  labels:
    app: myapp
    tier: frontend
  annotations:
    jingwei.com/create-by: "cluster admin"
spec:
  containers:
  - name: myapp
    image: ikubernetes/myapp:v1
    ports:
    - name:
      containerPort: 80
    volumeMounts:
    - name: html
      mountPath: /data/web/html
  - name: busybox
    image: busybox
    imagePullPolicy: IfNotPresent
    volumeMounts:
    - name: html
      mountPath: /data/
    command:
    - "/bin/sh"
    - "-c"
    - "sleep 7200"
  volumes:
  - name: html
    emptyDir: {}
```

写入数据并查看数据

```bash
[root@k8s-master yaml]# kubectl exec -it pod-demo -c busybox -- /bin/sh
/ # ls
bin   data  dev   etc   home  proc  root  sys   tmp   usr   var
/ # date > data/index.html
/ # date >> data/index.html
/ # date >> data/index.html
/ # date >> data/index.html
/ # cat data/index.html
Tue Sep  1 07:34:24 UTC 2020
Tue Sep  1 07:34:29 UTC 2020
Tue Sep  1 07:34:31 UTC 2020
Tue Sep  1 07:34:32 UTC 2020
/ #

[root@k8s-master yaml]# kubectl exec -it pod-demo -c myapp -- /bin/sh
/ # ls
bin    dev    home   media  proc   run    srv    tmp    var
data   etc    lib    mnt    root   sbin   sys    usr
/ # cat data/web/html/index.html
Tue Sep  1 07:34:24 UTC 2020
Tue Sep  1 07:34:29 UTC 2020
Tue Sep  1 07:34:31 UTC 2020
Tue Sep  1 07:34:32 UTC 2020
/ #
```

**2、hostpath**

```bash
[root@k8s-master yaml]# cat hostpath.yaml
apiVersion: v1
kind: Pod
metadata:
  name: test-pd
spec:
  containers:
  - image: nginx:1.8
    name: test-container
    volumeMounts:
    - mountPath: /usr/share/nginx/html
      name: test-volume
  volumes:
  - name: test-volume
    hostPath:
      path: /web
      type: DirectoryOrCreate
[root@k8s-master yaml]#
```

在节点1上面创建文件

```bash
[root@k8s-node1 ~]# mkdir /web
[root@k8s-node1 ~]# echo k8s-node1 > /web/index.html
```

节点2上面创建文件

```bash
[root@k8s-node2 ~]# mkdir /web
[root@k8s-node2 ~]# echo k8s-node2 > /web/index.html
```

应用yaml文件

```bash
[root@k8s-master yaml]# kubectl create -f hostpath.yaml
pod/test-pd created
[root@k8s-master yaml]# kubectl get pod -o wide
NAME                     READY   STATUS    RESTARTS   AGE   IP            NODE        NOMINATED NODE   READINESS GATES
test-pd                  1/1     Running   0          6s    10.244.2.5    k8s-node2   <none>           <none>
[root@k8s-master yaml]# while true; do curl 10.244.2.5;sleep 2; done
k8s-node2
k8s-node2
k8s-node2
k8s-node2
^C
[root@k8s-master yaml]#
```

3、pv

创建pv的时候不能使用namespace字段因为他是集群字段不是名称空间级别，但是可以使用标签

<a name="9aff6241"></a>
# 监控

<a name="d69183e7"></a>
#### 1、metrics

下载镜像

```bash
[root@k8s-node1 ~]# docker pull quay.io/coreos/kube-state-metrics:v1.5.0
```

在另外的机器上面也安装镜像

配置rbac.yaml

```yaml
[root@k8s-master yaml]# cat rbac.yaml
apiVersion: v1
kind: ServiceAccount
metadata:
  labels:
    addonmanager.kubernetes.io/mode: Reconcile
    kubernetes.io/cluster-service: "true"
  name: kube-state-metrics
  namespace: kube-system
---
apiVersion: rbac.authorization.k8s.io/v1
kind: ClusterRole
metadata:
  labels:
    addonmanager.kubernetes.io/mode: Reconcile
    kubernetes.io/cluster-service: "true"
  name: kube-state-metrics
rules:
- apiGroups:
  - ""
  resources:
  - configmaps
  - secrets
  - nodes
  - pods
  - services
  - resourcequotas
  - replicationcontrollers
  - limitranges
  - persistentvolumeclaims
  - persistentvolumes
  - namespaces
  - endpoints
  verbs:
  - list
  - watch
- apiGroups:
  - policy
  resources:
  - poddisruptionbudgets
  verbs:
  - list
  - watch
- apiGroups:
  - extensions
  resources:
  - daemonsets
  - deployments
  - replicasets
  verbs:
  - list
  - watch
- apiGroups:
  - apps
  resources:
  - statefulsets
  verbs:
  - list
  - watch
- apiGroups:
  - batch
  resources:
  - cronjobs
  - jobs
  verbs:
  - list
  - watch
- apiGroups:
  - autoscaling
  resources:
  - horizontalpodautoscalers
  verbs:
  - list
  - watch
---
apiVersion: rbac.authorization.k8s.io/v1
kind: ClusterRoleBinding
metadata:
  labels:
    addonmanager.kubernetes.io/mode: Reconcile
    kubernetes.io/cluster-service: "true"
  name: kube-state-metrics
roleRef:
  apiGroup: rbac.authorization.k8s.io
  kind: ClusterRole
  name: kube-state-metrics
subjects:
- kind: ServiceAccount
  name: kube-state-metrics
  namespace: kube-system
[root@k8s-master yaml]#
```

配置控制器文件

```yaml
[root@k8s-master yaml]# cat rbac.yaml
apiVersion: v1
kind: ServiceAccount
metadata:
  labels:
    addonmanager.kubernetes.io/mode: Reconcile
    kubernetes.io/cluster-service: "true"
  name: kube-state-metrics
  namespace: kube-system
---
apiVersion: rbac.authorization.k8s.io/v1
kind: ClusterRole
metadata:
  labels:
    addonmanager.kubernetes.io/mode: Reconcile
    kubernetes.io/cluster-service: "true"
  name: kube-state-metrics
rules:
- apiGroups:
  - ""
  resources:
  - configmaps
  - secrets
  - nodes
  - pods
  - services
  - resourcequotas
  - replicationcontrollers
  - limitranges
  - persistentvolumeclaims
  - persistentvolumes
  - namespaces
  - endpoints
  verbs:
  - list
  - watch
- apiGroups:
  - policy
  resources:
  - poddisruptionbudgets
  verbs:
  - list
  - watch
- apiGroups:
  - extensions
  resources:
  - daemonsets
  - deployments
  - replicasets
  verbs:
  - list
  - watch
- apiGroups:
  - apps
  resources:
  - statefulsets
  verbs:
  - list
  - watch
- apiGroups:
  - batch
  resources:
  - cronjobs
  - jobs
  verbs:
  - list
  - watch
- apiGroups:
  - autoscaling
  resources:
  - horizontalpodautoscalers
  verbs:
  - list
  - watch
---
apiVersion: rbac.authorization.k8s.io/v1
kind: ClusterRoleBinding
metadata:
  labels:
    addonmanager.kubernetes.io/mode: Reconcile
    kubernetes.io/cluster-service: "true"
  name: kube-state-metrics
roleRef:
  apiGroup: rbac.authorization.k8s.io
  kind: ClusterRole
  name: kube-state-metrics
subjects:
- kind: ServiceAccount
  name: kube-state-metrics
  namespace: kube-system
[root@k8s-master yaml]#
[root@k8s-master yaml]# cat deployment.yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  annotations:
    deployment.kubernetes.io/revision: "2"
  labels:
    grafanak8sapp: "true"
    app: kube-state-metrics
  name: kube-state-metrics
  namespace: kube-system
spec:
  selector:
    matchLabels:
      grafanak8sapp: "true"
      app: kube-state-metrics
  strategy:
    rollingUpdate:
      maxSurge: 25%
      maxUnavailable: 25%
    type: RollingUpdate
  template:
    metadata:
      labels:
        grafanak8sapp: "true"
        app: kube-state-metrics
    spec:
      containers:
      - name: kube-state-metrics
        image: quay.io/coreos/kube-state-metrics:v1.5.0
        imagePullPolicy: IfNotPresent
        ports:
        - containerPort: 8080
          name: http-metrics
          protocol: TCP
        readinessProbe:
          failureThreshold: 3
          httpGet:
            path: /healthz
            port: 8080
            scheme: HTTP
          initialDelaySeconds: 5
          periodSeconds: 10
          successThreshold: 1
          timeoutSeconds: 5
      serviceAccountName: kube-state-metrics
[root@k8s-master yaml]#
```

应用文件

```bash
[root@k8s-master yaml]# kubectl apply -f rbac.yaml
[root@k8s-master yaml]# kubectl apply -f deployment.yaml
[root@k8s-master yaml]# kubectl get pod -n kube-system -o wide |grep kube-state
kube-state-metrics-696bc47ccd-p2bxh   1/1     Running   0          8m46s   10.244.1.17     k8s-node1    <none>           <none>
[root@k8s-master yaml]# curl  10.244.1.17:8080
<html>
             <head><title>Kube Metrics Server</title></head>
             <body>
             <h1>Kube Metrics</h1>
                         <ul>
             <li><a href='/metrics'>metrics</a></li>
             <li><a href='/healthz'>healthz</a></li>
                         </ul>
             </body>
             </html>[roocurl curl 10.244.1.17:8080/healthz
ok[root@k8s-master yaml]#
```

<a name="6bd45826"></a>
#### 2、node-export

```bash
[root@k8s-node1 ~]# docker pull prom/node-exporter:v0.15.0
```

配置ds.yaml

```bash
[root@k8s-master yaml]# cat ds.yaml
apiVersion: apps/v1
kind: DaemonSet
metadata:
  name: node-exporter
  namespace: kube-system
  labels:
    daemon: "node-exporter"
    grafanak8sapp: "true"
spec:
  selector:
    matchLabels:
      daemon: "node-exporter"
      grafanak8sapp: "true"
  template:
    metadata:
      name: node-exporter
      labels:
        daemon: "node-exporter"
        grafanak8sapp: "true"
    spec:
      volumes:
      - name: proc
        hostPath:
          path: /proc
          type: ""
      - name: sys
        hostPath:
          path: /sys
          type: ""
      containers:
      - name: node-exporter
        image: prom/node-exporter:v0.15.0
        imagePullPolicy: IfNotPresent
        args:
        - --path.procfs=/host_proc
        - --path.sysfs=/host_sys
        ports:
        - name: node-exporter
          hostPort: 9100
          containerPort: 9100
          protocol: TCP
        volumeMounts:
        - name: sys
          readOnly: true
          mountPath: /host_sys
        - name: proc
          readOnly: true
          mountPath: /host_proc
      hostNetwork: true
[root@k8s-master yaml]#
```

查看pod

```bash
[root@k8s-master yaml]# kubectl apply -f ds.yaml
daemonset.apps/node-exporter created
[root@k8s-master yaml]# kubectl get pod -n kube-system -o wide  |grep ^node
node-exporter-b5rbj                   1/1     Running   0          3m52s   192.168.40.11   k8s-node1    <none>           <none>
node-exporter-rlmkj                   1/1     Running   0          3m52s   192.168.40.12   k8s-node2    <none>           <none>
[root@k8s-master yaml]#
```

节点查看是否监听到9100端口

```bash
[root@k8s-node1 ~]# netstat -antlp|grep 9100
tcp6       0      0 :::9100                 :::*                    LISTEN      50895/node_exporter
[root@k8s-node1 ~]#
[root@k8s-node2 ~]# netstat -antp|grep 9100
tcp6       0      0 :::9100                 :::*                    LISTEN      44029/node_exporter
[root@k8s-node2 ~]#
```

查看监控到的数据

```bash
[root@k8s-master yaml]# curl 192.168.40.11:9100/metrics |head
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
  0     0    0     0    0     0      0      0 --:--:-- --:--:-- --:--:--     0# HELP go_gc_duration_seconds A summary of the GC invocation durations.
# TYPE go_gc_duration_seconds summary
go_gc_duration_seconds{quantile="0"} 1.8958e-05
go_gc_duration_seconds{quantile="0.25"} 1.8958e-05
go_gc_duration_seconds{quantile="0.5"} 1.8958e-05
go_gc_duration_seconds{quantile="0.75"} 1.8958e-05
go_gc_duration_seconds{quantile="1"} 1.8958e-05
go_gc_duration_seconds_sum 1.8958e-05
go_gc_duration_seconds_count 1
# HELP go_goroutines Number of goroutines that currently exist.
 18  142k   18 27599    0     0  2724k      0 --:--:-- --:--:-- --:--:-- 2994k
curl: (23) Failed writing body (1073 != 16384)
[root@k8s-master yaml]#
[root@k8s-master yaml]# curl 192.168.40.12:9100/metrics |head
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
  0     0    0     0    0     0      0      0 --:--:-- --:--:-- --:--:--     0# HELP go_gc_duration_seconds A summary of the GC invocation durations.
# TYPE go_gc_duration_seconds summary
go_gc_duration_seconds{quantile="0"} 0
go_gc_duration_seconds{quantile="0.25"} 0
go_gc_duration_seconds{quantile="0.5"} 0
go_gc_duration_seconds{quantile="0.75"} 0
go_gc_duration_seconds{quantile="1"} 0
go_gc_duration_seconds_sum 0
go_gc_duration_seconds_count 0
# HELP go_goroutines Number of goroutines that currently exist.
 48  120k   48 59455    0     0   254k      0 --:--:-- --:--:-- --:--:--  254k
curl: (23) Failed writing body (1985 != 16384)
[root@k8s-master yaml]#
```

<a name="cbd83c77"></a>
#### 3、cadvisor

下载镜像

```bash
[root@k8s-node1 ~]# docker pull google/cadvisor:v0.28.3
v0.28.3: Pulling from google/cadvisor
ab7e51e37a18: Pull complete
a2dc2f1bce51: Pull complete
3b017de60d4f: Pull complete
Digest: sha256:9e347affc725efd3bfe95aa69362cf833aa810f84e6cb9eed1cb65c35216632a
Status: Downloaded newer image for google/cadvisor:v0.28.3
```

配置文件

```yaml
[root@k8s-master yaml]# cat ca.yaml
apiVersion: apps/v1
kind: DaemonSet
metadata:
  name: cadvisor
  namespace: kube-system
  labels:
    app: cadvisor
spec:
  selector:
    matchLabels:
      name: cadvisor
  template:
    metadata:
      labels:
        name: cadvisor
    spec:
      hostNetwork: true
      tolerations:
      - key: node-role.kubernetes.io/master
        effect: NoSchedule
      containers:
      - name: cadvisor
        image: google/cadvisor:v0.28.3
        imagePullPolicy: IfNotPresent
        volumeMounts:
        - name: rootfs
          mountPath: /rootfs
          readOnly: true
        - name: var-run
          mountPath: /var/run
        - name: sys
          mountPath: /sys
          readOnly: true
        - name: docker
          mountPath: /var/lib/docker
          readOnly: true
        ports:
          - name: http
            containerPort: 4194
            protocol: TCP
        readinessProbe:
          tcpSocket:
            port: 4194
          initialDelaySeconds: 5
          periodSeconds: 10
        args:
          - --housekeeping_interval=10s
          - --port=4194
      terminationGracePeriodSeconds: 30
      volumes:
      - name: rootfs
        hostPath:
          path: /
      - name: var-run
        hostPath:
          path: /var/run
      - name: sys
        hostPath:
          path: /sys
      - name: docker
        hostPath:
          path: /data/docker
[root@k8s-master yaml]#
```

