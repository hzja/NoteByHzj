Nginx 日志
<a name="QTjHm"></a>
## 前言
Nginx日志对于统计、系统服务排错很有用。<br />Nginx日志主要分为两种： `access_log`(访问日志)和`error_log`(错误日志)。通过访问日志可以得到用户的IP地址、浏览器的信息，请求的处理时间等信息。错误日志记录了访问出错的信息，可以帮助定位错误的原因。<br />本文将详细描述一下如何配置Nginx日志。
<a name="JvLfx"></a>
## 设置`access_log`
访问日志主要记录客户端的请求。客户端向Nginx服务器发起的每一次请求都记录在这里。客户端IP，浏览器信息，`referer`，请求处理时间，请求URL等都可以在访问日志中得到。当然具体要记录哪些信息，可以通过`log_format`指令定义。
<a name="h543M"></a>
### 语法
```nginx
access_log path [format [buffer=size] [gzip[=level]] [flush=time] [if=condition]]; # 设置访问日志  
access_log off; # 关闭访问日志  
```

- path 指定日志的存放位置。
- format 指定日志的格式。默认使用预定义的combined。
- buffer 用来指定日志写入时的缓存大小。默认是64k。
- gzip 日志写入前先进行压缩。压缩率可以指定，从1到9数值越大压缩比越高，同时压缩的速度也越慢。默认是1。
- flush 设置缓存的有效时间。如果超过flush指定的时间，缓存中的内容将被清空。
- if 条件判断。如果指定的条件计算为0或空字符串，那么该请求不会写入日志。

另外，还有一个特殊的值off。如果指定了该值，当前作用域下的所有的请求日志都被关闭。
<a name="WZ7xq"></a>
### 作用域
可以应用`access_log`指令的作用域分别有`http`，`server`，`location`，`limit_except`。也就是说，在这几个作用域外使用该指令，Nginx会报错。<br />以上是`access_log`指令的基本语法和参数的含义。下面看几个例子加深一下理解。
<a name="pbak8"></a>
### 基本用法
```nginx
access_log /var/logs/nginx-access.log
```
该例子指定日志的写入路径为/var/logs/nginx-access.log，日志格式使用默认的`combined`。
```nginx
access_log /var/logs/nginx-access.log buffer=32k gzip flush=1m  
```
该例子指定日志的写入路径为/var/logs/nginx-access.log，日志格式使用默认的`combined`，指定日志的缓存大小为32k，日志写入前启用gzip进行压缩，压缩比使用默认值1，缓存数据有效时间为1分钟。
<a name="gcEso"></a>
## 使用`log_format`自定义日志格式
Nginx预定义了名为`combined`日志格式，如果没有明确指定日志格式默认使用该格式：
```nginx
log_format combined '$remote_addr - $remote_user [$time_local] '  
                    '"$request" $status $body_bytes_sent '  
                    '"$http_referer" "$http_user_agent"';  
```
如果不想使用Nginx预定义的格式，可以通过`log_format`指令来自定义。
<a name="WNaNC"></a>
### 语法
```nginx
log_format name [escape=default|json] string ...;  
```

- name 格式名称。在access_log指令中引用。
- escape 设置变量中的字符编码方式是json还是default，默认是default。
- string 要定义的日志格式内容。该参数可以有多个。参数中可以使用Nginx变量。

下面是`log_format`指令中常用的一些变量：

| 变量 | 含义 |
| --- | --- |
| `$bytes_sent` | 发送给客户端的总字节数 |
| `$body_bytes_sent` | 发送给客户端的字节数，不包括响应头的大小 |
| `$connection` | 连接序列号 |
| `$connection_requests` | 当前通过连接发出的请求数量 |
| `$msec` | 日志写入时间，单位为秒，精度是毫秒 |
| `$pipe` | 如果请求是通过http流水线发送，则其值为"p"，否则为“." |
| `$request_length` | 请求长度（包括请求行，请求头和请求体） |
| `$request_time` | 请求处理时长，单位为秒，精度为毫秒，从读入客户端的第一个字节开始，直到把最后一个字符发送张客户端进行日志写入为止 |
| `$status` | 响应状态码 |
| `$time_iso8601` | 标准格式的本地时间,形如“2017-05-24T18:31:27+08:00” |
| `$time_local` | 通用日志格式下的本地时间，如"24/May/2017:18:31:27 +0800" |
| `$http_referer` | 请求的referer地址。 |
| `$http_user_agent` | 客户端浏览器信息。 |
| `$remote_addr` | 客户端IP |
| `$http_x_forwarded_for` | 当前端有代理服务器时，设置web节点记录客户端地址的配置，此参数生效的前提是代理服务器也要进行相关的x_forwarded_for设置。 |
| `$request` | 完整的原始请求行，如 "GET / HTTP/1.1" |
| `$remote_user` | 客户端用户名称，针对启用了用户认证的请求 |
| `$request_uri` | 完整的请求地址，如 "https://daojia.com/" |

