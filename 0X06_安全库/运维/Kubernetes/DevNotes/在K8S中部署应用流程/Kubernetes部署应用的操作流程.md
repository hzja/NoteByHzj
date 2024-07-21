Kubernetes
<a name="KryJv"></a>
### 制作镜像
制作镜像分为三步：

- 第一步基础镜像，是基于哪个操作系统，比如CentOS 7或者其他的
- 第二步中间件镜像，比如服务镜像，跑的像Nginx服务，Tomcat服务
- 第三步项目镜像，它是服务镜像之上的，将项目打包进去，那么这个项目就能在这个服务镜像里面运行了

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640577863746-4b535169-ead5-4274-866c-e5a641945529.webp#clientId=u32030b46-41e6-4&from=paste&id=uc2687787&originHeight=165&originWidth=343&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3b65d2c4-2e83-445d-aa2a-1d1efb539ff&title=)<br />一般运维人员都是提前将镜像做好，而开发人员就能直接拿这个镜像去用，这个镜像一定要符合现在环境部署的环境。
<a name="dTNGP"></a>
### 控制器管理Pod
也就是Kubernetes去部署这个镜像了，一般都会去拿控制器去部署，用的最多的就是Deployment。

- Deployment：无状态部署
- StatefulSet：有状态部署
- DaemonSet：守护进程部署
- Job & CronJob：批处理

无状态和有状态的有什么区别？<br />有状态的是有身份的，比如网络ID、存储，这两个是提前规划好的，有序启动/停止。
<a name="RTRdY"></a>
### Pod数据持久化
Pod数据持久化主要是应对一个应用程序说的，比如开发一个项目，这个项目有没有落地到本地文件，如果有落的话，就保证他持久的有了，那就必须要用到Pod数据的持久化了。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640577863959-eb452e47-2397-4600-9c41-876334c4004f.webp#clientId=u32030b46-41e6-4&from=paste&id=u96d6cd39&originHeight=221&originWidth=375&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=udc20c04a-b537-4cbb-82a2-b29046b7643&title=)<br />容器部署过程中一般有以下三种数据：

- 启动时需要的初始数据，可以是配置文件
- 启动过程中产生的临时数据，该临时数据需要多个容器间共享
- 启动过程中产生的持久化数据
<a name="S41eh"></a>
### 暴露应用
在Kubernetes中，部署一个Deployment是无法对外进行访问的，就是别的应用程序要想访问部署的Deployment，找不到该怎么去访问，为什么这么讲，因为Deployment一般都是多副本部署的，有可能会分布在不同的节点之上，而且重建Pod IP也会变，重新发布一下也会改变，所以没有办法去固定去访问哪个Pod，即使固定了，其他的Pod也访问不了，要想做到多个Pod都去提供服务，前面就必须要加一个负载均衡，提供一个访问入口，只有访问这个统一入口，才能转发到后端多个Pod上，只要访问这个Cluster IP就能转发到后端的Pod上。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640577863879-5c7d7626-1378-4648-b78e-54b2c11226e0.webp#clientId=u32030b46-41e6-4&from=paste&id=uf1973b1b&originHeight=189&originWidth=457&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc24ef5d4-00fe-409a-b2e1-9e14eba0799&title=)
<a name="avW2y"></a>
#### Service

