Kubernetes
<a name="bkVIV"></a>
### Node节点默认的Pod数量
Kubernetes Node节点每个默认允许最多创建110个Pod，有时可能会由于系统硬件的问题，从而需要控制Node节点的Pod的运行数量。<br />即：需要调整Node节点的最大可运行Pod数量。<br />一般来说，只需要在kubelet启动命令中增加`–max-pods`参数，然后，重启kubelet 服务，就生效。<br />重启kubelet，不影响现有运行中的容器，不会造成容器重启。
<a name="g52TR"></a>
### 修改限制Pod启动数量
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640530695232-874c40c2-d078-4f20-9769-3dee336a0ac9.webp#clientId=uf70c84f6-6ef6-4&from=paste&id=u1c9dde63&originHeight=367&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u395d6a79-c5dd-438f-8b0c-7e7ac06b2d7&title=)<br />可以看到，集群当前有3台Node节点，且最大可运行Pod数量均为默认值：110。<br />接下来修改该参数值。
<a name="H8XoY"></a>
#### 1、登录 Node 节点，查看kubelet启动文件路径
```bash
[root@VM-2-8-centos ~]# systemctl status kubelet
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640530695255-bbcf18bd-4a39-4878-b1e6-c5416857a695.webp#clientId=uf70c84f6-6ef6-4&from=paste&id=u02605a5f&originHeight=340&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5f0d13ba-8be8-4c4b-91e4-15fef8c5d48&title=)<br />从上图可以看到，kubelet 的启动文件为/usr/lib/systemd/system/kubelet.service
<a name="jT4TS"></a>
#### 2、查看 systemd 启动文件
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640530695309-d469f7ce-e0b6-4324-a8cb-5dc5952f5915.webp#clientId=uf70c84f6-6ef6-4&from=paste&id=u89388cbb&originHeight=286&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9d8caf98-5bca-440a-90de-dfbc3d44196&title=)<br />可以看到，Environment文件是引用/etc/kubernetes/kubelet，至此，只需要在/etc/kubernetes/kubelet中增加`max-pods`的变量，然后在/usr/lib/systemd/system/kubelet.service中，将变量添加到启动参数后即可。
<a name="EsZXV"></a>
#### 3、修改/etc/kubernetes/kubelet
强烈建议，在修改Pod数量的时候，如果是缩减（当前运行70个Pod，所见到20个），强烈建议先把Node上的Pod驱散，等驱散完了在重启，最后在激活Node。
```bash
[root@VM-2-8-centos ~]# vim /etc/kubernetes/kubelet  
# 增加  
MAX_PODS="--max-pods=61"
```
<a name="eFhPz"></a>
#### 4、修改/usr/lib/systemd/system/kubelet.service
注意：必须在启动命令后加入该变量，才可以使刚才定义的--max-pods=61生效。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640530695413-c0b379ae-434a-448e-8a98-150b33f7330f.webp#clientId=uf70c84f6-6ef6-4&from=paste&id=u24cd433b&originHeight=244&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=udc81d81f-567d-4fb9-a4d1-3406df99930&title=)
<a name="VYTHy"></a>
#### 5、重启kubelet
```bash
[root@VM-2-8-centos ~]# systemctl  daemon-reload  
[root@VM-2-8-centos ~]# systemctl restart kubelet
```
查看修改结果<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640530696332-4a5389d2-ab29-4b06-a516-06a6485f9c33.webp#clientId=uf70c84f6-6ef6-4&from=paste&id=ubb1e4db5&originHeight=365&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2e8e8663-c354-4df9-8d7b-bf310b41f7f&title=)<br />可以看到，已经完成修改的节点的最大可调度Pod数量已经调整为61。配置生效。
<a name="TOirQ"></a>
### 新建监控视图
在这里，可以部署Prometheus + Grafana，配置监控视图，更好的体现集群中节点的 Pod 分配率。<br />PromQL：
```bash
sum(kubelet_running_po\_count{node=~"$node"}) by (cluster, node) / sum(kube_node_status_allocatable_pods{node=~"$node"}) by (cluster, node)
```
效果展示：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640530695687-ff2b2af8-d013-430f-971a-e8b2f5c5d1bc.webp#clientId=uf70c84f6-6ef6-4&from=paste&id=u4e71bfee&originHeight=421&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud84ea251-b359-4e94-b4bb-8a82ec83197&title=)

 
