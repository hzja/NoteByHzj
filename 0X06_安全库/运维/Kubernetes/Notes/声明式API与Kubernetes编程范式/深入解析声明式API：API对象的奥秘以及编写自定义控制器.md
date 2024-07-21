Kubernetes<br />当把一个 YAML 文件提交给 Kubernetes 之后，它究竟是如何创建出一个 API 对象的呢？<br />这得从声明式 API 的设计谈起了。
<a name="dl3hp"></a>
## 声明式 API 的设计
在 Kubernetes 项目中，一个 API 对象在 Etcd 里的完整资源路径，是由：Group（API 组）、Version（API 版本）和 Resource（API 资源类型）三个部分组成的。<br />通过这样的结构，整个 Kubernetes 里的所有 API 对象，实际上就可以用如下的树形结构表示出来：<br />![709700eea03075bed35c25b5b6cdefda[1].png](https://cdn.nlark.com/yuque/0/2022/png/396745/1642468479407-ccb9bbc5-bdb1-4456-a6cd-bfab74b5277c.png#clientId=u46ce19b2-e3b2-4&from=ui&id=uc420ca74&originHeight=783&originWidth=1820&originalType=binary&ratio=1&rotation=0&showTitle=false&size=126670&status=done&style=shadow&taskId=u1cc29eef-690e-4fed-97f6-ad61308ed00&title=)<br />在这幅图中，可以很清楚地看到 Kubernetes 里 API 对象的组织方式，其实是层层递进的。<br />比如，现在要声明要创建一个 CronJob 对象，那么 YAML 文件的开始部分会这么写：
```yaml
apiVersion: batch/v2alpha1
kind: CronJob
...
```
在这个 YAML 文件中，“CronJob”就是这个 API 对象的资源类型（Resource），“batch”就是它的组（Group），v2alpha1 就是它的版本（Version）。<br />当提交了这个 YAML 文件之后，Kubernetes 就会把这个 YAML 文件里描述的内容，转换成 Kubernetes 里的一个 CronJob 对象。<br />那么，Kubernetes 是如何对 Resource、Group 和 Version 进行解析，从而在 Kubernetes 项目里找到 CronJob 对象的定义呢？
<a name="jATV6"></a>
### 首先，Kubernetes 会匹配 API 对象的组。
需要明确的是，对于 Kubernetes 里的核心 API 对象，比如：Pod、Node 等，是不需要 Group 的（即：它们的 Group 是“”）。所以，对于这些 API 对象来说，Kubernetes 会直接在 /api 这个层级进行下一步的匹配过程。<br />而对于 CronJob 等非核心 API 对象来说，Kubernetes 就必须在 /apis 这个层级里查找它对应的 Group，进而根据“batch”这个 Group 的名字，找到 /apis/batch。<br />不难发现，这些 API Group 的分类是以对象功能为依据的，比如 Job 和 CronJob 就都属于“batch” （离线业务）这个 Group。
<a name="vXf5K"></a>
### 然后，Kubernetes 会进一步匹配到 API 对象的版本号。
对于 CronJob 这个 API 对象来说，Kubernetes 在 batch 这个 Group 下，匹配到的版本号就是 v2alpha1。<br />在 Kubernetes 中，同一种 API 对象可以有多个版本，这正是 Kubernetes 进行 API 版本化管理的重要手段。这样，比如在 CronJob 的开发过程中，对于会影响到用户的变更就可以通过升级新版本来处理，从而保证了向后兼容。
<a name="mqk1s"></a>
### 最后，Kubernetes 会匹配 API 对象的资源类型。
在前面匹配到正确的版本之后，Kubernetes 就知道，要创建的原来是一个 /apis/batch/v2alpha1 下的 CronJob 对象。<br />这时候，APIServer 就可以继续创建这个 CronJob 对象了。为了方便理解，总结了一个如下所示流程图来阐述这个创建过程：<br />![df6f1dda45e9a353a051d06c48f0286f[1].png](https://cdn.nlark.com/yuque/0/2022/png/396745/1642468629063-898a0ae0-f3b8-4467-9484-a03822ad127d.png#clientId=u46ce19b2-e3b2-4&from=ui&id=u012e0284&originHeight=748&originWidth=1867&originalType=binary&ratio=1&rotation=0&showTitle=false&size=510552&status=done&style=shadow&taskId=uc2fb48bf-6ee1-4172-9f6f-aed501b7be3&title=)<br />首先，当发起了创建 CronJob 的 POST 请求之后，编写的 YAML 的信息就被提交给了 APIServer。<br />而 APIServer 的第一个功能，就是过滤这个请求，并完成一些前置性的工作，比如授权、超时处理、审计等。<br />然后，请求会进入 MUX 和 Routes 流程。如果编写过 Web Server 的话就会知道，MUX 和 Routes 是 APIServer 完成 URL 和 Handler 绑定的场所。而 APIServer 的 Handler 要做的事情，就是按照刚刚介绍的匹配过程，找到对应的 CronJob 类型定义。<br />接着，APIServer 最重要的职责就来了：根据这个 CronJob 类型定义，使用用户提交的 YAML 文件里的字段，创建一个 CronJob 对象。<br />而在这个过程中，APIServer 会进行一个 Convert 工作，即：把用户提交的 YAML 文件，转换成一个叫作 Super Version 的对象，它正是该 API 资源类型所有版本的字段全集。这样用户提交的不同版本的 YAML 文件，就都可以用这个 Super Version 对象来进行处理了。<br />接下来，APIServer 会先后进行 `Admission()` 和 `Validation()` 操作。比如Admission Controller 和 Initializer，就都属于 Admission 的内容。<br />而 Validation，则负责验证这个对象里的各个字段是否合法。这个被验证过的 API 对象，都保存在了 APIServer 里一个叫作 Registry 的数据结构中。也就是说，只要一个 API 对象的定义能在 Registry 里查到，它就是一个有效的 Kubernetes API 对象。<br />最后，APIServer 会把验证过的 API 对象转换成用户最初提交的版本，进行序列化操作，并调用 Etcd 的 API 把它保存起来。<br />由此可见，声明式 API 对于 Kubernetes 来说非常重要。所以，APIServer 这样一个在其他项目里“平淡无奇”的组件，却成了 Kubernetes 项目的重中之重。它不仅是 Google Borg 设计思想的集中体现，也是 Kubernetes 项目里唯一一个被 Google 公司和 RedHat 公司双重控制、其他势力根本无法参与其中的组件。<br />此外，由于同时要兼顾性能、API 完备性、版本化、向后兼容等很多工程化指标，所以 Kubernetes 团队在 APIServer 项目里大量使用了 Go 语言的代码生成功能，来自动化诸如 Convert、DeepCopy 等与 API 资源相关的操作。这部分自动生成的代码，曾一度占到 Kubernetes 项目总代码的 20%~30%。<br />这也是为何，在过去很长一段时间里，在这样一个极其“复杂”的 APIServer 中，添加一个 Kubernetes 风格的 API 资源类型，是一个非常困难的工作。<br />不过，在 Kubernetes v1.7 之后，这个工作就变得轻松得多了。这，当然得益于一个全新的 API 插件机制：CRD。<br />CRD 的全称是 Custom Resource Definition。顾名思义，它指的就是，允许用户在 Kubernetes 中添加一个跟 Pod、Node 类似的、新的 API 资源类型，即：自定义 API 资源。
<a name="cTC8l"></a>
## 自定义 API 资源
举个例子，现在要为 Kubernetes 添加一个名叫 Network 的 API 资源类型。<br />它的作用是，一旦用户创建一个 Network 对象，那么 Kubernetes 就应该使用这个对象定义的网络参数，调用真实的网络插件，比如 Neutron 项目，为用户创建一个真正的“网络”。这样，将来用户创建的 Pod，就可以声明使用这个“网络”了。<br />这个 Network 对象的 YAML 文件，名叫 example-network.yaml，它的内容如下所示：
```yaml
apiVersion: samplecrd.k8s.io/v1
kind: Network
metadata:
  name: example-network
spec:
  cidr: "192.168.0.0/16"
  gateway: "192.168.0.1"
```
可以看到，要描述“网络”的 API 资源类型是 Network；API 组是samplecrd.k8s.io；API 版本是 v1。<br />那么，Kubernetes 又该如何知道这个 API（samplecrd.k8s.io/v1/network）的存在呢？<br />其实，上面的这个 YAML 文件，就是一个具体的“自定义 API 资源”实例，也叫 CR（Custom Resource）。而为了能够让 Kubernetes 认识这个 CR，就需要让 Kubernetes 明白这个 CR 的宏观定义是什么，也就是 CRD（Custom Resource Definition）。<br />这就好比，想让计算机认识各种兔子的照片，就得先让计算机明白，兔子的普遍定义是什么。比如，兔子“是哺乳动物”“有长耳朵，三瓣嘴”。<br />所以，接下来，就先编写一个 CRD 的 YAML 文件，它的名字叫作 network.yaml，内容如下所示：
```yaml
apiVersion: apiextensions.k8s.io/v1beta1
kind: CustomResourceDefinition
metadata:
  name: networks.samplecrd.k8s.io
spec:
  group: samplecrd.k8s.io
  version: v1
  names:
    kind: Network
    plural: networks
  scope: Namespaced
```
可以看到，在这个 CRD 中，指定了“group: samplecrd.k8s.io”“version: v1”这样的 API 信息，也指定了这个 CR 的资源类型叫作 Network，复数（plural）是 networks。<br />然后，还声明了它的 scope 是 Namespaced，即：定义的这个 Network 是一个属于 Namespace 的对象，类似于 Pod。<br />这就是一个 Network API 资源类型的 API 部分的宏观定义了。这就等同于告诉了计算机：“兔子是哺乳动物”。所以这时候，Kubernetes 就能够认识和处理所有声明了 API 类型是“samplecrd.k8s.io/v1/network”的 YAML 文件了。<br />接下来，还需要让 Kubernetes“认识”这种 YAML 文件里描述的“网络”部分，比如“cidr”（网段），“gateway”（网关）这些字段的含义。这就相当于要告诉计算机：“兔子有长耳朵和三瓣嘴”。<br />这时候呢，就需要稍微做些代码工作了。<br />首先，要在 GOPATH 下，创建一个结构如下的项目：
:::tips
备注：在这里，假设已经了解了 Golang 的一些基本知识（比如，知道什么是 GOPATH）。而如果还不了解的话，可以在涉及到相关内容时，再去查阅一些相关资料。
:::
```bash
$ tree $GOPATH/src/github.com/<your-name>/k8s-controller-custom-resource
.
├── controller.go
├── crd
│   └── network.yaml
├── example
│   └── example-network.yaml
├── main.go
└── pkg
    └── apis
        └── samplecrd
            ├── register.go
            └── v1
                ├── doc.go
                ├── register.go
                └── types.go
```
其中，pkg/apis/samplecrd 就是 API 组的名字，v1 是版本，而 v1 下面的 types.go 文件里，则定义了 Network 对象的完整描述。<br />然后，在 pkg/apis/samplecrd 目录下创建了一个 register.go 文件，用来放置后面要用到的全局变量。这个文件的内容如下所示：
```go
package samplecrd

const (
 GroupName = "samplecrd.k8s.io"
 Version   = "v1"
)
```
接着，需要在 pkg/apis/samplecrd 目录下添加一个 doc.go 文件（Golang 的文档源文件）。这个文件里的内容如下所示：
```go
// +k8s:deepcopy-gen=package

// +groupName=samplecrd.k8s.io
package v1
```
在这个文件中，可以看到 +[=value]格式的注释，这就是 Kubernetes 进行代码生成要用的 Annotation 风格的注释。<br />其中，+k8s:deepcopy-gen=package 意思是，请为整个 v1 包里的所有类型定义自动生成 DeepCopy 方法；而+groupName=samplecrd.k8s.io，则定义了这个包对应的 API 组的名字。<br />可以看到，这些定义在 doc.go 文件的注释，起到的是全局的代码生成控制的作用，所以也被称为 Global Tags。<br />接下来，需要添加 types.go 文件。顾名思义，它的作用就是定义一个 Network 类型到底有哪些字段（比如，spec 字段里的内容）。这个文件的主要内容如下所示：
```go
package v1
...
// +genclient
// +genclient:noStatus
// +k8s:deepcopy-gen:interfaces=k8s.io/apimachinery/pkg/runtime.Object

// Network describes a Network resource
type Network struct {
    // TypeMeta is the metadata for the resource, like kind and apiversion
    metav1.TypeMeta `json:",inline"`
    // ObjectMeta contains the metadata for the particular object, including
    // things like...
    //  - name
    //  - namespace
    //  - self link
    //  - labels
    //  - ... etc ...
    metav1.ObjectMeta `json:"metadata,omitempty"`

    Spec networkspec `json:"spec"`
}
// networkspec is the spec for a Network resource
type networkspec struct {
    Cidr    string `json:"cidr"`
    Gateway string `json:"gateway"`
}

// +k8s:deepcopy-gen:interfaces=k8s.io/apimachinery/pkg/runtime.Object

// NetworkList is a list of Network resources
type NetworkList struct {
    metav1.TypeMeta `json:",inline"`
    metav1.ListMeta `json:"metadata"`

    Items []Network `json:"items"`
}
```
在上面这部分代码里，可以看到 Network 类型定义方法跟标准的 Kubernetes 对象一样，都包括了 TypeMeta（API 元数据）和 ObjectMeta（对象元数据）字段。<br />而其中的 Spec 字段，就是需要自己定义的部分。所以，在 networkspec 里，定义了 Cidr 和 Gateway 两个字段。其中，每个字段最后面的部分比如json:"cidr"，指的就是这个字段被转换成 JSON 格式之后的名字，也就是 YAML 文件里的字段名字。如果不熟悉这个用法的话，可以查阅一下 Golang 的文档。<br />此外，除了定义 Network 类型，还需要定义一个 NetworkList 类型，用来描述一组 Network 对象应该包括哪些字段。之所以需要这样一个类型，是因为在 Kubernetes 中，获取所有 X 对象的 `List()` 方法，返回值都是List 类型，而不是 X 类型的数组。这是不一样的。<br />同样地，在 Network 和 NetworkList 类型上，也有代码生成注释。<br />其中，+genclient 的意思是：请为下面这个 API 资源类型生成对应的 Client 代码（这个 Client，马上会说到）。而 +genclient:noStatus 的意思是：这个 API 资源类型定义里，没有 Status 字段。否则，生成的 Client 就会自动带上 UpdateStatus 方法。<br />如果类型定义包括了 Status 字段的话，就不需要这句 +genclient:noStatus 注释了。比如下面这个例子：
```go
// +genclient

// Network is a specification for a Network resource
type Network struct {
    metav1.TypeMeta   `json:",inline"`
    metav1.ObjectMeta `json:"metadata,omitempty"`

    Spec   NetworkSpec   `json:"spec"`
    Status NetworkStatus `json:"status"`
}
```
需要注意的是，+genclient 只需要写在 Network 类型上，而不用写在 NetworkList 上。因为 NetworkList 只是一个返回值类型，Network 才是“主类型”。<br />而由于在 Global Tags 里已经定义了为所有类型生成 `DeepCopy` 方法，所以这里就不需要再显式地加上 +k8s:deepcopy-gen=true 了。当然，这也就意味着可以用 +k8s:deepcopy-gen=false 来阻止为某些类型生成 `DeepCopy`。<br />可能已经注意到，在这两个类型上面还有一句+k8s:deepcopy-gen:interfaces=k8s.io/apimachinery/pkg/runtime.Object的注释。它的意思是，请在生成 `DeepCopy` 的时候，实现 Kubernetes 提供的 runtime.Object 接口。否则，在某些版本的 Kubernetes 里，这个类型定义会出现编译错误。这是一个固定的操作，记住即可。<br />或许会有这样的顾虑：这些代码生成注释这么灵活，该怎么掌握呢？<br />其实，上面讲述的内容，已经足以应对 99% 的场景了。当然，如果对代码生成感兴趣的话，推荐阅读[这篇博客](https://cloud.redhat.com/blog/kubernetes-deep-dive-code-generation-customresources)，它详细地介绍了 Kubernetes 的代码生成语法。<br />最后，需要再编写一个 pkg/apis/samplecrd/v1/register.go 文件。<br />在前面对 APIServer 工作原理的讲解中，已经提到，“registry”的作用就是注册一个类型（Type）给 APIServer。其中，Network 资源类型在服务器端注册的工作，APIServer 会自动完成。但与之对应的，还需要让客户端也能“知道”Network 资源类型的定义。这就需要在项目里添加一个 register.go 文件。它最主要的功能，就是定义了如下所示的 `addKnownTypes()` 方法：
```go
package v1
...
// addKnownTypes adds our types to the API scheme by registering
// Network and NetworkList
func addKnownTypes(scheme *runtime.Scheme) error {
    scheme.AddKnownTypes(
        SchemeGroupVersion,
        &Network{},
        &NetworkList{},
    )

    // register the type in the scheme
    metav1.AddToGroupVersion(scheme, SchemeGroupVersion)
    return nil
}
```
有了这个方法，Kubernetes 就能够在后面生成客户端的时候，“知道”Network 以及 NetworkList 类型的定义了。<br />像上面这种 register.go 文件里的内容其实是非常固定的，以后可以直接使用提供的这部分代码做模板，然后把其中的资源类型、GroupName 和 Version 替换成自己的定义即可。<br />这样，Network 对象的定义工作就全部完成了。可以看到，它其实定义了两部分内容：

1. 第一部分是，自定义资源类型的 API 描述，包括：组（Group）、版本（Version）、资源类型（Resource）等。这相当于告诉了计算机：兔子是哺乳动物。
2. 第二部分是，自定义资源类型的对象描述，包括：Spec、Status 等。这相当于告诉了计算机：兔子有长耳朵和三瓣嘴。

接下来，就要使用 Kubernetes 提供的代码生成工具，为上面定义的 Network 资源类型自动生成 clientset、informer 和 lister。其中，clientset 就是操作 Network 对象所需要使用的客户端。<br />这个代码生成工具名叫k8s.io/code-generator，使用方法如下所示：
```bash
# 代码生成的工作目录，也就是我们的项目路径
$ ROOT_PACKAGE="github.com/resouer/k8s-controller-custom-resource"
# API Group
$ CUSTOM_RESOURCE_NAME="samplecrd"
# API Version
$ CUSTOM_RESOURCE_VERSION="v1"

# 安装k8s.io/code-generator
$ go get -u k8s.io/code-generator/...
$ cd $GOPATH/src/k8s.io/code-generator

# 执行代码自动生成，其中pkg/client是生成目标目录，pkg/apis是类型定义目录
$ ./generate-groups.sh all "$ROOT_PACKAGE/pkg/client" "$ROOT_PACKAGE/pkg/apis" "$CUSTOM_RESOURCE_NAME:$CUSTOM_RESOURCE_VERSION"
```
代码生成工作完成之后，再查看一下这个项目的目录结构：
```bash
$ tree
.
├── controller.go
├── crd
│   └── network.yaml
├── example
│   └── example-network.yaml
├── main.go
└── pkg
    ├── apis
    │   └── samplecrd
    │       ├── constants.go
    │       └── v1
    │           ├── doc.go
    │           ├── register.go
    │           ├── types.go
    │           └── zz_generated.deepcopy.go
    └── client
        ├── clientset
        ├── informers
        └── listers
```
其中，pkg/apis/samplecrd/v1 下面的 zz_generated.deepcopy.go 文件，就是自动生成的 `DeepCopy` 代码文件。<br />而整个 client 目录，以及下面的三个包（clientset、informers、 listers），都是 Kubernetes 为 Network 类型生成的客户端库，这些库会在后面编写自定义控制器的时候用到。<br />可以看到，到目前为止的这些工作，其实并不要求写多少代码，主要考验的是“复制、粘贴、替换”这样的“基本功”。<br />而有了这些内容，现在就可以在 Kubernetes 集群里创建一个 Network 类型的 API 对象了。不妨一起来试验下。<br />首先，使用 network.yaml 文件，在 Kubernetes 中创建 Network 对象的 CRD（Custom Resource Definition）：
```bash
$ kubectl apply -f crd/network.yaml
customresourcedefinition.apiextensions.k8s.io/networks.samplecrd.k8s.io created
```
这个操作，就告诉了 Kubernetes，现在要添加一个自定义的 API 对象。而这个对象的 API 信息，正是 network.yaml 里定义的内容。可以通过 `kubectl get` 命令，查看这个 CRD：
```bash
$ kubectl get crd
NAME                        CREATED AT
networks.samplecrd.k8s.io   2018-09-15T10:57:12Z
```
然后，就可以创建一个 Network 对象了，这里用到的是 example-network.yaml：
```bash
$ kubectl apply -f example/example-network.yaml 
network.samplecrd.k8s.io/example-network created
```
通过这个操作，就在 Kubernetes 集群里创建了一个 Network 对象。它的 API 资源路径是samplecrd.k8s.io/v1/networks。<br />这时候，就可以通过 `kubectl get` 命令，查看到新创建的 Network 对象：
```bash
$ kubectl get network
NAME              AGE
example-network   8s
```
还可以通过 `kubectl describe` 命令，看到这个 Network 对象的细节：
```bash
$ kubectl describe network example-network
Name:         example-network
Namespace:    default
Labels:       <none>
...API Version:  samplecrd.k8s.io/v1
Kind:         Network
Metadata:
  ...
  Generation:          1
  Resource Version:    468239
  ...
Spec:
  Cidr:     192.168.0.0/16
  Gateway:  192.168.0.1
```
当然 ，也可以编写更多的 YAML 文件来创建更多的 Network 对象，这和创建 Pod、Deployment 的操作，没有任何区别。
<a name="ywKX8"></a>
## 为 Network 这个自定义 API 对象编写一个自定义控制器（Custom Controller）。
“声明式 API”并不像“命令式 API”那样有着明显的执行逻辑。这就使得基于声明式 API 的业务功能实现，往往需要通过控制器模式来“监视”API 对象的变化（比如，创建或者删除 Network），然后以此来决定实际要执行的具体工作。<br />总得来说，编写自定义控制器代码的过程包括：编写 main 函数、编写自定义控制器的定义，以及编写控制器里的业务逻辑三个部分。<br />首先来编写这个自定义控制器的 main 函数。<br />main 函数的主要工作就是，定义并初始化一个自定义控制器（Custom Controller），然后启动它。这部分代码的主要内容如下所示：
```go
func main() {
  ...

  cfg, err := clientcmd.BuildConfigFromFlags(masterURL, kubeconfig)
  ...
  kubeClient, err := kubernetes.NewForConfig(cfg)
  ...
  networkClient, err := clientset.NewForConfig(cfg)
  ...

  networkInformerFactory := informers.NewSharedInformerFactory(networkClient, ...)

  controller := NewController(kubeClient, networkClient,
                              networkInformerFactory.Samplecrd().V1().Networks())

  go networkInformerFactory.Start(stopCh)

  if err = controller.Run(2, stopCh); err != nil {
    glog.Fatalf("Error running controller: %s", err.Error())
  }
}
```
可以看到，这个 main 函数主要通过三步完成了初始化并启动一个自定义控制器的工作。<br />第一步：main 函数根据提供的 Master 配置（APIServer 的地址端口和 kubeconfig 的路径），创建一个 Kubernetes 的 client（kubeClient）和 Network 对象的 client（networkClient）。<br />但是，如果没有提供 Master 配置呢？<br />这时，main 函数会直接使用一种名叫 InClusterConfig 的方式来创建这个 client。这个方式，会假设自定义控制器是以 Pod 的方式运行在 Kubernetes 集群里的。<br />Kubernetes 里所有的 Pod 都会以 Volume 的方式自动挂载 Kubernetes 的默认 ServiceAccount。所以，这个控制器就会直接使用默认 ServiceAccount 数据卷里的授权信息，来访问 APIServer。<br />第二步：main 函数为 Network 对象创建一个叫作 InformerFactory（即：networkInformerFactory）的工厂，并使用它生成一个 Network 对象的 Informer，传递给控制器。<br />第三步：main 函数启动上述的 Informer，然后执行 `controller.Run`，启动自定义控制器。<br />至此，main 函数就结束了。<br />编写自定义控制器的过程难道就这么简单吗？这个 Informer 又是个什么东西呢？<br />接下来，就详细解释一下这个自定义控制器的工作原理。
<a name="hcSdR"></a>
## 自定义控制器的工作原理
在 Kubernetes 项目中，一个自定义控制器的工作原理，可以用下面这样一幅流程图来表示（在后面的叙述中用“示意图”来指代它）：<br />![自定义控制器的工作流程示意图](https://cdn.nlark.com/yuque/0/2022/png/396745/1642470013564-ce004847-4598-4729-81ac-36ff0cebb2be.png#clientId=u42fa31c6-2519-4&from=ui&id=u4856c2bd&originHeight=822&originWidth=1846&originalType=binary&ratio=1&rotation=0&showTitle=true&size=399944&status=done&style=shadow&taskId=u2c81914c-c60d-4c35-8e57-eaafcff94dc&title=%E8%87%AA%E5%AE%9A%E4%B9%89%E6%8E%A7%E5%88%B6%E5%99%A8%E7%9A%84%E5%B7%A5%E4%BD%9C%E6%B5%81%E7%A8%8B%E7%A4%BA%E6%84%8F%E5%9B%BE "自定义控制器的工作流程示意图")<br />先从这幅示意图的最左边看起。<br />这个控制器要做的第一件事，是从 Kubernetes 的 APIServer 里获取它所关心的对象，也就是定义的 Network 对象。<br />这个操作，依靠的是一个叫作 Informer（可以翻译为：通知器）的代码库完成的。Informer 与 API 对象是一一对应的，所以传递给自定义控制器的，正是一个 Network 对象的 Informer（Network Informer）。<br />创建这个 Informer 工厂的时候，需要给它传递一个 networkClient。<br />事实上，Network Informer 正是使用这个 networkClient，跟 APIServer 建立了连接。不过，真正负责维护这个连接的，则是 Informer 所使用的 Reflector 包。<br />更具体地说，Reflector 使用的是一种叫作 `ListAndWatch` 的方法，来“获取”并“监听”这些 Network 对象实例的变化。<br />在 ListAndWatch 机制下，一旦 APIServer 端有新的 Network 实例被创建、删除或者更新，Reflector 都会收到“事件通知”。这时，该事件及它对应的 API 对象这个组合，就被称为增量（Delta），它会被放进一个 Delta FIFO Queue（即：增量先进先出队列）中。<br />而另一方面，Informe 会不断地从这个 Delta FIFO Queue 里读取（Pop）增量。每拿到一个增量，Informer 就会判断这个增量里的事件类型，然后创建或者更新本地对象的缓存。这个缓存，在 Kubernetes 里一般被叫作 Store。<br />比如，如果事件类型是 Added（添加对象），那么 Informer 就会通过一个叫作 Indexer 的库把这个增量里的 API 对象保存在本地缓存中，并为它创建索引。相反，如果增量的事件类型是 Deleted（删除对象），那么 Informer 就会从本地缓存中删除这个对象。<br />这个同步本地缓存的工作，是 Informer 的第一个职责，也是它最重要的职责。<br />而 Informer 的第二个职责，则是根据这些事件的类型，触发事先注册好的 ResourceEventHandler。这些 Handler，需要在创建控制器的时候注册给它对应的 Informer。<br />接下来，就来编写这个控制器的定义，它的主要内容如下所示：
```go
func NewController(
    kubeclientset kubernetes.Interface,
    networkclientset clientset.Interface,
    networkInformer informers.NetworkInformer) *Controller {
    ...
    controller := &Controller{
        kubeclientset:    kubeclientset,
        networkclientset: networkclientset,
        networksLister:   networkInformer.Lister(),
        networksSynced:   networkInformer.Informer().HasSynced,
        workqueue:        workqueue.NewNamedRateLimitingQueue(...,  "Networks"),
        ...
    }
    networkInformer.Informer().AddEventHandler(cache.ResourceEventHandlerFuncs{
        AddFunc: controller.enqueueNetwork,
        UpdateFunc: func(old, new interface{}) {
            oldNetwork := old.(*samplecrdv1.Network)
            newNetwork := new.(*samplecrdv1.Network)
            if oldNetwork.ResourceVersion == newNetwork.ResourceVersion {
                return
            }
            controller.enqueueNetwork(new)
        },
        DeleteFunc: controller.enqueueNetworkForDelete,
        return controller
    }
```
前面在 main 函数里创建了两个 client（kubeclientset 和 networkclientset），然后在这段代码里，使用这两个 client 和前面创建的 Informer，初始化了自定义控制器。<br />值得注意的是，在这个自定义控制器里，还设置了一个工作队列（work queue），它正是处于示意图中间位置的 WorkQueue。这个工作队列的作用是，负责同步 Informer 和控制循环之间的数据。<br />实际上，Kubernetes 项目提供了很多个工作队列的实现，可以根据需要选择合适的库直接使用。<br />然后，为 networkInformer 注册了三个 Handler（AddFunc、UpdateFunc 和 DeleteFunc），分别对应 API 对象的“添加”“更新”和“删除”事件。而具体的处理操作，都是将该事件对应的 API 对象加入到工作队列中。<br />需要注意的是，实际入队的并不是 API 对象本身，而是它们的 Key，即：该 API 对象的<namespace>/<name>。<br />而后面即将编写的控制循环，则会不断地从这个工作队列里拿到这些 Key，然后开始执行真正的控制逻辑。<br />综合上面的讲述，现在应该就能明白，所谓 Informer，其实就是一个带有本地缓存和索引机制的、可以注册 EventHandler 的 client。它是自定义控制器跟 APIServer 进行数据同步的重要组件。<br />更具体地说，Informer 通过一种叫作 ListAndWatch 的方法，把 APIServer 中的 API 对象缓存在了本地，并负责更新和维护这个缓存。<br />其中，ListAndWatch 方法的含义是：首先，通过 APIServer 的 LIST API“获取”所有最新版本的 API 对象；然后，再通过 WATCH API 来“监听”所有这些 API 对象的变化。<br />而通过监听到的事件变化，Informer 就可以实时地更新本地缓存，并且调用这些事件对应的 EventHandler 了。<br />此外，在这个过程中，每经过 resyncPeriod 指定的时间，Informer 维护的本地缓存，都会使用最近一次 LIST 返回的结果强制更新一次，从而保证缓存的有效性。在 Kubernetes 中，这个缓存强制更新的操作就叫作：resync。<br />需要注意的是，这个定时 `resync` 操作，也会触发 Informer 注册的“更新”事件。但此时，这个“更新”事件对应的 Network 对象实际上并没有发生变化，即：新、旧两个 Network 对象的 ResourceVersion 是一样的。在这种情况下，Informer 就不需要对这个更新事件再做进一步的处理了。<br />这也是为什么在上面的 `UpdateFunc` 方法里，先判断了一下新、旧两个 Network 对象的版本（ResourceVersion）是否发生了变化，然后才开始进行的入队操作。<br />以上，就是 Kubernetes 中的 Informer 库的工作原理了。<br />接下来，就来到了示意图中最后面的控制循环（Control Loop）部分，也正是在 main 函数最后调用 `controller.Run()` 启动的“控制循环”。它的主要内容如下所示：
```go
func (c *Controller) Run(threadiness int, stopCh <-chan struct{}) error {
    ...
    if ok := cache.WaitForCacheSync(stopCh, c.networksSynced); !ok {
        return fmt.Errorf("failed to wait for caches to sync")
    }

    ...
    for i := 0; i < threadiness; i++ {
        go wait.Until(c.runWorker, time.Second, stopCh)
    }

    ...
    return nil
}
```
可以看到，启动控制循环的逻辑非常简单：

- 首先，等待 Informer 完成一次本地缓存的数据同步操作；
- 然后，直接通过 goroutine 启动一个（或者并发启动多个）“无限循环”的任务。

而这个“无限循环”任务的每一个循环周期，执行的正是真正关心的业务逻辑。<br />所以接下来就来编写这个自定义控制器的业务逻辑，它的主要内容如下所示：
```go
func (c *Controller) runWorker() {
    for c.processNextWorkItem() {
    }
}

func (c *Controller) processNextWorkItem() bool {
    obj, shutdown := c.workqueue.Get()

    ...

    err := func(obj interface{}) error {
        ...
        if err := c.syncHandler(key); err != nil {
            return fmt.Errorf("error syncing '%s': %s", key, err.Error())
        }

        c.workqueue.Forget(obj)
        ...
        return nil
    }(obj)

    ...

    return true
}

func (c *Controller) syncHandler(key string) error {

    namespace, name, err := cache.SplitMetaNamespaceKey(key)
    ...

    network, err := c.networksLister.Networks(namespace).Get(name)
    if err != nil {
        if errors.IsNotFound(err) {
            glog.Warningf("Network does not exist in local cache: %s/%s, will delete it from Neutron ...",
                          namespace, name)

            glog.Warningf("Network: %s/%s does not exist in local cache, will delete it from Neutron ...",
                          namespace, name)

            // FIX ME: call Neutron API to delete this network by name.
            //
            // neutron.Delete(namespace, name)

            return nil
        }
        ...

        return err
    }

    glog.Infof("[Neutron] Try to process network: %#v ...", network)

    // FIX ME: Do diff().
    //
    // actualNetwork, exists := neutron.Get(namespace, name)
    //
    // if !exists {
    //   neutron.Create(namespace, name)
    // } else if !reflect.DeepEqual(actualNetwork, network) {
    //   neutron.Update(namespace, name)
    // }

    return nil
}
```
可以看到，在这个执行周期里（processNextWorkItem），首先从工作队列里出队（workqueue.Get）了一个成员，也就是一个 Key（Network 对象的：namespace/name）。<br />然后，在 `syncHandler` 方法中，使用这个 Key，尝试从 Informer 维护的缓存中拿到了它所对应的 Network 对象。<br />可以看到，在这里，使用了 networksLister 来尝试获取这个 Key 对应的 Network 对象。这个操作，其实就是在访问本地缓存的索引。实际上，在 Kubernetes 的源码中，会经常看到控制器从各种 Lister 里获取对象，比如：podLister、nodeLister 等等，它们使用的都是 Informer 和缓存机制。<br />而如果控制循环从缓存中拿不到这个对象（即：networkLister 返回了 IsNotFound 错误），那就意味着这个 Network 对象的 Key 是通过前面的“删除”事件添加进工作队列的。所以，尽管队列里有这个 Key，但是对应的 Network 对象已经被删除了。<br />这时候，就需要调用 Neutron 的 API，把这个 Key 对应的 Neutron 网络从真实的集群里删除掉。<br />而如果能够获取到对应的 Network 对象，就可以执行控制器模式里的对比“期望状态”和“实际状态”的逻辑了。<br />其中，自定义控制器“千辛万苦”拿到的这个 Network 对象，正是 APIServer 里保存的“期望状态”，即：用户通过 YAML 文件提交到 APIServer 里的信息。当然，在例子里，它已经被 Informer 缓存在了本地。<br />那么，“实际状态”又从哪里来呢？<br />当然是来自于实际的集群了。<br />所以，控制循环需要通过 Neutron API 来查询实际的网络情况。<br />比如，可以先通过 Neutron 来查询这个 Network 对象对应的真实网络是否存在。

- 如果不存在，这就是一个典型的“期望状态”与“实际状态”不一致的情形。这时，就需要使用这个 Network 对象里的信息（比如：CIDR 和 Gateway），调用 Neutron API 来创建真实的网络。
- 如果存在，那么，就要读取这个真实网络的信息，判断它是否跟 Network 对象里的信息一致，从而决定是否要通过 Neutron 来更新这个已经存在的真实网络。

这样，就通过对比“期望状态”和“实际状态”的差异，完成了一次调协（Reconcile）的过程。<br />至此，一个完整的自定义 API 对象和它所对应的自定义控制器，就编写完毕了。
<a name="kp9E9"></a>
## 运行自定义控制器
接下来，就来把这个项目运行起来，查看一下它的工作情况。<br />可以自己编译这个项目，也可以直接使用编译好的二进制文件（samplecrd-controller）。编译并启动这个项目的具体流程如下所示：
```bash
# Clone repo
$ git clone https://github.com/resouer/k8s-controller-custom-resource$ cd k8s-controller-custom-resource

### Skip this part if you don't want to build
# Install dependency
$ go get github.com/tools/godep
$ godep restore
# Build
$ go build -o samplecrd-controller .

$ ./samplecrd-controller -kubeconfig=$HOME/.kube/config -alsologtostderr=true
I0915 12:50:29.051349   27159 controller.go:84] Setting up event handlers
I0915 12:50:29.051615   27159 controller.go:113] Starting Network control loop
I0915 12:50:29.051630   27159 controller.go:116] Waiting for informer caches to sync
E0915 12:50:29.066745   27159 reflector.go:134] github.com/resouer/k8s-controller-custom-resource/pkg/client/informers/externalversions/factory.go:117: Failed to list *v1.Network: the server could not find the requested resource (get networks.samplecrd.k8s.io)
...
```
可以看到，自定义控制器被启动后，一开始会报错。<br />这是因为，此时 Network 对象的 CRD 还没有被创建出来，所以 Informer 去 APIServer 里“获取”（List）Network 对象时，并不能找到 Network 这个 API 资源类型的定义，即：
```bash
Failed to list *v1.Network: the server could not find the requested resource (get networks.samplecrd.k8s.io)
```
所以，接下来就需要创建 Network 对象的 CRD。<br />在另一个 shell 窗口里执行：
```bash
$ kubectl apply -f crd/network.yaml
```
这时候，就会看到控制器的日志恢复了正常，控制循环启动成功：
```bash
...
I0915 12:50:29.051630   27159 controller.go:116] Waiting for informer caches to sync
...
I0915 12:52:54.346854   25245 controller.go:121] Starting workers
I0915 12:52:54.346914   25245 controller.go:127] Started workers
```
接下来，就可以进行 Network 对象的增删改查操作了。<br />首先，创建一个 Network 对象：
```bash
$ cat example/example-network.yaml 
apiVersion: samplecrd.k8s.io/v1
kind: Network
metadata:
  name: example-network
spec:
  cidr: "192.168.0.0/16"
  gateway: "192.168.0.1"
  
$ kubectl apply -f example/example-network.yaml 
network.samplecrd.k8s.io/example-network created
```
这时候，查看一下控制器的输出：
```bash
...
I0915 12:50:29.051349   27159 controller.go:84] Setting up event handlers
I0915 12:50:29.051615   27159 controller.go:113] Starting Network control loop
I0915 12:50:29.051630   27159 controller.go:116] Waiting for informer caches to sync
...
I0915 12:52:54.346854   25245 controller.go:121] Starting workers
I0915 12:52:54.346914   25245 controller.go:127] Started workers
I0915 12:53:18.064409   25245 controller.go:229] [Neutron] Try to process network: &v1.Network{TypeMeta:v1.TypeMeta{Kind:"", APIVersion:""}, ObjectMeta:v1.ObjectMeta{Name:"example-network", GenerateName:"", Namespace:"default", ... ResourceVersion:"479015", ... Spec:v1.NetworkSpec{Cidr:"192.168.0.0/16", Gateway:"192.168.0.1"}} ...
I0915 12:53:18.064650   25245 controller.go:183] Successfully synced 'default/example-network'
...
```
可以看到，上面创建 example-network 的操作，触发了 EventHandler 的“添加”事件，从而被放进了工作队列。<br />紧接着，控制循环就从队列里拿到了这个对象，并且打印出了正在“处理”这个 Network 对象的日志。<br />可以看到，这个 Network 的 ResourceVersion，也就是 API 对象的版本号，是 479015，而它的 Spec 字段的内容，跟提交的 YAML 文件一摸一样，比如，它的 CIDR 网段是：192.168.0.0/16。<br />这时候，来修改一下这个 YAML 文件的内容，如下所示：
```bash
$ cat example/example-network.yaml 
apiVersion: samplecrd.k8s.io/v1
kind: Network
metadata:
  name: example-network
spec:
  cidr: "192.168.1.0/16"
  gateway: "192.168.1.1"
```
可以看到，把这个 YAML 文件里的 CIDR 和 Gateway 字段修改成了 192.168.1.0/16 网段。<br />然后，执行了 `kubectl apply` 命令来提交这次更新，如下所示：
```bash
$ kubectl apply -f example/example-network.yaml 
network.samplecrd.k8s.io/example-network configured
```
这时候，就可以观察一下控制器的输出：
```bash
...
I0915 12:53:51.126029   25245 controller.go:229] [Neutron] Try to process network: &v1.Network{TypeMeta:v1.TypeMeta{Kind:"", APIVersion:""}, ObjectMeta:v1.ObjectMeta{Name:"example-network", GenerateName:"", Namespace:"default", ...  ResourceVersion:"479062", ... Spec:v1.NetworkSpec{Cidr:"192.168.1.0/16", Gateway:"192.168.1.1"}} ...
I0915 12:53:51.126348   25245 controller.go:183] Successfully synced 'default/example-network'
```
可以看到，这一次，Informer 注册的“更新”事件被触发，更新后的 Network 对象的 Key 被添加到了工作队列之中。<br />所以，接下来控制循环从工作队列里拿到的 Network 对象，与前一个对象是不同的：它的 ResourceVersion 的值变成了 479062；而 Spec 里的字段，则变成了 192.168.1.0/16 网段。<br />最后，再把这个对象删除掉：
```bash
$ kubectl delete -f example/example-network.yaml
```
这一次，在控制器的输出里，就可以看到，Informer 注册的“删除”事件被触发，并且控制循环“调用”Neutron API“删除”了真实环境里的网络。这个输出如下所示：
```bash
W0915 12:54:09.738464   25245 controller.go:212] Network: default/example-network does not exist in local cache, will delete it from Neutron ...
I0915 12:54:09.738832   25245 controller.go:215] [Neutron] Deleting network: default/example-network ...
I0915 12:54:09.738854   25245 controller.go:183] Successfully synced 'default/example-network'
```
以上，就是编写和使用自定义控制器的全部流程了。<br />实际上，这套流程不仅可以用在自定义 API 资源上，也完全可以用在 Kubernetes 原生的默认 API 对象上。<br />比如，在 main 函数里，除了创建一个 Network Informer 外，还可以初始化一个 Kubernetes 默认 API 对象的 Informer 工厂，比如 Deployment 对象的 Informer。这个具体做法如下所示：
```go
func main() {
  ...

  kubeInformerFactory := kubeinformers.NewSharedInformerFactory(kubeClient, time.Second*30)

  controller := NewController(kubeClient, exampleClient,
                              kubeInformerFactory.Apps().V1().Deployments(),
                              networkInformerFactory.Samplecrd().V1().Networks())

  go kubeInformerFactory.Start(stopCh)
  ...
}
```
在这段代码中，首先使用 Kubernetes 的 client（kubeClient）创建了一个工厂；<br />然后，用跟 Network 类似的处理方法，生成了一个 Deployment Informer；<br />接着，把 Deployment Informer 传递给了自定义控制器；当然，也要调用 `Start` 方法来启动这个 Deployment Informer。<br />而有了这个 Deployment Informer 后，这个控制器也就持有了所有 Deployment 对象的信息。接下来，它既可以通过 `deploymentInformer.Lister()` 来获取 Etcd 里的所有 Deployment 对象，也可以为这个 Deployment Informer 注册具体的 Handler 来。<br />更重要的是，这就使得在这个自定义控制器里面，可以通过对自定义 API 对象和默认 API 对象进行协同，从而实现更加复杂的编排功能。<br />比如：用户每创建一个新的 Deployment，这个自定义控制器，就可以为它创建一个对应的 Network 供它使用。
<a name="oOPHP"></a>
## 总结
所谓的 Informer，就是一个自带缓存和索引机制，可以触发 Handler 的客户端库。这个本地缓存在 Kubernetes 中一般被称为 Store，索引一般被称为 Index。<br />Informer 使用了 Reflector 包，它是一个可以通过 ListAndWatch 机制获取并监视 API 对象变化的客户端封装。<br />Reflector 和 Informer 之间，用到了一个“增量先进先出队列”进行协同。而 Informer 与要编写的控制循环之间，则使用了一个工作队列来进行协同。<br />在实际应用中，除了控制循环之外的所有代码，实际上都是 Kubernetes 自动生成的，即：pkg/client/{informers, listers, clientset}里的内容。<br />而这些自动生成的代码，就为提供了一个可靠而高效地获取 API 对象“期望状态”的编程库。<br />所以，接下来，作为开发者，就只需要关注如何拿到“实际状态”，然后如何拿它去跟“期望状态”做对比，从而决定接下来要做的业务逻辑即可。<br />以上内容，就是 Kubernetes API 编程范式的核心思想。
