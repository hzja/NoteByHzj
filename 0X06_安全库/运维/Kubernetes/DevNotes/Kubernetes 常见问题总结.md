Kubernetes
<a name="PBKI4"></a>
### 删除不一致状态下的 rc，deployment，service
在某些情况下，经常发现 kubectl 进程挂起现象，然后在 get 时候发现删了一半，而另外的删除不了
```bash
[root@k8s-master ~]# kubectl get -f fluentd-elasticsearch/
NAME DESIRED CURRENT READY AGE
rc/elasticsearch-logging-v1 0 2 2 15h
NAME DESIRED CURRENT UP-TO-DATE AVAILABLE AGE
deploy/kibana-logging 0 1 1 1 15h
Error from server (NotFound): services "elasticsearch-logging" not found
Error from server (NotFound): daemonsets.extensions "fluentd-es-v1.22" not found
Error from server (NotFound): services "kibana-logging" not found
```
删除这些 deployment，service 或者 rc 命令如下:
```bash
kubectl delete deployment kibana-logging -n kube-system --cascade=false
kubectl delete deployment kibana-logging -n kube-system  --ignore-not-found
delete rc elasticsearch-logging-v1 -n kube-system --force now --grace-period=0
1|2删除不了后如何重置etcd
```
<a name="0VhWF"></a>
### 删除不了后如何重置 etcd
```bash
rm -rf /var/lib/etcd/*
```
删除后重新 reboot master 结点。<br />reset etcd 后需要重新设置网络
```bash
etcdctl mk /atomic.io/network/config '{ "Network": "192.168.0.0/16" }'
```
<a name="UsOf8"></a>
### 启动 apiserver 失败
每次启动都是报如下问题：
```bash
start request repeated too quickly for kube-apiserver.service
```
但其实不是启动频率问题，需要查看/var/log/messages，这里的情况中是因为开启    ServiceAccount 后找不到 ca.crt 等文件，导致启动出错。
```
May 21 07:56:41 k8s-master kube-apiserver: Flag --port has been deprecated, see --insecure-port instead.
May 21 07:56:41 k8s-master kube-apiserver: F0521 07:56:41.692480 4299 universal_validation.go:104] Validate server run options failed: unable to load client CA file: open /var/run/kubernetes/ca.crt: no such file or directory
May 21 07:56:41 k8s-master systemd: kube-apiserver.service: main process exited, code=exited, status=255/n/a
May 21 07:56:41 k8s-master systemd: Failed to start Kubernetes API Server.
May 21 07:56:41 k8s-master systemd: Unit kube-apiserver.service entered failed state.
May 21 07:56:41 k8s-master systemd: kube-apiserver.service failed.
May 21 07:56:41 k8s-master systemd: kube-apiserver.service holdoff time over, scheduling restart.
May 21 07:56:41 k8s-master systemd: start request repeated too quickly for kube-apiserver.service
May 21 07:56:41 k8s-master systemd: Failed to start Kubernetes API Server.
```
在部署 fluentd 等日志组件的时候，很多问题都是因为需要开启 ServiceAccount 选项需要配置安全导致，所以说到底还是需要配置好 ServiceAccount.
<a name="TW8yN"></a>
### 出现 Permission denied 情况
在配置 fluentd 时候出现cannot create /var/log/fluentd.log: Permission denied 错误，这是因为没有关掉 SElinux 安全导致。
:::tips
可以在 /etc/selinux/config 中将 `SELINUX=enforcing` 设置成 disabled,然后 reboot
:::
<a name="OzlXy"></a>
### 基于 ServiceAccount 的配置
首先生成各种需要的 keys，k8s-master 需替换成 master 的主机名.
```bash
openssl genrsa -out ca.key 2048
openssl req -x509 -new -nodes -key ca.key -subj "/CN=k8s-master" -days 10000 -out ca.crt
openssl genrsa -out server.key 2048
echo subjectAltName=IP:10.254.0.1 > extfile.cnf
#ip由下述命令决定
#kubectl get services --all-namespaces |grep 'default'|grep 'kubernetes'|grep '443'|awk '{print $3}'
openssl req -new -key server.key -subj "/CN=k8s-master" -out server.csr
openssl x509 -req -in server.csr -CA ca.crt -CAkey ca.key -CAcreateserial -extfile extfile.cnf -out server.crt -days 10000
```
如果修改 /etc/kubernetes/apiserver 的配置文件参数的话，通过 systemctl start kube-apiserver 启动失败，出错信息为:
```
Validate server run options failed: unable to load client CA file: open /root/keys/ca.crt: permission denied
```
但可以通过命令行启动 API Server
```bash
/usr/bin/kube-apiserver --logtostderr=true --v=0 --etcd-servers=http://k8s-master:2379 --address=0.0.0.0 --port=8080 --kubelet-port=10250 --allow-privileged=true --service-cluster-ip-range=10.254.0.0/16 --admission-control=ServiceAccount --insecure-bind-address=0.0.0.0 --client-ca-file=/root/keys/ca.crt --tls-cert-file=/root/keys/server.crt --tls-private-key-file=/root/keys/server.key --basic-auth-file=/root/keys/basic_auth.csv --secure-port=443 &>> /var/log/kubernetes/kube-apiserver.log &
```
命令行启动 Controller-manager
```bash
/usr/bin/kube-controller-manager --logtostderr=true --v=0 --master=http://k8s-master:8080 --root-ca-file=/root/keys/ca.crt --service-account-private-key-file=/root/keys/server.key & >>/var/log/kubernetes/kube-controller-manage.log
```
<a name="H78ns"></a>
### ETCD 启动不起来-问题<1>
etcd是kubernetes 集群的zookeeper进程，几乎所有的service都依赖于etcd的启动，比如flanneld,apiserver,docker.....在启动etcd是报错日志如下：
```
May 24 13:39:09 k8s-master systemd: Stopped Flanneld overlay address etcd agent.
May 24 13:39:28 k8s-master systemd: Starting Etcd Server...
May 24 13:39:28 k8s-master etcd: recognized and used environment variable ETCD_ADVERTISE_CLIENT_URLS=http://etcd:2379,http://etcd:4001
May 24 13:39:28 k8s-master etcd: recognized environment variable ETCD_NAME, but unused: shadowed by corresponding flag
May 24 13:39:28 k8s-master etcd: recognized environment variable ETCD_DATA_DIR, but unused: shadowed by corresponding flag
May 24 13:39:28 k8s-master etcd: recognized environment variable ETCD_LISTEN_CLIENT_URLS, but unused: shadowed by corresponding flag
May 24 13:39:28 k8s-master etcd: etcd Version: 3.1.3
May 24 13:39:28 k8s-master etcd: Git SHA: 21fdcc6
May 24 13:39:28 k8s-master etcd: Go Version: go1.7.4
May 24 13:39:28 k8s-master etcd: Go OS/Arch: linux/amd64
May 24 13:39:28 k8s-master etcd: setting maximum number of CPUs to 1, total number of available CPUs is 1
May 24 13:39:28 k8s-master etcd: the server is already initialized as member before, starting as etcd member...
May 24 13:39:28 k8s-master etcd: listening for peers on http://localhost:2380
May 24 13:39:28 k8s-master etcd: listening for client requests on 0.0.0.0:2379
May 24 13:39:28 k8s-master etcd: listening for client requests on 0.0.0.0:4001
May 24 13:39:28 k8s-master etcd: recovered store from snapshot at index 140014
May 24 13:39:28 k8s-master etcd: name = master
May 24 13:39:28 k8s-master etcd: data dir = /var/lib/etcd/default.etcd
May 24 13:39:28 k8s-master etcd: member dir = /var/lib/etcd/default.etcd/member
May 24 13:39:28 k8s-master etcd: heartbeat = 100ms
May 24 13:39:28 k8s-master etcd: election = 1000ms
May 24 13:39:28 k8s-master etcd: snapshot count = 10000
May 24 13:39:28 k8s-master etcd: advertise client URLs = http://etcd:2379,http://etcd:4001
May 24 13:39:28 k8s-master etcd: ignored file 0000000000000001-0000000000012700.wal.broken in wal
May 24 13:39:29 k8s-master etcd: restarting member 8e9e05c52164694d in cluster cdf818194e3a8c32 at commit index 148905
May 24 13:39:29 k8s-master etcd: 8e9e05c52164694d became follower at term 12
May 24 13:39:29 k8s-master etcd: newRaft 8e9e05c52164694d [peers: [8e9e05c52164694d], term: 12, commit: 148905, applied: 140014, lastindex: 148905, lastterm: 12]
May 24 13:39:29 k8s-master etcd: enabled capabilities for version 3.1
May 24 13:39:29 k8s-master etcd: added member 8e9e05c52164694d [http://localhost:2380] to cluster cdf818194e3a8c32 from store
May 24 13:39:29 k8s-master etcd: set the cluster version to 3.1 from store
May 24 13:39:29 k8s-master etcd: starting server... [version: 3.1.3, cluster version: 3.1]
May 24 13:39:29 k8s-master etcd: raft save state and entries error: open /var/lib/etcd/default.etcd/member/wal/0.tmp: is a directory
May 24 13:39:29 k8s-master systemd: etcd.service: main process exited, code=exited, status=1/FAILURE
May 24 13:39:29 k8s-master systemd: Failed to start Etcd Server.
May 24 13:39:29 k8s-master systemd: Unit etcd.service entered failed state.
May 24 13:39:29 k8s-master systemd: etcd.service failed.
May 24 13:39:29 k8s-master systemd: etcd.service holdoff time over, scheduling restart.
```
核心语句：
```
raft save state and entries error: open /var/lib/etcd/default.etcd/member/wal/0.tmp: is a directory
```
进入相关目录，删除 0.tmp，然后就可以启动了
<a name="734490d9"></a>
### ETCD启动不起来-超时问题<2>
问题背景：当前部署了 3 个 etcd 节点，突然有一天 3 台集群全部停电宕机了。重新启动之后发现 K8S 集群是可以正常使用的，但是检查了一遍组件之后，发现有一个节点的 etcd 启动不了。<br />经过一遍探查，发现时间不准确，通过以下命令 ntpdate ntp.aliyun.com 重新将时间调整正确，重新启动 etcd，发现还是起不来，报错如下：
```
Mar 05 14:27:15 k8s-node2 etcd[3248]: etcd Version: 3.3.13
Mar 05 14:27:15 k8s-node2 etcd[3248]: Git SHA: 98d3084
Mar 05 14:27:15 k8s-node2 etcd[3248]: Go Version: go1.10.8
Mar 05 14:27:15 k8s-node2 etcd[3248]: Go OS/Arch: linux/amd64
Mar 05 14:27:15 k8s-node2 etcd[3248]: setting maximum number of CPUs to 4, total number of available CPUs is 4
Mar 05 14:27:15 k8s-node2 etcd[3248]: the server is already initialized as member before, starting as etcd member
...
Mar 05 14:27:15 k8s-node2 etcd[3248]: peerTLS: cert = /opt/etcd/ssl/server.pem, key = /opt/etcd/ssl/server-key.pe
m, ca = , trusted-ca = /opt/etcd/ssl/ca.pem, client-cert-auth = false, crl-file =
Mar 05 14:27:15 k8s-node2 etcd[3248]: listening for peers on https://192.168.25.226:2380
Mar 05 14:27:15 k8s-node2 etcd[3248]: The scheme of client url http://127.0.0.1:2379 is HTTP while peer key/cert
files are presented. Ignored key/cert files.
Mar 05 14:27:15 k8s-node2 etcd[3248]: listening for client requests on 127.0.0.1:2379
Mar 05 14:27:15 k8s-node2 etcd[3248]: listening for client requests on 192.168.25.226:2379
Mar 05 14:27:15 k8s-node2 etcd[3248]: member 9c166b8b7cb6ecb8 has already been bootstrapped
Mar 05 14:27:15 k8s-node2 systemd[1]: etcd.service: main process exited, code=exited, status=1/FAILURE
Mar 05 14:27:15 k8s-node2 systemd[1]: Failed to start Etcd Server.
Mar 05 14:27:15 k8s-node2 systemd[1]: Unit etcd.service entered failed state.
Mar 05 14:27:15 k8s-node2 systemd[1]: etcd.service failed.
Mar 05 14:27:15 k8s-node2 systemd[1]: etcd.service failed.
Mar 05 14:27:15 k8s-node2 systemd[1]: etcd.service holdoff time over, scheduling restart.
Mar 05 14:27:15 k8s-node2 systemd[1]: Starting Etcd Server...
Mar 05 14:27:15 k8s-node2 etcd[3258]: recognized environment variable ETCD_NAME, but unused: shadowed by correspo
nding flag
Mar 05 14:27:15 k8s-node2 etcd[3258]: recognized environment variable ETCD_DATA_DIR, but unused: shadowed by corr
esponding flag
Mar 05 14:27:15 k8s-node2 etcd[3258]: recognized environment variable ETCD_LISTEN_PEER_URLS, but unused: shadowed
 by corresponding flag
Mar 05 14:27:15 k8s-node2 etcd[3258]: recognized environment variable ETCD_LISTEN_CLIENT_URLS, but unused: shadow
ed by corresponding flag
Mar 05 14:27:15 k8s-node2 etcd[3258]: recognized environment variable ETCD_INITIAL_ADVERTISE_PEER_URLS, but unuse
d: shadowed by corresponding flag
Mar 05 14:27:15 k8s-node2 etcd[3258]: recognized environment variable ETCD_ADVERTISE_CLIENT_URLS, but unused: sha
dowed by corresponding flag
Mar 05 14:27:15 k8s-node2 etcd[3258]: recognized environment variable ETCD_INITIAL_CLUSTER, but unused: shadowed
by corresponding flag
Mar 05 14:27:15 k8s-node2 etcd[3258]: recognized environment variable ETCD_INITIAL_CLUSTER_TOKEN, but unused: sha
dowed by corresponding flag
Mar 05 14:27:15 k8s-node2 etcd[3258]: recognized environment variable ETCD_INITIAL_CLUSTER_STATE, but unused: sha
dowed by corresponding flag
```
<a name="kj6pf"></a>
#### 解决方法
检查日志发现并没有特别明显的错误，根据经验来讲，etcd 节点坏掉一个其实对集群没有大的影响，这时集群已经可以正常使用了，但是这个坏掉的 etcd 节点并没有启动，**解决方法如下：**

