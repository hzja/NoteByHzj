Nginx 
<a name="KB0go"></a>
## Nginx可优化点梳理
<a name="Mr5N7"></a>
### Linux配置
现代Linux内核（2.6+）能够很好的调节各种配置，有些配置可能想更改。如果操作系统配置太低，那么会在内核日志中看到错误信息，因此需要调节这些配置。
<a name="XIR0A"></a>
#### Backlog队列
以下设置与连接及其如何排队直接相关。如果传入的连接率很高而性能水平参差不齐，比如一些连接似乎被暂停了，那么更改这些配置可能会有用。
> - `net.core.somaxconn` 该项设置等待被Nginx接受的连接的排队大小。由于Nginx接受连接速度非常快，这个值通常不需要非常大，但是默认值是非常低的，所以如果有一个高流量网站，增加这个值是个好主意。如果设置过低，那么能在内核日志中看到错误信息，这时应该增加这个值直到没有错误信息。注意：如果将其设置为大于512的值，应该同时用listen指令的backlog参数匹配这个值来更改Nginx的配置。
> - net.core.netdev_max_backlog 该项设置在交由CPU处理之前网卡缓冲数据包的速率。对于拥有高带宽的机器，这个值可能需要增加。查看网卡文档寻求相关建议，或者检查内核日志相关错误信息。<br />

<a name="PSDew"></a>
#### 文件描述符
文件描述符是一种操作系统资源，用来处理诸如连接和打开文件的事情。对每一个连接，Nginx可以用上多达两个文件描述符。例如，如果Nginx用作代理，则其中一个用于客户端连接，另一个用于连接到被代理的服务器。如果使用了HTTP keepalive，则连接描述符的使用会少得多。对于有大量连接的系统，如下设置可能需要进行调整：
> - sys.fs.file_max 这是系统范围内的文件描述符限制。<br />
> - nofile 这是用户级别的文件描述符限制，在/etc/security/limits.conf文件中配置<br />

<a name="sb2dP"></a>
#### 临时端口
当Nginx被当作代理使用时，每一个到upstream服务器的连接都使用一个临时端口。
> - net.ipv4.ip_local_port_range 这个用来指定可以使用的起止端口号。如果可以看到端口耗尽，可以增加这个范围。常见的设置为1024到65000。<br />
> - net.ipv4.tcp_fin_timeout 这个用于指定一个不再被使用的端口多久之后可以被另一连接再次使用。通常，这个值默认为60秒，但是可以安全地减少到30甚至15秒。<br />

<a name="FLqv2"></a>
### Nginx配置
下面是一些可能影响性能的Nginx指令。仅讨论那些推荐大多数用户调整的指令。这里未提及到的任何指令，如果没有Nginx团队的指导，不推荐更改。
<a name="UXjoI"></a>
#### 工作进程
Nginx可以运行多个工作进程，每个都能处理大量连接。可以用如下指令控制工作进程个数以及连接如何被处理：
> - `worker_processes` 这个控制Nginx运行的工作进程个数。大多数情况下，一个CPU核心跑一个工作进程能够工作得很好。可以将该指令设为auto来达到与CPU核心数匹配的工作进程数。有时候，可以增加这个值，比如工作进程需要处理大量磁盘IO操作的时候。这个值默认为1。<br />
> - `worker_connections` 这个表示每个工作进程同时能够处理的最大连接数。默认值是512，但是大多数系统能处理更大的值。这个值该设为多少取决于服务器硬件配置以及流量的特性，可以通过测试来发现。<br />

<a name="o8x6W"></a>
#### Keepalives
持久连接可以减少打开和关闭连接所需要的CPU和网络开销，因而对性能有重大影响。Nginx终止所有客户端连接，并具有到upstream服务器的单独连接。Nginx支持客户端和upstream服务器的持久连接。如下指令涉及客户端持久连接：
> `keepalive_requests` 这表示客户端能在单个持久连接上发送多少请求。默认值是100，可以设置成更高的值，这在负载生成器从单个客户端发送大量请求的测试场景中非常有用。<br />
> `keepalive_timeout` 表示一个空闲持久连接保持打开状态多长时间。<br />

如下指令涉及upstream持久连接：
> `keepalive` 这个指定每个工作进程连接到upstream服务器的空闲持久连接数量。这个指令没有默认值。<br />