- Service定义了Pod的逻辑集合和访问这个集合的策略
- Service的引入为解决Pod的动态变化，提供了服务发现和负载均衡
- 使用CoreDNS解析Service名称
<a name="CiSTE"></a>
### 对外发布应用
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640577863741-f664a6f0-86e9-4e83-a0ab-0212e95f70b9.webp#clientId=u32030b46-41e6-4&from=paste&id=u14d104e3&originHeight=391&originWidth=490&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4e1a7220-3b22-470b-98c8-f57c89d11f6&title=)<br />暴露出去之后，也就是需要让用户去访问，比如搭建一个电商网站，让用户去访问，Ingress相对于Service，是一个互补的状态，Service主要提供了集群内部的访问，也可以暴露一个TCP/UDP的端口，而Ingress主要是一个7层的转发，也就是提供一个统一的入口，只要访问Ingress Controller，就能转发部署所有的项目，也就是所有的项目都使用域名去访问。
<a name="Q2BdD"></a>
#### 传统部署与Kubernetes部署的区别
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640577863739-ad945b5e-8838-4ce9-9535-f291a38ff404.webp#clientId=u32030b46-41e6-4&from=paste&id=u97e7f263&originHeight=388&originWidth=692&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u85c4b128-8721-42e8-829a-dc99342cf61&title=)<br />传统部署首先开发者将代码部署到代码仓库中，主流是使用Git或者GitLab，提交完代码通过CI/CD平台需要对代码进行拉取、编译、构建，产生一个War包，交给Ansible，然后发送到云主机上/物理机，之后通过负载均衡将项目暴露出去，最后会有数据库，监控系统，日志系统来提供相关的服务。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640577864413-398f045f-1ef8-45ab-b8c7-69b9c084c03c.webp#clientId=u32030b46-41e6-4&from=paste&id=u232cc79c&originHeight=379&originWidth=692&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u63e3292b-2092-4660-99dc-95c933c167a&title=)<br />Kubernetes部署首先也是开发将代码放在代码仓库，然后通过Jenkins去完成拉取代码，编译，上传到镜像仓库，这里是将代码打包成一个镜像，而不是刻意执行的war或者jar包，这个镜像包含了项目的运行环境和项目代码，这个镜像可以放在任何Docker上去run起来，都可以去访问，首先得保证能够在Docker上去部署起来，再部署到Kubernetes上，打出来的镜像去放在镜像仓库中，来集中管理这些镜像。<br />因为每天会产生几十或者上百个镜像，所以必须通过镜像仓库去管理，这里可能会去写一个脚本去连接Kubernetes Master，而Kubernetes会根据自己的部署去调度这些Pod，然后通过Ingress去发布应用，让用户去访问。每个Ingress会关联一组Pod，而Service会创建这组Pod的负载均衡，通过Service去区分这些节点上的Pod，数据库放在集群之外，监控系统、日志系统也可以放在Kubernetes集群去部署，也可以放在集群之外。放在Kubernetes集群内的，也不是特别敏感，主要用来运维和开发调试用，不会影响到业务，所以优先去Kubernetes中部署。<br />现在去部署一个Java项目到Kubernetes中。
<a name="kZnDV"></a>
#### 1、安装OpenJDK
```bash
[root@k8s-master ~]# yum -y install java-1.8.0-openjdk.x86_64 maven
[root@k8s-master ~]# java -version
openjdk version "1.8.0_222"
OpenJDK Runtime Environment (build 1.8.0_222-b10)
OpenJDK 64-Bit Server VM (build 25.222-b10, mixed mode)
```
然后将代码拉到本地一般Dockerfile中，跟代码都放在同一目录下。
```bash
[root@k8s-master tomcat-java-demo-master]# ls
db  Dockerfile  LICENSE  pom.xml  README.md  src
[root@k8s-master tomcat-java-demo-master]# vim Dockerfile 
FROM lizhenliang/tomcat
LABEL maintainer zhaochengcheng
RUN rm -rf /usr/local/tomcat/webapps/*
ADD target/*.war /usr/local/tomcat/webapps/ROOT.war
```
<a name="f95fk"></a>
#### 2、使用Maven进行编译
这里需要配置Maven的国内源，这样比较快一点。
```bash
[root@k8s-master CI]# vim /etc/maven/settings.xml
 <mirror>
      <id>central</id>
      <mirrorOf>central</mirrorOf>
      <name>aliyun maven</name>
      <url>https://maven.aliyun.com/repository/public</url>
    </mirror>
  </mirrors>
[root@k8s-master tomcat-java-demo-master]# mvn clean package -D maven test.skip=true
[root@k8s-master tomcat-java-demo-master]# ls
db  Dockerfile  LICENSE  pom.xml  README.md  src  target
[root@k8s-master tomcat-java-demo-master]# cd target/
[root@k8s-master target]# ls
classes  generated-sources  ly-simple-tomcat-0.0.1-SNAPSHOT  ly-simple-tomcat-0.0.1-SNAPSHOT.war  maven-archiver  maven-status
[root@k8s-master tomcat-java-demo-master]# cd target/
```
就使用这个编译好的war包，打成镜像，上传到Harbor仓库里。
```bash
[root@k8s-master target]# ls
classes            ly-simple-tomcat-0.0.1-SNAPSHOT      maven-archiver
generated-sources  ly-simple-tomcat-0.0.1-SNAPSHOT.war  maven-status

[root@k8s-master tomcat-java-demo-master]# docker build -t 192.168.30.24/library/java-demo:latest .
```
<a name="fBYD8"></a>
#### 3、上传到镜像仓库
```bash
[root@k8s-master tomcat-java-demo-master]# docker login 192.168.30.24
Username: admin
Password: 
Error response from daemon: Get https://192.168.30.24/v2/: dial tcp 192.168.30.24:443: connect: connection refused
```
这里报错，其实需要在每台Docker下都写入对Harbor仓库的信任才可以，后面上传镜像也会用。
```bash
[root@k8s-master java-demo]# vim /etc/docker/daemon.json
{
        "registry-mirrors": ["http://f1361db2.m.daocloud.io"],
        "insecure-registries": ["192.168.30.24"]
}
```
再等录一下push就可以了。
```bash
[root@k8s-master tomcat-java-demo-master]# docker push 192.168.30.24/library/java-demo:latest
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640577864334-58b721da-57db-4977-8e46-7516926c436a.webp#clientId=u32030b46-41e6-4&from=paste&id=u6ed070e9&originHeight=307&originWidth=693&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u57d1d5d8-1f7d-4a1b-83a5-588700be599&title=)
<a name="dSktP"></a>
#### 4、控制器管理Pod
编写Deployment，一般项目都写到自定义的命名空间下，名称写项目名称，方便记忆：
```bash
name: tomcat-java-demo
namespace: test
```
另外就是下一个项目名称，这里分为多个，一般由很多的组件组成，所以下面可以写个APP的名称，比如组件1、2、3，起码标签有这两个维度：

- project：www
- app：java-demo

另外就是镜像拉取，在哪个仓库去下载，这里建议镜像仓库的项目名称和定义的是一种，避免混了。<br />重新打个标签，并传到私有镜像仓库中。
```bash
[root@k8s-master java-demo]# docker tag 192.168.30.24/library/java-demo  192.168.30.24/tomcat-java-demo/java-demo  
[root@k8s-master java-demo]# docker push 192.168.30.24/tomcat-java-demo/java-demo:latest
```
镜像地址也改一下。
```yaml
imagePullSecrets:
  - name: registry-pull-secret
  containers:
  - name: tomcat
  image: 192.168.30.24/tomcat-java-demo/java-demo:latest
