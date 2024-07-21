Kubernetes 网络<br />Kubernetes网络是Kubernetes中一个核心概念。简而言之，Kubernetes网络模型可以确保集群上所有Kubernetes pod都能进行通信。此外，在Kubernetes网络模型的基础上，Kubernetes还有其他核心概念，即Kubernetes Services和Kubernetes Ingress。<br />本文将使用系统模型的方法探索Kubernetes网络。将开发一个简单的模型来了解容器与容器间的通信以及Pod之间的通信。<br />![2021-04-28-14-00-24-671568.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619589922853-c35659e8-1d8b-4193-9e9c-d4d7aec94928.png#clientId=u99f72ae4-809a-4&from=ui&id=u1ecbd9c1&originHeight=678&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=40496&status=done&style=shadow&taskId=u9e003ed5-0271-4231-a759-4932086f8ef&title=)
<a name="HGdeA"></a>
## 如何看待网络
毫无疑问，网络是一个极为广泛且复杂的领域，它需要多年的理论积累以及实践才能精通。这里在概念层面对网络进行梳理，暂时不涉及实现层面的细节。<br />![理想的网络模型](https://cdn.nlark.com/yuque/0/2021/png/396745/1619590177917-39923adc-fd2d-45fa-8f53-0a6b31b5d4fc.png#clientId=u99f72ae4-809a-4&from=ui&id=u871ec33d&originHeight=379&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=true&size=70301&status=done&style=shadow&taskId=u82c899b7-256b-43e7-aced-d12b9954426&title=%E7%90%86%E6%83%B3%E7%9A%84%E7%BD%91%E7%BB%9C%E6%A8%A1%E5%9E%8B "理想的网络模型")<br />上图将网络描述为Network Graph，该网络由一组节点以及节点之间的链接组成。如果当且仅当节点之间存在联系时，一个节点才可以与另一个节点交换信息。<br />![消息交换框架](https://cdn.nlark.com/yuque/0/2021/png/396745/1619590189062-2c48406d-0fda-4e32-8af0-c7170aa5d31c.png#clientId=u99f72ae4-809a-4&from=ui&id=u35670fb5&originHeight=204&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=true&size=46006&status=done&style=shadow&taskId=u1395eba9-ebc0-492b-9678-99f2bc5208b&title=%E6%B6%88%E6%81%AF%E4%BA%A4%E6%8D%A2%E6%A1%86%E6%9E%B6 "消息交换框架")<br />一个节点，即源节点，通过将消息放入目标的输入队列，与另一个节点，即目标交换消息。消息交换由源节点观察到的Send Event，Send·M和在目标节点观察到的相应的Receive Event，Recv·M表示。<br />![消息交换行为](https://cdn.nlark.com/yuque/0/2021/png/396745/1619590197537-1ae6b924-028b-4710-929d-7db684d1870b.png#clientId=u99f72ae4-809a-4&from=ui&id=ud4698f73&originHeight=662&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=true&size=56755&status=done&style=shadow&taskId=uc573ec42-9975-409b-8ba3-3d5cf628946&title=%E6%B6%88%E6%81%AF%E4%BA%A4%E6%8D%A2%E8%A1%8C%E4%B8%BA "消息交换行为")<br />网络中的节点要么是Process，要么是Switch。Process会产生和消耗消息，Switch根据其转发信息库（FIB）处理消息。<br />![S1和S2的转发信息库（FIB）](https://cdn.nlark.com/yuque/0/2021/png/396745/1619590207226-508a5f2c-0cda-4879-ac65-545fe01d0851.png#clientId=u99f72ae4-809a-4&from=ui&id=u22db01f8&originHeight=330&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=true&size=71607&status=done&style=shadow&taskId=u4dac7a40-c7bd-474c-88f2-8772c597c68&title=S1%E5%92%8CS2%E7%9A%84%E8%BD%AC%E5%8F%91%E4%BF%A1%E6%81%AF%E5%BA%93%EF%BC%88FIB%EF%BC%89 "S1和S2的转发信息库（FIB）")<br />上图描述了Switch的转发信息库（FIB）S1和S2。在收到消息时，每台Switch都会查询其转发信息库，以决定是发送（deliver）、转发（forward）还是丢弃（discard）该消息。<br />Switch：

- 将信息的请求头，即源地址、源端口、目标地址和目标端口与其转发信息库相匹配
- 执行相关操作，默认为弃置（discard）
<a name="pYgF3"></a>
## Kubernetes网络模型
![2021-04-28-14-00-27-266032.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619590227820-b1de9b4a-d8f7-46b6-ad57-7100504339b7.png#clientId=u99f72ae4-809a-4&from=ui&id=ua85b0d7a&originHeight=578&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=31303&status=done&style=shadow&taskId=u726c0b7b-988f-49ae-9f99-00ec5884df9&title=)<br />Kubernetes网络模型是一个描述性的网络模型，也就是说，任何满足Kubernetes网络模型规范的网络都是Kubernetes网络。<br />然而，Kubernetes并没有规定如何实现网络模型。事实上，现在市面上有许多替代的实现，称为网络插件。<br />本节将用一组关于消息交换的约束条件来描述Kubernetes网络模型。
<a name="nl73Z"></a>
### 限制条件：网络可寻址实体
Kubernetes网络模型定义了3个可寻址实体：K8S pod、K8S 节点以及K8S Service，每个实体都会分配到一个不同的IP地址。
```bash
∧ (K8s-Pod(E₁) ∨ K8s-Node(E₁) ∨ K8s-Service(E₁))
∧ (K8s-Pod(E₂) ∨ K8s-Node(E₂) ∨ K8s-Service(E₂)):
  addr(E₁, a) ∧ addr(E₂, a)₂
   ⟺ E₁ = E₂
```
然而，网络模型不对这些IP地址做任何进一步的声明。例如，Kubernetes网络模型不对从这些IP地址中提取的IP地址空间做任何进一步的声明。
<a name="n7Ybh"></a>
### 限制条件：容器间通信
Kubernetes网络模型要求在Pod P上下文中执行的容器C1可以通过localhost与在P上下文中执行的其他容器C2进行通信。
```bash
K8s-Pod(P) ∧ K8s-Container(C₁, P) ∧ K8s-Container(C₂, P):
  open(C₂, p)
   ⟹
    Send(e, C₁, 127.0.0.1, _, 127.0.0.1, p)
      ⟹
         Recv(e, C₂, 127.0.0.1, _, 127.0.0.1, p)
```
<a name="LGGZN"></a>
### 限制条件：Pod到Pod
Kubernetes网络模型要求在Pod P1上下文中执行的容器C1可以通过P2的地址与在P2上下文中执行的其他容器C2进行通信。
```bash
∧ K8s-Pod(P₁) ∧ K8s-Container(C₁, P₁)
∧ K8s-Pod(P₂) ∧ K8s-Container(C2, P₂):
addr(P₁, sa) ∧ addr(P₁, ta) ∧ open(C₂, tp)
  ⟹
   Send(e, C₁, sa, sp, ta, tp)
     ⟹
      Recv(e, C₂, sa, sp, ta, tp)
```
Kubernetes网络模型要求托管在节点N上的一个Process，称为Daemon D，可以通过P的地址与托管在N上的Pod P上下文中执行的任何容器C进行通信。
```bash
K8s-Node(N) ∧ K8s-Daemon(D) ∧ K8s-Pod(P) ∧ K8s-Container(C, P):
host(N, D) ∧ host(N, P) ∧ addr(P, a) ∧ open(C, p)
  ⟹
   Send(e, D, _, _, a, p)
    ⟹
     Recv(e, C, _, _, a, p)
```
<a name="UAIGK"></a>
## Kubernetes网络作为Network Graph
![2021-04-28-14-00-27-335172.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619590240255-13d67c69-a87d-4e6b-b538-c6d6f8c4e549.png#clientId=u99f72ae4-809a-4&from=ui&id=u79d4aba6&originHeight=579&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=33900&status=done&style=shadow&taskId=u96559b9c-0004-4315-90ff-172c1234073&title=)<br />本节用Kubernetes Network Graph这个理想的模型来描述Kubernetes网络模型。<br />下图描述了本节内容中的用例：Kubernetes集群K1由2个节点组成。每个节点托管2个Pod。每个Pod执行2个容器，一个容器监听8080端口，一个容器监听9090端口。此外，每个节点托管1个Daemon。<br />![2021-04-28-14-00-27-753449.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619590251543-42a979c0-1b9d-4f31-83d6-020151b34fea.png#clientId=u99f72ae4-809a-4&from=ui&id=u210eb39b&originHeight=427&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=103548&status=done&style=shadow&taskId=u679615b8-30b2-40e3-b9cd-ec1006e9f08&title=)<br />可以将Kubernetes集群网络建模为一个具有一组节点和一组链接的Graph。
<a name="n3fBA"></a>
### 节点
每个K8S容器C映射到网络Process C
```bash
K8s-Pod(P) ∧ K8s-Container(C, P):
   Process(C)
```
每个Daemon D映射到网络Process C
```bash
K8s-Daemon(D):
   Process(D)
```
每个K8s Pod P映射到网络Switch P, Pod的Switch
```bash
K8s-Pod(P):
  Switch(P)
```
每个K8S节点N 映射到网络 Switch N，节点的Switch：
```bash
K8s-Pod(N):
  Switch(N)
```
<a name="VyGUc"></a>
### 链接
每个容器C会被链接到其Pod Switch P
```bash
K8s-Pod(P) ∧ K8s-Container(C, P):
  link(C, P)
```
每个Daemon D会被链接到其节点Switch N
```bash
K8s-Node(N) ∧ K8s-Daemon(D):
 host(N, D)
  ⟹
   link(D, N)
```
每个Pod Switch P会被链接到其节点Switch N
```bash
K8s-Node(N) ∧ K8s-Pod(P):
  host(N, P)
    ⟹
      link(P, N)
```
每个节点Switch N1会被链接到其他各节点Switch N2
```bash
K8s-Node(N₁) ∧ K8s-Node(N₂):
  N₁ ≠ N₂
   ⟹
     link(N₁, N₂)
```
<a name="Mk1yr"></a>
### 在Pod Switch的转发信息库
![P2的转发信息库](https://cdn.nlark.com/yuque/0/2021/png/396745/1619589859674-28655233-be81-489e-9c67-0bb12f189678.png#clientId=u99f72ae4-809a-4&from=ui&id=ud256b603&originHeight=317&originWidth=861&originalType=binary&ratio=1&rotation=0&showTitle=true&size=21000&status=done&style=shadow&taskId=u401c35ef-c569-4d88-9ad3-917c7f31f23&title=P2%E7%9A%84%E8%BD%AC%E5%8F%91%E4%BF%A1%E6%81%AF%E5%BA%93 "P2的转发信息库")
```bash
1. Delivery on localhost
K8s-Pod(P) ∧ K8s-Container(C, P):
 open(C, p)
  ⟹
   [* * 127.0.0.1 p Deliver(C)] in FIB[P]
2. Delivery on Pod Address
K8s-Pod(P) ∧ K8s-Container(C, P):
 addr(P, a) ∧ open(C, p)
  ⟹
   [* * a p Deliver(C)] in FIB[P]
3. Local Forwarding Rule
K8s-Node(N) ∧ K8s-Pod(P):
 host(N, P)
  ⟹
   [* * * * Forward(N)] in FIB[P]
```
<a name="YBPUk"></a>
### 在节点Switch的转发信息库
![转发信息库 N2](https://cdn.nlark.com/yuque/0/2021/png/396745/1619589836125-85ee625a-4557-473a-b89f-996537d1e360.png#clientId=u99f72ae4-809a-4&from=ui&id=u0315d016&originHeight=342&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=true&size=33317&status=done&style=shadow&taskId=u8d771bff-edcb-417f-be5b-4ee5206b7ed&title=%E8%BD%AC%E5%8F%91%E4%BF%A1%E6%81%AF%E5%BA%93%20N2 "转发信息库 N2")
```bash
1. Node to Pod Forwarding Rule
K8s-Node(N) ∧ K8s-Pod(P):
  host(N, P) ∧ addr(P, a)
   ⟹
    [* * a * Forward(P)] in FIB[N]
2. Node to Node Forwalding Rule
K8s-Node(N₁) ∧ K8s-Node(N₂) ∧ K8s-Pod(P):
   N₁ ≠ N₂ ∧ host(N₂, P) ∧ addr(P, a)
    ⟹
     [* * a * Forward(N₂)] in FIB[N₁]
```
<a name="ACOvd"></a>
## 示例
本节将通过一些例子，按照Kubernetes集群网络K1中的消息生命（Life of a Message）来进行讲解。
<a name="SZDLC"></a>
### 容器到容器
容器C1.1需要与容器C1.2进行通信：

- C1.1在P1的上下文中执行
- C1.2在P1的上下文中执行

![C1.1通过127.0.0.1:9090到C1.2](https://cdn.nlark.com/yuque/0/2021/png/396745/1619589823421-24fd8839-eaef-4985-9577-57131869294c.png#clientId=u99f72ae4-809a-4&from=ui&id=u29e19972&originHeight=256&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=true&size=33548&status=done&style=shadow&taskId=u7354f497-b1c2-473a-9cf3-95c9d780274&title=C1.1%E9%80%9A%E8%BF%87127.0.0.1%3A9090%E5%88%B0C1.2 "C1.1通过127.0.0.1:9090到C1.2")
<a name="A06UH"></a>
### 节点内Pod到Pod通信
容器C 1.1需要与C 3.1进行通信：

- C 1.1在N1节点上的P1上下文中执行
- C 3.1在N1节点上的P3上下文中执行

![C 1.1通过10.1.1.2:8080到C 3.1](https://cdn.nlark.com/yuque/0/2021/png/396745/1619589789049-f80e8677-caf2-4069-a7fc-7a937f9ed2cd.png#clientId=u99f72ae4-809a-4&from=ui&id=ud96086b2&originHeight=463&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=true&size=65362&status=done&style=shadow&taskId=u0e9a43cb-0eae-47c7-a3d2-6097357a793&title=C%201.1%E9%80%9A%E8%BF%8710.1.1.2%3A8080%E5%88%B0C%203.1 "C 1.1通过10.1.1.2:8080到C 3.1")
<a name="t9z18"></a>
### 节点间Pod到Pod通信
容器C 1.1需要与容器C 2.1进行通信：

- C1.1是在N1节点上托管的P1的上下文中执行的
- C2.1在节点N2上的P2上下文中执行

![C1.1通过10.1.2.1:8080到C2.1](https://cdn.nlark.com/yuque/0/2021/png/396745/1619589765675-45aa09fb-4c3a-4741-ba97-1b037bebc612.png#clientId=u99f72ae4-809a-4&from=ui&id=uaf572f03&originHeight=568&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=true&size=82184&status=done&style=shadow&taskId=ud088fe8f-97b3-49d7-87b2-e7a2bd520da&title=C1.1%E9%80%9A%E8%BF%8710.1.2.1%3A8080%E5%88%B0C2.1 "C1.1通过10.1.2.1:8080到C2.1")
<a name="q0lLj"></a>
### Daemon到Pod通信
Daemon D1需要与容器 C 1.1通信：

- D1托管在节点N1上
- C 1.1在Pod P1的上下文中执行，该Pod托管在节点N1上

![D1通过10.1.1.1:8080到C 1.1](https://cdn.nlark.com/yuque/0/2021/png/396745/1619589753564-ae327b0c-b32e-4e39-aa0b-2c0e9201cf76.png#clientId=u99f72ae4-809a-4&from=ui&id=u6f4e092e&originHeight=348&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=true&size=48979&status=done&style=shadow&taskId=u400cbd90-a175-4544-979f-91791d1b58e&title=D1%E9%80%9A%E8%BF%8710.1.1.1%3A8080%E5%88%B0C%201.1 "D1通过10.1.1.1:8080到C 1.1")
<a name="O9MpA"></a>
## 总结
Kubernetes网络模型是一个允许性的网络模型，也就是说，任何满足Kubernetes网络模型约束的网络都是一个有效的Kubernetes网络。<br />将Kubernetes网络模型映射到Network Graph，使得我们能够在概念层面上对网络进行推理，并且跳过了在实现层面上推理所需的一系列细节。