为了启用到upstream的持久连接，需要增加如下指令：
> proxy_http_version 1.1;<br />
> proxy_set_header Connection "";<br />

<a name="f0PTS"></a>
#### Access日志
记录每个请求需要花费CPU和IO周期，减少这种影响的一种方法是启用access日志缓冲。这将导致Nginx缓冲一系列日志条目，然后一次性写入文件而不是单个单个写入。<br />通过指定access_log指令的"buffer=size"选项可以打开access日志缓冲，该设置指定要使用的缓冲区的大小。还可以使用"flush=time"选项告诉Nginx多长时间后把缓冲区中的条目写入文件。<br />定义了这两个选项后，当缓冲区放不下下一条日志，或者缓冲区中的条目超过了flush参数指定的时间，Nginx会将缓冲区中的条目写入日志文件。当工作进程重新打开日志文件或者关闭时，缓冲区中的条目也会被写入文件。也可以完全禁用access日志记录。
<a name="R9mn0"></a>
#### Sendfile
Sendfile是一个操作系统特性，可以在Nginx上启用。它通过在内核中从一个文件描述符向另一个文件描述符复制数据，往往能达到零拷贝，因而可以提供更快的TCP数据传输。Nginx可以使用该机制将缓存或者磁盘上的内容写到socket，无需从内核空间到用户空间的上下文切换，因而非常快并且使用较少的CPU开销。由于数据永远不会触及用户空间，所以不可能把需要访问数据的过滤器插入到处理链中，不能使用任何需要改变内容的Nginx过滤器，比如gzip过滤器。Nginx默认没有启用该机制。
<a name="fNH2o"></a>
#### 限制
Nginx和Nginx Plus允许设置各种限制，用来控制客户端资源消耗，以防影响系统性能以及用户体验和安全。以下是一些相关指令：
> - limit_conn / limit_conn_zone 这些指令可以用来限制Nginx允许的连接数，比如来自单个客户端IP地址的连接数。这可以防止单个客户端打开太多连接而消耗太多资源。<br />
> - limit_rate 这个用来限制客户端在单个连接上允许使用的带宽。这可以防止某些客户端导致系统超载，因而有利于为所有客户端提供QoS保证。<br />
> - limit_req / limit_req_zone 这些指令可以用来限制Nginx的请求处理速率。与limit_rate一起，可以防止某些客户端导致系统超载，因而有利于为所有客户端提供QoS保证。这些指令也可以用来增强安全性，尤其是对登录页面，通过限制请求速率，使得其对人类用户是合适的，而会减慢试图访问的应用的程序。<br />
> - max_conns 这个用来限制同时连接到upstream组中单个服务器的最大连接数。这可以防止upstream服务器超载。默认值是0，表示没有限制。<br />
> - queue 如果设置了max_conns，那么queue指令用来决定当一个请求由于upstream组中没有可用服务器或者这些服务器达到max_conns限制而不能得到处理时会发生什么。这个指令用来设定有多少请求将会排队以及排多久。如果没有设置这个指令，就不会有排队行为。

