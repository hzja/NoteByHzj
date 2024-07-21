Nginx
<a name="fbx0j"></a>
## Nginx安装
Nginx安装完成，没有sbin目录<br />cd进入到nginx-1.18.0目录内执行
```bash
[root@centos7 nginx-1.18.0]# ./configure --prefix=/usr/local/nginx
[root@centos7 nginx-1.18.0]# make
[root@centos7 nginx-1.18.0]# make install
```
ps：`–prefix=path` 定义一个目录，存放服务器上的文件 ，也就是Nginx的安装目录，默认使用 /usr/local/nginx<br />这步会看到local目录下有一个跟安装的Nginx1.12.2同级的Nginx目录，里面就有sbin目录了。
<a name="lkcQZ"></a>
## 基础指令
<a name="b83yP"></a>
### listen：该指令用于配置网络监听
```nginx
listen *:80 | *:8080      #监听所有80端口和8080端口
listen  IP_address:port   #监听指定的地址和端口号
listen  IP_address        #监听指定ip地址所有端口
listen port               #监听该端口的所有IP连接
```
server_name：该指令用于虚拟主机的配置。
<a name="nWJAc"></a>
#### 基于名称的虚拟主机配置
```nginx
server_name   name ...;
```
<a name="L5if5"></a>
#### 基于 IP 地址的虚拟主机配置
```nginx
server_name 192.168.1.1
```
<a name="bx5nN"></a>
### `location`：该指令用于匹配 URL
`location`指令的作用是根据用户请求的URI来执行不同的应用，也就是根据用户请求的网站URL进行匹配，匹配成功即进行相关的操作。
<a name="NNsUp"></a>
#### location的语法

