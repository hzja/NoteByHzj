<a name="sChiL"></a>
## 什么是 Datree？
它是一个开源 CLI 实用工具，通过管理策略来防止 Kubernetes 工作负载和 SaaS 平台的错误配置。它验证 Kubernetes YAML 文件。
<a name="aLIjT"></a>
## 为什么需要 Datree？
防止 Kubernetes 配置出错。它可以帮助所有者、开发人员管理策略执行，自动检查是否违反规则。使用 Datree 时，不需要与生产集群建立连接。可以从在线 Datree 仪表板应用策略和规则。
<a name="VD554"></a>
## Datree 是如何工作的？

1. 检查 Yaml 文件的语法是否正确写入。
2. Kubernetes 模式验证检查版本是否有效。
3. 策略检查：它可以是内存限制、CPU 限制或任何自定义策略检查。
4. 它提供了针对 Datree 应用策略运行的 YAML 文件的摘要。
<a name="AB6NT"></a>
## 快速简单地实现 Datree
按照[**官方文档**](https://hub.datree.io/#jx-quickstart)安装 Datree<br />Nginx 的示例 deployment.yaml 文件
```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: nginx-deployment
spec:
  selector:
    matchLabels:
      app: nginx
  replicas: 2
  template:
    metadata:
      labels:
        app: nginx
    spec:
      containers:
      - name: nginx
        image: nginx:1.14.2
        ports:
        - containerPort: 80
```
要使用 Datree 验证此 yaml，请运行以下命令：
```bash
$ datree test deployment.yaml
```
![配置错误](https://cdn.nlark.com/yuque/0/2022/png/396745/1648695849522-c93bbd52-4344-4356-8e39-de3160905d26.png#clientId=ub99b3756-900a-4&from=paste&id=u2c8045fb&originHeight=903&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u770e93e0-2d49-4a37-b153-b616a683a13&title=%E9%85%8D%E7%BD%AE%E9%94%99%E8%AF%AF "配置错误")<br />所以在上图中，可以看到通过的规则总数为 21、失败的规则总数为 9。<br />已经通过 Datree Dashboard 配置了 30 条规则的默认策略。可以通过 CLI 获取仪表板的链接，如下图所示：<br />![仪表板链接](https://cdn.nlark.com/yuque/0/2022/png/396745/1648695849501-d628326a-ca92-4a4d-ae25-eb98fd8d7538.png#clientId=ub99b3756-900a-4&from=paste&id=u57ae64eb&originHeight=238&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u3d58fef5-733a-483c-9446-8d729ba7640&title=%E4%BB%AA%E8%A1%A8%E6%9D%BF%E9%93%BE%E6%8E%A5 "仪表板链接")<br />在 Datree 仪表板中，可以根据需要编辑规则，这些规则将在验证部署 YAML 文件时应用。<br />![用于配置 datree 规则的仪表板](https://cdn.nlark.com/yuque/0/2022/png/396745/1648695849592-b31a73da-7881-47c4-8860-28a1dea21980.png#clientId=ub99b3756-900a-4&from=paste&id=u75f57fd4&originHeight=554&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u5cf4f439-97b8-42ba-af07-6592a11e786&title=%E7%94%A8%E4%BA%8E%E9%85%8D%E7%BD%AE%20datree%20%E8%A7%84%E5%88%99%E7%9A%84%E4%BB%AA%E8%A1%A8%E6%9D%BF "用于配置 datree 规则的仪表板")<br />所以，已经解决了上面配置错误中显示的前四个错误。<br />更新后的 deployment.yaml
```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: nginx-deployment
spec:
  selector:
    matchLabels:
      app: nginx
  replicas: 2
  template:
    metadata:
      labels:
        app: nginx
    spec:
      containers:
      - name: nginx
        image: nginx:1.14.2
        resources:
          limits:
            memory: 200Mi
            cpu: 1
          requests:
            memory: 100Mi
            cpu: 100m
        ports:
        - containerPort: 80
```
现在如果再次运行命令来检查：
```bash
$ datree test deployment.yaml
```
![解决了 4 个配置规则](https://cdn.nlark.com/yuque/0/2022/png/396745/1648695849555-6362f0f2-4bf5-4a54-809f-d5c89296161d.png#clientId=ub99b3756-900a-4&from=paste&id=ue26d63c0&originHeight=628&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=ufa47df53-19c7-4691-8ade-cc861d35ddb&title=%E8%A7%A3%E5%86%B3%E4%BA%86%204%20%E4%B8%AA%E9%85%8D%E7%BD%AE%E8%A7%84%E5%88%99 "解决了 4 个配置规则")<br />输出是 Total Rules Failed: 5，之前是 9。<br />在 deployment 文件中配置了内存请求、CPU 请求、内存限制和 CPU 限制。<br />因此，这就是 Datree 如何用于标准化和避免 K8s 中的错误配置。
