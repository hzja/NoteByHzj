IstioKubernetes<br />Istio，它是服务网格的一种具体实现。在此过程中，将介绍Istio的核心架构。
<a name="DwEhe"></a>
## 什么是服务网络
在过去的几十年中，已经看到了单体应用程序开始拆分为较小的应用程序。此外，诸如Docker之类的容器化技术和诸如Kubernetes之类的编排系统加速了这一变化。<br />尽管在像Kubernetes这样的分布式系统上采用微服务架构有许多优势，但它也具有相当的复杂性。由于分布式服务必须相互通信，因此必须考虑发现，路由，重试和故障转移。<br />还有其他一些问题，例如安全性和可观察性，还必须注意以下问题：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643015954723-955afad1-2ac1-4f61-8f46-e3fb501b9f8c.webp#clientId=u8667b80a-4d56-4&from=paste&id=u1b28c5d4&originHeight=366&originWidth=720&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0d4122da-1ce4-49e1-a39e-f1fa7847203&title=)<br />现在，在每个服务中建立这些通信功能可能非常繁琐，尤其是当服务范围扩大且通信变得复杂时，更是如此。这正是服务网格可以提供帮助的地方。基本上，服务网格消除了在分布式软件系统中管理所有服务到服务通信的责任。<br />服务网格能够通过一组网络代理来做到这一点。本质上，服务之间的请求是通过与服务一起运行但位于基础结构层之外的代理路由的：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643015954716-66584c52-2927-4834-ab50-0580b6d09e96.webp#clientId=u8667b80a-4d56-4&from=paste&id=u59d14de7&originHeight=383&originWidth=720&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u86b9351e-a24b-4f5b-b513-d1c923af17f&title=)<br />这些代理基本上为服务创建了一个网状网络——因此得名为服务网格！通过这些代理，服务网格能够控制服务到服务通信的各个方面。这样，可以使用它来解决分布式计算的八个谬误，这是一组断言，描述了经常对分布式应用程序做出的错误假设。
<a name="xg8P9"></a>
## 服务网格的特征
现在，了解服务网格可以提供的一些功能。请注意，实际功能列表取决于服务网格的实现。但是，总的来说，应该在所有实现中都期望其中大多数功能。<br />可以将这些功能大致分为三类：流量管理，安全性和可观察性。
<a name="seWDH"></a>
### 流量管理
服务网格的基本特征之一是流量管理。这包括动态服务发现和路由。尤其影子流量和流量拆分功能，这些对于实现金丝雀发布和A/B测试非常有用。<br />由于所有服务之间的通信都是由服务网格处理的，因此它还启用了一些可靠性功能。例如，服务网格可以提供重试，超时，速率限制和断路器。这些现成的故障恢复功能使通信更加可靠。
<a name="EqWZp"></a>
### 安全性
服务网格通常还处理服务到服务通信的安全性方面。这包括通过双向TLS（mTLS）强制进行流量加密，通过证书验证提供身份验证以及通过访问策略确保授权。<br />服务网格中还可能存在一些有趣的安全用例。例如，可以实现网络分段，从而允许某些服务进行通信而禁止其他服务。而且，服务网格可以为审核需求提供精确的历史信息。
<a name="KJ9gP"></a>
### 可观察性
强大的可观察性是处理分布式系统复杂性的基本要求。由于服务网格可以处理所有通信，因此正确放置了它可以提供可观察性的功能。例如，它可以提供有关分布式追踪的信息。<br />服务网格可以生成许多指标，例如延迟，流量，错误和饱和度。此外，服务网格还可以生成访问日志，为每个请求提供完整记录。这些对于理解单个服务以及整个系统的行为非常有用。
<a name="CNpXq"></a>
## Istio 简介
Istio是最初由IBM，Google和Lyft开发的服务网格的开源实现。它可以透明地分层到分布式应用程序上，并提供服务网格的所有优点，例如流量管理，安全性和可观察性。<br />它旨在与各种部署配合使用，例如本地部署，云托管，Kubernetes容器以及虚拟机上运行的服务程序。尽管Istio与平台无关，但它经常与Kubernetes平台上部署的微服务一起使用。<br />从根本上讲，Istio的工作原理是以Sidcar的形式将Envoy的扩展版本作为代理布署到每个微服务中：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643015955108-4eb550ab-0e96-4972-bc68-0e25d836e72e.webp#clientId=u8667b80a-4d56-4&from=paste&id=uebae5f22&originHeight=219&originWidth=720&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9a62a308-c045-4fdd-95ee-dd6855ae2c9&title=)<br />该代理网络构成了Istio架构的数据平面。这些代理的配置和管理是从控制平面完成的：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643015955116-cf7a2ec0-2b49-447d-a48a-922b4a850d69.webp#clientId=u8667b80a-4d56-4&from=paste&id=u86f59a30&originHeight=360&originWidth=720&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6e29f877-2a60-4221-8c11-220a1badb39&title=)<br />控制平面基本上是服务网格的大脑。它为数据平面中的Envoy代理提供发现，配置和证书管理。<br />当然，只有在拥有大量相互通信的微服务时，才能体现Istio的优势。在这里，sidecar代理在专用的基础架构层中形成一个复杂的服务网格：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643015955133-db652af4-21d1-4fd3-b7e2-9a8e87ae1a6d.webp#clientId=u8667b80a-4d56-4&from=paste&id=u369c18ce&originHeight=504&originWidth=720&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u51b6fa31-cb4f-4fcf-bba5-bd1b6a19d7a&title=)<br />Istio在与外部库和平台集成方面非常灵活。例如，可以将Istio与外部日志记录平台，遥测或策略系统集成。
<a name="LJu3U"></a>
## 了解 Istio 组件
已经看到，Istio体系结构由数据平面和控制平面组成。此外，还有几个使Istio起作用的核心组件。<br />在本节中，将详细介绍这些核心组件。
<a name="C4uhF"></a>
### 数据平面
Istio的数据平面主要包括Envoy代理的扩展版本。Envoy是一个开源边缘和服务代理，可帮助将网络问题与底层应用程序分离开来。应用程序仅向localhost发送消息或从localhost接收消息，而无需了解网络拓扑。<br />Envoy的核心是在OSI模型的L3和L4层运行的网络代理。它通过使用可插入网络过滤器链来执行连接处理。此外，Envoy支持用于基于HTTP的流量的附加L7层过滤器。而且，Envoy对HTTP/2和gRPC传输具有一流的支持。<br />Istio作为服务网格提供的许多功能实际上是由Envoy代理的基础内置功能启用的：

