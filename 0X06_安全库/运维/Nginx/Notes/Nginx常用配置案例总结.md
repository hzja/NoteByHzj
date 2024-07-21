Nginx
<a name="3AaGv"></a>
## 静态代理
Nginx擅长处理静态文件，是非常好的图片、文件服务器。把所有的静态资源的放到nginx上，可以使应用动静分离，性能更好。
<a name="PT5TM"></a>
## HTTP反向代理配置
nginx.conf 配置文件如下：
:::tips
**注：conf/nginx.conf 是 nginx 的默认配置文件。也可以使用 **`**nginx -c**`** 指定配置文件**
:::
```nginx
#运行用户
#user somebody;
#启动进程,通常设置成和cpu的数量相等
worker_processes  1;
#全局错误日志
error_log  D:/Tools/nginx-1.10.1/logs/error.log;
error_log  D:/Tools/nginx-1.10.1/logs/notice.log  notice;
error_log  D:/Tools/nginx-1.10.1/logs/info.log  info;
#PID文件，记录当前启动的nginx的进程ID
pid        D:/Tools/nginx-1.10.1/logs/nginx.pid;
#工作模式及连接数上限
events {
  worker_connections 1024;    #单个后台worker process进程的最大并发链接数
}
#设定http服务器，利用它的反向代理功能提供负载均衡支持
http {
  #设定mime类型(邮件支持类型),类型由mime.types文件定义
  include       D:/Tools/nginx-1.10.1/conf/mime.types;
  default_type  application/octet-stream;
  #设定日志
  log_format  main  '[$remote_addr] - [$remote_user] [$time_local] "$request" '
    '$status $body_bytes_sent "$http_referer" '
    '"$http_user_agent" "$http_x_forwarded_for"';
  access_log    D:/Tools/nginx-1.10.1/logs/access.log main;
  rewrite_log     on;
  #sendfile 指令指定 nginx 是否调用 sendfile 函数（zero copy 方式）来输出文件，对于普通应用，
  #必须设为 on,如果用来进行下载等应用磁盘IO重负载应用，可设置为 off，以平衡磁盘与网络I/O处理速度，降低系统的uptime.
  sendfile        on;
  #tcp_nopush     on;
  #连接超时时间
  keepalive_timeout  120;
  tcp_nodelay        on;
  #gzip压缩开关
  #gzip  on;
  #设定实际的服务器列表
  upstream zp_server1{
    server 127.0.0.1:8089;
  }
  #HTTP服务器
  server {
    #监听80端口，80端口是知名端口号，用于HTTP协议
    listen       80;
    #定义使用www.xx.com访问
    server_name  www.helloworld.com;
    #首页
    index index.html
      #指向webapp的目录
      root D:\01_Workspace\Project\github\zp\SpringNotes\spring-security\spring-shiro\src\main\webapp;
    #编码格式
    charset utf-8;
    #代理配置参数
    proxy_connect_timeout 180;
    proxy_send_timeout 180;
    proxy_read_timeout 180;
    proxy_set_header Host $host;
    proxy_set_header X-Forwarder-For $remote_addr;
    #反向代理的路径（和upstream绑定），location 后面设置映射的路径
    location / {
      proxy_pass http://zp_server1;
    }
    #静态文件，nginx自己处理
    location ~ ^/(images|javascript|js|css|flash|media|static)/ {
      root D:\01_Workspace\Project\github\zp\SpringNotes\spring-security\spring-shiro\src\main\webapp\views;
      #过期30天，静态文件不怎么更新，过期可以设大一点，如果频繁更新，则可以设置得小一点。
      expires 30d;
    }
    #设定查看Nginx状态的地址
    location /NginxStatus {
      stub_status           on;
      access_log            on;
      auth_basic            "NginxStatus";
      auth_basic_user_file  conf/htpasswd;
    }
    #禁止访问 .htxxx 文件
    location ~ /\.ht {
      deny all;
    }
    #错误处理页面（可选择性配置）
    #error_page   404              /404.html;
    #error_page   500 502 503 504  /50x.html;
    #location = /50x.html {
    #    root   html;
    #}
  }
}
```

1. 启动 webapp，注意启动绑定的端口要和 nginx 中的 `upstream` 设置的端口保持一致。
2. 更改 host：在 C:\Windows\System32\drivers\etc 目录下的 host 文件中添加一条 DNS 记录
```
127.0.0.1 www.helloworld.com
```

