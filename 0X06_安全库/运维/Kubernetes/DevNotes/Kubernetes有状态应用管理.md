Kubernetes
<a name="HBHnZ"></a>
## 什么是有状态应用
实例之间的不等关系以及实例对外数据有依赖关系的应用，就被称为"有状态应用"。<br />所谓实例之间的不等关系即对分布式应用来说，各实例，各应用之间往往有比较大的依赖关系，比如某个应用必须先于其他应用启动，否则其他应用将不能启动等。<br />对外数据有依赖关系的应用，最显著的就是数据库应用，对于数据库应用，是需要持久化保存其数据的，如果是无状态应用，在数据库重启数据和应用就失去了联系，这显然是违背初衷的，不能投入生产的。<br />所以，为了解决Kubernetes中有状态应用的有效支持，Kubernetes使用StatefulSet来编排管理有状态应用。StatefulSet类似于ReplicaSet，不同之处在于它可以控制Pod的启动顺序，它为每个Pod设置唯一的标识。其具有一下功能：

- 稳定的，唯一的网络标识符
- 稳定的，持久化存储
- 有序的，优雅部署和缩放
- 有序的，自动滚动更新

StatefulSet的设计很容易理解，它把现实世界抽象为以下两种情况：<br />（1）、拓扑状态。这就意味着应用之间是不对等关系，应用要按某种顺序启动，即使应用重启，也必须按其规定的顺序重启，并且重启后其网络标识必须和原来的一样，这样才能保证原访问者能通过同样的方法访问新的Pod；<br />（2）、存储状态 。这就意味着应用绑定了存储数据，不论什么时候，不论什么情况，对应用来说，只要存储里的数据没有变化，读取到的数据应该是同一份；<br />所以StatefulSet的核心功能就是以某种方式记录Pod的状态，然后在Pod被重新创建时，通过某种方法恢复其状态。
<a name="F6NzV"></a>
## 如何使用StatefulSet
介绍了Service是为一组Pod提供外部访问的一种方式。通常，使用 Service访问Pod有一下两种方式：<br />（1）、通过Cluster IP，这个Clustre IP就相当于VIP，访问这个IP，就会将请求转发到后端Pod上；<br />（2）、通过DNS方式，通过这种方式首先得确保Kubernetes集群中有DNS服务。这个时候只要访问"my-service.my-namespace.svc,cluster.local"，就可以访问到名为my-service的Service所代理的后端Pod；<br />而对于第二种方式，有下面两种处理方法：<br />（1）、Normal Service，即解析域名，得到的是Cluster IP，然后再按照方式一访问；<br />（2）、Headless Service，即解析域名，得到的是后端某个Pod的IP地址，这样就可以直接访问；<br />而在使用StatefulSet的时候，主要用到Headless Service，还记得Headless Service怎么定义的吗？<br />只需要把ClusterIP设置为None即可，如下：
```yaml
apiVersion: v1
kind: Service
metadata:
  name: nginx-headless-service
  labels:
    name: nginx-headless-service
spec:
  clusterIP: None
  selector:
    name: nginx
  ports:
  - port: 8000
    targetPort: 80
```
了解了Headless Service，还需要了解PV、PVC是怎么使用的。<br />下面，开始使用StatefulSet。<br />首先，创建两个个PV，因为准备为有状态应用创建两个副本，如下：
```yaml
apiVersion: v1
kind: PersistentVolume
metadata:
  name: nginx-pv01
  labels:
    storage: pv
spec:
  accessModes:
  - ReadWriteOnce
  capacity:
    storage: 1Gi
  persistentVolumeReclaimPolicy: Recycle
  nfs:
    path: /data/k8s
    server: 192.168.205.128
---
apiVersion: v1
kind: PersistentVolume
metadata:
  name: nginx-pv02
  labels:
    storage: pv
spec:
  accessModes:
  - ReadWriteOnce
  capacity:
    storage: 1Gi
  persistentVolumeReclaimPolicy: Recycle
  nfs:
    path: /data/k8s
    server: 192.168.205.128
```
然后编写StatefulSet需要的YAML文件，如下：
```yaml
apiVersion: v1
kind: Service
metadata:
  name: nginx
spec:
  ports:
  - port: 80
    name: web
  clusterIP: None
  selector:
    app: nginx
    role: stateful

---
apiVersion: apps/v1
kind: StatefulSet
metadata:
  name: web
spec:
  serviceName: "nginx"
  replicas: 2
  selector:
    matchLabels:
      app: nginx
  template:
    metadata:
      labels:
        app: nginx
        role: stateful
    spec:
      containers:
      - name: nginx
        image: nginx
        ports:
        - containerPort: 80
          name: web
        volumeMounts:
        - name: www
          mountPath: /usr/share/nginx/html
  volumeClaimTemplates:
  - metadata:
      name: www
    spec:
      accessModes: [ "ReadWriteOnce" ]
      resources:
        requests:
          storage: 1Gi
```
注意上面的 YAML 文件中和volumeMounts进行关联的是一个新的属性：volumeClaimTemplates，该属性会自动声明一个 pvc 对象和 pv 进行管理，而serviceName: "nginx"表示在执行控制循环的时候，用nginx这个Headless Service来保存Pod的可解析身份。<br />创建完成后，可以看到会起两个Pod：
```bash
$ kubectl get pod | grep web
web-0                                  1/1     Running   0             2m45s
web-1                                  1/1     Running   0             2m41s
```
从这两个Pod的命令可以看到，它们的名字不像Deployment那样随机生成的字符串，而是0，1这样的序号。这是因为StatefulSet要保证每个Pod顺序，确保每次重启或者更新，每个Pod依然保持以前的数据，不会错乱。所以StatefulSet会以[statefulset-name]-[index]规则进行命名，其中index从0开始。而且每个Pod的创建是有顺序的，如上只有web-0进入running状态后，web-1才创建。<br />当两个Pod都进入running状态后，就可以查看其各自的网络身份了，通过kubectl exec来查看，如下：
```bash
$ kubectl exec web-0 -- sh -c 'hostname'
web-0
$ kubectl exec web-1 -- sh -c 'hostname'
web-1
```
可以看到这两个pod的hostname和pod的名字是一致的，都被分配为对应的编号，接下来用DNS的方式来访问Headless Service。<br />先启动一个调试Pod，如下：
```
apiVersion: v1
kind: Pod
metadata:
  name: dnsutils
  namespace: default
spec:
  containers:
  - name: dnsutils
    image: lansible/dnstools
    command:
      - sleep
      - "3600"
    imagePullPolicy: IfNotPresent
  restartPolicy: Always
```
然后进入dnsutils容器进行解析，如下：
```bash
$ kubectl exec -it dnsutils -- /bin/sh
/ # nslookup web-0.nginx
Server:         10.96.0.10
Address:        10.96.0.10#53

Name:   web-0.nginx.default.svc.cluster.local
Address: 172.16.51.247

/ # nslookup web-1.nginx
Server:         10.96.0.10
Address:        10.96.0.10#53

Name:   web-1.nginx.default.svc.cluster.local
Address: 172.16.51.251

/ #
```
从nslookup的结果分析，在访问web-0.nginx的时候解析的是web-0这个Pod的IP，另一个亦然。这表示，如果在应用中配置web-0.nginx，则只会调用web-0这个Pod，在配置有状态应用，比如Zookeeper的时候，需要在配置文件里指定zkServer，这时候就可以指定类似：zk-0.zookeeper，zk-1.zookeeper。<br />如果现在更新StatefuleSet，起更新顺序是怎么样的呢？<br />首先，新开一个终端，输入以下命令用以观察：
```bash
$ kubectl get pods -w -l role=stateful
NAME    READY   STATUS    RESTARTS   AGE
web-0   1/1     Running   0          67m
web-1   1/1     Running   0          67m
```
然后使用以下命令更新应用的镜像，如下：
```bash
$ kubectl set image statefulset/web nginx=nginx:1.8
```
然后观察web应用的更新顺序，如下：
```bash
$ kubectl get pods -w -l role=stateful
NAME    READY   STATUS    RESTARTS   AGE
web-0   1/1     Running   0          67m
web-1   1/1     Running   0          67m
web-1   1/1     Terminating   0          68m
web-1   1/1     Terminating   0          68m
web-1   0/1     Terminating   0          68m
web-1   0/1     Terminating   0          68m
web-1   0/1     Terminating   0          68m
web-1   0/1     Pending       0          0s
web-1   0/1     Pending       0          0s
web-1   0/1     ContainerCreating   0          0s
web-1   0/1     ContainerCreating   0          1s
web-1   1/1     Running             0          10s
web-0   1/1     Terminating         0          69m
web-0   1/1     Terminating         0          69m
web-0   0/1     Terminating         0          69m
web-0   0/1     Terminating         0          69m
web-0   0/1     Terminating         0          69m
web-0   0/1     Pending             0          0s
web-0   0/1     Pending             0          0s
web-0   0/1     ContainerCreating   0          0s
web-0   0/1     ContainerCreating   0          1s
web-0   1/1     Running             0          9s
```
从整个顺序可以看到，起更新是从后往前进行更新的，也就是先更新web-1的pod，再更新web-0的pod。通过这种严格的对应规则，StatefulSet就保证了Pod的网络标识的稳定性，通过这个方法，就可以把Pod的拓扑状态按照Pod的名字+编号的方式固定起来。此外，Kubernetes还为每一个Pod提供了一个固定并且唯一的访问入口，即这个Pod的DNS记录。<br />由此，对StatefulSet梳理如下：<br />（1）、StatefulSet直接管理的是Pod。这是因为StatefulSet里的Pod实例不像ReplicaSet中的Pod实例完全一样，它们是有细微的区别，比如每个Pod的名字、hostname等是不同的，而且StatefulSet区分这些实例的方式就是为Pod加上编号；<br />（2）、Kubernetes通过Headless Service为这个编号的Pod在DNS服务器中生成带同样编号的记录。只要StatefulSet能保证这个Pod的编号不变，那么Service中类似于web-0.nginx.default.svc.cluster.local这样的DNS记录就不会变，而这条记录所解析的Pod IP地址会随着Pod的重新创建自动更新；<br />（3）、StatefulSet还可以为每个Pod分配并创建一个和Pod同样编号的PVC。这样Kubernetes就可以通过Persitent Volume机制为这个PVC绑定对应的PV，从而保证每一个Pod都拥有独立的Volume。这种情况下即使Pod被删除，它所对应的PVC和PV依然会保留下来，所以当这个Pod被重新创建出来过后，Kubernetes会为它找到同样编号的PVC，挂载这个PVC对应的Volume，从而获取到以前Volume以前的数据；
<a name="EwweJ"></a>
## 总结
StatefulSet这个控制器的主要作用之一，就是使用Pod模板创建Pod的时候，对它们进行编号，并且按照编号顺序完成作业，当StatefulSet的控制循环发现Pod的实际状态和期望状态不一致的时候，也会按着顺序对Pod进行操作。<br />当然 StatefulSet 还拥有其他特性，在实际的项目中，还是很少回去直接通过 StatefulSet 来部署有状态服务的，除非自己能够完全能够 hold 住，对于一些特定的服务，可能会使用更加高级的 Operator 来部署，比如 etcd-operator、prometheus-operator 等等，这些应用都能够很好的来管理有状态的服务，而不是单纯的使用一个 StatefulSet 来部署一个 Pod就行，因为对于有状态的应用最重要的还是数据恢复、故障转移等等。