- 进入 etcd 的数据存储目录进行备份 备份原有数据：
```bash
cd /var/lib/etcd/default.etcd/member/
cp *  /data/bak/
```

- 删除这个目录下的所有数据文件
```bash
rm -rf /var/lib/etcd/default.etcd/member/*
```

- 停止另外两台 etcd 节点，因为 etcd 节点启动时需要所有节点一起启动，启动成功后即可使用。
```bash
#master 节点
systemctl stop etcd
systemctl restart etcd
#node1 节点
systemctl stop etcd
systemctl restart etcd
#node2 节点
systemctl stop etcd
systemctl restart etcd
```
<a name="HblDK"></a>
### CentOS下配置主机互信
在每台服务器需要建立主机互信的用户名执行以下命令生成公钥/密钥，默认回车即可
```bash
ssh-keygen -t rsa
```
可以看到生成个公钥的文件。<br />互传公钥，第一次需要输入密码，之后就OK了。
```bash
ssh-copy-id -i /root/.ssh/id_rsa.pub root@192.168.199.132 (-p 2222)
```
-p 端口 默认端口不加-p，如果更改过端口，就得加上-p. 可以看到是在.ssh/下生成了个 authorized_keys的文件，记录了能登陆这台服务器的其他服务器的公钥。<br />测试看是否能登陆：
```bash
ssh 192.168.199.132 (-p 2222)
```
<a name="fffdac90"></a>
### CentOS 主机名的修改
```bash
hostnamectl set-hostname k8s-master1
```
<a name="YHKzm"></a>
### Virtualbox 实现 CentOS 复制和粘贴功能
如果不安装或者不输出，可以将 update 修改成 install 再运行。
```bash
yum install update
yum update kernel
yum update kernel-devel
yum install kernel-headers
yum install gcc
yum install gcc make
```
运行完后
```bash
sh VBoxLinuxAdditions.run
```
<a name="ZHlxj"></a>
### 删除Pod一直处于Terminating状态
可以通过下面命令强制删除
```bash
kubectl delete pod NAME --grace-period=0 --force
```
<a name="da8df3f2"></a>
### 删除namespace一直处于Terminating状态
可以通过以下脚本强制删除
```bash
[root@k8s-master1 k8s]# cat delete-ns.sh
#!/bin/bash
set -e
useage(){
    echo "useage:"
    echo " delns.sh NAMESPACE"
}
if [ $# -lt 1 ];then
    useage
    exit
fi
NAMESPACE=$1
JSONFILE=${NAMESPACE}.json
kubectl get ns "${NAMESPACE}" -o json > "${JSONFILE}"
vi "${JSONFILE}"
curl -k -H "Content-Type: application/json" -X PUT --data-binary @"${JSONFLE}" \
    http://127.0.0.1:8001/api/v1/namespaces/"${NAMESPACE}"/finalize
```
<a name="PDZ5p"></a>
### 容器包含有效的 CPU/内存 requests 且没有指定 limits 可能会出现什么问题？
下面创建一个对应的容器，该容器只有 requests 设定，但是没有 limits 设定，
```yaml
- name: busybox-cnt02
    image: busybox
    command: ["/bin/sh"]
    args: ["-c", "while true; do echo hello from cnt02; sleep 10;done"]
    resources:
      requests:
        memory: "100Mi"
        cpu: "100m"
```
<a name="KvN3W"></a>
#### 这个容器创建出来会有什么问题呢？
其实对于正常的环境来说没有什么问题，但是对于资源型 pod 来说，如果有的容器没有设定 limit 限制，资源会被其他的 pod 抢占走，可能会造成容器应用失败的情况。可以通过 limitrange 策略来去匹配，让 pod 自动设定，前提是要提前配置好limitrange 规则。
