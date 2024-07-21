<a name="N146M"></a>
### 获取节点相应服务的信息：`kubectl get nodes`
```bash
kubectl get pods
如下详细信息：
kubectl get pods -n beta # -n :指定命名空间
kubectl get pods --all-namespaces #获取所有命名空间的pod
kubectl get pods --selector name=redis #按selector名来查找pod
kubectl get pods -o wide #查看pods所在的运行节点
kubectl get pods -o yaml #查看pods定义的详细信息
kubectl get nodes –lzone #获取zone的节点
```
<a name="j2gYi"></a>
### 查看集群信息、指定的资源详细描述信息
```bash
kubectl cluster-info
kubectl describe service/kubernetes-dashboard --namespace=”kube-system”
kubectl describe pods/kubernetes-dashboard-xxx --namespace=“kuber-system”
kubectl describe pod nginx-77ai
```
<a name="XRPXW"></a>
### 查看运行的pod的环境变量
```bash
kubectl exec pod名 env
```
<a name="EEsvz"></a>
### 查看指定的pod的日志
```bash
kubectl logs -f pods/xxxx –n kube-system
例如 ：
kubectl logs -f beta -n wallet-beta
```
<a name="lBlYa"></a>
### 创建、重建、删除、加载资源
```bash
kubectl create -f 文件名.yaml #创建资源
kubectl replace -f 文件名 [–force] #重建资源
```
<a name="GVfg4"></a>
### 删除资源
```bash
kubectl delete -f 文件名
kubectl delete pod pod名
kubectl delete rc rc名
kubectl delete service service名
kubectl delete pods --all
```
<a name="W40pg"></a>
### 加载配置文件
```bash
kubectl apply –f 文件名.yaml
kubectl scale 动态伸缩：
kubectl scale rc nginx --replicas=5
kubectl scale deployment redis=slave --replicas=5
kubectl scale --replicas-=2 -f redis-slave-deployment.yaml
kubectl exec:进入pod启动的容器
kubectl exec -it redis-master-xxxx /bin/bash -n beta #指定命名空间
kubectl label :添加label值
kubectl get node --show-labels #查看节点label
kubectl label nodes node1 zone=north #nodes添加标签
kubectl label pod redis-master-xxx role=master #增加label值 [key]=[value]
kubectl label pod redis-master-xxx role- #删除label值
kubectl label pod redis-master-xxx role=backend --overwrite #修改label值
kubectl rolling-update：滚动升级
kubectl rolling-update redis-master -f redis-master-controller-v2.yaml #配置文件滚动升级
kubectl rolling-update redis-master --image=redis-master:2.0 #命令升级
kubectl rolling-update redis-master --image=redis-master:1.0 --rollback #pod版本回滚
```
<a name="RmnmB"></a>
### etcdctl 常用操作
```bash
etcdctl cluster-health #检查网络集群健康状态
etcdctl --endpoints=https://192.168.71.221:2379 cluster-health #带有安全认证检查网络集群健康状态
etcdctl member list
etcdctl set /k8s/network/config ‘{ “Network”: “10.1.0.0/16” }’
etcdctl get /k8s/network/config
```