3. 启动 startup.bat 的脚本命令
```powershell
@echo off
rem 如果启动前已经启动nginx并记录下pid文件，会kill指定进程
nginx.exe -s stop

rem 测试配置文件语法正确性
nginx.exe -t -c conf/nginx.conf

rem 显示版本信息
nginx.exe -v

rem 按照指定配置去启动nginx
nginx.exe -c conf/nginx.conf
```

4. 在浏览器中访问 www.helloworld.com，不出意外，已经可以访问了。
<a name="M79Jj"></a>
## 负载均衡配置
Nginx通过反向代理可以实现服务的负载均衡，避免了服务器单节点故障，把请求按照一定的策略转发到不同的服务器上，达到负载的效果。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617946022575-bce63015-e96f-434f-829d-b2589ff374b5.webp#averageHue=%233a3530&height=424&id=JBl7q&originHeight=424&originWidth=640&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=640)<br />网站在实际运营过程中，多半都是有多台服务器运行着同样的 app，这时需要使用负载均衡来分流。<br />nginx 也可以实现简单的负载均衡功能。<br />假设这样一个应用场景：将应用部署在 192.168.1.11:80、192.168.1.12:80、192.168.1.13:80 三台 linux 环境的服务器上。网站域名叫 www.helloworld.com，公网 IP 为 192.168.1.11。在公网 IP 所在的服务器上部署 nginx，对所有请求做负载均衡处理。<br />nginx.conf 配置如下：
```nginx
http {
  #设定mime类型,类型由mime.type文件定义
  include       /etc/nginx/mime.types;
  default_type  application/octet-stream;
  #设定日志格式
  access_log    /var/log/nginx/access.log;
  #设定负载均衡的服务器列表
  upstream load_balance_server {
    #weigth参数表示权值，权值越高被分配到的几率越大
    server 192.168.1.11:80   weight=5;
    server 192.168.1.12:80   weight=1;
    server 192.168.1.13:80   weight=6;
  }
  #HTTP服务器
  server {
    #侦听80端口
    listen       80;
    #定义使用www.xx.com访问
    server_name  www.helloworld.com;
    #对所有请求进行负载均衡请求
    location / {
      root        /root;                 #定义服务器的默认网站根目录位置
      index       index.html index.htm;  #定义首页索引文件的名称
      proxy_pass  http://load_balance_server ;#请求转向load_balance_server 定义的服务器列表
      #以下是一些反向代理的配置(可选择性配置)
      #proxy_redirect off;
      proxy_set_header Host $host;
      proxy_set_header X-Real-IP $remote_addr;
      #后端的Web服务器可以通过X-Forwarded-For获取用户真实IP
      proxy_set_header X-Forwarded-For $remote_addr;
      proxy_connect_timeout 90;          #nginx跟后端服务器连接超时时间(代理连接超时)
      proxy_send_timeout 90;             #后端服务器数据回传时间(代理发送超时)
      proxy_read_timeout 90;             #连接成功后，后端服务器响应时间(代理接收超时)
      proxy_buffer_size 4k;              #设置代理服务器（nginx）保存用户头信息的缓冲区大小
      proxy_buffers 4 32k;               #proxy_buffers缓冲区，网页平均在32k以下的话，这样设置
      proxy_busy_buffers_size 64k;       #高负荷下缓冲大小（proxy_buffers*2）
      proxy_temp_file_write_size 64k;    #设定缓存文件夹大小，大于这个值，将从upstream服务器传
      client_max_body_size 10m;          #允许客户端请求的最大单文件字节数
      client_body_buffer_size 128k;      #缓冲区代理缓冲用户端请求的最大字节数
    }
  }
}
```
<a name="232IP"></a>
### 负载均衡策略
Nginx 提供了多种负载均衡策略
<a name="H0Iou"></a>
#### 轮询
将请求按顺序轮流地分配到后端服务器上，它均衡地对待后端的每一台服务器，而不关心服务器实际的连接数和当前的系统负载。
```nginx
upstream bck_testing_01 {
  # 默认所有服务器权重为 1
  server 192.168.250.220:8080
  server 192.168.250.221:8080
  server 192.168.250.222:8080
}
```
<a name="oJT1d"></a>
#### 加权轮询
不同的后端服务器可能机器的配置和当前系统的负载并不相同，因此它们的抗压能力也不相同。给配置高、负载低的机器配置更高的权重，让其处理更多的请；而配置低、负载高的机器，给其分配较低的权重，降低其系统负载，加权轮询能很好地处理这一问题，并将请求顺序且按照权重分配到后端。
```nginx
upstream bck_testing_01 {
  server 192.168.250.220:8080   weight=3
  server 192.168.250.221:8080              # default weight=1
  server 192.168.250.222:8080              # default weight=1
}
```
<a name="bef1ea04"></a>
#### 最少连接
由于后端服务器的配置不尽相同，对于请求的处理有快有慢，最小连接数法根据后端服务器当前的连接情况，动态地选取其中当前积压连接数最少的一台服务器来处理当前的请求，尽可能地提高后端服务的利用效率，将负责合理地分流到每一台服务器。
```nginx
upstream bck_testing_01 {
  least_conn;
  # with default weight for all (weight=1)
  server 192.168.250.220:8080
  server 192.168.250.221:8080
  server 192.168.250.222:8080
}
```
<a name="UjLir"></a>
#### 加权最少连接
```nginx
upstream bck_testing_01 {
  least_conn;
  server 192.168.250.220:8080   weight=3
  server 192.168.250.221:8080              # default weight=1
  server 192.168.250.222:8080              # default weight=1
}
```
<a name="t6cpH"></a>
#### IP Hash
[根据获取客户端的IP地址，通过哈希函数计算得到一个数值，用该数值对服务器列表的大小进行取模运算，得到的结果便是客户端要访问服务器的序号。采用源地址哈希法进行负载均衡，同一IP地址的客户端，当后端服务器列表不变时，它每次都会映射到同一台后端服务器进行访问。](https://mp.weixin.qq.com/s?__biz=MzI4NTM1NDgwNw==&mid=2247490951&idx=2&sn=3646cc9105a61e076d1b7118cc103dda&scene=21#wechat_redirect)
```nginx
upstream bck_testing_01 {
  ip_hash;
  # with default weight for all (weight=1)
  server 192.168.250.220:8080
  server 192.168.250.221:8080
  server 192.168.250.222:8080
}
```
<a name="vKGdg"></a>
#### 普通 Hash
```nginx
upstream bck_testing_01 {
  hash $request_uri;
  # with default weight for all (weight=1)
  server 192.168.250.220:8080
  server 192.168.250.221:8080
  server 192.168.250.222:8080
}
```
<a name="tkmoS"></a>
#### 随机
通过系统的随机算法，根据后端服务器的列表大小值来随机选取其中的一台服务器进行访问。

<a name="pMQID"></a>
## 网站有多个 webapp 的配置
当一个网站功能越来越丰富时，往往需要将一些功能相对独立的模块剥离出来，独立维护。这样的话，通常，会有多个 webapp。<br />举个例子：假如 www.helloworld.com 站点有好几个 webapp，finance（金融）、product（产品）、admin（用户中心）。访问这些应用的方式通过上下文(context)来进行区分:<br />www.helloworld.com/finance/<br />www.helloworld.com/product/<br />www.helloworld.com/admin/<br />http 的默认端口号是 80，如果在一台服务器上同时启动这 3 个 webapp 应用，都用 80 端口，肯定是不成的。所以，这三个应用需要分别绑定不同的端口号。<br />那么，问题来了，用户在实际访问 www.helloworld.com 站点时，访问不同 webapp，总不会还带着对应的端口号去访问吧。所以需要用到反向代理来做处理。
```nginx
http {
  #此处省略一些基本配置
  upstream product_server{
    server www.helloworld.com:8081;
  }
  upstream admin_server{
    server www.helloworld.com:8082;
  }
  upstream finance_server{
    server www.helloworld.com:8083;
  }
  server {
    #此处省略一些基本配置
    #默认指向product的server
    location / {
      proxy_pass http://product_server;
    }
    location /product/{
      proxy_pass http://product_server;
    }
    location /admin/ {
      proxy_pass http://admin_server;
    }
    location /finance/ {
      proxy_pass http://finance_server;
    }
  }
}
```
<a name="r9YMW"></a>
## HTTPS 反向代理配置
一些对安全性要求比较高的站点，可能会使用 HTTPS（一种使用 ssl 通信标准的安全 HTTP 协议）。<br />这里不科普 HTTP 协议和 SSL 标准。但是，使用 nginx 配置 https 需要知道几点：

- HTTPS 的固定端口号是 443，不同于 HTTP 的 80 端口
- SSL 标准需要引入安全证书，所以在 nginx.conf 中需要指定证书和它对应的 key

其他和 HTTP 反向代理基本一样，只是在 `Server` 部分配置有些不同。
```nginx
#HTTP服务器
server {
  #监听443端口。443为知名端口号，主要用于HTTPS协议
  listen       443 ssl;
  #定义使用www.xx.com访问
  server_name  www.helloworld.com;
  #ssl证书文件位置(常见证书文件格式为：crt/pem)
  ssl_certificate      cert.pem;
  #ssl证书key位置
  ssl_certificate_key  cert.key;
  #ssl配置参数（选择性配置）
  ssl_session_cache    shared:SSL:1m;
  ssl_session_timeout  5m;
  #数字签名，此处使用MD5
  ssl_ciphers  HIGH:!aNULL:!MD5;
  ssl_prefer_server_ciphers  on;
  location / {
    root   /root;
    index  index.html index.htm;
  }
}
```
<a name="E4Yrg"></a>
## 静态站点配置
有时候需要配置静态站点(即 html 文件和一堆静态资源)。<br />举例来说：如果所有的静态资源都放在了 `/app/dist` 目录下，只需要在 `nginx.conf` 中指定首页以及这个站点的 host 即可。<br />配置如下：
```nginx
worker_processes  1;
events {
  worker_connections  1024;
}
http {
  include       mime.types;
  default_type  application/octet-stream;
  sendfile        on;
  keepalive_timeout  65;
  gzip on;
  gzip_types text/plain application/x-javascript text/css application/xml text/javascript application/javascript image/jpeg image/gif image/png;
  gzip_vary on;
  server {
    listen       80;
    server_name  static.zp.cn;
    location / {
      root /app/dist;
      index index.html;
      #转发任何请求到 index.html
    }
  }
}
```
然后，添加 HOST：<br />127.0.0.1 static.zp.cn<br />此时，在本地浏览器访问 static.zp.cn ，就可以访问静态站点了。
<a name="l2570"></a>
## 搭建文件服务器
有时候，团队需要归档一些数据或资料，那么文件服务器必不可少。使用 Nginx 可以非常快速便捷的搭建一个简易的文件服务。<br />Nginx 中的配置要点：

- 将 autoindex 开启可以显示目录，默认不开启。
- 将 autoindex_exact_size 开启可以显示文件的大小。
- 将 autoindex_localtime 开启可以显示文件的修改时间。
- root 用来设置开放为文件服务的根路径。
- charset 设置为 `charset utf-8,gbk;`，可以避免中文乱码问题（windows 服务器下设置后，依然乱码，暂时没有找到解决方法）。

一个最简化的配置如下：
```nginx
autoindex on;# 显示目录
autoindex_exact_size on;# 显示文件大小
autoindex_localtime on;# 显示文件时间
server {
  charset      utf-8,gbk; # windows 服务器下设置后，依然乱码，暂时无解
  listen       9050 default_server;
  listen       [::]:9050 default_server;
  server_name  _;
  root         /share/fs;
}
```
<a name="rtvEm"></a>
## 跨域解决方案
web 领域开发中，经常采用前后端分离模式。这种模式下，前端和后端分别是独立的 web 应用程序，例如：后端是 Java 程序，前端是 React 或 Vue 应用。<br />各自独立的 web app 在互相访问时，势必存在跨域问题。解决跨域问题一般有两种思路：
<a name="6duwu"></a>
### CORS
在后端服务器设置 HTTP 响应头，把需要运行访问的域名加入加入 `Access-Control-Allow-Origin` 中。
<a name="jsonp"></a>
###  jsonp
把后端根据请求，构造 json 数据，并返回，前端用 jsonp 跨域。<br />需要说明的是，nginx 根据第一种思路，也提供了一种解决跨域的解决方案。<br />举例：www.helloworld.com 网站是由一个前端 app ，一个后端 app 组成的。前端端口号为 9000， 后端端口号为 8080。<br />前端和后端如果使用 http 进行交互时，请求会被拒绝，因为存在跨域问题。来看看，nginx 是怎么解决的吧：<br />首先，在 enable-cors.conf 文件中设置 cors ：
```nginx
# allow origin list
set $ACAO '*';
# set single origin
if ($http_origin ~* (www.helloworld.com)$) {
  set $ACAO $http_origin;
}
if ($cors = "trueget") {
  add_header 'Access-Control-Allow-Origin' "$http_origin";
  add_header 'Access-Control-Allow-Credentials' 'true';
  add_header 'Access-Control-Allow-Methods' 'GET, POST, OPTIONS';
  add_header 'Access-Control-Allow-Headers' 'DNT,X-Mx-ReqToken,Keep-Alive,User-Agent,X-Requested-With,If-Modified-Since,Cache-Control,Content-Type';
}
if ($request_method = 'OPTIONS') {
  set $cors "${cors}options";
}
if ($request_method = 'GET') {
  set $cors "${cors}get";
}
if ($request_method = 'POST') {
  set $cors "${cors}post";
}
```
接下来，在服务器中 `include enable-cors.conf` 来引入跨域配置：
```nginx
# ----------------------------------------------------
# 此文件为项目 nginx 配置片段
# 可以直接在 nginx config 中 include（推荐）
# 或者 copy 到现有 nginx 中，自行配置
# www.helloworld.com 域名需配合 dns hosts 进行配置
# 其中，api 开启了 cors，需配合本目录下另一份配置文件
# ----------------------------------------------------
upstream front_server{
  server www.helloworld.com:9000;
}
upstream api_server{
  server www.helloworld.com:8080;
}
server {
  listen       80;
  server_name  www.helloworld.com;
  location ~ ^/api/ {
    include enable-cors.conf;
    proxy_pass http://api_server;
    rewrite "^/api/(.*)$" /$1 break;
  }
  location ~ ^/ {
    proxy_pass http://front_server;
  }
}
```
<a name="VVPI1"></a>
## 限流
Nginx的限流模块，是基于漏桶算法实现的，在高并发的场景下非常实用。<br />![640.webp](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617946229608-131d1d87-41ed-4bd8-8800-9ab8f4ed1f2a.webp#averageHue=%2333312a&height=360&id=USoig&originHeight=360&originWidth=640&originalType=binary&ratio=1&rotation=0&showTitle=false&size=20406&status=done&style=none&title=&width=640)
<a name="bMpsS"></a>
### 1、配置参数
`limit_req_zone`定义在http块中，`$binary_remote_addr` 表示保存客户端IP地址的二进制形式。<br />Zone定义IP状态及URL访问频率的共享内存区域。zone=keyword标识区域的名字，以及冒号后面跟区域大小。16000个IP地址的状态信息约1MB，所以示例中区域可以存储160000个IP地址。<br />Rate定义最大请求速率。示例中速率不能超过每秒100个请求。
<a name="V4nJj"></a>
### 2、设置限流
`burst`排队大小，`nodelay`不限制单个请求间的时间。
<a name="QQT0p"></a>
## 缓存
<a name="h1wVk"></a>
### 1、浏览器缓存，静态资源缓存用`expire`。
![640.webp](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617946249556-cf1aa5aa-ac29-4f8e-a18e-3b8f6b07499c.webp#averageHue=%232f302c&height=231&id=U8Xvw&originHeight=231&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=11796&status=done&style=none&title=&width=1080)
<a name="Xu8NL"></a>
### 2、代理层缓存
![640.webp](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617946264489-3a72059e-85da-4275-be25-5981f9b3d43d.webp#averageHue=%2332342e&height=268&id=s9i2A&originHeight=268&originWidth=640&originalType=binary&ratio=1&rotation=0&showTitle=false&size=21042&status=done&style=none&title=&width=640)
<a name="YEWcc"></a>
## 黑白名单
<a name="1W5gl"></a>
### 1、不限流白名单
![640.webp](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617946278662-3acebe26-4bcd-4f5a-b0dd-8fd112700aca.webp#averageHue=%2330302b&height=362&id=NTPnm&originHeight=362&originWidth=640&originalType=binary&ratio=1&rotation=0&showTitle=false&size=17168&status=done&style=none&title=&width=640)
<a name="eNfdA"></a>
### 2、黑名单
![640.webp](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617946294090-16020622-ea35-4d4d-8457-6c5ab221f886.webp#averageHue=%2331312c&height=234&id=V09oU&originHeight=234&originWidth=640&originalType=binary&ratio=1&rotation=0&showTitle=false&size=9816&status=done&style=none&title=&width=640)
