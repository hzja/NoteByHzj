自动化部署
<a name="JCckb"></a>
## **介绍**
Go Runner 是领先的开源自动化项目部署服务！支持各种 web、php、java、python、go 等代码的发布，**所有操作可以通过 web 来一键完成**。Go Runner 基于 go 构建安装简单，无需安装其他依赖环境，开箱即用。Go Runner 是一个可自由配置项目的开源上线部署系统。
<a name="t2r1o"></a>
## **特性**

- **支持 gitlab、github、gitee仓库；**
- **支持手动发布上线部署；**
- **支持Test、Dev等多种环境webhook自动化部署，减少团队协作及开发成本；**
- **websocket 实时展示部署中的 shell console**
<a name="Ah0Gm"></a>
## **下载**
可以通过访问 [https://gitee.com/marksirl/go-runner/releases](https://gitee.com/marksirl/go-runner/releases) 选择自己需要的安装包（根据操作系统）进行下载
<a name="UM7Ga"></a>
## **安装**
下载完成解压到任意文件夹。看到的目录结构如下<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657205992231-1f8105b7-24a9-44f8-8b5a-20cd49a5b44d.png#averageHue=%23061518&clientId=u34bdc6f0-6133-4&from=paste&id=uab23962d&originHeight=262&originWidth=1056&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6388413c-49cd-446e-a620-1a11dcf8213&title=)
<a name="Lqccn"></a>
## **配置**
<a name="sl4ZS"></a>
### 配置 etc/config.yaml
具体的配置文件如下：
```yaml
# 配置文件
Host: "127.0.0.1" # 运行addr
Port: "8080"      # 运行端口

# webhook 自动化部署回调域名，根域名，如www.gn.com/www.ol.com
Domain: "http://go.runner.io"

# mysql (此处配置暂时不用理会)
MysqlDNS: mysql://root:123456@tcp(127.0.0.1:3306)/pave?charset=utf8mb4&parseTime=True&loc=Local
TimeZone: Asia/Shanghai

# 本地redis连接配置
RedisDNS: 61c07e1ae08d08a2@127.0.0.1:6379
# 默认redis使用库
DefaultDb: 1
# redis队列使用库
QueueDb: 12
# 消费者work数量 
WorkNumber: 1
```
<a name="xWz2A"></a>
### Nginx 配置
```nginx
upstream runner {
	server 0.0.0.0:8080 weight=1;
}

server {
	listen       80;
	server_name  go.runner.io; # 域名设置
	access_log   /usr/local/var/log/nginx/runner.log; # 日志目录
	error_log    /usr/local/var/log/nginx/runner.error.log; # 日志目录
	
	index index.html index.htm;
	
	add_header Access-Control-Allow-Origin *;
	add_header Access-Control-Allow-Methods 'GET, POST, OPTIONS, PUT, DELETE, PATCH';
	add_header Access-Control-Allow-Headers 'DNT,Keep-Alive,User-Agent,Cache-Control,Content-Type,Authorization,Access-Token';
	# cors
	if ($request_method = 'OPTIONS') {
		return 204;
	}
	
	# api
	location / {
		client_max_body_size       50m;
		client_body_buffer_size    128k;
		proxy_connect_timeout      300;
		proxy_send_timeout         300;
		proxy_read_timeout         300;
		proxy_buffer_size          4k;
		proxy_buffers              4 32k;
		proxy_busy_buffers_size    64k;
		proxy_temp_file_write_size 64k;
		
		# 将客户端的 Host 和 IP 信息一并转发到对应节点
		proxy_set_header Host $http_host;
		proxy_set_header X-Real-IP $remote_addr;
		proxy_set_header REMOTE-HOST $remote_addr;
		proxy_set_header HTTP-VIA $http_via;
		proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
		
		# 转发Cookie，设置 SameSite
		# proxy_cookie_path / "/; secure; HttpOnly; SameSite=strict";
		proxy_cookie_path / "/; HttpOnly; SameSite=Lax";
		
		# 执行代理访问真实服务器
		proxy_pass http://runner;
	}
	
	# websocket
	location /ws {
		proxy_set_header Upgrade "websocket";
		proxy_set_header Connection "upgrade";
		proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
		proxy_set_header Host $host;
		proxy_http_version 1.1;
		
		# 转发到多个 ws server
		proxy_pass http://runner;
	}
}
```
<a name="rhcLe"></a>
## **运行项目**
运行项目前请保证 data 目录和 runtime 目录的可写权限
<a name="ywDkP"></a>
### windows 用户打开cmd 运行
```bash
.\awesome-runner.exe
```
<a name="uqMbb"></a>
### mac、linux 运行
```bash
./awesome-runner 
```
项目运行成功如下：<br />![2022-07-07-10-12-27-359978.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1657160096545-f5697b17-dc81-430c-bac6-4c38fb916892.png#averageHue=%23141414&clientId=u0f0a2aad-bb26-4&from=ui&id=u4335f149&originHeight=478&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1551785&status=done&style=shadow&taskId=u8cdd187b-46a6-41e0-a54f-420089b0a2f&title=)<br />打开浏览器访问http://go.runner.io/web 进入 web 控制台
<a name="kkNTo"></a>
### Web界面
![2022-07-07-10-12-27-410223.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1657160096619-5f136c6f-ffb6-4163-838a-009abf727fb4.png#averageHue=%23a3a4a3&clientId=u0f0a2aad-bb26-4&from=ui&id=rnlAt&originHeight=519&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1684878&status=done&style=shadow&taskId=uf4304e2d-9877-42a0-a8d2-5c57d4192c8&title=)![2022-07-07-10-12-27-480039.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1657160096608-6a13271f-bfa3-4b1c-ae28-6a428114926d.png#averageHue=%23c7c8c7&clientId=u0f0a2aad-bb26-4&from=ui&id=Yocsu&originHeight=519&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1684878&status=done&style=shadow&taskId=u43f5a01f-b065-46dd-b7a1-e1b17c0e7cf&title=)<br />![2022-07-07-10-12-27-623362.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1657160118183-630f6d81-852c-4905-b467-e61e4d2cbe6a.png#averageHue=%238f8f8d&clientId=u0f0a2aad-bb26-4&from=ui&id=u419fd1cc&originHeight=521&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1691372&status=done&style=shadow&taskId=uedf4c563-fd01-4505-b1b9-f8561f5e4f7&title=)
<a name="FZEgj"></a>
## **项目部署脚本**
需要明确部署脚本文件名为 xxx.runner-ci.yml，它是存在于实际项目根目录下，随仓库提交更新下载，用于约束和申明部署时执行的脚本的配置文件。配置文件格式说明如下：
```yaml
# 工作目录
webpath: /Users/zrone/Developments/dtest/runnnn
# 部署分支，至少一个，可以配置多个，用于自动化部署触发部署
only:
  - master
  - dev
  - test
# 环境变量，用于脚本执行时申明环境变量，仅当前session有效，不会污染全局，可配置多个
environment:
  workdir: /home/wwwroot/deploy
  a: b
  c: d
# pull前运行脚本
prepare:
  - nginx -s stop
# pull 完成运行脚本
script:
  - cd ${webpath} && pwd
# 发布运行脚本
release:
  - nginx 
  - netstae -tunlp | grep 8080
```
<a name="vMXhR"></a>
## **Webhook 自动化部署配置**
<a name="HnIM5"></a>
### sshkey
自动化部署使用 webhook 通知实现，支持 gitlab/github/gitee，实现前建议配置 sshkey，可通过以下代码生成 sshkey。
```bash
ssh-keygen -t rsa
```
<a name="OuhEc"></a>
### github webhook 配置
![2022-07-07-10-12-27-683548.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1657160117892-ca3c585e-717b-448e-a8bc-b15ae2b24938.png#averageHue=%23deddde&clientId=u0f0a2aad-bb26-4&from=ui&id=db58l&originHeight=345&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1120031&status=done&style=shadow&taskId=u090d5ea5-e2dd-46d8-8c86-c12c274d42b&title=)![2022-07-07-10-12-27-801130.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1657160118327-361342c8-d5b2-4999-bf98-8a274b07a18b.png#averageHue=%23cccccc&clientId=u0f0a2aad-bb26-4&from=ui&id=dsGjz&originHeight=557&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1808236&status=done&style=shadow&taskId=ud8dd1d5b-dd5b-4bb8-abcd-b2ac71b9e29&title=)Payload Url 和 Secret，可在web后台项目配置中找到，分别对应通知地址和秘钥，复制粘贴保存即可。<br />![2022-07-07-10-12-27-909972.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1657160147775-7b31dcfc-b011-4018-a5db-a3899bfff1f4.png#averageHue=%23b1b1b0&clientId=u0f0a2aad-bb26-4&from=ui&id=u114386d6&originHeight=322&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1045365&status=done&style=shadow&taskId=u4ade9ea9-a1da-48d0-8081-db4162698f1&title=)
<a name="D5s2N"></a>
### gitlab webhook 配置
![2022-07-07-10-12-28-013318.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1657160148370-501fe10a-8917-42ab-9bd1-dc682f8e6eee.png#averageHue=%23e1e1e1&clientId=u0f0a2aad-bb26-4&from=ui&id=aNhry&originHeight=866&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2811319&status=done&style=shadow&taskId=u6fd267a6-8c5a-415f-9934-ef8505ea86a&title=)配置链接和私密授权码即可
<a name="bLYeB"></a>
### gitee webhook 配置
![2022-07-07-10-12-28-171163.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1657160147974-6a399c4c-6f38-43b5-a20a-6d332b26fbd2.png#averageHue=%23e0dfdf&clientId=u0f0a2aad-bb26-4&from=ui&id=kyRFw&originHeight=521&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1691372&status=done&style=shadow&taskId=ud7bf229d-7a46-49c3-bccb-3e0377ed9ed&title=)需要注意的是，gitee 的签名方式需要选择签名秘钥，否则可能无法通过验证。