<a name="0bfd9492"></a>
### 其它考虑
Nginx还有一些特性可以用来提高web应用的性能。这些特性不常出现在调优讨论中，但是有必要一提，因为它们的影响也可能比较可观。
<a name="e80c310e"></a>
#### 缓存
对于一个为一组web服务器或者应用服务器作负载均衡的Nginx实例来说，启用缓存可以显著地降低响应时间，同时能显著减轻后端服务器的负载。
<a name="6612548a"></a>
#### 压缩
压缩响应可以大大减小响应的大小，减少带宽占用。不过，这需要CPU资源来处理压缩，所以最好在值得减少带宽占用的情况下使用。需要注意的是，不能对已经压缩的东西（比如jpeg图片）再次启用压缩。
<a name="8AFgo"></a>
## 基本参数优化
```nginx
server_tokens off;
#关闭在错误页面中的nginx版本号，安全性是有好处的
sendfile on;
#传输文件时发挥作用
tcp_nopush on;
#一个数据包里发送所有头文件
tcp_nodelay on;
#不缓存数据
keepalive_timeout 10; 
#在这个超时时间过后关闭客户端链接
client_header_timeout 10; 
#设置请求头的超时时间
client_body_timeout 10;
#设置请求体的超时时间
reset_timeout_connection on;
#开启关闭不响应的客户端连接功能，释放客户端所占的内存空间
send_timeout 10;
#客户端的响应超时时间。如果在这段时间内，客户端没有读取任何数据，nginx就会关闭连接。
#limit_conn_zone 设置用于保存各种key（比如当前连接数）的共享内存的参数。5m就是5兆字节，这个值应该被设置的足够大以存储（32K5）32byte状态或者（16K5）64byte状态。
limit_conn
#为给定的key设置最大连接数。这里key是addr，设置的值是100，也就是说允许每一个IP地址最多同时打开有100个连接。
default_type
#设置文件使用的默认的MIME-type。
charset
#设置头文件中的默认的字符集
```
<a name="zOP7Z"></a>
### Gzip压缩优化
```nginx
gzip_types  
#压缩的文件类型
 text/plain text/css 
 application/json 
 application/x-javascript 
 text/xml application/xml 
 application/xml+rss 
 text/javascript
gzip on;
#采用gzip压缩的形式发送数据
gzip_disable "msie6"
#为指定的客户端禁用gzip功能
gzip_static;
#压缩前查找是否有预先gzip处理过的资源
gzip_proxied any;
#允许或者禁止压缩基于请求和响应的响应流
gzip_min_length  1000;
#设置对数据启用压缩的最少字节数
gzip_comp_level 6;
#设置数据的压缩等级
```
<a name="SdNZr"></a>
### FastCGI参数优化
```nginx
fastcgi_cache_path 
/data/ngx_fcgi_cache #缓存路径
levels=2:2 #目录结构等级
keys_zone=ngx_fcgi_cache:512m 
	   #关键字区域存储时间
inactive=1d #非活动删除时间	 
fastcgi_connect_timeout 240; 
#连接到后端fastcgi的超时时间
fastcgi_send_timeout 240; 
#建立连接后多久不传送数据就断开
fastcgi_read_timeout 240; 
#接收fastcgi应答的超时时间
fastcgi_buffer_size 64k; 
#指定读取fastcgi应答缓冲区大小
fastcgi_buffers 4 64k;
#指定本地缓冲区大小（缓冲FaseCGI应答请求）
fastcgi_busy_buffers_size 128k; 
#繁忙时的buffer，可以是fastcgi_buffer的两倍
fastcgi_temp_file_write_size  128k; 
#在写入缓存文件时用多大的数据块，默认是fastcgi_buffer的两倍
fastcgi_cache mingongge;
#开启缓存时指定一个名称
fastcgi_cache_valid 200 302 1h;
#指定应答码200 302 缓存一小时
fastcgi_cache_valid 301 1d; 
#指定应答码301缓存一天
fastcgi_cache_valid any 1m;
#指定其它应答码缓存一月
```
<a name="y7g92"></a>
### 其它参数优化
```nginx
open_file_cache
#指定缓存最大数目以及缓存的时间
open_file_cache_valid
#在open_file_cache中指定检测正确信息的间隔时间
open_file_cache_min_uses   
#定义了open_file_cache中指令参数不活动时间期间里最小的文件数
open_file_cache_errors     
#指定了当搜索一个文件时是否缓存错误信息
location ~ .*\.(gif|jpg|jpeg|png|bmp|swf)$
#指定缓存文件的类型
{
  expires 3650d;    
  #指定缓存时间
}
location ~ .*\.(js|css)?$
{
  expires 3d;                     
}
```
**expires有个缺点就是如果更新WEB数据后，用户没有清理缓存，会看到旧的数据，因此建议将时间设置短一点**
<a name="68NOy"></a>
### 优化后完整的配置文件
```nginx
user www; 
pid /var/run/nginx.pid; 
worker_processes auto; 
worker_rlimit_nofile 100000; 
events { 
  worker_connections 2048; 
  multi_accept on; 
  use epoll; 
} 
http { 
  server_tokens off; 
  sendfile on; 
  tcp_nopush on; 
  tcp_nodelay on; 
  access_log off; 
  error_log /var/log/nginx/error.log crit; 
  keepalive_timeout 10; 
  client_header_timeout 10; 
  client_body_timeout 10; 
  reset_timedout_connection on; 
  send_timeout 10; 
  limit_conn_zone $binary_remote_addr zone=addr:5m; 
  limit_conn addr 100; 
  include /etc/nginx/mime.types; 
  default_type text/html; 
  charset UTF-8; 
  gzip on; 
  gzip_disable "msie6"; 
  gzip_proxied any; 
  gzip_min_length 1000; 
  gzip_comp_level 6; 
  gzip_types text/plain text/css application/json application/x-javascript text/xml application/xml application/xml+rss text/javascript; 
  open_file_cache max=100000 inactive=20s; 
  open_file_cache_valid 30s; 
  open_file_cache_min_uses 2; 
  open_file_cache_errors on; 
  include /etc/nginx/conf.d/*.conf; 
    include /etc/nginx/sites-enabled/*; 
}
```
<a name="9WrzL"></a>
## 一、自定义返回给客户端的404错误页面
1）优化前，客户端使用浏览器访问不存在的页面，会提示404文件未找到
```bash
[root@client ~]# firefox http://192.168.4.5/xxxxx        //访问一个不存在的页面
```
2）修改Nginx配置文件，自定义报错页面
```bash
[root@proxy ~]# vim /usr/local/nginx/conf/nginx.conf
.. ..
    charset utf-8;               //仅在需要中文时修改该选项
error_page   404  /404.html;    //自定义错误页面
.. ..
[root@proxy ~]# vim /usr/local/nginx/html/404.html      //生成错误页面
Oops,No NO no page …
[root@proxy ~]# nginx -s reload
#请先确保nginx是启动状态，否则运行该命令会报错,报错信息如下：
#[error] open() "/usr/local/nginx/logs/nginx.pid" failed (2: No such file or directory)
```
3）优化后，客户端使用浏览器访问不存在的页面，会提示自己定义的40x.html页面
```bash
[root@client ~]# firefox http://192.168.4.5/xxxxx    //访问一个不存在的页面
```
常见的http状态码可用参考表所示

