K8s<br />要弄明白k8s的细节，需要知道k8s是个什么东西。它的主要功能，就是容器的调度--也就是把部署实例，根据整体资源的使用状况，部署到任何地方。先不要扯别的，那会扰乱视线，增加复杂性。<br />注意任何这两个字，预示着并不能够通过常规的IP、端口方式访问部署的实例。复杂性由此而生。<br />学k8s，就要看它要调度哪些资源。以传统的感觉来看，无非就是cpu、内存、网络、io等。在了解怎么对这些资源调度之前，先要搞懂什么叫Pod，这可是k8s的核心概念之一。
<a name="kpBkJ"></a>
## 1、Pod
pod是k8s调度的最小单元，包含一个或者多个容器（这里的容器可以暂时认为是docker）。<br />Pod拥有一个唯一的IP地址，在包含多个容器的时候，依然是拥有一个IP地址，它是怎么办到的呢？<br />xjjdog之前写过两篇Docker原理的文章，指出其中两个使用到的底层技术，就是namespace和cgroup，k8s在使用多个容器的时候，用到的就是共享namespace，这样Pod里的容器就可以通过localhost通信了，就像两个进程一样。同理的，Pod 可以挂载多个共享的存储卷（Volume），这时内部的各个容器就可以访问共享的 Volume 进行数据的读写。<br />![2021-05-01-14-57-17-978093.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619853330784-dff3754e-c846-452d-8278-9e9fe646e7ee.png#clientId=u4ec34a90-4cf0-4&from=ui&id=u49aa7e85&originHeight=487&originWidth=831&originalType=binary&ratio=1&rotation=0&showTitle=false&size=21612&status=done&style=none&taskId=u4f56cead-d206-4540-ac8c-90fda713b7b&title=)<br />一些边车(Sidecar)，以及存活探针等，也是以容器的形式，存在于Pod中的。所以Pod是一个大杂烩，它取代了docker容器的一部分工作（这既是Pause容器的职责），比如创建一些共享的net namespace等。<br />那如何表示、声明一个Pod呢？又如何指定这些容器与Pod的关系呢？k8s选用了yaml这种配置方式，初衷是避免过度的API设计。<br />很好，这又引入了另外一个问题，那就是yml文件的膨胀。所有的k8s运维，都有过被yml文件给支配的恐惧。<br />没有银弹，只不过把问题转移到另外一个场景罢了。<br />声明一个Pod，就是写yml文件。一个Pod的yml样例，可能长得像下面这样。
```yaml
apiVersion: v1           #本版号
kind: Service            #创建的资源类型
metadata:                #元数据必选
  namespace: bcmall      #绑定命名空间
  name: bcmall-srv       #Service资源名称
spec:                    #定义详细信息
  type: NodePort         #类型
  selector:              #标签选择器
    app: container-bcmall-pod 
  ports:                 #定义端口
    - port: 8080         #port 指定server端口,此端口用于集群内部访问
      targetPort: 80     #绑定pod端口
      nodePort: 14000    #将server 端口映射到Node节点的端口,用于外网访问
      protocol: TCP      #端口协议
```
注意kind这个选项，这将是k8s概念膨胀的噩梦！k8s的各种配置，基本上都是围着这里转。要让这些yml文件生效，需要用到kubectl 命令，就像这样。
```bash
kubectl create -f ./bcmall.yaml
```
访问一个Pod，可以通过它的IP，也可以通过内部的域名（这时候就需要CoreDNS）。当这么用的时候，其实Pod的表现，就相当于一台普通的机器，里面的容器就是一堆进程而已。
<a name="NHoiy"></a>
## 2、探针和钩子
一个Pod被调度之后，就要进行初始化。初始化肯定是得有一个反馈的，否则都不知道最终有没有启动成功。这些健康检查的功能，叫做探针（Probe），一个比较怪异的英文名词。<br />常见的有livenessProbe、readinessProbe、startupProbe等三种探针。<br />livenessProbe有点像心跳，如果判定不在线了，就会把它干掉；readinessProbe一般表示就绪状态，也比较像心跳，证明服务在正常跑着；startupProbe用于判断容器是否已经启动好，避免一些超时等，比如JVM启动完毕了，才能对外提供服务。<br />![2021-05-01-14-57-18-183543.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619853306748-4fd2a9bf-66b0-4147-b84d-0e1f6ceea28d.png#clientId=u4ec34a90-4cf0-4&from=ui&id=u464edaf8&originHeight=446&originWidth=890&originalType=binary&ratio=1&rotation=0&showTitle=false&size=36487&status=done&style=none&taskId=ua2f66123-8ddd-4d64-92f0-d661bed1bfc&title=)<br />一般，花费120s startupProbe的启动实践，每隔5s检测一下livenessProbe，每隔10s检测一下readinessProbe,是常用的操作。<br />这些信息，也是在yml中配置的，具体的配置层次如何，这里不罗嗦，您就查文档去吧。<br />再说一下钩子（Hook）。主要有PostStart和PreStop两种。PostStart 可以在容器启动之后就执行，PreStop 则在容器被终止之前被执行。这没什么神奇的，就是执行一些shell脚本而已，只不过比较常用，就提升到了关键字的级别。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619858467405-5b1106d7-f447-426d-8071-7f1de7927452.png#clientId=u1bd4b050-ef46-4&from=paste&height=324&id=ueaedc4e3&originHeight=972&originWidth=882&originalType=binary&ratio=1&rotation=0&showTitle=false&size=112565&status=done&style=shadow&taskId=ud6229940-5a82-4893-a84b-60cc2eb3924&title=&width=294)<br />来看看它长什么样子。由于这些配置文件大同小异，后面就不再贴这样的代码了。
```yaml
apiVersion: v1
kind: Pod
metadata:
  labels:
    test: liveness
  name: liveness-exec
spec:
  containers:
  - name: liveness
    image: k8s.gcr.io/busybox
    args:
    - /bin/sh
    - -c
    - touch /tmp/healthy; sleep 30; rm -rf /tmp/healthy; sleep 600
    livenessProbe:
      exec:
        command:
        - cat
        - /tmp/healthy
      initialDelaySeconds: 5
      periodSeconds: 5
```
<a name="jovR8"></a>
## 3、高可用引起的名词爆炸
上面说到，yaml的复杂性，是由于kind的种类多所引起的。首先要接触的，就是ReplicaSet。<br />需要多个副本，才能做高可用。<br />原因很简单，一个Pod就相当于一台机器，当掉之后，就无法提供服务了，这是哪门子的高可用？所以对等的Pod，要有多份才行。<br />ReplicaSet简称RS，可以让Pod数量一直保持在某个水平。但它在操作起来还是有点麻烦了，所以一般使用更加高级的Deployment。Deployment可以实现一些滚动升级的需求，但前提是需要在spec.template.metadata.labels中设置了相应的键值对。<br />k8s的一些过滤工作，都是通过labels来实现的。这其实是一种非常折衷的做法，因为它本身并没有做一些类似于sql查询之类的工作，就只能在这一堆map的键值对上做文章。比如这样：
```bash
kubectl get pod -n demo -l app=nginx,version=v1
```
是不是很魔幻的写法？不要紧，习惯了就好了。<br />这些yml配置，通常都是一环套一环的，还会有交叉引用，所以也会有优先级。高阶的kind会直接顺带把低阶的kind一起创建了，属于一个级联关系。<br />一切不过是yml文件包了一层而已。<br />要接触下一个kind：service了。<br />为什么需要Service？因为上面创建的Pod，哪怕是Deployment创建的Pod，访问它都要费些功夫。虽然Pod有IP，但如果它重启了，或者销毁了，IP就会动态变化。因为Pod是被调度的，它并不知道自己会被调度到哪一台机器。<br />Service这个东西，就是要提供一种非IP的访问途径，使得不论Pod在哪里，都能访问的到它。<br />如图所示，通过Labels的过滤，可以把多个Pod归结为一类，然后以某种类型对外暴露服务。Service说白了也是一个组合后的东西。<br />![2021-05-01-14-57-18-605635.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619853041520-826d63ad-0cfd-43ae-9ad9-27aacfcc8e0c.png#clientId=u4ec34a90-4cf0-4&from=ui&id=u9eefaaf6&originHeight=558&originWidth=1016&originalType=binary&ratio=1&rotation=0&showTitle=false&size=59222&status=done&style=shadow&taskId=u83a8ca4e-0d3f-46c2-9fb9-d3f9b375ec5&title=)<br />对外访问的类型，这里要着重说明一下，因为它重要，所以要敲黑板。主要有4种：

- ClusterIP 创建一个虚拟的IP，唯一且不可修改。所有访问该IP的请求，都将被iptables转发到后端。这是默认的行为，就是一个coredns的插件
- NodePort 提供一个静态端口（NodePort）来暴露服务，主要使用的技术是NAT
- LoadBalancer LoadBalancer主要用于做外部的服务发现，即暴露给集群外部的访问
- ExternalName 使用较少，感兴趣的可以自行了解

但是等等。k8s是如何实现跨主机的Pod相互访问的呢？<br />在单个Node上的Pod相互访问可以理解，直接通过docker0网桥分配的IP，就可相互访问。<br />那k8s的底层网络是真么设置的呢？答案可能令人沮丧。k8s本身并不负责网络管理，也不为容器提供具体的网络设置，它是通过CNI（容器网络接口）来实现的。在不同的Node之上，不同的Pod访问就费了点劲，这正是CNI的工作。常用的CNI插件有：Flannel、Calico、Canal、Weave。<br />没错，又是一堆名词，而且各个都很难搞。<br />网络方面是k8s最复杂的知识点，框架也奇多。
<a name="Y6ooj"></a>
## 4、内部组件
在开启更多的Kind之前，来看一下k8s的内部组件。<br />下面这张图，就是官网的一张图片，说明了k8s的一系列必要的组件。其中，etcd根本就不是这个体系里的，但k8s的一些持久化状态，需要有个地方存，就引入了这么一个组件，用来存储配置信息。<br />![2021-05-01-16-42-59-529549.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619858594805-1d1a7440-f2f6-4c45-9e13-7ac1de3688e9.png#clientId=u1bd4b050-ef46-4&from=ui&id=uc48e9b34&originHeight=493&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=153636&status=done&style=shadow&taskId=ucd61c946-a9ff-49f8-b53d-20996e137e9&title=)<br />其中左半部分，是k8s自身的组件；右半部分，就在每个Node（也就是物理机）上的守护进程。它们的作用如下：

- kube-apiserver 提供Rest接口，属于k8s的灵魂，所有的认证、授权、访问控制、服务发现等功能，都通过它来暴露
- kube-scheduler 一看就是个调度组件，实际上它的作用也是这样。它会监听未调度的 Pod，实现指定的目标
- kube-controller-manager 负责维护整个k8s集群的状态。注意是k8s集群的状态，它不管Pod
- kubelet 这是个守护进程，用来和apiserver通信，汇报自己Node的状态；一些调度命令，也是通过kubelet来接收执行任务
- kube-proxy kube-proxy其实就是管理service的访问入口,包括集群内Pod到Service的访问和集群外访问service。上面提到的四种模式，就是通过proxy进行转发的

这些组件的职责，已经是非常非常清楚了。难点还是在多种Kind概念上。
<a name="tBQjE"></a>
## 5、更多概念
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619858554140-06a640a2-1c86-4508-a797-066547878a81.png#clientId=u1bd4b050-ef46-4&from=paste&height=541&id=ueb91bff5&originHeight=1624&originWidth=1597&originalType=binary&ratio=1&rotation=0&showTitle=false&size=226657&status=done&style=none&taskId=udeb36f0d-8ddf-43f4-a6d8-afc73821dff&title=&width=532.3333333333334)<br />图中的这些概念，本质上都是在Pod之上，又包了一层。层次越高，功能越抽象，依赖的配置也越多。下面将挑主要的进行介绍。

- StatefulSet Deployment部署后的实例，它的id都是随机的，比如bcmall-deployment-5d45f98bd9，它是无状态的。与此对用的是StatefulSet，生成的实例名称是类似bcmall-deployment-1这样的。它具备固定的网络标记，比如主机名，域名等，可以按照顺序来部署和扩展，非常适合类似MySQL这样的实例部署
- DaemonSet 用于确保集群中的每一个节点只运行特定的pod副本，通常用于实现系统级后台任务
- configMap和Secret 顾名思义，就是做配置用的，因为容器或多或少会需要外部传入一些环境变量。可以用来实现业务配置的统一管理， 允许将配置文件与镜像文件分离，以使容器化的应用程序具有可移植性
- PV和PVC 业务运行就需要存储，可以通过PV进行定义。PV的生命周期独立于Pod的生命周期，就是一段网络存储；PVC是用户对于存储的需求：Pod消耗节点资源，PVC消耗PV资源，PVC和PV是一一对应的。没错，它们都是通过yml文件声明的
- **StorageClass ** 可以实现动态PV，是更进一步的封装
- Job 只要完成就立即退出，不需要重启或重建
- Cronjob 周期性任务控制，不需要持续后台运行
- CRD
<a name="coy2k"></a>
## 6、资源限制
k8s的资源限制，仍然是通过cgroup来实现的。<br />k8s提供了requests和limits 两种类型参数对资源进行预分配和使用限制。<br />不要被这两个词给迷惑了。requests就相当于JVM参数中的-Xms，limits就相当于-Xmx。所以，如果类比着把这两个值设置成一样的，是一种最佳的实践方式。<br />只是它的设置有点怪异：
```yaml
requests:
 memory: "64Mi"
 cpu: "250m"
limits:
 memory: "128Mi"
 cpu: "500m"
```
内存的单位是Mi，而cpu的单位是m，要多别扭有多别扭，但它是有原因的。<br />m是毫核的意思。比如，操作系统有4核，把它乘以1000，那就是总CPU资源是4000毫核。如果想要应用最多占用1/4核，那就设置成250m。<br />再来看内存，Mi是MiB的意思（MB和MiB还真不一样）。<br />若内存使用超出限制，会引发系统的OOM机制，但CPU不会，顶多会影响系统运行罢了。<br />k8s还提供了叫做LimitRange和ResourceQuota的Kind，用来限定CPU和Memory的申请范围，功能更加高级。
<a name="rcEhY"></a>
## 7、集群搭建工具
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619858631550-75ae75ca-f156-4db8-b039-69a800dd2dca.png#clientId=u1bd4b050-ef46-4&from=paste&height=220&id=ua3764989&originHeight=661&originWidth=1110&originalType=binary&ratio=1&rotation=0&showTitle=false&size=80408&status=done&style=none&taskId=u3a6f9681-187f-4bb6-a12a-15bc13bbf70&title=&width=370)<br />k8s的集群搭建，常见的有kind，minikube，kubeadm，rancher2等。其中rancher2可以说是相对来说比较容易的方式。它考虑了一些网络不通的因素，有一些推荐的代理选项，对于新手来说，拿来尝尝鲜还是很好用的。<br />但在正常的使用中，还是推荐使用kubeadm这个工具，这是官方维护和推荐的搭建工具，并做到了向下兼容，按照官方的指引，只需要kubeadm init就能搭建完毕。至于什么监控、日志等，反倒是好处理的了。<br />k8s最麻烦的有三点：

- yml文件概念爆炸
- 网络方案多样、复杂
- 权限、证书配置繁琐

搞懂了这三个方面，可以说玩转k8s就没问题了。<br />![2021-05-01-14-57-19-739908.jpg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1619852658714-3143d425-c4c2-4538-980e-17a02b0b22cf.jpeg#clientId=u4ec34a90-4cf0-4&from=ui&id=u949b9f96&originHeight=1890&originWidth=920&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1119489&status=done&style=none&taskId=u475892f5-6cab-4473-bbe7-4f233127b94&title=)
