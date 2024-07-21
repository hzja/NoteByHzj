Nginx
<a name="Dp9ft"></a>
## 简介
通过proxy protocol协议，服务器端可以获得客户端的真实IP地址和端口，从而可以进行一些非常有意义的操作。<br />为什么获得客户端的真实IP地址会非常有意义呢？<br />考虑一个藏在proxy背后的数据库，如果有多个客户端通过proxy进行数据库的连接，事实上因为都是通过代理进行连接，所以数据库中的所有的操作都是proxy服务器的IP地址，这在对数据库的性能监控和优化是不利的，因为不知道真实异常的服务器IP地址。<br />这种情况下就需要用到proxy protocol协议，让数据库可以反映出真实客户端的IP地址，从而便于数据库的监控和问题定位。<br />事实上，数据库只是一个特定的例子，在很多其他的情况下也可能需要知道客户端真实IP的情况。<br />以现在最流行的http服务器和代理服务器Nginx为例，来看一下如何在Nginx中配置proxy protocol。
<a name="QYvDw"></a>
## proxy protocol在Nginx中应用
Nginx是一个web服务器和代理服务器，它一般工作在proxy server或者负载均衡软件（Haproxy,Amazon Elastic Load Balancer (ELB)的后面。<br />客户端首先请求proxy server或者LSB负载均衡软件，然后再到Nginx进行真实的web访问。<br />因为经过了多层软件，所以客户端的一些信息比如ip地址，端口号等可能就会被隐藏，这对于问题分析，数据统计都是不利的。因为对于Nginx来说，希望能够获得真实的客户端IP地址，这样才能获取真实的请求环境。<br />这种情况下就需要用到PROXY protocol了。<br />如果前面所说的proxy或者LSB都实现了PROXY protocol协议的话，不管是HTTP, SSL, HTTP/2, SPDY, WebSocket 还是 TCP协议，Nginx都可以拿到客户端的原始IP地址，从而根据原始IP地址进行一些特殊的操作，比如屏蔽恶意IP的访问，根据IP不同展示不同的语言或者页面，或者更加简单的日志记录和统计等，都非常有效。<br />当然，如果想要支持PROXY protocol，对Nginx的版本也是有要求的，具体版本需求如下：

- 想要支持PROXY protocol v2，需要NGINX Plus R16或者NGINX Open Source 1.13.11。
- 想要支持ROXY protocol for HTTP，需要NGINX Plus R3或者NGINX Open Source 1.5.12。
- 想要支持TCP client‑side PROXY protocol，需要NGINX Plus R7或者 NGINX Open Source 1.9.3。
- 想要支持PROXY protocol for TCP，需要NGINX Plus R11 或者 NGINX Open Source 1.11.4。

在Nginx中可以通过下面的变量来获得对应的客户端信息，具体而言如下所示：<br />`$proxy_protocol_addr`和`$proxy_protocol_port`分别表示的是原始客户端的IP地址和端口号。<br />`$remote_addr`和`$remote_port`表示的是load balancer的的IP地址和端口。<br />如果使用了RealIP扩展模块，那么这个模块会重写`$remote_addr`和`$remote_port`这两个值，将其替换成原始客户端的IP地址和端口号。<br />然后使用`$realip_remote_addr`和`$realip_remote_port`来表示load balancer的的IP地址和端口。<br />在RealIP扩展模块中，`$proxy_protocol_addr`和`$proxy_protocol_port`表示的含义不变，还是原始客户端的IP地址和端口号。
<a name="JWcsI"></a>
## 在Nginx中配置使用proxy protocol
上面提到了Nginx中proxy protocol的基本应用，下面来讲一下如何在Nginx中进行具体的配置。
<a name="AgkaG"></a>
### 在Nginx中启用proxy protocol
如果Nginx已经是支持proxy protocol的版本，那么启用proxy protocol非常简单，只需要在server中的listen中添加proxy_protocol即可，如下所示：
```nginx
http {
	#...
	server {
		listen 80   proxy_protocol;
		listen 443  ssl proxy_protocol;
		#...
	}
}

stream {
	#...
	server {
		listen 112233 proxy_protocol;
		#...
	}
}
```
可能大家比较熟悉的是http block，这表示的是Nginx对http/https的支持。stream模块可能大家比较陌生，这是Nginx提供的对tcp/udp协议的支持。<br />通过上面的配置，Nginx可以实现在tcp/udp协议和http/https协议同时支持proxy protocol。
<a name="fMasm"></a>
### 使用Real‑IP modules
Real‑IP modules是Nginx自带的一个模块，可以通过下面的命令来查看Nginx是否有安装real-ip模块：
```bash
nginx -V 2>&1 | grep -- 'http_realip_module'
nginx -V 2>&1 | grep -- 'stream_realip_module'
```
如果当前使用的版本没有real ip,也不要急，这时候可能需要从源代码进行编译。<br />在编译的过程中，需要执行一个`configure`命令，在这个`configure`命令中可以指定要开启的功能，比如stream或者http_ssl_module：
```bash
$ ./configure
--sbin-path=/usr/local/nginx/nginx
--conf-path=/usr/local/nginx/nginx.conf
--pid-path=/usr/local/nginx/nginx.pid
--with-pcre=../pcre-8.44
--with-zlib=../zlib-1.2.11
--with-http_ssl_module
--with-stream
--with-mail
```
如果要开启real-ip功能，则可以添加：
```bash
--with-http_realip_module
```
如果Nginx是运行在SLB或者proxy之后的，那么可以通过set_real_ip_from命令来指定代理或者负载均衡服务器的IP范围，如下所示：
```nginx
server {
    #...
    set_real_ip_from 192.168.1.0/24;
    #...
}
```
然后需要将proxy或者SLB的IP地址替换成为真实客户端的地址，那么可以这样使用：
```nginx
http {
	server {
		#...
		real_ip_header proxy_protocol;
	}
}
```
<a name="K35tQ"></a>
### 请求转发
不管是http还是stream block，都可能遇到请求向后续的upstream进行转发的情况，对于upstream来说，他们希望收到的是真实客户端IP地址，而不是proxy或者slb的地址，那么可以通过下面的设置来解决：
```nginx
http {
    proxy_set_header X-Real-IP       proxy_protocol_addr;
    proxy_set_header X-Forwarded-Forproxy_protocol_addr;
}
stream {
    server {
        listen 12345;
        proxy_pass example.com:12345;
        proxy_protocol on;
    }
}
```
http和stream的设置方式是不同的。
<a name="lYtu2"></a>
### 日志记录
日志是一个非常重要的功能，对于定位问题，执行数据统计分析都非常有用，当然需要的是真实的客户端IP地址。<br />可以通过使用变量`$proxy_protocol_addr`在http和stream block中记录对应的日志，如下所示：
```nginx
http {
	#...
	log_format combined 'proxy_protocol_addr -remote_user [time_local] '
		'"request" statusbody_bytes_sent '
		'"http_referer" "http_user_agent"';
}
stream {
	#...
	log_format basic 'proxy_protocol_addr -remote_user [time_local] '
		'protocol statusbytes_sent bytes_received '
		'session_time';
}
```
<a name="BtjRa"></a>
## 总结
通过上面的设置，Nginx已经可以使用proxoy protocol了，这会让后续分析工作变得更加轻松。
