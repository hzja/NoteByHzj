Kubernetes<br />项目通常有多个 k8s 集群环境，dev、testing、staging、prod，kubetcl 在多个环境中切换，操作集群 Pod 等资源对象，前提条件是将这三个环境的配置信息都写到本地机的 $HOME/.kube/config 文件中。<br />默认情况下kubectl会查找$HOME/.kube目录中命名为config的文件。那么如何将多个 kubeconfig 文件合并为一个呢？<br />以下是步骤：
<a name="z9ek4"></a>
### 备份是个好习惯
```bash
cp $HOME/.kube/config $HOME/.kube/config-backup
```
<a name="FChqW"></a>
### 设置KUBECONFIG环境变量
KUBECONFIG环境变量是配置文件的路径列表，例如/path/cluster1:/path/cluster2:/path/cluster3
```bash
export KUBECONFIG=$HOME/.kube/config:/path/cluster1:/path/cluster2
```
如果当前目录下有很多 kubeconfig 文件，可以使用以下命令快速设置KUBECONFIG 环境变量。
```bash
$ cd $HOME/.kube
$ ls $HOME/.kube
config-dev config-testing config-staging config-prod

$ export KUBECONFIG=$HOME/.kube/config:$(find $HOME/.kube -type f -maxdepth 1 | grep config | tr '\n' ':')
```
<a name="FOIuv"></a>
### 将所有 kubeconfig 文件合并为一个
```bash
kubectl config view --flatten > all-in-one-kubeconfig.yaml
```
<a name="qBr4Q"></a>
### 验证它是否有效
显示在 kubeconfig 中定义的所有集群：
```bash
kubectl config get-contexts --kubeconfig=$HOME/.kube/all-in-one-kubeconfig.yaml
```
<a name="ebDB7"></a>
### 用新合并的配置文件替换旧配置文件
```bash
mv all-in-one-kubeconfig.yaml $HOME/.kube/config
```
<a name="aDXtT"></a>
### 总结
其他快捷工具还有 [**kubectx**](https://github.com/ahmetb/kubectx)，它可以比 kubectl 更快地在上下文（集群）之间切换，但还是依赖于 config 的合并。<br />也可以使用国人开源的 [**ki**](https://github.com/ywgx/ki) 命令行工具，不用合并 config，直接使用`ki -s`即可快速完成切换。
<a name="JEUvu"></a>
### 参考资料
**kubectx: **[https://github.com/ahmetb/kubectx](https://github.com/ahmetb/kubectx)<br />**ki: **[https://github.com/ywgx/ki](https://github.com/ywgx/ki)