| 状态码 | 功能描述 |
| --- | --- |
| 200 | 一切正常 |
| 301 | 永久重定向 |
| 302 | 临时重定向 |
| 401 | 用户名或密码错误 |
| 403 | 禁止访问（客户端IP地址被拒绝） |
| 404 | 文件不存在 |
| 414 | 请求URL头部过长 |
| 500 | 服务器内部错误 |

<a name="rn93B"></a>
## 二、查看服务器状态信息（非常重要的功能）
1）编译安装时使用--with-http_stub_status_module开启状态页面模块
```bash
[root@proxy ~]# tar  -zxvf   nginx-1.12.2.tar.gz
[root@proxy ~]# cd  nginx-1.12.2
[root@proxy nginx-1.12.2]# ./configure   \
> --with-http_ssl_module                        //开启SSL加密功能
> --with-stream                                //开启TCP/UDP代理模块
> --with-http_stub_status_module                //开启status状态页面
[root@proxy nginx-1.12.2]# make && make install    //编译并安装
```
2）启用Nginx服务并查看监听端口状态<br />ss命令可以查看系统中启动的端口信息，该命令常用选项如下：

- -a显示所有端口的信息
- -n以数字格式显示端口号
- -t显示TCP连接的端口
- -u显示UDP连接的端口
- -l显示服务正在监听的端口信息，如httpd启动后，会一直监听80端口
- -p显示监听端口的服务名称是什么（也就是程序名称）
:::tips
注意：在RHEL7系统中可以使用ss命令替代netstat命令，功能一样，选项一样。
:::
```bash
[root@proxy ~]# /usr/local/nginx/sbin/nginx
[root@proxy ~]# netstat  -anptu  |  grep nginx
tcp        0        0 0.0.0.0:80        0.0.0.0:*        LISTEN        10441/nginx
[root@proxy ~]# ss  -anptu  |  grep nginx
```
3）修改Nginx配置文件，定义状态页面
```bash
[root@proxy ~]# cat /usr/local/nginx/conf/nginx.conf
… …
location /status {
                stub_status on;
                 #allow IP地址;
                 #deny IP地址;
        }
… …
[root@proxy ~]# /usr/local/nginx/sbin/nginx -s reload
```
4）优化后，查看状态页面信息
```
[root@proxy ~]# curl  http://192.168.4.5/status
Active connections: 1 
server accepts handled requests
 10 10 3 
Reading: 0 Writing: 1 Waiting: 0
```

