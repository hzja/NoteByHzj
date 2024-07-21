Kubernetes<br />![2021-06-16-16-35-56-989949.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1623832701469-27f65dac-4a09-4068-8d45-a094f99820ea.png#clientId=u07a2fcf5-2fbb-4&from=ui&id=uc67ff943&originHeight=181&originWidth=491&originalType=binary&ratio=3&rotation=0&showTitle=false&size=29887&status=done&style=shadow&taskId=u0d020f0b-7e3b-4893-a783-d7460b8911b&title=)
<a name="zAxbO"></a>
### 1、创建yaml模板
```bash
kubectl create deployment web --image=nginx --dry-run -o yaml > web.yaml
```
<a name="GlrNq"></a>
### 2、修改模板
```bash
vim web.yaml
```
```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  labels:
    app: web
  name: web
spec:
  replicas: 3
  selector:
    matchLabels:
      app: web
  template:
    metadata:
      labels:
        app: web
    spec:
      containers:
      - image: zhangfan5391621/java-demo
        name: java
```
<a name="lTQaD"></a>
### 3、创建项目pod
```bash
kubectl apply -f  web.yaml
kubectl logs web-dc77dd8db-hhrxz  #检查日志
```
![2021-06-16-16-35-57-053817.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1623832718791-08cd842c-780e-4176-b7a8-2ff53e1d5502.png#clientId=u07a2fcf5-2fbb-4&from=ui&id=ue4174bcd&originHeight=164&originWidth=596&originalType=binary&ratio=3&rotation=0&showTitle=false&size=5124&status=done&style=none&taskId=u71513843-7eb7-4200-8dd3-04a9c77d0e4&title=)
```yaml
kubectl get deploy  #查看deployment有那些
```
![2021-06-16-16-35-57-119712.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1623832736911-04fa0e20-2da0-465e-8ae2-c519a3e1bed3.png#clientId=u07a2fcf5-2fbb-4&from=ui&id=u214bc709&originHeight=87&originWidth=414&originalType=binary&ratio=3&rotation=0&showTitle=false&size=2706&status=done&style=none&taskId=u1ab550b9-1c93-43c1-a758-cf6d10910dd&title=)
<a name="b91rW"></a>
### 4、创建Service服务
```bash
kubectl expose --name=web deployment web --port=80 --target-port=8080 --type=NodePort
# kubectl  expose +(资源的类型)+(资源的名称)+(service端口)+(后端服务的端口)+类型（Nodeport随机端口）

kubectl get service  #查看service服务的端口
```
![2021-06-16-16-35-57-192525.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1623832756932-4c11b188-9f53-4977-b55f-783db1da89bf.png#clientId=u07a2fcf5-2fbb-4&from=ui&id=u9252b16c&originHeight=142&originWidth=633&originalType=binary&ratio=3&rotation=0&showTitle=false&size=5238&status=done&style=none&taskId=uec753e81-9b0a-46f0-9d0d-994a9f3bbbe&title=)<br />任意一个node节点的ip+30909就能访问该项目
<a name="kqih6"></a>
### 5、升级服务（滚动更新）
```bash
kubectl set image deployment web java=nginx
#kubectl set image+ 资源类型+名字（web）+ (java=nginx)原镜像名=新镜像名
#kubectl edit  service/web      #如果配置文件有改动，直接修改server
http://192.168.106.103:30909/    #能够访问到最新的项目
```
![2021-06-16-16-35-57-290256.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1623832764756-79340708-9ea5-4c09-8a2d-fb3471b69207.png#clientId=u07a2fcf5-2fbb-4&from=ui&id=u050f7ac5&originHeight=364&originWidth=1007&originalType=binary&ratio=3&rotation=0&showTitle=false&size=42671&status=done&style=shadow&taskId=ua391597a-b3b4-4819-bb87-6a3c5f6258b&title=)
<a name="lmwyp"></a>
### 6、回滚项目
```bash
kubectl rollout history deployment web  #查看历史版本
```
![2021-06-16-16-35-57-388113.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1623832779504-d56c7205-61bd-408c-8fcb-45281dd82905.png#clientId=u07a2fcf5-2fbb-4&from=ui&id=ua5730160&originHeight=111&originWidth=529&originalType=binary&ratio=3&rotation=0&showTitle=false&size=3030&status=done&style=none&taskId=uf1fd263f-3a76-4882-b027-b399118c710&title=)
```bash
kubectl rollout undo deployment web   #回滚到上一个版本
修改service/web端口改为8080,即可访问上一个版本的项目，nginx和tomcat是不同的端口
#kubectl rollout undo deployment web --revision=3  ，指定回滚版本
```
![2021-06-16-16-35-57-532805.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1623832787447-884c2fab-7ac9-4c9a-8be3-67a5d217d703.png#clientId=u07a2fcf5-2fbb-4&from=ui&id=ua5dd20b5&originHeight=228&originWidth=1080&originalType=binary&ratio=3&rotation=0&showTitle=false&size=236214&status=done&style=none&taskId=ua68c4dab-9564-4cf1-a56f-3463795009f&title=)
<a name="wW3Yh"></a>
### 7、副本的扩容/缩容
```bash
kubectl scale deployment web --replicas=5 
#将web服务副本扩容成5个(原来三个)
```
![2021-06-16-16-35-57-655515.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1623832795863-4558e971-7dbb-4f64-9818-45c72547308a.png#clientId=u07a2fcf5-2fbb-4&from=ui&id=u9def6ac5&originHeight=193&originWidth=604&originalType=binary&ratio=3&rotation=0&showTitle=false&size=5777&status=done&style=none&taskId=u0b930ce8-4d8e-4a7f-9d7c-d561640d565&title=)
```bash
kubectl scale deployment web --replicas=2
```
![2021-06-16-16-35-57-758513.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1623832804628-a474c66f-1f40-41a6-b6f7-4385a9df6afa.png#clientId=u07a2fcf5-2fbb-4&from=ui&id=ufda6bbbf&originHeight=141&originWidth=578&originalType=binary&ratio=3&rotation=0&showTitle=false&size=5183&status=done&style=none&taskId=u9be5bf30-4dc0-4bb2-bf02-49144c3f033&title=)<br />刚才5个副本已经变成2个副本了
<a name="F8C0h"></a>
### 8、删除service
```bash
kubectl delete deployment/web
kubectl delete svc/web
```