下面演示一下自定义日志格式的使用：
```nginx
access_log /var/logs/nginx-access.log main  
log_format  main  '$remote_addr - $remote_user [$time_local] "$request" '  
                  '$status $body_bytes_sent "$http_referer" '  
                  '"$http_user_agent" "$http_x_forwarded_for"';  
```
使用`log_format`指令定义了一个main的格式，并在`access_log`指令中引用了它。假如客户端有发起请求：https://***.com/，看一下截取的一个请求的日志记录:
```nginx
112.195.209.90 - - [20/Feb/2018:12:12:14 +0800]   
"GET / HTTP/1.1" 200 190 "-" "Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N)   
AppleWebKit/537.36 (KHTML, like Gecko) Chrome/63.0.3239.132 Mobile Safari/537.36" "-"  
```
可以看到最终的日志记录中`$remote_user`、`$http_referer`、`$http_x_forwarded_for`都对应了一个-，这是因为这几个变量为空。
<a name="Lt0E7"></a>
## 设置`error_log`
错误日志在Nginx中是通过`error_log`指令实现的。该指令记录服务器和请求处理过程中的错误信息。
<a name="mEdwB"></a>
### 语法
配置错误日志文件的路径和日志级别。
```nginx
error_log file [level];  
Default:      
error_log logs/error.log error; 
```
第一个参数指定日志的写入位置。<br />第二个参数指定日志的级别。level可以是`debug`, `info`, `notice`, `warn`, `error`, `crit`, `alert`, `emerg`中的任意值。可以看到其取值范围是按紧急程度从低到高排列的。只有日志的错误级别等于或高于level指定的值才会写入错误日志中。默认值是error。
<a name="nKsTr"></a>
### 基本用法
```nginx
error_log /var/logs/nginx/nginx-error.log  
```
它可以配置在：`main`, `http`, `mail`, `stream`, `server`, `location`作用域。<br />例子中指定了错误日志的路径为：/var/logs/nginx/nginx-error.log，日志级别使用默认的error。
<a name="lkQUt"></a>
## `open_log_file_cache`
每一条日志记录的写入都是先打开文件再写入记录，然后关闭日志文件。如果日志文件路径中使用了变量，如access_log /var/logs/$host/nginx-access.log，为提高性能，可以使用`open_log_file_cache`指令设置日志文件描述符的缓存。
<a name="eeoCL"></a>
### 语法
```nginx
open_log_file_cache max=N [inactive=time] [min_uses=N] [valid=time];  
```

- max 设置缓存中最多容纳的文件描述符数量，如果被占满，采用LRU算法将描述符关闭。
- inactive 设置缓存存活时间，默认是10s。
- min_uses 在inactive时间段内，日志文件最少使用几次，该日志文件描述符记入缓存，默认是1次。
- valid：设置多久对日志文件名进行检查，看是否发生变化，默认是60s。
- off：不使用缓存。默认为off。
<a name="GXTfc"></a>
### 基本用法
```nginx
open_log_file_cache max=1000 inactive=20s valid=1m min_uses=2;  
```
它可以配置在`http`、`server`、`location`作用域中。<br />例子中，设置缓存最多缓存1000个日志文件描述符，20s内如果缓存中的日志文件描述符至少被被访问2次，才不会被缓存关闭。每隔1分钟检查缓存中的文件描述符的文件名是否还存在。
<a name="MU5oQ"></a>
## 总结
Nginx中通过`access_log`和`error_log`指令配置访问日志和错误日志，通过`log_format`可以自定义日志格式。如果日志文件路径中使用了变量，可以通过`open_log_file_cache`指令来设置缓存，提升性能。<br />另外，在`access_log`和`log_format`中使用了很多变量，这些变量没有一一列举出来，详细的变量信息可以参考Nginx官方文档：[http://nginx.org/en/docs/varindex.html](http://nginx.org/en/docs/varindex.html)
