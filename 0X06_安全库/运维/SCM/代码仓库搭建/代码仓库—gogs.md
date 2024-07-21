代码仓库
<a name="fSSyR"></a>
## 自己搭建代码仓库的好处

1. github 在国外，从国内正常访问的话，速度很慢。
2. gitee、codechina 在国内，访问速度倒是很快，但是单个仓库大小、仓库总数量都受平台限制。
3. 有时公司需要搭建自己的代码仓库

这时候，自己搭建代码仓库的好处就体现出来了。下面使用 Gogs 搭建自己的代码仓库。
<a name="uNMZg"></a>
## 下载二进制程序
[https://gogs.io/docs/installation/install_from_binary.html](https://gogs.io/docs/installation/install_from_binary.html)<br />根据自己的平台下载压缩包，这里选择的环境是 Linux amd64
```bash
wget https://dl.gogs.io/0.12.3/gogs_0.12.3_linux_amd64.tar.gz
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629993224229-82723e10-efe4-4fa1-ad6d-ef04386f9367.png#clientId=u8c6d5653-fbd8-4&from=paste&height=247&id=u135c83ab&originHeight=740&originWidth=3323&originalType=binary&ratio=1&size=872222&status=done&style=none&taskId=u57e877cd-d3fd-4a2d-9e4c-1ea3f34255b&width=1107.6666666666667)
<a name="Sxsc6"></a>
## 解压
```bash
tar xf gogs_0.12.3_linux_amd64.tar.gz
```
<a name="BTcOm"></a>
## 运行
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629993311408-fc99b960-8162-462c-b76f-ed7608f71f9a.png#clientId=u8c6d5653-fbd8-4&from=paste&height=469&id=u2aeaf980&originHeight=1406&originWidth=3323&originalType=binary&ratio=1&size=1621914&status=done&style=none&taskId=u5e3ed205-098c-424d-9a4b-55e88eb4e92&width=1107.6666666666667)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629993328766-6c23c863-b114-4f75-a2de-5cd6b226d1af.png#clientId=u8c6d5653-fbd8-4&from=paste&height=583&id=u495e2f10&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&size=2140624&status=done&style=none&taskId=u19e93545-f3cd-49cb-afa1-8904b359a28&width=1107.6666666666667)
<a name="tcD1n"></a>
## 从浏览器进行后续部署
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629993545843-32cf1f11-2d68-4c44-bab0-17fbc31ea372.png#clientId=u8c6d5653-fbd8-4&from=paste&height=600&id=u01efbb2c&originHeight=1801&originWidth=3840&originalType=binary&ratio=1&size=241521&status=done&style=shadow&taskId=ub4b2aafc-394e-4b7d-87dc-54a81474d70&width=1280)<br />可能出现的问题：浏览器无响应。<br />解决办法：关闭服务器防火墙。<br />![2021-08-26-23-57-02-929262.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629993430172-4926e3be-070c-4206-bf6b-2b126e86ec33.png#clientId=u8c6d5653-fbd8-4&from=ui&id=u905edc11&originHeight=78&originWidth=710&originalType=binary&ratio=1&size=10851&status=done&style=shadow&taskId=ucb439687-8c12-4fb9-99ca-680ab57988e)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629993388753-deaa746e-675e-4c46-8a44-fd6d2278a894.png#clientId=u8c6d5653-fbd8-4&from=paste&height=600&id=u3fc06d99&originHeight=1799&originWidth=3840&originalType=binary&ratio=1&size=321397&status=done&style=shadow&taskId=uad3464e3-9b33-470b-935e-5f1e50827b4&width=1280)<br />为了部署简单，数据库选用 sqlite3，这样就不需要安装任何数据库了，甚至 sqlite3 命令都不需要安装。
<a name="HLBlv"></a>
## 登录
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629993549092-5534ab62-2a63-43e4-b6c7-1c1090dd8351.png#clientId=u8c6d5653-fbd8-4&from=paste&height=600&id=ud83c73be&originHeight=1801&originWidth=3840&originalType=binary&ratio=1&size=241521&status=done&style=shadow&taskId=uc63e7438-ea86-4223-b77f-86a455d4f42&width=1280)
<a name="rNMe7"></a>
## 创建仓库并上传项目
<a name="bUNgj"></a>
## 开机自启
如下

1. 在 rc.local 中添加 gogs/gogs web > /dev/null 2>&1 &，结果不生效，这行命令应该是执行了（在其上面添加了一句测试命令，执行到了），只是没有真正把 gogs 启动起来。
2. 在 /etc/init.d/ 中添加自启脚本，链接到 rc[2-5].d 目录下，依旧不生效。

最后发现，其实，gogs 已经提供了相应开机启动服务脚本，不过需要根据自己的配置修改gogs/scripts/systemd/gogs.service
```bash
[root@izbp1gxt3epy9qvyrcc08sz systemd]# pwd
/root/gogs/scripts/systemd
[root@izbp1gxt3epy9qvyrcc08sz systemd]# cat gogs.service
[Unit]
Description=Gogs
After=syslog.target
After=network.target
After=mariadb.service mysqld.service postgresql.service memcached.service redis.service

[Service]
# Modify these two values and uncomment them if you have
# repos with lots of files and get an HTTP error 500 because
# of that
###
#LimitMEMLOCK=infinity
#LimitNOFILE=65535
Type=simple
User=git
Group=git
WorkingDirectory=/home/git/gogs
ExecStart=/home/git/gogs/gogs web
Restart=always
Environment=USER=git HOME=/home/git

# Some distributions may not support these hardening directives. If you cannot start the service due
# to an unknown option, comment out the ones not supported by your version of systemd.
ProtectSystem=full
PrivateDevices=yes
PrivateTmp=yes
NoNewPrivileges=true

[Install]
WantedBy=multi-user.target
```
拷贝到 /usr/lib/systemd/system，并使能服务，重启<br />![2021-08-26-23-57-03-386256.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629993642223-bea56fbb-3ee7-40fe-8fdf-63c5d9b86bd2.png#clientId=u8c6d5653-fbd8-4&from=ui&id=lH5Ue&originHeight=97&originWidth=709&originalType=binary&ratio=1&size=21232&status=done&style=none&taskId=ue6f3ec3b-2f5b-4688-a19c-bbaa10a5903)<br />查看 gogs 是否启动<br />![2021-08-26-23-57-03-477257.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629993642216-b0a8aeb9-9f73-4645-a037-4f232d136182.png#clientId=u8c6d5653-fbd8-4&from=ui&id=ysZKM&originHeight=102&originWidth=703&originalType=binary&ratio=1&size=8214&status=done&style=shadow&taskId=uc976b838-fac8-40f9-a624-d93c2fd5342)<br />看到 gogs 已经自启了，浏览器也可以正常访问。
