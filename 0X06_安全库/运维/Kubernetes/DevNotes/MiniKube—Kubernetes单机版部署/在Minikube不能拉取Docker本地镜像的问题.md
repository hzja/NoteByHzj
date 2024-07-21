KubernetesMinikubeDocker
<a name="Unlxb"></a>
## 操作描述
通过Docker构建镜像至本地仓库后，然后编写应用的Kubernetes配置文件，执行`kubectl apply -f blog-service.yml`命令后查看Dashboard发现启动失败，具体报错如下：
<a name="nXLig"></a>
### 报错：Container image "com.fcant/blog:v1" is not present with pull policy of Never
```
Container image "com.fcant/blog:v1" is not present with pull policy of Never
Error: ErrImageNeverPull
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1663059197458-36658b29-89c6-4947-8a01-ea12ebabb9e9.png#clientId=u17429161-fc0d-4&from=paste&height=713&id=u561c52ae&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=537408&status=done&style=shadow&taskId=u34cc2075-3c1d-43a1-b848-584ff8c0a22&title=&width=1536)
<a name="PeOCZ"></a>
## 解决方案
<a name="aZwAI"></a>
### 1、将镜像构建至Minikube镜像仓库中（参考官网第八种解决方案：[https://minikube.sigs.k8s.io/docs/handbook/pushing/](https://minikube.sigs.k8s.io/docs/handbook/pushing/)）
根据[官网描述](https://minikube.sigs.k8s.io/docs/handbook/pushing/)，此时镜像在Docker中，而不在Minikube集群中，所以在Minikube中找不到对应的镜像。<br />[![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1663059510457-c9bcc864-9b78-4121-b2ae-0039b78b987a.png#clientId=u17429161-fc0d-4&from=paste&height=713&id=uc2bae7cf&originHeight=1783&originWidth=3798&originalType=binary&ratio=1&rotation=0&showTitle=false&size=705352&status=done&style=shadow&taskId=ub3f2944d-a314-40ca-a72a-8576b5d050a&title=&width=1519.2)](https://minikube.sigs.k8s.io/docs/handbook/pushing/)<br />查看Minikube Cluster Image，发现确实没有应用镜像
```bash
minikube image ls
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1663059646531-25f556b3-06c4-4328-8c79-9a71583f5681.png#clientId=u17429161-fc0d-4&from=paste&height=309&id=ud23feb30&originHeight=772&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=693049&status=done&style=shadow&taskId=uc69cf9f1-8710-4634-9768-2eec4fc8e3d&title=&width=1329.2)<br />执行Minikube的镜像构建命令，将应用构建至Minikube Cluster Image
```bash
minikube image build -t com.fcant/blog:v1 .
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1663059714137-054fdbe9-d8ed-436e-bf6a-ddfe9dcf701f.png#clientId=u17429161-fc0d-4&from=paste&height=566&id=ud6fd7335&originHeight=1415&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1148695&status=done&style=shadow&taskId=ubfbee88e-1802-4c37-b7be-5caccfe996e&title=&width=1329.2)<br />再次查看Minikube Cluster Image，可以看到对应的应用镜像了。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1663059738977-9bc7b1ed-224e-4415-902a-0a71918b3a90.png#clientId=u17429161-fc0d-4&from=paste&height=330&id=u85eb1d10&originHeight=825&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=740597&status=done&style=shadow&taskId=u07a57f4c-fb7a-4949-9418-c26d49c095f&title=&width=1329.2)<br />到Kubernetes Dashboard中可以看到应用程序已经探测到镜像并启动成功：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1663059822279-b9043a58-80a3-4910-a79f-bab6858bfeee.png#clientId=u17429161-fc0d-4&from=paste&height=713&id=u5ec00681&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=578777&status=done&style=shadow&taskId=ue694b375-babf-4757-8352-7c112f9c8e7&title=&width=1536)<br />查看应用日志也一切正常<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1663059887661-0674b407-7a5e-444b-8d3c-98468bce1999.png#clientId=u17429161-fc0d-4&from=paste&height=713&id=u10c38bb4&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=435758&status=done&style=shadow&taskId=ua73e28bf-967b-42bf-bedd-80f045080d9&title=&width=1536)
