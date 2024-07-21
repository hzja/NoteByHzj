Kubernetes<br />kubectl是K8s官方附带的命令行工具，可以方便的操作K8s集群。
<a name="mlOFb"></a>
## 打印当前使用的API
```bash
# kubectl 的主要作用就是与ApiServer进行交互, 而交互的过程, 可以通过下面的方式来打印, 
# 这个命令尤其适合调试自己的api接口时使用.
kubectl get ns -v=9
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676293397685-3398f3e4-7856-43e7-b733-5954f136999b.png#averageHue=%23f4d2d8&clientId=ue2f555dd-8538-4&from=paste&id=u2e9a45aa&originHeight=543&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8ff6f292-9494-49b9-9743-7229b18fe8e&title=)
<a name="tFZcc"></a>
## 按状态筛选容器以及删除
```bash
kubectl get pods --all-namespaces --field-selector status.phase=Pending -o json | \
  jq '.items[] | "kubectl delete pods \(.metadata.name) -n \(.metadata.namespace)"' | \
  xargs -n 1 bash -c


# 这个命令要拆开来看
# 首先, 获取所有ns中状态为Pending的pods, 并以json形式输出
# 这个语句其实由很多变体, 比如,想查找Failed的状态, 或是某个deployment
kubectl get pods --all-namespaces --field-selector status.phase=Pending -o json 

# 针对json变量进行处理, 生成可用的脚本
# 这里是想介绍的重点, 利用jq以及kubectl的输出, 构建出可用的命令
jq '.items[] | "kubectl delete pods \(.metadata.name) -n \(.metadata.namespace)"'

# 执行每一条命令
# 注意, 这种命令一定要好好调试, 删掉预期之外的pod就不好了.
xargs -n 1 bash -c


# 例如, 下面的语句可以找到所有的Pods并打印可以执行的语句
kubectl get pods --all-namespaces --field-selector status.phase=Running -o json | \
  jq '.items[] | "kubectl get pods \(.metadata.name) -o wide -n \(.metadata.namespace)"'

"kubectl get pods metrics-server-6d684c7b5-gtd6q -o wide -n kube-system"
"kubectl get pods local-path-provisioner-58fb86bdfd-98frc -o wide -n kube-system"
"kubectl get pods nginx-deployment-574b87c764-xppmx -o wide -n default"

# 当然, 如果只是删除单个NS下面的一些pods, 会选择下面的方法, 但是它操作多个NS就很不方便了.
kubectl -n default get pods  | grep Completed | awk '{print $1}' | xargs kubectl -n default delete pods
```
<a name="Jg6Uq"></a>
## 统计具体某台机器上运行的所有pod
kubectl可以使用两种选择器，一种是label，一种是field，可以看官网的介绍：LabelsandSelectorsFieldSelectors
```bash
# 它是一种选择器, 可以与上面的awk或者xargs配合使用.
# 个人平时都不喜欢用这个, 直接get全部pods, 然后grep查找感觉更快
kubectl get pods --all-namespaces -o wide --field-selector spec.nodeName=pve-node1
```
<a name="Uu5QM"></a>
## 统计Pod在不同机器的具体数量分布
基于kubernetes的PaaS平台中细力度控制pods方案的实现。均衡分布的工作前提是得知pod在各个机器的分布情况。最好的办法就是得到pod信息之后进行简单的统计，这个工作可以使用awk实现。
```bash
kubectl -n default get pods -o wide -l app="nginx" | awk '{print $7}'|\
 awk '{ count[$0]++  } 
 END { 
   printf("%-35s: %s\n","Word","Count");
   for(ind in count){
    printf("%-35s: %d\n",ind,count[ind]);
   }
 }'

# 执行结果如下
Word                               : Count
NODE                               : 1
pve-node1                          : 1
pve-node2                          : 1
```
<a name="gD0kA"></a>
## `kubectl proxy`的使用
可以理解为这个命令为K8s的ApiServer做了一层代理，使用该代理，可以直接调用API而不需要经过鉴权。启动之后，甚至可以实现kubectl套娃，下面是一个例子：
```bash
# 当没有设置kubeconfig而直接调用kubectl时
kubectl get ns -v=9
# 可以打印出下面类似的错误
curl -k -v -XGET  -H "Accept: application/json, */*" -H "User-Agent: kubectl/v1.21.3 (linux/amd64) kubernetes/ca643a4" 'http://localhost:8080/api?timeout=32s'
skipped caching discovery info due to Get "http://localhost:8080/api?timeout=32s": dial tcp 127.0.0.1:8080: connect: connection refused                     
# 也就是说当不指定kubeconfig文件时, kubectl会默认访问本机的8080端口
# 那么先启动一个kubectl proxy, 然后指定监听8080, 再使用kubectl直接访问, 是不是就可行了呢, 
# 事实证明, 安全与预想一致.
KUBECONFIG=~/.kube/config-symv3 kubectl proxy  -p 8080
kubectl get ns
NAME                           STATUS   AGE
default                        Active   127d
```
默认启动的proxy是屏蔽了某些api的，并且有一些限制，例如无法使用exec进入pod之中，可以使用`kubectl proxy --help`来看，例如
```bash
# 仅允许本机访问
--accept-hosts='^localhost$,^127\.0\.0\.1$,^\[::1\]$': Regular expression for hosts that the proxy should accept.
# 不允许访问下面的api, 也就是说默认没法exec进入容器
--reject-paths='^/api/.*/pods/.*/exec,^/api/.*/pods/.*/attach': Regular expression for paths that the proxy should reject. Paths specified here will be rejected even accepted by --accept-paths.

# 想跳过exec的限制也很简单, 把reject-paths去掉就可以了
kubectl proxy -p 8080 --keepalive 3600s --reject-paths='' -v=9
```
有人说这个kubectl proxy可能没什么作用，那可能仅仅是还没有实际的应用场景。例如当想要调试K8s dashboard代码的时候。如果直接使用kubeconfig文件，没法看到具体的请求过程，如果加上一层proxy转发，并且设置`-v=9`的时候，就自动获得了一个日志记录工具，在调试时相当有用。
<a name="u6tu9"></a>
## 总结
kubectl是一个强大的命令行工具，上面只是介绍了工作中对其用法的一点探索，也并不鼓励大家非要记住这些命令，只是希望当需要的时候，能够想起来kubectl可以有类似的功能，就不需要针对几个临时需求去研读client-api了。
