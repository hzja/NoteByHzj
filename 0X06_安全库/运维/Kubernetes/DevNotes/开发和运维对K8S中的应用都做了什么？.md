Kubernetes<br />在应用的整个生命周期里，开发和运维都和它密不可分。一个塑造它，一个保养它。<br />如果应用需要部署到K8S中，开发和运维在其中都做了什么呢？
<a name="oGnhs"></a>
## 开发侧
从开发侧来说，应用应该具备以下能力：

- 具有健康检测接口
- 具有优雅退出能力
- 具有metrics接口
- 能够接入链路追踪系统
- 日志输出标准统一
<a name="nyOM0"></a>
### 定义健康检测接口
健康检测接口用于检测应用的健康状态，在K8S中，使用Readiness和Liveness分别来探测应用是否就绪和是否存活，如果未就绪或者未存活，K8S会采取相应的措施来确保应用可用。<br />如果应用未定义好相应的健康检测接口，K8S就无法判断应用是否正常可用，整个应用对运维来说就是黑匣子，也就谈不上应用稳定性了。<br />定义一个简单的健康检测接口如下：
```go
package router

import (
	"github.com/gin-gonic/gin"
	v1 "go-hello-world/app/http/controllers/v1"
)

func SetupRouter(router *gin.Engine) {
	ruc := new(v1.RootController)
	router.GET("/", ruc.Root)

	huc := new(v1.HealthController)
	router.GET("/health", huc.HealthCheck)
}
```
```go
package v1

import (
	"github.com/gin-gonic/gin"
	"go-hello-world/app/http/controllers"
	"go-hello-world/pkg/response"
	"net/http"
)

type HealthController struct {
	controllers.BaseController
}

func (h *HealthController) HealthCheck(c *gin.Context) {
	response.WriteResponse(c, http.StatusOK, nil, gin.H{
		"result": "健康检测页面",
		"status": "OK",
	})
}
```
如上定义了health接口，当应用启动后，只需要探测这个接口，如果返回OK，表示应用是正常的。<br />当然，上面的接口是非常简单的，在实际情况下，应用本身也许还依赖起来应用，比如redis，mysql，mq等，如果它们异常，应用是不是异常的呢？那应用健康检测需不需要检测其他应用的健康状态呢？<br />既然定义好了健康检测接口，那YAML模板就可以增加健康检测功能，如下：
```yaml
readinessProbe:
  httpGet:
    path: /health
    port: http
  timeoutSeconds: 3
  initialDelaySeconds: 20
livenessProbe:
  httpGet:
    path: /health
    port: http
  timeoutSeconds: 3
  initialDelaySeconds: 30
```
<a name="bbFuT"></a>
### 定义优雅下线功能
应用发版是常规不能再常规的操作，通常情况下都是滚动更新的方式上线，也就是先起一个新应用，再删一个老应用。<br />如果这时候老应用有部分的流量，突然把老应用的进程杀了，这部分流量就无法得到正确的处理，部分用户也会因此受到影响。<br />怎么才会不受影响呢？<br />假如在停止应用之前先告诉网关或者注册中心，等对方把应用摘除后再下线，这样就不会有任何流量受到影响了。<br />在K8S中，当要删除Pod的时候，Pod会变成Terminating状态，kubelet看到Pod的状态如果为Terminating，就会开始执行关闭Pod的流程，给Pod发SIGTERM信号，如果达到宽限期Pod还未结束就给Pod发SIGKILL信号，从Endpoints中摘除Pod等。<br />从上面可知，Pod在停止之前会收到SIG信号，如果应用本身没有处理这些信号的能力，那应用如果知道什么时候该结束呢？<br />下面简单定义一个处理SIG信号的功能。
```go
package shutdown

import (
	"context"
	"fmt"
	"net/http"
	"os"
	"os/signal"
	"time"
)

// 优雅退出

type Shutdown struct {
	ch      chan os.Signal
	timeout time.Duration
}

func New(t time.Duration) *Shutdown {
	return &Shutdown{
		ch:      make(chan os.Signal),
		timeout: t,
	}
}

func (s *Shutdown) Add(signals ...os.Signal) {
	signal.Notify(s.ch, signals...)
}

func (s *Shutdown) Start(server *http.Server) {
	<-s.ch
	fmt.Println("start exist......")

	ctx, cannel := context.WithTimeout(context.Background(), s.timeout*time.Second)
	defer cannel()
	if err := server.Shutdown(ctx); err != nil {
		fmt.Println("Graceful exit failed. err: ", err)
	}
	fmt.Println("Graceful exit success.")
}
```
```go
package main

import (
	"github.com/gin-gonic/gin"
	"go-hello-world/pkg/shutdown"
	"go-hello-world/router"
	"log"
	"net/http"
	"syscall"
	"time"
)


func main() {
	r := gin.New()

	// 注册路由
	router.SetupRouter(r)

	server := &http.Server{
		Addr:    ":8080",
		Handler: r,
	}

	// 运行服务
	go func() {
		err := server.ListenAndServe()
		if err != nil && err != http.ErrServerClosed {
			log.Fatalf("server.ListenAndServe err: %v", err)
		}
	}()

	// 优雅退出
	quit := shutdown.New(10)
	quit.Add(syscall.SIGINT, syscall.SIGTERM)
	quit.Start(server)
}
```
当接收到SIG信号的时候，就会调用`Shutdown`方法做应用退出处理。<br />除此，还要结合K8S的PreStop Hook来定义结束前的钩子，如下：
```yaml
lifecycle:
  preStop:
    exec:
      command:
        - /bin/sh
        - '-c'
        - sleep 30
```
如果使用注册中心，比如nacos，可以在PreStop Hook中先告诉nacos要下线，如下：
```yaml
lifecycle:
  preStop:
    exec:
      command:
        - /bin/sh
        - -c
        - "curl -X DELETE your_nacos_ip:8848/nacos/v1/ns/instance?serviceName=nacos.test.1&ip=${POD_IP}&port=8880&clusterName=DEFAULT" && sleep 30
```
<a name="oZUQB"></a>
### 定义Metrics接口
Metrics主要用来暴露应用指标，可以根据实际情况自定义指标，以便于监控工具Prometheus进行数据收集展示。<br />有些语言有现成的`exporter`，比如java的jmx_exporter，没有的就需要自己在应用中集成。<br />比如：
```go
package main

import (
	"github.com/SkyAPM/go2sky"
	v3 "github.com/SkyAPM/go2sky-plugins/gin/v3"
	"github.com/SkyAPM/go2sky/reporter"
	"github.com/gin-gonic/gin"
	"github.com/prometheus/client_golang/prometheus/promhttp"
	"go-hello-world/pkg/shutdown"
	"go-hello-world/router"
	"log"
	"net/http"
	"syscall"
	"time"
)

var SKYWALKING_ENABLED = false

func main() {
	r := gin.New()

	// 注册路由
	router.SetupRouter(r)

	server := &http.Server{
		Addr:    ":8080",
		Handler: r,
	}

	// 启动metrics服务
	go func() {
		http.Handle("/metrics", promhttp.Handler())
		if err := http.ListenAndServe(":9527", nil); err != nil {
			log.Printf("metrics port listen failed. err: %s", err)
		}
	}()

	// 运行服务
	go func() {
		err := server.ListenAndServe()
		if err != nil && err != http.ErrServerClosed {
			log.Fatalf("server.ListenAndServe err: %v", err)
		}
	}()

	// 优雅退出
	quit := shutdown.New(10)
	quit.Add(syscall.SIGINT, syscall.SIGTERM)
	quit.Start(server)
}
```
这种会暴露默认的Http指标，可以通过`curl 127.0.0.1:9527/metrics`获取指标。
```
......
# HELP promhttp_metric_handler_requests_total Total number of scrapes by HTTP status code.
# TYPE promhttp_metric_handler_requests_total counter
promhttp_metric_handler_requests_total{code="200"} 0
promhttp_metric_handler_requests_total{code="500"} 0
promhttp_metric_handler_requests_total{code="503"} 0
```
如果需要自定义指标的话，只需按规则定义即可，如下：
```go
package metrics

import (
	"github.com/prometheus/client_golang/prometheus"
	"net/http"
	"time"
)

var (
	// HttpserverRequestTotal 表示接收http请求总数
	HttpserverRequestTotal = prometheus.NewCounterVec(prometheus.CounterOpts{
		Name: "httpserver_request_total",
		Help: "The Total number of httpserver requests",
	},
													  // 设置标签：请求方法和路径
													  []string{"method", "endpoint"})

	HttpserverRequestDuration = prometheus.NewHistogramVec(prometheus.HistogramOpts{
		Name:    "httpserver_request_duration_seconds",
		Help:    "httpserver request duration distribution",
		Buckets: []float64{0.1, 0.3, 0.5, 0.7, 0.9, 1},
	},
														   []string{"method", "endpoint"})
)

// 注册监控指标
func init() {
	prometheus.MustRegister(HttpserverRequestTotal)
	prometheus.MustRegister(HttpserverRequestDuration)
}

func NewMetrics(router http.HandlerFunc) http.HandlerFunc {
	return func(w http.ResponseWriter, r *http.Request) {
		start := time.Now()
		router(w, r)
		duration := time.Since(start)
		// httpserverRequestTotal 记录
		HttpserverRequestTotal.With(prometheus.Labels{"method": r.Method, "endpoint": r.URL.Path}).Inc()
		// httpserverRequestDuration 记录
		HttpserverRequestDuration.With(prometheus.Labels{"method": r.Method, "endpoint": r.URL.Path}).Observe(duration.Seconds())
	}
}
```
这样就定义了httpserver_request_total和httpserver_request_duration_seconds指标，引用过后就能在/metrics中看到对应的数据。<br />定义好了指标，下面就是收集了。既可以通过自定义收集规则收集，也可以通过自动发现的方式收集，为了方便，主要采用自动发现的方式。<br />只需要在deployment的templates中定义好annotation，prometheeus就会自动添加采集目标，如下：
```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  labels:
    app: httpserver
  name: httpserver
  namespace: default
spec:
  replicas: 2
  selector:
    matchLabels:
      app: httpserver
  template:
    metadata:
      annotations:
        prometheus.io/scrape: "true"
        prometheus.io/port: "metrics"
      labels:
        app: httpserver
    spec:
      containers:
          image: baidjay/httpserver:ubuntu-v3-metrics
          imagePullPolicy: IfNotPresent
          lifecycle:
            preStop:
              exec:
                command:
                  - /bin/sh
                  - -c
                  - sleep 15
          livenessProbe:
            failureThreshold: 3
            httpGet:
              path: /healthz
              port: http
              scheme: HTTP
            initialDelaySeconds: 30
            periodSeconds: 10
            successThreshold: 1
            timeoutSeconds: 3
          name: httpserver
          ports:
            - containerPort: 8080
              name: http
              protocol: TCP
            - name: metrics
              protocol: TCP
              containerPort: 9527
          readinessProbe:
            failureThreshold: 3
            httpGet:
              path: /healthz
              port: http
              scheme: HTTP
            initialDelaySeconds: 20
            periodSeconds: 10
            successThreshold: 1
            timeoutSeconds: 3
```
<a name="Squ4L"></a>
### 定义Trace功能
Trace用于跟踪，每个请求都会生成一个TraceID，这个ID会伴随请求的整个生命周期，也可以根据这个ID查询请求的整个链路情况。<br />链路追踪，目前市面上有很多开源系统，比如Skywalking，Jeager，Zipkin等，它们各有各的特点，如下。

