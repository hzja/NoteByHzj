Kubernetes
:::info
前提条件：

1. 访问主机CentOS8的和Docker的驱动不兼容，所以不能使用CentOS8以上系列的服务主机
2. 2核CPU的服务主机（必要）
3. 大于2G 内存的服务器主机[大于2G是为了防止其他硬件占用内存导致Kubernetes的使用内存不足的问题]（必要）
4. 这里的驱动是基于Docker，如果未安装则需要安装启动Docker，执行命令：`yum install -y docker`
:::
<a name="uQSyB"></a>
## 1、先决条件
<a name="sOzLu"></a>
### 安装Docker
[Docker安装和启动](https://www.yuque.com/fcant/operation/docker-install?view=doc_embed)
<a name="BAIwH"></a>
### 安装kubelet kubeadm kubectl
[安装升级Kubernetes基础组件kubelet kubeadm kubectl](https://www.yuque.com/fcant/operation/nxptw8?view=doc_embed)

<a name="KDHX3"></a>
## 2、安装minikube
参考：[https://minikube.sigs.k8s.io/docs/start/](https://minikube.sigs.k8s.io/docs/start/)
<a name="yNJNJ"></a>
### rpm方式安装
[minikube-latest.x86_64.zip](https://www.yuque.com/attachments/yuque/0/2021/zip/396745/1623129470514-81d5d3f2-21f3-4345-9ed7-a1f29776ca1a.zip?_lake_card=%7B%22src%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2021%2Fzip%2F396745%2F1623129470514-81d5d3f2-21f3-4345-9ed7-a1f29776ca1a.zip%22%2C%22name%22%3A%22minikube-latest.x86_64.zip%22%2C%22size%22%3A15078534%2C%22type%22%3A%22application%2Fx-zip-compressed%22%2C%22ext%22%3A%22zip%22%2C%22source%22%3A%22%22%2C%22status%22%3A%22done%22%2C%22mode%22%3A%22title%22%2C%22download%22%3Atrue%2C%22taskId%22%3A%22u4370fc95-8bc2-4e2c-8c02-956bfcdbb0b%22%2C%22taskType%22%3A%22upload%22%2C%22id%22%3A%22u38beba81%22%2C%22card%22%3A%22file%22%7D)
```bash
rpm -Uvh minikube-latest.x86_64.rpm
```
<a name="cixVG"></a>
### 命令安装
```bash
curl -LO https://storage.googleapis.com/minikube/releases/latest/minikube-linux-amd64
sudo install minikube-linux-amd64 /usr/local/bin/minikube
```
<a name="AdwVc"></a>
## 3、创建非root用户以及Docker组，并授权
```bash
useradd fcant
passwd fcant
groupadd docker
usermod -aG docker fcant
usermod -s /bin/bash fcant
```
:::danger
注意：如果在创建非root用户时已经启动Docker，则需要在用户授权成功后重启Docker使授权生效。<br />执行命令：`systemctl restart docker`<br />避免的问题：在非root用户启动时，导致无法读取的权限错误。
:::
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1623129428903-280201c6-0e05-448a-96c4-a059706281e2.png#clientId=u69ce4cd2-2bff-4&from=paste&height=583&id=uc5773b21&originHeight=1750&originWidth=3323&originalType=binary&ratio=3&rotation=0&showTitle=false&size=2113740&status=done&style=shadow&taskId=u93868356-5982-4715-8f12-3447c9bb0b9&title=&width=1107.6666666666667)
<a name="ZswM5"></a>
## 4、为非root用户授予root用户权限
:::tips
防止后面的操作中出现创建的非root用户没有权限的错误<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1623140371947-6c65722e-d26c-42a6-b911-491fd1147929.png#clientId=u88d530d2-dad5-4&from=paste&height=583&id=u553f5c24&originHeight=1750&originWidth=3323&originalType=binary&ratio=3&rotation=0&showTitle=false&size=2035550&status=done&style=shadow&taskId=ua463cd0d-3422-4e8b-8535-09543c45f47&title=&width=1107.6666666666667)
:::
```bash
sudo visudo
# 或者下面的命令、vim编辑时有高亮提示
vim /etc/sudoers
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1623140439213-8db77cc2-c21d-437f-aaee-6c2fe7e84adc.png#clientId=u88d530d2-dad5-4&from=paste&height=583&id=u80a2dd58&originHeight=1750&originWidth=3323&originalType=binary&ratio=3&rotation=0&showTitle=false&size=2103261&status=done&style=shadow&taskId=u3745cfe0-eadd-4e8c-9c04-1c581032193&title=&width=1107.6666666666667)
<a name="hExHH"></a>
## 5、切换非root用户，启动minikube
国内存在网络问题，所以启动时指定了镜像仓库地址
```bash
$ su fcant
$ minikube start --image-repository=registry.cn-hangzhou.aliyuncs.com/google_containers
😄  minikube v1.20.0 on Centos 7.6.1810 (amd64)
✨  Using the docker driver based on existing profile
👍  Starting control plane node minikube in cluster minikube
🚜  Pulling base image ...
    > index.docker.io/kicbase/sta...: 358.10 MiB / 358.10 MiB  100.00% 2.78 MiB
🤷  docker "minikube" container is missing, will recreate.
🔥  Creating docker container (CPUs=2, Memory=2200MB) ...
🐳  Preparing Kubernetes v1.20.2 on Docker 20.10.6 ...
    ▪ Generating certificates and keys ...
    ▪ Booting up control plane ...
    ▪ Configuring RBAC rules ...
🔎  Verifying Kubernetes components...
    ▪ Using image registry.cn-hangzhou.aliyuncs.com/google_containers/kubernetesui/dashboard:v2.1.0@sha256:7f80b5ba141bead69c4fee8661464857af300d7d7ed0274cf7beecedc00322e6 (global image repository)
    ▪ Using image registry.cn-hangzhou.aliyuncs.com/google_containers/k8s-minikube/storage-provisioner:v5 (global image repository)
    ▪ Using image registry.cn-hangzhou.aliyuncs.com/google_containers/kubernetesui/metrics-scraper:v1.0.4@sha256:555981a24f184420f3be0c79d4efb6c948a85cfce84034f85a563f4151a81cbf (global image repository)
🌟  Enabled addons: default-storageclass, storage-provisioner, dashboard
🏄  Done! kubectl is now configured to use "minikube" cluster and "default" namespace by default
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1623134777952-1e727206-34c2-486d-a692-79ae1b9ff4e8.png#clientId=u69ce4cd2-2bff-4&from=paste&height=468&id=uf04983ee&originHeight=1405&originWidth=3323&originalType=binary&ratio=3&rotation=0&showTitle=false&size=1778805&status=done&style=shadow&taskId=u52f8ee81-1f2b-4643-b93e-d8e0df0157e&title=&width=1107.6666666666667)
<a name="J7rdn"></a>
### 出错1：failed to parse kernel config: unable to load kernel module: "configs"
<a name="zOOJX"></a>
#### 错误内容
```bash
$ minikube start --image-repository=registry.cn-hangzhou.aliyuncs.com/google_containers --driver=docker --extra-config=kubelet.cgroup-driver=systemd
😄  minikube v1.26.1 on Ubuntu 22.04
✨  Using the docker driver based on existing profile
👍  Starting control plane node minikube in cluster minikube
🚜  Pulling base image ...
🏃  Updating the running docker "minikube" container ...
🐳  Preparing Kubernetes v1.24.3 on Docker 20.10.17 ...
    ▪ kubelet.cgroup-driver=systemd
    ▪ Generating certificates and keys ...
💢  initialization failed, will try again: wait: /bin/bash -c "sudo env PATH="/var/lib/minikube/binaries/v1.24.3:$PATH" kubeadm init --config /var/tmp/minikube/kubeadm.yaml  --ignore-preflight-errors=DirAvailable--etc-kubernetes-manifests,DirAvailable--var-lib-minikube,DirAvailable--var-lib-minikube-etcd,FileAvailable--etc-kubernetes-manifests-kube-scheduler.yaml,FileAvailable--etc-kubernetes-manifests-kube-apiserver.yaml,FileAvailable--etc-kubernetes-manifests-kube-controller-manager.yaml,FileAvailable--etc-kubernetes-manifests-etcd.yaml,Port-10250,Swap,Mem,SystemVerification,FileContent--proc-sys-net-bridge-bridge-nf-call-iptables": Process exited with status 1
stdout:
[init] Using Kubernetes version: v1.24.3
[preflight] Running pre-flight checks
[preflight] The system verification failed. Printing the output from the verification:    
KERNEL_VERSION: 5.15.0-46-generic                                                         
OS: Linux                                                                                 
CGROUPS_CPU: enabled                                                                      
CGROUPS_CPUSET: enabled                                                                   
CGROUPS_DEVICES: enabled                                                                  
CGROUPS_FREEZER: enabled                                                                  
CGROUPS_MEMORY: enabled                                                                   
CGROUPS_PIDS: enabled                                                                     
CGROUPS_HUGETLB: enabled                                                                  
CGROUPS_BLKIO: missing                                                                    
[preflight] Pulling images required for setting up a Kubernetes cluster
[preflight] This might take a minute or two, depending on the speed of your internet connection
[preflight] You can also perform this action in beforehand using 'kubeadm config images pull'
[certs] Using certificateDir folder "/var/lib/minikube/certs"
[certs] Using existing ca certificate authority
[certs] Using existing apiserver certificate and key on disk

stderr:
W0901 15:17:29.597926    4076 initconfiguration.go:120] Usage of CRI endpoints without URL scheme is deprecated and can cause kubelet errors in the future. Automatically prepending scheme "unix" to the "criSocket" with value "/var/run/cri-dockerd.sock". Please update your configuration!
        [WARNING SystemVerification]: missing optional cgroups: blkio
        [WARNING SystemVerification]: failed to parse kernel config: unable to load kernel module: "configs", output: "modprobe: FATAL: Module configs not found in directory /lib/modules/5.15.0-46-generic\n", err: exit status 1
        [WARNING Service-Kubelet]: kubelet service is not enabled, please run 'systemctl enable kubelet.service'
error execution phase certs/apiserver-kubelet-client: [certs] certificate apiserver-kubelet-client not signed by CA certificate ca: x509: certificate signed by unknown authority (possibly because of "crypto/rsa: verification error" while trying to verify candidate authority certificate "minikubeCA")
To see the stack trace of this error execute with --v=5 or higher

    ▪ Generating certificates and keys ...

💣  Error starting cluster: wait: /bin/bash -c "sudo env PATH="/var/lib/minikube/binaries/v1.24.3:$PATH" kubeadm init --config /var/tmp/minikube/kubeadm.yaml  --ignore-preflight-errors=DirAvailable--etc-kubernetes-manifests,DirAvailable--var-lib-minikube,DirAvailable--var-lib-minikube-etcd,FileAvailable--etc-kubernetes-manifests-kube-scheduler.yaml,FileAvailable--etc-kubernetes-manifests-kube-apiserver.yaml,FileAvailable--etc-kubernetes-manifests-kube-controller-manager.yaml,FileAvailable--etc-kubernetes-manifests-etcd.yaml,Port-10250,Swap,Mem,SystemVerification,FileContent--proc-sys-net-bridge-bridge-nf-call-iptables": Process exited with status 1
```
<a name="Ui5iB"></a>
#### 解决方案
参考[https://github.com/kubernetes/minikube/issues/14477](https://github.com/kubernetes/minikube/issues/14477)，通过指定参数`--kubernetes-version=v1.23.8`降低Kubernetes版本成功启动，注意在重启前需要删除原来的旧版本的Kubernetes集群
```bash
$ minikube start --image-repository=registry.cn-hangzhou.aliyuncs.com/google_containers --driver=docker --kubernetes-version=v1.23.8
😄  minikube v1.26.1 on Ubuntu 22.04

🙈  Exiting due to K8S_DOWNGRADE_UNSUPPORTED: Unable to safely downgrade existing Kubernetes v1.24.3 cluster to v1.23.8
💡  Suggestion:

    1) Recreate the cluster with Kubernetes 1.23.8, by running:

    minikube delete
    minikube start --kubernetes-version=v1.23.8

    2) Create a second cluster with Kubernetes 1.23.8, by running:

    minikube start -p minikube2 --kubernetes-version=v1.23.8

    3) Use the existing cluster at version Kubernetes 1.24.3, by running:

    minikube start --kubernetes-version=v1.24.3


$ minikube delete
🔥  Deleting "minikube" in docker ...
🔥  Deleting container "minikube" ...
🔥  Removing /home/fcant/.minikube/machines/minikube ...
💀  Removed all traces of the "minikube" cluster.
```
旧集群删除完成后，再启动指定Kubernetes版本的集群
```bash
$ minikube start --image-repository=registry.cn-hangzhou.aliyuncs.com/google_containers --driver=docker --kubernetes-version=v1.23.8
😄  minikube v1.26.1 on Ubuntu 22.04
✨  Using the docker driver based on user configuration
✅  Using image repository registry.cn-hangzhou.aliyuncs.com/google_containers
📌  Using Docker driver with root privileges
👍  Starting control plane node minikube in cluster minikube
🚜  Pulling base image ...
🔥  Creating docker container (CPUs=2, Memory=2200MB) ...
    > kubeadm.sha256:  64 B / 64 B [-------------------------] 100.00% ? p/s 0s
    > kubelet.sha256:  64 B / 64 B [-------------------------] 100.00% ? p/s 0s
    > kubectl.sha256:  64 B / 64 B [-------------------------] 100.00% ? p/s 0s
    > kubeadm:  43.12 MiB / 43.12 MiB [-------------] 100.00% 5.73 MiB p/s 7.7s
    > kubectl:  44.44 MiB / 44.44 MiB [--------------] 100.00% 3.71 MiB p/s 12s
    > kubelet:  118.78 MiB / 118.78 MiB [------------] 100.00% 6.27 MiB p/s 19s

    ▪ Generating certificates and keys ...
    ▪ Booting up control plane ...
    ▪ Configuring RBAC rules ...
🔎  Verifying Kubernetes components...
    ▪ Using image registry.cn-hangzhou.aliyuncs.com/google_containers/storage-provisioner:v5
🌟  Enabled addons: storage-provisioner, default-storageclass

❗  /usr/local/bin/kubectl is version 1.25.0, which may have incompatibilites with Kubernetes 1.23.8.
    ▪ Want kubectl v1.23.8? Try 'minikube kubectl -- get pods -A'
🏄  Done! kubectl is now configured to use "minikube" cluster and "default" namespace by default
```
<a name="J6LVs"></a>
## 6、启动可视化管理界面Dashboard
```python
$ minikube dashboard --url
🤔  Verifying dashboard health ...
🚀  Launching proxy ...
🤔  Verifying proxy health ...
http://127.0.0.1:44581/api/v1/namespaces/kubernetes-dashboard/services/http:kubernetes-dashboard:/proxy/
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1662622362596-a7067f8b-d814-4adc-8743-3a09c4488d42.png#clientId=ue3d8eb88-f608-4&from=paste&height=152&id=udd804db6&originHeight=379&originWidth=2154&originalType=binary&ratio=1&rotation=0&showTitle=false&size=43914&status=done&style=shadow&taskId=ub1f6c76e-3413-41dd-9592-ffabf8fdcae&title=&width=861.6)<br />启动kube proxy
```python
$ kubectl proxy --port=8888 --address='0.0.0.0' --accept-hosts='^.*'
Starting to serve on [::]:8888
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1662622269497-eab5e543-29d6-4d0c-9a68-96aa47aea681.png#clientId=ue3d8eb88-f608-4&from=paste&height=67&id=u623f86fd&originHeight=167&originWidth=1595&originalType=binary&ratio=1&rotation=0&showTitle=false&size=22532&status=done&style=shadow&taskId=uff1428a6-a14d-46e9-82ce-d5c3054f89e&title=&width=638)<br />然后使用`minikube dashboard --url`命令给出的URL（将端口修改为上面的代理端口8888，如果使用的是虚拟机则需要将IP修改为虚拟机的IP），然后在浏览器中访问即可看到如下管理界面：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1662622395429-d1079cc3-10f1-4310-a52b-b99611e554e6.png#clientId=ue3d8eb88-f608-4&from=paste&height=713&id=ub50e5026&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=219766&status=done&style=shadow&taskId=udaf2f289-29a8-4fe2-8042-dc19e838091&title=&width=1536)
<a name="hSmyR"></a>
## 7、配置tunnel外网访问服务
如果未配置tunnel，EXTERNAL-IP列显示的是pending状态<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1623141083049-4e19f6e1-e39c-448c-b421-74608699ed3b.png#clientId=u30321679-27de-4&from=paste&height=164&id=udc3a06c3&originHeight=491&originWidth=3323&originalType=binary&ratio=3&rotation=0&showTitle=false&size=582667&status=done&style=shadow&taskId=u4b102194-3df7-493f-83cc-88c140c242f&title=&width=1107.6666666666667)<br />由于应用部署在集群内部，集群有自己对应的IP，所以外网无法访问，需要用到tunnel。
```bash
$ minikube tunnel
[sudo] password for fcant:
Status:
        machine: minikube
        pid: 76873
        route: 10.96.0.0/12 -> 192.168.49.2
        minikube: Running
        services: [ingress-nginx]
    errors:
                minikube: no errors
                router: no errors
                loadbalancer emulator: no errors
Status:
        machine: minikube
        pid: 76873
        route: 10.96.0.0/12 -> 192.168.49.2
        minikube: Running
        services: [ingress-nginx]
    errors:
                minikube: no errors
                router: no errors
                loadbalancer emulator: no errors
```
注意该命令执行需要输入当前用户的登录密码，命令执行示例如下：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1623140699710-787a0498-adcc-4461-8049-a813df61390d.png#clientId=u30321679-27de-4&from=paste&height=583&id=u1f5fc0d7&originHeight=1750&originWidth=3323&originalType=binary&ratio=3&rotation=0&showTitle=false&size=2020768&status=done&style=shadow&taskId=ucf375785-3213-4e08-8b3b-d1ccc1c53d6&title=&width=1107.6666666666667)<br />然后再检查EXTERNAL-IP，可以发现已经有正确的IP了，并且使用curl去访问能得到正确的响应。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1623141180108-6105c82a-ef20-463e-82e1-79fc3921bbd0.png#clientId=u30321679-27de-4&from=paste&height=583&id=u74f6f819&originHeight=1750&originWidth=3323&originalType=binary&ratio=3&rotation=0&showTitle=false&size=2414162&status=done&style=shadow&taskId=u96593012-6381-491e-9fb1-74993d4ff34&title=&width=1107.6666666666667)
