Nginx 负载均衡
<a name="LpmTA"></a>
## 1、插件说明
<a name="9C6q7"></a>
### nginx_upstream_hash
url_hash是nginx的第三方模块，nginx本身不支持，需要第三方模块。nginx在做负载均衡的时，把转发的URL以hash的形式保存。这样可以保证同一个URL始终分给同一个real server，来提高访问速度。<br />官网：[https://github.com/evanmiller/nginx_upstream_hash](https://github.com/evanmiller/nginx_upstream_hash)
<a name="nginx-upstream-fair"></a>
### nginx-upstream-fair
upstream-fair是比内建的负载均衡更加智能的负载均衡模块，它采用的不是内建负载均衡使用的轮换的均衡算法，而是可以根据页面大小、加载时间长短智能的进行负载均衡。<br />官网：[https://github.com/gnosek/nginx-upstream-fair](https://github.com/gnosek/nginx-upstream-fair)
<a name="Zxv0y"></a>
### ngx_http_consistent_hash
通过一致性哈希算法来选择合适的后端节点<br />官网：[https://github.com/replay/ngx_http_consistent_hash](https://github.com/replay/ngx_http_consistent_hash)<br />[https://www.nginx.com/resources/wiki/modules/consistent_hash/](https://www.nginx.com/resources/wiki/modules/consistent_hash/)
<a name="VZlha"></a>
## 2、安装说明
<a name="YSsgH"></a>
### 分别下载这三款插件
```bash
wget https://github.com/gnosek/nginx-upstream-fair/archive/master.zip -O nginx-upstream-fair.zip
wget https://github.com/evanmiller/nginx_upstream_hash/archive/master.zip -O nginx_upstream_hash.zip
wget https://github.com/replay/ngx_http_consistent_hash/archive/master.zip -O ngx_http_consistent_hash.zip
```
<a name="bl2TP"></a>
### 解压对应的压缩包
```bash
unzip nginx-upstream-fair-master.zip 
unzip nginx_upstream_hash-master.zip 
unzip ngx_http_consistent_hash-master.zip
```
<a name="FwyjB"></a>
### 查看旧Nginx的安装配置参数
```bash
/usr/local/nginx/sbin/nginx -V
```
<a name="FSka6"></a>
### 重新编译添加负载均衡模块
```bash
./configure --prefix=/usr/local/nginx \
--user=nginx --group=nginx \
--with-http_secure_link_module \
--with-http_stub_status_module \
--with-stream_ssl_preread_module \
--with-http_ssl_module --with-http_image_filter_module \
--add-module=/local/dev/nginx/plugins/nginx_upstream_hash-master  \
--add-module=/local/dev/nginx/plugins/fastfds/nginx-upstream-fair-master  \
--add-module=/local/dev/nginx/plugins/fastfds/ngx_http_consistent_hash-master
```
关键编译参数是后面的添加模块，采用指向本地导入的方式。
<a name="NevVq"></a>
### 安装nginx
```bash
make
```
<a name="cIvKY"></a>
#### (1) 对于nginx_upstream_hash会出现多重定义错误
说明Nginx内部包含了一个同名的模块。

- 需要对nginx_upstream_hash进行一定的代码修改，ngx_http_upstream_hash_module定义<br />
```basic
ngx_module_t  ngx_http_upstream_hash_module = {
    NGX_MODULE_V1,
    &ngx_http_upstream_hash_module_ctx,    /* module context */
    ngx_http_upstream_hash_commands,       /* module directives */
    NGX_HTTP_MODULE,                       /* module type */
    NULL,                                  /* init master */
    NULL,                                  /* init module */
    NULL,                                  /* init process */
    NULL,                                  /* init thread */
    NULL,                                  /* exit thread */
    NULL,                                  /* exit process */
    NULL,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};
```
修改成
```basic
ngx_module_t  ngx_http_upstream_hash_module_ext = {
    NGX_MODULE_V1,
    &ngx_http_upstream_hash_module_ctx,    /* module context */
    ngx_http_upstream_hash_commands,       /* module directives */
    NGX_HTTP_MODULE,                       /* module type */
    NULL,                                  /* init master */
    NULL,                                  /* init module */
    NULL,                                  /* init process */
    NULL,                                  /* init thread */
    NULL,                                  /* exit thread */
    NULL,                                  /* exit process */
    NULL,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};
```
应用这个结构体的代码一并修改。

- 修改config配置文件<br />
```bash
ngx_addon_name=ngx_http_upstream_hash_module_ext
HTTP_MODULES="$HTTP_MODULES ngx_http_upstream_hash_module_ext"
NGX_ADDON_SRCS="$NGX_ADDON_SRCS $ngx_addon_dir/ngx_http_upstream_hash_module_ext.c"
have=NGX_HTTP_UPSTREAM_HASH . auto/have
~
```

- c文件名字也改下<br />
```bash
ngx_http_upstream_hash_module.c 
改成
ngx_http_upstream_hash_module_ext.c
```
这样重新configure和make才能成功
<a name="AWmFx"></a>
#### (2)对于nginx-upstream-fair会出现以下错误
```bash
nginx-upstream-fair/ngx_http_upstream_fair_module.c:543:28: 
error: ‘ngx_http_upstream_srv_conf_t’ has no member named ‘default_port’
```
到nginx源码目录找到src/http/ngx_http_upstream.h文件ngx_http_upstream_srv_conf_s结构添加in_port_t default_port;<br />经过错误修复，编译成功后，把objs目录下的nginx文件拷贝到/usr/local/nginx目录即可：
```bash
cp ./objs/nginx /usr/local/nginx
```
<a name="tLi33"></a>
## 3、基本语法
:::warning
以下三种随便选择，不过nginx_upstream_hash与nginx-upstream-fair已经很久没更新了。
:::

- nginx_upstream_hash<br />
```nginx
upstream backend_server {
    server 127.0.0.1:5000;
    server 127.0.0.1:5001;
    server 127.0.0.1:5002;
    hash      $request_uri;
    hash_again  10;  # default 0
}
```

- nginx-upstream-fair<br />
```nginx
upstream mongrel {
  	fair;
    server 127.0.0.1:5000;
 		server 127.0.0.1:5001;
 		server 127.0.0.1:5002;
}
```

- ngx_http_consistent_hash<br />
```nginx
upstream somestream {
  consistent_hash $request_uri;
  server 127.0.0.1:5000;
  server 127.0.0.1:5001;
  server 127.0.0.1:5002;
}
```

