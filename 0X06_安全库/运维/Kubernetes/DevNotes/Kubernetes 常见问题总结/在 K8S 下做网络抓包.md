在 Kubernetes 的实际使用过程中，经常会碰到一些业务上的异常问题，一般情况下通过日志监控和链路追踪足以能够对问题做出排查与诊断了。但是，在某些场景下，只靠这些手段往往是不够的，一些和网络相关的问题有时候非常棘手。<br />通过身边的案例观察到，服务上线出现问题时，通常具备网络排查能力的工程师，不论是在定位问题，还是在解决问题上，都会比不具备该能力的工程师快上很多。<br />所以，对开发工程师来说，学会在 Kubernetes 里抓包 是一项非常重要的技能，下面分享一下，在 Kubernetes 的世界里，处于不同条件下的几种常见抓包方法。
<a name="xR30A"></a>
## **1、宿主机上捕获**
应用其实是运行在 Pod 内的 Container 里的，所以只要定位到 Container 被调度到了哪个 Node 上，在相应的 Node 里，对容器进行抓包即可。
<a name="gUojr"></a>
### 0) 先决条件
需要有宿主机的访问权限
<a name="a3CWW"></a>
### 1) 定位 Pod 的 containerID 以及它所运行的宿主机 IP
在 Kubernetes 集群内执行下面这个指令，并从返回的结果中拿到两个信息

1. **宿主机 IP = 172.18.0.xxx**
2. **container ID = 78e91175699f.....**
```bash
# 注: 需要替换 namespace 和 pod name
➜ kubectl get pod \
  -n ${NAMESPACE}${POD_NAME} \
  -o json|jq '.status|{hostIP: .hostIP, container: [.containerStatuses[]|{name: .name, containerID: .containerID}]}'
{
  "hostIP": "172.18.0.xxx",
  "container": [
    {
      "name": "linkerd-proxy",
      "containerID": "docker://78e91175699f8cc0a3b0ff87da97407c19c7a86706a5b74e2d86f4428a4de75a"
    },
    {
      "name": "nginx",
      "containerID": "docker://7a6f7eabc2d5112437d30ee8ec1aa7ef963e97c3d09c3bc63613a70d106d7d01"
    }
  ]
}
```
<a name="kbxX4"></a>
### 2) 查找网络接口索引
通过 ssh 登陆到 Pod 所在的宿主机上，然后在容器内执行 cat /sys/class/net/eth0/iflink，查找容器中的网卡与宿主机的 veth 网卡之间的对应关系
```bash
# 注: 需要替换 container id
docker exec -it ${CONTAINER_ID} /bin/bash -c 'cat /sys/class/net/eth0/iflink'
10227
```
<a name="P8psy"></a>
### 3) 查找网络接口信息
在宿主机上做 ip link 操作
```bash
ip link |grep 10227
10227: calicf227ed888a@if4: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1440 qdisc noqueue state UP mode DEFAULT group default
```
<a name="VM0xY"></a>
### 4) 在宿主机上抓包
最后，通过 tcpdump 指令，将报文保存到文件中
```bash
tcpdump -i calicf227ed888a@if4 -w /root/tcpdump.pcap
```
<a name="DWz4q"></a>
#### 思考
通常情况下，并不是所有的开发者都能触碰到宿主机的权限的，所以这种方式只适合集群运维人员，而且这种操作确实也过于繁琐了。<br />对于开发者来说，离我们最近的还是 Pod，下面来看下直接通过 Pod 是如何进行抓包的。
<a name="sVCLN"></a>
## 2、在 Pod 内抓包
<a name="zLnjf"></a>
### 0) 先决条件

1. **目标 Pod 内的容器里必须要有 tcpdump 工具**
2. **本地安装 wireshark**
<a name="IUIO4"></a>
### 1) 执行以下命令对目标 Pod 抓包
```bash
# 注: -c ${CONTAINER_NAME} 是可选择的。如果 Pod 中仅包含一个容器，就可以忽略它
kubectl exec${POD_NAME} -c ${CONTAINER_NAME} -- tcpdump -i eth0 -w - | wireshark -k -i -
```
<a name="OZNHi"></a>
#### 思考
不难发现，这种方法比第一种在执行层面上少了很多步骤。<br />只要满足了 2 个前置条件，执行一条指令即可通过 wireshark 来解析报文，而且不需要宿主机的权限，但是它会存在一个问题：就是容器里必须要有 tcpdump 这个工具<br />一般来说可能会想到两个解决办法：

