Helm<br />通过 Helm 管理 K8S 的资源发布工作，可以更加有效、清晰的管理 K8S 各类资源，是非常推荐的 K8S 集群管理工具。以下总结一下在实际项目中 Helm 的应用。在开始使用 Helm 工具之前，提前安装好工具与环境配置。<br />Helm 安装非常简单，如果本地的 kubectl 可以正常访问 K8S 集群，那么 Helm 即装即用。<br />需要注意的是，安装 Helm 3.0 以上版本。因为 3.0 版本以后，Helm 不再需要在集群里安装 tiller 服务端，可以直接将安装历史记录在 ConfigMap 资源中。
<a name="TT1BP"></a>
## 基础设施
在开始部署 K8S 集群之前，可能首先需要的是部署整个集群的基础设施。如，数据库服务、缓存服务、队列服务等。
<a name="G7j6v"></a>
### 自建基础设施
如果需要自建基础设施，即直接在集群内搭建基础服务，不使用外部服务。常规安装过程如下:
<a name="lNeFy"></a>
#### 查询可用资源
```bash
$ helm search repo mysql
```
如果找不到相应的资源，可以直接 Google 相应的资源。所谓的 repo 其实就是一个简单的 Helm Charts 的索引。
<a name="zBAAI"></a>
#### 添加资源仓库
```bash
$ helm repo add jaegertracing https://jaegertracing.github.io/helm-charts
```
<a name="L9UHk"></a>
#### 查看本地资源仓库
```bash
$: helm repo list
```
找到需要的资源后，在部署之前，需要自行检查资源的默认配置是否满足要求，如果不满足的话还需要进行自定义设置。完成自定义设置后，再部署上线。具体过程如下:
```bash
# 拉取 mysql helm charts 到本地目录
$: helm pull bitnami/mysql --untar

# 修改定义配置即可
$: vim mysql/values.yaml

# 更新或安装
$: helm upgrade --install --namespace=[YOUR NAMESPACE] --atomic --history-max 3 mysql-demo ./mysql/ --values ./mysql/values.yaml
```
<a name="ziruD"></a>
### 外部基础设施
如果不需要在集群内自建基础设施，而是直接使用外部资源时。则在集群内配置外部基础设施更加简单，只需要定义相应的 Service 资源即可。<br />不过为了更加易于管理，同样可以通过 helm 管理外部基础设施的安装。基础过程如下:
```bash
# 创建一个新的 Helm Charts, 负责管理所有外部资源
$ helm create external-services
```
默认创建的 Helm Chart，创建了很多功能模板，可以根据需要自行取舍。对于外部基础设施而言，只需要定义相应的 Service 资源即可，其他没有必要的资源模板均可以直接删除。以阿里 RDS 资源为例, 删除 external-services/templates 下无关的资源模板。直接定义一个 mysql.yaml 模板即可
```yaml
apiVersion: v1
kind: Service
metadata:
  name: mysql
spec:
  externalName: {{.Values.mysql.address}}
  sessionAffinity: None
  type: ExternalName
```
清空 external-services/values.yaml 无关配置，提供一个 mysql.address 设置即可。
<a name="Wb2Kx"></a>
## 集群配置
完成集群的基础设施配置后，就可以开始完成整个集群的配置资源了。配置资源，同样通过一个专门的 Helm charts 进行管理。
```bash
# 创建一个新的 Helm Charts, 负责管理集群配置
$ helm create settings
```
删除无关的资源模板。开始添加集群配置。通常而言，集群配置也就以下几种配置类型：

- 证书类配置
- 密码类配置
- 容器镜像访问认证配置
- 通用业务服务配置

