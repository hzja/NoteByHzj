Kubernetes
<a name="MdS1u"></a>
## 一、环境准备
<a name="k9FDZ"></a>
### 1.1 硬件要求
| 序号 | 硬件 | 硬件 |
| --- | --- | --- |
| 1 | CPU | 至少2核 |
| 2 | 内存 | 至少2G |
| 3 | 磁盘 | 至少50G |

<a name="MQPsF"></a>
### 1.2 集群节点
| 主机名 | 主机IP |
| --- | --- |
| k8s-master01 | 10.211.55.15 |
| k8s-node01 | 10.211.55.16 |
| k8s-node02 | 10.211.55.17 |
| k8s-node03 | 10.211.55.18 |

<a name="kVban"></a>
## 二、下载 centos
centos下载地址：[http://mirrors.aliyun.com/centos/7/isos/x86_64/](http://mirrors.aliyun.com/centos/7/isos/x86_64/)<br />推荐大家使用 centos7.6 以上版本<br />查看 centos 系统版本命令：
```bash
cat /etc/centos-release
```
配置阿里云 yum 源：
```bash
# 1.下载安装wget 
yum install -y wget 

# 2.备份默认的yum 
mv /etc/yum.repos.d /etc/yum.repos.d.backup 

# 3.设置新的yum目录 
mkdir -p /etc/yum.repos.d 

# 4.下载阿里yum配置到该目录中，选择对应版本 
wget -O /etc/yum.repos.d/CentOS-Base.repo http://mirrors.aliyun.com/repo/Centos-7.repo 

# 5.更新epel源为阿里云epel源 
mv /etc/yum.repos.d/epel.repo /etc/yum.repos.d/epel.repo.backup 
mv /etc/yum.repos.d/epel-testing.repo /etc/yum.repos.d/epel- testing.repo.backup
wget -O /etc/yum.repos.d/epel.repo http://mirrors.aliyun.com/repo/epel- 7.repo

# 6.重建缓存 
yum clean all yum makecache 

# 7.看一下yum仓库有多少包 
yum repolist 
yum update
```
升级系统内核
```bash
rpm -Uvh http://www.elrepo.org/elrepo-release-7.0-3.el7.elrepo.noarch.rpm 
yum --enablerepo=elrepo-kernel install -y kernel-lt 
grep initrd16 /boot/grub2/grub.cfg 
grub2-set-default 0
reboot
```
查看centos系统内核命令：
```bash
uname -r 
uname -a
```
查看CPU命令：
```bash
lscpu
```
查看内存命令：
```bash
free 
free -h
```
查看硬盘信息：
```bash
fdisk -l
```
<a name="WTJfE"></a>
## 三、centos7 系统配置
<a name="g6AXZ"></a>
### 3.1 关闭防火墙
```bash
systemctl stop firewalld 
systemctl disable firewalld
```
<a name="maiqZ"></a>
### 3.2 关闭 selinux
```bash
sed -i 's/SELINUX=enforcing/SELINUX=disabled/g' /etc/sysconfig/selinux
setenforce 0
```
<a name="sUEeP"></a>
### 3.3 网桥过滤
```bash
vi /etc/sysctl.conf 

net.bridge.bridge-nf-call-ip6tables = 1 
net.bridge.bridge-nf-call-iptables = 1 
net.bridge.bridge-nf-call-arptables = 1 
net.ipv4.ip_forward=1 net.ipv4.ip_forward_use_pmtu = 0 

# 生效命令 
sysctl --system
# 查看效果 
sysctl -a|grep "ip_forward"
```
<a name="j3Prr"></a>
### 3.4 开启 IPVS
```bash
# 安装IPVS 
yum -y install ipset ipvsdm

# 编译ipvs.modules文件
vi /etc/sysconfig/modules/ipvs.modules

# 文件内容如下
#!/bin/bash 
modprobe -- ip_vs 
modprobe -- ip_vs_rr 
modprobe -- ip_vs_wrr 
modprobe -- ip_vs_sh 
modprobe -- nf_conntrack_ipv4

# 赋予权限并执行 
chmod 755 /etc/sysconfig/modules/ipvs.modules && bash /etc/sysconfig/modules/ipvs.modules &&lsmod | grep -e ip_vs -e nf_conntrack_ipv4

# 重启电脑，检查是否生效
reboot 
lsmod | grep ip_vs_rr
```
<a name="QVeJ1"></a>
### 3.5 同步时间
```bash
# 安装软件
yum -y install ntpdate

# 向阿里云服务器同步时间
ntpdate time1.aliyun.com

# 删除本地时间并设置时区为上海
rm -rf /etc/localtime 
ln -s /usr/share/zoneinfo/Asia/Shanghai /etc/localtime

# 查看时间 
date -R || date
```
<a name="ckzfb"></a>
### 3.6 命令补全
```bash
# 安装bash-completion 
yum -y install bash-completion bash-completion-extras 
# 使用bash-completion 
source /etc/profile.d/bash_completion.sh
```
<a name="lZbbi"></a>
### 3.7 关闭 swap 分区
```bash
# 临时关闭
swapoff -a

# 永久关闭
vi /etc/fstab

# 将文件中的/dev/mapper/centos-swap这行代码注释掉
# /dev/mapper/centos-swap swap swap defaults 0 0

# 确认swap已经关闭：若swap行都显示 0 则表示关闭成功
free -m
```
<a name="jEvgZ"></a>
### 3.8 hosts 配置
```bash
vi /etc/hosts

# 文件内容如下: 
10.211.55.15 k8s-master01 
10.211.55.16 k8s-node01 
10.211.55.17 k8s-node02 
10.211.55.18 k8s-node03
```
<a name="ETH0F"></a>
## 四、安装 docker
<a name="WaLmX"></a>
### 4.1 阿里云开发者平台
开发者平台官网地址：[https://www.aliyun.com/](https://www.aliyun.com/)，可以参考阿里云官网提供的docker安装教程进行安装。
<a name="z86GL"></a>
### 4.2 安装 docker 前置条件
```bash
yum install -y yum-utils device-mapper-persistent-data lvm2
```
<a name="XWxZh"></a>
### 4.3 添加源
```bash
yum-config-manager --add-repo http://mirrors.aliyun.com/docker-ce/linux/centos/docker-ce.repo 

yum makecache fast
```
<a name="Vp6Os"></a>
### 4.4 查看 docker 更新版本
```bash
yum list docker-ce --showduplicates | sort -r
```
<a name="amvir"></a>
### 4.5 安装 docker 最新版本
```bash
yum -y install docker-ce

# 安装指定版本： 
yum -y install docker-ce-18.09.8
```
<a name="NO56Z"></a>
### 4.6 开启 docker 服务
```bash
systemctl start docker

systemctl status docker
```
<a name="NRJQR"></a>
### 4.7 安装阿里云镜像加速器
```bash
tee /etc/docker/daemon.json <<-'EOF' 
{ 
    "registry-mirrors": ["自己的阿里云镜像加速地址"] 
}
EOF

systemctl daemon-reload 
systemctl restart docker
```
<a name="nZITb"></a>
### 4.8 设置 docker 开机启动服务
```bash
systemctl enable docker
```
<a name="QSYKQ"></a>
### 4.9 修改 Cgroup Driver
```bash
# 修改daemon.json，新增：
vi /etc/docker/daemon.json

"exec-opts": ["native.cgroupdriver=systemd"]

# 重启docker服务
systemctl daemon-reload
systemctl restart docker

# 查看修改后状态
docker info | grep Cgroup
```
> 修改cgroupdriver是为了消除安装k8s集群时的告警：<br />[WARNING IsDockerSystemdCheck]:<br />detected “cgroupfs” as the Docker cgroup driver. The recommended driver is “systemd”.<br />Please follow the guide at https://kubernetes.io/docs/setup/cri/……

<a name="Fjmhq"></a>
### 4.10 复习 docker 常用命令
```
docker -v 
docker version 
docker info
```
<a name="Yyox1"></a>
## 五、使用 kubeadm 快速安装
| 软件 | kubeadm | kubelet | kubectl | docker-ce |
| --- | --- | --- | --- | --- |
| 版本 | 初始化集群管理集群，版本：1.17.5 | 用于接收api-server指令，对pod生命周期进行管理，版本：1.17.5 | 集群命令行管理工具，版本：1.17.5 | 推荐使用版本：19.03.8 |

<a name="bPnNO"></a>
### 5.1 安装 yum 源
<a name="NobsL"></a>
#### 5.1.1 新建 repo 文件
```bash
vim /etc/yum.repos.d/kubernetes.repo
```
<a name="N7gtx"></a>
#### 5.1.2 文件内容
```
[kubernetes]
name=Kubernetes
baseurl=https://mirrors.aliyun.com/kubernetes/yum/repos/kubernetes-el7-x86_64
enabled=1
gpgcheck=1
repo_gpgcheck=1
gpgkey=https://mirrors.aliyun.com/kubernetes/yum/doc/yum-key.gpg
       https://mirrors.aliyun.com/kubernetes/yum/doc/rpm-package-key.gpg
```
如果有报错，则把 gpgcheck、repo_gpgcheck 的值设置成0。
<a name="Xcdgy"></a>
#### 5.1.3 更新缓存
```bash
yum clean all 
yum -y makecache
```
<a name="HZuK8"></a>
#### 5.1.4 验证源是否可用
```bash
yum list | grep kubeadm 
# 如果提示要验证yum-key.gpg是否可用，输入y。 
# 查找到kubeadm。显示版本
```
<a name="vg9H3"></a>
#### 5.1.5 查看 k8s 版本
```bash
yum list kubelet --showduplicates | sort -r
```
<a name="loDKy"></a>
#### 5.1.6 安装 k8s-1.17.5
```bash
yum install -y kubelet-1.17.5 kubeadm-1.17.5 kubectl-1.17.5
```
<a name="chIj0"></a>
### 5.2 设置 kubelet
<a name="PANgB"></a>
#### 5.2.1 增加配置信息
```bash
# 如果不配置kubelet，可能会导致K8S集群无法启动。为实现docker使用的cgroupdriver与kubelet 使用的cgroup的一致性。
vi /etc/sysconfig/kubelet 

KUBELET_EXTRA_ARGS="--cgroup-driver=systemd"
```
<a name="erWUn"></a>
#### 5.2.2 设置开机启动
```bash
systemctl enable kubelet
```
<a name="M6oWI"></a>
### 5.3 初始化镜像
如果是第一次安装k8s，手里没有备份好的镜像，可以执行如下操作。
<a name="SM3JM"></a>
#### 5.3.1 查看安装集群需要的镜像
```bash
kubeadm config images list
```
<a name="sqyVI"></a>
#### 5.3.2 编写执行脚本
```bash
mkdir -p /data
cd /data 
vi k8s.1.17.5-images.sh

images=( 
    kube-apiserver:v1.17.5 
    kube-controller-manager:v1.17.5 
    kube-scheduler:v1.17.5 
    kube-proxy:v1.17.5 
    pause:3.1 
    etcd:3.4.3-0 
    coredns:1.6.5 
)
for imageName in ${images[@]};
do
    docker pull registry.cn-hangzhou.aliyuncs.com/google_containers/$imageName
    docker tag registry.cn-hangzhou.aliyuncs.com/google_containers/$imageName k8s.gcr.io/$imageName
    docker rmi registry.cn-hangzhou.aliyuncs.com/google_containers/$imageName
done
```
<a name="gW1kt"></a>
#### 5.3.3 执行脚本
```bash
cd /data 
# 给脚本授权 
chmod +x images.sh
# 执行脚本 
./k8s.1.17.5-images.sh
```
<a name="XcYez"></a>
#### 5.3.4 保存镜像
```bash
docker save -o k8s.1.17.5.tar \ 
k8s.gcr.io/kube-proxy:v1.17.5 \ 
k8s.gcr.io/kube-apiserver:v1.17.5 \ 
k8s.gcr.io/kube-controller-manager:v1.17.5 \ 
k8s.gcr.io/kube-scheduler:v1.17.5 \ 
k8s.gcr.io/coredns:1.6.5 \ 
k8s.gcr.io/etcd:3.4.3-0 \ 
k8s.gcr.io/pause:3.1 \
```
<a name="e4q8j"></a>
#### 5.3.5 导入镜像
导入master节点镜像tar包
```bash
# master节点需要全部镜像 
docker load -i k8s.1.17.5.tar
```
导入node节点镜像tar包
```bash
# node节点需要kube-proxy:v1.17.5和pause:3.1,2个镜像 
docker load -i k8s.1.17.5.node.tar
```
<a name="zx5qb"></a>
### 5.4 初始化集群
配置k8s集群网络
<a name="c8fpx"></a>
#### 5.4.1 calico 官网地址
```bash
# 官网下载地址： 
https://docs.projectcalico.org/v3.14/manifests/calico.yaml 
# github地址： 
https://github.com/projectcalico/calico 
# 镜像下载： 
docker pull calico/cni:v3.14.2 
docker pull calico/pod2daemon-flexvol:v3.14.2 
docker pull calico/node:v3.14.2 
docker pull calico/kube-controllers:v3.14.2

# 配置hostname： 
hostnamectl set-hostname k8s-master01
```
<a name="wHw4n"></a>
#### ![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667455013294-52ae18ca-c185-4cf7-adc3-08f1e8499602.png#clientId=ufce5c5d8-a580-4&from=paste&id=udba5c9ee&originHeight=322&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u346e486f-17b5-4990-99ef-b5c724337fd&title=)
需要这么多镜像
<a name="Uur2Z"></a>
#### 5.4.2 初始化集群信息:calico网络
```bash
kubeadm init --apiserver-advertise-address=192.168.198.156 --kubernetes- version v1.17.5 --service-cidr=10.1.0.0/16 --pod-network-cidr=10.81.0.0/16
```
192.168.198.156 替换成主节点的IP地址。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667455013119-0a6d10a7-79d7-4095-8747-677638d58bb5.png#clientId=ufce5c5d8-a580-4&from=paste&id=u62ff9236&originHeight=423&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf047d044-1110-4ee9-9775-2e8a8155307&title=)
<a name="uzPgL"></a>
#### 5.4.3 执行配置命令
```bash
mkdir -p $HOME/.kube 
sudo cp -i /etc/kubernetes/admin.conf $HOME/.kube/config 
sudo chown $(id -u):$(id -g) $HOME/.kube/config
```
<a name="Bls0Y"></a>
#### 5.4.4 node节点加入集群信息
```bash
kubeadm join 192.168.198.186:6443 --token kksfgq.b9bhf82y35ufw4np \ 
    --discovery-token-ca-cert-hash sha256:e1e347e6db1db5c13fcdc2c7d51a2f9029100a4cc13c2d89a2dbfa5077f5b07f
```
<a name="BAl28"></a>
#### 5.4.5 kubectl命令自动补全
```bash
echo "source <(kubectl completion bash)" >> ~/.bash_profile 
source ~/.bash_profile
```
<a name="rMocE"></a>
#### 5.4.6 测试 k8s 集群环境
```bash
kubectl get nodes
```
<a name="om7d9"></a>
#### ![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667455013128-e2f6f616-012b-41b2-8760-13b89cbb54ef.png#clientId=ufce5c5d8-a580-4&from=paste&id=u3fc5b994&originHeight=224&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u40477140-3965-4d06-89aa-471ea3b4697&title=)
发现集群的节点都是 NotReady 的状态，这是因为还没有应用 calico 网络的配置。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667455013181-e6037b2b-9d69-423b-987c-e2570e7709dd.png#clientId=ufce5c5d8-a580-4&from=paste&id=u4216c0ad&originHeight=259&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4f833aa0-4fc1-404a-859b-7e7ff86b560&title=)
```bash
kubectl apply -f calico.yml
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667455013192-1d3be311-cf4b-4195-8dd0-a577e021c012.png#clientId=ufce5c5d8-a580-4&from=paste&id=u6ea11317&originHeight=262&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u799887cd-0c6a-45e4-8bb7-d7ada769869&title=)<br />执行完后，发现整个 k8s 集群环境是正常的了，至此，k8s 集群搭建实战完成。
