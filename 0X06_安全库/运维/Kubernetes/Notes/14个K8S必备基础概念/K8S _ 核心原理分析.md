Kubernetes
<a name="eJMFs"></a>
## 一、背景
基于分布式的架构中，需要管理的服务是非常多的，无论是服务的数量还是体系划分；<br />从服务的能力上看，可以进行分层管控，只是其中有相当一部分服务层，改动更新的频率很低，所以感知也不明显；<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1687146712442-3d914742-4a5f-496f-babf-4786fa458ce4.png#averageHue=%23e7eaec&clientId=u392bbe98-e000-4&from=paste&id=u6b232c56&originHeight=445&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3a3a75a9-a06e-4958-a2bb-645635094e6&title=)<br />就以自己当下参与研发的系统来说；<br />通过K8S进行管理的服务近百个，这中间有部分服务采用集群模式，即便是这个规模的系统，也几乎不可能依赖纯人工运维的形式，自动化流程必不可少；
<a name="zH4Gg"></a>
## 二、持续集成
此前围绕该主题写过一个完整的实践案例，主要围绕Jenkins、Docker、K8S等组件的使用层面，总结源码编译、打包、镜像构建、部署等自动化管理的流程；<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1687146712500-2725c9af-45e4-42de-8913-55cd919f5c42.png#averageHue=%23f3f3f3&clientId=u392bbe98-e000-4&from=paste&id=udfce6b68&originHeight=256&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue9d4587e-535e-46ac-b1f5-691cfcb40a9&title=)<br />**Jenkins**：是一个扩展性非常强的软件，用于自动化各种任务，包括构建、测试和部署等；<br />**Docker**：作为开源的应用容器引擎，可以把应用程序和其相关依赖打包生成一个Image镜像文件，是一个标准的运行环境，提供可持续交付的能力；<br />**Kubernetes**：作为开源的容器编排引擎，用来对容器化应用进行自动化部署、 扩缩和管理；
<a name="Alfv5"></a>
## 三、K8S架构
<a name="rHeIx"></a>
### 1、核心组件
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1687146712528-4de94a24-730f-4b12-bb72-828b2a839462.png#averageHue=%23dbdbdb&clientId=u392bbe98-e000-4&from=paste&id=ub86781f7&originHeight=529&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub9a42903-4d31-48af-aa16-b29e1121fb9&title=)
<a name="kzeB2"></a>
####  Control-Plane-Components：控制平面组件
对集群做出全局决策，例如：资源调度、检测、事件响应，可以在集群中的任何节点上运行；

- api：开放K8S的API，组件之间通过API交互，相当于控制面的前端；
- controllermanager：运行控制器进程，逻辑上是一个单独的进程；
- scheduler：监听新建未指定运行节点的Pods，并为Pod选择运行节点；
- etcd：兼具一致性和高可用性的键值数据库，作为保存K8S数据的后台库;
<a name="Ldly0"></a>
#### Node：节点组件
该组件会在每个节点上运行，负责维护运行的Pod并提供Kubernetes运行环境；

