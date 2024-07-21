推荐一款开源的代码部署工具，它具有简单、高效、易用等特点，可以提高团队的工作效率。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690957074932-abce703e-cde4-4160-87f6-ccc58ddec2d8.png#averageHue=%23fefefe&clientId=u84260640-6920-4&from=paste&id=uc1f528de&originHeight=163&originWidth=376&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u44cf4c5b-c510-42b5-b4b7-384d57dee49&title=)<br />官网地址：[https://gitee.com/dreamans/syncd/](https://gitee.com/dreamans/syncd/)<br />**目前只支持类Linux系统**
<a name="b1nU7"></a>
## 特性

- Go语言开发，编译简单、运行高效
- Web界面访问，交互友好
- 灵活的角色权限配置
- 支持Git仓库
- 分支、tag上线
- 部署hook支持
- 完善的上线工作流
- 邮件通知机制
<a name="hVHOO"></a>
## 原理
![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690957074941-2ec7f128-46b9-43ba-8e20-1e17e2cc2fe1.jpeg#averageHue=%23f3f3f2&clientId=u84260640-6920-4&from=paste&id=u02f8b8e9&originHeight=674&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf43929d9-b940-49da-b7e3-9fdb183b693&title=)
<a name="zq0bH"></a>
### Git
Syncd服务通过git-ssh(或password)方式从仓库中拉取指定tag(分支)代码。
<a name="mTUcP"></a>
### 构建
运行配置好的构建脚本，编译成可上线的软件包<br />在这一环节中，可运行单元测试 (例如 go test php phpunit，下载依赖 (如 go: glide install php: composer install)，编译软件包 (如 js: npm build go: go build xx.go java: javac xx.java c: cc xx.c) 等。
<a name="u85CG"></a>
### 分发
通过 scp 命令分发软件包到各机房生产服务器的临时目录，远程执行 pre-deploy 配置的命令，执行完毕后解压缩软件包到目标目录，然后执行 post-deploy 命令<br />分发上线过程是串行执行，并且任意步骤执行失败整个上线单会终止上线并将状态置为上线失败，需要点击 **再次上线** 重试。<br />将来会支持同一集群服务器并行执行，集群之间串行发布的特性。
<a name="J41VR"></a>
### SSH信任
生产服务器与部署服务器之间通过ssh-key建立信任<br />配置方法请参考 秘钥配置 章节
<a name="JlVqj"></a>
## 安装
<a name="gokXz"></a>
### 准备工作

- Go

推荐Go1.10以上版本，用来编译源代码

- Git

**请保持部署Syncd服务器的git版本为最新(>=2.20)**

- Nginx

Web服务依赖Nginx

- MySQL

系统依赖Mysql存储持久化数据，推荐版本 MySQL 5.7

- Linux + Bash

系统会使用到 git, ssh, scp 等命令，所以目前只推荐在Linux上使用，并且需要提前安装或更新这些命令道最新版本

- 秘钥配置

由于部署服务器(Syncd服务所在的服务器)与生产服务器(代码部署目标机)之间通过ssh协议通信，所以需要将部署机的公钥 (一般在这里: ~/.ssh/id_rsa.pub)加入到生产机的信任列表中(一般在这里 ~/.ssh/authorized_keys)<br />可使用 ssh-copy-id 命令添加，或手动拷贝. 拷贝后不要忘记进行测试连通性 `ssh {生产机用户名}@{生产机地址}`<br />最后建议将以下配置加入到部署服务器ssh配置/etc/ssh/ssh_config中，关闭公钥摘要的显示提示，防止后台脚本运行失败
```
Host *
    StrictHostKeyChecking no
```
请注意: ssh目录权限需按此设置，否则会出现无法免密登录的情况
```
~/.ssh  0700
~/.ssh/authorized_keys 0600
```
<a name="oGek1"></a>
### 安装

- 二进制包安装

二进制包下载地址：[https://github.com/dreamans/syncd/releases](https://github.com/dreamans/syncd/releases)

- 源码编译安装`curl https://raw.githubusercontent.com/dreamans/syncd/master/install.sh | bash`

当前路径中若生成 syncd-deploy 或者 syncd-deploy-xxx 目录则表明安装成功<br />生成的 syncd-deploy 目录可拷贝或移动到你想要的地方，但不要试图将此目录拷贝到其他服务器上运行，会造成不可预料的结果。

- 数据库依赖

需要将 github.com/dreamans/syncd/syncd.sql 数据表结构和数据导入到MySQL数据库中

- 修改配置文件

修改 syncd-deploy/etc/syncd.ini 中相关配置信息，具体配置描述可参考注释

- 启动服务
```
cd syncd-deploy

➜  syncd-deploy ./bin/syncd -c ./etc/syncd.ini
                                          __
   _____   __  __   ____     _____   ____/ /
  / ___/  / / / /  / __ \   / ___/  / __  /
 (__  )  / /_/ /  / / / /  / /__   / /_/ /
/____/   \__, /  /_/ /_/   \___/   \__,_/
        /____/

Service:              syncd
Version:              1.0.0
Config Loaded:        ./etc/syncd.ini
Log:                  stdout
Database:             127.0.0.1
Mail Enable:          0
HTTP Service:         :8868
Start Running...
```

- 添加Nginx配置
```nginx
upstream syncdServer {
  server 127.0.0.1:8868 weight=1;
}
server {
  listen       80;
  server_name  deploy.syncd.cc; # 此处替换成你的真实域名
  access_log   logs/deploy.syncd.cc.log;

  location / {
    try_files $uri $uri/ /index.html;
    root /path/syncd-deploy/public; # 此处/path请替换成真实路径
    index index.html index.htm;
  }

  location ^~ /api/ {
    proxy_pass          http://syncdServer;
    proxy_set_header    X-Forwarded-Host $host:$server_port;
    proxy_set_header    X-Real-IP     $remote_addr;
    proxy_set_header    Origin        $host:$server_port;
    proxy_set_header    Referer       $host:$server_port;
  }
}
```
重启Nginx服务
<a name="k6OI3"></a>
### 修改hosts
若域名未解析，可修改hosts进行临时解析
```bash
sudo vim /etc/hosts

127.0.0.1  deploy.syncd.cc;
```
<a name="HdmxW"></a>
### 安装完成
打开浏览器，访问 [http://deploy.syncd.cc](http://deploy.syncd.cc)<br />初始账号:
> 用户名: syncd
> 邮箱: syncd@syncd.cc
> 密码: syncd.cc

**!!!登录后请尽快修改密码**
<a name="JDTVG"></a>
## 使用
<a name="eMjOB"></a>
### 系统使用流程图
![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690957074875-d45bf34d-611b-4283-8e58-6b64e0595a96.jpeg#averageHue=%23fafaf6&clientId=u84260640-6920-4&from=paste&id=ua45a3c1a&originHeight=884&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8c39f023-9b82-4c56-bed1-a56064a890d&title=)
<a name="tBqTp"></a>
### 使用截图
| ![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690957074907-ab5b772e-cdc3-48b0-ad55-b0f9f68192f1.jpeg#averageHue=%23e1e6e1&clientId=u84260640-6920-4&from=paste&id=u96c41a62&originHeight=801&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u08916366-e345-42cb-92f9-bb433263a1a&title=) | ![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690957074943-c9b1a4da-a155-4a82-8b94-4de7edf9a844.jpeg#averageHue=%23fdfefb&clientId=u84260640-6920-4&from=paste&id=uee7760ab&originHeight=680&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0a1599a1-09f1-4c2c-b381-9d37ba584ef&title=) | ![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690957075227-97d4ab73-0a2b-4910-8bcb-80a076339c31.jpeg#averageHue=%235d5f5c&clientId=u84260640-6920-4&from=paste&id=u2263cbb3&originHeight=570&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8004c86c-980d-4c50-9a7d-659c2a47f55&title=) |
| --- | --- | --- |
| ![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690957075272-81944893-9391-4d04-a534-dba99c4dfbff.jpeg#averageHue=%23fefdfa&clientId=u84260640-6920-4&from=paste&id=u82b11c89&originHeight=622&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0cb2843e-1590-4c74-8d0f-8e9cb272904&title=) | ![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690957075356-fe5122ce-9faf-4541-90de-0431895ddde0.jpeg#averageHue=%2360635f&clientId=u84260640-6920-4&from=paste&id=u50bf217e&originHeight=595&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u215c775a-e1c8-4c9b-8dee-2a94a085548&title=) | ![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690957075377-ac2645d9-393b-420e-9489-6f1d646ca208.jpeg#averageHue=%23a3a39e&clientId=u84260640-6920-4&from=paste&id=ub8021342&originHeight=732&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4d21d28e-b1b1-4601-9007-d5a04ede5d0&title=) |
| ![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690957075566-b293f7a0-8c56-466f-b193-127484392db2.jpeg#averageHue=%23aeb0a9&clientId=u84260640-6920-4&from=paste&id=ub9ec4a47&originHeight=1254&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u98851329-258b-42dc-8c9e-581eb17198d&title=) | 

 |  |

