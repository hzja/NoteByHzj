Nginx<br />NginxWebUI是一个 Nginx 网页管理工具，使用网页来快速配置 Nginx 单机与集群。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661268348322-99102351-a409-4ef1-93c3-8f51f2c45c6c.png#clientId=u6d852763-0450-4&from=paste&id=u276e2ff2&originHeight=501&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u08392ebe-27b5-4956-8fe3-32e4bc00611&title=)
<a name="kZR8R"></a>
### **项目介绍**
NginxWebUI是一款图形化管理 Nginx  配置的工具，可以使用网页来快速配置 Nginx 的各项功能，包括 http 协议转发，tcp 协议转发，反向代理，负载均衡，静态 html 服务器，ssl 证书自动申请、续签、配置等，配置好后可一键生成 nginx.conf 文件，同时可控制 Nginx 使用此文件进行启动与重载，完成对 Nginx 的图形化控制闭环。<br />它也可管理多个 Nginx 服务器集群，随时一键切换到对应服务器上进行 Nginx 配置，也可以一键将某台服务器配置同步到其他服务器，方便集群管理。
<a name="X9loK"></a>
### **功能说明**
本项目是基于 solon 的 web 系统，数据库使用 h2，因此服务器上不需要安装任何数据库。
<a name="lZOCW"></a>
#### 系统运行状态
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661268348327-3f25a54a-c771-4e79-a9b8-4e164d915ad1.png#clientId=u6d852763-0450-4&from=paste&id=u1e00664d&originHeight=490&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8461f6cf-512c-4938-8a78-546b87810eb&title=)<br />通过查看运行状态可以查看 CPU，内存，磁盘占用率。
<a name="VKier"></a>
#### 基本参数配置
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661268348408-41040072-d9a6-40f3-8889-74e35e6e4548.png#clientId=u6d852763-0450-4&from=paste&id=ucd358cea&originHeight=521&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u56a043a2-7639-4c53-9c52-828b69effb3&title=)
<a name="FmasK"></a>
#### http 参数配置
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661268348331-99970948-ebf4-4aff-ad4a-ab13632364ed.png#clientId=u6d852763-0450-4&from=paste&id=u648a11ff&originHeight=429&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u51fab125-97a8-4763-888e-f9720283570&title=)
<a name="LAwlX"></a>
#### 反向代理
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661268348326-cd20f0eb-e48a-404e-910e-bad10d8df1d6.png#clientId=u6d852763-0450-4&from=paste&id=u2eb6bacf&originHeight=317&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u63a948f9-35c2-49a5-b824-a1ff47dea68&title=)
<a name="ShMPw"></a>
#### Stream参数配置
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661268348634-ad5b738a-8666-485a-8f79-d7d26e3c50ce.png#clientId=u6d852763-0450-4&from=paste&id=uc1458740&originHeight=358&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7fe2669c-e53e-487d-9355-0026e205745&title=)
<a name="pUqWu"></a>
#### 负载均衡(upstream)
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661268348652-715ce65b-b084-4d9e-b3d4-5626a3ab1db4.png#clientId=u6d852763-0450-4&from=paste&id=ub14a0c19&originHeight=349&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue5e43d63-f60e-4101-8297-bb6d914e1ff&title=)
<a name="B5sUL"></a>
### **安装**
<a name="fqSHg"></a>
#### 1、安装docker容器环境

- **Ubuntu：**
```bash
apt install docker.io
```

- **Centos：**
```bash
yum install docker
```
<a name="sxwEc"></a>
#### 2、拉取镜像：
```bash
docker pull cym1102/nginxwebui:latest
```
<a name="S0qUf"></a>
#### 3、启动容器：
```bash
docker run -itd \
  -v /home/nginxWebUI:/home/nginxWebUI \
  -e BOOT_OPTIONS="--server.port=8080" \
  --privileged=true \
  --net=host \
  cym1102/nginxwebui:latest
```
注意：启动容器时请使用`--net=host`参数，直接映射本机端口，因为内部Nginx可能使用任意一个端口，所以必须映射本机所有端口。<br />容器需要映射路径/home/nginxWebUI:/home/nginxWebUI，此路径下存放项目所有数据文件，包括数据库，Nginx配置文件，日志，证书等，升级镜像时，此目录可保证项目数据不丢失，请注意备份。<br />`-e BOOT_OPTIONS` 参数可填充java启动参数，可以靠此项参数修改端口号<br />`--server.port` 占用端口，不填默认以8080端口启动<br />日志默认存放在/home/nginxWebUI/log/nginxWebUI.log 另：使用docker-compose时配置文件如下
```yaml
version: "3.2"
services:
  nginxWebUi-server:
    image: cym1102/nginxwebui:latest
    volumes:
      - type: bind
        source: "/home/nginxWebUI"
        target: "/home/nginxWebUI"
    environment:
      BOOT_OPTIONS: "--server.port=8080"
    privileged: true
    network_mode: "host"
```
<a name="WJ6jv"></a>
### **项目地址**
[https://gitee.com/cym1102/nginxWebUI](https://gitee.com/cym1102/nginxWebUI)
