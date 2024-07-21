Nginx<br />Nginx 是开源的轻量级 Web 服务器、反向代理服务器，以及负载均衡器和 HTTP 缓存器。其特点是高并发，高性能和低内存。<br />Nginx 专为性能优化而开发，性能是其最重要的考量，实现上非常注重效率，能经受高负载的考验，最大能支持 50000 个并发连接数。<br />Nginx 还支持热部署，它的使用特别容易，几乎可以做到 7x24 小时不间断运行。Nginx 的网站用户有：百度、淘宝、京东、腾讯、新浪、网易等。
<a name="m2OUi"></a>
## 反向代理
<a name="Mls3S"></a>
### 正向代理
Nginx 不仅可以做反向代理，实现负载均衡，还能用做正向代理来进行上网等功能。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624340173395-de89e73d-30fc-4e9a-9a71-173d85ee3bd5.webp#averageHue=%23f6f6f6&clientId=u746bbfa7-2b13-4&from=paste&id=uff4e5fa7&originHeight=291&originWidth=640&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf2ed1a13-6b7c-45e9-8f7a-456638cdd4d&title=)
<a name="Z9G4O"></a>
### 反向代理
客户端对代理服务器是无感知的，客户端不需要做任何配置，用户只请求反向代理服务器，反向代理服务器选择目标服务器，获取数据后再返回给客户端。<br />反向代理服务器和目标服务器对外而言就是一个服务器，只是暴露的是代理服务器地址，而隐藏了真实服务器的 IP 地址。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624340173420-ee8a2a83-f6b8-4107-ac69-306614e45e9b.webp#averageHue=%23f6f6f6&clientId=u746bbfa7-2b13-4&from=paste&id=ud21dc6c1&originHeight=309&originWidth=640&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2cfcb319-5a78-45c6-aaf8-7521f795fde&title=)
<a name="oPUsK"></a>
## 负载均衡
将原先请求集中到单个服务器上的情况改为增加服务器的数量，然后将请求分发到各个服务器上，将负载分发到不同的服务器，即负载均衡。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624340173401-728fe19e-85f8-4bc1-84f6-5569a2d64234.webp#averageHue=%23f6f6f6&clientId=u746bbfa7-2b13-4&from=paste&id=uf505a954&originHeight=312&originWidth=640&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8defaf74-2e7a-4d88-85d0-23761ca1523&title=)
<a name="TwyUB"></a>
## 动静分离
为了加快网站的解析速度，可以把静态页面和动态页面由不同的服务器来解析，加快解析速度，降低原来单个服务器的压力。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624340173585-021d48fd-4496-4879-aecf-e30f1e8d1c64.webp#averageHue=%23f8f7f7&clientId=u746bbfa7-2b13-4&from=paste&id=u3ecdaadf&originHeight=282&originWidth=640&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=u30ff4949-b36b-499d-bed4-2b3f267b4d5&title=)
<a name="W4J6B"></a>
## 高可用
为了提高系统的可用性和容错能力，可以增加 Nginx 服务器的数量，当主服务器发生故障或宕机，备份服务器可以立即充当主服务器进行不间断工作。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624340173594-b85fc938-8763-485c-8685-edfdb7186e61.webp#averageHue=%23f6f5f5&clientId=u746bbfa7-2b13-4&from=paste&id=u4420036d&originHeight=291&originWidth=640&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=u603bafdd-8cc1-43c0-b654-8dd33b1c1bc&title=)
<a name="DKy6p"></a>
## Nginx 配置文件
<a name="mPdjg"></a>
### 文件结构
```nginx
...              #全局块
events {         #events块
   ...
}

http      #http块
{
    ...   #http全局块
    server        #server块
    { 
        ...       #server全局块
        location [PATTERN]   #location块
        {
            ...
        }
        location [PATTERN] 
        {
            ...
        }
    }
    server
    {
      ...
    }
    ...     #http全局块
}
```
Nginx 配置文件由三部分组成：

