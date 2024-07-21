Kubernetes<br />原文：[https://nonamesecurity.com/blog/the-new-way-to-expose-apis-in-kubernetes-cluster](https://nonamesecurity.com/blog/the-new-way-to-expose-apis-in-kubernetes-cluster)<br />Ingress 是 Kubernetes 中使用最广泛的资源之一。该组件负责基础设施和应用程序，并有助于将应用程序和服务暴露到集群外。然而，Kubernetes 网络技术已经有了长足的发展，许多新的用例很快暴露了 Ingress 的局限性。<br />Ingress API 没有提供用户想要定义的高级负载均衡功能，而且用户管理起来相当不切实际。供应商试图通过使用 annotations 来解决这些额外的用例以扩展平台，但这导致供应商之间的实现不一致。<br />[https://kubernetes.github.io/ingress-nginx/user-guide/nginx-configuration/annotations/](https://kubernetes.github.io/ingress-nginx/user-guide/nginx-configuration/annotations/)<br />因此，Gateway API 创建了一个新资源和 API 的集合，这些资源和 API 会改进并可能在未来取代 Ingress。<br />Gateway API 将 Ingress 功能分为 3 个组件，每个组件负责自己的部分，如下所述。此外，Gateway API 添加了其他有价值的功能。许多 Gateway API 的下游实施者和集成商都提供了公共预览版，并且正在快速迭代。<br />[https://gateway-api.sigs.k8s.io/implementations/](https://gateway-api.sigs.k8s.io/implementations/)<br />除了这个 Envoy 还宣布了他们的 Envoy Gateway 来标准化 Kubernetes Ingress。<br />[https://blog.envoyproxy.io/introducing-envoy-gateway-ad385cc59532](https://blog.envoyproxy.io/introducing-envoy-gateway-ad385cc59532)
<a name="ZtIbB"></a>
## 旧方法 - Ingress
使用安全最佳实践是通过 Ingress 公开 API，您应该配置了 ingress 的服务，然后再连接到负载均衡器。这意味着 Ingress 包括入口控制器详细信息（由平台管理员配置）和应用程序路由（由开发人员配置）。
<a name="jyYYY"></a>
## 新方式 - Gateway API
<a name="L8DeY"></a>
### 架构
![GatewayClass -> Gateway -> [*]Route](https://cdn.nlark.com/yuque/0/2022/png/396745/1657155727045-826ffe44-c9f9-4f1a-9b65-4e386c771f6d.png#clientId=u0ef0cf95-daf8-4&from=paste&id=u33d11096&originHeight=643&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u3bb38c84-391b-4e96-8ddc-508dd1e6d5e&title=GatewayClass%20-%3E%20Gateway%20-%3E%20%5B%2A%5DRoute "GatewayClass -> Gateway -> [*]Route")<br />GatewayClass 由平台/基础设施提供商提供，例如 Istio 或 Google Cloud。<br />Gateway 是由平台/集群管理员定义的 gatewayClass 的一个实例，它将网关绑定到 LoadBalancer。<br />*Route 用于将路由绑定到网关（可以将多个路由配置到一个网关）。在 TCP/IP 层中可用：

- HTTPRoute - 可以路由 HTTP 和 HTTPS
- TLSRoute - 根据 TLS 层中的参数和 SNI（服务器名称指示）路由 TLS 和路由
- TCPRoute - 基于 TCP 目标端口的路由
- UDPRoute - 基于 UDP 目标端口的路由
<a name="ubvDt"></a>
### 优点
使用新方式的优点：<br />网关 API 增加了新功能：

- 基于 HTTP 标头的匹配
- HTTP 标头操作
- 加权流量拆分
- 流量镜像
- 面向角色的资源模型

Gateway API 还支持扩展：

- 路由到其他协议
- 任意后端 CRD，例如桶、函数等。
- 自定义参数和配置（LB 算法、自定义匹配）
- 新方法将基础架构与应用程序解耦
<a name="KK6wB"></a>
## 展望未来
Gateway API 支持更复杂的 LB 功能，例如加权流量拆分和基于 HTTP 标头的匹配和操作。该功能仍在开发中，许多供应商已开始提供实验性支持，因此预计网关 API 将稳步向前发展。
