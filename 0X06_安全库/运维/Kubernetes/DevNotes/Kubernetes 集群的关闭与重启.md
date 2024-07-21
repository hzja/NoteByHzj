Kubernetes
<a name="tH7zW"></a>
## 前言
在日常对 Kubernetes 集群运行维护的过程中，可能需要临时的关闭或者是重启 Kubernetes 集群对集群进行维护，这里介绍如何去安全的关闭 K8s 集群以及如何重新启动集群。
<a name="YAsr6"></a>
## 日常节点运维方式
关闭 K8s 集群是个危险的操作！在关闭集群之前，必须完全了解这个操作所带来的后果。首先，要对集群内的应用、客户定义资源 CRD 和 Etcd 进行备份，然后再进行重启或关闭集群的操作。在通常运维的情况下，建议驱逐维护节点，而非重启整个集群。在这里，也把驱逐维护节点命令放在下面以供参考。<br />首先，确定想要移出的节点的名称。可以用以下命令列出集群中的所有节点:
```bash
$ kubectl get nodes
```
接下来，告诉 Kubernetes 需要移出的节点：
```bash
$ kubectl drain <node name>
```
一旦它返回（没有报错）， 就可以下线此节点（或者等价地在云平台上，删除支持该节点的虚拟机）。如果要在维护操作期间将节点留在集群中，则需要运行：
```bash
kubectl uncordon <node name>
```
然后告诉 Kubernetes，它可以继续在此节点上调度新的 Pods。
<a name="hfonp"></a>
## 在进行集群关闭前的准备工作
备份是最最重要的准备工作，保障应用可以重新正常服务为目的各种操作都是必须的。做一个自己的计划清单，在重要的步骤之前确认好。

- 主机之间已经设置 SSH 免密登录
- 集群内应用的备份
- 集群内客户自定义资源的备份
- 集群内 Etcd 的备份
<a name="ngvfb"></a>
## 关闭 Kubernetes 集群
**再次提示**在关闭集群前，请务必按照推荐的方法备份集群的数据与应用，以便在重新启动集群时如果遇到任何问题，可以通过备份还原集群与应用。使用本教程中的方法可以平稳关闭集群，但数据损坏的可能性仍然存在。<br />首先，要获取节点列表
```bash
k8snodes=$(kubectl get nodes -o name)
```
然后，就要关闭节点了，可以可一台一台的关闭。或运行以下脚本关闭节点:
```bash
for node in ${k8snodes[@]}
do
    echo "==== Shut down $node ===="
    ssh $node sudo shutdown -h 1
done
```
注意: 前提条件是主机之间已经设置 SSH 免密登录。<br />此时，就可以关闭其他的集群依赖项，对集群进行维护操作。
<a name="agHX1"></a>
## Kubernetes 集群重启
在重启后，需要仔细检查所有节点和核心组件的状态，并确保一切就绪。
```bash
$ kubectl get nodes -o wide
NAME        STATUS   ROLES                  AGE   VERSION   INTERNAL-IP    EXTERNAL-IP   OS-IMAGE             KERNEL-VERSION      CONTAINER-RUNTIME
mars-k8s1   Ready    control-plane,master   17d   v1.21.0   172.16.60.60   <none>        Ubuntu 20.04.1 LTS   5.11.0-40-generic   docker://20.10.10
mars-k8s2   Ready    <none>                 17d   v1.21.0   172.16.60.61   <none>        Ubuntu 20.04.1 LTS   5.11.0-40-generic   docker://20.10.10
mars-k8s3   Ready    <none>                 17d   v1.21.0   172.16.60.62   <none>        Ubuntu 20.04.1 LTS   5.11.0-40-generic   docker://20.10.10

$ kubectl get svc -n kube-system
NAME             TYPE        CLUSTER-IP       EXTERNAL-IP   PORT(S)                  AGE
kube-dns         ClusterIP   10.96.0.10       <none>        53/UDP,53/TCP,9153/TCP   17d
metrics-server   ClusterIP   10.111.227.248   <none>        443/TCP                  17d

$ kubectl get pod -n kube-system
NAME                                READY   STATUS    RESTARTS   AGE
coredns-558bd4d5db-h7jqc            1/1     Running   2          17d
coredns-558bd4d5db-wj4bn            1/1     Running   2          17d
etcd-mars-k8s1                      1/1     Running   2          17d
kube-apiserver-mars-k8s1            1/1     Running   3          17d
kube-controller-manager-mars-k8s1   1/1     Running   2          17d
kube-flannel-ds-677dg               1/1     Running   2          17d
kube-flannel-ds-bxhx6               1/1     Running   3          17d
kube-flannel-ds-r5pqf               1/1     Running   2          17d
kube-proxy-6w52h                    1/1     Running   2          17d
kube-proxy-p8zfp                    1/1     Running   2          17d
kube-proxy-v8t7j                    1/1     Running   2          17d
kube-scheduler-mars-k8s1            1/1     Running   2          17d
metrics-server-5f9459b95c-dtzbf     1/1     Running   2          17d
```
<a name="Q5bqe"></a>
## Kubernetes 集群重启维护避坑指南
在这个时代人定胜天的神迹比比皆是。但是所谓割接，割下来，接不上的情况也屡见不鲜。通常情况下，重新启动 Kubernetes 集群后就可以继续正常使用，但是由于意外情况，该集群可能不可用。例如：

- 关闭集群过程中 Etcd 数据损坏或是节点故障，这在 Bare Metal K8s Node 上很常见。
- 网络错误，这就需要检查所有集群依赖项的状态，一定用好监控工具，一步一步的查，别怕麻烦，要确保所有集群依赖项均已就绪。
- 应用的问题，节点是启动了，K8s 也好好的，应用不能对外提供服务，这时一系列的排错是最考验人的，所以备份恢复吧。这样可以确保 RTO。
