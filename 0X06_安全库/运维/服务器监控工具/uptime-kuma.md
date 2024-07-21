<a name="b1OIT"></a>
## uptime-kuma介绍
uptime-kuma 是一款开源的、多功能的服务器监控面板。其功能类似于 Uptime Robot。但相较于其它同类工具，Uptime Kuma 支持自托管服务，且限制更少。Uptime-Kuma 是一个轻量的监控工具，并且有着非常好看的监控 Dashboard 面板，并且它支持丰富的告警方式，只需要花几分钟的时间就可以搭建一个监控应用，如果对这个项目有兴趣，可以在Github上面找到它。<br />Github地址：[https://github.com/louislam/uptime-kuma](https://github.com/louislam/uptime-kuma)
<a name="G46TS"></a>
### 功能介绍
支持的功能也相对较为丰富：

- 监控HTTP(s)/TCP/HTTP(s)关键字/Ping/DNS记录/推送/Steam Game Server等的正常运行时间。
- 支持丰富的通知渠道，如通过Telegram、Discord、Gotify、Slack、Pushover、电子邮件 (SMTP) 和70 多种通知服务发送通知，请单击此处查看完整列表。
- 20 second intervals.
- 支持多种语言
- 简洁的状态页面
- Ping图
- 监控证书信息
<a name="K6T13"></a>
## 安装
注：监控服务与被监控的服务不要放在同一个服务器上，并且监控服务重要等级应该为level 1。
<a name="DERSI"></a>
### Docker 安装
```bash
docker run -d --restart=always -p 3001:3001 -v uptime-kuma:/app/data --name uptime-kuma louislam/uptime-kuma:1
```
<a name="DX15q"></a>
### 通过docker-compose安装
```yaml
version: "3"
services: 
  uptime-kuma:
    image: louislam/uptime-kuma:1.11.4
    container_name: "uptime-kuma"
    restart: "always"
    volumes: 
      - "/etc/localtime:/etc/localtime"
      - "/data/uptimekuma:/app/data"
    ports: 
      - "3001:3001"
```
配置nginx(可选)
```nginx
server {
  server_name status.devopsman.cn;
  listen 443 ssl http2;
  ssl_certificate /www/server/panel/vhost/nginx/ssl/status.devopsman.cn_bundle.pem;
  ssl_certificate_key /www/server/panel/vhost/nginx/ssl/status.devopsman.cn.key;
  location / {
    proxy_pass http://127.0.0.1:3001;
    proxy_http_version 1.1;
    proxy_set_header Upgrade $http_upgrade;
    proxy_set_header Connection "upgrade";
  }
  access_log  /www/wwwlogs/uptimestatus.log main;
  error_log /www/wwwlogs/uptimestatus.error.log;
}
```
相比blackbox_exporter去监控url和证书，这个就显示很高大上了，花里胡哨的监控面板，有时候更适合给领导和客户欣赏。
<a name="vGuWb"></a>
## 使用
部署完毕后，可以通过http://ip:3001来访问uptime-kuma（注意安全组放行3001端口），首次访问需要设置管理员账号、密码，根据提示完成即可。进入后台后可根据需要添加对应类型的监控，支持TCP/PING/HTTP等。另外uptime-kuma还支持多种监控告警方式，根据需要进行设置即可。
<a name="LLcU7"></a>
## 监控docker容器
具体的使用方法就不细讲了，主要简单的说一下怎么对docker容器进行监控。<br />这里一般有两种配置方法，第一种是**使用docker socket实现对容器的监控**，这个最简单，但需要将/var/run/docker.sock（/run/docker.sock也行）挂在uptime-kuma的容器下，否则uptime-kuma无法使用/var/run/docker.sock监控容器。
```bash
docker run -d --restart=always -p 3001:3001 -v uptime-kuma:/app/data -v /run/docker.sock:/run/docker.sock --name uptime-kuma louislam/uptime-kuma:1
```
监控类型选择docker容器，输入待监控的docker 容器的id，这里选择监控uptime-kuma自己。然后是配置docker宿主信息，这里选择连接方式为socket，然后在守护进程的位置填入docker socket的位置，一般都只在/var/run/docker.sock下（填/run/docker.sock也行），点击测试，连接成功后就会出现右下角的连接成功。最后保存，就可以了。第二种方法就是**通过开放docker远程api的方式**（也就是tcp的连接方式）去监控docker容器的状态，这个方法的好处就在于可以监控不同服务器上的docker容器，一般这个时候都会有个但是，但是开放docker远程api会很危险，一定要注意，因为开放后，就意味着别人可以通过tcp请求直接远程控制Docker，这非常的危险，然后服务器就中毒了被用来挖矿，docker直接就成了毒窝，所以直接开放docker远程api这么干很危险，很危险，很危险（说三次），那就是想开放这么办，就是想监控其他服务器上的docker容器。方法是有的，想了几个方法：

1. 修改docker默认的远程的api端口
2. 端口不对外开放，组个内网（可配合方法1使用）
3. 其实前两个还是不安全，虽然这个api没有用户密码，但是它可以使用证书认证。但即使把证书认证配置好了（搜索docker远程api证书认证就有了），但是uptime-kuma的配置页面并没有指定宿主机docker远程api证书的选项，当然也有可能是没找到，所以还是组个内网吧，端口不对外开放
<a name="MVuHW"></a>
### 下面是具体的配置（非证书认证版）
首先。需要在/etc/docker/ 目录下编辑daemon.json文件，添加如下内容。
```json
#daemon.json里本来就有"registry-mirrors":["https://selwkcml.mirror.aliyuncs.com"]，所以这里只需要在daemon.json追加"hosts": ["unix:///var/run/docker.sock", "tcp://0.0.0.0:2375"]就好了，还有就是如果daemon.json有其他内容，一定不要忘记配置项后面的逗号
{       
  "hosts": ["unix:///var/run/docker.sock", "tcp://0.0.0.0:2375"],
  "registry-mirrors":["https://selwkcml.mirror.aliyuncs.com"]
}
```
完成上的步骤后，使用`systemctl edit docker.service`编辑/etc/systemd/system/docker.service.d/override.conf文件，一般给文件都是空的，要不就全都被注释了，添加如下内容：
```
[Service]
#The blank ExecStart is required to clear the current entry point
ExecStart=
#Replace the existing ExecStart but only remove the properties that you have added into the daemon.json file, leave all else the same.
ExecStart=/usr/bin/dockerd --containerd=/run/containerd/containerd.sock
```
如果一直保存失败，可能是因为/etc/systemd/system/下没有docker.service.d目录，可以手动建一个docker.service.d目录，然后编辑override.conf文件，搞定后重启docker进程。
```bash
systemctl restart docker.service
```
然后在uptime-kuma上配置，保存就ok了。<br />PS：使用docker远程api就不需要/var/run/docker.sock将挂载到uptime-kuma下，还有最后就是开放2375端口，建议是，最好不要开放端口，要不然就在内网内，还有就是证书鉴权。