```
现在开始创建yaml。<br />创建项目的命名空间。
```bash
[root@k8s-master java-demo]# vim namespace.yaml 
apiVersion: v1
kind: Namespace
metadata:
  name: test

[root@k8s-master java-demo]# kubectl create -f namespace.yaml 
namespace/test created
[root@k8s-master java-demo]# kubectl get ns
NAME              STATUS   AGE
default           Active   22h
kube-node-lease   Active   22h
kube-public       Active   22h
kube-system       Active   22h
test              Active   5s
```
创建一个secret来保证Harbor镜像仓库的认证信息，这里一定要写上项目的命名空间。
```bash
[root@k8s-master java-demo]# kubectl create secret docker-registry registry-pull-secret --docker-username=admin --docker-password=Harbor12345 --docker-email=111@qq.com --docker-server=192.168.30.24 -n test
secret/registry-pull-secret created
[root@k8s-master java-demo]# kubectl get ns
NAME              STATUS   AGE
default           Active   23h
kube-node-lease   Active   23h
kube-public       Active   23h
kube-system       Active   23h
test              Active   6m39s
[root@k8s-master java-demo]# kubectl get secret
NAME                   TYPE                                  DATA   AGE
default-token-2vtgm    kubernetes.io/service-account-token   3      23h
registry-pull-secret   kubernetes.io/dockerconfigjson        1      46s

[root@k8s-master java-demo]# vim deployment.yaml 
apiVersion: apps/v1beta1
kind: Deployment
metadata:
  name: tomcat-java-demo
  namespace: test
spec:
  replicas: 3
  selector:
    matchLabels:
      project: www
      app: java-demo
  template:
    metadata:
      labels:
        project: www
        app: java-demo
    spec:
      imagePullSecrets:
      - name: registry-pull-secret
      containers:
      - name: tomcat
        image: 192.168.30.24/tomcat-java-demo/java-demo:latest
        imagePullPolicy: Always
        ports:
        - containerPort: 8080
          name: web
          protocol: TCP
        resources:
          requests:
