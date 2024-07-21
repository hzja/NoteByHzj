Nginx<br />严格说Nginx并没有自带针对负载均衡后端节点的健康检查功能，但是可以通过默认自带的ngx_http_proxy_module 模块和ngx_http_upstream_module模块中的相关指令来完成当后端节点出现故障时，自动切换到健康节点。
<a name="PzIiD"></a>
### 1、ngx_http_proxy_module模块

- proxy_connect_timeout 指令<br />
```
语  法:    proxy_connect_timeout time;
默认值:    proxy_connect_timeout 60s;
上下文:    http, server, location
```
该指令的功能是设置与后端服务器建立连接的超时时间。应该注意超时一般不可能大于75秒。

- proxy_read_timeout指令<br />
```
语  法:    proxy_read_timeout time;
默认值:    proxy_read_timeout 60s;
上下文:    http, server, location
```
定义从后端服务器读取响应的超时时间。该超时是指相邻两次读操作之间的最长时间间隔，而不是整个响应传输完成的最长时间。如果后端服务器在超时时间段内没有传输任何数据，则连接将被关闭。

- proxy_next_upstream指令<br />
```
语  法:    proxy_next_upstream error | timeout | invalid_header | http_500 | http_502 | http_503 | http_504 |http_404 | off ...;
默认值:    proxy_next_upstream error timeout;
上下文:    http, server, location
```
指定在何种情况下一个失败的请求应该被发送到下一台后端服务器节点。<br />error：Nginx与后端服务器建立连接时；或者向后端服务器发送请求时；或者从后端服务器接收响应头时，出现错误<br />timeout：和后端服务器建立连接时；或者向后端服务器发送请求时；或者从后端服务器接收响应头时，出现超时<br />invalid_header：后端服务器返回空响应或者非法响应头<br />http_500：后端服务器返回的响应状态码为500<br />http_502：后端服务器返回的响应状态码为502<br />http_503：后端服务器返回的响应状态码为503<br />http_504：后端服务器返回的响应状态码为504<br />http_404 ：后端服务器返回的响应状态码为404<br />off ：停止将请求发送给下一台后端服务器<br />需要理解一点的是，只有在没有向客户端发送任何数据之前，将请求转给下一台后端服务器才是可行的。也就是说，如果在传输响应到客户端时出现错误或者超时，这类错误是不可能恢复的。<br />例子：
```nginx
http {
    proxy_next_upstream http_502 http_504 http_404 error timeout invalid_header;
}
```
<a name="phdmS"></a>
### 2、ngx_http_upstream_module模块

- server指令<br />
```
语  法:    server address [parameters];
默认值:    —
上下文:    upstream
```
max_fails=number：设定Nginx与服务器通信的尝试失败的次数。在fail_timeout参数定义的时间段内，如果失败的次数达到此值，Nginx就认为服务器不可用。在下一个fail_timeout时间段，服务器不会再被尝试。失败的尝试次数默认是1。设为0就会停止统计尝试次数，认为服务器是一直可用的。可以通过指令proxy_next_upstream、fastcgi_next_upstream和 memcached_next_upstream来配置什么是失败的尝试。默认配置时，http_404状态不被认为是失败的尝试。<br />fail_timeout=time：设定服务器被认为不可用的时间段以及统计失败尝试次数的时间段。在这段时间中，服务器失败次数达到指定的尝试次数，服务器就被认为不可用。默认情况下，该超时时间是10秒。<br />例子：
```
upstream name {
   server 192.168.1.110:8081 max_fails=3 fail_timeout=60s;
   server 192.168.1.122:8082 max_fails=3 fail_timeout=60s;
}
```
:::warning
采用这个方式需要注意的一点是，如果后端有不健康节点，负载均衡器依然会先把该请求转发给该不健康节点，然后再转发给别的节点，这样就会浪费一次转发。
:::
