Kubernetes<br />这里所有网站都是基于 Kubernetes 来 serve 的，域名解析都是通过 Nginx Ingress 来做的，那怎么专门给某个网站升级 HTTP2 呢？<br />最重要的配置就在这里了：[https://kubernetes.github.io/ingress-nginx/user-guide/nginx-configuration/configmap/](https://kubernetes.github.io/ingress-nginx/user-guide/nginx-configuration/configmap/)，通过修改 Ingress Controller 就可以控制 HTTP 2 的开关。<br />那这个 ConfigMap 又和 Ingress 什么关系呢？<br />答案如下：

- Ingress 可以配置哪个域名转发到哪个后端来处理。
- Ingress 的使用需要依赖一个 Ingress Controller。
- 每个 Ingress Controller 会对应一个 ConfigMap 来配置当前 Nginx 服务器的行为。

在配置之前，所有的网站都是使用同一个 Ingress Controller 来处理的，而且 Ingress Controller 对应的 ConfigMap 都是一样的。<br />所以为了单独让某个网站应用 HTTP 2，而不影响其他网站，那就需要使用单独的 ConfigMap 来处理，那就需要一个新的 Ingress Controller。<br />所以，HTTP 1.1 和 HTTP 2 都分别对应一套 Ingress Controller 和 ConfigMap 的配置。<br />这样思路就清晰了。<br />所以，接下来需要新建一个 Ingress Controller，这里所有网站所在的 namespace 是 scrape，所以直接用 helm 在 scrape 这个 namespace 下新建一个 Nginx Ingress 套件，这里就直接取名为 ingress-nginx3 （因为其他 namespace 已经有 ingress-nginx2 了，就顺着取名）了：
```bash
helm install ingress-nginx3 -n scrape ingress-nginx/ingress-nginx
```
具体可以参见：[https://kubernetes.github.io/ingress-nginx/deploy/](https://kubernetes.github.io/ingress-nginx/deploy/)<br />OK，好了之后，可以观察到有一个对应的 ConfigMap 就生成了：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624030457814-fffab762-11e0-4c68-b762-c155de3e97bc.png#clientId=uad4f198f-00fb-4&from=paste&id=u45b15f75&originHeight=348&originWidth=1080&originalType=url&ratio=3&status=done&style=none&taskId=u007dc8f5-c40d-4782-b8ce-a0a765ab985)<br />OK，这里 yaml 文件类似如下：
```yaml
apiVersion: v1
kind: ConfigMap
metadata:
  annotations:
    meta.helm.sh/release-name: ingress-nginx3
    meta.helm.sh/release-namespace: scrape
  creationTimestamp: "2021-05-16T13:24:53Z"
  labels:
    app.kubernetes.io/component: controller
    app.kubernetes.io/instance: ingress-nginx3
    app.kubernetes.io/managed-by: Helm
    app.kubernetes.io/name: ingress-nginx
    app.kubernetes.io/version: 0.46.0
    helm.sh/chart: ingress-nginx-3.30.0
  managedFields:
  - apiVersion: v1
    manager: Go-http-client
    operation: Update
    time: "2021-05-16T13:24:53Z"
  - apiVersion: v1
    manager: tke-apiserver
    operation: Update
    time: "2021-05-16T13:46:33Z"
  name: ingress-nginx3-controller
  namespace: scrape
  resourceVersion: "2694665147"
  selfLink: /api/v1/namespaces/scrape/configmaps/ingress-nginx3-controller
  uid: bd60849d-be4c-4efa-9196-b00e8a4e2f8b
```
这里需要单独增加 HTTP2 的配置：
```yaml
apiVersion: v1
+ data:
+  use-http2: "true"
kind: ConfigMap
metadata:
...
```
另外还有很多 ConfigMap 的配置，可以查阅官方文档：[https://kubernetes.github.io/ingress-nginx/user-guide/nginx-configuration/configmap/](https://kubernetes.github.io/ingress-nginx/user-guide/nginx-configuration/configmap/)<br />这里就直接配置 use-http2 为 true 了。<br />这里有个很诡异的问题，看文档说 use-http2 默认就是 true，但不知道为什么之前一直不生效，加了这个才生效。<br />保存 yaml 即可，这样配置工作就完成了大半了。<br />接下来，再把域名解析的地址修改成新建的 Nginx Ingress 套件中的 Ingress Controller 上就好了，Ingress Controller 背后会自动创建一个 Load Balance，对应一个公网 IP，改下域名解析地址即可。<br />升级的是 spa16.scrape.center，所以只解析 spa16 的 A 地址即可。<br />改完之后，稍等片刻，ping 下验证下就好了。
<a name="HRlfH"></a>
## 结果
最后升级成 HTTP 2 的示例网站如下：https://spa16.scrape.center/，加载时 Network 面板如下：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624030457035-eb7ede32-fa4e-41fc-93a9-a20332dcac8f.png#clientId=uad4f198f-00fb-4&from=paste&id=ub1f3330f&originHeight=362&originWidth=1080&originalType=url&ratio=3&status=done&style=shadow&taskId=u05f8aeef-5478-4d2c-94e3-571f346c3b8)<br />可以看到加载时 Protocol 就是 h2 了，意思就是 HTTP2。<br />而另一个没升级的网站，网址如：https://spa15.scrape.center/，加载时 Network 面板如下：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624030458096-e2e4c03f-a66b-4f43-a682-127629e87493.png#clientId=uad4f198f-00fb-4&from=paste&id=uc932733c&originHeight=381&originWidth=1080&originalType=url&ratio=3&status=done&style=shadow&taskId=u70bf203d-4fe3-422f-9257-3193a264344)<br />这里的 Protocol 还是 http/1.1，意思就是 HTTP 1.1。<br />通过 Waterfall 大家也能看出二者的差别吧，由于 HTTP 1.1 在 Chrome 浏览器中同时只有 6-8 个连接，所以可以看到后面 2 个 chunk 文件的加载被滞后了，而 HTTP2 就不会有这个问题，Waterfall 是不是很整齐？这就是 HTTP 2 的优势之一。<br />其实也是爬虫示例网站的其中之一，大家可以来试试爬一爬～
<a name="E8iUY"></a>
## 后记
有的朋友可能问，就只改 Ingress 配置就好了吗？Docker 镜像里面本身用了 HTTP 1.1 也行吗？<br />是的！经过验证，只有最后一层 Ingress 的配置改成 HTTP 2 就好了，Ingress 背后的 Docker 服务该啥样就还是啥样，也不用改镜像，也不用改 Nginx 配置，也不用专门改 HTTPS，最后一层做了 HTTP 2 的支持就好了。
