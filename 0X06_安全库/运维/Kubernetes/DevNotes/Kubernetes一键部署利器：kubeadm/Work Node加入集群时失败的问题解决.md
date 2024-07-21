Kubernetes kubeadm
<a name="IBAmN"></a>
## 公网IP不在信任IP列表中的问题
<a name="NPbPs"></a>
### 报错内容：couldn't validate the identity of the API Server: certificate is valid for 10.96.0.1, 172.17.157.248, not 47.100.195.253
error execution phase preflight: couldn't validate the identity of the API Server: Get "[https://47.100.195.253:6443/api/v1/namespaces/kube-public/configmaps/cluster-info?timeout=10s":](https://47.100.195.253:6443/api/v1/namespaces/kube-public/configmaps/cluster-info?timeout=10s":) x509: certificate is valid for 10.96.0.1, 172.17.157.248, not 47.100.195.253<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1640352080102-13c8768c-a0f1-4456-a587-eb2788ce77fc.png#clientId=u37a84f8d-7b2e-4&from=paste&height=145&id=ub2ea1d4d&originHeight=436&originWidth=2510&originalType=binary&ratio=1&rotation=0&showTitle=false&size=996999&status=done&style=none&taskId=uc2dd13c4-7d80-4b1e-b88c-fd51dfc96f5&title=&width=836.6666666666666)
<a name="FXMB7"></a>
### 解决方案
公网IP未添加至信任IP中，Master Node需要重置启动，在启动时需要添加公网IP的参数`--apiserver-cert-extra-sans=120.79.23.226`。
<a name="UU3aB"></a>
## ConfigMap获取失败的问题
<a name="teGZb"></a>
### 报错内容：unable to fetch the kubeadm-config ConfigMap: failed to get config map
error execution phase preflight: unable to fetch the kubeadm-config ConfigMap: failed to get config map: Get "[https://172.17.157.248:6443/api/v1/namespaces/kube-system/configmaps/kubeadm-config?timeout=10s":](https://172.17.157.248:6443/api/v1/namespaces/kube-system/configmaps/kubeadm-config?timeout=10s":) dial tcp 172.17.157.248:6443: connect: no route to host<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1640422037150-51e0d579-50aa-4899-8cd6-229aa51c6816.png#clientId=u660c12d5-427f-4&from=paste&height=179&id=u4f20a6cb&originHeight=537&originWidth=2538&originalType=binary&ratio=1&rotation=0&showTitle=false&size=146140&status=done&style=none&taskId=u58ff7dce-a625-46bb-b0a3-373c845ce3a&title=&width=846)
<a name="R1LSi"></a>
### 解决方案
<a name="VlHSf"></a>
## Token失效的问题
<a name="fCPxX"></a>
### 报错内容：couldn't validate the identity of the API Server: could not find a JWS signature in the cluster-info ConfigMap for token ID "1t8xr4"
```bash
kubeadm join 47.100.195.253:6443 --token 1t8xr4.eqlezpou8dm8mpcy         --discovery-token-ca-cert-hash sha256:9170e19223b6ba70aedef919f2198467278e67e11826f8d92dc4d1cba5b93873
[preflight] Running pre-flight checks
error execution phase preflight: couldn't validate the identity of the API Server: could not find a JWS signature in the cluster-info ConfigMap for token ID "1t8xr4"
To see the stack trace of this error execute with --v=5 or higher
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1640338689362-22919615-722a-41ba-a678-fc9ceda933dd.png#clientId=u5508554a-d5d0-4&from=paste&height=112&id=uecd8caf5&originHeight=337&originWidth=2522&originalType=binary&ratio=1&rotation=0&showTitle=false&size=93239&status=done&style=none&taskId=u88163f01-14f1-466e-b92f-3562d00c3e7&title=&width=840.6666666666666)
<a name="kkFpp"></a>
### 解决方案(一)
<a name="mFMjW"></a>
#### 查看Master节点的Token
```bash
kubeadm token list
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1640338852251-cef23c5b-dc8d-4cc6-b9e9-6753f32f93e9.png#clientId=u5508554a-d5d0-4&from=paste&height=32&id=ubb157f84&originHeight=97&originWidth=1765&originalType=binary&ratio=1&rotation=0&showTitle=false&size=25847&status=done&style=none&taskId=u015de251-5ed1-4cc7-bdc9-3ee26111883&title=&width=588.3333333333334)
<a name="VDjwm"></a>
#### 创建新Token
```bash
[root@iZuf68qk0bio1l786kylamZ ~]# kubeadm token create
htpca1.bjmrvnfl2mojtvmc
[root@iZuf68qk0bio1l786kylamZ ~]# kubeadm token list
TOKEN                     TTL         EXPIRES                USAGES                   DESCRIPTION                                                EXTRA GROUPS
htpca1.bjmrvnfl2mojtvmc   23h         2021-12-25T09:31:34Z   authentication,signing   <none>                                                     system:bootstrappers:kubeadm:default-node-token
[root@iZuf68qk0bio1l786kylamZ ~]# openssl x509 -pubkey -in /etc/kubernetes/pki/ca.crt | openssl rsa -pubin -outform der 2>/dev/null | openssl dgst -sha256 -hex | sed 's/^.* //'
9170e19223b6ba70aedef919f2198467278e67e11826f8d92dc4d1cba5b93873
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1640339086638-aa1959c0-72a5-43a9-8573-7fc48d07a474.png#clientId=u5508554a-d5d0-4&from=paste&height=248&id=u1ff3e302&originHeight=745&originWidth=2527&originalType=binary&ratio=1&rotation=0&showTitle=false&size=178888&status=done&style=none&taskId=u63fd8e07-0787-4d63-94ba-bd4fc48b593&title=&width=842.3333333333334)
<a name="tvfcg"></a>
#### 替换新的Token和sha256再执行Work Node的添加操作
```bash
kubeadm join 172.17.157.248:6443 --token 1t8xr4.eqlezpou8dm8mpcy \
        --discovery-token-ca-cert-hash sha256:9170e19223b6ba70aedef919f2198467278e67e11826f8d92dc4d1cba5b93873
```
<a name="wK5JD"></a>
### 解决方案(二)
```bash
# 生成一个新的 token 令牌(比上面的方便)
$ kubeadm token generate

# 直接生成 join 命令(比上面的方便)
$ kubeadm token create <token_generate> --print-join-command --ttl=0
```
