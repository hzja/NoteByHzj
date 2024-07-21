Kubernetes<br />mizu 是为 Kubernetes 提供的一个简单而强大的 API 流量查看器，可以查看微服务之间的所有 API 通信，以帮助调试和排除故障。相当于 Kubernetes 的 TCPDump 和 Wireshark。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642991885143-f84bb69d-e37f-4dbd-a9db-a021f93a227a.webp#clientId=u7256ea7a-a6e4-4&from=paste&id=u036525e9&originHeight=634&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1c93028f-4fce-485c-a869-69154da701b&title=)

- 简单而强大的 CLI
- 丰富的过滤规则
- API 调用实时监控网络流量视图，支持的协议：
   - HTTP/1.1(REST 等)、HTTP/2(gRPC)
   - AMQP（RabbitMQ、Apache Qpid 等）
   - Kafka
   - Redis
<a name="s6jet"></a>
## 安装前准备
mizu 组件会用到以下四个镜像：
```bash
# Linux上执行mizu tap安装时，mizu-apiserver用到
ghcr.io/up9inc/basenine:v0.3.0

# Linux上执行mizu tap安装时，mizu-apiserver用到
gcr.io/up9-docker-hub/mizu/develop:0.22.21

# mac上执行mizu tap安装时，mizu-apiserver用到
gcr.io/up9-docker-hub/mizu/main:0.22.0

# mizu install安装mizu-apiserver用到
gcr.io/up9-docker-hub/mizu-kratos/stable:0.0.0
```
但在国内会拉取失败，已经将用到的镜像上传到阿里云仓库和 dockerhub 仓库，需要可自行拉取：
```bash
# 阿里云仓库
registry.cn-hangzhou.aliyuncs.com/smallsoup/up9inc-basenine:v0.3.0
# dockerhub 仓库
smallsoup/up9inc-basenine:v0.3.0

registry.cn-hangzhou.aliyuncs.com/smallsoup/mizu-develop:0.22.21
smallsoup/mizu-develop:0.22.21

registry.cn-hangzhou.aliyuncs.com/smallsoup/mizu-main:0.22.0
smallsoup/mizu-main:0.22.0

registry.cn-hangzhou.aliyuncs.com/smallsoup/mizu-kratos-stable:0.0.0
smallsoup/mizu-kratos-stable:0.0.0
```
由于有些镜像名称是 mizu 源码中写死的不可配置（除了 agent-name），所以拉下来后需要修改 tag 后使用：
```bash
docker tag registry.cn-hangzhou.aliyuncs.com/smallsoup/up9inc-basenine:v0.3.0 ghcr.io/up9inc/basenine:v0.3.0

docker tag registry.cn-hangzhou.aliyuncs.com/smallsoup/mizu-develop:0.22.21 gcr.io/up9-docker-hub/mizu/develop:0.22.21

docker tag registry.cn-hangzhou.aliyuncs.com/smallsoup/mizu-kratos-stable:0.0.0 gcr.io/up9-docker-hub/mizu-kratos/stable:0.0.0

docker tag registry.cn-hangzhou.aliyuncs.com/smallsoup/mizu-main:0.22.0 gcr.io/up9-docker-hub/mizu/main:0.22.0
```
<a name="M3kZg"></a>
## 安装可执行文件
[Mizu]()可以在[GitHub 仓库](https://github.com/up9inc/mizu)下载。<br />下面示例在 MacOS 上安装 mizu 二进制，k8s 版本为 1.18.14。
```bash
curl -Lo mizu \
https://github.com/up9inc/mizu/releases/latest/download/mizu_darwin_amd64 \
&& chmod 755 mizu
mv mizu /usr/local/bin
```
<a name="zXrJr"></a>
## 生成配置
mizu CLI 使用`--set config-path=<PATH>`提供的配置文件。如果未提供，将使用`${HOME}/.mizu/config.yaml`。<br />可以使用 CLI 参数覆盖默认值，获取默认配置参数可以运行`mizu config`。<br />使用`config -r`生成默认值配置文件：
```bash
# mizu config -r
Template File written to /Users/smallsoup/.mizu/config.yaml
```
<a name="Mx2VE"></a>
## 自定义配置
编辑~/.mizu/config.yaml
```yaml
tap:
    upload-interval: 10
    regex: .*
    gui-port: 8899
    proxy-host: 0.0.0.0
    namespaces: []
    analysis: false
    all-namespaces: false
    regex-masking: []
    ignored-user-agents: []
    no-redact: false
    max-entries-db-size: 200MB
    dry-run: false
    workspace: ""
    traffic-validation-file: ""
    contract: ""
    ask-upload-confirmation: true
    api-server-resources:
        cpu-limit: 750m
        memory-limit: 1Gi
        cpu-requests: 50m
        memory-requests: 50Mi
    tapper-resources:
        cpu-limit: 750m
        memory-limit: 1Gi
        cpu-requests: 50m
        memory-requests: 50Mi
    service-mesh: false
version:
    debug: false
view:
    gui-port: 8899
logs:
    file: ""
auth:
    env-name: up9.app
    token: ""
image-pull-policy: IfNotPresent
mizu-resources-namespace: mizu
telemetry: true
dump-logs: false
kube-config-path: ""
headless: false
```
修改的参数说明：<br />将配置文件中的image-pull-policy: Always改为 IfNotPresent 因为 mizu 运行时创建的 pod 使用的镜像在国内网络环境下无法正常拉取。<br />默认运行 mizu 需要通过本地主机访问http://localhost:8899：
```bash
# lsof -nP -iTCP -sTCP:LISTEN | grep mizu
mizu      59951 smallsoup   39u  IPv4 0xade0daed4de692db      0t0  TCP 127.0.0.1:8899 (LISTEN)
```
可以将tap.proxy-host更改为0.0.0.0，通过主机 IP 地址访问。
```bash
lsof -nP -iTCP -sTCP:LISTEN | grep mizu
mizu      60008 smallsoup   38u  IPv6 0xade0daed5a54616b      0t0  TCP *:8899 (LISTEN)
```
mizu 默认使用${HOME}/.kube/config中的配置。可以使用KUBECONFIG环境变量或`--set kube-config-path=<PATH>`修改。在这里使用默认的配置。
<a name="ReqgT"></a>
## 临时安装
即使用`mizu tap`启动，会在 kubernetes 集群创建资源，在命令终止时会自行清理资源，一般用于本地调试阶段。
```bash
mizu tap
Mizu will store up to 200MB of traffic, old traffic will be cleared once the limit is reached.
Tapping pods in namespaces "default"
+chart-1642261340-waypoint-runner-d54b5bc5c-x7w5w
+chart-1642261340-waypoint-server-0
+example-nodejs-v1-dbffc74f6-6prt4
+example-nodejs-waypoint-nodejs-8764766d8-5xfjf
+netchecker-agent-8fs2m
+netchecker-agent-hostnet-vknsm
+netchecker-server-59fcd6bf86-5mtkn
Waiting for Mizu Agent to start...
Mizu is available at http://localhost:8899
```
成功后，会自动打开浏览器，访问http://localhost:8899/。<br />这种方式安装的`mizu apiserver`为独立的Pod，mizu-tapper-daemon-set为Daemonset Pod。
```bash
# k get pod -n mizu
NAME                           READY   STATUS    RESTARTS   AGE
mizu-api-server                2/2     Running   0          2m21s
mizu-tapper-daemon-set-hb555   1/1     Running   0          23s

# k get ds -n mizu
NAME                     DESIRED   CURRENT   READY   UP-TO-DATE   AVAILABLE   NODE SELECTOR   AGE
mizu-tapper-daemon-set   1         1         1       1            1           <none>          8m19s
```
Mizu 将存储高达 200MB 的流量数据，一旦达到限制，旧的数据将被清除。这里没有 pvc 的创建，所以保存的流量数据会随着 Pod 被删除。
<a name="PUOju"></a>
## 永久安装
可以使用命令`mizu install`将 mizu 实例永久的安装在集群中：
```bash
# mizu install --config-path /root/.mizu/config.yaml
namespace/mizu created
configmap/mizu-config created
serviceaccount/mizu-service-account created
clusterrole.rbac.authorization.k8s.io/mizu-cluster-role created
clusterrolebinding.rbac.authorization.k8s.io/mizu-cluster-role-binding created
role.rbac.authorization.k8s.io/mizu-role-daemon created
rolebinding.rbac.authorization.k8s.io/mizu-role-binding-daemon created
deployment.apps/mizu-api-server created
service/mizu-api-server created
Waiting for Mizu server to start...
mizu-api-server-59fbb5fc55-5krjb pod is running
Installation completed, run `mizu view` to connect to the mizu daemon instance
```
这种方式安装的 `mizu apiserver` 为 1 副本的Deployment以及mizu-tapper-daemon-set Pod。
```bash
# kubectl  get deploy -n mizu
NAME              READY   UP-TO-DATE   AVAILABLE   AGE
mizu-api-server   1/1     1            1           5m34s

# kubectl  get ds -n mizu
NAME                     DESIRED   CURRENT   READY   UP-TO-DATE   AVAILABLE   NODE SELECTOR   AGE
mizu-tapper-daemon-set   1         1         1       1            1           <none>          3m10s

# kubectl  get pvc -n mizu
NAME                STATUS   VOLUME                                     CAPACITY   ACCESS MODES   STORAGECLASS   AGE
mizu-volume-claim   Bound    pvc-81b8ebb6-2a72-4bc3-915d-79c943e6dfbb   700M       RWO            nfs-client     10m
```
同时会创建 pvc 用于存储流量数据，所以集群中需要提前有`storageClass`。<br />在 k8s 集群某一节点执行port-forward，将主机 8080 端口映射到 svc 的 80 端口：
```bash
# kubectl  port-forward --address 0.0.0.0 -n mizu  service/mizu-api-server  8080:80
Forwarding from 0.0.0.0:8080 -> 8899
```
此时用主机IP:8080访问即可：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642991885192-58f142c5-1e58-4568-8ef4-7969e655089d.webp#clientId=u7256ea7a-a6e4-4&from=paste&id=u85068c6e&originHeight=1414&originWidth=956&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue58b98d5-8965-4784-a027-b7b6a456c08&title=)
<a name="GgNlq"></a>
## 体验使用
Mizu 有丰富的过滤语法，可以灵活有效地查询结果。语法可在界面上查看：![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642991885184-f867bfe9-dd84-4b6a-a27a-17cf8506a4fd.webp#clientId=u7256ea7a-a6e4-4&from=paste&id=uf5f6fcd2&originHeight=738&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u40406057-327a-498d-a3eb-25f08b76c70&title=)<br />选择指定命名空间，将显示所选命名空间的流量：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642991885176-c192a190-1577-4b39-b557-77fae738b1df.webp#clientId=u7256ea7a-a6e4-4&from=paste&id=ud216c0a6&originHeight=1091&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua3888d3d-1279-4506-872b-a7bf457512b&title=)<br />过滤出 Kafka 协议：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642991885208-78678e66-287d-422d-9045-fdcde831b043.webp#clientId=u7256ea7a-a6e4-4&from=paste&id=u199c9667&originHeight=573&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uba52b956-1447-4be6-b7b6-d1a96521765&title=)<br />过滤来源为ingress-nginx-controller的：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642991886038-5e66acce-066b-45fa-9383-9adf291b2b68.webp#clientId=u7256ea7a-a6e4-4&from=paste&id=u384b1471&originHeight=537&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1bee0812-a9b3-4ce2-852a-bf1820582f9&title=)
<a name="vEtbR"></a>
## 卸载
永久安装后，可以通过以下方式卸载：
```bash
# mizu  clean
Removing mizu resources
```
<a name="QOLAq"></a>
## 遇到的坑
<a name="DBX92"></a>
### 域名后缀问题
mizu-tapper Daemonset Pod一直重建，查看日志发现，报错如下：
```bash
[2022-01-23T05:33:19.455+0000] INFO  ▶ socket connection to ws://mizu-api-server.mizu.svc.cluster.local/wsTapper failed: dial tcp: lookup mizu-api-server.mizu.svc.cluster.local: no such host, retrying 5 out of 30 in 2 seconds... ▶ [1 main.go:422 dialSocketWithRetry]
```
daemonset Pod需要连接mizu apiserver，连接地址是在启动参数中指定的。
```bash
--api-server-address ws://mizu-api-server.mizu.svc.cluster.local/wsTapper
```
默认使用的域名后缀为cluster.local，如果修改过 k8s 集群的clusterDomain，则需要修改这里的值。不幸的是，这个cluster.local是写死在代码中的，需要安装之后手动edit daemonset修改。
<a name="lJvkg"></a>
### 镜像问题
mizu 组件使用到的镜像，只有mizu-api-server Pod 中一个容器的镜像可以修改：
```bash
agent-image: registry.cn-hangzhou.aliyuncs.com/smallsoup/mizu-develop:0.22.21
```
其他镜像都是写死在代码中的。<br />以上两点的不可配置，会给使用者带来很大的不便。
<a name="XtfFH"></a>
## 参考资料
Mizu: [https://github.com/up9inc/mizu](https://github.com/up9inc/mizu)<br />GitHub 仓库: [https://github.com/up9inc/mizu](https://github.com/up9inc/mizu)<br />k8s 流量查看器: [https://up9.com/traffic-viewer-kubernetes](https://up9.com/traffic-viewer-kubernetes)<br />mizu 高级用法: [https://github.com/up9inc/mizu#advanced-usage](https://github.com/up9inc/mizu#advanced-usage)
