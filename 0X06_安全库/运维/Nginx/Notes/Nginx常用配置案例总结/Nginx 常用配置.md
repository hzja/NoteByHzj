Nginx<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647564488283-f60f1d28-b74f-47e2-b0d4-9e218f336d46.png#clientId=u5df5f863-b630-4&from=paste&id=uf774c8cf&originHeight=540&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8cc9ee94-48f4-4562-b71e-21b6b03839c&title=)

---

<a name="mJ1GW"></a>
## 安装 Nginx
```bash
# 更新软件包，请确保服务器的软件包已经是最新的。
sudo yum update -y

# 添加 Nginx 源
sudo rpm -Uvh http://nginx.org/packages/centos/7/noarch/RPMS/nginx-release-centos-7-0.el7.ngx.noarch.rpm

# 安装 Nginx
sudo yum install -y nginx

# 启动 Nginx
sudo systemctl start nginx.service

# 设置开机自启 Nginx
sudo systemctl enable nginx.service
```
<a name="Muu3i"></a>
## 配置 Nginx
```bash
# 使用 vim 编辑创建 文件名.conf
vim /etc/nginx/conf.d/文件名.conf
```
添加配置1（配置1和2可以同事监听80端口，绑定不同域名，实现一台服务器nginx同时配置多个前端网站，多个域名空格隔开）
```nginx
server {
    listen 80;
    server_name 域名1 域名2 localhost;

    location / {
        proxy_set_header HOST $host;
        proxy_set_header X-Forwarded-Proto $scheme;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;

        proxy_pass http://127.0.0.1:端口/;
    }
}
```
添加配置2
```nginx
server {
    listen 80;
    server_name 域名1 域名2 localhost;

    location / {
        proxy_set_header HOST $host;
        proxy_set_header X-Forwarded-Proto $scheme;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;

        proxy_pass http://127.0.0.1:端口/;
    }

}
```
代理静态网站 root指定磁盘目录
```nginx
server {
    listen       80;
    server_name  域名1 域名2 多个域名空格隔开 localhost;
    location / {
       try_files $uri $uri/ /index.html;
       root D:\test-web;
    }
}
```
<a name="DmF7Q"></a>
## 常用命令
检查配置是否有误
```bash
sudo nginx -t
```
重载/重启 Nginx 配置
```bash
sudo nginx -s reload
```
查看nginx安装目录
```bash
ps -ef | grep nginx
```
<a name="lRKnT"></a>
## 错误及解决办法
不能访问子路径错误：
```nginx
location /flow {
    proxy_pass http://127.0.0.1:9083/;
}
```
修改：
```nginx
location /flow/ {
    proxy_pass http://127.0.0.1:9083/;
}
```
解决：`/flow` 后面加个`/`<br />配置多个前端代码/a/ /b/不能访问<br />解决：把/a/ /b/放在根目录下面 （就是只有一个根目录，不过根目录里面可以放很多项目文件夹）<br />去掉请求后面的斜杠 添加在location节点里面 解决浏览器请求拼接到接口前面的诡异问题
```nginx
rewrite ^/(.*)/$ /$1 permanent;
```
<a name="bFqe5"></a>
## 前后端分离项目部署
```nginx
##前端配置
location /gov_flow {
	##放再nginx下的/content/gov_flow目录
	alias /var/html/gov_flow;
	try_files $uri $uri/ /index.html;
	index index.html;
}

##后端接口配置            
location /stage-prod-api/ {
	proxy_set_header Host $http_host;
	proxy_set_header X-Real-IP $remote_addr;
	proxy_set_header REMOTE-HOST $remote_addr;
	proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
	proxy_pass http://flow:9083/;
}
location /stage-test-api/{
	proxy_set_header Host $http_host;
	proxy_set_header X-Real-IP $remote_addr;
	proxy_set_header REMOTE-HOST $remote_addr;
	proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
	proxy_pass http://localhost:8080/;
}

#两个后端项目接口合并成一个 给前端访问9991
server {
	listen       9991;
	server_name  localhost;
	location /yqt/{
		proxy_pass http://127.0.0.1:8888/yqt/;
	}
	
	location /ad/{
		proxy_pass http://127.0.0.1:8090/ad/;
	}
}   
```
<a name="iVOPS"></a>
## nginx配置ssl证书示例1
```nginx
http{
  #http节点中可以添加多个server节点
  server{
      #ssl 需要监听443端口
      listen 443;
      # CA证书对应的域名
      server_name www.ilovey.live;
      # 开启ssl
      ssl on;
      # 服务器证书绝对路径
      ssl_certificate /www/server/nginx/conf.d/4467149_www.ilovey.live.pem;
      # 服务器端证书key绝对路径 
      ssl_certificate_key /www/server/nginx/conf.d/4467149_www.ilovey.live.key;
      # session超时
      ssl_session_timeout 5m;
      # 协议类型
      ssl_protocols TLSv1 TLSv1.1 TLSv1.2;
      # ssl算法列表 
      ssl_ciphers ECDHE-RSA-AES128-GCM-SHA256:HIGH:!aNULL:!MD5:!RC4:!DHE;
      #  是否 服务器决定使用哪种算法  on/off   TLSv1.1 的话需要开启
      ssl_prefer_server_ciphers on;

      location ^~  /zhihao/ {
          proxy_set_header Host $host;
          proxy_pass http://127.0.0.1:8080/;
      }
  }
  # 如果用户通过 http 访问 直接重写 跳转到 https 这个是一个很有必要的操作
  server{
      listen 80;
      server_name www.ilovey.live;
      rewrite ^/(.*)$ https://www.ilovey.live:443/$1 permanent;
  }
}
```
<a name="G2Zun"></a>
## nginx配置ssl证书示例2
```nginx
server
{
	listen 80;
	listen 443 ssl http2;
	server_name ilovey.live;
	index index.php index.html index.htm default.php default.htm default.html;
	root /www/wwwroot/网站目录;
	
	#SSL-START SSL相关配置，请勿删除或修改下一行带注释的404规则
	#error_page 404/404.html;
	ssl_certificate    /www/server/nginx/conf.d/4467149_www.ilovey.live.pem;
	ssl_certificate_key    /www/server/nginx/conf.d/4467149_www.ilovey.live.key;
	ssl_protocols TLSv1 TLSv1.1 TLSv1.2;
	ssl_ciphers ECDHE-RSA-AES128-GCM-SHA256:HIGH:!aNULL:!MD5:!RC4:!DHE;
	ssl_prefer_server_ciphers on;
	ssl_session_cache shared:SSL:10m;
	ssl_session_timeout 10m;
	error_page 497  https://$host$request_uri;
	
	
	#SSL-END
	
	#ERROR-PAGE-START  错误页配置，可以注释、删除或修改
	#error_page 404 /404.html;
	#error_page 502 /502.html;
	#ERROR-PAGE-END
	
	#PHP-INFO-START  PHP引用配置，可以注释或修改
	
	#PROXY-START
	location /
		{
		proxy_pass http://localhost:8886;
		proxy_set_header Host $host;
		proxy_set_header X-Real-IP $remote_addr;
		proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
		proxy_set_header REMOTE-HOST $remote_addr;
		
		#持久化连接相关配置
		#proxy_connect_timeout 30s;
		#proxy_read_timeout 86400s;
		#proxy_send_timeout 30s;
		#proxy_http_version 1.1;
		#proxy_set_header Upgrade $http_upgrade;
		#proxy_set_header Connection "upgrade";
		##expires 12h;
	}
	
	#PHP-INFO-END
	
	
	
	#禁止访问的文件或目录
	location ~ ^/(\.user.ini|\.htaccess|\.git|\.svn|\.project|LICENSE|README.md)
		{
		return 404;
	}
	
	#一键申请SSL证书验证目录相关设置
	location ~ \.well-known{
		allow all;
	}
}
```