1. **将 tcpdump 打包到镜像里。但是这样做，它不仅会增加镜像包的大小，也会有一定的风险，另外不可能要求三方的镜像也必须默认装上对吧？**
2. **有同学说，直接进入容器里做现场安装咯？但是，有没有想过，很多客户部署的环境都是要求隔离外网的，甚至有些 base image 用的是 **[**无发行版镜像**](https://github.com/GoogleContainerTools/distroless)**，是根本没机会做这件事的。**

所以...上面两个方法不是说不能用，只能说不是最好的解决方案罢了。
<a name="otOd0"></a>
## 3、通过 ephemeral containers 抓包
值得一提的是 Kubernetes 在 v1.16 [Alpha] 开始支持 [**](https://kubernetes.io/docs/concepts/workloads/pods/ephemeral-containers/)，它正好可以解决上面提的 2 个痛点，临时容器对于排除交互式故障非常有用，Kubernetes 在 v1.23 [beta] 已经默认开启该功能了。<br />比如使用 [**nicolaka/netshoot**](https://github.com/nicolaka/netshoot) 镜像用来调试，用法如下
```bash
# 注: 需要替换 pod name 和 container name
kubectl debug -i ${POD_NAME} \
  --image=nicolaka/netshoot \
  --target=${CONTAINER_NAME} -- tcpdump -i eth0 -w - | wireshark -k -i -
```
<a name="J5MUG"></a>
#### 思考
但是，如果集群版本比较低，在 v1.23 版本以下，且集群管理员并没有开启 Ephemeral Containers 这个特性，咋办咧？<br />如果是在生产环境，不可能让管理员升级集群版本，或者开启某一个特性来做 Debug 吧？<br />那么问题来了，有没有一种工具，它既可以做到不用触碰宿主机，又可以不必在目标 Pod 内的容器里提前安装 tcpdump 呢？<br />非常好的一个问题，社区正好提供了 [**ksniff**](https://github.com/eldadru/ksniff) 这个插件解决了这个难题，下面一起来看一下吧。
<a name="SCblu"></a>
## 4、通过 Ksniff 抓包
ksniff 是一个 kubectl 的插件，它利用 tcpdump 和 Wireshark 对 Kubernetes 集群中的任何 Pod 启动远程抓包。
<a name="yOqql"></a>
### 0) 先决条件
<a name="ySOuU"></a>
#### 安装 Krew
```bash
(
  set -x; cd"$(mktemp -d)" &&
  curl -fsSLO "https://github.com/kubernetes-sigs/krew/releases/latest/download/krew.tar.gz" &&
  tar zxvf krew.tar.gz &&
  KREW=./krew-"$(uname | tr '[:upper:]' '[:lower:]')_$(uname -m | sed -e 's/x86_64/amd64/' -e 's/arm.*$/arm/')" &&
  "$KREW" install krew
)
export PATH="${KREW_ROOT:-$HOME/.krew}/bin:$PATH"
```
<a name="c2Hly"></a>
#### 安装 sniff 插件
一旦安装完 Krew 后，就可以通过以下指令完成 sniff 的安装
```bash
kubectl krew install sniff
```
<a name="UEHtj"></a>
### 1) 执行 kubectl sniff 命令抓包
```bash
# 注：需要替换 pod name 和 namespace
kubectl sniff ${POD_NAME} -n ${NAMESPACE}
```
执行 sniff 命令后，本地会自动启动 Wireshark 进行抓包，如下图<br />![wireshark](https://cdn.nlark.com/yuque/0/2023/png/396745/1702947246982-ca987e9e-d7eb-4e63-b2a4-ab7c37e4bc03.png#averageHue=%23e4e3e3&clientId=u3a1cf960-7f5e-4&from=paste&id=u9f2844ea&originHeight=828&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=uc8eccc60-27d7-465a-888b-f422a2f54a6&title=wireshark "wireshark")<br />以下是 sniff 运行的日志，只提取了日志的关键部分
```bash
➜ kubectl sniff httpbin -n default

time="2022-02-20T19:56:13+08:00" level=info msg="using tcpdump path at: '/Users/linqiong/.krew/store/sniff/v1.6.1/static-tcpdump'"
time="2022-02-20T19:56:13+08:00" level=info msg="selected container: 'httpbin'"
time="2022-02-20T19:56:13+08:00" level=info msg="uploading file: '/Users/linqiong/.krew/store/sniff/v1.6.1/static-tcpdump' to '/tmp/static-tcpdump' on container: 'httpbin'"
....
time="2022-02-20T19:56:14+08:00" level=info msg="tcpdump uploaded successfully"
time="2022-02-20T19:56:14+08:00" level=info msg="spawning wireshark!"
time="2022-02-20T19:56:14+08:00" level=info msg="start sniffing on remote container"
time="2022-02-20T19:56:14+08:00" level=info msg="executing command: '[/tmp/static-tcpdump -i any -U -w - ]' on container: 'httpbin', pod: 'httpbin', namespace: 'default'"
```
从运行的日志来看，sniff 是将本地的 static-tcpdump 文件上传到 Pod 的指定容器的 /tmp 目录下，然后在容器内，通过运行以下命令来达到抓包的目的
```bash
/tmp/static-tcpdump -i any -U -w -
```
另外需要说明的是，如果是在服务器上执行，且服务器并没有安装 wireshark，可以将报文输出到文件中，然后用本地的 wireshark 来解析报文。
```bash
kubectl sniff httpbin -n default -o httpbin.pcap
```
其余参数大家可以自行摸索做一下尝试。
<a name="v6aUs"></a>
## 推荐阅读

- [Ksniff: packet capture at pod level for k8s](https://kubesandclouds.com/index.php/2021/01/20/ksniff/)
<a name="cCFVj"></a>
## 总结
文章主要分享了在 Kubernetes 下是如何进行网络抓包的，并不涉及抓包分析技术。说到底，抓包技术主要还是要学会 tcpdump 和 Wireshark 这两个工具的用法，抓包分析技术其实是一门实践课，理论看得再多，用处也不大。<br />它需要的是实践...再实践...
<a name="fRLf8"></a>
## 参考资料
无发行版镜像: [_https://github.com/GoogleContainerTools/distroless_](https://github.com/GoogleContainerTools/distroless)<br />Ephemeral Containers: [_https://kubernetes.io/docs/concepts/workloads/pods/ephemeral-containers/_](https://kubernetes.io/docs/concepts/workloads/pods/ephemeral-containers/)<br />nicolaka/netshoot: [_https://github.com/nicolaka/netshoot_](https://github.com/nicolaka/netshoot)<br />ksniff: [_https://github.com/eldadru/ksniff_](https://github.com/eldadru/ksniff)<br />Ksniff: packet capture at pod level for k8s: [_https://kubesandclouds.com/index.php/2021/01/20/ksniff/_](https://kubesandclouds.com/index.php/2021/01/20/ksniff/)
