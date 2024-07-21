Ceph
<a name="qvrOK"></a>
## 一、概述
Ceph 在 k8s 中用做共享存储还是非常方便的，Ceph 是比较老牌的分布式存储系统，非常成熟，功能也强大，支持三种模式（块存储、文件系统存储、对象存储），所以接下来就详细讲解如何在 k8s 使用 ceph。<br />前提是需要一个 k8s 环境。
<a name="TVew4"></a>
## 二、Ceph Rook 介绍
_Rook是一个开源的**云原生存储编排工具**，_提供平台、框架和对各种存储解决方案的支持，以和云原生环境进行本地集成。

- Rook 将存储软件转变成自我管理、自我扩展和自我修复的存储服务，通过自动化部署、启动、配置、供应、扩展、升级、迁移、灾难恢复、监控和资源管理来实现。Rook 底层使用云原生容器管理、调度和编排平台提供的能力来提供这些功能。
- Rook 利用扩展功能将其深度地集成到云原生环境中，并为调度、生命周期管理、资源管理、安全性、监控等提供了无缝的体验。有关 Rook 当前支持的存储解决方案的状态相关的更多详细信息，可以参考 Rook 仓库 的项目介绍。Rook 目前支持 Ceph、NFS、Minio Object Store 和 CockroachDB。

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1673449119642-a43d25fe-33a6-4086-a5a5-21c5523d0606.png#averageHue=%23d8d69e&clientId=u77ec8be8-2c35-4&from=paste&id=ua811198a&originHeight=546&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5466e1e5-78a3-4cec-a516-6828b9492fa&title=)<br />官网：https://rook.io/<br />项目地址：https://github.com/rook
<a name="b3T1M"></a>
## 三、通过 Rook 在 k8s 中部署 Ceph
官方文档: https://rook.io/docs/rook/v1.10/Getting-Started/quickstart/<br />_【温馨提示】k8s 节点各挂载一块（或者多块）20GB 的未使用的磁盘。_
<a name="Cnmes"></a>
### 1）下载部署包
```bash
git clone --single-branch --branch v1.10.8 https://github.com/rook/rook.git
```
部署所用到的镜像如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1673449119626-bf3cb311-d0bb-4de7-b8c8-bc5a66a9d4db.png#averageHue=%23262321&clientId=u77ec8be8-2c35-4&from=paste&id=u0b5458c4&originHeight=275&originWidth=861&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6536cdc1-42f4-48d5-a54a-bbef8576e3e&title=)<br />由于镜像源在国外，国内无法下载，这里需要修改一些镜像或者提前下载 tag，操作如下：
```bash
cd rook/deploy/examples/

#(registry.aliyuncs.com/google_containers/<image>:<tag>)，后四个镜像我FQ下
docker pull registry.aliyuncs.com/google_containers/csi-node-driver-registrar:v2.5.1

docker tag registry.aliyuncs.com/google_containers/csi-node-driver-registrar:v2.5.1 registry.k8s.io/sig-storage/csi-node-driver-registrar:v2.5.1

docker pull registry.aliyuncs.com/google_containers/csi-snapshotter:v6.1.0
docker tag registry.aliyuncs.com/google_containers/csi-snapshotter:v6.1.0 registry.k8s.io/sig-storage/csi-snapshotter:v6.1.0

docker pull registry.aliyuncs.com/google_containers/csi-attacher:v4.0.0
docker tag registry.aliyuncs.com/google_containers/csi-attacher:v4.0.0 registry.k8s.io/sig-storage/csi-attacher:v4.0.0

docker pull registry.aliyuncs.com/google_containers/csi-resizer:v1.6.0
docker tag registry.aliyuncs.com/google_containers/csi-resizer:v1.6.0 registry.k8s.io/sig-storage/csi-resizer:v1.6.0

docker pull registry.aliyuncs.com/google_containers/csi-resizer:v1.6.0
docker tag registry.aliyuncs.com/google_containers/csi-resizer:v1.6.0 registry.k8s.io/sig-storage/csi-resizer:v1.6.0

docker pull registry.aliyuncs.com/google_containers/csi-provisioner:v3.3.0
docker tag registry.aliyuncs.com/google_containers/csi-provisioner:v3.3.0 registry.k8s.io/sig-storage/csi-provisioner:v3.3.0
```
<a name="n18a7"></a>
### 2）部署 Rook Operator
```bash
cd rook/deploy/examples
kubectl create -f crds.yaml -f common.yaml -f operator.yaml
# 检查
kubectl -n rook-ceph get pod
```
也可以通过 helm 部署
```bash
helm repo add rook-release https://charts.rook.io/release
helm install --create-namespace --namespace rook-ceph rook-ceph rook-release/rook-ceph -f values.yaml
```
<a name="og1fK"></a>
### 3）创建 Rook Ceph 集群
现在 Rook Operator 处于 Running 状态，接下来就可以创建 Ceph 集群了。为了使集群在重启后不受影响，请确保设置的 dataDirHostPath 属性值为有效得主机路径。
```bash
cd rook/deploy/examples
kubectl apply -f cluster.yaml
```
<a name="r1BWU"></a>
### 4）部署 Rook Ceph 工具
```bash
cd rook/deploy/examples
kubectl create -f toolbox.yaml
```
<a name="wOINL"></a>
### 5）部署 Ceph Dashboard
```bash
cd rook/deploy/examples
kubectl apply -f dashboard-external-https.yaml

# 获取 dashboard admin密码
kubectl -n rook-ceph get secret rook-ceph-dashboard-password -o jsonpath="{['data']['password']}" | base64 -d
```
通过 Ceph Dashboard 查看 Ceph 集群状态
```bash
# 查看对外端口
kubectl get svc -n rook-ceph
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1673449119570-a554d613-b9f3-448a-ae16-e3b3b5015dbc.png#averageHue=%23252321&clientId=u77ec8be8-2c35-4&from=paste&id=u92df1e24&originHeight=207&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u02d7aeec-c0a1-4cbb-8cdb-574a3561428&title=)<br />https://<nodeip>:nodePort/<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1673449119579-e1d50eb2-af0e-4ea1-b098-2fd83dc14659.png#averageHue=%23afd7a7&clientId=u77ec8be8-2c35-4&from=paste&id=u8e168ec3&originHeight=553&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u75b063e9-5ebc-4ec3-b032-50fea9aa2a8&title=)
<a name="FtUwS"></a>
### 6）检查
```bash
kubectl get pods,svc -n rook-ceph
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1673449119879-ce1118dc-21e1-4d4d-b21c-e40c87c8c755.png#averageHue=%23262422&clientId=u77ec8be8-2c35-4&from=paste&id=u67823e63&originHeight=672&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc1d3f418-5248-4ce4-8590-eb6b855b2d4&title=)
<a name="XL4wC"></a>
### 7）通过 ceph-tool 工具 pod 查看 ceph 集群状态
```bash
kubectl exec -it `kubectl get pods -n rook-ceph|grep rook-ceph-tools|awk '{print $1}'` -n rook-ceph -- bash

ceph -s
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1673449120118-adc9bbc1-a381-4a3d-b4e0-d9b8e16a34e0.png#averageHue=%231f1e1d&clientId=u77ec8be8-2c35-4&from=paste&id=u040114b2&originHeight=418&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=udb42caeb-f0c3-402f-a334-8cb60adab37&title=)
<a name="CIiTY"></a>
## 四、 测试验证
<a name="wfYEf"></a>
### 1） 块存储（RBD）测试
<a name="DwtL6"></a>
#### 1、创建 StorageClass
```bash
cd rook/deploy/examples
# 创建一个名为replicapool的rbd pool
kubectl apply -f csi/rbd/storageclass.yaml
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1673449120069-5902bcde-106e-410b-beda-8dcc4a6f0163.png#averageHue=%23232220&clientId=u77ec8be8-2c35-4&from=paste&id=u2e019d3a&originHeight=179&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua5adb7bb-12a8-4a72-bacd-93159760797&title=)
<a name="SeGYR"></a>
#### 2、部署 WordPress
```bash
kubectl apply -f mysql.yaml
kubectl apply -f wordpress.yaml
```
<a name="IvEiC"></a>
### 2）文件系统 （CephFS） 测试
<a name="j01mg"></a>
#### 1、创建 StorageClass
```bash
kubectl apply -f csi/cephfs/storageclass.yaml
```
<a name="xEiVA"></a>
#### 2、部署应用
```bash
kubectl apply -f filesystem.yaml
```
<a name="UMIJz"></a>
### 3）对象存储 （RGW） 测试
<a name="Ti0Ty"></a>
#### 1、创建对象存储
```bash
kubectl create -f object.yaml

# 验证rgw pod正常运行
kubectl -n rook-ceph get pod -l app=rook-ceph-rgw
```
<a name="IU2Ra"></a>
#### 2、创建对象存储 user
```bash
kubectl create -f object-user.yaml
```
<a name="PYb3J"></a>
#### 3、获取 accesskey secretkey
```bash
# 获取AccessKey
kubectl -n rook-ceph get secret rook-ceph-object-user-my-store-my-user -o yaml | grep AccessKey | awk '{print $2}' | base64 --decode

# 获取 SecretKey
kubectl -n rook-ceph get secret rook-ceph-object-user-my-store-my-user -o yaml | grep SecretKey | awk '{print $2}' | base64 --decode
```
<a name="PBz0H"></a>
#### 4、部署 rgw nodeport
```bash
kubectl apply -f rgw-external.yaml

kubectl -n rook-ceph get service rook-ceph-rgw-my-store rook-ceph-rgw-my-store-external
```
<a name="wfw95"></a>
#### 5、通过 api 接口使用 Ceph 对象存储
```bash
#首先，需要安装 python-boto 包，用于测试连接 S3。：
yum install python-boto -y

# 然后，编写 python 测试脚本。
# cat s3.py
```
```python
#!/usr/bin/python

import boto
import boto.s3.connection
access_key = 'C7492VVSL8O11NZBK3GT'
secret_key = 'lo8IIwMfmow4fjkSOMbjebmgjzTRBQSO7w83SvBd'
conn = boto.connect_s3(
    aws_access_key_id = access_key,
    aws_secret_access_key = secret_key,
    host = '192.168.182.110', port=30369,
    is_secure=False,
    calling_format = boto.s3.connection.OrdinaryCallingFormat(),
)
bucket = conn.create_bucket('my-first-s3-bucket')
for bucket in conn.get_all_buckets():
        print "{name}\t{created}".format(
                name = bucket.name,
                created = bucket.creation_date,
)
```
