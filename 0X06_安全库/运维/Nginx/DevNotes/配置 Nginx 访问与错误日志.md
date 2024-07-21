Nginx<br />Nginx是一个开放源代码的高性能HTTP和反向代理服务器，负责处理Internet上某些最大站点的负载。在管理NGINX网络服务器时，要执行的最常见任务之一就是检查日志文件。<br />在对服务器或应用程序问题进行故障排除时，知道如何配置和读取日志非常有用，因为它们提供了详细的调试信息。<br />Nginx用两种类型的日志记录其事件：访问日志和错误日志。访问日志记录有关客户端请求的信息，错误日志记录有关服务器和应用程序问题的信息。<br />本教程概述了如何配置和读取Nginx访问和错误日志。配置Nginx访问日志的格式与位置，配置错误日志格式与位置，默认的日志文件所在位置，读取和理解Nginx日志文件每个字段。
<a name="h9XTG"></a>
### 配置Nginx访问日志
每当处理客户请求时，Nginx都会在访问日志中生成一个新记录。每个事件记录都包含一个时间戳，并包含有关客户端和所请求资源的各种信息。访问日志可以显示访问者的位置，访问者的访问的页面等。<br />log_format指令允许定义记录日志的格式。access_log指令启用并设置日志文件的位置和使用的格式。<br />access_log指令的最基本语法如下：
```nginx
access_log log_file log_format;
```
其中log_file是日志文件的完整路径，log_format是日志文件使用的格式。可以在http，server或location指令的上下文中启用访问日志。<br />默认情况下，Nginx主配置文件中的http指令配置了全局访问日志格式。
```nginx
http {
  ...
  access_log  /var/log/nginx/access.log;
  ...
}
```
/etc/nginx/nginx.conf<br />为获得更好的可维护性，建议为每个服务器设置一个单独的访问日志文件。在server指令中设置的access_log指令将覆盖在http指令中设置的access_log。
```nginx
http {
  ...
  access_log  /var/log/nginx/access.log;
  ...

  server {
    server_name domain.com
    access_log  /var/log/nginx/domain.access.log;
    ...
  }
}
```
/etc/nginx/conf.d/domain.com.conf<br />如果未指定日志格式，Nginx将使用预定义的combined组合格式，如下所示：
```nginx
log_format combined '$remote_addr - $remote_user [$time_local] '
                    '"$request" $status $body_bytes_sent '
                    '"$http_referer" "$http_user_agent"';
```
要更改日志记录格式，请覆盖默认设置或定义一个新设置。例如，定义一个名为 main的新日志记录格式，它将用添加X-Forwarded-For header的值来扩展combined格式，在http或server中添加以下定义指令：
```nginx
log_format  custom  '$remote_addr - $remote_user [$time_local] "$request" '
                    '$status $body_bytes_sent "$http_referer" '
                    '"$http_user_agent" "$http_x_forwarded_for"';
```
要使用新日志格式，请在日志文件后指定其名称，如下所示：
```nginx
access_log  /var/log/nginx/access.log custom;
```
虽然访问日志提供了非常有用的信息。但它会占用磁盘空间，并可能影响服务器性能。如果服务器资源不足，并且网站繁忙，则可能要禁用访问日志。为此，请将access_log伪指令的值设置为`off`。理论上不会出现这种情况，Nginx会自动删除前期的日志文件：<br />access_log off;
<a name="yDjWq"></a>
### 配置错误日志
Nginx将应用程序和常规服务器错误的消息写入错误日志文件。如果在Web应用程序中遇到错误，则错误日志是开始进行排查问题的第一个位置。<br />error_log指令启用并设置错误日志的位置和严重性级别。它采用以下格式，可以在http，server或location上下文中进行设置：
```nginx
error_log log_file log_level
```
log_level参数设置日志记录级别。以下是按严重性从低到高列出的级别：

- debug-调试消息。
- info -信息性消息。
- notice -公告。
- warn - 警告。
- error -处理请求时出错。
- crit -关键问题。需要立即采取行动。
- alert -警报。必须立即采取行动。
- emerg - 紧急情况。系统处于无法使用的状态。

每个日志级别包括更高级别。例如，如果将日志级别设置为warn，则Nginx还将记录error，crit，alert和emerg消息。未指定log_level参数时，默认为error。<br />默认情况下，error_log指令在主nginx.conf文件内的http指令上下文中定义：
```nginx
http {
  ...
  error_log  /var/log/nginx/error.log;
  ...
}
```
/etc/nginx/nginx.conf<br />与访问日志相同，建议为每个服务器设置一个单独的错误日志文件，该文件将覆盖更高级别的设置如http指令上下文。<br />例如，要将domain.com的错误日志设置为warn，可以使用：
```nginx
http {
	...
		error_log  /var/log/nginx/error.log;
	...
		
		server {
		server_name domain.com
			error_log  /var/log/nginx/domain.error.log warn;
		...
	}
}
```
无论何时修改配置文件，都必须重新加载Nginx服务，以使更改生效。
<a name="FcMyB"></a>
### 日志文件的位置
在大多数Linux发行版中，例如Ubuntu ，CentOS和Debian。默认情况下，访问和错误日志位于/var/log/nginx目录中。
<a name="Mfxzy"></a>
### 读取和理解Nginx日志文件
可以使用cat，less，grep，cut ，awk等命令打开和解析nginx日志文件。以下是使用默认的Nginx日志格式的访问日志文件的记录：
```bash
192.168.33.1 - - [15/Oct/2019:19:41:46 +0000] "GET / HTTP/1" 200 396 "-" "Mozilla/0 (X11; Linux x86_64) AppleWebKit/536 (KHTML, like Gecko) Chrome/38120 Safari/536"
```
细分Nginx日志文件记录的每个字段意味着什么：

- `$remote_addr-192.168.33.1`-发出请求的客户端的IP地址。
- `$remote_user` ---HTTP身份验证用户。未设置用户名时，此字段显示-。
- `[$time_local]-[15/Oct/2019:19:41:46 +0000]`

本地服务器时间。

- `"$request" -"GET / HTTP/1.1"`-请求类型，路径和协议。
- `$status -200`-服务器响应代码。
- `$body_bytes_sent -396`-服务器响应的大小（以字节为单位）。
- `"$http_referer"` -"-"-引荐网址。
- `"$http_user_agent" -Mozilla/5.0` ...-客户端的用户代理（网络浏览器）。

使用tail命令实时观看日志文件记录：
```bash
tail -f access.log
```
