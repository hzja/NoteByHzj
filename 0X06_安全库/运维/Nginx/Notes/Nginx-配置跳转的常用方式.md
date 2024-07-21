Nginx <br />随着应用服务的增多，服务可能部署在不同的服务器上。这些服务有可能存在IP、端口Port、请求的ContextPath等一样的情况，怎么合理的配置他们的跳转呢？下面介绍三种常见的跳转方式。
<a name="NLL4N"></a>
### 1、根据不同域名判断跳转不同服务
就是根据在nginx.conf配置的server_name与域名或者（或者IP）匹配跳转不同的服务。
```nginx
#当客户端访问www.domain.com,监听端口号为80,直接跳转到data/www目录下文件
server {
    listen       80;
       server_name  www.domain.com;
       location / {
          root   data/www;
          index  index.html index.htm;
    }
 }
 #当客户端访问bbs.domain.com,监听端口号为80,直接跳转到data/bbs目录下文件
server {
     listen       80;
     server_name  bbs.domain.com;
     location / {
         root   data/bbs;
         index  index.html index.htm;
     }
}
```
<a name="6ELrZ"></a>
### 2、根据不同端口判断跳转不同服务
就是根据在nginx.conf配置的listen指令匹配跳转不同的服务。
```nginx
#当客户端访问www.domain.com,监听端口号为8081,直接跳转到data/www目录下文件
server {
      listen       8081;
      server_name  www.domain.com;
      location / {
          root   data/www;
          index  index.html index.htm;
      }
}
#当客户端访问www.domain.com,监听端口号为8082,直接跳转到data/bbs目录下文件
server {
      listen       8082;
      server_name  www.domain.com;
      location / {
          root   data/bbs;
          index  index.html index.htm;
      }
}
```
<a name="elTaY"></a>
### 3、根据链接的ContextPath不同跳转不同的服务器
主要根据每个应用服务器的ContextPath的普通，匹配跳转到不同的服务器。
```nginx
#服务创建监听的端口号
server {
    #监听的端口号
    listen       80;
    #服务名称
    server_name  www.domain.com;
   # 匹配项目名称为bbs开头
   location /bbs/ {
       #  配置反向代理
       proxy_pass http://192.168.1.188:8081/;
       index  index.html index.htm;
   }
   # 匹配项目名称为blog开头
   location /blog/ {
        # 配置反向代理
        proxy_pass http://192.168.1.188:8082/;
       index  index.html index.htm;
   }
}
```
