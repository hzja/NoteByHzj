Nginx<br />介绍一款 Nginx 可视化管理界面，非常好用，小白也能立马上手。<br />nginx-proxy-manager 是一个反向代理管理系统，它基于 NGINX，具有漂亮干净的 Web UI。还可以获得受信任的 SSL 证书，并通过单独的配置、自定义和入侵保护来管理多个代理。它是开源的，斩获 11.8K 的 Star 数。
<a name="jbg7W"></a>
## 特征

- 基于 Tabler([https://tabler.github.io/](https://tabler.github.io/)) 的美观安全的管理界面
- 无需了解 Nginx 即可轻松创建转发域、重定向、流和 404 主机
- 使用 Let's Encrypt 的免费 SSL 或提供您自己的自定义 SSL 证书
- 主机的访问列表和基本 HTTP 身份验证
- 高级 Nginx 配置可供超级用户使用
- 用户管理、权限和审核日志
<a name="qN8Rm"></a>
## 安装
<a name="MA03S"></a>
### 1、安装 Docker 和 Docker-Compose
<a name="ztTj1"></a>
### 2、创建一个docker-compose.yml文件
```yaml
version: '3'
services:
  app:
    image: 'jc21/nginx-proxy-manager:latest'
    restart: unless-stopped
    ports:
      - '80:80'
      - '81:81'
      - '443:443'
    volumes:
      - ./data:/data
      - ./letsencrypt:/etc/letsencrypt
```
<a name="CGEZN"></a>
### 3、运行
```bash
docker-compose up -d

#如果使用的是 docker-compose-plugin
docker compose up -d
```
<a name="tXeK6"></a>
### 4、访问网页
运行成功后，访问 http://127.0.0.1:81 就能看到界面啦<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679142697313-eee42de4-2b26-4abc-ac04-b89928baf8cd.png#averageHue=%23fdfcfc&clientId=uaab0875f-56e6-4&from=paste&id=ucaba08ca&originHeight=469&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud7b6eb69-e15c-4a08-a377-d4acd9d6a8a&title=)
<a name="s29OC"></a>
### 5、登录
网站默认账号和密码为
```
账号：admin@example.com
密码：changeme
```
登录成功后第一次要求修改密码，按照步骤修改即可！
<a name="zAl1X"></a>
### 6、登录成功主界面
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679142698286-e760ad93-42f3-4118-a311-92f8c1978f8e.png#averageHue=%2373c224&clientId=uaab0875f-56e6-4&from=paste&id=ub6b06a87&originHeight=487&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc097a7c3-fe04-47d1-9996-676e8a100b6&title=)
<a name="knyY3"></a>
## 实战：设置后台管理界面的反向代理
这里，就用 http://a.test.com 来绑定端口号为81的后台管理界面，实现浏览器输入 http://a.test.com 即可访问后台管理界面，并且设置HTTPS。
<a name="FRHwE"></a>
### 1、前提

- 安装好Nginx Proxy Manager
- 拥有一个域名
- 将 http://a.test.com 解析到安装Nginx Proxy Manager的服务器ip地址上
<a name="nlAnk"></a>
### 2、反向代理操作
先用ip:81 访问后台管理界面，然后输入账号密码进入后台。<br />点击绿色图标的选项<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679142697320-729eacc8-77d7-477b-b6ee-2358aab2c75d.png#averageHue=%2373c224&clientId=uaab0875f-56e6-4&from=paste&id=u34619aa8&originHeight=487&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u92c9037f-4ddd-4d1b-87dc-38689bd8f72&title=)<br />点击右边Add Proxy Host ，在弹出的界面Details选项中填写相应的字段。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679142697329-e4e2ba8a-7b52-4050-99dd-1ebdce839649.png#averageHue=%23b2b1b1&clientId=uaab0875f-56e6-4&from=paste&id=u13d14ed5&originHeight=487&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ucfb266ec-5c57-49f0-a42d-e3f975dceb9&title=)

- **Domain Names**: 填写要反向代理的域名，这里就是http://a.test.com
- **Forward Hostname / IP**: 填写的ip值见下文解释
- **Forward Port**: 反向代理的端口，这里就是81
- **Block Common Exploits**: 开启后阻止一些常见漏洞
- 其余两个暂不知作用

**Forward Hostname / IP填写说明**<br />如果搭建的服务和nginx proxy manager服务所在不是一个服务器，则填写能访问对应服务的IP。如果都在同一台服务器上，则填写在服务器中输入`ip addr show docker0` 命令获取得到的ip。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679142697333-4b47e0f8-d419-4e0b-b7fe-bd287e0d0062.png#averageHue=%23191b1e&clientId=uaab0875f-56e6-4&from=paste&id=uf72e3203&originHeight=229&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf00c995e-e33c-43c5-9b79-26b92f696f8&title=)<br />这里不填127.0.0.1的原因是使用的是docker容器搭建web应用，docker容器和宿主机即服务器不在同一个网络下，所以127.0.0.1并不能访问到宿主机，而ip addr show docker0获得的ip地址就是宿主机地址。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679142697655-7e317f1f-182e-4b2f-bbf1-1e0cb313c5a4.png#averageHue=%23fefefe&clientId=uaab0875f-56e6-4&from=paste&id=u46bf6a4b&originHeight=721&originWidth=688&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u61df88f5-d53a-42f7-918d-a7e15af4874&title=)<br />接下来即可用a.test.com 访问后台管理界面，此时还只是http协议，没有https。不过此时就可以把之前的81端口关闭了，输入a.test.com 访问的是服务器80端口，然后在转发给内部的81端口。
<a name="HEnKk"></a>
### 3、申请ssl证书
申请一个a.test.com 证书，这样就可以提供https访问了。<br />在Nginx Proxy Manager管理后台，选择Access Lists->Add SSL Certificate->Let's Encrypt选项。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679142697699-006d67e1-18c5-4470-9622-8432c67ad1a4.png#averageHue=%23e3d9ca&clientId=uaab0875f-56e6-4&from=paste&id=uca69c73a&originHeight=487&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u862b0805-41d9-4da1-b932-3dd1615a784&title=)<br />按照下图方式填写，点击Save就可以了<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679142697733-97c5a892-38bb-468c-9950-93e5c5617f90.png#averageHue=%23706b63&clientId=uaab0875f-56e6-4&from=paste&id=u17736fc7&originHeight=487&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua9222024-f4d3-4eb0-b3b2-0c2082b3967&title=)
<a name="uvkgS"></a>
### 4、设置HTTPS
进入反向代理设置界面，编辑上文创建的反代服务，选择SSL选项，下拉菜单中选择申请的证书，然后可以勾选Force SSL即强制HTTPS。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679142697737-9e8a2a0d-b9b0-479d-b3f7-e21416d90686.png#averageHue=%23a1a1a1&clientId=uaab0875f-56e6-4&from=paste&id=u39673b34&originHeight=487&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u18cf3d68-afe5-4447-9a7a-d1f9af3be8c&title=)
<a name="G9wnC"></a>
## 总结
以上就是本教程的全部内容，更多的使用教程，大家可以访问官方文档。<br />官方文档：[https://nginxproxymanager.com/guide/](https://nginxproxymanager.com/guide/)<br />开源地址：[_https://github.com/NginxProxyManager/nginx-proxy-manager_](https://github.com/NginxProxyManager/nginx-proxy-manager)