- 全局块，主要设置一些影响 Nginx 服务器整体运行的配置指令。比如：worker_processes 1；worker_processes 值越大，可以支持的并发处理量就越多。
- Events 块，涉及的指令主要影响 Nginx 服务器与用户的网络连接。比如：worker_connections 1024；支持的最大连接数。
- HTTP 块，又包括 HTTP 全局块和 Server 块，是服务器配置中最频繁的部分，包括配置代理、缓存、日志定义等绝大多数功能。Server 块：配置虚拟主机的相关参数。Location 块：配置请求路由，以及各种页面的处理情况。
<a name="qWjb1"></a>
### 配置文件
```nginx
########### 每个指令必须有分号结束。#################
#user administrator administrators;  #配置用户或者组，默认为nobody nobody。
#worker_processes 2;  #允许生成的进程数，默认为1
#pid /nginx/pid/nginx.pid;   #指定nginx进程运行文件存放地址
error_log log/error.log debug;  #制定日志路径，级别。这个设置可以放入全局块，http块，server块，级别以此为：debug|info|notice|warn|error|crit|alert|emerg
events {
    accept_mutex on;   #设置网路连接序列化，防止惊群现象发生，默认为on
    multi_accept on;  #设置一个进程是否同时接受多个网络连接，默认为off
    #use epoll;      #事件驱动模型，select|poll|kqueue|epoll|resig|/dev/poll|eventport
    worker_connections  1024;    #最大连接数，默认为512
}
http {
    include       mime.types;   #文件扩展名与文件类型映射表
    default_type  application/octet-stream; #默认文件类型，默认为text/plain
    #access_log off; #取消服务日志    
    log_format myFormat '$remote_addr–$remote_user [$time_local] $request $status $body_bytes_sent $http_referer $http_user_agent $http_x_forwarded_for'; #自定义格式
    access_log log/access.log myFormat;  #combined为日志格式的默认值
    sendfile on;   #允许sendfile方式传输文件，默认为off，可以在http块，server块，location块。
    sendfile_max_chunk 100k;  #每个进程每次调用传输数量不能大于设定的值，默认为0，即不设上限。
    keepalive_timeout 65;  #连接超时时间，默认为75s，可以在http，server，location块。

    upstream mysvr {   
      server 127.0.0.1:7878;
      server 192.168.10.121:3333 backup;  #热备
    }
    error_page 404 https://www.baidu.com; #错误页
    server {
        keepalive_requests 120; #单连接请求上限次数。
        listen       4545;   #监听端口
        server_name  127.0.0.1;   #监听地址       
        location  ~*^.+$ {       #请求的url过滤，正则匹配，~为区分大小写，~*为不区分大小写。
           #root path;  #根目录
           #index vv.txt;  #设置默认页
           proxy_pass  http://mysvr;  #请求转向mysvr 定义的服务器列表
           deny 127.0.0.1;  #拒绝的ip
           allow 172.18.5.54; #允许的ip           
        } 
    }
}
```
<a name="JYUxX"></a>
## 配置实例
下面通过配置 Nginx 配置文件，实现正向代理、反向代理、负载均衡、Nginx 缓存、动静分离和高可用 Nginx 6 种功能，并对 Nginx 的原理作进一步的解析。当需要使用 Nginx 配置文件时，参考本文实例即可，建议收藏。
<a name="SdsvY"></a>
### 正向代理
正向代理的代理对象是客户端。正向代理就是代理服务器替客户端去访问目标服务器。<br />实现效果：在浏览器输入 www.google.com , 浏览器跳转到 www.google.com 。<br />具体配置：
```nginx
server{
    resolver 8.8.8.8;
    listen 80;

    location / {
        proxy_pass http://$http_host$request_uri;
    }
}
```
在需要访问外网的客户端上执行以下一种操作即可：
<a name="chyAU"></a>
#### 方法1（推荐）
```bash
export http_proxy=http://你的正向代理服务器地址:代理端口   
```
<a name="T3f91"></a>
#### 方法2
```bash
vim ~/.bashrc
export http_proxy=http://你的正向代理服务器地址:代理端口   
```
<a name="XtNyy"></a>
### 反向代理
反向代理指代理后端服务器响应客户端请求的一个中介服务器，代理的对象是服务端。<br />实现效果：在浏览器输入 www.abc.com , 从 Nginx 服务器跳转到 Linux 系统 Tomcat 主页面。<br />具体配置：
```nginx
server {
  listen       80;   
  server_name  192.168.4.32;   #监听地址

  location  / {       
    root html;  #/html目录
    proxy_pass http://127.0.0.1:8080;  #请求转向
    index  index.html index.htm;      #设置默认页       
  } 
}
```
实现效果：根据在浏览器输入的路径不同，跳转到不同端口的服务中。<br />具体配置：
```nginx
server {
  listen       9000;   
  server_name  192.168.4.32;   #监听地址       

  location  ~ /example1/ {  
    proxy_pass http://127.0.0.1:5000;         
  } 

  location  ~ /example2/ {  
    proxy_pass http://127.0.0.1:8080;         
  } 
}
```
Location 指令说明：