- Active connections：当前活动的连接数量。
- Accepts：已经接受客户端的连接总数量。
- Handled：已经处理客户端的连接总数量。（一般与accepts一致，除非服务器限制了连接数量）。
- Requests：客户端发送的请求数量。
- Reading：当前服务器正在读取客户端请求头的数量。
- Writing：当前服务器正在写响应信息的数量。
- Waiting：当前多少客户端在等待服务器的响应。
<a name="nP0YS"></a>
## 三、优化Nginx并发量
1）优化前使用ab高并发测试
```
[root@proxy ~]# ab -n 2000 -c 2000 http://192.168.4.5/
Benchmarking 192.168.4.5 (be patient)
socket: Too many open files (24)                //提示打开文件数量过多
```
2）修改Nginx配置文件，增加并发量
```
[root@proxy ~]# vim /usr/local/nginx/conf/nginx.conf
.. ..
worker_processes  2;                    //与CPU核心数量一致
events {
worker_connections 65535;        //每个worker最大并发连接数
}
.. ..
[root@proxy ~]# /usr/local/nginx/sbin/nginx -s reload
```
3）优化Linux内核参数（最大文件数量）
```
[root@proxy ~]# ulimit -a                        //查看所有属性值
[root@proxy ~]# ulimit -Hn 100000                //设置硬限制（临时规则）
[root@proxy ~]# ulimit -Sn 100000                //设置软限制（临时规则）
[root@proxy ~]# vim /etc/security/limits.conf
    .. ..
*               soft    nofile            100000
*               hard    nofile            100000
#该配置文件分4列，分别如下：
#用户或组    硬限制或软限制    需要限制的项目   限制的值
```
4）优化后测试服务器并发量（因为客户端没调内核参数，所以在proxy测试）
```
[root@proxy ~]# ab -n 2000 -c 2000 http://192.168.4.5/
```
<a name="ksod0"></a>
## 四、优化Nginx数据包头缓存
1）优化前，使用脚本测试长头部请求是否能获得响应
```
[root@proxy ~]# cat lnmp_soft/buffer.sh 
#!/bin/bash
URL=http://192.168.4.5/index.html?
for i in {1..5000}
do
    URL=${URL}v$i=$i
done
curl $URL                                //经过5000次循环后，生成一个长的URL地址栏
[root@proxy ~]# ./buffer.sh
.. ..
<center><h1>414 Request-URI Too Large</h1></center>        //提示头部信息过大
```
2）修改Nginx配置文件，增加数据包头部缓存大小
```
[root@proxy ~]# vim /usr/local/nginx/conf/nginx.conf
.. ..
http {
client_header_buffer_size    1k;        //默认请求包头信息的缓存    
large_client_header_buffers  4 4k;        //大请求包头部信息的缓存个数与容量
.. ..
}
[root@proxy ~]# /usr/local/nginx/sbin/nginx -s reload
```
3）优化后，使用脚本测试长头部请求是否能获得响应
```
[root@proxy ~]# cat buffer.sh 
#!/bin/bash
URL=http://192.168.4.5/index.html?
for i in {1..5000}
do
    URL=${URL}v$i=$i
done
curl $URL
[root@proxy ~]# ./buffer.sh
```
<a name="BH4ri"></a>
## 五、浏览器本地缓存静态数据
1）使用Firefox浏览器查看缓存<br />以Firefox浏览器为例，在Firefox地址栏内输入about:cache将显示Firefox浏览器的缓存信息，点击List Cache Entries可以查看详细信息。<br />2）清空firefox本地缓存数据。<br />3）改Nginx配置文件，定义对静态页面的缓存时间
```
[root@proxy ~]# vim /usr/local/nginx/conf/nginx.conf
server {
        listen       80;
        server_name  localhost;
        location / {
            root   html;
            index  index.html index.htm;
        }
location ~* \.(jpg|jpeg|gif|png|css|js|ico|xml)$ {
expires        30d;            //定义客户端缓存时间为30天
}
}
[root@proxy ~]# cp /usr/share/backgrounds/day.jpg /usr/local/nginx/html
[root@proxy ~]# /usr/local/nginx/sbin/nginx -s reload
#请先确保nginx是启动状态，否则运行该命令会报错,报错信息如下：
#[error] open() "/usr/local/nginx/logs/nginx.pid" failed (2: No such file or directory)
```
4）优化后，使用Firefox浏览器访问图片，再次查看缓存信息
```
[root@client ~]# firefox http://192.168.4.5/day.jpg
```
在firefox地址栏内输入about:cache，查看本地缓存数据，查看是否有图片以及过期时间是否正确。
