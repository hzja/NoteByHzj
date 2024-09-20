Java SpringCloud 容器化
<a name="rrfO6"></a>
## 微服务项目启动时间长
因为引入了很多Spring Cloud中的组件，所以时间慢了<br />本来是写业务代码的，但是因为要处理很多微服务中的问题，不得已引入Spring Cloud各种组件，从而项目的体量变大了<br />(1）对业务代码的侵入性、耦合性比较强<br />(2）开发者而言，SC时间和精力成本还是比较大的<br />(3)一些互联网公司产品的升级是非常频繁的<br />(4）服务拆分的越细，只是感觉上解耦了，实际上维护的成本缺增加了---->**网络的问题**
<a name="JuwlG"></a>
## 微服务网格（Service Mesh）
prana（Netflix公司）、local proxy（唯品会）、linkerd----->某种意义上都和原有公司的业务强绑定，并不具备通用性
<a name="F1MDz"></a>
## 容器化部署

1. 节约机器资源
2. 方便服务迁移
<a name="yD5Aw"></a>
### istio
<a name="p5uVC"></a>
## 对于微服务的落地上线
(1) 容器化<br />(2) 容器编排：K8S<br />(3) Service Mesh<br />(4) Spring Cloud
<a name="FmUie"></a>
## Docker
<a name="HcHkK"></a>
### Docker构建jar项目镜像
<a name="F93hr"></a>
#### 1、将项目打包成jar
<a name="pHbQC"></a>
#### 2、编写DockerFile
```dockerfile
FROM openjdk:8
COPY pay-service-0.0.1-SNAPSHOT.jar pay-service.jar
CMD ["java", "-jar", "pay-service.jar"]
```
<a name="vUMAj"></a>
#### 3、执行Docker构建镜像命令
:::info
该命令默认使用当前目录下的DockerFile
:::
```bash
docker build -t pay-service-image .
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630831713330-9f844e8a-98f6-4898-9ef8-7d7e81b3178c.png#clientId=uae0a4302-a2f7-4&from=paste&height=271&id=u9f9a0ae9&originHeight=814&originWidth=2076&originalType=binary&ratio=1&rotation=0&showTitle=false&size=762620&status=done&style=none&taskId=u9a0917f7-9ba3-4ae8-bd6e-51363dc9d32&title=&width=692)
<a name="liSCR"></a>
#### 4、创建对应的镜像容器并启动
```bash
docker run -d --name payservice01 -p 9090:8080 pay-service-image
```
<a name="mhFRG"></a>
#### 5、将本地镜像发布至阿里云镜像仓库
在本地登录注册的阿里云镜像仓库
```bash
$ sudo docker login --username=fcs****@gmail.com registry.cn-beijing.aliyuncs.com
```
将镜像推送到Registry
```bash
$ docker login --username=fcs****@gmail.com registry.cn-beijing.aliyuncs.com
$ docker tag [ImageId] registry.cn-beijing.aliyuncs.com/fcant/save:[镜像版本号]
$ docker push registry.cn-beijing.aliyuncs.com/fcant/save:[镜像版本号]
```
从Registry中拉取镜像
```bash
$ docker pull registry.cn-beijing.aliyuncs.com/fcant/save:[镜像版本号]
```
<a name="DCQu2"></a>
## Docker集群的管理
<a name="K4hfC"></a>
### Docker swarm
<a name="SuPtT"></a>
### Mesos 
<a name="PQQVi"></a>
### Kubernetes
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630833234526-5485fa56-8f0f-473e-af24-72f949b57430.png#clientId=uae0a4302-a2f7-4&from=paste&height=500&id=u8166a18d&originHeight=1501&originWidth=2946&originalType=binary&ratio=1&rotation=0&showTitle=false&size=739781&status=done&style=shadow&taskId=u07674772-ae1a-4e22-bae2-2f71c1aa864&title=&width=982)<br />但是在K8s世界中，它不会直接操作container，而是会操作最小单元PodPod可以理解为是由一个或者多个container的组合。
<a name="Ol42b"></a>
#### K3s：轻量级 Kubernetes
[https://k3s.io/](https://k3s.io/)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631087688003-55e2fc7a-b1c9-4901-9fcf-8a4ab4682973.png#clientId=u5f549c6d-58a7-4&from=paste&height=600&id=u60e0470f&originHeight=1799&originWidth=3798&originalType=binary&ratio=1&rotation=0&showTitle=false&size=293919&status=done&style=shadow&taskId=u281e8484-b262-42b1-858e-9776efac976&title=&width=1266)<br />K3s的工作原理<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631087797731-5feddb81-3ef0-40b9-829b-bb2064175ecb.png#clientId=ue1365603-2883-4&from=paste&height=534&id=ud08a5ee9&originHeight=1603&originWidth=2850&originalType=binary&ratio=1&rotation=0&showTitle=false&size=251400&status=done&style=none&taskId=uc183c09e-2c21-4d15-837e-ba0878870ed&title=&width=950)
<a name="FTvLV"></a>
#### MiniKube：搭建单机版的K8s集群的小工具
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631087761048-4a563a47-2908-4c2a-8f08-93c0e6e960f0.png#clientId=u5f549c6d-58a7-4&from=paste&height=601&id=u20ee42d9&originHeight=1803&originWidth=3798&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1291862&status=done&style=shadow&taskId=uc3ac25da-f289-4daa-b03f-752dbb4225b&title=&width=1266)
<a name="CkELk"></a>
#### Kubeadm：搭建本地的多个节点的K8s集群
<a name="KB2Mx"></a>
### Kubernetes上发布应用的流程
<a name="DRSxS"></a>
#### 1、编写yaml文件
```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
	name: pay-service
spec:
	selector:
    matchLabels:
    	app: pay-service
  replicas: 2
  template:
    metadata:
      labels:
        app: pay-service
    spec:
      containers:
      - name: pay-service
        image: registry.cn-hangzhou.aliyuncs.com/fcant-springcloud/pay-service-image
        ports:
        	- containerPort: 8080
---
apiVersion: v1
kind: Service
metadata:
	name: pay-service
spec:
  ports:
  - port: 80
    protocol: TCP
    targetPort: 8080
  selector:
  	app: pay-service
  type: NodePort
```
<a name="ZArHR"></a>
#### 2、应用该yaml文件
```bash
kubectl apply -f pay-service.yaml
```
<a name="LAmjh"></a>
#### 3、查看创建的Pods
```bash
kubectl get pods -w
```
<a name="eevhx"></a>
#### 4、查看服务的情况
```bash
kubectl get svc
```
<a name="mbu30"></a>
#### 5、进行服务的扩缩容
```bash
kubectl scale deploy pay-service --replicas=10
```
<a name="XkiOw"></a>
### Kubernetes Web管理控制台Dashboard部署查看
<a name="XieHl"></a>
#### 1、使用官方提供的yaml文件创建Pod
[https://github.com/kubernetes/dashboard/blob/master/aio/deploy/recommended.yaml](https://github.com/kubernetes/dashboard/blob/master/aio/deploy/recommended.yaml)
<a name="JBjWC"></a>
#### 2、应用yaml配置
```yaml
kubectl apply -f recommended.yaml
```
<a name="EQHUK"></a>
#### 3、查看创建的Pods
```bash
kubectl get pods -n kubernetes-dashboard
```
<a name="BDlt7"></a>
#### 4、查看服务的详情
```bash
kubectl get svc -n kubernetes-dashboard
```
<a name="Mdwzc"></a>
#### 5、查看登录Dashboard Token的命令
```bash
kubectl describe secret -n kube-system ${ADMIN_SECRET} | grep -E '^token' | awk '{print $2}'
```
<a name="EiPtS"></a>
### MiniKube中启动Dashboard
```bash
minikube dashboard
```