- `~`：表示 uri 包含正则表达式，且区分大小写。
- `~*`：表示 uri 包含正则表达式，且不区分大小写。
- `=`：表示 uri 不含正则表达式，要求严格匹配。
<a name="GMYTP"></a>
### 负载均衡
实现效果：在浏览器地址栏输入 http://192.168.4.32/example/a.html ，平均到 5000 和 8080 端口中，实现负载均衡效果。<br />具体配置：
```nginx
upstream myserver {   
  server 192.167.4.32:5000;
  server 192.168.4.32:8080;
}


server {
  listen       80;   #监听端口
  server_name  192.168.4.32;   #监听地址

  location  / {       
    root html;  #html目录
    index index.html index.htm;  #设置默认页
    proxy_pass  http://myserver;  #请求转向 myserver 定义的服务器列表      
  } 
}
```
Nginx 分配服务器策略：<br />轮询（默认）：按请求的时间顺序依次逐一分配，如果服务器 down 掉，能自动剔除。<br />权重：weight 越高，被分配的客户端越多，默认为 1。<br />比如：
```nginx
upstream myserver {   
  server 192.167.4.32:5000 weight=10;
  server 192.168.4.32:8080 weight=5;
}
```
IP：按请求 IP 的 Hash 值分配，每个访客固定访问一个后端服务器。<br />比如：
```nginx
upstream myserver { 
  ip_hash;  
  server 192.167.4.32:5000;
  server 192.168.4.32:8080;
}
```
Fair：按后端服务器的响应时间来分配，响应时间短的优先分配到请求。<br />比如：
```nginx
upstream myserver { 
  fair;  
  server 192.168.4.32:5000;
  server 192.168.4.32:8080;
}
```
<a name="TA3To"></a>
### Nginx 缓存
实现效果：在 3 天内，通过浏览器地址栏访问 http://192.168.4.32/a.jpg，不会从服务器抓取资源，3 天后（过期）则从服务器重新下载。<br />具体配置：
```nginx
# http 区域下添加缓存区配置
proxy_cache_path /tmp/nginx_proxy_cache levels=1 keys_zone=cache_one:512m inactive=60s max_size=1000m;

# server 区域下添加缓存配置
location ~ \.(gif|jpg|png|htm|html|css|js)(.*) {
     proxy_pass http://192.168.4.32:5000；#如果没有缓存则转向请求
     proxy_redirect off;
     proxy_cache cache_one;
     proxy_cache_valid 200 1h;            #对不同的 HTTP 状态码设置不同的缓存时间
     proxy_cache_valid 500 1d;
     proxy_cache_valid any 1m;
     expires 3d;
}
```
Expires 是给一个资源设定一个过期时间，通过 Expires 参数设置，可以使浏览器缓存过期时间之前的内容，减少与服务器之间的请求和流量。<br />也就是说无需去服务端验证，直接通过浏览器自身确认是否过期即可，所以不会产生额外的流量。此种方法非常适合不经常变动的资源。
<a name="yAfDx"></a>
### 动静分离
实现效果：通过浏览器地址栏访问 www.abc.com/a.html ，访问静态资源服务器的静态资源内容。<br />通过浏览器地址栏访问 www.abc.com/a.jsp ，访问动态资源服务器的动态资源内容。<br />具体配置：
```nginx
upstream static {   
    server 192.167.4.31:80;
}

upstream dynamic {   
    server 192.167.4.32:8080;
}

server {
    listen       80;   #监听端口
    server_name  www.abc.com; 监听地址

    # 拦截动态资源
    location ~ .*\.(php|jsp)$ {
       proxy_pass http://dynamic;
    }

    # 拦截静态资源
    location ~ .*\.(jpg|png|htm|html|css|js)$ {       
       root /data/;  #html目录
       proxy_pass http://static;
       autoindex on;;  #自动打开文件列表
    }  
}
```
<a name="WXlvj"></a>
### 高可用
一般情况下，通过 Nginx 主服务器访问后台目标服务集群，当主服务器挂掉后，自动切换至备份服务器，此时由备份服务器充当主服务器的角色，访问后端目标服务器。<br />实现效果：准备两台 Nginx 服务器，通过浏览器地址栏访问虚拟 IP 地址，把主服务器的 Nginx 停止，再次访问虚拟 IP 地址仍旧有效。<br />具体配置：
```bash
# 安装 keepalived
yum install keepalived -y

# 检查版本
rpm -q -a keepalived
keepalived-1.3.5-16.el7.x86_64
```

