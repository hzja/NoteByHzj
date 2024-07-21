Kubernetes
<a name="2Zf1K"></a>
### 1.kubectl get deployment/pods
这个命令如此重要的原因是它无需显示大量内容即可显示很有用的信息。<br />如果要为工作负载使用部署，则有两种选择：
```bash
kubectl get deploy 
 
kubectl get deploy -n 名称空间
kubectl get deploy –all-namespaces [或“ -A”]
```
理想情况下，希望看到的是1/1或等值的2/2，以此类推。这表明部署已被接受，并已尝试进行部署。<br />接下来，可能需要查看kubectl get pod，以查看部署的后备Pod是否正确启动。
<a name="i14Xm"></a>
### 2. kubectl get events
此命令将打印出给定名称空间中的事件，非常适合查找关键问题，例如崩溃的pod或无法pull容器镜像。<br />Kubernetes中的日志是“未排序的”，因此，将需要添加以下内容，这些内容取自OpenFaaS文档。
```bash
$ kubectl get events --sort-by=.metadata.creationTimestamp
```
kubectl get事件的另一个接近的命令是是kubectl describe，就像get deploy / pod一样，它与对象的名称一起工作：
```bash
kubectl describe deploy/figlet -n openfaas
```
在这里获得非常详细的信息。可以描述大多数事情，包括节点，这些节点将显示由于资源限制或其他问题而无法启动 Pod。
<a name="wSfXh"></a>
### 3. kubectl logs
这个命令肯定经常使用，但很多人使用了错误的方式。<br />如果进行了部署，比方说cert-manager命名空间中的cert-manager，那么很多人认为他们首先必须找到Pod的长（唯一）名称并将其用作参数。其实这种操作不对。
```bash
kubectl logs deploy/cert-manager -n cert-manager
```
要跟踪日志，请添加-f
```bash
kubectl logs deploy/cert-manager -n cert-manager -f
```
可以将所有三个结合起来。<br />如果 Deployment 或 Pod 有任何标签，则可以使用 -l app = name 或任何其他标签集来附加到一个或多个匹配Pod的日志中。
```bash
kubectl logs -l app=nginx
```
有一些工具，例如 stern 和 kail，可以匹配模式并节省一些键入操作，但它们会分散注意力。
<a name="24nil"></a>
### 4.kubectl get -o yaml
当开始使用由另一个项目或诸如Helm之类的其他工具生成的YAML时，将很快需要它。在生产中检查镜像的版本或在某处设置的注释也很有用。
```bash
kubectl run nginx-1 --image=nginx --port=80 --restart=Always
```
输出yaml
```bash
kubectl get deploy/nginx-1 -o yaml
```
而且可以添加–export并将YAML保存在本地以进行编辑并再次应用。<br />实时编辑YAML的另一个选项是kubectl edit，如果对vim感到困惑，不知道如何使用，请在命令前加上VISUAL = nano，使用这个简化编辑器。
<a name="UVQMu"></a>
### 5. kubectl scale 打开和关闭它了吗？
Kubectl scale可用于将Deployment及其Pod缩小为零个副本，实际上杀死了所有副本。当将其缩放回1/1时，将创建一个新的Pod，重新启动应用程序。<br />语法非常简单，可以重新启动代码并再次进行测试。
```bash
kubectl scale deploy/nginx-1 --replicas=0 
 
kubectl scale deploy/nginx-1 --replicas=1
```
<a name="GCxwi"></a>
### 6. Port forwarding
通过kubectl进行的端口转发可以在自己计算机上的本地或远程群集上公开一项服务，以便在任何已配置的端口上访问它，而无需在Internet上公开它。<br />以下是在本地访问Nginx部署的示例：
```bash
kubectl port-forward deploy/nginx-1 8080:80
```
有人认为这仅适用于部署或Pod，这是错误的。服务间是公平的，通常是转发的选择，因为它们将模拟生产集群中的配置。<br />如果确实想在Internet上公开服务，通常会使用LoadBalancer服务，或运行kubectl暴露：
```bash
kubectl expose deployment nginx-1 --port=80 --type=LoadBalancer
```
