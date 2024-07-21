Kubernetes<br />Kubernetes是一个开源容器编排平台，大大简化了应用程序的创建和管理。<br />Kubernetes之类的分布式系统可能很难管理，原因是涉及许多活动组件，所有这些组件必须正常工作，整个系统才能顺畅运行。即使某个小组件坏掉，也需要检测和修复它。<br />这些操作还需要自动化。Kubernetes让我们可以借助就绪探针和存活探针做到这一点。这里详细讨论了这些探针，在此之前先讨论一下健康检查。
<a name="Lt6ld"></a>
## 1、什么是健康检查？
健康检查是一种让系统知道应用程序实例是否正常运行的简单方法。如果应用程序实例未正常运行，其他服务不会访问它或向它发送请求。相反，会将请求发送到另一个准备就绪的实例，或者应该重试发送请求。<br />系统应该能够使应用程序处于健康状态。默认情况下，pod内的所有容器都已启动后，Kubernetes会开始向该pod发送流量。容器崩溃时，Kubernetes会重启容器。这个默认行为应该足以开始上手。由于Kubernetes有助于创建自定义健康检查，提高部署稳健性就变得比较简单，现在先讨论一下pod生命周期。
<a name="mKx0A"></a>
## 2、pod生命周期
Kubernetes pod遵循已定义的生命周期。这些是不同的阶段：<br />• pod首次创建后，从pending（挂起）阶段开始。调度器试图搞清楚把pod放置在哪里。如果调度器找不到放置pod的节点，它将保持挂起状态。（要检查pod为何处于挂起状态，运行`kubectl describe pod <pod name>`命令）。<br />• 一旦pod被调度，它进入容器creating（创建）阶段，在此阶段拉取应用程序所需的镜像，随后容器启动。<br />• 一旦容器在pod中，它进入running（运行）阶段，在此阶段它继续运行，直至程序成功完成或终止。<br />要检查pod的状态，请运行`kubectl get pod`命令，检查STATUS列。在这种情况下，所有pod都处于运行状态。此外，READY列表明pod已准备好接受用户流量。
```bash
# kubectl get pod
NAME READY STATUS RESTARTS AGE
my-nginx-6b74b79f57-fldq6 1/1 Running 0 20s
my-nginx-6b74b79f57-n67wp 1/1 Running 0 20s
my-nginx-6b74b79f57-r6pcq 1/1 Running 0 20s
```
<a name="E0tP6"></a>
## 3、Kubernetes中不同类型的探针
Kubernetes提供了以下类型的健康检查：<br />• 就绪探针：该探针将告诉你应用程序何时准备为流量提供服务。在允许服务向pod发送流量之前，Kubernetes将确保就绪探针通过。如果就绪探针未通过，Kubernetes不会向pod发送流量，直至探针通过。<br />• 存活探针：存活探针会让Kubernetes可以知道应用程序是否健康。如果应用程序健康，Kubernetes不会干扰pod的运行，但如果不健康，Kubernetes就会销毁pod，并启动新的pod来替换它。<br />为了进一步理解这点，不妨以实际场景为例。应用程序需要一些时间来预热，或从GitHub等某个外部源下载应用程序内容。除非完全准备好，否则应用程序不会接受流量。默认情况下，一旦容器内的进程启动，Kubernetes就会开始发送流量。使用就绪探针，Kubernetes会等到应用程序完全启动后，才允许服务将流量发送到新副本。<br />不妨看另一种场景：应用程序因代码错误而崩溃（可能是极端情况），并且无限期挂起，停止为请求提供服务。由于进程默认继续运行，Kubernetes会将流量发送到坏掉的pod。使用存活探针，Kubernetes将检测到应用程序不再为请求提供服务，默认情况下重启出故障的pod。<br />现在看看如何定义探针。探针有三种类型：<br />• HTTP<br />• TCP<br />• 命令<br />注意：可以选择从定义就绪探针或存活探针开始，因为两者的实现都需要类似的模板。比如说，如果先定义存活探针，可以使用它来定义就绪探针，反之亦然。<br />• HTTP探针（httpGet）：这是最常见的探针类型。即使应用程序不是HTTP服务器，通常也可以在应用程序内创建轻量级 HTTP服务器以响应存活探针。Kubernetes将在特定端口（本例中端口8080）测式ping路径（比如`/healthz`）。如果它收到200或300范围内的HTTP响应，将被标记为健康。（想进一步了解HTTP响应代码，请参阅该链接[https://developer.mozilla.org/en-US/docs/Web/HTTP/Status](https://developer.mozilla.org/en-US/docs/Web/HTTP/Status)）。否则，它会被标记为不健康。以下是定义HTTP存活探针的方法：
```yaml
livenessProbe:
  httpGet:
    path: /healthz
    port: 8080
```
HTTP就绪探针的定义与HTTP存活探针类似，只需要把liveness换成readiness。
```yaml
readinessProbe:
  httpGet:
    path: /healthz
    port: 8080
```
• TCP探针（tcpSocket）：使用TCP探针，Kubernetes会尝试在指定端口（比如下例中的8080端口）建立TCP连接。如果可以建立连接，则认为容器是健康的。如果不能，视为失败。在HTTP或命令探针无法正常工作的情况下，这些探针会很方便。比如说，FTP服务将能够使用这种类型的探针。
```yaml
readinessProbe:
  tcpSocket:
  	port: 8080
```
• 命令探针（exec命令）：在命令探针情况下，Kubernetes将在容器内运行命令。如果命令返回退出代码0，容器将被标记为健康。否则，将被标记为不健康。当不能或不想运行HTTP服务器时，这种类型的探针很有用，可以运行命令来检查应用程序是否健康。在下面示例中，检查文件/tmp/healthy是否存在；如果命令返回退出代码0，容器将被标记为健康；否则，它将被标记为不健康。
```yaml
livenessProbe:
exec:
command:
  - cat
  - /tmp/healthy
```
可根据需要运行的频率、成功和失败阈值以及等待响应的时间，以多种方式配置探针。<br />• initialDelaySeconds（默认值0）：如果知道应用程序需要n秒（比如30秒）来预热，可以使用`initialDelaySeconds`来添加延迟（以秒为单位），直至执行第一次检查。<br />• `periodSeconds`（默认值10）：如果想要指定执行检查的频率，可以使用`periodSeconds`来定义。<br />• `timeoutSeconds`（默认值1）：这定义了探针操作超时之前的最大秒数。<br />• `successThreshold`（默认值1）：这是在失败后探针直至被认为成功的尝试次数。<br />• `failureThreshold`（默认值 3）：万一探针失败，Kubernetes会尝试多次，然后将探针标记为失败。<br />注意：默认情况下，如果应用程序在尝试3次后仍未准备好，探针将停止。如果是存活探针，它将重启容器。如果是就绪探针，它会将pod标记为不健康。<br />有关探针配置的更多信息，请参阅该链接：[https://kubernetes.io/docs/tasks/configure-pod-container/configure-liveness-readiness-startup-probes/#configure-probes](https://kubernetes.io/docs/tasks/configure-pod-container/configure-liveness-readiness-startup-probes/#configure-probes)。<br />不妨结合到目前为止已讨论过的所有内容。这里要注意的关键是readinessProbe与httpGet的使用。第一次检查将在10秒后执行，然后每5秒重复一次。
```yaml
apiVersion: v1
kind: Pod 
metadata:
  labels:
  run: nginx
  name: nginx
spec:
  containers:
  - image: nginx
    name: nginx
    readinessProbe:
    httpGet:
    path: /
    port: 80
    initialDelaySeconds: 10
    periodSeconds: 5
```
• 要创建pod，请使用`kubectl create`命令，并使用`-f`标志指定YAML清单文件。可以为该文件取任何名称，但后缀名应该是.yaml。
```bash
kubectl create -f readinessprobe.yaml
pod/nginx created
```
• 如果现在检查pod的状态，它应该在STATUS列下显示状态为Running。但如果检查READY列，它仍然会显示0/1，这意味着它没有准备好接受新的连接。
```bash
kubectl get pod
NAME READY STATUS RESTARTS AGE
nginx 0/1 Running 0 16s
```
• 由于将初始延迟设置为1秒，几秒钟后验证状态。现在，pod应该在运行。
```bash
kubectl get pod
NAME READY STATUS RESTARTS AGE
nginx 1/1 Running 0 28s
```
• 要检查定义就绪探针时使用的所有参数（比如`initialDelaySeconds`、`periodSeconds`等）的详细状态，运行`kubectl describe`命令。
```bash
kubectl describe pod nginx |grep -i readiness
Readiness: http-get http://:80/ delay=10s timeout=1s period=5s #success=1 #failure=3
```
不妨通过一个例子进一步加深对存活探针和就绪探针概念的了解。先从活性探针开始。在下面例子中，在执行命令：touch healthy; sleep 20; rm -rf healthy; sleep 600’。<br />借助该命令，使用touch 命令创建了文件名“healthy”。该文件前20秒将存在于容器中，然后使用`rm -rf`命令将其删除。最后，容器将休眠600秒。<br />随后定义了存活探针。它先使用`cat health`命令检查该文件是否存在。它以5秒的初始延迟完成此操作。进一步定义了参数`periodSeconds`，它每5秒就执行存活探针。一旦删除该文件，20秒后，探针将处于失败状态。
```yaml
apiVersion: v1
kind: Pod
metadata:
  labels:
  name: liveness-probe-exec
spec:
  containers:
  - name: liveness-probe
    image: busybox
    args:
      - /bin/sh
      - -c
      - touch healthy; sleep 20; rm -rf healthy; sleep 600
    livenessProbe:
    exec:
    command:
      - cat
      - healthy
    initialDelaySeconds: 5
    periodSeconds: 5
```
• 要创建pod，将上述代码存储在以`.yaml`结尾的文件中（比如liveness-probe.yaml），然后使用`-f <file name>` 执行`kubectl create`命令，这将创建pod。
```
# kubectl create -f liveness-probe.yaml
pod/liveness-probe-exec created
```
• 运行`kubectl get events`命令，会看到存活探针已失败，容器已被杀死并重启。
```
54s Normal Scheduled pod/liveness-probe-exec Successfully assigned default/liveness-probe-exec to controlplane
53s Normal Pulling pod/liveness-probe-exec Pulling image "busybox"
52s Normal Pulled pod/liveness-probe-exec Successfully pulled image "busybox" in 384.330188ms
52s Normal Created pod/liveness-probe-exec Created container liveness-probe
52s Normal Started pod/liveness-probe-exec Started container liveness-probe
18s Warning Unhealthy pod/liveness-probe-exec Liveness probe failed: cat: can't open 'healthy': No such file or directory
18s Normal Killing pod/liveness-probe-exec Container liveness-probe failed liveness probe, will be restarted
```
• 还可以使用`kubectl get pods`命令进行验证，正如在重启列中看到，容器重启了一次。
```bash
# kubectl get pods
NAME READY STATUS RESTARTS AGE
liveness-probe-exec 1/1 Running 1 24s
```
• 现在已了解存活探针的工作原理，不妨调整上面的例子将其定义为就绪探针，以此了解就绪探针的工作原理。在下面例子中，在容器内执行命令（sleep 20; touch healthy; sleep 600），它先休眠20秒，创建一个文件，最后休眠600秒。由于初始延迟设置为15秒，第一次检查以15 秒延迟来执行。
```yaml
apiVersion: v1
kind: Pod
metadata:
  labels:
  name: readiness-probe-exec
spec:
  containers:
    - name: readiness-probe
      image: busybox
      args:
        - /bin/sh
        - -c
        - sleep 20;touch healthy;sleep 600
      readinessProbe:
      exec:
      command:
        - cat
        - healthy
      initialDelaySeconds: 15
      periodSeconds: 5
```
• 要创建pod，将上述代码存储在以.yaml结尾的文件中，然后执行`kubectl create`命令，该命令将创建pod。
```bash
# kubectl create -f readiness-probe.yaml
pod/readiness-probe-exec created
```
• 如果在这里执行`kubectl get events`，会看到探针失败，因为文件不存在。
```bash
63s Normal Scheduled pod/readiness-probe-exec Successfully assigned default/readiness-probe-exec to controlplane
62s Normal Pulling pod/readiness-probe-exec Pulling image "busybox"
62s Normal Pulled pod/readiness-probe-exec Successfully pulled image "busybox" in 156.57701ms
61s Normal Created pod/readiness-probe-exec Created container readiness-probe
61s Normal Started pod/readiness-probe-exec Started container readiness-probe
42s Warning Unhealthy pod/readiness-probe-exec Readiness probe failed: cat: can't open 'healthy': No such file or directory
If you check the status of the container initially, it is not in a ready state.
# kubectl get pods
NAME READY STATUS RESTARTS AGE
readiness-probe-exec 0/1 Running 0 5s
```
• 但是如果在20秒后检查它，它应该处于运行状态。
```bash
# kubectl get pods
NAME READY STATUS RESTARTS AGE 
readiness-probe-exec 1/1 Running 0 27s
```
<a name="ZHIYp"></a>
## 结论
任何分布式系统都需要检查健康，Kubernetes也不例外。使用健康检查为Kubernetes服务提供稳固的基础、更高的可靠性和更长的正常运行时间。