1. 在两台 Nginx 服务器上安 Keepalived：Keepalived 相当于一个路由，它通过一个脚本来检测当前服务器是否还活着，如果还活着则继续访问，否则就切换到另一台备份服务器。
2. 修改主备服务器 /etc/keepalived/keepalivec.conf 配置文件（可直接替换），完成高可用主从配置。

Keepalived 将 Nginx 服务器绑定到一个虚拟 IP，Nginx 高可用集群对外统一暴露这个虚拟 IP，客户端都是通过访问这个虚拟 IP 来访问 Nginx 服务器 。
```nginx
global_defs {
    notification_email {
        acassen@firewall.loc
        failover@firewall.loc
        sysadmin@firewall.loc
    }
    notification_email_from_Alexandre.Cassen@firewall.loc
    smtp_server 192.168.4.32
    smtp_connect_timeout 30
    router_id LVS_DEVEL  # 在 /etc/hosts 文件中配置，通过它能访问到我们的主机
}

vrrp_script_chk_http_port {   
    script "/usr/local/src/nginx_check.sh"

    interval 2      # 检测脚本执行的时间间隔

    weight 2        # 权重每次加2
}

vrrp_instance VI_1 {
    interface ens7f0 # 网卡，需根据情况修改
    state MASTER    # 备份服务器上将 MASTER 改为 BACKUP
    virtual_router_id 51 # 主备机的 virtual_router_id 必须相同
    priority 100   # 主备机取不同的优先级，主机值较大，备份机值较小
    advert_int 1  # 每隔多长时间（默认1s）发送一次心跳，检测服务器是否还活着
    authentication {
      auth_type PASS
      auth_pass 1111
    }
    virtual_ipaddress {
        192.168.1.100 # VRRP H 虚拟地址，可以绑定多个
    }
}
```
字段说明如下：<br />`router_id`：在 /etc/hosts 文件中配置，通过它能访问到主机。
```bash
127.0.0.1   localhost localhost.localdomain localhost4 localhost4.localdomain4
::1         localhost localhost.localdomain localhost6 localhost6.localdomain6
127.0.0.1   LVS_DEVEL   
```
`interval`：设置脚本执行的间隔时间。<br />`weight`：当脚本执行失败即 Keepalived 或 Nginx 挂掉时，权重增加的值（可为负数）。<br />`interface`：输入 ifconfig 命令查看当前的网卡名是什么。
```bash
ens7f0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
inet 192.168.4.32  netmask 255.255.252.0  broadcast 192.168.7.255
inet6 fe80::e273:9c3c:e675:7c60  prefixlen 64  scopeid 0x20<link>
... ...
```

