Nginx 前端跨域<br />Nginx的CORS配置几乎都是类似下面这三两行：
```nginx
add_header Access-Control-Allow-Origin *;
add_header Access-Control-Allow-Headers X-Requested-With;
add_header Access-Control-Allow-Methods GET,POST,OPTIONS;
```
这样有用么？有用，以前这样使用也正常过，但后来还是遇到问题了，发现有些项目请求就不成功，也遇到有些浏览器成功，有些浏览器不成功；<br />在网上查找各种资料和调整写法，最后调整好的写法，基本的使用没问题！<br />部分配置示例：
```nginx
location /xxx-web {
  add_header 'Access-Control-Allow-Origin' $http_origin;
  add_header 'Access-Control-Allow-Credentials' 'true';
  add_header 'Access-Control-Allow-Methods' 'GET, POST, OPTIONS';
  add_header 'Access-Control-Allow-Headers' 'DNT,web-token,app-token,Authorization,Accept,Origin,Keep-Alive,User-Agent,X-Mx-ReqToken,X-Data-Type,X-Auth-Token,X-Requested-With,If-Modified-Since,Cache-Control,Content-Type,Range';
  add_header 'Access-Control-Expose-Headers' 'Content-Length,Content-Range';
  if ($request_method = 'OPTIONS') {
    add_header 'Access-Control-Max-Age' 1728000;
    add_header 'Content-Type' 'text/plain; charset=utf-8';
    add_header 'Content-Length' 0;
    return 204;
  }
  root   html;
  index  index.html index.htm;
  proxy_pass http://127.0.0.1:8080;
  proxy_set_header Host $host;
  proxy_set_header X-Real-IP $remote_addr;
  proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
  proxy_set_header X-Forwarded-Proto $scheme;
  proxy_connect_timeout 5;
}
```
跨域相关的配置，主要是下面这部分：
```nginx
add_header 'Access-Control-Allow-Origin' $http_origin;
add_header 'Access-Control-Allow-Credentials' 'true';
add_header 'Access-Control-Allow-Methods' 'GET, POST, OPTIONS';
add_header 'Access-Control-Allow-Headers' 'DNT,web-token,app-token,Authorization,Accept,Origin,Keep-Alive,User-Agent,X-Mx-ReqToken,X-Data-Type,X-Auth-Token,X-Requested-With,If-Modified-Since,Cache-Control,Content-Type,Range';
add_header 'Access-Control-Expose-Headers' 'Content-Length,Content-Range';
if ($request_method = 'OPTIONS') {
  add_header 'Access-Control-Max-Age' 1728000;
  add_header 'Content-Type' 'text/plain; charset=utf-8';
  add_header 'Content-Length' 0;
  return 204;
}

```
下面简单讲解一下，以便大家配置成功！<br />1、`Access-Control-Allow-Origin`，这里使用变量 `$http_origin`取得当前来源域，用“*”代表允许所有，实际使用并不成功，原因未知；<br />2、`Access-Control-Allow-Credentials`，为 true 的时候指请求时可带上Cookie，自己按情况配置；<br />3、`Access-Control-Allow-Methods`，OPTIONS一定要有的，另外一般也就GET和POST，如果有其它的也可加进去；<br />4、`Access-Control-Allow-Headers`，这个要注意，里面一定要包含自定义的http头字段（就是说前端请求接口时，如果在http头里加了自定义的字段，这里配置一定要写上相应的字段），从上面可看到写的比较长，里面有“web-token”和“app-token”；<br />5、`Access-Control-Expose-Headers`，可不设置，大致意思是默认只能获返回头的6个基本字段，要获取其它额外的，先在这设置才能获取它；<br />6、语句“` if ($request_method = 'OPTIONS') {` ”，因为浏览器判断是否允许跨域时会先往后端发一个 options 请求，然后根据返回的结果判断是否允许跨域请求，所以这里单独判断这个请求，然后直接返回；<br />按上面配置基本都可使用，下面发一个实际配置供参考，做了少量更改，如下：
```nginx

server {
  listen       80;
  server_name  xxx.com; 
  location /xxx-web/papi {
    add_header 'Access-Control-Allow-Origin' $http_origin;
    add_header 'Access-Control-Allow-Credentials' 'true';
    add_header 'Access-Control-Allow-Methods' 'GET, POST, OPTIONS';
    add_header 'Access-Control-Allow-Headers' 'DNT,web-token,app-token,Authorization,Accept,Origin,Keep-Alive,User-Agent,X-Mx-ReqToken,X-Data-Type,X-Auth-Token,X-Requested-With,If-Modified-Since,Cache-Control,Content-Type,Range';
    add_header 'Access-Control-Expose-Headers' 'Content-Length,Content-Range';
    if ($request_method = 'OPTIONS') {
      add_header 'Access-Control-Max-Age' 1728000;
      add_header 'Content-Type' 'text/plain; charset=utf-8';
      add_header 'Content-Length' 0;
      return 204;
    }
    root   html;
    index  index.html index.htm;
    proxy_pass http://127.0.0.1:7071;
    proxy_set_header Host $host;
    proxy_set_header X-Real-IP $remote_addr;
    proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
    proxy_set_header X-Forwarded-Proto $scheme;
    proxy_connect_timeout 5;
  }
 
  location /xxx-web {
    add_header 'Access-Control-Allow-Origin' $http_origin;
    add_header 'Access-Control-Allow-Credentials' 'true';
    add_header 'Access-Control-Allow-Methods' 'GET, POST, OPTIONS';
    add_header 'Access-Control-Allow-Headers' 'DNT,web-token,app-token,Authorization,Accept,Origin,Keep-Alive,User-Agent,X-Mx-ReqToken,X-Data-Type,X-Auth-Token,X-Requested-With,If-Modified-Since,Cache-Control,Content-Type,Range';
    add_header 'Access-Control-Expose-Headers' 'Content-Length,Content-Range';
    if ($request_method = 'OPTIONS') {
      add_header 'Access-Control-Max-Age' 1728000;
      add_header 'Content-Type' 'text/plain; charset=utf-8';
      add_header 'Content-Length' 0;
      return 204;
    }
    root   html;
    index  index.html index.htm;
    proxy_pass http://127.0.0.1:8080;
    proxy_set_header Host $host;
    proxy_set_header X-Real-IP $remote_addr;
    proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
    proxy_set_header X-Forwarded-Proto $scheme;
    proxy_connect_timeout 5;
  } 
  location / {
    root   /var/www/xxx/wechat/webroot;
    index  index.html index.htm;
  } 
  error_page   500 502 503 504  /50x.html;
  location = /50x.html {
    root   html;
  }
}
```
