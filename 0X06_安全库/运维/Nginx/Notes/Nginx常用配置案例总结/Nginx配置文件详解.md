Nginx<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624285810273-27286929-5a7d-47e5-a73b-86fe5303c5de.webp#clientId=u69879262-d36e-4&from=paste&id=ua0d2fac2&originHeight=794&originWidth=615&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=none&taskId=u36b3b110-18b2-4d71-b223-1e6d70b8ef1&title=)
<a name="tW6l3"></a>
## Nginx安装
<a name="Rq8v6"></a>
### Nginx安装完成，没有sbin目录
cd进入到nginx-1.18.0目录内执行
```bash
[root@centos7 nginx-1.18.0]# ./configure --prefix=/usr/local/nginx
[root@centos7 nginx-1.18.0]# make
[root@centos7 nginx-1.18.0]# make install
```
ps：`–prefix=path` 定义一个目录，存放服务器上的文件 ，也就是nginx的安装目录，默认使用 /usr/local/nginx<br />这步会看到local目录下有一个跟安装的nginx1.12.2同级的nginx目录，里面就有sbin目录了。
<a name="F89CW"></a>
## 基础指令
<a name="h3AhE"></a>
### `listen`
该指令用于配置网络监听。<br />1  `listen *:80 | *:8080` #监听所有80端口和8080端口<br />2  `listen  IP_address:port`   #监听指定的地址和端口号<br />3  `listen  IP_address`     #监听指定ip地址所有端口<br />4  `listen port`     #监听该端口的所有IP连接
<a name="V1GcJ"></a>
### `server_name`
该指令用于虚拟主机的配置。<br />a. 基于名称的虚拟主机配置
```nginx
server_name   name ...;
```
b. 基于 IP 地址的虚拟主机配置
```nginx
server_name 192.168.1.1
location
```
该指令用于匹配 URL。<br />`location`指令的作用是根据用户请求的URI来执行不同的应用，也就是根据用户请求的网站URL进行匹配，匹配成功即进行相关的操作。
<a name="Tcqta"></a>
#### `location`的语法
`=`开头表示精确匹配<br />如 A 中只匹配根目录结尾的请求，后面不能带任何字符串。<br />`^~` 开头表示uri以某个常规字符串开头，不是正则匹配<br />`~` 开头表示区分大小写的正则匹配;<br />`~*` 开头表示不区分大小写的正则匹配<br />`/` 通用匹配, 如果没有其它匹配,任何请求都会匹配到
<a name="s9p4c"></a>
#### Location正则案例
```nginx
#精确匹配,/后面不能带任何字符
server {
  listen       80;
  server_name  www.itmayiedu.com;
  #精确匹配,注解后面不能带任何字符
  location =/ {
    proxy_pass http://127.0.0.1:8080;
    index  index.html index.htm;
  }
}

#匹配所有以/开头请求
server {
  listen       80;
  server_name  www.itmayiedu.com;
  #匹配所有以/开头请求 
  location / {
    proxy_pass http://127.0.0.1:8080;
    index  index.html index.htm;
  }
}
###  以开头/itmayiedu_8080拦截  默认开启不区分大小写
server {
  listen       80;
  server_name  www.itmayiedu.com;
  ###  以开头/itmayiedu_8080 最终跳转到http://127.0.0.1:8080/;
  location /itmayiedu_8080/ {
    proxy_pass http://127.0.0.1:8080/;
    index  index.html index.htm;
  }
  ###  以开头/itmayiedu_8080 最终跳转到http://127.0.0.1:8081/;
  location /itmayiedu_8081/ {
    proxy_pass http://127.0.0.1:8081/;
    index  index.html index.htm;
  }
}
### 开头区分大小写
```
<a name="ZxPaX"></a>
### `proxy_pass`
该指令用于设置被代理服务器的地址。可以是主机名称、IP地址加端口号的形式。<br />语法结构如下：
```nginx
proxy_pass URL;
index
```
该指令用于设置网站的默认首页。
<a name="Lb8pc"></a>
### 域名重定向
```nginx
server{
  listen 80 ;
  server_name mxiaoqi.top aaa.com;
  if ( $host = mxiaoqi.top )
    #增加判断条件，当访问域名是mxiaoqi.top的时候
    {
    rewrite /(.*)  http://aaa.com/$1 permanent;
    #把mxiaoqi.top/后面的内容重新写到aaa.com/后面如果后面有多段则使用$2、$3以此类推
    #permanent是转发状态码
  }
  index index.html index.htm index.php;
  root /data/wwwroot/mxiaoqi.top;
}
```
<a name="oVGpu"></a>
### 反向代理
使用 nginx 反向代理 www.123.com 直接跳转到127.0.0.1:8080
```nginx
server {
  listen       80;
  server_name  www.123.com;

  location / {
    proxy_pass http://127.0.0.1:8080;
    # 欢迎页面，按照从左到右的顺序查找页面
    index  index.html index.htm index.jsp;
  }
}
```
监听80端口，访问域名为www.123.com，不加端口号时默认为80端口，故访问该域名时会跳转到127.0.0.1:8080路径上。
<a name="Smdiz"></a>
### 限流配置
<a name="zJOzP"></a>
#### 漏桶算法与令牌桶算法区别
主要区别在于“漏桶算法”能够强行限制数据的传输速率，<br />而“令牌桶算法”在能够限制数据的平均传输速率外，还允许某种程度的突发传输。在“令牌桶算法”中，只要令牌桶中存在令牌，那么就允许突发地传输数据直到达到用户配置的门限，因此它适合于具有突发特性的流量。<br />Nginx按请求速率限速模块使用的是漏桶算法，即能够强行保证请求的实时处理速度不会超过设置的阈值。