- 流量控制：Envoy通过HTTP，gRPC，WebSocket和TCP流量的丰富路由规则启用细粒度的流量控制应用
- 网络弹性：Envoy包括对自动重试，断路和故障注入的开箱即用支持
- 安全性：Envoy还可以实施安全策略，并对基础服务之间的通信应用访问控制和速率限制

Envoy在Istio上表现出色的另一个原因之一是它的可扩展性。Envoy提供了基于WebAssembly的可插拔扩展模型。这在定制策略执行和遥测生成中非常有用。此外，还可以使用基于Proxy-Wasm沙箱API的Istio扩展在Istio中扩展Envoy代理。
<a name="Fwxxf"></a>
### 控制面
如上所述，控制平面负责管理和配置数据平面中的Envoy代理。在Istio架构中，控制面核心组件是istiod，Istiod负责将高级路由规则和流量控制行为转换为特定于Envoy的配置，并在运行时将其传播到Sidercar。<br />如果回顾一下Istio控制平面的架构，将会注意到它曾经是一组相互协作的独立组件。它包括诸如用于服务发现的Pilot，用于配置的Galley，用于证书生成的Citadel以及用于可扩展性的Mixer之类的组件。由于复杂性，这些单独的组件被合并为一个称为istiod的单个组件。<br />从根本上来说，istiod仍使用与先前各个组件相同的代码和API。例如，Pilot负责抽象特定于平台的服务发现机制，并将其合成为Sidecar可以使用的标准格式。因此，Istio可以支持针对多个环境（例如Kubernetes或虚拟机）的发现。<br />此外，istiod还提供安全性，通过内置的身份和凭据管理实现强大的服务到服务和最终用户身份验证。此外，借助istiod，可以基于服务身份来实施安全策略。该过程也充当证书颁发机构（CA）并生成证书，以促进数据平面中的相互TLS（MTLS）通信。
<a name="wtM3j"></a>
## Istio 工作原理
已经了解了服务网格的典型特征是什么。此外，介绍了Istio架构及其核心组件的基础。现在，是时候了解Istio如何通过其架构中的核心组件提供这些功能了。<br />将专注于之前经历过的相同类别的功能。
<a name="QdZKV"></a>
### 流量管理
可以使用Istio流量管理API对服务网格中的流量进行精细控制。可以使用这些API将自己的流量配置添加到Istio。此外，可以使用Kubernetes自定义资源定义（CRD）定义API资源。帮助控制流量路由的关键API资源是虚拟服务和目标规则：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643015955401-39a5f548-be8e-42ba-a64c-9cfd1d0b7c9e.webp#clientId=u8667b80a-4d56-4&from=paste&id=u77e7ce54&originHeight=228&originWidth=720&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7a759721-97b7-4e2f-b3a1-dc124d6584e&title=)<br />基本上，虚拟服务可以配置如何将请求路由到Istio服务网格中的服务。因此，虚拟服务由一个或多个按顺序评估的路由规则组成。评估虚拟服务的路由规则后，将应用目标规则。目标规则有助于控制到达目标的流量，例如，按版本对服务实例进行分组。
<a name="hSmkO"></a>
### 安全性
Istio为每个服务提供身份。与每个Envoy代理一起运行的Istio代理与istiod一起使用以自动进行密钥和证书轮换：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643015955448-04c19fd8-eeac-4d4d-b78f-8770bdf51cbf.webp#clientId=u8667b80a-4d56-4&from=paste&id=u1474707e&originHeight=401&originWidth=634&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ueb7c1005-270a-4452-a911-4c46283d6d4&title=)<br />Istio提供两种身份验证——对等身份验证和请求身份验证。对等身份验证用于服务到服务的身份验证，其中Istio提供双向TLS作为全栈解决方案。请求身份验证用于最终用户身份验证，其中Istio使用自定义身份验证提供程序或OpenID Connect（OIDC）提供程序提供JSON Web令牌（JWT）验证。<br />Istio还允许通过简单地将授权策略应用于服务来实施对服务的访问控制。授权策略对Envoy代理中的入站流量实施访问控制。这样，就可以在各种级别上应用访问控制：网格，命名空间和服务范围。
<a name="huTlh"></a>
### 可观察性
Istio为网格网络内的所有服务通信生成详细的遥测，例如度量，分布式跟踪和访问日志。Istio生成一组丰富的代理级指标，面向服务的指标和控制平面指标。<br />之前，Istio遥测体系结构将Mixer作为核心组件。但是从Telemetry v2开始，混音器提供的功能已替换为Envoy代理插件：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643015955438-cf8824e7-d4e4-46cd-8040-0537c8f973ee.webp#clientId=u8667b80a-4d56-4&from=paste&id=u162fc157&originHeight=348&originWidth=720&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u38a3279f-c87b-44a8-a4e1-8259e3b8dac&title=)<br />此外，Istio通过Envoy代理生成分布式跟踪。Istio支持许多跟踪后端，例如Zipkin，Jaeger，Lightstep和Datadog。还可以控制跟踪速率的采样率。此外，Istio还以一组可配置的格式生成服务流量的访问日志。
<a name="IMcqq"></a>
## Istio 实战
上面已经讲述了Istio原理和架构，接下来开始实战部分。首先，将在Kubernetes集群中安装Istio。此外，将使用一个简单的基于微服务的应用程序来演示Istio在Kubernetes上的功能。
<a name="QDX0H"></a>
### 安装
有多种安装Istio的方法，但最简单的方法是下载并解压缩特定操作系统（例如Windows）的最新版本。提取的软件包在bin目录中包含istioctl客户端二进制文件。可以使用istioctl在目标Kubernetes集群上安装Istio：
```bash
istioctl install --set profile=demo -y
```
这会使用演示配置文件将Istio组件安装在默认的Kubernetes集群上。还可以使用任何其他特定于供应商的配置文件来代替演示。<br />最后，当在此Kubernetes集群上部署任何应用程序时，需要指示Istio自动注入Envoy sidecar代理：
```bash
kubectl label namespace default istio-injection=enabled
```
在这里使用kubectl的前提是，机器上已经有像Minikube这样的Kubernetes集群和Kubernetes CLI kubectl。
<a name="y4HT7"></a>
### 示例应用
为了演示，将想象一个非常简单的在线下订单应用程序。该应用程序包含三个微服务，它们相互交互以满足最终用户的订购请求：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643015955675-fbb17314-e01d-4203-bb1c-2207880ba3d5.webp#clientId=u8667b80a-4d56-4&from=paste&id=ubf1879a7&originHeight=273&originWidth=680&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u57dc495a-7e50-4515-beb3-c6e5b181695&title=)<br />没有讨论这些微服务的细节，但是使用Spring Boot和REST API可以很简单地创建它们。最重要的是，为这些微服务创建了一个Docker镜像，以便可以将它们部署在Kubernetes上。
<a name="pGKrM"></a>
### 部署
在像Minikube这样的Kubernetes集群上部署容器化的工作负载非常简单。将使用Deployment和Service资源类型来声明和访问工作负载。通常，在YAML文件中定义它们：
```yaml
apiVersion: apps/v1beta1
kind: Deployment
metadata:
  name: order-service
  namespace: default
spec:
  replicas: 1
  template:
    metadata:
      labels:
        app: order-service
        version: v1
    spec:
      containers:
      - name: order-service
        image: kchandrakant/order-service:v1
        resources:
          requests:
            cpu: 0.1
            memory: 200
---
apiVersion: v1
kind: Service
metadata:
  name: order-service
spec:
  ports:
  - port: 80
    targetPort: 80
    protocol: TCP
    name: http
  selector:
    app: order-service
```
对于订单服务的“部署和服务”，这是一个非常简单的定义。同样，可以为库存服务和运输服务定义YAML文件。<br />使用kubectl部署这些资源也非常简单：
```bash
kubectl apply -f booking-service.yaml -f inventory-service.yaml -f shipping-service.yaml
```
由于已经为默认命名空间启用了自动注入Envoy sidecar代理，因此一切都会由istiod来处理。或者，可以使用istioctl的kube-inject命令手动注入Envoy sidecar代理。
<a name="uWVgr"></a>
### 访问应用
现在，Istio主要负责处理所有的网状网络流量。因此，默认情况下，不允许进出网格的任何流量。Istio使用网关来管理来自网格的入站和出站流量。这样，可以精确地控制进入或离开网格的流量。Istio提供了一些预配置的网关代理部署：istio-ingressgateway和istio-egressgateway。<br />为应用程序创建一个网关和一个虚拟服务来实现此目的：
```yaml
apiVersion: networking.istio.io/v1alpha3
kind: Gateway
metadata:
  name: booking-gateway
spec:
  selector:
    istio: ingressgateway
  servers:
  - port:
      number: 80
      name: http
      protocol: HTTP
    hosts:
    - "*"
---
apiVersion: networking.istio.io/v1alpha3
kind: VirtualService
metadata:
  name: booking
spec:
  hosts:
  - "*"
  gateways:
  - booking-gateway
  http:
  - match:
    - uri:
        prefix: /api/v1/booking
    route:
    - destination:
        host: booking-service
        port:
          number: 8080
```
在这里，利用了Istio提供的默认入口控制器。此外，已经定义了一个虚拟服务，将请求路由到预订服务。<br />同样，也可以为来自网格的出站流量定义出口网关。
<a name="nocMN"></a>
## Istio 的常见用例
现在，已经看到了如何使用Istio在Kubernetes上部署一个简单的应用程序。但是，仍然没有利用Istio启用的任何有趣功能。下面介绍服务网格的一些常见用例，并了解如何使用Istio为简单应用程序实现它们。
<a name="vNCUz"></a>
### 请求路由
可能要以特定方式处理请求路由的原因有多个。例如，可能会部署微服务的多个版本，例如运输服务，并希望仅将一小部分请求路由到新版本。<br />可以使用虚拟服务的路由规则来实现这一点：
```yaml
apiVersion: networking.istio.io/v1alpha3
apiVersion: networking.istio.io/v1alpha3
kind: VirtualService
metadata:
  name: shipping-service
spec:
  hosts:
    - shipping-service
  http:
  - route:
    - destination:
        host: shipping-service
        subset: v1
      weight: 90
    - destination:
        host: shipping-service
        subset: v2
      weight: 10
---
apiVersion: networking.istio.io/v1alpha3
kind: DestinationRule
metadata:
  name: shipping-service
spec:
  host: shipping-service
  subsets:
  - name: v1
    labels:
      version: v1
  - name: v2
    labels:
      version: v2
```
路由规则还允许基于诸如header参数之类的属性来定义匹配条件。此外，目的地字段指定与条件匹配的流量的实际目的地。
<a name="NCv5k"></a>
### 熔断
熔断器基本上是一种软件设计模式，用于检测故障并封装防止故障进一步级联的逻辑。这有助于创建有弹性的微服务应用程序，以限制故障和延迟尖峰的影响。<br />在Istio中，可以使用DestinationRule中的trafficPolicy配置在调用诸如清单服务之类的服务时应用熔断：
```yaml
apiVersion: networking.istio.io/v1alpha3
kind: DestinationRule
metadata:
  name: inventory-service
spec:
  host: inventory-service
  trafficPolicy:
    connectionPool:
      tcp:
        maxConnections: 1
      http:
        http1MaxPendingRequests: 1
        maxRequestsPerConnection: 1
    outlierDetection:
      consecutive5xxErrors: 1
      interval: 1s
      baseEjectionTime: 3m
      maxEjectionPercent: 100
```
在这里，将DestinationRule配置为maxConnections为1，httpMaxPendingRequests为1，maxRequestsPerConnection为1。这实际上意味着，如果将并发请求数超过1，熔断器将开始trap一些请求。
<a name="BxDXi"></a>
### 启用双向 TLS
双向身份验证是指双方在诸如TLS之类的身份验证协议中同时相互进行身份验证的情况。默认情况下，具有代理的服务之间的所有流量在Istio中都使用相互TLS。但是，没有代理的服务仍继续以纯文本格式接收流量。<br />虽然Istio将具有代理的服务之间的所有流量自动升级为双向TLS，但这些服务仍可以接收纯文本流量。可以选择使用PeerAuthentication策略在整个网格范围内实施双向TLS：
```yaml
apiVersion: "security.istio.io/v1beta1"
kind: "PeerAuthentication"
metadata:
  name: "default"
  namespace: "istio-system"
spec:
  mtls:
    mode: STRICT
```
还提供了对每个命名空间或服务而不是在网格范围内强制实施双向TLS的选项。但是，特定于服务的PeerAuthentication策略优先于命名空间范围的策略。
<a name="sCwe5"></a>
### 使用JWT进行访问控制
JSON Web令牌（JWT）是用于创建数据的标准，该数据的有效载荷中包含声明许多声明的JSON。为了在身份提供者和服务提供者之间传递经过身份验证的用户的身份和标准或自定义声明，这一点已被广泛接受。<br />可以在Istio中启用授权策略，以允许访问基于JWT的预订服务之类的服务：
```yaml
apiVersion: security.istio.io/v1beta1
kind: AuthorizationPolicy
metadata:
  name: require-jwt
  namespace: default
spec:
  selector:
    matchLabels:
      app: booking-service
  action: ALLOW
  rules:
  - from:
    - source:
       requestPrincipals: ["testing@baeldung.com/testing@baeldung.io"]
```
在这里，AuthorizationPolicy强制所有请求具有有效的JWT，并将requestPrincipal设置为特定值。Istio通过组合声明JWT的iss和sub来创建requestPrincipal属性。
<a name="npXZb"></a>
## 思考
因此，到目前为止，已经看到像Istio这样的服务网格如何更轻松地处理诸如微服务之类的分布式架构中的许多常见问题。但是尽管如此，Istio还是一个复杂的系统，会增加最终部署的复杂性。与其他所有技术一样，Istio并非灵丹妙药，必须谨慎使用。
<a name="FPiF3"></a>
### 应该始终使用服务网格吗？
尽管已经看到了使用服务网格的足够理由，但下面列举了一些可能促使不使用它的原因：