3. 在 /usr/local/src 目录下添加检测脚本 nginx_check.sh：
```bash
#!/bin/bash
A=`ps -C nginx -no-header |wc -l`
if [ $A -eq 0 ];then
    /usr/local/nginx/sbin/nginx
    sleep 2
    if [ ps -C nginx -no-header |wc -l` -eq 0 ];then
        killall keepalived
    fi
fi
```

4. 启动两台服务器的 Nginx 和 Keepalived：
```bash
# 启动 nginx
./nginx

# 启动 keepalived
systemctl start keepalived.service
```

5. 查看虚拟 IP 地址 IP a。把主服务器 192.168.4.32 Nginx 和 Keepalived 停止，再访问虚拟 IP 查看高可用效果。
<a name="jY0x1"></a>
## 原理解析
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624340174169-ec9d36db-59a3-493c-905f-f7538d7184f9.webp#averageHue=%23fdfdf9&clientId=u746bbfa7-2b13-4&from=paste&id=uac325659&originHeight=445&originWidth=640&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1207e18d-ba6d-419b-b4df-1ff4b42f4aa&title=)<br />Nginx 启动之后，在 Linux 系统中有两个进程，一个为 Master，一个为 Worker。<br />Master 作为管理员不参与任何工作，只负责给多个 Worker 分配不同的任务（Worker 一般有多个）。
```bash
ps -ef |grep nginx
root     20473     1  0  2019 ?        00:00:00 nginx: master process /usr/sbin/nginx
nginx     4628 20473  0 Jan06 ?        00:00:00 nginx: worker process
nginx     4629 20473  0 Jan06 ?        00:00:00 nginx: worker process
```
<a name="lNZLd"></a>
### Worker 是如何工作的？
客户端发送一个请求首先要经过 Master，管理员收到请求后会将请求通知给 Worker。<br />多个 Worker 以争抢的机制来抢夺任务，得到任务的 Worker 会将请求经由 Tomcat 等做请求转发、反向代理、访问数据库等（Nginx 本身是不直接支持 Java 的）。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624340174138-0cc146a4-6e48-4e50-a528-7a74f342a2ca.webp#averageHue=%23fade83&clientId=u746bbfa7-2b13-4&from=paste&id=u2f284b0f&originHeight=322&originWidth=584&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9f12698a-42a3-4493-bc75-60d07a8b621&title=)
<a name="b2SMn"></a>
### 一个 Master 和多个 Worker 的好处？

- 可以使用 `nginx -s reload` 进行热部署。
- 每个 Worker 是独立的进程，如果其中一个 Worker 出现问题，其他 Worker 是独立运行的，会继续争抢任务，实现客户端的请求过程，而不会造成服务中断。
<a name="OB5wU"></a>
### 设置多少个 Worker 合适？
Nginx 和 Redis 类似，都采用了 IO 多路复用机制，每个 Worker 都是一个独立的进程，每个进程里只有一个主线程。<br />通过异步非阻塞的方式来处理请求，每个 Worker 的线程可以把一个 CPU 的性能发挥到极致，因此，Worker 数和服务器的 CPU 数相等是最为适宜的。
<a name="o63VC"></a>
### 思考：

- 发送一个请求，会占用 Worker 几个连接数？
- 有一个 Master 和 4 个 Worker，每个 Worker 支持的最大连接数为 1024，该系统支持的最大并发数是多少？
