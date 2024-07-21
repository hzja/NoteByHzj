Web 服务器是 Web 开发中不可或缺的基础服务，在开发中经常会用到。耳熟能详的开源 Web 服务器有久负盛名的 Apache、性能强劲的 Nginx。这里介绍一个开源的项目是采用 Go 编写的 Web 服务端“后起之秀”：**Caddy** 它拥有下载无需安装就能用、零配置实现 HTTPS 等特点，从而在强者如云的 Web 服务器中占据了一席之地。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1646286199629-4ca6a44f-8b1d-4386-acaf-e617364283ff.png#clientId=u2f3e3e1e-64f2-4&from=paste&id=uc65c1eaf&originHeight=443&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue9486ec4-ca73-426e-b6ee-a4f9f207ff5&title=)<br />在这个 HTTPS 必选的时代，Caddy 凭借无需额外配置自动 HTTPS，分分钟完成 HTTPS 站点搭建，使它成为了中小型 Web 服务的首选服务器。Caddy 深受开源爱好者们的喜爱，2014 年开源至今共收获了 3.6 万颗星。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1646286199674-759530d4-dea5-4b76-8437-80bae7f13f88.png#clientId=u2f3e3e1e-64f2-4&from=paste&id=uf0b1db57&originHeight=635&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue97f2d51-55d9-470a-a38c-dd447443bf0&title=)<br />项目地址：[https://github.com/caddyserver/caddy](https://github.com/caddyserver/caddy)
<a name="aHFqe"></a>
## 一、为什么使用 Caddy ?
Caddy 是一个强大的、可扩展的平台，可以为您的站点、服务和应用程序提供服务。<br />使用 Caddy 可以在 Linux、Mac、Windows 上快速部署 http(s) 站点或反向代理服务。支持：

- HTTP/1.1 和 HTTP/2
- 同时接受 HTTPS 自动签发和手动管理
- 虚拟主机 (多个站点工作在单个端口上)
- 原生 IPv4 和 IPv6 支持
- 静态文件分发
- 平滑重启/重载
- 反向代理 (HTTP 或 WebSocket)
- 负载均衡和健康性检查
- Markdown 渲染
- 文件浏览服务
- 等等