- 服务网格处理所有服务到服务的通信，而部署和操作服务网格则需要支付额外的费用。对于较简单的应用程序，这可能是不合理的
- 由于已经习惯于处理一些此类问题，例如应用程序代码中的熔断，因此可能导致服务网格中的重复处理
- 越来越依赖于诸如服务网格之类的外部系统可能会损害应用程序的可移植性，尤其是因为没有针对服务网格的行业标准
- 由于服务网格通常通过拦截通过代理的网格流量来工作，因此它可能会给请求增加不希望的延迟
- 服务网格增加了许多其他组件和配置，需要精确处理。这需要专业知识，并增加了学习曲线
- 最后，可能最终将操作逻辑（应在服务网格中存在）与业务逻辑（不应在服务网格中）混合在一起

因此，正如看到的，服务网格的故事不仅仅涉及好处，但这并不意味着它们不是真的。重要的是要仔细评估需求和应用程序的复杂性，然后权衡服务网格的好处和它们所增加的复杂性。
<a name="BK0jg"></a>
### Istio的替代品有哪些？
尽管Istio非常受欢迎，并得到了业内一些领导者的支持，但它当然不是唯一的选择。尽管在这里无法进行全面的比较，看一下Linkerd和Consul这两个选项。<br />Linkerd是已为Kubernetes平台创建的开源服务网格。它也很受欢迎，目前在CNCF中具有孵化项目的地位。它的工作原理类似于Istio等任何其他服务网格。它还利用TCP代理来处理网格流量。Linkerd使用用Rust编写的微型代理，称为Linkerd代理。<br />总体而言，Linkerd并不比Istio复杂，因为它仅支持Kubernetes。但是，除此之外，Linkerd中可用的功能列表与Istio中可用的功能非常相似。Linkerd的核心架构也非常类似于Istio。基本上，Linkerd包含三个主要组件：用户界面，数据平面和控制平面。<br />Consul是HashiCorp的服务网格的开源实现。它的好处是可以与HashiCorp的其他基础架构管理产品套件很好地集成，以提供更广泛的功能。Consul中的数据平面可以灵活地支持代理以及本机集成模型。它带有内置代理，但也可以与Envoy一起使用。<br />除了Kubernetes，Consul还可以与Nomad等其他平台一起使用。Consul通过在每个节点上运行Consul代理以执行运行状况检查来工作。这些代理与一台或多台存储和复制数据的Consul服务器通信。尽管它提供了服务网格（如Istio）的所有标准功能，但它是部署和管理的更复杂的系统。