cpu: 0.5
            memory: 1Gi
          limits:
            cpu: 1
            memory: 2Gi
        livenessProbe:
          httpGet:
            path: /
            port: 8080
          initialDelaySeconds: 60
          timeoutSeconds: 20
        readinessProbe:
          httpGet:
            path: /
            port: 8080
          initialDelaySeconds: 60
          timeoutSeconds: 20

[root@k8s-master java-demo]# kubectl get pod -n test
NAME                                READY   STATUS    RESTARTS   AGE
tomcat-java-demo-6d798c6996-fjjvk   1/1     Running   0          2m58s
tomcat-java-demo-6d798c6996-lbklf   1/1     Running   0          2m58s
tomcat-java-demo-6d798c6996-strth   1/1     Running   0          2m58s
```
另外就是暴露一个Service，这里的标签也要保持一致，不然找不到相应的标签就提供不了服务，这里应该使用Ingress来访问发布，直接使用ClusterIP就可以。
```bash
[root@k8s-master java-demo]# vim service.yaml 
apiVersion: v1
kind: Service
metadata:
  name: tomcat-java-demo
  namespace: test
spec:
  selector:
    project: www
    app: java-demo
  ports:
  - name: web
    port: 80
    targetPort: 8080

[root@k8s-master java-demo]# kubectl get pod,svc -n test
NAME                                    READY   STATUS    RESTARTS   AGE
pod/tomcat-java-demo-6d798c6996-fjjvk   1/1     Running   0          37m
pod/tomcat-java-demo-6d798c6996-lbklf   1/1     Running   0          37m
pod/tomcat-java-demo-6d798c6996-strth   1/1     Running   0          37m

NAME                       TYPE        CLUSTER-IP     EXTERNAL-IP   PORT(S)   AGE
service/tomcat-java-demo   ClusterIP   10.1.175.191   <none>        80/TCP    19s
```
测试访问项目，是可以的，现在要通过ingress发布出去。
```bash
[root@k8s-master java-demo]# curl 10.1.175.191
<!DOCTYPE html>
<html>
<head lang="en">
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <title>把美女带回家应用案例</title>
    <meta name="description" content="把美女带回家应用案例">
    <meta name="keywords" content="index">
```
现在部署一个ingress-nginx的控制器，这个网上都可以找到，官方也有，这里是按DaemonSet的方式去部署的，所以每个节点都会跑一个控制器。
```bash
[root@k8s-master java-demo]# kubectl get pod -n ingress-nginx
NAME                             READY   STATUS    RESTARTS   AGE
nginx-ingress-controller-g95pp   1/1     Running   0          3m6s
nginx-ingress-controller-wq6l6   1/1     Running   0          3m6s
```
<a name="h490v"></a>
#### 发布应用
这里注意两点，一个是网站域名，一个是Service的命名空间。
```bash
[root@k8s-master java-demo]# kubectl get pod,svc -n test
NAME                                    READY   STATUS    RESTARTS   AGE
pod/tomcat-java-demo-6d798c6996-fjjvk   1/1     Running   0          53m
pod/tomcat-java-demo-6d798c6996-lbklf   1/1     Running   0          53m
pod/tomcat-java-demo-6d798c6996-strth   1/1     Running   0          53m

NAME                       TYPE        CLUSTER-IP     EXTERNAL-IP   PORT(S)   AGE
service/tomcat-java-demo   ClusterIP   10.1.175.191   <none>        80/TCP    16m
[root@k8s-master java-demo]# vim service.yaml 
[root@k8s-master java-demo]# kubectl create -f ingress.yaml 
apiVersion: extensions/v1beta1
kind: Ingress
metadata:
  name: tomcat-java-demo
  namespace: test
spec:
  rules:
    - host: java.maidikebi.com
      http:
        paths:
        - path: /
          backend:
            serviceName: tomcat-java-demo
            servicePort: 80
```
这边是测试的，所以绑定本地的hosts来进行访问。在hosts文件里面加入域名和节点IP就能访问到项目了。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640577864294-30b69cd1-4ac6-4dda-b042-f6ce9a9983d8.webp#clientId=u32030b46-41e6-4&from=paste&id=u4a340f39&originHeight=797&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3cb69383-33d8-43bd-aae9-de5ae630544&title=)