与传统的 Nginx 或者 Apache 相比，Caddy 整体只有一个可执行文件，**安装便捷**不易出现奇怪的依赖问题，配置文件结构清晰语法简单**易于上手**，依托于模块化架构**可以使用 Go 语言快速开发扩展模块**。
<a name="MTQUS"></a>
## 二、安装
系统环境：Ubuntu 20.04 LTS<br />Caddy 可以作为一个系统服务安装，命令：
```bash
sudo apt install -y debian-keyring debian-archive-keyring apt-transport-https
curl -1sLf 'https://dl.cloudsmith.io/public/caddy/stable/gpg.key' | sudo tee /etc/apt/trusted.gpg.d/caddy-stable.asc
curl -1sLf 'https://dl.cloudsmith.io/public/caddy/stable/debian.deb.txt' | sudo tee /etc/apt/sources.list.d/caddy-stable.list
sudo apt update
sudo apt install caddy
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1646286199866-126c2453-a3d7-400f-a8f3-b7f51b65d160.png#clientId=u2f3e3e1e-64f2-4&from=paste&id=ud963bcc0&originHeight=366&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0d5efde1-1401-41d6-945f-f27de5b19ec&title=)
:::tips
注意：这种方式安装完成后会启动服务，开机自动启动。
:::
还可以下载一个可执行文件使用。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1646286199650-7c68c7b7-0b35-446f-8459-5c997ee69703.png#clientId=u2f3e3e1e-64f2-4&from=paste&id=u9c363a4d&originHeight=173&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue47cddd7-929f-4ad8-860c-b67d629afab&title=)

1. 下载地址：[https://caddyserver.com/download](https://caddyserver.com/download)
2. 在网页上方选择自己的操作系统后，点击右侧蓝色的 Download 按钮即可
3. 下载好的文件不能够直接运行，需要为其添加权限
```bash
$ mv caddy_linux_amd64 caddy # 将下载后的文件重命名方便后面指令输入，根据系统不同文件名不一样
$ sudo chmod a+x caddy # 为 Caddy 添加可执行权限
$ mv caddy /bin/caddy # 将 Caddy 复制到 bin 目录这样可以在命令行随时使用
```
命令行执行 `caddy version` 命令，出现版本信息：
```bash
v2.4.6 h1:HGkGICFGvyrodcqOOclHKfvJC0qTU7vny/7FhYp9hNw=
```
即为安装成功！
<a name="wfG5s"></a>
## 三、从零完成 HTTPS 站点
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1646286199691-9240879b-07ac-4187-b802-4d597bcf2218.png#clientId=u2f3e3e1e-64f2-4&from=paste&id=u4d72bf4d&originHeight=645&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub054b350-c26f-4186-b5ce-041b968a690&title=)<br />下面将演示如何通过 Caddy 完成 HTTPS 站点。只需要 4 步：<br />Tips 例子用的域名为：caddy.hellogithub.com 修改为你的域名即可。
<a name="qU9LQ"></a>
### 第一步：安装，采用上面介绍的命令安装
<a name="fxDdI"></a>
### 第二步：设置域名解析地址，在购买域名的网站设置
如下图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1646286200167-93530cee-62cf-4d18-a737-53ffd7a82d08.png#clientId=u2f3e3e1e-64f2-4&from=paste&id=ud8d2ff56&originHeight=845&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub9d384ed-583d-49d2-8be1-211c8581980&title=)<br />设置完生效需要几分钟，可通过下面的命令检查是否生效：
```bash
curl "https://cloudflare-dns.com/dns-query?name=caddy.hellogithub.com&type=A" -H "accept: application/dns-json"
```
域名设置生效，返回如下：
```json
{"Status":0,"TC":false,"RD":true,"RA":true,"AD":false,"CD":false,"Question":[{"name":"caddy.hellogithub.com","type":1}],"Answer":[{"name":"caddy.hellogithub.com","type":1,"TTL":592,"data":"107.150.122.176"}]}
```
<a name="FTBgk"></a>
### 第三步：创建配置文件 Caddyfile，无需额外的开启 HTTPS 的设置：
```
caddy.hellogithub.com

