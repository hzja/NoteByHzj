Nginx<br />反向代理是一种服务，它接受客户端请求，将请求发送到一个或多个代理服务器，获取响应，然后将服务器的响应传递给客户端。<br />由于其性能和可伸缩性，NGINX通常用作HTTP和非HTTP服务器的反向代理。典型的反向代理配置是将Nginx放在Node.js ， Python或Java应用程序的前面。<br />使用Nginx作为反向代理可以带来其他好处：

- 负载均衡-Nginx可以执行负载均衡，以在代理服务器之间分配客户端的请求，从而提高性能，可伸缩性和可靠性。
- 缓存-使用Nginx作为反向代理，可以缓存页面的预渲染版本以加快页面加载时间。它通过缓存从代理服务器的响应接收的内容，并使用它来响应客户端，而不必每次都与代理服务器联系以获取相同的内容。
- SSL-Nginx可以充当与客户端连接的SSL端点。它将处理和解密传入的SSL连接并加密代理服务器的响应。
- Compression -如果代理服务器未发送压缩的响应，则可以配置Nginx在发送响应之前对其进行压缩
- 减轻DDoS攻击-可以将传入请求和每个IP地址的连接数限制为常规用户的典型值。Nginx还允许根据客户端位置以及请求标头的值（例如"User-Agent"和"Referer"）来阻止或限制访问。

本文概述了将Nginx配置为反向代理所需的步骤。
<a name="f8MyQ"></a>
### 先决条件
假设在Ubuntu ， CentOS 或 Debian 服务器上安装了Nginx。
<a name="A9NIQ"></a>
### 使用Nginx作为反向代理
要将Nginx配置为HTTP服务器的反向代理，请打开域的服务器配置文件，并在其中指定位置和代理服务器：
```nginx
server {
    listen 80;
    server_name www.example.com example.com;

    location /app {
       proxy_pass http://127.0.0.1:8080;
    }
}
```
代理服务器的URL使用proxy_pass伪指令设置，可以将HTTP或HTTPS用作协议，域名或IP地址，并使用可选的端口和URI作为地址。<br />上面的配置告诉Nginx将所有请求/app 位置传递到到http://127.0.0.1:8080处的代理服务器。<br />在基于Ubuntu和Debian的发行版中，服务器配置文件存储在
```nginx
/etc/nginx/sites-available
```
目录中，而在CentOS上则存储在/etc/nginx/conf.d目录中。<br />为更好地说明location和proxy_pass指令的工作方式，举以下示例：
```nginx
server {
    listen 80;
    server_name www.example.com example.com;

    location /blog {
       proxy_pass http://nodecom:8000/wordpress/;
    }
}
```
如果访问者访问http://example.com/blog/my-post,则Nginx会将请求转发给http://nodecom:8000/wordpress/my-post<br />当代理服务器的地址包含URI（/wordpress/）时，传递给代理服务器的请求URI将替换为伪指令中指定的URI。如果指定的代理服务器地址没有URI，则完整的请求URI将传递到代理服务器。
<a name="LIiM7"></a>
### 传递请求标头header
当Nginx代理请求时，它会自动在客户端的代理请求中定义两个头字段Host和`Connection`，并删除空头。Host设置为`$proxy_host`变量，`Connection`设置为关闭。<br />要调整或设置代理连接的header，请使用`proxy_set_header`指令，后跟header值。可以在此处找到所有可用请求header及其允许值的列表。如果要防止将header传递到代理服务器，请将其设置为空字符串`""`。<br />在以下示例中，将Host header字段的值更改为`$host`，并通过将其值设置为空字符串来删除`Accept-Encoding`标头字段。
```nginx
location / {
    proxy_set_header Host $host;
    proxy_set_header Accept-Encoding "";
    proxy_pass http://localhost:3000;
}
```
无论何时修改配置文件，都必须重新启动Nginx服务，以使更改生效。
<a name="Wpwmm"></a>
### 将Nginx配置为非HTTP代理服务器的反向代理
要将Nginx配置为非HTTP代理服务器的反向代理，可以使用以下指令：

- `fastcgi_pass`-FastCGI服务器的反向代理。
- `uwsgi_pass` -反向代理到uwsgi服务器。
- `scgi_pass` -反向代理到SCGI服务器。
- `memcached_pass` -反向代理到 Memcached 服务器。

最常见的例子之一是使用Nginx作为 PHP-FPM 的反向代理
```nginx
server {

    # ... other directives

    location ~ \.php$ {
        include snippets/fastcgi-php.conf;
        fastcgi_pass unix:/run/php/php7.2-fpm.sock;
    }
}
```
<a name="S7RU9"></a>
### 常见的Nginx反向代理选项
如今，通过HTTPS提供内容已成为一种标准。提供HTTPS Nginx反向代理配置的示例，其中包括推荐的Nginx代理参数和header。
```nginx
location/ {
	proxy_pass http://127.0.0.1:3000;
	proxy_http_version  1.1;
	proxy_cache_bypass  $http_upgrade;
	
	proxy_set_header Upgrade           $http_upgrade;
	proxy_set_header Connection        "upgrade";
	proxy_set_header Host              $host;
	proxy_set_header X-Real-IP         $remote_addr;
	proxy_set_header X-Forwarded-For   $proxy_add_x_forwarded_for;
	proxy_set_header X-Forwarded-Proto $scheme;
	proxy_set_header X-Forwarded-Host  $host;
	proxy_set_header X-Forwarded-Port  $server_port;
}
```

- `proxy_http_version 1`- 定义用于代理的HTTP协议版本，默认情况下将其设置为1.0。对于Websocket和`keepalive`连接，需要使用1.1版。
- `proxy_cache_bypass $http_upgrade`-设置不从缓存中获取响应的条件。
- `Upgrade $http_upgrade`和`Connection "upgrade"`-如果应用程序使用Websockets，则这些header字段是必填字段。
- `Host $host` - 按以下优先顺序排列的`$host`变量包含：请求行中的主机名，或Host请求头字段中的主机名，或与请求匹配的服务器名。
- `X-Real-IP $remote_addr`-  将真实的访客远程IP地址转发到代理服务器。
- `X-Forwarded-For $proxy_add_x_forwarded_for`- 包含客户端被代理的每个服务器的IP地址的列表。
- `X-Forwarded-Proto $scheme`- 在HTTPS服务器块中使用时，来自代理服务器的每个HTTP响应都会重写为HTTPS。
- `X-Forwarded-Host $host`- 定义客户端请求的原始主机。
- `X-Forwarded-Port $server_port`- 定义客户端请求的原始端口。
