<a name="I6Mbr"></a>
## 从压缩包下载安装
<a name="zl8vj"></a>
### 1、下载Docker 安装包
在Docker官网下载所需要的Docker版本<br />[https://download.docker.com/linux/static/stable/x86_64/](https://download.docker.com/linux/static/stable/x86_64/)
<a name="bVdOG"></a>
### 2、安装
```bash
// 解压安装包
tar -xvf docker-20.10.8.tgz
// 将文件移动到安装目录
cp docker/* /usr/bin/
```
<a name="AP7vx"></a>
### 3、修改docker.service
```bash
// 将docker注册为服务
vim /etc/systemd/system/docker.service
```
```toml
[Unit]
Description=Docker Application Container Engine
Documentation=https://docs.docker.com
After=network-online.target firewalld.service
Wants=network-online.target

[Service]
Type=notify

# the default is not to use systemd for cgroups because the delegate issues still
# exists and systemd currently does not support the cgroup feature set required
# for containers run by docker
ExecStart=/usr/bin/dockerd
ExecReload=/bin/kill -s HUP $MAINPID

# Having non-zero Limit*s causes performance problems due to accounting overhead
# in the kernel. We recommend using cgroups to do container-local accounting.
LimitNOFILE=infinity
LimitNPROC=infinity
LimitCORE=infinity

# Uncomment TasksMax if your systemd version supports it.
# Only systemd 226 and above support this version.
#TasksMax=infinity
TimeoutStartSec=0

# set delegate yes so that systemd does not reset the cgroups of docker containers
Delegate=yes

# kill only the docker process, not all processes in the cgroup
KillMode=process

# restart the docker process if it exits prematurely
Restart=on-failure
StartLimitBurst=3
StartLimitInterval=60s

[Install]
WantedBy=multi-user.target
```
<a name="RDGSH"></a>
### 4、启动
```bash
#添加文件权限并启动docker
chmod +x /etc/systemd/system/docker.service 
#重载配置文件
systemctl daemon-reload 
#启动Docker
systemctl start docker 
#设置开机自启
systemctl enable docker.service
```
<a name="mbDJ6"></a>
### 5、验证
```bash
#查看Docker状态
systemctl status docker 
#查看Docker版本
docker -v
#下载hello-world镜像
docker pull hello-world
```
<a name="zZFMN"></a>
## 从rpm包下载安装
<a name="fRauP"></a>
### 方式一：从官网下载Docker离线安装包
Linux版本下载地址：[https://download.docker.com/linux/centos/7/x86_64/stable/Packages/](https://download.docker.com/linux/centos/7/x86_64/stable/Packages/)<br />Windows版本下载地址：[https://download.docker.com/win/static/stable/x86_64/](https://download.docker.com/win/static/stable/x86_64/)<br />根据自己的系统版本下载下面的安装包，查看云主机内核版本
```bash
uname -r
```
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1696815435760-37927dd1-d2bf-4a7b-9ac6-451ad6c7b8db.png#averageHue=%233d3d3d&clientId=ue3db4993-2210-4&from=paste&height=83&id=u7cdd3cf4&originHeight=208&originWidth=1106&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=78293&status=done&style=none&taskId=uee388f3d-2d33-4e76-b6d1-153cd35dde6&title=&width=442.4)<br />根据内核版本，下载Docker离线安装包<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1696815545965-1d79ddd9-aefd-4bd3-9b4e-8b9452db7534.png#averageHue=%23fffefe&clientId=ue3db4993-2210-4&from=paste&height=76&id=ue81fb25f&originHeight=189&originWidth=931&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=35399&status=done&style=none&taskId=uc2c34c87-e291-426a-8bb7-47e06897e6f&title=&width=372.4)
<a name="Z0PiI"></a>
### 方式二：使用yum下载Docker离线安装包
添加docker repo
```bash
yum-config-manager --add-repo https://download.docker.com/linux/centos/docker-ce.repo
```
```bash
yumdownloader --resolve --destdir /home/yum/docker-ce docker-ce
```
<a name="zrW4V"></a>
### 安装上序下载的rpm包
```bash
rpm -ivh *.rpm
```
<a name="oF8yc"></a>
### 安装完成之后，检测是否安装成功，查看Docker版本
```bash
docker --version
```