respond "Hello HTTPS！"
```

1. 域名
2. 访问返回的内容
<a name="pxMvR"></a>
### 第四步：启动，在同一目录下执行命令
```bash
sudo caddy run
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1646286200139-094b0664-e4bf-4b37-a818-1a030965eca2.png#clientId=u2f3e3e1e-64f2-4&from=paste&id=u4f7ae50d&originHeight=210&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub4199c35-3bfe-4cd2-8b4e-e4d44eaa99b&title=)<br />Caddy 默认使用执行命令目录下的 Caddyfile 文件内的配置。<br />最后，访问 caddy.hellogithub.com 检验效果吧！<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1646286200167-cacff890-cb8d-4673-8212-0c1e00523e34.png#clientId=u2f3e3e1e-64f2-4&from=paste&id=u8180cf3e&originHeight=645&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc13ff151-e1f4-4630-86d7-1822b8c22ac&title=)
<a name="LlQWr"></a>
## 四、上手教程
这里将从 Caddy 常用命令开始，再到配置讲解，最后会通过一个实际案例来展示如何用 Caddy 配置网站和上线。
<a name="yvuGm"></a>
### 4.1 常用命令
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1646286200341-73fb8138-3c61-4396-8ca4-19ebb72eda03.png#clientId=u2f3e3e1e-64f2-4&from=paste&id=u01099d55&originHeight=768&originWidth=816&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6871ce5d-ca53-42a9-9bcf-144ddc82e77&title=)
<a name="EEHVP"></a>
### 4.2 配置文件
Caddy 的原生配置文件使用的是 JSON 格式。但是为了用户编写方便它提供了 Caddyfile 作为接口让用户可以快速配置站点信息，运行时 Caddy 会自动将 Caddyfile 的配置信息转为 JSON 配置文件。<br />Caddyfile 所能提供功能不如 JSON 配置文件强大，但是对于不需要复杂配置的用户而言完全够用了。<br />Caddyfile 的配置指令格式如下：
```
directive [<matcher>] <args...> { # matcher 代表匹配器，如果提供则该指令将只对 matcher 描述的资源进行响应
	subdirective [<args...>]	# 子指令
}
```
Caddyfile 的文件结构如图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1646286200466-efae8ed7-c99d-4068-b258-978b4a890589.png#clientId=u2f3e3e1e-64f2-4&from=paste&id=u32b729a8&originHeight=597&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u33b0439f-de6a-4647-a615-2387cb15d11&title=)
<a name="BG8W0"></a>
### 4.3 演示
下面将演示如何用 Caddy 搭建站点，加深理解配置格式和命令使用。<br />目录结构：
```bash
.
├── Caddyfile
├── index.html
└── public
    └── HG.html
```
两个页面文件 index.html 和 HG.html 的内容如下：
```html
<!-- index.html -->
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="UTF-8">
		<title>Hello World!</title>
	</head>
	<body>
		你好，世界！
	</body>
</html>

<!-- HG.html -->
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="UTF-8">
		<title>HelloGitHub</title>
	</head>
	<body>
		HelloGitHub
	</body>
</html>
```
Caddyfile 配置内容如下：
```
# 如果本机没有 wordpress 则注释这一块儿的站点配置
#:80 { # 部署一个 wordpress 站点
#	root * /var/www/wordpress
#	php_fastcgi unix//run/php/php-version-fpm.sock # 配置一个 fastcig 服务
#	file_server	# 配置一个文件服务
#}

http://localhost:3000 {
	basicauth /public/* {
		# 匹配访问 localhost:3000/public/* 的请求，为其加上登陆保护
		HG JDJhJDE0JGEySk9janFMdHlBY2Y0aVdQZklQak9HcmwzNDZhNFg0N3V5Ny9EZkZMZHB1Nkt4ZE5BNGJt
		# 用户名 HG 密码 HelloGitHub，密码使用 caddy hash-passowrd 指令生成
	}

	root * ./ # 设置当前站点根目录为当前文件夹，* 表示匹配所有的 request
	templates
	file_server {
		# 配置当前站点为静态文件服务器，可用于博客系统的搭建
		hide .git # 隐藏所有的 .git 文件
	}
}

:4000 {
	reverse_proxy /public/* http://localhost:3000 # 配置反向代理
	# 只会匹配 locahost:4000/public 的请求，转发到 localhost:3000/public/
}
```
在当前目录输入：
```bash
$ caddy run # 启动 Caddy
```
最后，效果如下：

1. 访问：http://localhost:3000 可以看到页面展示 “你好，世界！”
2. 访问：http://localhost:3000/public/HG.html 提示输入用户名和密码，验证正确后才能看到页面。
3. 访问：http://localhost:4000 则会自动跳转到端口 3000 的页面
<a name="jNNs8"></a>
### 4.4 提供接口管理方式
Caddy 除了简单易懂的配置文件方式，还提供了管理配置的接口。通过这些接口可以轻松实现 Web 服务器管理自动化、集成发布等高级功能。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1646286200599-a42c8ef5-4694-4392-b639-936e35f192d3.png#clientId=u2f3e3e1e-64f2-4&from=paste&id=u03ec3121&originHeight=842&originWidth=1020&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub4d94017-f6f0-40ee-bfb8-4eee1158ced&title=)<br />提供的接口和功能：

- **POST /load** 设置或替换活动配置
- **POST /stop** 停止活动配置并退出进程
- **GET /config/[path]** 导出指定路径的配置
- **POST /config/[path]** 设置或替换对象；追加到数组
- **PUT /config/[path]** 创建新对象或插入数组
- **PATCH /config/[path]** 替换现有对象或数组元素
- **DELETE /config/[path]** 删除指定路径的值
- **在 JSON 中使用@id** 轻松遍历配置结构
- **GET /reverse_proxy/upstreams** 返回配置的代理上游的当前状态
