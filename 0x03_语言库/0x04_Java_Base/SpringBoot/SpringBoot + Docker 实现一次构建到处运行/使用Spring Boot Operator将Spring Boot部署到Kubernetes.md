Java SpringBoot Kubernetes<br />Spring Boot Operator基于Kubernetes的Custom Resource Definitions（CRDs）扩展API进行的开发。
<a name="FxPjv"></a>
## 打包Docker镜像
在讲部署之前需要先将Spring Boot应用打包成标准的DockerImage。<br />Java项目打包镜像用Maven/Gradle插件比较多，这里介绍一个新的Google开源的插件Jib，该插件使用起来比较方便。
:::danger
注意：Jib打包的镜像会导致Java应用的pid=1，在使用Spring Boot Operator进行发布时候，Operator会设置Kubernetes的`ShareProcessNamespace`参数为`true`（v1.10+版本都可使用）来解决该问题。
:::
下面就来演示一下，通过[https://start.spring.io](https://start.spring.io)生成一个标准的Spring Boot项目operator-demo，然后使用Jib插件进行镜像打包。
```bash
mvn com.google.cloud.tools:jib-maven-plugin:build \
-Djib.to.auth.username=${{ secrets.MY_USERNAME }} \
-Djib.to.auth.password=${{ secrets.MY_PASSWORD }} \
-Djib.container.jvmFlags=--add-opens,java.base/sun.nio.ch=ALL-UNNAMED \
-Djib.from.image=freemanliu/oprenjre:11.0.5 \
-Dimage=registry.cn-shanghai.aliyuncs.com/qingmuio/operator-demo/operator-demo:v1.0.0
```
执行上面的命令之后将得到一个标准的Docker镜像，该镜像会被推送到远程仓库。
<a name="uo2E1"></a>
## Operator快速体验
完成了镜像的构建之后，紧接着来安装Operator到Kubernetes集群。
<a name="Lyj0P"></a>
### 快速安装
此处快速安装只是为了快速体验demo。
```bash
kubectl apply -f https://raw.githubusercontent.com/goudai/spring-boot-operator/master/manifests/deployment.yaml
```
apply成功之后控制台输出。
```bash
namespace/spring-boot-operator-system created
customresourcedefinition.apiextensions.k8s.io/springbootapplications.springboot.qingmu.io created
role.rbac.authorization.k8s.io/spring-boot-operator-leader-election-role created
clusterrole.rbac.authorization.k8s.io/spring-boot-operator-manager-role created
clusterrole.rbac.authorization.k8s.io/spring-boot-operator-proxy-role created
clusterrole.rbac.authorization.k8s.io/spring-boot-operator-metrics-reader created
rolebinding.rbac.authorization.k8s.io/spring-boot-operator-leader-election-rolebinding created
clusterrolebinding.rbac.authorization.k8s.io/spring-boot-operator-manager-rolebinding created
clusterrolebinding.rbac.authorization.k8s.io/spring-boot-operator-proxy-rolebinding created
service/spring-boot-operator-controller-manager-metrics-service created
deployment.apps/spring-boot-operator-controller-manager created
```
稍等片刻查看是否已经安装成功。
```bash
kubectl  get po -n spring-boot-operator-system
```
成功如下输出：
```bash
NAME                                                       READY   STATUS    RESTARTS   AGE
spring-boot-operator-controller-manager-7f498596bb-wcwtn   2/2     Running   0          2m15s
```
部署OperatorDemo应用：<br />完成了Operator的部署之后，来部署第一个应用，这里就发布上面编写的Spring Boot应用opreator-demo。<br />首先需要先编写一个Spring Boot Application 的CRD部署yaml，如下。
```yaml
# Demo.yaml
apiVersion: springboot.qingmu.io/v1alpha1
kind: SpringBootApplication
metadata:
  name: operator-demo
spec:
  springBoot:
    version: v1.0.0
#    image: registry.cn-shanghai.aliyuncs.com/qingmuio/operator-demo/operator-demo:v1.0.0
```
为啥连Image都没有？这怎么发布，就name，version，就能完成发布？是的没错！就能完成发布，后面讲详细讲到他是如何完成的。<br />接着apply一下。
```bash
kubectl apply -f Demo.yaml
```
看到Console输出。
```bash
springbootapplication.springboot.qingmu.io/operator-demo created
```
验证<br />表示创建成功了，接着来看下部署的第一个应用，这里直接用上面的yaml中的name过滤即可。<br />查看Pod。
```bash
~# kubectl  get po | grep operator-demo
operator-demo-7574f4789c-mg58m             1/1     Running   0          76s
operator-demo-7574f4789c-ssr8v             1/1     Running   0          76s
operator-demo-7574f4789c-sznww             1/1     Running   0          76s
```
查看下pid不等于1的设置是否生效，根据下面的结果可以看到通过设置`ShareProcessNamespace`参数，可以在Kubernetes层面来解决这个pid=1的问题。
```bash
kubectl exec -it operator-demo-7574f4789c-mg58m bash
bash-5.0# ps -ef
UID        PID  PPID  C STIME TTY          TIME CMD
root         1     0  0 02:06 ?        00:00:00 /pause
root         6     0 26 02:06 ?        00:00:09 java --add-opens java.base/sun.nio.ch=ALL-UNNAMED -cp /app/resources:/app/classes:/app/libs/* io.qingmu.operator.operatordemo.Oper...
root        38     0  0 02:07 pts/0    00:00:00 bash
root        44    38  0 02:07 pts/0    00:00:00 ps -ef
```
查看SVC。
```bash
~# kubectl  get svc | grep operator-demo
operator-demo             ClusterIP   10.101.128.6     <none>        8080/TCP            2m52s
```
来访问一下试试。
```bash
root@server1:~# curl -i http://10.101.128.6:8080
HTTP/1.1 200 
Content-Type: text/plain;charset=UTF-8
Content-Length: 9
Date: Wed, 08 Apr 2020 08:45:46 GMT


hello !!!
```
来试着缩减他的副本数到1个。<br />编辑
```yaml
# Demo.yaml
apiVersion: springboot.qingmu.io/v1alpha1
kind: SpringBootApplication
metadata:
  name: operator-demo
spec:
  springBoot:
    version: v1.0.0
    replicas: 1
```
Demo.yaml，加入一个新的属性replicas。应用一下。
```bash
root@server1:~# kubectl apply -f Demo.yaml
springbootapplication.springboot.qingmu.io/operator-demo configured
```
再次查看Pod，可以发现Pod已经缩放为一个副本了。
```bash
~# kubectl  get po | grep operator-demo
operator-demo-7574f4789c-sznww             1/1     Running   0          8m29s
```
清理operator-demo：要删除该Pod，只需要执行delete即可。
```bash
~# kubectl delete -f Demo.yaml
springbootapplication.springboot.qingmu.io "operator-demo" deleted
```
再次查看Pod，已经没了。
```bash
kubectl  get po | grep operator-demo
```
<a name="pUQxK"></a>
### 部署自己的应用
部署自己私有仓库的应用需要需要先创建secret（如果已经创建跳过即可）。<br />创建docker-registry的secret。
```bash
kubectl create  \
secret docker-registry aliyun-registry-secret \
--docker-server=registry-vpc.cn-hangzhou.aliyuncs.com \
--docker-username=*** \
--docker-password=*** \
--docker-email=***
```
自己应用的crd Yaml。
```yaml
apiVersion: springboot.qingmu.io/v1alpha1
kind: SpringBootApplication
metadata:
  name: 你的应用的名称
spec:
  springBoot:
    version: v1.0.0
    replicas: 1 
    image: 你的image地址
    imagePullSecrets: 
      - 上面创建的secret
```
<a name="nGSml"></a>
## 一个完整的Spring Boot Application Yaml
下面是一个完整的yaml属性结构，大部分属性都可以用默认配置的即可。<br />不设置属性，默认使用Operator中设置的通用值详见后面的自定义安装Operator。
```yaml
apiVersion: springboot.qingmu.io/v1alpha1
kind: SpringBootApplication
metadata:
  name: operator-demo
  namespace: default
spec:
  springBoot:
    # image 可以不设置，如果不设置默认使用 IMAGE_REPOSITORY+/+mate.name+:+spec.springBoot.version
    # registry.cn-shanghai.aliyuncs.com/qingmuio + / + operator-demo + : + v1.0.0
    image: registry.cn-shanghai.aliyuncs.com/qingmuio/operator-demo:v1.0.0
    clusterIp: "" 
    version: v1.0.0 
    replicas: 1 
    resource:
      cpu:
        request: 50m
        limit: "" 
      memory:
        request: 1Gi
        limit: 1Gi 
    path:
      liveness: /actuator/health
      readiness: /actuator/health
      hostLog: /var/applog
      shutdown: /spring/shutdown
    imagePullSecrets: 
      - aliyun-docker-registry-secret
    env: 
      - name: EUREKA_SERVERS
        value: http://eureka1:8761/eureka/,http://eureka2:8761/eureka/,http://eureka3:8761/eureka/
    nodeAffinity: 
      key: "failure-domain.beta.kubernetes.io/zone"
      operator: "In"
      values:
        - "cn-i"
        - "cn-h"
        - "cn-g"
```
优雅停机的路径<br />由于优雅停机默认是关闭的并且并不支持Get请求所以需要开启和搭个桥。<br />首先在application.yml中启用。
```yaml
management:
  endpoints:
    web:
      exposure:
        include: "*"
  endpoint:
    shutdown:
      enabled: true
```
然后桥接一个Get方法。
```java
@RestController
public class ShutdownController {
    @Autowired
    private ShutdownEndpoint shutdownEndpoint;


    @GetMapping("/spring/shutdown")
    public Map<String, String> shutdown(HttpServletRequest request) {
        return shutdownEndpoint.shutdown();
    }
}
```
<a name="ISKS0"></a>
### Node亲和的使用
举一个列子，有一个Spring Boot应用user-service，希望能分布到3个可用区的6个节点上：<br />首先把机器划分多个可用区。
```bash
cn-i区(node-i1,node-i02)
cn-h区(node-g1,node-g02)
cn-g区(node-h1,node-h02)
```
现在有三个可以区 每个区有2台workload，一共6台。然后需要给这些机器分别打上label。<br />将全部的i区机器标注为cn-i。
```bash
kubectl label node node-i1 failure-domain.beta.kubernetes.io/zone=cn-i
kubectl label node node-i2 failure-domain.beta.kubernetes.io/zone=cn-i
```
同理将h区的标注为h，g区同理。
```bash
kubectl label node node-h1 failure-domain.beta.kubernetes.io/zone=cn-i
kubectl label node node-ih2 failure-domain.beta.kubernetes.io/zone=cn-i
```
现在准备工作就绪了，现在来设置让它达到调度效果，像如下编写即可。
```yaml
spec:
  springBoot:
    nodeAffinity: #可以不设置 节点亲和 这里演示的是尽量将pod分散到 i h g 三个可用区，默认设置了pod反亲和
      key: "failure-domain.beta.kubernetes.io/zone"
      operator: "In"
      values:
        - "cn-i"
        - "cn-h"
        - "cn-g"
```
<a name="byLUY"></a>
### Operator自定义安装
上面快速的安装了好了，接着来讲解下如何自定义安装，以及有哪些自定义的参数，可以个性化的参数用环境变量的方式注入。<br />下面来修改Deployment完成自己个性化的配置部署，从提供的部署yaml中拉倒最后，找到name是spring-boot-operator-controller-manager的Deployment，将修改它。
```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  labels:
    control-plane: controller-manager
  name: spring-boot-operator-controller-manager
  namespace: spring-boot-operator-system
.....


        #注意：一下配置针对通用全局的Spring Boot默认配置，对crd的Spring Boot生效，这里不配置也可以在部署的yaml中指定


        # 私有仓库的地址，比如我的最终打包的镜像地址是 registry.cn-shanghai.aliyuncs.com/qingmuio/operator-demo/operator-demo:v1.0.0
        # 那么配置的值是 registry.cn-shanghai.aliyuncs.com/qingmuio/operator-demo
        # 配置这个值之后，我们我们如果在发布的yaml中不写image，那么使用的image就是 IMAGE_REPOSITORY+"/"+mate.name+spec.springBoot.version
        - name: IMAGE_REPOSITORY
          value: registry.cn-shanghai.aliyuncs.com/qingmuio
        # 请求CPU限制
        - name: REQUEST_CPU
          value: 50m
        # 限制最大能用最大CPU Java应用可以不用限制，限制不合理会导致启动异常缓慢
        - name: LIMIT_CPU
          value: ""
        # 请求内存大小
        - name: REQUEST_MEMORY
          value: 500Mi
        # 限制最大内存大小 一般和request一样大即可
        - name: LIMIT_MEMORY
          value: 500Mi
        # 就绪检查Path，Spring Boot Actuator 默认Path
        - name: READINESS_PATH
          value: /actuator/health
        # 就绪存活Path，Spring Boot Actuator 默认Path
        - name: LIVENESS_PATH
          value: /actuator/health
        # 就绪存活Path，优雅停机Path
        - name: SHUTDOWN_PATH
          value: /spring/shutdown
        # 复制级 即副本数
        - name: REPLICAS
          value: "3"
        # 将日志外挂到主机磁盘Path，默认两者相同
        - name: HOST_LOG_PATH
          value: /var/applog
        # 用于pull镜像的secrets
        - name: IMAGE_PULL_SECRETS
          value: ""
        # 用于pull镜像的secrets
        - name: SPRING_BOOT_DEFAULT_PORT
          value: "8080"
        # Node亲和，比如我可以设置Pod尽量分散在不同可用区cn-i，cn-g，cn-h区
        - name: NODE_AFFINITY_KEY
          value: ""
        - name: NODE_AFFINITY_OPERATOR
          value: ""
        - name: NODE_AFFINITY_VALUES
          value: ""
        # 全局的环境变量，会追加到每个Spring Boot的每个Pod中，格式 k=v；k1=v2,
        # 如 EUREKA_SERVERS=http://eureka1:8761/eureka/,http://eureka2:8761/eureka/,http://eureka3:8761/eureka/;k=v
        - name: SPRING_BOOT_ENV
          value: ""
        image: registry.cn-shanghai.aliyuncs.com/qingmuio/spring-boot-operator-controller:latest
```
<a name="VdAsM"></a>
### 自定义安装之后部署
yaml可以简化为如下。
```yaml
apiVersion: springboot.qingmu.io/v1alpha1
kind: SpringBootApplication
metadata:
  name: 你的应用的名称
spec:
  springBoot:
    version: v1.0.0
```
<a name="fOPan"></a>
## 附录
环境变量表格。

| 环境变量名 | 是否可以空 | 默认值 | 说明 |
| --- | --- | --- | --- |
| IMAGE_REPOSITORY | true | “” | 私有仓库的地址 |
| REQUEST_CPU | true | 50m | 请求CPU限制 |
| LIMIT_CPU | true | “” | 限制最大能用最大CPU Java应用可以不用限制，限制不合理会导致启动异常缓慢 |
| REQUEST_MEMORY | true | 2Gi | 请求内存大小 |
| LIMIT_MEMORY | true | 2Gi | 限制最大内存大小 一般和request一样大即可 |
| READINESS_PATH | true | /actuator/health | 就绪检查Path，Spring Boot Actuator 默认Path |
| LIVENESS_PATH | true | /actuator/health | 存活检查Path，Spring Boot Actuator 默认Path |
| SHUTDOWN_PATH | true | /spring/shutdown | 就绪存活Path，优雅停机Path |
| REPLICAS | true | 3 | 副本数 |
| HOST_LOG_PATH | true | /var/applog | 将日志外挂到主机磁盘Path，默认两者相同 |
| IMAGE_PULL_SECRETS | true | 无 | 用于pull镜像的secrets |
| SPRING_BOOT_DEFAULT_PORT | true | 8080 | 用于pull镜像的secrets |
| NODE_AFFINITY_KEY | true | “” | Node亲和key，比如可以设置Pod尽量分散在不同可用区cn-i，cn-g，cn-h区 |
| NODE_AFFINITY_OPERATOR | true | “” | Node亲和操作符 |
| NODE_AFFINITY_VALUES | true | “” | Node亲和value |
| SPRING_BOOT_ENV | true | “” | 全局的环境变量，会追加到每个Spring Boot的每个Pod中，格式 k=v；k1=v2 |

Spring Boot Operator GitHub仓库：[https://github.com/goudai/spring-boot-operator](https://github.com/goudai/spring-boot-operator)<br />相关链接：

1. [https://github.com/goudai/spring-boot-operator](https://github.com/goudai/spring-boot-operator)
