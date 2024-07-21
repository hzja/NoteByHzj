Kubernetes Nginx-ingress
<a name="kz2K5"></a>
## 主机 Nginx
一般 nginx 做主机反向代理（网关）有以下配置
```nginx
upstream order{
    server 192.168.1.10:5001;
    server 192.168.1.11:5001;
}

server {
    listen 80;
    server_name  order.example.com;
    access_log     /var/log/nginx/order.example.com-access.log;
    error_log     /var/log/nginx/order.example.com-error.log;
    location / {
        proxy_pass_header Server;
        proxy_set_header Host $http_host;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header X-Scheme $scheme;
        proxy_pass http://order;
    }
}
```
其中 192.168.1.10:5001，192.168.1.10:5001 把他们称为 Endpoint，就是所谓的具体的服务，比如 order 订单服务。
<a name="LfZ0f"></a>
## pod nginx-ingress
nginx-ingress也是一种代理，是一个pod，外部的数据统一经过（必经）这个pod，然后通过该pod内部的nginx方向代理到各各服务（Endpoint）。nginx-ingress是ingress控制器插件的一种，这些插件有很多，比如istio-ingressgateway。
<a name="o0AXG"></a>
### 1、Pod
nginx-ingress pod有两个功能，controller 和 nginx：<br />controller：和kubernetes api通讯实时更新nginx配置（就是ingress yaml资源了） nginx：正常的反向代理<br />与主机nginx的区别是，该pod nginx-ingress是运行在pod里。主机在定义反向代理配置文件时，需要监听一个对外开放的端口，比如上边的80端口。那么pod中的nginx端口是如何配置的呢？<br />在github上找到了nginx-ingress的deployment.yaml<br />[https://raw.githubusercontent.com/kubernetes/ingress-nginx/master/deploy/static/mandatory.yaml](https://raw.githubusercontent.com/kubernetes/ingress-nginx/master/deploy/static/mandatory.yaml)<br />其中一段
```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: nginx-ingress-controller
  namespace: ingress-nginx
  labels:
    app.kubernetes.io/name: ingress-nginx
    app.kubernetes.io/part-of: ingress-nginx
spec:
  replicas: 1
  selector:
    matchLabels:
      app.kubernetes.io/name: ingress-nginx
      app.kubernetes.io/part-of: ingress-nginx
  template:
    metadata:
      labels:
        app.kubernetes.io/name: ingress-nginx
        app.kubernetes.io/part-of: ingress-nginx
      annotations:
        prometheus.io/port: "10254"
        prometheus.io/scrape: "true"
    spec:
      # wait up to five minutes for the drain of connections
      terminationGracePeriodSeconds: 300
      serviceAccountName: nginx-ingress-serviceaccount
      containers:
        - name: nginx-ingress-controller
          image: quay.io/kubernetes-ingress-controller/nginx-ingress-controller:0.26.1
          ...
          ...
          ...
          ports:
            - name: http
              containerPort: 80
            - name: https
              containerPort: 443
```
可以看到
```yaml
- name: http
  containerPort: 80
- name: https
  containerPort: 443
```
默认对外监听了两个端口80和443，也就是说，有这两个端口对外就可以web服务了。
<a name="UY9xs"></a>
### 2、ingress 资源
ingress 资源通过yaml进行管理的，比如以下：
```yaml
apiVersion: extensions/v1beta1
kind: Ingress
metadata:
  name: order
spec: 
  rules:
  - host: order.example.com
    http:
      paths: /
      backend: 
        serviceName: order
        servicePort: 80
```
以上定义了一个单一规则的ingress，该pod（nginx-ingress）接收到外部所有的请求，将被发送到内部order服务的80端口上。接下来看pod（nginx-ingress）如何把ingress资源转化为该pod中的nginx反向代理配置文件
```nginx
upstream order{
    server order:80;
}

server {
    listen 80;
    server_name  order.example.com;
    ...
    ...
    location / {
        proxy_pass_header Server;
        proxy_set_header Host $http_host;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header X-Scheme $scheme;
        proxy_pass http://order; # 对应ingress 资源 name: order
    }
}
```
当然ingress如果包含https，那么会转化nginx对应的443端口及证书的配置文件内容，这里就不写了。<br />那么，单一个规则的ingress资源代理多个服务（比如order服务，product服务）或者多个ingress资源文件如何转化为nginx配置？猜测，其实就是转化成了多个。
```nginx
upstream order{
    server order:80;
}
```
当然，被转化的nginx配置文件要比这些复杂的多，据说还是用lua脚本写的，灵活如openresty。
<a name="Rnl7L"></a>
### 3、nginx-ingress对外提供服务
一般来讲，pod直接对外提供服务就只有两种方式：

- create一个service，该service暴漏nodePort
- forward 映射

一般采用第一种。<br />nginx-ingress也是一个pod，所以，为了能使外部通过该pod代理访问，还需要nginx-ingress对外提供一个nodePort的service。这个service这里也不再写了。
<a name="XjYSi"></a>
### 4、nginx-ingress工作流程
![](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1631321125610-40478752-9667-485a-ae15-321015dbae6c.jpeg)<br />可以看到，因为 nginx-ingress 这个pod做了所有service的代理，在高并发情况下将承受巨大压力，可以增加多个pod实例。
