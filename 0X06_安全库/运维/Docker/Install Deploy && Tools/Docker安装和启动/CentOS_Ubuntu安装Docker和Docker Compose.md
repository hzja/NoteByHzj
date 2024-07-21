Docker
<a name="SDRAZ"></a>
## Docker 简介
Docker 是一个开源的应用容器引擎，基于 Go 语言 并遵从 Apache2.0 协议开源。<br />Docker 可以让开发者打包他们的应用以及依赖包到一个轻量级、可移植的容器中，然后发布到任何流行的 Linux 机器上，也可以实现虚拟化。<br />容器是完全使用沙箱机制，相互之间不会有任何接口（类似 iPhone 的 app），更重要的是容器性能开销极低。<br />Docker 从 17.03 版本之后分为 CE（Community Edition: 社区版） 和 EE（Enterprise Edition: 企业版），用社区版就可以了。
<a name="E2iDv"></a>
## Docker 安装
<a name="hWPef"></a>
### CentOS Docker 安装
```bash
## 一键安装 docker
curl -sSL https://get.docker.com/ | sh
# 或
wget -qO- https://get.docker.com/ | sh
```
<a name="ZYgxj"></a>
### 快速安装
```bash
yum install -y yum-utils  device-mapper-persistent-data lvm2 && \
yum-config-manager --add-repo http://mirrors.aliyun.com/docker-ce/linux/centos/docker-ce.repo && \
yum install -y docker-ce && \
mkdir -p /etc/docker && \
cat > /etc/docker/daemon.json <<EOF
{
  "registry-mirrors": [
  "https://2h3po24q.mirror.aliyuncs.com",
  "https://dockerhub.azk8s.cn",
  "https://mirror.ccs.tencentyun.com",
  "https://docker.mirrors.ustc.edu.cn"
  ],
  "exec-opts": ["native.cgroupdriver=systemd"],
  "log-driver": "json-file",
  "log-opts": {
    "max-size": "100m"
  }
}
EOF


sed -i 's|SELINUX=enforcing|SELINUX=disabled|' /etc/sysconfig/selinux && \
sed -i 's|SELINUX=enforcing|SELINUX=disabled|' /etc/selinux/config && \


cat > /etc/sysctl.d/docker.conf <<EOF
net.ipv4.ip_forward                 = 1
EOF

sysctl -p /etc/sysctl.d/docker.conf


curl -L https://github.com/docker/compose/releases/download/1.25.0/docker-compose-`uname -s`-`uname -m` -o /usr/local/bin/docker-compose && chmod +x /usr/local/bin/docker-compose && \
systemctl daemon-reload && systemctl enable --now docker && \
docker -v && docker-compose -v

Docker version 19.03.5, build 633a0ea
docker-compose version 1.25.0, build 0a186604
```
<a name="ji4Jr"></a>
### 一步一步操作
<a name="DbksR"></a>
#### 安装所需依赖
```bash
yum install -y yum-utils  device-mapper-persistent-data lvm2
```
<a name="udVjv"></a>
#### 添加 docker repo源
```bash
# 使用阿里云镜像 repo 源(国内网络环境推荐阿里云)
yum-config-manager --add-repo http://mirrors.aliyun.com/docker-ce/linux/centos/docker-ce.repo

# 或使用官方 repo 源
yum-config-manager --add-repo https://download.docker.com/linux/centos/docker-ce.repo
```
<a name="oQM9E"></a>
#### 安装 docker-ce
```bash
yum install -y docker-ce
```
<a name="eaCca"></a>
#### 确认安装
```bash
docker -v

Docker version 19.03.1, build 74b1e89
```
<a name="uRKLT"></a>
#### 配置 国内镜像加速
```bash
mkdir -p /etc/docker

cat > /etc/docker/daemon.json <<EOF
{
  "registry-mirrors": [
    "https://dockerhub.azk8s.cn",
    "https://mirror.ccs.tencentyun.com",
    "https://docker.mirrors.ustc.edu.cn"
  ],
  "exec-opts": ["native.cgroupdriver=systemd"],
  "log-driver": "json-file",
  "log-opts": {
    "max-size": "100m"
  }
}
EOF
```
<a name="GTTZU"></a>
#### 关闭 selinux
```bash
# 确认 selinux 状态为 disabled 如果不是需要关闭
sestatus

sed -i 's|SELINUX=enforcing|SELINUX=disabled|' /etc/sysconfig/selinux && \
sed -i 's|SELINUX=enforcing|SELINUX=disabled|' /etc/selinux/config && \

reboot
```
<a name="sKJRc"></a>
#### 开启主机报文转发 不开启会导致docker容器里面访问不到外网
```bash
cat > /etc/sysctl.d/docker.conf <<EOF
net.ipv4.ip_forward                 = 1
EOF && sysctl -p
```
<a name="xFwNZ"></a>
#### 开启2375管理端口 (外网服务器请勿开启 会导致 docker 服务器未授权访问)
```bash
vim /usr/lib/systemd/system/docker.service

#修改ExecStart 处为
ExecStart=/usr/bin/dockerd -H tcp://0.0.0.0:2375 -H unix://var/run/docker.sock
```
<a name="OMGhR"></a>
#### 重载配置 && 启动 docker && 配置 docker 开机自启
```bash
systemctl daemon-reload && systemctl enable --now docker
```
<a name="zho33"></a>
#### 确认docker安装
```bash
docker -v

Docker version 19.03.12, build xx
```
<a name="tuTO3"></a>
#### 测试 docker
```bash
docker run --rm hello-world
Unable to find image 'hello-world:latest' locally
latest: Pulling from library/hello-world
1b930d010525: Pull complete 
Digest: sha256:c3b4ada4687bbaa170745b3e4dd8ac3f194ca95b2d0518b417fb47e5879d9b5f
Status: Downloaded newer image for hello-world:latest

Hello from Docker!
This message shows that your installation appears to be working correctly.

To generate this message, Docker took the following steps:
 1. The Docker client contacted the Docker daemon.
 2. The Docker daemon pulled the "hello-world" image from the Docker Hub.
    (amd64)
 3. The Docker daemon created a new container from that image which runs the
    executable that produces the output you are currently reading.
 4. The Docker daemon streamed that output to the Docker client, which sent it
    to your terminal.

To try something more ambitious, you can run an Ubuntu container with:
 $ docker run -it ubuntu bash

Share images, automate workflows, and more with a free Docker ID:
 https://hub.docker.com/

For more examples and ideas, visit:
 https://docs.docker.com/get-started/
```
<a name="fQC1e"></a>
## Ubuntu Docker 安装
Docker Engine-Community 支持以下的 Ubuntu 版本：