- `limit_req_zone` 用来限制单位时间内的请求数，即速率限制,采用的漏桶算法 "leaky bucket"。
- `limit_req_conn` 用来限制同一时间连接数，即并发限制。
<a name="dLyXI"></a>
#### limit_req_zone 参数配置
Syntax:    `limit_req zone=name [burst=number] [nodelay];`<br />Default:    —<br />Context:    `http`, `server`, `location`
```nginx
limit_req_zone $binary_remote_addr zone=one:10m rate=1r/s;
```

- 第一个参数：`$binary_remote_addr` 表示通过`remote_addr`这个标识来做限制，“`binary_`”的目的是缩写内存占用量，是限制同一客户端ip地址。
- 第二个参数：`zone=one:10m`表示生成一个大小为10M，名字为one的内存区域，用来存储访问的频次信息。
- 第三个参数：rate=1r/s表示允许相同标识的客户端的访问频次，这里限制的是每秒1次，还可以有比如30r/m的。
```nginx
limit_req zone=one burst=5 nodelay;
```

- 第一个参数：`zone=one` 设置使用哪个配置区域来做限制，与上面`limit_req_zone` 里的name对应。
- 第二个参数：`burst=5`，重点说明一下这个配置，`burst`爆发的意思，这个配置的意思是设置一个大小为5的缓冲区当有大量请求（爆发）过来时，超过了访问频次限制的请求可以先放到这个缓冲区内。
- 第三个参数：nodelay，如果设置，超过访问频次而且缓冲区也满了的时候就会直接返回503，如果没有设置，则所有请求会等待排队。
<a name="pF6TW"></a>
#### `ngx_http_limit_conn_module` 参数配置
这个模块用来限制单个IP的请求数。并非所有的连接都被计数。只有在服务器处理了请求并且已经读取了整个请求头时，连接才被计数。<br />Syntax:    `limit_conn zone number;`<br />Default:    —<br />Context:    `http`, `server`, `location`