| 

 | Pinpoint | Zipkin | Jaeger | Skywalking |
| --- | --- | --- | --- | --- |
| OpenTracing兼容 | 否 | 是 | 是 | 是 |
| 客户端支持语言 | java\\php | java\\go\\php等 | java\\go\\php等 | java\\nodejs\\php等 |
| 存储 | hbase | es\\mysql\\内存等 | es\\kafka\\内存等 | es\\mysql\\h2等 |
| 传输协议支持 | thrift | http\\mq | udp\\http | grpc |
| UI丰富程度 | 高 | 低 | 中 | 中 |
| 实现方式 | 字节码注入 | 拦截请求 | 拦截请求 | 字节码注入 |
| 扩展性 | 低 | 高 | 高 | 中 |
| Trace查询 | 不支持 | 支持 | 支持 | 支持 |
| 告警支持 | 支持 | 不支持 | 不支持 | 支持 |
| JVM监控 | 支持 | 不支持 | 不支持 | 支持 |
| 性能损失 | 高 | 中 | 中 | 低 |

比较推荐使用Jaeger，它是CNCF的毕业项目，成长空间和云原生的系统架构兼容性比较好。<br />不过，这里采用的Skywalking。<br />Skywalking有许多现成的客户端，比如Java、Python等，可以直接使用，它们都会自动埋点，但是对于Go来说就只有自己手动埋点了，需要自己去写代码。<br />比如：
```go
package main

import (
	"github.com/SkyAPM/go2sky"
	v3 "github.com/SkyAPM/go2sky-plugins/gin/v3"
	"github.com/SkyAPM/go2sky/reporter"
	"github.com/gin-gonic/gin"
	"github.com/prometheus/client_golang/prometheus/promhttp"
	"go-hello-world/pkg/shutdown"
	"go-hello-world/router"
	"log"
	"net/http"
	"syscall"
	"time"
)

var SKYWALKING_ENABLED = false

func main() {
	r := gin.New()

	// 配置skywalking
	if SKYWALKING_ENABLED {
		rp, err := reporter.NewGRPCReporter("skywalking-oap:11800", reporter.WithCheckInterval(time.Second))
		if err != nil {
			log.Printf("create gosky reporter failed. err: %s", err)
		}
		defer rp.Close()
		tracer, _ := go2sky.NewTracer("go-hello-world", go2sky.WithReporter(rp))
		r.Use(v3.Middleware(r, tracer))
	}

	// 注册路由
	router.SetupRouter(r)

	server := &http.Server{
		Addr:    ":8080",
		Handler: r,
	}

	// 启动metrics服务
	go func() {
		http.Handle("/metrics", promhttp.Handler())
		if err := http.ListenAndServe(":9527", nil); err != nil {
			log.Printf("metrics port listen failed. err: %s", err)
		}
	}()

	// 运行服务
	go func() {
		err := server.ListenAndServe()
		if err != nil && err != http.ErrServerClosed {
			log.Fatalf("server.ListenAndServe err: %v", err)
		}
	}()

	// 优雅退出
	quit := shutdown.New(10)
	quit.Add(syscall.SIGINT, syscall.SIGTERM)
	quit.Start(server)
}
```
定义reporter用于上报数据给Skywalking，这就是一个简单的集成Trace的例子。
<a name="lfMEN"></a>
### 定义标准的日志
应用的可观测性主要来源日志、监控、链路追踪，标准的日志有利于日志收集以及排查问题。<br />原则上，不论是什么类型的日志输出，什么格式的日志内容，都能收集。但是为了方便友好，建议把日志输出到标准输出，这样收集更方便。<br />个人理解，在K8s中，完全没必要把日志输出到文件，浪费不说，没多大意义，因为所有的日志都会收集到日志系统，而输出到文件的日志也会随着应用发版而丢失，所以输出到文件的意义是什么呢？
<a name="doXHM"></a>
## 运维侧
开发把系统开发完，就会交付给运维部署。为了保障应用的稳定性，运维在部署应用的时候应该考虑以下几点。

