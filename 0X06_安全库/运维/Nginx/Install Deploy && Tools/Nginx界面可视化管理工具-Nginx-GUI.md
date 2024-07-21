Nginx
<a name="7xII5"></a>
### 需求
Nginx 可视化管理，例如

- 配置管理
- 性能监控
- 日志监控
- 其他配置
<a name="aQrTR"></a>
### 方案-Nginx-GUI
Nginx-GUI目前已实现前两条：配置管理，和性能监控。<br />日志分析监控这块还需要另找方案实现！
<a name="oYJqF"></a>
### Nginx-GUI的Github仓库地址
Nginx-GUI的Github地址：[https://github.com/onlyGuo/nginx-gui](https://github.com/onlyGuo/nginx-gui)
<a name="a5b5A"></a>
### Nginx-GUI安装配置
<a name="6RoLw"></a>
#### 1、下载并解压 Nginx-GUI-For-Linux-1.0.zip
<a name="w7RWr"></a>
#### 2、修改配置文件
文件位置：conf/conf.properties
```
# nginx 安装路径nginx.path = /usr/local/Cellar/nginx/1.15.12# nginx 配置文件全路径nginx.config = /Users/gsk/dev/apps/nginx-1.15.12/conf/nginx.conf# account.admin = admin
```
<a name="rDBPj"></a>
#### 3、重命名(此步骤仅linux版本需要)
根据原作者的描述 针对linux 64位版本 需要将 lib/bin/下的 java_vms 文件 重命名为 java_vms_nginx_gui
<a name="yMcFg"></a>
#### 4、在服务器上运行
前面的步骤都完成以后，直接打包发布到服务器
```bash
# 赋权
sudo chmod -R 777 nginx-gui/
# 后台启动
nohup bash /root/web/nginx-gui/startup.sh > logs/nginx-gui.out &
```
访问默认端口 8889 默认账号密码都是admin
<a name="8Gc4y"></a>
### Nginx-GUI界面演示截图
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1614912299368-add03b1f-128d-42c0-8a91-2e52add4e37b.png#averageHue=%23c1dbad&height=446&id=cXuHn&originHeight=733&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=657)<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1614912299376-4b00587a-fc7c-48e8-b8a6-20e6e6616fb1.png#averageHue=%23bea98b&height=723&id=hKr4l&originHeight=723&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=1080)![](https://cdn.nlark.com/yuque/0/2021/png/396745/1614912299507-ad449c7c-915c-41c9-a79e-5a5529e3d5e3.png#averageHue=%23bba78a&height=443&id=lxQw5&originHeight=729&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=657)![](https://cdn.nlark.com/yuque/0/2021/png/396745/1614912299416-3044077f-a815-4477-857e-347568bd3d23.png#averageHue=%23d7b59b&height=608&id=yAhin&originHeight=608&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=1080)
<a name="xzFKJ"></a>
### 说明
目前最新的版本为 V1.6，已实现不少新功能和修复了已知的所有 bug，推荐使用最新版本。