- kubelet：在每个节点上运行的代理，保证容器都运行在Pod中；
- kube-proxy：每个节点上运行的网络代理， 维护节点上的网络规则；
<a name="Ggoo5"></a>
#### Container-Runtime：容器运行时
负责运行容器的软件，支持Docker、containerd、CRI-O等多个容器运行环境，以及任何实现Kubernetes-CRI容器运行环境接口；
<a name="Wb7rH"></a>
### 2、分层结构
从整体的功能上来考虑，K8S集群可以分为：用户、控制平面、节点三个模块；<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1687146712432-a2060928-729f-4896-9017-a27acdf1fad9.png#averageHue=%23f5f5f5&clientId=u392bbe98-e000-4&from=paste&id=u4436add6&originHeight=329&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub5064ec3-1dac-48ad-841d-6a5c24b3013&title=)<br />**用户侧**：不论是CLI命令行还是UI界面，会与控制面板的APIserver进行交互，APIserver再与其他组件交互，最终执行相应的操作命令；<br />**控制平面**：以前也称为Master，核心组件包括APIserver、controller、scheduler、etcd，主要用来调度整个集群，以及做出全局决策；<br />**节点**：通过将容器放入在节点上运行的Pod中来执行工作负载，简单的理解工作负载就是各种应用程序等，节点上的核心组件包括Pod、kubelet、Container-Runtime、kube-proxy等；
<a name="AF5tY"></a>
### 3、核心能力
站在研发的视角来看，K8S提供极其强大的应用服务管理能力；
<a name="uvDIb"></a>
#### 3.1 发现与负载
服务Service可以将运行在一个或一组Pod上的网络应用程序公开为网络服务的方法，通常使用标签对资源对象进行筛选过滤；<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1687146712505-0536bc76-2adc-4172-bd05-9654293ed7e6.png#averageHue=%23f8f8f8&clientId=u392bbe98-e000-4&from=paste&id=u7d6e1df7&originHeight=355&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u35e4f727-99a5-42d0-a444-f9ed23c1566&title=)
<a name="FcGOC"></a>
#### 3.2 调度
调度器通过监测机制来发现集群中新创建且尚未被调度到节点上的Pod，由于Pod中的容器和Pod本身可能有不同的资源要求，调度会将Pod放置到合适的节点上；<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1687146712984-94c32ef0-7e22-4962-844f-bff40b2862ed.png#averageHue=%23f8f8f8&clientId=u392bbe98-e000-4&from=paste&id=ue1478731&originHeight=478&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u104f9e19-0472-4beb-ab3c-1ae2f624cf2&title=)
<a name="xBqbf"></a>
#### 3.3 自动伸缩
K8S可以通过指标检查工作负载的资源需求，例如CPU利用率、响应时长、内存利用率、或者其他，从而判断是否需要执行伸缩，垂直维度可以是更多的资源分配，水平维度可以是更多的集群部署；<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1687146713068-c32f4ed7-7674-4da1-818d-c6182e3d2a26.png#averageHue=%23f5f4f4&clientId=u392bbe98-e000-4&from=paste&id=uf99b3ad9&originHeight=280&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud25678fc-fc28-4960-962a-e5720f2cdb2&title=)<br />K8S可以自动伸缩，也具备自动修复的能力，当节点故障或者应用服务异常时，会被检查到，可能会进行节点迁移或者重启；
<a name="LhEnf"></a>
## 四、应用案例
<a name="bpiD5"></a>
### 1、服务部署
在此前的实践案例中，用CLI命令行和脚本文件的方式，完成的部署动作，而在整个流程中涉及集群的多个组件协作，多次的通信和调度；
```bash
kubectl create -f pod.yaml
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1687146713150-940d0f30-6620-4ea9-9608-158532c7b720.png#averageHue=%2374a365&clientId=u392bbe98-e000-4&from=paste&id=u8b4d8c9d&originHeight=1260&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub60bf280-ea6a-471f-a452-2215f141ffa&title=)
<a name="QJxQP"></a>
### 2、交互流程
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1687146713228-ad85fe9e-4e24-414c-91a8-f486695db6b4.png#averageHue=%23f9f9f9&clientId=u392bbe98-e000-4&from=paste&id=ue5d62995&originHeight=429&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubcc7755c-2e5c-4346-a84c-48facda0d43&title=)<br />【1】CLI命令行和UI界面，都是通过APIserver接口，与集群内部组件交互，比如上述的Pod部署操作；<br />【2】在APIserver收到请求之后，会将序列化状态的对象写入到etcd中完成存储操作；<br />【3】Scheduler调度器通过监测（Watch）机制来发现集群中新创建且尚未被调度到节点上的Pod；<br />【4】在集群中找到一个Pod的所有可调度节点，对这些可调度节点打分，选出其中得分最高的节点来运行Pod，然后调度器将这个调度决定通知给APIserver；<br />【5】APIserver完成信息存储后，然后通知相应节点的Kubelet；<br />【6】Kubelet是基于PodSpec来工作的，确保这些PodSpec中描述的容器处于运行状态且运行状况良好，每个PodSpec是一个描述Pod的YAML或JSON对象；<br />【7】Pod是可以在Kubernetes中创建和管理的、最小的可部署的计算单元，包括一个或多个容器；