- 应用尽可能保持无状态
- 应用尽可能保持高可用
- 应该具备优雅上线能力
- 应该具备异常自愈能力
- 可以使用HTTPS访问
<a name="fF5p2"></a>
### 应用尽可能保持无状态
K8S中可以部署有状态应用，也可以部署无状态应用。对于有状态应用，其实很少部署到K8S中，大部分还是部署的无状态应用，至于为什么，用多了就晓得了。<br />对于业务应用，强烈建议使其保持无状态，就算有需要持久化的东西，要么保存到数据库，要么保存到对象存储或者其他单独的文件系统中，不要挂载到应用Pod上。<br />这样的好处是，应用和数据是分开的，应用可以随意启停、扩展、迁移等。
<a name="D2Ics"></a>
### 应用尽可能的保持高可用
保持高可用应该是每个运维人员的使命。<br />在K8S中，应该怎么配置呢？<br />（1）应用Pod应该是多副本<br />（2）应用Pod之间做反亲和性，避免同一应用调度到同一台主机，如下。
```yaml
......
spec:
  affinity:
    podAntiAffinity:
      requiredDuringSchedulingIgnoredDuringExecution:
          - labelSelector:
            matchExpressions:
              - key: app
                operator: In
                values: [ "httpserver" ]
            topologyKey: kubernetes.io/hostname
......
```
（3） 为了避免应用因为节点维护等原因驱逐Pod，导致全部Pod被驱逐，特别配置了PodDisruptionBudget，保障应用至少有一个可用，如下。
```yaml
apiVersion: policy/v1beta1
kind: PodDisruptionBudget
metadata:
  name: httpserver
spec:
  minAvailable: 1
  selector:
    matchLables:
      app: httpserver
```
（4）如果某个节点因为一些原因需要驱逐一些Pod，为了避免重要应用被驱逐，应该给应用配置较高的QoS，如下：
```yaml
resources:
  limits:
    cpu: "1"
    memory: 2Gi
  requests:
    cpu: "1"
    memory: 2Gi
```
<a name="hBju6"></a>
### 应用具备优雅上线能力
所谓优雅上线能力，就是要确保应用能够提供服务了，再接入外界流量，不能在还没完全启动的情况下就提供服务。<br />在K8S中，应用在启动后会加入endpoints中，然后通过service接入流量，那在什么情况下才算启动成功呢？主要是通过K8S的ReadinessProbe来进行检测。这时候开发的健康检测接口就派上用场了，如下：
```yaml
...
readinessProbe:
  failureThreshold: 3
  httpGet:
    path: /health
    port: http
    scheme: HTTP
  initialDelaySeconds: 20
  periodSeconds: 10
  successThreshold: 1
  timeoutSeconds: 3
...

```
所以K8S的YAML文件应该加上如上的配置。
<a name="lklXx"></a>
### 应该具备异常自愈能力
所谓异常自愈，就是应用本身在出现Crash，或者应用Pod所在节点出现异常的情况，应用能够自动重启或者迁移。这时候就需要通过K8S的LivenessProbe来进行检测了，如下。
```yaml
......
livenessProbe:
  failureThreshold: 3
  httpGet:
    path: /health
    port: http
    scheme: HTTP
  initialDelaySeconds: 30
  periodSeconds: 10
  successThreshold: 1
  timeoutSeconds: 3
......
```
当K8S的YAML清单加上如上配置过后，就会定时去探测应用是否正常，如果异常，就会触发重启的动作。如果是节点异常，K8S会对Pod进行重新调度。
<a name="jITTf"></a>
### 可以使用HTTPS进行访问
应用通过HTTPS访问是比较常见的，企业级应用建议自己购买相应的SSL证书，然后进行配置即可。<br />比如。
```bash
# 创建证书secret
kubectl create secret tls httpserver-tls-secret --cert=path/to/tls.cert --key=path/to/tls.key
```
```python
# 在ingress中引用
......
spec:
  tls:
    hosts:
      - httpserver.coolops.cn
    secretName: httpserver-tls-secret
  rules:
    - host: httpserver.coolops.cn
......
```
<a name="hTRB9"></a>
## 总结
上面介绍了开发和运维对于应用上线应该做的工作，不全但够用。<br />在不同的企业都有不同的习性，但是作为运维，都要牢牢记住稳定永远是第一习性。通过上面的梳理，应用模板就整理如下：
```python
apiVersion: apps/v1
kind: Deployment
metadata:
  labels:
    app: httpserver
  name: httpserver
  namespace: default
spec:
  progressDeadlineSeconds: 600
  replicas: 2
  revisionHistoryLimit: 10
  selector:
    matchLabels:
      app: httpserver
  strategy:
    rollingUpdate:
      maxSurge: 25%
      maxUnavailable: 25%
    type: RollingUpdate
  template:
    metadata:
      annotations:
        prometheus.io/scrape: "true"
        prometheus.io/port: "metrics"
      labels:
        app: httpserver
    spec:
      affinity:
        podAntiAffinity:
          requiredDuringSchedulingIgnoredDuringExecution:
            - labelSelector:
                matchExpressions:
                  - key: app
                    operator: In
                    values: [ "httpserver" ]
              topologyKey: kubernetes.io/hostname
      containers:
        - env:
            - name: TZ
              value: Asia/Shanghai
            - name: POD_NAME
              valueFrom:
                fieldRef:
                  apiVersion: v1
                  fieldPath: metadata.name
            - name: POD_NAMESPACE
              valueFrom:
                fieldRef:
                  apiVersion: v1
                  fieldPath: metadata.namespace
          image: baidjay/httpserver:ubuntu-v3-metrics
          imagePullPolicy: IfNotPresent
          lifecycle:
            preStop:
              exec:
                command:
                  - /bin/sh
                  - -c
                  - sleep 15
          livenessProbe:
            failureThreshold: 3
            httpGet:
              path: /healthz
              port: http
              scheme: HTTP
            initialDelaySeconds: 30
            periodSeconds: 10
            successThreshold: 1
            timeoutSeconds: 3
          name: httpserver
          ports:
            - containerPort: 8080
              name: http
              protocol: TCP
            - name: metrics
              protocol: TCP
              containerPort: 9527
          readinessProbe:
            failureThreshold: 3
            httpGet:
              path: /healthz
              port: http
              scheme: HTTP
            initialDelaySeconds: 20
            periodSeconds: 10
            successThreshold: 1
            timeoutSeconds: 3
          resources:
            limits:
              cpu: "1"
              memory: 2Gi
            requests:
              cpu: "1"
              memory: 2Gi
          securityContext: {}
          terminationMessagePath: /dev/termination-log
          terminationMessagePolicy: File
      dnsPolicy: ClusterFirst
      restartPolicy: Always
      schedulerName: default-scheduler
---
apiVersion: v1
kind: Service
metadata:
  name: httpserver
spec:
  ports:
    - name: http
      port: 8080
      protocol: TCP
      targetPort: http
    - name: metrics
      port: 9527
      protocol: TCP
      targetPort: metrics
  selector:
    app: httpserver
  sessionAffinity: None
  type: ClusterIP
---
apiVersion: networking.k8s.io/v1
kind: Ingress
metadata:
  annotations:
    nginx.ingress.kubernetes.io/proxy-body-size: 100m
    nginx.ingress.kubernetes.io/proxy-connect-timeout: "600"
    nginx.ingress.kubernetes.io/proxy-read-timeout: "600"
    nginx.ingress.kubernetes.io/proxy-send-timeout: "600"
    nginx.ingress.kubernetes.io/service-weight: ""
    nginx.org/client-max-body-size: 100m
  name: httpserver-tls
spec:
  tls:
  - hosts:
      - httpserver.coolops.cn
    secretName: httpserver-tls-secret
  rules:
    - host: httpserver.coolops.cn
      http:
        paths:
          - pathType: Prefix
            path: /
            backend:
              service:
                name: httpserver
                port:
                  number: 8080
---
apiVersion: policy/v1
kind: PodDisruptionBudget
metadata:
  name: httpserver
spec:
  minAvailable: 1
  selector:
    matchLabels:
      app: httpserver
```
