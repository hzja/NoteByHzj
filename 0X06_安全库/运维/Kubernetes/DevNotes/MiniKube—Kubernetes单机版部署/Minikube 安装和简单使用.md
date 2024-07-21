Kubernetes Minikube<br />CentOS7 安装 Minikube，用于学习使用，可以在一台节点上很方便的部署 K8S 集群环境
<a name="uQSyB"></a>
## 一、先决条件
<a name="sOzLu"></a>
### 安装Docker
[Docker安装和启动](https://www.yuque.com/fcant/operation/docker-install?view=doc_embed)
<a name="BAIwH"></a>
### 安装kubelet kubeadm kubectl
[安装升级Kubernetes基础组件kubelet kubeadm kubectl](https://www.yuque.com/fcant/operation/nxptw8?view=doc_embed)
<a name="svp7X"></a>
## 二、安装 Minikube
[https://minikube.sigs.k8s.io/docs/start/](https://minikube.sigs.k8s.io/docs/start/)
```bash
curl -LO https://storage.googleapis.com/minikube/releases/latest/minikube-linux-amd64
sudo install minikube-linux-amd64 /usr/local/bin/minikube
```
<a name="UmWjI"></a>
## 三、启动
```bash
minikube start
```
默认为单节点，可添加节点，[https://minikube.sigs.k8s.io/docs/commands/node](https://minikube.sigs.k8s.io/docs/commands/node)
```bash
minikube node list
minikube node add
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1646225017875-8a6cdb3c-138b-4d94-8cd1-e2255a51a531.png#clientId=u3fe1e18c-2394-4&from=paste&id=ua20ca040&originHeight=429&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5b4224a4-0592-4a93-8711-d7a1e5d5f58&title=)<br />可视化
```bash
minikube dashboard --url # 让其它 IP 可以访问
kubectl proxy --port=8888 --address='0.0.0.0' --accept-hosts='^.*'
```
访问：http://10.74.2.71:8888/api/v1/namespaces/kubernetes-dashboard/services/http:kubernetes-dashboard:/proxy/#/overview?namespace=_all<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1646225017935-0184bd39-6dd6-4f6d-9c81-042cb116b9b3.png#clientId=u3fe1e18c-2394-4&from=paste&id=u96b8a10d&originHeight=525&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u933b077d-5496-4066-aad3-730a43ca802&title=)
<a name="JhflP"></a>
## 四、部署应用与访问应用
```bash
kubectl create deployment nginx --image=nginx
kubectl expose deployment nginx --port=80 --type=NodePort
# 获取访问地址
minikube service --url nginx

# 也可以通过 kubectl proxy 拼接 url 访问，https://kubernetes.io/zh/docs/tasks/access-application-cluster/access-cluster/#manually-constructing-apiserver-proxy-urls
# http://10.74.2.71:8888/api/v1/namespaces/default/services/nginx:80/proxy/
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1646225017920-7fa3d7a6-a55b-402a-8a98-97aa0ea5cea9.png#clientId=u3fe1e18c-2394-4&from=paste&id=ucd1a2d81&originHeight=260&originWidth=610&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6ff65461-23c1-4f52-9fd1-a9abc2695e3&title=)<br />使用负载均衡访问，Minikube 网络：[https://minikube.sigs.k8s.io/docs/handbook/accessing](https://minikube.sigs.k8s.io/docs/handbook/accessing)
```bash
# 新开窗口运行
minikube tunnel --cleanup=true

# 重新部署
kubectl delete deployment nginx
kubectl delete service nginx
kubectl create deployment nginx --image=nginx
kubectl expose deployment nginx --port=80 --type=LoadBalancer
# 查看外部地址
kubectl get svc
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1646225017903-4a1fb90b-514a-4b5e-9bfd-38b0d3adb993.png#clientId=u3fe1e18c-2394-4&from=paste&id=uc0b69bf7&originHeight=728&originWidth=1045&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u69daecf5-1431-4e8e-9aef-3976385e610&title=)<br />通过转发访问，https://kubernetes.io/zh/docs/tasks/access-application-cluster/port-forward-access-application-cluster<br />kubectl port-forward pods/nginx-6799fc88d8-p8llb 8080:80 --address='0.0.0.0'
<a name="UuV1H"></a>
## 五、卸载
[https://minikube.sigs.k8s.io/docs/commands/delete](https://minikube.sigs.k8s.io/docs/commands/delete)
```bash
minikube stop
minikube delete --all
docker rmi kicbase/stable:v0.0.25
rm -rf ~/.kube ~/.minikube
sudo rm -rf /usr/local/bin/kubectl /usr/local/bin/minikube
docker system prune -a
```
