Nginx 配置文件
<a name="fNID4"></a>
### 配置文件1
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
<a name="GHzn3"></a>
### 配置文件2
```nginx
#运行用户
user nobody;
#启动进程,通常设置成和cpu的数量相等
worker_processes  1;

#全局错误日志及PID文件
#error_log  logs/error.log;
#error_log  logs/error.log  notice;
#error_log  logs/error.log  info;

#pid        logs/nginx.pid;

#工作模式及连接数上限
events {
    #epoll是多路复用IO(I/O Multiplexing)中的一种方式,
    #仅用于linux2.6以上内核,可以大大提高nginx的性能
    use   epoll; 

    #单个后台worker process进程的最大并发链接数    
    worker_connections  1024;

    # 并发总数是 worker_processes 和 worker_connections 的乘积
    # 即 max_clients = worker_processes * worker_connections
    # 在设置了反向代理的情况下，max_clients = worker_processes * worker_connections / 4  为什么
    # 为什么上面反向代理要除以4，应该说是一个经验值
    # 根据以上条件，正常情况下的Nginx Server可以应付的最大连接数为：4 * 8000 = 32000
    # worker_connections 值的设置跟物理内存大小有关
    # 因为并发受IO约束，max_clients的值须小于系统可以打开的最大文件数
    # 而系统可以打开的最大文件数和内存大小成正比，一般1GB内存的机器上可以打开的文件数大约是10万左右
    # 我们来看看360M内存的VPS可以打开的文件句柄数是多少：
    # $ cat /proc/sys/fs/file-max
    # 输出 34336
    # 32000 < 34336，即并发连接总数小于系统可以打开的文件句柄总数，这样就在操作系统可以承受的范围之内
    # 所以，worker_connections 的值需根据 worker_processes 进程数目和系统可以打开的最大文件总数进行适当地进行设置
    # 使得并发总数小于操作系统可以打开的最大文件数目
    # 其实质也就是根据主机的物理CPU和内存进行配置
    # 当然，理论上的并发总数可能会和实际有所偏差，因为主机还有其他的工作进程需要消耗系统资源。
    # ulimit -SHn 65535

}


http {
    #设定mime类型,类型由mime.type文件定义
    include    mime.types;
    default_type  application/octet-stream;
    #设定日志格式
    log_format  main  '$remote_addr - $remote_user [$time_local] "$request" '
                      '$status $body_bytes_sent "$http_referer" '
                      '"$http_user_agent" "$http_x_forwarded_for"';

    access_log  logs/access.log  main;

    #sendfile 指令指定 nginx 是否调用 sendfile 函数（zero copy 方式）来输出文件，
    #对于普通应用，必须设为 on,
    #如果用来进行下载等应用磁盘IO重负载应用，可设置为 off，
    #以平衡磁盘与网络I/O处理速度，降低系统的uptime.
    sendfile     on;
    #tcp_nopush     on;

    #连接超时时间
    #keepalive_timeout  0;
    keepalive_timeout  65;
    tcp_nodelay     on;

    #开启gzip压缩
    gzip  on;
    gzip_disable "MSIE [1-6].";

    #设定请求缓冲
    client_header_buffer_size    128k;
    large_client_header_buffers  4 128k;


    #设定虚拟主机配置
    server {
        #侦听80端口
        listen    80;
        #定义使用 www.nginx.cn访问
        server_name  www.nginx.cn;

        #定义服务器的默认网站根目录位置
        root html;

        #设定本虚拟主机的访问日志
        access_log  logs/nginx.access.log  main;

        #默认请求
        location / {
            
            #定义首页索引文件的名称
            index index.php index.html index.htm;   

        }

        # 定义错误提示页面
        error_page   500 502 503 504 /50x.html;
        location = /50x.html {
        }

        #静态文件，nginx自己处理
        location ~ ^/(images|javascript|js|css|flash|media|static)/ {
            
            #过期30天，静态文件不怎么更新，过期可以设大一点，
            #如果频繁更新，则可以设置得小一点。
            expires 30d;
        }

        #PHP 脚本请求全部转发到 FastCGI处理. 使用FastCGI默认配置.
        location ~ .php$ {
            fastcgi_pass 127.0.0.1:9000;
            fastcgi_index index.php;
            fastcgi_param  SCRIPT_FILENAME  $document_root$fastcgi_script_name;
            include fastcgi_params;
        }

        #禁止访问 .htxxx 文件
            location ~ /.ht {
            deny all;
        }

    }
}
```
<a name="KEp71"></a>
### 配置文件3
```nginx
#nginx进程数，建议设置为等于CPU总核心数.
worker_processes 8; 
#全局错误日志定义类型，[ debug | info | notice | warn | error | crit ]
error_log /var/log/nginx/error.log info; 
#进程文件
pid /var/run/nginx.pid; 

#一个nginx进程打开的最多文件描述符数目，理论值应该是最多打开文件数（系统的值ulimit -n）与nginx进程数相除，但是nginx分配请求并不均匀，所以建议与ulimit -n的值保持一致。
worker_rlimit_nofile 65535;

#工作模式与连接数上限
events
{
　　#参考事件模型，use [ kqueue | rtsig | epoll | /dev/poll | select | poll ]; epoll模型是Linux 2.6以上版本内核中的高性能网络I/O模型，如果跑在FreeBSD上面，就用kqueue模型。
　　use epoll;
　　#单个进程最大连接数（最大连接数=连接数*进程数）
　　worker_connections 65535;
}

#设定http服务器
http
{

    include mime.types; #文件扩展名与文件类型映射表
    default_type application/octet-stream; #默认文件类型
    #charset utf-8; #默认编码
    server_names_hash_bucket_size 128; #服务器名字的hash表大小
    client_header_buffer_size 32k; #上传文件大小限制
    large_client_header_buffers 4 64k; #设定请求缓
    client_max_body_size 8m; #设定请求缓
    sendfile on; #开启高效文件传输模式，sendfile指令指定nginx是否调用sendfile函数来输出文件，对于普通应用设为 on，如果用来进行下载等应用磁盘IO重负载应用，可设置为off，以平衡磁盘与网络I/O处理速度，降低系统的负载。注意：如果图片显示不正常把这个改成off。
    autoindex on; #开启目录列表访问，合适下载服务器，默认关闭。
    tcp_nopush on; #防止网络阻塞
    tcp_nodelay on; #防止网络阻塞
    keepalive_timeout 120; #长连接超时时间，单位是秒

    #FastCGI相关参数是为了改善网站的性能：减少资源占用，提高访问速度。下面参数看字面意思都能理解。
    fastcgi_connect_timeout 300;
    fastcgi_send_timeout 300;
    fastcgi_read_timeout 300;
    fastcgi_buffer_size 64k;
    fastcgi_buffers 4 64k;
    fastcgi_busy_buffers_size 128k;
    fastcgi_temp_file_write_size 128k;

    #gzip模块设置
    gzip on; #开启gzip压缩输出
    gzip_min_length 1k; #最小压缩文件大小
    gzip_buffers 4 16k; #压缩缓冲区
    gzip_http_version 1.0; #压缩版本（默认1.1，前端如果是squid2.5请使用1.0）
    gzip_comp_level 2; #压缩等级
    gzip_types text/plain application/x-javascript text/css application/xml;
    #压缩类型，默认就已经包含text/html，所以下面就不用再写了，写上去也不会有问题，但是会有一个warn。
    gzip_vary on;
    #limit_zone crawler $binary_remote_addr 10m; #开启限制IP连接数的时候需要使用

    upstream blog.ha97.com {
        #upstream的负载均衡，weight是权重，可以根据机器配置定义权重。weigth参数表示权值，权值越高被分配到的几率越大。
        server 192.168.80.121:80 weight=3;
        server 192.168.80.122:80 weight=2;
        server 192.168.80.123:80 weight=3;
    }

    #虚拟主机的配置
    server
    {
        listen 80;　　　　#监听端口
        server_name aa.cn www.aa.cn ; #server_name end  #域名可以有多个，用空格隔开
        index index.html index.htm index.php;  # 设置访问主页
        set $subdomain '';  # 绑定目录为二级域名 bbb.aa.com  根目录 /bbb  文件夹
        if ( $host ~* "(?:(\w+\.){0,})(\b(?!www\b)\w+)\.\b(?!(com|org|gov|net|cn)\b)\w+\.[a-zA-Z]+" ) 
        { 
            set $subdomain "/$2"; 
        }

        root /home/wwwroot/aa.cn/web$subdomain;# 访问域名跟目录  
        include rewrite/dedecms.conf; #rewrite end   #载入其他配置文件

        location ~ .*.(php|php5)?$
        {
            fastcgi_pass 127.0.0.1:9000;
        　　fastcgi_index index.php;
            include fastcgi.conf;
        }
    #图片缓存时间设置
        location ~ .*.(gif|jpg|jpeg|png|bmp|swf)$
        {
    　　　　expires 10d;
        }
    #JS和CSS缓存时间设置
        location ~ .*.(js|css)?$
        {
    　　　　expiresexpires 1h;
        }

    }

    #日志格式设定

    log_format access '$remote_addr - $remote_user [$time_local] "$request" '
    '$status $body_bytes_sent "$http_referer" '
    '"$http_user_agent" $http_x_forwarded_for';
    #定义本虚拟主机的访问日志
    access_log /var/log/nginx/ha97access.log access;

    #对 "/" 启用反向代理
    location / {
        proxy_pass http://127.0.0.1:88;
        proxy_redirect off;
        proxy_set_header X-Real-IP $remote_addr;

    #后端的Web服务器可以通过X-Forwarded-For获取用户真实IP
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;

    #以下是一些反向代理的配置，可选。
        proxy_set_header Host $host;
        client_max_body_size 10m; #允许客户端请求的最大单文件字节数
        client_body_buffer_size 128k; #缓冲区代理缓冲用户端请求的最大字节数，
        proxy_connect_timeout 90; #nginx跟后端服务器连接超时时间(代理连接超时)
        proxy_send_timeout 90; #后端服务器数据回传时间(代理发送超时)
        proxy_read_timeout 90; #连接成功后，后端服务器响应时间(代理接收超时)
        proxy_buffer_size 4k; #设置代理服务器（nginx）保存用户头信息的缓冲区大小
        proxy_buffers 4 32k; #proxy_buffers缓冲区，网页平均在32k以下的设置
        proxy_busy_buffers_size 64k; #高负荷下缓冲大小（proxy_buffers*2）
        proxy_temp_file_write_size 64k;
        #设定缓存文件夹大小，大于这个值，将从upstream服务器传

    }
    #设定查看Nginx状态的地址

    location /NginxStatus {
        stub_status on;
        access_log on;
        auth_basic "NginxStatus";
        auth_basic_user_file conf/htpasswd;
        #htpasswd文件的内容可以用apache提供的htpasswd工具来产生。
    }

    #本地动静分离反向代理配置
    #所有jsp的页面均交由tomcat或resin处理
    location ~ .(jsp|jspx|do)?$ {
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
        proxy_pass http://127.0.0.1:8080;
    }

    #所有静态文件由nginx直接读取不经过tomcat或resin
    location ~ .*.(htm|html|gif|jpg|jpeg|png|bmp|swf|ioc|rar|zip|txt|flv|mid|doc|ppt|pdf|xls|mp3|wma)$
    {　　 expires 15d;　　 
  }
    location ~ .*.(js|css)?$
    { 　　expires 1h;　　 
  }
}
```
<a name="1t7x5"></a>
### 配置文件4
```nginx
######Nginx配置文件nginx.conf中文详解#####

#定义Nginx运行的用户和用户组
user www www;

#nginx进程数，建议设置为等于CPU总核心数。
worker_processes 8;

#全局错误日志定义类型，[ debug | info | notice | warn | error | crit ]
error_log /usr/local/nginx/logs/error.log info;

#进程pid文件
pid /usr/local/nginx/logs/nginx.pid;

#指定进程可以打开的最大描述符：数目
#工作模式与连接数上限
#这个指令是指当一个nginx进程打开的最多文件描述符数目，理论值应该是最多打开文件数（ulimit -n）与nginx进程数相除，但是nginx分配请求并不是那么均匀，所以最好与ulimit -n 的值保持一致。
#现在在linux 2.6内核下开启文件打开数为65535，worker_rlimit_nofile就相应应该填写65535。
#这是因为nginx调度时分配请求到进程并不是那么的均衡，所以假如填写10240，总并发量达到3-4万时就有进程可能超过10240了，这时会返回502错误。
worker_rlimit_nofile 65535;


events
{
  #参考事件模型，use [ kqueue | rtsig | epoll | /dev/poll | select | poll ]; epoll模型
  #是Linux 2.6以上版本内核中的高性能网络I/O模型，linux建议epoll，如果跑在FreeBSD上面，就用kqueue模型。
  #补充说明：
  #与apache相类，nginx针对不同的操作系统，有不同的事件模型
  #A）标准事件模型
  #Select、poll属于标准事件模型，如果当前系统不存在更有效的方法，nginx会选择select或poll
  #B）高效事件模型
  #Kqueue：使用于FreeBSD 4.1+, OpenBSD 2.9+, NetBSD 2.0 和 MacOS X.使用双处理器的MacOS X系统使用kqueue可能会造成内核崩溃。
  #Epoll：使用于Linux内核2.6版本及以后的系统。
  #/dev/poll：使用于Solaris 7 11/99+，HP/UX 11.22+ (eventport)，IRIX 6.5.15+ 和 Tru64 UNIX 5.1A+。
  #Eventport：使用于Solaris 10。 为了防止出现内核崩溃的问题， 有必要安装安全补丁。
  use epoll;

  #单个进程最大连接数（最大连接数=连接数*进程数）
  #根据硬件调整，和前面工作进程配合起来用，尽量大，但是别把cpu跑到100%就行。每个进程允许的最多连接数，理论上每台nginx服务器的最大连接数为。
  worker_connections 65535;

  #keepalive超时时间。
  keepalive_timeout 60;

  #客户端请求头部的缓冲区大小。这个可以根据你的系统分页大小来设置，一般一个请求头的大小不会超过1k，不过由于一般系统分页都要大于1k，所以这里设置为分页大小。
  #分页大小可以用命令getconf PAGESIZE 取得。
  #[root@web001 ~]# getconf PAGESIZE
  #4096
  #但也有client_header_buffer_size超过4k的情况，但是client_header_buffer_size该值必须设置为“系统分页大小”的整倍数。
  client_header_buffer_size 4k;

  #这个将为打开文件指定缓存，默认是没有启用的，max指定缓存数量，建议和打开文件数一致，inactive是指经过多长时间文件没被请求后删除缓存。
  open_file_cache max=65535 inactive=60s;

  #这个是指多长时间检查一次缓存的有效信息。
  #语法:open_file_cache_valid time 默认值:open_file_cache_valid 60 使用字段:http, server, location 这个指令指定了何时需要检查open_file_cache中缓存项目的有效信息.
  open_file_cache_valid 80s;

  #open_file_cache指令中的inactive参数时间内文件的最少使用次数，如果超过这个数字，文件描述符一直是在缓存中打开的，如上例，如果有一个文件在inactive时间内一次没被使用，它将被移除。
  #语法:open_file_cache_min_uses number 默认值:open_file_cache_min_uses 1 使用字段:http, server, location 这个指令指定了在open_file_cache指令无效的参数中一定的时间范围内可以使用的最小文件数,如果使用更大的值,文件描述符在cache中总是打开状态.
  open_file_cache_min_uses 1;

  #语法:open_file_cache_errors on | off 默认值:open_file_cache_errors off 使用字段:http, server, location 这个指令指定是否在搜索一个文件是记录cache错误.
  open_file_cache_errors on;
}



#设定http服务器，利用它的反向代理功能提供负载均衡支持
http
{
  #文件扩展名与文件类型映射表
  include mime.types;

  #默认文件类型
  default_type application/octet-stream;

  #默认编码
  #charset utf-8;

  #服务器名字的hash表大小
  #保存服务器名字的hash表是由指令server_names_hash_max_size 和server_names_hash_bucket_size所控制的。参数hash bucket size总是等于hash表的大小，并且是一路处理器缓存大小的倍数。在减少了在内存中的存取次数后，使在处理器中加速查找hash表键值成为可能。如果hash bucket size等于一路处理器缓存的大小，那么在查找键的时候，最坏的情况下在内存中查找的次数为2。第一次是确定存储单元的地址，第二次是在存储单元中查找键 值。因此，如果Nginx给出需要增大hash max size 或 hash bucket size的提示，那么首要的是增大前一个参数的大小.
  server_names_hash_bucket_size 128;

  #客户端请求头部的缓冲区大小。这个可以根据你的系统分页大小来设置，一般一个请求的头部大小不会超过1k，不过由于一般系统分页都要大于1k，所以这里设置为分页大小。分页大小可以用命令getconf PAGESIZE取得。
  client_header_buffer_size 32k;

  #客户请求头缓冲大小。nginx默认会用client_header_buffer_size这个buffer来读取header值，如果header过大，它会使用large_client_header_buffers来读取。
  large_client_header_buffers 4 64k;

  #设定通过nginx上传文件的大小
  client_max_body_size 8m;

  #开启高效文件传输模式，sendfile指令指定nginx是否调用sendfile函数来输出文件，对于普通应用设为 on，如果用来进行下载等应用磁盘IO重负载应用，可设置为off，以平衡磁盘与网络I/O处理速度，降低系统的负载。注意：如果图片显示不正常把这个改成off。
  #sendfile指令指定 nginx 是否调用sendfile 函数（zero copy 方式）来输出文件，对于普通应用，必须设为on。如果用来进行下载等应用磁盘IO重负载应用，可设置为off，以平衡磁盘与网络IO处理速度，降低系统uptime。
  sendfile on;

  #开启目录列表访问，合适下载服务器，默认关闭。
  autoindex on;

  #此选项允许或禁止使用socke的TCP_CORK的选项，此选项仅在使用sendfile的时候使用
  tcp_nopush on;

  tcp_nodelay on;

  #长连接超时时间，单位是秒
  keepalive_timeout 120;

  #FastCGI相关参数是为了改善网站的性能：减少资源占用，提高访问速度。下面参数看字面意思都能理解。
  fastcgi_connect_timeout 300;
  fastcgi_send_timeout 300;
  fastcgi_read_timeout 300;
  fastcgi_buffer_size 64k;
  fastcgi_buffers 4 64k;
  fastcgi_busy_buffers_size 128k;
  fastcgi_temp_file_write_size 128k;

  #gzip模块设置
  gzip on; #开启gzip压缩输出
  gzip_min_length 1k; #最小压缩文件大小
  gzip_buffers 4 16k; #压缩缓冲区
  gzip_http_version 1.0; #压缩版本（默认1.1，前端如果是squid2.5请使用1.0）
  gzip_comp_level 2; #压缩等级
  gzip_types text/plain application/x-javascript text/css application/xml; #压缩类型，默认就已经包含textml，所以下面就不用再写了，写上去也不会有问题，但是会有一个warn。
  gzip_vary on;

  #开启限制IP连接数的时候需要使用
  #limit_zone crawler $binary_remote_addr 10m;



  #负载均衡配置
  upstream piao.jd.com {

    #upstream的负载均衡，weight是权重，可以根据机器配置定义权重。weigth参数表示权值，权值越高被分配到的几率越大。
    server 192.168.80.121:80 weight=3;
    server 192.168.80.122:80 weight=2;
    server 192.168.80.123:80 weight=3;

    #nginx的upstream目前支持4种方式的分配
    #1、轮询（默认）
    #每个请求按时间顺序逐一分配到不同的后端服务器，如果后端服务器down掉，能自动剔除。
    #2、weight
    #指定轮询几率，weight和访问比率成正比，用于后端服务器性能不均的情况。
    #例如：
    #upstream bakend {
    # server 192.168.0.14 weight=10;
    # server 192.168.0.15 weight=10;
    #}
    #2、ip_hash
    #每个请求按访问ip的hash结果分配，这样每个访客固定访问一个后端服务器，可以解决session的问题。
    #例如：
    #upstream bakend {
    # ip_hash;
    # server 192.168.0.14:88;
    # server 192.168.0.15:80;
    #}
    #3、fair（第三方）
    #按后端服务器的响应时间来分配请求，响应时间短的优先分配。
    #upstream backend {
    # server server1;
    # server server2;
    # fair;
    #}
    #4、url_hash（第三方）
    #按访问url的hash结果来分配请求，使每个url定向到同一个后端服务器，后端服务器为缓存时比较有效。
    #例：在upstream中加入hash语句，server语句中不能写入weight等其他的参数，hash_method是使用的hash算法
    #upstream backend {
    # server squid1:3128;
    # server squid2:3128;
    # hash $request_uri;
    # hash_method crc32;
    #}

    #tips:
    #upstream bakend{#定义负载均衡设备的Ip及设备状态}{
    # ip_hash;
    # server 127.0.0.1:9090 down;
    # server 127.0.0.1:8080 weight=2;
    # server 127.0.0.1:6060;
    # server 127.0.0.1:7070 backup;
    #}
    #在需要使用负载均衡的server中增加 proxy_pass http://bakend/;

    #每个设备的状态设置为:
    #1.down表示单前的server暂时不参与负载
    #2.weight为weight越大，负载的权重就越大。
    #3.max_fails：允许请求失败的次数默认为1.当超过最大次数时，返回proxy_next_upstream模块定义的错误
    #4.fail_timeout:max_fails次失败后，暂停的时间。
    #5.backup： 其它所有的非backup机器down或者忙的时候，请求backup机器。所以这台机器压力会最轻。

    #nginx支持同时设置多组的负载均衡，用来给不用的server来使用。
    #client_body_in_file_only设置为On 可以讲client post过来的数据记录到文件中用来做debug
    #client_body_temp_path设置记录文件的目录 可以设置最多3层目录
    #location对URL进行匹配.可以进行重定向或者进行新的代理 负载均衡
  } 

  #虚拟主机的配置
  server
    {
    #监听端口
    listen 80;

    #域名可以有多个，用空格隔开
    server_name www.jd.com jd.com;
    index index.html index.htm index.php;
    root /data/www/jd;

    #对******进行负载均衡
    location ~ .*.(php|php5)?$
      {
      fastcgi_pass 127.0.0.1:9000;
      fastcgi_index index.php;
      include fastcgi.conf;
    }

    #图片缓存时间设置
    location ~ .*.(gif|jpg|jpeg|png|bmp|swf)$
      {
      expires 10d;
    }

    #JS和CSS缓存时间设置
    location ~ .*.(js|css)?$
      {
      expires 1h;
    }

    #日志格式设定
    #$remote_addr与$http_x_forwarded_for用以记录客户端的ip地址；
    #$remote_user：用来记录客户端用户名称；
    #$time_local： 用来记录访问时间与时区；
    #$request： 用来记录请求的url与http协议；
    #$status： 用来记录请求状态；成功是200，
    #$body_bytes_sent ：记录发送给客户端文件主体内容大小；
    #$http_referer：用来记录从那个页面链接访问过来的；
    #$http_user_agent：记录客户浏览器的相关信息；
    #通常web服务器放在反向代理的后面，这样就不能获取到客户的IP地址了，通过$remote_add拿到的IP地址是反向代理服务器的iP地址。反向代理服务器在转发请求的http头信息中，可以增加x_forwarded_for信息，用以记录原有客户端的IP地址和原来客户端的请求的服务器地址。
    log_format access '$remote_addr - $remote_user [$time_local] "$request" '
      '$status $body_bytes_sent "$http_referer" '
      '"$http_user_agent" $http_x_forwarded_for';

    #定义本虚拟主机的访问日志
    access_log /usr/local/nginx/logs/host.access.log main;
    access_log /usr/local/nginx/logs/host.access.404.log log404;

    #对 "/" 启用反向代理
    location / {
      proxy_pass http://127.0.0.1:88;
      proxy_redirect off;
      proxy_set_header X-Real-IP $remote_addr;

      #后端的Web服务器可以通过X-Forwarded-For获取用户真实IP
      proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;

      #以下是一些反向代理的配置，可选。
      proxy_set_header Host $host;

      #允许客户端请求的最大单文件字节数
      client_max_body_size 10m;

      #缓冲区代理缓冲用户端请求的最大字节数，
      #如果把它设置为比较大的数值，例如256k，那么，无论使用firefox还是IE浏览器，来提交任意小于256k的图片，都很正常。如果注释该指令，使用默认的client_body_buffer_size设置，也就是操作系统页面大小的两倍，8k或者16k，问题就出现了。
      #无论使用firefox4.0还是IE8.0，提交一个比较大，200k左右的图片，都返回500 Internal Server Error错误
      client_body_buffer_size 128k;

      #表示使nginx阻止HTTP应答代码为400或者更高的应答。
      proxy_intercept_errors on;

      #后端服务器连接的超时时间_发起握手等候响应超时时间
      #nginx跟后端服务器连接超时时间(代理连接超时)
      proxy_connect_timeout 90;

      #后端服务器数据回传时间(代理发送超时)
      #后端服务器数据回传时间_就是在规定时间之内后端服务器必须传完所有的数据
      proxy_send_timeout 90;

      #连接成功后，后端服务器响应时间(代理接收超时)
      #连接成功后_等候后端服务器响应时间_其实已经进入后端的排队之中等候处理（也可以说是后端服务器处理请求的时间）
      proxy_read_timeout 90;

      #设置代理服务器（nginx）保存用户头信息的缓冲区大小
      #设置从被代理服务器读取的第一部分应答的缓冲区大小，通常情况下这部分应答中包含一个小的应答头，默认情况下这个值的大小为指令proxy_buffers中指定的一个缓冲区的大小，不过可以将其设置为更小
      proxy_buffer_size 4k;

      #proxy_buffers缓冲区，网页平均在32k以下的设置
      #设置用于读取应答（来自被代理服务器）的缓冲区数目和大小，默认情况也为分页大小，根据操作系统的不同可能是4k或者8k
      proxy_buffers 4 32k;

      #高负荷下缓冲大小（proxy_buffers*2）
      proxy_busy_buffers_size 64k;

      #设置在写入proxy_temp_path时数据的大小，预防一个工作进程在传递文件时阻塞太长
      #设定缓存文件夹大小，大于这个值，将从upstream服务器传
      proxy_temp_file_write_size 64k;
    }


    #设定查看Nginx状态的地址
    location /NginxStatus {
      stub_status on;
      access_log on;
      auth_basic "NginxStatus";
      auth_basic_user_file confpasswd;
      #htpasswd文件的内容可以用apache提供的htpasswd工具来产生。
    }

    #本地动静分离反向代理配置
    #所有jsp的页面均交由tomcat或resin处理
    location ~ .(jsp|jspx|do)?$ {
      proxy_set_header Host $host;
      proxy_set_header X-Real-IP $remote_addr;
      proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
      proxy_pass http://127.0.0.1:8080;
    }

    #所有静态文件由nginx直接读取不经过tomcat或resin
    location ~ .*.(htm|html|gif|jpg|jpeg|png|bmp|swf|ioc|rar|zip|txt|flv|mid|doc|ppt|
      pdf|xls|mp3|wma)$
      {
      expires 15d; 
    }

    location ~ .*.(js|css)?$
      {
      expires 1h;
    }
  }
}
######Nginx配置文件nginx.conf中文详解#####
```
<a name="2s3qm"></a>
### 配置文件5
```nginx
user                              nobody nobody;   ## 指定运行用户和组
worker_processes                  4;               ## 指定worker数量，建议此处auto
worker_rlimit_nofile              51200;           ## 最大打开文件描述符

error_log                         logs/error.log  notice;

pid                               /var/run/nginx.pid;

events {
  use                             epoll;            ## 使用epoll事件驱动模型
  worker_connections              51200;            ## 一个worker能处理的最大并发
}

http {
  server_tokens                   off;                ## 隐藏nginx版本
  include                         mime.types;        

  proxy_redirect                off;                ## 关闭代理重定向
  proxy_set_header              Host $host;            
  proxy_set_header              X-Real-IP $remote_addr;
  proxy_set_header              X-Forwarded-For $proxy_add_x_forwarded_for;
  client_max_body_size          20m;            ## 设置客户端请求body的最大允许大小
  client_body_buffer_size       256k;            ## 设置客户端请求body的缓冲区大小
  proxy_connect_timeout         90;                ## 与后端服务器连接的超时时长
  proxy_send_timeout            90;                ## 把请求发送给后端服务器的超时时长
  proxy_read_timeout            90;                ## 等待后端服务器发送响应报文的超时时长
  proxy_buffer_size             128k;            ## 从代理服务器接收的响应的第一部分缓冲区
  proxy_buffers                 4 64k;            ## 从代理服务器读取响应的缓冲区number和size
  proxy_busy_buffers_size       128k;            ## 限制size在响应尚未完全读取时可能忙于向客户端发送响应的缓冲区总数
  proxy_temp_file_write_size    128k;            ## 该指令设置缓冲临时文件的最大值

  default_type                    application/octet-stream;
  charset                         utf-8;        ## 字符集
  
  client_body_temp_path           /var/tmp/client_body_temp 1 2;  ## 请求body临时目录
  proxy_temp_path                 /var/tmp/proxy_temp 1 2;    ## 代理服务器接受数据临时目录
  fastcgi_temp_path               /var/tmp/fastcgi_temp 1 2; ## FastCGI服务器接收临时目录 
  uwsgi_temp_path                 /var/tmp/uwsgi_temp 1 2; ## uwsgi 服务器接收临时目录
  scgi_temp_path                  /var/tmp/scgi_temp 1 2; ##scgi服务器接收临时目录

  ignore_invalid_headers          on;        ## 开启控制忽略具有无效名称的标头字段
  server_names_hash_max_size      256;        ## 服务器名称哈希表的最大值
  server_names_hash_bucket_size   64;        ## 服务器名称哈希表存储bucket大小
  client_header_buffer_size       8k;        ## 设置缓冲区以读取客户端请求标头
  large_client_header_buffers     4 32k;    ## 设置缓冲区以读取客户端请求标头最大值number和size
  connection_pool_size            256;        ## 允许精确调整每个连接的内存分配
  request_pool_size               64k;        ## 允许精确调整每个请求的内存分配

  output_buffers                  2 128k;    ## 设置用于从磁盘读取响应的缓冲区number和size
  postpone_output                 1460;        ## 客户端数据的传输最小值，单位字节

  client_header_timeout           1m;        ## 定义读取客户端请求标头的超时时长
  client_body_timeout             3m;        ## 定义读取客户端请求主体的超时时长
  send_timeout                    3m;        ## 设置将响应传输到客户端的超时时长


  log_format main                 '$server_addr $remote_addr [$time_local] $msec+$connection '
                                  '"$request" $status $connection $request_time $body_bytes_sent "$http_referer" '
                                  '"$http_user_agent" "$http_x_forwarded_for"';

  open_log_file_cache               max=1000 inactive=20s min_uses=1 valid=1m;

  access_log                      logs/access.log      main;
  log_not_found                   on;


  sendfile                        on;            
  tcp_nodelay                     on;        ## 启用长连接马上响应，提高性能
  tcp_nopush                      off;        ## 关闭套接字选项

  reset_timedout_connection       on;        ## 启用重置超时连接
  keepalive_timeout               10 5;        ## 第一个参数设置长连接超时时长，第二个浏览器识别为keep-alive:timeout=5
  keepalive_requests              100;        ## 设置可通过一个保持活动连接提供的最大请求数


  gzip                            on;        ## 开启压缩
  gzip_http_version               1.1;        ## 启用压缩时协议最小版本
  gzip_vary                       on;         
  gzip_proxied                    any;        ## 为所有代理请求启用压缩
  gzip_min_length                 1024;        ## 设置将被gzip压缩的响应的最小长度
  gzip_comp_level                 6;        ## 设置压缩等级
  gzip_buffers                    16 8k;    ## 设置用于压缩响应的缓冲区number和size
  gzip_proxied                    expired no-cache no-store private auth no_last_modified no_etag;
  gzip_types                      text/plain application/x-javascript text/css application/xml application/json;
  gzip_disable                    "MSIE [1-6]\.(?!.*SV1)";


  upstream tomcat8080 {
    ip_hash;

    server                        172.16.100.103:8080 weight=1 max_fails=2;
    server                        172.16.100.104:8080 weight=1 max_fails=2;
    server                        172.16.100.105:8080 weight=1 max_fails=2;
  }

  server {
    listen                        80;
    server_name                   www.chan.com;
    # config_apps_begin
    root                          /data/webapps/htdocs;
    access_log                    /var/logs/webapp.access.log     main;
    error_log                     /var/logs/webapp.error.log      notice;

    location / {
    
      location ~* ^.*/favicon.ico$ {
        root                      /data/webapps;
        expires                   180d;        ## 缓存180天
        break;
      }
    
      if ( !-f $request_filename ) {
        proxy_pass                http://tomcat8080;
        break;
      }
    }

    error_page                    500 502 503 504  /50x.html;
      location = /50x.html {
      root                        html;
    }
  }

  server {
    listen                        8088;
    server_name                   nginx_status;

      location / {
          access_log                  off;
          deny                        all;
          return                      503;
      }

      location /status {
          stub_status                 on;
          access_log                  off;
          allow                       127.0.0.1;
          allow                       172.16.100.71;
          deny                        all;
      }
  }
}
```
