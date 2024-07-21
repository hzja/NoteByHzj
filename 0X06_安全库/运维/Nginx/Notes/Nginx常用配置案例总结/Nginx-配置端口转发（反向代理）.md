Nginx 端口转发 反向代理
<a name="Hd73T"></a>
## 将域名转发到本地端口
最常用的就是将请求转发到本地另一个端口上
```nginx
server{
  listen 80;
  server_name localhost;
  index  index.php index.html index.htm;
  location / {
    proxy_pass http://127.0.0.1:8080; # 转发规则
    proxy_set_header Host $proxy_host; # 修改转发请求头，让8080端口的应用可以受到真实的请求
    proxy_set_header X-Real-IP $remote_addr;
    proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
  }
}
```
这样通过公网IP或者localhost访问时就会转发到本地的 8080 端口
<a name="UZgIq"></a>
## 将域名转发到另一个域名
```nginx
server{
  listen 80;
  server_name localhost;
  index  index.php index.html index.htm;
  location / {
    proxy_pass http://www.baidu.com;
    proxy_set_header Host $proxy_host;
    proxy_set_header X-Real-IP $remote_addr;
    proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
  }
}
```
这样通过公网IP或者localhost访问时就会转发到http://www.baidu.com，如果未指定目标端口，则默认为对应协议绑定的端口，比如HTTP绑定的80端口，HTTPS绑定的443端口。
<a name="tO4kG"></a>
## 本地端口转发到其他服务器的域名以及指定端口
```nginx
server{
  listen 80;
  server_name 127.0.0.1; # 公网ip
  index  index.php index.html index.htm;
  location / {
    proxy_pass http://127.0.0.1:8080; # 指定的目标端口
    proxy_set_header Host $proxy_host;
    proxy_set_header X-Real-IP $remote_addr;
    proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
  }
}
```
这样访问 http://127.0.0.1 时就会转发到本地的 8080 端口。
<a name="4pjSh"></a>
## `proxy_pass`代理转发时加 `/` 与不加 `/`
在配置proxy_pass代理转发时，如果后面的url加`/`，表示绝对根路径；如果没有`/`，表示相对路径<br />例如
<a name="uXity"></a>
### 加 `/`
```nginx
server_name localhost;
location /data/ {
    proxy_pass http://127.0.0.1/;
}
```
访问localhost时会转发到 http://127.0.0.1/index.html
<a name="S98A7"></a>
### 不加 `/`
```nginx
server_name localhost;
location /data/ {
    proxy_pass http://127.0.0.1;
}
```
访问localhost时会转发到 http://127.0.0.1/data/index.html
