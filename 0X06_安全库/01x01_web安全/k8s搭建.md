[https://blog.csdn.net/qq_27745471/article/details/122499655](https://blog.csdn.net/qq_27745471/article/details/122499655)<br />1、解決 k8s是如何搭建起來的<br />2、k8s中的pod是如何部署应用的<br />3、k8s中运维配置有什么可利用的方法


```
kubectl --kubeconfig /etc/kubernetes/kubelet.conf  get all -n kube-system
```
![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1696606173726-13472e10-271f-4431-9178-8430eee13bfd.png#averageHue=%237d7d7c&clientId=u6c0173f3-a074-4&from=paste&height=522&id=u140c0655&originHeight=653&originWidth=1024&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=334936&status=done&style=none&taskId=ue9c90192-aca3-4eb9-9cae-478844bcdbc&title=&width=819.2)

<a name="iPzS8"></a>
## 搭建步骤思路
1、在三台机器上均安装docker、kubeadm、kubelet，在master节点安装kubectl<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1697381293406-97366e33-f01d-4f82-a62b-c0556b6f8b0c.png#averageHue=%23e1a65a&clientId=ufd99743e-99ac-4&from=paste&height=73&id=udaaffef1&originHeight=91&originWidth=866&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=3963&status=done&style=none&taskId=u1644c337-8d0f-4fc5-8d84-bf0e50b5b78&title=&width=692.8)

**这三个工具的作用：**

- **kubeadm是用来创建k8和获取初始化凭证让node加入集群**
- **kubectl 用来列举k8s的信息、pod node啥的 各种信息，apply各种应用等等，例如kubectl apply -f calico.yaml、kubectl get pods -n kube-system**
- **docker k8s的每个节点，master、node创建pod是依靠docker提供的 containerd，例如如下图所示意思，也就是说docker逃逸成为攻击k8的一种手段也是存在的**

![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1697383051193-c3fe4763-116a-4912-9906-224736bcf14d.png#averageHue=%23f3f3f3&clientId=u70e18160-066c-4&from=paste&height=633&id=uf4714c74&originHeight=791&originWidth=1145&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=221413&status=done&style=none&taskId=udb20be90-2e78-4cf4-b71a-bdcfc80dbdf&title=&width=916)<br />docker与k8s的关系解剖<br />[https://zhuanlan.zhihu.com/p/651017500](https://zhuanlan.zhihu.com/p/651017500)

2、k8s初始化和node节点加入(master和node都是主机，pod只是node中启动的docker容器)<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1697383415876-0ff4193b-369b-4116-9880-c991ce32d6c6.png#averageHue=%23fcfcfc&clientId=u70e18160-066c-4&from=paste&height=289&id=u32cdace5&originHeight=361&originWidth=671&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=25209&status=done&style=none&taskId=ua52ee5c7-bbd9-4366-a715-0c753c6a72a&title=&width=536.8)<br />在实际应用中，通常使用Docker来构建和打包应用程序的容器镜像，然后使用Kubernetes来管理和调度这些容器。但是 Docker 并不是 K8s 对容器的唯一选择，K8s 支持对接多种容器  

![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1697468123318-feb2d16f-fe0d-4a76-af72-0ccf4e8fab27.png#averageHue=%23ecedf1&clientId=u65d8a15a-b9ff-4&from=paste&height=529&id=u7a66bc68&originHeight=661&originWidth=993&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=89172&status=done&style=none&taskId=u6e1ca55a-50a3-4f4b-adcb-4a6b6751569&title=&width=794.4)<br />通过gpt得知，k8的container有docker、CRI-O、rkt等等<br />[https://blog.csdn.net/xingzuo_1840/article/details/119579751](https://blog.csdn.net/xingzuo_1840/article/details/119579751)   k8s1.20 搭建-抛弃docker使用containerd

[https://blog.csdn.net/qq_25986067/article/details/122868438?utm_medium=distribute.pc_relevant.none-task-blog-2~default~baidujs_baidulandingword~default-4-122868438-blog-119579751.235^v38^pc_relevant_default_base&spm=1001.2101.3001.4242.3&utm_relevant_index=7](https://blog.csdn.net/qq_25986067/article/details/122868438?utm_medium=distribute.pc_relevant.none-task-blog-2~default~baidujs_baidulandingword~default-4-122868438-blog-119579751.235^v38^pc_relevant_default_base&spm=1001.2101.3001.4242.3&utm_relevant_index=7)   Kubernetes将弃用 Docker替代品是什么


3、部署k8的网络方案<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1697381410735-3a20603c-309a-49ae-ba72-4a7fb4c34685.png#averageHue=%23dcdbda&clientId=ufd99743e-99ac-4&from=paste&height=495&id=u7ac4ae58&originHeight=619&originWidth=1010&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=144683&status=done&style=none&taskId=u9f0de877-7026-46e9-8fcd-636ae77d550&title=&width=808)

<a name="Q6CfJ"></a>
## k8s的攻击思路脑图
[https://www.secpulse.com/archives/186424.html](https://www.secpulse.com/archives/186424.html)<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1697381455856-941396e5-9889-472d-9f58-413584c9bd46.png#averageHue=%23f6e7e0&clientId=ufd99743e-99ac-4&from=paste&height=481&id=u64d109da&originHeight=601&originWidth=886&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=191439&status=done&style=none&taskId=ue5911018-0ed5-41ce-b3a5-635379846fe&title=&width=708.8)

<a name="rz8O8"></a>
## 行动起来，开始搭建！
192.168.112.135 master<br />192.168.112.133 node1<br />192.168.112.134 node2 

- **1、安装好docker和k8s**

执行以下命令，配置好k8s的镜像源和地址
```
#配置k8s的yum源地址
cat <<EOF | sudo tee /etc/yum.repos.d/kubernetes.repo
[kubernetes]
name=Kubernetes
baseurl=http://mirrors.aliyun.com/kubernetes/yum/repos/kubernetes-el7-x86_64
enabled=1
gpgcheck=0
repo_gpgcheck=0
gpgkey=http://mirrors.aliyun.com/kubernetes/yum/doc/yum-key.gpg
   http://mirrors.aliyun.com/kubernetes/yum/doc/rpm-package-key.gpg
EOF
```
![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1697558152970-95466808-be74-47f8-8a5e-896fd61d06c8.png#averageHue=%23fdfdfd&clientId=uf2654a1d-de49-4&from=paste&height=283&id=E5xv3&originHeight=354&originWidth=740&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=53541&status=done&style=none&taskId=u16c2c2fc-c1c6-463b-b7b8-fd4431197b5&title=&width=592)<br />如果上述没配置的话，会出现这个问题<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1697558277561-ea6d4f96-5a81-4e08-968f-2094fa279b27.png#averageHue=%23fefefe&clientId=uf2654a1d-de49-4&from=paste&height=151&id=u61647350&originHeight=189&originWidth=743&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=32931&status=done&style=none&taskId=u3ac0242a-0c8f-47e4-a90a-567fa5f1162&title=&width=594.4)

yum install -y kubelet-1.23.0 kubeadm-1.23.0 kubectl-1.23.0<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1697558220514-5da04a68-a587-4c8a-bb4b-95b661e5cb1c.png#averageHue=%23fefefe&clientId=uf2654a1d-de49-4&from=paste&height=363&id=u65e665a6&originHeight=454&originWidth=795&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=110072&status=done&style=none&taskId=u1548b77c-65d9-4ab4-ac55-da2a51dd5cf&title=&width=636)

- **2、设置好各种的hostname和hosts**

分别设置master node1 node2的hostname
```shell
hostnamectl set-hostname master
hostnamectl set-hostname node1
hostnamectl set-hostname node2
```

每个主机都要设置hosts<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1697559233966-78ec7538-61cc-4863-8d36-11051e681614.png#averageHue=%23fefefe&clientId=uf2654a1d-de49-4&from=paste&height=107&id=u2d1468c7&originHeight=134&originWidth=748&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=17804&status=done&style=none&taskId=u8379df47-182b-49fd-bcb6-c02ac3c8564&title=&width=598.4)

- **3、初始化k8s**

我这里是把192.168.112.130作为master，apiserver-advertise-address值为master的IP、image-repository 值为镜像仓库、kubernetes-version指定k8s的版本、service-cidr指定service的网段、–pod-network-cidr指定pod的网段。<br />在master上执行
```shell
kubeadm init --apiserver-advertise-address=192.168.112.135 --image-repository=registry.cn-hangzhou.aliyuncs.com/lfy_k8s_images --kubernetes-version v1.23.0 --service-cidr=10.10.10.0/24 --pod-network-cidr=10.20.20.0/24 --ignore-preflight-errors=all
```

后来出现了各种报错。。。最后就是一个一个坑搜索百度，逐个逐个解决然后kubeadm reset<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1697641705924-ae3790b6-ec9f-4969-8736-2d7d28225ea4.png#averageHue=%23fefefe&clientId=uf2654a1d-de49-4&from=paste&height=194&id=u8864e39c&originHeight=243&originWidth=608&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=49723&status=done&style=none&taskId=u215157be-acad-4796-846a-733e307775c&title=&width=486.4)

最后重新初始化就成功了。。<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1697641590020-abac6788-47c0-4712-a7a9-b9f1c333bfa5.png#averageHue=%2333393b&clientId=uf2654a1d-de49-4&from=paste&height=319&id=uca9552d5&originHeight=399&originWidth=1044&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=58302&status=done&style=none&taskId=u662a9b02-ab07-4282-9e7d-b41b5ff37f0&title=&width=835.2)
```shell
Your Kubernetes control-plane has initialized successfully!

To start using your cluster, you need to run the following as a regular user:

  mkdir -p $HOME/.kube
  sudo cp -i /etc/kubernetes/admin.conf $HOME/.kube/config
  sudo chown $(id -u):$(id -g) $HOME/.kube/config

Alternatively, if you are the root user, you can run:

  export KUBECONFIG=/etc/kubernetes/admin.conf

You should now deploy a pod network to the cluster.
Run "kubectl apply -f [podnetwork].yaml" with one of the options listed at:
  https://kubernetes.io/docs/concepts/cluster-administration/addons/

Then you can join any number of worker nodes by running the following on each as root:

kubeadm join 192.168.112.135:6443 --token t4b4lk.dxmuujmitu6zeytq \
	--discovery-token-ca-cert-hash sha256:36a4fc6f4bae7cbd55513217538851bf96acbd312578bd5f31fe3a8087a876e9 

```

node节点加入的时候，也是一堆问题。。kubectl<br />systemctl status kubelet 查看的时候，服务启动失败<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1697646352379-572ff64f-56f9-44ae-ab2e-d0251a479609.png#averageHue=%23fefbfb&clientId=uf2654a1d-de49-4&from=paste&height=268&id=ue4129a53&originHeight=335&originWidth=749&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=74640&status=done&style=none&taskId=uda895a85-fe61-4d08-9ad4-96722237e22&title=&width=599.2)

最后是docker源的问题，然后各种操作<br />添加doker源
```shell
[root@master ~]# cat  /etc/docker/daemon.json 
{
  "registry-mirrors": ["https://m1kuyd9u.mirror.aliyuncs.com"],
  "exec-opts": ["native.cgroupdriver=systemd"]
}

```

重启docker、kubelet服务
```shell
sudo systemctl daemon-reload
sudo systemctl restart docker

docker info |grep "Cgroup Driver"
 Cgroup Driver: systemd #表明已经更新为了systemd

 systemctl restart kubelet
```
就成功解决了<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1697647497311-6c470d49-1f4c-47d6-9ee9-dfa49b02c8b2.png#averageHue=%23fefdfd&clientId=uf2654a1d-de49-4&from=paste&height=185&id=u9e768bf7&originHeight=231&originWidth=906&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=41892&status=done&style=none&taskId=uc77108e6-57b0-4127-9102-b0eac7d5eb7&title=&width=724.8)


然后node1节点成功加入集群，就搭建起来了！
```shell
kubeadm join 192.168.112.135:6443 --token t4b4lk.dxmuujmitu6zeytq \
	--discovery-token-ca-cert-hash sha256:36a4fc6f4bae7cbd55513217538851bf96acbd312578bd5f31fe3a8087a876e9 
```
![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1697647784137-6a68ac7b-075b-49fb-a702-066612e03331.png#averageHue=%23fefdfd&clientId=uf2654a1d-de49-4&from=paste&height=265&id=ubcbb9b88&originHeight=331&originWidth=1095&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=61752&status=done&style=none&taskId=u312ed20b-bb1d-418f-8e66-9ce78deb1e6&title=&width=876)

![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1697647840677-b47159f4-12b2-4669-9af3-9d7f6e30d5e1.png#averageHue=%23fefefe&clientId=uf2654a1d-de49-4&from=paste&height=105&id=u0c2e54aa&originHeight=131&originWidth=551&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=21492&status=done&style=none&taskId=uf9301e8e-72bb-4f9b-91f6-6387a0c7a63&title=&width=440.8)

- **配置pod网络中心**

在master中配置就好
```shell
wget https://docs.projectcalico.org/v3.19/manifests/calico.yaml --no-check-certificate
```
下载好后，去掉注释，添加网络中心<br />kubectl apply -f calico.yml后需要等一会，才会运行起来<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1697729631747-6dfcbff4-1b53-41aa-aa45-9de6b98358cb.png#averageHue=%23f9f2f0&clientId=u8363fddc-445f-4&from=paste&height=92&id=u5b8a168d&originHeight=115&originWidth=836&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=16211&status=done&style=none&taskId=u224d2b15-6b4f-443a-b14a-2cbe97155da&title=&width=668.8)

![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1697729573649-2422aee3-64ae-4d5e-8654-561b125c3eb9.png#averageHue=%23fefefe&clientId=u8363fddc-445f-4&from=paste&height=225&id=u224bfc28&originHeight=281&originWidth=875&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=62535&status=done&style=none&taskId=u22d6e316-f6f8-4562-a5ae-a1151979c28&title=&width=700)

可以看到pod的ip是10.20.20.1是之前配置的网络中心的范围<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1697730465148-5e4f32a1-09dd-4210-ba57-2e6174816a23.png#averageHue=%23fdfcfc&clientId=uc997d166-a1e3-4&from=paste&height=299&id=uc6d5cb22&originHeight=374&originWidth=865&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=64224&status=done&style=none&taskId=uc2ab46d4-e5b0-4715-9392-a549f814d84&title=&width=692)

![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1697730687878-c81f8172-482a-43fc-8240-353dd4b2e8db.png#averageHue=%23fefefe&clientId=uc997d166-a1e3-4&from=paste&height=105&id=u40f9617f&originHeight=131&originWidth=558&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=19495&status=done&style=none&taskId=u02d42a94-4739-485b-acf7-a550e8d2d3b&title=&width=446.4)


<a name="oCG9L"></a>
## 安装控制面板
```shell
wget https://raw.githubusercontent.com/kubernetes/dashboard/v2.4.0/aio/deploy/recommended.yaml
```

修改控制面板端口<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1697731015067-d8c4cb82-a148-4d4e-aaba-0fe83aece1eb.png#averageHue=%23f8f6f5&clientId=uc997d166-a1e3-4&from=paste&height=170&id=u91e05559&originHeight=213&originWidth=579&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=16944&status=done&style=none&taskId=ud1791877-5fe0-4d36-95e5-ec82f9e35e4&title=&width=463.2)<br />kubectl apply -f recommended.yaml<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1697731097755-253c37f4-f3aa-4fee-8e7c-45f02c2e9b8d.png#averageHue=%23fefefe&clientId=uc997d166-a1e3-4&from=paste&height=217&id=ua6fd6516&originHeight=271&originWidth=700&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=49267&status=done&style=none&taskId=u0d509f6b-f3df-4481-be59-00b871df95f&title=&width=560)

但是上面的修改好像没啥作用，面板还是访问不了，查看状态节点，都是正在运行的。<br />kubectl get pod -n kubernetes-dashboard -o wide<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1697732744914-611781d4-133a-4218-8eb5-6544209808f1.png#averageHue=%23fefefe&clientId=uc997d166-a1e3-4&from=paste&height=82&id=u9ae2f753&originHeight=103&originWidth=930&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=22265&status=done&style=none&taskId=ud5571045-db9b-4916-96ac-5a1d621b3c2&title=&width=744)

需要进去修改成node节点<br />kubectl edit svc kubernetes-dashboard -n kubernetes-dashboard(也就是可以通过这个方法获取k8的控制面板)<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1697732800109-501f5e24-c82c-42fe-9f60-8c91439143b9.png#averageHue=%23fcfbfb&clientId=uc997d166-a1e3-4&from=paste&height=386&id=u6b45d6fa&originHeight=483&originWidth=510&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=49846&status=done&style=none&taskId=ub2212c7f-c641-4db5-92b3-cfe5d9330ff&title=&width=408)

![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1697732838580-a7fcea2d-1c8e-4e40-b290-3a560ad1d41f.png#averageHue=%23f9f7f7&clientId=uc997d166-a1e3-4&from=paste&height=105&id=u9cb8e8c8&originHeight=131&originWidth=878&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=24329&status=done&style=none&taskId=u12b08090-89fa-4957-9b4a-92ff61b216a&title=&width=702.4)

直接访问面板[https://192.168.112.135:31179/#/login](https://192.168.112.135:31179/#/login)<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1697732702209-2ef7ea98-4321-411f-9648-7c225d863c03.png#averageHue=%23b5e9b5&clientId=uc997d166-a1e3-4&from=paste&height=647&id=uceb2cd2c&originHeight=809&originWidth=1709&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=59160&status=done&style=none&taskId=ud847c0d1-cee0-48d3-8a17-6f1b0fab8ef&title=&width=1367.2)

通过查看admi的token，登陆，直接无敌！直接控制整个k8的面板<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1697733295679-3b857120-3025-4252-9197-f9bc1ffe241a.png#averageHue=%23c5c7c7&clientId=ub5a41ee2-5a35-4&from=paste&height=330&id=u8e6983b6&originHeight=413&originWidth=1269&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=110497&status=done&style=none&taskId=ua7fc7aff-32be-4af0-a678-7eb0bfbc43d&title=&width=1015.2)

![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1697733367379-34474cb6-5556-4726-8b77-789394966bb5.png#averageHue=%23dde6b5&clientId=ub5a41ee2-5a35-4&from=paste&height=704&id=u142ca370&originHeight=880&originWidth=1920&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=155125&status=done&style=none&taskId=ud2a3ee90-3cf4-43f8-a15f-d72efe9df98&title=&width=1536)
```shell
# 创建用户
kubectl create serviceaccount dashboard-admin -n kube-system
# 用户授权
kubectl create clusterrolebinding dashboard-admin --clusterrole=cluster-admin --serviceaccount=kube-system:dashboard-admin
# 获取用户Token
kubectl describe secrets -n kube-system $(kubectl -n kube-system get secret | awk '/dashboard-admin/{print $1}')

kubectl describe pod bash-pod -n default
default是namespace    bash-pod是name
```
<a name="xwDP5"></a>
## kubectl命令
```shell
systemctl status kubelet  查看启动状态
systemctl restart kubelet 重启
journalctl -xefu kubelet 查看kubelet日志

kubectl apply -f xxx.yml
kubectl delete -f xxx.yml

kubectl get pod --all-namespaces

进入容器
kubectl -s 192.168.64.134:8080 --namespace=ability exec -it abilitygateway-54d9f4754d-p2m9s bash

查看映射的端口等服务
 kubectl get svc -n kubernetes-dashboard

查看运行状态
 kubectl get pod -n kubernetes-dashboard -o wide


 kubectl exec -it -n kubernetes-dashboard dashboard-metrics-scraper-799d786dbf-852jc -- /bin/sh

```

<a name="Lvmii"></a>
## 查看k8的apiserver有没有鉴权
查看 API Server 配置文件：API Server 的配置通常位于/etc/kubernetes/manifests/kube-apiserver.yaml<br />**--authorization-mode**通常默认情况下，它应该配置为使用授权模式（如 **RBAC、Node 或 ABAC**）<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1697873223480-a47f359a-8185-4c37-be97-2ece0ff173f3.png#averageHue=%23fefefe&clientId=u812996ec-b281-4&from=paste&height=337&id=u06eab75b&originHeight=421&originWidth=856&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=64122&status=done&style=none&taskId=ufdb4d216-e355-4dba-b5f6-ba311cba1b1&title=&width=684.8)

<a name="Z2T2R"></a>
## 哪些文件可以授权登录k8s
![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1697873822231-35cbd21f-40df-499d-8938-9c5016cf931a.png#averageHue=%23e9ebef&clientId=u812996ec-b281-4&from=paste&height=577&id=u72fc747b&originHeight=721&originWidth=794&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=110355&status=done&style=none&taskId=u2deacdfe-e5f5-4edb-a3bb-456925c076f&title=&width=635.2)