```nginx
limit_conn_zone $binary_remote_addr zone=addr:10m;
server {
  location /download/ {
    limit_conn addr 1;
  }
```
一次只允许每个IP地址一个连接。
<a name="FDMjz"></a>
### 负载均衡
```nginx
#user  nobody;
worker_processes  1;
error_log  logs/error.log;# 开启日志
pid        logs/nginx.pid;
...
upstream lyf {
  server 192.168.37.220:8001; # 3个tomcat服务
  server 192.168.37.220:8002;
  server 192.168.37.220:8003;
}
server {
  listen       80;
  server_name  192.168.37.220;# 监听ip
  location / {
    proxy_pass   http://lyf;  # 设置代理
    index  index.html index.htm;
  }
}
```
<a name="bEnTY"></a>
#### `keepalive` 长连接提高吞吐量
`keepalived`：设置长连接处理的数量<br />`proxy_http_version`：设置长连接http版本为1.1<br />`proxy_set_header`：清除connection header 信息
```nginx
upstream tomcats { 
  # server 192.168.1.173:8080 max_fails=2 fail_timeout=1s; 
  server 192.168.1.190:8080; 
  # server 192.168.1.174:8080 weight=1; 
  # server 192.168.1.175:8080 weight=1; 
  keepalive 32; 
}
server {
  listen 80; 
  server_name www.tomcats.com; 
  location / { 
    proxy_pass http://tomcats; 
    proxy_http_version 1.1; 
    proxy_set_header Connection ""; 
  } 
}
```
<a name="AWI8b"></a>
#### 工作方式
1. 轮询方式是Nginx负载默认的方式<br />2. 权重方式 指定每个服务的权重比例，weight和访问比率成正比
```nginx
upstream  dalaoyang-server {
  server    localhost:10001 weight=1;
  server    localhost:10002 weight=2;
}
```
3. `iphash`<br />每个请求都根据访问ip的hash结果分配，经过这样的处理，每个访客固定访问一个后端服务，如下配置（ip_hash可以和weight配合使用）。
```nginx
upstream  dalaoyang-server {
  ip_hash; 
  server    localhost:10001 weight=1;
  server    localhost:10002 weight=2;
}
```
4. 最少连接<br />将请求分配到连接数最少的服务上。
```nginx
upstream  dalaoyang-server {
  least_conn;
  server    localhost:10001 weight=1;
  server    localhost:10002 weight=2;
}
```
5.fair<br />按后端服务器的响应时间来分配请求，响应时间短的优先分配。
```nginx
upstream  dalaoyang-server {
  server    localhost:10001 weight=1;
  server    localhost:10002 weight=2;
  fair;  
}
```
<a name="QoULg"></a>
#### Consul+upsync+Nginx  实现无需raload动态负载均衡
传统的负载均衡，如果Upstream参数发生变化，每次都需要重新加载nginx.conf文件,<br />因此扩展性不是很高，所以可以采用动态负载均衡，实现Upstream可配置化、动态化，无需人工重新加载nginx.conf。
<a name="Fw45f"></a>
#### LVS+Keepalived+Nginx+Tomcat搭建高可用双机主从热备集群
需要明确的是：

