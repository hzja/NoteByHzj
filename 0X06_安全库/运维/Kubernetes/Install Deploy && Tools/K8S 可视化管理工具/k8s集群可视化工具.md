Kubernetes集群可视化<br />Kubermetrics 是一个开源工具，它以简单易懂的用户界面提供 Kubernetes 集群监控和数据可视化。Kubermetrics 集成了 Prometheus 和 Grafana 仪表板在一个页面，允许定制仪表板和警报。

- [Kubermetrics Dockerhub](https://hub.docker.com/r/kubermetrics/kubermetrics)
- [Kubermetrics Github](https://github.com/oslabs-beta/kubermetrics)
<a name="mFcTo"></a>
## 主页
按 namespace 显示所有 nodes, deployments, pods, services 和 ingresses 的仪表板。<br />![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1642993146203-0dc9cccb-afd0-45a6-82b4-8285a300d24e.gif#clientId=u21a60a41-0d85-4&from=paste&id=u10226d38&originHeight=607&originWidth=1079&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u89870edd-3990-4869-a665-e5fcc39f2f0&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1642993144953-f3da7f25-692d-468f-a19c-aa16fd27f639.gif#clientId=u21a60a41-0d85-4&from=paste&id=u0e2a1ab0&originHeight=607&originWidth=1079&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ubb467569-f4aa-4e67-b20b-1afd50074e1&title=)![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1642993145249-9ad5a4a0-849f-4702-a17f-53503250768b.gif#clientId=u21a60a41-0d85-4&from=paste&id=ucdc5b4fc&originHeight=607&originWidth=1079&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub3bfe95a-a9ac-4196-9168-422855aad12&title=)
<a name="qLodd"></a>
## 指标页
指标页面利用了完整的 Grafana 集成定制仪表板。
<a name="ILph6"></a>
## 告警页
警报页利用完整的 Prometheus 集成，以便访问警报，图表，prom-queries等！
<a name="EbiJU"></a>
## 部署 Kubermetrics
```bash
git clone https://github.com/oslabs-beta/kubermetrics.git
unzip kubermetrics.zip
```
如果未安装 Prometheus 和 Grafana，可以在 kubermetrics 根目录下执行apply，将安装全部依赖组件：
```bash
kubectl apply -f manifests
```
如果已安装 Prometheus 和 Grafana，则进入manifests目录，apply清单 yaml 文件：
```bash
# cd kubermetrics/manifests
# kubectl apply -f kubermetrics-depl.yaml
deployment.apps/kubermetrics-depl created
service/kubermetrics-srv created
```
创建的 Pod 一直重启，查看日志报错如下：
```bash
# kubectl get pod  | grep kuberme
kubermetrics-depl-5c54969bc-w4nnt     0/1     CrashLoopBackOff   1          88s

# k logs -f kubermetrics-depl-5c54969bc-w4nnt
standard_init_linux.go:211: exec user process caused "exec format error"
```
这个问题在 github 上有相同的[issue](https://github.com/oslabs-beta/kubermetrics/issues/30)，目前尚未解决。
```bash
# docker run -ti kubermetrics/kubermetrics:latest bash
standard_init_linux.go:211: exec user process caused "exec format error"

# cat /etc/redhat-release
CentOS Linux release 7.9.2009 (Core)

# uname -m
x86_64
```
该错误 CPU 架构不同导致的，默认镜像适用于linux/arm64。<br />为此编译了 x86 架构的镜像，推送到阿里云镜像仓库和 dockerhub 仓库：
```bash
# 阿里云仓库
docker pull registry.cn-hangzhou.aliyuncs.com/smallsoup/kubermetrics

# dockerhub
docker pull smallsoup/kubermetrics
```
请运行以下命令以确保 grafana 和 prometheus 在这些端口上启动并运行！
```bash
kubectl port-forward <grafana-pod-name-goes-here> --namespace=<yournamespace> 3000:3000
kubectl port-forward <prometheus-pod-name-goes-here> --namespace=<yournamespace> 9090:9090
```
找到您当前正在运行的 Kubermetrics Pod 并运行以下命令
```bash
# kubectl port-forward --address 0.0.0.0 pod/{pod名称} 8899:3068
Forwarding from 0.0.0.0:8899 -> 3068
Handling connection for 8899
```
这将在本机所有地址监听 8899 端口，转发到 Pod 的 3068 端口。<br />假如本机 IP 为10.0.11.2，则访问10.0.11.2:8899即可访问仪表板。
<a name="xDEGW"></a>
## 比较坑的是
应用程序在本地主机 localhost:3000 上查找 grafana，9090 上查找 promethues。不支持自定义。<br />Prometheus 的地址是写死：
```
./client/components/PrometheusMonitoring/PrometheusAlerts.js:            src={"http://localhost:9090/alerts"}
```
Grafana 的地址是写死：
```bash
./client/components/GrafanaMonitoring/ClusterDashboard.js:            src={"http://localhost:3000/goto/tJaUhdH7z?orgId=1"}
./client/components/GrafanaMonitoring/PodDashboard.js:            src={"http://localhost:3000/d-solo/AAOMjeHmk/kubernetes-pod-monitoring?orgId=1&refresh=10s&from=1632155279207&to=1632158879207&panelId=3"}
./client/components/GrafanaMonitoring/PodDashboard.js:            src={"http://localhost:3000/d-solo/AAOMjeHmk/kubernetes-pod-monitoring?orgId=1&refresh=10s&from=1632156042391&to=1632159642391&panelId=2"}
./client/components/GrafanaMonitoring/PodDashboard.js:            src={"http://localhost:3000/d-solo/AAOMjeHmk/kubernetes-pod-monitoring?orgId=1&refresh=10s&from=1632156127873&to=1632159727873&panelId=8"}
```
可以自己修改代码，以支持指定两组件地址作为参数。
<a name="llvkN"></a>
### 参考资料
Kubermetrics Dockerhub: [https://hub.docker.com/r/kubermetrics/kubermetrics](https://hub.docker.com/r/kubermetrics/kubermetrics)<br />Kubermetrics Github: [https://github.com/oslabs-beta/kubermetrics](https://github.com/oslabs-beta/kubermetrics)<br />issue: [https://github.com/oslabs-beta/kubermetrics/issues/30](https://github.com/oslabs-beta/kubermetrics/issues/30)
