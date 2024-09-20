<a name="oS605"></a>
## 本地Kubernetes部署架构
![](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1632240405064-5a754354-1656-46a7-8d1e-1078c4df0c6a.jpeg)
<a name="OphgP"></a>
## 校验Kubernetes安装
<a name="we1YH"></a>
### 1、启动minikube作为Kubernetes Server端
```bash
minikube start
```
<a name="aR7DU"></a>
### 2、校验Kubernetes版本
```bash
$ kubectl version
Client Version: version.Info{Major:"1", Minor:"21", GitVersion:"v1.21.1", GitCommit:"5e58841cce77d4bc13713ad2b91fa0d961e69192", GitTreeState:"clean", BuildDate:"2021-05-12T14:18:45Z", GoVersion:"go1.16.4", Compiler:"gc", Platform:"linux/amd64"}
Server Version: version.Info{Major:"1", Minor:"20", GitVersion:"v1.20.2", GitCommit:"faecb196815e248d3ecfb03c680a4507229c2a56", GitTreeState:"clean", BuildDate:"2021-01-13T13:20:00Z", GoVersion:"go1.15.5", Compiler:"gc", Platform:"linux/amd64"}
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632324702459-5c1cbc23-2c00-4421-b328-0f11fc76decf.png#clientId=u899f68a8-836c-4&from=paste&height=162&id=ucc4a5630&originHeight=485&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=575891&status=done&style=none&taskId=uf23272ee-7261-4a20-884f-37a49a197bc&title=&width=1107.6666666666667)
<a name="BYgRx"></a>
### 3、`kubectl config current-context`
```bash
$ kubectl config current-context
minikube
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632324960681-4735ff7b-1059-41d3-ae23-944b1ce5ac28.png#clientId=u899f68a8-836c-4&from=paste&height=72&id=ucf5918d8&originHeight=216&originWidth=1939&originalType=binary&ratio=1&rotation=0&showTitle=false&size=108465&status=done&style=none&taskId=u97d218cc-cdd1-4681-9b79-6c588581a9a&title=&width=646.3333333333334)
<a name="ezd4j"></a>
### 4、检查集群信息
```bash
$ kubectl cluster-info
Kubernetes control plane is running at https://192.168.49.2:8443
KubeDNS is running at https://192.168.49.2:8443/api/v1/namespaces/kube-system/services/kube-dns:dns/proxy

To further debug and diagnose cluster problems, use 'kubectl cluster-info dump'.
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632324997787-54314e39-9aa3-4986-aa68-7d25913f6109.png#clientId=u899f68a8-836c-4&from=paste&height=128&id=uff21c5db&originHeight=384&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=423713&status=done&style=none&taskId=u0f60a749-7ca0-4314-8024-7df5648535f&title=&width=1107.6666666666667)
<a name="PwHGq"></a>
### 5、检查节点信息
```bash
$ kubectl get nodes
NAME       STATUS   ROLES                  AGE   VERSION
minikube   Ready    control-plane,master   16d   v1.20.2
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632325013989-66a227fe-c038-49d7-b92d-d81e39f54452.png#clientId=u899f68a8-836c-4&from=paste&height=91&id=u4e581a88&originHeight=274&originWidth=1839&originalType=binary&ratio=1&rotation=0&showTitle=false&size=152848&status=done&style=none&taskId=u6e9e6724-940a-49ea-97ee-5ded84607eb&title=&width=613)
<a name="cCAoV"></a>
## 安装和访问Kubernetes Dashboard
<a name="KNfzU"></a>
### 1、安装Kubernetes Dashboard
[https://github.com/kubernetes/dashboard](https://github.com/kubernetes/dashboard)
```bash
kubectl apply -f https://raw.githubusercontent.com/kubernetes/dashboard/v2.3.1/aio/deploy/recommended.yaml
```
:::danger
如果未部署成功并且多次执行了apply命令，可能会出现如下报错：<br />`The ClusterRoleBinding "kubernetes-dashboard" is invalid: roleRef: Invalid value: rbac.RoleRef{APIGroup:"rbac.authorization.k8s.io", Kind:"ClusterRole", Name:"kubernetes-dashboard"}: cannot change roleRef`<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632362225454-7f28f2d7-3924-4aef-8f7c-f4212437e1f9.png#clientId=uffb02077-d29c-4&from=paste&height=344&id=uf948c052&originHeight=1031&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1261282&status=done&style=none&taskId=ub7c661db-6506-4a71-805f-b691e0c34a7&title=&width=1107.6666666666667)<br />原因是已经存在重复的ClusterRoleBinding，使用命令`kubectl delete clusterrolebinding kubernetes-dashboard`删除重复的绑定即可。<br />然后再执行该命令则不会出现问题<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632362373783-74f15606-3862-46f3-a0dc-2c90b37e5093.png#clientId=uffb02077-d29c-4&from=paste&height=287&id=u96b5a1ee&originHeight=860&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1105046&status=done&style=none&taskId=uda3f4495-7e93-43cc-b442-867f55f0b86&title=&width=1107.6666666666667)
:::
```java
kubectl get pods --namespace=kubernetes-dashboard
```
:::tips
注意：这里的命令行参数`--namespace`后面的值要改为对应的YAML文件里面的Namespace的值<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632361720683-967df67c-a6d9-4640-95dd-9d8ec41019ca.png#clientId=uffb02077-d29c-4&from=paste&height=508&id=DL9W2&originHeight=1524&originWidth=1792&originalType=binary&ratio=1&rotation=0&showTitle=false&size=154607&status=done&style=none&taskId=u5e2cd926-e9fb-4d8f-8fa9-6f40c46ecac&title=&width=597.3333333333334)
:::
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632361869639-7e5512d8-f03a-4772-9a29-d35677833a40.png#clientId=uffb02077-d29c-4&from=paste&height=234&id=ua5012548&originHeight=703&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=820967&status=done&style=none&taskId=u7b6ae7fe-1045-473f-9a82-1561b7f3792&title=&width=1107.6666666666667)
<a name="h635Z"></a>
### 2、启动Kube Proxy
```bash
kubectl proxy
# 如果kubernetes部署在服务器通过公网IP访问则使用下面的命令，否则默认绑定的是127.0.0.1
kubectl proxy --address='0.0.0.0'
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632361916246-896ad296-57b3-4b8a-b849-cd06e7c8b4c4.png#clientId=uffb02077-d29c-4&from=paste&height=71&id=ue5bb1d22&originHeight=214&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=188004&status=done&style=none&taskId=ub1232b23-3fc0-4305-bf14-7272133e542&title=&width=1107.6666666666667)
<a name="uVclz"></a>
### 3、生成访问令牌
```bash
kubectl -n kubernetes-dashboard describe secret $(kubectl -n kubernetes-dashboard get secret | grep admin-user | awk '{print $1}')
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632361978286-2e1c563d-7a3d-4e86-8338-001e8360abb2.png#clientId=uffb02077-d29c-4&from=paste&height=583&id=ue725279d&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2206638&status=done&style=none&taskId=u01d6296c-968f-4887-bd83-47c867a5706&title=&width=1107.6666666666667)
<a name="iaM7P"></a>
### 4、访问Dashboard
http://localhost:8001/api/v1/namespaces/kubernetes-dashboard/services/https:kubernetes-dashboard:/proxy/<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1663168018358-c20bcd89-cd9a-465f-aeb8-1f8010274712.png#clientId=ue46387f4-c2cc-4&from=paste&height=1189&id=u0d792f76&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=221140&status=done&style=shadow&taskId=u0a97f9da-30fa-4fbf-a2b4-ec9a34ec21c&title=&width=2560)
<a name="R3B7u"></a>
## Staffjoy部署到本地Kubernetes环境
<a name="FgW09"></a>
### 1、镜像构建（可选）
```bash
mvn clean package -DskipTests
docker-compose build
docker images
```
[服务容器化和Docker compose部署](https://www.yuque.com/fcant/java/bhd6gv?view=doc_embed&inner=RobPq)
<a name="k0lL0"></a>
### 2、部署MySQL数据库（可选）

- staffjoy_account
- staffjoy_company
<a name="INYmc"></a>
#### 授予ip访问权限
```bash
[root@iZuf68qk0bio1l786kylamZ ~]# docker exec -it mysql bash
root@c4c2e8d188de:/# mysql -uroot -p
Enter password:
Welcome to the MySQL monitor.  Commands end with ; or \g.
Your MySQL connection id is 39
Server version: 8.0.26 MySQL Community Server - GPL

Copyright (c) 2000, 2021, Oracle and/or its affiliates.

Oracle is a registered trademark of Oracle Corporation and/or its
affiliates. Other names may be trademarks of their respective
owners.

Type 'help;' or '\h' for help. Type '\c' to clear the current input statement.

mysql> use mysql;
Reading table information for completion of table and column names
You can turn off this feature to get a quicker startup with -A

Database changed
mysql> select user, host from user;
+------------------+-----------+
| user             | host      |
+------------------+-----------+
| root             | %         |
| mysql.infoschema | localhost |
| mysql.session    | localhost |
| mysql.sys        | localhost |
| root             | localhost |
+------------------+-----------+
5 rows in set (0.00 sec)

mysql> grant all privileges on *.* to root@127.0.0.1 identified by '123456' with grant options;
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632370673875-48b4f565-91ef-4c78-af27-ce6cd831bebc.png#clientId=u77fbdc3a-4907-4&from=paste&height=600&id=u9bef36f7&originHeight=1801&originWidth=2259&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1405341&status=done&style=shadow&taskId=u05d90b3f-6dc2-4527-8113-36573abf6c4&title=&width=753)
<a name="sp0ra"></a>
### 3、部署Staffjoy（K8s/test）
```bash
kubectl apply -f config.yml
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632375854612-328328d7-0964-444f-8fdc-7dc465fb2360.png#clientId=u5c625c78-9e84-4&from=paste&height=147&id=uef58a32a&originHeight=440&originWidth=1962&originalType=binary&ratio=1&rotation=0&showTitle=false&size=287666&status=done&style=none&taskId=ua1946662-d087-4815-960d-61f6b12dd91&title=&width=654)
```sql
# 指定test目录下的所有yaml文件
kubectl apply -f test
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632377040497-610c6c39-f9b0-49e6-8c37-5cbc6cec96b1.png#clientId=u5c625c78-9e84-4&from=paste&height=398&id=u11b8ae5a&originHeight=1195&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1459564&status=done&style=none&taskId=u7ee43125-37f2-45fa-9562-3ae47a4b1f0&title=&width=1107.6666666666667)
<a name="LL8FF"></a>
### 4、端口转发
```bash
# 查询faraday pod名
kubectl get pods
kubectl port-forward faraday-svc-deployment-8584d9c74d-v92wt 80:80
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632377426603-6a3bebf2-b8a6-4d28-8e64-1f57d81652ce.png#clientId=u5c625c78-9e84-4&from=paste&height=506&id=ued7f979a&originHeight=1517&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1890596&status=done&style=none&taskId=ucc31f868-09a2-4586-8771-80095e2de36&title=&width=1107.6666666666667)
<a name="uTF7O"></a>
### 5、启动SwitchHosts
<a name="e3xfp"></a>
### 6、命令行校验
```bash
kubectl get pods -o wide
kubectl get services
kubectl get deployments
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632377198798-41984973-b00b-4044-8a7c-8420a7653a69.png#clientId=u5c625c78-9e84-4&from=paste&height=687&id=uDL6C&originHeight=2060&originWidth=3836&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2934266&status=done&style=none&taskId=u41048e96-edf2-4a8a-973a-24294f498e5&title=&width=1278.6666666666667)
<a name="IQWeM"></a>
### 7、K8s Dashboard校验
<a name="orla9"></a>
### 8、Staffjoy校验
<a name="uFcWK"></a>
### 9、清理
```bash
kubectl delete deployment --all
kubectl delete service --all
kubectl delete configmaps --all
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632377664551-33e5b5d5-0d12-4c26-a9f3-09912784aaf7.png#clientId=u5c625c78-9e84-4&from=paste&height=486&id=u8378a6c2&originHeight=1457&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1715854&status=done&style=none&taskId=ud116ae58-69d5-4657-99b5-4286d6829b6&title=&width=1107.6666666666667)
<a name="gEHKm"></a>
## Staffjoy部署到阿里云Kubernetes架构
![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1662976333125-1c5e4665-c494-4561-aacc-eb35c89a93b8.jpeg)
<a name="xWqFD"></a>
### 阿里云Kubernetes拓扑结构
[https://developer.aliyun.com/article/88526](https://developer.aliyun.com/article/88526)<br />![fbf3d81e025b172496c49eb688d302414efac1ba.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632378304679-61983f42-f2a2-4e9a-9968-9ba1e9a6210a.png#clientId=u60188531-96ec-4&from=ui&id=u855cfe1b&originHeight=492&originWidth=828&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1224677&status=done&style=shadow&taskId=u7f61998f-6f3d-486c-aba0-06d5fca2432&title=)
<a name="AQ3MC"></a>
### 阿里云Kubernetes环境创建
<a name="wHnBI"></a>
#### 1、创建VPC
<a name="SE8dA"></a>
#### 2、创建RDS数据库
加IP白名单<br />更新JDBC连接字符串<br />创建ROOT帐号<br />创建数据库和表
<a name="KP2MH"></a>
#### 3、创建共享版Kubernetes集群
<a name="ZCwYa"></a>
#### 4、校验
```bash
kubectl config current-context
kubectl cluster-info
kubectl get nodes
```
<a name="UXKde"></a>
### Staffjoy部署
<a name="aRQEK"></a>
#### 1、部署Staffjoy（Kubernetes/UAT）
```bash
kubectl apply -f config
kubectl apply -f uat
```
<a name="hnZUg"></a>
#### 2、阿里云Dashboard校验
<a name="kkrx8"></a>
#### 3、命令行校验
```bash
kubectl get pods -o wide
kubectl get services
kubectl get deployments
```
<a name="FvH82"></a>
#### 4、启用SwitchHosts
更新ELB IP地址
<a name="cxoYq"></a>
#### 5、Staffjoy校验
<a name="xT41W"></a>
#### 6、删除阿里云Kubernetes和RDS