- Xenial 16.04 (LTS)
- Bionic 18.04 (LTS)
- Cosmic 18.10
- Disco 19.04
- 其他更新的版本……

Docker Engine - Community 支持上 x86_64（或 amd64）armhf，arm64，s390x （IBM Z），和 ppc64le（IBM的Power）架构。
<a name="vFGIV"></a>
### 使用官方安装脚本自动安装
安装命令如下：
```bash
curl -fsSL https://get.docker.com | bash -s docker --mirror Aliyun
```
也可以使用国内 daocloud 一键安装命令：
```bash
curl -sSL https://get.daocloud.io/docker | sh
```
<a name="bBZBx"></a>
### 手动安装
<a name="ZYgRt"></a>
#### 卸载旧版本
Docker 的旧版本被称为 docker，docker.io 或 docker-engine 。如果已安装，请卸载它们：
```bash
$ sudo apt-get remove docker docker-engine docker.io containerd runc
```
当前称为 Docker Engine-Community 软件包 docker-ce 。<br />安装 Docker Engine-Community，以下介绍两种方式。
<a name="PZXMC"></a>
#### 使用 Docker 仓库进行安装
在新主机上首次安装 Docker Engine-Community 之前，需要设置 Docker 仓库。之后，您可以从仓库安装和更新 Docker 。
<a name="FhfIK"></a>
#### 设置仓库
更新 apt 包索引。
```bash
$ sudo apt-get update
```
安装 apt 依赖包，用于通过HTTPS来获取仓库：
```bash
$ sudo apt-get install \ 
apt-transport-https \ 
ca-certificates \ 
curl \ 
gnupg-agent \ 
software-properties-common
```
添加 Docker 的官方 GPG 密钥：
```bash
$ curl -fsSL https://mirrors.ustc.edu.cn/docker-ce/linux/ubuntu/gpg | sudo apt-key add -
9DC8 5822 9FC7 DD38 854A E2D8 8D81 803C 0EBF CD88
```
通过搜索指纹的后8个字符，验证现在是否拥有带有指纹的密钥。
```bash
$ sudo apt-key fingerprint 0EBFCD88
pub rsa4096 2017-02-22 [SCEA]9DC8 5822 9FC7 DD38 854A E2D8 8D81 803C 0EBF CD88

uid [ unknown] Docker Release (CE deb)<docker@docker.com>sub rsa4096 2017-02-22 [S]
```
使用以下指令设置稳定版仓库
```bash
$ sudo add-apt-repository \
   "deb [arch=amd64] https://mirrors.ustc.edu.cn/docker-ce/linux/ubuntu/ \
  $(lsb_release -cs) \
  stable"
```
<a name="MJhth"></a>
#### 安装 Docker Engine-Community
更新 apt 包索引。
```bash
$ sudo apt-get update
```
安装最新版本的 Docker Engine-Community 和 containerd ，或者转到下一步安装特定版本：
```bash
$ sudo apt-get install docker-ce docker-ce-cli containerd.io
```
要安装特定版本的 Docker Engine-Community，请在仓库中列出可用版本，然后选择一种安装。列出您的仓库中可用的版本：
```bash
$ apt-cache madison docker-ce
```
使用第二列中的版本字符串安装特定版本，例如 5:18.09.1~3-0~ubuntu-xenial。
```bash
$ sudo apt-get install docker-ce=<VERSION_STRING> docker-ce-cli=<VERSION_STRING> containerd.io
```
测试 Docker 是否安装成功，输入以下指令，打印出以下信息则安装成功：
```bash
$ sudo docker run hello-world
Unable to find image 'hello-world:latest' locallylatest: Pulling from library/hello-world1b930d010525: Pull complete                                                                                                                                  Digest: sha256:c3b4ada4687bbaa170745b3e4dd8ac3f194ca95b2d0518b417fb47e5879d9b5fStatus: Downloaded newer image for hello-world:latest

Hello from Docker!This message shows that your installation appears to be working correctly.

To generate this message, Docker took the following steps: 1. The Docker client contacted the Docker daemon. 2. The Docker daemon pulled the "hello-world" image from the Docker Hub.    (amd64) 3. The Docker daemon created a new container from that image which runs the    executable that produces the output you are currently reading. 4. The Docker daemon streamed that output to the Docker client, which sent it    to your terminal.

To try something more ambitious, you can run an Ubuntu container with: $ docker run -it ubuntu bash

Share images, automate workflows, and more with a free Docker ID: https://hub.docker.com/

For more examples and ideas, visit: https://docs.docker.com/get-started/
```
<a name="IjStY"></a>
### 使用 Shell 脚本进行安装
Docker 在 get.docker.com 和 test.docker.com 上提供了方便脚本，用于将快速安装 Docker Engine-Community 的边缘版本和测试版本。脚本的源代码在 docker-install 仓库中。不建议在生产环境中使用这些脚本，在使用它们之前，您应该了解潜在的风险：