以上配置，在 K8S 集群中，可以通过 Secret 资源和 ConfigMap 资源进行配置。
<a name="CwQNX"></a>
### 证书类配置
证书类配置，主要是将证书文件自动化配置到集群中，可以通过 Helm 提供模板函数 .Files.Get 读取文件内容. 以常见的域名证书为例：
```yaml
apiVersion: v1
data:
  tls.crt: "{{ .Files.Get "files/example.com.pem" | b64enc }}"
  tls.key: "{{ .Files.Get "files/example.com.key" | b64enc }}"
kind: Secret
metadata:
  name: www-example-com-tls
type: IngressTLS
```
将证书文件存放到 settings/files 子目录下即可。
<a name="xZUUa"></a>
### 密码类配置
密码类配置同样非常简单，如：
```yaml
apiVersion: v1
data:
  ACCESS: {{ .Values.demo.access | b64enc }}
  SECRET: {{ .Values.demo.secret | b64enc }}
kind: Secret
metadata:
  name: demo-secret
type: Opaque
```
通过将具体的密码信息设置在 settings/values.yaml 即可。
<a name="yTS5A"></a>
### 容器镜像访问认证配置
为了让集群在发布服务时，可以取得相应的服务容器镜像，还需要提供服务容器镜像服务器的认证信息。通常，镜像服务器的认证信息可以通过命令行创建。如果需要固化存储，在将命令行创建的认证信息资源下载到本地保存即可。
```bash
# 可以通过命令帮助查询如何创建一个镜像服务的认证信息
$ kubectl create secret docker-registry -h
```
<a name="qn887"></a>
### 通用服务配置
通用服务配置，通常是一组业务服务可共用的配置，敏感度不高，则可以通过 ConfigMap 资源的形式进行配置。
```yaml
apiVersion: v1
kind: ConfigMap
metadata:
  name: your-config
data:
  [yourKey1]: {{.values.yourValue1}}
  [yourKey2]: {{.values.yourValue2}}
  [yourKey3]: {{.values.yourValue3}}
```
非常简单。<br />完成以上各类配置后，直接在 settings 中统一管理，就非常清楚。
<a name="JId09"></a>
## 业务服务
业务服务不同于配置，类似与自建基础设施服务，通常需要提供的部署资源可能包括：

- deployment 或 statefulset
- service
- ingress
- configmap 仅针对自己的配置
- secret 仅针对自己的密钥

所以，业务服务的资源定义类型就会比较多。简单列一下常规服务的配置：

- StatefulSet 资源配置
```yaml
apiVersion: apps/v1
kind: StatefulSet
metadata:
  name: demo
  labels:
    app: demo
spec:
  serviceName: demo
  replicas: {{ .Values.deployment.replicaCount }}
  selector:
    matchLabels:
      app: demo
  template:
    metadata:
      annotations:
        timstamp: "{{ date "20060102150405" .Release.Time }}"
      labels:
        app: demo
    spec:
      {{- with .Values.imagePullSecrets }}
      imagePullSecrets:
            {{- toYaml . | nindent 8 }}
      {{- end }}
      containers:
        - name: demo
          env:
            - name: DEMO_CONFIG
              valueFrom:
                configMapKeyRef:
                  key: CONFIG
                  name: demo-config
            - name: DEMO_SECRET
              valueFrom:
                secretKeyRef:
                  key: SECRET
                  name: demo-secret
          ports:
            - name: http
              containerPort: 8080
          image: "{{ .Values.deployment.image.name }}:{{ .Values.deployment.image.tag }}"
          imagePullPolicy: {{ .Values.deployment.image.imagePullPolicy }}
          command: ["demo", "http"]
```
提供的 values.yaml 配置:
```yaml
deployment:
  replicaCount: 1
  image:
    name: "your.image.registry/group/image.name"
    tag: latest
    imagePullPolicy: Always

imagePullSecrets:
  - name: "image-pull-secret-1"
  - name: "image-pull-secret-2"
```

- Service 配置
```yaml
apiVersion: v1
kind: Service
metadata:
  name: demo
  labels:
    app: demo
spec:
  type: ClusterIP
  ports:
    - name: demo
      port: 80
      targetPort: http
  selector:
    app: demo
```

- Ingress 配置
```yaml
apiVersion: networking.k8s.io/v1
kind: Ingress
metadata:
  name: demo
spec:
  tls:
    - hosts:
        - {{.Values.ingress.host.name}}
      secretName: {{.Values.ingress.tls.secretName}}
  rules:
    - host: {{.Values.ingress.host.name}}
      http:
        paths:
          - pathType: Prefix
            path: /v1/api/demo
            backend:
              service:
                name: demo
                port:
                  number: 80
```
<a name="JTbRN"></a>
## 总结
以上就是通过 Helm 的方式来管理 K8S 常规资源的分类管理与部署。如果，还想进一步完成资源的规范化管理，还可以自建 Helm Charts 的资源索引服务。将所有业务服务的 Helm Chart 进行打包管理。具体命令参考 `helm package`
> 作者：liujianping
> 出处：[https://goo.gs/9bds8](https://goo.gs/9bds8)

