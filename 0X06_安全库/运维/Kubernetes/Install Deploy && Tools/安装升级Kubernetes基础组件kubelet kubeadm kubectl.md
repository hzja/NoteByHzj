Kubernetes
<a name="l1d3G"></a>
## 1、安装Kubernetes基础组件
<a name="XkRr5"></a>
### 配置镜像仓库
<a name="mYcQd"></a>
#### 官方镜像源国内不可用
```bash
cat <<EOF > /etc/yum.repos.d/kubernetes.repo
[kubernetes]
name=Kubernetes
baseurl=https://packages.cloud.google.com/yum/repos/kubernetes-el7-x86_64
enabled=1
gpgcheck=1
repo_gpgcheck=1
gpgkey=https://packages.cloud.google.com/yum/doc/yum-key.gpg https://packages.cloud.google.com/yum/doc/rpm-package-key.gpg
EOF
$ yum install -y kubelet kubeadm kubectl
systemctl enable kubelet && systemctl start kubelet
```
<a name="j7W07"></a>
#### 配置使用阿里云镜像仓库并下载安装推荐
阿里云Kubernetes镜像地址：[https://developer.aliyun.com/mirror/kubernetes?spm=a2c6h.13651102.0.0.5b791b11ILfyDS](https://developer.aliyun.com/mirror/kubernetes?spm=a2c6h.13651102.0.0.5b791b11ILfyDS)
<a name="X97U9"></a>
##### Debian / Ubuntu
```bash
apt-get update && apt-get install -y apt-transport-https
curl https://mirrors.aliyun.com/kubernetes/apt/doc/apt-key.gpg | apt-key add - 
cat <<EOF >/etc/apt/sources.list.d/kubernetes.list
deb https://mirrors.aliyun.com/kubernetes/apt/ kubernetes-xenial main
EOF
apt-get update
apt-get install -y kubelet kubeadm kubectl
systemctl enable kubelet && systemctl start kubelet
```
<a name="Ksv2C"></a>
##### CentOS / RHEL / Fedora
```bash
cat <<EOF > /etc/yum.repos.d/kubernetes.repo
[kubernetes]
name=Kubernetes
baseurl=https://mirrors.aliyun.com/kubernetes/yum/repos/kubernetes-el7-x86_64/
enabled=1
gpgcheck=1
repo_gpgcheck=1
gpgkey=https://mirrors.aliyun.com/kubernetes/yum/doc/yum-key.gpg https://mirrors.aliyun.com/kubernetes/yum/doc/rpm-package-key.gpg
EOF
setenforce 0
yum install -y kubelet kubeadm kubectl
systemctl enable kubelet && systemctl start kubelet
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1639969800373-22e9a916-61f7-4860-a622-a55a4184a2e5.png#clientId=ue647eac4-e7fd-4&from=paste&height=280&id=u4e320671&originHeight=840&originWidth=2504&originalType=binary&ratio=1&rotation=0&showTitle=false&size=191456&status=done&style=none&taskId=u135c0728-d63c-4eb2-8bf6-6dab54aef5a&title=&width=834.6666666666666)