- `=`开头表示精确匹配
- 如 A 中只匹配根目录结尾的请求，后面不能带任何字符串。
- `^~` 开头表示uri以某个常规字符串开头，不是正则匹配
- `~` 开头表示区分大小写的正则匹配;
- `~*` 开头表示不区分大小写的正则匹配
- `/` 通用匹配，如果没有其它匹配，任何请求都会匹配到
<a name="f6JHb"></a>
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
<a name="fjFio"></a>
### proxy_pass：该指令用于设置被代理服务器的地址
可以是主机名称、IP地址加端口号的形式。<br />语法结构如下：
```nginx
proxy_pass URL;
```
<a name="BUFLi"></a>
### index：该指令用于设置网站的默认首页
<a name="tXpsf"></a>
### 域名重定向
```nginx
server
{
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
<a name="An31r"></a>
### 反向代理
使用 Nginx 反向代理 www.123.com 直接跳转到127.0.0.1:8080
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
<a name="Iq6zU"></a>
### 限流配置
漏桶算法与令牌桶算法区别：主要区别在于“漏桶算法”能够强行限制数据的传输速率，<br />而“令牌桶算法”在能够限制数据的平均传输速率外，还允许某种程度的突发传输。在“令牌桶算法”中，只要令牌桶中存在令牌，那么就允许突发地传输数据直到达到用户配置的门限，因此它适合于具有突发特性的流量。<br />Nginx按请求速率限速模块使用的是漏桶算法，即能够强行保证请求的实时处理速度不会超过设置的阈值。

- limit_req_zone 用来限制单位时间内的请求数，即速率限制，采用的漏桶算法 "leaky bucket"。
- limit_req_conn 用来限制同一时间连接数，即并发限制。

limit_req_zone 参数配置<br />Syntax:    limit_req zone=name [burst=number] [nodelay];<br />Default:    —<br />Context:    http, server, location
```nginx
limit_req_zone $binary_remote_addr zone=one:10m rate=1r/s;
```

- 第一个参数：$binary_remote_addr表示通过remote_addr这个标识来做限制，“binary_”的目的是缩写内存占用量，是限制同一客户端ip地址。
- 第二个参数：zone=one:10m表示生成一个大小为10M，名字为one的内存区域，用来存储访问的频次信息。
- 第三个参数：rate=1r/s表示允许相同标识的客户端的访问频次，这里限制的是每秒1次，还可以有比如30r/m的。
```nginx
limit_req zone=one burst=5 nodelay;
```

- 第一个参数：zone=one 设置使用哪个配置区域来做限制，与上面limit_req_zone 里的name对应。
- 第二个参数：burst=5，重点说明一下这个配置，burst爆发的意思，这个配置的意思是设置一个大小为5的缓冲区当有大量请求（爆发）过来时，超过了访问频次限制的请求可以先放到这个缓冲区内。
- 第三个参数：nodelay，如果设置，超过访问频次而且缓冲区也满了的时候就会直接返回503，如果没有设置，则所有请求会等待排队。
<a name="fqIHq"></a>
#### ngx_http_limit_conn_module 参数配置
这个模块用来限制单个IP的请求数。并非所有的连接都被计数。只有在服务器处理了请求并且已经读取了整个请求头时，连接才被计数。<br />Syntax:    limit_conn zone number;<br />Default:    —<br />Context:    http, server, location
```nginx
limit_conn_zone $binary_remote_addr zone=addr:10m;
server {
  location /download/ {
    limit_conn addr 1;
  }
}
```
一次只允许每个IP地址一个连接。
<a name="pL2KW"></a>
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
keepalive 长连接提高吞吐量<br />keepalived ：设置长连接处理的数量<br />proxy_http_version ：设置长连接http版本为1.1<br />proxy_set_header ：清除connection header 信息
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
<a name="buXnr"></a>
#### 工作方式
<a name="RFXTD"></a>
##### 轮询方式是Nginx负载默认的方式
权重方式 指定每个服务的权重比例，weight和访问比率成正比
```nginx
upstream  dalaoyang-server {
  server    localhost:10001 weight=1;
  server    localhost:10002 weight=2;
}
```
<a name="EKF4w"></a>
##### iphash
每个请求都根据访问ip的hash结果分配，经过这样的处理，每个访客固定访问一个后端服务，如下配置（ip_hash可以和weight配合使用）。
```nginx
upstream  dalaoyang-server {
  ip_hash; 
  server    localhost:10001 weight=1;
  server    localhost:10002 weight=2;
}
```
<a name="bBhsD"></a>
##### 最少连接
将请求分配到连接数最少的服务上。
```nginx
upstream  dalaoyang-server {
  least_conn;
  server    localhost:10001 weight=1;
  server    localhost:10002 weight=2;
}
```
<a name="NlqyB"></a>
##### fair
按后端服务器的响应时间来分配请求，响应时间短的优先分配。
```nginx
upstream  dalaoyang-server {
  server    localhost:10001 weight=1;
  server    localhost:10002 weight=2;
  fair;  
}
```
Consul+upsync+Nginx  实现无需raload动态负载均衡 。<br />传统的负载均衡，如果Upstream参数发生变化，每次都需要重新加载nginx.conf文件，<br />因此扩展性不是很高，所以可以采用动态负载均衡，实现Upstream可配置化、动态化，无需人工重新加载nginx.conf。<br />LVS+Keepalived+Nginx+Tomcat搭建高可用双机主从热备集群。<br />需要明确的是：

- Nginx两台是主备关系，只有一台在工作。后面的tomcat是集群，同时工作的。
- keepalived是同时安装在两台Nginx上的，不过文件配置不一样
- 这里的双机热备是指LVS，Nginx则是集群
<a name="pckFt"></a>
### keepalived
健康检查和失败切换是keepalived的两大核心功能。所谓的健康检查，就是采用tcp三次握手，icmp请求，http请求，udp echo请求等方式对负载均衡器后面的实际的服务器(通常是承载真实业务的服务器)进行保活；而失败切换主要是应用于配置了主备模式的负载均衡器，利用VRRP维持主备负载均衡器的心跳，当主负载均衡器出现问题时，由备负载均衡器承载对应的业务，从而在最大限度上减少流量损失，并提供服务的稳定性。<br />LVS是Linux Virtual Server的简写，意即Linux虚拟服务器，是一个虚拟的服务器集群系统。lvs目前是集成在Linux内的。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1656335041834-c350d2c7-79a7-49bf-a330-a53a87ff7c10.png#clientId=ud8b56db4-ae5d-4&from=paste&id=ud52a3316&originHeight=709&originWidth=808&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub2c58d01-ae5d-4f9b-86e3-8d775bf42bd&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1656335041772-f84d1171-7692-46e6-a9a1-5f80209c1cbe.png#clientId=ud8b56db4-ae5d-4&from=paste&id=ubf4c653e&originHeight=566&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u25318cdc-e5fd-4556-ba0f-846b4f90651&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1656335041844-843490a3-45cf-4b60-86e7-2057ff08cdf2.png#clientId=ud8b56db4-ae5d-4&from=paste&id=u2370357d&originHeight=632&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u58b771b5-e5a3-49e7-a3fd-d054674f7d2&title=)
<a name="eyV8M"></a>
#### 为什么要LVS+Nginx？
<a name="nyBps"></a>
##### 一、Nginx（应用层 网络七层负载均衡）
1、异步转发，请求数据和相应数据都要经过ngix，ngix和客户端建立连接<br />2、轮询所有的tomcat服务器，保证请求成功或者最后一台tomcat服务器也请求失败
<a name="DbfoP"></a>
##### 二、lvs(网络层  网络四层负载均衡)
1、同步转发接受请求数据，lvs转发到服务器，服务器直接和客户端建立连接<br />Nginx要承受所有的流量，当一台nigx承受不了，就需要搭建ngix集群。ngix+ngix 外层的ngix还是要承受所有流量。<br />lvs+ngix:lvs同步转发不会接受相应数据，LVS采用DR模式时不用响应服务器返回的内容。（通常请求数据是比较小的，响应数据会比较大）<br />![2022-06-27-21-04-53-777981.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656335767096-cbf4912c-3d26-439d-8aea-4f547e820a79.png#clientId=u0139d608-faa2-4&from=ui&id=ue6a4c0c4&originHeight=670&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2175064&status=done&style=shadow&taskId=u8af3a98a-b883-486a-825d-8ed58afbeca&title=)<br />静态资源配置
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
`sendfile on | off`，文件读取配置<br />默认sendfile是关闭的，可以配置在http,server,location,if in location中<br />`tcp_nopush on | off`，多个包整合<br />默认是关闭状态，可以在http,server,location中配置，它的作用是在sendfile开启的情况下，提高网络包的传输效率。什么意思呢，假设服务端收到请求，需要推送10个包，为了提高传输效率，这10个包不会一个一个返回给客户端，而是将10个包攒够了后一起返回回去。<br />`tcp_nodelay on | off`，网络包的实时性传输<br />默认开启，可以在http,server,location中配置，它的作用是在keepalive链接下，提高网络包的传输实时性。<br />`gzip on | off`，压缩<br />默认是关闭状态，可以在http,server,location,if in location中配置，作用是压缩传输。一般来说浏览器是可以对压缩后的内容进行解压的。<br />`gzip_comp_level level;`压缩级别<br />默认的压缩级别是1，可以在http,server,location中配置，级别配置的越高，压缩的越好，但是压缩会耗费服务端的计算资源，所以要控制好压缩级别<br />`gzip_http_version 1.0 | 1.1`，压缩对http协议的支持<br />默认对HTTP/1.1协议的请求才会进行gzip压缩，可以配置在http,server,location中配置。当用户想要读取一个1.html文件，首先会在目录中找寻1.html.gz是否存在，所以这就导致了磁盘资源的浪费，必须要存储两份文件。
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
跨域配置<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1656335041841-9ca6e26c-7177-41e5-8169-c5abc9a43051.png#clientId=ud8b56db4-ae5d-4&from=paste&id=u80770a35&originHeight=425&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4d9f3420-e3e4-4e0a-ac97-c4a687352c4&title=)<br />跨域就是在原站点访问域名不同的其他站点，同源策略会阻止一个域的javascript脚本和另外一个域的内容进行交互。所谓同源（即指在同一个域）就是两个页面具有相同的协议（protocol），主机（host）和端口号（port）。<br />CORS 是跨域资源分享（Cross-Origin Resource Sharing）的缩写。它是 W3C 标准，属于跨源 AJAX 请求的根本解决方法。<br />1、普通跨域请求：只需服务器端设置Access-Control-Allow-Origin<br />2、带cookie跨域请求：前后端都需要进行设置
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
防盗链
```nginx
#对源站点验证
valid_referers *.imooc.com; 
#非法引入会进入下方判断 
if ($invalid_referer) { return 404; }
```