- 脚本需要运行 root 或具有 sudo 特权。因此，在运行脚本之前，应仔细检查和审核脚本。
- 这些脚本尝试检测 Linux 发行版和版本，并为您配置软件包管理系统。此外，脚本不允许您自定义任何安装参数。从 Docker 的角度或您自己组织的准则和标准的角度来看，这可能导致不支持的配置。
- 这些脚本将安装软件包管理器的所有依赖项和建议，而无需进行确认。这可能会安装大量软件包，具体取决于主机的当前配置。
- 该脚本未提供用于指定要安装哪个版本的 Docker 的选项，而是安装了在 edge 通道中发布的最新版本。
- 如果已使用其他机制将 Docker 安装在主机上，请不要使用便捷脚本。

本示例使用 get.docker.com 上的脚本在 Linux 上安装最新版本的Docker Engine-Community。要安装最新的测试版本，请改用 test.docker.com。在下面的每个命令，取代每次出现 get 用 test。
```bash
$ curl -fsSL https://get.docker.com -o get-docker.sh

$ sudo sh get-docker.sh
```
如果要使用 Docker 作为非 root 用户，则应考虑使用类似以下方式将用户添加到 docker 组：
```bash
$ sudo usermod -aG docker your-user
```
<a name="mDgGN"></a>
### 卸载 docker
删除安装包：
```bash
sudo apt-get purge docker-ce
```
删除镜像、容器、配置文件等内容：
```bash
sudo rm -rf /var/lib/docker
```
<a name="tTN3Q"></a>
## Docker Compose 简介
Docker Compose 是用于定义和运行多容器 Docker 应用程序的工具。通过 Docker Compose，您可以使用 YML 文件来配置应用程序需要的所有服务。然后，使用一个命令，就可以从 YML 文件配置中创建并启动所有服务。
<a name="Pqtye"></a>
### 安装 docker-compose 1.26.2版本
```bash
curl -L https://github.com/docker/compose/releases/download/1.26.2/docker-compose-`uname -s`-`uname -m` -o /usr/local/bin/docker-compose && chmod +x /usr/local/bin/docker-compose
```
<a name="J3OXT"></a>
### 高版本(推荐)
```bash
sudo curl -L "https://github.com/docker/compose/releases/download/v2.2.2/docker-compose-$(uname -s)-$(uname -m)" -o /usr/local/bin/docker-compose
```
Docker Compose 存放在 GitHub，不太稳定。可以也通过执行下面的命令，高速安装 Docker Compose。
```bash
curl -L https://get.daocloud.io/docker/compose/releases/download/v2.4.1/docker-compose-`uname -s`-`uname -m` > /usr/local/bin/docker-compose
```
将可执行权限应用于二进制文件：
```bash
$ sudo chmod +x /usr/local/bin/docker-compose
```
创建软链：
```bash
$ sudo ln -s /usr/local/bin/docker-compose /usr/bin/docker-compose
```
<a name="IiNMf"></a>
### 确认docker-compose 安装
```bash
docker-compose -v

docker-compose version 1.26.2, build xx
```