1. Nginx两台是主备关系，只有一台在工作。后面的tomcat是集群，同时工作的。
2. keepalived是同时安装在两台Nginx上的，不过文件配置不一样
3. 这里的双机热备是指LVS，Nginx则是集群
<a name="E84Qz"></a>
##### keepalived
健康检查和失败切换是keepalived的两大核心功能。所谓的健康检查，就是采用tcp三次握手，icmp请求，http请求，udp echo请求等方式对负载均衡器后面的实际的服务器(通常是承载真实业务的服务器)进行保活；而失败切换主要是应用于配置了主备模式的负载均衡器，利用VRRP维持主备负载均衡器的心跳，当主负载均衡器出现问题时，由备负载均衡器承载对应的业务，从而在最大限度上减少流量损失，并提供服务的稳定性。<br />LVS是Linux Virtual Server的简写，意即Linux虚拟服务器，是一个虚拟的服务器集群系统。lvs目前是集成在Linux内的。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624285810441-8fd2bbd4-18bc-49ae-9206-6bc5ab63c444.webp#clientId=u69879262-d36e-4&from=paste&id=u3c6ae4e1&originHeight=709&originWidth=808&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2675d565-ee14-4769-9f8a-7e9086490b0&title=)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624285810483-952eeb91-bd02-4b81-8c8a-0be820649307.webp#clientId=u69879262-d36e-4&from=paste&id=u7570a78a&originHeight=566&originWidth=1080&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue51f2cb5-815a-4f60-a704-8b21b0a6061&title=)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624285810296-a6804a25-875f-46fb-851f-ff4e6bff955a.webp#clientId=u69879262-d36e-4&from=paste&id=ucc8037df&originHeight=632&originWidth=1080&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc81eb72c-1d07-4812-a756-96aecd2e8fe&title=)
<a name="NUOng"></a>
#### 为什么要LVS+Nginx？
<a name="L50hP"></a>
##### 一、ngix（应用层 网络七层负载均衡）
1、异步转发，请求数据和相应数据都要经过ngix，ngix和客户端建立连接<br />2、轮询所有的tomcat服务器，保证请求成功或者最后一台tomcat服务器也请求失败
<a name="RThfw"></a>
##### 二、lvs(网络层  网络四层负载均衡)
1、同步转发接受请求数据，lvs转发到服务器，服务器直接和客户端建立连接<br />nginx要承受所有的流量，当一台nigx承受不了，就需要搭建ngix集群。ngix+ngix 外层的ngix还是要承受所有流量。<br />lvs+ngix:lvs同步转发不会接受相应数据，LVS采用DR模式时不用响应服务器返回的内容。（通常请求数据是比较小的，响应数据会比较大）<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624285810293-65df4160-1fd2-4b7a-aa1b-637ecf06cdb8.webp#clientId=u69879262-d36e-4&from=paste&id=ud9d4729b&originHeight=670&originWidth=1080&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=u266815fc-1106-44e5-8cb6-3529cd2cea8&title=)
<a name="uWa3a"></a>
### 静态资源配置
```nginx
location ~ .*\.(jpg|gif|png)$ {
  gzip on;
  gzip_http_version 1.1;
  gzip_comp_level 2;
  gzip_types text/plain application/javascript image/jpeg image/gif image/png;
  root   /usr/share/nginx/images;
}

location ~ .*\.(txt|xml)$ {
  gzip on;
  gzip_http_version 1.1;
  gzip_comp_level 1;
  gzip_types text/plain application/javascript image/jpeg image/gif image/png;
  root   /usr/share/nginx/code;
}
location ~ ^/download {
  gzip_static on;
  tcp_nopush on;
  root /opt/app/code;
}
```
`sendfile on | off`，文件读取配置<br />默认`sendfile`是关闭的，可以配置在`http`,`server`,`location`,`if in location`中<br />`tcp_nopush on | off`，多个包整合<br />默认是关闭状态，可以在`http`,`server`,`location`中配置，它的作用是在`sendfile`开启的情况下，提高网络包的传输效率。什么意思呢，假设服务端收到请求，需要推送10个包，为了提高传输效率，这10个包不会一个一个返回给客户端，而是将10个包攒够了后一起返回回去。<br />`tcp_nodelay on | off`，网络包的实时性传输<br />默认开启，可以在`http`,`server`,`location`中配置，它的作用是在`keepalive`链接下，提高网络包的传输实时性。<br />`gzip on | off`，压缩<br />默认是关闭状态，可以在`http`,`server`,`location`,`if in location`中配置，作用是压缩传输。一般来说浏览器是可以对压缩后的内容进行解压的。<br />`gzip_comp_level level`;压缩级别<br />默认的压缩级别是`1`，可以在`http`,`server`,`location`中配置，级别配置的越高，压缩的越好，但是压缩会耗费服务端的计算资源，所以要控制好压缩级别<br />`gzip_http_version 1.0 | 1.1`，压缩对http协议的支持<br />默认对HTTP/1.1协议的请求才会进行gzip压缩,可以配置在`http`,`server`,`location`中配置。当用户想要读取一个1.html文件，首先会在目录中找寻1.html.gz是否存在，所以这就导致了磁盘资源的浪费，必须要存储两份文件。
```nginx
###静态资源访问
server {
  listen       80;
  server_name  static.itmayiedu.com;
  location /static/imgs {
    root F:/;
    index  index.html index.htm;
  }
}
###动态资源访问
server {
  listen       80;
  server_name  www.itmayiedu.com;

  location / {
    proxy_pass http://127.0.0.1:8080;
    index  index.html index.htm;
  }
}
```
<a name="rh4MM"></a>
### 跨域配置
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624285812854-ad590ae9-1b17-4f44-a8da-ec1602749034.webp#clientId=u69879262-d36e-4&from=paste&id=ud650a41e&originHeight=425&originWidth=1080&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4a0e2d41-88e8-4ee9-8efd-241d9645117&title=)<br />跨域就是在原站点访问域名不同的其他站点，同源策略会阻止一个域的javascript脚本和另外一个域的内容进行交互。所谓同源（即指在同一个域）就是两个页面具有相同的协议（protocol），主机（host）和端口号（port）。<br />CORS 是跨域资源分享（Cross-Origin Resource Sharing）的缩写。它是 W3C 标准，属于跨源 AJAX 请求的根本解决方法。<br />1、普通跨域请求：只需服务器端设置Access-Control-Allow-Origin<br />2、带cookie跨域请求：前后端都需要进行设置
```nginx
#允许跨域请求的域，*代表所有
add_header 'Access-Control-Allow-Origin' *;
#允许带上cookie请求 
add_header 'Access-Control-Allow-Credentials' 'true'; 
#允许请求的方法，比如 GET/POST/PUT/DELETE 
add_header 'Access-Control-Allow-Methods' *; 
#允许请求的header 
add_header 'Access-Control-Allow-Headers' *;
```
<a name="mtFwr"></a>
### 防盗链
```nginx
#对源站点验证
valid_referers *.imooc.com; 
#非法引入会进入下方判断 
if ($invalid_referer) { return 404; }
```
